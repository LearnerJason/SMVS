#pragma once

#include "LabelCtrl.h"
#include "afxcmn.h"
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <ppl.h>
#include <concurrent_vector.h>
#include "TypeDef.h"
#include "ChartCtrl.h"
#include "ColoredListCtrl.h"
#include "SliderControl.h"
#include "FileMappingProxy.h"
#include "afxwin.h"
#include <boost\lockfree\queue.hpp>
#include "ScreenDisplayOp.h"
#include "ChannelControlPanel.h"

class CImageProcessor;
using namespace std;
using namespace boost;
using namespace Concurrency;

#pragma region CONST VALUE
const int IDB_ON_OFF      = 77777;
const int IDB_CAMERA	  = 70000;
const int IDB_LIGHT		  = 70001;
const int IDB_ALGO		  = 70002;
const int IDB_IMG_SOURCE  = 70003;
const int IDB_FIRST       = 70004;
const int IDB_PREVIOUS    = 70005;
const int IDB_NEXT        = 70006;
const int IDB_LAST        = 70007;
const int IDB_LOCK        = 70008;
const int IDB_ZOOM_IN     = 70009;
const int IDB_ZOOM_OUT    = 70010;
const int IDB_DISP_MODE   = 70011;
const int IDB_REJECTOR    = 70013;
const int IDB_DEVICE_INFO = 70014;
// For Camera Setting
const int IDC_CAMERA_RUN_MODE		  = 70015; // Combo Box
const int IDC_EDGE_CTRL				  = 70016;
const int IDB_SAVE_IMG				  = 70017;
const int IDB_SAVE_ALL_IMG			  = 70018;
const int IDB_DELETE_CRT_IMG		  = 70019;
const int IDB_IMG_DISP_MODE			  = 70020;
const int IDB_LEFT_ARROW			  = 70021;
const int IDB_THRESHOLD_PANEL_LCL         = 70022;
const int IDB_THRESHOLD_PANEL_UCL     = 70023;
const int IDB_RIGHT_ARROW			  = 70024;
const int IDB_CH_DLG_CLOSE			  = 70025;
const int IDB_ADD_MODEL_IMG			  = 70026;
const int IDB_RESET_STATS_DATA		  = 70027;
const int IDC_EDIT_PARAMETER		  = 70028;
const int IDC_CAMERA_STATION		  = 70029; // Combo Box

const int TILTE_HEIGHT				 = 80;
const int TIMER_ID					 = 90000;
const int TIMER_ID_DLG_READY		 = 90001;
const int TIMER_ID_FOR_EDIT_CTRL     = 90002;
const int TIMER_ID_INCREASE_DECREASE = 90003;
const int TIMER_ID_SWITCH_CH         = 90004;
const int TIMER_ID_DEVICE_DATA       = 90005;

const int IDB_EDIT_FILE0      = 980000;
const int IDB_EDIT_FILE1      = 980001;
const int IDB_EDIT_FILE2      = 980002;
const int IDB_EDIT_FILE3      = 980003;
const int IDB_EDIT_FILE4      = 980004;
const int IDB_EDIT_FILE5      = 980005;
const int IDB_EDIT_FILE6      = 980006;
const int IDB_EDIT_FILE7      = 980007;
const int IDC_IFC             = 980010;

#pragma endregion

class CChannelConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChannelConfigDlg)

