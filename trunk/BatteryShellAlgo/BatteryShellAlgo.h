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
#include "BatteryShellAlgo_i.h"
#include "../AlgoUtils/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"
#include <ppl.h>
#include <concurrent_vector.h>
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

		MaxTrainedImgNum = 200;

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
		MaxTrainedImgNum = 200;
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

struct HobjectPac
{
	Hobject m_imagePart;
	int     m_partIndex;
	Hobject m_resultRegionsWhite;
	Hobject m_resultRegionsBlack;
	Hobject m_illuminateImage;
	Hobject m_convertImage;
	Hobject m_imageDomain;
	HTuple  m_cornerRow;
	HTuple  m_cornerCol;
	Hobject m_meanImage;
	Hobject m_expandedImage; 
	HTuple m_partImageWidth;
	HTuple m_partImageHeight;
	Hobject m_gaussFilters;

	HobjectPac()
	{

	}
	HobjectPac(Hobject imagePart, int partIndex,Hobject resultRegionsBlack,Hobject resultRegionsWhite,Hobject convertImage,Hobject illuminateImage,HTuple CornerRow,HTuple CornerCol,Hobject meanImage,Hobject expandedImage,Hobject imageDomain,HTuple partImageWidth,HTuple partImageHeight,Hobject gaussFilters)
		:m_imagePart(imagePart),m_partIndex(partIndex),m_resultRegionsWhite(resultRegionsWhite),m_resultRegionsBlack(resultRegionsBlack),m_illuminateImage(illuminateImage),m_convertImage(convertImage),m_cornerRow(CornerRow),m_cornerCol(CornerCol),m_meanImage(meanImage),m_expandedImage(expandedImage),m_imageDomain(imageDomain),m_partImageHeight(partImageHeight),m_partImageWidth(partImageWidth),m_gaussFilters(gaussFilters)
	{

	}
	HobjectPac(const HobjectPac& other)
	{
		m_imagePart            = other.m_imagePart;
		m_partIndex            = other.m_partIndex;
		m_resultRegionsWhite   = other.m_resultRegionsWhite;
		m_resultRegionsBlack   = other.m_resultRegionsBlack;
		m_illuminateImage      = other.m_illuminateImage;
		m_convertImage         = other.m_convertImage;
		m_imageDomain          = other.m_imageDomain;
		m_expandedImage        = other.m_expandedImage;
		m_cornerRow            = other.m_cornerRow;
		m_cornerCol            = other.m_cornerCol;
		m_meanImage            = other.m_meanImage;
		m_partImageWidth       = other.m_partImageWidth;
		m_partImageHeight      = other.m_partImageHeight;
		m_gaussFilters         = other.m_gaussFilters;

	}
	HobjectPac(HobjectPac&& other)
	{
		m_imagePart            = other.m_imagePart;
		m_partIndex            = other.m_partIndex;
		m_resultRegionsWhite   = other.m_resultRegionsWhite;
		m_resultRegionsBlack   = other.m_resultRegionsBlack;
		m_illuminateImage      = other.m_illuminateImage;
		m_convertImage         = other.m_convertImage;
		m_imageDomain          = other.m_imageDomain;
		m_cornerRow            = other.m_cornerRow;
		m_cornerCol            = other.m_cornerCol;
		m_meanImage            = other.m_meanImage;
		m_expandedImage        = other.m_expandedImage;
		m_partImageWidth       = other.m_partImageWidth;
		m_partImageHeight      = other.m_partImageHeight;
		m_gaussFilters         = other.m_gaussFilters;
	}
	HobjectPac& operator=(const HobjectPac& other)
	{
		if(&other == this)
			return *this;
		m_imagePart            = other.m_imagePart;
		m_partIndex            = other.m_partIndex;
		m_resultRegionsWhite   = other.m_resultRegionsWhite;
		m_resultRegionsBlack   = other.m_resultRegionsBlack;
		m_illuminateImage      = other.m_illuminateImage;
		m_convertImage         = other.m_convertImage;
		m_imageDomain          = other.m_imageDomain;
		m_cornerRow            = other.m_cornerRow;
		m_cornerCol            = other.m_cornerCol;
		m_meanImage            = other.m_meanImage;
		m_expandedImage        = other.m_expandedImage;
		m_partImageWidth       = other.m_partImageWidth;
		m_partImageHeight      = other.m_partImageHeight;
		m_gaussFilters         = other.m_gaussFilters;
		return *this;
	}
	HobjectPac& operator=(HobjectPac&& other)
	{
		if(&other == this)
			return *this;
		m_imagePart            = other.m_imagePart;
		m_partIndex            = other.m_partIndex;
		m_resultRegionsWhite   = other.m_resultRegionsWhite;
		m_resultRegionsBlack   = other.m_resultRegionsBlack;
		m_illuminateImage      = other.m_illuminateImage;
		m_convertImage         = other.m_convertImage;
		m_imageDomain          = other.m_imageDomain;
		m_cornerRow            = other.m_cornerRow;
		m_cornerCol            = other.m_cornerCol;
		m_meanImage            = other.m_meanImage;
		m_expandedImage        = other.m_expandedImage;
		m_partImageWidth       = other.m_partImageWidth;
		m_partImageHeight      = other.m_partImageHeight;
		m_gaussFilters         = other.m_gaussFilters;
		return *this;
	}
};

