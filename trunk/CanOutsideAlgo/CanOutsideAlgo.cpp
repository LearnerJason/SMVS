#include "stdafx.h"
#include "resource.h"
#include "CanOutsideAlgo_i.h"
#include "dllmain.h"

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
#include "stdafx.h"
#include "CanOutsideAlgo.h"

STDMETHODIMP CCanOutsideAlgo::SetCurrentImage(LONG* pImg)
{
	Hobject* pObj = (Hobject*)pImg;
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
		copy_image(m_crtImgMult, &m_crtImg);
	}
	if(m_bFileUpdated)
	{
		m_bFileUpdated=false;
	}

	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	//TODO:
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
{
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

STDMETHODIMP CCanOutsideAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{   
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+m_strTaskName;
	m_strTaskPath = CW2A(m_wstrTaskPath.c_str());
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	//TODO:
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+m_strTaskName;
	m_strTaskPath = CW2A(m_wstrTaskPath.c_str());
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{
	return S_FALSE;
}

STDMETHODIMP CCanOutsideAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;//S_OK,S_FALSE
}

STDMETHODIMP CCanOutsideAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::ReloadExternalFile(BSTR* bstrPath)
{
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::SetChannelIndex(LONG nChIndex)
{
	m_nChIndex = nChIndex;
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::ReceivePrerequisiteParam(LONG* pCameraIndex, LONG* pHomMat2DForFlag, LONG* pLeftCol4ImgsInStd, LONG* pRightCol4ImgsInStd)
{
	HTuple* pIndex = (HTuple*)pCameraIndex;
	m_nCameraIndex = *pIndex;

	HTuple* pHomMat = (HTuple*)pHomMat2DForFlag;
	m_HomMat2DForFlag = *pHomMat;

	HTuple* pLeft = (HTuple*)pLeftCol4ImgsInStd;
	m_LeftCol4ImgsInStd = *pLeft;

	HTuple* pRight = (HTuple*)pRightCol4ImgsInStd;
	m_RightCol4ImgsInStd = *pRight;
	return S_OK;
}

template<class T>
string to_string(const T& t)
{
	std::ostringstream oss;
	oss << t;
	return oss.str();
}

void CCanOutsideAlgo::SelectTupleBetweenMinMax(HTuple InputTuple, HTuple MinValue, HTuple MaxValue, HTuple* SelectedIndices)
{
	HTuple IndicesAboveMin, TupleAboveMin;
	tuple_find((InputTuple-MinValue[0].D()).Sgn(), 1, &IndicesAboveMin);
	tuple_select(InputTuple, IndicesAboveMin, &TupleAboveMin);
	tuple_find((TupleAboveMin - MaxValue[0].D()).Sgn(), -1, SelectedIndices);
	tuple_add(*SelectedIndices, IndicesAboveMin[0], SelectedIndices);
}

bool CCanOutsideAlgo:: LocateErrorInFlatRegion(Hobject ImageToDetect, Hobject ImageToDetectGray, Hobject ReferImage, Hobject ReferImageGray, Hobject FlatRegion, Hobject FlatCenterRegion, Hobject FlatSideRegion, Hobject* Errors, HTuple MinEdgeAmp, HTuple MinEdgeArea, HTuple DynThreshold, HTuple MinArea)
{
	gen_empty_obj(Errors);
	bool retFlag = true;

	Hobject FlatRegionErosion;
	Hobject ImageReduced, ImageEdgeAmp;
	Hobject EdgeRegion, EdgeRegionConnected;
	Hobject EdgeError;

	Hobject ImageDomain;
	HTuple  MaxArea, ErrorNum;
	get_domain(ImageToDetect, &ImageDomain);
	region_features(ImageDomain, "area", &MaxArea);

	//根据轮廓幅值，筛选缺陷
	//收缩一定大小，避免在区域边缘处误检
	erosion_circle(FlatRegion, &FlatRegionErosion, 3.5);
	reduce_domain(ImageToDetectGray, FlatRegionErosion, &ImageReduced);
	robinson_amp(ImageReduced, &ImageEdgeAmp);
	threshold(ImageEdgeAmp, &EdgeRegion, MinEdgeAmp, 255);
	connection(EdgeRegion, &EdgeRegionConnected);
	select_shape(EdgeRegionConnected, &EdgeError, "area", "and", MinEdgeArea, MaxArea);
	count_obj(EdgeError, &ErrorNum);
	if (ErrorNum > 0)
	{
		union2(*Errors, EdgeError, Errors);
		retFlag = false;
	}

	//动态阈值
	Hobject ReferImgReduced;
	Hobject DynThreshError, DynThreshErrorConnected;
	reduce_domain(ReferImageGray, FlatRegion, &ReferImgReduced);
	dyn_threshold(ImageReduced, ReferImgReduced, &DynThreshError, DynThreshold, "not_equal");
	connection(DynThreshError, &DynThreshErrorConnected);
	select_shape(DynThreshErrorConnected, &DynThreshError, "area", "and", MinArea, MaxArea);
	count_obj(DynThreshError, &ErrorNum);
	if (ErrorNum > 0)
	{
		union2(*Errors, DynThreshError, Errors);
		retFlag = false;
	}

	return retFlag;
}

bool CCanOutsideAlgo:: LocateErrorDynThresh1(Hobject ImageToDetect, Hobject ReferImage, Hobject CenterROI, Hobject SideROI, Hobject* Errors, HTuple DynThreshold, HTuple MinArea)
{
	bool retFlag = true;
	gen_empty_obj(Errors);

	Hobject ImageReduced, ImageReduced1;
	Hobject RegionDynThresh, ConnectedRegions, SelectedRegions;
	HTuple  Width, Height, Width1, Height1;
	HTuple  ErrorNum;
	get_image_size(ImageToDetect, &Width, &Height);
	get_image_size(ReferImage, &Width1, &Height1);

	HTuple MaxArea = Width*Height;

	Hobject ErrorsTemp, ErrorsTemp1;
	reduce_domain(ImageToDetect, CenterROI, &ImageReduced);
	reduce_domain(ReferImage, CenterROI, &ImageReduced1);
	dyn_threshold(ImageReduced, ImageReduced1, &RegionDynThresh, DynThreshold, "not_equal");
	//	dyn_threshold(ImageReduced, ImageReduced1, &RegionDynThresh, DynThreshold, "dark");
	connection(RegionDynThresh, &ConnectedRegions);
	select_shape(ConnectedRegions, &ErrorsTemp, "area", "and", MinArea, MaxArea);
	count_obj(ErrorsTemp, &ErrorNum);
	if (ErrorNum > 0)
	{
		union2(*Errors, ErrorsTemp, Errors);
		retFlag = false;
	}

	reduce_domain(ImageToDetect, SideROI, &ImageReduced);
	reduce_domain(ReferImage, SideROI, &ImageReduced1);
	dyn_threshold(ImageReduced, ImageReduced1, &RegionDynThresh, DynThreshold*1.5, "not_equal");
	//	dyn_threshold(ImageReduced, ImageReduced1, &RegionDynThresh, DynThreshold*1.5, "dark");
	connection(RegionDynThresh, &ConnectedRegions);
	select_shape(ConnectedRegions, &ErrorsTemp1, "area", "and", MinArea*2, MaxArea);
	count_obj(ErrorsTemp1, &ErrorNum);
	if (ErrorNum > 0)
	{
		union2(*Errors, ErrorsTemp1, Errors);
		retFlag = false;
	}

	return retFlag;

}

STDMETHODIMP CCanOutsideAlgo::SetCommonParameters(LONG* pStandardImg, LONG* pFlagModelRegion, LONG* pSeamRegionInStdImg, LONG* pStdModelRegion, LONG* pStitchedImgWidthUnion, LONG* pModelIDs, LONG* pModelRows, LONG* pModelCols, LONG* pFlagModelID, LONG* pSortedFlagColsInStdImg, LONG* pStichingParamCol, LONG* pCanRegWidth, LONG* pMappedImgWidths)
{
	Hobject * pObj = (Hobject*)pStandardImg;
	m_commonParam.StandardImg = *pObj;

	Hobject* pFlagModelRgn = (Hobject*)pFlagModelRegion;
	m_commonParam.FlagModelRegion = *pFlagModelRegion;

	Hobject* pSeamRgn = (Hobject*)pSeamRegionInStdImg;
	m_commonParam.SeamRegionInStdImg = *pSeamRgn;

	Hobject* pStdModelRgn = (Hobject*)pStdModelRegion;
	m_commonParam.StdModelRegion = *pStdModelRgn;

	HTuple* pValue = (HTuple*)pStitchedImgWidthUnion;
	m_commonParam.StitchedImgWidthUnion = *pValue;

	pValue = (HTuple*)pModelIDs;
	m_commonParam.ModelIDs = *pValue;

	pValue = (HTuple*)pModelRows;
	m_commonParam.ModelRows = *pValue;

	pValue = (HTuple*)pModelCols;
	m_commonParam.ModelCols = *pValue;

	pValue = (HTuple*)pFlagModelID;
	m_commonParam.FlagModelID = *pValue;

	pValue = (HTuple*)pSortedFlagColsInStdImg;
	m_commonParam.SortedFlagColsInStdImg = *pValue;

	pValue = (HTuple*)pStichingParamCol;
	m_commonParam.StitchingParamCol = *pValue;

	pValue = (HTuple*)pCanRegWidth;
	m_commonParam.CanRegWidth = *pValue;

	pValue = (HTuple*)pMappedImgWidths;
	m_commonParam.MappedImgWidths = *pValue;
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::SetDetectROI(LONG* pDetectRoi)
{
	Hobject* pValue = (Hobject*)pDetectRoi;
	m_DetecRoi = *pValue;
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::SetPreprocessedImg(LONG* pMappedImg, LONG* pMappedGrayImg)
{
	Hobject* pImg = (Hobject*)pMappedImg;
	m_MappedImg = *pImg;

	pImg = (Hobject*)pMappedGrayImg;
	m_MappedGrayImg = *pImg;

	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
	return S_OK;
}
STDMETHODIMP CCanOutsideAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("621$\
					 167;B$\
					 374;LP;6;2000;1;220$\
					 390;LP;1;255;1;40");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CCanOutsideAlgo::RectangleLocationAlgo(VARIANT* rgnId, VARIANT* centerRow, VARIANT* centerCol, VARIANT* nHalfWid, VARIANT* nHalfHeight)
{
	//TODO:
	return S_OK;
}
STDMETHODIMP CCanOutsideAlgo::RectangleLocationAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CCanOutsideAlgo::DetectError(VARIANT* rgnId, VARIANT* pFlatRgnThresh, VARIANT* pFlatRgnMinArea, VARIANT* pTextRgnThresh, VARIANT* pTextRgnMinArea,VARIANT* pDetectArea)
{
	if(m_bPilotImg)
	{
		return S_OK;
	}
	CComVariant retValue;
	int rId = (int)rgnId->fltVal;
	LONG nFlatRgnThresh  = (LONG)pFlatRgnThresh->fltVal;
	LONG nFlatRgnMinArea = (LONG)pFlatRgnMinArea->fltVal;
	LONG nTextRgnThresh  = (LONG)pTextRgnThresh->fltVal;
	LONG nTextRgnMinArea = (LONG)pTextRgnMinArea->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		HTuple StdImgWidth, StdImgHeight;
		get_image_size(m_commonParam.StandardImg, &StdImgWidth, &StdImgHeight);

		HTuple Temp;
		HTuple LeftSide4SearchModel =  0.5*(m_LeftCol4ImgsInStd[m_nChIndex].I() + m_RightCol4ImgsInStd[m_nChIndex].I())-140;
		HTuple RightSide4SearchModel = 0.5*(m_LeftCol4ImgsInStd[m_nChIndex].I() + m_RightCol4ImgsInStd[m_nChIndex].I())+140;

		HTuple SelectedModelIndex, SelectedModelIDs;
		HTuple SelectModelRows, SelectModelCols;
		Hobject SearchRect, ImgReduced;
		HTuple MatchedRow, MatchedCol, MatchedAngle, MatchedScore, MatchedModelIndex;
		HTuple HomMat2D;
		Hobject AffinTransImg, AffinTransImgDomain;
		Hobject MappedImgRect, AffinTransRect;
		HTuple  Width, Height;
		Hobject StdImgRegion, IntersectRegion;
		Hobject ImgReduce1,ImgToDetect, ImgToDetectGray;
		Hobject ImgReduce2, ReferenceImg, ReferenceImgGray;
		HTuple ReferWidth,ReferHeight, CroppedWidth, CroppedHeight;

		HTuple LeftUpRow, LeftUpCol, RightDownRow, RightDownCol;
		Hobject MovedRegion, IntersectContour;
		Hobject  IntersectRegSubSeam, MovedIntersectRegSubSeam;
		Hobject  SelectDetectROI,DetectROISubSeam;
		Hobject  FlatRegion, TextRegion;
		Hobject  FlatRegError, TextRegError;

		Hobject CenterROI, SideROI;
		Hobject TextCenterRegion, TextSideRegion;
		Hobject FlatCenterRegion, FlatSideRegion;
		HTuple  DetectROIArea, CenterPtRow, CenterPtCol;
		HTuple  Row1, Col1, Row2, Col2;
		HTuple  ROIHeight, ROIWidth;

		get_image_size(m_MappedImg, &Width, &Height);
		//标记位所在通道，直接使用标记位匹配的结果
		if (m_nChIndex == m_nCameraIndex)
		{
			HomMat2D = m_HomMat2DForFlag;		
		}
		else
		{
			SelectTupleBetweenMinMax(m_commonParam.ModelCols, LeftSide4SearchModel, RightSide4SearchModel, &SelectedModelIndex);
			tuple_select(m_commonParam.ModelIDs, SelectedModelIndex, &SelectedModelIDs);
			tuple_select(m_commonParam.ModelRows, SelectedModelIndex, &SelectModelRows);
			tuple_select(m_commonParam.ModelCols, SelectedModelIndex, &SelectModelCols);
			gen_rectangle1(&SearchRect, m_commonParam.ModelRows[0].D()-Width[0].I()*0.1,  Width[0].I()*0.3, m_commonParam.ModelRows[0].D()+Width[0].I()*0.1, Width[0].I()*0.7);
			reduce_domain(m_MappedGrayImg, SearchRect, &ImgReduced);
			find_shape_models(ImgReduced, SelectedModelIDs, HTuple(-10).Rad(), HTuple(20).Rad(), 0.3, 1, 0.5, "least_squares", 0, 0.8, &MatchedRow, &MatchedCol, &MatchedAngle, &MatchedScore, &MatchedModelIndex);
			//角度偏差判断
			if (MatchedAngle.Num() > 0 && MatchedAngle.Abs() > HTuple(2).Rad())
			{
				retValue = 0.0;
				retValue.Detach(pDetectArea);
				return S_FALSE;
			}
			if (MatchedScore.Num() > 0)
			{
				vector_angle_to_rigid(MatchedRow, MatchedCol, MatchedAngle, SelectModelRows[MatchedModelIndex], SelectModelCols[MatchedModelIndex], 0, &HomMat2D);
			}
			else //根据中心位置生成映射，此处是否可以采用最近的一次匹配成功时生成的映射？？？
			{
				vector_angle_to_rigid(0.5* (Height[0].D()-1), 0.5*(Width[0].D()-1), 0, 0.5*StdImgHeight[0].D(), 0.5*((m_LeftCol4ImgsInStd[m_nChIndex]).D() + m_RightCol4ImgsInStd[m_nChIndex].D()), 0, &HomMat2D);
			}
		}
		affine_trans_image(m_MappedImg, &AffinTransImg, HomMat2D, "nearest_neighbor", "true");
		get_domain(AffinTransImg, &AffinTransImgDomain);
		//变换区域
		get_domain(m_MappedImg, &MappedImgRect);
		affine_trans_region(MappedImgRect, &AffinTransRect, HomMat2D, "true");

		get_domain(m_commonParam.StandardImg, &StdImgRegion);
		intersection(StdImgRegion, AffinTransRect, &IntersectRegion);
		intersection(IntersectRegion, AffinTransImgDomain, &IntersectRegion);   //各通道图像在标准图片上的位置

		reduce_domain(AffinTransImg, IntersectRegion, &ImgReduce1);
		crop_domain(ImgReduce1, &ImgToDetect);	
		rgb1_to_gray(ImgToDetect, &ImgToDetectGray);

		reduce_domain(m_commonParam.StandardImg, IntersectRegion, &ImgReduce2);
		crop_domain(ImgReduce2, &ReferenceImg);
		rgb1_to_gray(ReferenceImg, &ReferenceImgGray);

		//防止变换产生的插值导致图像大小不一致
		get_image_size(ReferenceImg, &ReferWidth, &ReferHeight);
		get_image_size(ImgToDetect, &CroppedWidth, &CroppedHeight);
		if (ReferHeight[0].I() != CroppedHeight[0].I() || CroppedWidth[0].I() != ReferWidth[0].I())     
		{
			retValue = 0.0;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		//保存最终的四通道在标准图片上的位置
		Hobject RectsInStdImg;
		concat_obj(RectsInStdImg, IntersectRegion, &RectsInStdImg);

		//生成标准轮廓，用于后续检测
		smallest_rectangle1(IntersectRegion, &LeftUpRow, &LeftUpCol, &RightDownRow, &RightDownCol);
		intersection(m_commonParam.StdModelRegion, IntersectRegion, &IntersectContour);
		move_region(IntersectContour, &MovedRegion, LeftUpRow*(-1), LeftUpCol*(-1));
		//去掉焊缝区域
		difference(IntersectRegion, m_commonParam.SeamRegionInStdImg, &IntersectRegSubSeam);
		move_region(IntersectRegSubSeam, &MovedIntersectRegSubSeam, LeftUpRow*(-1), LeftUpCol*(-1));

		copy_obj(m_DetecRoi, &SelectDetectROI, 1, 1);

//		select_obj(m_DetecRoi, &SelectDetectROI, m_nChIndex);
		intersection(SelectDetectROI, MovedIntersectRegSubSeam, &DetectROISubSeam);

		//确定文字等的区域
		intersection(MovedRegion, DetectROISubSeam, &TextRegion);

		//确定平坦区域
		difference(DetectROISubSeam, TextRegion, &FlatRegion);
		//此部分可离线生成！！！
		//将检测区域分为三部分，中间严，两边松，目前仅针对轮廓部分的检测		
		area_center(DetectROISubSeam, &DetectROIArea, &CenterPtRow, &CenterPtCol);
		smallest_rectangle1(DetectROISubSeam, &Row1, &Col1, &Row2, &Col2);
		ROIHeight = Row2-Row1;
		ROIWidth =  Col2 - Col1;
		gen_rectangle2(&CenterROI, CenterPtRow, CenterPtCol, 0, 0.3*ROIWidth, 0.5*ROIHeight);
		intersection(TextRegion, CenterROI, &TextCenterRegion);
		difference(TextRegion, TextCenterRegion, &TextSideRegion);
		intersection(FlatRegion, CenterROI, &FlatCenterRegion);
		difference(FlatRegion, FlatCenterRegion, &FlatSideRegion);

		bool DetectFlatRegFlag = LocateErrorInFlatRegion(ImgToDetect, ImgToDetectGray, ReferenceImg, ReferenceImgGray, FlatRegion, FlatCenterRegion,FlatSideRegion, &FlatRegError, 23, 10, nFlatRgnThresh, nFlatRgnMinArea);
		bool DetectTextRegFlag = LocateErrorDynThresh1(ImgToDetect, ReferenceImg, TextCenterRegion, TextSideRegion, &TextRegError, nTextRgnThresh, nTextRgnMinArea);
		if (!DetectFlatRegFlag || !DetectTextRegFlag)
		{
			Hobject errRgn;
			union2(FlatRegError, TextRegError, &errRgn);
			HTuple Area, Row, Column;
			area_center(errRgn, &Area, &Row, &Column);
			retValue = Area[0].I();
		}	
		else
		{
			retValue = 0.0;
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
STDMETHODIMP CCanOutsideAlgo::DetectErrorHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("695$\
					 200;R;LP;1;1000;1;<=#20$\
					 167;B$\
					 696;LP;1;1000;1;185$\
					 697;LP;1;1000;1;208$\
					 698;LP;1;1000;1;40$\
					 699;LP;1;1000;1;320"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
