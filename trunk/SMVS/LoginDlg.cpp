#include "stdafx.h"
#include "SMVS.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

// Background image size
#define IMG_W 930
#define IMG_H 513
#define BTN_TOP 373
#define BTN_HEIGHT 30
#define BTN_WIDTH 140
#define OK_LEFT 472
#define CANCEL_LEFT 658 

#define ID_OK_LABEL     60001
#define ID_CANCEL_LABEL 60002

BOOL CLoginDlg::m_bIsSlacAdmin=FALSE;

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent),
	  TIMER_ID_LOGON_DLG(400800),
	  m_bOpRslt(true)
{
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	float rate = rect.Height() /1080.0f;
	m_titleFontSz = (int)(45 * rate);
	if(m_titleFontSz > 45)
	{
		m_titleFontSz = 45;
	}
	
	m_txtFontSz = (int)(30 * rate);
	if(m_txtFontSz > 30)
	{
		m_txtFontSz = 30;
	}
	m_titleFont.CreateFont(m_titleFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_txtFont.CreateFont(m_txtFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_smallFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
	m_titleBgColor = RGB(242,250,252);
	m_btnBgColor = RGB(29,106,222);

	m_iLoginFlag = 0;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_usernameEdit);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_passwordEdit);
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	switch (pMsg-> wParam)
	{
	case 'Q':
		if(::GetKeyState(VK_CONTROL) < 0)
		{	
			m_iLoginFlag = 1;
			return TRUE;
		}
		break;
	case 'W':
		if(::GetKeyState(VK_CONTROL) < 0)
		{	
			if (m_iLoginFlag==1)
			{
				m_iLoginFlag = 2;
				return TRUE;
			}
		}
		break;
	case 'T':
		if(::GetKeyState(VK_CONTROL) < 0)
		{	
			if (m_iLoginFlag==2)
			{
				CString sMsg;
				int nErrorCode = -1;
				if(!::gSystemMgr.ValidateAuthority(_T("SMVSADMIN"),_T("smvsadmin"), nErrorCode))
				{
					if(nErrorCode == 0) // Username is not existed
					{
						sMsg.LoadStringW(IDS_STRING_USERNAME_NOT_EXISTED);
					}
					if(nErrorCode == 1) // Password is not correct.
					{
						sMsg.LoadStringW(IDS_STRING_PASSWORD_NOT_CORRECT);
					}
					DisplayPromptMessage(sMsg, false);
					m_iLoginFlag = 0;
					return FALSE;
				}
				//m_bIsSlacAdmin=TRUE;
				CDialogEx::OnOK();
			}
		}
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_STN_CLICKED(ID_OK_LABEL, &CLoginDlg::OnStnClickedOk)
	ON_STN_DBLCLK(ID_OK_LABEL, &CLoginDlg::OnStnDblClickedOk)
	ON_STN_CLICKED(ID_CANCEL_LABEL, &CLoginDlg::OnStnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CLoginDlg::CreateLabels()
{
	


	CRect rcClient;
	GetClientRect(&rcClient);
	int left = (int)(rcClient.Width() / 3.0);
	int top = 30;
	int right = rcClient.right - 30;
	int bottom = 35;
	CRect rc(left,top,right,bottom);
	wstring strTitle = gConfigMgr.GetBasicSetting(L"LOGON_DLG_TITLE");
	CString sTitle = CW2T(strTitle.c_str());
	m_dlgTitle.Create(sTitle,WS_CHILD|WS_VISIBLE,rc,this, NULL);
	m_dlgTitle.SetTxtFont(m_titleFontSz, FW_BOLD);
	m_dlgTitle.SetColor(m_btnBgColor, m_titleBgColor);
	m_dlgTitle.Invalidate();

	float rateX = (float)rcClient.Width() / IMG_W;
	float rateY = (float)rcClient.Height() / IMG_H;
	left = int(OK_LEFT * rateX);
	top = int(BTN_TOP * rateY);
	right = left + int(BTN_WIDTH * rateX);
	bottom = top + int(BTN_HEIGHT * rateY) - 5;
	CRect rcOk(left,top,right,bottom);
	CString strLogin;
	strLogin.LoadString(IDS_STRING_LOGIN);
	m_okBtn.Create(strLogin,WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcOk, this, ID_OK_LABEL);

	int nLangID = ::gConfigMgr.GetLanguageID();
	if (nLangID ==LANG_RUS )
	{
		m_okBtn.SetTxtFont(20, FW_BOLD);
	}
	else
	{
		m_okBtn.SetTxtFont(m_txtFontSz, FW_BOLD);
	}
	m_okBtn.SetColor(RGB(255,255,255),m_btnBgColor);
	m_okBtn.Invalidate();

	left = int(CANCEL_LEFT * rateX);
	right = left + int(BTN_WIDTH * rateX);
	CRect rcCancel(left, top, right, bottom);
	CString strCancel;
	strCancel.LoadString(IDS_STRING_CANCEL);
	m_cancelBtn.Create(strCancel,WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcCancel, this, ID_CANCEL_LABEL);
	if (nLangID ==LANG_RUS )
	{
		m_cancelBtn.SetTxtFont(20, FW_BOLD);
	}
	else
	{
		m_cancelBtn.SetTxtFont(m_txtFontSz, FW_BOLD);
	}
	m_cancelBtn.SetColor(RGB(255,255,255),m_btnBgColor);
	m_cancelBtn.Invalidate();

}
// CLoginDlg 消息处理程序
BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
// 	int nLangID = ::gConfigMgr.GetLanguageID();
// 	if (nLangID ==LANG_RUS )
// 	{
// 		m_usernameEdit.SetFont(&m_smallFont);
// 		m_passwordEdit.SetFont(&m_smallFont);
// 	}

	m_usernameEdit.SetFont(&m_txtFont);
	m_passwordEdit.SetFont(&m_txtFont);

	CWnd* pWnd = GetDlgItem(IDC_STATIC_LOGON_DLG_MSG_PANEL);
	pWnd->SetFont(&m_txtFont);

	CreateLabels();

	UAC crtUser = gSystemMgr.GetCurrentUser();
	if(crtUser.m_nLevel == UAC_OPERATOR)
	{
		m_usernameEdit.SetWindowText(_T(""));
		m_passwordEdit.SetWindowText(_T(""));
	}
	UpdateData(FALSE);

	return TRUE;
}

void CLoginDlg::LoadBackgroundImage(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);

	CBitmap bakImg;
	if(bakImg.LoadBitmap(IDB_BMP_LOGIN_BG))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		CBitmap* pPngOld = dcMem.SelectObject(&bakImg);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	}
}

void CLoginDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_LOGON_DLG_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_LOGON_DLG_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_LOGON_DLG, 8000, NULL);
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this);
	LoadBackgroundImage(dc);
}

void CLoginDlg::OnStnClickedOk()
{
	UpdateData(TRUE);

	CString sUsername, sPassword, sMsg;
	m_usernameEdit.GetWindowText(sUsername);
	m_passwordEdit.GetWindowText(sPassword);

	if(sUsername.GetLength() <= 0 || sPassword.GetLength() <= 0)
	{
		sMsg.LoadString(IDS_STRING_USERNAME_OR_PASSWORD_IS_EMPTY);
		DisplayPromptMessage(sMsg,false);
		return;
	}
	int nErrorCode = -1;
	if(!::gSystemMgr.ValidateAuthority(sUsername,sPassword, nErrorCode))
	{
		if(nErrorCode == 0) // Username is not existed
		{
			sMsg.LoadString(IDS_STRING_USERNAME_NOT_EXISTED);
		}
		if(nErrorCode == 1) // Password is not correct.
		{
			sMsg.LoadString(IDS_STRING_PASSWORD_NOT_CORRECT);
		}
		DisplayPromptMessage(sMsg, false);
		return;
	}
	m_bIsSlacAdmin=FALSE;
	CDialogEx::OnOK();
}

void CLoginDlg::OnStnDblClickedOk()
{
	//Empty
}

void CLoginDlg::OnStnClickedCancel()
{
	CDialogEx::OnCancel();
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_LOGON_DLG_MSG_PANEL)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		if(id == IDC_STATIC_LOGON_DLG_MSG_PANEL)
		{
			if(m_bOpRslt)
				pDC->SetTextColor(RGB(20,200,100));
			else
				pDC->SetTextColor(RGB(200,20,20));
		}

		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(TIMER_ID_LOGON_DLG == nIDEvent)
	{
		SetDlgItemTextW(IDC_STATIC_LOGON_DLG_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_LOGON_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}
