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
#include "CanAlgo_i.h"
#include "../AlgoUtils/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"
using namespace std;
using namespace boost;
using namespace Halcon;

#ifndef   DLLEXPT
#define DLLEXPT extern "C" __declspec (dllexport )
#endif

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "ATL Error"
#endif

using namespace ATL;

//混罐检测新算法
#define  MIX_LABEL_ALGO_NEW

//曲面映射时使用预先生成的区域
#define  MAP_USE_FIX_REGION

struct MatrixData
{
	MatrixData()
	{

	}

	//初始化矩阵
	MatrixData(Hobject MappedImages)	
	{
		Initial(MappedImages);	
	}

	//使用训练结果的矩阵来初始化
	void InitialWithMatrix(HTuple ReferMatrix_)
	{
		copy_matrix(ReferMatrix_, &ReferMatrix);

		MaxTrainedImgNum = 500;

		HTuple MatrixColNum;
		get_size_matrix(ReferMatrix, &MatrixRowNum, &MatrixColNum);

		tuple_gen_sequence(0, MatrixRowNum-1, 1, &RowSequence);

		HTuple FirstRowMatrix;
		HTuple FirstRowMatrixValue;

		get_sub_matrix(ReferMatrix, 0, 0, 1, MatrixColNum, &FirstRowMatrix);
		get_full_matrix(FirstRowMatrix, &FirstRowMatrixValue);
		tuple_find_first(FirstRowMatrixValue, -1, &TrainedImgNum);

		if (TrainedImgNum == -1)
		{
			TrainedImgNum = MaxTrainedImgNum;
		}

		clear_matrix(FirstRowMatrix);
	}

	void  Initial(Hobject MappedImages)
	{
		MaxTrainedImgNum = 500;
		TrainedImgNum = 0;
		MatrixRowNum = 0;

		Halcon::HTuple ImgW, ImgH;
		Halcon::HTuple ObjNum;

		Hobject SelectedObj;
		count_obj(MappedImages, &ObjNum);
		Halcon::HTuple ChannelNum;

		for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
		{
			select_obj(MappedImages, &SelectedObj, ObjIndex);
			get_image_size(SelectedObj, &ImgW, &ImgH);

			count_channels(SelectedObj, &ChannelNum);
			MatrixRowNum = MatrixRowNum + ChannelNum * ImgH;
		}

		create_matrix(MatrixRowNum, MaxTrainedImgNum, -1, &ReferMatrix);

		tuple_gen_sequence(0, MatrixRowNum-1, 1, &RowSequence);
	}

	Halcon:: HTuple GetMatrixRow()
	{
		// 		HTuple MatrixRow, MatrixCol;
		// 		get_size_matrix(ReferMatrix, &MatrixRow, &MatrixCol);
		return MatrixRowNum;
	}

	Halcon::HTuple GetTrainedNum()
	{	
		return TrainedImgNum;
	}

	Halcon::HTuple GetMatrix()
	{
		return ReferMatrix;
	}

	Halcon::HTuple GetCroppedMatrix()
	{
		return ReferMatrixCropped;
	}

	Halcon::HTuple GetRowSequence()
	{
		return RowSequence;
	}

	HTuple  GetMaxTrainedImgNum()
	{
		return MaxTrainedImgNum;
	}



	//设置矩阵最大尺寸，须在初始化时设置，否则需要更新矩阵相关数据
	void   SetMaxTrainedImgNum(Halcon::HTuple MaxImgNumNew)
	{
		MaxTrainedImgNum = MaxImgNumNew;
	}

	//矩阵增加
	bool  AddVector(Halcon::HTuple FeatureVec)
	{
		bool result = true;		

		Halcon::HTuple VecLength;

		tuple_length(FeatureVec, &VecLength);

		if (VecLength != MatrixRowNum)
		{
			TrainedImgNum += 1;
			return false;
		}


		//beyond the matrix boundary
		if (TrainedImgNum >= MaxTrainedImgNum)
		{
			return false;
		}

		Halcon::HTuple ColSequence;
		tuple_gen_const(VecLength, TrainedImgNum, &ColSequence);


		set_value_matrix(ReferMatrix, RowSequence, ColSequence, FeatureVec);

		TrainedImgNum += 1;

		return result;
	}


