// DialogUpdateDatabase.cpp : 实现文件
//

#include "stdafx.h"
#include "SMVS.h"
#include "DialogUpdateDatabase.h"
#include "afxdialogex.h"



// CDialogUpdateDatabase 对话框

IMPLEMENT_DYNAMIC(CDialogUpdateDatabase, CDialogEx)

CDialogUpdateDatabase::CDialogUpdateDatabase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogUpdateDatabase::IDD, pParent)
	, m_bRecheck(false)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_textFont1.CreateFont(17 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

}

CDialogUpdateDatabase::~CDialogUpdateDatabase()
{
}

void CDialogUpdateDatabase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_UPDATE_RESERVED1, m_ditReserved1);
	DDX_Check(pDX, IDC_CHECK1, m_bRecheck);
}


BEGIN_MESSAGE_MAP(CDialogUpdateDatabase, CDialogEx)
	ON_STN_CLICKED(IDB_UPDATE_DATA_CLOSE,      &CDialogUpdateDatabase::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDialogUpdateDatabase::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogUpdateDatabase::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogUpdateDatabase 消息处理程序
CString CDialogUpdateDatabase::GetRecheck()
{
	if (m_bRecheck)
	{
		return _T("1");
	}
	else
	{
		return _T("0");
	}
}
void CDialogUpdateDatabase::InitUiWidget(int nCtrlID, int strID)
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

CString CDialogUpdateDatabase::GetReserved1()
{
	return m_Reserved1;
}

void CDialogUpdateDatabase::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_ditReserved1.GetWindowText(m_Reserved1); 
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDialogUpdateDatabase::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDialogUpdateDatabase::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CDialogUpdateDatabase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_RESERVED1||
		id == IDC_STATIC_RECHECK)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		pDC->SetTextColor(RGB(0,0,0));
		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
void CDialogUpdateDatabase:: SetStrInfo(CString str)
{
	m_strInfo = str;
}

BOOL CDialogUpdateDatabase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	InitUiWidget(IDOK);
	InitUiWidget(IDCANCEL);

//	InitUiWidget(IDC_STATIC_OLD_NAME,IDS_STRING_OLD_NAME);
	InitUiWidget(IDC_EDIT_UPDATE_RESERVED1);
	InitUiWidget(IDC_STATIC_UDDATE_TITLE);
	CRect RcClient;
	GetClientRect(RcClient);

	CRect rc;
	rc.left  = 0;
	rc.right = rc.left +RcClient.Width();
	rc.top = 0;
	rc.bottom = rc.top + 35;
	m_dlgTitle.Create(_T("数据库上传"),WS_CHILD|WS_VISIBLE|WS_EX_LEFT,rc,this, NULL);

	m_dlgTitle.SubclassDlgItem(IDC_STATIC_DLG_TITLE,this);
	m_dlgTitle.SetColor(RGB(10,10,115),RGB(171,178,185));
	
	double rate = 1.0f;
	int fontSz = (int)(26 * rate);
	m_dlgTitle.SetTxtFont(fontSz,FW_BOLD);

	rc.left = rc.right -35 ;
	m_btnClose.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc,this, IDB_UPDATE_DATA_CLOSE);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();

	rc.left = 160;
	rc.right =600;
	rc.top =70;
	rc.bottom =300;
	m_LabelCtrlInfo.Create(m_strInfo, WS_CHILD|WS_VISIBLE, rc,this, NULL);
	m_LabelCtrlInfo.SetFont(&m_textFont1);
	m_LabelCtrlInfo.Invalidate();

	 GetDlgItem(IDC_STATIC_RECHECK)->SetFont(&m_textFont);
	 GetDlgItem(IDC_STATIC_RESERVED1)->SetFont(&m_textFont);

//	InitUiWidget(IDC_STATIC_NEW_NAME,IDS_STRING_NEW_NAME);
//	InitUiWidget(IDC_EDIT_NEW_NAME);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
