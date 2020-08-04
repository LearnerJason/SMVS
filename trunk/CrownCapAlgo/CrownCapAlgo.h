#pragma once
#include "resource.h"     
#include <cpp\HalconCpp.h>
#include <vector>
#include <io.h>
#include "CrownCapAlgo_i.h"
#include <atlstr.h>
#include "../SMVS/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"
using namespace std;
using namespace Halcon;
const int IMAGE_TRAIN_NUM=100;
const int IMAGE_MODEL_NUM=12;
#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE"
#endif

using namespace ATL;
// exception handler
void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;	
}

class ATL_NO_VTABLE CCrownCapAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCrownCapAlgo, &CLSID_CrownCapAlgo>,
	public ISupportErrorInfoImpl<&IID_ICrownCapAlgo>,
	public IDispatchImpl<ICrownCapAlgo, &IID_ICrownCapAlgo, &LIBID_CrownCapAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCrownCapAlgo():m_vPos(128),m_vPicPos(128),m_vRgn(128),m_vErrorRgn(128)
	{
		m_meanGray_m=0.0;
		m_addNumber_m=0.0;
		m_meanGray_c=0.0;
		m_addNumber_c=0.0;
	}
	DECLARE_REGISTRY_RESOURCEID(IDR_EOEALGO1)

	BEGIN_COM_MAP(CCrownCapAlgo)
		COM_INTERFACE_ENTRY(ICrownCapAlgo)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

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
	// Example, apply several algorithm to the same region, then the region should be an internal value;
	std::vector<PIXEL_POS>             m_vPos;      // max size 128
	std::vector<PIXEL_POS>			   m_vPicPos;   // max size 128
	std::vector<Hobject>			   m_vRgn;      // max size 128
	std::vector<Hobject>			   m_vErrorRgn; // max size 128;
	bool                               m_bDebugImg;
	std::map<int, std::vector<float>>  m_dictSpecialData;
	Hobject m_crtImg; // current image.
	HTuple  m_centerTuple, m_modelID, Models, Model_Num, MatrixID;
	Hobject m_multiChannelModelImage;
	HTuple  ModelsNew,Model_Num_New,MatrixIDNew;
	Hobject m_multiChannelModelImageNew;
	HTuple  ModelsSort,Model_Num_Sort,MatrixIDSort,ScoreSort;
	Hobject m_multiChannelModelImageSort;
	HTuple  m_matchingPara_CH1,m_matchingPara_CH2;
	HTuple  m_matchingHomMat2D_CH1,m_matchingHomMat2D_CH2;
	Hobject MultiChannelImageTest;
	double m_meanGray_m;
	double m_addNumber_m;
	double m_meanGray_c;
	double m_addNumber_c;
    CString m_channel;
	CString m_newClassName;
	//
	HTuple m_models_CH1,m_model_CH1, m_matrixID_CH1,m_modelNum_CH1;
	Hobject m_multiChannelModelImage_CH1;
	HTuple m_models_CH2,m_model_CH2, m_matrixID_CH2,m_modelNum_CH2;
	Hobject m_multiChannelModelImage_CH2; 

	CAlgoSpy m_algoSpy;

	DWORD m_nSampleSize;
	DWORD m_nImgIndex;
	std::vector<float> m_vImgScore;


