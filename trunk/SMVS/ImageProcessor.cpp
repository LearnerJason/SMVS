#include "StdAfx.h"
#include "resource.h"
#include "ImageProcessor.h"
#include "ChannelControlPanel.h"
#include "ChannelConfigDlg.h"
#include "ChannelControlPanel.h"
#include <atlconv.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include "SystemManager.h"
#include "Logger.h"
#include "SecretaryDevice.h"

#define MIX_LABEL_ALGO_NEW

extern CSystemManager gSystemMgr;

boost::mutex CImageProcessor::_image_mu::mt_bottle;
boost::condition_variable CImageProcessor::_image_mu::cv_bottle;
bool CImageProcessor::_image_mu::bOk=false;

using namespace std;
using namespace boost;
using namespace boost::chrono;

CImageProcessor::CImageProcessor()
	:BUFFER_SZ(32),
	m_bProcessorOk(false),
	ERROR_IMG_BUFFER_SZ(64),
	m_pStationOwner(nullptr),
	m_pChSecretaryDevice(nullptr),
	m_bChWndOpened(false),
	m_imgIndex(-1),
	m_imgBuffer(BUFFER_SZ),
	m_fCrtModelScore(0.0f),
	m_bModelImgReady(false),
	m_bDispErrRgn(true),
	m_bFirstImg(true),
	m_pParamDataCs(nullptr),
	m_pChConfig(nullptr),
	m_pChLogger(nullptr),
	m_bHasInnerStatsIt(false),
	m_crtSelectedFuncIndex(-1),
	m_nCrtProductID(-1),
	m_cameraGroupIndex(-1),
	m_nCompoundRgnID(-1),
	m_glueRgnFuncID(-1),
	m_compoundStatsFuncID(-1),
	m_width(-1),
	m_height(-1)
{
	m_vModelImage.reserve(64);

#ifdef CROWN_PRODUCT
	CComPtr<ICrownCapAlgo> pCrownCapIt;
	boost::get<0>(m_algoArray) = pCrownCapIt;
#endif

#ifdef EOE_PRODUCT
	CComPtr<IEoeAlgo> pEoeAlgoIt;
	boost::get<0>(m_algoArray) = pEoeAlgoIt;
#endif

#ifdef CAN_PRODUCT
	CComPtr<ICanAlgo> pCanAlgoIt;
	boost::get<0>(m_algoArray) = pCanAlgoIt;
#endif

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		CComPtr<IEoeAlgo> pEoeAlgoIt;
		boost::get<0>(m_algoArray) = pEoeAlgoIt;
	}
	if(m_nCrtProductID == CAN)
	{
		CComPtr<ICanAlgo> pCanAlgoIt;
		boost::get<1>(m_algoArray) = pCanAlgoIt;
	}
#endif

#ifdef PET_PRODUCT
	CComPtr<IPetAlgo> pPetAlgoIt;
	boost::get<0>(m_algoArray) = pPetAlgoIt;
#endif

#ifdef JELLY_PRODUCT
	CComPtr<IJellyAlgo> pJellyAlgoIt;
	boost::get<0>(m_algoArray) = pJellyAlgoIt;
#endif

#ifdef PCC_PRODUCT
	CComPtr<IPccAlgo> pPccAlgoIt;
	boost::get<0>(m_algoArray) = pPccAlgoIt;
#endif

#ifdef CAP_PRODUCT
	CComPtr<ICapacitorAlgo> pCapacitorAlgoIt;
	boost::get<0>(m_algoArray) = pCapacitorAlgoIt;
#endif

#ifdef MAT_PRODUCT
	CComPtr<IRubberMatAlgo> pRubberMatAlgoIt;
	boost::get<0>(m_algoArray) = pRubberMatAlgoIt;
#endif

#ifdef NEEDLE_PRODUCT
	CComPtr<INeedleAlgo> pNeedleAlgoIt;
	boost::get<0>(m_algoArray) = pNeedleAlgoIt;
#endif

#ifdef PLUG_PRODUCT
	CComPtr<IPlugAlgo> pPlugAlgoIt;
	boost::get<0>(m_algoArray) = pPlugAlgoIt;
#endif

#ifdef HDPE_PRODUCT
	CComPtr<IHdpeAlgo> pHdpeAlgoIt;
	boost::get<0>(m_algoArray) = pHdpeAlgoIt;
#endif


#ifdef CAN_OUTSIDE_PRODUCT
	CComPtr<ICanOutsideAlgo> pCanOutsideAlgoIt;
	boost::get<0>(m_algoArray) = pCanOutsideAlgoIt;
#endif

#ifdef NAIL_PRODUCT
	CComPtr<INailAlgo>  pNailAlgoIt;
	boost::get<0>(m_algoArray) = pNailAlgoIt;
#endif

#ifdef BATTERY_PRODUCT
	CComPtr<IBatteryShellAlgo>  pBatteryAlgoIt;
	boost::get<0>(m_algoArray) = pBatteryAlgoIt;
#endif

#ifdef BOTTLE_PRODUCT
	CComPtr<IBottleCapAlgo>  pBottleCapAlgoIt;
	boost::get<0>(m_algoArray) = pBottleCapAlgoIt;
#endif

#ifdef AL_PLASTIC_PRODUCT
	CComPtr<IAlPlasticCapAlgo>  pAlPlasticCapAlgoIt;
	boost::get<0>(m_algoArray) = pAlPlasticCapAlgoIt;
#endif
}

CImageProcessor::~CImageProcessor(void)
{
}

void CImageProcessor::SetStationIndex(int index)
{
	m_cameraGroupIndex = index;
}

void CImageProcessor::SetChannelWindowOpenedFlag(bool bOpened)
{
	m_bChWndOpened  = true;
}

void CImageProcessor::UpdatedictBtnID_File(wstring& pstr,int index)
{
	wostringstream oss;  
	oss<<L"#"<<index;
	std::wstring wstr=oss.str(); 

	if (pstr.find(_T(".shm"))!= wstring::npos)
	{
		int i = pstr.find(_T("#"));
		int j = pstr.find_last_of(_T("#"));
		if (i != string::npos && j != string::npos)
		{
			pstr.replace(j,2,wstr);
			pstr.replace(i,2,wstr);
		}
	}
	if (pstr.find(_T(".reg"))!= wstring::npos)
	{
		int i = pstr.find(_T("#"));
		if (i != string::npos )
		{
			pstr.replace(i,2,wstr);
		}
	}
}

void CImageProcessor::UpdateImageProcessFuncInfo(std::vector<FUNC_INFO> *vFuncInfo)
{
	if (!m_pChConfig->m_bModelfiles)
	{
		m_vFuncInfo = *vFuncInfo;
		m_vFuncInfoDebug = *vFuncInfo;

		//////add by hzh
		std::vector<FUNC_INFO> funInfotmp = *vFuncInfo;
		BOOL bHaveGlueFunc=false;
		for (int i = 0; i < funInfotmp.size(); i++)
		{
			if(Check_GlueInjectorAvailability(funInfotmp[i].m_strFuncName))
				bHaveGlueFunc=true;
		}
		if(!bHaveGlueFunc)
			m_compoundStatsFuncID=-1;
	}
	else
	{
		BOOL bHaveGlueFunc=false;
		std::vector<FUNC_INFO> funInfotmp = *vFuncInfo;
		for (int i = 0; i < funInfotmp.size(); i++)
		{
			if(Check_GlueInjectorAvailability(funInfotmp[i].m_strFuncName))
				bHaveGlueFunc=true;
			for (int j = 0 ; j< funInfotmp[i].m_vParam.size();j++)
			{
				CString str = funInfotmp[i].m_vParam[j].m_wstrFile.c_str();
				UpdatedictBtnID_File(funInfotmp[i].m_vParam[j].m_wstrFile,m_cameraGroupIndex);
// 				if (!funInfotmp[i].m_vParam[j].m_wstrFile.empty())
// 				{
// 					OutputDebugString(_T("\n"));
// 					OutputDebugString(funInfotmp[i].m_vParam[j].m_wstrFile.c_str());
// 					OutputDebugString(_T("\n"));
// 				}
				funInfotmp[i].CreateComParamArray();
			}
		}

		m_vFuncInfo      = funInfotmp;
		m_vFuncInfoDebug = funInfotmp;

		if(!bHaveGlueFunc)
			m_compoundStatsFuncID=-1;
	}

}

void CImageProcessor::SetChLoggerPtr(CLogger* pLogger)
{
	m_pChLogger = pLogger;
}

void CImageProcessor::SetImgSize(const IMG_SZ& imgSz)
{
	m_imgSz = imgSz;
}

void CImageProcessor::SetChOwnerWindowPtr(CChannelControlPanel* pOwnerWnd)
{
	m_pStationOwner = pOwnerWnd;
}

void CImageProcessor::SetChSecretaryDevicePtr(CSecretaryDevice* pDevice)
{
	m_pChSecretaryDevice = pDevice;
}

void CImageProcessor::SetTask(TASK task)
{
	if (m_pChConfig->m_bModelfiles)
	{
		task.AdaptModelFiles(m_crtTask,m_cameraGroupIndex);
	}
	else
	{
		m_crtTask = task;
	}
}

void CImageProcessor::SetAlgoParamCriticalSecPtr(CRITICAL_SECTION* pCs)
{
	m_pParamDataCs = pCs;
}

void CImageProcessor::SetChConfigPtr(CHANNEL_CONFIG* pChConfig)
{
	m_pChConfig = pChConfig;
	TASK* pCrtTask = pChConfig->GetCurrentTask();
	if(pCrtTask == nullptr)
	{
		return;
	}
	if (pChConfig->m_bModelfiles)
	{
		pCrtTask->AdaptModelFiles(m_crtTask,m_cameraGroupIndex);
	}
	else
	{
		m_crtTask = *pCrtTask;
	}



	if(pChConfig != nullptr)
	{
		m_imgIndex = -1;
	}

	if(m_pStationOwner != NULL)
	{
		m_pStationOwner->SetBlowThresholdValue(m_crtTask.m_blowThreshold);
	}

}

void CImageProcessor::ReceiveImageEx(IMAGE_BOX& image,int mutAcqIdFlag)
{
	if (m_pStationOwner!=nullptr)
	{
		if (mutAcqIdFlag ==0) 
		{
			ReceiveImage(image);
		}
		else
		{
			//此处可以适当优化
			if (m_pStationOwner->m_subchannel==0)
			{
				ReceiveImage(image);
			}
			else
			{
				gSystemMgr.GetPanel(m_pStationOwner->m_subchannel)->m_vProcessorPtr[0]->ReceiveImage(image);
			}
		}
	}
}
void CImageProcessor::ReceiveImage(IMAGE_BOX& image)
{
	if(!m_bProcessorOk)
	{
		return;
	}
	if(m_imgBuffer.is_not_full())
	{
		if(!image.m_bLastFlag && m_crtTask.m_bSelfLearning && m_imgIndex >= m_crtTask.m_nSampleCount && !image.m_bDebugImg)
		{
			return;
		}
		if(!image.m_bDebugImg)
		{
			m_imgBuffer.push_front(image);
		}
		else
		{
			m_imgBuffer.push_back(image);
		}

	}
	else  // make sure that the last image is processed.
	{
		if(image.m_bLastFlag)
		{
			IMAGE_BOX crtImg;
			m_imgBuffer.pop_back(&crtImg);
			m_imgBuffer.push_back(image);
		}
		else if(image.m_bDebugImg)
		{
			IMAGE_BOX crtImg;
			m_imgBuffer.pop_back(&crtImg);
			m_imgBuffer.push_back(image);
		}
		else
		{
			m_pChLogger->QueueUpLog(L"Image lost!");
		}
	}
}

