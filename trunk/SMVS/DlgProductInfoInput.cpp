// DlgProductInfoInput.cpp : 实现文件
//

#include "stdafx.h"
#include "SMVS.h"
#include "DlgProductInfoInput.h"
#include "afxdialogex.h"

//extern ConfigManager  gSystemMgr;
// CDlgProductInfoInput 对话框

const int NEW_SYS_DLG_CLOSE_BTN		= 90001;

IMPLEMENT_DYNAMIC(CDlgProductInfoInput, CDialogEx)

CDlgProductInfoInput::CDlgProductInfoInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProductInfoInput::IDD, pParent)
{
	int txtFontSz = 22;
	m_txtFont.CreateFont(txtFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CDlgProductInfoInput::~CDlgProductInfoInput()
{
	if(!m_vecCStatic.empty())
	{
		std::for_each(m_vecCStatic.begin(), m_vecCStatic.end(),[&](CWnd* pWnd)
		{
			delete pWnd;
			pWnd = nullptr;
		});
	}
	if(!m_vecCEdit.empty())
	{
		std::for_each(m_vecCEdit.begin(), m_vecCEdit.end(),[&](CWnd* pWnd)
		{
			delete pWnd;
			pWnd = nullptr;
		});
	}
}

void CDlgProductInfoInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, ID_UPDATE, m_btnCancle);
}


BEGIN_MESSAGE_MAP(CDlgProductInfoInput, CDialogEx)
	ON_STN_CLICKED(NEW_SYS_DLG_CLOSE_BTN,	&CDlgProductInfoInput::OnCloseScreen)
	ON_BN_CLICKED(IDOK, &CDlgProductInfoInput::OnBnClickedOk)
	ON_BN_CLICKED(ID_UPDATE, &CDlgProductInfoInput::OnBnClickedUpdate)
END_MESSAGE_MAP()


// CDlgProductInfoInput 消息处理程序


BOOL CDlgProductInfoInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int iProductInfoSize = m_vecProductInfo.size();
//	SetBackgroundImage(IDB_BITMAP_NEW_SYS_DLG_BG);
	std::vector<CString> m_vecCurSysForeignKeyInfo;
	bool bCurInfo = gSystemMgr.GetCurSysForeignKeyInfo(m_sysForeignKey,m_vecCurSysForeignKeyInfo);
	MoveWindow(0,0,430,250+50*(iProductInfoSize-2));
	CenterWindow();

//	m_font.CreateFont(22 ,0, 0,	0,FW_NORMAL,FALSE, FALSE,0,ANSI_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("Arial"));
	SetBackgroundColor(RGB(200,220,250));
	CRect Dcrt;
	GetClientRect(&Dcrt);
	CRect rc;
	rc.left = Dcrt.right - 5 - 32;
	rc.top = Dcrt.top + 5;
	rc.right = rc.left + 32;
	rc.bottom = rc.top + 32;
	m_btnClose.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, NEW_SYS_DLG_CLOSE_BTN);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();

	CEdit* pEdit      = nullptr;
	CStatic* pStatic  = nullptr;

	CRect rcStatic, rcEdit;

	rcStatic.left = Dcrt.left +30;
	rcStatic.right = rcStatic .left +120;
	rcStatic.top = Dcrt.top+30;
	rcStatic.bottom = rcStatic.top ;

	rcEdit.left   = rcStatic.right + 35;
	rcEdit.right = rcEdit.left +200;
	rcEdit.top = rcEdit.top;
	rcEdit.bottom = rcStatic.bottom;

	////////////////////////////////////////////////////////////////
