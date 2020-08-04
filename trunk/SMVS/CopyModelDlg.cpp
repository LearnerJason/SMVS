#include "stdafx.h"
#include "SMVS.h"
#include "CopyModelDlg.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CCopyModelDlg, CDialogEx)

CCopyModelDlg::CCopyModelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCopyModelDlg::IDD, pParent),
	TIMER_ID_TASK_TEMPLATE_DLG(106667),
	m_bOpRslt(true)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CCopyModelDlg::~CCopyModelDlg()
{
}

void CCopyModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCopyModelDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CCopyModelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCopyModelDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


void CCopyModelDlg::InitUiWidget(int nCtrlID, int strID)
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

BOOL CCopyModelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	InitUiWidget(IDC_STATIC_MODEL_NAME,IDS_STRING_NEW_MODEL_NAME);
	InitUiWidget(IDC_EDIT_NEW_MODEL_NAME);
	InitUiWidget(IDC_STATIC_TT_MSG_PANEL);
	InitUiWidget(IDOK);
	InitUiWidget(IDCANCEL);
	return TRUE;
}

void CCopyModelDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_TT_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_TT_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_TASK_TEMPLATE_DLG, 8000, NULL);
}

void CCopyModelDlg::OnBnClickedOk()
{
	CString sName;
	CWnd* pWnd = GetDlgItem(IDC_EDIT_NEW_MODEL_NAME);
	pWnd->GetWindowText(sName);
	sName.Trim();
	CString sMsg;
	if(sName.GetLength() > 0)
	{
		wstring wstrModelName = CT2W(sName);
		if(utils::CheckSpecialCharacter(wstrModelName))
		{
			sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
			DisplayPromptMessage(sMsg, false);
			return;
		}
		if(gConfigMgr.IsTaskModelExisted(wstrModelName))
		{
			sMsg.LoadString(IDS_STRING_MSG_MODEL_EXISTED);
			sMsg.Replace(_T("#1"), wstrModelName.c_str());
			DisplayPromptMessage(sMsg, false);
			return;
		}
		gSystemMgr.SetNewModelName(sName);
	}
	else
	{
		sMsg.LoadString(IDS_STRING_MSG_MODEL_NAME_EMPTY);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	CDialogEx::OnOK();
}


void CCopyModelDlg::OnBnClickedCancel()
{
	gSystemMgr.SetNewModelName(_T(""));
	CDialogEx::OnCancel();
}

HBRUSH CCopyModelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_TT_MSG_PANEL)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		if(m_bOpRslt)
			pDC->SetTextColor(RGB(50,200,20));
		else
			pDC->SetTextColor(RGB(200,20,20));
		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}