bool CImageProcessor::ProduceFuncInfoList(wstring strHelp,IT_PARAM& itParams, FUNC_INFO& funcInfo)
{
	wstring funcName = itParams.m_itName;
	CComBSTR bstrFuncName(funcName.c_str());
	HRESULT hr = FALSE;
#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
		hr = m_algoArray.get<0>()->GetIDsOfNames(IID_NULL, &bstrFuncName, 1, LOCALE_SYSTEM_DEFAULT, &funcInfo.m_dispId); 
	else
		hr = m_algoArray.get<1>()->GetIDsOfNames(IID_NULL, &bstrFuncName, 1, LOCALE_SYSTEM_DEFAULT, &funcInfo.m_dispId); 
#else
	hr = m_algoArray.get<0>()->GetIDsOfNames(IID_NULL, &bstrFuncName, 1, LOCALE_SYSTEM_DEFAULT, &funcInfo.m_dispId);
#endif

	if(!utils::CheckResult(hr))
		return false;

	funcInfo.m_strFuncName = itParams.m_itName;
	funcInfo.m_funcIndex = boost::lexical_cast<int>(itParams.m_itIndex);
	funcInfo.m_bEnable   = itParams.m_bEnable;

	std::vector<wstring> vValue = itParams.m_vParam;
	size_t szValue = vValue.size();
	int index = -1;
	std::vector<wstring> vDesc = utils::SplitString(strHelp, L"$");

	bool m_bAgloError = false;
	if (strHelp.find(L";R")==wstring::npos)
	{
		if (szValue != vDesc.size())
		{
			m_bAgloError = true;
		}
	}
	else
	{
		if (szValue != vDesc.size()-1)
		{
			m_bAgloError = true;
		}
	}
	if (m_bAgloError)
	{
		CString strAlgoParam;
		for (int ii = 0; ii < itParams.m_vParam.size();++ii )
		{
			CString tmp;
			tmp.Format(_T("%s"),itParams.m_vParam[ii].c_str());
			if (ii != itParams.m_vParam.size()-1)
			{
				tmp+=_T(",");
			}
			strAlgoParam+=tmp;
		}
		CString strAlgoError;
		strAlgoError.Format(_T("CH%d:Error In ID=%d Name<%s>\nAlgoParam: %s\nAlgoHelp:%s"),m_pStationOwner->m_chIndex,funcInfo.m_funcIndex, funcInfo.m_strFuncName.c_str(),strAlgoParam,strHelp.c_str());
		AfxMessageBox(strAlgoError);
		return false;
	}


	CString strTmp;
	if(vDesc.size() > 0)
	{
		for(size_t i = 0; i < vDesc.size();++i)
		{
			wstring str = vDesc[i];
			vector<wstring> vInfo = utils::SplitString(str,L";");
			utils::TrimString(vInfo);
			size_t sz = vInfo.size();
			if(sz >= 3 && vInfo[1] == L"R")
			{
				funcInfo.m_bTestFunc = true;
				funcInfo.m_threshold.m_type = utils::GetType(vInfo[2]);
				funcInfo.m_threshold.m_helpId = boost::lexical_cast<long>(vInfo[0]);
				if(!utils::ParseThresholdSpec(funcInfo.m_threshold, vValue[0]))
				{
					//TODO:something wrong with the algorithm section of configuration file
					// log this error
					return false;
				}
				if(sz == 7) // Parse range restriction of the spec value
				{
					funcInfo.m_threshold.m_min     = boost::lexical_cast<float>(vInfo[3]);
					funcInfo.m_threshold.m_max     = boost::lexical_cast<float>(vInfo[4]);
					funcInfo.m_threshold.m_stepLen = boost::lexical_cast<float>(vInfo[5]);
				}
			}
			else
			{
				if(sz == 1) // function help string ID
				{
					funcInfo.m_strCustomizedName = itParams.m_itNickname;
					funcInfo.m_funcDescId = boost::lexical_cast<long>(vInfo[0].c_str());
					strTmp.LoadString(funcInfo.m_funcDescId);
					wstring strHelp = strTmp.GetString();
					std::vector<wstring> vStr = utils::SplitString(strHelp, L"$");
					utils::TrimString(vStr);
					size_t strSz = vStr.size();
					if(strSz == 1)
					{
						funcInfo.m_strHelp = vStr[0];
					}
					if(strSz == 2)
					{
						funcInfo.m_strOutputName = vStr[0];
						funcInfo.m_strHelp       = vStr[1];
					}
				}
				if(sz == 2 && vInfo[1] == L"B")
				{
					index = 1;
					INPUT_PARAM param;
					param.m_descID = boost::lexical_cast<long>(vInfo[0]);
					param.m_type   = utils::GetType(vInfo[1]);
					if(index >= szValue) return false;
					param.m_value  = utils::ConvertStr2Value(vValue[index]);
					funcInfo.m_vParam.push_back(param);
					index++;
				}
				if(sz == 2 && vInfo[1].find(L"SP") != wstring::npos)
				{
					INPUT_PARAM param;
					param.m_descID   = boost::lexical_cast<long>(vInfo[0]);
					param.m_type     = utils::GetType(vInfo[1]);
					if(index >= szValue) return false;
					param.m_wstrFile = vValue[index];
					param.m_nFileType = ((vInfo[1] == L"SPM")? 0 : 1);
					funcInfo.m_vParam.push_back(param);
					index++;
				}
				if(sz == 6)
				{
					INPUT_PARAM param;
					param.m_descID = boost::lexical_cast<long>(vInfo[0]);
					param.m_type = utils::GetType(vInfo[1]);
					if(index >= szValue) return false;
					param.m_value = utils::ConvertStr2Value(vValue[index]);
					param.m_min = boost::lexical_cast<float>(vInfo[2]);
					param.m_max = boost::lexical_cast<float>(vInfo[3]);
					param.m_step = boost::lexical_cast<float>(vInfo[4]);
					funcInfo.m_vParam.push_back(param);
					index++;
				}
			}
		}
	}
	return funcInfo.CreateComParamArray();
}

BOOL CImageProcessor::Check_GlueInjectorAvailability(std::wstring wName)
{
	if(wName == L"GlueInjectorIDDetectAlgo"||wName == L"GlueInjectorIDDetectAlgo1")
	{
#ifdef EOE_PRODUCT
		m_compoundStatsFuncID = utils::GetInterfaceDispID(m_algoArray.get<0>(), wName);
		m_glueRgnFuncID = utils::GetInterfaceDispID(m_algoArray.get<0>(), wName);
		//m_nCompoundRgnID      = retFuncInfo.GetTargetRegionID();
		return true;
#elif CAN_EOE_PRODUCT
		if(m_nCrtProductID == EOE)
		{
			m_compoundStatsFuncID = utils::GetInterfaceDispID(m_algoArray.get<0>(), wName);
			m_glueRgnFuncID = utils::GetInterfaceDispID(m_algoArray.get<0>(), wName);
			//m_nCompoundRgnID      = retFuncInfo.GetTargetRegionID();
		}
		return true;
#endif
	}
	return false;
}

bool CImageProcessor::Init_ImageChecking(LPVOID pInstance)
{
#ifdef CAN_OUTSIDE_PRODUCT
	m_algoArray.get<0>()->SetChannelIndex(m_pChConfig->m_chIndex);
	m_algoArray.get<0>()->SetCommonParameters(
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.StandardImg,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.FlagModelRegion,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.SeamRegionInStdImg,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.StdModelRegion,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.StitchedImgWidthUnion,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.ModelIDs,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.ModelRows,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.ModelCols,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.FlagModelID,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.SortedFlagColsInStdImg,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.StitchingParamCol,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.CanRegWidth,
		(LONG*)&gSystemMgr.m_canOutsideCommonParam.MappedImgWidths);
#endif

	auto ptr = (CImageProcessor*)pInstance;
	std::vector<IT_PARAM> vItParam = ptr->m_crtTask.m_vItParam;

	wstring strHelp;
	HRESULT hr = S_OK;

	for(size_t t = 0; t < vItParam.size(); t++)
	{
		IT_PARAM it_param = vItParam[t];

		wstring funcName = it_param.m_itName;
#ifdef CAN_EOE_PRODUCT
		if(m_nCrtProductID == EOE)
			hr = utils::GetInterfaceHelpString(m_algoArray.get<0>(),funcName, strHelp);
		else
			hr = utils::GetInterfaceHelpString(m_algoArray.get<1>(),funcName, strHelp);

#else
		hr = utils::GetInterfaceHelpString(m_algoArray.get<0>(),funcName, strHelp);
#endif
		if(utils::CheckResult(hr))
		{
			FUNC_INFO retFuncInfo;
			if(ProduceFuncInfoList(strHelp, it_param, retFuncInfo))
			{
				m_vFuncInfo.push_back(retFuncInfo);

				if(Check_GlueInjectorAvailability(it_param.m_itName))
					m_nCompoundRgnID      = retFuncInfo.GetTargetRegionID();

			}
			else//Message box or update the log file and to take down which interface is invalid?
			{
				m_vFuncInfo.clear();
				return false;
			}
// 			if(it_param.m_itName == L"GlueInjectorIDDetectAlgo"||it_param.m_itName == L"GlueInjectorIDDetectAlgo1")
// 			{
// 				m_glueRgnFuncID = utils::GetInterfaceDispID(m_algoArray.get<0>(), it_param.m_itName);
// 			}
		}
		else //There is no help information for XXXXX algorithm
		{
			CString strAlgoError;
			strAlgoError.Format(_T("CH%d:Algo  <%s> does not exist!"),m_pStationOwner->m_chIndex,funcName.c_str());
			AfxMessageBox(strAlgoError);
			m_vFuncInfo.clear();
			return false;
		}
	}
	if (m_cameraGroupIndex == 0)
	{
		m_pStationOwner->m_vFuncInfo.assign(m_vFuncInfo.begin(), m_vFuncInfo.end());
		m_pStationOwner->m_vFuncInfoDebug.assign(m_vFuncInfo.begin(), m_vFuncInfo.end());
	}
	m_vFuncInfoDebug.assign(m_vFuncInfo.begin(), m_vFuncInfo.end());

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
		m_bHasInnerStatsIt = utils::IsInterfaceSupported(m_algoArray.get<0>(), L"GetInternalStats");
	else
		m_bHasInnerStatsIt = utils::IsInterfaceSupported(m_algoArray.get<1>(), L"GetInternalStats");
#else
	m_bHasInnerStatsIt = utils::IsInterfaceSupported(m_algoArray.get<0>(), L"GetInternalStats");
#endif

	return true;
}
void CImageProcessor::SetRunImageSize(int Width,int Height)
{
	m_width = Width;
	m_height = Height;
}
void CImageProcessor::PilotRunImageProcessor()
{
	int width=0;
	int height=0;
	if (m_width == -1 && m_height == -1)
	{
		width  = m_pChConfig->m_uiSetting.m_width;
		height = m_pChConfig->m_uiSetting.m_height;
	}
	else
	{
		width  = m_width;
		height = m_height;
	}
	Hobject  initIimage;
	std::vector<unsigned char>  image(height * width, 0);
	for(int r = 0; r < height; r++)
	{
		for(int c = 0; c < width; c++)
			image[r * width + c] = 10 + r % 48 + c % 32;
	}
	gen_image1(&initIimage,"byte",width,height,(Hlong)&image[0]);

	set_grayval(initIimage, 0, 0, (int)'p');
	set_grayval(initIimage, 1, 0, (int)'i');
	set_grayval(initIimage, 2, 0, (int)'l');
	set_grayval(initIimage, 3, 0, (int)'o');
	set_grayval(initIimage, 4, 0, (int)'t');

	IMAGE_BOX firstImg;
	firstImg.m_img = initIimage;
	firstImg.m_bDebugImg  = true;
	firstImg.m_bSwitchImg = true;
	ReceiveImage(firstImg);
}

