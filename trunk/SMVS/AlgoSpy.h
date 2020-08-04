#pragma once

#include <string>
#include <vector>
#include <map>
#include <atlconv.h>
#include <cpp\HalconCpp.h>

using namespace std;
using namespace Halcon;

struct LOG_ITEM
{
	LOG_ITEM()
	{
	}
	LOG_ITEM(const wstring& strDesc, const Hobject& errImg):m_strDesc(strDesc), m_errImg(errImg)
	{
	}
	LOG_ITEM(const LOG_ITEM& other)
	{
		m_strDesc = other.m_strDesc;
		m_errImg = other.m_errImg;
	}
	LOG_ITEM(LOG_ITEM&& other)
	{
		m_strDesc = other.m_strDesc;
		m_errImg = other.m_errImg;
	}
	LOG_ITEM& operator=(const LOG_ITEM& other)
	{
		if(&other == this)
			return *this;
		m_strDesc = other.m_strDesc;
		m_errImg = other.m_errImg;
		return *this;
	}
	LOG_ITEM& operator=(LOG_ITEM&& other)
	{
		if(&other == this)
			return *this;
		m_strDesc = other.m_strDesc;
		m_errImg = other.m_errImg;
		return *this;
	}
	wstring m_strDesc; // description
	Hobject m_errImg;
};

class CAlgoSpy
{
public:
	CAlgoSpy()
	{
		TCHAR  moduleFileName[MAX_PATH];  
		::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
		(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
		ATL::CString strPath = moduleFileName;
		strPath.Append(_T("\\log"));
		strPath += _T("\\algo_log");
		if(!_wmkdir(strPath))
		{
			//TODO
		}
		m_strPath = ATL::CT2W(strPath);

		SYSTEMTIME dt;
		::GetLocalTime(&dt);
		ATL::CString strDate;
		strDate.Format(_T("%d-%02d-%02d-%02d-%02d-%02d-%02d"), dt.wYear, dt.wMonth, dt.wDay,dt.wHour, dt.wMinute, dt.wSecond, dt.wMilliseconds);
		wstring wstrDate = ATL::CT2W(strDate);
		m_strFile = m_strPath + L"\\" + wstrDate + L".log";
		m_nLogCount = 0;
	}
	void QueueUpLogMsg(const HException &except, const Hobject& errImg)
	{
		if(m_nLogCount >= 128)
			return;
		int errID = (int)except.err;
		if(m_dictLog.find(errID) != m_dictLog.end())
			return;

		USES_CONVERSION;
		string strMsg = except.proc;
		strMsg += " - ";
		strMsg += except.message;
		wstring wstrMsg = ATL::CA2W(strMsg.c_str());
		LOG_ITEM item(wstrMsg, errImg);
		m_dictLog[errID] = item;
		m_nLogCount++;
	}

	void FushLogCache()
	{
		if(m_dictLog.empty())
			return;

		FILE* fp = nullptr;
		_wfopen_s(&fp, m_strFile.c_str(), _T("a"));
		for(auto it = m_dictLog.begin(); it != m_dictLog.end(); it++)
		{
			wstring strMsg = it->second.m_strDesc;
			fputws(strMsg.c_str(), fp);
			fputws(L"\r\n", fp);

			if(it->second.m_errImg.Id() != H_EMPTY_REGION)
			{
				string strPath = ATL::CW2A(m_strPath.c_str());
				ATL::CString sFileName;
				sFileName.Format(_T("\\%04d.tiff"), it->first);
				string strFileName = strPath;
				strFileName.append(ATL::CT2A(sFileName));
				write_image(it->second.m_errImg, "tiff", 0, strFileName.c_str());
			}
		}
		fclose(fp);
		m_dictLog.clear();
	}
private:
	std::map<int, LOG_ITEM> m_dictLog;
	wstring m_strPath;
	wstring m_strFile;
	int m_nLogCount;
};
