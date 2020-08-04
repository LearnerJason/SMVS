#include "stdafx.h"
#include "SMVS.h"
#include "SysConfigDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CSysConfigDlg, CDialogEx)

CSysConfigDlg::CSysConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSysConfigDlg::IDD, pParent),
	  m_taskMgrDlg(gConfigMgr.m_vProductIDs),
	  m_modelMgrDlg(gConfigMgr.m_vProductIDs),
	  m_crtSettingType(T_INVALID),
	  m_pDictChConfig(nullptr)
{
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	float rate = rect.Height() /1080.0f;
	if(rect.Height() > 900)
		rate = 900 / 1080.0f;
	m_titleFontSz = (int)(45 * rate);
	m_titleFont.CreateFont(m_titleFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
	ANSI_CHARSET,             
	OUT_DEFAULT_PRECIS,        
	CLIP_DEFAULT_PRECIS,       
	DEFAULT_QUALITY,           
	DEFAULT_PITCH | FF_SWISS,  
	_T("Arial"));

	m_subTitleFontSz = (int)(30 * rate);
	m_subTitleFont.CreateFont(m_subTitleFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_txtFontSz = (int)(25 * rate);
	m_txtFont.CreateFont(m_txtFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_titleBgColor = RGB(230,243,246);
	m_btnBgColor = RGB(29,106,222);
}

CSysConfigDlg::~CSysConfigDlg()
{
}

void CSysConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSysConfigDlg, CDialogEx)
	ON_STN_CLICKED(NEW_SYS_DLG_CLOSE_BTN,	&CSysConfigDlg::OnCloseScreen)
	ON_STN_CLICKED(SYS_DLG_TASK_BTN,		&CSysConfigDlg::OnBtnClickTask)
	ON_STN_CLICKED(SYS_DLG_MODEL_BTN,		&CSysConfigDlg::OnBtnClickModel)
	ON_STN_CLICKED(SYS_DLG_CAMERA_BTN,		&CSysConfigDlg::OnBtnClickCamera)
	ON_STN_CLICKED(SYS_DLG_LIGHT_BTN,		&CSysConfigDlg::OnBtnClickLight)
	ON_STN_CLICKED(SYS_DLG_REJECTOR_BTN,	&CSysConfigDlg::OnBtnClickRejector)
	ON_STN_CLICKED(SYS_DLG_CHART_MGR_BTN,	&CSysConfigDlg::OnBtnClickChartMgr)
	ON_STN_CLICKED(SYS_DLG_CONFIG_FILE_BTN, &CSysConfigDlg::OnBtnClickConfigFileMgr)
	ON_STN_CLICKED(SYS_DLG_SHIFT_BTN,       &CSysConfigDlg::OnBtnClickShiftMgr)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_LIST, &CSysConfigDlg::OnCbnSelchangeComboProductList)
END_MESSAGE_MAP()

void CSysConfigDlg::LoadAllChannelConfig()
{
	m_pDictChConfig = gConfigMgr.GetChannelConfigPtr();
	if(m_pDictChConfig == nullptr)
		return;
}