unsigned int CImageProcessor::ImageCheckingThread()
{
	m_vFuncInfo.clear();
	m_vFuncInfoDebug.clear();

	m_bProcessorOk = false;
	HRESULT hr = ::CoInitialize(NULL);
	if(SUCCEEDED(hr))
	{
#ifdef CROWN_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(CrownCapAlgo));
#endif

#ifdef EOE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(EoeAlgo));
#endif

#ifdef CAN_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(CanAlgo));
#endif

#ifdef CAN_EOE_PRODUCT
		if(m_nCrtProductID == EOE)
			hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(EoeAlgo));
		if(m_nCrtProductID == CAN)
			hr = m_algoArray.get<1>().CoCreateInstance(__uuidof(CanAlgo));
#endif

#ifdef	PET_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(PetAlgo));
#endif



#ifdef JELLY_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(JellyAlgo));
#endif

#ifdef PCC_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(PccAlgo));
#endif
	
#ifdef CAP_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(CapacitorAlgo));
#endif

#ifdef MAT_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(RubberMatAlgo));
#endif

#ifdef NEEDLE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(NeedleAlgo));
#endif

#ifdef PLUG_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(PlugAlgo));
#endif

#ifdef HDPE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(HdpeAlgo));
#endif


#ifdef CAN_OUTSIDE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(CanOutsideAlgo));
#endif

#ifdef NAIL_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(NailAlgo));
#endif

#ifdef BATTERY_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(BatteryShellAlgo));
#endif

#ifdef BOTTLE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(BottleCapAlgo));
#endif

#ifdef AL_PLASTIC_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(AlPlasticCapAlgo));
#endif

		Sleep(200);
		if(!utils::CheckResult(hr) || !Init_ImageChecking(this))
		{

#ifdef CAN_EOE_PRODUCT
			if(m_nCrtProductID == EOE)
				m_algoArray.get<0>().Release();
			if(m_nCrtProductID == CAN)
				m_algoArray.get<1>().Release();
#else
			m_algoArray.get<0>().Release();
#endif
			ReportStationStatus2ChUi(false);
			::CoUninitialize();
			return -1;
		}
		
		m_bProcessorOk = true;
		ReportStationStatus2ChUi(true);
		PilotRunImageProcessor();

		CComBSTR bstrTaskName(m_crtTask.m_wstrTaskName.c_str());

#ifdef CAN_PRODUCT
		CString strchName;
		CComBSTR bstrChName;
		if (m_pStationOwner != NULL)
		{
			strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
			bstrChName = strchName.AllocSysString();
		}
		m_algoArray.get<0>()->SetCurrentTaskName(&bstrTaskName,&bstrChName);

#elif HDPE_PRODUCT
		CString strchName;
		CComBSTR bstrChName;
		if (m_pStationOwner != NULL)
		{
			strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
			bstrChName = strchName.AllocSysString();
		}
		m_algoArray.get<0>()->SetCurrentTaskName(&bstrTaskName,&bstrChName);

#elif AL_PLASTIC_PRODUCT
		CString strchName;
		CComBSTR bstrChName;
		if (m_pStationOwner != NULL)
		{
			strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
			bstrChName = strchName.AllocSysString();
		}
		m_algoArray.get<0>()->SetCurrentTaskName(&bstrChName);
#elif CAN_EOE_PRODUCT

		CString strchName;
		CComBSTR bstrChName;
		if (m_pStationOwner != NULL)
		{
			strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
			bstrChName = strchName.AllocSysString();
		}
		if(m_nCrtProductID == EOE)
			m_algoArray.get<0>()->SetCurrentTaskName(&bstrTaskName);
		if(m_nCrtProductID == CAN)
			m_algoArray.get<1>()->SetCurrentTaskName(&bstrTaskName,&bstrChName);

#elif MAT_PRODUCT

		CString strchName;
		CComBSTR bstrChName;
		if (m_pStationOwner != NULL)
		{
			strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
			bstrChName = strchName.AllocSysString();
		}
			m_algoArray.get<0>()->SetCurrentTaskName2(&bstrTaskName,&bstrChName);
#elif EOE_PRODUCT

		CString strchName;
		CString strChNameWithoutCameraIndex;
		CComBSTR bstrChName;
		CComBSTR bstrChNameWithoutCameraIndex;
		if (m_pStationOwner != NULL)
		{
			strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
			bstrChName = strchName.AllocSysString();
			strChNameWithoutCameraIndex.Format(_T("CH%d"),m_pStationOwner->m_chIndex);
			bstrChNameWithoutCameraIndex = strChNameWithoutCameraIndex.AllocSysString();
		}
		m_algoArray.get<0>()->SetCurrentTaskName2(&bstrTaskName,&bstrChName,&bstrChNameWithoutCameraIndex);
#elif BOTTLE_PRODUCT

		CString strchName;
		CString strChNameWithoutCameraIndex;
		CComBSTR bstrChName;
		CComBSTR bstrChNameWithoutCameraIndex;
		if (m_pStationOwner != NULL)
		{
			strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
			bstrChName = strchName.AllocSysString();
			strChNameWithoutCameraIndex.Format(_T("CH%d"),m_pStationOwner->m_chIndex);
			bstrChNameWithoutCameraIndex = strChNameWithoutCameraIndex.AllocSysString();
		}
		m_algoArray.get<0>()->SetCurrentTaskName2(&bstrTaskName,&bstrChName,&bstrChNameWithoutCameraIndex);
#else
		m_algoArray.get<0>()->SetCurrentTaskName(&bstrTaskName);
#endif

		IMAGE_BOX crtImg;
		std::vector<FUNC_INFO>* pFuncInfoArray = nullptr;
		while(true)
		{
			m_imgBuffer.pop_back(&crtImg);
			if(crtImg.m_bLastFlag )
			{
				break;
			}
			if (crtImg.m_EmtpyImg)
			{
				continue;
			}
			m_crtImage = crtImg.m_img;

//			Hobject m_sourceImg = m_crtImage;

			if(!crtImg.m_bDebugImg)
			{
				m_imgIndex++;
				m_pStationOwner->SetProBeginTime();
				
// 				if (m_imgIndex%5==0)
// 				{
// 					Sleep(200);
// 				}
// 
// 				CString str;
// 				str.Format(_T("ID:%d	  m_ImageTimestamp:%d  m_ImageTimestamp_PC:%d"),crtImg.m_ImageID,crtImg.m_ImageTimestamp,crtImg.m_ImageTimestamp_PC);
// 				gLogger.QueueUpLog(str.GetString(),true);

//				write_image(crtImg.m_img, "tiff", 0, "C:\\Save");
			}

			::EnterCriticalSection(m_pParamDataCs);
			pFuncInfoArray = (crtImg.m_bDebugImg ? &m_vFuncInfoDebug : &m_vFuncInfo);
			::LeaveCriticalSection(m_pParamDataCs);
#ifdef BOTTLE_PRODUCT
			if(m_cameraGroupIndex>=0)
			{
				LONG DetectParam=1;
				m_algoArray.get<0>()->SetDetectParam(&DetectParam);
				image_mu.reset();
			}
#endif		
			auto start_t = steady_clock::now();
			bool bOk = CheckCurrentImage(pFuncInfoArray,crtImg);
			StoreCheckResult(bOk,crtImg);
			m_crtImgProduct.m_AlgoTimeCount=duration_cast<milliseconds>(nanoseconds((steady_clock::now()- start_t).count())).count();
			if(crtImg.m_bDebugImg)
				m_crtImgProduct_Debug.m_nGII = m_crtDisplayResult.m_nGII;
			else
				m_crtImgProduct.m_nGII = m_crtDisplayResult.m_nGII;

			std::map<int, bool> dictTmpStats = ((!crtImg.m_bDebugImg) ? m_dictFuncIndex_Rslt : m_dictFuncIndex_Rslt_Debug);
			IMG_PRODUCT imgProduct = (!crtImg.m_bDebugImg) ? m_crtImgProduct : m_crtImgProduct_Debug;
			bool bProductOk = true;			
			
			bool bAllReady = m_pStationOwner->CollectCheckResult(m_cameraGroupIndex, bOk, imgProduct, m_crtDisplayResult, dictTmpStats, bProductOk);
			auto end_t=steady_clock::now();
			auto img_cal_end_time=m_timer.DoubleNow();
			auto diff = end_t-start_t;

			//if(diff.count() / 10000.0 > 65)
			if(m_crtImgProduct.m_AlgoTimeCount>90) //img_cal_end_time - crtImg.m_ImageTimestamp_PC >65)
			{
				gLogger.QueueUpLog(L"CheckCurrentImage time > 65ms");
			}
			if(bAllReady)
			{				
//  				CString str;
//  				str.Format(_T("%d,%d"),m_imgIndex,bProductOk);
//  				gLogger.QueueUpLog(str.GetString(),true);
// 				if (!bProductOk&&(!(crtImg.m_bDebugImg || crtImg.m_bLastFlag || crtImg.m_bSwitchImg)))
// 				{
// 					CString path(_T("C:\\test"));
// 					_wmkdir(path);
// 					CString Source;
// 					Source.Format(_T("c:\\test\\Source%d"),m_imgIndex);
// 					string	m_Source= CT2A(Source.GetString());
// 					CString Dil;
// 					Dil.Format(_T("c:\\test\\Dil%d"),m_imgIndex);
// 					string m_Dil = CT2A(Dil.GetString());
// 					write_image(m_sourceImg, "tiff", 0,m_Source.c_str());
// 					write_image(crtImg.m_img, "tiff", 0,m_Dil.c_str());
// 				}
// 				if (m_pStationOwner->m_chIndex==1)
// 				{
// 					bProductOk = true;
// 				}
// 				if (m_pStationOwner->m_chIndex==2)
// 				{
// 					bProductOk= true;
// 				}
				m_pStationOwner->SendRejectSignal_Ex(bProductOk,(crtImg.m_bDebugImg || crtImg.m_bLastFlag || crtImg.m_bSwitchImg),m_imgBuffer.size());
				m_pStationOwner->UpdateMainScreen(bProductOk,(crtImg.m_bDebugImg || crtImg.m_bLastFlag || crtImg.m_bSwitchImg));
				m_pStationOwner->UpdateChConfigScreen(bProductOk, crtImg);
#ifdef BOTTLE_PRODUCT
				if(m_cameraGroupIndex>=0)
				{
					image_mu._l();
				}				
#endif
			}
			else if(m_cameraGroupIndex>=0)
			{
#ifdef BOTTLE_PRODUCT
				image_mu._w();
#endif
			}
		}
	}
	else
	{
		ReportStationStatus2ChUi(false);
	}

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
		m_algoArray.get<0>().Release();
	if(m_nCrtProductID == CAN)
		m_algoArray.get<1>().Release();
