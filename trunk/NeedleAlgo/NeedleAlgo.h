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
#include "resource.h"   
#include "NeedleAlgo_i.h"
#include "../AlgoUtils/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"
using namespace std;
using namespace boost;
using namespace Halcon;
using namespace ATL;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "ATL Platform Error!"
#endif

void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;
}

class ATL_NO_VTABLE CNeedleAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CNeedleAlgo, &CLSID_NeedleAlgo>,
	public ISupportErrorInfo,
	public IDispatchImpl<INeedleAlgo, &IID_INeedleAlgo, &LIBID_NeedleAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CNeedleAlgo():m_vPos(128),m_vPicPos(128),m_vRgn(128),m_vRgnReadData(128),m_vErrorRgn(128),m_ringPara(128)
	{
		m_imageNumber     = 0;
		m_isFirst         = true;
		m_bDebugImg       = false;
		m_bPilotImg       = false;

		m_nCrtInjectorID  =-1;
		m_imageRadius     = 100;
		m_modelNumber     = 0;
		m_modelCenterRow  = 0.0;
		m_modelCenterCol  = 0.0;
		m_modelCenterRowTab  = 0.0;
		m_modelCenterColTab  = 0.0;
		m_modelRadius     = 150;
		m_bFileUpdated    = false;
		m_imageNumber     =0;
		m_isFirstReadSelfLearningPara = true;
		m_isFirstMatchingErrorMessage=true;
		m_gaussTransThred             = 30;
		m_gaussExtractThred           = 15;
		m_dynExtractThredSingle       = 20;
		m_dynExtractThredMulti        = 30;
		m_regionRadius                = 140;
		m_regionDilationSize          = 2.0;
		m_smallExtractArea            = 20.0;

		m_dictNewPathRgn.clear();
		m_dictCrtPathRgn.clear();

		m_dictNewPathModelID.clear();
		m_dictCrtPathModelID.clear();
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
		m_readModelState=false;
		m_resetModelForMultOcr=false;
	// NeedleAlgo
		m_aberrationFirst           =true;
		m_FirstJudgement            =true;

	}
DECLARE_REGISTRY_RESOURCEID(IDR_NEEDLEALGO1)

