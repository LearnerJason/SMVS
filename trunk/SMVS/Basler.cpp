#pragma once

#include "stdafx.h"
#include <stdlib.h>
#include <atlconv.h>
#include "resource.h"
#include "Basler.h"
#include "ImageProcessor.h"
#include "ConfigurationManager.h"
#include "SystemManager.h"
#include "CameraConfiguration.h"
#include "Logger.h"
#include "ChannelControlPanel.h"
extern ConfigManager  gConfigMgr;
extern CSystemManager gSystemMgr;
extern CLogger        gLogger;


#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif

Pylon::PylonAutoInitTerm autoInitTerm;

CBaslerCamera::CBaslerCamera()
   :m_nDIIP(-1),
    m_bSDStatus(false),
    m_bColorCamera(false),
	m_bDisplayImg(true),
	m_bInUsed(false),
	m_bOpened(false),
	m_bInCameraGroup(false),
	m_bFirstImg(true),
	m_imgDispWndID(-1),
	m_pImgProcessor(nullptr),
	m_imgCount(0),
	m_testImgCount(0),
	m_rjMode(DELAY_DISTANCE),
	m_nImgIndex(-1),
	m_cameraGroupIndex(-1),
	m_nMaxSpeed(40),
	m_bCameraAcquireMode(false),
	m_AcquisitionFrameCount(1)
{
	m_camera.RegisterConfiguration(new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
	m_camera.RegisterConfiguration(new CAcquireContinuousConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);
	m_camera.RegisterConfiguration(new CameraConfigEventHandler,RegistrationMode_ReplaceAll,Cleanup_Delete);
	m_camera.RegisterImageEventHandler(this, RegistrationMode_Append, Cleanup_None);
	QueryPerformanceFrequency(&m_freq);
	QueryPerformanceCounter(&m_timestamp);
	m_chunkTimestamp = m_timestamp.QuadPart;
	QueryPerformanceCounter(&m_lastSnapTimestamp);
	m_cameraFrequency = 125000000;
	m_chunkTimestamp = 0;
	m_ptrChLogger = nullptr;
	m_ContinuousGrabTime = 0;
	m_mutiAcqIdFlag = 0;
}

CBaslerCamera::~CBaslerCamera()
{
}

void CBaslerCamera::SetGroupIndex(int index)
{
	m_cameraGroupIndex = index;
}
void CBaslerCamera::SetDroppedImageInspectionPort(int port)
{
	m_nDIIP = port;
}
void CBaslerCamera::SetCameraAcquireMode(bool bAcquireMode)
{
	m_bCameraAcquireMode = bAcquireMode;
}
bool CBaslerCamera::InitCamera(const CAMERA_SETTING& cameraSetting, int nDIIP)
{
	if(m_camera.IsOpen())
	{
		m_camera.Close();
	}
    m_vCameraError.clear();
	m_vTestImg.clear();
	m_testImgCount = 0;
	ReadTestImage(cameraSetting.m_chIndex,m_cameraGroupIndex);
	m_cameraSetting = cameraSetting;
	m_nDIIP         = nDIIP;
	m_nMaxSpeed = gConfigMgr.GetChannelMaxSpeed(m_cameraSetting.m_chIndex);
	m_bOpened = OpenCamera();
	return m_bOpened;
}

bool CBaslerCamera::FindCamera(wstring strName,int& nDeviceIndex)
{
	CString sMsg;
	CString strCameraName = strName.c_str();

	nDeviceIndex = 0;
	bool bFind = false;
	USES_CONVERSION;
	for(DeviceInfoList_t::iterator it = gSystemMgr.m_cameraDeviceInfoList.begin (); it != gSystemMgr.m_cameraDeviceInfoList.end (); it++)
	{
		CString name((*it).GetFriendlyName().c_str());
		if (name == strCameraName)
		{
			bFind = true;
			break;
		}		
		nDeviceIndex++;
	}
	if(!bFind)
	{
		sMsg.LoadString(IDS_STRING_CANNOT_CONNECT_CH_CAMERA);
		sMsg.Append(strCameraName);
		wstring strMsg = CT2W(sMsg);

		CString strID;
		strID.LoadString(IDS_STRING_ERR_ERRID);
		CString streMsg;
		streMsg.Format(_T("%s%d0%d"),strID,CAMERA_CONNECT_ERROR,ERRORSUBID_CAMERA_IS_IN_USE);
		streMsg += _T("---")+CString(strMsg.c_str())+_T("");
		ERROR_INFO emsg(CAMERA_CONNECT_ERROR,ERRORSUBID_CAMERA_CONNECT_FAILED,streMsg.GetString());
        m_vCameraError.push_back(emsg);
	}
	return bFind;
}

bool CBaslerCamera::OpenCamera()
{
	try
	{
		int deviceIndex = -1;
		if (!FindCamera(m_cameraSetting.m_name,deviceIndex))
			return false;

		m_camera.DestroyDevice();
		
		if(CTlFactory::GetInstance().IsDeviceAccessible(gSystemMgr.m_cameraDeviceInfoList[deviceIndex]))
		{
			m_camera.Attach(CTlFactory::GetInstance().CreateDevice(gSystemMgr.m_cameraDeviceInfoList[deviceIndex]));	
			m_camera.Open();
			m_bOpened = true;

			PixelFormatEnums eFormat =  m_camera.PixelFormat.GetValue();
			m_bColorCamera = (eFormat > PixelFormat_Mono16 ? true : false);

			wstring strMsg;
			bool bRet = SetCameraParameters(strMsg);
			if(!bRet)
			{
				CString strID;
				strID.LoadString(IDS_STRING_ERR_ERRID);	
				CString streMsg;
				streMsg.Format(_T("%s%d0%d"),strID,CAMERA_CONNECT_ERROR,ERRORSUBID_CAMERA_IS_IN_USE);
				streMsg += _T("---")+CString(strMsg.c_str())+_T("");

				ERROR_INFO emsg(CAMERA_CONNECT_ERROR,ERRORSUBID_CAMERA_PARAMS_ERROR,streMsg.GetString());
				m_vCameraError.push_back(emsg);

				m_camera.Close();
				m_bOpened = false;
			}
			return bRet;
		}
		else
		{
			CString sName = CW2T(m_cameraSetting.m_name.c_str());
			CString sMsg;
			sMsg.LoadString(IDS_STRING_CAMERA_IS_IN_USE);
			sMsg.Replace(_T("#"),sName);
			wstring strMsg = CT2W(sMsg);

			CString strID;
			strID.LoadString(IDS_STRING_ERR_ERRID);
			CString streMsg;
			streMsg.Format(_T("%s%d0%d"),strID,CAMERA_CONNECT_ERROR,ERRORSUBID_CAMERA_IS_IN_USE);
			streMsg += _T("---")+CString(strMsg.c_str())+_T("");
			ERROR_INFO emsg(CAMERA_CONNECT_ERROR,ERRORSUBID_CAMERA_IS_IN_USE,streMsg.GetString());
			m_vCameraError.push_back(emsg);
			return false;
		}
	}
	catch(GenICam::GenericException &e)
	{
		string strError("OpenCamera failed --");
		strError.append(e.GetDescription());
		wstring sLog = utils::Utf8ToUnicode(strError);
		gLogger.QueueUpLog(sLog);
		return false;
	}
	return true;
}

bool CBaslerCamera::ResetCameraParameters(CAMERA_SETTING pSet,wstring& strMsg)
{
	if(m_bOpened == false)
	{
		return false;
	}
    if(m_camera.DeviceScanType.GetValue() == DeviceScanType_Linescan)
    {
        return true;
    }
	std::string strNodeName;
	try
	{
// 		if(!m_bSDStatus)
// 		{
// 			m_camera.TriggerDelayAbs.SetValue(m_cameraSetting.m_delayValue);
// 		}
		m_camera.GainAuto.SetValue(GainAuto_Off);
		m_camera.GainRaw.SetValue(pSet.m_gain);

		m_camera.ExposureAuto.SetValue(ExposureAuto_Off);
		m_camera.ExposureTimeAbs.SetValue(pSet.m_ExposureTime);
	}
	catch(GenICam::GenericException &e)
	{
		string strError("Invalid Camera Parameter:");
		strError.append(strNodeName);
		strError.append(" - ");
		strError.append(e.GetDescription());
		strMsg = utils::Utf8ToUnicode(strError);
		gLogger.QueueUpLog(strMsg);
		return false;
	}
	return true;
}

bool CBaslerCamera::SetCameraParameters(wstring& strMsg,bool bTaskParamAttach)
{
	if(m_bOpened == false)
	{
		return false;
	}
    if(m_camera.DeviceScanType.GetValue() == DeviceScanType_Linescan)
    {
        return true;
    }
	std::string strNodeName;
	try
	{
		//固件106200-13 bug 修复！debouncer 参数无法生效修复！
// 		m_camera.LineSelector.SetValue(LineSelector_Line1);
// 		double d = m_camera.LineDebouncerTimeAbs.GetValue();
// 		m_camera.LineDebouncerTimeAbs.SetValue(d+1);
// 		m_camera.LineDebouncerTimeAbs.SetValue(d);

		GenApi::CIntegerPtr pHeartbeat = m_camera.GetTLNodeMap().GetNode("HeartbeatTimeout");
		if(pHeartbeat != nullptr)
		{
			pHeartbeat->SetValue(20*1000);
		}
		m_imgSz.m_nWidth  = int(m_camera.Width.GetValue());
		m_imgSz.m_nHeight = int(m_camera.Height.GetValue());
	
		gSystemMgr.SetChImageSize(m_cameraSetting.m_chIndex, m_imgSz);
		if(IsWritable(m_camera.GevSCPSPacketSize))
		{
			m_camera.GevSCPSPacketSize.SetValue(m_cameraSetting.m_nPacketSz);
		}

		//相机chunkmode 使能
		m_camera.ChunkModeActive.SetValue(true);
		//相机FrameChunk Timestamp，Framecounter 使能
		m_camera.ChunkSelector.SetValue(ChunkSelector_Timestamp);
		m_camera.ChunkEnable.SetValue(true);

		m_camera.ChunkSelector.SetValue(ChunkSelector_Framecounter);
		m_camera.ChunkEnable.SetValue(true);

		m_camera.TriggerSelector.SetValue(TriggerSelector_AcquisitionStart);

		//相机输出曝光输出使能
		if(IsWritable(m_camera.LineSelector))
		{
			try{
				m_camera.LineSelector.SetValue((LineSelectorEnums)m_cameraSetting.m_LineSelector);
			}catch(GenICam::GenericException &e){
				m_camera.LineSelector.SetValue(LineSelector_Line2);
			}			
		}
		if(IsWritable(m_camera.LineSource) && IsAvailable(m_camera.LineSource))
		{
			try{
				m_camera.LineSource.SetValue((LineSourceEnums)m_cameraSetting.m_LineSource);
			}catch(GenICam::GenericException &e){

			}			
		}
		//m_camera.LineSelector.SetValue(LineSelector_Out1);
		//m_camera.LineSource.SetValue(LineSource_ExposureActive);
		//m_camera.LineSelector.SetValue(LineSelector_Out1);
		//m_camera.LineSource.SetValue(LineSource_TimerActive);
		
		//m_camera.TimerDelayTimebaseAbs.SetValue(10);
		//m_camera.TimerDurationAbs.SetValue(5000);

		if(IsWritable(m_camera.TriggerSelector))
		{
			if (m_bCameraAcquireMode)
			{
				m_AcquisitionFrameCount = m_camera.AcquisitionFrameCount.GetValue();
				m_camera.TriggerSelector.SetValue(TriggerSelector_FrameStart);
				m_camera.TriggerMode.SetValue(TriggerMode_Off);
				m_camera.TriggerSelector.SetValue(TriggerSelector_AcquisitionStart);
			}
			else
			{
				m_camera.TriggerSelector.SetValue(TriggerSelector_AcquisitionStart);
				m_camera.TriggerMode.SetValue(TriggerMode_Off);
				m_camera.TriggerSelector.SetValue(TriggerSelector_FrameStart);
			}
		}
		if(m_cameraSetting.m_triggerAct == RISE_EDGE)
		{
			m_camera.TriggerActivation.SetValue(TriggerActivation_RisingEdge);
		}
		else
		{
			m_camera.TriggerActivation.SetValue(TriggerActivation_FallingEdge);
		}
		if(m_bSDStatus ||((!m_bSDStatus) && m_cameraSetting.m_bTriggerMode))
		{
			m_camera.TriggerMode.SetValue(TriggerMode_On);
			m_camera.AcquisitionFrameRateEnable.SetValue(false);
		}
		else  // Camera Free Mode. 
		{
			m_camera.TriggerMode.SetValue(TriggerMode_Off);
			double dMin = m_camera.AcquisitionFrameRateAbs.GetMin();
			double dMax = m_camera.AcquisitionFrameRateAbs.GetMax();
			double dFrameRate = (dMin > (double)m_cameraSetting.m_nFrameRate) ? dMin : (double)m_cameraSetting.m_nFrameRate;
			long nRate = (long)::ceill(dFrameRate);
			if(nRate > dMax)
			{
				nRate = (long)::floor(dMax);
			}
			m_camera.AcquisitionFrameRateAbs.SetValue((double)nRate);
			m_camera.AcquisitionFrameRateEnable.SetValue(true);
		}

		if (!bTaskParamAttach)
		{
			m_camera.GainAuto.SetValue(GainAuto_Off);
			m_camera.GainRaw.SetValue(m_cameraSetting.m_gain);

			m_camera.ExposureAuto.SetValue(ExposureAuto_Off);
			m_camera.ExposureTimeAbs.SetValue(m_cameraSetting.m_ExposureTime);

// 			if(!m_bSDStatus)
// 			{
// 				m_camera.TriggerDelayAbs.SetValue(m_cameraSetting.m_delayValue);
// 			}
		}
		if (IsWritable(m_camera.ChunkModeActive))
		{
			m_camera.ChunkModeActive.SetValue(true);
			m_camera.ChunkSelector.SetValue(ChunkSelector_Timestamp);
			m_camera.ChunkEnable.SetValue(true);
		}
		else
		{
			throw RUNTIME_EXCEPTION( "The camera doesn't support chunk features");
		}
		if(IsReadable(m_camera.GevTimestampTickFrequency))
		{
			m_cameraFrequency = m_camera.GevTimestampTickFrequency.GetValue();
		}

		//Frame on图片没有缓存，连续图片分组识别
		m_ContinuousGrabTime = 1000.f/m_camera.ResultingFrameRateAbs.GetValue()+3;
		//m_ptrChLogger->QueueUpLog(L"Initialize Camera OK!");
	}
	catch(GenICam::GenericException &e)
	{
		if(m_ptrChLogger != nullptr)
		{
			string strError("Invalid Camera Parameter:");
			strError.append(strNodeName);
			strError.append(" - ");
			strError.append(e.GetDescription());
			strMsg = utils::Utf8ToUnicode(strError);
			m_ptrChLogger->QueueUpLog(strMsg,true);
		}
		return false;
	}
	return true;
}

void CBaslerCamera::SetChLoggerPtr(CLogger* pLogger)
{
	m_ptrChLogger = pLogger;
}

void CBaslerCamera::SetGroupFlag(bool bInGroup)
{
	m_bInCameraGroup = bInGroup;
}

void CBaslerCamera::Attach2ImageProcessor(CImageProcessor* pImgProcessor)
{
	m_pImgProcessor = pImgProcessor;
	//PilotRunImageProcessor
	m_pImgProcessor->SetRunImageSize(m_imgSz.m_nWidth,m_imgSz.m_nHeight);
}
//警戒模式恢复，重置相机FrameID
void CBaslerCamera::ResetCameraFrameID() 
{
	CSingleLock l(&m_mtxIndex);
	l.Lock();
	m_nImgIndex=-1;
	l.Unlock();
}
void CBaslerCamera::OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult) 
{
	static boost::chrono::system_clock::time_point time_send;
	{
		CSingleLock l(&m_mtxIndex);
		l.Lock();
		m_nImgIndex++;
		l.Unlock();
	}
	if(m_nImgIndex == 0)
	{
		//HANDLE hThread = Pylon::GetCurrentThreadHandle();
		//Pylon::SetRTThreadPriority(hThread, 15);
	}
	if(ptrGrabResult->GrabSucceeded())
	{
		if(!m_bInCameraGroup)
		{
			int nFlag =(((m_nImgIndex/m_AcquisitionFrameCount) % 2 == 0)? IO_ON : IO_OFF);
// 			CString str;
// 			str.Format(_T("%d,%d,%d"),m_nImgIndex,m_AcquisitionFrameCount,nFlag);
// 			OutputDebugString(str);
//			gSystemMgr.m_pciDevice.WritePort(m_nDIIP,nFlag);
			gSystemMgr.IncreaseSyncFlag(m_cameraSetting.m_chIndex);
		}

		GenApi::CIntegerPtr chunkFrameCounter(ptrGrabResult->GetChunkDataNodeMap().GetNode("ChunkFramecounter"));
		GenApi::CIntegerPtr chunkTimestamp(ptrGrabResult->GetChunkDataNodeMap().GetNode("ChunkTimestamp"));

		if(IsReadable(chunkTimestamp)&&IsReadable(chunkFrameCounter))
		{
			m_nFrameCounter = chunkFrameCounter->GetValue();

			int64_t ct = chunkTimestamp->GetValue();

			if (m_nImgIndex == 0)
			{
				m_nFirstFrameCounter = m_nFrameCounter;
			}

			float fCameraTriggerGap = (ct - m_chunkTimestamp) * 1000.0f / m_cameraFrequency;
// 			CString str;
// 			str.Format(_T("%.3f\n"),fCameraTriggerGap);
// 			OutputDebugString(str);
// 			if (fCameraTriggerGap < m_ContinuousGrabTime+5 )
// 			{
// 				m_mutiAcqIdFlag++;
// 			}
// 			else
// 			{
// 				m_mutiAcqIdFlag=0;
// 			}
			m_mutiAcqIdFlag = m_nImgIndex%m_AcquisitionFrameCount;
			if(fCameraTriggerGap < (600.0f/m_nMaxSpeed))
			{
				wstring wstrMsg = L"Abnormal camera trigger signal --> ";
				wstrMsg.append(boost::lexical_cast<wstring>(fCameraTriggerGap));
				m_ptrChLogger->QueueUpLog(wstrMsg,true);
			}
			m_chunkTimestamp = ct;
		}
		else
		{
			//if(m_nImgIndex < 16)
			{
				m_ptrChLogger->QueueUpLog(L"Reading camera timestamp failed...",true);
				return;
			}
		}
		try
		{
			if(time_send.time_since_epoch().count()>0)
			{
				auto now_time=boost::chrono::system_clock::now();
				auto diff= now_time - time_send;
				time_send = now_time;
				if(diff.count() /10000.0 > 100)
				{
					std::wostringstream wstr;
					wstr<<L"Image "<< m_nImgIndex << L"Grabbled Image interval time greater than 100ms";
					//gLogger.QueueUpLog(wstr.str());
					wstr.str(L"");
				}
			}
			else
				time_send=boost::chrono::system_clock::now();
			ProcessGrabbedImage(camera, ptrGrabResult);
		}
		catch (GenICam::GenericException &e)
		{
			string strError = "Process grabbed image Exception: "; 
			strError+=e.GetDescription();
			wstring sLog = utils::Utf8ToUnicode(strError);
			m_ptrChLogger->QueueUpLog(sLog,true);
			m_ptrChLogger->FlushLogCache();
			return;
		}
	}
	else  // The image was incompletely grabbed
	{
		if(!camera.IsCameraDeviceRemoved())// Do not handle errors caused by device removal.
		{
			// Grabbing an image can fail if the used network hardware, i.e. network adapter, 
			// switch or Ethernet cable, experiences performance problems.
			// Increase the Inter-Packet Delay to reduce the required bandwidth.
			// It is recommended to enable Jumbo Frames on the network adapter and switch.
			// Adjust the Packet Size on the camera to the highest supported frame size.
			// If this did not resolve the problem, check if the recommended hardware is used.
			// Aggressive power saving settings for the CPU can also cause the image grab to fail.

			m_nImgIndex        = -1;
			GenApi::CIntegerPtr chunkTimestamp(ptrGrabResult->GetChunkDataNodeMap().GetNode("ChunkTimestamp"));
			auto timeVal = chunkTimestamp->GetValue();
			string msg = "Image grab failed : ";
			msg += ptrGrabResult->GetErrorDescription();
			wstring wstrMsg = CA2W(msg.c_str());
			CString str;
			str.Format(_T("ChunkTimestamp:%I64u,camera name: %s"),timeVal,m_cameraSetting.m_name.c_str());
			wstrMsg += str.GetString();
			if(m_ptrChLogger != nullptr)
			{
				m_ptrChLogger->QueueUpLog(wstrMsg,true);
			}
		}
		else
		{
			m_ptrChLogger->QueueUpLog(L"Image grab failed : Camera Device Removed!",true);
		}
	}
}

