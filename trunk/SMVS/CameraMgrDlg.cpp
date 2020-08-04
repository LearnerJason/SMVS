#include "stdafx.h"
#include "SMVS.h"
#include "CameraMgrDlg.h"
#include "ChannelControlPanel.h"
#include "afxdialogex.h"
#include "Basler.h"


IMPLEMENT_DYNAMIC(CCameraMgrDlg, CDialogEx)

CCameraMgrDlg::CCameraMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraMgrDlg::IDD, pParent)
	, m_nCrtChIndex(-1)
	, m_bOpRslt(true)
	, m_pDictChConfig(nullptr)
	, TIMER_ID_CAMERA_DLG(100002)
	, m_delayType(0)
	, m_ListIndex(-1)
	,m_CurCHListIndex(-1)
{
	m_textFont.CreateFont(18 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CCameraMgrDlg::~CCameraMgrDlg()
{
}

void CCameraMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CAMERA_LIST, m_cameraList);
	DDX_Radio(pDX, IDC_RADIO_CAMERA_DELAY_TIME, m_delayType);
	DDX_Control(pDX, IDC_LIST_CAMERA_LIST_CH, m_CurCHcameraList);
}

void CCameraMgrDlg::InitUiWidget(int nCtrlID, int strID)
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

void CCameraMgrDlg::FillUpUiWidget(int CameraNumID)
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

	CString str;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_CAMERA_MODE);
	str.LoadString(IDS_STRING_TRIGGER_MODE);
	pCombo->AddString(str);

	str.LoadString(IDS_STRING_FREE_MODE);
	pCombo->AddString(str);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_EDGE_CTRL);
	str.LoadString(IDS_STRING_FALL_EDGE);
	pCombo->AddString(str);
	str.LoadString(IDS_STRING_RISE_EDGE);
	pCombo->AddString(str);

	m_nCrtChIndex = 1;
	DisplayCameraSettings(m_nCrtChIndex);
}

