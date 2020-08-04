#include "stdafx.h"//
#include "ChannelControlPanel.h"
#include "ConfigurationManager.h"
#include "SystemManager.h"
#include "resource.h"
#include "SMVSDlg.h"
#include "ApplySlResultDlg.h"
#include "ChannelConfigDlg.h"
#include "SpcDlg.h"
#include "LaneNameDlg.h"
#include "Logger.h"
#include "ui_decorator.h"
#include "ChannelEventMgrDlg.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include "ConfigurationManager.h"
#include <fstream>
#include <boost/thread/future.hpp>
using namespace boost;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma warning(disable:4244)
#pragma warning(disable:4996)

extern ConfigManager  gConfigMgr;
extern CSystemManager gSystemMgr;
extern ui_decorator   gUiDecorator;

#define  MINI_BTN_SIZE 24


CMutex CImageSave::s_mutex;

CChannelControlPanel::CChannelControlPanel(CHANNEL_CONFIG& channelConfig,CWnd* pParent):
		m_channelConfig(channelConfig),
		m_chSD(channelConfig.m_rejectorSetting),
	    m_chIndex(channelConfig.m_chIndex),
		m_bImgProcessorOK(false),
		m_statisticsBuffer(channelConfig.m_spcSetting.m_nXLen + 1),
		m_nSampleSize(1000),
		m_nTotalCount(0),
		m_nOldTotalCount(0),
		m_nDefective(0),
		m_nOldDefective(0),
		m_nArrivalCount(0),
		m_nCheckBeginArrivalCount(0),
		m_nArrivalCount_Debug(0),
		m_bFinalResult(true),
		m_bFinalResult_Debug(true),
		m_nIntervalCount(0),
		m_bRejectorCardOk(false),
		m_bLaunchSignal(false),
		m_pChConfigWnd(nullptr),
		m_bDebugMode(false),
		m_bDispAllImg(false),
		m_nImgIndex(0),
		m_nCurErrorIndex(0),
		m_RejectTime(0),
		m_sRate(0.0f),
		m_blowThreshold(3),
		m_bCurOk(true),
		m_RejectMinTime(0),
		m_bRjectError(false),
		m_ProBeginTime(0),
		m_mainchannel(0),
		m_subchannel(0),
		m_nErrorImgStayTime(0),
		m_bRjectErrorCount(0),
		m_ChannelWarnlevel(WL_NONE)
{	
	QueryPerformanceFrequency(&m_freq);
	QueryPerformanceCounter(&m_oldTimStamp);
	QueryPerformanceCounter(&m_timestamp);
	m_nTaskComboIndex = -1;
	::InitializeCriticalSection(&m_paramDataCs);
	::InitializeCriticalSection(&m_spcDataCs);
	::InitializeCriticalSection(&m_spcDataCs2);
	::InitializeCriticalSection(&m_deviceDataCs);
	::InitializeCriticalSection(&m_imgArrayCs);

	CString sEventName(_T("Sync-"));
	SYSTEMTIME dt;
	::GetLocalTime(&dt);

	//错误显示框！
	penFrame.CreatePen(PS_SOLID, 2, RGB(200,220,250));
	penFrame1.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

	m_font.CreateFont(16 ,0, 0,	0,FW_NORMAL,FALSE, FALSE,0,ANSI_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("Arial"));

	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	float rate = rect.Height() /1080.0;
	if(rate > 1.0f)
		rate = 1.0f;
	m_labelFontSz = (int)(22 * rate);
	m_valueFontSz = (int)(20 * rate);
	m_btnTextFont.CreateFont(m_labelFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0, ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("Arial"));

	m_pParentWnd = pParent;
	m_panelH = 204;
	m_panelW = 1000;
	
	m_fgColor = RGB(10,10,115);
	m_bgColor = RGB(200,220,250);
	m_currentImgWndID = 0;

	m_mainchannel = channelConfig.m_mainchannel;
	m_subchannel = channelConfig.m_subchannel;

	if (channelConfig.m_chWarningflag == _T(""))
	{
		m_enableFlagStr = _T("111111");
		channelConfig.m_chWarningflag = _T("111111");
	}
	else
	{
		m_enableFlagStr = channelConfig.m_chWarningflag.c_str();
	}
	Init(channelConfig);
	m_nCameraCount = (int)channelConfig.m_vecCameraSetting.size();
	bool bInCameraGroup = false;
	if(m_nCameraCount > 1)
	{
		bInCameraGroup = true;
	}

//	::gSystemMgr.CreateImageJunction(channelConfig.m_chIndex,m_nCameraCount);
	
	wstring wstr_ng_img_count = gConfigMgr.GetBasicSetting(_T("NGIMGCOUNT"));
	IMAGE_ARRAY_SZ = _wtoi(wstr_ng_img_count.c_str());
	if (IMAGE_ARRAY_SZ == 0)
	{
		IMAGE_ARRAY_SZ = 128;
	}
	//动态设置处理时间，1.应对连盖波动，2.黑图测试适配，3减少配置复杂度
	if (m_channelConfig.m_RejectMinTime !=0)
	{
		m_RejectMinTime  =  m_channelConfig.m_RejectMinTime;
	}
	else
	{
		m_RejectMinTime  = m_channelConfig.m_rejectorSetting.m_CameraGrabTime - 10;
		if (m_RejectMinTime < 10)
		{
			m_RejectMinTime =10;
		}
	}


    CreateInternalData();
}

void CChannelControlPanel::RemoveAllChError(int iErrorID)
{
	for (auto it  =  m_vChError.begin(); it != m_vChError.end();)
	{
		ERROR_INFO emsg = (*it);
		if (emsg.m_errorID == iErrorID)
		{
			 it = m_vChError.erase(it);
		}
		else
		{
			 it ++;
		}
	}
}

void CChannelControlPanel::RemoveChFirstError(int iErrorID)
{
	for (auto it  =  m_vChError.begin(); it != m_vChError.end();)
	{
		ERROR_INFO emsg = (*it);
		if (emsg.m_errorID == iErrorID)
		{
			it=m_vChError.erase(it);			
		}
		else
			it++;
	}
}
void CChannelControlPanel::RemoveChError(int iErrorID,int IErrorSubID)
{
	for (auto it  =  m_vChError.begin(); it != m_vChError.end();it++)
	{
		ERROR_INFO emsg = (*it);
		if (emsg.m_errorID == iErrorID && emsg.m_errorSubID == IErrorSubID)
		{
			m_vChError.erase(it);
			break;
		}
	}
}
 
void CChannelControlPanel::CreateInternalData()
{
	for(auto it= m_vChCameraPtr.begin(); it != m_vChCameraPtr.end();it++)
	{
		CBaslerCamera* p  = *it;
		delete p;
	}
	m_vChCameraPtr.clear();

	for(auto it= m_vProcessorPtr.begin(); it != m_vProcessorPtr.end();it++)
	{
		CImageProcessor* p  = *it;
		delete p;
	}
	m_vProcessorPtr.clear();

	m_nCameraCount = (int)m_channelConfig.m_vecCameraSetting.size();
	if(m_nCameraCount == 0&&m_mainchannel==0&&m_subchannel==0)
	{
		//TODO:
		return;
	}
	bool bInCameraGroup = false;
	if(m_nCameraCount > 1)
	{
		bInCameraGroup = true;
	}
	m_nDataBlockCount = (m_channelConfig.m_bSingleProcessor)? 1:m_nCameraCount;
	::gSystemMgr.CreateImageJunction(m_channelConfig.m_chIndex,m_nCameraCount);
	
	if (m_nCameraCount!=0)
	{
		m_crtImgGroup        = std::vector<IMG_PRODUCT>(m_nDataBlockCount,IMG_PRODUCT());
		m_crtImgGroup_Debug      = std::vector<IMG_PRODUCT>(m_nDataBlockCount,IMG_PRODUCT());
		m_crtDisplayResult       = std::vector<DisplayResult>(m_nDataBlockCount,DisplayResult());
		m_crtDisplayResult_Debug = std::vector<DisplayResult>(m_nDataBlockCount,DisplayResult());
		m_ngImgContainer         = std::vector<vector<IMG_PRODUCT>>(IMAGE_ARRAY_SZ, std::vector<IMG_PRODUCT>(m_nDataBlockCount,IMG_PRODUCT()));
	}
	else
	{
		m_crtImgGroup        = std::vector<IMG_PRODUCT>(1,IMG_PRODUCT());
		m_crtImgGroup_Debug      = std::vector<IMG_PRODUCT>(1,IMG_PRODUCT());
		m_crtDisplayResult       = std::vector<DisplayResult>(1,DisplayResult());
		m_crtDisplayResult_Debug = std::vector<DisplayResult>(1,DisplayResult());
		m_ngImgContainer         = std::vector<vector<IMG_PRODUCT>>(IMAGE_ARRAY_SZ, std::vector<IMG_PRODUCT>(1,IMG_PRODUCT()));
	}

// 	m_vecRjectErrorLog.reserve(100);
// 	m_vecRjectErrorLog.resize(100);

	m_contiguousImgArray     = boost::circular_buffer<std::vector<IMG_PRODUCT>>(IMAGE_ARRAY_SZ);

	for(auto t = 0; t < m_nCameraCount;t++)
	{
		//辅助线程不生成取图线程
		if (m_mainchannel <=0)
		{
			CBaslerCamera* pCamera = new CBaslerCamera;
			pCamera->SetGroupIndex(t);
			pCamera->SetGroupFlag(bInCameraGroup);
			pCamera->SetRejectionMode(m_channelConfig.m_vecCameraSetting[0].m_delayType);
			if (m_subchannel!=0)
			{
				pCamera->SetCameraAcquireMode(true);
			}
			wostringstream woss;
			woss << L"CH" << m_channelConfig.m_chIndex << L"_" << (t + 1);
			CLogger tmpLogger(woss.str());
			m_vChLogger.push_back(tmpLogger);
			m_vChCameraPtr.push_back(pCamera);
		}

		if(!m_channelConfig.m_bSingleProcessor)
		{
			CImageProcessor* pProcessor = new CImageProcessor;
			pProcessor->SetChOwnerWindowPtr(this);
			pProcessor->SetChSecretaryDevicePtr(&m_chSD);

			pProcessor->SetAlgoParamCriticalSecPtr(&m_paramDataCs);
			pProcessor->SetStationIndex(t);
			m_vProcessorPtr.push_back(pProcessor);
		}
	}

	//跨通道处理
	if (m_nCameraCount==0)
	{
		if(!m_channelConfig.m_bSingleProcessor)
		{
			CImageProcessor* pProcessor = new CImageProcessor;
			pProcessor->SetChOwnerWindowPtr(this);
			pProcessor->SetChSecretaryDevicePtr(&m_chSD);

			pProcessor->SetAlgoParamCriticalSecPtr(&m_paramDataCs);
			pProcessor->SetStationIndex(0);
			m_vProcessorPtr.push_back(pProcessor);
		}
	}
	if(m_channelConfig.m_bSingleProcessor)
	{
		CImageProcessor* pProcessor = new CImageProcessor;
		pProcessor->SetChOwnerWindowPtr(this);
		pProcessor->SetChSecretaryDevicePtr(&m_chSD);

		pProcessor->SetAlgoParamCriticalSecPtr(&m_paramDataCs);
		pProcessor->SetStationIndex(0);
		m_vProcessorPtr.push_back(pProcessor);
	}

	for(auto t = 0; t < m_nCameraCount;t++)
	{
		m_vChCameraPtr[t]->SetChLoggerPtr(&m_vChLogger[t]);
	}
	if(m_channelConfig.m_bSingleProcessor && !m_vProcessorPtr.empty() && !m_vChLogger.empty())
	{
		m_vProcessorPtr[0]->SetChLoggerPtr(&m_vChLogger[0]);
	}
	if(m_vProcessorPtr.size() == m_vChLogger.size())
	{
		for(auto tt = 0; tt < m_vProcessorPtr.size();tt++)
		{
			m_vProcessorPtr[tt]->SetChLoggerPtr(&m_vChLogger[tt]);
		}
	}
	m_chSD.SetChindex(m_channelConfig.m_chIndex);

	if (m_mainchannel!=0)
	{
		m_bRejectorCardOk = false;
	}
	else
	{
		m_bRejectorCardOk = m_chSD.InitDevice();
	}

	m_bRejectorCardOk= true;
	if(m_bRejectorCardOk)
	{
		bool bTaskParamAttach = false;
		//获取任务绑定参数标记位！
		TASK* pCrtTask = m_channelConfig.GetCurrentTask();
		if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
		{
			bTaskParamAttach = true;
		}

		//排盖模式，以是否任务绑定参数为准，如果已经任务绑定，则排盖模式切换至绑定任务的排盖模式！
		if (bTaskParamAttach)
		{
			m_channelConfig.m_rejectorSetting.m_mode = pCrtTask->m_vecTaskCameraSetting[0].m_delayType;
		}
		else
		{
			m_channelConfig.m_rejectorSetting.m_mode = m_channelConfig.m_vecCameraSetting[0].m_delayType;
		}

		m_chSD.ExecuteCmd(QOP_DEVICE_MODE_SET,DEVICE_OFF);
		m_chSD.ExecuteCmd(QOP_DATA_RESET,RESET_STATS);
	//	m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_COUNT,m_nCameraCount);
		m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_COUNT,1);

        m_channelConfig.m_rejectorSetting.m_mode = m_channelConfig.m_vecCameraSetting[0].m_delayType;
        if(m_channelConfig.m_rejectorSetting.m_mode == DELAY_TIME)
            {
                m_chSD.ExecuteCmd(QOP_PARAM_SET,ENCODER_OFF);
            }
            else
            {
                m_chSD.ExecuteCmd(QOP_PARAM_SET,ENCODER_ON);
            }
            //

		if (m_channelConfig.m_rejectorSetting.m_CameraGrabTime != 0)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMER_GRAB_TIME, m_channelConfig.m_rejectorSetting.m_CameraGrabTime);
		}

// 		if (m_channelConfig.m_rejectorSetting.m_nContinueFrameAlarm != 0)
// 		{
// 			m_chSD.ExecuteCmd(QOP_PARAM_SET,C_FRAME_UCL, m_channelConfig.m_rejectorSetting.m_nContinueFrameAlarm);
// 		}

		if (m_channelConfig.m_rejectorSetting.m_nContinueRejectAlarm != 0)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,C_REJ_UCL, m_channelConfig.m_rejectorSetting.m_nContinueRejectAlarm);
		}

		if (m_channelConfig.m_rejectorSetting.m_SignalIngoreTime != 0)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,SINGLE_INGORE_TIME, m_channelConfig.m_rejectorSetting.m_SignalIngoreTime); //感应器屏蔽时间
		}

		if (m_channelConfig.m_rejectorSetting.m_TriggerIngoreTime != 0)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,TRIGGER_INGORE_TIME, m_channelConfig.m_rejectorSetting.m_TriggerIngoreTime);
		}

		if (m_channelConfig.m_rejectorSetting.m_TriggerLightWidth != 0)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,LIGHT_TRIGGER_WIDTH, m_channelConfig.m_rejectorSetting.m_TriggerLightWidth*1000);
		}


		if(m_channelConfig.m_rejectorSetting.m_mode == DELAY_TIME)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,DELAY_T, m_channelConfig.m_rejectorSetting.m_delay*1000);
		}
		else
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,DELAY_D, m_channelConfig.m_rejectorSetting.m_delay);
		}
		m_chSD.ExecuteCmd(QOP_PARAM_SET,DURATION_T, m_channelConfig.m_rejectorSetting.m_signalWidth*1000);

		if (m_channelConfig.m_rejectorSetting.m_bReRejectFunc)
		{
			if (m_channelConfig.m_rejectorSetting.m_bReReject)
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,R_REJECT_ON);
				if(m_channelConfig.m_rejectorSetting.m_mode == DELAY_TIME)
				{//
					m_chSD.ExecuteCmd(QOP_PARAM_SET,R_SITOSOL_T,m_channelConfig.m_rejectorSetting.m_nReSitoSol*1000);
					m_chSD.ExecuteCmd(QOP_PARAM_SET,R_DELAY_T, m_channelConfig.m_rejectorSetting.m_nReDelay*1000);
				}
				else
				{
					m_chSD.ExecuteCmd(QOP_PARAM_SET,R_SITOSOL_D,m_channelConfig.m_rejectorSetting.m_nReSitoSol);
					m_chSD.ExecuteCmd(QOP_PARAM_SET,R_DELAY_D, m_channelConfig.m_rejectorSetting.m_nReDelay);
				}

				m_chSD.ExecuteCmd(QOP_PARAM_SET,R_DURATION_T, m_channelConfig.m_rejectorSetting.m_nReSignalWidth*1000);
			}
			else
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,R_REJECT_OFF);
			}
		}

		if(m_channelConfig.m_rejectorSetting.m_sa == SA_SWITCH_FOR_EACH)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,SA_SWITCH_EACH);
		}
		if(m_channelConfig.m_rejectorSetting.m_sa == SA_SWITCH_FOR_ALL)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,SA_SWITCH_ONCE_FORALL);
		}
		// Camera setting
        if(m_channelConfig.m_vecCameraSetting.empty())
        {
            return;
        }
		m_chSD.ExecuteCmd(QOP_PARAM_SET,DROP_IMG_UCL,m_channelConfig.m_vecCameraSetting[0].m_nDroppedImgUcl);

		//任务绑定参数下，延时参数取决于有没有任务绑定的参数
		if (bTaskParamAttach)
		{
			if(pCrtTask->m_vecTaskCameraSetting[0].m_delayType == DELAY_TIME)
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_TIME, pCrtTask->m_vecTaskCameraSetting[0].m_delayValue*1000);
			}
			if(pCrtTask->m_vecTaskCameraSetting[0].m_delayType == DELAY_DISTANCE)
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_DISTANCE, pCrtTask->m_vecTaskCameraSetting[0].m_delayValue);
			}
		}
		else
		{
			if(m_channelConfig.m_vecCameraSetting[0].m_delayType == DELAY_TIME)
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_TIME, m_channelConfig.m_vecCameraSetting[0].m_delayValue*1000);
			}
			if(m_channelConfig.m_vecCameraSetting[0].m_delayType == DELAY_DISTANCE)
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_DISTANCE, m_channelConfig.m_vecCameraSetting[0].m_delayValue);
			}
		}

		if(m_channelConfig.m_vecCameraSetting[0].m_bTriggerMode)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_TRIGGER_MODE);
		}
		else
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_FREE_MODE,1000000/m_channelConfig.m_vecCameraSetting[0].m_nFrameRate);
		}
	}
	else
	{
		if (m_subchannel!=0 || m_mainchannel==0)
		{
			CString stremsg;
			stremsg.LoadString(IDS_STRING_ERR_ER401);
			CString strID;
			strID.LoadString(IDS_STRING_ERR_ERRID);
			CString streMsg;
			streMsg.Format(_T("%s%d0%d"),strID,INIT_REJECTOR_SYS_FAILED,ERRORSUBID_CAMERA_RJECT_CONNECT_FAILED);
			streMsg += _T("---")+stremsg+_T("");

			ERROR_INFO emsg(INIT_REJECTOR_SYS_FAILED,ERRORSUBID_CAMERA_RJECT_CONNECT_FAILED,streMsg.GetString());
			AddChError(emsg);
		}
	}
    // System Error Information