#else
	m_algoArray.get<0>().Release();
#endif

	::CoUninitialize();
	m_bProcessorOk = false;
	return 0;
}

LONG CImageProcessor::GetDetectParam()
{
	LONG DetectParam=1; 
#ifdef BOTTLE_PRODUCT	
	m_algoArray.get<0>()->GetDetectParam(&DetectParam);
#endif
	return DetectParam;
}

wstring CImageProcessor::ParseSelfLearningParam(const wstring& wstrInput)
{
	wstring wstrRet(L"");
	if(wstrInput == L"") return wstrRet;

	std::vector<wstring> vItems = utils::SplitString(wstrInput, L",");
	if(vItems.empty()) return wstrRet;
	for(size_t t = 0; t < vItems.size(); t++)
	{
		std::vector<wstring> vValue = utils::SplitString(vItems[t], L"@");
		if(vValue.size() == 2)
		{
			wstrRet.append(vValue[1]);
			wstrRet.append(L",");
		}
	}
	wstrRet = wstrRet.substr(0, wstrRet.length() - 1);
	return wstrRet;
}

unsigned int CImageProcessor::SelfLearningThread()
{
	m_bProcessorOk = false;
	HRESULT hr = ::CoInitialize(NULL);
	if(SUCCEEDED(hr))
	{

#ifdef CROWN_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(CrownCapAlgo));
#endif

#ifdef EOE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(EoeAlgo));
#endif

#ifdef CAN_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(CanAlgo));
#endif

#ifdef CAN_EOE_PRODUCT
		if(m_nCrtProductID == EOE)
			hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(EoeAlgo));
		if(m_nCrtProductID == CAN)
			hr = m_algoArray.get<1>().CoCreateInstance(__uuidof(CanAlgo));
#endif

#ifdef	PET_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(PetAlgo));
#endif

#ifdef JELLY_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(JellyAlgo));
#endif

#ifdef PCC_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(PccAlgo));
#endif

#ifdef CAP_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(CapacitorAlgo));
#endif

#ifdef MAT_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(RubberMatAlgo));
#endif

#ifdef NEEDLE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(NeedleAlgo));
#endif

#ifdef PLUG_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(PlugAlgo));
#endif

#ifdef HDPE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(HdpeAlgo));
#endif

#ifdef CAN_OUTSIDE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(CanOutsideAlgo));
#endif

#ifdef NAIL_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(NailAlgo));
#endif

#ifdef BATTERY_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(BatteryShellAlgo));
#endif

#ifdef BOTTLE_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(BottleCapAlgo));
#endif

#ifdef AL_PLASTIC_PRODUCT
		hr = m_algoArray.get<0>().CoCreateInstance(__uuidof(AlPlasticCapAlgo));
#endif

		Sleep(20);
		if(!utils::CheckResult(hr))
		{	

#ifdef CAN_EOE_PRODUCT
			if(m_nCrtProductID == EOE)
				m_algoArray.get<0>().Release();
			if(m_nCrtProductID == CAN)
				m_algoArray.get<1>().Release();
#else
			m_algoArray.get<0>().Release();
#endif


			ReportStationStatus2ChUi(false);
			::CoUninitialize();
			return -1; //TODO:Something wrong with the TaskGroup Group
		}
		m_wstrSlParam = L"";
		m_bProcessorOk = true;
		ReportStationStatus2ChUi(true);
		m_wstrSlParam = ParseSelfLearningParam(m_crtTask.m_wstrSlParam);
		CComBSTR bstrSlParam(m_wstrSlParam.c_str());

#ifdef CAN_EOE_PRODUCT
		if(m_nCrtProductID == EOE)
			m_algoArray.get<0>()->SetSelfLearningParam(m_crtTask.m_nSampleCount,&bstrSlParam); 
		else
			m_algoArray.get<1>()->SetSelfLearningParam(m_crtTask.m_nSampleCount,&bstrSlParam); 
#else
		m_algoArray.get<0>()->SetSelfLearningParam(m_crtTask.m_nSampleCount,&bstrSlParam);
#endif

		m_vModelImage.clear();

		IMAGE_BOX crtImg;
		UINT tmpId     = (UINT)m_crtTask.m_nProductId;
		BYTE productId = (BYTE)tmpId;
		auto pStation  = (CImageProcessor*)this;
		while(true)
		{
			m_imgBuffer.pop_back(&crtImg);
			if(crtImg.m_bLastFlag)
			{
				break;
			}
			m_crtImage = crtImg.m_img;
			if(++m_imgIndex < m_crtTask.m_nSampleCount || crtImg.m_bDebugImg)
			{
				if(crtImg.m_bDebugImg)
				{

#ifdef CAN_EOE_PRODUCT
					m_algoArray.get<1>()->SetCurrentImage((long*)&crtImg.m_img);
					CComBSTR bstrSlParam(m_wstrSlParam.c_str());
					m_algoArray.get<1>()->ResetSelfLearningRegion(&bstrSlParam);

					Hobject retRgn;
					m_algoArray.get<1>()->GetSelfLearningRegion((long*)&retRgn);
					m_pStationOwner->DisPlayModelImage(m_cameraGroupIndex,crtImg.m_img,retRgn);	
					continue;
#elif BATTERY_PRODUCT
					LONG* ret=nullptr;
					m_algoArray.get<0>()->SetCurrentImage((long*)&crtImg.m_img,crtImg.m_iProcessID,ret);
					CComBSTR bstrSlParam(m_wstrSlParam.c_str());
					m_algoArray.get<0>()->ResetSelfLearningRegion(&bstrSlParam);

					Hobject retRgn;
					m_algoArray.get<0>()->GetSelfLearningRegion((long*)&retRgn);
					m_pStationOwner->DisPlayModelImage(m_cameraGroupIndex,crtImg.m_img,retRgn);
					continue;
#else
					m_algoArray.get<0>()->SetCurrentImage((long*)&crtImg.m_img);
					CComBSTR bstrSlParam(m_wstrSlParam.c_str());
					m_algoArray.get<0>()->ResetSelfLearningRegion(&bstrSlParam);

					Hobject retRgn;
					m_algoArray.get<0>()->GetSelfLearningRegion((long*)&retRgn);
					m_pStationOwner->DisPlayModelImage(m_cameraGroupIndex,crtImg.m_img,retRgn);
					continue;
#endif

				}
				else
				{
					m_pStationOwner->CollectSelfResultCount(m_imgIndex);
				}
			}
			else if(m_imgIndex == m_crtTask.m_nSampleCount)
			{
				gSystemMgr.PauseChannelCamera(m_pChConfig->m_chIndex);

				Hobject retModelImg;
				long nRetModelCount = 0;
				std::vector<float> vScore(500, 0.0f);

#ifdef CAN_EOE_PRODUCT
				if(m_nCrtProductID == EOE)
				{
					m_algoArray.get<0>()->GetSelfLearningResult((LONG*)&retModelImg, &nRetModelCount, &vScore[0], productId);
				}
				else
				{
					m_algoArray.get<1>()->GetSelfLearningResult((LONG*)&retModelImg, &nRetModelCount, &vScore[0], productId);
				}
#else
				m_algoArray.get<0>()->GetSelfLearningResult((LONG*)&retModelImg, &nRetModelCount, &vScore[0], productId);
#endif
				m_vModelImage.clear();				
				Hlong nModelCount = 0;

#ifndef  MIX_LABEL_ALGO_NEW
				count_channels(retModelImg, &nModelCount);
				for(long i = 1; i <= nModelCount; i++)
				{
					Hobject ret;
					access_channel(retModelImg, &ret, i);
					MODEL_IMAGE model;
					if(ret.Id() != H_EMPTY_REGION)
					{
						model.m_image  = ret;
						model.m_fScore = vScore[i-1];
						m_vModelImage.push_back(model);
					}
				}

#else
				count_obj(retModelImg, &nModelCount);
 				for(long i = 1; i <= nRetModelCount; i++)
 				{
					Hobject ret; 					
					Hobject ret1;

					if (nModelCount/nRetModelCount == 2)
					{
						select_obj(retModelImg, &ret, 2*i-1);
						select_obj(retModelImg, &ret1, 2*i);
						concat_obj(ret, ret1, &ret);
					} 
					else
					{
						select_obj(retModelImg, &ret, i);						
					}

					

					MODEL_IMAGE model;
					if(ret.Id() != H_EMPTY_REGION)
					{
						model.m_image  = ret;
						model.m_fScore = vScore[0];
						m_vModelImage.push_back(model);
					}
				}
#endif				
				m_pStationOwner->CollectSelfResultCount(m_imgIndex);
				m_pStationOwner->UpdateStatistics();
				if(m_pStationOwner != nullptr)
				{
					::PostMessage(m_pStationOwner->m_hWnd, MSG_MODEL_IMG_READY, (WPARAM)nRetModelCount, (LPARAM)-1);
				}
				m_bModelImgReady = true;

			}
			else
			{
				//TODO:
			}
			if(crtImg.m_bSwitchImg)
			{
				m_imgIndex--;
				continue;
			}
#ifdef CAN_EOE_PRODUCT
			if(m_nCrtProductID == EOE)
			{
				m_algoArray.get<0>()->SelectModelImage((long*)&crtImg.m_img, productId);
			}
			else
			{
				m_algoArray.get<1>()->SelectModelImage((long*)&crtImg.m_img, productId);
			}
#else
			m_algoArray.get<0>()->SelectModelImage((long*)&crtImg.m_img, productId);
#endif	
		}
	}
	else
	{
		ReportStationStatus2ChUi(false);
	}

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		m_algoArray.get<0>().Release();
	}
	else
	{
		m_algoArray.get<1>().Release();
	}
#else
	m_algoArray.get<0>().Release();
