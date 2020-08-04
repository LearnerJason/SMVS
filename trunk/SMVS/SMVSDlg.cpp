#include "stdafx.h"
#include "SMVS.h"
#include "SMVSDlg.h"
#include "afxdialogex.h"
#include "ConfigurationManager.h"
#include "SystemManager.h"
#include "LoginDlg.h"
#include "SysConfigDlg.h"
#include "UACDlg.h"
#include "HelpDlg.h"
#include "LanguageDlg.h"
#include "DialogCTypeOper.h"
#include <ATLComTime.h>
#include "DlgProductInfoInput.h"

#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSMVSDlg::CSMVSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSMVSDlg::IDD, pParent),
	m_crtIndex(-1),
	m_warningCount(-1),
	m_bStartGrab(FALSE),
	m_oldLen(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	gSystemMgr.SetMainDlgPtr(this);
	m_brush.CreateSolidBrush(RGB(200,220,250));

	int txtFontSz = 20;
	m_txtFont.CreateFont(txtFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
	ANSI_CHARSET,             
	OUT_DEFAULT_PRECIS,        
	CLIP_DEFAULT_PRECIS,       
	DEFAULT_QUALITY,           
	DEFAULT_PITCH | FF_SWISS,  
	_T("Arial"));

	
	int rjectFontSz = 22;
	m_rjectFont.CreateFont(rjectFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

void CSMVSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RJECT_INFO, m_StaRjectInfo);
	DDX_Control(pDX, IDC_STATIC_BATCHNUMBER, m_staBatchNumber);
	DDX_Control(pDX, IDC_EDIT_BATCHNUMBER, m_editBatchNumber);
	DDX_Control(pDX, IDC_STATIC_BATCHNUMBERID, m_staBatchNumberID);
}

BEGIN_MESSAGE_MAP(CSMVSDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDB_BTN_LOGIN,    &CSMVSDlg::OnStnClickedLogin)
	ON_STN_CLICKED(IDB_BTN_SETTING,  &CSMVSDlg::OnStnClickedConfiguration)
	ON_STN_CLICKED(IDB_BTN_UAC,      &CSMVSDlg::OnStnClickedUAC)
	ON_STN_CLICKED(IDB_BTN_HELP,     &CSMVSDlg::OnStnClickedHelp)
	ON_STN_CLICKED(IDB_BTN_LANGUAGE, &CSMVSDlg::OnStnClickedLanguage)
	ON_STN_CLICKED(IDB_BTN_TASKCHANGE, &CSMVSDlg::OnBnClickedButtonAutotaskchange)
	ON_STN_CLICKED(START_GRAB_BTN, &CSMVSDlg::OnStnClickedStart)
	ON_STN_CLICKED(PRIOR_BTN, &CSMVSDlg::OnStnClickedPrior)
	ON_STN_CLICKED(NEXT_BTN,  &CSMVSDlg::OnStnClickedNext)
	ON_STN_CLICKED(REFRESH_BTN, &CSMVSDlg::OnStnClickedRefresh)
	ON_STN_CLICKED(CLOSE_BTN, &CSMVSDlg::OnStnClickedClose)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(MSG_CH_ON_OFF, OnChanneOnOffOperation)
	ON_MESSAGE(WM_COMM_RXCHAR_DETECTED, OnCommunication)
	ON_WM_SYSCOMMAND()
//	ON_EN_CHANGE(IDC_EDIT_BATCHNUMBER, &CSMVSDlg::OnEnChangeEditBatchnumber)
	ON_STN_DBLCLK(IDC_STATIC_BATCHNUMBERID, &CSMVSDlg::OnStnClickedStaticBatchnumber)
//	ON_EN_KILLFOCUS(IDC_EDIT_BATCHNUMBER, &CSMVSDlg::OnKillfocusEditBatchnumber)
	ON_STN_DBLCLK(IDC_STATIC_BATCHNUMBER, &CSMVSDlg::OnStnDblclickStaticRjectInfo)
END_MESSAGE_MAP()


