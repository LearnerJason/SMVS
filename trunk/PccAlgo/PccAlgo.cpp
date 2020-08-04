#include "stdafx.h"
#include "resource.h"
#include "PccAlgo_i.h"
#include "dllmain.h"
#include "PccAlgo.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "../ImageProcessLib/ImageProcessLib.h"
using namespace EoeShell;

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


// CanAlgo.cpp : CCanAlgo  Implementation
STDMETHODIMP CPccAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IPccAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CPccAlgo::SetCurrentImage(LONG* pImg)
{
	Hobject* pObj = (Hobject*)pImg;
	m_crtImg      = *pObj;
	//HTuple Width,Height;
	//get_image_size (m_crtImg, &Width, &Height);
	//ResizeImage(m_crtImg,&m_crtImg,12000,Width,Height);
	m_bDebugImg   = ImageProcessAlg.IsDebugImage(m_crtImg);	
	m_bPilotImg   = ImageProcessAlg.IsPilotImage(m_crtImg);
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
STDMETHODIMP CPccAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CPccAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	//Hobject* pObj = (Hobject*)pRgn;
	//*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CPccAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CPccAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CPccAlgo::ReloadExternalFile(BSTR* bstrPath)
{
	CString sFilePath = *bstrPath;
	string strFile    = CT2A(sFilePath);
	if(strFile.find(".shm") != string::npos)
	{
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
STDMETHODIMP CPccAlgo::GetCenterLocation(BSTR* bstrLocationXY)
{
	//当前定位中心点，角度默认为0度。
	CString cstrXY;
	cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_vPos[0].m_x,m_vPos[0].m_y,0.0);
	//
	m_modelPARA[0]=m_vPos[0].m_y;
	m_modelPARA[1]=m_vPos[0].m_x;
	CComBSTR strLocationXY = cstrXY.AllocSysString();
	HRESULT hr = strLocationXY.CopyTo(bstrLocationXY);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//
STDMETHODIMP CPccAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{

	return S_OK;
}

STDMETHODIMP CPccAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{

	return S_FALSE;
}
STDMETHODIMP CPccAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}
STDMETHODIMP CPccAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}
STDMETHODIMP CPccAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{

	return S_OK;
}
STDMETHODIMP CPccAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{

	return S_OK;
}
STDMETHODIMP CPccAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{

	return S_OK;
}

STDMETHODIMP CPccAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
//
void CPccAlgo::ResizeImage(Hobject Image, Hobject* ImageNew,HTuple withStand,HTuple Width,HTuple Height)
{
	// Local iconic variables 
	Hobject  Rectangle, ImageReduced1, Region;
	Hobject  ConnectedRegions, SelectedRegions, RegionUnion;
	Hobject  RegionOpening, ImageZoomed;
	// Local control variables 
	HTuple   AreaDetect, Row3, Column3;
	HTuple  ScaleTest, AreaModel, HomMat2DZOOM;

	gen_rectangle1(&Rectangle, Height/2.0, 10, (Height/2.0)+30, Width-10);
	reduce_domain(Image, Rectangle, &ImageReduced1);
	threshold(ImageReduced1, &Region, 30, 255);
	connection(Region, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, "area", "and", 8000, 999999);
	union1(SelectedRegions, &RegionUnion);
	opening_rectangle1(RegionUnion, &RegionOpening, 1, 20);
	area_center(RegionOpening, &AreaDetect, &Row3, &Column3);
	if(AreaDetect>10)
	{
		ScaleTest = withStand/(AreaDetect+0.001);
		vector_angle_to_rigid(Height/2.0,HTuple(Column3[0]), 0, Height/2.0,HTuple(Column3[0]), 0, &HomMat2DZOOM);
		hom_mat2d_scale_local(HomMat2DZOOM, ScaleTest, ScaleTest, &HomMat2DZOOM);
		affine_trans_image(Image, ImageNew, HomMat2DZOOM, "bilinear", "false");
	}
	else
	{
		ImageNew=&Image;
	}
}
//
STDMETHODIMP CPccAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
STDMETHODIMP CPccAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("621$\
					 381;B$\
					 374;LP;4;2000;1;220$\
					 390;LP;1;255;1;40");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::ModelCreate(double Radius, HTuple * ModelID1)
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
STDMETHODIMP CPccAlgo::CenterLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
STDMETHODIMP CPccAlgo::CenterLocationAlgo1Help(BSTR* pHelpStr)
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
//use edegs define center
STDMETHODIMP CPccAlgo::CenterLocationUseEdgesAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,  VARIANT* pThresholdHigh, VARIANT* pMinCircularity)
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
STDMETHODIMP CPccAlgo::CenterLocationUseEdgesAlgoHelp(BSTR* pHelpStr)
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
//use edegs define center 2
STDMETHODIMP CPccAlgo::CenterLocationUseEdgesAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,  VARIANT* pThresholdHigh)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	int Radius = (int)pRadius->fltVal;
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
		gen_ellipse(&Ellipse, Height/2.0, Width/2.0, HTuple(0).Rad(), Width/2.0, 50+(Height/2));
		//*********
		// Local iconic variables 
		Hobject  Ellipse2, ImageReduced2;
		Hobject  Region1, RegionFillUp, RegionOpening, RegionOpeningExt;
		Hobject  RegionDifference, ConnectedRegions1, SelectedRegions1;
		Hobject  RegionErosion, RegionDilation, RegionDilation3;
		Hobject  ImageReduced1, Region, ConnectedRegions, SelectedRegions;
		Hobject  RegionUnion, EdgesRegion, Contours1, Region2, RegionIntersection;
		Hobject  RegionDifference1, RegionIntersection1, Contours;
		Hobject  SelectedXLD, UnionContours1, ObjectSelected;
		// Local control variables 
		HTuple  Number, Length, Indices, Row, Column,RadiusFit, StartPhi,EndPhi, PointOrder;

		gen_ellipse(&Ellipse2, (Height/2.0)-80, Width/2.0, HTuple(-40).Rad(), (Width/2.0)+30, Height/2);
		reduce_domain(Image, Ellipse, &ImageReduced2);
		//************
		threshold(ImageReduced2, &Region1, ThresholdHigh, 255);
		fill_up(Region1, &RegionFillUp);
		opening_circle(RegionFillUp, &RegionOpening, Radius-10);
		opening_circle(RegionFillUp, &RegionOpeningExt, 50.5);

		difference(RegionOpeningExt, RegionOpening, &RegionDifference);

		connection(RegionDifference, &ConnectedRegions1);
		select_shape(ConnectedRegions1, &SelectedRegions1, "area", "and", 150, 999999);

		erosion_circle(SelectedRegions1, &RegionErosion, 1.5);
		dilation_circle(RegionErosion, &RegionDilation, 8.5);

		dilation_circle(RegionOpening, &RegionDilation3, 11.5);
		reduce_domain(Image, RegionDilation3, &ImageReduced1);
		//*
		threshold(ImageReduced1, &Region, 0, ThresholdLow);
		connection(Region, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", 500, 999999);
		union1(SelectedRegions, &RegionUnion);
		dilation_rectangle1(RegionUnion, &EdgesRegion, 9, 9);
		//*
		gen_contour_region_xld(RegionOpening, &Contours1, "border");
		gen_region_contour_xld(Contours1, &Region2, "margin");
		intersection(Region2, EdgesRegion, &RegionIntersection);
		difference(RegionIntersection, RegionDilation, &RegionDifference1);
		intersection(RegionDifference1, Ellipse2, &RegionIntersection1);
		gen_contours_skeleton_xld(RegionIntersection1, &Contours, 1, "filter");
		//************************
		sort_contours_xld(Contours, &SelectedXLD, "upper_left", "false", "column");
		union_adjacent_contours_xld(SelectedXLD, &UnionContours1, 600, 600, "attr_keep");
		count_obj(UnionContours1, &Number);
		if (Number>0)
		{	
			Length = HTuple(0);
			length_xld(UnionContours1, &Length);
			tuple_sort_index(Length, &Indices);
			int XldLength=(int)(Indices[Number-1].I());
			if(Length[XldLength].D()>20)
			{
				select_obj(UnionContours1, &ObjectSelected, XldLength+1);
				fit_circle_contour_xld(ObjectSelected, "geometric", -1, 2, 0, 3, 2, &Row, &Column, &RadiusFit, &StartPhi, &EndPhi, &PointOrder);
				gen_circle(&Circle, Row, Column, Radius);

				m_vPos[0].m_x = (float)Column[0].D();
				m_vPos[0].m_y = (float)Row[0].D();
				Hobject RegionShow;
				concat_obj(RegionIntersection1,Circle,&RegionShow);
				m_vRgn[id] = RegionShow;
			}
			else
			{
				m_vPos[0].m_x = 0.0;
				m_vPos[0].m_y = 0.0;
				Hobject circleD;
				gen_circle(&circleD, Height/2.0, Width/2.0, Radius);
				m_vRgn[id] = circleD;
				return S_FALSE;
			}
		}
		else
		{
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			Hobject circleD;
			gen_circle(&circleD, Height/2.0, Width/2.0, Radius);
			m_vRgn[id] = circleD;
			return S_FALSE;
		}
		//************
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
STDMETHODIMP CPccAlgo::CenterLocationUseEdgesAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("632$\
					 381;B$\
					 374;LP;10;2000;1;220$\
					 648;LP;1;220;1;20$\
					 649;LP;5;255;1;40");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MatchingParaAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn      = m_vRgn[rId];
	float greediness = (float)pGreediness->fltVal;
	int ringOrOCR    = (int)pRingOrOCR->fltVal;
	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			CString sModelPath1  = *bstrShm1File;
			string strModelPath1 = CT2A(sModelPath1);
			if(!_access(strModelPath1.c_str(),0))
			{
				HTuple modelID;
				read_shape_model(strModelPath1.c_str(), &modelID);
				m_dictCrtPathModelID[strModelPath1] = modelID;
				m_dictNewPathModelID[strModelPath1] = modelID;
			}
			else
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("请检查模板初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the initial model path is correct or not."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				return S_FALSE;
			}

			if(ringOrOCR==2)
			{
				CString sModelPath2   = *bstrShm2File;
				string strModelPath2  = CT2A(sModelPath2);
				if (!_access(strModelPath2.c_str(),0))
				{
					HTuple modelID;
					read_shape_model(strModelPath2.c_str(), &modelID);
					m_dictCrtPathModelID[strModelPath2] = modelID;
					m_dictNewPathModelID[strModelPath2] = modelID;
				}
				else
				{
					if(m_isChineseLanguage)
					{
						MessageBox(NULL,_T("请检查模板2初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					else
					{
						MessageBox(NULL,_T("Please check whether the second initial model path is correct or not."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					retValue = -1;
					m_vErrorRgn[rId].Reset();
					retValue.Detach(pMatchingResult);
					m_modelMatchingPARA[0]=0.0;
					m_modelMatchingPARA[1]=0.0;
					m_modelMatchingPARA[2]=0.0;
					m_modelMatchingPARA[3]=0.0;
					m_modelMatchingPARA[4]=0.0;
					m_modelMatchingPARA[5]=0.0;
					m_modelMatchingPARA[6]=0.0;
					return S_FALSE;
				}
			}
		}
		//
		Hobject ImageReduced,Circle;
		HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;
		m_modelMatchingPARA[0] = m_imageRadius;

		if(rgn.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=0.0;
			m_modelMatchingPARA[2]=0.0;
			m_modelMatchingPARA[3]=0.0;
			m_modelMatchingPARA[4]=0.0;
			m_modelMatchingPARA[5]=0.0;
			m_modelMatchingPARA[6]=0.0;
			return S_FALSE;
		}
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		bool b_isOK = true;
		Hobject Image=m_crtImg;
		//

		HTuple ClassRegion;
		get_obj_class(rgn,&ClassRegion);
		if(ClassRegion=="region")
		{
			Circle=rgn;
		}
		else
		{
			gen_circle (&Circle, m_imgHeight/2.0, m_imgWidth/2.0,m_ringPara[rId].m_Radius);   //默认图片的大小有要求
		}
		//

		reduce_domain(Image, Circle, &ImageReduced);

		CString sModelPath1  = *bstrShm1File;
		string strModelPath1 = CT2A(sModelPath1);
		if(m_dictCrtPathModelID.find(strModelPath1) == m_dictCrtPathModelID.end())
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=0.0;
			m_modelMatchingPARA[2]=0.0;
			m_modelMatchingPARA[3]=0.0;
			m_modelMatchingPARA[4]=0.0;
			m_modelMatchingPARA[5]=0.0;
			m_modelMatchingPARA[6]=0.0;
			return S_FALSE;
		}
		HTuple modelID = m_dictCrtPathModelID[strModelPath1];
		HTuple dataLength1;
		tuple_length (modelID, &dataLength1);

		if(ringOrOCR == 1)
		{
			if(dataLength1 < 1)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("请检查模板初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the initial model path is correct or not."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				return S_FALSE;
			}
			//匹配计算时，搜索范围与建模板的角度范围一致！
			HTuple NumLevels,AngleStart, AngleExtent, AngleStep, ScaleMin, ScaleMax, ScaleStep, Metric, MinContrast;//Scale1,
			get_shape_model_params (modelID, &NumLevels, &AngleStart, &AngleExtent, &AngleStep, &ScaleMin, &ScaleMax, &ScaleStep, &Metric, &MinContrast);
			if (AngleExtent<3.14)
			{
				// find_scaled_shape_model (ImageReduced,modelID, HTuple(-10).Rad(), HTuple(20).Rad(), 0.95, 1.05, 0.5, 1, 0.5, "least_squares", 0, greediness, &Row_M, &Column_M, &Angle_M,&Scale1,&Score_M);
				find_shape_model(ImageReduced,modelID, HTuple(-10).Rad(), HTuple(20).Rad(), 0.5, 1,0.5, "least_squares",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M);
			}
			else
			{
				find_shape_model(ImageReduced,modelID, HTuple(0).Rad(), HTuple(360).Rad(), 0.5, 1,0.5, "least_squares",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M);
			}			if (0 != (Score_M>0.05))
			{
				m_modelMatchingPARA[1]=Row_M;
				m_modelMatchingPARA[2]=Column_M;
				m_modelMatchingPARA[3]=Angle_M;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				retValue = Score_M[0].D();
			}
			else
			{
				b_isOK = false;
			}
		}
		else if(ringOrOCR==2)
		{
			CString sModelPath2  = *bstrShm2File;
			string strModelPath2 = CT2A(sModelPath2);
			if(m_dictCrtPathModelID.find(strModelPath2) == m_dictCrtPathModelID.end())
			{
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				return S_FALSE;
			}
			HTuple modelID2 = m_dictCrtPathModelID[strModelPath2];
			HTuple dataLength2;

			tuple_length (modelID2, &dataLength2);
			if((dataLength1 < 1)||(dataLength2 < 1))
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("请检查模板初始化路径是否正确,是否存在拉环与图案两个模板."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the initial model path is correct or not.Tab model and OCR model whether or not exist."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				return S_FALSE;
			}
			find_shape_model(ImageReduced,modelID, HTuple(0).Rad(), HTuple(360).Rad(), 0.5, 1,0.5, "least_squares",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M);
			if (0 != (Score_M>0.05))
			{
				m_modelMatchingPARA[1]=Row_M;
				m_modelMatchingPARA[2]=Column_M;
				m_modelMatchingPARA[3]=Angle_M;
				retValue =Score_M[0].D();
			}
			else
				b_isOK=false;

			find_shape_model(ImageReduced,modelID2, HTuple(0).Rad(), HTuple(360).Rad(), 0.5, 1,0.5, "least_squares",(HTuple(4).Append(-2)),greediness, &Row_M, &Column_M, &Angle_M, &Score_M);
			if (0 != (Score_M>0.05))
			{
				m_modelMatchingPARA[4]=Row_M;
				m_modelMatchingPARA[5]=Column_M;
				m_modelMatchingPARA[6]=Angle_M;
				retValue =Score_M[0].D();
			}
			else
				b_isOK=false;
		}
		if(!b_isOK)
		{
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=0.0;
			m_modelMatchingPARA[2]=0.0;
			m_modelMatchingPARA[3]=0.0;
			m_modelMatchingPARA[4]=0.0;
			m_modelMatchingPARA[5]=0.0;
			m_modelMatchingPARA[6]=0.0;
			retValue = 0.0;	
			retValue.Detach(pMatchingResult);
			m_vErrorRgn[rId] = Circle;
			return S_FALSE;
		}
		else
		{
			retValue.Detach(pMatchingResult);
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_modelMatchingPARA[0]=0.0;
		m_modelMatchingPARA[1]=0.0;
		m_modelMatchingPARA[2]=0.0;
		m_modelMatchingPARA[3]=0.0;
		m_modelMatchingPARA[4]=0.0;
		m_modelMatchingPARA[5]=0.0;
		m_modelMatchingPARA[6]=0.0;
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pMatchingResult);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CPccAlgo::MatchingParaAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("439$\
					 440;R;FP;0.0;1.0;0.01$\
					 167;B$\
					 322;FP;0.2;1.0;0.05;0.85$\
					 441;LP;1;2;1;1$\
					 535;SPM$\
					 629;SPM");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CPccAlgo::MatchingParaMinScoreAlgo(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn      = m_vRgn[rId];
	float greediness = (float)pGreediness->fltVal;
	int ringOrOCR    = (int)pRingOrOCR->fltVal;
	double MinScore=(double)pMinScore->fltVal;
	int NumLevels=(int)pNumLevels->fltVal;
	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			CString sModelPath1  = *bstrShm1File;
			string strModelPath1 = CT2A(sModelPath1);
			if(!_access(strModelPath1.c_str(),0))
			{
				HTuple modelID;
				read_shape_model(strModelPath1.c_str(), &modelID);
				m_dictCrtPathModelID[strModelPath1] = modelID;
				m_dictNewPathModelID[strModelPath1] = modelID;
			}
			else
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("请检查模板初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the initial model path is correct or not."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				return S_FALSE;
			}

			if(ringOrOCR==2)
			{
				CString sModelPath2   = *bstrShm2File;
				string strModelPath2  = CT2A(sModelPath2);
				if (!_access(strModelPath2.c_str(),0))
				{
					HTuple modelID;
					read_shape_model(strModelPath2.c_str(), &modelID);
					m_dictCrtPathModelID[strModelPath2] = modelID;
					m_dictNewPathModelID[strModelPath2] = modelID;
				}
				else
				{
					if(m_isChineseLanguage)
					{
						MessageBox(NULL,_T("请检查模板2初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					else
					{
						MessageBox(NULL,_T("Please check whether the second initial model path is correct or not."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					retValue = -1;
					m_vErrorRgn[rId].Reset();
					retValue.Detach(pMatchingResult);
					m_modelMatchingPARA[0]=0.0;
					m_modelMatchingPARA[1]=0.0;
					m_modelMatchingPARA[2]=0.0;
					m_modelMatchingPARA[3]=0.0;
					m_modelMatchingPARA[4]=0.0;
					m_modelMatchingPARA[5]=0.0;
					m_modelMatchingPARA[6]=0.0;
					return S_FALSE;
				}
			}
		}
		//
		Hobject ImageReduced,Circle;
		HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;
		m_modelMatchingPARA[0] = m_imageRadius;

		if(rgn.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=0.0;
			m_modelMatchingPARA[2]=0.0;
			m_modelMatchingPARA[3]=0.0;
			m_modelMatchingPARA[4]=0.0;
			m_modelMatchingPARA[5]=0.0;
			m_modelMatchingPARA[6]=0.0;
			return S_FALSE;
		}
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		bool b_isOK = true;
		Hobject Image=m_crtImg;
		//
		Hobject ImageEnhance,EnhanceRegion,ImageReduced1,DiffOfGauss,ImageSub,ImageConverted;
		gen_circle (&EnhanceRegion, m_imgHeight/2.0, m_imgWidth/2.0, m_imgHeight/2.1);
		reduce_domain (Image, EnhanceRegion, &ImageReduced1);
		diff_of_gauss (ImageReduced1, &DiffOfGauss, 1.0, 20);
		convert_image_type (DiffOfGauss, &ImageConverted, "byte");
		sub_image (Image, ImageConverted, &ImageSub, 1, 0);
		//
		Circle=rgn;
		reduce_domain(ImageSub, Circle, &ImageReduced);
		Hobject ImageReducedOcr;
		reduce_domain(Image, Circle, &ImageReducedOcr);
		CString sModelPath1  = *bstrShm1File;
		string strModelPath1 = CT2A(sModelPath1);
		if(m_dictCrtPathModelID.find(strModelPath1) == m_dictCrtPathModelID.end())
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=0.0;
			m_modelMatchingPARA[2]=0.0;
			m_modelMatchingPARA[3]=0.0;
			m_modelMatchingPARA[4]=0.0;
			m_modelMatchingPARA[5]=0.0;
			m_modelMatchingPARA[6]=0.0;
			return S_FALSE;
		}
		HTuple modelID = m_dictCrtPathModelID[strModelPath1];
		HTuple dataLength1;
		tuple_length (modelID, &dataLength1);

		if(ringOrOCR == 1)
		{
			if(dataLength1 < 1)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("请检查模板初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the initial model path is correct or not."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				return S_FALSE;
			}
			//
			find_shape_model(ImageReduced,modelID, HTuple(0).Rad(), HTuple(360).Rad(), MinScore, 1,0.5, "least_squares",NumLevels,greediness, &Row_M, &Column_M, &Angle_M, &Score_M);
			if (0 != (Score_M>0.05))
			{
				m_modelMatchingPARA[1]=Row_M;
				m_modelMatchingPARA[2]=Column_M;
				m_modelMatchingPARA[3]=Angle_M;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				retValue = Score_M[0].D();
			}
			else
			{
				b_isOK = false;
			}
		}
		else if(ringOrOCR==2)
		{
			CString sModelPath2  = *bstrShm2File;
			string strModelPath2 = CT2A(sModelPath2);
			if(m_dictCrtPathModelID.find(strModelPath2) == m_dictCrtPathModelID.end())
			{
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				return S_FALSE;
			}
			HTuple modelID2 = m_dictCrtPathModelID[strModelPath2];
			HTuple dataLength2;

			tuple_length (modelID2, &dataLength2);
			if((dataLength1 < 1)||(dataLength2 < 1))
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("请检查模板初始化路径是否正确,是否存在拉环与图案两个模板."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the initial model path is correct or not.Tab model and OCR model whether or not exist."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				return S_FALSE;
			}
			find_shape_model(ImageReduced,modelID, HTuple(0).Rad(), HTuple(360).Rad(),MinScore , 1,0.5, "least_squares",NumLevels,greediness, &Row_M, &Column_M, &Angle_M, &Score_M);
			if (0 != (Score_M>0.05))
			{
				m_modelMatchingPARA[1]=Row_M;
				m_modelMatchingPARA[2]=Column_M;
				m_modelMatchingPARA[3]=Angle_M;
				retValue =Score_M[0].D();
			}
			else
				b_isOK=false;
			//判断搜索区域形状
			Hobject SearchRegionFillUp,SearchNew,CenterRegion;
			HTuple RowCenter,ColCenter,AreaCenter;
			fill_up(Circle,&SearchRegionFillUp);
			difference(SearchRegionFillUp,Circle,&CenterRegion);
			area_center(CenterRegion,&AreaCenter,&RowCenter,&ColCenter);
			if (AreaCenter>100)
			{
				float fCenterX = 0.0;
				float fCenterY = 0.0;
				fCenterX = m_vPos[0].m_x;
				fCenterY = m_vPos[0].m_y;
				Hobject SearchNew;
				gen_circle (&SearchNew, fCenterY, fCenterX,40.0);
				reduce_domain(Image, SearchNew, &ImageReducedOcr);
			} 
			find_shape_model(ImageReducedOcr,modelID2, HTuple(0).Rad(), HTuple(360).Rad(), 0.4, 1,0.5, "least_squares",(HTuple(4).Append(-2)),greediness, &Row_M, &Column_M, &Angle_M, &Score_M);
			if (0 != (Score_M>0.05))
			{
				m_modelMatchingPARA[4]=Row_M;
				m_modelMatchingPARA[5]=Column_M;
				m_modelMatchingPARA[6]=Angle_M;
				retValue =Score_M[0].D();
			}
			else
				b_isOK=false;
		}
		if(!b_isOK)
		{
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=0.0;
			m_modelMatchingPARA[2]=0.0;
			m_modelMatchingPARA[3]=0.0;
			m_modelMatchingPARA[4]=0.0;
			m_modelMatchingPARA[5]=0.0;
			m_modelMatchingPARA[6]=0.0;
			retValue = 0.0;	
			retValue.Detach(pMatchingResult);
			m_vErrorRgn[rId] = Circle;
			return S_FALSE;
		}
		else
		{
			retValue.Detach(pMatchingResult);
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_modelMatchingPARA[0]=0.0;
		m_modelMatchingPARA[1]=0.0;
		m_modelMatchingPARA[2]=0.0;
		m_modelMatchingPARA[3]=0.0;
		m_modelMatchingPARA[4]=0.0;
		m_modelMatchingPARA[5]=0.0;
		m_modelMatchingPARA[6]=0.0;
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pMatchingResult);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CPccAlgo::MatchingParaMinScoreAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("439$\
					 440;R;FP;0.0;1.0;0.01$\
					 167;B$\
					 654;FP;0.1;1.0;0.05;0.5$\
					 670;LP;0;4;1;3$\
					 322;FP;0.2;1.0;0.05;0.85$\
					 441;LP;1;2;1;1$\
					 535;SPM$\
					 629;SPM");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CPccAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CPccAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CPccAlgo::CircleRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//依据胶塞的中心的定位结果，得到匹配搜索区域，专门用于模板匹配
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	m_ringPara[id].m_Radius = (float)pRadius->fltVal;
	m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float Radius =(float)pRadius->fltVal;
	float m_RadiusExt = Radius - RoiWidth;

	int MoveSize = (int)pMoveSize->fltVal;

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y+MoveSize;

	if (fCenterY>=m_imgHeight-1)
	{
		fCenterY = m_vPos[0].m_y;
	}

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
STDMETHODIMP CPccAlgo::CircleRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("1145$\
					 167;B$\
					 1124;LP;1;150;1;100$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::DetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
{
	int id = (int)rgnId->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
	int RingOrOCR     = (int)pRingOrOCR->fltVal;
	//
	Hobject newRgn;
	HTuple tmp;
	tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
	tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
	m_vRgnReadData[id] = tmp;
	//
	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
	if(RingOrOCR < 1 || RingOrOCR > 2)
	{
		m_vRgn[id].Reset();
		return S_FALSE;
	}

	CString sRgnPath1  = *bstrRgnFile;
	string strRgnPath1 = CT2A(sRgnPath1);
	CString sRgnPath2  = *bstrExtractRgnFile;
	string strRgnPath2 = CT2A(sRgnPath2);
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			if(!_access(strRgnPath1.c_str(),0))
			{
				Hobject retRgn;
				read_region(&retRgn,strRgnPath1.c_str());
				m_dictCrtPathRgn[strRgnPath1] = retRgn;
				m_dictNewPathRgn[strRgnPath1] = retRgn;
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
			HTuple tmp;
			tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
			tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
			m_vRgnReadData[id] = tmp;

			if(RingOrOCR==2)
			{
				if(!_access(strRgnPath2.c_str(),0))
				{
					Hobject retRgn;
					read_region(&retRgn,strRgnPath2.c_str());
					m_dictCrtPathRgn[strRgnPath2] = retRgn;
					m_dictNewPathRgn[strRgnPath2] = retRgn;
				}
				else
				{
					if(m_isChineseLanguage)
					{
						MessageBox(NULL,_T("初始化参数,剔除区域路径设置错误。"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					else
					{
						MessageBox(NULL, _T("Initialization parameter（deducts region path）setting is wrong!"),_T("Initial configuration is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					m_vRgn[id].Reset();
					return S_FALSE;
				}
				HTuple tmp;
				tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
				tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
				m_vRgnReadData[id] = tmp;
			}
		}

		m_vRgn[id] = newRgn;
		//
		HTuple dataLength;
		tuple_length (m_modelMatchingPARA, &dataLength);
		if(dataLength < 3)
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('DetectRegionLocationAlgo')前需先调用匹配函数('MatchingParaAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MatchingParaAlgo')before calling location function('DetectRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		if(m_modelMatchingPARA[0].D() < 1.0)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject RegionDetect;

		HTuple modelPARAID = m_vRgnReadData[id];
		if(RingOrOCR==1)
		{	
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn ;
				return S_FALSE;
			}
			Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
			//
			//HTuple modelPARAIDLength;
			//tuple_length(modelPARAID,&modelPARAIDLength);
			//if(modelPARAIDLength[0].I()<6)
			//{
			//	MessageBox(NULL,_T("初始化参数（模板路径与检测区域路径）或区域编号设置错误"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			//	m_vRgn[id] = newRgn ;
			//	return S_FALSE;
			//}
			m_modelPARA = modelPARAID;
			//
			Hobject  RegionAffineTrans;
			//HTuple ringHomMat2D;
			//vector_angle_to_rigid(m_modelPARA[1].D(),m_modelPARA[2].D(),m_modelPARA[3].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&ringHomMat2D);
			//affine_trans_region(roiRgn,&RegionAffineTrans,ringHomMat2D,"false");
			//
			HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted;
			vector_angle_to_rigid(0, 0, m_modelPARA[3].D(), 0, 0, m_modelMatchingPARA[3].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

			Hobject rotatedRgn,RegionMoved1;
			HTuple RowModelData,ColModelData;
			RowModelData=m_modelPARA[1];
			ColModelData=m_modelPARA[2];
			move_region (roiRgn, &RegionMoved1,-RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_modelMatchingPARA[1];
			ColModelData=m_modelMatchingPARA[2];
			move_region (rotatedRgn,&RegionAffineTrans,RowModelData,ColModelData) ;
			//
			if(ErosionSize>=0.5)
			{
				erosion_circle (RegionAffineTrans, &RegionDetect,ErosionSize);
			}
			else if(ErosionSize<-0.5)
			{
				dilation_circle(RegionAffineTrans, &RegionDetect,abs(ErosionSize));
			}
			else
			{
				RegionDetect = RegionAffineTrans;
			}
		}
		else if(RingOrOCR==2)
		{
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}
			Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
			HTuple modelPARAID = m_vRgnReadData[id];

			//HTuple modelPARAIDLength;
			//tuple_length(modelPARAID,&modelPARAIDLength);
			//if(modelPARAIDLength[0].I()<6)
			//{
			//	MessageBox(NULL,_T("仅同时存在图案与拉环两个模板时‘盖面图案’参数可选择('2')"),_T("参数设置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			//	m_vRgn[id] = newRgn;
			//	return S_FALSE;
			//}
			m_modelPARA[0] = modelPARAID[0];
			m_modelPARA[1] = modelPARAID[1];
			m_modelPARA[2] = modelPARAID[2];
			m_modelPARA[3] = modelPARAID[3];
			m_modelPARA[4] = modelPARAID[4];
			m_modelPARA[5] = modelPARAID[5];
			m_modelPARA[6] = modelPARAID[6];
			//
			Hobject  RegionAffineTrans,OCRRegionAffineTrans,RegionDiff;
			//HTuple ringHomMat2D,OCRHomMat2D;
			//vector_angle_to_rigid(m_modelPARA[1].D(),m_modelPARA[2].D(),m_modelPARA[3].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&ringHomMat2D);
			//affine_trans_region(roiRgn,&RegionAffineTrans,ringHomMat2D,"false");
			//
			HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted;
			vector_angle_to_rigid(0, 0, m_modelPARA[3].D(), 0, 0, m_modelMatchingPARA[3].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

			Hobject rotatedRgn,RegionMoved1;	
			HTuple RowModelData,ColModelData;
			RowModelData=m_modelPARA[1];
			ColModelData=m_modelPARA[2];
			move_region (roiRgn, &RegionMoved1,-RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_modelMatchingPARA[1];
			ColModelData=m_modelMatchingPARA[2];
			move_region (rotatedRgn,&RegionAffineTrans,RowModelData,ColModelData) ;
			//
			//vector_angle_to_rigid(m_modelPARA[4].D(),m_modelPARA[5].D(),m_modelPARA[6].D(),m_modelMatchingPARA[4].D(),m_modelMatchingPARA[5].D(),m_modelMatchingPARA[6].D(),&OCRHomMat2D);

			if(m_dictCrtPathRgn.find(strRgnPath2) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}
			Hobject rgnExtract = m_dictCrtPathRgn[strRgnPath2];
			//affine_trans_region(rgnExtract,&OCRRegionAffineTrans,OCRHomMat2D,"false");
			//
			vector_angle_to_rigid(0, 0, m_modelPARA[6].D(), 0, 0, m_modelMatchingPARA[6].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
			RowModelData=m_modelPARA[4];
			ColModelData=m_modelPARA[5];
			move_region (rgnExtract, &RegionMoved1,-RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_modelMatchingPARA[4];
			ColModelData=m_modelMatchingPARA[5];
			move_region (rotatedRgn,&OCRRegionAffineTrans,RowModelData,ColModelData) ;
			//
			difference (RegionAffineTrans, OCRRegionAffineTrans, &RegionDiff);
			RegionAffineTrans = RegionDiff;
			if(ErosionSize>=0.5)
			{
				erosion_circle (RegionAffineTrans, &RegionDetect,ErosionSize);
			}
			else if(ErosionSize<-0.5)
			{
				dilation_circle(RegionAffineTrans, &RegionDetect,abs(ErosionSize));
			}
			else
			{
				RegionDetect=RegionAffineTrans;
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
STDMETHODIMP CPccAlgo::DetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("442$\
					 167;B$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 441;LP;1;2;1;1$\
					 267;SP$\
					 232;SP" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
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
STDMETHODIMP CPccAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CPccAlgo::DynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CPccAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CPccAlgo::BubbleDetectProfileAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CPccAlgo::BubbleDetectProfileAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CPccAlgo::BubbleDetectProfileAlgo1(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
		mean_image(ImageDetectNew, &ImageReduceMean, 1, BlackMaskSize);
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
			mean_image(ImageDetectNew, &ImageReduceMean, 1, WhiteMaskSize);
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
STDMETHODIMP CPccAlgo::BubbleDetectProfileAlgo1Help(BSTR* pHelpStr)
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
STDMETHODIMP CPccAlgo::BlackDetectExcludeEdgesAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize, VARIANT* pThresholdLow,  VARIANT* pMinEdgeSize, VARIANT* pDetectArea)
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
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, 99999999);
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
STDMETHODIMP CPccAlgo::BlackDetectExcludeEdgesAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CPccAlgo::BlackDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
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
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, 99999999);
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
STDMETHODIMP CPccAlgo::BlackDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CPccAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
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
STDMETHODIMP CPccAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CPccAlgo::UVCurlAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea)
{
	//UVCurlAlgo  对应第三版UVCurlDLL2.1
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
STDMETHODIMP CPccAlgo::UVCurlAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CPccAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CPccAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
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
		if (m_bDebugImg)
		{
			polar_trans_region_inv(BlackDefectsPolar, &BlackDefects, fCenterY, fCenterX, HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, OriginImgWidth, OriginImgHeight, InterpolationMethod);
			union1(BlackDefects, &BlackDefects);
			concat_obj(*pDefects, BlackDefects, pDefects);
		} 
		else
		{
			//在线检测阶段，只利用错误区域的重心，标记成圆形区域
			HTuple RadiusOrigin = RadiusStart - BlackDefectPolarRow;
			HTuple Theta = 1.0 * BlackDefectPolarCol / PolarImgWidth * HTuple((EndAngle - StartAngle).Abs()).Rad() + HTuple(StartAngle).Rad();

			HTuple sinTheta, cosTheta;
			tuple_sin(Theta, &sinTheta);
			tuple_cos(Theta, &cosTheta);

			HTuple Row_Origin = fCenterY - RadiusOrigin* sinTheta;
			HTuple Col_Origin = fCenterX + RadiusOrigin* cosTheta;

			HTuple Radius_Origin;
			HTuple DefectNum;
			tuple_length(BlackDefectPolarRow, &DefectNum);
			tuple_gen_const(DefectNum, 10, &Radius_Origin);
			gen_circle(&BlackDefects, Row_Origin, Col_Origin, Radius_Origin);
			union1(BlackDefects, &BlackDefects);
			concat_obj(*pDefects, BlackDefects, pDefects);
		}

		*pDefectPolarArea = BlackDefectPolarArea;
	}	
}
STDMETHODIMP CPccAlgo:: AnnularRotaDynThresholdAlgoBW(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	//m_vWhiteErrorRgn[rId].Reset();
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
		float RoiWidth = m_ringPara[rId].m_RoiWidth;
		float Radius = m_ringPara[rId].m_Radius;

		float StartAngle = m_sectorAngle[rId].m_angleStartAbs;	//逆时针转顺时针
		float EndAngle =  m_sectorAngle[rId].m_angleEndAbs;

		HTuple RadiusStart, RadiusEnd;

		if (Radius - RoiWidth < 1)
		{			
			RadiusEnd = 0;
			RadiusStart = Radius;
		}
		else
		{
			RadiusEnd = Radius - RoiWidth;
			RadiusStart = Radius;
		}

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

		Hobject WhiteDefects;
		gen_empty_obj(&WhiteDefects);
		HTuple  WhiteDefectPolarArea;
		GetDefectsInPolarImg4AnnualRota(Image, PolarTransImageOrigin, PolarTransImageRotated, &WhiteDefects, WhiteThredLow, WhiteDefectSize, Offset, m_bDebugImg, fCenterY, fCenterX, RadiusStart, RadiusEnd, RotaAngle, StartAngle, EndAngle, &WhiteDefectPolarArea, "white");

		union2(DefectRegs, WhiteDefects, &DefectRegs);

		HTuple Area, Row_test, Column_test;
		union1(DefectRegs, &DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);


		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
			//m_vWhiteErrorRgn[rId] = newRgn;
		}
		else
		{			
			dilation_circle (BlackDefects, &BlackDefects, 2.5);
			dilation_circle (WhiteDefects, &WhiteDefects, 2.5);
			union2(BlackDefects,WhiteDefects,&BlackDefects);
			m_vErrorRgn[rId] = BlackDefects;
			//m_vWhiteErrorRgn[rId] = WhiteDefects;			

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
		//m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CPccAlgo:: AnnularRotaDynThresholdAlgoBWHelp(BSTR* pHelpStr)
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

STDMETHODIMP CPccAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CPccAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CPccAlgo::LineDetectionAlgo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize,VARIANT* pDefectArea)
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
		
		// 2019年12月18日
		connection(defectRgn,&defectRgn );
		select_shape(defectRgn,&defectRgn,"area","and",detectSize-1,99999999 );
		union1(defectRgn,&defectRgn );

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
STDMETHODIMP CPccAlgo::LineDetectionAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CPccAlgo::LineDetectionExclude90Algo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize,VARIANT* pDefectArea)
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
		//剔除竖直线
		Hobject ConnectedRegions,SelectedRegions;
		connection (defectRgn, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions,"width", "and", 8, 99999);
		union1 (SelectedRegions, &defectRgn);
		//
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
STDMETHODIMP CPccAlgo::LineDetectionExclude90AlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("856$\
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

void max_min_point_rect (Hobject PolarTransImage,Hobject *Region,HTuple PointState,HTuple SmoothSize,HTuple *GrayMaxRow,HTuple *GrayMaxCol)
{

	// Local iconic variables 
	Hobject  ImageMean;


	// Local control variables 
	HTuple  Width, Height, ColDatas, RowDatas;
	HTuple  Index1, ColDataNew, Grayvals, Indices;

	(*GrayMaxRow) = HTuple();
	(*GrayMaxCol) = HTuple();
	gen_empty_obj(&(*Region));
	get_image_size(PolarTransImage, &Width, &Height);
	if (0 != (Width<50))
	{
		return;
	}
	if (0 != (Height<5))
	{
		return;
	}
	mean_image(PolarTransImage, &ImageMean, SmoothSize, 1);
	tuple_gen_const(Height, 1, &ColDatas);
	tuple_gen_sequence(0, Height-1, 1, &RowDatas);
	tuple_gen_sequence(0, Width-1, 1, &(*GrayMaxCol));
	{
		HTuple end_val14 = Width-1;
		HTuple step_val14 = 1;
		for (Index1=0; Index1.Continue(end_val14, step_val14); Index1 += step_val14)
		{
			ColDataNew = ColDatas*Index1;
			get_grayval(ImageMean, RowDatas, ColDataNew, &Grayvals);
			tuple_sort_index(Grayvals, &Indices);
			if (0 != (PointState==1))
			{
				(*GrayMaxRow).ReplaceElements(Index1,Indices.Select(Height-1));
			}
			else if (0 != (PointState==-1))
			{
				(*GrayMaxRow).ReplaceElements(Index1,Indices.Select(0));
			}
			else
			{
				(*GrayMaxRow).ReplaceElements(Index1,Indices.Select(Height-1));
			}
		}
	}
	gen_region_points(&(*Region), (*GrayMaxRow), (*GrayMaxCol));
}
STDMETHODIMP CPccAlgo::DeformationDetectAlgo(VARIANT* rgnId, VARIANT* pAlgoSelect,VARIANT* pSmoothSize, VARIANT* pDetectDist)
{
	int rId = (int)rgnId->fltVal;
	int RoiWidth =(int) m_ringPara[rId].m_RoiWidth;
	int Radius =(int) m_ringPara[rId].m_Radius;
	int AlgoSelect = (int)pAlgoSelect->fltVal;
	int SmoothSize = (int)pSmoothSize->fltVal;
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
		Hobject  PolarTransImage,Region,XYTransRegion,RegionDilation,RegionErosion1,ConnectedRegions1,SelectedRegions1;
		//
		HTuple RadiusExtC,AngleExt,DonutWidth,GrayMaxRow,GrayMaxCol;
		HTuple  Width1,Height1;
		//***************************************************************************************
		if(Radius<=RoiWidth)
		{
			RoiWidth=(int)(Radius-1.0);
		}
		////////////////////////////////////////////

		RadiusExtC =Radius;
		AngleExt = 6.3;
		DonutWidth = RoiWidth;
		polar_trans_image_ext(Image, &PolarTransImage,fCenterY, fCenterX, 0, AngleExt, RadiusExtC-DonutWidth, RadiusExtC, AngleExt*(RadiusExtC-(0.5*DonutWidth)), DonutWidth, "nearest_neighbor");
		max_min_point_rect(PolarTransImage, &Region, AlgoSelect, SmoothSize, &GrayMaxRow, &GrayMaxCol);
		get_image_size(Image, &Width1, &Height1);

		polar_trans_region_inv(Region, &XYTransRegion,fCenterY, fCenterX, 0, AngleExt, RadiusExtC-DonutWidth, RadiusExtC, AngleExt*(RadiusExtC-(0.5*DonutWidth)), DonutWidth, Width1, Height1, "nearest_neighbor");
		//*
		dilation_circle(XYTransRegion, &RegionDilation, 2.5);
		erosion_circle(RegionDilation, &RegionErosion1, 1.5);
		connection(RegionErosion1, &ConnectedRegions1);
		select_shape_std(ConnectedRegions1, &SelectedRegions1, "max_area", 70);
		HTuple Area5,Row5,Column5,RadiusDiff,EllipesDiff;
		area_center(SelectedRegions1, &Area5, &Row5, &Column5);
		if (0 != (Area5<100))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectDist);
			m_vRgn[rId]=XYTransRegion;
			return FALSE;
		}
		//*
		Hobject Skeleton,Contours,Circle,Circle1,RegionDifference1,RegionOpening1,RegionDifference2;
		HTuple  Row, Column,Phi,Radius11,Radius21,StartPhi,EndPhi,PointOrder;
		HTuple  Row1,Column1,Radius1,Row2,Column2,Radius2,Row4,Column4,Radius4;
		skeleton(SelectedRegions1, &Skeleton);
		gen_contour_region_xld(Skeleton, &Contours, "center");
		fit_ellipse_contour_xld(Contours, "fitzgibbon", -1, 0, 0, 200, 3, 2, &Row,  &Column, &Phi, &Radius11, &Radius21, &StartPhi, &EndPhi,  &PointOrder);
		//*
		smallest_circle(SelectedRegions1, &Row1, &Column1, &Radius1);
		gen_circle(&Circle, Row1, Column1, Radius1);
		difference(Circle, SelectedRegions1, &RegionDifference1);
		opening_circle(RegionDifference1, &RegionOpening1, 50.5);
		inner_circle(RegionOpening1, &Row2, &Column2, &Radius2);
		gen_circle(&Circle1, Row2, Column2, Radius2);
		difference(Circle, Circle1, &RegionDifference2);
		inner_circle(RegionDifference2, &Row4, &Column4, &Radius4);
		RadiusDiff = (Radius4*2.0)-4.5;
		EllipesDiff = Radius11-Radius21;
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
STDMETHODIMP CPccAlgo::DeformationDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("313$\
					 397;R;FP;0;200;1$\
					 381;B$\
					 385;LP;-1;1;1;1$\
					 733;LP;1;150;1;9");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::TabDetectProfileAlgo(VARIANT* rgnId,  VARIANT* pThredBackground,  VARIANT* pThredForeground,VARIANT* pAnglePointSize,VARIANT* pMinAngle, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
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
		Hobject  RegionBlackDetect, RegionCapDetect, RegionAngleDetect;
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
		reduce_domain(Image, rgn, &ImageReduced1);
		mean_image(ImageReduced1, &ImageMean, 5, 3);
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
			clip_contours_xld(Contours, &ClippedContours1, Row12-10, Column12+20, Row22+10, Column22-20);
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
STDMETHODIMP CPccAlgo::TabDetectProfileAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("647$\
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

STDMETHODIMP CPccAlgo::BlackDetectUseUpSizeAlgo(VARIANT* rgnId, VARIANT* pBlackThresh,VARIANT* pThredForeground,VARIANT* pWhiteSize,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pDetectArea)
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
STDMETHODIMP CPccAlgo::BlackDetectUseUpSizeAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CPccAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CPccAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CPccAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

STDMETHODIMP CPccAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

STDMETHODIMP CPccAlgo::DetectRegionLocationExtractOCRAlgo(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pErosionSize, BSTR* bstrOcrRgnFile)
{
	int id = (int)rgnId->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	float Radius =(float)pRadius->fltVal;
	CString sRgnPath1  = *bstrOcrRgnFile;
	string strRgnPath1 = CT2A(sRgnPath1);
	//
	//m_vRgnExTest[id].Reset();
	//m_vRgnEx[id].Reset();
	HTuple tmp;
	tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
	tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
	m_vRgnReadData[id] = tmp;
	Hobject newRgn;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg )
		{
			if(!_access(strRgnPath1.c_str(),0))
			{
				Hobject retRgn;
				read_region(&retRgn,strRgnPath1.c_str());
				m_dictCrtPathRgn[strRgnPath1] = retRgn;
				m_dictNewPathRgn[strRgnPath1] = retRgn;
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
				return FALSE;
			}
			HTuple tmp;
			tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
			tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
			m_vRgnReadData[id] = tmp;
		}


		m_vRgn[id] = newRgn;
		//
		HTuple dataLength;
		tuple_length (m_modelMatchingPARA, &dataLength);
		if(dataLength < 3)
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('DetectRegionLocationExtractOCRAlgo')前,请先检查匹配函数('MatchingParaAlgo')是否正常."),_T("函数异常提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the function('MatchingParaAlgo') is correct or not,before calling location function('DetectRegionLocationExtractOCRAlgo')."), _T("Function error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		//
		if(	m_modelMatchingPARA[0].D()<1.0)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject RegionDetectOcr,RegionDetect;

		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		Hobject  RegionCenter;
		gen_circle(&RegionCenter, fCenterY, fCenterX, Radius);

		if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
		HTuple modelPARAID = m_vRgnReadData[id];
		////
		//HTuple modelPARAIDLength;
		//tuple_length(modelPARAID,&modelPARAIDLength);
		//if(modelPARAIDLength[0].I()<6)
		//{
		//	MessageBox(NULL,_T("初始化参数（模板路径与检测区域路径）或区域编号设置错误"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		//	m_vRgn[id] = newRgn ;
		//	return S_FALSE;
		//}
		m_modelPARA = modelPARAID;
		//
		Hobject  RegionAffineTrans;
		HTuple ringHomMat2D;
		vector_angle_to_rigid(m_modelPARA[1].D(),m_modelPARA[2].D(),m_modelPARA[3].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&ringHomMat2D);
		affine_trans_region(roiRgn,&RegionAffineTrans,ringHomMat2D,"false");
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
		difference(RegionCenter,RegionDetectOcr,&RegionDetect);
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
STDMETHODIMP CPccAlgo::DetectRegionLocationExtractOCRAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("575$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 232;SPR" );
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CPccAlgo::CenterLocationAlgoHS(VARIANT* rgnId, VARIANT* pThresholdLow,VARIANT* pAreaMax)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int AreaMax = (int)pAreaMax->fltVal;
	//m_vRgnExTest[id].Reset();
	//m_vRgnEx[id].Reset();
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, CenterRadius, hv_Row3, hv_Column3;
	Hobject threshed,RegionConnected,RegionUnion,RegionOpening;
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
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.6, Height/2.6);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		threshold(ImageReduced, &threshed,0, ThresholdLow);
		
		connection(threshed,&RegionConnected);
		Hobject RegionSelect;
		select_shape(RegionConnected,&RegionSelect,"area","and",2000,AreaMax);
		union1(RegionSelect, &RegionUnion);
		opening_circle(RegionUnion, &RegionOpening,10);
		HTuple area,row,col;
		area_center(RegionUnion,&area,&row,&col);
		if (area>0)
		{	
			m_vPos[0].m_y = (float)row[0].D();
			m_vPos[0].m_x = (float)col[0].D();
			m_vRgn[id] = RegionOpening;
			return S_OK;
		}
		else
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, 60);
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
STDMETHODIMP CPccAlgo::CenterLocationAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("639$\
					 381;B$\
					 640;LP;1;255;1;50$\
					 6005;LP;2100;20000;1;8000");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//单接口
STDMETHODIMP CPccAlgo::MsiUpperSurfaceRegionExtractAlgo(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pThredLow,VARIANT* pThredUpper)
{
	int id = (int)rgnID->fltVal; 
	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;
	int ThredLow=(int)pThredLow->fltVal;
	int ThredUpper=(int)pThredUpper->fltVal;
	//
	if (ThredLow>ThredUpper)
	{
		ThredLow=ThredUpper/2;
	}
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
		if (RowStart>m_imgHeight/2)
		{
			RowStart=10;
		}
		if (ColStart>m_imgWidth/2)
		{
			ColStart=10;
		}
		if ((RegWidth+ColStart)>m_imgWidth-1)
		{
			RegWidth=10;
		}
		if ((RegHeight+RowStart)>m_imgHeight-1)
		{
			RegHeight=10;
		}
		Hobject SearchRgn,ImageReduced;
		gen_rectangle2(&SearchRgn,RowStart+RegHeight/2,ColStart+RegWidth/2,0,RegWidth/2,RegHeight/2);
		HTuple rectTopY, rectTopX,rectBottomY,rectBottomX;
		smallest_rectangle1(SearchRgn,&rectTopY, &rectTopX,&rectBottomY,&rectBottomX);
		m_rectTopX=rectTopX;
		m_rectTopY=rectTopY;
		rectBottomY=rectTopY+(((rectBottomY-rectTopY)/4).Round())*4;
		rectBottomX=rectTopX+(((rectBottomX-rectTopX)/4).Round())*4;
		reduce_domain(Image,SearchRgn,&ImageReduced);
		//ImageEnhance
		Hobject DiffGauss,ImageConted,ImageEnhanced;
		diff_of_gauss(ImageReduced,&DiffGauss,1,10);
		convert_image_type(DiffGauss,&ImageConted,"byte");
		abs_diff_image(ImageReduced,ImageConted,&ImageEnhanced,1);
		Hobject ExpandedImage,ImageReduced1,ModelImages,ModelRegions;
		Hobject RegionDilation,RegionFillUp;
		expand_domain_gray(ImageEnhanced,&ExpandedImage,7);
		reduce_domain(ExpandedImage,SearchRgn,&ImageReduced1);

		// 给侧面顶部拉丝检测算子使用
		m_ExpandedImage = ExpandedImage;

		inspect_shape_model(ImageReduced1,&ModelImages,&ModelRegions,1,HTuple(ThredLow).Append(HTuple(ThredUpper)));
		//区域膨胀尺寸，用于连接边缘孤立Region
		dilation_circle(ModelRegions,&RegionDilation,6);
		fill_up(RegionDilation,&RegionFillUp);

		if(m_bDebugImg)
		{

			HTuple HomMat2DIdentity,HomMat2DScale;
			hom_mat2d_identity(&HomMat2DIdentity);
			hom_mat2d_scale(HomMat2DIdentity, 1, 1, 0, 0, &HomMat2DScale);
			Hobject RegionAffineTrans;
			affine_trans_region(RegionFillUp, &RegionAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(RegionAffineTrans, &RegionClosingMoved, 0, 0);

			Hobject RegionMargin;
			rectBottomY=rectTopY+(((rectBottomY-rectTopY)/4).Round())*4;
			rectBottomX=rectTopX+(((rectBottomX-rectTopX)/4).Round())*4;
			gen_region_polygon(&RegionMargin, ((((HTuple(rectTopY).Append(rectTopY)).Append(rectBottomY)).Append(rectBottomY)).Append(rectTopY)), 
				((((HTuple(rectTopX).Append(rectBottomX)).Append(rectBottomX)).Append(rectTopX)).Append(rectTopX)));
			concat_obj (RegionMargin, RegionClosingMoved, &RegionClosingMoved);
		}

		Hobject RegionComplement,RegionIntersection,connectedRegions,SelectRegion,RegionDiff,SelectRegion1,connectedRegions1;
		complement(RegionFillUp,&RegionComplement);
		intersection(RegionComplement,SearchRgn,&RegionIntersection);
		connection(RegionIntersection,&connectedRegions);
		select_shape_std(connectedRegions,&SelectRegion,"max_area",70);
		difference(SearchRgn,SelectRegion,&RegionDiff);
		connection(RegionDiff,&connectedRegions1);
		select_shape_std(connectedRegions1,&SelectRegion1,"max_area",70);

		//HTuple number;
		//count_obj (SelectRegion1, &number); 
		HTuple CenterArea,CenterRow,CenterCol;
		area_center(SelectRegion1,&CenterArea,&CenterRow,&CenterCol);
		if (CenterArea<2)
		{
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			m_vRgn[id] = SearchRgn ;
			m_DetectReg= newRgn ;
			return S_FALSE;
		}
		Hobject DetectReg;
		//区域腐蚀尺寸，与dilation_circle对应
		erosion_circle(SelectRegion1,&DetectReg,7);
		connection(DetectReg,&DetectReg);
		select_shape_std(DetectReg,&DetectReg,"max_area",70);

		m_DetectReg=DetectReg;
		m_vPos[0].m_x = (float)(m_imgWidth/2.0);
		m_vPos[0].m_y = (float)(m_imgHeight/2.0);
		m_vRgn[id] = RegionClosingMoved ;	

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
STDMETHODIMP CPccAlgo::MsiUpperSurfaceRegionExtractAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("795$\
					 381;B$\
					 777;LP;1;240;1;25$\
					 778;LP;1;320;1;25$\
					 779;LP;1;640;1;620$\
					 780;LP;1;480;1;330$\
					 762;LP;0;255;1;15$\
					 763;LP;0;255;1;30");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiUpperSurfaceCenterRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pVClipSize,VARIANT* pLClipSize,VARIANT*pMoveUpSize,VARIANT*pMoveLowSize)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	int VClipSize=(int)pVClipSize->fltVal;
	int LClipSize=(int)pLClipSize->fltVal;
	int MoveUpSize=(int)pMoveUpSize->fltVal;
	int MoveLowSize=(int)pMoveLowSize->fltVal;

	Hobject newRgn;
	if(m_DetectReg.Id() == H_EMPTY_REGION)
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
		HTuple Class;
		get_obj_class(m_DetectReg,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiUpperSurfaceCenterRegionLocationAlgo')前需先调用轮廓提取函数('MsiUpperSurfaceRegionExtractAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiUpperSurfaceRegionExtractAlgo')before calling location function('MsiUpperSurfaceCenterRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");

		//剔除区域下方干扰区域
		Hobject RegClipped;
		clip_region_rel(m_DetectReg,&RegClipped,0,VClipSize,LClipSize,LClipSize);

		Hobject RgnMoved,TopRgn,BottomRgn,RgnUnion,ROI;
		move_region(RegClipped,&RgnMoved,MoveUpSize,0);
		difference(RegClipped,RgnMoved,&TopRgn);

		move_region(RegClipped,&RgnMoved,-MoveLowSize,0);
		difference(RegClipped,RgnMoved,&BottomRgn);
		union2(TopRgn,BottomRgn,&RgnUnion);
		difference(RegClipped,RgnUnion,&ROI);
		Hobject RegErosion;
		erosion_circle(ROI,&RegErosion,ErosionSize);

		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(RegErosion, RectangleImage, &RegionDetect);
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
STDMETHODIMP CPccAlgo::MsiUpperSurfaceCenterRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("784$\
					 167;B$\
					 781;FP;0.5;80;0.5;3.5$\
					 832;LP;0;170;1;55$\
					 836;LP;0;120;1;25$\
					 837;LP;1;100;1;30$\
					 838;LP;1;100;1;30"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiUpperSurfaceCenterRegionLocationAlgo1(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pRegHeight,VARIANT*pMoveUpSize,VARIANT* pLClipSize)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	int LClipSize=(int)pLClipSize->fltVal;
	int MoveUpSize=(int)pMoveUpSize->fltVal;
	int RegHeight=(int)pRegHeight->fltVal;

	Hobject newRgn;
	if(m_DetectReg.Id() == H_EMPTY_REGION)
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
		HTuple Class;
		get_obj_class(m_BottomSidesRgn,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiUpperSurfaceCenterRegionLocationAlgo1')前需先调用轮廓提取函数('MsiBottomSidesRegionLocationAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiBottomSidesRegionLocationAlgo')before calling location function('MsiUpperSurfaceCenterRegionLocationAlgo1')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		Hobject ImageRect,ConnectedRegions6,SelectedRegions3,Contours,Region,RegionIntersection2,ConnectedRegions2;
		HTuple Row1,Col1,Area4, Row2,Column1;
		gen_rectangle1(&ImageRect, 1, 1, m_imgHeight-1, m_imgWidth-1);
		gen_contour_region_xld(m_DetectReg, &Contours, "border");
		get_contour_xld(Contours, &Row1, &Col1);
		gen_region_points(&Region, Row1, Col1);
		intersection(m_BottomSidesRgn, Region, &RegionIntersection2);
		connection(RegionIntersection2, &ConnectedRegions2);
		area_center(ConnectedRegions2, &Area4, &Row2, &Column1);
		Hobject RegionLines,Rectangle3,CharRect,ROI,RegClipped,RegMoved,CenterRgn;
		Hobject RegErosion;
		HTuple Phi1;
		if (0 != ((Row2.Num())==2))
		{
			gen_region_line(&RegionLines, Row2.Select(0), Column1.Select(0), Row2.Select(1), Column1.Select(1));
			line_orientation(Row2.Select(0), Column1.Select(0), Row2.Select(1), Column1.Select(1), &Phi1);
			gen_rectangle2(&Rectangle3, m_CenterRow+100, m_CenterColumn, Phi1, m_imgHeight, RegHeight);
			intersection(Rectangle3, ImageRect, &CharRect);
			difference(m_DetectReg, CharRect, &ROI);
			//
			clip_region_rel(ROI,&RegClipped,0,0,LClipSize,LClipSize);
			move_region(RegClipped,&RegMoved,MoveUpSize,0);
			intersection(RegMoved,RegClipped,&CenterRgn);	
			erosion_circle(CenterRgn,&RegErosion,ErosionSize);
		}
		else
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		intersection(RegErosion, ImageRect, &RegionDetect);
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
STDMETHODIMP CPccAlgo::MsiUpperSurfaceCenterRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("784$\
					 167;B$\
					 781;FP;0.5;80;0.1;3.5$\
					 838;LP;50;400;1;205$\
					 837;LP;1;100;1;30$\
					 836;LP;0;120;1;25"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiUpperSurfaceTopRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pVClipSize,VARIANT* pLClipSize,VARIANT*pMoveSize)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	int VClipSize=(int)pVClipSize->fltVal;
	int LClipSize=(int)pLClipSize->fltVal;
	int MoveSize=(int)pMoveSize->fltVal;

	Hobject newRgn;
	if(m_DetectReg.Id() == H_EMPTY_REGION)
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
		HTuple Class;
		get_obj_class(m_DetectReg,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiUpperSurfaceTopRegionLocationAlgo')前需先调用轮廓提取函数('MsiUpperSurfaceRegionExtractAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiUpperSurfaceRegionExtractAlgo')before calling location function('MsiUpperSurfaceTopRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");

		//剔除区域下方干扰区域
		Hobject RegClipped;
		clip_region_rel(m_DetectReg,&RegClipped,0,VClipSize,LClipSize,LClipSize);

		Hobject RgnMoved,RgnDiff,ROI;
		move_region(RegClipped,&RgnMoved,MoveSize,0);
		difference(RegClipped,RgnMoved,&ROI);
		Hobject RegErosion;
		erosion_circle(ROI,&RegErosion,ErosionSize);

		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(RegErosion, RectangleImage, &RegionDetect);
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
STDMETHODIMP CPccAlgo::MsiUpperSurfaceTopRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("784$\
					 167;B$\
					 781;FP;0.5;80;0.5;3.5$\
					 832;LP;0;170;1;55$\
					 836;LP;0;120;1;25$\
					 835;LP;1;100;1;30"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiUpperSurfaceBottomRegionLocationAlgo(VARIANT* rgnId,VARIANT* pOpeningSize,VARIANT* pVClipSize,VARIANT* pLClipSize,VARIANT*pMoveSize,VARIANT* pErosionSize)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	int OpeningSize=(int)pOpeningSize->fltVal;
	int VClipSize=(int)pVClipSize->fltVal;
	int LClipSize=(int)pLClipSize->fltVal;
	int MoveSize=(int)pMoveSize->fltVal;

	Hobject newRgn;
	if(m_DetectReg.Id() == H_EMPTY_REGION)
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
		HTuple Class;
		get_obj_class(m_DetectReg,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiUpperSurfaceBottomRegionLocationAlgo')前需先调用轮廓提取函数('MsiUpperSurfaceRegionExtractAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiUpperSurfaceRegionExtractAlgo')before calling location function('MsiUpperSurfaceBottomRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");

		//剔除区域下方干扰区域
		Hobject RegClipped;
		clip_region_rel(m_DetectReg,&RegClipped,0,VClipSize,LClipSize,LClipSize);

		Hobject RgnMoved,RgnDiff,ROI;
		move_region(RegClipped,&RgnMoved,-MoveSize,0);
		difference(RegClipped,RgnMoved,&ROI);
		Hobject RegErosion;
		erosion_circle(ROI,&RegErosion,ErosionSize);
		Hobject RegOpening,ConnectedRegs,SelectedRegs;
		opening_rectangle1(RegErosion,&RegOpening,OpeningSize,1);
		connection(RegOpening,&ConnectedRegs);
		select_shape_std(ConnectedRegs,&SelectedRegs,"max_area",70);

		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(SelectedRegs, RectangleImage, &RegionDetect);
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
STDMETHODIMP CPccAlgo::MsiUpperSurfaceBottomRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("784$\
					 167;B$\
					 839;LP;1;50;1;15$\
					 832;LP;0;170;1;55$\
					 836;LP;0;120;1;25$\
					 835;LP;1;100;1;30$\
					 781;FP;0.5;80;0.5;3.5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiUpperSurfaceBottomRegionLocationAlgo1(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT*pMoveUpSize,VARIANT* pMoveLowSize)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	int MoveUpSize=(int)pMoveUpSize->fltVal;
	int MoveLowSize=(int)pMoveLowSize->fltVal;
	if (MoveUpSize<=MoveLowSize)
	{
		MoveUpSize=MoveLowSize+10;
	}
	Hobject newRgn;
	if(m_DetectReg.Id() == H_EMPTY_REGION)
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
		HTuple Class;
		get_obj_class(m_BottomSidesRgn,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiUpperSurfaceBottomRegionLocationAlgo1')前需先调用轮廓提取函数('MsiBottomSidesRegionLocationAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiBottomSidesRegionLocationAlgo')before calling location function('MsiUpperSurfaceBottomRegionLocationAlgo1')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		Hobject ImageRect,ConnectedRegions6,SelectedRegions3,Contours,Region,RegionIntersection2,ConnectedRegions2;
		HTuple Row1,Col1,Area4, Row2,Column1;
		gen_rectangle1(&ImageRect, 1, 1, m_imgHeight-1, m_imgWidth-1);
		gen_contour_region_xld(m_DetectReg, &Contours, "border");
		get_contour_xld(Contours, &Row1, &Col1);
		gen_region_points(&Region, Row1, Col1);
		intersection(m_BottomSidesRgn, Region, &RegionIntersection2);
		connection(RegionIntersection2, &ConnectedRegions2);
		area_center(ConnectedRegions2, &Area4, &Row2, &Column1);
		Hobject RegionLines,Rectangle3,CharRect,ROI,RegClipped,RegMoved,CenterRgn;
		Hobject RegErosion;
		HTuple Phi1;
		if (0 != ((Row2.Num())==2))
		{
			gen_region_line(&RegionLines, Row2.Select(0), Column1.Select(0), Row2.Select(1), Column1.Select(1));
			line_orientation(Row2.Select(0), Column1.Select(0), Row2.Select(1), Column1.Select(1), &Phi1);
			//
			Hobject Rectangle33,Rectangle4,RegionDifference4,CharRect1,CharRgn; 
			gen_rectangle2(&Rectangle33, m_CenterRow+100, m_CenterColumn, Phi1, m_imgHeight, MoveUpSize);
			gen_rectangle2(&Rectangle4, m_CenterRow+100, m_CenterColumn, Phi1, m_imgWidth, MoveLowSize);
			difference(Rectangle33, Rectangle4, &RegionDifference4);
			intersection(RegionDifference4, ImageRect, &CharRect1);
			intersection(m_DetectReg, CharRect1,&CharRgn);
			erosion_circle(CharRgn,&RegErosion,ErosionSize);
		}
		else
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		intersection(RegErosion, ImageRect, &RegionDetect);
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
STDMETHODIMP CPccAlgo::MsiUpperSurfaceBottomRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("851$\
					 167;B$\
					 781;FP;0.5;80;0.1;3.5$\
					 837;LP;1;400;1;228$\
					 838;LP;1;400;1;200");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiUpperSurfaceSidesRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pLClipSize)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	int LClipSize=(int)pLClipSize->fltVal;

	Hobject newRgn;
	if(m_DetectReg.Id() == H_EMPTY_REGION)
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
		HTuple Class;
		get_obj_class(m_DetectReg,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiUpperSurfaceRegionLocationAlgo')前需先调用轮廓提取函数('MsiUpperSurfaceRegionExtractAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiUpperSurfaceRegionExtractAlgo')before calling location function('MsiUpperSurfaceRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");

		//剔除区域下方干扰区域
		Hobject RegClipped,ROI;
		clip_region_rel(m_DetectReg,&RegClipped,0,0,LClipSize,LClipSize);
		difference(m_DetectReg,RegClipped,&ROI);
		Hobject RegErosion;
		erosion_circle(ROI,&RegErosion,ErosionSize);

		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(RegErosion, RectangleImage, &RegionDetect);
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
STDMETHODIMP CPccAlgo::MsiUpperSurfaceSidesRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("784$\
					 167;B$\
					 781;FP;0.5;80;0.5;3.5$\
					 836;LP;0;120;1;25"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiVoidCenterLocationAlgo(VARIANT* rgnId)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	Hobject Circle;
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

		gen_circle (&Circle,  Height/2.0, Width/2.0, 50);
		m_vPos[0].m_x = (float)(m_imgWidth/2.0);
		m_vPos[0].m_y = (float)(m_imgHeight/2.0);
		m_vRgn[id] = Circle;

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
STDMETHODIMP CPccAlgo::MsiVoidCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("783$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiGapDetectAlgo(VARIANT* rgnId, VARIANT* pSwitch, VARIANT* pVerticalOrLevel,VARIANT* pMeanMaskSize,VARIANT* pSigma, VARIANT* pLower, VARIANT* pUpper,VARIANT* pLengthSize,VARIANT* pLengthDiff)
{
	int rId = (int)rgnId->fltVal;
	int Switch=(int)pSwitch->fltVal;
	int VerticalOrLevel=(int)pVerticalOrLevel->fltVal;
	int MeanMaskSize = (int)pMeanMaskSize->fltVal;
	float  Sigma   = (float)pSigma->fltVal;
	double Lower   = (double)pLower->fltVal;
	double Upper = (double)pUpper->fltVal;
	float LengthSize   = (float)pLengthSize->fltVal;

	if(Upper<=Lower)
	{
		Upper=Lower+0.5;
	}
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pLengthDiff);
		return S_FALSE;
	}

#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ConnectedRgn;
		HTuple Number;
		connection(rgn,&ConnectedRgn);
		count_obj(ConnectedRgn,&Number);
		Hobject SortedRgn,ObjectSelected;
		//需要画左右两个矩形区域
		if (0!=(Number==2))
		{
			if (Switch)
			{

				Hobject ImageReducedLines,ExpandedImageLines,ImageReducedLines2,ImageReducedLinesMean,GuassLines,GuassLinesReg,DiffOfGauss,ImageConverted,ImageEnhance;
				HTuple LineNumber;
				reduce_domain(Image, rgn, &ImageReducedLines);
				//imageEnhance
				diff_of_gauss(ImageReducedLines, &DiffOfGauss, 1, 10);
				convert_image_type(DiffOfGauss, &ImageConverted, "byte");
				abs_diff_image(ImageReducedLines, ImageConverted, &ImageEnhance, 1);
				//
				Hobject ImageReduced0;
				reduce_domain(ImageEnhance,rgn,&ImageReduced0);
				expand_domain_gray(ImageReduced0, &ExpandedImageLines, 3); 
				if (VerticalOrLevel)
				{
					mean_image(ExpandedImageLines, &ImageReducedLinesMean, 1, MeanMaskSize);  
				}
				else
				{
					mean_image(ExpandedImageLines, &ImageReducedLinesMean, MeanMaskSize, 1);
				}
				reduce_domain(ImageReducedLinesMean, rgn, &ImageReducedLines2);
				lines_gauss(ImageReducedLines2, &GuassLines, Sigma, Lower, Upper, "dark", "true", "bar-shaped", "true");
				Hobject SelectContours;
				select_contours_xld(GuassLines,&SelectContours,"contour_length",LengthSize,999999,-0.5,0.5);
				gen_region_contour_xld(SelectContours,&GuassLinesReg,"filled");
				count_obj(SelectContours, &LineNumber);
				if (0 != (LineNumber!=2))
				{
					retValue = 100;
					m_vErrorRgn[rId] = GuassLinesReg;
					retValue.Detach(pLengthDiff);
					return S_FALSE;
				}
			}
			HTuple Row1,Column1,Row2,Column2,RowBegin,ColBegin,RowEnd,ColEnd,Length1,Length2,Diff,nDiff,LengthDiff1,Nr,Nc,Dist,DistanceDiff;
			Hobject Rect,Contour,CroppedContour,ErosionRgn,UnionContour,RegionLine,LineContour,Regions,ErrorRgns;
			DistanceDiff=0;
			gen_empty_obj(&ErrorRgns);
			sort_region(ConnectedRgn,&SortedRgn,"first_point","true","column");
			for (int Index1=1;Index1<=2;Index1++)
			{
				select_obj(SortedRgn,&ObjectSelected,Index1);
				smallest_rectangle1(ObjectSelected,&Row1,&Column1,&Row2,&Column2);
				gen_rectangle1(&Rect,Row1,Column1,Row2,Column2);
				if (m_DetectReg.Id()==H_EMPTY_REGION)
				{
					retValue = -1;
					m_vErrorRgn[rId].Reset();
					retValue.Detach(pLengthDiff);
					return S_FALSE;
				}
				//
				gen_contour_region_xld(m_DetectReg,&Contour,"center");
				crop_contours_xld(Contour,&CroppedContour,Row1,Column1,Row2,Column2,"false");
				if (CroppedContour.Id()==H_EMPTY_REGION)
				{
					retValue = 100.0;
					Hobject newRgn; 
					m_vErrorRgn[rId] = newRgn;
					retValue.Detach(pLengthDiff);
					return S_FALSE;
				}
				union_adjacent_contours_xld(CroppedContour,&UnionContour,20,1,"attr_keep");
				Hobject EmptyContour,ObjectSelectedContour;
				HTuple Number1,Length3;
				count_obj(UnionContour,&Number1);
				if (0!=(Number1!=1))
				{
					retValue = 100.0;
					Hobject newRgn; 
					m_vErrorRgn[rId] = newRgn;
					retValue.Detach(pLengthDiff);
					return S_FALSE;
				}
				Hobject Region1,RegionLines1;
				HTuple Row8,Col8,MaxDist,Distance;
				gen_region_contour_xld(UnionContour,&Region1,"filled");
				get_contour_xld(UnionContour,&Row8,&Col8);

				gen_region_line(&RegionLines1, Row8.Select(0), Col8.Select(0), Row8.Select((Row8.Num())-1), Col8.Select((Row8.Num())-1));
				distance_pl(Row8, Col8, Row8.Select(0), Col8.Select(0), Row8.Select((Row8.Num())-1), Col8.Select((Row8.Num())-1), &Distance);
				tuple_max(Distance, &MaxDist);
				concat_obj(Region1,ErrorRgns,&ErrorRgns);
				DistanceDiff = MaxDist.Concat(DistanceDiff);
			}
			HTuple MDistanceDiff;
			if (0!=(DistanceDiff.Num()))
			{
				tuple_max(DistanceDiff,&MDistanceDiff);
				retValue=MDistanceDiff[0].I();
				dilation_circle(ErrorRgns,&ErrorRgns,0.5);
				m_vErrorRgn[rId] = ErrorRgns;
			}
		}
		else
		{
			retValue = 100.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		retValue.Detach(pLengthDiff);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pLengthDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CPccAlgo::MsiGapDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("800$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 841;LP;0;1;1;1$\
					 834;LP;0;1;1;1$\
					 733;LP;3;101;1;25$\
					 791;FP;0.1;2;0.1;2$\
					 792;FP;0.1;10;0.1;0.5$\
					 793;FP;0.1;10;0.1;2$\
					 744;FP;1;100;0.5;15");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiAnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
		Hobject ImageReduced_t,ImageReduced,ExpandedImage,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		reduce_domain(Image,rgn,&ImageReduced_t);
		expand_domain_gray(ImageReduced_t,&ExpandedImage,7);
		reduce_domain(ExpandedImage,rgn,&ImageReduced);
		//
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
STDMETHODIMP CPccAlgo::MsiAnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CPccAlgo::MsiCircleRegionLocationAlgo(VARIANT* rgnId,VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pSigma,VARIANT* pLower, VARIANT* pUpper, VARIANT* pAngleDiff)
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

	float  Sigma   = (float)pSigma->fltVal;
	double Lower   = (double)pLower->fltVal;
	double Upper = (double)pUpper->fltVal;
	double AngleDiff=(double)pAngleDiff->fltVal;
	if(Upper<=Lower)
	{
		Upper=Lower+0.5;
	}
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
		//剔除白线干扰区域
		Hobject Image = m_crtImg;
		Hobject ImageReduced_t,ImageReduced1;	
		reduce_domain (Image, RegionDetect, &ImageReduced_t);
		Hobject ExpandedImage;
		expand_domain_gray(ImageReduced_t,&ExpandedImage,7);
		reduce_domain(ExpandedImage,RegionDetect,&ImageReduced1);

		Hobject Lines,UnionContours,EmptyRect;
		HTuple Length,RowBegin,ColBegin,RowEnd,ColEnd,Nr,Nc,Dist,Number,pi;
		lines_gauss(ImageReduced1,&Lines,Sigma, Lower, Upper,"light","true","bar-shaped","true");
		union_collinear_contours_xld(Lines,&UnionContours,10,1,2,0.1,"attr_keep");
		fit_line_contour_xld(UnionContours,"tukey",-1,0,5,2,&RowBegin,&ColBegin,&RowEnd,&ColEnd,&Nr,&Nc,&Dist);
		count_obj(UnionContours,&Number);
		pi=(HTuple(0).Acos())*2;
		gen_empty_obj(&EmptyRect);
		Hobject ObjectSelected,RegionLines,Rect0,Region1;
		HTuple RegionArea,Row,Column,Angle1,Angle2,RowC,ColumnC,PhiC,Length1C,Length2C;

		for (int i=1;i<=Number;i++)
		{
			select_obj(UnionContours,&ObjectSelected,i);
			gen_region_contour_xld(ObjectSelected,&Region1,"margin");
			area_center(Region1,&RegionArea,&Row,&Column);
			gen_region_line(&RegionLines,fCenterY,fCenterX,Row,Column);

			orientation_region(RegionLines,&Angle1);
			orientation_region(Region1,&Angle2);
			if(0!=(Angle2<0))
			{
				Angle2=Angle2+pi;
			}

			if(0!=(((Angle1-Angle2).Abs())<AngleDiff))
			{
				smallest_rectangle2(Region1,&RowC,&ColumnC,&PhiC,&Length1C,&Length2C);
				gen_rectangle2(&Rect0,RowC,ColumnC,PhiC,Length1C,Length2C+4);
				union2(Rect0,EmptyRect,&EmptyRect);
			}
		}	
		Hobject ImageReduced,ImageReduced2,ExpandedImage0;
		difference(RegionDetect,EmptyRect,&RegionDetect);
		union1(RegionDetect,&RegionDetect);

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
STDMETHODIMP CPccAlgo::MsiCircleRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("794$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 787;FP;0.1;2;0.1;1.5$\
					 788;FP;0.1;10;0.1;1$\
					 789;FP;0.1;10;0.1;3$\
					 790;FP;0.01;10;0.01;0.05"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiDetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile,VARIANT* pSigma,VARIANT* pLower, VARIANT* pUpper, VARIANT* pAngle, VARIANT* pCurv)
{
	int id = (int)rgnId->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
	int RingOrOCR     = (int)pRingOrOCR->fltVal;
	//
	Hobject newRgn;
	HTuple tmp;
	tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
	tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
	m_vRgnReadData[id] = tmp;
	//
	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
	if(RingOrOCR < 1 || RingOrOCR > 2)
	{
		m_vRgn[id].Reset();
		return S_FALSE;
	}

	CString sRgnPath1  = *bstrRgnFile;
	string strRgnPath1 = CT2A(sRgnPath1);
	CString sRgnPath2  = *bstrExtractRgnFile;
	string strRgnPath2 = CT2A(sRgnPath2);

	int Angle = (int)pAngle->fltVal;
	float Curv = (float)pCurv->fltVal;

	float  Sigma   = (float)pSigma->fltVal;
	double Lower   = (double)pLower->fltVal;
	double Upper = (double)pUpper->fltVal;
	if(Upper<=Lower)
	{
		Upper=Lower+0.5;
	}
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			if(!_access(strRgnPath1.c_str(),0))
			{
				Hobject retRgn;
				read_region(&retRgn,strRgnPath1.c_str());
				m_dictCrtPathRgn[strRgnPath1] = retRgn;
				m_dictNewPathRgn[strRgnPath1] = retRgn;
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
			HTuple tmp;
			tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
			tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
			m_vRgnReadData[id] = tmp;

			if(RingOrOCR==2)
			{
				if(!_access(strRgnPath2.c_str(),0))
				{
					Hobject retRgn;
					read_region(&retRgn,strRgnPath2.c_str());
					m_dictCrtPathRgn[strRgnPath2] = retRgn;
					m_dictNewPathRgn[strRgnPath2] = retRgn;
				}
				else
				{
					if(m_isChineseLanguage)
					{
						MessageBox(NULL,_T("初始化参数,剔除区域路径设置错误。"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					else
					{
						MessageBox(NULL, _T("Initialization parameter（deducts region path）setting is wrong!"),_T("Initial configuration is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					}
					m_vRgn[id].Reset();
					return S_FALSE;
				}
				HTuple tmp;
				tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
				tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
				m_vRgnReadData[id] = tmp;
			}
		}

		m_vRgn[id] = newRgn;
		//
		HTuple dataLength;
		tuple_length (m_modelMatchingPARA, &dataLength);
		if(dataLength < 3)
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiDetectRegionLocationAlgo')前需先调用匹配函数('MatchingParaAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MatchingParaAlgo')before calling location function('MsiDetectRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		if(m_modelMatchingPARA[0].D() < 1.0)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject RegionDetect;

		HTuple modelPARAID = m_vRgnReadData[id];
		if(RingOrOCR==1)
		{	
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn ;
				return S_FALSE;
			}
			Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
			//
			//HTuple modelPARAIDLength;
			//tuple_length(modelPARAID,&modelPARAIDLength);
			//if(modelPARAIDLength[0].I()<6)
			//{
			//	MessageBox(NULL,_T("初始化参数（模板路径与检测区域路径）或区域编号设置错误"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			//	m_vRgn[id] = newRgn ;
			//	return S_FALSE;
			//}
			m_modelPARA = modelPARAID;
			//
			Hobject  RegionAffineTrans;
			//HTuple ringHomMat2D;
			//vector_angle_to_rigid(m_modelPARA[1].D(),m_modelPARA[2].D(),m_modelPARA[3].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&ringHomMat2D);
			//affine_trans_region(roiRgn,&RegionAffineTrans,ringHomMat2D,"false");
			//
			HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted;
			vector_angle_to_rigid(0, 0, m_modelPARA[3].D(), 0, 0, m_modelMatchingPARA[3].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

			Hobject rotatedRgn,RegionMoved1;
			HTuple RowModelData,ColModelData;
			RowModelData=m_modelPARA[1];
			ColModelData=m_modelPARA[2];
			move_region (roiRgn, &RegionMoved1,-RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_modelMatchingPARA[1];
			ColModelData=m_modelMatchingPARA[2];
			move_region (rotatedRgn,&RegionAffineTrans,RowModelData,ColModelData) ;
			//
			if(ErosionSize>=0.5)
			{
				erosion_circle (RegionAffineTrans, &RegionDetect,ErosionSize);
			}
			else if(ErosionSize<-0.5)
			{
				dilation_circle(RegionAffineTrans, &RegionDetect,abs(ErosionSize));
			}
			else
			{
				RegionDetect = RegionAffineTrans;
			}
		}
		else if(RingOrOCR==2)
		{
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}
			Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
			HTuple modelPARAID = m_vRgnReadData[id];

			//HTuple modelPARAIDLength;
			//tuple_length(modelPARAID,&modelPARAIDLength);
			//if(modelPARAIDLength[0].I()<6)
			//{
			//	MessageBox(NULL,_T("仅同时存在图案与拉环两个模板时‘盖面图案’参数可选择('2')"),_T("参数设置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			//	m_vRgn[id] = newRgn;
			//	return S_FALSE;
			//}
			m_modelPARA[0] = modelPARAID[0];
			m_modelPARA[1] = modelPARAID[1];
			m_modelPARA[2] = modelPARAID[2];
			m_modelPARA[3] = modelPARAID[3];
			m_modelPARA[4] = modelPARAID[4];
			m_modelPARA[5] = modelPARAID[5];
			m_modelPARA[6] = modelPARAID[6];
			//
			Hobject  RegionAffineTrans,OCRRegionAffineTrans,RegionDiff;
			//HTuple ringHomMat2D,OCRHomMat2D;
			//vector_angle_to_rigid(m_modelPARA[1].D(),m_modelPARA[2].D(),m_modelPARA[3].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&ringHomMat2D);
			//affine_trans_region(roiRgn,&RegionAffineTrans,ringHomMat2D,"false");
			//
			HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted;
			vector_angle_to_rigid(0, 0, m_modelPARA[3].D(), 0, 0, m_modelMatchingPARA[3].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

			Hobject rotatedRgn,RegionMoved1;	
			HTuple RowModelData,ColModelData;
			RowModelData=m_modelPARA[1];
			ColModelData=m_modelPARA[2];
			move_region (roiRgn, &RegionMoved1,-RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_modelMatchingPARA[1];
			ColModelData=m_modelMatchingPARA[2];
			move_region (rotatedRgn,&RegionAffineTrans,RowModelData,ColModelData) ;
			//
			//vector_angle_to_rigid(m_modelPARA[4].D(),m_modelPARA[5].D(),m_modelPARA[6].D(),m_modelMatchingPARA[4].D(),m_modelMatchingPARA[5].D(),m_modelMatchingPARA[6].D(),&OCRHomMat2D);

			if(m_dictCrtPathRgn.find(strRgnPath2) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}
			Hobject rgnExtract = m_dictCrtPathRgn[strRgnPath2];
			//affine_trans_region(rgnExtract,&OCRRegionAffineTrans,OCRHomMat2D,"false");
			//
			vector_angle_to_rigid(0, 0, m_modelPARA[6].D(), 0, 0, m_modelMatchingPARA[6].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
			RowModelData=m_modelPARA[4];
			ColModelData=m_modelPARA[5];
			move_region (rgnExtract, &RegionMoved1,-RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_modelMatchingPARA[4];
			ColModelData=m_modelMatchingPARA[5];
			move_region (rotatedRgn,&OCRRegionAffineTrans,RowModelData,ColModelData) ;
			//
			difference (RegionAffineTrans, OCRRegionAffineTrans, &RegionDiff);
			RegionAffineTrans = RegionDiff;
			if(ErosionSize>=0.5)
			{
				erosion_circle (RegionAffineTrans, &RegionDetect,ErosionSize);
			}
			else if(ErosionSize<-0.5)
			{
				dilation_circle(RegionAffineTrans, &RegionDetect,abs(ErosionSize));
			}
			else
			{
				RegionDetect=RegionAffineTrans;
			}
		}
		//
		Hobject Image = m_crtImg;
		//剔除侧面竖线干扰区域
		Hobject ImageReduced0,ImageMean0,ExpandedImageMean0,ExpandedImageReduced0,Lines,rgn;
		reduce_domain(Image,RegionDetect,&ImageReduced0);
		mean_image(ImageReduced0,&ImageMean0,1,7);
		expand_domain_gray(ImageMean0,&ExpandedImageMean0,7);
		reduce_domain(ExpandedImageMean0,RegionDetect,&ExpandedImageReduced0);
		lines_gauss(ExpandedImageReduced0,&Lines,1.5,1,3,"dark","true","bar-shaped","true");
		HTuple pi,Eps;
		Hobject SelectedContours,UnionContours;
		pi=(HTuple(0).Acos())*2;
		Eps=pi/Angle;
		select_contours_xld(Lines, &SelectedContours, "direction", (pi/2)-Eps, (pi/2)+Eps, -0.5, 0.5);
		union_adjacent_contours_xld(SelectedContours, &UnionContours, 10, 1, "attr_keep");
		HTuple RowBegin, ColBegin,RowEnd, ColEnd, Nr, Nc, Dist,Distance,Length,Number;
		Hobject RemoveReg;

		fit_line_contour_xld(UnionContours, "tukey", -1, 0, 5, 2, & RowBegin, & ColBegin, & RowEnd, & ColEnd, & Nr, & Nc, & Dist);
		distance_pp( RowBegin,  ColBegin,  RowEnd,  ColEnd, &Distance);
		length_xld( UnionContours, &Length);
		count_obj( UnionContours, &Number);
		gen_empty_obj(&RemoveReg);

		Hobject LineRegSelected,RegionErosion,RegionLinesDilation,RegionDifference1,ConnectedRegions1,Rectangle,RemoveRegUnion,ObjectSelected;
		HTuple Number1,Row, Column, Phi1, Length1, Length2;
		for(int Index=1;Index<=Number;Index++)
		{
			if (0 != (((( Distance.Select( Index-1))-( Length.Select( Index-1))).Abs())<Curv))
			{
				select_obj( UnionContours, & ObjectSelected,  Index);
				gen_region_line(& LineRegSelected,  RowBegin.Select( Index-1),ColBegin.Select(Index-1),RowEnd.Select( Index-1), ColEnd.Select( Index-1));
				erosion_rectangle1(RegionDetect, &RegionErosion, 9, 9);
				dilation_rectangle1(LineRegSelected, & RegionLinesDilation, 2, 1);
				difference(RegionErosion, RegionLinesDilation, & RegionDifference1);
				connection(RegionDifference1, &ConnectedRegions1);
				count_obj(ConnectedRegions1, & Number1);
				if (0 != (Number1>1))
				{
					smallest_rectangle2_xld( ObjectSelected, & Row, & Column, & Phi1, & Length1, & Length2);
					gen_rectangle2(&Rectangle, Row, Column, Phi1, Length1+2, Length2+2);
					union2(Rectangle,RemoveReg,&RemoveReg);
					union1(RemoveReg,&RemoveRegUnion);
				}
			}
		}
		HTuple Width,Height;
		Hobject Rect;
		difference(RegionDetect,RemoveRegUnion,&RegionDetect);
		union1(RegionDetect,&RegionDetect);
		get_image_size(Image,&Width,&Height);
		gen_rectangle1(&Rect,0,0,Height-1,Width-1);
		intersection(Rect,RegionDetect,&RegionDetect);
		//
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
STDMETHODIMP CPccAlgo::MsiDetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("442$\
					 167;B$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 441;LP;1;2;1;1$\
					 267;SP$\
					 232;SP$\
					 791;FP;0.1;2;0.1;1.5$\
					 792;FP;0.1;10;0.1;1$\
					 793;FP;0.1;10;0.1;3$\
					 785;LP;0;200;1;60$\
					 786;FP;0.01;5;0.1;1" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

/*************************************************************************
** 函数名：MsiEndSidesRegionLocationAlgo
** 功  能：定位单接口侧面拍摄到的上表面耳边区域
**************************************************************************/
STDMETHODIMP CPccAlgo::MsiEndSidesRegionLocationAlgo(VARIANT* rgnId,VARIANT* pRgnSize,VARIANT* pErosionSize,VARIANT* pRowDist)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	int RgnSize=(int)pRgnSize->fltVal;
	int ErosionSize=(int)pErosionSize->fltVal;
	float RowDist=(float)pRowDist->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

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
		HTuple Class;
		get_obj_class(m_DetectReg,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiEndSidesRegionLocationAlgo')前需先调用轮廓提取函数('MsiUpperSurfaceRegionExtractAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiUpperSurfaceRegionExtractAlgo')before calling location function('MsiEndSidesRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		Hobject RegionClipped,RegionDifference,ConnectedRegions;
		HTuple Number;
		clip_region_rel(m_DetectReg,&RegionClipped,0,0,RgnSize,RgnSize);
		difference(m_DetectReg, RegionClipped, &RegionDifference);
		connection(RegionDifference, &ConnectedRegions);
		Hobject SelectRegs;
		select_shape(ConnectedRegions,&SelectRegs,"area","and",100,99999);
		count_obj(SelectRegs, &Number);
		Hobject UnionRgns;
		union1(ConnectedRegions,&UnionRgns);
		if (0!=(Number!=2))
		{
			m_vRgn[id] = UnionRgns ;
			return S_FALSE;
		}
		Hobject SortedRegions,ObjectLeft,LeftRgn,ZleftRgn,Rect1,Rect2;
		HTuple RowL, ColumnL, RowL1, ColumnL1,RowS, ColumnS, RowS1, ColumnS1;
		sort_region(SelectRegs, &SortedRegions, "first_point", "true", "column");
		select_obj(SortedRegions, &ObjectLeft, 1);
		inner_rectangle1(ObjectLeft, &RowL, &ColumnL, &RowL1, &ColumnL1);
		gen_rectangle1(&Rect1,RowL, ColumnL, RowL1, ColumnL1);
		smallest_rectangle1(ObjectLeft,&RowS, &ColumnS, &RowS1, &ColumnS1);
		gen_rectangle1(&Rect2,RowS, ColumnS,RowS1, ColumnS1);
		HTuple areaRect1,rowRect1,colRect1,areaRect2,rowRect2,colRect2;
		area_center(Rect1,&areaRect1,&rowRect1,&colRect1);
		area_center(Rect2,&areaRect2,&rowRect2,&colRect2);
		HTuple RowDiff;
		RowDiff=0;
		RowDiff=rowRect1-rowRect2;
		if (RowDiff>RowDist)
		{
			gen_rectangle1(&LeftRgn,RowS, ColumnS-RgnSize, RowL1, ColumnL1);
		}
		else if(RowDiff<-RowDist)
		{
			gen_rectangle1(&LeftRgn,RowL, ColumnS-RgnSize, RowS1, ColumnL1);
		}
		else
		{
			gen_rectangle1(&LeftRgn,RowL, ColumnS-RgnSize, RowL1, ColumnL1);
		}
		erosion_rectangle1(LeftRgn,&ZleftRgn,1,ErosionSize);

		Hobject ObjectRight,RightRgn,ZRightRgn,Rect3,Rect4;
		HTuple RowR, ColumnR, RowR1, ColumnR1,RowT, ColumnT, RowT1, ColumnT1;
		select_obj(SortedRegions, &ObjectRight, 2);
		inner_rectangle1(ObjectRight, &RowR, &ColumnR, &RowR1, &ColumnR1);
		gen_rectangle1(&Rect3,RowR, ColumnR, RowR1, ColumnR1);
		smallest_rectangle1(ObjectRight,&RowT, &ColumnT, &RowT1, &ColumnT1);
		gen_rectangle1(&Rect4,RowT, ColumnT, RowT1, ColumnT1);
		HTuple areaRect3,rowRect3,colRect3,areaRect4,rowRect4,colRect4;
		area_center(Rect3,&areaRect3,&rowRect3,&colRect3);
		area_center(Rect4,&areaRect4,&rowRect4,&colRect4);
		HTuple RowDiff1;
		RowDiff1=0;
		RowDiff1=rowRect3-rowRect4;
		if (RowDiff1>RowDist)
		{
			gen_rectangle1(&RightRgn, RowT, ColumnR, RowR1, ColumnT1+RgnSize);
		}
		else if(RowDiff1<-RowDist)
		{
			gen_rectangle1(&RightRgn, RowR, ColumnR, RowT1, ColumnT1+RgnSize);
		}
		else
		{
			gen_rectangle1(&RightRgn, RowR, ColumnR, RowR1, ColumnT1+RgnSize);
		}
		erosion_rectangle1(RightRgn,&ZRightRgn,1,ErosionSize);

		Hobject TotalRgn;
		union2(ZleftRgn,ZRightRgn,&TotalRgn);

		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(TotalRgn, RectangleImage, &RegionDetect);
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
STDMETHODIMP CPccAlgo::MsiEndSidesRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("796$\
					 167;B$\
					 829;LP;5;20;1;10$\
					 821;LP;1;80;1;20$\
					 823;FP;3;15;0.5;8"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*************************************************************************
** 函数名：MsiEndSidesRegionLocationAlgo1
** 功  能：定位单接口侧面拍摄到的上表面耳边区域,可以实现上下右自由缩放
** 时  间：2019年10月10日
**************************************************************************/
STDMETHODIMP CPccAlgo::MsiEndSidesRegionLocationAlgo1(VARIANT* rgnId,VARIANT* pOuterSize,VARIANT* pInnerSize,VARIANT* pUpSize,VARIANT* pDownSize)
{
	//得到检测区域
	int id         = (int)rgnId->fltVal;
	float  OuterSize = (float)pOuterSize->fltVal;
	float  InnerSize = (float)pInnerSize->fltVal;
	float  UpSize  = (float)pUpSize->fltVal;
	float DownSize = (float)pDownSize->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

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
		HTuple Class;
		get_obj_class(m_DetectReg,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiEndSidesRegionLocationAlgo')前需先调用轮廓提取函数('MsiUpperSurfaceRegionExtractAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiUpperSurfaceRegionExtractAlgo')before calling location function('MsiEndSidesRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		Hobject RegionClipped,RegionDifference,ConnectedRegions;
		HTuple Number;
		clip_region_rel(m_DetectReg,&RegionClipped,0,0,(Hlong)InnerSize,(Hlong)InnerSize);

		/*Hobject rgnMovedRight,rgnMovedLeft,rgnIntesct1,rgnIntesct2 ;
		move_region(RegionClipped,&rgnMovedRight,0,(Hlong)OuterSize );
		move_region(RegionClipped,&rgnMovedLeft,0,(Hlong)OuterSize );

		intersection(RegionClipped,rgnMovedRight,&rgnIntesct1 );
		intersection(RegionClipped,rgnMovedLeft,&rgnIntesct2 );

		union2(rgnIntesct2,rgnIntesct1,&RegionClipped );
		union1(RegionClipped,&RegionClipped);*/

		difference(m_DetectReg, RegionClipped, &RegionDifference);
		connection(RegionDifference, &ConnectedRegions);
		Hobject SelectRegs;
		select_shape(ConnectedRegions,&SelectRegs,"area","and",100,99999);
		// 上下位置缩放
		clip_region_rel(SelectRegs,&SelectRegs,(Hlong)UpSize,(Hlong)DownSize,0,0 );

		// 左右位置缩放
		Hobject SelectRegsCon,objSelL,objSelR;
		connection(SelectRegs,&SelectRegsCon );
		count_obj(SelectRegsCon, &Number);
		if (Number!=2)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		select_obj(SelectRegsCon,&objSelL,1 );
		clip_region_rel(objSelL,&objSelL,0,0,(Hlong)OuterSize,0 );
		select_obj(SelectRegsCon,&objSelR,2 );
		clip_region_rel(objSelR,&objSelR,0,0,0,(Hlong)OuterSize );

		union2(objSelR,objSelL,&SelectRegs );
		
		m_rgnBorderLR = SelectRegs;

		// 输出区域
		Hobject TotalRgn;
		union1(SelectRegs,&TotalRgn);

		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(TotalRgn, RectangleImage, &RegionDetect);
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
STDMETHODIMP CPccAlgo::MsiEndSidesRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("6160$\
					 167;B$\
					 6164;FP;0;20;0.5;1$\
					 6163;FP;5;50;0.5;20$\
					 6010;FP;0;50;0.5;5$\
					 6012;FP;0;50;0.5;5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*************************************************************************
** 函数名：MsiBorderRubberLessDetectAlgo
** 功  能：检测单接口侧面拍摄到的上表面耳边区域的各种位置的缺胶检测
** 时  间：2019年10月10日
**************************************************************************/
STDMETHODIMP CPccAlgo::MsiBorderRubberLessDetectAlgo(VARIANT* rgnId, VARIANT* pErosionSize,VARIANT* pAreaSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;

	float ErosionSize      = (float)pErosionSize->fltVal;
	int AreaSize = (int)pAreaSize->fltVal;

	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	union1(rgn,&rgn);
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	Hobject rgnBorderLR;
	union1(m_rgnBorderLR,&rgnBorderLR );
	Hobject SelectRegs = rgnBorderLR;
    HTuple dArea,dRow,dCol;
	area_center(rgnBorderLR,&dArea,&dRow,&dCol );
	if (dArea[0].D() <= 0)
	{
		retValue = -1;
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
		// get the image
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;

		HTuple Number ;
		connection(SelectRegs,&SelectRegs );
		count_obj(SelectRegs, &Number);
	
		if (Number!=2)
		{
			m_vRgn[rId] = rgnBorderLR ;
			return S_FALSE;
		}

		// 左边区域
		Hobject SortedRegions,ObjectLeft,rgnRectL,rgnRectSmlL;
		HTuple RowL, ColumnL, RowL1, ColumnL1,RowS, ColumnS, RowS1, ColumnS1;
		sort_region(SelectRegs, &SortedRegions, "first_point", "true", "column");
		select_obj(SortedRegions, &ObjectLeft, 1);
		inner_rectangle1(ObjectLeft, &RowL, &ColumnL, &RowL1, &ColumnL1);
		gen_rectangle1(&rgnRectL,RowL, ColumnL, RowL1, ColumnL1);
		smallest_rectangle1(ObjectLeft,&RowS, &ColumnS, &RowS1, &ColumnS1);
		gen_rectangle1(&rgnRectSmlL,RowS, ColumnS,RowS1, ColumnS1);

		Hobject rgnRectDiffL;
		difference(rgnRectSmlL,rgnRectL,&rgnRectDiffL );
		opening_circle(rgnRectDiffL,&rgnRectDiffL,ErosionSize);

		// 右边区域
		Hobject ObjectRight,rgnRectR,rgnRectSmlR;
		HTuple RowR, ColumnR, RowR1, ColumnR1,RowT, ColumnT, RowT1, ColumnT1;
		select_obj(SortedRegions, &ObjectRight, 2);
		inner_rectangle1(ObjectRight, &RowR, &ColumnR, &RowR1, &ColumnR1);
		gen_rectangle1(&rgnRectR,RowR, ColumnR, RowR1, ColumnR1);
		smallest_rectangle1(ObjectRight,&RowT, &ColumnT, &RowT1, &ColumnT1);
		gen_rectangle1(&rgnRectSmlR,RowT, ColumnT, RowT1, ColumnT1);

		Hobject rgnRectDiffR;
		difference(rgnRectSmlR,rgnRectR,&rgnRectDiffR );
		opening_circle(rgnRectDiffR,&rgnRectDiffR,ErosionSize); 

		Hobject rgnRectDiff,rgnRectDiffCon,rgnRectDiffSel,rgnError;
		union2(rgnRectDiffR,rgnRectDiffL,&rgnRectDiff);
		difference(rgnRectDiff,rgnBorderLR ,&rgnRectDiff );
		connection(rgnRectDiff,&rgnRectDiffCon);
		select_shape(rgnRectDiffCon,&rgnRectDiffSel,"area","and",AreaSize ,9999999 );

		union1(rgnRectDiffSel,&rgnError );
		HTuple  dArea1,dRow1,dCol1;
		area_center(rgnError,&dArea1,&dRow1,&dCol1);

		if(dArea1 <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			Hobject rgnDilation;
			retValue = dArea1[0].I();
			dilation_circle (rgnError, &rgnDilation, 0.5);
			m_vErrorRgn[rId] = rgnDilation;

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
STDMETHODIMP CPccAlgo::MsiBorderRubberLessDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6161$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 864;FP;1;30;0.5;3.5$\
					 6162;LP;1;250;1;20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*************************************************************************
** 函数名：MsiTopWholeRegionLocationAlgo
** 功  能：定位单接口顶部工位相机拍摄到的整体区域
** 时  间：2019年10月11日
**************************************************************************/
STDMETHODIMP CPccAlgo::MsiTopWholeRegionLocationAlgo(VARIANT* rgnId,VARIANT* pThreshold,VARIANT* pOpnSize)
{
	//得到检测区域
	int id         = (int)rgnId->fltVal;
	float  Threshold = (float)pThreshold->fltVal;                      // 90
	float  OpnSize = (float)pOpnSize->fltVal;                          // 1.5
	
	Hobject newRgn;
	m_vRgn[id] = newRgn;

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
		
		Hobject Image = m_crtImg;
		HTuple Height, Width ;
		get_image_size(Image,&Width,&Height );

		Hobject  rgnEllipse,ImageReduced;
		gen_ellipse(&rgnEllipse, Height/2.0, Width/2.0, 0, Width/3, Height/2.2);
		reduce_domain(Image, rgnEllipse, &ImageReduced);

		Hobject rgnThr,rgnFil,rgnOpn,rgnDil,rgnCon,rgnSel ;
		threshold(ImageReduced, &rgnThr, Threshold, 255);
		fill_up(rgnThr, &rgnFil);
		opening_circle(rgnFil, &rgnOpn, OpnSize);
		dilation_circle(rgnOpn, &rgnDil, OpnSize*3.5);
		connection(rgnDil, &rgnCon);
		select_shape_std(rgnCon, &rgnSel, "max_area", 70);

		m_rgnTopWhole = rgnSel;
		m_vRgn[id] = rgnSel ;	
		
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
STDMETHODIMP CPccAlgo::MsiTopWholeRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6165$\
					 167;B$\
					 6166;FP;1;250;0.5;90$\
					 6167;FP;0;50;0.5;1.5"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*************************************************************************
** 函数名：TopRubberLessDetectAlgo
** 功  能：检测单接口顶部拍摄的缺胶类缺陷
** 时  间：2019年10月11日
**************************************************************************/
STDMETHODIMP CPccAlgo::TopRubberLessDetectAlgo(VARIANT* rgnId, VARIANT* pDiagonalDist,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;

	int DiagonalDist = (int)pDiagonalDist->fltVal;

	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	union1(rgn,&rgn);
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	Hobject rgnTopWhole;
	rgnTopWhole = m_rgnTopWhole;
	union1(rgnTopWhole,&rgnTopWhole );
	HTuple dArea1,dRow1,dCol1;
	area_center(rgnTopWhole,&dArea1,&dRow1,&dCol1 );
	if (dArea1[0].D() <= 0)
	{
		retValue = -1;
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
		// get the image
		Hobject Image = m_crtImg;
		Hobject rgnSel  = rgnTopWhole;

		HTuple dArea,dRow,dCol;
		area_center(rgnSel,&dArea,&dRow,&dCol );
		if (dArea <= 0 )
		{
			Hobject newRgn;
			m_vRgn[rId] = newRgn;
		    return S_FALSE;
		}
		
		HTuple Row, Column, Phi, Length1, Length2 ;
		Hobject Rectangle ;
		smallest_rectangle2(rgnSel, &Row, &Column, &Phi, &Length1, &Length2);
		gen_rectangle2(&Rectangle, Row, Column, Phi, Length1, Length2);

		HTuple dDistMin,dDistMax;
		distance_pr(Rectangle,Row,Column,&dDistMin,&dDistMax);

		if(dDistMax[0].I()*2 < DiagonalDist)
		{
			Hobject rgnDilation;
			retValue = dArea[0].I();
			dilation_circle (rgnSel, &rgnDilation, 0.5);
			m_vErrorRgn[rId] = rgnDilation;	
		}
		else
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
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
STDMETHODIMP CPccAlgo::TopRubberLessDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6168$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 6169;LP;1;1000;1;420");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiBottomSidesRegionLocationAlgo(VARIANT* rgnId,VARIANT* pHp,VARIANT* pVp,VARIANT* pWidth,VARIANT* pHeight,VARIANT* pMovedSize,VARIANT* pErosionSize)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	int Hp=(int)pHp->fltVal;
	int Vp=(int)pVp->fltVal;
	int Width=(int)pWidth->fltVal;
	int Height=(int)pHeight->fltVal;
	int MovedSize=(int)pMovedSize->fltVal;
	int ErosionSize=(int)pErosionSize->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

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
		HTuple Class;
		get_obj_class(m_DetectReg,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiBottomSidesRegionLocationAlgo')前需先调用轮廓提取函数('MsiUpperSurfaceRegionExtractAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiUpperSurfaceRegionExtractAlgo')before calling location function('MsiBottomSidesRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		Hobject RegionClipped,RegionDifference,ConnectedRegions;
		HTuple Number;
		clip_region_rel(m_DetectReg,&RegionClipped,0,50,Hp,Hp);
		difference(m_DetectReg, RegionClipped, &RegionDifference);
		connection(RegionDifference, &ConnectedRegions);
		//
		Hobject SelectRegs1;
		select_shape(ConnectedRegions,&SelectRegs1,"area","and",2000,999999);
		//
		count_obj(SelectRegs1, &Number);
		if (0!=(Number!=3))
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject SortedRegions,ObjectLeft,LeftRgn,ZleftRgn,Rect1,Rect2,RegionUnion;
		HTuple RowL, ColumnL, RowL1, ColumnL1,RowS, ColumnS, RowE, ColumnE;
		sort_region(SelectRegs1, &SortedRegions, "first_point", "true", "column");
		select_obj(SortedRegions, &ObjectLeft, 1);
		HTuple Area1,Area2;
		Hobject ObjectSelected1;
		area_center(ObjectLeft, &Area1, &RowS, &ColumnS);
		select_obj(SortedRegions, &ObjectSelected1, Number);
		area_center(ObjectSelected1, &Area2, &RowE, &ColumnE);
		union2(ObjectLeft, ObjectSelected1, &RegionUnion);

		Hobject RegionClipped2,RegionDifference2,ConnectedRegions4,SortedRegions2;
		HTuple Number3;
		clip_region_rel(m_DetectReg, &RegionClipped2, 0, 0, Hp-10, Hp-10);
		difference(m_DetectReg, RegionClipped2, &RegionDifference2);
		connection(RegionDifference2, &ConnectedRegions4);
		//
		Hobject SelectRegs2;
		select_shape(ConnectedRegions4,&SelectRegs2,"area","and",2000,999999);
		//
		count_obj(SelectRegs2, &Number3);
		if (0!=(Number3!=2))
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		sort_region(SelectRegs2, &SortedRegions2, "first_point", "true", "column");
		Hobject ObjectSelected2,ObjectSelected3,RegionUnion2,RegionDifference3,ConnectedRegions5;
		HTuple Number4;
		select_obj(SortedRegions2, &ObjectSelected2, 1);
		select_obj(SortedRegions2, &ObjectSelected3, Number3);
		union2(ObjectSelected2, ObjectSelected3, &RegionUnion2);
		difference(RegionUnion, RegionUnion2, &RegionDifference3);
		connection(RegionDifference3, &ConnectedRegions5);
		count_obj(ConnectedRegions5, &Number4);
		Hobject ObjectSelected6;
		HTuple Area, CenterRow, CenterColumn;
		select_obj(SortedRegions, &ObjectSelected6, 2);
		area_center(ObjectSelected6, &Area, &CenterRow, &CenterColumn);
		m_CenterRow=CenterRow;
		m_CenterColumn=CenterColumn;

		Hobject ObjectSelected4,ZLeftROI,ObjectSelected5,ZRightROI,LeftROI,RightROI;
		HTuple Row13, Column13,Row23, Column23,Row14, Column14, Row24, Column24;
		if (0 != (Number4==2))
		{
			sort_region(ConnectedRegions5, &SortedRegions, "first_point", "true", "column");
			select_obj(SortedRegions, &ObjectSelected4, 1);
			inner_rectangle1(ObjectSelected4, &Row13, &Column13, &Row23, &Column23);
			gen_rectangle2(&LeftROI, Row23, (ColumnS+((CenterColumn-ColumnS)/2))-Vp, 0, Width, Height);
			erosion_rectangle1(LeftROI, &ZLeftROI, ErosionSize, ErosionSize);
			select_obj(SortedRegions, &ObjectSelected5, 2);
			inner_rectangle1(ObjectSelected5, &Row14, &Column14, &Row24, &Column24);
			gen_rectangle2(&RightROI, Row24, (CenterColumn+((ColumnE-CenterColumn)/2))+Vp, 0, Width, Height);
			erosion_rectangle1(RightROI, &ZRightROI, ErosionSize, ErosionSize);
		}
		else
		{
			m_vRgn[id] = ConnectedRegions5 ;
			return S_FALSE;

		}
		Hobject TotalRgn,TotalRgnMoved;
		union2(ZLeftROI, ZRightROI, &TotalRgn);
		move_region(TotalRgn,&TotalRgnMoved,MovedSize,0);

		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(TotalRgnMoved, RectangleImage, &RegionDetect);
		m_vRgn[id] = RegionDetect ;	
		m_BottomSidesRgn=RegionDetect;
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
STDMETHODIMP CPccAlgo::MsiBottomSidesRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("824$\
					 167;B$\
					 826;LP;30;150;1;110$\
					 825;LP;0;100;1;20$\
					 827;LP;0;50;1;20$\
					 828;LP;0;50;1;20$\
					 840;LP;0;50;1;5$\
					 821;LP;1;50;1;1"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiWhiteReinforcedRegionLocationAlgo(VARIANT* rgnId,VARIANT* pRegHeight,VARIANT* pRegWidth,VARIANT* pThreshValue,VARIANT* pErosionSizeWidth,VARIANT* pErosionSizeHeight)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	int RegHeight=(int)pRegHeight->fltVal;
	int RegWidth=(int)pRegWidth->fltVal;
	int ThreshValue=(int)pThreshValue->fltVal;
	int ErosionSizeWidth=(int)pErosionSizeWidth->fltVal;
	int ErosionSizeHeight=(int)pErosionSizeHeight->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

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
		HTuple Class;
		Hobject Image = m_crtImg;
		get_obj_class(m_BottomSidesRgn,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiWhiteReinforcedRegionLocationAlgo')前需先调用轮廓提取函数('MsiBottomSidesRegionLocationAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiBottomSidesRegionLocationAlgo')before calling location function('MsiWhiteReinforcedRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		Hobject ImageRect,ConnectedRegions6,SelectedRegions3,Contours,Region,RegionIntersection2,ConnectedRegions2;
		HTuple Row1,Col1,Area4, Row2,Column1;
		gen_rectangle1(&ImageRect, 1, 1, m_imgHeight-1, m_imgWidth-1);
		//connection(m_DetectReg, &ConnectedRegions6);
		//select_shape_std(ConnectedRegions6, &SelectedRegions3, "max_area", 70);
		gen_contour_region_xld(m_DetectReg, &Contours, "border");
		get_contour_xld(Contours, &Row1, &Col1);
		gen_region_points(&Region, Row1, Col1);
		intersection(m_BottomSidesRgn, Region, &RegionIntersection2);
		connection(RegionIntersection2, &ConnectedRegions2);
		area_center(ConnectedRegions2, &Area4, &Row2, &Column1);
		Hobject RegionLines,Rectangle3,CharRect,ROI;
		HTuple Phi1;
		if (0 != ((Row2.Num())==2))
		{
			gen_region_line(&RegionLines, Row2.Select(0), Column1.Select(0), Row2.Select(1), Column1.Select(1));
			line_orientation(Row2.Select(0), Column1.Select(0), Row2.Select(1), Column1.Select(1), &Phi1);
			gen_rectangle2(&Rectangle3, m_CenterRow+100, m_CenterColumn, Phi1, m_imgHeight, RegHeight);
			intersection(Rectangle3, ImageRect, &CharRect);
			difference(m_DetectReg, CharRect, &ROI);
		}
		else
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}

		Hobject Rectangle1,CenterRect,ImageReduced1,ImageClosing,Regs,RegionErosion,WhiteTROI,ROIClosingMoved;
		HTuple Row11, Column11, Row22, Column22;
		gen_rectangle2(&Rectangle1, m_CenterRow, m_CenterColumn, 3.1416/2, 2000,RegWidth);
		intersection(Rectangle1, ImageRect, &CenterRect);
		smallest_rectangle1(ROI, &Row11, &Column11, &Row22, &Column22);
		m_MoveRow=Row11;
		m_MoveCol=Column11;
		reduce_domain(Image, ROI, &ImageReduced1);
		gray_closing_rect(ImageReduced1, &ImageClosing, 1, 15);
		threshold(ImageClosing, &Regs, ThreshValue, 255);
		erosion_rectangle1(Regs, &RegionErosion, ErosionSizeWidth, ErosionSizeHeight);
		//erosion_circle (RegionErosion, RegionErosion1, 2)
		difference(RegionErosion, CenterRect, &WhiteTROI);
		m_WhiteTROI=WhiteTROI;
		if(m_bDebugImg)
		{

			HTuple HomMat2DIdentity,HomMat2DScale;
			hom_mat2d_identity(&HomMat2DIdentity);
			hom_mat2d_scale(HomMat2DIdentity, 1, 1, 0, 0, &HomMat2DScale);
			Hobject ROIAffineTrans,ROIAffineTransMoved;
			affine_trans_region(ROI, &ROIAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(ROIAffineTrans, &ROIAffineTransMoved, 0, 0);

			Hobject TROIAffineTrans,TROIAffineTransMoved;
			affine_trans_region(WhiteTROI, &TROIAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(TROIAffineTrans, &TROIAffineTransMoved, 0, 0);
			concat_obj (ROIAffineTransMoved, TROIAffineTransMoved, &ROIClosingMoved);
		}
		m_vRgn[id] = ROIClosingMoved;

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
STDMETHODIMP CPccAlgo::MsiWhiteReinforcedRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("843$\
					 167;B$\
					 844;LP;50;400;1;205$\
					 845;LP;20;300;1;70$\
					 846;LP;1;250;1;165$\
					 724;LP;1;50;1;5$\
					 725;LP;1;50;1;5"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiBlackReinforcedRegionLocationAlgo(VARIANT* rgnId,VARIANT* pRegHeight,VARIANT* pRegWidth,VARIANT* pThreshValue,VARIANT* pErosionSizeWidth,VARIANT* pErosionSizeHeight)
{
	//得到检测区域
	int id = (int)rgnId->fltVal;
	int RegHeight=(int)pRegHeight->fltVal;
	int RegWidth=(int)pRegWidth->fltVal;
	int ThreshValue=(int)pThreshValue->fltVal;
	int ErosionSizeWidth=(int)pErosionSizeWidth->fltVal;
	int ErosionSizeHeight=(int)pErosionSizeHeight->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;

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
		HTuple Class;
		Hobject Image = m_crtImg;
		get_obj_class(m_BottomSidesRgn,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MsiWhiteReinforcedRegionLocationAlgo')前需先调用轮廓提取函数('MsiBottomSidesRegionLocationAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('MsiBottomSidesRegionLocationAlgo')before calling location function('MsiWhiteReinforcedRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		//
		Hobject ImageRect,RegionDilation1,ConnectedRegions6,SelectedRegions3,Contours,Region,RegionIntersection2,ConnectedRegions2;
		HTuple Row1,Col1,Area4, Row2,Column1;
		gen_rectangle1(&ImageRect, 1, 1, m_imgHeight-1, m_imgWidth-1);
		gen_contour_region_xld(m_DetectReg, &Contours, "border");
		get_contour_xld(Contours, &Row1, &Col1);
		gen_region_points(&Region, Row1, Col1);
		intersection(m_BottomSidesRgn, Region, &RegionIntersection2);
		connection(RegionIntersection2, &ConnectedRegions2);
		area_center(ConnectedRegions2, &Area4, &Row2, &Column1);
		Hobject RegionLines,Rectangle3,CharRect,ROI;
		HTuple Phi1;
		if (0 != ((Row2.Num())==2))
		{
			gen_region_line(&RegionLines, Row2.Select(0), Column1.Select(0), Row2.Select(1), Column1.Select(1));
			line_orientation(Row2.Select(0), Column1.Select(0), Row2.Select(1), Column1.Select(1), &Phi1);
			gen_rectangle2(&Rectangle3, m_CenterRow+100, m_CenterColumn, Phi1, m_imgHeight, RegHeight);
			intersection(Rectangle3, ImageRect, &CharRect);
			difference(m_DetectReg, CharRect, &ROI);
		}
		else
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}

		Hobject Rectangle1,CenterRect,ImageReduced1,ImageClosing,Regs,RegionErosion,BlackTROI;
		HTuple Row11, Column11, Row22, Column22;
		gen_rectangle2(&Rectangle1, m_CenterRow, m_CenterColumn, 3.1416/2, 2000,RegWidth);
		intersection(Rectangle1, ImageRect, &CenterRect);
		smallest_rectangle1(ROI, &Row11, &Column11, &Row22, &Column22);
		m_MoveRow=Row11;
		m_MoveCol=Column11;
		reduce_domain(Image, ROI, &ImageReduced1);
		gray_closing_rect(ImageReduced1, &ImageClosing, 1, 15);
		threshold(ImageClosing, &Regs, ThreshValue, 255);
		Hobject RegionDifference1,RegionClosing,ROIClosingMoved;
		difference(ROI, Regs, &RegionDifference1);
		closing_circle(RegionDifference1, &RegionClosing, 3.5);
		erosion_rectangle1(RegionClosing, &RegionErosion, ErosionSizeWidth, ErosionSizeHeight);
		difference(RegionErosion, CenterRect, &BlackTROI);

		m_BlackTROI=BlackTROI;
		if(m_bDebugImg)
		{

			HTuple HomMat2DIdentity,HomMat2DScale;
			hom_mat2d_identity(&HomMat2DIdentity);
			hom_mat2d_scale(HomMat2DIdentity, 1, 1, 0, 0, &HomMat2DScale);
			Hobject ROIAffineTrans,ROIAffineTransMoved;
			affine_trans_region(ROI, &ROIAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(ROIAffineTrans, &ROIAffineTransMoved, 0, 0);

			Hobject TROIAffineTrans,TROIAffineTransMoved;
			affine_trans_region(BlackTROI, &TROIAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(TROIAffineTrans, &TROIAffineTransMoved, 0, 0);
			concat_obj (ROIAffineTransMoved, TROIAffineTransMoved, &ROIClosingMoved);
		}
		m_vRgn[id] = ROIClosingMoved ;

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
STDMETHODIMP CPccAlgo::MsiBlackReinforcedRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("843$\
					 167;B$\
					 844;LP;50;400;1;205$\
					 845;LP;20;300;1;70$\
					 846;LP;1;250;1;165$\
					 724;LP;1;50;1;5$\
					 725;LP;1;50;1;15"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiWhiteReinforcedBlackDetectAlgo(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSeriousBlackPointDynThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	union1(rgn,&rgn);
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int MoveSize = (int)pMoveSize->fltVal;
	int SeriousBlackPointDynThresh = (int)pSeriousBlackPointDynThresh->fltVal;
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
		Hobject  RegionBlack, RegionUnion, ConnectedRegions,SelectedRegions;
		HTuple rgnArea,rgnRow,rgnCol,MoveSizeMax;
		area_center(rgn,&rgnArea,&rgnRow,&rgnCol);
		MoveSizeMax=rgnRow/2;
		if (0!=(MoveSize>=MoveSizeMax[0].I()))
		{
			MoveSize=MoveSizeMax[0].I();
		}
		reduce_domain(Image, rgn, &ImageReduced);
		Hobject RegionMoved,ImageReduced2,ImagePart1,ImagePart2;
		crop_domain(ImageReduced, &ImagePart1);
		move_region(rgn, &RegionMoved, -MoveSize, 0);
		reduce_domain(Image, RegionMoved, &ImageReduced2);
		crop_domain(ImageReduced2, &ImagePart2);
		Hobject RegionDynThresh,RegionMoved1,RegionIntersection,ROIMoved,RegionIntersection1,RegionErosion3,DefectRegs1;
		//HTuple Row11, Column11;
		dyn_threshold(ImagePart1, ImagePart2, &RegionDynThresh, SeriousBlackPointDynThresh, "dark");
		move_region(RegionDynThresh, &RegionMoved1, m_MoveRow, m_MoveCol);
		intersection(RegionMoved1, m_WhiteTROI, &RegionIntersection);
		move_region(rgn, &ROIMoved, MoveSize+18, 0);
		intersection(ROIMoved, RegionIntersection, &RegionIntersection1);
		erosion_circle(rgn, &RegionErosion3, 10);
		intersection(RegionErosion3, RegionIntersection1, &DefectRegs1);
		Hobject RegionMovedUp,ImageReduced3,ImagePart3,RegionDynThreshUp,RegionMoved2,RegionIntersection3;

		move_region(rgn, &RegionMovedUp, MoveSize, 0);
		reduce_domain(Image, RegionMovedUp, &ImageReduced3);
		crop_domain(ImageReduced3, &ImagePart3);
		dyn_threshold(ImagePart1, ImagePart3, &RegionDynThreshUp, SeriousBlackPointDynThresh, "dark");
		move_region(RegionDynThreshUp, &RegionMoved2, m_MoveRow, m_MoveCol);
		intersection(RegionMoved2, m_WhiteTROI, &RegionIntersection3);
		Hobject ROIMoved2,RegionIntersection4,RegionErosion4,DefectRegs2;
		move_region(rgn, &ROIMoved2, -(MoveSize+10), 0);
		intersection(ROIMoved2, RegionIntersection3, &RegionIntersection4);
		erosion_circle(rgn, &RegionErosion4, 10);
		intersection(RegionErosion4, RegionIntersection4, &DefectRegs2);
		Hobject DefectRegs3,DefectRegs4,DefectRegs0;
		union2(DefectRegs1, DefectRegs2, &DefectRegs3);
		intersection(DefectRegs3, m_WhiteTROI, &DefectRegs4);
		intersection(DefectRegs4, rgn, &DefectRegs0);
		Hobject ConnectedDefectRegs,SelectedDefectRegs;
		connection(DefectRegs0, &ConnectedDefectRegs);
		select_shape(ConnectedDefectRegs, &SelectedDefectRegs, "area", "and", SeriousBlackPointSize, 9999999);

		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;	
		union1(SelectedDefectRegs, &DefectRegs);
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
STDMETHODIMP CPccAlgo::MsiWhiteReinforcedBlackDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("847$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 848;LP;0;60;1;45$\
					 849;LP;1;250;1;22$\
					 416;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiBlackReinforcedBlackDetectAlgo(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSeriousBlackPointDynThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	union1(rgn,&rgn);
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int MoveSize = (int)pMoveSize->fltVal;
	int SeriousBlackPointDynThresh = (int)pSeriousBlackPointDynThresh->fltVal;
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
		Hobject  RegionBlack, RegionUnion, ConnectedRegions,SelectedRegions;
		HTuple rgnArea,rgnRow,rgnCol,MoveSizeMax;
		area_center(rgn,&rgnArea,&rgnRow,&rgnCol);
		MoveSizeMax=rgnRow/2;
		if (0!=(MoveSize>=MoveSizeMax[0].I()))
		{
			MoveSize=MoveSizeMax[0].I();
		}
		reduce_domain(Image, rgn, &ImageReduced);
		Hobject RegionMoved,ImageReduced2,ImagePart1,ImagePart2;
		crop_domain(ImageReduced, &ImagePart1);
		move_region(rgn, &RegionMoved, -MoveSize, 0);
		reduce_domain(Image, RegionMoved, &ImageReduced2);
		crop_domain(ImageReduced2, &ImagePart2);
		Hobject RegionDynThresh,RegionMoved1,RegionIntersection,ROIMoved,RegionIntersection1,RegionErosion3,DefectRegs1;
		//HTuple Row11, Column11;
		dyn_threshold(ImagePart1, ImagePart2, &RegionDynThresh, SeriousBlackPointDynThresh, "dark");
		move_region(RegionDynThresh, &RegionMoved1, m_MoveRow, m_MoveCol);
		intersection(RegionMoved1, m_BlackTROI, &RegionIntersection);		
		move_region(rgn, &ROIMoved, MoveSize, 0);
		intersection(ROIMoved, RegionIntersection, &DefectRegs1);
		Hobject RegionMoved11,RegionMoved2,ImageReduced3,ImagePart3,RegionDynThresh1,RegionIntersection2;
		move_region(rgn, &RegionMoved11, MoveSize, 0);
		reduce_domain(Image, RegionMoved11, &ImageReduced3);
		crop_domain(ImageReduced3, &ImagePart3);
		dyn_threshold(ImagePart1, ImagePart3, &RegionDynThresh1, SeriousBlackPointDynThresh, "dark");
		move_region(RegionDynThresh1, &RegionMoved2, m_MoveRow, m_MoveCol);
		intersection(RegionMoved2, m_BlackTROI, &RegionIntersection2);

		Hobject ROIMoved2,RegionIntersection4,RegionErosion4,DefectRegs2;
		move_region(rgn, &ROIMoved2, -MoveSize, 0);
		intersection(ROIMoved2, RegionIntersection2, &DefectRegs2);		
		Hobject DefectRegs3,DefectRegs4,DefectRegs0;
		union2(DefectRegs1, DefectRegs2, &DefectRegs3);
		union1(DefectRegs3,&DefectRegs3);
		intersection(DefectRegs3, rgn, &DefectRegs0);
		Hobject ConnectedDefectRegs,SelectedDefectRegs;
		connection(DefectRegs0, &ConnectedDefectRegs);
		select_shape(ConnectedDefectRegs, &SelectedDefectRegs, "area", "and", SeriousBlackPointSize, 9999999);

		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;	
		union1(SelectedDefectRegs, &DefectRegs);
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
STDMETHODIMP CPccAlgo::MsiBlackReinforcedBlackDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("850$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 848;LP;0;60;1;45$\
					 849;LP;1;250;1;22$\
					 416;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiStarvingDetectAlgo(VARIANT* rgnId, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	union1(rgn,&rgn);
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
		//
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		//
		Hobject rgnconnected;
		HTuple rgnArea,rgnRow,rgnCol;
		connection(rgn,&rgnconnected);
		area_center(rgnconnected,&rgnArea,&rgnRow,&rgnCol);

		HTuple AreaDiff,Length;

		tuple_length(rgnArea,&Length);

		if ( Length >= 2)
		{
			AreaDiff=abs(rgnArea[1].I()-rgnArea[0].I());
		}
		else
			AreaDiff = 0;
	
		if(AreaDiff <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			Hobject rgnDilation;
			retValue = AreaDiff[0].I();
			dilation_circle (rgn, &rgnDilation, 2.5);
			m_vErrorRgn[rId] = rgnDilation;
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
STDMETHODIMP CPccAlgo::MsiStarvingDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("800$\
					 397;R;LP;0;1000;1$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

/******************************************************************************************************
**函 数 名：MsiBorderExtraSilkDetectAlgo
**功    能：拉丝检测：对区域边缘的多余拉丝进行检测
**时    间：2019年6月12日
********************************************************************************************************/
STDMETHODIMP CPccAlgo::MsiBorderExtraSilkDetectAlgo(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pClosingSize,VARIANT* pAreaSize,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;

	int MoveSize      = (int)pMoveSize->fltVal;
	float ClosingSize = (float)pClosingSize->fltVal;
	int AreaSize = (int)pAreaSize->fltVal;

	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	union1(rgn,&rgn);
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
		// get the image
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		
		//move region and closoing
		Hobject rgnMoved,rgnDiff,rgnCls,rgnError,rgnDiffCon,rgnDetect;
		HTuple rgnArea,rgnRow,rgnCol;
		
		union1(rgn,&rgn);
		move_region(rgn,&rgnMoved,-MoveSize,0);

		Hobject rgnSearch,rgnSearchMoved;
		difference(rgn,rgnMoved,&rgnSearch);
		
		move_region(rgnSearch,&rgnSearchMoved,-MoveSize,0);

		difference(rgnSearchMoved,rgn,&rgnDiff);
		connection(rgnDiff,&rgnDiffCon);
		select_shape_std(rgnDiffCon,&rgnDetect,"max_area",70);

		closing_circle(rgnDetect,&rgnCls,ClosingSize);

		difference(rgnCls,rgnDiff,&rgnError);
		connection(rgnError,&rgnError);
		select_shape(rgnError,&rgnError,"area","and",AreaSize ,99999999);

		union1(rgnError,&rgnError);
	
		area_center(rgnError,&rgnArea,&rgnRow,&rgnCol);

		if(rgnArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			Hobject rgnDilation;
			retValue = rgnArea[0].I();
			dilation_circle (rgnError, &rgnDilation, 2.5);
			m_vErrorRgn[rId] = rgnDilation;

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
STDMETHODIMP CPccAlgo::MsiBorderExtraSilkDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6042$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 1280;LP;0;150;1;20$\
					 1093;FP;1;150;0.5;50$\
					 6043;LP;1;150;1;5");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPccAlgo::MsiBorderExtraSilkDetectAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("6044$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 1280;LP;0;150;1;10$\
					 6045;FP;1;250;0.5;190$\
					 6046;LP;1;250;1;120$\
					 6047;LP;1;250;1;30$\
					 6048;FP;1;250;0.5;20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/******************************************************************************************************
**函 数 名：MsiBorderExtraSilkDetectAlgo1
**功    能：凹凸检测：对区域边缘进行提取，检测边缘的凹凸
**时    间：2019年6月13日
********************************************************************************************************/
STDMETHODIMP CPccAlgo::MsiBorderExtraSilkDetectAlgo1(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pLowThred,VARIANT* pLengthThred,VARIANT* pClipNum,VARIANT* pSizeThred,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;

	int MoveSize      = (int)pMoveSize->fltVal;      // 10
	float LowThred    = (float)pLowThred->fltVal;    // 190
	int LengthThred   = (int)pLengthThred->fltVal;   // 120   
	int ClipNum       = (int)pClipNum->fltVal;     // 30    
	float SizeThred   = (float)(pSizeThred->fltVal/20.0); //10

	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	union1(rgn,&rgn);
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
		// get the image
		Hobject Image = m_crtImg;

		//move region 
		Hobject rgnMoved,imgDetect,imgUpperMoved;
		HTuple strClass;

		get_obj_class(m_ExpandedImage,&strClass);
		if ( strClass == "image")
		{
			imgDetect = m_ExpandedImage;
		}
		else
		{
			imgDetect = Image;
		}

		union1(rgn,&rgn);
		move_region(rgn,&rgnMoved,-MoveSize,0);
		reduce_domain(imgDetect, rgnMoved, &imgUpperMoved);

		Hobject xldBorder,xldBorderSel,Rectangle ;
		HTuple  dRU, dCU, dPU, dLen1U, dLen2U;
		threshold_sub_pix(imgUpperMoved, &xldBorder, LowThred);
		select_contours_xld(xldBorder, &xldBorderSel, "contour_length", LengthThred, 9999999, -0.5, 0.5);
		select_contours_xld(xldBorder, &xldBorderSel, "open", LengthThred, 9999999, -0.5, 0.5);
		smallest_rectangle2_xld(xldBorderSel, &dRU, &dCU, &dPU, &dLen1U, &dLen2U);

		HTuple tLength;
		tuple_length(dRU,&tLength);
		if ( tLength != 1)
		{
			retValue = -1;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
			return S_FALSE;
		}
		gen_rectangle2_contour_xld(&Rectangle, dRU, dCU, dPU, dLen1U, dLen2U);

		HTuple Cos,Sin;
		tuple_cos(dPU, &Cos);
		tuple_sin(dPU, &Sin);

		//LT   Left Top
		HTuple LT_X, LT_Y,dLT_R,dLT_C;
		LT_X = (dLen1U*Cos)+(dLen2U*Sin);
		LT_Y = (dLen1U*Sin)-(dLen2U*Cos);
		dLT_R = dRU+LT_Y;
		dLT_C = dCU-LT_X;

		//LB  Left Bottom
		HTuple LB_X, LB_Y,dLB_R,dLB_C;
		LB_X = (dLen1U*Cos)+(dLen2U*Sin);
		LB_Y = (dLen1U*Sin)+(dLen2U*Cos);
		dLB_R = dRU+LB_Y;
		dLB_C = dCU-LB_X;

		//RT Right Top
		HTuple RT_X, RT_Y,dRT_R,dRT_C;
		RT_X = (dLen1U*Cos)-(dLen2U*Sin);
		RT_Y = (dLen1U*Sin)+(dLen2U*Cos);
		dRT_R = dRU-RT_Y;
		dRT_C = dCU+RT_X;

		//RB Right Bottom
		HTuple RB_X, RB_Y,dRB_R,dRB_C;
		RB_X = (dLen1U*Cos)+(dLen2U*Sin);
		RB_Y = (dLen1U*Sin)-(dLen2U*Cos);
		dRB_R = dRU-RB_Y;
		dRB_C = dCU+RB_X;
	
		//record the selected coordinate of the upper line
		HTuple tupleXldSelULUC,tupleXldSelULUR,tupleXldSelURDC,tupleXldSelURDR;
		tupleXldSelULUC = HTuple();
		tupleXldSelULUR = HTuple();
		tupleXldSelURDC = HTuple();
		tupleXldSelURDR = HTuple();

		//the coordinate of clipped contour of a rectangle2
		HTuple xldCpLUR,xldCpLUC,xldCpRDR,xldCpRDC,xldCpLen1U,xldCplen2U,xldCpCenR,xldCpCenC;
		xldCpLUR = dLT_R-10;
		xldCpLUC = dLT_C;
		xldCpRDR = (xldCpLUR+(2*dLen2U))+20;
		xldCpRDC = dLT_C;

		xldCpLen1U = dLen1U/ClipNum;
		xldCplen2U = ((2*dLen2U)+20)/2.0;

		xldCpCenR = (dLT_R-10)+xldCplen2U;
		xldCpCenC = dLT_C+(xldCpLen1U*(dPU.Cos()));

		//step of center
		HTuple dStepCenR,dStepCenC;
		dStepCenR = (xldCpLen1U*2)*(dPU.Sin());
		dStepCenC = (xldCpLen1U*2)*(dPU.Cos());

		for (int s = 1;s <= ClipNum*2-1;s++)
		{
			Hobject Rectangle2;
			HTuple dULT_X,dULT_Y,dULT_R,dULT_C;
			gen_rectangle2(&Rectangle2, xldCpCenR-((dStepCenR*(s-1))/2.0), 
			xldCpCenC+((dStepCenC*(s-1))/2.0), dPU, xldCpLen1U, xldCplen2U);
			dULT_X = (xldCpLen1U*(dPU.Cos()))+(xldCplen2U*(dPU.Sin()));
			dULT_Y = (xldCpLen1U*(dPU.Sin()))-(xldCplen2U*(dPU.Cos()));
			dULT_R = (xldCpCenR-((dStepCenR*(s-1))/2.0))+dULT_Y;
			dULT_C = (xldCpCenC+((dStepCenC*(s-1))/2.0))-dULT_X;

			HTuple dURB_X,dURB_Y,dURB_R,dURB_C;
			dURB_X = (xldCpLen1U*(dPU.Cos()))+(xldCplen2U*(dPU.Sin()));
			dURB_Y = (xldCpLen1U*(dPU.Sin()))-(xldCplen2U*(dPU.Cos()));
			dURB_R = (xldCpCenR-((dStepCenR*(s-1))/2.0))-dURB_Y;
			dURB_C = (xldCpCenC+((dStepCenC*(s-1))/2.0))+dURB_X;

			Hobject xldClpCots,rgnRect3;
			HTuple dRow4, dCol4, dP1, dLen11, dLen21;
			clip_contours_xld(xldBorderSel, &xldClpCots, dULT_R, dULT_C, dURB_R, dURB_C);
			smallest_rectangle2_xld(xldClpCots, &dRow4, &dCol4, &dP1, &dLen11, &dLen21);
			HTuple iLength;
			tuple_length(dRow4,&iLength);
			if ( iLength <= 0)
			{
				retValue = -1;
			    Hobject newRgn; 
			    m_vErrorRgn[rId] = newRgn;
			    return S_FALSE;
			}
			gen_rectangle2_contour_xld(&rgnRect3, dRow4, dCol4, dP1, dLen11, dLen21);

			HTuple endValue = (dLen21.Num())-1;
			for (int i = 0 ; i <=endValue;i++ )
			{
				HTuple itemp = dLen21.Select(i);
				if (0 != (itemp > SizeThred))
				{
				    tupleXldSelULUC = tupleXldSelULUC.Concat(dULT_C);
				    tupleXldSelULUR = tupleXldSelULUR.Concat(dULT_R);
				    tupleXldSelURDC = tupleXldSelURDC.Concat(dURB_C);
				    tupleXldSelURDR = tupleXldSelURDR.Concat(dURB_R);
				}
			}

		}

		Hobject xldContourSel;
		gen_empty_obj(&xldContourSel);
 		if ( tupleXldSelULUC.Num()>0.5 )
	    {
				HTuple endValue1 = (tupleXldSelULUC.Num())-1;
				for (int num=0; num <= endValue1; num ++)
				{
					Hobject xldCClipU;
					clip_contours_xld(xldBorderSel, &xldCClipU, tupleXldSelULUR.Select(num),tupleXldSelULUC.Select(num), tupleXldSelURDR.Select(num), tupleXldSelURDC.Select(num));
					concat_obj(xldContourSel, xldCClipU, &xldContourSel);
				}
		 }

		Hobject rgnError;
		HTuple rgnArea,rgnRow,rgnCol;
		gen_region_contour_xld(xldContourSel, &rgnError, "filled");
        union1(rgnError, &rgnError);
		area_center(rgnError,&rgnArea,&rgnRow,&rgnCol);

		if(rgnArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			Hobject rgnDilation;
			retValue = rgnArea[0].I();
			dilation_circle (rgnError, &rgnDilation, 1.5);
			m_vErrorRgn[rId] = rgnDilation;

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