BOOL CSysConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_NEW_SYS_DLG_BG);

	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
	int x_sz = rcWorkArea.Width();  
	int y_sz = rcWorkArea.Height();
	int x_bgn = 0;
	int y_bgn = 0;

	if(x_sz > 1000)
	{
		x_bgn = (int)((x_sz - 1000) / 2.0);
		x_sz = 1000;
	}
	if(y_sz > 700)
	{
		y_bgn = (int)((y_sz - 700) / 2.0) - 20;
		y_sz = 700;
	}
	::SetWindowPos(m_hWnd,HWND_TOP,x_bgn,y_bgn,x_sz, y_sz, SWP_SHOWWINDOW);

	CreateHeadCtrls(rcWorkArea);
	CreateCommonInfoCtrls(rcWorkArea);
	CreateNavigationButtons(rcWorkArea);

	LoadAllChannelConfig();

	m_taskMgrDlg.SetCrtProductIndex(0);
	m_modelMgrDlg.SetCrtProductIndex(0);

	if(gConfigMgr.m_vProductIDs[0] == CROWN_CAP)
		m_taskMgrDlg.Create(IDD_DIALOG_CROWN_CAP_TASK_MGR, this);
	else
		m_taskMgrDlg.Create(IDD_DIALOG_TASK_MGR, this);

	m_modelMgrDlg.Create(IDD_DIALOG_MODEL_MGR,this);
	m_modelMgrDlg.m_pTaskDlg = &m_taskMgrDlg;
	m_cameraMgrDlg.Create(IDD_DIALOG_CAMERA_MGR, this);
	m_lightMgrDlg.Create(IDD_DIALOG_LIGHT_MGR, this);
	m_rjMgrDlg.Create(IDD_DIALOG_REJECTOR_MGR, this);
	m_chartMgrDlg.Create(IDD_DIALOG_GRAPH_MGR,this);
	m_configFileMgrDlg.Create(IDD_DIALOG_CONFIG_FILE_MGR, this);
	m_shiftMgrDlg.Create(IDD_DIALOG_SHIFT_MGR, this);

	CRect rcClient, rc;
	GetClientRect(&rcClient);
	rc.left = rcClient.left + 79;
	rc.top = rcClient.top + 89;
	rc.right = rcClient.right - 15;
	rc.bottom = rcClient.bottom - 15;

	m_taskMgrDlg.MoveWindow(&rc);
	m_taskMgrDlg.ShowWindow(FALSE);

	m_modelMgrDlg.MoveWindow(&rc);
	m_modelMgrDlg.ShowWindow(FALSE);

	m_cameraMgrDlg.MoveWindow(&rc);
	m_cameraMgrDlg.ShowWindow(FALSE);

	m_lightMgrDlg.MoveWindow(&rc);
	m_lightMgrDlg.ShowWindow(FALSE);

	m_rjMgrDlg.MoveWindow(&rc);
	m_rjMgrDlg.ShowWindow(FALSE);

	m_chartMgrDlg.MoveWindow(&rc);
	m_chartMgrDlg.ShowWindow(FALSE);

	m_configFileMgrDlg.MoveWindow(&rc);
	m_configFileMgrDlg.ShowWindow(FALSE);

	m_shiftMgrDlg.MoveWindow(&rc);
	m_shiftMgrDlg.ShowWindow(FALSE);

	UpdateUiWidget(T_TASK);
	return TRUE;
}

void CSysConfigDlg::InitUiWidget(int nCtrlID, int strID)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	if(pWnd != nullptr)
	{
		pWnd->SetFont(&m_txtFont);
		if(strID != NULL)
		{
			CString str;
			str.LoadString(strID);
			pWnd->SetWindowText(str);
		}
		pWnd->Invalidate();
	}
}

void CSysConfigDlg::CreateHeadCtrls(CRect& rcWorkArea)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rc;

	rc.left = rcClient.left + 300;
	rc.top = rcClient.top + 10;
	rc.right = rc.left + 300;
	rc.bottom = rc.top + 50;

	CString strTitle;
	strTitle.LoadString(IDS_STRING_SYSTEM_CONFIG_DLG_TITLE);
	m_lblTitle.Create(strTitle, WS_CHILD|WS_VISIBLE, rc, this, NULL);
	m_lblTitle.SetTxtFont(m_titleFontSz, FW_BOLD);
	m_lblTitle.SetColor(m_btnBgColor, m_titleBgColor);
	m_lblTitle.Invalidate();

	rc.left = rcClient.right - 5 - 32;
	rc.top = rcClient.top + 5;
	rc.right = rc.left + 32;
	rc.bottom = rc.top + 32;
	m_btnClose.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, NEW_SYS_DLG_CLOSE_BTN);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();
}