void CSMVSDlg::SetCurrentDatetime()
{
	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	CString strTime;
	strTime.Format(_T("%d:%.2d "), dt.wHour, dt.wMinute);
	int nLangID = gConfigMgr.GetLanguageID();
	CString strDayOfWeek = utils::GetDayOfWeekStr(nLangID,dt.wDayOfWeek);
	strTime.Append(strDayOfWeek);
	CWnd* pCtrl = GetDlgItem(IDC_STATIC_TIME);
	pCtrl->SetWindowText(strTime);
	CRect rc;
	pCtrl->GetWindowRect(rc);
	InvalidateRect(rc);

	CString strDate;
	strDate.Format(_T("%d/%.2d/%.2d"), dt.wYear,dt.wMonth, dt.wDay);
	pCtrl = GetDlgItem(IDC_STATIC_DATE);
	pCtrl->SetWindowText(strDate);
	pCtrl->GetWindowRect(rc);
	InvalidateRect(rc);
}

void CSMVSDlg::CreateDateTimePanel(CRect& rcScreen)
{
	CWnd* pCtrl = (CWnd*)GetDlgItem(IDC_STATIC_TIME);
	CRect rc,rcPos;
	pCtrl->GetWindowRect(rc);
	rcPos.left = int(rcScreen.Width() * 125.0 / 1350.0);
	rcPos.top = 15;
	rcPos.right = rcPos.left + rc.Width();
	rcPos.bottom = rcPos.top + rc.Height();
	pCtrl->MoveWindow(rcPos);
	pCtrl->SetFont(&m_txtFont);
	SetTimer(TIMER_ID_DATE_TIME,1000*60, NULL);

	rcPos.top = rcPos.bottom + 1;
	rcPos.bottom = rcPos.top + rc.Height();
	pCtrl = GetDlgItem(IDC_STATIC_DATE);
	pCtrl->MoveWindow(rcPos);
	pCtrl->SetFont(&m_txtFont);

	SetCurrentDatetime();
}

void CSMVSDlg::CreateRjectInfoPanel(CRect& rcScreen)
{
	CRect rc,rcPos,rcBacth;
	m_StaRjectInfo.GetWindowRect(rc);
	rcPos.left = int(rcScreen.Width() * 225.0 / 1350.0);
	rcPos.top = 24;
	rcPos.right = rcPos.left + rc.Width();
	rcPos.bottom = rcPos.top + rc.Height();

	m_StaRjectInfo.SetFont(&m_rjectFont);
	m_StaRjectInfo.SetWindowText(_T(""));
	m_StaRjectInfo.MoveWindow(rcPos);
	
	if (gSystemMgr.IsBatchNumberFunTrue())
	{
		rcBacth  = rcPos;
		rcBacth.top = 2;
		rcBacth.bottom = rcBacth.top +22;

		CRect rcBacth1 = rcBacth;
		rcBacth1.right =	rcBacth.left+ 135;
		rcBacth1.left+=55;

		CString cstrStaBatchNumber;
		cstrStaBatchNumber.LoadString(IDS_STRING_STA_BATCHNUMBER);
		m_staBatchNumber.SetFont(&m_rjectFont);
		m_staBatchNumber.SetWindowText(cstrStaBatchNumber);
		m_staBatchNumber.MoveWindow(rcBacth1);

		CString cstrBatchNumber = gSystemMgr.GetCurBatchNumber();
		rcBacth .left+=135;
		rcBacth.right = rcBacth.left +155;
		m_staBatchNumberID.MoveWindow(rcBacth);
		m_staBatchNumberID.SetFont(&m_rjectFont);
		m_staBatchNumberID.SetWindowText(cstrBatchNumber);

		m_staBatchNumberID.ShowWindow(SW_SHOW);
		m_staBatchNumber.ShowWindow(SW_SHOW);

		m_editBatchNumber.MoveWindow(rcBacth);
		m_editBatchNumber.ShowWindow(SW_HIDE);
	}



}

void CSMVSDlg::CreateImageControlPanel(CRect& rcScreen)
{
	float fRate = 730.0f / rcScreen.Height();
	int nTitleHeight = int(45.0f / fRate);
	int btnSz = 64;
	int y = 3;
	if(nTitleHeight >= 70)
	{
		y = int((nTitleHeight - btnSz) / 2.0f);
	}
	int left = rcScreen.right - 10 - 6 * btnSz - 4 * 5;

	CRect rcBtn;
	rcBtn.top = y;
	rcBtn.bottom = rcBtn.top + btnSz;

	rcBtn.left = left;
	rcBtn.right = rcBtn.left + btnSz;

	m_btnAutoTaskChange.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_BTN_TASKCHANGE);
	m_btnAutoTaskChange.SetLabelImage(IMG_MAIN_TASK_CHANGE);
	m_btnAutoTaskChange.EnableWindow(TRUE);