#endif
	::CoUninitialize();
	m_bProcessorOk = false;
	return 0;
}
void CImageProcessor::RetrieveTargetRegion(int crtFunIndex)
{
	if(crtFunIndex != m_crtSelectedFuncIndex)
	{
		return;
	}
	if(!m_bChWndOpened)
	{
		return;
	}
	if(m_crtSelectedFuncIndex != -1)
	{
		int rgnID = m_vFuncInfo[m_crtSelectedFuncIndex].GetTargetRegionID();
		Hobject retRgn;
		Hobject retRgnEx;
		HRESULT hr = E_FAIL;

		Hobject retInterImg;

#ifdef CAN_EOE_PRODUCT
		if(m_nCrtProductID == EOE)
		{
			hr = m_algoArray.get<0>()->GetRegion(rgnID,(long*)&retRgn);
			if(m_nCrtProductID == EOE)
			{
				hr = m_algoArray.get<0>()->GetInterImgs(rgnID,(long*)&retInterImg);

				if (!m_vFuncInfo[m_crtSelectedFuncIndex].m_bTestFunc)
				{
					hr = m_algoArray.get<0>()->GetRegionEx(rgnID,(long*)&retRgnEx);
				}
				else
				{
					hr = m_algoArray.get<0>()->GetRegionExTest(rgnID,(long*)&retRgnEx);
				}
			}
		}
		else
		{
			hr = m_algoArray.get<1>()->GetRegion(rgnID,(long*)&retRgn);
			hr = m_algoArray.get<1>()->GetInterImgs(rgnID,(long*)&retInterImg);
		}

#elif CAN_PRODUCT
		hr = m_algoArray.get<0>()->GetRegion(rgnID,(long*)&retRgn);
		if(m_nCrtProductID == CAN)
		{
			hr = m_algoArray.get<0>()->GetInterImgs(rgnID,(long*)&retInterImg);
			if (!m_vFuncInfo[m_crtSelectedFuncIndex].m_bTestFunc)
			{
				hr = m_algoArray.get<0>()->GetRegionEx(rgnID, (long*)&retRgnEx);
			}
			else
			{
				hr = m_algoArray.get<0>()->GetRegionExTest(rgnID, (long*)&retRgnEx);
			}
		}
#elif BATTERY_PRODUCT
		hr = m_algoArray.get<0>()->GetRegion(rgnID,(long*)&retRgn);
		if(m_nCrtProductID == BATTERY)
		{
			hr = m_algoArray.get<0>()->GetInterImgs(rgnID,(long*)&retInterImg);

			if (!m_vFuncInfo[m_crtSelectedFuncIndex].m_bTestFunc)
			{
				hr = m_algoArray.get<0>()->GetRegionEx(rgnID,(long*)&retRgnEx);
			}
			else
			{
				hr = m_algoArray.get<0>()->GetRegionExTest(rgnID,(long*)&retRgnEx);
			}
	
		}
#elif EOE_PRODUCT
		hr = m_algoArray.get<0>()->GetRegion(rgnID,(long*)&retRgn);
		if(m_nCrtProductID == EOE)
		{
			hr = m_algoArray.get<0>()->GetInterImgs(rgnID,(long*)&retInterImg);

			if (!m_vFuncInfo[m_crtSelectedFuncIndex].m_bTestFunc)
			{
				hr = m_algoArray.get<0>()->GetRegionEx(rgnID,(long*)&retRgnEx);
			}
			else
			{
				hr = m_algoArray.get<0>()->GetRegionExTest(rgnID,(long*)&retRgnEx);
			}
		}
#elif BOTTLE_PRODUCT
		hr = m_algoArray.get<0>()->GetRegion(rgnID,(long*)&retRgn);
		if(m_nCrtProductID == BOTTLE)
		{
			hr = m_algoArray.get<0>()->GetInterImgs(rgnID,(long*)&retInterImg);

			if (!m_vFuncInfo[m_crtSelectedFuncIndex].m_bTestFunc)
			{
				hr = m_algoArray.get<0>()->GetRegionEx(rgnID,(long*)&retRgnEx);
			}
			else
			{
				hr = m_algoArray.get<0>()->GetRegionExTest(rgnID,(long*)&retRgnEx);
			}
		}
#elif AL_PLASTIC_PRODUCT
		hr = m_algoArray.get<0>()->GetRegion(rgnID,(long*)&retRgn);
		if(m_nCrtProductID == AL_PLASTIC)
		{
			hr = m_algoArray.get<0>()->GetInterImgs(rgnID,(long*)&retInterImg);

			if (!m_vFuncInfo[m_crtSelectedFuncIndex].m_bTestFunc)
			{
				hr = m_algoArray.get<0>()->GetRegionEx(rgnID,(long*)&retRgnEx);
			}
			else
			{
				hr = m_algoArray.get<0>()->GetRegionExTest(rgnID,(long*)&retRgnEx);
			}
		}
#else
		hr = m_algoArray.get<0>()->GetRegion(rgnID,(long*)&retRgn);
#endif

		if(utils::CheckResult(hr))
		{
			m_crtImgProduct_Debug.m_crtTargetRgn = retRgn;
		
			if ( m_nCrtProductID == CAN || m_nCrtProductID == EOE ||m_nCrtProductID == BATTERY||m_nCrtProductID == BOTTLE||m_nCrtProductID == AL_PLASTIC)
			{
				m_crtImgProduct_Debug.m_cvInterImgs = retInterImg;
				m_crtImgProduct_Debug.m_crtTargetRgnEx = retRgnEx;
			}
			
		}

	}
}

void CImageProcessor::StoreCheckResult(bool bOk,IMAGE_BOX& crtImg)
{
	if( crtImg.m_bSwitchImg || crtImg.m_bLastFlag)
	{
		return;
	}

	DisplayResult tmp;
	//TimeStatistic
	auto pDictTiming = &m_dictFuncTiming;
	double total = 0.0;
	for(auto it = pDictTiming->begin(); it != pDictTiming->end(); it++)
	{
		int nFuncIndex = it->first;
		if(nFuncIndex == m_crtSelectedFuncIndex)
		{
			tmp.m_IndexTime = it->second;
		}
		total += it->second;
	}
	tmp.m_Alltime = total;
	if((int)ceil(total) < m_crtTask.m_nTimingUcl)
	{
		tmp.m_bOverTime =  false;
	}
	else
	{
		tmp.m_bOverTime =  true;
	}

	tmp.m_bScoreValid = THRESHOLD_SCORE.m_bValid;
	tmp.m_strOutputName = CW2A(THRESHOLD_SCORE.m_wstrOutputName.c_str());

	float fScore = -1.0f;
	HRESULT hr = THRESHOLD_SCORE.m_score.ChangeType(VT_R4);

	if(SUCCEEDED(hr))
	{
		fScore = THRESHOLD_SCORE.m_score.fltVal;
	}

	if(THRESHOLD_SCORE.m_threshold.m_type == VT_I4)
	{
		tmp.m_Socre = HTuple((long)fScore);
	}
	else
	{
		tmp.m_Socre = HTuple(fScore);
	}

	if(fScore == -1.0f)
	{
		tmp.m_bScoreError = true;
	}
	else
	{
		bool bPassed = utils::CompareWithThreshold(THRESHOLD_SCORE.m_threshold, THRESHOLD_SCORE.m_score);
		
		if (THRESHOLD_SCORE.m_score.fltVal < -10.0f)
		{
//			tmp.m_Socre = THRESHOLD_SCORE.m_score.fltVal + 10;
			if (THRESHOLD_SCORE.m_threshold.m_type == VT_I4)
			{
				tmp.m_Socre = HTuple((long)(THRESHOLD_SCORE.m_score.fltVal + 10));
			}
			else
			{
				tmp.m_Socre = THRESHOLD_SCORE.m_score.fltVal + 10;
			}
			
		}

		if(bPassed)
		{
			tmp.m_bScoreError = false;
		}
		else
		{
			tmp.m_bScoreError = true;
		}
	}

	tmp.m_vInnerStatsData = m_vInnerStatsData;
	if(m_compoundStatsFuncID != -1)
	{
		HTuple gii;
		get_grayval(crtImg.m_img,8,0,&gii);
		tmp.m_nGII = gii[0].I();
	}
	m_crtDisplayResult    = tmp;
}

void CImageProcessor::UpdateImageCount(const IMAGE_BOX& crtImg, int nFuncIndex)
{
	if(crtImg.m_bSwitchImg)
	{
		return;
	}
	if(!crtImg.m_bDebugImg)
	{
		m_vFuncInfo[nFuncIndex].m_nTotalCount++;
		m_vFuncInfo[nFuncIndex].m_bPass = true;
	}
	else
	{
		m_vFuncInfoDebug[nFuncIndex].m_bPass = true;
	}
}

void CImageProcessor::CollectInnerStats(int nFuncIndex, int nRgnID)
{
	if(!m_bHasInnerStatsIt || nFuncIndex != m_crtSelectedFuncIndex)
		return;

	int nMaxSz = 2048;
	std::vector<float> vRet(nMaxSz);
	long nActualSz = 0;
	HRESULT hr = E_FAIL;
#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		hr = m_algoArray.get<0>()->GetInternalStats(nRgnID, nMaxSz, &vRet[0], &nActualSz);
	}
	else
	{
		hr = m_algoArray.get<1>()->GetInternalStats(nRgnID, nMaxSz, &vRet[0], &nActualSz);
	}
#else
	hr = m_algoArray.get<0>()->GetInternalStats(nRgnID, nMaxSz, &vRet[0], &nActualSz);
#endif
	if(SUCCEEDED(hr))
	{
		m_vInnerStatsData.assign(vRet.begin(), vRet.begin() + nActualSz);
	}
}

void CImageProcessor::CollectErrorRegion(int nRgnID)
{
	if(!m_bChWndOpened)
	{
		//return;
	}

	if(m_bDispErrRgn)
	{
		Hobject retRgn;
		HRESULT hr = E_FAIL;
#ifdef CAN_EOE_PRODUCT
		if(m_nCrtProductID == EOE)
		{
			hr = m_algoArray.get<0>()->GetErrorRegion(nRgnID,(long*)&retRgn);
		}
		else
		{
			hr = m_algoArray.get<1>()->GetErrorRegion(nRgnID,(long*)&retRgn);
		}
#else
		hr = m_algoArray.get<0>()->GetErrorRegion(nRgnID,(long*)&retRgn);
#endif
		if(SUCCEEDED(hr) && retRgn.Id() != H_EMPTY_REGION)
		{
			union1(retRgn,&retRgn);
			Hlong ErrorChCount = 0;
			count_obj(retRgn, &ErrorChCount);

			for (Hlong i = 0; i < ErrorChCount;i++)
			{
				Hobject ErrorObject; 	
				select_obj(retRgn, &ErrorObject,i+1);
				if(m_crtUnionErrRgn[i].Id() != H_EMPTY_REGION)
				{
					concat_obj(m_crtUnionErrRgn[i],ErrorObject, &m_crtUnionErrRgn[i]);
				}
				else
				{
					m_crtUnionErrRgn[i] = ErrorObject;
				}
			}
		}

//白缺陷附加代码
		if(m_nCrtProductID == CAN || m_nCrtProductID == EOE || m_nCrtProductID == BATTERY|| m_nCrtProductID == PLUG|| m_nCrtProductID == BOTTLE|| m_nCrtProductID == AL_PLASTIC)
		{
			hr = E_FAIL; // Com 接口返回值初始化
			Hobject retRgnWhite;
	
#ifdef CAN_EOE_PRODUCT
		
			if(m_nCrtProductID == CAN)
			{
					hr = m_algoArray.get<1>()->GetWhiteErrorRegion(nRgnID,(long*)&retRgnWhite);
			}
			else
			{
					hr = m_algoArray.get<0>()->GetWhiteErrorRegion(nRgnID,(long*)&retRgnWhite);
			}
#elif CAN_PRODUCT
			hr = m_algoArray.get<0>()->GetWhiteErrorRegion(nRgnID,(long*)&retRgnWhite);
#elif EOE_PRODUCT
			hr = m_algoArray.get<0>()->GetWhiteErrorRegion(nRgnID,(long*)&retRgnWhite);
#elif BATTERY_PRODUCT
			hr = m_algoArray.get<0>()->GetWhiteErrorRegion(nRgnID,(long*)&retRgnWhite);
#elif PLUG_PRODUCT
			hr = m_algoArray.get<0>()->GetWhiteErrorRegion(nRgnID,(long*)&retRgnWhite);
#elif BOTTLE_PRODUCT
			hr = m_algoArray.get<0>()->GetWhiteErrorRegion(nRgnID,(long*)&retRgnWhite);
#elif AL_PLASTIC_PRODUCT
			hr = m_algoArray.get<0>()->GetWhiteErrorRegion(nRgnID,(long*)&retRgnWhite);
#endif

			if(SUCCEEDED(hr) && retRgnWhite.Id() != H_EMPTY_REGION)
			{
				union1(retRgnWhite,&retRgnWhite);
				if(m_crtUnionErrRgn[1].Id() != H_EMPTY_REGION)
				{
					concat_obj(m_crtUnionErrRgn[1],retRgnWhite, &m_crtUnionErrRgn[1]);
				}
				else
				{
					m_crtUnionErrRgn[1] = retRgnWhite;
				}
			}
		}
	}
}