void CSysConfigDlg::CreateCommonInfoCtrls(CRect& rcWorkArea)
{
	InitUiWidget(IDC_STATIC_PRODUCT, IDS_STRING_SYS_DLG_PRODUCT);
	InitUiWidget(IDC_COMBO_PRODUCT_LIST);
	InitUiWidget(IDC_STATIC_SYS_SETTING);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRODUCT_LIST);
	if(pCombo == nullptr)
	{
		return;
	}
	CString sProduct;
	for(auto i = 0; i < gConfigMgr.m_vProductIDs.size(); i++)
	{
		if(gConfigMgr.m_vProductIDs[i] == CROWN_CAP)
			sProduct.LoadString(IDS_STRING_CROWN_CAP);
		if(gConfigMgr.m_vProductIDs[i] == EOE) 
			sProduct.LoadString(IDS_STRING_EOE_PRODUCT);
		if(gConfigMgr.m_vProductIDs[i] == CAN || gConfigMgr.m_vProductIDs[i] == CAN_OUTSIDE)
			sProduct.LoadString(IDS_STRING_CAN_PRODUCT);
		if(gConfigMgr.m_vProductIDs[i] == PET)
			sProduct.LoadString(IDS_STRING_PET_PRODUCT);
		if(gConfigMgr.m_vProductIDs[i] == NAIL)
			sProduct=_T("nail");
		if(gConfigMgr.m_vProductIDs[i] == PCC)
			sProduct.LoadString(IDS_STRING_PCC_PRODUCT);
		if(gConfigMgr.m_vProductIDs[i] == CAP)
			sProduct.LoadString(IDS_STRING_CAP_PRODUCT);
		if(gConfigMgr.m_vProductIDs[i] == PLUG)
			sProduct.LoadString(IDS_STRING_PLUG_PRODUCT);
		if(gConfigMgr.m_vProductIDs[i] == BOTTLE)
			sProduct.LoadString(IDS_STRING_BOTTLE_PRODUCT);
		if(gConfigMgr.m_vProductIDs[i] == AL_PLASTIC)
			sProduct.LoadString(IDS_STRING_AL_PLASTIC_PRODUCT);
		if(gConfigMgr.m_vProductIDs[i] == HDPE)
			sProduct=_T("hdpe");
		if(gConfigMgr.m_vProductIDs[i] == NEEDLE)
			sProduct=_T("needle");
		if(gConfigMgr.m_vProductIDs[i] == MAT)
			sProduct=_T("MAT");

		pCombo->AddString(sProduct);
	}
	pCombo->SetCurSel(0);

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SYS_SETTING);
	if(pWnd == nullptr)
		return;
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rc;
	rc.left = rcClient.left + 68;
	rc.top = rcClient.top + 70;
	rc.right = rcClient.right - 10;
	rc.bottom = rcClient.bottom - 10;

	pWnd->MoveWindow(rc);
}

void CSysConfigDlg::CreateNavigationButtons(CRect& rcWorkArea)
{
	int btnSz = 48;
	int gapV = 5;
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rc;
	rc.left = rcClient.left + 10;
	rc.top = rcClient.top + 70;
	rc.right = rc.left + btnSz;
	rc.bottom = rc.top + btnSz;
	m_btnTaskMgr.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, SYS_DLG_TASK_BTN);
	m_btnTaskMgr.SetLabelImage(IMG_SYS_TASK);
	m_btnTaskMgr.Invalidate();

	rc.top = rc.bottom + 5;
	rc.bottom = rc.top + 48;
	m_btnModelMgr.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, SYS_DLG_MODEL_BTN);
	m_btnModelMgr.SetLabelImage(IMG_SYS_MODEL);
	m_btnModelMgr.Invalidate();

	rc.top = rc.bottom + 5;
	rc.bottom = rc.top + 48;
	m_btnChartMgr.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, SYS_DLG_CHART_MGR_BTN);
	m_btnChartMgr.SetLabelImage(IMG_SYS_CHART_MGR);
	m_btnChartMgr.Invalidate();

	rc.top = rc.bottom + 5;
	rc.bottom = rc.top + 48;
	m_btnConfigFileMgr.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, SYS_DLG_CONFIG_FILE_BTN);
	m_btnConfigFileMgr.SetLabelImage(IMG_SYS_CONFIG_FILE);
	m_btnConfigFileMgr.Invalidate();

	rc.top = rc.bottom + 5;
	rc.bottom = rc.top + 48;
	m_btnShiftMgr.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, SYS_DLG_SHIFT_BTN);
	m_btnShiftMgr.SetLabelImage(IMG_SYS_SHIFT);
	m_btnShiftMgr.Invalidate();


	rc.top = rc.bottom + 5;
	rc.bottom = rc.top + 48;
	m_btnCameraConfig.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, SYS_DLG_CAMERA_BTN);
	m_btnCameraConfig.SetLabelImage(IMG_CH_CAMERA);
	m_btnCameraConfig.Invalidate();

	rc.top = rc.bottom + 5;
	rc.bottom = rc.top + 48;
	m_btnLightConfig.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, SYS_DLG_LIGHT_BTN);
	m_btnLightConfig.SetLabelImage(IMG_CH_LIGHT);
	m_btnLightConfig.Invalidate();

	rc.top = rc.bottom + 5;
	rc.bottom = rc.top + 48;
	m_btnRejectorConfig.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, SYS_DLG_REJECTOR_BTN);
	m_btnRejectorConfig.SetLabelImage(IMG_CH_REJECTOR);
	m_btnRejectorConfig.Invalidate();

	UAC crtUac = gSystemMgr.GetCurrentUser();
	if(crtUac.m_strUsername.compare(L"SMVSADMIN") != 0)
	{
		m_btnCameraConfig.ShowWindow(FALSE);
		m_btnLightConfig.ShowWindow(FALSE);
		m_btnRejectorConfig.ShowWindow(FALSE);
	}


}

