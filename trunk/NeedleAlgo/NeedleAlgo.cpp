#include "stdafx.h"
#include "resource.h"
#include "NeedleAlgo_i.h"
#include "dllmain.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "../ImageProcessLib/ImageProcessLib.h"
#pragma comment(lib,"ImageProcessLib.lib")
using namespace ColorImage;
using namespace Halcon;
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

// NeedleAlgo.cpp:
#include "stdafx.h"
#include "NeedleAlgo.h"

STDMETHODIMP CNeedleAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_INeedleAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CNeedleAlgo::SetCurrentImage(LONG* pImg)
{
	m_imageNumber++;
	Hobject* pObj = (Hobject*)pImg;

	m_crtImgColor = *pObj;
	m_crtImgMult  = *pObj;// 原始图片，可能是多通道图片（彩色图片）
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
		m_dictCrtPathModelID.clear();
		m_dictCrtPathModelID = m_dictNewPathModelID;

		m_dictCrtPathRgn.clear();
		m_dictCrtPathRgn = m_dictNewPathRgn;
		m_bFileUpdated = false;
	}

	return S_OK;
}
STDMETHODIMP CNeedleAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::GetGlueInjectorID(LONG* pInjectorID)
{
	*pInjectorID = m_nCrtInjectorID;
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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

// self-learning interface methods
STDMETHODIMP CNeedleAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{
	return S_FALSE;
}
STDMETHODIMP CNeedleAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}
STDMETHODIMP CNeedleAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::ResetModels(BYTE productID)
{

	return S_OK;
}
STDMETHODIMP CNeedleAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{

	return S_OK;
}
STDMETHODIMP CNeedleAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::ModelCreate(double Radius, HTuple * ModelID1)
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
STDMETHODIMP CNeedleAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CNeedleAlgo::ReloadExternalFile(BSTR* bstrPath)
{

	return S_OK;
}
STDMETHODIMP CNeedleAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CNeedleAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

