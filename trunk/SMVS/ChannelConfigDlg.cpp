#include "stdafx.h"
#include "SMVS.h"
#include <atlconv.h>
#include "ChannelConfigDlg.h"
#include "afxdialogex.h"
#include "ImageProcessor.h"
#include "utils.h"
#include "SystemManager.h"
#include "Logger.h"
#include "TestItemManagerDlg.h"
#include "NameModifierDlg.h"
#include "LoginDlg.h"
#include "utils.h"

#include "cpp/HalconCpp.h"


extern CSystemManager gSystemMgr;
extern CLogger        gLogger;


#pragma warning(disable : 4244)
IMPLEMENT_DYNAMIC(CChannelConfigDlg, CDialogEx)

#define MAX_CONFIG_IMG_WND_COUNT 6

using namespace Halcon;

CChannelConfigDlg::CChannelConfigDlg(CHANNEL_CONFIG& chConfig,vector<CImageProcessor*> VecpProcessStation, CWnd* pParent /*=NULL*/)
	: CDialogEx(CChannelConfigDlg::IDD, pParent),
	  m_chConfig(chConfig),
	  m_bErrorMode(true),
	  m_bDispErrRgn(TRUE),
	  m_bLocked(false),
	  m_crtImgIndex(-1),
	  m_nAlgoListIndex(-1),
	  m_crtFuncInfo(nullptr),
	  m_crtFuncInfoDebug(nullptr),
	  m_pProcessStation(VecpProcessStation[0]),
	  m_pFuncInfoArray(nullptr),
	  m_settingType(T_INVALID),
	  m_crtEditCtrl(nullptr),
	  m_nImgCount(0),
	  m_bDebugMode(false),
	  m_bSelfLearningMode(false),
	  m_bSelfLearningFunc(false),
	  m_bOpRslt(false),
	  m_radioIndex(-1),
	  m_bReadImg(FALSE),
	  m_fStrideLen(0.5f),
	  m_bDecBtnHold(false),
	  m_bIncBtnHold(false),
	  m_crtFuncIndex(-1),
	  m_bRsChanged(false),
	  m_bCsChanged(false),
	  m_bLsChanged(false),
	  m_bAlgoChanged(false),
	  m_bAppliedNewParam(true),
	  m_hModelToolProc(NULL),
	  m_nCrtLcID(-1),
	  m_nCrtMode(0),
	  m_pCrtSlider(nullptr),
	  m_nCrtFuncIndex(-1),
	  m_nCrtListIndex(-1),
	  m_bThresholdChanged(false),
	  m_bEditUCL(false),
	  m_hChImgWnd(NULL),
	  m_nChImgWndID(-1),
	  m_bZoomIn(false),
	  m_fZoomInRatioTotal(1.0f),
	  m_bSingleMode(true), //光源单通道设置模式 标记变量
	  m_nActiveID(0),	   //光源参数路数标记ID
	  m_bOneTouchFour(false),  //延时和时间 一触四模式
	  m_CameraIndex(-1),
	  m_bFullScreen(false),
	  m_bExitFlag(false),
	  m_pChannelControlPanel(NULL),
	  m_bSplicing(false),
	  m_bModelFiles(true),
	  m_pComCameraStation(NULL),
	  m_pStaCameraStation(NULL),
	  m_iCameraIndex(0),
	  m_fRatio(0),
	  m_bMultiChannnelImg(false),
	  m_nImgChannelCount(1),
	  m_nCrtImgChannel(1),
	  m_bDragZoomImage(false),//放大拖动图片初始为false
	  m_bSpaceButtonDown(false)
{
	m_VecpProcessStation = VecpProcessStation;
	m_fgColor = RGB(10,10,115);
	m_bgColor = RGB(200,220,250);
	m_wndBkColor = (COLORREF)::GetSysColor(COLOR_WINDOW);

	m_bSingleModeCH1 = false;
	m_bSingleModeCH2 = false;
	m_bSingleModeCH3 = false;
	m_bSingleModeCH4 = false;

	m_bgBrush.CreateSolidBrush(RGB(200,200,200));

	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	int txtFontSz = 20;
	int txtFontPB = 17;

	m_txtFont.CreateFont(txtFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
	ANSI_CHARSET,             
	OUT_DEFAULT_PRECIS,        
	CLIP_DEFAULT_PRECIS,       
	DEFAULT_QUALITY,           
	DEFAULT_PITCH | FF_SWISS,  
	_T("Arial"));

	m_txtFontPB.CreateFont(txtFontPB,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
	ANSI_CHARSET,             
	OUT_DEFAULT_PRECIS,        
	CLIP_DEFAULT_PRECIS,       
	DEFAULT_QUALITY,           
	DEFAULT_PITCH | FF_SWISS,  
	_T("Arial"));

	m_bigFont.CreateFont(24,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
	ANSI_CHARSET,             
	OUT_DEFAULT_PRECIS,        
	CLIP_DEFAULT_PRECIS,       
	DEFAULT_QUALITY,           
	DEFAULT_PITCH | FF_SWISS,  
	_T("Arial"));

	m_vFuncInfo.clear();

	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
	m_sManualPath = moduleFileName;
	m_sManualPath += _T("algo_manual\\");
	int nLangID = ::gConfigMgr.GetLanguageID();
	if(nLangID == LANG_CHS)
	{
		m_sManualPath.Append(_T("CHS"));
	}
	if(nLangID == LANG_ENU)
	{
		m_sManualPath.Append(_T("ENU"));
	}
	m_oldPt.x = 0;
	m_oldPt.y = 0;
	
	if(chConfig.m_vecCameraSetting.size() != m_VecpProcessStation.size())
	{
		m_bMultiChannnelImg = true;
		m_nImgChannelCount  = chConfig.m_vecCameraSetting.size();
	}

	m_hcursor_fingers_hand = AfxGetApp()->LoadCursor(IDC_CURSOR_FINGER);
	m_hcursor_hand = AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);

	m_hCursor =LoadCursor(NULL, IDC_ARROW);
}

CChannelConfigDlg::~CChannelConfigDlg()
{
	if(!m_vParamNameCtrl.empty())
	{
		std::for_each(m_vParamNameCtrl.begin(), m_vParamNameCtrl.end(),[&](CWnd* pWnd)
		{
			delete pWnd;
			pWnd = nullptr;
		});
	}
	if(!m_vParamEditCtrl.empty())
	{
		std::for_each(m_vParamEditCtrl.begin(), m_vParamEditCtrl.end(),[&](CWnd* pWnd)
		{
			delete pWnd;
			pWnd = nullptr;
		});
	}

	if (m_pStaCameraStation)
	{
		delete m_pStaCameraStation;
		m_pStaCameraStation = NULL;
	}

	if (m_pComCameraStation)
	{
		delete m_pComCameraStation;
		m_pComCameraStation = NULL;
	}
}

void CChannelConfigDlg::SetChannelControlPanelPtr(CChannelControlPanel* ptr)
{
	m_pChannelControlPanel = ptr;
}

void CChannelConfigDlg::SetCurCameraIndex(int index)
{
	size_t processCount = m_VecpProcessStation.size();

	if (index < processCount)
	{
	    m_CameraIndex = index;
		m_pProcessStation = m_VecpProcessStation[m_CameraIndex];
	}
}

BOOL CChannelConfigDlg::OnInitDialog()
{
	m_bSplicing = m_chConfig.m_bSplicing;
	m_bModelFiles  = m_chConfig.m_bModelfiles;

	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BMP_CH_DLG_BG);

	SetSelfLearningFlag();
	
	CreateChannelImageWindow(m_bSplicing);
	InitUiCtrl();

	m_transferStation.Init(L"SMVS-MODEL_TOOL");
	SetTimer(TIMER_ID_DLG_READY, 1000, NULL);
	return TRUE;
}
void CChannelConfigDlg::DisplayZoomDebugObject()
{
	set_part(m_nChImgWndID,m_rcZoonIn.top, 	m_rcZoonIn.left , m_rcZoonIn.bottom-1, m_rcZoonIn.right-1);

	m_pProcessStation->GetCrtDebugInterImageObject(m_crtInterImage);

	if (m_crtInterImage.Id() == H_EMPTY_REGION)
	{
		Hobject ZoomShowImage;
		m_pProcessStation->GetCrtDebugImageObject(ZoomShowImage);

		//添加双通道颠倒罐放大适配！
		Hobject img = ZoomShowImage;

		if(m_bMultiChannnelImg)
		{
			select_obj(ZoomShowImage,&img,m_nCrtImgChannel);
		}

		DealWithTransparentObjectForDisplay(img);
		disp_obj(img,m_nChImgWndID);	
	}
	else
	{
		DealWithTransparentObjectForDisplay(m_crtInterImage);
		disp_obj(m_crtInterImage,m_nChImgWndID);	
	}

	m_pProcessStation->GetCrtDebugImgTargetRegion(m_crtTargetRgn);
	DisplayErrImgTargetRegionOnChWnd(m_crtTargetRgn,m_nChImgWndID);

	m_pProcessStation->GetCrtDebugImgTargetRegionEx(m_crtTargetRgnEx);
	DisplayErrImgTargetRegionOnChWnd(m_crtTargetRgnEx,m_nChImgWndID,1);

	m_pProcessStation->GetCrtDebugImgErrorRegion(m_crtErrRgn);
	DisplayErrorRegionOnChWnd(m_crtErrRgn,m_nChImgWndID);
	

}

BOOL CChannelConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;

	if(pMsg->message == WM_KEYDOWN && param == VK_LEFT)
	{
		OnPreviousClicked();
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && param == VK_RIGHT)
	{
		OnNextClicked();
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && param == VK_UP)
	{
		OnFirstClicked();
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && param == VK_DOWN)
	{
		OnLastClicked();
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && param == VK_SPACE)
	{
		if (!m_bSpaceButtonDown)
		{
			m_bSpaceButtonDown= true;
			if (m_fZoomInRatioTotal> 1.0f)
			{
				DisplayZoomDebugObject();
			}
			else
			{
				DisplayALLImageOnChWnd();
			}
		}
		return TRUE;
	}
	if(pMsg->message == WM_KEYUP && param == VK_SPACE)
	{
		if(m_bSpaceButtonDown)
		{
			if (m_fZoomInRatioTotal> 1.0f)
			{
				DisplayZoomDebugObject();
			}
			else
			{
				DisplayALLImageOnChWnd();
			}

		}
		m_bSpaceButtonDown = false;
		return TRUE;
	}

	if(pMsg->message == WM_LBUTTONDOWN && !m_bDecBtnHold && !m_bIncBtnHold)
	{
		if(m_rcDecreaseBtn.PtInRect(pMsg->pt))
		{
			m_bDecBtnHold = true;
		}
		if(m_rcIncreaseBtn.PtInRect(pMsg->pt))
		{
			m_bIncBtnHold = true;
		}
		if(m_bDecBtnHold || m_bIncBtnHold)
			SetTimer(TIMER_ID_INCREASE_DECREASE,250, NULL);
	}
	if(pMsg->message == WM_LBUTTONUP)
	{
		m_bDecBtnHold = false;
		m_bIncBtnHold = false;
		KillTimer(TIMER_ID_INCREASE_DECREASE);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CChannelConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALGO, m_algoList);
	DDX_Control(pDX, IDC_STATIC_PARAM_GP, m_paramBoard);
	DDX_Control(pDX, IDC_STATIC_CH_CONFIG, m_paramBoard);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApplyParam);
	DDX_Radio(pDX, IDC_RADIO_CH1, m_radioIndex);
	DDX_Control(pDX, IDC_STATIC_HELP_STRING, m_algoHelpStrPanel);
	DDX_Control(pDX, IDC_BTN_SAVE_TRAINNING_RESULT, m_btnSaveTR);
	DDX_Check(pDX, IDC_CHECK_READ_FILE, m_bReadImg);
	DDX_Check(pDX, IDC_CHECK_DISP_ERROR_RGN, m_bDispErrRgn);
	DDX_Control(pDX, IDC_STATIC_PARAM_HELP_STRING, m_paramHelpStrPanel);
	DDX_Control(pDX, IDC_STATIC_LC_PORT, m_staticLcPort);
	DDX_Control(pDX, IDC_EDIT_LC_PORT, m_editLcPort);
	DDX_Control(pDX, IDC_STATIC_LC_OPTION, m_staticIDs);
	DDX_Control(pDX, IDC_COMBO_LC_OPTION, m_comboIDs);
	DDX_Control(pDX, IDC_STATIC_LC_MODE, m_staticMode);
	DDX_Control(pDX, IDC_COMBO_LC_MODE, m_comboMode);
	// Line 1
	DDX_Control(pDX, IDC_STATIC_TAG1, m_staticTag1);
	DDX_Control(pDX, IDC_EDIT_TAG1, m_editTag1);
	DDX_Control(pDX, IDC_STATIC_LUM1, m_staticLum1);
	DDX_Control(pDX, IDC_SLIDER_LUM1, m_sliderLum1);
	DDX_Control(pDX, IDC_STATIC_VALUE_LUM1, m_staticValueLum1);
	DDX_Control(pDX, IDC_STATIC_DELAY1, m_staticDelay);
	DDX_Control(pDX, IDC_SLIDER_DELAY, m_sliderDelay);
	DDX_Control(pDX, IDC_STATIC_VALUE_DELAY, m_staticValueDelay);
	DDX_Control(pDX, IDC_STATIC_DURATION, m_staticDur);
	DDX_Control(pDX, IDC_SLIDER_DURATION, m_sliderDur);
	DDX_Control(pDX, IDC_STATIC_VALUE_DURATION, m_staticValueDur);

	//add by zhujilong 
	DDX_Control(pDX, IDC_STATIC_DELAY2, m_staticDelay2);
	DDX_Control(pDX, IDC_SLIDER_DELAY2, m_sliderDelay2);
	DDX_Control(pDX, IDC_STATIC_VALUE_DELAY2, m_staticValueDelay2);
	DDX_Control(pDX, IDC_STATIC_DURATION2, m_staticDur2);
	DDX_Control(pDX, IDC_SLIDER_DURATION2, m_sliderDur2);
	DDX_Control(pDX, IDC_STATIC_VALUE_DURATION2, m_staticValueDur2);

	DDX_Control(pDX, IDC_STATIC_DELAY3, m_staticDelay3);
	DDX_Control(pDX, IDC_SLIDER_DELAY3, m_sliderDelay3);
	DDX_Control(pDX, IDC_STATIC_VALUE_DELAY3, m_staticValueDelay3);
	DDX_Control(pDX, IDC_STATIC_DURATION3, m_staticDur3);
	DDX_Control(pDX, IDC_SLIDER_DURATION3, m_sliderDur3);
	DDX_Control(pDX, IDC_STATIC_VALUE_DURATION3, m_staticValueDur3);

	DDX_Control(pDX, IDC_STATIC_DELAY4, m_staticDelay4);
	DDX_Control(pDX, IDC_SLIDER_DELAY4, m_sliderDelay4);
	DDX_Control(pDX, IDC_STATIC_VALUE_DELAY4, m_staticValueDelay4);
	DDX_Control(pDX, IDC_STATIC_DURATION4, m_staticDur4);
	DDX_Control(pDX, IDC_SLIDER_DURATION4, m_sliderDur4);
	DDX_Control(pDX, IDC_STATIC_VALUE_DURATION4, m_staticValueDur4);

	//end
	// Line 2
	DDX_Control(pDX, IDC_STATIC_TAG2, m_staticTag2);
	DDX_Control(pDX, IDC_EDIT_TAG2, m_editTag2);
	DDX_Control(pDX, IDC_STATIC_LUM2, m_staticLum2);
	DDX_Control(pDX, IDC_SLIDER_LUM2, m_sliderLum2);
	DDX_Control(pDX, IDC_STATIC_VALUE_LUM2, m_staticValueLum2);
	// Line 3
	DDX_Control(pDX, IDC_STATIC_TAG3, m_staticTag3);
	DDX_Control(pDX, IDC_EDIT_TAG3, m_editTag3);
	DDX_Control(pDX, IDC_STATIC_LUM3, m_staticLum3);
	DDX_Control(pDX, IDC_SLIDER_LUM3, m_sliderLum3);
	DDX_Control(pDX, IDC_STATIC_VALUE_LUM3, m_staticValueLum3);
	// Line 4
	DDX_Control(pDX, IDC_STATIC_TAG4, m_staticTag4);
	DDX_Control(pDX, IDC_EDIT_TAG4, m_editTag4);
	DDX_Control(pDX, IDC_STATIC_LUM4, m_staticLum4);
	DDX_Control(pDX, IDC_SLIDER_LUM4, m_sliderLum4);
	DDX_Control(pDX, IDC_STATIC_VALUE_LUM4, m_staticValueLum4);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD, m_editThreshold);
	DDX_Control(pDX, IDC_CHECK_LIGHT_SINGLEMODE, m_bCheckSingleMode);
	DDX_Control(pDX, IDC_CHECK_TASK_Param, m_bCheckTaskAttach);
}

BEGIN_MESSAGE_MAP(CChannelConfigDlg, CDialogEx)
	ON_STN_CLICKED(IDB_CH_DLG_CLOSE, &CChannelConfigDlg::OnCloseScreen)
	ON_STN_CLICKED(IDB_CAMERA,		 &CChannelConfigDlg::OnCameraClicked)
	ON_STN_CLICKED(IDB_LIGHT,		 &CChannelConfigDlg::OnLightClicked)
	ON_STN_CLICKED(IDB_REJECTOR,     &CChannelConfigDlg::OnRejectorClicked)
	ON_STN_CLICKED(IDB_DEVICE_INFO,	 &CChannelConfigDlg::OnDeviceInfoClicked)
	ON_STN_CLICKED(IDB_IMG_SOURCE,   &CChannelConfigDlg::OnImgSourceClicked)
	ON_STN_CLICKED(IDB_FIRST,        &CChannelConfigDlg::OnFirstClicked)
	ON_STN_CLICKED(IDB_PREVIOUS,     &CChannelConfigDlg::OnPreviousClicked)
    ON_STN_CLICKED(IDB_NEXT,		 &CChannelConfigDlg::OnNextClicked)
	ON_STN_CLICKED(IDB_LAST,		 &CChannelConfigDlg::OnLastClicked)
	ON_STN_CLICKED(IDB_IMG_DISP_MODE,&CChannelConfigDlg::OnImgDispModeClicked)
	ON_STN_CLICKED(IDB_LOCK,		 &CChannelConfigDlg::OnLockClicked)
//	ON_STN_CLICKED(IDB_ZOOM_IN,		 &CChannelConfigDlg::OnZoomInClicked)
	ON_STN_CLICKED(IDB_SAVE_IMG,     &CChannelConfigDlg::OnSaveImgClicked)
	ON_STN_CLICKED(IDB_SAVE_ALL_IMG, &CChannelConfigDlg::OnSaveAllImgClicked)
	ON_STN_CLICKED(IDB_DELETE_CRT_IMG,&CChannelConfigDlg::OnDeleteModelImgClicked)
	ON_STN_CLICKED(IDB_ADD_MODEL_IMG, &CChannelConfigDlg::OnAddModelImgClicked)
	ON_STN_CLICKED(IDB_RESET_STATS_DATA, &CChannelConfigDlg::OnResetStatsDataClicked)
	ON_STN_CLICKED(IDB_ON_OFF,&CChannelConfigDlg::OnOnOffButtonClicked)
	ON_STN_CLICKED(IDB_EDIT_FILE0, &CChannelConfigDlg::OnStnClickedStaticEditFile0)
	ON_STN_CLICKED(IDB_EDIT_FILE1, &CChannelConfigDlg::OnStnClickedStaticEditFile1)
	ON_STN_CLICKED(IDB_EDIT_FILE2, &CChannelConfigDlg::OnStnClickedStaticEditFile2)
	ON_STN_CLICKED(IDB_EDIT_FILE3, &CChannelConfigDlg::OnStnClickedStaticEditFile3)
	ON_STN_CLICKED(IDB_EDIT_FILE4, &CChannelConfigDlg::OnStnClickedStaticEditFile4)
	ON_STN_CLICKED(IDB_EDIT_FILE5, &CChannelConfigDlg::OnStnClickedStaticEditFile5)
	ON_STN_CLICKED(IDB_EDIT_FILE6, &CChannelConfigDlg::OnStnClickedStaticEditFile6)
	ON_STN_CLICKED(IDB_EDIT_FILE7, &CChannelConfigDlg::OnStnClickedStaticEditFile7)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALGO, &CChannelConfigDlg::OnNMClickListAlgo)
	ON_BN_CLICKED(IDOK, &CChannelConfigDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_CAMERA_RUN_MODE, &CChannelConfigDlg::OnCbnSelchangeRunMode)
	ON_CBN_SELCHANGE(IDC_CAMERA_STATION, &CChannelConfigDlg::OnCbnSelchangeCameraStation)
	ON_CBN_SELCHANGE(IDC_EDGE_CTRL, &CChannelConfigDlg::OnCbnSelchangeEdgeCtrl)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CChannelConfigDlg::OnBnClickedBtnApplyParam)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_CH1, &CChannelConfigDlg::OnBnClickedRadioCh1)
	ON_BN_CLICKED(IDC_RADIO_CH2, &CChannelConfigDlg::OnBnClickedRadioCh2)
	ON_BN_CLICKED(IDC_RADIO_CH3, &CChannelConfigDlg::OnBnClickedRadioCh3)
	ON_BN_CLICKED(IDC_RADIO_CH4, &CChannelConfigDlg::OnBnClickedRadioCh4)
	ON_BN_CLICKED(IDC_RADIO_CH5, &CChannelConfigDlg::OnBnClickedRadioCh5)
	ON_BN_CLICKED(IDC_RADIO_CH6, &CChannelConfigDlg::OnBnClickedRadioCh6)
	ON_BN_CLICKED(IDC_RADIO_CH7, &CChannelConfigDlg::OnBnClickedRadioCh7)
	ON_BN_CLICKED(IDC_RADIO_CH8, &CChannelConfigDlg::OnBnClickedRadioCh8)
	ON_BN_CLICKED(IDC_BTN_SAVE_TRAINNING_RESULT, &CChannelConfigDlg::OnBnClickedBtnSaveTrainningResult)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_READ_FILE, &CChannelConfigDlg::OnBnClickedCheckReadImgFile)
	ON_MESSAGE(MSG_MODEL_IMG_READY, OnRetrieveModelImage)
	ON_MESSAGE(WM_UPDATE_ALGO_LIST,    OnUpdateChannelScreen)
	ON_MESSAGE(MSG_BAR_GRAPH_CHANGED,  OnThresholdChanged)
	ON_MESSAGE(MSG_BAR_GRAPH_RANGE_CHANGED,  OnControlLimitChanged)
	ON_MESSAGE(WM_RGN_FILE_UPDATED,    OnExternalFileUpdated)
	ON_BN_CLICKED(IDC_CHECK_DISP_ERROR_RGN, &CChannelConfigDlg::OnBnClickedCheckDispErrorRgn)
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_LC_OPTION, &CChannelConfigDlg::OnCbnSelchangeComboLcOption)
	ON_CBN_SELCHANGE(IDC_COMBO_LC_MODE, &CChannelConfigDlg::OnCbnSelchangeComboLcMode)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LUM1, &CChannelConfigDlg::OnNMReleasedcaptureSliderLum1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DELAY, &CChannelConfigDlg::OnNMReleasedcaptureSliderDelay)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DURATION, &CChannelConfigDlg::OnNMReleasedcaptureSliderDuration)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LUM2, &CChannelConfigDlg::OnNMReleasedcaptureSliderLum2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LUM3, &CChannelConfigDlg::OnNMReleasedcaptureSliderLum3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LUM4, &CChannelConfigDlg::OnNMReleasedcaptureSliderLum4)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_ALGO_HELP, &CChannelConfigDlg::OnNMClickSyslinkAlgoHelp)
	ON_COMMAND(ID_ADD_REGION, OnAddRegion)
	ON_COMMAND(ID_DELETE_REGION, OnDelRegion)
	ON_COMMAND(ID_ADD_TESTITEM, OnAddTestItem)
	ON_COMMAND(ID_DELETE_TEST_ITEM, OnDelTestItem)
	ON_COMMAND(ID_RENAME_TEST_ITEM,OnRenameTestItem)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALGO, &CChannelConfigDlg::OnNMRClickListAlgo)
	ON_STN_CLICKED(IDC_STATIC_BTN_DECREASE, &CChannelConfigDlg::OnStnClickedStaticBtnDecrease)
	ON_STN_CLICKED(IDC_STATIC_BTN_INCREASE, &CChannelConfigDlg::OnStnClickedStaticBtnIncrease)
	ON_EN_SETFOCUS(IDC_EDIT_PARAMETER, &CChannelConfigDlg::OnEnSetfocusEditParameter)
	ON_STN_CLICKED(IDB_LEFT_ARROW,	 &CChannelConfigDlg::OnDecreaseThresholdClicked)
	ON_STN_CLICKED(IDB_RIGHT_ARROW,	 &CChannelConfigDlg::OnIncreaseThresholdClicked)
	ON_EN_KILLFOCUS(IDC_EDIT_THRESHOLD, &CChannelConfigDlg::OnEnKillfocusEditThreshold)
	ON_STN_CLICKED(IDB_THRESHOLD_PANEL_LCL, &CChannelConfigDlg::OnStnClickedLCL)
	ON_STN_DBLCLK(IDB_THRESHOLD_PANEL_LCL, &CChannelConfigDlg::OnStnDblclickStaticThresholdPanel_LCL)
	ON_STN_CLICKED(IDB_THRESHOLD_PANEL_UCL, &CChannelConfigDlg::OnStnClickedUCL)
	ON_STN_DBLCLK(IDB_THRESHOLD_PANEL_UCL, &CChannelConfigDlg::OnStnDblclickStaticThresholdPanel_UCL)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ALGO, &CChannelConfigDlg::OnLvnItemchangedListAlgo)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_LIGHT_SINGLEMODE, &CChannelConfigDlg::OnBnClickedCheckLightSingleMode)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DELAY2, &CChannelConfigDlg::OnNMReleasedcaptureSliderDelay2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DURATION2, &CChannelConfigDlg::OnNMReleasedcaptureSliderDuration2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DELAY3, &CChannelConfigDlg::OnNMReleasedcaptureSliderDelay3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DURATION3, &CChannelConfigDlg::OnNMReleasedcaptureSliderDuration3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DELAY4, &CChannelConfigDlg::OnNMReleasedcaptureSliderDelay4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DURATION4, &CChannelConfigDlg::OnNMReleasedcaptureSliderDuration4)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_CHECK_TASK_Param, &CChannelConfigDlg::OnBnClickedCheckTaskParam)
	ON_BN_CLICKED(IDC_RADIO_CAMERA1, &CChannelConfigDlg::OnBnClickedRadioCamera1)
	ON_BN_CLICKED(IDC_RADIO_CAMERA2, &CChannelConfigDlg::OnBnClickedRadioCamera2)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CChannelConfigDlg::InitUiCtrl()
{
	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
	CRect rcClient;
	GetClientRect(&rcClient);
	// Caution: DO NOT change the order of below functions.
	InitScreenHeadPanel(rcWorkArea);
	InitPromptMessagePanel(rcWorkArea);
	InitButtons(rcWorkArea, rcClient);
	InitAlgoListBoard(rcWorkArea, rcClient);
	InitParameterBoard(rcWorkArea, rcClient);
	InitLightControllerWidget();
	InitApplyParamButton(rcWorkArea);
	InitSaveTrainningResultButton(rcWorkArea);
	InitSpcChart(rcWorkArea);
	InitChannelSwitch(rcWorkArea);
	InitCameraSwitch(rcWorkArea);

	int x_sz = rcWorkArea.Width();  
	int y_sz = rcWorkArea.Height();
	::SetWindowPos(m_hWnd,HWND_TOP,0,0,x_sz, y_sz, SWP_SHOWWINDOW);
	SetButtonVisibility();
}

void CChannelConfigDlg::CreateChannelImageWindow(bool bSplicing)
{
	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate);
	int vtBtnSz = 64; // Vertical Button Size
	int hzBtnSz = 64; //  Horizontal Button Size;
	int gapH = 5;
	int gapV = 10;

	CRect rcWnd;
	long col = rcWorkArea.left + gapH + hzBtnSz + 5;
	long row = rcWorkArea.top + titleAreaH + vtBtnSz + 8;
 	int width  = m_chConfig.m_uiSetting.m_width;
 	int height = m_chConfig.m_uiSetting.m_height;

	size_t nProcessSize = m_VecpProcessStation.size();

	if (!bSplicing)
	{
		m_ScreenOp.SetCurRect(nProcessSize,width,height);
	}
	else
	{
		m_ScreenOp.SetCurRect(1,width,height);
	}

	m_rcImgWnd = m_ScreenOp.GetPosRect();
 	m_OldrcImgWnd = m_rcImgWnd;

	for (int i = 0 ; i < MAX_CONFIG_IMG_WND_COUNT ;i++)
	{
		long long CHImgWndID;
		std::string mode("visible");
		std::string strMachine("");
		set_check("~father");
		CRect rect;
		if(i < nProcessSize)
		{
			if (bSplicing && i != 0)
			{
				rect = CRect(0,0,1,1);
			}
			else
			{
				m_ScreenOp.GetIndexRect(i,rect);
			}
		}
		else
		{
			rect = CRect(0,0,1,1);
		}

		open_window(rect.top, rect.left,rect.Width(), rect.Height(),(Hlong)m_hWnd, mode.c_str(),strMachine.c_str(), (Hlong*)&CHImgWndID);
		if(CHImgWndID == -1)
		{
			return;
		}
		set_part(CHImgWndID,0, 0, height - 1, width - 1);
		set_check("father");
		set_draw(CHImgWndID,"margin"); //margin or fill
		set_color(CHImgWndID, "red");
		set_line_width(CHImgWndID,HTuple(1));

		HWND ChimgWnd;
		long disp;
		get_os_window_handle((Hlong)CHImgWndID, (Hlong*)&ChimgWnd, (Hlong*)&disp);
		CWnd* pImgWnd = FromHandle(ChimgWnd);

		m_VecCHImgWndID.push_back(CHImgWndID);
		m_VecCHImgWnd.push_back(ChimgWnd);
	}
	m_iCameraIndex = 0;
	m_nChImgWndID = m_VecCHImgWndID[m_iCameraIndex];
	m_hChImgWnd= m_VecCHImgWnd[m_iCameraIndex];
}

void CChannelConfigDlg::DisPlayModelImage(int StationIndex,Hobject crtImg,Hobject retRgn)
{
	if (StationIndex < m_VecpProcessStation.size())
	{
		//can 显示自学习结果扩展
		Hobject img = crtImg;

		Hlong nModelCount;
		count_obj(img, &nModelCount);

		if(m_bMultiChannnelImg)
		{
			if (m_nCrtImgChannel<=nModelCount)
			{
				if (img.Id()!=H_EMPTY_REGION)
				{
					select_obj(crtImg,&img,m_nCrtImgChannel);
				}
			}
		}
		DisplayImageOnChWnd(img,m_VecCHImgWndID[StationIndex]);


		set_color(m_VecCHImgWndID[StationIndex], "green");
		count_obj(retRgn, &nModelCount);
		Hobject TargetRgn= retRgn;

		//can 显示
		if(m_bMultiChannnelImg)
		{
			if (m_nCrtImgChannel<=nModelCount)
			{
				if (TargetRgn.Id()!=H_EMPTY_REGION)
				{
					select_obj(retRgn,&TargetRgn,m_nCrtImgChannel);
				}
			}
		}
		disp_obj(TargetRgn, m_VecCHImgWndID[StationIndex]);

	}
}

void CChannelConfigDlg::DisplayFirstModelImage()
{
	m_vecModelIndex.clear();
	m_vecModelCount.clear();
	m_btnSaveTR.EnableWindow(false);

	int m_CountMax = 0;
	for (size_t i = 0 ; i < m_VecpProcessStation.size(); i++)
	{
		m_pProcessStation = m_VecpProcessStation[i];
		bool bModelImageReady = m_VecpProcessStation[i]->IsModelImageReady();
		if(!bModelImageReady)
			return;
		m_nImgCount = m_VecpProcessStation[i]->GetModelImageCount();

		if (m_CountMax < m_nImgCount )
		{
			m_CountMax = m_nImgCount;
		}

		m_vecModelCount.push_back(m_nImgCount);

		m_crtImgIndex = 0;
		m_vecModelIndex.push_back(m_crtImgIndex);

		bool bEmpty = (m_nImgCount == 0);
		m_btnSaveTR.EnableWindow(!bEmpty);
		if(bEmpty)
		{
			continue;
		}

		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			wstring wstrParam = GetSelfLearningParam();
			m_VecpProcessStation[i]->DisplayModelImage(m_crtImgIndex, wstrParam);
		}
	}

	m_pProcessStation = m_VecpProcessStation[0];
	m_nImgCount = m_CountMax;

}

void CChannelConfigDlg::InitScreenHeadPanel(CRect& rcWorkArea)
{
	CRect rc;
	int width = rcWorkArea.Width();
	rc.left = rcWorkArea.left + (int)(width / 9.5);
	rc.top = rcWorkArea.top + 25;
	rc.right = rc.left + 196;
	rc.bottom = rc.top + 30;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_CH_RUN_MODE);
	pWnd->MoveWindow(rc);
	pWnd->SetFont(&m_txtFont);

	CString str;
	if(m_bSelfLearningMode)
	{
		str.LoadString(IDS_STRING_CH_RUN_MODE_TRAINNING);
	}
	else
	{
		str.LoadString(IDS_STRING_CH_RUN_MODE_TESTING);
	}
	pWnd->SetWindowText(str);
	RefreshUiCtrl(pWnd);
}

void CChannelConfigDlg::InitPromptMessagePanel(CRect& rcWorkArea)
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_WARNING_PANEL);
	pWnd->SetFont(&m_txtFont);
	CRect rcWnd;
	pWnd->GetWindowRect(rcWnd);

	int titleAreaH = int(TILTE_HEIGHT * rcWorkArea.Height() / 900.0f);
	CRect rc;
	int gapH = 5;
	rc.left = rcWorkArea.right - 450;
	rc.top = int((titleAreaH - rcWnd.Height()) / 2.0);
	rc.right = rcWorkArea.right - 100;
	rc.bottom = rc.top + rcWnd.Height();
	pWnd->MoveWindow(rc);
}

void CChannelConfigDlg::InitButtons(CRect& rcWorkArea, CRect& rcClient)
{
	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate);
	int vtBtnSz = 64; // Vertical Button Size
	int hzBtnSz = 64; //  Horizontal Button Size;
	int gapH = 2;
	int gapV = 5;
	// Close button
	CRect rcClose;
	rcClose.left = rcWorkArea.right - 2* gapH - 48;
	rcClose.top = int((titleAreaH - 48) / 2.0) - 5;
	rcClose.right = rcClose.left + 48;
	rcClose.bottom = rcClose.top + 48;
	m_btnClose.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcClose, this, IDB_CH_DLG_CLOSE);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();

	CRect rcBtn;
	rcBtn.left  = rcClient.left + gapH;
	rcBtn.top   = rcClient.top + titleAreaH + vtBtnSz + 8;
	rcBtn.right = rcBtn.left + vtBtnSz;
	rcBtn.bottom = rcBtn.top + vtBtnSz;
	m_btnOnOff.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_ON_OFF);
	bool bChCameraRun = ::gSystemMgr.IsChannelCameraRunning(m_chConfig.m_chIndex);
	UAC uac = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (uac.m_nLevel == UAC_ADMIN);
	if(!bChCameraRun)
	{
		if(!m_bSelfLearningMode)
		{
			EnableUiBtn(IDB_ON_OFF,true,IMG_CH_CAMERA_PAUSE);
		}
		else
		{
			EnableUiBtn(IDB_ON_OFF,false,IMG_CH_CAMERA_PAUSE_GRAY);
		}
	}
	else
	{
		if(bAdmin)
			EnableUiBtn(IDB_ON_OFF,true,IMG_CH_CAMERA_RUN);
		else
			EnableUiBtn(IDB_ON_OFF,false,IMG_CH_CAMERA_RUN_GRAY);
	}
	m_btnOnOff.Invalidate();

	// Device Information
	rcBtn.top = rcBtn.bottom + gapV;
	rcBtn.bottom = rcBtn.top + vtBtnSz;
	m_btnDeviceInfo.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_DEVICE_INFO);
	m_btnDeviceInfo.SetLabelImage(IMG_CH_DEVICE_INFO);
	m_btnDeviceInfo.Invalidate();
	//
	rcBtn.top = rcBtn.bottom + gapV;
	rcBtn.bottom = rcBtn.top + vtBtnSz;
	m_btnCamera.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this,IDB_CAMERA);
	m_btnCamera.SetLabelImage(IMG_CH_CAMERA);
	m_btnCamera.Invalidate();

	rcBtn.top = rcBtn.bottom + gapV;
	rcBtn.bottom = rcBtn.top + vtBtnSz;
	m_btnLight.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_LIGHT);
	m_btnLight.SetLabelImage(IMG_CH_LIGHT);
	m_btnLight.Invalidate();

	rcBtn.top = rcBtn.bottom + gapV;
	rcBtn.bottom = rcBtn.top + vtBtnSz;
	m_btnRejector.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_REJECTOR);
	m_btnRejector.SetLabelImage(IMG_CH_REJECTOR);
	m_btnRejector.Invalidate();

	UAC crtUac = gSystemMgr.GetCurrentUser();
	if(!bAdmin)
	{
		m_btnCamera.ShowWindow(FALSE);
		m_btnLight.ShowWindow(FALSE);
		m_btnRejector.ShowWindow(FALSE);
	}

	CRect rcCheck;
	rcCheck.left = 10;
	rcCheck.top = rcClient.top + titleAreaH + 20;
	rcCheck.right = rcCheck.left + 20;
	rcCheck.bottom = rcCheck.top + 25;
	CWnd* pWnd = GetDlgItem(IDC_CHECK_READ_FILE);
	pWnd->SetFont(&m_txtFont);
	pWnd->MoveWindow(rcCheck);

	rcCheck.left = rcCheck.right;
	rcCheck.top = rcCheck.top + 5;
	rcCheck.right = rcCheck.left + 50;
	rcCheck.bottom = rcCheck.top+35;
	pWnd = GetDlgItem(IDC_STATIC_READ_IMG);
	pWnd->SetFont(&m_txtFont);
	pWnd->MoveWindow(rcCheck);
	CString strTxt;
	strTxt.LoadString(IDS_STRING_DEGUG_IMAGE);
	pWnd->SetWindowText(strTxt);

	gapH = 3;
	rcBtn.left = 40 + vtBtnSz + gapH;
	rcBtn.top = rcClient.top + titleAreaH + 3;
	rcBtn.right = rcBtn.left + hzBtnSz;
	rcBtn.bottom = rcBtn.top + hzBtnSz;
	m_btnImgSource.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_IMG_SOURCE);
	m_btnImgSource.EnableWindow(FALSE);
	m_btnImgSource.SetLabelImage(IMG_CH_IMG_SOURCE_GRAY);
	m_btnImgSource.Invalidate();

	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnFirst.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_FIRST);
	m_btnFirst.SetLabelImage(IMG_CH_FIRST);
	m_btnFirst.Invalidate();

	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnPrevious.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_PREVIOUS);
	m_btnPrevious.SetLabelImage(IMG_CH_PREVIOUS);
	m_btnPrevious.Invalidate();

	
	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz + 50;
	CRect rcIndex;
	rcIndex.left = rcBtn.left;
	rcIndex.top = rcBtn.top + gapH;
	rcIndex.right = rcBtn.right;
	rcIndex.bottom = rcBtn.bottom - gapH;
	m_lblIndex.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rcIndex, this, NULL);
	m_lblIndex.SetTxtFont(27,FW_BOLD,CENTER);
	m_lblIndex.SetColor(m_fgColor, m_bgColor);
	m_lblIndex.Invalidate(); 

	rcBtn.left = rcBtn.right + gapH + 1;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnNext.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_NEXT);
	m_btnNext.SetLabelImage(IMG_CH_NEXT);
	m_btnNext.Invalidate();

	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnLast.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_LAST);
	m_btnLast.SetLabelImage(IMG_CH_LAST);
	m_btnLast.Invalidate();

	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnLock.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_LOCK);
	m_btnLock.SetLabelImage(IMG_CH_UNLOCKED);
	m_btnLock.Invalidate();

	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnDispMode.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_IMG_DISP_MODE);
	m_btnDispMode.SetLabelImage(IMG_CH_SINGLE_MODE);
	m_btnDispMode.Invalidate();


// 	rcBtn.left = rcBtn.right + gapH;
// 	rcBtn.right = rcBtn.left + hzBtnSz;
// 	m_btnZoomIn.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_ZOOM_IN);
// 	m_btnZoomIn.SetLabelImage(IMG_CH_ZOOM_IN_GRAY);
// 	m_btnZoomIn.EnableWindow(FALSE);
// 	m_btnZoomIn.Invalidate();

	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnSaveImg.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_SAVE_IMG);
	m_btnSaveImg.SetLabelImage(IMG_CH_SAVE_IMG_GRAY);
	m_btnSaveImg.EnableWindow(FALSE);
	m_btnSaveImg.Invalidate();

	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnSaveAllImg.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_SAVE_ALL_IMG);
	m_btnSaveAllImg.SetLabelImage(IMG_CH_SAVE_ALL_IMG_GRAY);
	m_btnSaveAllImg.EnableWindow(FALSE);
	m_btnSaveAllImg.Invalidate();

	m_SelfrcBtn = rcBtn;  //用于通道切换控件位置重置

	if (m_bSelfLearningFunc)
	{
		rcBtn.left = rcBtn.right + gapH;
		rcBtn.right = rcBtn.left + hzBtnSz;
		m_btnAddModelImg.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_ADD_MODEL_IMG);
		m_btnAddModelImg.SetLabelImage(IMG_CH_ADD_MODEL_IMG_GRAY);
		m_btnAddModelImg.EnableWindow(FALSE);
		m_btnAddModelImg.Invalidate();

		m_btnDelCrtImg.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_DELETE_CRT_IMG);
		m_btnDelCrtImg.SetLabelImage(IMG_CH_DELETE_MODEL_IMG_GRAY);
		m_btnDelCrtImg.EnableWindow(FALSE);
		m_btnDelCrtImg.Invalidate();

		if(m_bSelfLearningMode)
		{
			m_btnAddModelImg.ShowWindow(SW_HIDE);
			EnableAllButtons(false);
			EnableUiBtn(IDB_ADD_MODEL_IMG, true, IMG_CH_ADD_MODEL_IMG);
//			EnableUiBtn(IDB_ZOOM_IN, true, IMG_CH_ZOOM_IN);
		}
		else
		{
			m_btnDelCrtImg.ShowWindow(SW_HIDE);
		}
	}
	

	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnResetStatsData.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_RESET_STATS_DATA);
	if(bChCameraRun)
	{
		m_btnResetStatsData.SetLabelImage(IMG_CH_RESET_STATS_DATA_GRAY);
	}
	else
	{
		m_btnResetStatsData.SetLabelImage(IMG_CH_RESET_STATS_DATA);
	}
	m_btnResetStatsData.EnableWindow(TRUE);
	m_btnResetStatsData.Invalidate();
}

void CChannelConfigDlg::InitUiWidget(int nCtrlID, int strID)
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

void CChannelConfigDlg::InitUiWidget(int nCtrlID, CString str)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	if(pWnd != nullptr)
	{
		pWnd->SetFont(&m_txtFont);
		pWnd->SetWindowText(str);
		pWnd->Invalidate();
	}
}

void CChannelConfigDlg::InitLightControllerWidget()
{
	m_vLcWidgetPtr.clear();
	m_vLcTagPtr.clear();
	m_vLcSliderPtr.clear();
	m_vLcStaticPtr.clear();
	m_dictSlider_Value.clear();

	CRect rc;
	m_paramBoard.GetWindowRect(rc);

	int i = 0;
	CRect rcName, rcEdit;

	CreateUiRect(rc,i++,rcName,rcEdit);
	InitUiWidget(IDC_STATIC_LC_PORT, IDS_STRING_SERIAL_PORT);
	InitUiWidget(IDC_EDIT_LC_PORT);
	m_staticLcPort.MoveWindow(rcName);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLcPort);
	m_editLcPort.MoveWindow(rcEdit);
	m_vLcSliderPtr.push_back((CWnd*)&m_editLcPort);

	CreateUiRect(rc,i++,rcName,rcEdit);
	InitUiWidget(IDC_STATIC_LC_OPTION,IDS_STRING_LC_OPTION);
	InitUiWidget(IDC_COMBO_LC_OPTION);
	m_staticIDs.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticIDs);
	m_comboIDs.MoveWindow(rcEdit);   m_vLcWidgetPtr.push_back((CWnd*)&m_comboIDs);

	CreateUiRect(rc,i++,rcName,rcEdit);
	InitUiWidget(IDC_STATIC_LC_MODE,IDS_STRING_LIGHT_CONTROLLER_MODE);
	InitUiWidget(IDC_COMBO_LC_MODE);
	m_staticMode.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticMode);
	m_comboMode.MoveWindow(rcEdit);   m_vLcWidgetPtr.push_back((CWnd*)&m_comboMode);

	CRect rcValue;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcName.top+=10;
	m_bCheckSingleMode.MoveWindow(rcName);
	m_vLcWidgetPtr.push_back((CWnd*)&m_bCheckSingleMode);
	
	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DELAY1,IDS_STRING_LIGHT_CONTROLLER_DELAY);
	InitUiWidget(IDC_SLIDER_DELAY);
	InitUiWidget(IDC_STATIC_VALUE_DELAY); 
	m_staticDelay.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDelay);
	m_sliderDelay.MoveWindow(rcEdit); 
	m_sliderDelay.SetRange(0,2000);
	m_sliderDelay.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDelay);
	m_staticValueDelay.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDelay);
	m_dictSlider_Value[(CWnd*)&m_sliderDelay] = &m_staticValueDelay;

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DURATION,IDS_STRING_LIGHT_CONTROLLER_DURATION);
	InitUiWidget(IDC_SLIDER_DURATION);
	InitUiWidget(IDC_STATIC_VALUE_DURATION); 
	m_staticDur.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDur);
	m_sliderDur.MoveWindow(rcEdit);
	m_sliderDur.SetRange(1,200);
	m_sliderDur.SetPos(1);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDur);
	m_staticValueDur.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDur);
	m_dictSlider_Value[(CWnd*)&m_sliderDur] = &m_staticValueDur;

	int nDelta = 7;
	// Line #1
	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcEdit.left  += nDelta;
	rcEdit.right -= nDelta;
	InitUiWidget(IDC_STATIC_TAG1,IDS_STRING_LC_LINE_NOTE);
	InitUiWidget(IDC_EDIT_TAG1); 
	m_staticTag1.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticTag1);
	m_editTag1.MoveWindow(rcEdit);    m_vLcTagPtr.push_back((CWnd*)&m_editTag1);

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_LUM1,IDS_STRING_LUMINANCE);
	InitUiWidget(IDC_SLIDER_LUM1);
	InitUiWidget(IDC_STATIC_VALUE_LUM1); 
	m_staticLum1.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLum1);
	m_staticLum1.BringWindowToTop();
	m_sliderLum1.MoveWindow(rcEdit); 
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderLum1);
	m_sliderLum1.SetRange(0,255); m_sliderLum2.SetPos(0);
	m_staticValueLum1.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueLum1);
	m_dictSlider_Value[(CWnd*)&m_sliderLum1] = &m_staticValueLum1;

	// Line #2
	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcEdit.left  += nDelta;
	rcEdit.right -= nDelta;
	InitUiWidget(IDC_STATIC_TAG2,IDS_STRING_LC_LINE_NOTE);
	InitUiWidget(IDC_EDIT_TAG2); 
	m_staticTag2.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticTag2);
	m_editTag2.MoveWindow(rcEdit);    m_vLcTagPtr.push_back((CWnd*)&m_editTag2);

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_LUM2,IDS_STRING_LUMINANCE);
	InitUiWidget(IDC_SLIDER_LUM2);
	InitUiWidget(IDC_STATIC_VALUE_LUM2); 
	m_staticLum2.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLum2);
	m_sliderLum2.MoveWindow(rcEdit); 
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderLum2);
	m_sliderLum2.SetRange(0,255);
	m_sliderLum2.SetPos(0);
	m_staticValueLum2.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueLum2);
	m_dictSlider_Value[(CWnd*)&m_sliderLum2] = &m_staticValueLum2;
	// Line #3
	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcEdit.left  += nDelta;
	rcEdit.right -= nDelta;
	InitUiWidget(IDC_STATIC_TAG3,IDS_STRING_LC_LINE_NOTE);
	InitUiWidget(IDC_EDIT_TAG3); 
	m_staticTag3.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticTag3);
	m_editTag3.MoveWindow(rcEdit);    m_vLcTagPtr.push_back((CWnd*)&m_editTag3);

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_LUM3,IDS_STRING_LUMINANCE);
	InitUiWidget(IDC_SLIDER_LUM3);
	InitUiWidget(IDC_STATIC_VALUE_LUM3); 
	m_staticLum3.MoveWindow(rcName);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLum3);
	m_sliderLum3.MoveWindow(rcEdit);
	m_sliderLum3.SetRange(0,255);
	m_sliderLum3.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderLum3);
	m_staticValueLum3.MoveWindow(rcValue); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueLum3);
	m_dictSlider_Value[(CWnd*)&m_sliderLum3] = &m_staticValueLum3;

	// Line #4
	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcEdit.left  += nDelta;
	rcEdit.right -= nDelta;
	InitUiWidget(IDC_STATIC_TAG4,IDS_STRING_LC_LINE_NOTE);
	InitUiWidget(IDC_EDIT_TAG4); 
	m_staticTag4.MoveWindow(rcName);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticTag4);
	m_editTag4.MoveWindow(rcEdit);
	m_vLcTagPtr.push_back((CWnd*)&m_editTag4);

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_LUM4,IDS_STRING_LUMINANCE);
	InitUiWidget(IDC_SLIDER_LUM4);
	InitUiWidget(IDC_STATIC_VALUE_LUM4); 
	m_staticLum4.MoveWindow(rcName);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLum4);
	m_sliderLum4.MoveWindow(rcEdit); 
	m_sliderLum4.SetRange(0,255);
	m_sliderLum4.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderLum4);
	m_staticValueLum4.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueLum4);
	m_dictSlider_Value[(CWnd*)&m_sliderLum4] = &m_staticValueLum4;
}

void CChannelConfigDlg::FillLightControllerWidget()
{
	LIGHT_SETTING& ls = m_chConfig.m_lightCtrlSetting;
	TASK crtTask;
	if(!m_chConfig.GetCurrentTask(crtTask))
	{
		return;
	}

	CString sValue;
	sValue.Format(_T("COM%d"), gSystemMgr.m_nSerialPort);
	m_editLcPort.SetWindowText(sValue);
	m_editLcPort.EnableWindow(FALSE);

	TASK* pCrtTask = m_chConfig.GetCurrentTask();
    wstring wstrLcGpName = pCrtTask->m_wstrTaskName;

	m_comboIDs.ResetContent();
	for(size_t t = 0; t < ls.m_vLcID.size(); t++)
	{
		sValue.Format(_T("%d"), ls.m_vLcID[t]);
		m_comboIDs.AddString(sValue);
	}
	m_comboIDs.SetCurSel(0);
	// Mode combo box
	m_comboMode.ResetContent();

	if(!ls.m_vLcID.empty())
	{
		m_nCrtLcID = ls.m_vLcID[0];
		wstring wstrTag = L"";
		if(!ls.m_vTags.empty())
			wstrTag = ls.m_vTags[0];
	//标注 改为通道路数 #line1
		DisplayLightControllerParameter(ls, wstrLcGpName,m_nCrtLcID, wstrTag,true);

		CString sMode;
		sMode.LoadString(IDS_STRING_LC_MODE_ALWAYS_ON);
		m_comboMode.AddString(sMode);

		sMode.LoadString(IDS_STRING_LC_MODE_TRIGGER);
		m_comboMode.AddString(sMode);

		sMode.LoadString(IDS_STRING_LC_MODE_TRIGGER_TWICE);
		m_comboMode.AddString(sMode);
	}
	else
	{
		CString sID;
		int nIndex = m_comboIDs.GetCurSel();
		if(nIndex != -1)
		{
			m_comboIDs.GetLBText(nIndex, sID);
			m_nCrtLcID = (int)utils::to_float(sID);
		}
		m_comboMode.SetCurSel(-1);
	}

	UpdateData(FALSE);
}

void CChannelConfigDlg::ResetLcSliderWidget()
{
	// Line #1
	m_sliderLum1.SetPos(0);
	SetDlgItemInt(IDC_STATIC_VALUE_LUM1, 0);
	m_sliderDelay.SetPos(1);
	SetDlgItemText(IDC_STATIC_VALUE_DELAY,_T("0.1"));
	m_sliderDur.SetPos(0);
	SetDlgItemInt(IDC_STATIC_VALUE_DURATION, 0);
	// Line #2
	m_sliderLum2.SetPos(0);
	SetDlgItemInt(IDC_STATIC_VALUE_LUM2, 0);
	// Line #3
	m_sliderLum3.SetPos(0);
	SetDlgItemInt(IDC_STATIC_VALUE_LUM3, 0);
	// Line #4
	m_sliderLum4.SetPos(0);
	SetDlgItemInt(IDC_STATIC_VALUE_LUM4, 0);
}

void CChannelConfigDlg::RefreshLcSliderWidget()
{
	for(auto it = m_dictSlider_Value.begin(); it != m_dictSlider_Value.end(); it++)
	{
		CWnd* pValueCtrl = it->second;
		if(pValueCtrl != nullptr)
		{
			CRect rc;
			pValueCtrl->GetWindowRect(rc);
			InvalidateRect(rc);
		}
	}
}

void CChannelConfigDlg::DisplayLightControllerParameter(const LIGHT_SETTING& ls, wstring wstrLcGpName, int nControllerID, wstring wstrTags,bool bChange)
{
	if(wstrTags != L"")
	{
		std::vector<wstring> vTags = utils::SplitString(wstrTags, L"$",token_compress_off,false); //支持空字符串分割
		if (vTags[0]==L"")
		{
			if (m_bSingleMode)
			{
				SetCH1ShowWindow(SW_HIDE);
				m_bSingleModeCH1 = false;
			}
		}
		else
		{
			m_editTag1.SetWindowText(vTags[0].c_str());
		}
		if (vTags[1]==L"")
		{
			if (m_bSingleMode)
			{	
				SetCH2ShowWindow(SW_HIDE);
				m_bSingleModeCH2 = false;
			}
		}
		else
		{
			m_editTag2.SetWindowText(vTags[1].c_str());
		}
		if (vTags[2]==L"")
		{
			if (m_bSingleMode)
			{	
				SetCH3ShowWindow(SW_HIDE);
				m_bSingleModeCH3 = false;
			}
		}
		else
		{
			m_editTag3.SetWindowText(vTags[2].c_str());
		}
		if (vTags[3]==L"")
		{
			if (m_bSingleMode)
			{	
				SetCH4ShowWindow(SW_HIDE);
				m_bSingleModeCH4= false;
			}
		}
		else
		{
			m_editTag4.SetWindowText(vTags[3].c_str());
		}
		m_nCrtLcID = nControllerID;
	}
	if(ls.m_dictParamGroup.empty())
	{
		m_nCrtMode      = -1;
		return;
	}
	auto it = ls.m_dictParamGroup.find(wstrLcGpName);
	if(it == ls.m_dictParamGroup.end())
	{
		m_nCrtMode      = -1;

		m_sliderLum1.SetPos(0);
		m_sliderDelay.SetPos(0);
		m_sliderDur.SetPos(1);

		SetDlgItemInt(IDC_STATIC_VALUE_LUM1, 0);
		SetDlgItemText(IDC_STATIC_VALUE_DELAY,_T(""));
		SetDlgItemInt(IDC_STATIC_VALUE_DURATION, 1);

		m_sliderLum2.SetPos(0);
		m_sliderDelay2.SetPos(0);
		m_sliderDur2.SetPos(1);

		SetDlgItemInt(IDC_STATIC_VALUE_LUM2, 0);
		SetDlgItemText(IDC_STATIC_VALUE_DELAY2,_T(""));
		SetDlgItemInt(IDC_STATIC_VALUE_DURATION2, 1);

		m_sliderLum3.SetPos(0);
		m_sliderDelay3.SetPos(0);
		m_sliderDur3.SetPos(1);
		SetDlgItemInt(IDC_STATIC_VALUE_LUM3, 0);
		SetDlgItemText(IDC_STATIC_VALUE_DELAY3,_T(""));
		SetDlgItemInt(IDC_STATIC_VALUE_DURATION3, 1);

		m_sliderLum4.SetPos(0);
		m_sliderDelay4.SetPos(0);
		m_sliderDur4.SetPos(1);
		SetDlgItemInt(IDC_STATIC_VALUE_LUM4, 0);
		SetDlgItemText(IDC_STATIC_VALUE_DELAY4,_T(""));
		SetDlgItemInt(IDC_STATIC_VALUE_DURATION4, 1);
		return;
	}

	bool bDisplayOk = false;
	std::vector<LC> vLC = it->second;
	for(size_t t = 0; t < vLC.size(); t++)
	{
		LC lc = vLC[t];
		if(lc.m_nID == nControllerID)
		{
			if( bChange)
			{
				m_comboMode.SetCurSel(lc.m_nMode);
				m_nCrtMode      = lc.m_nMode;
			}
			else
			{
				m_comboMode.SetCurSel(m_nCrtMode);
			}

			if(lc.m_vChParam.size() == 4)
			{	CString sDelay;
				if( m_nCrtMode == LC_TRIGGER_TWICE)
				{
					m_sliderLum1.SetPos(lc.m_vChParam[0].m_nLum1);
					SetDlgItemInt(IDC_STATIC_VALUE_LUM1, lc.m_vChParam[0].m_nLum1);
					m_sliderDelay.SetPos(lc.m_vChParam[0].m_nDelay1);

					sDelay.Format(_T("%.1f"), float(lc.m_vChParam[0].m_nDelay1 / 10.0f));
					SetDlgItemText(IDC_STATIC_VALUE_DELAY,sDelay);
					m_sliderDur.SetPos(lc.m_vChParam[0].m_nDuration1);
					SetDlgItemInt(IDC_STATIC_VALUE_DURATION, lc.m_vChParam[0].m_nDuration1);

					//add by zhujilong 兼容单通道设置

					m_sliderDelay2.SetPos(lc.m_vChParam[1].m_nDelay1);
					sDelay.Format(_T("%.1f"), float(lc.m_vChParam[1].m_nDelay1 / 10.0f));
					SetDlgItemText(IDC_STATIC_VALUE_DELAY2,sDelay);
					m_sliderDur2.SetPos(lc.m_vChParam[1].m_nDuration1);
					SetDlgItemInt(IDC_STATIC_VALUE_DURATION2, lc.m_vChParam[1].m_nDuration1);

					m_sliderDelay3.SetPos(lc.m_vChParam[2].m_nDelay1);
					sDelay.Format(_T("%.1f"), float(lc.m_vChParam[2].m_nDelay1 / 10.0f));
					SetDlgItemText(IDC_STATIC_VALUE_DELAY3,sDelay);
					m_sliderDur3.SetPos(lc.m_vChParam[2].m_nDuration1);
					SetDlgItemInt(IDC_STATIC_VALUE_DURATION3, lc.m_vChParam[2].m_nDuration1);

					m_sliderDelay4.SetPos(lc.m_vChParam[3].m_nDelay1);
					sDelay.Format(_T("%.1f"), float(lc.m_vChParam[3].m_nDelay1 / 10.0f));
					SetDlgItemText(IDC_STATIC_VALUE_DELAY4,sDelay);
					m_sliderDur4.SetPos(lc.m_vChParam[3].m_nDuration1);
					SetDlgItemInt(IDC_STATIC_VALUE_DURATION4, lc.m_vChParam[3].m_nDuration1);
					//end

					// Line #2
					m_sliderLum2.SetPos(lc.m_vChParam[1].m_nLum1);
					SetDlgItemInt(IDC_STATIC_VALUE_LUM2, lc.m_vChParam[1].m_nLum1);
					// Line #3
					m_sliderLum3.SetPos(lc.m_vChParam[2].m_nLum1);
					SetDlgItemInt(IDC_STATIC_VALUE_LUM3, lc.m_vChParam[2].m_nLum1);
					// Line #4
					m_sliderLum4.SetPos(lc.m_vChParam[3].m_nLum1);
					SetDlgItemInt(IDC_STATIC_VALUE_LUM4, lc.m_vChParam[3].m_nLum1);
				
				}
				else
				// Line #1
				{
					m_sliderLum1.SetPos(lc.m_vChParam[0].m_nLum);
					SetDlgItemInt(IDC_STATIC_VALUE_LUM1, lc.m_vChParam[0].m_nLum);
					m_sliderDelay.SetPos(lc.m_vChParam[0].m_nDelay);

					sDelay.Format(_T("%.1f"), float(lc.m_vChParam[0].m_nDelay / 10.0f));
					SetDlgItemText(IDC_STATIC_VALUE_DELAY,sDelay);
					m_sliderDur.SetPos(lc.m_vChParam[0].m_nDuration);
					SetDlgItemInt(IDC_STATIC_VALUE_DURATION, lc.m_vChParam[0].m_nDuration);

					//add by zhujilong 兼容单通道设置
					if (!m_bOneTouchFour)
					{
						sDelay.Format(_T("%.1f"), float(lc.m_vChParam[1].m_nDelay / 10.0f));

						m_sliderDelay2.SetPos(lc.m_vChParam[1].m_nDelay);
						SetDlgItemText(IDC_STATIC_VALUE_DELAY2,sDelay);
						m_sliderDur2.SetPos(lc.m_vChParam[1].m_nDuration);
						SetDlgItemInt(IDC_STATIC_VALUE_DURATION2, lc.m_vChParam[1].m_nDuration);

						sDelay.Format(_T("%.1f"), float(lc.m_vChParam[2].m_nDelay / 10.0f));

						m_sliderDelay3.SetPos(lc.m_vChParam[2].m_nDelay);
						SetDlgItemText(IDC_STATIC_VALUE_DELAY3,sDelay);
						m_sliderDur3.SetPos(lc.m_vChParam[2].m_nDuration);
						SetDlgItemInt(IDC_STATIC_VALUE_DURATION3, lc.m_vChParam[2].m_nDuration);

						sDelay.Format(_T("%.1f"), float(lc.m_vChParam[3].m_nDelay / 10.0f));

						m_sliderDelay4.SetPos(lc.m_vChParam[3].m_nDelay);
						SetDlgItemText(IDC_STATIC_VALUE_DELAY4,sDelay);
						m_sliderDur4.SetPos(lc.m_vChParam[3].m_nDuration);
						SetDlgItemInt(IDC_STATIC_VALUE_DURATION4, lc.m_vChParam[3].m_nDuration);
					}
			
					//end

					// Line #2
					m_sliderLum2.SetPos(lc.m_vChParam[1].m_nLum);
					SetDlgItemInt(IDC_STATIC_VALUE_LUM2, lc.m_vChParam[1].m_nLum);
					// Line #3
					m_sliderLum3.SetPos(lc.m_vChParam[2].m_nLum);
					SetDlgItemInt(IDC_STATIC_VALUE_LUM3, lc.m_vChParam[2].m_nLum);
					// Line #4
					m_sliderLum4.SetPos(lc.m_vChParam[3].m_nLum);
					SetDlgItemInt(IDC_STATIC_VALUE_LUM4, lc.m_vChParam[3].m_nLum);
				}


				bDisplayOk = true;
			}
			break;
		}
	}
	if(!bDisplayOk)
	{
		ResetLcSliderWidget();
	}
	RefreshLcSliderWidget();
	UpdateData(FALSE);
}

bool CChannelConfigDlg::SingledRoiAlgo(FUNC_INFO& funcInfo)
{
	int nRgnID = funcInfo.GetTargetRegionID();
	long nCount = (long)std::count_if(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end(), [&](FUNC_INFO& fi)->bool
	{
		return (fi.GetTargetRegionID() == nRgnID);
	});

	return (nCount <= 1);
}

void CChannelConfigDlg::AddRoiFunc2List()
{
	int nListIndex = 0;
	m_dictListIndex_FuncIndex.clear();
	int nCrtRoiFuncIndex = 0;
	std::for_each(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end(), [&](FUNC_INFO& funcInfo)
	{
		CString sCustomizedName = funcInfo.m_strCustomizedName.c_str();
		int nFuncIndex = funcInfo.m_funcIndex;
		if(!funcInfo.m_bTestFunc)
		{
			nCrtRoiFuncIndex = nFuncIndex;
			bool bFolded = true;
			if(m_dictRoiFuncIndex_Folded.find(nFuncIndex) == m_dictRoiFuncIndex_Folded.end())
			{
				m_dictRoiFuncIndex_Folded[nFuncIndex] = true;
			}
			else
			{
				bFolded = m_dictRoiFuncIndex_Folded[nFuncIndex];
			}
			CString sFoldFlag;
			if(nListIndex == 0)
			{ 
				sFoldFlag = _T(" ");
			}
			else
			{
				sFoldFlag = (bFolded == true)? _T("+"):_T("—");
			}
			m_algoList.InsertItem(nListIndex,_T(""));
			m_algoList.SetItemText(nListIndex, 1, sFoldFlag);
			m_algoList.SetItemText(nListIndex, 2, sCustomizedName);
			m_algoList.SetItemText(nListIndex, 3, _T("0"));
			m_algoList.SetItemText(nListIndex, 4, _T(" "));
			m_algoList.SetCheck(nListIndex,funcInfo.m_bEnable);
			m_dictListIndex_FuncIndex[nListIndex] = nFuncIndex;
			nListIndex++;
			if(!bFolded)
			{
				AddTestFunc2List(nFuncIndex,nListIndex);
			}
		}
	});
	m_algoList.SetDictListIndexFuncIndex(m_dictListIndex_FuncIndex);
}

void CChannelConfigDlg::AddTestFunc2List(int nRoiFunIndex, int& nListIndex)
{
	size_t sz = m_pFuncInfoArray->size();
	for(size_t t = nRoiFunIndex + 1; t < sz; t++)
	{
		FUNC_INFO funcInfo = (*m_pFuncInfoArray)[t];
		if(!funcInfo.m_bTestFunc)
		{
			return;
		}
		CString sName = _T("     ");
		sName.Append(funcInfo.m_strCustomizedName.c_str());
		m_algoList.InsertItem(nListIndex,_T(""));
		m_algoList.SetItemText(nListIndex, 1, _T(""));
		m_algoList.SetItemText(nListIndex, 2, sName);
		CString str;
		str.Format(_T("%d"),funcInfo.m_nErrCount);
		m_algoList.SetItemText(nListIndex, 3, str);
		m_algoList.SetItemText(nListIndex, 4, _T("0.0"));
		m_algoList.SetCheck(nListIndex,funcInfo.m_bEnable);
		m_dictListIndex_FuncIndex[nListIndex] = funcInfo.m_funcIndex;
		nListIndex++;
	}
}

void CChannelConfigDlg::InitAlgoListBoard(CRect& rcWorkArea, CRect& rcClient)
{
	if(m_pProcessStation == nullptr)
	{
		return;
	}
	m_algoList.SetFont(&m_txtFontPB);

	DWORD dwStyleEx = m_algoList.GetExtendedStyle();
	dwStyleEx |= LVS_EX_FULLROWSELECT; 
	dwStyleEx |= LVS_EX_GRIDLINES;     
	dwStyleEx |= LVS_EX_CHECKBOXES;   
	m_algoList.SetExtendedStyle(dwStyleEx);

	CString strIndex = _T("");
	CString strAlgoName(MAKEINTRESOURCE(IDS_STRING_ALGO_NAME));
	CString strErrCount(MAKEINTRESOURCE(IDS_STRING_ERROR_COUNT));

	CString strLastN;
	strLastN.Format(_T("%d"),m_chConfig.m_rejectorSetting.m_nLastN);

	CString strErrCountRec = strErrCount+strLastN;
	CString strRate(MAKEINTRESOURCE(IDS_STRING_RATE));
	strRate.Append(_T("%"));
	CString strRateRec = strRate+strLastN;

	int nWidth0 = 20;
	int nWidth1 = 20;
	int nWidth2 = 130;
	int nWidth3 = 75;
	int nWidth4 = 75;
	int nWidth5 = 90+5;

	int vtBtnSz = 64; 
	int gap     = 5;
	int listW   = 530;
	if(rcWorkArea.Width() == 1920)
	{
		nWidth0 = int(nWidth0 * 1.15f);
		nWidth1 = int(nWidth1 * 1.15f);
		nWidth2 = int(nWidth2 * 1.15f);
	    nWidth3 = int(nWidth3 * 1.15f);
		nWidth4 = int(nWidth4 * 1.15f);
		nWidth5 = int(nWidth5 * 1.15f);
		listW   = int(listW * 1.15f);
	}
	m_algoList.InsertColumn(0, _T(""), LVCFMT_CENTER, nWidth0);
	m_algoList.InsertColumn(1, _T(""), LVCFMT_LEFT, nWidth1);
	m_algoList.InsertColumn(2, strAlgoName, LVCFMT_LEFT, nWidth2);
	m_algoList.InsertColumn(3, strErrCount,LVCFMT_LEFT,nWidth3);
	m_algoList.InsertColumn(4, strRate, LVCFMT_LEFT, nWidth4);
	m_algoList.InsertColumn(5, strErrCountRec, LVCFMT_LEFT, nWidth5);
	m_algoList.InsertColumn(6, strRateRec, LVCFMT_LEFT, nWidth5);
	

	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate);

	int imgDispWndW = m_chConfig.m_uiSetting.m_width;
	int imgDispWndH = m_chConfig.m_uiSetting.m_height;
	
	CRect crt;
	crt = m_ScreenOp.GetPosRect();
	imgDispWndW = crt.Width();
	imgDispWndH = crt.Height();

	if(rcWorkArea.Width() != 1920)
	{
		listW = 330;
	}

	CRect rc;
	rc.left = rcClient.left + gap + vtBtnSz + gap + imgDispWndW + gap;
	rc.top = titleAreaH + vtBtnSz + 8;
	rc.right = rc.left + listW;
	rc.bottom = rc.top + imgDispWndH - 58;

	m_algoList.MoveWindow(rc);

	CRect rcHelp;
	rcHelp.left   = rc.left + 1;
	rcHelp.top    = rc.bottom;
	rcHelp.right  = rc.right - 1;
	rcHelp.bottom = rc.top + imgDispWndH;
	m_algoHelpStrPanel.SetFont(&m_txtFontPB);
	m_algoHelpStrPanel.MoveWindow(rcHelp);

	m_pFuncInfoArray      = m_pProcessStation->GetFuncInfoArrayPtr();
	m_pDebugFuncInfoArray = m_pProcessStation->GetDebugFuncInfoArrayPtr();
	if(m_pFuncInfoArray == nullptr || m_pFuncInfoArray->size() == 0)
	{
		return;
	}
	AddRoiFunc2List();
}

void CChannelConfigDlg::InitParameterBoard(CRect& rcWorkArea, CRect& rcClient)
{
	CRect rc;
	m_algoList.GetWindowRect(rc);
	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate);

	rc.top = rcClient.top + titleAreaH + 3;
	rc.left = rc.right + 5;
	rc.right = rcWorkArea.right - 5;
	if(rc.right - rc.left > 400)
	{
		rc.right = rc.left + 400;
	}
	rc.bottom = rcWorkArea.bottom - 80;
	m_paramBoard.MoveWindow(rc);
	CString strName;
	strName.LoadString(IDS_STRING_ALGO_PARAM);
	m_paramBoard.SetWindowText(strName);

	CRect rcHelpPanel;
	rcHelpPanel.left   = rc.left + 8;
	rcHelpPanel.top    = rc.bottom - 100;
	rcHelpPanel.right  = rc.right - 8;
	rcHelpPanel.bottom = rc.bottom - 10;
	m_paramHelpStrPanel.SetFont(&m_txtFontPB);
	m_paramHelpStrPanel.MoveWindow(rcHelpPanel);

	CString str;
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_SYSLINK_ALGO_HELP);
	if(pWnd != NULL)
	{
		pWnd->SetFont(&m_bigFont);
		str.LoadString(IDS_STRING_TEST_PARAM);
		CString sTmp(_T("<a>"));
		sTmp.Append(str);
		sTmp.Append(_T("</a>"));
		pWnd->SetWindowText(sTmp);
	}
}

void CChannelConfigDlg::InitApplyParamButton(CRect& rcWorkArea)
{
	CRect rc;
	m_paramBoard.GetWindowRect(rc);
	CRect rcClient;
	m_btnSaveTR.GetClientRect(rcClient);
	CRect rcT;
	rcT.left = rc.left + int((rc.Width() - rcClient.Width()) / 2.0); 
	rcT.right = rcT.left + rcClient.Width();
	rcT.top = rc.bottom + (rcWorkArea.bottom - rc.bottom - rcClient.Height()) / 2;
	rcT.bottom = rcT.top + rcClient.Height();

	CString str;
	str.LoadString(IDS_STRING_APPLY_PARAM);
	m_btnApplyParam.SetFont(&m_txtFont);
	m_btnApplyParam.SetWindowText(str);
	m_btnApplyParam.MoveWindow(rcT);
	m_btnApplyParam.EnableWindow(FALSE);
}

void CChannelConfigDlg::InitSaveTrainningResultButton(CRect& rcWorkArea)
{
	CRect rc;
	m_paramBoard.GetWindowRect(rc);
	CRect rcClient;
	m_btnSaveTR.GetClientRect(rcClient);
	CRect rcT;
	rcT.left = rc.left + int((rc.Width() - rcClient.Width()) / 2.0); 
	rcT.right = rcT.left + rcClient.Width();
	rcT.top = rc.bottom + (rcWorkArea.bottom - rc.bottom - rcClient.Height()) / 2;
	rcT.bottom = rcT.top + rcClient.Height();

	CString str;
	str.LoadString(IDS_STRING_CH_DLG_SAVE_TRAINNING_RESULT);
	m_btnSaveTR.SetFont(&m_txtFont);
	m_btnSaveTR.SetWindowText(str);
	m_btnSaveTR.MoveWindow(rcT);
	bool bModelImageReady = m_pProcessStation->IsModelImageReady();
	m_btnSaveTR.EnableWindow(bModelImageReady);
	m_btnSaveTR.ShowWindow(m_bSelfLearningMode);
}

void CChannelConfigDlg::SetButtonVisibility()
{
	if(m_bSelfLearningMode)
	{
		m_btnApplyParam.ShowWindow(SW_HIDE);
		m_btnSaveTR.ShowWindow(SW_SHOW);
		m_btnAddModelImg.ShowWindow(SW_HIDE);
		m_btnDelCrtImg.ShowWindow(SW_SHOW);
		EnableAllButtons(false);
		EnableUiBtn(IDB_DELETE_CRT_IMG, true, IMG_CH_DELETE_MODEL_IMG);
	//	EnableUiBtn(IDB_ZOOM_IN, true, IMG_CH_ZOOM_IN);
	}
	else
	{
		m_btnApplyParam.ShowWindow(SW_SHOW);
		m_btnSaveTR.ShowWindow(SW_HIDE);
		if (m_bSelfLearningFunc)
		{
			m_btnAddModelImg.ShowWindow(SW_SHOW);
			m_btnDelCrtImg.ShowWindow(SW_HIDE);
		}
	
	}	
	UAC uac = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (uac.m_nLevel == UAC_ADMIN);

	bool bCameraRun  = gSystemMgr.IsChannelCameraRunning(m_chConfig.m_chIndex);
	if(bCameraRun)
	{
		if(bAdmin)
		{
			EnableUiBtn(IDB_ON_OFF,true,IMG_CH_CAMERA_RUN);
		}
		else
		{
			EnableUiBtn(IDB_ON_OFF,false,IMG_CH_CAMERA_RUN_GRAY);
		}
		m_btnResetStatsData.SetLabelImage(IMG_CH_RESET_STATS_DATA_GRAY);
        m_btnResetStatsData.EnableWindow(FALSE);

	}
	else
	{
		if(!m_bSelfLearningMode)
			EnableUiBtn(IDB_ON_OFF,true,IMG_CH_CAMERA_PAUSE);
		else
			EnableUiBtn(IDB_ON_OFF,false,IMG_CH_CAMERA_PAUSE_GRAY);

		m_btnResetStatsData.SetLabelImage(IMG_CH_RESET_STATS_DATA);//
        m_btnResetStatsData.EnableWindow(TRUE);
	}
    m_btnResetStatsData.Invalidate();
	m_btnOnOff.Invalidate();
}

void CChannelConfigDlg::InitSpcChart(CRect& rcWorkArea)
{
	CRect rcPanel, rcSettingBoard;
	m_algoHelpStrPanel.GetWindowRect(rcPanel);
	m_paramBoard.GetWindowRect(rcSettingBoard);

	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate);

	int vtBtnSz = 64; // Vertical Button Size
	int hzBtnSz = 64; //  Horizontal Button Size;
	int gapH = 5;
	int gapV = 10;
	 
	CRect rcChart;
	rcChart.left  = rcWorkArea.left + gapH + hzBtnSz + 5;
	rcChart.top   =  rcPanel.bottom + 45;
	rcChart.right = rcSettingBoard.left - 5;
	rcChart.bottom = rcWorkArea.bottom - 5;

	if (!m_chartCtrl)
	{
		m_chartCtrl.Create(NULL,NULL,NULL, rcChart, this,NULL);
		m_chartCtrl.CreateGraph(GT_2DBAR);
		m_chartCtrl.ShowGraphLegend(FALSE);
		m_chartCtrl.SetGraphBackgroundColor(RGB(200,220,250));
		CString title;
		title = _T("");
		m_chartCtrl.SetGraphTitle(title);
		m_chartCtrl.SetGraphTitleColor(RGB(255,0,0));

		m_chartCtrl.SetGraphSubtitle(_T(""));
		m_chartCtrl.SetGraphTitleShadow(FALSE);
		m_chartCtrl.SetGraphSubtitleShadow(FALSE);
		m_chartCtrl.Invalidate();
	}
	m_chartCtrl.MoveWindow(rcChart);

	UAC uac = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (uac.m_nLevel == UAC_ADMIN);
	int top = rcChart.top + int((rcChart.Height() - 3 * vtBtnSz - 2 * 5) / 2.0);
	CRect rcBtn;
	rcBtn.left = rcWorkArea.left + 5;
	rcBtn.top  = top;
	rcBtn.right = rcBtn.left + vtBtnSz;
	rcBtn.bottom = rcBtn.top + vtBtnSz;

	if (!m_btnDecrease)
	{
		m_btnDecrease.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this,IDB_LEFT_ARROW);
	}
	m_btnDecrease.MoveWindow(rcBtn);
	
	if(bAdmin)
	{
		EnableUiBtn(IDB_LEFT_ARROW, true,IMG_CH_LEFT_ARROW);
	}
	else
	{
		EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
	}
	m_btnDecrease.Invalidate();

	// LCL
	rcBtn.top = rcBtn.bottom + 5;
	rcBtn.bottom = rcBtn.top + vtBtnSz; 
	if(!m_lblThreshold_Lcl)
	{
		m_lblThreshold_Lcl.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rcBtn, this, IDB_THRESHOLD_PANEL_LCL);
		m_lblThreshold_Lcl.SetTxtFont(30,FW_BOLD,CENTER);
		m_lblThreshold_Lcl.SetColor(m_fgColor, m_bgColor);
		m_lblThreshold_Lcl.Invalidate();
	}
	m_lblThreshold_Lcl.MoveWindow(rcBtn);

	m_editThreshold.SetFont(&m_txtFont);
	m_editThreshold.MoveWindow(rcBtn);

	// UCL
	if (!m_lblThreshold_Ucl)
	{
		m_lblThreshold_Ucl.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rcBtn, this, IDB_THRESHOLD_PANEL_UCL);
		m_lblThreshold_Ucl.SetTxtFont(30,FW_BOLD,CENTER);
		m_lblThreshold_Ucl.SetColor(m_fgColor, m_bgColor);
		m_lblThreshold_Ucl.Invalidate();
	}
	m_lblThreshold_Ucl.MoveWindow(rcBtn);
 
	m_lblThreshold_Ucl.SetFont(&m_txtFont);
	m_lblThreshold_Ucl.ShowWindow(SW_HIDE);

	rcBtn.top = rcBtn.bottom + 5;
	rcBtn.bottom = rcBtn.top + vtBtnSz;
	if (! m_btnIncrease)
	{
		m_btnIncrease.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this,IDB_RIGHT_ARROW);
	}
	m_btnIncrease.MoveWindow(rcBtn);

	if(bAdmin)
	{
		EnableUiBtn(IDB_RIGHT_ARROW, true,IMG_CH_RIGHT_ARROW);
	}
	else
	{
		EnableUiBtn(IDB_RIGHT_ARROW, false,IMG_CH_RIGHT_ARROW_GRAY);
	}
	m_btnDecrease.EnableWindow(bAdmin);
	m_btnIncrease.Invalidate();
}

void CChannelConfigDlg::InitChannelSwitch(CRect& rcWorkArea)
{
	m_vChSwitchId.clear();
	m_vChSwitchId.push_back(IDC_RADIO_CH1);
	m_vChSwitchId.push_back(IDC_RADIO_CH2);
	m_vChSwitchId.push_back(IDC_RADIO_CH3);
	m_vChSwitchId.push_back(IDC_RADIO_CH4);
	m_vChSwitchId.push_back(IDC_RADIO_CH5);
	m_vChSwitchId.push_back(IDC_RADIO_CH6);
	m_vChSwitchId.push_back(IDC_RADIO_CH7);
	m_vChSwitchId.push_back(IDC_RADIO_CH8);

	std::for_each(m_vChSwitchId.begin(),m_vChSwitchId.end(), [&](UINT ctlId)
	{
		this->GetDlgItem(ctlId)->SetFont(&m_txtFont);
	});

	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate);

	int vtBtnSz = 64; // Vertical Button Size
	int hzBtnSz = 64; // Horizontal Button Size;
	int gapH = 5;
	int gapV = 10;
	 
	CRect rcBoard;
	m_paramBoard.GetWindowRect(rcBoard);
	CRect rcChart;
	m_chartCtrl.GetWindowRect(rcChart);

	int imgDispWndH;
	CRect crt;
	crt = m_ScreenOp.GetPosRect();
	imgDispWndH = crt.Height();


	CRect rc;
	rc.left = rcChart.left + 5;
	rc.top = rcWorkArea.top + titleAreaH + vtBtnSz + 8 + imgDispWndH;
	rc.right = rcBoard.left - 5;
	rc.bottom = rcChart.top - 5;

	auto ptrChSettings = gConfigMgr.GetChannelConfigPtr();
	size_t sz = ptrChSettings->size();

	CRect rcRadio;
	CWnd* pWnd = GetDlgItem(m_vChSwitchId[0]);
	pWnd->GetClientRect(rcRadio);
	int left = rc.left;
	rc.top = rc.top + int((rc.Height() - rcRadio.Height()) / 2.0);
	rc.bottom = rc.top  + rcRadio.Height();
	for(int t = 0; t < sz; t++)
	{
		rc.left   = left + t * (rcRadio.Width() + 10);
		rc.right  = rc.left + rcRadio.Width();

		pWnd = GetDlgItem(m_vChSwitchId[t]);
		pWnd->MoveWindow(rc);
		pWnd->ShowWindow(SW_SHOW);
		if(t == m_chConfig.m_chIndex - 1)
		{
			auto pBtn = (CButton*)pWnd;
			pBtn->SetCheck(BST_CHECKED);
			m_radioIndex = t;
			pBtn->Invalidate();
		}
	}

	CRect rcCheck;
	rcCheck.left   = rc.right + 40;
	rcCheck.right  = rcCheck.left + 20;
	rcCheck.top    = rc.top - 2;
	rcCheck.bottom = rc.bottom;
	pWnd = GetDlgItem(IDC_CHECK_DISP_ERROR_RGN);
	pWnd->SetFont(&m_txtFont);
	pWnd->MoveWindow(rcCheck);

	CRect rcCheckName = rcCheck;
	rcCheckName.left   = rcCheck.right + 5;
	rcCheckName.right  = rcCheckName.left + 80;
	int nLangID = gConfigMgr.GetLanguageID();
	if(nLangID == LANG_ENU)
	{
		rcCheckName.right = rcCheckName.left + 110;
	}
	pWnd = GetDlgItem(IDC_STATIC_DISP_ERROR_RGN);
	pWnd->SetFont(&m_txtFont);
	pWnd->MoveWindow(rcCheckName);

	CString strTxt;
	strTxt.LoadString(IDS_STRING_DISPLAY_ERROR_REGION);
	pWnd->SetWindowText(strTxt);

	CRect rcGrayValue = rcCheck;
	rcGrayValue.left   = rcCheckName.right + 10;
	rcGrayValue.right  = rcGrayValue.left + 180;
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_GRAY_VALIUE);
	pStatic->SetFont(&m_txtFont);
	pStatic->MoveWindow(rcGrayValue);
}

void CChannelConfigDlg::InitCameraSwitch(CRect& rcWorkArea)
{
	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate);

	int vtBtnSz = 64; // Vertical Button Size
	int hzBtnSz = 64; // Horizontal Button Size;
	int gapH = 5;
	int gapV = 10;

	CRect rcList;
	m_algoList.GetWindowRect(rcList);
	CRect rcChart;
	m_chartCtrl.GetWindowRect(rcChart);

	int imgDispWndH;
	CRect crt;
	crt = m_ScreenOp.GetPosRect();
	imgDispWndH = crt.Height();

	CRect rc;
	rc.top = rcWorkArea.top + titleAreaH + vtBtnSz + 10 + imgDispWndH;
	rc.left = rcList.left - 160;
	rc.right = rc.left + 75;
	rc.bottom = rcChart.top - 5;

	CWnd* pWnd = GetDlgItem(IDC_RADIO_CAMERA1);
	pWnd->MoveWindow(rc);
	pWnd->SetFont(&m_txtFont);
	if(m_bMultiChannnelImg)
	{
		pWnd->ShowWindow(SW_SHOW);
	}
	auto pBtn = (CButton*)pWnd;
	pBtn->SetCheck(BST_CHECKED);
	//m_radioIndex = t;
	pBtn->Invalidate();

	rc.left = rc.right + 10;
	rc.right = rc.left + 75;
	pWnd = GetDlgItem(IDC_RADIO_CAMERA2);
	pWnd->MoveWindow(rc);
	pWnd->SetFont(&m_txtFont);
	if(m_bMultiChannnelImg)
	{
		pWnd->ShowWindow(SW_SHOW);
	}
}

void CChannelConfigDlg::OnNMClickSyslinkAlgoHelp(NMHDR *pNMHDR, LRESULT *pResult)
{
	TASK* pTask = m_chConfig.GetCurrentTask();
	if(pTask != nullptr)
	{
		CString sProduct;
		if(pTask->m_nProductId == 1) sProduct = _T("CrownCap");
		if(pTask->m_nProductId == 2) sProduct = _T("EOE");
		if(pTask->m_nProductId == 3) sProduct = _T("CAN");
		if(pTask->m_nProductId == 4) sProduct = _T("PET");
		if(pTask->m_nProductId == 5) sProduct = _T("PCC");
		if(pTask->m_nProductId == 6) sProduct = _T("AL_PLASTIC");
		if(pTask->m_nProductId == 7) sProduct = _T("CAN_EOE");
		if(pTask->m_nProductId == 8) sProduct = _T("JELLY");
		if(pTask->m_nProductId == 9) sProduct = _T("CAN_OUTSIDE");
		if(pTask->m_nProductId == 10) sProduct = _T("NAIL");
		if(pTask->m_nProductId == 11) sProduct = _T("PLUG");
		if(pTask->m_nProductId == 12) sProduct = _T("HDPE");
		if(pTask->m_nProductId == 13) sProduct = _T("NEEDLE");
		if(pTask->m_nProductId == 14) sProduct = _T("RubberMat");
		if(pTask->m_nProductId == 15) sProduct = _T("BatteryShell");

		CString sLink;
		sLink.Format(_T("%s\\%s_%d.mht"),m_sManualPath, sProduct,m_crtFuncInfo->m_dispId);
		if(!utils::CheckFileExists(sLink))
		{
			CString sMsg;
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
			DisplayPromptMessage(sMsg, false);
			return;
		}
		ShellExecuteW(NULL, L"open", sLink, NULL, NULL, SW_SHOWNORMAL);
	}
	*pResult = 0;
}

void CChannelConfigDlg::CreateAlgoParamItems()
{
	CRect rc;
	m_paramBoard.GetWindowRect(rc);

	std::vector<INPUT_PARAM> vParam;
	if(m_crtFuncInfo != nullptr)
	{
		vParam = m_crtFuncInfo->m_vParam;
	}

	CString strHelp;
	int nNameCtrlWidth = 130;
    int nAdaptHeight = 0;  // 针对算子说明显示不全的扩展,双行显示

	int nLangID = ::gConfigMgr.GetLanguageID();
	if(nLangID == LANG_ENU)
	{
		nNameCtrlWidth = 160;
	}
	if(nLangID == LANG_RUS)
	{
		nNameCtrlWidth = 200;
		nAdaptHeight = 8;
	}
	CRect rcRoiStr;
	rcRoiStr.left   = rc.left + 5;
	rcRoiStr.top    = rc.top + 25;
	rcRoiStr.right  = rcRoiStr.left + 120;
	rcRoiStr.bottom = rcRoiStr.top + 26;
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_SYSLINK_ALGO_HELP);
	pWnd->MoveWindow(rcRoiStr);
	pWnd->ShowWindow(SW_SHOW);

	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);

	CRect rcStatic, rcEdit;
	rcStatic.top = rc.top + 35;
	rcEdit.top   = rc.top + 35;

	int nBtnIndex     = 0;
	CEdit* pEdit      = nullptr;
	CStatic* pStatic  = nullptr;

	for(int i = 0; i < vParam.size(); i++)
	{
		strHelp.LoadString(vParam[i].m_descID);
		wstring tmpStr = CT2CW(strHelp);
		std::vector<wstring> ret = utils::SplitString(tmpStr,L"$");
		m_vParamHelpStr.push_back(ret[1]);

		rcStatic.top += (24 + 5+nAdaptHeight);//
		rcEdit.top   = rcStatic.top;
		if(vParam[i].m_type == VT_BSTR)
		{
			rcStatic.left   = rc.left + 5;
			rcStatic.top    = rcEdit.bottom + 8;
			rcStatic.right  = rcStatic.left + nNameCtrlWidth;
			rcStatic.bottom = rcStatic.top + 22 + nAdaptHeight;
			pStatic = new CStatic;
			pStatic->Create(_T(""),WS_CHILD|WS_VISIBLE, rcStatic, this, 90000+i);
			pStatic->SetFont(&m_txtFontPB);
			pStatic->SetWindowText(ret[0].c_str());
			m_vParamNameCtrl.push_back(pStatic);

			rcEdit.left    = rcStatic.right + 8;
			rcEdit.top     = rcStatic.top;
			rcEdit.right   = rcEdit.left + 46;
			rcEdit.bottom  = rcEdit.top + 22;
			CStatic* pEditFileBtn = new CStatic;
			m_vParamEditCtrl.push_back(pEditFileBtn);
			long nEditBtnID = GetFileEditBtnID(nBtnIndex);
			pEditFileBtn->Create(_T(""),WS_CHILD|WS_DLGFRAME|WS_VISIBLE|SS_NOTIFY|SS_BITMAP|SS_CENTERIMAGE , rcEdit, this, nEditBtnID);
			pEditFileBtn->EnableWindow(bAdmin);

			nBtnIndex++;
			if(m_crtFuncInfo != nullptr)
			{
				m_dictBtnID_FI[nEditBtnID]       = m_crtFuncInfo;
				m_dictBtnID_FileType[nEditBtnID] = vParam[i].m_nFileType;
			}
			if(!vParam[i].m_wstrFile.empty())
			{
				wstring wstrFile = vParam[i].m_wstrFile; 
				boost::trim(wstrFile);
				if(!wstrFile.empty()/* && INVALID_FILE_ATTRIBUTES != (::GetFileAttributes(wstrFile.c_str()))*/)
				{
					m_dictBtnID_File[nEditBtnID] = wstrFile;
				}
			}
		}
		else
		{
			rcStatic.left   = rc.left + 5;
			rcStatic.right  = rcStatic.left + nNameCtrlWidth;
			rcStatic.bottom = rcStatic.top + 22 + nAdaptHeight;
			pStatic = new CStatic;
			pStatic->Create(_T(""),WS_CHILD|WS_VISIBLE, rcStatic, this, 90000+i);
			pStatic->SetFont(&m_txtFontPB);
			pStatic->SetWindowText(ret[0].c_str());
			m_vParamNameCtrl.push_back(pStatic);

			rcEdit.left    = rcStatic.right + 8;
			rcEdit.right   = rcEdit.left + 56;
			rcEdit.bottom  = rcEdit.top + 22;
			pEdit   = new CEdit;
			pEdit->Create(WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcEdit, this, IDC_EDIT_PARAMETER);
			pEdit->EnableWindow((i != 0 && i != vParam.size()) && bAdmin);
			pEdit->SetFont(&m_txtFontPB);
			float fValue = vParam[i].m_value;
			int   nValue = (int)fValue;
			CString strValue;
			if(vParam[i].m_type == VT_R4)
			{
				strValue.Format(_T("%.3f"), fValue);
			}
			else
			{
				strValue.Format(_T("%d"), nValue);
			}
			pEdit->SetWindowText(strValue);
			m_vParamEditCtrl.push_back(pEdit);
		}
	}
	CRect rcHelpPanel;
	rcHelpPanel.left   = rc.left + 8;
	rcHelpPanel.top    = rcEdit.bottom + 20;
	rcHelpPanel.right  = rc.right - 8;
	rcHelpPanel.bottom = rcEdit.bottom + 180;
	m_paramHelpStrPanel.MoveWindow(rcHelpPanel);
	m_paramBoard.ShowWindow(SW_HIDE);
	m_paramBoard.ShowWindow(SW_SHOW);

	SetFileButtonImage();
}

void CChannelConfigDlg::CreateSlAlgoParamItems()
{
	TASK task;
	if(!m_chConfig.GetCurrentTask(task) || task.m_bSelfLearning == false)
	{
		return;
	}
	wstring wstrParam = task.m_wstrSlParam;
	if(wstrParam == L"") return;

	std::vector<wstring> vItem = utils::SplitString(wstrParam,L",");
	if(vItem.empty()) return;

	CRect rcBoard;
	m_paramBoard.GetWindowRect(rcBoard);

	int nNameCtrlWidth = 120;
	int nLangID = ::gConfigMgr.GetLanguageID();
	if(nLangID == LANG_ENU)
	{
		nNameCtrlWidth = 150;
	}

	CRect rcName, rcValue;
	rcName.left    = rcBoard.left + 10;
	rcName.right   = rcName.left + nNameCtrlWidth;
	rcName.bottom  = rcBoard.top + 40;

	rcValue.left   = rcName.right + 8;
	rcValue.right  = rcValue.left + 60;
	rcValue.bottom = rcBoard.top + 40;
	CString sParamName;
	CStatic* pNameCtrl = nullptr;
	CEdit*  pValueCtrl = nullptr;

	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);
	for(size_t t = 0; t < vItem.size(); t++)
	{
		wstring wstrItem = vItem[t];
		std::vector<wstring> vNameValue = utils::SplitString(wstrItem, L"@");

		rcName.top    = rcName.bottom + 8;
		rcName.bottom = rcName.top + 22;
		
		pNameCtrl = new CStatic;
		pNameCtrl->Create(_T(""),WS_CHILD|WS_VISIBLE, rcName, this, UINT(99000+t));
		pNameCtrl->SetFont(&m_txtFontPB);
		long nStrID = boost::lexical_cast<long>(vNameValue[0]);
		CString sName;
		sName.LoadString(nStrID);
		int index = sName.Find(_T("$"));
		sName.Truncate(index);
		pNameCtrl->SetWindowText(sName);
		m_vParamNameCtrl.push_back(pNameCtrl);

		rcValue.top    = rcName.top;
		rcValue.bottom = rcName.bottom;

		pValueCtrl = new CEdit;
		pValueCtrl->Create(WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcValue, this, IDC_EDIT_PARAMETER);
		pValueCtrl->EnableWindow(bAdmin);
		pValueCtrl->SetFont(&m_txtFontPB);
		float fValue = boost::lexical_cast<float>(vNameValue[1]);
		CString sValue;
		sValue.Format(_T("%.3f"), fValue);
		pValueCtrl->SetWindowText(sValue);
		m_vParamEditCtrl.push_back(pValueCtrl);
	}
}

void CChannelConfigDlg::UpdatedictBtnID_File(int index)
{
	wostringstream oss;  
	oss<<L"#"<<index;
	std::wstring wstr=oss.str(); 

	for(auto it = m_dictBtnID_File.begin(); it != m_dictBtnID_File.end(); it++)
	{
	    wstring *pstr = &(it->second);
		int i = pstr->find(_T("#"));
		int j = pstr->find_last_of(_T("#"));
		if (i != string::npos && j != string::npos)
		{
			pstr->replace(j,2,wstr);
			pstr->replace(i,2,wstr);
		}
	}
}

void CChannelConfigDlg::SetFileButtonImage()
{
	UpdatedictBtnID_File(m_iCameraIndex);

	for(auto it = m_dictBtnID_FI.begin(); it != m_dictBtnID_FI.end(); it++)
	{
		long nBtnID = it->first;
		CStatic* pWnd = (CStatic*)GetDlgItem(nBtnID);
		if(pWnd != nullptr)
		{
			CBitmap bmpEdit;
			if(m_dictBtnID_File.find(nBtnID) != m_dictBtnID_File.end())
			{
				if (INVALID_FILE_ATTRIBUTES != (::GetFileAttributes(m_dictBtnID_File[nBtnID].c_str())))
 				{
					bmpEdit.LoadBitmapW(IDB_BITMAP_EDIT_REGION_OK);
				}
				else
 				{
 					bmpEdit.LoadBitmapW(IDB_BITMAP_EDIT_REGION_NG);
 				}
			}
			else
			{
				bmpEdit.LoadBitmapW(IDB_BITMAP_EDIT_REGION_NG);
			}
			pWnd->SetBitmap(bmpEdit);
		//	pWnd->ShowWindow(SW_HIDE);
			pWnd->ShowWindow(SW_SHOW);
		}
	}
}

void CChannelConfigDlg::DeleteAllSettingItem()
{
	m_crtFuncIndex         = -1;
	m_crtFuncInfo          = nullptr;
	m_crtFuncInfoDebug     = nullptr;
	m_pCrtSlider           = nullptr;

	m_dictBtnID_File.clear();
	m_dictBtnID_FileType.clear();
	m_dictBtnID_FI.clear();

	if (m_settingType != T_CAMERA)
	{
		if (m_pComCameraStation)
		{
			m_pComCameraStation->ShowWindow(SW_HIDE);
		}
	}

	m_bCheckTaskAttach.ShowWindow(SW_HIDE);

	std::for_each(m_vParamNameCtrl.begin(), m_vParamNameCtrl.end(),[&](CWnd* pWnd)
	{
		delete pWnd;
		pWnd = nullptr;
	});
	m_vParamNameCtrl.clear();
	std::for_each(m_vParamEditCtrl.begin(), m_vParamEditCtrl.end(),[&](CWnd* pWnd)
	{
		delete pWnd;
		pWnd = nullptr;
	});
	m_vParamEditCtrl.clear();
	m_vParamHelpStr.clear();

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_SYSLINK_ALGO_HELP);
	if(pWnd != NULL)
	{
		pWnd->ShowWindow(SW_HIDE);
	}

	m_crtEditCtrl = nullptr;
	CWnd* pBtn = GetDlgItem(IDC_STATIC_BTN_DECREASE);
	pBtn->ShowWindow(SW_HIDE);

	pBtn = GetDlgItem(IDC_STATIC_BTN_INCREASE);
	pBtn->ShowWindow(SW_HIDE);

	m_algoHelpStrPanel.SetWindowText(_T(""));
	m_algoHelpStrPanel.Invalidate();

	m_paramHelpStrPanel.SetWindowText(_T(""));
	m_paramHelpStrPanel.Invalidate();

	m_btnApplyParam.EnableWindow(FALSE);

	m_editThreshold.ShowWindow(SW_HIDE);

	if (m_pStaCameraStation)
	{
		m_pStaCameraStation->ShowWindow(SW_HIDE);
	}

	if (m_pComCameraStation)
	{
		m_pComCameraStation->ShowWindow(SW_HIDE);
	}

}

float CChannelConfigDlg::GetThresholdStrideLength()
{
	float fRet = m_crtFuncInfo->m_threshold.m_stepLen;

	bool bIncreaseStride = false;
	if(!m_dictOutputStats.empty())
	{
		float fCrtThreshold = m_crtFuncInfo->m_threshold.m_specV;
		m_fStatsMax = -99999999999.9f;
		m_fStatsMin = 999999999999.9f;
		for(auto it = m_dictOutputStats.begin(); it != m_dictOutputStats.end(); it++)
		{
			float fValue = it->first;
			if(fValue > m_fStatsMax)
				m_fStatsMax = fValue;
			if(fValue < m_fStatsMin)
				m_fStatsMin = fValue;
		}
		if(fCrtThreshold < m_fStatsMin)
		{
			bIncreaseStride = true;
			float fDelta = m_fStatsMin - fCrtThreshold;
			fRet = fDelta / 6.0f;
		}
		if(fCrtThreshold > m_fStatsMax)
		{
			bIncreaseStride = true;
			float fDelta = fCrtThreshold - m_fStatsMax;
			fRet = fDelta / 6.0f;
		}

		if(m_fStatsMax > m_crtFuncInfo->m_threshold.m_max)
		{
			m_crtFuncInfo->m_threshold.m_max = m_fStatsMax;
		}
	}
	else
	{
		m_fStatsMin = m_crtFuncInfo->m_threshold.m_min;
		m_fStatsMax = m_crtFuncInfo->m_threshold.m_max;
	}
	if(bIncreaseStride)
	{
		if(m_crtFuncInfo->m_threshold.m_type == VT_R4)
		{
			fRet = utils::RoundOffValue(fRet,m_crtFuncInfo->m_threshold.m_stepLen);
		}
		else
		{
			fRet = utils::RoundOffValue2Integer(fRet);
		}
	}

	return fRet;
}

void CChannelConfigDlg::UpdateStatisticsChart()
{
	m_chartCtrl.ShowWindow(SW_HIDE);
	m_chartCtrl.ClearGraph();
	m_dictOutputStats.clear();

	if(m_crtFuncInfo != nullptr  && m_crtFuncInfo->m_bTestFunc == true)
	{

		if(m_pChannelControlPanel != NULL)
		{
			if(m_pChannelControlPanel->GetFuncSpcData(m_crtFuncInfo->m_funcIndex, m_dictOutputStats))
			{
				m_chartCtrl.SetGrahpInfo(m_crtFuncInfo->m_threshold);
				m_chartCtrl.DisplayStatisticsData(m_dictOutputStats);
				m_chartCtrl.Invalidate();
			}
			m_fStrideLen = GetThresholdStrideLength();
		}

	}
	m_chartCtrl.ShowWindow(SW_SHOW);
}

void CChannelConfigDlg::ChangeThresholdValue()
{
	if(m_crtFuncInfo == nullptr)
		return;
	m_chartCtrl.SetThresholdValue(m_crtFuncInfo->m_threshold);
	m_chartCtrl.Invalidate();
}

void CChannelConfigDlg::ProcessPendingParameter()
{
	if(m_bAppliedNewParam == false)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_APPLY_NEW_PARAM);
		if(IDYES == AfxMessageBox(sMsg,MB_YESNO, NULL))
		{
			ApplyNewAlgoParam();
		}
		else
		{
			RestoreOldParam();
		}
		m_bAppliedNewParam = true;
	}
}

void CChannelConfigDlg::OnNMRClickListAlgo(NMHDR *pNMHDR, LRESULT *pResult)
{
	bool bCameraRun = gSystemMgr.IsChannelCameraRunning(m_chConfig.m_chIndex);
	UAC crtUser = gSystemMgr.GetCurrentUser();
	if(bCameraRun || crtUser.m_nLevel != UAC_ADMIN)
	{
		*pResult = 0;
		return;
	}
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nCrtListIndex                  = pNMItemActivate->iItem;

	if(m_nCrtListIndex == -1)
	{
		*pResult = 0;
		return;
	}

	DeleteAllSettingItem();
	m_nCrtRgnID = -1;

	m_lblThreshold_Lcl.SetWindowText(_T("N/A"));
	m_lblThreshold_Lcl.Invalidate();

	EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
	EnableUiBtn(IDB_RIGHT_ARROW, false, IMG_CH_RIGHT_ARROW_GRAY);

	m_chartCtrl.ShowWindow(SW_HIDE);
	m_chartCtrl.ClearGraph();
	m_dictOutputStats.clear();
	m_chartCtrl.ShowWindow(SW_SHOW);

	if(m_nCrtListIndex == -1)
	{
		m_nCrtFuncIndex = -1;
		*pResult = 0;
		return;
	}
	else
	{
		if(m_dictListIndex_FuncIndex.find(m_nCrtListIndex) != m_dictListIndex_FuncIndex.end())
		{
			m_nCrtFuncIndex = m_dictListIndex_FuncIndex[m_nCrtListIndex];
		}
	}
	bool bTestFunc = false;
	if(m_pProcessStation != nullptr)
	{
		auto pFuncInfoArray = m_pProcessStation->GetFuncInfoArrayPtr();
		if(pFuncInfoArray != nullptr)
		{
			FUNC_INFO funcInfo = (*pFuncInfoArray)[m_nCrtFuncIndex];
			bTestFunc = funcInfo.m_bTestFunc;
			m_nCrtRgnID = funcInfo.GetTargetRegionID();
		}
	}

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_MODIFY_TEST_ITEM));
	CMenu* popup = menu.GetSubMenu(0);
	if(m_nCrtListIndex == 0)
	{
		popup->EnableMenuItem(ID_DELETE_REGION,    MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		popup->EnableMenuItem(ID_ADD_TESTITEM,     MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		popup->EnableMenuItem(ID_DELETE_TEST_ITEM, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	if(bTestFunc)
	{
		popup->EnableMenuItem(ID_ADD_REGION,    MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		popup->EnableMenuItem(ID_DELETE_REGION, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	else
	{
		popup->EnableMenuItem(ID_DELETE_TEST_ITEM, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
	ASSERT(popup != NULL);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	popup->Detach();

	*pResult = 0;
}

void CChannelConfigDlg::OnNMClickListAlgo(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_settingType = T_ALGO;
	ShowLightControllerWidget(SW_HIDE);
	CString strTitle;
	strTitle.LoadString(IDS_STRING_ALGO_PARAM);
	m_paramBoard.SetWindowText(strTitle);
	if(m_bSelfLearningMode == true)
	{
		*pResult = 0;
		return;
	}

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int rowIndex = pNMItemActivate->iItem;
	int colIndex = pNMItemActivate->iSubItem;

	if (rowIndex == -1)
	{
		*pResult = 0;
		return;
	}
	
	if(colIndex == 1 && m_dictListIndex_FuncIndex.find(rowIndex) != m_dictListIndex_FuncIndex.end())
	{
		int topindex = m_algoList.GetTopIndex();

		int nFuncIndex = m_dictListIndex_FuncIndex[rowIndex];
		if(m_dictRoiFuncIndex_Folded.find(nFuncIndex) != m_dictRoiFuncIndex_Folded.end())
		{
			bool bFoldedFlag = m_dictRoiFuncIndex_Folded[nFuncIndex];
			m_dictRoiFuncIndex_Folded[nFuncIndex] = !bFoldedFlag;
			m_algoList.DeleteAllItems();
			AddRoiFunc2List();
			RedrawAlgoList();
			
			int itemCount = m_algoList.GetItemCount();
			int testcount = m_algoList. GetCountPerPage();
			
			m_algoList.EnsureVisible(topindex,FALSE);

			if (itemCount - topindex + 1> testcount)
			{
				m_algoList.EnsureVisible(topindex+testcount-1,FALSE);
			}
			else
			{
				m_algoList.EnsureVisible(itemCount-1,FALSE);
			}
		}
	}
	int oldFuncIndex = -1;
	if(m_dictListIndex_FuncIndex.find(m_nAlgoListIndex) != m_dictListIndex_FuncIndex.end())
	{
		oldFuncIndex = m_dictListIndex_FuncIndex[m_nAlgoListIndex];
	}
	int nNewFuncIndex = -1;
	if(m_dictListIndex_FuncIndex.find(rowIndex) != m_dictListIndex_FuncIndex.end())
	{
		nNewFuncIndex = m_dictListIndex_FuncIndex[rowIndex];
	}
	if(oldFuncIndex == nNewFuncIndex)
	{
		*pResult = 0;
		return;
	}
	m_nAlgoListIndex = rowIndex;

	ProcessPendingParameter();
	DeleteAllSettingItem();

	m_crtFuncIndex = m_dictListIndex_FuncIndex[rowIndex];

	std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
	{
		if(pStation != nullptr)
		{
			pStation->ResetDisplayRule();
			pStation->SetCrtSelectedFuncIndex(m_crtFuncIndex);
		}	
	});

	m_algoHelpStrPanel.ShowWindow(SW_HIDE);
	if(-1 != m_crtFuncIndex)
	{
		m_crtFuncInfo      = &(*m_pFuncInfoArray)[m_crtFuncIndex];
		m_crtFuncInfoDebug = &(*m_pDebugFuncInfoArray)[m_crtFuncIndex];

		CreateAlgoParamItems();
		m_bEditUCL = false;
		UpdateStatisticsChart();
		UpdateThresholdPanel();
		if(m_crtFuncInfo != nullptr)
		{
			m_algoHelpStrPanel.SetWindowText(m_crtFuncInfo->m_strHelp.c_str());
		}
	}
	else
	{
		m_algoHelpStrPanel.SetWindowText(_T(""));
		CRect rc;
		m_paramBoard.GetWindowRect(rc);
		RefreshUiCtrl(&m_paramBoard);
	}
	if(m_bDebugMode)
	{
		ProcessCurrentImage();
	}
	m_algoHelpStrPanel.ShowWindow(SW_SHOW);

	*pResult = 0;
}

void CChannelConfigDlg::OnLvnItemchangedListAlgo(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	NM_LISTVIEW* pNMItemActivate =(NM_LISTVIEW*)pNMHDR;	

	int rowIndex = pNMItemActivate->iItem;
	int colIndex = pNMItemActivate->iSubItem;

	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(pCrtTask == nullptr || colIndex != 0 || m_dictListIndex_FuncIndex.find(rowIndex) == m_dictListIndex_FuncIndex.end())
	{
		if(!((pNMItemActivate->uChanged & LVIF_STATE) && (pNMItemActivate->uNewState & LVIS_SELECTED)))
		{
			*pResult = 0;
			return;
		}
	}
	CString sMsg;
	int nFuncIndex = m_dictListIndex_FuncIndex[rowIndex];
	int sz = (int)m_pFuncInfoArray->size();
	if(nFuncIndex >= sz)
	{
		*pResult = TRUE;
		return;		
	}
	///////////////////////////////////////////////////////////////	
	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);
	if(bAdmin==FALSE)
	{
		if((pNMItemActivate->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
			&&(pNMItemActivate->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */) 	
		{			
			if((*m_pFuncInfoArray)[nFuncIndex].m_bEnable==TRUE)
				m_algoList.SetCheck(rowIndex,TRUE);
		}
		else if((pNMItemActivate->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
			&&(pNMItemActivate->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */) 	
		{
			
			if((*m_pFuncInfoArray)[nFuncIndex].m_bEnable==FALSE)
			{				
				m_algoList.SetCheck(rowIndex,FALSE);
			}
		}		
		*pResult = 0;
		return;
	}
	
	///////////////////////////////////////////////////////////////

	FUNC_INFO crtFi = (*m_pFuncInfoArray)[nFuncIndex];

	if((pNMItemActivate->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
	 &&(pNMItemActivate->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */ )
	{
		EnableFunc(nFuncIndex,true,crtFi.m_bTestFunc);
		pCrtTask->EnableTestItem(nFuncIndex,true,crtFi.m_bTestFunc);
		std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
		{
			if(pStation != nullptr)
			{
				pStation->EnableFunc(nFuncIndex,true,crtFi.m_bTestFunc);
			}
		});


		m_algoList.DeleteAllItems();
		AddRoiFunc2List();
	} 
	if((pNMItemActivate->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
	 &&(pNMItemActivate->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */) 	
	{ 
		bool bConfirm = true;
		if(!crtFi.m_bTestFunc || crtFi.m_bTestFunc)
		{
			
			sMsg.LoadString(IDS_STRING_MSG_DISABLE_ROI);
			if(IDNO == AfxMessageBox(sMsg,MB_YESNO, NULL))
			{
				bConfirm = false;
				m_algoList.SetCheck(rowIndex,TRUE);
			}
		}
		if(bConfirm)
		{
			pCrtTask->EnableTestItem(nFuncIndex,false,crtFi.m_bTestFunc);
			EnableFunc(nFuncIndex,false,crtFi.m_bTestFunc);
			std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
			{
				if(pStation != nullptr)
				{
					pStation->EnableFunc(nFuncIndex,false,crtFi.m_bTestFunc);
				}
			});

			if(!crtFi.m_bTestFunc)
			{
				if(m_dictRoiFuncIndex_Folded.find(nFuncIndex) != m_dictRoiFuncIndex_Folded.end() && m_dictRoiFuncIndex_Folded[nFuncIndex] == false)
				{
					m_algoList.DeleteAllItems();
					AddRoiFunc2List();
				}
			}
			
		}
		else
		{
			*pResult = TRUE;
			return;
		}
	}
	if (m_bDebugMode)
	{
		RedrawAlgoList();
	}
	m_bAlgoChanged = true;
	*pResult = 0;
}

void CChannelConfigDlg::EnableFunc(int nFuncIndex, bool bEnable, bool bTestFunc)
{
	int sz = (int)m_pFuncInfoArray->size();
	if(nFuncIndex < sz)
	{
		(*m_pFuncInfoArray)[nFuncIndex].m_bEnable = bEnable;
		(*m_pDebugFuncInfoArray)[nFuncIndex].m_bEnable = bEnable;
		if(!bTestFunc)
		{
			for(int i = nFuncIndex + 1; i < sz; i++)
			{
				if((*m_pFuncInfoArray)[i].m_bTestFunc)
				{
					(*m_pFuncInfoArray)[i].m_bEnable = bEnable;
					(*m_pDebugFuncInfoArray)[i].m_bEnable = bEnable;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			if(bEnable)
			{
				for(int i = nFuncIndex - 1; i > 0; i--)
				{
					if(!(*m_pFuncInfoArray)[i].m_bTestFunc)
					{
						(*m_pFuncInfoArray)[i].m_bEnable = bEnable;
						(*m_pDebugFuncInfoArray)[i].m_bEnable = bEnable;
						break;
					}
				}
			}
		}
	}
}

void CChannelConfigDlg::CreateUiRect(const CRect& rc, int index, CRect& rcName, CRect& rcEdit)
{
	int nNameWidth = 130;

	int nLangID = ::gConfigMgr.GetLanguageID();
	if(nLangID == LANG_ENU)
	{
		nNameWidth = 160;
	}
	rcName.left = rc.left + 8;
	rcName.top =  rc.top + 43 + index * (24 + 5);
	rcName.right = rcName.left + nNameWidth;
	rcName.bottom = rcName.top + 24;

	rcEdit.left = rcName.right + 5;
	rcEdit.top =  rc.top + 40 + index * (24 + 5);
	rcEdit.right = rc.right - 8;
	rcEdit.bottom = rcEdit.top + 24;
}

void CChannelConfigDlg::CreateUiRectForLC(const CRect& rc, int index, CRect& rcName, CRect& rcEdit)
{
	int nNameWidth = 107;

	int nLangID = ::gConfigMgr.GetLanguageID();
	if(nLangID == LANG_ENU)
	{
		nNameWidth = 140;
	}
	rcName.left = rc.left + 8;
	rcName.top =  rc.top + 32 + index * (24 + 5);
	rcName.right = rcName.left + nNameWidth - 1;
	rcName.bottom = rcName.top + 23;

	rcEdit.left = rcName.right;
	rcEdit.top =  rc.top + 30 + index * (24 + 5);
	rcEdit.right = rc.right - 70;
	rcEdit.bottom = rcEdit.top + 24;
}

void CChannelConfigDlg::CreateCameraParamItems()
{
	//判断相机参数是否为任务绑定
	bool bTaskParamAttach = false;
	CAMERA_SETTING cs = m_chConfig.m_vecCameraSetting[m_CameraIndex];
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
	{
		bTaskParamAttach = true;
	}
	if(m_CameraIndex < pCrtTask->m_vecTaskCameraSetting.size())
	{
		CAMERA_SETTING cstmp = cs;
		cs = pCrtTask->m_vecTaskCameraSetting[m_CameraIndex];
		cs.m_triggerAct = cstmp.m_triggerAct;
		cs.m_name = cstmp.m_name;
		cs.m_bTriggerMode = cstmp.m_bTriggerMode;
		cs.m_nFrameRate = cstmp.m_nFrameRate;
	}
	bool bChCameraRun = ::gSystemMgr.IsChannelCameraRunning(m_chConfig.m_chIndex);

	CRect rc;
	m_paramBoard.GetWindowRect(rc);

	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);

	int i = 0;
	CRect rcName, rcEdit;
	CreateUiRect(rc,i++,rcName,rcEdit);
	CString str;
	str.LoadString(IDS_STRING_CAMERA);
	if (m_pStaCameraStation ==NULL)
	{
		m_pStaCameraStation = new CStatic;
		CString strCameraStation;
		strCameraStation.LoadString(IDS_STRING_CameraStation);
		m_pStaCameraStation->Create(strCameraStation,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
		m_pStaCameraStation->SetFont(&m_txtFontPB);
		m_pStaCameraStation->Invalidate();
	}
	else
	{
		m_pStaCameraStation->MoveWindow(rcName);
	}

	if(m_pComCameraStation == NULL)
	{
		m_pComCameraStation = new CComboBox;
		m_pComCameraStation->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, rcEdit, this, IDC_CAMERA_STATION);

		m_pComCameraStation->SetFont(&m_txtFontPB);
		for(int i = 0 ; i < m_VecpProcessStation.size(); i++)
		{
			CString str;
			str.Format(_T("%d"),i);
			m_pComCameraStation->AddString(str);
		}
		m_pComCameraStation->SetCurSel(m_CameraIndex);
		m_pComCameraStation->Invalidate();
	}
	else
	{
		m_pComCameraStation->MoveWindow(rcEdit);
	}
	m_pComCameraStation->EnableWindow(bAdmin && !bChCameraRun);
	m_pStaCameraStation->ShowWindow(SW_SHOW);
	m_pComCameraStation->ShowWindow(SW_SHOW);
	/////////////////////////////////////////////////////////////////////////////////
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_CAMERA);
	CStatic* pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	CEdit* pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	pEdit->SetWindowText(CW2T(cs.m_name.c_str()));
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_ASYNC_MODE);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);
	
	rcEdit.bottom += 22*2;
	CComboBox* pCb = new CComboBox;
	pCb->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, rcEdit, this, IDC_CAMERA_RUN_MODE);
	pCb->EnableWindow(bAdmin && !bChCameraRun);
	pCb->SetFont(&m_txtFontPB);
	str.LoadString(IDS_STRING_TRIGGER_MODE);
	pCb->AddString(str);
	str.LoadString(IDS_STRING_FREE_MODE);
	pCb->AddString(str);
	if(cs.m_bTriggerMode)
		pCb->SetCurSel(0);
	else
		pCb->SetCurSel(1);

	pCb->Invalidate();
	m_vParamEditCtrl.push_back(pCb);
	


	// Frame Rate
	CreateUiRect(rc,i++,rcName,rcEdit);
	rcName.top += 2;
	rcEdit.top += 2;
	str.LoadString(IDS_STRING_FRAME_RATE);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_NUMBER, rcEdit, this, NULL);
	pEdit->EnableWindow(bAdmin && !bChCameraRun);
	pEdit->SetFont(&m_txtFontPB);
	CString sFrameRate;
	sFrameRate.Format(_T("%d"),cs.m_nFrameRate);
	pEdit->SetWindowText(sFrameRate);

	m_vParamEditCtrl.push_back(pEdit);

	if(!bChCameraRun)
	{
		if(cs.m_bTriggerMode)
			pEdit->EnableWindow(FALSE);
		else
		{
			pEdit->EnableWindow(bAdmin);
		}
	}
	else
	{
		pEdit->EnableWindow(FALSE);
	}
//	pEdit->EnableWindow(!bTaskParamAttach);

	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_EDGE_CTRL);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	rcEdit.bottom += 22*2;
	pCb = new CComboBox;
	pCb->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, rcEdit, this, IDC_EDGE_CTRL);
	pCb->EnableWindow(i && bAdmin && !bChCameraRun);
	pCb->SetFont(&m_txtFontPB);
	str.LoadString(IDS_STRING_FALL_EDGE);
	pCb->AddString(str);
	str.LoadString(IDS_STRING_RISE_EDGE);
	pCb->AddString(str);
	if(cs.m_triggerAct == FALL_EDGE)
		pCb->SetCurSel(0);
	else
		pCb->SetCurSel(1);

	pCb->EnableWindow(!bTaskParamAttach);
	pCb->Invalidate();
	m_vParamEditCtrl.push_back(pCb);

	// Camera Gain
	CreateUiRect(rc,i++,rcName,rcEdit);
	rcName.top += 2;
	rcEdit.top += 2;
	str.LoadString(IDS_STRING_CAMERA_GAIN);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_NUMBER, rcEdit, this, NULL);
	pEdit->EnableWindow(i && bAdmin && !bChCameraRun);
	pEdit->SetFont(&m_txtFontPB);

	wstring strGain = boost::lexical_cast<wstring>(cs.m_gain);

	pEdit->SetWindowText(strGain.c_str());
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	//exposureTime
 	CreateUiRect(rc,i++,rcName,rcEdit);
 	rcName.top += 2;
 	rcEdit.top += 2;
 	str.LoadString(IDS_STRING_ExposureTime);
 	pStatic = new CStatic;
 	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
 	pStatic->SetFont(&m_txtFontPB);
 	pStatic->Invalidate();
 	m_vParamNameCtrl.push_back(pStatic);
 
 	pEdit = new CEdit;
 	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_NUMBER, rcEdit, this, NULL);
 	pEdit->EnableWindow(i && bAdmin && !bChCameraRun);
 	pEdit->SetFont(&m_txtFontPB);

	wstring strExpTime = boost::lexical_cast<wstring>(cs.m_ExposureTime);

	pEdit->SetWindowText(strExpTime.c_str());
 	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	// Delay Time
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_DELAY);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);	

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_NUMBER, rcEdit, this, NULL);
	pEdit->EnableWindow(i && bAdmin && !bChCameraRun);
	pEdit->SetFont(&m_txtFontPB);
	
	int nDelayRaw = cs.m_delayValue;
	int nDelay    = (cs.m_delayType == DELAY_TIME) ? int(nDelayRaw / 5.0f) : nDelayRaw;

	wstring	strDelay = boost::lexical_cast<wstring>(nDelay);
	pEdit->SetWindowText(CW2T(strDelay.c_str()));
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	//任务绑定参数
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_REJECTOR_SIGNAL_WIDTH);

	rcName.top+=10;
	m_bCheckTaskAttach.MoveWindow(rcName);
	m_bCheckTaskAttach.ShowWindow(SW_SHOW);
	m_bCheckTaskAttach.EnableWindow(bAdmin && !bChCameraRun);
	m_bCheckTaskAttach.Invalidate();

	//任务参数绑定标记
	m_bCheckTaskAttach.SetCheck(bTaskParamAttach);
}

void CChannelConfigDlg::OnCameraClicked()
{
	if(m_bSelfLearningMode)
	return;

	CWnd* pBtn = GetDlgItem(IDC_STATIC_BTN_DECREASE);
	pBtn->ShowWindow(SW_HIDE);

	pBtn = GetDlgItem(IDC_STATIC_BTN_INCREASE);
	pBtn->ShowWindow(SW_HIDE);

	m_settingType = T_CAMERA;  
	ProcessPendingParameter();
	DeleteAllSettingItem();
	ShowLightControllerWidget(SW_HIDE);
	CString strTitle;
	strTitle.LoadString(IDS_STRING_CAMERA);
	m_paramBoard.SetWindowText(strTitle);

	CreateCameraParamItems();
    RefreshUiCtrl(&m_paramBoard);
}

void CChannelConfigDlg::OnLightClicked()
{
	if(m_bSelfLearningMode)
		return;
	CWnd* pBtn = GetDlgItem(IDC_STATIC_BTN_DECREASE);
	pBtn->ShowWindow(SW_HIDE);

	pBtn = GetDlgItem(IDC_STATIC_BTN_INCREASE);
	pBtn->ShowWindow(SW_HIDE);

	m_settingType = T_LIGHT;
	ProcessPendingParameter();
	DeleteAllSettingItem();
	CString strTitle;
	strTitle.LoadString(IDS_STRING_LIGHT);
	m_paramBoard.SetWindowText(strTitle);

	FillLightControllerWidget();
	UpdateSingleMode();
	SetSingleModeControlShow(true);
	InitLightSingleModeCtrlPos();
	ShowLightControllerWidget(SW_SHOW);
	RefreshUiCtrl(&m_paramBoard);


	LIGHT_SETTING& ls = m_chConfig.m_lightCtrlSetting;

	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(pCrtTask == nullptr)
	{
		return;
	}

	int nIndex = m_comboIDs.GetCurSel();
	if(nIndex >= 0)
	{
		CString sValue;
		m_comboIDs.GetLBText(nIndex, sValue);
		m_nCrtLcID     = (int)utils::to_float(sValue);
		wstring wstrTags(L"");
		if(nIndex < (int)ls.m_vTags.size())
		{
			wstrTags = ls.m_vTags[nIndex];
		}
		DisplayLightControllerParameter(ls,m_chConfig.m_crtTaskName, m_nCrtLcID, wstrTags,true);
	}

}

void CChannelConfigDlg::OnRejectorClicked()
{
	if(m_bSelfLearningMode)
		return;
	CWnd* pBtn = GetDlgItem(IDC_STATIC_BTN_DECREASE);
	pBtn->ShowWindow(SW_HIDE);

	pBtn = GetDlgItem(IDC_STATIC_BTN_INCREASE);
	pBtn->ShowWindow(SW_HIDE);

	m_settingType = T_REJECTOR;
	ProcessPendingParameter();
	DeleteAllSettingItem();
	ShowLightControllerWidget(SW_HIDE);
	CString strTitle;
	strTitle.LoadString(IDS_STRING_REJECTOR);
	m_paramBoard.SetWindowText(strTitle);
	
	REJECTOR_SETTING& rs = m_chConfig.m_rejectorSetting;
	CRect rc;
	m_paramBoard.GetWindowRect(rc);

	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);
	
	int i = 0;
	CRect rcName, rcEdit; 
	// Device IP
	CreateUiRect(rc,i++,rcName,rcEdit);
	CString str;
	str.LoadString(IDS_STRING_DEVICE_IP);
	CStatic* pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	CEdit* pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	CString strValue;
	pEdit->SetWindowText(rs.m_wstrDeviceIp.c_str());
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	//Grab Time
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_GRABTIME);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	strValue.Format(_T("%d"),rs.m_CameraGrabTime);
	pEdit->SetWindowText(strValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);
	
	// Trigger ingore Time
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_TRIGGERINGORETIME);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	strValue.Format(_T("%d"),rs.m_TriggerIngoreTime);
	pEdit->SetWindowText(strValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);
	// Rejection Mode
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_RM_MODE);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);	

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->EnableWindow(FALSE);
	pEdit->SetFont(&m_txtFontPB);
	CString sRjMode;
	if(m_chConfig.m_vecCameraSetting[0].m_delayType == DELAY_DISTANCE)
		sRjMode.LoadString(IDS_STRING_RM_DISTANCE);
	if(m_chConfig.m_vecCameraSetting[0].m_delayType == DELAY_TIME)
		sRjMode.LoadString(IDS_STRING_RM_TIME);
	pEdit->SetWindowText(sRjMode);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	// delay time
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_REJECTOR_DELAY);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);	

	bool bChCameraRun = ::gSystemMgr.IsChannelCameraRunning(m_chConfig.m_chIndex);
	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_NUMBER, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	int nDelayRaw = rs.m_delay;
//	int nDelay = (rs.m_mode == DELAY_TIME) ? int(nDelayRaw / 10.0f) : nDelayRaw;
	int nDelay = nDelayRaw;
	strValue.Format(_T("%d"), nDelay);
	pEdit->SetWindowText(strValue);
	pEdit->EnableWindow(!m_bSelfLearningMode && bAdmin && !bChCameraRun);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	//signal width
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_REJECTOR_SIGNAL_WIDTH);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);
	
	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_NUMBER, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	strValue.Format(_T("%d"), rs.m_signalWidth);
	pEdit->SetWindowText(strValue);
	pEdit->EnableWindow(!m_bSelfLearningMode && bAdmin && !bChCameraRun);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_REJECTOR_CFRAME);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_NUMBER, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	strValue.Format(_T("%d"), rs.m_SignalIngoreTime);
	pEdit->SetWindowText(strValue);	
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_REJECTOR_CDOWN);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_NUMBER, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	strValue.Format(_T("%d"), rs.m_nContinueRejectAlarm);
	pEdit->SetWindowText(strValue);	
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);
    RefreshUiCtrl(&m_paramBoard);
 }

void CChannelConfigDlg::OnDeviceInfoClicked()
{
	if(m_bSelfLearningMode)
		return;
	CWnd* pBtn = GetDlgItem(IDC_STATIC_BTN_DECREASE);
	pBtn->ShowWindow(SW_HIDE);

	pBtn = GetDlgItem(IDC_STATIC_BTN_INCREASE);
	pBtn->ShowWindow(SW_HIDE);

	m_settingType = T_DEVICE;
	DeleteAllSettingItem();
	ShowLightControllerWidget(SW_HIDE);

	CString strTitle;
	strTitle.LoadString(IDS_STRING_CH_STATUS);
	m_paramBoard.SetWindowText(strTitle);

	CHANNEL_STATUS data;
	gSystemMgr.GetChDeviceData(m_chConfig.m_chIndex,data);

	CRect rc;
	m_paramBoard.GetWindowRect(rc);

	int i = 0;
	CRect rcName, rcEdit;

	CString str, sValue;
	//管路速度
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_SPEED);
	CStatic* pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	CEdit* pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	if(data.m_nSpeed == -1)
	{
		sValue = _T("N/A");
	}
	else
	{
		sValue.Format(_T("%d"), data.m_nSpeed);//remarked by hzh
		//sValue.Format(_T("%0.1f"), data.m_nSpeed/10.0); //add by hzh
	}
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	// Sensor count //总件数
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_TOTAL);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	if(data.m_nSensorCount == -1)
	{
		sValue = _T("N/A");
	}
	else
	{
		sValue.Format(_T("%d"), data.m_nSensorCount);
	}
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	//过屏蔽触发计数
	CreateUiRect(rc,i++,rcName,rcEdit);
// 	str.LoadString(IDS_STRING_DROPPED_IMG_COUNT);
// 	str= _T("触发屏蔽");
	str.LoadString(IDS_STRING_NEWRJECT_TRIGGER_INGORE);

	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	sValue.Format(_T("%d"), data.m_nTriggerIgnoreCount);
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	// Dropped Image Count  //丢帧计数
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_DROPPED_IMG_COUNT);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	sValue.Format(_T("%d"), data.m_nDroppedImg);
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	// Rejection Count //剔除
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_REJECTION_COUNT);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	if(data.m_nRejectCount == -1)
	{
		sValue = _T("N/A");
	}
	else
	{
		sValue.Format(_T("%d"), data.m_nRejectCount);
	}
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	//Tracer 当前 LifeID
	CreateUiRect(rc,i++,rcName,rcEdit);

	str.LoadString(IDS_STRING_NEWRJECT_C_OK_COUNT);
	//str = _T("良品计数");
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	if(data.m_nSpeed == -1)
	{
		sValue = _T("N/A");
	}
	else
	{
		sValue.Format(_T("%d"), data.m_nTotalOkCount);
	}
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	/////////////////////new protocol ////////////////////////
	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_NEWRJECT_EOE_SPEED);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	if(data.m_nReject2Count == -1)
	{
		sValue = _T("N/A");
	}
	else
	{
		sValue.Format(_T("%0.2f"), data.m_nEoeSpeed/10.0);
	}
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_NEWRJECT_LOCATION_FAILED);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, CRect(rcName.left,rcName.top,rcName.right+20,rcName.bottom+10), this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	if(data.m_nReject2Count == -1)
	{
		sValue = _T("N/A");
	}
	else
	{
		sValue.Format(_T("%d"), data.m_nPositionError);
	}
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);


	CreateUiRect(rc,i++,rcName,rcEdit);
	str.LoadString(IDS_STRING_NEWRJECT_CURR_PULSE);
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	if(data.m_nReject2Count == -1)
	{
		sValue = _T("N/A");
	}
	else
	{
		sValue.Format(_T("%d"), data.m_nPulse);
	}
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	m_vParamEditCtrl.push_back(pEdit);

	/////定位失败解析标志位
	CreateUiRect(rc,i++,rcName,rcEdit);
	str=L"Locate failure flag:";
	pStatic = new CStatic;
	pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
	pStatic->SetFont(&m_txtFontPB);
	pStatic->Invalidate();
	pStatic->ShowWindow(CLoginDlg::m_bIsSlacAdmin?SW_SHOW:SW_HIDE);
	m_vParamNameCtrl.push_back(pStatic);

	pEdit = new CEdit;
	pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
	pEdit->SetFont(&m_txtFontPB);
	if(data.m_nPositionFailFlag == -1)
	{
		sValue = _T("N/A");
	}
	else
	{
		sValue.Format(_T("0x%x   0x%I64x"), data.m_nPositionFailFlag,data.m_nPositionFailCount);
	}
	pEdit->SetWindowText(sValue);
	pEdit->EnableWindow(FALSE);
	pEdit->Invalidate();
	pEdit->ShowWindow(CLoginDlg::m_bIsSlacAdmin?SW_SHOW:SW_HIDE);
	m_vParamEditCtrl.push_back(pEdit);
	///////// end new protocol/////////

	//多相机工位，相机2，相机3，相机4 丢帧统计
	if (m_VecpProcessStation.size() > 1)
	{
		//相机2丢帧
		CreateUiRect(rc,i++,rcName,rcEdit);
		str.LoadString(IDS_STRING_NEWRJECT_CAME2_LOSTFRAME);
	//	str = _T("相机2丢帧");
		pStatic = new CStatic;
		pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
		pStatic->SetFont(&m_txtFontPB);
		pStatic->Invalidate();
		m_vParamNameCtrl.push_back(pStatic);

		pEdit = new CEdit;
		pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
		pEdit->SetFont(&m_txtFontPB);
		if(data.m_nRejectCount == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nDroppedImg2);
		}
		pEdit->SetWindowText(sValue);
		pEdit->EnableWindow(FALSE);
		pEdit->Invalidate();
		m_vParamEditCtrl.push_back(pEdit);

		//相机3丢帧
		CreateUiRect(rc,i++,rcName,rcEdit);
		str.LoadString(IDS_STRING_NEWRJECT_CAME3_LOSTFRAME);
	//	str = _T("相机3丢帧");
		pStatic = new CStatic;
		pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
		pStatic->SetFont(&m_txtFontPB);
		pStatic->Invalidate();
		m_vParamNameCtrl.push_back(pStatic);

		pEdit = new CEdit;
		pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
		pEdit->SetFont(&m_txtFontPB);
		if(data.m_nRejectCount == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nDroppedImg3);
		}
		pEdit->SetWindowText(sValue);
		pEdit->EnableWindow(FALSE);
		pEdit->Invalidate();
		m_vParamEditCtrl.push_back(pEdit);

		//相机4丢帧
		CreateUiRect(rc,i++,rcName,rcEdit);
		str.LoadString(IDS_STRING_NEWRJECT_CAME4_LOSTFRAME);
	//	str = _T("相机4丢帧");
		pStatic = new CStatic;
		pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
		pStatic->SetFont(&m_txtFontPB);
		pStatic->Invalidate();
		m_vParamNameCtrl.push_back(pStatic);
	
		pEdit = new CEdit;
		pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
		pEdit->SetFont(&m_txtFontPB);
		if(data.m_nRejectCount == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nDroppedImg4);
		}
		pEdit->SetWindowText(sValue);
		pEdit->EnableWindow(FALSE);
		pEdit->Invalidate();
		m_vParamEditCtrl.push_back(pEdit);
	}
	if (m_chConfig.m_rejectorSetting.m_bReRejectFunc)
	{
		//ReReject
		CreateUiRect(rc,i++,rcName,rcEdit);
		str.LoadString(IDS_STRING_REREJECT_COUNT);
		pStatic = new CStatic;
		pStatic->Create(str,WS_CHILD|WS_VISIBLE, rcName, this, NULL);
		pStatic->SetFont(&m_txtFontPB);
		pStatic->Invalidate();
		m_vParamNameCtrl.push_back(pStatic);

		pEdit = new CEdit;
		pEdit->Create(WS_CHILD|WS_VISIBLE, rcEdit, this, NULL);
		pEdit->SetFont(&m_txtFontPB);
		if(data.m_nReject2Count == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nReject2Count);
		}
		pEdit->SetWindowText(sValue);
		pEdit->EnableWindow(FALSE);
		pEdit->Invalidate();
		m_vParamEditCtrl.push_back(pEdit);
	}

	

	RefreshUiCtrl(&m_paramBoard);

	SetTimer(TIMER_ID_DEVICE_DATA,3000,NULL);
}

void CChannelConfigDlg::ZoomSubwindow()
{
	if(m_bSelfLearningMode)
		return;

	if(m_bZoomIn)
	{
		HTuple imgWidth, imgHeight;
		get_image_size(m_crtImage, &imgWidth, &imgHeight);
		set_part(m_nChImgWndID, 0, 0, imgHeight-1, imgWidth-1);
		disp_obj(m_crtImage, m_nChImgWndID);
		m_pProcessStation->GetCrtDebugImgErrorRegion(m_crtErrRgn);
		DisplayErrorRegionOnChWnd(m_crtErrRgn,m_nChImgWndID);

		m_pProcessStation->GetCrtDebugImgTargetRegion(m_crtTargetRgn);
		DisplayErrImgTargetRegionOnChWnd(m_crtTargetRgn,m_nChImgWndID);
	
	}
	m_bZoomIn      = false;
	m_fZoomInRatioTotal = 1.0f;
	m_rcZoonIn.top    = 0;
	m_rcZoonIn.left   = 0;


	size_t iCount = m_VecpProcessStation.size();

	if(m_iCameraIndex > iCount -1)
	{
		return;
	}
	if(!m_bFullScreen)//
	{
		m_bFullScreen = true;
	   m_ScreenOp.SetCurRect(1,0,0,m_rcImgWnd);
	}
	else
	{
	 	m_bFullScreen = false;
	   m_ScreenOp.SetCurRect(iCount,0,0,m_rcImgWnd);
	 }
	 


	 CRect rect;
	 if (m_bFullScreen)
	 {
		 for (int i = 0 ; i < iCount ;i++)
		 {
			 if ( i > m_VecCHImgWndID.size())
			 {
				 return;
			 }
			 if (i == m_iCameraIndex)
			 {
				 rect = m_rcImgWnd;
				 long long CHImgWndID = m_VecCHImgWndID[i];
				 set_window_extents(CHImgWndID,rect.top, rect.left,rect.Width(), rect.Height());

			 }
			 else
			 {
				 rect= CRect(0,0,0,0);
				 long long CHImgWndID = m_VecCHImgWndID[i];
				 set_window_extents(CHImgWndID,rect.top, rect.left,rect.Width(), rect.Height());
			 }
		 }
	 }
	 else
	 {
		 for (int i = 0 ; i < iCount ;i++)
		 {
			 if (m_bSplicing && i != 0)
			 {
				 rect = CRect(0,0,1,1);
			 }
			 else
			 {
				 m_ScreenOp.GetIndexRect(i,rect);
			 }
			 long long CHImgWndID = m_VecCHImgWndID[i];
			 set_window_extents(CHImgWndID,rect.top, rect.left,rect.Width(), rect.Height());
		 }
	 }

	 FramePaint();
	 //refresh//
	 if (m_bDebugMode||m_bSelfLearningMode)
	 {

		 if (m_bSelfLearningMode&&!m_bFullScreen)
		 {
				for (int i = 0;i < m_VecpProcessStation.size(); i++)
				{
					SetSelfProcessIndex(i);
				}
				SetSelfProcessIndex(m_CameraIndex);
		 }
	 }
	 DisplayALLImageOnChWnd();
}

bool CChannelConfigDlg::SetSelfProcessIndex(int index)
{
	if (index > m_VecpProcessStation.size())
	{
		return false;
	}
	m_pProcessStation = m_VecpProcessStation[index];
	m_crtImgIndex = m_vecModelIndex[index];
	m_nImgCount = m_vecModelCount[index];
	return true;

}

bool CChannelConfigDlg::CheckImageIndex(int index)
{
	if(index < 0 || index > m_nImgCount - 1)
	{
		return false;
	}
	return true;
}

void CChannelConfigDlg::UpdateCurrentImgIndex(int index)
{
	CString str;
	str.Format(_T("%d/%d"),++index, m_nImgCount);
	m_lblIndex.SetWindowText(str);
	m_lblIndex.Invalidate();
}

void CChannelConfigDlg::ProcessCurrentImage()
{
	if(m_bLocked && m_bDebugMode && !m_bErrorMode)
	{
		RetestCurrentImage();
		return;
	}
	if(m_nImgCount == 0 || m_crtImgIndex < 0 || m_crtImgIndex >= m_nImgCount)
		return;

	if(m_bSelfLearningMode)
	{
		wstring wstrParam = GetSelfLearningParam();
		m_pProcessStation->DisplayModelImage(m_crtImgIndex,wstrParam);
	}
	else
	{
		RetestCurrentImage();
	}
}

void CChannelConfigDlg::RetestCurrentImage()
{
	if ( m_lockedImgArray.empty()||m_crtImgIndex > m_lockedImgArray.size()-1)
	{
		return;
	}
	int i = 0;
	IMAGE_BOX imgBox;

	m_crtImageGroup = m_lockedImgArray[m_crtImgIndex];

	for(auto it =m_VecpProcessStation.begin();it!= m_VecpProcessStation.end();it++ )
	{
		CImageProcessor* pStation = *it;
		
		m_crtImage = m_crtImageGroup[i].m_img;
		
		if(m_crtImage.Id() == H_EMPTY_REGION)
		{
				i++;
				continue;
		}

		utils::PrintDebugSymbol(m_crtImage);
		if(pStation != nullptr)
		{
		
			imgBox.m_img       = m_crtImage;
			imgBox.m_bDebugImg = true;
			imgBox.m_bSingleDebugImg = m_crtImageGroup[i].m_bSingleDebug;
			imgBox.m_EmtpyImg = m_crtImageGroup[i].m_bEmptyImg;

			m_fZoomInRatioTotal = 1.0f;
			m_rcZoonIn.top = 0;
			m_rcZoonIn.left = 0;

			pStation->ReceiveImage(imgBox);
			i++;
		}
	}

	if (m_iCameraIndex > m_VecpProcessStation.size()-1)
	{
		return;
	}

    m_crtImage = m_crtImageGroup[m_iCameraIndex].m_img;

	//重置放大区域参数值
}

void CChannelConfigDlg::OnImgSourceClicked()
{
	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	CString imgFolder = moduleFileName;
	int index = imgFolder.ReverseFind('\\');
	imgFolder.Truncate(index + 1);
	imgFolder.Append(_T("pictures\\"));
	if(::GetFileAttributes(imgFolder) == INVALID_FILE_ATTRIBUTES)
	{
		::CreateDirectory(imgFolder, NULL);
	}
	CString strFilter = _T("Image Files (*.tiff)|*.tiff|BMP File (*.bmp)|*.bmp|PNG File (*.png)|*.png|All Files (*.*)|*.*||");
	CFileDialog dlgFile(TRUE,_T("tiff"),imgFolder, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT, strFilter, this);
	const unsigned long numberOfFileName = 256;
	const unsigned long fileNameMaxLen = 128;
	const unsigned long bufferSz = (numberOfFileName * fileNameMaxLen) + 1;
	std::vector<TCHAR> fileNameBuffer(bufferSz, 0);
	dlgFile.m_ofn.lpstrFile = &fileNameBuffer[0];
	dlgFile.m_ofn.nMaxFile = bufferSz;

	size_t nProcessCount = m_VecpProcessStation.size();
	m_lockedImgArray.clear();
	m_crtImageGroup.clear();
	if(dlgFile.DoModal() == IDOK)
	{
		bool bSingleDebug = false;
		if (m_VecpProcessStation.size() > 1 )
		{
			if(MessageBox(_T("是否进入单工位调试模式"),_T("提示"),MB_ICONEXCLAMATION|MB_OKCANCEL)==IDOK) 
			{
				bSingleDebug = true;
			}
		}

		m_crtImgIndex = -1;
		POSITION pos = dlgFile.GetStartPosition();
		CString imgFile;
		Hobject curImg;
		m_nImgCount = 0;
		USES_CONVERSION;
		int i = 0;
		while(pos)
		{
			imgFile = dlgFile.GetNextPathName(pos);
			read_image(&curImg, CT2CA(imgFile));
			IMG_PRODUCT IMG;
			IMG.m_bDebug = true;
			IMG.m_img  = curImg;

			if(bSingleDebug) 
			{
				for (int j =  0; j<m_VecpProcessStation.size();j++)
				{
					if (j == m_iCameraIndex)
					{
						IMG.m_bSingleDebug = true;
						IMG.m_bDebug = true;
						m_crtImageGroup.push_back(IMG);
					}
					else
					{
						IMG_PRODUCT N_IMG;
						N_IMG.m_bDebug = true;
						N_IMG.m_bEmptyImg= true;
						m_crtImageGroup.push_back(N_IMG);
					}
					i++;
				}
			}
			else
			{
				m_crtImageGroup.push_back(IMG);
				i++;
			}
			
			if (i==nProcessCount)
			{
				m_lockedImgArray.push_back(m_crtImageGroup);
				i=0;
				m_nImgCount++;
				m_crtImageGroup.clear();
			}
		}
		if(m_nImgCount <= 0)
			return;

		m_crtImgIndex = 0;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
	}
}

void CChannelConfigDlg::OnFirstClicked()
{
	if (m_bFullScreen&&m_bSelfLearningMode)
	{
		if(m_crtImgIndex == 0 || m_nImgCount == 0)
			return;
		m_crtImgIndex = 0;
		m_vecModelIndex[m_iCameraIndex]=0;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
	}
	else if (m_bSelfLearningMode)
	{
		if (m_vecModelIndex.empty())
		{
			return;
		}
		for (size_t i = 0; i < m_VecpProcessStation.size();i++)
		{
			m_crtImgIndex = m_vecModelIndex[i];
			m_pProcessStation = m_VecpProcessStation[i];
			m_nImgCount = m_pProcessStation->GetModelImageCount();

			if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount)
				continue;
			m_crtImgIndex=0;
			m_vecModelIndex[i]=0;
			if(CheckImageIndex(m_crtImgIndex))
			{
				UpdateCurrentImgIndex(m_crtImgIndex);
				ProcessCurrentImage();
			}
		}
		m_pProcessStation = m_VecpProcessStation[m_CameraIndex];
		m_crtImgIndex = m_vecModelIndex[m_CameraIndex];
		m_nImgCount = m_pProcessStation->GetModelImageCount();
		UpdateCurrentImgIndex(m_crtImgIndex);
	}
	else
	{
		if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount)
			return;
		m_crtImgIndex=0;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
	}
}

void CChannelConfigDlg::OnPreviousClicked()
{
	if (m_bFullScreen&&m_bSelfLearningMode)
	{
		if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount)
			return;
		m_crtImgIndex--;
		m_vecModelIndex[m_iCameraIndex]--;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
		else
		{
			m_crtImgIndex++;
			m_vecModelIndex[m_iCameraIndex]++;
		}
	}
	else if (m_bSelfLearningMode)
	{
		if (m_vecModelIndex.empty())
		{
			return;
		}
		for (size_t i = 0; i < m_VecpProcessStation.size();i++)
		{
			m_crtImgIndex = m_vecModelIndex[i];
			m_pProcessStation = m_VecpProcessStation[i];
			m_nImgCount = m_pProcessStation->GetModelImageCount();

			if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount)
				continue;
			m_crtImgIndex--;
			m_vecModelIndex[i]--;
			if(CheckImageIndex(m_crtImgIndex))
			{
				UpdateCurrentImgIndex(m_crtImgIndex);
				ProcessCurrentImage();
			}
			else
			{
				m_crtImgIndex++;
				m_vecModelIndex[i]++;
			}

		}
		m_pProcessStation = m_VecpProcessStation[m_CameraIndex];
		m_crtImgIndex = m_vecModelIndex[m_CameraIndex];
		m_nImgCount = m_pProcessStation->GetModelImageCount();
		UpdateCurrentImgIndex(m_crtImgIndex);
	}
	else
	{
		if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount)
			return;
		m_crtImgIndex--;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
		else
			m_crtImgIndex++;
	}

}

void CChannelConfigDlg::OnNextClicked()
{
	if (m_bFullScreen&&m_bSelfLearningMode)
	{
		if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount - 1)
			return;
		m_crtImgIndex++;
		m_vecModelIndex[m_iCameraIndex]++;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
		else
		{
			m_crtImgIndex--;
			m_vecModelIndex[m_iCameraIndex]--;
		}
	}
	else if (m_bSelfLearningMode)
	{
		if (m_vecModelIndex.empty())
		{
			return;
		}
		for (size_t i = 0; i < m_VecpProcessStation.size();i++)
		{
			m_crtImgIndex = m_vecModelIndex[i];
			m_pProcessStation = m_VecpProcessStation[i];
			m_nImgCount = m_pProcessStation->GetModelImageCount();

			if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount - 1)
				continue;
			m_crtImgIndex++;
			m_vecModelIndex[i]++;
			if(CheckImageIndex(m_crtImgIndex))
			{
				UpdateCurrentImgIndex(m_crtImgIndex);
				ProcessCurrentImage();
			}
			else
			{
				m_crtImgIndex--;
				m_vecModelIndex[i]--;
			}

		}
		m_pProcessStation = m_VecpProcessStation[m_CameraIndex];
		m_crtImgIndex = m_vecModelIndex[m_CameraIndex];
		m_nImgCount = m_pProcessStation->GetModelImageCount();
		UpdateCurrentImgIndex(m_crtImgIndex);
	}
	else
	{
		if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount - 1)
			return;
		m_crtImgIndex++;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
		else
			m_crtImgIndex--;
	}

}

void CChannelConfigDlg::OnLastClicked()
{
	if (m_bFullScreen&&m_bSelfLearningMode)
	{
		if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount - 1)
			return;
		m_crtImgIndex=m_nImgCount - 1;
		m_vecModelIndex[m_iCameraIndex]=m_nImgCount - 1;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
	}
	else if (m_bSelfLearningMode)
	{
		if (m_vecModelIndex.empty())
		{
			return;
		}
		for (size_t i = 0; i < m_VecpProcessStation.size();i++)
		{
			m_crtImgIndex = m_vecModelIndex[i];
			m_pProcessStation = m_VecpProcessStation[i];
			m_nImgCount = m_pProcessStation->GetModelImageCount();

			if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount - 1)
				continue;
			m_crtImgIndex=m_nImgCount - 1;
			m_vecModelIndex[i]=m_nImgCount - 1;
			if(CheckImageIndex(m_crtImgIndex))
			{
				UpdateCurrentImgIndex(m_crtImgIndex);
				ProcessCurrentImage();
			}
		}
		m_pProcessStation = m_VecpProcessStation[m_CameraIndex];
		m_crtImgIndex = m_vecModelIndex[m_CameraIndex];
		m_nImgCount = m_pProcessStation->GetModelImageCount();
		UpdateCurrentImgIndex(m_crtImgIndex);
	}
	else
	{
		if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount - 1)
			return;
		m_crtImgIndex=m_nImgCount - 1;
		if(CheckImageIndex(m_crtImgIndex))
		{
			UpdateCurrentImgIndex(m_crtImgIndex);
			ProcessCurrentImage();
		}
	}
}

void CChannelConfigDlg::OnImgDispModeClicked()
{
	m_btnDispMode.SetLabelImage(IMG_INVALID);
	for (size_t i = 0; i < m_VecpProcessStation.size();i++)
	{
		m_VecpProcessStation[i]->ResetDisplayRule();
	}
	if(m_bErrorMode)
	{
		m_pChannelControlPanel->SetDispAllImageFlag(true);
		m_nImgCount   = 0;
		m_crtImgIndex = -1;
		m_btnDispMode.SetLabelImage(IMG_CH_CONTINOUS_MODE);
		m_bErrorMode = false;
	}
	else
	{
		m_pChannelControlPanel->SetDispAllImageFlag(false);
		m_btnDispMode.SetLabelImage(IMG_CH_SINGLE_MODE);
		m_bErrorMode = true;
	}
	RefreshUiCtrl(&m_btnDispMode);
}

void CChannelConfigDlg::SortImageInChronologicalOrder(vector<Hobject>& vImages, vector<Hobject>& vUnionErrRgn, std::vector<long long>& vTimestamp, int& nCrtIndex)
{
	vector<Hobject>   vTmpImg         = vImages;
	vector<Hobject>   vTmpUnionErrRgn = vUnionErrRgn;
	vector<long long> vTmpTimestamp   = vTimestamp;
	vImages.clear();
	vUnionErrRgn.clear();
	vTimestamp.clear();
	size_t sz1 = vTmpImg.size();
	std::map<long long, Hobject> dictSortedImg;
	std::map<long long, Hobject> dictSortedErrRgn;
	for(size_t t = 0; t < sz1; t++)
	{
		if(vTmpImg[t].Id() != H_EMPTY_REGION)
		{
			nCrtIndex++;
			long long timestamp         = vTmpTimestamp[t];
			dictSortedImg[timestamp]    = vTmpImg[t];
			dictSortedErrRgn[timestamp] = vTmpUnionErrRgn[t];
		}
	}
	nCrtIndex = -1;
	for(auto it = dictSortedImg.begin(); it != dictSortedImg.end(); it++)
	{
		long long nTimestamp = it->first;
		vTimestamp.push_back(nTimestamp);
		vImages.push_back(it->second);
		nCrtIndex++;
		if(dictSortedErrRgn.find(nTimestamp) != dictSortedErrRgn.end())
		{
			vUnionErrRgn.push_back(dictSortedErrRgn[nTimestamp]);
		}
	}
}

void CChannelConfigDlg::SortImageInChronologicalOrderNew(std::vector<std::vector<IMG_PRODUCT>>& NgImgContainer,int& nCrtIndex,long m_errorindex)
{

	if (m_errorindex < 0)
	{
		NgImgContainer.clear();
		return;
	}
	std::vector<std::vector<IMG_PRODUCT>>     TmpNgImgContainer       = NgImgContainer;
	NgImgContainer.clear();
	nCrtIndex = -1;
	size_t sz1 = TmpNgImgContainer.size();
	for (size_t t = m_errorindex+1;t < sz1;t++)
	{
		if(TmpNgImgContainer[t][0].m_img.Id() != H_EMPTY_REGION)
		{
			NgImgContainer.push_back(TmpNgImgContainer[t]);
			nCrtIndex++;
		}
	}

	for (size_t t=0;t <m_errorindex+1;t++)
	{
		if(TmpNgImgContainer[t][0].m_img.Id() != H_EMPTY_REGION)
		{
			NgImgContainer.push_back(TmpNgImgContainer[t]);
			nCrtIndex++;
		}
	}
}

void CChannelConfigDlg::LockCurrentImage()
{
	m_crtImage.Reset();
	if(m_bErrorMode)
	{
		int iErrorindex = 0;
		m_lockedImgArray = m_pChannelControlPanel->GetNgImgArray(iErrorindex);
		SortImageInChronologicalOrderNew(m_lockedImgArray,m_crtImgIndex,iErrorindex);
		m_nImgCount = (int)m_lockedImgArray.size();
		m_crtImgIndex    = m_nImgCount - 1;
		if(m_nImgCount > 0)
		{
			m_crtImageGroup = m_lockedImgArray[m_crtImgIndex];
			SetImageIndexPanelText(m_crtImgIndex + 1, m_nImgCount);
			ProcessCurrentImage();
		}
	}
	else // Continuous Mode
	{
		m_nImgCount = 0;
		m_lockedImgArray.clear();
		m_lockedImgArray = m_pChannelControlPanel->GetContiguousImgArray();
		m_nImgCount      = (int)m_lockedImgArray.size();
		m_crtImgIndex    = m_nImgCount - 1;
		if(m_nImgCount > 0)
		{
			m_crtImageGroup = m_lockedImgArray[m_crtImgIndex];
			SetImageIndexPanelText(m_crtImgIndex + 1, m_nImgCount);
			ProcessCurrentImage();
		}
	}
}

void CChannelConfigDlg::SetImageIndexPanelText(int index, int total)
{
	CString strIndex;
	if(total <= 0)
		strIndex = _T("N/A");
	else
	{
		if(index > total)
			index = total;
		strIndex.Format(_T("%d/%d"), index, total);
	}
	m_lblIndex.SetWindowText(strIndex);
	RefreshUiCtrl(&m_lblIndex);
}

void CChannelConfigDlg::OnLockClicked()
{
	CRect rc;
	m_btnLock.GetWindowRect(rc);
	m_btnLock.SetLabelImage(IMG_INVALID);
	m_crtImgIndex = -1;
	m_nImgCount   = 0;
	if(m_bLocked == false)
	{
		m_bLocked    = true;
		m_bDebugMode = true;
		CWnd* pCtl = GetDlgItem(IDC_CHECK_READ_FILE);
		pCtl->EnableWindow(FALSE);
		pCtl->Invalidate();
		LockCurrentImage();
		m_btnLock.SetLabelImage(IMG_CH_LOCKED);
	}
	else
	{
		m_btnLock.SetLabelImage(IMG_CH_UNLOCKED);
		m_bLocked    = false;
		m_bDebugMode = false;
		CWnd* pCtl = GetDlgItem(IDC_CHECK_READ_FILE);
		pCtl->EnableWindow(TRUE);
		pCtl->Invalidate();
		SetImageIndexPanelText(0,0);
	}
	m_pChannelControlPanel->SetDebugFlag(m_bDebugMode);

	EnableDisplayModeButton(!m_bLocked);
	if(m_bLocked && m_crtImage.Id() != H_EMPTY_REGION)
	{
		EnableUiBtn(IDB_ZOOM_IN, m_bLocked, IMG_CH_ZOOM_IN);
		EnableUiBtn(IDB_SAVE_IMG, m_bLocked, IMG_CH_SAVE_IMG);
		EnableUiBtn(IDB_SAVE_ALL_IMG, m_bLocked, IMG_CH_SAVE_ALL_IMG);
		if(m_bSelfLearningMode)
			EnableUiBtn(IDB_DELETE_CRT_IMG, m_bLocked, IMG_CH_DELETE_MODEL_IMG);
		else
			EnableUiBtn(IDB_ADD_MODEL_IMG, m_bLocked, IMG_CH_ADD_MODEL_IMG);
	}
	else
	{
//		EnableUiBtn(IDB_ZOOM_IN, m_bLocked, IMG_CH_ZOOM_IN_GRAY);
		EnableUiBtn(IDB_SAVE_IMG, m_bLocked, IMG_CH_SAVE_IMG_GRAY);
		EnableUiBtn(IDB_SAVE_ALL_IMG, m_bLocked, IMG_CH_SAVE_ALL_IMG_GRAY);

		if(m_bSelfLearningMode)
			EnableUiBtn(IDB_DELETE_CRT_IMG, m_bLocked, IMG_CH_DELETE_MODEL_IMG_GRAY);
		else
			EnableUiBtn(IDB_ADD_MODEL_IMG, m_bLocked, IMG_CH_ADD_MODEL_IMG_GRAY);
	}
	RefreshUiCtrl(&m_btnLock);
}

void CChannelConfigDlg::OnZoomInClicked()
{
	//颠倒管检测预留代码
#ifdef CAN_PRODUCT
	if (m_chConfig.m_bSingleProcessor &&m_chConfig.m_vecCameraSetting.size()>1)
	{
		return;
	}
#endif

	if(!m_bDebugMode)
	{
		return;
	}

	if (m_VecpProcessStation.size()>1&&!m_bFullScreen)
	{
		return ;
	}
	
}
void CChannelConfigDlg::DisplayImageWndZoomOut(CPoint point)
{
	if (m_VecpProcessStation.size()>1 && !m_bFullScreen)
	{
		return;
	}
	//判断是否有图片
	if(m_rcImgWnd.PtInRect(point))
	{
		if(m_crtImage.Id() != H_EMPTY_REGION)
		{
			HTuple imgWidth, imgHeight;
			get_image_size(m_crtImage, &imgWidth, &imgHeight);
			if(m_rcZoonIn.left == 0 && m_rcZoonIn.top == 0)
			{
				m_rcZoonIn.right  = imgWidth[0].I() /m_fZoomInRatioTotal+1 ;
				m_rcZoonIn.bottom = imgHeight[0].I() /m_fZoomInRatioTotal+1;
			}

			HTuple wndRow, wndCol, wndWidth, wndHeight;
			get_window_extents(m_nChImgWndID, &wndRow, &wndCol, &wndWidth, &wndHeight);

			float xRatio = (float)wndWidth[0].I() / m_rcZoonIn.Width();
			float yRatio = (float)wndHeight[0].I() / m_rcZoonIn.Height();

			int deltaColumn = int((point.x - wndCol[0].I()) / xRatio);
			int deltaRow    = int((point.y - wndRow[0].I()) / yRatio);

			if (deltaColumn == 0 && deltaRow == 0 )
			{
				return;
			}
			float fZoomInRatio = 2;
			m_fZoomInRatioTotal *= fZoomInRatio;

			if (m_fZoomInRatioTotal> 1.0f)
			{
				m_bZoomIn = true;
				m_hCursor = m_hcursor_fingers_hand;
				::SetCursor(m_hCursor);
			}

			int rowBgn     = m_rcZoonIn.top + int(deltaRow * (1.0f - 1.0f/fZoomInRatio));
			int rowEnd     = m_rcZoonIn.top + int(deltaRow + (m_rcZoonIn.Height() - deltaRow) * (1.0f/fZoomInRatio));
			int columnBgn  = m_rcZoonIn.left + int(deltaColumn *(1.0f - 1.0f/fZoomInRatio));
			int columnEnd  = m_rcZoonIn.left + int(deltaColumn +  (m_rcZoonIn.Width() - deltaColumn) * (1/fZoomInRatio));

			if((rowBgn >= rowEnd)||(columnBgn>= columnEnd))
				return;

			m_rcZoonIn.top    = rowBgn;
			m_rcZoonIn.bottom = rowEnd;
			m_rcZoonIn.left   = columnBgn;
			m_rcZoonIn.right  = columnEnd;

			DisplayZoomDebugObject();
		}
	}
}
void CChannelConfigDlg::DisplayImageWndZoomIn(CPoint point)
{
	if (m_VecpProcessStation.size()>1 && !m_bFullScreen)
	{
		return;
	}

	if( m_rcImgWnd.PtInRect(point))
	{
		if(m_crtImage.Id() != H_EMPTY_REGION)
		{
			HTuple imgWidth, imgHeight;
			get_image_size(m_crtImage, &imgWidth, &imgHeight);
			if(m_rcZoonIn.left == 0 && m_rcZoonIn.top == 0)
			{
				m_rcZoonIn.right  = imgWidth[0].I() /m_fZoomInRatioTotal +1 ;
				m_rcZoonIn.bottom = imgHeight[0].I() /m_fZoomInRatioTotal +1;
			}

			HTuple wndRow, wndCol, wndWidth, wndHeight;
			get_window_extents(m_nChImgWndID, &wndRow, &wndCol, &wndWidth, &wndHeight);

			float xRatio = (float)wndWidth[0].I() / m_rcZoonIn.Width();
			float yRatio = (float)wndHeight[0].I() / m_rcZoonIn.Height();

			int deltaColumn = int((point.x - wndCol[0].I()) / xRatio);
			int deltaRow    = int((point.y - wndRow[0].I()) / yRatio);

			float fZoomInRatio = 0.5f;
	
			if (m_fZoomInRatioTotal*fZoomInRatio< 1.0f)
			{
				//缩小至原图，鼠标指针恢复，直接return;
				m_hCursor =  LoadCursor(NULL, IDC_ARROW);
				::SetCursor(m_hCursor);	
				return;//
			}
			m_fZoomInRatioTotal *= fZoomInRatio;

			int rowBgn,rowEnd,columnBgn,columnEnd;
			if (m_fZoomInRatioTotal <= 1.0f)
			{
				m_bZoomIn = false;
				rowBgn = 0;
				columnBgn = 0;
				columnEnd  = imgWidth[0].I() /m_fZoomInRatioTotal ;
				rowEnd = imgHeight[0].I() /m_fZoomInRatioTotal;
			}
			else
			{
				rowBgn     = m_rcZoonIn.top + int(deltaRow * (1.0f - 1.0f/fZoomInRatio));
				rowEnd     = m_rcZoonIn.top + int(deltaRow + (m_rcZoonIn.Height() - deltaRow) * (1.0f/fZoomInRatio));
				columnBgn  = m_rcZoonIn.left + int(deltaColumn *(1.0f - 1.0f/fZoomInRatio));
				columnEnd  = m_rcZoonIn.left + int(deltaColumn +  (m_rcZoonIn.Width() - deltaColumn) * (1/fZoomInRatio));

				if((rowBgn >= rowEnd)||(columnBgn>= columnEnd))
					return;
			}

			if (rowBgn<0)
			{
				rowEnd -= rowBgn;
				rowBgn =0;
			}
			
			if (rowEnd > imgHeight[0].I() )
			{
				rowBgn = rowBgn- ( rowEnd-imgHeight[0].I() );
				rowEnd =  imgHeight[0].I();
			}

			if (columnBgn<0)
			{
				columnEnd -= columnBgn;
				columnBgn =0;
			}

			if (columnEnd > imgWidth[0].I() )
			{
				columnBgn = columnBgn- ( columnEnd-imgWidth[0].I() );
				columnEnd =  imgWidth[0].I() ;
			}
			
			m_rcZoonIn.top    = rowBgn;
			m_rcZoonIn.bottom = rowEnd;
			m_rcZoonIn.left   = columnBgn;
			m_rcZoonIn.right  = columnEnd;
			
			DisplayZoomDebugObject();
		}
	}
}
void CChannelConfigDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_rcImgWnd.PtInRect(point))
	{
		if(m_crtImage.Id() != H_EMPTY_REGION)
		{
			if (m_bZoomIn)
			{
				m_bDragZoomImage = false;
				m_rcZoonIn =m_rcZoonInDragEnd;
				m_hCursor = m_hcursor_fingers_hand;
				::SetCursor(m_hCursor);
			}
			else
			{
				m_hCursor =  LoadCursor(NULL, IDC_ARROW);
				::SetCursor(m_hCursor);
			}
		}
	}
	else
	{
		if (m_bZoomIn)
		{
			m_bDragZoomImage = false;
			m_rcZoonIn =m_rcZoonInDragEnd;
		}

		m_hCursor =  LoadCursor(NULL, IDC_ARROW);
		::SetCursor(m_hCursor);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CChannelConfigDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bZoomIn && m_rcImgWnd.PtInRect(point))
	{
		if(m_crtImage.Id() != H_EMPTY_REGION)
		{
 			if (m_fZoomInRatioTotal >1.0f)  //图片放大状态
 			{
				m_DragBeginPoint = point;
				m_bDragZoomImage = true;

				m_hCursor = m_hcursor_hand;
				::SetCursor(m_hCursor);
			}
		}
	}

	if(!m_bFullScreen)
	{
		if(m_rcImgWnd.PtInRect(point))
		{
			m_iCameraIndex = m_ScreenOp.GetRectIndex(point);
			if (m_iCameraIndex < m_crtImageGroup.size())
			{
				m_crtImage = m_crtImageGroup[m_iCameraIndex].m_img;
				m_nChImgWndID = m_VecCHImgWndID[m_iCameraIndex];
				m_hChImgWnd= m_VecCHImgWnd[m_iCameraIndex];
				m_pProcessStation = m_VecpProcessStation[m_iCameraIndex];
			}

			FramePaint();

			if (m_bSelfLearningMode)
			{
				//切换当前当前自学习线程！
				m_pProcessStation = m_VecpProcessStation[m_iCameraIndex];
				m_crtImgIndex = m_vecModelIndex[m_iCameraIndex];
				m_nImgCount = m_vecModelCount[m_iCameraIndex];
				
				//切换相机通道时候，自学习数据变化
				UpdateCurrentImgIndex(m_crtImgIndex);
			}
		}
	}
	
	if(m_settingType == T_CAMERA&&(!m_pChannelControlPanel->IsChCameraRunning()))
	{
		m_pComCameraStation->SetCurSel(m_iCameraIndex);
		OnCameraClicked();
	}
	else if (m_settingType == T_ALGO)
	{
		SetFileButtonImage();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CChannelConfigDlg::FramePaint()
{
	CRect crt;
	for (int i = 0 ; i < m_VecpProcessStation.size(); i++ )
	{

		::GetWindowRect(m_VecCHImgWnd[i],&crt);


		CClientDC dc(this);
		CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		CPen penFrame;
		CPen* pOldPen ;

		if (m_bFullScreen)
		{
					crt.left-=1;crt.right+=1;crt.top-=1;crt.bottom+=1;
					penFrame.CreatePen(PS_SOLID,3, RGB(200,220,250));
		}
		else
		{
					crt.left-=2;crt.right+=2;crt.top-=2;crt.bottom+=2;
					penFrame.CreatePen(PS_SOLID, 2, RGB(200,220,250));
		}
	
		pOldPen = dc.SelectObject(&penFrame);
		dc.Rectangle(crt);
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);
	}	

	if (m_bFullScreen)
	{
		return;
	}

	::GetWindowRect(m_VecCHImgWnd[m_iCameraIndex],&crt);
	crt.left-=2;crt.right+=2;crt.top-=2;crt.bottom+=2;
	CClientDC dc(this);
	CBrush* pOldBrush = (CBrush *) dc.SelectStockObject(NULL_BRUSH);
	CPen penFrame;
	CPen *pOldPen;

	penFrame.CreatePen(PS_SOLID,2, RGB(35,255,35));
	pOldPen = dc.SelectObject(&penFrame);
	dc.Rectangle(crt);

	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
}

void CChannelConfigDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bFullScreen)
	{
		if(m_rcImgWnd.PtInRect(point))
		{
			m_iCameraIndex = m_ScreenOp.GetRectIndex(point);
			if (m_iCameraIndex < m_crtImageGroup.size())
			{
				m_crtImage = m_crtImageGroup[m_iCameraIndex].m_img;
				m_nChImgWndID = m_VecCHImgWndID[m_iCameraIndex];
				m_hChImgWnd= m_VecCHImgWnd[m_iCameraIndex];
			}
		}
	}

	if(m_bZoomIn)
	{
		m_hCursor =  LoadCursor(NULL, IDC_ARROW);
		::SetCursor(m_hCursor);

		HTuple imgWidth, imgHeight;
				
		get_image_size(m_crtImage, &imgWidth, &imgHeight);

		m_rcZoonIn.top    = 0;
		m_rcZoonIn.left   = 0;
		m_rcZoonIn.right =imgWidth[0].I();
		m_rcZoonIn.bottom= imgHeight[0].I();
		DisplayZoomDebugObject();
	}
	m_bZoomIn      = false;
	m_fZoomInRatioTotal = 1.0f;
	m_rcZoonIn.top    = 0;
	m_rcZoonIn.left   = 0;

	CDialogEx::OnRButtonDown(nFlags, point);
}

void CChannelConfigDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	//颠倒管检测预留代码
// #ifdef CAN_PRODUCT
// 	if (m_chConfig.m_bSingleProcessor &&m_chConfig.m_vecCameraSetting.size()>1)
// 	{
// 		return;
// 	}
// #endif

	if(!m_bDebugMode || point == m_oldPt)
	{
		return;
	}

	if (m_VecpProcessStation.size()>1 && !m_bFullScreen)
	{
		return;
	}
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_GRAY_VALIUE);
	pStatic->ShowWindow(SW_HIDE);

	if(m_crtImage.Id() != H_EMPTY_REGION)
	{
		if (m_bZoomIn)
		{
			if (m_bDragZoomImage)  //拖动放大图片
			{
				m_hCursor = m_hcursor_hand;
				::SetCursor(m_hCursor);

				CPoint m_DragEndPoint = point;

				int xOffset = m_DragEndPoint.x - m_DragBeginPoint.x;
				int yOffset = m_DragEndPoint.y - m_DragBeginPoint.y;

				HTuple imgWidth, imgHeight;
				get_image_size(m_crtImage, &imgWidth, &imgHeight);
				if(m_rcZoonIn.left == 0 && m_rcZoonIn.top == 0)
				{
					m_rcZoonIn.right  = imgWidth[0].I() /m_fZoomInRatioTotal +1;
					m_rcZoonIn.bottom = imgHeight[0].I() /m_fZoomInRatioTotal+1;
				}

				HTuple wndRow, wndCol, wndWidth, wndHeight;
				get_window_extents(m_nChImgWndID, &wndRow, &wndCol, &wndWidth, &wndHeight);

				float xRatio = (float)wndWidth[0].I() / m_rcZoonIn.Width();
				float yRatio = (float)wndHeight[0].I() / m_rcZoonIn.Height();

				int deltaColumn = int(xOffset / xRatio);
				int deltaRow    = int(yOffset/ yRatio);

				int rowBgn     = m_rcZoonIn.top -deltaRow;
				int rowEnd     = rowBgn + m_rcZoonIn.Height();
				int columnBgn  = m_rcZoonIn.left - deltaColumn;
				int columnEnd  = columnBgn + m_rcZoonIn.Width() ;

				if (rowBgn<0)
				{
					rowEnd -= rowBgn;
					rowBgn =0;
				}

				if (rowEnd > imgHeight[0].I() )
				{
					rowBgn = rowBgn- ( rowEnd-imgHeight[0].I() );
					rowEnd =  imgHeight[0].I();
				}

				if (columnBgn<0)
				{
					columnEnd -= columnBgn;
					columnBgn =0;
				}

				if (columnEnd > imgWidth[0].I() )
				{
					columnBgn = columnBgn- ( columnEnd-imgWidth[0].I() );
					columnEnd =  imgWidth[0].I() ;
				}

// 				if((rowBgn >= rowEnd)||(columnBgn>= columnEnd))
// 					return;
// 
 				m_rcZoonInDragEnd.top    = rowBgn;
 				m_rcZoonInDragEnd.bottom = rowEnd;
 				m_rcZoonInDragEnd.left   = columnBgn;
 				m_rcZoonInDragEnd.right  = columnEnd;

				DisplayZoomDebugObject();

			}
			else
			{
				if (!m_rcImgWnd.PtInRect(point))
				{
					m_hCursor =  LoadCursor(NULL, IDC_ARROW);
					::SetCursor(m_hCursor);
				}
				else
				{
					m_hCursor =  m_hcursor_fingers_hand;
					::SetCursor(m_hCursor);
				}
	
			}
		}
		else
		{
			m_hCursor =  LoadCursor(NULL, IDC_ARROW);
			::SetCursor(m_hCursor);
		}
	
		if (!m_rcImgWnd.PtInRect(point)|| m_crtImage.Id() == H_EMPTY_REGION)
		{
			return;
		}
		//针对大分辨率图像的显示缩放问题
		HTuple imgWidth, imgHeight;
		int i_imgWidth = 0;
		int i_imgHeight = 0;
		float  m_fZoomInRatioTotalX = 1;
		float  m_fZoomInRatioTotalY = 1;
		get_image_size(m_crtImage, &imgWidth, &imgHeight);
		
		i_imgWidth = imgWidth[0].I();
		i_imgHeight= imgHeight[0].I();

		int imgDispWndW = m_rcImgWnd.Width();
		int imgDispWndH = m_rcImgWnd.Height();

		m_fZoomInRatioTotalX = (float)i_imgWidth / imgDispWndW;
	    m_fZoomInRatioTotalY =  (float)i_imgHeight / imgDispWndH;

		m_oldPt  = point;
		int nRow = point.y - m_rcImgWnd.top;
		int nCol = point.x - m_rcImgWnd.left;

		float fRow = nRow;
		float fCol = nCol;

		double retValue;
		Hobject ImageOneChannel;
		if (m_rcZoonIn.top==0 &&m_rcZoonIn.left == 0)
		{
			m_rcZoonIn.bottom = (i_imgHeight/m_fZoomInRatioTotal)+1;
			m_rcZoonIn.right = (i_imgWidth/m_fZoomInRatioTotal)+1;
		}
			 
 		fRow = m_rcZoonIn.top  +(float) (point.y - m_rcImgWnd.top)  /imgDispWndH*(m_rcZoonIn.Height());
 		fCol = m_rcZoonIn.left + (float)(point.x - m_rcImgWnd.left) /imgDispWndW*(m_rcZoonIn.Width());
  		nRow=(int)fRow;
 		nCol=(int)fCol; 
		
		//异常消失判断 防止get_grayval 异常导致调试界面消失
		if (nRow<0||nRow>i_imgHeight-1|| nCol <0 || nCol > i_imgWidth-1)
		{
			return;
		}
		CString str;
		str.Format(_T("(%d,%d)"),nRow, nCol);

		m_pProcessStation->GetCrtDebugInterImageObject(m_crtInterImage);
	
		if (m_crtInterImage.Id() == H_EMPTY_REGION)
		{
			Hobject ZoomShowImage;
			m_pProcessStation->GetCrtDebugImageObject(ZoomShowImage);

			//灰度值颠倒罐适配
			Hobject img = ZoomShowImage;
			if(m_bMultiChannnelImg)
			{
				select_obj(ZoomShowImage,&img,m_nCrtImgChannel);
			}

			HTuple nImageChannelCount;
			count_channels(img, &nImageChannelCount);

			for (int i = 0 ; i <nImageChannelCount[0].I() ;i++)
			{
				access_channel (img, &ImageOneChannel, i+1);
				HTuple imgWidth1, imgHeight1;
				get_image_size(ImageOneChannel, &imgWidth1, &imgHeight1);
				int ii_imgWidth = imgWidth1[0].I();
				int ii_imgHeight= imgHeight1[0].I();
				if (i_imgWidth==ii_imgWidth && i_imgHeight== ii_imgHeight)
				{
						get_grayval(ImageOneChannel, nRow, nCol, &retValue);
						long nGray = (long)retValue;
						CString strGray;
						strGray.Format(_T(",%d"),nGray);
						str+=strGray;
				}
			}
		}
		else
		{
			HTuple nImageChannelCount;
			count_channels(m_crtInterImage, &nImageChannelCount);

			//灰度值颠倒罐适配
			Hobject img = m_crtInterImage;
			if(m_bMultiChannnelImg)
			{
				select_obj(m_crtInterImage,&img,m_nCrtImgChannel);
			}

			for (int i = 0 ; i <nImageChannelCount[0].I() ;i++)
			{
				access_channel (img, &ImageOneChannel, i+1);
				HTuple imgWidth1, imgHeight1;
				get_image_size(ImageOneChannel, &imgWidth1, &imgHeight1);
				int ii_imgWidth = imgWidth1[0].I();
				int ii_imgHeight= imgHeight1[0].I();
				if (i_imgWidth==ii_imgWidth && i_imgHeight== ii_imgHeight)
				{
					get_grayval(ImageOneChannel, nRow, nCol, &retValue);

					long nGray = (long)retValue;
					CString strGray;
					strGray.Format(_T(",%d"),nGray);
					str+=strGray;
				}
			}
		}

		pStatic->SetWindowText(str);
		pStatic->ShowWindow(SW_SHOW);
	}
	else
	{
		//不在图片区域内，鼠标还原
		m_hCursor =  LoadCursor(NULL, IDC_ARROW);
		::SetCursor(m_hCursor);

		pStatic->SetWindowText(_T(""));
	}
	CRect rc;
	pStatic->GetWindowRect(rc);
	RedrawWindow(rc, NULL, RDW_INTERNALPAINT|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN); 
}

int CALLBACK  BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)  
{  
    switch(uMsg)  
    {  
    case BFFM_INITIALIZED:
        ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData); 
        break;  
    default:  
        break;  
    }  
    return 0;     
}

CString CChannelConfigDlg::SetTargetImageFilePath(bool bSingleImg)
{
	TCHAR  moduleFileName[MAX_PATH];  
	::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
	(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
	CString strDataFolder = moduleFileName;
	strDataFolder.Append(_T("data"));
	std::wstring strDefPath = CT2W(strDataFolder);

	CString strPath(_T(""));
	if(bSingleImg)
	{
		CString strFilter(_T("Image Files (*.tiff)|*.tiff||"));
		CFileDialog fileDlg(FALSE, _T("tiff"),_T("current_image"), OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, strFilter, this);
		if(fileDlg.DoModal() == IDOK)
		{
			strPath = fileDlg.GetPathName();
		}
	}
	else
	{
		BROWSEINFO  bi;
		bi.hwndOwner       = NULL;
		bi.pidlRoot        = NULL;
		bi.pszDisplayName  = NULL;
		bi.lpszTitle       = NULL;
		bi.ulFlags         = 0;
		bi.lpfn            = NULL;
		bi.iImage          = 0;
		if(PathFileExistsW(strDefPath.c_str()))
		{
			bi.lpfn        = BrowseCallbackProc;
			bi.lParam      = (LPARAM)strDefPath.c_str();
		}

		LPCITEMIDLIST pidl = SHBrowseForFolder(&bi);
		if(!pidl)
			return strPath;
		TCHAR  szDisplayName[255];
		SHGetPathFromIDList(pidl,szDisplayName);
		strPath = szDisplayName;
	}
	return strPath;
}

void CChannelConfigDlg::DisplayPromptMessage(CString strMsg, bool bStatus)
{
	m_bOpRslt = bStatus;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_WARNING_PANEL);
	pWnd->SetWindowText(strMsg);
	RefreshUiCtrl(pWnd);
	SetTimer(TIMER_ID,5000, NULL);
}

void CChannelConfigDlg::OnSaveImgClicked()
{
	CString strPath = SetTargetImageFilePath(true);
	for(int i = 0; i < m_crtImageGroup.size(); i++)
	{
		string tmpPath ;
		if(m_crtImageGroup[i].m_img.Id() == H_EMPTY_REGION)
			continue;
		CString strID;
		strID.Format(_T("_%d"),i);

		CString strPathtmp = strPath;
		int index = strPathtmp.ReverseFind('.');
		strPathtmp.Insert(index,strID);
		tmpPath = CT2A(strPathtmp);
		write_image(m_crtImageGroup[i].m_img, "tiff", 0, tmpPath.c_str());
	}
}

void CChannelConfigDlg::OnSaveAllImgClicked()
{
	if(!m_lockedImgArray.empty())
	{
		CString sPath = SetTargetImageFilePath(false);
		if(sPath.GetLength() <= 0)
			return;

		string strFolder = CT2A(sPath);
		int index = 0;

		std::for_each(m_lockedImgArray.begin(), m_lockedImgArray.end(),[&](std::vector<IMG_PRODUCT> ImgContainer)
		{
			CString sFileName;
			sFileName.Format(_T("%s\\%.3d.tiff"), sPath,index++);
			for(int i = 0; i < ImgContainer.size(); i++)
			{
				string tmpPath ;
				if(ImgContainer[i].m_img.Id() == H_EMPTY_REGION)
					continue;
				CString strID;
				strID.Format(_T("_%d"),i);
				 
				CString strPathtmp = sFileName;
				int index = strPathtmp.ReverseFind('.');
				strPathtmp.Insert(index,strID);
				tmpPath = CT2A(strPathtmp);
				write_image(ImgContainer[i].m_img, "tiff", 0, tmpPath.c_str());
			}
		});
	}
	else
	{
		CString strMsg;
		strMsg.LoadString(IDS_STRING_NO_ERROR_IMAGE);
		DisplayPromptMessage(strMsg,false);
	}
}

void CChannelConfigDlg::OnDeleteModelImgClicked()
{
	CString strMsg;
	if(m_nImgCount == 0)
	{
		strMsg.LoadString(IDS_STRING_NO_MODEL_IMAGE);
		DisplayPromptMessage(strMsg, false);
		return;
	}
	strMsg.LoadString(IDS_STRING_DELETE_THIS_MODEL_IMAGE);
	if(IDNO == AfxMessageBox(strMsg, MB_YESNO , NULL))
		return;
	if(m_crtImgIndex <= m_nImgCount-1)
	{
		if(!m_pProcessStation->DeleteModeImage(m_crtImgIndex))
			return;
		m_nImgCount--;
		m_vecModelCount[m_iCameraIndex]--;

		if(m_crtImgIndex > m_nImgCount - 1)
			m_crtImgIndex = m_nImgCount - 1;

		m_vecModelIndex[m_iCameraIndex]= m_crtImgIndex;

		UpdateCurrentImgIndex(m_crtImgIndex);
		wstring wstrParam = GetSelfLearningParam();
		m_pProcessStation->DisplayModelImage(m_crtImgIndex, wstrParam);

		strMsg.LoadString(IDS_STRING_OPERATION_OK);
		DisplayPromptMessage(strMsg, true);
		return;
	}
	strMsg.LoadString(IDS_STRING_OPERATION_NG);
	DisplayPromptMessage(strMsg, false);
}

void CChannelConfigDlg::OnAddModelImgClicked()
{
	CString strMsg;
	if(m_bDebugMode)
	{
		if(m_crtImage.Id() == H_EMPTY_REGION)
		{
			strMsg.LoadString(IDS_STRING_NO_MODEL_IMAGE);
			DisplayPromptMessage(strMsg, false);
			return;
		}
		CString sFolder = SetTargetImageFilePath(false);
		if(sFolder.GetLength() <= 0)
		{
			return;
		}

		if(!m_pProcessStation->AddModelImage(m_crtImage, sFolder))
		{
			strMsg.LoadString(IDS_STRING_OPERATION_NG);
			DisplayPromptMessage(strMsg, false);
		}
		else
		{
			strMsg.LoadString(IDS_STRING_OPERATION_OK);
			DisplayPromptMessage(strMsg, true);
		}
	}
	else
	{
		strMsg.LoadString(IDS_STRING_OPERATION_NG);
		DisplayPromptMessage(strMsg, false);
	}
}

void CChannelConfigDlg::OnResetStatsDataClicked()
{
	m_chartCtrl.ClearGraph();
	RefreshUiCtrl(&m_chartCtrl);

	gSystemMgr.SaveLeftoverStatisticsData(m_chConfig.m_chIndex);
	if(m_pChannelControlPanel != nullptr)
	{
		m_pChannelControlPanel->m_chSD.ExecuteCmd(QOP_DATA_RESET,RESET_STATS);
	}
	m_dictOutputStats.clear();

	for (size_t i = 0; i < m_VecpProcessStation.size();i++)
	{
		m_VecpProcessStation[i]->ResetDisplayRule();
	}

	std::for_each(m_vFuncInfo.begin(),m_vFuncInfo.end(), [&](FUNC_INFO& fi)
	{
		fi.m_nTotalCount  = 0;
		fi.m_nErrCount  = 0;
		fi.m_bPass  = true;

		while(!fi.m_VecRecentErrCount.empty())
		{
			fi.m_VecRecentErrCount.pop();
		}
	});
	OnUpdateChannelScreen(NULL,NULL);

	gSystemMgr.ResetChannelStats(m_chConfig.m_chIndex);

	UpdateData(FALSE);
}

void CChannelConfigDlg::EnableUiWidget(vector<CWnd*> vCtrl, bool bEnable)
{
	size_t sz = vCtrl.size();
	if(sz < 1) return;

	CRect rc;
	for(size_t t = 1; t < sz; t++)
	{
		CWnd* pCtrl = vCtrl[t];
		pCtrl->EnableWindow(bEnable);
		pCtrl->GetWindowRect(rc);
		InvalidateRect(rc);
	}
}

void CChannelConfigDlg::OnOnOffButtonClicked()
{
	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);

	int nChIndex = m_chConfig.m_chIndex;
	bool bCameraRun  = gSystemMgr.IsChannelCameraRunning(nChIndex);
	if(bCameraRun)
	{
		gSystemMgr.PauseChannelCamera(nChIndex);
		m_btnResetStatsData.EnableWindow(TRUE);
		m_btnResetStatsData.SetLabelImage(IMG_CH_RESET_STATS_DATA);
		m_btnResetStatsData.Invalidate();
		bCameraRun  = gSystemMgr.IsChannelCameraRunning(nChIndex);
		if(!bCameraRun)
		{
			m_btnOnOff.SetLabelImage(IMG_CH_CAMERA_PAUSE);
			if(m_settingType == T_CAMERA)
			{
				OnCameraClicked();
			}
		}
		PostMessage(WM_UPDATE_ALGO_LIST,0,0);
	}
	else
	{
		gSystemMgr.TurnOnChannelCamera(nChIndex);
		m_btnResetStatsData.EnableWindow(FALSE);
		m_btnResetStatsData.SetLabelImage(IMG_CH_RESET_STATS_DATA_GRAY);
		m_btnResetStatsData.Invalidate();

		bCameraRun  = gSystemMgr.IsChannelCameraRunning(nChIndex);
		if(bCameraRun)
		{
			m_btnOnOff.SetLabelImage(IMG_CH_CAMERA_RUN);
			if(m_settingType == T_CAMERA)
			{
				EnableUiWidget(m_vParamEditCtrl, false);
 				if (m_pComCameraStation!= NULL)
 				{
					m_pComCameraStation->EnableWindow(false);
				}
			}
		}
	}
	m_btnOnOff.Invalidate();
}

void CChannelConfigDlg::OnCbnSelchangeCameraStation()
{
	if(m_vParamEditCtrl.size() != 7)
		return;

	int i = m_pComCameraStation->GetCurSel();
	SetCurCameraIndex(i);
	if(m_settingType == T_CAMERA)
	{
		OnCameraClicked();
	}
}

void CChannelConfigDlg::OnCbnSelchangeRunMode()
{
	if(m_vParamEditCtrl.size() != 7)
		return;
	CComboBox* p = (CComboBox*)m_vParamEditCtrl[1];
	int tm = p->GetCurSel();
	bool bTriggerMode = (tm == 0?true:false);
	if(bTriggerMode != m_chConfig.m_vecCameraSetting[m_CameraIndex].m_bTriggerMode)
	{
		m_btnApplyParam.EnableWindow(TRUE);
	}
	CEdit* pEdit = (CEdit*)m_vParamEditCtrl[2];
	if(pEdit == nullptr)
	{
		return;
	}
	pEdit->EnableWindow(tm == 1);
	UpdateData(FALSE);
}

void CChannelConfigDlg::OnCbnSelchangeEdgeCtrl()
{
	if(m_vParamEditCtrl.size() != 7)
		return;
	CComboBox* p = (CComboBox*)m_vParamEditCtrl[2];
	int ta = p->GetCurSel();
	eTRIGGER_ACTIVATION triggerAct = (ta==0? FALL_EDGE:RISE_EDGE);
	if(triggerAct != m_chConfig.m_vecCameraSetting[m_CameraIndex].m_triggerAct)
	{
		m_btnApplyParam.EnableWindow(TRUE);
	}
}

void CChannelConfigDlg::ApplyNewAlgoParam()
{
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	
	if(m_crtFuncInfoDebug != nullptr)
	{
		m_crtFuncInfo = m_crtFuncInfoDebug;
		std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
		{
			if(pStation != nullptr)
			{
				pStation->ApplyNewParameter(m_crtFuncInfo->m_dispId, m_crtFuncInfo->m_funcIndex);
			}
		});

		if(!pCrtTask->UpdateParameter(m_crtFuncInfo->m_funcIndex,m_crtFuncInfo->m_vParam))
		{
			::gLogger.QueueUpLog(L"CChannelConfigDlg::ApplyNewAlgoParam ## pCrtTask->UpdateParameter(....)  FAILED!");
		}
	}

//	m_pFuncInfoArray = m_pProcessStation->GetDebugFuncInfoArrayPtr();
//  m_pDebugFuncInfoArray 已更新！
//	m_crtFuncInfo->assign(m_pDebugFuncInfoArray->begin(), m_pDebugFuncInfoArray->end());
	m_pFuncInfoArray->assign(m_pDebugFuncInfoArray->begin(), m_pDebugFuncInfoArray->end());
	
	CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
	*pChConfig = m_chConfig;

	m_bAlgoChanged = true;
}

void CChannelConfigDlg::RestoreOldParam()
{
	if(m_crtFuncInfo != nullptr)
	{
		for (size_t i = 0; i < m_VecpProcessStation.size();i++)
		{
			m_VecpProcessStation[i]->RestoreDebugParameter(m_crtFuncInfo->m_dispId, m_crtFuncInfo->m_funcIndex);
		}
	}
	m_pDebugFuncInfoArray = m_pProcessStation->GetDebugFuncInfoArrayPtr();
}

void CChannelConfigDlg::OnBnClickedBtnApplyParam()
{
	SetFocus();
	if(m_settingType == T_CAMERA)
	{
		bool bTaskParamAttach = false;
		TASK* pCrtTask = m_chConfig.GetCurrentTask();
		if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
		{
			bTaskParamAttach = true;
		}

		GetNewCameraSetting();

		if (bTaskParamAttach)
		{
			TASK* pCrtTask = m_chConfig.GetCurrentTask();
			if(!pCrtTask->m_vecTaskCameraSetting[m_CameraIndex].IsEqual(m_crtCS))
			{
				pCrtTask->m_vecTaskCameraSetting[m_CameraIndex] = m_crtCS;

				//任务绑定参数和公用配置参数 对于相机模式统一！
				m_chConfig.m_vecCameraSetting[m_CameraIndex].m_bTriggerMode = m_crtCS.m_bTriggerMode;
				m_chConfig.m_vecCameraSetting[m_CameraIndex].m_nFrameRate = m_crtCS.m_nFrameRate;
			}
			
		}
		else
		{
			if(!m_chConfig.m_vecCameraSetting[m_CameraIndex].IsEqual(m_crtCS))
			{
				m_chConfig.m_vecCameraSetting[m_CameraIndex] = m_crtCS;
			}
		}
		gSystemMgr.UpdateCameraSetting(m_chConfig.m_chIndex,m_crtCS);
		m_bCsChanged = true;
	}
	if(m_settingType == T_REJECTOR)
	{
		GetNewRejectorSetting();
		if(!m_chConfig.m_rejectorSetting.IsEqual(m_crtRS))
		{
			m_chConfig.m_rejectorSetting = m_crtRS;
            m_crtRS.m_mode = m_chConfig.m_vecCameraSetting[0].m_delayType;
			gSystemMgr.UpdateRejectorSetting(m_chConfig.m_chIndex,m_crtRS);
			m_bRsChanged = true;
		}
	}
	if(m_settingType == T_ALGO)
	{
		ApplyNewAlgoParam();
		m_bAppliedNewParam  = true;
	}
	if(m_settingType == T_UI)
	{
		//TODO:
	}
	CString sMsg;
	sMsg.LoadString(IDS_STRING_OPERATION_OK);
	DisplayPromptMessage(sMsg, true);
	m_btnApplyParam.EnableWindow(FALSE);
}

void CChannelConfigDlg::SavaChannelSettings2ConfigFile()
{
	if(m_bCsChanged)
	{
		bool bTaskParamAttach = false;
		TASK* pCrtTask = m_chConfig.GetCurrentTask();
		
		if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
		{
			bTaskParamAttach = true;
		}
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
			gConfigMgr.UpdateTaskCameraSetting(m_chConfig.m_chIndex,pCrtTask->m_wstrTaskName, strTaskCameraSetting);

			//相机任务参数绑定模式下,相机模式保持一致！
			m_chConfig.m_vecCameraSetting[m_CameraIndex].m_bTriggerMode = pCrtTask->m_vecTaskCameraSetting[m_CameraIndex].m_bTriggerMode;
			m_chConfig.m_vecCameraSetting[m_CameraIndex].m_nFrameRate = pCrtTask->m_vecTaskCameraSetting[m_CameraIndex].m_nFrameRate;
			gConfigMgr.UpdateCameraSetting(m_chConfig.m_chIndex, &(m_chConfig.m_vecCameraSetting[m_CameraIndex]));	
		}
		else
		{
			gConfigMgr.UpdateTaskCameraSetting(m_chConfig.m_chIndex,pCrtTask->m_wstrTaskName, "");
			gConfigMgr.UpdateCameraSetting(m_chConfig.m_chIndex, &m_crtCS);
		}

		CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
		*pChConfig = m_chConfig;

		m_bCsChanged = false;
	}
	if(m_bLsChanged)
	{
		gConfigMgr.UpdateLightControllerParam(m_chConfig.m_chIndex, &m_chConfig.m_lightCtrlSetting,true);
		m_bLsChanged = false;
	}
	if(m_bRsChanged) 
	{
		gConfigMgr.UpdateRejectorSetting(m_chConfig.m_chIndex,&m_crtRS);
		m_bRsChanged = false;
	}
	
	if(m_bAlgoChanged)
	{
		TASK* pCrtTask = m_chConfig.GetCurrentTask();
		if(m_bSelfLearningMode)
		{
			wstring wstrParam = GetSelfLearningParam();
			gConfigMgr.UpateSelfLearningParam(m_chConfig, wstrParam);
		}

		if(!gConfigMgr.UpdateTask(m_chConfig.m_chIndex, *pCrtTask, false))
		{
			::gLogger.QueueUpLog(L"CChannelConfigDlg::SavaChannelSettings2ConfigFile ## gConfigMgr.UpdateTask(....)  FAILED!");
		}
		// Update Channel Configuration;
		CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
		*pChConfig = m_chConfig;
		m_bAlgoChanged = false;
	}
}

void CChannelConfigDlg::OnCloseScreen()
{
 	m_bExitFlag = true;
 	KillTimer(TIMER_ID_DLG_READY);
 	long long nID = -1;

	m_pChannelControlPanel->SetChannelImageWindowPtr(nullptr);

	std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
	{
		if(pStation != nullptr)
		{
			pStation->SetChannelWindowOpenedFlag(false);
		}
	});

	Sleep(200);

	for (int i = 0; i < m_VecCHImgWndID.size(); i ++)
	{
		close_window(m_VecCHImgWndID[i]);
	}
	m_VecCHImgWnd.clear();
	m_VecCHImgWndID.clear();

	if(m_bLsChanged)
	{
		LC lc = GetLightControllerParameter();
		WriteNewLcParameter2Device();
		TASK* pCrtTask = m_chConfig.GetCurrentTask();
		if(pCrtTask != nullptr)
		{
			gSystemMgr.UpdateChLightController(m_chConfig.m_chIndex, m_chConfig.m_lightCtrlSetting);
		}
	}
	SavaChannelSettings2ConfigFile();
	gSystemMgr.RestoreSerialPortOwnerWindow();
	CDialogEx::OnOK();
}

bool CChannelConfigDlg::GetNewCameraSetting()
{
	size_t sz = m_vParamEditCtrl.size();
	assert(sz == 7);
	m_crtCS = m_chConfig.m_vecCameraSetting[m_CameraIndex];

	CComboBox* p = (CComboBox*)m_vParamEditCtrl[1];
	int tm = p->GetCurSel();
	m_crtCS.m_bTriggerMode = (tm == 0?true:false);

	CEdit* pEdit = (CEdit*)m_vParamEditCtrl[2];
	CString sFrameRate;
	pEdit->GetWindowText(sFrameRate);

	p = (CComboBox*)m_vParamEditCtrl[3];
	int ta = p->GetCurSel();
	m_crtCS.m_triggerAct = (ta==0? FALL_EDGE:RISE_EDGE);
	CString sGain;
	CString sDelay;
	CString sExpTime;
	m_vParamEditCtrl[4]->GetWindowText(sGain);
	m_vParamEditCtrl[5]->GetWindowText(sExpTime);
	m_vParamEditCtrl[6]->GetWindowText(sDelay);
	CString sMsg;
	if(!utils::IsNumericString(sFrameRate) || !utils::IsNumericString(sGain) || 
		!utils::IsNumericString(sDelay)|| !utils::IsNumericString(sExpTime))
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
		DisplayPromptMessage(sMsg,false);
		return false;
	}
	if(!sFrameRate.IsEmpty() && !sGain.IsEmpty() && !sDelay.IsEmpty()&& !sExpTime.IsEmpty())
	{
		try
		{
			int nFrameRate = (int)utils::to_float(sFrameRate);
			int nGain      = (int)utils::to_float(sGain);
			int nDelay     = (int)utils::to_float(sDelay);
			int nExpTime   = (int)utils::to_float(sExpTime);
			if(nFrameRate < 0 || nGain < 0 || nDelay < 0)
			{
				sMsg.LoadString(IDS_STRING_PARAM_CANNOT_BE_NEGATIVE);
				DisplayPromptMessage(sMsg,false);
				return false;
			}
			m_crtCS.m_nFrameRate = nFrameRate;
			m_crtCS.m_gain       = nGain;
			m_crtCS.m_ExposureTime      = nExpTime;
			if(m_crtCS.m_delayType == DELAY_TIME)
			{
				m_crtCS.m_delayValue = nDelay * 5;
			}
			else
			{
				m_crtCS.m_delayValue = nDelay;
			}
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
	return false;
}

bool CChannelConfigDlg::ValidateAlgoParameter(CWnd* pEdit, CString sValue, const INPUT_PARAM& inputParam)
{
	if(inputParam.m_type == VT_BSTR)
	{
		return true;
	}
	CString sMsg;
	bool isNumeric = utils::IsNumericString(sValue);
	if(!isNumeric)
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
		DisplayPromptMessage(sMsg,false);
		return false;
	}
	wstring str = CT2W(sValue);
	float fValue = boost::lexical_cast<float>(str);
	if(fValue != inputParam.m_value)
	{
		float max = inputParam.m_max;
		float min = inputParam.m_min;
		if(fValue > max || fValue < min)
		{
			sMsg.LoadString(IDS_STRING_PARAM_VALUE_OUT_RANGE);
			CString sMinMax;
			if(inputParam.m_type == VT_R4)
			{
				sMinMax.Format(_T("%.1f~%.1f"),  min, max);
			}
			else
			{
				sMinMax.Format(_T("%d~%d"), (int)min, (int)max);
			}
			sMsg.Replace(_T("#~#"),sMinMax);

			CString sOldValue;
			if(inputParam.m_type == VT_R4)
			{
				sOldValue.Format(_T("%.1f"),inputParam.m_value);
			}
			else
			{
				sOldValue.Format(_T("%d"),inputParam.m_value);
			}
			pEdit->SetWindowText(sOldValue);
			pEdit->SetFocus();
			DisplayPromptMessage(sMsg,false);
			return false;
		}
	}
	return true;
}

bool CChannelConfigDlg::GetNewAlgoParameters(vector<INPUT_PARAM>& vNewParam,vector<INPUT_PARAM>& vConfigParam)
{
	if(m_vParamEditCtrl.empty())
	{
		return false;
	}
	CString sValue;
	CString sMsg;
	int nBtnIndex = 0;
	if(m_crtFuncInfo != nullptr) // center location algorithm is selected
	{
		auto vParam = m_crtFuncInfo->m_vParam;
		for(size_t i = 0; i < m_vParamEditCtrl.size(); i++)
		{
			auto pWnd = m_vParamEditCtrl[i];
			pWnd->GetWindowText(sValue);
			if(!ValidateAlgoParameter(pWnd, sValue, vParam[i]))
			{
				vNewParam.clear();
				vConfigParam.clear();
				return false;
			}
			wstring wstrValue = CT2W(sValue);
			INPUT_PARAM newValue = vParam[i];
			INPUT_PARAM ConfigValue = vParam[i];
			if(newValue.m_type == VT_BSTR)
			{
				int nBtnID = GetFileEditBtnID(nBtnIndex);
				newValue.m_value = 0.0f;
				ConfigValue.m_value = 0.0f;
				if(m_dictBtnID_File.find(nBtnID) != m_dictBtnID_File.end())
				{
				    newValue.m_wstrFile = m_dictBtnID_File[nBtnID];
					ConfigValue.m_wstrFile = m_dictBtnID_File[nBtnID];
					if (m_bModelFiles)
					{
						ModelFilesAdaptToPath(ConfigValue.m_wstrFile);
					}

				}
				else
				{
					newValue.m_wstrFile = L" ";
					ConfigValue.m_wstrFile = L" ";
				}
				nBtnIndex++;
			}
			else
			{
				float fValue      = boost::lexical_cast<float>(wstrValue);
				newValue.m_value  = fValue;
				ConfigValue.m_value = fValue;
			}
			vNewParam.push_back(newValue);
			vConfigParam.push_back(ConfigValue);
		}
	}
	return true;
}

bool CChannelConfigDlg::ParameterChanged(DISPID& funcId, int funcIndex, const std::vector<INPUT_PARAM>& vCrtParam)
{
	for(int i = 0; i < m_pDebugFuncInfoArray->size(); i++)
	{
		if(funcId == (*m_pDebugFuncInfoArray)[i].m_dispId && funcIndex == (*m_pDebugFuncInfoArray)[i].m_funcIndex)
		{
			FUNC_INFO fi = (*m_pDebugFuncInfoArray)[i];
			if(fi.m_vParam.size() != vCrtParam.size())
			{
				return true;
			}
			for(size_t t = 0; t < vCrtParam.size(); t++)
			{
				if(!fi.m_vParam[t].IsEqual(vCrtParam[t]))
				{
					return true;
				}
			}
			break;
		}
	}
	return false;
}

void CChannelConfigDlg::UpdatevNewParamPath(std::vector<INPUT_PARAM> &vNewParam,int index)
{
	wostringstream oss;  
	oss<<L"#"<<index;
	std::wstring wstr=oss.str(); 

	for ( auto it = vNewParam.begin();it != vNewParam.end();it++)
	{
		INPUT_PARAM paramUnit = *it;
		if( it->m_type == VT_BSTR)
		{
			wstring *pstr = &(it->m_wstrFile);
			if (!pstr->empty())
			{
				int i = pstr->find(_T("#"));
				int j = pstr->find_last_of(_T("#"));
				if (i != string::npos && j != string::npos)
				{
					pstr->replace(j,2,wstr);
					pstr->replace(i,2,wstr);
				}
			}
		}
	}
}

void CChannelConfigDlg::OnBnClickedOk()
{
	if(m_settingType == T_INVALID)
	{
		return;
	}
	if(m_bSelfLearningMode)
	{
		if (m_bFullScreen)
		{
			ProcessCurrentImage();
			return;
		}
		if (m_vecModelIndex.empty())
		{
			return;
		}
		for (size_t i = 0; i < m_VecpProcessStation.size();i++)
		{
			m_crtImgIndex = m_vecModelIndex[i];
			m_pProcessStation = m_VecpProcessStation[i];
			m_nImgCount = m_pProcessStation->GetModelImageCount();

			if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount - 1)
				continue;

			if(CheckImageIndex(m_crtImgIndex))
			{
				UpdateCurrentImgIndex(m_crtImgIndex);
				ProcessCurrentImage();
			}
		}
		m_pProcessStation = m_VecpProcessStation[m_CameraIndex];
		m_crtImgIndex = m_vecModelIndex[m_CameraIndex];
		m_nImgCount = m_pProcessStation->GetModelImageCount();
		UpdateCurrentImgIndex(m_crtImgIndex);

		m_bAlgoChanged = true;
		return;
	}

	bool bParamChanged = false;
	if(m_settingType == T_ALGO)
	{
		if(m_bThresholdChanged)
		{
			m_editThreshold.PostMessage(WM_KILLFOCUS,0,0);
		}
		TASK* pCrtTask = m_chConfig.GetCurrentTask();

		std::vector<INPUT_PARAM> vNewParam;
		std::vector<INPUT_PARAM> vConfigParam;
		if(GetNewAlgoParameters(vNewParam,vConfigParam) && !vNewParam.empty())
		{			
			if(ParameterChanged(m_crtFuncInfo->m_dispId, m_crtFuncInfo->m_funcIndex, vNewParam))
			{				
				int i = 0;
				for (auto it = m_VecpProcessStation.begin();it != m_VecpProcessStation.end();it++)
				{
					CImageProcessor* pStation = *it;
					if(pStation != nullptr)
					{
						UpdatevNewParamPath(vNewParam,i);
						pStation->ChangeDebugParameter(m_crtFuncInfo->m_dispId, m_crtFuncInfo->m_funcIndex, vNewParam);						
					}
					i++;
				}
				
				if (m_pChannelControlPanel!=NULL)
				{
					m_pChannelControlPanel->ChangeDebugParameter(m_crtFuncInfo->m_dispId, m_crtFuncInfo->m_funcIndex, vNewParam);
				}
				
				m_crtFuncInfoDebug->m_vParam  = vNewParam;

				m_bAppliedNewParam = false;
				bParamChanged = true;
			}
		}
		if(m_bDebugMode && bParamChanged)
		{
			RetestCurrentImage();
		}
	}
	if(m_settingType == T_CAMERA)
	{
		if(GetNewCameraSetting() && !m_crtCS.IsEqual(m_chConfig.m_vecCameraSetting[m_CameraIndex]))
			bParamChanged = true;
	}
	if(m_settingType == T_REJECTOR)
	{
		if(GetNewRejectorSetting() && !m_crtRS.IsEqual(m_chConfig.m_rejectorSetting))
			bParamChanged = true;
	}
	if (bParamChanged)
	{
		m_btnApplyParam.EnableWindow(bParamChanged);
	}

	KillTimer(TIMER_ID_FOR_EDIT_CTRL);
	SetTimer(TIMER_ID_FOR_EDIT_CTRL, 30000, NULL);
}

HBRUSH CChannelConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT id = pWnd->GetDlgCtrlID();

	auto it1 = std::find(m_vChSwitchId.begin(), m_vChSwitchId.end(), id);
	auto it2 = std::find(m_vParamNameCtrl.begin(), m_vParamNameCtrl.end(), pWnd);
	auto it3 = std::find(m_vLcWidgetPtr.begin(), m_vLcWidgetPtr.end(), pWnd);
	auto it4 = std::find(m_vLcStaticPtr.begin(),m_vLcStaticPtr.end(), pWnd);
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		if(id == IDC_STATIC_HELP_STRING 
			|| id == IDC_STATIC_CH_RUN_MODE 
			|| id == IDC_STATIC_PARAM_GP
			|| id == IDC_STATIC_WARNING_PANEL
			|| id == IDC_CHECK_READ_FILE
			|| id == IDC_STATIC_READ_IMG
			|| id == IDC_CHECK_DISP_ERROR_RGN
			|| id == IDC_STATIC_DISP_ERROR_RGN
			|| id == IDC_STATIC_PARAM_HELP_STRING
			|| id == IDC_SYSLINK_ALGO_HELP
			|| id == IDC_STATIC_GRAY_VALIUE
			|| id == IDC_CHECK_LIGHT_SINGLEMODE
			|| id == IDC_CHECK_TASK_Param
			|| id == IDC_RADIO_CAMERA1
			|| id == IDC_RADIO_CAMERA2
			|| it1 != m_vChSwitchId.end()
			|| it2 != m_vParamNameCtrl.end() 
			|| it3 != m_vLcWidgetPtr.end()
			|| it4 != m_vLcStaticPtr.end())
		{
			pDC->SetBkMode(TRANSPARENT);
			CRect rc;
			pWnd->GetWindowRect(rc);
			ScreenToClient(&rc);
			CDC* dc = GetDC();
			if(id == IDC_STATIC_CH_RUN_MODE)
			{
				pDC->SetTextColor(RGB(200,20,20));
			}
			if(id == IDC_SYSLINK_ALGO_HELP)
			{
				pDC->SetTextColor(RGB(1,1,255));
			}
			if(id == IDC_STATIC_READ_IMG 
			|| id == IDC_STATIC_DISP_ERROR_RGN
			|| id == IDC_STATIC_PARAM_HELP_STRING
			|| id == IDC_STATIC_GRAY_VALIUE)
			{
				pDC->SetTextColor(RGB(255,255,255));
			}

			if(id == IDC_STATIC_WARNING_PANEL)
			{
				if(m_bOpRslt)
					pDC->SetTextColor(RGB(20,250,20));
				else
					pDC->SetTextColor(RGB(200,20,20));
			}
			pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
			if(id == IDC_STATIC_HELP_STRING)
			{
				pDC->SetBkColor(RGB(200,200,200));
				ReleaseDC(dc);
				return m_bgBrush;
			}
			ReleaseDC(dc);
			return HBRUSH(GetStockObject(NULL_BRUSH));	
		}
	}
	return hbr;
}
void CChannelConfigDlg::ResizeOtherControl()
{
	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);

	CRect rcClient;
	GetClientRect(&rcClient);

	int vtBtnSz = 64;
	int gap     = 5;
	int listW   = 500;

	if(rcWorkArea.Width() == 1920)
	{
		listW   = int(listW * 1.15f);
	}

	float fRate = 1080.0f / rcWorkArea.Height();
	int titleAreaH = int(75.0f / fRate);

	int imgDispWndW = m_chConfig.m_uiSetting.m_width;
	int imgDispWndH = m_chConfig.m_uiSetting.m_height;

	CRect crt;
	crt = m_ScreenOp.GetPosRect();
	imgDispWndW = crt.Width();
	imgDispWndH = crt.Height();


	CRect rc;
	rc.left = rcClient.left + gap + vtBtnSz + gap + imgDispWndW + gap;
	rc.top = titleAreaH + vtBtnSz + 8;
	rc.right = rc.left + listW;
	rc.bottom = rc.top + imgDispWndH - 58;

	m_algoList.MoveWindow(rc);

	CRect rcHelp;
	rcHelp.left   = rc.left + 1;
	rcHelp.top    = rc.bottom;
	rcHelp.right  = rc.right - 1;
	rcHelp.bottom = rc.top + imgDispWndH;
	m_algoHelpStrPanel.SetFont(&m_txtFontPB);
	m_algoHelpStrPanel.MoveWindow(rcHelp);


	/////////////////////
	InitParameterBoard(rcWorkArea, rcClient);
	OnCbnSelchangeComboLcOption();
	InitApplyParamButton(rcWorkArea);
	InitSaveTrainningResultButton(rcWorkArea);
	InitSpcChart(rcWorkArea);
	InitChannelSwitch(rcWorkArea);
	InitCameraSwitch(rcWorkArea);

	int x_sz = rcWorkArea.Width();  
	int y_sz = rcWorkArea.Height();
	::SetWindowPos(m_hWnd,HWND_TOP,0,0,x_sz, y_sz, SWP_SHOWWINDOW);

	//自学习相关按钮显示隐藏

	int hzBtnSz = 64; //  Horizontal Button Size;
	int gapH = 2;

	CRect rcBtn = m_SelfrcBtn;
	if (m_bSelfLearningFunc){
		if (!m_btnAddModelImg.GetSafeHwnd())
		{
			m_btnAddModelImg.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_ADD_MODEL_IMG);
			m_btnAddModelImg.SetLabelImage(IMG_CH_ADD_MODEL_IMG_GRAY);
			m_btnAddModelImg.EnableWindow(FALSE);
			m_btnAddModelImg.Invalidate();
		}

		if(!m_btnDelCrtImg.GetSafeHwnd())
		{
			m_btnDelCrtImg.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcBtn, this, IDB_DELETE_CRT_IMG);
			m_btnDelCrtImg.SetLabelImage(IMG_CH_DELETE_MODEL_IMG_GRAY);
			m_btnDelCrtImg.EnableWindow(FALSE);
			m_btnDelCrtImg.Invalidate();
		}
	}
	if (m_bSelfLearningFunc)
	{
		rcBtn.left = rcBtn.right + gapH;
		rcBtn.right = rcBtn.left + hzBtnSz;

		m_btnAddModelImg.MoveWindow(rcBtn);
		m_btnDelCrtImg.MoveWindow(rcBtn);

		if(m_bSelfLearningMode)
		{
			m_btnAddModelImg.ShowWindow(SW_HIDE);
			EnableAllButtons(false);
			EnableUiBtn(IDB_ADD_MODEL_IMG, true, IMG_CH_ADD_MODEL_IMG);
	//		EnableUiBtn(IDB_ZOOM_IN, true, IMG_CH_ZOOM_IN);
		}
		else
		{
			m_btnDelCrtImg.ShowWindow(SW_HIDE);
		}
	}


	rcBtn.left = rcBtn.right + gapH;
	rcBtn.right = rcBtn.left + hzBtnSz;
	m_btnResetStatsData.MoveWindow(rcBtn);
    SetButtonVisibility();

}
void CChannelConfigDlg::SwitchChannel(int chIndex, int StationIndex)
{
 	if(chIndex == m_chConfig.m_chIndex)
 		return;

	m_bMultiChannnelImg = false;
	m_nImgChannelCount  = 1;

	std::vector<CChannelControlPanel*> VecPanel = gSystemMgr.GetPanelArray();

	if(m_settingType == T_LIGHT && m_bLsChanged)
	{
		LC lc = GetLightControllerParameter();
		gSystemMgr.UpdateChLightController(m_chConfig.m_chIndex, m_chConfig.m_lightCtrlSetting);
		WriteNewLcParameter2Device();
	}

	SavaChannelSettings2ConfigFile();
	// Stop displaying images of current channel

	if(chIndex != m_chConfig.m_chIndex)
	{
		m_pChannelControlPanel->SetChannelImageWindowPtr(nullptr);
		long long nID = -1;
		std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
		{
			if(pStation != nullptr)
			{
				pStation->SetChannelWindowOpenedFlag(false);
			}
		});
		SetTimer(TIMER_ID_SWITCH_CH, 200, NULL);
	}
	if (chIndex-1 < VecPanel.size())
	{
	 	m_pChannelControlPanel = VecPanel[chIndex-1];
	 	m_VecpProcessStation = m_pChannelControlPanel->m_vProcessorPtr;
	}

	m_pChannelControlPanel->SetChannelImageWindowPtr(this);
	SetCurCameraIndex(StationIndex);


	CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(chIndex);
	m_chConfig = *pChConfig;
	m_bModelFiles  = m_chConfig.m_bModelfiles;

	m_bFullScreen = true;
	m_lockedImgArray.clear();
	if(m_chConfig.m_vecCameraSetting.size() != m_VecpProcessStation.size())
	{
		m_bMultiChannnelImg = true;
		m_nImgChannelCount  = m_chConfig.m_vecCameraSetting.size();
	}
	CWnd* pRadio = (CWnd*)GetDlgItem(IDC_RADIO_CAMERA1);
	pRadio->ShowWindow(m_bMultiChannnelImg);
	pRadio->Invalidate();
	pRadio = (CWnd*)GetDlgItem(IDC_RADIO_CAMERA2);
	pRadio->ShowWindow(m_bMultiChannnelImg);
	pRadio->Invalidate();

	int width  = m_chConfig.m_uiSetting.m_width;
	int height = m_chConfig.m_uiSetting.m_height;

	size_t nProcessSize = m_VecpProcessStation.size();
	
	m_ScreenOp.init();


	if (!m_chConfig.m_bSplicing)
	{
		m_ScreenOp.SetCurRect(nProcessSize,width,height);
	}
	else
	{
		m_ScreenOp.SetCurRect(1,width,height);
	}

	m_rcImgWnd = m_ScreenOp.GetPosRect();
	m_OldrcImgWnd = m_rcImgWnd;

	ResizeOtherControl();
 
	Invalidate();
	
	for(int i = 0 ; i < 6; i++)
	{
		if (i >= nProcessSize)
		{
			CRect rect= CRect(0,0,0,0);
			long long CHImgWndID = m_VecCHImgWndID[i];
			set_window_extents(CHImgWndID,rect.top, rect.left,rect.Width(), rect.Height());
		}
		else
		{
			CRect rect;
			m_ScreenOp.GetIndexRect(i,rect);
			long long CHImgWndID = m_VecCHImgWndID[i];
			set_window_extents(CHImgWndID,rect.top, rect.left,rect.Width(), rect.Height());
		}
	}
	m_iCameraIndex = 0;
	ZoomSubwindow();

	std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
	{
		if(pStation != nullptr)
		{
			pStation->ResetDisplayRule();
		}
	});

 	m_pFuncInfoArray      = m_pProcessStation->GetFuncInfoArrayPtr();
 	m_pDebugFuncInfoArray = m_pProcessStation->GetDebugFuncInfoArrayPtr();

	m_vFuncInfo = ((m_bDebugMode) ? *m_pFuncInfoArray : *m_pDebugFuncInfoArray);

	SetSelfLearningFlag();
	SetButtonVisibility();

	m_lblThreshold_Lcl.SetWindowText(_T("N/A"));
	RefreshUiCtrl(&m_lblThreshold_Lcl);

	if(m_bSelfLearningMode)
	{
		m_bLocked = false;
		EnableAllButtons(false);
		EnableUiBtn(IDB_DELETE_CRT_IMG, true, IMG_CH_DELETE_MODEL_IMG);
//		EnableUiBtn(IDB_ZOOM_IN, true, IMG_CH_ZOOM_IN);
	}
	else
	{
		EnableUiBtn(IDB_LOCK, true, IMG_CH_UNLOCKED); 
		m_bLocked = false;
		EnableUiBtn(IDB_IMG_DISP_MODE, true, IMG_CH_SINGLE_MODE);
		m_bErrorMode = true;

		CWnd* pCtl = GetDlgItem(IDC_CHECK_READ_FILE);
		pCtl->EnableWindow(TRUE);
		m_bReadImg = FALSE;
		UpdateData(FALSE);
		OnBnClickedCheckReadImgFile();
	}
	//Reset and redraw SPC chart
	m_chartCtrl.ClearGraph();
	RefreshUiCtrl(&m_chartCtrl);

	m_algoHelpStrPanel.SetWindowText(_T(""));
	RefreshUiCtrl(&m_algoHelpStrPanel);

	m_crtFuncInfo = nullptr;

	DeleteAllSettingItem();
	if(m_settingType == T_CAMERA)
		OnCameraClicked();

	if(m_settingType == T_LIGHT)
	{
		OnLightClicked();
	}

	if(m_settingType == T_REJECTOR)
		OnRejectorClicked();

	if(m_settingType == T_DEVICE)
		OnDeviceInfoClicked();

	if(m_settingType == T_ALGO)
		DeleteAllSettingItem();

	m_algoList.DeleteAllItems();
	AddRoiFunc2List();
	RefreshUiCtrl(&m_paramBoard);
	m_btnApplyParam.EnableWindow(FALSE);

	m_crtImgIndex    = -1;
	m_nAlgoListIndex = -1;
	m_nImgCount      = 0;

	CString strIndex(_T("N/A"));

	if(m_bSelfLearningMode)
	{
		bool bModelImgReady = true;
		std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
		{
			if(!pStation->IsModelImageReady())
			{
				bModelImgReady = false;
			}
		});
		if(bModelImgReady)
		{
			OnRetrieveModelImage(0,0);
			m_btnSaveTR.EnableWindow(TRUE);
		}
		else
		{
			std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
			{
				pStation->ResetDisplayWindow();
			});
			SetImageIndexPanelText(0, 0);
			m_btnSaveTR.EnableWindow(FALSE);
		}
	}
	else
	{
		SetImageIndexPanelText(0, 0);
		std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
		{
			pStation->ResetDisplayWindow();
		});
		//ch adapt the para m_bDispErrRgn
		bool bDispErrRgn = (m_bDispErrRgn == TRUE)? true:false;
		std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
		{
			pStation->SetDispErrRgnFlag(bDispErrRgn);
		});
	}
}

void CChannelConfigDlg::OnBnClickedRadioCh1()
{
	m_radioIndex = 0;
	SwitchChannel(m_radioIndex + 1);
}

void CChannelConfigDlg::OnBnClickedRadioCh2()
{
	m_radioIndex = 1;
	SwitchChannel(m_radioIndex + 1);
}

void CChannelConfigDlg::OnBnClickedRadioCh3()
{
	m_radioIndex = 2;
	SwitchChannel(m_radioIndex + 1);
}

void CChannelConfigDlg::OnBnClickedRadioCh4()
{
	m_radioIndex = 3;
	SwitchChannel(m_radioIndex + 1);
}

void CChannelConfigDlg::OnBnClickedRadioCh5()
{
	m_radioIndex = 4;
	SwitchChannel(m_radioIndex + 1);
}

void CChannelConfigDlg::OnBnClickedRadioCh6()
{
	m_radioIndex = 5;
	SwitchChannel(m_radioIndex + 1);
}

void CChannelConfigDlg::OnBnClickedRadioCh7()
{
	m_radioIndex = 6;
	SwitchChannel(m_radioIndex + 1);
}

void CChannelConfigDlg::OnBnClickedRadioCh8()
{
	m_radioIndex = 7;
	SwitchChannel(m_radioIndex + 1);
}

LRESULT  CChannelConfigDlg::OnRetrieveModelImage(WPARAM wParam,LPARAM lParam)
{
	if(m_bSelfLearningMode)
	{
		if (m_vecModelIndex.empty())
		{
			return FALSE;
		}
		for (size_t i = 0; i < m_VecpProcessStation.size();i++)
		{
			m_crtImgIndex = m_vecModelIndex[i];
			m_pProcessStation = m_VecpProcessStation[i];
			m_nImgCount = m_pProcessStation->GetModelImageCount();

			if(m_nImgCount == 0 || m_crtImgIndex >= m_nImgCount)
				continue;
			m_crtImgIndex=0;
			m_vecModelIndex[i]=0;
			if(CheckImageIndex(m_crtImgIndex))
			{
				UpdateCurrentImgIndex(m_crtImgIndex);
				ProcessCurrentImage();
			}
		}
		m_pProcessStation = m_VecpProcessStation[m_CameraIndex];
		m_crtImgIndex = m_vecModelIndex[m_CameraIndex];
		m_nImgCount = m_pProcessStation->GetModelImageCount();
		UpdateCurrentImgIndex(m_crtImgIndex);
		m_btnSaveTR.EnableWindow(TRUE);


	}
	return 0;
}

bool  CChannelConfigDlg::GetNewRejectorSetting()
{
	size_t sz = m_vParamEditCtrl.size();
	m_crtRS = m_chConfig.m_rejectorSetting;

	CString strDelay,strSignalWidth;
	m_vParamEditCtrl[4]->GetWindowText(strDelay);
	m_vParamEditCtrl[5]->GetWindowText(strSignalWidth);
	CString sMsg;
	if(!utils::IsNumericString(strDelay) || !utils::IsNumericString(strSignalWidth))
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
		DisplayPromptMessage(sMsg,false);
		return false;
	}
	if(!strDelay.IsEmpty() && !strSignalWidth.IsEmpty())
	{
		try
		{
			int nDelay = (int)utils::to_float(strDelay);
			int nSignalWidth = (int)utils::to_float(strSignalWidth);
			if(nDelay < 0 || nSignalWidth < 0)
			{
				sMsg.LoadString(IDS_STRING_PARAM_CANNOT_BE_NEGATIVE);
				DisplayPromptMessage(sMsg,false);
				return false;
			}
			m_crtRS.m_delay = nDelay; 
			m_crtRS.m_signalWidth = nSignalWidth;
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
	return false;
}

bool CChannelConfigDlg::GetNewUiSetting(UI_SETTING& newUS)
{
	return true;
}

void CChannelConfigDlg::SetAlgoListStats(std::vector<FUNC_INFO>& vFuncInfo)
{
	if(!::IsWindow(m_hWnd))
		return;
	if(!::IsWindow(m_algoList.m_hWnd))
	{
		return;
	}

	m_vFuncInfo = vFuncInfo;

}

void CChannelConfigDlg::SetDisplayResult(std::vector<DisplayResult>& ResultDisplay)
{
	if (m_bExitFlag)
	{
		return;
	}
	m_vResultDisplay = ResultDisplay;
}

void CChannelConfigDlg::SetCheckResult(std::vector<IMG_PRODUCT>& vResultGroup)
{
	if (m_bExitFlag)
	{
		return;
	}
	m_vResultGroup = vResultGroup;
}

bool CChannelConfigDlg::DisplayALLImageOnChWnd()
{
	if(!m_bSplicing)
	{
		for (int i = 0; i < m_vResultGroup.size();i++)
		{
			int imgWndID     = m_VecCHImgWndID[i];
			Hobject crtImg   = m_vResultGroup[i].m_img;
			std::map<Hlong,Hobject> crtimgError = m_vResultGroup[i].m_unionErrRgn;
			Hobject crtTargetRgn = m_vResultGroup[i].m_crtTargetRgn;
			Hobject CrtCvInterImgs = m_vResultGroup[i].m_cvInterImgs;
			Hobject CrtCorrectImg = m_vResultGroup[i].m_crtCorrectImg;
			Hobject crtTargetRgnEx = m_vResultGroup[i].m_crtTargetRgnEx;

			Hobject img = crtImg;
			
			if (CrtCvInterImgs.Id() == H_EMPTY_REGION)
			{
				if ((CrtCorrectImg.Id()!= H_EMPTY_REGION))
				{
					img = CrtCorrectImg;
				}

				if(m_bMultiChannnelImg)
				{
					select_obj(crtImg,&img,m_nCrtImgChannel);
				}
 				DisplayImageOnChWnd(img,imgWndID);
			}
			else
			{
//				clear_window(imgWndID);    //取消中间图片的clear_window函数，DisplayImageOnChWnd本身会对图像做处理，不会有图像残留
				DisplayImageOnChWnd(CrtCvInterImgs,imgWndID);
			}
			//TODO:
	

			CRect rect; 
			::GetWindowRect(m_VecCHImgWnd[i],rect);

			//fix bug : img用于显示中间图片，
			//Hobject 生成时间点会被 函数DealWithTransparentObjectForDisplay 破坏
			//显示相关时间信息会取自原来Hobject=》crtImg，图片时间点能正常显示

			DisplayTimeStatisticOnChWnd(crtImg,m_vResultDisplay[i],imgWndID);
			if(gSystemMgr.IsChannelCameraRunning(m_chConfig.m_chIndex) && !m_bLocked)
				DisplayGlueInjectorIndexChWnd(img,m_vResultDisplay[i].m_nGII,imgWndID);
			else
			{
				if(m_crtImgIndex<m_lockedImgArray.size())
					DisplayGlueInjectorIndexChWnd(img,m_lockedImgArray[m_crtImgIndex][0].m_nGII,imgWndID);
			}
			std::map<Hlong,Hobject> imgError = crtimgError;
			
// 			count_obj(crtimgError, &nModelCount);
// 
// 			if(m_bMultiChannnelImg)
// 			{
// 				if (m_nCrtImgChannel<=nModelCount)
// 				{
// 					if (imgError.Id()!=H_EMPTY_REGION)
// 					{
// 						select_obj(crtimgError,&imgError,m_nCrtImgChannel);
// 					}
// 				}
// 			}
			DisplayErrorRegionOnChWnd(imgError,imgWndID);

			DisplayInnerStatisticsOnChWnd(img,m_vResultDisplay[i].m_vInnerStatsData,imgWndID);

			if(m_bMultiChannnelImg){
				Hobject rgn=crtTargetRgn;
				Hobject rgnEx=crtTargetRgnEx;
				Hlong numberObj;
				count_obj(crtTargetRgn,&numberObj);
				if(m_nCrtImgChannel<=numberObj)
				{
					//select_obj(crtTargetRgn,&rgn,m_nCrtImgChannel);
					//select_obj(crtTargetRgnEx,&rgnEx,m_nCrtImgChannel);
					DisplayErrImgTargetRegionOnChWnd(rgn,imgWndID);				

					DisplayErrImgTargetRegionOnChWnd(rgnEx,imgWndID,1);
				}
				//write_object(crtTargetRgn, "D:/Region3.hobj");				
			}
			else{
				DisplayErrImgTargetRegionOnChWnd(crtTargetRgn,imgWndID);

				DisplayErrImgTargetRegionOnChWnd(crtTargetRgnEx,imgWndID,1);
			}
			if (!m_bFullScreen)
			{
				if (m_bSplicing && i != 0)
				{
					rect = CRect(0,0,1,1);
				}
				else
				{
					m_ScreenOp.GetIndexRect(i,rect);
				}
				long long CHImgWndID = m_VecCHImgWndID[i];
				if (m_vResultGroup[i].m_img.Id() == H_EMPTY_REGION)
				{
					set_window_extents(CHImgWndID,rect.top, rect.left,rect.Width(), rect.Height());
				}
			}
		}
	}
	else
	{
		IMG_PRODUCT vResultGroup  = GetSPlicingResultGroup(m_vResultGroup);
		int imgWndID = m_VecCHImgWndID[0];
		Hobject img = vResultGroup.m_img;
		std::map<Hlong,Hobject> imgError = vResultGroup.m_unionErrRgn;
		Hobject crtTargetRgn = vResultGroup.m_crtTargetRgn;
		DisplayImageOnChWnd(img,imgWndID);
	}

	return true;
}

IMG_PRODUCT CChannelConfigDlg::GetSPlicingResultGroup(std::vector<IMG_PRODUCT> vResultGroup)
{
	IMG_PRODUCT vRet;


	// Local iconic variables
	Hobject  ho_Image, ho_Imagetmp, ho_TiledImage;

	// Local control variables
	HTuple  hv_offsetX, hv_offsetY, hv_defaultData;
	HTuple  hv_Width, hv_Height, hv_nCount, hv_Index, hv_WidthNew;
	HTuple  hv_HeightNew;

	size_t nCount =  m_VecpProcessStation.size();
	gen_empty_obj(&ho_Image);

	hv_offsetX = HTuple();
	hv_offsetY = HTuple();
	hv_defaultData = HTuple();
	hv_Width = 0;
	hv_Height = 0;
	hv_nCount =(int) m_VecpProcessStation.size();
	{
		for (int i=0 ; i < m_VecpProcessStation.size();i++)
		{
			ho_Imagetmp = vResultGroup[i].m_img;

			if (ho_Imagetmp.Id() == H_EMPTY_REGION)
			{
				return vRet;
			}
	
			get_image_size(ho_Imagetmp, &hv_Width, &hv_Height);
			concat_obj(ho_Image, ho_Imagetmp, &ho_Image);
			tuple_concat(hv_offsetX, 0, &hv_offsetX);
			tuple_concat(hv_offsetY, (hv_Width.Select(0))*i, &hv_offsetY);
			tuple_concat(hv_defaultData, -1, &hv_defaultData);

		}
	}
	hv_WidthNew = (hv_Width.Select(0)) * hv_nCount;
	hv_HeightNew = hv_Height.Select(0);
	tile_images_offset(ho_Image, &ho_TiledImage, hv_offsetX, hv_offsetY, hv_defaultData, 
		hv_defaultData, hv_defaultData, hv_defaultData, hv_WidthNew, hv_HeightNew);
	
	vRet.m_img = ho_TiledImage;

	return vRet;
}

void CChannelConfigDlg::DisplayTimeStatisticOnChWnd(Hobject& img,DisplayResult& vResultDisplay,long long chImgWndID)
{
	if(img.Id() == H_EMPTY_REGION)
		return;

	if(chImgWndID == -1)
		return;
	
	int nWndHeight;

	HTuple height, width;
	get_image_size(img, &width, &height);
	nWndHeight = height[0].I();

	float fRatio = nWndHeight / 494.0f;
//	float fRatio = 1.5f;
	int pos      = int(2 * fRatio);
	double total = 0.0;

	set_color(chImgWndID, "green");
	string strTimeStamp = utils::ExtractTimestampString(img);
	HTuple timestamp = strTimeStamp.c_str();
	set_tposition(chImgWndID, pos, 3);
	write_string(chImgWndID, timestamp);

	//TimeStatistic
	if (vResultDisplay.m_IndexTime > 0.001f)
	{
		set_color(chImgWndID, "green");
		HTuple strTime = HTuple(vResultDisplay.m_IndexTime) + HTuple("ms");
		set_tposition(chImgWndID, pos += int(35*fRatio), 3);
		write_string(chImgWndID, strTime);
	}


	if(vResultDisplay.m_bOverTime)
	{
		set_color(chImgWndID, "red");
	}
	else
	{
		set_color(chImgWndID, "green");
	}
	total = vResultDisplay.m_Alltime;
	HTuple strTotal("Total:");
	HTuple strTime = HTuple(total) + HTuple("ms");
	set_tposition(chImgWndID, pos += int(35*fRatio), 3);
	write_string(chImgWndID, strTotal);
	set_tposition(chImgWndID, pos += int(35*fRatio), 3);
	write_string(chImgWndID, strTime);

	if (m_bDebugMode && vResultDisplay.m_bScoreValid)
	{
		if(vResultDisplay.m_bScoreError)
		{
			set_color(chImgWndID, "red");
		}
		else
		{
			set_color(chImgWndID, "green");
		}

		string strOutputName = vResultDisplay.m_strOutputName;
		HTuple strName = strOutputName.c_str();
		set_tposition(chImgWndID, nWndHeight - 65*fRatio, 3);
		write_string(chImgWndID, strName);

		set_tposition(chImgWndID, nWndHeight - 35*fRatio, 3);
		write_string(chImgWndID, vResultDisplay.m_Socre);
	}
}

void CChannelConfigDlg::DisplayGlueInjectorIndexChWnd(Hobject& img,int nGii,long long chImgWndID)
{
    if(img.Id() == H_EMPTY_REGION)
        return;

    if(chImgWndID == -1 || nGii == -1)
        return;

    HTuple height, width;
    get_image_size(img, &width, &height);
    int nWndHeight = height[0].I();
    int nWndWidth = width[0].I();

    float fRatio  = nWndHeight / 494.0f;
    float fWRatio = nWndWidth / 659.0f;
    int pos      = int(2 * fRatio);
    int wPos     = int(500*fWRatio);

    set_color(chImgWndID, "green");
    set_tposition(chImgWndID, pos,wPos);

    HTuple strId = HTuple("Injector:") + nGii;
    write_string(chImgWndID, strId);
}
void CChannelConfigDlg::DisplayErrorRegionOnChWnd(std::map<Hlong,Hobject>& unionErrRgn,long long chImgWndID)
{
// 	if(unionErrRgn.Id() == H_EMPTY_REGION)
// 		return;

	if(::GetKeyState(VK_SPACE) < 0)
	{	
		return;
	}

 	if(chImgWndID == -1)	
		return;
	int i = 0;
	for (auto it = unionErrRgn.begin(); it != unionErrRgn.end();it++)
	{
		if (it->first==0)
		{
			set_color(chImgWndID, "red");
		}
		else
		{
			set_color(chImgWndID, "orange");
		}
		Hobject ChunionErrRgn = it->second;
		if(ChunionErrRgn.Id() == H_EMPTY_REGION)
			return;
		disp_obj(ChunionErrRgn,chImgWndID);
		i ++;
	}
}

void CChannelConfigDlg::DisplayInnerStatisticsOnChWnd(Hobject& crtImg,std::vector<float>	m_vInnerStatsData,long long chImgWndID)
{
	size_t sz = m_vInnerStatsData.size();
	if(sz < 2) return;

	long long wndId = (long long)chImgWndID;

	if(wndId == -1/* || m_pChConfigWnd == nullptr*/) 
		return;
	if(crtImg.Id() == H_EMPTY_REGION) return;

	UI_SETTING us = m_chConfig.m_uiSetting;
	Hlong nWidth, nHeight;
	get_image_size(crtImg, &nWidth, &nHeight);

	float xRate = 1.0f * nWidth / us.m_width;
	float yRate = 1.0f * nHeight / us.m_height;


	if(sz==260)
	{
		HTuple featureValue = HTuple();
		int nDispType = 0; // Display all data
		float fThresholdOrg =m_vInnerStatsData[0];
		float hThresholdOrg =m_vInnerStatsData[1];
		float minDataX = m_vInnerStatsData[2];
		float maxDataX = m_vInnerStatsData[3];
		float minMaxDistX =maxDataX-minDataX;
		float fThreshold = (float)(255.0*((fThresholdOrg-minDataX)/minMaxDistX));
		float hThreshold = (float)(255.0*((hThresholdOrg-minDataX)/minMaxDistX));
		if(m_vInnerStatsData[0] == 999999.0f)
		{
			nDispType = -1;
			fThreshold = 0;
			hThreshold = ::abs(m_vInnerStatsData[1]);
		}
		if(m_vInnerStatsData[1] == -999999.0f)
		{
			nDispType = 1;
			fThreshold = ::abs(m_vInnerStatsData[0]);
			hThreshold = 0;
		}
		for (size_t i = 4; i < sz; i++)
		{
			featureValue.Append((HTuple)m_vInnerStatsData[i]*yRate);
		}
		auto itMinMax = std::minmax_element(m_vInnerStatsData.begin() + 4, m_vInnerStatsData.end());
		float fMaxValue = 1.0f;
		float fMinValue = 1.0f;
		if(itMinMax.first != m_vInnerStatsData.end())
			fMinValue = *(itMinMax.first);
		if(itMinMax.second != m_vInnerStatsData.end())
			fMaxValue = *(itMinMax.second);

		//test for showing the filter result
		HTuple XValue, xStringCut,xString;
		int xIndex = 0;
		xStringCut= minDataX;
		if(abs(minDataX)<1000)
		{
			xString[0] =xStringCut.ToString(".3");
		}
		else
		{
			xString[0] =xStringCut.ToString(".4");
		}
		for (int i = 0; i < sz - 4; i ++)
		{
			XValue[i] = i+1;
			if((i+1)%((sz-2)/8) == 0)
			{
				xIndex ++;
				xStringCut= ((i+1.000001)/256.0)*minMaxDistX+minDataX;
				if(abs(xStringCut[0].D())<1000)
				{
					xString[xIndex] =xStringCut.ToString(".3");
				}
				else
				{
					xString[xIndex] =xStringCut.ToString(".4");
				}
			}
		}

		Hobject contour11, baseLine, PosThreshLine, NegThreshLine;
		HTuple XStartValue = 140 * xRate;
		HTuple XRange = 400 * xRate;
		HTuple XScaleFactor = 400 * xRate/(sz - 4);
		HTuple YStartValue = nHeight - 125 * yRate; //TODO
		HTuple YRange = 100 * yRate;

		HTuple lineX;
		lineX.Append(XStartValue);
		lineX.Append(XStartValue + XRange);
		HTuple lineY;
		lineY.Append(YStartValue);
		lineY.Append(YStartValue);

		HTuple PosLineRow, PosLineCol;
		HTuple NegLineRow, NegLineCol;

		PosLineRow.Append(YStartValue);
		PosLineRow.Append(YStartValue - YRange);
		PosLineCol.Append(XStartValue + fThreshold * XScaleFactor);
		PosLineCol.Append(XStartValue + fThreshold * XScaleFactor);

		NegLineRow.Append(YStartValue);
		NegLineRow.Append(YStartValue - YRange);
		NegLineCol.Append(XStartValue + hThreshold * XScaleFactor);//
		NegLineCol.Append(XStartValue + hThreshold * XScaleFactor);

		gen_contour_polygon_xld(&baseLine, lineY, lineX);
		gen_contour_polygon_xld(&contour11, YStartValue + featureValue, XStartValue + XValue * XScaleFactor);
		gen_contour_polygon_xld(&PosThreshLine, PosLineRow, PosLineCol);
		gen_contour_polygon_xld(&NegThreshLine, NegLineRow, NegLineCol);

		set_color(wndId, "blue");
		disp_obj(contour11, wndId);
		disp_obj(baseLine, wndId);
		set_color(wndId, "yellow");
		for(int i = 0; i <= xIndex; i ++)   //输出横坐标
		{
			set_tposition(wndId, YStartValue + 3 * yRate, XStartValue + i * xRate * 50);
			write_string(wndId, xString[i]);
		}
		switch(nDispType)
		{
		case 1:
			set_color(wndId, "yellow");
			disp_obj(PosThreshLine, wndId);
			break;
		case -1:
			set_color(wndId, "yellow");
			disp_obj(NegThreshLine, wndId);
			break; 
		case 0:
			set_color(wndId, "yellow");
			disp_obj(PosThreshLine, wndId);
			set_color(wndId, "red");
			disp_obj(NegThreshLine, wndId);
			break;
		default:
			break;
		}
		return;
	}
	HTuple featureValue = HTuple();
	int nDispType = 0; // Display all data
	float fThreshold = ::abs(m_vInnerStatsData[0]);
	float hThreshold = ::abs(m_vInnerStatsData[1]);
	if(m_vInnerStatsData[0] == 999999.0f)
	{
		nDispType = -1;
		fThreshold = 0;
		hThreshold = ::abs(m_vInnerStatsData[1]);
	}
	if(m_vInnerStatsData[1] == -999999.0f)
	{
		nDispType = 1;
		fThreshold = ::abs(m_vInnerStatsData[0]);
		hThreshold = 0;
	}
	for (size_t i = 2; i < sz; i++)
	{
		featureValue.Append((HTuple)m_vInnerStatsData[i]*yRate);
	}
	auto itMinMax = std::minmax_element(m_vInnerStatsData.begin() + 2, m_vInnerStatsData.end());
	float fMaxValue = 1.0f;
	float fMinValue = 1.0f;
	if(itMinMax.first != m_vInnerStatsData.end())
		fMinValue = *(itMinMax.first);
	if(itMinMax.second != m_vInnerStatsData.end())
		fMaxValue = *(itMinMax.second);

	//test for showing the filter result
	HTuple XValue, xString;
	int xIndex = 0;
	xString[0] = 0;
	for (int i = 0; i < sz - 2; i ++)
	{
		XValue[i] = i+1;
		if((i+1)%((sz-2)/8) == 0)
		{
			xIndex ++;
			xString[xIndex] = i+1;
		}
	}

	Hobject contour11, baseLine, PosThreshLine, NegThreshLine;
	HTuple XStartValue = 140 * xRate;
	HTuple XRange = 400 * xRate;
	HTuple XScaleFactor = 400 * xRate/(sz - 2);
	HTuple YStartValue = nHeight - 125 * yRate; //TODO
	HTuple YRange = 100 * yRate;

	HTuple lineX;
	lineX.Append(XStartValue);
	lineX.Append(XStartValue + XRange);
	HTuple lineY;
	lineY.Append(YStartValue);
	lineY.Append(YStartValue);

	HTuple PosLineRow, PosLineCol;
	HTuple NegLineRow, NegLineCol;

	if (sz > 500)    //适用于阀值线为水平线的，比如卷边滤波算法
	{
		if(YStartValue[0].D() - fThreshold*yRate > nHeight - 125*yRate - 120*yRate)
		{
			PosLineRow.Append(YStartValue - fThreshold * yRate);
			PosLineRow.Append(YStartValue - fThreshold * yRate);
		}
		else
		{
			PosLineRow.Append(HTuple(nHeight - 125 * yRate - 120 * yRate));
			PosLineRow.Append(HTuple(nHeight - 125 * yRate - 120 * yRate));
		}
		PosLineCol.Append(XStartValue);
		PosLineCol.Append(XStartValue+XRange);

		if(YStartValue[0].D() + hThreshold*yRate < nHeight - 25*yRate)
		{
			NegLineRow.Append(YStartValue + hThreshold * yRate);
			NegLineRow.Append(YStartValue + hThreshold * yRate);
		}
		else
		{
			NegLineRow.Append(HTuple(nHeight - 5 * yRate));
			NegLineRow.Append(HTuple(nHeight - 5 * yRate));
		}
		NegLineCol.Append(XStartValue);
		NegLineCol.Append(XStartValue+XRange);
	}
	else         //适用于阀值线为竖直线的，比如是灰度直方图
	{
		PosLineRow.Append(YStartValue);
		PosLineRow.Append(YStartValue - YRange);
		PosLineCol.Append(XStartValue + fThreshold * XScaleFactor);
		PosLineCol.Append(XStartValue + fThreshold * XScaleFactor);

		NegLineRow.Append(YStartValue);
		NegLineRow.Append(YStartValue - YRange);
		NegLineCol.Append(XStartValue + hThreshold * XScaleFactor);//
		NegLineCol.Append(XStartValue + hThreshold * XScaleFactor);
	}

	gen_contour_polygon_xld(&baseLine, lineY, lineX);
	gen_contour_polygon_xld(&contour11, YStartValue + featureValue, XStartValue + XValue * XScaleFactor);
	gen_contour_polygon_xld(&PosThreshLine, PosLineRow, PosLineCol);
	gen_contour_polygon_xld(&NegThreshLine, NegLineRow, NegLineCol);

	set_color(wndId, "blue");
	disp_obj(contour11, wndId);
	disp_obj(baseLine, wndId);
	set_color(wndId, "yellow");
	for(int i = 0; i <= xIndex; i ++)   //输出横坐标
	{
		set_tposition(wndId, YStartValue + 3 * yRate, XStartValue + i * xRate * 50);
		write_string(wndId, xString[i]);
	}
	switch(nDispType)
	{
	case 1:
		set_color(wndId, "yellow");
		disp_obj(PosThreshLine, wndId);
		break;
	case -1:
		set_color(wndId, "yellow");
		disp_obj(NegThreshLine, wndId);
		break; 
	case 0:
		set_color(wndId, "yellow");
		disp_obj(PosThreshLine, wndId);
		set_color(wndId, "red");
		disp_obj(NegThreshLine, wndId);
		break;
	default:
		break;
	}
}

void CChannelConfigDlg::DisplayErrImgTargetRegionOnChWnd(Hobject& crtTargetRgn,long long chImgWndID,int iRegionColor) 
{ 
	Hlong nModelCount;
	Hobject TargetRgn;
	count_obj(crtTargetRgn, &nModelCount);
	TargetRgn= crtTargetRgn;
	if(m_bMultiChannnelImg)
	{
		if (m_nCrtImgChannel<=nModelCount)
		{
			if (TargetRgn.Id()!=H_EMPTY_REGION)
			{
				select_obj(crtTargetRgn,&TargetRgn,m_nCrtImgChannel);
			}
		}
	}

 	if(crtTargetRgn.Id() == H_EMPTY_REGION)
 		return;

	if(chImgWndID == -1)
		return;
		
	if (iRegionColor == 0)
	{
		set_color(chImgWndID, "green");
	}
	else
	{
		set_color(chImgWndID, "yellow");
	}
	
	disp_obj(crtTargetRgn, chImgWndID);

}
bool	CChannelConfigDlg::DealWithTransparentObjectForDisplay(Hobject &object)
{
	HTuple image_type;
	get_image_type(object, &image_type);
	HTuple height, width;
	get_image_size(object, &width, &height);
		
	Hobject Image_back;
	gen_image_const (&Image_back, image_type, width, height);

	HTuple nImageChannelCount;
	count_channels(object,&nImageChannelCount);

	if (nImageChannelCount==3)
	{
		Hobject Image_back1;
		append_channel (Image_back, Image_back, &Image_back1);
		append_channel (Image_back, Image_back1, &Image_back);
	}

	paint_gray(object, Image_back, &object);
	
	return true;
}
bool CChannelConfigDlg::DisplayImageOnChWnd(Hobject& img,long long chImgWndID)
{
	
	if(img.Id() == H_EMPTY_REGION)
		return false;

 	if(chImgWndID == -1)
 		return false;
// 	if(m_bDebugMode)
// 	{

 		HTuple height, width;
 		get_image_size(img, &width, &height);		
		set_part(chImgWndID, 0, 0, height - 1, width - 1);

		DealWithTransparentObjectForDisplay(img);
// 	}
// 	else
// 	{
// 		set_part(chImgWndID, 0, 0, m_imgSz.m_nHeight - 1, m_imgSz.m_nWidth - 1);
// 	}

	disp_obj(img,chImgWndID);

	return true;
}

LRESULT  CChannelConfigDlg::OnUpdateChannelScreen(WPARAM wParam,LPARAM lParam)
{
	try
	{
		DisplayALLImageOnChWnd();
	}
	catch(HException& except)
	{
		stringstream oss;
		oss << "Error ID:" << (int)except.err << "Description:" << except.proc << " - " << except.message;
		wstring wstrMsg = utils::Utf8ToUnicode(oss.str());
		gLogger.QueueUpLog(wstrMsg);
		gLogger.FlushLogCache();
	}
	
	m_algoList.UpdateStatsData(m_vFuncInfo,m_bDebugMode,m_chConfig.m_rejectorSetting.m_nLastN);
	m_algoList.Invalidate();

	return 0;
}

void CChannelConfigDlg::RedrawAlgoList()
{
	if(!::IsWindow(m_hWnd))
		return;

	m_algoList.UpdateStatsData(m_vFuncInfo,m_bDebugMode,m_chConfig.m_rejectorSetting.m_nLastN);
	m_algoList.Invalidate();
}

void CChannelConfigDlg::EnableDisplayModeButton(bool bEnable)
{
	if(m_bErrorMode)
	{
		(bEnable == true)?m_btnDispMode.SetLabelImage(IMG_CH_SINGLE_MODE):m_btnDispMode.SetLabelImage(IMG_CH_SINGLE_MODE_GRAY);
	}
	else
	{
		(bEnable == true)?m_btnDispMode.SetLabelImage(IMG_CH_CONTINOUS_MODE):m_btnDispMode.SetLabelImage(IMG_CH_CONTINOUS_MODE_GRAY);
	}
	m_btnDispMode.EnableWindow(bEnable);
	RefreshUiCtrl(&m_btnDispMode);
}

void CChannelConfigDlg::SetSelfLearningFlag()
{
	TASK crtTask;
	bool bRet = m_chConfig.GetCurrentTask(crtTask);
	m_bSelfLearningMode = crtTask.m_bSelfLearning;

	//函数功能扩展,任务列表中是否含有自学习任务
	m_bSelfLearningFunc = false;
	for (auto it = m_chConfig.m_vTask.begin();it != m_chConfig.m_vTask.end();it++)
	{
		TASK task = (*it);
		if (task.m_bSelfLearning)
		{
			m_bSelfLearningFunc = true;
			break;
		}
	}	
}

void CChannelConfigDlg::OnBnClickedBtnSaveTrainningResult()
{
	CString sFolder = SetTargetImageFilePath(false);
	if(sFolder.GetLength() <= 0)
		return;

	CString strMsg;
	wstring wstrFolder = CT2W(sFolder);
	for (int i = 0 ; i< m_VecpProcessStation.size(); i++)
	{
		if(!m_VecpProcessStation[i]->SaveTrainningResult(wstrFolder))
		{
			strMsg.LoadString(IDS_STRING_OPERATION_NG);
			DisplayPromptMessage(strMsg, false);
			return;
		}
	}

	strMsg.LoadString(IDS_STRING_OPERATION_OK);
	DisplayPromptMessage(strMsg, true);
	m_btnSaveTR.EnableWindow(FALSE);
}

void CChannelConfigDlg::UpdateDeviceData()
{
	if(m_settingType != T_DEVICE)
		return;

	if(m_vParamEditCtrl.size() >= 9)
	{
		CHANNEL_STATUS data;
		gSystemMgr.GetChDeviceData(m_chConfig.m_chIndex,data);
		CString sValue;
		CEdit* pEdit = nullptr;

		//QlifeID
		if(data.m_nLifeID == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nTotalOkCount);
		}
		pEdit = (CEdit*)m_vParamEditCtrl[5];
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		//管路速度
		if(data.m_nSpeed == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nSpeed);
		}
		pEdit = (CEdit*)m_vParamEditCtrl[0];
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		if(data.m_nSensorCount == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nSensorCount);
		}
		pEdit = (CEdit*)m_vParamEditCtrl[1];
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		pEdit = (CEdit*)m_vParamEditCtrl[2];
		sValue.Format(_T("%d"), data.m_nTriggerIgnoreCount);
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		pEdit = (CEdit*)m_vParamEditCtrl[3];
		sValue.Format(_T("%d"), data.m_nDroppedImg);
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		if(data.m_nRejectCount == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nRejectCount);
		}
		pEdit = (CEdit*)m_vParamEditCtrl[4];
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		if(data.m_nEoeSpeed == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%0.1f"), data.m_nEoeSpeed/10.0f);
		}
		pEdit = (CEdit*)m_vParamEditCtrl[6];
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		if(data.m_nPositionError == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nPositionError);
		}
		pEdit = (CEdit*)m_vParamEditCtrl[7];
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		if(data.m_nPulse == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("%d"), data.m_nPulse);
		}
		pEdit = (CEdit*)m_vParamEditCtrl[8];
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();

		//定位失败解析标志
		if(data.m_nPositionFailFlag == -1)
		{
			sValue = _T("N/A");
		}
		else
		{
			sValue.Format(_T("0x%x   0x%I64x"), data.m_nPositionFailFlag,data.m_nPositionFailCount);
		}
		pEdit = (CEdit*)m_vParamEditCtrl[9];
		pEdit->SetWindowText(sValue);
		pEdit->Invalidate();


		if (m_vParamEditCtrl.size() == 11 /*10*/)
		{
			if(data.m_nReject2Count < 0)
			{
				sValue = _T("N/A");
			}
			else
			{
				sValue.Format(_T("%d"), data.m_nReject2Count);
			}
			pEdit = (CEdit*)m_vParamEditCtrl[10 /*9*/];
			pEdit->SetWindowText(sValue);
			pEdit->Invalidate();
		}

		if (m_vParamEditCtrl.size() ==14 /*13*/)
		{
			if(data.m_nDroppedImg2 < 0)
			{
				sValue = _T("N/A");
			}
			else
			{
				sValue.Format(_T("%d"), data.m_nDroppedImg2);
			}
			pEdit = (CEdit*)m_vParamEditCtrl[10 /*9*/];
			pEdit->SetWindowText(sValue);
			pEdit->Invalidate();

			if(data.m_nDroppedImg3 < 0)
			{
				sValue = _T("N/A");
			}
			else
			{
				sValue.Format(_T("%d"), data.m_nDroppedImg3);
			}
			pEdit = (CEdit*)m_vParamEditCtrl[11 /*10*/];
			pEdit->SetWindowText(sValue);
			pEdit->Invalidate();

			if(data.m_nDroppedImg4 < 0)
			{
				sValue = _T("N/A");
			}
			else
			{
				sValue.Format(_T("%d"), data.m_nDroppedImg4);
			}
			pEdit = (CEdit*)m_vParamEditCtrl[12 /*11*/];
			pEdit->SetWindowText(sValue);
			pEdit->Invalidate();

			if(data.m_nReject2Count < 0)
			{
				sValue = _T("N/A");
			}
			else
			{
				sValue.Format(_T("%d"), data.m_nReject2Count);
			}
			pEdit = (CEdit*)m_vParamEditCtrl[13 /*12*/];
			pEdit->SetWindowText(sValue);
			pEdit->Invalidate();
		}
	}
}

void CChannelConfigDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_DLG_READY)
	{
		std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
		{
			if(pStation != nullptr)
			{
				pStation->SetChannelWindowOpenedFlag(true);
				pStation->SetDispErrRgnFlag(true);  //错误区域
			}
		});

		gSystemMgr.SetSerialPortOwnerWindow(nullptr);
		if(m_bSelfLearningMode)
		{
			m_settingType = T_ALGO;
			CreateSlAlgoParamItems();
			DisplayFirstModelImage();
		}
		m_pChannelControlPanel->SetChannelImageWindowPtr(this);
		FramePaint();
		KillTimer(TIMER_ID_DLG_READY);
	}
	if(nIDEvent == TIMER_ID)
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_WARNING_PANEL);
		pWnd->SetWindowText(_T(""));
		RefreshUiCtrl(pWnd);
		m_bOpRslt = false;
		KillTimer(TIMER_ID);
	}
	if(nIDEvent == TIMER_ID_FOR_EDIT_CTRL)
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_BTN_DECREASE);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_BTN_INCREASE);
		pWnd->ShowWindow(SW_HIDE);

		m_paramHelpStrPanel.SetWindowText(_T(""));
		RefreshUiCtrl(&m_paramHelpStrPanel);

		KillTimer(TIMER_ID_FOR_EDIT_CTRL);
		m_algoList.SetFocus();
	}

	if(nIDEvent == TIMER_ID_INCREASE_DECREASE)
	{
		if(m_bDecBtnHold)
		{
			OnStnClickedStaticBtnDecrease();
		}
		if(m_bIncBtnHold)
		{
			OnStnClickedStaticBtnIncrease();
		}
	}
	if(nIDEvent == TIMER_ID_SWITCH_CH)
	{		
		std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
		{
			if(pStation != nullptr)
			{
				pStation->SetChannelWindowOpenedFlag(true);
			}
		});
		KillTimer(TIMER_ID_SWITCH_CH);
	}

	if(nIDEvent == TIMER_ID_DEVICE_DATA)
	{
		if(m_settingType != T_DEVICE)
		{
			KillTimer(TIMER_ID_DEVICE_DATA);
		}
		else
		{
			UpdateDeviceData();
		}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

void CChannelConfigDlg::EnableUiBtn(int nCtlID, bool bEnable, eIMAGE_TYPE nImgType)
{
	CLabelCtrl* pCtrl = (CLabelCtrl*)GetDlgItem(nCtlID);
	if(pCtrl != nullptr)
	{
		pCtrl->EnableWindow(bEnable);
		pCtrl->SetLabelImage(nImgType);
		RefreshUiCtrl(pCtrl);
	}
}

void CChannelConfigDlg::OnBnClickedCheckReadImgFile()
{
	UpdateData(TRUE);
	m_crtImgIndex = -1;
	m_nImgCount   = 0;
	if(m_bReadImg)
	{
		m_bErrorMode = false;
		m_pChannelControlPanel ->SetDispAllImageFlag(true);

		m_bDebugMode = true;
		m_pChannelControlPanel->SetDebugFlag(true);

		EnableUiBtn(IDB_IMG_SOURCE, true,IMG_CH_IMG_SOURCE);
		EnableUiBtn(IDB_LOCK, false, IMG_CH_UNLOCKED_GRAY);
		EnableUiBtn(IDB_IMG_DISP_MODE, false, IMG_CH_CONTINOUS_MODE_GRAY);
//		EnableUiBtn(IDB_ZOOM_IN, true, IMG_CH_ZOOM_IN);
		EnableUiBtn(IDB_SAVE_IMG, false, IMG_CH_SAVE_IMG_GRAY);
		EnableUiBtn(IDB_SAVE_ALL_IMG, false, IMG_CH_SAVE_ALL_IMG_GRAY);
		if(m_bSelfLearningMode)
		{
			EnableUiBtn(IDB_DELETE_CRT_IMG, false, IMG_CH_DELETE_MODEL_IMG_GRAY);
		}
		else
		{
			EnableUiBtn(IDB_ADD_MODEL_IMG, false, IMG_CH_ADD_MODEL_IMG_GRAY);
		}
	}
	else
	{
		m_bDebugMode = false;
		m_pChannelControlPanel->SetDebugFlag(false);

		m_bErrorMode = true;
		m_pChannelControlPanel ->SetDispAllImageFlag(false);

		m_bLocked = false;
		EnableUiBtn(IDB_IMG_SOURCE, false,IMG_CH_IMG_SOURCE_GRAY);

		EnableUiBtn(IDB_LOCK, true, IMG_CH_UNLOCKED);
		if(m_bErrorMode)
			EnableUiBtn(IDB_IMG_DISP_MODE, true, IMG_CH_SINGLE_MODE);
		else
			EnableUiBtn(IDB_IMG_DISP_MODE, true, IMG_CH_CONTINOUS_MODE);
//		EnableUiBtn(IDB_ZOOM_IN, false, IMG_CH_ZOOM_IN_GRAY);
		EnableUiBtn(IDB_SAVE_IMG, false, IMG_CH_SAVE_IMG_GRAY);
		EnableUiBtn(IDB_SAVE_ALL_IMG, false, IMG_CH_SAVE_ALL_IMG_GRAY);
		EnableUiBtn(IDB_DELETE_CRT_IMG, false, IMG_CH_DELETE_MODEL_IMG_GRAY);
		EnableUiBtn(IDB_ADD_MODEL_IMG, false, IMG_CH_ADD_MODEL_IMG_GRAY);
	}

	m_lblIndex.SetWindowText(_T("N/A"));
	m_lblIndex.Invalidate();
	RefreshUiCtrl(&m_btnImgSource);
}

void CChannelConfigDlg::EnableAllButtons(bool bEnable)
{
	m_bReadImg = FALSE;
	UpdateData(FALSE);
	CWnd* pWnd = GetDlgItem(IDC_CHECK_READ_FILE);
	pWnd->EnableWindow(bEnable);
	if(!bEnable)
	{
		EnableUiBtn(IDB_LOCK, bEnable, IMG_CH_UNLOCKED_GRAY);
		if(m_bErrorMode)
			EnableUiBtn(IDB_IMG_DISP_MODE, bEnable, IMG_CH_SINGLE_MODE_GRAY);
		else
			EnableUiBtn(IDB_IMG_DISP_MODE, bEnable, IMG_CH_CONTINOUS_MODE_GRAY);
	//	EnableUiBtn(IDB_ZOOM_IN, bEnable, IMG_CH_ZOOM_IN_GRAY);
		EnableUiBtn(IDB_SAVE_IMG, bEnable, IMG_CH_SAVE_IMG_GRAY);
		EnableUiBtn(IDB_SAVE_ALL_IMG, bEnable, IMG_CH_SAVE_ALL_IMG_GRAY);
		EnableUiBtn(IDB_DELETE_CRT_IMG, bEnable, IMG_CH_DELETE_MODEL_IMG_GRAY);
		EnableUiBtn(IDB_ADD_MODEL_IMG, bEnable, IMG_CH_ADD_MODEL_IMG_GRAY);
	}
	else
	{
		EnableUiBtn(IDB_LOCK, bEnable, IMG_CH_UNLOCKED);
		if(m_bErrorMode)
			EnableUiBtn(IDB_IMG_DISP_MODE, bEnable, IMG_CH_SINGLE_MODE);
		else
			EnableUiBtn(IDB_IMG_DISP_MODE, bEnable, IMG_CH_CONTINOUS_MODE);
//		EnableUiBtn(IDB_ZOOM_IN, bEnable, IMG_CH_ZOOM_IN);
		EnableUiBtn(IDB_SAVE_IMG, bEnable, IMG_CH_SAVE_IMG);
		EnableUiBtn(IDB_SAVE_ALL_IMG, bEnable, IMG_CH_SAVE_ALL_IMG);
		EnableUiBtn(IDB_DELETE_CRT_IMG, bEnable, IMG_CH_DELETE_MODEL_IMG);
		EnableUiBtn(IDB_ADD_MODEL_IMG, bEnable, IMG_CH_ADD_MODEL_IMG);
	}
}

void CChannelConfigDlg::OnBnClickedCheckDispErrorRgn()
{
	UpdateData(TRUE);
	bool bDispErrRgn = (m_bDispErrRgn == TRUE)? true:false;
	for (size_t i = 0; i < m_VecpProcessStation.size();i++)
	{
		m_VecpProcessStation[i]->SetDispErrRgnFlag(bDispErrRgn);
	}
}

void CChannelConfigDlg::AdjustAlgorithmParam(bool bIncreased)
{
	std::vector<INPUT_PARAM>  vParam;
	if(m_crtFuncInfo != nullptr)
	{
		vParam = m_crtFuncInfo->m_vParam;
	}
	size_t sz = m_vParamEditCtrl.size();
	if(sz <= 0)
		return;

	auto it = std::find(m_vParamEditCtrl.begin(), m_vParamEditCtrl.end(), m_crtEditCtrl);
	if(it == m_vParamEditCtrl.end())
		return;

	long long index = std::distance(m_vParamEditCtrl.begin(), it);
	INPUT_PARAM param = vParam[index];

	float max = param.m_max;
	float min = param.m_min;
	CString strValue, sMsg;
	m_crtEditCtrl->GetWindowText(strValue);
	wstring str = CT2W(strValue);
	if(!utils::IsNumericString(str))
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
		DisplayPromptMessage(sMsg,false);
		return;
	}
	float fValue = boost::lexical_cast<float>(str);
	if(bIncreased)
		fValue += param.m_step;
	else
		fValue -= param.m_step;

	if(fValue < min || fValue > max)
	{
		sMsg.LoadString(IDS_STRING_PARAM_VALUE_OUT_RANGE);
		CString sMinMax;
		if(vParam[index].m_type == VT_R4)
		{
			sMinMax.Format(_T("%.1f~%.1f"), min, max);
		}
		else
		{
			sMinMax.Format(_T("%d~%d"), (int)min, (int)max);
		}
		sMsg.Replace(_T("#~#"),sMinMax);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	else
	{
		if(vParam[index].m_type == VT_R4)
		{
			if(param.m_step >= 0.5f)
			{
				strValue.Format(_T("%.1f"), fValue);
			}
			else
			{
				strValue.Format(_T("%.3f"), fValue);
			}
		}
		else
		{
			strValue.Format(_T("%d"), (int)fValue);
		}
		m_crtEditCtrl->SetWindowText(strValue);
		OnBnClickedOk();
	}
}

void CChannelConfigDlg::OnStnClickedStaticBtnDecrease()
{
	if(m_settingType == T_ALGO)
	{
		AdjustAlgorithmParam(false);
	}
}

void CChannelConfigDlg::OnStnClickedStaticBtnIncrease()
{
	if(m_settingType == T_ALGO)
	{
		AdjustAlgorithmParam(true);
	}
}

void CChannelConfigDlg::OnEnSetfocusEditParameter()
{
	if(m_bSelfLearningMode) return;
	size_t sz = m_vParamEditCtrl.size();
	if(sz <= 0) return;
	KillTimer(TIMER_ID_FOR_EDIT_CTRL);
	SetTimer(TIMER_ID_FOR_EDIT_CTRL, 30000, NULL);

	CRect rcParamBoard;
	m_paramBoard.GetWindowRect(rcParamBoard);
	CRect rc;
	m_crtEditCtrl = CWnd::GetFocus();
	if(m_crtEditCtrl != nullptr)
	{
		m_crtEditCtrl->GetWindowRect(rc);
		int nBtnWidth = (rcParamBoard.right - rc.right - 3 * 4) / 2;
		if(nBtnWidth > 50)
			nBtnWidth = 50;

		CWnd* pBtn = GetDlgItem(IDC_STATIC_BTN_DECREASE);
		CRect rcBtn;
		rcBtn.left   = rc.right + 4;
		rcBtn.right  = rcBtn.left + nBtnWidth;
		rcBtn.top    = rc.top;
		rcBtn.bottom = rc.bottom;
		pBtn->MoveWindow(rcBtn);
		pBtn->ShowWindow(SW_SHOW);
		m_rcDecreaseBtn = rcBtn;


		rcBtn.left = rcBtn.right + 4;
		rcBtn.right = rcBtn.left + nBtnWidth;
		pBtn = GetDlgItem(IDC_STATIC_BTN_INCREASE);
		pBtn->MoveWindow(rcBtn);
		pBtn->ShowWindow(SW_SHOW);
		m_rcIncreaseBtn = rcBtn;

		// Display Parameter Help String 
		auto it = std::find(m_vParamEditCtrl.begin(), m_vParamEditCtrl.end(), m_crtEditCtrl);
	    if(it == m_vParamEditCtrl.end())
			return;
		m_paramHelpStrPanel.SetWindowText(_T(""));

		int nDis = (int)std::distance(m_vParamEditCtrl.begin(),it);
		if(nDis >= 0 && nDis < (int)m_vParamHelpStr.size())
		{
			wstring strHelp = m_vParamHelpStr[nDis];
			m_paramHelpStrPanel.SetWindowText(strHelp.c_str());
		}
		m_paramHelpStrPanel.ShowWindow(SW_HIDE);
		m_paramHelpStrPanel.ShowWindow(SW_SHOW);
	}
}

void CChannelConfigDlg::RefreshUiCtrl(CWnd* pCtrl)
{
	if(pCtrl == nullptr)
		return;
	CRect rc;
	pCtrl->GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect(rc);
}
// void CImageProcessor::SyncNewDebugParameter(DISPID& funcId, int funcIndex)
// {
// 	::EnterCriticalSection(m_pParamDataCs);
// 
// 	for(int i = 0; i < m_vFuncInfo.size(); i++)
// 	{
// 		if(m_vFuncInfo[i].m_dispId == funcId && funcIndex == m_vFuncInfo[i].m_funcIndex)
// 		{
// 			m_vFuncInfo[i].m_vParam.assign(m_vFuncInfoDebug[i].m_vParam.begin(),m_vFuncInfoDebug[i].m_vParam.end());
// 			m_vFuncInfo[i].m_vComParam.assign(m_vFuncInfoDebug[i].m_vComParam.begin(),m_vFuncInfoDebug[i].m_vComParam.end());
// 			break;
// 		}
// 	}
// 
// 	::LeaveCriticalSection(m_pParamDataCs);
// }
LRESULT  CChannelConfigDlg::OnExternalFileUpdated(WPARAM wParam,LPARAM lParam)
{
	if(m_crtFuncInfo != nullptr)
	{
		CString sMsg;
		char chRcv[128];
		::memset((void*)chRcv, 0x00, 128);
		bool bOk = false;
		if(m_transferStation.Read(sizeof(char),chRcv, 128))
		{
			string strRcvFile(chRcv);
			wstring wstrRcvFile = utils::AnsiToUnicode(strRcvFile);
			if(wstrRcvFile == L"")
			{
				sMsg.LoadString(IDS_STRING_OPERATION_NG);
				DisplayPromptMessage(sMsg, false);
				gLogger.QueueUpLog(L"Receive EMPTY file from the Model Tool!");
				return 1;
			}

			FUNC_INFO* pFuncInfo = m_dictBtnID_FI[m_nCrtBtnID];
		
			DISPID dispID  = pFuncInfo->m_dispId;
			int nFuncIndex = pFuncInfo->m_funcIndex;

			std::vector<INPUT_PARAM> vNewParam;
			std::vector<INPUT_PARAM> vConfigParam;

			m_dictBtnID_File[m_nCrtBtnID] = wstrRcvFile;
			if(GetNewAlgoParameters(vNewParam,vConfigParam) && !vNewParam.empty())
			{
				TASK* pCrtTask = m_chConfig.GetCurrentTask();

				if(pCrtTask->UpdateParameter(nFuncIndex, vConfigParam)) // Update Current Task;
				{
					if(gConfigMgr.UpdateTask(m_chConfig.m_chIndex, *pCrtTask, false))
					{
						if (m_bModelFiles)
						{
							m_VecpProcessStation[m_iCameraIndex]->ChangeDebugParameter(dispID, nFuncIndex, vNewParam);
							m_VecpProcessStation[m_iCameraIndex]->ApplyNewParameter(dispID, nFuncIndex);
							m_VecpProcessStation[m_iCameraIndex]->ReloadExternalFile(wstrRcvFile);
							m_VecpProcessStation[m_iCameraIndex]->SetTask(*pCrtTask);

						}
						else
						{
							for (size_t i = 0; i < m_VecpProcessStation.size();i++)
							{
								m_VecpProcessStation[i]->ChangeDebugParameter(dispID, nFuncIndex, vNewParam);
								m_VecpProcessStation[i]->ApplyNewParameter(dispID, nFuncIndex);
								m_VecpProcessStation[i]->ReloadExternalFile(wstrRcvFile);
								m_VecpProcessStation[i]->SetTask(*pCrtTask);

							}
						}
						if (m_pChannelControlPanel!=NULL)
						{
							m_pChannelControlPanel->ChangeDebugParameter(m_crtFuncInfo->m_dispId, m_crtFuncInfo->m_funcIndex, vNewParam);
							m_pChannelControlPanel->ChangeParameter(m_crtFuncInfo->m_dispId, m_crtFuncInfo->m_funcIndex, vNewParam);
						}
						// Update Channel Configuration
						CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
						*pChConfig = m_chConfig;
						m_crtFuncInfo->m_vParam  = vNewParam;
						bOk = true;
					}
					else
					{
						gLogger.QueueUpLog(L"CChannelConfigDlg::OnExternalFileUpdated ### gConfigMgr.UpdateTask(...) FAILED!");
					}
				}
				else
				{
					gLogger.QueueUpLog(L"CChannelConfigDlg::OnExternalFileUpdated ### pCrtTask->UpdateParameter(...) FAILED!");
				}
			}
			else
			{
				gLogger.QueueUpLog(L"OnExternalFileUpdated::GetNewAlgoParameters FAILED");
			}
		}
		else
		{
			gLogger.QueueUpLog(L"Read memory map file failed!");
		}
		if(bOk)
		{
			sMsg.LoadString(IDS_STRING_OPERATION_OK);
			DisplayPromptMessage(sMsg, true);
		}
		else
		{
			m_dictBtnID_File[m_nCrtBtnID] = L" ";
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
			DisplayPromptMessage(sMsg, false);
		}
		SetFileButtonImage();
	}
	if(m_hModelToolProc != NULL)
	{
		DWORD dwExitCode;
		BOOL bRet = ::GetExitCodeProcess(m_hModelToolProc, &dwExitCode);
		if(bRet == TRUE && dwExitCode == STILL_ACTIVE)
		{
			::TerminateProcess(m_hModelToolProc,4);
			CloseHandle(m_hModelToolProc);
			m_hModelToolProc = NULL;
		}
	}
	return 0;
}

long CChannelConfigDlg::GetFileEditBtnID(int nIndex)
{
	if(nIndex < 0)
		return -1;

	return IDB_EDIT_FILE0 + nIndex;
}

void CChannelConfigDlg::ModifyFile(int nBtnID)
{
	bool bModelFile = false;
	bool bNewCreate = false;
	if(m_dictBtnID_File.find(nBtnID) != m_dictBtnID_File.end())
	{
		wstring wstrFile = m_dictBtnID_File[nBtnID];
		if(wstrFile.find(L".shm") != wstring::npos)
		{
			bModelFile = true;
		}
	}
	else
	{
		bNewCreate = true;
	}
	CString sMsg;
	if(gSystemMgr.IsChannelCameraRunning(m_chConfig.m_chIndex))
	{
		if(bNewCreate || bModelFile)
		{
			sMsg.LoadString(IDS_STRING_CH_CAMERA_IS_RUNNING);
			DisplayPromptMessage(sMsg,false);
			return;
		}
	}
	if(!m_bLocked && !m_bDebugMode)
	{
		sMsg.LoadString(IDS_STRING_MSG_MODIFY_FILE_ONLY_IN_DEBUG_MODE);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	if(m_crtImage.Id() == H_EMPTY_REGION)
	{
		return;
	}
	wstring  wstrInputFile;
	bool bNewModelPath = false;

	if(m_dictBtnID_File.find(nBtnID) != m_dictBtnID_File.end())
	{
		wstrInputFile = m_dictBtnID_File[nBtnID];
 		if (wstrInputFile.find(L".shm")!= wstring::npos)
		{
			if (INVALID_FILE_ATTRIBUTES ==::GetFileAttributes(wstrInputFile.c_str()))
			{
				bNewModelPath = true;
			}
		}
		if (wstrInputFile.find(L".reg")!= wstring::npos)
		{
			int pos = wstrInputFile.find_last_of(L"\\");
			if (pos != wstring::npos)
			{
				wstring wstrInputFilePath = wstrInputFile.substr(0,pos);
				if (INVALID_FILE_ATTRIBUTES ==::GetFileAttributes(wstrInputFilePath.c_str()))
				{
					bNewModelPath = true;
				}
			}
		}
	}
	else
	{
		bNewModelPath = true;
	}

	if(bNewModelPath)
	{
		TCHAR  moduleFileName[MAX_PATH];  
		::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
		(_tcsrchr(moduleFileName, _T('\\')))[1] = 0;
		CString strPath = moduleFileName;
//		CString strPath = _T("D:\\SMVS\\");
		strPath.Append(_T("data\\"));
		CString sCh;
		sCh.Format(_T("CH%d\\"), m_chConfig.m_chIndex);
		strPath.Append(sCh);
		TASK* pCrtTask = m_chConfig.GetCurrentTask();
		if(pCrtTask != nullptr)
		{
			CString sTask = CW2T(pCrtTask->m_wstrTaskName.c_str());
			strPath.Append(sTask);
		}
		wstrInputFile = CT2W(strPath);

		//新建路径扩展算法，直接传参，新的模板路径
		if (m_bModelFiles)
		{
			if(m_dictBtnID_File.find(nBtnID) != m_dictBtnID_File.end())
			{
				wstring wstrInputFile_old = m_dictBtnID_File[nBtnID];
				if (wstrInputFile_old.find(wstrInputFile)!= wstring::npos)
				{
					wstrInputFile = wstrInputFile_old;
				}
			}
		}
	}
	m_hModelToolProc = NULL;
	string strImgPath = "C:\\tmp.tiff";
	
	Hobject ZoomShowImage;
	m_pProcessStation->GetCrtDebugImageObject(ZoomShowImage);
	write_image(ZoomShowImage, "tiff", 0, strImgPath.c_str());

	Sleep(20);

	int nProcessID = utils::GetProcessIdFromName(_T("ModelTool.exe"));
	if(nProcessID != 0)
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,nProcessID);
		while (hProcess)
		{
			TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);

			nProcessID = utils::GetProcessIdFromName(_T("ModelTool.exe"));
			if(nProcessID != 0)
			{
				hProcess = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,nProcessID);
			}
			else
			{
				break;
			}
		}
	}
	CString strCmd;

	HMODULE module = GetModuleHandle(0); 
	TCHAR pFileName[MAX_PATH]; 
	GetModuleFileName(module, pFileName, MAX_PATH); 
 
	CString csFullPath(pFileName); 
	int nPos = csFullPath.ReverseFind(_T('\\')); 
	if(nPos <=0)
		return;
	CString path = csFullPath.Left(nPos); 
	strCmd.Append(_T("\""));
	strCmd.Append(path);
	strCmd.Append(_T("\\ModelTool.exe\" "));
	CString sImgFile = CA2T(strImgPath.c_str());
	strCmd.Append(sImgFile);
	strCmd.Append(_T("#"));
	CString strCmdLine = CW2T(wstrInputFile.c_str());
	strCmd.Append(strCmdLine);
	// Channel Window Handle
	wstring wstrWndHandle = boost::lexical_cast<wstring>((long long)m_hWnd);
	strCmd.Append(_T("@"));
	strCmd.Append(wstrWndHandle.c_str());

	strCmd.Append(_T("@"));
	CString sFileType;
	sFileType.Format(_T("%d"),m_dictBtnID_FileType[nBtnID]);
	strCmd.Append(sFileType);

	//Language 
	strCmd.Append(_T("@"));
	int nLangID = ::gConfigMgr.GetLanguageID();
	if(nLangID == LANG_CHS)
	{
		strCmd.Append(_T("CHS"));
	}
	else
	{
		strCmd.Append(_T("ENU"));
	}

	//
#if defined (BATTERY_PRODUCT)
	strCmd.Append(_T("#"));
	strCmd.Append(_T("CircleShape"));
	strCmd.Append(_T("#"));
	strCmd.Append(_T("CSM1"));			//Ignore polarity to create shape model
	if (m_crtFuncInfo->m_bEnable == true)
	{

		CString strXY;
		m_pProcessStation->GetCenterLocation(strXY);
		strCmd.Append(_T("#"));
		strCmd.Append(strXY);
	}
#endif
#if defined (PCC_PRODUCT)
	strCmd.Append(_T("#"));
	strCmd.Append(_T("CircleShape"));
	if (m_crtFuncInfo->m_bEnable == true)
	{

		CString strXY;
		m_pProcessStation->GetCenterLocation(strXY);
		strCmd.Append(_T("#"));
		strCmd.Append(strXY);
	}
#endif
	
#if defined (BOTTLE_PRODUCT)
	strCmd.Append(_T("#"));
	strCmd.Append(_T("CircleShape"));
	if (m_crtFuncInfo->m_bEnable == true)
	{

		CString strXY;
		m_pProcessStation->GetCenterLocation(strXY);
		strCmd.Append(_T("#"));
		strCmd.Append(strXY);
	}
#endif

#if defined (AL_PLASTIC_PRODUCT)
	strCmd.Append(_T("#"));
	strCmd.Append(_T("CircleShape"));
	strCmd.Append(_T("#"));
	strCmd.Append(_T("CSM1"));			//Ignore polarity to create shape model
	if (m_crtFuncInfo->m_bEnable == true)
	{

		CString strXY;
		m_pProcessStation->GetCenterLocation(strXY);
		strCmd.Append(_T("#"));
		strCmd.Append(strXY);
	}
#endif

#if defined (EOE_PRODUCT)
	int nPrior1 = m_crtFuncIndex - 1;
	int iAngle1 = -1;
	for (int index1=0;index1<m_crtFuncIndex;index1++)
	{
		FUNC_INFO* ptrPriorFunc = &(*m_pFuncInfoArray)[index1];

		if (ptrPriorFunc->m_bEnable == true &&(ptrPriorFunc->m_strFuncName.find(L"PhaseCorrelationRectangularAlgo") != wstring::npos || ptrPriorFunc->m_strFuncName.find(L"RegistrateRectangleAlgo") != wstring::npos))
		{
			iAngle1 = 1;
			break;

		}
	}
	if(iAngle1 ==1)
	{
		strCmd.Append(_T("#"));
		strCmd.Append(_T("RectShape"));
	}
#endif

	//模板配置工具,角度传参
#if  defined (EOE_PRODUCT)||defined (PLUG_PRODUCT)||defined (HDPE_PRODUCT) ||defined (MAT_PRODUCT)||defined (BATTERY_PRODUCT) ||defined (BOTTLE_PRODUCT)||defined (AL_PLASTIC_PRODUCT)
	if(m_crtFuncInfo->m_bTestFunc == false)
	{
		int nPrior = m_crtFuncIndex - 1;
		bool isReadXY;
		isReadXY=true;
		int iAngle = -1;
		for (int index=0;index<m_crtFuncIndex;index++)
		{
			FUNC_INFO* ptrPriorFunc = &(*m_pFuncInfoArray)[index];

			if (ptrPriorFunc->m_bEnable == true)
			{
				if(ptrPriorFunc->m_strFuncName.find(L"BarLocationAlgo") != wstring::npos ||
					ptrPriorFunc->m_strFuncName.find(L"DetectSmalCircleAngle") != wstring::npos||
					ptrPriorFunc->m_strFuncName.find(L"AngleDetectAlgo") != wstring::npos||
					ptrPriorFunc->m_strFuncName.find(L"BottomDot") != wstring::npos)
				{
					iAngle++;
				}
				if(ptrPriorFunc->m_strFuncName.find(L"MatchingPara") != wstring::npos)
				{
					isReadXY=false;
				}
			}

		}
		if(nPrior >= 0)
		{
			if(isReadXY)
			{
				CString strXY;
				m_pProcessStation->GetSpecialParamsStr(strXY);

				std::vector<wstring> vPara = utils::SplitString(strXY.GetString(), L"$");
				strCmd.Append(_T("@"));
				strCmd.Append(_T("8"));
				strCmd.Append(_T(","));

				if (vPara.size()!=1)
				{
					strCmd.Append(vPara[iAngle].c_str());
				}
				else
				{
					strCmd.Append(vPara[0].c_str());
				}

			}
		}
	}
#endif
	//最新版代码移植，EOE项目模板匹配失败情况下的特殊处理！
#ifdef EOE_PRODUCT

	if (m_crtFuncInfo->m_bEnable == true)
	{
		if(m_crtFuncInfo->m_bTestFunc != false)
		{
			if(m_crtFuncInfo->m_strFuncName.find(L"MatchingPara") != wstring::npos)
			{
				CString strXY;
				m_pProcessStation->GetCenterLocation(strXY);
				strCmd.Append(_T("@"));
				strCmd.Append(_T("8"));
				strCmd.Append(_T(","));
				strCmd.Append(strXY);
			}
		}
	}
#endif

	//modelfiles index
	if (m_bModelFiles)
	{
		strCmd.Append(_T("@"));
		CString strIndex;
		strIndex.Format(_T("#%d"),m_iCameraIndex);
		strCmd.Append(strIndex);
	}

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si,sizeof(si)); 
	si.cb            =   sizeof(STARTUPINFO); 
	si.dwXCountChars = 500;    
	si.dwYCountChars = 500;    
	si.dwFlags       = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES|STARTF_FORCEONFEEDBACK;
	si.wShowWindow   = SW_NORMAL;  
	si.hStdInput     = stdin;
	si.hStdOutput    = stdout;    
	si.hStdError     = stderr;

	wstring wstrCmd = strCmd.GetString();
	BOOL fRet = CreateProcess(NULL,(LPWSTR)wstrCmd.c_str(),NULL,NULL,TRUE,NULL ,NULL,NULL,&si,&(pi));
	if(fRet)   
	{   
		CloseHandle(pi.hThread);
		m_hModelToolProc = pi.hProcess;
	}   
	else   
	{   
		AfxMessageBox(_T("TODO:Add message here!"));  
	}
}

void CChannelConfigDlg::UpdateFuncInputFile(FUNC_INFO* pFi, int nFileIndex, wstring wstrNewFile)
{
	if(pFi == nullptr) return;

	int nIndex = 0;
	for(size_t t = 0; t < pFi->m_vParam.size(); t++)
	{
		auto param = pFi->m_vParam[t];
		if(param.m_type == VT_BSTR)
		{
			if(nIndex == nFileIndex)
			{
				pFi->m_vParam[t].m_wstrFile = wstrNewFile;
				pFi->CreateComParamArray();
				return;
			}
			nIndex++;
		}
	}
}

wstring CChannelConfigDlg::GetSelfLearningParam()
{
	wstring wstrParam(L"");
	if(m_settingType != T_ALGO || m_vParamEditCtrl.empty())
	{
		return wstrParam;
	}
	CString sValue;
	for(size_t t = 0; t < m_vParamEditCtrl.size(); t++)
	{
		CWnd* pWnd = m_vParamEditCtrl[t];
		if(pWnd != nullptr)
		{
			pWnd->GetWindowText(sValue);
			wstring wstrItem = CT2W(sValue);
			wstrParam.append(wstrItem);
			wstrParam.append(L",");
		}
	}
	size_t len = wstrParam.length();
	if(len > 1)
	{
		wstrParam = wstrParam.substr(0, len - 1);
	}
	return wstrParam;
}

void CChannelConfigDlg::OnStnClickedStaticEditFile0()
{
	m_nCrtBtnID = IDB_EDIT_FILE0;
	ModifyFile(m_nCrtBtnID);
}
void CChannelConfigDlg::OnStnClickedStaticEditFile1()
{
	m_nCrtBtnID = IDB_EDIT_FILE1;
	ModifyFile(m_nCrtBtnID);
}
void CChannelConfigDlg::OnStnClickedStaticEditFile2()
{
	m_nCrtBtnID = IDB_EDIT_FILE2;
	ModifyFile(m_nCrtBtnID);
}
void CChannelConfigDlg::OnStnClickedStaticEditFile3()
{
	m_nCrtBtnID = IDB_EDIT_FILE3;
	ModifyFile(m_nCrtBtnID);
}
void CChannelConfigDlg::OnStnClickedStaticEditFile4()
{
	m_nCrtBtnID = IDB_EDIT_FILE4;
	ModifyFile(m_nCrtBtnID);
}
void CChannelConfigDlg::OnStnClickedStaticEditFile5()
{
	m_nCrtBtnID = IDB_EDIT_FILE5;
	ModifyFile(m_nCrtBtnID);
}
void CChannelConfigDlg::OnStnClickedStaticEditFile6()
{
	m_nCrtBtnID = IDB_EDIT_FILE6;
	ModifyFile(m_nCrtBtnID);
}
void CChannelConfigDlg::OnStnClickedStaticEditFile7()
{
	m_nCrtBtnID = IDB_EDIT_FILE7;
	ModifyFile(m_nCrtBtnID);
}

void CChannelConfigDlg::ShowLightControllerWidget(BOOL bVisible)
{
	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);

	for(size_t i = 0; i < m_vLcStaticPtr.size(); i++)
	{
		CWnd* pWnd = m_vLcStaticPtr[i];
		pWnd->ShowWindow(bVisible);
		if(bVisible == SW_SHOW)
		{
			CRect rc;
			pWnd->GetWindowRect(rc);
			InvalidateRect(rc);
		}
	}
	for(size_t t = 0; t < m_vLcWidgetPtr.size(); t++)
	{
		CWnd* pWnd = m_vLcWidgetPtr[t];
		if(pWnd != nullptr)
		{
			int nCtrlID = pWnd->GetDlgCtrlID();
			pWnd->EnableWindow(bAdmin);
			pWnd->ShowWindow(bVisible);
			if(bVisible == SW_SHOW)
			{
				if(nCtrlID != IDC_EDIT_TAG1)
				{
					CRect rc;
					pWnd->GetWindowRect(rc);
					InvalidateRect(rc);
				}
			}
		}
	}
	for(size_t t = 0; t < m_vLcTagPtr.size(); t++)
	{
		CWnd* pWnd = m_vLcTagPtr[t];
		if(pWnd != nullptr)
		{
			int nCtrlID = pWnd->GetDlgCtrlID();
			pWnd->ShowWindow(bVisible);
		}
	}
	for(size_t i = 0; i < m_vLcSliderPtr.size(); i++)
	{
		CWnd* pWnd = m_vLcSliderPtr[i];
		if(pWnd != nullptr)
		{
			pWnd->ShowWindow(bVisible);
			pWnd->EnableWindow(i != 0 && bAdmin);
			if(bVisible == SW_SHOW)
			{
				CRect rc;
				pWnd->GetWindowRect(rc);
				InvalidateRect(rc);
			}
		}
	}

	m_bCheckSingleMode.ShowWindow(bVisible);
	m_bCheckSingleMode.EnableWindow(bVisible && bAdmin);
}

void CChannelConfigDlg::OnCbnSelchangeComboLcOption()
{
	if (m_settingType != T_LIGHT)
	{
		return;
	}
	LIGHT_SETTING& ls = m_chConfig.m_lightCtrlSetting;

	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(pCrtTask == nullptr)
	{
		return;
	}

	/////////////////////////////////////////////////////////

	UpdateSingleMode();//更新标记位
	//OnBnClickedCheckLightSingleMode();// 
	if(!m_bSingleMode)
	{
		InitLightControllerWidget();
		ShowLightControllerWidget(SW_SHOW);
		SetSingleModeControlShow(false);
		RefreshUiCtrl(&m_paramBoard);
	}
	else
	{

		ShowLightControllerWidget(SW_HIDE);
		SetSingleModeControlShow(true);
		InitLightSingleModeCtrlPos();
		ShowLightControllerWidget(SW_SHOW);
		RefreshUiCtrl(&m_paramBoard);
	}
	int nIndex = m_comboIDs.GetCurSel();
	if(nIndex >= 0)
	{
		CString sValue;
		m_comboIDs.GetLBText(nIndex, sValue);
		m_nCrtLcID     = (int)utils::to_float(sValue);
		wstring wstrTags(L"");
		if(nIndex < (int)ls.m_vTags.size())
		{
			wstrTags = ls.m_vTags[nIndex];
		}
		DisplayLightControllerParameter(ls,m_chConfig.m_crtTaskName, m_nCrtLcID, wstrTags,true);
	}
	UpdateData(FALSE);

}

void CChannelConfigDlg::OnCbnSelchangeComboLcMode()
{
	LIGHT_SETTING& ls = m_chConfig.m_lightCtrlSetting;
	m_nCrtMode = m_comboMode.GetCurSel();////////twice

	int nIndex = m_comboIDs.GetCurSel();
	if(nIndex >= 0)
	{
		CString sValue;
		m_comboIDs.GetLBText(nIndex, sValue);
		m_nCrtLcID     = (int)utils::to_float(sValue);
		wstring wstrTags(L"");
		if(nIndex < (int)ls.m_vTags.size()) 
		{
			wstrTags = ls.m_vTags[nIndex];
			//获取当前控制id，对应模式的 参数字符串
		}
		DisplayLightControllerParameter(ls,m_chConfig.m_crtTaskName, m_nCrtLcID, wstrTags);
	}
	ApplyNewLcParameter();
}

void CChannelConfigDlg::UpdateSliderPostionPanel(CSliderControl* pSlider)
{
	CWnd* pWnd = (CWnd*)pSlider;
	if(m_dictSlider_Value.find(pWnd) != m_dictSlider_Value.end())
	{
		int nPos = pSlider->GetPos();
		CWnd* pValueCtrl = m_dictSlider_Value[pWnd];
		int nValueCtrlID = pValueCtrl->GetDlgCtrlID();

		int nSliderID = pSlider->GetDlgCtrlID();
		if(nSliderID == IDC_SLIDER_DELAY||
			nSliderID == IDC_SLIDER_DELAY2||
			nSliderID == IDC_SLIDER_DELAY3||
			nSliderID == IDC_SLIDER_DELAY4)
		{
			float fValue = nPos / 10.0f;
			CString sValue;
			sValue.Format(_T("%.1f"), fValue);
			pValueCtrl->SetWindowText(sValue);

			if (m_bOneTouchFour)
			{
				((CSliderControl* )GetDlgItem(IDC_SLIDER_DELAY))->SetPos(nPos);
				((CSliderControl* )GetDlgItem(IDC_SLIDER_DELAY2))->SetPos(nPos);
				((CSliderControl* )GetDlgItem(IDC_SLIDER_DELAY3))->SetPos(nPos);
				((CSliderControl* )GetDlgItem(IDC_SLIDER_DELAY4))->SetPos(nPos);

				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DELAY))->SetWindowText(sValue);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DELAY2))->SetWindowText(sValue);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DELAY3))->SetWindowText(sValue);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DELAY4))->SetWindowText(sValue);

				CRect rc;
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DELAY))->GetWindowRect(rc);
				InvalidateRect(rc);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DELAY2))->GetWindowRect(rc);
				InvalidateRect(rc);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DELAY3))->GetWindowRect(rc);
				InvalidateRect(rc);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DELAY4))->GetWindowRect(rc);
				InvalidateRect(rc);
			}

		}
		else if(nSliderID == IDC_SLIDER_DURATION||
			nSliderID == IDC_SLIDER_DURATION2||
			nSliderID == IDC_SLIDER_DURATION3||
			nSliderID == IDC_SLIDER_DURATION4)
		{
			SetDlgItemInt(nValueCtrlID, nPos);

			if (m_bOneTouchFour)
			{
				((CSliderControl* )GetDlgItem(IDC_SLIDER_DURATION))->SetPos(nPos);
				((CSliderControl* )GetDlgItem(IDC_SLIDER_DURATION2))->SetPos(nPos);
				((CSliderControl* )GetDlgItem(IDC_SLIDER_DURATION3))->SetPos(nPos);
				((CSliderControl* )GetDlgItem(IDC_SLIDER_DURATION4))->SetPos(nPos);

				SetDlgItemInt(IDC_STATIC_VALUE_DURATION,nPos);
				SetDlgItemInt(IDC_STATIC_VALUE_DURATION2,nPos);
				SetDlgItemInt(IDC_STATIC_VALUE_DURATION3,nPos);
				SetDlgItemInt(IDC_STATIC_VALUE_DURATION4,nPos);
		

				CRect rc;
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DURATION))->GetWindowRect(rc);
				InvalidateRect(rc);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DURATION2))->GetWindowRect(rc);
				InvalidateRect(rc);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DURATION3))->GetWindowRect(rc);
				InvalidateRect(rc);
				((CSliderControl* )GetDlgItem(IDC_STATIC_VALUE_DURATION4))->GetWindowRect(rc);
				InvalidateRect(rc);
			}
		}
		else
		{
			SetDlgItemInt(nValueCtrlID, nPos);
		}
		CRect rc;
		pValueCtrl->GetWindowRect(rc);
		InvalidateRect(rc);
	}
}

void CChannelConfigDlg::OnHScroll(UINT nSBCode,UINT nPos,CScrollBar *pScrollBar)
{ 
	if(m_settingType == T_LIGHT)
	{
		m_bLsChanged = true;
		m_pCrtSlider = (CSliderControl*)pScrollBar;
		UpdateSliderPostionPanel(m_pCrtSlider);
	}
}

void CChannelConfigDlg::CollectLcChannelData(LC& lc)
{
	int nDelay = m_sliderDelay.GetPos();
	int nDur   = m_sliderDur.GetPos();

	if(lc.m_nMode == LC_TRIGGER_TWICE)
	{
		lc.m_vChParam[0].m_nLum1      = m_sliderLum1.GetPos();
		lc.m_vChParam[0].m_nDelay1    = nDelay;
		lc.m_vChParam[0].m_nDuration1 = nDur;

		lc.m_vChParam[1].m_nLum1      = m_sliderLum2.GetPos();
		lc.m_vChParam[1].m_nDelay1    = m_sliderDelay2.GetPos();;
		lc.m_vChParam[1].m_nDuration1 = m_sliderDur2.GetPos();

		lc.m_vChParam[2].m_nLum1      = m_sliderLum3.GetPos();
		lc.m_vChParam[2].m_nDelay1    = m_sliderDelay3.GetPos();
		lc.m_vChParam[2].m_nDuration1 = m_sliderDur3.GetPos();//

		lc.m_vChParam[3].m_nLum1      = m_sliderLum4.GetPos();
		lc.m_vChParam[3].m_nDelay1    = m_sliderDelay4.GetPos();
		lc.m_vChParam[3].m_nDuration1 = m_sliderDur4.GetPos();
	}
	else
	{
		if (lc.m_nActiveID == 0|| m_bOneTouchFour)
		{
			lc.m_vChParam[0].m_nLum      = m_sliderLum1.GetPos();
			lc.m_vChParam[0].m_nDelay    = nDelay;
			lc.m_vChParam[0].m_nDuration = nDur;

			lc.m_vChParam[1].m_nLum      = m_sliderLum2.GetPos();
			lc.m_vChParam[1].m_nDelay    = nDelay;
			lc.m_vChParam[1].m_nDuration = nDur;

			lc.m_vChParam[2].m_nLum      = m_sliderLum3.GetPos();
			lc.m_vChParam[2].m_nDelay    = nDelay;
			lc.m_vChParam[2].m_nDuration = nDur;

			lc.m_vChParam[3].m_nLum      = m_sliderLum4.GetPos();
			lc.m_vChParam[3].m_nDelay    = nDelay;
			lc.m_vChParam[3].m_nDuration = nDur;
		}
		else
		{
			lc.m_vChParam[0].m_nLum      = m_sliderLum1.GetPos();
			lc.m_vChParam[0].m_nDelay    = nDelay;
			lc.m_vChParam[0].m_nDuration = nDur;

			lc.m_vChParam[1].m_nLum      = m_sliderLum2.GetPos();
			lc.m_vChParam[1].m_nDelay    = m_sliderDelay2.GetPos();
			lc.m_vChParam[1].m_nDuration = m_sliderDur2.GetPos();;

			lc.m_vChParam[2].m_nLum      = m_sliderLum3.GetPos();
			lc.m_vChParam[2].m_nDelay    = m_sliderDelay3.GetPos();
			lc.m_vChParam[2].m_nDuration = m_sliderDur3.GetPos();;

			lc.m_vChParam[3].m_nLum      = m_sliderLum4.GetPos();
			lc.m_vChParam[3].m_nDelay    = m_sliderDelay4.GetPos();
			lc.m_vChParam[3].m_nDuration = m_sliderDur4.GetPos();;
		}

	}

}

LC CChannelConfigDlg::GetLightControllerParameter()
{
	LC retLc;
	std::map<wstring, vector<LC>>& dictParamGroup = m_chConfig.m_lightCtrlSetting.m_dictParamGroup;
	if(dictParamGroup.find(m_chConfig.m_crtTaskName) == dictParamGroup.end())
	{
		dictParamGroup[m_chConfig.m_crtTaskName] = std::vector<LC>();
	}
	if(dictParamGroup.find(m_chConfig.m_crtTaskName) != dictParamGroup.end())
	{
		std::vector<LC>& vLC = dictParamGroup[m_chConfig.m_crtTaskName];
		auto it = std::find_if(vLC.begin(), vLC.end(), [&](LC lc) -> bool
		{
			return (lc.m_nID == m_nCrtLcID);
		});

		if(it == vLC.end())
		{
			LC lc;
			lc.m_nID      = m_nCrtLcID;
			lc.m_nMode    = m_nCrtMode;
			lc.m_nActiveID    = m_nActiveID;
			lc.m_vChParam = std::vector<LC_CH_PARAM>(4,LC_CH_PARAM());
			CollectLcChannelData(lc);
			vLC.push_back(lc);
			return lc;
		}
		for(size_t t = 0; t< vLC.size(); t++)
		{
			LC& lc = vLC[t];
			if(lc.m_nID == m_nCrtLcID)
			{
				lc.m_nMode = m_nCrtMode;
				lc.m_nActiveID = m_nActiveID;
				CollectLcChannelData(lc);
				retLc = lc;
				break;
			}
		}
	}
	return retLc;
}

void CChannelConfigDlg::WriteNewLcParameter2Device()
{
	LIGHT_SETTING& ls = m_chConfig.m_lightCtrlSetting;
	if(ls.m_dictParamGroup.find(m_chConfig.m_crtTaskName) != ls.m_dictParamGroup.end())
	{
		std::vector<LC>& vLC = ls.m_dictParamGroup[m_chConfig.m_crtTaskName];
		for(size_t t = 0; t < vLC.size(); t++)
		{
			gSystemMgr.WriteLcParameter2EEP(vLC[t]);
			Sleep(100);
		}
	}
}

void CChannelConfigDlg::ApplyNewLcParameter(int CHID)
{
	LC lc = GetLightControllerParameter();
	if(!gSystemMgr.ApplyNewLcParameter(lc,CHID))
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		DisplayPromptMessage(sMsg, false);
	}
	else
	{
		m_bLsChanged = true;
		gSystemMgr.UpdateChLightController(m_chConfig.m_chIndex, m_chConfig.m_lightCtrlSetting);
	}
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderDelay(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(1);
	}
	else
	{
		ApplyNewLcParameter();
	}

	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderDuration(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(1);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderLum1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(1);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderLum2(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(2);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderLum3(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(3);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderLum4(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(4);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

int CChannelConfigDlg::GetDefaultNewRegionID()
{
	 if(m_pProcessStation != nullptr)
	 {
		 std::set<int> setRgnIDs;
		 auto pFuncInfoArray = m_pProcessStation->GetFuncInfoArrayPtr();
		 if(pFuncInfoArray != nullptr)
		 {
			 for(size_t t = 0; t < pFuncInfoArray->size(); t++)
			 {
				 FUNC_INFO fi = (*pFuncInfoArray)[t];
				 setRgnIDs.insert(fi.GetTargetRegionID());
			 }
			 int rgnID = 0;
			 while(setRgnIDs.find(rgnID) != setRgnIDs.end())
			 {
				 rgnID++;
			 }
			 return rgnID;
		 }
	 }
	return -1;
}

bool CChannelConfigDlg::AddNewFunc(const FUNC_INFO& newFi)
{
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(newFi.m_funcIndex == m_pFuncInfoArray->size())
	{
		m_pFuncInfoArray->push_back(newFi);
		IT_PARAM newIP = utils::GenerateItParam(newFi);
		pCrtTask->m_vItParam.push_back(newIP);
	}
	else
	{
		std::vector<FUNC_INFO> tmpFuncArray = *m_pFuncInfoArray;
		m_pFuncInfoArray->clear();

		std::vector<IT_PARAM> tmpIP = pCrtTask->m_vItParam;
		pCrtTask->m_vItParam.clear();

		size_t szIP = tmpIP.size();
		size_t szFI = tmpFuncArray.size();

		for(size_t i = 0; i < szIP; i++)
		{
			FUNC_INFO fi = tmpFuncArray[i];
			IT_PARAM  ip = tmpIP[i];
			if(fi.m_funcIndex < newFi.m_funcIndex)
			{
				m_pFuncInfoArray->push_back(fi);
				pCrtTask->m_vItParam.push_back(ip);
			}
			else if(fi.m_funcIndex == newFi.m_funcIndex)
			{
				m_pFuncInfoArray->push_back(newFi);
				IT_PARAM newIP = utils::GenerateItParam(newFi);
				pCrtTask->m_vItParam.push_back(newIP);

				fi.m_funcIndex++;
				m_pFuncInfoArray->push_back(fi);

				int nTmp = boost::lexical_cast<int>(ip.m_itIndex);
				nTmp++;
				ip.m_itIndex = boost::lexical_cast<wstring>(nTmp);
				pCrtTask->m_vItParam.push_back(ip);
			}
			else
			{
				fi.m_funcIndex++;
				m_pFuncInfoArray->push_back(fi);

				int nTmp = boost::lexical_cast<int>(ip.m_itIndex);
				nTmp++;
				ip.m_itIndex = boost::lexical_cast<wstring>(nTmp);
				pCrtTask->m_vItParam.push_back(ip);
			}
		}
	}
	m_pChannelControlPanel->ModifyInternalStats(newFi.m_funcIndex, true, newFi.m_bTestFunc);
 	m_pDebugFuncInfoArray->clear();
 	m_pDebugFuncInfoArray->assign(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end());
	m_algoList.DeleteAllItems();
	DeleteAllSettingItem();
	AddRoiFunc2List();
	for (size_t i = 0; i < m_VecpProcessStation.size();i++)
	{
		m_VecpProcessStation[i]->ResetDisplayRule();
	}
	return true;
}

bool CChannelConfigDlg::DeleteFunc(int crtFuncIndex,eTASK_OPERATION to)
{
	if(m_pFuncInfoArray == nullptr)
	{
		return false;
	}
	TASK* pTask = m_chConfig.GetCurrentTask();
	size_t szFI = m_pFuncInfoArray->size();

	std::vector<IT_PARAM>& vItParam = pTask->m_vItParam;
	size_t szIP = vItParam.size();
	if(szFI != szIP || crtFuncIndex < 0 || crtFuncIndex >= szFI)
	{
		return false;
	}
	FUNC_INFO fi = (*m_pFuncInfoArray)[crtFuncIndex];
	int nRgnID = fi.GetTargetRegionID();
	if(to == TO_DEL_ITEM)
	{
		if(fi.m_bTestFunc)
		{
			long nCount = (long)std::count_if(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end(), [&](FUNC_INFO& f) -> bool
			{
				return (nRgnID == f.GetTargetRegionID());
			});
// 			if(nCount == 2)
// 			{
// 				return DeleteFunc(crtFuncIndex,TO_DEL_REGION);
// 			}
		}

		CString strLog;
		strLog.Format(_T("Channel %d - Delete testing item : %s"),m_chConfig.m_chIndex,fi.m_strCustomizedName.c_str());
		gLogger.QueueUpLog(strLog.GetString(),true);

		m_pChannelControlPanel->ModifyInternalStats(m_nCrtFuncIndex, false, fi.m_bTestFunc);
		m_pFuncInfoArray->erase(std::remove_if(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end(), [&](FUNC_INFO f) -> bool
		{
			return (f.m_funcIndex == fi.m_funcIndex);
		}), m_pFuncInfoArray->end());

		std::for_each(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end(), [&](FUNC_INFO& f)
		{
			if(f.m_funcIndex > fi.m_funcIndex)
			{
				f.m_funcIndex--;
			}
		});

		wstring wstrIndex = boost::lexical_cast<wstring>(crtFuncIndex);
		vItParam.erase(std::remove_if(vItParam.begin(), vItParam.end(), [&](IT_PARAM& ip) -> bool
		{
			return (ip.m_itIndex == wstrIndex);
		}), vItParam.end());
		std::for_each(vItParam.begin(), vItParam.end(), [&](IT_PARAM& ip)
		{
			int nIndex = boost::lexical_cast<int>(ip.m_itIndex);
			if(nIndex > crtFuncIndex)
			{
				ip.m_itIndex = boost::lexical_cast<wstring>(--nIndex);
			}
		});

	}
	if(to == TO_DEL_REGION)
	{
		std::vector<int> vRgnFuncIndex;
		m_pFuncInfoArray->erase(std::remove_if(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end(), [&](FUNC_INFO f) -> bool
		{
			if(nRgnID == f.GetTargetRegionID())
			{
				vRgnFuncIndex.push_back(f.m_funcIndex);

				CString strLog;
                strLog.Format(_T("Channel %d - Delete region item : %s"),m_chConfig.m_chIndex,f.m_strCustomizedName.c_str());
				gLogger.QueueUpLog(strLog.GetString(),true);
				return true;
			}
			return false;
		}), m_pFuncInfoArray->end());

		m_pChannelControlPanel->RemoveInternalStats(vRgnFuncIndex);

		int nIndex = 0;
		std::for_each(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end(), [&](FUNC_INFO& f)
		{
			f.m_funcIndex = nIndex;
			nIndex++;
		});
		wstring wstrRgnID = boost::lexical_cast<wstring>(nRgnID);
		vItParam.erase(std::remove_if(vItParam.begin(), vItParam.end(), [&](IT_PARAM& ip) -> bool
		{
			return (wstrRgnID == ip.m_vParam[1]);
		}), vItParam.end());

		nIndex = 0;
		std::for_each(vItParam.begin(), vItParam.end(), [&](IT_PARAM& ip)
		{
			ip.m_itIndex = boost::lexical_cast<wstring>(nIndex);
			nIndex++;
		});
	}

 	m_pDebugFuncInfoArray->clear();
 	m_pDebugFuncInfoArray->assign(m_pFuncInfoArray->begin(), m_pFuncInfoArray->end());
	m_algoList.DeleteAllItems();
	DeleteAllSettingItem();
	AddRoiFunc2List();
	for (size_t i = 0; i < m_VecpProcessStation.size();i++)
	{
		m_VecpProcessStation[i]->ResetDisplayRule();
	}
	return true;
}

bool CChannelConfigDlg::IsAnySingledRoiFunc(wstring& wtrRgnName)
{
	if(m_pFuncInfoArray == nullptr)
	{
		return false;
	}
	size_t sz = m_pFuncInfoArray->size();
	int nRoiCount = 0;
	for(size_t t = 1; t < sz; t++)
	{
		FUNC_INFO fi = (*m_pFuncInfoArray)[t];
		if(!fi.m_bTestFunc && SingledRoiAlgo(fi))
		{
			wtrRgnName = fi.m_strCustomizedName;
			return true;
		}
	}
	return false;
}

void CChannelConfigDlg::UpdateStationFuncInfo()
{
	TASK crtTask;
	m_chConfig.GetCurrentTask(crtTask); 
	gConfigMgr.UpdateTask(m_chConfig.m_chIndex, crtTask);

	std::for_each(m_VecpProcessStation.begin(),m_VecpProcessStation.end(), [&](CImageProcessor* pStation)
	{
		if(pStation != nullptr)
		{
			pStation->SetTask(crtTask);
			pStation->UpdateImageProcessFuncInfo(m_pFuncInfoArray);

		}
	});

	if(m_pChannelControlPanel != NULL)
	{
		m_pChannelControlPanel->UpdateFunInfo(m_pFuncInfoArray,m_pFuncInfoArray);
	}

	m_vFuncInfo = *m_pFuncInfoArray;
}

void CChannelConfigDlg::OnAddRegion()
{
	CString sMsg;
	wstring wstrRgnName;
	if(IsAnySingledRoiFunc(wstrRgnName))
	{
		sMsg.LoadString(IDS_STRING_INVALID_ROI);
		sMsg.Replace(_T("#1"), wstrRgnName.c_str());
		DisplayPromptMessage(sMsg, false);
		return;
	}
	int nNewRgnID = GetDefaultNewRegionID();
	
	FUNC_INFO retFI;
	TASK oldTask;
	m_chConfig.GetCurrentTask(oldTask);
	std::set<wstring> setNickname = oldTask.GetItemNicknames();
	CTestItemManagerDlg tim(oldTask.m_nProductId, TO_ADD_REGION, nNewRgnID, setNickname,retFI);

	if(tim.DoModal() == IDOK)
	{
		retFI.m_funcIndex = GetTargetFuncIndex(m_nCrtFuncIndex, TO_ADD_REGION);
		if(AddNewFunc(retFI))
		{
			UpdateStationFuncInfo();
			TASK crtTask;
			m_chConfig.GetCurrentTask(crtTask); 
			if(gSystemMgr.ModifyDatabaseTable(oldTask, crtTask))
			{
				CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
				*pChConfig = m_chConfig;
				sMsg.LoadString(IDS_STRING_OPERATION_OK);
				DisplayPromptMessage(sMsg,true);
				m_bAlgoChanged = true;
			}
            else
            {
                wostringstream oss;
                oss << L"Modify database failed:Channel " << m_chConfig.m_chIndex << L" manipulate database failed!";
                gLogger.QueueUpLog(oss.str(),true);
            }
		}
		else
		{
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
			DisplayPromptMessage(sMsg,false);
		}
	}
	RedrawAlgoList();
	m_nCrtRgnID     = -1;
	m_nCrtListIndex = -1;
    m_nCrtFuncIndex = -1;

}

void CChannelConfigDlg::OnDelRegion()
{
	CString sMsg;
	sMsg.LoadString(IDS_STRING_WARNING_DEL_ROI);
	if(IDNO == AfxMessageBox(sMsg, MB_YESNO, NULL))
	{
		return;
	}
	TASK oldTask;
	m_chConfig.GetCurrentTask(oldTask);
	if(DeleteFunc(m_nCrtFuncIndex, TO_DEL_REGION))
	{
		UpdateStationFuncInfo();
		TASK crtTask;
		m_chConfig.GetCurrentTask(crtTask); 
		if(gSystemMgr.ModifyDatabaseTable(oldTask, crtTask))
		{
			CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
			*pChConfig = m_chConfig;
			sMsg.LoadString(IDS_STRING_OPERATION_OK);
			DisplayPromptMessage(sMsg,true);
			m_bAlgoChanged = true;
		}
        else
        {
            wostringstream oss;
            oss << L"Modify database failed:Channel " << m_chConfig.m_chIndex << L" manipulate database failed!";
            gLogger.QueueUpLog(oss.str(),true);
        }
	}
	else
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		DisplayPromptMessage(sMsg,false);
	}
	RedrawAlgoList();
	 m_nCrtRgnID     = -1;
	 m_nCrtListIndex = -1;
	 m_nCrtFuncIndex = -1;
}

void CChannelConfigDlg::OnAddTestItem()
{
	FUNC_INFO retFI;
	CString sMsg;
	TASK oldTask;
	m_chConfig.GetCurrentTask(oldTask);
	std::set<wstring> setNickname = oldTask.GetItemNicknames();
	CTestItemManagerDlg tim(oldTask.m_nProductId,TO_ADD_ITEM,m_nCrtRgnID,setNickname, retFI);
	if(tim.DoModal() == IDOK)
	{
		retFI.m_funcIndex = GetTargetFuncIndex(m_nCrtFuncIndex, TO_ADD_ITEM);
		if(AddNewFunc(retFI))
		{
			UpdateStationFuncInfo();
			TASK crtTask;
			m_chConfig.GetCurrentTask(crtTask); 
			if(gSystemMgr.ModifyDatabaseTable(oldTask, crtTask))
			{
				CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
				*pChConfig = m_chConfig;
				sMsg.LoadString(IDS_STRING_OPERATION_OK);
				DisplayPromptMessage(sMsg,true);
				m_bAlgoChanged = true;
			}
            else
            {
                wostringstream oss;
                oss << L"Modify database failed:Channel " << m_chConfig.m_chIndex << L" manipulate database failed!";
                gLogger.QueueUpLog(oss.str(),true);
            }
		}
		else
		{
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
			DisplayPromptMessage(sMsg,false);
		}
	}
	RedrawAlgoList();
	m_nCrtRgnID     = -1;
	m_nCrtListIndex = -1;
	m_nCrtFuncIndex = -1;
}

void CChannelConfigDlg::OnDelTestItem()
{
	CString sMsg;
	sMsg.LoadString(IDS_STRING_WARNING_DEL_TEST_ITEM);
	if(IDNO == AfxMessageBox(sMsg, MB_YESNO, NULL))
	{
		return;
	}
	TASK oldTask;
	m_chConfig.GetCurrentTask(oldTask);
	if(DeleteFunc(m_nCrtFuncIndex, TO_DEL_ITEM))
	{
		UpdateStationFuncInfo();
		TASK crtTask;
		m_chConfig.GetCurrentTask(crtTask); 
		if(gSystemMgr.ModifyDatabaseTable(oldTask, crtTask))
		{
			CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
			*pChConfig = m_chConfig;
			sMsg.LoadString(IDS_STRING_OPERATION_OK);
			DisplayPromptMessage(sMsg,true);
			m_bAlgoChanged = true;
		}
        else
        {
            wostringstream oss;
            oss << L"Modify database failed:Channel " << m_chConfig.m_chIndex << L" manipulate database failed!";
            gLogger.QueueUpLog(oss.str(),true);
        }
	}
	else
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
		DisplayPromptMessage(sMsg,false);
	}
	RedrawAlgoList();
	 m_nCrtRgnID     = -1;
	 m_nCrtListIndex = -1;
	 m_nCrtFuncIndex = -1;
}

void CChannelConfigDlg::OnRenameTestItem()
{
	if(m_nCrtFuncIndex == -1)
	{
		return;
	}

	TASK oldTask;
	m_chConfig.GetCurrentTask(oldTask);
	std::set<wstring> setNickname = oldTask.GetItemNicknames();
	wstring wstrOldName = oldTask.GetFuncNickname(m_nCrtFuncIndex);
	wstring wstrNewName = L"";

	CNameModifierDlg dlg(wstrOldName, setNickname, wstrNewName);
	if(dlg.DoModal() == IDOK)
	{
		CString sMsg;
		TASK* pNewTask = m_chConfig.GetCurrentTask();
		pNewTask->SetFuncNickname(m_nCrtFuncIndex,wstrNewName);
		if(gSystemMgr.ModifyDatabaseTable(oldTask, *pNewTask))
		{
			(*m_pFuncInfoArray)[m_nCrtFuncIndex].m_strCustomizedName = wstrNewName;
			(*m_pDebugFuncInfoArray)[m_nCrtFuncIndex].m_strCustomizedName = wstrNewName;

			m_algoList.SetItemText(m_nCrtListIndex, 2, wstrNewName.c_str());

			CHANNEL_CONFIG* pChConfig = gSystemMgr.GetChConfigReference(m_chConfig.m_chIndex);
			*pChConfig = m_chConfig;

			sMsg.LoadString(IDS_STRING_OPERATION_OK);
			DisplayPromptMessage(sMsg,true);
			m_bAlgoChanged = true;
		}
		else
		{
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
			DisplayPromptMessage(sMsg,false);
            wostringstream oss;
            oss << L"Modify database failed:Channel " << m_chConfig.m_chIndex << L" rename " << wstrOldName << L" to " << wstrNewName <<L"!";
            gLogger.QueueUpLog(oss.str(),true);
		}
	}
}

int CChannelConfigDlg::GetTargetFuncIndex(int nCrtFuncIndex, eTASK_OPERATION to)
{
	int nFuncIndex = -1;
	if(m_pFuncInfoArray == nullptr)
	{
		return nFuncIndex;
	}
	int sz = (int)m_pFuncInfoArray->size();
	if(to == TO_ADD_REGION)
	{
		if(nCrtFuncIndex == sz - 1)
		{
			return sz;
		}
		int nRet = sz;
		for(int i = nCrtFuncIndex + 1; i < sz; i++)
		{
			if((*m_pFuncInfoArray)[i].m_bTestFunc == false)
			{
				return i;
			}
		}
		return sz;
	}
	else
	{
		nFuncIndex = nCrtFuncIndex + 1;
	}
	return nFuncIndex;
}

void CChannelConfigDlg::UpdateThresholdPanel()
{
	if(m_crtFuncInfo == nullptr)
	{
		return;
	}

	UAC uac = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (uac.m_nLevel == UAC_ADMIN);

	CRect rcChart;
	m_chartCtrl.GetWindowRect(rcChart);
	m_lblThreshold_Lcl.ShowWindow(SW_HIDE);
	if(!m_crtFuncInfo->m_bTestFunc)
	{
 		m_lblThreshold_Lcl.SetWindowText(_T("N/A"));
		m_lblThreshold_Lcl.ShowWindow(SW_SHOW);
		m_lblThreshold_Ucl.ShowWindow(SW_HIDE);

		

		CRect rc;

		rc.left   = 5;
		rc.top    = rcChart.top + int((rcChart.Height() - 64 * 3 - 20) / 2.0f);
		rc.right  = rc.left + 64;
		rc.bottom = rc.top + 64;
		m_btnDecrease.MoveWindow(rc);

		rc.top = rc.bottom + 10;
		rc.bottom = rc.top + 64;
		m_lblThreshold_Lcl.MoveWindow(rc);

		rc.top = rc.bottom + 10;
		rc.bottom = rc.top + 64;
		m_btnIncrease.MoveWindow(rc);
				
		EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
		EnableUiBtn(IDB_RIGHT_ARROW, false,IMG_CH_RIGHT_ARROW_GRAY);
	}
	else
	{
		VARENUM type = m_crtFuncInfo->m_threshold.m_type;
		CRect rc;
		CString strValue;
		if(m_crtFuncInfo->m_threshold.m_compType <= LESS_EQUAL)
		{
			rc.left   = 5;
			rc.top    = rcChart.top + int((rcChart.Height() - 64 * 3 - 20) / 2.0f);
			rc.right  = rc.left + 64;
			rc.bottom = rc.top + 64;
			m_btnDecrease.MoveWindow(rc);

			if(m_crtFuncInfo->m_threshold.m_specV  <= m_crtFuncInfo->m_threshold.m_min)
			{
				EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
			}
			else
			{
				EnableUiBtn(IDB_LEFT_ARROW, true,IMG_CH_LEFT_ARROW);
			}

			rc.top = rc.bottom + 10;
			rc.bottom = rc.top + 64;
			m_lblThreshold_Lcl.MoveWindow(rc);

			rc.top = rc.bottom + 10;
			rc.bottom = rc.top + 64;
			m_btnIncrease.MoveWindow(rc);

			if(m_crtFuncInfo->m_threshold.m_specV  >= m_crtFuncInfo->m_threshold.m_max)
			{
				EnableUiBtn(IDB_RIGHT_ARROW, false,IMG_CH_RIGHT_ARROW_GRAY);
			}
			else
			{
				EnableUiBtn(IDB_RIGHT_ARROW, true,IMG_CH_RIGHT_ARROW);
			}

			if(type == VT_R4)
				strValue.Format(_T("%.3f"),m_crtFuncInfo->m_threshold.m_specV);
			else
				strValue.Format(_T("%d"), (int)m_crtFuncInfo->m_threshold.m_specV);
			m_lblThreshold_Lcl.SetWindowText(strValue);
			m_lblThreshold_Lcl.ShowWindow(SW_SHOW);
			m_lblThreshold_Ucl.ShowWindow(SW_HIDE);
		}
		else
		{
			m_lblThreshold_Ucl.ShowWindow(SW_HIDE);
			rc.top = rcChart.top + int((rcChart.Height() - 4*64 - 30) / 2.0f);
			rc.left = 5;
			rc.right = rc.left + 64;
			rc.bottom = rc.top + 64;
			m_btnDecrease.MoveWindow(rc);

			if(m_crtFuncInfo->m_threshold.m_lcl <= m_crtFuncInfo->m_threshold.m_min)
			{
				EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
			}
			else
			{
				EnableUiBtn(IDB_LEFT_ARROW, true,IMG_CH_LEFT_ARROW);
			}

			rc.top = rc.bottom + 10;
			rc.bottom = rc.top + 64;
			m_lblThreshold_Lcl.MoveWindow(rc);
			if(type == VT_R4)
				strValue.Format(_T("%.3f"),m_crtFuncInfo->m_threshold.m_lcl);
			else
				strValue.Format(_T("%d"), (int)m_crtFuncInfo->m_threshold.m_lcl);
			m_lblThreshold_Lcl.SetWindowText(strValue);

			rc.top = rc.bottom + 10;
			rc.bottom = rc.top + 64;
			m_lblThreshold_Ucl.MoveWindow(rc);
			if(type == VT_R4)
				strValue.Format(_T("%.3f"),m_crtFuncInfo->m_threshold.m_ucl);
			else
				strValue.Format(_T("%d"), (int)m_crtFuncInfo->m_threshold.m_ucl);
			m_lblThreshold_Ucl.SetWindowText(strValue);

			rc.top = rc.bottom + 10;
			rc.bottom = rc.top + 64;
			m_btnIncrease.MoveWindow(rc);

			if(m_crtFuncInfo->m_threshold.m_ucl >= m_crtFuncInfo->m_threshold.m_max)
			{
				EnableUiBtn(IDB_RIGHT_ARROW, false,IMG_CH_RIGHT_ARROW_GRAY);
			}
			else
			{
				EnableUiBtn(IDB_RIGHT_ARROW, true,IMG_CH_RIGHT_ARROW);
			}


			m_lblThreshold_Lcl.ShowWindow(SW_SHOW);
			m_lblThreshold_Ucl.ShowWindow(SW_SHOW);
		}
	}
}

LRESULT  CChannelConfigDlg::OnThresholdChanged(WPARAM wParam,LPARAM lParam)
{
	m_editThreshold.ShowWindow(SW_HIDE);
	if(m_settingType != T_ALGO || m_crtFuncInfo == nullptr)
		return -1;

	float newThreshold = (float)lParam;
	float fMaxX = (float)wParam;
	newThreshold /= 100.0f;
	fMaxX /= 100.0f;
	if(fMaxX > m_crtFuncInfo->m_threshold.m_max)
	{
		fMaxX = m_crtFuncInfo->m_threshold.m_max;
	}

	if(newThreshold < m_crtFuncInfo->m_threshold.m_min)
	{
		newThreshold = m_crtFuncInfo->m_threshold.m_min;
		EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
	}
	if(newThreshold >= fMaxX)
	{
		newThreshold = fMaxX;
		EnableUiBtn(IDB_RIGHT_ARROW, false, IMG_CH_RIGHT_ARROW_GRAY);
	}
	m_crtFuncInfo->m_threshold.m_specV      = newThreshold;
	m_crtFuncInfoDebug->m_threshold.m_specV = newThreshold;

	m_chartCtrl.SetThresholdValue(m_crtFuncInfo->m_threshold);
	m_chartCtrl.Invalidate();

	m_bAlgoChanged = true;
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(pCrtTask != nullptr)
	{
		pCrtTask->UpdateThresholdValue(m_crtFuncInfo->m_funcIndex, m_crtFuncInfo->m_threshold);
	}
	UpdateStationFuncInfo();
	UpdateThresholdPanel();

	if(m_bDebugMode)
	{
		RetestCurrentImage();
	}
	return 0;
}

LRESULT  CChannelConfigDlg::OnControlLimitChanged(WPARAM wParam,LPARAM lParam)
{
	m_editThreshold.ShowWindow(SW_HIDE);
	if(m_settingType != T_ALGO || m_crtFuncInfo == nullptr)
		return -1;

	float fCL = (float)lParam; // Control Limit
	int  nFlag = (int)wParam;  // 0: LCL is changed; 1: UCL is changed
	fCL /= 100.0f;

	if(nFlag == 0)
	{
		if (fCL <= m_crtFuncInfo->m_threshold.m_min)
		{
			fCL = m_crtFuncInfo->m_threshold.m_min;
			EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
		}
		else
		{
			EnableUiBtn(IDB_LEFT_ARROW, true,IMG_CH_LEFT_ARROW);	
		}
		m_crtFuncInfo->m_threshold.m_lcl      = fCL;
		m_crtFuncInfoDebug->m_threshold.m_lcl = fCL;

	}
	if(nFlag == 1)
	{
		if (fCL >= m_crtFuncInfo->m_threshold.m_max)
		{
			fCL = m_crtFuncInfo->m_threshold.m_max;
			EnableUiBtn(IDB_RIGHT_ARROW, false, IMG_CH_RIGHT_ARROW_GRAY);
		}
		else
		{
			EnableUiBtn(IDB_RIGHT_ARROW, true, IMG_CH_RIGHT_ARROW);
		}
		m_crtFuncInfo->m_threshold.m_ucl      = fCL;
		m_crtFuncInfoDebug->m_threshold.m_ucl = fCL;
	}

	m_chartCtrl.SetThresholdValue(m_crtFuncInfo->m_threshold);
	m_chartCtrl.Invalidate();

	m_bAlgoChanged = true;
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(pCrtTask != nullptr)
	{
		pCrtTask->UpdateThresholdValue(m_crtFuncInfo->m_funcIndex, m_crtFuncInfo->m_threshold);
	}

	UpdateStationFuncInfo();
	UpdateThresholdPanel();

	if(m_bLocked)
	{
		RetestCurrentImage();
	}
	return 0;
}

void  CChannelConfigDlg::UpdateThresholdStrideLength()
{
	if(m_crtFuncInfo->m_threshold.m_compType > LESS_EQUAL)
	{
		m_fStrideLen = m_crtFuncInfo->m_threshold.m_stepLen;
		return;
	}
	float fStepLen = m_crtFuncInfo->m_threshold.m_stepLen;
	float fCrtThreshold = m_crtFuncInfo->m_threshold.m_specV;
	if(fCrtThreshold >= m_fStatsMin &&  fCrtThreshold <= m_fStatsMax)
	{
		m_fStrideLen = fStepLen;
	}
	if(fCrtThreshold < m_fStatsMin)
	{
		m_fStrideLen = float((m_fStatsMin - fCrtThreshold) / 8.0f);
		if(m_crtFuncInfo->m_threshold.m_type == VT_R4)
		{
			int nTmpLen = int(m_fStrideLen * 100);
			m_fStrideLen = nTmpLen / 100.0f;
			if(m_fStrideLen < m_crtFuncInfo->m_threshold.m_stepLen)
			{
				m_fStrideLen = m_crtFuncInfo->m_threshold.m_stepLen;
			}
		}
		if(m_crtFuncInfo->m_threshold.m_type != VT_R4 && m_fStrideLen < 1.0f)
		{
			m_fStrideLen = 1.0f;
		}
	}
	if(fCrtThreshold > m_fStatsMax)
	{
		m_fStrideLen = float((fCrtThreshold - m_fStatsMax) / 8.0f);
		if(m_crtFuncInfo->m_threshold.m_type == VT_R4)
		{
			int nTmpLen = int(m_fStrideLen * 100);
			m_fStrideLen = nTmpLen / 100.0f;
			if(m_fStrideLen < m_crtFuncInfo->m_threshold.m_stepLen)
			{
				m_fStrideLen = m_crtFuncInfo->m_threshold.m_stepLen;
			}
		}
		if(m_fStrideLen < 1.0f && m_crtFuncInfo->m_threshold.m_type != VT_R4)
		{
			m_fStrideLen = 1.0f;
		}
	}
}

void CChannelConfigDlg::OnDecreaseThresholdClicked()
{

	if(m_settingType != T_ALGO || m_crtFuncInfo == nullptr)
		return;
	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);
	if(bAdmin==FALSE)
	{
		return;
	}
	UpdateThresholdStrideLength();

	m_editThreshold.ShowWindow(SW_HIDE);
	if(!m_bEditUCL)
	{
		if(m_crtFuncInfo->m_threshold.m_compType <= LESS_EQUAL)
		{
			m_crtFuncInfo->m_threshold.m_specV      -= m_fStrideLen;
			m_crtFuncInfoDebug->m_threshold.m_specV -= m_fStrideLen;

			EnableUiBtn(IDB_RIGHT_ARROW, true,IMG_CH_RIGHT_ARROW);
			m_btnIncrease.Invalidate();

			float fMinValue = m_crtFuncInfo->m_threshold.m_min;
			if(m_crtFuncInfo->m_threshold.m_specV <= fMinValue)
			{
				m_crtFuncInfo->m_threshold.m_specV      = fMinValue;
				m_crtFuncInfoDebug->m_threshold.m_specV = fMinValue;
				EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
				m_btnDecrease.Invalidate();
			}
		}
		else
		{
			
			if (m_crtFuncInfo->m_threshold.m_lcl - m_fStrideLen < m_crtFuncInfo->m_threshold.m_min )
			{
				m_crtFuncInfo->m_threshold.m_lcl = 	m_crtFuncInfo->m_threshold.m_min;
				EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
			}
			else
			{
				m_crtFuncInfo->m_threshold.m_lcl -= m_fStrideLen;
				EnableUiBtn(IDB_LEFT_ARROW, true,IMG_CH_LEFT_ARROW);
			}
		}
	}
	else
	{
		if(m_crtFuncInfo->m_threshold.m_ucl - m_fStrideLen < m_crtFuncInfo->m_threshold.m_lcl)
		{
			m_crtFuncInfo->m_threshold.m_ucl  = m_crtFuncInfo->m_threshold.m_lcl;
		}
		else
		{
			m_crtFuncInfo->m_threshold.m_ucl -= m_fStrideLen;
		}
	}
	m_bAlgoChanged = true;
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(pCrtTask != nullptr)
	{
		pCrtTask->UpdateThresholdValue(m_crtFuncInfo->m_funcIndex, m_crtFuncInfo->m_threshold);
	}
	
	m_pFuncInfoArray->assign(m_pDebugFuncInfoArray->begin(), m_pDebugFuncInfoArray->end());
	UpdateStationFuncInfo();
	UpdateThresholdPanel();
	ChangeThresholdValue();

	if(m_bDebugMode)
	{
		RetestCurrentImage();
	}
}

void CChannelConfigDlg::OnIncreaseThresholdClicked()
{
	if(m_settingType != T_ALGO || m_crtFuncInfo == nullptr)
		return;
	UAC crtUser = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (crtUser.m_nLevel == UAC_ADMIN);
	if(bAdmin==FALSE)
	{
		return;
	}
	UpdateThresholdStrideLength();

	//the clabel control has no focus!
	m_editThreshold.ShowWindow(SW_HIDE);
	if(!m_bEditUCL)
	{
		if(m_crtFuncInfo->m_threshold.m_compType <= LESS_EQUAL)
		{
			m_crtFuncInfo->m_threshold.m_specV      += m_fStrideLen;
			m_crtFuncInfoDebug->m_threshold.m_specV += m_fStrideLen;

			EnableUiBtn(IDB_LEFT_ARROW, true,IMG_CH_LEFT_ARROW);
			m_btnDecrease.Invalidate();

			float fMaxValue = m_crtFuncInfo->m_threshold.m_max; 
			if(m_crtFuncInfo->m_threshold.m_specV >= fMaxValue)
			{
				m_crtFuncInfo->m_threshold.m_specV      = fMaxValue;
				m_crtFuncInfoDebug->m_threshold.m_specV = fMaxValue;
				EnableUiBtn(IDB_RIGHT_ARROW, false, IMG_CH_RIGHT_ARROW_GRAY);
				m_btnIncrease.Invalidate();
			}
		}
		else
		{
	
			if (m_crtFuncInfo->m_threshold.m_lcl+ m_fStrideLen > m_crtFuncInfo->m_threshold.m_ucl)
			{
				m_crtFuncInfo->m_threshold.m_lcl = m_crtFuncInfo->m_threshold.m_ucl;
			}
			else
			{
				m_crtFuncInfo->m_threshold.m_lcl += m_fStrideLen;
			}
		}
	}
	else
	{
		
		if (m_crtFuncInfo->m_threshold.m_ucl + m_fStrideLen > m_crtFuncInfo->m_threshold.m_max)
		{
			m_crtFuncInfo->m_threshold.m_ucl = m_crtFuncInfo->m_threshold.m_max;
			EnableUiBtn(IDB_RIGHT_ARROW, false,IMG_CH_RIGHT_ARROW_GRAY);
		}
		else
		{
			m_crtFuncInfo->m_threshold.m_ucl += m_fStrideLen;
			EnableUiBtn(IDB_RIGHT_ARROW, true,IMG_CH_RIGHT_ARROW);
		}
	}
	m_bAlgoChanged = true;
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(pCrtTask != nullptr)
	{
		pCrtTask->UpdateThresholdValue(m_crtFuncInfo->m_funcIndex, m_crtFuncInfo->m_threshold);
	}

//	 *m_pFuncInfoArray = m_vFuncInfo;
	m_pFuncInfoArray->assign(m_pDebugFuncInfoArray->begin(), m_pDebugFuncInfoArray->end());
	 UpdateStationFuncInfo();

	UpdateThresholdPanel();
	ChangeThresholdValue();

	if(m_bDebugMode)
	{
		RetestCurrentImage();
	}

}

void CChannelConfigDlg::OnEnKillfocusEditThreshold()
{
	if(m_crtFuncInfo == nullptr || !m_bThresholdChanged)
	{
		return;
	}
	m_bThresholdChanged = false;
	m_editThreshold.ShowWindow(SW_HIDE);

	CString sNewValue;
	m_editThreshold.GetWindowText(sNewValue);
	if(!utils::IsNumericString(sNewValue))
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
		DisplayPromptMessage(sMsg,false);
		if(!m_bEditUCL)
		{
			m_lblThreshold_Lcl.ShowWindow(SW_SHOW);
			m_lblThreshold_Lcl.Invalidate();
		}
		else
		{
			m_lblThreshold_Ucl.ShowWindow(SW_SHOW);
			m_lblThreshold_Ucl.Invalidate();
		}
		return;
	}
	float dNewValue = utils::to_float(sNewValue);
	if(!m_bEditUCL)
	{
		m_lblThreshold_Lcl.ShowWindow(SW_SHOW);
		if(m_crtFuncInfo->m_threshold.m_compType <= LESS_EQUAL)
		{
			m_crtFuncInfo->m_threshold.m_specV      = dNewValue;
			m_crtFuncInfoDebug->m_threshold.m_specV = dNewValue;
			m_fStrideLen = m_crtFuncInfo->m_threshold.m_stepLen;

			EnableUiBtn(IDB_LEFT_ARROW, true,IMG_CH_LEFT_ARROW);
			EnableUiBtn(IDB_RIGHT_ARROW, true,IMG_CH_RIGHT_ARROW);

			float fMinValue = m_crtFuncInfo->m_threshold.m_min;
			float fMaxValue = m_crtFuncInfo->m_threshold.m_max;
			if(m_crtFuncInfo->m_threshold.m_specV <= fMinValue)
			{
				m_crtFuncInfo->m_threshold.m_specV      = fMinValue;
				m_crtFuncInfoDebug->m_threshold.m_specV = fMinValue;
				EnableUiBtn(IDB_LEFT_ARROW, false,IMG_CH_LEFT_ARROW_GRAY);
			}
			if(m_crtFuncInfo->m_threshold.m_specV >= fMaxValue)
			{
				m_crtFuncInfo->m_threshold.m_specV      = fMaxValue;
				m_crtFuncInfoDebug->m_threshold.m_specV = fMaxValue;
				EnableUiBtn(IDB_RIGHT_ARROW, false,IMG_CH_RIGHT_ARROW_GRAY);
			}
		}
		else
		{
			m_crtFuncInfo->m_threshold.m_lcl      = dNewValue;
			m_crtFuncInfoDebug->m_threshold.m_lcl = dNewValue;
		}
	}
	else //change UCL 
	{
		m_lblThreshold_Ucl.ShowWindow(SW_SHOW);
		m_crtFuncInfo->m_threshold.m_ucl      = dNewValue;
		m_crtFuncInfoDebug->m_threshold.m_ucl = dNewValue;
	}
	m_bAlgoChanged = true;
	TASK* pCrtTask = m_chConfig.GetCurrentTask();
	if(pCrtTask != nullptr)
	{
		pCrtTask->UpdateThresholdValue(m_crtFuncInfo->m_funcIndex, m_crtFuncInfo->m_threshold);
	}

	m_pFuncInfoArray->assign(m_pDebugFuncInfoArray->begin(), m_pDebugFuncInfoArray->end());
	UpdateStationFuncInfo();

	UpdateThresholdPanel();
	ChangeThresholdValue();

	if(m_bDebugMode)
	{
		RetestCurrentImage();
	}

	m_bEditUCL = false;
	m_bThresholdChanged = false;
}

void CChannelConfigDlg::OnStnClickedLCL()
{
	if (m_crtFuncInfo == nullptr)
	{
		return;
	}

	if(m_crtFuncInfo->m_threshold.m_compType > LESS_EQUAL)
	{
		if(m_bEditUCL)
		{

			m_editThreshold.ShowWindow(SW_HIDE);
			m_lblThreshold_Ucl.ShowWindow(SW_SHOW);
		}
	}
	m_bEditUCL = false;
}

void CChannelConfigDlg::OnStnDblclickStaticThresholdPanel_LCL()
{
	m_bEditUCL = false;

	if(m_nAlgoListIndex > 0 && m_crtFuncInfo != nullptr)
	{
		if(!m_crtFuncInfo->m_bTestFunc)
		return;

		m_lblThreshold_Lcl.ShowWindow(SW_HIDE);
		
		CString strValue;
		VARENUM type = m_crtFuncInfo->m_threshold.m_type;
		float fThreshold = 0.0f;
		if(m_crtFuncInfo->m_threshold.m_compType <= LESS_EQUAL)
		{
			fThreshold = m_crtFuncInfo->m_threshold.m_specV;
		}
		else
		{
			fThreshold = m_crtFuncInfo->m_threshold.m_lcl;
			m_lblThreshold_Ucl.ShowWindow(SW_SHOW);
		}
		if(type == VT_R4)
			strValue.Format(_T("%.3f"),fThreshold);
		else
			strValue.Format(_T("%d"), (int)fThreshold);
		CRect rc;
		m_lblThreshold_Lcl.GetWindowRect(rc);
		m_editThreshold.MoveWindow(rc);
		m_editThreshold.SetWindowText(strValue);
		m_editThreshold.ShowWindow(SW_SHOW);

		m_bThresholdChanged = true;
	}
}

void CChannelConfigDlg::OnStnClickedUCL()
{
	if (m_crtFuncInfo == nullptr)
	{
		return;
	}

	if(m_crtFuncInfo->m_threshold.m_compType > LESS_EQUAL)
	{
		if(!m_bEditUCL)
		{
			m_editThreshold.ShowWindow(SW_HIDE);
			m_lblThreshold_Lcl.ShowWindow(SW_SHOW);
		}
	}
	m_bEditUCL = true;
}

void CChannelConfigDlg::OnStnDblclickStaticThresholdPanel_UCL()
{
	m_bEditUCL = true;

	if(m_nAlgoListIndex > 0 && m_crtFuncInfo != nullptr)
	{
		m_lblThreshold_Ucl.ShowWindow(SW_HIDE);

		CString strValue;
		VARENUM type = m_crtFuncInfo->m_threshold.m_type;
		float fThreshold = 0.0f;
		if(m_crtFuncInfo->m_threshold.m_compType <= LESS_EQUAL)
		{
			fThreshold = m_crtFuncInfo->m_threshold.m_specV;
		}
		else
		{
			fThreshold = m_crtFuncInfo->m_threshold.m_ucl;
			m_lblThreshold_Lcl.ShowWindow(SW_SHOW);
		}
		if(type == VT_R4)
			strValue.Format(_T("%.3f"),fThreshold);
		else
			strValue.Format(_T("%d"), (int)fThreshold);
		CRect rc;
		m_lblThreshold_Ucl.GetWindowRect(rc);
		m_editThreshold.MoveWindow(rc);
		m_editThreshold.SetWindowText(strValue);
		m_editThreshold.ShowWindow(SW_SHOW);

		m_bThresholdChanged = true;
	}
}

void CChannelConfigDlg::InitLightSingleModeCtrlPos()
{
	//single mode control pos
	m_vLcWidgetPtr.clear();
	m_vLcTagPtr.clear();
	m_vLcSliderPtr.clear();
	m_vLcStaticPtr.clear();
	m_dictSlider_Value.clear();

	CRect rc;
	m_paramBoard.GetWindowRect(rc);

	int i = 0;
	CRect rcName, rcEdit;

	CreateUiRect(rc,i++,rcName,rcEdit);
	InitUiWidget(IDC_STATIC_LC_PORT, IDS_STRING_SERIAL_PORT);
	InitUiWidget(IDC_EDIT_LC_PORT);
	m_staticLcPort.MoveWindow(rcName);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLcPort);
	m_editLcPort.MoveWindow(rcEdit);
	m_vLcSliderPtr.push_back((CWnd*)&m_editLcPort);

	CreateUiRect(rc,i++,rcName,rcEdit);
	InitUiWidget(IDC_STATIC_LC_OPTION,IDS_STRING_LC_OPTION);
	InitUiWidget(IDC_COMBO_LC_OPTION);
	m_staticIDs.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticIDs);
	m_comboIDs.MoveWindow(rcEdit);   m_vLcWidgetPtr.push_back((CWnd*)&m_comboIDs);

	CreateUiRect(rc,i++,rcName,rcEdit);
	InitUiWidget(IDC_STATIC_LC_MODE,IDS_STRING_LIGHT_CONTROLLER_MODE);
	InitUiWidget(IDC_COMBO_LC_MODE);
	m_staticMode.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticMode);
	m_comboMode.MoveWindow(rcEdit);   m_vLcWidgetPtr.push_back((CWnd*)&m_comboMode);

 	CRect rcValue;

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcName.top+=10;
	m_bCheckSingleMode.MoveWindow(rcName);

	//m_vLcWidgetPtr.push_back((CWnd*)&m_bCheckSingleMode);

	int nDelta = 7;
	
	if (m_bSingleModeCH1)
	{
	// Line #1
	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcEdit.left  += nDelta;
	rcEdit.right -= nDelta;
	InitUiWidget(IDC_STATIC_TAG1,_T("#LINE1"));
	InitUiWidget(IDC_EDIT_TAG1); 
	m_staticTag1.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticTag1);
	m_editTag1.MoveWindow(rcEdit);    m_vLcTagPtr.push_back((CWnd*)&m_editTag1);

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_LUM1,IDS_STRING_LUMINANCE);
	InitUiWidget(IDC_SLIDER_LUM1);
	InitUiWidget(IDC_STATIC_VALUE_LUM1); 
	m_staticLum1.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLum1);
	m_staticLum1.BringWindowToTop();
	m_sliderLum1.MoveWindow(rcEdit); 
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderLum1);
	m_sliderLum1.SetRange(0,255); m_sliderLum2.SetPos(0);
	m_staticValueLum1.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueLum1);
	m_dictSlider_Value[(CWnd*)&m_sliderLum1] = &m_staticValueLum1;


	/////////////////////////
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DELAY1,IDS_STRING_LIGHT_CONTROLLER_DELAY);
	InitUiWidget(IDC_SLIDER_DELAY);
	InitUiWidget(IDC_STATIC_VALUE_DELAY); 
	m_staticDelay.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDelay);
	m_sliderDelay.MoveWindow(rcEdit); 
	m_sliderDelay.SetRange(0,2000);
	m_sliderDelay.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDelay);
	m_staticValueDelay.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDelay);
	m_dictSlider_Value[(CWnd*)&m_sliderDelay] = &m_staticValueDelay;

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DURATION,IDS_STRING_LIGHT_CONTROLLER_DURATION);
	InitUiWidget(IDC_SLIDER_DURATION);
	InitUiWidget(IDC_STATIC_VALUE_DURATION); 
	m_staticDur.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDur);
	m_sliderDur.MoveWindow(rcEdit);
	m_sliderDur.SetRange(1,200);
	m_sliderDur.SetPos(1);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDur);
	m_staticValueDur.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDur);
	m_dictSlider_Value[(CWnd*)&m_sliderDur] = &m_staticValueDur;

	}
	else
	{
		SetCH1ShowWindow(false);
	}
	////////////////////////////
	// Line #2
	if (m_bSingleModeCH2)
	{
	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcEdit.left  += nDelta;
	rcEdit.right -= nDelta;
	InitUiWidget(IDC_STATIC_TAG2,_T("#LINE2"));
	InitUiWidget(IDC_EDIT_TAG2); 
	m_staticTag2.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticTag2);
	m_editTag2.MoveWindow(rcEdit);    m_vLcTagPtr.push_back((CWnd*)&m_editTag2);

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_LUM2,IDS_STRING_LUMINANCE);
	InitUiWidget(IDC_SLIDER_LUM2);
	InitUiWidget(IDC_STATIC_VALUE_LUM2); 
	m_staticLum2.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLum2);
	m_sliderLum2.MoveWindow(rcEdit); 
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderLum2);
	m_sliderLum2.SetRange(0,255);
	m_sliderLum2.SetPos(0);
	m_staticValueLum2.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueLum2);
	m_dictSlider_Value[(CWnd*)&m_sliderLum2] = &m_staticValueLum2;

	//line2 delay and time 
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DELAY2,IDS_STRING_LIGHT_CONTROLLER_DELAY);
	InitUiWidget(IDC_SLIDER_DELAY2);
	InitUiWidget(IDC_STATIC_VALUE_DELAY2); 
	m_staticDelay2.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDelay2);
	m_sliderDelay2.MoveWindow(rcEdit); 
	m_sliderDelay2.SetRange(0,2000);
	m_sliderDelay2.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDelay2);
	m_staticValueDelay2.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDelay2);
	m_dictSlider_Value[(CWnd*)&m_sliderDelay2] = &m_staticValueDelay2;

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DURATION2,IDS_STRING_LIGHT_CONTROLLER_DURATION);
	InitUiWidget(IDC_SLIDER_DURATION2);
	InitUiWidget(IDC_STATIC_VALUE_DURATION2); 
	m_staticDur2.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDur2);
	m_sliderDur2.MoveWindow(rcEdit);
	m_sliderDur2.SetRange(1,200);
	m_sliderDur2.SetPos(1);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDur2);
	m_staticValueDur2.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDur2);
	m_dictSlider_Value[(CWnd*)&m_sliderDur2] = &m_staticValueDur2;

	}
	else
	{
		SetCH2ShowWindow(false);
	}
	// Line #3
	if (m_bSingleModeCH3)
	{

	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcEdit.left  += nDelta;
	rcEdit.right -= nDelta;
	InitUiWidget(IDC_STATIC_TAG3,_T("#LINE3"));
	InitUiWidget(IDC_EDIT_TAG3); 
	m_staticTag3.MoveWindow(rcName);  m_vLcStaticPtr.push_back((CWnd*)&m_staticTag3);
	m_editTag3.MoveWindow(rcEdit);    m_vLcTagPtr.push_back((CWnd*)&m_editTag3);

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_LUM3,IDS_STRING_LUMINANCE);
	InitUiWidget(IDC_SLIDER_LUM3);
	InitUiWidget(IDC_STATIC_VALUE_LUM3); 
	m_staticLum3.MoveWindow(rcName);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLum3);
	m_sliderLum3.MoveWindow(rcEdit);
	m_sliderLum3.SetRange(0,255);
	m_sliderLum3.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderLum3);
	m_staticValueLum3.MoveWindow(rcValue); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueLum3);
	m_dictSlider_Value[(CWnd*)&m_sliderLum3] = &m_staticValueLum3;

	//line #3 light and mode
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DELAY3,IDS_STRING_LIGHT_CONTROLLER_DELAY);
	InitUiWidget(IDC_SLIDER_DELAY3);
	InitUiWidget(IDC_STATIC_VALUE_DELAY3); 
	m_staticDelay3.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDelay3);
	m_sliderDelay3.MoveWindow(rcEdit); 
	m_sliderDelay3.SetRange(0,2000);
	m_sliderDelay3.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDelay3);
	m_staticValueDelay3.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDelay3);
	m_dictSlider_Value[(CWnd*)&m_sliderDelay3] = &m_staticValueDelay3;

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DURATION3,IDS_STRING_LIGHT_CONTROLLER_DURATION);
	InitUiWidget(IDC_SLIDER_DURATION3);
	InitUiWidget(IDC_STATIC_VALUE_DURATION3); 
	m_staticDur3.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDur3);
	m_sliderDur3.MoveWindow(rcEdit);
	m_sliderDur3.SetRange(1,200);
	m_sliderDur3.SetPos(1);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDur3);
	m_staticValueDur3.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDur3);
	m_dictSlider_Value[(CWnd*)&m_sliderDur3] = &m_staticValueDur3;

	///////////////////////////
	}
	else
	{
		SetCH3ShowWindow(false);
	}
	// Line #4
	if (m_bSingleModeCH4)
	{

	i++;
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcEdit.left  += nDelta;
	rcEdit.right -= nDelta;
	InitUiWidget(IDC_STATIC_TAG4,_T("#LINE4"));
	InitUiWidget(IDC_EDIT_TAG4); 
	m_staticTag4.MoveWindow(rcName);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticTag4);
	m_editTag4.MoveWindow(rcEdit);
	m_vLcTagPtr.push_back((CWnd*)&m_editTag4);

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_LUM4,IDS_STRING_LUMINANCE);
	InitUiWidget(IDC_SLIDER_LUM4);
	InitUiWidget(IDC_STATIC_VALUE_LUM4); 
	m_staticLum4.MoveWindow(rcName);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticLum4);
	m_sliderLum4.MoveWindow(rcEdit); 
	m_sliderLum4.SetRange(0,255);
	m_sliderLum4.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderLum4);
	m_staticValueLum4.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueLum4);
	m_dictSlider_Value[(CWnd*)&m_sliderLum4] = &m_staticValueLum4;

	/////////line #4 delay and time
	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DELAY4,IDS_STRING_LIGHT_CONTROLLER_DELAY);
	InitUiWidget(IDC_SLIDER_DELAY4);
	InitUiWidget(IDC_STATIC_VALUE_DELAY4); 
	m_staticDelay4.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDelay4);
	m_sliderDelay4.MoveWindow(rcEdit); 
	m_sliderDelay4.SetRange(0,2000);
	m_sliderDelay4.SetPos(0);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDelay4);
	m_staticValueDelay4.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDelay4);
	m_dictSlider_Value[(CWnd*)&m_sliderDelay4] = &m_staticValueDelay4;

	CreateUiRectForLC(rc,i++,rcName,rcEdit);
	rcValue = rcEdit;
	rcValue.left    = rcEdit.right + 5;
	rcValue.top     = rcEdit.top + 4;
	rcValue.right   = rc.right - 8;
	rcValue.bottom  = rcEdit.bottom + 4;
	InitUiWidget(IDC_STATIC_DURATION4,IDS_STRING_LIGHT_CONTROLLER_DURATION);
	InitUiWidget(IDC_SLIDER_DURATION4);
	InitUiWidget(IDC_STATIC_VALUE_DURATION4); 
	m_staticDur4.MoveWindow(rcName); 
	m_vLcStaticPtr.push_back((CWnd*)&m_staticDur4);
	m_sliderDur4.MoveWindow(rcEdit);
	m_sliderDur4.SetRange(1,200);
	m_sliderDur4.SetPos(1);
	m_vLcSliderPtr.push_back((CWnd*)&m_sliderDur4);
	m_staticValueDur4.MoveWindow(rcValue);
	m_vLcStaticPtr.push_back((CWnd*)&m_staticValueDur4);
	m_dictSlider_Value[(CWnd*)&m_sliderDur4] = &m_staticValueDur4;
	}
	else
	{
		SetCH4ShowWindow(false);
	}
}

void CChannelConfigDlg::OnBnClickedCheckLightSingleMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = m_bCheckSingleMode.GetCheck();
	if (i == BST_UNCHECKED)
	{
		m_bOneTouchFour = false;
		m_bSingleMode = true;
	}
	else
	{
		m_bOneTouchFour  = true;
		m_bSingleMode = true;
	}

}

void CChannelConfigDlg::SetCH1ShowWindow(int nCmdShow)
{
	m_staticTag1.ShowWindow(nCmdShow);
	m_editTag1.ShowWindow(nCmdShow);
	m_staticLum1.ShowWindow(nCmdShow);
	m_sliderLum1.ShowWindow(nCmdShow);
	m_staticValueLum1.ShowWindow(nCmdShow);

	m_staticDelay.ShowWindow(nCmdShow);
	m_sliderDelay.ShowWindow(nCmdShow);
	m_staticValueDelay.ShowWindow(nCmdShow);
	m_staticDur.ShowWindow(nCmdShow);
	m_sliderDur.ShowWindow(nCmdShow);
	m_staticValueDur.ShowWindow(nCmdShow);
}

void CChannelConfigDlg::SetCH2ShowWindow(int nCmdShow)
{
	m_staticTag2.ShowWindow(nCmdShow);
	m_editTag2.ShowWindow(nCmdShow);
	m_staticLum2.ShowWindow(nCmdShow);
	m_sliderLum2.ShowWindow(nCmdShow);
	m_staticValueLum2.ShowWindow(nCmdShow);

	m_staticDelay2.ShowWindow(nCmdShow);
	m_sliderDelay2.ShowWindow(nCmdShow);
	m_staticValueDelay2.ShowWindow(nCmdShow);
	m_staticDur2.ShowWindow(nCmdShow);
	m_sliderDur2.ShowWindow(nCmdShow);
	m_staticValueDur2.ShowWindow(nCmdShow);
}

void CChannelConfigDlg::SetCH3ShowWindow(int nCmdShow)
{
	m_staticTag3.ShowWindow(nCmdShow);
	m_editTag3.ShowWindow(nCmdShow);
	m_staticLum3.ShowWindow(nCmdShow);
	m_sliderLum3.ShowWindow(nCmdShow);
	m_staticValueLum3.ShowWindow(nCmdShow);

	m_staticDelay3.ShowWindow(nCmdShow);
	m_sliderDelay3.ShowWindow(nCmdShow);
	m_staticValueDelay3.ShowWindow(nCmdShow);
	m_staticDur3.ShowWindow(nCmdShow);
	m_sliderDur3.ShowWindow(nCmdShow);
	m_staticValueDur3.ShowWindow(nCmdShow);
}

void CChannelConfigDlg::SetCH4ShowWindow(int nCmdShow)
{
	m_staticTag4.ShowWindow(nCmdShow);
	m_editTag4.ShowWindow(nCmdShow);
	m_staticLum4.ShowWindow(nCmdShow);
	m_sliderLum4.ShowWindow(nCmdShow);
	m_staticValueLum4.ShowWindow(nCmdShow);

	m_staticDelay4.ShowWindow(nCmdShow);
	m_sliderDelay4.ShowWindow(nCmdShow);
	m_staticValueDelay4.ShowWindow(nCmdShow);
	m_staticDur4.ShowWindow(nCmdShow);
	m_sliderDur4.ShowWindow(nCmdShow);
	m_staticValueDur4.ShowWindow(nCmdShow);
}

void CChannelConfigDlg::UpdateSingleMode()
{
	LIGHT_SETTING& ls = m_chConfig.m_lightCtrlSetting;
	int nIndex = m_comboIDs.GetCurSel();
	if(nIndex >= 0)
	{
		CString sValue;
		m_comboIDs.GetLBText(nIndex, sValue);
		m_nCrtLcID     = (int)utils::to_float(sValue);
		wstring wstrTags(L"");
		if(nIndex < (int)ls.m_vTags.size())
		{
			wstrTags = ls.m_vTags[nIndex];
		}

		m_bSingleModeCH1 = false;
		m_bSingleModeCH2 = false;
		m_bSingleModeCH3 = false;
		m_bSingleModeCH4 = false;
		if(wstrTags != L"")
		{
			std::vector<wstring> vTags = utils::SplitString(wstrTags, L"$",token_compress_off,false);

			m_nActiveID = 0;

				if (vTags[0]!=L"")
				{
					m_bSingleModeCH1 = true;
					m_nActiveID += 1<<3;
				}
				if (vTags[1]!=L"")
				{
					m_bSingleModeCH2 = true;
					m_nActiveID += 1<<2;
				}
				if (vTags[2]!=L"")
				{
					m_bSingleModeCH3 = true;
					m_nActiveID += 1<<1;
				}
				if (vTags[3]!=L"")
				{
					m_bSingleModeCH4 = true;
					m_nActiveID += 1<<0;
				}

			if (m_nActiveID == 15)
			{
				m_bCheckSingleMode.ShowWindow(SW_SHOW);

				m_comboMode.ResetContent();

				CString sMode;
				sMode.LoadString(IDS_STRING_LC_MODE_ALWAYS_ON);
				m_comboMode.AddString(sMode);

				sMode.LoadString(IDS_STRING_LC_MODE_TRIGGER);
				m_comboMode.AddString(sMode);

				sMode.LoadString(IDS_STRING_LC_MODE_TRIGGER_TWICE);
				m_comboMode.AddString(sMode);
			}
			else
			{
				m_bCheckSingleMode.ShowWindow(SW_HIDE);

				m_comboMode.ResetContent();

				CString sMode;
				sMode.LoadString(IDS_STRING_LC_MODE_ALWAYS_ON);
				m_comboMode.AddString(sMode);

				sMode.LoadString(IDS_STRING_LC_MODE_TRIGGER);
				m_comboMode.AddString(sMode);
			}
			m_bCheckSingleMode.SetCheck(BST_UNCHECKED);
			m_bOneTouchFour = false;
			}	
		}
	
}

void CChannelConfigDlg::SetSingleModeControlShow(int nCmdShow)
{
	m_staticDelay4.ShowWindow(nCmdShow);
	m_sliderDelay4.ShowWindow(nCmdShow);
	m_staticValueDelay4.ShowWindow(nCmdShow);
	m_staticDur4.ShowWindow(nCmdShow);
	m_sliderDur4.ShowWindow(nCmdShow);
	m_staticValueDur4.ShowWindow(nCmdShow);

	//delay and lighttime //line3
	m_staticDelay2.ShowWindow(nCmdShow);
	m_sliderDelay2.ShowWindow(nCmdShow);
	m_staticValueDelay2.ShowWindow(nCmdShow);
	m_staticDur2.ShowWindow(nCmdShow);
	m_sliderDur2.ShowWindow(nCmdShow);
	m_staticValueDur2.ShowWindow(nCmdShow);

	//delay and lighttime //line4
	m_staticDelay3.ShowWindow(nCmdShow);
	m_sliderDelay3.ShowWindow(nCmdShow);
	m_staticValueDelay3.ShowWindow(nCmdShow);
	m_staticDur3.ShowWindow(nCmdShow);
	m_sliderDur3.ShowWindow(nCmdShow);
	m_staticValueDur3.ShowWindow(nCmdShow);
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderDelay2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSingleMode)
	{
		ApplyNewLcParameter(2);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderDuration2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(2);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderDelay3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(3);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderDuration3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(3);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderDelay4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(4);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnNMReleasedcaptureSliderDuration4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSingleMode)
	{
		ApplyNewLcParameter(4);
	}
	else
	{
		ApplyNewLcParameter();
	}
	*pResult = 0;
}

void CChannelConfigDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_VecpProcessStation.size() == 1)
	{
		return;
	}

 	if(m_bZoomIn || !m_rcImgWnd.PtInRect(point))
 	{
		return;
 	}
	ZoomSubwindow();
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CChannelConfigDlg::OnBnClickedCheckTaskParam()
{
	TASK* pCrtTask = m_chConfig.GetCurrentTask();

	int i = m_bCheckTaskAttach.GetCheck();
	if (i == BST_UNCHECKED)
	{
		//解除绑定
		pCrtTask->m_vecTaskCameraSetting.clear();
	}
	else
	{
		//任务从非绑定到绑定
		pCrtTask->m_vecTaskCameraSetting.assign(m_chConfig.m_vecCameraSetting.begin(),m_chConfig.m_vecCameraSetting.end());
	}
	m_btnApplyParam.EnableWindow(true);
	m_bCsChanged = true;
	OnCameraClicked();
}

void CChannelConfigDlg::OnBnClickedRadioCamera1()
{
	m_nCrtImgChannel = 1;
	if (!m_bSelfLearningMode)
	{
		DisplayALLImageOnChWnd();
	}
	else
	{
		wstring wstrParam = GetSelfLearningParam();
		m_pProcessStation->DisplayModelImage(m_crtImgIndex,wstrParam);
	}

}

void CChannelConfigDlg::OnBnClickedRadioCamera2()
{
	m_nCrtImgChannel = 2;
	if (!m_bSelfLearningMode)
	{
		DisplayALLImageOnChWnd();
	}
	else
	{
		wstring wstrParam = GetSelfLearningParam();
		m_pProcessStation->DisplayModelImage(m_crtImgIndex,wstrParam);
	}
}





BOOL CChannelConfigDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCursor(m_hCursor); 
	return TRUE;
//	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CChannelConfigDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_rcImgWnd.PtInRect(pt) && m_crtImage.Id() != H_EMPTY_REGION)
	{
		if(::GetKeyState(VK_CONTROL) < 0)
		{	
			if(zDelta >0)
				DisplayImageWndZoomOut(pt);
			else
				DisplayImageWndZoomIn(pt);
		}
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