class ATL_NO_VTABLE CBatteryShellAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBatteryShellAlgo, &CLSID_BatteryShellAlgo>,
	public IDispatchImpl<IBatteryShellAlgo, &IID_IBatteryShellAlgo, &LIBID_BatteryShellAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CBatteryShellAlgo():m_vPos(128),m_vPicPos(128),m_vRgn(128),m_vRgnEx(128),m_vRgnExTest(128),m_vRgnReal(128),m_rectLocation(128),
		m_equalFlage(128),m_partImageHeightOrigin(128), m_partImageWidthOrigin(128),m_rectLocationOld(128),
		m_sectorAngle(128),m_measureHandles(128),m_vRgnReadData(128),m_vErrorRgn(128),
		m_ringPara(128),m_vWhiteErrorRgn(128),m_vInterImgs(128)
	{
		m_crtImgIdentify					 = 0;
		m_imageNumber                   = 0;
		m_partNum                       = 64;
		m_isFirst                       = true;
		m_bDebugImg                     = false;
		m_bPilotImg                     = false;
		m_oldRadius                     = -1;
		m_meanGray                      = 0;
		m_nCrtInjectorID                = -1;
		m_imageRadius                   = 100;
		m_locationRadius                = 60;
		m_modelNumber                   = 0;
		m_modelCenterRow                = 0.0;
		m_modelCenterCol                = 0.0;
		m_modelCenterRowTab             = 0.0;
		m_modelCenterColTab             = 0.0;
		m_modelRadius                   = 150;
		m_bFileUpdated                  = false;
		m_bOcrRegionFlag                = false;
		m_imageNumber                   = 0;
		m_isFirstReadSelfLearningPara   = true;
		m_bIsRegParamChanged            = false;
		m_usingParallelFlage            = false;

		m_WindowFirst                   =true;

		m_isFirstMatchingErrorMessage   = true;
		m_gaussTransThred               = 30;
		m_gaussExtractThred             = 15;
		m_dynExtractThredSingle         = 20;
		m_dynExtractThredMulti          = 30;
		m_regionRadius                  = 140;
		m_regionDilationSize            = 2.0;
		m_smallExtractArea              = 20.0;
		m_meanDistAdd                   = 0.0;
		m_addNumber                     = 0;
		m_modelMatchingPARA[0]          = 0.0;
		m_modelMatchingPARA[1]          = 0.0;
		m_modelMatchingPARA[2]          = 0.0;
		m_modelMatchingPARA[3]          = 0.0;
		m_modelMatchingPARA[4]          = 0.0;
		m_modelMatchingPARA[5]          = 0.0;
		m_modelMatchingPARA[6]          = 0.0;
		m_imgWidth                      = 1;
		m_imgHeight                     = 1; 
		m_CenterRadius					=-1;
		m_dictNewPathRgn.clear();
		m_dictCrtPathRgn.clear();

		m_dictNewPathModelID.clear();
		m_dictCrtPathModelID.clear();
		LCID lcid ;
		lcid=GetUserDefaultUILanguage();
		if((lcid==2052)||(lcid==1028))
		{
			m_isChineseLanguage   =  true;
		}
		else
		{
			m_isChineseLanguage     = false;
		}
		m_readModelState            = false;
		m_resetModelForMultOcr      = false;
		m_ImgIndex = 0;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_BatteryShellAlgo1)


	BEGIN_COM_MAP(CBatteryShellAlgo)
		COM_INTERFACE_ENTRY(IBatteryShellAlgo)
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
		m_algoSpy.FushLogCache();
	}