//NeedleAlgo 
STDMETHODIMP CNeedleAlgo::BeltSideCenterLocationAlgo(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pThredLower,VARIANT* pThredUpper,VARIANT* pClosingSize)
{
	int id = (int)rgnID->fltVal; 
	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;

	int ThredLower=(int)pThredLower->fltVal;
	int ThredUpper=(int)pThredUpper->fltVal;
	int ClosingSize=(int)pClosingSize->fltVal;

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
		if(m_bPilotImg)
		{
			return S_OK;
		}
		//判断初始化图片
		if (ImageWidth==659 && ImageHeight==475)
		{
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			Hobject Rgn;
			gen_circle(&Rgn,m_vPos[0].m_y,m_vPos[0].m_x,100);
			m_vRgn[id] = Rgn ;
			return S_FALSE;
		}
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
		reduce_domain(Image,SearchRgn,&ImageReduced);
		//
		Hobject ImageEnhanced,ImageEquHisto;
		emphasize(ImageReduced,&ImageEnhanced,5,5,1);
		equ_histo_image(ImageEnhanced,&ImageEquHisto);
		//皮带区域
		Hobject BeltReg,BeltRegFillUp,BeltRegConnected,BeltRegSelected,BeltRegClosing;
		threshold(ImageEquHisto, &BeltReg, ThredLower, 255);
		closing_circle(BeltReg,&BeltRegClosing,80);

		fill_up(BeltRegClosing, &BeltRegFillUp);
		connection(BeltRegFillUp, &BeltRegConnected);
		select_shape_std(BeltRegConnected, &BeltRegSelected, "max_area", 70);

		//针套区域
		Hobject BeltRegErosion,BeltRegImageReduced;
		erosion_circle(BeltRegSelected,&BeltRegErosion,3.5);
		reduce_domain(ImageEnhanced,BeltRegErosion,&BeltRegImageReduced);

		Hobject ObjReg,ConnectedRegs,ImageClosing;
		gray_closing_rect(BeltRegImageReduced, &ImageClosing, 5, 5);
		threshold(ImageClosing,&ObjReg, 0, ThredUpper);
		Hobject SelectedRegs1,RegionFillUp;
		fill_up(ObjReg,&RegionFillUp);
		connection(RegionFillUp, &ConnectedRegs);
		select_shape_std(ConnectedRegs, &SelectedRegs1, "max_area", 70);

		Hobject RegionClosing,ROI,RegionOpening1;
		opening_circle(SelectedRegs1, &RegionOpening1, 10);
		closing_circle(RegionOpening1, &ROI, ClosingSize);

		if(m_bDebugImg)
		{

			HTuple HomMat2DIdentity,HomMat2DScale;
			hom_mat2d_identity(&HomMat2DIdentity);
			hom_mat2d_scale(HomMat2DIdentity, 1, 1, 0, 0, &HomMat2DScale);
			Hobject ROIAffineTrans;
			affine_trans_region(ROI, &ROIAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(ROIAffineTrans, &RegionClosingMoved, 0, 0);

			Hobject RegionMargin;
			rectBottomY=rectTopY+(((rectBottomY-rectTopY)/4).Round())*4;
			rectBottomX=rectTopX+(((rectBottomX-rectTopX)/4).Round())*4;
			gen_region_polygon(&RegionMargin, ((((HTuple(rectTopY).Append(rectTopY)).Append(rectBottomY)).Append(rectBottomY)).Append(rectTopY)), 
				((((HTuple(rectTopX).Append(rectBottomX)).Append(rectBottomX)).Append(rectTopX)).Append(rectTopX)));
			concat_obj (RegionMargin, RegionClosingMoved, &RegionClosingMoved);
		}

		m_DetectReg=ROI;
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
STDMETHODIMP CNeedleAlgo::BeltSideCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("951$\
					 167;B$\
					 939;LP;20;200;1;45$\
					 938;LP;34;800;1;35$\
					 940;LP;1;650;1;600$\
					 941;LP;1;490;1;338$\
					 983;LP;0;255;1;45$\
					 984;LP;0;255;1;55$\
					 950;LP;1;150;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::SideCenterLocationAlgo(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pThredLower)
{
	int id = (int)rgnID->fltVal; 
	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;

	int ThredLower=(int)pThredLower->fltVal;

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
		if(m_bPilotImg)
		{
			return S_OK;
		}
		//判断初始化图片
		if (ImageWidth==659 && ImageHeight==475)
		{
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			Hobject Rgn;
			gen_circle(&Rgn,m_vPos[0].m_y,m_vPos[0].m_x,100);
			m_vRgn[id] = Rgn ;
			return S_FALSE;
		}
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
		reduce_domain(Image,SearchRgn,&ImageReduced);
		//
		Hobject ImageEnhanced,ImageClosing;
		emphasize(ImageReduced,&ImageEnhanced,5,5,1);
		gray_closing_rect(ImageEnhanced,&ImageClosing,5,9);
		Hobject ObjReg,ConnectedRegs,ObjRegErosion,ObjRegDilation;
		threshold(ImageClosing,&ObjReg, ThredLower, 255);
		erosion_circle(ObjReg,&ObjRegErosion,1.5);
		dilation_circle(ObjRegErosion,&ObjRegDilation,1.5);

		connection(ObjRegDilation, &ConnectedRegs);
		Hobject RegionFillUp,SelectedRegs,ROI;
		fill_up(ConnectedRegs, &RegionFillUp);
		//
		Hobject RegionUnion2,RegionClosing3,RegionFillUp1,ConnectedRegionsS;
		union1 (RegionFillUp, &RegionUnion2);
		//
		Hobject ho_ModelImages,ho_ModelRegions,ho_RegionErosion,ho_RegionIntersection,ho_RegionUnion;
		inspect_shape_model(ImageClosing, &ho_ModelImages, &ho_ModelRegions, 1, ((HTuple(10).Append(20)).Append(20)));
		erosion_circle(SearchRgn, &ho_RegionErosion, 3.5);
		intersection(ho_ModelRegions, ho_RegionErosion, &ho_RegionIntersection);
		union2(RegionUnion2, ho_RegionIntersection, &ho_RegionUnion);
		//
		closing_rectangle1 (ho_RegionUnion, &RegionClosing3, 21, 7);

		fill_up (RegionClosing3, &RegionFillUp1);
		//
		opening_circle(RegionFillUp1, &RegionFillUp1, 2.5);
		//
		connection (RegionFillUp1, &ConnectedRegionsS);
		//
		select_shape_std(ConnectedRegionsS, &ROI, "max_area", 70);

		if(m_bDebugImg)
		{

			HTuple HomMat2DIdentity,HomMat2DScale;
			hom_mat2d_identity(&HomMat2DIdentity);
			hom_mat2d_scale(HomMat2DIdentity, 1, 1, 0, 0, &HomMat2DScale);
			Hobject ROIAffineTrans;
			affine_trans_region(ROI, &ROIAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(ROIAffineTrans, &RegionClosingMoved, 0, 0);

			Hobject RegionMargin;
			rectBottomY=rectTopY+(((rectBottomY-rectTopY)/4).Round())*4;
			rectBottomX=rectTopX+(((rectBottomX-rectTopX)/4).Round())*4;
			gen_region_polygon(&RegionMargin, ((((HTuple(rectTopY).Append(rectTopY)).Append(rectBottomY)).Append(rectBottomY)).Append(rectTopY)), 
				((((HTuple(rectTopX).Append(rectBottomX)).Append(rectBottomX)).Append(rectTopX)).Append(rectTopX)));
			concat_obj (RegionMargin, RegionClosingMoved, &RegionClosingMoved);
		}

		m_DetectReg=ROI;
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
STDMETHODIMP CNeedleAlgo::SideCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("961$\
					 167;B$\
					 939;LP;20;200;1;45$\
					 938;LP;34;800;1;35$\
					 940;LP;1;650;1;600$\
					 941;LP;1;490;1;338$\
					 984;LP;0;255;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::EndCenterLocationAlgo(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pGrayValue,VARIANT* pLowerValue,VARIANT* pUpValue,VARIANT* pBigRadius,VARIANT* pSmallRadius)
{
	int id = (int)rgnID->fltVal; 
	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;

	int GrayValue=(int)pGrayValue->fltVal;
	int LowerValue=(int)pLowerValue->fltVal;
	int UpValue=(int)pUpValue->fltVal;

	int BigRadius=(int)pBigRadius->fltVal;
	int SmallRadius=(int)pSmallRadius->fltVal;

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
		if(m_bPilotImg)
		{
			return S_OK;
		}
		//判断初始化图片
		if (ImageWidth==659 && ImageHeight==475)
		{
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			Hobject Rgn;
			gen_circle(&Rgn,m_vPos[0].m_y,m_vPos[0].m_x,100);
			m_vRgn[id] = Rgn ;
			return S_FALSE;
		}
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
		reduce_domain(Image,SearchRgn,&ImageReduced);
		//
		Hobject ImageEnhanced;
		emphasize(ImageReduced,&ImageEnhanced,5,5,1);
		Hobject Regions,RegionFillup;
		threshold(ImageEnhanced, &Regions, GrayValue, 255);
		fill_up(Regions, &RegionFillup);
		Hobject ConnectedRegions2,SelectedRegions3;
		connection(RegionFillup, &ConnectedRegions2);
		select_shape_std(ConnectedRegions2, &SelectedRegions3, "max_area", 70);
		Hobject RegionClosing,RegionClosing1;
		closing_rectangle1(SelectedRegions3, &RegionClosing, 100, 1);
		closing_rectangle1(RegionClosing, &RegionClosing1, 3000, 1);
		Hobject RegionDifference,ConnectedRegions,SelectedRegions;
		difference(RegionClosing1, RegionClosing, &RegionDifference);
		connection(RegionDifference, &ConnectedRegions);
		select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
		Hobject RegionOpening,RegionDilation;
		opening_circle(SelectedRegions, &RegionOpening, 3.5);

		m_RegionOpening=RegionOpening;

		dilation_rectangle1(RegionOpening, &RegionDilation, 5, 80);
		Hobject ImageReduced1,Regions1,ImageReduced2;
		reduce_domain(ImageEnhanced, RegionDilation, &ImageReduced1);
		threshold(ImageReduced1, &Regions1, 0, GrayValue);
		//
		Hobject Regions1Fillup,Regions1Closing;
		fill_up(Regions1,&Regions1Fillup);
		closing_circle(Regions1Fillup,&Regions1Closing,10);
		//
		reduce_domain(ImageEnhanced, Regions1Closing, &ImageReduced2);
		Hobject Regions2,RegionFillUp1;
		threshold(ImageReduced2, &Regions2, LowerValue, UpValue);
		fill_up(Regions2, &RegionFillUp1);
		Hobject ROI;
		HTuple StdRow, StdColumn, StdRadius;
		opening_circle(RegionFillUp1, &ROI, BigRadius-5);
		smallest_circle(ROI, &StdRow, &StdColumn, &StdRadius);

		if (StdRadius>BigRadius)
		{
			m_HeadRegion=ROI;
			//m_HeadRegion1=ROI;
			HTuple HeadRegionRaw,HeadRegionCol;
			area_center(m_HeadRegion,&m_HeadRegionArea,&HeadRegionRaw,&HeadRegionCol);

			m_vPos[0].m_x = HeadRegionCol[0].D();
			m_vPos[0].m_y = HeadRegionRaw[0].D();

			Hobject EmptyEndReg;
			gen_empty_region(&EmptyEndReg);
			m_EndRegion=EmptyEndReg;
		}
		else
		{
			Hobject RegionOpening1;
			opening_circle(RegionFillUp1, &RegionOpening1, SmallRadius-5);
			closing_circle(RegionOpening1, &ROI, SmallRadius);
			m_EndRegion=ROI;

			HTuple EndRegionRaw,EndRegionCol;
			area_center(m_EndRegion,&m_EndRegionArea,&EndRegionRaw,&EndRegionCol);

			m_vPos[0].m_x = EndRegionCol[0].D();
			m_vPos[0].m_y = EndRegionRaw[0].D();

			Hobject EmptyHeadReg;
			gen_empty_region(&EmptyHeadReg);
			m_HeadRegion=EmptyHeadReg;

		}

		if(m_bDebugImg)
		{
			HTuple HomMat2DIdentity,HomMat2DScale;
			hom_mat2d_identity(&HomMat2DIdentity);
			hom_mat2d_scale(HomMat2DIdentity, 1, 1, 0, 0, &HomMat2DScale);
			Hobject RegionAffineTrans;
			affine_trans_region(RegionFillUp1, &RegionAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(RegionAffineTrans, &RegionClosingMoved, 0, 0);

			Hobject RegionMargin;
			rectBottomY=rectTopY+(((rectBottomY-rectTopY)/4).Round())*4;
			rectBottomX=rectTopX+(((rectBottomX-rectTopX)/4).Round())*4;
			gen_region_polygon(&RegionMargin, ((((HTuple(rectTopY).Append(rectTopY)).Append(rectBottomY)).Append(rectBottomY)).Append(rectTopY)), 
				((((HTuple(rectTopX).Append(rectBottomX)).Append(rectBottomX)).Append(rectTopX)).Append(rectTopX)));
			concat_obj (RegionMargin, RegionClosingMoved, &RegionClosingMoved);
		}
		/*m_vPos[0].m_x = (float)(m_imgWidth/2.0);
		m_vPos[0].m_y = (float)(m_imgHeight/2.0);*/
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
STDMETHODIMP CNeedleAlgo::EndCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("968$\
					 167;B$\
					 939;LP;20;200;1;40$\
					 938;LP;34;800;1;60$\
					 940;LP;1;650;1;550$\
					 941;LP;1;490;1;338$\
					 969;LP;0;255;1;90$\
					 970;LP;0;255;1;20$\
					 971;LP;0;255;1;125$\
					 972;LP;0;200;1;72$\
					 973;LP;0;200;1;52");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::MidDetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pOffset,VARIANT* pClipSwitch,VARIANT* pMovedSize)
{
	int id = (int)rgnId->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
	int Offset=(int)pOffset->fltVal;
	int ClipSwitch=(int)pClipSwitch->fltVal;
	long MovedSize=(long)pMovedSize->fltVal;

	Hobject newRgn;
	if(m_DetectReg.Id() == H_EMPTY_REGION)
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	Hobject  RegionDetect;
	//添加中间区域公共变量
	Hobject EmptyEndReg;
	gen_empty_region(&EmptyEndReg);
	m_MiddleRegion=EmptyEndReg;
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
		HTuple Class;
		get_obj_class(m_DetectReg,&Class);
		if(Class!="region")
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('MidDetectRegionLocationAlgo')前需先调用中心轮廓提取函数('SideCenterLocationAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('CenterRegionExtractAlgo')before calling location function('DetectRegionLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		HTuple Row, Col, phi, Length1, Length2;
		Hobject Rectangle;
		smallest_rectangle2(m_DetectReg, &Row, &Col, &phi, &Length1, &Length2);
		gen_rectangle2(&Rectangle, Row, Col, phi, Length1, Length2);
		HTuple LengthLong,LengthShort;
		if (0 != (Length1>Length2))
		{
			LengthLong = Length1;
			LengthShort = Length2;
		}
		else
		{
			LengthLong = Length2;
			LengthShort = Length1;
		}

		Hobject Rectangle2,RegionHead1;
		HTuple Row2, col2;
		Row2 = Row+(LengthLong*(phi.Sin()));
		col2 = Col-(LengthLong*(phi.Cos()));
		gen_rectangle2(&Rectangle2, Row2, col2, phi, LengthShort+Offset,LengthShort+Offset);
		intersection(m_DetectReg, Rectangle2, &RegionHead1);

		Hobject Rectangle3,RegionHead2;
		HTuple Row3, Col3;
		Row3 = Row-(LengthLong*(phi.Sin()));
		Col3 = Col+(LengthLong*(phi.Cos()));
		gen_rectangle2(&Rectangle3, Row3, Col3, phi, LengthShort+Offset, LengthShort+Offset);
		intersection(m_DetectReg, Rectangle3, &RegionHead2);

		//MiddleRegion
		//*判断口部尾部
		//'1'剔除口部,'0'不剔除，'-1'剔除两端
		Hobject RegionMiddle,EmptyReg,RegionDifference;
		gen_empty_region(&EmptyReg);
		if (0 != (ClipSwitch==1))
		{
			HTuple AreaHead1, RowHead1, ColumnHead1,AreaHead2, RowHead2, ColumnHead2;
			area_center(RegionHead1, &AreaHead1, &RowHead1, &ColumnHead1);
			area_center(RegionHead2, &AreaHead2, &RowHead2, &ColumnHead2);
			if (0 != (AreaHead1<AreaHead2))
			{
				difference(m_DetectReg, RegionHead2, &RegionMiddle);
			}
			else
			{
				difference(m_DetectReg, RegionHead1, &RegionMiddle);
			}
		}
		else if(0 != (ClipSwitch==0))
		{
			difference(m_DetectReg, EmptyReg, &RegionMiddle);
		}
		else if(0 != (ClipSwitch==-1))
		{
			difference(m_DetectReg, RegionHead1, &RegionDifference);
			difference(RegionDifference, RegionHead2, &RegionMiddle);
		}

		//排除皮带拐角阴影的影响,需要根据设备图片决定
		Hobject RegionMidZ,RegionMoved;
		move_region(RegionMiddle, &RegionMoved, MovedSize, 0);
		difference(RegionMiddle, RegionMoved, &RegionMidZ);
		//
		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(RegionMidZ, RectangleImage, &RegionDetect);
		//
		if(ErosionSize>=0.5)
		{
			erosion_circle (RegionDetect, &RegionDetect,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(RegionDetect, &RegionDetect,abs(ErosionSize));
		}
		else
		{
			RegionDetect = RegionDetect;
		}
		m_vRgn[id] = RegionDetect ;	
		m_MiddleRegion=RegionDetect;
		//m_detectRoi=DetectRgn;
		/*m_MainRgn=RegionDetect;*/
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
STDMETHODIMP CNeedleAlgo::MidDetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("952$\
					 167;B$\
					 443;FP;-20.5;20.5;0.5;1.0$\
					 953;LP;0;45;1;10$\
					 954;LP;-1;1;1;1$\
					 955;LP;1;100;1;50"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::AberrationDetectAlgo(VARIANT* rgnId,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;

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
			return S_OK;
		}

		Hobject Image = m_crtImgColor;
		Hobject ImageReduced,R,G,B,l,a,b;
		HTuple  L1mean,L1deviation,A1mean,A1deviation,B1mean,B1deviation;
		HTuple  Ldiff,Adiff,Bdiff,AberrationValue,ChannelNum;

		count_channels(Image,&ChannelNum);
		if (ChannelNum!=3)
		{
			return S_OK;
		}
		reduce_domain(Image, rgn, &ImageReduced);
		decompose3(ImageReduced, &R, &G, &B);
		trans_from_rgb(R, G, B, &l, &a, &b, "cielab");

		intensity(rgn, l, &L1mean, &L1deviation);
		intensity(rgn, a, &A1mean, &A1deviation);
		intensity(rgn, b, &B1mean, &B1deviation);

		if(m_aberrationFirst)
		{
			m_LMeanStd=L1mean;
			m_AMeanStd=A1mean;
			m_BMeanStd=B1mean;

			m_aberrationFirst=false;
		}

		Ldiff = L1mean-m_LMeanStd;
		Adiff = A1mean-m_AMeanStd;
		Bdiff = B1mean-m_BMeanStd;

		AberrationValue = (((Ldiff*Ldiff)+(Adiff*Adiff))+(Bdiff*Bdiff)).Sqrt();

		retValue = AberrationValue[0].I();
		m_vErrorRgn[rId]=rgn;
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
STDMETHODIMP CNeedleAlgo::AberrationDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("956$\
					 397;R;LP;0;10000;1$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::CurveDetectionAlgo(VARIANT* rgnId, VARIANT* pDetectArea)
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
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		HTuple  Row, Col, phi, Length1, Length2;
		Hobject Rect,RectErosion,ErrorReg;
		smallest_rectangle2(rgn, &Row, &Col, &phi, &Length1, &Length2);
		gen_rectangle2(&Rect, Row, Col, phi, Length1, Length2);
		erosion_circle(Rect, &RectErosion, 2);
		difference(RectErosion, rgn, &ErrorReg);

		HTuple ErrorArea, ErrorRow, ErrorColumn;
		area_center(ErrorReg, &ErrorArea, &ErrorRow, &ErrorColumn);

		retValue =ErrorArea[0].I();
		m_vErrorRgn[rId]=Rect;
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
STDMETHODIMP CNeedleAlgo::CurveDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("957$\
					 397;R;LP;0;50000;1;<=#2000$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::SideDynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
		if(m_bPilotImg)
		{
			return S_OK;
		}
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
		//单一方向进行平滑
		mean_image(ImageDetectNew, &ImageReduceMean, BlackMaskSize, 1);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		if(abs(WhiteMaskSize-BlackMaskSize)>=1)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
			expand_domain_gray (ImageReduced, &ExpandedImage, 25);
			reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
			//单一方向进行平滑
			mean_image(ImageDetectNew, &ImageReduceMean, WhiteMaskSize, 1);
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
STDMETHODIMP CNeedleAlgo::SideDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("981$\
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

STDMETHODIMP CNeedleAlgo::LengthDetectionAlgo(VARIANT* rgnId, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(m_DetectReg,&area,&row,&col);
	if(m_DetectReg.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
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
		if(m_bPilotImg)
		{
			return S_OK;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		HTuple  Row, Col, phi, Length1, Length2;
		Hobject Rectangle;
		smallest_rectangle2(m_DetectReg, &Row, &Col, &phi, &Length1, &Length2);
		gen_rectangle2(&Rectangle, Row, Col, phi, Length1, Length2);
		HTuple LengthLong, LengthShort,LengthRatio;
		if (0 != (Length1>Length2))
		{
			LengthLong = Length1;
			LengthShort = Length2;
		}
		else
		{
			LengthLong = Length2;
			LengthShort = Length1;
		}

		retValue =LengthLong[0].I();
		m_vErrorRgn[rId]=Rectangle;
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
STDMETHODIMP CNeedleAlgo::LengthDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("958$\
					 397;R;LP;0;50000;1;<=#2000$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::HeadDetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize,VARIANT* pOffset,VARIANT* pClipSwitch)
{
	int id = (int)rgnId->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
	int Offset=(int)pOffset->fltVal;
	int ClipSwitch=(int)pClipSwitch->fltVal;

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
		if(m_bPilotImg)
		{
			return S_OK;
		}
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
					MessageBox(NULL,_T("调用函数('HeadDetectRegionLocationAlgo')前需先调用中心轮廓提取函数('SideCenterLocationAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('HeadDetectRegionLocationAlgo')before calling location function('SideCenterLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		HTuple Row, Col, phi, Length1, Length2;
		Hobject Rectangle;
		smallest_rectangle2(m_DetectReg, &Row, &Col, &phi, &Length1, &Length2);
		gen_rectangle2(&Rectangle, Row, Col, phi, Length1, Length2);
		HTuple LengthLong,LengthShort;
		if (0 != (Length1>Length2))
		{
			LengthLong = Length1;
			LengthShort = Length2;
		}
		else
		{
			LengthLong = Length2;
			LengthShort = Length1;
		}

		Hobject Rectangle2,RegionHead1;
		HTuple Row2, col2;
		Row2 = Row+(LengthLong*(phi.Sin()));
		col2 = Col-(LengthLong*(phi.Cos()));
		gen_rectangle2(&Rectangle2, Row2, col2, phi, LengthShort+Offset,LengthShort+Offset);
		intersection(m_DetectReg, Rectangle2, &RegionHead1);

		Hobject Rectangle3,RegionHead2;
		HTuple Row3, Col3;
		Row3 = Row-(LengthLong*(phi.Sin()));
		Col3 = Col+(LengthLong*(phi.Cos()));
		gen_rectangle2(&Rectangle3, Row3, Col3, phi, LengthShort+Offset, LengthShort+Offset);
		intersection(m_DetectReg, Rectangle3, &RegionHead2);

		//HeadRegion
		//*判断口部尾部
		//'1'口部,'0'两端，'-1'尾部
		Hobject RegionMiddle,EmptyReg,RegionDifference,HeadRegion;
		gen_empty_region(&EmptyReg);
		//HTuple AreaHead1, RowHead1, ColumnHead1,AreaHead2, RowHead2, ColumnHead2;
		//area_center(RegionHead1, &AreaHead1, &RowHead1, &ColumnHead1);
		//area_center(RegionHead2, &AreaHead2, &RowHead2, &ColumnHead2);
		//
		Hobject RegionDilationHead1,RegionDilationHead2;
		HTuple CircleRow1, CircleColumn1, CircleRadius1,CircleRow2, CircleColumn2, CircleRadius2;
		dilation_rectangle1 (RegionHead1, &RegionDilationHead1,15, 1);
		dilation_rectangle1 (RegionHead2, &RegionDilationHead2,15, 1);
		inner_circle (RegionDilationHead1, &CircleRow1, &CircleColumn1, &CircleRadius1);
		inner_circle (RegionDilationHead2, &CircleRow2, &CircleColumn2, &CircleRadius2);    
		//
		if (0 != (ClipSwitch==1))
		{
			if (0 != (CircleRadius1<CircleRadius2))
			{
				union2(EmptyReg,RegionHead2,&RegionDetect);
			}
			else
			{
				union2(EmptyReg,RegionHead1,&RegionDetect);
			}
		}
		else if(0 != (ClipSwitch==0))
		{
			concat_obj(RegionHead1, RegionHead2, &HeadRegion);
			union1(HeadRegion, &RegionDetect);
		}
		else if(0 != (ClipSwitch==-1))
		{
			if (0 != (CircleRadius1<CircleRadius2))
			{
				union2(EmptyReg,RegionHead1,&RegionDetect);
			}
			else
			{
				union2(EmptyReg,RegionHead2,&RegionDetect);
			}
		}
		//
		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(RegionDetect, RectangleImage, &RegionDetect);
		//
		if(ErosionSize>=0.5)
		{
			erosion_circle (RegionDetect, &RegionDetect,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(RegionDetect, &RegionDetect,abs(ErosionSize));
		}
		else
		{
			RegionDetect = RegionDetect;
		}
		m_vRgn[id] = RegionDetect ;	
		//m_detectRoi=DetectRgn;
		/*m_MainRgn=RegionDetect;*/
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
STDMETHODIMP CNeedleAlgo::HeadDetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("960$\
					 167;B$\
					 443;FP;-20.5;20.5;0.5;1.0$\
					 953;LP;0;80;1;30$\
					 959;LP;-1;1;1;1"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//STDMETHODIMP CNeedleAlgo::ConcaveDetectAlgo(VARIANT* rgnId, VARIANT* pClosingSize, VARIANT* pOpeningSize,VARIANT* pLimitArea,VARIANT* pDetectArea)
//{
//	int rId = (int)rgnId->fltVal;
//	m_vErrorRgn[rId].Reset();
//	Hobject rgn = m_vRgn[rId];
//	CComVariant retValue;
//
//
//	if(rgn.Id() == H_EMPTY_REGION)
//	{
//		retValue = -1;
//		m_vErrorRgn[rId].Reset();
//		retValue.Detach(pDetectArea);
//		return S_FALSE;
//	}
//	float ClosingSize = (float)pClosingSize->fltVal;
//	int LimitArea = (int)pLimitArea->fltVal;
//	float OpeningSize = (float)pOpeningSize->fltVal;
//
//	Hobject Image = m_crtImg;
//#ifdef NDEBUG
//	try
//	{
//#endif
//		//
//		if(m_bPilotImg)
//		{
//			return S_OK;
//		}
//		if(m_bDebugImg)
//		{
//			m_dictSpecialData[rId].clear();
//		}
//		//凹陷
//		Hobject RegionClosing,ErrorRegs1,ConnectedErrorRegs1,SelectedErrorRegs1;
//		closing_circle(rgn, &RegionClosing, ClosingSize);
//		difference(rgn, RegionClosing, &ErrorRegs1);
//		/*connection(ErrorRegs1, &ConnectedErrorRegs1);
//		select_shape(ConnectedErrorRegs1, &SelectedErrorRegs1, "area", "and", LimitArea, 99999);*/
//
//		//凸起
//		Hobject RegionOpening,ErrorRegs2,ConnectedErrorRegs2,SelectedErrorRegs2;
//		opening_circle(rgn, &RegionOpening, OpeningSize);
//		difference(rgn, RegionOpening, &ErrorRegs2);
//		/*connection(ErrorRegs2, &ConnectedErrorRegs2);
//		select_shape(ConnectedErrorRegs2, &SelectedErrorRegs2, "area", "and", LimitArea, 99999);*/
//
//		Hobject ErrorRegs;
//		concat_obj(ErrorRegs1, ErrorRegs2, &ErrorRegs);
//		union1(ErrorRegs, &ErrorRegs);
//
//
//		//*剔除两端拐角区域
//		HTuple Row11, Column11, Row22, Column22;
//		smallest_rectangle1(rgn, &Row11, &Column11, &Row22, &Column22);
//		//gen_rectangle1(&Rectangle1, Row11, Column11, Row22, Column22);
//		//*对缺陷进行缩放，排除误检
//		Hobject ConnectedRegions,RegionErosion,RegionDilation,SelectedRegions,RegionUnion;
//		connection(ErrorRegs, &ConnectedRegions);
//		erosion_circle(ConnectedRegions, &RegionErosion, 1);
//		dilation_circle(RegionErosion, &RegionDilation, 1);
//		if ((Column11+OpeningSize)>=(Column22-OpeningSize))
//		{
//			retValue = -1;
//			m_vErrorRgn[rId].Reset();
//			retValue.Detach(pDetectArea);
//			return S_FALSE;
//		}
//		select_shape(RegionDilation, &SelectedRegions, (HTuple("area").Append("column")), "and", HTuple(LimitArea).Concat(Column11+OpeningSize), HTuple(99999).Concat(Column22-OpeningSize));
//		union1(SelectedRegions, &RegionUnion);
//
//		HTuple ErrorArea, ErrorRow, ErrorColumn;
//		area_center(RegionUnion, &ErrorArea, &ErrorRow, &ErrorColumn);
//		//
//		if(ErrorArea <= 0)
//		{
//			retValue = 0.0;
//			Hobject newRgn;
//			m_vErrorRgn[rId] = newRgn;
//		}
//		else
//		{
//			retValue = ErrorArea[0].I();
//			Hobject UnionErrorRegs;
//			dilation_circle (RegionUnion, &UnionErrorRegs, 2.5);
//			m_vErrorRgn[rId] = UnionErrorRegs;	
//		}		
//		retValue.Detach(pDetectArea);
//#ifdef NDEBUG
//	}
//	catch (HException &except)  
//	{
//		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
//		retValue=-1;
//		m_vErrorRgn[rId].Reset();
//		retValue.Detach(pDetectArea);
//		return S_FALSE;
//	}
//#endif
//	return S_OK;
//}
//STDMETHODIMP CNeedleAlgo::ConcaveDetectAlgoHelp(BSTR* pHelpStr)
//{
//	CComBSTR strHelp("964$\
//					 397;R;LP;0;10000;1$\
//					 381;B$\
//					 965;FP;1;100;0.1;10$\
//					 966;FP;1;100;0.1;10$\
//					 868;FP;1;100;0.1;10$\
//					 967;FP;LP;1;250;1;10");
//
//	HRESULT hr = strHelp.CopyTo(pHelpStr);
//	if(FAILED(hr))
//		return S_FALSE;
//	return S_OK;
//}

STDMETHODIMP CNeedleAlgo::ConcaveDetectAlgo1(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue,VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area0,row0,col0;
	area_center(rgn,&area0,&row0,&col0);
	if(rgn.Id() == H_EMPTY_REGION||area0.Num()==0||area0==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue = (float)pThresholdValue->fltVal;

	Hobject Image = m_crtImg;
	Hobject ImageROI,JuanYuanPartitioned;
	HTuple PolarTransImageWidth, PolarTransImageHeight;
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
		HTuple Row1, Column1, Phi, Length11, Length21;
		smallest_rectangle2(rgn, &Row1, &Column1, &Phi, &Length11, &Length21);
		//区域校正
		HTuple HomMat2DIdentity,HomMat2DRotate;
		Hobject RegionAffineTrans;
		hom_mat2d_identity(&HomMat2DIdentity);
		hom_mat2d_rotate(HomMat2DIdentity, -Phi, Column1, Row1, &HomMat2DRotate);
		affine_trans_region(rgn, &RegionAffineTrans, HomMat2DRotate, "nearest_neighbor");

		//去除两端的干扰小区域
		Hobject RegionMiddleZ;
		opening_rectangle1(RegionAffineTrans, &RegionMiddleZ, 1, 30);
		Hobject ImageRoi;
		gen_rectangle1(&ImageRoi, 1, 1, m_imgHeight-1, m_imgWidth-1);
		intersection(ImageRoi, RegionMiddleZ, &RegionMiddleZ);

		HTuple area,row,col;
		area_center(RegionMiddleZ,&area,&row,&col);
		if (area==0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		change_domain(Image,RegionMiddleZ,&ImageROI);
		Hobject ImagePart;

		crop_domain(ImageROI,&ImagePart);
		get_image_size(ImagePart, &PolarTransImageWidth, &PolarTransImageHeight);

		if (NumPart>PolarTransImageWidth[0].I())
		{
			NumPart=PolarTransImageWidth[0].I();
		}
		partition_rectangle(ImageROI, &JuanYuanPartitioned, PolarTransImageWidth/NumPart, PolarTransImageHeight);

		Hobject PartitionedSorted;
		HTuple Number;
		sort_region(JuanYuanPartitioned, &PartitionedSorted, "first_point", "true", "column");
		count_obj(PartitionedSorted, &Number);
		Hobject PartitionedNewRegs;
		gen_empty_region(&PartitionedNewRegs);
		Hobject ObjectSelected;
		for (int i=5;i<=Number[0].I()-5;i++)
		{
			select_obj(PartitionedSorted, &ObjectSelected, i);
			concat_obj(PartitionedNewRegs, ObjectSelected, &PartitionedNewRegs);

		}
		HTuple PartitionedArea, PartitionedRow, PartitionedColumn,Length;
		area_center(PartitionedNewRegs, &PartitionedArea, &PartitionedRow, &PartitionedColumn);
		count_obj(PartitionedNewRegs, &Length);

		HTuple MeanDeviation,Exp;
		MeanDeviation=HTuple();
		for (int i=0;i<=Length[0].I()-2;i++)
		{
			//tuple_deviation((PartitionedArea.Select(i)).Concat(PartitionedArea.Select(i+1)), &Exp);
			//MeanDeviation.ReplaceElements(i,Exp);

			tuple_deviation(HTuple(PartitionedArea[i]).Concat(HTuple(PartitionedArea[i+1])), &Exp);
			tuple_abs(Exp, &Exp);
			MeanDeviation.ReplaceElements(i,Exp);
		}
		HTuple Length3,AbsSorted,LocationId,ResultID;
		//tuple_abs(MeanDeviation, &Abs);
		tuple_sort(MeanDeviation, &AbsSorted);
		tuple_sort_index(MeanDeviation, &LocationId);
		tuple_length(MeanDeviation, &Length3);

		for (int i=Length3[0].I()-1; i>=0; i+=-1)
		{
			if (0 != ((AbsSorted.Select(i))<ThresholdValue))
			{
				if (0 != (i==(Length3[0].I()-1)))
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
		HTuple Sorted,Uniq;
		Hobject ResultDefect,DefectRegs;
		tuple_sort(ResultID, &Sorted);
		tuple_uniq(Sorted, &Uniq);
		select_obj(PartitionedNewRegs, &ResultDefect, Uniq);
		union1(ResultDefect, &DefectRegs);

		HTuple ErrorArea, ErrorRow, ErrorColumn;
		intersection(DefectRegs, rgn, &DefectRegs);
		area_center(DefectRegs, &ErrorArea, &ErrorRow, &ErrorColumn);

		//
		if(ErrorArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = ErrorArea[0].I();
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
STDMETHODIMP CNeedleAlgo::ConcaveDetectAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("962$\
					 397;R;LP;0;10000;1$\
					 381;B$\
					 403;LP;5;500;1;300$\
					 963;FP;1;100;0.1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::EndFaceRegionLocationAlgo(VARIANT* rgnId,VARIANT* pMeanSize,VARIANT* pDynThresholdValue,VARIANT* pInnerRadius,VARIANT* pAreaLimitLower,VARIANT* pAreaLimitUpper,VARIANT* pCircularity,VARIANT* pErosionSize)
{
	int id = (int)rgnId->fltVal;
	int MeanSize=(int)pMeanSize->fltVal;
	int DynThresholdValue=(int)pDynThresholdValue->fltVal;
	int InnerRadius=(int)pInnerRadius->fltVal;

	int AreaLimitLower=(int)pAreaLimitLower->fltVal;
	int AreaLimitUpper=(int)pAreaLimitUpper->fltVal;
	float Circularity=(float)pCircularity->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;

	if (AreaLimitLower>=AreaLimitUpper)
	{
		AreaLimitLower=AreaLimitUpper-10;
	}

	HTuple EndRegionRaw,EndRegionCol,EndRegionArea;
	area_center(m_EndRegion,&EndRegionArea,&EndRegionRaw,&EndRegionCol);
	HTuple HeadRegionRaw,HeadRegionCol,HeadRegionArea;
	area_center(m_HeadRegion,&HeadRegionArea,&HeadRegionRaw,&HeadRegionCol);

	Hobject newRgn;
	if((EndRegionArea==0)&&(HeadRegionArea==0))
	{
		Hobject cirle;
		gen_circle(&cirle,m_imgHeight/2.0,m_imgWidth/2.0,115);
		m_vRgn[id] = cirle ;
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
		HTuple Class1, Class2;
		get_obj_class(m_HeadRegion,&Class1);
		get_obj_class(m_EndRegion,&Class2);
		if((Class1!="region")&&(Class1!="region"))
		{
			if(m_isFirstMatchingErrorMessage)
			{
				if(m_isChineseLanguage)
				{
					MessageBox(NULL,_T("调用函数('EndFaceRegionLocationAlgo')前需先调用中心轮廓提取函数('EndCenterLocationAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Call matching function('EndFaceRegionLocationAlgo')before calling location function('EndCenterLocationAlgo')."), _T("Calling function order is wrong!"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				m_isFirstMatchingErrorMessage=false;
			}
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		set_check("~give_error");
		Hobject Image = m_crtImg;
		Hobject Rgn,RegionClosing2;

		//加成员变量清空
		/*HTuple EndRegionRaw,EndRegionCol,EndRegionArea;
		area_center(m_EndRegion,&EndRegionArea,&EndRegionRaw,&EndRegionCol);
		HTuple HeadRegionRaw,HeadRegionCol,HeadRegionArea;
		area_center(m_HeadRegion,&HeadRegionArea,&HeadRegionRaw,&HeadRegionCol);*/

		if (0!=(HeadRegionArea==0))
		{
			HTuple Row, Column, Radius;
			smallest_circle(m_EndRegion, &Row, &Column, &Radius);
			gen_circle(&Rgn, Row, Column, Radius);

			Hobject newRgn;
			m_InnerCircle=newRgn;
		}
		else
		{
			Hobject ImageReduced2;
			reduce_domain(Image, m_HeadRegion, &ImageReduced2);
			Hobject ExpandedImage1,ImageReduced5;
			expand_domain_gray(ImageReduced2, &ExpandedImage1, 5);
			reduce_domain(ExpandedImage1, m_HeadRegion, &ImageReduced5);

			Hobject ImageMean,RegionDynThresh,RegionFillUp2,ConnectedRegions1;
			mean_image(ImageReduced5, &ImageMean, MeanSize, MeanSize);
			dyn_threshold(ImageReduced2, ImageMean, &RegionDynThresh, DynThresholdValue, "dark");
			fill_up(RegionDynThresh, &RegionFillUp2);
			connection(RegionFillUp2, &ConnectedRegions1);

			Hobject RegionOpening2,SelectedRegions2;
			//根据实际图片再调整一下
			opening_circle(ConnectedRegions1, &RegionOpening2, 10);
			select_shape_std(RegionOpening2, &SelectedRegions2, "max_area", 70);
			closing_circle(SelectedRegions2, &RegionClosing2, InnerRadius);

			Hobject SelectedRegions1;
			select_shape(RegionClosing2, &SelectedRegions1, (HTuple("area").Append("circularity")), "and", (HTuple(AreaLimitLower).Append(Circularity)), (HTuple(AreaLimitUpper).Append(1)));
			Hobject ConnectedRegions4;
			connection(SelectedRegions1,&ConnectedRegions4);
			HTuple Number;
			count_obj(ConnectedRegions4,&Number);

			m_InnerCircle=RegionClosing2;
			if (Number[0].I()!=0)
			{
				difference(m_HeadRegion, SelectedRegions1, &Rgn);			
			}
			else
			{
				//Hobject cirle;
				//gen_circle(&cirle,m_imgHeight/2.0,m_imgWidth/2.0,50);
				m_vRgn[id] = m_HeadRegion;
				return S_FALSE;
			}
		}
		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(Rgn, RectangleImage, &Rgn);
		//
		if(ErosionSize>=0.5)
		{
			erosion_circle (Rgn, &Rgn,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(Rgn, &Rgn,abs(ErosionSize));
		}
		else
		{
			Rgn = Rgn;
		}
		m_vRgn[id] = Rgn;
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
STDMETHODIMP CNeedleAlgo::EndFaceRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("974$\
					 167;B$\
					 975;LP;0;100;1;30$\
					 976;LP;0;100;1;5$\
					 977;LP;1;100;1;28$\
					 978;LP;1;5000;1;2000$\
					 993;LP;1;5000;1;3600$\
					 979;FP;0.1;1;0.1;0.8$\
					 443;FP;-20.5;20.5;0.5;1.0"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	m_ringPara[id].m_Radius = (float)pRadius->fltVal;
	m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
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
STDMETHODIMP CNeedleAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CNeedleAlgo::SolidDetectAlgo(VARIANT* rgnId, VARIANT* pDetectArea)
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

	HTuple EndRegionRaw,EndRegionCol,EndRegionArea;
	area_center(m_EndRegion,&EndRegionArea,&EndRegionRaw,&EndRegionCol);
	HTuple InnerCircleArea,InnerCircleRaw,InnerCircleCol;
	area_center(m_InnerCircle,&InnerCircleArea,&InnerCircleRaw,&InnerCircleCol);
	HTuple HeadRegionRaw,HeadRegionCol,HeadRegionArea;
	area_center(m_HeadRegion,&HeadRegionArea,&HeadRegionRaw,&HeadRegionCol);

	Hobject newRgn;
	if((InnerCircleArea==0)&&(HeadRegionArea!=0))
	{
		retValue = 255;
		Hobject cirle;
		gen_circle(&cirle,m_imgHeight/2.0,m_imgWidth/2.0,50);
		retValue.Detach(pDetectArea);
		m_vErrorRgn[rId] = cirle;
		return S_FALSE;
	}
	else if((InnerCircleArea==0)&&(HeadRegionArea==0)&&(EndRegionArea==0))
	{
		retValue = -1;
		Hobject cirle;
		gen_circle(&cirle,m_imgHeight/2.0,m_imgWidth/2.0,50);
		retValue.Detach(pDetectArea);
		m_vErrorRgn[rId] = cirle;
		return S_FALSE;
	}
	else if(m_InnerCircle.Id() == H_EMPTY_REGION)
	{
		retValue = 0.0;
		Hobject newRgn;
		m_vErrorRgn[rId] = newRgn;
		retValue.Detach(pDetectArea);
		return S_OK;
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
		Hobject DetectedRegion;
		erosion_circle(m_InnerCircle,&DetectedRegion,5);
		Hobject Image = m_crtImg;
		HTuple GrayMean,GrayDeviation;
		intensity(DetectedRegion,Image,&GrayMean,&GrayDeviation);

		Hobject ErrorRegs;
		retValue = GrayMean[0].I();
		dilation_circle (DetectedRegion, &ErrorRegs, 2.5);
		m_vErrorRgn[rId] = ErrorRegs;	

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
STDMETHODIMP CNeedleAlgo::SolidDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("980$\
					 397;R;LP;0;1000;1$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::EccentricityDetectAlgo(VARIANT* rgnId, VARIANT* pDetectArea)
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

	Hobject newRgn;
	if((m_InnerCircle.Id() == H_EMPTY_REGION)||(m_HeadRegion.Id() == H_EMPTY_REGION))
	{
		retValue = 0.0;
		Hobject newRgn;
		m_vErrorRgn[rId] = newRgn;
		return S_OK;
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
		HTuple InnerCircleRow, InnerCircleCol, InnerCircleRadius;
		HTuple OutCircleRow, OutCircleCol, OutCircleRadius;
		Hobject ErrorRng;
		smallest_circle(m_InnerCircle, &InnerCircleRow, &InnerCircleCol, &InnerCircleRadius);
		smallest_circle(m_HeadRegion, &OutCircleRow, &OutCircleCol, &OutCircleRadius);
		HTuple DistDiff;
		distance_pp(InnerCircleRow, InnerCircleCol, OutCircleRow, OutCircleCol, &DistDiff);
		difference(m_HeadRegion,m_InnerCircle,&ErrorRng);

		retValue = DistDiff[0].D();
		m_vErrorRgn[rId] = ErrorRng;	

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
STDMETHODIMP CNeedleAlgo::EccentricityDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("985$\
					 397;R;FP;0;1000;1$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//
//STDMETHODIMP CNeedleAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
//{
//	int rId = (int)rgnId->fltVal;
//	m_vErrorRgn[rId].Reset();
//	Hobject rgn = m_vRgn[rId];
//	CComVariant retValue;
//
//	HTuple area,row,col;
//	area_center(rgn,&area,&row,&col);
//	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
//	{
//		retValue = -1;
//		m_vErrorRgn[rId].Reset();
//		retValue.Detach(pDetectArea);
//		return S_FALSE;
//	}
//
//	int ThresholdValue = (int)pDynThresh->fltVal;
//	int DefectSize = (int) pDefectSize->fltVal;
//	float RotaAngle = (float)pRotaAngle->fltVal;
//#ifdef NDEBUG
//	try
//	{
//#endif
//		//
//		if(m_bPilotImg)
//		{
//			return S_OK;
//		}
//		if(m_bDebugImg)
//		{
//			m_dictSpecialData[rId].clear();
//		}
//		//
//		HTuple HomMat2D;
//		Hobject Image = m_crtImg;
//		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
//		//
//		float fCenterX = 0.0;
//		float fCenterY = 0.0;
//		fCenterX = m_vPos[0].m_x;
//		fCenterY = m_vPos[0].m_y;
//		reduce_domain (Image, rgn, &ImageReduced);
//		vector_angle_to_rigid(fCenterY,fCenterX, 0, fCenterY, fCenterX, HTuple(RotaAngle).Rad(), &HomMat2D);
//		affine_trans_image(ImageReduced, &ImageAffinTrans, HomMat2D, "bilinear", "false");
//		dyn_threshold(ImageReduced, ImageAffinTrans, &RegionDynThresh, ThresholdValue, "not_equal");
//		connection(RegionDynThresh, &ConnectedRegions4);
//		select_shape(ConnectedRegions4, &SelectedRegions4, "area", "and", DefectSize, 9999999);
//		Hobject DefectRegs;
//		union1(SelectedRegions4, &DefectRegs);
//		HTuple Area, Row_test, Column_test;
//		area_center(DefectRegs, &Area, &Row_test, &Column_test);
//		if(Area <= 0)
//		{
//			retValue = 0.0;
//			Hobject newRgn; 
//			m_vErrorRgn[rId] = newRgn;
//		}
//		else
//		{
//			retValue = Area[0].I();
//			dilation_circle (DefectRegs, &DefectRegs, 2.5);
//			m_vErrorRgn[rId] = DefectRegs;
//		}
//		retValue.Detach(pDetectArea);
//#ifdef NDEBUG
//	}
//	catch (HException &except)  
//	{
//		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
//		retValue=-1;
//		m_vErrorRgn[rId].Reset();
//		retValue.Detach(pDetectArea);
//		return S_FALSE;
//	}
//#endif
//	return S_OK;
//}
//STDMETHODIMP CNeedleAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
//{
//	CComBSTR strHelp("609$\
//					 397;R;LP;0;1000;1$\
//					 381;B$\
//					 610;FP;0.5;130;0.5;3.5$\
//					 630;LP;1;250;1;60$\
//					 387;LP;1;200;1;10");
//	HRESULT hr = strHelp.CopyTo(pHelpStr);
//	if(FAILED(hr))
//		return S_FALSE;
//	return S_OK;
//}

STDMETHODIMP CNeedleAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
		if(m_bPilotImg)
		{
			return S_OK;
		}
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
STDMETHODIMP CNeedleAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CNeedleAlgo::EndDynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||m_HeadRegion.Id()==H_EMPTY_REGION||area.Num()==0||area==0)
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
		HTuple HeadRegionRaw,HeadRegionCol,HeadRegionArea;
		area_center(m_HeadRegion,&HeadRegionArea,&HeadRegionRaw,&HeadRegionCol);
		
		if (0!=(HeadRegionArea==0))
		{
			//**尾部检测区域时，采用动态灰度阈值算法
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
			union2(BlackUnion, WhiteUnion, &DefectRegs);
			//**
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
STDMETHODIMP CNeedleAlgo::EndDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("982$\
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

STDMETHODIMP CNeedleAlgo::EndDynThresholdAlgo1(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||m_EndRegion.Id()==H_EMPTY_REGION||area.Num()==0||area==0)
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
		HTuple EndRegionRaw,EndRegionCol,EndRegionArea;
		area_center(m_EndRegion,&EndRegionArea,&EndRegionRaw,&EndRegionCol);
		if (0!=(EndRegionArea==0))
		{
			//**头部检测区域时，采用动态灰度阈值算法，目的是与尾部检测参数调节分开
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
			union2(BlackUnion, WhiteUnion, &DefectRegs);
			//**
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
STDMETHODIMP CNeedleAlgo::EndDynThresholdAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("1123$\
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

void shift_height_sub_image (Hobject Image, Hobject *ColEdgeImage, HTuple ShiftSize, HTuple BlackOrWhite, HTuple Mult)
{

	// Local iconic variables 
	Hobject  ImageShift, Rectangle, RectangleR;


	// Local control variables 
	HTuple  Width1, Height1, Rows, Columns;
	HTuple  Grayval, RowDown, RowsR, ColumnsR, GrayvalR;
	HTuple  RowUp;

	gen_empty_obj(&(*ColEdgeImage));
	get_image_size(Image, &Width1, &Height1);
	gen_image_const(&ImageShift, "byte", Width1, Height1);

	gen_rectangle1(&Rectangle, 0, 0, (Height1/2.0)+1, Width1-1);

	get_region_points(Rectangle, &Rows, &Columns);

	get_grayval(Image, Rows, Columns, &Grayval);
	RowDown = Rows+(ShiftSize.Round());

	set_grayval(ImageShift, RowDown, Columns, Grayval);
	//
	gen_rectangle1(&RectangleR, Height1/2.0, 0, Height1-1, Width1-1);

	get_region_points(RectangleR, &RowsR, &ColumnsR);

	get_grayval(Image, RowsR, ColumnsR, &GrayvalR);
	RowUp = RowsR-(ShiftSize.Round());

	set_grayval(ImageShift, RowUp, ColumnsR, GrayvalR);
	if (0 != (BlackOrWhite>0))
	{
		sub_image(Image, ImageShift, &(*ColEdgeImage), Mult, 0);
	}
	else
	{
		sub_image(ImageShift, Image, &(*ColEdgeImage), Mult, 0);
	}
}
STDMETHODIMP CNeedleAlgo::EndOpeningGapDetectAlgo(VARIANT* rgnId,VARIANT* pWhiteOrBlack,VARIANT* pThresholdValue,VARIANT* pWidthMinSize, VARIANT* pWidthMaxSize,VARIANT* pHeightMinSize, VARIANT* pHeightMaxSize,  VARIANT* pDetectWidth)
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
		retValue.Detach(pDetectWidth);
		return S_FALSE;
	}
	int WhiteOrBlack = (int) pWhiteOrBlack->fltVal;
	int ThresholdValue = (int) pThresholdValue->fltVal;
	int WidthMinSize = (int)pWidthMinSize->fltVal;
	int WidthMaxSize = (int) pWidthMaxSize->fltVal;
	int HeightMinSize = (int) pHeightMinSize->fltVal;
	int HeightMaxSize = (int) pHeightMaxSize->fltVal;

	if (WidthMinSize>=WidthMaxSize)
	{
		WidthMinSize=WidthMaxSize-1;
	}
	if (HeightMinSize>=HeightMaxSize)
	{
		HeightMinSize=HeightMaxSize-1;
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
		HTuple MiddleRegionRaw,MiddleRegionCol,MiddleRegionArea;
		area_center(m_MiddleRegion,&MiddleRegionArea,&MiddleRegionRaw,&MiddleRegionCol);		
		if (MiddleRegionArea>0)
		{
			// Local iconic variables 
			Hobject  RegionHeadBig, RegionMiddle;
			Hobject  RegionDilation1, RegionMiddleCut, ImageAffinTrans;
			Hobject  RegionAffineTransBig, RegionDilation, ImageReduced2;
			Hobject  ImagePart, ImageEmphasize, DiffOfGauss;
			Hobject  ImageConverted, ImageSub, ColEdgeImage;
			Hobject  ImageMax, Region1, RegionMoved, RegionClosing4;
			Hobject  ConnectedRegions1, SelectedRegions2, RegionHeadEdgeTrans;
			Hobject  Rectangle4, Partitioned, RegionIntersection2;
			Hobject  RectangleDist, RegionUnion1, RegionClosing1;
			Hobject  RegionOpening2, RectangleNor, RegionDilation2;
			Hobject  RegionDifference1, RegionClosing2, RegionOpening3;
			Hobject  ConnectedRegions2, RegionAffineTrans2, RegionAffineTrans3;
			Hobject  RectangleBig, RectangleNorDiff, RegionAffineTrans;
			Hobject  RegionAffineTrans1;


			// Local control variables 
			HTuple  Phi1, HomMat2D, HomMat2DInv, Row11;
			HTuple  Column1, Row21, Column2, Row12, Column11;
			HTuple  Row22, Column21, Row13, Column12, Row23;
			HTuple  Column22, Row14, Column13, Row24, Column23;
			HTuple  Number1, Row16, Column15, Row26, Column25;
			HTuple  Row15, Column14, Row25, Column24;
			//
			RegionHeadBig=rgn;
			RegionMiddle=m_MiddleRegion;
			//*校正提前！角度波动太大
			//切除一部分 减少弯曲对角度计算的影响
			dilation_rectangle1(RegionHeadBig, &RegionDilation1, 190, 50);
			intersection(RegionMiddle, RegionDilation1, &RegionMiddleCut);
			//判断区域是否存在
			HTuple CutRegionRaw,CutRegionCol,CutRegionArea;
			area_center(RegionMiddleCut,&CutRegionArea,&CutRegionRaw,&CutRegionCol);	
			if (CutRegionArea<60)
			{
				retValue = 0.0; 
				m_vErrorRgn[rId] = RegionMiddle;
				retValue.Detach(pDetectWidth);
				return S_FALSE;
			}
			//
			orientation_region(RegionMiddleCut, &Phi1);
			HTuple  Width,Height;
			get_image_size(Image, &Width, &Height);
			vector_angle_to_rigid(Height/2.0, Width/2.0, Phi1, Height/2.0, Width/2.0, 0, &HomMat2D);
			vector_angle_to_rigid(Height/2.0, Width/2.0, 0, Height/2.0, Width/2.0, Phi1, &HomMat2DInv);
			affine_trans_image(Image, &ImageAffinTrans, HomMat2D, "constant", "false");
			affine_trans_region(RegionHeadBig, &RegionAffineTransBig, HomMat2D, "nearest_neighbor");
			//大头个别提取不完全，增加膨胀尺寸，11,21
			dilation_rectangle1(RegionAffineTransBig, &RegionDilation, 21, 11);
			//*切割图片，提取边缘，提取边缘区域后移动到对应位置
			smallest_rectangle1(RegionDilation, &Row11, &Column1, &Row21, &Column2);
			reduce_domain(ImageAffinTrans, RegionDilation, &ImageReduced2);
			crop_domain(ImageReduced2, &ImagePart);
			emphasize(ImagePart, &ImageEmphasize, 5, 5, 1);
			diff_of_gauss(ImageEmphasize, &DiffOfGauss, 1, 16);
			convert_image_type(DiffOfGauss, &ImageConverted, "byte");
			sub_image(ImagePart, ImageConverted, &ImageSub, 1.5, 0);
			
			shift_height_sub_image(ImageSub, &ColEdgeImage, 2, WhiteOrBlack, 2.5);
			gray_dilation_rect(ColEdgeImage, &ImageMax, 1, 3);
			if ((ThresholdValue<255)&(ThresholdValue>=0))
			{
				threshold(ImageMax, &Region1, ThresholdValue, 255);
			}
			else
			{
				threshold(ImageMax, &Region1, 120, 255);
			}
			move_region(Region1, &RegionMoved, Row11, Column1);
			//***
			//连接小豁口
			closing_rectangle1(RegionMoved, &RegionClosing4, 5, 3);
			connection(RegionClosing4, &ConnectedRegions1);
			select_shape(ConnectedRegions1, &SelectedRegions2, "width", "and", WidthMinSize-3, 99999);
			union1(SelectedRegions2, &RegionHeadEdgeTrans);
			//判断区域是否存在
			HTuple EdgeRegionRaw,EdgeRegionCol,EdgeRegionArea;
			area_center(RegionHeadEdgeTrans,&EdgeRegionArea,&EdgeRegionRaw,&EdgeRegionCol);	
			if (EdgeRegionArea<60)
			{
				retValue = 0.0; 
				m_vErrorRgn[rId] = rgn;
				retValue.Detach(pDetectWidth);
				return S_FALSE;
			}
			//***********************
			smallest_rectangle1(RegionHeadEdgeTrans, &Row12, &Column11, &Row22, &Column21);
			gen_rectangle1(&Rectangle4, Row12-10, Column11.Round(), Row22+10, Column21.Round());
			partition_rectangle(Rectangle4, &Partitioned, 1, (Row22-Row12)+40);
			intersection(Partitioned, RegionHeadEdgeTrans, &RegionIntersection2);
			smallest_rectangle1(RegionIntersection2, &Row13, &Column12, &Row23, &Column22);
			gen_rectangle1(&RectangleDist, Row13, Column12, Row23, Column22);
			union1(RectangleDist, &RegionUnion1);
			//小豁口 3,1；5,1
			closing_rectangle1(RegionUnion1, &RegionClosing1, 9, 1);
			opening_rectangle1(RegionClosing1, &RegionOpening2, 5, 21);
			//判断区域是否存在
			HTuple Opening2RegionRaw,Opening2RegionCol,Opening2RegionArea;
			area_center(RegionOpening2,&Opening2RegionArea,&Opening2RegionRaw,&Opening2RegionCol);	
			if (Opening2RegionArea<60)
			{
				retValue = 0.0; 
				m_vErrorRgn[rId] = rgn;
				retValue.Detach(pDetectWidth);
				return S_FALSE;
			}
			//*region test
			inner_rectangle1(RegionOpening2, &Row14, &Column13, &Row24, &Column23);
			gen_rectangle1(&RectangleNor, Row14, Column13, Row24, Column23);
			//HeightDefine := Row24-Row14
			//膨胀后 再做差，避免左右两边有区域
			dilation_rectangle1(RectangleNor, &RegionDilation2, 31, 1);
			difference(RegionOpening2, RegionDilation2, &RegionDifference1);
			//避免小豁口*5,3;7,5
			closing_rectangle1(RegionDifference1, &RegionClosing2, 7, 3);
			opening_rectangle1(RegionClosing2, &RegionOpening3, 5, HeightMinSize-1);
			connection(RegionOpening3, &ConnectedRegions2);
			//*屏蔽个别小干扰点
			select_shape(ConnectedRegions2, &ConnectedRegions2,(HTuple("width").Append("height")),"and", (HTuple(WidthMinSize-1).Append(HeightMinSize-1)), (HTuple(WidthMaxSize+3).Append(HeightMaxSize+3)));
			union1(ConnectedRegions2, &RegionOpening3);
			//*
			count_obj(ConnectedRegions2, &Number1);
			affine_trans_region(RegionOpening2, &RegionAffineTrans2, HomMat2DInv, "nearest_neighbor");
			if (0 != (Number1==1))
			{
				//affine_trans_region(RectangleNor, &RegionAffineTrans3, HomMat2DInv, "nearest_neighbor");
				retValue = 0.0;
				m_vErrorRgn[rId] = RegionAffineTrans2;
			}
			else if (0 != (Number1==2))
			{
				//smallest_rectangle1(ConnectedRegions2, &Row16, &Column15, &Row26, &Column25);
				smallest_rectangle1(RegionOpening3, &Row15, &Column14, &Row25, &Column24);
				gen_rectangle1(&RectangleBig, Row15, Column14, Row25, Column24);
				difference(RectangleNor, RectangleBig, &RectangleNorDiff);
				affine_trans_region(RectangleNorDiff, &RegionAffineTrans, HomMat2DInv, "nearest_neighbor");
				affine_trans_region(RectangleBig, &RegionAffineTrans1, HomMat2DInv, "nearest_neighbor");
				//WidthMinHead = (((Column25.Select(0))-(Column15.Select(0))).Abs()).Min2(((Column25.Select(1))-(Column15.Select(1))).Abs());
				//HeightMaxHead = (((Row26.Select(0))-(Row16.Select(0))).Abs()).Max2(((Row26.Select(1))-(Row16.Select(1))).Abs());
				// stop(); only in hdevelop
				Hobject RectangleUnion;
				union2(RegionAffineTrans,RegionAffineTrans1,&RectangleUnion);
				retValue =Column24[0].D()-Column14[0].D();
				m_vErrorRgn[rId] = RectangleUnion;
			}
			else
			{
				retValue = 0.0;
				m_vErrorRgn[rId] = RegionAffineTrans2;
			}
		}
		else
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		retValue.Detach(pDetectWidth);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectWidth);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CNeedleAlgo::EndOpeningGapDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("875$\
					 881;R;LP;0;200;1$\
					 381;B$\
					 876;LP;-1;1;2;1$\
					 749;LP;5;254;1;120$\
					 877;LP;5;80;1;7$\
					 878;LP;10;160;1;30$\
					 879;LP;2;60;1;5$\
					 880;LP;5;120;1;20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||m_EndRegion.Id()==H_EMPTY_REGION||area.Num()==0||area==0)
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
		HTuple EndRegionRaw,EndRegionCol,EndRegionArea;
		area_center(m_EndRegion,&EndRegionArea,&EndRegionRaw,&EndRegionCol);

		//
		HTuple HomMat2D;
		Hobject Image = m_crtImg;
		if (0!=(EndRegionArea==0))
		{
			//头部检测区域时，旋转对比算法
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
STDMETHODIMP CNeedleAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1123$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 610;FP;0.5;130;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::ExtraMaterialRegionLocationAlgo(VARIANT* rgnId,VARIANT* pInnerEdge,VARIANT* pOutEdge,VARIANT* pBeltEdge)
{
	int id = (int)rgnId->fltVal;
	float InnerEdge = (float)pInnerEdge->fltVal;
	float OutEdge = (float)pOutEdge->fltVal;
	float BeltEdge = (float)pBeltEdge->fltVal;

	if (OutEdge<=InnerEdge)
	{
		OutEdge=InnerEdge+5;
	}

	Hobject newRgn;
	if(m_HeadRegion.Id() == H_EMPTY_REGION||m_RegionOpening.Id() == H_EMPTY_REGION)
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	HTuple Area1,Row1,Col1,Area2,Row2,Col2;
	area_center(m_RegionOpening,&Area1,&Row1,&Col1);
	area_center(m_HeadRegion,&Area2,&Row2,&Col2);
	if(Area1.Num()==0||Area1==0||Area2.Num()==0||Area2==0)
	{
		m_vRgn[id] = newRgn ;
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
			m_dictSpecialData[id].clear();
		}
		erosion_circle(m_RegionOpening, &m_RegionOpening, BeltEdge);

		Hobject ROIDilation,RegionDiff,RegionOpening3;
		dilation_circle(m_HeadRegion, &ROIDilation, InnerEdge);
		difference(m_RegionOpening, ROIDilation, &RegionDiff);
		opening_circle(RegionDiff, &RegionOpening3, 3.5);

		Hobject ConnectedRegions5,SelectedRegions5;
		connection(RegionOpening3, &ConnectedRegions5);
		select_shape_std(ConnectedRegions5, &SelectedRegions5, "max_area", 70);

		Hobject ROIDilation1;
		dilation_circle(m_HeadRegion, &ROIDilation1, OutEdge);
		Hobject  RegionDetect;
		intersection(ROIDilation1, SelectedRegions5, &RegionDetect);

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
STDMETHODIMP CNeedleAlgo::ExtraMaterialRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1026$\
					 167;B$\
					 1027;FP;1;30;0.5;3.5$\
					 1028;LP;1;80;1;40$\
					 1029;LP;1;30;1;10"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNeedleAlgo::ExtraMaterialDetectAlgo(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = 0;
		Hobject newRgn; 
		m_vErrorRgn[rId] = newRgn;
		retValue.Detach(pDetectArea);
		return S_OK;
	}

	int WhiteMaskSize = (int) pWhiteMaskSize->fltVal;
	float SeriousWhitePointDynThresh = (float) pSeriousWhitePointDynThresh->fltVal;
	int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;
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
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean,ExpandedImage,ImageDetectNew;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//

		ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
		expand_domain_gray (ImageReduced, &ExpandedImage, 25);
		reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
		//
		mean_image(ImageDetectNew, &ImageReduceMean, WhiteMaskSize, WhiteMaskSize);

		ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);

		HTuple Area, Row_test, Column_test;

		area_center(WhiteUnion, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
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
STDMETHODIMP CNeedleAlgo::ExtraMaterialDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1030$\
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