//     if(!gSystemMgr.m_pciDevice.GetRejectorStatus())
//     {
//         ERROR_INFO pciErrorInfo = gSystemMgr.m_pciDevice.GetPciErrorInfo();
// 		AddChError(pciErrorInfo);
//     }
    if(!gSystemMgr.IsLcSerialPortOk())
    {
        CString strMsg;
        strMsg.LoadString(IDS_STRING_PLEASE_CHANGE_LC_PORT);
        CString strID;
        strID.LoadString(IDS_STRING_ERR_ERRID);

		CString streMsg;
		streMsg.Format(_T("%s%d0%d"),strID,INIT_REJECTOR_SYS_FAILED,ERRORSUBID_LIGHT_COM_FAILED);
		streMsg += _T("---")+strMsg+_T("");

        ERROR_INFO lcPortError(LIGHT_CONTRL_CONNECT_ERROR,ERRORSUBID_LIGHT_COM_FAILED,streMsg.GetString());
		AddChError(lcPortError);
    }
}

CChannelControlPanel::~CChannelControlPanel()
{
	m_SaveImage.setEnable(false);
	for(auto it= m_vChCameraPtr.begin(); it != m_vChCameraPtr.end();it++)
	{
		CBaslerCamera* p  = *it;
		delete p;
	}
	m_vChCameraPtr.clear();

	for(auto it= m_vProcessorPtr.begin(); it != m_vProcessorPtr.end();it++)
	{
		CImageProcessor* p  = *it;
		delete p;
	}
	m_vProcessorPtr.clear();

	m_chartCtrl.DestroyWindow();
	::DeleteCriticalSection(&m_paramDataCs);
	::DeleteCriticalSection(&m_spcDataCs);
	::DeleteCriticalSection(&m_spcDataCs2);
	::DeleteCriticalSection(&m_deviceDataCs);
	::DeleteCriticalSection(&m_imgArrayCs);
}

void CChannelControlPanel::AddChError(ERROR_INFO &error)
{
	bool bNoError= true;
	for (auto it = m_vChError.begin(); it != m_vChError.end();it++)
	{
		if(it->m_errorID == error.m_errorID && it->m_errorSubID == error.m_errorSubID)
		{
			it->m_Counter++;
			it->m_strMsg = error.m_strMsg;
			bNoError = false;
			break;
		}
	}

	if (bNoError)
	{
		error.m_nChIndex = m_chIndex;
		CString streMsg;
		streMsg.Format(_T("%d0%d"),error.m_errorID,error.m_errorSubID);
		//error.m_level = gSystemMgr.GetErrorLevel(streMsg.GetString());
		if(error.m_errorID==LIGHT_CONTRL_CONNECT_ERROR && error.m_errorSubID==ERRORSUBID_LIGHT_CONNECT_FAILED)
			error.m_level=WL_NONE;
		else
			error.m_level = gSystemMgr.GetErrorLevel(streMsg.GetString());
		m_vChError.push_back(error);
	}

}

void CChannelControlPanel::DelChError(int errorID)
{
	for (auto it = m_vChError.begin(); it != m_vChError.end(); it++ )
	{
		if (it->m_errorID == errorID)
		{
			m_vChError.erase(it);
			return;
		}
	}
}

bool CChannelControlPanel::SetPanelLocation(int nChIndex, CRect& rcPanel)
{
	CRect rcScreen;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcScreen,0);
	int nChCount = gConfigMgr.GetChannelCount();
	if(nChCount <= 0)
	{
		return false;
	}
	int nScreenHeight = rcScreen.Height();

	float fRate = 1080.0f / rcScreen.Height();
	int nTitleHeight  = int(72.0f / fRate) - 2;
	int nTaskBarHeight = int(45.0f / fRate);
	if(nTaskBarHeight > 34)
	{
		nTaskBarHeight = 34;
	}

	int panelHeight = 190;
	if(nScreenHeight <= 1080)
	{
		if(nChCount > 8)// Message box here
		{
			return false;
		}
		panelHeight =  int((nScreenHeight - nTitleHeight - nTaskBarHeight) * 1.0 / nChCount);
		if(panelHeight > 200)
		{
			panelHeight = 200;
		}
		m_panelH = panelHeight;          // Panel Height
		m_panelW = rcScreen.Width() - 4; // Panel Width

		rcPanel.left   = 2;
		rcPanel.top    = nTitleHeight + (nChIndex - 1) * m_panelH;
		rcPanel.right  = rcPanel.left + m_panelW;
		rcPanel.bottom = rcPanel.top + m_panelH;
	}
	if(nScreenHeight > 1200)
	{
		if(nChCount >= 16)
		{
			return false;
		}
		if(nChCount <= 8)
		{
			panelHeight =  int((nScreenHeight - nTitleHeight - nTaskBarHeight) * 1.0 / nChCount);
			if(panelHeight > 240)
			{
				panelHeight = 240;
			}
			m_panelH = panelHeight; // Panel Height
			m_panelW = rcScreen.Width() - 4;

			rcPanel.left   = 2;
			rcPanel.top    = nTitleHeight + (nChIndex - 1) * m_panelH;
			rcPanel.right  = rcPanel.left + m_panelW;
			rcPanel.bottom = rcPanel.top + m_panelH;
		}
		else
		{
			int nTmpCount = (int)::ceill(nChCount / 2.0);
			panelHeight =  int((nScreenHeight - nTitleHeight - nTaskBarHeight) * 1.0 / nTmpCount);
			if(panelHeight > 240)
			{
				panelHeight = 240;
			}
		    m_panelH = panelHeight; // Panel Height
			m_panelW = 1080 - 8;

			rcPanel.left   = 2 + (1 - (nChIndex % 2)) * 1080;
			rcPanel.top    = nTitleHeight + ::floorl((nChIndex - 1) / 2.0) * m_panelH;
			rcPanel.right  = rcPanel.left + m_panelW;
			rcPanel.bottom = rcPanel.top + m_panelH;
		}
	}

	return true;
}

int CChannelControlPanel::Init(CHANNEL_CONFIG& chConfig)
{
	m_channelState = CH_OFF;
	if(!SetPanelLocation(chConfig.m_chIndex,m_rcPanel))
	{
		return -1;
	}

	wstring strIndex = boost::lexical_cast<wstring>(m_chIndex);
	Create(_T(""),WS_CHILD|WS_VISIBLE|BS_GROUPBOX, m_rcPanel, m_pParentWnd, 331);
	SetOwner(m_pParentWnd);
	SetFont(&m_font);

	CRect rcClient;
	GetClientRect(&rcClient);
	int gapW = 4;
	int widthAvailabel  = rcClient.Width() - 2*gapW;
	int nBtnColumnWidth = (int)3*widthAvailabel/28.0 - 35;
	

	int left = m_rcPanel.left + 2;
	int top = rcClient.top + 12;
	int right = left + nBtnColumnWidth;
	if(nBtnColumnWidth > 180)
	{
		right = left + 180;
	}
	int bottom = rcClient.bottom - 4;
	int canvasH = bottom - top;
	// Column 1 -- Operation Panel
	CRect rcColumn1(left,top, right,bottom);
	m_Rect1 = rcColumn1;
	m_Rect1.top +=1;
	m_Rect1.left +=1;
	CreateControlPanel(rcColumn1);

	// Column 2
	//主界面保证显示区域一
	//double ratio = (double)m_channelConfig.m_uiSetting.m_height / (double)m_channelConfig.m_uiSetting.m_width;
	double ratio = (double)494/659;
	left = rcColumn1.right + gapW;
	right = left + int(canvasH / ratio);
	CRect rcColumn2(left,top,right,bottom);
	CreateCurrentImageDispPanel(rcColumn2);

	// Column 3 -- button Column
	left = rcColumn2.right + gapW;
	right = left + nBtnColumnWidth*1.3f;
	CRect rcCloumn3(left, top, right, bottom);
	CreateButtonPanel(rcCloumn3);

	// Column 4 -- Statistic Column
	left = rcCloumn3.right + gapW;
	right = left + nBtnColumnWidth;
	if(nBtnColumnWidth > 200)
	{
		right = left + 200;
	}
	CRect rcColumn3(left,top,right, bottom);
	CreateStatisticPanel(rcColumn3);

	// Column 5 -- Chart
	left = rcColumn3.right + gapW;
	right = m_rcPanel.right - /*rcColumn2.Width() - 3 * gapW-*/2;
	CRect rcColumn4(left,top,right,bottom);
	CreateChartCtrlPanel(rcColumn4);

	m_SaveImageType=0;
	if(chConfig.m_SaveImageType==SAVE_NG_IMAGE || chConfig.m_SaveImageType==SAVE_OK_IMAGE)
	{
		m_SaveImage.setEnable(true);
		CString StrChannel;
		StrChannel.Format(L"\\CH_%d",m_chIndex);
		std::wstring wPath =gConfigMgr.GetBasicSetting(L"IMAGE_SAVE_PATH");
		if(wPath==L"")
			m_SaveImage.setSavePath(L"c:\\ChannelImages");
		else
			m_SaveImage.setSavePath(wPath + StrChannel.AllocSysString());
		CString StrPrefix;
		if(chConfig.m_SaveImageType==SAVE_NG_IMAGE)
			StrPrefix.Format(L"CH_%d_NG",m_chIndex);
		else if(chConfig.m_SaveImageType==SAVE_OK_IMAGE)
			StrPrefix.Format(L"CH_%d_OK",m_chIndex);
		else
			StrPrefix.Format(L"CH_%d",m_chIndex);
		m_SaveImage.setPrefix(StrPrefix.GetString());
		m_SaveImage.start();
		m_SaveImageType = chConfig.m_SaveImageType;
	}
	else
	{
		m_SaveImage.setEnable(false);
	}

	return 0;
}

void CChannelControlPanel::AddImage(Hobject obj,bool bOK)
{
	if(m_SaveImage.getEnable())
	{
		if(m_channelConfig.m_SaveImageType==SAVE_NG_IMAGE && !bOK)
			m_SaveImage.pushBack(obj);
		else if (m_channelConfig.m_SaveImageType==SAVE_OK_IMAGE && bOK)
			m_SaveImage.pushBack(obj);
	}
}

void CChannelControlPanel::ListAlgoGroup()
{
	int index = -1;
	m_nTaskComboIndex = 0;
	m_comboChTask.ResetContent();
	for(size_t t = 0; t < m_channelConfig.m_vTask.size();t++)
	{
		TASK task = m_channelConfig.m_vTask[t];
		m_comboChTask.AddString(task.m_wstrTaskName.c_str());
		index++;
		if(m_channelConfig.m_crtTaskName == task.m_wstrTaskName)
		{
			m_nTaskComboIndex = index;
		}
	}
	m_comboChTask.SetCurSel(m_nTaskComboIndex);
	CString sRet;
	m_comboChTask.GetLBText(m_nTaskComboIndex, sRet);
	m_channelConfig.m_crtTaskName = CT2W(sRet);
	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	(*pChConfigArray)[m_channelConfig.m_chIndex].m_crtTaskName = m_channelConfig.m_crtTaskName;

	m_comboChTask.Invalidate();
}

void CChannelControlPanel::ApplyNewChannelSetting(bool bHardware)
{
	int chIndex         = m_channelConfig.m_chIndex;
	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr(true);
	if(pChConfigArray != nullptr)
	{
		m_channelConfig = (*pChConfigArray)[chIndex];
		if(!bHardware)
		{
			ListAlgoGroup();
		}
		else
		{
			if(!IsChCameraRunning())
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,TEST_LC);
				m_chSD.ExecuteCmd(QOP_PARAM_SET,TEST_SOLENOID);
				TurnOnCameraAtSystemBoot();
			}
		}
		SetChannelStatus();
		std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
		{
			pProcessor->SetChConfigPtr(&m_channelConfig);
		});
	}
}

void CChannelControlPanel::ApplyNewCameraSetting(const CAMERA_SETTING& cameraSetting,bool bTaskParamAttach)
{
	wstring wstrErr;
	for(auto t = 0; t < m_channelConfig.m_vecCameraSetting.size();t++)
	{
		if(m_channelConfig.m_vecCameraSetting[t].m_name == cameraSetting.m_name)
		{
			if(m_vChCameraPtr.size()< t+1 )
			{
				CBaslerCamera* pCamera = new CBaslerCamera;
				pCamera->SetGroupIndex(t);
				pCamera->SetGroupFlag(m_channelConfig.m_vecCameraSetting.size()>1?TRUE:FALSE);
				pCamera->SetRejectionMode(m_channelConfig.m_vecCameraSetting[t].m_delayType);
				if (m_subchannel!=0)
				{
					pCamera->SetCameraAcquireMode(true);
				}
				wostringstream woss;
				woss << L"CH" << m_channelConfig.m_chIndex << L"_" << (t + 1);
				CLogger tmpLogger(woss.str());
				m_vChLogger.push_back(tmpLogger);
				m_vChCameraPtr.push_back(pCamera);
			}
			m_vChCameraPtr[t]->ApplyNewSetting(cameraSetting,wstrErr,bTaskParamAttach);
			m_channelConfig.m_vecCameraSetting[t] = cameraSetting;
		}
	}
}

void CChannelControlPanel::UpdateCameraSetting(const CAMERA_SETTING& cameraSetting,bool bCameraModeChange)
{
    if(!m_channelConfig.m_vecCameraSetting.empty())
    {
        bool bResetStatistic = (m_channelConfig.m_vecCameraSetting[0].m_bTriggerMode != cameraSetting.m_bTriggerMode);
        if(bResetStatistic)
        {
            std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
            {
                pProcessor->ChangeImageGrabbingMode();
            });
            ResetInternalStats();
        }
    }

	if(cameraSetting.m_bTriggerMode)
	{
		if (IsChCameraRunning())
		{

			m_chSD.ExecuteCmd(QOP_DEVICE_MODE_SET,DEVICE_OFF);
			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_TRIGGER_MODE);
			m_chSD.SetCLientTimeSyn();
			m_chSD.ExecuteCmd(QOP_DEVICE_MODE_SET,DEVICE_ON);
		}
		else
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_TRIGGER_MODE);
		}
	}
	else
	{
		if (IsChCameraRunning())
		{
			m_chSD.ExecuteCmd(QOP_DEVICE_MODE_SET,DEVICE_OFF);
			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_FREE_MODE,1000000/cameraSetting.m_nFrameRate);
			m_chSD.SetCLientTimeSyn();
			m_chSD.ExecuteCmd(QOP_DEVICE_MODE_SET,DEVICE_ON);
		}
		else
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_FREE_MODE,1000000/cameraSetting.m_nFrameRate);
		}
	}

		if(cameraSetting.m_delayType == DELAY_TIME)
		{
			if (bCameraModeChange)
			{

		//		MessageBox(_T("ENCODER_OFF"));
				m_chSD.ExecuteCmd(QOP_PARAM_SET,ENCODER_OFF);
			}

			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_TIME,cameraSetting.m_delayValue*1000);
		}
		else
		{
			if (bCameraModeChange)
			{
	//			MessageBox(_T("ENCODER_ON"));
				m_chSD.ExecuteCmd(QOP_PARAM_SET,ENCODER_ON);
			}
			m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_DISTANCE,cameraSetting.m_delayValue);
	}
	ApplyNewCameraSetting(cameraSetting);
}

void CChannelControlPanel::UpdateLightSetting(const wstring& wstrGpName, const LIGHT_SETTING& lightSetting)
{
	m_channelConfig.m_lightCtrlSetting = lightSetting;
}

void CChannelControlPanel::UpdateChLightController(const LIGHT_SETTING& ls)
{
	std::vector<int> vID = ls.m_vLcID;
	// 	if(vID.empty())
	// 	{
	// 		return;
	// 	}
	m_channelConfig.m_lightCtrlSetting = ls;
	m_channelConfig.m_lightCtrlSetting.m_vLcID.clear();
	m_channelConfig.m_lightCtrlSetting.m_vLcID = vID;
}

void CChannelControlPanel::UpdateRejectorSetting(const REJECTOR_SETTING& rejectorSetting)
{
	//更新取反信号点 （无此函数时取反信号点无法发即时生效）：
	if (m_vChCameraPtr.size()==1)
	{
		m_vChCameraPtr[0]->SetDroppedImageInspectionPort(rejectorSetting.m_nDropImgPort);
	}

	m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMER_GRAB_TIME, rejectorSetting.m_CameraGrabTime);
	m_chSD.ExecuteCmd(QOP_PARAM_SET,TRIGGER_INGORE_TIME, rejectorSetting.m_TriggerIngoreTime);

	//m_chSD.ExecuteCmd(QOP_PARAM_SET,C_FRAME_UCL, rejectorSetting.m_nContinueFrameAlarm);
	m_chSD.ExecuteCmd(QOP_PARAM_SET,C_REJ_UCL, rejectorSetting.m_nContinueRejectAlarm);

    if (rejectorSetting.m_mode == DELAY_TIME)
    {
        m_chSD.ExecuteCmd(QOP_PARAM_SET,DELAY_T, rejectorSetting.m_delay*1000);
    }
    else
    {
        m_chSD.ExecuteCmd(QOP_PARAM_SET,DELAY_D, rejectorSetting.m_delay);
    }

     m_chSD.ExecuteCmd(QOP_PARAM_SET,DURATION_T, rejectorSetting.m_signalWidth*1000);//

	if (rejectorSetting.m_bReRejectFunc)
	{
		if (rejectorSetting.m_bReReject)
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,R_REJECT_ON);

			if(rejectorSetting.m_mode == DELAY_TIME)
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,R_SITOSOL_T,rejectorSetting.m_nReSitoSol*1000);
				m_chSD.ExecuteCmd(QOP_PARAM_SET,R_DELAY_T, rejectorSetting.m_nReDelay*1000);

			}
			else
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,R_SITOSOL_D,rejectorSetting.m_nReSitoSol);
				m_chSD.ExecuteCmd(QOP_PARAM_SET,R_DELAY_D, rejectorSetting.m_nReDelay);
			}

			m_chSD.ExecuteCmd(QOP_PARAM_SET,R_DURATION_T, rejectorSetting.m_nReSignalWidth*1000);
		}
		else
		{
			m_chSD.ExecuteCmd(QOP_PARAM_SET,R_REJECT_OFF);
		}
	}

