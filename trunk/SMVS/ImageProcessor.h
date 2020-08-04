#pragma once

#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <algorithm>
#include "TypeDef.h"
#include "bounded_buffer.h"
#include "utils.h"
#include <boost\lockfree\queue.hpp>
#include <boost\circular_buffer.hpp>
#include <boost\tuple\tuple.hpp>
#include <boost\atomic.hpp>

using namespace std;
using namespace boost;

class CChannelControlPanel;
class CChannelConfigDlg;
class CLogger;
class CSecretaryDevice;

#ifdef CROWN_PRODUCT
#import "CrownCapAlgo.dll" no_namespace
#endif

#ifdef EOE_PRODUCT
#import "EoeAlgo.dll" no_namespace
#endif

#ifdef CAN_PRODUCT
#import "CanAlgo.dll" no_namespace
#endif

#ifdef CAN_EOE_PRODUCT
#import "EoeAlgo.dll" no_namespace
#import "CanAlgo.dll" no_namespace
#endif

#ifdef PET_PRODUCT
#import "PetAlgo.dll" no_namespace
#endif

#ifdef PCC_PRODUCT
#import "PccAlgo.dll" no_namespace
#endif

#ifdef CAP_PRODUCT
#import "CapacitorAlgo.dll" no_namespace
#endif

#ifdef MAT_PRODUCT
#import "RubberMatAlgo.dll" no_namespace
#endif

#ifdef NEEDLE_PRODUCT
#import "NeedleAlgo.dll" no_namespace
#endif

#ifdef PLUG_PRODUCT
#import "PlugAlgo.dll" no_namespace
#endif

#ifdef HDPE_PRODUCT
#import "HdpeAlgo.dll" no_namespace
#endif

#ifdef JELLY_PRODUCT
#import "JellyAlgo.dll" no_namespace
#endif

#ifdef CAN_OUTSIDE_PRODUCT
#import "CanOutsideAlgo.dll" no_namespace
#endif

#ifdef NAIL_PRODUCT
#import "NailAlgo.dll" no_namespace
#endif

#ifdef BATTERY_PRODUCT
#import "BatteryShellAlgo.dll" no_namespace
#endif

#ifdef BOTTLE_PRODUCT
#import "BottleCapAlgo.dll" no_namespace
#endif

#ifdef AL_PLASTIC_PRODUCT
#import "AlPlasticCapAlgo.dll" no_namespace
#endif


class CImageProcessor
{
public:
	CImageProcessor();
	~CImageProcessor(void);
public:
	void SetTask(TASK task);
	void SetChOwnerWindowPtr(CChannelControlPanel* pOwnerWnd);
	void SetChSecretaryDevicePtr(CSecretaryDevice* pDevice);
	void SetChannelWindowOpenedFlag(bool bOpened);
	void UpdatedictBtnID_File(wstring& wstr,int index);
	void UpdateImageProcessFuncInfo(std::vector<FUNC_INFO> *vFuncInfo);

	void SetAlgoParamCriticalSecPtr(CRITICAL_SECTION* pCs);
	void SetStationIndex(int index);
	void SetChConfigPtr(CHANNEL_CONFIG* pChConfig);
	void SetDispErrRgnFlag(bool bDispErrRgn);
	bool ChangeDebugParameter(DISPID& funcId, int funcIndex, std::vector<INPUT_PARAM>& vNewParam);
	void ApplyNewParameter(DISPID& funcId, int funcIndex);
	void RestoreDebugParameter(DISPID& funcId, int funcIndex);
	void ReceiveImage(IMAGE_BOX& image);
	void ReceiveImageEx(IMAGE_BOX& image,int mutiAcqIdFlag);
	void Start();
	void Stop();
	std::vector<FUNC_INFO>* GetFuncInfoArrayPtr();
	std::vector<FUNC_INFO>* GetDebugFuncInfoArrayPtr();
	void GetCrtDebugImgErrorRegion(std::map<Hlong,Hobject>& CurUnionErrorRgn);
	void GetCrtDebugImgTargetRegion(Hobject& crtTargetRgn);
	void GetCrtDebugImgTargetRegionEx(Hobject& crtTargetRgnEx);
	
