#include "stdafx.h"
#include "SMVS.h"
#include "LanguageDlg.h"
#include "afxdialogex.h"

// Background image size
#define IMG_W 930
#define IMG_H 513
#define BTN_TOP 338
#define BTN_HEIGHT 30
#define BTN_WIDTH 140
#define OK_LEFT 400
#define CANCEL_LEFT 618 

#define ID_OK_LABEL     60008
#define ID_CANCEL_LABEL 60009

IMPLEMENT_DYNAMIC(CLanguageDlg, CDialogEx)

CLanguageDlg::CLanguageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLanguageDlg::IDD, pParent)
	, TIMER_ID_LANG_DLG(100111)
	, m_radioCHS(0)
	, m_radioCHT(0)
	, m_radioENU(0)
	, m_nLangID(0)
{
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	float rate = rect.Height() /1080.0f;
	m_titleFontSz = (int)(40 * rate);
	m_txtFontSz = (int)(24 * rate);
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
	m_titleBgColor = RGB(242,250,252);
	m_btnBgColor = RGB(29,106,222);
}

CLanguageDlg::~CLanguageDlg()
{
}

void CLanguageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CHS, m_nLangID);
}

BOOL CLanguageDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CLanguageDlg::LoadBackgroundImage(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);

	CBitmap bakImg;
	if(bakImg.LoadBitmap(IDB_BMP_LANG_BG))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		CBitmap* pPngOld = dcMem.SelectObject(&bakImg);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	}
}

void CLanguageDlg::CreateLabels()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int left = (int)(rcClient.Width() / 3.0);
	int top = 30;
	int right = rcClient.right - 30;
	int bottom = 35;
	CRect rc(left,top,right,bottom);
	CString strTitle;
	strTitle.LoadString(IDS_STRING_LANG_TITLE);
	m_dlgTitle.Create(strTitle,WS_CHILD|WS_VISIBLE,rc,this, NULL);
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
	CString strOk;
	strOk.LoadString(IDS_STRING_OK);
	m_okBtn.Create(strOk,WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcOk, this, ID_OK_LABEL);
	m_okBtn.SetTxtFont(m_txtFontSz, FW_BOLD);
	m_okBtn.SetColor(RGB(255,255,255),m_btnBgColor);
	m_okBtn.Invalidate();

	left = int(CANCEL_LEFT * rateX);
	right = left + int(BTN_WIDTH * rateX);
	CRect rcCancel(left, top, right, bottom);
	CString strCancel;
	strCancel.LoadString(IDS_STRING_CANCEL);
	m_cancelBtn.Create(strCancel,WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcCancel, this, ID_CANCEL_LABEL);
	m_cancelBtn.SetTxtFont(m_txtFontSz, FW_BOLD);
	m_cancelBtn.SetColor(RGB(255,255,255),m_btnBgColor);
	m_cancelBtn.Invalidate();

}

BOOL CLanguageDlg::OnInitDialog()
{
	int nLangID = ::gConfigMgr.GetLanguageID();
	m_nLangID = nLangID - 1;

	CreateLabels();
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_CHS);
	pWnd->SetFont(&m_txtFont);
	
	pWnd = (CWnd*)GetDlgItem(IDC_STATIC_ENU);
	pWnd->SetFont(&m_txtFont);

	pWnd = (CWnd*)GetDlgItem(IDC_STATIC_LNAG_DLG_MSG);
	pWnd->SetFont(&m_txtFont);


	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CLanguageDlg, CDialogEx)
	ON_STN_CLICKED(ID_OK_LABEL, &CLanguageDlg::OnStnClickedOk)
	ON_STN_CLICKED(ID_CANCEL_LABEL, &CLanguageDlg::OnStnClickedCancel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_CHS, &CLanguageDlg::OnBnClickedRadioChs)
	ON_BN_CLICKED(IDC_RADIO_ENU, &CLanguageDlg::OnBnClickedRadioEnu)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CLanguageDlg::OnPaint()
{
	CPaintDC dc(this);
	LoadBackgroundImage(dc);
}

void CLanguageDlg::OnStnClickedOk()
{
	int nOldId = ::gConfigMgr.GetLanguageID();
	if(m_nLangID != nOldId - 1)
	{
		gConfigMgr.SetLanguageID(m_nLangID + 1);
	}
	CDialogEx::OnOK();
}

void CLanguageDlg::OnStnClickedCancel()
{
	CDialogEx::OnCancel();
}

HBRUSH CLanguageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_CHS
		|| id == IDC_STATIC_ENU
		|| id == IDC_STATIC_LNAG_DLG_MSG)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(id == IDC_STATIC_LNAG_DLG_MSG)
		{
			pDC->SetTextColor(RGB(200,20,20));
		}
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	return hbr;
}

void CLanguageDlg::SwitchLanguage()
{
	UpdateData(TRUE);
	int nOldId = ::gConfigMgr.GetLanguageID();
	if(m_nLangID != nOldId - 1)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_MSG_CHANGE_LANGUAGE);
		DisplayPromptMessage(sMsg);
	}
}

void CLanguageDlg::OnBnClickedRadioChs()
{
	SwitchLanguage();
}

void CLanguageDlg::OnBnClickedRadioCht()
{
	SwitchLanguage();
}

void CLanguageDlg::OnBnClickedRadioEnu()
{
	SwitchLanguage();
}

void CLanguageDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_LNAG_DLG_MSG, _T(""));
	RedrawWindow();
	SetDlgItemTextW(IDC_STATIC_LNAG_DLG_MSG, sMsg);
	SetTimer(TIMER_ID_LANG_DLG, 8000, NULL);
}

void CLanguageDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(TIMER_ID_LANG_DLG == nIDEvent)
	{
		SetDlgItemTextW(IDC_STATIC_LNAG_DLG_MSG, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_LANG_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}