private:
	std::vector<PIXEL_POS>             m_vPos;          // max size 128
	std::vector<PIXEL_POS>             m_vPicPos;  
	std::vector<HTuple>                m_rectLocation;
	std::vector<HTuple>                m_rectLocationOld;// max size 128
	std::vector<HTuple>                m_partImageHeightOrigin;
	std::vector<HTuple>                m_partImageWidthOrigin;
	std::vector<HTuple>                m_equalFlage;// max size 128
	std::vector<Hobject>               m_vRgnReal;          // max size 128
	//补充用来区分并行算子中去黑白区域前后实际处理和显示的区域
	std::vector<Hobject>               m_vRgn;          // max size 128
	//调试参考区域
	std::vector<Hobject>               m_vRgnEx;          // 区域算子
	std::vector<Hobject>               m_vRgnExTest;          // 检测算子
	//
	std::vector<HTuple>                m_vRgnReadData;  // max size 128
	std::vector<Hobject>               m_vInterImgs; 
	std::vector<Hobject>               m_vErrorRgn;     // max size 128
	std::vector<RING_PARA>             m_ringPara;
	std::vector<MEASURE_HANDLE>        m_measureHandles;
	std::map<int, std::vector<float>>  m_dictSpecialData; 
	std::vector<Hobject>               m_vWhiteErrorRgn;// max size 128
	std::vector<SECTOR_PARA>           m_sectorAngle;
	std::map<int, std::vector<HobjectPac>>  m_vROI; 
	//std::vector<HobjectPac>            m_vROI;
	Hobject                            m_crtImg;    // current image.
	Hobject                            m_crtImgMult;    // current image,mult.
	BYTE							   m_crtImgIdentify;   //图片标识 0为原始图，
	HTuple                             m_imageRadius;
	CAlgoSpy                           m_algoSpy;
	bool                               m_isFirst;
	bool                               m_bDebugImg;
	bool                               m_bOcrRegionFlag;
	bool                               m_bPilotImg;
	bool                               m_isChineseLanguage;
	HTuple                             m_modelId;
	/*******************/
	double                             m_oldRadius;
	double                             m_oldDetectType;
	/******************/
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
	HTuple                             m_innerRow;
	HTuple                             m_innerColumn;
	HTuple                             m_innerRadius;
	HTuple                             m_locationRadius;
	double                             m_angleDetect;
	//注胶头跟踪
	long                               m_nCrtInjectorID;
	double                             m_meanGray;
	//R角检测
	Hobject                            m_regionDetectR;
	HTuple							   m_CenterRadius;
	//显示中间图片
	long long						   m_CHImgWndID;
	bool							   m_WindowFirst;
	//去黑白噪点新加变量
	Hobject	m_BlackNoiseRegion, m_WhiteNoiseRegion,m_blackNoiseRegions,m_whiteNoiseRegions;
	//检测波纹干扰区域新加变量
	Hobject							   m_vRgnInterference;
	//电池壳底部OCR区域的相关参数
	Hobject							   m_OCRRgnAngle;
	Hobject						       m_OCRSectorRgn;
	//
	HTuple					m_centerTuple;
	HTuple					m_modelID;
	HTuple					Models;
	HTuple					Model_Num;
	HTuple					MatrixID;
	HTuple					ModelsNew;
	HTuple					Model_Num_New;
	HTuple					MatrixIDNew;
	HTuple					ModelsSort;
	HTuple					Model_Num_Sort;
	HTuple					MatrixIDSort;
	HTuple					ScoreSort;
	HTuple					m_matchingPara_CH1;
	HTuple					m_matchingPara_CH2;
	HTuple					m_matchingHomMat2D_CH1;
	HTuple					m_matchingHomMat2D_CH2;
	HTuple					m_models_CH1;
	HTuple					m_model_CH1;
	HTuple					m_matrixID_CH1;
	HTuple					m_modelNum_CH1;
	HTuple					m_models_CH2;
	HTuple					m_model_CH2;
	HTuple					m_matrixID_CH2;
	HTuple					m_modelNum_CH2;

	Hobject					m_multiChannelModelImage;
	Hobject					m_multiChannelModelImageNew;
	Hobject					m_multiChannelModelImageSort;
	Hobject					m_multiChannelModelImage_CH1;
	Hobject					m_multiChannelModelImage_CH2; 
	Hobject					m_regLearning; 
	Hobject					m_regExtractLearning;
	Hobject					m_gaussFilters; 
	Hobject					m_imageDistNum;
	Hobject					m_imageReg;

	BSTR					m_strRegionExtractPara;
	//
	HTuple					m_modelsForOcr,m_matrixForOcr;
	Hobject					m_modelImagesForOcr; 
	bool					m_readModelState;
	bool					m_resetModelForMultOcr;
	//
	Hobject					m_regionNew;
	Hobject					m_modelImageNew;

	HTuple					m_regionNewData;
	HTuple					m_modelIDNew;
	HTuple					m_fileIndex;

	bool					m_isFirstReadSelfLearningPara;
	bool					m_usingParallelFlage;
	bool					m_isFirstMatchingErrorMessage;
	double					m_gaussTransThred;
	double					m_gaussExtractThred;
	double					m_dynExtractThredSingle;
	double					m_dynExtractThredMulti;
	double					m_regionRadius;
	double					m_regionDilationSize;
	double					m_smallExtractArea;

	//图案模板匹配中心到盖面中心的距离（均值）
	double					m_meanDistAdd;
	double					m_addNumber;
	long					m_ImgIndex;
	Hobject					m_originImageMean;

	//电池壳
	HTuple					m_locateRow1;
	HTuple					m_locateCol1;
	HTuple					m_locateRow2;
	HTuple					m_locateCol2;

	int						m_partNum;
	int						m_rowBlock;
	int						m_expandedImageSize;

	int						m_oldSearchRegRow; 
	int						m_oldSearchRegCol; 
	int						m_oldOuterRadius; 
	int						m_oldRoiWidth; 
	int						m_oldScanLineNum; 
	int						m_oldScanLineDirection;
	int						m_oldCreateHandleWidth;
	int						m_oldCreateHandleHeight;

	bool					m_bIsRegParamChanged;	
	// For updating external files
	volatile bool  m_bFileUpdated;
	std::map<string, Hobject> m_dictNewPathRgn;     // File Path <===> Region
	std::map<string, Hobject> m_dictCrtPathRgn;     // File Path <===> Region

	std::map<string, HTuple>  m_dictNewPathModelID; // File Path <== Model ID
	std::map<string, HTuple>  m_dictCrtPathModelID; // File Path <== Model ID
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
	bool ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA);
	double(Sinc)(double n);
	void(ZoningRing)(Hobject Image, Hobject Ring, Hobject Middle_Circle, Hobject * ZoningReg, HTuple CenterRow, HTuple  CenterColumn, int RingWidth, HTuple * tuple_RegMean_DCoffset);
	void(Unit_Impulse_Response)(int paraLength, double angFreqStart, double angFreqCutoff, int winType, std::vector<double>& vFilterParam);
	void(BandPass_Filter)(std::vector<double>& FilterParam, int ParamLength, std::vector<double>& vInputData, std::vector<double>& vOutputData);
	void(PhaseAdjust)(std::vector<double>& vOutputData, int FilterWidth);
	void(Tuple_Replace)(HTuple & tuple, HTuple Indices, HTuple ReplaceValue);
	void (LinesToRegion)(Hobject DetectLines, int DefectSize, int DefectSizeUpper, Hobject &RegionUnion);
	void (SurfaceDent)(Hobject Image);
	void (ErrorDetect)(Hobject ImageReduced, int MaskSize, float SeriousPointDynThresh, int SeriousPointSize, Hobject& DetectUnion, bool defectType);
	bool (ReadModelsForOcr)( BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,bool errorInfoLanguage,HTuple &m_modelsForOcr,HTuple& m_matrixForOcr,Hobject& m_modelImagesForOcr);
	void my_disp_obj(const Halcon::Hobject Object);
	bool SecondExposureImagerotation(BYTE pIdentifier, float pPhi);
