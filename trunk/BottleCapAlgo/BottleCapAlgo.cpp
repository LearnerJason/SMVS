#include "stdafx.h"
#include "resource.h"
#include "BottleCapAlgo_i.h"
#include "dllmain.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "BottleCapAlgo.h"

#include "../ImageProcessLib/ImageProcessLib.h"
#pragma comment(lib,"ImageProcessLib.lib")
using namespace ColorImage;

using namespace std;
#define  HAMMING (11)
#define  BOXCAR (10)
#define  ALL  (0)
#define  DARK  (-1)
#define  LIGHT (1)
#define NOT_EQUAL (0)
#define MAX_AREA (999999)
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

STDMETHODIMP CBottleCapAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IBottleCapAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CBottleCapAlgo::SetCurrentImage(LONG* pImg)
{
	m_imageNumber++;
	m_ptrImgData = (Hobject*)pImg;

	m_crtImgMult  = *m_ptrImgData;// 原始图片，可能是多通道图片（彩色图片）
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
		m_crtImg  = *m_ptrImgData;
	}
	else
	{
		mean_n(m_crtImgMult, &m_crtImg);
	}
	Hlong Width,Height;
	get_image_size (m_crtImg, &Width, &Height);
	m_imgWidth  = (long)Width;
	m_imgHeight = (long)Height;
	//区域，辅助区域，辅助线的初始化
	if (m_bDebugImg||m_bPilotImg)
	{
		m_vRgnEx.clear();
		m_vRgnEx.resize(128);

		m_vRgnExTest.clear();
		m_vRgnExTest.resize(128);
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

STDMETHODIMP CBottleCapAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GetRegionEx(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnEx[rgnID];
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GetRegionExTest(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnExTest[rgnID];
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GetInterImgs(BYTE rgnID, LONG* pInterImgs)
{
	Hobject* pObj = (Hobject*)pInterImgs;
	*pObj = m_vInterImgs[rgnID];
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	//Hobject* pObj = (Hobject*)pRgn;
	//*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GetGlueInjectorID(LONG* pInjectorID)
{
	*pInjectorID = m_nCrtInjectorID;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GetDetectParam(LONG* pDetectParam)
{
	*pDetectParam = m_nDetectParam;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::SetDetectParam(LONG* pDetectParam)
{
	m_nDetectParam=*pDetectParam ;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GetWhiteErrorRegion(BYTE rgnID,  LONG* pWhiteErrorRgn)
{
	Hobject* pObj = (Hobject*)pWhiteErrorRgn;
	*pObj = m_vWhiteErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CBottleCapAlgo::GetCurrentLocationXY(BSTR* bstrLocationXY)
{
	// TODO: 在此添加实现代码
	CString cstrXY;
	if(m_modelMatchingPARA.Select(7)==1)
	{
		//方盖
		cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_angleMatchingPARA[2].D(),m_angleMatchingPARA[1].D(),m_angleMatchingPARA[3].D());
	}
	else
	{
		cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[3].D());
	}


	CComBSTR strLocationXY = cstrXY.AllocSysString();
	HRESULT hr = strLocationXY.CopyTo(bstrLocationXY);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GetCenterLocation(BSTR* bstrLocationXY)
{
	//当前定位中心点，角度默认为0度。
	CString cstrXY;
	cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_vPos[0].m_x,m_vPos[0].m_y,0.0);
	//NEW
	m_modelPARA[0]=m_vPos[0].m_y;
	m_modelPARA[1]=m_vPos[0].m_x;
	//
	CComBSTR strLocationXY = cstrXY.AllocSysString();
	HRESULT hr = strLocationXY.CopyTo(bstrLocationXY);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
bool CBottleCapAlgo::ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA)
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

STDMETHODIMP CBottleCapAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{

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
		m_gaussTransThred=(double)atof(vPara[0].c_str());
		m_gaussExtractThred=(double)atof(vPara[1].c_str());
		m_dynExtractThredSingle=(double)atof(vPara[2].c_str());
		m_dynExtractThredMulti=(double)atof(vPara[3].c_str());
		m_regionRadius=(double)atof(vPara[4].c_str());
		m_regionDilationSize=(double)atof(vPara[5].c_str());
		m_smallExtractArea=(double)atof(vPara[6].c_str());
		m_modelRadius=(int)m_regionRadius;
	}
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{
	if(productID==2)
	{
		Hobject* pObj = (Hobject*)pCrtImg;
		m_crtImg = *pObj;
		Hobject Image = m_crtImg;
		m_nImgIndex++;
		if(m_nSampleSize-m_nImgIndex>=0)//199
		{
			HTuple ImageClass;
			get_obj_class(MultiChannelImageTest, &ImageClass);
			if(ImageClass!=HTuple("image"))
			{
				HTuple With,Height;
				get_image_size(Image, &With, &Height);
				m_imgHeight=(long)Height[0].L();
				m_imgWidth=(long)With[0].L();
				m_modelRadius=m_modelRadius*(m_imgHeight/494.0); 
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
			Hobject Detect_Region,ImageReduced_Detect,ModelRegion,ModelImage,DupImage,Rectangle,RegionDifference;
			HTuple Score,Qy,Qx,Row3, Column3, Angle3, Score3,ModelID;
			HTuple Row, Column, Angle, Model,ModelID_Add;    
			gen_rectangle1(&Rectangle, 0, 0, m_imgHeight-1, m_imgWidth-1);
			HTuple Class;
			get_obj_class(m_multiChannelModelImage, &Class);

			if(Class!=HTuple("image"))
			{	
				//计算中心点
				HTuple CenterXY;
				ImageProcessAlg.CenterDefined(Image,(float)0.85,CenterXY);
				float fCenterX=0.0;
				float fCenterY=0.0;
				fCenterX=(float)CenterXY[0].D();
				fCenterY=(float)CenterXY[1].D();
				//ImageProcessAlg.RadiusDefinedTest(Image,fCenterY,fCenterX,20,40,10,120,m_modelRadius);
				m_modelRadius=m_modelRadius-10;
				//
				//中心定位失败的图片，不作为模板图片
				if((fCenterX >2.0)&(fCenterY>2.0))		
				{
					Qy=fCenterY;
					Qx=fCenterX;
					/////////////////////////////////////////////////////////////////
					gen_circle(&ModelRegion, Qy, Qx, m_modelRadius);
					m_modelRegion=ModelRegion;
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

STDMETHODIMP CBottleCapAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}

STDMETHODIMP CBottleCapAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::SaveResult(BSTR* strModelName,BYTE productID)
{
	m_isFirstReadSelfLearningPara=true;
	if(productID==2)
	{
#ifdef NDEBUG
		try
		{
#endif
			CString modelParaPath,modelParaPath1,saveName;
			string path_str;
			CString sPath = *strModelName;
			std::wstring wstrPath = CT2W(sPath);
			std::string strPath = CW2A(wstrPath.c_str());
			size_t index = strPath.find_last_of("\\");
			string strSaveName = strPath.substr(index + 1);
			saveName=strSaveName.c_str();
			//
			modelParaPath1.Format(_T("%s%s%s"),sPath,_T("\\"),saveName);
			if(Model_Num>=1)// 保存模板时，添加模板数量判断。
			{
				modelParaPath.Format(_T("%s%s"),modelParaPath1,_T(".shm"));
				path_str= CT2A(modelParaPath.GetString());
				write_shape_model(HTuple(Models[0]),path_str.c_str());
				//
				modelParaPath.Format(_T("%s%s"),modelParaPath1,_T(".tiff"));
				path_str= CT2A(modelParaPath.GetString());
				write_image(m_multiChannelModelImage, "tiff", 0, path_str.c_str());
				//
				modelParaPath.Format(_T("%s%s"),modelParaPath1,_T(".reg"));
				path_str= CT2A(modelParaPath.GetString());
				write_region(m_modelRegion, path_str.c_str());
				//
				HTuple Value_r2,Value_c2;
				get_value_matrix(MatrixID,0, 0, &Value_r2);
				get_value_matrix(MatrixID,0, 1, &Value_c2);
				modelParaPath.Format(_T("%s%s"),modelParaPath1,_T(".dat"));
				CString dataStr;
				dataStr.Format(_T("%f"),m_modelRadius[0].D());
				WritePrivateProfileString(_T("RegionData"),_T("ModelRadius"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),0.0);
				WritePrivateProfileString(_T("RegionData"),_T("Phi"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),Value_r2[0].D());
				WritePrivateProfileString(_T("RegionData"),_T("Row"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),Value_c2[0].D());
				WritePrivateProfileString(_T("RegionData"),_T("Col"),dataStr,modelParaPath);

				//write_matrix(MatrixID, "binary", path_str.c_str());
				//
				modelParaPath.Format(_T("%s%s"),modelParaPath1,_T("_RegionDetect.reg"));
				path_str= CT2A(modelParaPath.GetString());	
				get_value_matrix(MatrixID,0, 0, &Value_r2);
				get_value_matrix(MatrixID,0, 1, &Value_c2);
				Hobject RegionSave;
				move_region (m_regLearning, &RegionSave,-Value_r2,-Value_c2);
				write_region(RegionSave, path_str.c_str());
				//
				modelParaPath.Format(_T("%s%s"),modelParaPath1,_T("_RegionDetect.dat"));
				dataStr.Format(_T("%f"),m_modelRadius[0].D());
				WritePrivateProfileString(_T("RegionData"),_T("ModelRadius"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),0.0);
				WritePrivateProfileString(_T("RegionData"),_T("Phi"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),0.0);//Value_r2[0].D()
				WritePrivateProfileString(_T("RegionData"),_T("Row"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),0.0);//Value_c2[0].D()
				WritePrivateProfileString(_T("RegionData"),_T("Col"),dataStr,modelParaPath);
				//
				modelParaPath.Format(_T("%s%s"),modelParaPath1,_T("_RegionExtract.reg"));
				path_str= CT2A(modelParaPath.GetString());	
				move_region (m_regExtractLearning, &RegionSave,-Value_r2,-Value_c2);
				write_region(RegionSave, path_str.c_str());
				//
				modelParaPath.Format(_T("%s%s"),modelParaPath1,_T("_RegionExtract.dat"));
				dataStr.Format(_T("%f"),m_modelRadius[0].D());
				WritePrivateProfileString(_T("RegionData"),_T("ModelRadius"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),0.0);
				WritePrivateProfileString(_T("RegionData"),_T("Phi"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),0.0);//Value_r2[0].D()
				WritePrivateProfileString(_T("RegionData"),_T("Row"),dataStr,modelParaPath);
				dataStr.Format(_T("%f"),0.0);//Value_c2[0].D()
				WritePrivateProfileString(_T("RegionData"),_T("Col"),dataStr,modelParaPath);
				//
				gen_empty_obj(&m_multiChannelModelImage);
				gen_empty_obj(&MultiChannelImageTest);				
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

STDMETHODIMP CBottleCapAlgo::ResetModels(BYTE productID)
{
	if(productID==2)
	{
#ifdef NDEBUG
		try
		{
#endif
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
			m_centerTuple[0]=m_imgHeight/2.0;
			m_centerTuple[1]=m_imgWidth/2.0;
			Model_Radius=m_modelRadius;

			//*vc code**********************************************************************
			count_channels(MultiChannelImageTest, &Length_set);//100
			count_channels(m_multiChannelModelImage, &Model_Num);
			gen_circle(&ModelRegion, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]), Model_Radius);
			HTuple RadiusForData;
			RadiusForData= (m_imgHeight/2.0)*0.76;
			gen_circle(&RegionD, HTuple(m_centerTuple[0]), HTuple(m_centerTuple[1]),RadiusForData);
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
				HTuple CenterXY;
				ImageProcessAlg.CenterDefined(Image,(float)0.85,CenterXY);
				float fCenterX=0.0;
				float fCenterY=0.0;
				fCenterX=(float)CenterXY[0].D();
				fCenterY=(float)CenterXY[1].D();
				////////////////////////////////////////////////////
				if((fCenterX >2.0)&(fCenterY>2.0))
				{
					RowTrans=fCenterY;
					ColTrans=fCenterX;
					gen_circle(&Region, RowTrans, ColTrans, RadiusForData);
					gen_circle(&CircleD, RowTrans, ColTrans, 80.5*(m_imgHeight/494.0));
					reduce_domain(Image, CircleD, &ImageReducedFindModel);
					for (Index2=1; Index2<=Model_Num; Index2+=1)
					{
						get_value_matrix(MatrixID, Index2-1, 0, &Value_r);
						get_value_matrix(MatrixID, Index2-1, 1, &Value_c);
						access_channel(m_multiChannelModelImage, &Image1, Index2);
						find_shape_model(ImageReducedFindModel, HTuple(Models[Index2-1]), 0, 6.29, 0.5, 1, 0.5, "least_squares", 0, 0.75, &Row_AffinTrans, &Column_AffinTrans, &Angle_AffinTrans, &Score_AffinTrans);
						if (0 != (Score_AffinTrans>0.6))
						{
							vector_angle_to_rigid(Row_AffinTrans, Column_AffinTrans, Angle_AffinTrans, Value_r, Value_c, 0, &HomMat2DT);
							affine_trans_pixel(HomMat2DT, RowTrans, ColTrans, &RowTrans1, &ColTrans1);
							gen_circle(&RegionD, RowTrans1, ColTrans1,RadiusForData);
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
					if (0 != (Score3>0.75))
					{
						ModelsNew[Model_Num_New-1] = ModelIDR;
						set_value_matrix(MatrixIDNew, Model_Num_New-1, 0, Row3);
						set_value_matrix(MatrixIDNew, Model_Num_New-1, 1, Column3);
					}
					else if (0 != (Score3<=0.75))
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

STDMETHODIMP CBottleCapAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
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
		double GaussTransThred,GaussExtractThred,DynExtractThredSingle,DynExtractThredMulti;
		double RegionRadius,RegionDilationSize,SmallExtractArea;
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
		GaussTransThred=(double)atof(vPara[0].c_str());
		GaussExtractThred=(double)atof(vPara[1].c_str());
		DynExtractThredSingle=(double)atof(vPara[2].c_str());
		DynExtractThredMulti=(double)atof(vPara[3].c_str());
		RegionRadius=(double)atof(vPara[4].c_str());
		RegionDilationSize=(double)atof(vPara[5].c_str());
		SmallExtractArea=(double)atof(vPara[6].c_str());
		///////////////////////////////
		Hobject ImageMean,ImageReduced,CenterRegion;
		HTuple Value_r2,Value_c2;
		get_value_matrix(MatrixID,0, 0, &Value_r2);
		get_value_matrix(MatrixID,0, 1, &Value_c2);

		Hobject RegionUnionEdge,RegionExtract1,RegionExtract2;
		//
		if(m_isFirstReadSelfLearningPara)
			access_channel(m_multiChannelModelImage,&ImageMean,1);
		else
			ImageMean=m_crtImg;

		HTuple  Width, Height;
		get_image_size(ImageMean, &Width, &Height);
		if((Value_r2+RegionRadius>Height-10)||(Value_r2-RegionRadius<10)||(Value_c2+RegionRadius>Width-10)||(Value_c2-RegionRadius<10))
		{
			RegionRadius=100;
		}
		gen_circle(&CenterRegion,Value_r2,Value_c2,RegionRadius);
		reduce_domain(ImageMean,CenterRegion,&ImageReduced);
		ImageProcessAlg.ExtractRegionUseAdge(ImageReduced,1.5, RegionUnionEdge);//EdgeDilationSize
		ImageProcessAlg.ExtractRegionUseGaussDiff(ImageReduced,CenterRegion,GaussTransThred,2.0,GaussExtractThred, RegionExtract1);//GaussGamma
		int matchingNum=0;
		if((!m_isFirstReadSelfLearningPara)&(abs(m_dynExtractThredSingle-DynExtractThredSingle)<0.2))
		{
			HTuple Channels;
			Hobject Region2;
			count_channels(MultiChannelImageTest, &Channels);
			threshold(m_imageDistNum, &Region2, DynExtractThredMulti, Channels);
			dilation_circle(Region2, &RegionExtract2, 1.5);
			intersection(RegionExtract2,CenterRegion, &RegionExtract2);
		}
		else
		{
			matchingNum= ImageProcessAlg.ExtractRegionUseDynDetect(ImageMean,CenterRegion,Models[0], Value_r2,Value_c2,0.0,MultiChannelImageTest,0.8,DynExtractThredSingle,DynExtractThredMulti, RegionExtract2,m_imageDistNum);//DynExtractScore
		}
		Hobject RegionExtractUnion,ExtractUnionDilation,RegionDiff,RegionDetect;
		union2 (RegionExtract1, RegionExtract2, &RegionExtractUnion);
		union2 (RegionExtractUnion, RegionUnionEdge, &RegionExtractUnion);
		dilation_circle (RegionExtractUnion,& ExtractUnionDilation,(int)(abs(RegionDilationSize))+0.5 );
		difference (CenterRegion, ExtractUnionDilation, &RegionDiff);
		fill_up_shape (RegionDiff,&RegionDetect, "area", 0, abs(SmallExtractArea));
		m_regLearning=RegionDetect;
		m_regExtractLearning=ExtractUnionDilation;
		//
		m_isFirstReadSelfLearningPara=false;
		///////////////////////////////
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
STDMETHODIMP CBottleCapAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{
	ResetModels(productID);
	if(productID==2)
	{
#ifdef NDEBUG
		try
		{
#endif
			if(Model_Num_New<1)
				return S_FALSE;

			Model_Num_Sort=Model_Num_New;
			HTuple ModelsSort;
			ModelsSort = HTuple();
			ScoreSort[0] =85;
			//替换训练结果/////////////////////////////
			gen_empty_obj(&m_multiChannelModelImage);
			Models = HTuple();
			MatrixID = HTuple();
			copy_image(m_multiChannelModelImageNew,&m_multiChannelModelImage);
			copy_matrix(MatrixIDNew,&MatrixID);
			Models=ModelsNew;
			///////////////////////////////
			ResetSelfLearningRegion(&m_strRegionExtractPara);
#ifdef NDEBUG
		}

		catch (HException &except) 
		{
			m_algoSpy.QueueUpLogMsg(except, m_crtImg);
			return S_FALSE;
		}
#endif

		Hobject* pObj = (Hobject*)multiModelImag;
		*pObj = m_multiChannelModelImage;

		*cMax = (long)(Model_Num_Sort[0].I());//m_vImgScore.size();
		for(size_t t = 0; t <Model_Num_Sort[0].I(); t++)
		{
			vImgScore[t] = (float)ScoreSort[t].D();
		}
	}
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::ModelCreate(double Radius, HTuple * ModelID1)
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

STDMETHODIMP CBottleCapAlgo::CenterLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用形状模板来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	float MinScore = (float)pMinScore->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vInterImgs[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
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
STDMETHODIMP CBottleCapAlgo::CenterLocationAlgo1Help(BSTR* pHelpStr)
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
STDMETHODIMP CBottleCapAlgo:: CenterLocationAlgoUsePointFitting(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold)
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

		bool result = m_measureHandles[id].LocateEdgePoints(Image, Transition, PointSelection, Sigma, ThresholdVal);


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
STDMETHODIMP CBottleCapAlgo:: CenterLocationAlgoUsePointFittingHelp(BSTR* pHelpStr)
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
					 1302;LP;0;1;1;1$\
					 1303;LP;0;1;1;1$\
					 680;FP;0.4;10.0;0.05;1.0$\
					 1304;FP;0.1;100.0;0.1;10.0");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBottleCapAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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

	m_sectorAngle[id].m_angleStartAbs=0;
	m_sectorAngle[id].m_angleEndAbs=360;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
STDMETHODIMP CBottleCapAlgo::CircleRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	float RadiusSize =(float)pRadius->fltVal;
	float LocationRadius=(float)(m_locationRadius[0].I());
	float Radius =LocationRadius - RadiusSize;
	float m_RadiusExt =LocationRadius - RadiusSize-RoiWidth;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
STDMETHODIMP CBottleCapAlgo::CircleRegionLocationAlgo1Help(BSTR* pHelpStr)
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

STDMETHODIMP CBottleCapAlgo::MatchingParaAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn      = m_vRgn[id];
	float greediness = (float)pGreediness->fltVal;
	int ringOrOCR    = (int)pRingOrOCR->fltVal;
	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg )
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
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=0.0;
				m_modelMatchingPARA[2]=0.0;
				m_modelMatchingPARA[3]=0.0;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				m_modelMatchingPARA[7]=0.0;
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
					m_vErrorRgn[id].Reset();
					m_vWhiteErrorRgn[id].Reset();
					retValue.Detach(pMatchingResult);
					m_modelMatchingPARA[0]=0.0;
					m_modelMatchingPARA[1]=0.0;
					m_modelMatchingPARA[2]=0.0;
					m_modelMatchingPARA[3]=0.0;
					m_modelMatchingPARA[4]=0.0;
					m_modelMatchingPARA[5]=0.0;
					m_modelMatchingPARA[6]=0.0;
					m_modelMatchingPARA[7]=0.0;
					return S_FALSE;
				}
			}
		}
		//
		Hobject ImageReduced,Circle;
		HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;
		m_modelMatchingPARA[0] = m_imageRadius;

		HRegion rgn_copy = rgn;
		if(rgn_copy.Area() <= 0) 
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		bool b_isOK = true;
		Hobject Image=m_crtImg;
		//
		Circle=rgn;
		reduce_domain(Image, Circle, &ImageReduced);

		CString sModelPath1  = *bstrShm1File;
		string strModelPath1 = CT2A(sModelPath1);
		if(m_dictCrtPathModelID.find(strModelPath1) == m_dictCrtPathModelID.end())
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
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
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
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
				m_modelMatchingPARA[7]=0.0;
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
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
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
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
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
				m_modelMatchingPARA[7]=0.0;
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
				m_modelMatchingPARA[7]=0.0;
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
			m_vErrorRgn[id] = Circle;
			return S_FALSE;
		}
		else
		{
			retValue.Detach(pMatchingResult);
		}
		m_modelMatchingPARA[7]=0.0;
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
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pMatchingResult);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::MatchingParaAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("439$\
					 440;R;FP;0.0;1.0;0.01;>=#0.5$\
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
STDMETHODIMP CBottleCapAlgo::MatchingParaMinScoreAlgo(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn      = m_vRgn[id];
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
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
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
					m_vErrorRgn[id].Reset();
					m_vWhiteErrorRgn[id].Reset();
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

		HRegion rgn_copy = rgn;
		if(rgn_copy.Area() <= 0) 
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		bool b_isOK = true;
		Hobject Image=m_crtImg;
		HTuple With,Height;
		get_image_size(Image, &With, &Height);
		m_imgHeight=(long)Height[0].L();
		m_imgWidth=(long)With[0].L();
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
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
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
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
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
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
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
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
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
			m_vErrorRgn[id] = Circle;
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
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pMatchingResult);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::MatchingParaMinScoreAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("439$\
					 440;R;FP;0.0;1.0;0.01;>=#0.5$\
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
STDMETHODIMP CBottleCapAlgo::RotatingAngleDetectAlgo(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pBlackThredLow,VARIANT* pEdgeType,VARIANT* pEdgeDist, VARIANT* pDefectSize, VARIANT* pDetectAngle)
{
	int id = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	float GaussGama = (float)pMaskSize->fltVal;
	float BlackThredLow = (float)pBlackThredLow->fltVal;
	int   EdgeType  =(int)pEdgeType->fltVal;
	float EdgeWidth =  (float)pEdgeDist->fltVal;
	float DefectSize =  (float)pDefectSize->fltVal;
	m_vErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
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
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}

	if(Radius<=RoiWidth)
	{
		RoiWidth=(float)(Radius-1.0);
	}

	if((RoiWidth<6.0)||(RoiWidth+0.4<3+(GaussGama-0.6)/0.25))
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		Hobject Image = m_crtImg;
		HTuple Width,Height;
		get_image_size(Image,&Width,&Height);
		///////////
		Hobject  PolarTransImage,ImageMax,DerivGauss,Region2,RegionOpening2,ConnectedRegions1;
		Hobject  SelectedRegions,SortedRegions;
		HTuple AngleExt,RadiusInner,RadiusExt,WidthPolar,HeightPolar,Area1,Row2,Column2;
		//***************************************************************************************
		AngleExt= 3.141593*2.5;
		RadiusInner = Radius-RoiWidth;
		RadiusExt = Radius;
		WidthPolar = RadiusExt*AngleExt;
		HeightPolar = RadiusExt-RadiusInner;
		polar_trans_image_ext(Image, &PolarTransImage, fCenterY, fCenterX, 0, AngleExt, RadiusInner, RadiusExt, WidthPolar, HeightPolar, "nearest_neighbor");
		//
		if (EdgeType==-1)
		{
			gray_erosion_rect(PolarTransImage, &ImageMax, 5, 1);
			derivate_gauss(ImageMax, &DerivGauss, GaussGama, "xx");
			threshold(DerivGauss, &Region2,BlackThredLow,90000);
		}
		else if (EdgeType==1)
		{
			gray_dilation_rect(PolarTransImage, &ImageMax, 5, 1);
			derivate_gauss(ImageMax, &DerivGauss, GaussGama, "xx");
			threshold(DerivGauss, &Region2, -90000, -BlackThredLow);
		}
		else if (EdgeType==0)
		{
			gray_dilation_rect(PolarTransImage, &ImageMax, 5, 1);
			derivate_gauss(ImageMax, &DerivGauss, GaussGama, "xx");
			threshold(DerivGauss, &Region2, -90000, -BlackThredLow);
			//
			gray_erosion_rect(PolarTransImage, &ImageMax, 5, 1);
			derivate_gauss(ImageMax, &DerivGauss, GaussGama, "xx");
			Hobject RegionBlack;
			threshold(DerivGauss, &RegionBlack,BlackThredLow,90000);
			concat_obj(RegionBlack,Region2,&Region2);
		}
		else
		{
			retValue = -1;
			m_vErrorRgn[id] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}

		if(m_bDebugImg)
		{
			//显示辅助调试信息
			HTuple Width1, Height1, BandpassMeanValue,BandpassMeanValueAbs, Deviation,InputDataLength;
			Hobject Rectangle,Partitioned;
			get_image_size (DerivGauss, &Width1, &Height1);
			gen_rectangle1 (&Rectangle, 2, 0, Height1-2, Width1-1);
			partition_rectangle (Rectangle, &Partitioned, 1, Height1);    
			intensity (Partitioned, DerivGauss,& BandpassMeanValue, &Deviation);
			tuple_abs (BandpassMeanValue, &BandpassMeanValueAbs);
			tuple_length(BandpassMeanValueAbs,&InputDataLength);
			BandpassMeanValue=BandpassMeanValue*(-1);
			//
			HTuple  maxValue = 0;
			tuple_max(BandpassMeanValueAbs, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			switch(EdgeType)
			{
			case 0:
				m_dictSpecialData[id].push_back(BlackThredLow * YScaleFactor);
				m_dictSpecialData[id].push_back((-1)* BlackThredLow * YScaleFactor);
				break;
			case 1:
				m_dictSpecialData[id].push_back(BlackThredLow * YScaleFactor);
				m_dictSpecialData[id].push_back(-999999.0f);
				break;
			case -1:
				m_dictSpecialData[id].push_back(999999.0f);
				m_dictSpecialData[id].push_back((-1)* BlackThredLow * YScaleFactor);
				break;
			default:
				break;
			}
			for(int i = 0; i < InputDataLength; i++)
			{
				m_dictSpecialData[id].push_back(float(BandpassMeanValue[i].D() * YScaleFactor));
			}
		}
		opening_rectangle1(Region2, &RegionOpening2, 1, 3);
		connection(RegionOpening2, &ConnectedRegions1);
		select_shape(ConnectedRegions1, &SelectedRegions, "height", "and", DefectSize,10000);
		sort_region(SelectedRegions, &SortedRegions, "upper_left", "true", "column");
		//write_region(SortedRegions,"D://应用资料//中粮//reg.reg");
		area_center(SortedRegions, &Area1, &Row2, &Column2);
		HTuple EdgeDist = EdgeWidth;
		HTuple ColCenter,EdgeDists,LeftNum,RightNum,EdgeNum;
		ColCenter = 500;
		EdgeDists = HTuple();
		LeftNum = HTuple();
		RightNum = HTuple();
		EdgeNum = 0;
		Hobject EdgeRegion,XYTransRegion;
		gen_empty_obj(&EdgeRegion);
		gen_empty_obj(&XYTransRegion);
		if (0 != ((Column2.Num())<1))
		{
			retValue = -1;
			m_vErrorRgn[id] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		else if (0 != ((Column2.Num())==1))
		{
			ColCenter = HTuple(Column2[0])+(EdgeDist/2.0);
			concat_obj(EdgeRegion, SortedRegions, &EdgeRegion);
			polar_trans_region_inv(EdgeRegion, &XYTransRegion, fCenterY, fCenterX, 0, AngleExt, RadiusInner, RadiusExt, WidthPolar, HeightPolar, Width, Height, "nearest_neighbor");
		}
		else if (0 != ((Column2.Num())==2))
		{
			ColCenter = (HTuple(Column2[0])+HTuple(Column2[1]))/2.0;
			concat_obj(EdgeRegion, SortedRegions, &EdgeRegion);
			polar_trans_region_inv(EdgeRegion, &XYTransRegion,  fCenterY, fCenterX,0, AngleExt, RadiusInner, RadiusExt, WidthPolar, HeightPolar, Width, Height, "nearest_neighbor");
		}
		else if (0 != ((Column2.Num())>2))
		{
			{
				HTuple end_val67 = (Column2.Num())-1;
				HTuple step_val67 = 1;
				HTuple Index1,Index2;
				for (Index1=0; Index1.Continue(end_val67, step_val67); Index1 += step_val67)
				{
					{
						HTuple end_val68 = (Column2.Num())-1;
						HTuple step_val68 = 1;
						for (Index2=Index1+1; Index2.Continue(end_val68, step_val68); Index2 += step_val68)
						{
							EdgeDists[EdgeNum] = HTuple(Column2[Index2])-HTuple(Column2[Index1]);
							LeftNum[EdgeNum] = Index1;
							RightNum[EdgeNum] = Index2;
							EdgeNum += 1;
						}
					}
				}
			}
			HTuple DisDiff,Indices1;
			Hobject ObjectSelected1,ObjectSelected2;
			DisDiff = (EdgeDists-EdgeDist).Abs();
			tuple_sort_index(DisDiff, &Indices1);
			ColCenter = (HTuple(Column2[HTuple(LeftNum[HTuple(Indices1[0])])])+HTuple(Column2[HTuple(RightNum[HTuple(Indices1[0])])]))/2.0;
			select_obj(SortedRegions, &ObjectSelected1, HTuple(LeftNum[HTuple(Indices1[0])])+1);
			select_obj(SortedRegions, &ObjectSelected2, HTuple(RightNum[HTuple(Indices1[0])])+1);
			concat_obj(EdgeRegion, ObjectSelected1, &EdgeRegion);
			concat_obj(EdgeRegion, ObjectSelected2, &EdgeRegion);
			polar_trans_region_inv(EdgeRegion, &XYTransRegion,  fCenterY, fCenterX, 0, AngleExt, RadiusInner, RadiusExt, WidthPolar, HeightPolar, Width, Height, "nearest_neighbor");
		}
		HTuple Rate_Result,With_Point,Height_Point;
		Hobject RegionLines;
		Rate_Result = (ColCenter/WidthPolar)*AngleExt;
		if (Rate_Result>3.1415*2.0)
		{
			Rate_Result=Rate_Result-3.1415*2.0;
		}
		else if (Rate_Result<0)
		{
			Rate_Result=0.0;
		}
		///////////
		With_Point = fCenterX+(180*(Rate_Result.Cos()));

		Height_Point = fCenterY-(180*(Rate_Result.Sin()));
		gen_region_line(&RegionLines, Height_Point,With_Point,  fCenterY, fCenterX);

		m_modelMatchingPARA[3]=Rate_Result[0];
		m_modelMatchingPARA[0] =Radius;

		retValue =180.0*(m_modelMatchingPARA[3].D()/(3.14159));
		//
		Hobject ShowRegion;
		concat_obj(XYTransRegion, RegionLines, &ShowRegion);
		m_vErrorRgn[id] = ShowRegion;
		//
		concat_obj(rgn, ShowRegion, &rgn);
		m_vRgn[id]=rgn;

		retValue.Detach(pDetectAngle);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::RotatingAngleDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("926$\
					 397;R;FP;-10;460;1;>=#0$\
					 381;B$\
					 679;FP;0.5;5.0;0.1;1.0$\
					 433;FP;0.25;200;0.25;8.0$\
					 936;LP;-1;1;1;1$\
					 927;FP;0.5;650;0.5;141.5$\
					 387;FP;1;200;0.5;5.0");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
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
	if(  RingOrOCR > 2)
	{
		m_vRgn[id].Reset();
		return S_FALSE;
	}
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
		if(m_modelMatchingPARA[0].D() < 1.0 && m_angleMatchingPARA[0].D() < 1.0)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject RegionDetect;

		HTuple modelPARAID = m_vRgnReadData[id];
		//
		if(RingOrOCR==0)
		{	
			m_calibrationMat2d  = HTuple();
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
			vector_angle_to_rigid(0, 0, m_modelPARA[3].D(), 0, 0, m_angleMatchingPARA[3].D(), &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

			Hobject rotatedRgn,RegionMoved1;
			HTuple RowModelData,ColModelData;
			RowModelData=m_modelPARA[1];
			ColModelData=m_modelPARA[2];
			move_region (roiRgn, &RegionMoved1, -RowModelData, -ColModelData);        
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			RowModelData=m_angleMatchingPARA[1];
			ColModelData=m_angleMatchingPARA[2];
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

		//
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
		else if (RingOrOCR == 3)
		{
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = newRgn ;
				return S_FALSE;
			}
			HRegion roiRgn = m_dictCrtPathRgn[strRgnPath1];
			HTuple HomMat2D;
			vector_angle_to_rigid(0, 0, 0, m_modelMatchingPARA[1], m_modelMatchingPARA[2], m_modelMatchingPARA[3], &HomMat2D);
			HRegion RegionAffineTrans = roiRgn.AffineTransRegion(HomMat2D, "nearest_neighbor");
			if(ErosionSize >= 0.5)
				RegionDetect = RegionAffineTrans.ErosionCircle(ErosionSize).Id();
			else if(ErosionSize < -0.5)
				RegionDetect = RegionAffineTrans.ErosionCircle(abs(ErosionSize)).Id();
			else
				RegionDetect = RegionAffineTrans.Id();
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
STDMETHODIMP CBottleCapAlgo::DetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("442$\
					 167;B$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 441;LP;0;3;1;1$\
					 267;SPR$\
					 232;SPR" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DetectRegionLocationExtractOCRAlgo(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pErosionSize, BSTR* bstrOcrRgnFile)
{
	int id = (int)rgnId->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	float Radius =(float)pRadius->fltVal;
	CString sRgnPath1  = *bstrOcrRgnFile;
	string strRgnPath1 = CT2A(sRgnPath1);
	//
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
STDMETHODIMP CBottleCapAlgo::DetectRegionLocationExtractOCRAlgoHelp(BSTR* pHelpStr)
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
//图案偏心检测
STDMETHODIMP CBottleCapAlgo::DetectOcrDistOffsetAlgo(VARIANT* rgnId,VARIANT* pDistanceOffset)
{
	int id = (int)rgnId->fltVal;
	CComVariant retValue;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistanceOffset);
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

		//
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		double distNew=0.0;

		if(m_imageNumber>=1)
		{
			distNew=sqrt((m_modelMatchingPARA[1].D()-fCenterY)*(m_modelMatchingPARA[1].D()-fCenterY)+(m_modelMatchingPARA[2].D()-fCenterX)*(m_modelMatchingPARA[2].D()-fCenterX));
			//m_meanDistAdd=m_meanDistAdd+distNew;
			//m_addNumber=m_addNumber+1.0;			
			//retValue=fabs((m_meanDistAdd/(m_addNumber+0.01))-distNew);
			retValue=distNew;
		}
		else
			retValue=0.0;	

		m_vErrorRgn[id] = rgn;
		retValue.Detach(pDistanceOffset);

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistanceOffset);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DetectOcrDistOffsetAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("587$\
					 247;R;LP;0;200;1;<=#20$\
					 167;B" );
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

STDMETHODIMP CBottleCapAlgo::DetectRegionLocationAlgoOCR(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
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
	m_vInterImgs[id].Reset();
	m_bOcrRegionFlag = true;
	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vRgnExTest[id]=newRgn;
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
		if (m_bPilotImg || m_bFileUpdatedModel)
		{

			if(!_access(strRgnPath1.c_str(),0))
			{
				Hobject retRgn;
				read_region(&retRgn,strRgnPath1.c_str());
				m_dictCrtPathRgn[strRgnPath1] = retRgn;
				m_dictNewPathRgn[strRgnPath1] = retRgn;
				m_bFileUpdatedCopy = true;
				m_bFileUpdatedModel = false;
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
				string strModelPath, strModelName;
				ParseModelPath(strRgnPath1,strModelPath, strModelName);

				string strModelImg = strModelPath + "\\" + strModelName + ".tiff";
				if (!_access(strModelImg.c_str(),0))
				{
					read_image(&m_modelImageOCR,strModelImg.c_str());
				}
				HTuple ImageClass1;
				get_obj_class(m_modelImageOCR, &ImageClass1);
				if(ImageClass1!=HTuple("image"))
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
				string strModelPath, strModelName;
				ParseModelPath(strRgnPath1,strModelPath, strModelName);

				string strModelImg = strModelPath + "\\" + strModelName + ".tiff";
				if (!_access(strModelImg.c_str(),0))
				{
					read_image(&m_modelImageOCR,strModelImg.c_str());
				}
				HTuple ImageClass1;
				get_obj_class(m_modelImageOCR, &ImageClass1);
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

STDMETHODIMP CBottleCapAlgo::DetectRegionLocationAlgoOCRHelp(BSTR* pHelpStr)
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


STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vInterImgs[id].Reset();
	m_vRgnEx[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		//为空不报错，用于特殊区域检测
		retValue = 0.0;
		m_vErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
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
		ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (BlackUnion, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
			dilation_circle (WhiteUnion, &DefectRegs, 2.5);
			m_vWhiteErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::BlackDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;

	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
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
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(-999999.0f);
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::BlackDetectAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBottleCapAlgo::MinMaxGrayDetectAlgo(VARIANT* rgnId, VARIANT* pBlackThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteThresh,VARIANT* pSeriousWhitePointSize,VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;

	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int BlackThresh = (int)pBlackThresh->fltVal;
	int SeriousBlackPointSize = (int)pSeriousBlackPointSize->fltVal;
	int WhiteThresh = (int)pWhiteThresh->fltVal;
	int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		//
		Hobject  RegionBlack, RegionUnion, ConnectedRegions,SelectedRegions;
		reduce_domain(Image, rgn, &ImageReduced);
		Hobject ExpandedImage,ImageDetectNew;
		expand_domain_gray (ImageReduced, &ExpandedImage, 5);
		reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
		mean_image(ImageDetectNew, &ImageReduceMean, 3, 3);
		if (BlackThresh>255)
		{
			BlackThresh=255;
		}
		if (BlackThresh<0)
		{
			BlackThresh=0;
		}
		threshold(ImageReduceMean, &RegionBlack,0,BlackThresh);
		connection(RegionBlack, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, 9999999);
		Hobject UnionBlack;
		union1(SelectedRegions,&UnionBlack);
		//
		if (WhiteThresh>255)
		{
			WhiteThresh=255;
		}
		if (WhiteThresh<0)
		{
			WhiteThresh=0;
		}
		Hobject RegionWhite;
		threshold(ImageReduceMean, &RegionWhite,WhiteThresh,255);
		connection(RegionWhite, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousWhitePointSize, 9999999);
		Hobject UnionWhite;
		union1(SelectedRegions,&UnionWhite);
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(WhiteThresh));
			m_dictSpecialData[id].push_back(float(BlackThresh));

			HTuple AbsoluteHisto, RelativeHisto, Length, maxValue = 0;
			gray_histo (rgn, ImageReduceMean, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(AbsoluteHisto, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			int histoLength = Length[0].I();
			for (int i = 0; i < histoLength; i ++)
			{
				m_dictSpecialData[id].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;	
		//
		union2(UnionWhite, RegionBlack, &DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::MinMaxGrayDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1260$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 198;LP;1;255;1;60$\
					 416;LP;1;600;1;10$\
					 842;LP;1;255;1;160$\
					 421;LP;1;600;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//////////////////////////////////////////
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoBlackHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("492$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;50$\
					 416;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoBlack(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		Hobject BlackUnion;
		HTuple blackArea, rowBlack, colBlack;
		//
		ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(-999999.0f);
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		if(blackArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = blackArea[0].I();
			dilation_circle (BlackUnion, &BlackUnion, 2.5);
			m_vErrorRgn[id] = BlackUnion;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoBlackNumberHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("494$\
					 397;R;LP;0;100;1;<=#3$\
					 381;B$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;50$\
					 416;LP;1;200;1;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoBlackNumber(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectNumber)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}

	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		Hobject BlackUnion,ConnectedRegions;
		HTuple blackArea, rowBlack, colBlack,Number;
		//
		ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(-999999.0f);
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			sub_image ( ImageReduceMean, ImageReduced,&ImageSub, 1, 0);
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		if(blackArea <= 0)
		{
			retValue =0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			connection(BlackUnion, &ConnectedRegions);
			count_obj (ConnectedRegions, &Number);
			retValue = Number[0].I();
			dilation_circle (BlackUnion, &BlackUnion, 2.5);
			m_vErrorRgn[id] = BlackUnion;
		}
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoWhiteHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("493$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;250;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoWhite(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
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
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
			m_dictSpecialData[id].push_back(-999999.0f);
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		if(whiteArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = whiteArea[0].I();
			dilation_circle (WhiteUnion, &WhiteUnion, 2.5);
			m_vWhiteErrorRgn[id] = WhiteUnion;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}


STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoWhiteNumberHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("495$\
					 397;R;LP;0;100;1;<=#3$\
					 381;B$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;250;1;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoWhiteNumber(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectNumber)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vWhiteErrorRgn[id].Reset();
		m_vErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
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
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		Hobject WhiteUnion,ConnectedRegions;
		HTuple  whiteArea, rowWhite, colWhite,Number;
		//
		ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
		ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
			m_dictSpecialData[id].push_back(-999999.0f);
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		if(whiteArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			connection(WhiteUnion, &ConnectedRegions);
			count_obj (ConnectedRegions, &Number);
			retValue = Number[0].I();
			dilation_circle (WhiteUnion, &WhiteUnion, 2.5);
			m_vWhiteErrorRgn[id] = WhiteUnion;
		}
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoBlackWhiteNumberHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("511$\
					 397;R;LP;0;100;1;<=#3$\
					 381;B$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;50$\
					 416;LP;1;200;1;3$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;250;1;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoBlackWhiteNumber(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectNumber)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	Hobject rgn = m_vRgn[id];
	m_vRgnExTest[id].Reset();
	m_vInterImgs[id].Reset();
	m_vRgnEx[id].Reset();
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}

	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	//
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
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageReduceMean;
		Hobject WhiteUnion,ConnectedRegions;
		HTuple  whiteArea, rowWhite, colWhite,Number;

		Hobject BlackUnion,ConnectedRegionsBlack;
		HTuple blackArea, rowBlack, colBlack,NumberBlack;
		Hobject DetectUnion;
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
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		if((whiteArea<=0)&&(blackArea <= 0))
		{
			retValue =0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{	
			connection(WhiteUnion, &ConnectedRegions);
			count_obj (ConnectedRegions, &Number);
			dilation_circle (WhiteUnion, &WhiteUnion, 2.5);
			//
			connection(BlackUnion, &ConnectedRegionsBlack);
			count_obj (ConnectedRegionsBlack, &NumberBlack);
			dilation_circle (BlackUnion, &BlackUnion, 2.5);
			//
			Number=Number+NumberBlack;
			retValue = Number[0].I();
			union2(WhiteUnion,BlackUnion,&DetectUnion);
			m_vErrorRgn[id] = BlackUnion;
			m_vWhiteErrorRgn[id] = WhiteUnion;
		}
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoOCRHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("496$\
					 397;R;LP;0;1000;1;<=#20$\
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
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoOCR(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	float ErosionSize=(float)pErosionSize->fltVal;
	Hobject rgn = m_vRgn[id];
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
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
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
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (BlackUnion, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
			dilation_circle (WhiteUnion, &DefectRegs, 2.5);
			m_vWhiteErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("609$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 610;FP;0.5;60;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;600;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//旋转对比算法核心部分，简化
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


	HTuple PolarImgWidth = (AngleEndOrigin- HTuple(StartAngle).Rad())*RadiusStart;
	HTuple InterpolationMethod = "nearest_neighbor";

	//	closing_circle(BlackRegDyn, &BlackRegDyn, 2.5);
	connection(BlackRegDyn, &BlackRegConnected);
	select_shape(BlackRegConnected, &BlackDefectsPolar, "area", "and", DefectSizeLow, OriginImgWidth * OriginImgHeight);
	HTuple BlackDefectPolarArea, BlackDefectPolarRow, BlackDefectPolarCol;
	union1(BlackDefectsPolar,&BlackDefectsPolar);
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
STDMETHODIMP CBottleCapAlgo::AnnularRotaDynThresholdAlgoBW(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image = m_crtImg;
		Hobject ImageReduced, ImageExpanded;
		reduce_domain(Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ImageExpanded, 1);
		//
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		//get the params for polar trans
		float RoiWidth = m_ringPara[id].m_RoiWidth;
		float Radius = m_ringPara[id].m_Radius;

		float StartAngle = m_sectorAngle[id].m_angleStartAbs;	//逆时针转顺时针
		float EndAngle =  m_sectorAngle[id].m_angleEndAbs;

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
		polar_trans_image_ext(ImageExpanded, &PolarTransImageOrigin, fCenterY, fCenterX,  HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);
		polar_trans_image_ext(ImageExpanded, &PolarTransImageRotated, fCenterY, fCenterX, HTuple(StartAngle + RotaAngle).Rad(), AngleEndOrigin + HTuple(RotaAngle).Rad(), RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);

		//	HTuple MiddleRadius = 0.5*(RadiusStart + RadiusEnd);
		HTuple Offset = RadiusStart * HTuple(RotaAngle).Rad();
		//		HTuple OriginImgWidth, OriginImgHeight;
		//		get_image_size(Image, &OriginImgWidth, &OriginImgHeight);

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
			m_vErrorRgn[id] = newRgn;
			m_vWhiteErrorRgn[id] = newRgn;
		}
		else
		{			
			dilation_circle (BlackDefects, &BlackDefects, 2.5);
			dilation_circle (WhiteDefects, &WhiteDefects, 2.5);
			m_vErrorRgn[id] = BlackDefects;
			m_vWhiteErrorRgn[id] = WhiteDefects;			

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
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::AnnularRotaDynThresholdAlgoBWHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBottleCapAlgo::SectorRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT*pDilationSize,VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int ThresholdValue = (int)pDynThresh->fltVal;
	int DefectSize = (int) pDefectSize->fltVal;
	float RotaAngle = (float)pRotaAngle->fltVal;
	int DilationSize = (int) pDilationSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image = m_crtImg;
		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		//
		HTuple OutRadius=m_ringPara[id].m_Radius-1;
		HTuple InnerRadius=OutRadius-m_ringPara[id].m_RoiWidth+1;
		Hobject OutCircle,InnerCircle,RegionDiff,RegionCut;
		gen_circle(&OutCircle,fCenterY,fCenterX,OutRadius);
		gen_circle(&InnerCircle,fCenterY,fCenterX,InnerRadius);
		difference(OutCircle,InnerCircle,&RegionDiff);
		difference(RegionDiff,rgn,&RegionCut);
		//
		reduce_domain (Image, rgn, &ImageReduced);
		vector_angle_to_rigid(fCenterY,fCenterX, 0, fCenterY, fCenterX, HTuple(RotaAngle).Rad(), &HomMat2D);
		affine_trans_image(ImageReduced, &ImageAffinTrans, HomMat2D, "bilinear", "false");
		dyn_threshold(ImageReduced, ImageAffinTrans, &RegionDynThresh, ThresholdValue, "not_equal");
		connection(RegionDynThresh, &ConnectedRegions4);
		select_shape(ConnectedRegions4, &SelectedRegions4, "area", "and", DefectSize, 9999999);
		Hobject DefectRegs;
		union1(SelectedRegions4, &DefectRegs);
		//端剔除
		dilation_circle(RegionCut,&RegionCut,DilationSize);
		difference(DefectRegs,RegionCut,&DefectRegs);
		//
		HTuple Area, Row_test, Column_test;
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::SectorRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("609$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 610;FP;0.5;60;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;600;1;10$\
					 1286;LP;1;60;1;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::SubDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("411$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 415;LP;1;250;1;60$\
					 416;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::SubDynThresholdAlgo(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vInterImgs[id].Reset();
	m_vRgnEx[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = 0.0;
		m_vErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//
		Hobject Image = m_crtImgMult;
		Hobject ImageReduced;
		//
		Hobject R,G,B,SeriousPointDynThredImg;
		reduce_domain(Image,rgn,&ImageReduced);
		HTuple ImageChannels;
		count_channels(Image,&ImageChannels);
		if(ImageChannels[0].I()!=3)
		{
			retValue = 0.0;
			m_vErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		decompose3(ImageReduced,&R,&G,&B);
		dyn_threshold(G, R, &SeriousPointDynThredImg, SeriousBlackPointDynThresh, "dark");
		Hobject SeriousConnected,SeriousPointReg,DetectUnion;
		connection(SeriousPointDynThredImg, &SeriousConnected); 
		select_shape(SeriousConnected, &SeriousPointReg, "area", "and", SeriousBlackPointSize, 99999);
		union1(SeriousPointReg, &DetectUnion);

		HTuple Area, Row_test, Column_test;
		area_center(DetectUnion, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			Hobject DefectRegs;
			retValue = Area[0].I();
			dilation_circle (DetectUnion, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

HTuple GrayValueDifferenceSegment(HTuple& Diff, int Threshold, int DefectType)
{
	HTuple TupleSub, Sgn, Indices, Lighter, Darker;
	if(DefectType == NOT_EQUAL)
	{
		tuple_sub(Diff, Threshold, &TupleSub);
		tuple_sgn(TupleSub, &Sgn);
		tuple_find(Sgn, 1, &Lighter);
		if(Lighter == -1)
			Lighter = HTuple();

		tuple_sub(Diff, -Threshold, &TupleSub);
		tuple_sgn(TupleSub, &Sgn);
		tuple_find(Sgn, -1, &Darker);
		if(Darker == -1)
			Darker = HTuple();

		Indices = Lighter.Concat(Darker); 
	}
	else
	{
		tuple_sub(Diff, Threshold * DefectType, &TupleSub);
		tuple_sgn(TupleSub, &Sgn);
		tuple_find(Sgn, DefectType, &Indices);
		if(Indices == -1)
			Indices = HTuple();
	}
	return Indices;
}
STDMETHODIMP CBottleCapAlgo::GSurfaceAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("424$\
					 397;R;LP;0;1000;1;<=#20$\
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
STDMETHODIMP CBottleCapAlgo::GSurfaceAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
{
	//盖面垫模算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vInterImgs[id].Reset();
	m_vRgnEx[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
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
			m_dictSpecialData[id].push_back(float( (-1)*BlackThred) );
			m_dictSpecialData[id].push_back(float(WhiteThred));
			m_dictSpecialData[id].push_back(float(Min[0].D()*1000.0));
			m_dictSpecialData[id].push_back(float(Max[0].D()*1000.0));
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GLineAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBottleCapAlgo::GLineAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
{
	//盖面找线算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue         = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
	int BlackDefectSizeUpper = 90000;
	int WhiteDefectSizeUpper = 90000;
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
			m_dictSpecialData[id].clear();
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GLineNewAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("425$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 426;FP;0.1;20;0.1;1.1$\
					 427;FP;0;8;0.5;3$\
					 611;FP;1;255;0.5;8$\
					 428;LP;1;200;1;10$\
					 429;FP;0.1;20;0.1;1.0$\
					 430;FP;0;8;0.5;3$\
					 612;FP;1;255;0.5;8$\
					 431;LP;1;200;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GLineNewAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackThredHigh, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteThredHigh, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
{
	//盖面找线算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue         = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	float BlackMaskSize = (float)pBlackMaskSize->fltVal;
	double BlackThredLow = (double)pBlackThredLow->fltVal;
	int BlackDefectSize = (int)pBlackDefectSize->fltVal;
	float WhiteMaskSize = (float)pWhiteMaskSize->fltVal;
	double WhiteThredLow = (double)pWhiteThredLow->fltVal;
	int WhiteDefectSize = (int)pWhiteDefectSize->fltVal;
	double BlackThredUpper = (double)pBlackThredHigh->fltVal;
	double WhiteThredUpper = (double)pWhiteThredHigh->fltVal;
	int BlackDefectSizeUpper = 90000;
	int WhiteDefectSizeUpper = 90000;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		if(BlackThredLow > BlackThredUpper)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		if(WhiteThredLow > WhiteThredUpper)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
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
			m_dictSpecialData[id].clear();
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBottleCapAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
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
			m_dictSpecialData[id].push_back(float(EdgeThred));
			m_dictSpecialData[id].push_back(-999999.0f);
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
				m_dictSpecialData[id].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GContoursinkAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
{
	//盖面嵌环算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		Hobject ImageReduced, DerivGauss;
		reduce_domain (Image, rgn, &ImageReduced);
		derivate_gauss(ImageReduced, &DerivGauss, 5, "det");

		Hobject Region1, ConnectedRegions1, SelectedRegions1, WhiteUnion;
		threshold(DerivGauss, &Region1, WhiteThred/100.0, 1);
		connection(Region1, &ConnectedRegions1);
		select_shape(ConnectedRegions1, &SelectedRegions1, "area", "and", WhiteArea, 99999);
		union1(SelectedRegions1, &WhiteUnion);

		Hobject Regions2, ConnectedRegions2, SelectedRegions2, BlackUnion;
		threshold(DerivGauss, &Regions2, -1, (-1)*BlackThred/100.0);
		connection(Regions2, &ConnectedRegions2);
		select_shape(ConnectedRegions2, &SelectedRegions2, "area", "and", BlackArea, 99999);
		union1(SelectedRegions2, &BlackUnion);
		if (m_bDebugImg)
		{
			HTuple Rows, Columns,Grayvals,Max,Min;
			get_region_points (rgn, &Rows, &Columns);        
			get_grayval (DerivGauss, Rows, Columns,& Grayvals);
			tuple_max (Grayvals, &Max) ;
			tuple_min (Grayvals, &Min);
			m_dictSpecialData[id].push_back(float( (-1)*BlackThred) );
			m_dictSpecialData[id].push_back(float(WhiteThred));
			m_dictSpecialData[id].push_back(float(Min[0].D()*100.0));
			m_dictSpecialData[id].push_back(float(Max[0].D()*100.0));
			HTuple Sum,Log,Prod,AbsoluteHisto, RelativeHisto, Length;
			Hobject ImageScaleMax;
			scale_image_max (DerivGauss, &ImageScaleMax);
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GContoursinkAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("406$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 407;LP;1;100;1;10$\
					 408;LP;1;400;1;12$\
					 409;LP;1;100;1;80$\
					 410;LP;1;400;1;12");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//
STDMETHODIMP CBottleCapAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("380$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 382;FP;1;50;1;2$\
					 383;FP;2;50;1;10$\
					 384;LP;10;11;1;11$\
					 385;LP;-1;1;1;0$\
					 386;FP;0.1;100.0;0.1;22$\
					 387;LP;1;100;1;2");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CBottleCapAlgo::ZoningRing(Hobject Image, Hobject Ring, Hobject MiddleCircle, Hobject* ZoningReg, HTuple CenterRow, HTuple  CenterColumn, int RingWidth, HTuple* tuple_RegMean_DCoffset)
{
	//对环形区域进行分割
	Hobject ImageReduced, Rectangle;
	HTuple  Rows, Cols, RowCenter, ColumnCenter;
	HTuple  Length, Phi, HalfHeight, HalfWidth;
	HTuple  tuple_RegMean,Mean, Deviation, MeanValue;

	Hobject ObjectSelected, RegionIntersection;
	reduce_domain(Image, Ring, &ImageReduced);

	get_region_contour(MiddleCircle, &Rows, &Cols);
	tuple_length(Rows, &Length);
	tuple_gen_const(Length, CenterRow, &RowCenter);
	tuple_gen_const(Length, CenterColumn, &ColumnCenter);
	line_orientation(RowCenter, ColumnCenter, Rows, Cols, &Phi);
	tuple_gen_const(Length, 0.5, &HalfHeight);
	tuple_gen_const(Length, RingWidth/2, &HalfWidth);
	gen_rectangle2(&Rectangle, Rows, Cols, Phi, HalfWidth, HalfHeight);
	copy_obj(Rectangle, ZoningReg, 1, Length);

	tuple_RegMean = HTuple();
	intensity(Rectangle, ImageReduced, &tuple_RegMean, &Deviation);
	tuple_mean(tuple_RegMean, &MeanValue);
	tuple_sub(tuple_RegMean, MeanValue, tuple_RegMean_DCoffset);
}

double CBottleCapAlgo::Sinc(double n)
{
	if (0 == n)
	{
		return (double) 1;
	} 
	else
	{
		return (double)(sin(PI*n)/(PI*n));
	}
}

void CBottleCapAlgo::Unit_Impulse_Response(int paraLength, double angFreqStart, double angFreqCutoff, int winType, std::vector<double>& vFilterParam)
{
	signed int Count = 0;

	for (Count = -(paraLength-1)/2; Count <= (paraLength-1)/2; Count++)
	{
		vFilterParam.push_back ((angFreqCutoff/PI)*Sinc((angFreqCutoff/PI)*(double)(Count)) - (angFreqStart/PI)*Sinc((angFreqStart/PI)*(double)(Count)));
	}

	switch (winType)
	{
	case HAMMING:
		for (Count = -(paraLength-1)/2; Count <= (paraLength-1)/2; Count++)
		{
			vFilterParam[Count + (paraLength-1)/2] *= (0.54 + 0.46 * cos((2*PI*Count)/(paraLength-1)));
		}
		break;
	case BOXCAR:  //矩形窗
		break;
	default:
		break;
	}
}

double CalcSum(int index, std::vector<double>& vFilterParam, int ParaLength, std::vector<double>& vOutTemp)
{
	double dSum = 0.0;
	return std::inner_product(vFilterParam.begin(), vFilterParam.end(), vOutTemp.begin() + index, dSum);
}

void CBottleCapAlgo::BandPass_Filter(std::vector<double>& vFilterParam, int ParaLength, std::vector<double>& vInputData, std::vector<double>& vOutputData)
{
	std::vector<double> vOutTemp;
	int inputSz = (int)vInputData.size();
	vOutTemp = vInputData;
	vOutTemp.insert(vOutTemp.end(), vInputData.begin(), vInputData.end());

	int index = inputSz - inputSz % 8;
	for (int k = 0; k < index; k+=8)
	{
		vOutputData.push_back(CalcSum(k,   vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+1, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+2, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+3, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+4, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+5, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+6, vFilterParam, ParaLength, vOutTemp));
		vOutputData.push_back(CalcSum(k+7, vFilterParam, ParaLength, vOutTemp));
	}

	int nLeftover = inputSz % 8;
	for (int k = 0; k < nLeftover; k++)
	{
		vOutputData.push_back(CalcSum(k, vFilterParam, ParaLength, vOutTemp));
	}
	PhaseAdjust(vOutputData, (ParaLength-1)/2);	
}

void CBottleCapAlgo::PhaseAdjust(std::vector<double>& vOutputData, int FilterWidth)
{
	int outputSz = (int)vOutputData.size();
	std::vector<double> vOutTemp1;
	vOutTemp1.reserve(outputSz);

	vOutTemp1.assign(vOutputData.begin() + (outputSz-FilterWidth),  vOutputData.begin() + outputSz);
	vOutTemp1.insert(vOutTemp1.end(), vOutputData.begin(), vOutputData.begin() + outputSz - FilterWidth);
	std::copy(vOutTemp1.begin(), vOutTemp1.begin() + outputSz, vOutputData.begin());
}

void CBottleCapAlgo::Tuple_Replace(HTuple& tuple, HTuple Indices, HTuple ReplaceValue)
{
	HTuple  Length, SelectedIndice;
	tuple_length(Indices, &Length);
	for (int i = 0; i < Length; i++)
	{
		tuple_select(Indices, i, &SelectedIndice);
		tuple[SelectedIndice] = ReplaceValue;
	}
}

STDMETHODIMP CBottleCapAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pWinType, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	//利用带通滤波器的卷边优化算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	m_dictSpecialData[id].clear();
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	double FreqStart = (double)pFreqStart->fltVal;
	FreqStart = FreqStart/100;
	double FreqCutoff = (double)pFreqCutoff->fltVal;
	FreqCutoff = FreqCutoff/100;
	int WinType = (int)pWinType->fltVal;
	int DefectType = (int)pDefectType->fltVal;
	float WaveThreshold = (float)pWaveThreshold->fltVal;
	int MinArea = (int)pMinArea->fltVal;

	std::vector<double>  vInputData;  
	std::vector<double>  vOutputData;
	std::vector<double>  vFilterParam;

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
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		Hobject RegionDifference_ext, ImageReduced_ext;
		Hobject  Ring, ZoningReg, MiddleCircle;
		HTuple MeanDiffValues, ImageWidth, ImageHeight, Area, Row_test, Column_test;
		get_image_size (Image, &ImageWidth, &ImageHeight);
		RegionDifference_ext = rgn;
		reduce_domain(Image, RegionDifference_ext, &ImageReduced_ext);
		float MidRadius;
		if(Radius-RoiWidth > 0)
		{
			MidRadius = Radius - RoiWidth/2;
		}
		else
		{
			MidRadius = Radius/2;
		}
		gen_circle (&MiddleCircle, fCenterY, fCenterX, MidRadius);
		//Zoning the ROI and calculate the DC offset Mean 
		ZoningRing(Image, RegionDifference_ext, MiddleCircle, &ZoningReg, fCenterY, fCenterX,(int)abs(RoiWidth), &MeanDiffValues);
		HTuple  tempLength;
		tuple_length(MeanDiffValues, &tempLength);
		int InputDataLength = tempLength[0].I();
		for (int i = 0; i < InputDataLength; i++)
		{
			vInputData.push_back(MeanDiffValues[i].D());
		}

		double angFreqStart = FreqStart * 2 * PI;
		double angFreqCutoff = FreqCutoff * 2 * PI;
		double angFreqStart0 = angFreqStart * 0.5;
		double angFreqCutoff0 = angFreqCutoff * 1.2;
		if (FreqCutoff <= FreqStart)
		{
			return S_FALSE;
		}

		int ParamLength = 0;
		switch(WinType)
		{
		case 10:
			ParamLength = (int)((1.8*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
			break;
		case 11:
			ParamLength = (int)((6.6*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
			if(ParamLength>=0.5*InputDataLength)
			{
				ParamLength = (int)(0.5*InputDataLength);
			}
			break;
		default:
			ParamLength = (int)((6.6*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
			break;
		}
		if (0 == ParamLength%2)
			ParamLength++;

		Unit_Impulse_Response(ParamLength, angFreqStart, angFreqCutoff, WinType, vFilterParam);
		BandPass_Filter(vFilterParam, ParamLength, vInputData, vOutputData);

		HTuple BandpassMeanValue = HTuple();
		for (int j = 0; j < InputDataLength; j++)
		{
			BandpassMeanValue.Append((HTuple)vOutputData[j]);
		}

		HTuple absBandpassMeanValue, DefectIndices, tempDiff, sgnBandpass;
		HTuple sgnDiff, IndicesTemp;
		Hobject DefectReg, DefectRegs, DeftectRegConnection;
		tuple_sgn(BandpassMeanValue, &sgnBandpass);

		switch(DefectType)
		{
		case ALL:
			break;
		case LIGHT:
			tuple_find(sgnBandpass, -1, &IndicesTemp);
			if(IndicesTemp[0].I()>-1)
			{
				Tuple_Replace(BandpassMeanValue, IndicesTemp, 0);
			}
			break;
		case DARK:
			tuple_find(sgnBandpass, 1, &IndicesTemp);
			if(IndicesTemp[0].I()>-1)
			{
				Tuple_Replace(BandpassMeanValue, IndicesTemp, 0);
			}
			break;
		default:
			break;
		}

		tuple_abs(BandpassMeanValue, &absBandpassMeanValue);
		tuple_sub(absBandpassMeanValue, WaveThreshold, &tempDiff);
		tuple_sgn(tempDiff, &sgnDiff);
		tuple_find(sgnDiff, 1, &DefectIndices);

		if(m_bDebugImg)
		{
			HTuple  maxValue = 0;
			tuple_max(absBandpassMeanValue, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			switch(DefectType)
			{
			case ALL:
				m_dictSpecialData[id].push_back(WaveThreshold * YScaleFactor);
				m_dictSpecialData[id].push_back((-1)* WaveThreshold * YScaleFactor);
				break;
			case LIGHT:
				m_dictSpecialData[id].push_back(WaveThreshold * YScaleFactor);
				m_dictSpecialData[id].push_back(-999999.0f);
				break;
			case DARK:
				m_dictSpecialData[id].push_back(999999.0f);
				m_dictSpecialData[id].push_back((-1)* WaveThreshold * YScaleFactor);
				break;
			default:
				break;
			}
			for(int i = 0; i < InputDataLength; i++)
			{
				m_dictSpecialData[id].push_back(float(BandpassMeanValue[i].D()*YScaleFactor*(-1)));
			}
		}

		if (DefectIndices != -1)
		{
			HTuple DefectNum;
			select_obj(ZoningReg, &DefectReg, DefectIndices+1);

			Hobject DefectUnionReg;
			union1(DefectReg, &DefectUnionReg);
			connection(DefectUnionReg, &DeftectRegConnection);
			fill_up(DeftectRegConnection, &DeftectRegConnection);
			select_shape(DeftectRegConnection, &DefectReg, "area", "and", MinArea, 99999);
			count_obj(DefectReg, &DefectNum);
			union1 (DefectReg, &DefectRegs);
			area_center (DefectRegs, &Area, &Row_test, &Column_test);
			if(Area <= 0)
			{
				retValue = 0.0;
			}
			else
			{
				retValue = Area[0].I();
			}
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		else
		{
			retValue = 0.0; 
			Hobject newRgn;
			m_vErrorRgn[id] = newRgn;
		}		
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::CurlDetectAlgo2Help(BSTR* pHelpStr)
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
STDMETHODIMP CBottleCapAlgo::CurlDetectAlgo2(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
	Hobject PolarTransImage, ImageScaleMax, JuanYuanPartitioned, ResultDefect;
	HTuple PolarTransImageWidth, PolarTransImageHeight, Mean, Deviation, MeanLength, Exp, MeanDeviation;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		polar_trans_image_ext(Image, &PolarTransImage, fCenterY, fCenterX, 0, HTuple(360).Rad(), HTuple(RegionalOut), HTuple(RegionalIn), (HTuple(360).Rad())*HTuple(RegionalOut), HTuple(RegionalOut-RegionalIn), "nearest_neighbor");
		get_image_size(PolarTransImage, &PolarTransImageWidth, &PolarTransImageHeight);
		scale_image_max(PolarTransImage, &ImageScaleMax);
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
		HTuple Area, Row_test, Column_test;
		area_center (DefectRegs, &Area, &Row_test, &Column_test);
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;	
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	if (m_bPilotImg)
	{
		return S_OK;
	}

	Hobject rgn = m_vRgn[id];
	gen_empty_region(&rgn );
	HTuple area1,row1,col1;
	area_center(rgn,&area1,&row1,&col1);
	CComVariant retValue;

	if(area1<= 0) 
	{
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		intensity (rgn, Image, &MeanGray, &Deviation);

		if(m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(MeanGray[0].D()));
			m_dictSpecialData[id].push_back(-999999.0f);

			HTuple AbsoluteHisto, RelativeHisto, Length, maxValue = 0;
			gray_histo (rgn, Image, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(AbsoluteHisto, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			int histoLength = Length[0].I();
			for (int i = 0; i < histoLength; i ++)
			{
				m_dictSpecialData[id].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
			}
		}
		retValue = MeanGray[0].I();
		dilation_circle (rgn, &RegionDilat, 1.5);
		m_vErrorRgn[id] = RegionDilat;
		retValue.Detach(pGrayValueMean);
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1;<=#20$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::EccentricDetectionAlgo(VARIANT* rgnId, VARIANT* pDistance)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[id];
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistance);
		return S_FALSE;
	}
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
#ifdef NDEBUG
	try
	{
#endif
		Hobject regionOpening;
		opening_circle (rgn, &regionOpening, 5.5);
		smallest_circle (regionOpening, &m_innerRow, &m_innerColumn, &m_innerRadius);
		float innerColumn = (float)m_innerColumn[0].D();
		float innerRow    = (float)m_innerRow[0].D();
		float centerDis = sqrt(pow(fCenterX - innerColumn, 2) + pow(fCenterY - innerRow, 2));

		Hobject disLine;
		gen_region_line(&disLine, m_innerRow, m_innerColumn, (HTuple)fCenterY, (HTuple)fCenterX);
		retValue         = centerDis;
		m_vErrorRgn[id] = disLine;
		retValue.Detach(pDistance);		
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		Hobject newRgn;
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::EccentricDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("576$\
					 577;R;LP;0;100;1;<=#10$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::InnerCircleLocationAlgo(VARIANT* rgnId, VARIANT* pLowThred, VARIANT* pHighThred, VARIANT* pLowLength, VARIANT* pHighLength)
{
	int id           = (int)rgnId->fltVal;
	int lowThred      = (int)pLowThred->fltVal;
	int highThred     = (int)pHighThred->fltVal;
	double lowLength  = (double)pLowLength->fltVal;
	double highLength = (double)pHighLength->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
	if(fCenterX < 2.0|| fCenterY < 2.0)
	{
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImg = m_crtImg;
		Hobject circleRegion;
		HTuple circleArea;
		ImageProcessAlg.SegementCircleRegion(crtImg, lowThred, highThred, lowLength, highLength, circleArea, circleRegion);
		if (circleArea.Num() > 0)
		{
			m_vRgn[id] = circleRegion;
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
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::InnerCircleLocationAlgoHelp(VARIANT* pHelpStr)
{
	CComBSTR strHelp("578$\
					 167;B$\
					 579;FP;1;255;1;20$\
					 580;FP;1;255;1;40$\
					 581;FP;100;99999;1;200$\
					 582;FP;101;99999;1;600");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::ReprintDetectionAlgo(VARIANT* rgnId, VARIANT* pGrayDiff)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[id];
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pGrayDiff);
		return S_FALSE;
	}
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImg = m_crtImg;
		Hobject RegionDifference1;
		double diff = 0;
		ImageProcessAlg.RegionGrayDiff(crtImg, rgn, diff, RegionDifference1);
		retValue         = diff;
		m_vErrorRgn[id] = RegionDifference1;
		retValue.Detach(pGrayDiff);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		Hobject newRgn;
		m_vRgn[id] = newRgn;
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::ReprintDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("583$\
					 586;R;LP;0;100;1;<=#20$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CBottleCapAlgo::ImgEnhancement(Hobject crtImg, Hobject rgn, Hobject &imgReduced)
{
	Hobject rgnErosion, rgnErosion1, rgnDifference1, imgResult, imgScaleMax, rgnErosion2, imgReduced2;
	erosion_circle (rgn, &rgnErosion, 1.5);
	dilation_circle (rgn, &rgnErosion1, 6.5);
	difference (rgnErosion1, rgnErosion, &rgnDifference1);
	reduce_domain (crtImg, rgnErosion1, &imgReduced2);

	paint_region (rgnDifference1, imgReduced2, &imgResult, 100, "fill");
	scale_image_max (imgResult, &imgScaleMax);
	erosion_circle (rgn, &rgnErosion2, 3.5);
	reduce_domain (imgScaleMax, rgnErosion2, &imgReduced);
}
STDMETHODIMP CBottleCapAlgo::WLineDetectionAlgo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper, VARIANT* pDefectArea)
{
	int id           = (int)rgnId->fltVal;
	float maskSize    = (float)pMaskSize->fltVal;
	double thredLow   = (double)pThredLow->fltVal;
	double thredUpper = (double)pThredUpper->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[id];
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		Hobject crtImg = m_crtImg;
		Hobject imgReduced, defectRgn;
		HTuple Length, maxLength;
		ImgEnhancement(crtImg, rgn, imgReduced);
		ImageProcessAlg.LinesGaussScratch(imgReduced, maskSize, thredLow, thredUpper, Length, defectRgn);
		if(Length.Num() > 0)
		{
			tuple_max (Length, &maxLength);
			retValue = maxLength[0].I();
			dilation_circle(defectRgn, &defectRgn, 2.5);
			m_vErrorRgn[id] = defectRgn;
		}
		else
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::WLineDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("425$\
					 397;R;FP;0;1000;4;<=#5$\
					 381;B$\
					 429;FP;0.1;20;0.1;1.2$\
					 430;FP;0.1;10;0.1;2$\
					 574;FP;0.1;10;0.1;3.5");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::WSpotDetectionAlgo(VARIANT* rgnId, VARIANT* pSpotMaskSize, VARIANT* pSpotThred, VARIANT* pDefectArea)
{
	int id               = (int)rgnId->fltVal;
	int spotMaskSize    = (int)pSpotMaskSize->fltVal;
	double spotThred      = (double)pSpotThred->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[id];
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImg = m_crtImg;
		Hobject imgReduced, expandedImage, imgMean, rgnDynThresh1;
		Hobject rgnDilation, connectedRegions, selectedRegions;
		HTuple Area1, Row1, Column1;
		ImgEnhancement(crtImg, rgn, imgReduced);
		expand_domain_gray(imgReduced, &expandedImage, 2);

		mean_image(expandedImage, &imgMean, spotMaskSize, spotMaskSize);
		dyn_threshold(imgReduced, imgMean, &rgnDynThresh1, spotThred, "light");
		area_center(rgnDynThresh1, &Area1, &Row1, &Column1);		
		if (Area1 < 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			Hobject defectRgn;
			retValue = Area1[0].I();
			dilation_circle(rgnDynThresh1, &defectRgn, 2.5);
			m_vErrorRgn[id] = defectRgn;
		}
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::WSpotDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("493$\
					 397;R;LP;0;1000;2;<=#5$\
					 381;B$\
					 417;LP;3;101;2;7$\
					 420;LP;1;255;1;40");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::WBlockDetectionAlgo(VARIANT* rgnId, VARIANT* pBlockThred, VARIANT* pMinArea, VARIANT* pDefectArea)
{
	int id               = (int)rgnId->fltVal;
	double blockThred     = (double)pBlockThred->fltVal;
	float minArea         = pMinArea->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[id];
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImg = m_crtImg;
		Hobject imgReduced;
		Hobject selectedRegions;
		HTuple Area2, Row2, Column2;
		ImgEnhancement(crtImg, rgn, imgReduced);
		ImageProcessAlg.WhiteBlockDetect(imgReduced, rgn, blockThred, minArea, selectedRegions);
		area_center(selectedRegions, &Area2, &Row2, &Column2);
		if (Area2 < 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			Hobject defectRgn;
			retValue = Area2[0].I();
			dilation_circle(selectedRegions, &defectRgn, 2.5);
			m_vErrorRgn[id] = defectRgn;
		}
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::WBlockDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("493$\
					 397;R;LP;0;1000;12;<=#10$\
					 381;B$\
					 418;LP;1;255;1;24$\
					 419;LP;1;250;1;12");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;

	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::ReloadExternalFile(BSTR* bstrPath)
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
		m_bFileUpdatedModel=true;
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

STDMETHODIMP CBottleCapAlgo::RRegionDetectAlgo(VARIANT* rgnId,VARIANT* pSmoothPara,VARIANT* pMaskSize,VARIANT* pThredLow,VARIANT* pARMin,VARIANT*pDetectArea )
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	///////////////////////////////////////////////////
	float m_smoothSize = (float)pSmoothPara->fltVal;
	int m_maskSize=(int)pMaskSize->fltVal;
	int m_thredLow=(int)pThredLow->fltVal;
	int m_ARMin=(int)pARMin->fltVal;
	//int m_minRegionNumber=(int)pMinRegionNumber->fltVal;
	///////////////////////////////////////////////////
	Hobject  ImageMean,ImageMedian,ImageMin,  RegionDifference,RegionDifference_ext,Region_ext,Region_inner,RegionIntersection1;
	Hobject ImageReduced_ext,DerivGauss ,ConnectedRegions1,SelectedRegions1;
	Hobject Regions_detect,RegionUnion1_All;
	HTuple  Area_White,Row_Test,Column_Test;//,pDetectArea
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject Image=m_crtImg;
		RegionDifference=rgn;
		//
		fill_up(RegionDifference,&Region_ext);
		difference(Region_ext,RegionDifference,&Region_inner);
		dilation_circle(Region_ext,&Region_ext,5);
		difference(Region_ext,Region_inner,&RegionDifference_ext);
		//////////////////////////////
		reduce_domain(Image, RegionDifference_ext, &ImageReduced_ext);
		median_image(ImageReduced_ext, &ImageMedian, "circle",m_smoothSize, "continued");//1.0
		/////////////////////////////////////////////////
		corner_response(ImageMedian, &ImageMean,m_maskSize , 0.05);//3；0.05，变小时(0.04)容易检测到边缘点
		//
		gray_erosion_shape(ImageMean, &ImageMin, 3, 3, "rhombus");
		//
		threshold(ImageMin, &Regions_detect,m_thredLow , 255);//20
		m_regionDetectR=Regions_detect;
		//
		intersection(RegionDifference, Regions_detect, &RegionIntersection1);
		connection(RegionIntersection1, &ConnectedRegions1);
		select_shape(ConnectedRegions1, &SelectedRegions1, "area", "and", m_ARMin, 99999);
		union1(SelectedRegions1,&RegionUnion1_All);
		area_center(RegionUnion1_All, &Area_White, &Row_Test, &Column_Test);
		/////////
		if(Area_White<=0)
		{
			retValue = 0;
		}
		else
		{
			retValue = Area_White[0].I();
		}
		dilation_circle (RegionUnion1_All, &RegionUnion1_All, 2.5);
		m_vErrorRgn[id] = RegionUnion1_All;
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::RRegionDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("422$\
					 200;R;LP;1;1000;1;<=#20$\
					 167;B$\
					 417;FP;0.5;4.5;0.1;1.0$\
					 196;LP;3;21;1;7$\
					 191;LP;0;255;1;1$\
					 421;LP;1;300;1;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::RRegionDetectBigAlgo(VARIANT* rgnId,VARIANT* pDiffWhite,VARIANT* pARMax,VARIANT*pDetectArea )
{
	//仅应用于环形区域
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	float m_center_x=0.0;
	float m_center_y=0.0;
	m_center_x=m_vPos[0].m_x;
	m_center_y=m_vPos[0].m_y;

	///////////////////////////////////////////////////
	int diffWhite=(int)pDiffWhite->fltVal;
	int m_ARMax=(int)pARMax->fltVal;
	///////////////////////////////////////////////////
	Hobject  ImageMedian_Big,ImageRate,  RegionDifference_Ext;
	Hobject  ImageReduced_Ext_Big,Region3_White,RegionErosion_White,RegionDilation_White;
	Hobject  RegionIntersection2,ConnectedRegions2,SelectedRegions2,RegionUnion_Big;
	HTuple Area_White,Row_Test,Column_Test,HomMat2D;//,pDetectArea
	Area_White[0]=0;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject Image=m_crtImg;
		RegionDifference_Ext=rgn;
		//*环检测**************/////////////////////////////
		reduce_domain(Image, RegionDifference_Ext, &ImageReduced_Ext_Big);
		median_image(ImageReduced_Ext_Big, &ImageMedian_Big, "circle",3, "continued");//1.0
		vector_angle_to_rigid(HTuple(m_center_y),HTuple(m_center_x), 0, HTuple(m_center_y),HTuple(m_center_x), HTuple(12.5).Rad(), &HomMat2D);
		affine_trans_image(ImageMedian_Big, &ImageRate, HomMat2D, "bilinear", "false");
		dyn_threshold(ImageMedian_Big, ImageRate, &Region3_White, diffWhite, "light");
		erosion_circle(Region3_White, &RegionErosion_White,1.5);
		dilation_circle(RegionErosion_White, &RegionDilation_White,2);
		//大缺陷检测//////////////////////////////////////////////////
		intersection(RegionDifference_Ext,RegionDilation_White, &RegionIntersection2);
		connection(RegionIntersection2, &ConnectedRegions2);
		select_shape(ConnectedRegions2, &SelectedRegions2, "area", "and",m_ARMax, 99999);
		union1(SelectedRegions2,&RegionUnion_Big);
		area_center(RegionUnion_Big, &Area_White, &Row_Test, &Column_Test);
		if(Area_White<=0)
		{
			retValue = 0;
		}
		else
		{
			retValue = Area_White[0].I();
		}
		dilation_circle (RegionUnion_Big, &RegionUnion_Big, 2.5);
		m_vErrorRgn[id] = RegionUnion_Big;
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::RRegionDetectBigAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("435$\
					 200;R;LP;1;1000;1;<=#20$\
					 167;B$\
					 250;LP;1;255;1;20$\
					 421;LP;2;600;1;20");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CBottleCapAlgo::ErrorDetect(Hobject ImageReduced, int MaskSize, float SeriousPointDynThresh, int SeriousPointSize, Hobject& DetectUnion, bool defectType)
{
	Hobject ImageMean, PointDynThredImg, PointReg, Union1;
	mean_image(ImageReduced, &ImageMean, MaskSize, MaskSize);

	//*找大范围黑/白缺陷，适用于缺陷浅以及数量比较多的情况，参数越细，误报越高
	/*dyn_threshold(ImageReduced, ImageMean, &PointDynThredImg, PointDynThresh, "dark");
	select_shape(PointDynThredImg, &PointReg, "area", "and", PointSize, 99999);
	union1(PointReg, &Union1);*/

	//*找单一块极黑/白缺陷，适用于伤的比较深，污点比较黑/白的情况
	Hobject SeriousPointDynThredImg, SeriousConnected, SeriousPointReg, Union2;
	if (defectType == 0)
	{
		dyn_threshold(ImageReduced, ImageMean, &SeriousPointDynThredImg, SeriousPointDynThresh, "dark");
	}
	else
	{
		dyn_threshold(ImageReduced, ImageMean, &SeriousPointDynThredImg, SeriousPointDynThresh, "light");
	}
	connection(SeriousPointDynThredImg, &SeriousConnected); 
	select_shape(SeriousConnected, &SeriousPointReg, "area", "and", SeriousPointSize, 99999);
	union1(SeriousPointReg, &DetectUnion);
}
void CBottleCapAlgo::SurfaceDent(Hobject Image)
{
	//注胶头判断
	float radius = 25;
	float innerRadius = 40;
	float outerRadius = 50;
	float m_center_x = 0.0;
	float m_center_y = 0.0;
	m_center_x = m_vPos[0].m_x;
	m_center_y = m_vPos[0].m_y;
	Hobject Circle, Circle1, Circle2, annularRegion, ImageReduced1, Region1, Region2, RegionUnion;
	Hobject RegionDilation, ImageAffinTrans, ImageReduced2, newImage;
	HTuple centerMean, centerDeviation, annularMean, annularDeviation, HomMat2D, Rows, Columns, Grayval;
	copy_image (Image, &newImage);
	gen_circle(&Circle, m_center_y, m_center_x, radius);
	intensity (Circle, newImage, &centerMean, &centerDeviation);
	gen_circle (&Circle1, m_center_y, m_center_x, innerRadius);
	gen_circle (&Circle2, m_center_y, m_center_x, outerRadius);
	difference (Circle2, Circle1, &annularRegion);
	//去除环形区域内的极黑点和极白点的影响
	reduce_domain (newImage, annularRegion, &ImageReduced1);
	threshold (ImageReduced1, &Region1, 147, 255);
	threshold (ImageReduced1, &Region2, 0, 90);
	union2 (Region1, Region2, &RegionUnion);
	dilation_circle (RegionUnion, &RegionDilation, 1.5);
	vector_angle_to_rigid (m_center_y, m_center_x, 0, m_center_y, m_center_x, HTuple(8).Rad(), &HomMat2D);
	affine_trans_image (ImageReduced1, &ImageAffinTrans, HomMat2D, "nearest_neighbor", "false");
	//从旋转后的图像中切割下相同的区域来代替缺陷区域灰度值
	reduce_domain (ImageAffinTrans, RegionDilation, &ImageReduced2);
	get_region_points (RegionDilation, &Rows, &Columns);
	HTuple Numbers;
	tuple_length(Rows,&Numbers);
	if (Numbers>=2)
	{
		get_grayval (ImageReduced2, Rows, Columns, &Grayval);
		set_grayval (ImageReduced1, Rows, Columns, Grayval);
	}
	intensity (annularRegion, newImage, &annularMean, &annularDeviation);
	double meanDiff = fabs(annularMean[0].D()- centerMean[0].D());
	double deviationDiff = fabs(annularDeviation[0].D() - centerDeviation[0].D());
	if((meanDiff >= 5.0)||(deviationDiff >= 1.5))
	{
		m_nCrtInjectorID = 0;
	}
	else
	{
		if(m_nCrtInjectorID >= 0)
		{
			m_nCrtInjectorID ++;
		}
	}
}
STDMETHODIMP CBottleCapAlgo::GlueInjectorDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pGrayDiffValue, VARIANT* pDetectArea)
{
	// 注胶区域动态灰度值算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue         = -1;
		m_nCrtInjectorID = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int BlackMaskSize				  =	  (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh  =	  (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize		  =	  (int) pSeriousBlackPointSize->fltVal;
	int WhiteMaskSize				  =	  (int) pWhiteMaskSize->fltVal;
	float SeriousWhitePointDynThresh  =	  (float) pSeriousWhitePointDynThresh->fltVal;
	int SeriousWhitePointSize		  =	  (int) pSeriousWhitePointSize->fltVal;
	float grayDiffValue				  =   (float)pGrayDiffValue->fltVal;
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
		//注胶头判断
		Hobject regionUnion;
		//SurfaceDent(Image, BlackThred, BlackArea, WhiteThred, WhiteArea, regionUnion);
		SurfaceDent(Image);
		//光盖检测
		HTuple Mean, Deviation;
		intensity (rgn, Image, &Mean, &Deviation);
		double crtMeanValue = Mean[0].D();
		double meanValue = 0.0;
		m_addNumber ++;
		m_meanGray = m_meanGray + crtMeanValue;
		meanValue = m_meanGray/m_addNumber;
		if (fabs(crtMeanValue - meanValue)> grayDiffValue)
		{
			m_vErrorRgn[id] = rgn;
			retValue=-1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		//动态灰度值判断
		Hobject ImageReduced, ExpandedImage;
		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);
		reduce_domain(ExpandedImage, rgn, &ImageReduced);

		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//*===================================================================
		//*找黑色划痕与污点
		ErrorDetect(ImageReduced, BlackMaskSize, SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion, 0);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//*===================================================================
		//*找白色划痕与污点
		ErrorDetect(ImageReduced, WhiteMaskSize, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion, 1);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			//
			Hobject ImageReduceMean;
			HTuple newMaskSize;
			newMaskSize=(BlackMaskSize+WhiteMaskSize)/2.0;
			mean_image(ImageReduced, &ImageReduceMean,newMaskSize ,newMaskSize );
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, defectRow, defectColumn;
		union2(BlackUnion, WhiteUnion, &DefectRegs);
		area_center(DefectRegs, &Area, &defectRow, &defectColumn);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GlueInjectorDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("268$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;25$\
					 416;LP;1;200;1;10$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;35$\
					 421;LP;1;250;1;10$\
					 270;LP;1;255;1;20");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//注胶检测
STDMETHODIMP CBottleCapAlgo::GlueDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pGrayDiffValue, VARIANT* pDetectArea)
{
	// 注胶区域动态灰度值算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue         = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int BlackMaskSize				  =	  (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh  =	  (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize		  =	  (int) pSeriousBlackPointSize->fltVal;
	int WhiteMaskSize				  =	  (int) pWhiteMaskSize->fltVal;
	float SeriousWhitePointDynThresh  =	  (float) pSeriousWhitePointDynThresh->fltVal;
	int SeriousWhitePointSize		  =	  (int) pSeriousWhitePointSize->fltVal;
	float grayDiffValue				  =   (float)pGrayDiffValue->fltVal;
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
		//注胶头判断
		Hobject regionUnion;
		//光盖检测
		HTuple Mean, Deviation;
		intensity (rgn, Image, &Mean, &Deviation);
		double crtMeanValue = Mean[0].D();
		double meanValue = 0.0;
		m_addNumber ++;
		m_meanGray = m_meanGray + crtMeanValue;
		meanValue = m_meanGray/m_addNumber;
		if (fabs(crtMeanValue - meanValue)> grayDiffValue)
		{
			m_vErrorRgn[id] = rgn;
			retValue=-1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		//动态灰度值判断
		Hobject ImageReduced, ExpandedImage;
		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);
		reduce_domain(ExpandedImage, rgn, &ImageReduced);

		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//*===================================================================
		//*找黑色划痕与污点
		ErrorDetect(ImageReduced, BlackMaskSize, SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion, 0);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//*===================================================================
		//*找白色划痕与污点
		ErrorDetect(ImageReduced, WhiteMaskSize, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion, 1);
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			//
			Hobject ImageReduceMean;
			HTuple newMaskSize;
			newMaskSize=(BlackMaskSize+WhiteMaskSize)/2.0;
			mean_image(ImageReduced, &ImageReduceMean,newMaskSize ,newMaskSize );
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, defectRow, defectColumn;
		union2(BlackUnion, WhiteUnion, &DefectRegs);
		area_center(DefectRegs, &Area, &defectRow, &defectColumn);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GlueDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("891$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;25$\
					 416;LP;1;200;1;10$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;35$\
					 421;LP;1;250;1;10$\
					 270;LP;1;255;1;20");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//注胶头编号检测
void CBottleCapAlgo::SurfaceDent(Hobject Image,HTuple ThresholdWhite,HTuple ThresholdBlack,HTuple MeanDiffSet,HTuple DeviationDiffSet, Hobject *AnnularRegion, Hobject *InjectRegion)
{
	//注胶头判断
	float radius = 25;
	float innerRadius = 40;
	float outerRadius = 50;
	float m_center_x = 0.0;
	float m_center_y = 0.0;
	m_center_x = m_vPos[0].m_x;
	m_center_y = m_vPos[0].m_y;
	Hobject Circle, Circle1, Circle2, annularRegion, ImageReduced1, Region1, Region2, RegionUnion;
	Hobject RegionDilation, ImageAffinTrans, ImageReduced2, newImage;
	HTuple centerMean, centerDeviation, annularMean, annularDeviation, HomMat2D, Rows, Columns, Grayval;
	copy_image (Image, &newImage);
	gen_circle(&Circle, m_center_y, m_center_x, radius);
	intensity (Circle, newImage, &centerMean, &centerDeviation);
	gen_circle (&Circle1, m_center_y, m_center_x, innerRadius);
	gen_circle (&Circle2, m_center_y, m_center_x, outerRadius);
	difference (Circle2, Circle1, &annularRegion);
	*AnnularRegion=annularRegion;
	//去除环形区域内的极黑点和极白点的影响
	reduce_domain (newImage, annularRegion, &ImageReduced1);
	threshold (ImageReduced1, &Region1, ThresholdWhite, 255);
	threshold (ImageReduced1, &Region2, 0, ThresholdBlack);
	union2 (Region1, Region2, &RegionUnion);
	dilation_circle (RegionUnion, &RegionDilation, 1.5);
	union2(RegionDilation,Circle,InjectRegion);
	vector_angle_to_rigid (m_center_y, m_center_x, 0, m_center_y, m_center_x, HTuple(8).Rad(), &HomMat2D);
	affine_trans_image (ImageReduced1, &ImageAffinTrans, HomMat2D, "constant", "false");
	//从旋转后的图像中切割下相同的区域来代替缺陷区域灰度值
	reduce_domain (ImageAffinTrans, RegionDilation, &ImageReduced2);
	get_region_points (RegionDilation, &Rows, &Columns);
	HTuple Numbers;
	tuple_length(Rows,&Numbers);
	if (Numbers>=2)
	{
		get_grayval (ImageReduced2, Rows, Columns, &Grayval);
		set_grayval (newImage, Rows, Columns, Grayval);
	}
	intensity (annularRegion, newImage, &annularMean, &annularDeviation);
	double meanDiff = fabs(annularMean[0].D()- centerMean[0].D());
	double deviationDiff = fabs(annularDeviation[0].D() - centerDeviation[0].D());
	if((meanDiff >= MeanDiffSet)||(deviationDiff >= DeviationDiffSet))
	{
		if (!m_bDebugImg)
		{
			m_nCrtInjectorID = 1;
		}
		else//不计数 添加注胶头十字标记。
		{
			if (m_nCrtInjectorID<1)
			{
				m_nCrtInjectorID = 1;
			}		
			Hobject line1,line2,lines;
			gen_region_line (&line1, Hlong(m_center_y - 12), Hlong(m_center_x),  Hlong(m_center_y + 12), Hlong(m_center_x));
			gen_region_line (&line2, Hlong(m_center_y), Hlong(m_center_x - 12),  Hlong(m_center_y), Hlong(m_center_x + 12));
			union2(line2, line1, &lines);
			dilation_circle(lines, &lines, 1.5);
			difference(*InjectRegion, lines, InjectRegion);
		}
	}
	else
	{
		if(m_nCrtInjectorID >= 1)
		{
			if (!m_bDebugImg)
			{
				m_nCrtInjectorID ++;
			}
		}
		else
		{
			if (m_bDebugImg)
			{
				m_nCrtInjectorID=1;
			}
		}
	}
}
STDMETHODIMP CBottleCapAlgo::GlueInjectorIDDetectAlgo(VARIANT* rgnId,VARIANT* pBlackPointThresh, VARIANT* pWhitePointThresh, VARIANT* pMeanGrayDiffValue, VARIANT* pDeviationGrayDiffValue, VARIANT* pMaxID, VARIANT* pDetectID)
{
	// 注胶区域动态灰度值算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	int MaxID		             =	  (int) pMaxID->fltVal;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		if(!m_bDebugImg)
		{
			if (m_nCrtInjectorID<=MaxID-1)
			{
				m_nCrtInjectorID=m_nCrtInjectorID+1;
			}
			else
			{
				m_nCrtInjectorID=1;
			}
			PrintGlueInjectorIndex(m_nCrtInjectorID);
		}
		retValue         = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectID);
		return S_FALSE;
	}
	int BlackPointThresh         =	  (int)pBlackPointThresh->fltVal;
	int WhitePointThresh         =	  (int) pWhitePointThresh->fltVal;
	float MeanGrayDiffValue	     =	  (float) pMeanGrayDiffValue->fltVal;
	float DeviationGrayDiffValue =    (float)pDeviationGrayDiffValue->fltVal;

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
		mean_image(Image,&Image,3,3);
		//注胶头判断
		Hobject regionUnion;
		//SurfaceDent(Image, BlackThred, BlackArea, WhiteThred, WhiteArea, regionUnion);
		//调试状态下不重新计数，在算子内部处理。
		Hobject InjectRegs,AnnularRegions;
		SurfaceDent(Image,WhitePointThresh,BlackPointThresh,MeanGrayDiffValue,DeviationGrayDiffValue,&AnnularRegions,&InjectRegs);
		if (m_nCrtInjectorID>MaxID)
		{
			m_nCrtInjectorID=1;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(BlackPointThresh));
			m_dictSpecialData[id].push_back(float(WhitePointThresh));

			HTuple AbsoluteHisto, RelativeHisto, Length, maxValue = 0;
			gray_histo (AnnularRegions, Image, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(AbsoluteHisto, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			int histoLength = Length[0].I();
			for (int i = 0; i < histoLength; i ++)
			{
				m_dictSpecialData[id].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
			}
		}
		if(m_nCrtInjectorID >=1)
		{
			retValue = m_nCrtInjectorID;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;	
			m_vRgn[id] = InjectRegs;
		}
		else
		{
			retValue = m_nCrtInjectorID;
			m_vErrorRgn[id] = InjectRegs;
		}
		if(!m_bDebugImg)
		{
			PrintGlueInjectorIndex(m_nCrtInjectorID);
		}
		retValue.Detach(pDetectID);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectID);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::GlueInjectorIDDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("892$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 198;LP;1;240;1;90$\
					 842;LP;20;255;1;145$\
					 893;FP;0.5;200;0.5;5.0$\
					 894;FP;0.0;200;0.5;1.5$\
					 895;LP;1;50;1;12");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::GlueInjectorIDDetectAlgo1(VARIANT* rgnId,VARIANT* pRefreshIndex,VARIANT* pStartIndex, VARIANT* pTotalInjectorNum,VARIANT* pDetectID)
{
	// 注胶头统计功能
	int id = (int)rgnId->fltVal;
	int RefreshInjectorIndex = (int)pRefreshIndex->fltVal;
	int StartIndex = (int)pStartIndex->fltVal;
	int TotalInjectorNum = (int)pTotalInjectorNum->fltVal;

	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif


		if(m_bPilotImg || m_refreshInjectorIndex_old!=RefreshInjectorIndex||m_refreshInjectorIndex_old==-1)
		{
			m_nCrtInjectorID = StartIndex-1; 
			m_refreshInjectorIndex_old = RefreshInjectorIndex;
			retValue = 0;
			retValue.Detach(pDetectID);
			PrintGlueInjectorIndex(m_nCrtInjectorID);
			return S_OK;

		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
			retValue = 0;
			retValue.Detach(pDetectID);
			PrintGlueInjectorIndex(m_nCrtInjectorID);
			return S_OK;
		}
		m_nCrtInjectorID = (m_nCrtInjectorID+1)% TotalInjectorNum;
		retValue = 0;
		retValue.Detach(pDetectID);
		PrintGlueInjectorIndex(m_nCrtInjectorID);
		return S_OK;
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectID);
		return S_FALSE;
	}
#endif
}
STDMETHODIMP CBottleCapAlgo::GlueInjectorIDDetectAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("1392$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 1393;LP;0;1;1;1$\
					 1415;LP;0;20;1;6$\
					 1416;LP;0;20;1;12");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::UVCurlAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea)
{
	//UVCurlAlgo  对应第三版UVCurlDLL2.1
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	if((Radius<RoiWidth*2.1)||(RoiWidth<1.6))
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
	Hobject PolarTransImage, JuanYuanPartitioned, ResultDefect;
	HTuple PolarTransImageWidth, PolarTransImageHeight, Mean, Deviation, MeanLength, Exp, MeanDeviation;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
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
			m_dictSpecialData[id].clear();
		}
		//
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;	
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::UVCurlAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBottleCapAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	float MinScore = (float)pMinScore->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.2, Height/1.6);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		if (MinScore>120)
		{
			MinScore=120;

		}
		int LowContrast=(int)MinScore;
		int HighContrast=(int)(MinScore*2);
		inspect_shape_model(ImageReduced, &ho_ModelImages1, &ho_ModelRegions1,1,((HTuple(LowContrast).Append(HighContrast)).Append(25)));
		//inspect_shape_model(ImageReduced, &ho_ModelImages1, &ho_ModelRegions1, 1,(int)MinScore);
		dilation_circle(ho_ModelRegions1, &ho_RegionDilation1, 2.5);

		connection(ho_RegionDilation1, &ho_ConnectedRegions);
		fill_up(ho_ConnectedRegions, &ho_RegionFillUp);
		erosion_circle(ho_RegionFillUp, &ho_RegionErosion, 2.5);
		if (Radius<50)
		{
			Radius=50;
		}
		Hobject ho_RegionOpenning;
		opening_circle (ho_RegionErosion, &ho_RegionOpenning, Radius-30);
		double AreaMax,AreaMin;
		AreaMin=(Radius-8)*(Radius-8)*3.1416;
		AreaMax=(Radius+8)*(Radius+8)*3.1416;
		select_shape(ho_RegionOpenning, &ho_SelectedRegions, "area", "and", AreaMin, AreaMax);//inner_radius
		//*
		HTuple hv_Number,hv_Number2,hv_Row,hv_Column,hv_Radius1,hv_Abs,hv_Indices;
		count_obj(ho_SelectedRegions, &hv_Number);
		count_obj(ho_RegionErosion, &hv_Number2);
		if (0 != (hv_Number<1))
		{
			if (0 != (hv_Number2>0))
			{
				smallest_circle(ho_RegionErosion, &hv_Row, &hv_Column, &hv_Radius1);
				tuple_abs(hv_Radius1-Radius, &hv_Abs);
				tuple_sort_index(hv_Abs, &hv_Indices);
				gen_circle(&ho_SelectedRegions, hv_Row.Select(hv_Indices.Select(0)), hv_Column.Select(hv_Indices.Select(0)), Radius);
			}
		}
		//*
		union1(ho_SelectedRegions, &ho_RegionUnion);
		m_centerLocationRegion=ho_RegionUnion;
		HTuple x,y;
		smallest_circle(ho_RegionUnion,&x,&y,&m_locationRadius);
		get_obj_class(ho_RegionUnion, &hv_Class);
		if (0 != (hv_Class==HTuple("region")))
		{
			area_center(ho_RegionUnion, &hv_Area1, &hv_Row3, &hv_Column3);
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
STDMETHODIMP CBottleCapAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("621$\
					 381;B$\
					 374;LP;10;2000;1;220$\
					 390;LP;1;255;1;40");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CenterLocationAlgoHS(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
		fill_up(threshed, &RegionFillUp);
		union1(RegionFillUp, &RegionUnion);
		opening_circle(RegionUnion, &RegionOpening,OutlierRemoverSize+0.5);
		m_centerLocationRegion=RegionOpening;
		get_obj_class(RegionOpening, &hv_Class);
		if (0 != (hv_Class==HTuple("region")))
		{	
			smallest_circle(RegionOpening, &hv_Row3, &hv_Column3, &CenterRadius);
			m_locationRadius=CenterRadius;
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
STDMETHODIMP CBottleCapAlgo::CenterLocationAlgoHSHelp(BSTR* pHelpStr)
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

//calibration 
STDMETHODIMP CBottleCapAlgo::CircleRegionAngleAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
{
	int id = (int)rgnId->fltVal;
	m_ringPara[id].m_Radius = (float)pRadius->fltVal;
	m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	float m_RadiusInner = Radius - RoiWidth;
	float DriftAngle1=(float)pDriftAngle1->fltVal;
	float DriftAngle2=(float)pDriftAngle2->fltVal;
	m_angleDetect = m_modelMatchingPARA[3].D();
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
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();

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
						tuple_fmod(StartAngle, 6.283, &StartAngle1);
						tuple_fmod(EndAngle, 6.283, &EndAngle1);	

						gen_circle_sector(&OuterSector, fCenterY, fCenterX, Radius, StartAngle1, EndAngle1);
						gen_circle_sector(&InnerSector, fCenterY, fCenterX, m_RadiusInner, StartAngle1, EndAngle1);
						difference(OuterSector, InnerSector, &RegionDetect); 				
						opening_circle (RegionDetect, &RegionDetect, 1);
					}


				}

				m_sectorAngle[id].m_angleStartAbs = float(StartAngle[0].D()) / float(3.14159) * 180;
				m_sectorAngle[id].m_angleEndAbs =  float(EndAngle[0].D()) / float(3.14159) * 180;
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
STDMETHODIMP CBottleCapAlgo::CircleRegionAngleAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("613$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 614;FP;0;720.0;0.5;40$\
					 615;FP;0;720.0;0.5;320"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CircleRegionAngleAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RadiusSize = (float)pRadius->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;

	m_angleDetect = m_modelMatchingPARA[3].D();
	float LocationRadius=(float)(m_locationRadius[0].I());
	float Radius =LocationRadius - RadiusSize;
	float m_RadiusInner =LocationRadius - RadiusSize-RoiWidth;

	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
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
	m_vInterImgs[id].Reset();



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
						opening_circle (RegionDetect, &RegionDetect, 1);
					} 
					else
					{
						tuple_fmod(StartAngle, 6.283, &StartAngle1);
						tuple_fmod(EndAngle, 6.283, &EndAngle1);					

						gen_circle_sector(&OuterSector, fCenterY, fCenterX, Radius, StartAngle1, EndAngle1);
						gen_circle_sector(&InnerSector, fCenterY, fCenterX, m_RadiusInner, StartAngle1, EndAngle1);
						difference(OuterSector, InnerSector, &RegionDetect); 	
						opening_circle (RegionDetect, &RegionDetect, 1);
					}
				}

				m_sectorAngle[id].m_angleStartAbs= float(StartAngle[0].D()) / float(3.14159) * 180;
				m_sectorAngle[id].m_angleEndAbs=  float(EndAngle[0].D()) / float(3.14159) * 180;
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
STDMETHODIMP CBottleCapAlgo::CircleRegionAngleAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("8002$\
					 167;B$\
					 693;FP;-2000;2000.0;0.5;10$\
					 413;FP;1.5;2000;0.5;208$\
					 614;FP;0;720.0;0.5;40$\
					 615;FP;0;720.0;0.5;320"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
//sector

STDMETHODIMP CBottleCapAlgo::SectorRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	m_sectorAngle[id].m_angelStart=DriftAngle1;
	m_sectorAngle[id].m_angelEnd=DriftAngle2;
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	m_angleDetect = m_modelMatchingPARA[3].D();


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
						tuple_fmod(StartAngle, 6.283, &StartAngle1);
						tuple_fmod(EndAngle, 6.283, &EndAngle1);	

						gen_circle_sector(&OuterSector, fCenterY, fCenterX, Radius, StartAngle1, EndAngle1);
						gen_circle_sector(&InnerSector, fCenterY, fCenterX, m_RadiusInner, StartAngle1, EndAngle1);
						difference(OuterSector, InnerSector, &RegionDetect); 			
						opening_circle (RegionDetect, &RegionDetect, 1);
					}


				}

				m_sectorAngle[id].m_angleStartAbs= float(StartAngle[0].D()) / float(3.14159) * 180;
				m_sectorAngle[id].m_angleEndAbs=  float(EndAngle[0].D()) / float(3.14159) * 180;
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
STDMETHODIMP CBottleCapAlgo::SectorRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBottleCapAlgo::SectorRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	float RadiusSize = (float)pRadius->fltVal;
	float RoiWidth = (float)pRoiWidth->fltVal;
	m_angleDetect = m_modelMatchingPARA[3].D();

	float LocationRadius=(float)(m_locationRadius[0].I());
	float Radius =LocationRadius - RadiusSize;
	float m_RadiusInner =LocationRadius - RadiusSize-RoiWidth;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
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
	m_vInterImgs[id].Reset();



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
						opening_circle (RegionDetect, &RegionDetect, 1);
					} 
					else
					{
						tuple_fmod(StartAngle, 6.283, &StartAngle1);
						tuple_fmod(EndAngle, 6.283, &EndAngle1);					

						gen_circle_sector(&OuterSector, fCenterY, fCenterX, Radius, StartAngle1, EndAngle1);
						gen_circle_sector(&InnerSector, fCenterY, fCenterX, m_RadiusInner, StartAngle1, EndAngle1);
						difference(OuterSector, InnerSector, &RegionDetect); 		
						opening_circle (RegionDetect, &RegionDetect, 1);
					}
				}

				m_sectorAngle[id].m_angleStartAbs = float(StartAngle[0].D()) / float(3.14159) * 180;
				m_sectorAngle[id].m_angleEndAbs =  float(EndAngle[0].D()) / float(3.14159) * 180;
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
STDMETHODIMP CBottleCapAlgo::SectorRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("613$\
					 167;B$\
					 693;FP;-2000;2000.0;0.5;10$\
					 413;FP;1.5;2000;0.5;208$\
					 614;FP;0;720.0;0.5;40$\
					 615;FP;0;720.0;0.5;320"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

bool CBottleCapAlgo::ReadModelsForOcr( BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,bool errorInfoLanguage,HTuple &m_modelsForOcr,HTuple& m_matrixForOcr,Hobject& m_modelImagesForOcr)
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
	gen_empty_obj(&m_imageReg);
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
					inspect_shape_model (ImageOCR, &ModelImages, &ImageReg, 1,  ((HTuple(20).Append(35)).Append(8)));
					concat_obj(m_imageReg,ImageReg,&m_imageReg);
				}
				else
				{
					append_channel(m_modelImagesForOcr, ImageOCR, &m_modelImagesForOcr);
					inspect_shape_model (ImageOCR, &ModelImages, &ImageReg, 1,  ((HTuple(20).Append(35)).Append(8)));
					concat_obj(m_imageReg,ImageReg,&m_imageReg);
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
STDMETHODIMP CBottleCapAlgo::MultOcrDetectAlgo(VARIANT* rgnId, VARIANT* pGreediness, BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize,VARIANT* pLineDetectFlage,VARIANT* pLowContrast,VARIANT* pDilationSize,VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	Hobject rgn      = m_vRgn[id];
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	CComVariant retValue;
	float greediness = (float)pGreediness->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	float SeriousWhitePointDynThresh = (float) pSeriousWhitePointDynThresh->fltVal;
	int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;
	int LineDetectFlage = (int) pLineDetectFlage->fltVal;
	int LowContrast = (int) pLowContrast->fltVal;
	int DilationSize = (int) pDilationSize->fltVal;

	int searchRadius=(int)(m_ringPara[id].m_Radius/5.0);
	m_vInterImgs[id].Reset();
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
		HRegion rgn_copy = rgn;
		if(rgn_copy.Area() <= 0) 
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
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
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		if(!m_readModelState)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		//

		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//

		Hobject ImageReduced,ImageReducedForSearch,DetectRegion,SearchRegion;
		HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;


		Hobject ImageRegionStand,ImageRegionStandAffine,StandEdges,ImageReg,ModelImages;
		Hobject interReg,LineError;
		Hobject Image=m_crtImg;
		DetectRegion=rgn;
		gen_circle(&SearchRegion, fCenterY, fCenterX, searchRadius);
		reduce_domain(Image, SearchRegion, &ImageReducedForSearch);
		//
		find_shape_models(ImageReducedForSearch,m_modelsForOcr,0, HTuple(360).Rad(), 0.5, 1,0.5, "least_squares",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M, &Model_M);
		if (0 != (Score_M>0.05))
		{
			HTuple ModelCenterY,ModelCenterX,ModelAngle,matchingHomMat2D;
			Hobject ModelImage,ModelImageTrans;	
			access_channel(m_modelImagesForOcr,&ModelImage,Model_M+1);
			get_value_matrix(m_matrixForOcr,Model_M,1,&ModelCenterY);
			get_value_matrix(m_matrixForOcr,Model_M,2,&ModelCenterX);
			get_value_matrix(m_matrixForOcr,Model_M,3,&ModelAngle);
			vector_angle_to_rigid(ModelCenterY,ModelCenterX, ModelAngle,Row_M, Column_M,Angle_M,  &matchingHomMat2D);
			affine_trans_image (ModelImage,&ModelImageTrans, matchingHomMat2D,"bilinear","false");
			//
			Hobject BlackUnion, WhiteUnion;
			HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
			reduce_domain (Image, DetectRegion, &ImageReduced);
			ImageProcessAlg.ErrorDetectOCR(ModelImageTrans,ImageReduced, SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
			if(ErosionSize>=0.5)
			{
				erosion_circle (BlackUnion, &BlackUnion,ErosionSize);
				dilation_circle (BlackUnion, &BlackUnion,ErosionSize);
			}
			area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
			//
			ImageProcessAlg.ErrorDetectOCR(ModelImageTrans,ImageReduced, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
			if(ErosionSize>=0.5)
			{
				erosion_circle (WhiteUnion, &WhiteUnion,ErosionSize);
				dilation_circle (WhiteUnion, &WhiteUnion,ErosionSize);
			}
			area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
			if (m_bDebugImg)
			{
				m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
				m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
				count_channels (ModelImageTrans, &Channels);
				if(Channels>1)
					access_channel (ModelImageTrans, &ImageOneChannelModel, Channels);
				else
					ImageOneChannelModel=ModelImageTrans;
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
						m_dictSpecialData[id].push_back(-histoAdd);
					else
						m_dictSpecialData[id].push_back(-200.0);
				}
			}
			//


			Hobject DefectRegs;
			HTuple Area, Row_test, Column_test;
			union2(BlackUnion, WhiteUnion, &DefectRegs);

			if(LineDetectFlage==1)
			{
				select_obj(m_imageReg,&ImageRegionStand,Model_M+1);
				affine_trans_region (ImageRegionStand, &StandEdges, matchingHomMat2D, "false");	
				intersection(DetectRegion,StandEdges,&StandEdges);
				inspect_shape_model (ImageReduced, &ModelImages, &ImageReg, 1,  ((HTuple(LowContrast).Append(LowContrast+10)).Append(1)));
				dilation_circle(ImageReg,&ImageReg,DilationSize);
				fill_up_shape (ImageReg, &ImageReg, "area", 1, 100);
				intersection(StandEdges,ImageReg,&interReg);
				difference(StandEdges,interReg,&LineError);
				dilation_circle (LineError, &LineError,4.5);
				erosion_circle (LineError, &LineError,4.5);
				connection(LineError, &LineError);
				select_shape(LineError, &LineError, "area", "and", 2, 999999);
				union2(DefectRegs, LineError, &DefectRegs);
			}

			area_center(DefectRegs, &Area, &Row_test, &Column_test);
			if(Area <= 0)
			{
				retValue = 0.0;
				Hobject newRgn; 
				m_vErrorRgn[id] = newRgn;
			}
			else
			{
				retValue = Area[0].I();
				dilation_circle (DefectRegs, &DefectRegs, 2.5);
				m_vErrorRgn[id] = DefectRegs;
			}
			retValue.Detach(pDetectArea);
		}
		else
		{
			retValue = -1;	
			retValue.Detach(pDetectArea);
			m_vErrorRgn[id] = SearchRegion;
			return S_FALSE;
		}
		if (m_bDebugImg)
		{
			if(LineDetectFlage==1)
			{
				Hobject ModelDetectReg;
				dilation_circle(StandEdges,&ModelDetectReg,DilationSize);
				m_vRgn[id]=ModelDetectReg;
			}
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::MultOcrDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("636$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 322;FP;0.2;1.0;0.05;0.85$\
					 535;SPM$\
					 535;SPM$\
					 535;SPM$\
					 535;SPM$\
					 415;LP;1;250;1;50$\
					 416;LP;1;200;1;10$\
					 420;LP;1;250;1;60$\
					 421;LP;1;200;1;10$\
					 501;FP;0;30;0.5;1.5$\
					 682;LP;0;1.5;1;1$\
					 683;LP;1;200.5;1;20$\
					 684;FP;0;30;0.5;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::PrintErrorDetectOcrAlgo(VARIANT* rgnId,VARIANT* pLowContrast,VARIANT* pHeightContrast,VARIANT* pDilationSize,VARIANT* pDetectArea)
{
	int id            = (int)rgnId->fltVal;
	int HeightContrast = (int) pHeightContrast->fltVal;
	int LowContrast    = (int) pLowContrast->fltVal;
	int DilationSize   = (int) pDilationSize->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	HTuple ImageClass, Area,Row_test, Column_test;
	Hobject ImageReduced,DetectRegion,SearchRegion,DefectRegs;
	Hobject ImageRegionStand,ImageRegionStandAffine,StandEdges,ImageReg,ModelImages;
	Hobject interReg,LineError,rgn;
	CComVariant retValue;

	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	rgn      = m_vRgn[id];
	if(m_bOcrRegionFlag!=true)
	{
		if(m_isChineseLanguage)
		{
			MessageBox(NULL,_T("调用图案检测函数('PrintErrorDetectOcrAlgo')前需先调用匹配函数('DetectRegionLocationAlgoOCR')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Please check whether the function('DetectRegionLocationAlgoOCR') is correct or not,before calling location function('PrintErrorDetectOcrAlgo')."), _T("Function error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
	}
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	get_obj_class(m_modelImageTransOCR, &ImageClass);
	if(ImageClass!=HTuple("image"))
	{
		if(m_isChineseLanguage)
		{
			MessageBox(NULL,_T("调用图案检测函数('PrintErrorDetectOcrAlgo')前需先调用匹配函数('DetectRegionLocationAlgoOCR')."),_T("标准图片读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		else
		{
			MessageBox(NULL,_T("Please check whether the function('DetectRegionLocationAlgoOCR') is correct or not,before calling location function('PrintErrorDetectOcrAlgo')."), _T("StandImage error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
		}
		return S_FALSE;
	}


#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg||m_bFileUpdatedCopy)
		{
			Hobject StandEdges,ModelImages;
			inspect_shape_model (m_modelImageTransOCR, &ModelImages, &m_standEdges, 1,  ((HTuple(20).Append(35)).Append(8)));
			m_bFileUpdatedCopy=false;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		DetectRegion  = rgn;
		reduce_domain(Image,DetectRegion,&ImageReduced);
		intersection(m_standEdges,DetectRegion,&m_standEdges);
		inspect_shape_model (ImageReduced, &ModelImages, &ImageReg, 1,  ((HTuple(LowContrast).Append(HeightContrast)).Append(1)));
		dilation_circle(ImageReg,&ImageReg,DilationSize);
		fill_up_shape (ImageReg, &ImageReg, "area", 1, 100);
		intersection(m_standEdges,ImageReg,&interReg);
		difference(m_standEdges,interReg,&LineError);
		dilation_circle (LineError, &LineError,4.5);
		erosion_circle (LineError, &LineError,4.5);
		connection(LineError, &LineError);
		select_shape(LineError, &DefectRegs, "area", "and", 2, 999999);

		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
		/*if (m_bDebugImg)
		{
		Hobject ModelDetectReg;
		dilation_circle(m_standEdges,&ModelDetectReg,1.5);
		m_vRgn[id]=ModelDetectReg;
		}*/
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PrintErrorDetectOcrAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("636$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 683;LP;1;255;1;20$\
					 428;LP;1;255;1;35$\
					 684;FP;0;50;0.5;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CBottleCapAlgo::MultOcrDetectExcludeTabAlgo(VARIANT* rgnId, VARIANT* pGreediness, BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File, BSTR* bstrExtractRgnFile,VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize,VARIANT* pLineDetectFlage,VARIANT* pLowContrast,VARIANT* pDilationSize,VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn      = m_vRgn[id];
	CComVariant retValue;
	float greediness = (float)pGreediness->fltVal;
	float ErosionSize=(float)pErosionSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	float SeriousWhitePointDynThresh = (float) pSeriousWhitePointDynThresh->fltVal;
	int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;
	int LineDetectFlage = (int) pLineDetectFlage->fltVal;
	int LowContrast = (int) pLowContrast->fltVal;
	int DilationSize = (int) pDilationSize->fltVal;

	int searchRadius=(int)(m_ringPara[id].m_Radius/5.0);

	if(searchRadius<35)
	{
		searchRadius=35;
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
			//
			string strRgnPath2;
			CString sRgnPath2  = *bstrExtractRgnFile;
			strRgnPath2 = CT2A(sRgnPath2);
			if(!_access(strRgnPath2.c_str(),0))
			{
				Hobject retRgn;
				read_region(&retRgn,strRgnPath2.c_str());
				m_dictCrtPathRgn[strRgnPath2] = retRgn;
				m_dictNewPathRgn[strRgnPath2] = retRgn;
				m_modelPARA[0]=0.0;
				m_modelPARA[1]=0.0;
				m_modelPARA[2]=0.0;
				m_modelPARA[3]=0.0;
				m_modelPARA[4]=0.0;
				m_modelPARA[5]=0.0;
				m_modelPARA[6]=0.0;
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
			}
		}
		//
		HRegion rgn_copy = rgn;
		if(rgn_copy.Area() <= 0) 
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
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
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		if(!m_readModelState)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		//

		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//

		Hobject ImageReduced,ImageReducedForSearch,DetectRegion,SearchRegion;
		HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;


		Hobject ImageRegionStand,ImageRegionStandAffine,StandEdges,ImageReg,ModelImages;
		Hobject interReg,LineError;
		Hobject Image=m_crtImg;
		//添加检测区域预处理
		//
		string strRgnPath2;
		CString sRgnPath2  = *bstrExtractRgnFile;
		strRgnPath2 = CT2A(sRgnPath2);
		if(m_dictCrtPathRgn.find(strRgnPath2) == m_dictCrtPathRgn.end())
		{
			DetectRegion=rgn;
		}
		else
		{
			Hobject extractRgn = m_dictCrtPathRgn[strRgnPath2];
			//
			Hobject RegionMoved1,rotatedRgn,OCRRegionAffineTrans;
			HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted,RowModelData,ColModelData;
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
			difference (rgn, OCRRegionAffineTrans, &DetectRegion);
		}
		//////
		gen_circle(&SearchRegion, fCenterY, fCenterX, searchRadius);
		reduce_domain(Image, SearchRegion, &ImageReducedForSearch);
		//
		find_shape_models(ImageReducedForSearch,m_modelsForOcr,0, HTuple(360).Rad(), 0.5, 1,0.5, "least_squares",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M, &Model_M);
		if (0 != (Score_M>0.05))
		{
			HTuple ModelCenterY,ModelCenterX,ModelAngle,matchingHomMat2D;
			Hobject ModelImage,ModelImageTrans;	
			access_channel(m_modelImagesForOcr,&ModelImage,Model_M+1);
			get_value_matrix(m_matrixForOcr,Model_M,1,&ModelCenterY);
			get_value_matrix(m_matrixForOcr,Model_M,2,&ModelCenterX);
			get_value_matrix(m_matrixForOcr,Model_M,3,&ModelAngle);
			vector_angle_to_rigid(ModelCenterY,ModelCenterX, ModelAngle,Row_M, Column_M,Angle_M,  &matchingHomMat2D);
			affine_trans_image (ModelImage,&ModelImageTrans, matchingHomMat2D,"bilinear","false");
			//
			Hobject BlackUnion, WhiteUnion;
			HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
			reduce_domain (Image, DetectRegion, &ImageReduced);
			ImageProcessAlg.ErrorDetectOCR(ModelImageTrans,ImageReduced, SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
			intersection(BlackUnion,DetectRegion,&BlackUnion);
			if(ErosionSize>=0.5)
			{
				erosion_circle (BlackUnion, &BlackUnion,ErosionSize);
				dilation_circle (BlackUnion, &BlackUnion,ErosionSize);
			}
			area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
			//
			ImageProcessAlg.ErrorDetectOCR(ModelImageTrans,ImageReduced, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
			intersection(WhiteUnion,DetectRegion,&WhiteUnion);
			if(ErosionSize>=0.5)
			{
				erosion_circle (WhiteUnion, &WhiteUnion,ErosionSize);
				dilation_circle (WhiteUnion, &WhiteUnion,ErosionSize);
			}
			area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
			if (m_bDebugImg)
			{
				m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
				m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
				count_channels (ModelImageTrans, &Channels);
				if(Channels>1)
					access_channel (ModelImageTrans, &ImageOneChannelModel, Channels);
				else
					ImageOneChannelModel=ModelImageTrans;
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
						m_dictSpecialData[id].push_back(-histoAdd);
					else
						m_dictSpecialData[id].push_back(-200.0);
				}
			}
			//


			Hobject DefectRegs;
			HTuple Area, Row_test, Column_test;
			union2(BlackUnion, WhiteUnion, &DefectRegs);

			if(LineDetectFlage==1)
			{
				select_obj(m_imageReg,&ImageRegionStand,Model_M+1);
				affine_trans_region (ImageRegionStand, &StandEdges, matchingHomMat2D, "false");
				intersection(DetectRegion,StandEdges,&StandEdges);
				inspect_shape_model (ImageReduced, &ModelImages, &ImageReg, 1,  ((HTuple(LowContrast).Append(LowContrast+10)).Append(1)));
				dilation_circle(ImageReg,&ImageReg,DilationSize);
				fill_up_shape (ImageReg, &ImageReg, "area", 1, 100);
				intersection(StandEdges,ImageReg,&interReg);
				difference(StandEdges,interReg,&LineError);
				intersection(LineError,DetectRegion,&LineError);
				dilation_circle (LineError, &LineError,4.5);
				erosion_circle (LineError, &LineError,4.5);
				connection(LineError, &LineError);
				select_shape(LineError, &LineError, "area", "and", 2, 999999);
				union2(DefectRegs, LineError, &DefectRegs);
			}

			area_center(DefectRegs, &Area, &Row_test, &Column_test);
			if(Area <= 0)
			{
				retValue = 0.0;
				Hobject newRgn; 
				m_vErrorRgn[id] = newRgn;
			}
			else
			{
				retValue = Area[0].I();
				dilation_circle (DefectRegs, &DefectRegs, 2.5);
				m_vErrorRgn[id] = DefectRegs;
			}
			retValue.Detach(pDetectArea);
		}
		else
		{
			retValue = -1;	
			retValue.Detach(pDetectArea);
			m_vErrorRgn[id] = SearchRegion;
			return S_FALSE;
		}
		if (m_bDebugImg)
		{
			if(LineDetectFlage==1)
			{
				Hobject ModelDetectReg;
				dilation_circle(StandEdges,&ModelDetectReg,DilationSize);
				m_vRgn[id]=ModelDetectReg;
			}
		}
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::MultOcrDetectExcludeTabAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("694$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 322;FP;0.2;1.0;0.05;0.85$\
					 535;SPM$\
					 535;SPM$\
					 535;SPM$\
					 535;SPM$\
					 232;SPR$\
					 415;LP;1;250;1;50$\
					 416;LP;1;200;1;10$\
					 420;LP;1;250;1;60$\
					 421;LP;1;200;1;10$\
					 501;FP;0;30;0.5;1.5$\
					 682;LP;0;1.5;1;1$\
					 683;LP;1;200.5;1;20$\
					 684;FP;0;30;0.5;3");
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
STDMETHODIMP CBottleCapAlgo::SetCurrentTaskName(BSTR* bstrTaskName,BSTR* bstrChName)
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

STDMETHODIMP CBottleCapAlgo::SetCurrentTaskName2(BSTR* bstrTaskName,BSTR* bstrChName,BSTR* bstrChNameWithoutCameraIndex)
{   
	m_strTaskName = *bstrTaskName;
	wstring m_strExePath;
	GetExePath(m_strExePath);
	m_wstrTaskPath = m_strExePath +L"\\data\\"+*bstrChName+L"\\"+m_strTaskName;
	m_strTaskPath = CW2A(m_wstrTaskPath.c_str());
	m_wstrChannelPath= m_strExePath +L"\\data\\"+*bstrChName;
	m_strChannelPath = CW2A(m_wstrChannelPath.c_str());

	m_wstrTaskPathWithoutCameraIndex = m_strExePath +L"\\data\\"+*bstrChNameWithoutCameraIndex+L"\\"+m_strTaskName;
	m_strTaskPathWithoutCameraIndex = CW2A(m_wstrTaskPathWithoutCameraIndex.c_str());
	m_wstrChannelPathWithoutCameraIndex= m_strExePath +L"\\data\\"+*bstrChNameWithoutCameraIndex;
	m_strChannelPathWithoutCameraIndex = CW2A(m_wstrChannelPathWithoutCameraIndex.c_str());
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

//Color image process, Algo.
STDMETHODIMP CBottleCapAlgo::CenterLocationForColorAlgoHS(VARIANT* rgnId,  VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	double Radius = (double)pRadius->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	Hobject newRgn;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
		get_image_size (m_crtImgMult, &Width, &Height);
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTransAll(m_crtImgMult,&Image,  ColorTransType, ChannelSelect))
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle;
			return S_FALSE;
		}
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
		m_vInterImgs[id]=Image;
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
STDMETHODIMP CBottleCapAlgo::CenterLocationForColorAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("659$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 374;LP;10;2000;1;200$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::RingLocationForColorAlgoHS(VARIANT* rgnId,  VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pRegionWidth)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	double Radius = (double)pRadius->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	float RoiWidth = (float)pRegionWidth->fltVal;
	Hobject newRgn;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
		get_image_size (m_crtImgMult, &Width, &Height);
		Hobject Image,RegionDetect,Circle2_EXT,Circle1_Inner;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTransAll(m_crtImgMult,&Image,  ColorTransType, ChannelSelect))
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle;
			return S_FALSE;
		}
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
		m_vInterImgs[id]=Image;
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Radius, Radius);   //默认图片的大小有要求
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

			m_vPos[0].m_y = (float)hv_Row3[0].D();
			m_vPos[0].m_x = (float)hv_Column3[0].D();
			HTuple RadiusExt=CenterRadius-RoiWidth;
			if(RadiusExt>0)
			{
				gen_circle(&Circle2_EXT, m_vPos[0].m_y, m_vPos[0].m_x, CenterRadius);
				gen_circle(&Circle1_Inner, m_vPos[0].m_y, m_vPos[0].m_x, RadiusExt);
				difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
				m_ringPara[id].m_RoiWidth =(float)RadiusExt[0].D() ;
				m_ringPara[id].m_Radius = (float)CenterRadius[0].D() ;
			}
			else
			{
				if(CenterRadius>0)
				{
					gen_circle(&RegionDetect, m_vPos[0].m_y, m_vPos[0].m_x, CenterRadius);
				}
				else
				{
					RegionDetect = threshed;
				}				
			}


			m_vRgn[id] = RegionDetect;


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
STDMETHODIMP CBottleCapAlgo::RingLocationForColorAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("659$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 374;LP;10;2000;1;200$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70$\
					 413;FP;1;2000;0.5;208");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::CurlOptimizeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pFreqStart, VARIANT* pFreqCutoff,VARIANT* pWinType, VARIANT* pDefectType,VARIANT* pWaveThreshold,VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	//利用带通滤波器的卷边优化算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	m_dictSpecialData[id].clear();
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	double FreqStart = (double)pFreqStart->fltVal;
	FreqStart = FreqStart/100;
	double FreqCutoff = (double)pFreqCutoff->fltVal;
	FreqCutoff = FreqCutoff/100;
	int WinType = (int)pWinType->fltVal;
	int DefectType = (int)pDefectType->fltVal;
	float WaveThreshold = (float)pWaveThreshold->fltVal;
	int MinArea = (int)pMinArea->fltVal;

	std::vector<double>  vInputData;  
	std::vector<double>  vOutputData;
	std::vector<double>  vFilterParam;

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
			m_dictSpecialData[id].clear();
		}
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image, rgn, ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectNumber);
			return S_FALSE;
		}
		m_vInterImgs[id]=Image;
		Hobject RegionDifference_ext, ImageReduced_ext;
		Hobject  Ring, ZoningReg, MiddleCircle;
		HTuple MeanDiffValues, ImageWidth, ImageHeight, Area, Row_test, Column_test;
		get_image_size (Image, &ImageWidth, &ImageHeight);
		RegionDifference_ext = rgn;
		reduce_domain(Image, RegionDifference_ext, &ImageReduced_ext);
		float MidRadius;
		if(Radius-RoiWidth > 0)
		{
			MidRadius = Radius - RoiWidth/2;
		}
		else
		{
			MidRadius = Radius/2;
		}
		gen_circle (&MiddleCircle, fCenterY, fCenterX, MidRadius);
		//Zoning the ROI and calculate the DC offset Mean 
		ZoningRing(Image, RegionDifference_ext, MiddleCircle, &ZoningReg, fCenterY, fCenterX,(int)abs(RoiWidth), &MeanDiffValues);
		HTuple  tempLength;
		tuple_length(MeanDiffValues, &tempLength);

		int InputDataLength = tempLength[0].I();
		for (int i = 0; i < InputDataLength; i++)
		{
			vInputData.push_back(MeanDiffValues[i].D());
		}

		double angFreqStart = FreqStart * 2 * PI;
		double angFreqCutoff = FreqCutoff * 2 * PI;
		double angFreqStart0 = angFreqStart * 0.5;
		double angFreqCutoff0 = angFreqCutoff * 1.2;
		if (FreqCutoff <= FreqStart)
		{
			return S_FALSE;
		}

		int ParamLength = 0;
		switch(WinType)
		{
		case 10:
			ParamLength = (int)((1.8*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
			break;
		case 11:
			ParamLength = (int)((6.6*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
			if(ParamLength>=0.5*InputDataLength)
			{
				ParamLength = (int)0.5*InputDataLength;
			}
			break;
		default:
			ParamLength = (int)((6.6*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
			break;
		}
		if (0 == ParamLength%2)
			ParamLength++;

		Unit_Impulse_Response(ParamLength, angFreqStart, angFreqCutoff, WinType, vFilterParam);
		BandPass_Filter(vFilterParam, ParamLength, vInputData, vOutputData);

		HTuple BandpassMeanValue = HTuple();
		for (int j = 0; j < InputDataLength; j++)
		{
			BandpassMeanValue.Append((HTuple)vOutputData[j]);
		}

		HTuple absBandpassMeanValue, DefectIndices, tempDiff, sgnBandpass;
		HTuple sgnDiff, IndicesTemp;
		Hobject DefectReg, DefectRegs, DeftectRegConnection;
		tuple_sgn(BandpassMeanValue, &sgnBandpass);

		switch(DefectType)
		{
		case ALL:
			break;
		case LIGHT:
			tuple_find(sgnBandpass, -1, &IndicesTemp);
			if(IndicesTemp[0].I()>-1)
			{
				Tuple_Replace(BandpassMeanValue, IndicesTemp, 0);
			}
			break;
		case DARK:
			tuple_find(sgnBandpass, 1, &IndicesTemp);
			if(IndicesTemp[0].I()>-1)
			{
				Tuple_Replace(BandpassMeanValue, IndicesTemp, 0);
			}
			break;
		default:
			break;
		}

		tuple_abs(BandpassMeanValue, &absBandpassMeanValue);
		tuple_sub(absBandpassMeanValue, WaveThreshold, &tempDiff);
		tuple_sgn(tempDiff, &sgnDiff);
		tuple_find(sgnDiff, 1, &DefectIndices);

		if(m_bDebugImg)
		{
			HTuple  maxValue = 0;
			tuple_max(absBandpassMeanValue, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			switch(DefectType)
			{
			case ALL:
				m_dictSpecialData[id].push_back(WaveThreshold * YScaleFactor);
				m_dictSpecialData[id].push_back((-1)* WaveThreshold * YScaleFactor);
				break;
			case LIGHT:
				m_dictSpecialData[id].push_back(WaveThreshold * YScaleFactor);
				m_dictSpecialData[id].push_back(-999999.0f);
				break;
			case DARK:
				m_dictSpecialData[id].push_back(999999.0f);
				m_dictSpecialData[id].push_back((-1)* WaveThreshold * YScaleFactor);
				break;
			default:
				break;
			}
			for(int i = 0; i < InputDataLength; i++)
			{
				m_dictSpecialData[id].push_back(float(BandpassMeanValue[i].D()*YScaleFactor*(-1)));
			}
		}

		if (DefectIndices != -1)
		{
			HTuple DefectNum;
			select_obj(ZoningReg, &DefectReg, DefectIndices+1);

			Hobject DefectUnionReg;
			union1(DefectReg, &DefectUnionReg);
			connection(DefectUnionReg, &DeftectRegConnection);
			fill_up(DeftectRegConnection, &DeftectRegConnection);
			select_shape(DeftectRegConnection, &DefectReg, "area", "and", MinArea, 99999);
			count_obj(DefectReg, &DefectNum);
			union1 (DefectReg, &DefectRegs);
			area_center (DefectRegs, &Area, &Row_test, &Column_test);
			if(Area <= 0)
			{
				retValue = 0.0;
			}
			else
			{
				retValue = Area[0].I();
			}
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		else
		{
			retValue = 0.0; 
			Hobject newRgn;
			m_vErrorRgn[id] = newRgn;
		}		
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CurlOptimizeForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("656$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 382;FP;1;50;1;2$\
					 383;FP;2;50;1;10$\
					 384;LP;10;11;1;11$\
					 385;LP;-1;1;1;0$\
					 386;FP;0.1;100.0;0.1;22$\
					 387;LP;1;100;1;2");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[id]=Image;
		Hobject ImageReduced, ImageReduceMean;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		//预处理
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(Image, &Image, MaskSize, MaskSize);
		//
		if(SeriousBlackPointDynThresh<254)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
			ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
			area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		}
		if(SeriousWhitePointDynThresh<254)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
			ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
			area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		}
		//

		if (m_bDebugImg &&(SeriousBlackPointDynThresh<254||SeriousWhitePointDynThresh<254))
		{
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;

		if(whiteArea>0 ||blackArea>0 )
		{
			union2(BlackUnion, WhiteUnion, &DefectRegs);
			area_center(DefectRegs, &Area, &Row_test, &Column_test);
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		else
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}

		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DynThresholdForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("660$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 412;LP;3;101;2;7$\
					 415;LP;1;255;1;60$\
					 416;LP;1;1000;1;10$\
					 417;LP;3;101;2;7$\
					 420;LP;1;255;1;60$\
					 421;LP;1;1000;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::EdgeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[id]=Image;
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
			m_dictSpecialData[id].push_back(float(EdgeThred));
			m_dictSpecialData[id].push_back(-999999.0f);
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
				m_dictSpecialData[id].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
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
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id]=DefectRegs;
			m_vWhiteErrorRgn[id].Reset();
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::EdgeForColorAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBottleCapAlgo::AnnularRotaDynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image, rgn, ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
		//

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		m_vInterImgs[id] = Image;
		reduce_domain (Image, rgn, &ImageReduced);
		//预处理
		Hobject ExpandedImage;
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);
		Hobject ImageMean;
		median_rect(ExpandedImage, &ImageMean,4,4);
		//mean_image(ExpandedImage, &ImageMean, MaskSize, MaskSize);
		reduce_domain(ImageMean, rgn, &ImageReduced);
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::AnnularRotaDynThresholdForColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("662$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 610;FP;0.5;60;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;9999;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CBottleCapAlgo::AnnularRotaDynThresholdForColorAlgo1(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		HTuple HomMat2D;
		Hobject Image;
		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;
		//
		if(!H_ColorImageTrans(m_crtImgMult,&Image, rgn, ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[id] = Image;
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		//get the params for polar trans
		HTuple InnerRadius, OuterRadius;
		region_features (rgn, "inner_radius", &InnerRadius);
		region_features (rgn, "outer_radius", &OuterRadius);

		float RoiWidth = (float)InnerRadius[0].D();
		float Radius = (float)OuterRadius[0].D();

		float StartAngle = m_sectorAngle[id].m_angleStartAbs;	//逆时针转顺时针
		float EndAngle =  m_sectorAngle[id].m_angleEndAbs;

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
		HTuple OriginImgWidth, OriginImgHeight;
		get_image_size(Image, &OriginImgWidth, &OriginImgHeight);
		Hobject BlackDefects;
		gen_empty_obj(&BlackDefects);
		HTuple  BlackDefectPolarArea;

		Hobject BlackRegDynLight, BlackRegDynDark, BlackRegDynDarkMoved, BlackRegDyn;
		Hobject BlackRegConnected, BlackDefectsPolar;
		dyn_threshold(PolarTransImageRotated, PolarTransImageOrigin, &BlackRegDyn, ThresholdValue, "not_equal");
		connection(BlackRegDyn, &BlackRegConnected);
		select_shape(BlackRegConnected, &BlackDefectsPolar, "area", "and", DefectSize, OriginImgWidth * OriginImgHeight);
		HTuple BlackDefectPolarRow, BlackDefectPolarCol;
		union1(BlackDefectsPolar,&BlackDefectsPolar);
		area_center(BlackDefectsPolar, &BlackDefectPolarArea, &BlackDefectPolarRow, &BlackDefectPolarCol);

		if (BlackDefectPolarArea > 0)
		{
			HTuple OriginImgWidth, OriginImgHeight;
			get_image_size(Image, &OriginImgWidth, &OriginImgHeight);

			HTuple AngleEndOrigin = HTuple(EndAngle).Rad();
			HTuple PolarImgHeight = RadiusStart - RadiusEnd;

			HTuple PolarImgWidth = (AngleEndOrigin- HTuple(StartAngle).Rad())*RadiusStart;
			HTuple InterpolationMethod = "nearest_neighbor";
			if (m_bDebugImg)
			{

				polar_trans_region_inv(BlackDefectsPolar, &BlackDefects, fCenterY, fCenterX, HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, OriginImgWidth, OriginImgHeight, InterpolationMethod);
				union1(BlackDefects, &BlackDefects);
			} 
			else
			{
				//在线检测阶段，只利用错误区域的重心，标记成圆形区域
				HTuple RadiusOrigin = RadiusStart - BlackDefectPolarRow;
				HTuple Theta = 1.0 * BlackDefectPolarCol / PolarImgWidth * HTuple(((HTuple)EndAngle - (HTuple)StartAngle).Abs()).Rad() + HTuple(StartAngle).Rad();

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

			}
			dilation_circle (BlackDefects, &BlackDefects, 2.5);
			m_vErrorRgn[id] = BlackDefects;
			retValue = BlackDefectPolarArea[0].I();
			retValue.Detach(pDetectArea);

		}
		else
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
			m_vWhiteErrorRgn[id] = newRgn;
		}

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::AnnularRotaDynThresholdForColorAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("8003$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 610;FP;0.5;60;0.5;3.5$\
					 630;LP;1;250;1;60$\
					 387;LP;1;9999;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DetectRegionLocationAlgoTabOCR(VARIANT* rgnId,VARIANT* pErosionSize, BSTR* bstrRgnFile)
{
	int id            = (int)rgnId->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;
	//
	HTuple tmp;
	tmp[0]=0.0;tmp[1]=0.0;tmp[2]=0.0;tmp[3]=0.0;
	tmp[4]=0.0;tmp[5]=0.0;tmp[6]=0.0;
	m_vRgnReadData[id] = tmp;
	//
	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	string strRgnPath1;
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
				read_image(&m_modelImageTabOCR,strModelImg.c_str());
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
			if(!ReadShapeModelAxieData(strModelDat, "" , &m_modelPARATab))
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

			m_modelCenterRowTab  = m_modelPARATab[1];
			m_modelCenterColTab  = m_modelPARATab[2];
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
					MessageBox(NULL,_T("调用函数('DetectRegionLocationAlgoTabOCR')前,请先检查匹配函数('MatchingParaAlgo')是否正常."),_T("函数异常提示："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
				}
				else
				{
					MessageBox(NULL,_T("Please check whether the function('MatchingParaAlgo') is correct or not,before calling location function('DetectRegionLocationAlgoTabOCR')."), _T("Function error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
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
		if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
		HTuple modelPARAID = m_vRgnReadData[id];
		HTuple ImageClass;
		get_obj_class(m_modelImageTabOCR, &ImageClass);
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
		m_modelPARATab=modelPARAID;
		//
		Hobject  RegionAffineTrans;
		HTuple ringHomMat2D;
		vector_angle_to_rigid(m_modelPARATab[1].D(),m_modelPARATab[2].D(),m_modelPARATab[3].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&ringHomMat2D);
		affine_trans_region(roiRgn,&RegionAffineTrans,ringHomMat2D,"false");
		//
		HTuple imageHomMat2D;
		vector_angle_to_rigid(m_modelCenterRowTab,m_modelCenterColTab,0.0,m_modelMatchingPARA[1].D(),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[3].D(),&imageHomMat2D);
		affine_trans_image (m_modelImageTabOCR,&m_modelImageTransTabOCR, imageHomMat2D,"bilinear","false");
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
STDMETHODIMP CBottleCapAlgo::DetectRegionLocationAlgoTabOCRHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("442$\
					 167;B$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 267;SPR");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoTabOCRHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("496$\
					 397;R;LP;0;1000;1;<=#20$\
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
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoTabOCR(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	float ErosionSize=(float)pErosionSize->fltVal;
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		Hobject Image = m_crtImg;
		Hobject ImageReduced;

		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		HTuple ImageClass;
		get_obj_class(m_modelImageTransTabOCR, &ImageClass);
		if(ImageClass!=HTuple("image"))
		{
			if(m_isChineseLanguage)
			{
				MessageBox(NULL,_T("调用图案检测函数('DynThresholdAlgoTabOCR')前需先调用匹配函数('DetectRegionLocationAlgoTabOCR')."),_T("函数调用次序错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			else
			{
				MessageBox(NULL,_T("Please check whether the function('DetectRegionLocationAlgoTabOCR') is correct or not,before calling location function('DynThresholdAlgoTabOCR')."), _T("Function error:"), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
			}
			return S_FALSE;
			//
		}
		//
		reduce_domain (Image, rgn, &ImageReduced);
		ImageProcessAlg.ErrorDetectOCR(m_modelImageTransTabOCR,ImageReduced, SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		if(ErosionSize>=0.5)
		{
			erosion_circle (BlackUnion, &BlackUnion,ErosionSize);
			dilation_circle (BlackUnion, &BlackUnion,ErosionSize);
		}
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		ImageProcessAlg.ErrorDetectOCR(m_modelImageTransTabOCR,ImageReduced, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		if(ErosionSize>=0.5)
		{
			erosion_circle (WhiteUnion, &WhiteUnion,ErosionSize);
			dilation_circle (WhiteUnion, &WhiteUnion,ErosionSize);
		}
		area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
			count_channels (m_modelImageTransTabOCR, &Channels);
			if(Channels>1)
				access_channel (m_modelImageTransTabOCR, &ImageOneChannelModel, Channels);
			else
				ImageOneChannelModel=m_modelImageTransTabOCR;
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
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
		//	m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
		//	m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
		//		m_dictSpecialData[id].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
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
			m_vErrorRgn[id] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
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

bool ArrowDiffDist(Hobject Image, Hobject AllRegions,int  ThroldLow,double DilateSize,int MaxDist,HTuple &DistDiff )
{
	// Local iconic variables
	Hobject  RegionDilation;
	Hobject  ConnectedRegions, TransRegions, RegionAffineTrans1;
	Hobject  RegionAffineTrans2, ImageReduced, Region;
	Hobject  RegionClosing, RegionDilation1, RegionIntersection1;
	Hobject  RegionDilation2, RegionIntersection2;

	// Local control variables
	HTuple  Area, Row, Column, Length, Indices;
	HTuple  distSize1, distSize2, Index1, Area1;
	HTuple  Row1, Column1, Index2, Area2, Row2,Column2;
	bool    find_ok1,find_ok2;
	find_ok1=false;
	find_ok2=false;
	//dilation_circle(AllRegions, &RegionDilation, 2.5);

	connection(AllRegions, &ConnectedRegions);
	area_center(ConnectedRegions, &Area, &Row, &Column);
	tuple_length(Area, &Length);
	if (0 != (Length!=3))
	{
		DistDiff=999;
		return false;
	}

	tuple_sort_index(Area, &Indices);
	select_obj(ConnectedRegions, &TransRegions, (Indices.Select(Length-1))+1);
	select_obj(ConnectedRegions, &RegionAffineTrans1, (Indices.Select(Length-2))+1);
	select_obj(ConnectedRegions, &RegionAffineTrans2, (Indices.Select(Length-3))+1);
	reduce_domain(Image, TransRegions, &ImageReduced);
	if (ThroldLow<1)
	{
		ThroldLow=1;
	}
	if (ThroldLow>255)
	{
		ThroldLow=254;
	}
	threshold(ImageReduced, &Region, ThroldLow, 255);
	if (DilateSize<1)
	{
		DilateSize=1.0;
	}
	if (MaxDist<4)
	{
		MaxDist=4;
	}
	if (MaxDist>201)
	{
		MaxDist=201;
	}
	closing_circle(Region, &RegionClosing, DilateSize);
	distSize1 = 0;
	distSize2 = 0;
	{
		HTuple end_val23 = MaxDist;
		HTuple step_val23 = 1;
		for (Index1=3; Index1.Continue(end_val23, step_val23); Index1 += step_val23)
		{
			dilation_circle(RegionAffineTrans1, &RegionDilation1, Index1);

			intersection(RegionDilation1, RegionClosing, &RegionIntersection1);

			area_center(RegionIntersection1, &Area1, &Row1, &Column1);
			if (0 != (Area1>5))
			{
				find_ok1=true;
				break;
			}
		}
	}
	distSize1 = Index1;
	{
		HTuple end_val35 = MaxDist;
		HTuple step_val35 = 1;
		for (Index2=3; Index2.Continue(end_val35, step_val35); Index2 += step_val35)
		{
			dilation_circle(RegionAffineTrans2, &RegionDilation2, Index2);
			intersection(RegionDilation2, RegionClosing, &RegionIntersection2);
			area_center(RegionIntersection2, &Area2, &Row2, &Column2);
			if (0 != (Area2>5))
			{
				find_ok2=true;
				break;
			}
		}
	}
	distSize2 = Index2;
	DistDiff = (distSize2-distSize1).Abs();
	if (find_ok1&find_ok2)
	{
		return true;
	}
	else
	{
		return false;
	}

}
STDMETHODIMP CBottleCapAlgo::ArrowDistOffsetAlgo(VARIANT* rgnId,VARIANT* pLowThred,VARIANT* pErosionSize,VARIANT* pBlackPointUpSize,VARIANT* pDistanceOffset)
{
	int id = (int)rgnId->fltVal;
	CComVariant retValue;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistanceOffset);
		return S_FALSE;
	}
	int LowThred = (int)pLowThred->fltVal;
	float DilateSize = (float)pErosionSize->fltVal;
	int MaxDist = (int)pBlackPointUpSize->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}

		//
		HTuple DistDiff;
		Hobject Image = m_crtImg;
		bool is_ok=ArrowDiffDist( Image,rgn,LowThred, DilateSize,MaxDist,DistDiff );
		//
		if(is_ok)
		{	
			retValue=DistDiff[0].D();			
		}
		else
			retValue=-1;	
		m_vErrorRgn[id] = rgn;
		retValue.Detach(pDistanceOffset);

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistanceOffset);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::ArrowDistOffsetAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("738$\
					 247;R;LP;0;200;1;<=#10$\
					 167;B$\
					 579;LP;1;254;1;130$\
					 443;FP;0;15.0;0.5;1.5$\
					 650;LP;3;101;1;31" );
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//方盖检测
STDMETHODIMP CBottleCapAlgo::CenterRectangleLocationAlgoHS(VARIANT* rgnId, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
{
	/*利用边缘轮廓来定位圆*/
	int id = (int)rgnId->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	m_vInterImgs[id] = m_crtImg;
	Hobject newRgn;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
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
		fill_up(threshed, &RegionFillUp);
		union1(RegionFillUp, &RegionUnion);
		opening_circle(RegionUnion, &RegionOpening,OutlierRemoverSize+0.5);
		Hobject ConnectionReg;
		connection(RegionOpening,&ConnectionReg);
		select_shape_std(ConnectionReg,&RegionOpening,"max_area", 70);
		closing_circle(RegionOpening, &RegionOpening,OutlierRemoverSize*2);
		m_centerLocationRegion=RegionOpening;
		get_obj_class(RegionOpening, &hv_Class);
		if (0 != (hv_Class==HTuple("region")))
		{	
			smallest_circle(RegionOpening, &hv_Row3, &hv_Column3, &CenterRadius);
			m_locationRadius=CenterRadius;
			gen_empty_region(&m_centerLocationRegion);
			m_centerLocationRegion=RegionOpening;
			m_vPos[0].m_y = (float)hv_Row3[0].D();
			m_vPos[0].m_x = (float)hv_Column3[0].D();
			m_vRgn[id] = RegionOpening;
		}
		else
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, 100);
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
STDMETHODIMP CBottleCapAlgo::CenterRectangleLocationAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("639$\
					 381;B$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CBottleCapAlgo::CurlDetectAlgo1(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//方盖分块求平均灰度值算法
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vInterImgs[id].Reset();
	HRegion WorkingRegion = m_vRgn[id];
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	CComVariant retValue;	
	if(WorkingRegion.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	int NumPart = (int)pNumPart->fltVal;
	float ThresholdValue = (float)pThresholdValue->fltVal;
	int GrayValue = (int)pGrayValue->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
	HImage Image = m_crtImg;
	Hlong ImgHeight;
	Hlong ImgWidth = Image.GetImageSize(&ImgHeight);
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		HTuple Columns;
		HTuple Rows = WorkingRegion.Skeleton().GetRegionContour(&Columns);
		HTuple PartsWidth = Rows.Num() / NumPart;
		if(PartsWidth<3)
			PartsWidth=3;

		if(Rows.Num()<NumPart)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		//生成分块区域
		int LineLength = 7;
		int Overlap = 2;
		HTuple StartIndices = StartIndices.GenSequence(0, Rows.Num(), PartsWidth - Overlap);
		HTuple EndIndices = StartIndices + LineLength;
		Rows.Append(Rows(0, LineLength));
		Columns.Append(Columns(0, LineLength));
		HRegionArray LineRegions = LineRegions.GenRegionLine(Rows.Select(StartIndices), Columns.Select(StartIndices), Rows.Select(EndIndices), Columns.Select(EndIndices));
		HTuple PhiTuple, Length1, Length2, Columns2, RecNumber, Deviation;
		HTuple Row2 = LineRegions.SmallestRectangle2(&Columns2, &PhiTuple, &Length1, &Length2);
		HTuple Length1Tuple;
		tuple_gen_const(StartIndices.Num(), (PartsWidth - 1) / 2.0, &Length1Tuple);
		HTuple Length2Tuple = Length2 + RoiWidth / 2 + 1;
		HRegionArray PartitionedRegs = PartitionedRegs.GenRectangle2(Row2, Columns2, PhiTuple, Length1Tuple, Length2Tuple);
		HRegionArray Rectangles = PartitionedRegs.Intersection(WorkingRegion);

		//提取相邻区域灰度值差大于设定值的Indices
		HTuple Mean = Rectangles.Intensity(Image, &Deviation);
		HTuple MeanShifted = Mean(1, Mean.Num() - 1).Append(Mean[0]);
		HTuple GreaterIndices = (Mean - MeanShifted).Abs().GreaterElem(ThresholdValue).Find(1);
		if(GreaterIndices == -1)
			GreaterIndices = HTuple();
		else
			GreaterIndices += 1;

		//提取平均灰度值小于设定值的分块Indices
		HTuple LesserIndices = Mean.LessElem(GrayValue).Find(1);
		if(LesserIndices == -1)
			LesserIndices = HTuple();
		else
			LesserIndices += 1;

		//选择对应缺陷块
		HTuple Uniq = GreaterIndices.Concat(LesserIndices).Sort().Uniq();
		Hobject DefectRecs, DefectRegs;
		select_obj(Rectangles.Id(), &DefectRecs, Uniq);
		HRegionArray DefectRegions = DefectRecs;
		HRegion DefectRegion =  DefectRegion.GenEmptyRegion();
		DefectRegion = DefectRegion.Union2(DefectRegions);

		if(DefectRegion.Area() <= 0)
		{
			retValue = 0.0;
			m_vErrorRgn[id] = Hobject();
		}
		else
		{
			retValue = DefectRegion.Area();
			HRegion DisplayRegion = DefectRegion.DilationCircle(2.5);
			m_vErrorRgn[id] = DisplayRegion.Id();
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CurlDetectAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("2013$\
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

STDMETHODIMP CBottleCapAlgo::BoundaryRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	int RoiWidth = (int)pRoiWidth->fltVal;
	int Radius =(int)pRadius->fltVal;
	float LocationRadius=(float)(m_locationRadius[0].I());
	m_ringPara[id].m_Radius = LocationRadius + Radius;
	m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}

	if (RoiWidth < 1)
		RoiWidth = 1;

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
			m_vRgnExTest[id].Reset();
			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		Hobject RegionFill;
		fill_up(m_centerLocationRegion, &RegionFill);
		if(Radius >= 1)
		{
			dilation_circle(RegionFill, &Region_EXT, Radius + 0.5);
		}
		else if(Radius <= -1)
		{
			erosion_circle(RegionFill, &Region_EXT, -Radius + 0.5);
		}
		else
		{
			Region_EXT = RegionFill;
		}
		erosion_circle(Region_EXT, &Region_Inner, RoiWidth + 0.5);
		difference(Region_EXT, Region_Inner, &RegionDetect);
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
STDMETHODIMP CBottleCapAlgo::BoundaryRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1278$\
					 167;B$\
					 693;LP;-1500;1500;1;10$\
					 413;LP;1;800;1;100"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*********************************
*
*
*BottleCap
*
*
**********************************/
void CBottleCapAlgo::P_GetEllipseLongAxis(Halcon::Hobject Ellipse, Halcon::Hobject * BaseRegionLine)
{
	HTuple BaseEllipseArea2, BaseEllipseRow2, BaseEllipseColumn2;
	HTuple BaseEllipseRadius2, BaseEllipseRadius3, BaseEllipsePhi2;
	area_center(Ellipse, &BaseEllipseArea2, &BaseEllipseRow2, &BaseEllipseColumn2);
	elliptic_axis(Ellipse, &BaseEllipseRadius2, &BaseEllipseRadius3, &BaseEllipsePhi2);
	HTuple RowPoint, ColPoint;
	get_points_ellipse(0, BaseEllipseRow2, BaseEllipseColumn2, BaseEllipsePhi2, BaseEllipseRadius2, BaseEllipseRadius3, &RowPoint, &ColPoint);
	HTuple RowPoint2,ColPoint2;
	RowPoint2 = RowPoint+((BaseEllipseRadius2*(BaseEllipsePhi2.Sin()))*2);
	ColPoint2 = ColPoint-((BaseEllipseRadius2*(BaseEllipsePhi2.Cos()))*2);
	gen_region_line(&(*BaseRegionLine), RowPoint, ColPoint, RowPoint2, ColPoint2);
	return;

}
STDMETHODIMP CBottleCapAlgo::CenterTopLocationAlgo(VARIANT* rgnId, VARIANT* pChannelSelect,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight, VARIANT* pThresholdValue)
{
	/*侧面瓶口定位*/
	int id = (int)rgnId->fltVal;
	int ChannelSelect=(int)pChannelSelect->fltVal;
	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;

	int ThresholdValue = (int)pThresholdValue->fltVal;
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image=m_crtImgMult;
		HTuple ChannelNum;
		count_channels(m_crtImgMult,&ChannelNum);
		if (ChannelNum[0].I()==3){
			access_channel(m_crtImgMult,&Image,ChannelSelect);
		}	
		Hlong Width, Height;
		get_image_size (Image, &Width, &Height);
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
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
		reduce_domain (Image, SearchRgn, &ImageReduced);
		Hobject SelectedRegions,threshed,RegionFillUp,RegionConnected;
		threshold(ImageReduced, &threshed,ThresholdValue, 255);
		fill_up(threshed, &RegionFillUp);
		connection(RegionFillUp,&RegionConnected);
		select_shape_std(RegionConnected, &SelectedRegions, "max_area", 70);

		m_CenterTopRegion=SelectedRegions;//用于图案校正时候的位置仿射变换
		if (m_bDebugImg)
		{
			concat_obj(m_vRgnEx[id],SearchRgn,&m_vRgnEx[id]);
		}

		Hobject BaseRegionXld,TopEllipse,TopEllipseBorder;
		HTuple CenterArea, CenterRow, CenterColumn;
		area_center(SelectedRegions, &CenterArea, &CenterRow, &CenterColumn);
		HTuple a=CenterArea;
		if ((CenterArea!=0)&& (CenterArea.Num()!=0))
		{	
			gen_contour_region_xld(SelectedRegions,&BaseRegionXld,"border");
			HTuple Row, Column, Phi, Radius1, Radius2, StartPhi, EndPhi, PointOrder;
			fit_ellipse_contour_xld(BaseRegionXld, "fitzgibbon", -1, 0, 0, 200, 3, 2, &Row, &Column, &Phi, &Radius1, &Radius2, &StartPhi, &EndPhi, &PointOrder);
			gen_ellipse(&TopEllipse, Row, Column, Phi, Radius1, Radius2);
			boundary(TopEllipse, &TopEllipseBorder, "outer");
			Hobject BaseRegionLine;
			P_GetEllipseLongAxis(TopEllipseBorder, &BaseRegionLine);

			Hobject BaseRegionLineDiation,RegionDiff;
			dilation_rectangle1(BaseRegionLine, &BaseRegionLineDiation, 5, 2);
			difference(TopEllipseBorder, BaseRegionLineDiation, &RegionDiff);
			Hobject RegionDiffConnected,RegionDiffSorted;
			connection(RegionDiff, &RegionDiffConnected);
			sort_region(RegionDiffConnected, &RegionDiffSorted, "first_point", "true", "row");
			HTuple BaseEllipseArea, BaseEllipseRow, BaseEllipseColumn;
			Hobject RegionSelected;
			area_center(TopEllipse, &BaseEllipseArea, &BaseEllipseRow, &BaseEllipseColumn);
			select_shape(RegionDiffSorted, &RegionSelected, "row", "and", BaseEllipseRow, 999999);
			m_topbaseLine=RegionSelected;
			m_vPos[0].m_y = (float)BaseEllipseRow[0].D();
			m_vPos[0].m_x = (float)BaseEllipseColumn[0].D();
			m_vRgn[id] = TopEllipse;
			//
			return S_OK;
		}
		else
		{
			Hobject Circle1;
			gen_circle(&Circle1, Height/2.0, Width/2.0, 20);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle1;
			return S_FALSE;
		}
		//concat_obj(m_vRgnEx[id],SearchRgn,&m_vRgnEx[id]);
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
STDMETHODIMP CBottleCapAlgo::CenterTopLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5558$\
					 381;B$\
					 658;LP;1;3;1;1$\
					 939;LP;20;500;1;100$\
					 938;LP;34;600;1;100$\
					 940;LP;1;1300;1;1000$\
					 941;LP;1;1000;1;600$\
					 1141;LP;1;255;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CBottleCapAlgo::R_EdgePointsFitCurve(Hobject SearchReg, Hobject Image,HTuple PartInterval, HTuple RemoveSize, HTuple Threshold,HTuple Transition, HTuple Select, HTuple * DefinedRow, HTuple *DefinedCol)
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
	//
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

	//
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
		HTuple RowEdge, ColEdge, Amplitude, Distance;

		if (Length1*0.5<=1)
		{
			continue;
		}
		measure_pos(Image, MsrHandle_Measure, 1, Threshold, Transition, Select, &RowEdge, &ColEdge, &Amplitude, &Distance);
		//measure_pos(Image, MsrHandle_Measure, 1, Threshold, "negative", "last", &RowEdge, &ColEdge, &Amplitude, &Distance);
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
void CBottleCapAlgo::P_EdgePointsFitCurve(Hobject SearchReg, Hobject Image,HTuple PartInterval, HTuple LineLength, HTuple RemoveSize, HTuple * DefinedRow, HTuple *DefinedCol)
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
	HTuple pRow,pCol,RowRound,ColRound,Grayval1,Width, Height,RowMax,ColumnMax;
	Hobject Region3;
	for (int i=0;i<=LineLength;i++)
	{
		pRow = BeginRow+((EndRow-BeginRow)*(i/(LineLength+0.001)));
		pCol = BeginCol+((EndCol-BeginCol)*(i/(LineLength+0.001)));
		tuple_round(pRow, &RowRound);
		tuple_round(pCol, &ColRound);
		gen_region_points(&Region3, RowRound, ColRound);
		//
		get_image_size(Image, &Width, &Height);
		tuple_max(RowRound, &RowMax);
		tuple_max(ColRound, &ColumnMax);
		//min
		HTuple RowMin,ColumnMin;
		tuple_min(RowRound, &RowMin);
		tuple_min(ColRound, &ColumnMin);
		//
		if((RowMax>=Height-1)||(ColumnMax>=Width-1)||(RowMin<=0)||(ColumnMin<=0))
		{
			continue;
		}

		//
		/*Hobject Domain,RegDiff;
		HTuple area,row,col;
		get_domain(Image,&Domain);
		difference(Region3,Domain,&RegDiff);
		area_center(RegDiff,&area,&row,&col);
		if (area>1)
		{
		continue;
		}*/
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

	HTuple Values1,Values2,Diff,absDiff,Indices,ValueX,ValueY,MatrixID_subY1,MatrixID_subY2;
	for (int Index1=0;Index1<=StepNum-1;Index1++)
	{
		get_sub_matrix(MatrixID, 0, Index1, LineLength, 1, &MatrixID_subY1);
		get_full_matrix(MatrixID_subY1, &Values1);
		get_sub_matrix(MatrixID, 1, Index1, LineLength, 1, &MatrixID_subY2);
		get_full_matrix(MatrixID_subY2, &Values2);
		tuple_sub(Values1, Values2, &Diff);
		tuple_abs(Diff,&absDiff);
		tuple_sort_index(absDiff, &Indices);
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
STDMETHODIMP CBottleCapAlgo::BaseEdgeDetectAlgo(VARIANT* rgnId, VARIANT* pChannelSelect,VARIANT* pMoveSize,VARIANT* pSearchSize, VARIANT* pThreshold,VARIANT* pTransition, VARIANT* pPointSelection,VARIANT* pRemoveSize,VARIANT* pBaseEdgeArea)
{
	int rId           = (int)rgnId->fltVal;
	int ChannelSelect=(int)pChannelSelect->fltVal;
	int MoveSize    = (int)pMoveSize->fltVal;
	int SearchSize    =(int)pSearchSize->fltVal;
	int Threshold   = (int)pThreshold->fltVal;
	int Transition = (int)pTransition->fltVal;
	int PointSelection = (int)pPointSelection->fltVal;
	int RemoveSize   = (int)pRemoveSize->fltVal;

	m_vRgnExTest[rId].Reset();
	m_vRgnEx[rId].Reset();
	m_vInterImgs[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseEdgeArea);
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
		//
		Hobject Image = m_crtImgMult;
		HTuple ChannelNum;
		count_channels(m_crtImgMult,&ChannelNum);
		if (ChannelNum==3){
			access_channel(m_crtImgMult,&Image,ChannelSelect);
		}
		//mean_image(Image,&Image,3,3);
		median_rect(Image,&Image,4,4);
		//
		m_vInterImgs[rId]=Image;
		Hobject TopSlopEdge,TopSlopEdgeDilation,TopSlopEdgeDiff;
		move_region(m_topbaseLine, &TopSlopEdge, MoveSize, 0);
		dilation_rectangle1(TopSlopEdge, &TopSlopEdgeDilation, 1, SearchSize); //实际情况调整膨胀搜索区域

		Hobject TopSlopSearchReg,TopSlopImageReduced,ImageDomain;
		get_domain(Image,&ImageDomain);
		intersection(TopSlopEdgeDilation, ImageDomain, &TopSlopSearchReg);
		Hobject ImageSearchReg;
		reduce_domain(Image,TopSlopSearchReg,&ImageSearchReg);
		scale_image_max(ImageSearchReg,&ImageSearchReg);
		HTuple DefinedRow,DefinedCol;
		//PartInterval=5;LineLength:=膨胀尺寸;RemoveSize=2

		string strTransition, strPointSelection;

		switch (Transition)
		{
		case 1:
			strTransition = "positive";
			break;
		case 0:
			strTransition = "negative";
			break;
		default:
			strTransition = "positive";
			break;
		}

		switch (PointSelection)
		{
		case 1:
			strPointSelection = "first";
			break;
		case 0:
			strPointSelection = "last";
			break;
		default:
			strPointSelection = "first";
			break;
		}

		R_EdgePointsFitCurve(TopSlopSearchReg, ImageSearchReg, 8, RemoveSize, Threshold, strTransition.c_str(), strPointSelection.c_str(),&DefinedRow, &DefinedCol);
		//P_EdgePointsFitCurve(TopSlopSearchReg, TopSlopImageReduced, 5, SearchSize, 2, &DefinedRow, &DefinedCol);
		Hobject TopSlopEdgePoints;
		HTuple TopSlopEdgeArea, TopSlopEdgeRow, TopSlopEdgeCol;
		gen_region_points(&TopSlopEdgePoints, DefinedRow, DefinedCol);
		area_center(TopSlopEdgePoints, &TopSlopEdgeArea, &TopSlopEdgeRow, &TopSlopEdgeCol);
		m_BaseEdgeRow=TopSlopEdgeRow;
		if (m_bDebugImg)
		{
			concat_obj(m_vRgnExTest[rId],TopSlopSearchReg,&m_vRgnExTest[rId]);
			concat_obj(m_vRgnExTest[rId],TopSlopEdgePoints,&m_vRgnExTest[rId]);
		}
		if (DefinedRow.Num()==0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pBaseEdgeArea);
			return S_FALSE;
		}
		Hobject Region,ABEdgeXld,ABEdgeSmoothed;
		gen_region_points(&Region, DefinedRow, DefinedCol);
		gen_contour_polygon_xld(&ABEdgeXld, DefinedRow, DefinedCol);
		smooth_contours_xld(ABEdgeXld, &ABEdgeSmoothed, 15);

		HTuple ABRow,ABCol,ABEdgeArea, ABEdgeRow, ABEdgeCol,PointOrder;
		Hobject ABEdge;
		get_contour_xld(ABEdgeSmoothed, &ABRow, &ABCol);
		//得到基部边缘。
		gen_region_contour_xld(ABEdgeSmoothed,&m_ABEdgeReg,"margin");
		Hobject RegionClip;
		move_region(m_ABEdgeReg,&RegionClip,-3,0);
		fill_up(RegionClip,&RegionClip);
		difference(m_ABEdgeReg,RegionClip,&m_ABEdgeReg);
		//
		//gen_region_polygon(&ABEdge, ABRow, ABCol);
		//area_center(ABEdge, &ABEdgeArea, &ABEdgeRow, &ABEdgeCol);
		area_center_xld(ABEdgeSmoothed, &ABEdgeArea, &ABEdgeRow, &ABEdgeCol,&PointOrder);
		//
		if (ABEdgeArea>0)
		{
			Hobject TopSlopEdgePointsDilation;
			retValue = ABEdgeArea[0].I();
			dilation_circle(TopSlopEdgePoints, &TopSlopEdgePointsDilation, 2);
			m_vErrorRgn[rId] = TopSlopEdgePointsDilation;
		}
		else
		{
			Hobject Circle;
			gen_circle(&Circle, m_imgHeight/2.0, m_imgWidth/2.0, 100);
			retValue = -1;
			m_vErrorRgn[rId] = Circle;
		}
		retValue.Detach(pBaseEdgeArea);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseEdgeArea);
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseEdgeArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::BaseEdgeDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5559$\
					 397;R;FP;0;10000;1;>=#1$\
					 381;B$\
					 658;LP;1;3;1;1$\
					 991;LP;0;700;1;58$\
					 998;LP;1;100;1;30$\
					 1304;LP;1;60;1;15$\
					 1302;LP;0;1;1;1$\
					 1303;LP;0;1;1;1$\
					 5561;LP;1;10;1;1");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::SideEdgeRegionLocationAlgo(VARIANT* rgnId,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize)
{
	int rId           = (int)rgnId->fltVal;
	int MoveSize   = (int)pMoveSize->fltVal;
	int HErosionSize   = (int)pHErosionSize->fltVal;
	int VErosionSize   = (int)pVErosionSize->fltVal;
	//
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	m_vInterImgs[rId].Reset();
	Hobject Image=m_crtImgMult;
	m_vInterImgs[rId]=Image;

	if(m_ABEdgeReg.Id() == H_EMPTY_REGION)
	{
		m_vRgn[rId] = newRgn ;
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
		Hobject ABEdgeMoved,RegionDilation;
		dilation_rectangle1(m_ABEdgeReg,&RegionDilation, 1, VErosionSize);

		//水平缩放
		clip_region_rel(RegionDilation, &RegionDilation, 0, 0, HErosionSize, HErosionSize);
		move_region(RegionDilation,&RegionDilation,MoveSize,0);
		m_vRgn[rId] = RegionDilation;
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::SideEdgeRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5560$\
					 167;B$\
					 991;LP;-600;600;1;0$\
					 994;LP;1;100;1;6$\
					 995;LP;1;500;1;6");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::TopEdgeRegionLocationAlgo(VARIANT* rgnId,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize)
{
	int rId           = (int)rgnId->fltVal;
	int MoveSize   = (int)pMoveSize->fltVal;
	int HErosionSize   = (int)pHErosionSize->fltVal;
	int VErosionSize   = (int)pVErosionSize->fltVal;
	//
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	m_vInterImgs[rId].Reset();
	Hobject Image=m_crtImgMult;
	m_vInterImgs[rId]=Image;

	if(m_topbaseLine.Id() == H_EMPTY_REGION)
	{
		m_vRgn[rId] = newRgn ;
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
		Hobject ABEdgeMoved,RegionDilation;
		dilation_rectangle1(m_topbaseLine,&RegionDilation, 1, VErosionSize);

		//水平缩放
		clip_region_rel(RegionDilation, &RegionDilation, 0, 0, HErosionSize, HErosionSize);
		move_region(RegionDilation,&RegionDilation,MoveSize,0);
		m_vRgn[rId] = RegionDilation;
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::TopEdgeRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5562$\
					 167;B$\
					 991;LP;-600;600;1;0$\
					 994;LP;1;100;1;6$\
					 995;LP;1;500;1;6");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CBottleCapAlgo::GenPrintingRegion (Halcon::Hobject Image, Halcon::Hobject *MovedPrintingRegion,Halcon::Hobject *Char, Halcon::Hobject *Slash, Halcon::HTuple RegionRow, Halcon::HTuple RegionHeight, Halcon::HTuple Cut, Halcon::HTuple CharWidth, 
	Halcon::HTuple SlashWidth, Halcon::HTuple SlashHeight, Halcon::HTuple DistanceCharSlash, Halcon::HTuple DistanceCharChar, Halcon::HTuple MoveDistance)
{

	// Local iconic variables 
	Hobject  PringtingRegion, c1, c2, c3;
	Hobject  c4, c, Compen;


	// Local control variables 
	HTuple  slash_col1, slash_col2, slash_row1;
	HTuple  slash_row2;

	//gen_rectangle1 (c1, RegionRow, Cut,RegionRow+RegionHeight, CharWidth, RegionRow + RegionHeight)
	gen_rectangle1(&c1, RegionRow, 0, RegionRow+RegionHeight, CharWidth);
	gen_rectangle1(&c2, RegionRow, CharWidth+DistanceCharChar, RegionRow+RegionHeight,(CharWidth*2)+DistanceCharChar);
	gen_rectangle1(&c3, RegionRow, (CharWidth*2)+(DistanceCharChar*2),RegionRow+RegionHeight, (CharWidth*3)+(DistanceCharChar*2));
	gen_rectangle1(&c4, RegionRow, (CharWidth*3)+(DistanceCharChar*3),RegionRow+RegionHeight, (CharWidth*4)+(DistanceCharChar*3));
	union2(c1, c2, &c);
	union2(c, c3, &c);
	union2(c, c4, &(*Char));
	slash_col1 = ((CharWidth*4)+(DistanceCharChar*3))+DistanceCharSlash;
	slash_col2 = slash_col1+SlashWidth;
	slash_row1 = RegionRow+((RegionHeight-SlashHeight)/2);
	slash_row2 = slash_row1+SlashHeight;
	gen_rectangle1(&(*Slash), slash_row1, slash_col1, slash_row2, slash_col2);
	union2((*Slash), (*Char), &PringtingRegion);

	move_region(PringtingRegion, &(*MovedPrintingRegion), 0, MoveDistance);
	if (0 != (MoveDistance<0))
	{
		move_region(PringtingRegion, &Compen, 0, (((MoveDistance+(CharWidth*4))+(DistanceCharChar*3))+(DistanceCharSlash*2))+SlashWidth);
	}
	else
	{
		move_region(PringtingRegion, &Compen, 0, (((MoveDistance-(CharWidth*4))-(DistanceCharChar*3))-(DistanceCharSlash*2))-SlashWidth);
	}
	union2((*MovedPrintingRegion), Compen, &(*MovedPrintingRegion));

	return;
}

int CBottleCapAlgo::PrintingLocation (Halcon::Hobject Image, Halcon::Hobject CharRegion, Halcon::Hobject SlashRegion, Halcon::Hobject *characters,Halcon::Hobject *Char, Halcon::Hobject *Slash, Halcon::HTuple Flag, Halcon::HTuple RegionRow, Halcon::HTuple Cut, 
	Halcon::HTuple Threshold, Halcon::HTuple CharHeight, Halcon::HTuple CharWidth, Halcon::HTuple DistanceCharChar, Halcon::HTuple DistanceCharSlash, Halcon::HTuple SlashWidth)
{

	// Local iconic variables 
	Hobject  WorkingRegion, pattern_image, ImageMedian;
	Hobject  image_scalemax, patterns, characters0,pattern;
	Hobject  CompenChar, CompenSlash;


	// Local control variables 
	HTuple  img_width, img_height, CenterCol;
	HTuple  _charHeight, _charWidth, Rows1, Cols1;
	HTuple  Rows2, Cols2, char_num, Distance, i;
	HTuple  a, b, col_char1, greater, indices;

	get_image_size(Image, &img_width, &img_height);
	gen_rectangle1(&WorkingRegion, RegionRow, Cut, RegionRow+CharHeight, (img_width-1)-Cut);
	region_features(WorkingRegion, "column", &CenterCol);

	reduce_domain(Image, WorkingRegion, &pattern_image);
	median_image(pattern_image,&ImageMedian,"circle",2.0,"mirrored");
	//
	Hobject Regions,RegionDilation,ImageResult;
	threshold (ImageMedian, &Regions, 100, 255);
	dilation_circle(Regions, &RegionDilation, 3.5);
	paint_region(RegionDilation, ImageMedian, &ImageResult, 10, "fill");
	//
	//mean_image(pattern_image, &imagemean, 3, 3);
	scale_image_max(ImageResult, &image_scalemax);
	Hobject ImageMax,ExpandedImage,ImageMean,ImageReduced;
	gray_erosion_shape(image_scalemax,&ImageMax,3,3,"octagon");
	//threshold(ImageMax, &pattern, Threshold, 255);
	expand_domain_gray(ImageMax, &ExpandedImage, 60);
	reduce_domain(ExpandedImage, WorkingRegion, &ImageReduced);
	mean_image(ImageReduced, &ImageMean, 36, 36);
	dyn_threshold(ImageReduced, ImageMean, &pattern, Threshold, "light");
	//
	fill_up(pattern,&pattern);
	//形态学处理
	//Hobject SE,patternopening;
	//gen_rectangle2(&SE,90,90,HTuple(-111).Rad(),3,2);
	//opening(pattern,SE,&patternopening);
	connection(pattern, &patterns);

	//_charHeight = 56;
	//_charWidth = 56;
	select_shape(patterns, &characters0, "height", "and", CharHeight-30, CharHeight+30);
	select_shape(characters0, &(*characters), "width", "and", CharWidth-30, CharWidth+30);
	//排序
	Hobject Sortedcharacters;
	sort_region(*characters, &Sortedcharacters, "first_point", "true", "column");
	smallest_rectangle1(Sortedcharacters, &Rows1, &Cols1, &Rows2, &Cols2);
	//write_tuple(Cols1,"c:/cols.tup");
	tuple_length(Cols1, &char_num);
	//汉字数量少于2为缺陷
	if ((char_num<2)||(char_num>4))
	{
		return -2;
	}

	tuple_sort(Cols1, &Cols1);
	Distance = (Cols1(1,char_num-1))-(Cols1(0,char_num-2));


	//判断汉字间距  //图片差异大
	{
		HTuple end_val30 = char_num-2;
		HTuple step_val30 = 1;
		for (i=0; i.Continue(end_val30, step_val30); i += step_val30)
		{
			//a = ((((Distance.Select(i))-(DistanceCharSlash*2))-SlashWidth)-CharWidth).Abs();
			//b = (((Distance.Select(i))-DistanceCharChar)-CharHeight).Abs();
			if (0 != (HTuple((((((Distance.Select(i))-(DistanceCharSlash*2))-SlashWidth)-CharWidth).Abs())>46).And(((((Distance.Select(i))-DistanceCharChar)-CharWidth).Abs())>46)))
			{
				return -3;
			}
		}
	}
	if (0 != (char_num==4))
	{
		//col_char1 := Cols1[0]
		//row_char2 := Rows2[char_num - 1]
		tuple_greater_elem(Distance, (CharWidth+DistanceCharChar)+20, &greater);
		tuple_find(greater, 1, &indices);
		if (0 != (indices.Select(0)!=-1))
		{
			col_char1 = Cols1.Select(indices.Select(0)+1);
			//*       elseif ((Cols1[0] - Cut < CharWidth * 1.2) and (Cols1[0]<=CenterCol))
			//col_char1 := Cols1[0] - DistanceCharChar*2.5 - CharWidth*2
			//Flag := 0
			//*       elseif ((Cols1[0] - Cut < CharWidth * 1.2) and (Cols1[0]>CenterCol))
			//col_char1 := Cols1[0] - DistanceCharChar - CharWidth
		}
		else
		{
			col_char1 = Cols1.Select(0);
		}
	}
	else if (0 != (char_num==3))
	{
		tuple_greater_elem(Distance, (CharWidth+DistanceCharChar)+10, &greater);
		tuple_find(greater, 1, &indices);

		if (0 != (indices.Select(0)!=-1))
		{
			col_char1 = Cols1.Select(indices.Select(0)+1);
		}
		/*else if (0 != (HTuple(((Cols1.Select(0))-Cut)<(CharWidth*1.2)).And((Cols1.Select(0))<=CenterCol)))
		{
		col_char1 = ((Cols1.Select(0))-(DistanceCharChar*1))-(CharWidth*1);
		Flag = 0;
		}*/
		else if ((Cols1.Select(0)-Cut)<((CharWidth[0].I())*1.2))
		{
			col_char1 = ((Cols1.Select(0))-DistanceCharChar)-CharWidth;

		}
		else
		{
			col_char1 = Cols1.Select(0);
		}
	}
	else if (0 != (char_num==2))
	{
		tuple_greater_elem(Distance, (CharWidth+DistanceCharChar)+10, &greater);
		tuple_find(greater, 1, &indices);

		if (0 != (indices.Select(0)!=-1))
		{
			col_char1 = Cols1.Select(indices.Select(0)+1);
		}
		else if (0 != (HTuple(((Cols1.Select(0))-Cut)<(CharWidth*1.2)).And((Cols1.Select(0))<=CenterCol)))
		{
			col_char1 = ((Cols1.Select(0))-(DistanceCharChar*2.5))-(CharWidth*2);
			Flag = 0;
		}
		else if (0 != (HTuple(((Cols1.Select(0))-Cut)<(CharWidth*1.2)).And((Cols1.Select(0))>CenterCol)))
		{
			col_char1 = ((Cols1.Select(0))-DistanceCharChar)-CharWidth;

		}
		else
		{
			col_char1 = Cols1.Select(0);
		}

	}
	if (0 != (Flag==1))
	{
		move_region(CharRegion, &(*Char), 0, col_char1);
		move_region(CharRegion, &CompenChar, 0, (((col_char1-(CharWidth*4))-(DistanceCharChar*3))-(DistanceCharSlash*2))-SlashWidth);
		union2((*Char), CompenChar, &(*Char));
	}
	else
	{
		move_region(CharRegion, &(*Char), 0, col_char1);
		move_region(CharRegion, &CompenChar, 0, (((col_char1+(CharWidth*4))+(DistanceCharChar*3))+(DistanceCharSlash*2))+SlashWidth);
		union2((*Char), CompenChar, &(*Char));
	}

	move_region(SlashRegion, &(*Slash), 0, col_char1);
	move_region(SlashRegion, &CompenSlash, 0, (((col_char1-(CharWidth*4))-(DistanceCharChar*3))-(DistanceCharSlash*2))-SlashWidth);
	union2((*Slash), CompenSlash, &(*Slash));

	return 0;
}

STDMETHODIMP CBottleCapAlgo::UpCharLocationAlgo(VARIANT* rgnId, VARIANT* pMode, VARIANT* pRegionRow/*,VARIANT* pRegionHeight*/, VARIANT* pThreshold,VARIANT* pCharWidth,VARIANT* pCharHeight,VARIANT* pSlashWidth,VARIANT* pSlashHeight,VARIANT* pDistanceCharChar,VARIANT* pDistanceCharSlash,VARIANT* pMoveDistance,VARIANT* pCharFlag)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	//m_vInterImgs[id] = Hobject();
	Hobject rgn  = m_vRgn[id];
	int Mode = (int)pMode->fltVal;
	int RegionRow = (int)pRegionRow->fltVal;
	//int RegionHeight = (int)pRegionHeight->fltVal;
	int Threshold = (int)pThreshold->fltVal;

	int CharWidth = (int)pCharWidth->fltVal;
	int CharHeight = (int)pCharHeight->fltVal;
	int SlashWidth = (int)pSlashWidth->fltVal;
	int SlashHeight = (int)pSlashHeight->fltVal;

	int DistanceCharChar = (int)pDistanceCharChar->fltVal;
	int DistanceCharSlash = (int)pDistanceCharSlash->fltVal;

	int MoveDistance = (int)pMoveDistance->fltVal;

	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}		
		Hobject Image=m_crtImgMult;
		HTuple ChannelNum;
		
		//标定文件路径
		string MapImgFile = m_strTaskPathWithoutCameraIndex;
		string CanRegionFile = m_strTaskPathWithoutCameraIndex;
		string TopRegionFile = m_strTaskPathWithoutCameraIndex;
		//
		string CharRegionFile = m_strTaskPathWithoutCameraIndex;
		string SlashRegionFile = m_strTaskPathWithoutCameraIndex;

		if(m_bDebugImg)
			m_dictSpecialData[id].clear();
		//Generate the task directory if not exist
		if(_access(MapImgFile.c_str(),0))
		{
			CreateDirectoryA(MapImgFile.c_str(), NULL);
		}
		MapImgFile.append("\\");
		MapImgFile.append("MapImage");
		MapImgFile.append(".tiff");

		CanRegionFile.append("\\");
		CanRegionFile.append("CanRegion");
		CanRegionFile.append(".reg");

		TopRegionFile.append("\\");
		TopRegionFile.append("TopRegion");
		TopRegionFile.append(".reg");

		CharRegionFile.append("\\");
		CharRegionFile.append("CharRegion");
		CharRegionFile.append(".reg");

		SlashRegionFile.append("\\");
		SlashRegionFile.append("SlashRegion");
		SlashRegionFile.append(".reg");

		if(m_isFirst)
		{         
			if(_access(MapImgFile.c_str(),0)||_access(CanRegionFile.c_str(),0)||_access(TopRegionFile.c_str(),0))
			{
				retValue = -1;
				retValue.Detach(pCharFlag);
				m_vErrorRgn[id].Reset();
				return S_FALSE;
			}
			read_image(&m_Map, MapImgFile.c_str());
			read_region(&m_CanRegion,CanRegionFile.c_str());
			read_region(&m_CanTopRegion,TopRegionFile.c_str());
			m_isFirst=false;
		}

		HTuple ColCenter,RowCenter;
		region_features(m_CanRegion, "column", &ColCenter);
		region_features(m_CanTopRegion, "row2", &RowCenter);
		//
		HTuple RowPos,ColumnPos;
		region_features(m_CenterTopRegion, "row2", &RowPos);
		region_features(m_CenterTopRegion, "column", &ColumnPos);
		
		HTuple RowDiff = RowPos-RowCenter;
		HTuple ColDiff = ColumnPos-ColCenter;
		if (ColDiff.Abs()>500)
		{
			retValue = -1;
			retValue.Detach(pCharFlag);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		HTuple HomMat2D;
		Hobject ImageAffinTrans,ImageMapped;
		vector_angle_to_rigid(0, 0, 0, -RowDiff, -ColDiff, 0, &HomMat2D);
		affine_trans_image(Image, &ImageAffinTrans, HomMat2D, "constant", "false");
		map_image(ImageAffinTrans, m_Map, &ImageMapped);
		Hobject ImageMappedRotate;
		rotate_image(ImageMapped, &ImageMappedRotate, -90, "constant");
		get_image_size(ImageMappedRotate, &m_mapimg_width, &m_mapimg_height);
		//m_ImageMapped彩色用于检测
		m_ImageMapped=ImageMappedRotate;
		count_channels(ImageMappedRotate,&ChannelNum);
		if (ChannelNum[0].I()==3){
			access_channel(ImageMappedRotate,&ImageMappedRotate,2);
		}
		m_GreenMapped=ImageMappedRotate;
		Hobject ImageDomain;
		get_domain(ImageMappedRotate,&ImageDomain);
		//access_channel(ImageAffine, &Green, 2);
		m_vInterImgs[id]=ImageMappedRotate;
		Hobject MovedPrintingRegion;
		//
		/*HTuple RegionRow = 60;
		HTuple RegionHeight = 73;
		HTuple Cut = 30;
		HTuple Threshold = 30;
		HTuple CharHeight = 60;
		HTuple CharWidth = 70;
		HTuple SlashWidth = 130;
		HTuple SlashHeight = 50;
		HTuple DistanceCharSlash = 65;
		HTuple DistanceCharChar = 7;
		HTuple MoveDistance = -100;*/
		HTuple Cut = 70;
		HTuple Result;
		//model=0训练模式，model=1检测模式
		if (Mode==0)
		{

			//sum_ = (((CharHeight*4)+(DistanceCharSlash*2))+(DistanceCharChar*3))+SlashHeight;
			//根据汉字高度和间隔调整印刷区域尺寸,调节平移距离，将MovedPrintingRegion与模板图片套准
			Hobject CharRegion, SlashRegion;
			GenPrintingRegion(ImageMappedRotate, &MovedPrintingRegion, &CharRegion, &SlashRegion, RegionRow, CharHeight, Cut, CharWidth, SlashWidth, SlashHeight, DistanceCharSlash, DistanceCharChar, MoveDistance);

			concat_obj(m_vRgnExTest[id],MovedPrintingRegion,&m_vRgnExTest[id]);
			//保存本地
			write_region(CharRegion,CharRegionFile.c_str());
			write_region(SlashRegion,SlashRegionFile.c_str());
			Result=0;
		}
		else if(Mode==1)
		{
			if(_access(CharRegionFile.c_str(),0)||_access(SlashRegionFile.c_str(),0))
			{
				retValue = -1;
				retValue.Detach(pCharFlag);
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
				return S_FALSE;
			}
			//读取
			Hobject CharRegionR,SlashRegionR;
			read_region(&CharRegionR, CharRegionFile.c_str());
			read_region(&SlashRegionR, SlashRegionFile.c_str());

			HTuple Flag = 1;
			Hobject Char, Slash,characters;
			Result=PrintingLocation(ImageMappedRotate, CharRegionR, SlashRegionR, &characters,&Char, &Slash, Flag, RegionRow,Cut, Threshold, CharHeight, CharWidth, DistanceCharChar, DistanceCharSlash, SlashWidth);

			if (Result==0)
			{
				intersection(ImageDomain,Char,&m_Char);
				intersection(ImageDomain,Slash,&m_Slash);
				if (m_bDebugImg)
				{
					concat_obj(m_vRgnExTest[id],characters,&m_vRgnExTest[id]);
				}
			}
			else
			{
				m_vErrorRgn[id]=characters;
				gen_empty_region(&m_Char);
				gen_empty_region(&m_Slash);
			}

			if (m_bDebugImg)
			{
				//concat_obj(m_vRgnExTest[id],characters,&m_vRgnExTest[id]);
				concat_obj(m_vRgnExTest[id],m_Char,&m_vRgnExTest[id]);
				concat_obj(m_vRgnExTest[id],m_Slash,&m_vRgnExTest[id]);
			}

		}
		retValue=Result[0].I();
		retValue.Detach(pCharFlag);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		retValue.Detach(pCharFlag);
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		retValue.Detach(pCharFlag);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::UpCharLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5573$\
					 397;R;FP;-10;10;1;>=#0$\
					 381;B$\
					 1340;LP;0;1;1;1$\
					 5563;LP;1;200;1;60$\
					 5572;LP;1;160;1;60$\
					 5565;LP;1;100;1;70$\
					 5566;LP;1;100;1;60$\
					 5567;LP;1;200;1;130$\
					 5568;LP;1;200;1;50$\
					 5569;LP;1;60;1;7$\
					 5570;LP;1;100;1;65$\
					 5571;LP;-200;200;1;-100"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::UpCharRegionLocationAlgo(VARIANT* rgnId,VARIANT* pSelectmodel,VARIANT* pRegionRow, VARIANT* pRegionHeight, VARIANT*pRegionWidth,VARIANT* pCharSize,VARIANT* pSlashSize)
{
	int rId           = (int)rgnId->fltVal;
	int Selectmodel   = (int)pSelectmodel->fltVal;

	int RegionRow   = (int)pRegionRow->fltVal;
	int RegionHeight   = (int)pRegionHeight->fltVal;
	int RegionWidth   = (int)pRegionWidth->fltVal;
	int CharSize   = (int)pCharSize->fltVal;
	int SlashSize   = (int)pSlashSize->fltVal;


	//
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	m_vInterImgs[rId].Reset();

	HRegion char_clone=m_Char;
	HRegion slash_clone=m_Slash;
	if(char_clone.Area()==0||slash_clone.Area()==0)
	{
		m_vRgn[rId] = newRgn ;
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
		Hobject Image=m_ImageMapped;
		m_vInterImgs[rId]=m_ImageMapped; //用于区域和图片实时匹配显示
		Hobject ImageDomain;
		get_domain(Image,&ImageDomain);
		//HTuple Cut=30;
		Hobject DetectRegion,CharRegDilation,SlashRegDilation,CharSlashUnion,FlatReg,RegionClosing;
		closing_rectangle1(m_Char, &RegionClosing, 16, 1);
		dilation_circle(RegionClosing,&CharRegDilation,CharSize);
		dilation_circle(m_Slash,&SlashRegDilation,SlashSize);
		union2(SlashRegDilation,CharRegDilation,&CharSlashUnion);
		intersection(ImageDomain,CharSlashUnion,&CharSlashUnion);
		if (Selectmodel==0)
		{
			//字符区域
			m_vRgn[rId] = CharSlashUnion;
		} 
		else if(Selectmodel==1)
		{
			//斜线区域
			m_vRgn[rId] = SlashRegDilation;
		}
		else if(Selectmodel==2)
		{
			gen_rectangle1(&DetectRegion, RegionRow, RegionWidth, RegionRow+RegionHeight, (m_mapimg_width-1)-RegionWidth);
			difference(DetectRegion,CharSlashUnion,&FlatReg);
			intersection(ImageDomain,FlatReg,&FlatReg);
			m_vRgn[rId] = FlatReg;
		}

#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::UpCharRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5560$\
					 167;B$\
					 5574;LP;0;2;1;0$\
					 5575;LP;1;300;1;60$\
					 5576;LP;1;130;1;60$\
					 5582;LP;30;300;1;60$\
					 5577;LP;1;30;1;3$\
					 5578;LP;1;30;1;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::LowCharLocationAlgo(VARIANT* rgnId, VARIANT* pMoveSize,  VARIANT* pCharSize,VARIANT* pSlashSize, VARIANT* pThreshold,VARIANT* pMaxArea,VARIANT* pCharFlag)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	//m_vInterImgs[id] = Hobject();
	Hobject rgn  = m_vRgn[id];
	int MoveSize = (int)pMoveSize->fltVal;

	int CharSize   = (int)pCharSize->fltVal;
	int SlashSize   = (int)pSlashSize->fltVal;
	int Threshold = (int)pThreshold->fltVal;
	int MaxArea = (int)pMaxArea->fltVal;

	//gen_empty_region(&m_LowCharRegion);
	m_LowCharRegion=m_EmptyRegion;
	CComVariant retValue;
	HRegion char_clone=m_Char;
	HRegion slash_clone=m_Slash;
	if(char_clone.Area()==0||slash_clone.Area()==0)
	{
		retValue = -1;
		retValue.Detach(pCharFlag);
		m_vErrorRgn[id].Reset();
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
		
		Hobject Image=m_GreenMapped;
		m_vInterImgs[id]=m_GreenMapped; //用于区域和图片实时匹配显示
		Hobject ImageDomain,RegionClosing,CharRegDilation,SlashRegDilation;
		get_domain(Image,&ImageDomain);

		closing_rectangle1(m_Char, &RegionClosing, 16, 1);
		dilation_circle(RegionClosing,&CharRegDilation,CharSize);
		Hobject SlashH;
		dilation_circle(m_Slash,&SlashH,SlashSize);
		dilation_rectangle1(SlashH,&SlashRegDilation,160,1); //错位
		//汉字下方的搜索区域
		Hobject CharRegionSearch;
		move_region(CharRegDilation,&CharRegionSearch,MoveSize,0);
		//斜杠下方的搜索区域
		Hobject SlashRegionSearch;
		move_region(SlashRegDilation,&SlashRegionSearch,MoveSize,0);
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[id],CharRegionSearch,&m_vRgn[id]);
			concat_obj(m_vRgn[id],SlashRegionSearch,&m_vRgn[id]);
		}
		//*************************
		//判别汉字搜索区域内有无字符
		HTuple row1,row2,RowDiff,SizeInt;
		region_features(CharRegionSearch,"row1",&row1);
		region_features(CharRegionSearch,"row2",&row2);
		RowDiff=(row2-row1).Abs();
		tuple_int(RowDiff,&SizeInt);
		Hobject CharRegionSearchErosion,CharRegionSearchErosions,CharRegionSearchs,CharImageReduced,SearchRegionUnion,ImageReducedScale;
		union2(CharRegionSearch,SlashRegionSearch,&SearchRegionUnion);
		HTuple Row1,Col1,Row2,Col2;
		Hobject SearchRect;
		smallest_rectangle1(SearchRegionUnion,&Row1,&Col1,&Row2,&Col2);
		if (Row1>86)
		{
			gen_rectangle1(&SearchRect,Row1-80,Col1,Row2,Col2);
		}
		else
		{
			gen_rectangle1(&SearchRect,1,Col1,Row2,Col2);
		}	
		reduce_domain(Image,SearchRect,&CharImageReduced);
		scale_image_max(CharImageReduced,&ImageReducedScale);
		mean_image(ImageReducedScale,&ImageReducedScale,3,3);
		erosion_rectangle1(CharRegionSearch,&CharRegionSearchErosion,1,SizeInt);
		//connection(CharRegionSearchErosion,&CharRegionSearchErosions);
		//write_image(ImageReducedScale, "tiff", 0, "C:\\22.tiff");

		connection(CharRegionSearch,&CharRegionSearchs);
		Hobject RegionTest;
		HTuple RegNum;
		HTuple CharImageMean,Deviation1;
		Hobject ImageExpand,ImageMean;
		count_obj(CharRegionSearchErosions,&RegNum);
		if (RegNum>1)
		{
			for (int i=1;i<=RegNum;i++)
			{
				Hobject CharRegionSelect,ImageCharReduced,RegionTestClosing,RegionTestClosings,RegionTestSelect;
				select_obj(CharRegionSearchs,&CharRegionSelect,i);
				//select_obj(SlashRegionSearchErosions,&RegionSelect1,i);
				//intensity(RegionSelect1,SlashImageReducedScale,&SlashImageMean,&Deviation2);
				reduce_domain(ImageReducedScale,CharRegionSelect,&ImageCharReduced);
				expand_domain_gray(ImageCharReduced,&ImageExpand,60);
				reduce_domain(ImageExpand,CharRegionSelect,&ImageCharReduced);
				//threshold(ImageCharReduced,&RegionTest,Threshold,255);
				mean_image(ImageCharReduced,&ImageMean,60,60);
				dyn_threshold(ImageCharReduced,ImageMean,&RegionTest,Threshold,"light");
				//
				opening_circle(RegionTest,&RegionTest,2);
				closing_circle(RegionTest,&RegionTestClosing,8);
				connection(RegionTestClosing,&RegionTestClosings);
				select_shape_std(RegionTestClosings,&RegionTestSelect,"max_area",70);
				HTuple RegArea,RegRow,RegCol;
				area_center(RegionTestSelect,&RegArea,&RegRow,&RegCol);

				if (RegArea>MaxArea)
				{
					Hobject Rectangle;
					HTuple row1,col1,row2,col2;
					smallest_rectangle1(RegionTestSelect,&row1,&col1,&row2,&col2);
					gen_rectangle1(&Rectangle,row1,col1,row2,col2);
					union2(m_LowCharRegion,Rectangle,&m_LowCharRegion);
				}
			}
		} 
		else
		{
			Hobject ImageCharReduced,RegionTestClosing,RegionTestSelect,RegionTestClosings;
			reduce_domain(ImageReducedScale,CharRegionSearch,&ImageCharReduced);
			//threshold(ImageCharReduced,&RegionTest,Threshold,255);
			expand_domain_gray(ImageCharReduced,&ImageExpand,60);
			reduce_domain(ImageExpand,CharRegionSearch,&ImageCharReduced);
			mean_image(ImageCharReduced,&ImageMean,60,60);
			dyn_threshold(ImageCharReduced,ImageMean,&RegionTest,Threshold,"light");
			//
			opening_circle(RegionTest,&RegionTest,2);
			closing_circle(RegionTest,&RegionTestClosing,8);
			connection(RegionTestClosing,&RegionTestClosings);
			select_shape_std(RegionTestClosings,&RegionTestSelect,"max_area",70);
			HTuple RegArea,RegRow,RegCol;
			area_center(RegionTestSelect,&RegArea,&RegRow,&RegCol);

			if (RegArea>MaxArea)
			{
				Hobject Rectangle;
				HTuple row1,col1,row2,col2;
				smallest_rectangle1(RegionTestSelect,&row1,&col1,&row2,&col2);
				gen_rectangle1(&Rectangle,row1,col1,row2,col2);
				union2(m_LowCharRegion,Rectangle,&m_LowCharRegion);
			}
		}
		//********************************
		//判别斜杠搜索区域内有无字符
		HTuple row3,row4,RowDiff1,SizeInt1;
		region_features(SlashRegionSearch,"row1",&row3);
		region_features(SlashRegionSearch,"row2",&row4);
		RowDiff1=(row3-row4).Abs();
		tuple_int(RowDiff1,&SizeInt1);
		Hobject SlashRegionSearchErosion,SlashRegionSearchErosions,SlashRegionSearchs;
		//reduce_domain(Image,SlashRegionSearch,&SlashImageReduced);
		//scale_image_max(SlashImageReduced,&SlashImageReducedScale);

		erosion_rectangle1(SlashRegionSearch,&SlashRegionSearchErosion,1,SizeInt1);
		//connection(SlashRegionSearchErosion,&SlashRegionSearchErosions);
		connection(SlashRegionSearch,&SlashRegionSearchs);
		HTuple RegNum1;
		HTuple SlashImageMean,Deviation2;
		count_obj(SlashRegionSearchErosions,&RegNum1);
		if (RegNum1>1)
		{
			for (int i=1;i<=RegNum1;i++)
			{
				Hobject SlashRegionSelect,ImageSlashReduced,RegionTestClosing,RegionTestClosings,RegionTestSelect;
				select_obj(SlashRegionSearchs,&SlashRegionSelect,i);
				//select_obj(SlashRegionSearchErosions,&RegionSelect1,i);
				//intensity(RegionSelect1,SlashImageReducedScale,&SlashImageMean,&Deviation2);
				reduce_domain(ImageReducedScale,SlashRegionSelect,&ImageSlashReduced);
				//threshold(ImageSlashReduced,&RegionTest,Threshold,255);
				expand_domain_gray(ImageSlashReduced,&ImageExpand,60);
				reduce_domain(ImageExpand,SlashRegionSelect,&ImageSlashReduced);
				mean_image(ImageSlashReduced,&ImageMean,60,60);
				dyn_threshold(ImageSlashReduced,ImageMean,&RegionTest,Threshold,"light");
				//
				opening_circle(RegionTest,&RegionTest,2);
				closing_circle(RegionTest,&RegionTestClosing,8);
				connection(RegionTestClosing,&RegionTestClosings);
				select_shape_std(RegionTestClosings,&RegionTestSelect,"max_area",70);
				HTuple RegArea,RegRow,RegCol;
				area_center(RegionTestSelect,&RegArea,&RegRow,&RegCol);

				if (RegArea>MaxArea)
				{
					Hobject Rectangle;
					HTuple row1,col1,row2,col2;
					smallest_rectangle1(RegionTestSelect,&row1,&col1,&row2,&col2);
					gen_rectangle1(&Rectangle,row1,col1,row2,col2);
					union2(m_LowCharRegion,Rectangle,&m_LowCharRegion);
				}
			}
		} 
		else
		{
			Hobject ImageCharReduced,RegionTestClosing,RegionTestSelect,ImageSlashReduced,RegionTestClosings;
			reduce_domain(ImageReducedScale,SlashRegionSearch,&ImageSlashReduced);
			//threshold(ImageSlashReduced,&RegionTest,Threshold,255);
			expand_domain_gray(ImageSlashReduced,&ImageExpand,60);
			reduce_domain(ImageExpand,SlashRegionSearch,&ImageSlashReduced);
			mean_image(ImageSlashReduced,&ImageMean,60,60);
			dyn_threshold(ImageSlashReduced,ImageMean,&RegionTest,Threshold,"light");
			//
			opening_circle(RegionTest,&RegionTest,2);
			closing_circle(RegionTest,&RegionTestClosing,8);
			connection(RegionTestClosing,&RegionTestClosings);
			select_shape_std(RegionTestClosings,&RegionTestSelect,"max_area",70);
			HTuple RegArea,RegRow,RegCol;
			area_center(RegionTestSelect,&RegArea,&RegRow,&RegCol);

			if (RegArea>MaxArea)
			{
				Hobject Rectangle;
				HTuple row1,col1,row2,col2;
				smallest_rectangle1(RegionTestSelect,&row1,&col1,&row2,&col2);
				gen_rectangle1(&Rectangle,row1,col1,row2,col2);
				union2(m_LowCharRegion,Rectangle,&m_LowCharRegion);
			}

		}

		union1(m_LowCharRegion,&m_LowCharRegion);
		HTuple area,row,col;
		area_center(m_LowCharRegion,&area,&row,&col);

		if (area>0)
		{
			intersection(ImageDomain,m_LowCharRegion,&m_LowCharRegion);
			if (m_bDebugImg)
			{
				concat_obj(m_vRgnExTest[id],RegionTest,&m_vRgnExTest[id]);
				concat_obj(m_vRgnExTest[id],m_LowCharRegion,&m_vRgnExTest[id]);
			}
		}
		else
		{
			m_vErrorRgn[id]=RegionTest;
			m_LowCharRegion=m_EmptyRegion;
			//gen_empty_region(&m_LowCharRegion);
		}
		
		retValue=0;  //检测算子不报错
		retValue.Detach(pCharFlag);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		retValue.Detach(pCharFlag);
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		retValue.Detach(pCharFlag);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::LowCharLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5573$\
					 397;R;FP;-10;10;1;>=#0$\
					 381;B$\
					 5575;LP;0;300;1;100$\
					 5577;LP;1;30;1;3$\
					 5578;LP;1;30;1;3$\
					 5572;LP;1;160;1;60$\
					 5580;LP;1;10000;1;2000"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::LowCharRegionLocationAlgo(VARIANT* rgnId,VARIANT* pSelectmodel,VARIANT* pRegionRow, VARIANT* pRegionHeight, VARIANT*pRegionWidth,VARIANT* pCharSize)
{
	int rId           = (int)rgnId->fltVal;
	int Selectmodel   = (int)pSelectmodel->fltVal;

	int RegionRow   = (int)pRegionRow->fltVal;
	int RegionHeight   = (int)pRegionHeight->fltVal;
	int RegionWidth   = (int)pRegionWidth->fltVal;
	int CharSize   = (int)pCharSize->fltVal;

	//
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	m_vInterImgs[rId].Reset();
	/*HRegion char_clone=m_LowCharRegion;
	if(char_clone.Area()==0)
	{
	m_vRgn[rId] = newRgn ;
	return S_FALSE;
	}*/

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
		Hobject Image=m_ImageMapped;
		m_vInterImgs[rId]=m_ImageMapped; //用于区域和图片实时匹配显示
		Hobject ImageDomain;
		get_domain(Image,&ImageDomain);
		//HTuple Cut=30;
		Hobject DetectRegion,CharRegDilation,SlashRegDilation,CharSlashUnion,FlatReg;

		dilation_circle(m_LowCharRegion,&SlashRegDilation,CharSize);
		intersection(ImageDomain,SlashRegDilation,&CharSlashUnion);
		if (Selectmodel==0)
		{
			//字符区域
			m_vRgn[rId] = CharSlashUnion;
		} 
		else if(Selectmodel==1)
		{
			gen_rectangle1(&DetectRegion, RegionRow, RegionWidth, RegionRow+RegionHeight, (m_mapimg_width-1)-RegionWidth);
			difference(DetectRegion,CharSlashUnion,&FlatReg);
			intersection(ImageDomain,FlatReg,&FlatReg);
			m_vRgn[rId] = FlatReg;
		}

#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::LowCharRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5560$\
					 167;B$\
					 5574;LP;0;1;1;0$\
					 5575;LP;1;300;1;60$\
					 5576;LP;1;130;1;60$\
					 5582;LP;30;300;1;60$\
					 5577;LP;1;30;1;3");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::CurlNoCompoundAlgoNewHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5581$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 420;LP;1;250;1;60");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CurlNoCompoundAlgoNew(VARIANT* rgnId, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vInterImgs[id].Reset();
	m_vRgnEx[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	Hobject Image = m_ImageMapped;
	m_vInterImgs[id]=m_ImageMapped;

	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = 1;
		m_vErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	//int WhiteMaskSize = (int) pWhiteMaskSize->fltVal;
	float SeriousWhitePointDynThresh = (float) pSeriousWhitePointDynThresh->fltVal;
	//int SeriousWhitePointSize = (int) pSeriousWhitePointSize->fltVal;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//
		
		Hobject ImageReduced, ImageReduceMean;
		Hobject RegionTest,ImageReducedScale;
		//
		reduce_domain(Image,rgn,&ImageReduced);
		scale_image_max(ImageReduced,&ImageReducedScale);
		mean_image(ImageReducedScale,&ImageReducedScale,3,3);

		mean_image(ImageReducedScale,&ImageReduceMean,30,30);
		dyn_threshold(ImageReducedScale,ImageReduceMean,&RegionTest,SeriousWhitePointDynThresh,"light");
		//
		opening_circle(RegionTest,&RegionTest,2);

		HTuple Area, Row_test, Column_test;
		//ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
		//ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
		area_center(RegionTest,&Area, &Row_test, &Column_test);
		//
		if(Area > 0)
		{
			m_nDetectParam=0;
		}
		Hobject newRgn; 
		m_vErrorRgn[id] = newRgn;
		retValue = 1.0;
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoNew(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_ImageMapped,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[id]=Image;
		Hobject ImageReduced, ImageReduceMean;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		//预处理
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(Image, &Image, MaskSize, MaskSize);
		//
		if(SeriousBlackPointDynThresh<254)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
			ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
			area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		}
		if(SeriousWhitePointDynThresh<254)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
			ImageProcessAlg.ErrorDetect(ImageReduced, ImageReduceMean, SeriousWhitePointDynThresh, SeriousWhitePointSize, WhiteUnion,0);
			area_center(WhiteUnion, &whiteArea, &rowWhite, &colWhite);
		}
		//

		if (m_bDebugImg &&(SeriousBlackPointDynThresh<254||SeriousWhitePointDynThresh<254))
		{
			m_dictSpecialData[id].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[id].push_back(float(SeriousWhitePointDynThresh));
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
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;

		if(whiteArea>0 ||blackArea>0 )
		{
			union2(BlackUnion, WhiteUnion, &DefectRegs);
			area_center(DefectRegs, &Area, &Row_test, &Column_test);
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vErrorRgn[id] = DefectRegs;
		}
		else
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}

		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::DynThresholdAlgoNewHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("660$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 412;LP;3;101;2;7$\
					 415;LP;1;255;1;60$\
					 416;LP;1;1000;1;10$\
					 417;LP;3;101;2;7$\
					 420;LP;1;255;1;60$\
					 421;LP;1;1000;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::LineDetectionAlgo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize,VARIANT* pDefectArea)
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
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = 0.0;
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
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::LineDetectionAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBottleCapAlgo::LineDetectionColorAlgo(VARIANT* rgnId,VARIANT* pColorTransType, VARIANT* pChannelSelect,  VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize,VARIANT* pDefectArea)
{
	int rId           = (int)rgnId->fltVal;
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
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
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = 0.0;
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
		Hobject Image;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pDefectArea);
			return S_FALSE;
		}
		m_vInterImgs[rId]=Image;
		//
		Hobject crtImg = Image;
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
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::LineDetectionColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("425$\
					 397;R;FP;0;1000;4$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
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

STDMETHODIMP CBottleCapAlgo::BubbleDetectProfileAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vInterImgs[rId].Reset();
	Hobject rgn = m_vRgn[rId];

	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
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
		Hobject Image= m_crtImgMult;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(Image,&Image,rgn,ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[rId]=Image;
		Hobject ImageReduced, ImageReduceMean,ExpandedImage,ImageDetectNew;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		//预处理
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(Image, &Image, MaskSize, MaskSize);
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
STDMETHODIMP CBottleCapAlgo::BubbleDetectProfileAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("646$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
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
STDMETHODIMP CBottleCapAlgo::BubbleDetectProfileAlgoNew(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vInterImgs[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
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
		if(!H_ColorImageTrans(m_ImageMapped,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[rId]=Image;
		Hobject ImageReduced, ImageReduceMean,ExpandedImage,ImageDetectNew;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		//预处理
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(Image, &Image, MaskSize, MaskSize);
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
			if (m_bDebugImg)
			{
				dilation_circle (DefectRegs, &DefectRegs, 2.5);
				m_vErrorRgn[rId] = DefectRegs;
			}
			else
			{
				HTuple DefectArea,DefectRow,DefectCol;
				area_center(DefectRegs,&DefectArea,&DefectRow,&DefectCol);
				Hobject DefectCircle;
				gen_circle(&DefectCircle,DefectRow,DefectCol,10);
				m_vErrorRgn[rId] = DefectCircle;
			}
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::BubbleDetectProfileAlgoNewHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("646$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
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

STDMETHODIMP CBottleCapAlgo::CurlNoCompoundColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT*pGrayValueMean )
{
	int id = (int)rgnId->fltVal;
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	if (m_bPilotImg)
	{

		return S_OK;
	}

	Hobject rgn = m_vRgn[id];
	HTuple area1,row1,col1;
	area_center(rgn,&area1,&row1,&col1);
	CComVariant retValue;

	if(area1<= 0) 
	{
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		HTuple Width, Height;
		get_image_size (m_crtImgMult, &Width, &Height);
		Hobject Image;// = m_crtImg;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		//if(!H_ColorImageTransAll(m_crtImgMult,&Image,  ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pGrayValueMean);
			return S_FALSE;
		}
		m_vInterImgs[id]=Image;
		intensity (rgn, Image, &MeanGray, &Deviation);

		if(m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(MeanGray[0].D()));
			m_dictSpecialData[id].push_back(-999999.0f);

			HTuple AbsoluteHisto, RelativeHisto, Length, maxValue = 0;
			gray_histo (rgn, Image, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(AbsoluteHisto, &maxValue);

			float YRange = 100;
			float YScaleFactor = float(YRange/maxValue[0].D());

			int histoLength = Length[0].I();
			for (int i = 0; i < histoLength; i ++)
			{
				m_dictSpecialData[id].push_back(float((-1)*YScaleFactor*AbsoluteHisto[i].D()));
			}
		}
		retValue = MeanGray[0].I();
		dilation_circle (rgn, &RegionDilat, 1.5);
		m_vErrorRgn[id] = RegionDilat;
		retValue.Detach(pGrayValueMean);
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CurlNoCompoundColorAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1;<=#20$\
					 167;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBottleCapAlgo::BlackDetectExcludeEdgesAlgo(VARIANT* rgnId,  VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize, VARIANT* pThresholdLow,  VARIANT* pMinEdgeSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
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
		Hobject Image;
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[rId]=Image;
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
STDMETHODIMP CBottleCapAlgo::BlackDetectExcludeEdgesAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("635$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
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

void CBottleCapAlgo::my_disp_obj(const Halcon::Hobject Object)
{
	if (m_bPilotImg)
	{
		return;
	}
	std::string mode("visible");
	std::string strMachine("");
	set_check("~father");
	HTuple Width, Height;

	if(m_WindowFirst)
	{
		//get_image_size (m_crtImg, &Width, &Height);
		open_window(1,1,m_imgWidth/2,m_imgHeight/2,(Hlong)NULL, mode.c_str(),strMachine.c_str(), (Hlong*)&m_CHImgWndID);
		m_WindowFirst=false;
		disp_obj(m_crtImg,m_CHImgWndID);
	}
	set_part(m_CHImgWndID,0, 0, m_imgWidth, m_imgHeight);
	set_check("father");
	set_draw(m_CHImgWndID,"margin"); 
	set_color(m_CHImgWndID, "red");
	set_line_width(m_CHImgWndID,HTuple(1));
	disp_obj(Object,m_CHImgWndID);

	write_object(Object, "D:/Region3.hobj");
	write_image(m_crtImg, "tiff", 0,"D:/Image.tiff");
}
/*******************************************************************************************************************************
**********************************塑料瓶盖定位算法（以PS_标识，即Plastic）*******************************************************
********************************************************************************************************************************/

/***************************************************************************
** 函数名：PS_SideCenterTopLocationAlgo
** 功  能：针对一种塑料瓶盖侧面拍摄的口部进行定位
** 时  间：2019年11月13日
****************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_SideCenterTopLocationAlgo(VARIANT* rgnId, VARIANT* pChannelSelect,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight, VARIANT* pThresholdValue,VARIANT* pAreaSize,VARIANT* pDownPose)
{
	/*侧面瓶口定位*/
	int id = (int)rgnId->fltVal;
	int ChannelSelect=(int)pChannelSelect->fltVal;
	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;
	int Threshold = (int)pThresholdValue->fltVal;    
    int AreaSize = (int)pAreaSize->fltVal;             // 8000
	int DownPose = (int)pDownPose->fltVal;             // 15

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}

		// 对图片进行旋转
		rotate_image(m_crtImgMult, &m_crtImgMult, 90, "constant");

		Hobject Image=m_crtImgMult;
		HTuple ChannelNum;
		count_channels(m_crtImgMult,&ChannelNum);
		if (ChannelNum[0].I()==3)
		{
			access_channel(m_crtImgMult,&Image,ChannelSelect);
			m_vInterImgs[id] = Image;
		}	
		Hlong Width, Height;
		get_image_size (m_crtImgMult, &Width, &Height);
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;

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

		Hobject rgnDetect,imgDetect,rgnThr,rgnFil,rgnOpn,rgnCls,rgnCon,rgnSel,rgnSort;
		gen_rectangle2(&rgnDetect,RowStart+RegHeight/2,ColStart+RegWidth/2,0,RegWidth/2,RegHeight/2);
		reduce_domain(Image, rgnDetect, &imgDetect);
		threshold(imgDetect, &rgnThr, Threshold, 255);
		fill_up(rgnThr, &rgnFil);
		//closing_circle (rgnFil, rgnFil, 3.5)
		opening_circle(rgnFil, &rgnOpn, 1.5);
		closing_rectangle1(rgnOpn, &rgnCls, 10, 10);
		connection(rgnCls, &rgnCon);
		select_shape(rgnCon, &rgnSel, "area", "and", AreaSize, 9999999);
		sort_region(rgnSel, &rgnSort, "first_point", "true", "row");
		HTuple Number;
		Hobject rgnTopOriCls;
		count_obj(rgnSort, &Number);

		if (Number < 1 )
		{
			Hobject Circle1;
			gen_circle(&Circle1, Height/2.0, Width/2.0, 20);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle1;
			return S_FALSE;
		}
		else
		{
			Hobject rgnTopOri,rgnTopOriMov,rgnTopMovDil,rgnTop ;
			select_obj(rgnSort, &rgnTopOri, 1);
			closing_rectangle1(rgnTopOri, &rgnTopOriCls, 10, 30);
			move_region(rgnTopOriCls, &rgnTopOriMov, DownPose, 1);
			if (DownPose <= 5 )
			{
				rgnTopMovDil = rgnTopOriMov;
			}
			else
			{
				dilation_rectangle1(rgnTopOriMov, &rgnTopMovDil, 20, 10);
			}
			union2(rgnTopMovDil, rgnTopOriCls, &rgnTop);
			closing_circle(rgnTop, &rgnTop, 30.5);

		    m_CenterTopRegion=rgnTop;//用于图案校正时候的位置仿射变换
		}

		HTuple CenterArea, CenterRow, CenterColumn;
		area_center(m_CenterTopRegion, &CenterArea, &CenterRow, &CenterColumn);

		// 获取顶部的下边缘
		Hobject  rgnTopOriClsFil,rgnTopBorder,rgnTopBorderClip,rgnTopBorderCon,rgnTopBorderSort,rgnTopBorderSel;
		HTuple dTopRow;
		fill_up(rgnTopOriCls,&rgnTopOriClsFil);
		region_features(rgnTopOriCls, "row", &dTopRow);
		boundary(rgnTopOriClsFil, &rgnTopBorder, "inner");
		clip_region_rel(rgnTopBorder, &rgnTopBorderClip, 0, 0, 10, 10);
		connection(rgnTopBorderClip, &rgnTopBorderCon);
		sort_region(rgnTopBorderCon, &rgnTopBorderSort, "first_point", "true", "row");
		select_shape(rgnTopBorderSort, &rgnTopBorderSel, "row", "and", dTopRow, 99999);

		m_topbaseLine=rgnTopBorderSel;           // 用于提取基准线区域，或者直接生成相应区域

		if (m_bDebugImg)
		{
			concat_obj(m_vRgnEx[id],rgnDetect,&m_vRgnEx[id]);
		}

		if ((CenterArea!=0)&& (CenterArea.Num()!=0))
		{			
			m_vPos[0].m_y = (float)CenterRow[0].D();
			m_vPos[0].m_x = (float)CenterColumn[0].D();
			m_vRgn[id] = m_CenterTopRegion;
			//
			return S_OK;
		}
		else
		{
			Hobject Circle1;
			gen_circle(&Circle1, Height/2.0, Width/2.0, 20);
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
STDMETHODIMP CBottleCapAlgo::PS_SideCenterTopLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5558$\
					 381;B$\
					 658;LP;1;3;1;1$\
					 938;LP;20;500;1;60$\
					 939;LP;34;600;1;150$\
					 940;LP;1;1300;1;900$\
					 941;LP;1;1000;1;180$\
					 1141;LP;1;255;1;150$\
					 6005;LP;1;30000;1;8000$\
					 1052;LP;1;50;1;15");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/***************************************************************************
** 函数名：PS_SideCenterBodyLocationAlgo
** 功  能：针对一种塑料瓶盖侧面拍摄的整体区域进行定位
** 时  间：2019年11月13日
****************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_SideCenterBodyLocationAlgo(VARIANT* rgnId, VARIANT* pChannelSelect,VARIANT* pLocationSelect,VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight, VARIANT* pThresholdValue)
{
	/*侧面整体定位*/
	int id = (int)rgnId->fltVal;
	int ChannelSelect=(int)pChannelSelect->fltVal; 
    int LocationSelect=(int)pLocationSelect->fltVal;   // 1--侧面整体定位；0--顶部以下区域定位
	int RowStart=(int)pRowStart->fltVal;
	int ColStart=(int)pColStart->fltVal;
	int RegWidth = (int)pRegWidth->fltVal; 
	int RegHeight=(int)pRegHeight->fltVal;
	int Threshold = (int)pThresholdValue->fltVal;    

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image=m_crtImgMult;
		HTuple ChannelNum;
		count_channels(m_crtImgMult,&ChannelNum);
		if (ChannelNum[0].I()==3)
		{
			access_channel(m_crtImgMult,&Image,ChannelSelect);
			m_vInterImgs[id] = Image;
		}	
		Hlong Width, Height;
		get_image_size (Image, &Width, &Height);
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
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

		Hobject rgnTop = m_CenterTopRegion;
		if ( LocationSelect == 1)
		{
			gen_empty_region(&rgnTop);
		}
		HTuple dTopCenRow;
		region_features(rgnTop,"row",&dTopCenRow);

		Hobject rgnDetect,imgDetect,rgnTopRectDil,rgnBodyThr,rgnOpnCir,rgnOpnFil,rgnSideBodyOri,rgnSideBodyOriCon,rgnSideBodyOriSel;
		gen_rectangle2(&rgnDetect,RowStart+RegHeight/2,ColStart+RegWidth/2,0,RegWidth/2,RegHeight/2);
		reduce_domain(Image, rgnDetect, &imgDetect);
		
		dilation_rectangle1(rgnTop, &rgnTopRectDil, 200, 1);
		threshold(imgDetect, &rgnBodyThr, Threshold, 255);
		opening_circle(rgnBodyThr, &rgnOpnCir, 10.5);
		fill_up(rgnOpnCir, &rgnOpnFil);
		difference(rgnOpnFil, rgnTopRectDil, &rgnSideBodyOri);

		connection(rgnSideBodyOri, &rgnSideBodyOriCon);
		select_shape(rgnSideBodyOriCon, &rgnSideBodyOriSel, "row", "and", dTopCenRow, 9999999);
		closing_circle(rgnSideBodyOriSel, &rgnSideBodyOriSel, 50.5);
		select_shape_std(rgnSideBodyOriSel, &rgnSideBodyOriSel, "max_area", 70);

		HTuple dRow1, dCol1, dPhi1, dLength1, dLength2;
		Hobject rgnSideBodyOriClip,rgnSideBodyOriTrans,rgnSideBody,rgnSide ;
		smallest_rectangle2(rgnSideBodyOriSel, &dRow1, &dCol1, &dPhi1, &dLength1, &dLength2);
		clip_region_rel(rgnSideBodyOriSel, &rgnSideBodyOriClip, dLength1/2.0, 20, 0, 0);
		shape_trans(rgnSideBodyOriClip, &rgnSideBodyOriTrans, "rectangle1");
		union2(rgnSideBodyOriTrans, rgnSideBodyOriSel, &rgnSideBody);
		union2(rgnSideBody, rgnTop, &rgnSide);
		closing_circle(rgnSide, &rgnSide, 50.5);
		closing_rectangle1(rgnSide, &rgnSide, 200, 1);

		fill_up(rgnSide, &rgnSide);
		opening_circle(rgnSide, &rgnSide, 10.5);
		union1(rgnSide, &rgnSide);

		if (m_bDebugImg)
		{
			concat_obj(m_vRgnEx[id],rgnDetect,&m_vRgnEx[id]);
		}

		HTuple CenterArea, CenterRow, CenterColumn;
		area_center(rgnSide, &CenterArea, &CenterRow, &CenterColumn);
		HTuple a=CenterArea;
		if ((CenterArea!=0)&& (CenterArea.Num()!=0))
		{			
			m_vRgn[id] = rgnSide;
			return S_OK;
			if ( LocationSelect == 1)
			{
				m_vPos[0].m_y = (float)CenterRow[0].D();
				m_vPos[0].m_x = (float)CenterColumn[0].D();
				m_vRgn[id] = rgnSide;
				return S_OK;
			}
		}
		else
		{
			Hobject Circle1;
			gen_circle(&Circle1, Height/2.0, Width/2.0, 20);
			m_vRgn[id] = Circle1;
			return S_FALSE;
			if ( LocationSelect == 1)
			{
				m_vPos[0].m_x = 0.0;
				m_vPos[0].m_y = 0.0;
				m_vRgn[id] = Circle1;
				return S_OK;
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
STDMETHODIMP CBottleCapAlgo::PS_SideCenterBodyLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6174$\
					 381;B$\
					 658;LP;1;3;1;1$\
					 6173;LP;0;1;1;0$\
					 939;LP;20;500;1;50$\
					 938;LP;34;600;1;150$\
					 940;LP;1;1300;1;900$\
					 941;LP;1;1500;1;850$\
					 1141;LP;1;255;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/***************************************************************************
** 函数名：PS_BaseEdgeDetectAlgo
** 功  能：针对一种塑料瓶盖侧面边缘线定位（包括顶部下边缘线、中部边缘线、底部边缘线）
** 时  间：2019年11月13日
****************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_BaseEdgeDetectAlgo(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pChannelSelect,VARIANT* pMoveSize,VARIANT* pSearchSize, VARIANT* pHSearchSize,VARIANT* pThreshold,VARIANT* pTransition, VARIANT* pPointSelection,VARIANT* pRemoveSize,VARIANT* pBaseEdgeArea)
{
	int rId           = (int)rgnId->fltVal;
	int Selection=(int)pSelection->fltVal;            // 1--顶部边缘线  2--中部边缘线  3--底部边缘线
	int ChannelSelect=(int)pChannelSelect->fltVal;
	int MoveSize    = (int)pMoveSize->fltVal;
	int SearchSize    =(int)pSearchSize->fltVal; 
    int HSearchSize   = (int)pHSearchSize->fltVal;
	int Threshold   = (int)pThreshold->fltVal;
	int Transition = (int)pTransition->fltVal;
	int PointSelection = (int)pPointSelection->fltVal;
	int RemoveSize   = (int)pRemoveSize->fltVal;

	m_vRgnExTest[rId].Reset();
	m_vRgnEx[rId].Reset();
	m_vInterImgs[rId].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[rId];
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseEdgeArea);
		return S_FALSE;
	}

	HTuple dArea1,dRow1,dCol1;
	region_features(m_topbaseLine,"area",&dArea1 );
	if ( dArea1 <= 0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseEdgeArea);
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
		//
		Hobject Image = m_crtImgMult;
		HTuple ChannelNum;
		count_channels(m_crtImgMult,&ChannelNum);
		if (ChannelNum==3){
			access_channel(m_crtImgMult,&Image,ChannelSelect);
		}
		//mean_image(Image,&Image,3,3);
		median_rect(Image,&Image,4,4);
		//
		m_vInterImgs[rId]=Image;
		Hobject TopSlopEdge,TopSlopEdgeDilation,TopSlopEdgeDiff;
		

		if ( Selection == 3)
		{
			move_region(m_rgnMidEge, &TopSlopEdge, MoveSize, 0);
		}
		else
		{
			move_region( m_topbaseLine, &TopSlopEdge, MoveSize, 0);
		}

		dilation_rectangle1(TopSlopEdge, &TopSlopEdgeDilation, 1, SearchSize); //实际情况调整膨胀搜索区域
		if (HSearchSize < 0)
		{
			dilation_rectangle1(TopSlopEdgeDilation, &TopSlopEdgeDilation, -HSearchSize, 1);
		}
		else
		{
			clip_region_rel(TopSlopEdgeDilation,&TopSlopEdgeDilation,0,0,HSearchSize,HSearchSize); // 水平尺寸缩放
		}
		
		Hobject TopSlopSearchReg,TopSlopImageReduced,ImageDomain;
		get_domain(Image,&ImageDomain);
		intersection(TopSlopEdgeDilation, ImageDomain, &TopSlopSearchReg);
		Hobject ImageSearchReg;
		reduce_domain(Image,TopSlopSearchReg,&ImageSearchReg);
		scale_image_max(ImageSearchReg,&ImageSearchReg);
		HTuple DefinedRow,DefinedCol;
		//PartInterval=5;LineLength:=膨胀尺寸;RemoveSize=2

		string strTransition, strPointSelection;

		switch (Transition)
		{
		case 1:
			strTransition = "negative" ;
			break;
		case 2:
			strTransition = "positive";
			break;
		case 3:
			strTransition = "all";
			break;
		default:
			strTransition = "all";
			break;
		}

		switch (PointSelection)
		{
		case 1:
			strPointSelection = "first";
			break;
		case 2:
			strPointSelection = "last";
			break;
		case 3:
			strPointSelection = "all";
			break;
		default:
			strPointSelection = "all";
			break;
		}

		R_EdgePointsFitCurve(TopSlopSearchReg, ImageSearchReg, 8, 2, Threshold, strTransition.c_str(), strPointSelection.c_str(),&DefinedRow, &DefinedCol);
		HTuple Length;
		tuple_length( DefinedRow,&Length);
		if (Length < 5 )
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pBaseEdgeArea);
			return S_FALSE;
		}

		// 筛除部分离群点
		HTuple DefinedRowMean,DefinedRowMeanS,dDiffS,dComapre,iComResultS,iIndisRest ;
		tuple_mean(DefinedRow, &DefinedRowMean);
		tuple_gen_const(DefinedRow.Num(), DefinedRowMean, &DefinedRowMeanS);
		dDiffS = (DefinedRow-DefinedRowMeanS).Abs();

		tuple_gen_const(dDiffS.Num(), RemoveSize, &dComapre);
		tuple_greater_elem(dDiffS, dComapre, &iComResultS);
		tuple_find(iComResultS, 1, &iIndisRest);

		tuple_remove(DefinedRow, iIndisRest, &DefinedRow);
		tuple_remove(DefinedCol, iIndisRest, &DefinedCol);
		
		if (DefinedRow.Num()==0)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pBaseEdgeArea);
			return S_FALSE;
		}
		Hobject Region,ABEdgeXld,ABEdgeSmoothed;
		gen_region_points(&Region, DefinedRow, DefinedCol);
		gen_contour_polygon_xld(&ABEdgeXld, DefinedRow, DefinedCol);
		smooth_contours_xld(ABEdgeXld, &ABEdgeSmoothed, 15);

		HTuple ABRow,ABCol,ABEdgeArea, ABEdgeRow, ABEdgeCol,PointOrder;
		Hobject ABEdge;
		get_contour_xld(ABEdgeSmoothed, &ABRow, &ABCol);
		if (ABRow.Num()<5)
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			retValue.Detach(pBaseEdgeArea);
			return S_FALSE;
		}

		// 二次剔除干扰点
		Hobject rgnABEdgeSmot,rgnABEdgeMov,rgnABEdgeMovFil,rgnABEdgeMovDif;
		gen_region_contour_xld(ABEdgeSmoothed,&rgnABEdgeSmot,"margin");
		if ( Selection == 3)
		{
			move_region(rgnABEdgeSmot,&rgnABEdgeMov,-3,0 );
		}
		else
		{
			move_region(rgnABEdgeSmot,&rgnABEdgeMov,3,0 );
		}	
		fill_up(rgnABEdgeMov,&rgnABEdgeMovFil);
		difference(rgnABEdgeSmot,rgnABEdgeMovFil,&rgnABEdgeMovDif );

		Hobject rgnBaseLinePoints,xldCross,rgnCross;
		HTuple BaseLineArea, BaseLineRow, BaseLineCol;
		gen_region_points(&rgnBaseLinePoints, ABRow, ABCol);
		area_center(rgnBaseLinePoints, &BaseLineArea, &BaseLineRow, &BaseLineCol);
		gen_cross_contour_xld(&xldCross,ABRow,ABCol,5,0.618 );
		gen_region_contour_xld(xldCross,&rgnCross, "filled");
		m_BaseEdgeRow=BaseLineRow;
		if (m_bDebugImg)
		{
			concat_obj(m_vRgn[rId],TopSlopSearchReg,&m_vRgn[rId]);
			Hobject rgnCrossDil;
			dilation_circle(rgnCross,&rgnCrossDil,0.5);
			concat_obj(m_vRgn[rId],rgnCrossDil,&m_vRgn[rId]);
		}

		if (Selection == 1 )
		{
			// 顶部下边缘基准线
			//得到基部边缘。
			m_ABEdgeReg = rgnABEdgeMovDif;
			m_rgnTopDownEge = rgnABEdgeMovDif;

		}
		else if(Selection == 2)
		{
			// 中部边缘基准线
			Hobject rgnLine,rgnLineDil ,rgnLineDilClip;
			gen_region_line(&rgnLine,ABRow.Select((ABRow.Num())/3),ABCol.Select((ABRow.Num())/3), ABRow.Select((((ABRow.Num())*2)/3)+1),ABCol.Select((((ABRow.Num())*2)/3)+1));;
			dilation_rectangle1(rgnLine, &rgnLineDil, 1000, 1);
			skeleton(rgnLineDil, &rgnLineDil);

			HTuple  dRow1,dCol1,dRow2,dCol2,Height ;
			Height = m_imgHeight;
			smallest_rectangle1(m_CenterTopRegion,&dRow1,&dCol1,&dRow2,&dCol2);
			clip_region(rgnLineDil,&rgnLineDilClip,1,dCol1 ,Height-1,dCol2 );
			m_rgnMidEge = rgnLineDilClip;

		}
		else if(Selection == 3)
		{
			// 底部边缘基准线
			m_rgnBottomEge = rgnABEdgeMovDif;
		}
		else
		{
			Hobject Circle;
			gen_circle(&Circle, m_imgHeight/2.0, m_imgWidth/2.0, 100);
			retValue = -1;
			m_vErrorRgn[rId] = Circle;
		}

		area_center_xld(ABEdgeSmoothed, &ABEdgeArea, &ABEdgeRow, &ABEdgeCol,&PointOrder);
		//
		if (ABEdgeArea>0)
		{
			Hobject rgnBaseLinePointsDil;
			retValue = ABEdgeArea[0].I();
			dilation_circle(rgnBaseLinePoints, &rgnBaseLinePointsDil, 2);
			m_vErrorRgn[rId] = rgnBaseLinePointsDil;
		}
		else
		{
			Hobject Circle;
			gen_circle(&Circle, m_imgHeight/2.0, m_imgWidth/2.0, 100);
			retValue = -1;
			m_vErrorRgn[rId] = Circle;
		}
		retValue.Detach(pBaseEdgeArea);
#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseEdgeArea);
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pBaseEdgeArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PS_BaseEdgeDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5559$\
					 397;R;FP;0;10000;1;>=#1$\
					 381;B$\
					 6175;LP;1;3;1;1$\
					 658;LP;1;3;1;1$\
					 991;LP;0;1000;1;58$\
					 998;LP;1;100;1;30$\
					 1101;LP;-100;100;1;1$\
					 1304;LP;1;60;1;15$\
					 6075;LP;1;3;1;1$\
					 6076;LP;1;3;1;1$\
					 6118;LP;1;50;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/***************************************************************************
** 函数名：PS_SideEdgeRegionLocationAlgo
** 功  能：根据PS_BaseEdgeDetectAlgo算子提取的边缘线生成对应的区域 （不同位置选择不同的边缘线：顶部下边缘线、中部边缘线、底部边缘线）
** 时  间：2019年11月13日
****************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_SideEdgeRegionLocationAlgo(VARIANT* rgnId,VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize)
{
	int rId           = (int)rgnId->fltVal;
	int Selection   = (int)pSelection->fltVal;
	int MoveSize   = (int)pMoveSize->fltVal;
	int HErosionSize   = (int)pHErosionSize->fltVal;
	int VErosionSize   = (int)pVErosionSize->fltVal;
	//
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	m_vInterImgs[rId].Reset();
	Hobject Image=m_crtImgMult;
	m_vInterImgs[rId]=Image;

	if(m_ABEdgeReg.Id() == H_EMPTY_REGION)
	{
		m_vRgn[rId] = newRgn ;
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
		Hobject ABEdgeMoved,RegionDilation;
		if ( Selection== 1)
		{
			// 顶部下边缘线
			dilation_rectangle1(m_ABEdgeReg,&RegionDilation, 1, VErosionSize);
		}
		else if ( Selection== 2)
		{
			// 中部边缘线
			dilation_rectangle1(m_rgnMidEge,&RegionDilation, 1, VErosionSize);
		}
		else if( Selection== 3)
		{
			// 底部边缘线
			dilation_rectangle1(m_rgnBottomEge,&RegionDilation, 1, VErosionSize);
		}
		else
		{
			dilation_rectangle1(m_ABEdgeReg,&RegionDilation, 1, VErosionSize);
		}

		//水平缩放
		clip_region_rel(RegionDilation, &RegionDilation, 0, 0, HErosionSize, HErosionSize);
		move_region(RegionDilation,&RegionDilation,MoveSize,0);
		m_vRgn[rId] = RegionDilation;
#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PS_SideEdgeRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6176$\
					 167;B$\
					 6175;LP;1;3;1;1$\
					 991;LP;-600;600;1;0$\
					 994;LP;1;100;1;6$\
					 995;LP;1;500;1;6");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CBottleCapAlgo::GenPatternRegion(Hobject Image,Hobject* rgnSmlCharS,Hobject* rgnBigCharS,Hobject* rgnCircle,HTuple dRowBgn,HTuple dColBgn,HTuple SmlCharHeight,HTuple SmlCharWidth,HTuple SmlCharSpace,HTuple CharDistHeight,HTuple CharDistWidth, HTuple BigCharHeight,HTuple BigCharWidth,HTuple BigCharSpace,HTuple CircleWidth,HTuple CircleHeight)
{
	// 生成图案区域，选择图片的时候尽量选择图案位于中间的图案
	//* Small Character
	gen_empty_obj(&(*rgnSmlCharS));
	Hobject rgnSmlChar1,rgnSmlChar2,rgnSmlChar3,rgnSmlCharSTemp;
	gen_empty_obj(&rgnSmlCharSTemp);
    gen_rectangle1(&rgnSmlChar1, dRowBgn, dColBgn, dRowBgn+SmlCharHeight,dColBgn+SmlCharWidth);
    gen_rectangle1(&rgnSmlChar2,dRowBgn+SmlCharHeight+SmlCharSpace, dColBgn, dRowBgn+SmlCharHeight*2+SmlCharSpace, dColBgn+SmlCharWidth);
    gen_rectangle1(&rgnSmlChar3, dRowBgn+SmlCharHeight*2+SmlCharSpace*2, dColBgn, dRowBgn+SmlCharHeight*3.2+SmlCharSpace, dColBgn+SmlCharWidth);

    concat_obj((*rgnSmlCharS), rgnSmlChar1, &(*rgnSmlCharS));
    concat_obj((*rgnSmlCharS), rgnSmlChar2, &(*rgnSmlCharS));
    concat_obj((*rgnSmlCharS), rgnSmlChar3, &(*rgnSmlCharS));

    //* Big Character
	Hobject rgnBigCharUp,rgnBigCharDown,rgnBigChar1,rgnBigChar2;
	HTuple dBigCharRowBgn,dBigCharColBgn;
	dBigCharRowBgn = dRowBgn - CharDistHeight;
	dBigCharColBgn = dColBgn + SmlCharWidth + CharDistWidth;

    gen_rectangle1(&rgnBigChar1,dBigCharRowBgn, dBigCharColBgn, dBigCharRowBgn + BigCharHeight,dBigCharColBgn + BigCharWidth);
    gen_rectangle1(&rgnBigChar2, dBigCharRowBgn + BigCharHeight + BigCharSpace, dBigCharColBgn - 3, dBigCharRowBgn + BigCharSpace + BigCharHeight*2 +14 , dBigCharColBgn + BigCharWidth+3);

	gen_empty_obj(&(*rgnBigCharS));
    concat_obj((*rgnBigCharS), rgnBigChar1, &(*rgnBigCharS));
    concat_obj((*rgnBigCharS), rgnBigChar2, &(*rgnBigCharS));

	// Circle Region
	HTuple dCircleRowBgn,dCircleColBgn;
	Hobject  rgnRectCircle;
	dCircleRowBgn = dBigCharRowBgn;
	dCircleColBgn = dColBgn + SmlCharWidth + CharDistWidth + BigCharWidth;

	gen_empty_obj(&(*rgnCircle));
	gen_rectangle1(&rgnRectCircle,dCircleRowBgn,dCircleColBgn - CircleWidth - 5,dCircleRowBgn+CircleHeight,dCircleColBgn -5);
	concat_obj((*rgnCircle), rgnRectCircle, &(*rgnCircle));
   
}
void CBottleCapAlgo::PatternLocation(Hobject* rgnSmlCharS,Hobject* rgnBigCharS,Hobject* rgnCircle,Hobject rgnPatternBorder,Hobject SmlCharS,Hobject BigCharS ,Hobject CircleRU,HTuple PoseAdjustRow,HTuple PoseAdjustCol,HTuple MoveRight)
{
	// according to the pattern border to moving the pattern
	HTuple dArea,dRow,dCol;
	Hobject CharUni;
	union2(SmlCharS,BigCharS,&CharUni);
	union1(CharUni,&CharUni);
	HTuple dRow1,dCol1,dRow2,dCol2, dRowBorder1,dColBorder1,dRowBorder2,dColBorder2;
	smallest_rectangle1(CharUni,&dRow1,&dCol1,&dRow2,&dCol2 );
	smallest_rectangle1(rgnPatternBorder, &dRowBorder1,&dColBorder1,&dRowBorder2,&dColBorder2);
	HTuple iLength; 
	tuple_length(dRowBorder1,& iLength);
	if ( iLength < 1)
	{
		gen_empty_obj(&(*rgnSmlCharS));
		gen_empty_obj(&(*rgnBigCharS));
		return ;
	}

	//area_center(CharUni, &dArea, &dRow, &dCol);

	Hobject SmlCharSMov,SmlCharSMovL,SmlCharSMovR ;
	Hobject BigCharSMov,BigCharSMovL,BigCharSMovR ;
	HTuple RectRightUpRow,RectRightUpCol;
	RectRightUpRow = dRowBorder1 - PoseAdjustRow - dRow1 ;
	RectRightUpCol = dColBorder2 + PoseAdjustCol -dCol2;

	// 小字符
	move_region(SmlCharS, &SmlCharSMov, RectRightUpRow , RectRightUpCol);
	move_region(SmlCharS, &SmlCharSMovL,RectRightUpRow, RectRightUpCol - MoveRight);
	move_region(SmlCharS, &SmlCharSMovR,RectRightUpRow,  RectRightUpCol +  MoveRight);
	union2(SmlCharSMov, SmlCharSMovL, &(*rgnSmlCharS));
	union2((*rgnSmlCharS), SmlCharSMovR, &(*rgnSmlCharS));

	// 大字符
	move_region(BigCharS, &BigCharSMov,RectRightUpRow, RectRightUpCol);
	move_region(BigCharS, &BigCharSMovL, RectRightUpRow, RectRightUpCol - MoveRight);
	move_region(BigCharS, &BigCharSMovR, RectRightUpRow,  RectRightUpCol +  MoveRight);
	union2(BigCharSMov, BigCharSMovL, &(*rgnBigCharS));
	union2((*rgnBigCharS), BigCharSMovR, &(*rgnBigCharS));

	// 右上角小圆区域
	Hobject CircleRUMove,CircleRUL,CircleRUR;
	move_region(CircleRU, &CircleRUMove,RectRightUpRow, RectRightUpCol);
	move_region(CircleRU, &CircleRUL, RectRightUpRow, RectRightUpCol - MoveRight);
	move_region(CircleRU, &CircleRUR, RectRightUpRow,  RectRightUpCol +  MoveRight);

	union2(CircleRUL, CircleRUR, &(*rgnCircle));
	union2((*rgnCircle), CircleRUMove, &(*rgnCircle));

	//my_disp_obj( (*CircleRU));
}
/************************************************************************************************************
** 函数名：PS_PatternRegionLocationAlgo
** 功  能：根据PS_PatternReferenceLocationAlgo算子提取的黑色区域作为图案定位的参考位置进而对图案区域进行定位
** 时  间：2019年11月13日
***********************************************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_PatternRegionLocationAlgo(VARIANT* rgnId, VARIANT* pMode, VARIANT* pChannelSelect, VARIANT* pRegionSelect,VARIANT* pPoseAdjustRow,VARIANT* pPoseAdjustCol,VARIANT* pMoveSize,VARIANT* pRowBgn,VARIANT* pColBgn,VARIANT* pSmlCharHeight,VARIANT* pSmlCharWidth,\
	                                                VARIANT* pSmlCharSpace,VARIANT* pCharDistHeight,VARIANT* pCharDistWidth, VARIANT* pBigCharHeight,VARIANT* pBigCharWidth,VARIANT* pBigCharSpace,VARIANT* pCircleWidth,VARIANT* pCircleHeight)
{
	int id = (int)rgnId->fltVal;
	//m_vErrorRgn[id].Reset();
	//m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();  
	m_vRgn[id].Reset();
	
	Hobject rgn  = m_vRgn[id];
	int Mode = (int)pMode->fltVal;
	int ChannelSelect=(int)pChannelSelect->fltVal;
	int RegionSelect=(int)pRegionSelect->fltVal;           // 区域选择，1--小字符区域；2-大字符区域；3-字符整体区域
    int PoseAdjustRow = (int)pPoseAdjustRow->fltVal;      // 25
	int PoseAdjustCol = (int)pPoseAdjustCol->fltVal;      // 50
	int MoveSize = (int)pMoveSize->fltVal;          // 360

	int RowBgn = (int)pRowBgn->fltVal;               // 0-800
	int ColBgn = (int)pColBgn->fltVal;               // 0-800
	int SmlCharHeight = (int)pSmlCharHeight->fltVal; // 44
	int SmlCharWidth = (int)pSmlCharWidth->fltVal;   // 50
	int SmlCharSpace = (int)pSmlCharSpace->fltVal;   // 2

	int CharDistHeight = (int)pCharDistHeight->fltVal;  // 35 
	int CharDistWidth = (int)pCharDistWidth->fltVal;    // 8

	int BigCharHeight = (int)pBigCharHeight->fltVal;    // 105
	int BigCharWidth = (int)pBigCharWidth->fltVal;      // 78
	int BigCharSpace = (int)pBigCharSpace->fltVal;      // 3

	int CircleWidth = (int)pCircleWidth->fltVal;      // 34
	int CircleHeight = (int)pCircleHeight->fltVal;      // 34

	Hobject newRgn;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}		
		Hobject Image=m_crtImgMult;
		HTuple ChannelNum;
		
		//标定文件路径
		string SmlCharRegionFile = m_strTaskPathWithoutCameraIndex;
		string BigCharRegionFile = m_strTaskPathWithoutCameraIndex;
        string CircleRegionFile = m_strTaskPathWithoutCameraIndex;

		if(m_bDebugImg)
			m_dictSpecialData[id].clear();
		
		SmlCharRegionFile.append("\\");
		SmlCharRegionFile.append("SmlCharSRegion");
		SmlCharRegionFile.append(".reg");

		BigCharRegionFile.append("\\");
		BigCharRegionFile.append("BigCharSRegion");
		BigCharRegionFile.append(".reg");

		CircleRegionFile.append("\\");
		CircleRegionFile.append("CircleRegion");
		CircleRegionFile.append(".reg");

		Hobject ImageMappedRotate,imgMappedSingle;
		Hobject ImageMapped ;
		ImageMapped = m_ImageMapped;

		count_channels(ImageMapped,&ChannelNum);
		if (ChannelNum[0].I()==3)
		{
			access_channel(ImageMapped,&imgMappedSingle,ChannelSelect);
		}
		else
		{
			imgMappedSingle = m_crtImg;
		}

		Hobject ImageDomain;
		get_domain(imgMappedSingle,&ImageDomain);
		
		m_vInterImgs[id]=imgMappedSingle;
	
		HTuple dArea ;
		dArea = 0;
		//model=0训练模式，model=1检测模式
		if (Mode==0)
		{
			//根据汉字高度和间隔调整印刷区域尺寸,调节平移距离，将MovedPrintingRegion与模板图片套准
			Hobject rgnSmlCharS, rgnBigCharS,rgnCharS,rgnCircle;
			GenPatternRegion(imgMappedSingle,&rgnSmlCharS,&rgnBigCharS,&rgnCircle,RowBgn,ColBgn,SmlCharHeight,SmlCharWidth,SmlCharSpace,CharDistHeight,CharDistWidth,BigCharHeight,BigCharWidth,BigCharSpace,CircleWidth,CircleHeight);
			union2(rgnSmlCharS,rgnBigCharS,&rgnCharS );
			concat_obj(m_vRgnExTest[id],rgnCharS,&m_vRgnExTest[id]);
			//保存本地
			write_region(rgnBigCharS,BigCharRegionFile.c_str());
			write_region(rgnSmlCharS,SmlCharRegionFile.c_str());
			write_region(rgnCircle,CircleRegionFile.c_str());
			if (m_bDebugImg)
			{
				concat_obj(m_vRgn[id],rgnSmlCharS,&m_vRgn[id]);
				concat_obj(m_vRgn[id],rgnBigCharS,&m_vRgn[id]);
				concat_obj(m_vRgn[id],rgnCircle,&m_vRgn[id]);
			}

		}
		else if(Mode==1)
		{
			if(_access(BigCharRegionFile.c_str(),0)||_access(SmlCharRegionFile.c_str(),0)||_access(CircleRegionFile.c_str(),0) )
			{
				Hobject newRgn;
				m_vRgn[id] = newRgn;
			    return S_FALSE;
			}
			
			Hobject rgnBigCharS,rgnSmlCharS,rgnPatternBorder,rgnCircle;
			rgnPatternBorder = m_rgnPatternCorner;
		
			//读取
			Hobject BigCharRegion,SmlCharRegion,CircleRU;
			read_region(&BigCharRegion, BigCharRegionFile.c_str());
			read_region(&SmlCharRegion, SmlCharRegionFile.c_str());
			read_region(&CircleRU, CircleRegionFile.c_str());

			PatternLocation(&rgnSmlCharS,&rgnBigCharS,&rgnCircle,rgnPatternBorder,SmlCharRegion,BigCharRegion,CircleRU,PoseAdjustRow,PoseAdjustCol,MoveSize);

			Hobject rgnPattern;
			HTuple dCol,dRow;
			union2(rgnSmlCharS,rgnBigCharS,&rgnPattern );   
			area_center(rgnPattern, &dArea, &dRow, &dCol);

			if (dArea > 0)
			{
				intersection(ImageDomain,rgnPattern,&rgnPattern);
				intersection(ImageDomain,rgnSmlCharS,&rgnSmlCharS);
				intersection(ImageDomain,rgnBigCharS,&rgnBigCharS);
				intersection(ImageDomain,rgnCircle,&rgnCircle);

				// 传递图案区域参数
				m_rgnSmlCharS = rgnSmlCharS;
				m_rgnBigCharS = rgnBigCharS;
				m_rgnCircle   = rgnCircle;
				m_rgnPattern  = rgnPattern;

				if ( RegionSelect == 1)
				{
					// 小字符区域
					m_vRgn[id] = rgnSmlCharS;

				}
				else if (RegionSelect == 2)
				{
					// 大字符字符区域
					m_vRgn[id] = rgnBigCharS;
				}
				else if ( RegionSelect == 0)
				{
					//右上角小圆区域
					m_vRgn[id] = rgnCircle;
				}
				else 
				{
					// 整体区域
					m_vRgn[id] = rgnPattern;
				}

			}
			else
			{
				Hobject newRgn;
				m_vRgn[id] = newRgn;
				return S_FALSE;

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
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PS_PatternRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6193$\
					 167;B$\
					 1340;LP;0;1;1;1$\
					 658;LP;1;3;1;1$\
					 6192;LP;0;3;1;3$\
					 6188;LP;1;200;1;25$\
					 6191;LP;1;200;1;45$\
					 6189;LP;1;600;1;360$\
					 6177;LP;1;600;1;230$\
					 6178;LP;1;600;1;170$\
					 6180;LP;1;300;1;44$\
					 6181;LP;1;300;1;50$\
					 6182;LP;1;200;1;2$\
                     6183;LP;1;200;1;35$\
					 6184;LP;1;100;1;8$\
					 6185;LP;1;300;1;105$\
					 6186;LP;1;300;1;78$\
					 6187;LP;1;100;1;3$\
					 6227;LP;1;100;1;33$\
					 6228;LP;1;100;1;33"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*******************************************************************************
** 函数名：PS_PatternRegionLocationAlgo1Help
** 功  能：根据图案区域定位算子生成图案检测区域算子
** 时  间：2019年12月12日
********************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_PatternRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("6233$\
					 167;B$\
					 6234;LP;0;3;1;3$\
					 1063;LP;-10;20;1;1"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PS_PatternRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pSelect, VARIANT* pScaleSize)
{
	//得到环形区域
	int id          = (int)rgnId->fltVal;
	int Select      = (int)pSelect->fltVal; // 0--小圆区域；1--小字符区域；2--大字符区域；3--整体区域
	int ScaleSize   = (int)pScaleSize->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();

	HTuple dArea,dArea1,dArea2,dArea3;
	Hobject rgnPattern,rgnSmlCharS,rgnBigCharS,rgnCircle;
	rgnPattern = m_rgnPattern;
	rgnSmlCharS = m_rgnSmlCharS;
	rgnBigCharS = m_rgnBigCharS;
	rgnCircle   = m_rgnCircle;
	
	region_features(rgnPattern,"area",& dArea);
	region_features(rgnSmlCharS,"area",& dArea1);
	region_features(rgnBigCharS,"area",& dArea2);
	region_features(rgnCircle,"area",& dArea3);
	
	if ( (dArea <=  0) || (dArea <=  1) || (dArea <=  2) || (dArea <=  3))
	{
		Hobject newRgn;
		m_vRgn[id] = newRgn;
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

		Hobject Image,rgnDetect;
		HTuple imgMappedHeight,imgMappedWidth;
		imgMappedHeight = m_mapimg_height;
		imgMappedWidth = m_mapimg_width;
		Image = m_ImageMapped;

		m_vInterImgs[id] = Image;

		if (Select == 0 )
		{
			rgnDetect = rgnCircle;
		}
		else if ( Select == 1 )
		{
			rgnDetect = rgnSmlCharS;
		}
		else if (Select == 2)
		{
			rgnDetect = rgnBigCharS;
		}
		else if (Select == 3)
		{
			rgnDetect = rgnPattern;
		}
		else
		{
			rgnDetect = rgnPattern;
		}

		if (ScaleSize > 0 )
		{
			dilation_rectangle1(rgnDetect,&rgnDetect,ScaleSize,ScaleSize );
		}
		if ( ScaleSize < 0)
		{
			erosion_rectangle1(rgnDetect,&rgnDetect,-ScaleSize,-ScaleSize );
		}
		
		m_vRgn[id] = rgnDetect ;	
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
/*******************************************************************************
** 函数名：PS_PatternDefectDetectAlgo
** 功  能：检测图案印刷区域上的图案缺失问题
** 时  间：2019年12月13日
********************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_PatternDefectDetectAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pWidthSelect,VARIANT* pWidthThred, VARIANT* pPatternRatio,VARIANT* pAreaThred,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int ColorTransType               = (int)pColorTransType->fltVal;
	int ChannelSelect                = (int)pChannelSelect->fltVal;
	int WidthSelect                  = (int)pWidthSelect->fltVal;
	int WidthThred                   = (int)pWidthThred->fltVal;
	float PatternRatio               = (float)pPatternRatio->fltVal;
	int AreaThred                    = (int)pAreaThred->fltVal;

	int BlackMaskSize                = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize        = (int) pSeriousBlackPointSize->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		
		Hobject rgnCon,rgnSel;
		HTuple  tWidthS,iNumber,iNumber1;
		connection(rgn,&rgnCon );
		select_shape(rgnCon,&rgnSel,"width","and",WidthSelect,9999999 );
		count_obj(rgnSel,&iNumber );
		if (iNumber < 1)
		{
			// 区域为空时不报错
			retValue = 1000;
			m_vErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_OK;
		}

		Hobject Image;
		if(!H_ColorImageTrans(m_ImageMapped,&Image,rgn,  ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[id]=Image;

		Hobject rgnBlackUniWhole,rgnDefect,rgnDefectRect;
		gen_empty_obj(&rgnBlackUniWhole );
		gen_empty_obj(&rgnDefect );
		gen_empty_obj(&rgnDefectRect );

		for(int i = 1;i <= iNumber;i++)
		{
			Hobject  objSel;
			HTuple tAreaTemp,tRowTemp,tColTemp ;
			select_obj(rgnSel,&objSel,i );
			area_center(objSel,&tAreaTemp,&tRowTemp,&tColTemp );

			Hobject Image;
			if(!H_ColorImageTrans(m_ImageMapped,&Image,objSel,  ColorTransType, ChannelSelect))
			{
				retValue = -1;
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
				retValue.Detach(pDetectArea);
				return S_FALSE;
			}
			Hobject ImageReduced, ImageReduceMean;
			Hobject BlackUnion, WhiteUnion;
			HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
			//
			//预处理
			HTuple MaskSize;
			MaskSize = 3;
			mean_image(Image, &Image, MaskSize, MaskSize);
			//
			if(SeriousBlackPointDynThresh<254)
			{
				ImageProcessAlg.MeanImageReduced(Image,objSel, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
				ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
				area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
			}

			union1(BlackUnion,&BlackUnion );
			concat_obj(rgnBlackUniWhole,BlackUnion ,&rgnBlackUniWhole );

			// 判断1(图案宽度)
			HTuple  tWidth;
			region_features(BlackUnion,"width",&tWidth );

			// 判断2 ( 图案比例阈值)
			HTuple  tArea,tRatio;
			region_features(BlackUnion,"area",&tArea );  
			tRatio = tArea/(tAreaTemp[0].D());

			if (tWidth < WidthThred )
			{
				concat_obj(rgnDefect,BlackUnion ,&rgnDefect );
				concat_obj(rgnDefectRect,objSel ,&rgnDefectRect );
			}
			else if (tRatio < PatternRatio )
			{
			    concat_obj(rgnDefect,BlackUnion ,&rgnDefect);
				concat_obj(rgnDefectRect,objSel ,&rgnDefectRect );
			}
			else if (tArea < AreaThred )
			{
				concat_obj(rgnDefect,BlackUnion ,&rgnDefect);
				concat_obj(rgnDefectRect,objSel ,&rgnDefectRect );
			}

			HTuple dAreaBlack,dRowBlack,dColBlack;
			area_center(BlackUnion,&dAreaBlack,&dRowBlack,&dColBlack );
			if (dAreaBlack > 0 )
			{
				 concat_obj(m_vRgnExTest[id],BlackUnion ,&m_vRgnExTest[id]);
			}

		}

		union1(rgnDefect,&rgnDefect);
		HTuple blackArea,dRow,dCol;
		area_center(rgnDefect,&blackArea,&dRow,&dCol );

		Hobject DefectRegs; 
		HTuple Area, Row_test, Column_test;

		if(blackArea > 0 )
		{
			union1(rgnDefect, &DefectRegs);
			area_center(DefectRegs, &Area, &Row_test, &Column_test);
			retValue = 0.0;
		}
		else
		{
			retValue = 100.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}

		union1(rgnDefectRect,&rgnDefectRect );
		erosion_circle (rgnDefectRect, &rgnDefectRect, 3.5);
		m_vErrorRgn[id] = rgnDefectRect;

		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PS_PatternDefectDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6239$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 657;LP;0;6;1;0$\
					 658;LP;1;3;1;1$\
					 6235;LP;10;120;1;20$\
					 6236;LP;10;120;1;25$\
					 6237;FP;0.01;1;0.01;0.1$\
					 6238;LP;1;20000;1;100$\
					 412;LP;3;101;2;7$\
					 415;LP;1;255;1;60$\
					 416;LP;1;1000;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*******************************************************************************
** 函数名：PS_PatternReferenceLocationAlgo
** 功  能：图案参考位置定位(定位毛的右上角黑色区域)
** 时  间：2019年11月14日
********************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_PatternReferenceLocationAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pSerchRowPos,VARIANT* pSerchWidth ,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	int id = rId;
	m_vRgnExTest[rId].Reset();
	m_vRgnEx[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vInterImgs[rId].Reset();
	Hobject rgn = m_vRgn[rId];

	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int ColorTransType = (int)pColorTransType->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	int SerchRowPos = (int) pSerchRowPos->fltVal;
	int SerchWidth = (int) pSerchWidth->fltVal;
	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
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
		Hobject Image= m_crtImgMult;

		//标定文件路径
		string MapImgFile = m_strTaskPathWithoutCameraIndex;
		string TopRegionFile = m_strTaskPathWithoutCameraIndex;

		if(_access(MapImgFile.c_str(),0))
		{
			CreateDirectoryA(MapImgFile.c_str(), NULL);
		}
		MapImgFile.append("\\");
		MapImgFile.append("MapImage");
		MapImgFile.append(".tif");

		TopRegionFile.append("\\");
		TopRegionFile.append("TopRegion");
		TopRegionFile.append(".reg");

		if(m_isFirst)
		{         
			if(_access(MapImgFile.c_str(),0)||_access(TopRegionFile.c_str(),0))
			{
				retValue = -1;
				retValue.Detach(pDetectArea);
				m_vErrorRgn[id].Reset();
				return S_FALSE;
			}
			read_image(&m_Map, MapImgFile.c_str());
			read_region(&m_CanTopRegion,TopRegionFile.c_str());
			m_isFirst=false;
		}

		HTuple ColCenter,RowCenter;
		region_features(m_CanTopRegion, "column", &ColCenter);
		region_features(m_CanTopRegion, "row", &RowCenter);
		//
		HTuple RowPos,ColumnPos;
		region_features(m_CenterTopRegion, "row", &RowPos);              // 侧面中心定位算子传出
		region_features(m_CenterTopRegion, "column", &ColumnPos);

		HTuple RowDiff = RowPos-RowCenter;
		HTuple ColDiff = ColumnPos-ColCenter;
		if (ColDiff.Abs()>300)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		HTuple HomMat2D;
		Hobject ImageAffinTrans,ImageMapped,imgMappedSingle;
		vector_angle_to_rigid(0, 0, 0, -RowDiff, -ColDiff, 0, &HomMat2D);
		affine_trans_image(Image, &ImageAffinTrans, HomMat2D, "constant", "false");
		map_image(ImageAffinTrans, m_Map, &ImageMapped);
		//Hobject ImageMappedRotate;
		get_image_size(ImageMapped, &m_mapimg_width, &m_mapimg_height);
		//m_ImageMapped彩色用于检测
		m_ImageMapped=ImageMapped;
		HTuple ChannelNum;
		count_channels(ImageMapped,&ChannelNum);
		if (ChannelNum[0].I()==3){
			access_channel(ImageMapped,&imgMappedSingle,ChannelSelect);
		}
		else
		{
			imgMappedSingle = m_crtImg;
		}
		m_imgMappedSingle=imgMappedSingle;
		Hobject ImageDomain;
		get_domain(imgMappedSingle,&ImageDomain);
		m_vInterImgs[id]=imgMappedSingle;

		Hobject rgnSerch,imgRectDetect ;
		gen_rectangle1(&rgnSerch,SerchRowPos , 0, SerchRowPos+SerchWidth, m_mapimg_width-1);
		reduce_domain(imgMappedSingle, rgnSerch, &imgRectDetect);
		if(m_bDebugImg)
		{
			concat_obj(m_vRgn[rId], rgnSerch, &m_vRgn[rId]);
		}

		Hobject ImageReduced, ImageReduceMean,ExpandedImage,ImageDetectNew;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		//预处理
		HTuple MaskSize;
		MaskSize = 3;
		Hobject  imgRectDetectScale,ImageMedian;

		scale_image_max(imgRectDetect, &imgRectDetectScale);
		median_image(imgRectDetectScale, &ImageMedian, "circle", 3, "mirrored");
		gray_dilation_rect(ImageMedian, &imgRectDetectScale, 3, 3);

		//
		//ImageProcessAlg.MeanImageReduced(imgRectDetectScale,rgnSerch, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		expand_domain_gray (imgRectDetectScale, &ExpandedImage, 45);
		reduce_domain (ExpandedImage, rgnSerch, &ImageDetectNew); 
		mean_image(ImageDetectNew, &ImageReduceMean, BlackMaskSize, BlackMaskSize);
		//
		Hobject SeriousPointDynThredImg,SeriousConnected,RegionErosion,RegionIntersection,SeriousPointReg;	
		erosion_circle (rgnSerch, &RegionErosion, 2.5);
		dyn_threshold(ImageDetectNew, ImageReduceMean, &SeriousPointDynThredImg, SeriousBlackPointDynThresh, "dark");	

		Hobject rgnPatternDark,rgnPatternDarkCls,rgnPatternDarkOpn ;
		closing_circle(SeriousPointDynThredImg, &rgnPatternDarkCls, 2.5);
		opening_circle(rgnPatternDarkCls, &rgnPatternDarkOpn, 2.5);

		Hobject rgnRectDetectEro;
		erosion_rectangle1(rgnSerch, &rgnRectDetectEro, 20, 1);

		intersection (SeriousPointDynThredImg, rgnRectDetectEro, &RegionIntersection);
		connection(RegionIntersection, &SeriousConnected); 
		select_shape(SeriousConnected, &SeriousPointReg, "area", "and", SeriousBlackPointSize, 99999);
		select_shape_std(SeriousPointReg, &SeriousPointReg, "max_area", 70);
		union1(SeriousPointReg, &BlackUnion);

		// 传递该图案定位参考位置
		m_rgnPatternCorner = SeriousPointReg;

		if(m_bDebugImg)
		{
			concat_obj(m_vRgn[rId], BlackUnion, &m_vRgn[rId]);
		}
		//
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;
		DefectRegs = BlackUnion;
		//union1(BlackUnion,&DefectRegs);
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
STDMETHODIMP CBottleCapAlgo::PS_PatternReferenceLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6190$\
					 397;R;FP;0;10000;1;>=#1$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 5563;LP;1;600;1;220$\
					 5564;LP;1;200;1;30$\
					 412;LP;3;101;2;45$\
					 415;LP;1;250;1;60$\
					 416;LP;1;200;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/***************************************************************************************
**函数名：PS_RemovePatternRegionLocationAlgo
**功  能：图案平缓区域定位（扣除图案区域）
**时  间：2019年11月15日
****************************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_RemovePatternRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6195$\
					 167;B$\
					 6011;LP;1;800;1;1$\
					 6013;LP;1;800;1;1$\
					 6010;LP;1;500;1;50$\
					 6012;LP;1;500;1;25$\
					 6194;FP;0.5;100;0.5;3.5$\
					 6196;FP;0.5;100;0.5;3.5"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PS_RemovePatternRegionLocationAlgo(VARIANT* rgnId, VARIANT* pColBgn, VARIANT* pColEnd,VARIANT* pRowBgn, VARIANT* pRowEnd,VARIANT* pScaleSizeH,VARIANT* pScaleSizeV)
{
	//得到环形区域
	int id          = (int)rgnId->fltVal;
	int ColBgn      = (int)pColBgn->fltVal;
	int ColEnd      = (int)pColEnd->fltVal;
	int RowBgn      = (int)pRowBgn->fltVal;
	int RowEnd      = (int)pRowEnd->fltVal;
	float ScaleSizeH = (float)pScaleSizeH->fltVal;
	float ScaleSizeV = (float)pScaleSizeV->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();

	HTuple dArea,dRow,dCol;
	Hobject rgnPattern;
	rgnPattern = m_rgnPattern;
	area_center(rgnPattern,&dArea,&dRow,&dCol );
	if ( dArea <=  0)
	{
		Hobject newRgn;
		m_vRgn[id] = newRgn;
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
	
		Hobject Image;
		HTuple imgMappedHeight,imgMappedWidth;
		imgMappedHeight = m_mapimg_height;
		imgMappedWidth = m_mapimg_width;
		Image = m_ImageMapped;

		m_vInterImgs[id] = Image;

		HTuple  dRow1,dCol1,dRow2,dCol2;
		Hobject rgnPatternRect,rgnPatternCon,rgnPatternCls,rgnPatternSel;
		union1(rgnPattern,&rgnPattern );
		//connection(rgnPattern,&rgnPatternCon );
		closing_rectangle1(rgnPattern,&rgnPatternCls,20,10);
		//connection(rgnPatternCls,&rgnPatternCon );
		select_shape_std(rgnPatternCls,&rgnPatternSel,"max_area", 70);

		if (dRow1 < RowBgn )
		{
			HTuple tTemp;
			tTemp = RowBgn + 1;
			dRow1 = tTemp[0].I();
		}
		if ( (dRow2 + RowEnd + 1) >  imgMappedHeight)
		{
			HTuple tTemp;
			tTemp = imgMappedHeight - dRow2 -2;
			RowEnd = tTemp[0].I();
		}
		if ( ColEnd > imgMappedWidth)
		{
			HTuple tTemp;
			tTemp = imgMappedWidth -1;
			ColEnd = tTemp[0].I();

			if ( (imgMappedWidth - ColEnd - 3) < ColBgn)
			{
				HTuple tTemp;
				tTemp = imgMappedWidth - ColEnd -1;
				ColBgn = tTemp[0].I();
			}
		}
		smallest_rectangle1(rgnPatternSel, &dRow1, &dCol1, &dRow2, &dCol2);
		Hobject rgnRectDil;
		dilation_rectangle1(rgnPattern,&rgnRectDil, (Hlong)ScaleSizeH,(Hlong)ScaleSizeV);

		gen_rectangle1(&rgnPatternRect,(dRow1 - RowBgn) , ColBgn,(dRow2 + RowEnd),(imgMappedWidth - ColEnd) );

		Hobject rgnRemovePattern;
		difference(rgnPatternRect,rgnRectDil,&rgnRemovePattern );

		m_vRgn[id] = rgnRemovePattern ;	
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
/***************************************************************************************
**函数名：PS_TeethRegionLocationAlgo
**功  能：一种塑料瓶盖侧面防伪齿定位(检测算子)
**时  间：2019年11月15日
****************************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_TeethRegionDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6198$\
					 397;R;FP;0;10000;1;>=#1$\
					 381;B$\
					 658;LP;1;3;1;1$\
					 5563;LP;1;800;1;70$\
					 5564;LP;1;200;1;30$\
					 6179;FP;1;100;0.5;6$\
					 6010;LP;1;200;1;18$\
					 6012;LP;1;200;1;30$\
					 6197;FP;1;100;0.5;6$\
					 735;LP;1;100;1;10"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PS_TeethRegionDetectAlgo(VARIANT* rgnId, VARIANT* pChannelSelect, VARIANT* pRowBgn, VARIANT* pRgnHeight,VARIANT* pDarkThred,VARIANT* pTeethPosUp,VARIANT* pTeethPosDown, VARIANT*pLightThred ,VARIANT* pLightAreaLimit,VARIANT* pDetectArea)
{
	//得到环形区域
	int id                 = (int)rgnId->fltVal;
	int ChannelSelect      = (int)pChannelSelect->fltVal;           // 1
	int RowBgn             = (int)pRowBgn->fltVal;                  // 73
	int RgnHeight          = (int)pRgnHeight->fltVal;               // 30
	float DarkThred        = (float)pDarkThred->fltVal;               // 6
	int TeethPosUp         = (int)pTeethPosUp->fltVal;              // 18
	int TeethPosDown       = (int)pTeethPosDown->fltVal;            // 30
	int LightThred         = (int)pLightThred->fltVal;              // 5
    int LightAreaLimit     = (int)pLightAreaLimit->fltVal;          // 10

	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();  
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();

	Hobject rgn = m_vRgn[id];
	HTuple Area;
	region_features(rgn,"area",&Area );
	
	CComVariant retValue;
	if((Area <= 0) )
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		
		Hobject imgMappedSingle,ImageMapped;
		HTuple  ChannelNum;
		ImageMapped = m_ImageMapped;

		count_channels(ImageMapped,&ChannelNum);
		if (ChannelNum[0].I()==3)
		{
			access_channel(ImageMapped,&imgMappedSingle,ChannelSelect);
		}
		else
		{
			imgMappedSingle = m_crtImg;
		}

		Hobject ImageDomain,Image;
		Image = imgMappedSingle;
		get_domain(imgMappedSingle,&ImageDomain);

		m_vInterImgs[id]=imgMappedSingle;

		// step1 :search the black border of teeth
		Hobject rgnTeethSerch,imgTeethSerch,imgTeethSerchExpand,imgTeethSerch1 ;
		HTuple imgRWidth, imgRHeight;

		imgRWidth = m_mapimg_height;
		imgRHeight = m_mapimg_width;

		gen_rectangle1(&rgnTeethSerch,RowBgn, 0,RowBgn+RgnHeight,imgRWidth-1);
		reduce_domain(Image, rgnTeethSerch, &imgTeethSerch);
		expand_domain_gray(imgTeethSerch, &imgTeethSerchExpand, 25);
		//scale_image_max (imgTeethSerchExpand, ImageScaleMax)
		reduce_domain(imgTeethSerchExpand, rgnTeethSerch, &imgTeethSerch1);

		Hobject imgTeethSerchMean,rgnThrTeeth,rgnFilTeeth,rgnOpnTeeth,rgnClsTeeth,rgnConTeeth,rgnSelTeeth,rgnBorderUp ;
		mean_image(imgTeethSerch1, &imgTeethSerchMean, 1, 50);
		dyn_threshold(imgTeethSerch1, imgTeethSerchMean, &rgnThrTeeth,DarkThred, "dark");
		fill_up(rgnThrTeeth, &rgnFilTeeth);
		opening_circle(rgnFilTeeth, &rgnOpnTeeth, 2.5);
		closing_rectangle1(rgnOpnTeeth, &rgnClsTeeth, 20, 1);
		connection(rgnClsTeeth, &rgnConTeeth);
		select_shape_std(rgnConTeeth, &rgnSelTeeth, "max_area", 70);
		rgnBorderUp = rgnSelTeeth;

		//concat_obj(m_rgnBorderUp, rgnSelTeeth, &m_rgnBBorderUp);
		Hobject rgnRectTeeth,rgnTeethSerchMovedDown,rgnTeethSerchMovedUp,rgnTeeth;
		HTuple Row12,Column11,Row22,Column21,dAreaOri,dRowOri,dColOri;
		shape_trans(rgnSelTeeth, &rgnRectTeeth, "rectangle1");
		smallest_rectangle1(rgnRectTeeth, &Row12, &Column11, &Row22, &Column21);
		move_region(rgnTeethSerch, &rgnTeethSerchMovedDown,TeethPosDown, 0);
		move_region(rgnTeethSerch, &rgnTeethSerchMovedUp,TeethPosUp, 0);
		union2(rgnTeethSerchMovedUp, rgnTeethSerchMovedDown, &rgnTeeth);
		closing_rectangle1(rgnTeeth, &rgnTeeth, 1, 20);

		clip_region(rgnTeeth, &rgnTeeth, 0,Column11,imgRHeight-1,Column21);
		area_center(rgnTeeth, &dAreaOri, &dRowOri, &dColOri);

		m_rgnTeeth = rgnTeeth;

		if(m_bDebugImg)
		{
			concat_obj(m_vRgn[id], rgnTeethSerch, &m_vRgn[id]);
			concat_obj(m_vRgnEx[id], rgnTeeth, &m_vRgnEx[id]);
		}

		//Step2:White Region Location
		Hobject imgTeeth,ImageMean,rgnTeethThrDetect ;
		reduce_domain(Image, rgnTeeth, &imgTeeth);
		mean_image(imgTeeth, &ImageMean, 40, 30);
		dyn_threshold(imgTeeth, ImageMean, &rgnTeethThrDetect,LightThred, "light");
		fill_up(rgnTeethThrDetect, &rgnTeethThrDetect);
		closing_rectangle1(rgnTeethThrDetect, &rgnTeethThrDetect, 3, 3);

		Hobject  rgnTeethOpnDetect,rgnTeethConDetect,rgnTeethSelDetect,rgnTeethEro,rgnTeethSelIntsct;
		opening_circle(rgnTeethThrDetect, &rgnTeethOpnDetect, 1.5);
		connection(rgnTeethOpnDetect, &rgnTeethConDetect);
		select_shape(rgnTeethConDetect, &rgnTeethSelDetect, "width", "and",LightAreaLimit, 99999);

		erosion_rectangle1(rgnTeeth, &rgnTeethEro, 15, 10);
		intersection(rgnTeethEro, rgnTeethSelDetect, &rgnTeethSelIntsct);
		connection(rgnTeethSelIntsct, &rgnTeethSelIntsct);
		select_shape(rgnTeethSelIntsct, &rgnTeethSelIntsct, "area", "and", LightAreaLimit, 99999);
		union1(rgnTeethSelIntsct, &rgnTeethSelDetect);

		HTuple dAreaDetect, dRowDetect,dColDetect,dMean,dDeviation;
		area_center(rgnTeethSelDetect, &dAreaDetect, &dRowDetect, &dColDetect);
		intensity(rgnTeethSelDetect, Image, &dMean, &dDeviation);
	
		 //Step 3:Teeth 下边缘黑色区域定位
		Hobject rgnTeethDown,imgTeethDown,imgTeethDownExp, imgTeethDown1;
		move_region(rgnTeeth, &rgnTeethDown,25, 0);
	
		reduce_domain(Image, rgnTeethDown, &imgTeethDown);
		expand_domain_gray(imgTeethDown, &imgTeethDownExp, 25);
		reduce_domain(imgTeethDownExp, rgnTeethDown, &imgTeethDown1);

		Hobject  imgTeethDown1Mean,rgnDownDarkOri,rgnDownDarkOriFil,rgnDownDarkOriOpn,rgnDownDarkOriCls;
		mean_image(imgTeethDown1, &imgTeethDown1Mean, 1, 20);
		dyn_threshold(imgTeethDown1, imgTeethDown1Mean, &rgnDownDarkOri,DarkThred, "dark");
		
		Hobject rgnDownDarkOriCon,rgnDownDark ;
		fill_up(rgnDownDarkOri, &rgnDownDarkOriFil);
		opening_circle(rgnDownDarkOriFil, &rgnDownDarkOriOpn, 2.5);
		closing_rectangle1(rgnDownDarkOriOpn, &rgnDownDarkOriCls, 20, 1);
		connection(rgnDownDarkOriCls, &rgnDownDarkOriCon);
		select_shape_std(rgnDownDarkOriCon, &rgnDownDark, "max_area", 70);

		//* 剔除下边缘黑色区域
		Hobject rgnBBorderDTrans,rgnBBorderDTransDil ;
		shape_trans(rgnDownDark, &rgnBBorderDTrans, "rectangle1");
		dilation_rectangle1(rgnBBorderDTrans, &rgnBBorderDTransDil, 50, 1);
		difference(rgnTeethSelDetect, rgnBBorderDTransDil, &rgnTeethSelDetect);

		//* 剔除上边缘黑色区域 rgnBorderUp
		Hobject rgnBorderUpDil,rgnTeethWhiteCon,rgnTeethWhiteSel;
		HTuple iNum ;
		dilation_rectangle1(rgnBorderUp, &rgnBorderUpDil, 1, 1);
		difference(rgnTeethSelDetect, rgnBorderUp, &rgnTeethSelDetect);

		connection(rgnTeethSelDetect, &rgnTeethWhiteCon);
		select_shape(rgnTeethWhiteCon, &rgnTeethWhiteSel, "area", "and", LightAreaLimit, 99999);
		count_obj(rgnTeethWhiteSel, &iNum);

		Hobject rgnTeethWhiteFinal ;
		gen_empty_obj(&rgnTeethWhiteFinal);
		if (iNum>1)
		{			
			for (int i=1;i <= iNum;i ++)
			{
				Hobject objBlackSel;
				HTuple Ra, Rb, Phi,dDeg;
				select_obj(rgnTeethWhiteSel, &objBlackSel,i);
				elliptic_axis(objBlackSel, &Ra, &Rb, &Phi);
				dDeg = (Phi.Deg()).Abs();
				if ((dDeg<70) || (dDeg>120))
				{
					concat_obj(rgnTeethWhiteFinal, objBlackSel, &rgnTeethWhiteFinal);
				}
			}
			
		}
		else
		{
			rgnTeethWhiteFinal = rgnTeethWhiteSel;

		}
		Hobject  rgnTeethWhiteUni,rgnTeethWhiteRect;
		union1(rgnTeethWhiteFinal, &rgnTeethWhiteUni);
		shape_trans(rgnTeethWhiteUni, &rgnTeethWhiteRect, "rectangle1");
		m_rgnTeethWhiteRect = rgnTeethWhiteRect;

		if(m_bDebugImg)
		{
			concat_obj(m_vRgn[id], rgnTeethWhiteRect, &m_vRgn[id]);
			concat_obj(m_vRgnEx[id], rgnTeethWhiteFinal, &m_vRgnEx[id]);
		}
		else
		{
			m_vRgn[id] = rgnTeethWhiteRect ;	
		}

		// 输出信息
		HTuple dArea ;
		region_features(rgnTeethWhiteRect,"area",&dArea );
		if ( dArea > 0)
		{
			retValue= dArea[0].I();
			Hobject rgnTeethWhiteRectEro;
			erosion_circle(rgnTeethWhiteRect,&rgnTeethWhiteRectEro,2);
			m_vErrorRgn[id] = rgnTeethWhiteRectEro;
			retValue.Detach(pDetectArea);
		} 
		else
		{
			retValue= 1;
			m_vRgnExTest[id] = rgnTeethWhiteRect;
			retValue.Detach(pDetectArea);
		}
	
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
/***************************************************************************************
**函数名：PS_TeethRegionLocationAlgo
**功  能：一种塑料瓶盖侧面防伪齿区域
**时  间：2019年11月29日
****************************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_TeethRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRegionPos,VARIANT* pUpSize,VARIANT* pDownSize, VARIANT* pScaleSize)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	int RegionPos = (int)pRegionPos->fltVal;
	int UpSize = (int)pUpSize->fltVal;
	int DownSize =(int)pDownSize->fltVal;
	float ScaleSize =(float)pScaleSize->fltVal;

	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();

	Hobject newRgn;
	m_vRgn[id] = newRgn;

	m_vInterImgs[id] = m_ImageMapped;

	Hobject rgnTeethWhiteRect;
	HTuple  dArea ;
	rgnTeethWhiteRect = m_rgnTeethWhiteRect;
	region_features(rgnTeethWhiteRect,"area",&dArea);
	if(dArea <= 0)
	{
		m_vRgn[id] = newRgn;
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

		Hobject  rgnMoveUp,rgnMoveDown,rgnIntersct,rgnEro;

		move_region(rgnTeethWhiteRect,&rgnTeethWhiteRect,RegionPos,0 );

		move_region(rgnTeethWhiteRect,&rgnMoveUp,- DownSize,0 );
		move_region(rgnTeethWhiteRect,&rgnMoveDown,UpSize,0 );
		intersection(rgnMoveUp,rgnMoveDown,&rgnIntersct );
		erosion_rectangle1(rgnIntersct,&rgnEro,(Hlong)ScaleSize,(Hlong)ScaleSize );

		HTuple dArea1 ;
		region_features(rgnEro,"area",&dArea1);
		if(dArea1 <= 0)
		{
			m_vRgn[id] = newRgn;
			return S_FALSE;
		}
		else
		{
			m_vRgn[id] = rgnEro;
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
STDMETHODIMP CBottleCapAlgo::PS_TeethRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6209$\
					 167;B$\
					 6224;LP;-50;50;1;0$\
					 6010;LP;0;30;1;0$\
					 6012;LP;0;30;1;0$\
					 1376;FP;0.5;30;1.0;1.0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/***************************************************************************************
**函数名：PS_TeethOffsetDetectAlgo
**功  能：一种塑料瓶盖侧面防伪齿缺陷检测
**时  间：2019年11月18日
****************************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_TeethOffsetDetectAlgo(VARIANT* rgnId,VARIANT* pChannelSelect,VARIANT* pTeethWidth,VARIANT* pOffset, VARIANT* pDetectArea)
{
	int rId           = (int)rgnId->fltVal;
	int ChannelSelect = (int)pChannelSelect->fltVal;
	int TeethWidth    = (int)pTeethWidth->fltVal;
	int Offset        = (int)pOffset->fltVal;
	//
	Hobject newRgn;

	m_vInterImgs[rId].Reset();
	m_vRgnEx[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();

	Hobject rgnTeeth,rgnTeethWhiteRect ;
	rgnTeeth = m_rgnTeeth ;
	rgnTeethWhiteRect = m_rgnTeethWhiteRect;

	HTuple dArea1,dArea2;
	region_features(rgnTeeth,"area",&dArea1);
	region_features(rgnTeethWhiteRect,"area",&dArea2);

	CComVariant retValue;
	if((dArea1 <= 0) /*|| (dArea2 <= 0)*/)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	// 如果防伪齿宽度太小则不进行检测
	HTuple iTeethWidth,iTeethWhiteRectWidth;
	region_features(rgnTeeth,"width",&iTeethWidth);
	region_features(rgnTeethWhiteRect,"width",&iTeethWhiteRectWidth);
	if ( (iTeethWidth < TeethWidth) || ( iTeethWhiteRectWidth < TeethWidth))
	{
		retValue = 0;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		m_vInterImgs[rId] = m_ImageMapped;
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

		Hobject imgMappedSingle,ImageMapped;
		HTuple  ChannelNum;
		ImageMapped = m_ImageMapped;

		count_channels(ImageMapped,&ChannelNum);
		if (ChannelNum[0].I()==3)
		{
			access_channel(ImageMapped,&imgMappedSingle,ChannelSelect);
		}
		else
		{
			imgMappedSingle = m_ImageMapped;
		}

		Hobject ImageDomain,Image;
		Image = imgMappedSingle;
		get_domain(imgMappedSingle,&ImageDomain);

		m_vInterImgs[rId]=imgMappedSingle;

		//* 判断是否齿偏
		Hobject rgnRectOpenPatternDil,rgnRectDiff,rgnRectDiffCon ;
		HTuple dWidthS;
		dilation_rectangle1(rgnTeethWhiteRect, &rgnRectOpenPatternDil, 1, 100);
		difference(rgnTeeth, rgnRectOpenPatternDil, &rgnRectDiff);
		connection(rgnRectDiff, &rgnRectDiffCon);
		region_features(rgnRectDiffCon, "width", &dWidthS);
		
		HTuple dWidthMax,Indice,dArea ;
		Hobject rgnTeethOffset ;
		if ((dWidthS.Num())>0)
		{
			tuple_max(dWidthS, &dWidthMax);
			tuple_find(dWidthS,dWidthMax, &Indice);
			select_obj(rgnRectDiffCon, &rgnTeethOffset,Indice+1);
			region_features(rgnTeethOffset,"area",&dArea );
		}
		else
		{
			dWidthMax = HTuple();
			gen_empty_region(&rgnTeethOffset);
		}
		
		if ( dWidthMax > Offset)
		{
			retValue= dArea[0].I();
			Hobject rgnTeethOffsetEro;
			erosion_circle(rgnTeethOffset,&rgnTeethOffsetEro,2);
			m_vErrorRgn[rId] = rgnTeethOffsetEro;
			retValue.Detach(pDetectArea);
		} 
		else
		{
			retValue= 0;
			m_vRgnExTest[rId] = rgnTeethOffset;
			retValue.Detach(pDetectArea);
		}

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
STDMETHODIMP CBottleCapAlgo::PS_TeethOffsetDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6199$\
					 397;R;FP;0;10000;1;>=#1$\
					 381;B$\
					 658;LP;1;3;1;1$\
					 6208;LP;1;100;1;10$\
					 6200;LP;1;100;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/******************************************************************************
** 函数名：PS_DeformableModelDetectAlgo
** 功  能：通过变形模板匹配图案区域，检测图案上的缺失位置
** 说  明：该算子匹配失败时，不报错
** 时  间：2019年11月21日
*******************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_DeformableModelDetectAlgo(VARIANT* rgnId,VARIANT* pChannelSelect,VARIANT* pMatchScore, VARIANT* pSmoothSize,VARIANT* pThreshold,VARIANT* pAreaSize,VARIANT* pDetectArea)
{
	int rId                 = (int)rgnId->fltVal;
	int ChannelSelect       = (int)pChannelSelect->fltVal;
	float MatchScore        = (float)pMatchScore->fltVal;	
	int SmoothSize        = (int)pSmoothSize->fltVal;
	int Threshold           = (int)pThreshold->fltVal;       //  60 区分匹配到的轮廓中的小区域属于白区域还是黑区域
	int AreaSize            = (int)pAreaSize->fltVal;        //  50
	//
	Hobject newRgn;
	Hobject rgn = m_vRgn[rId];

	m_vInterImgs[rId].Reset();
	m_vRgnEx[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();

	HTuple dArea1,dArea2;
	region_features(rgn,"area",&dArea1);

	CComVariant retValue;
	// 区域为空报错
	if((dArea1 <= 0) )
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	// 该算子匹配不上时，不报错

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

		Hobject imgMappedSingle,ImageMapped;
		HTuple  ChannelNum,WidthMap;
		ImageMapped = m_ImageMapped;
		WidthMap = m_mapimg_width;

		count_channels(ImageMapped,&ChannelNum);
		if (ChannelNum[0].I()==3)
		{
			access_channel(ImageMapped,&imgMappedSingle,ChannelSelect);
		}
		else
		{
			imgMappedSingle = m_ImageMapped;
		}

		Hobject ImageDomain,Image;
		Image = imgMappedSingle;
		get_domain(imgMappedSingle,&ImageDomain);

		m_vInterImgs[rId]=imgMappedSingle;

		// 读取模板文件
		//标定文件路径
		string ModelIDWholeFile = m_strTaskPathWithoutCameraIndex;
		string ModelIDSmlFile = m_strTaskPathWithoutCameraIndex;
		string ModelIDBigFile = m_strTaskPathWithoutCameraIndex;
		string ModelIDBigLessFile = m_strTaskPathWithoutCameraIndex;

		//long t1 = GetTickCount();

		ModelIDWholeFile.append("\\");
		ModelIDWholeFile.append("ModelIDWhole");
		ModelIDWholeFile.append(".dfm");

		ModelIDSmlFile.append("\\");
		ModelIDSmlFile.append("ModelIDSml");
		ModelIDSmlFile.append(".dfm");

		ModelIDBigFile.append("\\");
		ModelIDBigFile.append("ModelIDBig");
		ModelIDBigFile.append(".dfm");

		ModelIDBigLessFile.append("\\");
		ModelIDBigLessFile.append("ModelIDBigLess");
		ModelIDBigLessFile.append(".dfm");

		HTuple ModelIDWhole,ModelIDSml,ModelIDBig,ModelIDBigLess ;
        
		if(_access(ModelIDWholeFile.c_str(),0)||_access(ModelIDSmlFile.c_str(),0)||_access(ModelIDBigFile.c_str(),0) || _access(ModelIDBigLessFile.c_str(),0))
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			m_vErrorRgn[rId].Reset();
			return S_FALSE;
		}
		read_deformable_model(ModelIDWholeFile.c_str(), &ModelIDWhole);
		read_deformable_model(ModelIDSmlFile.c_str(), &ModelIDSml);
		read_deformable_model(ModelIDBigFile.c_str(), &ModelIDBig);
		read_deformable_model(ModelIDBigLessFile.c_str(), &ModelIDBigLess);

		//long t2 = GetTickCount();

		// 对区域进行匹配
		Hobject rgnCon,rgnDil,rgnUni,rgnUniCon;
		connection(rgn, &rgnCon);
		dilation_circle(rgnCon,&rgnDil,10.5 );
		union1(rgnDil,&rgnUni );
		connection(rgnUni,&rgnUniCon );

		HTuple iNumRect ;
		count_obj(rgnUniCon, &iNumRect);

		Hobject DeformedContours ;
		gen_empty_obj(&DeformedContours);

		for (int i=1; i <= iNumRect; i++)
		{
			Hobject objSel,Rectangle ;
			HTuple dRowSel,dColSel,dPhiSel,dLen1Sel,dLen2Sel ;
			select_obj(rgnUniCon, &objSel, i);
			smallest_rectangle2(objSel, &dRowSel, &dColSel, &dPhiSel, &dLen1Sel, &dLen2Sel);
			gen_rectangle2(&Rectangle, dRowSel, dColSel, dPhiSel, dLen1Sel, dLen2Sel);

			// 一定要将DeformedContoursTemp初始化为contour
			Hobject imgRectPatternR,DeformedContoursTemp;
			gen_empty_obj(&DeformedContoursTemp );
			gen_contour_region_xld(DeformedContoursTemp, &DeformedContoursTemp, "border");
			
			reduce_domain(Image, Rectangle, &imgRectPatternR);
			
			HTuple Smoothness = (int)SmoothSize;
			if ((dLen2Sel*2)>125)
			{
					//* whole match
					HTuple ScoreWhole, Row,Column,Score;
					Hobject ImageRectified,VectorField ,DeformedContoursBig,DeformedContoursBigLess,DeformedContoursSml;
					Hobject rgnDeformedContours,rgnDeformedContoursBigLess,rgnDeformedContoursSml;
					find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField, &DeformedContoursTemp, ModelIDWhole, 0, 0, 1, 1, 1, 1, 0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), ((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")), \
												  Smoothness.Concat((HTuple(0).Append(1))), &ScoreWhole, &Row, &Column);
					if (ScoreWhole<MatchScore)
					{
					  find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField, &DeformedContoursBig, ModelIDBig, 0, 0, 1, 1, 1, 1, 0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
						  ((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")), Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row, &Column);
					  concat_obj(DeformedContours, DeformedContoursBig, &DeformedContours);
	
					}
					/*if (ScoreWhole<MatchScore)
					{
					find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField, &DeformedContoursBigLess, ModelIDBigLess, 0, 0, 1, 1, 1, 1, 0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
					((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")),  Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row, &Column);
					concat_obj(DeformedContours, DeformedContoursBigLess, &DeformedContours );
					}*/
					if (ScoreWhole<MatchScore)
					{
					  find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField, &DeformedContoursSml, ModelIDSml, 0, 0, 1, 1, 1, 1, 0.5, 1, 1,  4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
						  ((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")), Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row, &Column);
					  concat_obj(DeformedContours, DeformedContoursSml, &DeformedContours);			
					}
				
			  }
			  else
			  {

				if (dColSel<=(WidthMap/2.0))
				{
					  Hobject ImageRectified,VectorField ;
					  HTuple ScoreWhole, Row,Column,Score;
					  if ( (dLen2Sel*2)<=125 && (dLen2Sel*2)>75)
					  {
						//big match
						find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField, &DeformedContoursTemp, ModelIDBig, 0, 0, 1, 1, 1, 1, 0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
							((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")), (HTuple)Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row, &Column);
						if (Score<MatchScore)
						{
						  find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField, &DeformedContoursTemp, ModelIDBigLess, 0,  0, 1, 1, 1, 1, 0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
							  ((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")), (HTuple)Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row, &Column);
						}

					  }
					  else if ( ((dLen2Sel*2)<=75) && ((dLen2Sel*2)>55))
					  {
						//big less match
						Hobject ImageRectified,VectorField ;
						find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField, &DeformedContoursTemp, ModelIDBigLess, 0, 0, 1, 1, 1, 1, 0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
												   ((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")), (HTuple)Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row, &Column);
					  }
				  }
				  else
				  {
					      Hobject ImageRectified,VectorField ,DeformedContoursBig,DeformedContoursBigLess,DeformedContoursSml;
						  HTuple ScoreWhole, Row,Column,Score;
						  if (((dLen2Sel*2)<=125) && ((dLen2Sel*2)>75))
						  {
							  //bigless and sml  match
							  find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField,  &DeformedContoursBigLess, ModelIDBigLess, 0, 0, 1, 1, 1, 1,  0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
								  ((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")), (HTuple)Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row, &Column);
							  find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField,  &DeformedContoursSml, ModelIDSml, 0, 0, 1, 1, 1, 1, 0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
								  ((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")),  (HTuple)Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row,  &Column);
							  concat_obj(DeformedContours, DeformedContoursBigLess, &DeformedContours );
							  concat_obj(DeformedContours, DeformedContoursSml, &DeformedContours );

						  }
						  else if (((dLen2Sel*2)<=75)&&((dLen2Sel*2)>30))
						  {
							  //sml match
							  find_local_deformable_model(imgRectPatternR, &ImageRectified, &VectorField,  &DeformedContoursTemp, ModelIDSml, 0, 0, 1, 1, 1, 1, 0.5, 1, 1, 4, 0.9, ((HTuple("image_rectified").Append("vector_field")).Append("deformed_contours")), 
								  ((HTuple("deformation_smoothness").Append("expand_border")).Append("subpixel")),  (HTuple)Smoothness.Concat((HTuple(0).Append(1))), &Score, &Row, &Column);
						  }

					}

				}

				concat_obj(DeformedContours, DeformedContoursTemp, &DeformedContours);

		}

	/*	long t3 = GetTickCount();
		long time1 = t3 - t2;
		long time2 = t2 - t1;*/

		// 清除模板文件
		clear_deformable_model(ModelIDWhole);
		clear_deformable_model( ModelIDSml);
		clear_deformable_model( ModelIDBig);
		clear_deformable_model(ModelIDBigLess);

		HTuple Number1 ;
		Hobject UnionContours,UnionContours1;
		count_obj(DeformedContours, &Number1);
		if (Number1< 1 )
		{
			// 匹配不上时，不报错
			retValue= 0;
			m_vErrorRgn[rId] = newRgn;
			retValue.Detach(pDetectArea);
			return S_OK;
		}
		
		//write_object(DeformedContours,"C:\\DeformedContours.hobj" );
		union_collinear_contours_xld(DeformedContours, &DeformedContours, 10, 1, 2, 0.1, "attr_keep");
		union_adjacent_contours_xld(DeformedContours, &UnionContours1, 10, 1, "attr_keep");

		Hobject  rgnUnionContours1,rgnUnionContours2;
		HTuple  dMeanAve,dDeviaAve;
		gen_region_contour_xld(UnionContours1, &rgnUnionContours1, "filled");
		union1(rgnUnionContours1,&rgnUnionContours2 );
		intensity(rgnUnionContours2, Image, &dMeanAve, &dDeviaAve);
		m_vRgnExTest[rId] =  rgnUnionContours1;

		Hobject rgnBlack,rgnWhite  ; 
		gen_empty_region(&rgnBlack);
		gen_empty_region(&rgnWhite);

		HTuple Number2 ;
		count_obj(UnionContours1, &Number2);
		{
			for (int i=1; i <= Number2; i ++)
			{
				Hobject xldSel,rgnSelTemp ;
				HTuple dRowXld, dColXld,dAreaTemp,dRowTemp,dColTemp ;
				select_obj(UnionContours1, &xldSel, i);
				get_contour_xld(xldSel, &dRowXld, &dColXld);

				HTuple dMeanTemp,dDeviaTemp ;
				gen_region_contour_xld(xldSel, &rgnSelTemp, "filled");
				area_center(rgnSelTemp, &dAreaTemp, &dRowTemp, &dColTemp);
				intensity(rgnSelTemp, Image, &dMeanTemp, &dDeviaTemp);
				if ( dMeanTemp> dMeanAve + Threshold )
				{
					union2(rgnWhite, rgnSelTemp, &rgnWhite);
				}
				else
				{
					union2(rgnBlack, rgnSelTemp, &rgnBlack);
				}

			}
		}

		Hobject rgnDiffBlaWhi,imgDiffBlaWhi,rgnPatternError ;
		difference(rgnBlack, rgnWhite, &rgnDiffBlaWhi);
		reduce_domain(Image, rgnDiffBlaWhi, &imgDiffBlaWhi);

		threshold(imgDiffBlaWhi, &rgnPatternError, dMeanAve + Threshold, 255);
		opening_circle(rgnPatternError, &rgnPatternError, 1.5);
		connection(rgnPatternError, &rgnPatternError);
		select_shape(rgnPatternError, &rgnPatternError, "area", "and", AreaSize, 9999999);

		HTuple dArea,dRow,dCol;
		area_center(rgnPatternError,&dArea,&dRow,&dCol);

		if ( dArea > 0)
		{
			retValue= dArea[0].I();
			Hobject rgnPatternErrorDil;
			dilation_circle(rgnPatternError,&rgnPatternErrorDil,2);
			m_vErrorRgn[rId] = rgnPatternErrorDil;
			retValue.Detach(pDetectArea);
		} 
		else
		{
			retValue= 0;
			m_vErrorRgn[rId] = newRgn;
			retValue.Detach(pDetectArea);
		}

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
STDMETHODIMP CBottleCapAlgo::PS_DeformableModelDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6210$\
					 397;R;FP;0;10000;1;>=#1$\
					 381;B$\
					 658;LP;1;3;1;1$\
					 654;FP;0.5;1.0;0.5;0.5$\
					 733;LP;3;100;1;21$\
					 6211;LP;1;250;1;10$\
					 6005;LP;1;1000;1;50");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/******************************************************************************
** 函数名：CurlNoCompoundForClolorNewAlgo
** 功  能：检测防伪齿区域的平均灰度值以确定防伪齿是否偏了
** 时  间：2019年11月25日
*******************************************************************************/
STDMETHODIMP CBottleCapAlgo::CurlNoCompoundForClolorNewAlgo(VARIANT* rgnId,VARIANT* pChannelSelect,VARIANT*pGrayValueMean )
{
	int id = (int)rgnId->fltVal;
	int ChannelSelect       = (int)pChannelSelect->fltVal;

	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	if (m_bPilotImg)
	{
		return S_OK;
	}

	Hobject rgn = m_vRgn[id];
	Hobject rgnTeethWhiteRect = m_rgnTeethWhiteRect;
	m_vRgnExTest[id] = rgnTeethWhiteRect;
	HTuple area1,row1,col1;
	area_center(rgnTeethWhiteRect,&area1,&row1,&col1);
	CComVariant retValue;

	// 防伪齿区域为空时(不报错)
	if(area1< 0) 
	{
		retValue=0;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		m_vInterImgs[id] = m_ImageMapped;
		retValue.Detach(pGrayValueMean);
		return S_OK;
	}

	Hobject RegionDilat;
	HTuple MeanGray, Deviation;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		
		///******************************************
		Hobject imgMappedSingle,ImageMapped;
		HTuple  ChannelNum,WidthMap;
		ImageMapped = m_ImageMapped;
		WidthMap = m_mapimg_width;

		count_channels(ImageMapped,&ChannelNum);
		if (ChannelNum[0].I()==3)
		{
			access_channel(ImageMapped,&imgMappedSingle,ChannelSelect);
		}
		else
		{
			imgMappedSingle = m_ImageMapped;
		}

		Hobject ImageDomain,Image;
		Image = imgMappedSingle;
		get_domain(imgMappedSingle,&ImageDomain);
		m_vInterImgs[id]=imgMappedSingle;
		//********************************************

		intensity (rgnTeethWhiteRect, Image, &MeanGray, &Deviation);

		retValue = MeanGray[0].I();
		dilation_circle (rgnTeethWhiteRect, &RegionDilat, 3.5);
		m_vErrorRgn[id] = RegionDilat;
		retValue.Detach(pGrayValueMean);

#ifdef NDEBUG 
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue =-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::CurlNoCompoundForClolorNewAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6201$\
					 200;R;LP;1;255;1;<=#20$\
					 167;B$\
					 658;LP;1;3;1;1");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/******************************************************************************
** 函数名：InWallDynThresholdAlgoNew
** 功  能：滤波方向：垂直；使用场景：侧面矫正之后的图片
** 时  间：2019年11月29日
*******************************************************************************/
STDMETHODIMP CBottleCapAlgo::SideWallDynThresholdAlgoNew(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	//侧面内壁区域的动态灰度算法检测、纵向平滑
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vRgnEx[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	m_vInterImgs[rId].Reset();

	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		//为空不报错，用于特殊区域检测
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
		Hobject Image;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_ImageMapped,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[rId]=Image;
		Hobject ImageReduced, ImageReduceMean,ExpandedImage,ImageDetectNew;
		Hobject BlackUnion, WhiteUnion;

		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//预处理
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(Image, &Image, MaskSize, MaskSize);

		ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		expand_domain_gray (ImageReduced, &ExpandedImage, 25);
		reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
		mean_image(ImageDetectNew, &ImageReduceMean, 1, BlackMaskSize);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		if(abs(WhiteMaskSize-BlackMaskSize)>=1)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
			expand_domain_gray (ImageReduced, &ExpandedImage, 25);
			reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
			mean_image(ImageDetectNew, &ImageReduceMean, 1, WhiteMaskSize);
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
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::SideWallDynThresholdAlgoNewHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6207$\
					 397;R;LP;0;1000;1$\
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
/******************************************************************************
** 函数名：InWallDynThresholdAlgoNew
** 功  能：滤波方向：垂直；使用场景：侧面为矫正的图片
** 时  间：2019年12月02日
*******************************************************************************/
STDMETHODIMP CBottleCapAlgo::SideWallDynThresholdAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	//侧面内壁区域的动态灰度算法检测、纵向平滑
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vRgnEx[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	m_vInterImgs[rId].Reset();

	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;

	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		//为空不报错，用于特殊区域检测
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
		Hobject Image;
		//按要求变换彩色空间，并提取单通道图像。
		if(!H_ColorImageTrans(m_crtImgMult,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[rId]=Image;
		Hobject ImageReduced, ImageReduceMean,ExpandedImage,ImageDetectNew;
		Hobject BlackUnion, WhiteUnion;

		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//预处理
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(Image, &Image, MaskSize, MaskSize);

		ImageProcessAlg.MeanImageReduced(Image,rgn, BlackMaskSize, BlackMaskSize,1,ImageReduced, ImageReduceMean);
		expand_domain_gray (ImageReduced, &ExpandedImage, 25);
		reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
		mean_image(ImageDetectNew, &ImageReduceMean, 1, BlackMaskSize);
		ImageProcessAlg.ErrorDetect(ImageReduced,ImageReduceMean , SeriousBlackPointDynThresh, SeriousBlackPointSize, BlackUnion,1);
		area_center(BlackUnion, &blackArea, &rowBlack, &colBlack);
		//
		if(abs(WhiteMaskSize-BlackMaskSize)>=1)
		{
			ImageProcessAlg.MeanImageReduced(Image,rgn, WhiteMaskSize, WhiteMaskSize,1,ImageReduced, ImageReduceMean);
			expand_domain_gray (ImageReduced, &ExpandedImage, 25);
			reduce_domain (ExpandedImage, rgn, &ImageDetectNew); 
			mean_image(ImageDetectNew, &ImageReduceMean, 1, WhiteMaskSize);
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
	catch (...) 
	{
		m_algoSpy.QueueUpLogMsg(__FUNCTION__, m_crtImg);
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::SideWallDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6207$\
					 397;R;LP;0;1000;1$\
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
/**************************************************************************
** 函数名：N_BubbleDetectProfileAlgoNew
** 功  能：用于侧面展开图中（水平滤波、区域为空不报错）检测齿偏、断齿缺陷
***************************************************************************/
STDMETHODIMP CBottleCapAlgo::PS_TeethDisappearDetectAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pAreaSize, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	int AreaSize = (int)pAreaSize->fltVal;
	m_vWhiteErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vInterImgs[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	HTuple area,row,col;
	union1(rgn,&rgn);
	area_center(rgn,&area,&row,&col);
	// 防伪齿面积小于面积阈值时不报错
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area<AreaSize)
	{
		retValue = 10000;
		m_vInterImgs[rId] = m_ImageMapped;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_OK;
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
		if(!H_ColorImageTrans(m_ImageMapped,&Image,rgn,   ColorTransType, ChannelSelect))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		m_vInterImgs[rId]=Image;
		Hobject ImageReduced, ImageReduceMean,ExpandedImage,ImageDetectNew;
		Hobject BlackUnion, WhiteUnion;
		HTuple blackArea, whiteArea, rowBlack, colBlack, rowWhite, colWhite;
		//
		//预处理
		HTuple MaskSize;
		MaskSize = 3;
		mean_image(Image, &Image, MaskSize, MaskSize);
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
		select_shape(SeriousConnected, &SeriousPointReg, "area", "and", SeriousBlackPointSize, 9999999);
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
		select_shape(SeriousConnected, &SeriousPointReg, "area", "and", SeriousWhitePointSize, 9999999);
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
			retValue =-1;
			Hobject  rgnDil;
			dilation_circle (rgn, &rgnDil, 3.5);
			m_vErrorRgn[rId] = rgnDil;
		}
		else
		{
			if (m_bDebugImg)
			{
				Hobject  rgnDil;
				dilation_circle (rgn, &rgnDil, 3.5);
				m_vErrorRgn[rId] = rgnDil;

				m_vRgnExTest[rId] = DefectRegs;
			}
			else
			{
				HTuple DefectArea,DefectRow,DefectCol;
				area_center(DefectRegs,&DefectArea,&DefectRow,&DefectCol);
				Hobject DefectCircle;
				gen_circle(&DefectCircle,DefectRow,DefectCol,10);
				m_vErrorRgn[rId] = DefectCircle;
			}
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
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBottleCapAlgo::PS_TeethDisappearDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("6225$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 657;LP;0;6;1;1$\
					 658;LP;1;3;1;1$\
					 6226;LP;3;2000;2;350$\
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