void CCameraMgrDlg::DisplayCameraSettings(int chIndex,int CameraNumID)
{
	if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
	{
		//display camera params adapt
		CHANNEL_CONFIG chConfigNew  = (*m_pDictChConfig)[chIndex];
		CHANNEL_CONFIG chConfig;

		bool bTaskParamAttach = false;
		TASK* pCrtTask = (*m_pDictChConfig)[m_nCrtChIndex].GetCurrentTask();
		if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
		{
			bTaskParamAttach = true;
		}
			
		//all camerasetting removed
		if (chConfigNew.m_vecCameraSetting.empty())
		{
			EnableAllWidge(false);
			return;
		}
		chConfig.m_cameraSetting = chConfigNew.m_vecCameraSetting[CameraNumID];
		
		//参数绑定情况下，把任务绑定的参数显示于设置界面的相机参数
		if (bTaskParamAttach)
		{
			chConfig.m_cameraSetting.m_delayType = pCrtTask->m_vecTaskCameraSetting[CameraNumID].m_delayType;
			chConfig.m_cameraSetting.m_delayValue = pCrtTask->m_vecTaskCameraSetting[CameraNumID].m_delayValue;
			chConfig.m_cameraSetting.m_ExposureTime= pCrtTask->m_vecTaskCameraSetting[CameraNumID].m_ExposureTime;
			chConfig.m_cameraSetting.m_gain = pCrtTask->m_vecTaskCameraSetting[CameraNumID].m_gain;
		}

		CWnd* pWnd = GetDlgItem(IDC_EDIT_SYS_DLG_CAMERA_NAME);
		pWnd->SetWindowText(CW2T(chConfig.m_cameraSetting.m_name.c_str()));

		CString sStatus;
		bool bValidCamera = true;
		if(m_dictCamera_State.find(chConfig.m_cameraSetting.m_name) != m_dictCamera_State.end())
		{
			bool bAccessible = m_dictCamera_State[chConfig.m_cameraSetting.m_name];
			if(bAccessible)
			{
				sStatus.LoadString(IDS_STRING_CAMERA_NO_USE);
			}
			else
			{
				sStatus.LoadString(IDS_STRING_CAMERA_IN_USE);
			}
		}
		else
		{
			bValidCamera = false;
			sStatus.LoadString(IDS_STRING_CANNOT_CONNECT_TO_CAMERA);
		}
		SetDlgItemTextW(IDC_EDIT_SYS_CAMERA_STATE, sStatus);
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_CAMERA_MODE);
		int index = (chConfig.m_cameraSetting.m_bTriggerMode == true) ? 0 : 1; 
		pCombo->SetCurSel(index);

		CString strValue;
		strValue.Format(_T("%d"),chConfig.m_cameraSetting.m_nFrameRate);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_RATE);
		if(pEdit != nullptr)
		{
			pEdit->EnableWindow(chConfig.m_cameraSetting.m_bTriggerMode == false);
			pEdit->SetWindowText(strValue);
		}

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_EDGE_CTRL);
		index = (chConfig.m_cameraSetting.m_triggerAct == FALL_EDGE) ? 0 : 1;
		pCombo->SetCurSel(index);

		
		SetDlgItemInt(IDC_EDIT_SYS_DLG_CAMERA_GAIN,chConfig.m_cameraSetting.m_gain);
		SetDlgItemInt(IDC_EDIT_SYS_DLG_CAMERA_ExposureTime,chConfig.m_cameraSetting.m_ExposureTime);

		m_delayType = (int)chConfig.m_cameraSetting.m_delayType;
		int nDelay = (m_delayType == 0) ? int(chConfig.m_cameraSetting.m_delayValue / 5.0f) : chConfig.m_cameraSetting.m_delayValue;
		SetDlgItemInt(IDC_EDIT_SYS_DLG_CAMERA_DELAY,nDelay);
		SetDlgItemInt(IDC_EDIT_SYS_DLG_CAMERA_PACKET_SIZE,chConfig.m_cameraSetting.m_nPacketSz);
		SetDlgItemInt(IDC_EDIT_SYS_DLG_CAMERA_LOST_IMAGE_UCL,chConfig.m_cameraSetting.m_nDroppedImgUcl);

		bool bChCameraRun = ::gSystemMgr.IsChannelCameraRunning(chIndex);
		if(bChCameraRun)
		{
			EnableAllWidge(false);
		}
		else
		{
			if(!bValidCamera)
			{
				EnableAllWidge(false);
				EnableUiWidget(IDC_LIST_CAMERA_LIST, true);
				EnableUiWidget(IDC_STATIC_APPLY_CAMERA_PARAM,true);
			}
			else
			{
				EnableAllWidge(true);
			}
		}
	}
	UpdateData(FALSE);
}

void CCameraMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_CAMERA_DLG_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_CAMERA_DLG_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_CAMERA_DLG,5000, NULL);
}
void CCameraMgrDlg::DisPlayCurCHCamera(int chIndex)
{
	m_CurCHcameraList.DeleteAllItems();

	if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
	{
		//display camera params adapt
		CHANNEL_CONFIG chConfigNew  = (*m_pDictChConfig)[chIndex];
		int listindex = 0;
		for (auto it = chConfigNew.m_vecCameraSetting.begin(); it != chConfigNew.m_vecCameraSetting.end(); it++)
		{
			CString strIndex;
			CAMERA_SETTING camerasetting = (*it);
			strIndex.Format(_T("%d"), listindex + 1);
			m_CurCHcameraList.InsertItem(listindex, strIndex);
			m_CurCHcameraList.SetItemText(listindex, 1, W2CT(camerasetting.m_name.c_str()));
			listindex ++;;
		}
	}
}
void CCameraMgrDlg::DisplayAllAvailableCamera()
{
	m_cameraList.DeleteAllItems();
	if(!m_dictCamera_State.empty())
	{
		int index = 0;
		CString strIndex;
		CString sState;
		for(auto it = m_dictCamera_State.begin(); it != m_dictCamera_State.end(); it++)
		{
			strIndex.Format(_T("%d"), index + 1);
			m_cameraList.InsertItem(index, strIndex);
			m_cameraList.SetItemText(index, 1, W2CT((*it).first.c_str()));
			bool bAccessible = (*it).second;
			if(bAccessible)
			{
				sState.LoadString(IDS_STRING_CAMERA_NO_USE);
				m_cameraList.SetItemText(index, 2, sState);
			}
			else
			{
				sState.LoadString(IDS_STRING_CAMERA_IN_USE);
				m_cameraList.SetItemText(index, 2, sState);
			}
			index++;
		}
	}
}

