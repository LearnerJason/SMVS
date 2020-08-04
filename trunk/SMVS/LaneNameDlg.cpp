#include "stdafx.h"
#include "SMVS.h"
#include "LaneNameDlg.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CLaneNameDlg, CDialogEx)

CLaneNameDlg::CLaneNameDlg(CRect rcPanel,wstring& wstrRetName,CWnd* pParent /*=NULL*/)
	: m_rcPanel(rcPanel),
	  m_wstrNewName(wstrRetName),
	  CDialogEx(CLaneNameDlg::IDD, pParent)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CLaneNameDlg::~CLaneNameDlg()
{
}

BOOL CLaneNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rc;
	GetWindowRect(rc);
	int x = m_rcPanel.left + 220;
	int y = m_rcPanel.top + 20;
	::SetWindowPos(m_hWnd,HWND_TOP, x ,y, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	CWnd* pCtrl = (CWnd*)GetDlgItem(IDC_STATIC_LANE_NAME);
	pCtrl->SetFont(&m_textFont);
	pCtrl->ShowWindow(SW_HIDE);
	pCtrl->ShowWindow(SW_SHOW);

	pCtrl = (CWnd*)GetDlgItem(IDC_EDIT_LANE_NAME);
	pCtrl->SetFont(&m_textFont);

	pCtrl = (CWnd*)GetDlgItem(IDOK);
	pCtrl->SetFont(&m_textFont);

	pCtrl = (CWnd*)GetDlgItem(IDCANCEL);
	pCtrl->SetFont(&m_textFont);
	
	return TRUE;
}

void CLaneNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CLaneNameDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CLaneNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLaneNameDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLaneNameDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void CLaneNameDlg::OnBnClickedOk()
{
	CString sLaneName;
	GetDlgItemText(IDC_EDIT_LANE_NAME,sLaneName);
	m_wstrNewName = sLaneName.GetString();
	CDialogEx::OnOK();
}

void CLaneNameDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