//二次排盖不启用的状态不发送二次排盖禁用指令
// 	else
// 	{
// 		m_chSD.ExecuteCmd(QOP_PARAM_SET,R_REJECT_OFF);
// 	}


	if(rejectorSetting.m_sa == SA_SWITCH_FOR_EACH)
	{
		m_chSD.ExecuteCmd(QOP_PARAM_SET,SA_SWITCH_EACH);
	}
	if(rejectorSetting.m_sa == SA_SWITCH_FOR_ALL)
	{
		m_chSD.ExecuteCmd(QOP_PARAM_SET,SA_SWITCH_ONCE_FORALL);
	}
	m_channelConfig.m_rejectorSetting = rejectorSetting;
	std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
	{
		pProcessor->SetChConfigPtr(&m_channelConfig);
	});
}

BEGIN_MESSAGE_MAP(CChannelControlPanel, CButton)
	ON_STN_DBLCLK(IDB_CH_NICKNAME,      &CChannelControlPanel::OnStnDblClickedNickName)
	ON_STN_CLICKED(IDB_ON_OFF_BTN,      &CChannelControlPanel::OnStnClickedOnOff_EX)
	ON_COMMAND(ID_Menu_cleardata,    &CChannelControlPanel::OnStnClickedDataClear)
	ON_STN_CLICKED(IDB_BTN_WARNING_SHOW,      &CChannelControlPanel::OnStnClickedEventFlag)
	ON_CBN_SELCHANGE(IDC_COMBO_ALGO_GP, &CChannelControlPanel::OnCbnSelchangeAlgoGroup)
	ON_CBN_SELCHANGE(IDB_COMBO_STAY_TIME, &CChannelControlPanel::OnCbnSelchangeStayTime)
	ON_MESSAGE(MSG_MODEL_IMG_READY,          OnSelfLearningComplete)
	ON_MESSAGE(MSG_CH_STATION_STATE_CHANGED, OnChannelStatusChanged)
END_MESSAGE_MAP()
	


void CChannelControlPanel::CreateControlPanel(CRect& rcColumn)
{
	m_backgroundCtrl1.Create(_T(""),WS_CHILD|WS_VISIBLE, rcColumn, this);
	m_backgroundCtrl1.SetColor(m_fgColor,m_bgColor);
	m_backgroundCtrl1.Invalidate();

	CRect rcIndex(15,16,35,32);
	CString sChIndex;
	sChIndex.Format(_T("CH%d"),m_chIndex); 
	m_chIndexPanel.Create(sChIndex,WS_CHILD|WS_VISIBLE,rcIndex,this, NULL);
	m_chIndexPanel.SetTxtFont(m_valueFontSz, FW_BOLD);
	m_chIndexPanel.SetColor(m_fgColor,m_bgColor);
	m_chIndexPanel.Invalidate();

	CRect rcNickname;
	rcNickname.top   = rcIndex.top;
	rcNickname.left  = rcIndex.right + 20;
	rcNickname.right = rcColumn.right;
	rcNickname.bottom = rcIndex.bottom;
	
	m_laneNamePanel.Create(m_channelConfig.m_wstrLaneName.c_str(),WS_CHILD|WS_VISIBLE|SS_NOTIFY,rcNickname,this, IDB_CH_NICKNAME);
	m_laneNamePanel.SetTxtFont(m_valueFontSz, FW_BOLD, LEFT);
	m_laneNamePanel.SetColor(m_fgColor,m_bgColor);
	m_laneNamePanel.Invalidate();
	
	int margin = 16;
	int w = rcColumn.Width() - 2 * margin; 
	int h = rcColumn.Height() - 2 * margin;
	int sz = min(w,h);



	CRect rcDC1Btn;

	rcDC1Btn.right = rcColumn.right-2;
	rcDC1Btn.left = rcColumn.right - 32-2;
	rcDC1Btn.bottom = rcColumn.bottom-2;
	rcDC1Btn.top = rcColumn.bottom - 32 -2;

	m_cLabWarnShow.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY,rcDC1Btn, this, IDB_BTN_WARNING_SHOW);
	m_cLabWarnShow.EnableWindow(TRUE);
	m_cLabWarnShow.SetLabelImage(IMG_WARN_NORMAL);
	m_cLabWarnShow.Invalidate();

	CRect rcSpeed;
	rcSpeed.left = rcColumn.left + 5;
	rcSpeed.top  = rcColumn.bottom - 20;
	rcSpeed.right = rcDC1Btn.left - 20;
	rcSpeed.bottom = rcColumn.bottom-3;

	m_chSpeedPanel.Create(_T(""),WS_CHILD|WS_VISIBLE, rcSpeed, this, NULL);
	m_chSpeedPanel.SetTxtFont(m_valueFontSz, FW_BOLD, LEFT);
	m_chSpeedPanel.SetColor(m_fgColor,m_bgColor);
	m_chSpeedPanel.Invalidate();


	int l = rcColumn.left + (int)((rcColumn.Width() - sz) / 2.0f);
	int t = rcColumn.top + (int)((rcColumn.Height() - sz) / 2.0f)+ 2;
	int r = l + sz;
	int b = t + sz;
	CRect rcBtn(l,t,r,b);
	m_OnOffBtn.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY,rcBtn, this, IDB_ON_OFF_BTN);
	m_OnOffBtn.SetLabelImage(IMG_STOP);
	m_OnOffBtn.EnableWindow(TRUE);
	m_OnOffBtn.Invalidate();
}

void CChannelControlPanel::CreateCurrentImageDispPanel(CRect& rcColumn)
{
	m_currentImgWnd.Create(_T(""),WS_CHILD|WS_VISIBLE, rcColumn, this);
	m_currentImgWnd.SetColor(m_fgColor,m_bgColor);
	m_currentImgWnd.Invalidate();
	CreateHalconImageWnd(rcColumn,m_currentImgWnd.m_hWnd,m_currentImgWndID);
	gSystemMgr.m_displyManager.RegisterRealTimeImgWindowID(m_chIndex,m_currentImgWndID);
}

void CChannelControlPanel::CreateButtonPanel(CRect& rcColumn)
{
	m_backgroundCtrl3.Create(_T(""),WS_CHILD|WS_VISIBLE, rcColumn, this);
	m_backgroundCtrl3.SetColor(m_fgColor,m_bgColor);
	m_backgroundCtrl3.Invalidate();

	int l = rcColumn.left + 8;
	int t = rcColumn.top + 5;
	int r = l + rcColumn.Width() - 16;
	int b = t + int((float)rcColumn.Width()/ 3) + 30;
	CRect rcCombo(l,t,r,b);
	m_comboChTask.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL, rcCombo, this, IDC_COMBO_ALGO_GP);
	m_comboChTask.SetColor(m_fgColor,m_bgColor);
	m_comboChTask.SetFont(&m_font);
	m_comboChTask.SetCurSel(-1);
	m_comboChTask.SetDroppedWidth(rcCombo.Width()*1.3f);
	ListAlgoGroup();

	/////////////////// hzh /////////////////////////////
	CRect rcSt;
	//rcSt.top = rcCombo.bottom+5;
	rcSt.top = rcCombo.top+25;
	rcSt.left = rcCombo.left;
	rcSt.right = rcCombo.left+(rcCombo.right-rcCombo.left)/1.5;
	rcSt.bottom = rcSt.top+20;
	CString StrCaption;
	StrCaption.LoadString(IDS_STRING_BAD_PICTURE_STAY_TIME);
	m_lbStayTime.Create(StrCaption,WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY,rcSt,this,0);
	m_lbStayTime.SetTxtFont(14,FW_BOLD,CENTER);
	m_lbStayTime.SetColor(RGB(10,10,115), RGB(200,220,250));
	m_lbStayTime.Invalidate();
	rcSt.left = rcSt.right+2;
	rcSt.right=rcCombo.right;
	m_comboStayTime.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL, rcSt, this, IDB_COMBO_STAY_TIME);
	m_comboStayTime.AddString(_T("0"));
	m_comboStayTime.AddString(_T("1"));
	m_comboStayTime.AddString(_T("3"));
	m_comboStayTime.AddString(_T("5"));
	m_comboStayTime.AddString(_T("10"));
	m_comboStayTime.AddString(_T("15"));
	m_comboStayTime.AddString(_T("20"));
	m_comboStayTime.SetCurSel(0);
	/////////////////////////////////////////////////
	CRect rc;
	int nBtnHeight = (int)(2*rcColumn.Height()/3.0f - 10)/2.0f;
	m_comboStayTime.GetWindowRect(rc);
	int nGap = 10;
	l = rcCombo.left + 5;
	r = rcCombo.right - 5;
	t = rcColumn.top + rcColumn.Height()/3.0f  + 10;
	b = t + nBtnHeight-5;
	if(b - t > 40)
	{
		b = t + 40;
	}
	CRect rcParam(l,t,r,b);
	CString strConfig;
	strConfig.LoadString(IDS_STRING_CONFIG);
	m_btnChDebug.Create(strConfig,WS_CHILD|WS_VISIBLE, rcParam, this, NULL);
	m_btnChDebug.SetFont(&m_btnTextFont);
	m_btnChDebug.SetButtonType(BT_PARAM);
	m_btnChDebug.SetChCtrlPanelPtr(this);
	m_btnChDebug.Invalidate();
	m_btnChDebug.EnableWindow(FALSE);

	// SPC Button
	
	t = b + 5;
	b = t + nBtnHeight-5;
	if(b - t > 40)
	{
		t = b - 40;
	}
	CRect rcSpc(l,t,r,b);
	CString strSPC;
	strSPC.LoadString(IDS_STRING_SPC_BTN);
	m_spcBtn.Create(strSPC,WS_CHILD|WS_VISIBLE, rcSpc, this, NULL);
	m_spcBtn.SetFont(&m_btnTextFont);
	m_spcBtn.SetButtonType(BT_SPC);
	m_spcBtn.SetChCtrlPanelPtr(this);
	m_spcBtn.Invalidate();
}

void CChannelControlPanel::CreateStatisticPanel(CRect& rcColumn)
{
	m_backgroundCtrl4.Create(_T(""),WS_CHILD|WS_VISIBLE,rcColumn,this);
	m_backgroundCtrl4.SetColor(m_fgColor,m_bgColor);
	m_backgroundCtrl4.Invalidate();
	//
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	float rate = rect.Height() /1080.0;
	if(rate > 1.0f)
		rate = 1.0f;
	int tmpH = int(22 * rate);

	int nVGap  = 4;
	int nDelta = (int)(rcColumn.Height() - 4 * tmpH - 3*nVGap) / 2.0f;

	int l = rcColumn.left + 6;
	int t = rcColumn.top + nDelta;
	int r = l + rcColumn.Width() - 12;
	int b = t + tmpH;
	CRect rcTotalT(l,t,r,b);
	CString strTotal;
	strTotal.LoadString(IDS_STRING_TOTAL);
	m_totalText.Create(strTotal,WS_CHILD|WS_VISIBLE,rcTotalT,this, NULL);
	m_totalText.SetTxtFont(m_labelFontSz, FW_BOLD);
	m_totalText.SetColor(m_fgColor,m_bgColor);
	m_totalText.Invalidate();
	// Total Value 
	t = rcTotalT.bottom + nVGap;
	b = t + tmpH;
	CRect rcTotalV(l,t,r,b);
	m_totalValue.Create(_T(""),WS_CHILD|WS_VISIBLE, rcTotalV, this, NULL);
	m_totalValue.SetTxtFont(m_valueFontSz, FW_BOLD, LEFT);
	m_totalValue.SetColor(RGB(0,0,0),RGB(255,255,255));
	m_totalValue.Invalidate();
	// Defectie Text
	t = rcTotalV.bottom + nVGap;
	b = t + tmpH;
	CRect rcFailedT(l,t,r,b);
	CString strFailed;
	strFailed.LoadString(IDS_STRING_FAILED);
	m_failedText.Create(strFailed, WS_CHILD|WS_VISIBLE, rcFailedT, this, NULL);
	m_failedText.SetTxtFont(m_labelFontSz, FW_BOLD);
	m_failedText.SetColor(m_fgColor, m_bgColor);
	m_failedText.Invalidate();
	// Defective Value
	t = rcFailedT.bottom + nVGap;
	b = t + tmpH;
	CRect rcFailedV(l,t,r,b);
	m_failedValue.Create(_T(""),WS_CHILD|WS_VISIBLE, rcFailedV, this, NULL);
	m_failedValue.SetTxtFont(m_valueFontSz, FW_BOLD, LEFT);
	m_failedValue.SetColor(RGB(0,0,0),RGB(255,255,255));
	m_failedValue.Invalidate();

	CRect rcDC2Btn;

	rcDC2Btn.right = rcColumn.right -2;
	rcDC2Btn.left = rcDC2Btn.right -38;
	rcDC2Btn.top = rcColumn.top;
	rcDC2Btn.bottom = rcDC2Btn.top + 27;


	m_DataClearBtn.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY,rcDC2Btn, this, IDB_DATA_CLEAR_BTN+m_chIndex+1);
	m_DataClearBtn.SubclassDlgItem(IDB_DATA_CLEAR_BTN,this);
	m_DataClearBtn.MoveWindow(rcDC2Btn);
	m_DataClearBtn.SetFlat(TRUE);
	m_DataClearBtn.SetBitmaps(IDB_BITMAP_CH_CONFIG2,RGB(0,0,0),IDB_BITMAP_CH_CONFIG1,RGB(0,0,0));
	m_DataClearBtn.DrawTransparent(TRUE);
	m_DataClearBtn.SetMenu(IDR_MENU_CH_SET, m_hWnd);
	m_DataClearBtn.EnableWindow(TRUE);

	UAC crtUser = gSystemMgr.GetCurrentUser();

	if(crtUser.m_nLevel == UAC_ADMIN)
	{
		m_DataClearBtn.ShowWindow(SW_SHOW);
	}
	else
	{
		m_DataClearBtn.ShowWindow(SW_HIDE);
	}
}

void CChannelControlPanel::CreateChartCtrlPanel(CRect& rcChart)
{
	m_chartCtrl.Create(NULL,NULL,NULL, rcChart, this,123);
	m_chartCtrl.CreateGraph(GT_2DLINE, &m_channelConfig.m_spcSetting);
	m_chartCtrl.SetChIndex(m_channelConfig.m_chIndex);
	m_chartCtrl.SetGraphBackgroundColor(RGB(200,220,250));
	CString title;
	title = _T("");
	m_chartCtrl.SetGraphTitle(title);
	m_chartCtrl.SetGraphTitleColor(RGB(255,0,0));

	m_chartCtrl.SetGraphSubtitle(_T(""));
	m_chartCtrl.SetGraphTitleShadow(FALSE);
	m_chartCtrl.SetGraphSubtitleShadow(FALSE);

	m_chartCtrl.Receive2DLineGraphData(m_statisticsBuffer);
	m_chartCtrl.Invalidate();
}

bool CChannelControlPanel::CreateHalconImageWnd(CRect& rcWnd,HWND& wnd, long long& wndID)
{
	try
	{
		Hobject  initIimage;
		int height = rcWnd.Height();
		int width = rcWnd.Width();
		std::vector<unsigned char>  image(height * width, 0);
		for(int r = 0; r < height; r++)
		{
			for(int c = 0; c < width; c++)
				image[r * width + c] = 50 + r % 32 + c % 16;
		}
		CSyncLock l;
		gen_image1(&initIimage,"byte",width,height,(Hlong)&image[0]);
		if(wndID == 0)
		{
			set_check("~father");
			std::string mode("visible");
			std::string strMachine("");
			open_window(0,0,width, height,(Hlong)wnd,mode.c_str(),strMachine.c_str(), (Hlong*)&wndID);
			set_check("father");
		}
		set_draw(wndID,"margin"); //margin or fill
		set_color(wndID, "red");
		set_part(wndID,0, 0, height, width);
		disp_obj(initIimage, wndID);
		set_line_width(wndID,HTuple(1));
	}
	catch (HException &except) 
	{
		string msg;
		msg = except.line;
		msg+=except.message;
		return false;
	}
	return true;
}

void CChannelControlPanel::StartImgProcessor(bool bCameraSetChanged)
{
	//如果有参数设置,则重新
	if (bCameraSetChanged)
	{
		TASK* pCrtTask = m_channelConfig.GetCurrentTask();
		if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
		{
			for(size_t t = 0; t < m_vChCameraPtr.size(); t++)
			{
				wstring str;
				CBaslerCamera* pCamera      = m_vChCameraPtr[t];
				if (t < pCrtTask->m_vecTaskCameraSetting.size())
				{
					pCamera->ResetCameraParameters(pCrtTask->m_vecTaskCameraSetting[t],str);
				}
			}
			if(pCrtTask->m_vecTaskCameraSetting[0].m_delayType == DELAY_TIME)
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_TIME, pCrtTask->m_vecTaskCameraSetting[0].m_delayValue*1000);
			}
			if(pCrtTask->m_vecTaskCameraSetting[0].m_delayType == DELAY_DISTANCE)
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_DISTANCE, pCrtTask->m_vecTaskCameraSetting[0].m_delayValue);
			}
		}
		else
		{
			for(size_t t = 0; t < m_vChCameraPtr.size(); t++)
			{
				wstring str;
				CBaslerCamera* pCamera      = m_vChCameraPtr[t];
				if ( t<m_channelConfig.m_vecCameraSetting.size())
				{
					pCamera->ResetCameraParameters(m_channelConfig.m_vecCameraSetting[t],str);
				}
			}

            if(!m_channelConfig.m_vecCameraSetting.empty())
            {
                if(m_channelConfig.m_vecCameraSetting[0].m_delayType == DELAY_TIME)
                {
                    m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_TIME, m_channelConfig.m_vecCameraSetting[0].m_delayValue*1000);
                }
                if(m_channelConfig.m_vecCameraSetting[0].m_delayType == DELAY_DISTANCE)
                {
                    m_chSD.ExecuteCmd(QOP_PARAM_SET,CAMERA_DELAY_DISTANCE, m_channelConfig.m_vecCameraSetting[0].m_delayValue);
                }
            }
		}
	}
	
	

	if(m_channelConfig.m_rejectorSetting.m_bTestRejector)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_REJECTOR_IN_TEST_MODE);
		CString strIndex;
		strIndex.Format(_T("%d"), m_channelConfig.m_chIndex);
		sMsg.Replace(_T("#1"), strIndex);
		if(IDNO == AfxMessageBox(sMsg, MB_YESNO, NULL))
		{ 
			m_channelConfig.m_rejectorSetting.m_bTestRejector = false;
			gSystemMgr.UpdateRejectorSetting(m_channelConfig.m_chIndex,m_channelConfig.m_rejectorSetting);
			gConfigMgr.UpdateRejectorSetting(m_channelConfig.m_chIndex,&(m_channelConfig.m_rejectorSetting));
		}
	}
	m_vFuncInfo.clear();
	m_vFuncInfoDebug.clear();

	std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
	{
		if (m_channelConfig.GetCurrentTask()->m_bSelfLearning)
		{
			ExecuteCmd(QOP_PARAM_SET,SELF_LEARN_FUNC,1);
		}
		else
		{
			ExecuteCmd(QOP_PARAM_SET,SELF_LEARN_FUNC,0);
		}
		pProcessor->SetChConfigPtr(&m_channelConfig);
		pProcessor->Start();
	});

	ResetInternalStats();
}