// 	rcStatic.top = rcStatic.bottom + 20;
// 	rcStatic.bottom = rcStatic.top+30 ;
// 
// 	rcEdit.top = rcStatic.top;
// 	rcEdit.bottom = rcStatic.bottom;
// 
// 	pStatic = new CStatic;
// 	pStatic->Create(_T("批次号"),WS_CHILD|WS_VISIBLE, rcStatic, this, 90000);
// 	pStatic->SetFont(&m_txtFont);
// 
// 	m_vecCStatic.push_back(pStatic);
// 	//		m_vParamNameCtrl.push_back(pStatic);
// 
// 	pEdit   = new CEdit;
// 	pEdit->Create(WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcEdit, this, 90100);
// 	pEdit->SetFont(&m_txtFont);
// 	pEdit->SetWindowText(m_strbatchNumber);
// 
// 	m_vecCEdit.push_back(pEdit);

	for(int i = 0 ; i < m_vecProductInfo.size();i++)
	{
		rcStatic.top = rcStatic.bottom + 20;
		rcStatic.bottom = rcStatic.top+30 ;
	
		rcEdit.top = rcStatic.top;
		rcEdit.bottom = rcStatic.bottom;

		pStatic = new CStatic;
		pStatic->Create(_T(""),WS_CHILD|WS_VISIBLE, rcStatic, this, 90000+i+1);
		pStatic->SetFont(&m_txtFont);
		pStatic->SetWindowText(m_vecProductInfo[i]);
			m_vecCStatic.push_back(pStatic);
//		m_vParamNameCtrl.push_back(pStatic);

		pEdit   = new CEdit;
		pEdit->Create(WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcEdit, this, 90100+i+1);
		pEdit->SetFont(&m_txtFont);
		if (bCurInfo)
		{
				pEdit->SetWindowText(m_vecCurSysForeignKeyInfo[i+1]);
		}

		m_vecCEdit.push_back(pEdit);
	}

	CRect rectOK;
	rectOK.left = rcStatic.left +80;
	rectOK.right = rectOK.left + 80;
	rectOK.top = rcStatic.bottom +25;
	rectOK.bottom = rectOK.top +30;

	m_btnOk.SetFont(&m_txtFont);
	m_btnOk.MoveWindow(rectOK);

	CRect rectCancle;
	rectCancle.left = rectOK.right +50;
	rectCancle.right = rectCancle.left + 80;
	rectCancle.top = rectOK.top ;
	rectCancle.bottom = rectOK.bottom ;
	m_btnCancle.SetFont(&m_txtFont);
	m_btnCancle.MoveWindow(rectCancle);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgProductInfoInput::OnCloseScreen()
{
		CDialogEx::OnCancel();
}

BOOL CDlgProductInfoInput::SetProductInfoKey(std::vector<CString> m_vecKey)
{
	m_vecProductInfo =  m_vecKey;
	return true;
}
BOOL CDlgProductInfoInput::GetProductInfoValue(std::vector<CString> &m_vecValue)
{
	m_vecValue = m_vecProductInfoValue;
	return true;
}

BOOL CDlgProductInfoInput::SetSysForeignKey(CString SysForeignKey)
{
	m_sysForeignKey = SysForeignKey;
	return true;
}
CString CDlgProductInfoInput::GetSysForeignKey()
{
	return m_sysForeignKey;
}
//添加模具号关键字段

void CDlgProductInfoInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	m_sysForeignKey.Format(_T("%I64d"),utils::DateTime2Int64(dt));
	gSystemMgr.SetCurSysForeignKey(m_sysForeignKey);
	gConfigMgr.SetSysForeignKey(m_sysForeignKey);

	m_vecProductInfoValue.clear();
	for (auto it = m_vecCEdit.begin(); it != m_vecCEdit.end(); it++)
	{
		CEdit *p = (*it);
		CString str;
		p->GetWindowText(str);
		
		m_vecProductInfoValue.push_back(str);
	}

	gSystemMgr.ProductInfoToDataBase(m_vecProductInfoValue);
}


void CDlgProductInfoInput::OnBnClickedUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_vecProductInfoValue.clear();
	for (auto it = m_vecCEdit.begin(); it != m_vecCEdit.end(); it++)
	{
		CEdit *p = (*it);
		CString str;
		p->GetWindowText(str);

		m_vecProductInfoValue.push_back(str);
	}

	gSystemMgr.ProductInfoToDataBase(m_vecProductInfoValue);
}
