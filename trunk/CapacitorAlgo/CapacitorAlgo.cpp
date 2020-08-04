#include "stdafx.h"
#include "resource.h"
#include "CapacitorAlgo_i.h"
#include "dllmain.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "CapacitorAlgo.h"

#include "../ImageProcessLib/ImageProcessLib.h"
#pragma comment(lib,"ImageProcessLib.lib")
using namespace ColorImage;

using namespace std;
#define  HAMMING (11)
#define  BOXCAR (10)
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

// CapacitorAlgo.cpp : CCapacitorAlgo  Implementation
STDMETHODIMP CCapacitorAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_ICapacitorAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CCapacitorAlgo::SetCurrentImage(LONG* pImg)
{
	//颠倒罐检测 添加模板（更新现有模板组）
	if(m_addModelState)
	{
		m_models.Append(m_addModelID);
		m_modelNum=m_modelNum+1;
		m_addModelState=false;
		m_nImgIndex=0;
	}
	Hobject* pObj = (Hobject*)pImg;
	m_crtImgMult      = *pObj;// 原始图片，可能是多通道图片（彩色图片）
	m_bDebugImg   = ImageProcessAlg.IsDebugImage(m_crtImgMult);
	m_bPilotImg   = ImageProcessAlg.IsPilotImage(m_crtImgMult);
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

	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	Hobject regionOrg;
	//显示模板区域(校正后图片对应的区域替换为原始图片对应的区域)
	Hobject* pObj = (Hobject*)pRgn;
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
	*pObj = regionOrg;
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CCapacitorAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{   
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+L"\\"+m_strTaskName;
	m_strTaskPath = CW2A(m_wstrTaskPath.c_str());
	return S_OK;
}

STDMETHODIMP CCapacitorAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	//TODO:
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+L"\\"+m_strTaskName;
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
STDMETHODIMP CCapacitorAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
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
	}
	return S_OK;
}

STDMETHODIMP CCapacitorAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{
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
}

STDMETHODIMP CCapacitorAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
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
		find_shape_model(ImageReduced, m_addModelID,  m_angleStart, m_angleExtent, 0.45, 1, 0.5, "least_squares",  (HTuple(6).Append(-2)), 0.6,  &Row_M, &Column_M, &Angle_M, &Score_M);//(HTuple(4).Append(-2)), 0.8
		if (Score_M>0.3)
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
}

STDMETHODIMP CCapacitorAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
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
}

STDMETHODIMP CCapacitorAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{

	return S_OK;
}


STDMETHODIMP CCapacitorAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
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
STDMETHODIMP CCapacitorAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
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
			vImgScore[t] = 85;
		}
	}
	return S_OK;
}

