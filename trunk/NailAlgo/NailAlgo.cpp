#include "stdafx.h"
#include "resource.h"
#include "NailAlgo_i.h"
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

// NailAlgo.cpp:
#include "stdafx.h"
#include "NailAlgo.h"

STDMETHODIMP CNailAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_INailAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CNailAlgo::SetCurrentImage(LONG* pImg)
{
	m_imageNumber++;
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
		m_dictCrtPathModelID.clear();
		m_dictCrtPathModelID = m_dictNewPathModelID;

		m_dictCrtPathRgn.clear();
		m_dictCrtPathRgn = m_dictNewPathRgn;
		m_bFileUpdated = false;
	}

	return S_OK;
}
STDMETHODIMP CNailAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CNailAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CNailAlgo::GetGlueInjectorID(LONG* pInjectorID)
{
	*pInjectorID = m_nCrtInjectorID;
	return S_OK;
}
STDMETHODIMP CNailAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CNailAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CNailAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{
	return S_OK;
}
STDMETHODIMP CNailAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{
	
	return S_FALSE;
}
STDMETHODIMP CNailAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}
STDMETHODIMP CNailAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}
STDMETHODIMP CNailAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{

	return S_OK;
}
STDMETHODIMP CNailAlgo::ResetModels(BYTE productID)
{

	return S_OK;
}
STDMETHODIMP CNailAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{

	return S_OK;
}
STDMETHODIMP CNailAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{
	
	return S_OK;
}
STDMETHODIMP CNailAlgo::ModelCreate(double Radius, HTuple * ModelID1)
{

	return S_OK;
}
STDMETHODIMP CNailAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CNailAlgo::ReloadExternalFile(BSTR* bstrPath)
{
	return S_OK;
}
STDMETHODIMP CNailAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CNailAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