	//矩阵删除某一列
	bool RemoveVect(Halcon::HTuple RemovedIndex)
	{
		/*		bool result = true;

		if (RemovedIndex < 0 || RemovedIndex >= TrainedImgNum)
		{
		return false;
		}

		Halcon::HTuple InitMatrix;
		create_matrix(MatrixRowNum, MaxTrainedImgNum, -1, &InitMatrix);

		//remove the first column
		if (RemovedIndex == 0)
		{
		Halcon::HTuple SubMatrix1;
		get_sub_matrix(ReferMatrix, 0, 1, MatrixRowNum, TrainedImgNum-1, &SubMatrix1);
		set_sub_matrix(InitMatrix, SubMatrix1, 0, 0);
		copy_matrix(InitMatrix, &ReferMatrix);

		clear_matrix(InitMatrix);
		clear_matrix(SubMatrix1);

		TrainedImgNum = TrainedImgNum - 1;
		}		
		else if (RemovedIndex == TrainedImgNum -1)
		{
		//remove the last  column
		Halcon::HTuple ColSequence, ConstValue;
		tuple_gen_const(MatrixRowNum, RemovedIndex, &ColSequence);
		tuple_gen_const(MatrixRowNum, -1, &ConstValue);

		set_value_matrix(ReferMatrix, RowSequence, ColSequence, ConstValue);
		TrainedImgNum = TrainedImgNum - 1; 
		}
		else
		{
		//remove the normal column
		Halcon::HTuple SubMatrix1, SubMatrix2;		

		get_sub_matrix(ReferMatrix, 0, 0, MatrixRowNum, RemovedIndex, &SubMatrix1);
		set_sub_matrix(InitMatrix, SubMatrix1, 0, 0);	

		get_sub_matrix(ReferMatrix, 0, RemovedIndex+1, MatrixRowNum, TrainedImgNum - RemovedIndex -1, &SubMatrix2);
		set_sub_matrix(InitMatrix, SubMatrix2, 0, RemovedIndex);

		copy_matrix(InitMatrix, &ReferMatrix);

		clear_matrix(InitMatrix);
		clear_matrix(SubMatrix1);
		clear_matrix(SubMatrix2);
		TrainedImgNum = TrainedImgNum - 1;
		}
		clear_matrix(InitMatrix);
		return result;
		*/
		bool result1 = SingleMatrixRemove(ReferMatrix, RemovedIndex, TrainedImgNum);
		bool result2 = SingleMatrixRemove(ReferMatrixCropped, RemovedIndex, TrainedImgNum);

		if (result1 & result2)
		{
			TrainedImgNum = TrainedImgNum - 1; 
			return true;
		} 
		else
		{
			return false;
		}
	}


	bool SingleMatrixRemove(HTuple& TargetMatrix, HTuple RemovedIndex, HTuple TrainedImgNum)
	{
		bool result = true;

		if (RemovedIndex < 0 || RemovedIndex >= TrainedImgNum)
		{
			return false;
		}

		HTuple MatrixRowNumTemp, ColNumTemp;
		get_size_matrix(TargetMatrix, &MatrixRowNumTemp, &ColNumTemp);

		Halcon::HTuple InitMatrix;
		create_matrix(MatrixRowNumTemp, MaxTrainedImgNum, -1, &InitMatrix);

		//remove the first column
		if (RemovedIndex == 0)
		{
			Halcon::HTuple SubMatrix1;
			get_sub_matrix(TargetMatrix, 0, 1, MatrixRowNumTemp, TrainedImgNum-1, &SubMatrix1);
			set_sub_matrix(InitMatrix, SubMatrix1, 0, 0);
			copy_matrix(InitMatrix, &TargetMatrix);

			clear_matrix(InitMatrix);
			clear_matrix(SubMatrix1);

		}		
		else if (RemovedIndex == TrainedImgNum -1)
		{
			//remove the last  column
			Halcon::HTuple ColSequence, ConstValue, RowSequenceTemp;
			tuple_gen_const(MatrixRowNumTemp, RemovedIndex, &ColSequence);
			tuple_gen_const(MatrixRowNumTemp, -1, &ConstValue);
			tuple_gen_sequence(0, MatrixRowNumTemp-1, 1, &RowSequenceTemp);

			set_value_matrix(TargetMatrix, RowSequenceTemp, ColSequence, ConstValue);			
		}
		else
		{
			//remove the normal column
			Halcon::HTuple SubMatrix1, SubMatrix2;		

			get_sub_matrix(TargetMatrix, 0, 0, MatrixRowNumTemp, RemovedIndex, &SubMatrix1);
			set_sub_matrix(InitMatrix, SubMatrix1, 0, 0);	

			get_sub_matrix(TargetMatrix, 0, RemovedIndex+1, MatrixRowNumTemp, TrainedImgNum - RemovedIndex -1, &SubMatrix2);
			set_sub_matrix(InitMatrix, SubMatrix2, 0, RemovedIndex);

			copy_matrix(InitMatrix, &TargetMatrix);

			clear_matrix(InitMatrix);
			clear_matrix(SubMatrix1);
			clear_matrix(SubMatrix2);			
		}
		clear_matrix(InitMatrix);
		return result;		
	}