STDMETHODIMP CCapacitorAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::ReloadExternalFile(BSTR* bstrPath)
{

	return S_OK;
}
//
STDMETHODIMP CCapacitorAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
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
STDMETHODIMP CCapacitorAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CCapacitorAlgo::ModelCreate(double Radius, HTuple * ModelID1)
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
STDMETHODIMP CCapacitorAlgo::CenterLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
STDMETHODIMP CCapacitorAlgo::CenterLocationAlgo1Help(BSTR* pHelpStr)
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
STDMETHODIMP CCapacitorAlgo::CenterLocationAlgoHS(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
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
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
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
			m_locationRadius=CenterRadius;
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
				return S_FALSE;
			}
		}
		else
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle;
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
STDMETHODIMP CCapacitorAlgo::CenterLocationAlgoHSHelp(BSTR* pHelpStr)
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
STDMETHODIMP CCapacitorAlgo::CenterRelocationAlgoHS(VARIANT* rgnId,  VARIANT* pRadiusLong,  VARIANT* pRadiusShort, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
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
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, RadiusLong,RadiusShort);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		threshold(ImageReduced, &threshed,ThresholdLow, 255);
		fill_up(threshed, &RegionFillUp);
		union1(RegionFillUp, &RegionUnion);
		opening_circle(RegionUnion, &RegionOpening,OutlierRemoverSize+0.5);
		get_obj_class(RegionOpening, &hv_Class);
		if (0 != (hv_Class==HTuple("region")))
		{	
			smallest_circle(RegionOpening, &hv_Row3, &hv_Column3, &CenterRadius);
			if((CenterRadius<Radius*1.4)&(CenterRadius>Radius*0.6))
			{
				gen_circle(&Circle, hv_Row3, hv_Column3, Radius);
				m_locationRadius=CenterRadius;
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
				return S_FALSE;
			}
		}
		else
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle;
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
STDMETHODIMP CCapacitorAlgo::CenterRelocationAlgoHSHelp(BSTR* pHelpStr)
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
//提取边缘轮廓
void shift_height_sub_image (Halcon::Hobject ho_Image, Halcon::Hobject *ho_ColEdgeImage, Halcon::HTuple hv_ShiftSize, Halcon::HTuple hv_BlackOrWhite, Halcon::HTuple hv_Mult)
{

	// Local iconic variables 
	Hobject  ho_ImageShift, ho_Rectangle, ho_RectangleR;
	// Local control variables 
	HTuple  hv_Width1, hv_Height1, hv_Rows, hv_Columns;
	HTuple  hv_Grayval, hv_RowDown, hv_RowsR, hv_ColumnsR;
	HTuple  hv_GrayvalR, hv_RowUp;
	gen_empty_obj(&(*ho_ColEdgeImage));
	get_image_size(ho_Image, &hv_Width1, &hv_Height1);
	gen_image_const(&ho_ImageShift, "byte", hv_Width1, hv_Height1);

	gen_rectangle1(&ho_Rectangle, 0, 0, hv_Height1/2.0+1, hv_Width1-1);

	get_region_points(ho_Rectangle, &hv_Rows, &hv_Columns);

	get_grayval(ho_Image, hv_Rows, hv_Columns, &hv_Grayval);
	hv_RowDown = hv_Rows+(hv_ShiftSize.Round());
	set_grayval(ho_ImageShift, hv_RowDown, hv_Columns, hv_Grayval);
	//

	gen_rectangle1(&ho_RectangleR,  hv_Height1/2.0,0, hv_Height1-1, hv_Width1-1);

	get_region_points(ho_RectangleR, &hv_RowsR, &hv_ColumnsR);

	get_grayval(ho_Image, hv_RowsR, hv_ColumnsR, &hv_GrayvalR);
	hv_RowUp = hv_RowsR-(hv_ShiftSize.Round());
	set_grayval(ho_ImageShift, hv_RowUp, hv_ColumnsR, hv_GrayvalR);
	if (0 != (hv_BlackOrWhite>0))
	{
		sub_image(ho_Image, ho_ImageShift, &(*ho_ColEdgeImage), hv_Mult, 0);
	}
	else
	{
		sub_image(ho_ImageShift, ho_Image, &(*ho_ColEdgeImage), hv_Mult, 0);
	}
}
//新的定位方法:在设定区域内找检测物体中心位置。
STDMETHODIMP CCapacitorAlgo::CenterLocationHeightAlgo(VARIANT* rgnID ,VARIANT* pLeftCol,VARIANT*pRegWidth,VARIANT* pContrast)
{
	int id = (int)rgnID->fltVal; 
	int LeftCol=(int)pLeftCol->fltVal;
	int RegWidth=(int)pRegWidth->fltVal;
	int Contrast=(int)pContrast->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG        
	try
	{
#endif
		Hobject  Image = m_crtImg;
		HTuple   ImageWidth,ImageHeight;
		get_image_size(Image,&ImageWidth,&ImageHeight);	
		m_imgWidth  = (long)ImageWidth[0].I();
		m_imgHeight = (long)ImageHeight[0].I();
		//
		Hobject ImagePart2;
		Hobject RegionClosingMoved;	
		//
		m_angleSlant =0.0;	
		if (m_bPilotImg)
		{
			m_leftCol  =LeftCol;
			//m_height= LeftCol;  //用于提取标准高度
			//
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y =0.0;
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		m_nImgIndex++;
		//
		if (ImageWidth-RegWidth<=ImageWidth/2)
		{
			RegWidth=10;
		}
		//ImageWidth
		if (LeftCol>=ImageWidth-80-RegWidth)
		{
			LeftCol=ImageWidth[0].I()-220-RegWidth;
		}
		else if (LeftCol<=0)
		{
			LeftCol=10;
		}	

		HTuple rectTopY, rectTopX,rectBottomY,rectBottomX;
		rectTopY=150;
        rectTopX=LeftCol;
		rectBottomY=ImageHeight-149;
		rectBottomX= ImageWidth-30-RegWidth;
		rectBottomY=rectTopY+(((rectBottomY-rectTopY)/4).Round())*4;
		rectBottomX=rectTopX+(((rectBottomX-rectTopX)/4).Round())*4;
		//
		crop_rectangle1(Image,&ImagePart2,rectTopY, rectTopX,rectBottomY-1, rectBottomX-1);
		HTuple Width, Height;
		get_image_size (ImagePart2, &Width, &Height);
		Hobject ImageZoom2,ImageSub1,ImageMean;
		zoom_image_size(ImagePart2, &ImageZoom2,Width*0.25, Height*0.25, "nearest_neighbor");
		HTuple WidthZoom,HeightZoom;
		get_image_size(ImageZoom2, &WidthZoom, &HeightZoom);
		Hobject ImageAbsDiff,GrayImageAbsDiff,ImageR, ImageG, ImageB,ImageResult1, ImageResult2, ImageResult3;
		Hobject ImageMean1;
		mean_image (ImageZoom2, &ImageMean1, 5, 1);   
		shift_height_sub_image(ImageMean1, &ImageSub1, 2, 1, 2.0);
		//
		mean_image(ImageSub1, &ImageMean, 3, 17);
		Hobject RegionDynThresh,Rectangle,RegionIntersection,ConnectedRegions,SelectedRegions,RegionUnion,RegionDifference,Partitioned;
		dyn_threshold(ImageSub1, ImageMean, &RegionDynThresh, Contrast, "light");
		//*
		if (WidthZoom<20)
		{
			gen_rectangle1(&Rectangle, 10, 10, HeightZoom-6,14);
		}
		else
		{
			gen_rectangle1(&Rectangle, 10, 10, HeightZoom-6, WidthZoom-6);
		}
		intersection(RegionDynThresh, Rectangle, &RegionIntersection);

		connection(RegionIntersection, &ConnectedRegions);

		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", 35, 99999);

		union1(SelectedRegions, &RegionUnion);
		//调试状态下显示边缘区域
		if(m_bDebugImg)
		{
			HTuple HomMat2DIdentity,HomMat2DScale;
			hom_mat2d_identity(&HomMat2DIdentity);
			hom_mat2d_scale(HomMat2DIdentity, 4, 4, 0, 0, &HomMat2DScale);
			Hobject RegionAffineTrans;
			affine_trans_region(RegionUnion, &RegionAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(RegionAffineTrans, &RegionClosingMoved, rectTopY, rectTopX);
			//
			rectTopY=150;
			rectTopX=LeftCol;
			rectBottomY=ImageHeight-149;
			rectBottomX= ImageWidth-30-RegWidth;
			Hobject RegionMargin;
			gen_region_polygon(&RegionMargin, ((((HTuple(rectTopY).Append(rectTopY)).Append(rectBottomY)).Append(rectBottomY)).Append(rectTopY)), 
				((((HTuple(rectTopX).Append(rectBottomX)).Append(rectBottomX)).Append(rectTopX)).Append(rectTopX)));
			concat_obj (RegionMargin, RegionClosingMoved, &RegionClosingMoved);
		}
		//
		difference(Rectangle, RegionUnion, &RegionDifference);
		partition_rectangle(Rectangle, &Partitioned, 1, HeightZoom);

		Hobject RegionIntersection1,RectangleS,SelectedRegions1,RegionCenters,RegionDilation,ConnectedRegions1,SelectedRegions2,CenterRegionUnion,SelectedRegions3;
		HTuple  Row1, Column1, Row2, Column2,Area, Row, Column,RowOrg,ColumnOrg,Area3, Row3, Column3;
		intersection(Partitioned, RegionUnion, &RegionIntersection1);
		select_shape (RegionIntersection1, &SelectedRegions3, "area", "and", 2, 9999999);
		smallest_rectangle1(SelectedRegions3, &Row1, &Column1, &Row2, &Column2);
		//
		HTuple number;
		count_obj (SelectedRegions3, &number); 
		if (number<2)
		{
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y =0.0;
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}

		gen_rectangle1(&RectangleS, Row1, Column1, Row2, Column2);

		select_shape(RectangleS, &SelectedRegions1, "area", "and", 41, 9999999);
		area_center(SelectedRegions1, &Area, &Row, &Column);
		RowOrg = ((Row/0.25).Round())+rectTopY;
		ColumnOrg = ((Column/0.25).Round())+rectTopX;
		gen_region_points(&RegionCenters, RowOrg, ColumnOrg);

		dilation_rectangle1(RegionCenters, &RegionDilation, 47, 1);

		connection(RegionDilation, &ConnectedRegions1);

		select_shape_std(ConnectedRegions1, &SelectedRegions2, "max_area", 50);
		union1(SelectedRegions2,&CenterRegionUnion);
		area_center(CenterRegionUnion, &Area3, &Row3, &Column3);
		m_vPos[0].m_x = (float)(ImageWidth[0].D()/2.0);
		m_vPos[0].m_y =(float)Row3[0].D();
		m_vRgn[id] = RegionClosingMoved;
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
STDMETHODIMP CCapacitorAlgo::CenterLocationHeightAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("726$\
					 381;B$\
					 742;LP;10;1800;1;500$\
					 668;LP;10;450;1;400$\
					 390;LP;0;255;1;20");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//
STDMETHODIMP CCapacitorAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CCapacitorAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CCapacitorAlgo::CircleRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	float LocationRadius=(float)(m_locationRadius[0].I());
	float Radius =LocationRadius - RadiusSize;
	float m_RadiusExt =LocationRadius - RadiusSize-RoiWidth;

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
STDMETHODIMP CCapacitorAlgo::CircleRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 693;FP;-1500;1500;0.5;10$\
					 413;FP;1;2000;0.5;208"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//Rectangle  
STDMETHODIMP CCapacitorAlgo::RectangleRegionLocationAlgo(VARIANT* rgnId,VARIANT* pLeftColOffset,VARIANT* pRightCol,VARIANT* pRegionHeight)
{
	//得到矩形检测区域
	int id = (int)rgnId->fltVal;
	int LeftCol =m_leftCol+(int)pLeftColOffset->fltVal;
	int RightCol =(int)pRightCol->fltVal;
	int RegionHeight =(int)pRegionHeight->fltVal;
	if (LeftCol<1)
	{
		LeftCol=1;
	}
	if (RightCol<10)
	{
		RightCol=10;
	}
	if (RightCol>m_imgWidth-4)
	{
		RightCol=m_imgWidth-4;
	}
	if (LeftCol>RightCol)
	{
		LeftCol=RightCol-2;
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
		Hobject Rectangle,RectangleImage;
		get_image_size(Image, &Width, &Height);
		CenterCol = (LeftCol+RightCol)/2.0;
		gen_rectangle2 (&Rectangle, fCenterY, CenterCol, m_angleSlant, (RightCol-LeftCol)/2.0, RegionHeight/2.0);
		gen_rectangle1(&RectangleImage, 0,0, Height-1, Width-1);
		intersection(Rectangle, RectangleImage, &RegionDetect);
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
STDMETHODIMP CCapacitorAlgo::RectangleRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("708$\
					 167;B$\
					 721;LP;-600;1600;1;20$\
					 721;LP;10;1600;1;1500$\
					 667;LP;10;1200;1;600"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::RectangleFixedWidthLocationAlgo(VARIANT* rgnId,VARIANT* pLeftColOffset,VARIANT* pRegionWidth,VARIANT* pRegionHeight)
{
	//得到矩形检测区域
	int id = (int)rgnId->fltVal;
	int LeftCol =m_leftCol+(int)pLeftColOffset->fltVal;
	if (LeftCol<1)
	{
		LeftCol=1;
	}
	int RightCol =LeftCol+(int)pRegionWidth->fltVal;
	int RegionHeight =(int)pRegionHeight->fltVal;
	if (RightCol<10)
	{
		RightCol=10;
	}
	if (RightCol>m_imgWidth-4)
	{
		RightCol=m_imgWidth-4;
	}
	if (LeftCol>RightCol)
	{
		LeftCol=RightCol-2;
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
		Hobject Rectangle,RectangleImage;
		get_image_size(Image, &Width, &Height);
		CenterCol = (LeftCol+RightCol)/2.0;
		gen_rectangle2 (&Rectangle, fCenterY, CenterCol, m_angleSlant, (RightCol-LeftCol)/2.0, RegionHeight/2.0);
		gen_rectangle1(&RectangleImage, 0,0, Height-1, Width-1);
		intersection(Rectangle, RectangleImage, &RegionDetect);
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
STDMETHODIMP CCapacitorAlgo::RectangleFixedWidthLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("708$\
					 167;B$\
					 721;LP;-600;1600;1;20$\
					 668;LP;10;1600;1;1500$\
					 667;LP;10;1200;1;600"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//
STDMETHODIMP CCapacitorAlgo::CurlDetectAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::CurlDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("402$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 403;LP;5;500;1;300$\
					 404;FP;1;100;0.1;30$\
					 405;LP;1;255;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::CurlMeanGrayDetectAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pGrayValueLower, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::CurlMeanGrayDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("689$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 403;LP;5;500;1;300$\
					 191;LP;-1;255;1;50$\
					 192;LP;1;256;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	//利用带通滤波器的卷边优化算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_dictSpecialData[rId].clear();
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
{
		CComBSTR strHelp("380$\
					 397;R;LP;0;1000;1$\
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
STDMETHODIMP CCapacitorAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)


{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("609$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 610;FP;0.5;60;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//
STDMETHODIMP CCapacitorAlgo::GLineAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
{
	//盖面找线算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue         = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::GLineAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("425$\
					 397;R;LP;0;1000;1$\
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
STDMETHODIMP CCapacitorAlgo::LineDetectionAlgo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize,VARIANT* pDefectArea)
{
	int rId           = (int)rgnId->fltVal;
	float maskSize    = (float)pMaskSize->fltVal;
	double thredLow   = (double)pThredLow->fltVal;
	double thredUpper = (double)pThredUpper->fltVal;
	if(thredUpper<=thredLow)
	{
		thredUpper=thredLow+0.5;
	}
	int detectModel   = (int)pDefectType->fltVal;
	int detectSize   = (int)pDefectSize->fltVal;
	m_vErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
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
		Hobject crtImg = m_crtImg;
		Hobject imgReduced, defectRgn;
		HTuple Length, maxLength;
		reduce_domain(crtImg, rgn, &imgReduced);
		//
		Hobject Lines, UnionContours, selectedContours,rgnErosion;
		if(detectModel==1)
		{
			lines_gauss (imgReduced, &Lines, maskSize, thredLow, thredUpper, "light", "true", "bar-shaped", "true");
			union_adjacent_contours_xld (Lines, &UnionContours, 10, 1, "attr_keep");
		}
		else if(detectModel==-1)
		{
			lines_gauss (imgReduced, &Lines, maskSize, thredLow, thredUpper, "dark", "true", "bar-shaped", "true");
			union_adjacent_contours_xld (Lines, &UnionContours, 10, 1, "attr_keep");
		}
		else if(detectModel==0)
		{
			Hobject UnionContoursLight,UnionContoursDark;
			lines_gauss (imgReduced, &Lines, maskSize, thredLow, thredUpper, "light", "true", "bar-shaped", "true");
			union_adjacent_contours_xld (Lines, &UnionContoursLight, 10, 1, "attr_keep");

			lines_gauss (imgReduced, &Lines, maskSize, thredLow, thredUpper, "dark", "true", "bar-shaped", "true");
			union_adjacent_contours_xld (Lines, &UnionContoursDark, 10, 1, "attr_keep");

			concat_obj(UnionContoursLight,UnionContoursDark,&UnionContours);
		}
		erosion_circle(rgn, &rgnErosion, 2.5);
		ImageProcessAlg.LinesToRegion(UnionContours, detectSize, 99999, defectRgn);
		intersection (rgnErosion, defectRgn, &defectRgn);
		HTuple Area, Row_test, Column_test;
		area_center(defectRgn, &Area, &Row_test, &Column_test);
		//
		if(Area > 0)
		{
			retValue = Area[0].I();
			dilation_circle(defectRgn, &defectRgn, 2.5);
			m_vErrorRgn[rId] = defectRgn;
		}
		else
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::LineDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("425$\
					 397;R;FP;0;1000;4$\
					 381;B$\
					 643;FP;0.1;20;0.1;1.0$\
					 644;FP;0.1;20;0.1;3$\
					 645;FP;0.1;30;0.1;8$\
					 385;LP;-1;1;1;0$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("432$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 433;LP;1;255;1;30$\
					 434;LP;1;250;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
	Hobject RegionDilat;
	HTuple MeanGray, Deviation;//pDetectArea

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
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::DynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("411$\
					 397;R;LP;0;1000;1$\
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
STDMETHODIMP CCapacitorAlgo::DynThresholdAvoidReflectionAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::DynThresholdAvoidReflectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("616$\
					 397;R;LP;0;1000;1$\
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

STDMETHODIMP CCapacitorAlgo::HeightDiffDetectionAlgo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectSize,VARIANT* pDefectDiff)
{
	int rId           = (int)rgnId->fltVal;
	int maskSize    = (int)pMaskSize->fltVal;
	int thredLow   = (int)pThredLow->fltVal;
	int thredUpper = (int)pThredUpper->fltVal;
	if(thredUpper<=thredLow)
	{
		thredUpper=thredLow+3;
	}
	int detectSize   = (int)pDefectSize->fltVal;
	m_vErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
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
		Hobject crtImg = m_crtImg;
		Hobject imgReduced, defectRgn;
		HTuple Length, maxLength;
		reduce_domain(crtImg, rgn, &imgReduced);
		// Local iconic variables 
		Hobject  ImageMean2;
		Hobject  Edges, CroppedContours, SelectedXLD, RegionLines;
		Hobject  ObjectSelected;
		// Local control variables 
		HTuple  RowBegin, ColBegin, RowEnd, ColEnd;
		HTuple  Nr, Nc, Dist, DistCol, Index1, Sorted;
		HTuple  Indices, DistColDefine;
		HTuple  Row11, Column11, Row21, Column21;
		Hobject ExpandedImage;
		expand_domain_gray(imgReduced, &ExpandedImage, 3);
		mean_image(ExpandedImage, &ImageMean2, 2, maskSize);
		reduce_domain(ImageMean2, rgn, &ImageMean2);
		smallest_rectangle1 (rgn, &Row11, &Column11, &Row21, &Column21);
		edges_sub_pix(ImageMean2, &Edges, "canny", 1, thredLow, thredUpper);
		int fCenterY = 0;
		fCenterY = (int)m_vPos[0].m_y;
		crop_contours_xld (Edges, &CroppedContours,(int)(fCenterY-60) ,Column11-2, (int)(fCenterY+60), Column21-2, "false");

		select_shape_xld(CroppedContours, &SelectedXLD, "contlength", "and", detectSize, 99999);
		fit_line_contour_xld(SelectedXLD, "tukey", -1, 0, 5, 2, &RowBegin, &ColBegin,&RowEnd, &ColEnd, &Nr, &Nc, &Dist);
		gen_region_line(&RegionLines, RowBegin, ColBegin, RowEnd, ColEnd);
		DistCol = 100;
		gen_empty_obj(&ObjectSelected);

		HTuple end_val10 = (ColBegin.Num())-1;
		HTuple step_val10 = 1;
		for (Index1=0; Index1.Continue(end_val10, step_val10); Index1 += step_val10)
		{
			if (0 != (((Nc.Select(Index1)).Abs())<0.001))
			{
				DistCol.ReplaceElements(Index1,100);
			}
			else
			{
				DistCol.ReplaceElements(Index1,((Dist.Select(Index1))-((Nr.Select(Index1))*fCenterY))/(Nc.Select(Index1)));
			}
		}
		if (0 != ((ColBegin.Num())>=1))
		{
			tuple_sort(DistCol, &Sorted);
			tuple_sort_index(DistCol, &Indices);
			select_obj(RegionLines, &ObjectSelected, (Indices.Select(0))+1);
			DistColDefine = Sorted.Select(0);
			retValue =abs( DistColDefine[0].I()-m_leftCol);
			
			if (m_nImgIndex<60)
			{
				////自动提取标准高度
				int tmp=abs( DistColDefine[0].I()-m_leftCol);
				if (tmp<15)
				{
					m_StdHeight.push_back(DistColDefine[0].I());
				}
				double sum = std::accumulate(std::begin(m_StdHeight), std::end(m_StdHeight), 0.0);			
				if (m_StdHeight.size()==0)
				{
					m_leftCol=m_leftCol;
				}
				else
				{
					double mean =  sum/m_StdHeight.size(); //均值
					m_leftCol=(int)mean;
				}
			}
			//
			dilation_circle(defectRgn, &defectRgn, 2.5);
			m_vErrorRgn[rId] = ObjectSelected;
		}
		else
		{
			retValue = 100.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		retValue.Detach(pDefectDiff);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::HeightDiffDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("743$\
					 397;R;LP;0;300;1$\
					 381;B$\
					 733;LP;3;31;2;11$\
					 644;LP;1;200;1;10$\
					 645;LP;5;254;1;20$\
					 744;LP;1;300;1;40");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//
STDMETHODIMP CCapacitorAlgo::BubbleDetectProfileAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		Hobject ImageReduced, ImageReduceMean,ExpandedImage,ImageDetectNew;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		expand_domain_gray (ImageReduced, &ExpandedImage, 45);
		reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
		mean_image(ImageDetectNew, &ImageReduceMean, BlackMaskSize, 1);
		//
		Hobject SeriousPointDynThredImg,SeriousConnected,RegionErosion,RegionIntersection,SeriousPointReg;	
		erosion_circle (rgn, &RegionErosion, 2.5);
		dyn_threshold(ImageReduced, ImageReduceMean, &SeriousPointDynThredImg, SeriousBlackPointDynThresh, "dark");	
		intersection (SeriousPointDynThredImg, RegionErosion, &RegionIntersection);
		connection(RegionIntersection, &SeriousConnected); 
		select_shape(SeriousConnected, &SeriousPointReg, "area", "and", SeriousBlackPointSize, 99999);
		union1(SeriousPointReg, &BlackUnion);
		//
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		if(abs(WhiteMaskSize-BlackMaskSize)>=1)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
			expand_domain_gray (ImageReduced, &ExpandedImage, 45);
			reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
			mean_image(ImageDetectNew, &ImageReduceMean, WhiteMaskSize, 1);
		}
		//
		dyn_threshold(ImageReduced, ImageReduceMean, &SeriousPointDynThredImg, SeriousWhitePointDynThresh, "light");	
		intersection (SeriousPointDynThredImg, RegionErosion, &RegionIntersection);
		connection(RegionIntersection, &SeriousConnected); 
		select_shape(SeriousConnected, &SeriousPointReg, "area", "and", SeriousWhitePointSize, 99999);
		union1(SeriousPointReg, &WhiteUnion);
		//
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::BubbleDetectProfileAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("758$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 412;LP;3;101;2;45$\
					 415;LP;1;250;1;60$\
					 416;LP;1;200;1;10$\
					 417;LP;3;101;2;45$\
					 420;LP;1;250;1;20$\
					 421;LP;1;250;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//新加检测算子
STDMETHODIMP CCapacitorAlgo::GSurfaceAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
{
	//盖面垫模算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	Hobject imgRect;
	gen_rectangle1(&imgRect, 0, 0, m_imgHeight-1, m_imgWidth-1);
	intersection(imgRect, rgn, &rgn);
	int BlackThred = (int)pBlackThred->fltVal;
	int BlackArea = (int)pBlackAreaThred->fltVal;
	int WhiteThred = (int)pWhiteThred->fltVal;
	int WhiteArea = (int)pWhiteAreaThred->fltVal;
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
		Hobject ImageReduced, ExpandedImage, DerivGauss,DerivGaussInver;
		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);
		derivate_gauss(ExpandedImage, &DerivGaussInver, 5, "mean_curvature");
		invert_image (DerivGaussInver, &DerivGauss);
		reduce_domain(DerivGauss, rgn, &ImageReduced);

		Hobject Regions1, ConnectedRegions1, SelectedRegions1, BlackUnion, WhiteUnion;
		threshold(ImageReduced, &Regions1, -1,(-1)* BlackThred/1000.0);
		connection(Regions1, &ConnectedRegions1);
		select_shape(ConnectedRegions1, &SelectedRegions1, "area", "and", BlackArea, 99999);
		union1(SelectedRegions1, &BlackUnion);

		Hobject Regions2, ConnectedRegions2, SelectedRegions2;
		threshold(ImageReduced, &Regions2, WhiteThred/1000.0 ,1);
		connection(Regions2, &ConnectedRegions2);
		select_shape(ConnectedRegions2, &SelectedRegions2, "area", "and", WhiteArea, 99999);
		union1(SelectedRegions2, &WhiteUnion);
		if (m_bDebugImg)
		{
			HTuple Rows, Columns,Grayvals,Max,Min;
			get_region_points (rgn, &Rows, &Columns);        
			get_grayval (ImageReduced, Rows, Columns,& Grayvals);
			tuple_max (Grayvals, &Max) ;
			tuple_min (Grayvals, &Min);
			m_dictSpecialData[rId].push_back(float( (-1)*BlackThred) );
			m_dictSpecialData[rId].push_back(float(WhiteThred));
			m_dictSpecialData[rId].push_back(float(Min[0].D()*1000.0));
			m_dictSpecialData[rId].push_back(float(Max[0].D()*1000.0));
			HTuple Sum,Log,Prod,AbsoluteHisto, RelativeHisto, Length;
			Hobject ImageScaleMax;
			scale_image_max (ImageReduced, &ImageScaleMax);
			gray_histo (rgn, ImageScaleMax, &AbsoluteHisto, &RelativeHisto);   
			tuple_add (AbsoluteHisto,1, &Sum);
			tuple_log10 (Sum, &Log);   
			tuple_mult (Log, 100, &Prod);
			tuple_length (AbsoluteHisto, &Length);
			int histoLength = Length[0].I();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(Prod[i].D());
				if(histoAdd<=200)
					m_dictSpecialData[rId].push_back(-histoAdd);
				else
					m_dictSpecialData[rId].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;
		union2(WhiteUnion, BlackUnion, &DefectRegs);
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::GSurfaceAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("424$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 407;LP;1;999;1;100$\
					 408;LP;1;250;1;20$\
					 409;LP;1;999;1;999$\
					 410;LP;1;250;1;250");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCapacitorAlgo::SlotDetectAlgo1(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue = (float)pThresholdValue->fltVal;
	int GrayValue = (int)pGrayValue->fltVal;
	int MeanGrayValue=(int)pMeanGrayValue->fltVal;

	HTuple WidthOri, HeightOri;
	Hobject Image = m_crtImg;
	get_image_size (m_crtImg, &WidthOri, &HeightOri);
	Hobject ImageROI,JuanYuanPartitioned, ResultDefect;
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
		//int angleCirle=370;//重复一定角度，避免起始角度位置缺陷被忽略。
		//polar_trans_image_ext(Image, &PolarTransImage, fCenterY, fCenterX, 0, HTuple(angleCirle).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(angleCirle).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), "nearest_neighbor");
		change_domain(Image,rgn,&ImageROI);
		get_image_size(ImageROI, &PolarTransImageWidth, &PolarTransImageHeight);
		partition_rectangle(ImageROI, &JuanYuanPartitioned, PolarTransImageWidth, PolarTransImageHeight/NumPart);
		intensity(JuanYuanPartitioned, ImageROI, &Mean, &Deviation);
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
		//polar_trans_region_inv(ResultDefect, &BackTransRegion, fCenterY, fCenterX, 0, HTuple(angleCirle).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(angleCirle).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), WidthOri, HeightOri, "nearest_neighbor");
		union1 (ResultDefect, &DefectRegs);
		intersection(DefectRegs,rgn,&DefectRegs);
		//
		HTuple Mean1, Deviation1;
		intensity(rgn, Image, &Mean1, &Deviation1);
		if (Mean1<MeanGrayValue)
		{
			union2 (rgn,DefectRegs, &DefectRegs);
		}
		//
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::SlotDetectAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("745$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 403;LP;5;500;1;300$\
					 404;FP;1;100;0.1;30$\
					 405;LP;1;255;1;50$\
					 608;LP;1;255;1;60");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCapacitorAlgo::TabDetectProfileAlgo(VARIANT* rgnId,  VARIANT* pThredBackground,  VARIANT* pThredForeground,VARIANT* pAnglePointSize,VARIANT* pMinAngle, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int thredBackground = (int)pThredBackground->fltVal;
	int thredForeground = (int)pThredForeground->fltVal;
	int anglePointSize = (int) pAnglePointSize->fltVal;
	float minAngle = (float) pMinAngle->fltVal;
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
		// Local iconic variables 
		Hobject  RegionBlackDetect, RegionCapDetect, RegionAngleDetect,ImageRotate;
		Hobject  ImageReduced1, ImageMean, RegionBlack;
		Hobject  ConnectedRegions,RegionErosionB,RegionDilationB, SelectedRegions, Region, RegionFillUp;
		Hobject  RegionOpening, RegionClosing, RegionDifference;
		Hobject  ConnectedRegions1, RegionErosion, RegionDilation;
		Hobject  SelectedRegions2, Contours, ClippedContours1, SortedContours1;
		Hobject  ObjectSelected1, RegionEdge, RegionDilation3, RegionIntersection;
		Hobject  RegionUnion, DefectRegs;
		// Local control variables 
		HTuple  BlackNumber, Area1;
		HTuple  Row1, Column1, Indices, Area2, Row2, Column2, Row12;
		HTuple  Column12, Row22, Column22, Number, RowDatas, ColDatas;
		HTuple  Length1, RowDatasSelected1, RowDatasSelected2, RowDatasSelected3;
		HTuple  ColDatasSelected1, ColDatasSelected2, ColDatasSelected3;
		HTuple  Angle1, Prod1, Abs1, IndicesTest, SelectedRow, SelectedCol;
		HTuple  Area3, Row3, Column3;


		gen_empty_obj(&RegionBlackDetect);
		gen_empty_obj(&RegionCapDetect);
		gen_empty_obj(&RegionAngleDetect);
		//rotate_image(Image, &ImageRotate, -90, "constant");
		reduce_domain(Image, rgn, &ImageReduced1);
		//

		mean_image(ImageReduced1, &ImageMean, 2, 30);
		//1**
		threshold(ImageMean, &RegionBlack, 0, thredBackground);
		connection(RegionBlack, &ConnectedRegions);
		erosion_rectangle1(ConnectedRegions, &RegionErosionB, 2, 2);
		dilation_rectangle1(RegionErosionB, &RegionDilationB, 2, 2);
		select_shape(RegionDilationB, &SelectedRegions, "area", "and", SeriousBlackPointSize*1.5,9999999);
		count_obj(SelectedRegions, &BlackNumber);
		if (0 != (BlackNumber>2))
		{
			area_center(SelectedRegions, &Area1, &Row1, &Column1);
			tuple_sort_index(Area1, &Indices);
			select_obj(SelectedRegions, &RegionBlackDetect, HTuple(Indices[0])+1);
		}
		//2***
		threshold(ImageMean, &Region, thredForeground, 255);
		fill_up(Region, &RegionFillUp);
		opening_rectangle1(RegionFillUp, &RegionOpening, 11, 1);
		closing_rectangle1(RegionOpening, &RegionClosing, 11, 5);
		difference(RegionClosing, RegionOpening, &RegionDifference);
		connection(RegionDifference, &ConnectedRegions1);
		erosion_rectangle1(ConnectedRegions1, &RegionErosion, 3, 2);
		dilation_rectangle1(RegionErosion, &RegionDilation, 3, 2);
		select_shape(RegionDilation, &SelectedRegions2, "area", "and", SeriousBlackPointSize, 9999999);
		union1(SelectedRegions2, &RegionCapDetect);
		//3***
		area_center(RegionFillUp, &Area2, &Row2, &Column2);
		if (0 != (Area2>3000))
		{
			gen_contour_region_xld(RegionFillUp, &Contours, "border_holes");
			smallest_rectangle1(RegionFillUp, &Row12, &Column12, &Row22, &Column22);
			clip_contours_xld(Contours, &ClippedContours1, Row12+20, Column12-10, Row22-20, Column22+10);
			sort_contours_xld(ClippedContours1, &SortedContours1, "upper_left", "true", "row");
			count_obj(SortedContours1, &Number);
			if (0 != (Number>=1))
			{
				select_obj(SortedContours1, &ObjectSelected1, 1);
				get_contour_xld(ObjectSelected1, &RowDatas, &ColDatas);
				gen_region_points(&RegionEdge, RowDatas, ColDatas);
				tuple_length(ColDatas, &Length1);
				if (0 != (Length1>40))
				{
					tuple_select_range(RowDatas, 0, (Length1-(anglePointSize*2))-1, &RowDatasSelected1);
					tuple_select_range(RowDatas, anglePointSize, (Length1-anglePointSize)-1,&RowDatasSelected2);
					tuple_select_range(RowDatas, anglePointSize*2, Length1-1, &RowDatasSelected3);
					tuple_select_range(ColDatas, 0, (Length1-(anglePointSize*2))-1, &ColDatasSelected1);
					tuple_select_range(ColDatas, anglePointSize, (Length1-anglePointSize)-1,&ColDatasSelected2);
					tuple_select_range(ColDatas, anglePointSize*2, Length1-1, &ColDatasSelected3);
					angle_ll(RowDatasSelected2, ColDatasSelected2, RowDatasSelected1, ColDatasSelected1,RowDatasSelected2, ColDatasSelected2, RowDatasSelected3, ColDatasSelected3,&Angle1);
					tuple_mult(Angle1, 180.0/3.14159, &Prod1);
					tuple_abs(Prod1, &Abs1);
					tuple_find((Abs1-minAngle).Sgn(), -1, &IndicesTest);
					if (0 != (HTuple(IndicesTest[0])>-1))
					{
						tuple_select(RowDatasSelected2, IndicesTest, &SelectedRow);
						tuple_select(ColDatasSelected2, IndicesTest, &SelectedCol);
						gen_region_points(&RegionAngleDetect, SelectedRow, SelectedCol);
					}
				}
			}
		}
		else
		{
			RegionAngleDetect=RegionBlack;
		}
		//*********
		dilation_circle(RegionEdge, &RegionDilation3, 5.5);
		intersection(RegionCapDetect, RegionDilation3, &RegionIntersection);
		union2(RegionIntersection, RegionAngleDetect, &RegionUnion);

		union2(RegionUnion, RegionBlackDetect, &DefectRegs);

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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::TabDetectProfileAlgoHelp(BSTR* pHelpStr)
{
		CComBSTR strHelp("756$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 648;LP;1;220;1;20$\
					 649;LP;5;255;1;40$\
					 650;FP;3;17;1;5$\
					 651;FP;20;200;0.5;130$\
					 416;LP;1;400;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCapacitorAlgo::GapDetectAlgoHelp(BSTR* pHelpStr)
{
	// TODO: 在此添加实现代码
	CComBSTR strHelp("747$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 749;LP;1;255;1;80$\
					 750;FP;1;300;0.5;20$\
					 751;LP;1;300;1;100$\
					 752;FP;1;99999;1;150");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;

	return S_OK;
}
STDMETHODIMP CCapacitorAlgo::GapDetectAlgo(VARIANT* rgnId, VARIANT* pGrayThresh, VARIANT* pClosingCircleSize, VARIANT* pClosingRectSize, VARIANT* pDetectMinArea,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int GrayThresh = (int)pGrayThresh->fltVal;
	float ClosingCircleSize = (float)pClosingCircleSize->fltVal;
	int ClosingRectSize = (int) pClosingRectSize->fltVal;
	float DetectMinArea = (float)pDetectMinArea->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		
		// Local iconic variables 
		Hobject  ho_ImageReduced;
		Hobject  ho_Regions, ho_ConnectedRegions, ho_SelectedRegions;
		Hobject  ho_RegionFillUp, ho_RegionClosing, ho_RegionDilation;
		Hobject  ho_RegionDifference, ho_ConnectedRegions1, ho_SelectedRegions1,DefectRegs;
		// Local control variables 
		HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;
		Hobject Image = m_crtImg;
		reduce_domain(Image, rgn, &ho_ImageReduced);
		threshold(ho_ImageReduced, &ho_Regions, GrayThresh, 255);
		connection(ho_Regions, &ho_ConnectedRegions);
		select_shape_std(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
		fill_up(ho_SelectedRegions, &ho_RegionFillUp);
		closing_circle(ho_RegionFillUp, &ho_RegionClosing, ClosingCircleSize);
		closing_rectangle1(ho_RegionClosing, &ho_RegionDilation, 1, ClosingRectSize);
		difference(ho_RegionDilation, ho_RegionClosing, &ho_RegionDifference);
		connection(ho_RegionDifference, &ho_ConnectedRegions1);
		select_shape(ho_ConnectedRegions1, &DefectRegs, "area", "and", DetectMinArea, 999999);

		HTuple Area, Row_test, Column_test;
		area_center (DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 3.5);
			m_vErrorRgn[rId] = DefectRegs;	
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;




	return S_OK;
}

STDMETHODIMP CCapacitorAlgo::DynThresholdAlgoWhite(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
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
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		Hobject WhiteUnion;
		HTuple  whiteArea,rowWhite, colWhite;
		//
		ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(SeriousWhitePointDynThresh));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			sub_image (ImageReduced, ImageReduceMean, &ImageSub, 1, 0);
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
		if(whiteArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = whiteArea[0].I();
			dilation_circle (WhiteUnion, &WhiteUnion, 2.5);
			m_vErrorRgn[rId] = WhiteUnion;
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
STDMETHODIMP CCapacitorAlgo::DynThresholdAlgoWhiteHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("757$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;250;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCapacitorAlgo::CCenterLocationAlgo(VARIANT* rgnId, VARIANT* pMinScore)
{
	/*利用边缘轮廓来定位圆,提取最大轮廓*/
	int id = (int)rgnId->fltVal;
	int MinScore = (int)pMinScore->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, hv_Area1, hv_Row3, hv_Column3;
	Hobject  ho_ModelImages1, ho_ModelRegions1,ho_RegionDilation1,ho_ConnectedRegions,ho_RegionFillUp,ho_RegionErosion,ho_SelectedRegions,ho_RegionUnion;
	Hobject  Ellipse,Circle;
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
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
		
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.2, Height/1.6);   //默认图片的大小有要求
		double Pt_X, Pt_Y;
		/////
		Hobject  LocatedCircle;
		//gen_empty_obj(&LocatedCircle);
		Hobject  ImageReduced, ModelImages, ModelRegions; 
		Hobject  RegionDilation, ConnectedRegions, RegionFillUp, RegionErosion;
		Hobject  SelectedRegions, RegionUnion;
		HTuple   RegionUnion_Class, CircleArea, CenterRow, CenterCol;
		reduce_domain (Image, Ellipse, &ImageReduced);
		inspect_shape_model(ImageReduced, &ModelImages, &ModelRegions, 1, MinScore);
		dilation_circle(ModelRegions, &RegionDilation, 2.5);

		connection(RegionDilation, &ConnectedRegions);
		fill_up(ConnectedRegions, &RegionFillUp);
		erosion_circle(RegionFillUp, &RegionErosion, 2.5);
		opening_circle (RegionErosion, &RegionErosion, 70);
		select_shape_std(RegionErosion,&SelectedRegions, "max_area",60);
		//select_shape(RegionErosion, &SelectedRegions, "area", "and", AreaMin, AreaMax);//inner_radius
		union1(SelectedRegions, &LocatedCircle);
		get_obj_class(LocatedCircle, &RegionUnion_Class);
		if (RegionUnion_Class==HTuple("region"))
		{
			area_center(LocatedCircle, &CircleArea, &CenterRow, &CenterCol);

			Pt_Y = CenterRow[0].D();
			Pt_X = CenterCol[0].D();

		}
		else
		{
			HTuple Width, Height,Radius;
			get_image_size(Image, &Width, &Height);
			gen_circle(&LocatedCircle, Height/2.0, Width/2.0, Radius);
			Pt_Y = 0.5 * Height[0].D();
			Pt_X = 0.5 * Width[0].D();
			return  FALSE;
		}
		HTuple x,y;
		smallest_circle(LocatedCircle,&x,&y,&m_locationRadius);
		m_vPos[0].m_x = (float)Pt_X;
		m_vPos[0].m_y = (float)Pt_Y;
		m_vRgn[id] = LocatedCircle;
	
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
STDMETHODIMP CCapacitorAlgo::CCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("755$\
					 381;B$\
					 390;LP;1;255;1;40");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
