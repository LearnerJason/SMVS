#include "stdafx.h"
#include "SMVS.h"
#include "ShiftMgrDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CShiftMgrDlg, CDialogEx)

CShiftMgrDlg::CShiftMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShiftMgrDlg::IDD, pParent),
	  m_bOpRslt(true),
	  SHIFT_DLG_TIMER_ID(10017),
	  m_bAutoReset(FALSE)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CShiftMgrDlg::~CShiftMgrDlg()
{
}

void CShiftMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1_BGN_TIME, m_bgnTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END_TIME, m_endTime);
	DDX_Control(pDX, IDC_LIST_SHIFT, m_shiftListCtrl);
	DDX_Control(pDX, IDC_BUTTON_CREATE_SHIFT, m_btnCreateShift);
	DDX_Control(pDX, IDC_BUTTON_UPDATE_SHIFT, m_btnUpdateShift);
	DDX_Control(pDX, IDC_BUTTON_DELETE_SHIFT, m_btnDeleteShift);
	DDX_Control(pDX, IDC_EDIT_SHIFT_NAME, m_editShiftName);
	DDX_Check(pDX, IDC_CHECK_RESET_COUNT, m_bAutoReset);
}

BEGIN_MESSAGE_MAP(CShiftMgrDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CREATE_SHIFT, &CShiftMgrDlg::OnBnClickedButtonCreateShift)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_SHIFT, &CShiftMgrDlg::OnBnClickedButtonUpdateShift)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_SHIFT, &CShiftMgrDlg::OnBnClickedButtonDeleteShift)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SHIFT, &CShiftMgrDlg::OnNMClickListShift)
	ON_BN_CLICKED(IDC_CHECK_RESET_COUNT, &CShiftMgrDlg::OnBnClickedCheckResetCount)
END_MESSAGE_MAP()

void CShiftMgrDlg::InitUiWidget(int nCtrlID, int strID)
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

void CShiftMgrDlg::EnumerateAllShift()
{
	m_shiftListCtrl.DeleteAllItems();

	auto pShiftArray = gConfigMgr.GetShiftArrayPtr();
	m_vShift.clear();
	m_vShift = *pShiftArray;

	int index = 0;
	CString strIndex;
	for(auto it = m_vShift.begin(); it != m_vShift.end(); it++)
	{
		strIndex.Format(_T("%d"), index + 1);
		m_shiftListCtrl.InsertItem(index, strIndex);
		long nBgnTime = it->m_bgnTime;
		long nEndTime = it->m_endTime;

		m_shiftListCtrl.SetItemText(index, 1, it->m_wstrShiftName.c_str());

		CString sBgnTime = utils::GetTimeMinuteString(nBgnTime);
		m_shiftListCtrl.SetItemText(index, 2, sBgnTime);
		
		CString sEndTime = utils::GetTimeMinuteString(nEndTime);
		m_shiftListCtrl.SetItemText(index, 3, sEndTime);
		index++;
	}
}

bool CShiftMgrDlg::IsShiftExist(const wstring&  wstrShiftName)
{
	if(m_vShift.empty())
		return false;

	for(auto it = m_vShift.begin(); it != m_vShift.end(); it++)
	{
		if(wstrShiftName == it->m_wstrShiftName)
		{
			return true;
		}
	}
	return false;
}