#ifndef CAP_PRODUCT
	m_btnAutoTaskChange.ShowWindow(SW_HIDE);
#endif

	rcBtn.left = rcBtn.right + 5;
	rcBtn.right = rcBtn.left + btnSz;

	m_btnLogin.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_BTN_LOGIN);
	m_btnLogin.SetLabelImage(IMG_MAIN_LOGIN);
	m_btnLogin.BringWindowToTop();
	m_btnLogin.EnableWindow(TRUE);
	m_btnLogin.Invalidate();

	rcBtn.left = rcBtn.right + 5;
	rcBtn.right = rcBtn.left + btnSz;
	m_btnSetting.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_BTN_SETTING);
		
	rcBtn.left = rcBtn.right + 5;
	rcBtn.right = rcBtn.left + btnSz;
	m_btnUAC.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_BTN_UAC);

	rcBtn.left = rcBtn.right + 5;
	rcBtn.right = rcBtn.left + btnSz;
	m_btnHelp.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_BTN_HELP);
	m_btnHelp.SetLabelImage(IMG_MAIN_HELP);
	m_btnHelp.EnableWindow(TRUE);
	m_btnHelp.Invalidate();

	rcBtn.left = rcBtn.right + 5;
	rcBtn.right = rcBtn.left + btnSz;
	m_btnLanguage.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_BTN_LANGUAGE);
}

void CSMVSDlg::CreateDialogTitlePanel(CRect& rcScreen)
{
	CWnd* pCtrl = (CWnd*)GetDlgItem(IDC_STATIC_TIME);
	CRect rcDatetime, rcLoginBtn;
	pCtrl->GetWindowRect(rcDatetime);
	m_btnAutoTaskChange.GetWindowRect(rcLoginBtn);

	m_title.SubclassDlgItem(IDC_STATIC_TITLE,this);
	std::wstring strTitle = gConfigMgr.GetBasicSetting(_T("CUSTOMISED_TITLE"));
	m_title.SetWindowText(strTitle.c_str());
	m_title.SetColor(RGB(10,10,115),RGB(171,178,185));
	double rate = rcScreen.Height() /1080.0;
	if(rate >= 1.0)
	{
		rate = 1.0;
	}
	int fontSz = (int)(60 * rate);
	int len = strTitle.length();
	if (len <=18 )
	{
			m_title.SetTxtFont(fontSz,FW_BOLD);//n.
	}
	else
	{
			m_title.SetTxtFont((int)(fontSz*0.8f),FW_BOLD);//n.
	}


	fontSz = (int)(30 * rate);
	m_warningFont.CreateFont(fontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	CRect titleRect;
	titleRect.left   = rcDatetime.right +250;
	titleRect.top    = rcLoginBtn.top;
	titleRect.right  = rcLoginBtn.left - 20 ;
	titleRect.bottom = rcLoginBtn.bottom;
	m_title.MoveWindow(titleRect);
}

void CSMVSDlg::CreateStatusBar(CRect& rcScreen, CRect& rcChart)
{
	CRect rc;

	rc.left = rcScreen.left + 3;
 	rc.top = rcScreen.bottom - 35;
	rc.right = rcChart.right-33;
 	rc.bottom = rcScreen.bottom - 2;
	int nLen = 32 * 5 + 5 * 3;
	if((rcScreen.right - rc.right) <= nLen)
	{
		rc.right = rcScreen.right - nLen;
	}
	m_DlgWarningEx.MoveWindow(rc);
	m_DlgWarningEx.SetWarningFont(&m_warningFont);
	m_DlgWarningEx.ShowWindow(SW_SHOW);

	CRect rect;
	rect.left   = rc.right + 3;
	rect.top    =  rc.bottom -36;
	rect.right  = rect.left + 32;
	rect.bottom = rc.bottom;

	m_startBtn.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY ,rect,this, START_GRAB_BTN);
	m_startBtn.SetLabelImage(IMG_START_GRAB);
	m_startBtn.Invalidate();

	rect.left = rect.right + 3;
	rect.right = rect.left + 32;
	m_priorBtn.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY ,rect,this, PRIOR_BTN);
	m_priorBtn.SetLabelImage(IMG_PRIOR);
	m_priorBtn.Invalidate();

	rect.left = rect.right + 3;
	rect.right = rect.left + 32;
	m_nextBtn.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rect, this, NEXT_BTN);
	m_nextBtn.SetLabelImage(IMG_NEXT);
	m_nextBtn.Invalidate();

	rect.left = rect.right + 3;
	rect.right = rect.left + 32;
	m_refreshBtn.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY ,rect,this, REFRESH_BTN);
	m_refreshBtn.SetLabelImage(IMG_REFRESH);
	m_refreshBtn.Invalidate();

	rect.left   = rect.right + 3;
	rect.right  = rect.left + 32;
	m_closeBtn.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rect, this, CLOSE_BTN);
	m_closeBtn.SetLabelImage(IMG_CLOSE_DLG);
	m_closeBtn.Invalidate();

}

