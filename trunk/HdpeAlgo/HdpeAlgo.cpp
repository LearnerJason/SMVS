#include "stdafx.h"
#include "resource.h"
#include "HdpeAlgo_i.h"
#include "dllmain.h"
#include "HdpeAlgo.h"
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


// CanAlgo.cpp : CCanAlgo  Implementation
STDMETHODIMP CHdpeAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IHdpeAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CHdpeAlgo::SetCurrentImage(LONG* pImg)
{
	Hobject* pObj = (Hobject*)pImg;
	m_crtImgMult  = *pObj;// 原始图片，可能是多通道图片（彩色图片）
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
STDMETHODIMP CHdpeAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	//Hobject* pObj = (Hobject*)pRgn;
	//*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CHdpeAlgo::GetCurrentLocationXY(BSTR* bstrLocationXY)
{
	// TODO: 在此添加实现代码
	CString cstrXY;
	//cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[3].D());
	cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_vPos[0].m_x,m_vPos[0].m_y,m_angleDetect);
	CComBSTR strLocationXY = cstrXY.AllocSysString();
	HRESULT hr = strLocationXY.CopyTo(bstrLocationXY);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
bool CHdpeAlgo::ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA)
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

STDMETHODIMP CHdpeAlgo::ReloadExternalFile(BSTR* bstrPath)
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
//
STDMETHODIMP CHdpeAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{

	return S_OK;
}

STDMETHODIMP CHdpeAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{

	return S_FALSE;
}
STDMETHODIMP CHdpeAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}
STDMETHODIMP CHdpeAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{

	return S_OK;
}
STDMETHODIMP CHdpeAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{

	return S_OK;
}
STDMETHODIMP CHdpeAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{

	return S_OK;
}

STDMETHODIMP CHdpeAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
//
void CHdpeAlgo::ResizeImage(Hobject Image, Hobject* ImageNew,HTuple withStand,HTuple Width,HTuple Height)
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

//STDMETHODIMP CHdpeAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
//{
//	/*利用边缘轮廓来定位圆*/
//	int id = (int)rgnId->fltVal;
//	double Radius = (double)pRadius->fltVal;
//	float MinScore = (float)pMinScore->fltVal;
//	Hobject newRgn;
//	m_vRgn[id] = newRgn;
//	Hlong Width, Height;
//	HTuple hv_Class, hv_Area1, hv_Row3, hv_Column3;
//	Hobject  ho_ModelImages1, ho_ModelRegions1,ho_RegionDilation1,ho_ConnectedRegions,ho_RegionFillUp,ho_RegionErosion,ho_SelectedRegions,ho_RegionUnion;
//	Hobject  Ellipse, ImageReduced,Circle;
//#ifdef NDEBUG
//	try
//	{
//#endif
//		//
//		if(m_bDebugImg)
//		{
//			m_dictSpecialData.clear();
//		}
//		Hobject Image = m_crtImg;
//		get_image_size (Image, &Width, &Height);
//		m_imgWidth  = (long)Width;
//		m_imgHeight = (long)Height;
//		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.2, Height/1.6);   //默认图片的大小有要求
//		reduce_domain (Image, Ellipse, &ImageReduced);
//		if (MinScore>120)
//		{
//			MinScore=120;
//
//		}
//		int LowContrast=(int)MinScore;
//		int HighContrast=(int)(MinScore*2);
//		inspect_shape_model(ImageReduced, &ho_ModelImages1, &ho_ModelRegions1,1,((HTuple(LowContrast).Append(HighContrast)).Append(25)));
//		//inspect_shape_model(ImageReduced, &ho_ModelImages1, &ho_ModelRegions1, 1,(int)MinScore);
//		dilation_circle(ho_ModelRegions1, &ho_RegionDilation1, 2.5);
//
//		connection(ho_RegionDilation1, &ho_ConnectedRegions);
//		fill_up(ho_ConnectedRegions, &ho_RegionFillUp);
//		erosion_circle(ho_RegionFillUp, &ho_RegionErosion, 2.5);
//		if (Radius<10)
//		{
//			gen_circle (&Circle,  Height/2.0, Width/2.0, Radius);
//			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
//			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
//			m_vRgn[id] = Circle;
//			return S_OK;
//		}
//		if (Radius<50)
//		{
//			Radius=50;
//		}
//		Hobject ho_RegionOpenning;
//		opening_circle (ho_RegionErosion, &ho_RegionOpenning, Radius-30);
//		double AreaMax,AreaMin;
//		AreaMin=(Radius-8)*(Radius-8)*3.1416;
//		AreaMax=(Radius+8)*(Radius+8)*3.1416;
//		select_shape(ho_RegionOpenning, &ho_SelectedRegions, "area", "and", AreaMin, AreaMax);//inner_radius
//		//*
//		HTuple hv_Number,hv_Number2,hv_Row,hv_Column,hv_Radius1,hv_Abs,hv_Indices;
//		count_obj(ho_SelectedRegions, &hv_Number);
//		count_obj(ho_RegionErosion, &hv_Number2);
//		if (0 != (hv_Number<1))
//		{
//			if (0 != (hv_Number2>0))
//			{
//				smallest_circle(ho_RegionErosion, &hv_Row, &hv_Column, &hv_Radius1);
//				tuple_abs(hv_Radius1-Radius, &hv_Abs);
//				tuple_sort_index(hv_Abs, &hv_Indices);
//				gen_circle(&ho_SelectedRegions, hv_Row.Select(hv_Indices.Select(0)), hv_Column.Select(hv_Indices.Select(0)), Radius);
//			}
//		}
//		//*
//		union1(ho_SelectedRegions, &ho_RegionUnion);
//		get_obj_class(ho_RegionUnion, &hv_Class);
//		if (0 != (hv_Class==HTuple("region")))
//		{
//			area_center(ho_RegionUnion, &hv_Area1, &hv_Row3, &hv_Column3);
//			gen_circle(&Circle, hv_Row3, hv_Column3, Radius);
//			m_vPos[0].m_y = (float)hv_Row3[0].D();
//			m_vPos[0].m_x = (float)hv_Column3[0].D();
//			m_vRgn[id] = Circle;
//		}
//		else
//		{
//			gen_circle (&Circle,  Height/2.0, Width/2.0, Radius);
//			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
//			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
//			m_vRgn[id] = Circle;
//			return S_FALSE;
//		}
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
//STDMETHODIMP CHdpeAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
//{
//	CComBSTR strHelp("621$\
//					 381;B$\
//					 374;LP;10;2000;1;220$\
//					 390;LP;1;255;1;40");
//
//	HRESULT hr = strHelp.CopyTo(pHelpStr);
//	if(FAILED(hr))
//		return S_FALSE;
//	return S_OK;
//}







