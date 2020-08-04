// VcAdoSqlserver.cpp: implementation of the VcAdoSqlserver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VcAdoSqlserver.h"
#include "utils.h"
#include <fstream>
#include <iostream>
//#include "Common.h"
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString VariantToString(VARIANT var)
{
	CString strValue;
	_variant_t var_t;
	_bstr_t bstr_t;
	time_t cur_time;
	CTime time_value;
	COleCurrency var_currency;
	switch(var.vt)
	{
	case VT_EMPTY:
	case VT_NULL:strValue=_T("");break;
	case VT_UI1:strValue.Format(_T("%d"),var.bVal);break;
	case VT_I2:strValue.Format(_T("%d"),var.iVal);break;
	case VT_I4:strValue.Format(_T("%d"),var.lVal);break;
	case VT_R4:strValue.Format(_T("%f"),var.fltVal);break;
	case VT_R8:strValue.Format(_T("%f"),var.dblVal);break;
	case VT_CY:
		var_currency=var;
		strValue=var_currency.Format(0);break;
	case VT_BSTR:
		var_t =var;
		bstr_t=var_t;
		strValue.Format(_T("%s"),(const WCHAR *)bstr_t);break;
	case VT_DATE:
		cur_time=var.date;
		time_value=cur_time;
		strValue.Format(_T("%A,%B,%d,%Y"));break;
	case VT_BOOL:strValue.Format(_T("%d"),var.boolVal);break;
	default:strValue=_T("");break;
	}
	return strValue;
}

VcAdoSqlserver* VcAdoSqlserver::instance=NULL;

VcAdoSqlserver* VcAdoSqlserver::GetInstance()
{
	if( instance == NULL)
	{
		instance = new VcAdoSqlserver();
	}

	instance->init();

	return instance;   
}

void VcAdoSqlserver::DestoryInstance()
{
	if (instance != NULL) {
		delete instance;
		instance = NULL;
	}
}

void VcAdoSqlserver::Execute(_bstr_t bstrSQL)
{
	try
    {
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
    }
	catch (_com_error e)
    {
		m_databaseflag = FALSE;
		AfxMessageBox(e.Description());
 		m_pConnection->Close();
 		m_pConnection.Release();
 		init();
		if (m_databaseflag == TRUE)
		{
			m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		}
		return;
    }
	m_databaseflag = TRUE;
}

BOOL VcAdoSqlserver::GetConnectStr(TCHAR *Section,TCHAR *Tag,WCHAR *pszLogin){
	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	CString StrSet = moduleFileName;
	int index = StrSet.ReverseFind('\\');
	StrSet.Truncate(index + 1);
	StrSet.Append(_T("database.ini"));	
	if(GetPrivateProfileString(Section,Tag,NULL,pszLogin,256,StrSet)>0)
		return TRUE;
	return FALSE;
}