//NailAlgo 
STDMETHODIMP CNailAlgo::CenterRegionExtractAlgo(VARIANT* rgnID ,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight,VARIANT* pThredLow,VARIANT* pThredUpper,VARIANT* pThredValue)
{
	int id = (int)rgnID->fltVal; 
	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;
	int ThredLow=(int)pThredLow->fltVal;
	int ThredUpper=(int)pThredUpper->fltVal;
	int ThredValue=(int)pThredValue->fltVal;
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
		if (m_bPilotImg)
		{
			return S_OK;
		}
		Hobject  Image = m_crtImg;
		HTuple   ImageWidth,ImageHeight;
		get_image_size(Image,&ImageWidth,&ImageHeight);	
		m_imgWidth  = (long)ImageWidth[0].I();
		m_imgHeight = (long)ImageHeight[0].I();
		//判断初始化图片
		if ((ImageWidth==659 && ImageHeight==475)||(ImageWidth==659 && ImageHeight==494))
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
		//判断搜索框的上下边缘是否有螺钉
		if (RegWidth<=(ColStart+270))
		{
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			m_vRgn[id] = newRgn ;
			m_DetectReg= newRgn ;
			return S_FALSE;
		}
		Hobject UpRect,LowRect;
		gen_rectangle1(&UpRect, RowStart, ColStart+270, RowStart+4, RegWidth);
		gen_rectangle1(&LowRect, (RowStart+RegHeight)-4, ColStart+270, RowStart+RegHeight,RegWidth);

		Hobject UpRectImageReduced,ImagePart,LowRectImageReduced,ImagePart1,RegionDynThresh,RegionUnion,RegionMoved;
		reduce_domain(Image, UpRect, &UpRectImageReduced);
		crop_domain(UpRectImageReduced, &ImagePart);
		reduce_domain(Image, LowRect, &LowRectImageReduced);
		crop_domain(LowRectImageReduced, &ImagePart1);
		dyn_threshold(ImagePart, ImagePart1, &RegionDynThresh, 100, "not_equal");
		union1(RegionDynThresh, &RegionUnion);
		HTuple AreaB, RowB, ColumnB;
		move_region(RegionUnion,&RegionMoved,  RowStart, ColStart+270);
		area_center(RegionMoved, &AreaB, &RowB, &ColumnB);
		if (AreaB>10)
		{
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			Hobject Rgn;
			gen_circle(&Rgn,m_vPos[0].m_y,m_vPos[0].m_x,100);
			m_vRgn[id] = Rgn ;
			m_DetectReg= newRgn ;
			return S_FALSE;
		}

		Hobject SearchRgn,ImageReduced;
		gen_rectangle2(&SearchRgn,RowStart+RegHeight/2,ColStart+RegWidth/2,0,RegWidth/2,RegHeight/2);
		HTuple rectTopY, rectTopX,rectBottomY,rectBottomX;
		smallest_rectangle1(SearchRgn,&rectTopY, &rectTopX,&rectBottomY,&rectBottomX);
		//m_rectTopX=rectTopX;
		//m_rectTopY=rectTopY;
		//rectBottomY=rectTopY+(((rectBottomY-rectTopY)/4).Round())*4;
		//rectBottomX=rectTopX+(((rectBottomX-rectTopX)/4).Round())*4;
		reduce_domain(Image,SearchRgn,&ImageReduced);
		//
		Hobject DiffGauss,ImageConted,ImageEnhanced;
		diff_of_gauss(ImageReduced,&DiffGauss,1,10);
		convert_image_type(DiffGauss,&ImageConted,"byte");
		abs_diff_image(ImageReduced,ImageConted,&ImageEnhanced,1);
		Hobject ImageEnhancedMedian;
		median_image(ImageEnhanced,&ImageEnhancedMedian,"circle",1,"mirrored");
		Hobject ExpandedImage,ImageReduced1,ModelImages,ModelRegions;
		Hobject RegionDilation,RegionFillUp;
		expand_domain_gray(ImageEnhancedMedian,&ExpandedImage,2);
		reduce_domain(ExpandedImage,SearchRgn,&ImageReduced1);
		inspect_shape_model(ImageReduced1,&ModelImages,&ModelRegions,1,HTuple(ThredLow).Append(HTuple(ThredUpper)));
		dilation_circle(ModelRegions,&RegionDilation,4);
		fill_up(RegionDilation,&RegionFillUp);

		Hobject RegionComplement,RegionIntersection,connectedRegions,SelectRegion,RegionDiff,SelectRegion1,connectedRegions1;
		complement(RegionFillUp,&RegionComplement);
		intersection(RegionComplement,SearchRgn,&RegionIntersection);
		connection(RegionIntersection,&connectedRegions);
		select_shape_std(connectedRegions,&SelectRegion,"max_area",70);
		difference(SearchRgn,SelectRegion,&RegionDiff);
		connection(RegionDiff,&connectedRegions1);
		select_shape_std(connectedRegions1,&SelectRegion1,"max_area",70);
		HTuple CenterArea,CenterRow,CenterCol;
		area_center(SelectRegion1,&CenterArea,&CenterRow,&CenterCol);
		Hobject DetectReg;
		erosion_circle(SelectRegion1,&DetectReg,4);
		connection(DetectReg,&DetectReg);
		select_shape_std(DetectReg,&DetectReg,"max_area",70);
		//
		Hobject RegionOpening2;
		opening_rectangle1(DetectReg, &RegionOpening2, 1, 60);
		//判断螺钉有无（白黑图）
		Hobject RegionErosion2,RegionDifference2,RegionIntersection1,ConnectedRegions4,SelectedRegions3;
		HTuple Row5, Column5, Phi1, Length11, Length21;
		dilation_rectangle1(RegionOpening2, &RegionErosion2, 12, 12);
		difference(SelectRegion1, RegionErosion2, &RegionDifference2);
		intersection(RegionDifference2, SearchRgn, &RegionIntersection1);
		connection(RegionIntersection1, &ConnectedRegions4);
		select_shape_std(ConnectedRegions4, &SelectedRegions3, "max_area", 70);
		HTuple AreaH,RowH,ColH;
		area_center(SelectedRegions3,&AreaH,&RowH,&ColH);

		if ((ColH<250)&&(AreaH<2000))
		{
			m_vPos[0].m_x = (float)(m_imgWidth/2.0);
			m_vPos[0].m_y = (float)(m_imgHeight/2.0);
			Hobject Rgn;
			gen_circle(&Rgn,m_vPos[0].m_y,m_vPos[0].m_x,100);
			m_vRgn[id] = Rgn ;
			m_DetectReg= newRgn ;
			return S_FALSE;
		}

		//剔除钉子背景的干扰区域
		Hobject ImageReduced2,Regions,RegionOpening3,RegionDilation5,RegionDifference3,ConnectedRegions6,SelectedRegions5;
		reduce_domain(Image, RegionOpening2, &ImageReduced2);
		threshold(ImageReduced2, &Regions, ThredValue, 255);
		opening_rectangle1(Regions, &RegionOpening3, 22, 1);
		dilation_rectangle1(RegionOpening3, &RegionDilation5, 60, 1);
		difference(DetectReg, RegionDilation5, &RegionDifference3);
		connection(RegionDifference3, &ConnectedRegions6);
		select_shape_std(ConnectedRegions6, &SelectedRegions5, "max_area", 70);

		if(m_bDebugImg)
		{

			HTuple HomMat2DIdentity,HomMat2DScale;
			hom_mat2d_identity(&HomMat2DIdentity);
			hom_mat2d_scale(HomMat2DIdentity, 1, 1, 0, 0, &HomMat2DScale);
			Hobject RegionAffineTrans;
			affine_trans_region(SelectedRegions5, &RegionAffineTrans, HomMat2DScale, "nearest_neighbor");
			move_region(RegionAffineTrans, &RegionClosingMoved, 0, 0);

			Hobject RegionMargin;
			rectBottomY=rectTopY+(((rectBottomY-rectTopY)/4).Round())*4;
			rectBottomX=rectTopX+(((rectBottomX-rectTopX)/4).Round())*4;
			gen_region_polygon(&RegionMargin, ((((HTuple(rectTopY).Append(rectTopY)).Append(rectBottomY)).Append(rectBottomY)).Append(rectTopY)), 
				((((HTuple(rectTopX).Append(rectBottomX)).Append(rectBottomX)).Append(rectTopX)).Append(rectTopX)));
			concat_obj (RegionMargin, RegionClosingMoved, &RegionClosingMoved);
		}

		m_DetectReg=SelectedRegions5;
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
STDMETHODIMP CNailAlgo::CenterRegionExtractAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("922$\
					 167;B$\
					 939;LP;20;200;1;45$\
					 938;LP;34;800;1;35$\
					 940;LP;1;1600;1;1530$\
					 941;LP;1;450;1;260$\
					 942;LP;0;255;1;15$\
					 943;LP;0;255;1;30$\
					 857;LP;1;254;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNailAlgo::DetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pNum,VARIANT* pOpeningSize,VARIANT* pClosingSize)
{
	int id = (int)rgnId->fltVal;
	int Num=(int)pNum->fltVal;
	int OpeningSize=(int)pOpeningSize->fltVal;
	int ClosingSize=(int)pClosingSize->fltVal;

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
					MessageBox(NULL,_T("调用函数('DetectRegionLocationAlgo')前需先调用轮廓提取函数('CenterRegionExtractAlgo')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
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
		Hobject PartitionedRegs,SortedRegions,EmptyObject;
		HTuple Number;
		partition_rectangle(m_DetectReg,&PartitionedRegs,1,500);
		sort_region(PartitionedRegs, &SortedRegions, "character", "true", "column");
		count_obj(SortedRegions, &Number);
		gen_empty_obj(&EmptyObject);
		int i;
		Hobject ObjectSelected;
		for (i=(Number[0].I()-Num);i<=Number[0].I();i++)
		{
			select_obj(SortedRegions, &ObjectSelected, i);
			union2(ObjectSelected, EmptyObject, &EmptyObject);
		}
		Hobject DetectRgn;
		difference(m_DetectReg, EmptyObject, &DetectRgn);
		Hobject RegionOpening,RegionClosing,ConnectedRegions,SelectedRegions;
		opening_rectangle1(DetectRgn, &RegionOpening, OpeningSize, 1);
		closing_rectangle1(RegionOpening, &RegionClosing, ClosingSize, 2);
		connection(RegionClosing, &ConnectedRegions);
		select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);

		Hobject RectangleImage;
		gen_rectangle1(&RectangleImage, 0,0, m_imgHeight-1, m_imgWidth-1);
		intersection(SelectedRegions, RectangleImage, &RegionDetect);
		m_vRgn[id] = RegionDetect ;	
		m_detectRoi=DetectRgn;
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
STDMETHODIMP CNailAlgo::DetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("944$\
					 167;B$\
					 945;LP;0;200;1;80$\
					 946;LP;1;200;1;35$\
					 947;LP;1;200;1;35"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNailAlgo::CurveDetectAlgo(VARIANT* rgnId, VARIANT* pDetectArea)
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
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject PartitionedB,SortedRegions1,RegionUnion1;
		HTuple Number;
		partition_rectangle(m_detectRoi, &PartitionedB, 1, 500);
		sort_region(PartitionedB, &SortedRegions1, "upper_left", "true", "column");
		count_obj(SortedRegions1, &Number);
		gen_empty_obj(&RegionUnion1);
		HTuple Area_StartRoi, Row_StartRoi, Column_StartRoi;
		Hobject StartRgn,StartRoi;
		for (int a=1; a<=50; a+=1)
		{
			select_obj(SortedRegions1, &StartRgn, a);
			union2(StartRgn, RegionUnion1, &RegionUnion1);
		}
		union1(RegionUnion1, &StartRoi);
		area_center(StartRoi, &Area_StartRoi, &Row_StartRoi, &Column_StartRoi);
		HTuple Row11, Column11, Row2, Column2,StartColSize;
		smallest_rectangle1(StartRoi, &Row11, &Column11, &Row2, &Column2);
		StartColSize = (Row2-Row11).Abs();

		Hobject RegionUnion2,MidRgn,MidRoi;
		HTuple Area_MidRoi, Row_MidRoi, Column_MidRoi;
		gen_empty_obj(&RegionUnion2);
		for (int b=Number[0].I()/2;b<=(Number[0].I()/2+50); b++)
		{
			select_obj(SortedRegions1, &MidRgn, b);
			union2(MidRgn, RegionUnion2, &RegionUnion2);
		}
		union1(RegionUnion2, &MidRoi);
		area_center(MidRoi, &Area_MidRoi, &Row_MidRoi, &Column_MidRoi);

		Hobject RegionUnion3,EndRgn,EndRoi;
		HTuple Area_EndRoi, Row_EndRoi, Column_EndRoi;
		gen_empty_obj(&RegionUnion3);
		{
			for (int c=Number[0].I(); c>=(Number[0].I()-50); c--)
			{
				select_obj(SortedRegions1, &EndRgn, c);
				union2(EndRgn, RegionUnion3, &RegionUnion3);
			}
		}
		union1(RegionUnion3, &EndRoi);
		area_center(EndRoi, &Area_EndRoi, &Row_EndRoi, &Column_EndRoi);

		HTuple RowDiff1,RowDiff2;
		HTuple AreaB, RowB, ColumnB,Error_AreaB, Error_RowB, Error_ColumnB;
		Hobject ContourB,ClosedContoursB,RegionB;
		RowDiff1 = (Row_EndRoi-Row_StartRoi).Abs();
		RowDiff2 = (Row_MidRoi-Row_StartRoi).Abs();
		if (0 != ((RowDiff1>(StartColSize*2))||(RowDiff2>(StartColSize*2))))
		{
			area_center(PartitionedB, &AreaB, &RowB, &ColumnB);
			gen_contour_polygon_xld(&ContourB, RowB, ColumnB);
			close_contours_xld(ContourB, &ClosedContoursB);
			gen_region_contour_xld(ClosedContoursB, &RegionB, "filled");
			area_center(RegionB, &Error_AreaB, &Error_RowB, &Error_ColumnB);
			//
			retValue = Error_AreaB[0].I();
			m_vErrorRgn[rId] = RegionB;
		}
		else
		{
			Hobject RegPartitioned,Contour,ClosedContour,ErrorRgn;
			HTuple Areas, Rows, Columns,ErrorArea,ErrorRow,ErrorColumn;
			partition_rectangle(rgn, &RegPartitioned, 1, 500);
			area_center(RegPartitioned, &Areas, &Rows, &Columns);
			gen_contour_polygon_xld(&Contour, Rows, Columns);
			close_contours_xld(Contour, &ClosedContour);
			gen_region_contour_xld(ClosedContour, &ErrorRgn, "filled");
			area_center(ErrorRgn, &ErrorArea, &ErrorRow, &ErrorColumn);

			if(ErrorArea <= 0)
			{
				retValue = 0.0;
				Hobject newRgn; 
				m_vErrorRgn[rId] = newRgn;
			}
			else
			{
				retValue = ErrorArea[0].I();
				//dilation_circle (DefectRegs, &DefectRegs, 2.5);
				m_vErrorRgn[rId] = ErrorRgn;
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CNailAlgo::CurveDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("923$\
					 397;R;LP;0;20000;1;<=#2000$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CNailAlgo::ThreadDetectAlgo(VARIANT* rgnId, VARIANT* pDetectArea)
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
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}

		if(m_detectRoi.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		else
		{
			Hobject RgnDiff,RgnDiffUnion;
			HTuple error_area,error_row,error_col;
			difference(m_detectRoi,rgn,&RgnDiff);
			union1(RgnDiff,&RgnDiffUnion);
			area_center(RgnDiffUnion,&error_area,&error_row,&error_col);
			if(error_area <= 0)
			{
				retValue = 0.0;
				Hobject newRgn; 
				m_vErrorRgn[rId] = newRgn;
			}
			else
			{
				retValue = error_area[0].I();
				m_vErrorRgn[rId] = RgnDiffUnion;
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CNailAlgo::ThreadDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("867$\
					 397;R;LP;0;50000;1;<=#2000$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