BOOL CShiftMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);
	// Messge Panel
	InitUiWidget(IDC_STATIC_SHIFT_DLG_MSG_PANEL);

	InitUiWidget(IDC_STATIC_SHIFT_NAME, IDS_STRING_SHIFT_NAME);
	InitUiWidget(IDC_EDIT_SHIFT_NAME);
	InitUiWidget(IDC_STATIC_SHIFT_BGN_TIME, IDS_STRING_BEGIN_DATETIME);
	InitUiWidget(IDC_DATETIMEPICKER_BGN_TIME);
	InitUiWidget(IDC_STATIC_SHIFT_END_TIME, IDS_STRING_END_DATETIME);
	InitUiWidget(IDC_DATETIMEPICKER_END_TIME);

	m_bgnTime.SetFormat(_T("HH:mm"));
	m_endTime.SetFormat(_T("HH:mm"));
	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	dt.wMinute = 0;
	dt.wSecond = 0;
	m_bgnTime.SetTime(&dt);
	m_endTime.SetTime(&dt);

	InitUiWidget(IDC_BUTTON_CREATE_SHIFT, IDS_STRING_CREATE);
	InitUiWidget(IDC_BUTTON_UPDATE_SHIFT, IDS_STRING_UPDATE);
	InitUiWidget(IDC_BUTTON_DELETE_SHIFT, IDS_STRING_DELETE);
	InitUiWidget(IDC_CHECK_RESET_COUNT,IDS_STRING_RESET_COUNT);

	InitUiWidget(IDC_LIST_SHIFT);
	DWORD dwStyleEx = m_shiftListCtrl.GetExtendedStyle();
	dwStyleEx |= LVS_EX_FULLROWSELECT; 
	dwStyleEx |= LVS_EX_GRIDLINES;     
	dwStyleEx |= LVS_EX_CHECKBOXES;   
	m_shiftListCtrl.SetExtendedStyle(dwStyleEx);

	m_shiftListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strIndex(MAKEINTRESOURCE(IDS_STRING_INDEX));
	CString strShiftName(MAKEINTRESOURCE(IDS_STRING_SHIFT_NAME));
	CString strBgnTime(MAKEINTRESOURCE(IDS_STRING_BEGIN_DATETIME));
	CString strEndTime(MAKEINTRESOURCE(IDS_STRING_END_DATETIME));
	m_shiftListCtrl.InsertColumn(0, strIndex, LVCFMT_CENTER, 65);
	m_shiftListCtrl.InsertColumn(1, strShiftName, LVCFMT_LEFT, 200);
	m_shiftListCtrl.InsertColumn(2, strBgnTime, LVCFMT_LEFT, 148);
	m_shiftListCtrl.InsertColumn(3, strEndTime, LVCFMT_LEFT, 148);

	EnumerateAllShift();
	m_bAutoReset = gConfigMgr.m_bAutoResetShiftStats;
	UpdateData(FALSE);
	return TRUE;
}

BOOL CShiftMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CShiftMgrDlg::ResetUiWidget()
{
	EnumerateAllShift();
	m_editShiftName.SetWindowText(_T(""));

	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	dt.wMinute = 0;
	dt.wSecond = 0;
	m_bgnTime.SetTime(&dt);
	m_endTime.SetTime(&dt);
	m_bgnTime.Invalidate();
	m_endTime.Invalidate();

	m_btnCreateShift.EnableWindow(TRUE);
	m_btnUpdateShift.EnableWindow(FALSE);
	m_btnDeleteShift.EnableWindow(FALSE);
	UpdateData(FALSE);
}

bool CShiftMgrDlg::GetNewShift(SHIFT& shift)
{
	CString sShiftName;
	m_editShiftName.GetWindowText(sShiftName);

	CString sMsg;
	if(sShiftName.GetLength() == 0)
	{
		sMsg.LoadString(IDS_STRING_ERROR_SHIFT_NAME_EMPTY);
		DisplayPromptMessage(sMsg,false);
		return false;
	}
	wstring wstrShiftName = CT2W(sShiftName);
	shift.m_wstrShiftName = wstrShiftName;
	SYSTEMTIME tmpDT;
	m_bgnTime.GetTime(&tmpDT);
	shift.m_bgnTime = tmpDT.wHour * 100 + tmpDT.wMinute;

	m_endTime.GetTime(&tmpDT);
	shift.m_endTime = tmpDT.wHour * 100 + tmpDT.wMinute;

	return true;
}

void CShiftMgrDlg::OnBnClickedButtonCreateShift()
{
	UpdateData(TRUE);

	SHIFT shift;
	if(!GetNewShift(shift))
	{
		return;
	}
	CString sMsg;
	if(IsShiftExist(shift.m_wstrShiftName))
	{
		sMsg.LoadString(IDS_STRING_ERROR_SHIFT_IS_EXISTED);
		DisplayPromptMessage(sMsg,false);
		return;
	}
	sMsg.LoadString(IDS_STRING_OPERATION_OK);
	m_bOpRslt = true;
	if(!gConfigMgr.AddNewShift(shift))
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		m_bOpRslt = false;
	}
	ResetUiWidget();
	DisplayPromptMessage(sMsg,m_bOpRslt);
}