CAMERA_SETTING CChannelControlPanel::GetNewCameraSetting(int t)
{
	CAMERA_SETTING CameraSet = m_channelConfig.m_vecCameraSetting[t];

	TASK* pCrtTask = m_channelConfig.GetCurrentTask();

	if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
	{
		CameraSet.m_delayType = pCrtTask->m_vecTaskCameraSetting[t].m_delayType;
		CameraSet.m_delayValue = pCrtTask->m_vecTaskCameraSetting[t].m_delayValue;
		CameraSet.m_ExposureTime = pCrtTask->m_vecTaskCameraSetting[t].m_ExposureTime;
		CameraSet.m_gain = pCrtTask->m_vecTaskCameraSetting[t].m_gain;
	}

	return CameraSet;
}

void CChannelControlPanel::SetBatchNumber(CString batchnumberstr)
{
	m_strbatchnumber = batchnumberstr;
}

void CChannelControlPanel::CollectChannelError(std::vector<ERROR_INFO>& vError)
{
    if(!vError.empty())
    {
		for (auto it = vError.begin(); it != vError.end();it++)
		{
			AddChError(*it);
		}
    }
}

bool CChannelControlPanel::TurnOnCameraAtSystemBoot()
{
	int nCameraIndex = 0;
	CImageProcessor* pProcessor = nullptr;
	if(m_vChCameraPtr.size() > 1 && m_channelConfig.m_bSingleProcessor)
	{
		pProcessor = m_vProcessorPtr[0];
	}
	for(size_t t = 0; t < m_vChCameraPtr.size(); t++)
	{
		CBaslerCamera* pCamera      = m_vChCameraPtr[t];
		if(pProcessor == nullptr)
		{
			pProcessor = m_vProcessorPtr[t];
		}
		pCamera->SetSecretaryDeviceStatus(m_chSD.GetDeviceStatus());

		CAMERA_SETTING CameraSet = GetNewCameraSetting(t);

		if(pCamera->InitCamera(CameraSet,m_channelConfig.m_rejectorSetting.m_nDropImgPort))
		{
			pCamera->SetDispWindowID(m_currentImgWndID);
			pCamera->Attach2ImageProcessor(pProcessor);
			pCamera->Start();
			pCamera->Pause();
			IMG_SZ imgSz = gSystemMgr.GetChImageSize(m_channelConfig.m_chIndex);
			pProcessor->SetImgSize(imgSz);
		}
		else
		{
            std::vector<ERROR_INFO> vCameraError = pCamera->GetCameraErrorArray();
            CollectChannelError(vCameraError);
			wostringstream woss;
			woss << L"Initialize camera " << CameraSet.m_name << L"failed.";
			m_vChLogger[t].QueueUpLog(woss.str(),true);
		}
	}
	return true;
}

void CChannelControlPanel::TurnOnCamera()
{
	std::for_each(m_vChCameraPtr.begin(),m_vChCameraPtr.end(),[](CBaslerCamera* pCamera)
	{
		pCamera->Resume();
	});
	Sleep(20);
	m_chSD.Start();

	SetChannelStatus();
}

void CChannelControlPanel::PauseChCamera()
{
	m_chSD.Stop();
	Sleep(20);
	std::for_each(m_vChCameraPtr.begin(),m_vChCameraPtr.end(),[](CBaslerCamera* pCamera)
	{
		pCamera->Pause();
	});
	SetChannelStatus();
}

void CChannelControlPanel::HandleNoCameraConnectionEvent()
{
	SetChannelStatus();
	CString sMsg;
	sMsg.LoadString(IDS_STRING_ERROR_CAMERA_CONNECTION_LOST);
	CString sChIndex;
	sChIndex.Format(_T("%d"), m_channelConfig.m_chIndex);
	sMsg.Replace(_T("#1"), sChIndex);
	AfxMessageBox(sMsg, MB_ICONERROR);
}

TASK CChannelControlPanel::GetCurrentTask()
{
	TASK crtTask;
	m_channelConfig.GetCurrentTask(crtTask);
	return crtTask;
}

wstring CChannelControlPanel::GetCurrentTaskName()
{
	return m_channelConfig.m_crtTaskName;
}
CString CChannelControlPanel::GetCurTaskName()
{
	int curIndex = m_comboChTask.GetCurSel();
	CString retStr;
	m_comboChTask.GetLBText(curIndex,retStr);
	return retStr;

}
bool CChannelControlPanel::findAllTaskname(CString strNameBegin,CString strNameEnd,CString& ReStrTaskname,CString &strErrorinfo)
{
	ReStrTaskname=_T("");
	vector<CString> VecStrTaskName;
	VecStrTaskName.clear();

	CString StrTaskname;
	for (int i = 0 ;i < m_comboChTask.GetCount();i++)
	{

		m_comboChTask.GetLBText(i,StrTaskname);

		CString strtasknameAdapt = StrTaskname;
		//高度整数适配
		int ibegin = StrTaskname.Find(_T("_"));
		int iend = StrTaskname.Find(_T("_"),ibegin+1);
		if (iend - ibegin == 3)
		{
			strtasknameAdapt.Insert(iend,_T(".0"));
			iend = strtasknameAdapt.Find(_T("_"),ibegin+1);
		}

		CString strLeft1 = strtasknameAdapt.Left(ibegin);
		CString strLeft2 = strNameBegin.Left(ibegin);

		CString strMid = strtasknameAdapt.Mid(ibegin+1,iend - ibegin-1);
		CString strMidB= strNameBegin.Mid(ibegin+1,iend - ibegin-1);
		CString strMidE= strNameEnd.Mid(ibegin+1,iend - ibegin-1);


		CString StrRight1 = strtasknameAdapt.Mid(iend,strtasknameAdapt.GetLength()-iend);
		CString StrRight2 = strNameBegin.Mid(iend,strNameBegin.GetLength()-iend);


		if(strMid >= strMidB && strMid < strMidE && strLeft1 ==strLeft2&& StrRight1==StrRight2)
		{
			if (ReStrTaskname.IsEmpty())
			{
				ReStrTaskname = StrTaskname;
			}
			if (strErrorinfo.IsEmpty())
			{
				strErrorinfo += StrTaskname;
			}
			else
			{
				strErrorinfo +=_T(",");
				strErrorinfo += StrTaskname;
				return false;
			}
		}
	}
	if (strErrorinfo.IsEmpty())
	{
		return false;
	}

	return true;


}

TASK CChannelControlPanel::GetTask(wstring wstrTaskName)
{
	TASK task = m_channelConfig.GetTask(wstrTaskName);
	return task;
}

SPC_DATA_PT CChannelControlPanel::GetCurrentTaskLastStatsData(CString strbatchnumber,CString username)
{
	SPC_DATA_PT spcDataPt;
	CString CrtUser = gSystemMgr.GetCurrentUser().m_strUsername.c_str();
	if (!username.IsEmpty()&&username != CrtUser)
	{
		return spcDataPt;
	}
	if (strbatchnumber.IsEmpty())
	{
		int nDeno = m_nSampleSize / 1000;
		spcDataPt.m_totalImgCount    = m_nTotalCount;
		spcDataPt.m_totalNgImgCount  = (m_nDefective >= 0) ? m_nDefective : 0;
		spcDataPt.m_fNgRatio         = 1.0f * (m_nDefective - m_nOldDefective) / nDeno;
		m_nOldDefective              = m_nDefective;
		CollectInternalStats(spcDataPt,false);
		return spcDataPt;
	}
	else
	{
		if(strbatchnumber == gSystemMgr.GetCurBatchNumber())
		{
			int nDeno = m_nSampleSize / 1000;
			spcDataPt.m_totalImgCount    = m_nTotalCount;
			spcDataPt.m_totalNgImgCount  = (m_nDefective >= 0) ? m_nDefective : 0;
			spcDataPt.m_fNgRatio         = 1.0f * (m_nDefective - m_nOldDefective) / nDeno;
			spcDataPt.m_strBatchNumber   = strbatchnumber;
			spcDataPt.m_strUser = gSystemMgr.GetCurrentUser().m_strUsername.c_str();
			m_nOldDefective              = m_nDefective;
			CollectInternalStats(spcDataPt,false);
		}
		return spcDataPt;
	}
}

void CChannelControlPanel::UpdateTaskThreshold()
{
	int newThreshold  = m_chartCtrl.GetBlowThresholdValue();

	for (int i = 0 ; i <m_channelConfig.m_vTask.size();i ++)
	{
		if (m_channelConfig.m_vTask[i].m_wstrTaskName == m_channelConfig.m_crtTaskName)
		{
			if (m_channelConfig.m_vTask[i].m_blowThreshold!= newThreshold)
			{
				m_channelConfig.m_vTask[i].m_blowThreshold = newThreshold;
				gConfigMgr.SetChannelConfig(m_channelConfig.m_chIndex,m_channelConfig);
				gConfigMgr.UpdateTaskBlowThreshold(m_chIndex,m_channelConfig.m_crtTaskName,newThreshold);
			}
			break;
		}
	}
}

void CChannelControlPanel::WriteLeftoverStatsData2Database(wstring wstrTaskName)
{
	TASK* pTask = m_channelConfig.GetCurrentTask();
	if(!pTask->m_bSelfLearning && m_lastDataPt.m_totalImgCount > 0)
	{
		gSystemMgr.ReceiveStatsDataPoint(*pTask, m_lastDataPt);
		if(m_chIndex == gSystemMgr.m_nSysChIndex)
		{
			gSystemMgr.ReceiveSystemStatsDataPoint(true);
		}
	}

	UpdateStatistics();
	m_statisticsBuffer.clear();
	m_lastDataPt.Reset();
}

void CChannelControlPanel::StopImgProcessor()
{
	std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
	{
		pProcessor->Stop();
	});

	int nChCount = m_vChThreadHandle.size();
	::WaitForMultipleObjects(nChCount, &m_vChThreadHandle[0], TRUE, INFINITE);
	m_vChThreadHandle.clear();

	std::for_each(m_vChLogger.begin(),m_vChLogger.end(),[](CLogger& logger)
	{
		logger.FlushLogCache();
	});
}

void CChannelControlPanel::OpenSpcScreen()
{
	gSystemMgr.m_displyManager.EnableDisplaying(false);
	CSpcDlg spcDlg(m_channelConfig.m_chIndex);

	ResetDisplayRule();
    TASK crtTask;
	m_channelConfig.GetCurrentTask(crtTask);
	SetDebugFlag(false);
	if(spcDlg.DoModal() == IDOK)
	{
 		gSystemMgr.m_displyManager.EnableDisplaying(true);
	}
}

void CChannelControlPanel::OpenChannelScreen()
{
	if(m_channelConfig.m_vecCameraSetting.size()==0)
	{
		m_btnChDebug.EnableWindow(FALSE);
		return;
	}
	gSystemMgr.m_displyManager.EnableDisplaying(false);
	m_bDispAllImg = false;
	CChannelConfigDlg chConfigDlg(m_channelConfig, m_vProcessorPtr);
	chConfigDlg.SetChannelControlPanelPtr(this);
	ResetDisplayRule();
    TASK crtTask;
	m_channelConfig.GetCurrentTask(crtTask);
	chConfigDlg.SetCurCameraIndex(0);
	SetDebugFlag(false);
	if(chConfigDlg.DoModal() == IDOK)
	{
		gSystemMgr.m_displyManager.EnableDisplaying(true);
		if(!IsChCameraRunning())
		{
			m_lastDataPt.Reset();
			UpdateStatistics();
		}
	}
}

std::vector<CImageProcessor*> CChannelControlPanel::GetImgProcessorPtr()
{
	return m_vProcessorPtr;
}

void CChannelControlPanel::SetChannelStatus()
{
	m_comboChTask.EnableWindow(TRUE);
	bool bRun= false;

	//跨通道相机 运行判断
	if (m_mainchannel)
	{
		  bRun = gSystemMgr.GetPanel(m_mainchannel)->IsChCameraRunning();
	}
	else
	{
		  bRun = IsChCameraRunning();
	}

	
	if(m_bImgProcessorOK)
	{
		m_chSD.ExecuteCmd(RED_LIGHT_OFF);
		if(bRun)
		{
			m_OnOffBtn.SetLabelImage(IMG_RUN);
			m_chSD.ExecuteCmd(GREEN_LIGHT_ON);
			m_channelState = CH_ON;
			m_comboChTask.EnableWindow(FALSE);
//			m_DataClearBtn.ShowWindow(SW_HIDE);

		}
		else
		{
			m_OnOffBtn.SetLabelImage(IMG_STOP);
			m_chSD.ExecuteCmd(GREEN_LIGHT_OFF);
			m_channelState = CH_OFF;
			UAC crtUser = gSystemMgr.GetCurrentUser();
			if(crtUser.m_nLevel == UAC_ADMIN)
			{
				m_DataClearBtn.ShowWindow(SW_SHOW);
			}
			else
			{
				m_DataClearBtn.ShowWindow(SW_HIDE);
			}

		}
		TASK task;
		m_channelConfig.GetCurrentTask(task);
		bool bSlCompleted = IsModelImageReady();
		if(task.m_bSelfLearning)
		{
			m_btnChDebug.EnableWindow(!bRun);
		}
		else
		{
			int nTaskIndex = m_comboChTask.GetCurSel();
			m_btnChDebug.EnableWindow(nTaskIndex != -1);
		}
	}
	else
	{
		m_OnOffBtn.SetLabelImage(IMG_WARNING);
		m_btnChDebug.EnableWindow(false);
		m_chSD.ExecuteCmd(GREEN_LIGHT_OFF);
		m_chSD.ExecuteCmd(RED_LIGHT_ON);
		m_channelState = CH_DEBUG;
	}
	CRect rc;
	m_OnOffBtn.GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect(rc);
}

bool CChannelControlPanel::IsChCameraRunning()
{
    if(m_vChCameraPtr.empty())
    {
        return false;
    }
	for(auto i = 0; i < m_vChCameraPtr.size(); i++)
	{
		if(!m_vChCameraPtr[i]->IsCameraRunning())
		{
			return false;
		}
	}
	return true;
}

bool CChannelControlPanel::IsChCameraOpened()
{
	for(auto i = 0; i < m_vChCameraPtr.size(); i++)
	{
		if(!m_vChCameraPtr[i]->IsCameraOpened())
		{
			return false;
		}
	}
	return true;
}

std::vector<ERROR_INFO> CChannelControlPanel::GetErrorInfo()
{
	return m_vChError;
}

bool CChannelControlPanel::ResumeAllCamera()
{
	for(auto i = 0; i < m_vChCameraPtr.size(); i++)
	{
		if(!m_vChCameraPtr[i]->Resume())
		{
			return false;
		}
	}
	//暂停所有通道相机, 通道计数为零（多相机工位）-》用于取反信号
	m_nImgIndex = 0;

	return true;
}

void CChannelControlPanel::OnStnDblClickedNickName()
{
	UAC crtUac = gSystemMgr.GetCurrentUser();
	if(crtUac.m_strUsername.compare(L"SMVSADMIN") == 0)
	{
		int nChIndex = m_channelConfig.m_chIndex;
		wstring wstrNewLaneName = L"";
		CLaneNameDlg dlg(m_rcPanel,wstrNewLaneName);
		if(dlg.DoModal() == IDOK)
		{
			if(wstrNewLaneName != m_channelConfig.m_wstrLaneName)
			{
				m_channelConfig.m_wstrLaneName = wstrNewLaneName;
				m_laneNamePanel.SetWindowText(wstrNewLaneName.c_str());
				m_laneNamePanel.Invalidate();
				gConfigMgr.SaveLaneName(nChIndex,wstrNewLaneName);
			}
		}
	}
}
void  CChannelControlPanel::ClearChData()
{
	UAC crtUser = gSystemMgr.GetCurrentUser();
	if(crtUser.m_nLevel == UAC_ADMIN)
	{
		gSystemMgr.SaveLeftoverStatisticsData(m_channelConfig.m_chIndex);
        m_chSD.ExecuteCmd(QOP_DATA_RESET,RESET_STATS);
		ResetInternalStats();
		m_chartCtrl.ClearGraph();
		m_chartCtrl.Invalidate(TRUE);

		DelChError(THRESHOLD_BLOW_ERROR);
		UpdateWarningFlag();
	}
}


void  CChannelControlPanel::ClearChData_Ex()
{
	if (m_mainchannel)
	{
		gSystemMgr.GetPanel(m_mainchannel)->ClearChData();
		ClearChData();
		return;

	}
	if (m_subchannel)
	{
		ClearChData();
		gSystemMgr.GetPanel(m_subchannel)->ClearChData();
		return;

	}

	ClearChData();
}

void  CChannelControlPanel::OnStnClickedDataClear()
{
// 	if (m_channelState == CH_ON)
// 	{
// 		return;
// 	}
	if (gSystemMgr.IsClearAllDataFunTrue())
	{
		gSystemMgr.ResetAllChannelData();
	}
	else
	{
		ClearChData_Ex();
	}
}