	//矩阵更新某一列（实时更新数据）
	void  UpdateMatrix(Halcon::HTuple ColIndex, Halcon::HTuple FeatureVec)
	{
		Halcon::HTuple ColSequence;		


		if (ColIndex >= 0 && ColIndex < TrainedImgNum)
		{
			tuple_gen_const(MatrixRowNum, ColIndex, &ColSequence);

			set_value_matrix(ReferMatrix, RowSequence, ColSequence, FeatureVec);
		}	
	}


	//矩阵的裁剪，以适应其对应的实际区域高度
	void  CropMatrixBaseReg(Halcon::HTuple StartRow, Halcon::HTuple EndRow)
	{
		//更新相关数据
		MatrixRowNum = EndRow - StartRow + 1;
		tuple_gen_sequence(0, MatrixRowNum-1, 1, &RowSequence);


		get_sub_matrix(ReferMatrix, StartRow, 0, MatrixRowNum, MaxTrainedImgNum, &ReferMatrix);
	}

	void  CropMatrixBasePos(Hobject ReferImg, HTuple StartRowPercents, HTuple EndRowPercents, HTuple* MatrixNew, bool SelfWrited = false)
	{
		*MatrixNew = HTuple();
		HTuple MatrixNewTemp;
		create_matrix(MatrixRowNum, MaxTrainedImgNum, -1, &MatrixNewTemp);

		HTuple ObjNum;
		HTuple ImgWidths, ImgHeights;

		get_image_size(ReferImg, &ImgWidths, &ImgHeights);
		count_obj(ReferImg, &ObjNum);

		HTuple StartRows, EndRows;
		tuple_int(1.0*ImgHeights*StartRowPercents /100, &StartRows);
		tuple_int(1.0*ImgHeights*EndRowPercents /100, &EndRows);


		Hobject SelectedImg;
		HTuple  SelectedImgChannelNum;
		HTuple  SubMatrixTemp;
		HTuple  StartPos, TargetPos;

		TargetPos = 0;	

		for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
		{
			select_obj(ReferImg, &SelectedImg, ObjIndex);
			count_channels(SelectedImg, &SelectedImgChannelNum);

			//起始行
			if (ObjIndex == 1)
			{
				StartPos = 0;
			}
			else
				StartPos = StartPos + ImgHeights[ObjIndex-1] * SelectedImgChannelNum;


			for (int ChannelIndex = 1; ChannelIndex <= SelectedImgChannelNum; ChannelIndex++)
			{
				HTuple StartRowTemp = StartPos + ImgHeights[ObjIndex-1].I() *(ChannelIndex-1) + StartRows[ObjIndex-1].I();

				get_sub_matrix(ReferMatrix, StartRowTemp, 0, EndRows[ObjIndex-1].I() - StartRows[ObjIndex-1].I(), MaxTrainedImgNum, &SubMatrixTemp);
				set_sub_matrix(MatrixNewTemp, SubMatrixTemp, TargetPos, 0);

				TargetPos = TargetPos + EndRows[ObjIndex-1].I() - StartRows[ObjIndex-1].I();
			}
		}


		get_sub_matrix(MatrixNewTemp, 0, 0, TargetPos, MaxTrainedImgNum, MatrixNew);

		if (SelfWrited)
		{
			copy_matrix(*MatrixNew, &ReferMatrix);
			HTuple MatrixHeight;

			get_size_matrix(ReferMatrix, &MatrixRowNum, &MatrixHeight);
		}	

		//更新到ReferMatrixCropped
		copy_matrix(*MatrixNew, &ReferMatrixCropped);
	}