void CImageProcessor::CollectFuncErrorCount(const IMAGE_BOX& crtImg, int nFuncIndex,int nRoiFuncIndex)
{
	if(!crtImg.m_bSwitchImg)
	{
		if(!crtImg.m_bDebugImg)
		{
			m_vFuncInfo[nFuncIndex].m_bPass = false;
			m_dictFuncIndex_Rslt[nRoiFuncIndex] = false;
			m_dictFuncIndex_Rslt[nFuncIndex] = false;
		}
		else
		{
			m_vFuncInfoDebug[nFuncIndex].m_bPass = false;
			m_dictFuncIndex_Rslt_Debug[nRoiFuncIndex] = false;
			m_dictFuncIndex_Rslt_Debug[nFuncIndex] = false;
		}
	}
}

UINT CImageProcessor::CollectFuncOutputValue(const IMAGE_BOX& crtImg, const FUNC_INFO& fi, float fRetValue)
{	
	auto start_t = boost::chrono::steady_clock::now();
	if(!crtImg.m_bDebugImg && fRetValue > 0.0000001f)
	{
		float value = fRetValue;
		if(fi.m_threshold.m_type != VT_I4)
		{
			value = utils::RoundOffValue(value,fi.m_threshold.m_stepLen);
		}
		bool bAdjustRange = ((m_imgIndex > 1) && (m_imgIndex % 3000 == 0));
		m_pStationOwner->CollectFuncOutputValueALL(fi.m_funcIndex,fi.m_threshold.m_specV,value,bAdjustRange);
	}
	return boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::nanoseconds((boost::chrono::steady_clock::now()- start_t).count())).count();
}

void CImageProcessor::CollectCrtFuncScore(wstring wstrOutputName,const THRESHOLD& threshold, CComVariant& score)
{
	THRESHOLD_SCORE.m_wstrOutputName = wstrOutputName;
	THRESHOLD_SCORE.m_threshold      = threshold;
	THRESHOLD_SCORE.m_score          = score;
	THRESHOLD_SCORE.m_bValid         = true;
}

void CImageProcessor::ResetInternalData(IMAGE_BOX crtImg)
{
	m_dictFuncTiming.clear();
	m_crtUnionErrRgn.clear();

	if(!crtImg.m_bDebugImg)
	{
		m_crtImgProduct.m_img = crtImg.m_img;
		m_crtImgProduct.m_bDebug = false;
		m_crtImgProduct.m_bSingleDebug =crtImg.m_bSingleDebugImg;
		m_crtImgProduct.m_crtTargetRgn.Reset();
		m_crtImgProduct.m_crtTargetRgnEx.Reset();
		m_crtImgProduct.m_unionErrRgn.clear();
		m_crtImgProduct.m_cvInterImgs.Reset();
		m_crtImgProduct.m_crtCorrectImg.Reset();
		m_crtImgProduct.m_iProcessID = crtImg.m_iProcessID;

		m_crtImgProduct.m_ImageTimeCount = crtImg.m_ImageTimestamp;
		m_crtImgProduct.m_ImageTimeCount_PC = crtImg.m_ImageTimestamp_PC;
		m_crtImgProduct.m_ImageID = crtImg.m_ImageID;

	}
	else
	{
		m_crtImgProduct_Debug.m_img = crtImg.m_img;
		m_crtImgProduct_Debug.m_bDebug = true;
		m_crtImgProduct_Debug.m_bSingleDebug = crtImg.m_bSingleDebugImg;
		m_crtImgProduct_Debug.m_crtTargetRgn.Reset();
		m_crtImgProduct_Debug.m_crtTargetRgnEx.Reset();
		m_crtImgProduct_Debug.m_unionErrRgn.clear();
		m_crtImgProduct_Debug.m_cvInterImgs.Reset();
		m_crtImgProduct_Debug.m_crtCorrectImg.Reset();
		m_crtImgProduct_Debug.m_cvInterImgs.Reset();
		m_crtImgProduct_Debug.m_iProcessID = crtImg.m_iProcessID;

		m_crtImgProduct_Debug.m_ImageID = crtImg.m_ImageID;
		m_crtImgProduct_Debug.m_ImageTimeCount = crtImg.m_ImageTimestamp;
		m_crtImgProduct_Debug.m_ImageTimeCount_PC = crtImg.m_ImageTimestamp_PC;

	}
	THRESHOLD_SCORE.m_bValid = false;
	m_dictFuncIndex_Rslt.clear();
	m_dictFuncIndex_Rslt_Debug.clear();

	m_vInnerStatsData.clear();

}

bool CImageProcessor::CheckCurrentImage(std::vector<FUNC_INFO>* pFuncInfoArray,IMAGE_BOX& crtImg)
{
	ResetInternalData(crtImg);

	HRESULT hr = E_FAIL;
#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		hr = m_algoArray.get<0>()->SetCurrentImage((long*)&crtImg.m_img);
	}
	else
	{
		hr = m_algoArray.get<1>()->SetCurrentImage((long*)&crtImg.m_img);
	}
#elif BATTERY_PRODUCT
	Hobject nRet;
	hr = m_algoArray.get<0>()->SetCurrentImage((long*)&crtImg.m_img,crtImg.m_iProcessID, (long*)&nRet);

	if (nRet.Id()!=H_EMPTY_REGION)
	{
		if(!crtImg.m_bDebugImg)
		{
			m_crtImgProduct.m_crtCorrectImg = nRet;
		}
		else
		{
			m_crtImgProduct_Debug.m_crtCorrectImg = nRet;
		}
	}
#else
	//write_image(crtImg.m_img,"tiff", 0, "c:\\ttt.tiff");
	hr = m_algoArray.get<0>()->SetCurrentImage((long*)&crtImg.m_img);
#endif
	if(!utils::CheckResult(hr))
		return false;

	bool bOk = true;

	int nFuncInex = -1;
	int nCrtRoiFuncIndex = 0;
	int nDieId            = -1;
	FUNC_INFO* pFuncDieId = nullptr;

	bool bCompoundRgnOk          = true;
	int nGlueInjectorID          = -1;
	FUNC_INFO* pGlueInjectorFunc = nullptr;
	long long nCollectFunc_Time=0;

	for(auto it = pFuncInfoArray->begin(); it != pFuncInfoArray->end(); it++)
	{
		FUNC_INFO& fi = *it;
		UpdateImageCount(crtImg,++nFuncInex);

		if(!fi.m_bEnable)
		{
			continue;
		}
		DISPID funcId = fi.m_dispId;
		int nRgnId = fi.GetTargetRegionID();

		DISPPARAMS dispParams = {&fi.m_vComParam[0], NULL, (UINT)fi.m_vComParam.size(), 0};

		if(!fi.m_bTestFunc)
		{
			nCrtRoiFuncIndex = fi.m_funcIndex;
			m_timer.start();
			auto tm_start=steady_clock::now();
#ifdef CAN_EOE_PRODUCT
			if(m_nCrtProductID == EOE)
			{
				hr = m_algoArray.get<0>()->Invoke(funcId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, NULL, NULL, NULL);
			}
			else
			{
				hr = m_algoArray.get<1>()->Invoke(funcId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, NULL, NULL, NULL);
			}
#else
			hr = m_algoArray.get<0>()->Invoke(funcId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, NULL, NULL, NULL);
#endif		
			nCollectFunc_Time+= (steady_clock::now()-tm_start).count();
			m_dictFuncTiming[fi.m_funcIndex] = m_timer.duration();
			
			if(!utils::CheckResult(hr))
			{
				//TODO:
			}
		}
		else
		{
			CComVariant retValue;
			m_timer.start();
			auto tm_start=steady_clock::now();
#ifdef CAN_EOE_PRODUCT
			if(m_nCrtProductID == EOE)
			{
				hr = m_algoArray.get<0>()->Invoke(funcId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, &retValue, NULL, NULL);
			}
			else
			{
				hr = m_algoArray.get<1>()->Invoke(funcId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, &retValue, NULL, NULL);
			}
#else
			hr = m_algoArray.get<0>()->Invoke(funcId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispParams, &retValue, NULL, NULL);
#endif
			nCollectFunc_Time+= (steady_clock::now()-tm_start).count();
			m_dictFuncTiming[fi.m_funcIndex] = m_timer.duration();
			
			long nRet = retValue.lVal;
			if(utils::CheckResult(hr) || nRet == -1)
			{
				bool bFuncPassed = utils::CompareWithThreshold(fi.m_threshold, retValue);
				if(fi.m_strFuncName == L"FindOcrNumberAlgo")
				{
					nDieId      = nRet;
					bFuncPassed = true;
					pFuncDieId  = &fi;
				}
				if(fi.m_strFuncName == L"GlueInjectorIDDetectAlgo"||fi.m_strFuncName == L"GlueInjectorIDDetectAlgo1")
				{
					nGlueInjectorID   = nRet;
					bFuncPassed       = true;
					pGlueInjectorFunc = &fi;
				}
				if(fi.m_strFuncName == L"CalibrationImageAlgo" || fi.m_strFuncName == L"PhaseCorrelationRectangularAlgo" || fi.m_strFuncName == L"RegistrateRectangleAlgo")
				{
					if (nRet>100)
					{	
						void* pAddr = (void*)nRet;
						Hobject* pTmpImg = (Hobject*)pAddr;
						Hobject tmpImg = *pTmpImg;//
						if(!crtImg.m_bDebugImg)
						{
							m_crtImgProduct.m_crtCorrectImg = tmpImg;
						}
						else
						{
							m_crtImgProduct_Debug.m_crtCorrectImg = tmpImg;
						}
						//write_image(tmpImg, "tiff", 0, "D:\\0002.tiff");
					}
				} 
				bOk &= bFuncPassed;
				if(!bFuncPassed)
				{
					CollectErrorRegion(nRgnId);
					CollectFuncErrorCount(crtImg,nFuncInex,nCrtRoiFuncIndex);
					if(m_nCompoundRgnID == nRgnId)
					{
						bCompoundRgnOk = false;
					}
				}
				if(fi.m_strFuncName != L"FindOcrNumberAlgo")
				{
					//retvalue值 类型统一为vt_R4,用于正常显示算子输出直方图！
					retValue.ChangeType(VT_R4);
					//nCollectFunc_Time+=CollectFuncOutputValue(crtImg, fi, retValue.fltVal);
					if(crtImg.m_bDebugImg)
						m_crtImgProduct_Debug.m_nGII=TrackGlueInjectorID(fi.m_dispId);
					else
						m_crtImgProduct.m_nGII=TrackGlueInjectorID(fi.m_dispId);
				}
				if(crtImg.m_bDebugImg)
				{
					CollectInnerStats(fi.m_funcIndex, (int)fi.m_vParam[0].m_value);
				}

				if(crtImg.m_bDebugImg && fi.m_funcIndex == m_crtSelectedFuncIndex)
				{
					CollectCrtFuncScore(fi.m_strOutputName, fi.m_threshold, retValue);
				}
			}	
		}
		if(crtImg.m_bDebugImg)
		{
			RetrieveTargetRegion(fi.m_funcIndex);
		}

	}	
	m_crtImgProduct.m_CollectFunInfoCount=duration_cast<nanoseconds>(nanoseconds(nCollectFunc_Time)).count();
	if(crtImg.m_bDebugImg)
	if(!bOk && pFuncDieId != nullptr && nDieId > -1)
	{
		CollectFuncOutputValue(crtImg, *pFuncDieId, (float)nDieId);
	}
	// Compound injector statistics
	if(nGlueInjectorID != -1 && !bCompoundRgnOk && !crtImg.m_bDebugImg)
	{
		CollectFuncOutputValue(crtImg, *pGlueInjectorFunc, (float)nGlueInjectorID);
	}
	if(!crtImg.m_bDebugImg)
	{
		m_crtImgProduct.m_unionErrRgn = m_crtUnionErrRgn;
	}
	else
	{
		m_crtImgProduct_Debug.m_unionErrRgn = m_crtUnionErrRgn;
	}
	return bOk;
}

