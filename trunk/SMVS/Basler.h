#pragma once

#include <stdlib.h>
#include "cpp/HalconCpp.h"
#include "TypeDef.h"
#include <map>
#include <boost/atomic.hpp>
#include <pylon/PylonIncludes.h>
#include <pylon/PylonGUI.h>
#include <pylon\gige\_BaslerGigECameraParams.h>
#include <pylon\gige\BaslerGigEInstantCamera.h>
#include <pylon\ConfigurationEventHandler.h>
#include <pylon\ThreadPriority.h>
#include "utils.h"

using namespace Basler_GigECamera;

using namespace Pylon;
using namespace std;
using namespace Halcon;
using namespace GenApi;

class CImageProcessor;
class CLogger;
extern Pylon::PylonAutoInitTerm autoInitTerm;

class CBaslerCamera : public CImageEventHandler // For receiving grabbed images.
{
public:	
	CBaslerCamera();
	~CBaslerCamera();

	virtual void OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult);
	virtual void DestroyImageEventHandler();
public:
	void ResetCameraFrameID();
	bool InitCamera(const CAMERA_SETTING& cameraSetting, int nDIIP);
	bool FindCamera(wstring strName, int& nDeviceIndex);
	bool OpenCamera();
	bool ApplyNewSetting(const CAMERA_SETTING& cameraSetting, wstring& strError,bool bTaskParamAttach = false);
	void SetSecretaryDeviceStatus(bool bStatus);
	void SetDispWindowID(Hlong dispWndId,CWnd* pWnd = nullptr);
	void Attach2ImageProcessor(CImageProcessor* pImgProcessor);
	void SetOpenFlag(bool bOpenFlag);
	void Start();
	void Pause();
	bool Resume();
	bool IsCameraOpened();
	void UpdateSingleFrame();
	bool IsCameraRunning();
	void SetStatusFlag(bool bInUsed);
	void SetRejectionMode(eDELAY_TYPE mode);
	void PreprocessImage(Hobject crtImg,Hobject* MappedImage, Hobject* MappedGrayImg);
	void SetChLoggerPtr(CLogger* pLogger);
	void SetGroupIndex(int index);
	void SetGroupFlag(bool bInGroup);
	bool ResetCameraParameters(CAMERA_SETTING pSet,wstring& strMsg);
	void SetDroppedImageInspectionPort(int port);
	void SetCameraAcquireMode(bool bAcquireMode);
    std::vector<ERROR_INFO> GetCameraErrorArray();
private:
	CAMERA_SETTING			m_cameraSetting;
	int                     m_nDIIP; // Dropped Image Inspection Port
	CImageFormatConverter	m_Converter;
	CPylonImage				m_TargetImage;
	CBaslerGigEInstantCamera m_camera;        // The CInstant camera
	bool                    m_bSDStatus;          // channel Secretary Device Status
	bool					m_bColorCamera;
	boost::atomic<bool>		m_bDisplayImg;
	bool					m_bInUsed;
	bool					m_bOpened;
	bool                    m_bInCameraGroup;
	bool                    m_bFirstImg;
	Hlong					m_imgDispWndID;
	CImageProcessor*        m_pImgProcessor;
	eDELAY_TYPE			    m_rjMode;
	LARGE_INTEGER			m_freq;
	LARGE_INTEGER			m_timestamp;
	LARGE_INTEGER			m_lastSnapTimestamp;
	int64_t                 m_cameraFrequency;
	int64_t                 m_chunkTimestamp;
	int64_t					m_chunkTimestamp_PC;
	long long               m_nLastImgIndex;
	long long				m_nImgIndex;
	long long				m_nFrameCounter;
	long long				m_nFirstFrameCounter;
	long					m_imgCount;
	std::vector<Hobject>	m_vTestImg;
	int						m_testImgCount;
	IMG_SZ                  m_imgSz;
	CWnd*                   m_ptrImgWnd;
	CAN_OUTSIDE_CH_PARAM    m_canOutsideChParam;
	//Testing code
	CLogger*                m_ptrChLogger;
	int                     m_cameraGroupIndex;
	HTuple                  m_measureHandles;
	int                     m_nMaxSpeed;
    std::vector<ERROR_INFO> m_vCameraError;
private:
	bool SetCameraParameters(wstring& strMsg,bool bTaskParamAttach= false);
	void ProcessGrabbedImage(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult);
	void ReadTestImage(int chIndex,int stationIndex);

	chronograph m_SystemTime;
	double m_ContinuousGrabTime;
	int		m_mutiAcqIdFlag;
	int        m_AcquisitionFrameCount;
	CMutex		m_mtxIndex;
	bool		m_bCameraAcquireMode;
};

