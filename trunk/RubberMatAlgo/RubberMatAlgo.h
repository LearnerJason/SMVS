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
#include "RubberMatAlgo_i.h"

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

class ATL_NO_VTABLE CRubberMatAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CRubberMatAlgo, &CLSID_RubberMatAlgo>,
	public IDispatchImpl<IRubberMatAlgo, &IID_IRubberMatAlgo, &LIBID_RubberMatAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<ISelfLearning, &IID_ISelfLearning, &LIBID_RubberMatAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CRubberMatAlgo():m_vPos(128),m_vRgn(128),m_vRgnReadData(128),m_vErrorRgn(128),m_ringPara(128),m_sectorAngle(128)
	{
		m_imageNumber     = 0;
		m_isFirst         = true;
		m_bDebugImg       = false;
	    m_bPilotImg       = false;
		m_isFirstMatchingErrorMessage=true;
		m_angleDetect     =1.57;
		m_oldRadius       = -1;
		m_imageRadius     = 100;
		m_centerCol       =320;

		//ccc
		m_innerCircleRadius  = 100;
		m_smallCircleModelFirst=true;
		m_uniqueCircleModelFirst=true;
		m_circlesNumber=0;
		m_picNumber=0;
		m_picNumber1=0;
		gen_empty_region(&m_UniqueCirclesRegion);
		gen_empty_region(&m_InnerCirclesRegion);
		gen_empty_region(&m_centerLocationRegion);
		gen_empty_region(&m_centerRegion);
		gen_empty_region(&m_outRegion);

		m_centerColAll=HTuple();
		m_upEdgeRowAll=HTuple();
		m_UpperEdgeRow=76;

		m_FirstCenterX=0.0;
		m_FirstCenterY=0.0;
		m_FirstinnerCircleRadius=60;
		m_modelAngle=0.0;
		m_WindowFirst     =true;
		m_phiValue=0.0;
		m_pixelSize       =0.024766355;

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
		m_resetModelForMultOcr=false;
		m_readModelState=false;

	}

DECLARE_REGISTRY_RESOURCEID(IDR_PCCALGO1)


