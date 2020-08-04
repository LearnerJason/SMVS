#pragma once
#include "dbcontrol.h"
#include "CppSQLite3.h"
#include "Logger.h"
#include <exception>
#include <windows.h>
#include <string>
#include <stdio.h>
#include "utf8conv.h"
using namespace std;
using namespace utf8util;

extern CLogger gLogger;

using namespace std;

template <typename _QueryType>
class CSqliteDB :public CDBControl<_QueryType>
{
public:
	CSqliteDB();
	virtual ~CSqliteDB(void);

	bool Connect(void);
	bool Connect(char* dbName);
	bool Connect(char* serverName, char* dbName, char* userName, char* pwd, bool isSecure);
	bool CreateTable(const char* tbname, const char* fieldstr);
	bool CreateTable(const char* sqlstr);
	bool BeginTransaction();
	bool CommitTransaction();
	int Excute(const char* sqlstr);
	_QueryType Query(const char* sqlstr);
	bool Close(void);
	__int64 GetLastRowID();
	bool TableExists(const char* tbname)
	{
		return m_db.tableExists(tbname);
	}

protected:
	bool Open(void);
	bool Open(char* dbname);
	bool Lock();
	void Unlock();

private:
	CppSQLite3DB m_db;
	HANDLE       m_hfile;
};

template <typename _QueryType>
CSqliteDB<_QueryType>::CSqliteDB()
{
	m_hfile = NULL;
}

template <typename _QueryType>
CSqliteDB<_QueryType>::~CSqliteDB(void)
{
	m_db.close();
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::Open(void)
{
	try
	{
		m_db.open(m_strDbName.c_str());
	}
	catch (CppSQLite3Exception& e)
	{
		TCHAR msg[MSG_LEN], file[FILE_LEN], func[FUNC_LEN] ;
		_stprintf_s(msg, MSG_LEN, _T("%s"), e.errorMessage());
		_stprintf_s(file, FILE_LEN, _T("%s"), __FILE__);
		_stprintf_s(func, FUNC_LEN, _T("%s"), __FUNCTION__);
		CString strMsg = msg;
		wstring wstrMsg = CT2W(strMsg);
		gLogger.QueueUpLog(wstrMsg);
		return false;
	}
	return true;
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::Open(char* dbname)
{              
	SetDBName(dbname);
	return Open();
}

template <typename _QueryType>
__int64 CSqliteDB<_QueryType>::GetLastRowID()
{
	return m_db.lastRowId();
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::Connect(void)
{
	return Open();
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::Connect(char* dbName)
{
	return Open(dbName);
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::Connect(char* serverName, char* dbName, char* userName, char* pwd, bool isSecure)
{
	return Open(dbName);
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::CreateTable(const char *tbname, const char *fieldstr)
{
	bool exists = m_db.tableExists(tbname);
	if (!exists)
	{
		std::string sqlStr("Create Table ");
		sqlStr.append(tbname);
		sqlStr.append("(");
		sqlStr.append(fieldstr);
		sqlStr.append(");");
		try
		{
			m_db.execDML(sqlStr.c_str());
		}
		catch (CppSQLite3Exception& e)
		{
			wstring wstrMsg(L"Create Table [");
			wstrMsg.append(utf8util::UTF16FromUTF8(tbname));
			wstrMsg.append(L"] failed - ");
			wstrMsg.append(utf8util::UTF16FromUTF8(e.errorMessage()));
			gLogger.QueueUpLog(wstrMsg);
			return false;
		}
	}
	return true;
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::CreateTable(const char *sqlstr)
{
	try
	{
		m_db.execDML(sqlstr);
	}
	catch (CppSQLite3Exception& e)
	{
		wstring wstrMsg = utf8util::UTF16FromUTF8(e.errorMessage());
		wstrMsg.append(L" - Create Table failed ! - ");
		wstrMsg.append(utf8util::UTF16FromUTF8(sqlstr));
		gLogger.QueueUpLog(wstrMsg);
	}
	return true;
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::BeginTransaction()
{
	int nRet = 0;
	try
	{
		nRet = m_db.execDML("BEGIN TRANSACTION");
	}
	catch(CppSQLite3Exception& e)
	{
		string strMsg = e.errorMessage();
		wstring wstrMsg = CA2W(strMsg.c_str());
		wstrMsg.append(L" - Error Code : ");
		wstring wstrErrCode = boost::lexical_cast<wstring>(e.errorCode());
		wstrMsg.append(wstrErrCode);
		gLogger.QueueUpLog(wstrMsg);
		return false;
	}
	return (nRet == 0);
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::CommitTransaction()
{
    int nRet = 0;
	try
	{
		m_db.execDML("COMMIT TRANSACTION"); 
	}
	catch(CppSQLite3Exception&)
	{
		return false;
	}
	return (nRet == 0); 
}

template <typename _QueryType>
int CSqliteDB<_QueryType>::Excute(const char* sqlstr)
{
	int ret = 0;
	try
	{
		ret = m_db.execDML(sqlstr);
	}
	catch (CppSQLite3Exception& e)
	{
		wstring wstrMsg = utf8util::UTF16FromUTF8(e.errorMessage());
		wstrMsg.append(L" - Execute Command [");
		wstrMsg.append(utf8util::UTF16FromUTF8(sqlstr));
		wstrMsg.append(L"] failed!");
		gLogger.QueueUpLog(wstrMsg);
		return ret;
	}
	return ret;
}

template <typename _QueryType>
_QueryType CSqliteDB<_QueryType>::Query(const char* sqlstr)
{
	_QueryType result;
	try
	{
		result = m_db.execQuery(sqlstr);
	}
	catch (CppSQLite3Exception& e)
	{
		wstring wstrMsg(L"Execute Query Command [");
		wstrMsg.append(utf8util::UTF16FromUTF8(sqlstr));
		wstrMsg.append(L"] failed! - ");
		wstrMsg.append(utf8util::UTF16FromUTF8(e.errorMessage()));
		gLogger.QueueUpLog(wstrMsg);
	}
	return result;
}

template <typename _QueryType>
bool CSqliteDB<_QueryType>::Close(void)
{
	if (m_hfile != NULL && INVALID_HANDLE_VALUE != m_hfile)
	{
		CloseHandle(m_hfile);
		m_hfile = NULL;
	}
	try
	{
		m_db.close();
	}
	catch(CppSQLite3Exception& e)
	{
		std::string strMsg = e.errorMessage();
		std::wstring wstrMsg = utils::Utf8ToUnicode(strMsg);
		gLogger.QueueUpLog(wstrMsg);
		return false;
	}
	return true;
}