BOOL CCameraMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);
	
	InitUiWidget(IDC_STATIC_CAMERA_LIST_TITLE, IDS_STRING_SYS_DLG_CAMERA_LIST_TITLE);
	InitUiWidget(IDC_LIST_CAMERA_LIST);
	InitUiWidget(IDC_LIST_CAMERA_LIST_CH);

	InitUiWidget(IDC_STATIC_SYS_DLG_CRT_CH, IDS_STRING_SYS_DLG_CRT_CH);
	InitUiWidget(IDC_COMBO_CRT_CHANNEL_ID);

	InitUiWidget(IDC_STATIC_SYS_DLG_CAMERA_NAME, IDS_STRING_SYS_DLG_CAMERA_NAME);
	InitUiWidget(IDC_EDIT_SYS_DLG_CAMERA_NAME);

	// Camera Status
	InitUiWidget(IDC_STATIC_CAMERA_STATUS, IDS_STRING_SYS_DLG_CAMERA_STATE);
	InitUiWidget(IDC_EDIT_SYS_CAMERA_STATE);

	// Run Mode
	InitUiWidget(IDC_STATIC_SYS_DLG_CAMERA_MODE, IDS_STRING_ASYNC_MODE);
	InitUiWidget(IDC_COMBO_SYS_DLG_CAMERA_MODE);

	// Frame Rate
	InitUiWidget(IDC_STATIC_FRAME_RATE, IDS_STRING_FRAME_RATE);
	InitUiWidget(IDC_EDIT_FRAME_RATE);

	// Trigger Edge
	InitUiWidget(IDC_STATIC_SYS_DLG_EDGE_CONTROL, IDS_STRING_EDGE_CTRL);
	InitUiWidget(IDC_COMBO_SYS_DLG_EDGE_CTRL);

	//Exposure time
	InitUiWidget(IDC_STATIC_SYS_DLG_CAMERA_ExposureTime, IDS_STRING_ExposureTime);
	InitUiWidget(IDC_EDIT_SYS_DLG_CAMERA_ExposureTime);

	// Camera Gain
	InitUiWidget(IDC_STATIC_SYS_DLG_CAMERA_GAIN, IDS_STRING_CAMERA_GAIN);
	InitUiWidget(IDC_EDIT_SYS_DLG_CAMERA_GAIN);


	// Trigger Delay
	InitUiWidget(IDC_RADIO_CAMERA_DELAY_TIME, IDS_STRING_RM_TIME);
	InitUiWidget(IDC_RADIO_CAMERA_DELAY_DISTANCE, IDS_STRING_RM_DISTANCE);
	InitUiWidget(IDC_STATIC_SYS_DLG_CAMERA_DELAY, IDS_STRING_DELAY);
	InitUiWidget(IDC_EDIT_SYS_DLG_CAMERA_DELAY);

	// Packet size
	InitUiWidget(IDC_STATIC_SYS_DLG_CAMERA_PACKET_SIZE, IDS_STRING_PACKET_SIZE);
	InitUiWidget(IDC_EDIT_SYS_DLG_CAMERA_PACKET_SIZE);
	//Lost Image
	InitUiWidget(IDC_STATIC_SYS_DLG_CAMERA_LOST_IMAGE, IDS_STRING_LOST_IMG_UCL);
	InitUiWidget(IDC_EDIT_SYS_DLG_CAMERA_LOST_IMAGE_UCL);

	InitUiWidget(IDC_STATIC_CAMERA_DLG_MSG_PANEL);

	// Add Camera List Title
	DWORD dwStyleEx = m_cameraList.GetExtendedStyle();
	dwStyleEx |= LVS_EX_FULLROWSELECT; 
	dwStyleEx |= LVS_EX_GRIDLINES;     
	dwStyleEx |= LVS_EX_CHECKBOXES;   
	m_cameraList.SetExtendedStyle(dwStyleEx);
	m_cameraList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	dwStyleEx = m_CurCHcameraList.GetExtendedStyle();
	dwStyleEx |= LVS_EX_FULLROWSELECT; 
	dwStyleEx |= LVS_EX_GRIDLINES;     
	dwStyleEx |= LVS_EX_CHECKBOXES;  
	m_CurCHcameraList.SetExtendedStyle(dwStyleEx);

	CString strIndex(MAKEINTRESOURCE(IDS_STRING_INDEX));
	CString strCameraName(MAKEINTRESOURCE(IDS_STRING_SYS_DLG_CAMERA_NAME));
	CString strState(MAKEINTRESOURCE(IDS_STRING_STATUS));
	m_cameraList.InsertColumn(0, strIndex, LVCFMT_CENTER, 65);
	m_cameraList.InsertColumn(1, strCameraName, LVCFMT_LEFT, 245);
	m_cameraList.InsertColumn(2, strState, LVCFMT_LEFT, 100);

	//当前通道相机列表
	m_CurCHcameraList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_CurCHcameraList.InsertColumn(0, strIndex, LVCFMT_CENTER, 65);
	m_CurCHcameraList.InsertColumn(1, strCameraName, LVCFMT_LEFT, 245);
	m_CurCHcameraList.InsertColumn(2, strState, LVCFMT_LEFT, 100);

	m_pDictChConfig = gConfigMgr.GetChannelConfigPtr(); 
	if(m_pDictChConfig == nullptr)
		return FALSE;

	m_dictCamera_State = ::gSystemMgr.m_dictCamera_State;
	DisplayAllAvailableCamera();
	DisPlayCurCHCamera();
	FillUpUiWidget();

	return TRUE;
}

