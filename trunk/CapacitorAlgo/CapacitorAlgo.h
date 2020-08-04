#pragma once
#include <vector>
#include <io.h>
#include <map>
#include <numeric>
#include <algorithm>
#include <atlstr.h>
#include <string>
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>
#include "cpp/HalconCpp.h"
#include "resource.h"       // 主符号
#include "CapacitorAlgo_i.h"
#include "../AlgoUtils/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"
using namespace std;
using namespace boost;
using namespace Halcon;



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "ATL Error!"
#endif

using namespace ATL;

void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;
}

class ATL_NO_VTABLE CCapacitorAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCapacitorAlgo, &CLSID_CapacitorAlgo>,
	public ISupportErrorInfo,
	public IDispatchImpl<ICapacitorAlgo, &IID_ICapacitorAlgo, &LIBID_CapacitorAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCapacitorAlgo():m_vPos(128),m_vRgn(128),m_vRgnReadData(128),m_vErrorRgn(128),m_ringPara(128),m_sectorAngle(128)
	{
		m_imageNumber     = 0;
		m_isFirst         = true;
		m_bDebugImg       = false;
		m_bPilotImg       = false;
		m_angleDetect     =1.57;
		m_leftCol         =170;
		m_angleSlant      =0.0;
		m_oldRadius       = -1;
		m_locationRadius  = 60;
		LCID lcid ;
		lcid=GetUserDefaultUILanguage();
		if((lcid==2052)||(lcid==1028))
		{
			m_isChineseLanguage=true;
		}
		else
		{
			m_isChineseLanguage=false;
		}
		m_readDataState=false;
		m_addModelState=false;
		m_modelNum=0;
		m_nImgIndex=0;
		m_isFirstReadSelfLearningPara=true;
		//double end para
		m_oldRadiusWidth=-1;
		m_oldRadiusHeight=-1;
		m_oldBottomRow=-1;
		m_oldRegionWidth=-1;
		m_oldRegionHeight=-1;
		m_contrast=30;
		m_minContrast=5;
		m_modelParaOfSelfLearning = HTuple();
		m_modelParaOfSelfLearning.ReplaceElements(0,30);
		m_modelParaOfSelfLearning.ReplaceElements(1,5);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_CapacitorAlgo1)


	BEGIN_COM_MAP(CCapacitorAlgo)
		COM_INTERFACE_ENTRY(ICapacitorAlgo)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HException::InstallHHandler(&MyHalconExceptionHandler);
		return S_OK;
	}

	void FinalRelease()
	{
		m_algoSpy.FushLogCache();
	}

	std::vector<PIXEL_POS>             m_vPos;          // max size 128
	std::vector<Hobject>               m_vRgn;          // max size 128
	std::vector<HTuple>                m_vRgnReadData;  // max size 128
	std::vector<Hobject>               m_vErrorRgn;     // max size 128
	std::map<int, std::vector<float>>  m_dictSpecialData; 
	std::vector<RING_PARA>             m_ringPara;
	std::vector<SECTOR_PARA>           m_sectorAngle;
	Hobject                            m_crtImg;    // current image.
	Hobject                            m_crtImgMult;    // current image,mult.
	HTuple                             m_imageRadius;
	CAlgoSpy                           m_algoSpy;
	bool                               m_isChineseLanguage;
	bool                               m_bDebugImg;
	bool                               m_isFirst;
	bool                               m_bPilotImg;
	long int                           m_imageNumber;
	long                               m_imgWidth;
	long                               m_imgHeight;
	volatile bool                      m_bFileUpdated;
	double                             m_angleDetect;
	double                             m_oldRadius;
	HTuple                             m_modelId;
	HTuple                             m_locationRadius;
	//侧面定位相关参数
	int                                m_leftCol;
	double                             m_angleSlant;
	std::vector<int>                   m_StdHeight; 
	int                                m_height;
	//direction detect para
	wstring                            m_wstrTaskPath,m_strTaskName;
	string                             m_strTaskPath;
	Hobject                            m_mapImg, m_mulImg;
	Hobject                            m_modelRegion;
	HTuple                             m_modelNum,m_angleStart,m_angleExtent;
	HTuple                             m_models;		 //模板组	
	HTuple                             m_modelCenterRow,m_modelCenterCol;		 //模板区域中心坐标	
	HTuple                             m_addModelID;
	bool                               m_addModelState;
	bool                               m_readDataState;
	DWORD                              m_nImgIndex;
	Hobject                            m_multiChannelModelImage,m_multiChannelModelImageRectified;
	int                                m_modelRegionHeight,m_modelMinScore,m_modelAnge,m_readChessImage,m_chessNum,m_contrast,m_minContrast;
	HTuple                             m_modelParaOfSelfLearning;
	BSTR                               m_strRegionExtractPara;
	bool                               m_isFirstReadSelfLearningPara;
	//double end para
	int                                m_oldRadiusWidth,m_oldRadiusHeight,m_oldBottomRow,m_oldRegionWidth,m_oldRegionHeight;
	HTuple                             m_pointsRow,m_pointsRowReset,m_pointsCol,m_highsOffsetAll,m_highsOffset,m_minCol,m_maxCol,m_minRow,m_maxRow;
private:
	void TrimString(std::vector<string>& vStr)
	{
		for(size_t t = 0; t < vStr.size(); ++t)
		{
			boost::trim(vStr[t]);
		}
	}
	std::vector<string> SplitString(string strTarget, string strSeperator)
	{
		std::vector<string> vSplitRslt;
		boost::split(vSplitRslt, strTarget, boost::is_any_of(strSeperator),token_compress_on);
		return vSplitRslt;
	}
	bool (ReadDatasForDirection)( string bstrDataPath,bool errorInfoLanguage,Hobject &MapImg,Hobject& MulImg,Hobject &ModelRegion,HTuple& ModelNum,HTuple& AngleStart,HTuple& AngleExtent,HTuple& Models,HTuple& ModelParaOfSelfLearning);