void  CChannelControlPanel::HandleCurrentErrorEvent(const ERROR_INFO& crtErrorInfo)
{
//    bool bWarningEvent = (crtErrorInfo.m_level > WL_NONE);

	 
	if (crtErrorInfo.m_level == WL_ERROR)
	{
		m_cLabWarnShow.SetLabelImage(IMG_WARN_WARNED);
		if (m_ChannelWarnlevel != WL_ERROR)
		{
			m_chSD.ExecuteCmd(QOP_PC_STATUS_SET,PC_STATUS_SET,2<<24);
		}

	}
	else if (crtErrorInfo.m_level == WL_WARNING)
	{
		m_cLabWarnShow.SetLabelImage(IMG_WARN_WARNED);
		if (m_ChannelWarnlevel != WL_WARNING)
		{
			m_chSD.ExecuteCmd(QOP_PC_STATUS_SET,PC_STATUS_SET,1<<24);
		}
	}
	else
	{
		m_cLabWarnShow.SetLabelImage(IMG_WARN_NORMAL);//
		if (m_ChannelWarnlevel != WL_NONE)
		{
			m_chSD.ExecuteCmd(QOP_PC_STATUS_SET,PC_STATUS_SET,0);
		}
// 		if (IsChCameraRunning())
// 		{
// 			   m_chSD.ExecuteCmd(GREEN_LIGHT_ON);
// 		}
	}

	m_ChannelWarnlevel = crtErrorInfo.m_level;
	m_cLabWarnShow.ShowWindow(SW_HIDE);
	m_cLabWarnShow.ShowWindow(SW_SHOW);
}

void  CChannelControlPanel::ReTestLightController()
{
	ResetEvent(gSystemMgr.m_dLcTestOverEvent);
    gSystemMgr.RemoveLcIdFromCheckResult(m_channelConfig.m_lightCtrlSetting.m_vLcID);
	if (gSystemMgr.IsLcSerialPortOk())
	{
		ResetEvent(gSystemMgr.m_dLcTestOverEvent);
		
	// 光源控制器只有在单独测试的时候，才会用到这个函数，不需要通过添加定时器来检测光源测试结果
	//	((CSMVSDlg *)m_pParentWnd)->SetTimer(TIMER_ID_START_UP_REPORT, 1000, NULL);
		gSystemMgr.SendLightControllerTestSignal(m_chIndex);
	}
}

void CChannelControlPanel::VerifyLightControllerTestResult(const std::set<int>& allOkLcIDs)
{
    bool bTestOk = true;
    std::vector<int> vLcID = m_channelConfig.m_lightCtrlSetting.m_vLcID;
    CString sMsg;
    CString sIDALL;
    std::for_each(vLcID.begin(), vLcID.end(), [&](int id)
    {
        if(allOkLcIDs.find(id) == allOkLcIDs.end())
        {
            CString sID;
            sID.Format(_T("%d,"), id);
            sIDALL+=sID;
            bTestOk = false;
        }
    });

    if (!bTestOk)
    {
        sIDALL=sIDALL.Left(sIDALL.GetLength()-1);
        sMsg.LoadString(IDS_STRING_SYS_ERROR_CANNOT_ACCESS_TO_LC);
        sMsg.Replace(_T("#1"), sIDALL);
        wstring strMsg = CT2W(sMsg);	

        CString strID;
        strID.LoadString(IDS_STRING_ERR_ERRID);
        CString streMsg;
        streMsg.Format(_T("%s%d0%d"),strID,LIGHT_CONTRL_CONNECT_ERROR,ERRORSUBID_LIGHT_CONNECT_FAILED);
        streMsg += _T("---")+CString(strMsg.c_str())+_T("");
        ERROR_INFO emsg(LIGHT_CONTRL_CONNECT_ERROR,ERRORSUBID_LIGHT_CONNECT_FAILED,streMsg.GetString());
        //AddChError(emsg);
		gLogger.QueueUpLog(streMsg.GetString());
    }
}

void  CChannelControlPanel::OnStnClickedEventFlag()
{
	CChannelEventMgrDlg dlg(m_chIndex,m_enableFlagStr,m_vChError,this);//
        
	while (m_enableFlagStr.GetLength() < SMVS_WARNING_MAXNUM)
	{
		m_enableFlagStr += _T("1");
	}
	if(dlg.DoModal()== IDOK)
	{

	}
    m_channelConfig.m_chWarningflag = m_enableFlagStr;
    UpdateWarningFlag();
}

void CChannelControlPanel::ClearChError()
{
	m_vChError.clear();
}

void CChannelControlPanel::UpdateWarningFlag()
{
	if( m_sRate >= m_channelConfig.m_rejectorSetting.m_fLastNRatio)
	{
		if(m_chSD.GetDeviceStatus()&&m_channelConfig.m_rejectorSetting.m_bEnableLastNAlarm)
		{
			CString str;
			str.LoadString(IDS_STRING_LASTNINFO);
			CString stremsg;
			stremsg.Format(str,m_channelConfig.m_rejectorSetting.m_nLastN,m_sRate*100,m_channelConfig.m_rejectorSetting.m_fLastNRatio*100);

			//m_chSD.ExecuteCmd(RED_LIGHT_ON);
			CString strID;
			strID.LoadString(IDS_STRING_ERR_ERRID);
			CString streMsg;
			streMsg.Format(_T("%s%d0%d"),strID,CDP_ERROR,ERRORSUBID_CDP_LASTN_ERROR);
			streMsg += _T("---")+stremsg+_T("");
			ERROR_INFO emsg(CDP_ERROR,ERRORSUBID_CDP_LASTN_ERROR,streMsg.GetString());
			AddChError(emsg);
		}
	}


    ERROR_INFO crtErrorInfo;
	eERROR_LEVEL emWarnLevel = WL_NONE;  //0无告警,1 黄告警, 2红告警
	for (int i = 0 ; i< m_vChError.size() ; i++)
	{
		int iErrorqueue = (m_vChError[i].m_errorID - PCI_CREATE_FAILED_ERROR);
		if (m_enableFlagStr.Mid(iErrorqueue,1) == _T("1"))
		{
			if (emWarnLevel < m_vChError[i].m_level)
			{
				emWarnLevel = m_vChError[i].m_level;
                crtErrorInfo = m_vChError[i];
			}
		}
		if(m_vChError[i].m_level!=WL_NONE)
			m_chSD.ExecuteCmd(RED_LIGHT_ON);
	}
    HandleCurrentErrorEvent(crtErrorInfo);
}

eCHANNEL_STATE  CChannelControlPanel::GetChannelState()
{
	return m_channelState;
}

void CChannelControlPanel::ShowChannelDataClearButton(bool bShow)
{
	m_DataClearBtn.ShowWindow(bShow);
}

void  CChannelControlPanel::OnStnClickedOnOff_EX()
{
	if (m_mainchannel)
	{
		gSystemMgr.GetPanel(m_mainchannel)->OnStnClickedOnOff();
		OnStnClickedOnOff();
		return;

	}
	if (m_subchannel)
	{
		OnStnClickedOnOff();
		gSystemMgr.GetPanel(m_subchannel)->OnStnClickedOnOff();
		return;

	}
	OnStnClickedOnOff();
}
void  CChannelControlPanel::OnStnClickedOnOff()
{
//	gLogger.QueueUpLog(_T("this is a test"));

//     if(m_vChCameraPtr.empty())
//     {
//         CString sMsg;
//         sMsg.LoadString(IDS_STRING_CAMERA_ABSENT);
//         CString msg;
//         msg.Format(sMsg,m_channelConfig.m_chIndex);
//         AfxMessageBox(msg,MB_OK|MB_ICONERROR,NULL);
//         return;
//     }

	TASK task;
	m_channelConfig.GetCurrentTask(task);
	eCHANNEL_STATE oldState = m_channelState;
	CString sMsg;

	UAC crtUser = gSystemMgr.GetCurrentUser();
	if(oldState == CH_ON && crtUser.m_nLevel == UAC_ADMIN)
	{
// 		m_chSD.Stop();
// 		Sleep(20);
		PauseChCamera();

		m_lastDataPt.Reset();
		UpdateStatistics();
		m_DataClearBtn.ShowWindow(SW_SHOW);
	}
	if(oldState == CH_ON && crtUser.m_nLevel == UAC_OPERATOR)
	{
		return;
	}
	if(oldState == CH_OFF)
	{
		bool bSlCompleted = IsModelImageReady();
		if(task.m_bSelfLearning && bSlCompleted)
		{
			sMsg.LoadString(IDS_STRING_RESTART_SL_PROCEDURE);
			if(IDYES == AfxMessageBox(sMsg, MB_YESNO, NULL))
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,SELF_LEARN_FUNC,1);
				ResetSelfLearningProcedure();
			}
			else
			{
				m_chSD.ExecuteCmd(QOP_PARAM_SET,SELF_LEARN_FUNC,0);
				return;
			}
		}
			

		m_lastDataPt.Reset();
		bool bIsOpen = ResumeAllCamera();

		if (m_mainchannel||m_subchannel)
		{
			m_DataClearBtn.ShowWindow(SW_HIDE);
		}
// 		if (bIsOpen)
// 		{
// 			m_DataClearBtn.ShowWindow(SW_HIDE);
// 		}
		Sleep(20);
		m_chSD.Start();
	}
	if(oldState == CH_DEBUG)
	{
		CString sMsg;
		sMsg.Format(_T("CH%d - "), m_channelConfig.m_chIndex);
		CString sTmp;
		sTmp.LoadString(IDS_STRING_ALGO_GP_PARAM_ERROR);
		sMsg.Append(sTmp);
		AfxMessageBox(sMsg, MB_OK);
	}
	SetChannelStatus();
	if(oldState != m_channelState)
	{
		::PostMessageW(m_pParentWnd->m_hWnd, MSG_CH_ON_OFF, 0, 0);
	}
}

void CChannelControlPanel::SelectSlefLearningTargetTaskName()
{
	TASK* pCrtTask = m_channelConfig.GetCurrentTask();
	if(pCrtTask == nullptr || !pCrtTask->m_bSelfLearning)
	{
		return;
	}
	std::vector<wstring> vTaskName;
	std::for_each(m_channelConfig.m_vTask.begin(),m_channelConfig.m_vTask.end(),[&vTaskName](TASK t)
	{
		if(!t.m_bSelfLearning)
		{
			vTaskName.push_back(t.m_wstrTaskName);
		}
	});
	wstring wstrRetTaskName;
	CApplySlResultDlg dlg(m_channelConfig.m_chIndex,vTaskName,wstrRetTaskName);
	if(dlg.DoModal() == IDOK)
	{
		std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
		{
			pProcessor->SetSelfLearningTargetTaskName(wstrRetTaskName);
		});
	}
}
CString CChannelControlPanel::FindAdaptTaskNameEx(CString taskname)
{
	int i=0;
	for ( i = 0 ; i <taskname.GetLength();i++)
	{
		int unicode = (int)taskname.GetAt(i);
		if (unicode < 255)
		break;
	}

	if (i==taskname.GetLength())
	{
		return _T("");
	}
	CString strPattern = taskname.Mid(i);
	//第二通道 型号特例!
	if (m_chIndex==2)
	{
		int j = taskname.ReverseFind(_T('_'));
		strPattern = taskname.Mid(i,j-i);
	}
	for ( int k = 0; k < m_channelConfig.m_vTask.size(); k++)
	{
		CString strname = m_channelConfig.m_vTask[k].m_wstrTaskName.c_str();;
		if (strname.Find(strPattern) != -1)
		{
			return strname;
		}
	}

	return _T("");
}

CString CChannelControlPanel::findAdaptTaskName(CString taskname)
{
	int i = taskname.Find('_');
	int j = taskname.ReverseFind('_');

	if (i!=-1 && j!= -1 && j<=i)
	{
		return _T("");
	}
	CString strPattern = taskname.Mid(i,j-i);

	for ( int k = 0; k < m_channelConfig.m_vTask.size(); k++)
	{
		CString strname = m_channelConfig.m_vTask[k].m_wstrTaskName.c_str();;
		if (strname.Find(strPattern) != -1)
		{
			return strname;
		}
	}

	return _T("");
}
int CChannelControlPanel::GetTaskSelByName(CString strname)
{
	for (int i = 0; i< m_comboChTask.GetCount();i++)
	{
		CString RetStr;
		m_comboChTask.GetLBText(i,RetStr);
		if(strname== RetStr)
		{
			return i;
		}
	}
	return -1;
}
bool CChannelControlPanel::AutoChangeTask(CString& taskname,bool bAuto)
{
	if (bAuto)
	{
		taskname = findAdaptTaskName(taskname);
		if (taskname.IsEmpty())
		{
			return false;
		}
		int index = GetTaskSelByName(taskname);
		if (index != -1)
		{
			m_comboChTask.SetCurSel(index);
		}
	}
	bool boldattach = false;
	bool bnewattach = false;
	TASK* pCrtTask = m_channelConfig.GetCurrentTask();
	if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
	{
		boldattach = true;
	}

	wstring taskName = taskname.GetString();
	if(m_channelConfig.m_crtTaskName == taskName)
	{
		return false;
	}
	else
	{
		if(m_channelState == CH_OFF || m_channelState == CH_DEBUG)
		{
			if (!bAuto)
			{
				CString sMsg;
				sMsg.LoadString(IDS_STRIN_MSG_CHANGE_ALGO_GP);
				if(IDNO == AfxMessageBox(sMsg, MB_YESNO, NULL))
				{
					m_comboChTask.SetCurSel(m_nTaskComboIndex);
					return false;
				}
			}
		}
	}

	if(IsChCameraRunning())
	{
		PauseChCamera();
		Sleep(20);
	}
	StopImgProcessor();
	m_channelConfig = gConfigMgr.GetChannelConfig(m_channelConfig.m_chIndex);

	UpdateTaskThreshold();
	gSystemMgr.SaveLeftoverStatisticsData(m_channelConfig.m_chIndex);
	
	wstring wstrname = GetCurrentTaskName();
	m_chSD.ExecuteCmd(QOP_DATA_RESET,RESET_STATS);
	
	WriteLeftoverStatsData2Database(m_channelConfig.m_crtTaskName);
	Sleep(20);

	m_channelConfig.m_crtTaskName = taskName;
	gConfigMgr.UpdateCrtTaskName(m_channelConfig.m_chIndex, taskName);
	// Update channel configuration
	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	(*pChConfigArray)[m_channelConfig.m_chIndex] = m_channelConfig;

	if(m_channelConfig.m_rejectorSetting.m_bTestRejector)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_REJECTOR_IN_TEST_MODE);
		CString strIndex;
		strIndex.Format(_T("%d"), m_channelConfig.m_chIndex);
		sMsg.Replace(_T("#1"), strIndex);
		if(IDNO == AfxMessageBox(sMsg, MB_YESNO, NULL))
		{
			m_channelConfig.m_rejectorSetting.m_bTestRejector = false;
			gSystemMgr.UpdateRejectorSetting(m_channelConfig.m_chIndex,m_channelConfig.m_rejectorSetting);
			gConfigMgr.UpdateRejectorSetting(m_channelConfig.m_chIndex,&(m_channelConfig.m_rejectorSetting));
		}
	}
//	gSystemMgr.m_pciDevice.CleanNgSignal(m_channelConfig.m_chIndex);
	gSystemMgr.SendLightControllerTestSignal(m_chIndex);
	SetChannelStatus();

	pCrtTask = m_channelConfig.GetCurrentTask();
	if (pCrtTask!= NULL && pCrtTask->m_vecTaskCameraSetting.size()!= 0)
	{
		bnewattach = true;
	}

	if((boldattach == false) && (bnewattach == false))
	{
		StartImgProcessor();
	}
	else
	{
		StartImgProcessor(true);
	}

	SelectSlefLearningTargetTaskName();

	return true;
}
void  CChannelControlPanel::OnCbnSelchangeAlgoGroup()
{
	int curIndex = m_comboChTask.GetCurSel();
	CString strTaskname;
	m_comboChTask.GetLBText(curIndex,strTaskname);
	if (gSystemMgr.IsAutoChangeTaskFuncTrue())
	{
		//不启动一键切换任务功能！
		if (!gSystemMgr.AutoChangeTask(strTaskname))
		{
			if(AutoChangeTask(strTaskname))
			{
				m_nTaskComboIndex = curIndex;
			}
		}
	}
	else
	{
		if(AutoChangeTask(strTaskname))
		{
			m_nTaskComboIndex = curIndex;
		}
	}
 }

void  CChannelControlPanel::OnCbnSelchangeStayTime()
{
	int curIndex = m_comboStayTime.GetCurSel();
	CString strTaskname;
	m_comboStayTime.GetLBText(curIndex,strTaskname);
	m_nErrorImgStayTime = _tstoi(strTaskname)*1000;
}

void CChannelControlPanel::SetTestItemPassFlag(bool bDebug, std::map<int, bool>& crtStats,bool bErrCountAdd)
{
	std::vector<FUNC_INFO>& pTmpFuncArray = ((!bDebug) ? m_vFuncInfo : m_vFuncInfoDebug);

	for(int i = 0 ; i < pTmpFuncArray.size();i++)
	{
		int nFuncIndex = pTmpFuncArray[i].m_funcIndex;
		if(crtStats.find(nFuncIndex) != crtStats.end())
		{
			pTmpFuncArray[i].m_bPass = false;
		}
		if(!bDebug)
		{
			if (bErrCountAdd)
			{
				if (!pTmpFuncArray[i].m_bPass)
				{
					pTmpFuncArray[i].m_nErrCount++;
				}
				UpdateRecentNGCount(pTmpFuncArray[i].m_funcIndex,pTmpFuncArray[i].m_bPass);
			}
		}
	}
}

void CChannelControlPanel::UpdateRecentNGCount(int nFuncInex,bool bPass)
{
	if (m_vFuncInfo[nFuncInex].m_nTotalCount - m_channelConfig.m_rejectorSetting.m_nLastN >=0)
	{
		if (!m_vFuncInfo[nFuncInex].m_VecRecentErrCount.empty())
		{
		
			if (m_vFuncInfo[nFuncInex].m_VecRecentErrCount.front()+m_channelConfig.m_rejectorSetting.m_nLastN<=m_nTotalCount)
			{
				m_vFuncInfo[nFuncInex].m_VecRecentErrCount.pop();
			}
// 			else
// 			{
// 				if(nFuncInex==1&&m_nTotalCount>30)
// 				{
// 					CString str;
// 					str.Format(_T("%d,%d,%d,%d"),m_vFuncInfo[nFuncInex].m_VecRecentErrCount.front(),m_nTotalCount,m_channelConfig.m_rejectorSetting.m_nLastN,m_vFuncInfo[nFuncInex].m_VecRecentErrCount.front()-(m_nTotalCount-m_channelConfig.m_rejectorSetting.m_nLastN));
// 					gLogger.QueueUpLog(str.GetString(),true);
// 				}
// 			}

		}
	}
	if(!bPass)
	{
		m_vFuncInfo[nFuncInex].m_VecRecentErrCount.push(m_nTotalCount);
	}
}

void CChannelControlPanel::ResetTestItemPassFlag(bool bDebug)
{
	std::vector<FUNC_INFO>* pFuncInfoArray = (!bDebug) ? &m_vFuncInfo : &m_vFuncInfoDebug;
	std::for_each(pFuncInfoArray->begin(),pFuncInfoArray->end(),[&](FUNC_INFO& funcInfo)
	{
		funcInfo.m_bPass = true;
	});
}