BOOL VcAdoSqlserver::UpdateDataBaseData(DATA_BASE_DATA databasedata)
{
	if (!m_databaseflag)
	{
		return FALSE;
	}

	try
	{
		
		WCHAR szTable[256]={0};
		if(!GetConnectStr(_T("sqlserver"),_T("Chart"),szTable)) 
			return FALSE;
		wstring wstrsz(szTable);
// 		bstr_t bstrSQL = "sp_columns ASI021";
// 		string bInsertStrSQL = "insert into ASI021 values(";
		bstr_t bstrSQL = "sp_columns ";
		bstrSQL += utils::Unicode2Ansi(wstrsz).c_str();

		string bInsertStrSQL = "insert into ";
		bInsertStrSQL+=utils::Unicode2Ansi(wstrsz).c_str();
		bInsertStrSQL +=" values(";

		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		//	m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfaceP tr(),adOpenDynamic,adLockOptimistic,adCmdText);
		int   count=0;	
		while(!m_pRecordset-> EndOfFile) 
		{ 
			CString	ColumName = ::VariantToString(m_pRecordset->GetCollect("COLUMN_NAME"));
			CString	TypeName = ::VariantToString(m_pRecordset->GetCollect("TYPE_NAME"));
			if (TypeName == _T("int identity"))
			{
				m_pRecordset-> MoveNext(); 
				continue;
			}
			CString ColumnNameNew;
			if (m_mapDatabaseMatch.find(ColumName)!= m_mapDatabaseMatch.end())
			{
				 ColumnNameNew = m_mapDatabaseMatch[ColumName];
			}
			else
			{
				ColumnNameNew = ColumName;
			}
			if(databasedata.m_mapErrorCount.find(ColumnNameNew)!=databasedata.m_mapErrorCount.end())
			{
				CString str;
				str.Format(_T(",'%d'"),databasedata.m_mapErrorCount[ColumnNameNew]);
				bInsertStrSQL +=	utils::Unicode2Ansi(str.GetString());
			}
			else
			{
				if (ColumnNameNew == _T("UserName"))
				{
					bInsertStrSQL+="'";
					bInsertStrSQL+=utils::Unicode2Ansi(databasedata.UserName.GetString());
					bInsertStrSQL+="'";
				}
				else if (ColumnNameNew == _T("Recheck"))
				{
					bInsertStrSQL+=",'";
					bInsertStrSQL+=utils::Unicode2Ansi(databasedata.Recheck.GetString());
					bInsertStrSQL+="'";
				}
				else if (ColumnNameNew == _T("Reserved1"))
				{
					bInsertStrSQL+=",'";
					bInsertStrSQL+=utils::Unicode2Ansi(databasedata.Reserved1.GetString());
					bInsertStrSQL+="'";
				}
				else if (ColumnNameNew == _T("ProductName"))
				{
					bInsertStrSQL+=",'";
					bInsertStrSQL+=utils::Unicode2Ansi(databasedata.ProductName.GetString());
					bInsertStrSQL+="'";
				}
				else if (ColumnNameNew == _T("ProductBatch"))
				{
					bInsertStrSQL+=",'";
					bInsertStrSQL+=utils::Unicode2Ansi(databasedata.ProductBatch.GetString());
					bInsertStrSQL+="'";
				}
				else if (ColumnNameNew == _T("StartTime"))
				{
					bInsertStrSQL+=",'";
					bInsertStrSQL+= utils::Unicode2Ansi(databasedata.StartTime.GetString());
					bInsertStrSQL+="'";
				}
				else if (ColumnNameNew == _T("EndTime"))
				{
					bInsertStrSQL+=",'";
					bInsertStrSQL+=utils::Unicode2Ansi(databasedata.EndTime.GetString());
					bInsertStrSQL+="'";
				}
				else if (ColumnNameNew == _T("Total"))
				{
					CString str;
					str.Format(_T(",'%d'"),databasedata.Total);
					bInsertStrSQL+=utils::Unicode2Ansi(str.GetString());
				}
				else if (ColumnNameNew == _T("Qualified"))
				{
					CString str;
					str.Format(_T(",'%d'"),databasedata.Qualified);
					bInsertStrSQL+=utils::Unicode2Ansi(str.GetString());
				}
				else if (ColumnNameNew == _T("Rejected"))
				{
					CString str;
					str.Format(_T(",'%d'"),databasedata.Rejected);
					bInsertStrSQL+=utils::Unicode2Ansi(str.GetString());
				}
				else if (ColumnNameNew == _T("Passrate"))
				{
					CString str;
					str.Format(_T(",'%.4f'"),databasedata.Passrate);
					bInsertStrSQL+=utils::Unicode2Ansi(str.GetString());
				}
				else
				{
					bInsertStrSQL+=",''";
				}

			}
			m_pRecordset-> MoveNext(); 
		} 

		bInsertStrSQL+=")";
		m_pRecordset->Close();

		m_pConnection->Execute((_bstr_t)(bInsertStrSQL.c_str()),NULL,adCmdText);
	}
	catch(_com_error e)  
	{  
		AfxMessageBox(e.Description());  
		return FALSE;  
	}  

	return TRUE;
}

