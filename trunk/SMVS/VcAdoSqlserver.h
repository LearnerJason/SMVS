// VcAdoSqlserver.h: interface for the VcAdoSqlserver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VCADOSQLSERVER_H__DDB4F078_1A01_49F3_A44E_A608FAEFEEEC__INCLUDED_)
#define AFX_VCADOSQLSERVER_H__DDB4F078_1A01_49F3_A44E_A608FAEFEEEC__INCLUDED_

#include <iostream>  
#include "TypeDef.h"
using namespace std;  

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class VcAdoSqlserver  
{
public:
	VcAdoSqlserver();
	virtual ~VcAdoSqlserver();
private:
	static VcAdoSqlserver* instance;
public:   
	static VcAdoSqlserver * GetInstance();	
	static void DestoryInstance();
	BOOL GetConnectStr(TCHAR *Section,TCHAR *Tag,WCHAR *pszLogin);
	BOOL init();
	_RecordsetPtr m_pRecordset;//("ADODB.Recordset");
	 _ConnectionPtr m_pConnection;//("ADODB.Connection");
	//执行数据库操作
	void Execute(_bstr_t bstrSQL);
	BOOL m_databaseflag;

	BOOL UpdateDataBaseData(DATA_BASE_DATA databasedata);

	std::map<CString,CString> m_mapDatabaseMatch;

	wstring m_wstrDatabaseName;
	//
};

#endif // !defined(AFX_VCADOSQLSERVER_H__DDB4F078_1A01_49F3_A44E_A608FAEFEEEC__INCLUDED_)
