#pragma once

#include <cpp/HalconCpp.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <afxwin.h>
#include <ppl.h>
#include <concurrent_vector.h>
#include <boost\circular_buffer.hpp>
#include "TypeDef.h"
#include "LabelCtrl.h"
#include "BtnCtrl.h"
#include "ComboBoxCtrl.h"
#include "ChartCtrl.h"
#include "SecretaryDevice.h"
#include "Basler.h"
#include "SerialPort.h"
#include "ImageProcessor.h"
#include <boost/circular_buffer.hpp>
#include "Logger.h"
#include "ButtonST/WinXPButtonST.h"
#include "ChannelControlPanel.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost;
using namespace Halcon;
using namespace Concurrency;
using namespace boost::gregorian;

const int IDC_COMBO_ALGO_GP     = 60000; // ID of algorithm group combo box
const int IDB_ON_OFF_BTN        = 60001;
const int IDB_CH_NICKNAME       = 60002;
const int IDB_DATA_CLEAR_BTN    = 60003;
const int IDB_BTN_WARNING_SHOW  = 60004;
const int IDB_COMBO_STAY_TIME   = 60005;
#define BOOST_DATE_TIME_SOURCE 

class CImageSave{
public:
	CImageSave():bEnable(TRUE){
		
	}
	void setEnable(bool bVal){
		bEnable = bVal;
	}
	bool getEnable(){
		return bEnable;
	}
	void pushBack(Hobject obj){		
		boost::lock_guard<boost::mutex> ll(image_mutex);
		imageBuffer.push(obj);
		cv.notify_one();
	}
	Hobject popFront(){		
		boost::unique_lock<boost::mutex> ll(image_mutex);
		cv.wait(ll,[this]{return !this->imageBuffer.empty();});
		auto obj= imageBuffer.front();
		imageBuffer.pop();
		ll.unlock();
		cv.notify_one();
		return obj;
	}
	size_t getSize(){
		return imageBuffer.size();
	}
	bool isEmpty(){
		return imageBuffer.empty();
	}
	void setSavePath(std::wstring path){
		strPath=path;
	}
	void setPrefix(std::wstring prefix){
		strPrefix = prefix;
	}
	BOOL DeleteFolder(CString lpszPath)
	{ 
		TCHAR szFromPath[_MAX_PATH]={'\0'};
		wcscpy(szFromPath, lpszPath);
		szFromPath[lpszPath.GetLength() + 1] = '\0';
		szFromPath[lpszPath.GetLength() + 2] = '\0';


		SHFILEOPSTRUCT FileOp; 
		SecureZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCT));
		


		FileOp.fFlags = FOF_NOCONFIRMATION; 
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL; 
		FileOp.lpszProgressTitle = NULL;
		FileOp.pFrom = szFromPath; 
		FileOp.pTo = NULL; 
		FileOp.wFunc = FO_DELETE; 
		return SHFileOperation(&FileOp) == 0;
	}
	void start(){		
		boost::thread([this](std::wstring strPath,std::wstring strPrefix){				
			DWORD dwAttrib = GetFileAttributes(strPath.c_str());
			if(!(INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)))
			{
				if(CreateDirectory(strPath.c_str(),NULL) ==INVALID_FILE_ATTRIBUTES)
				{
					throw "Create directory failed";
					return;
				}
			}
			long nCount=1;			
			strPath +=L"\\";
			std::string strPath_cs=utils::Unicode2Ansi(strPath);
			[&](CImageSave *pServer){
				std::string strTime = posix_time::to_iso_string(posix_time::second_clock::local_time());
				int pos = strTime.find('T'); 				
				std::string strDate = strTime.substr(0,pos);
				//int nToday = atoi(strDate.c_str());
				date dtToday(atoi(strDate.substr(0,4).c_str()),atoi(strDate.substr(4,2).c_str()),atoi(strDate.substr(6,2).c_str()));
				WIN32_FIND_DATA ffd;
				HANDLE hFind;
				hFind = FindFirstFile(CString(strPath.c_str()), &ffd);
				if (INVALID_HANDLE_VALUE != hFind)
				{
					do{
						if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && _tcscmp(ffd.cFileName,_T("."))!=0 && _tcscmp(ffd.cFileName,_T(".."))!=0)
						{
							TCHAR *pEnd=NULL;
							CString StrFileName=ffd.cFileName;
							if(StrFileName.GetLength()==8)
							{
								date dtFile(_tcstol(StrFileName.Mid(0,4),&pEnd,10),_tcstol(StrFileName.Mid(4,2),&pEnd,10),_tcstol(StrFileName.Mid(6,2),&pEnd,10));
								dtFile += days(15);
								if(dtFile<dtToday)
								{
									pServer->DeleteFolder(CString(strPath.c_str())+StrFileName);
								}
							}
							
						}
					}while(FindNextFile(hFind, &ffd) != 0);
				}
			}(this);
			while(bEnable){							
				auto obj = popFront(); 				
				syncLock sl(&s_mutex);
				std::string strTime = posix_time::to_iso_string(posix_time::second_clock::local_time());
				int pos = strTime.find('T'); 
				std::string strDate="";
				if(pos !=std::string::npos){
					strTime.replace(pos,1,std::string("-")); 
					strDate = strTime.substr(0,pos);	
				}
				std::wstring wfileName=strPrefix +L"_" + utils::AnsiToUnicode(strTime) + utils::to_wstring(nCount);
				DWORD   dwAttr = GetFileAttributes(CString((strPath_cs + strDate).c_str()));
				if(dwAttr==INVALID_FILE_ATTRIBUTES || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) ==0)
				{
					CreateDirectory(CString((strPath_cs + strDate).c_str()),NULL);
				}
				
				strDate+="\\";
				std::string fileName =strPath_cs + strDate + utils::Unicode2Ansi(wfileName);
				const UINT three_gb=3221225472;
				ULARGE_INTEGER freeSpace;
				GetDiskFreeSpaceEx(CString(strPath_cs.c_str()),NULL,NULL,&freeSpace);
				if(freeSpace.QuadPart > three_gb)
				{
					write_image(obj,"tiff",0,fileName.c_str());
					nCount++;
				}
			}
		},strPath,strPrefix);
	}
