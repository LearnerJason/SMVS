#include "stdafx.h"
#include "resource.h"
#include "CanAlgo_i.h"
#include "dllmain.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "CanAlgo.h"

#include "../ImageProcessLib/ImageProcessLib.h"
using namespace ColorImage;

#ifndef   DLLEXPT
#define DLLEXPT __declspec (dllexport )
#endif

using namespace std;
using namespace CylinderCalibration;

#ifndef MIX_LABEL_ALGO_NEW
//混罐检测新算法
#define  MIX_LABEL_ALGO_NEW
#endif

#ifndef  MAP_USE_FIX_REGION
//曲面映射时使用预先生成的区域
#define  MAP_USE_FIX_REGION
#endif

#define  HAMMING (11)
#define  ALL  (0)
#define  DARK  (-1)
#define  LIGHT (1)
#pragma region DO_NOT_CHANGE
STDAPI DllCanUnloadNow(void)
{
	return _AtlModule.DllCanUnloadNow();
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{
	HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
	HRESULT hr = E_FAIL;
	static const wchar_t szUserSwitch[] = L"user";

	if (pszCmdLine != NULL)
	{
		if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
		{
			ATL::AtlSetPerUserRegistration(true);
		}
	}

	if (bInstall)
	{	
		hr = DllRegisterServer();
		if (FAILED(hr))
		{
			DllUnregisterServer();
		}
	}
	else
	{
		hr = DllUnregisterServer();
	}

	return hr;
}

#pragma endregion

//MIX_LABEL_ALGO_NEW新算法中，检测参数合法性
void CheckParamValid(HTuple& InputParam)
{
	if (InputParam < 0.0)
	{
		InputParam = 0.0;
	}

	if (InputParam > 100.0)
	{
		InputParam = 100.0;
	}
}

// CanAlgo.cpp : CCanAlgo  Implementation
STDMETHODIMP CCanAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ICanAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CCanAlgo::SetCurrentImage(LONG* pImg)
{
	m_vROI.clear();
	//颠倒罐检测 添加模板（更新现有模板组）
	if(m_addModelState)
	{
		m_models.Append(m_addModelID);
		m_modelNum=m_modelNum+1;
		m_addModelState=false;
		m_nImgIndex=0;
	}
	gen_empty_obj(&m_weldRegions);
	Hobject* pObj = (Hobject*)pImg;
	m_crtImgMult      = *pObj;// 原始图片，可能是多通道图片（彩色图片）
	m_bDebugImg   = ImageProcessAlg.IsDebugImage(m_crtImgMult);
	m_bPilotImg   = ImageProcessAlg.IsPilotImage(m_crtImgMult);
#ifndef MIX_LABEL_ALGO_NEW
	HTuple  Channels;
	count_channels(m_crtImgMult, &Channels);
	//黑白图片
	if(Channels[0].I()==3)
	{
		rgb1_to_gray(m_crtImgMult, &m_crtImg);
	}
	else if(Channels[0].I()==1)
	{
		m_crtImg  = *pObj;
	}
	else
	{
		mean_n(m_crtImgMult, &m_crtImg);
	}
	if(m_bFileUpdated)
	{
		m_bFileUpdated=false;
	}
#else
	//采用原始图像
	m_crtImg  = *pObj;


#endif
	HTuple Width,Height;
	get_image_size (m_crtImg, &Width, &Height);
	m_imgWidth  = Width[0].L();
	m_imgHeight = Height[0].L(); 
	return S_OK;
}

STDMETHODIMP CCanAlgo::GetInterImgs(BYTE rgnID, LONG* pInterImgs)
{
	Hobject* pObj = (Hobject*)pInterImgs;
	*pObj = m_vInterImgs[rgnID];
	return S_OK;
}
STDMETHODIMP CCanAlgo::GetRegionEx(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnEx[rgnID];
	return S_OK;
}
STDMETHODIMP CCanAlgo::GetRegionExTest(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnExTest[rgnID];
	return S_OK;
}
STDMETHODIMP CCanAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CCanAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	Hobject regionOrg;
	//显示模板区域(校正后图片对应的区域替换为原始图片对应的区域)
	Hobject* pObj = (Hobject*)pRgn;

#ifndef MIX_LABEL_ALGO_NEW
	//调整模板区域位置
	HTuple  ModelImageWidth, ModelImageHeight;
	get_image_size(m_crtImg, &ModelImageWidth, &ModelImageHeight);
	if (m_modelRegionHeight<6)
	{
		//HTuple  ModelRegionRow, ModelRegionCol, ModelRegionRadius;
		//inner_circle (m_modelRegion, &ModelRegionRow, &ModelRegionCol, &ModelRegionRadius);
		//gen_rectangle2(&regionOrg, 0.5 *ModelImageHeight, 0.5*ModelImageWidth, 0, 0.4 *ModelImageWidth,ModelRegionRadius);//ModelRegionRadius
		regionOrg=m_modelRegion;
	}
	else
	{
		//与模板区域一致。
		gen_rectangle2(&regionOrg, 0.5 *ModelImageHeight, 0.5*ModelImageWidth, 0, 0.45 *ModelImageWidth,m_modelRegionHeight/2.0);
		//
	}
	//显示模板轮廓
	Hobject ModelReduced;
	reduce_domain(m_crtImg, regionOrg, &ModelReduced);
	HTuple ModelIDShow;
	HTuple  AngleStart, AngleExtent;
	AngleStart = -(m_modelAnge/180.0)*3.14;
	AngleExtent = 2 *(m_modelAnge/180.0)*3.14;
	int returnTrue=create_shape_model(ModelReduced, 0, AngleStart, AngleExtent, "auto", "auto", "use_polarity", m_contrast, m_minContrast, &ModelIDShow);
	if(returnTrue==2)
	{
		HTuple CenterRow,CenterCol,Area;
		Hobject regionOrgUnion;
		union1(regionOrg,&regionOrgUnion);
		area_center (regionOrgUnion, &Area, &CenterRow, &CenterCol);

		Hobject ModelContours,ModelRegEdges,ModelRegEdgesMoved; 
		get_shape_model_contours (&ModelContours, ModelIDShow, 1);
		ImageProcessAlg.XldsToRegion(ModelContours,&ModelRegEdges,0.5);
		move_region (ModelRegEdges, &ModelRegEdgesMoved,CenterRow,CenterCol);
		concat_obj (regionOrgUnion, ModelRegEdgesMoved, &regionOrg);
	}
	//
	*pObj = regionOrg;
	return S_OK;
#else
	//颠倒罐新算法-2017.10.13
	//显示模板区域(校正后图片对应的区域替换为原始图片对应的区域)	

	//利用最新的参数，生成其检测区域

	if (m_bIsRegParamChanged)
	{
		Hobject RegInOrigImg,RegInOrigImgTemp, m_mapImgTemp;
		HTuple  ObjNum;
		Hobject SelectedMapImg;

		count_obj(m_mapImgOrigin, &ObjNum);
		gen_empty_obj(&m_mapImg);
		gen_empty_obj(&RegInOrigImg);
		for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
		{
			select_obj(m_mapImgOrigin, &SelectedMapImg, ObjIndex);
			H_CropMap(SelectedMapImg, &RegInOrigImgTemp ,m_StartRowPercents[ObjIndex-1], m_EndRowPercents[ObjIndex-1], &m_mapImgTemp);

			concat_obj(RegInOrigImg, RegInOrigImgTemp, &RegInOrigImg);
			concat_obj(m_mapImg, m_mapImgTemp, &m_mapImg);
		}

		string strModelRegionPath;
		strModelRegionPath=m_strTaskPath+ "\\Modelling.reg";
		write_region(RegInOrigImg, HTuple(strModelRegionPath.c_str()));
		m_MapRegInOrigImg = RegInOrigImg;

		string strMapPath, strMatrixPath;
		strMapPath=m_strTaskPath+"\\Map.tiff";		
		strMatrixPath = m_strTaskPath + "\\ReferMatrix.mtx";

		write_image(m_mapImg, "tiff", 0, HTuple(strMapPath.c_str()));

#ifdef MAP_USE_FIX_REGION
		string strOriginPartitionROI_1=m_strTaskPath+"\\OriginPartitionROI_1.reg";
		string strOriginPartitionROI_2=m_strTaskPath+"\\OriginPartitionROI_2.reg";

		H_GetMultiMapRegInOrigImg(m_mapImg, &m_OriginPartitionROI_1, &m_OriginPartitionROI_2);

		write_region(m_OriginPartitionROI_1, HTuple(strOriginPartitionROI_1.c_str()));
		write_region(m_OriginPartitionROI_2, HTuple(strOriginPartitionROI_2.c_str()));					
#endif

		m_MatrixDataInstance.CropMatrixBasePos(m_ReferMappedImg, m_StartRowPercents, m_EndRowPercents, &m_ReferMatrix);
		write_matrix(m_MatrixDataInstance.GetCroppedMatrix(), "binary", HTuple(strMatrixPath.c_str()));

		*pObj = RegInOrigImg;
	} 
	else
	{
		*pObj = m_MapRegInOrigImg;
	}


	return S_OK;
#endif


}
STDMETHODIMP CCanAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CCanAlgo:: GetWhiteErrorRegion(BYTE rgnID,  LONG* pWhiteErrorRgn)
{
	Hobject* pObj = (Hobject*)pWhiteErrorRgn;
	*pObj = m_vWhiteErrorRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CCanAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
{
	if(m_dictSpecialData.find(rgnID) == m_dictSpecialData.end())
		return S_FALSE;

	std::vector<float> vStats = m_dictSpecialData[rgnID];
	int sz = (int)vStats.size();
	if(sz == 0)
		return S_FALSE;

	long count = min(sz, nMaxLen);
	*nActualLen = count;
	for(long i = 0; i < count; i++)
	{
		pStatsArray[i] = vStats[i];
	}
	return S_OK;
}
bool GetExePath(wstring& strExePath)
{

	TCHAR exeFullPath[MAX_PATH]; 
	memset(exeFullPath,0,MAX_PATH);  
	GetModuleFileName(NULL,exeFullPath,MAX_PATH); 
	CString exePath;
	exePath.Format(_T("%s"),exeFullPath);  

	std::wstring wstrPath = CT2W(exePath);

	if(wstrPath == L"")
	{
		return false;
	}
	wstring strTmp = wstrPath;
	size_t index = strTmp.find_last_of(L"\\");
	if(index != string::npos)
	{
		strExePath = strTmp.substr(0, index);
	}
	return true;
}
STDMETHODIMP CCanAlgo::SetCurrentTaskName(BSTR* bstrTaskName,BSTR* bstrChName)
{   
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+*bstrChName+L"\\"+m_strTaskName;
	m_strTaskPath = CW2A(m_wstrTaskPath.c_str());
	return S_OK;
}

STDMETHODIMP CCanAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName,BSTR* bstrChName)
{
	//TODO:
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+*bstrChName+L"\\"+m_strTaskName;
	m_strTaskPath = CW2A(m_wstrTaskPath.c_str());
	return S_OK;
}

//判断映射图像是否存在
BOOL IsImageExsit(string MapPath, string MulImgPath)
{
	if((_access(MapPath.c_str(),0)==0) &&
		(_access(MulImgPath.c_str(),0)==0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//判断图像是否为棋盘格图像
BOOL  IsCheckerboardImg(Hobject Image, int MinContrast, int  GrayThreshold, double  AreaRatioThresh, int  SaddlePtThresh)
{
	//?????
	MinContrast=20;
	GrayThreshold=10;
	AreaRatioThresh=0.3;
	//
	HTuple Width, Height, GridRegionArea;
	Hobject GridRegion, ImageReduced;
	HTuple row, column, AreaRatio, PointNum;

	get_image_size(Image, &Width, &Height);
	find_rectification_grid(Image, &GridRegion, MinContrast, 7.5);
	area_center(GridRegion, &GridRegionArea, &row, &column);
	AreaRatio = 1.0*GridRegionArea/(Width*Height);

	reduce_domain(Image, GridRegion, &ImageReduced);
	saddle_points_sub_pix(ImageReduced, "facet", 1.5, GrayThreshold, &row, &column);
	tuple_length(row, &PointNum);
	if (AreaRatio > AreaRatioThresh &&  PointNum > SaddlePtThresh)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//产生映射图像
void GetMapMulImage(Hobject CheckerBoard, HTuple ProgramDir, Hobject * MapImg, Hobject * MulImg, HTuple GrayThreshold)
{
	Hobject ImageDilation, ImageErosion;
	Hobject ThreshReg, RegionErosion, ConnectedRegion;
	Hobject RegionDilation, RegionErosion1, RegionDifference;
	Hobject OuterRect, BlockOutLine;
	int		GrayMaskSize = 3;
	HTuple  ImageHeight, ImageWidth;
	get_image_size(CheckerBoard, &ImageWidth, &ImageHeight);
	//去除噪声
	gray_dilation_rect(CheckerBoard, &ImageDilation, GrayMaskSize, GrayMaskSize);
	gray_erosion_rect(ImageDilation, &ImageErosion, GrayMaskSize, GrayMaskSize);

	//定位黑色方格外围边框
	threshold(ImageErosion, &ThreshReg,0, GrayThreshold);
	erosion_circle(ThreshReg, &RegionErosion, 2);
	connection(RegionErosion, &ConnectedRegion);
	dilation_circle(ConnectedRegion, &RegionDilation, 7.5);
	erosion_circle(RegionDilation, &RegionErosion1, 2.5);
	difference(RegionDilation, RegionErosion1, &RegionDifference);
	gen_rectangle1(&OuterRect, 5, 5, ImageHeight-5, ImageWidth-5);
	intersection(RegionDifference, OuterRect, &BlockOutLine);

	//获得灰度均衡映射
	HTuple Mean, Deviation;
	Hobject ImageResult,ImageReduce1, ExpandedImage;
	Hobject ImageMean, WhiteImage, MapTemp, MulTemp;
	intensity(BlockOutLine, CheckerBoard, &Mean, &Deviation);
	paint_region(RegionErosion1, ImageErosion, &ImageResult, Mean, "fill");
	reduce_domain(ImageResult, OuterRect, &ImageReduce1);
	expand_domain_gray(ImageReduce1, &ExpandedImage, 5);
	mean_image(ExpandedImage, &ImageMean, 57,57);
	gen_image_proto(ExpandedImage, &WhiteImage, 255);
	div_image(WhiteImage, ImageMean, &MulTemp, 50, 0);
	copy_image(MulTemp, MulImg);

	//获得校正映射
	Hobject EnhancedImg, GridRegion;
	Hobject GridImage, ImageMedian;
	Hobject GridPoints, GridPointsDilation, GridConnected, ConnectingLines;
	HTuple Row, Col;
	HTuple Area1, Row1, Col1;
	mult_image(ImageErosion, MulTemp, &EnhancedImg, 0.02, 0);
	find_rectification_grid(EnhancedImg, &GridRegion, 20,10);
	dilation_circle(GridRegion, &GridRegion, 6.5);
	reduce_domain(EnhancedImg, GridRegion, &GridImage);
	median_image(GridImage, &ImageMedian, "circle", 1, "mirrored");
	saddle_points_sub_pix(ImageMedian, "facet", 1.0, 8, &Row, &Col);
	gen_region_points(&GridPoints, Row, Col);
	dilation_circle(GridPoints, &GridPointsDilation, 3.5);
	connection(GridPointsDilation, &GridConnected);
	area_center(GridConnected, &Area1, &Row1, &Col1);
	HTuple Sigma = 0.9;
	HTuple MaxDist = 5;
	HTuple GridSpacing = 25;
	Hobject Mesh;
	connect_grid_points(GridImage, &ConnectingLines, Row1, Col1,Sigma, MaxDist);
	gen_grid_rectification_map(GridImage, ConnectingLines, &MapTemp, &Mesh, GridSpacing, 0, Row1, Col1, "coord_map_sub_pix");
	copy_image(MapTemp,MapImg );
	write_image(MapTemp,"tiff",0,ProgramDir+"\\Map.tiff");
	write_image(MulTemp,"tiff",0,ProgramDir+"\\MulImg.tiff");
}
//创建模板(use model region)
void CreateShapeModels(Hobject OriginImg, Hobject Image,Hobject ModelRegion , HTuple MinScore, HTuple Contrast, HTuple MinContrast,HTuple AngleTolerance,HTuple ProgramDir,Hobject &MultiChannelModelImage,Hobject &MultiChannelModelImageRect, HTuple& SaveModelIndex, HTuple& Models,BOOL * m_bModel)
{
	set_check("~give_error");
	HTuple  ImageHeightOrg, ImageWidthOrg,ImageHeight, ImageWidth;
	HTuple  AngleStart, AngleExtent;

	Hobject SearchRegion;

	*m_bModel = FALSE;
	AngleStart = -AngleTolerance.Rad();
	AngleExtent = 2 * AngleTolerance.Rad();

	//确定SearchRegion	//针对棋盘格在图像中水平放置，设定相关区域
	get_image_size(Image, &ImageWidth, &ImageHeight);
	HTuple ModelArea1,ModelRow1, ModelColumn1;
	area_center (ModelRegion, &ModelArea1, &ModelRow1, &ModelColumn1);
	gen_rectangle2(&SearchRegion, ModelRow1, ModelColumn1, 0, 0.2 *ImageWidth,  0.05*ImageHeight);
	Hobject  ImageReduced, ModelReduced;
	HTuple ImageFolder, ModelFolder, ImageFiles, ModelFiles;
	HTuple Index, ModelID;
	HTuple  Row_matched, Column_matched, Angle_matched, Score_matched, ModelIndex_matched;
	ImageFolder = ProgramDir + "\\TemplateImage";
	ModelFolder = ProgramDir + "\\Models";

	if (0 ==SaveModelIndex)
	{
		//无文件夹时首先创建文件夹
		if (_access(ImageFolder[0].S(), 0))
		{
			CString  ImageDir( ImageFolder[0].S());
			CreateDirectory(ImageDir, NULL);
		}

		if (_access(ModelFolder[0].S(), 0))
		{
			CString  ModelDir( ModelFolder[0].S());
			CreateDirectory(ModelDir, NULL);
		}

		//first delete the exist templates
		list_files(ImageFolder, (HTuple("files").Append("follow_links")), &ImageFiles);
		tuple_regexp_select(ImageFiles, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ImageFiles);
		HTuple Length_pre  = ImageFiles.Num();
		if (Length_pre > 0 )
		{
			for (Index=0; Index <= (Length_pre -1); Index+=1)
				delete_file(HTuple(ImageFiles[Index]));
		}

		list_files(ModelFolder, (HTuple("files").Append("follow_links")), &ModelFiles);
		tuple_regexp_select(ModelFiles, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ModelFiles);
		HTuple Length_Model  = ModelFiles.Num();
		if (Length_Model > 0 )
		{
			for (Index=0; Index <= (Length_Model -1); Index+=1)
				delete_file(HTuple(ModelFiles[Index]));
		}
		reduce_domain(Image, ModelRegion, &ImageReduced);
		int returnTrue=create_shape_model(ImageReduced, 0, AngleStart, AngleExtent, "auto", "auto", "use_polarity", Contrast, MinContrast, &ModelID);
		if(returnTrue==2)
		{
			Models.Reset();
			Models.Append(ModelID);
			write_shape_model(ModelID, ModelFolder+"\\000.shm");
			//
			write_image(OriginImg,"tiff",0, ImageFolder+"\\000.tiff");
			write_region(ModelRegion,ProgramDir+"\\Modelling.reg");
			write_image(Image,"tiff",0,ProgramDir+"\\ModellingImage.tiff");
			SaveModelIndex+= 1;
			*m_bModel = TRUE;
			channels_to_image(OriginImg, &MultiChannelModelImage);
			channels_to_image(Image, &MultiChannelModelImageRect);
		}
		else
		{
			*m_bModel = FALSE;
		}
	} 
	else if(SaveModelIndex[0].I() > 0)
	{
		reduce_domain(Image,SearchRegion, &ImageReduced);
		find_shape_models(ImageReduced, Models,  AngleStart,	AngleExtent, 0.5, 1, 0.5, "least_squares", 0, 0.7,&Row_matched, &Column_matched, &Angle_matched,  &Score_matched, &ModelIndex_matched);
		HTuple Length_Score ;
		tuple_length(Score_matched, &Length_Score);
		//write image if the current image doesn't match the exist images 
		if (Score_matched < MinScore || Length_Score == 0)
		{
			reduce_domain(Image, ModelRegion, &ModelReduced);
			int returnTrue=create_shape_model(ModelReduced, 0, AngleStart, AngleExtent, "auto", "auto", "use_polarity", Contrast, MinContrast, &ModelID);
			if(returnTrue==2)
			{
				Models.Append(ModelID);	
				write_shape_model(ModelID, ModelFolder+"\\"+ HTuple(SaveModelIndex.ToString(".03")+".shm"));
				write_image(OriginImg,"tiff",0, ImageFolder+ "\\"+HTuple(SaveModelIndex.ToString(".03")+".tiff"));
				SaveModelIndex += 1;
				*m_bModel = TRUE;
				append_channel(MultiChannelModelImage, OriginImg, &MultiChannelModelImage);
				append_channel(MultiChannelModelImageRect,Image, &MultiChannelModelImageRect);
			}
			else
			{
				*m_bModel = FALSE;
			}
		}
		else
		{
			*m_bModel = FALSE;
		}
	}
	set_check("give_error");
}
//创建模板
void CreateShapeModels(Hobject OriginImg, Hobject Image, double ModelRegionHalfHeightRatio, double SearchRegionHalfWidthRatio, HTuple MinScore, HTuple Contrast, HTuple MinContrast,HTuple AngleTolerance,HTuple ProgramDir,Hobject &MultiChannelModelImage,Hobject &MultiChannelModelImageRect,	Hobject &ModelRegion, HTuple& SaveModelIndex, HTuple& Models,BOOL * m_bModel)
{
	set_check("~give_error");
	HTuple  ImageHeightOrg, ImageWidthOrg,ImageHeight, ImageWidth;
	HTuple  AngleStart, AngleExtent;

	Hobject SearchRegion;

	*m_bModel = FALSE;
	AngleStart = -AngleTolerance.Rad();
	AngleExtent = 2 * AngleTolerance.Rad();

	//确定SearchRegion, ModelRegion
	get_image_size(Image, &ImageWidthOrg, &ImageHeightOrg);
	get_image_size(Image, &ImageWidth, &ImageHeight);
	//针对棋盘格在图像中水平放置，设定相关区域
	gen_rectangle2(&ModelRegion, 0.5 *ImageHeight, 0.5*ImageWidth, 0, 0.45 *ImageWidth,  ModelRegionHalfHeightRatio *ImageHeightOrg);
	gen_rectangle2(&SearchRegion, 0.5 *ImageHeight, 0.5*ImageWidth, 0, SearchRegionHalfWidthRatio *ImageWidth,  0.05*ImageHeight);


	Hobject  ImageReduced, ModelReduced;
	HTuple ImageFolder, ModelFolder, ImageFiles, ModelFiles;
	HTuple Index, ModelID;
	HTuple  Row_matched, Column_matched, Angle_matched, Score_matched, ModelIndex_matched;
	ImageFolder = ProgramDir + "\\TemplateImage";
	ModelFolder = ProgramDir + "\\Models";

	if (0 ==SaveModelIndex)
	{
		//无文件夹时首先创建文件夹
		if (_access(ImageFolder[0].S(), 0))
		{
			CString  ImageDir( ImageFolder[0].S());
			CreateDirectory(ImageDir, NULL);
		}

		if (_access(ModelFolder[0].S(), 0))
		{
			CString  ModelDir( ModelFolder[0].S());
			CreateDirectory(ModelDir, NULL);
		}

		//first delete the exist templates
		list_files(ImageFolder, (HTuple("files").Append("follow_links")), &ImageFiles);
		tuple_regexp_select(ImageFiles, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ImageFiles);
		HTuple Length_pre  = ImageFiles.Num();
		if (Length_pre > 0 )
		{
			for (Index=0; Index <= (Length_pre -1); Index+=1)
				delete_file(HTuple(ImageFiles[Index]));
		}

		list_files(ModelFolder, (HTuple("files").Append("follow_links")), &ModelFiles);
		tuple_regexp_select(ModelFiles, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ModelFiles);
		HTuple Length_Model  = ModelFiles.Num();
		if (Length_Model > 0 )
		{
			for (Index=0; Index <= (Length_Model -1); Index+=1)
				delete_file(HTuple(ModelFiles[Index]));
		}
		reduce_domain(Image, ModelRegion, &ImageReduced);
		int returnTrue=create_shape_model(ImageReduced, 0, AngleStart, AngleExtent, "auto", "auto", "use_polarity", Contrast, MinContrast, &ModelID);
		if(returnTrue==2)
		{
			Models.Reset();
			Models.Append(ModelID);
			write_shape_model(ModelID, ModelFolder+"\\000.shm");
			//
			write_image(OriginImg,"tiff",0, ImageFolder+"\\000.tiff");
			write_region(ModelRegion,ProgramDir+"\\Modelling.reg");
			write_image(Image,"tiff",0,ProgramDir+"\\ModellingImage.tiff");
			SaveModelIndex+= 1;
			*m_bModel = TRUE;
			channels_to_image(OriginImg, &MultiChannelModelImage);
			channels_to_image(Image, &MultiChannelModelImageRect);
		}
		else
		{
			*m_bModel = FALSE;
		}
	} 
	else if(SaveModelIndex[0].I() > 0)
	{
		reduce_domain(Image,SearchRegion, &ImageReduced);
		find_shape_models(ImageReduced, Models,  AngleStart,	AngleExtent, 0.5, 1, 0.5, "least_squares", 0, 0.7,&Row_matched, &Column_matched, &Angle_matched,  &Score_matched, &ModelIndex_matched);
		HTuple Length_Score ;
		tuple_length(Score_matched, &Length_Score);
		//write image if the current image doesn't match the exist images 
		if (Score_matched < MinScore || Length_Score == 0)
		{
			reduce_domain(Image, ModelRegion, &ModelReduced);
			int returnTrue=create_shape_model(ModelReduced, 0, AngleStart, AngleExtent, "auto", "auto", "use_polarity", Contrast, MinContrast, &ModelID);
			if(returnTrue==2)
			{
				Models.Append(ModelID);	
				write_shape_model(ModelID, ModelFolder+"\\"+ HTuple(SaveModelIndex.ToString(".03")+".shm"));
				write_image(OriginImg,"tiff",0, ImageFolder+ "\\"+HTuple(SaveModelIndex.ToString(".03")+".tiff"));
				SaveModelIndex += 1;
				*m_bModel = TRUE;
				append_channel(MultiChannelModelImage, OriginImg, &MultiChannelModelImage);
				append_channel(MultiChannelModelImageRect,Image, &MultiChannelModelImageRect);
			}
			else
			{
				*m_bModel = FALSE;
			}
		}
		else
		{
			*m_bModel = FALSE;
		}
	}
	set_check("give_error");
}
STDMETHODIMP CCanAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{
	// m_isFirstReadSelfLearningPara 在合适位置重置，设为true.
	if(m_isFirstReadSelfLearningPara)
	{	
		CString strExtractPara=*bstrParam;
		std::wstring wstrPath = CT2W(strExtractPara);
		std::string strPara = CW2A(wstrPath.c_str());
		std::vector<string> vPara = SplitString(strPara, ",");
		if(vPara.empty())
		{
			return S_FALSE;
		}
		size_t paraNum = vPara.size();

#ifndef MIX_LABEL_ALGO_NEW
		if(paraNum!=7)
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("自学习参数个数与设定值不一致。"),_T("参数数量错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("The number of self-learning parameters is different from setting value."),_T("The number of parameters is wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}

			return S_FALSE;
		}
		m_strRegionExtractPara=*bstrParam;
		m_modelRegionHeight=(int)atof(vPara[0].c_str());
		m_modelMinScore=(int)atof(vPara[1].c_str());
		m_modelAnge=(int)atof(vPara[2].c_str());
		m_contrast=(int)atof(vPara[3].c_str());
		m_minContrast=(int)atof(vPara[4].c_str());
		m_readChessImage=(int)atof(vPara[5].c_str());
		m_chessNum=(int)atof(vPara[6].c_str());
		//
		m_modelParaOfSelfLearning.ReplaceElements(0,m_contrast);
		m_modelParaOfSelfLearning.ReplaceElements(1,m_minContrast);
		//
#else
		if(paraNum!=6)
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("自学习参数个数与设定值不一致。"),_T("参数数量错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("The number of self-learning parameters is different from setting value."),_T("The number of parameters is wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			return S_FALSE;
		}

		m_LeftCameraStartRowPercent = (double)atof(vPara[0].c_str());
		m_LeftCameraEndRowPercent = (double)atof(vPara[1].c_str());
		m_RightCameraStartRowPercent = (double)atof(vPara[2].c_str());
		m_RightCameraEndRowPercent = (double)atof(vPara[3].c_str());
		m_VecDiffThreshold = atof(vPara[4].c_str());
		m_readChessImage=(int)atof(vPara[5].c_str());

		CheckParamValid(m_LeftCameraStartRowPercent);
		CheckParamValid(m_LeftCameraEndRowPercent);
		if (m_LeftCameraStartRowPercent >= m_LeftCameraEndRowPercent)
		{
			m_LeftCameraStartRowPercent = m_LeftCameraEndRowPercent - 5;
		}


		CheckParamValid(m_RightCameraStartRowPercent);
		CheckParamValid(m_RightCameraEndRowPercent);
		if (m_RightCameraStartRowPercent >= m_RightCameraEndRowPercent)
		{
			m_RightCameraStartRowPercent = m_RightCameraEndRowPercent -5;
		}


		m_StartRowPercents[0] = m_LeftCameraStartRowPercent;
		m_StartRowPercents[1] = m_RightCameraStartRowPercent;

		m_EndRowPercents[0] = m_LeftCameraEndRowPercent;
		m_EndRowPercents[1] = m_RightCameraEndRowPercent;

		m_StartRowPercents_Last = m_StartRowPercents;
		m_EndRowPercents_Last = m_EndRowPercents;
		m_bIsRegParamChanged = false;

#endif
	}
	return S_OK;
}

bool TryDeleteFile(string FilePath)
{
	bool result = false;
	try
	{
		if (_access(FilePath.c_str(),0)==0)
		{
			delete_file(FilePath.c_str());
			result = true;
		}
	}
	catch (HException &except) 
	{
		return S_FALSE;
	}


	return result;
}

STDMETHODIMP CCanAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{
#ifndef MIX_LABEL_ALGO_NEW 
	//临时变量
	//m_strTaskPath="C:\\SMVS2106\\trunk\\x64\\Debug\\data\\TESTD6\\";
	//无文件夹时首先创建文件夹
	if (_access(m_strTaskPath.c_str(), 0))
	{
		CString  ImageDir( m_strTaskPath.c_str());
		CreateDirectory(ImageDir, NULL);
	}
	string CheckerBoardImg;
	string strMapPath;
	string strMulImgPath;
	string strModelFolderPath;
	string strTempImgFolderPath;
	string strModelPath;
	string strModelRegionPath;
	string strModelParaPath;
	CheckerBoardImg=m_strTaskPath+"\\CheckerBoardImg.tiff";	
	strMapPath=m_strTaskPath+"\\Map.tiff";
	strMulImgPath=m_strTaskPath+"\\MulImg.tiff";
	strModelRegionPath=m_strTaskPath+ "\\Modelling.reg" ;	
	strModelParaPath=m_strTaskPath+ "\\ModelPara.tup" ;	
	strModelFolderPath=m_strTaskPath+"\\Models\\";
	strTempImgFolderPath=m_strTaskPath+ "\\TemplateImage\\" ;	
	//
	if(productID==3)
	{
		Hobject* pObj = (Hobject*)pCrtImg;
		Hobject ImageOrg= *pObj;
		Hobject Image;
		//
		HTuple  Channels;
		count_channels(ImageOrg, &Channels);
		//黑白图片
		if(Channels[0].I()==3)
		{
			rgb1_to_gray(ImageOrg, &Image);
		}
		else if(Channels[0].I()==1)
		{
			Image  = *pObj;
		}
		else
		{
			mean_n(ImageOrg, &Image);
		}		m_nImgIndex++;
		if (m_strTaskPath.length()<3)
		{
			if (m_nImgIndex<=1)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("路径错误！"),_T("训练提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T( "Save path error."),_T("Training Info:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}	
			}
			return S_FALSE;
		}
#ifdef NDEBUG
		try
		{
#endif
			HTuple bMapExsit, bCheckerBoardImg;
			bMapExsit = IsImageExsit(strMapPath, strMulImgPath);
			if ((m_readChessImage>0)&(bMapExsit[0].I()))
			{
				bCheckerBoardImg=0;
			}
			else
			{
				bCheckerBoardImg = IsCheckerboardImg(Image, 20,10, 0.30, m_chessNum);
			}
			if (bCheckerBoardImg[0].I())
			{
				if (bMapExsit[0].I())
				{
					delete_file(HTuple(strMapPath.c_str()));
					delete_file(HTuple(strMulImgPath.c_str()));
				}

				GetMapMulImage(Image,HTuple(m_strTaskPath.c_str()), &m_mapImg, &m_mulImg, 40);
				//重新建模
				m_modelNum=0;
				m_nImgIndex=0;
				write_image(Image,"tiff",0,HTuple(CheckerBoardImg.c_str()));
				return S_OK;
			}
			else
			{
				if (!bMapExsit[0].I())
				{
					if (m_nImgIndex<=1)
					{
						if(m_isChineseLanguage)
						{
							MessageBox(NULL,_T("棋盘格图像缺失，请放入样罐"),_T("训练提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
						}
						else
						{
							MessageBox(NULL,_T( "Missing Image Map.tiff, please put in sample can with CheckerBoard"),_T("Training Info:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
						}	
					}
					return S_FALSE;
				}
				else
				{
					Hobject ImageEnhanced, ImageRectified;
					if (m_modelNum <=0 )//m_nSampleSize
					{
						read_image(&m_mulImg, HTuple(strMulImgPath.c_str()));
						read_image(&m_mapImg, HTuple(strMapPath.c_str()));
						write_tuple(m_modelParaOfSelfLearning, HTuple(strModelParaPath.c_str()));
					}
					mult_image(Image, m_mulImg, &ImageEnhanced, 0.02, 0);
					map_image(ImageEnhanced, m_mapImg, &ImageRectified);
					HTuple WithRectify,HeightRectify;
					get_image_size(ImageRectified,&WithRectify,&HeightRectify);
					HTuple  AngleTolerance = m_modelAnge;
					HTuple  MinScore =m_modelMinScore/100.0;
					double  ModelRegionHalfHeightRatio =(0.5*m_modelRegionHeight)/((HeightRectify[0].I())+0.0001);
					double  SearchRegionHalfWidthRatio =40/100.0;
					int Contrast=(int)m_contrast;
					int MinContrast=(int)m_minContrast;
					//异常数据处理！
					if ((Contrast<1)||(Contrast>100))
					{
						Contrast=40;
					}
					if ((MinContrast<1)||(MinContrast>100))
					{
						MinContrast=5;
					}
					if ((MinScore<0.1)||(MinScore>0.99))
					{
						MinScore=0.6;
					}
					if ((AngleTolerance<1)||(AngleTolerance>180))
					{
						AngleTolerance=20;
					}
					if (ModelRegionHalfHeightRatio>0.49)
					{
						ModelRegionHalfHeightRatio=0.05;
					}
					//
					if (m_modelRegionHeight>=6)
					{
						//表示当前图像是否加入模板组
						BOOL    bModel;
						if (m_modelNum <= 149)//m_nSampleSize; m_nImgIndex <= 100 
						{
							//创建模板组
							CreateShapeModels(Image, ImageRectified, ModelRegionHalfHeightRatio, SearchRegionHalfWidthRatio, MinScore,Contrast,MinContrast, AngleTolerance,HTuple(m_strTaskPath.c_str()),m_multiChannelModelImage,m_multiChannelModelImageRectified,m_modelRegion,m_modelNum ,m_models,&bModel);
							if(bModel)
							{
								return S_OK;
							}
						} 
					}
					else
					{
						if(_access(strModelRegionPath.c_str(),0)==0)

						{
							if(m_modelNum <=0)
							{
								read_region(&m_modelRegion,HTuple(strModelRegionPath.c_str()));
								//添加模板区域判断！
								HTuple RegionArea,RegionRow,RegionCol;
								area_center(m_modelRegion,&RegionArea,&RegionRow,&RegionCol);
								if(RegionArea<60)
								{
									if(m_isChineseLanguage)
									{
										MessageBox(NULL,_T("模板区域太小！"),_T("训练提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
									}
									else
									{
										MessageBox(NULL,_T( "The model region is too small."),_T("Training Info:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
									}	
									return S_FALSE;
								}
								///////
								HTuple  HomMat2DRotate;
								Hobject RegionAffineTrans,imageRect;
								vector_angle_to_rigid(HeightRectify/2.0, WithRectify/2.0,0,HeightRectify/2.0, WithRectify/2.0, 3.1416, &HomMat2DRotate);
								projective_trans_region (m_modelRegion, &RegionAffineTrans, HomMat2DRotate, "bilinear");
								union2(m_modelRegion, RegionAffineTrans, &m_modelRegion);
								gen_rectangle1(&imageRect,1,1,HeightRectify-2,WithRectify-2);
								intersection(m_modelRegion,imageRect,&m_modelRegion);
							}
						}
						else
						{
							if(m_modelNum <=0)//避免多次重复提示
							{
								if(m_isChineseLanguage)
								{
									MessageBox(NULL,_T("模板区域不存在，目前使用临时区域训练。"),_T("训练提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
								}
								else
								{
									MessageBox(NULL,_T( "Missing Model Region,use the Temporary model region this time."),_T("Training Info:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
								}
								gen_rectangle2(&m_modelRegion, 0.5 *HeightRectify, 0.5*WithRectify, 0, 0.4 *WithRectify,  0.1 *HeightRectify);
							}
							//return S_FALSE;
						}
						//使用现有模板区域
						BOOL    bModel;
						if (m_modelNum <= 149)//m_nSampleSize; m_nImgIndex <= 100 
						{
							//创建模板组
							CreateShapeModels(Image, ImageRectified,m_modelRegion, MinScore,Contrast,MinContrast, AngleTolerance,HTuple(m_strTaskPath.c_str()),m_multiChannelModelImage,m_multiChannelModelImageRectified,m_modelNum ,m_models,&bModel);
							if(bModel)
							{
								return S_OK;
							}
						} 

					}

				}
			}
#ifdef NDEBUG
		}
		catch (HException &except) 
		{
			m_algoSpy.QueueUpLogMsg(except, m_crtImg);
			return S_FALSE;
		}
#endif
	}
	return S_FALSE;

#else
	//无文件夹时首先创建文件夹
	if (_access(m_strTaskPath.c_str(), 0))
	{
		CString  ImageDir( m_strTaskPath.c_str());
		CreateDirectory(ImageDir, NULL);
	}

	string CheckerBoardImg;
	string strMapOriginPath;	//裁剪前的原始映射
	string strMapPath;
	string strMatrixOriginPath;	//裁剪前的原始矩阵
	string strMatrixPath;
	string strTempImgFolderPath;	
	string strModelRegionPath;	//投影到原始图片中的检测区域

	CheckerBoardImg=m_strTaskPath+"\\CheckerBoardImg.tiff";	
	strMapOriginPath = m_strTaskPath + "\\MapOrigin.tiff";
	strMapPath=m_strTaskPath+"\\Map.tiff";

	strMatrixOriginPath = m_strTaskPath + "\\ReferMatrixOrigin.mtx";
	strMatrixPath = m_strTaskPath + "\\ReferMatrix.mtx";
	strTempImgFolderPath=m_strTaskPath+ "\\TemplateImage\\";
	strModelRegionPath=m_strTaskPath+ "\\Modelling.reg";

	if (_access(strTempImgFolderPath.c_str(), 0))
	{
		CString  ImageDir(strTempImgFolderPath.c_str());
		CreateDirectory(ImageDir, NULL);
	}


	if(productID==3)
	{
		Hobject* pObj = (Hobject*)pCrtImg;
		Hobject Image= *pObj;
		m_nImgIndex++;
		if (m_strTaskPath.length()<3)
		{
			if (m_nImgIndex<=1)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("路径错误！"),_T("训练提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T( "Save path error."),_T("Training Info:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}	
			}
			return S_FALSE;
		}
#ifdef NDEBUG
		try
		{
#endif
			HTuple bMapExsit, bCheckerBoardImg;
			HTuple InnerR, OuterR;
			Hobject CanRegion;
			//Hobject MapRegInOrigImg;
			HTuple ErrorMsg;

			string strOriginPartitionROI_1=m_strTaskPath+"\\OriginPartitionROI_1.reg";
			string strOriginPartitionROI_2=m_strTaskPath+"\\OriginPartitionROI_2.reg";
			bMapExsit = !_access(strMapOriginPath.c_str(), 0);
			if ((m_readChessImage>0)&(bMapExsit[0].I()))
			{
				bCheckerBoardImg=0;
			}
			else
			{
				bCheckerBoardImg = H_IsCalibImage_new(Image, &CanRegion,&InnerR, &OuterR);				
			}


			if (bCheckerBoardImg[0].I())
			{
				if (bMapExsit[0].I())
				{
					//删除已有的文件
					delete_file(HTuple(strMapOriginPath.c_str()));					
				}						

				bool bMapResult = H_GetMap(Image, CanRegion, &m_mapImgOrigin, &m_MapRegInOrigImg ,InnerR, OuterR, &ErrorMsg);

				if (bMapResult)
				{
					//重新建模
					m_modelNum=0;
					m_nImgIndex=0;
					write_image(Image,"tiff",0,HTuple(CheckerBoardImg.c_str()));
					write_image(m_mapImgOrigin, "tiff", 0, HTuple(strMapOriginPath.c_str()));
					write_region(m_MapRegInOrigImg, HTuple(strModelRegionPath.c_str()));

#ifdef MAP_USE_FIX_REGION


					H_GetMultiMapRegInOrigImg(m_mapImgOrigin, &m_OriginPartitionROI_1, &m_OriginPartitionROI_2);

					write_region(m_OriginPartitionROI_1, HTuple(strOriginPartitionROI_1.c_str()));
					write_region(m_OriginPartitionROI_2, HTuple(strOriginPartitionROI_2.c_str()));					
#endif
					return S_OK;
				}				
			}
			else
			{
				if (!bMapExsit[0].I())
				{
					if (m_nImgIndex<=1)
					{
						if(m_isChineseLanguage)
						{
							MessageBox(NULL,_T("棋盘格图像缺失，请放入样罐"),_T("训练提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
						}
						else
						{
							MessageBox(NULL,_T( "Missing Image Map.tiff, please put in sample can with CheckerBoard"),_T("Training Info:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
						}	
					}
					return S_FALSE;
				}
				else
				{
					Hobject MappedImg;
					Hobject ImgDomain;
					HTuple  FeatureVector;
					HTuple  DiffValue, DiffIndex;
					HTuple TrainedImgNum;
					TrainedImgNum = m_MatrixDataInstance.GetTrainedNum();

					if (m_bPilotImg)
					{
						return S_OK;						
					}


					if (TrainedImgNum <=0 && !m_bPilotImg)//m_nSampleSize
					{
						TryDeleteFile(strOriginPartitionROI_1);
						TryDeleteFile(strOriginPartitionROI_2);
						TryDeleteFile(strMapPath);
						TryDeleteFile(strMatrixOriginPath);
						TryDeleteFile(strMatrixPath);
						//			TryDeleteFile(strTempImgFolderPath);
						TryDeleteFile(strModelRegionPath);

						read_image(&m_mapImgOrigin, HTuple(strMapOriginPath.c_str()));	

						H_GetMapOrginRegDualChannel(m_mapImgOrigin, &m_MapRegInOrigImg);

						Hobject MappedImgTemp;

						//存在checkerBordImg时，用其初始化
						if (_access(CheckerBoardImg.c_str(), 0))
						{
							H_MapImgs(Image, m_mapImgOrigin, &MappedImgTemp, &ErrorMsg);
						} 
						else
						{
							Hobject CalibImgTemp;
							read_image(&CalibImgTemp, HTuple(CheckerBoardImg.c_str()));
							H_MapImgs(CalibImgTemp, m_mapImgOrigin, &MappedImgTemp, &ErrorMsg);
						}

						m_MatrixDataInstance.Initial(MappedImgTemp);


						gen_empty_obj(&m_multiChannelModelImageRectified);
						concat_obj(m_multiChannelModelImageRectified, Image, &m_multiChannelModelImageRectified);

						copy_image(MappedImgTemp, &m_ReferMappedImg);

#ifdef MAP_USE_FIX_REGION
						string strOriginPartitionROI_1=m_strTaskPath+"\\OriginPartitionROI_1.reg";
						string strOriginPartitionROI_2=m_strTaskPath+"\\OriginPartitionROI_2.reg";


						if (_access(strOriginPartitionROI_1.c_str(),0) || _access(strOriginPartitionROI_2.c_str(),0))
						{
							H_GetMultiMapRegInOrigImg(m_mapImgOrigin, &m_OriginPartitionROI_1, &m_OriginPartitionROI_2);

							write_region(m_OriginPartitionROI_1, HTuple(strOriginPartitionROI_1.c_str()));
							write_region(m_OriginPartitionROI_2, HTuple(strOriginPartitionROI_2.c_str()));


						} 
						else
						{
							read_region(&m_OriginPartitionROI_1, HTuple(strOriginPartitionROI_1.c_str()));
							read_region(&m_OriginPartitionROI_2, HTuple(strOriginPartitionROI_2.c_str()));
						}
						//为了检测的时候节约时间 union耗时
						union1(m_OriginPartitionROI_1,&m_OriginPartitionUnionROI_1);
						union1(m_OriginPartitionROI_2,&m_OriginPartitionUnionROI_2);

						string strModelRegionPath= m_strTaskPath+ "\\Modelling.reg";
						if (_access(strModelRegionPath.c_str(),0))
						{
							gen_empty_obj(&m_MapRegInOrigImg);
							H_GetMapOrginRegDualChannel(m_mapImgOrigin, &m_MapRegInOrigImg);
							write_region(m_MapRegInOrigImg , HTuple(strModelRegionPath.c_str()));
						} 
						else
						{
							read_region(&m_MapRegInOrigImg, HTuple(strModelRegionPath.c_str()));
						}
#endif
					}

#ifndef MAP_USE_FIX_REGION
					H_MapImgs(Image, m_mapImgOrigin, &MappedImg, &ErrorMsg);
					get_domain(MappedImg, &ImgDomain);
					H_GetFeatureVector(MappedImg, ImgDomain, &FeatureVector);
#else

					H_GetFeatureVector_FixReg(Image, m_MapRegInOrigImg, m_OriginPartitionROI_1, m_OriginPartitionROI_2,m_OriginPartitionUnionROI_1, m_OriginPartitionUnionROI_2 ,&FeatureVector);
#endif

					bool IsToSaveImg = false;


					if (TrainedImgNum <= 0)
					{
						m_MatrixDataInstance.AddVector(FeatureVector);

						IsToSaveImg = true;
					}
					else if(TrainedImgNum <= m_MatrixDataInstance.GetMaxTrainedImgNum())
					{
						m_ReferMatrixOrigin = m_MatrixDataInstance.GetMatrix();

						H_Calc_Vector_Diff(m_ReferMatrixOrigin, FeatureVector, TrainedImgNum, &DiffValue, &DiffIndex);


						if (DiffValue > m_VecDiffThreshold)
						{
							m_MatrixDataInstance.AddVector(FeatureVector);
							IsToSaveImg = true;

							//查看原始图片
							concat_obj(m_multiChannelModelImageRectified, Image, &m_multiChannelModelImageRectified);
						}
					}

					TrainedImgNum = m_MatrixDataInstance.GetTrainedNum();
					m_modelNum = TrainedImgNum;
					// 					if (IsToSaveImg)
					// 					{
					// 						write_image(Image,"tiff",0, HTuple(strTempImgFolderPath.c_str())+ "\\"+HTuple(TrainedImgNum.ToString(".03")+".tiff"));
					// 					}
				}
			}

#ifdef NDEBUG
		}
		catch (HException &except) 
		{
			m_algoSpy.QueueUpLogMsg(except, m_crtImg);
			return S_FALSE;
		}
#endif
	}
	return S_FALSE;
#endif	
}

STDMETHODIMP CCanAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
#ifndef MIX_LABEL_ALGO_NEW
	Hobject* pObj = (Hobject*)pImg;
	Hobject Image;
	Hobject AddModelImgMult      = *pObj;// 原始图片，可能是多通道图片（彩色图片）
	HTuple  Channels;
	count_channels(AddModelImgMult, &Channels);
	//黑白图片
	if(Channels[0].I()==3)
	{
		rgb1_to_gray(AddModelImgMult, &Image);
	}
	else if(Channels[0].I()==1)
	{
		Image  = *pObj;
	}
	else
	{
		mean_n(AddModelImgMult, &Image);
	}
	if(m_bFileUpdated)
	{
		m_bFileUpdated=false;
	}
	//
	HTuple  Class;
	get_obj_class(m_mulImg, &Class);
	if (!(Class==HTuple("image")))
	{
		if(m_isChineseLanguage)
		{
			MessageBox(NULL,_T("添加模板失败"),_T("添加模板："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T( "Adding model fail"),_T("Add Model:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		m_addModelState=false;
		return S_FALSE;
	}
	//
	HTuple  Width, Height,Width1, Height1,Width2, Height2;
	get_image_size(Image, &Width, &Height);
	get_image_size(m_mulImg, &Width1, &Height1);
	if((Width!=Width1)||(Height!=Height1))
	{
		zoom_image_size(Image, &Image, Width1, Height1, "constant");
	}
	//保存原图像
	string strImagePath,strModelPath;
	int modelNum=m_modelNum[0].I();
	CString	NumberPath;
	NumberPath.Format(_T("%03i"),modelNum);
	string	NumberPath_str= CT2A(NumberPath.GetString());
	//
	strImagePath=(m_strTaskPath+"\\TemplateImage\\"+NumberPath_str)+".tiff";//+ (m_modelNum).ToString(".03")
	strModelPath=(m_strTaskPath+"\\Models\\"+NumberPath_str)+".shm";//+ (m_modelNum).ToString(".03")
	//

	Hobject AddImageEnhanced,AddImageRectified,ImageReducedForCreateModel; 
	mult_image(Image, m_mulImg, &AddImageEnhanced, 0.02, 0);
	map_image(AddImageEnhanced, m_mapImg, &AddImageRectified);
	//
	HTuple WithRectify,HeightRectify;
	get_image_size(AddImageRectified,&WithRectify,&HeightRectify);
	//添加模板区域判断！
	HTuple RegionArea,RegionRow,RegionCol;
	area_center(m_modelRegion,&RegionArea,&RegionRow,&RegionCol);
	if(RegionArea<60)
	{
		if(m_isChineseLanguage)
		{
			MessageBox(NULL,_T("模板区域太小！"),_T("训练提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T( "The model region is too small."),_T("Training Info:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}	
		return S_FALSE;
	}
	///////
	HTuple  HomMat2DRotate;
	Hobject RegionAffineTrans,ModelRegion,imageRect;
	vector_angle_to_rigid(HeightRectify/2.0, WithRectify/2.0,0,HeightRectify/2.0, WithRectify/2.0, 3.1416, &HomMat2DRotate);
	projective_trans_region (m_modelRegion, &RegionAffineTrans, HomMat2DRotate, "bilinear");
	union2(m_modelRegion, RegionAffineTrans, &ModelRegion);
	gen_rectangle1(&imageRect,1,1,HeightRectify-2,WithRectify-2);
	intersection(ModelRegion,imageRect,&ModelRegion);
	//
	reduce_domain(AddImageRectified,ModelRegion,&ImageReducedForCreateModel);
	int returnTrue=create_shape_model(ImageReducedForCreateModel, 0, m_angleStart, m_angleExtent, "auto", "auto", "use_polarity", m_contrast, m_minContrast, &m_addModelID);
	//
	if(returnTrue==2)
	{
		Hobject Circle,ImageReduced;
		get_image_size(AddImageRectified, &Width2, &Height2);
		gen_circle(&Circle, Height2/2.0, Width2/2.0, 30.5);
		reduce_domain(AddImageRectified, Circle, &ImageReduced);
		//
		HTuple Row_M,Column_M,Angle_M,Score_M;
		find_shape_model(ImageReduced, m_addModelID,  m_angleStart, m_angleExtent, 0.5, 1, 0.5, "least_squares", 0, 0.7,  &Row_M, &Column_M, &Angle_M, &Score_M);//(HTuple(4).Append(-2)), 0.8
		if (Score_M>0.50)
		{
			write_image(AddModelImgMult,"tiff",0,HTuple(strImagePath.c_str()));
			write_shape_model(m_addModelID, HTuple(strModelPath.c_str()));
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("添加模板成功"),_T("添加模板："), MB_OK);
			}
			else
			{
				MessageBox(NULL,_T( "Adding model successfully"),_T("Add Model:"), MB_OK);
			}
			m_addModelState=true;
			return S_OK;//S_OK,S_FALSE
		}
		else
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("添加模板失败"),_T("添加模板："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T( "Adding model fail"),_T("Add Model:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			m_addModelState=false;
			return S_FALSE;//S_OK,S_FALSE
		}
	}
	else
	{
		if(m_isChineseLanguage)
		{
			MessageBox(NULL,_T("添加模板失败"),_T("添加模板："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T( "Adding model fail"),_T("Add Model:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		m_addModelState=false;
		return S_FALSE;//S_OK,S_FALSE
	}
#else
	Hobject* pObj = (Hobject*)pImg;
	Hobject Image = *pObj;// 原始图片，可能是多通道图片（彩色图片）

	//用最新的裁剪后的map映射
	Hobject MappedImg, ImgDomain;
	HTuple  FeatureVector,ErrorMsg;

#ifndef MAP_USE_FIX_REGION
	H_MapImgs(Image, m_mapImg, &MappedImg, &ErrorMsg);		

	H_GetFeatureVector(MappedImg, m_ProjROI, &FeatureVector);
#else
	H_GetFeatureVector_FixReg(Image, m_MapRegInOrigImg, m_OriginPartitionROI_1, m_OriginPartitionROI_2,m_OriginPartitionUnionROI_1, m_OriginPartitionUnionROI_2, &FeatureVector);
#endif


	bool result = m_MatrixDataInstance.AddVector(FeatureVector);

	if(result)
	{	
		//更新到硬盘中
		string strMatrixPath = m_strTaskPath + "\\ReferMatrix.mtx";
		write_matrix(m_MatrixDataInstance.GetMatrix(), "binary", HTuple(strMatrixPath.c_str()));
		return S_OK;
	}
	else
		return S_FALSE;
#endif
}

STDMETHODIMP CCanAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
#ifndef MIX_LABEL_ALGO_NEW
	if(productID==3)
	{
#ifdef NDEBUG
		try
		{
#endif


			Hobject  MultiChannelImage_New,Image_e;
			HTuple   i, Sub_ModelIndex;
			Sub_ModelIndex = nIndex+1;
			gen_empty_obj(&MultiChannelImage_New);
			for (i=1; i<=m_modelNum-1; i+=1)
			{
				if (0 != (Sub_ModelIndex<=m_modelNum))
				{
					if (0 != (i<Sub_ModelIndex))
					{
						access_channel(m_multiChannelModelImage, &Image_e, i);
						append_channel(MultiChannelImage_New, Image_e, &MultiChannelImage_New);
					}
					else if (0 != (i>=Sub_ModelIndex))
					{
						access_channel(m_multiChannelModelImage, &Image_e, i+1);
						append_channel(MultiChannelImage_New, Image_e, &MultiChannelImage_New);

						m_models[i-1] = HTuple(m_models[i]);
					}
				} 
			}

			if((Sub_ModelIndex<=m_modelNum)&&(Sub_ModelIndex>0)&&(m_modelNum>1))
			{
				copy_image(MultiChannelImage_New,&m_multiChannelModelImage);
				m_modelNum=m_modelNum-1;
				//保存新模板组
				HTuple ImageFolder, ModelFolder, ImageFiles, ModelFiles;
				HTuple Index,IndexM;
				ImageFolder = (m_strTaskPath+ "\\TemplateImage").c_str() ;
				ModelFolder = (m_strTaskPath+ "\\Models").c_str();
				//first delete the exist templates
				list_files(ImageFolder, (HTuple("files").Append("follow_links")), &ImageFiles);
				tuple_regexp_select(ImageFiles, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ImageFiles);
				HTuple Length_pre  = ImageFiles.Num();
				if (Length_pre > 0 )
				{
					for (Index=0; Index <= (Length_pre -1); Index+=1)
						delete_file(HTuple(ImageFiles[Index]));
				}

				list_files(ModelFolder, (HTuple("files").Append("follow_links")), &ModelFiles);
				tuple_regexp_select(ModelFiles, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ModelFiles);
				HTuple Length_Model  = ModelFiles.Num();
				if (Length_Model > 0 )
				{
					for (Index=0; Index <= (Length_Model -1); Index+=1)
						delete_file(HTuple(ModelFiles[Index]));
				}
				//
				for (IndexM=0;IndexM<m_modelNum;IndexM+=1)
				{
					write_shape_model(m_models[IndexM], ModelFolder+"\\"+ HTuple(IndexM.ToString(".03")+".shm"));
					access_channel(m_multiChannelModelImage, &Image_e, IndexM+1);
					write_image(Image_e,"tiff",0, ImageFolder+ "\\"+HTuple(IndexM.ToString(".03")+".tiff"));
				}
			}
			else
			{
				return S_FALSE;
			}
#ifdef NDEBUG
		}
		catch (HException &except) 
		{
			m_algoSpy.QueueUpLogMsg(except, m_crtImg);
			return S_FALSE;
		}
#endif
	}
	return S_OK;

#else

	if(productID==3)
	{
#ifdef NDEBUG
		try
		{
#endif
			HTuple   Sub_ModelIndex = nIndex;

			bool result = m_MatrixDataInstance.RemoveVect(Sub_ModelIndex);

			if (result)
			{	
				string strMatrixPath = m_strTaskPath + "\\ReferMatrix.mtx";

				string strMatrixOriginPath = m_strTaskPath + "\\ReferMatrixOrigin.mtx";


				write_matrix(m_MatrixDataInstance.GetMatrix(), "binary", HTuple(strMatrixOriginPath.c_str()));
				write_matrix(m_MatrixDataInstance.GetCroppedMatrix(), "binary", HTuple(strMatrixPath.c_str()));

				return S_OK; 
			}
			else
			{	
				return S_FALSE;
			}
#ifdef NDEBUG
		}
		catch (HException &except) 
		{
			m_algoSpy.QueueUpLogMsg(except, m_crtImg);
			return S_FALSE;
		}
#endif
	}
	return S_OK;
#endif

}

STDMETHODIMP CCanAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{
	return S_OK;
}


STDMETHODIMP CCanAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{
#ifdef NDEBUG
	try
	{
#endif
		//自动提取区域测试/////////////////////////////
		CString strExtractPara=*strRegionExtractPara;
		std::wstring wstrPath = CT2W(strExtractPara);
		std::string strPara = CW2A(wstrPath.c_str());
		std::vector<string> vPara = SplitString(strPara, ",");
		if(vPara.empty())
		{
			return S_FALSE;
		}
		size_t paraNum = vPara.size();

#ifndef  MIX_LABEL_ALGO_NEW
		if(paraNum!=7)
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("自学习参数个数与设定值不一致。"),_T("参数数量错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("The number of self-learning parameters is different from setting value."),_T("The number of parameters is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}			
			return S_FALSE;
		}
		if((abs(m_modelRegionHeight-(int)atof(vPara[0].c_str()))>=0.5)||(abs(m_modelMinScore-(int)atof(vPara[1].c_str()))>=0.5)||(abs(m_modelAnge-(int)atof(vPara[2].c_str()))>=0.5)||(abs(m_contrast-(int)atof(vPara[3].c_str()))>=0.5)||(abs(m_minContrast-(int)atof(vPara[4].c_str()))>=0.5)||(abs(m_readChessImage-(int)atof(vPara[5].c_str()))>=0.5)||(abs(m_chessNum-(int)atof(vPara[6].c_str()))>=0.5))//如果有参数变更，模板初始化，重新开始训练	//重新建模
		{
			m_modelNum=0;
			m_nImgIndex=0;
		}
		m_modelRegionHeight=(int)atof(vPara[0].c_str());
		m_modelMinScore=(int)atof(vPara[1].c_str());
		m_modelAnge=(int)atof(vPara[2].c_str());
		m_contrast=(int)atof(vPara[3].c_str());
		m_minContrast=(int)atof(vPara[4].c_str());
		m_readChessImage=(int)atof(vPara[5].c_str());
		m_chessNum=(int)atof(vPara[6].c_str());
		//
		m_modelParaOfSelfLearning.ReplaceElements(0,m_contrast);
		m_modelParaOfSelfLearning.ReplaceElements(1,m_minContrast);
		//
#else
		//待添加
		//更新数据
		if(paraNum!=6)
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("自学习参数个数与设定值不一致。"),_T("参数数量错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("The number of self-learning parameters is different from setting value."),_T("The number of parameters is wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}

			return S_FALSE;
		}

		m_LeftCameraStartRowPercent = (int)atof(vPara[0].c_str());
		m_LeftCameraEndRowPercent = (int)atof(vPara[1].c_str());
		m_RightCameraStartRowPercent = (int)atof(vPara[2].c_str());
		m_RightCameraEndRowPercent = (int)atof(vPara[3].c_str());
		m_VecDiffThreshold = atof(vPara[4].c_str());
		m_readChessImage=(int)atof(vPara[5].c_str());

		CheckParamValid(m_LeftCameraStartRowPercent);
		CheckParamValid(m_LeftCameraEndRowPercent);
		if (m_LeftCameraStartRowPercent >= m_LeftCameraEndRowPercent)
		{
			m_LeftCameraStartRowPercent = m_LeftCameraEndRowPercent - 5;
		}


		CheckParamValid(m_RightCameraStartRowPercent);
		CheckParamValid(m_RightCameraEndRowPercent);
		if (m_RightCameraStartRowPercent >= m_RightCameraEndRowPercent)
		{
			m_RightCameraStartRowPercent = m_RightCameraEndRowPercent -5;
		}

		m_StartRowPercents[0] = m_LeftCameraStartRowPercent;
		m_StartRowPercents[1] = m_RightCameraStartRowPercent;

		m_EndRowPercents[0] = m_LeftCameraEndRowPercent;
		m_EndRowPercents[1] = m_RightCameraEndRowPercent;

		if (m_StartRowPercents_Last != m_StartRowPercents || m_EndRowPercents_Last != m_EndRowPercents)
		{
			m_StartRowPercents_Last = m_StartRowPercents;
			m_EndRowPercents_Last = m_EndRowPercents;
			m_bIsRegParamChanged = true;
		}
		else
			m_bIsRegParamChanged = false;		
#endif



#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{
	if(productID==3)
	{
#ifdef NDEBUG
		try
		{
#endif
			if(m_modelNum<1)
				return S_FALSE;
			//ResetSelfLearningRegion(&m_strRegionExtractPara);
#ifdef NDEBUG
		}

		catch (HException &except) 
		{
			m_algoSpy.QueueUpLogMsg(except, m_crtImg);
			return S_FALSE;
		}
#endif

		Hobject* pObj = (Hobject*)multiModelImag;
		*pObj = m_multiChannelModelImageRectified;
		*cMax = (long)(m_modelNum[0].I());//m_vImgScore.size();
		for(size_t t = 0; t <m_modelNum[0].I(); t++)
		{
#ifndef  MIX_LABEL_ALGO_NEW
			vImgScore[t] = 85;
#else
			vImgScore[t] = 0; //训练阶段的分数值由于加入自身，差异值均为0
#endif			
		}

#ifdef  MIX_LABEL_ALGO_NEW
		string strMatrixOriginPath;
		strMatrixOriginPath = m_strTaskPath + "\\ReferMatrixOrigin.mtx";
		write_matrix(m_MatrixDataInstance.GetMatrix(), "binary", HTuple(strMatrixOriginPath.c_str()));

		//根据初始化的参数，裁剪生成Map, ReferMatrix, ModellingReg, OriginPartitionROI_1, OriginPartitionROI_2
		Hobject RegInOrigImg,RegInOrigImgTemp, m_mapImgTemp;
		HTuple  ObjNum;
		Hobject SelectedMapImg;

		count_obj(m_mapImgOrigin, &ObjNum);
		gen_empty_obj(&m_mapImg);
		gen_empty_obj(&RegInOrigImg);
		for (int ObjIndex = 1; ObjIndex <= ObjNum; ObjIndex++)
		{
			select_obj(m_mapImgOrigin, &SelectedMapImg, ObjIndex);
			H_CropMap(SelectedMapImg, &RegInOrigImgTemp ,m_StartRowPercents[ObjIndex-1], m_EndRowPercents[ObjIndex-1], &m_mapImgTemp);

			concat_obj(RegInOrigImg, RegInOrigImgTemp, &RegInOrigImg);
			concat_obj(m_mapImg, m_mapImgTemp, &m_mapImg);
		}

		string strModelRegionPath;
		strModelRegionPath=m_strTaskPath+ "\\Modelling.reg";
		write_region(RegInOrigImg, HTuple(strModelRegionPath.c_str()));
		m_MapRegInOrigImg = RegInOrigImg;

		string strMapPath, strMatrixPath;
		strMapPath=m_strTaskPath+"\\Map.tiff";		
		strMatrixPath = m_strTaskPath + "\\ReferMatrix.mtx";

		write_image(m_mapImg, "tiff", 0, HTuple(strMapPath.c_str()));

#ifdef MAP_USE_FIX_REGION
		string strOriginPartitionROI_1=m_strTaskPath+"\\OriginPartitionROI_1.reg";
		string strOriginPartitionROI_2=m_strTaskPath+"\\OriginPartitionROI_2.reg";

		H_GetMultiMapRegInOrigImg(m_mapImg, &m_OriginPartitionROI_1, &m_OriginPartitionROI_2);

		write_region(m_OriginPartitionROI_1, HTuple(strOriginPartitionROI_1.c_str()));
		write_region(m_OriginPartitionROI_2, HTuple(strOriginPartitionROI_2.c_str()));					
#endif

		m_MatrixDataInstance.CropMatrixBasePos(m_ReferMappedImg, m_StartRowPercents, m_EndRowPercents, &m_ReferMatrix);
		write_matrix(m_MatrixDataInstance.GetCroppedMatrix(), "binary", HTuple(strMatrixPath.c_str()));

		//保存默认的映射
		// 		string strMapPath=m_strTaskPath+"\\Map.tiff";;
		// 		write_image(m_mapImgOrigin, "tiff", 0, HTuple(strMapPath.c_str()));
		// 
		// 		string strMatrixPath = m_strTaskPath + "\\ReferMatrix.mtx";
		// 		write_matrix(m_MatrixDataInstance.GetMatrix(), "binary", HTuple(strMatrixPath.c_str()));
#endif
	}
	return S_OK;
}

STDMETHODIMP CCanAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CCanAlgo::ReloadExternalFile(BSTR* bstrPath)
{

	return S_OK;
}
//
STDMETHODIMP CCanAlgo::LocationAlgo(VARIANT* rgnId)
{
	//假的定位算子
	return S_OK;
}
STDMETHODIMP CCanAlgo::LocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("621$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	int MinScore = (int)pMinScore->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, hv_Area1, hv_Row3, hv_Column3;
	Hobject  ho_ModelImages1, ho_ModelRegions1,ho_RegionDilation1,ho_ConnectedRegions,ho_RegionFillUp,ho_RegionErosion,ho_SelectedRegions,ho_RegionUnion;
	Hobject  Ellipse, ImageReduced,Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);

		if (Radius<10)
		{
			gen_circle (&Circle,  Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			m_vRgn[id] = Circle;
			return S_OK;
		}
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.2, Height/1.6);   //默认图片的大小有要求
		double Pt_X, Pt_Y;
		BOOL Result = H_LocateCircleUseProfile(Image, Ellipse, &Circle, Radius, MinScore,  &Pt_X, &Pt_Y);
		m_centerLocationRegion=Circle;
		HTuple x,y;
		smallest_circle(Circle,&x,&y,&m_locationRadius);
		m_vPos[0].m_x = (float)Pt_X;
		m_vPos[0].m_y = (float)Pt_Y;
		m_vRgn[id] = Circle;

		if (!Result)
		{
			return S_FALSE;
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("621$\
					 381;B$\
					 374;LP;6;2000;1;220$\
					 390;LP;1;255;1;40");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CenterLocationAlgoMultImg(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
	//用于颠倒罐检测，兼容多对象图片
	return S_OK;

	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	int MinScore = (int)pMinScore->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, hv_Area1, hv_Row3, hv_Column3;
	Hobject  ho_ModelImages1, ho_ModelRegions1,ho_RegionDilation1,ho_ConnectedRegions,ho_RegionFillUp,ho_RegionErosion,ho_SelectedRegions,ho_RegionUnion;
	Hobject  Ellipse, ImageReduced,Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);

		if (Radius<10)
		{
			gen_circle (&Circle,  Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			m_vRgn[id] = Circle;
			return S_OK;
		}
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.2, Height/1.6);   //默认图片的大小有要求
		double Pt_X, Pt_Y;
		BOOL Result = H_LocateCircleUseProfile(Image, Ellipse, &Circle, Radius, MinScore,  &Pt_X, &Pt_Y);
		m_centerLocationRegion=Circle;
		HTuple x,y;
		smallest_circle(Circle,&x,&y,&m_locationRadius);
		m_vPos[0].m_x = (float)Pt_X;
		m_vPos[0].m_y = (float)Pt_Y;
		m_vRgn[id] = Circle;

		if (!Result)
		{
			return S_FALSE;
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CenterLocationAlgoMultImgHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("621$\
					 381;B$\
					 374;LP;6;2000;1;220$\
					 390;LP;1;255;1;40");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::ModelCreate(double Radius, HTuple * ModelID1)
{
	//产生一个标准圆创建一个形状模板
	if (Radius <= 0) 
	{
		return S_FALSE;
	}
	else
	{
		Hobject ContCircle, EmptyImage, SyntheticModelImage;
		HTuple  SizeSynthImage = 2* Radius + 10;

		gen_circle_contour_xld(&ContCircle, SizeSynthImage/2, SizeSynthImage/2, Radius, 0, 6.28318, "positive", 1);
		gen_image_const(&EmptyImage, "byte", SizeSynthImage, SizeSynthImage);
		paint_xld(ContCircle, EmptyImage, &SyntheticModelImage, 128);
		create_scaled_shape_model(SyntheticModelImage, "auto", 0, 0, 0, 0.95, 1.05, "auto","auto", "use_polarity", 40, "auto", ModelID1);
		return S_OK;
	}
}
STDMETHODIMP CCanAlgo::CenterLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用形状模板来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	float MinScore = (float)pMinScore->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;

	double Row, Column, Radius1;
	Hlong Width, Height;
	HTuple CenterRow, CenterColumn, Angle, Scale, Score, CenterRadius, MovementOfObject, MoveAndScaleOfObject;
	Hobject  ContCircle, EmptyImage, SyntheticModelImage;
	Hobject  ShapeModelImage, ShapeModelRegion, ShapeModel, SearchImage;
	Hobject  Ellipse, ImageReduced, ModelAtNewPosition, Region, Circle1,Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		//
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/5.0, Height/5.0);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		if(m_isFirst)
		{
			ModelCreate (Radius, &m_modelId);
			m_isFirst = false;
		}
		if(m_oldRadius != Radius)
		{
			clear_shape_model(m_modelId);
			ModelCreate (Radius, &m_modelId);
			m_oldRadius = Radius;
		}
		get_shape_model_contours (&ShapeModel, m_modelId, 1);
		find_scaled_shape_model (ImageReduced, m_modelId, 0, 0, 0.95, 1.05, MinScore, 1, 0.5, "least_squares", (HTuple(5).Append(-2)), 0.7, &CenterRow, &CenterColumn, &Angle, &Scale, &Score);
		if (0 != ((Score.Num())>0))
		{
			vector_angle_to_rigid(0, 0, 0, CenterRow[0].D(), CenterColumn[0].D(), Angle[0].D(), &MovementOfObject);
			hom_mat2d_scale(MovementOfObject, Scale[0].D(), Scale[0].D(),CenterRow[0].D(), CenterColumn[0].D(), &MoveAndScaleOfObject);
			affine_trans_contour_xld(ShapeModel, &ModelAtNewPosition, MoveAndScaleOfObject);
			gen_region_contour_xld(ModelAtNewPosition, &Region, "filled");
			smallest_circle(Region, &Row, &Column, &Radius1);
			m_locationRadius=Radius1;
			Hobject RegionDifference;
			gen_circle(&Circle, Row, Column, Radius1);
			gen_circle(&Circle1, Row, Column, Radius);
			m_vPos[0].m_y = (float)Row;
			m_vPos[0].m_x = (float)Column;
			if(Circle.Id() != H_EMPTY_REGION) 
			{
				if(Radius1 >= Radius)
				{
					if(Radius1-Radius < 1)
					{
						gen_circle(&Circle, Row, Column, Radius1+1);
					}
					else
					{
						gen_circle(&Circle, Row, Column, Radius1);
					}
					difference (Circle, Circle1, &RegionDifference);
				}
				else
				{
					if(Radius - Radius1 < 1)
					{
						gen_circle(&Circle, Row, Column, Radius1-1);
					}
					else
					{
						gen_circle(&Circle, Row, Column, Radius1);
					}
					difference (Circle1, Circle, &RegionDifference);
				}
				m_vRgn[id] = RegionDifference;
			}
		}
		else
		{
			gen_circle(&Circle1, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle1;
			return S_FALSE;
		}
		m_centerLocationRegion=Circle;
#ifdef NDEBUG
	}

	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CenterLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 374;LP;10;2000;1;192$\
					 377;FP;0.5;1.0;0.05;0.85");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CenterLocationAlgoHS(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, CenterRadius, hv_Row3, hv_Column3;
	Hobject threshed,RegionFillUp,RegionUnion,RegionOpening;
	Hobject  Ellipse, ImageReduced,Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.2, Height/1.6);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		threshold(ImageReduced, &threshed,ThresholdLow, 255);
		fill_up(threshed, &RegionFillUp);
		union1(RegionFillUp, &RegionUnion);
		opening_circle(RegionUnion, &RegionOpening,OutlierRemoverSize+0.5);
		get_obj_class(RegionOpening, &hv_Class);

		if (0 != (hv_Class==HTuple("region")))
		{	
			smallest_circle(RegionOpening, &hv_Row3, &hv_Column3, &CenterRadius);

			m_centerLocationRegion=RegionOpening;
			m_locationRadius=CenterRadius;
			m_CenterRadius=CenterRadius;//尺寸检测
			if((CenterRadius<Radius*1.3)&(CenterRadius>Radius*0.7))
			{
				gen_circle(&Circle, hv_Row3, hv_Column3, CenterRadius);
				m_vPos[0].m_y = (float)hv_Row3[0].D();
				m_vPos[0].m_x = (float)hv_Column3[0].D();
				m_vRgn[id] = Circle;
			}
			else
			{
				gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
				m_vPos[0].m_x = 0.0;
				m_vPos[0].m_y = 0.0;
				m_vRgn[id] = Circle;
				m_centerLocationRegion=m_vRgn[id];
				return S_FALSE;
			}
		}
		else
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle;
			m_centerLocationRegion=m_vRgn[id];
			return S_FALSE;
		}

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CenterLocationAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("639$\
					 381;B$\
					 374;LP;10;2000;1;200$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	m_ringPara[id].m_Radius = (float)pRadius->fltVal;
	m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float Radius =(float)pRadius->fltVal;
	float m_RadiusExt = Radius - RoiWidth;

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	m_sectorAngle[id].m_angleStartAbs=0;
	m_sectorAngle[id].m_angleEndAbs=360;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  Circle1_Inner,Circle2_EXT;
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		if (RoiWidth<0)
		{
			gen_circle(&Circle2_EXT, fCenterY, fCenterX, m_RadiusExt);
			gen_circle(&Circle1_Inner, fCenterY, fCenterX, Radius);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		}
		else if (RoiWidth==0)
		{
			gen_circle(&Circle2_EXT, fCenterY, fCenterX, Radius+1);
			gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusExt);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		}
		else
		{
			if(m_RadiusExt>0)
			{
				gen_circle(&Circle2_EXT, fCenterY, fCenterX, Radius);
				gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusExt);
				difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
			}
			else
			{
				gen_circle(&RegionDetect, fCenterY, fCenterX, Radius);
			}
		}
		m_vRgn[id] = RegionDetect ;	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::CircleRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	float LocationRadius=(float)(m_locationRadius[0].I());
	float Radius =LocationRadius - RadiusSize;
	float m_RadiusExt =LocationRadius - RadiusSize-RoiWidth;

	m_sectorAngle[id].m_angleStartAbs=0.0;
	m_sectorAngle[id].m_angleEndAbs=360.0;

	if (Radius>0)
	{

		m_ringPara[id].m_Radius = LocationRadius - RadiusSize;
		m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	}
	else
	{

		m_ringPara[id].m_Radius =(float)pRoiWidth->fltVal+2;
		m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	}

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  Circle1_Inner,Circle2_EXT;
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		if( m_RadiusExt>0 && Radius>0)
		{
			gen_circle(&Circle2_EXT, fCenterY, fCenterX, Radius);
			gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusExt);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
			m_vRgn[id] = RegionDetect ;	
		}
		else if(m_RadiusExt<0 && Radius>0)
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, Radius);
			m_vRgn[id] = RegionDetect ;	
		}
		else
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, 40);
			m_vRgn[id] = RegionDetect ;
			return S_FALSE;
		}

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CircleRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("8001$\
					 167;B$\
					 693;FP;-1500;1500;0.5;10$\
					 413;FP;1;2000;0.5;208"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CenterRelocationAlgo(VARIANT* rgnId, VARIANT* pRadiusSet, VARIANT* pRoiWidth, VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	//
	float RoiWidth = (float)(pRoiWidth->fltVal);
	float RadiusExt =(float)pRadiusSet->fltVal;
	if(RoiWidth<=2)
	{ 
		RoiWidth=2.0;
	}
	if(RadiusExt<=20)
	{
		RadiusExt=20;
	}	
	float RadiusInner = RadiusExt - RoiWidth;
	//
	double Radius = (double)pRadius->fltVal;
	int MinScore = (int)pMinScore->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, hv_Area1, hv_Row3, hv_Column3;
	Hobject  ho_ModelImages1, ho_ModelRegions1,ho_RegionDilation1,ho_ConnectedRegions,ho_RegionFillUp,ho_RegionErosion,ho_SelectedRegions,ho_RegionUnion;
	Hobject  Ellipse, ImageReduced,Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
		if (Radius<10)
		{
			gen_circle (&Circle,  Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			m_vRgn[id] = Circle;
			return S_OK;
		}

		if(RadiusInner>=1)
		{
			Hobject Circle2_EXT,Circle1_Inner;
			gen_circle(&Circle2_EXT, Height/2.0, Width/2.0, RadiusExt);
			gen_circle(&Circle1_Inner,Height/2.0, Width/2.0, RadiusInner);
			difference(Circle2_EXT,Circle1_Inner,&Ellipse);
		}
		else
		{
			gen_circle(&Ellipse, Height/2.0, Width/2.0, RadiusExt);
		}
		Hobject RegionShow;
		boundary(Ellipse, &RegionShow, "outer");
		//gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.2, Height/1.6);   //默认图片的大小有要求
		double Pt_X, Pt_Y;
		BOOL Result = H_LocateCircleUseProfile(Image, Ellipse, &Circle, Radius, MinScore,  &Pt_X, &Pt_Y);
		m_centerLocationRegion=Circle;
		HTuple x,y;
		smallest_circle(Circle,&x,&y,&m_locationRadius);
		m_vPos[0].m_x = (float)Pt_X;
		m_vPos[0].m_y = (float)Pt_Y;
		m_vRgn[id] = Circle;
		union2(m_vRgn[id],RegionShow,&m_vRgn[id]);
		if (!Result)
		{
			return S_FALSE;
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CenterRelocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("621$\
					 381;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 374;LP;6;2000;1;220$\
					 390;LP;1;255;1;40");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CenterRelocationAlgoHS(VARIANT* rgnId,  VARIANT* pRadiusLong,  VARIANT* pRadiusShort, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	double RadiusLong= (double)pRadiusLong->fltVal;
	double RadiusShort= (double)pRadiusShort->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, CenterRadius, hv_Row3, hv_Column3;
	Hobject threshed,RegionFillUp,RegionUnion,RegionOpening;
	Hobject  Ellipse, ImageReduced,Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, RadiusLong,RadiusShort);   //默认图片的大小有要求
		Hobject RegionShow;
		boundary(Ellipse, &RegionShow, "outer");
		reduce_domain (Image, Ellipse, &ImageReduced);
		threshold(ImageReduced, &threshed,ThresholdLow, 255);
		//
		Hobject  ConnectedRegions, SelectedRegions,RegionDifference;
		connection(threshed, &ConnectedRegions);
		fill_up(ConnectedRegions, &RegionFillUp);
		Hobject SelectedRegions2;
		select_shape (RegionFillUp, &SelectedRegions2, "area", "and", 500, 999999999);
		opening_circle(SelectedRegions2, &RegionOpening,OutlierRemoverSize+0.5);
		//设置区域最大外半径
		double MinRadiusSet,MaxRadiusSet,EllipseRadiusShort;
		EllipseRadiusShort=min(RadiusLong,RadiusShort);
		MaxRadiusSet=min(Radius*1.15,EllipseRadiusShort);
		MinRadiusSet= Radius*0.8;
		if (MinRadiusSet>=MaxRadiusSet)
		{
			MinRadiusSet=MaxRadiusSet-1;
		}
		select_shape(RegionOpening, &SelectedRegions, "outer_radius", "and",MinRadiusSet,MaxRadiusSet);
		HTuple  Row, Column, RadiusFit;
		smallest_circle(SelectedRegions, &Row, &Column, &RadiusFit);
		if (0 != ((Row.Num())>=2))
		{
			gen_circle(&Circle, Row, Column, RadiusFit);
			difference(Circle, SelectedRegions, &RegionDifference);
			HTuple Row1, Column1, Radius1, Indices;
			inner_circle(RegionDifference, &Row1, &Column1, &Radius1);
			tuple_sort_index(Radius1, &Indices);
			Hobject RegionCenter;
			select_obj(SelectedRegions,&RegionCenter,Indices.Select(0)+1);
			//
			CenterRadius = RadiusFit.Select(Indices.Select(0));	
			hv_Row3=Row.Select(Indices.Select(0));	
			hv_Column3=Column.Select(Indices.Select(0));	
			gen_circle(&Circle, hv_Row3, hv_Column3, Radius);
			m_locationRadius=CenterRadius;
			m_vPos[0].m_y = (float)hv_Row3[0].D();
			m_vPos[0].m_x = (float)hv_Column3[0].D();
			m_vRgn[id] = Circle;
			//
			m_centerLocationRegion=RegionCenter;
			union2(m_vRgn[id],RegionShow,&m_vRgn[id]);
		}
		else if (0 != ((Row.Num())==1))
		{
			CenterRadius= RadiusFit.Select(0);	
			hv_Row3=Row.Select(0);
			hv_Column3=Column.Select(0);
			gen_circle(&Circle, hv_Row3, hv_Column3, Radius);
			m_locationRadius=CenterRadius;
			m_vPos[0].m_y = (float)hv_Row3[0].D();
			m_vPos[0].m_x = (float)hv_Column3[0].D();
			m_vRgn[id] = Circle;
			//
			m_centerLocationRegion=SelectedRegions;
			union2(m_vRgn[id],RegionShow,&m_vRgn[id]);
		}
		else
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_locationRadius=Radius;
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle;
			m_centerLocationRegion=m_vRgn[id];
			union2(m_vRgn[id],RegionShow,&m_vRgn[id]);
			return S_FALSE;
		}
		if( Height-m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_x<m_locationRadius||Width-m_vPos[0].m_x<m_locationRadius)
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle;
			m_centerLocationRegion=m_vRgn[id];
			return S_FALSE;
		}

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CenterRelocationAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("685$\
					 381;B$\
					 665;LP;30;1200;1;200$\
					 666;LP;10;1000;1;100$\
					 374;LP;10;2000;1;200$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo:: CenterLocationAlgoUsePointFitting(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold)
{
	int id = (int)rgnID->fltVal;
	int SearchRegRow = (int)pSearchRegRow->fltVal;
	int SearchRegCol = (int)pSearchRegCol->fltVal;
	int OuterRadius = (int)pOuterRadius->fltVal;
	int RoiWidth = (int)pRoiWidth->fltVal;
	int ScanLineNum = (int)pScanLineNum->fltVal;
	int ScanLineDirection = (int)pScanLineDirection->fltVal;
	int Transition = (int)pTransition->fltVal;
	int PointSelection = (int)pPointSelection->fltVal;
	double Sigma = (double)pSigma->fltVal;
	double ThresholdVal = (double)pThreshold->fltVal;

	Hobject newRgn;
	gen_empty_region(&newRgn);
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	Hobject  Ellipse, ImageReduced,Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		m_ImgIndex++;

		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
		Hobject OuterCircle, InnerCircle, Donut, CenterPtCross;


		gen_circle(&OuterCircle,SearchRegRow, SearchRegCol, OuterRadius);			

		int InnerRadius = OuterRadius - RoiWidth;
		if (InnerRadius <= 0)
		{
			InnerRadius = 0;
			concat_obj(m_vRgn[id], OuterCircle, &m_vRgn[id]);
		}
		else
		{
			gen_circle(&InnerCircle, SearchRegRow, SearchRegCol, InnerRadius);
			difference(OuterCircle, InnerCircle, &Donut);
			concat_obj(m_vRgn[id], Donut, &m_vRgn[id]);
		}

		if (m_bDebugImg)
		{
			gen_cross_contour_xld(&CenterPtCross, SearchRegRow, SearchRegCol, 16, HTuple(45).Rad());				
			concat_obj(m_vRgn[id], CenterPtCross, &m_vRgn[id]);
		}

		if (m_bPilotImg)
		{
			m_oldSearchRegRow = SearchRegRow;
			m_oldSearchRegCol = SearchRegCol;
			m_oldOuterRadius = OuterRadius;
			m_oldRoiWidth = RoiWidth;
			m_oldScanLineNum = ScanLineNum;
			m_oldScanLineDirection = ScanLineDirection;

			m_bIsRegParamChanged = true;
			return S_OK;
		}

		if (m_oldSearchRegRow != SearchRegRow || m_oldSearchRegCol != SearchRegCol || m_oldOuterRadius != OuterRadius   ||  m_oldRoiWidth != RoiWidth	||	m_oldScanLineNum != ScanLineNum	  || m_oldScanLineDirection != ScanLineDirection)
		{
			m_bIsRegParamChanged = true;

			m_oldSearchRegRow = SearchRegRow;
			m_oldSearchRegCol = SearchRegCol;
			m_oldOuterRadius = OuterRadius;
			m_oldRoiWidth = RoiWidth;
			m_oldScanLineNum = ScanLineNum;
			m_oldScanLineDirection = ScanLineDirection;
		}



		if ( m_bIsRegParamChanged || (m_bDebugImg && m_bIsRegParamChanged))
		{
			m_measureHandles[id].CreateHandles(Width, Height, SearchRegRow, SearchRegCol, OuterRadius, RoiWidth, ScanLineNum, ScanLineDirection);

			m_bIsRegParamChanged = false;
		}

		//bool result = m_measureHandles[id].LocateEdgePoints(Image, Transition, PointSelection, Sigma, ThresholdVal);
		bool result = m_measureHandles[id].LocateEdgePoints1(Image,SearchRegRow, SearchRegCol,Transition, PointSelection, Sigma, ThresholdVal);

		HTuple CenterPtRow, CenterPtCol, CenterRadius;
		Hobject EdgePointCross,Contour,CenterEdgeRegion;
		if (result)
		{
			m_measureHandles[id].FitCircleContour(m_measureHandles[id].EdgePtRows, m_measureHandles[id].EdgePtCols, &CenterPtRow, &CenterPtCol, &CenterRadius,&Contour);

			Hobject CircleFitted;
			gen_circle(&CircleFitted, CenterPtRow, CenterPtCol, CenterRadius);
			concat_obj(m_vRgn[id], CircleFitted, &m_vRgn[id]);
			//输出中心定位的边缘轮廓
			gen_region_contour_xld(Contour,&CenterEdgeRegion,"margin");
			Hobject RegionUnion,RegionOpening;
			fill_up(CenterEdgeRegion, &RegionUnion);
			opening_circle(RegionUnion, &RegionOpening,CenterRadius/2+0.5);
			m_centerLocationRegion=RegionOpening;
			m_locationRadius = CenterRadius;
			m_CenterRadius=CenterRadius;
			if (m_bDebugImg)
			{
				gen_cross_contour_xld(&EdgePointCross, m_measureHandles[id].EdgePtRows, m_measureHandles[id].EdgePtCols, 16, HTuple(45).Rad());				
				concat_obj(m_vRgn[id], EdgePointCross, &m_vRgn[id]);

				Hobject FitCircleCenterCross;
				gen_cross_contour_xld(&FitCircleCenterCross, CenterPtRow, CenterPtCol, 16, 0);
				concat_obj(m_vRgn[id], FitCircleCenterCross, &m_vRgn[id]);
			}

			m_vPos[0].m_x = (float)CenterPtCol[0].D();
			m_vPos[0].m_y = (float)CenterPtRow[0].D();
		} 
		else
		{
			if (m_vPos[0].m_x <= 0 )
			{
				//m_vPos[0].m_x = 0.0;
				//m_vPos[0].m_y = 0.0;

				m_vPos[0].m_x = (float)(Width/2.0);
				m_vPos[0].m_y = (float)(Height/2.0);
				m_centerLocationRegion=newRgn;
			} 			

			return S_FALSE;
		}




#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo:: CenterLocationAlgoUsePointFittingHelp(BSTR* pHelpStr)
{
	//字符串待修改！！
	CComBSTR strHelp("1297$\
					 381;B$\
					 1298;LP;-600;1600;1;247$\
					 1299;LP;-600;1200;1;330$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 1300;LP;10;100;1;75$\
					 1301;LP;0;1;1;1$\
					 1302;LP;0;2;1;1$\
					 1303;LP;0;2;1;1$\
					 680;FP;0.4;10.0;0.05;1.0$\
					 1304;FP;0.1;100.0;0.1;10.0");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

/***********************************************************************
** 函数名：CenterLocationAlgoUsePointFitting1
** 说  明：在原来算子基础上加强了对离群点的筛除
** 时  间：2019年08月14日
************************************************************************/
STDMETHODIMP CCanAlgo:: CenterLocationAlgoUsePointFitting1(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold, VARIANT* pDistThresh)
{
	int id = (int)rgnID->fltVal;
	int SearchRegRow = (int)pSearchRegRow->fltVal;
	int SearchRegCol = (int)pSearchRegCol->fltVal;
	int OuterRadius = (int)pOuterRadius->fltVal;
	int RoiWidth = (int)pRoiWidth->fltVal;
	int ScanLineNum = (int)pScanLineNum->fltVal;
	int ScanLineDirection = (int)pScanLineDirection->fltVal;
	int Transition = (int)pTransition->fltVal;
	int PointSelection = (int)pPointSelection->fltVal;
	double Sigma = (double)pSigma->fltVal;
	double ThresholdVal = (double)pThreshold->fltVal;
	int  dDistThresh  = (int)pDistThresh->fltVal;             // 100

	Hobject newRgn;
	gen_empty_region(&newRgn);
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	Hobject  Ellipse, ImageReduced,Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		m_ImgIndex++;

		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
		Hobject OuterCircle, InnerCircle, Donut, CenterPtCross;

		gen_circle(&OuterCircle,SearchRegRow, SearchRegCol, OuterRadius);			

		int InnerRadius = OuterRadius - RoiWidth;
		if (InnerRadius <= 0)
		{
			InnerRadius = 0;
			concat_obj(m_vRgn[id], OuterCircle, &m_vRgn[id]);
		}
		else
		{
			gen_circle(&InnerCircle, SearchRegRow, SearchRegCol, InnerRadius);
			difference(OuterCircle, InnerCircle, &Donut);
			concat_obj(m_vRgn[id], Donut, &m_vRgn[id]);
		}

		if (m_bDebugImg)
		{
			gen_cross_contour_xld(&CenterPtCross, SearchRegRow, SearchRegCol, 16, HTuple(45).Rad());				
			concat_obj(m_vRgn[id], CenterPtCross, &m_vRgn[id]);
		}

		if (m_bPilotImg)
		{
			m_oldSearchRegRow = SearchRegRow;
			m_oldSearchRegCol = SearchRegCol;
			m_oldOuterRadius = OuterRadius;
			m_oldRoiWidth = RoiWidth;
			m_oldScanLineNum = ScanLineNum;
			m_oldScanLineDirection = ScanLineDirection;

			m_bIsRegParamChanged = true;
			return S_OK;
		}

		if (m_oldSearchRegRow != SearchRegRow || m_oldSearchRegCol != SearchRegCol || m_oldOuterRadius != OuterRadius   ||  m_oldRoiWidth != RoiWidth	||	m_oldScanLineNum != ScanLineNum	  || m_oldScanLineDirection != ScanLineDirection)
		{
			m_bIsRegParamChanged = true;

			m_oldSearchRegRow = SearchRegRow;
			m_oldSearchRegCol = SearchRegCol;
			m_oldOuterRadius = OuterRadius;
			m_oldRoiWidth = RoiWidth;
			m_oldScanLineNum = ScanLineNum;
			m_oldScanLineDirection = ScanLineDirection;
		}

		if ( m_bIsRegParamChanged || (m_bDebugImg && m_bIsRegParamChanged))
		{
			m_measureHandles[id].CreateHandles(Width, Height, SearchRegRow, SearchRegCol, OuterRadius, RoiWidth, ScanLineNum, ScanLineDirection);

			m_bIsRegParamChanged = false;
		}

		//bool result = m_measureHandles[id].LocateEdgePoints(Image, Transition, PointSelection, Sigma, ThresholdVal);
		bool result = m_measureHandles[id].LocateEdgePoints1(Image,SearchRegRow, SearchRegCol,Transition, PointSelection, Sigma, ThresholdVal);

		HTuple CenterPtRow, CenterPtCol, CenterRadius;
		Hobject EdgePointCross,Contour,CenterEdgeRegion;
		if (result)
		{
			m_measureHandles[id].FitCircleContour1(m_measureHandles[id].EdgePtRows, m_measureHandles[id].EdgePtCols,dDistThresh, &CenterPtRow, &CenterPtCol, &CenterRadius,&Contour);

			Hobject CircleFitted;
			gen_circle(&CircleFitted, CenterPtRow, CenterPtCol, CenterRadius);
			concat_obj(m_vRgn[id], CircleFitted, &m_vRgn[id]);
			//输出中心定位的边缘轮廓
			gen_region_contour_xld(Contour,&CenterEdgeRegion,"margin");
			Hobject RegionUnion,RegionOpening;
			fill_up(CenterEdgeRegion, &RegionUnion);
			opening_circle(RegionUnion, &RegionOpening,CenterRadius/2+0.5);
			m_centerLocationRegion=RegionOpening;
			m_locationRadius=CenterRadius;
			m_CenterRadius=CenterRadius;
			if (m_bDebugImg)
			{
				gen_cross_contour_xld(&EdgePointCross, m_measureHandles[id].EdgePtRows, m_measureHandles[id].EdgePtCols, 16, HTuple(45).Rad());				
				concat_obj(m_vRgn[id], EdgePointCross, &m_vRgn[id]);

				Hobject FitCircleCenterCross;
				gen_cross_contour_xld(&FitCircleCenterCross, CenterPtRow, CenterPtCol, 16, 0);
				concat_obj(m_vRgn[id], FitCircleCenterCross, &m_vRgn[id]);
			}

			m_vPos[0].m_x = (float)CenterPtCol[0].D();
			m_vPos[0].m_y = (float)CenterPtRow[0].D();
		} 
		else
		{
			if (m_vPos[0].m_x <= 0 )
			{
				//m_vPos[0].m_x = 0.0;
				//m_vPos[0].m_y = 0.0;

				m_vPos[0].m_x = (float)(Width/2.0);
				m_vPos[0].m_y = (float)(Height/2.0);
				m_centerLocationRegion=newRgn;
			} 			

			return S_FALSE;
		}

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo:: CenterLocationAlgoUsePointFitting1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("6119$\
					 381;B$\
					 1298;LP;-600;1600;1;247$\
					 1299;LP;-600;1200;1;330$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 1300;LP;10;100;1;75$\
					 1301;LP;0;1;1;1$\
					 1302;LP;0;2;1;1$\
					 1303;LP;0;2;1;1$\
					 680;FP;0.4;10.0;0.05;1.0$\
					 1304;FP;0.1;100.0;0.1;10.0$\
					 6118;LP;1;500;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo:: CenterLocationAlgoUsePointFittingColor(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pSigma, VARIANT* pThreshold, VARIANT* pColorChannel, VARIANT* pTransType)
{

	int id = (int)rgnID->fltVal;
	int SearchRegRow = (int)pSearchRegRow->fltVal;
	int SearchRegCol = (int)pSearchRegCol->fltVal;
	int OuterRadius = (int)pOuterRadius->fltVal;
	int RoiWidth = (int)pRoiWidth->fltVal;
	int ScanLineNum = (int)pScanLineNum->fltVal;
	int ScanLineDirection = (int)pScanLineDirection->fltVal;
	int Transition = (int)pTransition->fltVal;
	int TransType = (int) pTransType->fltVal;  
	double Sigma = (double)pSigma->fltVal;
	double ThresholdVal = (double)pThreshold->fltVal;
	int ColorChannel = (int)pColorChannel->fltVal;
	//int ColorTransType = 0;
	CComVariant retValue;
	Hobject newRgn,rgn;
	gen_empty_obj(&newRgn);
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	Hobject Image;
	Hobject  Ellipse, ImageReduced,Circle;
	Image = m_crtImgMult;
	get_image_size(Image, &Width, &Height); 
	gen_rectangle1(&rgn, 0, 0, Height, Width);
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		m_ImgIndex++;

		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn, TransType, ColorChannel))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		if (1)
		{
			m_vInterImgs[id] = Image;
		}
		else
		{
			Hobject EmptyObj;					
			m_vInterImgs[id] = EmptyObj;
		}
		reduce_domain (Image, rgn, &ImageReduced);


		Hobject OuterCircle, InnerCircle, Donut, CenterPtCross;

		gen_circle(&OuterCircle,SearchRegRow, SearchRegCol, OuterRadius);			

		int InnerRadius = OuterRadius - RoiWidth;
		if (InnerRadius <= 0)
		{
			InnerRadius = 0;
			concat_obj(m_vRgn[id], OuterCircle, &m_vRgn[id]);
		}
		else
		{
			gen_circle(&InnerCircle, SearchRegRow, SearchRegCol, InnerRadius);
			difference(OuterCircle, InnerCircle, &Donut);
			concat_obj(m_vRgn[id], Donut, &m_vRgn[id]);
		}

		if (m_bDebugImg)
		{
			gen_cross_contour_xld(&CenterPtCross, SearchRegRow, SearchRegCol, 16, HTuple(45).Rad());				
			concat_obj(m_vRgn[id], CenterPtCross, &m_vRgn[id]);
		}

		if (m_bPilotImg)
		{
			m_oldSearchRegRow = SearchRegRow;
			m_oldSearchRegCol = SearchRegCol;
			m_oldOuterRadius = OuterRadius;
			m_oldRoiWidth = RoiWidth;
			m_oldScanLineNum = ScanLineNum;
			m_oldScanLineDirection = ScanLineDirection;

			m_bIsRegParamChanged = true;
			return S_OK;
		}

		if (m_oldSearchRegRow != SearchRegRow || m_oldSearchRegCol != SearchRegCol || m_oldOuterRadius != OuterRadius   ||  m_oldRoiWidth != RoiWidth	||	m_oldScanLineNum != ScanLineNum	  || m_oldScanLineDirection != ScanLineDirection)
		{
			m_bIsRegParamChanged = true;

			m_oldSearchRegRow = SearchRegRow;
			m_oldSearchRegCol = SearchRegCol;
			m_oldOuterRadius = OuterRadius;
			m_oldRoiWidth = RoiWidth;
			m_oldScanLineNum = ScanLineNum;
			m_oldScanLineDirection = ScanLineDirection;
		}



		if ( m_bIsRegParamChanged || (m_bDebugImg && m_bIsRegParamChanged))
		{
			m_measureHandles[id].CreateHandles(Width, Height, SearchRegRow, SearchRegCol, OuterRadius, RoiWidth, ScanLineNum, ScanLineDirection);

			m_bIsRegParamChanged = false;
		}

		bool result = m_measureHandles[id].LocateEdgePoints(Image, Transition, 1, Sigma, ThresholdVal);


		HTuple CenterPtRow, CenterPtCol, CenterRadius;
		Hobject EdgePointCross,Contour,CenterEdgeRegion;
		if (result)
		{
			m_measureHandles[id].FitCircleContour(m_measureHandles[id].EdgePtRows, m_measureHandles[id].EdgePtCols, &CenterPtRow, &CenterPtCol, &CenterRadius,&Contour);

			Hobject CircleFitted;
			gen_circle(&CircleFitted, CenterPtRow, CenterPtCol, CenterRadius);
			concat_obj(m_vRgn[id], CircleFitted, &m_vRgn[id]);
			//输出中心定位的边缘轮廓
			gen_region_contour_xld(Contour,&CenterEdgeRegion,"margin");
			Hobject RegionUnion,RegionOpening;
			union1(CenterEdgeRegion, &RegionUnion);
			opening_circle(RegionUnion, &RegionOpening,CenterRadius/2+0.5);
			m_centerLocationRegion=RegionOpening;

			m_CenterRadius=CenterRadius;
			if (m_bDebugImg)
			{
				gen_cross_contour_xld(&EdgePointCross, m_measureHandles[id].EdgePtRows, m_measureHandles[id].EdgePtCols, 16, HTuple(45).Rad());				
				concat_obj(m_vRgn[id], EdgePointCross, &m_vRgn[id]);

				Hobject FitCircleCenterCross;
				gen_cross_contour_xld(&FitCircleCenterCross, CenterPtRow, CenterPtCol, 16, 0);
				concat_obj(m_vRgn[id], FitCircleCenterCross, &m_vRgn[id]);
			}


			m_vPos[0].m_x = (float)CenterPtCol[0].D();
			m_vPos[0].m_y = (float)CenterPtRow[0].D();
		} 
		else
		{
			if (m_vPos[0].m_x <= 0 )
			{
				//m_vPos[0].m_x = 0.0;
				//m_vPos[0].m_y = 0.0;

				m_vPos[0].m_x = (float)(Width/2.0);
				m_vPos[0].m_y = (float)(Height/2.0);
				m_centerLocationRegion=newRgn;
			} 			

			return S_FALSE;
		}




#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif

	return S_OK;
	return S_OK;
}
STDMETHODIMP CCanAlgo:: CenterLocationAlgoUsePointFittingColorHelp(BSTR* pHelpStr)
{
	// TODO: 在此添加实现代码
	CComBSTR strHelp("1334$\
					 381;B$\
					 1298;LP;-500;1600;1;247$\
					 1299;LP;-500;1200;1;330$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 1300;LP;10;100;1;75$\
					 1301;LP;0;1;1;1$\
					 1302;LP;0;1;1;1$\
					 680;FP;0.4;10.0;0.05;1.0$\
					 1304;FP;0.1;100.0;0.1;10.0$\
					 1335;LP;1;3;1;2$\
					 1336;LP;0;6;1;0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::ModelCreate2(double Radius,int UsePolarity,  HTuple * ModelID1)
{
	//产生一个标准圆创建一个形状模板
	if (Radius <= 0) 
	{
		return S_FALSE;
	}
	else
	{
		Hobject ContCircle, EmptyImage, SyntheticModelImage;
		HTuple  SizeSynthImage = 2* Radius + 10;

		gen_circle_contour_xld(&ContCircle, SizeSynthImage/2, SizeSynthImage/2, Radius, 0, 6.28318, "positive", 1);
		gen_image_const(&EmptyImage, "byte", SizeSynthImage, SizeSynthImage);
		paint_xld(ContCircle, EmptyImage, &SyntheticModelImage, 128);
		//
		if(UsePolarity)
		{
			invert_image(SyntheticModelImage,&SyntheticModelImage);
		}
		create_scaled_shape_model(SyntheticModelImage, "auto", 0, 0, 0, 0.95, 1.05, "auto","auto", "use_polarity", 40, "auto", ModelID1);
		//create_scaled_shape_model(SyntheticModelImage, "auto", 0, 0, 0, 0.9, 1.1, "auto","auto", "use_polarity", 40, "auto", ModelID1);
		return S_OK;
	}
}
STDMETHODIMP CCanAlgo::CenterReLocationAlgo2(VARIANT* rgnId,VARIANT* pUsePolarity, VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用形状模板来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	float MinScore = (float)pMinScore->fltVal;
	int  UsePolarity=(int)pUsePolarity->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;

	double Row, Column, Radius1;
	Hlong Width, Height;
	HTuple CenterRow, CenterColumn, Angle, Scale, Score, CenterRadius, MovementOfObject, MoveAndScaleOfObject;
	Hobject  ContCircle, EmptyImage, SyntheticModelImage;
	Hobject  ShapeModelImage, ShapeModelRegion, ShapeModel, SearchImage;
	Hobject  Ellipse, ImageReduced, ModelAtNewPosition, Region, Circle1;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		//
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/5.0, Height/5.0);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		if(m_isFirst)
		{
			ModelCreate2 (Radius,UsePolarity, &m_modelId);
			m_isFirst = false;
		}
		if(m_oldRadius != Radius ||m_oldPalarity!=UsePolarity )
		{
			clear_shape_model(m_modelId);
			ModelCreate2 (Radius,UsePolarity, &m_modelId);
			m_oldRadius = Radius;
			m_oldPalarity=UsePolarity;
		}
		get_shape_model_contours (&ShapeModel, m_modelId, 1);
		find_scaled_shape_model (ImageReduced, m_modelId, 0, 0, 0.95, 1.05, MinScore, 1, 0.5, "least_squares", (HTuple(3).Append(-2)), 0.7, &CenterRow, &CenterColumn, &Angle, &Scale, &Score);
		if (0 != (Score>0.05))
		{
			vector_angle_to_rigid(0, 0, 0, CenterRow[0].D(), CenterColumn[0].D(), Angle[0].D(), &MovementOfObject);
			hom_mat2d_scale(MovementOfObject, Scale[0].D(), Scale[0].D(),CenterRow[0].D(), CenterColumn[0].D(), &MoveAndScaleOfObject);
			affine_trans_contour_xld(ShapeModel, &ModelAtNewPosition, MoveAndScaleOfObject);
			gen_region_contour_xld(ModelAtNewPosition, &Region, "filled");
			smallest_circle(Region, &Row, &Column, &Radius1);

			Hobject Circle, RegionDifference;
			gen_circle(&Circle, Row, Column, Radius1);
			gen_circle(&Circle1, Row, Column, Radius);
			m_vPos[0].m_y = (float)Row;
			m_vPos[0].m_x = (float)Column;

			if(Circle.Id() != H_EMPTY_REGION) 
			{
				if(Radius1 >= Radius)
				{
					if(Radius1-Radius < 1)
					{
						gen_circle(&Circle, Row, Column, Radius1+1);
					}
					else
					{
						gen_circle(&Circle, Row, Column, Radius1);
					}
					difference (Circle, Circle1, &RegionDifference);
				}
				else
				{
					if(Radius - Radius1 < 1)
					{
						gen_circle(&Circle, Row, Column, Radius1-1);
					}
					else
					{
						gen_circle(&Circle, Row, Column, Radius1);
					}
					difference (Circle1, Circle, &RegionDifference);
				}
				m_vRgn[id] = RegionDifference;
			}
		}
		else
		{
			gen_circle(&Circle1, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle1;
			return S_FALSE;
		}
#ifdef NDEBUG
	}

	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CenterReLocationAlgo2Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 949;LP;0;1;1;1$\
					 374;LP;10;2000;1;192$\
					 377;FP;0.5;1.0;0.05;0.85");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::SectorRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	m_ringPara[id].m_Radius = (float)pRadius->fltVal;
	m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	float m_RadiusInner = Radius - RoiWidth;
	float DriftAngle1=(float)pDriftAngle1->fltVal;
	float DriftAngle2=(float)pDriftAngle2->fltVal;
	if(DriftAngle2-DriftAngle1<2.5)
	{
		DriftAngle2=(float)(DriftAngle1+2.5);
	}	
	m_sectorAngle[id].m_angelStart=DriftAngle1;
	m_sectorAngle[id].m_angelEnd=DriftAngle2;
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  Circle1_Inner,Circle2_EXT;
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		if (RoiWidth==0)
		{
			gen_circle(&Circle2_EXT, fCenterY, fCenterX, Radius+1);
			gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusInner);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		}
		else
		{
			if(m_RadiusInner>0)
			{
				//
				Hobject WeldRegNew,Rectangle1,OuterSector, InnerSector,ConnectedRegions;
				HTuple  Area, Row, Column,WeldCenterRow,WeldCenterColumn,LocationAngle;
				HTuple  Pointer, Type, WidthOri, HeightOri;
				Hobject Image = m_crtImg;

				area_center(m_weldRegions,&Area, &Row, &Column);
				if(Area>0)
				{
					gen_circle(&OuterSector, fCenterY, fCenterX, Radius);
					gen_circle(&InnerSector, fCenterY, fCenterX, m_RadiusInner);
					difference(OuterSector, InnerSector, &RegionDetect); 
					intersection(m_weldRegions,RegionDetect,&WeldRegNew);
					connection (WeldRegNew, &ConnectedRegions);
					area_center (ConnectedRegions, &Area, &Row, &Column);
					tuple_mean (Row, &WeldCenterRow);
					tuple_mean (Column, &WeldCenterColumn);

					if(WeldCenterColumn.Num()==1)
					{
						tuple_atan2 (fCenterY-WeldCenterRow[0].D(), WeldCenterColumn[0].D()-fCenterX, &LocationAngle);
						if(LocationAngle<0)
							LocationAngle=LocationAngle+2*3.14159;
						m_angleDetect = LocationAngle[0].D();
					}

				}
				get_image_pointer1(Image, &Pointer, &Type, &WidthOri, &HeightOri);

				HTuple ImgSize = WidthOri * HeightOri;
				HTuple StartAngle = m_angleDetect+(DriftAngle1/360.0)*6.283;
				HTuple EndAngle = m_angleDetect+(DriftAngle2/360.0)*6.283;


				if (ImgSize[0].I() < 5e5)
				{
					gen_rectangle1(&Rectangle1, 0, 0, RoiWidth, WidthOri-1);
					polar_trans_region_inv(Rectangle1, &RegionDetect,fCenterY, fCenterX, m_angleDetect+(DriftAngle1/360.0)*6.283,m_angleDetect+(DriftAngle2/360.0)*6.283, m_RadiusInner,Radius,  WidthOri, RoiWidth+1, WidthOri, HeightOri, "nearest_neighbor");
				} 
				else
				{
					//适应大尺寸图像
					Hobject OuterSector, InnerSector;								
					HTuple StartAngle1, EndAngle1;

					HTuple AngleDiff;
					tuple_abs(EndAngle-StartAngle, &AngleDiff);

					if ( AngleDiff >= HTuple(360).Rad())
					{
						StartAngle1 = 0;
						EndAngle1 = HTuple(360).Rad();

						gen_circle(&OuterSector, fCenterY, fCenterX, Radius);
						gen_circle(&InnerSector, fCenterY, fCenterX, m_RadiusInner);
						difference(OuterSector, InnerSector, &RegionDetect); 
						opening_circle (RegionDetect, &RegionDetect, 1);
					} 
					else
					{
						StartAngle1 =StartAngle;
						EndAngle1 = EndAngle;

						while (StartAngle1 < 0)
						{
							StartAngle1 = StartAngle1 + HTuple(360).Rad();
						}

						while (EndAngle1 < 0)
						{
							EndAngle1 = EndAngle1 + HTuple(360).Rad();
						}


						tuple_fmod(StartAngle1, 6.283, &StartAngle1);
						tuple_fmod(EndAngle1, 6.283, &EndAngle1);

						gen_circle_sector(&OuterSector, fCenterY, fCenterX, Radius, StartAngle1, EndAngle1);
						gen_circle_sector(&InnerSector, fCenterY, fCenterX, m_RadiusInner, StartAngle1, EndAngle1);
						difference(OuterSector, InnerSector, &RegionDetect);
						opening_circle (RegionDetect, &RegionDetect, 1);

					}						


				}

				m_sectorAngle[id].m_angleStartAbs= StartAngle[0].D() /3.14159*180;
				m_sectorAngle[id].m_angleEndAbs=  EndAngle[0].D() /3.14159*180;
			}
			else
			{
				gen_circle(&RegionDetect, fCenterY, fCenterX, Radius);
			}
		}
		m_vRgn[id] = RegionDetect ;	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::SectorRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("613$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 614;FP;0;720;0.5;40$\
					 615;FP;0;720;0.5;320"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CCanAlgo::SectorRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RadiusSize = (float)pRadius->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;


	float LocationRadius=(float)(m_locationRadius[0].I());
	float Radius =LocationRadius - RadiusSize;
	float m_RadiusInner =LocationRadius - RadiusSize-RoiWidth;


	if (Radius>0)
	{
		m_ringPara[id].m_Radius = LocationRadius - RadiusSize;
		m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	}
	else
	{
		Radius=1;
		m_ringPara[id].m_Radius =(float)pRoiWidth->fltVal+2;
		m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	}


	float DriftAngle1=(float)pDriftAngle1->fltVal;
	float DriftAngle2=(float)pDriftAngle2->fltVal;
	if(DriftAngle2-DriftAngle1<2.5)
	{
		DriftAngle2=(float)(DriftAngle1+2.5);
	}	
	m_sectorAngle[id].m_angelStart=DriftAngle1;
	m_sectorAngle[id].m_angelEnd=DriftAngle2;
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;




	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  Circle1_Inner,Circle2_EXT;
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		if (RoiWidth==0)
		{
			gen_circle(&Circle2_EXT, fCenterY, fCenterX, Radius+1);
			gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusInner);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		}
		else
		{
			if(m_RadiusInner>0)
			{
				Hobject  Rectangle1;
				// Local control variables 
				HTuple  Pointer, Type, WidthOri, HeightOri;
				Hobject Image = m_crtImg;
				Hobject WeldRegNew,OuterSector, InnerSector,ConnectedRegions;
				HTuple  Area, Row, Column,WeldCenterRow,WeldCenterColumn,LocationAngle;

				area_center(m_weldRegions,&Area, &Row, &Column);
				if(Area>0)
				{
					gen_circle(&OuterSector, fCenterY, fCenterX, Radius);
					gen_circle(&InnerSector, fCenterY, fCenterX, m_RadiusInner);
					difference(OuterSector, InnerSector, &RegionDetect); 
					intersection(m_weldRegions,RegionDetect,&WeldRegNew);
					connection (WeldRegNew, &ConnectedRegions);
					area_center (ConnectedRegions, &Area, &Row, &Column);
					tuple_mean (Row, &WeldCenterRow);
					tuple_mean (Column, &WeldCenterColumn);

					if(WeldCenterColumn.Num()==1)
					{
						tuple_atan2 (fCenterY-WeldCenterRow[0].D(), WeldCenterColumn[0].D()-fCenterX, &LocationAngle);
						if(LocationAngle<0)
							LocationAngle=LocationAngle+2*3.14159;
						m_angleDetect = LocationAngle[0].D();
					}

				}


				get_image_pointer1(Image, &Pointer, &Type, &WidthOri, &HeightOri);
				//gen_rectangle1(&Rectangle1, 0, 0, RoiWidth, WidthOri-1);
				//polar_trans_region_inv(Rectangle1, &RegionDetect,fCenterY, fCenterX, m_angleDetect+(DriftAngle1/360.0)*6.283,m_angleDetect+(DriftAngle2/360.0)*6.283, m_RadiusInner,Radius,  WidthOri, RoiWidth+1, WidthOri, HeightOri, "nearest_neighbor");

				HTuple ImgSize = WidthOri * HeightOri;
				HTuple StartAngle = m_angleDetect+(DriftAngle1/360.0)*6.283;
				HTuple EndAngle = m_angleDetect+(DriftAngle2/360.0)*6.283;


				if (ImgSize[0].I() < 5e5)
				{
					gen_rectangle1(&Rectangle1, 0, 0, RoiWidth, WidthOri-1);
					polar_trans_region_inv(Rectangle1, &RegionDetect,fCenterY, fCenterX, m_angleDetect+(DriftAngle1/360.0)*6.283,m_angleDetect+(DriftAngle2/360.0)*6.283, m_RadiusInner,Radius,  WidthOri, RoiWidth+1, WidthOri, HeightOri, "nearest_neighbor");
				} 
				else
				{
					//适应大尺寸图像
					Hobject OuterSector, InnerSector;					
					HTuple StartAngle1, EndAngle1;

					HTuple AngleDiff;
					tuple_abs(EndAngle-StartAngle, &AngleDiff);

					if ( AngleDiff >= HTuple(360).Rad())
					{
						StartAngle1 = 0;
						EndAngle1 = HTuple(360).Rad();

						gen_circle(&OuterSector, fCenterY, fCenterX, Radius);
						gen_circle(&InnerSector, fCenterY, fCenterX, m_RadiusInner);
						difference(OuterSector, InnerSector, &RegionDetect); 
					} 
					else
					{
						tuple_fmod(StartAngle, 6.283, &StartAngle1);
						tuple_fmod(EndAngle, 6.283, &EndAngle1);

						gen_circle_sector(&OuterSector, fCenterY, fCenterX, Radius, StartAngle1, EndAngle1);
						gen_circle_sector(&InnerSector, fCenterY, fCenterX, m_RadiusInner, StartAngle1, EndAngle1);
						difference(OuterSector, InnerSector, &RegionDetect); 			
					}					
				}

				m_sectorAngle[id].m_angleStartAbs= StartAngle[0].D() /3.14159*180;
				m_sectorAngle[id].m_angleEndAbs=  EndAngle[0].D() /3.14159*180;
			}
			else
			{
				gen_circle(&RegionDetect, fCenterY, fCenterX, Radius);
			}
		}
		m_vRgn[id] = RegionDetect ;	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::SectorRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("613$\
					 167;B$\
					 693;FP;-2000;2000.0;0.5;10$\
					 413;FP;1.5;2000;0.5;208$\
					 614;FP;-60;360.0;0.5;40$\
					 615;FP;-60;420.0;0.5;320"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::BoundaryRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	//float LocationRadius=(float)(m_locationRadius[0].I());

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	//
	if (RoiWidth<0.5)
	{
		RoiWidth=0.5;
	}
	//
	Hobject  Region_Inner,Region_EXT;
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_centerLocationRegion.Id()==H_EMPTY_REGION)
		{
			m_vWhiteErrorRgn[id].Reset();
			m_vErrorRgn[id].Reset();

			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		m_sectorAngle[id].m_angelStart = 0;
		m_sectorAngle[id].m_angelEnd = 365;

		Hobject RegionFill;
		fill_up(m_centerLocationRegion,&RegionFill);
		if( RadiusSize>=0.5)
		{
			dilation_circle(RegionFill,&Region_EXT,RadiusSize);
		}
		else if( RadiusSize<=-0.5)
		{
			erosion_circle(RegionFill,&Region_EXT,-RadiusSize);
		}
		else
		{
			Region_EXT=RegionFill;
		}
		erosion_circle(Region_EXT,&Region_Inner,RoiWidth);
		difference(Region_EXT,Region_Inner,&RegionDetect);
		m_vRgn[id] = RegionDetect ;	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::BoundaryRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1278$\
					 167;B$\
					 693;FP;-1500;1500;0.5;10$\
					 413;FP;1;2000;0.5;208"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//提取焊缝区域
STDMETHODIMP CCanAlgo::WeldRegionExtractionAlgo(VARIANT* rgnId, VARIANT* pGrayThresh, VARIANT* pDetectArea)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float GrayThresh = (float)pGrayThresh->fltVal;

	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	Hobject rgn = m_vRgn[id];
	Hobject PolarTransImage, Region, Image, DerivGauss, newRgn;
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	if ((fCenterX < 2.0) || (fCenterY < 2.0))
	{
		return S_FALSE;
	}

#ifdef NDEBUG
	try
	{
#endif
		//
		CComVariant retValue;
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		HTuple RadiusStart, RadiusEnd, RoiWidth, RoiHeight;
		distance_pr(rgn, fCenterY, fCenterX, &RadiusStart, &RadiusEnd);
		if (RadiusStart.Num() < 1)
		{
			RadiusStart = 2;
			RadiusEnd = 1;
		}
		if (RadiusStart == RadiusEnd)
		{
			RadiusStart = RadiusEnd - 1;
		}
		RoiWidth = (RadiusEnd + RadiusStart) / 2.0*3.14159;
		RoiHeight = RadiusEnd - RadiusStart;


		HTuple Width, Height;
		Image = m_crtImg;
		get_image_size(Image, &Width, &Height);
		polar_trans_image_ext(Image, &PolarTransImage, fCenterY, fCenterX, 0, 6.5, RadiusStart, RadiusEnd, RoiWidth, RoiHeight, "nearest_neighbor");
		mean_image(PolarTransImage, &PolarTransImage, 3, 12);
		derivate_gauss(PolarTransImage, &DerivGauss, 0.5, "x");
		Hobject Region1, Region2, RegionUnion, ConnectedRegions, SelectedRegions, RegionUnion1;
		Hobject XYTransRegion, RegionTrans, Circle2_EXT, RingRegion, Circle1_Inner, SectorRegion;
		threshold(DerivGauss, &Region1, -255, -GrayThresh);
		threshold(DerivGauss, &Region2, GrayThresh, 255);
		union2(Region1, Region2, &RegionUnion);

		//对比度低容易粘连其它区域
		closing_rectangle1(RegionUnion, &RegionUnion1, 1, RoiHeight / 4.0);

		connection(RegionUnion1, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "height", "and", RoiHeight*0.5, 100000);

		//筛选出来的区域比较多的时候需要过滤
		HTuple Area11, Row11, Column11, Column11Deviation, Area11Indices, RowStart, RowEnd;

		area_center(SelectedRegions, &Area11, &Row11, &Column11);

		if (Row11.Num() <= 0)
		{
			m_angleDetect = 0;
			retValue = -1;
			m_vErrorRgn[id] = RegionUnion;
			return S_FALSE;
		}

		tuple_deviation(Column11, &Column11Deviation);

		if (Area11.Num() > 2 && Column11Deviation < RoiWidth / 4.0)
		{
			tuple_sort_index(Area11, &Area11Indices);
			select_obj(SelectedRegions, &SelectedRegions, ((Area11Indices.Select((Area11.Num()) - 1)) + 1).Concat((Area11Indices.Select((Area11.Num()) - 2)) + 1));
		}
		if (Area11.Num() > 3 && Column11Deviation > RoiWidth / 4.0)
		{
			tuple_sort_index(Area11, &Area11Indices);
			select_obj(SelectedRegions, &SelectedRegions, (((Area11Indices.Select((Area11.Num()) - 1)) + 1).Concat((Area11Indices.Select((Area11.Num()) - 2)) + 1)).Concat((Area11Indices.Select((Area11.Num()) - 3)) + 1));
		}
		area_center(SelectedRegions, &Area11, &Row11, &Column11);

		/*tuple_gen_const(Row11.Num(), 0, &RowStart);
		tuple_gen_const(Row11.Num(), RoiHeight, &RowEnd);
		gen_region_line(&SelectedRegions, RowStart, Column11, RowEnd, Column11);*/

		union1(SelectedRegions, &SelectedRegions);
		polar_trans_region_inv(SelectedRegions, &XYTransRegion, fCenterY, fCenterX, 0, 6.5, RadiusStart, RadiusEnd, RoiWidth, RoiHeight, Width, Height, "nearest_neighbor");
	
		HTuple  Row3, Column3, Area1, ATan, ObjNum, locationRadius;
		Hobject ConnectedRegions2, LineRegions;
		area_center(XYTransRegion, &Area1, &Row3, &Column3);
		if (Row3.Num() != 1)
		{
			m_angleDetect = 0;
			retValue = -1;
			m_vErrorRgn[id] = RegionUnion;
			return S_FALSE;
		}
		tuple_atan2(fCenterY - Row3, Column3 - fCenterX, &ATan);
		if (ATan < 0)
			ATan = ATan + 6.28318;
		connection(XYTransRegion, &ConnectedRegions2);
		count_obj(ConnectedRegions2, &ObjNum);

		gen_empty_obj(&LineRegions);
		if (m_locationRadius < RadiusEnd)
		{
			locationRadius = RadiusEnd * 2;
		}
		else
		{
			locationRadius = m_locationRadius;
		}

		for (int i = 1; i <= ObjNum; i++)
		{
			Hobject ObjectSelected, Contour, RegionLines1;
			HTuple Rows, Columns, RowBegin1, ColBegin1, RowEnd1, ColEnd1, Nr1, Nc1, Dist1;
			HTuple Row2, Column2, Row21, Column21;
			select_obj(ConnectedRegions2, &ObjectSelected, i);
			get_region_points(ObjectSelected, &Rows, &Columns);
			gen_contour_polygon_xld(&Contour, Rows, Columns);
			fit_line_contour_xld(Contour, "tukey", -1, 0, 5, 2, &RowBegin1, &ColBegin1, &RowEnd1, &ColEnd1, &Nr1, &Nc1, &Dist1);
			if (RowBegin1.Num() <= 0)
			{
				m_angleDetect = 0;
				retValue = -1;
				m_vErrorRgn[id] = XYTransRegion;
				return S_FALSE;
			}
			intersection_line_circle(RowBegin1, ColBegin1, RowEnd1, ColEnd1, fCenterY, fCenterX, locationRadius, ATan - 1, ATan + 1, "positive", &Row2, &Column2);
			intersection_line_circle(RowBegin1, ColBegin1, RowEnd1, ColEnd1, fCenterY, fCenterX, 80, ATan - 1, ATan + 1, "positive", &Row21, &Column21);

			if (Row21.Num() <= 0)
			{
				Row21 = fCenterY;
				Column21 = fCenterX;
			}
			if (Row2.Num() <= 0)
			{
				Row2 = 0;
				Column2 = 0;

			}
			gen_region_line(&RegionLines1, Row21[0].D(), Column21[0].D(), Row2[0].D(), Column2[0].D());
			concat_obj(LineRegions, RegionLines1, &LineRegions);

		}
		union1(LineRegions, &XYTransRegion);
		//shape_trans (LineRegions, &XYTransRegion,"convex");

		m_weldRegions = XYTransRegion;
		if (ATan.Num() > 0)
		{
			m_angleDetect = ATan[0].D();
			retValue = 180.0*(m_angleDetect / (3.14159));
		}
		else
		{
			m_angleDetect = 0;
			retValue = -1;
			m_vErrorRgn[id] = XYTransRegion;
		}

		m_vRgn[id] = LineRegions;
		retValue.Detach(pDetectArea);

#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldRegionExtractionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("8015$\
					 397;R;LP;0;1000;1;<=#0$\
					 167;B$\
					 433;FP;0.1;255;0.1;5.5");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if (FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//焊缝区域选择
STDMETHODIMP CCanAlgo:: WeldRegionSelection(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth , VARIANT* pWeldSelection, VARIANT* pRegionErosionSize)
{
	int id                     = (int)rgnId->fltVal;
	m_ringPara[id].m_Radius    = (float)pRadius->fltVal;
	m_ringPara[id].m_RoiWidth  = (float)pRoiWidth->fltVal;
	int WeldSelection          = (int)pWeldSelection->fltVal;
	float RegionErosionSize    = (float)pRegionErosionSize->fltVal;
	Hobject  Circle1_Inner,Circle2_EXT,RegionDetect,RegionDiff,RegionDiffDilation,DetectRegion,SectorRegion;
	Hobject newRgn,WeldRegion;
#ifdef NDEBUG
	try
	{
#endif
		m_vRgn[id] = newRgn ;
		if(m_weldRegions.Id()==H_EMPTY_REGION)
		{
			return S_FALSE;
		}
		m_sectorAngle[id].m_angleStartAbs=0;
		m_sectorAngle[id].m_angleEndAbs=360;
		float RoiWidth = m_ringPara[id].m_RoiWidth;
		float Radius = m_ringPara[id].m_Radius;
		float m_RadiusInner = Radius - RoiWidth;

		if(m_RadiusInner<=0)
		{
			m_RadiusInner=0;
		}
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		if((fCenterX<2.0)||(fCenterY<2.0))
		{
			return S_FALSE;
		}
		gen_circle(&Circle2_EXT, fCenterY, fCenterX, Radius);
		gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusInner);
		difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		shape_trans(m_weldRegions,&WeldRegion,"convex");
		difference(RegionDetect,WeldRegion,&SectorRegion);
		if(WeldSelection==0)
		{
			if(RegionErosionSize<0)
			{
				dilation_rectangle1(SectorRegion,&RegionDiffDilation,-RegionErosionSize+0.5,-RegionErosionSize+0.5);
				difference(WeldRegion,RegionDiffDilation,&DetectRegion);
			}
			else
			{
				dilation_circle(WeldRegion,&RegionDiffDilation,RegionErosionSize+0.5);
				intersection(RegionDiffDilation,RegionDetect,&DetectRegion);
			}
		}
		else if(WeldSelection==1)
		{
			if(RegionErosionSize<0)
			{
				dilation_circle(WeldRegion,&RegionDiffDilation,-RegionErosionSize+0.5);
				difference(RegionDetect,RegionDiffDilation,&DetectRegion);
			}
			else
			{
				dilation_circle(SectorRegion,&RegionDiffDilation,RegionErosionSize+0.5);
				intersection(RegionDetect,RegionDiffDilation,&DetectRegion);
			}
		}
		else
		{
			if(RegionErosionSize<=0)
			{
				RegionErosionSize=-RegionErosionSize;
			}
			dilation_circle(m_weldRegions,&RegionDiffDilation,RegionErosionSize+2.5);
			intersection(RegionDetect,RegionDiffDilation,&DetectRegion);

		}
		intersection(DetectRegion,RegionDetect,&DetectRegion);
		m_vRgn[id] = DetectRegion ;	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo:: WeldRegionSelectionHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("8016$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 8017;LP;0;2;1;0$\
					 443;FP;-150.5;150.5;0.5;1.0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CCanAlgo::BoundaryRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pClosingType,VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	int ClosingType= (int)pClosingType->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	//float LocationRadius=(float)(m_locationRadius[0].I());

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	//
	if (RoiWidth<0.5)
	{
		RoiWidth=0.5;
	}
	//
	Hobject  Region_Inner,Region_EXT;
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_centerLocationRegion.Id()==H_EMPTY_REGION)
		{
			m_vWhiteErrorRgn[id].Reset();
			m_vErrorRgn[id].Reset();

			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		m_sectorAngle[id].m_angelStart = 0;
		m_sectorAngle[id].m_angelEnd = 365;

		Hobject RegionFill,centerLocationRegion;
		if(ClosingType==0)
		{
			shape_trans (m_centerLocationRegion, &centerLocationRegion, "convex");
		}
		else if(ClosingType==1)
		{
			shape_trans (m_centerLocationRegion, &centerLocationRegion, "ellipse");
		}
		else
		{
			closing_circle(m_centerLocationRegion,&centerLocationRegion,m_locationRadius);
		}
		fill_up(centerLocationRegion,&RegionFill);
		if( RadiusSize>=0.5)
		{
			dilation_circle(RegionFill,&Region_EXT,RadiusSize);
		}
		else if( RadiusSize<=-0.5)
		{
			erosion_circle(RegionFill,&Region_EXT,-RadiusSize);
		}
		else
		{
			Region_EXT=RegionFill;
		}
		erosion_circle(Region_EXT,&Region_Inner,RoiWidth);
		difference(Region_EXT,Region_Inner,&RegionDetect);
		m_vRgn[id] = RegionDetect ;	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::BoundaryRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("1278$\
					 167;B$\
					 8014;LP;0;2;1;1$\
					 693;FP;-1500;1500;0.5;10$\
					 413;FP;1;2000;0.5;208"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//RectangleLeft
STDMETHODIMP CCanAlgo::RectangleLeftLocationAlgo(VARIANT* rgnId,  VARIANT* pThresholdLow, VARIANT* pWidthPlaceSet,VARIANT* pRegionHeight, VARIANT* pRegionWidth,VARIANT* pErosionSize)
{
	//得到矩形检测区域
	int id = (int)rgnId->fltVal;

	int ThresholdLow =(int)pThresholdLow->fltVal;
	int WidthPlaceSet =(int)pWidthPlaceSet->fltVal;
	int HeightSet =(int)(pRegionHeight->fltVal);
	int WidthSet =(int)(pRegionWidth->fltVal);
	float ErosionSize = (float)pErosionSize->fltVal;

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		set_check("~give_error");
		Hobject Image=m_crtImg;
		HTuple Width, Height,CenterCol;
		Hobject Rectangle;
		get_image_size(Image, &Width, &Height);
		//
		if((WidthPlaceSet<2)||(HeightSet>Height-10)||(WidthSet<3))
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		// Local iconic variables 
		Hobject  ImageReduced;
		Hobject  Region, RegionFillUp, RegionOpening, ConnectedRegions;
		Hobject  SelectedRegions, SelectedRegions1;
		// Local control variables 
		HTuple  Area;
		HTuple  Row, Column, Row1, Column1, Row2;
		HTuple  Column2;

		if (0 != (HeightSet<10))
		{
			HeightSet = 10;
		}
		if (0 != (WidthSet<3))
		{
			WidthSet = 3;
		}
		if (0 != (ThresholdLow<0))
		{
			ThresholdLow = 0;
		}
		if (0 != (WidthPlaceSet>(Width-1)))
		{
			WidthPlaceSet = Width[0].I()-1;
		}
		if (0 != ((WidthPlaceSet-80)<2))
		{
			gen_rectangle1(&Rectangle, (fCenterY)-(HeightSet/2.0), 2, (fCenterY)+(HeightSet/2.0),  WidthPlaceSet+80);
		}
		else
		{
			if (WidthPlaceSet+80>=Width-1)
			{
				gen_rectangle1(&Rectangle, (fCenterY)-(HeightSet/2.0), WidthPlaceSet-80, (fCenterY)+(HeightSet/2.0),Width-1);
			}
			else
			{
				gen_rectangle1(&Rectangle, (fCenterY)-(HeightSet/2.0), WidthPlaceSet-80, (fCenterY)+(HeightSet/2.0), WidthPlaceSet+80);
			}
		}
		reduce_domain(Image, Rectangle, &ImageReduced);
		threshold(ImageReduced, &Region, 0, ThresholdLow);
		fill_up_shape(Region, &RegionFillUp, "area", 1, 200);
		opening_rectangle1(RegionFillUp, &RegionOpening, 3, HeightSet/2.0);
		connection(RegionOpening, &ConnectedRegions);
		select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
		select_shape(SelectedRegions, &SelectedRegions1, (HTuple("width").Append("height")),"and", HTuple(10).Concat(HeightSet*0.6), (HTuple(99999).Append(99999)));
		area_center(SelectedRegions1, &Area, &Row, &Column);
		if (0 != (Area>0))
		{
			inner_rectangle1(SelectedRegions1, &Row1, &Column1, &Row2, &Column2);
			WidthPlaceSet = Column2[0].D();
		}
		gen_rectangle1(&RegionDetect, (fCenterY)-(HeightSet/2.0), WidthPlaceSet, (fCenterY)+(HeightSet/2.0), WidthPlaceSet+WidthSet);
		if(ErosionSize>=0.5)
		{
			erosion_circle (RegionDetect, &RegionDetect,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(RegionDetect, &RegionDetect,abs(ErosionSize));
		}
		m_vRgn[id] = RegionDetect ;	
		set_check("give_error");
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::RectangleLeftLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("708$\
					 167;B$\
					 191;LP;1;255;1;50$\
					 825;LP;10;1276;1;134$\
					 667;LP;20;800;1;150$\
					 668;LP;5;1600;1;40$\
					 443;FP;-10.5;10.5;0.5;1.0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::DynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	//焊缝区域检测时，去掉焊缝两侧边界的影响
	if (m_WeldLocatedAlgoRegIndex >= 0)
	{
		Hobject WeldBoudary;
		select_obj(m_WeldRegRelated[m_WeldLocatedAlgoRegIndex], &WeldBoudary, 3);
		difference(rgn, WeldBoudary, &rgn);
	}


	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	int WhiteMaskSize = (int) pWhiteMaskSize->fltVal;
	float SeriousWhitePointDynThresh = (float) pSeriousWhitePointDynThresh->fltVal;
	int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		if(abs(WhiteMaskSize-BlackMaskSize)>=1)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
		}
		ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[rId].push_back(float(SeriousWhitePointDynThresh));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			abs_diff_image (ImageReduced, ImageReduceMean, &ImageSub, 1);
			gray_histo (rgn, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd* Width* Height<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*(Width[0].D()* Height[0].D()));
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;
		union2(BlackUnion, WhiteUnion, &DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (BlackUnion, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
			dilation_circle (WhiteUnion, &DefectRegs, 2.5);
			m_vWhiteErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("411$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;60$\
					 416;LP;1;200;1;10$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;250;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::DynThresholdAvoidReflectionAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	int WhiteMaskSize = (int) pWhiteMaskSize->fltVal;
	float SeriousWhitePointDynThresh = (float) pSeriousWhitePointDynThresh->fltVal;
	int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;
		HTuple Channels;
		count_channels (Image, &Channels);
		if(Channels>1)
		{
			channels_to_image (Image, &Image);
		}
		Hobject ImageReduced, ImageReduceMean;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//去掉面积大于200白色倒影区域
		Hobject  ImageReduced_t,ImageReduced_Paint,ExpandedImage, Region_ext1, RegionDilation3;
		Hobject  ConnectedRegions2, SelectedRegions2, RegionDilation4;
		Hobject  RegionDifference1,Rectangle;
		HTuple  AbsoluteHisto, RelativeHisto;
		HTuple  th_rat,th_Result,Index1,Mean2, Deviation2,Width1, Height1;
		get_image_size(Image, &Width1, &Height1);
		gen_rectangle1(&Rectangle, 0, 0, Height1-1, Width1-1);
		intersection (rgn, Rectangle, &rgn);
		change_domain(Image, rgn, &ImageReduced_t);
		gray_histo(rgn, Image, &AbsoluteHisto, &RelativeHisto);
		th_rat = 0.0;
		th_Result = 0;
		for (Index1=255; Index1>=0; Index1+=-1)
		{
			th_rat = HTuple(RelativeHisto[Index1])+th_rat;
			if (0 != (th_rat>0.1))
			{
				th_Result = Index1;
				Index1 = -1;
				break;
			}
		}
		threshold(ImageReduced_t, &Region_ext1, th_Result, 255);
		dilation_circle(Region_ext1, &RegionDilation3, 1.5);
		connection(RegionDilation3, &ConnectedRegions2);
		select_shape(ConnectedRegions2, &SelectedRegions2, "area", "and", 200, 9999999);//面积小于200的白点，默认为是孤立白点，与倒影无关。
		dilation_circle(SelectedRegions2, &RegionDilation4, 3.5);
		difference(RegionDilation4, SelectedRegions2, &RegionDifference1);
		intensity(RegionDifference1, Image, &Mean2, &Deviation2);
		paint_region(SelectedRegions2, Image, &ImageReduced_Paint, Mean2, "fill");
		//
		HTuple RegionNumber;
		tuple_length(Mean2,&RegionNumber);
		if(RegionNumber>0)
		{
			expand_domain_gray (ImageReduced_Paint, &ExpandedImage, 20);
		}
		else
		{
			ExpandedImage=Image;
		}
		//
		ImageProcessAlg.MeanImageReduced(ExpandedImage,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		if(abs(WhiteMaskSize-BlackMaskSize)>=1)
		{
			ImageProcessAlg.MeanImageReduced(ExpandedImage,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
		}
		ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[rId].push_back(float(SeriousWhitePointDynThresh));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			abs_diff_image (ImageReduced, ImageReduceMean, &ImageSub, 1);
			gray_histo (rgn, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd* Width* Height<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*(Width[0].D()* Height[0].D()));
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;
		union2(BlackUnion, WhiteUnion, &DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (BlackUnion, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
			dilation_circle (WhiteUnion, &DefectRegs, 2.5);
			m_vWhiteErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::DynThresholdAvoidReflectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("616$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;60$\
					 416;LP;1;200;1;10$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;250;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int ThresholdValue = (int)pDynThresh->fltVal;
	int DefectSize = (int) pDefectSize->fltVal;
	float RotaAngle = (float)pRotaAngle->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image = m_crtImg;
		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
		//

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		reduce_domain (Image, rgn, &ImageReduced);
		vector_angle_to_rigid(fCenterY,fCenterX, 0, fCenterY, fCenterX, HTuple(RotaAngle).Rad(), &HomMat2D);
		affine_trans_image(ImageReduced, &ImageAffinTrans, HomMat2D, "bilinear", "false");
		dyn_threshold(ImageReduced, ImageAffinTrans, &RegionDynThresh, ThresholdValue, "not_equal");
		connection(RegionDynThresh, &ConnectedRegions4);
		select_shape(ConnectedRegions4, &SelectedRegions4, "area", "and", DefectSize, 9999999);

		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(ThresholdValue));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			sub_image (ImageAffinTrans,ImageReduced,  &ImageSub, 1, 0);
			gray_histo (rgn, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd* Width* Height<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*(Width[0].D()* Height[0].D()));
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}

		Hobject DefectRegs;
		union1(SelectedRegions4, &DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("609$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 610;FP;0.5;60;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//旋转对比算法核心部分，简化
void GetDefectsInPolarImg4AnnualRota(Hobject Image, Hobject PolarTransImageOrigin, Hobject PolarTransImageRotated, Hobject* pDefects, HTuple ThredLow, HTuple DefectSizeLow, HTuple Offset, bool m_bDebugImg, HTuple fCenterY, HTuple fCenterX, HTuple RadiusStart, HTuple RadiusEnd, HTuple RotaAngle, HTuple StartAngle, HTuple EndAngle, HTuple* pDefectPolarArea, HTuple DefectType = "dark")
{
	gen_empty_obj(pDefects);
	*pDefectPolarArea = 0;

	Hobject BlackRegDynLight, BlackRegDynDark, BlackRegDynDarkMoved, BlackRegDyn;
	Hobject BlackRegConnected, BlackDefectsPolar;
	dyn_threshold(PolarTransImageRotated, PolarTransImageOrigin, &BlackRegDynLight, ThredLow, "light");
	dyn_threshold(PolarTransImageRotated, PolarTransImageOrigin, &BlackRegDynDark, ThredLow, "dark");	

	if (DefectType == "dark")
	{
		move_region(BlackRegDynDark, &BlackRegDynDarkMoved, 0, Offset);
		intersection(BlackRegDynDarkMoved, BlackRegDynLight, &BlackRegDyn);
	} 
	else if(DefectType == "white")
	{
		move_region(BlackRegDynLight, &BlackRegDynDarkMoved, 0, Offset);
		intersection(BlackRegDynDarkMoved, BlackRegDynDark, &BlackRegDyn);
	}

	HTuple OriginImgWidth, OriginImgHeight;
	get_image_size(Image, &OriginImgWidth, &OriginImgHeight);

	HTuple AngleEndOrigin = HTuple(EndAngle).Rad();
	HTuple PolarImgHeight = RadiusStart - RadiusEnd;

	//HTuple PolarImgWidth = AngleEndOrigin *0.5*(RadiusStart + RadiusEnd);


	HTuple PolarImgWidth = (AngleEndOrigin- HTuple(StartAngle).Rad())*RadiusStart;

	HTuple InterpolationMethod = "nearest_neighbor";

	closing_circle(BlackRegDyn, &BlackRegDyn, 2.5);
	connection(BlackRegDyn, &BlackRegConnected);
	select_shape(BlackRegConnected, &BlackDefectsPolar, "area", "and", DefectSizeLow, OriginImgWidth * OriginImgHeight);
	union1(BlackDefectsPolar,&BlackDefectsPolar);
	HTuple BlackDefectPolarArea, BlackDefectPolarRow, BlackDefectPolarCol;
	area_center(BlackDefectsPolar, &BlackDefectPolarArea, &BlackDefectPolarRow, &BlackDefectPolarCol);

	Hobject BlackDefects;
	if (BlackDefectPolarArea > 0)
	{
		/*if (m_bDebugImg)
		{*/
		polar_trans_region_inv(BlackDefectsPolar, &BlackDefects, fCenterY, fCenterX, HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, OriginImgWidth, OriginImgHeight, InterpolationMethod);
		union1(BlackDefects, &BlackDefects);
		concat_obj(*pDefects, BlackDefects, pDefects);
		//} 
		//else
		//{
		//	//在线检测阶段，只利用错误区域的重心，标记成圆形区域
		//	HTuple RadiusOrigin = RadiusStart - BlackDefectPolarRow;
		//	HTuple Theta = 1.0 * BlackDefectPolarCol / PolarImgWidth * HTuple((EndAngle - StartAngle).Abs()).Rad() + HTuple(StartAngle).Rad();

		//	HTuple sinTheta, cosTheta;
		//	tuple_sin(Theta, &sinTheta);
		//	tuple_cos(Theta, &cosTheta);

		//	HTuple Row_Origin = fCenterY - RadiusOrigin* sinTheta;
		//	HTuple Col_Origin = fCenterX + RadiusOrigin* cosTheta;

		//	HTuple Radius_Origin;
		//	HTuple DefectNum;
		//	tuple_length(BlackDefectPolarRow, &DefectNum);
		//	tuple_gen_const(DefectNum, 10, &Radius_Origin);
		//	gen_circle(&BlackDefects, Row_Origin, Col_Origin, Radius_Origin);
		//	union1(BlackDefects, &BlackDefects);
		//	concat_obj(*pDefects, BlackDefects, pDefects);
		//}

		*pDefectPolarArea = BlackDefectPolarArea;
	}	
}

STDMETHODIMP CCanAlgo:: AnnularRotaDynThresholdAlgoBW(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int BlackThredLow = (int)pBlackThredLow->fltVal;
	int BlackDefectSize = (int) pBlackDefectSize->fltVal;
	float RotaAngle = (float)pRotaAngle->fltVal;
	int WhiteThredLow = (int)pWhiteThredLow->fltVal;
	int WhiteDefectSize = (int) pWhiteDefectSize->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image = m_crtImg;
		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
		//

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		//get the params for polar trans
		HTuple RadiusStart, RadiusEnd;
		distance_pr(rgn, fCenterY, fCenterX, &RadiusEnd, &RadiusStart);
		if (RadiusStart.Num()< 1)
		{
			RadiusStart = 2;
			RadiusEnd= 1;
		}
		if (RadiusStart == RadiusEnd)
		{
			RadiusStart = RadiusEnd - 1;
		}

		/*
		float RoiWidth = m_ringPara[rId].m_RoiWidth;
		float Radius = m_ringPara[rId].m_Radius;


		if (Radius - RoiWidth < 1)
		{			
		RadiusEnd = 0;
		RadiusStart = Radius;
		}
		else
		{
		RadiusEnd = Radius - RoiWidth;
		RadiusStart = Radius;
		}*/
		float StartAngle = m_sectorAngle[rId].m_angleStartAbs;	//逆时针转顺时针
		float EndAngle =  m_sectorAngle[rId].m_angleEndAbs;


		HTuple AngleEndOrigin = HTuple(EndAngle).Rad();
		HTuple PolarImgHeight = RadiusStart - RadiusEnd;

		HTuple PolarImgWidth = (AngleEndOrigin- HTuple(StartAngle).Rad())*RadiusStart;

		HTuple InterpolationMethod = "nearest_neighbor";

		//As RadiusStart > RadiusEnd, the polar transformation is performed clockwise(顺时针)
		Hobject PolarTransImageOrigin, PolarTransImageRotated;
		polar_trans_image_ext(Image, &PolarTransImageOrigin, fCenterY, fCenterX,  HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);
		polar_trans_image_ext(Image, &PolarTransImageRotated, fCenterY, fCenterX, HTuple(StartAngle + RotaAngle).Rad(), AngleEndOrigin + HTuple(RotaAngle).Rad(), RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);

		HTuple MiddleRadius = 0.5*(RadiusStart + RadiusEnd);
		HTuple Offset = RadiusStart * HTuple(RotaAngle).Rad();
		HTuple OriginImgWidth, OriginImgHeight;
		get_image_size(Image, &OriginImgWidth, &OriginImgHeight);


		Hobject DefectRegs;
		gen_empty_obj(&DefectRegs);

		Hobject BlackDefects;
		gen_empty_obj(&BlackDefects);
		HTuple  BlackDefectPolarArea;

		GetDefectsInPolarImg4AnnualRota(Image, PolarTransImageOrigin, PolarTransImageRotated, &BlackDefects, BlackThredLow, BlackDefectSize, Offset, m_bDebugImg, fCenterY, fCenterX, RadiusStart, RadiusEnd, RotaAngle, StartAngle, EndAngle, &BlackDefectPolarArea, "dark");
		union2(DefectRegs, BlackDefects, &DefectRegs);
		intersection(DefectRegs,rgn,&DefectRegs);

		Hobject WhiteDefects;
		gen_empty_obj(&WhiteDefects);
		HTuple  WhiteDefectPolarArea;
		GetDefectsInPolarImg4AnnualRota(Image, PolarTransImageOrigin, PolarTransImageRotated, &WhiteDefects, WhiteThredLow, WhiteDefectSize, Offset, m_bDebugImg, fCenterY, fCenterX, RadiusStart, RadiusEnd, RotaAngle, StartAngle, EndAngle, &WhiteDefectPolarArea, "white");
		intersection(WhiteDefects,rgn,&WhiteDefects);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(BlackThredLow));
			m_dictSpecialData[rId].push_back(float(WhiteThredLow));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub,Domain;
			get_domain(PolarTransImageRotated,&Domain);
			get_image_size (Image,& Width, &Height);
			abs_diff_image (PolarTransImageOrigin, PolarTransImageRotated, &ImageSub, 1);
			gray_histo (Domain, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd* Width* Height<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*(Width[0].D()* Height[0].D()));
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}
		union2(DefectRegs, WhiteDefects, &DefectRegs);
		HTuple Area, Row_test, Column_test;
		union1(DefectRegs, &DefectRegs);

		area_center(DefectRegs, &Area, &Row_test, &Column_test);

		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
			m_vWhiteErrorRgn[rId] = newRgn;
		}
		else
		{			
			dilation_circle (BlackDefects, &BlackDefects, 2.5);
			intersection(BlackDefects,rgn,&BlackDefects);

			dilation_circle (WhiteDefects, &WhiteDefects, 2.5);
			intersection(WhiteDefects,rgn,&WhiteDefects);


			m_vErrorRgn[rId] = BlackDefects;
			m_vWhiteErrorRgn[rId] = WhiteDefects;			

			//			retValue = Area[0].I();

			//输出为极坐标下的缺陷面积，避免前后不一致，2018.05.20
			HTuple PolarDefectArea = 0;

			if (BlackDefectPolarArea[0].I() > 0)
			{
				PolarDefectArea = PolarDefectArea + BlackDefectPolarArea;
			}

			if (WhiteDefectPolarArea[0].I() > 0)
			{
				PolarDefectArea = PolarDefectArea + WhiteDefectPolarArea;
			}

			retValue = PolarDefectArea[0].I();
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo:: AnnularRotaDynThresholdAlgoBWHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1295$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 610;FP;0.5;60;0.5;3.5$\
					 427;FP;0;255;0.5;23$\
					 428;LP;1;1000;1;10$\
					 430;FP;0;255;0.5;23$\
					 431;LP;1;1000;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo:: SectorRotaDynThresholdAlgo(VARIANT* rgnId, VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int ThresholdValue = (int)pDynThresh->fltVal;
	int DefectSize = (int) pDefectSize->fltVal;
	float RotaAngle = (float)pRotaAngle->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image = m_crtImg;
		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
		//

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		//get the params for polar trans
		HTuple RadiusStart, RadiusEnd;
		distance_pr(rgn, fCenterY, fCenterX, &RadiusEnd, &RadiusStart);
		if (RadiusStart.Num()< 1)
		{
			RadiusStart = 2;
			RadiusEnd= 1;
		}
		if (RadiusStart == RadiusEnd)
		{
			RadiusStart = RadiusEnd - 1;
		}

		/*
		float RoiWidth = m_ringPara[rId].m_RoiWidth;
		float Radius = m_ringPara[rId].m_Radius;


		if (Radius - RoiWidth < 1)
		{			
		RadiusEnd = 0;
		RadiusStart = Radius;
		}
		else
		{
		RadiusEnd = Radius - RoiWidth;
		RadiusStart = Radius;
		}*/
		float StartAngle = m_sectorAngle[rId].m_angleStartAbs;	//逆时针转顺时针
		float EndAngle =  m_sectorAngle[rId].m_angleEndAbs;


		HTuple AngleEndOrigin = HTuple(EndAngle).Rad();
		HTuple PolarImgHeight = RadiusStart - RadiusEnd;

		HTuple PolarImgWidth = (AngleEndOrigin- HTuple(StartAngle).Rad())*RadiusStart;

		HTuple InterpolationMethod = "nearest_neighbor";

		//As RadiusStart > RadiusEnd, the polar transformation is performed clockwise(顺时针)
		Hobject PolarTransImageOrigin, PolarTransImageRotated;
		polar_trans_image_ext(Image, &PolarTransImageOrigin, fCenterY, fCenterX,  HTuple(StartAngle).Rad(), AngleEndOrigin-HTuple(RotaAngle).Rad(), RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);
		polar_trans_image_ext(Image, &PolarTransImageRotated, fCenterY, fCenterX, HTuple(StartAngle + RotaAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);

		HTuple MiddleRadius = 0.5*(RadiusStart + RadiusEnd);
		HTuple Offset = RadiusStart * HTuple(RotaAngle).Rad();
		HTuple OriginImgWidth, OriginImgHeight;
		get_image_size(Image, &OriginImgWidth, &OriginImgHeight);


		Hobject DefectRegs,newRgn;
		HTuple Area, Row_test, Column_test;
		gen_empty_obj(&DefectRegs);

		dyn_threshold(PolarTransImageOrigin, PolarTransImageRotated, &RegionDynThresh, ThresholdValue, "not_equal");
		connection(RegionDynThresh, &ConnectedRegions4);
		select_shape(ConnectedRegions4, &SelectedRegions4, "area", "and", DefectSize, 9999999);
		union1(SelectedRegions4,&DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);

		if(Area <= 0)
		{
			retValue = 0.0;
			m_vErrorRgn[rId] = newRgn;
			m_vWhiteErrorRgn[rId] = newRgn;
		}
		else
		{			
			polar_trans_region_inv(DefectRegs, &DefectRegs, fCenterY, fCenterX, HTuple(StartAngle).Rad(), AngleEndOrigin-HTuple(RotaAngle).Rad(), RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, OriginImgWidth, OriginImgHeight, InterpolationMethod);

			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			intersection(DefectRegs,rgn,&DefectRegs);
			m_vErrorRgn[rId] = DefectRegs;
			m_vWhiteErrorRgn[rId] = newRgn;			
			retValue = Area[0].I();
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo:: SectorRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("609$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 610;FP;0.5;60;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo:: RingRegionDynThresholdAlgoBW(VARIANT* rgnId, VARIANT* pDetectType, VARIANT* pMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int DetectType = (int)pDetectType->fltVal;
	int MaskSize = (int)pMaskSize->fltVal;
	int BlackThredLow = (int)pBlackThredLow->fltVal;
	int BlackDefectSize = (int) pBlackDefectSize->fltVal;
	int WhiteThredLow = (int)pWhiteThredLow->fltVal;
	int WhiteDefectSize = (int) pWhiteDefectSize->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image = m_crtImg;
		Hobject ImageReduced,ImageAffinTrans,ConnectedRegions4,SelectedRegions4;
		//

		float fCenterX,fCenterY;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		//get the params for polar trans
		Hobject newRgn; 
		HTuple RadiusStart, RadiusEnd;
		distance_pr(rgn, fCenterY, fCenterX, &RadiusEnd, &RadiusStart);
		if (RadiusStart.Num()< 1)
		{
			RadiusStart = 2;
			RadiusEnd= 1;
		}
		if (RadiusStart == RadiusEnd)
		{
			RadiusStart = RadiusEnd - 1;
		}
		float StartAngle = m_sectorAngle[rId].m_angleStartAbs;	//逆时针转顺时针
		float EndAngle =  m_sectorAngle[rId].m_angleEndAbs;


		HTuple AngleEndOrigin = HTuple(EndAngle).Rad();
		HTuple PolarImgHeight = RadiusStart - RadiusEnd;

		HTuple PolarImgWidth = (AngleEndOrigin- HTuple(StartAngle).Rad())*RadiusStart;

		//HTuple InterpolationMethod = "nearest_neighbor";
		HTuple InterpolationMethod = "bilinear";

		//As RadiusStart > RadiusEnd, the polar transformation is performed clockwise(顺时针)
		Hobject PolarTransImageOrigin,ImageMean,RegionDynThresh,RegionDynThresh1,SeriousConnected;
		Hobject SeriousPointReg,DetectUnion,WhiteDetectUnion,DarkDetectUnion;
		polar_trans_image_ext(Image, &PolarTransImageOrigin, fCenterY, fCenterX,  HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);
		mean_image (PolarTransImageOrigin, &ImageMean, MaskSize, 1);
		if(DetectType==0)
		{
			dyn_threshold (PolarTransImageOrigin, ImageMean, &RegionDynThresh, BlackThredLow, "dark");
			connection(RegionDynThresh, &SeriousConnected); 
			select_shape(SeriousConnected, &SeriousPointReg, "area", "and", BlackDefectSize, 99999);
			union1(SeriousPointReg, &DetectUnion);
		}
		else if (DetectType==1)
		{
			dyn_threshold (PolarTransImageOrigin, ImageMean, &RegionDynThresh, WhiteThredLow, "light");
			connection(RegionDynThresh, &SeriousConnected); 
			select_shape(SeriousConnected, &SeriousPointReg, "area", "and", WhiteDefectSize, 99999);
			union1(SeriousPointReg, &DetectUnion);
		}
		else
		{
			dyn_threshold (PolarTransImageOrigin, ImageMean, &RegionDynThresh, BlackThredLow, "dark");
			connection(RegionDynThresh, &SeriousConnected); 
			select_shape(SeriousConnected, &SeriousPointReg, "area", "and", BlackDefectSize, 99999);
			union1(SeriousPointReg, &DarkDetectUnion);

			dyn_threshold (PolarTransImageOrigin, ImageMean, &RegionDynThresh1, WhiteThredLow, "light");
			connection(RegionDynThresh1, &SeriousConnected); 
			select_shape(SeriousConnected, &SeriousPointReg, "area", "and", WhiteDefectSize, 99999);
			union1(SeriousPointReg, &WhiteDetectUnion);
			union2(WhiteDetectUnion,DarkDetectUnion,&DetectUnion);
		}

		HTuple Area, Row_test, Column_test;
		area_center(DetectUnion, &Area, &Row_test, &Column_test);
		retValue = 0.0;
		if(Area > 0)
		{
			HTuple OriginImgWidth, OriginImgHeight;

			get_image_size(Image, &OriginImgWidth, &OriginImgHeight);
			polar_trans_region_inv (DetectUnion, &DetectUnion, fCenterY, fCenterX,  HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, OriginImgWidth, OriginImgHeight, "nearest_neighbor");
			intersection(DetectUnion,rgn,&DetectUnion);
			area_center(DetectUnion, &Area, &Row_test, &Column_test);

			if(Area > 0)
			{
				retValue = Area[0].I();
				dilation_circle (DetectUnion, &DetectUnion, 2.5);
				intersection(DetectUnion,rgn,&DetectUnion);
				m_vErrorRgn[rId] = DetectUnion;
				m_vWhiteErrorRgn[rId] = newRgn;
			}
		}

		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo:: RingRegionDynThresholdAlgoBWHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("8012$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 1314;LP;-1;1;1;0$\
					 8013;LP;3;101;2;7$\
					 427;FP;0;255;0.5;23$\
					 428;LP;1;1000;1;10$\
					 430;FP;0;255;0.5;23$\
					 431;LP;1;1000;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CurlDetectAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue = (float)pThresholdValue->fltVal;
	int GrayValue = (int)pGrayValue->fltVal;

	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
	double RegionalOut, RegionalIn;
	if(Radius - RoiWidth > 0)
	{
		if(RoiWidth > 0)
		{
			RegionalOut = Radius;
			RegionalIn = Radius - RoiWidth;
		}
		else
		{
			RegionalOut = Radius - RoiWidth;
			RegionalIn = Radius;
		}
	}
	else
	{
		RegionalOut = Radius;
		RegionalIn = 0;
	}
	HTuple WidthOri, HeightOri;
	Hobject Image = m_crtImg;
	get_image_size (m_crtImg, &WidthOri, &HeightOri);
	Hobject ImageScaleMax, JuanYuanPartitioned, ResultDefect;//PolarTransImage
	HTuple PolarTransImageWidth, PolarTransImageHeight, Mean, Deviation, MeanLength, Exp, MeanDeviation;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		polar_trans_image_ext(Image, &ImageScaleMax, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), "nearest_neighbor");
		get_image_size(ImageScaleMax, &PolarTransImageWidth, &PolarTransImageHeight);
		//scale_image_max(PolarTransImage, &ImageScaleMax);
		partition_rectangle(ImageScaleMax, &JuanYuanPartitioned, PolarTransImageWidth/NumPart, PolarTransImageHeight);
		intensity(JuanYuanPartitioned, ImageScaleMax, &Mean, &Deviation);
		tuple_length(Mean, &MeanLength);
		MeanDeviation = HTuple();
		for (int i=0; i<=MeanLength-2; i+=1)
		{
			tuple_deviation(HTuple(Mean[i]).Concat(HTuple(Mean[i+1])), &Exp);
			MeanDeviation[i] = Exp; 
		}

		HTuple Abs, AbsSorted, LocationId, Length, errorValue, ResultID;
		tuple_abs(MeanDeviation, &Abs);
		tuple_sort(Abs, &AbsSorted);
		tuple_sort_index(Abs, &LocationId);
		tuple_length(Abs, &Length);
		for (int i=Length[0].I()-1; i>=0; i+=-1)
		{
			if (0 != (HTuple(AbsSorted[i])<ThresholdValue))
			{
				if (0 != (i==(Length-1)))
				{
					ResultID = HTuple();
					break;
				}
				else
				{
					tuple_last_n(LocationId, i+1, &ResultID);
					break;
				}
			}
		}
		ResultID += 1;

		HTuple MeanSorted, LocationId2, Length1, ResultID2, ResultID3, Sorted, Uniq;
		tuple_sort(Mean, &MeanSorted);
		tuple_sort_index(Mean, &LocationId2);
		tuple_length(Mean, &Length1);
		ResultID2 = HTuple();
		for (int i=0; i<=Length1-1; i+=1)
		{
			if (0 != (HTuple(MeanSorted[i]) < GrayValue))
			{
				ResultID2[i] = HTuple(LocationId2[i]);
			}
			else
			{
				break;
			}
		}
		ResultID2 += 1;

		tuple_concat(ResultID, ResultID2, &ResultID3);
		tuple_sort(ResultID3, &Sorted);
		tuple_uniq(Sorted, &Uniq);
		Hobject BackTransRegion, DefectRegs;
		select_obj(JuanYuanPartitioned, &ResultDefect, Uniq);
		polar_trans_region_inv(ResultDefect, &BackTransRegion, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), WidthOri, HeightOri, "nearest_neighbor");
		union1 (BackTransRegion, &DefectRegs);
		intersection(DefectRegs,rgn,&DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center (DefectRegs, &Area, &Row_test, &Column_test);
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;	
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("402$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 403;LP;5;500;1;300$\
					 404;FP;1;100;0.1;30$\
					 405;LP;1;255;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	//利用带通滤波器的卷边优化算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_dictSpecialData[rId].clear();
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	double FreqStart = (double)pFreqStart->fltVal;
	FreqStart = FreqStart/100;
	double FreqCutoff = (double)pFreqCutoff->fltVal;
	FreqCutoff = FreqCutoff/100;
	int DefectType = (int)pDefectType->fltVal;
	float WaveThreshold = (float)pWaveThreshold->fltVal;
	int MinArea = (int)pMinArea->fltVal;
	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image = m_crtImg;
		Hobject DetectRegion;
		int DetectNumber;
		std::vector<float>DictSpecialData;
		if(H_CurlOptimizeAlgo(Image,rgn,&DetectRegion,fCenterX,fCenterY,RoiWidth, Radius ,FreqStart,FreqCutoff,DefectType, WaveThreshold,MinArea, &DetectNumber,DictSpecialData))
		{
			retValue = DetectNumber; 
			m_vErrorRgn[rId] = DetectRegion;
			if(m_bDebugImg)//显示数据
			{
				bool isSector;
				isSector=true;
				if(m_sectorAngle[rId].m_angelEnd-m_sectorAngle[rId].m_angelStart>359.99)
				{
					isSector=false;
				}
				std::vector<float>DictSpecialDataRota;
				std::vector<float>DictSpecialDataRotaDetectAngle;
				int DataLength=(int)(DictSpecialData.size()-2);
				int angleO=(int)(DataLength*(85.6/360.0));//起始角度不是90度。
				DictSpecialDataRota.push_back(DictSpecialData[0]);
				DictSpecialDataRota.push_back(DictSpecialData[1]);
				for(int i = angleO+1; i >= 2; i--)
				{
					DictSpecialDataRota.push_back(DictSpecialData[i]);
				}
				for(int i =DataLength+1; i >angleO+1; i--)
				{
					DictSpecialDataRota.push_back(DictSpecialData[i]);
				}
				//
				if(isSector)
				{
					if((m_angleDetect>=0)&&(m_angleDetect<6.28))
					{
						angleO=(int)(DataLength*(m_angleDetect/6.283));
					}
					else if((m_angleDetect>=-6.28)&&(m_angleDetect<0))
					{
						angleO=(int)(DataLength*(1+m_angleDetect/6.283));
					}
					else
					{
						angleO=2;
					}

					DictSpecialDataRotaDetectAngle.push_back(DictSpecialData[0]);
					DictSpecialDataRotaDetectAngle.push_back(DictSpecialData[1]);

					for(int i =angleO+1; i <=DataLength+1; i++)
					{
						DictSpecialDataRotaDetectAngle.push_back(DictSpecialDataRota[i]);
					}
					for(int i =2; i<angleO+1; i++)
					{
						DictSpecialDataRotaDetectAngle.push_back(DictSpecialDataRota[i]);
					}

					int startData=(int)((m_sectorAngle[rId].m_angelStart/360.0)*DataLength);
					int endData=(int)((m_sectorAngle[rId].m_angelEnd/360.0)*DataLength);
					if(startData<0)
						startData=0;
					if(startData>=DataLength)
						startData=2;
					if(endData<=startData)
						endData=DataLength;
					if(endData>=DataLength)
						endData=DataLength;
					std::vector<float>DictSpecialSector;
					DictSpecialSector.push_back(DictSpecialData[0]);
					DictSpecialSector.push_back(DictSpecialData[1]);
					for(int i =startData+2; i<endData+2; i++)
					{
						DictSpecialSector.push_back(DictSpecialDataRotaDetectAngle[i]);
					}
					m_dictSpecialData[rId]=DictSpecialSector;
				}
				else
					m_dictSpecialData[rId]=DictSpecialDataRota;//DictSpecialData;
				//


			}
		}
		else
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectNumber);
			return S_FALSE;
		}		
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("380$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 382;FP;1;50;1;2$\
					 383;FP;2;50;1;10$\
					 385;LP;-1;1;1;0$\
					 386;FP;0.1;100.0;0.1;22$\
					 387;LP;1;100;1;2");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::WeldAngleDetectAlgo(VARIANT* rgnId, VARIANT* pDetectAngle)
{
	int rId = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	gen_empty_obj(&m_weldRegions);
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
	if(RoiWidth<11.0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;

		///////////
		Hobject  ImageReduced,RegionDifference, ConnectedRegions, RegionFillUp1;
		Hobject  SelectedRegions, DEMO, PolarTransImage;
		Hobject  DerivGauss, ImageScaleMax, ImageEmphasize1, ImageMean;
		Hobject  Rectangle, Region1, RegionDilation, RegionIntersection,RegionDilation2,RectangleT,ConnectedRegions2,ObjectSelectedE,RegionIntersection1;
		Hobject  ConnectedRegions1, SelectedRegions1, RegionLines,RegionUnion1;
		//
		HTuple   Result_Radius1, Result_Radius2, RadiusResult;
		HTuple  WithResult, HightResult, WithResult_ext, Width1;
		HTuple  Height1, AbsoluteHisto, RelativeHisto, Thred_Result;
		HTuple  Thred_gray, Index1, Area, Row, Column, Area_sort,Row3, Column3,Radius3,Area4, Row4, Column4;
		HTuple  Length, With_Center, Rate_Result, With_Point, Height_Point;

		//***************************************************************************************
		reduce_domain(Image, rgn, &ImageReduced);
		if(Radius<=RoiWidth)
		{
			RoiWidth=(float)(Radius-1.0);
		}
		Result_Radius1 =0;
		Result_Radius2 =RoiWidth;
		RadiusResult = Radius-(RoiWidth)/2.0;

		WithResult = (RadiusResult*3.14159)*2;
		HightResult =RoiWidth;
		WithResult_ext = WithResult*(6.68319/(3.14159*2));
		polar_trans_image_ext(ImageReduced, &PolarTransImage, fCenterY, fCenterX, 0, 6.68319,  Radius-RoiWidth,Radius, WithResult_ext, Result_Radius2-Result_Radius1, "nearest_neighbor");
		get_image_size(PolarTransImage, &Width1, &Height1);
		derivate_gauss(PolarTransImage, &DerivGauss, 2.5, "x");
		scale_image_max(DerivGauss, &ImageScaleMax);
		emphasize(ImageScaleMax, &ImageEmphasize1, 11, 3, 1.6);
		mean_image(ImageEmphasize1, &ImageMean, 3, 21);
		gen_rectangle2(&Rectangle, 0, 0, 0, Width1, Height1);
		gray_histo(Rectangle, ImageMean, &AbsoluteHisto, &RelativeHisto);
		Thred_Result = 0;  
		Thred_gray = 0;
		for (Index1=255; Index1>=0; Index1+=-1)
		{
			Thred_Result += HTuple(RelativeHisto[Index1]);
			if (0 != (Thred_Result>0.004))
			{
				Thred_gray = Index1;
				break;
			}
		}
		threshold(ImageMean, &Region1, Thred_gray, 255);
		dilation_rectangle1(Region1, &RegionDilation, 3, 11);
		intersection(Rectangle, RegionDilation, &RegionIntersection);
		connection(RegionIntersection, &ConnectedRegions1);
		dilation_rectangle1(ConnectedRegions1, &RegionDilation2, 2, HightResult/4.0);
		union1(RegionDilation2, &RegionUnion1);
		connection(RegionUnion1, &ConnectedRegions2);
		//area_center(ConnectedRegions2, &Area, &Row, &Column);
		//tuple_length(Area, &Length);
		//选择最长的特征区域，避免因个别区域比较宽，引起面积较大导致选择错误。
		smallest_circle(ConnectedRegions2, &Row3, &Column3, &Radius3);
		tuple_length(Radius3, &Length);
		tuple_sort_index(Radius3, &Area_sort);
		gen_rectangle1(&RectangleT, (3*Height1)/4, 0, Height1, Width1);
		//如果两个区域相差很小时，选择出两个区域？
		//select_shape(ConnectedRegions2, &SelectedRegions2, "area", "and", HTuple(Area[HTuple(Area_sort[Length-1])])-10, 	99999);
		//直接读取，不用面积选择，避免可能出现的“选择两个区域”情况
		select_obj(ConnectedRegions2, &ObjectSelectedE, HTuple(Area_sort[Length-1])+1);
		intersection(RectangleT, ObjectSelectedE, &RegionIntersection1);
		area_center(RegionIntersection1, &Area4, &Row4, &Column4);
		//With_Center := Column[Area_sort[Length-1]]
		With_Center = Column4;
		if (0 != (With_Center<WithResult))
		{
			Rate_Result = (With_Center/WithResult)*(3.14159*2);
		}
		else if (0 != (With_Center>=WithResult))
		{
			Rate_Result = ((With_Center-WithResult)/WithResult)*(3.14159*2);
		}
		///////////
		With_Point = fCenterX+(180*(Rate_Result.Cos()));

		Height_Point = fCenterY-(180*(Rate_Result.Sin()));
		gen_region_line(&RegionLines, Height_Point,With_Point,  fCenterY, fCenterX);

		m_angleDetect=Rate_Result[0].D();

		if((m_angleDetect>=0)&(m_angleDetect <6.284))
		{
			retValue =180.0*(m_angleDetect/(3.14159));
			m_vErrorRgn[rId] = RegionLines;
			//
			concat_obj(rgn,RegionLines,&rgn);
			m_vRgn[rId]=rgn;
		}
		else
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
		}
		retValue.Detach(pDetectAngle);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldAngleDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("617$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldAngleDetectGrayAlgo(VARIANT* rgnId, VARIANT* pAlgoSelect, VARIANT* pWeldWidth, VARIANT* pSmoothSize, VARIANT* pDetectAngle)
{
	int rId = (int)rgnId->fltVal;
	int RoiWidth =(int) m_ringPara[rId].m_RoiWidth;
	int Radius =(int) m_ringPara[rId].m_Radius;
	int AlgoSelect = (int)pAlgoSelect->fltVal;
	int WeldWidth = (int)pWeldWidth->fltVal;
	int SmoothSize = (int)pSmoothSize->fltVal;
	gen_empty_obj(&m_weldRegions);
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;

		///////////
		Hobject  ImageReduced;
		Hobject  PolarTransImage;
		Hobject  Rectangle;

		//
		HTuple  RadiusResult;
		HTuple  WithResult, HightResult, WithResult_ext, Width1,Height1;
		HTuple  With_Center, Rate_Result, With_Point, Height_Point;

		//***************************************************************************************
		reduce_domain(Image, rgn, &ImageReduced);
		if(Radius<=RoiWidth)
		{
			RoiWidth=(int)(Radius-1.0);
		}
		////////////////////////////////////////////
		RadiusResult =Radius-(RoiWidth)/2.0;
		WithResult = (RadiusResult*3.14159*2).Round();
		HightResult =RoiWidth;
		WithResult_ext = WithResult*(6.9/(3.14159*2));
		polar_trans_image_ext(ImageReduced, &PolarTransImage, fCenterY, fCenterX, 0, 6.9,  Radius-RoiWidth,Radius, WithResult_ext, HightResult, "nearest_neighbor");
		get_image_size(PolarTransImage, &Width1, &Height1);
		//倒数和灰度值定位方法测试、、、、、、、、、、、、、
		Hobject Partitioned;
		HTuple Mean,Deviation,FunctionData,GrayDiff;
		gen_rectangle1(&Rectangle, 0, 0, Height1-1, Width1-1);
		partition_rectangle(Rectangle, &Partitioned, 1, Height1);
		intensity(Partitioned, PolarTransImage, &Mean, &Deviation);
		create_funct_1d_array(Mean, &FunctionData);

		if (0 != (AlgoSelect==0))
		{
			HTuple SmoothedFunction,SmoothedSelected;
			smooth_funct_1d_mean(FunctionData, SmoothSize, 2, &SmoothedFunction);
			tuple_select_range(SmoothedFunction, 3, Width1+2, &SmoothedSelected);
			tuple_sub(Mean, SmoothedSelected, &GrayDiff);
		}
		else
		{
			HTuple Derivative;
			derivate_funct_1d(FunctionData, "first", &Derivative);
			tuple_select_range(Derivative, 3, Width1+2, &GrayDiff);
		}
		HTuple Min,Indices1,LinePlaceX,ClipTuple,Mean1,MeanGray,Indices2;
		tuple_min(GrayDiff, &Min);
		tuple_find(GrayDiff, Min, &Indices1);
		if (0 != (HTuple(Indices1[0])<(WeldWidth*3)))
		{
			LinePlaceX = HTuple(Indices1[0])+WithResult;
		}
		else if (0 != ((HTuple(Indices1[0])+(WeldWidth*3))>WithResult_ext))
		{
			LinePlaceX = HTuple(Indices1[0])-WithResult;
		}
		else
		{
			LinePlaceX = Indices1[0];
		}
		MeanGray = HTuple();
		for (int Index1=-2*WeldWidth; Index1<=2*WeldWidth; Index1+=1)
		{
			tuple_select_range(Mean, (LinePlaceX+Index1)-(WeldWidth/2), ((LinePlaceX+Index1)+(WeldWidth/2))-1, &ClipTuple);
			tuple_mean(ClipTuple, &Mean1);
			MeanGray[Index1+(2*WeldWidth)] = Mean1;
		}
		tuple_sort_index(MeanGray, &Indices2);
		With_Center = (LinePlaceX-(2*WeldWidth))+((((HTuple(Indices2[0])+HTuple(Indices2[1]))+HTuple(Indices2[2]))/3.0).Round());
		/////////////////////////////////////////////////////////////
		if (0 != (With_Center<WithResult))
		{
			Rate_Result = ((With_Center*3.14159*2)/(WithResult+0.000001));
		}
		else if (0 != (With_Center>=WithResult))
		{
			Rate_Result = (((With_Center-WithResult)*3.14159*2)/(WithResult+0.000001));
		}
		///////////
		With_Point = fCenterX+(180*(Rate_Result.Cos()));
		Hobject RegionLines;
		Height_Point = fCenterY-(180*(Rate_Result.Sin()));
		gen_region_line(&RegionLines, Height_Point,With_Point,  fCenterY, fCenterX);

		m_angleDetect=Rate_Result[0].D();

		if((m_angleDetect>=0)&(m_angleDetect <6.284))
		{
			retValue =180.0*(m_angleDetect/(3.14159));
			m_vErrorRgn[rId] = RegionLines;
			//
			concat_obj(rgn,RegionLines,&rgn);
			m_vRgn[rId]=rgn;
		}
		else
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
		}
		retValue.Detach(pDetectAngle);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldAngleDetectGrayAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("690$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B$\
					 691;LP;0;1;1;0$\
					 692;LP;1;50;1;8$\
					 412;LP;1;50;1;8");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::WeldAngleDetectWhiteGlueAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("618$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B$\
					 433;LP;1;250;1;30$\
					 619;FP;0.1;50;0.1;5$\
					 620;LP;1;300;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldAngleDetectWhiteGlueAlgo(VARIANT* rgnId, VARIANT* pDynThreshold, VARIANT* pAspectRationThresh, VARIANT* pClosingMaskWidth, VARIANT* pDetectAngle)
{
	int rId = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	gen_empty_obj(&m_weldRegions);
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
	//
	int DynThreshold = (int)pDynThreshold->fltVal;
	int AspectRationThresh = (int)pAspectRationThresh->fltVal;
	int ClosingMaskWidth = (int)pClosingMaskWidth->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;
		if(Radius<=RoiWidth)
		{
			RoiWidth=(float)(Radius-1.0);
		}
		///////////
		HTuple  OuterRadius, InnerRadius, MiddleRadius, Circumference_ext, Circumference;
		HTuple  CenterRow, CenterColumn, RingWidth; //圆环参数
		HTuple  PolarImageWidth, PolarImageHeight;
		HTuple  MaskScale;
		HTuple  RectRow1,RectCol1, RectRow2, RectCol2; //区域外接最小矩形的相关参数
		HTuple  RectWidth, RectHeight, AspectRatio, Indices;

		Hobject PolarTransImage, ImageScaleMax, ImageMean;
		Hobject RegionDynThresh, ConnectedRegion, SelectedRegion;
		HTuple   ImageHeight, ImageWidth; 
		get_image_size(Image, &ImageWidth, &ImageHeight);
		//***************************************************************************************
		OuterRadius =Radius;
		InnerRadius = Radius-RoiWidth;
		CenterRow = m_vPos[0].m_y;
		CenterColumn =m_vPos[0].m_x;
		MiddleRadius = (OuterRadius + InnerRadius)/2;
		Circumference = MiddleRadius*2*PI;
		Circumference_ext = MiddleRadius * 6.88319;   //设定圆环展开的周长，展开角度大于360度
		RingWidth = OuterRadius - InnerRadius;

		polar_trans_image_ext(Image, &PolarTransImage, CenterRow, CenterColumn, 0, 6.68319, InnerRadius, OuterRadius, Circumference_ext, RingWidth, "nearest_neighbor");
		get_image_size(PolarTransImage, &PolarImageWidth, &PolarImageHeight);
		if(PolarImageHeight<5)
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		//new,用于去除三片罐焊缝区域中间的焊线对定位的影响（2015.07.10修改）
		gray_closing_rect (PolarTransImage, &PolarTransImage, PolarImageHeight*0.2, 5);

		//平滑模版尺寸比例，可兼容大尺寸的图像（如1280*1024）
		MaskScale = (ImageHeight * ImageWidth)/300000.0;
		scale_image_max(PolarTransImage, &ImageScaleMax);

		//修改参数值81为241，用于准确定位焊缝（2015.07.10修改）
		mean_image(ImageScaleMax, &ImageMean, MaskScale * 241, RingWidth*0.75); //25

		dyn_threshold(ImageScaleMax, ImageMean, &RegionDynThresh, DynThreshold, "light");  //可能要设置一个参数？
		//筛掉小的区域
		connection(RegionDynThresh, &ConnectedRegion);
		select_shape(ConnectedRegion, &SelectedRegion, "height", "and", 0.4*PolarImageHeight, PolarImageHeight);
		smallest_rectangle1(SelectedRegion, &RectRow1, &RectCol1, &RectRow2, &RectCol2);
		HTuple Length;
		tuple_length (RectRow2,& Length);
		if(Length<1)
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		RectWidth = (RectCol2 - RectCol1).Abs();
		RectHeight = (RectRow2 - RectRow1).Abs();
		AspectRatio =  1.0*RectWidth/RectHeight;
		tuple_find((AspectRatio - AspectRationThresh).Sgn(), -1, &Indices);  //阈值4可能会设置为调节参数？
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(DynThreshold));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub,Rectangle;
			get_image_size (ImageScaleMax,& Width, &Height);
			gen_rectangle1 (&Rectangle, 0, 0, Height-1, Width-1);
			abs_diff_image (ImageScaleMax, ImageMean, &ImageSub, 1);
			gray_histo (Rectangle, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd* Width* Height<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*(Width[0].D()* Height[0].D()));
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}
		//
		Hobject SelectedRegion1, RegionUnion, RegionClosing;
		Hobject ConnectedRegion1, ResultRegion, ResultRegion1, XYTransRegion, StdXYTransRegion;
		HTuple  WeldPolarCenterRow, WeldPolarCenterCol, WeldArea;
		HTuple  WeldCenterRow, WeldCenterCol, WeldCenterPosRad;

		//去掉横向的“长条”区域
		if (Indices != -1)
		{
			select_obj(SelectedRegion, &SelectedRegion1, Indices + 1);
			union1(SelectedRegion1, &RegionUnion);
			closing_rectangle1(RegionUnion, &RegionClosing, ClosingMaskWidth, 3);  //调节参数？
			connection(RegionClosing, &ConnectedRegion1);
			select_shape_std(ConnectedRegion1, &ResultRegion, "max_area", 0);
			shape_trans(ResultRegion, &ResultRegion1, "rectangle1");

			Hobject WeldCenterLine,StdWeldRect;
			HTuple  ULRow,ULCol,LRRow, LRCol;
			HTuple  WeldAngleDetected, StdWeldWidth, StdWeldAngle;

			//计算焊缝中心线的位置，要用展开的角度（6.68319）来算
			area_center(ResultRegion1, &WeldArea, &WeldPolarCenterRow, &WeldPolarCenterCol);

			WeldCenterPosRad = (WeldPolarCenterCol/Circumference_ext)*6.68319; 
			m_angleDetect = (WeldCenterPosRad.Fmod(2*3.14159)) [0].D();
			WeldCenterRow = CenterRow -(MiddleRadius*(WeldCenterPosRad.Sin()));
			WeldCenterCol = CenterColumn + (MiddleRadius*(WeldCenterPosRad.Cos()));
			gen_region_line(&WeldCenterLine,  WeldCenterRow, WeldCenterCol, CenterRow, CenterColumn);
			retValue =180.0*(m_angleDetect/(3.14159));
			m_vErrorRgn[rId] = WeldCenterLine;
			//
			concat_obj(rgn,WeldCenterLine,&rgn);
			//			m_vRgn[rId]=rgn;
			//计算焊缝区域边界及其对应的焊缝中心点，起始角度、终止角度,2018.05.06, LukeChen添加
			/*			Hobject RegionBorder;
			HTuple  BorderRows, BorderCols;

			boundary(ResultRegion, &RegionBorder, "inner");
			get_region_points(RegionBorder, &BorderRows, &BorderCols);

			//根据ROI宽度来确定相关的节点线， >12时，选取1/3, 2/3处，小于12时，选取首尾点
			int FirstRowVal, SecondRowVal;

			if (RoiWidth >= 12.0)
			{
			FirstRowVal = (int)(RoiWidth/3.0);
			SecondRowVal =  (int)(RoiWidth*2.0/3.0);
			} 
			else
			{
			FirstRowVal = 0;
			SecondRowVal =  PolarImageHeight[0].I() -1;
			}


			HTuple Indices1, Indices2, Indices3, Indices4;
			tuple_find(BorderRows, FirstRowVal, &Indices1);
			tuple_find(BorderRows, SecondRowVal, &Indices2);

			if (Indices1.Num() > 2)
			{
			Indices1 = (Indices1.Select(0)).Concat(Indices1.Select(Indices1.Num() -1));
			}

			if (Indices2.Num() > 2)
			{
			Indices2 = (Indices2.Select(0)).Concat(Indices2.Select(Indices2.Num() -1));
			}

			tuple_sort_index(BorderCols.Select(Indices1), &Indices3);
			tuple_sort_index(BorderCols.Select(Indices2), &Indices4);

			HTuple PointRows1, PointCols1, Rho1;
			PointRows1.Reset();
			PointRows1.Append(BorderRows.Select(Indices1.Select(Indices3)));
			PointRows1.Append(BorderRows.Select(Indices2.Select(Indices4)));

			PointCols1.Reset();
			PointCols1.Append(BorderCols.Select(Indices1.Select(Indices3)));
			PointCols1.Append(BorderCols.Select(Indices2.Select(Indices4)));

			Rho1 = ((1.0*PointCols1)/PolarImageWidth)*6.68319;  //6.68319与polar_trans_image_ext保持一致

			//边缘点在原始图片上的位置
			HTuple PointRowsInOrigImg1, PointColsInOrigImg1;
			PointRowsInOrigImg1 = CenterRow-((OuterRadius-PointRows1)*(Rho1.Sin()));
			PointColsInOrigImg1 = CenterColumn+((OuterRadius-PointRows1)*(Rho1.Cos()));

			//求焊缝区域边界的交点
			HTuple WeldLineIntersectPointRow, WeldLineIntersectPointCol, IsLineOverlapping;
			intersection_lines(PointRowsInOrigImg1.Select(0), PointColsInOrigImg1.Select(0), 
			PointRowsInOrigImg1.Select(2), PointColsInOrigImg1.Select(2), PointRowsInOrigImg1.Select(1), 
			PointColsInOrigImg1.Select(1), PointRowsInOrigImg1.Select(3), PointColsInOrigImg1.Select(3), 
			&WeldLineIntersectPointRow, &WeldLineIntersectPointCol, &IsLineOverlapping);


			//////////////////////////////////////////////////////////////////////////
			Hobject PointCross;
			gen_cross_contour_xld(&PointCross, PointRowsInOrigImg1, PointColsInOrigImg1,  20, HTuple(45).Rad());
			concat_obj(rgn,PointCross,&rgn);

			Hobject Line1, Line2;
			gen_region_line(&Line1, WeldLineIntersectPointRow, WeldLineIntersectPointCol, PointRowsInOrigImg1.Select(0), PointColsInOrigImg1.Select(0));
			gen_region_line(&Line2, WeldLineIntersectPointRow, WeldLineIntersectPointCol, PointRowsInOrigImg1.Select(1), PointColsInOrigImg1.Select(1));
			concat_obj(rgn,Line1,&rgn);
			concat_obj(rgn,Line2,&rgn);
			//////////////////////////////////////////////////////////////////////////
			*/

			/*			HTuple RegRun_Row, RegRun_ColBegin, RegRun_ColEnd;
			get_region_runs(ResultRegion, &RegRun_Row, &RegRun_ColBegin, &RegRun_ColEnd);

			Hobject LeftContour, RightContour;
			Hobject LeftLineFitted, RightLineFitted;

			HTuple  LeftLineRowBegion, LeftLineColBegion, LeftLineRowEnd, LeftLineColEnd;
			HTuple  hv_Nr, hv_Nc, hv_Dist;
			HTuple  RightLineRowBegion, RightLineColBegion, RightLineRowEnd, RightLineColEnd;
			HTuple  hv_Nr1, hv_Nc1, hv_Dist1;

			gen_contour_polygon_xld(&LeftContour, RegRun_Row, RegRun_ColBegin);
			fit_line_contour_xld(LeftContour, "tukey", -1, 0, 5, 2, &LeftLineRowBegion, &LeftLineColBegion, &LeftLineRowEnd, &LeftLineColEnd, &hv_Nr, &hv_Nc, &hv_Dist);

			gen_contour_polygon_xld(&RightContour, RegRun_Row, RegRun_ColEnd);
			fit_line_contour_xld(RightContour, "tukey", -1, 0, 5, 2, &RightLineRowBegion, &RightLineColBegion, &RightLineRowEnd, &RightLineColEnd, &hv_Nr1, &hv_Nc1, &hv_Dist1);

			//转换到原始图片中
			HTuple PointRows1, PointCols1, Rho1;
			PointRows1.Reset();
			PointRows1.Append(LeftLineRowBegion);
			PointRows1.Append(LeftLineRowEnd);
			PointRows1.Append(RightLineRowBegion);
			PointRows1.Append(RightLineRowEnd);

			PointCols1.Reset();
			PointCols1.Append(LeftLineColBegion);
			PointCols1.Append(LeftLineColEnd);
			PointCols1.Append(RightLineColBegion);
			PointCols1.Append(RightLineColEnd);

			Rho1 = ((1.0*PointCols1)/PolarImageWidth)*6.68319;  //6.68319与polar_trans_image_ext保持一致

			//边缘点在原始图片上的位置
			HTuple PointRowsInOrigImg1, PointColsInOrigImg1;
			PointRowsInOrigImg1 = CenterRow-((OuterRadius-PointRows1)*(Rho1.Sin()));
			PointColsInOrigImg1 = CenterColumn+((OuterRadius-PointRows1)*(Rho1.Cos()));

			HTuple WeldLineIntersectPointRow, WeldLineIntersectPointCol, IsLineOverlapping;
			intersection_lines(PointRowsInOrigImg1.Select(0), PointColsInOrigImg1.Select(0), 
			PointRowsInOrigImg1.Select(1), PointColsInOrigImg1.Select(1), PointRowsInOrigImg1.Select(2), 
			PointColsInOrigImg1.Select(2), PointRowsInOrigImg1.Select(3), PointColsInOrigImg1.Select(3), 
			&WeldLineIntersectPointRow, &WeldLineIntersectPointCol, &IsLineOverlapping);

			//////////////////////////////////////////////////////////////////////////
			Hobject Line1, Line2;
			gen_region_line(&Line1, WeldLineIntersectPointRow, WeldLineIntersectPointCol, PointRowsInOrigImg1.Select(0), PointColsInOrigImg1.Select(0));
			gen_region_line(&Line2, WeldLineIntersectPointRow, WeldLineIntersectPointCol, PointRowsInOrigImg1.Select(2), PointColsInOrigImg1.Select(2));
			concat_obj(rgn,Line1,&rgn);
			concat_obj(rgn,Line2,&rgn);

			Hobject Cross1;
			gen_cross_contour_xld(&Cross1, PointRowsInOrigImg1, PointColsInOrigImg1, 16, HTuple(45).Rad());
			concat_obj(rgn,Cross1,&rgn);
			//////////////////////////////////////////////////////////////////////////
			*/			
			m_vRgn[rId]=rgn;

		}
		else
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
		}
		retValue.Detach(pDetectAngle);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldAngleDetectGlueAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("937$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B$\
					 385;LP;-1;1;1;1$\
					 197;LP;0;21;1;4$\
					 433;LP;1;250;1;30$\
					 619;FP;0.1;50;0.1;5$\
					 620;LP;1;300;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldAngleDetectGlueAlgo(VARIANT* rgnId, VARIANT* pBlackWhite,VARIANT* pEnhancePara,VARIANT* pDynThreshold, VARIANT* pAspectRationThresh, VARIANT* pClosingMaskWidth, VARIANT* pDetectAngle)
{
	int rId = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	gen_empty_obj(&m_weldRegions);
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
	//
	int BlackWhite = (int)pBlackWhite->fltVal;
	int EnhancePara = (int)pEnhancePara->fltVal;
	int DynThreshold = (int)pDynThreshold->fltVal;
	int AspectRationThresh = (int)pAspectRationThresh->fltVal;
	int ClosingMaskWidth = (int)pClosingMaskWidth->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;
		if(Radius<=RoiWidth)
		{
			RoiWidth=(float)(Radius-1.0);
		}
		///////////
		HTuple  OuterRadius, InnerRadius, MiddleRadius, Circumference_ext, Circumference;
		HTuple  CenterRow, CenterColumn, RingWidth; //圆环参数
		HTuple  RectRow1,RectCol1, RectRow2, RectCol2; //区域外接最小矩形的相关参数
		HTuple  RectWidth, RectHeight, AspectRatio, Indices;


		HTuple   ImageHeight, ImageWidth; 
		get_image_size(Image, &ImageWidth, &ImageHeight);
		//***************************************************************************************
		OuterRadius =Radius;
		InnerRadius = Radius-RoiWidth;
		if(RoiWidth<5)
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		CenterRow = m_vPos[0].m_y;
		CenterColumn =m_vPos[0].m_x;
		MiddleRadius = (OuterRadius + InnerRadius)/2;
		Circumference = MiddleRadius*2*PI;
		double AngleExt=7.065;
		Circumference_ext = MiddleRadius *AngleExt;   //设定圆环展开的周长，展开角度大于360度
		RingWidth = OuterRadius - InnerRadius;
		//
		Hobject Circle1,Circle2,RegionDifference,RegionDynThresh;
		Hobject PolarTransRegion,RegionFillUp1,RegionClosing,ConnectedRegion, SelectedRegion,RegionOpening;
		Hobject ImageReduced,ImageMean,ImageResult,ImageAffinTrans;
		HTuple Mean, Deviation;
		gen_circle(&Circle1, CenterRow, CenterColumn, InnerRadius);
		gen_circle(&Circle2,CenterRow, CenterColumn, OuterRadius);
		difference(Circle2, Circle1, &RegionDifference);
		reduce_domain(Image, RegionDifference, &ImageReduced);
		//*
		mean_image(ImageReduced, &ImageMean, 3, 3);
		intensity(RegionDifference, ImageMean, &Mean, &Deviation);
		//*
		HTuple GammaLUT,HomMat2D;
		GammaLUT = HTuple();
		HTuple NewGray;
		NewGray = 0;
		for (int G=0; G<=255; G+=1)
		{
			NewGray = (((G/Mean).Pow(EnhancePara))*G).Round();
			if (0 != (NewGray>255))
			{
				NewGray = 255;
			}
			GammaLUT = GammaLUT.Concat(NewGray);
		}
		lut_trans(ImageMean, &ImageResult, GammaLUT);
		//*
		vector_angle_to_rigid(CenterRow, CenterColumn,0, CenterRow, CenterColumn, HTuple(45).Rad(), &HomMat2D);
		affine_trans_image(ImageResult, &ImageAffinTrans, HomMat2D, "constant", "false");
		if (BlackWhite==1)
		{
			dyn_threshold(ImageResult, ImageAffinTrans, &RegionDynThresh, DynThreshold, "light");
		}
		else if (BlackWhite==-1)
		{
			dyn_threshold(ImageResult, ImageAffinTrans, &RegionDynThresh, DynThreshold, "dark");
		}
		else if (BlackWhite==0)
		{
			dyn_threshold(ImageResult, ImageAffinTrans, &RegionDynThresh, DynThreshold, "not_equal");
		}
		else
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		//
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(DynThreshold));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple AbsoluteHisto, RelativeHisto, Length,rateMax,RegionArea,Row12,Col12;
			Hobject ImageSub;
			area_center(RegionDifference,&RegionArea,&Row12,&Col12);
			abs_diff_image (ImageResult, ImageAffinTrans, &ImageSub, 1);
			gray_histo (RegionDifference, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd*RegionArea[0].D()<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*RegionArea[0].D());
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}
		//
		polar_trans_region(RegionDynThresh, &PolarTransRegion,  CenterRow, CenterColumn, 0, AngleExt, InnerRadius, OuterRadius, Circumference_ext, RingWidth, "nearest_neighbor");
		fill_up(PolarTransRegion, &RegionFillUp1);	
		//用于去除三片罐焊缝区域中间的焊线(可能存在的黑条)对定位的影响
		if (ClosingMaskWidth<=5)
		{
			closing_rectangle1(RegionFillUp1, &RegionClosing, 1, 3);
		}
		else
		{
			closing_rectangle1(RegionFillUp1, &RegionClosing, ClosingMaskWidth*0.2, 3);
		}
		//筛掉小的区域
		connection(RegionClosing, &ConnectedRegion);
		select_shape(ConnectedRegion, &SelectedRegion, "height", "and", 0.6*RingWidth, RingWidth+8);
		if (ClosingMaskWidth<2)
		{
			ClosingMaskWidth=2;
			opening_rectangle1(SelectedRegion, &RegionOpening,1, 0.6*RingWidth);
		}
		else
		{
			opening_rectangle1(SelectedRegion, &RegionOpening, ClosingMaskWidth*0.6, 0.6*RingWidth);
		}
		smallest_rectangle1(RegionOpening, &RectRow1, &RectCol1, &RectRow2, &RectCol2);
		HTuple Length;
		tuple_length (RectRow2,& Length);
		if(Length<1)
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		RectWidth = (RectCol2 - RectCol1).Abs();
		RectHeight = (RectRow2 - RectRow1).Abs();
		AspectRatio =  1.0*RectWidth/RectHeight;
		tuple_find((AspectRatio - AspectRationThresh).Sgn(), -1, &Indices);  //阈值4可能会设置为调节参数？
		Hobject SelectedRegion1, RegionUnion, RegionClosing2;
		Hobject ConnectedRegion1, ResultRegion, XYTransRegion, StdXYTransRegion;
		HTuple  WeldPolarCenterRow, WeldPolarCenterCol, WeldArea;
		HTuple  WeldCenterRow, WeldCenterCol, WeldCenterPosRad;

		//去掉横向的“长条”区域
		if (Indices != -1)
		{
			select_obj(RegionOpening, &SelectedRegion1, Indices + 1);
			union1(SelectedRegion1, &RegionUnion);
			closing_rectangle1(RegionUnion, &RegionClosing2, ClosingMaskWidth*0.6, 3);  //调节参数？
			connection(RegionClosing2, &ConnectedRegion1);
			select_shape_std(ConnectedRegion1, &ResultRegion, "max_area", 0);
			shape_trans(ResultRegion, &ResultRegion, "rectangle1");

			Hobject WeldCenterLine,StdWeldRect;
			HTuple  ULRow,ULCol,LRRow, LRCol;
			HTuple  WeldAngleDetected, StdWeldWidth, StdWeldAngle;

			//计算焊缝中心线的位置，要用展开的角度（AngleExt）来算
			area_center(ResultRegion, &WeldArea, &WeldPolarCenterRow, &WeldPolarCenterCol);

			WeldCenterPosRad = (WeldPolarCenterCol/Circumference_ext)*AngleExt; 
			m_angleDetect = (WeldCenterPosRad.Fmod(2*3.14159)) [0].D();
			WeldCenterRow = CenterRow -(MiddleRadius*(WeldCenterPosRad.Sin()));
			WeldCenterCol = CenterColumn + (MiddleRadius*(WeldCenterPosRad.Cos()));
			gen_region_line(&WeldCenterLine,  WeldCenterRow, WeldCenterCol, CenterRow, CenterColumn);
			retValue =180.0*(m_angleDetect/(3.14159));
			m_vErrorRgn[rId] = WeldCenterLine;
			//
			concat_obj(rgn,WeldCenterLine,&rgn);
			m_vRgn[rId]=rgn;
		}
		else
		{
			retValue =-1;
			m_vErrorRgn[rId] = rgn;
		}
		retValue.Detach(pDetectAngle);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CCanAlgo::WeldWhiteGlueRegExtractAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pWeldRegArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	gen_empty_obj(&m_weldRegions);
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pWeldRegArea);
		return S_FALSE;
	}

	int BlackMaskSize = (int)pBlackMaskSize->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;
		Hobject ImageReduced, WeldRegTemp, WeldRegTempFillUp;
		HTuple  UsedThreshold;
		Hobject WeldRegTempClosing;

		HTuple  WeldRegArea;

		reduce_domain(Image, rgn, &ImageReduced);
		binary_threshold(ImageReduced, &WeldRegTemp, "smooth_histo", "light", &UsedThreshold);
		fill_up(WeldRegTemp, &WeldRegTempFillUp);
		closing_circle(WeldRegTempFillUp, &WeldRegTempClosing, BlackMaskSize);
		//		opening_circle(WeldRegTempClosing, &WeldRegTempClosing, BlackMaskSize);
		union1(WeldRegTempClosing, &WeldRegTempClosing);

		region_features(WeldRegTempClosing, "area", &WeldRegArea);

		retValue = WeldRegArea[0].I();

		Hobject WeldNeighborReg;
		difference(rgn, WeldRegTempClosing, &WeldNeighborReg);

		HTuple DilationRadius = 3.5;
		Hobject Temp1, Temp2;
		Hobject WeldBoundary;

		dilation_circle(WeldRegTempClosing, &Temp1, DilationRadius);
		dilation_circle(WeldNeighborReg, &Temp2, DilationRadius);
		intersection(Temp1, Temp2, &WeldBoundary);
		intersection(WeldBoundary, rgn, &WeldBoundary);

		//		m_vErrorRgn[rId] = WeldRegTempClosing;
		concat_obj(m_vRgn[rId], WeldRegTempClosing, &m_vRgn[rId]);	//显示定位结果

		m_WeldLocatedAlgoRegIndex = rId;
		gen_empty_obj(&m_WeldRegRelated[rId]);
		concat_obj(m_WeldRegRelated[rId], WeldRegTempClosing, &m_WeldRegRelated[rId]);
		concat_obj(m_WeldRegRelated[rId], WeldNeighborReg, &m_WeldRegRelated[rId]);
		concat_obj(m_WeldRegRelated[rId], WeldBoundary, &m_WeldRegRelated[rId]);
		retValue.Detach(pWeldRegArea);		

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pWeldRegArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldWhiteGlueRegExtractAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1323$\
					 1324;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 412;LP;3;101;2;7");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::WeldRegSelectAlgo(VARIANT* rgnId, VARIANT* pWeldSelection)
{
	int id = (int)rgnId->fltVal;
	Hobject newRgn;
	int RegIndex = (int)pWeldSelection->fltVal;

#ifdef NDEBUG
	try
	{
#endif

		if (m_WeldLocatedAlgoRegIndex == -1)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}

		Hobject SelectReg;
		select_obj(m_WeldRegRelated[m_WeldLocatedAlgoRegIndex], &SelectReg, RegIndex+1);

		m_vRgn[id] = SelectReg ;	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::WeldRegSelectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1325$\
					 167;B$\
					 1326;LP;0;1;1;0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CCanAlgo::BlackDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	int SeriousBlackPointDynThresh = (int)pSeriousBlackPointDynThresh->fltVal;
	int BlackThresh = (int)pBlackThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		//
		Hobject  ImageOpening;
		Hobject  RegionDynThresh;
		Hobject  RegionBlack, RegionUnion, ConnectedRegions,SelectedRegions;
		reduce_domain(Image, rgn, &ImageReduced);
		gray_opening_rect(ImageReduced,&ImageOpening,7,7);
		if(BlackMaskSize>=0.5)
			median_image(ImageOpening, &ImageReduceMean, "circle", BlackMaskSize, "mirrored");
		else
			ImageReduceMean=ImageOpening;

		dyn_threshold(ImageReduced, ImageReduceMean, &RegionDynThresh, SeriousBlackPointDynThresh, "dark");
		threshold(ImageReduced, &RegionBlack, 0, BlackThresh);
		union2(RegionDynThresh, RegionBlack, &RegionUnion);
		connection(RegionUnion, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, 9999999);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			sub_image (ImageReduceMean,ImageReduced,  &ImageSub, 1, 0);
			gray_histo (rgn, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd* Width* Height<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*(Width[0].D()* Height[0].D()));
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;	
		union1(SelectedRegions, &DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::BlackDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("492$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 412;LP;1;101;2;3$\
					 415;LP;1;250;1;40$\
					 198;LP;1;250;1;60$\
					 416;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	float MaskSize = (float)fabs(pMaskSize->fltVal);
	float SmoothSize = (float) fabs(pSmoothSize->fltVal);
	int DefectType = (int)pDefectType->fltVal;
	float SeriousBlackPointDynThresh = (float)(pSeriousBlackPointDynThresh->fltVal/100.0);
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	float SeriousWhitePointDynThresh = (float) (-pSeriousWhitePointDynThresh->fltVal/100.0);
	int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean,DerivGauss,DerivGauss1;
		Hobject BlackRegion,ConnectedRegions1,SelectedRegions1,WhiteRegion,ConnectedRegions2,SelectedRegions2,BlackUnion, WhiteUnion;
		//
		reduce_domain (Image,rgn, &ImageReduced);	
		derivate_gauss(ImageReduced, &DerivGauss, MaskSize, "kitchen_rosenfeld");
		derivate_gauss(DerivGauss, &DerivGauss1, SmoothSize, "none");

		/*if (m_bDebugImg)
		{
		m_dictSpecialData[rId].push_back(float(SeriousBlackPointDynThresh));
		m_dictSpecialData[rId].push_back(float(SeriousWhitePointDynThresh));
		HTuple AbsoluteHisto, RelativeHisto, Length;
		gray_histo (rgn, DerivGauss1, &AbsoluteHisto, &RelativeHisto);
		HTuple  maxValue = 0;
		tuple_max (AbsoluteHisto, &maxValue);
		tuple_length (AbsoluteHisto, &Length);
		float YRange = 100;
		float YScaleFactor = float(YRange/maxValue[0].D());
		int histoLength = Length[0].I();
		for (int i=0; i< histoLength; i++)
		{
		m_dictSpecialData[rId].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
		}
		}*/

		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;
		if (DefectType==-1)
		{
			//*找单一块极黑缺陷，适用于伤的比较深，污点比较黑的情况
			threshold(DerivGauss1, &BlackRegion,SeriousBlackPointDynThresh , 100);
			connection(BlackRegion, &ConnectedRegions1);
			select_shape(ConnectedRegions1, &SelectedRegions1, "area", "and", SeriousBlackPointSize, 99999);
			union1(SelectedRegions1,&DefectRegs);
		}
		else if (DefectType==1)
		{
			//*极白点
			threshold(DerivGauss1, &WhiteRegion, -100,SeriousWhitePointDynThresh);
			connection(WhiteRegion, &ConnectedRegions2);
			select_shape(ConnectedRegions2, &SelectedRegions2, "area", "and",SeriousWhitePointSize, 99999);
			union1(SelectedRegions2,&DefectRegs);
		}
		else
		{
			//*找单一块极黑缺陷，适用于伤的比较深，污点比较黑的情况
			threshold(DerivGauss1, &BlackRegion,SeriousBlackPointDynThresh , 100);
			connection(BlackRegion, &ConnectedRegions1);
			select_shape(ConnectedRegions1, &SelectedRegions1, "area", "and", SeriousBlackPointSize, 99999);
			union1(SelectedRegions1,&BlackUnion);
			//*极白点
			threshold(DerivGauss1, &WhiteRegion, -100,SeriousWhitePointDynThresh);
			connection(WhiteRegion, &ConnectedRegions2);
			select_shape(ConnectedRegions2, &SelectedRegions2, "area", "and",SeriousWhitePointSize, 99999);
			union1(SelectedRegions2,&WhiteUnion);
			union2(BlackUnion, WhiteUnion, &DefectRegs);
		}
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("678$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 679;FP;0.1;5.0;0.1;1.0$\
					 680;FP;0.1;5.0;0.1;1.0$\
					 385;LP;-1;1;1;-1$\
					 407;LP;1;1000;1;200$\
					 408;LP;1;200;1;10$\
					 409;LP;1;1000;1;200$\
					 410;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::VarThreshDarkDetectAlgo(VARIANT* rgnId, VARIANT* pWindowWidth, VARIANT* pGrayThresh, VARIANT* pAreaFilter, VARIANT* pDetectArea)
{
	int rId                = (int)rgnId->fltVal;
	int WindowWidth        = (int)pWindowWidth->fltVal;
	int WindowHeight       =  WindowWidth;
	int GrayThresh        = (int)pGrayThresh->fltVal;
	int AreaFilter         = (int)pAreaFilter->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject DefectRegs,ImageReduce;
	Hobject ResultRegionBlack,SelectedRegions,ConnectedRegions;
	HTuple hv_Area, hv_Row, hv_Column;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}

		Hobject rgn = m_vRgn[rId];
		HTuple area,row,col;
		union1(rgn,&rgn);
		area_center(rgn,&area,&row,&col);
		if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image = m_crtImg;
		HTuple hv_width,hv_height;
		get_image_size(Image,&hv_width,&hv_height);
		Hobject Region;
		if(hv_width<=WindowWidth)
		{
			WindowWidth=hv_width[0].I();
		}
		if(hv_height<=WindowHeight)
		{
			WindowHeight=hv_height[0].I();
		}
		reduce_domain(Image,rgn,&ImageReduce);
		var_threshold (ImageReduce, &ResultRegionBlack, WindowWidth, WindowHeight, 0.2,GrayThresh, "dark");
		connection (ResultRegionBlack, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions,"area", "and", AreaFilter, 9999999);
		union1(SelectedRegions,&DefectRegs);
		area_center(DefectRegs, &hv_Area, &hv_Row, &hv_Column);
		if(hv_Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = hv_Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 3.5);
			m_vErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::VarThreshDarkDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1290$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 412;LP;1;1000;1;150$\
					 314;LP;1;255;1;20$\
					 434;LP;1;9999999;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CCanAlgo::GLineAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
{
	//盖面找线算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue         = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	float BlackMaskSize = (float)pBlackMaskSize->fltVal;
	double BlackThredLow = (double)pBlackThredLow->fltVal;
	int BlackDefectSize = (int)pBlackDefectSize->fltVal;
	float WhiteMaskSize = (float)pWhiteMaskSize->fltVal;
	double WhiteThredLow = (double)pWhiteThredLow->fltVal;
	int WhiteDefectSize = (int)pWhiteDefectSize->fltVal;
	int BlackThredUpper = 8;
	int WhiteThredUpper = 8;
	int BlackDefectSizeUpper = 200;
	int WhiteDefectSizeUpper = 200;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ExpandedImage, DerivGauss, rgnErosion;
		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);
		//*===================================================================
		Hobject BlackLines, BlackUnion, WhiteLines, WhiteUnion;	
		reduce_domain(ExpandedImage, rgn, &ImageReduced);
		erosion_circle(rgn, &rgnErosion, 2.5);
		lines_gauss(ImageReduced, &BlackLines, BlackMaskSize, BlackThredLow, BlackThredUpper, "dark", "true", "bar-shaped", "true");
		ImageProcessAlg.LinesToRegion(BlackLines, BlackDefectSize, BlackDefectSizeUpper, BlackUnion);
		intersection (rgnErosion, BlackUnion, &BlackUnion);

		lines_gauss(ImageReduced, &WhiteLines, WhiteMaskSize, WhiteThredLow, WhiteThredUpper, "light", "true", "bar-shaped", "true");
		ImageProcessAlg.LinesToRegion(WhiteLines, WhiteDefectSize, WhiteDefectSizeUpper, WhiteUnion);
		intersection (rgnErosion, WhiteUnion, &WhiteUnion);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;
		union2(BlackUnion, WhiteUnion, &DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);

		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::GLineAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("425$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 426;FP;0.1;20;0.1;1.1$\
					 427;FP;0;8;0.5;3$\
					 428;LP;1;200;1;10$\
					 429;FP;0.1;20;0.1;1.0$\
					 430;FP;0;8;0.5;3$\
					 431;LP;1;200;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int EdgeThred = (int)pEdgeThred->fltVal;
	int DefectSize = (int)pDefectSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ExpandedImage, DerivGauss;
		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);

		Hobject ImageMean, ImageExpandedReduced;
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(ExpandedImage, &ImageMean, MaskSize, MaskSize);
		reduce_domain(ImageMean, rgn, &ImageExpandedReduced);

		Hobject ImageEdgeRobinson, ThresholdRegion, ConnectedThresholdRegion, SelectedRegion;
		robinson_amp(ImageExpandedReduced, &ImageEdgeRobinson);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(EdgeThred));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple AbsoluteHisto, RelativeHisto, Length;
			gray_histo (rgn, ImageEdgeRobinson, &AbsoluteHisto, &RelativeHisto);
			HTuple  maxValue = 0;
			tuple_max (AbsoluteHisto, &maxValue);
			tuple_length (AbsoluteHisto, &Length);
			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			int histoLength = Length[0].I();
			for (int i=0; i< histoLength; i++)
			{
				m_dictSpecialData[rId].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
			}
		}
		threshold(ImageEdgeRobinson, &ThresholdRegion, EdgeThred, 255);
		connection(ThresholdRegion, &ConnectedThresholdRegion);
		select_shape(ConnectedThresholdRegion, &SelectedRegion, "area", "and", DefectSize, 99999);
		Hobject DefectRegs;
		union1(SelectedRegion, &DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
		}	

		retValue.Detach(pDetectArea);




#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("432$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 433;LP;1;255;1;30$\
					 434;LP;1;250;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
	Hobject RegionDilat;
	HTuple MeanGray, Deviation;//pDetectArea

#ifdef NDEBUG
	try
	{
#endif
		//防止多个区域，如CenterLocationAlgoUsePointFitting算法会显示边界点
		HTuple ObjNum;
		count_obj(rgn, &ObjNum);
		if (ObjNum > 1)
		{
			select_obj(rgn, &rgn, 1);
		}

		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image = m_crtImg;
		intensity (rgn, Image, &MeanGray, &Deviation);

		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(MeanGray[0].D()));
			m_dictSpecialData[rId].push_back(-999999.0f);

			HTuple AbsoluteHisto, RelativeHisto, Length, maxValue = 0;
			gray_histo (rgn, Image, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(AbsoluteHisto, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			int histoLength = Length[0].I();
			for (int i = 0; i < histoLength; i ++)
			{
				m_dictSpecialData[rId].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
			}
		}
		retValue = MeanGray[0].I();
		dilation_circle (rgn, &RegionDilat, 1.5);
		m_vErrorRgn[rId] = RegionDilat;
		retValue.Detach(pGrayValueMean);
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1;<=#150$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

bool CCanAlgo::ReadDatasForDirection( string bstrDataPath,bool errorInfoLanguage,Hobject &MapImg,Hobject& MulImg,Hobject &ModelRegion,Hobject &OrgModelRegion,HTuple& ModelNum,HTuple& AngleStart,HTuple& AngleExtent,HTuple& Models,HTuple& ModelParaOfSelfLearning)
{
	//
	gen_empty_obj(&MapImg);
	gen_empty_obj(&MulImg);
	gen_empty_obj(&ModelRegion);
	gen_empty_obj(&OrgModelRegion);
	ModelNum = HTuple();
	AngleStart = HTuple();
	AngleExtent = HTuple();
	Models = HTuple();
	ModelParaOfSelfLearning = HTuple();
	//
	string strMapPath;
	string strMulImgPath;
	string strModelFolderPath;
	string strTempImgFolderPath;
	string strModelPath;
	string strModelRegionPath;
	string strModelParaPath;
	strMapPath=bstrDataPath+"\\Map.tiff";
	strMulImgPath=bstrDataPath+"\\MulImg.tiff";
	strModelRegionPath=bstrDataPath+ "\\Modelling.reg" ;	
	strModelParaPath=bstrDataPath+ "\\ModelPara.tup" ;	
	strModelFolderPath=bstrDataPath+"\\Models\\";
	strTempImgFolderPath=bstrDataPath+ "\\TemplateImage\\" ;	
	//读取映射图像
	if((_access(strMapPath.c_str(),0))||(_access(strMulImgPath.c_str(),0)))
	{
		if(errorInfoLanguage)
		{
			MessageBox(NULL,_T("棋盘格相关图像缺失，请使用训练算子"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Missing CheckerBoard Related Image, please use training operator"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		return 0;
	}
	else
	{
		read_image(&MulImg, HTuple(strMulImgPath.c_str()));
		read_image(&MapImg, HTuple(strMapPath.c_str()));
	}
	//
	if(_access(strModelParaPath.c_str(),0))
	{
		if(errorInfoLanguage)
		{
			MessageBox(NULL,_T("模板相关参数缺失,使用默认值"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Missing model parameter file, use the default parameter."),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}

	}
	else
	{
		read_tuple(HTuple(strModelParaPath.c_str()),&ModelParaOfSelfLearning);
	}
	//判断模板图片文件夹是否存在
	if(_access(strTempImgFolderPath.c_str(),0) )
	{
		if(errorInfoLanguage)
		{
			MessageBox(NULL,_T("图像文件夹读取错误"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Image Folder Reading Error"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		return 0;
	} 
	//读取模板区域
	if(_access(strModelRegionPath.c_str(),0) )
	{
		if(errorInfoLanguage)
		{
			MessageBox(NULL,_T("模板区域读取错误"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Model Region Reading Error"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		return 0;
	} 
	else
	{
		read_region(&ModelRegion,HTuple(strModelRegionPath.c_str()));

	}
	//原图像对应的模板区域
	Hobject MapRow, MapCol;
	vector_field_to_real (MapImg, &MapRow, &MapCol);
	HTuple  Rows, Columns,RowGrayval,ColGrayval;
	get_region_points (ModelRegion,&Rows,&Columns);
	get_grayval (MapRow, Rows, Columns, &RowGrayval);
	get_grayval (MapCol, Rows, Columns, &ColGrayval);
	gen_region_points (&OrgModelRegion, RowGrayval, ColGrayval);
	//
	//读取模板组（先判断模板文件夹是否存在）
	if(_access(strModelFolderPath.c_str(),0) )
	{
		if(errorInfoLanguage)
		{
			MessageBox(NULL,_T("模板文件夹读取错误"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Model Folder Reading Error"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		return 0;
	}
	else
	{
		HTuple  ModelID;
		HTuple  ModelFiles;
		list_files(strModelFolderPath.c_str(), (HTuple("files").Append("follow_links")), &ModelFiles);
		tuple_regexp_select(ModelFiles, (HTuple("\\.(shm)$").Append("ignore_case")), &ModelFiles);
		ModelNum=HTuple(ModelFiles.Num());
		if(ModelNum<1)
		{
			if(errorInfoLanguage)
			{
				MessageBox(NULL,_T("模板不存在"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("No Model"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			return 0;
		}
		for (int Index=0; Index <= ModelFiles.Num() -1; Index+=1)
		{
			read_shape_model(HTuple(ModelFiles[Index]), &ModelID);
			Models.Append(ModelID);
			if(Index==0)
			{
				HTuple  NumLevels, AngleStep, ScaleMin, ScaleMax, ScaleStep,Metric, MinContrast;
				get_shape_model_params(ModelID, &NumLevels, &AngleStart, &AngleExtent, &AngleStep, &ScaleMin, &ScaleMax, &ScaleStep, &Metric, &MinContrast);
			}
		}
	}
	return 1;
}
STDMETHODIMP CCanAlgo::BodyDirectionDetectAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pMatchingResult)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn      = m_vRgn[rId];
	CComVariant retValue;
	float greediness = (float)pGreediness->fltVal;
	//float MinSocre=(float)pMinSocre->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			//读取信息前，先清空模板相关数据。
			HTuple ModelParaOfSelfLearning,ParaLength;
			m_readDataState=ReadDatasForDirection(m_strTaskPath,m_isChineseLanguage,m_mapImg,m_mulImg,m_modelRegion,m_orgModelRegion,m_modelNum,m_angleStart,m_angleExtent,m_models,ModelParaOfSelfLearning);
			tuple_length(ModelParaOfSelfLearning,&ParaLength);
			if (ParaLength>=2)
			{
				m_contrast=ModelParaOfSelfLearning[0];
				m_minContrast=ModelParaOfSelfLearning[1];
			}
			if(m_readDataState)
			{
				//
				HTuple ModelArea;
				area_center (m_modelRegion, &ModelArea, &m_modelCenterRow, &m_modelCenterCol);
				//
				retValue =1.0;
				m_vErrorRgn[rId].Reset();
				m_vWhiteErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				return S_OK;
			}
		}
		//
		if(rgn.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			return S_FALSE;
		}
		//
		if(!m_readDataState)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			return S_FALSE;
		}
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject AddImageEnhanced,AddImageRectified,ImageReducedForSearch,SearchRegion; 
		HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;
		Hobject Image=m_crtImg;
		//
		HTuple  Width, Height,Width1, Height1;//,Width2, Height2
		get_image_size(Image, &Width, &Height);
		get_image_size(m_mulImg, &Width1, &Height1);
		if((Width!=Width1)||(Height!=Height1))
		{
			zoom_image_size(Image, &Image, Width1, Height1, "constant");
		}
		//
		SearchRegion=rgn;
		mult_image(Image, m_mulImg, &AddImageEnhanced, 0.02, 0);
		map_image(AddImageEnhanced, m_mapImg, &AddImageRectified);
		//get_image_size(AddImageRectified, &Width2, &Height2);
		move_region(SearchRegion,&SearchRegion,m_modelCenterRow-(Height1)/2.0,m_modelCenterCol-(Width1)/2.0);
		reduce_domain(AddImageRectified,SearchRegion, &ImageReducedForSearch);
		find_shape_models(ImageReducedForSearch,m_models,m_angleStart, m_angleExtent, 0.45, 1,0.5, "least_squares",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M, &Model_M);//(HTuple(4).Append(-2))
		if (0 != (Score_M>=0.05))
		{
			retValue =Score_M[0].D();
			Hobject newRgn; 
			m_vErrorRgn[rId] = m_orgModelRegion;
			retValue.Detach(pMatchingResult);
		}
		else
		{
			retValue = 0.0;	
			retValue.Detach(pMatchingResult);
			move_region(SearchRegion,&SearchRegion,(Height1)/2.0-m_modelCenterRow,(Width1)/2.0-m_modelCenterCol);
			m_vErrorRgn[rId] = m_orgModelRegion;
			return S_FALSE;
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pMatchingResult);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::BodyDirectionDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("439$\
					 440;R;FP;0.0;1.0;0.01;>=#0.60$\
					 167;B$\
					 322;FP;0.2;1.0;0.05;0.85");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//
STDMETHODIMP CCanAlgo::BodyDirectionDetectMinScoreAlgo(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels,  VARIANT* pGreediness, VARIANT* pMatchingResult)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn      = m_vRgn[rId];
	CComVariant retValue;
	float greediness = (float)pGreediness->fltVal;
	double MinScore=(double)pMinScore->fltVal;
	int NumLevels=(int)pNumLevels->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			//读取信息前，先清空模板相关数据。
			HTuple ModelParaOfSelfLearning,ParaLength;
			m_readDataState=ReadDatasForDirection(m_strTaskPath,m_isChineseLanguage,m_mapImg,m_mulImg,m_modelRegion,m_orgModelRegion,m_modelNum,m_angleStart,m_angleExtent,m_models,ModelParaOfSelfLearning);
			tuple_length(ModelParaOfSelfLearning,&ParaLength);
			if (ParaLength>=2)
			{
				m_contrast=ModelParaOfSelfLearning[0];
				m_minContrast=ModelParaOfSelfLearning[1];
			}
			if(m_readDataState)
			{
				//
				HTuple ModelArea;
				area_center (m_modelRegion, &ModelArea, &m_modelCenterRow, &m_modelCenterCol);
				//
				retValue =1.0;
				m_vErrorRgn[rId].Reset();
				m_vWhiteErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				return S_OK;
			}
		}
		//
		if(rgn.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			return S_FALSE;
		}
		//
		if(!m_readDataState)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			return S_FALSE;
		}
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject AddImageEnhanced,AddImageRectified,ImageReducedForSearch,SearchRegion; 
		HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;
		Hobject Image=m_crtImg;
		//
		HTuple  Width, Height,Width1, Height1;//,Width2, Height2
		get_image_size(Image, &Width, &Height);
		get_image_size(m_mulImg, &Width1, &Height1);
		if((Width!=Width1)||(Height!=Height1))
		{
			zoom_image_size(Image, &Image, Width1, Height1, "constant");
		}
		//
		SearchRegion=rgn;
		mult_image(Image, m_mulImg, &AddImageEnhanced, 0.02, 0);
		map_image(AddImageEnhanced, m_mapImg, &AddImageRectified);
		//get_image_size(AddImageRectified, &Width2, &Height2);
		move_region(SearchRegion,&SearchRegion,m_modelCenterRow-(Height1)/2.0,m_modelCenterCol-(Width1)/2.0);
		reduce_domain(AddImageRectified,SearchRegion, &ImageReducedForSearch);
		find_shape_models(ImageReducedForSearch,m_models,m_angleStart, m_angleExtent, MinScore, 1,0.5, "least_squares",NumLevels,greediness, &Row_M, &Column_M, &Angle_M, &Score_M, &Model_M);//(HTuple(4).Append(-2))
		if (0 != (Score_M>=0.05))
		{
			retValue =Score_M[0].D();
			Hobject newRgn; 
			m_vErrorRgn[rId] = m_orgModelRegion;
			retValue.Detach(pMatchingResult);
		}
		else
		{
			retValue = 0.0;	
			retValue.Detach(pMatchingResult);
			move_region(SearchRegion,&SearchRegion,(Height1)/2.0-m_modelCenterRow,(Width1)/2.0-m_modelCenterCol);
			m_vErrorRgn[rId] = m_orgModelRegion;
			return S_FALSE;
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pMatchingResult);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::BodyDirectionDetectMinScoreAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("439$\
					 440;R;FP;0.0;1.0;0.01;>=#0.60$\
					 167;B$\
					 654;FP;0.05;1.0;0.01;0.5$\
					 670;LP;0;8;1;0$\
					 322;FP;0.1;1.0;0.01;0.85");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CurlDetectAlgo1(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue = (float)pThresholdValue->fltVal;
	int GrayValue = (int)pGrayValue->fltVal;

	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	HTuple WidthOri, HeightOri;
	Hobject Image = m_crtImg;
	get_image_size (m_crtImg, &WidthOri, &HeightOri);
	Hobject  ResultDefect;
	HTuple  Mean, Deviation, MeanLength, Exp, MeanDeviation;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject SkeletonReg,RegionLine,RectReg,JuanYuanPartitioned;
		HTuple Rows, Columns,Length;
		HTuple Row2, Column2, Phi1, Length11, Length21;
		skeleton (rgn, &SkeletonReg);
		get_region_contour (SkeletonReg, &Rows, &Columns);
		tuple_length (Rows, &Length);
		HTuple PartsWidth=Length/NumPart;
		if(PartsWidth<3)
		{
			PartsWidth=3;
		}
		if(Rows.Num()<NumPart)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}


		for (HTuple i=0; i<=Length-PartsWidth-1; i=i+(PartsWidth-2))
		{
			gen_region_line (&RegionLine, HTuple(Rows[i]), HTuple(Columns[i]),HTuple(Rows[i+PartsWidth]), HTuple(Columns[i+PartsWidth]));
			smallest_rectangle2 (RegionLine, &Row2, &Column2, &Phi1, &Length11, &Length21);
			gen_rectangle2 (&RectReg, Row2, Column2, Phi1, Length11, Length21+RoiWidth/2);
			concat_obj (JuanYuanPartitioned, RectReg, &JuanYuanPartitioned);
		}
		intensity(JuanYuanPartitioned, Image, &Mean, &Deviation);
		tuple_length(Mean, &MeanLength);
		MeanDeviation = HTuple();
		for (int i=0; i<=MeanLength-2; i+=1)
		{
			tuple_deviation(HTuple(Mean[i]).Concat(HTuple(Mean[i+1])), &Exp);
			MeanDeviation[i] = Exp; 
		}

		HTuple Abs, AbsSorted, LocationId, errorValue, ResultID;
		tuple_abs(MeanDeviation, &Abs);
		tuple_sort(Abs, &AbsSorted);
		tuple_sort_index(Abs, &LocationId);
		tuple_length(Abs, &Length);
		for (int i=Length[0].I()-1; i>=0; i+=-1)
		{
			if (0 != (HTuple(AbsSorted[i])<ThresholdValue))
			{
				if (0 != (i==(Length-1)))
				{
					ResultID = HTuple();
					break;
				}
				else
				{
					tuple_last_n(LocationId, i+1, &ResultID);
					break;
				}
			}
		}
		ResultID += 1;

		HTuple MeanSorted, LocationId2, Length1, ResultID2, ResultID3, Sorted, Uniq;
		tuple_sort(Mean, &MeanSorted);
		tuple_sort_index(Mean, &LocationId2);
		tuple_length(Mean, &Length1);
		ResultID2 = HTuple();
		for (int i=0; i<=Length1-1; i+=1)
		{
			if (0 != (HTuple(MeanSorted[i]) < GrayValue))
			{
				ResultID2[i] = HTuple(LocationId2[i]);
			}
			else
			{
				break;
			}
		}
		ResultID2 += 1;

		tuple_concat(ResultID, ResultID2, &ResultID3);
		tuple_sort(ResultID3, &Sorted);
		tuple_uniq(Sorted, &Uniq);
		Hobject BackTransRegion, DefectRegs;
		select_obj(JuanYuanPartitioned, &BackTransRegion, Uniq);
		union1 (BackTransRegion, &DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center (DefectRegs, &Area, &Row_test, &Column_test);
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;	
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlDetectAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("402$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 403;LP;5;500;1;300$\
					 404;FP;1;100;0.1;30$\
					 405;LP;1;255;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//基于特征向量的混罐检测算法



STDMETHODIMP CCanAlgo::BodyDirectionDetectVecDiffAlgo(VARIANT* rgnId, /*VARIANT* pVecDiffThreshold,*/ VARIANT* pVecDiffValue)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn      = m_vRgn[rId];
	CComVariant retValue;
	//float VecDiffThreshold = (float)pVecDiffThreshold->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg || m_bInitialSuccess == false)
		{
			//初始化
			string strMapPath=m_strTaskPath+"\\Map.tiff";
			string strMatrixPath = m_strTaskPath + "\\ReferMatrix.mtx";

			if(_access(strMapPath.c_str(),0))
			{
				if (!m_bErrorShowed)
				{
					if(m_isChineseLanguage)
					{
						MessageBox(NULL,_T("棋盘格相关图像缺失，请使用训练算子"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					else
					{
						MessageBox(NULL,_T("Missing CheckerBoard Related Image, please use training operator"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}

					m_bErrorShowed = true;
				}			

				retValue = 255;
				m_vErrorRgn[rId].Reset();
				m_vWhiteErrorRgn[rId].Reset();
				retValue.Detach(pVecDiffValue);
				return S_FALSE;
			}

			if(_access(strMatrixPath.c_str(),0))
			{
				if (!m_bErrorShowed)
				{
					if(m_isChineseLanguage)
					{
						MessageBox(NULL,_T("训练结果缺失，请使用训练算子"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					else
					{
						MessageBox(NULL,_T("Missing Training result file, please use training operator"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					m_bErrorShowed = true;
				}				
				retValue = 255;
				m_vErrorRgn[rId].Reset();
				m_vWhiteErrorRgn[rId].Reset();
				retValue.Detach(pVecDiffValue);
				return S_FALSE;
			}
			m_bInitialSuccess = true;

			read_image(&m_mapImg, HTuple(strMapPath.c_str()));
			HTuple MatrixTemp;
			read_matrix(HTuple(strMatrixPath.c_str()), &MatrixTemp);
			m_MatrixDataInstance.InitialWithMatrix(MatrixTemp);

			get_domain(m_mapImg, &m_ProjROI);


			//////////////////////////////////////////////////////////////////////////
			string strOriginPartitionROI_1=m_strTaskPath+"\\OriginPartitionROI_1.reg";
			string strOriginPartitionROI_2=m_strTaskPath+"\\OriginPartitionROI_2.reg";

			if (_access(strOriginPartitionROI_1.c_str(),0) || _access(strOriginPartitionROI_2.c_str(),0))
			{
				H_GetMultiMapRegInOrigImg(m_mapImg, &m_OriginPartitionROI_1, &m_OriginPartitionROI_2);

				write_region(m_OriginPartitionROI_1, HTuple(strOriginPartitionROI_1.c_str()));
				write_region(m_OriginPartitionROI_2, HTuple(strOriginPartitionROI_2.c_str()));
			} 
			else
			{
				read_region(&m_OriginPartitionROI_1, HTuple(strOriginPartitionROI_1.c_str()));
				read_region(&m_OriginPartitionROI_2, HTuple(strOriginPartitionROI_2.c_str()));
			}	
			//为了检测的时候节约时间 union耗时
			union1(m_OriginPartitionROI_1,&m_OriginPartitionUnionROI_1);
			union1(m_OriginPartitionROI_2,&m_OriginPartitionUnionROI_2);

			string strModelRegionPath= m_strTaskPath+ "\\Modelling.reg";
			if (_access(strModelRegionPath.c_str(),0))
			{
				gen_empty_obj(&m_MapRegInOrigImg);
				H_GetMapOrginRegDualChannel(m_mapImg, &m_MapRegInOrigImg);
				write_region(m_MapRegInOrigImg , HTuple(strModelRegionPath.c_str()));
			} 
			else
			{
				read_region(&m_MapRegInOrigImg, HTuple(strModelRegionPath.c_str()));
			}			
			//////////////////////////////////////////////////////////////////////////
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}

		HTuple ErrorMsg;
		Hobject CurrentImg = m_crtImg;
		Hobject MappedImgs;
		HTuple FeatureVectors;

#ifndef MAP_USE_FIX_REGION
		H_MapImgs(CurrentImg, m_mapImg, &MappedImgs, &ErrorMsg);		

		H_GetFeatureVector(MappedImgs, m_ProjROI, &FeatureVector);
#else
		//并行处理
		HTuple ObjNum;
		count_obj(CurrentImg, &ObjNum);
		if(ObjNum>1)
		{
			H_GetParallelFeatureVector_FixReg(CurrentImg, m_MapRegInOrigImg, m_OriginPartitionROI_1, m_OriginPartitionROI_2, m_OriginPartitionUnionROI_1, m_OriginPartitionUnionROI_2,&FeatureVectors);
		}
		else
		{
			H_GetFeatureVector_FixReg(CurrentImg, m_MapRegInOrigImg, m_OriginPartitionROI_1, m_OriginPartitionROI_2, m_OriginPartitionUnionROI_1, m_OriginPartitionUnionROI_2,&FeatureVectors);
		}

#endif

		HTuple MatrixNew = m_MatrixDataInstance.GetMatrix();
		HTuple TrainedImgNum = m_MatrixDataInstance.GetTrainedNum();


		HTuple DiffValue, DiffIndex;

		H_Calc_Vector_Diff(MatrixNew, FeatureVectors, TrainedImgNum, &DiffValue, &DiffIndex);

		retValue =DiffValue[0].D();
		retValue.Detach(pVecDiffValue);
		//if (/*DiffValue> VecDiffThreshold ||*/ DiffValue == -1)
		//{

		//	Hobject newRgn; 
		//	m_vErrorRgn[rId] = rgn;

		//	return S_FALSE;
		//}
		//else
		//{
		//	//更新模板数据
		//	HTuple MaxTrainedImgNum = m_MatrixDataInstance.GetMaxTrainedImgNum();

		//	if (DiffValue > 0 && DiffValue < 4.0/*DiffValue < 0.6 * VecDiffThreshold && DiffValue[0].D() != 0.0*/)
		//	{
		//		if (TrainedImgNum <= MaxTrainedImgNum)
		//		{
		//			m_MatrixDataInstance.AddVector(FeatureVector);
		//			m_bMatrixUpdated = true;
		//		} 
		//		else
		//		{
		//			m_MatrixDataInstance.UpdateMatrix(DiffIndex[0].I(), FeatureVector);
		//			m_bMatrixUpdated = true;
		//		}							
		//	}
		//}


#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = 255;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pVecDiffValue);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::BodyDirectionDetectVecDiffAlgoHelp( BSTR* pHelpStr)
{
	CComBSTR strHelp("1089$\
					 1090;R;FP;0.1;255.0;0.1;<=#3.0$\
					 167;B");
	//$\
	//	1091;FP;0.1;255.0;0.1;3.0

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CCanAlgo::BodyDirectionVecDiffLocationAlgo(VARIANT* rgnId)
{
	int id = (int)rgnId->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;


#ifdef NDEBUG
	try
	{
#endif
		HTuple RegArea, AreaLength;
		region_features(m_MapRegInOrigImg, "area", &RegArea);
		tuple_length(RegArea, &AreaLength);

		if(m_bPilotImg || AreaLength <= 0)
		{
			string strMapPath=m_strTaskPath+"\\Map.tiff";
			if(_access(strMapPath.c_str(),0))
			{
				if (!m_bErrorShowed)
				{
					if(m_isChineseLanguage)
					{
						MessageBox(NULL,_T("棋盘格相关图像缺失，请使用训练算子"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					else
					{
						MessageBox(NULL,_T("Missing CheckerBoard Related Image, please use training operator"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					m_bErrorShowed = true;
				}				
				return S_FALSE;
			}

			Hobject MapImgTemp;
			read_image(&MapImgTemp, HTuple(strMapPath.c_str()));

			gen_empty_obj(&m_MapRegInOrigImg);
			H_GetMapOrginRegDualChannel(MapImgTemp, &m_MapRegInOrigImg);
		}

		m_vRgn[id] = m_MapRegInOrigImg ;

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::BodyDirectionVecDiffLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1092$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


//Color image process, Algo.
STDMETHODIMP CCanAlgo::CurlOptimizeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pIsShowInterImg,  VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pDefectType,VARIANT* pWaveThreshold,VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	//利用带通滤波器的卷边优化算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_dictSpecialData[rId].clear();
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	double FreqStart = (double)pFreqStart->fltVal;
	FreqStart = FreqStart/100;
	double FreqCutoff = (double)pFreqCutoff->fltVal;
	FreqCutoff = FreqCutoff/100;
	int DefectType = (int)pDefectType->fltVal;
	float WaveThreshold = (float)pWaveThreshold->fltVal;
	int MinArea = (int)pMinArea->fltVal;
	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	bool IsShowInterImg = (bool)pIsShowInterImg->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。

		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,  ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectNumber);
			return S_FALSE;
		}
		Hobject DetectRegion;
		int DetectNumber;
		std::vector<float>DictSpecialData;
		if(H_CurlOptimizeAlgo(Image,rgn,&DetectRegion,fCenterX,fCenterY,RoiWidth, Radius ,FreqStart,FreqCutoff,DefectType, WaveThreshold,MinArea, &DetectNumber,DictSpecialData))
		{
			retValue = DetectNumber; 
			m_vErrorRgn[rId] = DetectRegion;
			if(m_bDebugImg)//显示数据
			{
				if (IsShowInterImg)
				{
					m_vInterImgs[rId] = Image;
				}
				else
				{
					Hobject EmptyObj;					
					m_vInterImgs[rId] = EmptyObj;
				}


				bool isSector;
				isSector=true;
				if(m_sectorAngle[rId].m_angelEnd-m_sectorAngle[rId].m_angelStart>359.99)
				{
					isSector=false;
				}
				std::vector<float>DictSpecialDataRota;
				std::vector<float>DictSpecialDataRotaDetectAngle;
				int DataLength=(int)(DictSpecialData.size()-2);
				int angleO=(int)(DataLength*(85.6/360.0));//起始角度不是90度。
				DictSpecialDataRota.push_back(DictSpecialData[0]);
				DictSpecialDataRota.push_back(DictSpecialData[1]);
				for(int i = angleO+1; i >= 2; i--)
				{
					DictSpecialDataRota.push_back(DictSpecialData[i]);
				}
				for(int i =DataLength+1; i >angleO+1; i--)
				{
					DictSpecialDataRota.push_back(DictSpecialData[i]);
				}
				//
				if(isSector)
				{
					if((m_angleDetect>=0)&&(m_angleDetect<6.28))
					{
						angleO=(int)(DataLength*(m_angleDetect/6.283));
					}
					else if((m_angleDetect>=-6.28)&&(m_angleDetect<0))
					{
						angleO=(int)(DataLength*(1+m_angleDetect/6.283));
					}
					else
					{
						angleO=2;
					}

					DictSpecialDataRotaDetectAngle.push_back(DictSpecialData[0]);
					DictSpecialDataRotaDetectAngle.push_back(DictSpecialData[1]);

					for(int i =angleO+1; i <=DataLength+1; i++)
					{
						DictSpecialDataRotaDetectAngle.push_back(DictSpecialDataRota[i]);
					}
					for(int i =2; i<angleO+1; i++)
					{
						DictSpecialDataRotaDetectAngle.push_back(DictSpecialDataRota[i]);
					}

					int startData=(int)((m_sectorAngle[rId].m_angelStart/360.0)*DataLength);
					int endData=(int)((m_sectorAngle[rId].m_angelEnd/360.0)*DataLength);
					if(startData<0)
						startData=0;
					if(startData>=DataLength)
						startData=2;
					if(endData<=startData)
						endData=DataLength;
					if(endData>=DataLength)
						endData=DataLength;
					std::vector<float>DictSpecialSector;
					DictSpecialSector.push_back(DictSpecialData[0]);
					DictSpecialSector.push_back(DictSpecialData[1]);
					for(int i =startData+2; i<endData+2; i++)
					{
						DictSpecialSector.push_back(DictSpecialDataRotaDetectAngle[i]);
					}
					m_dictSpecialData[rId]=DictSpecialSector;
				}
				else
					m_dictSpecialData[rId]=DictSpecialDataRota;//DictSpecialData;
				//


			}
		}
		else
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectNumber);
			return S_FALSE;
		}		
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlOptimizeForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("656$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 1327;LP;0;1;1;1$\
					 382;FP;1;50;1;2$\
					 383;FP;2;50;1;10$\
					 385;LP;-1;1;1;0$\
					 386;FP;0.1;100.0;0.1;22$\
					 387;LP;1;100;1;2");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::DynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pIsShowInterImg,  VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	int WhiteMaskSize = (int) pWhiteMaskSize->fltVal;
	float SeriousWhitePointDynThresh = (float) pSeriousWhitePointDynThresh->fltVal;
	int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;

	bool IsShowInterImg = (bool)pIsShowInterImg->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,  ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		if (IsShowInterImg)
		{
			m_vInterImgs[rId] = Image;
		}
		else
		{
			Hobject EmptyObj;					
			m_vInterImgs[rId] = EmptyObj;
		}

		Hobject ImageReduced, ImageReduceMean;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[rId].push_back(float(SeriousWhitePointDynThresh));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			abs_diff_image (ImageReduced, ImageReduceMean, &ImageSub, 1);
			gray_histo (rgn, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd* Width* Height<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*(Width[0].D()* Height[0].D()));
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;
		union2(BlackUnion, WhiteUnion, &DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::DynThresholdForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("660$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 1327;LP;0;1;1;0$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;60$\
					 416;LP;1;200;1;10$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;250;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::EdgeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pIsShowInterImg,  VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	int EdgeThred = (int)pEdgeThred->fltVal;
	int DefectSize = (int)pDefectSize->fltVal;
	bool IsShowInterImg = (bool)pIsShowInterImg->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image, rgn, ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		if (IsShowInterImg)
		{
			m_vInterImgs[rId] = Image;
		}
		else
		{
			Hobject EmptyObj;					
			m_vInterImgs[rId] = EmptyObj;
		}

		Hobject ImageReduced, ExpandedImage, DerivGauss;
		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);

		Hobject ImageMean, ImageExpandedReduced;
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(ExpandedImage, &ImageMean, MaskSize, MaskSize);
		reduce_domain(ImageMean, rgn, &ImageExpandedReduced);

		Hobject ImageEdgeRobinson, ThresholdRegion, ConnectedThresholdRegion, SelectedRegion;
		robinson_amp(ImageExpandedReduced, &ImageEdgeRobinson);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(EdgeThred));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple AbsoluteHisto, RelativeHisto, Length;
			gray_histo (rgn, ImageEdgeRobinson, &AbsoluteHisto, &RelativeHisto);
			HTuple  maxValue = 0;
			tuple_max (AbsoluteHisto, &maxValue);
			tuple_length (AbsoluteHisto, &Length);
			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			int histoLength = Length[0].I();
			for (int i=0; i< histoLength; i++)
			{
				m_dictSpecialData[rId].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
			}
		}
		threshold(ImageEdgeRobinson, &ThresholdRegion, EdgeThred, 255);
		connection(ThresholdRegion, &ConnectedThresholdRegion);
		select_shape(ConnectedThresholdRegion, &SelectedRegion, "area", "and", DefectSize, 99999);
		Hobject DefectRegs;
		union1(SelectedRegion, &DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::EdgeForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("661$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 1327;LP;0;1;1;0$\
					 433;LP;1;255;1;30$\
					 434;LP;1;250;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::AnnularRotaDynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pIsShowInterImg,  VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	int ThresholdValue = (int)pDynThresh->fltVal;
	int DefectSize = (int) pDefectSize->fltVal;
	float RotaAngle = (float)pRotaAngle->fltVal;
	bool IsShowInterImg = (bool)pIsShowInterImg->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image, rgn,  ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		if (IsShowInterImg)
		{
			m_vInterImgs[rId] = Image;
		}
		else
		{
			Hobject EmptyObj;					
			m_vInterImgs[rId] = EmptyObj;
		}

		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
		//

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		reduce_domain (Image, rgn, &ImageReduced);
		vector_angle_to_rigid(fCenterY,fCenterX, 0, fCenterY, fCenterX, HTuple(RotaAngle).Rad(), &HomMat2D);
		affine_trans_image(ImageReduced, &ImageAffinTrans, HomMat2D, "bilinear", "false");
		dyn_threshold(ImageReduced, ImageAffinTrans, &RegionDynThresh, ThresholdValue, "not_equal");
		connection(RegionDynThresh, &ConnectedRegions4);
		select_shape(ConnectedRegions4, &SelectedRegions4, "area", "and", DefectSize, 9999999);
		Hobject DefectRegs;
		union1(SelectedRegions4, &DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::AnnularRotaDynThresholdForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("662$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 1327;LP;0;1;1;0$\
					 610;FP;0.5;60;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CurlNoCompoundForColorAlgo(VARIANT* rgnId,VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT*pGrayValueMean )
{
	int rId = (int)rgnId->fltVal;
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
	Hobject RegionDilat;
	HTuple MeanGray, Deviation;//pDetectArea

	bool IsShowInterImg = (bool)pIsShowInterImg->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image, rgn,  ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pGrayValueMean);
			return S_FALSE;
		}
		if (IsShowInterImg)
		{
			m_vInterImgs[rId] = Image;
		}
		else
		{
			Hobject EmptyObj;					
			m_vInterImgs[rId] = EmptyObj;
		}

		intensity (rgn, Image, &MeanGray, &Deviation);

		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(MeanGray[0].D()));
			m_dictSpecialData[rId].push_back(-999999.0f);

			HTuple AbsoluteHisto, RelativeHisto, Length, maxValue = 0;
			gray_histo (rgn, Image, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(AbsoluteHisto, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			int histoLength = Length[0].I();
			for (int i = 0; i < histoLength; i ++)
			{
				m_dictSpecialData[rId].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
			}
		}
		retValue = MeanGray[0].I();
		dilation_circle (rgn, &RegionDilat, 1.5);
		m_vErrorRgn[rId] = RegionDilat;
		retValue.Detach(pGrayValueMean);
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlNoCompoundForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("686$\
					 200;R;LP;1;255;1;<=#150$\
					 167;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 1327;LP;0;1;1;0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CurlDetectForColorAlgo(VARIANT* rgnId,VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int rId = (int)rgnId->fltVal;
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue = (float)pThresholdValue->fltVal;
	int GrayValue = (int)pGrayValue->fltVal;

	bool IsShowInterImg = (bool)pIsShowInterImg->fltVal;
	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
	double RegionalOut, RegionalIn;
	if(Radius - RoiWidth > 0)
	{
		if(RoiWidth > 0)
		{
			RegionalOut = Radius;
			RegionalIn = Radius - RoiWidth;
		}
		else
		{
			RegionalOut = Radius - RoiWidth;
			RegionalIn = Radius;
		}
	}
	else
	{
		RegionalOut = Radius;
		RegionalIn = 0;
	}
	HTuple WidthOri, HeightOri;
	Hobject Image;// = m_crtImg;
	//按要求变换彩色空间，并提取单通道图像。
	if(!H_ColorImageTrans(m_crtImgMult,&Image, rgn,  ColorTransType, ChannelSelect))
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	if (IsShowInterImg)
	{
		m_vInterImgs[rId] = Image;
	}
	else
	{
		Hobject EmptyObj;					
		m_vInterImgs[rId] = EmptyObj;
	}

	get_image_size (Image, &WidthOri, &HeightOri);
	Hobject ImageScaleMax, JuanYuanPartitioned, ResultDefect;//PolarTransImage
	HTuple PolarTransImageWidth, PolarTransImageHeight, Mean, Deviation, MeanLength, Exp, MeanDeviation;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		polar_trans_image_ext(Image, &ImageScaleMax, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), "nearest_neighbor");
		get_image_size(ImageScaleMax, &PolarTransImageWidth, &PolarTransImageHeight);
		//scale_image_max(PolarTransImage, &ImageScaleMax);
		partition_rectangle(ImageScaleMax, &JuanYuanPartitioned, PolarTransImageWidth/NumPart, PolarTransImageHeight);
		intensity(JuanYuanPartitioned, ImageScaleMax, &Mean, &Deviation);
		tuple_length(Mean, &MeanLength);
		MeanDeviation = HTuple();
		for (int i=0; i<=MeanLength-2; i+=1)
		{
			tuple_deviation(HTuple(Mean[i]).Concat(HTuple(Mean[i+1])), &Exp);
			MeanDeviation[i] = Exp; 
		}

		HTuple Abs, AbsSorted, LocationId, Length, errorValue, ResultID;
		tuple_abs(MeanDeviation, &Abs);
		tuple_sort(Abs, &AbsSorted);
		tuple_sort_index(Abs, &LocationId);
		tuple_length(Abs, &Length);
		for (int i=Length[0].I()-1; i>=0; i+=-1)
		{
			if (0 != (HTuple(AbsSorted[i])<ThresholdValue))
			{
				if (0 != (i==(Length-1)))
				{
					ResultID = HTuple();
					break;
				}
				else
				{
					tuple_last_n(LocationId, i+1, &ResultID);
					break;
				}
			}
		}
		ResultID += 1;

		HTuple MeanSorted, LocationId2, Length1, ResultID2, ResultID3, Sorted, Uniq;
		tuple_sort(Mean, &MeanSorted);
		tuple_sort_index(Mean, &LocationId2);
		tuple_length(Mean, &Length1);
		ResultID2 = HTuple();
		for (int i=0; i<=Length1-1; i+=1)
		{
			if (0 != (HTuple(MeanSorted[i]) < GrayValue))
			{
				ResultID2[i] = HTuple(LocationId2[i]);
			}
			else
			{
				break;
			}
		}
		ResultID2 += 1;

		tuple_concat(ResultID, ResultID2, &ResultID3);
		tuple_sort(ResultID3, &Sorted);
		tuple_uniq(Sorted, &Uniq);
		Hobject BackTransRegion, DefectRegs;
		select_obj(JuanYuanPartitioned, &ResultDefect, Uniq);
		polar_trans_region_inv(ResultDefect, &BackTransRegion, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), WidthOri, HeightOri, "nearest_neighbor");
		union1 (BackTransRegion, &DefectRegs);
		intersection(DefectRegs,rgn,&DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center (DefectRegs, &Area, &Row_test, &Column_test);
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;	
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlDetectForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("687$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 1327;LP;0;1;1;0$\
					 403;LP;5;500;1;300$\
					 404;FP;1;100;0.1;30$\
					 405;LP;1;255;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CurlMeanGrayDetectAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pGrayValueLower, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue =300;
	int GrayValue = (int)pGrayValue->fltVal;
	int GrayValueLower = (int)pGrayValueLower->fltVal;
	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
	double RegionalOut, RegionalIn;
	if(Radius - RoiWidth > 0)
	{
		if(RoiWidth > 0)
		{
			RegionalOut = Radius;
			RegionalIn = Radius - RoiWidth;
		}
		else
		{
			RegionalOut = Radius - RoiWidth;
			RegionalIn = Radius;
		}
	}
	else
	{
		RegionalOut = Radius;
		RegionalIn = 0;
	}
	HTuple WidthOri, HeightOri;
	Hobject Image = m_crtImg;
	get_image_size (Image, &WidthOri, &HeightOri);
	Hobject ImageScaleMax, JuanYuanPartitioned, ResultDefect;//PolarTransImage
	HTuple PolarTransImageWidth, PolarTransImageHeight, Mean, Deviation, MeanLength, Exp, MeanDeviation;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		polar_trans_image_ext(Image, &ImageScaleMax, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), "nearest_neighbor");
		get_image_size(ImageScaleMax, &PolarTransImageWidth, &PolarTransImageHeight);
		//scale_image_max(PolarTransImage, &ImageScaleMax);
		partition_rectangle(ImageScaleMax, &JuanYuanPartitioned, PolarTransImageWidth/NumPart, PolarTransImageHeight);
		intensity(JuanYuanPartitioned, ImageScaleMax, &Mean, &Deviation);
		tuple_length(Mean, &MeanLength);
		MeanDeviation = HTuple();
		for (int i=0; i<=MeanLength-2; i+=1)
		{
			tuple_deviation(HTuple(Mean[i]).Concat(HTuple(Mean[i+1])), &Exp);
			MeanDeviation[i] = Exp; 
		}

		HTuple Abs, AbsSorted, LocationId, Length, errorValue, ResultID;
		tuple_abs(MeanDeviation, &Abs);
		tuple_sort(Abs, &AbsSorted);
		tuple_sort_index(Abs, &LocationId);
		tuple_length(Abs, &Length);
		for (int i=Length[0].I()-1; i>=0; i+=-1)
		{
			if (0 != (HTuple(AbsSorted[i])<ThresholdValue))
			{
				if (0 != (i==(Length-1)))
				{
					ResultID = HTuple();
					break;
				}
				else
				{
					tuple_last_n(LocationId, i+1, &ResultID);
					break;
				}
			}
		}
		ResultID += 1;

		HTuple MeanSorted, LocationId2, Length1, ResultID2, ResultID3, Sorted, Uniq;
		tuple_sort(Mean, &MeanSorted);
		tuple_sort_index(Mean, &LocationId2);
		tuple_length(Mean, &Length1);
		ResultID2 = HTuple();
		int numDetect=0;
		for (int i=0; i<=Length1-1; i+=1)
		{
			if  ( (HTuple(MeanSorted[i]) >= GrayValueLower)&& (HTuple(MeanSorted[i]) <= GrayValue))
			{
				ResultID2[numDetect] = HTuple(LocationId2[i]);
				numDetect+=1;
			}
			else if ((HTuple(MeanSorted[i])>GrayValue))
			{
				break;
			}
		}
		ResultID2 += 1;

		tuple_concat(ResultID, ResultID2, &ResultID3);
		tuple_sort(ResultID3, &Sorted);
		tuple_uniq(Sorted, &Uniq);
		Hobject BackTransRegion, DefectRegs;
		select_obj(JuanYuanPartitioned, &ResultDefect, Uniq);
		polar_trans_region_inv(ResultDefect, &BackTransRegion, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), WidthOri, HeightOri, "nearest_neighbor");
		union1 (BackTransRegion, &DefectRegs);
		intersection(DefectRegs,rgn,&DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center (DefectRegs, &Area, &Row_test, &Column_test);
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;	
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlMeanGrayDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("689$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 403;LP;5;500;1;300$\
					 191;LP;-1;255;1;50$\
					 192;LP;1;256;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::CurlMeanGrayDetectForColorAlgo(VARIANT* rgnId,VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pIsShowInterImg, VARIANT* pNumPart, VARIANT* pGrayValueLower, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int rId = (int)rgnId->fltVal;
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	bool IsShowInterImg = (bool)pIsShowInterImg->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue =300;
	int GrayValue = (int)pGrayValue->fltVal;
	int GrayValueLower = (int)pGrayValueLower->fltVal;
	float fCenterX;
	float fCenterY;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
	double RegionalOut, RegionalIn;
	if(Radius - RoiWidth > 0)
	{
		if(RoiWidth > 0)
		{
			RegionalOut = Radius;
			RegionalIn = Radius - RoiWidth;
		}
		else
		{
			RegionalOut = Radius - RoiWidth;
			RegionalIn = Radius;
		}
	}
	else
	{
		RegionalOut = Radius;
		RegionalIn = 0;
	}
	HTuple WidthOri, HeightOri;
	Hobject Image;// = m_crtImg;
	//按要求变换彩色空间，并提取单通道图像。
	if(!H_ColorImageTrans(m_crtImgMult,&Image, rgn,  ColorTransType, ChannelSelect))
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	if (IsShowInterImg)
	{
		m_vInterImgs[rId] = Image;
	}
	else
	{
		Hobject EmptyObj;					
		m_vInterImgs[rId] = EmptyObj;
	}

	get_image_size (Image, &WidthOri, &HeightOri);
	Hobject ImageScaleMax, JuanYuanPartitioned, ResultDefect;//PolarTransImage
	HTuple PolarTransImageWidth, PolarTransImageHeight, Mean, Deviation, MeanLength, Exp, MeanDeviation;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		polar_trans_image_ext(Image, &ImageScaleMax, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), "nearest_neighbor");
		get_image_size(ImageScaleMax, &PolarTransImageWidth, &PolarTransImageHeight);
		//scale_image_max(PolarTransImage, &ImageScaleMax);
		partition_rectangle(ImageScaleMax, &JuanYuanPartitioned, PolarTransImageWidth/NumPart, PolarTransImageHeight);
		intensity(JuanYuanPartitioned, ImageScaleMax, &Mean, &Deviation);
		tuple_length(Mean, &MeanLength);
		MeanDeviation = HTuple();
		for (int i=0; i<=MeanLength-2; i+=1)
		{
			tuple_deviation(HTuple(Mean[i]).Concat(HTuple(Mean[i+1])), &Exp);
			MeanDeviation[i] = Exp; 
		}

		HTuple Abs, AbsSorted, LocationId, Length, errorValue, ResultID;
		tuple_abs(MeanDeviation, &Abs);
		tuple_sort(Abs, &AbsSorted);
		tuple_sort_index(Abs, &LocationId);
		tuple_length(Abs, &Length);
		for (int i=Length[0].I()-1; i>=0; i+=-1)
		{
			if (0 != (HTuple(AbsSorted[i])<ThresholdValue))
			{
				if (0 != (i==(Length-1)))
				{
					ResultID = HTuple();
					break;
				}
				else
				{
					tuple_last_n(LocationId, i+1, &ResultID);
					break;
				}
			}
		}
		ResultID += 1;

		HTuple MeanSorted, LocationId2, Length1, ResultID2, ResultID3, Sorted, Uniq;
		tuple_sort(Mean, &MeanSorted);
		tuple_sort_index(Mean, &LocationId2);
		tuple_length(Mean, &Length1);
		ResultID2 = HTuple();
		int numDetect=0;
		for (int i=0; i<=Length1-1; i+=1)
		{
			if  ( (HTuple(MeanSorted[i]) >= GrayValueLower)&& (HTuple(MeanSorted[i]) <= GrayValue))
			{
				ResultID2[numDetect] = HTuple(LocationId2[i]);
				numDetect+=1;
			}
			else if ((HTuple(MeanSorted[i])>GrayValue))
			{
				break;
			}
		}
		ResultID2 += 1;

		tuple_concat(ResultID, ResultID2, &ResultID3);
		tuple_sort(ResultID3, &Sorted);
		tuple_uniq(Sorted, &Uniq);
		Hobject BackTransRegion, DefectRegs;
		select_obj(JuanYuanPartitioned, &ResultDefect, Uniq);
		polar_trans_region_inv(ResultDefect, &BackTransRegion, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), WidthOri, HeightOri, "nearest_neighbor");
		union1 (BackTransRegion, &DefectRegs);
		intersection(DefectRegs,rgn,&DefectRegs);
		HTuple Area, Row_test, Column_test;
		area_center (DefectRegs, &Area, &Row_test, &Column_test);
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[rId] = DefectRegs;	
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::CurlMeanGrayDetectForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("688$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 1327;LP;0;1;1;0$\
					 403;LP;5;500;1;300$\
					 191;LP;-1;255;1;50$\
					 192;LP;1;256;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}




void  CCanAlgo::ExtractEdgesPoints(Hobject Image, HTuple Elements, HTuple DetectHeight, HTuple DetectWidth, HTuple Sigma, HTuple Threshold, HTuple Transition, HTuple Select, 
	HTuple Row1, HTuple Column1,HTuple Row2, HTuple Column2, HTuple *ResultRow, HTuple *ResultColumn)
{
	HTuple Width,Height;
	get_image_size(Image, &Width, &Height);
	HTuple Deg,ATan,RowC,ColC;
	tuple_atan2((-Row2)+Row1, Column2-Column1, &ATan);
	ATan += HTuple(90).Rad();
	tuple_deg(ATan, &Deg);
	for (int i=1; i<=Elements; i++)
	{
		RowC = Row1+(((Row2-Row1)*i)/(Elements+1));
		ColC = Column1+(((Column2-Column1)*i)/(Elements+1));
		if ((RowC>(Height-1))||(RowC<0)||(ColC>(Width-1))||(ColC<0))
		{
			continue;
		}
		//显示区域
		HTuple MsrHandle_Measure;
		gen_measure_rectangle2(RowC, ColC, Deg.Rad(), DetectHeight/2, DetectWidth/2, Width, Height, "nearest_neighbor", &MsrHandle_Measure);

		if (0 != (Transition==HTuple("negative")))
		{
			Transition = "negative";
		}
		else
		{
			if (0 != (Transition==HTuple("positive")))
			{

				Transition = "positive";
			}
			else
			{
				Transition = "all";
			}
		}

		if (0 != (Select==HTuple("first")))
		{
			Select = "first";
		}
		else
		{
			if (0 != (Select==HTuple("last")))
			{

				Select = "last";
			}
			else
			{
				Select = "all";
			}
		}
		HTuple RowEdge, ColEdge, Amplitude, Distance;
		measure_pos(Image, MsrHandle_Measure, Sigma, Threshold, Transition, Select, &RowEdge, &ColEdge, &Amplitude, &Distance);
		close_measure(MsrHandle_Measure);
		HTuple tRow = 0;
		HTuple tCol = 0;
		HTuple t = 0;
		HTuple Number;
		tuple_length(RowEdge, &Number);
		if (0 != (Number<1))
		{
			continue;
		}

		for (int j=0;j<=Number-1; j++)
		{
			if (0 != (((Amplitude.Select(j)).Abs())>t))
			{

				tRow = RowEdge.Select(j);
				tCol = ColEdge.Select(j);
				t = (Amplitude.Select(j)).Abs();
			}
		}

		if (0 != (t>0))
		{

			*ResultRow = (*ResultRow).Concat(tRow);
			*ResultColumn = (*ResultColumn).Concat(tCol);
		}
	}
}
void  CCanAlgo::FitLineUseEdgesPoints(HTuple Rows,HTuple Cols,HTuple ActiveNum,HTuple *Row1,HTuple *Col1,HTuple *Row2,HTuple *Col2)
{
	Hobject  Contour; 
	HTuple  Length, Nr, Nc, Dist,Length1;
	*Row1 = 0;
	*Col1 = 0;
	*Row2 = 0;
	*Col2 = 0;
	tuple_length(Cols, &Length);
	if ((Length>=ActiveNum)&&(ActiveNum>1))
	{
		gen_contour_polygon_xld(&Contour, Rows, Cols);
		fit_line_contour_xld(Contour, "tukey", ActiveNum, 0, 5, 2, &(*Row1), &(*Col1), &(*Row2), &(*Col2), &Nr, &Nc, &Dist);
		tuple_length(Dist,&Length1);
		if (Length1<1)
		{
			return;
		}

	}
	return;
}
STDMETHODIMP CCanAlgo::SideCenterLocationAlgo(VARIANT* rgnID ,VARIANT*pRowStart,VARIANT*pColStart,VARIANT*pRegWidth,VARIANT*pRegHeight,VARIANT* pThredValueUp)
{
	int id = (int)rgnID->fltVal;

	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;
	int ThredValueUp=(int)pThredValueUp->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG        
	try
	{
#endif
		Hobject  Image = m_crtImg;
		HTuple ImageWidth,ImageHeight;
		get_image_size(Image,&ImageWidth,&ImageHeight);	
		//	
		if (RowStart>ImageHeight/2)
		{
			RowStart=10;
		}
		if (ColStart>ImageWidth/2)
		{
			ColStart=10;
		}
		if ((RegWidth+ColStart)>ImageWidth-1)
		{
			RegWidth=10;
		}
		if ((RegHeight+RowStart)>ImageHeight-1)
		{
			RegHeight=10;
		}
		Hobject SearchRgn;
		gen_rectangle2(&SearchRgn,RowStart+RegHeight/2,ColStart+RegWidth/2,0,RegWidth/2,RegHeight/2);
		HTuple RectTopY, RectTopX, RectButtomY, RectButtomX;
		smallest_rectangle1(SearchRgn, &RectTopY, &RectTopX, &RectButtomY, &RectButtomX);

		//上边缘定位
		HTuple StartRow,StartCol,EndRow,EndCol;
		StartRow = (RectTopY+RectButtomY)/2;
		StartCol = RectTopX;
		EndRow = (RectTopY+RectButtomY)/2;
		EndCol = RectButtomX;
		HTuple ResultRow, ResultColumn;
		Hobject Regions;
		ExtractEdgesPoints(Image, 36, 230, 10, 1, ThredValueUp, "positive", "first", EndRow, EndCol, StartRow, StartCol, &ResultRow, &ResultColumn);

		if (ResultRow.Num()<2)
		{
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y =0.0;
			m_vRgn[id] = SearchRgn ;
			return S_FALSE;
		}
		Hobject Contour,SmoothedContours,BaseRegion;
		gen_contour_polygon_xld(&Contour,ResultRow,ResultColumn);
		smooth_contours_xld(Contour,&SmoothedContours,5);
		HTuple RowSmoothed,ColSmoothed;
		get_contour_xld(SmoothedContours,&RowSmoothed,&ColSmoothed);
		gen_region_polygon(&m_BaseRegion,RowSmoothed,ColSmoothed);

		HTuple Area,CenterRow,CenterCol;
		area_center(m_BaseRegion,&Area,&CenterRow,&CenterCol);
		m_vPos[0].m_x =CenterRow[0].D();
		m_vPos[0].m_y =CenterCol[0].D();
		//
		Hobject Cross,CrossReg;
		gen_cross_contour_xld(&Cross, ResultRow, ResultColumn, 6, 0.785398);
		gen_region_contour_xld(Cross,&CrossReg,"margin");

		if(m_bDebugImg)
		{
			concat_obj (m_vRgn[id], SearchRgn, &m_vRgn[id]);
			concat_obj (m_vRgn[id], CrossReg, &m_vRgn[id]);
		}

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_vPos[0].m_x = 0.0;
		m_vPos[0].m_y =0.0;
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::SideCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("726$\
					 381;B$\
					 939;LP;10;100;1;60$\
					 938;LP;10;200;1;60$\
					 940;LP;1;1250;1;1133$\
					 941;LP;1;800;1;396$\
					 6039;LP;1;250;1;20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::SideRegionLocationAlgo(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pErosioSizeH,VARIANT* pErosioSizeV)
{
	int id = (int)rgnId->fltVal;
	int MoveSize    = (int)pMoveSize->fltVal;

	int ErosionSizeH =(int)pErosioSizeH->fltVal;
	int ErosionSizeV =(int)pErosioSizeV->fltVal;

	float fCenterX = 0.0;
	float fCenterY = 0.0;

	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject ImageDomain,TotalRgion,TotalRgionMoved,TotalBaseLine;
		get_domain(m_crtImg,&ImageDomain);
		move_region(m_BaseRegion,&TotalRgionMoved, MoveSize, 0);
		intersection(ImageDomain,TotalRgionMoved,&TotalRgion);
		Hobject RegionClipped,DetectRegion;
		clip_region_rel(TotalRgion,&RegionClipped,0,0,ErosionSizeH,ErosionSizeH);
		dilation_rectangle1(RegionClipped,&DetectRegion,1,ErosionSizeV);
		m_vRgn[id]=DetectRegion;

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::SideRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6040$\
					 167;B$\
					 6041;LP;1;300;1;60$\
					 994;LP;1;100;1;20$\
					 995;LP;1;300;1;20"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//dimension measurement
STDMETHODIMP CCanAlgo::CenterLocationAlgoM(VARIANT* rgnId,VARIANT* pThreshValue)
{
	/*中心定位，初步获取工件中心点坐标*/
	int id = (int)rgnId->fltVal;
	int ThreshValue = (int)pThreshValue->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image = m_crtImg;
		Hlong Width, Height;
		get_image_size (Image, &Width, &Height);
		//
		Hobject Ellipse,ImageReduced,Regions,ConnectedRegions,CenterRegion;
		HTuple CenterArea, CenterRow, CenterColumn;
		//gen_ellipse(&Ellipse, m_imgHeight/2, m_imgWidth/2, 0, m_imgWidth/2, m_imgHeight/2);
		//reduce_domain(Image, Ellipse, &ImageReduced);
		threshold(Image, &Regions, 0, ThreshValue);
		connection(Regions, &ConnectedRegions);
		select_shape_std(ConnectedRegions, &CenterRegion, "max_area", 70);
		area_center(CenterRegion, &CenterArea, &CenterRow, &CenterColumn);
		Hobject Circle;
		if ((CenterArea!=0)&& (CenterArea.Num()!=0))
		{	
			gen_circle(&Circle, CenterRow,CenterColumn,10);
			m_vPos[0].m_y = (float)CenterRow[0].D();
			m_vPos[0].m_x = (float)CenterColumn[0].D();
		}
		else
		{
			gen_empty_region(&Circle);
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
		}
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[id],CenterRegion,&m_vRgn[id]);
			concat_obj(m_vRgn[id],Circle,&m_vRgn[id]);
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::CenterLocationAlgoMHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1315$\
					 381;B$\
					 1316;LP;1;255;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::RectangleLocationAlgo(VARIANT* rgnId, VARIANT* pWidthPlaceSet,VARIANT* pRegionHeight, VARIANT* pRegionWidth)
{
	//得到矩形检测区域
	int id = (int)rgnId->fltVal;

	int WidthPlaceSet =(int)pWidthPlaceSet->fltVal;
	int HeightSet =(int)(pRegionHeight->fltVal);
	int WidthSet =(int)(pRegionWidth->fltVal);

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		set_check("~give_error");
		Hobject Image=m_crtImg;
		Hobject BaseRect,ImageDomain;
		get_domain(Image,&ImageDomain);

		//
		if((WidthPlaceSet<2)||(WidthPlaceSet>fCenterX))
		{
			WidthPlaceSet=10;
		}
		if((HeightSet*2>m_imgHeight-10)||(HeightSet<5))
		{
			HeightSet=10;
		}
		if((WidthSet*2>m_imgWidth-10)||(WidthSet<5))
		{
			HeightSet=10;
		}
		gen_rectangle2(&BaseRect, fCenterY, fCenterX-WidthPlaceSet, 0, WidthSet, HeightSet);
		Hobject DetectRegion;
		intersection(ImageDomain,BaseRect,&DetectRegion);
		m_vRgn[id] = DetectRegion;	
		set_check("give_error");
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::RectangleLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("708$\
					 167;B$\
					 825;LP;1;700;1;130$\
					 667;LP;20;800;1;300$\
					 668;LP;5;200;1;80"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::BaseLineDetectAlgo(VARIANT* rgnId, VARIANT* pThredLow, VARIANT* pElements,VARIANT* pTransition,VARIANT* pSelect,VARIANT* pSigma,VARIANT* pThreshold,VARIANT* pDefectArea)
{
	int rId           = (int)rgnId->fltVal;
	int ThredLow   = (int)pThredLow->fltVal;

	int Elements = (int)pElements->fltVal;
	int Transition = (int)pTransition->fltVal;
	int Select = (int)pSelect->fltVal;
	double Sigma = (double)pSigma->fltVal;
	double Threshold = (double)pThreshold->fltVal;

	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		Hobject crtImg = m_crtImg;
		Hobject ImageReduced,Edges,UnionContours;
		reduce_domain(crtImg, rgn, &ImageReduced);
		edges_sub_pix(ImageReduced, &Edges, "canny", 1, ThredLow, ThredLow*2);
		union_collinear_contours_xld(Edges, &UnionContours, 10, 1, 2, 0.1, "attr_keep");
		//
		HTuple num,length,Ln;
		Hobject BaseEdge;
		count_obj(Edges, &num);
		if (num<1)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectArea);
			return S_FALSE;
		}
		length_xld(Edges, &length);
		tuple_sort_index(length, &Ln);
		select_obj(Edges, &BaseEdge, (Ln.Select(num-1))+1);
		//
		HTuple RowBegin, ColBegin, RowEnd, ColEnd, Nr, Nc, Dist;
		fit_line_contour_xld(BaseEdge, "tukey", -1, 0, 5, 2, &RowBegin, &ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
		HTuple ResultRow, ResultColumn;
		ImageProcessAlg.ExtractEdgesPoints(ImageReduced,Elements,50,10,Sigma,Threshold,Transition,Select,RowBegin, ColBegin, RowEnd, ColEnd,&ResultRow, &ResultColumn);
		if (m_bDebugImg)
		{
			Hobject Cross1;
			gen_cross_contour_xld(&Cross1, ResultRow, ResultColumn, 26, 0.785398);
			concat_obj(m_vRgn[rId],Cross1,&m_vRgn[rId]);
		}
		HTuple BaseRowBegin0, BaseColBegin0, BaseRowEnd0, BaseColEnd0;
		ImageProcessAlg.FitLineUseEdgesPoints(ResultRow, ResultColumn, Elements, &BaseRowBegin0,  &BaseColBegin0, &BaseRowEnd0, &BaseColEnd0);

		//图像校正
		HTuple Angle;
		line_orientation(BaseRowBegin0, BaseColBegin0, BaseRowEnd0, BaseColEnd0, &Angle);
		HTuple Angle1,Angle2;
		if (0 != (Angle<0))
		{
			Angle1 = Angle+(2*PI);
			Angle2 = -(HTuple(90).Rad());
		}
		else
		{
			Angle1 = Angle;
			Angle2 = HTuple(90).Rad();
		}
		m_Angle1=Angle1;
		m_Angle2=Angle2;
		HTuple HomMat2D1;
		Hobject ImageAffinTrans,ImageReduced2;
		vector_angle_to_rigid(m_imgHeight/2, m_imgWidth/2, Angle1, m_imgHeight/2, m_imgWidth/2, Angle2, &HomMat2D1);
		affine_trans_image(ImageReduced, &ImageAffinTrans, HomMat2D1, "constant", "false");
		//m_ImageAffinTrans=ImageAffinTrans;
		reduce_domain(ImageAffinTrans, rgn, &ImageReduced2);

		//作为计算罐口厚度的基准线
		HTuple ResultRow1, ResultColumn1;
		ImageProcessAlg.ExtractEdgesPoints(ImageReduced2,Elements,50,10,Sigma,Threshold,Transition,Select,RowBegin, ColBegin, RowEnd, ColEnd,&ResultRow1, &ResultColumn1);
		HTuple BaseRowBegin1, BaseColBegin1, BaseRowEnd1, BaseColEnd1; 
		ImageProcessAlg.FitLineUseEdgesPoints(ResultRow1, ResultColumn1, Elements, &BaseRowBegin1,  &BaseColBegin1, &BaseRowEnd1, &BaseColEnd1);
		Hobject BaseLine;
		gen_region_line(&BaseLine,BaseRowBegin1, BaseColBegin1, BaseRowEnd1, BaseColEnd1);
		m_BaseRowBegin=BaseRowBegin1;
		m_BaseColBegin=BaseColBegin1;
		m_BaseRowEnd=BaseRowEnd1;
		m_BaseColEnd=BaseColEnd1;

		//作为计算罐口外径的基准线
		HTuple Deg0,Deg1;
		line_orientation(BaseRowBegin1, BaseColBegin1, BaseRowEnd1, BaseColEnd1, &Deg0);
		if (0 != (Deg0<0))
		{
			Deg1 = Deg0+(2*PI);
		}
		else
		{
			Deg1 = Deg0;
		}
		HTuple Length,Deg,RowMid,ColMid,RowMid1,ColMid1;
		Length = 90;
		Deg = Deg1;
		RowMid = (BaseRowBegin1+BaseRowEnd1)/2;
		ColMid = (BaseColBegin1+BaseColEnd1)/2;
		RowMid1 = RowMid+(Length*((Deg).Cos()));
		ColMid1 = ColMid+(Length*((Deg).Sin()));
		m_MidRowBegin=RowMid;
		m_MidColBegin=ColMid;
		m_MidRowEnd=RowMid1;
		m_MidColEnd=ColMid1;

		//用于debug显示
		HTuple HomMat2D2;
		Hobject BaseLineDisp;
		vector_angle_to_rigid(m_imgHeight/2, m_imgWidth/2, Angle2, m_imgHeight/2, m_imgWidth/2, Angle1, &HomMat2D2);
		if (m_bDebugImg)
		{
			affine_trans_region(BaseLine, &BaseLineDisp, HomMat2D2,"nearest_neighbor");
			concat_obj(m_vRgn[rId],BaseLineDisp,&m_vRgn[rId]);
		}

		HTuple Area, Row_test, Column_test;
		area_center(BaseLine, &Area, &Row_test, &Column_test);
		if(Area <= 1)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectArea);
			return S_FALSE;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (BaseLineDisp, &BaseLineDisp, 2.5);
			m_vErrorRgn[rId] = BaseLineDisp;
		}		
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::BaseLineDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1318$\
					 397;R;FP;0;300;1$\
					 381;B$\
					 1317;LP;3;100;1;20$\
					 1300;LP;2;100;1;75$\
					 1302;LP;0;1;1;1$\
					 1303;LP;0;1;1;1$\
					 680;FP;0.1;10.0;0.1;1.0$\
					 1304;FP;0.1;50.0;0.1;10.0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::RectangleLocation2Algo(VARIANT* rgnId, VARIANT* pWidthPlaceSet,VARIANT*pHeightPlaceSet,VARIANT* pRegionHeight, VARIANT* pRegionWidth)
{
	//以基准线为基础，生成两个矩形检测区域
	int id = (int)rgnId->fltVal;

	int WidthPlaceSet =(int)pWidthPlaceSet->fltVal;
	int HeightPlaceSet =(int)pHeightPlaceSet->fltVal;
	int HeightSet =(int)(pRegionHeight->fltVal);
	int WidthSet =(int)(pRegionWidth->fltVal);

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		set_check("~give_error");
		Hobject Image=m_crtImg;
		Hobject BaseRect,ImageDomain;
		get_domain(Image,&ImageDomain);

		if((WidthPlaceSet<2)||(WidthPlaceSet>fCenterX))
		{
			WidthPlaceSet=10;
		}
		if((HeightPlaceSet<2)||(HeightPlaceSet>fCenterY))
		{
			HeightPlaceSet=10;
		}
		if((HeightSet*2>m_imgHeight-10)||(HeightSet<5))
		{
			HeightSet=10;
		}
		if((WidthSet*2>m_imgWidth-10)||(WidthSet<5))
		{
			WidthSet=10;
		}

		//上拐角矩形区域
		HTuple RowRectu,ColRectu;
		RowRectu = (m_BaseRowBegin+m_BaseRowEnd)/2;
		ColRectu = (m_BaseColBegin+m_BaseColEnd)/2;

		Hobject RectRegU,RectRegD,RectRegs;
		gen_rectangle2(&RectRegU, RowRectu-HeightPlaceSet, ColRectu+WidthPlaceSet, m_Angle1, HeightSet, WidthSet);
		gen_rectangle2(&RectRegD, RowRectu+HeightPlaceSet, ColRectu+WidthPlaceSet, m_Angle1, HeightSet, WidthSet);
		union2(RectRegU,RectRegD,&RectRegs);

		Hobject DetectRegion;
		intersection(ImageDomain,RectRegs,&DetectRegion);
		m_vRgn[id] = DetectRegion;	
		set_check("give_error");
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::RectangleLocation2AlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("708$\
					 167;B$\
					 825;LP;1;700;1;370$\
					 826;LP;1;700;1;90$\
					 667;LP;20;800;1;80$\
					 668;LP;5;200;1;50"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::HeightMeasureAlgo(VARIANT* rgnId,VARIANT* pThredValue,VARIANT* pOffset,VARIANT* pDefectDiff)
{
	int rId           = (int)rgnId->fltVal;
	int ThredValue   = (int)pThredValue->fltVal;
	double Offset    = (double)pOffset->fltVal;

	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		Hobject crtImg = m_crtImg;
		Hobject rgnConnected;
		HTuple Number;
		connection(rgn,&rgnConnected);
		count_obj(rgnConnected,&Number);
		if (Number!=2)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectDiff);
			return S_FALSE;
		}
		Hobject SortedRegions,RectRegU,RectRegD;
		sort_region(rgnConnected, &SortedRegions, "upper_left", "true", "row");
		////上拐角点
		Hobject ImageRectU,ImageMedianU,BorderXldUp;
		select_obj(SortedRegions, &RectRegU, 1);
		//
		HTuple HomMat2D1;
		vector_angle_to_rigid(m_imgHeight/2, m_imgWidth/2, m_Angle1, m_imgHeight/2, m_imgWidth/2, m_Angle2, &HomMat2D1);
		Hobject ImageRectUAffinTrans;
		reduce_domain(crtImg,RectRegU, &ImageRectU);
		affine_trans_image(ImageRectU, &ImageRectUAffinTrans, HomMat2D1, "constant", "false");
		reduce_domain(ImageRectUAffinTrans, RectRegU, &ImageRectU);
		//`
		median_image(ImageRectU, &ImageMedianU, "circle", 1.0, "mirrored");
		threshold_sub_pix(ImageMedianU, &BorderXldUp, ThredValue);
		//
		HTuple num,length,Ln;
		Hobject BorderUp;
		count_obj(BorderXldUp, &num);
		if (num<1)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectDiff);
			return S_FALSE;
		}
		length_xld(BorderXldUp, &length);
		tuple_sort_index(length, &Ln);
		select_obj(BorderXldUp, &BorderUp, (Ln.Select(num-1))+1);
		//
		HTuple RowUp, ColUp,RowUpMax,RowIndice;
		get_contour_xld(BorderUp, &RowUp, &ColUp);
		tuple_max(RowUp, &RowUpMax);
		tuple_find(RowUp, RowUpMax, &RowIndice);
		//筛选拐点
		HTuple ColUps,Col;
		ColUps = HTuple();
		for (int x=0; x<=RowIndice.Num()-1; x++)
		{
			Col = ColUp.Select(RowIndice.Select(x));
			ColUps = ColUps.Concat(Col);
		}
		HTuple ColUpMin;
		tuple_min(ColUps, &ColUpMin);	
		HTuple HomMat2D2;
		vector_angle_to_rigid(m_imgHeight/2, m_imgWidth/2, m_Angle2, m_imgHeight/2, m_imgWidth/2, m_Angle1, &HomMat2D2);
		Hobject CrossUp,CrossUpAffinTrans;
		gen_cross_contour_xld(&CrossUp, RowUpMax, ColUpMin, 26, 0.785398);
		affine_trans_contour_xld(CrossUp, &CrossUpAffinTrans, HomMat2D2);
		//调试显示
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[rId],CrossUpAffinTrans,&m_vRgn[rId]);
		}

		////下拐角点
		select_obj(SortedRegions, &RectRegD, 2);
		Hobject ImageRectD,ImageMedianD,BorderXldDown;
		//
		Hobject ImageRectDAffinTrans;
		reduce_domain(crtImg,RectRegD, &ImageRectD);
		affine_trans_image(ImageRectD, &ImageRectDAffinTrans, HomMat2D1, "constant", "false");
		reduce_domain(ImageRectDAffinTrans, RectRegD, &ImageRectD);
		//`
		median_image(ImageRectD, &ImageMedianD, "circle", 1.0, "mirrored");
		threshold_sub_pix(ImageMedianD, &BorderXldDown, ThredValue);
		//
		Hobject BorderDown;
		count_obj(BorderXldDown, &num);
		if (num<1)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectDiff);
			return S_FALSE;
		}
		length_xld(BorderXldDown, &length);
		tuple_sort_index(length, &Ln);
		select_obj(BorderXldDown, &BorderDown, (Ln.Select(num-1))+1);
		//
		HTuple RowDown, ColDown,RowDownMin;
		get_contour_xld(BorderDown, &RowDown, &ColDown);
		tuple_min(RowDown, &RowDownMin);
		tuple_find(RowDown, RowDownMin, &RowIndice);
		//筛选拐点
		HTuple ColDowns,Col1;
		ColDowns = HTuple();
		for (int y=0; y<=(RowIndice.Num())-1; y ++)
		{
			Col1 = ColDown.Select(RowIndice.Select(y));
			ColDowns = ColDowns.Concat(Col1);
		}
		HTuple ColDownMin;
		tuple_min(ColDowns, &ColDownMin);

		Hobject CrossDown,CrossDownAffinTrans;
		gen_cross_contour_xld(&CrossDown, RowDownMin, ColDownMin, 26, 0.785398);
		affine_trans_contour_xld(CrossDown, &CrossDownAffinTrans, HomMat2D2);
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[rId],CrossDownAffinTrans,&m_vRgn[rId]);
		}
		//计算
		if ((ColUpMin.Num()!=1)||(ColDownMin.Num()!=1))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectDiff);
			return S_FALSE;
		}

		HTuple UpDistance,DownDistance,Height;
		distance_pl(RowUpMax, ColUpMin, m_BaseRowBegin, m_BaseColBegin, m_BaseRowEnd, m_BaseColEnd, &UpDistance);
		distance_pl(RowDownMin, ColDownMin, m_BaseRowBegin, m_BaseColBegin, m_BaseRowEnd, m_BaseColEnd, &DownDistance);
		Height = (UpDistance+DownDistance)/2;
		//
		retValue =abs(Height[0].D()*m_pixelSize+Offset);
		Hobject ErrorReg,Region1,Region2;
		gen_region_contour_xld (CrossUpAffinTrans, &Region1, "margin");
		gen_region_contour_xld (CrossDownAffinTrans, &Region2, "margin");
		union2(Region1,Region2,&ErrorReg);
		union1(ErrorReg,&ErrorReg);
		m_vErrorRgn[rId] = ErrorReg;
		retValue.Detach(pDefectDiff);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::HeightMeasureAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1319$\
					 397;R;FP;0;300;1$\
					 381;B$\
					 1317;LP;3;250;1;128$\
					 1321;FP;-10;10;0.01;0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::DiameterMeasureAlgo(VARIANT* rgnId,VARIANT* pThredValue, VARIANT* pElements,VARIANT* pTransition,VARIANT* pSelect,VARIANT* pSigma,VARIANT* pThreshold,VARIANT* pOffset,VARIANT* pDefectDiff)
{
	int rId           = (int)rgnId->fltVal;
	int ThredValue   = (int)pThredValue->fltVal;

	int Elements = (int)pElements->fltVal;
	int Transition = (int)pTransition->fltVal;
	int Select = (int)pSelect->fltVal;
	double Sigma = (double)pSigma->fltVal;
	double Threshold = (double)pThreshold->fltVal;
	double Offset    = (double)pOffset->fltVal;

	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		Hobject crtImg = m_crtImg;
		Hobject rgnConnected;
		HTuple Number;
		connection(rgn,&rgnConnected);
		count_obj(rgnConnected,&Number);
		if (Number!=2)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectDiff);
			return S_FALSE;
		}
		Hobject SortedRegions,RectRegU,RectRegD;
		sort_region(rgnConnected, &SortedRegions, "upper_left", "true", "row");
		////上边缘
		Hobject ImageRectU,ImageMedianU,BorderXldUp;
		select_obj(SortedRegions, &RectRegU, 1);
		//
		HTuple HomMat2D1;
		vector_angle_to_rigid(m_imgHeight/2, m_imgWidth/2, m_Angle1, m_imgHeight/2, m_imgWidth/2, m_Angle2, &HomMat2D1);
		Hobject ImageRectUAffinTrans;
		reduce_domain(crtImg,RectRegU, &ImageRectU);
		affine_trans_image(ImageRectU, &ImageRectUAffinTrans, HomMat2D1, "constant", "false");
		reduce_domain(ImageRectUAffinTrans, RectRegU, &ImageRectU);
		//`
		median_image(ImageRectU, &ImageMedianU, "circle", 1.0, "mirrored");
		threshold_sub_pix(ImageMedianU, &BorderXldUp, ThredValue);
		//
		HTuple num,length,Ln;
		Hobject BorderUp;
		count_obj(BorderXldUp, &num);
		if (num<1)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectDiff);
			return S_FALSE;
		}
		length_xld(BorderXldUp, &length);
		tuple_sort_index(length, &Ln);
		select_obj(BorderXldUp, &BorderUp, (Ln.Select(num-1))+1);

		HTuple RowBegin1, ColBegin1, RowEnd1, ColEnd1, Nr1, Nc1, Dist1;
		fit_line_contour_xld(BorderUp, "tukey", -1, 0, 5, 2, &RowBegin1, &ColBegin1, &RowEnd1, &ColEnd1, &Nr1, &Nc1, &Dist1);
		HTuple ResultRow1, ResultColumn1;
		ImageProcessAlg.ExtractEdgesPoints(ImageRectU,Elements,50,10,Sigma,Threshold,Transition,Select,RowBegin1, ColBegin1, RowEnd1, ColEnd1,&ResultRow1, &ResultColumn1);
		//调试显示
		HTuple HomMat2D2;
		vector_angle_to_rigid(m_imgHeight/2, m_imgWidth/2, m_Angle2, m_imgHeight/2, m_imgWidth/2, m_Angle1, &HomMat2D2);
		Hobject CrossUp,CrossUpAffinTrans;
		gen_cross_contour_xld(&CrossUp,  ResultRow1, ResultColumn1, 20, 0.785398);
		affine_trans_contour_xld(CrossUp, &CrossUpAffinTrans, HomMat2D2);
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[rId],CrossUpAffinTrans,&m_vRgn[rId]);
		}

		////下边缘
		select_obj(SortedRegions, &RectRegD, 2);
		Hobject ImageRectD,ImageMedianD,BorderXldDown;
		//
		Hobject ImageRectDAffinTrans;
		reduce_domain(crtImg,RectRegD, &ImageRectD);
		affine_trans_image(ImageRectD, &ImageRectDAffinTrans, HomMat2D1, "constant", "false");
		reduce_domain(ImageRectDAffinTrans, RectRegD, &ImageRectD);
		//`
		median_image(ImageRectD, &ImageMedianD, "circle", 1.0, "mirrored");
		threshold_sub_pix(ImageMedianD, &BorderXldDown, ThredValue);
		//
		Hobject BorderDown;
		count_obj(BorderXldDown, &num);
		if (num<1)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectDiff);
			return S_FALSE;
		}
		length_xld(BorderXldDown, &length);
		tuple_sort_index(length, &Ln);
		select_obj(BorderXldDown, &BorderDown, (Ln.Select(num-1))+1);
		//
		HTuple RowBegin2, ColBegin2, RowEnd2, ColEnd2, Nr2, Nc2, Dist2;
		fit_line_contour_xld(BorderDown, "tukey", -1, 0, 5, 2, &RowBegin2, &ColBegin2, &RowEnd2, &ColEnd2, &Nr2, &Nc2, &Dist2);
		HTuple ResultRow2, ResultColumn2;
		ImageProcessAlg.ExtractEdgesPoints(ImageRectD,Elements,50,10,Sigma,Threshold,Transition,Select,RowBegin2, ColBegin2, RowEnd2, ColEnd2,&ResultRow2, &ResultColumn2);
		//调试显示
		Hobject CrossDown,CrossDownAffinTrans;
		gen_cross_contour_xld(&CrossDown, ResultRow2, ResultColumn2, 20, 0.785398);
		affine_trans_contour_xld(CrossDown, &CrossDownAffinTrans, HomMat2D2);
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[rId],CrossDownAffinTrans,&m_vRgn[rId]);
		}
		//计算
		HTuple UpDistance,UpDistMean;
		distance_pl(ResultRow1, ResultColumn1, m_MidRowBegin, m_MidColBegin, m_MidRowEnd, m_MidColEnd, &UpDistance);
		tuple_mean(UpDistance, &UpDistMean);
		HTuple DownDistance,DownDistMean,Diameter;
		distance_pl(ResultRow2, ResultColumn2, m_MidRowBegin, m_MidColBegin, m_MidRowEnd, m_MidColEnd, &DownDistance);
		tuple_mean(DownDistance, &DownDistMean);
		Diameter = (UpDistMean+DownDistMean)/2;

		//
		retValue =abs(Diameter[0].D()*m_pixelSize+Offset);
		Hobject ErrorReg,Region1,Region2;
		gen_region_contour_xld (CrossUpAffinTrans, &Region1, "margin");
		gen_region_contour_xld (CrossDownAffinTrans, &Region2, "margin");
		union2(Region1,Region2,&ErrorReg);
		union1(ErrorReg,&ErrorReg);
		m_vErrorRgn[rId] = ErrorReg;
		retValue.Detach(pDefectDiff);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}

#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::DiameterMeasureAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1320$\
					 397;R;FP;0;300;1$\
					 381;B$\
					 1317;LP;3;250;1;128$\
					 1300;LP;2;100;1;75$\
					 1302;LP;0;1;1;1$\
					 1303;LP;0;1;1;1$\
					 680;FP;0.1;10.0;0.1;1.0$\
					 1304;FP;0.1;50.0;0.1;10.0$\
					 1321;FP;-10;10;0.01;0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCanAlgo::InnerDiameterMeasureAlgo(VARIANT* rgnId,VARIANT* pSwitch,VARIANT* pOffset,VARIANT* pDefectDiff)
{
	int rId       = (int)rgnId->fltVal;
	int Switch    = (int)pSwitch->fltVal;
	double Offset    = (double)pOffset->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		if(m_CenterRadius == -1)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDefectDiff);
			return S_FALSE;
		}
		double pixelSize;
		if (Switch==0)
		{
			pixelSize=m_pixelSize;
		} 
		else if(Switch==1)
		{
			pixelSize=1;
		}
		//计算
		HTuple Diameter;
		Diameter = m_CenterRadius*2;
		//
		retValue =abs(Diameter[0].D()*pixelSize+Offset);
		m_vErrorRgn[rId] = rgn;
		retValue.Detach(pDefectDiff);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::InnerDiameterMeasureAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1328$\
					 397;R;FP;0;300;1$\
					 381;B$\
					 1417;LP;0;1;1;0$\
					 1321;FP;-10;10;0.01;0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}



//double end 
STDMETHODIMP CCanAlgo::DoubleEndRegionLocationAlgo(VARIANT* rgnId, VARIANT* pBottomRow, VARIANT* pRadiusWidth, VARIANT* pRadiusHeight, VARIANT* pRegionHeight, VARIANT* pRegionWidth)
{
	//得到双盖检测区域
	int id = (int)rgnId->fltVal;

	int CenterRow =(int)pBottomRow->fltVal;
	int Radius1 =(int)pRadiusWidth->fltVal;
	int Radius2 =(int)pRadiusHeight->fltVal;
	int MoveRow =(int)pRegionHeight->fltVal;
	int RegionWitdhHalf =(int)(pRegionWidth->fltVal/2.0);
	m_maxRow=CenterRow+Radius2;
	m_minRow=m_maxRow-MoveRow;
	if (m_minRow<8)
	{
		m_minRow=8;
	}
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  RegionDetect;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		set_check("~give_error");
		Hobject Image=m_crtImg;
		HTuple Width, Height,CenterCol;
		Hobject Rectangle,Ellipse,Ellipse1,RegionDifference;
		get_image_size(Image, &Width, &Height);
		//
		if((m_minRow<=100)||(m_maxRow>Height-10)||(RegionWitdhHalf>Width/2.0-10)||(CenterRow<Radius2+10))
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		//
		CenterCol = Width/2.0;
		gen_rectangle1(&Rectangle, 10, CenterCol-RegionWitdhHalf, Height-10, CenterCol+RegionWitdhHalf);
		gen_ellipse(&Ellipse, CenterRow, CenterCol, 0, Radius1, Radius2);
		move_region(Ellipse, &Ellipse1, -MoveRow, 0);
		difference(Ellipse, Ellipse1, &RegionDifference);
		intersection(RegionDifference, Rectangle, &RegionDetect);
		//计算校正数据 m_oldRadiusWidth,m_oldRadiusHeight,m_oldBottomRow,m_oldRegionWidth,m_oldRegionHeight;
		if (!m_bPilotImg)
		{
			if((m_oldBottomRow!=CenterRow)||(m_oldRadiusWidth!=Radius1)||(m_oldRadiusHeight!=Radius2)||(m_oldRegionWidth!=RegionWitdhHalf)||(m_oldRegionHeight!=MoveRow))
			{
				get_region_points(RegionDetect, &m_pointsRow, &m_pointsCol);
				tuple_max(m_pointsCol, &m_maxCol);
				tuple_min(m_pointsCol, &m_minCol);
				HTuple Length;
				tuple_length(m_pointsRow, &Length);	
				if (Length>100) 
				{
					m_highsOffsetAll = HTuple();	
					tuple_gen_const(Length, 0, &m_highsOffsetAll);
				}
				else
				{
					m_vRgn[id] = newRgn ;
					return S_FALSE;
				}
				if((m_oldRadiusWidth!=Radius1)||(m_oldRadiusHeight!=Radius2))
				{
					m_highsOffset = HTuple();
					tuple_gen_const(Width, 0, &m_highsOffset);
					double RadiusPara = Radius2/(Radius1+0.0001);
					HTuple RadiusParaPowDiffHTuple =(Radius1*Radius1)-(CenterCol*CenterCol);
					double RadiusParaPowDiff=RadiusParaPowDiffHTuple[0].D();
					for (int Index1=m_minCol[0].I(); Index1<=m_maxCol[0].I(); Index1+=1)
					{
						m_highsOffset[Index1] = Radius2-(RadiusPara*((RadiusParaPowDiff-(Index1*(Index1-(2*CenterCol)))).Sqrt()));
					}

				}	
				for (int Index1=m_minCol[0].I(); Index1<=m_maxCol[0].I(); Index1+=1)
				{
					HTuple Indices,Length1;
					tuple_find(m_pointsCol, Index1, &Indices);
					tuple_length(Indices, &Length1);
					for (int Index3=0; Index3<=Length1-1; Index3+=1)
					{
						m_highsOffsetAll[HTuple(Indices[Index3])] = HTuple(m_highsOffset[Index1]);
					}
				}
				tuple_add(m_pointsRow, m_highsOffsetAll, &m_pointsRowReset);
				m_pointsRowReset = m_pointsRowReset.Round();
				m_oldBottomRow=CenterRow;
				m_oldRadiusWidth=Radius1;
				m_oldRadiusHeight=Radius2;
				m_oldRegionWidth=RegionWitdhHalf;
				m_oldRegionHeight=MoveRow;
			}
		}
		m_vRgn[id] = RegionDetect ;	
		set_check("give_error");
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::DoubleEndRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("663$\
					 167;B$\
					 664;LP;100;1600;1;450$\
					 665;LP;80;1200;1;550$\
					 666;LP;60;1000;1;265$\
					 667;LP;10;800;1;150$\
					 668;LP;80;1600;1;640"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanAlgo::DoubleEndDetectAlgo(VARIANT* rgnId, VARIANT* pSigmaSize, VARIANT* pBlackThredLow,VARIANT* pBlackThredHigh, VARIANT* pBlackDefectSize, VARIANT* pDetectArea)
{
	//盖面找线算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue         = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	float BlackMaskSize = (float)pSigmaSize->fltVal;
	double BlackThredLow = (double)pBlackThredLow->fltVal;
	int BlackThredUpper = (int)pBlackThredHigh->fltVal;
	int BlackDefectSize = (int)pBlackDefectSize->fltVal;
	int BlackDefectSizeUpper = 200;
	int WhiteDefectSizeUpper = 200;
#ifdef NDEBUG
	try
	{
#endif
		//
		if (m_bPilotImg)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}

		set_check("~give_error");
		Hobject Image = m_crtImg;
		Hobject  Image1, RegionDetect, ImageReduced,DefectRegs;
		Hobject  ExpandedImage, ImageClosing, ImageOpening, ImageMean;
		Hobject  ImageMean2, RegionForBack, ImageReduced1, ImageReduced2;
		Hobject  ImageSub, Region1, RegionIntersection2, RegionErosion1;
		Hobject  RegionErosion2, ConnectedRegions1, RegionFillUp1;
		Hobject  SortedRegions, ObjectSelected, RegionMoved, RegionOpeningLine;
		Hobject  Contours, ClippedContours, SortedContours1, RegionLinesBottom;
		Hobject  Edges, ImageZoomed, ContoursAffinTrans, SelectedXLD;
		Hobject  UnionContours, SortedContours, RegionLines, ModelRegions;
		Hobject  RegionDetectErosion, RegionIntersection, Region2;
		//
		HTuple  Grayvals, Width, Height,Area;
		HTuple  Row3, Column3, GrayvalsMean, Number, Row11, Column11;
		HTuple  Row21, Column21, LengthBottoms, LengthBottomNumber;
		HTuple  RowBegin1, ColBegin1, RowEnd1, ColEnd1, Nr1, Nc1;
		HTuple  Dist1, ATan, BottomAngle, BottomRowDist, HomMat2D;
		HTuple  HomMat2DScale, LineLength, LineLengthNumber, RowBegin;
		HTuple  ColBegin, RowEnd, ColEnd, Nr, Nc, Dist, Quot, LineAngle;
		HTuple  LineRowDist, LineDistOfBottom, LineAngeOfBottom;
		HTuple  Rows1, Columns1, Min1, Max1, Index2, Indices2, Length2;
		HTuple  Index4;

		get_image_size(Image, &Width, &Height);
		get_grayval(Image, m_pointsRow, m_pointsCol, &Grayvals);
		gen_image_const(&Image1, "byte", Width, Height);
		set_grayval(Image1, m_pointsRowReset, m_pointsCol, Grayvals);
		m_vInterImgs[rId]=Image1;
		gen_rectangle1(&RegionDetect, m_minRow+4, m_minCol+4,m_maxRow-4,  m_maxCol-4);
		//*
		reduce_domain(Image1, RegionDetect, &ImageReduced);
		area_center(RegionDetect, &Area, &Row3, &Column3);
		//*
		expand_domain_gray(ImageReduced, &ExpandedImage, 13);
		//*********
		gray_closing_rect(ExpandedImage, &ImageClosing, 1, 21);
		gray_opening_rect(ImageClosing, &ImageOpening, 1, 21);
		mean_image(ImageOpening, &ImageMean, 9, 1);
		//*BottomLines
		gen_image_const(&ImageMean2, "byte", Width, Height);
		get_grayval(ImageMean, m_pointsRowReset, m_pointsCol, &GrayvalsMean);
		set_grayval(ImageMean2, m_pointsRowReset+2, m_pointsCol, GrayvalsMean);
		gen_rectangle1(&RegionForBack, m_minRow+4, ((m_minCol+m_maxCol)/2.0)-((m_maxCol-m_minCol)/4.0),m_maxRow-4,  ((m_minCol+m_maxCol)/2.0)+((m_maxCol-m_minCol)/4.0));
		reduce_domain(ImageMean, RegionDetect, &ImageReduced1);
		reduce_domain(ImageMean2, RegionDetect, &ImageReduced2);

		sub_image(ImageReduced2, ImageReduced1, &ImageSub, 3.5, 0);

		threshold(ImageSub, &Region1, 0, 20);

		intersection(Region1, RegionForBack, &RegionIntersection2);
		erosion_rectangle1(RegionIntersection2, &RegionErosion1, 11, 1);
		erosion_rectangle1(RegionErosion1, &RegionErosion2, 11, 1);

		connection(RegionErosion2, &ConnectedRegions1);
		fill_up(ConnectedRegions1, &RegionFillUp1);

		sort_region(RegionFillUp1, &SortedRegions, "lower_left", "true", "row");

		count_obj(SortedRegions, &Number);
		select_obj(SortedRegions, &ObjectSelected, Number);

		move_region(ObjectSelected, &RegionMoved, -2, 0);

		opening_rectangle1(RegionMoved, &RegionOpeningLine, 3, 11);

		inner_rectangle1(RegionOpeningLine, &Row11, &Column11, &Row21, &Column21);
		gen_contour_region_xld(RegionOpeningLine, &Contours, "border");
		clip_contours_xld(Contours, &ClippedContours, Row11-50, Column11+10, Row21-10, Column21-10);
		//
		Hobject SelectedXLDBottom;
		select_shape_xld(ClippedContours, &SelectedXLDBottom, "contlength", "and",10 , 99999);
		//
		sort_contours_xld(SelectedXLDBottom, &SortedContours1, "upper_left", "true", "row");
		length_xld(SortedContours1, &LengthBottoms);
		tuple_length(LengthBottoms, &LengthBottomNumber);
		if (0 != (LengthBottomNumber>=1))
		{
			fit_line_contour_xld(SortedContours1, "tukey", -1, 0, 5, 2, &RowBegin1, &ColBegin1, &RowEnd1, &ColEnd1, &Nr1, &Nc1, &Dist1);
			gen_region_line(&RegionLinesBottom, HTuple(RowBegin1[0]), HTuple(ColBegin1[0]), HTuple(RowEnd1[0]), HTuple(ColEnd1[0]));
			tuple_atan((-Nc1)/Nr1, &ATan);
			BottomAngle = ATan*(180.0/3.1416);
			BottomRowDist = (Dist1-((Nc1*Width)/2.0))/Nr1;
		}
		if (0 != (Width>=1024))
		{
			zoom_image_factor(ImageMean, &ImageZoomed, 0.5, 0.5, "bilinear");
			lines_gauss(ImageZoomed, &Edges, BlackMaskSize, BlackThredLow, BlackThredUpper, "dark", "true", "bar-shaped", "true");
			//
			vector_angle_to_rigid(Row3/2.0, Column3/2.0, 0, Row3, Column3, 0, &HomMat2D);
			hom_mat2d_scale(HomMat2D, 2.0, 2.0, Row3, Column3, &HomMat2DScale);
			affine_trans_contour_xld(Edges, &ContoursAffinTrans, HomMat2DScale);
			select_shape_xld(ContoursAffinTrans, &SelectedXLD, "contlength", "and",BlackDefectSize , 99999);
		}
		else
		{
			lines_gauss(ImageMean, &Edges, BlackMaskSize, BlackThredLow, BlackThredUpper, "dark", "true", "bar-shaped", "true");
			select_shape_xld(Edges, &SelectedXLD, "contlength", "and",BlackDefectSize, 99999);
		}
		union_collinear_contours_xld(SelectedXLD, &UnionContours, 10, 1, 2, 0.1, "attr_keep");
		sort_contours_xld(UnionContours, &SortedContours, "upper_left", "false", "row");
		length_xld(SortedContours, &LineLength);
		tuple_length(LineLength, &LineLengthNumber);
		if (0 != (LineLengthNumber>=1))
		{
			fit_line_contour_xld(SortedContours, "tukey", -1, 0, 5, 2, &RowBegin, &ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
			gen_region_line(&RegionLines, RowBegin, ColBegin, RowEnd, ColEnd);
			tuple_div(-Nc, Nr, &Quot);
			tuple_atan(Quot, &ATan);
			LineAngle = ATan*(180.0/3.1416);
			LineRowDist = (Dist-((Nc*Width)/2.0))/Nr;
			if (LengthBottomNumber>=1)
			{	
				LineDistOfBottom = HTuple(BottomRowDist[0])-LineRowDist;
				LineAngeOfBottom = (HTuple(BottomAngle[0])-LineAngle).Abs();
			}
			ImageProcessAlg.XldsToRegion(SortedContours, &ModelRegions, 0.0);
			erosion_circle(RegionDetect, &RegionDetectErosion, 2.5);
			intersection(ModelRegions, RegionDetectErosion, &RegionIntersection);
			//*转换到原始
			get_region_points(RegionIntersection, &Rows1, &Columns1);
			//
			HTuple LegnthEnd;
			tuple_length(Rows1,&LegnthEnd);
			if (LegnthEnd<10)
			{
				retValue = 0.0;
				Hobject newRgn; 
				m_vErrorRgn[rId] = newRgn;
				retValue.Detach(pDetectArea);
				return S_OK;
			}
			//
			tuple_min(Columns1, &Min1);
			tuple_max(Columns1, &Max1);
			//注意，当前范围不能超出校正范围乾
			for (Index2=Min1; Index2<=Max1; Index2+=1)
			{
				tuple_find(Columns1, Index2, &Indices2);

				tuple_length(Indices2, &Length2);
				if (0 != (Length2>1))
				{

					for (Index4=0; Index4<=Length2-1; Index4+=1)
					{
						Rows1[HTuple(Indices2[Index4])] = HTuple(Rows1[HTuple(Indices2[Index4])])-HTuple(m_highsOffset[Index2]);
					}
				}
				else if (0 != (Length2==1))
				{
					if (0 != ((Indices2+1)>0))
					{
						Rows1[HTuple(Indices2[0])] = HTuple(Rows1[HTuple(Indices2[0])])-HTuple(m_highsOffset[Index2]);
					}
				}

			}
			gen_region_points(&DefectRegs, Rows1, Columns1);
			HTuple Area, Row_test, Column_test;
			area_center(DefectRegs, &Area, &Row_test, &Column_test);

			if(Area > 0)
			{
				retValue = Area[0].I();
				if (LengthBottomNumber>=1)
				{	
					concat_obj(RegionLinesBottom,DefectRegs,&DefectRegs);
				}
				m_vErrorRgn[rId] = DefectRegs;

			}
			else
			{
				retValue = 0.0;
				Hobject newRgn; 
				m_vErrorRgn[rId] = newRgn;
				retValue.Detach(pDetectArea);
				return S_OK;
			}
		}
		else
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
			retValue.Detach(pDetectArea);
			return S_OK;
		}
		retValue.Detach(pDetectArea);
		set_check("give_error");
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCanAlgo::DoubleEndDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("669$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 426;FP;0.1;5.0;0.1;1.5$\
					 644;FP;0.5;12;0.25;1.0$\
					 645;FP;1.5;20;0.5;3$\
					 428;LP;1;400;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


//利用边缘检测的方法进行双盖定位，定位成一个椭圆
//STDMETHODIMP CCanAlgo::D_EllipseLocationAlgo(VARIANT* rgnId,VARIANT* pRadiusWidth, VARIANT* pRadiusHeight, VARIANT* pEdgeThresh,VARIANT* pRegionCenterRow,VARIANT* pRegionWidth,VARIANT* pRegionHeight)
//{
//	int id = (int)rgnId->fltVal;
//	int RectWidth =(int)pRadiusWidth->fltVal;
//	int RectHeight =(int)pRadiusHeight->fltVal;
//	float EdgeThresh =(float)pEdgeThresh->fltVal;
//	int RegionCenterRow =(int)pRegionCenterRow->fltVal;
//	int RegionWidth =(int)pRegionWidth->fltVal;
//	int RegionHeight =(int)pRegionHeight->fltVal;
//
//	Hobject Image=m_crtImg;
//	Hobject ImageReduced,DerivGauss,Region,RegionFillUp,newRgn;
//	Hobject RegionClosing,ConnectedRegions,SelectedRegions,Contours;
//	Hobject Ellipse1,Ellipse2,RegionDifference,Rectangle,RegionIntersection;
//	HTuple Row1, Column1, Row2, Column2,RectCenterCol,Number,region_width;
//	HTuple CenterRow, CenterCol, Phi, Radius1, Radius2, StartPhi, EndPhi, PointOrder;
//
//#ifdef NDEBUG
//	try
//	{
//#endif
//		//
//		if(m_bDebugImg)
//		{
//			m_dictSpecialData[id].clear();
//		}
//
//
//		m_vRgn[id] = newRgn;
//		gen_rectangle2 (&Rectangle, m_imgHeight/2, m_imgWidth/2, 0, RectWidth, RectHeight);
//		reduce_domain (Image, Rectangle, &ImageReduced);
//		derivate_gauss (ImageReduced, &DerivGauss, 1, "y");
//		threshold (DerivGauss, &Region, -255, -EdgeThresh);
//		fill_up_shape (Region, &RegionFillUp,"area", 1, 900);
//
//		closing_circle(RegionFillUp, &RegionClosing,5.5);
//		connection (RegionClosing,&ConnectedRegions);
//		select_shape (ConnectedRegions,&ConnectedRegions, "width", "and", RectWidth/2.0, m_imgWidth);
//		select_shape (ConnectedRegions, &SelectedRegions, "row", "and",  RegionCenterRow-50,  RegionCenterRow+50);
//		fill_up (SelectedRegions, &SelectedRegions);
//		count_obj (SelectedRegions, &Number);
//		if(Number!=1)
//		{
//			m_vRgn[id] = RegionClosing ;
//			return S_FALSE;
//		}
//		gen_contour_region_xld (SelectedRegions, &Contours, "border");
//		fit_ellipse_contour_xld (Contours, "fitzgibbon", -1, 0, 0, 200, 3, 2,  &m_ellipseCenterRow, &m_ellipseCenterCol, &m_ellipsePhi, &m_ellipseRadius1, &m_ellipseRadius2, &StartPhi, &EndPhi, &PointOrder);
//		if(m_ellipseCenterRow.Num()!=1)
//		{
//			m_vRgn[id] = SelectedRegions ;
//			return S_FALSE;
//		}
//		gen_ellipse (&Ellipse1, m_ellipseCenterRow+8, m_ellipseCenterCol, m_ellipsePhi, m_ellipseRadius1, m_ellipseRadius2);
//		gen_ellipse (&Ellipse2, m_ellipseCenterRow-RegionHeight, m_ellipseCenterCol, m_ellipsePhi, m_ellipseRadius1, m_ellipseRadius2);
//		difference (Ellipse1, Ellipse2, &RegionDifference);
//		smallest_rectangle1 (RegionDifference, &Row1, &Column1, &Row2, &Column2);
//		RectCenterCol=(Column2+Column1)/2.0;
//
//		gen_rectangle1 (&Rectangle, Row1, RectCenterCol-RegionWidth/2.0, Row2, RectCenterCol+RegionWidth/2.0);
//		intersection (Rectangle, RegionDifference, &RegionIntersection);
//		region_features (RegionIntersection,"width", &region_width);
//		if(region_width<RegionWidth*0.9 || region_width>RegionWidth*1.1)
//		{
//			m_centerLocationRegion=RegionIntersection;
//			m_vRgn[id] = RegionIntersection ;
//			return S_FALSE;
//		}
//		m_centerLocationRegion=RegionIntersection;
//		m_vRgn[id] = RegionIntersection ;
//
//#ifdef NDEBUG
//	}
//	catch (HException &except) 
//	{
//		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
//		m_vRgn[id] = newRgn ;
//		return S_FALSE;
//	}
//#endif
//	return S_OK;
//}
//STDMETHODIMP CCanAlgo::D_EllipseLocationAlgoHelp(BSTR* pHelpStr)
//{
//	CComBSTR strHelp("8018$\
//					 167;B$\
//					 8019;LP;100;1600;1;450$\
//					 8020;LP;80;1200;1;550$\
//					 433;FP;0.01;255;0.5;10.5$\
//					 8022;LP;60;1000;1;265$\
//					 668;LP;10;800;1;250$\
//					 667;LP;10;1600;1;10"); 
//
//	HRESULT hr = strHelp.CopyTo(pHelpStr);
//	if(FAILED(hr))
//		return S_FALSE;
//	return S_OK;
//}

STDMETHODIMP CCanAlgo::D_EllipseLocationAlgo(VARIANT* rgnId, VARIANT* pRadiusWidth, VARIANT* pRadiusHeight, VARIANT* pEdgeThresh, VARIANT* pRegionCenterRow, VARIANT* pRegionWidth, VARIANT* pRegionHeight)
{
	//得到双盖检测区域
	int id = (int)rgnId->fltVal;
	int RectWidth = (int)pRadiusWidth->fltVal;
	int RectHeight = (int)pRadiusHeight->fltVal;
	float EdgeThresh = (float)pEdgeThresh->fltVal;
	int RegionCenterRow = (int)pRegionCenterRow->fltVal;
	int RegionWidth = (int)pRegionWidth->fltVal;
	int RegionHeight = (int)pRegionHeight->fltVal;
	Hobject newRgn, RegionDetect;
	HTuple Width, Height, CenterCol, Area11, Row11, Col11;
	Hobject Rectangle, Ellipse, Ellipse1, RegionDifference;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG
	try
	{
#endif
		//
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		get_image_size(Image, &Width, &Height);

		CenterCol = Width / 2.0;
		gen_rectangle1(&Rectangle, 10, CenterCol - RegionWidth / 2.0, Height - 10, CenterCol + RegionWidth / 2.0);
		gen_ellipse(&Ellipse, RegionCenterRow, CenterCol, 0, RectHeight, RectWidth);
		move_region(Ellipse, &Ellipse1, -RegionHeight, 0);
		difference(Ellipse, Ellipse1, &RegionDifference);
		intersection(RegionDifference, Rectangle, &RegionDetect);
		area_center(RegionDetect, &Area11, &Row11, &Col11);
		if (Area11 <= 0)
		{
			intersection(Ellipse, Rectangle, &Ellipse);
			m_vRgn[id] = Ellipse;
		}
		else
		{
			m_vRgn[id] = RegionDetect;
		}
		m_centerLocationRegion = m_vRgn[id];
		m_ellipseCenterRow = RegionCenterRow;
		m_ellipseCenterCol = CenterCol;
		m_ellipseRadius1 = RectHeight;
		m_ellipseRadius2 = RectWidth;

#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::D_EllipseLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("8018$\
					 167;B$\
					 8019;LP;100;1600;1;450$\
					 8020;LP;80;1200;1;550$\
					 433;FP;0.01;255;0.5;10.5$\
					 8022;LP;60;1000;1;265$\
					 668;LP;10;800;1;250$\
					 667;LP;10;1600;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if (FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CCanAlgo::D_CalibRegionExtract(VARIANT* rgnId)
{
	int id = (int)rgnId->fltVal;

	Hobject Image = m_crtImg;
	Hobject detect_region = m_centerLocationRegion;
	Hobject Image1, rectangle, Image2, newRgn, image_domain;
	HTuple point_rows, point_cols, max_col, min_col, points_num;
	HTuple hv_radius_ratio, hv_pow_diff, point_cols_sequence, hv_center_col_2;
	HTuple hv_sub, hv_m_ellipseRadius2, hv_prod1, hv_diff_sub, hv_sqrt1;
	HTuple hv_prod2, hv_highsOffset, hv_select_index, hight_offset_all;
	HTuple hv_min_col;

#ifdef NDEBUG
	try
	{
#endif
		//
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		if (m_bPilotImg)
		{
			return S_OK;
		}
		double radius_ratio, pow_diff, center_col_2;
		get_domain(Image, &image_domain);

		intersection(detect_region, image_domain, &detect_region);
		get_region_points(detect_region, &point_rows, &point_cols);
		if (point_cols.Num() <= 1)
		{
			m_vRgn[id] = detect_region;
			return S_FALSE;
		}
		tuple_max(point_cols, &max_col);
		tuple_min(point_cols, &min_col);
		points_num = max_col - min_col + 1;
		radius_ratio = (m_ellipseRadius2 / (m_ellipseRadius1 + 0.0001))[0].D();
		tuple_gen_const(points_num, radius_ratio, &hv_radius_ratio);
		pow_diff = ((m_ellipseRadius1*m_ellipseRadius1) - (m_ellipseCenterCol*m_ellipseCenterCol))[0].D();
		tuple_gen_const(points_num, pow_diff, &hv_pow_diff);
		tuple_gen_const(points_num, m_ellipseRadius2, &hv_m_ellipseRadius2);
		tuple_gen_sequence(min_col, max_col, 1, &point_cols_sequence);
		center_col_2 = 2 * m_ellipseCenterCol[0].D();
		tuple_gen_const(points_num, center_col_2, &hv_center_col_2);
		tuple_sub(point_cols_sequence, hv_center_col_2, &hv_sub);

		tuple_mult(point_cols_sequence, hv_sub, &hv_prod1);

		tuple_sub(hv_pow_diff, hv_prod1, &hv_diff_sub);
		tuple_sqrt(hv_diff_sub.Abs(), &hv_sqrt1);
		tuple_mult(hv_radius_ratio, hv_sqrt1, &hv_prod2);
		tuple_sub(hv_m_ellipseRadius2, hv_prod2, &hv_highsOffset);

		tuple_gen_const(point_rows.Num(), min_col, &hv_min_col);
		tuple_sub(point_cols, hv_min_col, &hv_select_index);
		tuple_select(hv_highsOffset, hv_select_index, &hight_offset_all);

		HTuple point_rows_calib, point_cols_calib, hv_grayval;

		point_rows_calib = point_rows + hight_offset_all;
		point_cols_calib = point_cols;
		tuple_min(point_rows_calib, &m_minRow);
		tuple_min(point_cols_calib, &m_minCol);
		tuple_max(point_rows_calib, &m_maxRow);
		tuple_max(point_cols_calib, &m_maxCol);

		gen_rectangle1(&rectangle, m_minRow + 4, m_minCol + 4, m_maxRow - 4, m_maxCol - 4);

		get_grayval(Image, point_rows, point_cols, &hv_grayval);
		gen_image_const(&Image1, "byte", m_imgWidth, m_imgHeight);
		set_grayval(Image1, point_rows_calib, point_cols_calib, hv_grayval);


		Hobject ImageReduced, ExpandedImage, ImageSub, Partitioned;
		HTuple Row, Column, Phi, Length1, Length2, Number, part_width;
		reduce_domain(Image1, rectangle, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 13);

		gen_image_const(&Image2, "byte", m_imgWidth, m_imgHeight);
		set_grayval(Image2, point_rows_calib + 2, point_cols_calib, hv_grayval);
		sub_image(ExpandedImage, Image2, &ImageSub, 3.5, 0);
		m_vInterImgs[id] = ImageSub;
		m_calibrateEllipseImage = ImageSub;
		m_vRgn[id] = rectangle;

#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::D_CalibRegionExtractHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("8021$\
					 167;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if (FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//利用测量的方式检测边缘的变形
STDMETHODIMP CCanAlgo::D_EdgeCurlMearsureAlgo(VARIANT* rgnId, VARIANT* pPartWidth, VARIANT* pGradientThresh, VARIANT* pDistanceThresh, VARIANT* pDefectsArea)
{
	int id = (int)rgnId->fltVal;
	int PartWidth = (int)pPartWidth->fltVal;
	int GradientThresh = (int)pGradientThresh->fltVal;
	float DistanceThresh = (float)pDistanceThresh->fltVal;

	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();

	Hobject rectangle, CrtImage, ImageReduced, Partitioned, newRgn, DispContour;
	HTuple Row11, Column11, Row21, Column21, Length1, Length2, CenterRow, CenterCol;
	HTuple Number, ErrorArea, ErrorRow, ErrorCol;

#ifdef NDEBUG
	try
	{
#endif
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		if (m_bPilotImg)
		{
			return S_OK;
		}

		rectangle = m_vRgn[id];
		CrtImage = m_vInterImgs[id];
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		CComVariant retValue;
		if (rectangle.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDefectsArea);
			return S_FALSE;
		}

		reduce_domain(CrtImage, rectangle, &ImageReduced);
		partition_rectangle(rectangle, &Partitioned, PartWidth, 999);
		smallest_rectangle1(Partitioned, &Row11, &Column11, &Row21, &Column21);
		Length1 = (Column21 - Column11) / 2.0;
		Length2 = (Row21 - Row11) / 2.0;
		if (Length1 <= 0 || Length2 <= 0)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDefectsArea);
			return S_FALSE;
		}
		CenterRow = (Row21 + Row11) / 2.0;
		CenterCol = (Column21 + Column11) / 2.0;
		count_obj(Partitioned, &Number);
		m_edgeRows = HTuple();
		m_edgeCols = HTuple();
		for (int i = 0; i < Number; i++)
		{
			HTuple MeasureHandle, RowEdge, ColumnEdge, Amplitude, Distance;
			RowEdge = HTuple();
			ColumnEdge = HTuple();
			gen_measure_rectangle2(CenterRow[i], CenterCol[i], HTuple(90).Rad(), Length2[i], Length1[i], m_imgWidth, m_imgHeight, "nearest_neighbor", &MeasureHandle);
			measure_pos(CrtImage, MeasureHandle, 1, GradientThresh, "positive", "first", &RowEdge, &ColumnEdge, &Amplitude, &Distance);
			close_measure(MeasureHandle);
			if (RowEdge.Num() > 0)
			{
				m_edgeRows = m_edgeRows.Concat(RowEdge[0]);
				m_edgeCols = m_edgeCols.Concat(ColumnEdge[0]);
			}
		}
		if (m_edgeRows.Num() <= 2)
		{
			retValue = -1;
			m_vRgn[id] = rectangle;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDefectsArea);
			return S_FALSE;

		}
		Hobject Contour1, SmoothedContours1, SelectObjs, RegionUnion, RegionIntersection;
		HTuple  SmoothEdgeRows, SmoothEdgeCols, DiffRows, GreaterDist, GreaterDistIndex;
		gen_region_polygon(&m_locationEdge, m_edgeRows, m_edgeCols);
		gen_contour_polygon_xld(&Contour1, m_edgeRows, m_edgeCols);

		smooth_contours_xld(Contour1, &SmoothedContours1, 5);
		concat_obj(Contour1, SmoothedContours1, &DispContour);
		m_vRgnExTest[id] = DispContour;
		get_contour_xld(SmoothedContours1, &SmoothEdgeRows, &SmoothEdgeCols);

		tuple_sub(SmoothEdgeRows, m_edgeRows, &DiffRows);
		tuple_abs(DiffRows, &DiffRows);
		tuple_greater_elem(DiffRows.Abs(), DistanceThresh, &GreaterDist);
		tuple_find(GreaterDist, 1, &GreaterDistIndex);

		if (GreaterDistIndex != -1)
		{
			select_obj(Partitioned, &SelectObjs, GreaterDistIndex + 1);
		}
		union1(SelectObjs, &RegionUnion);
		intersection(RegionUnion, m_locationEdge, &RegionIntersection);
		dilation_circle(RegionIntersection, &RegionIntersection, 2.5);
		area_center(RegionIntersection, &ErrorArea, &ErrorRow, &ErrorCol);

		m_vErrorRgn[id] = RegionIntersection;
		if (ErrorArea.Num() > 0)
		{
			retValue = ErrorArea[0].I();
		}
		else
		{
			retValue = -1;
		}

		retValue.Detach(pDefectsArea);

#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCanAlgo::D_EdgeCurlMearsureAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("8023$\
					 397;R;FP;0;9999;11$\
					 381;B$\
					 8024;LP;8;50;1;10$\
					 1304;LP;1;255;1;45$\
					 8025;FP;0.1;10;0.1;1");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if (FAILED(hr))
		return S_FALSE;
	return S_OK;
}

