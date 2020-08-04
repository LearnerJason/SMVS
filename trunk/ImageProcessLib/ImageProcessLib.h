/********************************************************************
	Copyright(c) 2009-2015, SLAC Corporation, All rights reserved.

	
*	文件名称:	ImageProcessLib.h
*	功能描述:	图像处理方法库
	
*	当前版本: 	1.0	
*	摘    要:	添加罐外壁印刷图案360度检测基础方法函数
*	作    者:				
*	完成日期:			
*********************************************************************/


#include "stdafx.h"
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
#include <ppl.h>
#include <fstream>
#include <concurrent_vector.h>
using namespace Concurrency;
using namespace std;
using namespace Halcon;
using namespace boost;
#pragma once

#ifndef   DLLEXPT
#define DLLEXPT extern "C" __declspec (dllexport )
#endif
struct HobjectPacForCan
{
	Hobject m_crtSingleImage;
	int     m_objIndex;

	HobjectPacForCan()
	{

	}
	HobjectPacForCan(Hobject crtSingleImage, int objIndex )
		:m_crtSingleImage(crtSingleImage),m_objIndex(objIndex)
	{

	}
	HobjectPacForCan(const HobjectPacForCan& other)
	{
		m_crtSingleImage      = other.m_crtSingleImage;
		m_objIndex            = other.m_objIndex;
	}
	HobjectPacForCan(HobjectPacForCan&& other)
	{
		m_crtSingleImage      = other.m_crtSingleImage;
		m_objIndex            = other.m_objIndex;
	}
	HobjectPacForCan& operator=(const HobjectPacForCan& other)
	{
		if(&other == this)
			return *this;
		m_crtSingleImage      = other.m_crtSingleImage;
		m_objIndex            = other.m_objIndex;
		return *this;
	}
	HobjectPacForCan& operator=(HobjectPacForCan&& other)
	{
		if(&other == this)
			return *this;
		m_crtSingleImage      = other.m_crtSingleImage;
		m_objIndex            = other.m_objIndex;
		return *this;
	}
};
std::vector<HobjectPacForCan>  m_vROI;

struct PallerPac
{
	Hobject m_crtSingleImage;
	int     m_objIndex;
	vector<HTuple> m_featureVectors;
	Hobject m_multiModelImages;

	PallerPac()
	{

	}
	PallerPac(Hobject crtSingleImage, int objIndex ,vector<HTuple> featureVectors,Hobject multiModelImages)
		:m_crtSingleImage(crtSingleImage),m_objIndex(objIndex),m_featureVectors(featureVectors),m_multiModelImages(multiModelImages)
	{

	}
	PallerPac(const PallerPac& other)
	{
		m_crtSingleImage      = other.m_crtSingleImage;
		m_objIndex            = other.m_objIndex;
		m_featureVectors      = other.m_featureVectors;
		m_multiModelImages      =other.m_multiModelImages;
	}
	PallerPac(PallerPac&& other)
	{
		m_crtSingleImage      = other.m_crtSingleImage;
		m_objIndex            = other.m_objIndex;
		m_featureVectors      = other.m_featureVectors;
		m_multiModelImages      =other.m_multiModelImages;
	}
	PallerPac& operator=(const PallerPac& other)
	{
		if(&other == this)
			return *this;
		m_crtSingleImage      = other.m_crtSingleImage;
		m_objIndex            = other.m_objIndex;
		m_featureVectors      = other.m_featureVectors;
		m_multiModelImages      =other.m_multiModelImages;
		return *this;
	}
	PallerPac& operator=(PallerPac&& other)
	{
		if(&other == this)
			return *this;
		m_crtSingleImage      = other.m_crtSingleImage;
		m_objIndex            = other.m_objIndex;
		m_featureVectors      = other.m_featureVectors;
		m_multiModelImages      =other.m_multiModelImages;
		return *this;
	}
};
std::vector<PallerPac>  m_vROIVector;

struct MatrixPacForCan
{
	HTuple  m_standFeatureTuple;
	HTuple  m_crtFeatureTuple;

