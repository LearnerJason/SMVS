#pragma once
#include <string>
#include <utility>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <boost/atomic.hpp>
// #include "PCI2310Device.h"
// #include "PCI2312Device.h"
// #include "PCI2330Device.h"
//#include "PCI2330Device.h"
#include "TypeDef.h"
#include "SqliteDB.h"
#include "SerialPort.h"
#include "bounded_buffer.h"
#include "Basler.h"
#include "WarningError.h"
#include "DisplayingManager.h"
#include "DongleKey.h"
#include "stackwalker.h"
#include "TaskChangePara.h"

class CChannelControlPanel;
class CImageProcessor;

using namespace std;
using namespace boost;

class CSystemManager
{
public:
	CSystemManager();
	 ~CSystemManager();

// #ifdef PCI2312
// 	CPCI2312Device       m_pciDevice; // using PCI 2312
// #endif
// 
// #ifdef PCI2310
// 	CPCI2310             m_pciDevice;  // PCI 2310
// #endif
// 
// #ifdef PCI2330
// 	CPCI2330Device             m_pciDevice;  // PCI 2310
// #endif

	CDisplayingManager   m_displyManager;
	// Cameras within system LAN
	CDongleKey           m_dongleKey;
	long long            m_nActionCount;

	DeviceInfoList_t       m_cameraDeviceInfoList;
	std::map<wstring,bool> m_dictCamera_State;
	std::vector<HANDLE>  m_vHandle;
	std::vector<HANDLE>  m_vChThreadHandle;
	int                  m_nSerialPort;
	CSerialPort*         m_pSerialPort;
    std::set<int>        m_setCheckedLcID;
	HANDLE				 m_hLcTestReadyEvent; // Light controller testing ready event 
	HANDLE               m_dLcTestOverEvent;  // Indicate that light controller testing is over;
	bool				 m_ExternFlag;
	
	volatile int  m_nSysChIndex;    // which channel the counter is linked to
	wstring       m_crtSysTaskName; 
	SYS_STAT_DATA m_crtSysStats;
	// Can Outside
	CAN_OUTSIDE_COMMON_PARAM  m_canOutsideCommonParam;