void CSysConfigDlg::ResetUiWidget(SETTING_TYPE oldType, SETTING_TYPE newType)
{
	if(oldType == T_INVALID)
		return;
	if(oldType == newType)
		return;
	CLabelCtrl* pCtrl = nullptr;
	if(oldType == T_TASK)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_TASK_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_TASK);
		}
		m_taskMgrDlg.ShowWindow(FALSE);
	}
	if(oldType == T_MODEL)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_MODEL_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_MODEL);
		}
		m_modelMgrDlg.ShowWindow(FALSE);
	}
	if(oldType == T_CAMERA)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_CAMERA_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_CH_CAMERA);
		}
		m_cameraMgrDlg.ShowWindow(FALSE);
	}
	if(oldType == T_LIGHT)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_LIGHT_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_CH_LIGHT);
		}
		m_lightMgrDlg.ShowWindow(FALSE);
	}
	if(oldType == T_REJECTOR)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_REJECTOR_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_CH_REJECTOR);
		}
		m_rjMgrDlg.ShowWindow(FALSE);
	}
	if(oldType == T_CHART_MGR)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_CHART_MGR_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_CHART_MGR);
		}
		m_chartMgrDlg.ShowWindow(FALSE);
	}
	//
	if(oldType == T_CONFIG_FILE_MGR)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_CONFIG_FILE_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_CONFIG_FILE);
		}
		m_configFileMgrDlg.ShowWindow(FALSE);
	}

	if(oldType == T_SYS_SHIFT_MGR)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_SHIFT_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_SHIFT);
		}
		m_shiftMgrDlg.ShowWindow(FALSE);
	}

	if(pCtrl != nullptr)
	{
		CRect rc;
		pCtrl->GetWindowRect(rc);
		InvalidateRect(rc);	
	}
}