	void GetCrtDebugInterImageObject(Hobject&  crtInterImage);
	void GetCrtDebugImageObject(Hobject&  crtImage);
	// Self-learning 
	int GetModelImageCount();
	void DisplayModelImage(int nIndex, const wstring& wstrParam);
	bool AddModelImage(Hobject& modelImg, CString sFolder);
	bool DeleteModeImage(unsigned int imgIndex);
	bool SaveTrainningResult(wstring& wstrFolder);
	void SetSelfLearningTargetTaskName(wstring wstrTaskName);

	void SetCrtSelectedFuncIndex(int funcIndex);
	void ResetDisplayRule();
	void ResetDisplayWindow();
	bool IsModelImageReady();

	volatile bool  m_bStationStatus;
	void ResetSelfLearningProcedure();
	void ReportStationStatus2ChUi(bool bOk);
	void ChangeImageGrabbingMode();
	void SetChLoggerPtr(CLogger* pLogger);

	void SetImgSize(const IMG_SZ& imgSz);
	void ResetInternalStats();
	void PilotRunImageProcessor();
	void ReloadExternalFile(wstring wstrRgnFile);
	void EnableFunc(int nFuncIndex, bool bEnable, bool bTestFunc);
	void PassInParam(HTuple CameraIndexOfFlag,HTuple HomMat2DForFlag,HTuple LeftCol4ImgsInStd,HTuple RightCol4ImgsInStd);
	bool GetSpecialParamsStr(CString& strParams);
	void GetCenterLocation(CString& strParams);
    bool HasGlueInjectorTracingFunc();
	void SetRunImageSize(int Width,int Height);
	LONG GetDetectParam();
private:
    bool Init_ImageChecking(LPVOID pInstance);
	void ResetInternalData(IMAGE_BOX crtImg);
	bool ProduceFuncInfoList(wstring strHelp,IT_PARAM& itParams, FUNC_INFO& funcInfo);
	unsigned int __stdcall ImageCheckingThread();
	unsigned int __stdcall SelfLearningThread();
	bool CheckCurrentImage(std::vector<FUNC_INFO>* pFuncInfoArray, IMAGE_BOX& crtImg);

	inline void RetrieveTargetRegion(int crtFunIndex);
	void CollectInternalStats(SPC_DATA_PT& spcDataPt);
	void StoreCheckResult(bool bOk,IMAGE_BOX& crtImg);
	void UpdateImageCount(const IMAGE_BOX& crtImg, int nFuncIndex);
	void CollectInnerStats(int nFuncIndex,int nRgnID);
	void CollectErrorRegion(int nRgnID);
	void CollectFuncErrorCount(const IMAGE_BOX& crtImg,int nFuncIndex,int nRoiFuncIndex);
	UINT CollectFuncOutputValue(const IMAGE_BOX& crtImg, const FUNC_INFO& fi, float fRetValue);
	void CollectCrtFuncScore(wstring wstrOutputName, const THRESHOLD& threshold,CComVariant& score);
	wstring ParseSelfLearningParam(const wstring& wstrInput);
	long TrackGlueInjectorID(DISPID funcId);
	BOOL Check_GlueInjectorAvailability(std::wstring wName);
private:
	int                    m_cameraGroupIndex;
	bool                   m_bProcessorOk;
	CChannelControlPanel*  m_pStationOwner;
	CSecretaryDevice*      m_pChSecretaryDevice;

#ifdef CROWN_PRODUCT
	boost::tuple<CComPtr<ICrownCapAlgo> m_algoArray;
#endif
	
#ifdef EOE_PRODUCT
	boost::tuple<CComPtr<IEoeAlgo>>  m_algoArray;
#endif

#ifdef CAN_PRODUCT
	boost::tuple<CComPtr<ICanAlgo>>  m_algoArray;
#endif

#ifdef CAN_EOE_PRODUCT
	boost::tuple<CComPtr<IEoeAlgo>,CComPtr<ICanAlgo>>  m_algoArray;
#endif

#ifdef PET_PRODUCT
	boost::tuple<CComPtr<IPetAlgo>>  m_algoArray;
#endif

#ifdef JELLY_PRODUCT
	boost::tuple<CComPtr<IJellyAlgo>>  m_algoArray;
#endif

#ifdef PCC_PRODUCT
	boost::tuple<CComPtr<IPccAlgo>>  m_algoArray;
#endif

#ifdef CAP_PRODUCT
	boost::tuple<CComPtr<ICapacitorAlgo>>  m_algoArray;
#endif

#ifdef MAT_PRODUCT
	boost::tuple<CComPtr<IRubberMatAlgo>>  m_algoArray;
#endif

#ifdef NEEDLE_PRODUCT
	boost::tuple<CComPtr<INeedleAlgo>>  m_algoArray;
#endif

#ifdef PLUG_PRODUCT
	boost::tuple<CComPtr<IPlugAlgo>>  m_algoArray;
#endif

#ifdef HDPE_PRODUCT
	boost::tuple<CComPtr<IHdpeAlgo>>  m_algoArray;
#endif

#ifdef CAN_OUTSIDE_PRODUCT
	boost::tuple<CComPtr<ICanOutsideAlgo>>  m_algoArray;
#endif

#ifdef NAIL_PRODUCT
	boost::tuple<CComPtr<INailAlgo>>        m_algoArray;
#endif

#ifdef BATTERY_PRODUCT
	boost::tuple<CComPtr<IBatteryShellAlgo>>  m_algoArray;
#endif

#ifdef BOTTLE_PRODUCT
	boost::tuple<CComPtr<IBottleCapAlgo>>  m_algoArray;
#endif

#ifdef AL_PLASTIC_PRODUCT
	boost::tuple<CComPtr<IAlPlasticCapAlgo>>  m_algoArray;
#endif


