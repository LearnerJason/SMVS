#pragma once
#include "resource.h"   
#include "PetAlgo_i.h"    
#include <cpp\HalconCpp.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <io.h>
#include <atlstr.h>
#include "../AlgoUtils/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>
using namespace std;
using namespace Halcon;
using namespace boost;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE"
#endif

using namespace ATL;

void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;
}

class ATL_NO_VTABLE CPetAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPetAlgo, &CLSID_PetAlgo>,
	public ISupportErrorInfoImpl<&IID_IPetAlgo>,
	public IDispatchImpl<IPetAlgo, &IID_IPetAlgo, &LIBID_PetAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPetAlgo():m_vPos(128),m_vRgn(128),m_vErrorRgn(128),m_ring(128)
	{
	   m_isFirst         = true;
	   m_oldRadius       = -1;
	     m_fileReadPathRight=true;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_PETALGO1)


	BEGIN_COM_MAP(CPetAlgo)
		COM_INTERFACE_ENTRY(IPetAlgo)
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
	std::vector<PIXEL_POS>             m_vPos;       // max size 128
	std::vector<Hobject>               m_vRgn;       // max size 128
	std::vector<Hobject>               m_vErrorRgn;  // max size 128
	std::map<int, std::vector<float>>  m_dictSpecialData; 
	std::vector<RING_PARA>             m_ring;
	Hobject                            m_crtImg; // current image.
	CAlgoSpy                           m_algoSpy;
	bool                               m_bDebugImg;
	Hobject							   m_regLearning;
	float                              m_levelRef;
	double                             m_capLevelDiff;
	DWORD                              m_nSampleSize;
	DWORD							   m_nImgIndex;
	HTuple                             m_models;
	HTuple                             m_modelNum;
	Hobject                            m_multiChannelModelImage;
	Hobject                            m_labelRgn;
	Hobject                            m_capRgn;
	HTuple                             m_labelRefLevel;
	HTuple                             m_imgWidth;
	HTuple                             m_imgHeight;
	Hobject                            m_capRect;
	double							   m_capRectDiff;
	bool                               m_isFirst;
	double                             m_oldRadius;
	HTuple                             m_modelId;
	Hobject                            m_Rect;
	bool                          m_fileReadPathRight;
	HTuple                        m_modelMatchingPARA;
	float                                  Score;
	HTuple                        m_ringModelID;
private:
	bool IsDebugImg(Hobject& Image);