void CBaslerCamera::DestroyImageEventHandler()
{
	//rewrite this function and DO NOT do anything.
}

void CBaslerCamera::ProcessGrabbedImage(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult)
{
	static long long imageTime_s=0;
	if (m_camera.IsGrabbing()) 
	{
		int iWidth  = ptrGrabResult->GetWidth();
		int iHeight = ptrGrabResult->GetHeight();
		Hobject crtImg;
		if(!m_bColorCamera)
 		{
			CSyncLock l;
			gen_image1(&crtImg,"byte",iWidth, iHeight,(Hlong)ptrGrabResult->GetBuffer());
		}
		else
		{
			m_Converter.OutputPixelFormat = PixelType_RGB8planar ;
			m_Converter.Convert(m_TargetImage,ptrGrabResult);
			Hlong redPlaneBuff   = (Hlong)m_TargetImage.GetPlane(0).GetBuffer();
			Hlong greenPlaneBuff = (Hlong)m_TargetImage.GetPlane(1).GetBuffer();
			Hlong bluePlaneBuff  = (Hlong)m_TargetImage.GetPlane(2).GetBuffer();
			gen_image3(&crtImg,"byte",iWidth,iHeight,redPlaneBuff, greenPlaneBuff, bluePlaneBuff);
		}
		if(m_testImgCount != 0)
		{
			int index = m_imgCount % m_testImgCount;
			m_imgCount++;
			crtImg = m_vTestImg[index];
			utils::RemoveDebugSymbol(crtImg);
		}
		if(crtImg.Id() == H_EMPTY_REGION)
			return;

		IMAGE_BOX product(crtImg); 
		product.m_ImageID = m_nFrameCounter-m_nFirstFrameCounter;  //图片序号
		product.m_ImageTimestamp = (long long )(m_chunkTimestamp *1000000.0/ m_cameraFrequency); //  图片相机时间戳，单位微秒
		product.m_ImageTimestamp_PC = m_SystemTime.DoubleNowEx(); //此时间节点与取反信号之间可能存在时间差

		if(imageTime_s==0)
			imageTime_s = product.m_ImageTimestamp;
		long long imageTime_e=product.m_ImageTimestamp;
		if(imageTime_e-imageTime_s > 100 * 1000 )
		{
			CString str;
			str.Format(_T("channel:%d -- ID:%u	  m_ImageTime_s:%u  m_ImageTime_e:%u"),m_cameraSetting.m_chIndex,product.m_ImageID,imageTime_s,imageTime_e);
			//gLogger.QueueUpLog(str.GetString(),true);
		}
		imageTime_s = imageTime_e;

		//if (m_cameraSetting.m_chIndex==1)
		if((product.m_ImageTimestamp-product.m_ImageTimestamp_PC)/ 1000000.0 > 40)
		{
			CString str;
			str.Format(_T("channel:%d, ID:%u, FrameID:%u,  m_ImageTimestamp:%u, m_ImageTimestamp_PC:%u"),m_cameraSetting.m_chIndex,product.m_ImageID,m_nFrameCounter,product.m_ImageTimestamp,product.m_ImageTimestamp_PC);
			//gLogger.QueueUpLog(str.GetString(),true);
		}

		if(!m_bInCameraGroup)
		{
			m_pImgProcessor->ReceiveImageEx(product,m_mutiAcqIdFlag);			
		}
 		else
 		{
// 			if (m_cameraSetting.m_chIndex==4)
// 			{
// 				CString str;
// 				str.Format(_T("m_chIndex:%d  groupindex:%d"),m_cameraSetting.m_chIndex,m_cameraGroupIndex);
// 				gLogger.QueueUpLog(str.GetString(),true);
// 			}
// 			for (int i = 0 ; i < 4 ; i ++)
// 			{
// 				if (i==3 && m_nImgIndex%10 ==9)
// 				{
// 					continue;
// 				}
// 				gSystemMgr.CollectImageFromGroupCamera(m_cameraSetting.m_chIndex,  i , product);
// 			}
			//CString str;
			//str.Format(_T("CH_%d_camera_%d	ID=%d,m_nFrameCounter=%d,m_nFirstFrameCounter=%d"),m_cameraSetting.m_chIndex, m_cameraGroupIndex,product.m_ImageID,
			//	m_nFrameCounter,m_nFirstFrameCounter);
			//gLogger.QueueUpLog(str.GetString(),true);
			gSystemMgr.CollectImageFromGroupCamera(m_cameraSetting.m_chIndex, m_cameraGroupIndex, product);

 		}

		if(m_cameraGroupIndex == 0)
		{
			LARGE_INTEGER bgnStamp;
			QueryPerformanceCounter(&bgnStamp);
// 			if((bgnStamp.QuadPart - m_timestamp.QuadPart) * 1000.0f / m_freq.QuadPart < 333.33f)
// 			{
// 				return;
// 			}
// 			else
// 			{
				if(m_mutiAcqIdFlag==0)
				{
					gSystemMgr.m_displyManager.ReceiveRealTimeImage(m_cameraSetting.m_chIndex,crtImg);
				}
				else
				{
					int subindex = gSystemMgr.GetPanel(m_cameraSetting.m_chIndex)->m_subchannel;
					if (subindex !=0)
					{
						gSystemMgr.m_displyManager.ReceiveRealTimeImage(subindex,crtImg);
					}
				}
				
	//		}
//			m_timestamp = bgnStamp;
		}
	}
}