BOOL CCameraMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CCameraMgrDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CRT_CHANNEL_ID, &CCameraMgrDlg::OnCbnSelchangeComboCrtChannelId)
	ON_STN_CLICKED(IDC_STATIC_BTN_REFRESH_CAMERA_LIST, &CCameraMgrDlg::OnStnClickedStaticBtnRefreshCameraList)
	ON_STN_CLICKED(IDC_STATIC_APPLY_CAMERA_PARAM, &CCameraMgrDlg::OnStnClickedStaticApplyCameraParam)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CAMERA_LIST, &CCameraMgrDlg::OnNMClickListCameraList)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_SYS_DLG_CAMERA_MODE, &CCameraMgrDlg::OnCbnSelchangeComboSysDlgCameraMode)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CAMERA_LIST_CH, &CCameraMgrDlg::OnClickListCameraListCh)
	ON_STN_CLICKED(IDC_STATIC_BTN_REFRESH_CAMERA_LIST_ADD, &CCameraMgrDlg::OnStnClickedStaticBtnRefreshCameraListAdd)
	ON_STN_CLICKED(IDC_STATIC_BTN_REFRESH_CAMERA_LIST_DEL, &CCameraMgrDlg::OnStnClickedStaticBtnRefreshCameraListDel)
END_MESSAGE_MAP()


void CCameraMgrDlg::OnCbnSelchangeComboCrtChannelId()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	if(pCombo == nullptr)
		return;

	m_nCrtChIndex = pCombo->GetCurSel();
	m_nCrtChIndex += 1;
	m_CurCHcameraList.DeleteAllItems();
	DisPlayCurCHCamera(m_nCrtChIndex);
	DisplayCameraSettings(m_nCrtChIndex);
}