public:
	STDMETHOD(ModelCreate)(double Radius, HTuple * ModelID);
	STDMETHOD(SetCurrentImage)(LONG* pImg);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen);
	STDMETHOD(GetSelfLearningRegion)(LONG* pRgn);
	STDMETHOD(CapLocationAlgo)(VARIANT* rgnID, VARIANT* pGrayThred, VARIANT* pAreaThred,VARIANT* pAreaThredHigh);
	STDMETHOD(CapLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SetSelfLearningParam)(DWORD sampleSz, BSTR* bstrParam);
	STDMETHOD(SelectModelImage)(LONG* pCrtImg, BYTE productID);
	STDMETHOD(DeleteModelImage)(BYTE nIndex, BYTE productID);
	STDMETHOD(SaveResult)(BSTR* strModelName, BYTE productID);
	STDMETHOD(AddModelImage)(LONG* pImg, BSTR* strFolder, BYTE productID);
	STDMETHOD(GetSelfLearningResult)(LONG* multiModelImag, long* cMax, float* vImgScore, BYTE productID);
	STDMETHOD(ResetSelfLearningRegion)(BSTR* strRegionExtractPara);
	STDMETHOD(ResetSelfLearningProcedure)(void);
	STDMETHOD(LevelLocationAlgo)(VARIANT* rgnId, VARIANT* pLevelRef, VARIANT* pLevelRange, VARIANT* pLevelWidth);
	STDMETHOD(LevelLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(LevelDetectionAlgo)(VARIANT* rgnId,VARIANT* pDynThr, VARIANT* pAreaLimit, VARIANT* pGrayThred, VARIANT* pAreaThred);
	STDMETHOD(LevelDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CapSkewOrHighLocationAlgo)(VARIANT* rgnId, VARIANT* pCapLevelDiff, VARIANT* pCapRectDiff, VARIANT* pCapRectHeight, VARIANT* pCapRectWidth);
	STDMETHOD(CapSkewOrHighLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CapSkewDetectionAlgo)(VARIANT* rgnId, VARIANT* pHeightDiff);
	STDMETHOD(CapSkewDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CapHighDetectionAlgo)(VARIANT* rgnId, VARIANT* pHeightDiff);
	STDMETHOD(CapHighDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BottleNeckLocationAlgo)(VARIANT* rgnId, VARIANT* pLevelNeckDiff, VARIANT* pNeckWidth, VARIANT* pNeckHeight);
	STDMETHOD(BottleNeckLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BottleNeckDetectionAlgo)(VARIANT* rgnId, VARIANT* pGrayThred, VARIANT* pAreaLimit, VARIANT* pDefectArea);
	STDMETHOD(BottleNeckDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SecurityRingDetectionAlgo)(VARIANT* rgnId, VARIANT* pGrayThred, VARIANT* pAreaLimit, VARIANT* pDefectArea);
	STDMETHOD(SecurityRingDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(LabelLocationAlgo)(VARIANT* rgnId, VARIANT* pGrayThred, VARIANT* pSmoothFactor, VARIANT* pAreaThred);
	STDMETHOD(LabelLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(InverseLocationAlgo)(VARIANT* rgnId, VARIANT* pRectHeight, VARIANT* pRectWidth);
	STDMETHOD(InverseLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(InverseDetectionAlgo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pMinScore);
	STDMETHOD(InverseDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(HighOrLowLabelLocationAlgo)(VARIANT* rgnId, VARIANT* pNormalLevel);
	STDMETHOD(HighOrLowLabelLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(HighOrLowLabelDetectionAlgo)(VARIANT* rgnId, VARIANT* pHeightDiff);
	STDMETHOD(SlantLabelLocationAlgo)(VARIANT* rgnId, VARIANT* pRectHeight, VARIANT* pRectWidth, VARIANT* pCenterDiff, VARIANT* pLabelRectDiff);
	STDMETHOD(SlantLabelLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SlantLabelDetectionAlgo)(VARIANT* rgnId, VARIANT* pHeightDiff);
	STDMETHOD(SlantLabelDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(HighOrLowLabelDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(TotalBottlesLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(TotalBottlesLocationAlgo)(VARIANT* rgnId);
	STDMETHOD(BottlesLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BottlesLocationAlgo)(VARIANT* rgnId);
	STDMETHOD(BottlesDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BottlesDetectionAlgo)(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pAreaSelect, VARIANT* pNumber);
	STDMETHOD(PetCenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PetCenterLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(PetCircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PetCircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);
	STDMETHOD(PetCircleRegionDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PetCircleRegionDetectionAlgo)(VARIANT* rgnId, VARIANT* pDynThreshold, VARIANT* pSelectArea, VARIANT* pDetectArea);
	STDMETHOD(CodeLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CodeLocationAlgo)(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pWidth, VARIANT* pHeight, VARIANT* pArea);
	STDMETHOD(CodingLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CodingLocationAlgo)(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pDilationCircle,VARIANT* pSelect, VARIANT* pWidth, VARIANT* pHeight);
	STDMETHOD(CodingDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CodingDetectionAlgo)(VARIANT* rgnId, VARIANT* pNumber);
	STDMETHOD(KunRunShanLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(KunRunShanLocationAlgo)(VARIANT* rgnId);
	STDMETHOD(PetMatchingLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PetMatchingLocationAlgo)(VARIANT* rgnId,VARIANT* pRow1, VARIANT* pColumn1, VARIANT* pRow2, VARIANT* pColumn2);
	STDMETHOD(PetMatchingParaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (PetMatchingParaAlgo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(KunRunShanRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(KunRunShanRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRow1Length, VARIANT* pColumn1Length, VARIANT* pRow2Length, VARIANT* pColumn2Length);
	STDMETHOD(PetMatchingDetectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PetMatchingDetectionAlgo)(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pMask, VARIANT* pSelectMin, VARIANT* pNumber);
	STDMETHOD(ReloadExternalFile)(BSTR* bstrPath);
	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);
};

OBJECT_ENTRY_AUTO(__uuidof(PetAlgo), CPetAlgo)


static std::vector<string> SplitString(string strTarget, string strSeperator)
{
	std::vector<string> vSplitRslt;
	boost::split(vSplitRslt, strTarget, boost::is_any_of(strSeperator),token_compress_on);
	return vSplitRslt;
}