#include "stdafx.h"
#include "SMVS.h"
#include "HelpDlg.h"
#include "afxdialogex.h"
#include "SystemManager.h"
#include "../SVN_VERSION/version.h"

#define IMG_W 930
#define IMG_H 513
#define HELP_DLG_CLOSE_BTN  60044


extern CSystemManager gSystemMgr;

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHelpDlg::IDD, pParent),
	  HELP_DLG_TIMER_ID(100026)
{
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	float rate = rect.Height() /1080.0f;

	m_titleFontSz = (int)(40 * rate);
	m_titleFont.CreateFont(m_titleFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_txtFontSz = (int)(24 * rate);
	m_txtFont.CreateFont(m_txtFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_titleBgColor = RGB(242,250,252);
	m_btnBgColor = RGB(29,106,222);
}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::CreateLabels()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int left = (int)(rcClient.Width() / 4.0);
	int top = 20;
	int right = rcClient.right - 30;
	int bottom = 35;
	CRect rc(left,top,right,bottom);
	CString strUacTitle;
	strUacTitle.LoadString(IDS_STRING_SYSTEM_HELP);
	m_dlgTitle.Create(strUacTitle,WS_CHILD|WS_VISIBLE,rc,this, NULL);
	m_dlgTitle.SetTxtFont(m_titleFontSz, FW_BOLD);
	m_dlgTitle.SetColor(m_btnBgColor, m_titleBgColor);
	m_dlgTitle.Invalidate();

	float rateX = (float)rcClient.Width() / IMG_W;
	float rateY = (float)rcClient.Height() / IMG_H;

	CRect rcCloseBtn;
	rcCloseBtn.left = rcClient.right - 5 - 32;
	rcCloseBtn.top = rcClient.top + 5;
	rcCloseBtn.right = rcCloseBtn.left + 32;
	rcCloseBtn.bottom = rcCloseBtn.top + 32;

	m_btnClose.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcCloseBtn, this, HELP_DLG_CLOSE_BTN);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();
}


BOOL CHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CreateLabels();
	m_StrBlowVersion.SetFont(&m_txtFont);
	CString strVersion = gSystemMgr.GetAllSecretaryDeviceVersion();
	m_StrBlowVersion.SetWindowText(strVersion);
	m_StrBlowVersion.ShowWindow(SW_SHOW);

	m_StrSmvsVersion.SetFont(&m_txtFont);
	m_StrSmvsVersion.SetWindowText(_T(SMVS_VERSION));
	return TRUE;
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SYSTEM_INFO, m_StrBlowVersion);
	DDX_Control(pDX, IDC_STATIC_SMVS_VERSION, m_StrSmvsVersion);
}

BOOL CHelpDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
	ON_STN_CLICKED(HELP_DLG_CLOSE_BTN, &CHelpDlg::OnStnClickedClose)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CHelpDlg::LoadBackgroundImage(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);

	CBitmap bakImg;
	if(bakImg.LoadBitmap(IDB_BITMAP_HELP_DLG_BG))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		CBitmap* pPngOld = dcMem.SelectObject(&bakImg);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	}
}

void CHelpDlg::OnPaint()
{
	CPaintDC dc(this);
	LoadBackgroundImage(dc);
}

void CHelpDlg::OnStnClickedClose()
{
	// TODO:
	CDialogEx::OnCancel();
}


HBRUSH CHelpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_SYSTEM_INFO||IDC_STATIC_SMVS_VERSION)

	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();

		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
return hbr;;
}
