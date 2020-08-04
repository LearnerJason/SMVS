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
#include "PccAlgo_i.h"

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

class ATL_NO_VTABLE CPccAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPccAlgo, &CLSID_PccAlgo>,
	public IDispatchImpl<IPccAlgo, &IID_IPccAlgo, &LIBID_PccAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<ISelfLearning, &IID_ISelfLearning, &LIBID_PccAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPccAlgo():m_vPos(128),m_vRgn(128),m_vRgnReadData(128),m_vErrorRgn(128),m_ringPara(128),m_sectorAngle(128)
	{
		m_imageNumber     = 0;
		m_isFirst         = true;
		m_bDebugImg       = false;
	    m_bPilotImg       = false;
		m_isFirstMatchingErrorMessage=true;
		m_angleDetect     =1.57;
		m_oldRadius       = -1;
		m_imageRadius     = 100;

		gen_empty_obj(&m_ExpandedImage);  
		gen_empty_obj(&m_rgnBorderLR);
		gen_empty_obj(&m_rgnTopWhole);

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

DECLARE_REGISTRY_RESOURCEID(IDR_PCCALGO1)


BEGIN_COM_MAP(CPccAlgo)
	COM_INTERFACE_ENTRY(IPccAlgo)
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
	bool                               m_isFirst;
	bool                               m_bPilotImg;
	long int                           m_imageNumber;
	long                               m_imgWidth;
	long                               m_imgHeight;
	volatile bool                      m_bFileUpdated;
	double                             m_angleDetect;
	double                             m_oldRadius;
	HTuple                             m_modelId;
	bool                               m_isChineseLanguage;
	bool                               m_isFirstMatchingErrorMessage;
	void(ResizeImage)(Hobject Image, Hobject* ImageNew,HTuple withStand,HTuple Width,HTuple Height);


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
	Hobject                   m_DetectReg;
	Hobject                   m_ContourRegs;
	Hobject                   m_BottomSidesRgn;
	Hobject                   m_WhiteTROI;
	Hobject                   m_BlackTROI;
	Hobject                   m_ExpandedImage;
	Hobject                   m_rgnBorderLR;
	Hobject                   m_rgnTopWhole;

	HTuple                    m_CenterRow;
	HTuple                    m_CenterColumn;
	HTuple                    m_MoveRow;
	HTuple                    m_MoveCol;
	HTuple                    m_rectTopX;
	HTuple                    m_rectTopY;
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
	STDMETHOD(GetCenterLocation)(BSTR* bstrLocationXY);
	//
	STDMETHOD(CenterLocationAlgo)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ModelCreate)(double Radius, HTuple * ModelID);

	STDMETHOD(CenterLocationAlgo1)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationUseEdgesAlgo)(VARIANT* rgnID, VARIANT* pRadius,VARIANT* pThresholdLow,VARIANT* pThresholdHigh,VARIANT* pMinCircularity);
	STDMETHOD(CenterLocationUseEdgesAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD(CenterLocationUseEdgesAlgo1)(VARIANT* rgnID, VARIANT* pRadius,VARIANT* pThresholdLow,VARIANT* pThresholdHigh);
	STDMETHOD(CenterLocationUseEdgesAlgo1Help)( BSTR* pHelpStr);
	STDMETHOD(MatchingParaAlgo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MatchingParaMinScoreAlgo)(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels,VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaMinScoreAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(DetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile);
	STDMETHOD(DetectRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CurlOptimizeAlgo)(VARIANT* rgnId, VARIANT* pFreq_start, VARIANT* pFreq_cutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CurlOptimizeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BubbleDetectProfileAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(BubbleDetectProfileAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BubbleDetectProfileAlgo1)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(BubbleDetectProfileAlgo1Help)(BSTR* pHelpStr);
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
	STDMETHOD(DeformationDetectAlgo)(VARIANT* rgnId, VARIANT* pAlgoSelect, VARIANT* pSmoothSize,VARIANT* pDetectDist);
	STDMETHOD(DeformationDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(TabDetectProfileAlgo)(VARIANT* rgnId,  VARIANT* pThredBackground,  VARIANT* pThredForeground,VARIANT* pAnglePointSize,VARIANT* pMinAngle, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea);
	STDMETHOD(TabDetectProfileAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);

	STDMETHOD(AnnularRotaDynThresholdAlgoBW)(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea);
	STDMETHOD(AnnularRotaDynThresholdAlgoBWHelp)(BSTR* pHelpStr);

	//单接口
	STDMETHOD (MsiUpperSurfaceRegionExtractAlgo)(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pThredLow,VARIANT* pThredUpper);
	STDMETHOD (MsiUpperSurfaceRegionExtractAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiVoidCenterLocationAlgo)(VARIANT* rgnId);
	STDMETHOD (MsiVoidCenterLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiGapDetectAlgo)(VARIANT* rgnId, VARIANT* pSwitch, VARIANT* pVerticalOrLevel,VARIANT* pMeanMaskSize,VARIANT* pSigma, VARIANT* pLower, VARIANT* pUpper,VARIANT* pLengthSize,VARIANT* pLengthDiff);
	STDMETHOD (MsiGapDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiAnnularRotaDynThresholdAlgo)( VARIANT* rgnId,VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (MsiAnnularRotaDynThresholdAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD (MsiCircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pSigma,VARIANT* pLower, VARIANT* pUpper, VARIANT* pAngleDiff);	
	STDMETHOD (MsiCircleRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiDetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile,VARIANT* pSigma,VARIANT* pLower, VARIANT* pUpper,VARIANT* pAngle, VARIANT* pCurv);
	STDMETHOD (MsiDetectRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiEndSidesRegionLocationAlgo)(VARIANT* rgnID ,VARIANT* pRgnSize,VARIANT* pErosionSize,VARIANT* pRowDist);
	STDMETHOD (MsiEndSidesRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (MsiEndSidesRegionLocationAlgo1)(VARIANT* rgnID ,VARIANT* pOuterSize,VARIANT* pInnerSize,VARIANT* pUpSize,VARIANT* pDownSize);
	STDMETHOD (MsiEndSidesRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD (MsiTopWholeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pThreshold,VARIANT* pOpnSize);
	STDMETHOD (MsiTopWholeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiBottomSidesRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pHp,VARIANT* pVp,VARIANT* pWidth,VARIANT* pHeight,VARIANT* pMovedSize,VARIANT* pErosionSize);
	STDMETHOD (MsiBottomSidesRegionLocationAlgoHelp)(BSTR* pHelpStr);
	//

	STDMETHOD (MsiUpperSurfaceCenterRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pVClipSize,VARIANT* pLClipSize,VARIANT*pMoveUpSize,VARIANT*pMoveLowSize);
	STDMETHOD (MsiUpperSurfaceCenterRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiUpperSurfaceTopRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pVClipSize,VARIANT* pLClipSize,VARIANT*pMoveSize);
	STDMETHOD (MsiUpperSurfaceTopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiUpperSurfaceBottomRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pOpeningSize,VARIANT* pVClipSize,VARIANT* pLClipSize,VARIANT*pMoveSize,VARIANT* pErosionSize);
	STDMETHOD (MsiUpperSurfaceBottomRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiUpperSurfaceSidesRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pLClipSize);
	STDMETHOD (MsiUpperSurfaceSidesRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiWhiteReinforcedRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pRegHeight,VARIANT* pRegWidth,VARIANT* pThreshValue,VARIANT* pErosionSizeWidth,VARIANT* pErosionSizeHeight);
	STDMETHOD (MsiWhiteReinforcedRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiBlackReinforcedRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pRegHeight,VARIANT* pRegWidth,VARIANT* pThreshValue,VARIANT* pErosionSizeWidth,VARIANT* pErosionSizeHeight);
	STDMETHOD (MsiBlackReinforcedRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiWhiteReinforcedBlackDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSeriousBlackPointDynThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea);
	STDMETHOD (MsiWhiteReinforcedBlackDetectAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD (MsiBlackReinforcedBlackDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSeriousBlackPointDynThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea);
	STDMETHOD (MsiBlackReinforcedBlackDetectAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD (MsiStarvingDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectArea);
	STDMETHOD (MsiStarvingDetectAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD (MsiUpperSurfaceCenterRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pRegHeight,VARIANT*pMoveUpSize,VARIANT* pLClipSize);
	STDMETHOD (MsiUpperSurfaceCenterRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD (MsiUpperSurfaceBottomRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT*pMoveUpSize,VARIANT* pMoveLowSize);
	STDMETHOD (MsiUpperSurfaceBottomRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(CenterLocationAlgoHS)(VARIANT* rgnId, VARIANT* pThresholdLow,VARIANT* pAreaMax);
	STDMETHOD(CenterLocationAlgoHSHelp)(BSTR* pHelpStr);

	STDMETHOD(DetectRegionLocationExtractOCRAlgo)(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pErosionSize, BSTR* bstrOcrRgnFile);
	STDMETHOD(DetectRegionLocationExtractOCRAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MsiBorderExtraSilkDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pClosingSize,VARIANT* pAreaSize,VARIANT* pDetectArea);
	STDMETHOD (MsiBorderExtraSilkDetectAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD (MsiBorderExtraSilkDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pLowThred,VARIANT* pLengthThred,VARIANT* pClipNum,VARIANT* pSizeThred,VARIANT* pDetectArea);
	STDMETHOD (MsiBorderExtraSilkDetectAlgo1Help)( BSTR* pHelpStr);

	STDMETHOD (MsiBorderRubberLessDetectAlgo)(VARIANT* rgnId, VARIANT* pErosionSize,VARIANT* pAreaSize,VARIANT* pDetectArea);
	STDMETHOD (MsiBorderRubberLessDetectAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD (TopRubberLessDetectAlgo)(VARIANT* rgnId, VARIANT* pDiagonalDist,VARIANT* pDetectArea);
	STDMETHOD (TopRubberLessDetectAlgoHelp)( BSTR* pHelpStr);
};

OBJECT_ENTRY_AUTO(__uuidof(PccAlgo), CPccAlgo)