void CChannelControlPanel::UpdateFunInfo(std::vector<FUNC_INFO> *vFuncInfo, std::vector<FUNC_INFO> *vFuncInfoDebug )
{
	m_vFuncInfo      = *vFuncInfo;
	m_vFuncInfoDebug = *vFuncInfoDebug;
}
void CChannelControlPanel::ChangeParameter(DISPID& funcId, int funcIndex, std::vector<INPUT_PARAM>& vNewParam)
{
	if(vNewParam.empty())
		return ;

	for(int i = 0; i < m_vFuncInfo.size(); i++)
	{
		if(funcId == m_vFuncInfo[i].m_dispId && funcIndex == m_vFuncInfo[i].m_funcIndex)
		{
			m_vFuncInfo[i].m_vParam.assign(vNewParam.begin(), vNewParam.end());
			m_vFuncInfo[i].CreateComParamArray();
			break;
		}
	}
}

void CChannelControlPanel::ChangeDebugParameter(DISPID& funcId, int funcIndex, std::vector<INPUT_PARAM>& vNewParam)
{
	if(vNewParam.empty())
		return ;

	for(int i = 0; i < m_vFuncInfoDebug.size(); i++)
	{
		if(funcId == m_vFuncInfoDebug[i].m_dispId && funcIndex == m_vFuncInfoDebug[i].m_funcIndex)
		{
			m_vFuncInfoDebug[i].m_vParam.assign(vNewParam.begin(), vNewParam.end());
			m_vFuncInfoDebug[i].CreateComParamArray();
			break;
		}
	}
}

void CChannelControlPanel::CollectInternalStats(SPC_DATA_PT& spcDataPt,bool boldDictFI_ECUpdate)
{
	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	spcDataPt.m_timestamp = utils::DateTime2Int64(dt); 

	spcDataPt.m_vItemStats.clear();

	long nTotalErrCount = 0;

	for (int i = 0; i < m_vFuncInfo.size(); i++)
	{
		if (i == 0)
		{
			continue;
		}
		if (m_dictFI_EC.find(i)!= m_dictFI_EC.end())
		{
			auto it = m_dictFI_EC.find(i);
			ITEM_STATS itemStats;
			int funcIndex   = it->first;
			nTotalErrCount += it->second;
			itemStats.m_nFuncIndex        = funcIndex;
			itemStats.m_wstrName          = m_vFuncInfo[funcIndex].m_strCustomizedName;
			itemStats.m_nRgnID            = m_vFuncInfo[funcIndex].GetTargetRegionID();

			if(m_oldDictFI_EC.find(funcIndex) != m_oldDictFI_EC.end())
			{
				itemStats.m_nItemErrorCount = it->second - m_oldDictFI_EC[funcIndex];
			}
			else
			{
				itemStats.m_nItemErrorCount = it->second;
			}
			spcDataPt.m_vItemStats.push_back(itemStats);
		}
		else
		{
			ITEM_STATS itemStats;
			int funcIndex   = m_vFuncInfo[i].m_funcIndex;
			itemStats.m_nFuncIndex        = funcIndex;
			itemStats.m_wstrName          = m_vFuncInfo[funcIndex].m_strCustomizedName;
			itemStats.m_nRgnID            = m_vFuncInfo[funcIndex].GetTargetRegionID();
			itemStats.m_nItemErrorCount = 0;
			spcDataPt.m_vItemStats.push_back(itemStats);
		}
	}
	spcDataPt.m_totalErrCount = nTotalErrCount;
	if (boldDictFI_ECUpdate)
	{
		m_oldDictFI_EC = m_dictFI_EC;
	}

	m_statisticsBuffer.push_back(spcDataPt.m_fNgRatio);
}

void CChannelControlPanel::UpdateInternalStatistics()
{
	std::for_each(m_vFuncInfo.begin(),m_vFuncInfo.end(),[&](FUNC_INFO& funcInfo)
	{
		if(!funcInfo.m_bPass)
		{
			m_dictFI_EC[funcInfo.m_funcIndex] = funcInfo.m_nErrCount;
		}
		funcInfo.m_nTotalCount++;
	});


	long index = m_nTotalCount % m_nSampleSize;
	if(index == 0)
	{
		int nDeno = m_nSampleSize / 1000;
		if(nDeno == 0)
			return;
		SPC_DATA_PT spcDataPt;
		spcDataPt.m_totalImgCount    = m_nTotalCount;
		spcDataPt.m_totalNgImgCount  = (m_nDefective >= 0) ? m_nDefective : 0;
		spcDataPt.m_fNgRatio         = 1.0f * (m_nDefective - m_nOldDefective) / nDeno;
		spcDataPt.m_strBatchNumber   = gSystemMgr.GetCurBatchNumber();
		spcDataPt.m_strUser   = gSystemMgr.GetCurrentUser().m_strUsername.c_str();
		m_nOldDefective              = m_nDefective;

        /*
		spcDataPt.m_fNgRatio 
        在此处判断当前数据点是否超限——红点还是留点，并触发警报；
        过期失效原则：仅仅是最新的数据点触发警报，历史数据点无论如何（例如，阈值被调整）都会不触发警报
        */

		if (spcDataPt.m_fNgRatio >= m_blowThreshold)
		{

			CString stremsg;
			stremsg.LoadString(IDS_STRING_ERR_ER501);
			//		stremsg.Format(_T("当前排盖率最大值%3.0f‰(设定阈值%d‰)"),spcDataPt.m_fNgRatio *1000.0f,m_blowThreshold);
			CString strparam1;
			strparam1.Format(_T("%3.0f"),spcDataPt.m_fNgRatio);
			CString strparam2;
			strparam2.Format(_T("%d"),m_blowThreshold);
			stremsg.Replace(_T("#1"),strparam1);
			stremsg.Replace(_T("#2"),strparam2);
			CString strID;
			strID.LoadString(IDS_STRING_ERR_ERRID);
			CString streMsg;
			streMsg.Format(_T("%s%d0%d"),strID,THRESHOLD_BLOW_ERROR,ERRORSUBID_THRESHOLD_ERROR);
			streMsg += _T("---")+stremsg+_T("");

			ERROR_INFO emsg(THRESHOLD_BLOW_ERROR,ERRORSUBID_THRESHOLD_ERROR,streMsg.GetString());
			AddChError(emsg);
		}

		CollectInternalStats(spcDataPt);
		TASK crtTask;
		m_channelConfig.GetCurrentTask(crtTask);
		//Write channel statistics data into database
		gSystemMgr.ReceiveStatsDataPoint(crtTask,spcDataPt);
		// Write system statistics data into database
		if(m_chIndex == gSystemMgr.m_nSysChIndex)
		{
			gSystemMgr.ReceiveSystemStatsDataPoint();
		}
		//Update statistic chart
		RedrawStatisticsChart();
	}
}

void CChannelControlPanel::ModifyInternalStats(int nFuncIndex, bool bAdded, bool bTestFunc)
{
	auto tmp = m_dictFI_EC;
	m_dictFI_EC.clear();

	auto tmpOld = m_oldDictFI_EC;
	m_oldDictFI_EC.clear();

	if(bAdded)
	{
		for(auto it = tmp.begin(); it != tmp.end(); it++)
		{
			int index = it->first;
			if(index < nFuncIndex)
			{
				m_dictFI_EC[index] = it->second;
			}
			if(index >= nFuncIndex)
			{
				m_dictFI_EC[++index] = it->second;
			}
		}
		for(auto it = tmpOld.begin(); it != tmpOld.end(); it++)
		{
			int index = it->first;
			if(index < nFuncIndex)
			{
				m_oldDictFI_EC[index] = it->second;
			}
			if(index >= nFuncIndex)
			{
				m_oldDictFI_EC[++index] = it->second;
			}
		}
		if(bTestFunc)
		{
			m_dictFI_EC[nFuncIndex]    = 0;
			m_oldDictFI_EC[nFuncIndex] = 0;
		}
	}
	else
	{
		for(auto it = tmp.begin(); it != tmp.end(); it++)
		{
			int index = it->first;
			if(index < nFuncIndex)
			{
				m_dictFI_EC[index] = it->second;
			}
			if(index > nFuncIndex)
			{
				m_dictFI_EC[--index] = it->second;
			}
		}
		for(auto it = tmpOld.begin(); it != tmpOld.end(); it++)
		{
			int index = it->first;
			if(index < nFuncIndex)
			{
				m_oldDictFI_EC[index] = it->second;
			}
			if(index > nFuncIndex)
			{
				m_oldDictFI_EC[--index] = it->second;
			}
		}
	}

	std::map<long,std::map<float, long>>  tmpstat;
	std::map<long, MIN_MAX>               tmpMinMax;

	if (!bAdded)
	{
		for (auto it = m_statistics.begin(); it != m_statistics.end();it++)
		{
			if(it->first < nFuncIndex)
			{
				tmpstat[it->first]   = m_statistics[it->first];
				tmpMinMax[it->first] = m_dictFuncIndex_MinMax[it->first];
			}
			else if (it->first > nFuncIndex)
			{
				tmpstat[it->first - 1]   = m_statistics[it->first];
				tmpMinMax[it->first - 1] = m_dictFuncIndex_MinMax[it->first];
			}

		}
	}
	else
	{
		for (auto it = m_statistics.begin(); it != m_statistics.end();it++)
		{
			if(it->first <= nFuncIndex)
			{
				tmpstat[it->first]   = m_statistics[it->first];
				tmpMinMax[it->first] = m_dictFuncIndex_MinMax[it->first];
			}
			else if (it->first > nFuncIndex)
			{
				tmpstat[it->first + 1]   = m_statistics[it->first];
				tmpMinMax[it->first + 1] = m_dictFuncIndex_MinMax[it->first];
			}
		}
	}
	m_statistics.clear();
	m_statistics = tmpstat;

	m_dictFuncIndex_MinMax.clear();
	m_dictFuncIndex_MinMax = tmpMinMax;
}

void CChannelControlPanel::RemoveInternalStats(std::vector<int>& vFuncIndex)
{
	int sz = (int)vFuncIndex.size();
	if(sz == 0)
	{
		return;
	}
	auto result = std::minmax_element(vFuncIndex.begin(), vFuncIndex.end());

	auto tmp = m_dictFI_EC;
	m_dictFI_EC.clear();

	auto tmpOld = m_oldDictFI_EC;
	m_oldDictFI_EC.clear();

	for(auto it = tmp.begin(); it != tmp.end(); it++)
	{
		int index = it->first;
		if(index < *result.first)
		{
			m_dictFI_EC[index] = it->second;
		}
		if(index > *result.second)
		{
			m_dictFI_EC[index - sz] = it->second;
		}
	}

	for(auto it = tmpOld.begin(); it != tmpOld.end(); it++)
	{
		int index = it->first;
		if(index < *result.first)
		{
			m_oldDictFI_EC[index] = it->second;
		}
		if(index > *result.second)
		{
			m_oldDictFI_EC[index - sz] = it->second;
		}
	}
}

std::vector<vector<IMG_PRODUCT>> CChannelControlPanel::GetNgImgArray(int& ErrorIndex)
{
	::EnterCriticalSection(&m_imgArrayCs);
	ErrorIndex = m_nCurErrorIndex;
	::LeaveCriticalSection(&m_imgArrayCs);
	return m_ngImgContainer;
}

std::vector<vector<IMG_PRODUCT>> CChannelControlPanel::GetContiguousImgArray()
{
	boost::circular_buffer<std::vector<IMG_PRODUCT>> tmpArray(IMAGE_ARRAY_SZ);
	::EnterCriticalSection(&m_imgArrayCs);
	tmpArray = m_contiguousImgArray;
	::LeaveCriticalSection(&m_imgArrayCs);
	std::vector<vector<IMG_PRODUCT>> retArray;
	while(!tmpArray.empty())
	{
		std::vector<IMG_PRODUCT> group = tmpArray.front();
		tmpArray.pop_front();
		retArray.push_back(group);
	}
	return retArray;
}

void CChannelControlPanel::CheckContinueDefective()
{
	int nCDC = m_channelConfig.m_rejectorSetting.m_nCDC;
	if(nCDC <= 0)
	{
		return;
	}
	unsigned long imgIndex = m_nTotalCount - 1;
	if(!m_stackNgIndex.empty())
	{
		long nIndex = m_stackNgIndex.top();
		if(imgIndex - nIndex > 1)
		{
			m_stackNgIndex.pop();
			m_stackNgIndex.push(imgIndex);
		}
		if(imgIndex - nIndex == 1)
		{
			m_stackNgIndex.push(imgIndex);
		}
	}
	else
	{
		m_stackNgIndex.push(imgIndex);
	}
	if(m_stackNgIndex.size() >= nCDC)
	{
		CString str;
		str.LoadString(IDS_STRING_CDPINFO);
		CString stremsg;
		stremsg.Format(str,m_stackNgIndex.size(),nCDC);

		while(!m_stackNgIndex.empty())
		{
			m_stackNgIndex.pop();
		}
		if(m_chSD.GetDeviceStatus())
		{
			CString strID;
			strID.LoadString(IDS_STRING_ERR_ERRID);
			CString streMsg;
			streMsg.Format(_T("%s%d0%d"),strID,CDP_ERROR,ERRORSUBID_CDP_ERROR);
			streMsg += _T("---")+stremsg+_T("");
			ERROR_INFO emsg(CDP_ERROR,ERRORSUBID_CDP_ERROR,streMsg.GetString());
			AddChError(emsg);
//             m_chSD.ExecuteCmd(GREEN_LIGHT_OFF);
//             m_chSD.ExecuteCmd(RED_LIGHT_ON);
//             PauseChCamera();
//             SetChannelStatus();
		}
	}
}

void CChannelControlPanel::CheckLastNDefective(bool bPass)
{
	int nLastN = m_channelConfig.m_rejectorSetting.m_nLastN;
	if(nLastN <= 0)
	{
		return;
	}
	unsigned long imgIndex = m_nTotalCount - 1;

	//////////////////////////////////////////////////////////////////
	if (m_nTotalCount - m_channelConfig.m_rejectorSetting.m_nLastN >=0)
	{
		if (!m_QueRecentErrCount.empty())
		{
			if (m_QueRecentErrCount.front() - (m_nTotalCount- m_channelConfig.m_rejectorSetting.m_nLastN)<=0)
			{
				m_QueRecentErrCount.pop();
			}
		}
	}
	if(!bPass)
	{
		m_QueRecentErrCount.push(m_nTotalCount);
	}


	////////////////////////////////////////////////////////////////
	long nRecErrCount = (long)m_QueRecentErrCount.size();
// 	if(m_nTotalCount - m_channelConfig.m_rejectorSetting.m_nLastN >= 0)
// 	{
		m_sRate= 1.0f * nRecErrCount /m_channelConfig.m_rejectorSetting.m_nLastN;
// 	}
// 	else
// 	{
// 		sRate = 1.0f * nRecErrCount /m_nTotalCount;
// 	}
}

bool CChannelControlPanel::GetFuncSpcData(int& funcIndex, std::map<float, long>& statistics)
{
	statistics.clear();
	std::map<long,std::map<float, long>> tmpSpcData;

	if(FALSE != ::TryEnterCriticalSection(&m_spcDataCs))//
	{
		tmpSpcData = m_statistics;
		::LeaveCriticalSection(&m_spcDataCs);
	}
	else
	{
		return false;
	}
	if(tmpSpcData.empty() || tmpSpcData[funcIndex].empty())
	{
		return false;
	}
	statistics = tmpSpcData[funcIndex];
	return true;
}

void CChannelControlPanel::CollectFuncOutputValueALL(int fiIndex,float fSpecV,float fiValue,bool bAdjustRange)
{
    if(FALSE != ::TryEnterCriticalSection(&m_spcDataCs))
    {
        m_statistics[fiIndex][fiValue]++;
        if(m_dictFuncIndex_MinMax.find(fiIndex) != m_dictFuncIndex_MinMax.end())
        {
            if(fiValue > m_dictFuncIndex_MinMax[fiIndex].m_fMax)
            {
                m_dictFuncIndex_MinMax[fiIndex].m_fMax = fiValue;
            }
            if(fiValue < m_dictFuncIndex_MinMax[fiIndex].m_fMin)
            {
                m_dictFuncIndex_MinMax[fiIndex].m_fMin = fiValue;
            }
        }
        else
        {
            MIN_MAX tmpMinMax;
            m_dictFuncIndex_MinMax[fiIndex] = tmpMinMax;
        }
		
		if(bAdjustRange && m_statistics.find(fiIndex) != m_statistics.end())
		{
			if(m_dictFuncIndex_MinMax.find(fiIndex) != m_dictFuncIndex_MinMax.end())
			{				
				auto func=[&]()
				{
					EnterCriticalSection(&m_spcDataCs2);
					std::map<float,long> &oldStats = m_statistics[fiIndex];                

					MIN_MAX minMax = m_dictFuncIndex_MinMax[fiIndex];
					if(minMax.m_fMax < 12.0f * fSpecV)
					{
						minMax.m_fMax = 12.0f * fSpecV; 
					}
					if(minMax.m_fMin > fSpecV)
					{
						minMax.m_fMin = 0.0f;
					}
					// Reset Min and Max value.
					//MIN_MAX tmpMinMax;
					//m_dictFuncIndex_MinMax[fiIndex] = tmpMinMax;

					//std::map<float,long> dictStats;
					for(auto it0 = oldStats.begin(); it0 != oldStats.end();)
					{                    
						if(it0->first < minMax.m_fMin || it0->first > minMax.m_fMax)
						{                        
							it0=oldStats.erase(it0);
						}
						else 
							it0++;
					}
					LeaveCriticalSection(&m_spcDataCs2);
				};
				boost::thread t(func);
				t.detach();
			}
		}		
		::LeaveCriticalSection(&m_spcDataCs);
    }
}

