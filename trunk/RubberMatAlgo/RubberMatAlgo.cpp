#include "stdafx.h"
#include "resource.h"
#include "RubberMatAlgo_i.h"
#include "dllmain.h"
#include "RubberMatAlgo.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "../ImageProcessLib/ImageProcessLib.h"
using namespace EoeShell;
using namespace ColorImage;
#ifndef   DLLEXPT
#define DLLEXPT __declspec (dllexport )
#endif

using namespace std;

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

STDMETHODIMP CRubberMatAlgo::GetCurrentLocationXY(BSTR* bstrLocationXY)
{

	CString cstrXY;
	cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_vPos[0].m_x,m_vPos[0].m_y,m_modelAngle[0].D());
	CComBSTR strLocationXY = cstrXY.AllocSysString();
	HRESULT hr = strLocationXY.CopyTo(bstrLocationXY);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;

}

// CanAlgo.cpp : CCanAlgo  Implementation
STDMETHODIMP CRubberMatAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IRubberMatAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CRubberMatAlgo::SetCurrentImage(LONG* pImg)
{
	Hobject* pObj = (Hobject*)pImg;
	m_crtImg      = *pObj;
	m_crtcolorImg = *pObj;
	m_crtImgMult      = *pObj;// 原始图片，可能是多通道图片（彩色图片）
	//HTuple Width,Height;
	//get_image_size (m_crtImg, &Width, &Height);
	//ResizeImage(m_crtImg,&m_crtImg,12000,Width,Height);
	m_bDebugImg   = ImageProcessAlg.IsDebugImage(m_crtImg);	
	m_bPilotImg   = ImageProcessAlg.IsPilotImage(m_crtImg);

	HTuple Channels;
	Hobject MedianCrtImage;
	count_channels(m_crtImg,&Channels);

	//黑白图片
	if(Channels[0].I()==3)
	{
		rgb1_to_gray(m_crtImg,&m_crtImg);
	}
	else if(Channels[0].I()==1)
	{
		m_crtImg  = *pObj;
	}
	else
	{
		mean_n(m_crtImg, &m_crtImg);
	}
	if(m_bFileUpdated)
	{
		m_dictCrtPathModelID.clear();
		m_dictCrtPathModelID = m_dictNewPathModelID;
		m_dictCrtPathRgn.clear();
		m_dictCrtPathRgn = m_dictNewPathRgn;
		m_bFileUpdated=false;
	}

	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	//Hobject* pObj = (Hobject*)pRgn;
	//*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CRubberMatAlgo::ReloadExternalFile(BSTR* bstrPath)
{
	CString sFilePath = *bstrPath;
	string strFile    = CT2A(sFilePath);
	if(strFile.find(".shm") != string::npos)
	{
		m_resetModelForMultOcr=true;
		if (!_access(strFile.c_str(),0))
		{
			HTuple newModelID;
			read_shape_model(strFile.c_str(), &newModelID);
			m_dictNewPathModelID[strFile] = newModelID;
		}
		else
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("模板损坏或路径错误！"),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("Please check the validity of the model path."),_T("Reading model file failed!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			return S_FALSE;
		}
	}

	if(strFile.find(".reg") != string::npos)
	{
		if(!_access(strFile.c_str(),0))
		{
			Hobject retRgn;
			read_region(&retRgn,strFile.c_str());
			m_dictNewPathRgn[strFile] = retRgn;
		}
		else
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("区域损坏或路径设置错误。"),_T("区域读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("Please check the validity of the region path."),_T("Reading region failed!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			return S_FALSE;
		}
	}
	m_bFileUpdated = true;

	return S_OK;
}
//
STDMETHODIMP CRubberMatAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{

	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{

	return S_FALSE;
}
STDMETHODIMP CRubberMatAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}
STDMETHODIMP CRubberMatAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{

	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{

	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{

	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::ModelCreate(double Radius,int UsePolarity, HTuple * ModelID1)
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
		if(UsePolarity)
		{
			invert_image(SyntheticModelImage,&SyntheticModelImage);
		}

		create_scaled_shape_model(SyntheticModelImage, "auto", 0, 0, 0, 0.95, 1.05, "auto","auto", "use_polarity", 40, "auto", ModelID1);
		return S_OK;
	}
}
STDMETHODIMP CRubberMatAlgo::CenterLocationModelCreateAlgo(VARIANT* rgnId,VARIANT* pUsePolarity, VARIANT* pRadius, VARIANT* pMinScore)
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
		emphasize(Image,&Image,7,7,1);

		m_ImageWidth  = (long)Width;
		m_ImageHeight = (long)Height;
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/5.0, Height/5.0);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		if(m_isFirst)
		{
			ModelCreate (Radius,UsePolarity, &m_modelId);
			m_isFirst = false;
		}
		if(m_oldRadius != Radius ||m_oldPalarity!=UsePolarity )
		{
			clear_shape_model(m_modelId);
			ModelCreate (Radius,UsePolarity, &m_modelId);
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

			m_innerCircleRadius=Radius1;
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
STDMETHODIMP CRubberMatAlgo::CenterLocationModelCreateAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 949;LP;0;1;1;1$\
					 374;LP;10;2000;1;192$\
					 377;FP;0.3;1.0;0.05;0.85");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//胶垫内面二次定位
STDMETHODIMP CRubberMatAlgo::InnerEdgeLocationAlgo(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pMoveSize,VARIANT* pThresh)
{
	//
	int id = (int)rgnId->fltVal;
	float Radius =(float)pRadius->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	int MoveSize =(int)pMoveSize->fltVal;
	float Thresh =(float)pThresh->fltVal;

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	if ((fCenterX<1.0)||(fCenterY<1.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	if (Radius<15)
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject OutCircle,InnerCircle,RegionDiff,rgn;
		gen_circle(&OutCircle, fCenterY, fCenterX, Radius);
		//
		Hobject ImageReduced;
		Hobject Image=m_crtImg;
		Hlong Width, Height;
		get_image_size(Image, &Width, &Height);
		HTuple RadiusExt,RadiusInner,AngleS,WithPolar,HeightPolar;

		RadiusExt =(int) Radius;
		if (Radius-RoiWidth>3)
		{
			RadiusInner = RadiusExt-RoiWidth;
		}
		else
		{
			RadiusInner =2;
		}
		AngleS = 3.1416*2.3;
		WithPolar = (((RadiusExt+RadiusInner)*AngleS)/2.0).Round();
		HeightPolar = ((RadiusExt-RadiusInner).Abs()).Round();
		//
		Hobject SearchReg;
		gen_circle(&InnerCircle, fCenterY, fCenterX, RadiusInner);
		difference(OutCircle,InnerCircle,&SearchReg);
		m_vRgn[id] = SearchReg ;	
		//
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[id], SearchReg, &m_vRgn[id]); //用于显示debug情况
		}
		Hobject PolarTransImage1;
		polar_trans_image_ext(Image, &PolarTransImage1, fCenterY, fCenterX, 0, AngleS, RadiusInner, RadiusExt, WithPolar, HeightPolar, "nearest_neighbor");
		Hobject Domain,ImageAffinTrans,ImagePart,Domain1,ImageReduced1,ImagePart1,RegionDynThresh,RegionFillUp;
		HTuple Area1, Row2, Column2,HomMat2D;
		get_domain(PolarTransImage1, &Domain);
		area_center(Domain, &Area1, &Row2, &Column2);
		vector_angle_to_rigid(Row2, Column2, 0, Row2-MoveSize, Column2, 0, &HomMat2D);
		affine_trans_image(PolarTransImage1, &ImageAffinTrans, HomMat2D, "constant", "false");
		crop_domain(ImageAffinTrans, &ImagePart);
		get_domain(ImageAffinTrans, &Domain1);
		reduce_domain(PolarTransImage1, Domain1, &ImageReduced1);
		crop_domain(ImageReduced1, &ImagePart1);
		dyn_threshold(ImagePart, ImagePart1, &RegionDynThresh, Thresh, "dark");
		fill_up(RegionDynThresh, &RegionFillUp);

		Hobject RegionClosing3,RegionOpening1,RegionClosing2,ConnectedRegions2,SelectedRegions2,RegionMoved2;
		closing_rectangle1(RegionFillUp, &RegionClosing3, 9, 3);
		opening_rectangle1(RegionClosing3, &RegionOpening1, 9, 3);
		closing_rectangle1(RegionOpening1, &RegionClosing2, 21, 1);
		connection(RegionClosing2, &ConnectedRegions2);
		select_shape_std(ConnectedRegions2, &SelectedRegions2, "max_area", 70);
		move_region(SelectedRegions2, &RegionMoved2,MoveSize, 0);

		Hobject RegionUnion1,XYTransRegion,RegionComplement,ConnectedRegions4,SelectedRegions1;
		HTuple CenterRow, CenterCol, CenterRadius;
		union1(RegionMoved2, &RegionUnion1);
		polar_trans_region_inv(RegionUnion1, &XYTransRegion, fCenterY, fCenterX, 0, AngleS, RadiusInner, RadiusExt, WithPolar, HeightPolar, Width, Height, "nearest_neighbor");
		complement(XYTransRegion, &RegionComplement);
		connection(RegionComplement, &ConnectedRegions4);
		select_shape(ConnectedRegions4, &SelectedRegions1, "circularity","and", 0.75,1);
		//
		select_shape_std(SelectedRegions1, &SelectedRegions1, "max_area",70);
		//
		m_centerLocationRegion=SelectedRegions1;
		//
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[id], SelectedRegions1, &m_vRgn[id]); //用于显示debug情况
		}
		//
		HTuple area,row,col;
		area_center(SelectedRegions1,&area,&row,&col);
		if ((area==0)||(area.Num()==0))
		{
			m_vRgn[id] = SearchReg ;
			return S_FALSE;
		}

		smallest_circle(SelectedRegions1, &CenterRow, &CenterCol, &CenterRadius);
		///*Hobject Circle;
		//gen_circle(&Circle, CenterRow, CenterCol, CenterRadius);
		m_innerCircleRadius=CenterRadius;
		m_vPos[0].m_x = (float)CenterCol[0].D();
		m_vPos[0].m_y = (float)CenterRow[0].D();
		//union1(m_vRgn[id],&m_vRgn[id]);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		m_vRgn[id]=newRgn;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::InnerEdgeLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1035$\
					 167;B$\
					 194;FP;80.0;250;0.5;165$\
					 413;FP;1.5;100;0.5;40$\
					 1280;LP;1;20;1;5$\
					 1175;FP;1;20;0.5;3.5"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::CenterLocationUseEdgesAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,  VARIANT* pThresholdHigh, VARIANT* pMinCircularity)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	double MinCircularity = (double)pMinCircularity->fltVal;
	int ThresholdLow=(int)pThresholdLow->fltVal;
	int ThresholdHigh=(int)pThresholdHigh->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
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
		BOOL Result =H_LocateCircleUseEdges(Image, Ellipse, &Circle, Radius,ThresholdLow, ThresholdHigh, MinCircularity, &Pt_X, &Pt_Y);
		m_vPos[0].m_x = (float)Pt_X;
		m_vPos[0].m_y = (float)Pt_Y;

		HTuple CircleRow,CircleColumn,CircleRadius;
		inner_circle(Circle,&CircleRow,&CircleColumn,&CircleRadius);
		m_innerCircleRadius=CircleRadius;

		//第一次的中心点坐标
		m_FirstCenterX=(float)Pt_X;
		m_FirstCenterY=(float)Pt_Y;
		m_FirstinnerCircleRadius=CircleRadius;

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
STDMETHODIMP CRubberMatAlgo::CenterLocationUseEdgesAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("632$\
					 381;B$\
					 374;LP;10;2000;1;220$\
					 579;LP;1;220;1;20$\
					 580;LP;5;255;1;40$\
					 633;FP;0.05;1.0;0.025;0.20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	//利用带通滤波器的卷边优化算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_dictSpecialData[rId].clear();
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
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
STDMETHODIMP CRubberMatAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CRubberMatAlgo::DynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
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
		expand_domain_gray (ImageReduced, &ExpandedImage, 25);
		reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
		mean_image(ImageDetectNew, &ImageReduceMean, BlackMaskSize, BlackMaskSize);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		if(abs(WhiteMaskSize-BlackMaskSize)>=1)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
			expand_domain_gray (ImageReduced, &ExpandedImage, 25);
			reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
			mean_image(ImageDetectNew, &ImageReduceMean, WhiteMaskSize, WhiteMaskSize);
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
STDMETHODIMP CRubberMatAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CRubberMatAlgo::BubbleDetectProfileAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
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
STDMETHODIMP CRubberMatAlgo::BubbleDetectProfileAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("646$\
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

STDMETHODIMP CRubberMatAlgo::BlackDetectExcludeEdgesAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize, VARIANT* pThresholdLow,  VARIANT* pMinEdgeSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	int SeriousBlackPointDynThresh = (int)pSeriousBlackPointDynThresh->fltVal;
	int BlackThresh = (int)pBlackThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	int ThresholdLow = (int) pThresholdLow->fltVal;
	int ThresholdHigh=ThresholdLow*2;
	if (ThresholdHigh>255)
	{
		ThresholdHigh=255;
	}
	int MinEdgeSize = (int) pMinEdgeSize->fltVal;
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
		Hobject  ModelImages, ModelRegions;
		Hobject  RegionDilation, RegionDynThresh, RegionDifference;
		Hobject  RegionBlack, RegionUnion, ConnectedRegions;
		Hobject  SelectedRegions;

		reduce_domain(Image, rgn, &ImageReduced);
		if(BlackMaskSize>=0.5)
			median_image(ImageReduced, &ImageReduceMean, "circle", BlackMaskSize, "mirrored");
		else
			ImageReduceMean=ImageReduced;

		inspect_shape_model(ImageReduced, &ModelImages, &ModelRegions, 1, ((HTuple(ThresholdLow).Append(ThresholdHigh)).Append(MinEdgeSize)));
		dilation_circle(ModelRegions, &RegionDilation, 2.5);
		dyn_threshold(ImageReduced, ImageReduceMean, &RegionDynThresh, SeriousBlackPointDynThresh, "dark");
		H_SelectRegionExcludeLinear (RegionDynThresh,&RegionDynThresh, 0.5,18);
		difference(RegionDynThresh, RegionDilation, &RegionDifference);

		threshold(ModelImages, &RegionBlack, 0, BlackThresh);
		union2(RegionDifference, RegionBlack, &RegionUnion);
		connection(RegionUnion, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, 9999);
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::BlackDetectExcludeEdgesAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("635$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 412;LP;3;101;2;3$\
					 415;LP;1;250;1;40$\
					 198;LP;1;250;1;60$\
					 416;LP;1;200;1;10$\
					 579;LP;1;250;1;10$\
					 581;LP;1;800;1;100");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::BlackDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::BlackDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CRubberMatAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	union1(rgn,&rgn); //debug显示
	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
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
		m_vErrorRgn[rId] = rgn;
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
STDMETHODIMP CRubberMatAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::UVCurlAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea)
{
	//UVCurlAlgo  对应第三版UVCurlDLL2.1
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	HTuple Row6, Column6, RadiusOut,Row8, Column8, HalfRoiWidth;
	smallest_circle (rgn, &Row6, &Column6, &RadiusOut);
	inner_circle (rgn, &Row8, &Column8, &HalfRoiWidth);

	float RoiWidth =(float)(2.0*HalfRoiWidth[0].D());
	float Radius =(float)RadiusOut[0].D();
	if((Radius<RoiWidth*2.1)||(RoiWidth<1.2))
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	if((Radius<RoiWidth*2.1)||(RoiWidth<1.6))
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
	Hobject PolarTransImage, ImageScaleMax, JuanYuanPartitioned, ResultDefect;
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
		int angleCirle=370;//重复一定角度，避免起始角度位置缺陷被忽略。
		polar_trans_image_ext(Image, &PolarTransImage, fCenterY, fCenterX, 0, HTuple(angleCirle).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(angleCirle).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), "nearest_neighbor");
		get_image_size(PolarTransImage, &PolarTransImageWidth, &PolarTransImageHeight);
		partition_rectangle(PolarTransImage, &JuanYuanPartitioned, PolarTransImageWidth/NumPart, PolarTransImageHeight);
		intensity(JuanYuanPartitioned, PolarTransImage, &Mean, &Deviation);
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
		polar_trans_region_inv(ResultDefect, &BackTransRegion, fCenterY, fCenterX, 0, HTuple(angleCirle).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(angleCirle).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), WidthOri, HeightOri, "nearest_neighbor");
		union1 (BackTransRegion, &DefectRegs);
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
STDMETHODIMP CRubberMatAlgo::UVCurlAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("607$\
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

STDMETHODIMP CRubberMatAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
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
STDMETHODIMP CRubberMatAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("609$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 610;FP;0.5;180;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
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
STDMETHODIMP CRubberMatAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CRubberMatAlgo::LineDetectionAlgo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize,VARIANT* pDefectArea)
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

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
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
STDMETHODIMP CRubberMatAlgo::LineDetectionAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CRubberMatAlgo::BlackDetectUseUpSizeAlgo(VARIANT* rgnId, VARIANT* pBlackThresh,VARIANT* pThredForeground,VARIANT* pWhiteSize,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int BlackThresh = (int)pBlackThresh->fltVal;
	int ThredForeground = (int) pThredForeground->fltVal;
	int WhiteSize = (int)pWhiteSize->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	int BlackPointUpSize = (int)pBlackPointUpSize->fltVal;
	if (BlackPointUpSize<=SeriousBlackPointSize)
	{
		BlackPointUpSize=SeriousBlackPointSize+10;
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
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		//
		Hobject  ImageOpening;
		Hobject  RegionDynThresh;
		Hobject  RegionBlack, RegionUnion, ConnectedRegions,SelectedRegions;
		mean_image(Image,&Image,5,5);
		reduce_domain(Image, rgn, &ImageReduced);

		threshold(ImageReduced, &RegionBlack, 0, BlackThresh);
		connection(RegionBlack, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, BlackPointUpSize);
		union1(SelectedRegions, &RegionBlack);
		//排除拉环过低干扰
		Hobject RegionWhite,RegionFillUp,RegionErosion,RegionDilation;
		threshold (ImageReduced, &RegionWhite, ThredForeground,255)  ;   
		fill_up (RegionWhite, &RegionFillUp);
		erosion_rectangle1 (RegionFillUp, &RegionErosion, WhiteSize, 3);
		dilation_rectangle1 (RegionErosion, &RegionDilation,WhiteSize, 3);
		difference (RegionBlack, RegionDilation, &RegionBlack);
		//	
		connection(RegionBlack, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, BlackPointUpSize);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(BlackThresh));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple AbsoluteHisto, RelativeHisto, Length;
			gray_histo (rgn, Image, &AbsoluteHisto, &RelativeHisto);
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::BlackDetectUseUpSizeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("676$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 198;LP;1;250;1;60$\
					 649;LP;5;255;1;80$\
					 668;LP;5;360;1;170$\
					 416;LP;1;400;1;10$\
					 677;LP;1;8000;1;600");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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

		//if (m_bDebugImg)
		//{
		//	m_dictSpecialData[rId].push_back(float(SeriousBlackPointDynThresh));
		//	m_dictSpecialData[rId].push_back(float(SeriousWhitePointDynThresh));
		//	HTuple AbsoluteHisto, RelativeHisto, Length;
		//	gray_histo (rgn, DerivGauss1, &AbsoluteHisto, &RelativeHisto);
		//	HTuple  maxValue = 0;
		//	tuple_max (AbsoluteHisto, &maxValue);
		//	tuple_length (AbsoluteHisto, &Length);
		//	float YRange = 100;
		//	float YScaleFactor = float(YRange/maxValue[0].D());
		//	int histoLength = Length[0].I();
		//	for (int i=0; i< histoLength; i++)
		//	{
		//		m_dictSpecialData[rId].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
		//	}
		//}
		//
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("678$\
					 397;R;LP;0;1000;1$\
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

STDMETHODIMP CRubberMatAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

//STDMETHODIMP CRubberMatAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
//{
//	/*利用形状模板来定位圆*/
//	int id = (int)rgnId->fltVal;
//	double Radius = (double)pRadius->fltVal;
//	float MinScore = (float)pMinScore->fltVal;
//
//	m_threeCircleLocation=true;
//
//	Hobject newRgn;
//	m_vRgn[id] = newRgn;
//
//	double Row, Column, Radius1;
//	Hlong Width, Height;
//	HTuple CenterRow, CenterColumn, Angle, Scale, Score, CenterRadius, MovementOfObject, MoveAndScaleOfObject;
//	Hobject  ContCircle, EmptyImage, SyntheticModelImage;
//	Hobject  ShapeModelImage, ShapeModelRegion, ShapeModel, SearchImage;
//	Hobject  Ellipse, ImageReduced, ModelAtNewPosition, Region, Circle1;
//#ifdef NDEBUG
//	try
//	{
//#endif
//		//
//		if(m_bDebugImg)
//		{
//			m_dictSpecialData.clear();
//		}
//		//
//		Hobject Image = m_crtImg;
//		get_image_size (Image, &Width, &Height);
//		m_imgWidth  = (long)Width;
//		m_imgHeight = (long)Height;
//		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/5.0, Height/5.0);   //默认图片的大小有要求
//		reduce_domain (Image, Ellipse, &ImageReduced);
//		if(m_isFirst)
//		{
//			ModelCreate (Radius, &m_modelId);
//			m_isFirst = false;
//		}
//		if(m_oldRadius != Radius)
//		{
//			clear_shape_model(m_modelId);
//			ModelCreate (Radius, &m_modelId);
//			m_oldRadius = Radius;
//		}
//		get_shape_model_contours (&ShapeModel, m_modelId, 1);
//		find_scaled_shape_model (ImageReduced, m_modelId, 0, 0, 0.95, 1.05, MinScore, 1, 0.5, "least_squares", (HTuple(5).Append(-2)), 0.7, &CenterRow, &CenterColumn, &Angle, &Scale, &Score);
//		if (0 != ((Score.Num())>0))
//		{
//			vector_angle_to_rigid(0, 0, 0, CenterRow[0].D(), CenterColumn[0].D(), Angle[0].D(), &MovementOfObject);
//			hom_mat2d_scale(MovementOfObject, Scale[0].D(), Scale[0].D(),CenterRow[0].D(), CenterColumn[0].D(), &MoveAndScaleOfObject);
//			affine_trans_contour_xld(ShapeModel, &ModelAtNewPosition, MoveAndScaleOfObject);
//			gen_region_contour_xld(ModelAtNewPosition, &Region, "filled");
//			smallest_circle(Region, &Row, &Column, &Radius1);
//			m_innerCircleRadius=Radius1;
//			Hobject Circle, RegionDifference;
//			gen_circle(&Circle, Row, Column, Radius1);
//			gen_circle(&Circle1, Row, Column, Radius);
//			m_vPos[0].m_y = (float)Row;
//			m_vPos[0].m_x = (float)Column;
//			if(Circle.Id() != H_EMPTY_REGION) 
//			{
//				if(Radius1 >= Radius)
//				{
//					if(Radius1-Radius < 1)
//					{
//						gen_circle(&Circle, Row, Column, Radius1+1);
//					}
//					else
//					{
//						gen_circle(&Circle, Row, Column, Radius1);
//					}
//					difference (Circle, Circle1, &RegionDifference);
//				}
//				else
//				{
//					if(Radius - Radius1 < 1)
//					{
//						gen_circle(&Circle, Row, Column, Radius1-1);
//					}
//					else
//					{
//						gen_circle(&Circle, Row, Column, Radius1);
//					}
//					difference (Circle1, Circle, &RegionDifference);
//				}
//				m_vRgn[id] = RegionDifference;
//
//			}
//		}
//		else
//		{
//			gen_circle(&Circle1, Height/2.0, Width/2.0, Radius);
//			m_vPos[0].m_x = 0.0;
//			m_vPos[0].m_y = 0.0;
//			m_vRgn[id] = Circle1;
//			return S_FALSE;
//		}
//#ifdef NDEBUG
//	}
//
//	catch (HException &except) 
//	{
//		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
//		m_vRgn[id] = newRgn ;
//		return S_FALSE;
//	}
//#endif
//	return S_OK;
//}
//STDMETHODIMP CRubberMatAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
//{
//	CComBSTR strHelp("372$\
//					 381;B$\
//					 374;LP;10;2000;1;130$\
//					 377;FP;0.5;1.0;0.05;0.85");
//	HRESULT hr = strHelp.CopyTo(pHelpStr);
//	if(FAILED(hr))
//		return S_FALSE;
//	return S_OK;
//}

STDMETHODIMP CRubberMatAlgo::CenterRelocationAlgoHS(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong  Width, Height;
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
		//gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, HTuple(Height/5*3),HTuple(Height/2));   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		threshold(ImageReduced, &threshed,ThresholdLow, 255);
		fill_up(threshed, &RegionFillUp);
		union1(RegionFillUp, &RegionUnion);
		opening_circle(RegionUnion, &RegionOpening,OutlierRemoverSize+0.5);
		m_outRegion=RegionOpening;
		get_obj_class(RegionOpening, &hv_Class);
		if (0 != (hv_Class==HTuple("region")))
		{	
			smallest_circle(RegionOpening, &hv_Row3, &hv_Column3, &CenterRadius);
			if((CenterRadius<Radius*1.4)&(CenterRadius>Radius*0.6))
			{
				gen_circle(&Circle, hv_Row3, hv_Column3, Radius);
				m_innerCircleRadius=Radius;
				m_vPos[0].m_y = (float)hv_Row3[0].D();
				m_vPos[0].m_x = (float)hv_Column3[0].D();
				m_vRgn[id] = Circle;

				//第一次的中心点坐标
				m_FirstCenterX=(float)hv_Column3[0].D();
				m_FirstCenterY=(float)hv_Row3[0].D();
				m_FirstinnerCircleRadius=Radius;
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
STDMETHODIMP CRubberMatAlgo::CenterRelocationAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("685$\
					 381;B$\
					 374;LP;10;2000;1;180$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	float LocationRadius=(float)(m_innerCircleRadius[0].I());
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
STDMETHODIMP CRubberMatAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
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

void shift_sub_image (Halcon::Hobject Image, Halcon::Hobject *ColEdgeImage, Halcon::HTuple ShiftSize, Halcon::HTuple BlackOrWhite, Halcon::HTuple Mult)
{

	Hobject  ImageShift, RectangleL, RectangleR;
	HTuple  Width1, Height1, Rows, Columns;
	HTuple  GrayvalL, ColumnsRight,RowsL,ColumnsL;
	HTuple  GrayvalR, ColumnsLeft, RowsR, ColumnsR;
	gen_empty_obj(&(*ColEdgeImage));
	get_image_size(Image, &Width1, &Height1);
	gen_image_const(&ImageShift, "byte", Width1, Height1);

	//
	gen_rectangle1(&RectangleL, 0, 0, Height1-1, (Width1/2.0)+1);
	get_region_points(RectangleL, &RowsL, &ColumnsL);
	get_grayval(Image, RowsL, ColumnsL, &GrayvalL);
	ColumnsRight = ColumnsL+(ShiftSize.Round());
	set_grayval(ImageShift, RowsL, ColumnsRight, GrayvalL);

	//
	gen_rectangle1(&RectangleR, 0, Width1/2.0, Height1-1, Width1-1);
	get_region_points(RectangleR, &RowsR, &ColumnsR);
	get_grayval(Image, RowsR, ColumnsR, &GrayvalR);
	ColumnsLeft = ColumnsR-(ShiftSize.Round());
	set_grayval(ImageShift, RowsR, ColumnsLeft, GrayvalR);
	if (0 != (BlackOrWhite>0))
	{
		sub_image(Image, ImageShift, &(*ColEdgeImage), Mult, 0);
	}
	else
	{
		sub_image(ImageShift, Image, &(*ColEdgeImage), Mult, 0);
	}
}
STDMETHODIMP CRubberMatAlgo::SideCenterLocationAlgo(VARIANT* rgnID ,VARIANT*pRowStart,VARIANT*pColStart,VARIANT*pRegWidth,VARIANT*pRegHeight,VARIANT* pContrast)
{
	int id = (int)rgnID->fltVal;

	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;

	int Contrast=(int)pContrast->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG        
	try
	{
#endif
		Hobject  Image = m_crtImg;
		get_image_size(Image,&m_ImageWidth,&m_ImageHeight);	

		//首先判断搜索区域是否有其它胶垫。
		//	
		if (RowStart>m_ImageHeight/2)
		{
			RowStart=10;
		}
		if (ColStart>m_ImageWidth/2)
		{
			ColStart=10;
		}
		if ((RegWidth+ColStart)>m_ImageWidth-1)
		{
			RegWidth=10;
		}
		if ((RegHeight+RowStart)>m_ImageHeight-1)
		{
			RegHeight=10;
		}
		Hobject SearchRgn;
		gen_rectangle2(&SearchRgn,RowStart+RegHeight/2,ColStart+RegWidth/2,0,RegWidth/2,RegHeight/2);
		HTuple SearchRow11,SearchCol11,SearchRow22,SearchCol22;
		smallest_rectangle1(SearchRgn,&SearchRow11,&SearchCol11,&SearchRow22,&SearchCol22);

		Hobject RectLeft,RectRight;
		gen_rectangle1(&RectLeft,SearchRow11,SearchCol11,SearchRow22,SearchCol11+2);
		gen_rectangle1(&RectRight,SearchRow11,SearchCol22-2,SearchRow22,SearchCol22);

		/*Hobject SearchRgn;
		gen_rectangle1(&SearchRgn,5, 5, 400, RightCol);
		Hobject RectLeft,RectRight;
		gen_rectangle1(&RectLeft, 100, LeftCol, 400,LeftCol+2);
		gen_rectangle1(&RectRight, 100, RightCol-2, 400,RightCol);*/

		HTuple  RectLeftRows,RectLeftCols,RectLeftGrayVal,LeftGreater,LeftIndices;
		get_region_points(RectLeft, &RectLeftRows, &RectLeftCols);
		get_grayval(Image, RectLeftRows, RectLeftCols, &RectLeftGrayVal);
		tuple_greater_elem(RectLeftGrayVal, 30, &LeftGreater);
		tuple_find(LeftGreater, 1, &LeftIndices);

		HTuple  RectRightRows,RectRightCols,RectRightGrayVal,RightGreater,RightIndices;
		get_region_points(RectRight, &RectRightRows, &RectRightCols);
		get_grayval(Image, RectRightRows, RectRightCols, &RectRightGrayVal);
		tuple_greater_elem(RectRightGrayVal, 30, &RightGreater);
		tuple_find(RightGreater, 1, &RightIndices);

		Hobject ImagePart;
		//gen_rectangle1(&SearchRgn,100, LeftCol, 400, RightCol);

		crop_rectangle1(Image,&ImagePart,SearchRow11,SearchCol11,SearchRow22,SearchCol22);

		if (0 != (HTuple((LeftIndices.Num())<50).And((RightIndices.Num())<50)))
		{
			//
			HTuple   ImagePartWidth,ImagePartHeight;
			get_image_size(ImagePart,&ImagePartWidth,&ImagePartHeight);
			m_rectTopY=SearchRow11;
			m_rectTopX=SearchCol11;

			Hobject ImageZoom,ImageSub1,ImageMean;
			HTuple  ImageZoomWidth,ImageZoomHeight;
			zoom_image_size(ImagePart, &ImageZoom,ImagePartWidth*0.25,ImagePartHeight*0.25,"nearest_neighbor");
			get_image_size(ImageZoom,&ImageZoomWidth,&ImageZoomHeight);

			shift_sub_image(ImageZoom, &ImageSub1, 2, 1, 1);
			//
			mean_image(ImageSub1, &ImageMean, 20, 3);
			Hobject RegionDynThresh,Rectangle,RegionIntersection,ConnectedRegions,SelectedRegions,RegionUnion,RegionDifference,Partitioned;
			dyn_threshold(ImageSub1, ImageMean, &RegionDynThresh, Contrast, "light");
			//*
			gen_rectangle1(&Rectangle, 0, 0, ImageZoomHeight-1, ImageZoomWidth);

			intersection(RegionDynThresh, Rectangle, &RegionIntersection);

			connection(RegionIntersection, &ConnectedRegions);

			select_shape(ConnectedRegions, &SelectedRegions, "area", "and",5,99999);
			select_shape(SelectedRegions,&SelectedRegions,"height","and",5,99999);
			union1(SelectedRegions, &RegionUnion);

			Hobject RegionMoved;
			if(m_bDebugImg)
			{
				HTuple HomMat2DIdentity,HomMat2DScale;
				hom_mat2d_identity(&HomMat2DIdentity);
				hom_mat2d_scale(HomMat2DIdentity, 4, 4, 0, 0, &HomMat2DScale);
				Hobject RegionAffineTrans;
				affine_trans_region(RegionUnion, &RegionAffineTrans, HomMat2DScale, "nearest_neighbor");
				move_region(RegionAffineTrans, &RegionMoved, m_rectTopY, m_rectTopX);
			}
			//
			difference(Rectangle, RegionUnion, &RegionDifference);
			partition_rectangle(Rectangle, &Partitioned, ImageZoomWidth, 1);

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

			select_shape(RectangleS, &SelectedRegions1, "area", "and", 70, 99999);
			area_center(SelectedRegions1, &Area, &Row, &Column);
			RowOrg = ((Row/0.25).Round())+m_rectTopY;
			ColumnOrg = ((Column/0.25).Round())+m_rectTopX;
			gen_region_points(&RegionCenters, RowOrg, ColumnOrg);

			dilation_rectangle1(RegionCenters, &RegionDilation, 1, 47);

			connection(RegionDilation, &ConnectedRegions1);

			select_shape_std(ConnectedRegions1, &SelectedRegions2, "max_area", 50);
			union1(SelectedRegions2,&CenterRegionUnion);
			area_center(CenterRegionUnion, &Area3, &Row3, &Column3);

			if (0 != (m_picNumber<100))
			{
				m_centerColAll = m_centerColAll.Concat(Column3);
				tuple_mean(m_centerColAll, &m_centerCol);

				m_picNumber=m_picNumber+1;
			}

			//upper edge
			HTuple CenterRow,CenterColumn,Length1,Length2,Dist;
			HTuple Interpolation,MeasureHandle,MeasureHandle2,MeasureHandle3,MeasureHandle4,MeasureHandle5;
			Interpolation="nearest_neighbor";
			CenterRow=ImagePartWidth*0.5;
			CenterColumn=ImagePartHeight*0.5;
			Length1=ImagePartWidth*0.5;
			Length2=ImagePartHeight*0.5;

			Dist=(ImagePartWidth*1.0)/20;

			gen_measure_rectangle2(CenterRow, CenterColumn, HTuple(-90).Rad(), Length1, Length2,ImagePartWidth, ImagePartHeight, Interpolation, &MeasureHandle);
			gen_measure_rectangle2(CenterRow, CenterColumn+Dist,  HTuple(-90).Rad(), Length1,Length2, ImagePartWidth, ImagePartHeight, Interpolation, &MeasureHandle2);
			gen_measure_rectangle2(CenterRow, CenterColumn+(Dist*2),  HTuple(-90).Rad(), Length1,Length2, ImagePartWidth, ImagePartHeight, Interpolation, &MeasureHandle3);
			gen_measure_rectangle2(CenterRow, CenterColumn-Dist,  HTuple(-90).Rad(), Length1,Length2, ImagePartWidth, ImagePartHeight, Interpolation, &MeasureHandle4);
			gen_measure_rectangle2(CenterRow, CenterColumn-(Dist*2),  HTuple(-90).Rad(), Length1,Length2, ImagePartWidth, ImagePartHeight, Interpolation, &MeasureHandle5);

			HTuple Sigma,ThresholdValue,Transition,SelectModel;
			HTuple Row11,Row21,Row31,Row41,Row51;
			HTuple Column11,Column21,Column31,Column41,Column51;
			HTuple Amplitude1,Amplitude2,Amplitude3,Amplitude4,Amplitude5;
			HTuple Distance1,Distance2,Distance3,Distance4,Distance5;

			Sigma=0.8;
			ThresholdValue=30;
			Transition="positive";
			SelectModel="first";

			measure_pos(ImagePart, MeasureHandle, Sigma, ThresholdValue, Transition,SelectModel, &Row11, &Column11, &Amplitude1, &Distance1);
			measure_pos(ImagePart, MeasureHandle2, Sigma, ThresholdValue, Transition,SelectModel, &Row21, &Column21, &Amplitude2, &Distance2);
			measure_pos(ImagePart, MeasureHandle3, Sigma, ThresholdValue, Transition,SelectModel, &Row31, &Column31, &Amplitude3, &Distance3);
			measure_pos(ImagePart, MeasureHandle4, Sigma, ThresholdValue, Transition,SelectModel, &Row41, &Column41, &Amplitude4, &Distance4);
			measure_pos(ImagePart, MeasureHandle5, Sigma, ThresholdValue, Transition,SelectModel, &Row51, &Column51, &Amplitude5, &Distance5);

			HTuple  IntraDistances,IntraDistancesSorted,Selected,IntraDistancesMean;

			IntraDistances.Reset();
			IntraDistances.Append(Row11);
			IntraDistances.Append(Row21);
			IntraDistances.Append(Row31);
			IntraDistances.Append(Row41);
			IntraDistances.Append(Row51);

			//释放内存
			close_measure(MeasureHandle);
			close_measure(MeasureHandle2);
			close_measure(MeasureHandle3);
			close_measure(MeasureHandle4);
			close_measure(MeasureHandle5);

			if (0 != (HTuple((IntraDistances.Num())!=5).And((IntraDistances.Num())>0)))
			{
				tuple_max(IntraDistances, &IntraDistancesMean);
				IntraDistancesMean=IntraDistancesMean+m_rectTopY;
			}
			if (0 != ((IntraDistances.Num())==5))
			{
				tuple_sort(IntraDistances, &IntraDistancesSorted);
				tuple_select_range(IntraDistancesSorted, 3, 4, &Selected);
				tuple_mean(Selected, &IntraDistancesMean);
				IntraDistancesMean=IntraDistancesMean+m_rectTopY;

				if (0 != (m_picNumber1<100))
				{
					m_upEdgeRowAll = m_upEdgeRowAll.Concat(IntraDistancesMean);
					m_UpperEdgeRow = m_upEdgeRowAll.Mean();

					m_picNumber1=m_picNumber1+1;
				}
			}
			if(0!=(IntraDistances.Num())==0)
			{
				IntraDistancesMean=m_UpperEdgeRow;
			}

			m_vPos[0].m_x = (float)Column3[0].D();	
			m_vPos[0].m_y = (float)IntraDistancesMean[0].D();
			//
			if(m_bDebugImg)
			{
				concat_obj (m_vRgn[id], SearchRgn, &m_vRgn[id]);
				concat_obj (m_vRgn[id], RegionMoved, &m_vRgn[id]);
			}
		}
		else
		{
			m_vPos[0].m_x = (float)m_centerCol[0].D();
			m_vPos[0].m_y = (float)m_UpperEdgeRow[0].D();
			m_vRgn[id] = SearchRgn ;
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
STDMETHODIMP CRubberMatAlgo::SideCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("726$\
					 381;B$\
					 939;LP;10;100;1;15$\
					 938;LP;10;200;1;45$\
					 940;LP;1;890;1;800$\
					 941;LP;1;245;1;230$\
					 192;LP;0;255;1;30");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//New
void  CRubberMatAlgo::ExtractEdgesPoints(Hobject Image, HTuple Elements, HTuple DetectHeight, HTuple DetectWidth, HTuple Sigma, HTuple Threshold, HTuple Transition, HTuple Select, 
	HTuple Row1, HTuple Column1,HTuple Row2, HTuple Column2, HTuple *ResultRow, HTuple *ResultColumn,Hobject *Regions)
{
	gen_empty_obj(&(*Regions));
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
		Hobject Rectangle;
		HTuple Distance0;
		if (0 != (Elements==1))
		{
			distance_pp(Row1, Column1, Row2, Column2, &Distance0);
			gen_rectangle2_contour_xld(&Rectangle, RowC, ColC, Deg.Rad(), DetectHeight/2, Distance0/2);
		}
		else
		{
			gen_rectangle2_contour_xld(&Rectangle, RowC, ColC, Deg.Rad(), DetectHeight/2, DetectWidth/2);
		}

		concat_obj((*Regions), Rectangle, &(*Regions));
		//HTuple RowL2,RowL1,ColL2,ColL1;
		//Hobject Arrow1;
		//if (0 != (i==1))
		//{
		//	RowL2 = RowC+((DetectHeight/2)*((-ATan).Sin()));
		//	RowL1 = RowC-((DetectHeight/2)*((-ATan).Sin()));
		//	ColL2 = ColC+((DetectHeight/2)*((-ATan).Cos()));
		//	ColL1 = ColC-((DetectHeight/2)*((-ATan).Cos()));
		//	//gen_arrow_contour_xld(&Arrow1, RowL1, ColL1, RowL2, ColL2, 25, 25);
		//	//concat_obj((*Regions), Arrow1, &(*Regions));

		//	
		//}
		//
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
	gen_region_contour_xld((*Regions), &(*Regions), "margin");
}
void  CRubberMatAlgo::FitLineUseEdgesPoints(HTuple Rows,HTuple Cols,HTuple ActiveNum,HTuple *Row1,HTuple *Col1,HTuple *Row2,HTuple *Col2)
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
STDMETHODIMP CRubberMatAlgo::SideCenterLocationAlgo1(VARIANT* rgnID ,VARIANT*pRowStart,VARIANT*pColStart,VARIANT*pRegWidth,VARIANT*pRegHeight,VARIANT* pThredValueSide,VARIANT* pThredValueUp)
{
	int id = (int)rgnID->fltVal;

	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;

	int ThredValueSide=(int)pThredValueSide->fltVal;
	int ThredValueUp=(int)pThredValueUp->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG        
	try
	{
#endif
		Hobject  Image = m_crtImg;
		get_image_size(Image,&m_ImageWidth,&m_ImageHeight);	
		//	
		if (RowStart>m_ImageHeight/2)
		{
			RowStart=10;
		}
		if (ColStart>m_ImageWidth/2)
		{
			ColStart=10;
		}
		if ((RegWidth+ColStart)>m_ImageWidth-1)
		{
			RegWidth=10;
		}
		if ((RegHeight+RowStart)>m_ImageHeight-1)
		{
			RegHeight=10;
		}
		Hobject SearchRgn;
		gen_rectangle2(&SearchRgn,RowStart+RegHeight/2,ColStart+RegWidth/2,0,RegWidth/2,RegHeight/2);
		HTuple RectTopY, RectTopX, RectButtomY, RectButtomX;
		smallest_rectangle1(SearchRgn, &RectTopY, &RectTopX, &RectButtomY, &RectButtomX);

		//
		HTuple MiddleRow;
		MiddleRow = ((RectButtomY-RectTopY)/2)+RectTopY;
		HTuple RectArea, RectRow, RectCol;
		Hobject Rect;
		gen_rectangle1(&Rect, RectTopY, RectTopX, RectTopY+10, RectButtomX);
		area_center(Rect, &RectArea, &RectRow, &RectCol);
		Hobject RectMoved;
		move_region(Rect, &RectMoved, MiddleRow-RectRow, 0);

		Hobject ImageRect,ImagePart1,ImagePart2,ImageRectMoved,RegionDynThresh;
		reduce_domain(Image, Rect, &ImageRect);
		crop_domain(ImageRect, &ImagePart1);
		reduce_domain(Image, RectMoved, &ImageRectMoved);
		crop_domain(ImageRectMoved, &ImagePart2);
		dyn_threshold(ImagePart2, ImagePart1, &RegionDynThresh, ThredValueSide, "light");

		HTuple HomMat2DIdentity;
		Hobject RegionAffineTrans,RegionMoved,RegionConnected,CenterRegion;
		hom_mat2d_identity(&HomMat2DIdentity);
		affine_trans_region(RegionDynThresh, &RegionAffineTrans, HomMat2DIdentity, "nearest_neighbor");
		move_region(RegionAffineTrans, &RegionMoved, RectTopY+(MiddleRow-RectRow), RectTopX);
		connection(RegionMoved, &RegionConnected);
		select_shape_std(RegionConnected, &CenterRegion, "max_area", 70);

		HTuple Row, Column, Phi, Length1, Length2,CenterArea, CenterRow, CenterCol;
		smallest_rectangle2(CenterRegion, &Row, &Column, &Phi, &Length1, &Length2);
		area_center(CenterRegion, &CenterArea, &CenterRow, &CenterCol);

		HTuple ShortLength,LongLength,ColOrg;
		ShortLength = HTuple();
		LongLength = HTuple();
		if (0 != (Length1>=Length2))
		{
			ShortLength = Length2;
			LongLength = Length1;
		}
		else
		{
			ShortLength = Length1;
			LongLength = Length2;
		}

		if (((LongLength*2)<490)&&(LongLength>0))
		{
			ColOrg = CenterCol;

		}
		else
		{
			HTuple ImageArea, ImageRow, ImageCol;
			Hobject Domain;
			get_domain(Image, &Domain);
			area_center(Domain, &ImageArea, &ImageRow, &ImageCol);
			//如果搜索区域内有胶垫相连接的话，就用图像的中心点
			ColOrg = ImageCol;
		}

		m_vPos[0].m_x = (float)ColOrg[0].D();	

		//上边缘定位
		Hobject SearchRegLine;
		gen_region_line(&SearchRegLine, 90, ColOrg-160, 90, ColOrg+160);
		HTuple StartRow,StartCol,EndRow,EndCol;
		StartRow = 90;
		StartCol = ColOrg-160;
		EndRow = 90;
		EndCol = ColOrg+160;


		HTuple ResultRow, ResultColumn;
		Hobject Regions;
		ExtractEdgesPoints(Image, 10, 160, 10, 1, ThredValueUp, "positive", "first", EndRow, EndCol, StartRow, StartCol, &ResultRow, &ResultColumn,&Regions);

		HTuple RowBegin, ColBegin, RowEnd, ColEnd;
		FitLineUseEdgesPoints(ResultRow, ResultColumn, 5, &RowBegin, &ColBegin, &RowEnd, &ColEnd);

		Hobject BaseRegionLine;
		gen_region_line(&BaseRegionLine, RowBegin, ColBegin, RowEnd, ColEnd);
		HTuple RowOrg;
		RowOrg = (RowBegin+RowEnd)/2;

		m_vPos[0].m_y = (float)RowOrg[0].D();

		//
		Hobject Cross,CrossReg;
		gen_cross_contour_xld(&Cross, ResultRow, ResultColumn, 6, 0.785398);
		gen_region_contour_xld(Cross,&CrossReg,"margin");

		if(m_bDebugImg)
		{
			concat_obj (m_vRgn[id], SearchRgn, &m_vRgn[id]);
			concat_obj (m_vRgn[id], CenterRegion, &m_vRgn[id]);
			concat_obj (m_vRgn[id], Regions, &m_vRgn[id]);
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
STDMETHODIMP CRubberMatAlgo::SideCenterLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("726$\
					 381;B$\
					 939;LP;10;100;1;25$\
					 938;LP;10;200;1;60$\
					 940;LP;1;799;1;700$\
					 941;LP;1;245;1;210$\
					 1057;LP;0;250;1;30$\
					 1058;LP;1;250;1;20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::SideCenterLocationAlgo11(VARIANT* rgnID ,VARIANT*pRowStart,VARIANT*pColStart,VARIANT*pRegWidth,VARIANT*pRegHeight,VARIANT* pGrayValue,VARIANT* pOpeningSizeH,VARIANT*pOpeningSizeV,VARIANT* pThredValueSide,VARIANT* pThredValueUp)
{
	int id = (int)rgnID->fltVal;

	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;

	int GrayValue=(int)pGrayValue->fltVal;
	int OpeningSizeH=(int)pOpeningSizeH->fltVal;
	int OpeningSizeV=(int)pOpeningSizeV->fltVal;

	int ThredValueSide=(int)pThredValueSide->fltVal;
	int ThredValueUp=(int)pThredValueUp->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG        
	try
	{
#endif
		Hobject  Image = m_crtImg;
		get_image_size(Image,&m_ImageWidth,&m_ImageHeight);	
		//	
		if (RowStart>m_ImageHeight/2)
		{
			RowStart=10;
		}
		if (ColStart>m_ImageWidth/2)
		{
			ColStart=10;
		}
		if ((RegWidth+ColStart)>m_ImageWidth-1)
		{
			RegWidth=10;
		}
		if ((RegHeight+RowStart)>m_ImageHeight-1)
		{
			RegHeight=10;
		}
		Hobject SearchRgn;
		gen_rectangle2(&SearchRgn,RowStart+RegHeight/2,ColStart+RegWidth/2,0,RegWidth/2,RegHeight/2);
		HTuple RectTopY, RectTopX, RectButtomY, RectButtomX;
		smallest_rectangle1(SearchRgn, &RectTopY, &RectTopX, &RectButtomY, &RectButtomX);
		//提取整体轮廓
		Hobject ImageReduced,Region,RegionFillUp,RegionOpening,RegionConnection,RegionSelected;
		reduce_domain(Image,SearchRgn,&ImageReduced);
		threshold (ImageReduced, &Region, GrayValue, 255);
		fill_up(Region, &RegionFillUp);
		opening_rectangle1(RegionFillUp, &RegionOpening, OpeningSizeH, OpeningSizeV);
		connection(RegionOpening,&RegionConnection);
		select_shape_std(RegionConnection,&RegionSelected,"max_area",70);
		m_centerLocationRegion=RegionSelected;

		//
		HTuple MiddleRow;
		MiddleRow = ((RectButtomY-RectTopY)/2)+RectTopY;
		HTuple RectArea, RectRow, RectCol;
		Hobject Rect;
		gen_rectangle1(&Rect, RectTopY, RectTopX, RectTopY+10, RectButtomX);
		area_center(Rect, &RectArea, &RectRow, &RectCol);
		Hobject RectMoved;
		move_region(Rect, &RectMoved, MiddleRow-RectRow, 0);

		Hobject ImageRect,ImagePart1,ImagePart2,ImageRectMoved,RegionDynThresh;
		reduce_domain(Image, Rect, &ImageRect);
		crop_domain(ImageRect, &ImagePart1);
		reduce_domain(Image, RectMoved, &ImageRectMoved);
		crop_domain(ImageRectMoved, &ImagePart2);
		dyn_threshold(ImagePart2, ImagePart1, &RegionDynThresh, ThredValueSide, "light");

		HTuple HomMat2DIdentity;
		Hobject RegionAffineTrans,RegionMoved,RegionConnected,CenterRegion;
		hom_mat2d_identity(&HomMat2DIdentity);
		affine_trans_region(RegionDynThresh, &RegionAffineTrans, HomMat2DIdentity, "nearest_neighbor");
		move_region(RegionAffineTrans, &RegionMoved, RectTopY+(MiddleRow-RectRow), RectTopX);
		connection(RegionMoved, &RegionConnected);
		select_shape_std(RegionConnected, &CenterRegion, "max_area", 70);

		HTuple Row, Column, Phi, Length1, Length2,CenterArea, CenterRow, CenterCol;
		smallest_rectangle2(CenterRegion, &Row, &Column, &Phi, &Length1, &Length2);
		area_center(CenterRegion, &CenterArea, &CenterRow, &CenterCol);

		HTuple ShortLength,LongLength,ColOrg;
		ShortLength = HTuple();
		LongLength = HTuple();
		if (0 != (Length1>=Length2))
		{
			ShortLength = Length2;
			LongLength = Length1;
		}
		else
		{
			ShortLength = Length1;
			LongLength = Length2;
		}

		if (((LongLength*2)<490)&&(LongLength>0))
		{
			ColOrg = CenterCol;

		}
		else
		{
			HTuple ImageArea, ImageRow, ImageCol;
			Hobject Domain;
			get_domain(Image, &Domain);
			area_center(Domain, &ImageArea, &ImageRow, &ImageCol);
			//如果搜索区域内有胶垫相连接的话，就用图像的中心点
			ColOrg = ImageCol;
		}

		m_vPos[0].m_x = (float)ColOrg[0].D();	

		//上边缘定位
		Hobject SearchRegLine;
		gen_region_line(&SearchRegLine, 90, ColOrg-160, 90, ColOrg+160);
		HTuple StartRow,StartCol,EndRow,EndCol;
		StartRow = 90;
		StartCol = ColOrg-160;
		EndRow = 90;
		EndCol = ColOrg+160;


		HTuple ResultRow, ResultColumn;
		Hobject Regions;
		ExtractEdgesPoints(Image, 10, 160, 10, 1, ThredValueUp, "positive", "first", EndRow, EndCol, StartRow, StartCol, &ResultRow, &ResultColumn,&Regions);

		HTuple RowBegin, ColBegin, RowEnd, ColEnd;
		FitLineUseEdgesPoints(ResultRow, ResultColumn, 5, &RowBegin, &ColBegin, &RowEnd, &ColEnd);

		Hobject BaseRegionLine;
		gen_region_line(&BaseRegionLine, RowBegin, ColBegin, RowEnd, ColEnd);
		HTuple RowOrg;
		RowOrg = (RowBegin+RowEnd)/2;

		m_vPos[0].m_y = (float)RowOrg[0].D();

		//
		Hobject Cross,CrossReg;
		gen_cross_contour_xld(&Cross, ResultRow, ResultColumn, 6, 0.785398);
		gen_region_contour_xld(Cross,&CrossReg,"margin");

		if(m_bDebugImg)
		{
			concat_obj (m_vRgn[id], SearchRgn, &m_vRgn[id]);
			concat_obj (m_vRgn[id], CenterRegion, &m_vRgn[id]);
			concat_obj (m_vRgn[id], Regions, &m_vRgn[id]);
			concat_obj (m_vRgn[id], CrossReg, &m_vRgn[id]);
			concat_obj (m_vRgn[id], RegionSelected, &m_vRgn[id]);
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
STDMETHODIMP CRubberMatAlgo::SideCenterLocationAlgo11Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("726$\
					 381;B$\
					 939;LP;10;100;1;25$\
					 938;LP;10;200;1;60$\
					 940;LP;1;799;1;700$\
					 941;LP;1;245;1;210$\
					 1141;LP;1;150;1;50$\
					 724;LP;1;400;1;20$\
					 725;LP;1;120;1;15$\
					 1057;LP;0;250;1;30$\
					 1058;LP;1;250;1;20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::SideCenterLocationAlgo12(VARIANT* rgnID ,VARIANT*pGrayValueLow,VARIANT*pGrayValueHigh,VARIANT*pThredValueSide,VARIANT*pRegHeight)
{
	//兼容胶垫跳动
	int id = (int)rgnID->fltVal;
	int GrayValueLow     =(int)pGrayValueLow->fltVal;
	int GrayValueHigh    =(int)pGrayValueHigh->fltVal;
	int ThredValueSide   =(int)pThredValueSide->fltVal;
	int RegionHeight     = (int)pRegHeight->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG        
	try
	{
#endif
		Hobject  Image = m_crtImg;
		HTuple m_imgWidth,m_imgHeight;
		get_image_size(Image,&m_imgWidth,&m_imgHeight);	
		//	
		Hobject Region,RegionFillUp,RegionOpening,ConnectedRegions;	
		Hobject UpperRect,Partitioned,SelectedRegions;
		HTuple  Row1, Column1, Row2, Column2,Num1;
		threshold (Image, &Region, GrayValueLow, GrayValueHigh);	
		fill_up_shape (Region, &RegionFillUp, "area", 1, 500);	
		opening_rectangle1 (RegionFillUp, &RegionOpening, 150, 100);	
		connection (RegionOpening, &ConnectedRegions);	
		select_shape_std (ConnectedRegions, &SelectedRegions, "max_area", 70);	
		smallest_rectangle1 (SelectedRegions, &Row1, &Column1, &Row2, &Column2);	
		if(Row1.Num()!=1)
		{
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y =0.0;
			m_vRgn[id] = RegionOpening ;
			return S_FALSE;
		}
	    if(Row1 * Column1 * Row2 * Column2 == 0)
		{
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = RegionOpening;
			return S_FALSE;
		}
		gen_rectangle1 (&UpperRect,Row1-30, Column1+20, Row2+30, Column2-20);
		partition_rectangle (UpperRect, &Partitioned, 10, 200);
		sort_region (Partitioned, &Partitioned, "first_point", "true", "column");
		HTuple MeasureRow = HTuple();
		HTuple MeasureCol= HTuple();
		count_obj (Partitioned, &Num1);
		for (int i=1;i<=Num1;i++)
		{
			Hobject ObjectSelected;
			HTuple Row11, Column11, Phi11, Length1, Length2,MeasureHandle;
			HTuple RowEdge, ColumnEdge, Amplitude, Distance11,minvalue;
			select_obj (Partitioned, &ObjectSelected, i);
			smallest_rectangle2 (ObjectSelected, &Row11, &Column11, &Phi11, &Length1, &Length2);
			gen_measure_rectangle2 (Row11, Column11, Phi11, Length1, Length2, m_imgWidth,m_imgHeight, "nearest_neighbor", &MeasureHandle);
			measure_pos (Image, MeasureHandle, 1, ThredValueSide, "positive", "all", &RowEdge, &ColumnEdge, &Amplitude, &Distance11);
			if(RowEdge.Num()>1)
			{
				tuple_min (RowEdge, &minvalue);
				MeasureRow = MeasureRow.Concat(minvalue);
				MeasureCol =MeasureCol.Concat(ColumnEdge[0]);
			}
			else
			{
				MeasureRow = MeasureRow.Concat(RowEdge);
				MeasureCol = MeasureCol.Concat(ColumnEdge);

			}
			close_measure(MeasureHandle);
		}
		if(MeasureRow.Num()<2)
		{
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y =0.0;
			m_vRgn[id] = UpperRect ;
			return S_FALSE;
		}
		Hobject Contour,SmoothedContours,RegionMoved;
		gen_contour_polygon_xld (&Contour, MeasureRow, MeasureCol);
		smooth_contours_xld (Contour, &SmoothedContours, 7);
		get_contour_xld (SmoothedContours, &Row1, &Column1);
		gen_region_polygon (&m_BaseRegionLine,Row1, Column1);
		move_region (m_BaseRegionLine, &RegionMoved, RegionHeight/2, 0);
		dilation_rectangle1 (RegionMoved, &m_centerLocationRegion, 1, RegionHeight);
		opening_rectangle1(m_centerLocationRegion,&m_centerLocationRegion,25,1);
		HTuple Area,CenterRow,CenterCol;
		area_center(m_BaseRegionLine,&Area,&CenterRow,&CenterCol);
		m_vPos[0].m_x =CenterRow[0].D();
		m_vPos[0].m_y =CenterCol[0].D();

		if(m_bDebugImg)
		{
			m_vRgn[id] = m_centerLocationRegion;

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
STDMETHODIMP CRubberMatAlgo::SideCenterLocationAlgo12Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("1306$\
					 381;B$\
					 1305;LP;1;255;1;30$\
					 1307;LP;1;255;1;200$\
					 1308;LP;1;799;1;700$\
					 1309;LP;1;245;1;210");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::SideRegionLocationAlgo(VARIANT* rgnId,VARIANT* pWidth,VARIANT* pHeightUpper,VARIANT* pHeightLower)
{
	int id = (int)rgnId->fltVal;
	int HalfWidth =(int)pWidth->fltVal;
	float HeightUpper =(float)pHeightUpper->fltVal;
	float HeightLower =(float)pHeightLower->fltVal;

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
		Hobject Image,ImageDomain,Rectangle;
		gen_rectangle1(&ImageDomain,1,1,m_ImageHeight-1,m_ImageWidth-1);

		if(HeightUpper>HeightLower)
		{
			swap(*(&HeightLower),*(&HeightUpper));
		}
		if(HeightUpper==HeightLower)
		{
			HeightUpper=HeightUpper-1;
		}
		if (fCenterY+HeightUpper<=1)
		{

			HeightUpper=fCenterY-5;

		}

		gen_rectangle1(&Rectangle,fCenterY+HeightUpper,fCenterX-HalfWidth/2.0,fCenterY+HeightLower,fCenterX+HalfWidth/2.0);
		intersection(Rectangle, ImageDomain, &RegionDetect);
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
STDMETHODIMP CRubberMatAlgo::SideRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1050$\
					 167;B$\
					 668;LP;100;750;2;450$\
					 1051;FP;-80;300;1;10$\
					 1052;FP;1;300;1;90"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::UpBaseEdgeDetectAlgo(VARIANT* rgnId, VARIANT* pWidthSize, VARIANT* pVerticalPos, VARIANT* pMaskSize, VARIANT* pThredValue, VARIANT* pBaseLineArea)
{
	int rId           = (int)rgnId->fltVal;

	float WidthSize     = (float)pWidthSize->fltVal;
	float VerticalPos     = (float)pVerticalPos->fltVal;

	float maskSize    = (float)pMaskSize->fltVal;
	double ThredValue   = (double)pThredValue->fltVal;

	m_vErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseLineArea);
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
		//
		Hobject crtImg = m_crtImg;
		Hobject ABsearchImage;
		//整个胶垫的搜索区域
		reduce_domain(crtImg, rgn, &ABsearchImage);
		//
		HTuple Row1, Column1, Row2, Column2;
		smallest_rectangle1(rgn,&Row1, &Column1, &Row2, &Column2);

		float fCenterX = 0.0;
		float fCenterY = 0.0;

		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		if (fCenterY==0.0)
		{
			fCenterY=20;
		}
		if (WidthSize>=fCenterX)
		{
			WidthSize=fCenterX-5;
		}
		if (((fCenterX-WidthSize)<Column1[0].I()+2)||(fCenterX+WidthSize>Column2[0].I()-2))
		{
			WidthSize=20;
		}
		Hobject SearchRegLine;
		gen_region_line(&SearchRegLine, fCenterY, HTuple(fCenterX-WidthSize), fCenterY, HTuple(fCenterX+WidthSize));
		HTuple StartRow,StartCol,EndRow,EndCol;
		StartRow=fCenterY;
		StartCol=fCenterX-WidthSize;
		EndRow=fCenterY;
		EndCol=fCenterX+WidthSize;

		HTuple ResultRow, ResultColumn;
		Hobject Regions;
		ExtractEdgesPoints(ABsearchImage, 20, 60, 10, maskSize, ThredValue, "positive", "first", EndRow, EndCol, StartRow, StartCol, &ResultRow, &ResultColumn,&Regions);

		HTuple RowBegin, ColBegin, RowEnd, ColEnd;
		FitLineUseEdgesPoints(ResultRow, ResultColumn, 10, &RowBegin, &ColBegin, &RowEnd, &ColEnd);

		/*
		if(ResultColumn.Num()<3)
		{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseLineArea);
		return S_FALSE;

		}*/
		Hobject Cross,CrossReg;
		gen_cross_contour_xld(&Cross, ResultRow, ResultColumn, 6, 0.785398);
		gen_region_contour_xld(Cross,&CrossReg,"margin");

		//Hobject Contour;
		//gen_contour_polygon_xld(&Contour, ResultRow, ResultColumn);
		//HTuple RowBegin, ColBegin, RowEnd, ColEnd, Nr, Nc, Dist;
		//fit_line_contour_xld(Contour, "tukey", -1, 0, 5, 2, &RowBegin, &ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
		Hobject BaseRegionLine;
		gen_region_line(&BaseRegionLine, RowBegin, ColBegin, RowEnd, ColEnd);

		//往下平移5个像素
		move_region(BaseRegionLine,&BaseRegionLine,5,0);

		HTuple BaseLineArea, BaseLineRow, BaseLineCol;
		area_center(BaseRegionLine, &BaseLineArea, &BaseLineRow, &BaseLineCol);

		m_BaseRegionLine=BaseRegionLine;
		if(m_bDebugImg)
		{
			concat_obj (m_vRgn[rId], Regions, &m_vRgn[rId]);
			concat_obj (m_vRgn[rId], SearchRegLine, &m_vRgn[rId]);
			concat_obj (m_vRgn[rId], CrossReg, &m_vRgn[rId]);
		}
		//

		if(BaseLineArea > 1)
		{
			Hobject BaseRegionLineDilation;
			retValue = BaseLineArea[0].I();
			dilation_circle(BaseRegionLine,&BaseRegionLineDilation,2);
			m_vErrorRgn[rId] = BaseRegionLine;
		}
		else
		{
			Hobject Circle;
			gen_circle(&Circle, m_ImageHeight/2.0, m_ImageWidth/2.0, 100);
			retValue = -1;
			m_vErrorRgn[rId] = Circle;
		}
		retValue.Detach(pBaseLineArea);

#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseLineArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::UpBaseEdgeDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1036$\
					 397;R;FP;0;10000;1;>=#1$\
					 381;B$\
					 1043;FP;1;500;1;220$\
					 1044;FP;-50;50;1;1$\
					 1037;FP;0.1;10;0.1;1.0$\
					 1038;LP;1;100;1;20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CRubberMatAlgo::R_EdgePointsFitCurve(Hobject SearchReg, Hobject Image,HTuple PartInterval, HTuple LineLength, HTuple RemoveSize, HTuple Threshold,HTuple * DefinedRow, HTuple *DefinedCol)
{
	Hobject ReinforcedRegPartitioned,SortedRegions,ReinRegPartitioned;
	HTuple PartitionedNum,SearchRegArea,SearchRegRow,SearchRegCol;
	area_center(SearchReg,&SearchRegArea,&SearchRegRow,&SearchRegCol);
	if (SearchRegArea==0||SearchRegArea.Num()==0)
	{
		return;
	}
	partition_rectangle(SearchReg, &ReinforcedRegPartitioned, PartInterval, 300);
	sort_region(ReinforcedRegPartitioned, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &PartitionedNum);
	gen_empty_obj(&ReinRegPartitioned);
	if ((RemoveSize<1)||(RemoveSize>PartitionedNum))
	{
		return;
	}
	Hobject PartitionedSelected;
	for (int i=RemoveSize[0].I();i<=(PartitionedNum-RemoveSize);i++)
	{
		select_obj(SortedRegions, &PartitionedSelected, i);
		concat_obj(PartitionedSelected, ReinRegPartitioned, &ReinRegPartitioned);

	}
	HTuple PartitionedArea, PartitionedRow, PartitionedCol;
	area_center(ReinRegPartitioned, &PartitionedArea, &PartitionedRow, &PartitionedCol);
	if (PartitionedArea.Num()==0)
	{
		return;
	}

	HTuple StepNum,BeginRow,BeginCol,EndRow,EndCol;
	StepNum = PartitionedRow.Num();
	HTuple Width, Height;
	get_image_size(Image, &Width, &Height);
	(*DefinedRow) = HTuple();
	(*DefinedCol) = HTuple();

	Hobject ObjectSelected;
	HTuple RowC, ColC, Phi, Length1, Length2,MsrHandle_Measure;
	for (int i=1;i<=StepNum;i++)
	{
		select_obj(ReinRegPartitioned, &ObjectSelected, i);
		smallest_rectangle2(ObjectSelected, &RowC, &ColC, &Phi, &Length1, &Length2);
		//异常
		if ((RowC.Num()==0)||(Length1<1)||(Length2<1))
		{
			continue;
		}
		gen_measure_rectangle2(RowC, ColC, Phi, Length1, Length2, Width, Height, "nearest_neighbor", &MsrHandle_Measure);
		if (Length1*0.5<=1)
		{
			continue;
		}
		HTuple RowEdge, ColEdge, Amplitude, Distance;
		measure_pos(Image, MsrHandle_Measure, 1, Threshold, "positive", "first", &RowEdge, &ColEdge, &Amplitude, &Distance);
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

			*DefinedRow = (*DefinedRow).Concat(tRow);
			*DefinedCol = (*DefinedCol).Concat(tCol);
		}
	}
	return;
}
void CRubberMatAlgo::P_EdgePointsFitCurve(Hobject SearchReg, Hobject Image,HTuple PartInterval, HTuple LineLength, HTuple RemoveSize, HTuple * DefinedRow, HTuple *DefinedCol)
{
	Hobject ReinforcedRegPartitioned,SortedRegions,ReinRegPartitioned;
	HTuple PartitionedNum,SearchRegArea,SearchRegRow,SearchRegCol;
	area_center(SearchReg,&SearchRegArea,&SearchRegRow,&SearchRegCol);
	if (SearchRegArea==0||SearchRegArea.Num()==0)
	{
		return;
	}
	partition_rectangle(SearchReg, &ReinforcedRegPartitioned, PartInterval, 300);
	sort_region(ReinforcedRegPartitioned, &SortedRegions, "first_point", "true", "column");
	count_obj(SortedRegions, &PartitionedNum);
	gen_empty_obj(&ReinRegPartitioned);
	if ((RemoveSize<1)||(RemoveSize>PartitionedNum))
	{
		return;
	}
	Hobject PartitionedSelected;
	for (int i=RemoveSize[0].I();i<=(PartitionedNum-RemoveSize);i++)
	{
		select_obj(SortedRegions, &PartitionedSelected, i);
		concat_obj(PartitionedSelected, ReinRegPartitioned, &ReinRegPartitioned);

	}
	HTuple PartitionedArea, PartitionedRow, PartitionedCol;
	area_center(ReinRegPartitioned, &PartitionedArea, &PartitionedRow, &PartitionedCol);

	if (PartitionedArea.Num()==0)
	{
		return;
	}
	HTuple StepNum,BeginRow,BeginCol,EndRow,EndCol;
	StepNum = PartitionedRow.Num();
	BeginRow = PartitionedRow-(LineLength/2);
	BeginCol = PartitionedCol;
	EndRow = PartitionedRow+(LineLength/2);
	EndCol = PartitionedCol;

	HTuple MatrixID,MatrixIDX,MatrixIDY,MatrixID_sub;
	create_matrix(LineLength+1, StepNum, 0, &MatrixID);
	create_matrix(LineLength+1, StepNum, 0, &MatrixIDX);
	create_matrix(LineLength+1, StepNum, 0, &MatrixIDY);

	create_matrix(1, StepNum, 0, &MatrixID_sub);
	//create_matrix(1, StepNum, 0, &MatrixID_sub1);
	//create_matrix(1, StepNum, 0, &MatrixID_sub2);

	//create_matrix(LineLength+1, 1, 0, &MatrixID_subY);
	//create_matrix(LineLength, 1, 0, &MatrixID_subY1);
	//create_matrix(LineLength, 1, 0, &MatrixID_subY2);
	HTuple pRow,pCol,RowRound,ColRound,Grayval1;
	Hobject Region3;
	for (int i=0;i<=LineLength;i++)
	{
		pRow = BeginRow+((EndRow-BeginRow)*(i/(LineLength+0.001)));
		pCol = BeginCol+((EndCol-BeginCol)*(i/(LineLength+0.001)));
		tuple_round(pRow, &RowRound);
		tuple_round(pCol, &ColRound);
		gen_region_points(&Region3, RowRound, ColRound);
		get_grayval(Image, RowRound, ColRound, &Grayval1);
		if (Grayval1.Num()==0)
		{
			continue;
		}
		set_full_matrix(MatrixID_sub, Grayval1);
		set_sub_matrix(MatrixID, MatrixID_sub, i, 0);
		set_full_matrix(MatrixID_sub, RowRound);
		set_sub_matrix(MatrixIDY, MatrixID_sub, i, 0);
		set_full_matrix(MatrixID_sub, ColRound);
		set_sub_matrix(MatrixIDX, MatrixID_sub, i, 0);

	}
	(*DefinedRow) = HTuple();
	(*DefinedCol) = HTuple();

	HTuple Values1,Values2,Diff,Indices,ValueX,ValueY,MatrixID_subY1,MatrixID_subY2;
	for (int Index1=0;Index1<=StepNum-1;Index1++)
	{
		get_sub_matrix(MatrixID, 0, Index1, LineLength, 1, &MatrixID_subY1);
		get_full_matrix(MatrixID_subY1, &Values1);
		get_sub_matrix(MatrixID, 1, Index1, LineLength, 1, &MatrixID_subY2);
		get_full_matrix(MatrixID_subY2, &Values2);
		tuple_sub(Values1, Values2, &Diff);
		tuple_sort_index(Diff, &Indices);
		get_value_matrix(MatrixIDX, Indices.Select(LineLength-1), Index1, &ValueX);
		get_value_matrix(MatrixIDY, Indices.Select(LineLength-1), Index1, &ValueY);
		(*DefinedRow).ReplaceElements(Index1,ValueY);
		(*DefinedCol).ReplaceElements(Index1,ValueX);

		clear_matrix(MatrixID_subY1);
		clear_matrix(MatrixID_subY2);
	}

	clear_matrix(MatrixID);
	clear_matrix(MatrixIDX);
	clear_matrix(MatrixIDY);
	clear_matrix(MatrixID_sub);

	return;
}

STDMETHODIMP CRubberMatAlgo::ScrapEdgeDetectAlgo(VARIANT* rgnId, VARIANT* pUpMoveSize, VARIANT* pDownMoveSize,VARIANT* pSearchSize, VARIANT* pThredValue,VARIANT* pSensitivity,VARIANT* pEdgeArea)
{
	int rId           = (int)rgnId->fltVal;
	int UpMoveSize    = (int)pUpMoveSize->fltVal;
	int DownMoveSize    = (int)pDownMoveSize->fltVal;
	int SearchSize    =(int)pSearchSize->fltVal;

	double ThredValue    =(double)pThredValue->fltVal;
	double Sensitivity    =(double)pSensitivity->fltVal;

	m_vErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if((rgn.Id() == H_EMPTY_REGION)||m_BaseRegionLine.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pEdgeArea);
		return S_FALSE;
	}
	//
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
			m_dictSpecialData[rId].clear();
		}
		//画一个囊括整个胶垫的矩形区域
		Hobject crtImg = m_crtImg;
		Hobject crtImgReduced;
		reduce_domain(crtImg, rgn, &crtImgReduced);

		Hobject UpEdgeDilation,DownEdgeDilation,UpCotReg,DownCotReg;
		gen_empty_region(&UpEdgeDilation);
		gen_empty_region(&DownEdgeDilation);

		gen_empty_region(&UpCotReg);
		gen_empty_region(&DownCotReg);


		//UpEdge
		Hobject UpRegionLine,UpRegion;
		move_region(m_BaseRegionLine, &UpRegionLine, UpMoveSize, 0);

		dilation_rectangle1(UpRegionLine, &UpRegion, 1, SearchSize);
		//
		Hobject domain;
		HTuple UpRegionArea,UpRegionRow,UpRegionCol;
		get_domain(crtImg,&domain);
		intersection(domain,UpRegion,&UpRegion);
		area_center(UpRegion,&UpRegionArea,&UpRegionRow,&UpRegionCol);
		if(UpRegionArea == 0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pEdgeArea);
			return S_FALSE;
		}
		Hobject ImageReduced,ExpandedImage,ImageReduced1,DerivGauss,ImageReduced2;
		reduce_domain(crtImgReduced,UpRegion, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 5);
		reduce_domain(ExpandedImage, UpRegion, &ImageReduced1);
		derivate_gauss(ImageReduced1, &DerivGauss, 1, "y");
		//
		Hobject ImageMedian;
		median_image(DerivGauss, &ImageMedian, "circle", 1, "continued");
		reduce_domain(ImageMedian, UpRegion, &ImageReduced1);
		//

		HTuple DefinedRow1, DefinedCol1;
		R_EdgePointsFitCurve(UpRegion, ImageReduced1, 5, SearchSize, 2,ThredValue, &DefinedRow1, &DefinedCol1);

		HTuple Num1;
		Num1 = DefinedRow1.Num();

		if (0 != (Num1>Sensitivity))
		{
			Hobject UpContXld,UpContXldSmoothed;
			gen_region_points(&UpCotReg, DefinedRow1, DefinedCol1);
			gen_contour_nurbs_xld(&UpContXld, DefinedRow1, DefinedCol1, "auto", "auto", 3, 1, 5);
			smooth_contours_xld(UpContXld, &UpContXldSmoothed, 15);

			HTuple UpContXldRow, UpContXldCol;
			Hobject UpContSmoothedReg;
			get_contour_xld(UpContXldSmoothed, &UpContXldRow, &UpContXldCol);
			gen_region_points(&UpContSmoothedReg, UpContXldRow, UpContXldCol);
			//gen_region_polygon(&ContSmoothedReg, ContXldRow, ContXldCol);

			HTuple BaseDregArea, BaseDregRow, BaseDregCol,UpCotRegArea, UpCotRegRow, UpCotRegCol;
			area_center(m_BaseRegionLine, &BaseDregArea, &BaseDregRow, &BaseDregCol);
			area_center(UpContSmoothedReg, &UpCotRegArea, &UpCotRegRow, &UpCotRegCol);

			Hobject UpEdgeReg;
			move_region(m_BaseRegionLine, &UpEdgeReg, UpCotRegRow-BaseDregRow+0, 0);
			dilation_rectangle1(UpEdgeReg, &UpEdgeDilation, 1, 6);

		}


		if(m_bDebugImg)
		{
			concat_obj (m_vRgn[rId], UpRegion, &m_vRgn[rId]);
			concat_obj (m_vRgn[rId], UpCotReg, &m_vRgn[rId]);
		}

		//DownEdge
		Hobject DownRegionLine,DownRegion;
		move_region(m_BaseRegionLine, &DownRegionLine, DownMoveSize, 0);

		dilation_rectangle1(DownRegionLine, &DownRegion, 1, SearchSize);
		//
		HTuple DownRegionArea,DownRegionRow,DownRegionCol;;
		intersection(domain,DownRegion,&DownRegion);
		area_center(DownRegion,&DownRegionArea,&DownRegionRow,&DownRegionCol);
		if(DownRegionArea == 0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pEdgeArea);
			return S_FALSE;
		}
		Hobject DerivGauss2,ImageMedian2;
		reduce_domain(crtImgReduced,DownRegion, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 5);
		reduce_domain(ExpandedImage, DownRegion, &ImageReduced2);
		derivate_gauss(ImageReduced2, &DerivGauss2, 1, "y");
		//reduce_domain(DerivGauss, DownRegion, &ImageReduced2);
		//

		median_image(DerivGauss2, &ImageMedian2, "circle", 1, "continued");
		reduce_domain(ImageMedian2, DownRegion, &ImageReduced2);
		//
		HTuple DefinedRow2, DefinedCol2;
		R_EdgePointsFitCurve(DownRegion, ImageReduced2, 5, SearchSize, 2,ThredValue, &DefinedRow2, &DefinedCol2);

		HTuple Num2;
		Num2 = DefinedRow2.Num();

		if (0 != (Num2>Sensitivity))
		{
			Hobject DownContXld,DownContXldSmoothed;
			gen_region_points(&DownCotReg, DefinedRow2, DefinedCol2);
			gen_contour_nurbs_xld(&DownContXld, DefinedRow2, DefinedCol2, "auto", "auto", 3, 1, 5);
			smooth_contours_xld(DownContXld, &DownContXldSmoothed, 15);

			HTuple DownContXldRow, DownContXldCol;
			Hobject DownContSmoothedReg;
			get_contour_xld(DownContXldSmoothed, &DownContXldRow, &DownContXldCol);
			gen_region_points(&DownContSmoothedReg, DownContXldRow, DownContXldCol);

			HTuple BaseDregArea, BaseDregRow, BaseDregCol,DownCotRegArea, DownCotRegRow, DownCotRegCol;
			area_center(m_BaseRegionLine, &BaseDregArea, &BaseDregRow, &BaseDregCol);
			area_center(DownContSmoothedReg, &DownCotRegArea, &DownCotRegRow, &DownCotRegCol);

			Hobject DownEdgeReg;
			move_region(m_BaseRegionLine, &DownEdgeReg, DownCotRegRow-BaseDregRow, 0);
			dilation_rectangle1(DownEdgeReg, &DownEdgeDilation, 1, 6);

		}

		if(m_bDebugImg)
		{
			concat_obj (m_vRgn[rId], DownRegion, &m_vRgn[rId]);
			concat_obj (m_vRgn[rId], DownCotReg, &m_vRgn[rId]);
		}
		Hobject EdgeDilationUnion;
		union2(UpEdgeDilation,DownEdgeDilation,&EdgeDilationUnion);

		union1(EdgeDilationUnion,&EdgeDilationUnion);
		Hobject EdgeDilationConnected;
		connection(EdgeDilationUnion,&EdgeDilationConnected);
		HTuple EdgesNum;
		count_obj(EdgeDilationConnected,&EdgesNum);
		//空区域个数为1
		HTuple Area,Row,col;
		area_center(EdgeDilationUnion,&Area,&Row,&col);
		Hobject EdgeDilation;
		if(EdgesNum>1)
		{
			if(Num1>=Num2)
			{
				EdgeDilation=UpEdgeDilation;
			}
			else
			{
				EdgeDilation=DownEdgeDilation;
			}
		}
		else if((EdgesNum==1)&&(Area>0))
		{
			EdgeDilation=EdgeDilationUnion;
		}
		else
		{
			//UP
			HTuple DefinedRow1, DefinedCol1;
			R_EdgePointsFitCurve(UpRegion, ImageReduced1, 5, SearchSize, 2,ThredValue, &DefinedRow1, &DefinedCol1);

			HTuple Num1;
			Num1 = DefinedRow1.Num();
			//
			HTuple DefinedRow2, DefinedCol2;
			R_EdgePointsFitCurve(DownRegion, ImageReduced2, 5, SearchSize, 2,ThredValue, &DefinedRow2, &DefinedCol2);

			HTuple DefinedRow, DefinedCol;
			HTuple Num2;
			Num2 = DefinedRow2.Num();
			if (Num1>=Num2)
			{
				DefinedRow=DefinedRow1;
				DefinedCol=DefinedCol1;
			}
			else if(Num1<Num2)
			{
				DefinedRow=DefinedRow2;
				DefinedCol=DefinedCol2;
			}

			if (DefinedRow.Num()<=3||(Num1==Num2))
			{
				//如果没有提取到切边，剔除上下边缘区域
				Hobject UpEdgeRegDilation;
				dilation_rectangle1(m_BaseRegionLine,&UpEdgeRegDilation,1,40);

				Hobject DownEdgeReg,DownEdgeRegDilation;
				move_region(m_BaseRegionLine,&DownEdgeReg,125,0);
				dilation_rectangle1(DownEdgeReg,&DownEdgeRegDilation,1,40);
				Hobject EdgeRegUnion;
				union2(UpEdgeRegDilation,DownEdgeRegDilation,&EdgeRegUnion);
				union1(EdgeRegUnion,&EdgeRegUnion);
				m_EdgeDilation=EdgeRegUnion;

				//Hobject Circle;
				//gen_circle(&Circle, m_ImageHeight/2.0, m_ImageWidth/2.0, 100);
				retValue = 100;
				m_vErrorRgn[rId] = EdgeRegUnion;
				retValue.Detach(pEdgeArea);
				return S_OK;
			}

			Hobject UpContXld,UpContXldSmoothed;
			gen_region_points(&UpCotReg, DefinedRow, DefinedCol);
			gen_contour_nurbs_xld(&UpContXld, DefinedRow, DefinedCol, "auto", "auto", 2, 1, 5);
			smooth_contours_xld(UpContXld, &UpContXldSmoothed, 15);

			HTuple UpContXldRow, UpContXldCol;
			Hobject UpContSmoothedReg;
			get_contour_xld(UpContXldSmoothed, &UpContXldRow, &UpContXldCol);
			gen_region_points(&UpContSmoothedReg, UpContXldRow, UpContXldCol);
			//gen_region_polygon(&ContSmoothedReg, ContXldRow, ContXldCol);

			HTuple BaseDregArea, BaseDregRow, BaseDregCol,UpCotRegArea, UpCotRegRow, UpCotRegCol;
			area_center(m_BaseRegionLine, &BaseDregArea, &BaseDregRow, &BaseDregCol);
			area_center(UpContSmoothedReg, &UpCotRegArea, &UpCotRegRow, &UpCotRegCol);

			Hobject UpEdgeReg;
			move_region(m_BaseRegionLine, &UpEdgeReg, UpCotRegRow-BaseDregRow-5, 0);
			dilation_rectangle1(UpEdgeReg, &EdgeDilation, 1,20);
		}

		if(m_bDebugImg)
		{
			concat_obj (m_vRgn[rId], UpCotReg, &m_vRgn[rId]);
		}
		m_EdgeDilation=EdgeDilation;

		//
		HTuple EdgeArea,EdgeRow,EdgeCol;
		area_center(EdgeDilation,&EdgeArea,&EdgeRow,&EdgeCol);

		if (EdgeArea>0)
		{
			retValue = EdgeArea[0].I();
			m_vErrorRgn[rId] = EdgeDilation;
		}
		else
		{
			//如果没有提取到切边，剔除上下边缘区域
			Hobject UpEdgeRegDilation;
			dilation_rectangle1(m_BaseRegionLine,&UpEdgeRegDilation,1,50);

			Hobject DownEdgeReg,DownEdgeRegDilation;
			move_region(m_BaseRegionLine,&DownEdgeReg,125,0);
			dilation_rectangle1(DownEdgeReg,&DownEdgeRegDilation,1,50);
			Hobject EdgeRegUnion;
			union2(UpEdgeRegDilation,DownEdgeRegDilation,&EdgeRegUnion);
			union1(EdgeRegUnion,&EdgeRegUnion);
			m_EdgeDilation=EdgeRegUnion;

			//Hobject Circle;
			//gen_circle(&Circle, m_ImageHeight/2.0, m_ImageWidth/2.0, 100);
			retValue = 100;
			m_vErrorRgn[rId] = EdgeRegUnion;
		}
		retValue.Detach(pEdgeArea);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pEdgeArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::ScrapEdgeDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1039$\
					 397;R;FP;0;10000;1;>=#0$\
					 381;B$\
					 1040;LP;0;60;1;20$\
					 1041;LP;0;130;1;75$\
					 998;LP;1;100;1;30$\
					 1042;FP;1;20;0.5;9$\
					 1056;LP;4;100;1;5");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::ScrapRegionExtraction(VARIANT* rgnId, VARIANT* pUpMoveSize, VARIANT* pDownMoveSize,VARIANT*pErosioSizeV,VARIANT* pSensitivity)
{
	//提取切线区域
	int rId           = (int)rgnId->fltVal;
	int UpMoveSize    = (int)pUpMoveSize->fltVal;
	int DownMoveSize    = (int)pDownMoveSize->fltVal;
	int ErosioSizeV =(int)pErosioSizeV->fltVal;
	int Sensitivity    =(int)pSensitivity->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	m_EdgeDilation = newRgn;
	//
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
			m_dictSpecialData[rId].clear();
		}

		Hobject  Image = m_crtImg;
		Hobject UpperLocation,DownLocation,RegionUnion,ImageReduced,ExpandedImage;
		Hobject EdgeAmplitude,Edges;
		move_region (m_BaseRegionLine, &UpperLocation, UpMoveSize, 0);
		dilation_rectangle1 (UpperLocation, &UpperLocation, 1, ErosioSizeV);
		move_region (m_BaseRegionLine, &DownLocation, DownMoveSize, 0);
		dilation_rectangle1 (DownLocation, &DownLocation, 1, ErosioSizeV);
		union2 (UpperLocation, DownLocation, &RegionUnion);
		reduce_domain (Image, RegionUnion, &ImageReduced);
		expand_domain_gray (ImageReduced, &ExpandedImage, 5);
		sobel_amp (ExpandedImage, &EdgeAmplitude,"sum_abs", 3);
		//
		Hobject ImageMean,RegionDynThresh;
		mean_image(EdgeAmplitude, &ImageMean, 1, 20);
		dyn_threshold(EdgeAmplitude, ImageMean, &RegionDynThresh, Sensitivity, "light");
		//
		//threshold (EdgeAmplitude,&Edges, Sensitivity, 255);
		intersection (RegionDynThresh, RegionUnion, &Edges);
		Hobject RegionIntersection;
		HTuple  UpperArea,DownArea, Row1, Column1;
		intersection (Edges, UpperLocation, &RegionIntersection);
		area_center (RegionIntersection, &UpperArea, &Row1, &Column1);
		intersection (Edges, DownLocation, &RegionIntersection);
		area_center (RegionIntersection, &DownArea, &Row1, &Column1);
		if(UpperArea>DownArea)
		{
			m_EdgeDilation= UpperLocation;
		}
		else
		{
			m_EdgeDilation= DownLocation;
		}
		m_vRgn[rId] = m_EdgeDilation ;

#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::ScrapRegionExtractionHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1039$\
					 381;B$\
					 1040;LP;0;1000;1;20$\
					 1041;LP;0;1000;1;75$\
					 995;LP;5;35;1;20$\
					 1056;LP;1;255;1;15");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::MoveBlockDetectAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//水平分块检测
	int rId = (int)rgnId->fltVal;
	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue = (float)pThresholdValue->fltVal;
	int GrayValue = (int)pGrayValue->fltVal;

	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	m_vErrorRgn[rId].Reset();
	if(rgn.Id() == H_EMPTY_REGION)
	{
		//为空不报错
		retValue = 0;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
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

		HTuple Row1, Column1, Row2, Column2,ExpGray;
		HTuple Width,Height,MeanGray,DeviationGray,MeanDeviation,MeanLength;
		HTuple AbsValue, AbsSorted, LocationId, LengthTuple, errorValue, ResultID;
		HTuple MeanSorted, LocationId2, Length1, ResultID2, ResultID3, Sorted, UniqTule;
		HTuple RgnArea, Row_test, Column_test;

		Hobject PartitionedRgn;
		Hobject BackTransRegion, DefectRegs,ResultDefect;

		Hobject CrtImage = m_crtImg;
		smallest_rectangle1 (rgn, &Row1, &Column1, &Row2, &Column2);
		HTuple area,row,col;
		area_center(rgn,&area,&row,&col);
		if(Row1.Num()==0||area==0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Width  = Column2 - Column1;
		Height = Row2 - Row1;
		if(Width / NumPart * Height == 0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		partition_rectangle(rgn, &PartitionedRgn,Width/NumPart, Height);
		sort_region (PartitionedRgn, &PartitionedRgn,"first_point", "true", "column");
		intensity(PartitionedRgn, CrtImage, &MeanGray, &DeviationGray);
		tuple_length(MeanGray, &MeanLength);
		MeanDeviation = HTuple();
		for (int i=0; i<=MeanLength-2; i+=1)
		{
			tuple_deviation(HTuple(MeanGray[i]).Concat(HTuple(MeanGray[i+1])), &ExpGray);
			MeanDeviation[i] = ExpGray; 
		}
		tuple_abs(MeanDeviation, &AbsValue);
		tuple_sort(AbsValue, &AbsSorted);
		tuple_sort_index(AbsValue, &LocationId);
		tuple_length(AbsValue, &LengthTuple);
		for (int i=LengthTuple[0].I()-1; i>=0; i+=-1)
		{
			if (0 != (HTuple(AbsSorted[i])<ThresholdValue))
			{
				if (0 != (i==(LengthTuple-1)))
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
		tuple_sort(MeanGray, &MeanSorted);
		tuple_sort_index(MeanGray, &LocationId2);
		tuple_length(MeanGray, &Length1);
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
		tuple_uniq(Sorted, &UniqTule);
		select_obj(PartitionedRgn, &ResultDefect, UniqTule);
		union1 (ResultDefect, &DefectRegs);
		area_center (DefectRegs, &RgnArea, &Row_test, &Column_test);
		//
		if(RgnArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = RgnArea[0].I();
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
STDMETHODIMP CRubberMatAlgo::MoveBlockDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1277$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 403;LP;5;500;1;200$\
					 404;FP;1;100;0.1;30$\
					 405;LP;1;255;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CRubberMatAlgo::SideDetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pSelect,VARIANT* pErosioSizeH,VARIANT* pErosioSizeV,VARIANT* pErosioSizeE)
{
	int id = (int)rgnId->fltVal;

	int MoveSize    = (int)pMoveSize->fltVal;
	//MoveSize 无效

	int Select =(int)pSelect->fltVal;
	int ErosioSizeH =(int)pErosioSizeH->fltVal;
	int ErosioSizeV =(int)pErosioSizeV->fltVal;

	float ErosioSizeE =(float)pErosioSizeE->fltVal;
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
		gen_rectangle1(&ImageDomain,1,1,m_ImageHeight-1,m_ImageWidth-1);

		TotalRgion=m_centerLocationRegion;
		move_region(TotalRgion,&TotalRgionMoved, MoveSize, 0);
		intersection(TotalRgion,TotalRgionMoved,&TotalRgion);

		Hobject EdgeRgion,RegionDiff;
		dilation_circle(m_EdgeDilation,&EdgeRgion,ErosioSizeE);
		//剔除上边缘高亮位置
		dilation_circle(m_BaseRegionLine, &TotalBaseLine, 8);
		move_region(TotalBaseLine,&TotalBaseLine, -7, 0);
		HTuple Row1,Col1,Row2,Col2;
		smallest_rectangle1(EdgeRgion,&Row1,&Col1,&Row2,&Col2);
		clip_region(TotalRgion,&TotalRgion,1,Col1,m_ImageHeight-1,Col2);

		difference(TotalRgion,TotalBaseLine,&TotalRgion);
		erosion_rectangle1(TotalRgion,&TotalRgion,ErosioSizeH,ErosioSizeV);
		difference(TotalRgion,EdgeRgion,&RegionDiff);
		//difference(RegionDiff,TotalBaseLine,&RegionDiff);

		Hobject RegionDiffConnected;
		connection(RegionDiff,&RegionDiffConnected);
		Hobject BigReg,SmallReg;
		if (Select==1)
		{
			select_shape_std(RegionDiffConnected, &BigReg, "max_area", 70);
			intersection(BigReg, ImageDomain, &BigReg);
			union1(BigReg,&BigReg);
			m_vRgn[id]=BigReg;

		} 
		else if(Select==2)
		{
			select_shape_std(RegionDiffConnected, &BigReg, "max_area", 70);
			difference(RegionDiffConnected, BigReg, &SmallReg);
			intersection(SmallReg, ImageDomain, &SmallReg);
			union1(SmallReg,&SmallReg);
			m_vRgn[id]=SmallReg;

		}
		else if(Select==3)
		{
			intersection(TotalRgion, ImageDomain, &TotalRgion);
			union1(TotalRgion,&TotalRgion);
			m_vRgn[id]=TotalRgion;
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
STDMETHODIMP CRubberMatAlgo::SideDetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1045$\
					 167;B$\
					 1048;LP;1;100;1;45$\
					 1049;LP;1;3;1;1$\
					 994;LP;1;60;1;2$\
					 995;LP;1;35;1;2$\
					 1047;FP;1;20;0.5;2"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::RegionExcludeScrapExtraction(VARIANT* rgnId,VARIANT* pSelect,VARIANT* pErosioSizeH,VARIANT* pErosioSizeV)
{
	//切口区域以外的区域
	int id = (int)rgnId->fltVal;
	int Select =(int)pSelect->fltVal;
	int ErosioSizeH =(int)pErosioSizeH->fltVal;
	int ErosioSizeV =(int)pErosioSizeV->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		Hobject DetectRegion,TotalRgion,RegConnected,ConnectedRegions,SortedRegions;
		Hobject ObjectSelected,BigReg,SmallReg,ImageDomain;
		HTuple Area, Row1, Column1,Indices;
		get_domain(m_crtImg,&ImageDomain);
	    difference(m_centerLocationRegion,m_EdgeDilation,&DetectRegion);
		connection (DetectRegion, &ConnectedRegions);
		//sort_region (ConnectedRegions, &SortedRegions, "first_point", "true", "row");
		area_center (ConnectedRegions, &Area, &Row1, &Column1);
		tuple_sort_index (Area, &Indices);
		if(Select==1 && Indices.Num()>=1)
		{
			select_shape_std(ConnectedRegions, &BigReg, "max_area", 70);
			//select_obj (SortedRegions, &ObjectSelected, Indices.Select(1)+1);
			erosion_rectangle1(BigReg,&BigReg,ErosioSizeH,ErosioSizeV);
			intersection(BigReg, ImageDomain, &BigReg);
			union1(BigReg,&BigReg);
			m_vRgn[id]=BigReg;
		}
		if(Select==2 && Indices.Num()>=1)
		{
			select_shape_std(ConnectedRegions, &BigReg, "max_area", 70);
			difference(ConnectedRegions, BigReg, &SmallReg);
			//select_obj (SortedRegions, &ObjectSelected, Indices.Select(0)+1);
			erosion_rectangle1(SmallReg,&SmallReg,ErosioSizeH,ErosioSizeV);
			intersection(BigReg, ImageDomain, &BigReg);
			union1(SmallReg,&SmallReg);
			m_vRgn[id]=SmallReg;
		}
		if(Select==3)
		{
			erosion_rectangle1(m_centerLocationRegion,&TotalRgion,ErosioSizeH,ErosioSizeV);
			m_vRgn[id]=TotalRgion;
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
STDMETHODIMP CRubberMatAlgo::RegionExcludeScrapExtractionHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1045$\
					 167;B$\
					 1049;LP;1;3;1;1$\
					 994;LP;1;200;1;2$\
					 995;LP;1;200;1;2"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CRubberMatAlgo::ScrapDynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		//为空不报错
		retValue = 0;
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
		expand_domain_gray (ImageReduced, &ExpandedImage, 25);
		reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
		mean_image(ImageDetectNew, &ImageReduceMean, BlackMaskSize, BlackMaskSize);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		if(abs(WhiteMaskSize-BlackMaskSize)>=1)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
			expand_domain_gray (ImageReduced, &ExpandedImage, 25);
			reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
			mean_image(ImageDetectNew, &ImageReduceMean, WhiteMaskSize, WhiteMaskSize);
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
STDMETHODIMP CRubberMatAlgo::ScrapDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1055$\
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

//
STDMETHODIMP CRubberMatAlgo::UniqueCircleLocationAlgo(VARIANT* rgnId, VARIANT* pCircleRadius,VARIANT* pMinScore)
{
	int id = (int)rgnId->fltVal;
	//float RoiWidth = (float)pRoiWidth->fltVal;
	//float RadiusExt =(float)pRadius->fltVal;
	//float RadiusInner = RadiusExt - RoiWidth;
	//
	float CircleRadius = (float)pCircleRadius->fltVal;
	float MinScore    = (float)pMinScore->fltVal;

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG
	try
	{
#endif

		Hobject ImageReduced,Circle;
		Hobject Image=m_crtImg;
		//图像增强
		/*Hobject DiffOfGauss,ImageConverted;
		diff_of_gauss(Image,&DiffOfGauss,5,10);
		convert_image_type(DiffOfGauss,&ImageConverted,"byte");
		abs_diff_image(Image,ImageConverted,&Image,1);*/

		gen_circle(&Circle,fCenterY,fCenterX,m_innerCircleRadius-30);
		reduce_domain(Image, Circle, &ImageReduced);

		if (m_uniqueCircleModelFirst)
		{
			Hobject ContCircle,ConstImage,SyntheticModelImage;
			HTuple SynthImageWidth;
			SynthImageWidth = (2*CircleRadius)+10;
			m_CircleRadiusOld=CircleRadius;
			gen_circle_contour_xld(&ContCircle, SynthImageWidth/2, SynthImageWidth/2,CircleRadius, 0, 6.28318, "positive", 1);
			gen_image_const(&ConstImage, "byte", SynthImageWidth, SynthImageWidth);
			paint_xld(ContCircle, ConstImage, &SyntheticModelImage, 128);
			invert_image(SyntheticModelImage, &SyntheticModelImage);

			HTuple   UniqueCircleModelID;
			Hobject  UniqueModelContour;
			create_scaled_shape_model(SyntheticModelImage, 0, 0, 0,"auto", 0.9, 1.1, "auto", "none", "use_polarity", 40, "auto", &UniqueCircleModelID);
			get_shape_model_contours(&UniqueModelContour, UniqueCircleModelID, 1);

			m_uniqueModelContour=UniqueModelContour;
			m_uniqueCircleModelID=UniqueCircleModelID;
			m_uniqueCircleModelFirst = false;
		}

		if(m_CircleRadiusOld != CircleRadius)
		{
			clear_shape_model(m_uniqueCircleModelID);
			Hobject ContCircle,ConstImage,SyntheticModelImage;
			HTuple SynthImageWidth;
			SynthImageWidth = (2*CircleRadius)+10;
			m_CircleRadiusOld=CircleRadius;
			gen_circle_contour_xld(&ContCircle, SynthImageWidth/2, SynthImageWidth/2,CircleRadius, 0, 6.28318, "positive", 1);
			gen_image_const(&ConstImage, "byte", SynthImageWidth, SynthImageWidth);
			paint_xld(ContCircle, ConstImage, &SyntheticModelImage, 128);
			invert_image(SyntheticModelImage, &SyntheticModelImage);

			HTuple   UniqueCircleModelID;
			Hobject  UniqueModelContour;
			create_scaled_shape_model(SyntheticModelImage, 0, 0, 0,"auto", 0.9, 1.1, "auto", "none", "use_polarity",  40, "auto", &UniqueCircleModelID);
			get_shape_model_contours(&UniqueModelContour, UniqueCircleModelID, 1);

			m_uniqueModelContour=UniqueModelContour;
			m_uniqueCircleModelID=UniqueCircleModelID;
			m_uniqueCircleModelFirst = false;
		}

		HTuple UniqueCircleRow,UniqueCircleCol,UniqueCircleAgnle,UniqueCircleScale,UniqueCircleScore;
		//
		/*if(RadiusInner<5)
		{
		RadiusInner=5;
		}
		if(RadiusExt-RadiusInner<2)
		{
		RadiusExt=RadiusInner+2;
		}
		Hobject Circle2_EXT,Circle1_Inner,ModelSearchRgn,ModelSearchImg;
		gen_circle(&Circle2_EXT, fCenterY, fCenterX, RadiusExt);
		gen_circle(&Circle1_Inner, fCenterY, fCenterX, RadiusInner);
		difference(Circle2_EXT,Circle1_Inner,&ModelSearchRgn);
		reduce_domain(Image,ModelSearchRgn,&ModelSearchImg);*/

		find_scaled_shape_model(ImageReduced, m_uniqueCircleModelID, 0, 0,0.9, 1.1, MinScore, 1, 0.5, "least_squares", (HTuple(3).Append(-2)), 0.7, &UniqueCircleRow,&UniqueCircleCol, &UniqueCircleAgnle, &UniqueCircleScale, &UniqueCircleScore);
		m_circlesNumber=UniqueCircleScore.Num();
		m_phiValue=0;
		if(UniqueCircleScore.Num()==1)
		{
			HTuple HomMat2D,HomMat2DScale;
			Hobject ContourAffineTrans;
			vector_angle_to_rigid(0, 0, 0, UniqueCircleRow, UniqueCircleCol,UniqueCircleAgnle, &HomMat2D);
			hom_mat2d_scale(HomMat2D,UniqueCircleScale, UniqueCircleScale, UniqueCircleRow,UniqueCircleCol, &HomMat2DScale);
			affine_trans_contour_xld(m_uniqueModelContour, &ContourAffineTrans, HomMat2DScale);

			//Angle [0,2Pi)
			Hobject CenterCircle,ModelCenterCircle,RegionUnion;
			HTuple  UniqueCirclePhi;
			gen_circle(&CenterCircle, fCenterY,fCenterX, 20);
			gen_circle(&ModelCenterCircle, UniqueCircleRow, UniqueCircleCol, 5);

			union2(CenterCircle, ModelCenterCircle, &RegionUnion);
			orientation_region(RegionUnion, &UniqueCirclePhi);

			if(UniqueCirclePhi<0)
			{
				UniqueCirclePhi=2*PI+UniqueCirclePhi;
			}

			//
			Hobject Region;
			HTuple  Row,Column,Radius1;
			gen_region_contour_xld(ContourAffineTrans, &Region, "filled");
			m_UniqueCirclesRegion=Region;
			m_InnerCirclesRegion=Region;

			smallest_circle(Region, &Row, &Column, &Radius1);
			Hobject Circle, Circle1,RegionDifference;
			gen_circle(&Circle, Row, Column, Radius1);
			gen_circle(&Circle1, Row, Column, CircleRadius);

			if(Circle.Id() != H_EMPTY_REGION) 
			{
				if(Radius1 >= CircleRadius)
				{
					if(Radius1-CircleRadius < 1)
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
					if(CircleRadius - Radius1 < 1)
					{
						gen_circle(&Circle, Row, Column, Radius1-1);
					}
					else
					{
						gen_circle(&Circle, Row, Column, Radius1);
					}
					difference (Circle1, Circle, &RegionDifference);
				}
			}

			m_modelRow=UniqueCircleRow;
			m_modelCol=UniqueCircleCol;
			m_modelAngle=UniqueCirclePhi;

			m_phiValue=UniqueCirclePhi;
			//Hobject RegionDisp;
			//concat_obj(RegionDifference,ModelSearchRgn,&RegionDisp);
			m_vRgn[id]=RegionDifference;
		}
		else
		{
			Hobject newRgn;
			m_vRgn[id] = newRgn;
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
STDMETHODIMP CRubberMatAlgo::UniqueCircleLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 374;LP;1;200;1;100$\
					 377;FP;0.5;1.0;0.05;0.85");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::ThreeCirclesLocationAlgo1(VARIANT* rgnId,VARIANT*pSelect, VARIANT* pCircleRadius,VARIANT* pMinScore,VARIANT*pRowUpLeft,VARIANT*pLength1,VARIANT*pLength2,VARIANT*pThredValue)
{
	//三个圆半径相同时，通过提取字符边缘，确定其中唯一圆的位置，计算角度
	int id = (int)rgnId->fltVal;
	int Select=(int)pSelect->fltVal;
	float CircleRadius = (float)pCircleRadius->fltVal;
	int MinScore    = (int)pMinScore->fltVal;
	//
	int RowUpLeft=(int)pRowUpLeft->fltVal;
	int Length1=(int)pLength1->fltVal;
	int Length2=(int)pLength2->fltVal;
	int ThredValue    = (int)pThredValue->fltVal;
	//
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG
	try
	{
#endif

		Hobject ImageReduced,Circle;
		Hobject Image=m_crtImg;
		//
		//gen_circle(&Circle,fCenterY,fCenterX,m_innerCircleRadius);
		//reduce_domain(Image, Circle, &ImageReduced);
		//
		Hobject ContCircle,ModelImage,ImagerInvert,ModelImages,ModelRegions,ModelContour;
		HTuple  ModelImageHeight,ModelID;

		if(m_smallCircleModelFirst)
		{
			ModelImageHeight=CircleRadius*2+10;
			m_CircleRadiusOld=CircleRadius;
			gen_circle_contour_xld(&ContCircle,ModelImageHeight/2,ModelImageHeight/2,CircleRadius,0,6.28318,"positive",1);
			gen_image_const(&ModelImage,"byte",ModelImageHeight,ModelImageHeight);
			paint_xld(ContCircle,ModelImage,&ModelImage,128);
			invert_image(ModelImage,&ImagerInvert);
			create_scaled_shape_model(ImagerInvert,"auto",0,0,0,0.8,1.2,"auto","none","use_polarity",40,"auto",&ModelID);
			get_shape_model_contours(&ModelContour,ModelID,1);
			m_modelID=ModelID;
			m_smallCircleContour=ModelContour;
			m_smallCircleModelFirst=false;
		}
		if(m_CircleRadiusOld != CircleRadius)
		{
			clear_shape_model(m_modelID);

			ModelImageHeight=CircleRadius*2+10;
			m_CircleRadiusOld=CircleRadius;
			gen_circle_contour_xld(&ContCircle,ModelImageHeight/2,ModelImageHeight/2,CircleRadius,0,6.28318,"positive",1);
			gen_image_const(&ModelImage,"byte",ModelImageHeight,ModelImageHeight);
			paint_xld(ContCircle,ModelImage,&ModelImage,128);
			invert_image(ModelImage,&ImagerInvert);
			create_scaled_shape_model(ImagerInvert,"auto",0,0,0,0.8,1.2,"auto","none","use_polarity",40,"auto",&ModelID);
			get_shape_model_contours(&ModelContour,ModelID,1);
			m_modelID=ModelID;
			m_smallCircleContour=ModelContour;
			m_smallCircleModelFirst=false;
		}

		Hobject ModelSearchRgn,ModelSearchImg;
		HTuple  ModelRow,ModelColumn,ModelAngle,ModelScale,ModelScore;
		gen_circle(&ModelSearchRgn,fCenterY,fCenterX,m_innerCircleRadius-40);

		reduce_domain(Image,ModelSearchRgn,&ModelSearchImg);
		find_scaled_shape_model(ModelSearchImg,m_modelID,0,0,0.8,1.2,MinScore,3,0.5,"least_squares",(HTuple(2).Append(-2)),0.7,&ModelRow,&ModelColumn,&ModelAngle,&ModelScale,&ModelScore);
		m_circlesNumber=ModelScore.Num();

		Hobject  ContoursConcat,ContourAffineTrans,Circles,CirclesDilation,ThreeCirclesRegion;
		HTuple   HomMat2DIdentity,HomMat2DRotate,HomMat2DTranslate,HomMat2DScale;
		if ((ModelScore.Num())==3)
		{
			gen_empty_obj(&ContoursConcat);
			for (int i=0;i<=(ModelScore.Num()-1);i=i+1)
			{
				hom_mat2d_identity(&HomMat2DIdentity);
				hom_mat2d_rotate (HomMat2DIdentity, ModelAngle.Select(i), 0, 0, &HomMat2DRotate);
				hom_mat2d_translate (HomMat2DRotate, ModelRow.Select(i), ModelColumn.Select(i), &HomMat2DTranslate);
				hom_mat2d_scale(HomMat2DTranslate,ModelScale.Select(i),ModelScale.Select(i),ModelRow.Select(i),ModelColumn.Select(i),&HomMat2DScale);
				affine_trans_contour_xld(m_smallCircleContour,&ContourAffineTrans,HomMat2DScale);
				concat_obj(ContourAffineTrans,ContoursConcat,&ContoursConcat);
			}
			gen_region_contour_xld(ContoursConcat, &ThreeCirclesRegion, "filled");
			//
			HTuple  ModelRowSorted,Column2Indice,Column1Indice,NumberFirst,NumberSecond,Column12,MaxValue,MinValue;
			HTuple  PhiValue;
			tuple_sort(ModelRow,&ModelRowSorted);
			tuple_find(ModelRow,ModelRowSorted[1],&NumberFirst);
			tuple_find(ModelRow,ModelRowSorted[2],&NumberSecond);

			line_orientation(ModelRowSorted[1].D(),ModelColumn[NumberFirst.Select(0)].D(),ModelRowSorted[2].D(),ModelColumn[NumberSecond.Select(0)].D(),&PhiValue);
			HTuple  HomMat2D1,HomMat2D2,HomMat2D3;
			Hobject Rect,Rect1,Rect2,Rect3,ObjectsConcat;
			gen_rectangle2(&Rect, fCenterY+RowUpLeft, fCenterX, 0, Length1, Length2);

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue, &HomMat2D1);
			affine_trans_region(Rect, &Rect1, HomMat2D1, "constant");

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue+(HTuple(120).Rad()),&HomMat2D2);
			affine_trans_region(Rect, &Rect2, HomMat2D2, "constant");

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue+(HTuple(240).Rad()),&HomMat2D3);
			affine_trans_region(Rect, &Rect3, HomMat2D3, "constant");

			concat_obj(Rect1, Rect2, &ObjectsConcat);
			concat_obj(ObjectsConcat, Rect3, &ObjectsConcat);
			sort_region(ObjectsConcat, &ObjectsConcat, "first_point", "true", "row");
			//
			/*if(m_bDebugImg)
			{
			concat_obj (m_vRgn[id], ObjectsConcat, &m_vRgn[id]);
			}*/
			HTuple DistAll,AreaAll,RectArea, RectRow, RectColumn,SelectedArea, SelectedRow, SelectedColumn,Distance;
			Hobject ImageEmphasize,ObjectSelected,ImageReduced1,ExpandedImage,ImageMedian,ModelImages1,ModelRegions,EmptyObject,RegionUnion1,RegionErosion,RegionIntersection;
			gen_empty_region(&EmptyObject);
			DistAll = HTuple();
			AreaAll = HTuple();
			for (int i=1; i<=3; i+=1)
			{
				select_obj(ObjectsConcat, &ObjectSelected, i);
				area_center(ObjectSelected, &RectArea, &RectRow, &RectColumn);
				reduce_domain(Image, ObjectSelected, &ImageReduced1);
				emphasize(ImageReduced1, &ImageEmphasize, 11, 11, 1);
				median_image(ImageEmphasize, &ImageMedian, "circle", 1.5, "mirrored");
				expand_domain_gray(ImageMedian, &ExpandedImage, 2);
				reduce_domain(ExpandedImage, ObjectSelected, &ImageReduced1);
				inspect_shape_model(ImageReduced1, &ModelImages1, &ModelRegions, 1,((HTuple(ThredValue).Append(ThredValue*2)).Append(5)));
				//添加判断条件
				erosion_circle(ObjectSelected, &RegionErosion, 3.5);
				intersection(RegionErosion, ModelRegions, &RegionIntersection);
				area_center(RegionIntersection, &SelectedArea, &SelectedRow, &SelectedColumn);
				if (Select==1)//无字符，基于面积判断
				{
					AreaAll = AreaAll.Concat(SelectedArea);
					union1(RegionIntersection, &RegionUnion1);
					concat_obj(EmptyObject, RegionUnion1, &EmptyObject);
				}
				else if(Select==2)//有字符，基于距离判断
				{
					distance_pp(RectRow, RectColumn, SelectedRow, SelectedColumn, &Distance);
					DistAll = DistAll.Concat(Distance);
					union1(RegionIntersection, &RegionUnion1);
					concat_obj(EmptyObject, RegionUnion1, &EmptyObject);
				}	
			}
			Hobject CharRgn,RectRgn,ContourSelected;
			if (Select==1)
			{
				select_obj(EmptyObject, &CharRgn, (AreaAll.Find(AreaAll.Min()))+1);
				select_obj(ObjectsConcat, &RectRgn, (AreaAll.Find(AreaAll.Min()))+1);
			}
			else if (Select==2)
			{
				select_obj(EmptyObject, &CharRgn, (DistAll.Find(DistAll.Min()))+1);
				select_obj(ObjectsConcat, &RectRgn, (DistAll.Find(DistAll.Min()))+1);
			}
			if(m_bDebugImg)
			{
				concat_obj (m_vRgn[id], EmptyObject, &m_vRgn[id]);
				concat_obj (m_vRgn[id], RectRgn, &m_vRgn[id]);
			}
			area_center(RectRgn, &RectArea, &RectRow, &RectColumn);
			HTuple DistP,DistP1,Num,ContArea, ContRow, ContColumn, PointOrder,DistanceP1,DistanceP;
			DistP = HTuple();
			DistP1 = HTuple();
			for (int i=0; i<=3-1; i++)
			{
				distance_pp(ModelRow.Select(i), ModelColumn.Select(i), RectRow[0], RectColumn[0], &DistanceP);
				DistP = DistP.Concat(DistanceP);
			}
			for (int i=1; i<=3; i++)
			{
				select_obj(ContoursConcat, &ContourSelected, i);
				area_center_xld(ContourSelected, &ContArea, &ContRow, &ContColumn, &PointOrder);
				distance_pp(ContRow, ContColumn, RectRow[0], RectColumn[0], &DistanceP1);
				DistP1 = DistP1.Concat(DistanceP1);
			}
			HTuple AngleArea, AngleRow, AngleColumn;
			select_obj(ContoursConcat, &ContourSelected, (DistP1.Find(DistP1.Max()))+1);
			area_center_xld(ContourSelected, &AngleArea, &AngleRow, &AngleColumn, &PointOrder);

			//Angle [0,2Pi)
			Hobject CenterCircle,ModelCenterCircle,RegionUnion,ContRegion;
			HTuple  UniqueCirclePhi;
			gen_circle(&CenterCircle, fCenterY,fCenterX, 20);
			gen_circle(&ModelCenterCircle, AngleRow, AngleColumn, 5);

			union2(CenterCircle, ModelCenterCircle, &RegionUnion);
			orientation_region(RegionUnion, &UniqueCirclePhi);

			if(UniqueCirclePhi<0)
			{
				UniqueCirclePhi=2*PI+UniqueCirclePhi;
			}
			m_modelAngle=UniqueCirclePhi;
			m_phiValue=UniqueCirclePhi;
			gen_region_contour_xld(ContourSelected,&ContRegion,"filled");
			//m_vRgn[id]=ContRegion;
			Hobject Line;
			gen_region_line(&Line,fCenterY,fCenterX,AngleRow, AngleColumn);
			concat_obj(m_vRgn[id],Line,&m_vRgn[id]);
			concat_obj(m_vRgn[id],ContRegion,&m_vRgn[id]);
			
			//
			union1(ThreeCirclesRegion,&ThreeCirclesRegion);
			m_threeCirclesRegion=ThreeCirclesRegion;
			m_InnerCirclesRegion=ThreeCirclesRegion;
		}
		else
		{
			Hobject newRgn;
			m_vRgn[id] = newRgn;
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
STDMETHODIMP CRubberMatAlgo::ThreeCirclesLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 1293;LP;1;2;1;1$\
					 374;LP;20;50;1;30$\
					 377;FP;0.5;1.0;0.05;0.5$\
					 507;FP;-200;200;1;90$\
					 500;LP;1.5;2000;1;50$\
					 499;LP;-60;360.0;1;30$\
					 1209;LP;2;80;1;15");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::ThreeCirclesLocationAlgo2(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT*pSelect, VARIANT* pCircleRadius,VARIANT* pMinScore,VARIANT*pRowUpLeft,VARIANT*pLength1,VARIANT*pLength2,VARIANT*pThredValue)
{
	//三个圆半径相同时，通过提取字符边缘，确定其中唯一圆的位置，计算角度
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusExt =(float)pRadius->fltVal;
	float RadiusInner = RadiusExt - RoiWidth;

	//
	int Select=(int)pSelect->fltVal;
	float CircleRadius = (float)pCircleRadius->fltVal;
	int MinScore    = (int)pMinScore->fltVal;
	//
	int RowUpLeft=(int)pRowUpLeft->fltVal;
	int Length1=(int)pLength1->fltVal;
	int Length2=(int)pLength2->fltVal;
	int ThredValue    = (int)pThredValue->fltVal;
	//
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG
	try
	{
#endif
		HTuple   Seconds1,Seconds2;	

		Hobject ImageReduced,Circle;
		Hobject Image=m_crtImg;
		//
		gen_circle(&Circle,fCenterY,fCenterX,m_innerCircleRadius);
		reduce_domain(Image, Circle, &ImageReduced);
		//
		Hobject ContCircle,ModelImage,ImagerInvert,ModelImages,ModelRegions,ModelContour;
		HTuple  ModelImageHeight,ModelID;

		if(m_smallCircleModelFirst)
		{
			ModelImageHeight=CircleRadius*2+10;
			m_CircleRadiusOld=CircleRadius;
			gen_circle_contour_xld(&ContCircle,ModelImageHeight/2,ModelImageHeight/2,CircleRadius,0,6.28318,"positive",1);
			gen_image_const(&ModelImage,"byte",ModelImageHeight,ModelImageHeight);
			paint_xld(ContCircle,ModelImage,&ModelImage,128);
			invert_image(ModelImage,&ImagerInvert);
			create_scaled_shape_model(ImagerInvert,"auto",0,0,0,0.8,1.2,"auto","none","use_polarity",40,"auto",&ModelID);
			get_shape_model_contours(&ModelContour,ModelID,1);
			m_modelID=ModelID;
			m_smallCircleContour=ModelContour;
			m_smallCircleModelFirst=false;
		}
		if(m_CircleRadiusOld != CircleRadius)
		{
			clear_shape_model(m_modelID);

			ModelImageHeight=CircleRadius*2+10;
			m_CircleRadiusOld=CircleRadius;
			gen_circle_contour_xld(&ContCircle,ModelImageHeight/2,ModelImageHeight/2,CircleRadius,0,6.28318,"positive",1);
			gen_image_const(&ModelImage,"byte",ModelImageHeight,ModelImageHeight);
			paint_xld(ContCircle,ModelImage,&ModelImage,128);
			invert_image(ModelImage,&ImagerInvert);
			create_scaled_shape_model(ImagerInvert,"auto",0,0,0,0.8,1.2,"auto","none","use_polarity",40,"auto",&ModelID);
			get_shape_model_contours(&ModelContour,ModelID,1);
			m_modelID=ModelID;
			m_smallCircleContour=ModelContour;
			m_smallCircleModelFirst=false;
		}

		Hobject ModelSearchRgn,ModelSearchImg;
		HTuple  ModelRow,ModelColumn,ModelAngle,ModelScale,ModelScore;
		//
		if(RadiusInner<5)
		{
			RadiusInner=5;
		}
		if(RadiusExt-RadiusInner<2)
		{
			RadiusExt=RadiusInner+2;
		}
		Hobject Circle2_EXT,Circle1_Inner;
		gen_circle(&Circle2_EXT, fCenterY, fCenterX, RadiusExt);
		gen_circle(&Circle1_Inner, fCenterY, fCenterX, RadiusInner);
		difference(Circle2_EXT,Circle1_Inner,&ModelSearchRgn);
		//
		reduce_domain(Image,ModelSearchRgn,&ModelSearchImg);
		find_scaled_shape_model(ModelSearchImg,m_modelID,0,0,0.8,1.2,MinScore,3,0.5,"least_squares",(HTuple(2).Append(-2)),0.7,&ModelRow,&ModelColumn,&ModelAngle,&ModelScale,&ModelScore);
		m_circlesNumber=ModelScore.Num();
		Hobject  ContoursConcat,ContourAffineTrans,Circles,CirclesDilation,ThreeCirclesRegion;
		HTuple   HomMat2DIdentity,HomMat2DRotate,HomMat2DTranslate,HomMat2DScale;
		if ((ModelScore.Num())==3)
		{
			gen_empty_obj(&ContoursConcat);
			for (int i=0;i<=(ModelScore.Num()-1);i=i+1)
			{
				hom_mat2d_identity(&HomMat2DIdentity);
				hom_mat2d_rotate (HomMat2DIdentity, ModelAngle.Select(i), 0, 0, &HomMat2DRotate);
				hom_mat2d_translate (HomMat2DRotate, ModelRow.Select(i), ModelColumn.Select(i), &HomMat2DTranslate);
				hom_mat2d_scale(HomMat2DTranslate,ModelScale.Select(i),ModelScale.Select(i),ModelRow.Select(i),ModelColumn.Select(i),&HomMat2DScale);
				affine_trans_contour_xld(m_smallCircleContour,&ContourAffineTrans,HomMat2DScale);
				concat_obj(ContourAffineTrans,ContoursConcat,&ContoursConcat);
			}
			gen_region_contour_xld(ContoursConcat, &ThreeCirclesRegion, "filled");
			//
			HTuple  ModelRowSorted,Column2Indice,Column1Indice,NumberFirst,NumberSecond,Column12,MaxValue,MinValue;
			HTuple  PhiValue;
			tuple_sort(ModelRow,&ModelRowSorted);
			tuple_find(ModelRow,ModelRowSorted[1],&NumberFirst);
			tuple_find(ModelRow,ModelRowSorted[2],&NumberSecond);

			line_orientation(ModelRowSorted[1].D(),ModelColumn[NumberFirst.Select(0)].D(),ModelRowSorted[2].D(),ModelColumn[NumberSecond.Select(0)].D(),&PhiValue);
			HTuple  HomMat2D1,HomMat2D2,HomMat2D3;
			Hobject Rect,Rect1,Rect2,Rect3,ObjectsConcat;
			gen_rectangle2(&Rect, fCenterY+RowUpLeft, fCenterX, 0, Length1, Length2);

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue, &HomMat2D1);
			affine_trans_region(Rect, &Rect1, HomMat2D1, "constant");

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue+(HTuple(120).Rad()),&HomMat2D2);
			affine_trans_region(Rect, &Rect2, HomMat2D2, "constant");

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue+(HTuple(240).Rad()),&HomMat2D3);
			affine_trans_region(Rect, &Rect3, HomMat2D3, "constant");

			concat_obj(Rect1, Rect2, &ObjectsConcat);
			concat_obj(ObjectsConcat, Rect3, &ObjectsConcat);
			sort_region(ObjectsConcat, &ObjectsConcat, "first_point", "true", "row");
			//
			/*if(m_bDebugImg)
			{
			concat_obj (m_vRgn[id], ObjectsConcat, &m_vRgn[id]);
			}*/
			HTuple DistAll,AreaAll,RectArea, RectRow, RectColumn,SelectedArea, SelectedRow, SelectedColumn,Distance;
			Hobject ImageEmphasize,ObjectSelected,ImageReduced1,ExpandedImage,ImageMedian,ModelImages1,ModelRegions,EmptyObject,RegionUnion1,RegionErosion,RegionIntersection;
			gen_empty_region(&EmptyObject);
			DistAll = HTuple();
			AreaAll = HTuple();
			for (int i=1; i<=3; i+=1)
			{
				select_obj(ObjectsConcat, &ObjectSelected, i);
				area_center(ObjectSelected, &RectArea, &RectRow, &RectColumn);
				reduce_domain(Image, ObjectSelected, &ImageReduced1);
				emphasize(ImageReduced1, &ImageEmphasize, 11, 11, 1);
				median_image(ImageEmphasize, &ImageMedian, "circle", 1.5, "mirrored");
				expand_domain_gray(ImageMedian, &ExpandedImage, 2);
				reduce_domain(ExpandedImage, ObjectSelected, &ImageReduced1);
				inspect_shape_model(ImageReduced1, &ModelImages1, &ModelRegions, 1,((HTuple(ThredValue).Append(ThredValue*2)).Append(5)));
				//添加判断条件
				erosion_circle(ObjectSelected, &RegionErosion, 3.5);
				intersection(RegionErosion, ModelRegions, &RegionIntersection);
				area_center(RegionIntersection, &SelectedArea, &SelectedRow, &SelectedColumn);
				if (Select==1)//无字符，基于面积判断
				{
					AreaAll = AreaAll.Concat(SelectedArea);
					union1(RegionIntersection, &RegionUnion1);
					concat_obj(EmptyObject, RegionUnion1, &EmptyObject);
				}
				else if(Select==2)//有字符，基于距离判断
				{
					distance_pp(RectRow, RectColumn, SelectedRow, SelectedColumn, &Distance);
					DistAll = DistAll.Concat(Distance);
					union1(RegionIntersection, &RegionUnion1);
					concat_obj(EmptyObject, RegionUnion1, &EmptyObject);
				}	
			}
			Hobject CharRgn,RectRgn,ContourSelected;
			if (Select==1)
			{
				select_obj(EmptyObject, &CharRgn, (AreaAll.Find(AreaAll.Min()))+1);
				select_obj(ObjectsConcat, &RectRgn, (AreaAll.Find(AreaAll.Min()))+1);
			}
			else if (Select==2)
			{
				select_obj(EmptyObject, &CharRgn, (DistAll.Find(DistAll.Min()))+1);
				select_obj(ObjectsConcat, &RectRgn, (DistAll.Find(DistAll.Min()))+1);
			}
			if(m_bDebugImg)
			{
				concat_obj (m_vRgn[id], EmptyObject, &m_vRgn[id]);
				concat_obj (m_vRgn[id], RectRgn, &m_vRgn[id]);
			}
			area_center(RectRgn, &RectArea, &RectRow, &RectColumn);
			HTuple DistP,DistP1,Num,ContArea, ContRow, ContColumn, PointOrder,DistanceP1,DistanceP;
			DistP = HTuple();
			DistP1 = HTuple();
			for (int i=0; i<=3-1; i++)
			{
				distance_pp(ModelRow.Select(i), ModelColumn.Select(i), RectRow[0], RectColumn[0], &DistanceP);
				DistP = DistP.Concat(DistanceP);
			}
			for (int i=1; i<=3; i++)
			{
				select_obj(ContoursConcat, &ContourSelected, i);
				area_center_xld(ContourSelected, &ContArea, &ContRow, &ContColumn, &PointOrder);
				distance_pp(ContRow, ContColumn, RectRow[0], RectColumn[0], &DistanceP1);
				DistP1 = DistP1.Concat(DistanceP1);
			}
			HTuple AngleArea, AngleRow, AngleColumn;
			select_obj(ContoursConcat, &ContourSelected, (DistP1.Find(DistP1.Max()))+1);
			area_center_xld(ContourSelected, &AngleArea, &AngleRow, &AngleColumn, &PointOrder);

			//Angle [0,2Pi)
			Hobject CenterCircle,ModelCenterCircle,RegionUnion,ContRegion;
			HTuple  UniqueCirclePhi;
			gen_circle(&CenterCircle, fCenterY,fCenterX, 20);
			gen_circle(&ModelCenterCircle, AngleRow, AngleColumn, 5);

			union2(CenterCircle, ModelCenterCircle, &RegionUnion);
			orientation_region(RegionUnion, &UniqueCirclePhi);

			if(UniqueCirclePhi<0)
			{
				UniqueCirclePhi=2*PI+UniqueCirclePhi;
			}
			m_modelAngle=UniqueCirclePhi;
			m_phiValue=UniqueCirclePhi;
			gen_region_contour_xld(ContourSelected,&ContRegion,"filled");
			//m_vRgn[id]=ContRegion;
			Hobject Line;
			gen_region_line(&Line,fCenterY,fCenterX,AngleRow, AngleColumn);
			concat_obj(m_vRgn[id],Line,&m_vRgn[id]);
			concat_obj(m_vRgn[id],ContRegion,&m_vRgn[id]);
			
			//
			union1(ThreeCirclesRegion,&ThreeCirclesRegion);
			m_threeCirclesRegion=ThreeCirclesRegion;
			m_InnerCirclesRegion=ThreeCirclesRegion;
		}
		else
		{
			m_vRgn[id] = ModelSearchRgn;
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
STDMETHODIMP CRubberMatAlgo::ThreeCirclesLocationAlgo2Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 1293;LP;1;2;1;1$\
					 374;LP;20;50;1;30$\
					 377;FP;0.5;1.0;0.05;0.5$\
					 507;FP;-200;200;1;90$\
					 500;LP;1.5;2000;1;50$\
					 499;LP;-60;360.0;1;30$\
					 1209;LP;2;80;1;15");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::ThreeCirclesLocationAlgo(VARIANT* rgnId, VARIANT* pCircleRadius,VARIANT* pMinScore)
{
	//定位内部三个圆的位置

	int id = (int)rgnId->fltVal;
	float CircleRadius = (float)pCircleRadius->fltVal;
	int MinScore    = (int)pMinScore->fltVal;

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG
	try
	{
#endif

		Hobject ImageReduced,Circle;
		Hobject Image=m_crtImg;
		//

		gen_circle(&Circle,fCenterY,fCenterX,m_innerCircleRadius);
		reduce_domain(Image, Circle, &ImageReduced);

		//
		Hobject ContCircle,ModelImage,ImagerInvert,ModelImages,ModelRegions,ModelContour;
		HTuple  ModelImageHeight,ModelID;

		if(m_smallCircleModelFirst)
		{
			ModelImageHeight=CircleRadius*2+10;
			m_CircleRadiusOld=CircleRadius;
			gen_circle_contour_xld(&ContCircle,ModelImageHeight/2,ModelImageHeight/2,CircleRadius,0,6.28318,"positive",1);
			gen_image_const(&ModelImage,"byte",ModelImageHeight,ModelImageHeight);
			paint_xld(ContCircle,ModelImage,&ModelImage,128);
			invert_image(ModelImage,&ImagerInvert);
			create_scaled_shape_model(ImagerInvert,"auto",0,0,0,0.8,1.2,"auto","none","use_polarity",40,"auto",&ModelID);
			get_shape_model_contours(&ModelContour,ModelID,1);
			m_modelID=ModelID;
			m_smallCircleContour=ModelContour;
			m_smallCircleModelFirst=false;
		}
		if(m_CircleRadiusOld != CircleRadius)
		{
			clear_shape_model(m_modelID);

			ModelImageHeight=CircleRadius*2+10;
			m_CircleRadiusOld=CircleRadius;
			gen_circle_contour_xld(&ContCircle,ModelImageHeight/2,ModelImageHeight/2,CircleRadius,0,6.28318,"positive",1);
			gen_image_const(&ModelImage,"byte",ModelImageHeight,ModelImageHeight);
			paint_xld(ContCircle,ModelImage,&ModelImage,128);
			invert_image(ModelImage,&ImagerInvert);
			create_scaled_shape_model(ImagerInvert,"auto",0,0,0,0.8,1.2,"auto","none","use_polarity",40,"auto",&ModelID);
			get_shape_model_contours(&ModelContour,ModelID,1);
			m_modelID=ModelID;
			m_smallCircleContour=ModelContour;
			m_smallCircleModelFirst=false;
		}

		Hobject ModelSearchRgn,ModelSearchImg;
		HTuple  ModelRow,ModelColumn,ModelAngle,ModelScale,ModelScore;
		gen_circle(&ModelSearchRgn,fCenterY,fCenterX,m_innerCircleRadius-40);

		reduce_domain(Image,ModelSearchRgn,&ModelSearchImg);
		find_scaled_shape_model(ModelSearchImg,m_modelID,0,0,0.8,1.2,MinScore,3,0.5,"least_squares",(HTuple(2).Append(-2)),0.7,&ModelRow,&ModelColumn,&ModelAngle,&ModelScale,&ModelScore);
		m_circlesNumber=ModelScore.Num();

		Hobject  ContoursConcat,ContourAffineTrans,Circles,CirclesDilation,ThreeCirclesRegion;
		HTuple   HomMat2DIdentity,HomMat2DRotate,HomMat2DTranslate,HomMat2DScale;
		if ((ModelScore.Num())!=3 &&(ModelScore.Num())!=0)
		{
			gen_empty_obj(&ContoursConcat);
			for (int i=0;i<=(ModelScore.Num()-1);i=i+1)
			{
				hom_mat2d_identity(&HomMat2DIdentity);
				hom_mat2d_rotate (HomMat2DIdentity, ModelAngle.Select(i), 0, 0, &HomMat2DRotate);
				hom_mat2d_translate (HomMat2DRotate, ModelRow.Select(i), ModelColumn.Select(i), &HomMat2DTranslate);
				hom_mat2d_scale(HomMat2DTranslate,ModelScale.Select(i),ModelScale.Select(i),ModelRow.Select(i),ModelColumn.Select(i),&HomMat2DScale);
				affine_trans_contour_xld(m_smallCircleContour,&ContourAffineTrans,HomMat2DScale);
				concat_obj(ContourAffineTrans,ContoursConcat,&ContoursConcat);
			}

			Hobject ThreeCirclesRegion,SelectCircle,Circle,Circle1;
			HTuple  Row,Column,Radius1;
			gen_region_contour_xld(ContoursConcat, &ThreeCirclesRegion, "filled");

			Hobject RegionDifference,RegionDiffConcat;
			gen_empty_obj(&RegionDiffConcat);

			for (int i=1;i<=(ModelScore.Num());i=i+1)
			{
				select_obj(ThreeCirclesRegion,&SelectCircle,i);

				smallest_circle(SelectCircle, &Row, &Column, &Radius1);
				gen_circle(&Circle, Row, Column, Radius1);
				gen_circle(&Circle1, Row, Column, CircleRadius);

				//
				if(Radius1 >= CircleRadius)
				{
					if(Radius1-CircleRadius < 1)
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
					if(CircleRadius - Radius1 < 1)
					{
						gen_circle(&Circle, Row, Column, Radius1-1);
					}
					else
					{
						gen_circle(&Circle, Row, Column, Radius1);
					}
					difference (Circle1, Circle, &RegionDifference);
				}

				concat_obj(RegionDiffConcat,RegionDifference,&RegionDiffConcat);
			}
			Hobject  RegionDiffUnion;
			union1(RegionDiffConcat,&RegionDiffUnion);

			m_phiValue=0;
			m_circleNumber=ModelScore.Num();

			union1(ThreeCirclesRegion,&ThreeCirclesRegion);
			m_threeCirclesRegion=ThreeCirclesRegion;

			m_vRgn[id] = RegionDiffUnion;
			return  S_FALSE;

		}
		else if(ModelScore.Num()==0)
		{
			m_circleNumber=0;
			m_vRgn[id] = newRgn;
			return  S_FALSE;
		}
		else
		{
			HTuple  ModelRowSorted,Column2Indice,Column1Indice,NumberFirst,NumberSecond,Column12,MaxValue,MinValue;
			HTuple  PhiValue;

			tuple_sort(ModelRow,&ModelRowSorted);
			tuple_find(ModelRow,ModelRowSorted[1],&NumberFirst);
			tuple_find(ModelRow,ModelRowSorted[2],&NumberSecond);

			line_orientation(ModelRowSorted[1].D(),ModelColumn[NumberFirst.Select(0)].D(),ModelRowSorted[2].D(),ModelColumn[NumberSecond.Select(0)].D(),&PhiValue);
			m_phiValue=PhiValue;
			m_modelAngle=PhiValue;

			gen_empty_obj(&ContoursConcat);
			for (int i=0;i<=(ModelScore.Num()-1);i=i+1)
			{
				hom_mat2d_identity(&HomMat2DIdentity);
				hom_mat2d_rotate (HomMat2DIdentity, ModelAngle.Select(i), 0, 0, &HomMat2DRotate);
				hom_mat2d_translate (HomMat2DRotate, ModelRow.Select(i), ModelColumn.Select(i), &HomMat2DTranslate);
				hom_mat2d_scale(HomMat2DTranslate,ModelScale.Select(i),ModelScale.Select(i),ModelRow.Select(i),ModelColumn.Select(i),&HomMat2DScale);
				affine_trans_contour_xld(m_smallCircleContour,&ContourAffineTrans,HomMat2DScale);
				concat_obj(ContourAffineTrans,ContoursConcat,&ContoursConcat);
			}

			Hobject ThreeCirclesRegion,SelectCircle,Circle,Circle1;
			HTuple  Row,Column,Radius1;
			gen_region_contour_xld(ContoursConcat, &ThreeCirclesRegion, "filled");

			Hobject RegionDifference,RegionDiffConcat;
			gen_empty_obj(&RegionDiffConcat);

			for (int i=1;i<=(ModelScore.Num());i=i+1)
			{
				select_obj(ThreeCirclesRegion,&SelectCircle,i);

				smallest_circle(SelectCircle, &Row, &Column, &Radius1);
				gen_circle(&Circle, Row, Column, Radius1);
				gen_circle(&Circle1, Row, Column, CircleRadius);

				//
				if(Radius1 >= CircleRadius)
				{
					if(Radius1-CircleRadius < 1)
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
					if(CircleRadius - Radius1 < 1)
					{
						gen_circle(&Circle, Row, Column, Radius1-1);
					}
					else
					{
						gen_circle(&Circle, Row, Column, Radius1);
					}
					difference (Circle1, Circle, &RegionDifference);
				}

				concat_obj(RegionDiffConcat,RegionDifference,&RegionDiffConcat);
			}
			Hobject  RegionDiffUnion;
			union1(RegionDiffConcat,&RegionDiffUnion);

			m_circleNumber=ModelScore.Num();
			m_vRgn[id] = RegionDiffUnion;
			union1(ThreeCirclesRegion,&ThreeCirclesRegion);
			m_threeCirclesRegion=ThreeCirclesRegion;
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
STDMETHODIMP CRubberMatAlgo::ThreeCirclesLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 374;LP;20;50;1;30$\
					 377;FP;0.5;1.0;0.05;0.5");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::RegionWithoutCharAlgo(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pThredValue,VARIANT* pAreaMin,VARIANT*pCompactness,VARIANT*pRowUpLeft,VARIANT*pLength1,VARIANT*pLength2)
{
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	int RowUpLeft=(int)pRowUpLeft->fltVal;
	int Length1=(int)pLength1->fltVal;
	int Length2=(int)pLength2->fltVal;
	//
	int ThredValue=(int)pThredValue->fltVal;

	float AreaMin=(float)pAreaMin->fltVal;
	float LowCompactness=(float)pCompactness->fltVal;

	float LocationRadius=(float)(m_innerCircleRadius[0].I());
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

		Hobject Image,ThreeCirclesRegion;
		HTuple  PhiValue,CirclesNumber;
		Image=m_crtImg;
		//

		ThreeCirclesRegion=m_threeCirclesRegion;
		PhiValue=m_phiValue;
		CirclesNumber=m_circlesNumber;

		//
		if(0!=(CirclesNumber!=3))
		{
			m_vRgn[id]=ThreeCirclesRegion;
			return S_FALSE;
		}
		else if(0!=(CirclesNumber==0))
		{
			m_vRgn[id]=newRgn;
			return S_FALSE;
		}
		else
		{
			HTuple  HomMat2D1,HomMat2D2,HomMat2D3;
			Hobject Rect,Rect1,Rect2,Rect3,ObjectsConcat;
			gen_rectangle2(&Rect, fCenterY+RowUpLeft, fCenterX, 0, Length1, Length2);

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue, &HomMat2D1);
			affine_trans_region(Rect, &Rect1, HomMat2D1, "constant");

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue+(HTuple(120).Rad()),&HomMat2D2);
			affine_trans_region(Rect, &Rect2, HomMat2D2, "constant");

			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, PhiValue+(HTuple(240).Rad()),&HomMat2D3);
			affine_trans_region(Rect, &Rect3, HomMat2D3, "constant");

			concat_obj(Rect1, Rect2, &ObjectsConcat);
			concat_obj(ObjectsConcat, Rect3, &ObjectsConcat);

			Hobject  ThreeCirclesDilation;
			dilation_circle(ThreeCirclesRegion,&ThreeCirclesDilation,2.5);
			difference(ObjectsConcat,ThreeCirclesDilation,&ObjectsConcat);


			Hobject  ObjectSelected,ImageReduced,ModelImage,ModelRegion,RegionUnionConcat,CharRectRgn;
			HTuple  AreaAll,UnionArea,UnionRow,UnionColumn;

			gen_empty_obj(&RegionUnionConcat);
			AreaAll = HTuple();
			for (int i=1; i<=3; i+=1)
			{
				select_obj(ObjectsConcat, &ObjectSelected, i);
				reduce_domain(Image, ObjectSelected, &ImageReduced);
				inspect_shape_model(ImageReduced,&ModelImage,&ModelRegion,1,((HTuple(ThredValue).Append(ThredValue*2)).Append(10)));
				concat_obj(RegionUnionConcat,ModelRegion,&RegionUnionConcat);
				area_center(ModelRegion, &UnionArea, &UnionRow, &UnionColumn);
				AreaAll = AreaAll.Concat(UnionArea);
			}

			Hobject DiffRegion;
			difference(RegionDetect,ThreeCirclesDilation,&DiffRegion);
			if(AreaAll.Max()>AreaMin )
			{
				select_obj(ObjectsConcat, &CharRectRgn, (AreaAll.Find(AreaAll.Max()))+1);

				Hobject CharRegion;
				HTuple  CharCompactness;
				select_obj(RegionUnionConcat,&CharRegion,(AreaAll.Find(AreaAll.Max()))+1);
				compactness(CharRegion,&CharCompactness);

				if(CharCompactness>LowCompactness)
				{
					Hobject DiffRegion1;
					difference(DiffRegion,CharRectRgn,&DiffRegion1);
					m_vRgn[id] = DiffRegion1 ;	
				}
				else
				{
					m_vRgn[id]=DiffRegion;
					return S_FALSE;
				}
			}
			else
			{
				m_vRgn[id]=DiffRegion;
			}
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
STDMETHODIMP CRubberMatAlgo::RegionWithoutCharAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 693;FP;-1500;1500;0.5;10$\
					 413;FP;1;2000;0.5;208$\
					 527;LP;1;15;1;6$\
					 462;LP;1;2000;1;300$\
					 754;LP;1;2000;1;30$\
					 507;FP;-200;200;1;90$\
					 500;LP;1.5;2000;1;50$\
					 499;LP;-60;360.0;1;30"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::ThreeCirclesRegionAlgo1(VARIANT* rgnId,VARIANT*pSelection ,VARIANT* pErosionSize0,VARIANT* pErosionSize)
{
	int   id          =(int)rgnId->fltVal;
	int   Selection   =(int)pSelection->fltVal;
	float ErosionSize0 =(float)pErosionSize0->fltVal;
	float ErosionSize =(float)pErosionSize->fltVal;

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

	Hobject CirclesDilation,CharRgn;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}

		Hobject Image,ThreeCirclesRegion;
		HTuple  PhiValue,CirclesNumber;
		Image=m_crtImg;
		//
		ThreeCirclesRegion=m_threeCirclesRegion;  
		PhiValue=m_phiValue;
		CirclesNumber=m_circlesNumber;

		if(0!=(CirclesNumber!=3))
		{
			m_vRgn[id]=ThreeCirclesRegion;
			return S_FALSE;
		}
		else if(0!=(CirclesNumber==0))
		{
			m_vRgn[id]=newRgn;
			return S_FALSE;
		}
		else
		{
			if(Selection==1)
			{
				Hobject CircleErosion1,CircleErosion2,ThreeRing,ThreeRingUnion;
				erosion_circle(ThreeCirclesRegion,&CircleErosion1,ErosionSize0);
				erosion_circle(ThreeCirclesRegion,&CircleErosion2,ErosionSize);
				difference(CircleErosion1,CircleErosion2,&ThreeRing);
				union1(ThreeRing,&ThreeRingUnion);
				m_vRgn[id]=ThreeRingUnion;
			}
			else if(Selection==2)
			{
				Hobject CircleErosion3,CircleErosionUnion;
				erosion_circle(ThreeCirclesRegion,&CircleErosion3,ErosionSize);
				union1(CircleErosion3,&CircleErosionUnion);
				m_vRgn[id]=CircleErosionUnion;
			}
		}
		//

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
STDMETHODIMP CRubberMatAlgo::ThreeCirclesRegionAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 1059;LP;1;2.0;1;2$\
					 1225;FP;1.0;20;0.5;5$\
					 443;FP;1;60;1;2.5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::ThreeCirclesRegionAlgo(VARIANT* rgnId,VARIANT*pSelection ,VARIANT* pErosionSize)
{
	int   id          =(int)rgnId->fltVal;
	int   Selection   =(int)pSelection->fltVal;
	//float ErosionSize0 =(float)pErosionSize0->fltVal;
	float ChangingSize =(float)pErosionSize->fltVal;

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

	Hobject CirclesDilation,CharRgn;

#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject Image,ThreeCirclesRegion;
		HTuple  PhiValue,CirclesNumber;
		Image=m_crtImg;
		ThreeCirclesRegion=m_threeCirclesRegion;
		PhiValue=m_phiValue;
		CirclesNumber=m_circlesNumber;
		if (CirclesNumber == 0)
		{
			m_vRgn[id]=newRgn;
			return S_FALSE;
		}
		if (CirclesNumber != 3)
		{
			m_vRgn[id]=ThreeCirclesRegion;
			return S_FALSE;
		}		
		Hobject ThreeCirclesOuter, ThreeCirclesInner, ThreeRing, ThreeRingUnion;
		if (Selection == 0)
		{
			//检测三圆边缘
			if (ChangingSize < -9.5)
				ChangingSize = -9.5;

			if (ChangingSize >= 0.5)
			{
				dilation_circle(ThreeCirclesRegion, &ThreeCirclesOuter, ChangingSize + 10);
				dilation_circle(ThreeCirclesRegion, &ThreeCirclesInner, ChangingSize);
			}
			else if (ChangingSize <= -0.5)
			{
				dilation_circle(ThreeCirclesRegion, &ThreeCirclesOuter, ChangingSize + 10);
				erosion_circle(ThreeCirclesRegion, &ThreeCirclesInner, -ChangingSize);
			}
			else
			{
				dilation_circle(ThreeCirclesRegion, &ThreeCirclesOuter, ChangingSize + 10);
				ThreeCirclesInner = ThreeCirclesRegion;
			}
			difference(ThreeCirclesOuter, ThreeCirclesInner, &ThreeRing);
			union1(ThreeRing, &ThreeRingUnion);
		}
		else if (Selection == 1)
		{
			//检测三圆斜坡
			if (ChangingSize > 0.5)
			{
				erosion_circle(ThreeCirclesRegion, &ThreeCirclesOuter, 3);
				erosion_circle(ThreeCirclesRegion, &ThreeCirclesInner, 3 + ChangingSize);
				difference(ThreeCirclesOuter, ThreeCirclesInner, &ThreeRing);
				union1(ThreeRing, &ThreeRingUnion);
			}
			else
				union1(ThreeCirclesRegion, &ThreeRingUnion);
		}
		else if(Selection == 2)
		{
			//检测三圆底部
			if (ChangingSize >= 0.5)
				erosion_circle(ThreeCirclesRegion, &ThreeCirclesOuter, ChangingSize);
			else
				ThreeCirclesOuter = ThreeCirclesRegion;

			union1(ThreeCirclesOuter, &ThreeRingUnion);
		}
		else
		{
			m_vRgn[id]=newRgn;
			return S_FALSE;
		}
		m_vRgn[id]=ThreeRingUnion;
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
STDMETHODIMP CRubberMatAlgo::ThreeCirclesRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 1059;LP;0;2;1;2$\
					 443;FP;-100;100;0.5;2.5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::UniqueCirclesRegionAlgo(VARIANT* rgnId,VARIANT*pSelection ,VARIANT* pErosionSize0,VARIANT* pErosionSize)
{
	int   id          =(int)rgnId->fltVal;
	int   Selection   =(int)pSelection->fltVal;
	float ErosionSize0 =(float)pErosionSize0->fltVal;
	float ErosionSize =(float)pErosionSize->fltVal;

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

	Hobject CirclesDilation,CharRgn;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}

		Hobject Image,UniqueCirclesRegion;
		HTuple  PhiValue,CirclesNumber;
		Image=m_crtImg;
		//
		UniqueCirclesRegion=m_UniqueCirclesRegion;
		PhiValue=m_phiValue;
		CirclesNumber=m_circlesNumber;

		if(0!=(CirclesNumber!=1))
		{
			m_vRgn[id]=UniqueCirclesRegion;
			return S_FALSE;
		}
		else if(0!=(CirclesNumber==0))
		{
			m_vRgn[id]=newRgn;
			return S_FALSE;
		}
		else
		{
			if(Selection==1)
			{
				Hobject CircleErosion1,CircleErosion2,ThreeRing,ThreeRingUnion;
				erosion_circle(UniqueCirclesRegion,&CircleErosion1,ErosionSize0);
				erosion_circle(UniqueCirclesRegion,&CircleErosion2,ErosionSize);
				difference(CircleErosion1,CircleErosion2,&ThreeRing);
				union1(ThreeRing,&ThreeRingUnion);
				m_vRgn[id]=ThreeRingUnion;
			}
			else if(Selection==2)
			{
				Hobject CircleErosion3,CircleErosionUnion;
				erosion_circle(UniqueCirclesRegion,&CircleErosion3,ErosionSize);
				union1(CircleErosion3,&CircleErosionUnion);
				m_vRgn[id]=CircleErosionUnion;
			}
		}
		//

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
STDMETHODIMP CRubberMatAlgo::UniqueCirclesRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 1059;LP;0;2;1;2$\
					 1225;FP;1.0;20;0.5;5$\
					 443;FP;1;60;1;2.5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::OuterRingLocationAlgo(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pDist,VARIANT* pAngleShift)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	float Dist=(float)pDist->fltVal;
	float AngleShift =(float)pAngleShift->fltVal;

	float LocationRadius=(float)(m_FirstinnerCircleRadius[0].I());
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
	fCenterX = m_FirstCenterX;
	fCenterY = m_FirstCenterY;

	HTuple ImgWidth=m_imgWidth;
	HTuple ImgHeight=m_imgHeight;

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
		}
		else if(m_RadiusExt<0 && Radius>0)
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, Radius);
		}
		else
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, 40);
			m_vRgn[id] = RegionDetect ;
			return S_FALSE;
		}

		Hobject Image,ThreeCirclesRegion;
		HTuple  PhiValue,CirclesNumber;
		Image=m_crtImg;
		//
		ThreeCirclesRegion=m_threeCirclesRegion;
		PhiValue=m_phiValue;
		CirclesNumber=m_circlesNumber;

		if(0!=(CirclesNumber!=3))
		{
			m_vRgn[id]=ThreeCirclesRegion;
			return S_FALSE;
		}
		else if(0!=(CirclesNumber==0))
		{
			m_vRgn[id]=newRgn;
			return S_FALSE;
		}
		else
		{
			HTuple  HomMat2DCircleRoate;
			Hobject ImageEnhanceAffine;

			Hobject Rect,RectTrans1,RectTrans2,RectTrans3,RegionUnion;
			HTuple  AngleStart1,AngleEnd1,AngleStart2,AngleEnd2,AngleStart3,AngleEnd3;
			gen_rectangle1(&Rect,0,0,40,ImgWidth-1);

			AngleStart1 = (((89/360.0)*6.28)+PhiValue)-((AngleShift/360.0)*6.28);
			AngleEnd1 = (((91/360.0)*6.28)+PhiValue)+((AngleShift/360.0)*6.28);
			polar_trans_region_inv(Rect, &RectTrans1, fCenterY,fCenterX, AngleStart1,AngleEnd1, m_innerCircleRadius-40+Dist,m_innerCircleRadius+Dist , ImgWidth, 40+1,ImgWidth, ImgHeight, "nearest_neighbor");

			AngleStart2 = ((((89/360.0)*6.28)+PhiValue)+(6.24/3.0))-((AngleShift/360.0)*6.28);
			AngleEnd2 = ((((91/360.0)*6.28)+PhiValue)+(6.24/3.0))+((AngleShift/360.0)*6.28);
			polar_trans_region_inv(Rect, &RectTrans2, fCenterY,fCenterX, AngleStart2,AngleEnd2, m_innerCircleRadius-40+Dist,m_innerCircleRadius+Dist , ImgWidth, 40+1,ImgWidth, ImgHeight, "nearest_neighbor");

			AngleStart3 = ((((89/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))-((AngleShift/360.0)*6.28);
			AngleEnd3 = ((((91/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))+((AngleShift/360.0)*6.28);
			polar_trans_region_inv(Rect, &RectTrans3, fCenterY,fCenterX, AngleStart3,AngleEnd3, m_innerCircleRadius-40+Dist,m_innerCircleRadius+Dist , ImgWidth, 40+1,ImgWidth, ImgHeight, "nearest_neighbor");

			union2(RectTrans1, RectTrans2, &RegionUnion);
			union2(RegionUnion, RectTrans3, &RegionUnion);

			Hobject RegionDiff3;
			difference(RegionDetect,RegionUnion,&RegionDiff3);
			m_vRgn[id]=RegionDiff3;
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
STDMETHODIMP CRubberMatAlgo::OuterRingLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 693;FP;-1500;1500;0.5;10$\
					 413;FP;1;2000;0.5;208$\
					 480;FP;2 ;100;1;62$\
					 614;FP;0;60;1;10"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::OuterRingLocationAlgo1(VARIANT* rgnId,VARIANT*pSelect,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pDist,VARIANT*pBotSize,VARIANT* pAngleShift)
{
	//存在三个圆、唯一圆定位角度，扣除外环三个凸点
	int id = (int)rgnId->fltVal;
	int Select=(int)pSelect->fltVal;

	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	float Dist=(float)pDist->fltVal;
	float AngleShift =(float)pAngleShift->fltVal;
	float BotSize =(float)pBotSize->fltVal;

	float LocationRadius=(float)(m_FirstinnerCircleRadius[0].I());
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
	fCenterX = m_FirstCenterX;
	fCenterY = m_FirstCenterY;

	HTuple ImgWidth=m_imgWidth;
	HTuple ImgHeight=m_imgHeight;

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
		}
		else if(m_RadiusExt<0 && Radius>0)
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, Radius);
		}
		else
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, 40);
			m_vRgn[id] = RegionDetect ;
			return S_FALSE;
		}

		Hobject Image,UniqueCirclesRegion,ThreeCirclesRegion;
		HTuple  PhiValue,CirclesNumber;
		Image=m_crtImg;
		//
		UniqueCirclesRegion=m_UniqueCirclesRegion;
		ThreeCirclesRegion=m_threeCirclesRegion;
		PhiValue=m_phiValue;
		CirclesNumber=m_circlesNumber;

		if (Select==1)
		{
			if(0!=(CirclesNumber!=1))
			{
				m_vRgn[id]=UniqueCirclesRegion;
				return S_FALSE;
			}
			else if(0!=(CirclesNumber==0))
			{
				m_vRgn[id]=newRgn;
				return S_FALSE;
			}
			else
			{
				HTuple  HomMat2DCircleRoate;
				Hobject ImageEnhanceAffine;

				Hobject Rect,RectTrans1,RectTrans2,RectTrans3,RegionUnion;
				HTuple  AngleStart1,AngleEnd1,AngleStart2,AngleEnd2,AngleStart3,AngleEnd3;
				gen_rectangle1(&Rect,0,0,60,ImgWidth-1);

				AngleStart1 = (((BotSize/360.0)*6.28)+PhiValue)-((AngleShift/360.0)*6.28);
				AngleEnd1 = (((BotSize/360.0)*6.28)+PhiValue)+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans1, fCenterY,fCenterX, AngleStart1,AngleEnd1, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				AngleStart2 = ((((BotSize/360.0)*6.28)+PhiValue)+(6.24/3.0))-((AngleShift/360.0)*6.28);
				AngleEnd2 = ((((BotSize/360.0)*6.28)+PhiValue)+(6.24/3.0))+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans2, fCenterY,fCenterX, AngleStart2,AngleEnd2, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				AngleStart3 = ((((BotSize/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))-((AngleShift/360.0)*6.28);
				AngleEnd3 = ((((BotSize/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans3, fCenterY,fCenterX, AngleStart3,AngleEnd3, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				union2(RectTrans1, RectTrans2, &RegionUnion);
				union2(RegionUnion, RectTrans3, &RegionUnion);
				Hobject RegionDiff3;
				difference(RegionDetect,RegionUnion,&RegionDiff3);
				m_vRgn[id]=RegionDiff3;
			}
		}
		else if(Select==2)
		{
			if(0!=(CirclesNumber!=3))
			{
				m_vRgn[id]=ThreeCirclesRegion;
				return S_FALSE;
			}
			else if(0!=(CirclesNumber==0))
			{
				m_vRgn[id]=newRgn;
				return S_FALSE;
			}
			else
			{
				HTuple  HomMat2DCircleRoate;
				Hobject ImageEnhanceAffine;

				Hobject Rect,RectTrans1,RectTrans2,RectTrans3,RegionUnion;
				HTuple  AngleStart1,AngleEnd1,AngleStart2,AngleEnd2,AngleStart3,AngleEnd3;
				gen_rectangle1(&Rect,0,0,60,ImgWidth-1);

				AngleStart1 = (((BotSize/360.0)*6.28)+PhiValue)-((AngleShift/360.0)*6.28);
				AngleEnd1 = (((BotSize/360.0)*6.28)+PhiValue)+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans1, fCenterY,fCenterX, AngleStart1,AngleEnd1, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				AngleStart2 = ((((BotSize/360.0)*6.28)+PhiValue)+(6.24/3.0))-((AngleShift/360.0)*6.28);
				AngleEnd2 = ((((BotSize/360.0)*6.28)+PhiValue)+(6.24/3.0))+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans2, fCenterY,fCenterX, AngleStart2,AngleEnd2, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				AngleStart3 = ((((BotSize/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))-((AngleShift/360.0)*6.28);
				AngleEnd3 = ((((BotSize/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans3, fCenterY,fCenterX, AngleStart3,AngleEnd3, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				union2(RectTrans1, RectTrans2, &RegionUnion);
				union2(RegionUnion, RectTrans3, &RegionUnion);

				Hobject RegionDiff3;
				difference(RegionDetect,RegionUnion,&RegionDiff3);
				m_vRgn[id]=RegionDiff3;
			}
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
STDMETHODIMP CRubberMatAlgo::OuterRingLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 1294;LP;1;2.0;1;2$\
					 693;FP;-1500;1500;0.5;10$\
					 413;FP;1;2000;0.5;208$\
					 480;FP;2 ;100;1;62$\
					 614;FP;0;60;1;5$\
					 1103;FP;0;50;1;5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::OuterRingLocationAlgo2(VARIANT* rgnId,VARIANT*pSelect,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pDist,VARIANT*pBotSize,VARIANT* pAngleShift,VARIANT* pThredValue,VARIANT*pGrayValue,VARIANT* pDilationSize)
{
	//存在三个圆、唯一圆定位角度，扣除外环三个凸点
	int id = (int)rgnId->fltVal;
	int Select=(int)pSelect->fltVal;

	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	float Dist=(float)pDist->fltVal;
	float AngleShift =(float)pAngleShift->fltVal;
	float BotSize =(float)pBotSize->fltVal;
	//
	float ThredValue    = (float)pThredValue->fltVal;
	float GrayValue    = (float)pGrayValue->fltVal;
	float DilationSize = (float)pDilationSize->fltVal;

	float LocationRadius=(float)(m_FirstinnerCircleRadius[0].I());
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
	fCenterX = m_FirstCenterX;
	fCenterY = m_FirstCenterY;

	HTuple ImgWidth=m_imgWidth;
	HTuple ImgHeight=m_imgHeight;

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
		}
		else if(m_RadiusExt<0 && Radius>0)
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, Radius);
			//
			Circle1_Inner=RegionDetect;
		}
		else
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, 40);
			Circle1_Inner=RegionDetect;
			m_vRgn[id] = RegionDetect ;
			return S_FALSE;
		}

		Hobject Image,UniqueCirclesRegion,ThreeCirclesRegion;
		HTuple  PhiValue,CirclesNumber;
		Hobject ImageReduced,DiffOfGauss,ImageConverted,ImageEnhance;
		Image=m_crtImg;
		reduce_domain(Image, m_outRegion, &ImageReduced);
		diff_of_gauss(ImageReduced, &DiffOfGauss, 5, 10);
		convert_image_type(DiffOfGauss, &ImageConverted, "byte");
		abs_diff_image(ImageReduced, ImageConverted, &ImageEnhance, 1);
		//
		UniqueCirclesRegion=m_UniqueCirclesRegion;
		ThreeCirclesRegion=m_threeCirclesRegion;
		PhiValue=m_phiValue;
		CirclesNumber=m_circlesNumber;

		if (Select==1)
		{
			if(0!=(CirclesNumber!=1))
			{
				m_vRgn[id]=UniqueCirclesRegion;
				return S_FALSE;
			}
			else if(0!=(CirclesNumber==0))
			{
				m_vRgn[id]=newRgn;
				return S_FALSE;
			}
			else
			{
				HTuple  HomMat2DCircleRoate;
				Hobject ImageEnhanceAffine;

				Hobject Rect,RectTrans1,RectTrans2,RectTrans3,RegionUnion;
				HTuple  AngleStart1,AngleEnd1,AngleStart2,AngleEnd2,AngleStart3,AngleEnd3;
				gen_rectangle1(&Rect,0,0,60,ImgWidth-1);

				AngleStart1 = (((BotSize/360.0)*6.28)+PhiValue)-((AngleShift/360.0)*6.28);
				AngleEnd1 = (((BotSize/360.0)*6.28)+PhiValue)+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans1, fCenterY,fCenterX, AngleStart1,AngleEnd1, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				AngleStart2 = ((((BotSize/360.0)*6.28)+PhiValue)+(6.24/3.0))-((AngleShift/360.0)*6.28);
				AngleEnd2 = ((((BotSize/360.0)*6.28)+PhiValue)+(6.24/3.0))+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans2, fCenterY,fCenterX, AngleStart2,AngleEnd2, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				AngleStart3 = ((((BotSize/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))-((AngleShift/360.0)*6.28);
				AngleEnd3 = ((((BotSize/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans3, fCenterY,fCenterX, AngleStart3,AngleEnd3, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				union2(RectTrans1, RectTrans2, &RegionUnion);
				union2(RegionUnion, RectTrans3, &RegionUnion);
				////////////////////////
				Hobject OutCircle,Roi;
				erosion_circle(m_outRegion, &OutCircle, 4.0);
				//gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusExt);
				difference(OutCircle, Circle1_Inner, &Roi);
				//
				Hobject ImageReduced1,EdgeAmplitude,ImageSub1,ImageEmphasize2,RoiErosion1;
				reduce_domain(ImageEnhance, Roi, &ImageReduced1);
				sobel_amp(ImageReduced1, &EdgeAmplitude, "sum_abs", 3);
				sub_image(ImageReduced1, EdgeAmplitude, &ImageSub1, 1, 10);
				emphasize(ImageSub1, &ImageEmphasize2, 13, 13, 1);
				median_image(ImageEmphasize2, &ImageEmphasize2, "circle", 2, "mirrored");
				erosion_circle(Roi, &RoiErosion1, 3.5);
				reduce_domain(ImageEmphasize2, RoiErosion1, &ImageEmphasize2);
				//
				Hobject ImageAffinTrans,RegionDynThresh,RegionClosing1,RegionOpening4,RegionFillUp1;
				HTuple HomMat2D;
				vector_angle_to_rigid(fCenterY, fCenterX, HTuple(10).Rad(), fCenterY, fCenterX, 0, &HomMat2D);
				affine_trans_image(ImageEmphasize2, &ImageAffinTrans, HomMat2D, "constant", "false");
				dyn_threshold(ImageEmphasize2, ImageAffinTrans, &RegionDynThresh, ThredValue, "dark");
				closing_circle(RegionDynThresh, &RegionClosing1, 4.5);
				fill_up_shape(RegionClosing1, &RegionFillUp1, "area", 1, 500);
				opening_circle(RegionFillUp1, &RegionOpening4, 4.5);

				HTuple HomMat2D120,HomMat2D_120;
				Hobject RegionAffineTrans1,RegionAffineTrans,RegionUnion1,ThreeCirclesRegion;
				vector_angle_to_rigid(fCenterY, fCenterX, HTuple(120).Rad(), fCenterY, fCenterX, 0, &HomMat2D120);
				vector_angle_to_rigid(fCenterY, fCenterX, HTuple(-120).Rad(), fCenterY, fCenterX, 0, &HomMat2D_120);
				//异常判断，剔除大黑点
				Hobject ConnectedRegions1,CircleRegs,ObjectSelected;
				HTuple Number,Mean, Deviation,Mean1,Deviation1,MeanDiff;
				connection(RegionOpening4, &ConnectedRegions1);
				count_obj(ConnectedRegions1, &Number);
				gen_empty_region(&CircleRegs);
				if(Number>=1)
				{
					for (int i=1;i<=Number; i++)
					{
						select_obj(ConnectedRegions1, &ObjectSelected, i);
						affine_trans_region(ObjectSelected, &RegionAffineTrans1, HomMat2D_120, "nearest_neighbor");
						intensity(RegionAffineTrans1, ImageReduced, &Mean, &Deviation);
						intensity(ObjectSelected, ImageReduced, &Mean1, &Deviation1);
						MeanDiff = (Mean1-Mean).Abs();
						if (0 != (MeanDiff<GrayValue))
						{
							concat_obj(ObjectSelected, CircleRegs, &CircleRegs);
						}
					}
				}
				union1(CircleRegs, &CircleRegs);
				affine_trans_region(CircleRegs, &RegionAffineTrans1, HomMat2D_120, "nearest_neighbor");
				affine_trans_region(CircleRegs, &RegionAffineTrans, HomMat2D120, "nearest_neighbor");
				union2(RegionAffineTrans1, RegionAffineTrans, &RegionUnion1);
				union2(RegionUnion1, CircleRegs, &RegionUnion1);

				//
				Hobject ConnectedRegions,ThreeCircleRegs,RegIntersection;
				HTuple Number1;
				union1(RegionUnion1,&RegionUnion1);
				intersection(RegionUnion1,RegionUnion,&RegIntersection);
				connection(RegIntersection, &ConnectedRegions);
				count_obj(ConnectedRegions, &Number1);
				if (Number1==3)
				{
					dilation_circle(RegIntersection,&ThreeCirclesRegion,DilationSize);
					ThreeCircleRegs=ThreeCirclesRegion;

				} 
				else if(Number1>3)
				{
					Hobject SortedRegions,ObjectSelected1,ObjectSelected2,ObjectSelected3,RegionUnion2;
					sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "row");
					select_obj(SortedRegions, &ObjectSelected1, 1);
					select_obj(SortedRegions, &ObjectSelected2, 2);
					select_obj(SortedRegions, &ObjectSelected3, 3);
					union2(ObjectSelected1, ObjectSelected2, &RegionUnion2);
					union2(RegionUnion2, ObjectSelected3, &RegionUnion2);
					union1(RegionUnion2,&RegionUnion2);
					//
					dilation_circle(RegionUnion2,&ThreeCirclesRegion,DilationSize);
					ThreeCircleRegs=ThreeCirclesRegion;
				}
				else
				{
					ThreeCircleRegs=RegionUnion;
				}
				////////////////////////////
				Hobject RegionDiff3;
				difference(RegionDetect,ThreeCircleRegs,&RegionDiff3);
				m_vRgn[id]=RegionDiff3;
			}
		}
		else if(Select==2)
		{
			if(0!=(CirclesNumber!=3))
			{
				m_vRgn[id]=ThreeCirclesRegion;
				return S_FALSE;
			}
			else if(0!=(CirclesNumber==0))
			{
				m_vRgn[id]=newRgn;
				return S_FALSE;
			}
			else
			{
				HTuple  HomMat2DCircleRoate;
				Hobject ImageEnhanceAffine;

				Hobject Rect,RectTrans1,RectTrans2,RectTrans3,RegionUnion;
				HTuple  AngleStart1,AngleEnd1,AngleStart2,AngleEnd2,AngleStart3,AngleEnd3;
				gen_rectangle1(&Rect,0,0,60,ImgWidth-1);

				AngleStart1 = (((BotSize/360.0)*6.28)+PhiValue)-((AngleShift/360.0)*6.28);
				AngleEnd1 = (((BotSize/360.0)*6.28)+PhiValue)+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans1, fCenterY,fCenterX, AngleStart1,AngleEnd1, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				AngleStart2 = ((((BotSize/360.0)*6.28)+PhiValue)+(6.24/3.0))-((AngleShift/360.0)*6.28);
				AngleEnd2 = ((((BotSize/360.0)*6.28)+PhiValue)+(6.24/3.0))+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans2, fCenterY,fCenterX, AngleStart2,AngleEnd2, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				AngleStart3 = ((((BotSize/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))-((AngleShift/360.0)*6.28);
				AngleEnd3 = ((((BotSize/360.0)*6.28)+PhiValue)+((6.24/3.0)*2))+((AngleShift/360.0)*6.28);
				polar_trans_region_inv(Rect, &RectTrans3, fCenterY,fCenterX, AngleStart3,AngleEnd3, m_innerCircleRadius-60+Dist,m_innerCircleRadius+Dist , ImgWidth, 60+1,ImgWidth, ImgHeight, "nearest_neighbor");

				union2(RectTrans1, RectTrans2, &RegionUnion);
				union2(RegionUnion, RectTrans3, &RegionUnion);
				////////////////////////
				Hobject OutCircle,Roi;
				erosion_circle(m_outRegion, &OutCircle, 4.0);
				//gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusExt);
				difference(OutCircle, Circle1_Inner, &Roi);
				//
				Hobject ImageReduced1,EdgeAmplitude,ImageSub1,ImageEmphasize2,RoiErosion1;
				reduce_domain(ImageEnhance, Roi, &ImageReduced1);
				sobel_amp(ImageReduced1, &EdgeAmplitude, "sum_abs", 3);
				sub_image(ImageReduced1, EdgeAmplitude, &ImageSub1, 1, 10);
				emphasize(ImageSub1, &ImageEmphasize2, 13, 13, 1);
				median_image(ImageEmphasize2, &ImageEmphasize2, "circle", 2, "mirrored");
				erosion_circle(Roi, &RoiErosion1, 3.5);
				reduce_domain(ImageEmphasize2, RoiErosion1, &ImageEmphasize2);
				//
				Hobject ImageAffinTrans,RegionDynThresh,RegionClosing1,RegionOpening4,RegionFillUp1;
				HTuple HomMat2D;
				vector_angle_to_rigid(fCenterY, fCenterX, HTuple(10).Rad(), fCenterY, fCenterX, 0, &HomMat2D);
				affine_trans_image(ImageEmphasize2, &ImageAffinTrans, HomMat2D, "constant", "false");
				dyn_threshold(ImageEmphasize2, ImageAffinTrans, &RegionDynThresh, ThredValue, "dark");
				closing_circle(RegionDynThresh, &RegionClosing1, 4.5);
				fill_up_shape(RegionClosing1, &RegionFillUp1, "area", 1, 500);
				opening_circle(RegionFillUp1, &RegionOpening4, 4.5);

				HTuple HomMat2D120,HomMat2D_120;
				Hobject RegionAffineTrans1,RegionAffineTrans,RegionUnion1,ThreeCirclesRegion;
				vector_angle_to_rigid(fCenterY, fCenterX, HTuple(120).Rad(), fCenterY, fCenterX, 0, &HomMat2D120);
				vector_angle_to_rigid(fCenterY, fCenterX, HTuple(-120).Rad(), fCenterY, fCenterX, 0, &HomMat2D_120);
				//异常判断，剔除大黑点
				Hobject ConnectedRegions1,CircleRegs,ObjectSelected;
				HTuple Number,Mean, Deviation,Mean1,Deviation1,MeanDiff;
				connection(RegionOpening4, &ConnectedRegions1);
				count_obj(ConnectedRegions1, &Number);
				gen_empty_region(&CircleRegs);
				if(Number>=1)
				{
					for (int i=1;i<=Number; i++)
					{
						select_obj(ConnectedRegions1, &ObjectSelected, i);
						affine_trans_region(ObjectSelected, &RegionAffineTrans1, HomMat2D_120, "nearest_neighbor");
						intensity(RegionAffineTrans1, ImageReduced, &Mean, &Deviation);
						intensity(ObjectSelected, ImageReduced, &Mean1, &Deviation1);
						MeanDiff = (Mean1-Mean).Abs();
						if (0 != (MeanDiff<GrayValue))
						{
							concat_obj(ObjectSelected, CircleRegs, &CircleRegs);
						}
					}
				}
				union1(CircleRegs, &CircleRegs);
				affine_trans_region(CircleRegs, &RegionAffineTrans1, HomMat2D_120, "nearest_neighbor");
				affine_trans_region(CircleRegs, &RegionAffineTrans, HomMat2D120, "nearest_neighbor");
				union2(RegionAffineTrans1, RegionAffineTrans, &RegionUnion1);
				union2(RegionUnion1, CircleRegs, &RegionUnion1);
				
				//
				Hobject ConnectedRegions,ThreeCircleRegs,RegIntersection;
				HTuple Number1;
				union1(RegionUnion1,&RegionUnion1);
				intersection(RegionUnion1,RegionUnion,&RegIntersection);
				connection(RegIntersection, &ConnectedRegions);
				count_obj(ConnectedRegions, &Number1);
				if (Number1==3)
				{
					dilation_circle(RegIntersection,&ThreeCirclesRegion,DilationSize);
					ThreeCircleRegs=ThreeCirclesRegion;

				} 
				else if(Number1>3)
				{
					Hobject SortedRegions,ObjectSelected1,ObjectSelected2,ObjectSelected3,RegionUnion2;
					sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "row");
					select_obj(SortedRegions, &ObjectSelected1, 1);
					select_obj(SortedRegions, &ObjectSelected2, 2);
					select_obj(SortedRegions, &ObjectSelected3, 3);
					union2(ObjectSelected1, ObjectSelected2, &RegionUnion2);
					union2(RegionUnion2, ObjectSelected3, &RegionUnion2);
					union1(RegionUnion2,&RegionUnion2);
					//
					dilation_circle(RegionUnion2,&ThreeCirclesRegion,DilationSize);
					ThreeCircleRegs=ThreeCirclesRegion;
				}
				else
				{
					ThreeCircleRegs=RegionUnion;
				}
				////////////////////////////
				Hobject RegionDiff3;
				difference(RegionDetect,ThreeCircleRegs,&RegionDiff3);
				m_vRgn[id]=RegionDiff3;
			}
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
STDMETHODIMP CRubberMatAlgo::OuterRingLocationAlgo2Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 1294;LP;1;2.0;1;2$\
					 693;FP;-1500;1500;0.5;10$\
					 413;FP;1;2000;0.5;208$\
					 480;FP;2 ;100;1;62$\
					 614;FP;0;60;1;5$\
					 1103;FP;0;50;1;5$\
					 749;FP;2.0;50.0;0.5;15$\
					 1268;FP;2;100.0;0.5;30$\
					 1121;FP;3;20;0.5;3.5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::RegionLocationByModelTool(VARIANT*rgnId,BSTR* bstrRgnFile)
{
	int id = (int)rgnId->fltVal;
	//
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	int fCenterX = 0;
	int fCenterY = 0;
	fCenterX = (int)m_vPos[0].m_x;
	fCenterY = (int)m_vPos[0].m_y;
	//
	CString sRgnPath  = *bstrRgnFile;
	string strRgnPath = CT2A(sRgnPath);

#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			if(!_access(strRgnPath.c_str(),0))
			{
				Hobject retRgn;
				read_region(&retRgn,strRgnPath.c_str());
				m_dictCrtPathRgn[strRgnPath] = retRgn;
				m_dictNewPathRgn[strRgnPath] = retRgn;

			}
			else
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("初始化参数,检测区域路径设置错误。"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );	
				}
				else
				{
					MessageBox(NULL, _T("Initialization parameter（detect region path）setting is wrong!"),_T("Initial configuration is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_vRgn[id].Reset();
				return S_FALSE;
			}
			m_vRgn[id].Reset();
			return S_OK;
		}

		Hobject RegionDetect;

		HTuple modelPARAID = m_vRgnReadData[id];
		if(m_dictCrtPathRgn.find(strRgnPath) == m_dictCrtPathRgn.end())
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject roiRgn = m_dictCrtPathRgn[strRgnPath];
		m_modelPARA = modelPARAID;

		Hobject  RegionAffineTrans;
		move_region (roiRgn,&RegionAffineTrans,fCenterY,fCenterX) ;
		//move_region (roiRgn,&RegionAffineTrans,fCenterY-m_ImageHeight/2.0,fCenterX-m_ImageWidth/2.0) ;

		HTuple   HomMat2D,HomMat2DTmp,HomMat2DAdapted;

		//添加错误判断机制
		if (0!=(m_modelAngle.Num()!=0))
		{
			vector_angle_to_rigid(fCenterY,fCenterX,0,fCenterY,fCenterX,m_modelAngle,&HomMat2D);
			hom_mat2d_translate(HomMat2D, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
			projective_trans_region (RegionAffineTrans, &RegionAffineTrans, HomMat2DAdapted, "bilinear");
			//
			m_vRgn[id] = RegionAffineTrans ;	
		}
		else
		{
			m_vRgn[id] = newRgn ;
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
STDMETHODIMP CRubberMatAlgo::RegionLocationByModelToolHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("442$\
					 167;B$\
					 267;SPR" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::EdgeDistortionAlgo(VARIANT* rgnId,VARIANT*pThreshold,VARIANT*pMaxMinSubThreshold,VARIANT*pMaxMinSub )
{
	int rId = (int)rgnId->fltVal;
	int ThresholdValue=(int)pThreshold->fltVal;
	int MaxMinSubThreshold=(int)pMaxMinSubThreshold->fltVal;

	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pMaxMinSub);
		return S_FALSE;
	}


	float fCenterX = 0.0;
	float fCenterY = 0.0;
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

		Hobject  ImageReduced1,Region,RegionOpening,ConnectedRegions,SelectedRegions;
		reduce_domain(Image, rgn, &ImageReduced1);

		threshold(ImageReduced1, &Region, ThresholdValue, 255);
		opening_circle(Region, &RegionOpening, 5.5);
		connection(RegionOpening, &ConnectedRegions);
		select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);

		Hobject Circle,RegionDifference,IntersectionAll,Lines,RegionIntersection;
		gen_circle(&Circle,fCenterY,fCenterX,m_innerCircleRadius);
		difference(SelectedRegions, Circle, &RegionDifference);

		HTuple i;
		gen_empty_obj(&IntersectionAll);
		for ( i=1; i<=360; i+=3)
		{
			gen_region_line(&Lines, fCenterY, fCenterX, fCenterY-(500*((i.Rad()).Sin())),fCenterX+(500*((i.Rad()).Cos())));
			intersection(RegionDifference, Lines, &RegionIntersection);
			concat_obj(IntersectionAll, RegionIntersection, &IntersectionAll);
		}

		HTuple ContLength,MaxMinSub,Indices;
		Hobject ObjectSelected,RegionBorder,RegionIntersection1,RegionDilation,RegionUnion;
		contlength(IntersectionAll, &ContLength);

		MaxMinSub = (ContLength.Max())-(ContLength.Min());

		if (0 != (MaxMinSub>MaxMinSubThreshold))
		{
			//相差18个像素，大约1mm
			tuple_sort_index(ContLength, &Indices);
			select_obj(IntersectionAll, &ObjectSelected, (Indices.Select((Indices.Num())-1))+1);
			boundary(SelectedRegions, &RegionBorder, "inner");
			intersection(ObjectSelected, RegionBorder, &RegionIntersection1);
			dilation_circle(RegionIntersection1, &RegionDilation, 22.5);

			retValue = MaxMinSub[0].I();
			m_vErrorRgn[rId] = RegionDilation;
			retValue.Detach(pMaxMinSub);
		}
		else
		{
			retValue = MaxMinSub[0].I();
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
			retValue.Detach(pMaxMinSub);
		}

#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pMaxMinSub);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::EdgeDistortionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("854$\
					 855;R;LP;1;255;15$\
					 167;B$\
					 649;LP;1;255;1;20$\
					 855;LP;1;50;1;15");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::DetectRegionLocationExtractOCRAlgo(VARIANT* rgnId,VARIANT* pErosionSize0,VARIANT* pErosionSize,VARIANT*pCircleErosionSize,BSTR* bstrOcrRgnFile)
{
	int id = (int)rgnId->fltVal;
	float ErosionSize0=(float)pErosionSize0->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	float CircleErosionSize=(float)pCircleErosionSize->fltVal;
	//
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	int fCenterX = 0;
	int fCenterY = 0;
	fCenterX = (int)m_vPos[0].m_x;
	fCenterY = (int)m_vPos[0].m_y;
	//
	CString sRgnPath  = *bstrOcrRgnFile;
	string strRgnPath = CT2A(sRgnPath);
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			if(!_access(strRgnPath.c_str(),0))
			{
				Hobject retRgn;
				read_region(&retRgn,strRgnPath.c_str());
				m_dictCrtPathRgn[strRgnPath] = retRgn;
				m_dictNewPathRgn[strRgnPath] = retRgn;

			}
			else
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("初始化参数,检测区域路径设置错误。"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );	
				}
				else
				{
					MessageBox(NULL, _T("Initialization parameter（detect region path）setting is wrong!"),_T("Initial configuration is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_vRgn[id].Reset();
				return S_FALSE;
			}
			m_vRgn[id].Reset();
			return S_OK;
		}

		Hobject RegionDetect;

		HTuple modelPARAID = m_vRgnReadData[id];
		if(m_dictCrtPathRgn.find(strRgnPath) == m_dictCrtPathRgn.end())
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject roiRgn = m_dictCrtPathRgn[strRgnPath];
		m_modelPARA = modelPARAID;
		Hobject  RegionAffineTrans;
		move_region (roiRgn,&RegionAffineTrans,fCenterY,fCenterX) ;

		HTuple   HomMat2D,HomMat2DTmp,HomMat2DAdapted;
		//添加错误判断机制
		if (0!=(m_modelAngle.Num()!=0))
		{
			vector_angle_to_rigid(fCenterY,fCenterX,0,fCenterY,fCenterX,m_modelAngle,&HomMat2D);
			hom_mat2d_translate(HomMat2D, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
			projective_trans_region (RegionAffineTrans, &RegionAffineTrans, HomMat2DAdapted, "bilinear");	
		}
		else
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject RegionDetectOcr;
		if(ErosionSize >= 0.5)
		{
			erosion_circle (RegionAffineTrans, &RegionDetectOcr,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(RegionAffineTrans, &RegionDetectOcr,abs(ErosionSize));
		}
		else
		{
			RegionDetectOcr=RegionAffineTrans;
		}
		Hobject  RegionFill,InnerCirclesRegion,LocationRegion,RegionCircle;
		fill_up(m_centerLocationRegion,&RegionFill);
		HTuple row1,col1,radius1;
		smallest_circle(RegionFill,&row1,&col1,&radius1);
		if (row1.Num()!=1)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		gen_circle(&RegionCircle,row1,col1,radius1);
		erosion_circle (RegionCircle, &LocationRegion,ErosionSize0);
		//gen_circle(&RegionCenter, fCenterY, fCenterX, Radius);
		dilation_circle(m_InnerCirclesRegion,&InnerCirclesRegion,CircleErosionSize);
		difference(LocationRegion,InnerCirclesRegion,&RegionDetect);
		difference(RegionDetect,RegionDetectOcr,&RegionDetect);
		m_vRgn[id] = RegionDetect;	
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
STDMETHODIMP CRubberMatAlgo::DetectRegionLocationExtractOCRAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("575$\
					 167;B$\
					 1225;FP;1.0;2000.0;0.5;185$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 1296;FP;0.5;10.5;0.5;1.0$\
					 232;SPR" );
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void  CRubberMatAlgo::ExtractEdgesPointsDist(Hobject Image, HTuple Elements, HTuple DetectHeight, HTuple DetectWidth, HTuple Sigma, HTuple Threshold, HTuple Transition, HTuple Select, 
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
STDMETHODIMP CRubberMatAlgo::DiameterMeasureAlgo(VARIANT* rgnId,  VARIANT* pPixelSize,  VARIANT* pStdDiameter,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff)
{
	int rId           = (int)rgnId->fltVal;
	float StdDiameter   = (float)pStdDiameter->fltVal;
	float PixelSize   = (float)pPixelSize->fltVal;//866
	m_pixelSize=(double)fabs(PixelSize);
	float maskSize    = (float)pMaskSize->fltVal;
	int thredLow   = (int)pThredLow->fltVal;
	int thredUpper = (int)pThredUpper->fltVal;
	if(thredUpper<=thredLow)
	{
		thredUpper=thredLow+3;
	}
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
		if(m_bPilotImg)
		{
			return S_OK;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//
		Hobject crtImg = m_crtImg;
		Hobject imgReduced;

		Hobject  ExpandedImage,ConnectedRegions,ImageGuass, Edges;
		Hobject  CroppedContours, SelectedXLD,Rectangle1,ConnectedRegions1;
		Hobject  SortedContours, ObjectSelected, Regions1;
		Hobject  Cross, ObjectSelected1, Regions2,Cross2;
		Hobject  SortedContours3, ObjectSelected8;

		HTuple  Length,Number,Number1, RowBegin, ColBegin, RowEnd;
		HTuple  ColEnd, Nr, Nc, Dist, ResultRow;
		HTuple  ResultColumn, Row11, Col1, Row21, Col2;
		HTuple  RowBegin2, ColBegin2, RowEnd2, ColEnd2;
		HTuple  Nr2, Nc2, Dist2, ResultRow2, ResultColumn2,Distance;
		HTuple  Row113, Column112, Row213;
		HTuple  Column212, Row7, Column7, Phi11, Length13;
		HTuple  Length23, Number8, Diameter;

		connection(rgn, &ConnectedRegions1);
		count_obj(ConnectedRegions1, &Number1);
		if (0 != (Number1==1))
		{
			smallest_rectangle1(rgn, &Row113, &Column112, &Row213, &Column212);
			//inner_rectangle1(rgn, &Row113, &Column112, &Row213, &Column212);
			smallest_rectangle2(rgn, &Row7, &Column7, &Phi11,  &Length13, &Length23);
			Hobject ImageReduced2,ImageMean;
			if (0 != (Length23>5))
			{
				reduce_domain(crtImg, rgn, &imgReduced);
				mean_image(imgReduced, &ImageMean, 1, 7);
				expand_domain_gray(ImageMean, &ExpandedImage, 3);
				reduce_domain(ExpandedImage, rgn, &ImageReduced2);

				gauss_filter(imgReduced,&ImageGuass,5);
				edges_sub_pix(ImageReduced2, &Edges, "canny", maskSize, thredLow, thredUpper);
				//
				Hobject ErrorReg,Region,ObjectSelected2;
				HTuple Number3,XRow, XCol;
				//gen_region_contour_xld(Edges1,&Reg, "margin");
				//
				count_obj(Edges, &Number3);
				if (Number3>=1)
				{
					gen_empty_region(&ErrorReg);
					for (int a=1; a<=Number3[0].I(); a++)
					{
						select_obj(Edges, &ObjectSelected2, a);
						get_contour_xld(ObjectSelected2, &XRow, &XCol);
						gen_region_points(&Region, XRow, XCol);
						concat_obj(ErrorReg, Region, &ErrorReg);
					}
				}
				else
				{
					retValue = -1;
					m_vErrorRgn[rId].Reset();
					retValue.Detach(pDefectDiff);
					return S_FALSE;
				}

				crop_contours_xld(Edges, &CroppedContours, Row113+5, Column112,  Row213-5, Column212, "false");				
				//提取两端xld
				length_xld(CroppedContours, &Length);
				select_shape_xld(CroppedContours, &SelectedXLD, "contlength", "and", Length23-5, 999999);
				sort_contours_xld(SelectedXLD, &SortedContours3, "upper_left", "true",  "column");

				count_obj(SortedContours3, &Number8);
				select_obj(SortedContours3, &ObjectSelected8, HTuple(1).Concat(Number8));
				//
				count_obj(ObjectSelected8, &Number);
				Hobject SelectedXLD1,SelectedXLD2;

				if (0 != (Number==2))
				{
					sort_contours_xld(ObjectSelected8, &SortedContours, "upper_left",  "true", "column");
					select_obj(SortedContours, &ObjectSelected, 1);
					Hobject ContoursSplit,SortedContours1;
					HTuple Number1;
					segment_contours_xld(ObjectSelected, &ContoursSplit, "lines_circles", 5, 4, 2);
					sort_contours_xld(ContoursSplit, &SortedContours1, "upper_left", "true", "column");
					count_obj(SortedContours1, &Number1);
					select_obj(SortedContours1, &SelectedXLD1, 1);
					//
					HTuple SelectedXLD1Length;
					length_xld(SelectedXLD1,&SelectedXLD1Length);

					fit_line_contour_xld(SelectedXLD1, "tukey", -1, 0, 5, 2, &RowBegin,  &ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
					ExtractEdgesPointsDist(ImageGuass, 10, 20, 5, 1, 20, "all", "max", RowBegin,  ColBegin, RowEnd, ColEnd, &ResultRow, &ResultColumn);

					Hobject Cross1Reg;
					gen_cross_contour_xld(&Cross, ResultRow, ResultColumn, 3, 0.785398);
					gen_region_contour_xld(Cross,&Cross1Reg,"margin");

					if(m_bDebugImg)
					{
						concat_obj (m_vRgn[rId], Cross1Reg, &m_vRgn[rId]);
					}

					HTuple Row1Base,  Col1Base, Row2Base, Col2Base;
					FitLineUseEdgesPoints(ResultRow, ResultColumn, 8, &Row1Base,  &Col1Base, &Row2Base, &Col2Base);

					select_obj(SortedContours, &ObjectSelected1, 2);
					Hobject ContoursSplit1,SortedContours2;
					HTuple Number2;
					segment_contours_xld(ObjectSelected1, &ContoursSplit1, "lines_circles", 5, 4, 2);
					sort_contours_xld(ContoursSplit1, &SortedContours2, "upper_left", "true", "column");
					count_obj(SortedContours2, &Number2);
					select_obj(SortedContours2, &SelectedXLD2, Number2);
					//
					HTuple SelectedXLD2Length;
					length_xld(SelectedXLD2,&SelectedXLD2Length);

					fit_line_contour_xld(SelectedXLD2, "tukey", -1, 0, 5, 2, &RowBegin2,  &ColBegin2, &RowEnd2, &ColEnd2, &Nr2, &Nc2, &Dist2);
					ExtractEdgesPointsDist(ImageGuass, 10, 20, 5, 1, 20, "all", "max", RowBegin2,  ColBegin2, RowEnd2, ColEnd2, &ResultRow2, &ResultColumn2);

					Hobject Cross2Reg;
					gen_cross_contour_xld(&Cross2, ResultRow2, ResultColumn2, 3, 0.785398);
					gen_region_contour_xld(Cross2,&Cross2Reg,"margin");

					if(m_bDebugImg)
					{
						concat_obj (m_vRgn[rId], Cross2Reg, &m_vRgn[rId]);
					}

					HTuple Row3Base,  Col3Base, Row4Base, Col4Base;
					FitLineUseEdgesPoints(ResultRow2, ResultColumn2, 8, &Row3Base,  &Col3Base, &Row4Base, &Col4Base);
					//
					if (0 != ((SelectedXLD1Length-SelectedXLD2Length)>10))
					{
						distance_pl(ResultRow2, ResultColumn2, Row1Base, Col1Base, Row2Base, Col2Base, &Distance);

					}
					else if (0 != ((SelectedXLD1Length-SelectedXLD2Length)<-10))
					{
						distance_pl(ResultRow, ResultColumn, Row3Base, Col3Base, Row4Base, Col4Base, &Distance);
					}
					else
					{
						if (ResultRow.Num()==ResultRow2.Num())
						{
							distance_pp(ResultRow, ResultColumn, ResultRow2, ResultColumn2, &Distance);
						}
						else
						{
							retValue = -1;
							m_vErrorRgn[rId].Reset();
							retValue.Detach(pDefectDiff);
							return S_FALSE;
						}

					}
					if (0 != (Distance.Num()))
					{
						tuple_mean(Distance, &Diameter);
					}
					else
					{
						retValue = -1;
						m_vErrorRgn[rId].Reset();
						retValue.Detach(pDefectDiff);
						return S_FALSE;
					}
					retValue =abs(Diameter[0].D()*m_pixelSize-StdDiameter);
					concat_obj(ErrorReg,rgn,&ErrorReg);
					m_vErrorRgn[rId] = ErrorReg;
				}
			}
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
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDefectDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::DiameterMeasureAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("766$\
					 397;R;FP;0;300;1$\
					 381;B$\
					 866;FP;0.00001;10;0.001;0.02$\
					 767;FP;1;30;0.01;18.8$\
					 643;FP;0.1;20;0.1;1.0$\
					 762;LP;1;100;1;20$\
					 763;LP;5;254;1;40");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CRubberMatAlgo::ScrapDiameterMeasureAlgo(VARIANT* rgnId,VARIANT* pDiameter)
{
	int rId           = (int)rgnId->fltVal;
	//float Offset   = (float)pOffset->fltVal;

	m_vErrorRgn[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDiameter);
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
		Hobject UnionCoCircleContour;
		gen_contour_region_xld (m_centerRegion, &UnionCoCircleContour,"border");
		HTuple Number;
		count_obj(UnionCoCircleContour, &Number);
		if (Number[0].I()==0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDiameter);
			return S_FALSE;
		}
		HTuple Row, Column, Radius, StartPhi, EndPhi, PointOrder;
		Hobject Circle;
		fit_circle_contour_xld(UnionCoCircleContour, "algebraic", -1, 0, 0, 3, 2, &Row, &Column, &Radius, &StartPhi, &EndPhi, &PointOrder);
		gen_circle(&Circle, Row, Column, Radius);

		HTuple Diameter;
		Diameter = Radius*2;
		if (Diameter.Num()==0)
		{
			retValue = -1;
			m_vErrorRgn[rId]=rgn;
			retValue.Detach(pDiameter);
			return S_FALSE;

		}

		retValue = Diameter[0].D();
		m_vErrorRgn[rId] = Circle;
		retValue.Detach(pDiameter);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDiameter);
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDiameter);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::ScrapDiameterMeasureAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1139$\
					 397;R;FP;0;2000;1$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::DeformationDetectAlgo(VARIANT* rgnId,VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize, VARIANT* pDetectDist)
{
	int rId = (int)rgnId->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectDist);
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
		Hobject  Region,RegionDilation;
		//
		HTuple RadiusExtC,AngleExt,DonutWidth,GrayMaxRow,GrayMaxCol;
		HTuple  Width1,Height1;
		//***************************************************************************************
		Hobject ImageReduced,RegionFillUp,RegionClosing,RegionOpening; 
		reduce_domain (Image, rgn, &ImageReduced);
		threshold (ImageReduced, &Region, ThresholdLow, 255);
		fill_up (Region, &RegionFillUp);
		closing_circle (RegionFillUp, &RegionClosing, 23.5);
		opening_circle (RegionClosing, &RegionOpening,OutlierRemoverSize);
		//
		HTuple Area5,Row5,Column5,RadiusDiff;
		area_center(RegionOpening, &Area5, &Row5, &Column5);
		m_centerRegion=RegionOpening;
		if (0 != (Area5<100))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectDist);
			m_vRgn[rId]=RegionOpening;
			return FALSE;
		}
		//*
		Hobject SelectedRegions1,Contours,Circle,Circle1,RegionDifference1,RegionOpening1,RegionDifference2;
		HTuple  Row1,Column1,Radius1,Row2,Column2,Radius2,Row4,Column4,Radius4;
		//*
		boundary (RegionOpening, &SelectedRegions1, "outer");
		dilation_circle(SelectedRegions1, &SelectedRegions1, 2.5);
		erosion_circle(SelectedRegions1, &SelectedRegions1, 1.5);
		//
		smallest_circle(SelectedRegions1, &Row1, &Column1, &Radius1);
		gen_circle(&Circle, Row1, Column1, Radius1);
		difference(Circle, SelectedRegions1, &RegionDifference1);
		opening_circle(RegionDifference1, &RegionOpening1, 50.5);
		inner_circle(RegionOpening1, &Row2, &Column2, &Radius2);
		gen_circle(&Circle1, Row2, Column2, Radius2);
		difference(Circle, Circle1, &RegionDifference2);
		inner_circle(RegionDifference2, &Row4, &Column4, &Radius4);
		RadiusDiff = (Radius4*2.0)-3.5;
		if(RadiusDiff<0)
		{
			RadiusDiff=0.0;
		}
		//
		retValue =RadiusDiff[0].D();
		m_vErrorRgn[rId] = RegionDifference2;
		retValue.Detach(pDetectDist);
		//
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectDist);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::DeformationDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("313$\
					 397;R;FP;0;200;1$\
					 381;B$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//Color image process, Algo.
STDMETHODIMP CRubberMatAlgo::DynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		//为空不报错
		retValue = 0.0;
		m_vErrorRgn[rId].Reset();
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
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::DynThresholdForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("660$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
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
STDMETHODIMP CRubberMatAlgo::EdgeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		//为空不报错
		retValue = 0.0;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
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
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::EdgeForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("661$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 433;LP;1;255;1;30$\
					 434;LP;1;250;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::AnnularRotaDynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		//为空不报错
		retValue = 0.0;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
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
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
		//

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		Hobject ImageMean;
		mean_image (Image, &ImageMean, 9, 9);
		reduce_domain (ImageMean, rgn, &ImageReduced);
		vector_angle_to_rigid(fCenterY,fCenterX, 0, fCenterY, fCenterX, HTuple(RotaAngle).Rad(), &HomMat2D);
		affine_trans_image(ImageReduced, &ImageAffinTrans, HomMat2D, "bilinear", "false");
		dyn_threshold(ImageReduced, ImageAffinTrans, &RegionDynThresh, ThresholdValue, "dark");
		opening_circle(RegionDynThresh,&RegionDynThresh,1.5);
		closing_circle(RegionDynThresh,&RegionDynThresh,3.5);
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
STDMETHODIMP CRubberMatAlgo::AnnularRotaDynThresholdForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("662$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 610;FP;0.5;180;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::CurlNoCompoundForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pGrayValueMean)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		//为空不报错
		retValue = 0.0;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;

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
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pGrayValueMean);
			return S_FALSE;
		}
		HTuple MeanGray, Deviation;
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
		m_vErrorRgn[rId] = rgn;
		retValue.Detach(pGrayValueMean);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::CurlNoCompoundForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("816$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CRubberMatAlgo::my_disp_obj(const Halcon::Hobject Object)
{
	if (m_bPilotImg)
	{
		return;
	}
	std::string mode("visible");
	std::string strMachine("");
	set_check("~father");
	HTuple Width, Height;
	get_image_size (m_crtImg, &Width, &Height);
	if(m_WindowFirst)
	{
		open_window(1,1,Width/2,Height/2,(Hlong)NULL, mode.c_str(),strMachine.c_str(), (Hlong*)&m_CHImgWndID);
		m_WindowFirst=false;
		disp_obj(m_crtImg,m_CHImgWndID);
	}
	set_part(m_CHImgWndID,0, 0, Width-1, Height-1);
	set_check("father");
	set_draw(m_CHImgWndID,"margin"); 
	set_color(m_CHImgWndID, "red");
	set_line_width(m_CHImgWndID,HTuple(1));
	disp_obj(Object,m_CHImgWndID);

	write_object(Object, "D:/Region3.hobj");
	write_image(m_crtImg, "tiff", 0,"D:/Image.tiff");
}

STDMETHODIMP CRubberMatAlgo::ThreeCirclesLocationMorphologyAlgo(VARIANT* rgnId, VARIANT* pRegionRadius, VARIANT* pSobelSize, VARIANT* pMinGrayCircle, VARIANT* pMinGrayChar, VARIANT* pDistanceCo, VARIANT* pRecLength, VARIANT* pRecWidth, VARIANT* pFeature)
{
	//Detect the angle of the central circle, applying morphology
	int id = (int)rgnId->fltVal;
	int RegionRadius = (int)pRegionRadius->fltVal;
	int SobelSize = (int)pSobelSize->fltVal;
	int MinGrayCircle = (int)pMinGrayCircle->fltVal;
	int MinGrayChar = (int)pMinGrayChar->fltVal;
	float DistanceCo = (float)pDistanceCo->fltVal;
	int RecLength = (int)pRecLength->fltVal;
	int RecWidth = (int)pRecWidth->fltVal;
	int Feature = (int)pFeature->fltVal;	
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
	Hobject newRgn;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG
	try
	{
#endif
		if(SobelSize % 2 != 1 )
		{
			m_vRgn[id] = newRgn;
			return S_FALSE;
		}
		
		Hobject ImageReduced;
		Hobject Image=m_crtImg;
		Hobject SearchRegion, EdgeAmplitude, Edges, EdgesConnected, Edges1, ClosedEdges;
		Hobject UniEdges, EdgesFillUp, EdgesFillUp2, EdgesOpen, EdgesClosed, SelectedEdges, C;
		HTuple  Row1, Column1, Radius1;

		gen_circle(&SearchRegion,fCenterY, fCenterX, RegionRadius);
		reduce_domain(Image, SearchRegion, &ImageReduced);
		concat_obj(m_vRgn[id], SearchRegion, &m_vRgn[id]);
		sobel_amp(ImageReduced, &EdgeAmplitude, "sum_sqrt", SobelSize);
		threshold(EdgeAmplitude, &Edges, MinGrayCircle, 255);
		connection(Edges, &EdgesConnected);
		select_shape(EdgesConnected, &Edges1, (HTuple("width").Append("height")), "or", (HTuple(5).Append(5)), (HTuple(175).Append(175)));
		close_edges_length(Edges1, EdgeAmplitude, &ClosedEdges, 1, 12);
		union1(ClosedEdges, &UniEdges);
		fill_up(UniEdges, &EdgesFillUp);
		connection(EdgesFillUp, &EdgesFillUp2);
		opening_circle(EdgesFillUp2, &EdgesOpen, 25);
		closing_circle(EdgesOpen, &EdgesClosed, 30);
		select_shape(EdgesClosed, &SelectedEdges, "circularity", "and", 0.9, 1);
		HTuple Length;
		count_obj(SelectedEdges, &Length);
		if(Length != 3)
		{
			m_vRgn[id] = newRgn;
			return S_FALSE;
		}
        m_circlesNumber = Length;
		smallest_circle(SelectedEdges, &Row1, &Column1, &Radius1);
		HTuple Angle;
		//gen_empty_obj(&Circles);
		Hobject Rectangle, Rectangles, Circles, Pattern, Patterns, Characters;
		HTuple Number, PatternNumber, Roww, Coll, Phi, Result;
		threshold(EdgeAmplitude, &Characters, MinGrayChar, 255);
		for (int i=0; i<3; i++)
		{
			gen_circle(&C, Row1.Select(i), Column1.Select(i), Radius1.Select(i));
			concat_obj(Circles, C, &Circles);
			Phi = (fCenterY - Row1.Select(i)).Atan2(Column1.Select(i) - fCenterX);
			Angle[i] = Phi;
			if(Feature != 0)
			{
      	        gen_rectangle2(&Rectangle, DistanceCo * fCenterY - (DistanceCo - 1) * Row1.Select(i), DistanceCo * fCenterX - (DistanceCo - 1) * Column1.Select(i), Phi + HTuple(90).Rad(), RecLength, RecWidth);
			    concat_obj(Rectangles, Rectangle, &Rectangles);
			    intersection(Rectangle, Characters, &Pattern);
			    area_center(Pattern, &Number, &Roww, &Coll);
			    concat_obj(Patterns, Pattern, &Patterns);
			    PatternNumber[i] = Number;
		    }
		}
		if(Feature == 1)
		{
			Result = PatternNumber.Find(PatternNumber.Min());
		}
		else if(Feature == 2)
		{
			Result = PatternNumber.Find(PatternNumber.Max());
		}
		else
		{
			Result = 0;
	    }
		Hobject SelectedRectangle, SelectedPattern, SelectedCircle;
		if(Feature != 0)
		{
	    select_obj(Rectangles, &SelectedRectangle, Result + 2);
		concat_obj(m_vRgn[id], SelectedRectangle, &m_vRgn[id]);
		select_obj(Patterns, &SelectedPattern, Result + 2);
		concat_obj(m_vRgn[id], Patterns, &m_vRgn[id]);
		}
		select_obj(Circles, &SelectedCircle, Result + 2);
		concat_obj(m_vRgn[id], SelectedCircle, &m_vRgn[id]);
		m_UniqueCirclesRegion = SelectedCircle;
		m_modelAngle = Angle[Result];
		m_phiValue = Angle[Result];
		Hobject Line;
		gen_region_line(&Line, fCenterY, fCenterX, Row1.Select(Result), Column1.Select(Result));
		concat_obj(m_vRgn[id], Line, &m_vRgn[id]);
		m_threeCirclesRegion = Circles;
		m_InnerCirclesRegion = Circles;
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
STDMETHODIMP CRubberMatAlgo::ThreeCirclesLocationMorphologyAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1344$\
					 381;B$\
					 1345;LP;1;200;1;120$\
					 1346;LP;3;13;2;5$\
					 1347;LP;0;255;1;25$\
					 1352;LP;0;255;1;25$\
					 1348;FP;0.5;5;0.05;2.3$\
					 1349;LP;2;100;1;50$\
					 1350;LP;2;100;1;20$\
					 1351;LP;0;2;1;2");
					 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::OuterRingLocationAlgo3(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pThredValue,VARIANT*pGrayValue,VARIANT* pDilationSize)
{
	//抠除外环三个凸点
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float Radius =(float)pRadius->fltVal;
	float ThredValue    = (float)pThredValue->fltVal;
	float GrayValue    = (float)pGrayValue->fltVal;
	float DilationSize = (float)pDilationSize->fltVal;
	float InnerRadius = Radius - RoiWidth;
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_FirstCenterX;
	fCenterY = m_FirstCenterY;
	HTuple ImgWidth=m_imgWidth;
	HTuple ImgHeight=m_imgHeight;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject Image, OuterCircle, InnerCircle, Ring;
		Hobject ImageReduced,DiffOfGauss,ImageConverted;
		Image=m_crtImg;
		if(Radius < RoiWidth)
		{
			m_vRgn[id] = newRgn;
			return S_FALSE;
		}
		gen_circle(&OuterCircle,fCenterY, fCenterX, Radius);
		gen_circle(&InnerCircle, fCenterY, fCenterX, InnerRadius);
		difference(OuterCircle, InnerCircle, &Ring);
		reduce_domain(Image, Ring, &ImageReduced);
		Hobject ImageAffinTrans,RegionDynThresh,RegionClosing1,RegionOpening4,RegionFillUp1;
		HTuple HomMat2D;
		vector_angle_to_rigid(fCenterY, fCenterX, HTuple(10).Rad(), fCenterY, fCenterX, 0, &HomMat2D);
		affine_trans_image(ImageReduced, &ImageAffinTrans, HomMat2D, "constant", "false");
		dyn_threshold(ImageReduced, ImageAffinTrans, &RegionDynThresh, ThredValue, "dark");
		closing_circle(RegionDynThresh, &RegionClosing1, 7.5);
		fill_up_shape(RegionClosing1, &RegionFillUp1, "area", 1, 500);
	//	close_edges_length(RegionFillUp1, ImageGray, &ClosedEdges, 20, 8)
		opening_circle(RegionFillUp1, &RegionOpening4, 7.5);

		HTuple HomMat2D120,HomMat2D_120;
		Hobject RegionAffineTrans1,RegionAffineTrans,RegionUnion1,ThreeCirclesRegion;
		vector_angle_to_rigid(fCenterY, fCenterX, HTuple(120).Rad(), fCenterY, fCenterX, 0, &HomMat2D120);
		vector_angle_to_rigid(fCenterY, fCenterX, HTuple(-120).Rad(), fCenterY, fCenterX, 0, &HomMat2D_120);
		//异常判断，剔除大黑点
		Hobject ConnectedRegions1,CircleRegs,ObjectSelected,RegionAffineTrans2;
		HTuple Number,Mean, Deviation,Mean1,Deviation1,Deviation2,MeanDiff,MeanDiff2,Mean2;
		connection(RegionOpening4, &ConnectedRegions1);
		count_obj(ConnectedRegions1, &Number);
		gen_empty_region(&CircleRegs);
		if(Number>=1)
		{
			for (int i=1;i<=Number; i++)
			{
				select_obj(ConnectedRegions1, &ObjectSelected, i);
				affine_trans_region(ObjectSelected, &RegionAffineTrans1, HomMat2D_120, "nearest_neighbor");
				affine_trans_region(ObjectSelected, &RegionAffineTrans2, HomMat2D120, "nearest_neighbor");

				intensity(RegionAffineTrans1, ImageReduced, &Mean1, &Deviation1);
				intensity(RegionAffineTrans2, ImageReduced, &Mean2, &Deviation2);
				intensity(ObjectSelected, ImageReduced, &Mean, &Deviation);
				MeanDiff = (Mean1-Mean).Abs();
				MeanDiff2 = (Mean2-Mean).Abs();
				if (MeanDiff<GrayValue && MeanDiff2<GrayValue)
				{
					concat_obj(ObjectSelected, CircleRegs, &CircleRegs);
				}
			}
		}
		else
		{
			
			m_vRgn[id] = Ring;
			return S_FALSE;
		}
		union1(CircleRegs, &CircleRegs);
		affine_trans_region(CircleRegs, &RegionAffineTrans1, HomMat2D_120, "nearest_neighbor");
		affine_trans_region(CircleRegs, &RegionAffineTrans, HomMat2D120, "nearest_neighbor");
		union2(RegionAffineTrans1, RegionAffineTrans, &RegionUnion1);
		union2(RegionUnion1, CircleRegs, &RegionUnion1);

		//
		Hobject ConnectedRegions,ThreeCircleRegs,RegIntersection;
		HTuple Number1;
		union1(RegionUnion1,&RegionUnion1);
		intersection(RegionUnion1,Ring,&RegIntersection);
		connection(RegIntersection, &ConnectedRegions);
		count_obj(ConnectedRegions, &Number1);
		Hobject RegionDiff3, UniqueCircleRegion, SmallCircle0, SmallCircle1, SmallCircle2;
	    HTuple AreaUnique, RowUnique, ColumnUnique, RowUnique1, ColumnUnique1;
		if (Number1==3)
		{
			dilation_circle(RegIntersection,&ThreeCirclesRegion,DilationSize);
			ThreeCircleRegs = ThreeCirclesRegion;
			difference(Ring,ThreeCircleRegs,&RegionDiff3);
			m_vRgn[id] = RegionDiff3;
		} 
		else if(Number1>3)
		{
			Hobject SortedRegions,ObjectSelected1,ObjectSelected2,ObjectSelected3,RegionUnion2;
			sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "row");
			select_obj(SortedRegions, &ObjectSelected1, 1);
			select_obj(SortedRegions, &ObjectSelected2, 2);
			select_obj(SortedRegions, &ObjectSelected3, 3);
			union2(ObjectSelected1, ObjectSelected2, &RegionUnion2);
			union2(RegionUnion2, ObjectSelected3, &RegionUnion2);
			union1(RegionUnion2,&RegionUnion2);
			//
			dilation_circle(RegionUnion2,&ThreeCirclesRegion,DilationSize);
			ThreeCircleRegs = ThreeCirclesRegion;
			difference(Ring,ThreeCircleRegs,&RegionDiff3);
			m_vRgn[id]=RegionDiff3;
			/*concat_obj(m_vRgn[id], ConnectedRegions, &m_vRgn[id]);*/
		}
		else
		{
			//如果无法检测到外圈小圆，用中心三个圆估计
			UniqueCircleRegion = m_UniqueCirclesRegion;
			area_center(UniqueCircleRegion, &AreaUnique, &RowUnique, &ColumnUnique);
			RowUnique1 = fCenterY + (RowUnique - fCenterY) * (Radius - 0.5 * RoiWidth) / ((RowUnique - fCenterY).Pow(2) + (ColumnUnique - fCenterX).Pow(2)).Sqrt();
			ColumnUnique1 = fCenterX + (ColumnUnique - fCenterX) * (Radius - 0.5 * RoiWidth) / ((RowUnique - fCenterY).Pow(2) + (ColumnUnique - fCenterX).Pow(2)).Sqrt();
			gen_circle(&SmallCircle0, RowUnique1, ColumnUnique1, DilationSize + 15);
			affine_trans_region(SmallCircle0, &SmallCircle1, HomMat2D120, "nearest_neighbor");
			affine_trans_region(SmallCircle0, &SmallCircle2, HomMat2D_120, "nearest_neighbor");
			union2(SmallCircle0, SmallCircle1, &ThreeCirclesRegion);
			union2(SmallCircle2, ThreeCirclesRegion, &ThreeCirclesRegion);
			difference(Ring, ThreeCirclesRegion, &ThreeCircleRegs);
			//concat_obj(UniqueCircleRegion, ThreeCircleRegs, &ThreeCircleRegs);
			m_vRgn[id] = ThreeCircleRegs;
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
STDMETHODIMP CRubberMatAlgo::OuterRingLocationAlgo3Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 194;FP;50;1500;0.5;220$\
					 827;FP;1;200;0.5;40$\
					 749;FP;2.0;50.0;0.5;15$\
					 1268;FP;2;100.0;0.5;30$\
					 1121;FP;3;20;0.5;3.5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
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
STDMETHODIMP CRubberMatAlgo::SetCurrentTaskName2(BSTR* bstrTaskName,BSTR* bstrChName)
{   
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+*bstrChName+L"\\"+m_strTaskName;
	m_strTaskPath = CW2A(m_wstrTaskPath.c_str());
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::OuterRingLocationPCAlgo(VARIANT* rgnId, VARIANT* pOuterRadius, VARIANT* pRingWidth, VARIANT* pMode, VARIANT* pDilationSize, VARIANT* pStdAngle, VARIANT* pOutputRadius, VARIANT* pOutputWidth)
{
	int rId = (int) rgnId->fltVal;
	int Mode = (int) pMode->fltVal;
	float OuterRadius = (float) pOuterRadius->fltVal;
	float RingWidth = (float) pRingWidth->fltVal;
	float DilationSize = (float) pDilationSize->fltVal;
	float StdAngle = (float) pStdAngle->fltVal;
	float OutputRadius =(float) pOutputRadius->fltVal;
	float OutputWidth = (float) pOutputWidth->fltVal;
    float InnerRadius = OuterRadius - RingWidth;
	float OutputInnerRadius = OutputRadius - OutputWidth;
	//m_vWhiteErrorRgn[rId].Reset();
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}	
		Hobject ImageReduced;		
		string ModelImgFile = m_strTaskPath;

		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image=m_crtImg;		
		Hobject PolarTransImage;	
		float fCenterX, fCenterY, Circumference;
		fCenterX = m_FirstCenterX;
		fCenterY = m_FirstCenterY;
		float EndRadius = OuterRadius - RingWidth;
		Circumference = 2 * 3.14159 * (OuterRadius - 0.5 * RingWidth);
		//将图像从极坐标转换到直角坐标
		polar_trans_image_ext(Image, &PolarTransImage,fCenterY, fCenterX, 0,  HTuple(360).Rad(), OuterRadius, EndRadius, Circumference, RingWidth, "nearest_neighbor"  );
		
		if(_access(ModelImgFile.c_str(),0))
		{
			CreateDirectoryA(ModelImgFile.c_str(), NULL);
			ModelImgFile.append("\\");
			ModelImgFile.append("ModelImagePC");
			ModelImgFile.append(".tiff");
		}
		Hobject OuterCircle, InnerCircle, Ring;
		gen_circle(&OuterCircle, fCenterY, fCenterX, OuterRadius);
		gen_circle(&InnerCircle, fCenterY, fCenterX, InnerRadius);
		difference(OuterCircle, InnerCircle, &Ring);
		if(Mode == 0)
		{         
			//创建模板图片      	
			//test show
			write_image(PolarTransImage, "tiff", 0, ModelImgFile.c_str());	
			Hobject Circle0, Circle1, Circle2, ModelRegion;
			gen_circle(&Circle0, fCenterY - (OuterRadius - 0.5 * RingWidth) * (HTuple(StdAngle).Rad()).Sin(), fCenterX + (OuterRadius - 0.5 * RingWidth) * (HTuple(StdAngle).Rad()).Cos(), 20);
			gen_circle(&Circle1, fCenterY - (OuterRadius - 0.5 * RingWidth) * (HTuple(StdAngle + 120).Rad()).Sin(), fCenterX + (OuterRadius - 0.5 * RingWidth) * (HTuple(StdAngle + 120).Rad()).Cos(), 20);
			gen_circle(&Circle2, fCenterY - (OuterRadius - 0.5 * RingWidth) * (HTuple(StdAngle + 240).Rad()).Sin(), fCenterX + (OuterRadius - 0.5 * RingWidth) * (HTuple(StdAngle + 240).Rad()).Cos(), 20);
			union2(Circle0, Circle1, &ModelRegion);
			union2(Circle2, ModelRegion, &ModelRegion);
			m_ModelRegion = ModelRegion;
			difference(Ring, ModelRegion, &ModelRegion);
		//	union2(ModelRegion, Ring, &ModelRegion);
			//gen_region_line(&Show, fCenterY, fCenterX, 568, 190);
			m_vRgn[rId] = ModelRegion;
			m_OldCenterY = fCenterY;
			m_OldCenterX = fCenterX;
		}
		else 
		{
			if(_access(ModelImgFile.c_str(),0))
			{		
				return S_FALSE;
			}
			//相位相关计算
			Hobject ModelImage;
			read_image(&ModelImage, ModelImgFile.c_str());
			HTuple Maximum, Area, RowMax, ColMax, Angle;
			HTuple Width, Height,ModelWidth, ModelHeight;
			Hobject ImageFFT, ImagePhaseCorrelationFFT, ImageFFTModel, ImagePhaseCorrelation;
			get_image_size(PolarTransImage, &Width, &Height);
			get_image_size(ModelImage,&ModelWidth,&ModelHeight);
			
			if(Width != ModelWidth || Height != ModelHeight)
			{
				m_vErrorRgn[rId].Reset();
				return S_FALSE;
			}
			HTuple HomMat2D;			
			rft_generic(PolarTransImage, &ImageFFT, "to_freq", "none", "complex", Width);
			rft_generic(ModelImage, &ImageFFTModel, "to_freq", "none", "complex", Width);
			phase_correlation_fft(ImageFFTModel, ImageFFT, &ImagePhaseCorrelationFFT);
			rft_generic(ImagePhaseCorrelationFFT, &ImagePhaseCorrelation, "from_freq", "n", "real", Width);
			Hobject Region, Maxi,ShowRegion;
			gen_rectangle1(&Region, 0, 0, 1, Width);
			gray_features(Region, ImagePhaseCorrelation, "max", &Maximum);

			if(Maximum.Num() == 1)
			{
				threshold(ImagePhaseCorrelation, &Maxi, Maximum, Maximum);
				area_center(Maxi, &Area, &RowMax, &ColMax);
				Angle = ColMax / Circumference * 360 ;
			    
				vector_angle_to_rigid(m_OldCenterY,m_OldCenterX,0,fCenterY, fCenterX, HTuple(Angle).Rad(),&HomMat2D);
				affine_trans_region(m_ModelRegion, &ShowRegion, HomMat2D, "nearest_neighbor");
				dilation_circle(ShowRegion, &ShowRegion, DilationSize);
				Hobject OutputCircle0, OutputCircle1, Ring1;
				gen_circle(&OutputCircle0, fCenterY, fCenterX, OutputRadius);
				gen_circle(&OutputCircle1, fCenterY, fCenterX, OutputRadius - OutputWidth);
				difference( OutputCircle0, OutputCircle1, &Ring1);
				difference(Ring1, ShowRegion, &ShowRegion);			
				m_vRgn[rId] = ShowRegion;
			}
			else
			{
				m_vRgn[rId] = Ring ;
				return S_FALSE;
			}			
		}
	

	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo:: OuterRingLocationPCAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1353$\
					 167;B$\
					 194;FP;100;500;1;198$\
					 413;FP;1;50;0.5;13$\
					 1340;LP;0;1;1;0$\
					 1121;FP;0;40;1;25$\
					 1354;FP;0;120;0.5;0$\
					 1355;FP;0;500;0.5;200$\
					 1356;FP;0;200;0.5;20"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CRubberMatAlgo::SideCenterLocationTiltingAlgo(VARIANT* rgnID ,VARIANT*pRubberMatHeight,VARIANT* pContrast, VARIANT* pErosionSide, VARIANT* pErosionTop)
{
	int id = (int)rgnID->fltVal;
	int RubberMatHeight=(int)pRubberMatHeight->fltVal;
	int Contrast=(int)pContrast->fltVal;
	int ErosionSide=(int)pErosionSide->fltVal;
	int ErosionTop=(int)pErosionTop->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
#ifdef NDEBUG        
	try
	{
#endif
		Hobject  Image = m_crtImg;
		Hobject Region, RgionFillUp, ConnectedRegions, SelectedRegions, RegionAffineTrans, RegionBorder, Body;
		Hobject RegionFillUp, NormalLine, IntersectionReg, Intersection, CropRegion, RegionMoved;
		HTuple Rows, Cols, Indices, Ind0, Length0, StartRow0, StartCol0, EndRow0, EndCol0, HomMat2D, Ind1;
		HTuple Areas, RowPoint, ColPoint, Length2, CoY, Number0;	
		get_image_size(Image,&m_ImageWidth,&m_ImageHeight);
		threshold(Image, &Region, Contrast, 255);
		fill_up(Region, &RegionFillUp);
		connection(RegionFillUp, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", 1000, 999999);	
		count_obj(SelectedRegions, &Number0);
		if(Number0 != 1)
		{
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y =0.0;
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		get_region_contour(SelectedRegions, &Rows, &Cols);
		tuple_sort_index(Cols, &Indices);
		tuple_find(Cols, Cols.Select(Indices.Select(0)), &Ind0);
		tuple_length(Cols, &Length0);
		StartRow0 = (Rows.Select(Ind0)).Min();
		StartCol0 = Cols.Select(Indices.Select(0));
		tuple_find(Cols, Cols.Select(Indices.Select(Length0 -1)), &Ind1);
		EndRow0 = (Rows.Select(Ind1)).Min();
		EndCol0 = Cols.Select(Indices.Select(Length0 - 1));
		vector_angle_to_rigid(0, 0, 0, RubberMatHeight, 0, 0, &HomMat2D);
		affine_trans_region(SelectedRegions, &RegionAffineTrans, HomMat2D, "constant");
		gen_region_polygon_filled(&Body, (((StartRow0.Concat(EndRow0)).Concat(EndRow0 + RubberMatHeight)).Concat(StartRow0 + RubberMatHeight )).Concat(StartRow0),
			(((StartCol0.Concat(EndCol0)).Concat(EndCol0)).Concat(StartCol0)).Concat(StartCol0));
		union2(RegionAffineTrans, Body, &Body);
		difference(Body, SelectedRegions, &Body);
		//erosion
		gen_rectangle1(&CropRegion, 0, StartCol0 + ErosionSide, m_ImageHeight, EndCol0 - ErosionSide);
		intersection(Body, CropRegion, &Body);
		move_region(SelectedRegions, &RegionMoved, ErosionTop, 0);
		difference(Body, RegionMoved, &Body);
		boundary(Body, &RegionBorder, "outer");
		gen_region_line(&NormalLine, StartRow0, 0.5 * (StartCol0 + EndCol0), StartRow0 + RubberMatHeight + 30, 0.5 * (StartCol0 + EndCol0));
		intersection(RegionBorder, NormalLine, &IntersectionReg);
		connection(IntersectionReg, &IntersectionReg);
		area_center(IntersectionReg, &Areas, &RowPoint, &ColPoint);
	//	boundary(SelectedRegions, &EllipseBorder, "outer");
		intersection(SelectedRegions, RegionBorder, &Intersection);
		m_BaseRegionLine = Intersection;
		m_centerLocationRegion = Body;
		if(RowPoint.Num() == 2)
		{
			m_vPos[0].m_x = 0.5 * (StartCol0[0].D() + EndCol0[0].D());
			CoY = 0.5 * (RowPoint.Select(0) + RowPoint.Select(1));
			m_vPos[0].m_y = (float) CoY[0].D();
		}
		m_vRgn[id] = Body;	
			if(m_bDebugImg)
			{
				concat_obj (m_vRgn[id], newRgn, &m_vRgn[id]);
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
STDMETHODIMP CRubberMatAlgo::SideCenterLocationTiltingAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1383$\
					 381;B$\
					 1309;LP;10;200;1;110$\
					 989;LP;0;255;1;180$\
					 994;LP;0;100;1;0$\
					 995;LP;0;30;1;0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

bool ParseModelPath(string strTargetFile, string& strModelPath, string& strModelName)
{
	if(strTargetFile == "")
	{
		return false;
	}
	string strTmp = strTargetFile;
	size_t index = strTmp.find_last_of("\\");
	if(index != string::npos)
	{
		strModelPath = strTmp.substr(0, index);
	}

	index = strModelPath.find_last_of("\\");
	if(index != string::npos)
	{
		strModelName = strModelPath.substr(index + 1);
	}
	return true;
}
bool CRubberMatAlgo::ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA)
{
	(*ModelPARA).Reset();
	(*ModelPARA)[0]=0.0;
	(*ModelPARA)[1]=0.0;
	(*ModelPARA)[2]=0.0;
	(*ModelPARA)[3]=0.0;
	(*ModelPARA)[4]=0.0;
	(*ModelPARA)[5]=0.0;
	(*ModelPARA)[6]=0.0;
	USES_CONVERSION;

	string datname = strModelPath;
	if (!_access(datname.c_str(),0))
	{
		TCHAR vv[64];
		GetPrivateProfileString(_T("RegionData"),_T("ModelRadius"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
		(*ModelPARA)[0]= _tstof(vv);
		GetPrivateProfileString(_T("RegionData"),_T("Row"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
		(*ModelPARA)[1]= _tstof(vv);
		GetPrivateProfileString(_T("RegionData"),_T("Col"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
		(*ModelPARA)[2]= _tstof(vv);
		GetPrivateProfileString(_T("RegionData"),_T("Phi"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
		(*ModelPARA)[3]= _tstof(vv);
	}
	else 
	{
		return false;
	}
	if(strExactModelPath.size() > 0)
	{
		datname = strExactModelPath;
		if (!_access(datname.c_str(),0))
		{
			TCHAR vv[64];
			GetPrivateProfileString(_T("RegionData"),_T("Row"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
			(*ModelPARA)[4]= _tstof(vv);
			GetPrivateProfileString(_T("RegionData"),_T("Col"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
			(*ModelPARA)[5]= _tstof(vv);
			GetPrivateProfileString(_T("RegionData"),_T("Phi"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
			(*ModelPARA)[6]= _tstof(vv);
		}
		else 
		{
			return false;
		}
	}
	return true;
}

bool CRubberMatAlgo::ReadModelsForOcr( BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,bool errorInfoLanguage,HTuple &m_modelsForOcr,HTuple& m_matrixForOcr,Hobject& m_modelImagesForOcr)
{
	CString sModelPath;
	string strModelPath;
	HTuple modelID;
	HTuple modelPARA;
	Hobject ImageOCR;
	modelPARA[0] =0;
	modelPARA[1] =0;
	modelPARA[2] =0;
	modelPARA[3] =0;
	modelPARA[4] =0;
	modelPARA[5] =0;
	modelPARA[6] =0;
	int modelNumber=0;
	//gen_empty_obj(&m_imageReg);
	for(int pathNum=1;pathNum<=4;pathNum++)
	{
		if(pathNum==1)
		{
			sModelPath  = *bstrShm1File;
			strModelPath = CT2A(sModelPath);
		}
		else if(pathNum==2)
		{
			sModelPath  = *bstrShm2File;
			strModelPath = CT2A(sModelPath);
		}
		else if(pathNum==3)
		{
			sModelPath  = *bstrShm3File;
			strModelPath = CT2A(sModelPath);
		}
		else if(pathNum==4)
		{
			sModelPath  = *bstrShm4File;
			strModelPath = CT2A(sModelPath);
		}
		if(sModelPath.GetLength()<5)
		{
			continue;
		}
		Hobject ModelImages,ImageReg;

		if(!_access(strModelPath.c_str(),0))
		{ 
			read_shape_model(strModelPath.c_str(), &modelID);
			m_modelsForOcr[modelNumber] = modelID;

			//
			string strPath,strModelName;
			ParseModelPath(strModelPath,strPath, strModelName);
			string strModelImg = strPath + "\\" + strModelName + ".tiff";
			if (!_access(strModelImg.c_str(),0))
			{
				read_image(&ImageOCR,strModelImg.c_str());
				HTuple Class;
				get_obj_class(m_modelImagesForOcr, &Class);
				if(Class!=HTuple("image"))
				{
					channels_to_image(ImageOCR, &m_modelImagesForOcr);
					//inspect_shape_model (ImageOCR, &ModelImages, &ImageReg, 1,  ((HTuple(20).Append(35)).Append(8)));
					//concat_obj(m_imageReg,ImageReg,&m_imageReg);
				}
				else
				{
					append_channel(m_modelImagesForOcr, ImageOCR, &m_modelImagesForOcr);
					//inspect_shape_model (ImageOCR, &ModelImages, &ImageReg, 1,  ((HTuple(20).Append(35)).Append(8)));
					//concat_obj(m_imageReg,ImageReg,&m_imageReg);
				}
			}
			else
			{
				if(errorInfoLanguage)
				{
					MessageBox(NULL, _T("初始化参数（模板图片路径）设置错误"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL, _T("Initialization parameter（model image path）setting is wrong!"),_T("Initial configuration is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				return false;
			}
			string strModelDat = strPath + "\\" + strModelName + ".dat";
			if(ReadShapeModelAxieData(strModelDat, "" , &modelPARA))
			{
				set_value_matrix(m_matrixForOcr, modelNumber, 0, modelPARA[0]);
				set_value_matrix(m_matrixForOcr, modelNumber, 1, modelPARA[1]);
				set_value_matrix(m_matrixForOcr, modelNumber, 2, modelPARA[2]);
				set_value_matrix(m_matrixForOcr, modelNumber, 3, modelPARA[3]);
			}
			else
			{
				if(errorInfoLanguage)
				{
					MessageBox(NULL,_T("初始化参数,模板图像对应的‘dat’文件不存在"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Initialization parameter, 'dat’file corresponding to model image isn't exist!"),_T("Initialization error："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				return false;
			}
			modelNumber++;
		}
		else
		{
			if(errorInfoLanguage)
			{
				MessageBox(NULL,_T("请检查模板初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("Please check whether the initial model path is correct or not."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			continue;
		}

	}
	if(modelNumber<1)
		return false;
	else
		return true;
}
STDMETHODIMP CRubberMatAlgo::MultOcrDetectAlgo(VARIANT* rgnId, VARIANT* pGreediness, BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,VARIANT* pMatchingResult)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn      = m_vRgn[rId];
	CComVariant retValue;
	float greediness = (float)pGreediness->fltVal;
	int searchRadius=(int)(m_ringPara[rId].m_Radius/5.0);

	if(searchRadius<15)
	{
		searchRadius=15;
	}
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg||m_resetModelForMultOcr)
		{
			//读取信息前，先清空模板相关数据。

			m_modelsForOcr=HTuple();
			create_matrix(10, 7, 0, &m_matrixForOcr);
			gen_empty_obj(&m_modelImagesForOcr);
			m_readModelState=ReadModelsForOcr(bstrShm1File,bstrShm2File,bstrShm3File, bstrShm4File,m_isChineseLanguage,m_modelsForOcr,m_matrixForOcr,m_modelImagesForOcr);
			m_resetModelForMultOcr=false;
		}
		//
		if(rgn.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			return S_FALSE;
		}
		//
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		if((fCenterX<2.0)||(fCenterY<2.0))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			return S_FALSE;
		}
		if(!m_readModelState)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			return S_FALSE;
		}
		//

		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//

		Hobject ImageReduced,ImageReducedForSearch,DetectRegion,SearchRegion;
		HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;
		Hobject Image=m_crtImg;
		DetectRegion=rgn;
		gen_circle(&SearchRegion, fCenterY, fCenterX, searchRadius);
		reduce_domain(Image, SearchRegion, &ImageReducedForSearch);
		//
		find_shape_models(ImageReducedForSearch,m_modelsForOcr,0, HTuple(360).Rad(), 0.5, 1,0.5, "least_squares",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M, &Model_M);
		if (Score_M>0.05)
		{
			retValue = Score_M[0].D();		
		} 
		else
		{
			retValue = 0;
		}
		m_vErrorRgn[rId] = rgn;
		retValue.Detach(pMatchingResult);

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pMatchingResult);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CRubberMatAlgo::MultOcrDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("636$\
					 440;R;FP;0.0;1.0;0.01;>=#0.5$\
					 381;B$\
					 322;FP;0.2;1.0;0.05;0.85$\
					 535;SPM$\
					 535;SPM$\
					 535;SPM$\
					 535;SPM");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