BEGIN_COM_MAP(CNeedleAlgo)
	COM_INTERFACE_ENTRY(INeedleAlgo)
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
private:
	std::vector<PIXEL_POS>             m_vPos;          // max size 128
	std::vector<PIXEL_POS>             m_vPicPos;       // max size 128
	std::vector<Hobject>               m_vRgn;          // max size 128
	std::vector<HTuple>                m_vRgnReadData;  // max size 128
	std::vector<Hobject>               m_vErrorRgn;     // max size 128
    std::vector<RING_PARA>             m_ringPara;
	std::map<int, std::vector<float>>  m_dictSpecialData; 
	Hobject                            m_crtImg;         // current image.
	Hobject                            m_crtImgMult;     // current image,mult.
	HTuple                             m_imageRadius;
	CAlgoSpy                           m_algoSpy;
	bool                               m_isFirst;
	bool                               m_bDebugImg;
	bool                               m_bPilotImg;
	bool                               m_isChineseLanguage;
	DWORD                              m_nSampleSize;
	DWORD                              m_nImgIndex;
	Hobject                            MultiChannelImageTest;
	HTuple                             m_modelRadius;
	Hobject                            m_modelRegion;
	long int                           m_imageNumber;
	HTuple                             m_modelCenterRow;
	HTuple                             m_modelCenterCol;
	long                               m_imgWidth;
	long                               m_imgHeight;
	long                               m_nCrtInjectorID;

	HTuple  m_centerTuple, m_modelID, Models, Model_Num, MatrixID;
	Hobject m_multiChannelModelImage;
	HTuple  ModelsNew,Model_Num_New,MatrixIDNew;
	Hobject m_multiChannelModelImageNew;
	HTuple  ModelsSort,Model_Num_Sort,MatrixIDSort,ScoreSort;
	Hobject m_regLearning; 
	Hobject m_regExtractLearning; 
	Hobject m_imageDistNum,m_imageReg;
	BSTR   m_strRegionExtractPara;
	//
	HTuple m_modelsForOcr,m_matrixForOcr;
	Hobject m_modelImagesForOcr; 
	bool m_readModelState;
	bool m_resetModelForMultOcr;
	//
	Hobject m_regionNew;
	Hobject m_modelImageNew;
	HTuple  m_regionNewData,m_modelIDNew,m_fileIndex;
	bool    m_isFirstReadSelfLearningPara;
	bool    m_isFirstMatchingErrorMessage;
	double	m_gaussTransThred;
	double	m_gaussExtractThred;
	double	m_dynExtractThredSingle;
	double	m_dynExtractThredMulti;
	double	m_regionRadius;
	double	m_regionDilationSize;
	double	m_smallExtractArea;

	// For updating external files
	volatile bool  m_bFileUpdated;
	std::map<string, Hobject> m_dictNewPathRgn;     // File Path <===> Region
	std::map<string, Hobject> m_dictCrtPathRgn;     // File Path <===> Region
	std::map<string, HTuple>  m_dictNewPathModelID; // File Path <== Model ID
	std::map<string, HTuple>  m_dictCrtPathModelID; // File Path <== Model ID

	// NeedleAlgo
	HTuple  m_rectTopX;
	HTuple  m_rectTopY;

	Hobject m_DetectReg;
	Hobject m_detectRoi;
	Hobject m_MainRgn;

	bool    m_aberrationFirst;
	Hobject m_crtImgColor;
    HTuple  m_LMeanStd;
	HTuple  m_AMeanStd;
	HTuple  m_BMeanStd;
	Hobject m_HeadRegion;
	Hobject m_HeadRegion1;
	Hobject m_MiddleRegion;
	Hobject m_EndRegion;
	Hobject m_InnerCircle;

	HTuple  m_HeadRegionArea;
	HTuple  m_EndRegionArea;
	bool    m_FirstJudgement;

	Hobject m_RegionOpening;


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
public:

	ImageProcess ImageProcessAlg;
	//
	HTuple  m_modelPARA;
	Hobject m_modelImageOCR,m_modelImageTransOCR;
	HTuple  m_modelMatchingPARA;
	int m_detectRegionNumber,m_modelNumber;
	//Tab
	HTuple                             m_modelCenterRowTab;
	HTuple                             m_modelCenterColTab;
	HTuple  m_modelPARATab;
	Hobject m_modelImageTabOCR,m_modelImageTransTabOCR;
	//
	STDMETHOD(SetCurrentImage)(LONG* pImg);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetSelfLearningRegion)(LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen,FLOAT* pStatsArray, LONG* nActualLen);
	STDMETHOD(GetGlueInjectorID)(LONG* pInjectorID);
	// self-learning interface methods
	STDMETHOD(SetSelfLearningParam)(DWORD sampleSz, BSTR* bstrParam);
	STDMETHOD(SelectModelImage)(LONG* pCrtImg, BYTE productID);
	STDMETHOD(AddModelImage)(LONG* pImg, BSTR* strFolder, BYTE productID);
	STDMETHOD(DeleteModelImage)(BYTE nIndex, BYTE productID);
	STDMETHOD(SaveResult)(BSTR* strModelNameD, BYTE productID);
	STDMETHOD(GetSelfLearningResult)(LONG*  multiModelImag, long* cMax, float* vImgScore,BYTE productID);
	STDMETHOD(ResetSelfLearningRegion)(BSTR* strRegionExtractPara);
	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);
	STDMETHOD(ReloadExternalFile)(BSTR* bstrPath);
	STDMETHOD(ResetSelfLearningProcedure)(void);
	STDMETHOD(ModelCreate)(double Radius, HTuple * ModelID);/**/
	STDMETHOD(ResetModels)(BYTE productID);
	
	//NeedleAlgo
	STDMETHOD(BeltSideCenterLocationAlgo)(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pThredLower,VARIANT* pThredUpper,VARIANT* pClosingSize);
	STDMETHOD(BeltSideCenterLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(MidDetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pOffset,VARIANT* pClipSwitch,VARIANT* pMovedSize);
	STDMETHOD(MidDetectRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(AberrationDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectArea);
	STDMETHOD(AberrationDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CurveDetectionAlgo)(VARIANT* rgnId, VARIANT* pDetectArea);
	STDMETHOD(CurveDetectionAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SideDynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(SideDynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(LengthDetectionAlgo)(VARIANT* rgnId, VARIANT* pDetectArea);
	STDMETHOD(LengthDetectionAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(HeadDetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pOffset,VARIANT* pClipSwitch);
	STDMETHOD(HeadDetectRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SideCenterLocationAlgo)(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pThredUpper);
	STDMETHOD(SideCenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	/*STDMETHOD(ConcaveDetectAlgo)(VARIANT* rgnId, VARIANT* pClosingSize, VARIANT* pOpeningSize,VARIANT* pLimitArea,VARIANT* pDetectArea);
	STDMETHOD(ConcaveDetectAlgoHelp)(BSTR* pHelpStr);*/

	STDMETHOD(ConcaveDetectAlgo1)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue,VARIANT* pDetectArea);
	STDMETHOD(ConcaveDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(EndCenterLocationAlgo)(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pGrayValue,VARIANT* pLowerValue,VARIANT* pUpValue,VARIANT* pBigRadius,VARIANT* pSmallRadius);
	STDMETHOD(EndCenterLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(EndFaceRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMeanSize,VARIANT* pDynThresholdValue,VARIANT* pInnerRadius,VARIANT* pAreaLimitLower,VARIANT* pAreaLimitUpper,VARIANT* pCircularity,VARIANT* pErosionSize);
	STDMETHOD(EndFaceRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SolidDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectArea);
	STDMETHOD(SolidDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(EccentricityDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectArea);
	STDMETHOD(EccentricityDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (AnnularRotaDynThresholdAlgo)( VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD(DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(EndDynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(EndDynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ExtraMaterialRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pInnerEdge,VARIANT* pOutEdge,VARIANT* pBeltEdge);
	STDMETHOD(ExtraMaterialRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ExtraMaterialDetectAlgo)(VARIANT* rgnId,  VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(ExtraMaterialDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(EndDynThresholdAlgo1)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(EndDynThresholdAlgo1Help)(BSTR* pHelpStr);


	STDMETHOD(EndOpeningGapDetectAlgo)(VARIANT* rgnId,VARIANT* pWhiteOrBlack,VARIANT* pThresholdValue,VARIANT* pWidthMinSize, VARIANT* pWidthMaxSize,VARIANT* pHeightMinSize, VARIANT* pHeightMaxSize,  VARIANT* pDetectWidth);
	STDMETHOD(EndOpeningGapDetectAlgoHelp)(BSTR* pHelpStr);

};

OBJECT_ENTRY_AUTO(__uuidof(NeedleAlgo), CNeedleAlgo)
