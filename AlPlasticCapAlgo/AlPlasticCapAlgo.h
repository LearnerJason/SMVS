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
#include "AlPlasticCapAlgo_i.h"
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

class ATL_NO_VTABLE CAlPlasticCapAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAlPlasticCapAlgo, &CLSID_AlPlasticCapAlgo>,
	public ISupportErrorInfo,
	public IDispatchImpl<IAlPlasticCapAlgo, &IID_IAlPlasticCapAlgo, &LIBID_AlPlasticCapAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CAlPlasticCapAlgo():m_vPos(128),m_vPicPos(128),m_vRgn(128),m_vRgnExTest(128),m_vRgnEx(128),m_vRgnReadData(128),m_vWhiteErrorRgn(128),m_measureHandles(128),m_sectorAngle(128),m_vErrorRgn(128),m_ringPara(128),m_WeldRegRelated(128),m_vInterImgs(128)
	{
		m_imageNumber     = 0;
		m_isFirst         = true;
		m_bDebugImg       = false;
		m_bPilotImg       = false;
		m_oldRadius       = -1;
		m_angleDetect     =1.57;
		m_meanGray        = 0;
		m_nCrtInjectorID  =-1;
		m_imageRadius     = 100;
		m_locationRadius  = 60;
		m_modelNumber     = 0;
		m_modelCenterRow  = 0.0;
		m_modelCenterCol  = 0.0;
		m_modelCenterRowTab  = 0.0;
		m_modelCenterColTab  = 0.0;
		m_ImgIndex = 0;
		m_modelRadius     = 150;
		m_bFileUpdated    = false;
		m_bOcrRegionFlag  = false;
		m_bFileUpdatedCopy = false;
		m_bFileUpdatedModel = false;
		m_imageNumber     = 0;
		m_isFirstReadSelfLearningPara = true;
		m_isFirstMatchingErrorMessage=true;
		m_gaussTransThred             = 30;
		m_gaussExtractThred           = 15;
		m_dynExtractThredSingle       = 20;
		m_dynExtractThredMulti        = 30;
		m_regionRadius                = 140;
		m_regionDilationSize          = 2.0;
		m_smallExtractArea            = 20.0;
		m_meanDistAdd                 = 0.0;
		m_addNumber                   = 0;
		//
		m_WeldLocatedAlgoRegIndex = -1;
		m_CenterRadius=-1;

		//my_disp_obj
		m_WindowFirst                   =true;
		//
		m_modelMatchingPARA[0]=0.0;
		m_modelMatchingPARA[1]=0.0;
		m_modelMatchingPARA[2]=0.0;
		m_modelMatchingPARA[3]=0.0;
		m_modelMatchingPARA[4]=0.0;
		m_modelMatchingPARA[5]=0.0;
		m_modelMatchingPARA[6]=0.0;
		//兼容方盖子的标志位
		m_modelMatchingPARA[7]=0.0;//该值为0 用模板匹配 该值为1 用角度搜索
		m_angleMatchingPARA[0]=0.0;
		m_angleMatchingPARA[1]=0.0;
		m_angleMatchingPARA[2]=0.0;
		m_angleMatchingPARA[3]=0.0;
		m_angleMatchingPARA[4]=0.0;
		m_angleMatchingPARA[5]=0.0;
		m_angleMatchingPARA[6]=0.0;
		m_currentInjectorIndex = 0;
		m_refreshInjectorIndex_old = -1;
		m_dictNewPathRgn.clear();
		m_dictCrtPathRgn.clear();
		gen_empty_region(&m_centerLocationRegion);
		gen_empty_region(&m_RectanglePlaneRegion);
		gen_empty_obj(&m_ImageAffinTrans);
		m_RectangleAngle             =0.0;
		gen_empty_obj(&m_modelImageOCR);
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
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_AlPlasticCapAlgo1)


	BEGIN_COM_MAP(CAlPlasticCapAlgo)
		COM_INTERFACE_ENTRY(IAlPlasticCapAlgo)
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
	std::vector<Hobject>               m_vRgnEx;          // 区域算子
	std::vector<Hobject>               m_vRgnExTest;          // 检测算子
	std::vector<HTuple>                m_vRgnReadData;  // max size 128
	std::vector<Hobject>               m_vErrorRgn; 
	std::vector<Hobject>               m_vWhiteErrorRgn;// max size 128
	std::vector<RING_PARA>             m_ringPara;
	std::vector<SECTOR_PARA>           m_sectorAngle;
    std::vector<MEASURE_HANDLE>        m_measureHandles;
	std::map<int, std::vector<float>>  m_dictSpecialData; 
	std::vector<Hobject>               m_WeldRegRelated;
	std::vector<Hobject>               m_vInterImgs;          // max size 128, 中间图像
	Hobject                            m_crtImg;    // current image.
	Hobject*                           m_ptrImgData;
	Hobject                            m_crtImgMult;    // current image,mult.
	Hobject                            m_centerLocationRegion;
	HTuple                             m_imageRadius;
	CAlgoSpy                           m_algoSpy;
	bool                               m_isFirst;
	bool                               m_bDebugImg;
	bool                               m_bOcrRegionFlag,m_bFileUpdatedModel,m_bFileUpdatedCopy;
	bool                               m_bPilotImg;
	bool                               m_isChineseLanguage;
	HTuple                             m_modelId;
	double                             m_oldRadius;
	double                             m_angleDetect;
	DWORD                              m_nSampleSize;
	DWORD                              m_nImgIndex;
	Hobject                            MultiChannelImageTest;
	HTuple                             m_modelRadius;
	Hobject                            m_modelRegion;
	Hobject                            m_RectanglePlaneRegion;
	Hobject                            m_ImageAffinTrans;
	HTuple                             m_RectangleAngle;	
	long int                           m_imageNumber;
	HTuple                             m_modelCenterRow;
	HTuple                             m_modelCenterCol;
	long                               m_imgWidth;
	long                               m_imgHeight;
	HTuple                             m_innerRow;
	HTuple                             m_innerColumn;
	HTuple                             m_innerRadius;
	HTuple                             m_locationRadius;


	//my_disp_obj
	long          					   m_CHImgWndID;
	bool							   m_WindowFirst;

	//注胶头跟踪
	int                                m_nCrtInjectorID,m_refreshInjectorIndex_old;
	int								   m_oldSearchRegRow, m_oldSearchRegCol, m_oldOuterRadius, m_oldRoiWidth, m_oldScanLineNum, m_oldScanLineDirection;
	bool							   m_bIsRegParamChanged;		//表示是否修改了参数
	double                             m_meanGray;
	//R角检测
	Hobject                            m_regionDetectR;
	HTuple							   m_CenterRadius;
	//铝塑盖侧面
	HTuple							   m_fReciprocal;			//斜率倒数
	HTuple							   m_TopEdgeCenterY;		//上边缘中心Y
	HTuple							   m_StepCenterY;			//中间台阶边缘中心Y
	HTuple							   m_BottomCenterY;			//下边缘中心Y
	Hobject							   m_TopEdge;				//上边沿骨架
	Hobject							   m_BottomEdge;			//下边沿骨架
	//
	HTuple  m_centerTuple, m_modelID, Models, Model_Num, MatrixID;
	Hobject m_multiChannelModelImage;
	HTuple  ModelsNew,Model_Num_New,MatrixIDNew;
	Hobject m_multiChannelModelImageNew;
	HTuple  ModelsSort,Model_Num_Sort,MatrixIDSort,ScoreSort;
	Hobject m_multiChannelModelImageSort;
	HTuple  m_matchingPara_CH1,m_matchingPara_CH2;
	HTuple  m_matchingHomMat2D_CH1,m_matchingHomMat2D_CH2;
	HTuple  m_models_CH1,m_model_CH1, m_matrixID_CH1,m_modelNum_CH1;
	Hobject m_multiChannelModelImage_CH1;
	HTuple  m_models_CH2,m_model_CH2, m_matrixID_CH2,m_modelNum_CH2;
	Hobject m_multiChannelModelImage_CH2; 
	Hobject m_regLearning; 
	Hobject m_regExtractLearning; 
	Hobject m_imageDistNum,m_imageReg;
	BSTR   m_strRegionExtractPara;
	long   m_ImgIndex;	//图片计数索引
	//
	HTuple m_modelsForOcr,m_matrixForOcr;
	Hobject m_modelImagesForOcr; 
	bool m_readModelState;
	bool m_resetModelForMultOcr;
	//
	int	  m_WeldLocatedAlgoRegIndex;	//保存焊缝区域定位算子的区域索引
	int	  m_currentInjectorIndex;	
	//

	Hobject m_regionNew,m_calibrationRegion;
	Hobject m_modelImageNew,m_standEdges;
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
	Hobject m_ShapeModelRegion;
	//图案模板匹配中心到盖面中心的距离（均值）
	double m_meanDistAdd;
	double m_addNumber;
	// For updating external files
	volatile bool  m_bFileUpdated;
	std::map<string, Hobject> m_dictNewPathRgn;     // File Path <===> Region
	std::map<string, Hobject> m_dictCrtPathRgn;     // File Path <===> Region

	std::map<string, HTuple>  m_dictNewPathModelID; // File Path <== Model ID
	std::map<string, HTuple>  m_dictCrtPathModelID; // File Path <== Model ID
	wstring  m_wstrTaskPath,m_strTaskName,m_wstrChannelPath;
	string   m_strTaskPath,m_strChannelPath; //任务路径
	wstring m_wstrTaskPathWithoutCameraIndex, m_wstrChannelPathWithoutCameraIndex;
    string  m_strTaskPathWithoutCameraIndex, m_strChannelPathWithoutCameraIndex;//任务路径，不带相机序号
	Hobject m_StandardLine;
	float m_OldCenterY;
	float m_OldCenterX;
	
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
	void PrintGlueInjectorIndex(int nIndex)
	{
		Hobject ImageOneChannel;
		Hobject imgEx;

		select_obj(*m_ptrImgData,&imgEx,1);
		access_channel (imgEx, &ImageOneChannel, 1);
		set_grayval(ImageOneChannel,8, 0, nIndex);
	}
	bool ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA);
	double(Sinc)(double n);
	void(ZoningRing)(Hobject Image, Hobject Ring, Hobject Middle_Circle, Hobject * ZoningReg, HTuple CenterRow, HTuple  CenterColumn, int RingWidth, HTuple * tuple_RegMean_DCoffset);
	void(Unit_Impulse_Response)(int paraLength, double angFreqStart, double angFreqCutoff, int winType, std::vector<double>& vFilterParam);
	void(BandPass_Filter)(std::vector<double>& FilterParam, int ParamLength, std::vector<double>& vInputData, std::vector<double>& vOutputData);
	void(PhaseAdjust)(std::vector<double>& vOutputData, int FilterWidth);
	void(Tuple_Replace)(HTuple & tuple, HTuple Indices, HTuple ReplaceValue);
	void (LinesToRegion)(Hobject DetectLines, int DefectSize, int DefectSizeUpper, Hobject &RegionUnion);
	void (ErrorDetect)(Hobject ImageReduced, int MaskSize, float SeriousPointDynThresh, int SeriousPointSize, Hobject& DetectUnion, bool defectType);
	bool (ReadModelsForOcr)( BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,bool errorInfoLanguage,HTuple &m_modelsForOcr,HTuple& m_matrixForOcr,Hobject& m_modelImagesForOcr);
	void (GrayValueContrast)(int DetectType, HTuple Diff, int Threshold, HTuple* Indices);
	void (GenerateDefectRegions)(HTuple RowsDefect, HTuple ColumnsDefect, int DefectSize, Hobject* SelectedEdges, HTuple* Area);

	void (my_disp_obj)(const Halcon::Hobject Object);
	void (P_EdgePointsFitCurve)(Hobject TopSlopSearchReg, Hobject Image,HTuple PartInterval, HTuple LineLength, HTuple RemoveSize, HTuple * DefinedRow, HTuple *DefinedCol);
	void (SkeletonSmooth)(Hobject SkeletonReg, HTuple SmoothSize, Hobject* SkeletonSmoothReg);
	void (SkeletonClosedRegion)(Hobject Skeleton1, Hobject Skeleton2, Hobject* ClosedRegion);