	MatrixPacForCan()
	{

	}
	MatrixPacForCan(HTuple standFeatureTuple, HTuple crtFeatureTuple )
		:m_standFeatureTuple(standFeatureTuple),m_crtFeatureTuple(crtFeatureTuple)
	{

	}
	MatrixPacForCan(const MatrixPacForCan& other)
	{
		m_standFeatureTuple      = other.m_standFeatureTuple;
		m_crtFeatureTuple        = other.m_crtFeatureTuple;
	}
	MatrixPacForCan(MatrixPacForCan&& other)
	{
		m_standFeatureTuple      = other.m_standFeatureTuple;
		m_crtFeatureTuple        = other.m_crtFeatureTuple;
	}
	MatrixPacForCan& operator=(const MatrixPacForCan& other)
	{
		if(&other == this)
			return *this;
		m_standFeatureTuple      = other.m_standFeatureTuple;
		m_crtFeatureTuple        = other.m_crtFeatureTuple;
		return *this;
	}
	MatrixPacForCan& operator=(MatrixPacForCan&& other)
	{
		if(&other == this)
			return *this;
		m_standFeatureTuple      = other.m_standFeatureTuple;
		m_crtFeatureTuple        = other.m_crtFeatureTuple;
		return *this;
	}
};
std::vector<MatrixPacForCan>  m_vMatrix; 

namespace  CylinderCalibration
{
	//训练过程中，判断图像是否为棋盘格图像
	DLLEXPT BOOL  H_IsCalibImage(Hobject MultiImage, Hobject* CalibPtRegion,  HTuple CalibPtRadiusInPixel, HTuple MinCalibPtNum);

	//增强圆柱体表面的棋盘格的灰度均匀性
	DLLEXPT void  H_EnhanceCalibImage(Hobject Image, Hobject* EnhancedImage, Hobject* MultImage, long GrayClosingMaskRadius, int UpperThreshold4BlackPt, double EnhancePara);
		
	//判断当前图像中是否存在接缝
	DLLEXPT BOOL  H_IsImageWithSeam(Hobject Image, int UpperThresholdForLine, string SeamDirection, long UpperInnerRadius);

	//提取黑色标定点区域
	DLLEXPT void  H_ExtractCalibDots(Hobject Image, Hobject* DotsRegion, int MaskWidth, int DynThreshold, int MaxAreaOfHole, double OpeningMaskRadius, HTuple TargetFeatureMin, HTuple TargetFeatureMax);

	

	//去除黑色标定点区域中的噪声区域
	DLLEXPT void  H_RemoveNoiseReg(Hobject DotsRegion, Hobject* DotsRegionRevomedNoise, int DilationRectHeight, int InnerRadiusMin, int InnerRadiusMax, int OuterRadiusMin, int OuterRadiusMax, double DilationRadiusForExtractDotReg);

	//获得横向棋盘格区域
	DLLEXPT void  H_GetRowConnectedDotsRegion(Hobject DotsRegion, Hobject* RowDotsRegion, int MinDistBetweenDots, double OpeningCircleRadius, double InnerRadiusMin, double InnerRadiusMax, double OuterRadiusMin, double OuterRadiusMax);

	//获得纵向棋盘格区域
	DLLEXPT void  H_GetColConnectedDotsRegion(Hobject DotsRegion, Hobject* ColCentralDotsRegion, int ImageWidth, int ImageHeight, int MinDistBetweenDots);

	//校正图像增强后的棋盘格图像，得到校正映射
	DLLEXPT void  H_CreateRectifyMapUsePoints(Hobject DotsRegion, Hobject RowDotsRegion, Hobject ColCentralDotsRegion, Hobject* RectifyMap, int ImageWidth, int ImageHeight, int GridSpacing, double Sigma, double MaxDist);


	//自动判断，并提取棋盘格点参数（内径、外径）
	DLLEXPT bool  H_IsCalibImage_new(Hobject Image, Hobject* CanRegion, HTuple* InnerRadius, HTuple* OuterRadius);

	void  H_ExtractCalibDots_new(Hobject Image, Hobject CanRegion, Hobject* DotsRegion, HTuple InnerRs, HTuple OuterRs, HTuple* ObjNum);

	void  H_GetRowConnectedDotsRegion_new(Hobject& DotsRegion, Hobject* RowDotsRegion, HTuple InnerRs, HTuple OuterRs, HTuple ObjNum, HTuple* RowNum, HTuple* ErrorMsg);

	void  H_GetColConnectedDotsRegion_new(Hobject DotsRegion, Hobject* ColDotsRegion, HTuple InnerRs, HTuple OuterRs, HTuple ObjNum, HTuple RowNum, HTuple* ColNum, HTuple* ErrorMsg);

	void  H_CreateRectifyMapUsePoints(Hobject Image, Hobject DotsRegion, Hobject RowDotsRegion, Hobject ColDotsRegion, Hobject* Map, Hobject* MapRegInOrigImg, HTuple ObjNum, HTuple RowNum, HTuple ColNum, HTuple* ErrorMsg);


