#include "stdafx.h"
#include "resource.h"
#include "CrownCapAlgo_i.h"
#include "dllmain.h"
#include "cpp/HalconCpp.h"
#include <string>
using namespace std;
using namespace Halcon;
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

// DllUnregisterServer - 在系统注册表中移除项。
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

// DllInstall - 按用户和计算机在系统注册表中逐一添加/移除项。
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

#include "stdafx.h"
#include "CrownCapAlgo.h"
//#include <atlstr.h>
STDMETHODIMP CCrownCapAlgo::SetCurrentImage(LONG* pImg)
{
	Hobject* pObj = (Hobject*)pImg;
	m_crtImg = *pObj;
	m_bDebugImg = IsDebugImg(m_crtImg);
	return S_OK;
}
bool CCrownCapAlgo::IsDebugImg(Hobject& Image)
{
	if(Image.Id() == H_EMPTY_REGION)
	{
		return false;
	}
	double dRet = 0.0;
	get_grayval(Image, 0, 0, &dRet);
	if((int)dRet != (int)'d')
	{
		return false;
	}
	get_grayval(Image, 1, 0, &dRet);
	if((int)dRet != (int)'e')
	{
		return false;
	}
	get_grayval(Image, 2, 0, &dRet);
	if((int)dRet != (int)'b')
	{
		return false;
	}
	get_grayval(Image, 3, 0, &dRet);
	if((int)dRet != (int)'u')
	{
		return false;
	}
	get_grayval(Image, 4, 0, &dRet);
	if((int)dRet != (int)'g')
	{
		return false;
	}

	return true;
}

STDMETHODIMP CCrownCapAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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