	void SetMainDlgPtr(CWnd* pMainDlg);
	void SetSerialPortOwnerWindow(CWnd* pOwnerWnd);
	void RestoreSerialPortOwnerWindow();
	bool CreatePciDevice();
	void EnableDataClearButton();
	void InitializeSerialPortForLightController();
	void CreateAllPanels();
	void ReCreateAllPanels();
	CString GetAllSecretaryDeviceVersion();
	void StartRejetorThread();
	void StartChannelImgProcessor();
	void StartImageDisplayThread();
	void UpdateAllChannelWarningFlag();
	void StartChannelStatusSentryThread();
	void StartDatabaseSentryThread();
	void StartCameraGroupSupervisorThread();
	void StopWorkerThread();
	void CheckTaskNameUniqueness();
	void ReceiveStatsDataPoint(TASK& task, const SPC_DATA_PT& dataPt);
	void ReceiveSystemStatsDataPoint(bool bLastData = false);
	bool TurnOnAllCameras();
	void TurnOnChannelCamera(int nChIndex);
	void PauseAllCamera();
	void PauseChannelCamera(int nChIndex);
	bool IsChannelCameraOk(int nChIndex);
	bool IsChannelCameraRunning(int nChIndex);
	bool IsAnyCameraRunning();
	void StopImgProcessor();
	void UpdateTaskThreshold();
	void StopRejector();
	void CloseSerialPort();
	void FinalClean();
	void SaveLeftoverStatisticsData();
	void SaveLeftoverStatisticsData(int nIndex);
	void UpdateCameraSetting(int chIndex, const CAMERA_SETTING& cameraSetting,bool bCameraModeChange =false);
	void UpdateChLightController(int nChIndex,const LIGHT_SETTING& ls);
	void UpdateChannelStats();
	void UpdateRejectorSetting(int chIndex, const REJECTOR_SETTING& rejectorSetting);
	void UpdateTaskSetting();
	void RefreshSystem();
	std::vector<CChannelControlPanel*> GetPanelArray();
	CChannelControlPanel* GetPanel(int ChIndex);
	std::vector<ERROR_INFO>  GetAllErrorInfo();
	bool IsTableExists(string strTableName);
	bool AllChannelCameraStopped();
	bool IsBatchNumberFunTrue();
	bool IsUpdateDatabaseFunTrue();
	bool IsClearAllDataFunTrue();
	bool IsAutoChangeTaskFuncTrue();
	bool AutoChangeTask(CString taskname);
	bool IsShowSystemStateFunTrue();
	eERROR_LEVEL GetErrorLevel(wstring wstrID);
	bool GetProductNameFromCH1(CString taskname,CString &strPattern);
	bool QueryProductColumn(int curChIndex, wstring strTaskName,vector<CString> &m_vecColumnName);
	bool FindCHFullTaskName(int curChIndex, wstring strTaskName,CString &StrTaskNameNew);
	bool QueryProductStats(int nChIndex, wstring strTaskName, long long lBgn, long long lEnd, std::map<long long, SPC_DATA_PT>& dictDataPoint,CString strbatchnumber = _T(""),CString username=_T(""));
	bool QuerySystemProductStats(long long lBgn, long long lEnd, std::map<long long, SYS_STAT_DATA>& dictDataPoint,CString strbatchnumber = _T(""),CString username=_T(""));
	bool ExtractSystemStatsFromDatabase(long long lBgn, long long lEnd, std::map<long long, SYS_STAT_DATA>& dictDataPoint,CString strbatchnumber= _T(""),CString username=_T(""));
	bool ExtractDataFromDatabase(int nChIndex, wstring strTaskName, long long lBgn, long long lEnd, std::map<long long, SPC_DATA_PT>& dictDataPoint,CString strbatchnumber= _T(""),CString username=_T(""));
	void ExtractSystemStatsFromMemory(long long lBgn, long long lEnd, std::map<long long, SYS_STAT_DATA>& dictDataPoint,CString strbatchnumber= _T(""),CString username=_T(""));
	void ExtractDataFromMemory(int nChIndex, wstring strTaskName, long long lBgn, long long lEnd, std::map<long long, SPC_DATA_PT>& dictDataPoint,CString strbatchnumber= _T(""),CString username=_T(""));	void ReportNoCameraConnection(int nChIndex);
	// UAC management
	bool CreateUacTable();
	bool InsertDefualtUser();
	bool InsertUacData(const UAC& uacData);
	bool AddNewUser(const UAC& uacData, CString& sError);
	vector<CString> GetLastMonthBatchNumber(std::string strTableName,long long lBgn, long long lEnd);
	bool GetCurBatctNumberMinMaxTime(std::string strTableName,CString cstrBatchNumber,vector<CString> &m_vecTime);
	bool UpdateUser(const UAC& oldUac, const UAC& newUac, CString& sError);
	bool DeleteUser(wstring sUsername);
	bool GetAllUacInfo(std::map<wstring, UAC>& uacDict);
	bool ValidateAuthority(CString sUsername, CString sPassword, int& nErrorCode);
	UAC  GetCurrentUser();
	bool SetCurrentUser(const wstring& crtUser);
	//
	bool GetTaskChangedFlag();
	void SetChannelTaskChangedFlag(int nCh,eTASK_ACTION ta);
	void ResetChannelTaskChangedFlag();
	void SetChImageSize(int nChIndex, const IMG_SZ& imgSz);
	IMG_SZ GetChImageSize(int nChIndex);
	void ResetChannelStats(int nChIndex);
	CRect GetChartRect();

