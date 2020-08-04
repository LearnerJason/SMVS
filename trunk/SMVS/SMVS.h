#pragma once
#ifndef __AFXWIN_H__
	#error "Please include stdafx.h before this file to generate PCH file."
#endif

#include "resource.h"
#include "ConfigurationManager.h"
#include "SystemManager.h"
#include "Logger.h"
#include "ui_decorator.h"
#include "LanguageProxy.h"

#include <GdiPlus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


class CSMVSApp : public CWinApp
{
public:
	CSMVSApp();

	CLanguageProxy m_languageProxy;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hSingleton;
	ULONG_PTR  m_gdiplusToken;
};

// CAUTION: DO NOT change the order of below variables
extern CSMVSApp       theApp;
extern ConfigManager  gConfigMgr;
extern CLogger        gLogger;
extern SOCKET         gPcSocket;
extern CSystemManager gSystemMgr;
extern ui_decorator   gUiDecorator;