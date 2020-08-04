#include "StdAfx.h"
#include "ChannelRejector.h"
#include "ConfigurationManager.h"
#include "Logger.h"
#include "SystemManager.h"

extern ConfigManager  gConfigMgr;
extern CSystemManager gSystemMgr;

CChannelRejector::CChannelRejector(const REJECTOR_SETTING& rs, CLogger* pLogger)
	:SIGNAL_BUFFER_SZ(16),
	 m_nRjPort(rs.m_rejectorPort),
	 m_nEncoderID(rs.m_encoderID),
	 m_pLogger(pLogger),
	 m_bRejectorReady(false),
	 m_ngSignal(SIGNAL_BUFFER_SZ),
	 m_firedNgSignal(SIGNAL_BUFFER_SZ),
	 m_exitFlag(0)
{
	if(!InitRejector())
	{
	}
	QueryPerformanceFrequency(&m_freq);
}

CChannelRejector::~CChannelRejector(void)
{
}

bool CChannelRejector::InitRejector()
{
	PCI2361_PARA_COUNTER_CTRL ctrlParam;
	//TODO:
	ctrlParam.OperateType = PCI2361_OperateType_3;
	ctrlParam.CountMode   = PCI2361_CountMode_5;
	ctrlParam.BCD         = PCI2361_BCD_0;
	BOOL bRet = PCI2361_InitDevCounter(NULL, &ctrlParam, 0, m_nEncoderID);
	if(bRet == FALSE)
	{
		m_pLogger->QueueUpLog(L"Initalize channnel encoder failed!");
		return false;
	}
	m_bRejectorReady = true;
	return true;
}

void CChannelRejector::Start()
{
	UINT threadId;
	RejectorProc.MemProc = &CChannelRejector::RejectorThreadFunc;
	m_hTriggerThread = (HANDLE)_beginthreadex(NULL, 0, RejectorProc.ThreadProc, (LPVOID)this, 0, &threadId);
	::SetThreadPriority(m_hTriggerThread, THREAD_PRIORITY_HIGHEST);
}

void CChannelRejector::Stop()
{
	::InterlockedIncrement(&m_exitFlag);
}

void CChannelRejector::SetRejectorPort(int nPort)
{
	m_nRjPort = nPort;
}

long CChannelRejector::GetEncoderCrtValue()
{
	long nCrtValue = -1;
	BOOL bRet = FALSE; //PCI2361_GetDevCounter(gSystemMgr.m_hPCI2361, &nCrtValue, m_nEncoderID);
	if(bRet == FALSE)
	{
		wstring wstrMsg(L"Read encoder #");
		wstring wstrID = boost::lexical_cast<wstring>(m_nEncoderID);
		wstrMsg.append(wstrID);
		wstrMsg.append(L" failed!");
		wstring wstrLastLog = m_pLogger->GetLastLogString();
		if(wstrLastLog == L"" || wstrLastLog != wstrMsg)
		{
			m_pLogger->QueueUpLog(wstrMsg);
		}
		return -1;
	}
	return nCrtValue;
}

bool CChannelRejector::GetRejectorStatus()
{
	return m_bRejectorReady;
}

BYTE CChannelRejector::GetPortDI(int nPortNum, PCI2361_PARA_DI& paraDI)
{
	if(nPortNum >= 16)
	{
		nPortNum = nPortNum % 16;
	}
	if(nPortNum == 0)  return paraDI.DI0;
	if(nPortNum == 1)  return paraDI.DI1;
	if(nPortNum == 2)  return paraDI.DI2;
	if(nPortNum == 3)  return paraDI.DI3;
	if(nPortNum == 4)  return paraDI.DI4;
	if(nPortNum == 5)  return paraDI.DI5;
	if(nPortNum == 6)  return paraDI.DI6;
	if(nPortNum == 7)  return paraDI.DI7;
	if(nPortNum == 8)  return paraDI.DI8;
	if(nPortNum == 9)  return paraDI.DI9;
	if(nPortNum == 10) return paraDI.DI10;
	if(nPortNum == 11) return paraDI.DI11;
	if(nPortNum == 12) return paraDI.DI12;
	if(nPortNum == 13) return paraDI.DI13;
	if(nPortNum == 14) return paraDI.DI14;
	if(nPortNum == 15) return paraDI.DI15;

	return 0xFF;
}

BYTE CChannelRejector::ReadPort(int portNum)
{
	if(!m_bRejectorReady)
	{
		return IO_OFF;
	}

	BOOL bRet = FALSE;
	PCI2361_PARA_DI paramDI;
	if(portNum < 16)
	{
		//bRet = PCI2361_GetDeviceDIL(gSystemMgr.m_hPCI2361, &paramDI);
	}
	else
	{
		//bRet = PCI2361_GetDeviceDIH(gSystemMgr.m_hPCI2361, &paramDI);
	}
	if(bRet == FALSE)
	{
		return IO_OFF;
	}
	BYTE ret[16];
	::memcpy((void*)ret, (void*)&paramDI, sizeof(BYTE)*16);
	return ret[portNum];
}