public:
	//
	ImageProcess ImageProcessAlg;
	HTuple  m_modelPARA;
	Hobject m_modelImageOCR,m_modelImageTransOCR,m_rgnDetect;
	HTuple  m_modelMatchingPARA;
	int m_detectRegionNumber,m_modelNumber;
	//
	STDMETHOD(SetCurrentImage)(LONG* pImg);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen,FLOAT* pStatsArray, LONG* nActualLen);
	STDMETHOD(ReloadExternalFile)(BSTR* bstrPath);
	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);
	// self-learning interface methods
	STDMETHOD(SetSelfLearningParam)(DWORD sampleSz, BSTR* bstrParam);
	STDMETHOD(SelectModelImage)(LONG* pCrtImg, BYTE productID);
	STDMETHOD(DeleteModelImage)(BYTE nIndex, BYTE productID);
	STDMETHOD(SaveResult)(BSTR* strModelNameD, BYTE productID);
	STDMETHOD(AddModelImage)(LONG* pImg, BSTR* strFolder, BYTE productID);
	STDMETHOD(GetSelfLearningResult)(LONG*  multiModelImag, long* cMax, float* vImgScore,BYTE productID);
	STDMETHOD(ResetSelfLearningRegion)(BSTR* strRegionExtractPara);
	STDMETHOD(GetSelfLearningRegion)(LONG* pRgn);
	STDMETHOD(ResetSelfLearningProcedure)(void);
	//
	STDMETHOD(CenterLocationAlgo)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ModelCreate)(double Radius, HTuple * ModelID);
	STDMETHOD(CenterLocationAlgo1)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationAlgoHS)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterLocationAlgoHSHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterRelocationAlgoHS)(VARIANT* rgnId,VARIANT* pRadiusLong,  VARIANT* pRadiusShort, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterRelocationAlgoHSHelp)(BSTR* pHelpStr);
	STDMETHOD (CenterLocationHeightAlgo)(VARIANT* rgnID ,VARIANT* pLeftCol,VARIANT*pRegWidth,VARIANT* pContrast);
	STDMETHOD (CenterLocationHeightAlgoHelp)(BSTR* pHelpStr);

	//
	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(RectangleRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pLeftColOffset,VARIANT* pRightCol,VARIANT* pRegionHeight);	
	STDMETHOD(RectangleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RectangleFixedWidthLocationAlgo)(VARIANT* rgnId,VARIANT* pLeftColOffset,VARIANT* pRegionWidth,VARIANT* pRegionHeight);	
	STDMETHOD(RectangleFixedWidthLocationAlgoHelp)(BSTR* pHelpStr);	
	//
	STDMETHOD(CurlDetectAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea);
	STDMETHOD(CurlDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlMeanGrayDetectAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pGrayValueLower, VARIANT* pGrayValue, VARIANT* pDetectArea);
	STDMETHOD(CurlMeanGrayDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlOptimizeAlgo)(VARIANT* rgnId, VARIANT* pFreq_start, VARIANT* pFreq_cutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CurlOptimizeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AnnularRotaDynThresholdAlgo)(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh,VARIANT* pDefectSize,VARIANT* pDetectArea);
	STDMETHOD(AnnularRotaDynThresholdAlgoHelp)(BSTR* pHelpStr);	

	STDMETHOD(GLineAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea);
	STDMETHOD(GLineAlgoHelp)(BSTR* pHelpStr);	
	STDMETHOD(LineDetectionAlgo)(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize, VARIANT* pDefectArea);
	STDMETHOD(LineDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(EdgeAlgo)(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlNoCompoundAlgo)(VARIANT* rgnId,VARIANT*pGrayValueMean );
	STDMETHOD(CurlNoCompoundAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAvoidReflectionAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAvoidReflectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(HeightDiffDetectionAlgo)(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectSize, VARIANT* pDefectDiff);
	STDMETHOD(HeightDiffDetectionAlgoHelp)(BSTR* pHelpStr);
	//
	STDMETHOD(BubbleDetectProfileAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(BubbleDetectProfileAlgoHelp)(BSTR* pHelpStr);

	//新加算子
	STDMETHOD(GSurfaceAlgo)(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea);
	STDMETHOD(GSurfaceAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SlotDetectAlgo1)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea);
	STDMETHOD(SlotDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(TabDetectProfileAlgo)(VARIANT* rgnId,  VARIANT* pThredBackground,  VARIANT* pThredForeground,VARIANT* pAnglePointSize,VARIANT* pMinAngle, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea);
	STDMETHOD(TabDetectProfileAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(GapDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GapDetectAlgo)(VARIANT* rgnId, VARIANT* pGrayThresh, VARIANT* pClosingCircleSize, VARIANT* pClosingRectSize,  VARIANT* pDetectMinArea,VARIANT* pDetectArea);

	STDMETHOD(DynThresholdAlgoWhite)(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoWhiteHelp)(BSTR* pHelpStr);

	STDMETHOD(CCenterLocationAlgo)(VARIANT* rgnID,VARIANT* pMinScore);
	STDMETHOD(CCenterLocationAlgoHelp)(BSTR* pHelpStr);



};

OBJECT_ENTRY_AUTO(__uuidof(CapacitorAlgo), CCapacitorAlgo)
