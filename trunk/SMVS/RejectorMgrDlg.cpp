#include "stdafx.h"
#include "SMVS.h"
#include "RejectorMgrDlg.h"
#include "afxdialogex.h"
#include "ChannelControlPanel.h"
#include "SystemManager.h"

extern CSystemManager gSystemMgr;
IMPLEMENT_DYNAMIC(CRejectorMgrDlg, CDialogEx)

CRejectorMgrDlg::CRejectorMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRejectorMgrDlg::IDD, pParent)
	, m_nCrtChIndex(-1)
	, m_pDictChConfig(nullptr)
	, TIMER_ID_RJ_DLG(100001)
	, m_bOpRslt(true)
	, m_rjMode(0)
	, m_bEnableTest(FALSE)
	, m_nTestMode(0)
	, m_nActionType(0)
	, m_bReReject(false)
	, m_bReRejectFunc(false)
    , m_bEnableLastNAlarm(FALSE)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_smallFont.CreateFont(17 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CRejectorMgrDlg::~CRejectorMgrDlg()
{
}

void CRejectorMgrDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_ENABLE_TEST, m_bEnableTest);
    DDX_Radio(pDX, IDC_RADIO_REJECT_ALL, m_nTestMode);
 //   DDX_Radio(pDX, IDC_RADIO_RM_TIME, m_rjMode);
    DDX_Radio(pDX, IDC_RADIO_ACTION_PER_ONE, m_nActionType);
    DDX_Check(pDX, IDC_CHECK_LAST_N_ENABLE, m_bEnableLastNAlarm);
}

void CRejectorMgrDlg::InitUiWidget(int nCtrlID, int strID)
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