long CImageProcessor::TrackGlueInjectorID(DISPID funcId)
{
	long nCrtInjectorID = -1;
	HRESULT hr = E_FAIL;

	if(funcId == m_glueRgnFuncID)
	{
#ifdef  EOE_PRODUCT
		hr = m_algoArray.get<0>()->GetGlueInjectorID(&nCrtInjectorID);
#endif
		if(SUCCEEDED(hr) && nCrtInjectorID != -1)
		{
			gSystemMgr.LogGlueInjectorError(m_pChConfig->m_chIndex, nCrtInjectorID);
		}
	}
	return nCrtInjectorID;
}

bool CImageProcessor::ChangeDebugParameter(DISPID& funcId, int funcIndex, std::vector<INPUT_PARAM>& vNewParam)
{
	if(vNewParam.empty())
		return false;
	::EnterCriticalSection(m_pParamDataCs);
	for(int i = 0; i < m_vFuncInfoDebug.size(); i++)
	{
		if(funcId == m_vFuncInfoDebug[i].m_dispId && funcIndex == m_vFuncInfoDebug[i].m_funcIndex)
		{
			m_vFuncInfoDebug[i].m_vParam.assign(vNewParam.begin(), vNewParam.end());
			m_vFuncInfoDebug[i].CreateComParamArray();
			break;
		}
	}
	::LeaveCriticalSection(m_pParamDataCs);

	return true;
}

void CImageProcessor::ApplyNewParameter(DISPID& funcId, int funcIndex)
{
	::EnterCriticalSection(m_pParamDataCs);
	for(int i = 0; i < m_vFuncInfo.size(); i++)
	{
		if(m_vFuncInfo[i].m_dispId == funcId && funcIndex == m_vFuncInfo[i].m_funcIndex)
		{
			m_vFuncInfo[i].m_vParam.assign(m_vFuncInfoDebug[i].m_vParam.begin(),m_vFuncInfoDebug[i].m_vParam.end());
			m_vFuncInfo[i].m_vComParam.assign(m_vFuncInfoDebug[i].m_vComParam.begin(),m_vFuncInfoDebug[i].m_vComParam.end());
			break;
		}
	}
	::LeaveCriticalSection(m_pParamDataCs);
}

void CImageProcessor::RestoreDebugParameter(DISPID& funcId, int funcIndex)
{
	::EnterCriticalSection(m_pParamDataCs);

	for(int i = 0; i < m_vFuncInfoDebug.size(); i++)
	{
		if(m_vFuncInfoDebug[i].m_dispId == funcId && funcIndex == m_vFuncInfoDebug[i].m_funcIndex)
		{
			m_vFuncInfoDebug[i].m_vParam.assign(m_vFuncInfo[i].m_vParam.begin(),m_vFuncInfo[i].m_vParam.end());
			m_vFuncInfoDebug[i].m_vComParam.assign(m_vFuncInfo[i].m_vComParam.begin(),m_vFuncInfo[i].m_vComParam.end());
			break;
		}
	}
	::LeaveCriticalSection(m_pParamDataCs);
}

void CImageProcessor::Start()
{

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		if(m_algoArray.get<0>() != nullptr)
		{
			m_algoArray.get<0>().Release();
		}
	}
	else
	{
		if(m_algoArray.get<1>() != nullptr)
		{
			m_algoArray.get<1>().Release();
		}
	}
#else
	m_algoArray.get<0>().Release();
#endif

	m_vFuncInfo.clear();
	m_vFuncInfoDebug.clear();

	m_dictFuncTiming.clear();
	m_vModelImage.clear();

	m_imgIndex        = -1;
	m_crtErrorImg.Reset();
	m_crtImage.Reset();
	m_nCrtProductID = m_crtTask.m_nProductId;
	if(!m_crtTask.m_bSelfLearning)
	{
		Proc.MemProc = &CImageProcessor::ImageCheckingThread;
	}
	else
	{
		m_bModelImgReady = false;
		Proc.MemProc = &CImageProcessor::SelfLearningThread;
	}

	UINT threadId;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Proc.ThreadProc, (LPVOID)this, 0, &threadId);
	if(m_hThread == NULL)
	{
		AfxMessageBox(_T("Starting thread failed"), MB_OK);
	}

	//::SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);
// 	if (m_pStationOwner->m_chIndex ==1)
// 	{
// 		SetThreadAffinityMask(m_hThread,0x00000000);
// 	}
// 	if (m_pStationOwner->m_chIndex ==2)
// 	{
// 		SetThreadAffinityMask(m_hThread,0x00000002);
// 	}
// 	if (m_pStationOwner->m_chIndex ==3)
// 	{
// 		SetThreadAffinityMask(m_hThread,0x00000004);
// 	}
// 	if (m_pStationOwner->m_chIndex ==4)//
// 	{
// 
// 		SetThreadAffinityMask(m_hThread,0x000000f0);
// 	}
// 	if (m_pStationOwner->m_chIndex ==5)
// 	{
// 		SetThreadAffinityMask(m_hThread,0x00000f00);
// 	}
	if (m_pStationOwner != NULL)
	{
		m_pStationOwner->m_vChThreadHandle.push_back(m_hThread);
	}
	gSystemMgr.m_vChThreadHandle.push_back(m_hThread);

}

void CImageProcessor::Stop()
{
	IMAGE_BOX lastOne;
	lastOne.m_bLastFlag = true;
	ReceiveImage(lastOne);
}

std::vector<FUNC_INFO>* CImageProcessor::GetFuncInfoArrayPtr()
{
	if (m_pStationOwner)
	{
		return &(m_pStationOwner->m_vFuncInfo);
	}
	return NULL;
}

std::vector<FUNC_INFO>* CImageProcessor::GetDebugFuncInfoArrayPtr()
{
	if (m_pStationOwner)
	{
		return &(m_pStationOwner->m_vFuncInfoDebug);
	}
	return NULL;
}

void CImageProcessor::GetCrtDebugImgErrorRegion(std::map<Hlong,Hobject>&  CurUnionErrorRgn)
{
	CurUnionErrorRgn = m_crtImgProduct_Debug.m_unionErrRgn;
}
//显示图片优先级  中间图片=》校正图片=》原始图片
void CImageProcessor::GetCrtDebugInterImageObject(Hobject&  crtInterImage)
{
	crtInterImage = m_crtImgProduct_Debug.m_cvInterImgs;
}

void CImageProcessor::GetCrtDebugImageObject(Hobject&  crtImage)
{
	if (m_crtImgProduct_Debug.m_crtCorrectImg.Id() != H_EMPTY_REGION)
	{
			crtImage = m_crtImgProduct_Debug.m_crtCorrectImg;
	}
	else
	{
			crtImage = m_crtImgProduct_Debug.m_img;
	}

}
void CImageProcessor::GetCrtDebugImgTargetRegion(Hobject& crtTargetRgn)
{
	crtTargetRgn = m_crtImgProduct_Debug.m_crtTargetRgn;
}

void CImageProcessor::GetCrtDebugImgTargetRegionEx(Hobject& crtTargetRgnEx)
{
	crtTargetRgnEx = m_crtImgProduct_Debug.m_crtTargetRgnEx;
}

void CImageProcessor::SetDispErrRgnFlag(bool bDispErrRgn)
{
	m_bDispErrRgn = bDispErrRgn;
}

int CImageProcessor::GetModelImageCount()
{
	return (int)m_vModelImage.size();
}

void CImageProcessor::DisplayModelImage(int nIndex, const wstring& wstrParam)
{
	if(nIndex < 0 || nIndex > (int)m_vModelImage.size() - 1)
		return;
	MODEL_IMAGE model =  m_vModelImage[nIndex];
	if(model.m_image.Id() == H_EMPTY_REGION)
		return;
	m_wstrSlParam = wstrParam;
	IMAGE_BOX imgBox;
	imgBox.m_img       = model.m_image;
	imgBox.m_bDebugImg = true;
	m_fCrtModelScore   = model.m_fScore;
	ReceiveImage(imgBox);
}

bool CImageProcessor::AddModelImage(Hobject& modelImg, CString sFolder)
{
	if(modelImg.Id() == H_EMPTY_REGION)
		return false;

	CComBSTR bstrFolder(sFolder);
	UINT tmpId = (UINT)m_crtTask.m_nProductId;
	BYTE productId = (BYTE)tmpId;
	HRESULT hr = E_FAIL;

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		if(m_algoArray.get<0>() != nullptr)
		{
			hr = m_algoArray.get<0>()->AddModelImage((long*)&modelImg, &bstrFolder, productId);
		}
	}
	else
	{
		if(m_algoArray.get<1>() != nullptr)
		{
			hr = m_algoArray.get<1>()->AddModelImage((long*)&modelImg, &bstrFolder, productId);
		}
	}
#else
	if(m_algoArray.get<0>() != nullptr)
	{
		hr = m_algoArray.get<0>()->AddModelImage((long*)&modelImg, &bstrFolder, productId);
	}
#endif
	return utils::CheckResult(hr);
}

bool CImageProcessor::DeleteModeImage(unsigned int imgIndex)
{
	if(m_vModelImage.empty())
		return false;
	std::vector<MODEL_IMAGE> vImg = m_vModelImage;
	m_vModelImage.clear();
	for(int t = 0; t < vImg.size(); t++)
	{
		if(t != imgIndex)
			m_vModelImage.push_back(vImg[t]);
	}

	UINT tmpId = (UINT)m_crtTask.m_nProductId;
	BYTE productId = (BYTE)tmpId;
	HRESULT hr = E_FAIL;
#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		if(m_algoArray.get<0>() != nullptr)
		{
			hr = m_algoArray.get<0>()->DeleteModelImage(imgIndex, productId);
		}
	}
	else
	{
		if(m_algoArray.get<1>() != nullptr)
		{
			hr = m_algoArray.get<1>()->DeleteModelImage(imgIndex, productId);
		}
	}
#else
	hr = m_algoArray.get<0>()->DeleteModelImage(imgIndex, productId);
#endif
	return utils::CheckResult(hr);
}

