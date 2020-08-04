#include "stdafx.h"
#include "SMVS.h"
#include "NameModifierDlg.h"
#include "afxdialogex.h"
#include "utils.h"


IMPLEMENT_DYNAMIC(CNameModifierDlg, CDialogEx)

CNameModifierDlg::CNameModifierDlg(wstring wstrOldName, set<wstring> setNickname, wstring& retNewName, CWnd* pParent /*=NULL*/)
	: CDialogEx(CNameModifierDlg::IDD, pParent),
	  m_wstrOldName(wstrOldName),
	  m_retNewName(retNewName)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_setNickname = setNickname;
}

CNameModifierDlg::~CNameModifierDlg()
{
}

void CNameModifierDlg::InitUiWidget(int nCtrlID, int strID)
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

BOOL CNameModifierDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rcScreen;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcScreen,0);
	CRect rc;
	GetWindowRect(rc);
	int x = (rcScreen.Width() - rc.Width()) / 2 + rc.Width() + 100;
	int y = (rcScreen.Height() - rc.Height()) / 2;
	::SetWindowPos(m_hWnd,HWND_TOP, x ,y, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	InitUiWidget(IDC_STATIC_OLD_NAME,IDS_STRING_OLD_NAME);
	InitUiWidget(IDC_EDIT_OLD_NAME);
	InitUiWidget(IDC_STATIC_NEW_NAME,IDS_STRING_NEW_NAME);
	InitUiWidget(IDC_EDIT_NEW_NAME);
	InitUiWidget(IDC_STATIC_NM_MSG_PANEL);
	InitUiWidget(IDOK);
	InitUiWidget(IDCANCEL);

	SetDlgItemTextW(IDC_EDIT_OLD_NAME,m_wstrOldName.c_str());

	return TRUE;
}

BOOL CNameModifierDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CNameModifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNameModifierDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CNameModifierDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNameModifierDlg::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


HBRUSH CNameModifierDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_NM_MSG_PANEL)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		pDC->SetTextColor(RGB(250,50,50));
		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CNameModifierDlg::DisplayPromptMessage(CString sMsg)
{
	SetDlgItemTextW(IDC_STATIC_NM_MSG_PANEL, _T(""));
	CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_NM_MSG_PANEL);
	pPanel->ShowWindow(SW_SHOW);
	SetDlgItemTextW(IDC_STATIC_NM_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_NM_DLG, 8000, NULL);
}

void CNameModifierDlg::OnBnClickedOk()
{
	CString sMsg;
	CString sNewName;
	GetDlgItemTextW(IDC_EDIT_NEW_NAME,sNewName);
	wstring wstrNewName = sNewName.GetString();
	if(wstrNewName == L"")
	{
		return;
	}
	if(utils::CheckSpecialCharacter(wstrNewName))
	{
		sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
		DisplayPromptMessage(sMsg);
		return;
	}
	if(m_setNickname.find(wstrNewName) != m_setNickname.end())
	{
		sMsg.LoadString(IDS_STRING_WARNING_NICKNAME_EXISTED);
		DisplayPromptMessage(sMsg);
		return;
	}
	m_retNewName = wstrNewName;
    wostringstream oss;
    oss << L"Testing item " << m_wstrOldName << L" is renamed to " << m_retNewName << L".";
    gLogger.QueueUpLog(oss.str(),true);
	CDialogEx::OnOK();
}

void CNameModifierDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CNameModifierDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_NM_DLG)
	{
		SetDlgItemTextW(IDC_STATIC_NM_MSG_PANEL, _T(""));
		CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_NM_MSG_PANEL);
		pPanel->ShowWindow(SW_HIDE);
		KillTimer(TIMER_ID_NM_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}