private:
	std::queue<Hobject> imageBuffer;
	std::wstring strPath;
	std::wstring strPrefix;	
	boost::mutex	image_mutex;
	static CMutex s_mutex;
	boost::condition_variable cv;
	struct syncLock{
		CMutex *pMutex;
		syncLock(CMutex *object){
			pMutex=object;
			if(pMutex!=NULL)
				pMutex->Lock();
			else
				throw 1;
		}
		~syncLock(){
			pMutex->Unlock();
		}
	};

	bool bEnable;
};

class CChannelControlPanel: public CButton
{
	DECLARE_MESSAGE_MAP()
public:
	CChannelControlPanel(CHANNEL_CONFIG& channelConfig,CWnd* pParent);
	virtual ~CChannelControlPanel();
	const int               m_chIndex;       // Channel Index -- ID
	CHANNEL_CONFIG		    m_channelConfig;
	LARGE_INTEGER           m_freq;
	LARGE_INTEGER           m_oldTimStamp;
	LARGE_INTEGER			m_timestamp;
	LARGE_INTEGER			m_begintime;
	std::vector<FUNC_INFO>  m_vFuncInfo;
	std::vector<FUNC_INFO>  m_vFuncInfoDebug;
	std::vector<CLogger>   m_vChLogger;
	CSecretaryDevice        m_chSD;          // channel Secretary Device
	vector<CBaslerCamera*>   m_vChCameraPtr;
	vector<CImageProcessor*> m_vProcessorPtr;
	std::vector<HANDLE>  m_vChThreadHandle;

	int								m_mainchannel;     //此通道的父通道
	int								m_subchannel;		  //此通道的自通道

	std::map<unsigned long,bool>       m_dictSignalMap;   //坏盖信号缓存