BOOL CSMVSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	
	SetWindowText(_T("SLAC - SMVS"));

	CRect rcScreen;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcScreen,0);

	m_DlgWarningEx.Create(IDD_DIALOG_WARNING_EX,this);

	CreateDateTimePanel(rcScreen);
	CreateImageControlPanel(rcScreen);
	CreateDialogTitlePanel(rcScreen);
	CreateRjectInfoPanel(rcScreen);

	::SetWindowPos(m_hWnd,HWND_TOP,0,0, rcScreen.Width(), rcScreen.Height(), SWP_SHOWWINDOW);

	gSystemMgr.CreatePciDevice();
    gSystemMgr.InitializeSerialPortForLightController();
	gSystemMgr.CreateAllPanels();

	CRect rcChart = gSystemMgr.GetChartRect();
	CreateStatusBar(rcScreen, rcChart);
	SetButtonAccessibility();

	gSystemMgr.StartRejetorThread();

	gSystemMgr.StartCameraGroupSupervisorThread();
	gSystemMgr.TurnOnAllCameras();
	gSystemMgr.StartChannelImgProcessor();

	gSystemMgr.CheckTaskNameUniqueness();
	gSystemMgr.StartChannelStatusSentryThread();
	gSystemMgr.StartDatabaseSentryThread();
	gSystemMgr.StartImageDisplayThread();

	gSystemMgr.m_setCheckedLcID.clear();
	if (gSystemMgr.IsLcSerialPortOk())
	{
		ResetEvent(gSystemMgr.m_dLcTestOverEvent);
		gSystemMgr.SendLightControllerTestSignal();
		SetTimer(TIMER_ID_START_UP_REPORT, 4000, NULL);
	}
	
	SetTimer(TIMER_ID_SYS_CH_WARNING,5000,NULL); // Update channel warning status every 5 seconds
	SetTimer(TIMER_ID_CH_STATS, 3000, NULL);
	SetTimer(TIMER_ID_RT_SPEED,10000,NULL);

	if (gSystemMgr.IsBatchNumberFunTrue())
	{
		SetTimer(TIMER_ID_BATCH_WARNING,0,NULL);
	}

	return TRUE;
}

BOOL CSMVSDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;

	if(pMsg->wParam==VK_RETURN )
	{
 		if(GetFocus()->GetDlgCtrlID() == IDC_EDIT_BATCHNUMBER)
 		{
			ChangeBatchNumber();
		}
	}
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSMVSDlg::LoadBackgroundImage(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);

	CBitmap bakImg;
	if(bakImg.LoadBitmap(IDB_BITMAP_BAK))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		CBitmap* pPngOld = dcMem.SelectObject(&bakImg);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, rect.Width(),rect.Height(),SRCCOPY);
	}
}

void CSMVSDlg::UpdateWarningCount(int count)
{
	m_warningCount = count;
}

