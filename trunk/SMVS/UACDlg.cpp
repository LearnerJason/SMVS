#include "stdafx.h"
#include "SMVS.h"
#include "UACDlg.h"
#include "afxdialogex.h"

// Background image size
#define IMG_W 930
#define IMG_H 513
#define BTN_TOP 460
#define BTN_HEIGHT 28
#define BTN_WIDTH 150
#define OK_LEFT 330
#define CANCEL_LEFT 520
 

// Create,Edit and Delete buttons
#define OP_BTN_LEFT       710
#define OP_BTN_H          20
#define OP_BTN_W          116
#define OP_BTN_CREATE_TOP 297
#define OP_BTN_EDIT_TOP   356
#define OP_BTN_DEL_TOP    410

#define UAC_DLG_CLOSE_BTN  60004

IMPLEMENT_DYNAMIC(CUACDlg, CDialogEx)

CUACDlg::CUACDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUACDlg::IDD, pParent),
	  UAC_DLG_TIMER_ID(100016),
	  m_bOpRslt(true)
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

	int nListFontSz = (int)(24 * rate);
	m_listFont.CreateFont(nListFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_titleBgColor = RGB(242,250,252);
	m_btnBgColor = RGB(29,106,222);
	
	int nLangId = ::gConfigMgr.GetLanguageID();
	BTN_ID_CREATE         = (nLangId != LANG_CHS) ? IDB_BITMAP_UAC_DLG_CREATE_BTN_E : IDB_BITMAP_UAC_DLG_CREATE_BTN;
	BTN_ID_CREATE_DISABLE = (nLangId != LANG_CHS) ? IDB_BITMAP_UAC_DLG_CREATE_BTN_E_GRAY : IDB_BITMAP_UAC_DLG_CREATE_BTN_GRAY;
	BTN_ID_UPDATE         = (nLangId != LANG_CHS) ? IDB_BITMAP_UAC_DLG_UPDATE_BTN_E : IDB_BITMAP_UAC_DLG_UPDATE_BTN;
	BTN_ID_UPDATE_DISABLE = (nLangId != LANG_CHS) ? IDB_BITMAP_UAC_DLG_UPDATE_BTN_E_GRAY : IDB_BITMAP_UAC_DLG_UPDATE_BTN_GRAY;
	BTN_ID_DELETE         = (nLangId != LANG_CHS) ? IDB_BITMAP_UAC_DLG_DELETE_BTN_E : IDB_BITMAP_UAC_DLG_DELETE_BTN;
	BTN_ID_DELETE_DISABLE = (nLangId != LANG_CHS) ? IDB_BITMAP_UAC_DLG_DELETE_BTN_E_GRAY : IDB_BITMAP_UAC_DLG_DELETE_BTN_GRAY;

	ADMIN_STR.LoadString(IDS_STRING_ADMINISTRATOR);
	ENGNIEER_STR.LoadString(IDS_STRING_ENGINEER);
}

CUACDlg::~CUACDlg()
{
}

void CUACDlg::InitUserList()
{
	m_listUsers.SetFont(&m_listFont);

	DWORD dwStyleEx = m_listUsers.GetExtendedStyle();
	dwStyleEx |= LVS_EX_FULLROWSELECT; 
	dwStyleEx |= LVS_EX_GRIDLINES;     
	dwStyleEx |= LVS_EX_CHECKBOXES;   
	m_listUsers.SetExtendedStyle(dwStyleEx);

	m_listUsers.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strIndex(MAKEINTRESOURCE(IDS_STRING_INDEX));
	CString strUsername(MAKEINTRESOURCE(IDS_STRING_USERNAME));
	CString strPassword(MAKEINTRESOURCE(IDS_STRING_PASSWORD));
	CString strLevel(MAKEINTRESOURCE(IDS_STRING_USER_LEVEL));

	m_listUsers.InsertColumn(0, strIndex, LVCFMT_CENTER, 75);
	m_listUsers.InsertColumn(1, strUsername, LVCFMT_LEFT, 180);
	m_listUsers.InsertColumn(2, strPassword, LVCFMT_LEFT, 150);
	m_listUsers.InsertColumn(3, strLevel, LVCFMT_LEFT, 160);

	m_dictUac.clear();
	gSystemMgr.GetAllUacInfo(m_dictUac);
}

void CUACDlg::FillUserList()
{
	m_listUsers.DeleteAllItems();

	int index = 0;
	CString sLevel;
	for(auto it = m_dictUac.begin(); it != m_dictUac.end(); it++)
	{
		UAC uac = it->second;
		//if(uac.m_nLevel == UAC_ADMIN)
		{
			CString strIndex;
			strIndex.Format(_T("%d"), index + 1);
			m_listUsers.InsertItem(index, strIndex);
			m_listUsers.SetItemText(index, 1, uac.m_strUsername.c_str());
			m_listUsers.SetItemText(index, 2, _T("******"));
			if(uac.m_nLevel == UAC_ADMIN)
				sLevel = ADMIN_STR;
			else
				sLevel = ENGNIEER_STR;
			m_listUsers.SetItemText(index, 3, sLevel);
			index++;
		}
	}
}

void CUACDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_UAC_DLG_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_UAC_DLG_MSG_PANEL, sMsg);
	SetTimer(UAC_DLG_TIMER_ID, 8000, NULL);
}

BOOL CUACDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_editUsername.Clear();
	m_editPassword.Clear();
	m_comboGroup.Clear();
	InitUserList();
	FillUserList();
	m_editUsername.SetFont(&m_txtFont);
	m_editPassword.SetFont(&m_txtFont);
	m_comboGroup.SetFont(&m_txtFont);
	CString strGroup;
	strGroup.LoadString(IDS_STRING_ADMINISTRATOR);
	m_comboGroup.AddString(strGroup);
	m_comboGroup.AddString(ENGNIEER_STR);
	m_comboGroup.SetCurSel(-1);

	// Messge Panel
	CWnd* pWnd = GetDlgItem(IDC_STATIC_UAC_DLG_MSG_PANEL);
	pWnd->SetFont(&m_txtFont);

	CreateLabels();
	return TRUE;
}

void CUACDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_LIST_USER, m_listUsers);
	DDX_Control(pDX, IDC_BUTTON1, m_btnCreate);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_btnUpdate);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
}

BOOL CUACDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CUACDlg::LoadBackgroundImage(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);

	CBitmap bakImg;
	if(bakImg.LoadBitmap(IDB_BITMAP_UAC_BG))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		CBitmap* pPngOld = dcMem.SelectObject(&bakImg);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	}
}

void CUACDlg::CreateLabels()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int left = (int)(rcClient.Width() / 4.0);
	int top = 20;
	int right = rcClient.right - 30;
	int bottom = 35;
	CRect rc(left,top,right,bottom);
	CString strUacTitle;
	strUacTitle.LoadString(IDS_STRING_UAC_TITLE);
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

	m_btnClose.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcCloseBtn, this, UAC_DLG_CLOSE_BTN);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();

	int nBtnWidth  = 128;
	int nBtnHeight = 37;
	CRect rcUsername;
	m_editUsername.GetWindowRect(rcUsername);
	left   = rcUsername.right + 90;
	right  = left + nBtnWidth;
	top    = rcUsername.top;
	bottom = top + nBtnHeight;
	CRect rcCreate(left,top,right,bottom);
	m_btnCreate.MoveWindow(rcCreate);
	CBitmap bmp;
	bmp.LoadBitmapW(BTN_ID_CREATE);
	m_btnCreate.SetBitmap(bmp);

	CRect rcPassword;
	m_editPassword.GetWindowRect(rcPassword);
	left   = rcPassword.right + 90;
	top    = rcPassword.top;
	right  = left + nBtnWidth;
	bottom = top + nBtnHeight;
	CRect rcUpdate(left, top, right, bottom);
	m_btnUpdate.MoveWindow(rcUpdate);
	CBitmap bmpUpdate;
	bmpUpdate.LoadBitmapW(BTN_ID_UPDATE_DISABLE);
	m_btnUpdate.SetBitmap(bmpUpdate);
	m_btnUpdate.EnableWindow(FALSE);

	CRect rcLevel;
	m_comboGroup.GetWindowRect(rcLevel);
	left   = rcLevel.right + 90;
	top    = rcLevel.top;
	right  = left + nBtnWidth;
	bottom = top + nBtnHeight;
	CRect rcDelete(left,top, right, bottom);
	m_btnDelete.MoveWindow(rcDelete);
	CBitmap bmpDel;
	bmpDel.LoadBitmapW(BTN_ID_DELETE_DISABLE);
	m_btnDelete.SetBitmap(bmpDel);
	m_btnDelete.EnableWindow(FALSE);
}

BEGIN_MESSAGE_MAP(CUACDlg, CDialogEx)
	ON_STN_CLICKED(UAC_DLG_CLOSE_BTN, &CUACDlg::OnStnClickedClose)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_LIST_USER, &CUACDlg::OnNMClickListUser)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CUACDlg::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CUACDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CUACDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()



void CUACDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    LoadBackgroundImage(dc);
}

void CUACDlg::OnStnClickedClose()
{
	// TODO:
	CDialogEx::OnCancel();
}

void CUACDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(UAC_DLG_TIMER_ID == nIDEvent)
	{
		SetDlgItemTextW(IDC_STATIC_UAC_DLG_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(UAC_DLG_TIMER_ID);
	}
	CDialogEx::OnTimer(nIDEvent);
}