	CRITICAL_SECTION        m_paramDataCs;   // used for updating Channel Configuration Screen.
	CRITICAL_SECTION        m_spcDataCs;
	CRITICAL_SECTION        m_spcDataCs2;
	CRITICAL_SECTION        m_deviceDataCs;
	CRITICAL_SECTION        m_imgArrayCs;
	LRESULT  OnSelfLearningComplete(WPARAM wParam,LPARAM lParam);
	LRESULT  OnChannelStatusChanged(WPARAM wParam,LPARAM lParam);
	void ApplyNewChannelSetting(bool bHardware);
	void UpdateCameraSetting(const CAMERA_SETTING& cameraSetting,bool bTaskParamAttach = false);
	void UpdateLightSetting(const wstring& wstrGpName, const LIGHT_SETTING& lightSetting);
	void UpdateChLightController(const LIGHT_SETTING& ls);
	void UpdateRejectorSetting(const REJECTOR_SETTING& rejectorSetting);
	void UpdateChannelStatistics();
	bool TurnOnCameraAtSystemBoot();
	void AddChError(ERROR_INFO &error);
	void DelChError(int errorID);
	CAMERA_SETTING GetNewCameraSetting(int t);
	void TurnOnCamera();
	void PauseChCamera();
	void HandleNoCameraConnectionEvent();
	void StartImgProcessor(bool bCameraSetChanged = false);
	void StopImgProcessor();
	void OpenSpcScreen();
	void OpenChannelScreen();
	std::vector<CImageProcessor*> GetImgProcessorPtr();
	void SetChannelStatus();
	bool IsChCameraRunning();
	bool IsChCameraOpened();
	bool ResumeAllCamera();
	std::vector<ERROR_INFO> GetErrorInfo();
	eCHANNEL_STATE  GetChannelState();
	void ShowChannelDataClearButton(bool bShow);
	int GetTaskSelByName(CString strname);
	bool AutoChangeTask(CString& taskname,bool bAuto= false);
	CString FindAdaptTaskNameEx(CString taskname);
	CString findAdaptTaskName(CString taskname);
	afx_msg void  OnStnClickedOnOff();
	afx_msg void OnStnClickedOnOff_EX();
	afx_msg void  OnCbnSelchangeAlgoGroup();
	afx_msg void  OnCbnSelchangeStayTime();
	afx_msg void  OnStnDblClickedNickName();
	afx_msg void  OnStnClickedDataClear();
	afx_msg void  OnStnClickedEventFlag();
	void  ClearChData_Ex();
	void ReTestLightController();
	void HandleCurrentErrorEvent(const ERROR_INFO& crtErrorInfo);
	void UpdateWarningFlag();
	void ClearChError();
	void UpdateTaskThreshold();
	void ResetInternalStats();
	TASK GetCurrentTask();
	wstring GetCurrentTaskName();
	CString GetCurTaskName();
	bool findAllTaskname(CString tasknameCH1Begin,CString tasknameCH1End,CString& ReStrTaskname,CString &strErrorinfo);
	TASK GetTask(wstring wstrTaskName);
	SPC_DATA_PT GetCurrentTaskLastStatsData(CString strbatchnumber = _T(""),CString username=_T(""));
	CRect GetChartRect();
	void ExecuteQueryCmd();
	void GetChDeviceData(CHANNEL_STATUS& data);
	void SetBlowThresholdValue(int th,bool bFreshUI = true);
	void SetTestItemPassFlag_V2(bool bDebugImg);
	bool CollectCheckResult(int nProcessorIndex, bool bOk, IMG_PRODUCT& crtImgProduct,DisplayResult& crtDisplayResult,std::map<int, bool> dictFuncIndex_Rslt, bool& bRet);
	void CollectSelfResultCount(int nCount);
	void CollectFuncOutputValueALL(int fiindex, float fSpecV,float fivalue, bool bAdjustRange);
	bool GetFuncSpcData(int& funcIndex, std::map<float, long>& statistics);
	std::vector<vector<IMG_PRODUCT>> GetNgImgArray(int& ErrorIndex);
	std::vector<vector<IMG_PRODUCT>> GetContiguousImgArray();
	void RefreshChannelPanel();
	void ReConnectChannelCamera();
	void UpdateTaskSetting(eTASK_ACTION ta);
	void UpdateFunInfo(std::vector<FUNC_INFO> *m_vFuncInfo,std::vector<FUNC_INFO> *m_vFuncInfoDebug );
	void ChangeDebugParameter(DISPID& funcId, int funcIndex, std::vector<INPUT_PARAM>& vNewParam);
	void ChangeParameter(DISPID& funcId, int funcIndex, std::vector<INPUT_PARAM>& vNewParam);
	void ModifyInternalStats(int nFuncIndex, bool bAdded, bool bTestFunc);
	void RemoveInternalStats(std::vector<int>& vFuncIndex);
	//void SendRejectSignal(bool bOk,bool bDebugImg);
	//void SendRejectSignal_Ex(bool bOk,bool bDebugImg);
	void SendRejectSignal(bool bOk,bool bDebugImg,int nImgBufferSize);
	void SendRejectSignal_Ex(bool bOk,bool bDebugImg,int nImgBufferSize);
	void UpdateMainScreen(bool bOk,bool bDebugImg,bool bforce = false);
	void UpdateChConfigScreen(bool bOk,IMAGE_BOX& crtImg);
	void SetChannelImageWindowPtr(CChannelConfigDlg* pWnd);
	void SetDispAllImageFlag(bool bDispAllImage);
	void SetDebugFlag(bool bDebug);
	void DisPlayModelImage(int StationIndex,Hobject crtImg,Hobject retRgn);
	void UpdateStatistics();
	void SendDIIPAndSyncFlag();
	void SetProBeginTime();
	void GetArriveTime(LARGE_INTEGER time);
	void UpdateChannelSpeed();
	void CreateInternalData();
	void RemoveChError(int iErrorID,int IErrorSubID);
	void RemoveAllChError(int iErrorID);
	void RemoveChFirstError(int iErrorID);
	void ExecuteCmd(int QOpCmdID,int nCmdID, int nValue = 0);
	void ExecuteCmd(int nQOpCmdID,string strCmd);
	void ExecuteCmd(int nCmdID);
    void VerifyLightControllerTestResult(const std::set<int>& allOkLcIDs);
	void ClearChData();
	int GetErrorImageStayTime(){return m_nErrorImgStayTime;}
	int GetSaveImageType(){return m_SaveImageType;}
	BOOL IsSaveNgImge(){return m_SaveImageType==SAVE_NG_IMAGE;}
	BOOL GetIsInterlock();
	void AddImage(Hobject obj,bool bOK);
private:
	int Init(CHANNEL_CONFIG& chConfig);
	bool SetPanelLocation(int nChIndex, CRect& rcPanel);
	bool CreateHalconImageWnd(CRect& rcWnd, HWND& wnd, long long& wndID);
	void CreateControlPanel(CRect& rcColumn);
	void CreateCurrentImageDispPanel(CRect& rcColumn);
	void CreateButtonPanel(CRect& rcColumn);
	void CreateStatisticPanel(CRect& rcColumn);
	void CreateChartCtrlPanel(CRect& rcChart);
	void ListAlgoGroup();

	
	void WriteLeftoverStatsData2Database(wstring wstrTaskName);
	void SelectSlefLearningTargetTaskName();
	void ApplyNewCameraSetting(const CAMERA_SETTING& cameraSetting,bool bTaskParamAttach = false);
	void ResetDisplayRule();
	void ResetSelfLearningProcedure();
	bool IsModelImageReady();
	void SetTestItemPassFlag(bool bDebugImg,std::map<int, bool>& crtStats,bool bErrCountAdd = FALSE);
	void ResetTestItemPassFlag(bool bDebug);
	void UpdateRecentNGCount(int nFuncindex,bool bPass = false);
	void CheckLastNDefective(bool bPass = false);
	void UpdateInternalStatistics();
	void CollectInternalStats(SPC_DATA_PT& spcDataPt,bool boldDictFI_ECUpdate = true);
	
