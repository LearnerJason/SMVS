#include "stdafx.h"
#include "SMVS.h"
#include "ChartMgrDlg.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CChartMgrDlg, CDialogEx)

CChartMgrDlg::CChartMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChartMgrDlg::IDD, pParent)
	, m_bOpRslt(true)
	, m_pDictChConfig(nullptr)
	, TIMER_ID_CHART_MGR_DLG(100003)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CChartMgrDlg::~CChartMgrDlg()
{
}

void CChartMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChartMgrDlg, CDialogEx)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_APPLY_CHART_PARAM, &CChartMgrDlg::OnStnClickedStaticApplyChartParam)
	ON_STN_CLICKED(IDC_STATIC_APPLY_SPC_RULE, &CChartMgrDlg::OnStnClickedStaticApplySpcRule)
	ON_CBN_SELCHANGE(IDC_COMBO_CRT_CHANNEL_ID, &CChartMgrDlg::OnCbnSelchangeComboCrtChannelId)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL CChartMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	InitUiWidget(IDC_STATIC_SYS_DLG_CRT_CH, IDS_STRING_SYS_DLG_CRT_CH);
	InitUiWidget(IDC_COMBO_CRT_CHANNEL_ID);
	
	InitUiWidget(IDC_STATIC_X_AXIS_UNIT, IDS_STRING_X_AXIS_UNIT);
	InitUiWidget(IDC_COMBO_X_AXIS_UNIT);

	InitUiWidget(IDC_STATIC_X_AXIS_LEN, IDS_STRING_X_AXIS_LEN);
	InitUiWidget(IDC_EDIT_X_AXIS_LEN);

	InitUiWidget(IDC_STATIC_Y_AXIS_UNIT, IDS_STRING_Y_AXIS_UNIT);
	InitUiWidget(IDC_COMBO_Y_AXIS_UNIT);

	InitUiWidget(IDC_STATIC_SPC_RULE, IDS_STRING_SPC_RULE);
	InitUiWidget(IDC_STATIC_CHART_MGR_DLG_MSG_PANEL);

	m_pDictChConfig = gConfigMgr.GetChannelConfigPtr(); 
	if(m_pDictChConfig == nullptr)
		return FALSE;

	FillUpUiWidget();

	return TRUE;
}

BOOL CChartMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CChartMgrDlg::InitUiWidget(int nCtrlID, int strID)
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

void CChartMgrDlg::FillUpUiWidget()
{
	size_t sz = m_pDictChConfig->size();
	if(sz == 0) return;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	if(pCombo == nullptr)
		return;

	CString strCh;
	for(int t = 1; t <= sz; t++)
	{
		strCh.Format(_T("CH%d"), t);
		pCombo->AddString(strCh);
	}
	pCombo->SetCurSel(0);
	int chIndex = 1;
	DisplayChartSettings(chIndex);
}

void CChartMgrDlg::DisplayChartSettings(int chIndex)
{
	if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
	{
		CHANNEL_CONFIG chConfig  = (*m_pDictChConfig)[chIndex];
		int index = -1;
		if(chConfig.m_spcSetting.m_strUnitX == L"X1000")
			index = 0;
		if(chConfig.m_spcSetting.m_strUnitX == L"X2000")
			index = 1;
		if(chConfig.m_spcSetting.m_strUnitX == L"X5000")
			index = 2;
		if(chConfig.m_spcSetting.m_strUnitX == L"X10000")
			index = 3;
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_X_AXIS_UNIT);
		pCombo->SetCurSel(index);
		pCombo->EnableWindow(FALSE);

		SetDlgItemInt(IDC_EDIT_X_AXIS_LEN,chConfig.m_spcSetting.m_nXLen);

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_Y_AXIS_UNIT);
		pCombo->SetCurSel(0);
		pCombo->EnableWindow(FALSE);

		UpdateData(FALSE);
	}
}

void CChartMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_CHART_MGR_DLG_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_CHART_MGR_DLG_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_CHART_MGR_DLG,5000, NULL);
}

void CChartMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_CHART_MGR_DLG)
	{
		SetDlgItemTextW(IDC_STATIC_CHART_MGR_DLG_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_CHART_MGR_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CChartMgrDlg::OnCbnSelchangeComboCrtChannelId()
{
	CComboBox* pCombo = ((CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID));
	if(pCombo != nullptr)
	{
		int index = pCombo->GetCurSel();
		if(index == -1)
			return;
		DisplayChartSettings(index + 1);
	}
}

void CChartMgrDlg::OnStnClickedStaticApplyChartParam()
{
	UpdateData(TRUE);
	int chIndex = -1;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	bool bOpRslt = true;
	CString sMsg;
	if(pCombo != nullptr)
	{
		int index = pCombo->GetCurSel();
		if(index != -1)
			chIndex = index + 1;
		if(chIndex == -1)
			return;
		SPC_SETTING spcSetting;
		CString strUnitX;
		GetDlgItemTextW(IDC_COMBO_X_AXIS_UNIT, strUnitX);
		spcSetting.m_strUnitX = CT2W(strUnitX);

		CString strXLen;
		GetDlgItemTextW(IDC_EDIT_X_AXIS_LEN, strXLen);

		if(!utils::IsNumericStringEx(strXLen,utils::Type_Positive_integer))
		{
		    sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_POSITIVE_INTEGER);
			DisplayPromptMessage(sMsg, false);
			return;
		}
		spcSetting.m_nXLen = (int)utils::to_float(strXLen);

		CString strUnitY;
		GetDlgItemTextW(IDC_COMBO_Y_AXIS_UNIT, strUnitY);
		spcSetting.m_strUnitY = CT2W(strUnitY);

		if(gConfigMgr.UpdateSpcSettingSetting(chIndex, &spcSetting))
		{
			sMsg.LoadString(IDS_STRING_OPERATION_OK);
		}
		else
		{
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
			bOpRslt = false;
		}
		DisplayPromptMessage(sMsg, bOpRslt);
	}
}

void CChartMgrDlg::OnStnClickedStaticApplySpcRule()
{
	// TODO:
}

HBRUSH CChartMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_CHART_MGR_DLG_MSG_PANEL)
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
