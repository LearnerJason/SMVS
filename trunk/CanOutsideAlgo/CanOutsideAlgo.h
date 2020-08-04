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
#include "CanOutsideAlgo_i.h"
#include "../AlgoUtils/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"

using namespace std;
using namespace boost;
using namespace Halcon;

struct COMMON_PARAM
{
	Hobject StandardImg;
	Hobject FlagModelRegion;
	Hobject SeamRegionInStdImg;
	Hobject StdModelRegion;
	HTuple  StitchedImgWidthUnion;
	HTuple  ModelIDs;
	HTuple  ModelRows;
	HTuple  ModelCols;
	HTuple  FlagModelID;
	HTuple  SortedFlagColsInStdImg;
	HTuple  StitchingParamCol;
	HTuple  CanRegWidth;
	HTuple  MappedImgWidths;

	COMMON_PARAM()
	{
		gen_empty_obj(&StandardImg);
		gen_empty_obj(&FlagModelRegion);
		gen_empty_obj(&SeamRegionInStdImg);
		gen_empty_obj(&StdModelRegion);

		StitchedImgWidthUnion   = HTuple();
		ModelIDs                = HTuple();
		ModelRows               = HTuple();
		ModelCols               = HTuple();
		FlagModelID             = HTuple();
		SortedFlagColsInStdImg  = HTuple();
		StitchingParamCol       = HTuple();
		CanRegWidth             = HTuple();
		MappedImgWidths         = HTuple();
	}
};

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE Error!"
#endif

using namespace ATL;
void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;
}

class ATL_NO_VTABLE CCanOutsideAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCanOutsideAlgo, &CLSID_CanOutsideAlgo>,
	public IDispatchImpl<ICanOutsideAlgo, &IID_ICanOutsideAlgo, &LIBID_CanOutsideAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCanOutsideAlgo():m_vPos(128),m_vRgn(128),m_vRgnReadData(128),m_vErrorRgn(128),m_ringPara(128),m_sectorAngle(128)
	{
		m_imageNumber     = 0;
		m_isFirst         = true;
		m_bDebugImg       = false;
		m_bPilotImg       = false;
		m_angleDetect     =1.57;
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

		m_nChIndex = -1;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CANOUTSIDEALGO1)


BEGIN_COM_MAP(CCanOutsideAlgo)
	COM_INTERFACE_ENTRY(ICanOutsideAlgo)
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
	}

private:
	ImageProcess                       ImageProcessAlg;
	std::vector<PIXEL_POS>             m_vPos;          // max size 128
	std::vector<Hobject>               m_vRgn;          // max size 128
	std::vector<HTuple>                m_vRgnReadData;  // max size 128
	std::vector<Hobject>               m_vErrorRgn;     // max size 128
	std::map<int, std::vector<float>>  m_dictSpecialData; 
	std::vector<RING_PARA>             m_ringPara;
	std::vector<SECTOR_PARA>           m_sectorAngle;
	Hobject                            m_crtImg;        // current image.
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
	wstring                            m_wstrTaskPath;
	wstring                            m_strTaskName;
	string                             m_strTaskPath;

	long                               m_nChIndex;
	HTuple                             m_nCameraIndex;
	HTuple                             m_HomMat2DForFlag;
	HTuple                             m_LeftCol4ImgsInStd;
	HTuple                             m_RightCol4ImgsInStd;
	COMMON_PARAM                       m_commonParam;
	Hobject                            m_DetecRoi;
	Hobject                            m_MappedImg;
	Hobject                            m_MappedGrayImg;
public:
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
	STDMETHOD(SetChannelIndex)(LONG nChIndex);
	STDMETHOD(ReceivePrerequisiteParam)(LONG* pCameraIndex, LONG* pHomMat2DForFlag, LONG* pLeftCol4ImgsInStd, LONG* pRightCol4ImgsInStd);

private:
	bool LocateErrorInFlatRegion(Hobject ImageToDetect, Hobject ImageToDetectGray, Hobject ReferImage, Hobject ReferImageGray, Hobject FlatRegion, Hobject FlatCenterRegion, Hobject FlatSideRegion, Hobject* Errors, HTuple MinEdgeAmp, HTuple MinEdgeArea, HTuple DynThreshold, HTuple MinArea);
	bool LocateErrorDynThresh1(Hobject ImageToDetect, Hobject ReferImage, Hobject CenterROI, Hobject SideROI, Hobject* Errors, HTuple DynThreshold, HTuple MinArea);
	void SelectTupleBetweenMinMax(HTuple InputTuple, HTuple MinValue, HTuple MaxValue, HTuple* SelectedIndices);
	bool ReadFilesForDetection(Hobject* MultImg, Hobject* MapImg, Hobject* StandardImg, Hobject* FlagModelRegion, Hobject*SeamRegionInStdImg, Hobject* CanRegion, HTuple* StitchedImgWidthUnion, HTuple* ModelIDs, HTuple* ModelRows, HTuple* ModelCols,  HTuple* FlagModelID, HTuple* SortedFlagColsInStdImg, HTuple* StitchingParamCol, int ModelNum, HTuple* CanRegWidth, BOOL * IsMapExist, HTuple* MappedImgWidths);
public:
	STDMETHOD(SetCommonParameters)(LONG* pStandardImg, LONG* pFlagModelRegion, LONG* pSeamRegionInStdImg, LONG* pStdModelRegion, LONG* pStitchedImgWidthUnion, LONG* pModelIDs, LONG* pModelRows, LONG* pModelCols, LONG* pFlagModelID, LONG* pSortedFlagColsInStdImg, LONG* pStichingParamCol, LONG* pCanRegWidth, LONG* pMappedImgWidths);
	STDMETHOD(SetDetectROI)(LONG* pDetectRoi);
	STDMETHOD(SetPreprocessedImg)(LONG* pMappedImg, LONG* pMappedGrayImg);
	STDMETHOD(CenterLocationAlgo)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RectangleLocationAlgo)(VARIANT* rgnId, VARIANT* centerRow, VARIANT* centerCol, VARIANT* nHalfWid, VARIANT* nHalfHeight);
	STDMETHOD(RectangleLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectError)(VARIANT* rgnId, VARIANT* pFlatRgnThresh, VARIANT* pFlatRgnMinArea, VARIANT* pTextRgnThresh, VARIANT* pTextRgnMinArea,VARIANT* pDetectArea);
	STDMETHOD(DetectErrorHelp)(BSTR* pHelpStr);
};

OBJECT_ENTRY_AUTO(__uuidof(CanOutsideAlgo), CCanOutsideAlgo)
