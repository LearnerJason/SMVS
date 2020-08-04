#pragma once
#include <vector>
#include <io.h>
#include <map>
#include <numeric>
#include <algorithm>
#include <atlstr.h>
#include <string>
#include <boost\algorithm\string.hpp>
#ifndef   DLLEXPT
#define DLLEXPT extern "C" __declspec (dllexport )
#endif

#include <boost\lexical_cast.hpp>
#include "cpp/HalconCpp.h"
#include "resource.h"  
#include "HdpeAlgo_i.h"

#include "../AlgoUtils/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"
#include <string>
using namespace std;
using namespace boost;
using namespace Halcon;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "ATL framework error!"
#endif

using namespace ATL;

void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;
}

class ATL_NO_VTABLE CHdpeAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHdpeAlgo, &CLSID_HdpeAlgo>,
	public IDispatchImpl<IHdpeAlgo, &IID_IHdpeAlgo, &LIBID_HdpeAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<ISelfLearning, &IID_ISelfLearning, &LIBID_HdpeAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CHdpeAlgo():m_vPos(128),m_vRgn(128),m_vRgnReadData(128),m_vErrorRgn(128),m_ringPara(128),m_sectorAngle(128)
	{
		m_bOcrRegionFlag  = false;
		m_imageNumber     = 0;
		m_modelCenterRow  = 0.0;
		m_modelCenterCol  = 0.0;
		m_isFirst         = true;
		m_bDebugImg       = false;
		m_bPilotImg       = false;
		m_isFirstMatchingErrorMessage=true;
		m_angleDetect     =1.57;
		/*******************/
		m_oldRadius       = -1;
		m_oldDetectType   = -1;
		/******************/
		m_imageRadius     = 100;
		m_ocrModelIds1 = HTuple();
		m_ocrModelIds2 = HTuple();
		m_ocrNumbers1 = HTuple();
		m_ocrNumbers2 = HTuple();
		m_ocrNumber=0;
		m_firstUse=true;
		//
		gen_empty_obj(&m_regEdgesArea);
		gen_empty_obj(&m_regProfileArea);
		gen_empty_obj(&m_regSmallRegular);
		//
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
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_HDPEALGO1)


	BEGIN_COM_MAP(CHdpeAlgo)
		COM_INTERFACE_ENTRY(IHdpeAlgo)
		COM_INTERFACE_ENTRY(ISelfLearning)
		COM_INTERFACE_ENTRY(IDispatch)
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
	}
private:
	std::vector<PIXEL_POS>             m_vPos;          // max size 128
	std::vector<Hobject>               m_vRgn;          // max size 128
	std::vector<HTuple>                m_vRgnReadData;  // max size 128
	std::vector<Hobject>               m_vErrorRgn;     // max size 128
	std::map<int, std::vector<float>>  m_dictSpecialData; 
	std::vector<RING_PARA>             m_ringPara;
	std::vector<SECTOR_PARA>           m_sectorAngle;
	Hobject                            m_crtImg;    // current image.
	HTuple                             m_imageRadius;
	CAlgoSpy                           m_algoSpy;

	bool                               m_bDebugImg;
	bool                               m_bOcrRegionFlag;
	bool                               m_isFirst;
	bool                               m_bPilotImg;
	long int                           m_imageNumber;
	HTuple                             m_modelCenterRow;
	HTuple                             m_modelCenterCol;
	long                               m_imgWidth;
	long                               m_imgHeight;
	volatile bool                      m_bFileUpdated;
	double                             m_angleDetect;
	double                             m_oldRadius;
	double                             m_oldDetectType;
	HTuple                             m_modelId;
	bool                               m_isChineseLanguage;
	bool                               m_isFirstMatchingErrorMessage;
	//for ocr number detect
	HTuple                             	m_ocrModelIds1 ;
	HTuple                             	m_ocrModelIds2;
	HTuple                             	m_ocrNumbers1;
	HTuple                             	m_ocrNumbers2;
	int                                 m_ocrNumber;
	bool                                m_firstUse;
	//direction detect para
	wstring                            m_wstrTaskPath,m_wstrChannelPath,m_strTaskName;
	string                             m_strTaskPath,m_strChannelPath;
	HTuple                             m_imageFolder1, m_modelFolder1,m_imageFolder2, m_modelFolder2;
	//
	void(ResizeImage)(Hobject Image, Hobject* ImageNew,HTuple withStand,HTuple Width,HTuple Height);
	bool ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA);

