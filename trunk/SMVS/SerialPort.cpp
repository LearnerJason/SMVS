#include "StdAfx.h"
#include "SerialPort.h"
#include <assert.h>
#include <process.h>

CSerialPort::CSerialPort(int nPortNum)
	:m_nPortNum(nPortNum),
	 m_hComm(0),
	 m_hWriteEvent(0),
	 m_hShutdownEvent(0),
	 m_bReadingPortFinished(false),
	 m_bThreadAlive(false),
	 m_pOwnerWnd(nullptr)
{
	::memset((void*)&m_ov, 0x00, sizeof(OVERLAPPED));

	m_ov.hEvent		 = CreateEvent(NULL,TRUE,FALSE,NULL);  assert(NULL != m_ov.hEvent);
	m_hWriteEvent	 = CreateEvent(NULL,TRUE,FALSE,NULL);  assert(NULL != m_hWriteEvent);
	m_hShutdownEvent = CreateEvent(NULL,TRUE,FALSE,NULL);  assert(NULL != m_hShutdownEvent);

	m_hEventArray[0] = m_hShutdownEvent;
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	m_CommTimeouts.ReadIntervalTimeout			= 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier	= 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant		= 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier	= 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant	= 1000;

	EVENT_2_MSG_MAP = InitEventMsgMap();

	InitializeCriticalSection(&m_csCommunicationSync);
	m_RXBuff = std::vector<byte>(19,0x00);
}

CSerialPort::~CSerialPort()
{
	TryToShutdown();
	m_bThreadAlive = FALSE;
	TRACE("Thread ended\n");
	::DeleteCriticalSection(&m_csCommunicationSync);
}

std::map<UINT, UINT> CSerialPort::InitEventMsgMap()
{
	std::map<UINT, UINT> tmpMap;
	tmpMap[EV_RXCHAR] = WM_COMM_RXCHAR_DETECTED;
	tmpMap[EV_CTS]    = WM_COMM_CTS_DETECTED;
	tmpMap[EV_RXFLAG] = WM_COMM_RXFLAG_DETECTED;
	tmpMap[EV_BREAK]  = WM_COMM_BREAK_DETECTED;
	tmpMap[EV_ERR]    = WM_COMM_ERR_DETECTED;
	tmpMap[EV_RING]   = WM_COMM_RING_DETECTED;

	return tmpMap;
}

bool CSerialPort::InitPort(UINT baudrate, char parity, UINT databits, UINT stopsbits, DWORD dwCommEvents)
{
	if (m_bThreadAlive)
	{
		TryToShutdown();
	}
	if(m_nPortNum < 0)
	{
		return false;
	}
	ResetEvent(m_ov.hEvent);
	ResetEvent(m_hWriteEvent);
	ResetEvent(m_hShutdownEvent);

	EnterCriticalSection(&m_csCommunicationSync);

	if (NULL != m_hComm)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	std::wostringstream oss1, oss2;
	oss1 << L"\\\\.\\COM" << m_nPortNum;
	oss2 << L"baud=" << baudrate <<L" parity=" << parity <<L" data=" << databits <<L" stop=" << stopsbits;
	wstring wstrPort = oss1.str();
	wstring wstrBaud = oss2.str();
	m_hComm = CreateFileW(
		wstrPort.c_str(), 
		GENERIC_READ | GENERIC_WRITE,
		NULL,
		NULL,
		OPEN_EXISTING, // 调用CreateFile来打开文件之外的其他设备时，必须使用OPEN_EXISTING参数 ！！！
		FILE_FLAG_OVERLAPPED|FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		return false;
	}
	bool bOk = true;
	if(!SetCommTimeouts(m_hComm,&m_CommTimeouts))
	{
		ProcessErrorMessage("SetCommTimeouts"); bOk = false;
		goto LastW;
	}
	if(!SetCommMask(m_hComm,dwCommEvents))
	{
		ProcessErrorMessage("SetCommMask"); bOk = false;
		goto LastW;
	}
	if(!GetCommState(m_hComm,&m_dcb))
	{
		ProcessErrorMessage("GetCommState"); bOk = false;
		goto LastW;
	}
	m_dcb.EvtChar     = 'q';
	m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
	if(!BuildCommDCB(wstrBaud.c_str(), &m_dcb))
	{
		ProcessErrorMessage("BuildCommDCB"); bOk = false;
		goto LastW;
	}
	if (!SetCommState(m_hComm,&m_dcb))
	{
		ProcessErrorMessage("SetCommState"); bOk = false;
		goto LastW;
	}
	LastW:
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_TXABORT|PURGE_RXABORT);
	LeaveCriticalSection(&m_csCommunicationSync);

	return bOk;
}