	void RedrawStatisticsChart();
	void CheckContinueDefective();
	void SendSignal(bool bOK);
	void SendRejectorTestingSignal(bool bOk);
    void CollectChannelError(std::vector<ERROR_INFO>& vError);
	void SetBatchNumber(CString batchnumberstr);

	bool AllInspectingResultReady();
	std::vector<IMG_PRODUCT> GetCurrentImgGroup();

	//add  UDP send DATA info Reject Error
//	bool m_bRjectError;
//	std::vector<CString> m_vecRjectErrorLog;
private:
#pragma region UI
	eCHANNEL_STATE m_channelState;
	CFont          m_font;
	CFont          m_btnTextFont;
	int            m_panelH;        // Panel Height;
	int            m_panelW;        // Panle Width;
	CRect          m_rcPanel;
	COLORREF       m_fgColor;       // foreground color;
	COLORREF       m_bgColor;       // Background color;
	int            m_labelFontSz;   // Label Font Size
	int            m_valueFontSz;   // SPC result value font size;

	CString		   m_strbatchnumber;   // 批次号

	CLabelCtrl    m_backgroundCtrl1; // Control Column Background
	CLabelCtrl    m_chIndexPanel;    // Channel index panel.
	CLabelCtrl    m_laneNamePanel;
	CLabelCtrl    m_OnOffBtn;        // Start and Stop Button

	CWinXPButtonST m_DataClearBtn;
	CLabelCtrl    m_cLabWarnShow;
	CLabelCtrl    m_chSpeedPanel;    // Channel real-time speed