void CRejectorMgrDlg::DisplayRejectorSettings(int chIndex)
{
	if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
	{
		CHANNEL_CONFIG chConfig  = (*m_pDictChConfig)[chIndex];

		REJECTOR_SETTING& rs = chConfig.m_rejectorSetting;
		m_bReReject  = rs.m_bReReject;
		m_bReRejectFunc = rs.m_bReRejectFunc;

		if(rs.m_wstrDeviceIp != L"")
		{
			SetDlgItemTextW(IDC_EDIT_DEVICE_IP,rs.m_wstrDeviceIp.c_str());
		}
		else
		{
			wstring wstrIp = L"129.101.1.";
			wstring wstrMask = boost::lexical_cast<wstring>(159 + chIndex);
			wstrIp.append(wstrMask);
			SetDlgItemTextW(IDC_EDIT_DEVICE_IP,wstrIp.c_str());
		}
		CString strValue;
		CWnd* pWnd = GetDlgItem(IDC_EDIT_GRABTIME);
		if(pWnd != nullptr)
		{
			strValue.Format(_T("%d"), rs.m_CameraGrabTime);
			pWnd->SetWindowText(strValue);
		}

		pWnd = GetDlgItem(IDC_EDIT_TRIGGER_INGNOR_TIME);
		if(pWnd != nullptr)
		{
			strValue.Format(_T("%d"), rs.m_TriggerIngoreTime);
			pWnd->SetWindowText(strValue);
		}

		pWnd = GetDlgItem(IDC_EDIT_SYS_DLG_CFRAME);
		if(pWnd != nullptr)
		{
			strValue.Format(_T("%d"), rs.m_SignalIngoreTime);
			pWnd->SetWindowText(strValue);
		}

		pWnd = GetDlgItem(IDC_EDIT_SYS_DLG_CDOWN);
		if(pWnd != nullptr)
		{
			strValue.Format(_T("%d"), rs.m_nContinueRejectAlarm);
			pWnd->SetWindowText(strValue);
		}

// 		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_REJECTOR_PORT);
// 		if(pCombo != nullptr)
// 		{
// 			pCombo->SetCurSel(rs.m_nRejectPort);
// 		}
// 		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_DROPPED_IMAGE_PORT);
// 		if(pCombo != nullptr)
// 		{
// 			pCombo->SetCurSel(rs.m_nDropImgPort);
// 		}
		m_rjMode = (chConfig.m_rejectorSetting.m_mode == DELAY_TIME)? 0 : 1;

		pWnd = GetDlgItem(IDC_EDIT_SYS_DLG_DELAY);
		if(pWnd != nullptr)
		{
			int nDelay = (m_rjMode == 0) ? int(rs.m_delay / 1.0f) : (rs.m_delay);
			//int nDelay = (m_rjMode == 0) ? int(rs.m_delay) : (rs.m_delay); //add by hzh
			strValue.Format(_T("%d"), nDelay);
			pWnd->SetWindowText(strValue);
		}

		pWnd = GetDlgItem(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH);
		if(pWnd != nullptr)
		{
			strValue.Format(_T("%d"), rs.m_signalWidth);
			pWnd->SetWindowText(strValue);
		}
        //Last N
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_N_OPTION);
        pCombo->SetCurSel(rs.m_nLastN/5000 - 1);
        strValue.Format(_T("%.2f"),rs.m_fLastNRatio);
        SetDlgItemTextW(IDC_EDIT_LAST_N_DEFECTIVE_RATIO,strValue);
        m_bEnableLastNAlarm = rs.m_bEnableLastNAlarm;

		//二次排盖相关逻辑
		if (rs.m_bReRejectFunc)
		{
			GetDlgItem(IDC_CHECK_ENABLE_RE_REJECT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SYS_REJECTOR_SITOSOL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SYS_DLG_SITOSOL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SYS_REJECTOR_DELAY_RE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SYS_DLG_DELAY_RE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_SYS_REJECTOR_SIGNAL_WIDTH_RE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_CHECK_ENABLE_RE_REJECT)->EnableWindow(TRUE);

			if (rs.m_bReReject)
			{
				((CButton*)GetDlgItem(IDC_CHECK_ENABLE_RE_REJECT))->SetCheck(BST_CHECKED);
				GetDlgItem(IDC_STATIC_SYS_REJECTOR_SITOSOL)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_SYS_DLG_SITOSOL)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_SYS_REJECTOR_DELAY_RE)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_SYS_DLG_DELAY_RE)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIC_SYS_REJECTOR_SIGNAL_WIDTH_RE)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE)->EnableWindow(TRUE);

				int nReSiToSol = (m_rjMode == 0) ? int(rs.m_nReSitoSol / 10.0f) : (rs.m_nReSitoSol);
				strValue.Format(_T("%d"), nReSiToSol);
				GetDlgItem(IDC_EDIT_SYS_DLG_SITOSOL)->SetWindowText(strValue);

				int nReDelay = (m_rjMode == 0) ? int(rs.m_nReDelay / 10.0f) : (rs.m_nReDelay);
				strValue.Format(_T("%d"), nReDelay);
				GetDlgItem(IDC_EDIT_SYS_DLG_DELAY_RE)->SetWindowText(strValue);

				strValue.Format(_T("%d"), rs.m_nReSignalWidth);
				GetDlgItem(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE)->SetWindowText(strValue);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_ENABLE_RE_REJECT))->SetCheck(BST_UNCHECKED);

				GetDlgItem(IDC_STATIC_SYS_REJECTOR_SITOSOL)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_SYS_DLG_SITOSOL)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_SYS_REJECTOR_DELAY_RE)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_SYS_DLG_DELAY_RE)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_SYS_REJECTOR_SIGNAL_WIDTH_RE)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE)->EnableWindow(FALSE);

			}
		}
		else
		{
			GetDlgItem(IDC_CHECK_ENABLE_RE_REJECT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SYS_REJECTOR_SITOSOL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SYS_DLG_SITOSOL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SYS_REJECTOR_DELAY_RE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SYS_DLG_DELAY_RE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SYS_REJECTOR_SIGNAL_WIDTH_RE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE)->ShowWindow(SW_HIDE);
		}

		m_bEnableTest = rs.m_bTestRejector;
		m_nTestMode   = rs.m_nTestMode;
		m_nActionType = (int)rs.m_sa;
		pWnd = (CWnd*)GetDlgItem(IDC_RADIO_REJECT_ALL);
		pWnd->EnableWindow(m_bEnableTest);

		pWnd = (CWnd*)GetDlgItem(IDC_RADIO_OPTIONAL_REJECT);
		pWnd->EnableWindow(m_bEnableTest);

		pWnd = (CWnd*)GetDlgItem(IDC_EDIT_NUMERATOR);
		pWnd->EnableWindow(m_bEnableTest && m_nTestMode == 1);

		pWnd = (CWnd*)GetDlgItem(IDC_EDIT_DENOMINATOR);
		pWnd->EnableWindow(m_bEnableTest && m_nTestMode == 1);

		pWnd = (CWnd*)GetDlgItem(IDC_RADIO_ODD);
		pWnd->EnableWindow(m_bEnableTest);

		pWnd = (CWnd*)GetDlgItem(IDC_RADIO_EVEN);
		pWnd->EnableWindow(m_bEnableTest);

		CString strNumerator = _T("");
		CString strDemoninator = _T("");
		if(m_bEnableTest && m_nTestMode == 1)
		{
			strNumerator.Format(_T("%d"),rs.m_nNumerator);
			strDemoninator.Format(_T("%d"),rs.m_nDenominator);
		}
		SetDlgItemTextW(IDC_EDIT_NUMERATOR,strNumerator);
		SetDlgItemTextW(IDC_EDIT_DENOMINATOR,strDemoninator);

		CString sCDC;
		sCDC.Format(_T("%d"), rs.m_nCDC);
		SetDlgItemTextW(IDC_EDIT_CDC_UCL, sCDC);

		bool bChCameraRun = ::gSystemMgr.IsChannelCameraRunning(chIndex);
		EnableAllWidge(!bChCameraRun);
	}
	UpdateData(FALSE);
}

