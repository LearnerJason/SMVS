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
#include "NailAlgo_i.h"
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

class ATL_NO_VTABLE CNailAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CNailAlgo, &CLSID_NailAlgo>,
	public ISupportErrorInfo,
	public IDispatchImpl<INailAlgo, &IID_INailAlgo, &LIBID_NailAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CNailAlgo():m_vPos(128),m_vPicPos(128),m_vRgn(128),m_vRgnReadData(128),m_vErrorRgn(128)
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
	// NailAlgo

	}
DECLARE_REGISTRY_RESOURCEID(IDR_NAILALGO1)

BEGIN_COM_MAP(CNailAlgo)
	COM_INTERFACE_ENTRY(INailAlgo)
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

	// NailAlgo
	Hobject m_DetectReg;
	Hobject m_detectRoi;
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
	
	//NailAlgo
	STDMETHOD(CenterRegionExtractAlgo)(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pThredLow,VARIANT* pThredUpper,VARIANT* pThredValue);
	STDMETHOD(CenterRegionExtractAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pNum,VARIANT* pOpeningSize,VARIANT* pClosingSize);
    STDMETHOD(DetectRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CurveDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectArea);
	STDMETHOD(CurveDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ThreadDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectArea);
	STDMETHOD(ThreadDetectAlgoHelp)(BSTR* pHelpStr);

};

OBJECT_ENTRY_AUTO(__uuidof(NailAlgo), CNailAlgo)