bool CImageProcessor::SaveTrainningResult(wstring& wstrFolder)
{
	CComBSTR bstrFolder(wstrFolder.c_str());
	UINT tmpId = (UINT)m_crtTask.m_nProductId;
	BYTE productId = (BYTE)tmpId;
	HRESULT hr = E_FAIL;

#ifdef EOE_PRODUCT

	CString str;
	if (m_cameraGroupIndex !=-1)
	{
		str.Format(_T("\\S%d"),m_cameraGroupIndex);
	}
	wstring NewStrFolder = wstrFolder + str.GetString();
	_wmkdir(NewStrFolder.c_str());
	CComBSTR newbstrFolder(NewStrFolder.c_str());
	hr = m_algoArray.get<0>()->SaveResult(&newbstrFolder, productId);

	CString strfilenameSource;
	CString StrfilenameDest;
	strfilenameSource.Format(_T("%s\\S%d.tiff"),NewStrFolder.c_str(),m_cameraGroupIndex);
	StrfilenameDest.Format(_T("%s\\S%d.tiff"),wstrFolder.c_str(),m_cameraGroupIndex);
	CopyFile(strfilenameSource,StrfilenameDest,FALSE);

#elif CAN_EOE_PRODUCT

	if(m_nCrtProductID == EOE)
	{
		CString str;
		if (m_cameraGroupIndex !=-1)
		{
			str.Format(_T("\\S%d"),m_cameraGroupIndex);
		}
		wstring NewStrFolder = wstrFolder + str.GetString();
		_wmkdir(NewStrFolder.c_str());
		CComBSTR newbstrFolder(NewStrFolder.c_str());
		hr = m_algoArray.get<0>()->SaveResult(&newbstrFolder, productId);

		CString strfilenameSource;
		CString StrfilenameDest;
		strfilenameSource.Format(_T("%s\\S%d.tiff"),NewStrFolder.c_str(),m_cameraGroupIndex);
		StrfilenameDest.Format(_T("%s\\S%d.tiff"),wstrFolder.c_str(),m_cameraGroupIndex);
		CopyFile(strfilenameSource,StrfilenameDest,FALSE);
	}
	else
	{
		if(m_algoArray.get<1>() != nullptr)
		{
			hr = m_algoArray.get<1>()->SaveResult(&bstrFolder, productId);
		}
	}

#else
	CComBSTR newbstrFolder(wstrFolder.c_str());
	hr = m_algoArray.get<0>()->SaveResult(&newbstrFolder, productId);

#endif

	return utils::CheckResult(hr);
}

void CImageProcessor::SetSelfLearningTargetTaskName(wstring wstrTaskName)
{
	CComBSTR bstrTaskName(wstrTaskName.c_str());

#ifdef CAN_PRODUCT
	CString strchName;
	CComBSTR bstrChName;

	if (m_pStationOwner != NULL)
	{
		strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
		bstrChName = strchName.AllocSysString();

		wstring m_strExePath;
		utils::GetExePath(m_strExePath);
		wstring m_wstrTaskPath = m_strExePath +L"\\data\\"+ strchName.GetString();
		utils::CreateFullDirectory(m_wstrTaskPath);
	}
	m_algoArray.get<0>()->SetSelfLearningTargetTaskName(&bstrTaskName,&bstrChName);

#elif CAN_EOE_PRODUCT

	if(m_nCrtProductID == EOE)
	{
		m_algoArray.get<0>()->SetSelfLearningTargetTaskName(&bstrTaskName);
	}
	else
	{
		CString strchName;
		CComBSTR bstrChName;

		if (m_pStationOwner != NULL)
		{
			strchName.Format(_T("CH%d%d"),m_pStationOwner->m_chIndex,m_cameraGroupIndex);
			bstrChName = strchName.AllocSysString();

			wstring m_strExePath;
			utils::GetExePath(m_strExePath);
			wstring m_wstrTaskPath = m_strExePath +L"\\data\\"+ strchName.GetString();
			utils::CreateFullDirectory(m_wstrTaskPath);
		}
		m_algoArray.get<1>()->SetSelfLearningTargetTaskName(&bstrTaskName,&bstrChName);
	}
#else
	m_algoArray.get<0>()->SetSelfLearningTargetTaskName(&bstrTaskName);
#endif
}

void CImageProcessor::SetCrtSelectedFuncIndex(int funcIndex)
{
	m_crtSelectedFuncIndex = funcIndex;
}

void CImageProcessor::ResetDisplayRule()
{
	m_crtSelectedFuncIndex = -1;;
}

void CImageProcessor::ResetDisplayWindow()
{
	if(m_crtTask.m_bSelfLearning)
		return;

	ResetDisplayRule();

	IMAGE_BOX firstImg;
	if(m_crtErrorImg.Id() != H_EMPTY_REGION)
	{
		firstImg.m_img = m_crtErrorImg;
		firstImg.m_bDebugImg = true;
		firstImg.m_bSwitchImg = true;
		ReceiveImage(firstImg);
		return;
	}
}

void CImageProcessor::ChangeImageGrabbingMode()
{
	ResetDisplayRule();

	IMAGE_BOX imgBox;
	while(m_imgBuffer.is_not_empty())
	{
		m_imgBuffer.pop_back(&imgBox);
	}

	m_vModelImage.clear();
	m_imgIndex        = -1;
}

bool CImageProcessor::IsModelImageReady()
{
	return m_bModelImgReady;
}

void CImageProcessor::ResetSelfLearningProcedure()
{
	m_imgIndex = 0;
	m_vModelImage.clear();
	m_bModelImgReady = false;
#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		if(m_algoArray.get<0>() != nullptr)
		{
			m_algoArray.get<0>()->ResetSelfLearningProcedure();	
		}
	}
	else
	{
		if(m_algoArray.get<1>() != nullptr)
		{
			m_algoArray.get<1>()->ResetSelfLearningProcedure();	
		}
	}
#else
	if(m_algoArray.get<0>() != nullptr)
	{
		m_algoArray.get<0>()->ResetSelfLearningProcedure();
	}
#endif
}

void CImageProcessor::ReportStationStatus2ChUi(bool bOk)
{
	if(m_pStationOwner != nullptr)
	{
		int flag = ((bOk == true)? 1:0);
		::PostMessage(m_pStationOwner->m_hWnd, MSG_CH_STATION_STATE_CHANGED, 0, (LPARAM)flag);
	}
}

void CImageProcessor::ResetInternalStats()
{
	ResetDisplayRule();

	IMAGE_BOX imgBox;
	while(m_imgBuffer.is_not_empty())
	{
		m_imgBuffer.pop_back(&imgBox);
	}
	m_vModelImage.clear();

	m_imgIndex = -1;
}

void CImageProcessor::ReloadExternalFile(wstring wstrRgnFile)
{
	CComBSTR bstrPath(wstrRgnFile.c_str());

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		if(m_algoArray.get<0>() != nullptr)
		{
			m_algoArray.get<0>()->ReloadExternalFile(&bstrPath);	
		}
	}
	else
	{
		if(m_algoArray.get<1>() != nullptr)
		{
			m_algoArray.get<1>()->ReloadExternalFile(&bstrPath);	
		}
	}
#else
	if(m_algoArray.get<0>() != nullptr)
	{
		m_algoArray.get<0>()->ReloadExternalFile(&bstrPath);	
	}
#endif
}

void CImageProcessor::EnableFunc(int nFuncIndex, bool bEnable, bool bTestFunc)
{
	int sz = (int)m_vFuncInfo.size();
	if(nFuncIndex < sz)
	{
		::EnterCriticalSection(m_pParamDataCs);
		m_vFuncInfo[nFuncIndex].m_bEnable = bEnable;
		if(Check_GlueInjectorAvailability(m_vFuncInfo[nFuncIndex].m_strFuncName))
			m_compoundStatsFuncID= bEnable==FALSE?-1:m_compoundStatsFuncID;
		m_vFuncInfoDebug[nFuncIndex].m_bEnable = bEnable;
		if (bEnable)
		{
			for(int k = 0; k< m_vFuncInfo[nFuncIndex].m_vParam.size();k++)
			{
				if (m_vFuncInfo[nFuncIndex].m_vParam[k].m_nFileType!=-1 && m_vFuncInfo[nFuncIndex].m_vParam[k].m_wstrFile!=L" " )
				{
					ReloadExternalFile(m_vFuncInfo[nFuncIndex].m_vParam[k].m_wstrFile);
				}
			}
		}

		if(!bTestFunc)
		{
			for(int i = nFuncIndex + 1; i < sz; i++)
			{
				if(m_vFuncInfo[i].m_bTestFunc)
				{
					m_vFuncInfo[i].m_bEnable = bEnable;
					m_vFuncInfoDebug[i].m_bEnable = bEnable;
					if (bEnable)
					{
						for(int k = 0; k< m_vFuncInfo[i].m_vParam.size();k++)
						{
							if (m_vFuncInfo[i].m_vParam[k].m_nFileType!=-1 && m_vFuncInfo[i].m_vParam[k].m_wstrFile!=L" " )
							{
								ReloadExternalFile(m_vFuncInfo[i].m_vParam[k].m_wstrFile);
							}
						}
					}

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
					if(!m_vFuncInfo[i].m_bTestFunc)
					{
						m_vFuncInfo[i].m_bEnable = bEnable;
						m_vFuncInfoDebug[i].m_bEnable = bEnable;

						if (bEnable)
						{
							for(int k = 0; k< m_vFuncInfo[i].m_vParam.size();k++)
							{
								if (m_vFuncInfo[i].m_vParam[k].m_nFileType!=-1 && m_vFuncInfo[i].m_vParam[k].m_wstrFile!=L" " )
								{
									ReloadExternalFile(m_vFuncInfo[i].m_vParam[k].m_wstrFile);
								}
							}
						}

						break;
					}
				}
			}
		}
		::LeaveCriticalSection(m_pParamDataCs);
	}
}

void CImageProcessor::PassInParam(HTuple CameraIndexOfFlag,HTuple HomMat2DForFlag,HTuple LeftCol4ImgsInStd,HTuple RightCol4ImgsInStd)
{
#ifdef CAN_OUTSIDE_PRODUCT
	m_algoArray.get<0>()->ReceivePrerequisiteParam((LONG*)&CameraIndexOfFlag,(LONG*)&HomMat2DForFlag,(LONG*)&LeftCol4ImgsInStd,(LONG*)&RightCol4ImgsInStd);
#endif
}

bool CImageProcessor::GetSpecialParamsStr(CString& strParams)
{
#if  defined (EOE_PRODUCT)||defined (PLUG_PRODUCT)||defined (HDPE_PRODUCT)  ||defined (MAT_PRODUCT)||defined (BATTERY_PRODUCT) ||defined (BOTTLE_PRODUCT) ||defined (AL_PLASTIC_PRODUCT)
	BSTR bstrText = m_algoArray.get<0>()->GetCurrentLocationXY();
	strParams = bstrText;
#endif

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		BSTR bstrText = m_algoArray.get<0>()->GetCurrentLocationXY();
		strParams = bstrText;
	}
#endif
	return true;
}

void CImageProcessor::GetCenterLocation(CString& strParams)
{
#ifdef BATTERY_PRODUCT
	BSTR bstrText = m_algoArray.get<0>()->GetCenterLocation();
	strParams = bstrText;
#endif

#ifdef AL_PLASTIC_PRODUCT
	BSTR bstrText = m_algoArray.get<0>()->GetCenterLocation();
	strParams = bstrText;
#endif

#ifdef EOE_PRODUCT
	BSTR bstrText = m_algoArray.get<0>()->GetCenterLocation();
	strParams = bstrText;

#endif

#ifdef BOTTLE_PRODUCT
	BSTR bstrText = m_algoArray.get<0>()->GetCenterLocation();
	strParams = bstrText;

#endif

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProductID == EOE)
	{
		BSTR bstrText = m_algoArray.get<0>()->GetCenterLocation();
		strParams = bstrText;
	}
#endif
}

bool CImageProcessor::HasGlueInjectorTracingFunc()
{
	return (m_compoundStatsFuncID != -1);
}