	bool ApplyNewLcParameter(LC lc,int CHID = 0);
	bool WriteLcParameter2EEP(const LC& lc);
	void SendLightControllerTestSignal(int nChIndex = -1);
	void VerifyLightControllerTestResult();
    void RemoveLcIdFromCheckResult(const std::vector<int>& vLcID);
	void SetNewModelName(CString sNewModelName);
	CString GetNewModelName();
	bool ModifyDatabaseTable(TASK& oldTask, TASK& newTask);
    void UpdateChConfig(int nChIndex,CHANNEL_CONFIG chConfig);
	long IncreaseSyncFlag(int nChIndex);
	long  DecreaseSyncFlag(int nChIndex);
	void ResetSyncFlag(int nChIndex);
	CHANNEL_CONFIG* GetChConfigReference(int nChIndex);
	void GetChDeviceData(int nChIndex,CHANNEL_STATUS& data);
	void InitSerialPort();
	void ListAllAvailableCamera(std::map<wstring, bool>& mapNameState);
	void AddPendingImage(int nChIndex, IMAGE_BOX crtImgBox,PREPROCESS_RESULT chData);
    bool IsLcSerialPortOk();
	void CreateImageJunction(int nChIndex, int nCameraCount);
	void CollectImageFromGroupCamera(int nChIndex, int nGroupIndex, IMAGE_BOX& crtProduct);
	void OnCameraConnectionLostEvent(wstring wstrCameraName);
public:
	bool DeleteTable(wstring wstrTableName);
	void UpdateChannelSpeed();
	void ClearCameraWarning(int nChIndex);
	void ClearAllChError();
	CString GetCurBatchNumber();
	void SetCurBatchNumber(CString strBatchNumber);
	void UpdateBatchNumberStr();
	void ResetAllChannelData();

	//电容项目换型
	bool PanelAutoChangeTask(vector<CString> vec);
	bool CheckAutoTaskName(double CapHeight,bool bRollNeck,int e_Exptype,int diameter,TypeData& DataPara,vector<CString>& m_vec,CString& ReErrorinfo);
	TypeData FindTypePara(double CapHeight);
	CString GetCHTaskName(int i);
	double GetLightHeight1(double CapHeiht1);
	double GetLightHeight2(double CapHeight2);
	vector<CString> GetExpppType();
	double GetCapDiameter();
	//电容项目换型

	bool GetProductInfoVecter(vector<CString> &vec);

	bool ProductInfoToDataBase(std::vector<CString> m_vec);
	bool CreateProductInfoTable(std::vector<CString> m_vec);
	bool GetCurSysForeignKeyInfo(CString sysForeignKey, std::vector<CString> &m_vec);

	//获取外键
	CString GetCurSysForeignKey();
	void SetCurSysForeignKey(CString strBatchNumber);
	void UpdateSysForeignKey();

	void LogGlueInjectorError(int nChIndex,long nInjectorID);
	std::map<long, long> GetGlueInjectorErrorStats(int nChIndex);
	BOOL StartAllCameraEx();
	BOOL PauseAllCameraEx();
private:
	bool CreateTable(const TASK& task);
	bool CreateSystemDataTable(wstring wstrTableName);
	bool InsertChannelStatsData(const wstring& strTableName, const SPC_DATA_PT& dataPt);
	bool InsertSystemStatsData(const wstring& strTableName, const SYS_STAT_DATA& dataPt);
	void RemoveObsoleteDataFromDatabase();
	string GenerateInsertCommand(const wstring& strTableName, const SPC_DATA_PT& dataPt);
	bool ReadCommonPrerequisiteFiles();
	void GetApproxRectsInStdImg(HTuple MatchedFlagCol, HTuple CameraIndexOfFlag, HTuple* LeftCol4ImgsInStd, HTuple* RightCol4ImgsInStd);
	unsigned int __stdcall ChannelStatusSentryThreadFunc();
	unsigned int __stdcall DatabaseSentryThreadFunc();
	unsigned int __stdcall LightControllerTestingThreadFunc();
	unsigned int __stdcall CameraGroupSupervisorThreadFunc();
	void DisptchGroupImage(int nChIndex,IMAGE_JUNCTION& imgJunction);
	bool IsAnyPendingGroup();
	void QueueGroupImage(int nChIndex, int nGroupIndex, IMAGE_BOX& crtProduct);
	void HarvestGroupImage(int nChIndex);
	void ResetObsoleteImage(IMAGE_JUNCTION& imgJunction);
	bool IsAnyStatsData(wstring& wstrTaskName,SPC_DATA_PT& dataPt);
	bool IsAnySystemStatsData(wstring& wstrTaskName,SYS_STAT_DATA& dataPt);