void CBaslerCamera::Start()
{
	if (m_camera.IsPylonDeviceAttached())
	{
		if(!m_bOpened)
			m_camera.Open();
		try
		{
			m_camera.StartGrabbing(GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
		}
		catch (GenICam::GenericException &e)
		{
			string strError = e.GetDescription();
			//TODO: Write this error message into log file.
			return;
		}
		m_bOpened = true;
		m_bInUsed = true;
	}
}

void CBaslerCamera::Pause()
{
	try
	{
		if(m_camera.IsGrabbing())
		{
			m_camera.StopGrabbing();
			m_bInUsed = false;
		}
		gSystemMgr.ResetSyncFlag(m_cameraSetting.m_chIndex);
	}
	catch (GenICam::GenericException &e)
	{
		string strError = e.GetDescription();
		wstring sLog = utils::Utf8ToUnicode(strError);
		m_ptrChLogger->QueueUpLog(sLog,true);
		m_ptrChLogger->FlushLogCache();
	}
}

bool CBaslerCamera::Resume()
{
	if(!m_bOpened)
		return m_bOpened;

	try
		{
			if(!m_camera.IsGrabbing())
			{
				m_nImgIndex   = -1;
				m_camera.StartGrabbing(GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);

				m_bInUsed = true;
				if(m_bInCameraGroup)
				{
					if (m_cameraGroupIndex==0)
					{
						gSystemMgr.ResetSyncFlag(m_cameraSetting.m_chIndex);
					}
				}
				else
				{
					gSystemMgr.ResetSyncFlag(m_cameraSetting.m_chIndex);
				}

				return m_bOpened;
			}
		}

		catch (GenICam::GenericException &e)
		{
			string strError = e.GetDescription();
			wstring sLog = utils::Utf8ToUnicode(strError);
			m_ptrChLogger->QueueUpLog(sLog,true);
			m_ptrChLogger->FlushLogCache();

			return false;
		}
		return m_bOpened;
}

void CBaslerCamera::SetSecretaryDeviceStatus(bool bStatus)
{
	m_bSDStatus = bStatus;
}

void CBaslerCamera::SetDispWindowID(Hlong dispWndId,CWnd* pWnd)
{
	m_imgDispWndID = dispWndId;
	if(pWnd != nullptr)
	{
		m_ptrImgWnd = pWnd;
	}
}

bool CBaslerCamera::ApplyNewSetting(const CAMERA_SETTING& cameraSetting, wstring& strError,bool bTaskParamAttach)
{
	m_cameraSetting = cameraSetting;
	return SetCameraParameters(strError,bTaskParamAttach);
}

bool CBaslerCamera::IsCameraRunning()
{
	return m_bInUsed && m_bOpened;
}

bool CBaslerCamera::IsCameraOpened()
{
	return m_bOpened;
}

void CBaslerCamera::SetStatusFlag(bool bInUsed)
{
	m_bInUsed = bInUsed;
}

void CBaslerCamera::ReadTestImage(int chIndex,int stationIndex)
{
	CFileFind ff;
	BOOL ret = FALSE;
	CString strCh;
	strCh.Format(_T("\\CH%d%d"), chIndex,stationIndex+1);
	CString strPath = _T("C:\\TestImageFiles") + strCh;

	CString imgPath = strPath + _T("\\*.tiff");
	BOOL flag = ff.FindFile(imgPath);

	CString strFileName;
	while(flag)
	{
		flag = ff.FindNextFile();
		if(ff.IsDirectory())
		{
			continue;
		}
		strFileName = strPath + _T("\\") + ff.GetFileName();
		string fileName = CT2A(strFileName);
		Hobject img;
		read_image(&img, fileName.c_str());
		utils::RemoveDebugSymbol(img);
		m_vTestImg.push_back(img);
	}

	m_testImgCount = (int)m_vTestImg.size();
}

void CBaslerCamera::SetRejectionMode(eDELAY_TYPE mode)
{
	m_rjMode = mode;
}

void CBaslerCamera::SetOpenFlag(bool bOpenFlag)
{
	m_bOpened = bOpenFlag;
} 

std::vector<ERROR_INFO> CBaslerCamera::GetCameraErrorArray()
{
    return m_vCameraError;
}