public:
	ImageProcess ImageProcessAlg;
	//
	HTuple						m_modelPARA;
	HTuple						m_modelMatchingPARA;

	Hobject						m_modelImageOCR;
	Hobject						m_modelImageTransOCR;
	Hobject						m_gaussFilter;
	
	int							m_detectRegionNumber;
	int							m_modelNumber;
	//Tab
	HTuple                      m_modelCenterRowTab;
	HTuple                      m_modelCenterColTab;
	HTuple						m_modelPARATab;

	Hobject						m_modelImageTabOCR;
	Hobject						m_modelImageTransTabOCR;
	Hobject						m_centerLocationRegion;
	Hobject						m_centerLocationCircle;
	//
	STDMETHOD(SetCurrentImage)(LONG* pImg,BYTE crtImgIdentify,LONG* pImgCorrect);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetRegionEx)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetRegionExTest)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetSelfLearningRegion)(LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetWhiteErrorRegion)(BYTE rgnID,  LONG* pWhiteErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen,FLOAT* pStatsArray, LONG* nActualLen);
	STDMETHOD(GetGlueInjectorID)(LONG* pInjectorID);
	STDMETHOD(GetInterImgs)(BYTE rgnID,  LONG* pInterImgs);
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
	STDMETHOD(ModelCreate1)(double Radius, HTuple * ModelID);
	STDMETHOD(CenterLocationAlgo1)(VARIANT* rgnID, VARIANT* pDetectType, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(ExcludePointGradientDetectAlgo)(VARIANT* rgnId, VARIANT* pEdgeThresh, VARIANT* pBlackNoiseThred, VARIANT* pWhiteNoiseThred, VARIANT* pWhiteNoiseSize, VARIANT* pDeviationDiff, VARIANT* pBlackAreaThred,  VARIANT* pDetectArea);
	STDMETHOD(ExcludePointGradientDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(OneWayDetectAlgo)(VARIANT* rgnId, VARIANT* pLightDirection, VARIANT* pFilterSize, VARIANT* pAreaShrink, VARIANT* pDetectionMode, VARIANT* pSensitivity, VARIANT* pAreaThred,  VARIANT* pDetectArea);
	STDMETHOD(OneWayDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GrayLevelUniformityCorrectionAlgo)(VARIANT* rgnId, VARIANT* pFilterSize, VARIANT* pDetectArea);
	STDMETHOD(GrayLevelUniformityCorrectionAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pIsShowInterImg, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CircleRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(CenterLocationAlgoUsePointFitting)(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold);
	STDMETHOD(CenterLocationAlgoUsePointFittingHelp)(BSTR* pHelpStr);

	STDMETHOD(IlluminateCenterLocationAlgoHS)(VARIANT* rgnId,VARIANT* pIsShowInterImg,VARIANT* pDetectType, VARIANT* pRadius,VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(IlluminateCenterLocationAlgoHSHelp)(BSTR* pHelpStr);
	STDMETHOD(EdgeDetectWithLengthFilterAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(EdgeDetectWithLengthFilterAlgo)(VARIANT* rgnId, VARIANT* pErosionSize,VARIANT* pGaussFilterSize, VARIANT* pContrasthredLow, VARIANT* pContrastThredHigh, VARIANT* pEdgeGrayTthresh, VARIANT* pErrorLengthSize,VARIANT* pDetectArea);

	//dimension measurement
	STDMETHOD(CircleRegionAngleAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2);	
	STDMETHOD(CircleRegionAngleAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(CircleRegionAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pSmoothSize, VARIANT* pBlackGayThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteGayThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea);
	STDMETHOD(CircleRegionAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(IrregularRingRegionLocationAlgo)(VARIANT* rgnId, VARIANT* CandidateRadial, VARIANT* pDetectType, VARIANT* pRadius, VARIANT* pMinScore, VARIANT* pInsideRatio, VARIANT* pOutsideRatio);
	STDMETHOD(IrregularRingRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(IrregularRingRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pTransition, VARIANT* pLocationGayThred, VARIANT* pInsideRatio, VARIANT* pOutsideRatio);
	STDMETHOD(IrregularRingRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(DiffRegionMaxWidthDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectWidth);
	STDMETHOD(DiffRegionMaxWidthDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CircleIncisionShapeChangeDetectAlgo)(VARIANT* rgnId, VARIANT* pDetectWidth);
	STDMETHOD(CircleIncisionShapeChangeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DynThresholdAlgo)(VARIANT* rgnId,VARIANT* pIsShowInterImg, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DynThresholdAlgoBlack)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoBlackHelp)(BSTR* pHelpStr);

	STDMETHOD(DynThresholdAlgoWhite)(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoWhiteHelp)(BSTR* pHelpStr);
	//
	STDMETHOD(GSurfaceAlgo)(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea);
	STDMETHOD(GSurfaceAlgoHelp)(BSTR* pHelpStr);	
		
	STDMETHOD(GDarkLineWithFilterAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GDarkLineWithFilterAlgo)(VARIANT* rgnId,VARIANT* pIsShowInterImg,VARIANT* pBlackMaskSize,VARIANT* pBlackThredLow,VARIANT* pDarkGrayThresh,VARIANT* pBlackDefectSize, VARIANT* pDetectArea);
	STDMETHOD(GWhiteineWithFilterAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GWhiteineWithFilterAlgo)(VARIANT* rgnId,VARIANT* pIsShowInterImg,VARIANT* pBlackMaskSize,VARIANT* pBlackThredLow,VARIANT* pDarkGrayThresh, VARIANT* pBlackDefectSize, VARIANT* pDetectArea);
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
	
	STDMETHOD(CircleRegionAngleAlgo)(VARIANT* rgnId,VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2);
	STDMETHOD(CircleRegionAngleAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(RotaDynThresholdWithFilterAlgo)(VARIANT* rgnId,VARIANT* pIsShowInterImg, VARIANT* pFilterSize,VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea);
	STDMETHOD(RotaDynThresholdWithFilterAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(EmphasizeDarkPointDetectAlgo)(VARIANT* rgnId, VARIANT* pFilterSize,VARIANT* pWindowWidth, VARIANT* pEmphasizeFactor, VARIANT* pAreaFilter, VARIANT* pDetectArea);
	STDMETHOD(EmphasizeDarkPointDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(GSurfaceUnionAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GSurfaceUnionAlgo)(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pWhiteThred,VARIANT* pLengthFilterSize,  VARIANT* pErroreAreaThred, VARIANT* pDetectArea);


	STDMETHOD(BoundaryRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);
	STDMETHOD(BoundaryRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);

	STDMETHOD(DetectTheOCRSectorRegionAlgo)(VARIANT* rgnId, VARIANT* pThresholdValue,VARIANT* pCentralAngle, VARIANT* pOCRCenterDirection);
	STDMETHOD(DetectTheOCRSectorRegionAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DynDetectTheOCRSectorRegionAlgo)(VARIANT* rgnId, VARIANT* pThresholdValue,VARIANT* pCentralAngle, VARIANT* pOCRCenterDirection);
	STDMETHOD(DynDetectTheOCRSectorRegionAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DisOCRCircleRegionAngleAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2, VARIANT* pDisOCR);
	STDMETHOD(DisOCRCircleRegionAngleAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(GetCurrentLocationXY)(BSTR* bstrLocationXY);
	STDMETHOD(GetCenterLocation)(BSTR* bstrLocationXY);
	//////////////////////////////////////////////////////////////////////////
	STDMETHOD(RectLocationAlgo)(VARIANT* rgnID, VARIANT* pGrayThresh, VARIANT* pRectWidth, VARIANT* pShowInterRegions, VARIANT* pBlackNoiseGray, VARIANT* pWhiteNoiseGray, VARIANT* pWhiteNoiseSize);
	STDMETHOD(RectLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RectRegionExtraction)(VARIANT* rgnId, VARIANT* pLeftErosion, VARIANT* pRightErosion);
	STDMETHOD(RectRegionExtractionHelp)(BSTR* pHelpStr);
	STDMETHOD(PRectRegionExtraction)(VARIANT* rgnId,VARIANT* pRowBlock, VARIANT* pLeftErosion, VARIANT* pRightErosion);
	STDMETHOD(PRectRegionExtractionHelp)(BSTR* pHelpStr);

	STDMETHOD(MoveImageDynThreshAlgo)(VARIANT* rgnId, VARIANT* pMoveMode, VARIANT* pShiftSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea);
	STDMETHOD(MoveImageDynThreshAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DentMeanCurveAlgo)(VARIANT* rgnId,  VARIANT* pFilterSize,VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea);
	STDMETHOD(DentMeanCurveAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PDentMeanCurveAlgo)(VARIANT* rgnId,  VARIANT* pFilterSize,VARIANT* pBlackThred, VARIANT* pBlackAreaThred,VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred,VARIANT* pUnionSize,VARIANT* pTotalSizeFilterLow, VARIANT* pDetectArea);
	STDMETHOD(PDentMeanCurveAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PEdgeAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(PEdgeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PGDarkLineAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow,VARIANT* pBlackThredHigh,  VARIANT* pBlackDefectSize, VARIANT* pDetectArea);
	STDMETHOD(PGDarkLineAlgoHelp)(BSTR* pHelpStr);	
	STDMETHOD(PDynThreshAlgoBlack)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea);
	STDMETHOD(PDynThreshAlgoBlackHelp)(BSTR* pHelpStr);

	STDMETHOD(PDynThreshAlgoWhite)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea);
	STDMETHOD(PDynThreshAlgoWhiteHelp)(BSTR* pHelpStr);
	STDMETHOD(PDyn2TimesAlgoBlack)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea);
	STDMETHOD(PDyn2TimesAlgoBlackHelp)(BSTR* pHelpStr);

	STDMETHOD(ReducedResolutionDynThreshAlgo)(VARIANT* rgnId, VARIANT* pBlockSize, VARIANT* pDefectType, VARIANT* pFilterSize, VARIANT* pOffSet, VARIANT* pHeightThred, VARIANT* pWidthThred, VARIANT* pDetectArea);
	STDMETHOD(ReducedResolutionDynThreshAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(TheColumnOffsetAlgo)(VARIANT* rgnId, VARIANT* pColumnValue, VARIANT* pOffsetValue);
	STDMETHOD(TheColumnOffsetAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(FindingInterferenceAreaAlgo)(VARIANT* rgnId, VARIANT* pDynThreshThred, VARIANT* pHeightLimit, VARIANT* pAreaLimit, VARIANT* pInterferenceArea);
	STDMETHOD(FindingInterferenceAreaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WaterSpotsDetectAlgo)(VARIANT* rgnId, VARIANT* pGrayThred, VARIANT* pBlackThred, VARIANT* pAreaThred, VARIANT* pDetectArea);
	STDMETHOD(WaterSpotsDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PFindingMoldScratchAreaAlgo)(VARIANT* rgnId, VARIANT* pThreshThred, VARIANT* pWidthMin, VARIANT* pWidthMax, VARIANT* pHeightMin, VARIANT* pHeightMax, VARIANT* pDilationSize, VARIANT* pInterferenceArea);
	STDMETHOD(PFindingMoldScratchAreaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ShoulderRingDeformationDetectAlgo)(VARIANT* rgnId, VARIANT* pGrayValue, VARIANT* pRingWidth, VARIANT* pDetectWidth);
	STDMETHOD(ShoulderRingDeformationDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(PDentAreaAlgo)(VARIANT* rgnId, VARIANT* pFilterSize,VARIANT* pSensitiveThred,VARIANT* pErrorAreaFilter,VARIANT* pDetectArea);
	STDMETHOD(PDentAreaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PHessianMatrixAlgo)(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pGrayDeviationThresh, VARIANT* pBlackAreaThred, VARIANT* pDetectArea);
	STDMETHOD(PHessianMatrixAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PGradientDetectAlgo)(VARIANT* rgnId, VARIANT* pEdgeThresh, VARIANT* pBlackAreaThred, VARIANT* pDetectArea);
	STDMETHOD(PGradientDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PExDarkPointGradientDetectAlgo)(VARIANT* rgnId,VARIANT* pEdgeThresh, VARIANT* pBlackAreaThred , VARIANT* pBlackErrorFilterThresh, VARIANT* pErrorDeviation, VARIANT* pDetectArea);
	STDMETHOD(PExDarkPointGradientDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(PBrushDetectAlgo)(VARIANT* rgnId, VARIANT* pDynThreshold,VARIANT* pLineAreaFilter, VARIANT* pErrorWidthFilter, VARIANT* pDetectArea);
	STDMETHOD(PBrushDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PEmphasizeDarkPointDetectAlgo)(VARIANT* rgnId, VARIANT* pWindowWidth,VARIANT* pWindowHeight, VARIANT* pEmphasizeFactor, VARIANT* pAreaFilter, VARIANT* pDetectArea);
	STDMETHOD(PEmphasizeDarkPointDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PVarThreshDarkDetectAlgo)(VARIANT* rgnId, VARIANT* pWindowWidth, VARIANT* pGrayThresh, VARIANT* pAreaFilter, VARIANT* pDetectArea);
	STDMETHOD(PVarThreshDarkDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PVarThreshLightDetectAlgo)(VARIANT* rgnId, VARIANT* pWindowWidth, VARIANT* pGrayThresh, VARIANT* pAreaFilter, VARIANT* pDetectArea);
	STDMETHOD(PVarThreshLightDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(PThreshLightDetectAlgo)(VARIANT* rgnId, VARIANT* pGrayThresh, VARIANT* pAreaFilter, VARIANT* pDetectArea);
	STDMETHOD(PThreshLightDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(PThreshDarkDetectAlgo)(VARIANT* rgnId, VARIANT* pGrayThresh, VARIANT* pAreaFilter, VARIANT* pDetectArea);
	STDMETHOD(PThreshDarkDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(PDynThreshDeviationBlack)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousBlackPointSizeUpper, VARIANT* pBlackPointDeviation, VARIANT* pDetectArea);
	STDMETHOD(PDynThreshDeviationBlackHelp)(BSTR* pHelpStr);
	STDMETHOD(PGLineXAlgo)(VARIANT* rgnId, VARIANT* pBlackFilterSize, VARIANT* pBlackThredLow, VARIANT* pHeightSizeFilter, VARIANT* pDetectArea);
	STDMETHOD(PGLineXAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MoveBlockDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MoveBlockDetectAlgo)(VARIANT* rgnId, VARIANT* pIsShowInterImg,VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pBlackGrayValue, VARIANT* pWhiteGrayValue, VARIANT* pDetectArea);
	STDMETHOD(PYPartialDetectAlgo)(VARIANT* rgnId, VARIANT* pFilterSize, VARIANT* pMinResponse, VARIANT* pSegementRatio, VARIANT* pDeviationThred, VARIANT* pBlackNoiseThred, VARIANT* pWhiteNoiseThred, VARIANT* pWhiteNoiseSize, VARIANT* pAreaThred, VARIANT* pDetectArea);
	STDMETHOD(PYPartialDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(PYPartialDetectAlgo1)(VARIANT* rgnId, VARIANT* pRangeValue, VARIANT* pDeviationThred, VARIANT* pBlackNoiseThred, VARIANT* pWhiteNoiseThred, VARIANT* pWhiteNoiseSize, VARIANT* pAreaThred, VARIANT* pDetectArea);
	STDMETHOD(PYPartialDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(MatchingParaAlgo)(VARIANT* rgnId, VARIANT* pGreediness, BSTR* bstrShm1File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize, BSTR* bstrRgnFile);
	STDMETHOD(DetectRegionLocationAlgoHelp)(BSTR* pHelpStr);
};

OBJECT_ENTRY_AUTO(__uuidof(BatteryShellAlgo), CBatteryShellAlgo)
