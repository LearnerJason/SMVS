#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

#define  WM_COMM_BREAK_DETECTED		WM_USER + 1// A break was dettected on input;
#define  WM_COMM_CTS_DETECTED		WM_USER + 2// The CTS(clear-to-send) signal changed state
#define  WM_COMM_DSR_DETECTED		WM_USER + 3// The DSR(data-set-ready) signal changed state
#define  WM_COMM_ERR_DETECTED		WM_USER + 4// A line-status error occurred. Line-status errors are CE_FRAME,CE_OVERRUN,CE_RXPARITY
#define  WM_COMM_RING_DETECTED      WM_USER + 5// A ring indicator was detected.
#define  WM_COMM_RLSD_DETECTED      WM_USER + 6// The RLSD(receive-line-signal-detect) signal changed state
#define  WM_COMM_RXCHAR_DETECTED    WM_USER + 7// A character was receive and placed in the input input buffer
#define  WM_COMM_RXFLAG_DETECTED    WM_USER + 8// The event character was received and placed in the input buffer
#define  WM_COMM_TXEMPTY_DETECTED   WM_USER + 9// The last character in the output buffer was sent
#define  WM_COMM_WRTOK_DETECTED     WM_USER + 10//

const size_t READ_BUFFER_SIZE  = 64;

class CChannelControlPanel;

class CSerialPort
{
public:
	CSerialPort(int nPortNum);
	virtual ~CSerialPort();
    bool InitPort(UINT baud = 19200, char parity = 'N', UINT databits = 8, UINT atopsbits = 1, DWORD dwCommEvents = EV_RXCHAR|EV_CTS);
	void SetOwnerWindowPtr(CWnd* pWnd);
	bool  StartMonitoring();
	bool  RestartMonitoring();
	DWORD GetWriteBufferSize();
	void  SendCmdToPort(const std::vector<byte>& cmd);
	void  SendCmdToPort(const std::string& strCmd);
	void ReceiveChar(COMSTAT comstat);
	bool GetReadFinishedFlag();
	void ClosePort();

private:
	unsigned int __stdcall SerialPortThreadFunc();
	std::map<UINT, UINT> InitEventMsgMap();
	void TryToShutdown();
	void ProcessErrorMessage(char* ErrorText);
	void WritePort();

private:
	bool				 m_bThreadAlive;
	HANDLE				 m_hShutdownEvent;
	HANDLE				 m_hComm;
	HANDLE				 m_hWriteEvent;
	HANDLE				 m_hEventArray[3];
	HANDLE				 m_hThread;
	UINT				 m_iThreadId;
	OVERLAPPED			 m_ov;
	DCB					 m_dcb;
	COMMTIMEOUTS		 m_CommTimeouts;
	CRITICAL_SECTION	 m_csCommunicationSync;
	int				     m_nPortNum;
	std::string          m_writeBuffer;
	std::map<UINT, UINT> EVENT_2_MSG_MAP;
	bool                 m_bReadingPortFinished;
	std::vector<byte>    m_RXBuff;
	CWnd*                m_pOwnerWnd;

	union _SerialPortProc
	{
		unsigned (_stdcall * ThreadProc)(void*);
		unsigned (_stdcall CSerialPort::*MemProc)();
	} SerialPortProc;
};