void CRejectorMgrDlg::FillUpUiWidget()
{
	size_t sz = m_pDictChConfig->size();
	if(sz == 0)
		return;

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

	m_nCrtChIndex = 1;
	DisplayRejectorSettings(m_nCrtChIndex);
}

void CRejectorMgrDlg::EnableUiWidget(int nCtrlID, BOOL bEnable)
{
	CWnd* pWnd = (CWnd*)GetDlgItem(nCtrlID);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
}

void CRejectorMgrDlg::EnableAllWidge(bool bEnable)
{
	EnableUiWidget(IDC_COMBO_REJECTOR_PORT,bEnable);
	EnableUiWidget(IDC_COMBO_DROPPED_IMAGE_PORT,bEnable);
	EnableUiWidget(IDC_RADIO_RM_DISTANCE, bEnable);
	EnableUiWidget(IDC_RADIO_RM_TIME,bEnable);
	EnableUiWidget(IDC_EDIT_SYS_DLG_DELAY,bEnable);
	EnableUiWidget(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH,bEnable);

	bool bEnableTest = bEnable && m_bEnableTest;
	EnableUiWidget(IDC_CHECK_ENABLE_TEST,bEnable);
	EnableUiWidget(IDC_RADIO_REJECT_ALL, bEnableTest);
	EnableUiWidget(IDC_RADIO_OPTIONAL_REJECT, bEnableTest);
	EnableUiWidget(IDC_RADIO_ODD,bEnableTest);
	EnableUiWidget(IDC_RADIO_EVEN,bEnableTest);

	EnableUiWidget(IDC_EDIT_CDC_UCL,bEnable);
	EnableUiWidget(IDC_STATIC_APPLY_IO_PARAM,bEnable);
    EnableUiWidget(IDC_COMBO_N_OPTION,bEnable);
    EnableUiWidget(IDC_EDIT_LAST_N_DEFECTIVE_RATIO,bEnable);
    EnableUiWidget(IDC_CHECK_LAST_N_ENABLE,bEnable);
}

