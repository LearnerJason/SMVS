#pragma once

#include <map>
#include <boost\circular_buffer.hpp>
#include "bounded_buffer.h"
#include "TypeDef.h"
#include "utils.h"
#include "PCI2361.h"

using namespace std;
using namespace boost;

class CLogger;

const long long ENCODER_MAX_COUNTER = 65536;

class CChannelRejector
{
public:
	CChannelRejector(const REJECTOR_SETTING& rs, CLogger* pLogger);
	~CChannelRejector(void);

public:
	void Start();
	void Stop();
	void SetRejectorPort(int nPort);
	long GetEncoderCrtValue();
	void ReceiveNgSignal(REJECT_SIGNAL& ngSignal);
	void ExitRejectorThread();
	bool GetRejectorStatus();
	BYTE ReadPort(int portNum);
	BOOL WritePort(int portNum, BYTE st);
private:
	bool InitRejector();
	unsigned int __stdcall RejectorThreadFunc();
	bool NeedHoldSignal(REJECT_SIGNAL& signal);
	void HandleTriggeredSignal();
	void HandleToBeTriggeredSignal();
	BYTE GetPortDI(int nPortNum, PCI2361_PARA_DI& paraDI);
	long long CalculateDistance(long long crtValue, long long oldValue);
private:
	const int             SIGNAL_BUFFER_SZ; // 16
	CLogger*              m_pLogger;
	int                   m_nRjPort;
	int                   m_nEncoderID;
	HANDLE                m_hTriggerThread;
	LARGE_INTEGER         m_freq;
	int                   m_szSignalArray;
	volatile unsigned int m_exitFlag;
	bounded_buffer<REJECT_SIGNAL>  m_ngSignal;
	bounded_buffer<REJECT_SIGNAL>  m_firedNgSignal;

	union _RejectorProc
	{
		unsigned (_stdcall * ThreadProc)(void*);
		unsigned (_stdcall CChannelRejector::*MemProc)();
	} RejectorProc;

	bool          m_bRejectorReady;
	HANDLE        m_hIoCard;
	HANDLE        m_hIoEvent;
};