void CCameraMgrDlg::EnableUiWidget(int nCtrlID, BOOL bEnable)
{
	CWnd* pWnd = (CWnd*)GetDlgItem(nCtrlID);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
}

void CCameraMgrDlg::EnableAllWidge(bool bEnable)
{
//	EnableUiWidget(IDC_LIST_CAMERA_LIST,bEnable);
	EnableUiWidget(IDC_COMBO_SYS_DLG_CAMERA_MODE, bEnable);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_CAMERA_MODE);
	bool bFreeMode = (pCombo->GetCurSel() == 1);
	EnableUiWidget(IDC_EDIT_FRAME_RATE,bEnable && bFreeMode);

	EnableUiWidget(IDC_COMBO_SYS_DLG_EDGE_CTRL,bEnable);
	EnableUiWidget(IDC_EDIT_SYS_DLG_CAMERA_GAIN,bEnable);
	EnableUiWidget(IDC_EDIT_SYS_DLG_CAMERA_ExposureTime,bEnable);
	EnableUiWidget(IDC_RADIO_CAMERA_DELAY_TIME,bEnable);
	EnableUiWidget(IDC_RADIO_CAMERA_DELAY_DISTANCE,bEnable);
	EnableUiWidget(IDC_EDIT_SYS_DLG_CAMERA_DELAY,bEnable);
	EnableUiWidget(IDC_EDIT_SYS_DLG_CAMERA_PACKET_SIZE, bEnable);
	EnableUiWidget(IDC_EDIT_SYS_DLG_CAMERA_LOST_IMAGE_UCL, bEnable);
	EnableUiWidget(IDC_STATIC_APPLY_CAMERA_PARAM,bEnable);
}

void CCameraMgrDlg::OnStnClickedStaticBtnRefreshCameraList()
{
	if(::gSystemMgr.AllChannelCameraStopped())
	{
		gSystemMgr.ListAllAvailableCamera(m_dictCamera_State);
		DisplayAllAvailableCamera();
		gSystemMgr.m_dictCamera_State = m_dictCamera_State;
	}
}

