#include "StdAfx.h"
#include "PCI2330Device.h"
#include "ConfigurationManager.h"
#include "Logger.h"
#include "PCIE2330.h"
#include "resource.h"

extern ConfigManager  gConfigMgr;
extern CLogger        gLogger;


CPCI2330Device::CPCI2330Device(void)
	:m_exitFlag(0),
	SIGNAL_BUFFER_SZ(32), // 32
	m_szSignalArray(0),
	m_bIoCardReady(false),
	m_hIoCard(NULL),
	m_hIoEvent(NULL)
{
	auto ptrConfig = gConfigMgr.GetChannelConfigPtr();
	for(auto it = ptrConfig->begin(); it != ptrConfig->end(); it++)
	{
		m_dictSignal[it->first]		        = new bounded_buffer<NG_IMG_SIGNAL>(SIGNAL_BUFFER_SZ);
		m_dictFiredSignal[it->first]		= new bounded_buffer<NG_IMG_SIGNAL>(SIGNAL_BUFFER_SZ);
		m_szSignalArray++;
	}
	::memset((void*)m_firedTime, 0x00, sizeof(LARGE_INTEGER)*16);
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	for(int i = 0; i < 16; i++)
	{
		m_firedTime[i] = now;
	}
}

CPCI2330Device::~CPCI2330Device(void)
{
	if(m_bIoCardReady)
	{
		PCIE2330_ReleaseSystemEvent(m_hIoEvent);
		PCIE2330_ReleaseDeviceInt(m_hIoCard);	// �ͷ��ж���Դ
		PCIE2330_ReleaseDevice(m_hIoCard);		// �ͷ��豸����
	}

	for(auto it = m_dictSignal.begin(); it != m_dictSignal.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_dictSignal.clear();

	for(auto it = m_dictFiredSignal.begin(); it != m_dictFiredSignal.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_dictFiredSignal.clear();
}

bool CPCI2330Device::CreatePCIDevice()
{
	try
	{
		m_hIoCard = PCIE2330_CreateDevice(0);
		if(m_hIoCard == INVALID_HANDLE_VALUE)
		{
			CString strID;
			strID.LoadString(IDS_STRING_ERR_ERRID);
			strID += _T("101");
			CString sMsg;
			sMsg.LoadString(IDS_STRING_PCI_INIT_ERROR);
			sMsg.Replace(_T("#"),_T("2330"));
			CString streMsg = strID + _T("---") + sMsg;//
			ERROR_INFO  pciError(PCI_CREATE_FAILED_ERROR,1,streMsg.GetString());
			m_errorInfo = pciError;
			return false;
		}
		m_hIoEvent = PCIE2330_CreateSystemEvent(); // �����ж��¼����
		if(m_hIoEvent == INVALID_HANDLE_VALUE)
		{
			m_errorInfo.m_nChIndex = 0;
			m_errorInfo.m_level = WL_ERROR;
			m_errorInfo.m_errorSubID   = 0;
			m_errorInfo.m_strMsg = L"PCI2312:Create system event failed!";
			return false;
		}

		PCIE2330_PARA_INT IntPara; // �����жϵ��ź�Դȷ��(ֻ��)
		PCIE2330_INT_MODE IntMode;

		IntPara.bINTDI0  = TRUE;
		IntPara.bINTDI8  = TRUE;
		IntPara.bINTDI16 = TRUE;
		IntPara.bINTDI24 = TRUE;

		IntMode.bINTModeDI0  = FALSE;
		IntMode.bINTModeDI8  = FALSE;
		IntMode.bINTModeDI16 = FALSE;
		IntMode.bINTModeDI24 = FALSE;

		if(!PCIE2330_InitDeviceInt(m_hIoCard, m_hIoEvent,&IntPara,&IntMode))
		{
			m_errorInfo.m_nChIndex = 0;
			m_errorInfo.m_level = WL_ERROR;
			m_errorInfo.m_errorSubID   = 0;
			m_errorInfo.m_strMsg = L"PCI2312:Initialize device failed!";
			return false;
		}
		m_bIoCardReady = true;
		return m_bIoCardReady;
	}
	catch(...)
	{
		m_errorInfo.m_nChIndex = 0;
		m_errorInfo.m_level = WL_ERROR;
		m_errorInfo.m_errorSubID   = 0;
		m_errorInfo.m_strMsg = L"PCI2312:unknown exception!";
		return false;
	}
	return m_bIoCardReady;
}

void CPCI2330Device::Start()
{
	if(m_bIoCardReady)
	{
		QueryPerformanceFrequency(&m_freq);
		StartTriggerSignalThread();
	}
	else
	{
		//TODO:
	}
}

void CPCI2330Device::Stop()
{
	::InterlockedIncrement(&m_exitFlag);
	Sleep(10);
	::memset((void*)m_ioBuff, IO_OFF, sizeof(BYTE) * 32);
	PCIE2330_SetDeviceDO(m_hIoCard,m_ioBuff);
}

void CPCI2330Device::StartTriggerSignalThread()
{
	UINT threadId;
	RejectorProc.MemProc = &CPCI2330Device::TriggerSignalThreadFunc;
	m_hTriggerThread = (HANDLE)_beginthreadex(NULL, 0, RejectorProc.ThreadProc, (LPVOID)this, 0, &threadId);
	// #ifdef PCC_PRODUCT
	// 	::SetThreadPriority(m_hTriggerThread, THREAD_PRIORITY_ABOVE_NORMAL);
	// #else
	// 	::SetThreadPriority(m_hTriggerThread, THREAD_PRIORITY_HIGHEST);
	// #endif
}

bool CPCI2330Device::GetRejectorStatus()
{
	return m_bIoCardReady;
}

void CPCI2330Device::WritePort(int portNum, BYTE st)
{
	if(!m_bIoCardReady)
		return;
	m_ioBuff[portNum] = st;
	BOOL ret = PCIE2330_SetDeviceDO(m_hIoCard,m_ioBuff);
	if(ret == FALSE)
	{
		//TODO
	}
}

// long CPCI2330Device::GetCrtPulseCount()
// {
// 	if(m_bIoCardReady)
// 	{
// 		return PCIE2330_GetDeviceIntCount(m_hIoCard);
// 	}
// 	return -1;
// }

// BYTE CPCI2330Device::ReadPort(int portNum)
// {
// 	if(!m_bIoCardReady)
// 	{
// 		return IO_OFF;
// 	}
// 
// 	BOOL ret = PCIE2330_GetDeviceDI(m_hIoCard,m_ioBuff);
// 	if(ret == FALSE)
// 	{
// 		//TODO
// 		return IO_OFF;
// 	}
// 	return m_ioBuff[portNum];
// }

void CPCI2330Device::HandleTriggeredSignal(int arrayId)
{
	if(m_dictFiredSignal[arrayId]->is_empty())
	{
		return;
	}

	NG_IMG_SIGNAL signal;
	m_dictFiredSignal[arrayId]->pop_back(&signal);

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double delta = (now.QuadPart - signal.m_bgn.QuadPart) * 1000.0f / m_freq.QuadPart;

	if(delta >= 2)
	{
		m_ioBuff[signal.m_nPort] = IO_OFF;
		PCIE2330_SetDeviceDO(m_hIoCard,m_ioBuff);
	}
	else
	{
		if(m_dictFiredSignal[arrayId]->is_not_full())
		{
			m_dictFiredSignal[arrayId]->push_front(signal);
		}
	}
}

void CPCI2330Device::HandleToBeTriggeredSignal(int arrayId)
{
	if(m_dictSignal[arrayId]->is_empty())
	{
		return;
	}

	NG_IMG_SIGNAL signal;
	m_dictSignal[arrayId]->pop_back(&signal);

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double delta = 0;

	delta = now.QuadPart * 1000.0f / m_freq.QuadPart - signal.m_arrivetime;
	if(delta < 0)
	{
		m_dictSignal[arrayId]->push_front(signal);
		return;
	}

	if((now.QuadPart - m_firedTime[signal.m_nPort].QuadPart) * 1000.0f / m_freq.QuadPart <= 4)
	{
		m_dictSignal[arrayId]->push_front(signal);
		return;
	}
	m_ioBuff[signal.m_nPort] = IO_ON;
	PCIE2330_SetDeviceDO(m_hIoCard,m_ioBuff);
	signal.m_bgn = now;
	m_firedTime[signal.m_nPort] = now;

	if(m_dictFiredSignal[arrayId]->is_not_full())
	{
		m_dictFiredSignal[arrayId]->push_front(signal);
	}
}

bool CPCI2330Device::IsSignalAvailable()
{
	int index = m_szSignalArray;
	while(index >= 1)
	{
		if(m_dictSignal[index]->is_not_empty())
		{
			return true;
		}
		if(m_dictFiredSignal[index]->is_not_empty())
		{
			return true;
		}
		index--;
	}
	return false;
}

unsigned int CPCI2330Device::TriggerSignalThreadFunc()
{
	while(!m_exitFlag)
	{
		if(m_exitFlag)
			break;
		if(!IsSignalAvailable())
		{
			Sleep(5);
			continue;
		}
		else
		{
			int arrayId = m_szSignalArray;
			while(arrayId >= 1)
			{
				auto pSignalArray       = m_dictSignal[arrayId];
				auto pFiredSignalArray  = m_dictFiredSignal[arrayId];
				bool bRawArrayEmpty     = pSignalArray->is_empty();
				bool bFiredArrayEmpty   = pFiredSignalArray->is_empty();
				if(bRawArrayEmpty && bFiredArrayEmpty)
				{
					arrayId--;
					continue;
				}
				// Fired signal
				HandleTriggeredSignal(arrayId);
				// Firing signal
				HandleToBeTriggeredSignal(arrayId);

				arrayId--;
			}
		}
	}
	return 0;
}

void CPCI2330Device::ReceiveNgSignal(int chIndex, NG_IMG_SIGNAL& ngSignal)
{
	if(!m_bIoCardReady)
	{
		return;
	}
	auto ptr = m_dictSignal[chIndex];
	if(ptr != nullptr && ptr->is_not_full())
	{
		ptr->push_front(ngSignal);
	}
	else
	{
		NG_IMG_SIGNAL obsolete;
		ptr->pop_back(&obsolete);
		gLogger.QueueUpLog(L"Rejection signal queue 1 is full!");
	}
}

void CPCI2330Device::CleanNgSignal(int chIndex)
{
	NG_IMG_SIGNAL signal;

	auto signalArrayPtr = m_dictSignal[chIndex];
	while(signalArrayPtr->is_not_empty())
	{
		signalArrayPtr->pop_back(&signal);
	}

	auto firedSignalArrayPtr = m_dictFiredSignal[chIndex];
	while(firedSignalArrayPtr->is_not_empty())
	{
		firedSignalArrayPtr->pop_back(&signal);
	}
}

ERROR_INFO CPCI2330Device::GetPciErrorInfo()
{
	return m_errorInfo;
}