void CRejectorMgrDlg::ListAvailablePort()
{
	int nPortCount = 16;
#ifdef PCI2310
	nPortCount = 32;
#endif

	CComboBox* pCombo1 = (CComboBox*)GetDlgItem(IDC_COMBO_REJECTOR_PORT);
	CComboBox* pCombo2 = (CComboBox*)GetDlgItem(IDC_COMBO_DROPPED_IMAGE_PORT);
	if(pCombo1 != nullptr && pCombo2 != nullptr)
	{
		CString sIndex;
		for(int i = 0; i < nPortCount; i++)
		{
			sIndex.Format(_T("%d"),i);
			pCombo1->AddString(sIndex);
			pCombo2->AddString(sIndex);
		}
	}
}

BOOL CRejectorMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	InitUiWidget(IDC_STATIC_SYS_DLG_CRT_CH, IDS_STRING_SYS_DLG_CRT_CH);
	InitUiWidget(IDC_COMBO_CRT_CHANNEL_ID);

	InitUiWidget(IDC_STATIC_DEVICE_IP,IDS_STRING_DEVICE_IP);
	InitUiWidget(IDC_EDIT_DEVICE_IP);
	// Camera GrabTime
	InitUiWidget(IDC_STATIC_GRABTIME,IDS_STRING_GRABTIME);
	InitUiWidget(IDC_EDIT_GRABTIME);
	// Camera TriggerIngoreTime

	InitUiWidget(IDC_STATIC_TRIGGER_INGNOR_TIME,IDS_STRING_TRIGGERINGORETIME);
	InitUiWidget(IDC_EDIT_TRIGGER_INGNOR_TIME);
//	ListAvailablePort();

	InitUiWidget(IDC_STATIC_SYS_REJECTOR_CFRAME,IDS_STRING_REJECTOR_CFRAME);
	InitUiWidget(IDC_EDIT_SYS_DLG_CFRAME);

	InitUiWidget(IDC_STATIC_SYS_REJECTOR_CDOWN,IDS_STRING_REJECTOR_CDOWN);
	InitUiWidget(IDC_EDIT_SYS_DLG_CDOWN);

	InitUiWidget(IDC_RADIO_RM_DISTANCE, IDS_STRING_RM_DISTANCE);
	InitUiWidget(IDC_RADIO_RM_TIME, IDS_STRING_RM_TIME);

	InitUiWidget(IDC_STATIC_SYS_REJECTOR_DELAY, IDS_STRING_REJECTOR_DELAY);
	InitUiWidget(IDC_EDIT_SYS_DLG_DELAY);

	InitUiWidget(IDC_STATIC_SYS_REJECTOR_SIGNAL_WIDTH, IDS_STRING_REJECTOR_SIGNAL_WIDTH);
	InitUiWidget(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH);

	InitUiWidget(IDC_CHECK_ENABLE_RE_REJECT,IDS_STRING_ENABLE_RE_REJECT);
	InitUiWidget(IDC_STATIC_SYS_REJECTOR_DELAY_RE, IDS_STRING_REJECTOR_DELAY);
	InitUiWidget(IDC_EDIT_SYS_DLG_DELAY_RE);
	InitUiWidget(IDC_STATIC_RM_UNIT_MS);

	InitUiWidget(IDC_STATIC_SYS_REJECTOR_SIGNAL_WIDTH_RE, IDS_STRING_REJECTOR_SIGNAL_WIDTH);
	InitUiWidget(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE);

	InitUiWidget(IDC_STATIC_RJ_DLG_MSG_PANEL);

	InitUiWidget(IDC_STATIC_RJ_TEST,IDS_STRING_REJECTION_TEST);
	InitUiWidget(IDC_CHECK_ENABLE_TEST, IDS_STRING_TEST_REJECTION);
	InitUiWidget(IDC_RADIO_REJECT_ALL, IDS_STRING_REJECT_ALL);
	InitUiWidget(IDC_RADIO_OPTIONAL_REJECT, IDS_STRING_OPTIONAL_REJECT);
	InitUiWidget(IDC_EDIT_NUMERATOR);
	InitUiWidget(IDC_STATIC_SLASH);
	InitUiWidget(IDC_EDIT_DENOMINATOR);
	InitUiWidget(IDC_RADIO_ODD, IDS_STRING_REJECT_ODD);
	InitUiWidget(IDC_RADIO_EVEN, IDS_STRING_REJECT_EVEN);

	InitUiWidget(IDC_RADIO_ACTION_PER_ONE,IDS_STRING_ACTION_PER_ONE);
	InitUiWidget(IDC_RADIO_ACTION_FOR_ALL,IDS_STRING_ACTION_ONCE_FOR_ALL);

	InitUiWidget(IDC_STATIC_CDC,IDS_STRING_CDC_SETTING);
	InitUiWidget(IDC_STATIC_CDC_UCL,IDS_STRING_CDC_UCL);
	InitUiWidget(IDC_EDIT_CDC_UCL);

	InitUiWidget(IDC_STATIC_SYS_REJECTOR_SITOSOL,IDS_STRING_TWO_SENSOR_DISTANCE);
	InitUiWidget(IDC_EDIT_SYS_DLG_SITOSOL);

    InitUiWidget(IDC_STATIC_LAST_N,IDS_STRING_LAST_N_SETTING);
    InitUiWidget(IDC_STATIC_N_OPTION,IDS_STRING_LAST_N);
    InitUiWidget(IDC_COMBO_N_OPTION);
    InitUiWidget(IDC_STATIC_LAST_N_DEFECTIVE_RATIO,IDS_STRING_LAST_N_DEFECTIVE_UCL);
    InitUiWidget(IDC_EDIT_LAST_N_DEFECTIVE_RATIO);
    InitUiWidget(IDC_STATIC_PERCENT);
    InitUiWidget(IDC_CHECK_LAST_N_ENABLE,IDS_STRING_LAST_N_ENABLE_ALARM);
	InitUiWidget(IDC_HD_TEST,IDS_STRING_HD_TEST);
	m_bEnableTest = FALSE;

	UpdateData(FALSE);

	m_pDictChConfig = gConfigMgr.GetChannelConfigPtr(); 
	if(m_pDictChConfig == nullptr)
		return FALSE;

	FillUpUiWidget();
	return TRUE;
}