void CShiftMgrDlg::OnBnClickedButtonUpdateShift()
{
	UpdateData(TRUE);

	SHIFT shift;
	if(!GetNewShift(shift))
	{
		return;
	}
	if(shift.IsEqual(m_crtShift))
	{
		return;
	}
	CString sMsg;
	sMsg.LoadString(IDS_STRING_OPERATION_OK);
	m_bOpRslt = true;
	if(!gConfigMgr.UpdateShift(m_crtShift, shift))
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		m_bOpRslt = false;
	}
	ResetUiWidget();
	DisplayPromptMessage(sMsg,m_bOpRslt);
}

void CShiftMgrDlg::OnBnClickedButtonDeleteShift()
{
	UpdateData(TRUE);

	CString sShiftName;
	m_editShiftName.GetWindowText(sShiftName);

	CString sMsg;
	if(sShiftName.GetLength() == 0)
	{
		sMsg.LoadString(IDS_STRING_ERROR_SHIFT_NAME_EMPTY);
		DisplayPromptMessage(sMsg,false);
		return;
	}
	wstring wstrShiftName = CT2W(sShiftName);
	sMsg.LoadString(IDS_STRING_OPERATION_OK);
	m_bOpRslt = true;
	if(!gConfigMgr.DeleteShift(wstrShiftName))
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		m_bOpRslt = false;
	}
	ResetUiWidget();
	DisplayPromptMessage(sMsg,m_bOpRslt);
}

void CShiftMgrDlg::OnNMClickListShift(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int index = pNMItemActivate->iItem;
	if(index == -1)
	{
		m_editShiftName.SetWindowText(_T(""));

		SYSTEMTIME dt;
		::GetLocalTime(&dt);
		dt.wMinute = 0;
		dt.wSecond = 0;
		m_bgnTime.SetTime(&dt);
		m_endTime.SetTime(&dt);

		m_btnCreateShift.EnableWindow(TRUE);
		m_btnUpdateShift.EnableWindow(FALSE);
		m_btnDeleteShift.EnableWindow(FALSE);
	}
	else
	{
		if(index < (int)m_vShift.size())
		{
			m_crtShift = m_vShift[index];
			m_editShiftName.SetWindowText(m_crtShift.m_wstrShiftName.c_str());

			SYSTEMTIME dt;
			::GetLocalTime(&dt);
			dt.wHour   = (WORD)m_crtShift.m_bgnTime / 100;
			dt.wMinute = (WORD)m_crtShift.m_bgnTime  % 100;
			dt.wSecond = 0;
			m_bgnTime.SetTime(&dt);

			dt.wHour   = (WORD)m_crtShift.m_endTime / 100;
			dt.wMinute = (WORD)m_crtShift.m_endTime % 100;
			m_endTime.SetTime(&dt);

			m_btnCreateShift.EnableWindow(FALSE);
			m_btnUpdateShift.EnableWindow(TRUE);
			m_btnDeleteShift.EnableWindow(TRUE);
		}
	}
	UpdateData(false);
	*pResult = 0;
}

HBRUSH CShiftMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_SHIFT_DLG_MSG_PANEL ||
	   id == IDC_CHECK_RESET_COUNT)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();

		if(m_bOpRslt)
			pDC->SetTextColor(RGB(56,94,15));
		else
			pDC->SetTextColor(RGB(200,20,20));

		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CShiftMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == SHIFT_DLG_TIMER_ID)
	{
		SetDlgItemTextW(IDC_STATIC_SHIFT_DLG_MSG_PANEL, _T(""));
		CWnd* pWnd = GetDlgItem(IDC_STATIC_SHIFT_DLG_MSG_PANEL);
		pWnd->ShowWindow(SW_HIDE);
		KillTimer(SHIFT_DLG_TIMER_ID);
	}
	UpdateData(FALSE);
}

void CShiftMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_SHIFT_DLG_MSG_PANEL, _T(""));
	m_bOpRslt = bOpRslt;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SHIFT_DLG_MSG_PANEL);
	pWnd->ShowWindow(SW_HIDE);
	SetDlgItemTextW(IDC_STATIC_SHIFT_DLG_MSG_PANEL, sMsg);
	pWnd->ShowWindow(SW_SHOW);
	SetTimer(SHIFT_DLG_TIMER_ID,5000, NULL);
	UpdateData(FALSE);
}

void CShiftMgrDlg::OnBnClickedCheckResetCount()
{
	UpdateData(TRUE);
	//gConfigMgr.m_bAutoResetShiftStats = (bool)m_bAutoReset;
}
