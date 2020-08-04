#include "StdAfx.h"
#include "Logger.h"
#include "utils.h"


CLogger::CLogger(void)
{
	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
	CString strPath = moduleFileName;
	m_wstrPath = strPath.GetString();
	m_wstrPath.append(L"\log");
	if(!_wmkdir(m_wstrPath.c_str()))
	{
		//TODO
	}
	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	CString strDate;
	strDate.Format(_T("%d-%02d-%02d"), dt.wYear, dt.wMonth, dt.wDay);
	m_wstrPath = m_wstrPath + L"\\" + strDate.GetString() + L".txt";
	::InitializeCriticalSection(&m_LogDataCs);
}

CLogger::CLogger(wstring wstrLogName)
{
	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
	CString strPath = moduleFileName;
	strPath.Append(_T("\log\\"));
	m_wstrPath = strPath.GetString();
	m_wstrPath.append(wstrLogName);
	if(!_wmkdir(m_wstrPath.c_str()))
	{
		//TODO
	}
	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	CString strDate;
	strDate.Format(_T("%d-%02d-%02d"), dt.wYear, dt.wMonth, dt.wDay);
	m_wstrPath = m_wstrPath + L"\\" + strDate.GetString() + L".txt";
	::InitializeCriticalSection(&m_LogDataCs);
}

CLogger::CLogger(const CLogger& other)
{
	m_wstrPath      = other.m_wstrPath;
	m_dictLogMsg    = other.m_dictLogMsg;
	m_LogDataCs = other.m_LogDataCs;
}

CLogger::CLogger(CLogger&& other)
{
	m_wstrPath      = other.m_wstrPath;
	m_dictLogMsg    = other.m_dictLogMsg;
	m_LogDataCs = other.m_LogDataCs;
}

CLogger& CLogger::operator=(const CLogger& other)
{
	if(&other == this)
		return *this;
	m_wstrPath      = other.m_wstrPath;
	m_dictLogMsg  = other.m_dictLogMsg;
	m_LogDataCs   = other.m_LogDataCs;

	return *this;
}

CLogger& CLogger::operator=(CLogger&& other)
{
	if(&other == this)
		return *this;

	m_wstrPath      = other.m_wstrPath;
	m_dictLogMsg    = other.m_dictLogMsg;
	m_LogDataCs   = other.m_LogDataCs;

	return *this;
}

CLogger::~CLogger(void)
{
	//Debug状态下，关闭软件会中断，原因未知！
	//::DeleteCriticalSection(&m_LogDataCs);
	FlushLogCache();
}

void CLogger::QueueUpLog(wstring strMsg,bool bHighResolution)
{
	if (TryEnterCriticalSection(&m_LogDataCs) == FALSE)
	{
		
	//	返回
	}
	else
	{
	//	进入临界区
		size_t sz = m_dictLogMsg.size();
		if(sz >= 1)
		{
			FlushLogCache();
			//LeaveCriticalSection(&m_LogDataCs);
			//return;
		}

		SYSTEMTIME dt;
		::GetLocalTime(&dt);
		CString sTimestamp;
		if(!bHighResolution)
			sTimestamp.Format(_T("%2d-%2d | %02d:%02d:%02d,"),dt.wMonth,dt.wDay,dt.wHour, dt.wMinute,dt.wSecond);
		else
			sTimestamp.Format(_T("%2d-%2d | %02d:%02d:%02d:%03d,"),dt.wMonth,dt.wDay,dt.wHour, dt.wMinute,dt.wSecond,dt.wMilliseconds);
		wstring strTimestamp = CT2W(sTimestamp);
		if(m_dictLogMsg.find(strTimestamp) != m_dictLogMsg.end())
			return;
		wstring strLog = strTimestamp + strMsg;
		m_dictLogMsg[strTimestamp] = strLog;
		LeaveCriticalSection(&m_LogDataCs);
	}
}

void CLogger::FlushLogCache(int index)
{
	if(m_dictLogMsg.empty())
	{
		return;
	}
	FILE* fp = nullptr;
	string strFile = utils::Unicode2Utf8(m_wstrPath);

	if (index > 0)
	{
		std::ostringstream ss;
		ss << index;
		string strHex = ss.str();
		strFile += strHex;
	}
	////////////////////////////////////////////////
	errno_t err = fopen_s(&fp,strFile.c_str(), "a");
	if (err== 0 &&fp != nullptr )
	{
		for(auto it = m_dictLogMsg.begin(); it != m_dictLogMsg.end(); it++)
		{
			string strLog = utils::Unicode2Utf8(it->second);
			fputs(strLog.c_str(), fp);
			fputs("\r\n", fp);
		}
		fclose(fp);
		m_dictLogMsg.clear();
	}
}

void CLogger::WriteDebugInfo(string wstrMsg)
{
	FILE* fp = nullptr;
	string strFile = utils::Unicode2Utf8(m_wstrPath);
	fopen_s(&fp,strFile.c_str(), "a");
	fputs(wstrMsg.c_str(), fp);
	fputs("\r\n", fp);
	fclose(fp);
}