void CSMVSDlg::SetButtonAccessibility()
{
	UAC crtUser = gSystemMgr.GetCurrentUser();
	bool bAllChStopped = gSystemMgr.AllChannelCameraStopped();
	bool bAdmin        =  (crtUser.m_nLevel == UAC_ADMIN);

	m_btnSetting.EnableWindow(bAdmin);
	if(bAdmin)
	{
		m_btnSetting.SetLabelImage(IMG_MAIN_SETTING);
		m_btnLogin.SetLabelImage(IMG_MAIN_LOGGED_IN);
		m_refreshBtn.SetLabelImage(IMG_REFRESH);
		m_refreshBtn.EnableWindow(TRUE);
	}
	else
	{
		m_btnSetting.SetLabelImage(IMG_MAIN_SETTING_GRAY);
		m_btnLogin.SetLabelImage(IMG_MAIN_LOGIN);
		m_refreshBtn.SetLabelImage(IMG_REFRESH_GRAY);
		m_refreshBtn.EnableWindow(FALSE);
	}
	if(!bAdmin || !bAllChStopped)
	{
		m_btnLanguage.SetLabelImage(IMG_MAIN_LANGUAGE_GRAY);
		m_btnLanguage.EnableWindow(FALSE);
#ifdef CAP_PRODUCT
		m_btnAutoTaskChange.ShowWindow(SW_HIDE);
#endif
	}

	if(bAdmin && bAllChStopped)
	{
		m_btnLanguage.SetLabelImage(IMG_MAIN_LANGUAGE);
		m_btnLanguage.EnableWindow(TRUE);
#ifdef CAP_PRODUCT
		m_btnAutoTaskChange.ShowWindow(SW_SHOW);
#endif
	}
	m_refreshBtn.Invalidate();
	m_btnSetting.Invalidate();
	m_btnLanguage.Invalidate();
	//
	if(crtUser.m_strUsername == L"SMVSADMIN")
	{
		m_btnUAC.SetLabelImage(IMG_MAIN_UAC);
		m_btnUAC.EnableWindow(TRUE);
	}
	else
	{
		m_btnUAC.SetLabelImage(IMG_MAIN_UAC_GRAY);
		m_btnUAC.EnableWindow(FALSE);
	}

//	m_closeBtn.EnableWindow(bAdmin);
	if(bAdmin)
	{
		m_closeBtn.SetLabelImage(IMG_CLOSE_DLG);
	}
	else
	{
		m_closeBtn.SetLabelImage(IMG_CLOSE_DLG_GRAY);
	}
	m_btnLogin.Invalidate();
	m_closeBtn.Invalidate();
	m_btnUAC.Invalidate();
	m_btnAutoTaskChange.Invalidate();
}

void CSMVSDlg::OnPaint()
{
	CRect rect;
	GetClientRect(&rect);
	CPaintDC dc(this);
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		LoadBackgroundImage(dc);	
	}


}