void CCameraMgrDlg::OnStnClickedStaticApplyCameraParam()
{
	UpdateData(TRUE);

	//任务绑定参数标记
	bool bTaskParamAttach = false;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	int chIndex = pCombo->GetCurSel() + 1;
	CString sMsg;
	CAMERA_SETTING newConfig;
	newConfig.m_chIndex = chIndex;
	CString strName;
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_CAMERA_NAME, strName);
	if(strName.GetLength() <= 0)
	{
		sMsg.LoadString(IDS_STRING_CAMERA_NAME_EMPTY);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	// old camera name
	bool bOldCameraConnected = true;
	if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
	{
		CHANNEL_CONFIG oldConfig  = (*m_pDictChConfig)[chIndex];
		if(m_dictCamera_State.find(oldConfig.m_cameraSetting.m_name) == m_dictCamera_State.end())
		{
			bOldCameraConnected = false;
		}
	}

	newConfig.m_name = CT2W(strName);
	for(auto it = m_pDictChConfig->begin(); it != m_pDictChConfig->end(); it++)
	{
		if(it->first != chIndex && it->second.m_cameraSetting.m_name == newConfig.m_name)
		{
			sMsg.LoadString(IDS_STRING_WARNING_CAMERA_IS_OCCUPIED);
			sMsg.Replace(_T("#1"), strName);
			CString strChIndex;
			strChIndex.Format(_T("%d"),it->first);
			sMsg.Replace(_T("#2"), strChIndex);
			DisplayPromptMessage(sMsg, false);
			return;
		}
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_CAMERA_MODE);
	newConfig.m_bTriggerMode = (pCombo->GetCurSel() == 0);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_EDGE_CTRL);
	newConfig.m_triggerAct = (pCombo->GetCurSel() == 0) ? FALL_EDGE : RISE_EDGE;

	CString sFrameRate, sGain, sDelay, sPacketSz, sUcl,sExTime;
	GetDlgItemTextW(IDC_EDIT_FRAME_RATE, sFrameRate);
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_CAMERA_GAIN, sGain);
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_CAMERA_DELAY, sDelay);
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_CAMERA_PACKET_SIZE, sPacketSz);
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_CAMERA_LOST_IMAGE_UCL, sUcl);
	GetDlgItemTextW(IDC_EDIT_SYS_DLG_CAMERA_ExposureTime, sExTime);
	
	if(!utils::IsNumericStringEx(sFrameRate,utils::Type_Positive_integer) || 
	   !utils::IsNumericStringEx(sGain,utils::Type_Positive_integer) || 
	   !utils::IsNumericStringEx(sExTime,utils::Type_Positive_integer) || 
	   !utils::IsNumericStringEx(sPacketSz,utils::Type_Positive_integer) ||
	   !utils::IsNumericStringEx(sUcl,utils::Type_Positive_integer))
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_POSITIVE_INTEGER);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	 if(!utils::IsNumericStringEx(sDelay,utils::Type_NonNegative_integer))
	 {
		 sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_INTEGEREx);
		 DisplayPromptMessage(sMsg, false);
		 return;
	 }


	newConfig.m_nFrameRate      = (int)utils::to_float(sFrameRate);
	newConfig.m_gain            = (int)utils::to_float(sGain);
	newConfig.m_ExposureTime           = (int)utils::to_float(sExTime);
	newConfig.m_delayType       = (m_delayType == 0) ? DELAY_TIME : DELAY_DISTANCE;
	int nDelayRaw = (int)utils::to_float(sDelay);
	int nDelay = (m_delayType == 0)? (nDelayRaw * 5) : nDelayRaw;
	newConfig.m_delayValue      = nDelay;
	newConfig.m_nPacketSz       = (int)utils::to_float(sPacketSz);
	newConfig.m_nDroppedImgUcl  = (int)utils::to_float(sUcl);

	//获取任务绑定参数标记位！
	CHANNEL_CONFIG m_chConfig =  (*m_pDictChConfig)[m_nCrtChIndex];
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
	{
		bTaskParamAttach = true;
	}

	bool bCameraModeChange = false;
	int i = 0;
	if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
	{
		CHANNEL_CONFIG chConfigNew  = (*m_pDictChConfig)[chIndex];

		for (auto it = chConfigNew.m_vecCameraSetting.begin(); it != chConfigNew.m_vecCameraSetting.end(); it++)
		{
			//update camera data
			CAMERA_SETTING camerasetting = (*it);
			if (camerasetting.m_name ==newConfig.m_name)
			{
// 				if(camerasetting.IsEqual(newConfig))
// 					return;

				if (bTaskParamAttach)
				{
					//任务参数绑定下，触发模式参数会应用公用参数配置文件中
					camerasetting.m_bTriggerMode = newConfig.m_bTriggerMode;
					camerasetting.m_nFrameRate = newConfig.m_nFrameRate;
					
					if (pCrtTask->m_vecTaskCameraSetting[i].m_delayType != newConfig.m_delayType)
					{
						bCameraModeChange = true;
					}

					pCrtTask->m_vecTaskCameraSetting[i]= newConfig;

				}
				else
				{
					if (camerasetting.m_delayType != newConfig.m_delayType)
					{
						bCameraModeChange = true;
					}
					camerasetting = newConfig;
				}	
			}
			i++;
		}
	}
	//点击应用时，同时更新任务绑定的任务参数
	if (bTaskParamAttach)
	{
		string strTaskCameraSetting;
 	
 			size_t sz = pCrtTask->m_vecTaskCameraSetting.size();
 			for(size_t t = 0; t < sz; t++)
 			{	
 				if (pCrtTask->m_vecTaskCameraSetting[t].m_delayType == DELAY_TIME)
 				{
 					strTaskCameraSetting.append("T");
 				}
 				else
 				{
 					strTaskCameraSetting.append("D");
 				}
 				strTaskCameraSetting.append(",");
 				strTaskCameraSetting.append(boost::lexical_cast<string>(pCrtTask->m_vecTaskCameraSetting[t].m_delayValue));
 				strTaskCameraSetting.append(",");
 				strTaskCameraSetting.append(boost::lexical_cast<string>(pCrtTask->m_vecTaskCameraSetting[t].m_gain));
 				strTaskCameraSetting.append(",");
 				strTaskCameraSetting.append(boost::lexical_cast<string>(pCrtTask->m_vecTaskCameraSetting[t].m_ExposureTime));
 	
 				if(t != sz - 1)
 				{
 					strTaskCameraSetting.append(";");
 				}
 			}
 			gConfigMgr.UpdateTaskCameraSetting(chIndex,pCrtTask->m_wstrTaskName, strTaskCameraSetting);

			CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
		   *pChConfig = m_chConfig;
 	 }

	if (bTaskParamAttach)
	{
		CString str;
		str.LoadString(IDS_STRING_PARAM_TASK_ATTACH_TIPS);
		AfxMessageBox(str);
	}

	if(gConfigMgr.UpdateCameraSetting(chIndex, &newConfig, bTaskParamAttach))
	{
		//调试界面与设置界面参数一直，所以可以直接生效!
		gSystemMgr.UpdateCameraSetting(m_nCrtChIndex, newConfig,bCameraModeChange);

		if (bCameraModeChange)
		{
			m_chConfig.m_rejectorSetting.m_mode = newConfig.m_delayType;
			gSystemMgr.UpdateRejectorSetting(m_nCrtChIndex,m_chConfig.m_rejectorSetting);
		}
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		DisplayPromptMessage(sMsg, true);
	}
	else
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		DisplayPromptMessage(sMsg, false);
	}
	if(!bOldCameraConnected)
	{
		gSystemMgr.m_dictCamera_State[newConfig.m_name] = false;
		m_dictCamera_State = gSystemMgr.m_dictCamera_State;

		DisplayAllAvailableCamera();
		DisplayCameraSettings(chIndex);
	}
}

void CCameraMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_CAMERA_DLG)
	{
		SetDlgItemTextW(IDC_STATIC_CAMERA_DLG_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_CAMERA_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CCameraMgrDlg::OnNMClickListCameraList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_ListIndex= pNMItemActivate->iItem;

	if(m_ListIndex != -1)
	{
		CString strCameraName = m_cameraList.GetItemText(m_ListIndex, 1);

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
		int chIndex = pCombo->GetCurSel() + 1;	
		int index = 0;
		if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
		{
			//display camera params adapt
			CHANNEL_CONFIG chConfigNew  = (*m_pDictChConfig)[chIndex];
			for (auto it = chConfigNew.m_vecCameraSetting.begin(); it != chConfigNew.m_vecCameraSetting.end(); it++)
			{
				CAMERA_SETTING camerasetting = (*it);
				if (camerasetting.m_name == strCameraName.GetString())
				{
					DisplayCameraSettings(m_nCrtChIndex,index);///
					EnableAllWidge(true);
					break;
				}
				index++;
			}
		}

	//	SetDlgItemTextW(IDC_EDIT_SYS_DLG_CAMERA_NAME, strCameraName);
	//	EnableAllWidge(true);
	}

	UpdateData(FALSE);

	*pResult = 0;
}

HBRUSH CCameraMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_CAMERA_DLG_MSG_PANEL ||
	   id == IDC_RADIO_CAMERA_DELAY_TIME ||
	   id == IDC_RADIO_CAMERA_DELAY_DISTANCE)
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

void CCameraMgrDlg::OnCbnSelchangeComboSysDlgCameraMode()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_CAMERA_MODE);
	if(pCombo == nullptr)
		return;

	int chIndex = pCombo->GetCurSel();
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_FRAME_RATE);
	if(pEdit != nullptr)
	{
		pEdit->EnableWindow(chIndex == 1);
	}
	UpdateData(FALSE);
}