public:
	//
	ImageProcess ImageProcessAlg;
	HTuple  m_modelMatchingPARA;
	HTuple  m_modelPARA;
	// For updating external files
	std::map<string, Hobject> m_dictNewPathRgn;     // File Path <===> Region
	std::map<string, Hobject> m_dictCrtPathRgn;     // File Path <===> Region

	std::map<string, HTuple>  m_dictNewPathModelID; // File Path <== Model ID
	std::map<string, HTuple>  m_dictCrtPathModelID; // File Path <== Model ID
	//单接口
	Hobject                   m_DetectReg,m_crtImgMult;
	Hobject                   m_ContourRegs;
	Hobject                   m_BottomSidesRgn;
	Hobject                   m_WhiteTROI;
	Hobject                   m_BlackTROI;

	HTuple                    m_CenterRow;
	HTuple                    m_CenterColumn;
	HTuple                    m_MoveRow;
	HTuple                    m_MoveCol;
	HTuple                    m_rectTopX;
	HTuple                    m_rectTopY;
	//塑料盖
	Hobject                   m_regEdgesArea;
	Hobject                   m_regProfileArea;
	Hobject                   m_regSmallRegular;
	//

	Hobject m_modelImageOCR,m_modelImageTransOCR;
	//
	STDMETHOD(SetCurrentImage)(LONG* pImg);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen,FLOAT* pStatsArray, LONG* nActualLen);
	STDMETHOD(ReloadExternalFile)(BSTR* bstrPath);
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
	STDMETHOD(ModelCreate1)(double Radius, HTuple * ModelID);
	STDMETHOD(CenterLocationByMatchAlgo)(VARIANT* rgnID,VARIANT* pDetectType, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationByMatchAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CenterLocationAlgo1)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationUseEdgesAlgo)(VARIANT* rgnID, VARIANT* pRadius,VARIANT* pThresholdLow,VARIANT* pThresholdHigh,VARIANT* pMinCircularity);
	STDMETHOD(CenterLocationUseEdgesAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD(CenterLocationAlgoHS)(VARIANT* rgnId,VARIANT* pDetectType, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterLocationAlgoHSHelp)(BSTR* pHelpStr);
	STDMETHOD(RectLocationAlgoHS)(VARIANT* rgnId,VARIANT* pDetectType, VARIANT* pThresholdLow ,VARIANT* pOutlierRemoverSize);
	STDMETHOD(RectLocationAlgoHSHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationAlgoGapHS)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pClosingSize,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterLocationAlgoGapHSHelp)(BSTR* pHelpStr);
	STDMETHOD(MatchingParaAlgo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MatchingParaMinScoreAlgo)(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels,VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaMinScoreAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GapAngleDetectAlgo)(VARIANT* rgnId,VARIANT* pDetectType,VARIANT* pBinThred,VARIANT* pEdgeDist, VARIANT* pErosionSize,  VARIANT* pDetectAngle);
	STDMETHOD(GapAngleDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(OcrAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectAngle);
	STDMETHOD(OcrAngleDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(OcrAngleDetectAlgo1)(VARIANT* rgnId, VARIANT* pAreaRatio,VARIANT* pDetectAngle);
	STDMETHOD(OcrAngleDetectAlgo1Help)(BSTR* pHelpStr);


	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SectorRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2);	
	STDMETHOD(SectorRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RectRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRgnWidth,VARIANT* pRgnHeight,VARIANT* pCenterMoveR,VARIANT* pCenterMoveC);
	STDMETHOD(RectRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile);
	STDMETHOD(DetectRegionLocationAlgoHelp)(BSTR* pHelpStr);
	//
	STDMETHOD(DetectRegionEdgesAlgo)(VARIANT* rgnId,VARIANT* pSeriousBlackPointDynThresh,VARIANT* pErosionSize, BSTR* bstrRgnFile);
	STDMETHOD(DetectRegionEdgesAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectRegionRegularAlgo)(VARIANT* rgnId,VARIANT* pRegMinWidth,VARIANT* pErosionSize);
	STDMETHOD(DetectRegionRegularAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectRegionIrregularAlgo)(VARIANT* rgnId,VARIANT* pErosionSize);
	STDMETHOD(DetectRegionIrregularAlgoHelp)(BSTR* pHelpStr);
	//
	STDMETHOD(DetectRegionLocationAlgoOCR)(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile);
	STDMETHOD(DetectRegionLocationAlgoOCRHelp)(BSTR* pHelpStr);

	STDMETHOD(CurlOptimizeAlgo)(VARIANT* rgnId, VARIANT* pFreq_start, VARIANT* pFreq_cutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CurlOptimizeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgoOCR)(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize,VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoOCRHelp)(BSTR* pHelpStr);
	STDMETHOD(BubbleDetectProfileAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(BubbleDetectProfileAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlNoCompoundAlgo)(VARIANT* rgnId,VARIANT*pGrayValueMean );
	STDMETHOD(CurlNoCompoundAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(UVCurlAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue, VARIANT* pDetectArea);
	STDMETHOD(UVCurlAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdAlgo)( VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD (BlackDetectExcludeEdgesAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize, VARIANT* pThresholdLow,  VARIANT* pMinEdgeSize, VARIANT* pDetectArea);
	STDMETHOD (BlackDetectExcludeEdgesAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (BlackDetectAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,  VARIANT* pDetectArea);
	STDMETHOD (BlackDetectAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (BlackDetectUseUpSizeAlgo)(VARIANT* rgnId, VARIANT* pBlackThresh,VARIANT* pThredForeground,VARIANT* pWhiteSize,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pDetectArea);
	STDMETHOD (BlackDetectUseUpSizeAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD(EdgeAlgo)(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(LineDetectionAlgo)(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize, VARIANT* pDefectArea);
	STDMETHOD(LineDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(LineDetectionExclude90Algo)(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize, VARIANT* pDefectArea);
	STDMETHOD(LineDetectionExclude90AlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(TabDetectProfileAlgo)(VARIANT* rgnId,  VARIANT* pThredBackground,  VARIANT* pThredForeground,VARIANT* pAnglePointSize,VARIANT* pMinAngle, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea);
	STDMETHOD(TabDetectProfileAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CreateOcrModelAlgo)(VARIANT* rgnId,VARIANT* pMaxNumber,VARIANT* pContrastMin,VARIANT* pContrast, VARIANT* pEdgeSize,VARIANT* pDilationRow, VARIANT* pDilationCol,VARIANT* pOcrHeight, VARIANT* pOcrWidth,VARIANT*pSampleNumber );
	STDMETHOD(CreateOcrModelAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(FindOcrNumberAlgo)(VARIANT* rgnId,VARIANT* pMaxNumber,VARIANT* pContrastMin,VARIANT* pContrast, VARIANT* pEdgeSize,VARIANT* pDilationRow, VARIANT* pDilationCol,VARIANT* pOcrHeight, VARIANT* pOcrWidth,VARIANT*pOcrNumber );
	STDMETHOD(FindOcrNumberAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName,BSTR* bstrChName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);
	STDMETHOD(GetCurrentLocationXY)(BSTR* bstrLocationXY);
	////
	STDMETHOD(DynThresholdForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(EdgeForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgo)( VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD(CurlNoCompoundForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect,VARIANT* pGrayValueMean);
	STDMETHOD(CurlNoCompoundForColorAlgoHelp)(BSTR* pHelpStr);

};

OBJECT_ENTRY_AUTO(__uuidof(HdpeAlgo), CHdpeAlgo)