	Halcon::HTuple ReferMatrix;
	Halcon::HTuple MaxTrainedImgNum;
	Halcon::HTuple TrainedImgNum;
	Halcon::HTuple MatrixRowNum;

	Halcon::HTuple ReferMatrixCropped;	//裁剪后的矩阵
	//行号序列
	Halcon::HTuple RowSequence;
};


void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;
}

class ATL_NO_VTABLE CCanAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCanAlgo, &CLSID_CanAlgo>,
	public IDispatchImpl<ICanAlgo, &IID_ICanAlgo, &LIBID_CanAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CCanAlgo():m_vPos(128),m_vRgn(128),m_vRgnReadData(128),m_vErrorRgn(128), m_vRgnExTest(128), m_vRgnEx(128), m_vWhiteErrorRgn(128), m_ringPara(128),m_sectorAngle(128),m_measureHandles(128),m_vInterImgs(128),m_WeldRegRelated(128)
	{
		m_imageNumber     = 0;
		m_image_index     =0;
		m_isFirst         = true;
		m_bDebugImg       = false;
		m_bPilotImg       = false;
		m_angleDetect     =1.57;
		m_oldRadius       = -1;
		m_locationRadius  = 60;
		m_isFirstBot      = true;
		m_oldRadiusBot    = -1;
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
		m_imgWidth                      = 1;
		m_imgHeight                     = 1; 
		m_contrast=30;
		m_minContrast=5;
		m_modelParaOfSelfLearning = HTuple();
		m_modelParaOfSelfLearning.ReplaceElements(0,30);
		m_modelParaOfSelfLearning.ReplaceElements(1,5);

		m_VecDiffThreshold = 3.0;

		m_LeftCameraStartRowPercent = 0;
		m_LeftCameraEndRowPercent = 100;
		m_RightCameraStartRowPercent = 0;
		m_RightCameraEndRowPercent = 100;
		//
		m_CenterRadius=-1;
		m_pixelSize=0.035;				
		m_bInitialSuccess = false;
		gen_empty_region(&m_centerLocationRegion);
		gen_empty_obj(&m_MapRegInOrigImg);

		gen_empty_obj(&m_OriginPartitionROI_1);
		gen_empty_obj(&m_OriginPartitionROI_2);
		gen_empty_obj(&m_weldRegions);

		m_bErrorShowed = false;

		m_bMatrixUpdated = false;


		m_bIsRegParamChanged = false;

		m_ImgIndex = 0;

		m_WeldLocatedAlgoRegIndex = -1;

	}

	DECLARE_REGISTRY_RESOURCEID(IDR_CANALGO1)


	BEGIN_COM_MAP(CCanAlgo)
		COM_INTERFACE_ENTRY(ICanAlgo)
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
#ifdef MIX_LABEL_ALGO_NEW
		if (m_bMatrixUpdated)
		{
			string strMatrixPath = m_strTaskPath + "\\ReferMatrix.mtx";
			write_matrix(m_MatrixDataInstance.GetMatrix(), "binary", HTuple(strMatrixPath.c_str()));
			m_bMatrixUpdated = false;
		}
#endif
	}