	//应该增加反映射到原始图像中区域的输出项
	DLLEXPT bool H_GetMap(Hobject CalibImg, Hobject CanRegion, Hobject* RectifyMap, Hobject* MapRegInOrigImg, HTuple InnerR, HTuple OuterR, HTuple* ErrorMsg);

	DLLEXPT bool H_MapImgs(Hobject Images, Hobject Maps, Hobject* MappedImgs, HTuple* ErrorMsg);

	DLLEXPT void H_GetFeatureVector(Hobject MappedImgs, Hobject ProjROI, HTuple* FeatureVector);

	//根据固定的区域，计算特征向量
	DLLEXPT void H_GetFeatureVector_FixReg(Hobject OriginImg, Hobject ModellingRegs, Hobject OriginPartitionROI_1, Hobject OriginPartitionROI_2,Hobject OriginPartitionUnionROI_1, Hobject OriginPartitionUnionROI_2, HTuple* FeatureVector);
	
	DLLEXPT void H_GetParallelFeatureVector_FixReg(Hobject OriginImg, Hobject ModellingRegs, Hobject OriginPartitionROI_1, Hobject OriginPartitionROI_2,Hobject OriginPartitionUnionROI_1, Hobject OriginPartitionUnionROI_2, HTuple* FeatureVector);
	

	DLLEXPT void H_Calc_Vector_Diff(HTuple ReferMatrix, HTuple FeatureVector, HTuple TrainedImgNum, HTuple* DiffValue, HTuple* DiffIndex, HTuple Method = "Sqrt_sum");
	DLLEXPT void H_Calc_Vector_Diff_Parallel(HTuple ReferMatrix, HTuple FeatureVector, HTuple TrainedImgNum, HTuple* DiffValue, HTuple* DiffIndex, HTuple Method = "Sqrt_sum");



	
	DLLEXPT void H_CropMap(Hobject MapImg, Hobject* RegInOrigImg ,HTuple StartRowPercent, HTuple EndRowPercent, Hobject* MapImgNew);

	DLLEXPT void H_GetMapOrginReg(Hobject MapImg, Hobject* MapRegInOrigImg);

	DLLEXPT void H_GetMapOrginRegDualChannel(Hobject MapImg, Hobject* MapRegInOrigImgs);

	DLLEXPT void H_GetSingleMapRegInOrigImg(Hobject SingleMap, Hobject* SingleMapRegInOrigImg);

	//多相机区域处理
	DLLEXPT void H_GetMultiMapRegInOrigImg(Hobject Maps, Hobject* OriginPartitionROI_1, Hobject* OriginPartitionROI_2);
}

namespace Stitching
{
	//拼接两个图像
	DLLEXPT void H_Stitch2Img(Hobject LeftImg, Hobject RightImg, Hobject* StitchedImg, HTuple HomMat2D_R2L, HTuple MatchedColumn);

	//计算拼接区域的比例系数
	void  H_ComputeBlendingFactor(HTuple BlendingSeamWidth, HTuple* BlendingFactorLeft, HTuple* BlendingFactorRight);
}
//盖面边缘定位
DLLEXPT BOOL H_LocateCircleUseProfile(Hobject Image, Hobject SearchRegion, Hobject* LocatedCircle, double Radius, int MinScore,  double* Pt_X, double* Pt_Y);
//卷边优化算子
DLLEXPT BOOL H_CurlOptimizeAlgo(Hobject Image,Hobject SearchRegion, Hobject* DetectRegion,float CenterX,float CenterY,float RoiWidth, float Radius , double FreqStart, double FreqCutoff, int DefectType, float WaveThreshold, int MinArea, int* pDetectNumber,std::vector<float>&DictSpecialData);
//剔除线状区域
DLLEXPT void H_SelectRegionExcludeLinear (Hobject RegionForSelect,Hobject *BlackPoints, HTuple InnerRadius, HTuple LengthRadiusRatio);
namespace EoeShell
{
	//利用边缘圆拟合定位
	DLLEXPT BOOL H_LocateCircleUseEdges(Hobject Image, Hobject SearchRegion, Hobject* LocatedCircle, double Radius,int ThresholdLow, int ThresholdHigh, double circleScore,double* Pt_X, double* Pt_Y);
}
namespace ColorImage
{
	//彩色图像处理，提取所需的单通道图像
	DLLEXPT BOOL H_ColorImageTransAll(Hobject ColorImage, Hobject* ImageSelect,   int TransType, int ChannelSelect);
	DLLEXPT BOOL H_ColorImageTrans(Hobject ColorImage, Hobject* ImageSelect, Hobject RegionDetect,   int TransType, int ChannelSelect);
}