void CSerialPort::SetOwnerWindowPtr(CWnd* pWnd)
{
	m_pOwnerWnd = pWnd;
}

unsigned int CSerialPort::SerialPortThreadFunc()
{
	DWORD Event = 0, CommEvent = 0, dwError = 0;
	COMSTAT comstat;
	::memset((void*)&comstat, 0x00, sizeof(COMSTAT));
	// 该函数终止读写、清空缓冲区，根据dwFlags的设置完成指定功能,相关函数有FlushFileBuffer。
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_TXABORT|PURGE_RXABORT);
	BOOL bResult = TRUE;
	// Begin forever loop. This loop will run as long as the thread is alive
    for (;;)
    {
		// Make a call to WaitCommEvent(). This call will return immediatly because our port was created as an async port (FILE_FLAG_OVERLAPPED)
		// and an m_ov structure specified). 
		bResult = WaitCommEvent(m_hComm, &Event, &m_ov);
		if (!bResult)
		{
			dwError = GetLastError();
			if(ERROR_IO_PENDING == dwError)
			{
				// This is normal return value if there are no bytes to read at the port.
				// Do nothing!
			}
			else if(ERROR_INVALID_PARAMETER == dwError)
			{
				// Under Windows NT, this value is returned for some reason.
			}
			else
			{
				// All other error codes indicate a serious error has occurred,Process this error.
				ProcessErrorMessage("WaitCommEvent");
			}
		}
		else // If WaitCommEvent() returns TRUE, check to be sure there are actually bytes in the buffer to read.
		{   
			// ClearCommError will reset the event handle, and if there are no bytes to read we can loop back through WaitCommEvent() again,
			// then proceed. If there are really bytes to read, do nothing and proceed.
			ClearCommError(m_hComm, &dwError, &comstat);
			if (0 == comstat.cbInQue)
			{
				continue;
			}
		}
		Event = WaitForMultipleObjects(3, m_hEventArray, FALSE, INFINITE);
		if(0 == Event) // Shutdown Event
		{
			CloseHandle(m_hComm);
			m_hComm = NULL;
			m_bThreadAlive = FALSE;
			return 0;
		}
		else if(1 == Event) // Read Event
		{
            GetCommMask(m_hComm,&CommEvent);
			if (CommEvent & EV_RXCHAR)
			{
				ReceiveChar(comstat);
			}
			else
			{

			}
		}
		else if(2 == Event)  // Write Event
		{
			WritePort();
		}
		else
		{
		}
    }
	return 0;
}

void CSerialPort::ReceiveChar(COMSTAT comstat)
{
	BOOL bRead      = TRUE;
	BOOL bResult    = TRUE;
	DWORD dwError   = 0;
	DWORD BytesRead = 0;
	m_RXBuff = std::vector<byte>(19,0x00);

	for (;;)
	{
		EnterCriticalSection(&m_csCommunicationSync);
		bResult = ClearCommError(m_hComm, &dwError, &comstat);
		LeaveCriticalSection(&m_csCommunicationSync);
		if(0 == comstat.cbInQue)
		{
			break; // Break out if all bytes have been read 
		}

		EnterCriticalSection(&m_csCommunicationSync);
		bResult = ReadFile(m_hComm, &m_RXBuff[0], 18, &BytesRead, &m_ov);
		if (!bResult)
		{
			if(ERROR_IO_PENDING == GetLastError()) //ERROR_IO_PENDING 表示I/O请求已经被成功地添加到了设备驱动程序的队列中，等待处理！
			{
				bRead = FALSE;
			}
			else
			{
				ProcessErrorMessage("ReadFile");
			}
		}
		else
		{
			bRead = TRUE;
		}
		if (!bRead)
		{
			if (!GetOverlappedResult(m_hComm, &m_ov, &BytesRead, TRUE))
			{
				ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}
			bRead = TRUE;
		}
		LeaveCriticalSection(&m_csCommunicationSync);

		if(m_pOwnerWnd != nullptr) // Notify the parent window that a byte was received.
		{
			::SendMessage(m_pOwnerWnd->m_hWnd, WM_COMM_RXCHAR_DETECTED, (WPARAM)NULL, (LPARAM)&m_RXBuff[0]);
		}
	}
	m_bReadingPortFinished = true;
}