private:
	std::vector<PIXEL_POS>             m_vPos;          // max size 128
	std::vector<Hobject>               m_vRgn;          // max size 128
	std::vector<HTuple>                m_vRgnReadData;  // max size 128
	std::vector<Hobject>               m_vErrorRgn;     // max size 128
	std::vector<Hobject>               m_vWhiteErrorRgn;     // max size 128
	std::vector<Hobject>               m_vInterImgs;          // max size 128, 中间图像

	std::vector<Hobject>               m_WeldRegRelated;  // max size 128,焊缝区域相关的区域
	std::vector<Hobject>               m_vRgnEx;          // 区域算子
	std::vector<Hobject>               m_vRgnExTest;          // 检测算子

	std::map<int, std::vector<float>>  m_dictSpecialData; 

	std::vector<RING_PARA>             m_ringPara;
	std::vector<SECTOR_PARA>           m_sectorAngle;
	std::vector<MEASURE_HANDLE>        m_measureHandles;
	Hobject                            m_crtImg;        // current image.
	Hobject                            m_crtImgMult;    // current image,mult.
	HTuple                             m_imageRadius;
	Hobject                            m_centerLocationRegion;
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
	//direction detect para
	wstring                            m_wstrTaskPath,m_strTaskName;
	string                             m_strTaskPath;
	Hobject                            m_mapImg, m_mulImg;
	Hobject                            m_modelRegion;
	Hobject                            m_orgModelRegion;
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

	Hobject							   m_mapImgOrigin;	//未裁剪前的映射
	HTuple							   m_ReferMatrixOrigin;
	HTuple							   m_ReferMatrix;
	MatrixData						   m_MatrixDataInstance;
	Hobject							   m_MapRegInOrigImg;

	HTuple							   m_VecDiffThreshold;			//外部参数	
	HTuple							   m_LeftCameraStartRowPercent;
	HTuple							   m_LeftCameraEndRowPercent;
	HTuple							   m_RightCameraStartRowPercent;
	HTuple							   m_RightCameraEndRowPercent;
	HTuple							   m_StartRowPercents;
	HTuple						       m_EndRowPercents;

	HTuple							   m_StartRowPercents_Last;		//保存最近一次修改后的值
	HTuple							   m_EndRowPercents_Last;		
	bool							   m_bIsRegParamChanged;		//表示是否修改了参数

	Hobject							   m_ReferMappedImg;					//颠倒罐实际检测图片的参考图像
	Hobject							   m_ProjROI;					//颠倒罐特征向量投影时的ROI
	bool							   m_bInitialSuccess;			//初始化是否成功
	Hobject							   m_OriginPartitionROI_1;		//相机1中的曲面映射区域分块
	Hobject							   m_OriginPartitionROI_2;	
	bool							   m_bErrorShowed;				//是否显示错误
	bool							   m_bMatrixUpdated;			//颠倒罐检测阶段是否更新模板数据

	int								   m_oldSearchRegRow, m_oldSearchRegCol, m_oldOuterRadius, m_oldRoiWidth, m_oldScanLineNum, m_oldScanLineDirection;
	long							   m_ImgIndex;	//图片计数索引

	int								   m_WeldLocatedAlgoRegIndex;	//保存焊缝区域定位算子的区域索引

	long long						   m_CHImgWndID;
	bool							   m_WindowFirst;

	////dimension measurement
	HTuple							   m_BaseRowBegin,m_BaseColBegin,m_BaseRowEnd,m_BaseColEnd,m_MidRowBegin,m_MidColBegin,m_MidRowEnd,m_MidColEnd,m_Angle1,m_Angle2;
	Hobject							   m_ImageAffinTrans,m_OriginPartitionUnionROI_1,m_OriginPartitionUnionROI_2;
	double							   m_pixelSize;
	HTuple							   m_CenterRadius;
	Hobject                            m_BaseRegion;

	bool                               m_isFirstBot;
	double                             m_oldRadiusBot;
	HTuple                             m_modelIdBot,m_image_index;
	int                                m_oldPalarity;
	Hobject                            m_weldRegions, m_locationEdge,m_calibrateEllipseImage;
	HTuple                             m_ellipseCenterRow, m_ellipseCenterCol, m_ellipsePhi, m_ellipseRadius1, m_ellipseRadius2;
	HTuple                             m_edgeRows,m_edgeCols;

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
	bool (ReadDatasForDirection)( string bstrDataPath,bool errorInfoLanguage,Hobject &MapImg,Hobject& MulImg,Hobject &ModelRegion,Hobject &OrgModelRegion,HTuple& ModelNum,HTuple& AngleStart,HTuple& AngleExtent,HTuple& Models,HTuple& ModelParaOfSelfLearning);

	void(ExtractEdgesPoints)(Hobject Image, HTuple Elements, HTuple DetectHeight, HTuple DetectWidth, HTuple Sigma, HTuple Threshold, HTuple Transition, HTuple Select, 
		HTuple Row1, HTuple Column1,HTuple Row2, HTuple Column2, HTuple *ResultRow, HTuple *ResultColumn);
	void(FitLineUseEdgesPoints)(HTuple Rows,HTuple Cols,HTuple ActiveNum,HTuple *Row1,HTuple *Col1,HTuple *Row2,HTuple *Col2);