HCURSOR CSMVSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSMVSDlg::ProductInfoDlgShow()
{
		CString strSysForeignKey = gSystemMgr.GetCurSysForeignKey();
		std::vector<CString> m_veckey;
		if (gSystemMgr.GetProductInfoVecter(m_veckey))
		{
			CDlgProductInfoInput dlg;
			dlg.SetProductInfoKey(m_veckey);
			dlg.SetSysForeignKey(strSysForeignKey);
			dlg.DoModal();
			CString strSysForeignKeyNew = dlg.GetSysForeignKey();
			if (strSysForeignKeyNew==strSysForeignKey)
			{
				return;
			}
			else
			{
				gSystemMgr.SetCurSysForeignKey(strSysForeignKeyNew);
				gConfigMgr.SetSysForeignKey(strSysForeignKeyNew);
			}
		}

}
void CSMVSDlg::ChangeBatchNumber()
{
	CString strBatchNumber;
	CString strBatchNumberOld = gSystemMgr.GetCurBatchNumber();
	m_editBatchNumber.GetWindowText(strBatchNumber);

	std::wregex expression;
	expression = L"^[a-zA-Z0-9_\-]+$";//\u4e00-\u9fa5
	bool ret = std::regex_match(strBatchNumber.GetString(), expression);
	if(!ret)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_BATCH_NAME_ERROR);
		::AfxMessageBox(sMsg);
		return;
	}

	if (strBatchNumberOld != strBatchNumber)
	{
		CString str;
		str.LoadString(IDS_STRING_CHANGE_BATCHNUMBER_INFO);

		if (gSystemMgr.IsUpdateDatabaseFunTrue())
		{
			CString sMsg(_T("\n请确认当前批次号相关数据是否上传数据库"));//
			str+=sMsg;
			//		sMsg.LoadString(IDS_STRINg_MSG_CHANGE_ALGO_GP);
		}
		str+=_T("\n是否继续更改批次号?");
		if(IDNO == ::AfxMessageBox(str, MB_YESNO, NULL))
		{
			m_staBatchNumberID.SetWindowText(strBatchNumberOld);
			m_editBatchNumber.ShowWindow(SW_HIDE);
			m_staBatchNumberID.ShowWindow(SW_HIDE);
			m_staBatchNumberID.ShowWindow(SW_SHOW);
			return ;
		}
//		MessageBox(str);
		gSystemMgr.ResetAllChannelData();
	}
	gSystemMgr.SetCurBatchNumber(strBatchNumber);
	m_staBatchNumberID.SetWindowText(strBatchNumber);
 
	m_editBatchNumber.ShowWindow(SW_HIDE);
	m_staBatchNumberID.ShowWindow(SW_HIDE);
	m_staBatchNumberID.ShowWindow(SW_SHOW);

	return;
}
void CSMVSDlg::BackupUserData()
{
	wstring wstrTarget = L"C:\\SMVS_bk";
	bool bExist = true;
	if(!utils::CheckFileExists(wstrTarget))
	{
		bExist = utils::CreateFullDirectory(wstrTarget);
	}
	if(bExist)
	{
		TCHAR  moduleFileName[MAX_PATH];  
		::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
		CString sConfigFile = moduleFileName;
		sConfigFile.Append(_T(".config"));
		wstring strOldFileName(sConfigFile.GetString());
		wstring strNewFileName = L"C:\\SMVS_bk\\SMVS.exe.config";
		BOOL ret = ::CopyFileW(strOldFileName.c_str(), strNewFileName.c_str(), false);

		(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
		CString sDataPath = moduleFileName;
		sDataPath.Append(_T("\data"));
		wstring wstrDataFolder = sDataPath.GetString();
		utils::CopyFiles(wstrDataFolder, L"C:\\SMVS_bk");
	}
}

void CSMVSDlg::OnStnClickedClose()
{
	CString sMsg;
	sMsg.LoadString(IDS_STRING_MSG_SHUTDOWN_SYSTEM);
	if(IDNO == AfxMessageBox(sMsg, MB_YESNO, NULL))
	{
		return;
	}

	KillTimer(TIMER_ID_CH_STATS);
	KillTimer(TIMER_ID_DATE_TIME);
	KillTimer(TIMER_ID_SYS_CH_WARNING);
	KillTimer(TIMER_ID_RT_SPEED);

	gSystemMgr.PauseAllCamera();
	gSystemMgr.m_displyManager.EnableDisplaying(false);
	gSystemMgr.StopImgProcessor();
	if(!gSystemMgr.m_vChThreadHandle.empty())
	{
		int nChCount = gSystemMgr.m_vChThreadHandle.size();
		::WaitForMultipleObjects(nChCount, &gSystemMgr.m_vChThreadHandle[0], TRUE, INFINITE);
	}

	gSystemMgr.SaveLeftoverStatisticsData();
	Sleep(1000);
	gSystemMgr.StopRejector();
	gSystemMgr.CloseSerialPort();
	gSystemMgr.StopWorkerThread();
	gSystemMgr.UpdateTaskThreshold();
	gSystemMgr.UpdateBatchNumberStr();

	if(!gSystemMgr.m_vHandle.empty())
	{
		::WaitForMultipleObjects(3, &gSystemMgr.m_vHandle[0], TRUE, INFINITE);
	}
	
	gSystemMgr.FinalClean();
	gLogger.FlushLogCache();
	//BackupUserData();
	CDialogEx::OnOK();
}

void CSMVSDlg::ShowSystemStats(SYS_STAT_DATA m_crtSysStats)
{
	if (m_crtSysStats.m_totalOkCount <= 0)
	{
		return;
	}

	//次品数为0的情况，也会更新界面，保证全是次品的情况，系统统计计数正常显示
	//if (m_crtSysStats.m_totalCount-m_crtSysStats.m_totalOkCount < 0)
	//	return;

	CString formatStr;
	formatStr.LoadString(IDS_STRING_SYS_COUNT);
	float fRate = 0.0;
	long nNG = (m_crtSysStats.m_totalCount - m_crtSysStats.m_totalOkCount)>=0?(m_crtSysStats.m_totalCount - m_crtSysStats.m_totalOkCount):0;
	if(m_crtSysStats.m_totalCount > 0)
	{
		//fRate = 100.0f * nNG / m_crtSysStats.m_totalCount;
		fRate = 100.0f * (m_crtSysStats.m_totalOkCount*1.0) / (m_crtSysStats.m_totalCount*1.0);
	}
	//m_strRjectInfo.Format(formatStr,m_crtSysStats.m_totalCount,nNG,fRate);
	m_strRjectInfo.Format(formatStr,m_crtSysStats.m_totalCount,m_crtSysStats.m_totalOkCount,fRate);
	m_StaRjectInfo.SetWindowText(m_strRjectInfo);
	int Len = m_strRjectInfo.GetLength();
	//if (Len < m_oldLen )
	{
		CRect rc;
		m_StaRjectInfo.GetWindowRect(rc);
		InvalidateRect(rc);
	}
	m_oldLen = Len;
}

void CSMVSDlg::UpdateErrorMsg()
{
    gSystemMgr.UpdateAllChannelWarningFlag();
	m_DlgWarningEx.UpdateWarningMsg();
}

LRESULT CSMVSDlg::OnChanneOnOffOperation(WPARAM wParam,LPARAM lParam)
{
	SetButtonAccessibility();
	return 0;
}

void CSMVSDlg::OnStnClickedLogin()
{
	UAC crtUser = gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);

	if(!bAdmin)
	{
		CLoginDlg loginDlg;
		if(loginDlg.DoModal() == IDOK)
		{
			loginDlg.DestroyWindow();
			gSystemMgr.EnableDataClearButton();	
		}
	}
	else
	{
		gSystemMgr.SetCurrentUser(L"OPERATOR");
		gSystemMgr.EnableDataClearButton();
	}
	SetButtonAccessibility();
}