void CSerialPort::SendCmdToPort(const std::vector<byte>& cmd)
{
	assert(m_hComm != 0);
	m_writeBuffer.clear();
	m_writeBuffer.assign(cmd.begin(), cmd.end());
	SetEvent(m_hWriteEvent);
}

void  CSerialPort::SendCmdToPort(const std::string& strCmd)
{
	m_writeBuffer.clear();
	m_writeBuffer = strCmd;
	SetEvent(m_hWriteEvent);
}

bool CSerialPort::StartMonitoring()
{
	UINT nThreadID;
	SerialPortProc.MemProc = &CSerialPort::SerialPortThreadFunc;

	m_hThread = (HANDLE)_beginthreadex(NULL,0,SerialPortProc.ThreadProc, this, 0, &nThreadID);
	if (m_hThread)
	{
		m_bThreadAlive = TRUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool CSerialPort::RestartMonitoring()
{
	DWORD dwRet = ::ResumeThread(m_hThread);
	if (0xFFFFFFFF == dwRet)
	{
		LPVOID lpMsgBuf;
		UINT flags = (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS);
		FormatMessage(flags, NULL, GetLastError(), 0, (LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
		LocalFree( lpMsgBuf );
	}
	return TRUE;
}

void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		0, // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL );
	wstring wstrErrText = CA2W(ErrorText);
	wstring wstrBuff = CT2W((LPTSTR)lpMsgBuf);
	std::wostringstream oss;
	oss << "WARNING: " << wstrErrText.c_str() << L" Failed with following error:\n" << wstrBuff.c_str() << L"Port:" << m_nPortNum;
	wstring errorDesc = oss.str();
	MessageBoxW( NULL, errorDesc.c_str(), _T("Error"), MB_OK | MB_ICONINFORMATION );
	LocalFree( lpMsgBuf );
}

void CSerialPort::WritePort()
{
	if(m_writeBuffer.empty())
	{
		return;
	}
	UINT portNum	= m_nPortNum;
	BOOL bWrite		= TRUE;
	DWORD BytesSent = 0;
	ResetEvent(m_hWriteEvent);

	EnterCriticalSection(&m_csCommunicationSync);

	m_ov.Offset     = 0;
	m_ov.OffsetHigh = 0;
	PurgeComm(m_hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_TXABORT|PURGE_RXABORT);
	BOOL bResult = WriteFile(m_hComm, m_writeBuffer.c_str(), (DWORD)m_writeBuffer.length(), &BytesSent, &m_ov);
	if (!bResult)
	{
		DWORD nErrCode = GetLastError();
		if(ERROR_IO_PENDING == nErrCode)
		{
			BytesSent = 0;
			bWrite = FALSE;
		}
		else
		{
			ProcessErrorMessage("WriteFile");
		}
	}
	else
	{
		LeaveCriticalSection(&m_csCommunicationSync);
	}
	if(!bWrite)
	{
		bWrite = TRUE;
		bResult = GetOverlappedResult(m_hComm, &m_ov, &BytesSent, TRUE);
		LeaveCriticalSection(&m_csCommunicationSync);
		if (!bResult)
		{
			//ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}
	}
}

void CSerialPort::TryToShutdown()
{
	do 
	{
		if(m_hShutdownEvent)
			SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);
}

bool CSerialPort::GetReadFinishedFlag()
{
	return m_bReadingPortFinished;
}

void CSerialPort::ClosePort()
{
	if (m_bThreadAlive)
	{
		SetEvent(m_hShutdownEvent);
	}
    WaitForSingleObject(m_hThread,5000);
    CloseHandle(m_hThread);
	m_bThreadAlive = FALSE;
}