public:
	STDMETHOD(SetSelfLearningParam)(DWORD sampleSz, BSTR* bstrParam);
	STDMETHOD(ResetSelfLearningRegion)(BSTR* strRegionExtractPara);
	STDMETHOD(ResetSelfLearningProcedure)(void);
	STDMETHOD(GetSelfLearningRegion)(LONG* pRgn);
	//
	STDMETHOD(SetCurrentImage)(LONG* pImg);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen,FLOAT* pStatsArray, LONG* nActualLen);

	STDMETHOD(CrownCapLocationAlgo)(VARIANT* rgnId, VARIANT* pThredLow,  VARIANT* pThredHigh, VARIANT* pRadiusExt, VARIANT* pRadius);
	STDMETHOD(CrownCapLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CrownCapLocationUseEdgeAlgo)(VARIANT* rgnId, VARIANT* pThredLow,  VARIANT* pThredHigh, VARIANT* pRadiusExt, VARIANT* pRadius);
	STDMETHOD(CrownCapLocationUseEdgeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CrownCapLocationCH2Algo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRadius,  VARIANT* pOffSetX,  VARIANT* pOffSetY);
	STDMETHOD(CrownCapLocationCH2AlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleDetectAlgo)(VARIANT* rgnID,VARIANT* pEnhanceSize, VARIANT* pEnhanceRatio, VARIANT* pBlackThred, VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CircleDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CrownCapCircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWith,VARIANT* pChannel);
	STDMETHOD(CrownCapCircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleOtherDetectAlgo)(VARIANT* rgnID,VARIANT* pRateAngle, VARIANT* pBlackDist,VARIANT* pErosionSize,  VARIANT* pMinArea, VARIANT* pChannel,VARIANT* pErrorGrayDist, VARIANT* pDetectNumber);
	STDMETHOD(CircleOtherDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleRateDetectAlgo)(VARIANT* rgnID, VARIANT* pBlackThred,VARIANT* pSmoothSize,  VARIANT* pCircleRate);
	STDMETHOD(CircleRateDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CrownCapDetectAlgo)(VARIANT* rgnID, VARIANT* pBlackThred,VARIANT* pARMin,VARIANT* pARMax,  VARIANT* pCrownAnglDiff);
	STDMETHOD(CrownCapDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CrownCapMatchingParaAlgo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pChannel, VARIANT* pMatchingResult);
	STDMETHOD(CrownCapMatchingParaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CrownCapPositionDistAlgo)(VARIANT* rgnId, VARIANT* pShowRadius, VARIANT* pPositionDist);
	STDMETHOD(CrownCapPositionDistAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CrownCapCenterImageDetectAlgo)(VARIANT* rgnId,VARIANT* pSmoothSize,VARIANT* pDiffBlack,VARIANT* pDiffWhite, VARIANT* pErosionSize, VARIANT* pMinArea , VARIANT* pChannel,VARIANT* pDetectNumber);
	STDMETHOD(CrownCapCenterImageDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CanLocationAlgo)(VARIANT* rgnId, VARIANT* pThredLow, VARIANT* pRadius, VARIANT* pRoiWith);
	STDMETHOD(CanLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CanCircleDetectAlgo)(VARIANT* rgnID,VARIANT* pRateAngle, VARIANT* pBlackDist,VARIANT* pErosionSize,  VARIANT* pMinArea, VARIANT* pChannel,VARIANT* pErrorGrayDist, VARIANT* pDetectNumber);
	STDMETHOD(CanCircleDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ReloadExternalFile)(BSTR* bstrPath);

	STDMETHOD(SelectModelImage)(LONG* pCrtImg, BYTE productID);
	STDMETHOD(AddModelImage)(LONG* pImg, BSTR* strFolder, BYTE productID);
	STDMETHOD(DeleteModelImage)(BYTE nIndex, BYTE productID);
	STDMETHOD(SaveResult)(BSTR* strModelNameD, BYTE productID);
	STDMETHOD(ResetModels)(BYTE productID);
	STDMETHOD(GetSelfLearningResult)(LONG*  multiModelImag, long* cMax, float* vImgScore,BYTE productID);
	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);


private:
	bool IsDebugImg(Hobject& Image);
	void(redefine_use_line) (Hobject Image,Hobject *RegionOpening, HTuple StepAng, HTuple Radius_inner,HTuple Radius, HTuple Row_1, HTuple Column_1);
	void (CenterDefined)(Hobject Image, float pThredLow,  float pThredHigh, float pRadiusExt,float pRadius, HTuple& CenterXY);
};

OBJECT_ENTRY_AUTO(__uuidof(CrownCapAlgo), CCrownCapAlgo)
