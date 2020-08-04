#include "StdAfx.h"
#include "PCI2310Device.h"
#include "ConfigurationManager.h"
#include "Logger.h"
#include "PCI2310.h"
#include "resource.h"

extern ConfigManager  gConfigMgr;
extern CLogger        gLogger;


CPCI2310::CPCI2310(void)
	:m_exitFlag(0),
	SIGNAL_BUFFER_SZ(64), // 32
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
	::memset((void*)m_firedTime, 0x00, sizeof(LARGE_INTEGER)*32);
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	for(int i = 0; i < 32; i++)
	{
		m_firedTime[i] = now;
	}
}

CPCI2310::~CPCI2310(void)
{
	if(m_bIoCardReady)
	{
		PCI2310_ReleaseSystemEvent(m_hIoEvent);
		PCI2310_ReleaseDeviceInt(m_hIoCard);	// 释放中断资源
		PCI2310_ReleaseDevice(m_hIoCard);		// 释放设备对象
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

bool CPCI2310::CreatePCIDevice()
{
	try
	{
		m_hIoCard = PCI2310_CreateDevice(0);
		if(m_hIoCard == INVALID_HANDLE_VALUE)
		{
            CString strID;
            strID.LoadString(IDS_STRING_ERR_ERRID);
            strID += _T("101");
            CString sMsg;
            sMsg.LoadString(IDS_STRING_PCI_INIT_ERROR);
            sMsg.Replace(_T("#"),_T("2310"));
            CString streMsg = strID + _T("---") + sMsg;//
            ERROR_INFO  pciError(PCI_CREATE_FAILED_ERROR,1,streMsg.GetString());
            m_errorInfo = pciError;
			return false;
		}
		m_hIoEvent = PCI2310_CreateSystemEvent(); // 创建中断事件句柄
		if(m_hIoEvent == INVALID_HANDLE_VALUE)
		{
			m_errorInfo.m_nChIndex = 0;
			m_errorInfo.m_level = WL_ERROR;
			m_errorInfo.m_errorSubID   = 0;
			m_errorInfo.m_strMsg = L"PCI2310:Create system event failed!";
			return false;
		}

		if(!PCI2310_InitDeviceInt(m_hIoCard, m_hIoEvent))
		{
			m_errorInfo.m_nChIndex = 0;
			m_errorInfo.m_level = WL_ERROR;
			m_errorInfo.m_errorSubID   = 0;
			m_errorInfo.m_strMsg = L"PCI2310:Initialize device failed!";
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
		m_errorInfo.m_strMsg = L"PCI2310:unknown exception!";
		return false;
	}
	return m_bIoCardReady;
}

void CPCI2310::Start()
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

void CPCI2310::Stop()
{
	::InterlockedIncrement(&m_exitFlag);
	Sleep(10);
	::memset((void*)m_ioBuff, IO_OFF, sizeof(BYTE) * 32);
	PCI2310_SetDeviceDO(m_hIoCard,m_ioBuff);
}

void CPCI2310::StartTriggerSignalThread()
{
	UINT threadId;
	RejectorProc.MemProc = &CPCI2310::TriggerSignalThreadFunc;
	m_hTriggerThread = (HANDLE)_beginthreadex(NULL, 0, RejectorProc.ThreadProc, (LPVOID)this, 0, &threadId);
	//::SetThreadPriority(m_hTriggerThread, THREAD_PRIORITY_HIGHEST);
}

bool CPCI2310::GetRejectorStatus()
{
	return m_bIoCardReady;
}

void CPCI2310::WritePort(int portNum, BYTE st)
{
	if(!m_bIoCardReady)
		return;
	m_ioBuff[portNum] = st;
	BOOL ret = PCI2310_SetDeviceDO(m_hIoCard,m_ioBuff);
	if(ret == FALSE)
	{
		//TODO
	}
}

void CPCI2310::HandleTriggeredSignal(int arrayId)
{
	if(m_dictFiredSignal[arrayId]->is_empty())
	{
		return;
	}

	NG_IMG_SIGNAL signal;
	m_dictFiredSignal[arrayId]->pop_back(&signal);

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	long long delta = (long long)::abs((now.QuadPart - signal.m_bgn.QuadPart) * 1000 / m_freq.QuadPart);

	if(delta >= 2)
	{
		m_ioBuff[signal.m_nPort] = IO_OFF;
		PCI2310_SetDeviceDO(m_hIoCard,m_ioBuff);
	}
	else
	{
		if(m_dictFiredSignal[arrayId]->is_not_full())
		{
			m_dictFiredSignal[arrayId]->push_front(signal);
		}
	}
}

void CPCI2310::HandleToBeTriggeredSignal(int arrayId)
{
	if(m_dictSignal[arrayId]->is_empty())
	{
		return;
	}

	NG_IMG_SIGNAL signal;
	m_dictSignal[arrayId]->pop_back(&signal);

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	if((now.QuadPart - m_firedTime[signal.m_nPort].QuadPart) * 1000 / m_freq.QuadPart <= 5)
	{
		m_dictSignal[arrayId]->push_front(signal);
		return;
	}
	m_ioBuff[signal.m_nPort] = IO_ON;
	PCI2310_SetDeviceDO(m_hIoCard,m_ioBuff);
	signal.m_bgn = now;
	m_firedTime[signal.m_nPort] = now;

	if(m_dictFiredSignal[arrayId]->is_not_full())
	{
		m_dictFiredSignal[arrayId]->push_front(signal);
	}
}

bool CPCI2310::IsSignalAvailable()
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

unsigned int CPCI2310::TriggerSignalThreadFunc()
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
			Sleep(2);
		}
	}
	return 0;
}

void CPCI2310::ReceiveNgSignal(int chIndex, NG_IMG_SIGNAL& ngSignal)
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

void CPCI2310::CleanNgSignal(int chIndex)
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

ERROR_INFO CPCI2310::GetPciErrorInfo()
{
    return m_errorInfo;
}