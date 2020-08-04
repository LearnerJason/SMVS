#pragma once
#include <string>
#include <map>
#include <vector>
#include <boost\lexical_cast.hpp>

using namespace std;
using namespace boost;

class CLogger
{
public:
	CLogger(void);
	CLogger(wstring wstrLogName);
	CLogger(const CLogger& other);
	CLogger(CLogger&& other);
	CLogger& operator=(const CLogger& other);
	CLogger& operator=(CLogger&& other);
	~CLogger(void);
	void QueueUpLog(wstring strMsg,bool bHighResolution = false);
	void FlushLogCache(int index = -1);
	void WriteDebugInfo(string wstrMsg);
private:
	wstring m_wstrPath;
	std::map<wstring, wstring> m_dictLogMsg;
	CRITICAL_SECTION                     m_LogDataCs;
};