BOOL CRejectorMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CRejectorMgrDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CRT_CHANNEL_ID, &CRejectorMgrDlg::OnCbnSelchangeComboCrtChannelId)
	ON_STN_CLICKED(IDC_STATIC_APPLY_IO_PARAM, &CRejectorMgrDlg::OnStnClickedStaticApplyIoParam)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_ENABLE_TEST, &CRejectorMgrDlg::OnBnClickedCheckEnableTest)
	ON_BN_CLICKED(IDC_RADIO_REJECT_ALL, &CRejectorMgrDlg::OnBnClickedRadioRejectAll)
	ON_BN_CLICKED(IDC_RADIO_OPTIONAL_REJECT, &CRejectorMgrDlg::OnBnClickedRadioOptionalReject)
	ON_BN_CLICKED(IDC_RADIO_ODD, &CRejectorMgrDlg::OnBnClickedRadioOdd)
	ON_BN_CLICKED(IDC_RADIO_EVEN, &CRejectorMgrDlg::OnBnClickedRadioEven)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_RE_REJECT, &CRejectorMgrDlg::OnBnClickedCheckEnableReReject)
	ON_BN_CLICKED(IDC_HD_TEST,&CRejectorMgrDlg::OnBnClickedHDTest)
END_MESSAGE_MAP()


void CRejectorMgrDlg::OnBnClickedHDTest()
{
	gSystemMgr.GetPanel(m_nCrtChIndex)->ExecuteCmd(QOP_PARAM_SET,HDW_TEST,1);
}

