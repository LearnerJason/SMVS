
#include "stdafx.h"
#include "ModelTool.h"
#include "ModelToolDlg.h"
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CModelToolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CModelToolApp::CModelToolApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CModelToolApp  theApp;
ui_decorator   gUiDecorator;


void PathAdaptToModelFiles(wstring &path,wstring wstr)
{
	if(path.find(L".shm") == wstring::npos)
	{
		if(path.find(L".reg") == wstring::npos)
		{
			return;
		}
	}

	wstr +=L"\\";  
	std::wregex expression;
	expression = L"^[a-zA-Z]:(((\\\\(?! )[^/:*?<>\\\"\"|\\\\]+)+\\\\?)|(\\\\)?)\\s*$";

	bool ret = std::regex_match(path, expression);
	if(ret)
	{
		int i = path.rfind(L"\\");
		path.replace(i,1,wstr);
		return;
	}
	return;
}

void ModelFilesAdaptToPath(wstring &path)
{
	CString str(path.c_str());

	int i = str.ReverseFind('#');
	int j = str.ReverseFind('\\');
	CString strpath = str.Left(i)+str.Mid(j);
	path = strpath.GetString();

	return;
}

BOOL CModelToolApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));	
	CString strCmd = GetCommandLine();
	
//	CString strCmd =_T("C:\\SMVS_MCS\\trunk\\x64\\Debug\\ModelTool.exe C:\\tmp.tiff#C:\\Program Files\\SMVS\data\\CH2\\A模测量CH2\\尺寸\\尺寸.shm@394082@0@CHS");//A模测量CH2\尺寸\尺寸.shm
	std::wstring wstrCmd = strCmd.GetString();
	wstring wstrParam     = L"";
	wstring wstrWndHandle = L"";
	wstring wstrFileType  = L"0";
	wstring wstrLangID    = L"";
	wstring wstrMatchingType    = L"2";
	wstring wstrModelFilesIndex = L"";
	bool    bGetMatchingType=false;
	bool    bGetModefFiles=false;
	CModelToolDlg dlg;
	if(!wstrCmd.empty())
	{
		std::vector<wstring> vItem = SplitString(wstrCmd, L"@");
		if(vItem.size() >= 4)
		{
			wstrCmd       = vItem[0];
			wstrWndHandle = vItem[1];
			wstrFileType  = vItem[2];
			std::vector<wstring> vItemInter = SplitString(vItem[3], L"#");
			if(vItemInter.size()==3)
			{
				vItem[3] = vItemInter[0];
				wstrLangID    = vItem[3];
				dlg.SetModelAngle(vItemInter[1]);

				std::vector<wstring> vItemCenterXY = SplitString(vItemInter[2], L",");
				wstring	wstrAngleCenter ;
				wstring	wstrColCenter ;
				wstring	wstrRowCenter ;
				if(vItemCenterXY.size() == 3)
				{
					wstrColCenter      = vItemCenterXY[0];
					wstrRowCenter      = vItemCenterXY[1];
					wstrAngleCenter    = vItemCenterXY[2];
				
					double colCenter = boost::lexical_cast< double>(wstrColCenter);
					double rowCenter = boost::lexical_cast<double>(wstrRowCenter);
					
					dlg.GetCenterXY(rowCenter,colCenter);
				}
			}
			else if(vItemInter.size()==4)
			{
				vItem[3] = vItemInter[0];
				wstrLangID    = vItem[3];
				dlg.SetModelAngle(vItemInter[1]);
				dlg.SetCreateModelMethod(vItemInter[2]);

				std::vector<wstring> vItemCenterXY = SplitString(vItemInter[3], L",");
				wstring	wstrAngleCenter ;
				wstring	wstrColCenter ;
				wstring	wstrRowCenter ;
				if(vItemCenterXY.size() == 3)
				{
					wstrColCenter      = vItemCenterXY[0];
					wstrRowCenter      = vItemCenterXY[1];
					wstrAngleCenter    = vItemCenterXY[2];

					double colCenter = boost::lexical_cast< double>(wstrColCenter);
					double rowCenter = boost::lexical_cast<double>(wstrRowCenter);

					dlg.GetCenterXY(rowCenter,colCenter);
				}
			}
			else
			{
				wstrLangID    = vItem[3];
			}
		
			//
			if (vItem.size()== 5)
			{
				CString str5 = vItem[4].c_str();
				if (str5.Left(1)==_T("#"))
				{
					bGetModefFiles =  true;
					wstrModelFilesIndex =  str5.GetString();
				}
				else
				{
					bGetMatchingType=true;
					wstrMatchingType= vItem[4];
				}	
			}
			if (vItem.size()== 6)
			{
				bGetMatchingType=true;
				wstrMatchingType= vItem[4];

				CString str5 = vItem[5].c_str();
				if (str5.Left(1)==_T("#"))
				{
					bGetModefFiles =  true;
					wstrModelFilesIndex =  str5.GetString();
				}	
			}
			//
			size_t it = wstrCmd.find(L".exe");
			if(it != wstring::npos)
			{
				size_t itBgn = wstrCmd.find(L":", it);
				if(itBgn != wstring::npos)
				{
					wstrParam = wstrCmd.substr(itBgn - 1);
				}
			}
		}
	}


	if(wstrParam != L"")
	{
		auto it = wstrParam.find(L"#");
		if(it != wstring::npos)
		{
			if(wstrWndHandle != L"")
			{
				long long lHandle = boost::lexical_cast<long long>(wstrWndHandle);
				dlg.SetChannelWindowHandle((HWND)lHandle);
			}
			bool bChs = (wstrLangID == L"CHS");
			dlg.SetChsLangFlag(bChs);

			wstring wstrImgFile = wstrParam.substr(0, it);
			dlg.SetImageFilePath(wstrImgFile);
			wstring wstrTargetFilePath = wstrParam.substr(it + 1);
			dlg.SetTargetFilePath(wstrTargetFilePath);
			dlg.SetTargetFileType(wstrFileType);
			//no matching state
			if (bGetMatchingType)
			{
				dlg.SetTargetMatchingType(wstrMatchingType);
			}
			if (bGetModefFiles)
			{
				dlg.SetModelFiles(wstrModelFilesIndex);
			}

			//
		}
	}
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	return FALSE;
}

