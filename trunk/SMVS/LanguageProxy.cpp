#include "stdafx.h"
#include "resource.h"
#include "TypeDef.h"
#include "LanguageProxy.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Forces automatic link of version.lib
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

#ifndef countof
#define countof(x) (sizeof(x)/sizeof((x)[0]))
#endif

void CLanguageProxy::LoadCrtLanguage(int nLangID)
{ 
	CString dllPath;
	if(nLangID == LANG_ENU)
	{
		dllPath = _T("SMVSENU.dll");
	}
	if(nLangID == LANG_CHS)
	{
		dllPath = _T("SMVSCHS.dll");
	}
	if(nLangID == LANG_RUS)
	{
		dllPath = _T("SMVSRUS.dll");
	}
	if(m_hDll != NULL)
	{
		FreeLibrary(m_hDll);
		m_hDll= NULL;
	}

    m_hDll = LoadLibrary(dllPath);
	if (m_hDll != NULL)
	{ 
		TRACE1("Resource DLL %s loaded successfully\n",dllPath);
		::AfxSetResourceHandle(m_hDll);	
	}
}

#if _MFC_VER<0x0700 // for VC6 users. Depending on your version of the platform SDK, you may need these lines or not.
typedef LPARAM LONG_PTR;
#endif


CLanguageProxy::CLanguageProxy()
{
	m_hDll = NULL;
}

CLanguageProxy::~CLanguageProxy()
{
	if(m_hDll!=NULL)
	{
		FreeLibrary(m_hDll);
		m_hDll= NULL;
	}
}