public:
	//
	ImageProcess ImageProcessAlg;
	HTuple  m_modelPARA;
	Hobject m_modelImageOCR,m_modelImageTransOCR,m_rgnDetect;
	HTuple  m_modelMatchingPARA;
	int m_detectRegionNumber,m_modelNumber;
	//
	STDMETHOD(SetCurrentImage)(LONG* pImg);
	STDMETHOD(GetInterImgs)(BYTE rgnID,  LONG* pInterImgs);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetRegionEx)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetRegionExTest)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetWhiteErrorRegion)(BYTE rgnID, LONG* pWhiteErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen,FLOAT* pStatsArray, LONG* nActualLen);
	STDMETHOD(ReloadExternalFile)(BSTR* bstrPath);
	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName,BSTR* bstrChName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName,BSTR* bstrChName);
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
	STDMETHOD(CenterLocationAlgoMultImg)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgoMultImgHelp)(BSTR* pHelpStr);
	STDMETHOD(LocationAlgo)(VARIANT* rgnID);
	STDMETHOD(LocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ModelCreate)(double Radius, HTuple * ModelID);
	STDMETHOD(CenterLocationAlgo1)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(SectorRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2);	
	STDMETHOD(SectorRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SectorRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2);	
	STDMETHOD(SectorRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(BoundaryRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);
	STDMETHOD(BoundaryRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BoundaryRegionLocationAlgo1)(VARIANT* rgnId,  VARIANT* pClosingType, VARIANT* pRadius, VARIANT* pRoiWidth);
	STDMETHOD(BoundaryRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(RectangleLeftLocationAlgo)(VARIANT* rgnId,  VARIANT* pThresholdLow, VARIANT* pWidthPlaceSet,VARIANT* pRegionHeight, VARIANT* pRegionWidth,VARIANT* pErosionSize);
	STDMETHOD(RectangleLeftLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AnnularRotaDynThresholdAlgo)(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh,VARIANT* pDefectSize,VARIANT* pDetectArea);
	STDMETHOD(AnnularRotaDynThresholdAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SectorRotaDynThresholdAlgo)(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh,VARIANT* pDefectSize,VARIANT* pDetectArea);
	STDMETHOD(SectorRotaDynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(AnnularRotaDynThresholdAlgoBW)(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea);
	STDMETHOD(AnnularRotaDynThresholdAlgoBWHelp)(BSTR* pHelpStr);
	STDMETHOD(RingRegionDynThresholdAlgoBW)(VARIANT* rgnId, VARIANT* pDetectType, VARIANT* pMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea);
	STDMETHOD(RingRegionDynThresholdAlgoBWHelp)(BSTR* pHelpStr);

	STDMETHOD(DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BlackDetectAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,  VARIANT* pDetectArea);
	STDMETHOD(BlackDetectAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD(VarThreshDarkDetectAlgo)(VARIANT* rgnId, VARIANT* pWindowWidth, VARIANT* pGrayThresh, VARIANT* pAreaFilter, VARIANT* pDetectArea);
	STDMETHOD(VarThreshDarkDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlDetectAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(CurlDetectAlgo1)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea);

	STDMETHOD(ModelCreate2)(double Radius, int UsePolarity,HTuple * ModelID);
	STDMETHOD(CenterReLocationAlgo2)(VARIANT* rgnID, VARIANT* pUsePolarity,VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterReLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(DynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAvoidReflectionAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAvoidReflectionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlDetectAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea);
	STDMETHOD(CurlDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlOptimizeAlgo)(VARIANT* rgnId, VARIANT* pFreq_start, VARIANT* pFreq_cutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CurlOptimizeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WeldAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectAngle);
	STDMETHOD(WeldAngleDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WeldAngleDetectGrayAlgo)(VARIANT* rgnId, VARIANT* pAlgoSelect, VARIANT* pWeldWidth, VARIANT* pSmoothSize,VARIANT* pDetectAngle);
	STDMETHOD(WeldAngleDetectGrayAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WeldAngleDetectWhiteGlueAlgo)(VARIANT* rgnId, VARIANT* pDynThreshold, VARIANT* pAspectRationThresh, VARIANT* pClosingMaskWidth, VARIANT* pDetectAngle);
	STDMETHOD(WeldAngleDetectWhiteGlueAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WeldAngleDetectGlueAlgo)(VARIANT* rgnId,VARIANT* pBlackWhite,VARIANT* pEnhancePara, VARIANT* pDynThreshold, VARIANT* pAspectRationThresh, VARIANT* pClosingMaskWidth, VARIANT* pDetectAngle);
	STDMETHOD(WeldAngleDetectGlueAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GLineAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea);
	STDMETHOD(GLineAlgoHelp)(BSTR* pHelpStr);	
	STDMETHOD(EdgeAlgo)(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlNoCompoundAlgo)(VARIANT* rgnId,VARIANT*pGrayValueMean );
	STDMETHOD(CurlNoCompoundAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (BodyDirectionDetectAlgo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pMatchingResult);
	STDMETHOD (BodyDirectionDetectAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (BodyDirectionDetectMinScoreAlgo)(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels,  VARIANT* pGreediness, VARIANT* pMatchingResult);
	STDMETHOD (BodyDirectionDetectMinScoreAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (BodyDirectionDetectVecDiffAlgo)(VARIANT* rgnId, /*VARIANT* pVecDiffThreshold,*/ VARIANT* pVecDiffValue);
	STDMETHOD (BodyDirectionDetectVecDiffAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (BodyDirectionVecDiffLocationAlgo)(VARIANT* rgnId);
	STDMETHOD (BodyDirectionVecDiffLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DoubleEndRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pBottomRow, VARIANT* pRadiusWidth, VARIANT* pRadiusHeight, VARIANT* pRegionHeight, VARIANT* pRegionWidth);
	STDMETHOD(DoubleEndRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DoubleEndDetectAlgo)(VARIANT* rgnId, VARIANT* pSigmaSize, VARIANT* pBlackThredLow, VARIANT* pBlackThredHigh, VARIANT* pBlackDefectSize, VARIANT* pDetectArea);
	STDMETHOD(DoubleEndDetectAlgoHelp)(BSTR* pHelpStr);	
	STDMETHOD(CenterLocationAlgoHS)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterLocationAlgoHSHelp)(BSTR* pHelpStr);
	//color image
	STDMETHOD(CurlOptimizeForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT* pFreq_start,VARIANT* pFreqCutoff, VARIANT* pDefectType,VARIANT* pWaveThreshold,VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CurlOptimizeForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(EdgeForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgo)( VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD(CurlDetectForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea);
	STDMETHOD(CurlDetectForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlMeanGrayDetectAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pGrayValueLower, VARIANT* pGrayValue, VARIANT* pDetectArea);
	STDMETHOD(CurlMeanGrayDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlMeanGrayDetectForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect,VARIANT* pIsShowInterImg,  VARIANT* pNumPart, VARIANT* pGrayValueLower, VARIANT* pGrayValue, VARIANT* pDetectArea);
	STDMETHOD(CurlMeanGrayDetectForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CurlNoCompoundForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT*pGrayValueMean );
	STDMETHOD(CurlNoCompoundForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterRelocationAlgoHS)(VARIANT* rgnId,VARIANT* pRadiusLong,  VARIANT* pRadiusShort, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterRelocationAlgoHSHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterRelocationAlgo)(VARIANT* rgnID, VARIANT* pRadiusSet, VARIANT* pRoiWidth, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterRelocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CenterLocationAlgoUsePointFitting)(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold);
	STDMETHOD(CenterLocationAlgoUsePointFittingHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationAlgoUsePointFitting1)(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold,VARIANT* pDistThresh);
	STDMETHOD(CenterLocationAlgoUsePointFitting1Help)(BSTR* pHelpStr);

	//dimension measurement
	STDMETHOD(CenterLocationAlgoM)(VARIANT* rgnId,VARIANT* pThreshValue);
	STDMETHOD(CenterLocationAlgoMHelp)(BSTR* pHelpStr);
	STDMETHOD(RectangleLocationAlgo)(VARIANT* rgnId, VARIANT* pWidthPlaceSet,VARIANT* pRegionHeight, VARIANT* pRegionWidth);
	STDMETHOD(RectangleLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BaseLineDetectAlgo)(VARIANT* rgnId, VARIANT* pThredLow, VARIANT* pElements,VARIANT* pTransition,VARIANT* pSelect,VARIANT* pSigma,VARIANT* pThreshold,VARIANT* pDefectArea);
	STDMETHOD(BaseLineDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RectangleLocation2Algo)(VARIANT* rgnId, VARIANT* pWidthPlaceSet,VARIANT*pHeightPlaceSet,VARIANT* pRegionHeight, VARIANT* pRegionWidth);
	STDMETHOD(RectangleLocation2AlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(HeightMeasureAlgo)(VARIANT* rgnId,VARIANT* pThredValue,VARIANT* pOffset,VARIANT* pDefectDiff);
	STDMETHOD(HeightMeasureAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DiameterMeasureAlgo)(VARIANT* rgnId,VARIANT* pThredValue, VARIANT* pElements,VARIANT* pTransition,VARIANT* pSelect,VARIANT* pSigma,VARIANT* pThreshold,VARIANT* pOffset,VARIANT* pDefectDiff);
	STDMETHOD(DiameterMeasureAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WeldWhiteGlueRegExtractAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pWeldRegArea);
	STDMETHOD(WeldWhiteGlueRegExtractAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(WeldRegSelectAlgo)(VARIANT* rgnId, VARIANT* pWeldSelection);
	STDMETHOD(WeldRegSelectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(InnerDiameterMeasureAlgo)(VARIANT* rgnId,VARIANT* pSwitch,VARIANT* pOffset,VARIANT* pDefectDiff);
	STDMETHOD(InnerDiameterMeasureAlgoHelp)(BSTR* pHelpStr);


	STDMETHOD(CenterLocationAlgoUsePointFittingColor)(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pSigma, VARIANT* pThreshold, VARIANT* pColorChannel, VARIANT* pTransType);
	STDMETHOD(CenterLocationAlgoUsePointFittingColorHelp)(BSTR* pHelpStr);

	STDMETHOD(SideCenterLocationAlgo)(VARIANT* rgnID ,VARIANT*pRowStart,VARIANT*pColStart,VARIANT*pRegWidth,VARIANT*pRegHeight,VARIANT* pThredValueUp);	
	STDMETHOD(SideCenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SideRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pErosioSizeH,VARIANT* pErosioSizeV);
	STDMETHOD(SideRegionLocationAlgoHelp)(BSTR* pHelpStr);

	//焊缝区域选择
	STDMETHOD(WeldRegionSelection)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth , VARIANT* pWeldSelection, VARIANT* pRegionErosionSize);
	STDMETHOD(WeldRegionSelectionHelp)(BSTR* pHelpStr);

	//提取焊缝区域
	STDMETHOD(WeldRegionExtractionAlgo)(VARIANT* rgnId, VARIANT* pGrayThresh, VARIANT* pDetectArea);
	STDMETHOD(WeldRegionExtractionAlgoHelp)(BSTR* pHelpStr);


	STDMETHOD (D_EllipseLocationAlgo)(VARIANT* rgnId,VARIANT* pRadiusWidth, VARIANT* pRadiusHeight, VARIANT* pEdgeThresh,VARIANT* pRegionCenterRow,VARIANT* pRegionWidth,VARIANT* pRegionHeight);
	STDMETHOD (D_EllipseLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (D_CalibRegionExtract)(VARIANT* rgnId);
	STDMETHOD (D_CalibRegionExtractHelp)(BSTR* pHelpStr);

	STDMETHOD (D_EdgeCurlMearsureAlgo)(VARIANT* rgnId,VARIANT* pPartWidth,VARIANT* pGradientThresh,VARIANT* pDistanceThresh, VARIANT* pDefectsArea);
	STDMETHOD (D_EdgeCurlMearsureAlgoHelp)(BSTR* pHelpStr);



};

OBJECT_ENTRY_AUTO(__uuidof(CanAlgo), CCanAlgo)