public:
	CChannelConfigDlg(CHANNEL_CONFIG& chConfig,vector<CImageProcessor*> pProcessStation, CWnd* pParent = NULL);
	virtual ~CChannelConfigDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	enum { IDD = IDD_DLG_CH_CONFIG }; 
	void SetAlgoListStats(std::vector<FUNC_INFO>& vFuncInfo);
	void SetCheckResult(std::vector<IMG_PRODUCT>& vResultGroup);
	void SetDisplayResult(std::vector<DisplayResult>& ResultDisplay);
	IMG_PRODUCT CChannelConfigDlg::GetSPlicingResultGroup(std::vector<IMG_PRODUCT> vResultGroup);
	bool DisplayImageOnChWnd(Hobject& img,long long chImgWndID);
	bool DealWithTransparentObjectForDisplay(Hobject &object);
	void DisplayTimeStatisticOnChWnd(Hobject& img,DisplayResult& vDisplayResult,long long chImgWndID);
    void DisplayGlueInjectorIndexChWnd(Hobject& img,int nGii,long long chImgWndID);
	void DisplayErrorRegionOnChWnd(std::map<Hlong,Hobject>& unionErrRgn,long long chImgWndID);
	void DisplayInnerStatisticsOnChWnd(Hobject& img,std::vector<float>	m_vInnerStatsData,long long chImgWndID);
	void DisplayErrImgTargetRegionOnChWnd(Hobject& unionErrRgn,long long chImgWndID,int iRegionColor=0) ;
	bool DisplayALLImageOnChWnd();
	void RedrawAlgoList();
	void SetCurCameraIndex(int index);
	void SetChannelControlPanelPtr(CChannelControlPanel* ptr);
 public:
	afx_msg void OnCloseScreen();
	afx_msg void OnCameraClicked();
	afx_msg void OnLightClicked();
	afx_msg void OnRejectorClicked();
	afx_msg void OnDeviceInfoClicked();
	afx_msg void OnDecreaseThresholdClicked();
	afx_msg void OnIncreaseThresholdClicked();
	afx_msg void OnImgSourceClicked();
	afx_msg void OnFirstClicked();
	afx_msg void OnPreviousClicked();
	afx_msg void OnNextClicked();
	afx_msg void OnLastClicked();
	afx_msg void OnImgDispModeClicked();
	afx_msg void OnLockClicked();
	afx_msg void OnZoomInClicked();
	afx_msg void OnSaveImgClicked();
	afx_msg void OnSaveAllImgClicked();
	afx_msg void OnDeleteModelImgClicked();
	afx_msg void OnAddModelImgClicked();
	afx_msg void OnResetStatsDataClicked();
	afx_msg void OnOnOffButtonClicked();
	afx_msg void OnNMClickListAlgo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeRunMode();
	afx_msg void OnCbnSelchangeCameraStation();
	afx_msg void OnCbnSelchangeEdgeCtrl();
	afx_msg void OnBnClickedBtnApplyParam();
	afx_msg void OnBnClickedRadioCh1();
	afx_msg void OnBnClickedRadioCh2();
	afx_msg void OnBnClickedRadioCh3();
	afx_msg void OnBnClickedRadioCh4();
	afx_msg void OnBnClickedRadioCh5();
	afx_msg void OnBnClickedRadioCh6();
	afx_msg void OnBnClickedRadioCh7();
	afx_msg void OnBnClickedRadioCh8();
	afx_msg void OnBnClickedBtnSaveTrainningResult();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckReadImgFile();
	afx_msg void OnBnClickedCheckDispErrorRgn();
	afx_msg void OnEnSetfocusEditParameter();
	afx_msg void OnStnClickedStaticBtnDecrease();
	afx_msg void OnStnClickedStaticBtnIncrease();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStnClickedStaticEditFile0();
	afx_msg void OnStnClickedStaticEditFile1();
	afx_msg void OnStnClickedStaticEditFile2();
	afx_msg void OnStnClickedStaticEditFile3();
	afx_msg void OnStnClickedStaticEditFile4();
	afx_msg void OnStnClickedStaticEditFile5();
	afx_msg void OnStnClickedStaticEditFile6();
	afx_msg void OnStnClickedStaticEditFile7();
	afx_msg void OnCbnSelchangeComboLcOption();
	afx_msg void OnCbnSelchangeComboLcMode();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMReleasedcaptureSliderDelay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderDuration(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderLum1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderLum2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderLum3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderLum4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMClickSyslinkAlgoHelp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddRegion();
	afx_msg void OnDelRegion();
	afx_msg void OnAddTestItem();
	afx_msg void OnDelTestItem();
	afx_msg void OnRenameTestItem();
	afx_msg void OnNMRClickListAlgo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditThreshold();
	afx_msg void OnStnClickedLCL();
	afx_msg void OnStnDblclickStaticThresholdPanel_LCL();
	afx_msg void OnStnClickedUCL();
	afx_msg void OnStnDblclickStaticThresholdPanel_UCL();

	LRESULT  OnRetrieveModelImage(WPARAM wParam,LPARAM lParam);
	LRESULT  OnUpdateChannelScreen(WPARAM wParam,LPARAM lParam);
	LRESULT  OnThresholdChanged(WPARAM wParam,LPARAM lParam);
	LRESULT  OnControlLimitChanged(WPARAM wParam,LPARAM lParam);
	LRESULT  OnExternalFileUpdated(WPARAM wParam,LPARAM lParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()

private:
	void InitUiCtrl();
	void CreateChannelImageWindow(bool m_bSplicing = false);
	CScreenDisplayOp m_ScreenOp;
	CRect		m_OldrcImgWnd;
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void InitUiWidget(int nCtrlID, CString str);
	void DisplayFirstModelImage();
	vector<int> m_vecModelIndex;
	vector<int> m_vecModelCount;

	void DisplayImageWndZoomOut(CPoint point);
	void DisplayImageWndZoomIn(CPoint point);
	void InitPromptMessagePanel(CRect& rcWorkArea);
	void InitScreenHeadPanel(CRect& rcWorkArea);
	void InitButtons(CRect& rcWorkArea, CRect& rcClient);
	void InitLightControllerWidget();
	void FillLightControllerWidget();
	void AddRoiFunc2List();
	void AddTestFunc2List(int nRoiFunIndex, int& nListIndex);
	void InitAlgoListBoard(CRect& rcWorkArea, CRect& rcClient);
	void InitParameterBoard(CRect& rcWorkArea, CRect& rcClient);
	void InitApplyParamButton(CRect& rcWorkArea);
	void InitSaveTrainningResultButton(CRect& rcWorkArea);
	void SetButtonVisibility();
	void InitSpcChart(CRect& rcWorkArea);
	void InitChannelSwitch(CRect& rcWorkArea);
	void InitCameraSwitch(CRect& rcWorkArea);
	void CreateAlgoParamItems();
	void CreateSlAlgoParamItems();
	void CreateCameraParamItems();
	void DeleteAllSettingItem();
	bool CheckImageIndex(int index);
	bool SetSelfProcessIndex(int index);
	void UpdateCurrentImgIndex(int index);
	void ProcessCurrentImage();
	void LockCurrentImage();
	void SortImageInChronologicalOrderNew(std::vector<std::vector<IMG_PRODUCT>>& NgImgContainer,int& nCrtIndex,long m_errorindex);
	void SwitchChannel(int chIndex,int StationIndex = 0);
	void ResizeOtherControl();
	void UpdateStatisticsChart();
	void ChangeThresholdValue();
	bool ValidateAlgoParameter(CWnd* pEdit, CString sValue, const INPUT_PARAM& inputParam);
	bool GetNewAlgoParameters(vector<INPUT_PARAM>& vNewTestParam,vector<INPUT_PARAM>& vConfigParam);
	void RetestCurrentImage();
	void UpdateThresholdPanel();
	bool GetNewRejectorSetting();
	bool GetNewCameraSetting();
	bool GetNewUiSetting(UI_SETTING& newUS);
	void EnableDisplayModeButton(bool bEnable);
	void SetSelfLearningFlag();
	CString SetTargetImageFilePath(bool bSingleImg);
	void DisplayPromptMessage(CString strMsg, bool bStatus);
	void EnableUiBtn(int nCtlID, bool bEnable, eIMAGE_TYPE nImgType);
	void EnableAllButtons(bool bEnable);
	void EnableUiWidget(vector<CWnd*> vCtrl, bool bEnable);
	void SetImageIndexPanelText(int index, int total);
	float GetThresholdStrideLength();
	void UpdateThresholdStrideLength();
	bool ParameterChanged(DISPID& funcId, int funcIndex, const std::vector<INPUT_PARAM>& vCrtParam);
	// For hiding ROI extracting algorithm items
	int m_crtFuncIndex;
	int GetTargetFuncIndex(int nCrtFuncIndex, eTASK_OPERATION to);
	void RefreshUiCtrl(CWnd* pCtrl);
	void CreateUiRect(const CRect& rc, int index, CRect& rcName, CRect& rcEdit);
	void EnableFunc(int nFuncIndex, bool bEnable, bool bTestFunc);
	void CreateUiRectForLC(const CRect& rc, int index, CRect& rcName, CRect& rcEdit);
	void AdjustAlgorithmParam(bool bIncreased = true);
	void SavaChannelSettings2ConfigFile();
	void SortImageInChronologicalOrder(vector<Hobject>& vImages, vector<Hobject>& vUnionErrRgn, std::vector<long long>& vTimestamp, int& nCrtIndex);
	long GetFileEditBtnID(int nIndex);
	void SetFileButtonImage();
	void UpdatedictBtnID_File(int i);
	void ModifyFile(int nBtnID);
	void UpdateFuncInputFile(FUNC_INFO* pFi, int nFileIndex, wstring wstrNewFile);
	wstring GetSelfLearningParam();
	void ShowLightControllerWidget(BOOL bVisible);
	void DisplayLightControllerParameter(const LIGHT_SETTING& ls, wstring wstrLcGpName, int nControllerID, wstring wstrTags, bool bChange = false);
	void ResetLcSliderWidget();
	void RefreshLcSliderWidget();
	void CollectLcChannelData(LC& lc);
	LC GetLightControllerParameter();
	void WriteNewLcParameter2Device();
	void ApplyNewLcParameter(int CHID = 0);
	void ApplyNewAlgoParam();
	void RestoreOldParam();
	void UpdateSliderPostionPanel(CSliderControl* pSlider);
	int GetDefaultNewRegionID();
	bool IsAnySingledRoiFunc(wstring& wtrRgnName);
	bool AddNewFunc(const FUNC_INFO& newFi);
	bool DeleteFunc(int crtFuncIndex,eTASK_OPERATION to);
	bool SingledRoiAlgo(FUNC_INFO& funcInfo);
	void ProcessPendingParameter();
	void UpdateDeviceData();
	void UpdateStationFuncInfo();
	void UpdatevNewParamPath(std::vector<INPUT_PARAM> &vNewParam,int index);
    void ZoomSubwindow();
private:

	bool m_bExitFlag;
	CFileMappingProxy<char, 128> m_transferStation;
	CLabelCtrl m_dlgTitle;
	CLabelCtrl m_btnClose;
	CLabelCtrl m_btnOnOff;
	// Vertical Option Panel
	CLabelCtrl m_btnCamera;
	CLabelCtrl m_btnLight;
	CLabelCtrl m_btnRejector;
	CLabelCtrl m_btnAlgo;
	CLabelCtrl m_btnDeviceInfo;
	CLabelCtrl m_btnUiConfig;
	CLabelCtrl m_btnDecrease;
	CLabelCtrl m_lblThreshold_Lcl;
	CLabelCtrl m_lblThreshold_Ucl;
	bool       m_bEditUCL;
	CLabelCtrl m_btnIncrease;
	// Horizontal Option Panel
	CLabelCtrl m_btnImgSource;
	CLabelCtrl m_btnFirst;
	CLabelCtrl m_btnPrevious;
	CLabelCtrl m_lblIndex;
	CLabelCtrl m_btnNext;
	CLabelCtrl m_btnLast;
	CLabelCtrl m_btnDispMode; // Just display error image? Or display all images;
	CLabelCtrl m_btnLock;
	bool       m_bErrorMode;
	BOOL       m_bDispErrRgn;
	bool       m_bLocked;
	Hobject    m_crtImage;
	std::map<Hlong,Hobject>    m_crtErrRgn;
	Hobject  m_crtTargetRgn;
	Hobject  m_crtTargetRgnEx;
	Hobject  m_crtInterImage;
	CLabelCtrl m_btnZoomIn;
	CLabelCtrl m_btnZoomOut; // double click to restore the original image;
	CLabelCtrl m_btnSaveImg;
	CLabelCtrl m_btnSaveAllImg;
	CLabelCtrl m_btnDelCrtImg;
	CLabelCtrl m_btnAddModelImg;
	CLabelCtrl m_btnResetStatsData;

	CStatic*   m_pStaCameraStation;
	CComboBox* m_pComCameraStation;

	float m_fRatio;   //图片分辨率 和显示窗口的比率 
	// Image Display Window
	long long                m_nChImgWndID;
	HWND                     m_hChImgWnd;

	vector<long long>		 m_VecCHImgWndID;     //halcon 通道显示窗口ID
	vector<HWND>             m_VecCHImgWnd;		//通道显示窗口

	CRect                    m_rcImgWnd;
	CPoint                   m_oldPt;
	
	int						 m_crtImgIndex;
	int						 m_iCameraIndex;
	CHANNEL_CONFIG			 m_chConfig;
	CFont					 m_txtFont;
	CFont					 m_txtFontPB; // Font for Parameter Board
	CFont                    m_bigFont;
	COLORREF				 m_fgColor;
	COLORREF				 m_bgColor;
	CBrush					 m_bgBrush;
	FUNC_INFO*				 m_crtFuncInfo;
	FUNC_INFO*				 m_crtFuncInfoDebug;      // Current Selected Function Information.
	bool                     m_bAlgoChanged;
	bool                     m_bAppliedNewParam;
	CImageProcessor*         m_pProcessStation;
	std::vector<CImageProcessor*> m_VecpProcessStation;
	std::vector<FUNC_INFO>*  m_pFuncInfoArray; 
	std::vector<FUNC_INFO>*  m_pDebugFuncInfoArray;
	std::map<int, int>       m_dictListIndex_FuncIndex;
	std::map<int, bool>      m_dictRoiFuncIndex_Folded;
	SETTING_TYPE         m_settingType; // Camera, Light, Rejector and TaskGroup
	CColoredListCtrl     m_algoList;
	int                  m_nAlgoListIndex;
	bool                 m_bThresholdChanged;
	CStatic              m_paramBoard;
	CStatic              m_algoHelpStrPanel;
	CStatic              m_paramHelpStrPanel;
	CEdit                m_editThreshold;
	CChartCtrl           m_chartCtrl;
	std::vector<wstring> m_vParamHelpStr;
	std::vector<CWnd*>   m_vParamNameCtrl;
	std::vector<CWnd*>   m_vParamEditCtrl;
	CWnd*                m_crtEditCtrl;
	CButton              m_btnApplyParam;
	CButton              m_btnSaveTR;

	int                     m_nImgCount;
	bool					m_bDebugMode;        // TRUE:if the image source is folder; else FLASE
	bool					m_bSelfLearningMode; // default value is false
	bool					m_bSelfLearningFunc; // if ch has selflearning func 
	CRect					m_SelfrcBtn;		 // 可变按钮区域
	bool					m_bOpRslt;
	int						m_radioIndex;
	std::vector<UINT>		m_vChSwitchId;

	REJECTOR_SETTING		m_crtRS; 
	bool                    m_bRsChanged;
	CAMERA_SETTING			m_crtCS;
	bool                    m_bCsChanged;
	LIGHT_SETTING           m_crtLS;
	bool                    m_bLsChanged;
	COLORREF				m_wndBkColor;
	std::map<float,long>    m_dictOutputStats;
	float                   m_fStrideLen;
	float                   m_fStatsMin;
	float                   m_fStatsMax;
	BOOL                    m_bReadImg;
	// Increase/Decrease Button Location
	CRect                   m_rcDecreaseBtn;
	bool                    m_bDecBtnHold;
	bool                    m_bIncBtnHold;
	CRect                   m_rcIncreaseBtn;
	// Edit Region Button
	long                    m_nCrtBtnID;
	// For adjusting light luminance value
	HANDLE                  m_hModelToolProc;

	std::map<long, FUNC_INFO*> m_dictBtnID_FI;       // Edit Button's Pointer <--> function information;
	std::map<long, wstring>    m_dictBtnID_File;     // Edit Button's  Pointer <--> file path
	std::map<long, int>        m_dictBtnID_FileType; // Edit Button's  Pointer <--> file type (Model file or region file)

	// Light Conroller Widgets
	std::vector<CWnd*>         m_vLcWidgetPtr;
	std::vector<CWnd*>         m_vLcTagPtr;
	std::vector<CWnd*>         m_vLcSliderPtr;
	std::vector<CWnd*>         m_vLcStaticPtr;
	std::map<CWnd*, CWnd*>     m_dictSlider_Value;

	CStatic			m_staticLcPort;
	CEdit			m_editLcPort;
	CStatic			m_staticIDs;
	CComboBox		m_comboIDs;
	CStatic			m_staticMode;
	CComboBox		m_comboMode;
	// Line 1
	CStatic			m_staticTag1;
	CEdit			m_editTag1;
	CStatic			m_staticLum1;
	CSliderControl	m_sliderLum1;
	CStatic			m_staticValueLum1;


	//delay and lighttime //line1
	CStatic			m_staticDelay;
	CSliderControl	m_sliderDelay;
	CStatic			m_staticValueDelay;
	CStatic			m_staticDur;
	CSliderControl	m_sliderDur;
	CStatic			m_staticValueDur;

	//delay and lighttime //line2
	CStatic			m_staticDelay4;
	CSliderControl	m_sliderDelay4;
	CStatic			m_staticValueDelay4;
	CStatic			m_staticDur4;
	CSliderControl	m_sliderDur4;
	CStatic			m_staticValueDur4;

	//delay and lighttime //line3
	CStatic			m_staticDelay2;
	CSliderControl	m_sliderDelay2;
	CStatic			m_staticValueDelay2;
	CStatic			m_staticDur2;
	CSliderControl	m_sliderDur2;
	CStatic			m_staticValueDur2;

	//delay and lighttime //line4
	CStatic			m_staticDelay3;
	CSliderControl	m_sliderDelay3;
	CStatic			m_staticValueDelay3;
	CStatic			m_staticDur3;
	CSliderControl	m_sliderDur3;
	CStatic			m_staticValueDur3;

	//the Singlemode of lightcontrol set
	bool m_bLightSetSingleMode;


	// Line 2
	CStatic			m_staticTag2;
	CEdit			m_editTag2;
	CStatic			m_staticLum2;
	CSliderControl	m_sliderLum2;
	CStatic			m_staticValueLum2;
	// Line 3
	CStatic			m_gbLine3;
	CStatic			m_staticTag3;
	CEdit			m_editTag3;
	CStatic			m_staticLum3;
	CSliderControl	m_sliderLum3;
	CStatic			m_staticValueLum3;
	// Line 4
	CStatic			m_gbLine4;
	CStatic			m_staticTag4;
	CEdit			m_editTag4;
	CStatic			m_staticLum4;
	CSliderControl	m_sliderLum4;
	CStatic			m_staticValueLum4;

	LIGHT_SETTING   m_newLightSetting;
	int             m_nCrtLcID;
	int             m_nCrtMode;
	int				m_nActiveID;  //当前激活通道
	CSliderControl* m_pCrtSlider;

	CString         m_sManualPath;
	// Add or delete test item in place
	int             m_nCrtRgnID;
	int             m_nCrtFuncIndex;
	int             m_nCrtListIndex;	
	// image zoom-in/zoom-out
	bool            m_bZoomIn;
	float           m_fZoomInRatioTotal;
	CRect           m_rcZoonIn;
	CRect           m_rcZoonInDragEnd;
	HCURSOR         m_bkCursor;

	// For updating algorithm list
	std::vector<FUNC_INFO>           m_vFuncInfo;
	std::vector<IMG_PRODUCT>         m_vResultGroup;
	std::vector<DisplayResult>       m_vResultDisplay;
	std::vector<IMG_PRODUCT>         m_crtImageGroup;
	std::vector<vector<IMG_PRODUCT>> m_lockedImgArray;
	CChannelControlPanel*           m_pChannelControlPanel;

	// For displaying multi-channel images
	bool   m_bMultiChannnelImg;
	size_t m_nImgChannelCount;
	int    m_nCrtImgChannel;
    int    m_CameraIndex; 
    bool   m_bFullScreen;
    bool   m_bSplicing;
    bool   m_bModelFiles;

	HCURSOR m_hcursor_fingers_hand;
	HCURSOR m_hcursor_hand;

	HCURSOR m_hCursor;
public:

	afx_msg void OnLvnItemchangedListAlgo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void FramePaint();

	//light single mode
	afx_msg void OnBnClickedCheckLightSingleMode();
	void InitLightSingleModeCtrlPos();
	void SetSingleModeControlShow(int nCmdShow);
	void UpdateSingleMode();
	void SetCH1ShowWindow(int nCmdShow);
	void SetCH2ShowWindow(int nCmdShow);
	void SetCH3ShowWindow(int nCmdShow);
	void SetCH4ShowWindow(int nCmdShow);

	void DisPlayModelImage(int StationIndex,Hobject crtImg,Hobject retRgn);
	CButton m_bCheckSingleMode;
	bool m_bSingleMode;
	bool m_bSingleModeCH1;
	bool m_bSingleModeCH2;
	bool m_bSingleModeCH3;
	bool m_bSingleModeCH4;

	bool m_bOneTouchFour;

	afx_msg void OnNMReleasedcaptureSliderDelay2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderDuration2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderDelay3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderDuration3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderDelay4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderDuration4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	CButton m_bCheckTaskAttach;
	afx_msg void OnBnClickedCheckTaskParam();
	afx_msg void OnBnClickedRadioCamera1();
	afx_msg void OnBnClickedRadioCamera2();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	bool m_bDragZoomImage;
	CPoint  m_DragBeginPoint;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	bool m_bSpaceButtonDown;

	void DisplayZoomDebugObject();
};
