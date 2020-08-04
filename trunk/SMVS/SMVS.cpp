#include "stdafx.h"
#include "SMVS.h"
#include "LoginDlg.h"
#include "SMVSDlg.h"
//#include "dumf.h"
#include <WinBase.h>
#include "cpp/HalconCpp.h"
#include "utils.h"
#include <fstream>
#include <string>

using namespace std;
using namespace Halcon;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSMVSApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CSMVSApp       theApp;
ConfigManager  gConfigMgr;
CLogger        gLogger;
SOCKET         gPcSocket;
CSystemManager gSystemMgr;
ui_decorator   gUiDecorator;

class StackWalkerToConsole : public StackWalker
{
protected:
	virtual void OnOutput(LPCSTR szText) { 
	//	printf("%s", szText); 
		SYSTEMTIME dt;
		::GetLocalTime(&dt);
		CString sTimestamp;
			sTimestamp.Format(_T("%2d-%2d | [%02d:%02d:%02d]    "),dt.wMonth,dt.wDay,dt.wHour, dt.wMinute,dt.wSecond);

//		::AfxMessageBox(str);
		ofstream  ofile("unhandle.txt",ios::app);
		ofile<<utils::Unicode2Ansi(sTimestamp.GetString()).c_str()<< szText <<endl;
	}
};


#if defined(_M_X64) || defined(_M_IX86)
static BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	if (hKernel32 == NULL)
		return FALSE;
	void* pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if (pOrgEntry == NULL)
		return FALSE;

#ifdef _M_IX86
	// Code for x86:
	// 33 C0                xor         eax,eax
	// C2 04 00             ret         4
	unsigned char szExecute[] = {0x33, 0xC0, 0xC2, 0x04, 0x00};
#elif _M_X64
	// 33 C0                xor         eax,eax
	// C3                   ret
	unsigned char szExecute[] = {0x33, 0xC0, 0xC3};
#else
#error "The following code only works for x86 and x64!"
#endif

	DWORD dwOldProtect = 0;
	BOOL  bProt = VirtualProtect(pOrgEntry, sizeof(szExecute), PAGE_EXECUTE_READWRITE, &dwOldProtect);

	SIZE_T bytesWritten = 0;
	BOOL   bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, szExecute, sizeof(szExecute),
		&bytesWritten);

	if ((bProt != FALSE) && (dwOldProtect != PAGE_EXECUTE_READWRITE))
	{
		DWORD dwBuf;
		VirtualProtect(pOrgEntry, sizeof(szExecute), dwOldProtect, &dwBuf);
	}
	return bRet;
}
#else
#pragma message("This code works only for x86 and x64!")
#endif

static TCHAR s_szExceptionLogFileName[_MAX_PATH] = _T("\\exceptions.log"); // default
static BOOL  s_bUnhandledExeptionFilterSet = FALSE;
static LONG __stdcall CrashHandlerExceptionFilter(EXCEPTION_POINTERS* pExPtrs)
{
#ifdef _M_IX86
	if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW)
	{
		static char MyStack[1024 * 128]; // be sure that we have enough space...
		// it assumes that DS and SS are the same!!! (this is the case for Win32)
		// change the stack only if the selectors are the same (this is the case for Win32)
		//__asm push offset MyStack[1024*128];
		//__asm pop esp;
		__asm mov eax, offset MyStack[1024 * 128];
		__asm mov esp, eax;
	}
#endif

	StackWalkerToConsole sw; // output to console
	sw.ShowCallstack(GetCurrentThread(), pExPtrs->ContextRecord);
	TCHAR lString[500];
	_stprintf_s(lString,
		_T("*** Unhandled Exception! See console output for more infos!\n")
		_T("   ExpCode: 0x%8.8X\n")
		_T("   ExpFlags: %d\n")
#if _MSC_VER >= 1900
		_T("   ExpAddress: 0x%8.8p\n")
#else
		_T("   ExpAddress: 0x%8.8X\n")
#endif
		_T("   Please report!"),
		pExPtrs->ExceptionRecord->ExceptionCode, pExPtrs->ExceptionRecord->ExceptionFlags,
		pExPtrs->ExceptionRecord->ExceptionAddress);
	FatalAppExit(-1, lString);
//	return EXCEPTION_CONTINUE_SEARCH;
	return EXCEPTION_CONTINUE_EXECUTION;
}

static void InitUnhandledExceptionFilter()
{
	TCHAR szModName[_MAX_PATH];
	if (GetModuleFileName(NULL, szModName, sizeof(szModName) / sizeof(TCHAR)) != 0)
	{
		_tcscpy_s(s_szExceptionLogFileName, szModName);
		_tcscat_s(s_szExceptionLogFileName, _T(".exp.log"));
	}
	if (s_bUnhandledExeptionFilterSet == FALSE)
	{
		// set global exception handler (for handling all unhandled exceptions)
		SetUnhandledExceptionFilter(CrashHandlerExceptionFilter);
#if defined _M_X64 || defined _M_IX86
		PreventSetUnhandledExceptionFilter();
#endif
		s_bUnhandledExeptionFilterSet = TRUE;
	}
}


CSMVSApp::CSMVSApp()
{
}



LONG WINAPI TopLevelUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	StackWalker sw;
	sw.ShowCallstack();

	LONG nRet = -1;
	gLogger.QueueUpLog(L"Unhandled Exception!",true);
	return nRet;
}

BOOL CSMVSApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	
	InitUnhandledExceptionFilter();
	CWinApp::InitInstance();
	AfxEnableControlContainer();
	
	int nLangID = gConfigMgr.GetLanguageID();
	m_languageProxy.LoadCrtLanguage(nLangID);

	m_hSingleton = ::CreateMutexW(0, true, _T("Singleton"));
	if(m_hSingleton != NULL && ERROR_ALREADY_EXISTS == GetLastError())
	{
		::ReleaseMutex(m_hSingleton);
		CString sMsg;
		sMsg.LoadString(IDS_STRING_CANNOT_RUN_MULTI_SMVS);
		AfxMessageBox(sMsg, MB_OK|MB_ICONWARNING, NULL);
		return FALSE;
	}

	CShellManager *pShellManager = new CShellManager;

	BOOL ret = ::SetPriorityClass(::GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
	if(ret == FALSE)
	{
		//TODO:
	}
	// Enhance performance of Halcon
	set_system("clip_region", "false");
	set_system("thread_num", 1);
	//auto ret_val =set_system("thread_num", "default");

	GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::Status st = GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	if(st != Ok)
	{
		return FALSE;
	}

	SetRegistryKey(_T("SLAC - Machine Vision System(SMVS)"));

//º”√‹
//#ifndef EOE_PRODUCT
//	if(gSystemMgr.m_dongleKey.VerifyAuthorization())
//	{
//#endif
		CSMVSDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
		}
		else if (nResponse == IDCANCEL)
		{
		}
//#ifndef EOE_PRODUCT
//	}
//#endif
	
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	return FALSE;
}

int CSMVSApp::ExitInstance()
{
//#ifndef EOE_PRODUCT
//	gSystemMgr.m_dongleKey.OnSmvsShutdown();
//#endif
	GdiplusShutdown(m_gdiplusToken);
	::ReleaseMutex(m_hSingleton);
	::CloseHandle(m_hSingleton);
	return CWinApp::ExitInstance();
}