STDMETHODIMP CHdpeAlgo::CenterLocationAlgoHS(VARIANT* rgnId,VARIANT* pDetectType, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pOutlierRemoverSize)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	int DetectType = (int)pDetectType->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	int BlackPointUpSize = (int)pBlackPointUpSize->fltVal;
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
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/1.8, Height/1.6);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		if(DetectType==1)
		{
			threshold(ImageReduced, &threshed,ThresholdLow, 255);
		}
		else
		{
			threshold(ImageReduced, &threshed,0, ThresholdLow);
		}
		//fill_up(threshed, &RegionFillUp);
		Hobject RegionFillUpSmall;
		if (SeriousBlackPointSize<1)
		{
			SeriousBlackPointSize=1;
		}
		if (BlackPointUpSize<1)
		{
			BlackPointUpSize=1;
		}
		if (BlackPointUpSize>99999999)
		{
			BlackPointUpSize=99999998;
		}
		fill_up_shape (threshed, &RegionFillUpSmall, "area", 1, SeriousBlackPointSize);
		fill_up_shape (RegionFillUpSmall, &RegionFillUp,"area", BlackPointUpSize, 99999999);
		union1(RegionFillUp, &RegionUnion);
		opening_circle(RegionUnion, &RegionOpening,OutlierRemoverSize+0.5);
		get_obj_class(RegionOpening, &hv_Class);
		if (0 != (hv_Class==HTuple("region")))
		{	
			smallest_circle(RegionOpening, &hv_Row3, &hv_Column3, &CenterRadius);
			if((CenterRadius<Radius*1.3)&(CenterRadius>Radius*0.7))
			{
				gen_circle(&Circle, hv_Row3, hv_Column3, Radius);
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
STDMETHODIMP CHdpeAlgo::CenterLocationAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("639$\
					 381;B$\
					 1314;LP;0;1;1;1$\
					 374;LP;10;2000;1;200$\
					 640;LP;1;255;1;50$\
					 416;LP;1;1200;1;300$\
					 677;LP;1;8000;1;2000$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//
STDMETHODIMP CHdpeAlgo::RectLocationAlgoHS(VARIANT* rgnId,VARIANT* pDetectType, VARIANT* pThresholdLow ,VARIANT* pOutlierRemoverSize)
{
	/*定位矩形区域*/
	int id = (int)rgnId->fltVal;
	int DetectType = (int)pDetectType->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, CenterRadius;
	HTuple hv_Row31, hv_Column31,hv_Row32, hv_Column32;
	Hobject threshed,RegionFillUp,RegionUnion,RegionOpening,SelectedRegions;
	Hobject  Ellipse, ImageReduced,RectRegion,ConnectedRegions;
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
			m_dictSpecialData.clear();
		}
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/1.8, Height/1.6);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		if(DetectType==1)
		{
			threshold(ImageReduced, &threshed,ThresholdLow, 255);
		}
		else
		{
			threshold(ImageReduced, &threshed,0, ThresholdLow);
		}
		//fill_up(threshed, &RegionFillUp);
		connection (threshed, &ConnectedRegions);
		select_shape_std (ConnectedRegions, &SelectedRegions, "max_area", 70);
		fill_up(SelectedRegions, &RegionFillUp);

		opening_rectangle1(RegionFillUp, &RegionOpening,OutlierRemoverSize+0.5,OutlierRemoverSize+0.5);
		get_obj_class(RegionOpening, &hv_Class);
		if (0 != (hv_Class==HTuple("region")))
		{	
			smallest_rectangle1(RegionOpening, &hv_Row31, &hv_Column31,&hv_Row32, &hv_Column32);
			gen_rectangle1(&RectRegion, hv_Row31, hv_Column31,hv_Row32, hv_Column32);
			m_vPos[0].m_y = (float)((hv_Row32+hv_Row31)/2.0)[0].D();
			m_vPos[0].m_x = (float)((hv_Column32+hv_Column31)/2.0)[0].D();
			m_vRgn[id] = RectRegion;
		}
		else
		{
			gen_rectangle1(&RectRegion, Height/3.0, Width/3.0,Height*2/3.0, Width*2/3.0);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = RectRegion;
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
STDMETHODIMP CHdpeAlgo::RectLocationAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1313$\
					 381;B$\
					 1314;LP;0;1;1;0$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//华康 有缺口盖子 中心定位，加闭运算 特殊处理。
STDMETHODIMP CHdpeAlgo::CenterLocationAlgoGapHS(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pClosingSize,VARIANT* pOutlierRemoverSize)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	int BlackPointUpSize = (int)pBlackPointUpSize->fltVal;
	int ClosingSize = (int)pClosingSize->fltVal;
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
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/1.8, Height/1.6);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		threshold(ImageReduced, &threshed,ThresholdLow, 255);
		//fill_up(threshed, &RegionFillUp);
		Hobject RegionFillUpSmall;
		if (SeriousBlackPointSize<1)
		{
			SeriousBlackPointSize=1;
		}
		if (BlackPointUpSize<1)
		{
			BlackPointUpSize=1;
		}
		if (BlackPointUpSize>99999999)
		{
			BlackPointUpSize=99999998;
		}
		fill_up_shape (threshed, &RegionFillUpSmall, "area", 1, SeriousBlackPointSize);
		fill_up_shape (RegionFillUpSmall, &RegionFillUp,"area", BlackPointUpSize, 99999999);
		union1(RegionFillUp, &RegionUnion);
		Hobject RegionOpening1,RegionClosing;
		//gap
		opening_circle(RegionUnion, &RegionOpening1,7);
		closing_circle(RegionOpening1, &RegionClosing,ClosingSize);
		//
		opening_circle(RegionClosing, &RegionOpening,OutlierRemoverSize+0.5);
		get_obj_class(RegionOpening, &hv_Class);
		if (0 != (hv_Class==HTuple("region")))
		{	
			smallest_circle(RegionOpening, &hv_Row3, &hv_Column3, &CenterRadius);
			if((CenterRadius<Radius*1.3)&(CenterRadius>Radius*0.7))
			{
				gen_circle(&Circle, hv_Row3, hv_Column3, Radius);
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
STDMETHODIMP CHdpeAlgo::CenterLocationAlgoGapHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("639$\
					 381;B$\
					 374;LP;10;2000;1;200$\
					 640;LP;1;255;1;50$\
					 416;LP;1;1200;1;300$\
					 677;LP;1;8000;1;2000$\
					 775;LP;1;200;1;30$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CHdpeAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	int MinScore = (int)pMinScore->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple Class, Area1, Row3, Column3;
	Hobject  ModelImages1, ModelRegions1,RegionDilation1,ConnectedRegions,RegionFillUp,RegionErosion,SelectedRegions,RegionUnion;
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
STDMETHODIMP CHdpeAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
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

//原始定位算子黑中找白
STDMETHODIMP CHdpeAlgo::ModelCreate(double Radius, HTuple * ModelID1)
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
STDMETHODIMP CHdpeAlgo::CenterLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
STDMETHODIMP CHdpeAlgo::CenterLocationAlgo1Help(BSTR* pHelpStr)
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

//新定位算子，找边缘的极性可以选择
STDMETHODIMP CHdpeAlgo::ModelCreate1(double Radius, HTuple * ModelID1)
{
	//产生一个标准圆创建一个形状模板
	if (Radius <= 0) 
	{
		return S_FALSE;
	}
	else
	{
		Hobject ContCircle, EmptyImage, SyntheticModelImage,Domain;
		HTuple  SizeSynthImage = 2* Radius + 10;

		gen_circle_contour_xld(&ContCircle, SizeSynthImage/2, SizeSynthImage/2, Radius, 0, 6.28318, "positive", 1);
		gen_image_const(&EmptyImage, "byte", SizeSynthImage, SizeSynthImage);
		get_domain (EmptyImage, &Domain);
		paint_region (Domain, EmptyImage, &EmptyImage, 128, "fill");
		paint_xld(ContCircle, EmptyImage, &SyntheticModelImage, 0);
		create_scaled_shape_model(SyntheticModelImage, "auto", 0, 0, 0, 0.95, 1.05, "auto","auto", "use_polarity", 40, "auto", ModelID1);
		return S_OK;
	}
}
STDMETHODIMP CHdpeAlgo::CenterLocationByMatchAlgo(VARIANT* rgnId, VARIANT* pDetectType,VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用形状模板来定位圆*/
	int id = (int)rgnId->fltVal;

	double DetectType = (double)pDetectType->fltVal;
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
		/*************判断模板是否需要重新绘制*************/
		if(!m_isFirst)
		{
			if((m_oldRadius != Radius)||(m_oldDetectType != DetectType))
			{
				clear_shape_model(m_modelId);
				m_isFirst = true;
			}
		}
		/*************************************************/
		if(m_isFirst)
		{
			if(DetectType==1)
			{
				ModelCreate (Radius, &m_modelId);
			}
			else
			{
				ModelCreate1 (Radius, &m_modelId);
			}
			m_isFirst = false;
			m_oldRadius = Radius;
			m_oldDetectType = DetectType;
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
STDMETHODIMP CHdpeAlgo::CenterLocationByMatchAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 1314;LP;0;1;1;0$\
					 374;LP;10;2000;1;192$\
					 377;FP;0.5;1.0;0.05;0.85");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//use edegs define center
STDMETHODIMP CHdpeAlgo::CenterLocationUseEdgesAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,  VARIANT* pThresholdHigh, VARIANT* pMinCircularity)
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
STDMETHODIMP CHdpeAlgo::CenterLocationUseEdgesAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CHdpeAlgo::MatchingParaAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
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
			}
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
STDMETHODIMP CHdpeAlgo::MatchingParaAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CHdpeAlgo::MatchingParaMinScoreAlgo(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
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
STDMETHODIMP CHdpeAlgo::MatchingParaMinScoreAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CHdpeAlgo::GapAngleDetectAlgo(VARIANT* rgnId,VARIANT* pDetectType,VARIANT* pBinThred,VARIANT* pEdgeDist, VARIANT* pErosionSize, VARIANT* pDetectAngle)
{
	int rId = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	float BlackThredLow = (float)pBinThred->fltVal;
	int   EdgeType  =(int)pDetectType->fltVal;
	float GapDistance =  (float)pEdgeDist->fltVal;
	float DefectSize =  (float)pErosionSize->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	//
	m_modelMatchingPARA[0]=0.0;
	m_modelMatchingPARA[1]=m_vPos[0].m_y;
	m_modelMatchingPARA[2]=m_vPos[0].m_x;
	m_modelMatchingPARA[3]=0.0;
	m_modelMatchingPARA[4]=0.0;
	m_modelMatchingPARA[5]=0.0;
	m_modelMatchingPARA[6]=0.0;
	//
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}

	if(Radius<=RoiWidth)
	{
		RoiWidth=(float)(Radius-1.0);
	}

	if(GapDistance<3)
	{
		GapDistance=3;
	}
	if(DefectSize<0.5)
	{
		DefectSize=0.5;
	}
	if(RoiWidth<3.0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		HTuple Width,Height;
		get_image_size(Image,&Width,&Height);
		///////////
		Hobject  ImageReduced,Region2,RegionClosing,RegionOpening1;
		//***************************************************************************************
		reduce_domain(Image, rgn, &ImageReduced);

		HTuple AngleDetect;
		AngleDetect = -1;
		m_angleDetect=1.57;
		// 
		if (EdgeType==-1)
		{
			threshold(ImageReduced, &Region2, 0, BlackThredLow);
		}
		else if (EdgeType==1)
		{
			threshold(ImageReduced, &Region2, BlackThredLow,255);
		}
		else if (EdgeType==0)
		{
			threshold(ImageReduced, &Region2, BlackThredLow,255);
			Hobject RegionBlack;
			threshold(ImageReduced, &RegionBlack, 0, BlackThredLow);
			concat_obj(RegionBlack,Region2,&Region2);
		}
		else
		{
			retValue = -1;
			m_vErrorRgn[rId] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}

		closing_circle(Region2, &RegionClosing, 3);
		opening_circle(RegionClosing, &RegionOpening1, DefectSize+1.5);
		//*
		Hobject  RegionClosing2,RegionDilation,RegionDilation1,ConnectedRegions1,SelectedRegions,ObjectSelected,RegionIntersection,ConnectedRegions;
		HTuple   Area1,Row1,Column1,Area2,Row2,Column2,Indices1;
		//该处理方式，相距为设定值得区域，最终还会分开。需要设定参数大于区域的距离。
		//closing_circle(RegionOpening1, &RegionClosing2, GapDistance);
		dilation_circle (RegionOpening1, &RegionDilation, GapDistance*1.12);
		erosion_circle (RegionDilation, &RegionClosing2, GapDistance-1.5);
		//
		dilation_circle(RegionClosing2, &RegionDilation1, 3.5);
		connection(RegionDilation1, &ConnectedRegions1);
		select_shape(ConnectedRegions1, &SelectedRegions, "max_diameter", "and", HTuple(3).Max2(GapDistance-20), 3+(GapDistance*2.0));
		area_center(SelectedRegions, &Area1, &Row1, &Column1);
		tuple_sort_index(Area1, &Indices1);
		if (0 != ((Indices1.Num())<1))
		{
			retValue = -1;
			m_vErrorRgn[rId] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		select_obj(SelectedRegions, &ObjectSelected, (Indices1.Select(0))+1);
		intersection(RegionOpening1, ObjectSelected, &RegionIntersection);
		//*
		connection(RegionIntersection, &ConnectedRegions);
		area_center(ConnectedRegions, &Area2, &Row2, &Column2);
		if (0 != ((Area2.Num())<1))
		{
			retValue = -1;
			m_vErrorRgn[rId] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		HTuple RowCenter,ColumnCenter,Angles;
		RowCenter = ((Row2+10).Sgn())*fCenterY;
		ColumnCenter = ((Column2+10).Sgn())*fCenterX;
		//***
		angle_ll(RowCenter, ColumnCenter, RowCenter, ((Column2+10).Sgn())*(Width-30), RowCenter, ColumnCenter, Row2, Column2, &Angles);
		HTuple Index1;
		for (Index1=0; Index1.Continue( (Angles.Num())-1, 1); Index1 += 1)
		{
			if (0 != ((Angles.Select(Index1))<0))
			{
				Angles.ReplaceElements(Index1,(Angles.Select(Index1))+(2*3.1415926));
			}
		}
		if (0 != ((Angles.Num())==1))
		{
			AngleDetect = Angles;
		}
		else if (0 != ((Angles.Num())==2))
		{
			if (0 != ((((Angles.Select(0))-(Angles.Select(1))).Abs())>3.2))
			{
				AngleDetect = (((Angles.Select(0))+(Angles.Select(1)))/2.0)+3.1416;
			}
			else
			{
				AngleDetect = ((Angles.Select(0))+(Angles.Select(1)))/2.0;
			}
		}
		///////////
		HTuple With_Point,Height_Point;
		Hobject RegionLines;
		With_Point = fCenterX+(180*(AngleDetect.Cos()));

		Height_Point = fCenterY-(180*(AngleDetect.Sin()));
		gen_region_line(&RegionLines, Height_Point,With_Point,  fCenterY, fCenterX);
		if (AngleDetect[0].D()<0)
		{
			AngleDetect[0]=AngleDetect[0].D()+6.283;
		}
		if (AngleDetect[0].D()>6.283)
		{
			AngleDetect[0]=AngleDetect[0].D()-6.283;
		}
		m_modelMatchingPARA[3]=AngleDetect[0];
		m_modelMatchingPARA[0] =Radius;
		if((m_modelMatchingPARA[3].D()>=0)&(m_modelMatchingPARA[3].D()<6.284))
		{
			retValue =180.0*(m_modelMatchingPARA[3].D()/(3.14159));
			//
			Hobject ShowRegion;
			concat_obj(RegionIntersection, RegionLines, &ShowRegion);
			m_vErrorRgn[rId] = ShowRegion;
			//
			concat_obj(rgn, ShowRegion, &rgn);
			m_vRgn[rId]=rgn;
			//
			m_angleDetect=m_modelMatchingPARA[3].D();
			//
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
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::GapAngleDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("926$\
					 397;R;FP;-10;460;1;>=#0$\
					 381;B$\
					 385;LP;-1;1;1;1$\
					 732;LP;1;254;1;20$\
					 927;LP;1;650;1;130$\
					 443;FP;0.5;20.5;0.5;1.0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CHdpeAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CHdpeAlgo::RectRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRgnWidth,VARIANT* pRgnHeight,VARIANT* pCenterMoveR,VARIANT* pCenterMoveC)
{
	int id           = (int)rgnId->fltVal;
	int RgnWidth     = (int)pRgnWidth->fltVal;
	int RgnHeight    = (int)pRgnHeight->fltVal;
	int CenterMoveR  = (int)pCenterMoveR->fltVal;
	int CenterMoveC  = (int)pCenterMoveC->fltVal;
	Hobject newRgn,Rect,ImgeDomain;
	m_vRgn[id] = newRgn;

#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		
		float CenterY = CenterMoveR+fCenterY;
		float CenterX = CenterMoveC+fCenterX;
		
		gen_rectangle1(&Rect, CenterY-RgnHeight/2, CenterX-RgnWidth/2, CenterY+RgnHeight/2, CenterX+RgnWidth/2);
		gen_rectangle1(&ImgeDomain, 0, 0,m_imgHeight,m_imgWidth);
		intersection(ImgeDomain,Rect,&Rect);

		m_vRgn[id] = Rect;

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
STDMETHODIMP CHdpeAlgo::RectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("898$\
					 381;B$\
					 940;LP;1;9999;1;100$\
					 941;LP;1;9999;1;30$\
					 324;LP;-2000;2000;1;1$\
					 323;LP;-2000;2000;1;1");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CHdpeAlgo::OcrAngleDetectAlgo(VARIANT* rgnId, VARIANT* pDetectAngle)
{
	int rId = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
	if(RoiWidth<11.0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		Hobject  Rectangle, Region1, RegionDilation,RegionDilation1, RegionIntersection,RegionDilation2,RectangleT,ConnectedRegions2,ObjectSelectedE,RegionIntersection1;
		Hobject  ConnectedRegions1, SelectedRegions1, RegionLines,RegionUnion,RegionUnion1;
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
		derivate_gauss(PolarTransImage, &DerivGauss, 2.5, "gradient");
		scale_image_max(DerivGauss, &ImageScaleMax);
		emphasize(ImageScaleMax, &ImageEmphasize1, 7, 7, 1.6);
		mean_image(ImageEmphasize1, &ImageMean, 3, 3);
		gen_rectangle2(&Rectangle, 0, 0, 0, Width1-1, Height1-1);
		gray_histo(Rectangle, ImageMean, &AbsoluteHisto, &RelativeHisto);
		Thred_Result = 0;  
		Thred_gray = 0;
		for (Index1=255; Index1>=0; Index1+=-1)
		{
			Thred_Result += HTuple(RelativeHisto[Index1]);
			if (0 != (Thred_Result>0.025))
			{
				Thred_gray = Index1;
				break;
			}
		}
		threshold(ImageMean, &Region1, Thred_gray, 255);
		dilation_circle(Region1, &RegionDilation, 3.5);	
		intersection(Rectangle, RegionDilation, &RegionIntersection);
		connection(RegionIntersection, &ConnectedRegions1);
		//*****************
		select_shape(ConnectedRegions1, &SelectedRegions, "area", "and", 100, 99999);
		union1(SelectedRegions, &RegionUnion1);

		select_shape_std(SelectedRegions, &SelectedRegions1, "max_area", 70);
		union1(SelectedRegions1, &RegionUnion);
		dilation_rectangle1(RegionUnion, &RegionDilation1, 41, 1);
		intersection(RegionUnion1, RegionDilation1, &RegionIntersection);
		smallest_rectangle1(RegionIntersection, &Row3, &Column3, &Row4, &Column4);
		if (0 != ((Column4.Num())<1))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		//*****************
		With_Center = (Column4[0].D()+Column3[0].D())/2.0;
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
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::OcrAngleDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("617$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::OcrAngleDetectAlgo1(VARIANT* rgnId,VARIANT* pAreaRatio, VARIANT* pDetectAngle)
{
	int rId = (int)rgnId->fltVal;
	float AreaRatio = (float)pAreaRatio->fltVal;
	
	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
	if(RoiWidth<11.0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
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
		Hobject  Rectangle, Region1, RegionDilation,RegionDilation1, RegionIntersection,RegionDilation2,RectangleT,ConnectedRegions2,ObjectSelectedE,RegionIntersection1;
		Hobject  ConnectedRegions1, SelectedRegions1, RegionLines,RegionUnion,RegionUnion1;
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
		derivate_gauss(PolarTransImage, &DerivGauss, 2.5, "gradient");
		scale_image_max(DerivGauss, &ImageScaleMax);
		emphasize(ImageScaleMax, &ImageEmphasize1, 7, 7, 1.6);
		mean_image(ImageEmphasize1, &ImageMean, 3, 3);
		gen_rectangle2(&Rectangle, 0, 0, 0, Width1-1, Height1-1);
		gray_histo(Rectangle, ImageMean, &AbsoluteHisto, &RelativeHisto);
		Thred_Result = 0;  
		Thred_gray = 0;
		for (Index1=255; Index1>=0; Index1+=-1)
		{
			Thred_Result += HTuple(RelativeHisto[Index1]);
			if (0 != (Thred_Result>AreaRatio))
			{
				Thred_gray = Index1;
				break;
			}
		}
		threshold(ImageMean, &Region1, Thred_gray, 255);
		dilation_circle(Region1, &RegionDilation, 3.5);	
		intersection(Rectangle, RegionDilation, &RegionIntersection);
		connection(RegionIntersection, &ConnectedRegions1);
		//*****************
		select_shape(ConnectedRegions1, &SelectedRegions, "area", "and", 100, 99999);
		union1(SelectedRegions, &RegionUnion1);

		select_shape_std(SelectedRegions, &SelectedRegions1, "max_area", 70);
		union1(SelectedRegions1, &RegionUnion);
		dilation_rectangle1(RegionUnion, &RegionDilation1, 41, 1);
		intersection(RegionUnion1, RegionDilation1, &RegionIntersection);
		smallest_rectangle1(RegionIntersection, &Row3, &Column3, &Row4, &Column4);
		if (0 != ((Column4.Num())<1))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		//*****************
		With_Center = (Column4[0].D()+Column3[0].D())/2.0;
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
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::OcrAngleDetectAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("617$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B$\
					 5024;FP;0.002;1;0.001;0.005");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::SectorRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
				Hobject  Rectangle1;
				// Local control variables 
				HTuple  Pointer, Type, WidthOri, HeightOri;
				Hobject Image = m_crtImg;
				get_image_pointer1(Image, &Pointer, &Type, &WidthOri, &HeightOri);
				gen_rectangle1(&Rectangle1, 0, 0, RoiWidth, WidthOri-1);
				polar_trans_region_inv(Rectangle1, &RegionDetect,fCenterY, fCenterX, m_angleDetect+(DriftAngle1/360.0)*6.283,m_angleDetect+(DriftAngle2/360.0)*6.283, m_RadiusInner,Radius,  WidthOri, RoiWidth+1, WidthOri, HeightOri, "nearest_neighbor");
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
STDMETHODIMP CHdpeAlgo::SectorRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("613$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 614;FP;-60;360.0;0.5;40$\
					 615;FP;-60;420.0;0.5;320"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CHdpeAlgo::DetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
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
		write_region(RegionDetect,"C://reg.reg");
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
STDMETHODIMP CHdpeAlgo::DetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("442$\
					 167;B$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 441;LP;1;2;1;1$\
					 267;SPR$\
					 232;SPR" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//
void Extract_Edges_Region_HuaTuo (Hobject Image, Hobject Region, HTuple ContrastBlack,Hobject *EdgesRegion)
{
	// Local iconic variables 
	Hobject  ImageReduced1, ImageClosing, RegionDynThresh;
	Hobject  RegionClosing, RegionOpening1, Rectangle1;
	Hobject  RegionUnion, RegionOpening2, RegionUnion1;
	Hobject  RegionUnion2;
	// Local control variables 
	HTuple  AreaCenter, RowCenter, ColumnCenter;
	HTuple  Area, Row, Column;

	gen_empty_obj(&(*EdgesRegion));
	reduce_domain(Image, Region, &ImageReduced1);
	gray_closing_rect(ImageReduced1, &ImageClosing, 3, 31);
	dyn_threshold(ImageReduced1, ImageClosing, &RegionDynThresh, ContrastBlack, "dark");
	closing_rectangle1(RegionDynThresh, &RegionClosing, 3, 5);
	opening_rectangle1(RegionClosing, &RegionOpening1, 5, 21);
	area_center(Region, &AreaCenter, &RowCenter, &ColumnCenter);
	area_center(RegionOpening1, &Area, &Row, &Column);
	if (0 != ((Area.Num())<1))
	{
		return;
	}
	if (0 != (Column<ColumnCenter))
	{
		gen_rectangle2(&Rectangle1, 280, 180, 1.37, 17, 1);
	}
	else
	{
		gen_rectangle2(&Rectangle1, 280, 543, -1.37, 17, 1);
	}
	union1(RegionClosing, &RegionUnion);
	opening_seg(RegionUnion, Rectangle1, &RegionOpening2);
	union1(RegionOpening2, &RegionUnion1);
	union2(RegionOpening1, RegionUnion1, &RegionUnion2);
	dilation_rectangle1(RegionUnion2, &(*EdgesRegion), 5, 7);
	//*尽量避免错误区域被当做边缘区域！
	Hobject  ConnectedRegions, SelectedRegions,SelectedRegions1;
	HTuple  Row1, Column1,Radius,Number, Area1, Row2, Column2,Sorted;
	inner_circle(Region, &Row1, &Column1, &Radius);
	connection((*EdgesRegion), &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, "height", "and", Radius*1.5,Radius*2.8);
	count_obj(SelectedRegions, &Number);
	if (0 != (Number<1))
	{
		gen_empty_obj(&(*EdgesRegion));
	}
	else if (0 != (Number==1))
	{
		area_center(SelectedRegions, &Area1, &Row2, &Column2);
		if (0 != (Column2<ColumnCenter))
		{
			select_shape(ConnectedRegions, &SelectedRegions1, "column", "and", 0, Column2+5);
			union1(SelectedRegions1, &(*EdgesRegion));
		}
		else if (0 != (Column2>ColumnCenter))
		{
			select_shape(ConnectedRegions, &SelectedRegions1, "column", "and", Column2-5, 99999);
			union1(SelectedRegions1, &(*EdgesRegion));
		}
		else
		{
			gen_empty_obj(&(*EdgesRegion));
		}
	}
	else if (0 != (Number==2))
	{
		area_center(SelectedRegions, &Area1, &Row2, &Column2);
		tuple_sort(Column2, &Sorted);
		select_shape(ConnectedRegions, &SelectedRegions1, "column", "and", (Sorted.Select(0))-5, (Sorted.Select(1))+5);
		union1(SelectedRegions1, &(*EdgesRegion));
	}
	else
	{
		gen_empty_obj(&(*EdgesRegion));
	}
	return;
}
STDMETHODIMP CHdpeAlgo::DetectRegionEdgesAlgo(VARIANT* rgnId,VARIANT* pSeriousBlackPointDynThresh,VARIANT* pErosionSize, BSTR* bstrRgnFile)
{
	int id = (int)rgnId->fltVal;
	int SeriousBlackPointDynThresh = (int)pSeriousBlackPointDynThresh->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
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

	CString sRgnPath1  = *bstrRgnFile;
	string strRgnPath1 = CT2A(sRgnPath1);
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

		if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];

		m_modelPARA = modelPARAID;
		//
		Hobject  RegionAffineTrans;
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
		//**********************************
		Hobject Image = m_crtImg;
		Hobject EdgesRegion,RegionAffineTransDilation;
		dilation_rectangle1(RegionAffineTrans,&RegionAffineTransDilation,21,1);
		Extract_Edges_Region_HuaTuo(Image, RegionAffineTransDilation,SeriousBlackPointDynThresh, &EdgesRegion);
		//
		if(ErosionSize>=0.5)
		{
			erosion_circle (EdgesRegion, &EdgesRegion,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(EdgesRegion, &EdgesRegion,abs(ErosionSize));
		}
		m_regEdgesArea=EdgesRegion;
		m_regProfileArea=RegionAffineTrans;
		//******************(检测区域不能为空！)
		HTuple Area, Row_test, Column_test;
		area_center(EdgesRegion, &Area, &Row_test, &Column_test);
		if (Area<1)
		{
			m_vRgn[id] = RegionAffineTrans ;	
		}
		else
		{
			m_vRgn[id] = EdgesRegion ;	
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
STDMETHODIMP CHdpeAlgo::DetectRegionEdgesAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("872$\
					 167;B$\
					 415;LP;1;250;1;15$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 267;SPR" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::DetectRegionRegularAlgo(VARIANT* rgnId,VARIANT* pRegMinWidth,VARIANT* pErosionSize)
{
	int id = (int)rgnId->fltVal;
	int RegMinWidth = (int)pRegMinWidth->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
	//
	Hobject newRgn;
	//
	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
#ifdef NDEBUG
	try
	{
#endif
		//**********************************
		Hobject RegionDetect,EdgesRegion,RegionDifference,ConnectedRegions,SelectedRegions,RegionIrregular,RegionRegular;
		HTuple  Area,Row,Column;
		EdgesRegion=m_regEdgesArea;
		difference(m_regProfileArea,EdgesRegion,&RegionDifference);
		connection (RegionDifference, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions, "area_holes", "and", 1, 999999);
		union1 (SelectedRegions, &RegionIrregular);
		difference(RegionDifference,RegionIrregular,&RegionRegular);
		//把比较窄的区域分到不规则区域内
		Hobject ConnectedRegionsForSmall,SelectedRegionsBig,RegionUnionBig;
		connection (RegionRegular, &ConnectedRegionsForSmall);
		select_shape (ConnectedRegionsForSmall,& SelectedRegionsBig, "inner_radius", "and", RegMinWidth/2.0+0.5, 99999);
		union1 (SelectedRegionsBig, &RegionUnionBig);
		difference (RegionRegular, RegionUnionBig, &m_regSmallRegular);
		RegionRegular=RegionUnionBig;
		//
		if(ErosionSize>=0.5)
		{
			erosion_circle (RegionRegular, &RegionRegular,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(RegionRegular, &RegionRegular,abs(ErosionSize));
		}
		area_center (RegionRegular,& Area, &Row, &Column);
		if (Area>3)
		{
			RegionDetect=RegionRegular;
		}
		else
		{
			if(ErosionSize>=0.5)
			{
				erosion_circle (RegionDifference, &RegionDifference,ErosionSize);
			}
			else if(ErosionSize<-0.5)
			{
				dilation_circle(RegionDifference, &RegionDifference,abs(ErosionSize));
			}
			RegionDetect=RegionDifference;
		}
		//******************

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
STDMETHODIMP CHdpeAlgo::DetectRegionRegularAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("873$\
					 167;B$\
					 845;LP;1;160;1;5$\
					 443;FP;-10.5;10.5;0.5;1.0" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::DetectRegionIrregularAlgo(VARIANT* rgnId,VARIANT* pErosionSize)
{
	int id = (int)rgnId->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
	//
	Hobject newRgn;
	//
	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
#ifdef NDEBUG
	try
	{
#endif
		//**********************************
		Hobject RegionDetect,EdgesRegion,RegionDifference,ConnectedRegions,SelectedRegions,RegionIrregular;
		HTuple  Area,Row,Column;
		EdgesRegion=m_regEdgesArea;
		difference(m_regProfileArea,EdgesRegion,&RegionDifference);
		connection (RegionDifference, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions, "area_holes", "and", 1, 999999);
		union1 (SelectedRegions, &RegionIrregular);
		//加上比较窄的规则区域
		union2(RegionIrregular,m_regSmallRegular,&RegionIrregular);
		if(ErosionSize>=0.5)
		{
			erosion_circle (RegionIrregular, &RegionIrregular,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(RegionIrregular, &RegionIrregular,abs(ErosionSize));
		}
		area_center (RegionIrregular,& Area, &Row, &Column);
		if (Area>3)
		{
			RegionDetect=RegionIrregular;
		}
		else
		{
			if(ErosionSize>=0.5)
			{
				erosion_circle (RegionDifference, &RegionDifference,ErosionSize);
			}
			else if(ErosionSize<-0.5)
			{
				dilation_circle(RegionDifference, &RegionDifference,abs(ErosionSize));
			}
			RegionDetect=RegionDifference;
		}
		//******************

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
STDMETHODIMP CHdpeAlgo::DetectRegionIrregularAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("874$\
					 167;B$\
					 443;FP;-10.5;10.5;0.5;1.0" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
} 
//
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
STDMETHODIMP CHdpeAlgo::DetectRegionLocationAlgoOCR(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
{
	int id            = (int)rgnId->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
	int RingOrOCR     = (int)pRingOrOCR->fltVal;
	//
	HTuple tmp;
	tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
	tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
	m_vRgnReadData[id] = tmp;
	//
	m_bOcrRegionFlag = true;
	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	if(RingOrOCR < 1 || RingOrOCR > 2)
	{
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
	string strRgnPath1,strRgnPath2;
	CString sRgnPath1  = *bstrRgnFile;
	strRgnPath1 = CT2A(sRgnPath1);
#ifdef NDEBUG
	try
	{
#endif
		if (m_bPilotImg)
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
					MessageBox(NULL, _T("Initialization parameter（detect region path）setting is wrong!"),_T("Initial configuration is wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}
			HTuple tmp;
			tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
			tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
			m_vRgnReadData[id] = tmp;
			//
			string strModelPath, strModelName;
			ParseModelPath(strRgnPath1,strModelPath, strModelName);

			string strModelImg = strModelPath + "\\" + strModelName + ".tiff";
			if (!_access(strModelImg.c_str(),0))
			{
				read_image(&m_modelImageOCR,strModelImg.c_str());
			}
			else
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL, _T("初始化参数（模板图片路径）设置错误"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL, _T("Initialization parameter（model image path）setting is wrong!"),_T("Initial configuration is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}
			string strModelDat = strModelPath + "\\" + strModelName + ".dat";
			if(!ReadShapeModelAxieData(strModelDat, "" , &m_modelPARA))
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("初始化参数,模板图像对应的‘dat’文件不存在"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Initialization parameter, 'dat’file corresponding to model image isn't exist!"),_T("Initialization error："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}

			m_modelCenterRow  = m_modelPARA[1];
			m_modelCenterCol  = m_modelPARA[2];

			if(RingOrOCR==2)
			{
				CString sRgnPath2  = *bstrExtractRgnFile;
				strRgnPath2 = CT2A(sRgnPath2);
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
					m_vRgn[id] = newRgn;
					return S_FALSE;
				}
			}
		}
		//
		HTuple dataLength;
		tuple_length (m_modelMatchingPARA, &dataLength);
		if(dataLength<3)
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('DetectRegionLocationAlgoOCR')前,请先检查匹配函数('MatchingParaAlgo')是否正常."),_T("函数异常提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the function('MatchingParaAlgo') is correct or not,before calling location function('DetectRegionLocationAlgoOCR')."), _T("Function error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn;
			return S_FALSE;
		}
		//
		if(	m_modelMatchingPARA[0].D()<1.0)
		{
			m_vRgn[id] = newRgn;
			return S_FALSE;
		}
		Hobject RegionDetect;

		if(RingOrOCR==1)
		{
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn ;
				return S_FALSE;
			}
			Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
			HTuple modelPARAID = m_vRgnReadData[id];
			HTuple ImageClass;
			get_obj_class(m_modelImageOCR, &ImageClass);
			if(ImageClass!=HTuple("image"))
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL, _T("初始化参数（模板图片路径）设置错误"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL, _T("Initialization parameter（model image path）setting is wrong!"),_T("Initial configuration is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}
			////
			//HTuple modelPARAIDLength;
			//tuple_length(modelPARAID,&modelPARAIDLength);
			//if(modelPARAIDLength[0].I()<6)
			//{
			//	MessageBox(NULL,_T("初始化参数（模板路径与检测区域路径）或区域编号设置错误"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			//	m_vRgn[id] = newRgn ;
			//	return S_FALSE;
			//}
			m_modelPARA=modelPARAID;
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
			HTuple imageHomMat2D;
			vector_angle_to_rigid(m_modelCenterRow,m_modelCenterCol,0.0,m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&imageHomMat2D);
			affine_trans_image (m_modelImageOCR,&m_modelImageTransOCR, imageHomMat2D,"bilinear","false");
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
				RegionDetect=RegionAffineTrans;
			}
		}
		else if (RingOrOCR==2)
		{
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn ;
				return S_FALSE;
			}
			Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
			HTuple modelPARAID=m_vRgnReadData[id];
			//
			HTuple ImageClass;
			get_obj_class(m_modelImageOCR, &ImageClass);
			if(ImageClass!=HTuple("image"))
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL, _T("初始化参数（模板图片路径）设置错误"),_T("初始配置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL, _T("Initialization parameter（model image path）setting is wrong!"),_T("Initial configuration is wrong："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_vRgn[id] = newRgn;
				return S_FALSE;
				//
			}
			HTuple modelPARAExtract=m_vRgnReadData[id];
			//HTuple modelPARAIDLength,modelPARAExtractLength;
			//tuple_length(modelPARAID,&modelPARAIDLength);
			//tuple_length(modelPARAExtract,&modelPARAExtractLength);
			//if((modelPARAExtractLength[0].I()<6)|(modelPARAIDLength[0].I()<6))
			//{
			//	MessageBox(NULL,_T("仅同时存在图案与拉环两个模板时‘盖面图案’参数可选择('2')"),_T("参数设置错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			//	m_vRgn[id] = newRgn ;
			//	return S_FALSE;
			//}
			m_modelPARA[0]=modelPARAID[0];
			m_modelPARA[1]=modelPARAID[1];
			m_modelPARA[2]=modelPARAID[2];
			m_modelPARA[3]=modelPARAID[3];
			m_modelPARA[4]=modelPARAExtract[4];
			m_modelPARA[5]=modelPARAExtract[5];
			m_modelPARA[6]=modelPARAExtract[6];
			//
			Hobject  RegionAffineTrans,OCRRegionAffineTrans,RegionDiff;
			//HTuple ringHomMat2D,OCRHomMat2D;
			//vector_angle_to_rigid(m_modelPARA[1].D(),m_modelPARA[2].D(),m_modelPARA[3].D(),m_modelMatchingPARA[4].D(),m_modelMatchingPARA[5].D(),m_modelMatchingPARA[6].D(),&OCRHomMat2D);
			//vector_angle_to_rigid(m_modelPARA[4].D(),m_modelPARA[5].D(),m_modelPARA[6].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&ringHomMat2D);
			//affine_trans_region(roiRgn,&RegionAffineTrans,OCRHomMat2D,"false");
			//
			HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted;
			vector_angle_to_rigid(0, 0, m_modelPARA[3].D(), 0, 0, m_modelMatchingPARA[6].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

			Hobject rotatedRgn,RegionMoved1;	
			HTuple RowModelData,ColModelData;
			RowModelData=m_modelPARA[1];
			ColModelData=m_modelPARA[2];
			move_region (roiRgn, &RegionMoved1,-RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_modelMatchingPARA[4];
			ColModelData=m_modelMatchingPARA[5];
			move_region (rotatedRgn,&RegionAffineTrans,RowModelData,ColModelData) ;

			//
			//
			HTuple imageHomMat2D;
			vector_angle_to_rigid(m_modelCenterRow,m_modelCenterCol,0.0,m_modelMatchingPARA[4].D(),m_modelMatchingPARA[5].D(),m_modelMatchingPARA[6].D(),&imageHomMat2D);
			affine_trans_image (m_modelImageOCR,&m_modelImageTransOCR, imageHomMat2D,"bilinear","false");
			//
			CString sRgnPath2  = *bstrExtractRgnFile;
			strRgnPath2 = CT2A(sRgnPath2);
			if(m_dictCrtPathRgn.find(strRgnPath2) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn ;
				return S_FALSE;
			}
			Hobject extractRgn = m_dictCrtPathRgn[strRgnPath2];
			//affine_trans_region(extractRgn,&OCRRegionAffineTrans,ringHomMat2D,"false");
			//
			vector_angle_to_rigid(0, 0, m_modelPARA[6].D(), 0, 0, m_modelMatchingPARA[3].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
			RowModelData=m_modelPARA[4];
			ColModelData=m_modelPARA[5];
			move_region (extractRgn, &RegionMoved1,-RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_modelMatchingPARA[1];
			ColModelData=m_modelMatchingPARA[2];
			move_region (rotatedRgn,&OCRRegionAffineTrans,RowModelData,ColModelData) ;
			//
			difference (RegionAffineTrans, OCRRegionAffineTrans, &RegionDiff);
			RegionAffineTrans=RegionDiff;
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
STDMETHODIMP CHdpeAlgo::DetectRegionLocationAlgoOCRHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("442$\
					 167;B$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 441;LP;1;2;1;1$\
					 267;SPR$\
					 232;SPR");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CHdpeAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
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
STDMETHODIMP CHdpeAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("380$\
					 397;R;LP;0;9000;1;<=#20$\
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

STDMETHODIMP CHdpeAlgo::DynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("411$\
					 397;R;LP;0;9000;1;<=#20$\
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
STDMETHODIMP CHdpeAlgo::DynThresholdAlgoOCR(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	float ErosionSize=(float)pErosionSize->fltVal;
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(m_bOcrRegionFlag!=true)
	{
		if(m_isChineseLanguage)
		{
			MessageBox(NULL,_T("调用图案检测函数('DynThresholdAlgoOCR')前需先调用匹配函数('DetectRegionLocationAlgoOCR')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Please check whether the function('DetectRegionLocationAlgoOCR') is correct or not,before calling location function('DynThresholdAlgoOCR')."), _T("Function error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
	}
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
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
		Hobject ImageReduced;

		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		HTuple ImageClass;
		get_obj_class(m_modelImageTransOCR, &ImageClass);
		if(ImageClass!=HTuple("image"))
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("调用图案检测函数('DynThresholdAlgoOCR')前需先调用匹配函数('DetectRegionLocationAlgoOCR')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("Please check whether the function('DetectRegionLocationAlgoOCR') is correct or not,before calling location function('DynThresholdAlgoOCR')."), _T("Function error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			return S_FALSE;
			//
		}
		//
		reduce_domain (Image, rgn, &ImageReduced);
		ImageProcessAlg.ErrorDetectOCR(m_modelImageTransOCR,ImageReduced, SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		if(ErosionSize>=0.5)
		{
			erosion_circle (BlackUnion, &BlackUnion,ErosionSize);
			dilation_circle (BlackUnion, &BlackUnion,ErosionSize);
		}
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		ImageProcessAlg.ErrorDetectOCR(m_modelImageTransOCR,ImageReduced, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		if(ErosionSize>=0.5)
		{
			erosion_circle (WhiteUnion, &WhiteUnion,ErosionSize);
			dilation_circle (WhiteUnion, &WhiteUnion,ErosionSize);
		}
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[rId].push_back(float(SeriousWhitePointDynThresh));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageOneChannel,ImageOneChannelModel,ImageSub;
			get_image_size (Image,& Width, &Height);
			HTuple Channels;
			count_channels (ImageReduced, &Channels);
			if(Channels>1)
				access_channel (ImageReduced, &ImageOneChannel, Channels);
			else
				ImageOneChannel=ImageReduced;
			//
			count_channels (m_modelImageTransOCR, &Channels);
			if(Channels>1)
				access_channel (m_modelImageTransOCR, &ImageOneChannelModel, Channels);
			else
				ImageOneChannelModel=m_modelImageTransOCR;
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
STDMETHODIMP CHdpeAlgo::DynThresholdAlgoOCRHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("496$\
					 397;R;LP;0;9000;1;<=#20$\
					 381;B$\
					 415;LP;1;250;1;50$\
					 416;LP;1;200;1;10$\
					 420;LP;1;250;1;60$\
					 421;LP;1;200;1;10$\
					 501;FP;0;10.5;0.5;1.5");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CHdpeAlgo::BubbleDetectProfileAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::BubbleDetectProfileAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("646$\
					 397;R;LP;0;1000;1;<=#20$\
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
STDMETHODIMP CHdpeAlgo::BlackDetectExcludeEdgesAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize, VARIANT* pThresholdLow,  VARIANT* pMinEdgeSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::BlackDetectExcludeEdgesAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("635$\
					 397;R;LP;0;1000;1;<=#20$\
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
STDMETHODIMP CHdpeAlgo::BlackDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::BlackDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("492$\
					 397;R;LP;0;1000;1;<=#20$\
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

STDMETHODIMP CHdpeAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
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
STDMETHODIMP CHdpeAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1;<=#20$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::UVCurlAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::UVCurlAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("607$\
					 397;R;LP;0;1000;1;<=#20$\
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
STDMETHODIMP CHdpeAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("609$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 610;FP;0.5;186.0;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CHdpeAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CHdpeAlgo::LineDetectionAlgo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize,VARIANT* pDefectArea)
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
STDMETHODIMP CHdpeAlgo::LineDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("425$\
					 397;R;FP;0;1000;4;<=#20$\
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
STDMETHODIMP CHdpeAlgo::LineDetectionExclude90Algo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize,VARIANT* pDefectArea)
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
STDMETHODIMP CHdpeAlgo::LineDetectionExclude90AlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("856$\
					 397;R;FP;0;1000;4;<=#20$\
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

STDMETHODIMP CHdpeAlgo::TabDetectProfileAlgo(VARIANT* rgnId,  VARIANT* pThredBackground,  VARIANT* pThredForeground,VARIANT* pAnglePointSize,VARIANT* pMinAngle, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::TabDetectProfileAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("647$\
					 397;R;LP;0;1000;1;<=#20$\
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

STDMETHODIMP CHdpeAlgo::BlackDetectUseUpSizeAlgo(VARIANT* rgnId, VARIANT* pBlackThresh,VARIANT* pThredForeground,VARIANT* pWhiteSize,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::BlackDetectUseUpSizeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("676$\
					 397;R;LP;0;1000;1;<=#20$\
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

STDMETHODIMP CHdpeAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
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
//
void ocr_segment_yaoBo (Halcon::Hobject Image, Halcon::Hobject Region, Halcon::Hobject *OcrRegion, Halcon::HTuple ContrsatMin, Halcon::HTuple Contrast, Halcon::HTuple MinSize, Halcon::HTuple RowDilationSize, Halcon::HTuple ColDilationSize, Halcon::HTuple OcrHeight, Halcon::HTuple OcrWidth, Halcon::HTuple *SegmentNumber)
{
	// Local iconic variables 
	Hobject  ImageReduced1, RegionAffineTransOCR;
	Hobject  ModelImages, RegionDilation, ConnectedRegions;
	Hobject  SelectedRegions;

	reduce_domain(Image, Region, &ImageReduced1);
	//derivate_gauss (ImageReduced1, DerivGauss, 1.5, 'gradient')
	//scale_image_max (DerivGauss, ImageScaleMax)
	//* emphasize (ImageScaleMax, ImageEmphasize, 7, 7, 1.6)
	//mean_image (ImageEmphasize, ImageMean, 3, 3)
	//gray_erosion_rect (ImageMean, ImageMin, 11, 15)
	//dyn_threshold (ImageMean, ImageMin, RegionAffineTransOCR, 150, 'light')
	//*
	inspect_shape_model(ImageReduced1, &ModelImages, &RegionAffineTransOCR, 1, (ContrsatMin.Concat(Contrast)).Concat(MinSize));
	//*
	dilation_rectangle1(RegionAffineTransOCR, &RegionDilation, ColDilationSize, RowDilationSize);
	connection(RegionDilation, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, (HTuple("width").Append("height")), "and", (HTuple(3).Append(20)), (OcrWidth+4).Concat(OcrHeight+21));
	sort_region(SelectedRegions, &ConnectedRegions, "upper_left", "true", "column");
	fill_up_shape(ConnectedRegions, &(*OcrRegion), "area", 1, 100);
	count_obj((*OcrRegion), &(*SegmentNumber));
}
STDMETHODIMP CHdpeAlgo::CreateOcrModelAlgo(VARIANT* rgnId,VARIANT* pMaxNumber,VARIANT* pContrastMin,VARIANT* pContrast, VARIANT* pEdgeSize,VARIANT* pDilationRow, VARIANT* pDilationCol,VARIANT* pOcrHeight, VARIANT* pOcrWidth,VARIANT*pSampleNumber )
{
	int rId = (int)rgnId->fltVal;
	int MaxNumber = (int)pMaxNumber->fltVal;
	int contrastMin   = (int)pContrastMin->fltVal;
	int contrast      = (int)pContrast->fltVal;
	if (contrastMin<=0)
	{
		contrastMin=4;
	}
	if (contrastMin>220)
	{
		contrastMin=220;
	}
	if (contrast<=0)
	{
		contrastMin=20;
	}
	if (contrast>240)
	{
		contrast=240;
	}
	if (contrastMin>=contrast)
	{
		int mid=contrastMin;
		contrastMin=contrast;
		contrast=mid+1;
	}
	int edgeSize      = (int)pEdgeSize->fltVal;
	if (edgeSize<=0)
	{
		edgeSize=2;
	}
	float dilationRow =(float)pDilationRow->fltVal;
	if (dilationRow<=1.0)
	{
		dilationRow=1.0;
	}
	float dilationCol =(float)pDilationCol->fltVal;
	if (dilationCol<=1.0)
	{
		dilationCol=1.0;
	}
	int ocrHeight     = (int)pOcrHeight->fltVal;
	if (ocrHeight<=8)
	{
		ocrHeight=8;
	}
	int ocrWidth      = (int)pOcrWidth->fltVal;
	if (ocrWidth<=2)
	{
		ocrWidth=2;
	}
	//
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pSampleNumber);
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
		if(m_bPilotImg)
		{
			m_ocrNumber=0;
		}
		Hobject Image = m_crtImg;
		//create models
		Herror ret ;
		HTuple Width1, Height1,Area11,row11,col11;
		Hobject ImageAffinTrans,RegionAffineTrans,RegionFillUp;
		Hobject RegionOcr,ShowOcrReg;
		HTuple Area1, RowOcrCenter,ColumnOcrCenter,Angle2,HomMat2D,HomMat2DInv,Number;
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		get_image_size(Image, &Width1, &Height1);
		area_center(rgn, &Area1, &RowOcrCenter, &ColumnOcrCenter);
		angle_lx(fCenterY, fCenterX, RowOcrCenter, ColumnOcrCenter,  &Angle2);
		vector_angle_to_rigid(fCenterY, fCenterX, 0, Height1/2.0, Width1/2.0, (-Angle2)-(HTuple(90).Rad()), &HomMat2D);
		affine_trans_image(Image, &ImageAffinTrans, HomMat2D, "bilinear", "false");
		affine_trans_region(rgn, &RegionAffineTrans, HomMat2D, "nearest_neighbor");
		ocr_segment_yaoBo(ImageAffinTrans, RegionAffineTrans, &RegionFillUp, contrastMin, contrast, edgeSize, dilationRow, dilationCol, ocrWidth, ocrHeight, &Number);
		area_center(RegionFillUp,&Area11,&row11,&col11);
		if(Area11<20)
		{
			retValue=-1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pSampleNumber);
			return S_FALSE;
		}
		
		if(Number>0)
		{
			vector_angle_to_rigid(Height1/2.0, Width1/2.0, (-Angle2)-(HTuple(90).Rad()),fCenterY, fCenterX, 0,  &HomMat2DInv);
			union1(RegionFillUp,&RegionOcr);
			affine_trans_region(RegionOcr, &ShowOcrReg, HomMat2DInv, "nearest_neighbor");
			m_vRgn[rId] = ShowOcrReg;
		}
		if((!m_bDebugImg)&(!m_bPilotImg)&(m_firstUse)&(m_ocrNumber<=MaxNumber))
		{
			string strModelFolderPath1,strTempImgFolderPath1,strModelFolderPath2,strTempImgFolderPath2;
			HTuple  Index,ImageFolder1, ModelFolder1,ImageFolder2, ModelFolder2,ImageFiles1,ModelFiles1,ImageFiles2,ModelFiles2;

			if (0 ==m_ocrNumber)
			{
				//无文件夹时首先创建文件夹
				HTuple ChannelFolder,TaskFolder;
				ChannelFolder=HTuple(m_strChannelPath.c_str());
				if (_access(ChannelFolder[0].S(), 0))
				{
					CString  FolderDir( ChannelFolder[0].S());
					CreateDirectory(FolderDir, NULL);
				}
				//
				TaskFolder=HTuple(m_strTaskPath.c_str());
				if (_access(TaskFolder[0].S(), 0))
				{
					CString  FolderDir( TaskFolder[0].S());
					CreateDirectory(FolderDir, NULL);
				}

				m_ocrModelIds1 = HTuple();
				strModelFolderPath1=m_strTaskPath+"\\Models1";
				strTempImgFolderPath1=m_strTaskPath+ "\\TemplateImage1" ;	
				ImageFolder1 = HTuple(strTempImgFolderPath1.c_str());
				ModelFolder1 = HTuple(strModelFolderPath1.c_str());
				m_imageFolder1=ImageFolder1;
				m_modelFolder1=ModelFolder1;
				if (_access(ImageFolder1[0].S(), 0))
				{
					CString  ImageDir( ImageFolder1[0].S());
					CreateDirectory(ImageDir, NULL);
				}

				if (_access(ModelFolder1[0].S(), 0))
				{
					CString  ModelDir( ModelFolder1[0].S());
					CreateDirectory(ModelDir, NULL);
				}

				//first delete the exist templates
				list_files(ImageFolder1, (HTuple("files").Append("follow_links")), &ImageFiles1);
				tuple_regexp_select(ImageFiles1, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ImageFiles1);
				HTuple Length_pre  = ImageFiles1.Num();
				if (Length_pre > 0 )
				{
					for (Index=0; Index <= (Length_pre -1); Index+=1)
						delete_file(HTuple(ImageFiles1[Index]));
				}

				list_files(ModelFolder1, (HTuple("files").Append("follow_links")), &ModelFiles1);
				tuple_regexp_select(ModelFiles1, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ModelFiles1);
				HTuple Length_Model  = ModelFiles1.Num();
				if (Length_Model > 0 )
				{
					for (Index=0; Index <= (Length_Model -1); Index+=1)
						delete_file(HTuple(ModelFiles1[Index]));
				}

				if(MaxNumber>9)
				{
					m_ocrModelIds2 = HTuple();
					strModelFolderPath2=m_strTaskPath+"\\Models2\\";
					strTempImgFolderPath2=m_strTaskPath+ "\\TemplateImage2" ;	
					ImageFolder2 = HTuple(strTempImgFolderPath2.c_str());
					ModelFolder2 = HTuple(strModelFolderPath2.c_str());
					m_imageFolder2=ImageFolder2;
					m_modelFolder2=ModelFolder2;
					if (_access(ImageFolder2[0].S(), 0))
					{
						CString  ImageDir( ImageFolder2[0].S());
						CreateDirectory(ImageDir, NULL);
					}

					if (_access(ModelFolder2[0].S(), 0))
					{
						CString  ModelDir( ModelFolder2[0].S());
						CreateDirectory(ModelDir, NULL);
					}

					//first delete the exist templates
					list_files(ImageFolder2, (HTuple("files").Append("follow_links")), &ImageFiles2);
					tuple_regexp_select(ImageFiles2, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ImageFiles2);
					Length_pre  = ImageFiles2.Num();
					if (Length_pre > 0 )
					{
						for (Index=0; Index <= (Length_pre -1); Index+=1)
							delete_file(HTuple(ImageFiles2[Index]));
					}

					list_files(ModelFolder2, (HTuple("files").Append("follow_links")), &ModelFiles2);
					tuple_regexp_select(ModelFiles2, (HTuple("\\.(shm|tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 	&ModelFiles2);
					Length_Model  = ModelFiles2.Num();
					if (Length_Model > 0 )
					{
						for (Index=0; Index <= (Length_Model -1); Index+=1)
							delete_file(HTuple(ModelFiles2[Index]));
					}
					//write_shape_model(ModelID, ModelFolder1+"\\000.shm");
					//write_image(OriginImg,"tiff",0, ImageFolder1+"\\000.tiff");
				}
			}
			//*
			HTuple numberHtuple;
			if (Number>0)
			{
				Hobject ObjectSelected,ImageReducedModel;
				HTuple ModelID;
				select_obj(RegionFillUp, &ObjectSelected, 1);
				reduce_domain(ImageAffinTrans, ObjectSelected, &ImageReducedModel);
				
				ret = create_shape_model(ImageReducedModel, "auto", HTuple(-20).Rad(), HTuple(40).Rad(),  "auto", "auto", "use_polarity", 20, "auto", &ModelID);
				m_ocrModelIds1 = m_ocrModelIds1.Concat(ModelID);
				m_ocrNumbers1 = m_ocrNumbers1.Concat(m_ocrNumber+1);
				numberHtuple=m_ocrNumber;
				write_shape_model(ModelID, m_modelFolder1+"\\"+ HTuple(numberHtuple.ToString(".02")+".shm"));
				write_image(ImageAffinTrans,"tiff",0, m_imageFolder1+ "\\"+HTuple(numberHtuple.ToString(".02")+".tiff"));
			}
			if (Number==2 && MaxNumber>9)
			{
				Hobject ObjectSelected,ImageReducedModel;
				HTuple ModelID;
				select_obj(RegionFillUp, &ObjectSelected, 2);
				reduce_domain(ImageAffinTrans, ObjectSelected, &ImageReducedModel);
				ret = create_shape_model(ImageReducedModel, "auto", HTuple(-20).Rad(), HTuple(40).Rad(),  "auto", "auto", "use_polarity", 20, "auto", &ModelID);
				m_ocrModelIds2 = m_ocrModelIds2.Concat(ModelID);
				m_ocrNumbers2 = m_ocrNumbers2.Concat(m_ocrNumber+1);
				numberHtuple=m_ocrNumber;
				write_shape_model(ModelID, m_modelFolder2+"\\"+ HTuple(numberHtuple.ToString(".02")+".shm"));
				write_image(ImageAffinTrans,"tiff",0, m_imageFolder2+ "\\"+HTuple(numberHtuple.ToString(".02")+".tiff"));
			}
			if (0 != (Number>0))
			{
				m_ocrNumber=m_ocrNumber+1;
			}
		}

		retValue = m_ocrNumber;
		m_vErrorRgn[rId] = ShowOcrReg;
		retValue.Detach(pSampleNumber);
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pSampleNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::CreateOcrModelAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("869$\
					 200;R;LP;1;255;1;>=#1$\
					 167;B$\
					 1310;LP;1;50;1;9$\
					 762;LP;1;120;1;10$\
					 763;LP;5;250;1;20$\
					 744;LP;1;400;1;6$\
					 524;FP;0.5;50.0;0.5;13.0$\
					 523;FP;0.5;40.0;0.5;5.0$\
					 844;LP;5;300;1;90$\
					 845;LP;2;200;1;50");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

bool ReadDatasForDirection( string bstrDataPath,int MaxNumber,bool errorInfoLanguage,HTuple& ModelNum1,HTuple& ModelNum2,HTuple& Models1,HTuple& Models2)
{

	//
	ModelNum1 = HTuple();
	Models1 = HTuple();
	ModelNum2 = HTuple();
	Models2 = HTuple();
	//
	string strModelFolderPath1,strModelFolderPath2;
	strModelFolderPath1=bstrDataPath+"\\Models1\\";
	//读取模板组1（先判断模板文件夹是否存在）
	if(_access(strModelFolderPath1.c_str(),0) )
	{
		if(errorInfoLanguage)
		{
			MessageBox(NULL,_T("模板文件夹1读取错误"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Model Folder one Reading Error"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		return 0;
	}
	else
	{
		HTuple  ModelID;
		HTuple  ModelFiles;
		list_files(strModelFolderPath1.c_str(), (HTuple("files").Append("follow_links")), &ModelFiles);
		tuple_regexp_select(ModelFiles, (HTuple("\\.(shm)$").Append("ignore_case")), &ModelFiles);
		ModelNum1=HTuple(ModelFiles.Num());
		if(ModelNum1<1)
		{
			if(errorInfoLanguage)
			{
				MessageBox(NULL,_T("模板1不存在"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("No Model One"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			return 0;
		}
		for (int Index=0; Index <= ModelFiles.Num() -1; Index+=1)
		{
			read_shape_model(HTuple(ModelFiles[Index]), &ModelID);
			Models1.Append(ModelID);
		}
	}
	
	if(MaxNumber>9)
	{
		//读取模板组2（先判断模板文件夹是否存在）
		strModelFolderPath2=bstrDataPath+"\\Models2\\";
		if(_access(strModelFolderPath2.c_str(),0) )
		{
			if(errorInfoLanguage)
			{
				MessageBox(NULL,_T("模板文件夹2读取错误"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("Model Folder two Reading Error"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			return 0;
		}
		else
		{
			HTuple  ModelID;
			HTuple  ModelFiles;
			list_files(strModelFolderPath2.c_str(), (HTuple("files").Append("follow_links")), &ModelFiles);
			tuple_regexp_select(ModelFiles, (HTuple("\\.(shm)$").Append("ignore_case")), &ModelFiles);
			ModelNum2=HTuple(ModelFiles.Num());
			if(ModelNum2<1)
			{
				if(errorInfoLanguage)
				{
					MessageBox(NULL,_T("模板2不存在"),_T("读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("No Model Two"),_T("Read error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				return 0;
			}
			for (int Index=0; Index <= ModelFiles.Num() -1; Index+=1)
			{
				read_shape_model(HTuple(ModelFiles[Index]), &ModelID);
				Models2.Append(ModelID);
			}
		}
	}
	return 1;
}
STDMETHODIMP CHdpeAlgo::FindOcrNumberAlgo(VARIANT* rgnId,VARIANT* pMaxNumber,VARIANT* pContrastMin,VARIANT* pContrast, VARIANT* pEdgeSize,VARIANT* pDilationRow, VARIANT* pDilationCol,VARIANT* pOcrHeight, VARIANT* pOcrWidth,VARIANT*pOcrNumber )
{
	int rId = (int)rgnId->fltVal;
	int MaxNumber = (int)pMaxNumber->fltVal;
	int contrastMin   = (int)pContrastMin->fltVal;
	int contrast      = (int)pContrast->fltVal;
	if (contrastMin<=0)
	{
		contrastMin=4;
	}
	if (contrastMin>220)
	{
		contrastMin=220;
	}
	if (contrast<=0)
	{
		contrastMin=20;
	}
	if (contrast>240)
	{
		contrast=240;
	}
	if (contrastMin>=contrast)
	{
		int mid=contrastMin;
		contrastMin=contrast;
		contrast=mid+1;
	}
	int edgeSize      = (int)pEdgeSize->fltVal;
	if (edgeSize<=0)
	{
		edgeSize=2;
	}
	float dilationRow =(float)pDilationRow->fltVal;
	if (dilationRow<=1.0)
	{
		dilationRow=1.0;
	}
	float dilationCol =(float)pDilationCol->fltVal;
	if (dilationCol<=1.0)
	{
		dilationCol=1.0;
	}
	int ocrHeight     = (int)pOcrHeight->fltVal;
	if (ocrHeight<=8)
	{
		ocrHeight=8;
	}
	int ocrWidth      = (int)pOcrWidth->fltVal;
	if (ocrWidth<=2)
	{
		ocrWidth=2;
	}
	//
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pOcrNumber);
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
		//
		if(m_bPilotImg||m_firstUse)
		{
			m_firstUse=false;
			//读取信息前，先清空模板相关数据。
			HTuple ModelNum1,ModelNum2;
			bool m_readDataState=ReadDatasForDirection(m_strTaskPath,MaxNumber,m_isChineseLanguage,ModelNum1,ModelNum2,m_ocrModelIds1,m_ocrModelIds2);
			if(!m_readDataState)
			{
				m_firstUse=true;
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("请使用数字建模算子建模."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please create the model."),_T("Reading model goes wrong:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				retValue = -1;
				m_vErrorRgn[rId].Reset();
				retValue.Detach(pOcrNumber);
				return S_FALSE;
			}
		}
		//
		Hobject Image = m_crtImg;
		HTuple DetectNumber,MatchingNumber,MatchingNumber2,Width1,Height1,RowOcrCenter,ColumnOcrCenter,Area1,Angle2,HomMat2D,HomMat2DInv;
		HTuple Area4, Row4, Column4,Number;
		Hobject ImageAffinTrans,ImageReduced,RegionAffineTrans,RegionFillUp,RegionDilation1,DiffOfGauss,ImageConverted,ImageSub,ImageScaleMax;
		DetectNumber = 0;
		MatchingNumber = 0;
		MatchingNumber2 = 0;

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		get_image_size(Image, &Width1, &Height1);
		area_center(rgn, &Area1, &RowOcrCenter, &ColumnOcrCenter);
		angle_lx(fCenterY, fCenterX, RowOcrCenter, ColumnOcrCenter,  &Angle2);
		vector_angle_to_rigid(fCenterY, fCenterX, 0, Height1/2.0, Width1/2.0, (-Angle2)-(HTuple(90).Rad()), &HomMat2D);
		affine_trans_image(Image, &ImageAffinTrans, HomMat2D, "bilinear", "false");

		affine_trans_region(rgn, &RegionAffineTrans, HomMat2D, "nearest_neighbor");
		//*
		ocr_segment_yaoBo(ImageAffinTrans, RegionAffineTrans, &RegionFillUp, contrastMin, contrast, edgeSize, dilationRow, dilationCol, ocrWidth, ocrHeight, &Number);
		//*
		area_center(RegionFillUp, &Area4, &Row4, &Column4);
		dilation_circle(RegionAffineTrans, &RegionDilation1, 7.5);
		reduce_domain(ImageAffinTrans, RegionDilation1, &ImageReduced);
		diff_of_gauss(ImageReduced, &DiffOfGauss, 1.5, 15.6);
		convert_image_type(DiffOfGauss, &ImageConverted, "byte");
		sub_image(ImageReduced, ImageConverted, &ImageSub, 1, 0);

		scale_image_max(ImageSub, &ImageScaleMax);
		//******************
		if (0 != (Number>0))
		{
			Hobject SearchRegion,ImageReduced2;
			HTuple  Row3, Column3, Angle3, Score3, ModelNumber;
			gen_circle(&SearchRegion, Row4.Select(0), Column4.Select(0), 10);
			reduce_domain(ImageScaleMax, SearchRegion, &ImageReduced2);
			find_shape_models(ImageReduced2, m_ocrModelIds1, HTuple(-20).Rad(), HTuple(40).Rad(), 0.2, 1, 0.5, "least_squares", 0, 0.75, &Row3, &Column3, &Angle3, &Score3, &ModelNumber);
			if ((ModelNumber.Num())==1)
			{
				MatchingNumber = ModelNumber+1;
				if (0 != (MatchingNumber>9))
				{
					MatchingNumber = MatchingNumber/10;
				}
			}

		}
		if (Number==2 && MaxNumber>9)
		{
			Hobject SearchRegion,ImageReduced2;
			HTuple  Row3, Column3, Angle3, Score3, ModelNumber;
			gen_circle(&SearchRegion, Row4.Select(1), Column4.Select(1), 10);
			reduce_domain(ImageScaleMax, SearchRegion, &ImageReduced2);
			find_shape_models(ImageReduced2, m_ocrModelIds2, HTuple(-20).Rad(), HTuple(40).Rad(), 0.2, 1, 0.5, "least_squares", 0, 0.75, &Row3, &Column3, &Angle3, &Score3, &ModelNumber);
			if ((ModelNumber.Num())==1)
			{
				MatchingNumber2 = ModelNumber;
			}
		}
		DetectNumber = ((MatchingNumber*(HTuple(10).Pow(Number-1)))+(MatchingNumber2*(Number-1))).Round();
		//
		if (Number>2)
		{
			DetectNumber=0;
		}
		//////////////
		vector_angle_to_rigid(Height1/2.0, Width1/2.0, (-Angle2)-(HTuple(90).Rad()),fCenterY, fCenterX, 0,  &HomMat2DInv);
		Hobject RegionOcr,ShowOcrReg;
		union1(RegionFillUp,&RegionOcr);
		affine_trans_region(RegionOcr, &ShowOcrReg, HomMat2DInv, "nearest_neighbor");
		//

		retValue = DetectNumber[0].I();
		m_vErrorRgn[rId] = ShowOcrReg;
		m_vRgn[rId] = ShowOcrReg;
		retValue.Detach(pOcrNumber);
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pOcrNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CHdpeAlgo::FindOcrNumberAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("870$\
					 200;R;LP;1;255;1;>=#1$\
					 167;B$\
					 1310;LP;1;50;1;9$\
					 762;LP;1;120;1;10$\
					 763;LP;5;250;1;20$\
					 744;LP;1;400;1;6$\
					 524;FP;0.5;50.0;0.5;13.0$\
					 523;FP;0.5;40.0;0.5;5.0$\
					 844;LP;5;300;1;90$\
					 845;LP;2;200;1;50");
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
STDMETHODIMP CHdpeAlgo::SetCurrentTaskName(BSTR* bstrTaskName,BSTR* bstrChName)
{
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+*bstrChName+L"\\"+m_strTaskName;
	m_strTaskPath = CW2A(m_wstrTaskPath.c_str());
	m_wstrChannelPath= m_strExePath +L"\\data\\"+*bstrChName;
	m_strChannelPath = CW2A(m_wstrChannelPath.c_str());
	return S_OK;
}

STDMETHODIMP CHdpeAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

//Color image process, Algo.
STDMETHODIMP CHdpeAlgo::DynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::DynThresholdForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("660$\
					 397;R;LP;0;9000;1;<=#20$\
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
STDMETHODIMP CHdpeAlgo::EdgeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::EdgeForColorAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CHdpeAlgo::AnnularRotaDynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CHdpeAlgo::AnnularRotaDynThresholdForColorAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CHdpeAlgo::CurlNoCompoundForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pGrayValueMean)
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
STDMETHODIMP CHdpeAlgo::CurlNoCompoundForColorAlgoHelp(BSTR* pHelpStr)
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