void CRejectorMgrDlg::OnCbnSelchangeComboCrtChannelId()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	if(pCombo == nullptr)
		return;

	m_nCrtChIndex = pCombo->GetCurSel();
	m_nCrtChIndex += 1;
	DisplayRejectorSettings(m_nCrtChIndex);
}

void CRejectorMgrDlg::OnStnClickedStaticApplyIoParam()
{
	UpdateData(TRUE);
	
	REJECTOR_SETTING newSetting;
	newSetting.m_mode = (m_rjMode == 0) ? DELAY_TIME : DELAY_DISTANCE;

	CString sIp;
	GetDlgItemTextW(IDC_EDIT_DEVICE_IP,sIp);
	newSetting.m_wstrDeviceIp = CT2W(sIp);

	CString sGrabTime, sTriggerIngoreTime;
	GetDlgItemTextW(IDC_EDIT_GRABTIME, sGrabTime);
	GetDlgItemTextW(IDC_EDIT_TRIGGER_INGNOR_TIME, sTriggerIngoreTime);

	newSetting.m_CameraGrabTime =(int)utils::to_float(sGrabTime.GetLength()>0?sGrabTime:L"0");
	newSetting.m_TriggerIngoreTime = (int)utils::to_float(sTriggerIngoreTime.GetLength()>0?sTriggerIngoreTime:L"0");

	CString sCFrameAlarm,sCRejectAlarm;
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_CFRAME, sCFrameAlarm);
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_CDOWN, sCRejectAlarm);

	newSetting.m_SignalIngoreTime =(int)utils::to_float(sCFrameAlarm.GetLength()>0?sCFrameAlarm:L"0"); //感应器屏蔽时间
	newSetting.m_nContinueRejectAlarm = (int)utils::to_float(sCRejectAlarm.GetLength()>0?sCRejectAlarm:L"0");

	CString sMsg;
	CString sDelay, sSignalWidth;
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_DELAY, sDelay);
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH, sSignalWidth);
	if(!utils::IsNumericString(sDelay,true) || !utils::IsNumericString(sSignalWidth,true))
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	int nRawDelay = (int)utils::to_float(sDelay);
	int nDelay = (m_rjMode == 0) ? (1 * nRawDelay) : (nRawDelay);
	newSetting.m_delay       = nDelay;
	newSetting.m_signalWidth = (int)utils::to_float(sSignalWidth);

	if(newSetting.m_delay <= 0 ||newSetting.m_signalWidth <= 0)
	{
		sMsg.LoadString(IDS_STRING_PARAM_CANNOT_BE_NEGATIVE);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	newSetting.m_bReRejectFunc = m_bReRejectFunc;
	//二次排盖数据读取
	newSetting.m_bReReject = (TRUE == m_bReReject);
	if (m_bReReject)
	{
		CString sMsg;
		CString sReSiToSol,sReDelay, sReSignalWidth;
		GetDlgItemTextW(IDC_EDIT_SYS_DLG_SITOSOL, sReSiToSol);
		GetDlgItemTextW(IDC_EDIT_SYS_DLG_DELAY_RE, sReDelay);
		GetDlgItemTextW(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE, sReSignalWidth);

		int nRawReSiToSol = (int)utils::to_float(sReSiToSol);
		int nReSiToSol = (m_rjMode == 0) ? (nRawReSiToSol) : (nRawReSiToSol);
		newSetting.m_nReSitoSol     = nReSiToSol;

		int nRawReDelay = (int)utils::to_float(sReDelay);
		int nReDelay = (m_rjMode == 0) ? (nRawReDelay) : (nRawReDelay);
		newSetting.m_nReDelay       = nReDelay;

		newSetting.m_nReSignalWidth = (int)utils::to_float(sReSignalWidth);
	}

	newSetting.m_bTestRejector = (TRUE == m_bEnableTest);
	newSetting.m_nTestMode     = m_nTestMode;
	if(m_bEnableTest)
	{
		if(m_nTestMode == 3)
		{
			CString sNumerator, sDemominator;
			GetDlgItemTextW(IDC_EDIT_NUMERATOR,   sNumerator);
			GetDlgItemTextW(IDC_EDIT_DENOMINATOR, sDemominator);
			if(!utils::IsNumericString(sNumerator,true) || !utils::IsNumericString(sDemominator,true))
			{
				sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
				DisplayPromptMessage(sMsg, false);
				return;
			}

			int numerator   = (int)utils::to_float(sNumerator);
			int denominator = (int)utils::to_float(sDemominator);
			if(numerator <= 0 ||denominator <= 0)
			{
				sMsg.LoadString(IDS_STRING_PARAM_CANNOT_BE_NEGATIVE);
				DisplayPromptMessage(sMsg, false);
				return;
			}
			newSetting.m_nNumerator   = numerator;
			newSetting.m_nDenominator = denominator;
		}
	}
	newSetting.m_sa = (m_nActionType == 0) ? SA_SWITCH_FOR_EACH : SA_SWITCH_FOR_ALL;

	CString sCDC;
	GetDlgItemTextW(IDC_EDIT_CDC_UCL, sCDC);
	if(!utils::IsNumericString(sCDC,true))
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	wstring wstrCDC = CT2W(sCDC);
	int nCDC        = boost::lexical_cast<int>(wstrCDC);
	if(nCDC < 0 || nCDC > 32)
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_WITHIN_VALID_RANGE);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	newSetting.m_nCDC = nCDC;
    // Last N
    CComboBox* pCb = (CComboBox*)GetDlgItem(IDC_COMBO_N_OPTION);
    int nIndex = pCb->GetCurSel();
    newSetting.m_nLastN = (nIndex + 1) * 5000;
    CString sLastNRatio;
    GetDlgItemTextW(IDC_EDIT_LAST_N_DEFECTIVE_RATIO, sLastNRatio);
    if(!utils::IsNumericString(sLastNRatio))
    {
        sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
        DisplayPromptMessage(sMsg, false);
        return;
    }
    newSetting.m_fLastNRatio = boost::lexical_cast<float>(sLastNRatio.GetString());
    newSetting.m_bEnableLastNAlarm = (bool)m_bEnableLastNAlarm;
    CHANNEL_CONFIG chConfig ;
    
	if(m_pDictChConfig->find(m_nCrtChIndex) != m_pDictChConfig->end())
	{
		chConfig  = (*m_pDictChConfig)[m_nCrtChIndex];
		if(newSetting.IsEqual(chConfig.m_rejectorSetting))
			return;
	}

	if(gConfigMgr.UpdateRejectorSetting(m_nCrtChIndex, &newSetting))
	{
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		DisplayPromptMessage(sMsg, true);
        newSetting.m_mode = chConfig.m_vecCameraSetting[0].m_delayType;
		gSystemMgr.UpdateRejectorSetting(m_nCrtChIndex, newSetting);
	}
	else
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		DisplayPromptMessage(sMsg, false);
	}
}

void CRejectorMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_RJ_DLG_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_RJ_DLG_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_RJ_DLG,5000, NULL);
}

HBRUSH CRejectorMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_RADIO_RM_DISTANCE || 
	   id == IDC_RADIO_RM_TIME || 
	   id == IDC_STATIC_RJ_DLG_MSG_PANEL ||
	   id == IDC_STATIC_RJ_TEST ||
	   id == IDC_CHECK_ENABLE_TEST ||
	   id == IDC_RADIO_REJECT_ALL ||
	   id == IDC_RADIO_OPTIONAL_REJECT ||
	   id == IDC_RADIO_ODD ||
	   id == IDC_RADIO_EVEN ||
	   id == IDC_RADIO_ACTION_PER_ONE ||
	   id == IDC_RADIO_ACTION_FOR_ALL ||
	   id == IDC_CHECK_ENABLE_RE_REJECT||
       id == IDC_CHECK_LAST_N_ENABLE)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		if(id == IDC_STATIC_RJ_DLG_MSG_PANEL)
		{
			if(m_bOpRslt)
				pDC->SetTextColor(RGB(50,200,20));
			else
				pDC->SetTextColor(RGB(200,20,20));
		}
		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CRejectorMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_RJ_DLG)
	{
		SetDlgItemTextW(IDC_STATIC_RJ_DLG_MSG_PANEL, _T(" "));
		RedrawWindow();
		KillTimer(TIMER_ID_RJ_DLG);
	}
}