BEGIN_COM_MAP(CRubberMatAlgo)
	COM_INTERFACE_ENTRY(IRubberMatAlgo)
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
	Hobject                            m_crtImgMult;    // current image,mult.
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

	//ccc
	bool                               m_smallCircleModelFirst;
	Hobject                            m_smallCircleContour;
	HTuple							   m_modelID;
	HTuple							   m_innerCircleRadius;

	bool                           	   m_threeCircleLocation;
	HTuple                             m_circlesNumber;
	Hobject                            m_threeCirclesRegion;
	HTuple							   m_phiValue;
	Hobject							   m_UniqueCirclesRegion;
	Hobject							   m_InnerCirclesRegion;


	HTuple                             m_ImageWidth;//侧面图片
	HTuple                             m_ImageHeight;
	HTuple                             m_rectTopY;
	HTuple                             m_rectTopX;
	int                                m_picNumber;
	HTuple                             m_centerColAll;
	HTuple                             m_centerCol;
	int                                m_picNumber1;

	bool							   m_uniqueCircleModelFirst;
	Hobject							   m_uniqueModelContour;
	HTuple                             m_uniqueCircleModelID;
	HTuple                             m_modelRow;
	HTuple                             m_modelCol;
	HTuple                             m_modelAngle;
	HTuple                             m_CircleRadiusOld;

	HTuple                             m_circleNumber;
	HTuple                             m_UpperEdgeRow;
	HTuple                             m_upEdgeRowAll;

	int                                m_oldPalarity;
	Hobject							   m_crtcolorImg;
	Hobject							   m_BaseRegionLine;
	Hobject							   m_EdgeDilation;
	Hobject                            m_centerLocationRegion,m_locationMatBodyRegion;
	float							   m_FirstCenterX;
	float							   m_FirstCenterY;
	HTuple							   m_FirstinnerCircleRadius;
	Hobject		                       m_centerRegion;
	Hobject						       m_outRegion;
	//
	long long						   m_CHImgWndID;
	bool							   m_WindowFirst;
	double                             m_pixelSize;
	Hobject                            m_ModelRegion;
	string                             m_strTaskPath;
	wstring                            m_wstrTaskPath,m_strTaskName;
	float                              m_OldCenterY, m_OldCenterX;
	//模具识别
	HTuple m_modelsForOcr,m_matrixForOcr,m_readModelState;
	Hobject m_modelImagesForOcr;
	bool m_resetModelForMultOcr;

	void(ExtractEdgesPointsDist)(Hobject Image, HTuple Elements, HTuple DetectHeight, HTuple DetectWidth, HTuple Sigma, HTuple Threshold, HTuple Transition, HTuple Select, 
		HTuple Row1, HTuple Column1,HTuple Row2, HTuple Column2, HTuple *ResultRow, HTuple *ResultColumn);

	void(ExtractEdgesPoints)(Hobject Image, HTuple Elements, HTuple DetectHeight, HTuple DetectWidth, HTuple Sigma, HTuple Threshold, HTuple Transition, HTuple Select, 
		HTuple Row1, HTuple Column1,HTuple Row2, HTuple Column2, HTuple *ResultRow, HTuple *ResultColumn,Hobject *Regions);
	void R_EdgePointsFitCurve(Hobject TopSlopSearchReg, Hobject Image,HTuple PartInterval, HTuple LineLength, HTuple RemoveSize, HTuple Threshold,HTuple * DefinedRow, HTuple *DefinedCol);
	void(FitLineUseEdgesPoints)(HTuple Rows,HTuple Cols,HTuple ActiveNum,HTuple *Row1,HTuple *Col1,HTuple *Row2,HTuple *Col2);

	void P_EdgePointsFitCurve(Hobject TopSlopSearchReg, Hobject Image,HTuple PartInterval, HTuple LineLength, HTuple RemoveSize, HTuple * DefinedRow, HTuple *DefinedCol);
	bool ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA);
	bool CRubberMatAlgo::ReadModelsForOcr( BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,bool errorInfoLanguage,HTuple &m_modelsForOcr,HTuple& m_matrixForOcr,Hobject& m_modelImagesForOcr);
	//
	void (my_disp_obj)(const Halcon::Hobject Object);

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

	STDMETHOD (GetCurrentLocationXY)(BSTR* bstrLocationXY);
	
	//
	//STDMETHOD(ModelCreate)(double Radius, HTuple * ModelID);
	STDMETHOD(ModelCreate)(double Radius,int UsePolarity,  HTuple * ModelID);

	STDMETHOD(CenterLocationUseEdgesAlgo)(VARIANT* rgnID, VARIANT* pRadius,VARIANT* pThresholdLow,VARIANT* pThresholdHigh,VARIANT* pMinCircularity);
	STDMETHOD(CenterLocationUseEdgesAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD(CurlOptimizeAlgo)(VARIANT* rgnId, VARIANT* pFreq_start, VARIANT* pFreq_cutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CurlOptimizeAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoHelp)(BSTR* pHelpStr);

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

	STDMETHOD(DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);

	STDMETHOD(CenterLocationModelCreateAlgo)(VARIANT* rgnID,VARIANT* pUsePolarity, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationModelCreateAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CenterRelocationAlgoHS)(VARIANT* rgnId,VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterRelocationAlgoHSHelp)(BSTR* pHelpStr);

	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SideCenterLocationAlgo)(VARIANT* rgnID ,VARIANT*pRowStart,VARIANT*pColStart,VARIANT*pRegWidth,VARIANT*pRegHeight,VARIANT* pContrast);	
	STDMETHOD(SideCenterLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SideCenterLocationAlgo1)(VARIANT* rgnID ,VARIANT*pRowStart,VARIANT*pColStart,VARIANT*pRegWidth,VARIANT*pRegHeight,VARIANT* pThredValueSide,VARIANT* pThredValueUp);	
	STDMETHOD(SideCenterLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(SideCenterLocationAlgo11)(VARIANT* rgnID ,VARIANT*pRowStart,VARIANT*pColStart,VARIANT*pRegWidth,VARIANT*pRegHeight,VARIANT* pGrayValue,VARIANT* pOpeningSizeH,VARIANT*pOpeningSizeV,VARIANT* pThredValueSide,VARIANT* pThredValueUp);	
	STDMETHOD(SideCenterLocationAlgo11Help)(BSTR* pHelpStr);

	STDMETHOD(SideCenterLocationAlgo12)(VARIANT* rgnID ,VARIANT*pGrayValueLow,VARIANT*pGrayValueHigh,VARIANT*pThredValueSide,VARIANT*pRegHeight);
	STDMETHOD(SideCenterLocationAlgo12Help)(BSTR* pHelpStr);

	STDMETHOD(SideRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pWidth,VARIANT* pHeightUpper,VARIANT* pHeightLower);	
	STDMETHOD(SideRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (ThreeCirclesLocationAlgo)(VARIANT* rgnId, VARIANT* pCircleRadius,VARIANT* pMinScore);
	STDMETHOD (ThreeCirclesLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (RegionWithoutCharAlgo)(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pThredValue,VARIANT* pAreaMin,VARIANT*pCompactness,VARIANT*pRowUpLeft,VARIANT*pLength1,VARIANT*pLength2);
	STDMETHOD (RegionWithoutCharAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (ThreeCirclesRegionAlgo)(VARIANT* rgnId,VARIANT*pSelection ,VARIANT* pErosionSize);
	STDMETHOD (ThreeCirclesRegionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (ThreeCirclesRegionAlgo1)(VARIANT* rgnId,VARIANT*pSelection ,VARIANT* pErosionSize0,VARIANT* pErosionSize);
	STDMETHOD (ThreeCirclesRegionAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(OuterRingLocationAlgo)(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pDist,VARIANT* pAngleShift);	
	STDMETHOD(OuterRingLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(UniqueCircleLocationAlgo)(VARIANT* rgnId, VARIANT* pCircleRadius,VARIANT* pMinScore);	
	STDMETHOD(UniqueCircleLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ThreeCirclesLocationAlgo2)(VARIANT* rgnId,  VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT*pSelect,VARIANT* pCircleRadius,VARIANT* pMinScore,VARIANT*pRowUpLeft,VARIANT*pLength1,VARIANT*pLength2,VARIANT*pThredValue);	
	STDMETHOD(ThreeCirclesLocationAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD(ThreeCirclesLocationAlgo1)(VARIANT* rgnId, VARIANT*pSelect,VARIANT* pCircleRadius,VARIANT* pMinScore,VARIANT*pRowUpLeft,VARIANT*pLength1,VARIANT*pLength2,VARIANT*pThredValue);	
	STDMETHOD(ThreeCirclesLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(OuterRingLocationAlgo1)(VARIANT* rgnId,VARIANT*pSelect,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pDist,VARIANT*pBotSize,VARIANT* pAngleShift);	
	STDMETHOD(OuterRingLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(OuterRingLocationAlgo2)(VARIANT* rgnId,VARIANT*pSelect,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pDist,VARIANT*pBotSize,VARIANT* pAngleShift,VARIANT* pThredValue,VARIANT*pGrayValue,VARIANT* pDilationSize);	
	STDMETHOD(OuterRingLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD (RegionLocationByModelTool)(VARIANT*rgnId,BSTR* bstrRgnFile);
	STDMETHOD (RegionLocationByModelToolHelp)(BSTR* pHelpStr);

	STDMETHOD(EdgeDistortionAlgo)(VARIANT* rgnId,VARIANT*pThreshold,VARIANT*pMaxMinSubThreshold,VARIANT*pMaxMinSub);
	STDMETHOD(EdgeDistortionAlgoHelp)(BSTR* pHelpStr);

	//

	STDMETHOD(UpBaseEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pWidthSize, VARIANT* pVerticalPos,VARIANT* pMaskSize, VARIANT* pThredValue, VARIANT* pBaseLineArea);
	STDMETHOD(UpBaseEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ScrapEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pUpMoveSize, VARIANT* pDownMoveSize,VARIANT* pSearchSize, VARIANT* pThredValue,VARIANT* pSensitivity,VARIANT* pEdgeArea);
	STDMETHOD(ScrapEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SideDetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pSelect,VARIANT* pErosioSizeH,VARIANT* pErosioSizeV,VARIANT* pErosioSizeE);	
	STDMETHOD(SideDetectRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ScrapDynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(ScrapDynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ScrapRegionExtraction)(VARIANT* rgnId, VARIANT* pUpMoveSize, VARIANT* pDownMoveSize,VARIANT*pErosioSizeV,VARIANT* pSensitivity);
    STDMETHOD(ScrapRegionExtractionHelp)(BSTR* pHelpStr);

	STDMETHOD(RegionExcludeScrapExtraction)(VARIANT* rgnId,VARIANT* pSelect,VARIANT* pErosioSizeH,VARIANT* pErosioSizeV);
	STDMETHOD(RegionExcludeScrapExtractionHelp)(BSTR* pHelpStr);



	STDMETHOD(InnerEdgeLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pMoveSize,VARIANT* pThresh);	
	STDMETHOD(InnerEdgeLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (UniqueCirclesRegionAlgo)(VARIANT* rgnId,VARIANT*pSelection ,VARIANT* pErosionSize0,VARIANT* pErosionSize);
	STDMETHOD (UniqueCirclesRegionAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DetectRegionLocationExtractOCRAlgo)(VARIANT* rgnId,VARIANT* pErosionSize0,VARIANT* pErosionSize, VARIANT* pCircleErosionSize,BSTR* bstrOcrRgnFile);
	STDMETHOD(DetectRegionLocationExtractOCRAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (DiameterMeasureAlgo)(VARIANT* rgnId,  VARIANT* pPixelSize, VARIANT* pStdDiameter,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (DiameterMeasureAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ScrapDiameterMeasureAlgo)(VARIANT* rgnId,VARIANT* pDiameter);
	STDMETHOD(ScrapDiameterMeasureAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DeformationDetectAlgo)(VARIANT* rgnId,VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pDetectDist);
	STDMETHOD(DeformationDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DynThresholdForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(EdgeForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgo)( VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD(CurlNoCompoundForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect,VARIANT* pGrayValueMean);
	STDMETHOD(CurlNoCompoundForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MoveBlockDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MoveBlockDetectAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea);

	STDMETHOD(ThreeCirclesLocationMorphologyAlgoHelp)(BSTR* pHelpStr);
    STDMETHOD(ThreeCirclesLocationMorphologyAlgo)(VARIANT* rgnId, VARIANT* pRegionRadius, VARIANT* pSobelSize, VARIANT* pMinGrayCircle, VARIANT* pMinGrayChar, VARIANT* pDistanceCo, VARIANT* pRecLength, VARIANT* pRecWidth, VARIANT* pFeature);
	STDMETHOD(OuterRingLocationAlgo3)(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pThredValue,VARIANT*pGrayValue,VARIANT* pDilationSize);
	STDMETHOD(OuterRingLocationAlgo3Help)(BSTR* pHelpStr);
	STDMETHOD(OuterRingLocationPCAlgo)(VARIANT* rgnId, VARIANT* pOuterRadius, VARIANT* pRingWidth, VARIANT* pMode, VARIANT* pDilationSize, VARIANT* pStdAngle, VARIANT* pOutputRadius, VARIANT* pOutputWidth);
	STDMETHOD(OuterRingLocationPCAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SetCurrentTaskName2)(BSTR* bstrTaskName,BSTR* bstrChName);
	STDMETHOD (SideCenterLocationTiltingAlgo)(VARIANT* rgnID ,VARIANT*pRubberMatHeight,VARIANT* pContrast, VARIANT* pErosionSide, VARIANT* pErosionTop);
	STDMETHOD (SideCenterLocationTiltingAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (MultOcrDetectAlgo)(VARIANT* rgnId, VARIANT* pGreediness, BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,VARIANT* pDetectArea);
	STDMETHOD (MultOcrDetectAlgoHelp)( BSTR* pHelpStr);

};

OBJECT_ENTRY_AUTO(__uuidof(RubberMatAlgo), CRubberMatAlgo)