void CChannelControlPanel::CollectSelfResultCount(int nCount)
{
	m_nTotalCount = nCount;
}
bool CChannelControlPanel::AllInspectingResultReady()
{
	for(int i = 0; i < m_nDataBlockCount; i++)
	{
		if(m_pendingInspectedImgGroup.find(i) != m_pendingInspectedImgGroup.end())
		{
			if(m_pendingInspectedImgGroup[i].size() <= 0)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}
std::vector<IMG_PRODUCT> CChannelControlPanel::GetCurrentImgGroup()
{
	std::vector<IMG_PRODUCT> vRet;
	for(int i = 0; i < m_nDataBlockCount; i++)
	{
		IMG_PRODUCT imgProduct = m_pendingInspectedImgGroup[i].front();
		m_pendingInspectedImgGroup[i].pop();
		vRet.push_back(imgProduct);
	}
	return vRet;
}
void CChannelControlPanel::SetTestItemPassFlag_V2(bool bDebugImg)
{
	int nCount = 0;
	bool bAdjustCountFlag = false;
	for(auto it = m_dictProcessorIndex_FuncRslt.begin();it != m_dictProcessorIndex_FuncRslt.end();it++)
	{
		nCount++;
		if(nCount == m_nCameraCount)
		{
			bAdjustCountFlag = true;
		}
		queue<map<int,bool>>& funcRsltArray = it->second;
		std::map<int,bool> crtStats = funcRsltArray.front();
		funcRsltArray.pop();
 		if (nCount == m_dictProcessorIndex_FuncRslt.size())
 		{
 			SetTestItemPassFlag(bDebugImg,crtStats,true);//
		}
		else
 		{
			SetTestItemPassFlag(bDebugImg,crtStats);
		}
	}
}

bool CChannelControlPanel::CollectCheckResult(int nProcessorIndex, bool bOk, IMG_PRODUCT& crtImgProduct,DisplayResult& crtDisplayResult, std::map<int, bool> dictFuncIndex_Rslt,bool& bRet)
{	
	bool bDebug = crtImgProduct.m_bDebug;
	if((!bDebug&&m_nArrivalCount==0) ||(bDebug && m_nArrivalCount_Debug==0))
	{
		ResetTestItemPassFlag(bDebug);
	}

	if (m_nDataBlockCount==0)
	{
		m_nDataBlockCount=1;
	}

	if(!bDebug)
	{
		::EnterCriticalSection(&m_imgArrayCs);
		if(!bOk) 
			m_bFinalResult = false;	
		
		m_pendingInspectedImgGroup[nProcessorIndex].push(crtImgProduct);
		m_dictProcessorIndex_FuncRslt[nProcessorIndex].push(dictFuncIndex_Rslt);
		m_pendingDisplayResult[nProcessorIndex].push(crtDisplayResult);

// 		CString strlog;
// 		m_nArrivalCount++;
// 		strlog.Format(_T("%d|||"),m_nArrivalCount);
// 		for (auto it = dictFuncIndex_Rslt.begin(); it != dictFuncIndex_Rslt.end();it++)
// 		{
// 			CString str;
// 			str.Format(_T("%d,%d   "),it->first,it->second);
// 			strlog+=str;
// 		}
// 		OutputDebugString(strlog);	
// 		OutputDebugString(_T("\n"));

		if(AllInspectingResultReady())
		{
#ifdef BOTTLE_PRODUCT			
			if(m_nCameraCount>1)
			{			
				BOOL bOpenLabel=FALSE;			
				for (auto i = 0; i < m_vProcessorPtr.size();i++)
					if(m_vProcessorPtr[i]->GetDetectParam()==0)
						bOpenLabel=TRUE;
				if(!bOpenLabel)
				{
					m_bFinalResult = FALSE;
					gLogger.QueueUpLog(L"Open label lost!");
				}
			}
#endif
			 IMG_PRODUCT retFirst = m_pendingInspectedImgGroup[0].front();
			 m_ImageTimeCount    = retFirst.m_ImageTimeCount;
			 m_ImageTimeCount_PC = retFirst.m_ImageTimeCount_PC;
			 m_ImageID           = retFirst.m_ImageID;

			 for (auto i = 0; i < m_vProcessorPtr.size();i++)
			 {
				 m_crtDisplayResult[i] = m_pendingDisplayResult[i].front();
				 m_pendingDisplayResult[i].pop();
			 }
			SetTestItemPassFlag_V2(bDebug);
	//		SetTestItemPassFlag(bDebug,dictFuncIndex_Rslt,TRUE);
			m_crtImgGroup = GetCurrentImgGroup();
			if(m_crtImgGroup[0].m_unionErrRgn.size()>0)
				gSystemMgr.m_displyManager.ReceiveNgImage(m_chIndex,m_crtImgGroup,m_crtDisplayResult);

			m_nArrivalCount = 0;
			m_nTotalCount++;
			if(!m_bFinalResult)
			{
				m_nCurErrorIndex = m_nDefective % IMAGE_ARRAY_SZ;
				m_ngImgContainer[m_nCurErrorIndex] = m_crtImgGroup;
				m_nDefective++;
				CheckContinueDefective();
			}
			else
			{
				while(!m_stackNgIndex.empty())
				{
					m_stackNgIndex.pop();
				}
			}
			m_contiguousImgArray.push_back(m_crtImgGroup);	
			CheckLastNDefective(m_bFinalResult);			
			UpdateInternalStatistics();
			bRet = m_bFinalResult;
			m_bFinalResult = true;					
			::LeaveCriticalSection(&m_imgArrayCs);
			return true;
		}
		else
		{
			//此函数第三个参数用于算子列表计数，默认为false，Debug图片不计数！
			//新排盖3.7，程序在最后一次 综合处理相关算子 错误值！
//			SetTestItemPassFlag(bDebug,dictFuncIndex_Rslt);//
		}
		::LeaveCriticalSection(&m_imgArrayCs);
	}
	else
	{
		if(!bOk)
			m_bFinalResult_Debug = false;
#ifdef BOTTLE_PRODUCT			
		if(m_nCameraCount>1)
		{			
			BOOL bOpenLabel=FALSE;			
			for (auto i = 0; i < m_vProcessorPtr.size();i++)
				if(m_vProcessorPtr[i]->GetDetectParam()==0)
					bOpenLabel=TRUE;
			if(!bOpenLabel)
				m_bFinalResult_Debug = FALSE;
		}
#endif
		if(crtImgProduct.m_bSingleDebug)
		{
			for (int i = 0 ; i < m_nDataBlockCount;i++)
			{
				if (nProcessorIndex == i)
				{
					m_crtImgGroup_Debug[i] = crtImgProduct;
					m_crtDisplayResult_Debug[i] = crtDisplayResult;
				}
				else
				{
					IMG_PRODUCT crtImgProducttmp;
					DisplayResult crtDisplayResulttmp;
					m_crtImgGroup_Debug[i]      = crtImgProducttmp;
					m_crtDisplayResult_Debug[i] = crtDisplayResulttmp;
				}
				m_nArrivalCount_Debug++;
			}
		}
		else
		{
			m_crtImgGroup_Debug[nProcessorIndex]      = crtImgProduct;
			m_crtDisplayResult_Debug[nProcessorIndex] = crtDisplayResult;
			m_nArrivalCount_Debug++;
		}
		if(m_nArrivalCount_Debug == m_nDataBlockCount)
		{
			SetTestItemPassFlag(bDebug,dictFuncIndex_Rslt,TRUE);
			bRet = m_bFinalResult_Debug;
			m_nArrivalCount_Debug = 0;
			m_bFinalResult_Debug  = true;
			if (m_vFuncInfoDebug.size() == m_vFuncInfo.size())
			{		
				for(int i = 0 ; i < m_vFuncInfoDebug.size();i++)
				{
					m_vFuncInfoDebug[i].m_nErrCount = m_vFuncInfo[i].m_nErrCount;
					m_vFuncInfoDebug[i].m_nTotalCount = m_vFuncInfo[i].m_nTotalCount;
				}
			}
			return true;
		}
		else
		{
			SetTestItemPassFlag(bDebug,dictFuncIndex_Rslt);
		}
	}
	return false;
}

void CChannelControlPanel::UpdateChannelSpeed()
{
	long nSpeed = 0;
	if(m_nTotalCount >= m_nOldTotalCount)
	{
		nSpeed = (m_nTotalCount - m_nOldTotalCount) * 6;
		CString formatstr;
		formatstr.LoadString(IDS_STRING_PRODUCT_SPEED);
		
		CString sSpeed;
		sSpeed.Format(formatstr,nSpeed);
		m_chSpeedPanel.SetWindowText(sSpeed);
		m_chSpeedPanel.Invalidate();
	}
	m_nOldTotalCount = m_nTotalCount;
}

void CChannelControlPanel::UpdateStatistics()
{
	CString strTotoalCount;
	strTotoalCount.Format(_T("%d"), m_nTotalCount);
	m_totalValue.SetWindowText(strTotoalCount);
	m_totalValue.Invalidate();

	if(m_nDefective > m_nTotalCount)
	{
		m_nDefective = m_nTotalCount;
	}
	float ngRatio = 0.00f;
	if(m_nTotalCount != 0)
	{
		ngRatio = (float)m_nDefective / (float)m_nTotalCount;
	}

	CString strRatio;
	strRatio.Format(_T("%d / %.2f%%"), m_nDefective, 100 * ngRatio);
	m_failedValue.SetWindowText(strRatio);
	m_failedValue.Invalidate();
}

 void CChannelControlPanel::UpdateChannelStatistics()
{
// 	if(!IsChCameraRunning()&&m_mainchannel==0)
// 	{
// 		return;
// 	}
	UpdateStatistics();
}

void  CChannelControlPanel::RedrawStatisticsChart()
{
	if(::IsHungAppWindow(this->m_hWnd))
		return;

	m_chartCtrl.Receive2DLineGraphData(m_statisticsBuffer);
	m_chartCtrl.Invalidate();
}

LRESULT  CChannelControlPanel::OnSelfLearningComplete(WPARAM wParam,LPARAM lParam)
{
	if(lParam == -1)
	{
		CString sMsg;
		sMsg.Format(_T("CH%d - "), m_channelConfig.m_chIndex);
		CString sTmp;
		sTmp.LoadString(IDS_STRING_SELF_LEARNING_COMPLETE);
		sMsg.Append(sTmp);

		CString sValue;
		sValue.Format(_T("%d"),wParam);
		sMsg.Replace(_T("#"), sValue);
		AfxMessageBox(sMsg, MB_OK, NULL);
		m_btnChDebug.EnableWindow(TRUE);

		::PostMessageW(m_pParentWnd->m_hWnd, MSG_CH_ON_OFF, 0, 0);
		
	}
	return 0;
}

LRESULT  CChannelControlPanel::OnChannelStatusChanged(WPARAM wParam,LPARAM lParam)
{
	int flag = (int)lParam;
	m_bImgProcessorOK = ((flag == 1)? true:false);
	SetChannelStatus();
	return 0;
}

CRect CChannelControlPanel::GetChartRect()
{
	CRect rc;
	m_chartCtrl.GetWindowRect(rc);
	return rc;
}

void CChannelControlPanel::ExecuteQueryCmd()
{
	m_bRjectError = false; //警戒模式初始
	
	//时间同步
	m_chSD.SetCLientTimeSyn();
	if(IsChCameraRunning() || !IsChCameraRunning())
 	{
		CHANNEL_STATUS retData;
        std::vector<ERROR_INFO> vDeviceError;

 		if(m_chSD.ExecuteQueryCmd(retData,vDeviceError,m_bRjectError))
 		{

// 				SYSTEMTIME dt;
// 				::GetLocalTime(&dt);
// 				CString sTimestamp;
// 				sTimestamp.Format(_T("%2d-%2d | [%02d:%02d:%02d]    "),dt.wMonth,dt.wDay,dt.wHour, dt.wMinute,dt.wSecond);
// 				string strLogName("RejectError");
// 				std::string strChIndex = boost::lexical_cast<string>(m_chIndex);
// 				strLogName.append(strChIndex);
// 				strLogName.append(".txt");
// 				ofstream  ofile(strLogName,ios::app);
// 			
// 				ofile<<utils::Unicode2Ansi(sTimestamp.GetString()).c_str()<<endl;
// 				for (int i= 0 ; i < m_vecRjectErrorLog.size();i++)
// 				{
// 						ofile<<utils::Unicode2Ansi(m_vecRjectErrorLog[i].GetString()).c_str()<<endl;
// 				}
// 
// 				ofile.close();
// 			}
			::EnterCriticalSection(&m_deviceDataCs);
			m_chDeviceData = retData;
			::LeaveCriticalSection(&m_deviceDataCs);

			//批次号 和 系统统计功能一致
			if (gSystemMgr.IsShowSystemStateFunTrue())
			{
				if(m_chIndex == 1)
				{
					if (m_chDeviceData.m_nSensorCount >0) //考虑到Udp包丢失的情况，m_nSensorCount 的值会是-1，此时不能对m_crtSysStats.m_totalCount 赋值
					{																//否则系统总数会存在-1的情况，导致系统数据 统计异常！
																					//m_crtSysStats.m_totalCount 保存最近一次未丢包的数据！
						gSystemMgr.m_crtSysStats.m_totalCount = m_chDeviceData.m_nSensorCount;
					}
				}

 				if(m_chDeviceData.m_nTotalOkCount > 0)
 				{

			 		gSystemMgr.m_crtSysStats.m_totalOkCount = m_chDeviceData.m_nTotalOkCount;
					SYSTEMTIME dt;
					::GetLocalTime(&dt);
					gSystemMgr.m_crtSysStats.m_timestamp = utils::DateTime2Int64(dt); 
					gSystemMgr.m_crtSysStats.m_strBatchNumber = gSystemMgr.GetCurBatchNumber();
					gSystemMgr.m_crtSysStats.m_strUser = gSystemMgr.GetCurrentUser().m_strUsername.c_str();
					gSystemMgr.m_crtSysStats.m_strSysForeignKey =gSystemMgr.GetCurSysForeignKey();
					gSystemMgr.m_crtSysTaskName          = m_channelConfig.m_crtTaskName;
					gSystemMgr.m_nSysChIndex             = m_chIndex;
				}
			}
 			CollectChannelError(vDeviceError);

			if (m_bRjectError)
			{
				if (m_bRjectErrorCount>=3)
				{
					return;
				}

				m_bRjectErrorCount++;
				//警戒模式，恢复模式
				for(auto i = 0; i < m_vChCameraPtr.size(); i++)
				{
					m_vChCameraPtr[i]->ResetCameraFrameID();
				}
				Sleep(20);//与开机机制保持一致

				m_chSD.ExecuteCmd(QOP_DEVICE_RESET,DEVICE_RESET); //重置指令暂时不启用
				m_chSD.Start();
			
				
			}
			else
			{
				m_bRjectErrorCount=0;
			}
		}
	}
}

void CChannelControlPanel::GetChDeviceData(CHANNEL_STATUS& data)
{
	::EnterCriticalSection(&m_deviceDataCs);
	data    = m_chDeviceData;
	::LeaveCriticalSection(&m_deviceDataCs);
}

void CChannelControlPanel::SetBlowThresholdValue(int th, bool bFreshUI)
{
	m_blowThreshold = th;
	m_chartCtrl.SetBlowThresholdValue(th);
	m_chartCtrl.Invalidate();
}

void CChannelControlPanel::ResetDisplayRule()
{
	std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
	{
		pProcessor->ResetDisplayRule();
	});
}

void CChannelControlPanel::ResetSelfLearningProcedure()
{
	std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
	{
		pProcessor->ResetSelfLearningProcedure();
	});
}

bool CChannelControlPanel::IsModelImageReady()
{
	for(auto it = m_vProcessorPtr.begin();it != m_vProcessorPtr.end();it++)
	{
		CImageProcessor* ptr = *it;
		if(!ptr->IsModelImageReady())
			return false;
	}
	return true;
}

void CChannelControlPanel::ResetInternalStats()
{
	std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
	{
		pProcessor->ResetInternalStats();
	});

	m_ProBeginTime = 0;
	m_RejectTime = 0;

	m_nTotalCount = 0;
	m_nOldTotalCount = 0;
	m_nDefective  = 0;
	m_dictFI_EC.clear();
	m_oldDictFI_EC.clear();
	while(!m_stackNgIndex.empty())
	{
		m_stackNgIndex.pop();
	}
	m_chDeviceData.Reset();
	//////////////////////////////////////////////////////////////////////////
	m_statisticsBuffer.clear();
	m_nOldDefective = 0;
	m_statistics.clear();//数据统计

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

	while (!m_QueRecentErrCount.empty())
	{
		m_QueRecentErrCount.pop();
	}

	m_ngImgContainer.clear();
	if (m_nCameraCount!=0)
	{
		m_ngImgContainer = std::vector<vector<IMG_PRODUCT>>(IMAGE_ARRAY_SZ, std::vector<IMG_PRODUCT>(m_nCameraCount,IMG_PRODUCT()));
	}
	else
	{
		m_ngImgContainer = std::vector<vector<IMG_PRODUCT>>(IMAGE_ARRAY_SZ, std::vector<IMG_PRODUCT>(1,IMG_PRODUCT()));
	}
	UpdateStatistics();

	UpdateMainScreen(true,false,true);
}

void CChannelControlPanel::UpdateTaskSetting(eTASK_ACTION ta)
{
	ListAlgoGroup();
	if(ta == TA_DELETE)
	{
		if(IsChCameraRunning())
		{
			PauseChCamera();
			Sleep(20);
		}
		StopImgProcessor();
//		gSystemMgr.m_pciDevice.CleanNgSignal(m_channelConfig.m_chIndex);
		StartImgProcessor(true);
		SetChannelStatus();

		m_channelConfig = gConfigMgr.GetChannelConfig(m_channelConfig.m_chIndex);
		std::for_each(m_vProcessorPtr.begin(),m_vProcessorPtr.end(),[&](CImageProcessor* pProcessor)
		{
			pProcessor->SetChConfigPtr(&m_channelConfig);
		});
	}
}

void CChannelControlPanel::RefreshChannelPanel()
{
	if(IsChCameraRunning())
	{
		return;
	}
    m_vChError.clear();

	int chIndex         = m_channelConfig.m_chIndex;
	int CamereCountOld  = m_channelConfig.m_vecCameraSetting.size();
	m_channelConfig = gConfigMgr.GetChannelConfig(m_channelConfig.m_chIndex);

	int CamereCount = m_channelConfig.m_vecCameraSetting.size();
	StopImgProcessor();
    CreateInternalData();
    gSystemMgr.ReCreateAllPanels();
	
	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	if(pChConfigArray != nullptr)
	{
		m_channelConfig = (*pChConfigArray)[chIndex];
		if(!IsChCameraRunning())
		{
			m_chSD.ExecuteCmd(TEST_LC);
			m_chSD.ExecuteCmd(TEST_SOLENOID);			
		}
		SetChannelStatus();
		StartImgProcessor(true);
		if(CamereCount>0)
		{
			m_btnChDebug.EnableWindow(TRUE);
			TurnOnCameraAtSystemBoot();
		}
		else
			m_btnChDebug.EnableWindow(FALSE);
	}
}