void CRejectorMgrDlg::OnBnClickedCheckEnableTest()
{
	UpdateData(TRUE);

	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_RADIO_REJECT_ALL);
	pWnd->EnableWindow(m_bEnableTest);

	pWnd = (CWnd*)GetDlgItem(IDC_RADIO_OPTIONAL_REJECT);
	pWnd->EnableWindow(m_bEnableTest);

	pWnd = (CWnd*)GetDlgItem(IDC_EDIT_NUMERATOR);
	pWnd->EnableWindow(m_bEnableTest && m_nTestMode != 0);
	SetDlgItemTextW(IDC_EDIT_NUMERATOR, _T(""));

	pWnd = (CWnd*)GetDlgItem(IDC_EDIT_DENOMINATOR);
	pWnd->EnableWindow(m_bEnableTest && m_nTestMode != 0);
	SetDlgItemTextW(IDC_EDIT_DENOMINATOR, _T(""));

	pWnd = (CWnd*)GetDlgItem(IDC_RADIO_ODD);
	pWnd->EnableWindow(m_bEnableTest);

	pWnd = (CWnd*)GetDlgItem(IDC_RADIO_EVEN);
	pWnd->EnableWindow(m_bEnableTest);

	UpdateData(FALSE);
}

void CRejectorMgrDlg::OnBnClickedRadioRejectAll()
{
	UpdateData(TRUE);
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_EDIT_NUMERATOR);
	pWnd->EnableWindow(FALSE);

	pWnd = (CWnd*)GetDlgItem(IDC_EDIT_DENOMINATOR);
	pWnd->EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CRejectorMgrDlg::OnBnClickedRadioOptionalReject()
{
	UpdateData(TRUE);
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_EDIT_NUMERATOR);
	pWnd->EnableWindow(TRUE);

	pWnd = (CWnd*)GetDlgItem(IDC_EDIT_DENOMINATOR);
	pWnd->EnableWindow(TRUE);
	UpdateData(FALSE);
}

void CRejectorMgrDlg::OnBnClickedRadioOdd()
{
	UpdateData(TRUE);
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_EDIT_NUMERATOR);
	pWnd->EnableWindow(FALSE);

	pWnd = (CWnd*)GetDlgItem(IDC_EDIT_DENOMINATOR);
	pWnd->EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CRejectorMgrDlg::OnBnClickedRadioEven()
{
	UpdateData(TRUE);
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_EDIT_NUMERATOR);
	pWnd->EnableWindow(FALSE);

	pWnd = (CWnd*)GetDlgItem(IDC_EDIT_DENOMINATOR);
	pWnd->EnableWindow(FALSE);

	UpdateData(FALSE);
}


void CRejectorMgrDlg::OnBnClickedCheckEnableReReject()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_RE_REJECT))->GetCheck();
	if (i == BST_UNCHECKED)
	{
		m_bReReject = false;
		GetDlgItem(IDC_STATIC_SYS_REJECTOR_SITOSOL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SYS_DLG_SITOSOL)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SYS_REJECTOR_DELAY_RE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SYS_DLG_DELAY_RE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SYS_REJECTOR_SIGNAL_WIDTH_RE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE)->EnableWindow(FALSE);
	}
	else
	{
		m_bReReject = true;
		GetDlgItem(IDC_STATIC_SYS_REJECTOR_SITOSOL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SYS_DLG_SITOSOL)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SYS_REJECTOR_DELAY_RE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SYS_DLG_DELAY_RE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SYS_REJECTOR_SIGNAL_WIDTH_RE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SYS_DLG_SIGNAL_WIDTH_RE)->EnableWindow(TRUE);
	}
}