BOOL CChannelRejector::WritePort(int portNum, BYTE st)
{
	PCI2361_PARA_DO paraDO;
	BOOL bRet = FALSE;
	if(portNum < 16)
	{
		gSystemMgr.m_DOL[portNum] = st;
		::memcpy((void*)&paraDO,(void*)gSystemMgr.m_DOL, sizeof(BYTE)*16);
		//bRet = PCI2361_SetDeviceDOL(gSystemMgr.m_hPCI2361, &paraDO);
	}
	else
	{
		portNum = portNum % 16;
		gSystemMgr.m_DOH[portNum] = st;
		::memcpy((void*)&paraDO,(void*)gSystemMgr.m_DOH, sizeof(BYTE)*16);
		//bRet = PCI2361_SetDeviceDOH(gSystemMgr.m_hPCI2361, &paraDO);
	}
	return bRet;
}

void CChannelRejector::ReceiveNgSignal(REJECT_SIGNAL& ngSignal)
{
	if(m_ngSignal.is_not_full())
	{
		m_ngSignal.push_front(ngSignal);
	}
	else
	{
		gLogger.QueueUpLog(L"Rejection signal queue 1 is full!");
	}
}

unsigned int CChannelRejector::RejectorThreadFunc()
{
	while(!m_exitFlag)
	{
		if(m_ngSignal.is_empty() && m_firedNgSignal.is_empty())
		{
			if(m_exitFlag)
				break;
			Sleep(3);
			continue;
		}
		HandleTriggeredSignal();
		HandleToBeTriggeredSignal();
		Sleep(1);
	}
	return 0;
}

bool CChannelRejector::NeedHoldSignal(REJECT_SIGNAL& signal)
{
	bool bHold = false;
	int nRjPort = m_nRjPort;
	if(signal.m_rjMode == RM_DISTANCE)
	{
		bHold = (gSystemMgr.m_firedTime[nRjPort].QuadPart != signal.m_bgn.QuadPart) && 
				(::abs(gSystemMgr.m_firedTime[nRjPort].QuadPart - signal.m_bgn.QuadPart) <= signal.m_signalWidth);
	}
	else //RM_TIME
	{
		bHold = (gSystemMgr.m_firedTime[nRjPort].QuadPart != signal.m_bgn.QuadPart) && 
				(::abs((gSystemMgr.m_firedTime[nRjPort].QuadPart - signal.m_bgn.QuadPart) * 1000 / m_freq.QuadPart) <= signal.m_signalWidth);
	}
	return bHold;
}

long long CChannelRejector::CalculateDistance(long long crtValue, long long oldValue)
{
	if(crtValue <= oldValue)
	{
		return  oldValue - crtValue;
	}

	return oldValue + (ENCODER_MAX_COUNTER - crtValue);
}

void CChannelRejector::HandleTriggeredSignal()
{
	if(m_firedNgSignal.is_empty())
	{
		return;
	}

	REJECT_SIGNAL signal;
	m_firedNgSignal.pop_back(&signal);

	LARGE_INTEGER now;
	long long delta;
	if(signal.m_rjMode == RM_DISTANCE)
	{
		now.QuadPart = (long long)GetEncoderCrtValue();
		delta = CalculateDistance(now.QuadPart, signal.m_bgn.QuadPart);
	}
	else //RM_TIME
	{
		QueryPerformanceCounter(&now);
		delta = (long long)::abs((now.QuadPart - signal.m_bgn.QuadPart) * 1000 / m_freq.QuadPart);
	}

	if(delta >= (long long)signal.m_signalWidth)
	{
		if(NeedHoldSignal(signal))
		{
			return;
		}
		else
		{
			if(FALSE == WritePort(m_nRjPort, IO_OFF))
			{
				wstring wstrMsg(L"Write port #");
				wstrMsg.append(boost::lexical_cast<wstring>(m_nRjPort));
				wstrMsg.append(L" IO_OFF failed!");
				wstring wstrLastLog = m_pLogger->GetLastLogString();
				if(wstrLastLog == L"" || wstrLastLog != wstrMsg)
				{
					m_pLogger->QueueUpLog(wstrMsg);
				}
			}
		}
	}
	else
	{
		if(m_firedNgSignal.is_not_full())
		{
			m_firedNgSignal.push_back(signal);
		}
	}
}

void CChannelRejector::HandleToBeTriggeredSignal()
{
	if(m_ngSignal.is_empty())
	{
		return;
	}

	REJECT_SIGNAL signal;
	m_ngSignal.pop_back(&signal);

	LARGE_INTEGER now;
	long long delta = 0;
	if(signal.m_rjMode == RM_DISTANCE)
	{
		now.QuadPart = (long long)GetEncoderCrtValue();
		delta = CalculateDistance(now.QuadPart, signal.m_bgn.QuadPart);
	}
	else
	{
		QueryPerformanceCounter(&now);
		delta = (long long)::fabs((now.QuadPart - signal.m_bgn.QuadPart) * 1000.0f / m_freq.QuadPart);
	}
	if(delta == 0) //transfer belt is stopped
		return; 
	
	if(delta >= (long long)signal.m_delay)
	{
		if(WritePort(m_nRjPort, IO_ON))
		{
			signal.m_bgn = now;
			gSystemMgr.m_firedTime[m_nRjPort] = now;
			if(m_firedNgSignal.is_not_full())
			{
				m_firedNgSignal.push_front(signal);
			}
		}
		else
		{
			//TODO:
		}
	}
	else
	{
		if(m_ngSignal.is_not_full())
		{
			m_ngSignal.push_back(signal);
		}
	}
}