void CSysConfigDlg::UpdateUiWidget(SETTING_TYPE newType)
{
	CLabelCtrl* pCtrl = nullptr;
	CString str;
	if(newType == T_TASK)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_TASK_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_TASK_SELECTED);
			str.LoadString(IDS_STRING_TASK_MANAGEMENT);
		}
		m_taskMgrDlg.ShowWindow(TRUE);
	}
	if(newType == T_MODEL)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_MODEL_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_MODEL_SELECTED);
			str.LoadString(IDS_STRING_MODEL_MANAGEMENT);
		}
		m_modelMgrDlg.ShowWindow(TRUE);
	}
	if(newType == T_CAMERA)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_CAMERA_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_CAMERA_SELECTED);
			str.LoadString(IDS_STRING_CAMERA_MANAGEMENT);
		}
		m_cameraMgrDlg.ShowWindow(TRUE);
	}
	if(newType == T_LIGHT)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_LIGHT_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_LIGHT_SELECTED);
			str.LoadString(IDS_STRING_LIGHT_MANAGEMENT);
		}
		m_lightMgrDlg.ShowWindow(TRUE);
	}
	if(newType == T_REJECTOR)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_REJECTOR_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_REJECTOR_SELECTED);
			str.LoadString(IDS_STRING_REJECTION_MANAGEMENT);
		}
		m_rjMgrDlg.ShowWindow(TRUE);
	}
	if(newType == T_CHART_MGR)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_CHART_MGR_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_CHART_MGR_SELECTED);
			str.LoadString(IDS_STRING_CHART_MGR);
		}
		m_chartMgrDlg.ShowWindow(TRUE);
	}
	if(newType == T_CONFIG_FILE_MGR)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_CONFIG_FILE_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_CONFIG_FILE_SELECTED);
			str.LoadString(IDS_STRING_SYS_CONFIG_FILE_MGR);
		}
		m_configFileMgrDlg.ShowWindow(TRUE);
	}

	if(newType == T_SYS_SHIFT_MGR)
	{
		pCtrl = (CLabelCtrl*)GetDlgItem(SYS_DLG_SHIFT_BTN);
		if(pCtrl != nullptr)
		{
			pCtrl->SetLabelImage(IMG_SYS_SHIFT_SELECTED);
			str.LoadString(IDS_STRING_SHIFT_MGR);
		}
		m_shiftMgrDlg.ShowWindow(TRUE);
	}
	if(pCtrl != nullptr)
	{
		CRect rc;
		pCtrl->GetWindowRect(rc);
		InvalidateRect(rc);	
	}
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SYS_SETTING);
	if(pWnd != nullptr)
	{
		pWnd->SetWindowText(str);
	}
	m_crtSettingType = newType;
	RedrawWindow();
	UpdateData(FALSE);
}

BOOL CSysConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CSysConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CSysConfigDlg::OnCloseScreen()
{
	gConfigMgr.SaveResetShiftStatsFlag();
	CDialogEx::OnOK();
}

void CSysConfigDlg::OnBtnClickTask()
{
	ResetUiWidget(m_crtSettingType, T_TASK);
	UpdateUiWidget(T_TASK);
}

void CSysConfigDlg::OnBtnClickModel()
{
	ResetUiWidget(m_crtSettingType, T_MODEL);
	UpdateUiWidget(T_MODEL);
}

void CSysConfigDlg::OnBtnClickCamera()
{
	ResetUiWidget(m_crtSettingType, T_CAMERA);
	UpdateUiWidget(T_CAMERA);
}

void CSysConfigDlg::OnBtnClickLight()
{
	ResetUiWidget(m_crtSettingType, T_LIGHT);
	UpdateUiWidget(T_LIGHT);
}

void CSysConfigDlg::OnBtnClickRejector()
{
	ResetUiWidget(m_crtSettingType, T_REJECTOR);
	UpdateUiWidget(T_REJECTOR);
}

void CSysConfigDlg::OnBtnClickChartMgr()
{
	ResetUiWidget(m_crtSettingType, T_CHART_MGR);
	UpdateUiWidget(T_CHART_MGR);
}

void CSysConfigDlg::OnBtnClickConfigFileMgr()
{
	ResetUiWidget(m_crtSettingType, T_CONFIG_FILE_MGR);
	UpdateUiWidget(T_CONFIG_FILE_MGR);
}

void CSysConfigDlg::OnBtnClickShiftMgr()
{
	ResetUiWidget(m_crtSettingType, T_SYS_SHIFT_MGR);
	UpdateUiWidget(T_SYS_SHIFT_MGR);
}

void CSysConfigDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
}

void CSysConfigDlg::OnCbnSelchangeComboProductList()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PRODUCT_LIST);
	int nProductIndex = pCombo->GetCurSel();
	if(nProductIndex != -1)
	{
		m_taskMgrDlg.SwitchToProduct(nProductIndex);
		m_modelMgrDlg.SwitchToProduct(nProductIndex);
	}
}