	bool UpdateDatabaseData(BOOL bRecheck=FALSE,CString sText=L"");

private:
	CRect                              m_rcChChart;
	volatile unsigned long long        m_bootTime;
	std::vector<CChannelControlPanel*> m_vPanels;
	std::vector<vector<CImageProcessor*>> m_vChProcessorArray;
	std::vector<wstring>               m_vSerialPort;
	CWnd*                              m_pMainDlg;
	CSqliteDB<CppSQLite3Query>         m_db;
	std::map<int,int>                  m_dictChCameraConnection;
	std::map<int,double>               m_dictChMinTimeSpan;
	std::map<int,IMG_SZ>               m_dictChImgSz;   // Channel Image Size;
	std::map<int, map<long, long>>     m_dictGII_EC;    // Glue Injector ID <-->Error Count 
	CRITICAL_SECTION                   m_csGIIT;
	std::map<int,long>                 m_dictSyncFlag;
	std::map<int,IMAGE_JUNCTION>       m_dictChIndex_ImgJunction; //Channel Index <===> Image Junction;
    bool							   m_bLcSerialPortOk;
	// UAC management
	UAC                          m_crtUser;
	std::map<wstring,UAC>        m_dictUac;
    std::map<int, eTASK_ACTION>  m_dictCh_TaskAction; // channel index <---> task updated flag; 
	// Light Controller
	CString m_sNewModelName;
	// Database
	std::map<wstring, bounded_buffer<SPC_DATA_PT>*> m_dictStats; // task name <===> statistics data.
	std::map<wstring, bounded_buffer<SYS_STAT_DATA>*> m_dictSystemStatsData; // task name <===> statistics data.
	volatile unsigned int m_nExitFlag;

	// Can Outside
	std::vector<IMAGE_BOX>             m_vCOImage; // Can Outside Image array
	boost::atomic<int>                 m_nCanImgCount;
	boost::atomic<float>               m_fMaxScore;
	std::vector<PREPROCESS_RESULT>     m_vMatchData;
	CRITICAL_SECTION                   m_imgJunctionCs;
	int                                m_nMaxImgSyncTime; // maximum value of image synchronizing time, in ms
	LONGLONG                           m_nMaxImgSyncDelta;// in timestamp ticks

	// Light Controller Testing Thread
	int m_nTestFlag; // -1:test all light controllers; otherwise just test one ight controller
	union _Proc
	{
		unsigned (_stdcall * ThreadProc)(void*);
		unsigned (_stdcall CSystemManager::*MemProc)();
	} Proc;

private:
	CSystemManager(CSystemManager const &);
	CSystemManager& operator=(CSystemManager const &);

	CString m_strBatchNumber;
	CString m_strSysForeignKey;
	CString m_strlastUploadtime;
	bool m_bBatchNumberFun;
	bool m_bUpdateDatabaseFun;
	bool m_bShowSystemStateFun;
	bool m_bClearDataALLFun;

	std::vector<wstring> m_vecSeriousWarning;

	std::vector<CString> m_vecProductInfoKey;
	std::vector<CString> m_vecProductInfoValue;

	bool m_bAutoChangeTaskFun;
	
	CTaskChangePara m_TaskChange;

	std::vector<wstring> m_vecProductInfo;

	//数据库起始时间戳！
	long long m_timebegin;

};