BOOL VcAdoSqlserver::init()
{	
	if (m_databaseflag)
	{
		return TRUE;
	}
	::CoInitialize(NULL); // 初始化OLE/COM库环境 ，为访问ADO接口做准备 

	//创建链接对象
	HRESULT hr = m_pConnection.CreateInstance(__uuidof(Connection));
	if (FAILED(hr))
		return FALSE;

	//创建记录集对象
	hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
		return FALSE;
	
	//设置链接字符串
//	_bstr_t strConnect = "Provider=SQLOLEDB; Server=172.27.2.196,1433; Database=master; uid=sa; pwd=slac1028";
//	_bstr_t strConnect = "Provider=SQLOLEDB; Server=(local); Database=master; uid=sa; pwd=slac1028";

//读取配置文件，数据库连接字符串！
	WCHAR szLogin[256]={0};
	if(GetConnectStr(_T("sqlserver"),_T("strConnect"),szLogin)==FALSE)
		return FALSE;
	wstring wstrsz(szLogin);
	_bstr_t strConnect= utils::Unicode2Ansi(wstrsz).c_str();

	int i = wstrsz.find(_T("Database="));
	int j = wstrsz.find(_T(";"),i);
	
	m_wstrDatabaseName = wstrsz.substr(i+9,j-i-9);
	try
	{
		// Open方法连接字串必须四BSTR或者_bstr_t类型
		hr = m_pConnection->Open(strConnect, "", "", adModeUnknown);	
	}
	catch(_com_error e)
	{
		CString errormessage;
		errormessage.Format(_T("连接数据库失败!\r\n错误信息:%s"),e.ErrorMessage());
		AfxMessageBox(errormessage);///显示错误信息
		m_databaseflag = FALSE;
		return FALSE;
	}

	m_databaseflag = TRUE;
//	AfxMessageBox("连接成功！");
	
// 	//创建自己的数据库
//    _bstr_t MySQL("if not exists(select * from sysdatabases where name='SMVS') Create database SMVS");
//    m_pConnection->Execute((_bstr_t)MySQL,NULL,adCmdText);	
// 
	m_wstrDatabaseName = wstrsz.substr(i+9,j-i-9);

	bstr_t MySQL("use ");
	MySQL += utils::Unicode2Ansi(m_wstrDatabaseName).c_str();

   m_pConnection->Execute((_bstr_t)MySQL,NULL,adCmdText);	

	return TRUE;
}


VcAdoSqlserver::VcAdoSqlserver()
{
	m_wstrDatabaseName= _T("");
	//初始化数据库映射
	m_databaseflag = FALSE;
	string filename = "DatabaseMatch.txt";
	fstream fin;
	fin.open(filename.c_str(), ios::in);


	string DatabaseName;
	string TestUnitName;
	while (!fin.eof() )  
	{  
		fin>>TestUnitName>>DatabaseName;
		CString strDatabaseName(DatabaseName.c_str());
		CString strTestUnitName(TestUnitName.c_str());
		m_mapDatabaseMatch[strDatabaseName] =strTestUnitName;
	} 
	fin.close();
}

VcAdoSqlserver::~VcAdoSqlserver()
{
//关闭记录集和连接  
	if (m_pRecordset->GetState() == adStateOpen)
	{
		m_pRecordset->Close();
	}
	if (m_pRecordset)
	{
		m_pRecordset.Release();
		m_pRecordset = NULL;
	}
	if (m_pConnection->GetState() == adStateOpen)
	{
		m_pConnection->Close();
	}
	if (m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	 }
	//释放环境  
	::CoUninitialize(); 

}