void CSMVSDlg::OnStnClickedConfiguration()
{
	CSysConfigDlg configDlg;
	if(configDlg.DoModal())
	{
		configDlg.DestroyWindow();
	}
	bool bTaskChanged = gSystemMgr.GetTaskChangedFlag();
	if(bTaskChanged)
	{
		m_DlgWarningEx.SetWarningText(_T(""));
		UpdateData(FALSE);
		gSystemMgr.UpdateTaskSetting();
		gSystemMgr.ResetChannelTaskChangedFlag();
	}
}

void CSMVSDlg::OnStnClickedUAC()
{
	CUACDlg uacDlg;
	if(uacDlg.DoModal() == IDOK)
	{
		uacDlg.DestroyWindow();
	}
}

void CSMVSDlg::OnStnClickedHelp()
{
	CHelpDlg helpDlg;
	if(helpDlg.DoModal())
	{
		helpDlg.DestroyWindow();
	}
}

void CSMVSDlg::OnStnClickedLanguage()
{
	CLanguageDlg langDlg;
	if(langDlg.DoModal())
	{
		langDlg.DestroyWindow();
	}
}

void  CSMVSDlg::OnStnClickedNext()
{

	m_DlgWarningEx.DisplayWarningMsgNext();
}

void  CSMVSDlg::OnStnClickedPrior()
{

	m_DlgWarningEx.DisplayWarningMsgPrior();
}

void  CSMVSDlg::OnStnClickedStart()
{
	if(m_bStartGrab)
	{
		if(gSystemMgr.GetCurrentUser().m_nLevel != UAC_ADMIN)
			return;
		if(!gSystemMgr.PauseAllCameraEx())
			return;
		m_startBtn.SetLabelImage(IMG_START_GRAB);
		
	}
	else
	{		
		if(!gSystemMgr.StartAllCameraEx())
			return;
		m_startBtn.SetLabelImage(IMG_PAUSE_GRAB);		
	}
	m_startBtn.Invalidate();
	m_bStartGrab=!m_bStartGrab;

}

void  CSMVSDlg::OnStnClickedRefresh()
{
 	m_DlgWarningEx.SetWindowText(_T(""));
	UpdateData(FALSE);

	gSystemMgr.ClearAllChError();
	gSystemMgr.RefreshSystem();

	ResetEvent(gSystemMgr.m_dLcTestOverEvent);
	
	if (gSystemMgr.IsLcSerialPortOk())
	{
		ResetEvent(gSystemMgr.m_dLcTestOverEvent);
		SetTimer(TIMER_ID_START_UP_REPORT, 4000, NULL);
		gSystemMgr.SendLightControllerTestSignal();
	}

}