	const long                  BUFFER_SZ;
	const long                  ERROR_IMG_BUFFER_SZ;
	long                        m_imgIndex;
	bounded_buffer<IMAGE_BOX>   m_imgBuffer;
	// SPC
	Hobject                      m_crtImage;
	std::map<Hlong,Hobject>         m_crtUnionErrRgn;
	std::vector<MODEL_IMAGE>     m_vModelImage;
	float                        m_fCrtModelScore;
	//
	IMG_PRODUCT                m_crtImgProduct;
	IMG_PRODUCT                m_crtImgProduct_Debug; 
	DisplayResult			   m_crtDisplayResult;
	volatile bool              m_bModelImgReady; 
	Hobject                    m_crtErrorImg;
	boost::atomic<bool>        m_bChWndOpened;
	bool                       m_bDispErrRgn;
	// For Testing Rejector
	bool                       m_bFirstImg;

	HANDLE                                m_hThread;
	CRITICAL_SECTION*                     m_pParamDataCs;          // used for reading
	CHANNEL_CONFIG*                       m_pChConfig;
	int                                   m_nCrtProductID;
	TASK                                  m_crtTask;
	std::vector<FUNC_INFO>                m_vFuncInfo;      //Parameters for all algorithm interfaces
	std::vector<FUNC_INFO>                m_vFuncInfoDebug; // Function Information for debugging
	std::map<int,bool>                    m_dictFuncIndex_Rslt;
	std::map<int,bool>					  m_dictFuncIndex_Rslt_Debug;

	std::vector<float>					  m_vInnerStatsData;

	std::map<int,double>                  m_dictFuncTiming;
	int                                   m_crtSelectedFuncIndex;
	chronograph                           m_timer;

	CLogger*                              m_pChLogger;
	bool                                  m_bHasInnerStatsIt; // Does algorithm COM support inner statistics interface?
	IMG_SZ                                m_imgSz;
	//self-learning
	wstring                               m_wstrSlParam; 
	DISPID                                m_glueRgnFuncID; // For Glue Injector ID Tracing
	union _Proc
	{
		unsigned (_stdcall * ThreadProc)(void*);
		unsigned (_stdcall CImageProcessor::*MemProc)();
	} Proc;

	struct _THRESHOLD_SCORE
	{
		wstring     m_wstrOutputName;
		THRESHOLD   m_threshold;
		CComVariant m_score;
		bool        m_bValid;
	}THRESHOLD_SCORE;

    DISPID             m_compoundStatsFuncID; // For Glue Injector ID Tracing
    int                m_nCompoundRgnID;
    std::map<int,long> m_dictInjectorID_Count;

	int m_width;
	int m_height;

	struct _image_mu{
	private:
		static boost::mutex mt_bottle;
		static boost::condition_variable cv_bottle;
		static bool	bOk;
	public:
		static void _w()
		{
			boost::unique_lock<boost::mutex> l(mt_bottle);
			cv_bottle.wait_for(l,boost::chrono::milliseconds(200),[](){return bOk;});
		}
		static void _l()
		{
			boost::lock_guard<boost::mutex> l(mt_bottle);
			bOk=true;
			cv_bottle.notify_all();
		}
		static void reset()
		{
			bOk=false;
		}
	}image_mu;
};