STDMETHODIMP CCrownCapAlgo::CrownCapLocationAlgo(VARIANT* rgnId, VARIANT* pThredLow,  VARIANT* pThredHigh,  VARIANT* pRadiusExt, VARIANT* pRadius)
{
	int id = (int)rgnId->fltVal;
	int Thred_Low = (int)pThredLow->fltVal;
	int Thred_High = (int)pThredHigh->fltVal;
	long m_RadiusExt = (long)pRadiusExt->fltVal;
	long m_Radius = (long)pRadius->fltVal;
	///////////////////////////////////////////////////
	Hobject newRgn;
	m_vRgn[id] = newRgn;

	Hobject  Circle, ImageReduced, Region,Contours1,ContoursSplit,UnionContours,ObjectSelected,Circle_Center;
	Hobject  ConnectedRegions, RegionOpening, SelectedRegions1,SelectedXLD;
	Hobject  RegionUnion1, RegionFillUp;
	int   m_thred;
	Hlong Width, Height;
	HTuple Row,Column, Area,Number_XLD,Length1, Indices,Radius2, StartPhi2, EndPhi2, PointOrder2;
	try
	{
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		//
		Hobject Image=m_crtImg;
		/////////
		if (Thred_Low>Thred_High)
		{
			m_thred=Thred_Low;
			Thred_Low=Thred_High;
			Thred_High=m_thred;
		}
		get_image_size(Image, &Width, &Height);
		gen_circle(&Circle, Height/2.0, Width/2.0, m_RadiusExt);

		reduce_domain(Image, Circle, &ImageReduced);
		threshold(ImageReduced, &Region, Thred_Low,Thred_High);
		connection(Region, &ConnectedRegions);
		opening_circle(ConnectedRegions, &ConnectedRegions, 15);//可以排除内外环中间区域影响定位
		select_shape(ConnectedRegions, &SelectedRegions1, "area", "and", 15000, 999999);
		fill_up(SelectedRegions1, &RegionFillUp);
		opening_circle(RegionFillUp, &RegionOpening, 50.5);

		gen_contour_region_xld(RegionOpening, &Contours1, "border");

		segment_contours_xld(Contours1, &ContoursSplit, "lines_circles", 5, 4, 2);

		union_cocircular_contours_xld(ContoursSplit, &UnionContours, 0.5, 0.1, 0.2, 30, 10, 10, "true", 1);

		select_shape_xld(UnionContours, &SelectedXLD, "contlength", "and", 50, 99999);
		count_obj(SelectedXLD, &Number_XLD);
		if(Number_XLD<1)
		{
			m_vPos[0].m_x=0.0;
			m_vPos[0].m_y=0.0;
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		length_xld(SelectedXLD, &Length1);
		tuple_sort_index(Length1, &Indices);

		select_obj(SelectedXLD, &ObjectSelected, HTuple(Indices[Number_XLD-1])+1);
		fit_circle_contour_xld(ObjectSelected, "algebraic", -1, 0, 0, 3, 2, &Row, &Column, &Radius2, &StartPhi2, &EndPhi2, &PointOrder2);
		///////////////////////////
		if((Radius2[0].D()-m_Radius)>10 )
		{
			m_vPos[0].m_x=0.0;
			m_vPos[0].m_y=0.0;
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		else 
		{
			gen_circle(&Circle_Center,Row, Column,m_Radius);
			m_vPos[0].m_x=(float)Column[0].D();
			m_vPos[0].m_y=(float)Row[0].D();
			if(Circle_Center.Id() != H_EMPTY_REGION)
			{
				m_vRgn[id] = Circle_Center ;
			}
		}
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CrownCapLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("190$\
					 167;B$\
					 191;LP;0;255;1;50$\
					 192;LP;10;255;1;80$\
					 193;LP;10;180;1;170$\
					 194;LP;20;160;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CCrownCapAlgo::CrownCapLocationUseEdgeAlgo(VARIANT* rgnId, VARIANT* pThredLow,  VARIANT* pThredHigh,  VARIANT* pRadiusExt, VARIANT* pRadius)
{
	int id = (int)rgnId->fltVal;
	int Thred_Low = (int)pThredLow->fltVal;
	int Thred_High = (int)pThredHigh->fltVal;
	long m_RadiusExt = (long)pRadiusExt->fltVal;
	long m_Radius = (long)pRadius->fltVal;
	///////////////////////////////////////////////////
	Hobject newRgn;
	m_vRgn[id] = newRgn;

	Hobject  Circle,Circle2,Ring, ImageReduced,EdgeAmplitude,regionExtract,ImageReducedSoble,Lines,SelectedXLD,SelectedContours,Circle_Center;
	int   m_thred;
	Hlong Width, Height;
	HTuple Define_OK,Index2,Row_C,Column_C,Row,Column, Area,Length1 ,Radius, StartPhi, EndPhi, PointOrder,thred_low,thred_high,range;
	try
	{
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		//
		Hobject Image=m_crtImg;
		Define_OK=0;
		if (Thred_Low>Thred_High)
		{
			m_thred=Thred_Low;
			Thred_Low=Thred_High;
			Thred_High=m_thred;
		}
		get_image_size(Image, &Width, &Height);
		gen_circle(&Circle, Height/2.0, Width/2.0, m_RadiusExt);
		gen_circle(&Circle2, Height/2.0, Width/2.0, 65);
		difference(Circle,Circle2,&Ring);
		reduce_domain(Image, Ring, &ImageReduced);
		sobel_amp(ImageReduced, &EdgeAmplitude, "sum_abs", 5);
		//time//////////////////
		min_max_gray(Ring,EdgeAmplitude,40,&thred_low,&thred_high,&range);
		threshold(EdgeAmplitude,&regionExtract,thred_high,255);
		reduce_domain(EdgeAmplitude, regionExtract, &ImageReducedSoble);
		///////////////////
		//lines_gauss(EdgeAmplitude, &Lines, 1.5, Thred_Low, Thred_High, "light", "true", "bar-shaped", "true");
		lines_facet(ImageReducedSoble, &Lines, 5, Thred_Low, Thred_High, "light");
		select_shape_xld(Lines, &SelectedXLD, "circularity", "and", 0.45, 99999);
		select_contours_xld(SelectedXLD, &SelectedContours, "contour_length", 300, 20000, -0.5, 0.5);
		fit_circle_contour_xld(SelectedContours, "algebraic", -1, 0, 0, 3, 2, &Row, &Column, &Radius, &StartPhi, &EndPhi, &PointOrder);
		tuple_length(Radius, &Length1);
		for (Index2=0; Index2<=Length1-1; Index2+=1)
		{
			if (0 != (((HTuple(Radius[Index2])-103).Abs())<25))
			{
				Row_C=Row[Index2];
				Column_C=Column[Index2];
				Define_OK = 1;
			}
		}
		//*********************************************************************************
		if (0 != (Define_OK==1))
		{
			gen_circle(&Circle_Center,Row_C, Column_C,m_Radius);
			m_vPos[0].m_x=(float)Column_C[0].D();
			m_vPos[0].m_y=(float)Row_C[0].D();
			if(Circle_Center.Id() != H_EMPTY_REGION)
			{
				m_vRgn[id] = Circle_Center ;
			}
		}
		else
		{
			m_vPos[0].m_x=0.0;
			m_vPos[0].m_y=0.0;
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CrownCapLocationUseEdgeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("320$\
					 167;B$\
					 191;LP;0;255;1;3$\
					 192;LP;5;255;1;8$\
					 193;LP;10;180;1;170$\
					 194;LP;20;160;1;98");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
void CCrownCapAlgo::CenterDefined(Hobject Image, float pThredLow,  float pThredHigh, float pRadiusExt,float pRadius, HTuple& CenterXY)
{
	//
	int Thred_Low = (int)pThredLow;
	int Thred_High = (int)pThredHigh;
	long m_RadiusExt = (long)pRadiusExt;
	long m_Radius = (long)pRadius;
	///////////////////////////////////////////////////
	Hobject  Circle,Circle2,Ring, ImageReduced,EdgeAmplitude,regionExtract,ImageReducedSoble,Lines,SelectedXLD,SelectedContours,Circle_Center;
	int   m_thred;
	Hlong Width, Height;
	HTuple Define_OK,Index2,Row_C,Column_C,Row,Column, Area,Length1 ,Radius, StartPhi, EndPhi, PointOrder,thred_low,thred_high,range;
	try
	{
		//Hobject Image=Image;
		Define_OK=0;
		if (Thred_Low>Thred_High)
		{
			m_thred=Thred_Low;
			Thred_Low=Thred_High;
			Thred_High=m_thred;
		}
		get_image_size(Image, &Width, &Height);
		gen_circle(&Circle, Height/2.0, Width/2.0, m_RadiusExt);
		gen_circle(&Circle2, Height/2.0, Width/2.0, 65);
		difference(Circle,Circle2,&Ring);
		reduce_domain(Image, Ring, &ImageReduced);
		sobel_amp(ImageReduced, &EdgeAmplitude, "sum_abs", 5);
		//time//////////////////
		min_max_gray(Ring,EdgeAmplitude,40,&thred_low,&thred_high,&range);
		threshold(EdgeAmplitude,&regionExtract,thred_high,255);
		reduce_domain(EdgeAmplitude, regionExtract, &ImageReducedSoble);
		///////////////////
		//lines_gauss(EdgeAmplitude, &Lines, 1.5, Thred_Low, Thred_High, "light", "true", "bar-shaped", "true");
		lines_facet(ImageReducedSoble, &Lines, 5, Thred_Low, Thred_High, "light");
		select_shape_xld(Lines, &SelectedXLD, "circularity", "and", 0.45, 99999);
		select_contours_xld(SelectedXLD, &SelectedContours, "contour_length", 300, 20000, -0.5, 0.5);
		fit_circle_contour_xld(SelectedContours, "algebraic", -1, 0, 0, 3, 2, &Row, &Column, &Radius, &StartPhi, &EndPhi, &PointOrder);
		tuple_length(Radius, &Length1);
		for (Index2=0; Index2<=Length1-1; Index2+=1)
		{
			if (0 != (((HTuple(Radius[Index2])-103).Abs())<25))
			{
				Row_C=Row[Index2];
				Column_C=Column[Index2];
				Define_OK = 1;
			}
		}
		//*********************************************************************************
		if (0 != (Define_OK==1))
		{
			CenterXY[1]=Row_C[0];
			CenterXY[0]=Column_C[0];
		}
		else
		{
			CenterXY[1]=0.0;
			CenterXY[0]=0.0;
		}
	}
	catch (HException &except) 
	{
		CenterXY[1]=0.0;
		CenterXY[0]=0.0;
	}

}
STDMETHODIMP CCrownCapAlgo::CrownCapLocationCH2Algo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRadius,  VARIANT* pOffSetX,  VARIANT* pOffSetY)
{
	int id = (int)rgnId->fltVal;
	float m_greediness = (float)pGreediness->fltVal;
	int m_Radius = (int)pRadius->fltVal;
	int m_offSetX = (int)pOffSetX->fltVal;
	int m_offSetY = (int)pOffSetY->fltVal;
	///////////////////////////////////////////////////
	Hobject newRgn;
	m_vRgn[id] = newRgn;

	Hobject Rectangle2 , ImageReduced2,Circle_Center;
	HTuple Width, Height;
	HTuple Define_OK,Row_M,Column_M,Angle_M,Score_M,Define_Center_Row,Define_Center_Column,HomMat2D;
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
		Hobject Image=m_crtImg;
		Define_OK=0;
		/////////
		get_image_size(Image, &Width, &Height);

		gen_rectangle1(&Rectangle2, 40,40,Height-40,Width-40);
		reduce_domain(Image, Rectangle2, &ImageReduced2);
		find_shape_model(ImageReduced2,m_modelID , -0.49, 0.49, 0.5, 1, 0.5, "least_squares", 0,m_greediness , &Row_M, &Column_M, &Angle_M, &Score_M);

		if (0 != (Score_M>0.7))
		{
			vector_angle_to_rigid(HTuple(m_centerTuple[2]), HTuple(m_centerTuple[3]), 0, Row_M, Column_M, Angle_M, &HomMat2D);
			affine_trans_point_2d(HomMat2D,HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), &Define_Center_Row, &Define_Center_Column);
			//
			tuple_add(Define_Center_Row,m_offSetY, &Define_Center_Row);
			tuple_add(Define_Center_Column,m_offSetX, &Define_Center_Column);
			Define_OK = 1;
		}
		//*********************************************************************************
		if (0 != (Define_OK==1))
		{
			gen_circle(&Circle_Center,Define_Center_Row, Define_Center_Column,m_Radius);
			m_vPos[0].m_x=(float)Define_Center_Column[0].D();
			m_vPos[0].m_y=(float)Define_Center_Row[0].D();
			if(Circle_Center.Id() != H_EMPTY_REGION)
			{
				m_vRgn[id] = Circle_Center ;
			}
		}
		else
		{
			m_vPos[0].m_x=0.0;
			m_vPos[0].m_y=0.0;
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
STDMETHODIMP CCrownCapAlgo::CrownCapLocationCH2AlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("321$\
					 167;B$\
					 322;FP;0.2;1.0;0.05;0.8$\
					 194;LP;5;240;1;190$\
					 323;LP;-20;20;1;0$\
					 324;LP;-20;20;1;0");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CrownCapCircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWith, VARIANT* pChannel)
{
	int id = (int)rgnId->fltVal;
	int m_Radius = (int)pRadius->fltVal;
	int m_RoiWith = (int)pRoiWith->fltVal;
	int m_RadiusExt=m_Radius+m_RoiWith;
	int m_productID=(int)pChannel->fltVal;
	float m_center_x=0.0;
	float m_center_y=0.0;
	if(m_productID==1)
	{
		m_center_x=m_vPos[0].m_x;
		m_center_y=m_vPos[0].m_y;
	}
	else if(m_productID==2)
	{
		m_center_x=m_vPicPos[0].m_x;
		m_center_y=m_vPicPos[0].m_y;
	}
	///////////////////////////////////////////////////
	Hobject newRgn;
	m_vRgn[id] = newRgn;

	if((m_center_x<2.0)||(m_center_y<2.0))
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
		/////////
		if (m_RoiWith>0)
		{
			gen_circle(&Circle2_EXT, m_center_y, m_center_x, m_RadiusExt);
			gen_circle(&Circle1_Inner, m_center_y, m_center_x, m_Radius);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		}
		else if (m_RoiWith==0)
		{
			gen_circle(&Circle2_EXT, m_center_y, m_center_x, m_RadiusExt+1);
			gen_circle(&Circle1_Inner, m_center_y, m_center_x, m_Radius);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		}
		else
		{
			if(m_RadiusExt>0)
			{
				gen_circle(&Circle2_EXT, m_center_y, m_center_x, m_RadiusExt);
				gen_circle(&Circle1_Inner, m_center_y, m_center_x, m_Radius);
				difference(Circle1_Inner,Circle2_EXT,&RegionDetect);
			}
			else
			{
				gen_circle(&RegionDetect, m_center_y, m_center_x, m_Radius);
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
STDMETHODIMP CCrownCapAlgo::CrownCapCircleRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 194;LP;10;220;1;114$\
					 308;LP;-160;160;1;18$\
					 252;LP;1;20;1;1");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CircleDetectAlgo(VARIANT* rgnID,VARIANT* pEnhanceSize, VARIANT* pEnhanceRatio, VARIANT* pBlackThred, VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	int rId = (int)rgnID->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	int m_enhanceSize = (int)pEnhanceSize->fltVal;
	float m_enhanceRatio=(float)pEnhanceRatio->fltVal;
	int m_blackThred =(int)pBlackThred->fltVal;
	int m_minArea = (int)pMinArea->fltVal;

	Hobject Image = m_crtImg;
	Hobject  RegionDifference_ext,RegionUnion;
	Hobject  ImageReduced_ext;
	Hobject  ImageEmphasize_ext;
	Hobject  Region_ext,  ConnectedRegions,RegionUnion_detect,RegionErosion_c,RegionDifference_c;
	Hobject  SelectedRegions;
	HTuple   Mean_ext, Deviation_ext;
	HTuple   Number, Row_Test, Column_Test,Length;
	HTuple  Mean, Deviation,  Mean_2, Deviation_2;

	Number[0]=0;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		RegionDifference_ext=rgn;
		reduce_domain(Image, RegionDifference_ext, &ImageReduced_ext);
		//*环检测**************
		emphasize(ImageReduced_ext, &ImageEmphasize_ext,m_enhanceSize,m_enhanceSize ,m_enhanceRatio );
		intensity(RegionDifference_ext, Image, &Mean_ext, &Deviation_ext);
		threshold(ImageEmphasize_ext, &Region_ext, 0,m_blackThred );//Mean_ext-30//黑点检测
		connection(Region_ext, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and",m_minArea, 99999);	
		union1(SelectedRegions,&RegionUnion_detect);
		area_center(RegionUnion_detect, &Number, &Row_Test, &Column_Test);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(m_blackThred));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple Sum,Log,Prod,AbsoluteHisto, RelativeHisto, Length;
			gray_histo (rgn, ImageEmphasize_ext, &AbsoluteHisto, &RelativeHisto);   
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
		//count_obj(SelectedRegions, &Number);
		if(Number<=0)//SelectedRegions.Id() == H_EMPTY_REGION
		{
			retValue = 0;
		}
		else
		{
			retValue = Number[0].I();
		}
		m_vErrorRgn[rId] = SelectedRegions;
		retValue.Detach(pDetectNumber);
		//临时检测
		if(rId==2)
		{
			erosion_circle(RegionDifference_ext, &RegionErosion_c, 16);
			difference(RegionDifference_ext, RegionErosion_c, &RegionDifference_c);
			intensity(RegionErosion_c,Image, &Mean, &Deviation);
			intensity(RegionDifference_c,Image, &Mean_2, &Deviation_2);
			double m_test1=Mean[0].D();
			double m_test2=Mean_2[0].D();
			if(m_test2-m_test1>-8)
			{
				retValue =-1;
				retValue.Detach(pDetectNumber);
				return S_FALSE;
			}
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CircleDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("195$\
					 200;R;LP;1;300;1$\
					 167;B$\
					 196;LP;3;21;1;5$\
					 197;FP;0.5;11.5;0.1;1.5$\
					 198;LP;2;255;1;84$\
					 199;LP;1;400;1;6");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CircleOtherDetectAlgo(VARIANT* rgnID,VARIANT* pRateAngle, VARIANT* pBlackDist,VARIANT* pErosionSize,  VARIANT* pMinArea, VARIANT* pChannel,VARIANT* pErrorGrayDist, VARIANT* pDetectNumber)
{
	int rId = (int)rgnID->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	float m_rateAngle = (float)pRateAngle->fltVal;
	m_rateAngle=(float)((m_rateAngle/180.0)*3.14);
	int m_blackDist =(int)pBlackDist->fltVal;
	int m_grayDist =(int)pErrorGrayDist->fltVal;
	float m_erosionSize=(float)pErosionSize->fltVal;
	int m_minArea = (int)pMinArea->fltVal;
	int m_detectNumber = (int)pDetectNumber->fltVal;
	int m_productID=(int)pChannel->fltVal;
	float m_center_x;
	float m_center_y;
	if(m_productID==1)
	{
		m_center_x=m_vPos[0].m_x;
		m_center_y=m_vPos[0].m_y;
	}
	else if(m_productID==2)
	{
		m_center_x=m_vPicPos[0].m_x;
		m_center_y=m_vPicPos[0].m_y;
	}

	Hobject Image = m_crtImg;
	Hobject  RegionDifference_ext;
	Hobject  ImageReduced_ext,ImageReduced_Rate;
	HTuple   HomMat2D;
	Hobject  Region_Black,RegionErosion_Black,  ConnectedRegions,RegionDilation_Black,RegionUnion_detect;
	Hobject  SelectedRegions,RegionUnionEnd;
	HTuple   Number, Row_Test, Column_Test;
	HTuple  Mean, Deviation;
	Number[0]=0;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		RegionDifference_ext=rgn;
		reduce_domain(Image, RegionDifference_ext, &ImageReduced_ext);

		//*环检测**************
		vector_angle_to_rigid(m_center_y, m_center_x, 0,m_center_y, m_center_x, m_rateAngle, &HomMat2D);

		affine_trans_image(ImageReduced_ext, &ImageReduced_Rate, HomMat2D, "bilinear", "false");

		dyn_threshold(ImageReduced_ext, ImageReduced_Rate, &Region_Black, m_blackDist, "dark");

		erosion_circle(Region_Black, &RegionErosion_Black,m_erosionSize);

		dilation_circle(RegionErosion_Black, &RegionDilation_Black,m_erosionSize+0.5);

		connection(RegionDilation_Black, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and",m_minArea, 99999);	
		union1(SelectedRegions,&RegionUnion_detect);
		area_center(RegionUnion_detect, &Number, &Row_Test, &Column_Test);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(m_blackDist));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			sub_image (ImageReduced_Rate,ImageReduced_ext,  &ImageSub, 1, 0);
			gray_histo (rgn, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			int histoLength = Length[0].I();
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
		//count_obj(SelectedRegions, &Number);
		if(Number<=0)//SelectedRegions.Id() == H_EMPTY_REGION
		{
			retValue = 0;
		}
		else
		{
			retValue = Number[0].I();
		}
		m_vErrorRgn[rId] = SelectedRegions;
		retValue.Detach(pDetectNumber);
		//临时检测
		if(m_productID==1)
		{
			intensity(RegionDifference_ext,Image, &Mean, &Deviation);
			double m_test=Mean[0].D();
			double m_mean_gray=0.0;
			if(Number<=0)//SelectedRegions.Id() == H_EMPTY_REGION
			{
				if(rId==4)
				{
					m_meanGray_m=m_meanGray_m+m_test;
					m_addNumber_m=m_addNumber_m+1.0;
				}
				if(rId==2)
				{
					m_meanGray_c=m_meanGray_c+m_test;
					m_addNumber_c=m_addNumber_c+1.0;
				}				
			}
			else
			{
				if(Number[0].I()<120)
				{	
					if(rId==4)
					{
						m_meanGray_m=m_meanGray_m+m_test;
						m_addNumber_m=m_addNumber_m+1.0;
					}
					if(rId==2)
					{
						m_meanGray_c=m_meanGray_c+m_test;
						m_addNumber_c=m_addNumber_c+1.0;
					}
				}
			}

			if(rId==4)
			{
				if(m_addNumber_m<1.0)
				{
					m_mean_gray=m_test;
				}
				else
					m_mean_gray=m_meanGray_m/m_addNumber_m;

				if(fabs(m_test-m_mean_gray)>m_grayDist)
				{
					retValue =-1;
					retValue.Detach(pDetectNumber);
					return S_FALSE;
				}
			}
			if(rId==2)
			{
				if(m_addNumber_c<1.0)
				{
					m_mean_gray=m_test;
				}
				else
					m_mean_gray=m_meanGray_c/m_addNumber_c;
				if(fabs(m_test-m_mean_gray)>m_grayDist)
				{
					retValue =-1;
					retValue.Detach(pDetectNumber);
					return S_FALSE;
				}
			}
		}
		//
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CircleOtherDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("309$\
					 200;R;LP;1;300;1$\
					 167;B$\
					 310;FP;0.5;20;0.1;5.5$\
					 311;LP;1;255;1;15$\
					 312;FP;0.5;15;0.5;2.5$\
					 199;LP;1;400;1;15$\
					 252;LP;1;20;1;1$\
					 293;LP;2;250;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CircleRateDetectAlgo(VARIANT* rgnID, VARIANT* pBlackThred,VARIANT* pSmoothSize,  VARIANT* pCircleRate)
{
	int rId = (int)rgnID->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pCircleRate);
		return S_FALSE;
	}
	int BlackThred =(int)pBlackThred->fltVal;
	int m_SmoothSize=(int)pSmoothSize->fltVal;
	///////////////////////////////////////////////////////

	Hobject Image = m_crtImg;
	Hobject  RegionDifference_ext,Region2,RegionFillUp_All,RegionFillUp1,Region_Out,RegionDif;
	Hobject  ImageReduced,Circle_u,RegionUnion2,ConnectedRegions1,SelectedRegions,RegionUnion1,RegionClosing,RegionOpening1;
	Hobject   Contours;
	HTuple   Radius_inner , Radius_ext,Rate,Result1,Row3, Column3,Row4, Column4,RowD, ColumnD, Radius_D;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		RegionDifference_ext=rgn;
		reduce_domain(Image, RegionDifference_ext, &ImageReduced);
		threshold(ImageReduced, &Region2, BlackThred, 255);
		fill_up(RegionDifference_ext, &RegionFillUp_All);
		difference(RegionFillUp_All, RegionDifference_ext, &Circle_u);
		erosion_circle(Circle_u, &Circle_u,5);
		dilation_circle(Circle_u, &Circle_u,5+3.5);
		union2(Region2, Circle_u, &RegionUnion2);
		connection(RegionUnion2, &ConnectedRegions1);
		select_shape(ConnectedRegions1, &SelectedRegions, "area", "and", 135, 999999);
		union1(SelectedRegions, &RegionUnion1);

		fill_up(RegionUnion1, &RegionFillUp1);
		closing_circle(RegionFillUp1, &RegionClosing, 20.5);
		opening_circle(RegionClosing, &RegionOpening1, m_SmoothSize);//95
		gen_contour_region_xld(RegionOpening1, &Contours, "border");
		HTuple m_detect_num;
		count_obj(Contours,&m_detect_num);
		if (m_detect_num>=1)
		{
			smallest_circle(RegionOpening1, &Row4, &Column4, &Radius_ext);
			gen_circle(&Region_Out,Row4, Column4, Radius_ext);
			difference(Region_Out,RegionOpening1,&RegionDif);
			inner_circle(RegionDif, &RowD, &ColumnD, &Radius_D);
			Result1=Radius_D*2.0;
			//
			Rate = (1.0-Result1/Radius_ext)*100.0;
			retValue=Rate[0].D();
		}
		else
		{
			retValue=0.0;
		}
		m_vErrorRgn[rId] = RegionOpening1;
		retValue.Detach(pCircleRate);

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pCircleRate);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CircleRateDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("313$\
					 315;R;FP;0.0;100.0;0.5$\
					 167;B$\
					 191;LP;1;255;1;62$\
					 314;LP;5;200;1;110");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CrownCapDetectAlgo(VARIANT* rgnID, VARIANT* pBlackThred,VARIANT* pARMin,VARIANT* pARMax,  VARIANT* pCrownAnglDiff)
{
	int rId = (int)rgnID->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pCrownAnglDiff);
		return S_FALSE;
	}
	int BlackThred =(int)pBlackThred->fltVal;
	int m_ARMin=(int)pARMin->fltVal;
	int m_ARMax=(int)pARMax->fltVal;
	HTuple    Row_center=m_vPos[0].m_y;
	HTuple Column_center=m_vPos[0].m_x;
	///////////////////////////////////////////////////////

	Hobject Image = m_crtImg;
	Hobject  RegionDifference_ext;
	//
	Hobject  ImageReduced, Region2, ConnectedRegions2, RegionOpening2;
	Hobject  SelectedRegions_Chi_end, Circle4,Circle1,RegionDilation,RegionUnion1,ConnectedRegions;
	// Local control variables 
	HTuple  Number, Row_for_chi;
	HTuple  Column_for_chi, Radius_for_chi, ange_test, Prod;
	HTuple  row_distance, column_distance, Index1, Angle_ascend;
	HTuple  Angle_Result, Index2, Row2, Column2, distance_Sqrt;
	HTuple  distance_ascend;
	HTuple  Angle_Diff,Angle_Diff_ascend;
	HTuple  Row_for_chi_m, Column_for_chi_m, Radius_for_chi_m;
	HTuple  Radius_Sorted, Radius_Inverted, Radius_scale;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		RegionDifference_ext=rgn;
		reduce_domain(Image, RegionDifference_ext, &ImageReduced);
		threshold(ImageReduced, &Region2, BlackThred, 255);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(999999.0f);
			m_dictSpecialData[rId].push_back(-float(BlackThred));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,Sum,Log,Prod;
			get_image_size (Image,& Width, &Height);
			gray_histo (rgn, ImageReduced, &AbsoluteHisto, &RelativeHisto);
			tuple_add (AbsoluteHisto,1, &Sum);
			tuple_log10 (Sum, &Log);   
			tuple_mult (Log, 25, &Prod);
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
		connection(Region2, &ConnectedRegions2);
		closing_circle(ConnectedRegions2, &RegionOpening2, 3.5);
		//opening_circle(ConnectedRegions2, &RegionOpening2, 1.5);
		//  *添加选择，排除分割的特殊小区域*****************
		select_shape(RegionOpening2, &SelectedRegions_Chi_end, "area", "and", m_ARMin, m_ARMax);
		count_obj(SelectedRegions_Chi_end, &Number);
		//二次处理，避免小的白斑点影响。
		//*****************

		if ((0 != (Number<=21))&&(0 != (Number>=1)))//极端情况，没有找到一个齿顶。
		{
			smallest_circle(SelectedRegions_Chi_end, &Row_for_chi, &Column_for_chi, &Radius_for_chi);
			gen_circle(&Circle4, Row_for_chi, Column_for_chi, Radius_for_chi);
		}
		else if (0 != (Number>21))
		{
			smallest_circle(SelectedRegions_Chi_end, &Row_for_chi_m, &Column_for_chi_m, &Radius_for_chi_m);
			gen_circle(&Circle1, Row_for_chi_m, Column_for_chi_m, Radius_for_chi_m);
			tuple_sort(Radius_for_chi_m, &Radius_Sorted);
			tuple_inverse(Radius_Sorted, &Radius_Inverted);
			Radius_scale = HTuple(Radius_Inverted[20])/HTuple(Radius_Inverted[21]);
			if (0 != (Radius_scale<=1.5))//比值较小时，不去掉该点。
			{
				smallest_circle(SelectedRegions_Chi_end, &Row_for_chi, &Column_for_chi, &Radius_for_chi);
				gen_circle(&Circle4, Row_for_chi, Column_for_chi, Radius_for_chi);
			}
			else if (0 != (Radius_scale>1.5))
			{
				union1(Circle1, &RegionUnion1);
				dilation_circle(RegionUnion1, &RegionDilation, 3.5);//去掉半径偏小，距离与齿顶接近的点
				connection(RegionDilation, &ConnectedRegions);
				inner_circle(ConnectedRegions, &Row_for_chi, &Column_for_chi, &Radius_for_chi);
				gen_circle(&Circle4, Row_for_chi, Column_for_chi, Radius_for_chi);
				erosion_circle(Circle4, &Circle4, 3.5);
				count_obj(Circle4, &Number);
			}
		}
		else
		{
			retValue =-1;
			retValue.Detach(pCrownAnglDiff);
			return S_FALSE;
		}

		//*** 角度分布 换中心点以后可能对角度分布有些影响。特别是偏心较大的时候
		tuple_sub(Row_for_chi, Row_center, &row_distance);
		tuple_sub(Column_for_chi, Column_center, &column_distance);

		//角度判断用的时间较长
		for (Index1=0; Index1<=Number-1; Index1+=1)
		{
			if (0 != (HTuple(row_distance[Index1])<0))
			{
				if (0 != (HTuple(column_distance[Index1])>0))
				{	  
					tuple_atan(row_distance[Index1]/column_distance[Index1],&ange_test);
					tuple_mult(ange_test, 360/(2*3.1415926), &ange_test);
					//*tuple_mult (Prod[Index1], -1, Prod_test)
					Prod[Index1] = HTuple(ange_test)*-1;
				}
				else if (0 != (HTuple(column_distance[Index1])<0))
				{
					tuple_atan(row_distance[Index1]/column_distance[Index1],&ange_test);
					tuple_mult(ange_test, 360/(2*3.1415926), &ange_test);
					Prod[Index1] = 180-HTuple(ange_test);
				}
				else
				{
					Prod[Index1] = 90;
				}
			}
			else
			{
				if (0 != (HTuple(column_distance[Index1])>0))
				{
					tuple_atan(row_distance[Index1]/column_distance[Index1],&ange_test);
					tuple_mult(ange_test, 360/(2*3.1415926), &ange_test);				  
					Prod[Index1] = 360-HTuple(ange_test);
				}
				else if (0 != (HTuple(column_distance[Index1])<0))
				{
					tuple_atan(row_distance[Index1]/column_distance[Index1],&ange_test);
					tuple_mult(ange_test, 360/(2*3.1415926), &ange_test);
					Prod[Index1] = (HTuple(ange_test)*-1)+180;
				}
				else
				{
					Prod[Index1] = 270;
				}
			}

		}
		tuple_sort(Prod, &Angle_ascend);

		Angle_Result = 360.0/(21+0.000001);//m_Parameters.Crown_Number
		for (Index2=1; Index2<=Number-1; Index2+=1)
		{
			Angle_Diff[Index2]=((HTuple(Angle_ascend[Index2])-HTuple(Angle_ascend[Index2-1]))-Angle_Result).Abs();
		}
		//tuple_add(m_ng_ok, 1, &m_ng_ok);
		Angle_Diff[0]=(((360-HTuple(Angle_ascend[Number-1]))+HTuple(Angle_ascend[0]))-Angle_Result).Abs();
		tuple_sort(Angle_Diff, &Angle_Diff_ascend);
		//默认瓶盖为21个齿。m_Parameters.Crown_Number
		if(Number<21)
		{
			retValue = -1;
		}
		else
		{
			retValue=Angle_Diff_ascend[Number-1].D();
		}
		m_vErrorRgn[rId] =SelectedRegions_Chi_end;
		retValue.Detach(pCrownAnglDiff);

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pCrownAnglDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CrownCapDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("316$\
					 319;R;FP;0.0;60.0;0.25$\
					 167;B$\
					 191;LP;1;255;1;156$\
					 317;LP;1;1500;1;25$\
					 318;LP;10;3000;1;1600");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CrownCapMatchingParaAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pChannel, VARIANT* pMatchingResult)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];

	float m_greediness = (float)pGreediness->fltVal;
	int m_productID = (int)pChannel->fltVal;

	Hobject ImageReduced,Circle;
	HTuple ProductModels,Define_OK,Row_M,Column_M,Angle_M,Score_M,Model_M,HomMat2D,MatchingParaCH2,m_ModelCenterX,m_ModelCenterY;
	HTuple Row_Center,Column_Center;
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pMatchingResult);
		MatchingParaCH2[0]=1;
		MatchingParaCH2[1]=1;
		MatchingParaCH2[2]=0.0;
		MatchingParaCH2[3]=0.01;
		MatchingParaCH2[4]=1;
		if(m_productID==2)
		{
			m_matchingPara_CH2=MatchingParaCH2;
		}
		else if(m_productID==1)
		{
			m_matchingPara_CH1=MatchingParaCH2;
		}
		m_vPicPos[0].m_x=0.0;
		m_vPicPos[0].m_y=0.0;
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
		Hobject Image=m_crtImg;
		Define_OK=0;
		/////////
		Circle=rgn;
		reduce_domain(Image, Circle, &ImageReduced);
		if(m_productID==2)
		{
			ProductModels=m_models_CH2;
		}
		else if(m_productID==1)
		{
			ProductModels=m_models_CH1;
		}
		find_shape_models(ImageReduced, ProductModels, 0, HTuple(360).Rad(), 0.6, 1, 0.5, "least_squares", 0, m_greediness, &Row_M, &Column_M, &Angle_M, &Score_M, &Model_M);
		if (0 != (Score_M>0.6))
		{
			MatchingParaCH2[0]=Row_M;
			MatchingParaCH2[1]=Column_M;
			MatchingParaCH2[2]=Angle_M;
			MatchingParaCH2[3]=Score_M;
			MatchingParaCH2[4]=Model_M+1;//编号相差1.
			if(m_productID==2)
			{
				m_matchingPara_CH2=MatchingParaCH2;
				//
				get_value_matrix(m_matrixID_CH2,Model_M,0,&m_ModelCenterY);
				get_value_matrix(m_matrixID_CH2,Model_M,1,&m_ModelCenterX);	
				//添加判断，满足条件时，再检测//图案中心点不一定是瓶盖中心点
				vector_angle_to_rigid(m_ModelCenterY,m_ModelCenterX, 0,Row_M, Column_M,Angle_M,  &m_matchingHomMat2D_CH2);
				//中心点应该为模板图片图案中心点儿
				affine_trans_point_2d(m_matchingHomMat2D_CH2, m_ModelCenterY,m_ModelCenterX,&Row_Center,&Column_Center);
				//
			}
			else if(m_productID==1)
			{
				m_matchingPara_CH1=MatchingParaCH2;
				//
				get_value_matrix(m_matrixID_CH1,Model_M,0,&m_ModelCenterY);
				get_value_matrix(m_matrixID_CH1,Model_M,1,&m_ModelCenterX);
				//添加判断，满足条件时，再检测//图案中心点不一定是瓶盖中心点
				vector_angle_to_rigid(m_ModelCenterY,m_ModelCenterX, 0,Row_M, Column_M,Angle_M,  &m_matchingHomMat2D_CH1);
				//中心点应该为模板图片图案中心点儿
				affine_trans_point_2d(m_matchingHomMat2D_CH1, m_ModelCenterY,m_ModelCenterX,&Row_Center,&Column_Center);
				//
			}
			m_vPicPos[0].m_x=(float)Column_Center[0].D();
			m_vPicPos[0].m_y=(float)Row_Center[0].D();
			retValue =Score_M[0].D();
			retValue.Detach(pMatchingResult);
		}
		else
		{
			MatchingParaCH2[0]=1;
			MatchingParaCH2[1]=1;
			MatchingParaCH2[2]=0.0;
			MatchingParaCH2[3]=0.01;
			MatchingParaCH2[4]=1;
			if(m_productID==2)
			{
				m_matchingPara_CH2=MatchingParaCH2;
			}
			else if(m_productID==1)
			{
				m_matchingPara_CH1=MatchingParaCH2;
			}
			m_vPicPos[0].m_x=0.0;
			m_vPicPos[0].m_y=0.0;
			retValue =0.0;
			retValue.Detach(pMatchingResult);
			m_vErrorRgn[rId] = Circle;
			return S_FALSE;
		}
		////////////////////////////////////////////	

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		MatchingParaCH2[0]=1;
		MatchingParaCH2[1]=1;
		MatchingParaCH2[2]=0.0;
		MatchingParaCH2[3]=0.01;
		MatchingParaCH2[4]=1;
		if(m_productID==2)
		{
			m_matchingPara_CH2=MatchingParaCH2;
		}
		else if(m_productID==1)
		{
			m_matchingPara_CH1=MatchingParaCH2;
		}
		m_vPicPos[0].m_x=0.0;
		m_vPicPos[0].m_y=0.0;

		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pMatchingResult);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CrownCapMatchingParaAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("325$\
					 440;R;FP;0.0;1.0;0.05$\
					 167;B$\
					 322;FP;0.2;1.0;0.05;0.85$\
					 252;LP;1;20;1;1");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CrownCapPositionDistAlgo(VARIANT* rgnId, VARIANT* pShowRadius, VARIANT* pPositionDist)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pPositionDist);
		return S_FALSE;
	}
	if((m_vPicPos[0].m_x<5)||(m_vPicPos[0].m_y<5))
	{
		retValue=-1;
		retValue.Detach(pPositionDist);
		return S_FALSE;
	}
	int m_showRadius = (int)pShowRadius->fltVal;
	float m_positionDist=0.0;
	///////////////////////////////////////////////////

	Hobject Circle;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		//Hobject Image=m_crtImg;
		/////////
		gen_circle(&Circle,m_vPicPos[0].m_y,m_vPicPos[0].m_x,m_showRadius);
		////////////////////////////////////////////	
		m_positionDist=sqrt((m_vPos[0].m_y-m_vPicPos[0].m_y)*(m_vPos[0].m_y-m_vPicPos[0].m_y)+(m_vPos[0].m_x -m_vPicPos[0].m_x)*(m_vPos[0].m_x-m_vPicPos[0].m_x));
		m_vErrorRgn[rId] = Circle;
		retValue=m_positionDist;
		retValue.Detach(pPositionDist);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pPositionDist);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CrownCapPositionDistAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("245$\
					 247;R;FP;0.0;100.0;0.5$\
					 167;B$\
					 246;LP;100;240;1;159");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CrownCapCenterImageDetectAlgo(VARIANT* rgnId,VARIANT* pSmoothSize,VARIANT* pDiffBlack,VARIANT* pDiffWhite, VARIANT* pErosionSize, VARIANT* pMinArea, VARIANT* pChannel, VARIANT* pDetectNumber )
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	if((m_vPicPos[0].m_x<5)||(m_vPicPos[0].m_y<5))
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	float m_smoothSize = (float)pSmoothSize->fltVal;
	int m_diffBlack=(int)pDiffBlack->fltVal;
	int m_diffWhite=(int)pDiffWhite->fltVal;
	float m_erosionSize =(float)pErosionSize->fltVal;
	int m_minArea = (int)pMinArea->fltVal;
	int m_productID = (int)pChannel->fltVal;

	//int m_detectNumber = (int)pDetectNumber->fltVal;
	///////////////////////////////////////////////////

	Hobject Image_Model,ImageIconTrans,ImageReduced,ImageReduced_ocr,ImageMedian,Circle,Circle2,RegionAffineTrans;
	Hobject Region_ocr_detect_black,Region_ocr_detect_white,Region_white_open,Region_black_open,RegionUnion_ocr,
		RegionDilation_detect,RegionErosion_detect,ConnectedRegions_detect,SelectedRegions_detect,RegionUnion_detect,RegionUnionEnd;
	HTuple Number,Row_Test, Column_Test;
	Number[0]=0;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image=m_crtImg;
		/////////
		RegionAffineTrans=rgn;
		reduce_domain(Image, Circle, &ImageReduced);
		if(m_productID==2)
		{
			access_channel(m_multiChannelModelImage_CH2, &Image_Model,(Hlong)m_matchingPara_CH2[4]);
			affine_trans_image(Image_Model, &ImageIconTrans, m_matchingHomMat2D_CH2, "bilinear",  "false");
		}
		else if(m_productID==1)
		{
			access_channel(m_multiChannelModelImage_CH1, &Image_Model,(Hlong)m_matchingPara_CH1[4]);
			affine_trans_image(Image_Model, &ImageIconTrans, m_matchingHomMat2D_CH1, "bilinear",  "false");
		}

		reduce_domain(Image, RegionAffineTrans, &ImageReduced_ocr);//检测图像图标区域	
		if(m_smoothSize>=1.5)
		{
			median_image(ImageReduced_ocr, &ImageMedian, "circle", m_smoothSize, "mirrored");
			median_image(ImageIconTrans, &ImageIconTrans, "circle", m_smoothSize, "mirrored");

		}
		else
		{
			copy_image(ImageReduced_ocr, &ImageMedian);
			//dyn_threshold(ImageReduced_ocr,ImageIconTrans, &Region_ocr_detect_white,m_diffWhite,"light");
			//dyn_threshold(ImageReduced_ocr,ImageIconTrans, &Region_ocr_detect_black,m_diffBlack,"dark");
		}
		dyn_threshold(ImageMedian,ImageIconTrans, &Region_ocr_detect_white,m_diffWhite,"light");
		dyn_threshold(ImageMedian,ImageIconTrans, &Region_ocr_detect_black,m_diffBlack,"dark");
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(m_diffBlack));
			m_dictSpecialData[rId].push_back(float(m_diffWhite));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageOneChannel,ImageOneChannelModel,ImageSub;
			get_image_size (Image,& Width, &Height);
			HTuple Channels;
			count_channels (ImageMedian, &Channels);
			if(Channels>1)
				access_channel (ImageMedian, &ImageOneChannel, Channels);
			else
				ImageOneChannel=ImageMedian;
			//
			count_channels (ImageIconTrans, &Channels);
			if(Channels>1)
				access_channel (ImageIconTrans, &ImageOneChannelModel, Channels);
			else
				ImageOneChannelModel=ImageIconTrans;
			abs_diff_image (ImageOneChannel, ImageOneChannelModel, &ImageSub, 1);
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
		////去除特细小的区域，减少套印错误 误报//////////////////////
		//opening_circle(Region_ocr_detect_white,&Region_white_open,m_erosionSize); 
		//opening_circle(Region_ocr_detect_black,&Region_black_open,m_erosionSize); 
		/////////////////////////
		//union2(Region_white_open, Region_black_open, &RegionUnion_ocr);
		union2(Region_ocr_detect_white, Region_ocr_detect_black, &RegionUnion_ocr);
		if(m_erosionSize>=1.0)
		{
			erosion_circle(RegionUnion_ocr, &RegionErosion_detect, m_erosionSize);
			dilation_circle(RegionErosion_detect,&RegionDilation_detect, m_erosionSize);

		}
		else
		{
			RegionDilation_detect=RegionUnion_ocr;
		}
		connection(RegionDilation_detect, &ConnectedRegions_detect);
		select_shape(ConnectedRegions_detect, &SelectedRegions_detect, "area", "and",m_minArea, 999999);
		union1(SelectedRegions_detect,&RegionUnion_detect);
		area_center(RegionUnion_detect, &Number, &Row_Test, &Column_Test);
		//count_obj(SelectedRegions_detect, &Number);
		if(Number<=0)//if(SelectedRegions_detect.Id() == H_EMPTY_REGION)
		{
			retValue = 0;
		}
		else
		{
			retValue = Number[0].I();
		}
		m_vErrorRgn[rId] = SelectedRegions_detect;
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CrownCapCenterImageDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("248$\
					 200;R;LP;1;200;1$\
					 167;B$\
					 314;FP;0.5;25;0.5;1.5$\
					 249;LP;1;255;1;30$\
					 250;LP;1;255;1;35$\
					 251;FP;0.5;25;0.5;1.5$\
					 199;LP;1;400;1;6$\
					 252;LP;1;20;1;1");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{
	if(productID==1)
	{
		Hobject* pObj = (Hobject*)pCrtImg;
		Hobject Image= *pObj;
	
		m_nImgIndex++;
		if((m_nSampleSize-m_nImgIndex<=IMAGE_TRAIN_NUM-1)&&(m_nSampleSize-m_nImgIndex>=0))//199
		{
			HTuple ImageClass;
			get_obj_class(MultiChannelImageTest, &ImageClass);
			if(ImageClass!=HTuple("image"))
			{
				channels_to_image(Image, &MultiChannelImageTest);
			}
			else
			{
				append_channel(MultiChannelImageTest, Image, &MultiChannelImageTest);
			}
		}
#ifdef NDEBUG
		try
		{
#endif
			if(m_channel=="CH2")
			{
				Hobject Detect_Region,ImageReduced_Detect,ModelRegion,ModelImage,DupImage,Rectangle,RegionDifference;
				HTuple Row2,Column2, Angle2,Score, Score1,HomMat2D,Qy,Qx,Model_Radius,Detect_Radius,ModelID,Row3, Column3, Angle3, Score3;
				HTuple Row, Column, Angle, Model,ModelID_Add;
				Model_Radius = 120;
				Detect_Radius = 160;
				gen_rectangle1(&Rectangle, 0, 0, 494, 659);
				HTuple Class;
				get_obj_class(m_multiChannelModelImage, &Class);
				if(Class!=HTuple("image"))
				{
					find_shape_model(Image, m_modelID, -0.39, 0.78, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row2, &Column2, &Angle2, &Score1);
					//匹配不成功以及匹配度过低的图片，不作为模板图片
					if(Score1>0.5)		
					{
						vector_angle_to_rigid(HTuple(m_centerTuple[2]), HTuple(m_centerTuple[3]), 0, Row2, Column2, Angle2,&HomMat2D);
						affine_trans_point_2d(HomMat2D, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), &Qy, &Qx);
						gen_circle(&Detect_Region, Qy, Qx,Detect_Radius);
						reduce_domain(Image, Detect_Region, &ImageReduced_Detect);
						/////////////////////////////////////////////////////////////////
						gen_circle(&ModelRegion, Qy, Qx, Model_Radius);
						reduce_domain(Image, ModelRegion, &ModelImage);
						create_shape_model(ModelImage,  "auto", 0, HTuple(360).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID);
						find_shape_model(Image, ModelID, 0, 0.5, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row3, &Column3, &Angle3, &Score3);
						create_matrix(200, 4, 0, &MatrixID);
						if(Score3>0.5)
						{
							Model_Num = 1;
							Models[Model_Num-1] = ModelID;
							copy_image(Image, &DupImage);
							difference(Rectangle, Detect_Region, &RegionDifference);
							overpaint_region(DupImage, RegionDifference, 0, "fill");
							channels_to_image(DupImage, &m_multiChannelModelImage);
							set_value_matrix(MatrixID, Model_Num-1, 0, Row3);
							set_value_matrix(MatrixID, Model_Num-1, 1, Column3);
							set_value_matrix(MatrixID, Model_Num-1, 2, Qy);
							set_value_matrix(MatrixID, Model_Num-1, 3, Qx);
							return S_OK;
						}
					}
				}
				else
				{
					find_shape_model(Image, m_modelID, -0.39, 0.78, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row2, &Column2, &Angle2, &Score1);
					//匹配不成功以及匹配度过低的图片，不作为模板图片
					if(Score1>0.5)
					{
						vector_angle_to_rigid(HTuple(m_centerTuple[2]), HTuple(m_centerTuple[3]), 0, Row2, Column2, Angle2, &HomMat2D);
						affine_trans_point_2d(HomMat2D, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), &Qy, &Qx);
						gen_circle(&Detect_Region, Qy, Qx, Detect_Radius);
						reduce_domain(Image, Detect_Region, &ImageReduced_Detect);
						find_shape_models(ImageReduced_Detect, Models, 0, HTuple(360).Rad(), 0.6, 1, 0, "least_squares", 0, 1, &Row, &Column, &Angle, &Score, &Model);
						if (0 != (Score<0.95))
						{
							gen_circle(&ModelRegion, Qy, Qx, Model_Radius);
							reduce_domain(Image, ModelRegion, &ModelImage);
							create_shape_model(ModelImage,  "auto", 0, HTuple(360).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID_Add);
							find_shape_model(ImageReduced_Detect, ModelID_Add, 0, 0.5, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row3, &Column3, &Angle3, &Score3);
							if(Score3>0.5)
							{
								copy_image(Image, &DupImage);
								difference(Rectangle, Detect_Region, &RegionDifference);
								overpaint_region(DupImage, RegionDifference, 0, "fill");
								if (Model_Num<IMAGE_MODEL_NUM)//60
								{
									append_channel(m_multiChannelModelImage, DupImage, &m_multiChannelModelImage);
									Models[Model_Num] = ModelID_Add;
									Model_Num += 1;	
									set_value_matrix(MatrixID, Model_Num-1, 0, Row3);
									set_value_matrix(MatrixID, Model_Num-1, 1, Column3);
									set_value_matrix(MatrixID, Model_Num-1, 2, Qy);
							        set_value_matrix(MatrixID, Model_Num-1, 3, Qx);
									return S_OK;
								}
							}
						}
					}
				}
			}
			else if(m_channel=="CH1")
			{
				Hobject Detect_Region,ImageReduced_Detect,ModelRegion,ModelImage,DupImage,Rectangle,RegionDifference;
				HTuple Score,Qy,Qx,Model_Radius,Detect_Radius,Row3, Column3, Angle3, Score3,ModelID;
				HTuple Row, Column, Angle, Model,ModelID_Add;
				Model_Radius = 80;
				Detect_Radius = 150;
				gen_rectangle1(&Rectangle, 0, 0, 494, 659);
				HTuple Class;
				get_obj_class(m_multiChannelModelImage, &Class);
				////计算中心点
				//CComVariant var1(0.0f),var2(3.0f),var3(8.0f),var4(140.0f),var5(102.0f);
				//CrownCapLocationUseEdgeAlgo(&var1,&var2,&var3,&var4,&var5);
				//float m_center_x=0.0;
				//float m_center_y=0.0;
				//m_center_x=m_vPos[0].m_x;
				//m_center_y=m_vPos[0].m_y;
				////
				//计算中心点
				HTuple CenterXY;
				CenterDefined(Image,(float)3.0,(float)8.0,(float)140.0,(float)102.0,CenterXY);
				float m_center_x=0.0;
				float m_center_y=0.0;
				m_center_x=(float)CenterXY[0].D();
				m_center_y=(float)CenterXY[1].D();
				if(Class!=HTuple("image"))
				{
					//中心定位失败的图片，不作为模板图片
					if((m_center_x >2.0)&(m_center_y>2.0))		
					{
						Qy=m_center_y;
						Qx=m_center_x;
						gen_circle(&Detect_Region, Qy, Qx,Detect_Radius);
						reduce_domain(Image, Detect_Region, &ImageReduced_Detect);
						/////////////////////////////////////////////////////////////////
						gen_circle(&ModelRegion, Qy, Qx, Model_Radius);
						reduce_domain(Image, ModelRegion, &ModelImage);
						create_shape_model(ModelImage,  "auto", 0, HTuple(360).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID);
						find_shape_model(Image, ModelID, 0, 0.5, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row3, &Column3, &Angle3, &Score3);
						create_matrix(200, 4, 0, &MatrixID);
						if(Score3>0.5)
						{
							Model_Num = 1;
							Models[Model_Num-1] = ModelID;
							copy_image(Image, &DupImage);
							difference(Rectangle, Detect_Region, &RegionDifference);
							overpaint_region(DupImage, RegionDifference, 0, "fill");
							channels_to_image(DupImage, &m_multiChannelModelImage);
							set_value_matrix(MatrixID, Model_Num-1, 0, Row3);
							set_value_matrix(MatrixID, Model_Num-1, 1, Column3);
							set_value_matrix(MatrixID, Model_Num-1, 2, Qy);
							set_value_matrix(MatrixID, Model_Num-1, 3, Qx);
							return S_OK;
						}
					}
				}
				else
				{
					if((m_center_x >2.0)&(m_center_y>2.0))
					{
						Qy=m_center_y;
						Qx=m_center_x;
						gen_circle(&Detect_Region, Qy, Qx, Detect_Radius);
						reduce_domain(Image, Detect_Region, &ImageReduced_Detect);
						find_shape_models(ImageReduced_Detect, Models, 0, HTuple(360).Rad(), 0.6, 1, 0, "least_squares", 0, 1, &Row, &Column, &Angle, &Score, &Model);
						if (0 != (Score<0.95))
						{
							gen_circle(&ModelRegion, Qy, Qx, Model_Radius);
							reduce_domain(Image, ModelRegion, &ModelImage);
							create_shape_model(ModelImage,  "auto", 0, HTuple(360).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID_Add);
							find_shape_model(ImageReduced_Detect, ModelID_Add, 0, 0.5, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row3, &Column3, &Angle3, &Score3);
							if(Score3>0.5)
							{
								copy_image(Image, &DupImage);
								difference(Rectangle, Detect_Region, &RegionDifference);
								overpaint_region(DupImage, RegionDifference, 0, "fill");
								if (Model_Num<IMAGE_MODEL_NUM)//60
								{
									append_channel(m_multiChannelModelImage, DupImage, &m_multiChannelModelImage);
									Models[Model_Num] = ModelID_Add;
									Model_Num += 1;	
									set_value_matrix(MatrixID, Model_Num-1, 0, Row3);
									set_value_matrix(MatrixID, Model_Num-1, 1, Column3);
									set_value_matrix(MatrixID, Model_Num-1, 2, Qy);
							        set_value_matrix(MatrixID, Model_Num-1, 3, Qx);
									return S_OK;
								}
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

STDMETHODIMP CCrownCapAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	// 在检测状态，对于经常误检的图片，添加模板！与训练过程中的自动生成模板和删除模板，有些区别。变量不一致！
	if(productID==1)
	{
#ifdef NDEBUG
		try
		{
#endif
			CString	m_channelAdd=_T("");
			CString m_modelParaPath,m_modelParaPath1,m_strFileName,m_paraPath;
			string m_path_str;
			m_strFileName=*strFolder;
			int m_place=m_strFileName.Find('\\');
			//m_newClassName=m_strFileName.Left(m_place);
			m_channelAdd= m_strFileName.Right(3);
			//CString m_midStr=m_strFileName.Mid(0, m_strFileName.GetLength()-4 ); 
			//m_modelParaPath1.Format(_T("%s%s%s"),_T("C:\\CrownCapData\\"),m_midStr,_T("\\"));
			m_paraPath.Format(_T("%s%s"),m_strFileName,_T("\\"));
			//////////////////////////////////////////////////
			if(m_channelAdd=="CH2")
			{
				Hobject* pObj = (Hobject*)pImg;
				Hobject Image= *pObj;
				Hobject Detect_Region,ImageReduced_Detect,ModelRegion,ModelImage,DupImage,Rectangle,RegionDifference;
				HTuple Row2,Column2, Angle2, Score1,HomMat2D,Qy,Qx,Model_Radius,Detect_Radius,ModelID,Row3, Column3, Angle3, Score3;
				Model_Radius = 120;
				Detect_Radius = 160;
				find_shape_model(Image, m_modelID, -0.39, 0.78, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row2, &Column2, &Angle2, &Score1);
				if(Score1>0.5)
				{
					vector_angle_to_rigid(HTuple(m_centerTuple[2]), HTuple(m_centerTuple[3]), 0, Row2, Column2, Angle2, &HomMat2D);
					affine_trans_point_2d(HomMat2D, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), &Qy, &Qx);
					gen_circle(&Detect_Region, Qy, Qx, Detect_Radius);
					reduce_domain(Image, Detect_Region, &ImageReduced_Detect);
					/////////////////////////////////////////////////////////////////
					gen_circle(&ModelRegion, Qy, Qx, Model_Radius);
					reduce_domain(Image, ModelRegion, &ModelImage);
					create_shape_model(ModelImage,  "auto", 0, HTuple(360).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID);
					find_shape_model(Image, ModelID, 0, 0.5, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row3, &Column3, &Angle3, &Score3);	
					if(Score3>0.5)
					{
						gen_rectangle1(&Rectangle, 0, 0, 494, 659);
						copy_image(Image, &DupImage);
						difference(Rectangle, Detect_Region, &RegionDifference);
						overpaint_region(DupImage, RegionDifference, 0, "fill");
						HTuple Class;
						get_obj_class(m_multiChannelModelImage_CH2, &Class);
						if(Class!=HTuple("image"))
						{
							create_matrix(200, 2, 0, &m_matrixID_CH2);
							m_modelNum_CH2=0;
							channels_to_image(DupImage, &m_multiChannelModelImage_CH2);
						}
						else
						{
							append_channel(m_multiChannelModelImage_CH2, DupImage, &m_multiChannelModelImage_CH2);
						}
						m_modelNum_CH2 =m_modelNum_CH2+1;
						m_models_CH2[m_modelNum_CH2-1] = ModelID;
						//Models[m_modelNum_CH2-1] = ModelID;
						set_value_matrix(m_matrixID_CH2, m_modelNum_CH2-1, 0, Row3);
						set_value_matrix(m_matrixID_CH2, m_modelNum_CH2-1, 1, Column3);
						set_value_matrix(m_matrixID_CH2, m_modelNum_CH2-1, 2, Qy);
						set_value_matrix(m_matrixID_CH2, m_modelNum_CH2-1, 3, Qx);
						//重新保存模板相关数据
						//CString m_channer_name;
						//m_channer_name.Format(_T("%s"),_T("SelfLearning_CH2\\"));
						//m_paraPath.Format(_T("%s%s"),m_modelParaPath1,m_channer_name);
						m_modelParaPath.Format(_T("%s%s"),m_paraPath,_T("Model\\"));

						if( CreateDirectory(m_modelParaPath1, NULL) )
						{
							CreateDirectory(m_modelParaPath1, NULL);
						}
						if( CreateDirectory(m_paraPath, NULL) )
						{
							CreateDirectory(m_paraPath, NULL);
						}
						if( CreateDirectory(m_modelParaPath, NULL) )
						{
							CreateDirectory(m_modelParaPath, NULL);
						}
						//
						int m_modelNum=m_modelNum_CH2[0].I();
						CString	m_modelPath;m_modelPath.Format(_T("%s%s%03i%s"),m_modelParaPath,_T("model_"),m_modelNum,_T(".shm"));
						string	m_path_str= CT2A(m_modelPath.GetString());
						write_shape_model(ModelID,m_path_str.c_str());
						//write_shape_model(HTuple(Models[m_modelNum_CH2-1]),m_path_str.c_str());//用Models时可以保存，但是用m_models_CH2就报错，奇怪！
						//
						m_modelParaPath.Format(_T("%s%s"),m_paraPath,_T("ModelCenter.mtx"));
						m_path_str= CT2A(m_modelParaPath.GetString());
						write_matrix(m_matrixID_CH2, "binary", m_path_str.c_str());
						//
						m_modelParaPath.Format(_T("%s%s"),m_paraPath,_T("ModelImages.tiff"));
						m_path_str= CT2A(m_modelParaPath.GetString());
						write_image(m_multiChannelModelImage_CH2, "tiff", 0, m_path_str.c_str());
						///////////////////////////////////////////////////////////
						return S_OK;
					}
				}
			}
			else if(m_channelAdd=="CH1")
			{
				Hobject* pObj = (Hobject*)pImg;
				Hobject Image=*pObj;
				Hobject Detect_Region,ImageReduced_Detect,ModelRegion,ModelImage,DupImage,Rectangle,RegionDifference;
				HTuple Row2,Column2, Angle2, Score1,HomMat2D,Qy,Qx,Model_Radius,Detect_Radius,ModelID,Row3, Column3, Angle3, Score3;
				Model_Radius = 90;
				Detect_Radius = 150;
				//计算中心点
				HTuple CenterXY;
				CenterDefined(Image,(float)3.0,(float)8.0,(float)140.0,(float)102.0,CenterXY);
				float m_center_x=0.0;
				float m_center_y=0.0;
				m_center_x=(float)CenterXY[0].D();
				m_center_y=(float)CenterXY[1].D();
				if((m_center_x >2.0)&(m_center_y>2.0))
				{
					Qy=m_center_y;
					Qx=m_center_x;
					gen_circle(&Detect_Region, Qy, Qx, Detect_Radius);
					reduce_domain(Image, Detect_Region, &ImageReduced_Detect);
					/////////////////////////////////////////////////////////////////
					gen_circle(&ModelRegion, Qy, Qx, Model_Radius);
					reduce_domain(Image, ModelRegion, &ModelImage);
					create_shape_model(ModelImage,  "auto", 0, HTuple(360).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID);
					find_shape_model(Image, ModelID, 0, 0.5, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row3, &Column3, &Angle3, &Score3);	
					if(Score3>0.5)
					{
						gen_rectangle1(&Rectangle, 0, 0, 494, 659);
						copy_image(Image, &DupImage);
						difference(Rectangle, Detect_Region, &RegionDifference);
						overpaint_region(DupImage, RegionDifference, 0, "fill");
						HTuple Class;
						get_obj_class(m_multiChannelModelImage_CH1, &Class);
						if(Class!=HTuple("image"))
						{
							create_matrix(200, 2, 0, &m_matrixID_CH1);
							m_modelNum_CH1=0;
							channels_to_image(DupImage, &m_multiChannelModelImage_CH1);
						}
						else
						{
							append_channel(m_multiChannelModelImage_CH1, DupImage, &m_multiChannelModelImage_CH1);
						}
						m_modelNum_CH1 =m_modelNum_CH1+1;
						m_models_CH1[m_modelNum_CH1-1] = ModelID;
						set_value_matrix(m_matrixID_CH1, m_modelNum_CH1-1, 0, Row3);
						set_value_matrix(m_matrixID_CH1, m_modelNum_CH1-1, 1, Column3);
						set_value_matrix(m_matrixID_CH1, m_modelNum_CH1-1, 2, Qy);
						set_value_matrix(m_matrixID_CH1, m_modelNum_CH1-1, 3, Qx);
						//重新保存模板相关数据
						//CString m_channer_name;
						//m_channer_name.Format(_T("%s"),_T("SelfLearning_CH1\\"));
						//m_paraPath.Format(_T("%s%s"),m_modelParaPath1,m_channer_name);
						m_modelParaPath.Format(_T("%s%s"),m_paraPath,_T("Model\\"));

						if( CreateDirectory(m_modelParaPath1, NULL) )
						{
							CreateDirectory(m_modelParaPath1, NULL);
						}
						if( CreateDirectory(m_paraPath, NULL) )
						{
							CreateDirectory(m_paraPath, NULL);
						}
						if( CreateDirectory(m_modelParaPath, NULL) )
						{
							CreateDirectory(m_modelParaPath, NULL);
						}
						//
						int m_modelNum=m_modelNum_CH1[0].I();
						CString	m_modelPath;m_modelPath.Format(_T("%s%s%03i%s"),m_modelParaPath,_T("model_"),m_modelNum,_T(".shm"));
						string	m_path_str= CT2A(m_modelPath.GetString());
						write_shape_model(ModelID,m_path_str.c_str());
						//
						m_modelParaPath.Format(_T("%s%s"),m_paraPath,_T("ModelCenter.mtx"));
						m_path_str= CT2A(m_modelParaPath.GetString());
						write_matrix(m_matrixID_CH1, "binary", m_path_str.c_str());
						//
						m_modelParaPath.Format(_T("%s%s"),m_paraPath,_T("ModelImages.tiff"));
						m_path_str= CT2A(m_modelParaPath.GetString());
						write_image(m_multiChannelModelImage_CH1, "tiff", 0, m_path_str.c_str());
						///////////////////////////////////////////////////////////
						return S_OK;
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

STDMETHODIMP CCrownCapAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	if(productID==1)
	{
#ifdef NDEBUG
		try
		{
#endif
			Hobject  MultiChannelImage_New,Image_e;
			HTuple   i, Sub_ModelIndex, Value_R,Value_C;
			Sub_ModelIndex = nIndex;
			gen_empty_obj(&MultiChannelImage_New);
			for (i=1; i<=Model_Num-1; i+=1)
			{
				if (0 != (Sub_ModelIndex<=Model_Num))
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

						Models[i-1] = HTuple(Models[i]);
						get_value_matrix(MatrixID, i, 0, &Value_R);
						get_value_matrix(MatrixID, i, 1, &Value_C);
						set_value_matrix(MatrixID, i-1, 0, Value_R);
						set_value_matrix(MatrixID, i-1, 1, Value_C);
					}
				} 
			}

			if((Sub_ModelIndex<=Model_Num)&&(Sub_ModelIndex>0)&&(Model_Num>1))
			{
				copy_image(MultiChannelImage_New,&m_multiChannelModelImage);
				Model_Num=Model_Num-1;
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

STDMETHODIMP CCrownCapAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{
	if(productID==1)
	{
#ifdef NDEBUG
		try
		{
#endif
			CString m_modelParaPath,m_strModelName,m_modelParaPath1,m_modelParaPath2,saveClassPath,modelSetPath;
			wstring m_path;
			string m_path_str;
			m_strModelName=*strModelName;
			CString m_newModel=m_newClassName;//m_strModelName.Left(m_strModelName.GetLength()-4);
			//
			std::wstring wstrPath = CT2W(m_strModelName);
			std::string strPath = CW2A(wstrPath.c_str());
			size_t pathLength=strPath.length();
			size_t index = strPath.find_last_of("\\");
			string strSaveName;
			bool strEnd=false;
			if(index+1>=pathLength)
			{
				strEnd=true;
				string strT1 = strPath.substr(0,index);
				size_t index = strT1.find_last_of("\\");
				strSaveName = strT1.substr(0,index);
			}
			else
			{
				strSaveName = strPath.substr(0,index);
			}

			//
			saveClassPath=strSaveName.c_str();
			modelSetPath.Format(_T("%s%s%s"),saveClassPath,_T("\\"),m_newModel);
			//判断默认设置文件夹是否存在
			int retval; //从“PathFileExists”返回值。
			retval = PathFileExists(modelSetPath);
			//
			HTuple  Channels, i, Result_Path;
			CString m_name;
			count_channels(m_multiChannelModelImage, &Channels);
			Model_Num = Channels;
			int m_list=0;
			if(strEnd)
			{
				if(!retval)
				{
					m_modelParaPath1.Format(_T("%s%s%s"),m_strModelName,m_newModel,_T("\\"));
				}
				else
				{
					m_modelParaPath1.Format(_T("%s"),m_strModelName);
				}
			}
			else
			{
				if(!retval)
				{
					m_modelParaPath1.Format(_T("%s%s%s%s"),m_strModelName,_T("\\"),m_newModel,_T("\\"));
				}
				else
				{
					m_modelParaPath1.Format(_T("%s%s"),m_strModelName,_T("\\"));
				}
			}

			CString m_channer_name,m_channerPath;
			if(m_channel=="CH1")
			{
				m_channer_name.Format(_T("%s"),_T("SelfLearning_CH1/"));
				m_channerPath.Format(_T("%s%s"),m_modelParaPath1,m_channer_name);
			}
			else if (m_channel=="CH2")
			{
				m_channer_name.Format(_T("%s"),_T("SelfLearning_CH2/"));
				m_channerPath.Format(_T("%s%s"),m_modelParaPath1,m_channer_name);
			}
			m_modelParaPath2.Format(_T("%s%s%s"),m_modelParaPath1,m_channer_name,_T("Model/"));

			if( CreateDirectory(m_modelParaPath1, NULL) )
			{
				CreateDirectory(m_modelParaPath1, NULL);
			}
			if( CreateDirectory(m_channerPath, NULL) )
			{
				CreateDirectory(m_channerPath, NULL);
			}
			if( CreateDirectory(m_modelParaPath2, NULL) )
			{
				CreateDirectory(m_modelParaPath2, NULL);
			}

			for (i=1; i<=Model_Num; i+=1)
			{
				m_list=i[0].I();

				Result_Path = ("model_"+(i.ToString(".3")))+".shm";		
				m_name=Result_Path[0].S();
				m_modelParaPath.Format(_T("%s%s"),m_modelParaPath2,m_name);
				//m_path =m_modelParaPath.GetString();
				m_path_str= CT2A(m_modelParaPath.GetString());
				write_shape_model(HTuple(Models[i-1]),m_path_str.c_str());
			}
			if(Model_Num>=1)// 保存模板时，添加模板数量判断。
			{
				m_modelParaPath.Format(_T("%s%s"),m_channerPath,_T("ModelImages.tiff"));
				m_path_str= CT2A(m_modelParaPath.GetString());
				write_image(m_multiChannelModelImage, "tiff", 0, m_path_str.c_str());
				m_modelParaPath.Format(_T("%s%s"),m_channerPath,_T("ModelCenter.mtx"));
				m_path_str= CT2A(m_modelParaPath.GetString());
				write_matrix(MatrixID, "binary", m_path_str.c_str());
				//添加初始化模块。
				gen_empty_obj(&m_multiChannelModelImage);
				Models = HTuple();
				MatrixID = HTuple();
			}
			else
				return S_FALSE;
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

STDMETHODIMP CCrownCapAlgo::ResetModels(BYTE productID)
{
	if(productID==1)
	{
#ifdef NDEBUG
		try
		{
#endif
			if(m_channel=="CH2")
			{
				// Local iconic variables 
				Hobject  Image,ImageReducedDefine,ImageReduced3;
				Hobject  ModelRegion, RegionD, DupImage, Image1,Rectangle_Contour;
				Hobject  CircleD, ImageReducedFindModel, ImageAffinTrans_Model;
				Hobject  ImageChange, ModelImage;


				// Local control variables 
				HTuple  Length_set, Index;
				HTuple  Model_Num,Model_Radius;
				HTuple  Rows, Columns, Length2;
				HTuple  train_begin, Index1, Row, Column, Angle;
				HTuple  Score, HomMat2D, RowTrans, ColTrans, Index2, Value_r;
				HTuple  Value_c, Row_AffinTrans, Column_AffinTrans, Angle_AffinTrans;
				HTuple  Score_AffinTrans, HomMat2DT, RowTrans1, ColTrans1, RowRound, ColRound;
				HTuple  RowTransAll, ColTransAll, Grayval;
				//HTuple  MatrixAdd,MatrixSubID, MatrixIDData, MatrixIDTemp;
				HTuple  IndexM, ModelIDR, Row3, Column3, Angle3, Score3;
				HTuple  Value_r2, Value_c2,AllModelData,SelectedTuple;
				Model_Radius = 120;
				//*vc code**********************************************************************
				count_channels(MultiChannelImageTest, &Length_set);//100
				count_channels(m_multiChannelModelImage, &Model_Num);
				gen_circle(&ModelRegion, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]),Model_Radius);
				gen_circle(&RegionD, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), 190.5);
				get_region_points(RegionD, &Rows, &Columns);
				copy_image(m_multiChannelModelImage, &DupImage);
				tuple_length(Columns, &Length2);
				gen_rectangle1(&Rectangle_Contour, 120-90, 330-90, 120+90, 330+90);
				tuple_gen_const(Length2*(Model_Num), 0, &AllModelData);
				train_begin = HTuple();
				for (Index1=0; Index1<=Model_Num; Index1+=1)
				{
					train_begin[Index1] = 0;
				}
				gen_empty_obj(&Image1);
				for (Index=0; Index<=Length_set-1; Index+=1)
				{
					access_channel(MultiChannelImageTest, &Image, Index+1);
					reduce_domain(Image, Rectangle_Contour, &ImageReducedDefine);
					find_shape_model(ImageReducedDefine, m_modelID, -0.49, 0.49, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row, &Column, &Angle, &Score);

					if (0 != (Score>0.6))
					{
						vector_angle_to_rigid(HTuple(m_centerTuple[2]), HTuple(m_centerTuple[3]), 0, Row, Column, Angle, &HomMat2D);
						affine_trans_pixel(HomMat2D, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), &RowTrans, &ColTrans);
						gen_circle(&CircleD, RowTrans, ColTrans, 30.5);
						reduce_domain(Image, CircleD, &ImageReducedFindModel);
						for (Index2=1; Index2<=Model_Num; Index2+=1)
						{
							get_value_matrix(MatrixID, Index2-1, 0, &Value_r);
							get_value_matrix(MatrixID, Index2-1, 1, &Value_c);
							access_channel(m_multiChannelModelImage, &Image1, Index2);
							find_shape_model(ImageReducedFindModel, HTuple(Models[Index2-1]), 0, 6.29, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row_AffinTrans, &Column_AffinTrans, &Angle_AffinTrans, &Score_AffinTrans);
							if (0 != (Score_AffinTrans>0.85))
							{
								vector_angle_to_rigid(Row_AffinTrans, Column_AffinTrans, Angle_AffinTrans, Value_r, Value_c, 0, &HomMat2DT);
								affine_trans_pixel(HomMat2DT, RowTrans, ColTrans, &RowTrans1, &ColTrans1);
                                gen_circle(&RegionD, RowTrans1, ColTrans1, 190.5);
                                reduce_domain(Image, RegionD, &ImageReduced3);
								affine_trans_image(ImageReduced3, &ImageAffinTrans_Model, HomMat2DT, "bilinear", "false");
								RowTransAll = Rows+(Value_r-HTuple(m_centerTuple[0]));
								ColTransAll = Columns+(Value_c-HTuple(m_centerTuple[1]));
								get_grayval_interpolated(ImageAffinTrans_Model, RowTransAll, ColTransAll, "bilinear", &Grayval);
								tuple_select_range(AllModelData, (Index2-1)*Length2, (Index2*Length2)-1, &SelectedTuple);
		                        tuple_add(SelectedTuple, Grayval, &SelectedTuple);
		                        memcpy(&AllModelData[(Index2-1)*Length2],&SelectedTuple[0],(Length2[0].I())*16);
								train_begin[Index2] = HTuple(train_begin[Index2])+1;
							}

						}
					}

				}
				ModelsNew = HTuple();
				Model_Num_New = 0;
				gen_empty_obj(&m_multiChannelModelImageNew);
				create_matrix(200, 4, 0, &MatrixIDNew);
				for (IndexM=1; IndexM<=Model_Num; IndexM+=1)
				{
					if (0 != (HTuple(train_begin[IndexM])>5))
					{
						Model_Num_New += 1;
						tuple_select_range(AllModelData, (IndexM-1)*Length2, (IndexM*Length2)-1, &Grayval);
						//get_sub_matrix(MatrixIDData, IndexM-1, 0, 1, Length2, &MatrixSubID);
						//get_full_matrix(MatrixSubID, &Grayval);

						Grayval = Grayval/(HTuple(train_begin[IndexM])+0.01);
						access_channel(DupImage, &ImageChange, IndexM);
						get_value_matrix(MatrixID, IndexM-1, 0, &Value_r);
						get_value_matrix(MatrixID, IndexM-1, 1, &Value_c);
						RowTransAll = Rows+(Value_r-HTuple(m_centerTuple[0]));
						ColTransAll = Columns+(Value_c-HTuple(m_centerTuple[1]));
						tuple_round(RowTransAll, &RowRound);
						tuple_round(ColTransAll, &ColRound);
						set_grayval(ImageChange, RowRound, ColRound, Grayval);
						//*
						if (0 != (Model_Num_New==1))
						{
							channels_to_image(ImageChange, &m_multiChannelModelImageNew);
						}
						else if (0 != (Model_Num_New>1))
						{
							append_channel(m_multiChannelModelImageNew, ImageChange, &m_multiChannelModelImageNew);
						}
						gen_circle(&ModelRegion, Value_r, Value_c, 120);
						reduce_domain(ImageChange, ModelRegion, &ModelImage);
						create_shape_model(ModelImage, 4, 0, HTuple(360).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelIDR);
						find_shape_model(ModelImage, ModelIDR, 0, 0.5, 0.5, 1, 0.5, "least_squares",0, 0.9, &Row3, &Column3, &Angle3, &Score3);
						if (0 != (Score3>0.8))
						{
							ModelsNew[Model_Num_New-1] = ModelIDR;
							set_value_matrix(MatrixIDNew, Model_Num_New-1, 0, Row3);
							set_value_matrix(MatrixIDNew, Model_Num_New-1, 1, Column3);
						}
						else if (0 != (Score3<=0.8))
						{
							ModelsNew[Model_Num_New-1] = HTuple(Models[IndexM-1]);
							get_value_matrix(MatrixID, IndexM-1, 0, &Value_r2);
							get_value_matrix(MatrixID, IndexM-1, 1, &Value_c2);
							set_value_matrix(MatrixIDNew, Model_Num_New-1, 0, Value_r2);
							set_value_matrix(MatrixIDNew, Model_Num_New-1, 1, Value_c2);
						}
						get_value_matrix(MatrixID, IndexM-1, 2, &Value_r2);
						get_value_matrix(MatrixID, IndexM-1, 3, &Value_c2);
						set_value_matrix(MatrixIDNew, Model_Num_New-1, 2, Value_r2);
						set_value_matrix(MatrixIDNew, Model_Num_New-1, 3, Value_c2);
					}
				}
			}
			else if	(m_channel=="CH1")
			{
				// Local iconic variables 
				Hobject  Image,ImageReduced3;
				Hobject  ModelRegion, RegionD, DupImage, Image1, Region;
				Hobject  CircleD, ImageReducedFindModel, ImageAffinTrans_Model;
				Hobject  ImageChange, ModelImage;


				// Local control variables 
				HTuple  Length_set, Index;
				HTuple  Model_Num,Model_Radius;
				HTuple  Rows, Columns, Length2;
				HTuple  train_begin, Index1, Row, Column, Angle;
				HTuple  Score, HomMat2D, RowTrans, ColTrans, Index2, Value_r;
				HTuple  Value_c, Row_AffinTrans, Column_AffinTrans, Angle_AffinTrans;
				HTuple  Score_AffinTrans, HomMat2DT, RowTrans1, ColTrans1;
				HTuple  RowTransAll, ColTransAll, Grayval, RowRound, ColRound;
				//HTuple  MatrixAdd,MatrixSubID, MatrixIDData, MatrixIDTemp;
				HTuple  IndexM, ModelIDR, Row3, Column3, Angle3, Score3;
				HTuple  Value_r2, Value_c2,AllModelData,SelectedTuple;
				HTuple  m_centerTuple;
				m_centerTuple[0]=240.5;
				m_centerTuple[1]=330.5;
				Model_Radius=80;

				//*vc code**********************************************************************
				count_channels(MultiChannelImageTest, &Length_set);//100
				count_channels(m_multiChannelModelImage, &Model_Num);
				gen_circle(&ModelRegion, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), Model_Radius);
				gen_circle(&RegionD, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), 190.5);
				get_region_points(RegionD, &Rows, &Columns);
				copy_image(m_multiChannelModelImage, &DupImage);
				tuple_length(Columns, &Length2);
				tuple_gen_const(Length2*(Model_Num), 0, &AllModelData);
				train_begin = HTuple();
				for (Index1=0; Index1<=Model_Num; Index1+=1)
				{
					train_begin[Index1] = 0;
				}
				gen_empty_obj(&Image1);
				for (Index=0; Index<=Length_set-1; Index+=1)
				{
					access_channel(MultiChannelImageTest, &Image, Index+1);
					//计算中心点
					m_crtImg =Image;
					CComVariant var1(0.0f),var2(3.0f),var3(8.0f),var4(140.0f),var5(102.0f);
					CrownCapLocationUseEdgeAlgo(&var1,&var2,&var3,&var4,&var5);
					float m_center_x=0.0;
					float m_center_y=0.0;
					m_center_x=m_vPos[0].m_x;
					m_center_y=m_vPos[0].m_y;
					////////////////////////////////////////////////////
					if((m_center_x >2.0)&(m_center_y>2.0))
					{
						RowTrans=m_center_y;
						ColTrans=m_center_x;
						gen_circle(&Region, RowTrans, ColTrans, 190.5);
						gen_circle(&CircleD, RowTrans, ColTrans, 30.5);
						reduce_domain(Image, CircleD, &ImageReducedFindModel);
						for (Index2=1; Index2<=Model_Num; Index2+=1)
						{
							get_value_matrix(MatrixID, Index2-1, 0, &Value_r);
							get_value_matrix(MatrixID, Index2-1, 1, &Value_c);
							access_channel(m_multiChannelModelImage, &Image1, Index2);
							find_shape_model(ImageReducedFindModel, HTuple(Models[Index2-1]), 0, 6.29, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row_AffinTrans, &Column_AffinTrans, &Angle_AffinTrans, &Score_AffinTrans);
							if (0 != (Score_AffinTrans>0.85))
							{
								vector_angle_to_rigid(Row_AffinTrans, Column_AffinTrans, Angle_AffinTrans, Value_r, Value_c, 0, &HomMat2DT);
								affine_trans_pixel(HomMat2DT, RowTrans, ColTrans, &RowTrans1, &ColTrans1);
                                gen_circle(&RegionD, RowTrans1, ColTrans1, 190.5);
                                reduce_domain(Image, RegionD, &ImageReduced3);
								affine_trans_image(ImageReduced3, &ImageAffinTrans_Model, HomMat2DT, "bilinear", "false");
								RowTransAll = Rows+(Value_r-HTuple(m_centerTuple[0]));
								ColTransAll = Columns+(Value_c-HTuple(m_centerTuple[1]));
								get_grayval_interpolated(ImageAffinTrans_Model, RowTransAll, ColTransAll, "bilinear", &Grayval);
								tuple_select_range(AllModelData, (Index2-1)*Length2, (Index2*Length2)-1, &SelectedTuple);
		                        tuple_add(SelectedTuple, Grayval, &SelectedTuple);
		                        memcpy(&AllModelData[(Index2-1)*Length2],&SelectedTuple[0],(Length2[0].I())*16);
								train_begin[Index2] = HTuple(train_begin[Index2])+1;
							}

						}
					}

				}
				ModelsNew = HTuple();
				Model_Num_New = 0;
				gen_empty_obj(&m_multiChannelModelImageNew);
				create_matrix(200, 4, 0, &MatrixIDNew);
				for (IndexM=1; IndexM<=Model_Num; IndexM+=1)
				{
					if (0 != (HTuple(train_begin[IndexM])>5))
					{
						Model_Num_New += 1;
						tuple_select_range(AllModelData, (IndexM-1)*Length2, (IndexM*Length2)-1, &Grayval);
						Grayval = Grayval/(HTuple(train_begin[IndexM])+0.01);
						access_channel(DupImage, &ImageChange, IndexM);
						get_value_matrix(MatrixID, IndexM-1, 0, &Value_r);
						get_value_matrix(MatrixID, IndexM-1, 1, &Value_c);
						RowTransAll = Rows+(Value_r-HTuple(m_centerTuple[0]));
						ColTransAll = Columns+(Value_c-HTuple(m_centerTuple[1]));
						tuple_round(RowTransAll, &RowRound);
						tuple_round(ColTransAll, &ColRound);
						set_grayval(ImageChange, RowRound, ColRound, Grayval);
						//*
						if (0 != (Model_Num_New==1))
						{
							channels_to_image(ImageChange, &m_multiChannelModelImageNew);
						}
						else if (0 != (Model_Num_New>1))
						{
							append_channel(m_multiChannelModelImageNew, ImageChange, &m_multiChannelModelImageNew);
						}
						gen_circle(&ModelRegion, Value_r, Value_c, Model_Radius);
						reduce_domain(ImageChange, ModelRegion, &ModelImage);
						create_shape_model(ModelImage, 4, 0, HTuple(360).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelIDR);
						find_shape_model(ModelImage, ModelIDR, 0, 0.5, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row3, &Column3, &Angle3, &Score3);
						if (0 != (Score3>0.8))
						{
							ModelsNew[Model_Num_New-1] = ModelIDR;
							set_value_matrix(MatrixIDNew, Model_Num_New-1, 0, Row3);
							set_value_matrix(MatrixIDNew, Model_Num_New-1, 1, Column3);
						}
						else if (0 != (Score3<=0.8))
						{
							ModelsNew[Model_Num_New-1] = HTuple(Models[IndexM-1]);
							get_value_matrix(MatrixID, IndexM-1, 0, &Value_r2);
							get_value_matrix(MatrixID, IndexM-1, 1, &Value_c2);
							set_value_matrix(MatrixIDNew, Model_Num_New-1, 0, Value_r2);
							set_value_matrix(MatrixIDNew, Model_Num_New-1, 1, Value_c2);
						}
						get_value_matrix(MatrixID, IndexM-1, 2, &Value_r2);
						get_value_matrix(MatrixID, IndexM-1, 3, &Value_c2);
						set_value_matrix(MatrixIDNew, Model_Num_New-1, 2, Value_r2);
						set_value_matrix(MatrixIDNew, Model_Num_New-1, 3, Value_c2);
					}
				}
			}
			if(Model_Num_New<1)
				return S_FALSE;
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

STDMETHODIMP CCrownCapAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{
	ResetModels(productID);
	if(productID==1)
	{
		try
		{
			if(Model_Num_New<1)
				return S_FALSE;
			if(m_channel=="CH2")
			{
				// Local iconic variables 
				Hobject  Image;
				Hobject  ImageM,ImageAffinTrans,ImageReduced2;
				Hobject  Image1,Rectangle_Contour;

				// Local control variables 
				HTuple  Index, ModelID, Tuple;
				HTuple  ModelMatchNum, ModelDefineNum, Length_set, Index_m;
				HTuple  RowY1, ColumnX1, center_data_x, center_data_y,ResultRows,ResultColumns,ResultAngles,ResultScores;
				HTuple  train_begin, Row, Column, Angle, Score, HomMat2D;
				HTuple  RowTrans, ColTrans, HomMat2DT, Row_AffinTrans, Column_AffinTrans;
				HTuple  Angle_AffinTrans, Score_AffinTrans, HomMat2D2, distX;
				HTuple  distY, distPowX, distPowY, distXY, SqrtDistXY, Mean;
				HTuple  Deviation, mean_m, deviation_m, ImageModelScore;
				HTuple  ImageDist, Index1, Indices;
				HTuple  IndexM, Value_r, Value_c;
				HTuple  Value_r2, Value_c2;
				//*vc code begin*****************************************************************
				Tuple = HTuple();
				ModelMatchNum = HTuple();
				ModelDefineNum = HTuple();
				count_channels(MultiChannelImageTest, &Length_set);
				for (Index_m=1; Index_m<=Model_Num_New; Index_m+=1)
				{
					ModelDefineNum[Index_m-1] = 0;
				}
				//
				gen_rectangle1(&Rectangle_Contour, 120-90, 330-90, 120+90, 330+90);
				//**************
				ResultRows = HTuple();
				ResultColumns = HTuple();
				ResultAngles = HTuple();
				ResultScores = HTuple();
				for (Index=0; Index<=Length_set-1; Index+=1)
				{
					access_channel(MultiChannelImageTest, &Image, Index+1);
					reduce_domain(Image, Rectangle_Contour, &ImageReduced2);
					find_shape_model(ImageReduced2, m_modelID, -0.49, 0.49, 0.5, 1, 0.5, "least_squares", 0, 0.9, &Row, &Column, &Angle, &Score);
					if (0 != (Score>0.1))
					{
						ResultRows[Index] = Row;
						ResultColumns[Index] = Column;
						ResultAngles[Index] = Angle;
						ResultScores[Index] = Score;
					}
					else if (0 != (Score<=0.1))
					{
						ResultRows[Index] = 0;
						ResultColumns[Index] = 0;
						ResultAngles[Index] = 0;
						ResultScores[Index] = 0;
					}
				}
				//
				for (Index_m=1; Index_m<=Model_Num_New; Index_m+=1)
				{
					access_channel(m_multiChannelModelImageNew, &ImageM, Index_m);
					get_value_matrix(MatrixIDNew, Index_m-1, 0, &RowY1);
					get_value_matrix(MatrixIDNew, Index_m-1, 1, &ColumnX1);
					ModelID = ModelsNew[Index_m-1];
					Tuple[1] = RowY1;
					Tuple[2] = ColumnX1;
					Tuple[0] = 0;
					Tuple[4] = RowY1;
					Tuple[5] = ColumnX1;
					center_data_x = HTuple();
					center_data_y = HTuple();
					//***************
					train_begin = 0;
					for (Index=0; Index<=Length_set-1; Index+=1)
					{
						access_channel(MultiChannelImageTest, &Image, Index+1);
						Row = ResultRows[Index];
						Column = ResultColumns[Index];
						Angle = ResultAngles[Index];
						Score = ResultScores[Index];
						if (0 != (Score>0.6))
						{
							vector_angle_to_rigid(HTuple(m_centerTuple[2]), HTuple(m_centerTuple[3]), 0, Row, Column, Angle, &HomMat2D);
							affine_trans_pixel(HomMat2D, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), &RowTrans, &ColTrans);
							vector_angle_to_rigid(RowTrans, ColTrans, 0, RowY1, ColumnX1, 0, &HomMat2DT);
							affine_trans_image(Image, &ImageAffinTrans, HomMat2DT, "bilinear", "false");
							find_shape_model(ImageAffinTrans, ModelID, 0, HTuple(360).Rad(), 0.3, 1, 0.5, "least_squares", (HTuple(4).Append(-2)), 0.7, &Row_AffinTrans, &Column_AffinTrans, &Angle_AffinTrans, &Score_AffinTrans);
							if (0 != (Score_AffinTrans>0.65))
							{
								vector_angle_to_rigid(Row_AffinTrans, Column_AffinTrans, Angle_AffinTrans, RowY1, ColumnX1, 0, &HomMat2D2);
								affine_trans_pixel(HomMat2D2, RowY1, ColumnX1, &RowTrans, &ColTrans);
								center_data_x[train_begin] = ColTrans;
								center_data_y[train_begin] = RowTrans;
								train_begin += 1;
							}
							ModelDefineNum[Index_m-1] = HTuple(ModelDefineNum[Index_m-1])+1;
						}
					}
					if (0 != (train_begin>0))
					{
						distX = center_data_x-ColumnX1;
						distY = center_data_y-RowY1;
						tuple_pow(distX, 2, &distPowX);
						tuple_pow(distY, 2, &distPowY);
						tuple_add(distPowY, distPowX, &distXY);
						tuple_sqrt(distXY, &SqrtDistXY);
						tuple_mean(SqrtDistXY, &Mean);
						tuple_deviation(SqrtDistXY, &Deviation);
						mean_m[Index_m-1] = Mean;
						deviation_m[Index_m-1] = Deviation;
					}
					else if (0 != (train_begin==0))
					{
						mean_m[Index_m-1] = 100;
						deviation_m[Index_m-1] = 100;
					}
					ModelMatchNum[Index_m-1] = train_begin;
				}

				ImageModelScore = HTuple();
				ImageDist = 18.0;//negative number ,sometimes mean_m[Index1-1]) is bigger than ImageDist. 
				for (Index1=1; Index1<=Model_Num_New; Index1+=1)
				{
					ImageModelScore[Index1-1] = ((ImageDist-HTuple(mean_m[Index1-1]))/ImageDist)*(HTuple(ModelMatchNum[Index1-1])/(HTuple(ModelDefineNum[Index1-1])+0.001));
					ImageModelScore[Index1-1] = HTuple(ImageModelScore[Index1-1])*100;
				}
				tuple_sort_index(ImageModelScore, &Indices);
				ModelsSort = HTuple();
				gen_empty_obj(&m_multiChannelModelImageSort);
				create_matrix(200, 4, 0, &MatrixIDSort);
				for (IndexM=1; IndexM<=Model_Num_New; IndexM+=1)
				{
					access_channel(m_multiChannelModelImageNew, &Image1, HTuple(Indices[IndexM-1])+1);
					if (0 != (IndexM==1))
					{
						channels_to_image(Image1, &m_multiChannelModelImageSort);
					}
					else if (0 != (IndexM>1))
					{
						append_channel(m_multiChannelModelImageSort, Image1, &m_multiChannelModelImageSort);
					}
					ModelsSort[IndexM-1] = HTuple(ModelsNew[HTuple(Indices[IndexM-1])]);
					get_value_matrix(MatrixIDNew, HTuple(Indices[IndexM-1]), 0, &Value_r);
					get_value_matrix(MatrixIDNew, HTuple(Indices[IndexM-1]), 1, &Value_c);
					set_value_matrix(MatrixIDSort, IndexM-1, 0, Value_r);
					set_value_matrix(MatrixIDSort, IndexM-1, 1, Value_c);
					get_value_matrix(MatrixIDNew, HTuple(Indices[IndexM-1]), 2, &Value_r2);
					get_value_matrix(MatrixIDNew, HTuple(Indices[IndexM-1]), 3, &Value_c2);
					set_value_matrix(MatrixIDSort, IndexM-1, 2, Value_r2);
					set_value_matrix(MatrixIDSort, IndexM-1, 3, Value_c2);
					ScoreSort[IndexM-1] =ImageModelScore[HTuple(Indices[IndexM-1])];
				}
				Model_Num_Sort=Model_Num_New;
			}
			else if(m_channel=="CH1")
			{
				// Local iconic variables 
				Hobject  Image;
				Hobject  ImageM,ImageAffinTrans;
				Hobject  Image1;
				// Local control variables 
				HTuple  Index, ModelID, Tuple;
				HTuple  ModelMatchNum, ModelDefineNum, Length_set, Index_m;
				HTuple  RowY1, ColumnX1, center_data_x, center_data_y,ResultRows,ResultColumns;
				HTuple  train_begin, Row, Column, Angle, Score, HomMat2D;
				HTuple  RowTrans, ColTrans, HomMat2DT, Row_AffinTrans, Column_AffinTrans;
				HTuple  Angle_AffinTrans, Score_AffinTrans, HomMat2D2, distX;
				HTuple  distY, distPowX, distPowY, distXY, SqrtDistXY, Mean;
				HTuple  Deviation, mean_m, deviation_m, ImageModelScore;
				HTuple  ImageDist, Index1, Indices;
				HTuple  IndexM, Value_r, Value_c;
				HTuple  Value_r2, Value_c2;
				//*vc code begin*****************************************************************
				Tuple = HTuple();
				ModelMatchNum = HTuple();
				ModelDefineNum = HTuple();
				count_channels(MultiChannelImageTest, &Length_set);
				for (Index_m=1; Index_m<=Model_Num_New; Index_m+=1)
				{
					ModelDefineNum[Index_m-1] = 0;
				}
				//**************
				ResultRows = HTuple();
				ResultColumns = HTuple();
				for (Index=0; Index<=Length_set-1; Index+=1)
				{
					access_channel(MultiChannelImageTest, &Image, Index+1);
					//计算中心点
					m_crtImg =Image;
					CComVariant var1(0),var2(3),var3(8),var4(180),var5(102);
					CrownCapLocationUseEdgeAlgo(&var1,&var2,&var3,&var4,&var5);
					ResultColumns[Index]=m_vPos[0].m_x;
					ResultRows[Index]=m_vPos[0].m_y;

				}	
				for (Index_m=1; Index_m<=Model_Num_New; Index_m+=1)
				{
					access_channel(m_multiChannelModelImageNew, &ImageM, Index_m);
					get_value_matrix(MatrixIDNew, Index_m-1, 0, &RowY1);
					get_value_matrix(MatrixIDNew, Index_m-1, 1, &ColumnX1);
					ModelID = ModelsNew[Index_m-1];
					Tuple[1] = RowY1;
					Tuple[2] = ColumnX1;
					Tuple[0] = 0;
					Tuple[4] = RowY1;
					Tuple[5] = ColumnX1;
					center_data_x = HTuple();
					center_data_y = HTuple();
					//***************
					train_begin = 0;
					for (Index=0; Index<=Length_set-1; Index+=1)
					{
						access_channel(MultiChannelImageTest, &Image, Index+1);
						float m_center_x=0.0;
						float m_center_y=0.0;
						m_center_x=(float)ResultColumns[Index].D();
						m_center_y=(float)ResultRows[Index].D();
						////////////////////////////////////////////////////
						if((m_center_x >2.0)&(m_center_y>2.0))
						{
							vector_angle_to_rigid(m_center_y, m_center_x, 0, RowY1, ColumnX1, 0, &HomMat2DT);
							affine_trans_image(Image, &ImageAffinTrans, HomMat2DT, "bilinear", "false");
							find_shape_model(ImageAffinTrans, ModelID, 0, HTuple(360).Rad(), 0.3, 1, 0.5, "least_squares", (HTuple(4).Append(-2)), 0.7, &Row_AffinTrans, &Column_AffinTrans, &Angle_AffinTrans, &Score_AffinTrans);
							if (0 != (Score_AffinTrans>0.65))
							{
								vector_angle_to_rigid(Row_AffinTrans, Column_AffinTrans, Angle_AffinTrans, RowY1, ColumnX1, 0, &HomMat2D2);
								affine_trans_pixel(HomMat2D2, RowY1, ColumnX1, &RowTrans, &ColTrans);
								center_data_x[train_begin] = ColTrans;
								center_data_y[train_begin] = RowTrans;
								train_begin += 1;
							}
							ModelDefineNum[Index_m-1] = HTuple(ModelDefineNum[Index_m-1])+1;
						}
					}
					if (0 != (train_begin>0))
					{
						distX = center_data_x-ColumnX1;
						distY = center_data_y-RowY1;
						tuple_pow(distX, 2, &distPowX);
						tuple_pow(distY, 2, &distPowY);
						tuple_add(distPowY, distPowX, &distXY);
						tuple_sqrt(distXY, &SqrtDistXY);
						tuple_mean(SqrtDistXY, &Mean);
						tuple_deviation(SqrtDistXY, &Deviation);
						mean_m[Index_m-1] = Mean;
						deviation_m[Index_m-1] = Deviation;
					}
					else if (0 != (train_begin==0))
					{
						mean_m[Index_m-1] = 100;
						deviation_m[Index_m-1] = 100;
					}
					ModelMatchNum[Index_m-1] = train_begin;
				}

				ImageModelScore = HTuple();
				ImageDist = 18.0;//negative number ,sometimes mean_m[Index1-1]) is bigger than ImageDist. 
				for (Index1=1; Index1<=Model_Num_New; Index1+=1)
				{
					ImageModelScore[Index1-1] = ((ImageDist-HTuple(mean_m[Index1-1]))/ImageDist)*(HTuple(ModelMatchNum[Index1-1])/(HTuple(ModelDefineNum[Index1-1])+0.001));
					ImageModelScore[Index1-1] = HTuple(ImageModelScore[Index1-1])*100;
				}
				tuple_sort_index(ImageModelScore, &Indices);
				ModelsSort = HTuple();
				gen_empty_obj(&m_multiChannelModelImageSort);
				create_matrix(200, 4, 0, &MatrixIDSort);
				for (IndexM=1; IndexM<=Model_Num_New; IndexM+=1)
				{
					access_channel(m_multiChannelModelImageNew, &Image1, HTuple(Indices[IndexM-1])+1);
					if (0 != (IndexM==1))
					{
						channels_to_image(Image1, &m_multiChannelModelImageSort);
					}
					else if (0 != (IndexM>1))
					{
						append_channel(m_multiChannelModelImageSort, Image1, &m_multiChannelModelImageSort);
					}
					ModelsSort[IndexM-1] = HTuple(ModelsNew[HTuple(Indices[IndexM-1])]);
					get_value_matrix(MatrixIDNew, HTuple(Indices[IndexM-1]), 0, &Value_r);
					get_value_matrix(MatrixIDNew, HTuple(Indices[IndexM-1]), 1, &Value_c);
					set_value_matrix(MatrixIDSort, IndexM-1, 0, Value_r);
					set_value_matrix(MatrixIDSort, IndexM-1, 1, Value_c);
					get_value_matrix(MatrixIDNew, HTuple(Indices[IndexM-1]), 2, &Value_r2);
					get_value_matrix(MatrixIDNew, HTuple(Indices[IndexM-1]), 3, &Value_c2);
					set_value_matrix(MatrixIDSort, IndexM-1, 2, Value_r2);
					set_value_matrix(MatrixIDSort, IndexM-1, 3, Value_c2);
					ScoreSort[IndexM-1] =ImageModelScore[HTuple(Indices[IndexM-1])];
				}
				Model_Num_Sort=Model_Num_New;
			}
			//替换训练结果/////////////////////////////
			gen_empty_obj(&m_multiChannelModelImage);
			Models = HTuple();
			MatrixID = HTuple();
			copy_image(m_multiChannelModelImageSort,&m_multiChannelModelImage);
			copy_matrix(MatrixIDSort,&MatrixID);
			Models=ModelsSort;
			///////////////////////////////
		}
		catch (HException &except) 
		{
			m_algoSpy.QueueUpLogMsg(except, m_crtImg);
			return S_FALSE;
		}
	}

	Hobject* pObj = (Hobject*)multiModelImag;
	*pObj = m_multiChannelModelImage;

	*cMax = (long)(Model_Num_Sort[0].I());//m_vImgScore.size();
	for(size_t t = 0; t <Model_Num_Sort[0].I(); t++)
	{
		vImgScore[t] = (float)ScoreSort[t].D();
	}
	return S_OK;
}


void CCrownCapAlgo::redefine_use_line (Hobject Image,Hobject *RegionOpening, HTuple StepAng, 
	HTuple Radius_inner, HTuple Radius,HTuple Row_1, HTuple Column_1)
{
	// Local iconic variables 
	Hobject  RegionLines, Region3, Contour_nurbs;
	Hobject  RegionTest, RegionClosing;


	// Local control variables 
	HTuple  StepNum, BeginRow, BeginCol, EndRow, EndCol;
	HTuple  SinRate, CosRate, i, MatrixID, MatrixIDX, MatrixIDY;
	HTuple  MatrixID_sub, MatrixID_subY, MatrixID_subY1, MatrixID_subY2;
	HTuple  pRow, pCol, Min, Max, Min1, Max1, RowRound, ColRound;
	HTuple  Index2, Grayval1, DefinedRow, DefinedCol, Index1;
	HTuple  Values1, Values2, Diff, Indices, ValueX, ValueY;

	//***
	//StepAng := 1.5
	StepNum = 360/StepAng;
	BeginRow = HTuple();
	BeginCol = HTuple();
	EndRow = HTuple();
	EndCol = HTuple();
	SinRate = HTuple();
	CosRate = HTuple();
	//Radius_inner := 207
	//Radius := 220
	//StepNum
	for (i=1; i<=StepNum; i+=1)
	{
		SinRate.Append(((StepAng*(i-1)).Rad()).Sin());
		CosRate.Append(((StepAng*(i-1)).Rad()).Cos());
	}
	BeginRow = Row_1-(Radius_inner*SinRate);
	BeginCol = Column_1+(Radius_inner*CosRate);
	EndRow = Row_1-(Radius*SinRate);
	EndCol = Column_1+(Radius*CosRate);
	gen_region_line(&RegionLines, BeginRow, BeginCol, EndRow, EndCol);
	create_matrix((Radius-Radius_inner)+1, StepNum, 0, &MatrixID);
	create_matrix((Radius-Radius_inner)+1, StepNum, 0, &MatrixIDX);
	create_matrix((Radius-Radius_inner)+1, StepNum, 0, &MatrixIDY);
	create_matrix(1, StepNum, 0, &MatrixID_sub);
	create_matrix((Radius-Radius_inner)+1, 1, 0, &MatrixID_subY);
	create_matrix(Radius-Radius_inner, 1, 0, &MatrixID_subY1);
	create_matrix(Radius-Radius_inner, 1, 0, &MatrixID_subY2);
	for (i=0; i<=Radius-Radius_inner; i+=1)
	{
		pRow = BeginRow+((EndRow-BeginRow)*(i/((Radius-Radius_inner)+0.001)));
		pCol = BeginCol+((EndCol-BeginCol)*(i/((Radius-Radius_inner)+0.001)));
		tuple_min(pRow, &Min);
		tuple_max(pRow, &Max);
		tuple_min(pCol, &Min1);
		tuple_max(pCol, &Max1);
		//坐标超出图片范围！
		tuple_round(pRow, &RowRound);
		tuple_round(pCol, &ColRound);
		for (Index2=0; Index2<=StepNum-1; Index2+=1)
		{
			if (0 != (HTuple(RowRound[Index2])<0))
			{
				RowRound[Index2] = 0;
			}
		}
		gen_region_points(&Region3, RowRound, ColRound);
		get_grayval(Image, RowRound, ColRound, &Grayval1);
		set_full_matrix(MatrixID_sub, Grayval1);
		set_sub_matrix(MatrixID, MatrixID_sub, i, 0);
		set_full_matrix(MatrixID_sub, RowRound);
		set_sub_matrix(MatrixIDY, MatrixID_sub, i, 0);
		set_full_matrix(MatrixID_sub, ColRound);
		set_sub_matrix(MatrixIDX, MatrixID_sub, i, 0);
	}
	DefinedRow = HTuple();
	DefinedCol = HTuple();
	for (Index1=0; Index1<=StepNum-1; Index1+=1)
	{
		get_sub_matrix(MatrixID, 0, Index1, Radius-Radius_inner, 1, &MatrixID_subY1);
		get_full_matrix(MatrixID_subY1, &Values1);
		get_sub_matrix(MatrixID, 1, Index1, Radius-Radius_inner, 1, &MatrixID_subY2);
		get_full_matrix(MatrixID_subY2, &Values2);
		tuple_sub(Values1, Values2, &Diff);
		tuple_sort_index(Diff, &Indices);
		get_value_matrix(MatrixIDX, HTuple(Indices[(Radius-Radius_inner)-1]), Index1, &ValueX);
		get_value_matrix(MatrixIDY, HTuple(Indices[(Radius-Radius_inner)-1]), Index1, &ValueY);
		DefinedRow[Index1] = ValueY;
		DefinedCol[Index1] = ValueX;
	}
	DefinedRow[StepNum] = HTuple(DefinedRow[0]);
	DefinedCol[StepNum] = HTuple(DefinedCol[0]);
	gen_contour_nurbs_xld(&Contour_nurbs, DefinedRow, DefinedCol, "auto", "auto", 3, 1, 5);
	gen_region_contour_xld(Contour_nurbs, &RegionTest, "filled");
	closing_circle(RegionTest, &RegionClosing, 50.5);
	opening_circle(RegionClosing, &(*RegionOpening),Radius_inner-6.5);
}

STDMETHODIMP CCrownCapAlgo::CanLocationAlgo(VARIANT* rgnId, VARIANT* pThredLow, VARIANT* pRadius, VARIANT* pRoiWith)
{
	int id = (int)rgnId->fltVal;
	int Thred_Low = (int)pThredLow->fltVal;
	long m_RadiusInner = (long)pRadius->fltVal;	
	long m_RadiusExt = m_RadiusInner+(long)pRoiWith->fltVal;
	///////////////////////////////////////////////////
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	// Local iconic variables 
	Hobject  Image,Rectangle, ImageReduced1, Region;
	Hobject  ConnectedRegions, SelectedRegions, RegionUnion;
	Hobject  RegionFillUp, RegionOpeningCut, RegionClosingT;
	Hobject  RegionOpeningT, Contours, Ellipse, Circle1, Circle2;
	Hobject  RegionErosion1, RegionDifference1, ImageReduced;
	Hobject  Region1, RegionErosion, RegionDilation_ex, RegionDifference;
	Hobject  RegionOpening;

	// Local control variables 
	HTuple  Row1, Column1, Phi, Radius1, Radius2;
	HTuple  StartPhi, EndPhi, PointOrder, Row, Column, Radius;
	HTuple  Mean, Deviation, Area_1, Row_1, Column_1, Area;
	HTuple  Row_C, Column_C;

	try
	{
		Image=m_crtImg;
		gen_ellipse(&Rectangle, 248, 328, 0, 350, 290);
		reduce_domain(Image, Rectangle, &ImageReduced1);
		threshold(ImageReduced1, &Region, Thred_Low, 255);
		erosion_circle(Region, &Region, 1.5);
		connection(Region, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", 4000, 250000);
		union1(SelectedRegions, &RegionUnion);
		fill_up(RegionUnion, &RegionFillUp);
		opening_circle(RegionFillUp, &RegionOpeningCut, 100.5);
		closing_circle(RegionOpeningCut, &RegionClosingT, 50.5);
		opening_circle (RegionClosingT, &RegionOpeningT, 160.5); 
		gen_contour_region_xld(RegionOpeningT, &Contours, "border");
		fit_ellipse_contour_xld(Contours, "fitzgibbon", -1, 0, 0, 200, 3, 2, &Row1, &Column1, &Phi, &Radius1, &Radius2, &StartPhi, &EndPhi, &PointOrder);
		gen_ellipse(&Ellipse, Row1, Column1, Phi, Radius1, Radius2);

		erosion_circle(Ellipse, &RegionErosion, 13.5);
		area_center(RegionErosion, &Area_1, &Row_1, &Column_1);
		redefine_use_line(Image, &RegionOpening, 1.5, m_RadiusInner, m_RadiusExt, Row_1, Column_1);
		//*********************************************************************************
		if(RegionOpening.Id() != H_EMPTY_REGION)
		{	
			area_center(RegionOpening, &Area, &Row_C, &Column_C);
			m_vPos[0].m_x=(float)Column_C[0].D();
			m_vPos[0].m_y=(float)Row_C[0].D();
			m_vRgn[id] = RegionOpening ;
		}
		else
		{
			m_vPos[0].m_x=0.0;
			m_vPos[0].m_y=0.0;
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CanLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("320$\
					 167;B$\
					 191;LP;0;255;1;150$\
					 194;LP;100;280;1;140$\
					 308;LP;4;100;1;20");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::CanCircleDetectAlgo(VARIANT* rgnID,VARIANT* pRateAngle, VARIANT* pBlackDist,VARIANT* pErosionSize,  VARIANT* pMinArea, VARIANT* pChannel,VARIANT* pErrorGrayDist, VARIANT* pDetectNumber)
{
	int rId = (int)rgnID->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	float m_rateAngle = (float)pRateAngle->fltVal;
	m_rateAngle=(float)((m_rateAngle/180.0)*3.14);
	int m_blackDist =(int)pBlackDist->fltVal;
	int m_grayDist =(int)pErrorGrayDist->fltVal;
	float m_erosionSize=(float)pErosionSize->fltVal;
	int m_minArea = (int)pMinArea->fltVal;
	int m_detectNumber = (int)pDetectNumber->fltVal;
	int m_productID=(int)pChannel->fltVal;
	float m_center_x;
	float m_center_y;
	if(m_productID==1)
	{
		m_center_x=m_vPos[0].m_x;
		m_center_y=m_vPos[0].m_y;
	}
	else if(m_productID==2)
	{
		m_center_x=m_vPicPos[0].m_x;
		m_center_y=m_vPicPos[0].m_y;
	}

	Hobject Image = m_crtImg;
	Hobject  RegionDifference_ext;
	Hobject  ImageReduced_ext,ImageReduced_Rate;
	HTuple   HomMat2D;
	Hobject  Region_Black,RegionErosion_Black,  ConnectedRegions,RegionDilation_Black,RegionUnion_detect,Region_black,Region_black_ex;
	Hobject  SelectedRegions,RegionUnionEnd;
	HTuple   Number, Row_Test, Column_Test,Min2, Max2, Range2;
	HTuple  Mean, Deviation;
	Number[0]=0;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		RegionDifference_ext=rgn;
		reduce_domain(Image, RegionDifference_ext, &ImageReduced_ext);

		//*环检测**************
		vector_angle_to_rigid(m_center_y, m_center_x, 0,m_center_y, m_center_x, m_rateAngle, &HomMat2D);

		affine_trans_image(ImageReduced_ext, &ImageReduced_Rate, HomMat2D, "bilinear", "false");
		//排除黑色影响
		min_max_gray (RegionDifference_ext, ImageReduced_Rate,0.5, &Min2, &Max2, &Range2);
		threshold(ImageReduced_Rate, &Region_black, 0, Min2);   
		dilation_circle(Region_black, &Region_black_ex, 1.5);
		//
		dyn_threshold(ImageReduced_ext, ImageReduced_Rate, &Region_Black, m_blackDist, "light");//not_equal
		//
		difference(Region_Black,Region_black_ex,&Region_Black);
		//
		erosion_circle(Region_Black, &RegionErosion_Black,m_erosionSize);

		dilation_circle(RegionErosion_Black, &RegionDilation_Black,m_erosionSize+0.5);

		connection(RegionDilation_Black, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and",m_minArea, 99999);	
		union1(SelectedRegions,&RegionUnion_detect);
		area_center(RegionUnion_detect, &Number, &Row_Test, &Column_Test);
		//count_obj(SelectedRegions, &Number);
		if(Number<=0)//SelectedRegions.Id() == H_EMPTY_REGION
		{
			retValue = 0;
		}
		else
		{
			retValue = Number[0].I();
		}
		m_vErrorRgn[rId] = SelectedRegions;
		retValue.Detach(pDetectNumber);
		//临时检测
		if(m_productID==1)
		{
			intensity(RegionDifference_ext,Image, &Mean, &Deviation);
			double m_test=Mean[0].D();
			double m_mean_gray=0.0;
			if(Number<=0)//SelectedRegions.Id() == H_EMPTY_REGION
			{
				if(rId==2)
				{
					m_meanGray_m=m_meanGray_m+m_test;
					m_addNumber_m=m_addNumber_m+1.0;
				}
				if(rId==0)
				{
					m_meanGray_c=m_meanGray_c+m_test;
					m_addNumber_c=m_addNumber_c+1.0;
				}				
			}
			else
			{
				if(Number[0].I()<120)
				{	
					if(rId==2)
					{
						m_meanGray_m=m_meanGray_m+m_test;
						m_addNumber_m=m_addNumber_m+1.0;
					}
					if(rId==0)
					{
						m_meanGray_c=m_meanGray_c+m_test;
						m_addNumber_c=m_addNumber_c+1.0;
					}
				}
			}

			if(rId==2)
			{
				if(m_addNumber_m<1.0)
				{
					m_mean_gray=m_test;
				}
				else
					m_mean_gray=m_meanGray_m/m_addNumber_m;

				if((m_test-m_mean_gray)>m_grayDist)
				{
					retValue =-1;
					retValue.Detach(pDetectNumber);
					return S_FALSE;
				}
			}
			if(rId==0)
			{
				if(m_addNumber_c<1.0)
				{
					m_mean_gray=m_test;
				}
				else
					m_mean_gray=m_meanGray_c/m_addNumber_c;
				if((m_test-m_mean_gray)>m_grayDist)
				{
					retValue =-1;
					retValue.Detach(pDetectNumber);
					return S_FALSE;
				}
			}
		}
		//
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::CanCircleDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("309$\
					 200;R;LP;1;200;1$\
					 167;B$\
					 310;FP;0.5;20;0.1;7.5$\
					 311;LP;1;255;1;50$\
					 312;FP;0.5;15;0.5;2.5$\
					 199;LP;2;200;1;10$\
					 252;LP;1;20;1;1$\
					 293;LP;2;250;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{
	return S_OK;
}
STDMETHODIMP CCrownCapAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::ResetSelfLearningProcedure(void)
{
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::ReloadExternalFile(BSTR* bstrPath)
{
	return S_FALSE;
}

STDMETHODIMP CCrownCapAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码
	return S_OK;
}

STDMETHODIMP CCrownCapAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码
	return S_OK;
}