public:
	ImageProcess ImageProcessAlg;
	//
	HTuple  m_modelPARA;
	Hobject m_modelImageOCR,m_modelImageTransOCR;
	HTuple  m_modelMatchingPARA,m_angleMatchingPARA;
	int m_detectRegionNumber,m_modelNumber;
	//Tab
	HTuple                             m_modelCenterRowTab;
	HTuple                             m_modelCenterColTab;
	HTuple  m_modelPARATab,m_calibrationMat2d;
	Hobject m_modelImageTabOCR,m_modelImageTransTabOCR;
	//
	STDMETHOD(SetCurrentImage)(LONG* pImg);
	STDMETHOD(GetInterImgs)(BYTE rgnID,  LONG* pInterImgs);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetRegionEx)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetRegionExTest)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetSelfLearningRegion)(LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen,FLOAT* pStatsArray, LONG* nActualLen);
	STDMETHOD(GetWhiteErrorRegion)(BYTE rgnID,  LONG* pWhiteErrorRgn);
	// self-learning interface methods
	STDMETHOD(SetSelfLearningParam)(DWORD sampleSz, BSTR* bstrParam);
	STDMETHOD(SelectModelImage)(LONG* pCrtImg, BYTE productID);
	STDMETHOD(AddModelImage)(LONG* pImg, BSTR* strFolder, BYTE productID);
	STDMETHOD(DeleteModelImage)(BYTE nIndex, BYTE productID);
	STDMETHOD(SaveResult)(BSTR* strModelNameD, BYTE productID);
	STDMETHOD(ResetModels)(BYTE productID);
	STDMETHOD(GetSelfLearningResult)(LONG*  multiModelImag, long* cMax, float* vImgScore,BYTE productID);
	STDMETHOD(ResetSelfLearningRegion)(BSTR* strRegionExtractPara);
	//

	STDMETHOD(ModelCreate)(double Radius, HTuple * ModelID);
	STDMETHOD(CenterLocationAlgo1)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationAlgoUsePointFitting)(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold);
	STDMETHOD(CenterLocationAlgoUsePointFittingHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(CircleRegionAngleAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2);	
	STDMETHOD(CircleRegionAngleAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(BoundaryRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);
	STDMETHOD(BoundaryRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(MatchingParaAlgo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MatchingParaMinScoreAlgo)(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels,VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaMinScoreAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RotatingAngleDetectAlgo)(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pBlackThredLow,VARIANT* pEdgeType,VARIANT* pEdgeDist, VARIANT* pDefectSize,  VARIANT* pDetectAngle);
	STDMETHOD(RotatingAngleDetectAlgoHelp)(BSTR* pHelpStr);
	
	STDMETHOD(DetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile);
	STDMETHOD(DetectRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectRegionLocationAlgoOCR)(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile);
	STDMETHOD(DetectRegionLocationAlgoOCRHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectRegionLocationExtractOCRAlgo)(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pErosionSize, BSTR* bstrOcrRgnFile);
	STDMETHOD(DetectRegionLocationExtractOCRAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectOcrDistOffsetAlgo)(VARIANT* rgnId,VARIANT* pDistanceOffset);
	STDMETHOD(DetectOcrDistOffsetAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SectorRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2);	
	STDMETHOD(SectorRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SectorRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2);	
	STDMETHOD(SectorRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(DynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (BlackDetectAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,  VARIANT* pDetectArea);
	STDMETHOD (BlackDetectAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (MinMaxGrayDetectAlgo)(VARIANT* rgnId,  VARIANT* pBlackThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteThresh,VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD (MinMaxGrayDetectAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgoBlack)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoBlackHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgoBlackNumber)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectNumber);
	STDMETHOD(DynThresholdAlgoBlackNumberHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgoWhite)(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoWhiteHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgoWhiteNumber)(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectNumber);
	STDMETHOD(DynThresholdAlgoWhiteNumberHelp)(BSTR* pHelpStr);
	//
	STDMETHOD(DynThresholdAlgoBlackWhiteNumber)(VARIANT* rgnId,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectNumber);
	STDMETHOD(DynThresholdAlgoBlackWhiteNumberHelp)(BSTR* pHelpStr);
	STDMETHOD(AnnularRotaDynThresholdAlgoBW)(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea);
	STDMETHOD(AnnularRotaDynThresholdAlgoBWHelp)(BSTR* pHelpStr);
	//
	STDMETHOD(PrintErrorDetectOcrAlgo)(VARIANT* rgnId,VARIANT* pLowContrast,VARIANT* pHeightContrast,VARIANT* pDilationSize,VARIANT* pDetectArea);
	STDMETHOD(PrintErrorDetectOcrAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgoOCR)(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize,VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoOCRHelp)(BSTR* pHelpStr);
	STDMETHOD(GSurfaceAlgo)(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea);
	STDMETHOD(GSurfaceAlgoHelp)(BSTR* pHelpStr);	
	STDMETHOD(GLineAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea);
	STDMETHOD(GLineAlgoHelp)(BSTR* pHelpStr);	
	STDMETHOD(GLineNewAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow,VARIANT* pBlackThredHigh,  VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteThredHigh, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea);
	STDMETHOD(GLineNewAlgoHelp)(BSTR* pHelpStr);	
	STDMETHOD(EdgeAlgo)(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GContoursinkAlgo)(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea);
	STDMETHOD(GContoursinkAlgoHelp)(BSTR* pHelpStr);
	//
	STDMETHOD(CurlOptimizeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlOptimizeAlgo)(VARIANT* rgnId, VARIANT* pFreq_start, VARIANT* pFreq_cutoff, VARIANT* pWinType, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber);
	
	STDMETHOD(CurlDetectAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD(CurlDetectAlgo2)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea);
	STDMETHOD(CurlNoCompoundAlgo)(VARIANT* rgnId,VARIANT*pGrayValueMean );
	STDMETHOD(CurlNoCompoundAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ReloadExternalFile)(BSTR* bstrPath);
	STDMETHOD(ResetSelfLearningProcedure)(void);
	void ImgEnhancement(Hobject crtImg, Hobject rgn, Hobject &imgReduced);
	
	//


	STDMETHOD(UVCurlAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue, VARIANT* pDetectArea);
	STDMETHOD(UVCurlAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationAlgoHS)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterLocationAlgoHSHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterRectangleLocationAlgoHS)(VARIANT* rgnId, VARIANT* pThresholdLow, VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterRectangleLocationAlgoHSHelp)(BSTR* pHelpStr);
	
	STDMETHOD(CircleRegionAngleAlgo)(VARIANT* rgnId,VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2);
	STDMETHOD(CircleRegionAngleAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdAlgo)( VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdAlgoHelp)( BSTR* pHelpStr);
	

	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetCurrentTaskName2)(BSTR* bstrTaskName,BSTR* bstrChName,BSTR* bstrChNameWithoutCameraIndex);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);
	STDMETHOD(CenterLocationForColorAlgoHS)(VARIANT* rgnId,VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterLocationForColorAlgoHSHelp)(BSTR* pHelpStr);
	STDMETHOD(RingLocationForColorAlgoHS)(VARIANT* rgnId,VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pRegionWidth);
	STDMETHOD(RingLocationForColorAlgoHSHelp)(BSTR* pHelpStr);

	STDMETHOD(CurlOptimizeForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect,VARIANT* pFreq_start,VARIANT* pFreqCutoff,VARIANT* pWinType, VARIANT* pDefectType,VARIANT* pWaveThreshold,VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CurlOptimizeForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(EdgeForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgo)( VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgoHelp)( BSTR* pHelpStr);
	
	STDMETHOD(DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GetCurrentLocationXY)(BSTR* bstrLocationXY);
	STDMETHOD(GetCenterLocation)(BSTR* bstrLocationXY);

	//铝塑盖
	STDMETHOD(SideCenterLocationAlgoForColor)(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pDividingLine, VARIANT* BackgroundGray, VARIANT* AlThreshold, VARIANT* PlasticThreshold, VARIANT* pSmoothSize);
	STDMETHOD(SideCenterLocationAlgoForColorHelp)(BSTR* pHelpStr);
	STDMETHOD(PlasticRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRowMove, VARIANT* pRegionHeight, VARIANT* pWidthShrink);
	STDMETHOD(PlasticRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ExpansionContourGenerationRegionAlgo)(VARIANT* rgnId, VARIANT* pType, VARIANT* pRowMove, VARIANT* pDilationHeight, VARIANT* pWidthZoom);
	STDMETHOD(ExpansionContourGenerationRegionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(StepEdgeDetectAlgo)(VARIANT* rgnId,VARIANT* pSearchSize, VARIANT* pChannelSelect, VARIANT* pSmoothSize, VARIANT* pEdgeHeight);
	STDMETHOD(StepEdgeDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AlRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pUpRowMove, VARIANT* pDownRowMove, VARIANT* pWidthShrink);
	STDMETHOD(AlRegionLocationAlgoHelp)(BSTR* pHelpStr);
};

OBJECT_ENTRY_AUTO(__uuidof(AlPlasticCapAlgo), CAlPlasticCapAlgo)