void CChannelControlPanel::ReConnectChannelCamera()
{
	if(IsChCameraRunning())
	{
		return;
	}
	int chIndex     = m_channelConfig.m_chIndex;
	m_channelConfig = gConfigMgr.GetChannelConfig(m_channelConfig.m_chIndex);
	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	if(pChConfigArray != nullptr)
	{
		m_channelConfig = (*pChConfigArray)[chIndex];
		if(!IsChCameraRunning())
		{
			TurnOnCameraAtSystemBoot();
		}
		SetChannelStatus();
	}
}

void CChannelControlPanel::SendSignal(bool bOK)
{
	//	m_crtImgGroup

	if (m_nCameraCount == 4)
	{
		int i0= m_crtImgGroup[0].m_ImageID;
		int i1= m_crtImgGroup[1].m_ImageID;
		int i2= m_crtImgGroup[2].m_ImageID;
		int i3= m_crtImgGroup[3].m_ImageID;
		if(i0==i1&&i1==i2&&i2==i3)
		{

		}
		else
		{
			CString str;
			str.Format(_T("timeCount%d,%d,%d,%d"),i0,i1,i2,i3);
			//MessageBox(str);
			gLogger.QueueUpLog(str.GetBuffer(),true);
		}

	}
// 	if (m_nCameraCount == 1)
// 	{
// 			int i0= m_crtImgGroup[0].m_ImageID;
// 			CString str;
// 			str.Format(_T("timeCount%d"),i0);
// 			MessageBox(str);
// 	}

	//形成控件标记QData
	string strCmd;
	int MCount = 0; //标记条目数
	if (bOK) //好盖信号！
	{
		strCmd +="88 00";  
	}
	else
	{
		strCmd +="88 ff";
	}

	MCount++;//标记分类号条目+1
	string strHex;
	for(int i = 0 ; i < m_nCameraCount ;i++)
	{
		//Mhead
	
		string stri = boost::lexical_cast<string>(i);
		string StrCmdHead1 = string(" A")+stri+" ";;
		std::ostringstream ss1;
		ss1 << std::hex << std::setfill('0');
		ss1 << std::setw(8);
		ss1 << (unsigned int)(m_crtImgGroup[i].m_ImageTimeCount);
		
// 		CString str;
// 		str.Format(_T("m_crtImgGroup[i].m_ImageTimeCount:%u\n"),(unsigned int)m_crtImgGroup[i].m_ImageTimeCount);
// 		gLogger.QueueUpLog(str.GetString(),true);
		strHex = ss1.str();
		strHex.insert(2," ");
		strHex.insert(5," ");
		strHex.insert(8," ");

		strCmd+=StrCmdHead1;
		strCmd+=strHex;
		MCount++;
		////////////////////////////////////////////////////////////////////////////////
		string StrCmdHead2 = string(" B")+stri+" ";
		std::ostringstream ss2;
		ss2 << std::hex << std::setfill('0');
		ss2 << std::setw(8);
		ss2 << (unsigned int)(m_crtImgGroup[i].m_ImageID);
		strHex = ss2.str();
		strHex.insert(2," ");
		strHex.insert(5," ");
		strHex.insert(8," ");
		strCmd+=StrCmdHead2;
		strCmd+=strHex;
		MCount++;
		////////////////////////////////////////////////////////////////////////////////////////////
		string StrCmdHead3 = string(" C")+stri+" ";
		std::ostringstream ss3;
		ss3 << std::hex << std::setfill('0');
		ss3 << std::setw(8);
		ss3<< (unsigned int)(m_crtImgGroup[i].m_ImageTimeCount_PC);
		strHex = ss3.str();
		strHex.insert(2," ");
		strHex.insert(5," ");
		strHex.insert(8," ");
		strCmd+=StrCmdHead3;
		strCmd+=strHex;
		MCount++;
		/////////////////////////////////////for test///////////////////////////////////////////////////////
		string StrCmdHead4 = string(" D")+stri+" ";
		std::ostringstream ss4;
		ss4 << std::hex << std::setfill('0');
		ss4 << std::setw(8);
		ss4<< (unsigned int)(m_crtImgGroup[i].m_AlgoTimeCount);
		strHex = ss4.str();
		strHex.insert(2," ");
		strHex.insert(5," ");
		strHex.insert(8," ");
		strCmd+=StrCmdHead4;
		strCmd+=strHex;
		MCount++;
		/////////////////////////////////////for test///////////////////////////////////////////////////////
		string StrCmdHead5 = string(" E")+stri+" ";
		std::ostringstream ss5;
		ss5 << std::hex << std::setfill('0');
		ss5 << std::setw(8);
		ss5<< (unsigned int)(m_crtImgGroup[i].m_CollectFunInfoCount);
		strHex = ss5.str();
		strHex.insert(2," ");
		strHex.insert(5," ");
		strHex.insert(8," ");
		strCmd+=StrCmdHead5;
		strCmd+=strHex;
		MCount++;
	}

	MCount = 4;
	//QData Mcount + MFlag1 + MFlag2
	std::ostringstream ssMcount;
	ssMcount << std::hex << std::setfill('0');
	ssMcount << std::setw(2);
	ssMcount<< MCount;
	strHex = ssMcount.str();//Mcount
	strHex +=" 00"; //MFlag1
	strHex +=" 00"; //MFlag2
	strCmd=strHex+ string(" ")+strCmd;


// 	string strCmdQhead= "00 00 00 00 ";
// 	strCmd+=" 00"; // QFlags 标记位  3-1 恢复结果消息 4-1 QlifeID需要核对！
// 	strCmd+=" 05"; // Opcode Ox05 标记工件
// 
// 	std::ostringstream ssDlen;   //QDlen
// 	ssDlen << std::hex << std::setfill('0');
// 	ssDlen << std::setw(2);
// 	ssDlen << strCmd.length()/3+1;
// 	strHex = ssDlen.str();//Mcount

// 	CString str;
// 	str.Format(_T("Qdata: %s"),utils::Utf8ToUnicode(strCmd).c_str());//
//	gLogger.QueueUpLog(str.GetString(),true);

// 	if (!m_bRjectError)
// 	{
// 		m_vecRjectErrorLog[m_nTotalCount%100]=str;
// 	}
	
	m_chSD.ExecuteCmd(QOP_PRODUCT_FLAG,strCmd);

}
// 	LARGE_INTEGER now;
// 	QueryPerformanceCounter(&now);
// 
// 	m_nSyncFlag = gSystemMgr.DecreaseSyncFlag(m_channelConfig.m_chIndex);
// 
// //介于相机暂停时候回重置 m_nSyncFlag标记位，
// //在图片存在缓存队列的时候，m_nSyncFlag会出现小于0的情况，导致下面for 语句死循环
// 	if(m_nSyncFlag < 0 || m_nSyncFlag > 1)
// 	{
// 		wstring wstrNum = boost::lexical_cast<wstring>(m_nSyncFlag);
// 		gLogger.QueueUpLog(wstrNum,true);
// 		m_nSyncFlag = 0;
// 	    gSystemMgr.ResetSyncFlag(m_channelConfig.m_chIndex);
// 	}
// 
// 	double timenow = now.QuadPart * 1000.0f / m_freq.QuadPart;
// 
// 
// 	//保证两个排盖信号之间的时间间隔大于10ms
// 	if(timenow - m_RejectTime < 10)
// 	{
// 		timenow = m_RejectTime + 10;
// 	}
// 	
// 		if (timenow - m_ProBeginTime < m_RejectMinTime)
// 		{
// 			NG_IMG_SIGNAL signal(m_channelConfig.m_rejectorSetting.m_nRejectPort,m_ProBeginTime+m_RejectMinTime);
// 			gSystemMgr.m_pciDevice.ReceiveNgSignal(m_channelConfig.m_chIndex, signal);
// 		}
// 		else
// 		{
// 			NG_IMG_SIGNAL signal(m_channelConfig.m_rejectorSetting.m_nRejectPort,timenow);
// 			gSystemMgr.m_pciDevice.ReceiveNgSignal(m_channelConfig.m_chIndex, signal);
// 		}
// 
// 	m_RejectTime = timenow;
// 
// }
void CChannelControlPanel::UpdateMainScreen(bool bOk,bool bDebugImg,bool bforce)
{
	if(bDebugImg)
	{
		return;
	}
	double timenow = m_chronograph.DoubleNow();

	if(bforce)
	{
		m_bCurOk = true;
		CClientDC dc(this);
		CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);

		CPen* pOldPen ;
		pOldPen = dc.SelectObject(&penFrame);
		dc.Rectangle(m_Rect1);
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);

		m_LastErrorTime = 0;
		return ;
	}

	if(bOk)
	{
		if (timenow - m_LastErrorTime <= 100)
		{
			return;
		}
	}
	m_LastErrorTime = timenow;

	if (bOk && !m_bCurOk)
	{
		CClientDC dc(this);
		CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		
		CPen* pOldPen ;
		pOldPen = dc.SelectObject(&penFrame);
		dc.Rectangle(m_Rect1);
		m_bCurOk = !m_bCurOk;
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);
	}
	else if (!bOk &&m_bCurOk)
	{
		CClientDC dc(this);
		CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);

		CPen* pOldPen ;
		pOldPen = dc.SelectObject(&penFrame1);
		dc.Rectangle(m_Rect1);
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);

		m_bCurOk = !m_bCurOk;
	}
}
void CChannelControlPanel::SendRejectSignal_Ex(bool bOk,bool bDebugImg,int nImgBufferSize)
{
	if (m_mainchannel!=0)
	{
		gSystemMgr.GetPanel(m_mainchannel)->SendRejectSignal(bOk,bDebugImg,nImgBufferSize);
	}
	else
	{
		SendRejectSignal(bOk,bDebugImg,nImgBufferSize);
	}
}
void CChannelControlPanel::SendRejectSignal(bool bOk,bool bDebugImg,int nImgBufferSize)
{
	if(bDebugImg)
	{
		return;
	}
	static boost::chrono::system_clock::time_point time_send;
	if(time_send.time_since_epoch().count()>0)
	{
		auto now_time=boost::chrono::system_clock::now();
		auto diff= now_time - time_send;
		time_send = now_time;
		if(diff.count() /10000.0 > 150)
		{
			std::wostringstream wstr;
			wstr<<L"channel "<< m_chIndex << L"send signal interval time greater than 150ms";
			//gLogger.QueueUpLog(wstr.str());
			wstr.str(L"");
		}
	}
	else
		time_send=boost::chrono::system_clock::now();

	//跨通道排盖的排盖信号汇总
	if (m_mainchannel!=0 || m_subchannel!=0)
	{
// 		CString str;
// 		str.Format(_T("%d,%d,%d\n"),m_chIndex,m_nTotalCount,bOk);

		if(m_subchannel!=0)
		{
			if (m_dictSignalMap.find(m_nTotalCount)!=m_dictSignalMap.end())
			{
// 				CString str;
// 				str.Format(_T("%d,%d = %d\n"),m_dictSignalMap[m_nTotalCount],bOk,bOk&&m_dictSignalMap[m_nTotalCount]);
// 				OutputDebugString(str);
				bOk &= m_dictSignalMap[m_nTotalCount];	
			}
			else
			{
				m_dictSignalMap.clear();
				m_dictSignalMap[m_nTotalCount]=bOk;
				m_nSyncFlag = gSystemMgr.DecreaseSyncFlag(m_channelConfig.m_chIndex);
				return;
			}
		}
	}

	if(!m_channelConfig.m_rejectorSetting.m_bTestRejector)
	{
		m_nSyncFlag = gSystemMgr.DecreaseSyncFlag(m_channelConfig.m_chIndex);
		SendSignal(bOk);
	}
	else
	{
		SendRejectorTestingSignal(bOk);
	}

	if(m_nSyncFlag < 0 || m_nSyncFlag > 1)
	{
// 		wstring wstrNum = boost::lexical_cast<wstring>(m_nSyncFlag);
// 		gLogger.QueueUpLog(wstrNum,true);
	//	m_nSyncFlag = 0;
		gSystemMgr.ResetSyncFlag(m_channelConfig.m_chIndex);
		int nImageHeapUp=0;//boost::lexical_cast<int>(gConfigMgr.GetBasicSetting(_T("IMAGE_HEAP_UP")).c_str());
		if(nImageHeapUp<1) nImageHeapUp=10;
		if (nImgBufferSize > nImageHeapUp)
		{
			CString streMsg(_T("算子处理时间过长,图片积压"));
			ERROR_INFO lcPortError(INIT_REJECTOR_SYS_FAILED,ERRORSUBID_CAMERA_RJECT_UNKNOW_ERROR,streMsg.GetString());
			AddChError(lcPortError);
		}
	}

}

void CChannelControlPanel::SendRejectorTestingSignal(bool bOk)
{
	int nRejectorTestMode = m_channelConfig.m_rejectorSetting.m_nTestMode;

	bool bSend = false;
	if(nRejectorTestMode == 0) // Reject All
	{
		bSend = true;
	}	
	else if(nRejectorTestMode == 3) // Interval Reject
	{
		int nNumerator   = m_channelConfig.m_rejectorSetting.m_nNumerator;
		int nDemonimator = m_channelConfig.m_rejectorSetting.m_nDenominator;
		int nInterval    = nNumerator + nDemonimator;
		int index = m_nTotalCount - m_nIntervalCount * nInterval;

		if(!m_bLaunchSignal && index % nNumerator == 0)
		{
			m_bLaunchSignal = true;
			gSystemMgr.DecreaseSyncFlag(m_channelConfig.m_chIndex);
			return;
		}
		if(m_bLaunchSignal)
		{
			if(index < nNumerator)
			{
				bSend = false;
			}
			else if(index > nNumerator && index < nInterval)
			{
				bSend = true;
			}
			else if(index == nInterval)
			{
				bSend = true;
				m_nIntervalCount++;
				m_bLaunchSignal = false;
			}
			else
			{
			}
		}
	}
	else if(nRejectorTestMode == 1) // Reject odd ones
	{
		bSend = false;
	}
	else if(nRejectorTestMode == 2) // Reject even ones
	{
		bSend = bOk;
	}
	else
	{
		//Nothing
	}

	SendSignal(!bSend);
	m_nSyncFlag = gSystemMgr.DecreaseSyncFlag(m_channelConfig.m_chIndex);
}

void CChannelControlPanel::SetChannelImageWindowPtr(CChannelConfigDlg* pWnd)
{
	m_pChConfigWnd = (void*)pWnd;
}

void CChannelControlPanel::SetDispAllImageFlag(bool bDispAllImage)
{
	m_bDispAllImg = bDispAllImage;
}

void CChannelControlPanel::SetDebugFlag(bool bDebug)
{
	m_bDebugMode = bDebug;
}

void CChannelControlPanel::DisPlayModelImage(int StationIndex,Hobject crtImg,Hobject retRgn)
{
	CChannelConfigDlg* pWnd = (CChannelConfigDlg*)(void*)m_pChConfigWnd;
	if(pWnd != nullptr)
	{
		pWnd->DisPlayModelImage(StationIndex,crtImg,retRgn);
	}
}

void CChannelControlPanel::UpdateChConfigScreen(bool bOk, IMAGE_BOX& crtImg)
{
	CChannelConfigDlg* pWnd = (CChannelConfigDlg*)(void*)m_pChConfigWnd;
	if(pWnd != nullptr)
	{
		int debugFlag = crtImg.m_bDebugImg;
		if(m_bDebugMode)
		{
			if (debugFlag != 1)
			{
				return;
			}
			debugFlag = 1;
			pWnd->SetAlgoListStats(m_vFuncInfoDebug);
			pWnd ->SetCheckResult(m_crtImgGroup_Debug);
			pWnd ->SetDisplayResult(m_crtDisplayResult_Debug);
			::PostMessage(pWnd->m_hWnd, WM_UPDATE_ALGO_LIST, debugFlag, 0);
		}
		else
		{
			bool bDisplay = false;
			LARGE_INTEGER bgnStamp;
			QueryPerformanceCounter(&bgnStamp);
			if((bgnStamp.QuadPart - m_timestamp.QuadPart) * 1000.0f / m_freq.QuadPart > 333.3f)
			{
				bDisplay = true;
				m_timestamp = bgnStamp;
			}
			if(!bDisplay)
			{
				return;
			}
			if(m_bDispAllImg)//
			{
				pWnd->SetAlgoListStats(m_vFuncInfo);
				pWnd ->SetCheckResult(m_crtImgGroup);
				pWnd ->SetDisplayResult(m_crtDisplayResult);
				::PostMessage(pWnd->m_hWnd, WM_UPDATE_ALGO_LIST, debugFlag, 0);
			}
			else
			{
				if(!bOk)
				{
					pWnd->SetAlgoListStats(m_vFuncInfo);
					pWnd ->SetCheckResult(m_crtImgGroup);
					pWnd ->SetDisplayResult(m_crtDisplayResult);
					::PostMessage(pWnd->m_hWnd, WM_UPDATE_ALGO_LIST, debugFlag, 0);
				}
			}
		}
	}
}
void CChannelControlPanel::SetProBeginTime()
{
	m_nCheckBeginArrivalCount++;
	double now = m_chronograph.DoubleNow();

	if (m_nCheckBeginArrivalCount ==m_nCameraCount)
	{
		m_ProBeginTime = now;
		m_nCheckBeginArrivalCount =0;
	}
}

void CChannelControlPanel::SendDIIPAndSyncFlag()
{
	int nFlag = ((m_nImgIndex % 2 == 0) ? IO_ON : IO_OFF);
//	gSystemMgr.m_pciDevice.WritePort(m_channelConfig.m_rejectorSetting.m_nDropImgPort,nFlag);
	long		m_nSyncFlag = gSystemMgr.IncreaseSyncFlag(m_channelConfig.m_chIndex);
// 	CString str;
// 	str.Format(_T("  Send     m_nTotalCount:%d  m_nSyncFlag:%d!\n"),m_nTotalCount,m_nSyncFlag);
// 	OutputDebugString(str);

	m_nImgIndex++;
}

void CChannelControlPanel::GetArriveTime(LARGE_INTEGER time)
{
	m_ArriveTime = time;
}

void CChannelControlPanel::ExecuteCmd(int nCmdID)
{
	//扩展性测试，原来代码不报错！
	return;
}

void CChannelControlPanel::ExecuteCmd(int QOpCmdID,int nCmdID, int nValue)
{
	m_chSD.ExecuteCmd(QOpCmdID,nCmdID, nValue);
}
void CChannelControlPanel::ExecuteCmd(int QOpCmdID,string strCmd)
{
	m_chSD.ExecuteCmd(QOpCmdID,strCmd);
}

BOOL CChannelControlPanel::GetIsInterlock(){
	return m_chDeviceData.m_bInterLock;
}