HBRUSH CUACDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_UAC_DLG_MSG_PANEL)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();

		if(m_bOpRslt)
			pDC->SetTextColor(RGB(20,100,50));
		else
			pDC->SetTextColor(RGB(200,20,20));

		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CUACDlg::OnNMClickListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int index = pNMItemActivate->iItem;
	ResetUiWidget();
	if(index != -1 && index < (int)m_dictUac.size())
	{
		CBitmap bmpCreate;
		bmpCreate.LoadBitmapW(BTN_ID_CREATE_DISABLE);
		m_btnCreate.SetBitmap(bmpCreate);
		m_btnCreate.EnableWindow(FALSE);

		CString sUsername = m_listUsers.GetItemText(index, 1);
		m_crtSelectedUac.m_strUsername = CT2W(sUsername);
		UAC uac = m_dictUac[m_crtSelectedUac.m_strUsername];
		m_crtSelectedUac.m_strPassword = uac.m_strPassword;
		CString sLevel = m_listUsers.GetItemText(index, 3);
		if(sLevel == ADMIN_STR)
			m_crtSelectedUac.m_nLevel = 0;
		else
			m_crtSelectedUac.m_nLevel = 1;

		DisplayUac(m_crtSelectedUac);
		UAC crtUser = gSystemMgr.GetCurrentUser();

		CBitmap bmpUpdate, bmpDelete;
		bmpUpdate.LoadBitmapW(BTN_ID_UPDATE);
		m_btnUpdate.SetBitmap(bmpUpdate);
		m_btnUpdate.EnableWindow(TRUE);
		bmpDelete.LoadBitmapW(BTN_ID_DELETE);
		m_btnDelete.SetBitmap(bmpDelete);
		m_btnDelete.EnableWindow(TRUE);

		bool bSysAdmin = (m_crtSelectedUac.m_strUsername == L"SMVSADMIN");
		m_editUsername.EnableWindow(!bSysAdmin);
		m_comboGroup.EnableWindow(!bSysAdmin);
		CBitmap tmpDelBmp;
		if(bSysAdmin)
		{
			tmpDelBmp.LoadBitmapW(BTN_ID_DELETE_DISABLE);
		}
		else
		{
			tmpDelBmp.LoadBitmapW(BTN_ID_DELETE);
		}
		m_btnDelete.SetBitmap(tmpDelBmp);
		m_btnDelete.EnableWindow(!bSysAdmin);
	}
	if(index == -1)
	{
		CBitmap bmpCreate;
		bmpCreate.LoadBitmapW(BTN_ID_CREATE);
		m_btnCreate.SetBitmap(bmpCreate);
		m_btnCreate.EnableWindow(TRUE);

		CBitmap bmpUpdate;
		bmpUpdate.LoadBitmapW(BTN_ID_UPDATE_DISABLE);
		m_btnUpdate.SetBitmap(bmpUpdate);
		m_btnUpdate.EnableWindow(FALSE);
		CBitmap bmpDelete;
		bmpDelete.LoadBitmapW(BTN_ID_DELETE_DISABLE);
		m_btnDelete.SetBitmap(bmpDelete);
		m_btnDelete.EnableWindow(FALSE);
	}
	*pResult = 0;

	UpdateData(FALSE);
}

void CUACDlg::AddNewUser2Table(CString sUsername, CString sPassword, int nLevel)
{
	int index = (int)m_dictUac.size();
	CString strIndex;
	strIndex.Format(_T("%d"), index);
	m_listUsers.InsertItem(index - 1, strIndex);
	m_listUsers.SetItemText(index - 1, 1, sUsername);
	m_listUsers.SetItemText(index - 1, 2, _T("******"));

	CString sLevel;
	if(nLevel == UAC_ADMIN)
		sLevel.LoadString(IDS_STRING_ADMINISTRATOR);
	else
		sLevel.LoadString(IDS_STRING_ENGINEER);
	m_listUsers.SetItemText(index - 1, 3,sLevel);

	UAC uac;
	uac.m_strUsername = CT2W(sUsername);
	uac.m_strPassword = CT2W(sPassword);
	uac.m_nLevel      = nLevel;
	m_dictUac[uac.m_strUsername] = uac;

	m_listUsers.Invalidate();
}