HBRUSH CSMVSDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	int ctrlID = pWnd->GetDlgCtrlID();
	if(ctrlID == IDC_STATIC_TIME || ctrlID == IDC_STATIC_DATE)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(10,10,115));
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	if(ctrlID == IDC_STATIC_RJECT_INFO||ctrlID ==IDC_STATIC_BATCHNUMBER||ctrlID ==IDC_STATIC_BATCHNUMBERID)
	{
		pDC->SetBkColor(RGB(171,178,185));
		pDC->SetTextColor(RGB(10,10,115));
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}

	return hbr;
}

void CSMVSDlg::OnTimer(UINT_PTR nIDEvent)
{
	//添加批次号启动提示
	if(nIDEvent == TIMER_ID_BATCH_WARNING)
	{
		KillTimer(TIMER_ID_BATCH_WARNING);
		CString strWarning;
		strWarning.LoadString(IDS_STRING_BATCH_WARNING);
		MessageBox(strWarning);

	}

	if(nIDEvent == TIMER_ID_DATE_TIME)
	{
		SetCurrentDatetime();
	}
	if(nIDEvent == TIMER_ID_CH_STATS)
	{
		gSystemMgr.UpdateChannelStats();
		ShowSystemStats(gSystemMgr.m_crtSysStats);
	}

	// 光源控制器最多16个，考虑的分路的情况，最长测试时间 3*16*80ms = 3.84s，
	// 所有情况都超时的情况下 3*16*150ms
	// WaitForSingleObject 会导致阻塞, 测试光源控制器 设置4s后测试 光源控制器链接情况！
	// 一般情况下 4s内 完全可以保证 ，光源控制的测试连接线程，特殊情况，此定时器 通过 WaitForSingleObject 4000 来延时到8s后出测试结果
	if(nIDEvent == TIMER_ID_START_UP_REPORT)
	{
		DWORD dwRet = ::WaitForSingleObject(gSystemMgr.m_dLcTestOverEvent,4000);
		gSystemMgr.SendLightControllerTestSignal();
		dwRet = ::WaitForSingleObject(gSystemMgr.m_dLcTestOverEvent,2000);
		gSystemMgr.SendLightControllerTestSignal();
		dwRet = ::WaitForSingleObject(gSystemMgr.m_dLcTestOverEvent,2000);
		gSystemMgr.VerifyLightControllerTestResult();
		//UpdateErrorMsg();
		KillTimer(TIMER_ID_START_UP_REPORT);
	}
	if (nIDEvent == TIMER_ID_SYS_CH_WARNING)
	{
		UpdateErrorMsg();
	}
	if(nIDEvent == TIMER_ID_RT_SPEED)
	{
		gSystemMgr.UpdateChannelSpeed();
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CSMVSDlg::OnCommunication(WPARAM ch, LPARAM lcID)
{
	unsigned char* pChRcv = (unsigned char*)(char*)lcID;

// 	CString str;
// 	str.Format(_T("recv lcID : %d"),*pChRcv);
// 	gLogger.QueueUpLog(str.GetString(),true);

	Sleep(10);
	string strRet = utils::bytes_to_hexstr(pChRcv,pChRcv + 2);
	string strHex(strRet.begin(), strRet.begin() + 2);
	long nLcID = ::strtol(strHex.c_str(),NULL,16);
	gSystemMgr.m_setCheckedLcID.insert(nLcID);
	SetEvent(gSystemMgr.m_hLcTestReadyEvent);
	return 0;
}

void CSMVSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID == SC_CLOSE)
	{
		return;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}
void CSMVSDlg::OnStnClickedStaticBatchnumber()
{
	//相机打开状态下，无法修改批次号
	if (gSystemMgr.IsAnyCameraRunning())
	{
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	m_editBatchNumber.ShowWindow(SW_SHOW);
	CString strBatchNumber = gSystemMgr.GetCurBatchNumber();
	m_editBatchNumber.SetWindowText(strBatchNumber);
	m_staBatchNumberID.ShowWindow(SW_HIDE);
}

void CSMVSDlg::OnBnClickedButtonAutotaskchange()
{
#ifdef CAP_PRODUCT
	CDialogCTypeOper CTypeOper;
	CString taskname1 = gSystemMgr.GetCHTaskName(1);
	CString taskname2 = gSystemMgr.GetCHTaskName(2);
	CTypeOper.SetCurTaskName(taskname1,taskname2);
	CTypeOper.DoModal();
#endif

}

void CSMVSDlg::OnStnDblclickStaticRjectInfo()
{
	ProductInfoDlgShow();
	// TODO: 在此添加控件通知处理程序代码
}
