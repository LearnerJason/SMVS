#pragma once

#include <map>
#include <boost\circular_buffer.hpp>
#include "bounded_buffer.h"
#include "TypeDef.h"
#include "utils.h"

using namespace std;
using namespace boost;

class CPCI2312Device
{
public:
	CPCI2312Device(void);
	~CPCI2312Device(void);
public:
	bool CreatePCIDevice();
	void Start();
	void Stop();
	void ReceiveNgSignal(int chIndex, NG_IMG_SIGNAL& ngSignal);
	void CleanNgSignal(int chIndex);
	bool GetRejectorStatus();
	void WritePort(int portNum, BYTE st);
     ERROR_INFO GetPciErrorInfo();
private:
	unsigned int __stdcall TriggerSignalThreadFunc();
	void StartTriggerSignalThread();
	void HandleTriggeredSignal(int arrayId);
	void HandleToBeTriggeredSignal(int arrayId);
	bool IsSignalAvailable();
private:
	const int             SIGNAL_BUFFER_SZ;
	HANDLE                m_hTriggerThread;
	volatile unsigned int m_exitFlag;
	LARGE_INTEGER         m_freq;
	int                   m_szSignalArray;
	LARGE_INTEGER         m_firedTime[16];
	//unsigned long long    m_nCount;
	bool                  m_bIoCardReady;
	HANDLE                m_hIoCard;
	HANDLE                m_hIoEvent;
	BYTE		          m_ioBuff[16];
    ERROR_INFO            m_errorInfo;

	std::map<int, bounded_buffer<NG_IMG_SIGNAL>*>  m_dictSignal;
	std::map<int, bounded_buffer<NG_IMG_SIGNAL>*>  m_dictFiredSignal;

	union _RejectorProc
	{
		unsigned (_stdcall * ThreadProc)(void*);
		unsigned (_stdcall CPCI2312Device::*MemProc)();
	} RejectorProc;
};