void CUACDlg::OnBnClickedButtonCreate()
{
	UpdateData(TRUE);

	CString sUsername, sPassword;
	m_editUsername.GetWindowText(sUsername);
	m_editPassword.GetWindowText(sPassword);
	int nLevel = m_comboGroup.GetCurSel();

	CString sMsg;
	if(sUsername.GetLength() <= 0 || sPassword.GetLength() <= 0)
	{
		sMsg.LoadString(IDS_STRING_USERNAME_OR_PASSWORD_IS_EMPTY);
		DisplayPromptMessage(sMsg,false);
		return;
	}
	wstring strUsername = CT2W(sUsername);
	if(m_dictUac.find(strUsername) != m_dictUac.end())
	{
		sMsg.LoadString(IDS_STRING_MSG_USERNAME_EXISTED);
		sMsg.Replace(_T("#1"), sUsername);
		sMsg.Replace(_T("#1"), sUsername);
		DisplayPromptMessage(sMsg,false);
		return;
	}
	if(nLevel == -1)
	{
		sMsg.LoadString(IDS_STRING_PLEASE_ASSIGN_USER_LEVEL);
		DisplayPromptMessage(sMsg,false);
		return;
	}
	CString sError;
	UAC uac;
	uac.m_strUsername = CT2W(sUsername);
	uac.m_strPassword = CT2W(sPassword);
	uac.m_nLevel      = nLevel;
	bool bRet = gSystemMgr.AddNewUser(uac,sError);
	if(bRet)
	{
		AddNewUser2Table(sUsername, sPassword, nLevel);
		m_dictUac.clear();
		gSystemMgr.GetAllUacInfo(m_dictUac);
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		m_editUsername.SetWindowText(_T(""));
		m_editPassword.SetWindowText(_T(""));
		m_comboGroup.SetCurSel(-1);
	}
	else
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		sMsg.Append(sError);
	}
	DisplayPromptMessage(sMsg, bRet);
	UpdateData(FALSE);
}

void CUACDlg::OnBnClickedButtonUpdate()
{
	UpdateData(TRUE);

	CString sUsername, sPassword;
	m_editUsername.GetWindowText(sUsername);
	m_editPassword.GetWindowText(sPassword);
	int nLevel = m_comboGroup.GetCurSel();

	UAC newUac;
	newUac.m_strUsername = CT2W(sUsername);
	newUac.m_strPassword = CT2W(sPassword);
	newUac.m_nLevel      = nLevel;

	CString sMsg;
	if(sUsername.GetLength() <= 0 || sPassword.GetLength() <= 0)
	{
		sMsg.LoadString(IDS_STRING_USERNAME_OR_PASSWORD_IS_EMPTY);
		DisplayPromptMessage(sMsg,false);
		return;
	}

	if(nLevel == -1)
	{
		sMsg.LoadString(IDS_STRING_PLEASE_ASSIGN_USER_LEVEL);
		DisplayPromptMessage(sMsg,false);
		return;
	}
	CString sError;
	bool bRet = gSystemMgr.UpdateUser(m_crtSelectedUac,newUac, sError);
	if(bRet)
	{
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		DisplayPromptMessage(sMsg, true);
		m_dictUac.clear();
		gSystemMgr.GetAllUacInfo(m_dictUac);
		FillUserList();
		ResetUiWidget();
	}
}

void CUACDlg::OnBnClickedButtonDelete()
{
	UpdateData(TRUE);
	CString sMsg;
	if(m_crtSelectedUac.m_strUsername == L"SMVSADMIN")
	{
		sMsg.LoadString(IDS_STRING_CANNOT_DELETE_DEFAULT_ADMIN);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	bool bRet = gSystemMgr.DeleteUser(m_crtSelectedUac.m_strUsername);
	if(bRet)
	{
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		DisplayPromptMessage(sMsg, true);
		m_dictUac.clear();
		gSystemMgr.GetAllUacInfo(m_dictUac);
		FillUserList();
		ResetUiWidget();
	}
}

void CUACDlg::ResetUiWidget()
{
	m_editUsername.SetWindowText(_T(""));
	m_editUsername.EnableWindow(TRUE);
	m_editPassword.SetWindowText(_T(""));
	m_comboGroup.SetCurSel(-1);
	m_comboGroup.EnableWindow(TRUE);

	CBitmap bmpCreate;
	bmpCreate.LoadBitmapW(BTN_ID_CREATE);
	m_btnCreate.SetBitmap(bmpCreate);
	m_btnCreate.EnableWindow(TRUE);

	CBitmap bmpUpdate;
	bmpUpdate.LoadBitmapW(BTN_ID_UPDATE_DISABLE);
	m_btnUpdate.SetBitmap(bmpUpdate);
	m_btnUpdate.EnableWindow(FALSE);
	CBitmap bmpDelete;
	bmpDelete.LoadBitmapW(BTN_ID_DELETE_DISABLE);
	m_btnDelete.SetBitmap(bmpDelete);
	m_btnDelete.EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CUACDlg::DisplayUac(const UAC& uac)
{
	m_editUsername.SetWindowText(uac.m_strUsername.c_str());
	m_editPassword.SetWindowText(uac.m_strPassword.c_str());
	m_comboGroup.SetCurSel(uac.m_nLevel);
	UpdateData(FALSE);
}
