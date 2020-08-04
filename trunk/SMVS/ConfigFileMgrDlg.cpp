#include "stdafx.h"
#include "SMVS.h"
#include "ConfigFileMgrDlg.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CConfigFileMgrDlg, CDialogEx)

CConfigFileMgrDlg::CConfigFileMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigFileMgrDlg::IDD, pParent)
	, m_bOpRslt(true)
	,TIMER_ID_CONFIG_FILE_DLG(100004)
	, m_bReplaceOldConfig(TRUE)
	, m_bImportTaskTemplate(FALSE)
	, m_bImportTask(FALSE)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CConfigFileMgrDlg::~CConfigFileMgrDlg()
{
}

void CConfigFileMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_REPLACE_ALL, m_bReplaceOldConfig);
	DDX_Check(pDX, IDC_CHECK_IMPORT_TASK_MODEL, m_bImportTaskTemplate);
	DDX_Check(pDX, IDC_CHECK_IMPORT_TASK, m_bImportTask);
}

BEGIN_MESSAGE_MAP(CConfigFileMgrDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_BTN_EXPORT, &CConfigFileMgrDlg::OnStnClickedStaticBtnExport)
	ON_STN_CLICKED(IDC_STATIC_BTN_IMPORT, &CConfigFileMgrDlg::OnStnClickedStaticBtnImport)
	ON_BN_CLICKED(IDC_CHECK_REPLACE_ALL, &CConfigFileMgrDlg::OnBnClickedCheckReplaceAll)
	ON_BN_CLICKED(IDC_CHECK_IMPORT_TASK_MODEL, &CConfigFileMgrDlg::OnBnClickedCheckImportTaskTemplate)
	ON_BN_CLICKED(IDC_CHECK_IMPORT_TASK, &CConfigFileMgrDlg::OnBnClickedCheckImportTask)
END_MESSAGE_MAP()

BOOL CConfigFileMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	InitUiWidget(IDC_STATIC_EXPORT, IDS_STRING_EXPORT);
	InitUiWidget(IDC_STATIC_FILE_NAME, IDS_STRING_FILE_NAME);
	InitUiWidget(IDC_EDIT_EXPORTED_FILE_NAME);
	InitUiWidget(IDC_STATIC_EXPORT_PROMPT, IDS_STRING_EXPORT_PROMPT);

	InitUiWidget(IDC_STATIC_IMPORT, IDS_STRING_IMPORT);
	InitUiWidget(IDC_CHECK_REPLACE_ALL);
	InitUiWidget(IDC_STATIC_CHECK_ALL,IDS_STRING_REPLACE_OLD_CONFIG);
	InitUiWidget(IDC_CHECK_IMPORT_TASK_MODEL);
	InitUiWidget(IDC_STATIC_CHECK_TEMPLATE,IDS_STRING_IMPORT_TASK_MODEL);
	InitUiWidget(IDC_CHECK_IMPORT_TASK);
	InitUiWidget(IDC_STATIC_CHECK_TASK,IDS_STRING_IMPORT_TASK);
	InitUiWidget(IDC_STATIC_IMPORT_FILE, IDS_STRING_FILE_NAME);
	InitUiWidget(IDC_EDIT_IMPORT_FILE);
	InitUiWidget(IDC_STATIC_IMPORT_PROMPT, IDS_STRING_IMPORT_PROMPT);

	InitUiWidget(IDC_STATIC_PROMPT_MSG_PANEL);


	return TRUE;
}

BOOL CConfigFileMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CConfigFileMgrDlg::InitUiWidget(int nCtrlID, int strID)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	if(pWnd != nullptr)
	{
		pWnd->SetFont(&m_textFont);
		if(strID != NULL)
		{
			CString str;
			str.LoadString(strID);
			pWnd->SetWindowText(str);
		}
		pWnd->Invalidate();
	}
}

void CConfigFileMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_PROMPT_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_PROMPT_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_CONFIG_FILE_DLG,5000, NULL);
}

void CConfigFileMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_CONFIG_FILE_DLG)
	{
		SetDlgItemTextW(IDC_STATIC_PROMPT_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_CONFIG_FILE_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}

HBRUSH CConfigFileMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_CHECK_REPLACE_ALL ||
	   id == IDC_CHECK_IMPORT_TASK_MODEL ||
	   id == IDC_CHECK_IMPORT_TASK ||
	   id == IDC_STATIC_PROMPT_MSG_PANEL)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();

		if(id == IDC_STATIC_PROMPT_MSG_PANEL)
		{
			if(m_bOpRslt)
				pDC->SetTextColor(RGB(50,200,20));
			else
				pDC->SetTextColor(RGB(200,20,20));
		}

		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CConfigFileMgrDlg::OnStnClickedStaticBtnExport()
{
	CString sMsg;
	CString sFileName;
	GetDlgItemTextW(IDC_EDIT_EXPORTED_FILE_NAME, sFileName);
	if(sFileName.GetLength() <= 0)
	{
		sMsg.LoadString(IDS_STRING_MSG_INPUT_FILE_NAME);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	std::wstring str = sFileName.GetString();
	std::wregex expression;


	expression =L"^[a-zA-Z0-9_]+$";
	BOOL ret = std::regex_match(str, expression);
	if (!ret)
	{
		sMsg.LoadString(IDS_STRING_MSG_INPUT_FILENAME_ERROR);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	CString sConfigFile = moduleFileName;
	sConfigFile.Append(_T(".config"));
	wstring strOldFileName(sConfigFile.GetString());

	TCHAR sPath[MAX_PATH];
	SHGetSpecialFolderPath(0, sPath, CSIDL_DESKTOPDIRECTORY, 0);
	CString strPath = sPath;
	CString sFile = strPath + _T("\\") + sFileName + _T(".config");
	wstring strNewFileName(sFile.GetString());

	ret = ::CopyFileW(strOldFileName.c_str(), strNewFileName.c_str(), false);
	if(ret == FALSE)
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	// copy data folder
	(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
	CString sDataPath = moduleFileName;
	sDataPath.Append(_T("\data"));
	wstring wstrDataFolder = sDataPath.GetString();

	CString strDesktop = sPath;
	wstring wstrDesktop = strDesktop.GetString();
	if(!utils::CopyFiles(wstrDataFolder,wstrDesktop))
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	sMsg.LoadString(IDS_STRING_OPERATION_OK);
	DisplayPromptMessage(sMsg, true);
}

void CConfigFileMgrDlg::OnStnClickedStaticBtnImport()
{
	UpdateData(TRUE);
	CString sMsg;

	CString strFilter(_T("Configuration File (*.config)|*.config||"));
	CFileDialog fileDlg(TRUE, _T("config"),_T("*.config"), OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, strFilter, this);
	if(fileDlg.DoModal() == IDOK)
	{
		TCHAR  moduleFileName[MAX_PATH];  
		::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
		CString sConfigFile = moduleFileName;
		sConfigFile.Append(_T(".config"));
		wstring strDstFileName = CT2W(sConfigFile);

		CString strSrcFile = fileDlg.GetPathName();
		wstring strSrcConfigFile = CT2W(strSrcFile);

		SetDlgItemTextW(IDC_EDIT_IMPORT_FILE, strSrcFile);

		if(m_bReplaceOldConfig == TRUE)
		{
			BOOL ret = ::CopyFileW(strSrcConfigFile.c_str(), strDstFileName.c_str(), false);
			if(ret == FALSE)
			{
				sMsg.LoadString(IDS_STRING_OPERATION_NG);
				DisplayPromptMessage(sMsg, false);
				return;
			}
			sMsg.LoadString(IDS_STRING_OPERATION_OK);
			DisplayPromptMessage(sMsg, true);
		}
		else
		{
			ConfigManager tmpMgr(strSrcConfigFile);
			bool bOpRslt = true;
			if(m_bImportTaskTemplate)
			{
				std::map<wstring, TASK_MODEL>*  pNewTaskModelArray = tmpMgr.GetTaskTemplateArrayPtr();
				std::map<wstring, TASK_MODEL>*  pCrtTaskModelArray = ::gConfigMgr.GetTaskTemplateArrayPtr();
				for(auto it = pNewTaskModelArray->begin(); it != pNewTaskModelArray->end(); it++)
				{
					TASK_MODEL taskModel = it->second;
					auto ret = pCrtTaskModelArray->find(taskModel.m_strModelName);
					if(ret == pCrtTaskModelArray->end())
					{
						bOpRslt &= gConfigMgr.AddTaskTemplate(taskModel);
					}
					else
					{
						bOpRslt &= gConfigMgr.ModifyTaskTemplate(ret->second, taskModel);
					}
				}
				if(!bOpRslt)
				{
					sMsg.LoadString(IDS_STRING_OPERATION_NG);
					DisplayPromptMessage(sMsg, bOpRslt);
					return;
				}
			}
			if(m_bImportTask)
			{
				std::map<int, CHANNEL_CONFIG>* pNewChConfigArray = tmpMgr.GetChannelConfigPtr();
				std::map<int, CHANNEL_CONFIG>* pCrtChConfigArray = ::gConfigMgr.GetChannelConfigPtr();

				if(pNewChConfigArray->size() != pCrtChConfigArray->size())
				{
					sMsg.LoadString(IDS_STRING_IMPORT_FAILED_CH_COUNT_NOT_EQUAL);
					DisplayPromptMessage(sMsg, false);
					return;
				}
				else
				{
					for(auto it = pNewChConfigArray->begin(); it != pNewChConfigArray->end(); it++)
					{
						int chIndex = it->first;
						CHANNEL_CONFIG chNewConfig = it->second;
						if(pCrtChConfigArray->find(chIndex) != pCrtChConfigArray->end())
						{
							CHANNEL_CONFIG crtChConfig = (*pCrtChConfigArray)[chIndex];

							for(auto taskIt = chNewConfig.m_vTask.begin(); taskIt != chNewConfig.m_vTask.end(); taskIt++)
							{
								auto retIt = std::find_if(crtChConfig.m_vTask.begin(), crtChConfig.m_vTask.end(), [&](TASK task) -> bool
								{
									return taskIt->m_wstrTaskName == task.m_wstrTaskName;
								});
								if(retIt  != crtChConfig.m_vTask.end())
								{
									bOpRslt &= gConfigMgr.UpdateTask(chIndex, *taskIt);
								}
								else
								{
									bOpRslt &= gConfigMgr.AddTask(chIndex, *taskIt);
								}
							}
						}
					}
					if(!bOpRslt)
					{
						sMsg.LoadString(IDS_STRING_OPERATION_NG);
						DisplayPromptMessage(sMsg, bOpRslt);
						return;
					}
				}
			}

			if(bOpRslt)
			{
				sMsg.LoadString(IDS_STRING_OPERATION_OK);
				DisplayPromptMessage(sMsg, bOpRslt);
			}
		}
	}
}

void CConfigFileMgrDlg::OnBnClickedCheckReplaceAll()
{
	UpdateData(TRUE);
	if(m_bReplaceOldConfig == TRUE)
	{
		m_bImportTask      = FALSE;
		m_bImportTaskTemplate = FALSE;
	}
	UpdateData(FALSE);
}

void CConfigFileMgrDlg::OnBnClickedCheckImportTaskTemplate()
{
	UpdateData(TRUE);
	if(m_bImportTaskTemplate == TRUE)
	{
		m_bReplaceOldConfig = FALSE;
	}
	UpdateData(FALSE);
}

void CConfigFileMgrDlg::OnBnClickedCheckImportTask()
{
	UpdateData(TRUE);
	if(m_bImportTask == TRUE)
	{
		m_bReplaceOldConfig = FALSE;
	}
	UpdateData(FALSE);
}