	CLabelCtrl    m_currentImgWnd;
	long long     m_currentImgWndID;
	CLabelCtrl    m_backgroundCtrl3;
	CComboBoxCtrl m_comboChTask;
	CLabelCtrl	  m_lbStayTime;
	CComboBoxCtrl m_comboStayTime;
	int           m_nTaskComboIndex;
	CBtnCtrl      m_btnChDebug;       // Button used for changing channel setting
	CBtnCtrl      m_spcBtn; // SPC/Statistic Detail
	CLabelCtrl    m_backgroundCtrl4;
	CLabelCtrl	  m_totalText;
	CLabelCtrl    m_totalValue;
	CLabelCtrl    m_failedText;
	CLabelCtrl    m_failedValue;
	CLabelCtrl    m_ratioText; 
	CChartCtrl    m_chartCtrl;
	CLabelCtrl	  m_hNgImgWnd;
#pragma endregion
	CWnd*					      m_pParentWnd;
	volatile bool                 m_bImgProcessorOK;
	// Database
	long                          m_nSampleSize;
	SPC_DATA_PT                   m_lastDataPt;
	boost::circular_buffer<float> m_statisticsBuffer;
    long                          m_nOldImgCount;
	CHANNEL_STATUS                m_chDeviceData;
	CString                       m_enableFlagStr;

	unsigned long                m_nTotalCount;
	unsigned long				 m_nOldTotalCount;
	unsigned long                m_nDefective;
	unsigned long                m_nOldDefective;
	int                          m_nCameraCount;
	int                          m_nDataBlockCount;
	std::vector<vector<IMG_PRODUCT>>  m_ngImgContainer;
	int							 IMAGE_ARRAY_SZ;

	boost::circular_buffer<std::vector<IMG_PRODUCT>> m_contiguousImgArray;
	int							 m_nCurErrorIndex;
	std::map<int,queue<IMG_PRODUCT>> m_pendingInspectedImgGroup;
	std::map<int,queue<DisplayResult>> m_pendingDisplayResult;
	std::map<int,queue<map<int, bool>>> m_dictProcessorIndex_FuncRslt;

	boost::atomic_int            m_nArrivalCount;
	boost::atomic_int            m_nCheckBeginArrivalCount;
	boost::atomic_bool           m_bFinalResult;
	std::vector<IMG_PRODUCT>     m_crtImgGroup;
	std::vector<IMG_PRODUCT>     m_crtImgGroup_Debug;

	std::vector<DisplayResult>   m_crtDisplayResult;
	std::vector<DisplayResult>   m_crtDisplayResult_Debug;


	boost::atomic_int            m_nArrivalCount_Debug;
	boost::atomic_bool           m_bFinalResult_Debug;
	boost::atomic_address        m_pChConfigWnd;
	bool                         m_bDebugMode;
	bool                         m_bDispAllImg;
	// Check continuous NG product
	std::stack<unsigned long>    m_stackNgIndex;
	std::stack<unsigned long>    m_stackNgLastN;

	// Updating Main Screen
	std::map<int, unsigned long> m_dictFI_EC;       // Function Index <-> Error Count
	std::map<int, unsigned long> m_oldDictFI_EC;

	std::map<long,std::map<float, long>>  m_statistics; //Function index ,value ,count
	std::map<long, MIN_MAX>               m_dictFuncIndex_MinMax; // Function Index <===> Min/Max of this function output 
	//Rejector
	volatile bool  m_bRejectorCardOk;
	long           m_nIntervalCount;
	bool           m_bLaunchSignal;
	
	int             m_nImgIndex;
	LARGE_INTEGER   m_ArriveTime;
	double			m_RejectTime;
	double			m_ProBeginTime;
	int				m_RejectMinTime;
	long				m_nSyncFlag;
	std::queue<int>	m_QueRecentErrCount;
	float           m_sRate;
	std::vector<ERROR_INFO>  m_vChError;

	int m_blowThreshold;
	//
	chronograph m_chronograph;
	double m_LastErrorTime;
	CPen penFrame;
	CPen penFrame1;

	CRect m_Rect1;
	bool m_bCurOk;

	//新排盖方式添加
	long long m_ImageID;  //相机计数
	long long m_ImageTimeCount;  //相机时间戳
	long long m_ImageTimeCount_PC; //PC时间戳	

	bool m_bRjectError;    //警戒模式标记
	int  m_bRjectErrorCount;   //警戒模式自动恢复次数
	int m_nErrorImgStayTime; //error image stay time
	eERROR_LEVEL m_ChannelWarnlevel;

	int m_SaveImageType;
	CImageSave m_SaveImage;
};