void CCameraMgrDlg::OnClickListCameraListCh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int index = pNMItemActivate->iItem;		//cur camera name in curCHCameraList

	m_CurCHListIndex = index;
	int chIndex=0;
	if(index != -1)
	{
		CString strCameraName = m_CurCHcameraList.GetItemText(index, 1);
		
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
		chIndex = pCombo->GetCurSel() + 1;

		if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
		{
			//display camera params adapt
			CHANNEL_CONFIG chConfigNew  = (*m_pDictChConfig)[chIndex];
			for (auto it = chConfigNew.m_vecCameraSetting.begin(); it != chConfigNew.m_vecCameraSetting.end(); it++)
			{
				CAMERA_SETTING camerasetting = (*it);
				if (camerasetting.m_name == strCameraName.GetString())
				{
					DisplayCameraSettings(m_nCrtChIndex,index);///
				}
			}
		}
		
	}
	if(chIndex>0 && gSystemMgr.GetPanel(chIndex)!=NULL)
	{
		if(gSystemMgr.GetPanel(chIndex)->GetChannelState()!=CH_ON)
			EnableAllWidge(TRUE);
	}
	UpdateData(FALSE);
	*pResult = 0;
}


void CCameraMgrDlg::OnStnClickedStaticBtnRefreshCameraListAdd()
{
	//获取相机列表 相机名字
	CString strCameraName; 
	if(m_ListIndex != -1)
	{
		strCameraName = m_cameraList.GetItemText(m_ListIndex, 1);
		SetDlgItemTextW(IDC_EDIT_SYS_DLG_CAMERA_NAME, strCameraName);
		EnableAllWidge(true);
	}
	else
	{
		return;
	}
	
	//检查相机添加，名字是否占用问题
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	int icount = pCombo->GetCount();

	for (int i = 0 ; i < icount ; i ++)
	{
		//display camera params adapt
		CHANNEL_CONFIG chConfigNew  = (*m_pDictChConfig)[i+1];
		for (auto it = chConfigNew.m_vecCameraSetting.begin(); it != chConfigNew.m_vecCameraSetting.end(); it++)
		{
			CAMERA_SETTING camerasetting = (*it);
			if (camerasetting.m_name == strCameraName.GetString())
			{
				MessageBox(_T("被占用"));
				return;
			}
		}
	}




	//当前通道相机列表，增加
	int iNum = m_CurCHcameraList.GetItemCount();
	m_CurCHcameraList.InsertItem(iNum, strCameraName);
	m_CurCHcameraList.SetItemText(iNum, 1, strCameraName);
	//配置文件添加
	CAMERA_SETTING newConfig;
	newConfig.m_name = strCameraName.GetString();


//	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	int chIndex = pCombo->GetCurSel() + 1;

	gConfigMgr.AddCameraSetting(chIndex, &newConfig);
	
	//结构体更新
	m_CurCHcameraList.DeleteAllItems();
	DisPlayCurCHCamera(chIndex);
}


void CCameraMgrDlg::OnStnClickedStaticBtnRefreshCameraListDel()
{

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	int chIndex = pCombo->GetCurSel() + 1;
	int chCount = m_CurCHcameraList.GetItemCount();


	CString strCameraName = m_CurCHcameraList.GetItemText(m_CurCHListIndex,1);

	gConfigMgr.DelCameraSetting(chIndex, strCameraName.GetString());
	
	m_CurCHcameraList.DeleteItem(m_CurCHListIndex);
	if(m_CurCHListIndex == chCount-1)
	{
		m_CurCHListIndex --;
	}
	//结构体更新

//	DisPlayCurCHCamera(chIndex);
	if (m_CurCHListIndex != -1)
	{
		DisplayCameraSettings(chIndex,m_CurCHListIndex);
	}

}
