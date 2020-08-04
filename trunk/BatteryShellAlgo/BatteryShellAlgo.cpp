#include "stdafx.h"
#include "resource.h"
#include "BatteryShellAlgo_i.h"
#include "dllmain.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "BatteryShellAlgo.h"
#include "../ImageProcessLib/ImageProcessLib.h"

#include <ppl.h>
#include <fstream>
#include <concurrent_vector.h>

using namespace std;
using namespace Halcon;
using namespace Concurrency;

using namespace ColorImage;
#ifndef   DLLEXPT
#define DLLEXPT __declspec (dllexport )
#endif
//
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

STDMETHODIMP CBatteryShellAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IBatteryShellAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP CBatteryShellAlgo::SetCurrentImage(LONG* pImg,BYTE crtImgIdentify,LONG* pImgCorrect)
{
	m_crtImgIdentify = crtImgIdentify;
	m_imageNumber++;
	m_vROI.clear();
	m_vInterImgs.clear();
	Hobject EmptyObj;
	m_originImageMean = EmptyObj;
	m_vInterImgs.resize(128);
	m_vRgnInterference = EmptyObj;
	Hobject* pObj = (Hobject*)pImg;
	m_crtImgMult  = *pObj;// 原始图片，可能是多通道图片（彩色图片）
	m_bDebugImg   = ImageProcessAlg.IsDebugImage(m_crtImgMult);	
	m_bPilotImg	  = ImageProcessAlg.IsPilotImage(m_crtImgMult);
	//区域，辅助区域，辅助线的初始化
	if (m_bDebugImg||m_bPilotImg)
	{
		m_vRgnEx.clear();
		m_vRgnEx.resize(128);

		m_vRgnExTest.clear();
		m_vRgnExTest.resize(128);
	}
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
	Hlong Width,Height;
	get_image_size (m_crtImg, &Width, &Height);
	m_imgWidth  = (long)Width;
	m_imgHeight = (long)Height;
	if(m_bFileUpdated)
	{
		m_dictCrtPathModelID.clear();
		m_dictCrtPathModelID = m_dictNewPathModelID;

		m_dictCrtPathRgn.clear();
		m_dictCrtPathRgn = m_dictNewPathRgn;
		m_bFileUpdated = false;
	}
	float Phi=90;
	if(SecondExposureImagerotation(m_crtImgIdentify, Phi))
	{
		Hobject* pObj =(Hobject *)pImgCorrect;
		*pObj = m_crtImg;
	}
	else
	{
		pImgCorrect = nullptr;
	}
	m_vPos[0].m_x = (float)(m_imgWidth/2);
	m_vPos[0].m_y = (float)(m_imgHeight/2);
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::GetRegionEx(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnEx[rgnID];
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::GetRegionExTest(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnExTest[rgnID];
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GetGlueInjectorID(LONG* pInjectorID)
{
	*pInjectorID = m_nCrtInjectorID;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GetWhiteErrorRegion(BYTE rgnID,  LONG* pWhiteErrorRgn)
{
	Hobject* pObj = (Hobject*)pWhiteErrorRgn;
	*pObj = m_vWhiteErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CBatteryShellAlgo::GetCurrentLocationXY(BSTR* bstrLocationXY)
{
	// TODO: 在此添加实现代码
	CString cstrXY;
	cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_modelMatchingPARA[2].D(),m_modelMatchingPARA[1].D(),m_modelMatchingPARA[3].D());
	CComBSTR strLocationXY = cstrXY.AllocSysString();
	HRESULT hr = strLocationXY.CopyTo(bstrLocationXY);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GetCenterLocation(BSTR* bstrLocationXY)
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

bool CBatteryShellAlgo::ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA)
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
STDMETHODIMP CBatteryShellAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
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
STDMETHODIMP CBatteryShellAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
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
STDMETHODIMP CBatteryShellAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}
STDMETHODIMP CBatteryShellAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::SaveResult(BSTR* strModelName,BYTE productID)
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
STDMETHODIMP CBatteryShellAlgo::ResetModels(BYTE productID)
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
STDMETHODIMP CBatteryShellAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
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
STDMETHODIMP CBatteryShellAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
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
STDMETHODIMP CBatteryShellAlgo::ModelCreate(double Radius, HTuple * ModelID1)
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
STDMETHODIMP CBatteryShellAlgo::ModelCreate1(double Radius, HTuple * ModelID1)
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
STDMETHODIMP CBatteryShellAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::ReloadExternalFile(BSTR* bstrPath)
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
STDMETHODIMP CBatteryShellAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GetInterImgs(BYTE rgnID, LONG* pInterImgs)
{
	Hobject* pObj = (Hobject*)pInterImgs;
	*pObj = m_vInterImgs[rgnID];
	return S_OK;
}

//void GetDefectsInPolarImg4AnnualRota(Hobject Image, Hobject PolarTransImageOrigin, Hobject PolarTransImageRotated, Hobject* pDefects, HTuple ThredLow, HTuple DefectSizeLow, HTuple Offset, bool m_bDebugImg, HTuple fCenterY, HTuple fCenterX, HTuple RadiusStart, HTuple RadiusEnd, HTuple RotaAngle, HTuple StartAngle, HTuple EndAngle, HTuple* pDefectPolarArea, HTuple DefectType = "dark")
//{
//	gen_empty_obj(pDefects);
//	*pDefectPolarArea = 0;
//
//	Hobject BlackRegDynLight, BlackRegDynDark, BlackRegDynDarkMoved, BlackRegDyn;
//	Hobject BlackRegConnected, BlackDefectsPolar;
//	dyn_threshold(PolarTransImageRotated, PolarTransImageOrigin, &BlackRegDynLight, ThredLow, "light");
//	dyn_threshold(PolarTransImageRotated, PolarTransImageOrigin, &BlackRegDynDark, ThredLow, "dark");	
//
//	if (DefectType == "dark")
//	{
//		move_region(BlackRegDynDark, &BlackRegDynDarkMoved, 0, Offset);
//		intersection(BlackRegDynDarkMoved, BlackRegDynLight, &BlackRegDyn);
//	} 
//	else if(DefectType == "white")
//	{
//		move_region(BlackRegDynLight, &BlackRegDynDarkMoved, 0, Offset);
//		intersection(BlackRegDynDarkMoved, BlackRegDynDark, &BlackRegDyn);
//	}
//
//	HTuple OriginImgWidth, OriginImgHeight;
//	get_image_size(Image, &OriginImgWidth, &OriginImgHeight);
//
//	HTuple AngleEndOrigin = HTuple(EndAngle).Rad();
//	HTuple PolarImgHeight = RadiusStart - RadiusEnd;
//
//	HTuple PolarImgWidth = AngleEndOrigin *0.5*(RadiusStart + RadiusEnd);
//	HTuple InterpolationMethod = "nearest_neighbor";
//
//	closing_circle(BlackRegDyn, &BlackRegDyn, 2.5);
//	connection(BlackRegDyn, &BlackRegConnected);
//	select_shape(BlackRegConnected, &BlackDefectsPolar, "area", "and", DefectSizeLow, OriginImgWidth * OriginImgHeight);
//	union1(BlackDefectsPolar,&BlackDefectsPolar);
//	HTuple BlackDefectPolarArea, BlackDefectPolarRow, BlackDefectPolarCol;
//	area_center(BlackDefectsPolar, &BlackDefectPolarArea, &BlackDefectPolarRow, &BlackDefectPolarCol);
//
//	Hobject BlackDefects;
//	if (BlackDefectPolarArea > 0)
//	{
//		if (m_bDebugImg)
//		{
//			polar_trans_region_inv(BlackDefectsPolar, &BlackDefects, fCenterY, fCenterX, HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, OriginImgWidth, OriginImgHeight, InterpolationMethod);
//			union1(BlackDefects, &BlackDefects);
//			concat_obj(*pDefects, BlackDefects, pDefects);
//		} 
//		else
//		{
//			//在线检测阶段，只利用错误区域的重心，标记成圆形区域
//			HTuple RadiusOrigin = RadiusStart - BlackDefectPolarRow;
//			HTuple Theta = 1.0 * BlackDefectPolarCol / PolarImgWidth * HTuple((EndAngle - StartAngle).Abs()).Rad() + HTuple(StartAngle).Rad();
//
//			HTuple sinTheta, cosTheta;
//			tuple_sin(Theta, &sinTheta);
//			tuple_cos(Theta, &cosTheta);
//
//			HTuple Row_Origin = fCenterY - RadiusOrigin* sinTheta;
//			HTuple Col_Origin = fCenterX + RadiusOrigin* cosTheta;
//
//			HTuple Radius_Origin;
//			HTuple DefectNum;
//			tuple_length(BlackDefectPolarRow, &DefectNum);
//			tuple_gen_const(DefectNum, 10, &Radius_Origin);
//			gen_circle(&BlackDefects, Row_Origin, Col_Origin, Radius_Origin);
//			union1(BlackDefects, &BlackDefects);
//			concat_obj(*pDefects, BlackDefects, pDefects);
//		}
//
//		*pDefectPolarArea = BlackDefectPolarArea;
//	}	
//}
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

	HTuple PolarImgWidth = AngleEndOrigin *0.5*(RadiusStart + RadiusEnd);
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
		polar_trans_region_inv(BlackDefectsPolar, &BlackDefects, fCenterY, fCenterX, HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, OriginImgWidth, OriginImgHeight, InterpolationMethod);
		union1(BlackDefects, &BlackDefects);
		concat_obj(*pDefects, BlackDefects, pDefects);
		*pDefectPolarArea = BlackDefectPolarArea;
	}	
}

STDMETHODIMP CBatteryShellAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
	//利用边缘提取一个闭合区域，显示为圆形，实际生成的是个不规则的形状传给其他算子
	int		rId			= (int)rgnId->fltVal;
	double	Radius		= (double)pRadius->fltVal;
	int		MinScore	= (int)pMinScore->fltVal;

	Hobject newRgn;
	m_vRgn[rId] = newRgn;
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
			m_vRgn[rId] = Circle;
			return S_OK;
		}
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.2, Height/1.6);   //默认图片的大小有要求
		double Pt_X, Pt_Y;
		BOOL Result = H_LocateCircleUseProfile(Image, Ellipse, &Circle, Radius, MinScore,  &Pt_X, &Pt_Y);
		m_centerLocationRegion=Circle;
		HTuple x,y;
		smallest_circle(Circle,&x,&y,&m_locationRadius);
		gen_circle(&m_centerLocationCircle,x,y,m_locationRadius);
		m_vPos[0].m_x = (float)Pt_X;
		m_vPos[0].m_y = (float)Pt_Y;
		m_vRgn[rId] = Circle;

		if (!Result)
		{
			return S_FALSE;
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
STDMETHODIMP CBatteryShellAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBatteryShellAlgo::CenterLocationAlgo1(VARIANT* rgnId, VARIANT* pDetectType,VARIANT* pRadius, VARIANT* pMinScore)
{
	/*利用形状模板来定位圆*/
	int		rId			= (int)rgnId->fltVal;
	double	DetectType	= (double)pDetectType->fltVal;
	double	Radius		= (double)pRadius->fltVal;
	float	MinScore	= (float)pMinScore->fltVal;

	Hobject newRgn;
	m_vRgn[rId] = newRgn;

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
			m_centerLocationRegion = Circle;
			gen_circle(&Circle1, Row, Column, Radius);
			m_vPos[0].m_y = (float)Row;
			m_vPos[0].m_x = (float)Column;
			m_locationRadius = Radius1;
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
				m_vRgn[rId] = RegionDifference;
			}
		}
		else
		{
			gen_circle(&Circle1, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[rId] = Circle1;
			return S_FALSE;
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
STDMETHODIMP CBatteryShellAlgo::CenterLocationAlgo1Help(BSTR* pHelpStr)
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

STDMETHODIMP CBatteryShellAlgo:: CenterLocationAlgoUsePointFitting(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold)
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
STDMETHODIMP CBatteryShellAlgo:: CenterLocationAlgoUsePointFittingHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBatteryShellAlgo::IlluminateCenterLocationAlgoHS(VARIANT* rgnId,VARIANT* pIsShowInterImg,VARIANT* pDetectType, VARIANT* pRadius,VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
{
	int rId					= (int)rgnId->fltVal;
	int IsShowInterImg		= (int)pIsShowInterImg->fltVal;
	int Radius				= (int)pRadius->fltVal;
	int DetectType			= (int)pDetectType->fltVal;
	int ThresholdLow		= (int)pThresholdLow->fltVal;
	int OutlierRemoverSize	= (int)pOutlierRemoverSize->fltVal;

	Hobject newRgn,SelectedRegions,EmptyObj;
	m_vRgn[rId] = newRgn;
	Hlong Width, Height;
	HTuple hv_Class, CenterRadius, hv_Row3, hv_Column3;
	Hobject threshed,RegionFillUp,RegionUnion,RegionOpening;
	Hobject  Ellipse, ImageReduced,Circle,RegionConnection;
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
		m_originImageMean =EmptyObj;
		get_image_size (Image, &Width, &Height);
		m_vInterImgs[rId]=EmptyObj;
		 
		 
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Radius, Radius);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		if(DetectType==1)
		{
			threshold(ImageReduced, &threshed,ThresholdLow, 255);
		}
		else
		{
			threshold(ImageReduced, &threshed,0, ThresholdLow);
		}
		fill_up(threshed, &RegionFillUp);
		union1(RegionFillUp, &RegionUnion);
		//****矫正圆****
		Hobject RegionDilation;
		HTuple CenterArea;
		connection(RegionUnion, &RegionConnection);
		select_shape_std (RegionConnection, &SelectedRegions, "max_area", 70);
		dilation_circle(SelectedRegions, &RegionDilation, 15.5);
		opening_circle(RegionDilation, &RegionOpening,OutlierRemoverSize+0.5);
		area_center (RegionOpening, &CenterArea, &hv_Row3, &hv_Column3);
		tuple_sqrt(CenterArea/PI, &CenterRadius);
		if (CenterRadius<15)
		{
			return S_FALSE;
		}
		CenterRadius = CenterRadius-15.0;
		gen_circle(&Circle, hv_Row3, hv_Column3, CenterRadius);
		get_obj_class(Circle, &hv_Class);
		m_centerLocationRegion=Circle;
		//
		if (0 != (hv_Class==HTuple("region")))
		{	
			m_locationRadius=CenterRadius;
			m_vPos[0].m_y = (float)hv_Row3[0].D();
			m_vPos[0].m_x = (float)hv_Column3[0].D();
			m_vRgn[rId] = Circle;
		}
		else
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_locationRadius=Radius;
			m_vRgn[rId] = Circle;
			m_centerLocationRegion=m_vRgn[rId];
			return S_FALSE;
		}
		//灰度拉伸
		if(IsShowInterImg)
		{
			Hobject ImageMean,ExpandedImage,ImageResult;
			reduce_domain (Image, Circle, &ImageReduced);
			expand_domain_gray (ImageReduced, &ExpandedImage, 45);
			mean_image (ExpandedImage, &ImageMean, 30, 30);
			div_image (ExpandedImage, ImageMean, &ImageResult, 100, 0);
			reduce_domain (ImageResult, Circle, &m_originImageMean);
			m_vInterImgs[rId]=m_originImageMean;
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
STDMETHODIMP CBatteryShellAlgo::IlluminateCenterLocationAlgoHSHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("639$\
					 381;B$\
					 5016;LP;0;1;1;1$\
					 1314;LP;0;1;1;1$\
					 5018;LP;10;2000;1;200$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::GrayLevelUniformityCorrectionAlgo(VARIANT* rgnId, VARIANT* pFilterSize, VARIANT* pDetectArea)
{
	//图片拉伸，对大面积灰度不均的图片进行校正，得到预处理的中间图片。
	//当做检测算子使用，判决条件>=0即可，可灵活搭配不同中心定位及任意区域。
	int rId                       = (int)rgnId->fltVal;
	int FilterSize                = (int)pFilterSize->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI   = m_vRgn[rId];
		Hobject Image= m_crtImg;
		if(ROI.Id() == H_EMPTY_REGION) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Hobject EmptyObj, ImageReduced, ImageMean,ExpandedImage,ImageResult;
		m_vInterImgs[rId]=EmptyObj;
		reduce_domain (Image, ROI, &ImageReduced);
		expand_domain_gray (ImageReduced, &ExpandedImage, FilterSize);
		mean_image (ExpandedImage, &ImageMean, FilterSize, FilterSize);
		div_image (ExpandedImage, ImageMean, &ImageResult, 127, 0);
		reduce_domain (ImageResult, ROI, &m_originImageMean);
		m_vInterImgs[rId]=m_originImageMean;
		
		retValue = 1;
		retValue.Detach(pDetectArea);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GrayLevelUniformityCorrectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4029$\
					 4030;R;LP;0;99999;1;>=#0$\
					 381;B$\
					 4031;LP;3;300;1;30");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::ExcludePointGradientDetectAlgo(VARIANT* rgnId, VARIANT* pEdgeThresh, VARIANT* pBlackNoiseThred, VARIANT* pWhiteNoiseThred, VARIANT* pWhiteNoiseSize, VARIANT* pDeviationDiff, VARIANT* pAreaThred,  VARIANT* pDetectArea)
{
	//剔除纯黑、白色缺陷的检测算子，与PGradientDetectAlgo检测方式类似。黑点直接剔除，白点通过大小剔除
	int		rId                       = (int)rgnId->fltVal;
	int		EdgeThresh                = (int)pEdgeThresh->fltVal;
	int		BlackNoiseThred           = (int)pBlackNoiseThred->fltVal;
	int		WhiteNoiseThred           = (int)pWhiteNoiseThred->fltVal;
	int		WhiteNoiseSize            = (int)pWhiteNoiseSize->fltVal;
	float	DeviationDiff			  = (float)pDeviationDiff->fltVal;
	int		AreaThred				  = (int)pAreaThred->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI   = m_vRgn[rId];
		if(ROI.Id() == H_EMPTY_REGION) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Hobject DefectRegs;
		HTuple hv_Area, hv_Row, hv_Column;
		Hobject ImageReduced,DerivGauss,Region1,ConnectedRegions1,RegionDilation1,RegionUnion,RegionIntersection,RegionDifference;
		Hobject BlackRegion,WhiteRegion,BlackRegions,WhiteRegions,BlackRegionsDilation,WhiteRegionsDilation,RegionDifference1;
		Hobject SelectedWhiteRegions,BlackRegionUnion,WhiteRegionUnion,BWRegionUnion,ConnectedRegions2,SelectedRegions1,SelectedRegions2;
		HTuple Mean,Deviation;

		Hobject Image,EmptyObj;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? Image = m_originImageMean:Image = m_crtImg;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? m_vInterImgs[rId]=m_originImageMean: m_vInterImgs[rId]=EmptyObj;

		reduce_domain(Image, ROI,&ImageReduced);
		derivate_gauss(Image, &DerivGauss,3, "gradient");
		threshold (DerivGauss, &Region1,HTuple(EdgeThresh/10.0), 255);
		connection (Region1, &ConnectedRegions1);
		dilation_circle (ConnectedRegions1, &RegionDilation1, 3.5);
		union1 (RegionDilation1, &RegionUnion);
		intersection (ROI, RegionUnion, &RegionIntersection);
		difference (ROI, RegionIntersection, &RegionDifference);
		intensity (RegionDifference, ImageReduced, &Mean, &Deviation);

		//找黑点、小白点并剔除
		threshold (ImageReduced, &BlackRegions, 0, BlackNoiseThred);
		threshold (ImageReduced, &WhiteRegion, WhiteNoiseThred,255 );
       //connection (BlackRegion, &BlackRegions);
		connection (WhiteRegion, &WhiteRegions);
		dilation_circle (BlackRegions, &BlackRegionsDilation, 1.5);
		dilation_circle (WhiteRegions, &WhiteRegionsDilation, 1.5);
		select_shape (WhiteRegionsDilation, &SelectedWhiteRegions, "area", "and", 0, WhiteNoiseSize);
		union1 (SelectedWhiteRegions, &WhiteRegionUnion);
		//union1 (BlackRegionsDilation, &BlackRegionUnion);
		union2 (BlackRegionsDilation, WhiteRegionUnion, &BWRegionUnion);
		difference (RegionIntersection, BWRegionUnion, &RegionDifference1);
		double SDeviation;
		SDeviation = Deviation[0].D()+DeviationDiff;
		connection (RegionDifference1, &ConnectedRegions2);
		select_gray (ConnectedRegions2, ImageReduced, &SelectedRegions1, "deviation", "and", SDeviation, 255);
		select_shape (SelectedRegions1, &SelectedRegions2, "area", "and",AreaThred, 99999);
		union1(SelectedRegions2,&DefectRegs);
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::ExcludePointGradientDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5003$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 5020;LP;1;255;1;36$\
					 3094;LP;1;127;1;65$\
					 3095;LP;127;255;1;220$\
					 3093;LP;1;99999;1;10$\
					 3096;FP;1;50;0.1;8$\
					 434;LP;1;99999;1;80");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::OneWayDetectAlgo(VARIANT* rgnId, VARIANT* pLightDirection, VARIANT* pFilterSize, VARIANT* pAreaShrink, VARIANT* pDetectionMode, VARIANT* pSensitivity, VARIANT* pAreaThred,  VARIANT* pDetectArea)
{
	//单方向梯度异常检测，用于检测单侧光下的鼓顶缺陷。
	int rId                       = (int)rgnId->fltVal;
	int LightDirection            = (int)pLightDirection->fltVal;
	int FilterSize                = (int)pFilterSize->fltVal;
	int AreaShrink				  = (int)pAreaShrink->fltVal;
	int DetectionMode             = (int)pDetectionMode->fltVal;
	int Sensitivity			      = (int)pSensitivity->fltVal;
	int AreaThred				  = (int)pAreaThred->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI   = m_vRgn[rId];
		if(ROI.Id() == H_EMPTY_REGION) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Hobject DefectRegs;
		HTuple hv_Area, hv_Row, hv_Column;
		HTuple SensitivityReal;
		Hobject ImageReduced, DerivGauss, RegionErosion, ImageErosion, Region1, ConnectedRegions, SelectedRegions;
		Hobject Image= m_crtImg;
		int FilterType = LightDirection%2; //取余判断滤波方向
		int DefectType;
		(LightDirection<2)?DefectType = 0 : DefectType = 1; //大小判断缺陷类型
		reduce_domain (Image, ROI, &ImageReduced);
		if (FilterType == 0)
		{
			derivate_gauss (ImageReduced, &DerivGauss, FilterSize, "x");
		} 
		else
		{
			derivate_gauss (ImageReduced, &DerivGauss, FilterSize, "y");
		}
		erosion_circle (ROI, &RegionErosion, AreaShrink + 0.5);
		reduce_domain (DerivGauss, RegionErosion, &ImageErosion);
		tuple_real (Sensitivity, &SensitivityReal);

		
		SensitivityReal = -(SensitivityReal - 300)/100;   //参数归一化
		if (DetectionMode == DefectType)//00/11 缺陷为黑 10/01缺陷为白
		{
			
			threshold (ImageErosion, &Region1, -999, SensitivityReal);
		} 
		else
		{
			SensitivityReal = -SensitivityReal;
			threshold (ImageErosion, &Region1, SensitivityReal, 999);
		}
		//
		connection(Region1, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions, "area", "and",AreaThred, 99999);
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::OneWayDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4011$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 4012;LP;0;3;1;0$\
					 4006;LP;5;21;1;11$\
					 501;LP;0;50;1;10$\
					 4013;LP;0;1;1;0$\
					 1091;LP;200;500;1;380$\
					 1379;LP;1;999;1;80");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::RotaDynThresholdWithFilterAlgo(VARIANT* rgnId,VARIANT* pIsShowInterImg,VARIANT* pFilterSize,VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea)
{
	//带滤波的旋转动态灰度值，带灰度拉伸的检测
	int rId = (int)rgnId->fltVal;
	int IsShowInterImg = (int)pIsShowInterImg->fltVal;
	float FilterSize = (float)pFilterSize->fltVal;
	float RotaAngle = (float)pRotaAngle->fltVal;
	int BlackThredLow = (int)pBlackThredLow->fltVal;
	int BlackDefectSize = (int) pBlackDefectSize->fltVal;
	int WhiteThredLow = (int)pWhiteThredLow->fltVal;
	int WhiteDefectSize = (int) pWhiteDefectSize->fltVal;
	CComVariant retValue;

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
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		Hobject rgn = m_vRgn[rId];

		if(rgn.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		//
		Hobject Image,EmptyObj;
		Hobject ImageReduced,ImageAffinTrans,RegionDynThresh,ConnectedRegions4,SelectedRegions4;

		HTuple RadiusStart, RadiusEnd;

		m_vInterImgs[rId]=EmptyObj;
		Image = m_crtImg;
		if(IsShowInterImg && m_originImageMean.Id()!=H_EMPTY_REGION)
		{
			m_vInterImgs[rId]=m_originImageMean;
			Image = m_originImageMean;
		}
		if(FilterSize>0.01)
		{
			smooth_image (Image, &Image, "gauss",FilterSize);
		}

		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		//get the params for polar trans

		HTuple InnerRadius, OuterRadius;
		region_features (rgn, "inner_radius", &InnerRadius);
		region_features (rgn, "outer_radius", &OuterRadius);

		float RoiWidth = InnerRadius[0].D();
		float Radius = OuterRadius[0].D();
		float StartAngle = m_sectorAngle[rId].m_angleStartAbs;	//逆时针转顺时针
		float EndAngle =  m_sectorAngle[rId].m_angleEndAbs;


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

		HTuple PolarImgWidth = AngleEndOrigin *0.5*(RadiusStart + RadiusEnd);
		HTuple InterpolationMethod = "nearest_neighbor";

		//As RadiusStart > RadiusEnd, the polar transformation is performed clockwise(顺时针)
		Hobject PolarTransImageOrigin, PolarTransImageRotated;
		polar_trans_image_ext(Image, &PolarTransImageOrigin, fCenterY, fCenterX,  HTuple(StartAngle).Rad(), AngleEndOrigin, RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);
		polar_trans_image_ext(Image, &PolarTransImageRotated, fCenterY, fCenterX, HTuple(StartAngle + RotaAngle).Rad(), AngleEndOrigin + HTuple(RotaAngle).Rad(), RadiusStart, RadiusEnd, PolarImgWidth, PolarImgHeight, InterpolationMethod);

		HTuple MiddleRadius = 0.5*(RadiusStart + RadiusEnd);
		HTuple Offset = MiddleRadius * HTuple(RotaAngle).Rad();
		HTuple OriginImgWidth, OriginImgHeight;
		get_image_size(Image, &OriginImgWidth, &OriginImgHeight);

		Hobject DefectRegs;
		gen_empty_obj(&DefectRegs);

		Hobject BlackDefects;
		gen_empty_obj(&BlackDefects);
		HTuple  BlackDefectPolarArea=0;

		if(BlackThredLow<255)
		{
			GetDefectsInPolarImg4AnnualRota(Image, PolarTransImageOrigin, PolarTransImageRotated,&BlackDefects, BlackThredLow, BlackDefectSize, Offset, m_bDebugImg, fCenterY, fCenterX, RadiusStart, RadiusEnd, RotaAngle, StartAngle, EndAngle, &BlackDefectPolarArea, "dark");
			intersection(rgn, BlackDefects,&BlackDefects);
		}
		union2(DefectRegs, BlackDefects, &DefectRegs);

		Hobject WhiteDefects;
		HTuple  WhiteDefectPolarArea=0;
		gen_empty_obj(&WhiteDefects);
		if(WhiteThredLow<255)
		{
			GetDefectsInPolarImg4AnnualRota(Image, PolarTransImageOrigin, PolarTransImageRotated, &WhiteDefects, WhiteThredLow, WhiteDefectSize, Offset, m_bDebugImg, fCenterY, fCenterX, RadiusStart, RadiusEnd, RotaAngle, StartAngle, EndAngle, &WhiteDefectPolarArea, "white");
			intersection(rgn, WhiteDefects, &WhiteDefects);
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
			dilation_circle (WhiteDefects, &WhiteDefects, 2.5);
			m_vErrorRgn[rId] = BlackDefects;
			m_vWhiteErrorRgn[rId] = WhiteDefects;			

		    retValue = Area[0].I();

			////输出为极坐标下的缺陷面积，避免前后不一致，2018.05.20
			//HTuple PolarDefectArea = 0;

			//if (BlackDefectPolarArea[0].I() > 0)
			//{
			//	PolarDefectArea = PolarDefectArea + BlackDefectPolarArea;
			//}

			//if (WhiteDefectPolarArea[0].I() > 0)
			//{
			//	PolarDefectArea = PolarDefectArea + WhiteDefectPolarArea;
			//}

			//retValue = PolarDefectArea[0].I();
		}
		retValue.Detach(pDetectArea);


#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::RotaDynThresholdWithFilterAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5017$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 5023;LP;0;1;1;1$\
					 679;FP;0;15;0.1;0$\
					 610;FP;0.5;60;0.5;3.5$\
					 427;LP;0;255;1;23$\
					 428;LP;1;1000;1;10$\
					 430;LP;0;255;1;23$\
					 431;LP;1;1000;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::EmphasizeDarkPointDetectAlgo(VARIANT* rgnId, VARIANT* pFilterSize,VARIANT* pWindowWidth, VARIANT* pEmphasizeFactor, VARIANT* pAreaFilter, VARIANT* pDetectArea)
{
	//有拉伸图像用拉伸图像，没有用原始图形
	int		rId					= (int)rgnId->fltVal;
	float	FilterSize			= (float)pFilterSize->fltVal;
	int		WindowWidth			= (int)pWindowWidth->fltVal;
	float	EmphasizeFactor		= (float)pEmphasizeFactor->fltVal;
	int		AreaFilter			= (int)pAreaFilter->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];

		if(ROI.Id() == H_EMPTY_REGION ) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Hobject DefectRegs,ConvertImage,ImageReduced,ExpandedImage,ConnectedRegions,SelectedRegions,Image,EmptyObj;
		HTuple hv_Area, hv_Row, hv_Column;
		HTuple hv_width,hv_height;

		(m_originImageMean.Id()!=H_EMPTY_REGION)? Image = m_originImageMean:Image = m_crtImg;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? m_vInterImgs[rId]=m_originImageMean: m_vInterImgs[rId]=EmptyObj;

		get_image_size(Image,&hv_width,&hv_height);
		Hobject ImageMean,RegionDynThresh,RegionDilation,RegionErosion;
		reduce_domain(Image,ROI,&ImageReduced);
		smooth_image (ImageReduced, &ImageMean, "gauss",FilterSize);
		if(hv_width*2<=WindowWidth)
		{
			WindowWidth=hv_width[0].I()*2-1;
		}
		if(hv_height*2<=WindowWidth)
		{
			WindowWidth=hv_height[0].I()*2-1;
		}
		expand_domain_gray (ImageMean, &ExpandedImage,33);
		emphasize (ExpandedImage, &ConvertImage, WindowWidth,WindowWidth,EmphasizeFactor);
		threshold (ConvertImage, &RegionDynThresh, 0, 10);
		intersection(ROI,RegionDynThresh,&RegionDynThresh);
		connection (RegionDynThresh, &ConnectedRegions);
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
		/////////////////////////////////////////////

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::EmphasizeDarkPointDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1255$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 679;FP;0.01;20;0.01;0.7$\
					 1257;LP;3;201;1;150$\
					 1258;FP;1;255;0.02;2$\
					 434;LP;1;9999999;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::GDarkLineWithFilterAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1338$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 5023;LP;0;1;1;1$\
					 426;FP;1.1;4;0.1;1.1$\
					 427;FP;0;8;0.5;3$\
					 1358;LP;1;255;1;50$\
					 428;LP;1;9999;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GDarkLineWithFilterAlgo(VARIANT* rgnId,VARIANT* pIsShowInterImg, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pDarkGrayThresh, VARIANT* pBlackDefectSize, VARIANT* pDetectArea)
{
	//盖面找线算法 有拉伸图像用拉伸图像，没有用原始图形，带滤波的找线算子
	int		rId						= (int)rgnId->fltVal;
	int		IsShowInterImg			= (int)pIsShowInterImg->fltVal;
	float	BlackMaskSize			= (float)pBlackMaskSize->fltVal;
	double	BlackThredLow			= (double)pBlackThredLow->fltVal;
	int		DarkGrayThresh			= (int)pDarkGrayThresh->fltVal;
	int		BlackDefectSize			= (int)pBlackDefectSize->fltVal;

	int		BlackDefectSizeUpper	= 90000;
	Hobject rgn						= m_vRgn[rId];
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue         = -1;
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
		Hobject Image,EmptyObj;
		m_vInterImgs[rId]=EmptyObj;
		Image = m_crtImg;
		if(IsShowInterImg && m_originImageMean.Id()!=H_EMPTY_REGION)
		{
			m_vInterImgs[rId]=m_originImageMean;
			Image = m_originImageMean;
		}

		Hobject ImageReduced, ExpandedImage, DerivGauss, rgnErosion;
		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);
		//*===================================================================
		Hobject BlackLines, ContoursSplit, SelectedContours, Regions,SelectedRegions;	
		reduce_domain(ExpandedImage, rgn, &ImageReduced);
		erosion_circle(rgn, &rgnErosion, 2.5);
		lines_gauss(ImageReduced, &BlackLines, BlackMaskSize, BlackThredLow, 8, "dark", "false", "bar-shaped", "false");
		segment_contours_xld (BlackLines, &ContoursSplit, "lines_circles", 5, 4, 2);
		select_contours_xld (ContoursSplit, &SelectedContours, "curvature", 0.0, 0.5, -0.5, 0.5);

		ImageProcessAlg.XldsToRegions(SelectedContours,&Regions,0.5);
		select_gray (Regions, Image, &SelectedRegions, "mean", "and", 0, DarkGrayThresh);
		select_shape (SelectedRegions, &SelectedRegions, "area", "and", BlackDefectSize, BlackDefectSizeUpper);

		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
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
		m_vRgnExTest[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::GWhiteineWithFilterAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1357$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 5023;LP;0;1;1;1$\
					 429;FP;1.1;4;0.1;1.1$\
					 430;FP;0;8;0.5;3$\
					 1359;LP;1;255;1;50$\
					 431;LP;1;9999;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GWhiteineWithFilterAlgo(VARIANT* rgnId,VARIANT* pIsShowInterImg, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pDarkGrayThresh, VARIANT* pBlackDefectSize, VARIANT* pDetectArea)
{
	//盖面找线算法 有拉伸图像用拉伸图像，没有用原始图形，带滤波的找线算子
	int		rId					= (int)rgnId->fltVal;
	int		IsShowInterImg		= (int)pIsShowInterImg->fltVal;
	float	BlackMaskSize		= (float)pBlackMaskSize->fltVal;
	double	BlackThredLow		= (double)pBlackThredLow->fltVal;
	int		DarkGrayThresh		= (int)pDarkGrayThresh->fltVal;
	int		BlackDefectSize     = (int)pBlackDefectSize->fltVal;

	int		BlackDefectSizeUpper = 99999;
	Hobject rgn					 = m_vRgn[rId];
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue         = -1;
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
		Hobject Image,EmptyObj;
		m_vInterImgs[rId]=EmptyObj;
		Image = m_crtImg;
		if(IsShowInterImg && m_originImageMean.Id()!=H_EMPTY_REGION)
		{
			m_vInterImgs[rId]=m_originImageMean;
			Image = m_originImageMean;
		}

		Hobject ImageReduced, ExpandedImage, DerivGauss, rgnErosion,SelectedRegions;
		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 2);
		//*===================================================================
		Hobject BlackLines, ContoursSplit, SelectedContours, Regions;	
		reduce_domain(ExpandedImage, rgn, &ImageReduced);
		erosion_circle(rgn, &rgnErosion, 2.5);
		lines_gauss(ImageReduced, &BlackLines, BlackMaskSize, BlackThredLow, 8, "light", "false", "bar-shaped", "false");
		segment_contours_xld (BlackLines, &ContoursSplit, "lines_circles", 5, 4, 2);
		select_contours_xld (ContoursSplit, &SelectedContours, "curvature", 0.0, 0.5, -0.5, 0.5);
		ImageProcessAlg.XldsToRegions(SelectedContours,&Regions,0.5);
		select_gray (Regions, Image, &SelectedRegions, "mean", "and", DarkGrayThresh, 255);
		select_shape (SelectedRegions, &SelectedRegions, "area", "and", BlackDefectSize, BlackDefectSizeUpper);

		if (m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
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
			m_vWhiteErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vWhiteErrorRgn[rId] = DefectRegs;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::EdgeDetectWithLengthFilterAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5005$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 443;FP;-15;25;0.5;1.5$\
					 643;FP;1;5;0.1;1.5$\
					 644;LP;1;255;1;20$\
					 645;LP;1;255;1;45$\
					 5006;LP;1;255;1;20$\
					 5009;FP;1;9999;0.5;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::EdgeDetectWithLengthFilterAlgo(VARIANT* rgnId, VARIANT* pErosionSize,VARIANT* pGaussFilterSize, VARIANT* pContrasthredLow, VARIANT* pContrastThredHigh, VARIANT* pEdgeGrayTthresh, VARIANT* pErrorLengthSize,VARIANT* pDetectArea)
{
	////利用提取边缘的方式检测较小的打痕，尤其是长宽比比较大的,检测细长锋利的底部打痕迹很有用。剔除黑点
	int		rId					= (int)rgnId->fltVal;
	float	ErosionSize			= (float)pErosionSize->fltVal;
	float	GaussFilterSize		= (float)pGaussFilterSize->fltVal;
	int		ContrasthredLow		= (int)pContrasthredLow->fltVal;
	int		ContrastThredHigh	= (int)pContrastThredHigh->fltVal;
	int		EdgeGrayTthresh		= (int)pEdgeGrayTthresh->fltVal;
	float	ErrorLengthSize		= (float)pErrorLengthSize->fltVal;

	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue         = -1;
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
		//*===================================================================
		Hobject Image,EmptyObj;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? Image = m_originImageMean:Image = m_crtImg;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? m_vInterImgs[rId]=m_originImageMean:m_vInterImgs[rId]=EmptyObj ;
		//*===================================================================
		Hobject ImageReduced, ExpandedImage, DerivGauss, rgnErosion,ImageResult,ImaAmp, ImaDir;
		Hobject Region,RegionFillUp,ConnectedRegions,SelectedRegions,DefectRegs,BlackUnion,RegionDetect;
		HTuple Area, Row_test, Column_test;
		HTuple  Row1, Column1, Phi, Length1, Length2,QuotWH,Indices;

		if(ErosionSize>=0.5)
		{
			erosion_circle (rgn, &RegionDetect,ErosionSize);
		}
		else if(ErosionSize<-0.5)
		{
			dilation_circle(rgn, &RegionDetect,abs(ErosionSize));
		}
		else
		{
			RegionDetect = rgn;
		}

		reduce_domain (Image, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 10);
		edges_image (ExpandedImage,&ImaAmp, &ImaDir, "canny", GaussFilterSize,"nms", ContrasthredLow>ContrastThredHigh?ContrastThredHigh:ContrasthredLow, ContrasthredLow>ContrastThredHigh?ContrasthredLow:ContrastThredHigh);

		threshold (ImaAmp, &Region, EdgeGrayTthresh, 255);
		fill_up_shape (Region, &RegionFillUp, "area", 1, 300);

		intersection(RegionFillUp,RegionDetect,&RegionFillUp);
		connection(RegionFillUp,&ConnectedRegions);

		select_shape (ConnectedRegions, &SelectedRegions, "outer_radius", "and", ErrorLengthSize/2.0, 99999);

		union1(SelectedRegions,&DefectRegs);

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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 1327;LP;0;1;1;1$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::CircleRegionLocationAlgo(VARIANT* rgnId,VARIANT* pIsShowInterImg, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域
	int		rId					= (int)rgnId->fltVal;
	int		IsShowInterImg		= (int)pIsShowInterImg->fltVal;
	m_ringPara[rId].m_Radius	= (float)pRadius->fltVal;
	m_ringPara[rId].m_RoiWidth	= (float)pRoiWidth->fltVal;

	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;
	float m_RadiusExt = Radius - RoiWidth;

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	m_sectorAngle[rId].m_angleStartAbs=0;
	m_sectorAngle[rId].m_angleEndAbs=360;

	Hobject newRgn;
	m_vRgn[rId] = newRgn;

	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[rId] = newRgn ;
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
			m_dictSpecialData[rId].clear();
		}

		Hobject Image,EmptyObj;
		m_vInterImgs[rId]=EmptyObj;
		Image = m_crtImg;
		if(IsShowInterImg && m_originImageMean.Id()!=H_EMPTY_REGION)
		{
			m_vInterImgs[rId]=m_originImageMean;
		}
		HTuple Width,Height,ImageSize;
		get_image_size (Image,&Width, &Height);
		tuple_min(Height.Concat(Width), &ImageSize);
		if(m_RadiusExt>ImageSize || Radius>ImageSize)
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, ImageSize/2);
		}
		else
		{
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
		}
		m_vRgn[rId] = RegionDetect ;	
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
STDMETHODIMP CBatteryShellAlgo::CircleRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//利用定位的到的相对半径，绘制圆形区域
	int		rId				= (int)rgnId->fltVal;
	float	RoiWidth		= (float)pRoiWidth->fltVal;
	float	RadiusSize		=(float)pRadius->fltVal;
	float	LocationRadius	=(float)(m_locationRadius[0].I());

	float Radius = LocationRadius - RadiusSize;
	float m_RadiusExt = LocationRadius - RadiusSize-RoiWidth;

	if (Radius>0)
	{

		m_ringPara[rId].m_Radius = LocationRadius - RadiusSize;
		m_ringPara[rId].m_RoiWidth = (float)pRoiWidth->fltVal;
	}
	else
	{

		m_ringPara[rId].m_Radius =(float)pRoiWidth->fltVal+2;
		m_ringPara[rId].m_RoiWidth = (float)pRoiWidth->fltVal;
	}

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;
	m_sectorAngle[rId].m_angleStartAbs=0;
	m_sectorAngle[rId].m_angleEndAbs=360;

	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[rId] = newRgn ;
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
			m_dictSpecialData[rId].clear();
		}
		if( m_RadiusExt>0 && Radius>0)
		{
			gen_circle(&Circle2_EXT, fCenterY, fCenterX, Radius);
			gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusExt);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
			m_vRgn[rId] = RegionDetect ;	
		}
		else if(m_RadiusExt<0 && Radius>0)
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, Radius);
			m_vRgn[rId] = RegionDetect ;	
		}
		else
		{
			gen_circle(&RegionDetect, fCenterY, fCenterX, 40);
			m_vRgn[rId] = RegionDetect ;
			return S_FALSE;
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
STDMETHODIMP CBatteryShellAlgo::CircleRegionLocationAlgo1Help(BSTR* pHelpStr)
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
STDMETHODIMP CBatteryShellAlgo::BoundaryRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	//得到环形区域和带HS的定位算子配合使用
	int		rId				= (int)rgnId->fltVal;
	float	RoiWidth		= (float)pRoiWidth->fltVal;
	float	RadiusSize		= (float)pRadius->fltVal;
	float	LocationRadius	= (float)(m_locationRadius[0].I());

	float fCenterX = 0.0;
	float fCenterY = 0.0;
	fCenterX = m_vPos[0].m_x;
	fCenterY = m_vPos[0].m_y;

	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[rId] = newRgn ;
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
			m_vWhiteErrorRgn[rId].Reset();
			m_vErrorRgn[rId].Reset();
			m_vRgnExTest[rId].Reset();
			return S_FALSE;
		}
		if(m_bDebugImg) 
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject RegionFill;

		fill_up(m_centerLocationRegion,&RegionFill);
		if( RadiusSize>=0.5)
		{
			erosion_circle(RegionFill,&Region_EXT,RadiusSize);
		}
		else if( RadiusSize<=-0.5)
		{
			dilation_circle(RegionFill,&Region_EXT,-RadiusSize);

		}
		else
		{
			Region_EXT=RegionFill;
		}
		erosion_circle(Region_EXT,&Region_Inner,RoiWidth);
		difference(Region_EXT,Region_Inner,&RegionDetect);

		HTuple hv_Row3, hv_Column3, CenterRadius;
		smallest_circle(RegionDetect, &hv_Row3, &hv_Column3, &CenterRadius);

		m_ringPara[rId].m_Radius = (float)CenterRadius[0].D();
		m_ringPara[rId].m_RoiWidth = RoiWidth;
		m_vRgn[rId] = RegionDetect ;	
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
STDMETHODIMP CBatteryShellAlgo::BoundaryRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBatteryShellAlgo::CircleRegionAngleAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
{
	int		id					= (int)rgnId->fltVal;
	float	DriftAngle1			= (float)pDriftAngle1->fltVal;
	float	DriftAngle2			= (float)pDriftAngle2->fltVal;
	m_ringPara[id].m_Radius		= (float)pRadius->fltVal;
	m_ringPara[id].m_RoiWidth	= (float)pRoiWidth->fltVal;


	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	float m_RadiusInner = Radius - RoiWidth;

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
STDMETHODIMP CBatteryShellAlgo::CircleRegionAngleAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBatteryShellAlgo::CircleRegionAngleAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
{
	//得到环形区域
	int		id			= (int)rgnId->fltVal;
	float	RadiusSize	= (float)pRadius->fltVal;
	float	RoiWidth	= (float)pRoiWidth->fltVal;

	m_angleDetect = m_modelMatchingPARA[3].D();
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
STDMETHODIMP CBatteryShellAlgo::CircleRegionAngleAlgo1Help(BSTR* pHelpStr)
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

STDMETHODIMP CBatteryShellAlgo::CircleRegionAngleDetectAlgo(VARIANT* rgnId, VARIANT* pSmoothSize, VARIANT* pBlackGayThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteGayThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
{
	//Large defects were detected in the fan-shaped ring area at the bottom of the battery shell（One side lighting scheme）
	int rId                       = (int)rgnId->fltVal;
	int SmoothSize                = (int)pSmoothSize->fltVal;
	int BlackGayThred			  = (int)pBlackGayThred->fltVal;
	int BlackAreaThred            = (int)pBlackAreaThred->fltVal;
	int WhiteGayThred             = (int)pWhiteGayThred->fltVal;
	int WhiteAreaThred			  = (int)pWhiteAreaThred->fltVal;
	CComVariant retValue;

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
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		Hobject rgn = m_vRgn[rId];
		if(rgn.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		//
		Hobject Image, DefectRegs;
		HTuple hv_Area, hv_Row, hv_Column;
		Hobject ImageMean, ImageResult, ImageReduced, ImageMedian, BlackRegion, ConnectedBlackRegions, SelectedBlackRegions;
		Hobject WhiteRegion, ConnectedWhiteRegions, SelectedWhiteRegions;
		Hobject ;
		HTuple ;
		Image = m_crtImg;
		mean_image(Image, &ImageMean, 100, 100);
		div_image(Image, ImageMean, &ImageResult, 100, 0);
		reduce_domain(ImageResult, rgn, &ImageReduced);
		median_image(ImageReduced, &ImageMedian, "circle", SmoothSize, "mirrored");
		threshold (ImageMedian, &BlackRegion, 0, BlackGayThred);
		connection (BlackRegion, &ConnectedBlackRegions);
		select_shape (ConnectedBlackRegions, &SelectedBlackRegions, "area", "and", BlackAreaThred, 99999);
		threshold (ImageMedian, &WhiteRegion, WhiteGayThred, 255);
		connection (WhiteRegion, &ConnectedWhiteRegions);
		select_shape (ConnectedWhiteRegions, &SelectedWhiteRegions, "area", "and", WhiteAreaThred, 99999);
		union2(SelectedBlackRegions, SelectedWhiteRegions, &DefectRegs);
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::CircleRegionAngleDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("3101$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 3104;LP;1;20;1;5$\
					 3102;LP;1;127;1;70$\
					 416;LP;1;99999;1;30$\
					 3103;LP;127;255;1;220$\
					 419;LP;1;99999;1;30");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::IrregularRingRegionLocationAlgo(VARIANT* rgnId, VARIANT* pCandidateRadial, VARIANT* pDetectType, VARIANT* pRadius, VARIANT* pMinScore, VARIANT* pInsideRatio, VARIANT* pOutsideRatio)
{
	//用于提取电池壳内壁非同心圆间的不规则环形区域，内圆用模板定位。
	int		rId						= (int)rgnId->fltVal;
	int		CandidateRadial			= (int)pCandidateRadial->fltVal;
	int		DetectType				= (int)pDetectType->fltVal;
	int		Radius					= (int)pRadius->fltVal;
	float	MinScore				= (float)pMinScore->fltVal;
	float	OutsideRatio			= (float)pOutsideRatio->fltVal;
	float	InsideRatio				= (float)pInsideRatio->fltVal;

	float Row0 = 0.0;
	float Column0 = 0.0;
	Column0 = m_vPos[0].m_x;
	Row0 = m_vPos[0].m_y;
	HTuple Radius0 = m_locationRadius;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	m_vRgnEx[rId] = newRgn;
	//
	//m_vRgnEX[rId]存参考量Object
	//m_vRgn[rId]存实际的待检测区域
	//
	if(((Row0<2.0)||(Column0<2.0))||(OutsideRatio<=InsideRatio))
	{
		m_vRgn[rId] = newRgn;
		m_vRgnEx[rId] = newRgn;
		return S_FALSE;
	}
	Hobject  Circle1_Outside, Circle2_Inside;
	Hobject  RingRegion;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image = m_crtImg;
		Hobject RefeCircle, ImageReduced, ShapeModel, ModelAtNewPosition, Region;
		HTuple Row3, Column3, Radius3, RowDiff, ColumnDiff, RadiusDiff, Area, Row, Column, Radius1, Radius2, Row1, Row2, Column1, Column2;
		HTuple CenterRow, CenterColumn, Angle, Scale, Score, CenterRadius, MovementOfObject, MoveAndScaleOfObject;

		if(Radius >= CandidateRadial)
		{
			m_vRgn[rId] = newRgn ;
			m_vRgnEx[rId] = newRgn;
			return S_FALSE;
		}
		gen_circle(&RefeCircle, Row0, Column0, CandidateRadial);
		concat_obj(m_vRgnEx[rId], RefeCircle, &m_vRgnEx[rId]);
		reduce_domain (Image, RefeCircle, &ImageReduced);

		/***************************模板匹配内圆***************************************/

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
			smallest_circle(Region, &Row3, &Column3, &Radius3);
		}
		else
		{
			m_vRgn[rId] = newRgn;
			return S_FALSE;
		}
		/**********************************************************************************/

		area_center (Region, &Area, &Row, &Column);
		if(Area<1)
		{
			m_vRgn[rId] = newRgn ;
			return S_FALSE;
		}

		RowDiff = (Row0-Row3).Abs();
		ColumnDiff = (Column0-Column3).Abs();
		RadiusDiff = (Radius0-Radius3).Abs();
		if(Row0>Row3)
		{
			Row1 = Row3 + RowDiff*OutsideRatio;
			Row2 = Row3 + RowDiff*InsideRatio;
		}
		else
		{
			Row1 = Row3 - RowDiff*OutsideRatio;
			Row2 = Row3 - RowDiff*InsideRatio;
		}
		if(Column0>Column3)
		{
			Column1 = Column3 + ColumnDiff*OutsideRatio;
			Column2 = Column3 + ColumnDiff*InsideRatio;
		}
		else
		{
			Column1 = Column3 - ColumnDiff*OutsideRatio;
			Column2 = Column3 - ColumnDiff*InsideRatio;
		}
		Radius1 = Radius3 + RadiusDiff*OutsideRatio;
		Radius2 = Radius3 + RadiusDiff*InsideRatio;
		gen_circle(&Circle1_Outside, Row1, Column1, Radius1);
		gen_circle(&Circle2_Inside, Row2, Column2, Radius2);
		difference(Circle1_Outside, Circle2_Inside, &RingRegion);
		m_vRgn[rId] = RingRegion ;	
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
STDMETHODIMP CBatteryShellAlgo::IrregularRingRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("3097$\
					 381;B$\
					 4018;LP;1;150;1;80$\
					 1314;LP;0;1;1;0$\
					 374;LP;10;300;1;60$\
					 377;FP;0.5;1.0;0.05;0.5$\
					 3100;FP;0.0;1.0;0.01;0.0$\
					 3099;FP;0.0;1.0;0.01;1.0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::IrregularRingRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pTransition, VARIANT* pLocationGayThred, VARIANT* pInsideRatio, VARIANT* pOutsideRatio)
{
	//用于提取电池壳内壁非同心圆间的不规则环形区域，对原算法进行优化增加适应性。
	int		rId						= (int)rgnId->fltVal;
	int		SearchRegRow			= (int)pSearchRegRow->fltVal;
	int		SearchRegCol			= (int)pSearchRegCol->fltVal;
	int		OuterRadius				= (int)pOuterRadius->fltVal;
	int		RoiWidth				= (int)pRoiWidth->fltVal;
	int		Transition				= (int)pTransition->fltVal;
	int		LocationGayThred	    = (int)pLocationGayThred->fltVal;
	float	OutsideRatio			= (float)pOutsideRatio->fltVal;
	float	InsideRatio				= (float)pInsideRatio->fltVal;

	float Row0 = 0.0;
	float Column0 = 0.0;
	Column0 = m_vPos[0].m_x;
	Row0	= m_vPos[0].m_y;
	HTuple Radius0 = m_locationRadius;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	m_vRgnEx[rId] = newRgn;
	//
	//m_vRgnRX[rId]存参考量Object
	//m_vRgn[rId]存实际的待检测区域
	//
	if(((Row0<2.0)||(Column0<2.0))||(OutsideRatio<=InsideRatio))
	{
		m_vRgn[rId] = newRgn ;
		m_vRgnEx[rId] = newRgn;
		return S_FALSE;
	}
	Hobject  Circle1_Outside, Circle2_Inside;
	Hobject  RingRegion;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject Image = m_crtImg;
		HTuple Width,Height;
		Hobject RefeCircle1, RefeCircle2, SearchCenterPtCross, RefeRegion, ImageReduced, ConnectedRegions, RegionFillUp, SelectedRegions, RegionOpening, Region;
		HTuple Row3, Column3, Radius3, RowDiff, ColumnDiff, RadiusDiff, SelectedRegionArea, SelectedRegionRow, SelectedRegionColumn, Radius1, Radius2, Row1, Row2, Column1, Column2;
		get_image_size(Image, &Width, &Height);
		gen_circle(&RefeCircle1, SearchRegRow, SearchRegCol, OuterRadius);
		if (OuterRadius<RoiWidth)
		{
			RoiWidth=OuterRadius;
		}
		gen_circle(&RefeCircle2, SearchRegRow, SearchRegCol, OuterRadius-RoiWidth);
		difference(RefeCircle1, RefeCircle2, &RefeRegion);
		concat_obj(m_vRgnEx[rId], RefeRegion, &m_vRgnEx[rId]);
		gen_cross_contour_xld(&SearchCenterPtCross, SearchRegRow, SearchRegCol, 13, HTuple(45).Rad());
		concat_obj(m_vRgnEx[rId], SearchCenterPtCross, &m_vRgnEx[rId]);
		reduce_domain (Image, RefeRegion, &ImageReduced);
		if (Transition!=0)
		{
			threshold (ImageReduced, &Region, LocationGayThred, 255);
		} 
		else
		{
			threshold (ImageReduced, &Region, 0, LocationGayThred);
		}
		connection (Region, &ConnectedRegions);
		fill_up (ConnectedRegions, &RegionFillUp);
		select_shape_std(RegionFillUp, &SelectedRegions, "max_area", 70);
		area_center (SelectedRegions, &SelectedRegionArea, &SelectedRegionRow, &SelectedRegionColumn);
		if(SelectedRegionArea<1)
		{
			m_vRgn[rId] = newRgn ;
			return S_FALSE;
		}
		opening_circle (SelectedRegions, &RegionOpening, 3.5);
		smallest_circle (RegionOpening, &Row3, &Column3, &Radius3);
		RowDiff = (Row0-Row3).Abs();
		ColumnDiff = (Column0-Column3).Abs();
		RadiusDiff = (Radius0-Radius3).Abs();
		if(Row0>Row3)
		{
			Row1 = Row3 + RowDiff*OutsideRatio;
			Row2 = Row3 + RowDiff*InsideRatio;
		}
		else
		{
			Row1 = Row3 - RowDiff*OutsideRatio;
			Row2 = Row3 - RowDiff*InsideRatio;
		}
		if(Column0>Column3)
		{
			Column1 = Column3 + ColumnDiff*OutsideRatio;
			Column2 = Column3 + ColumnDiff*InsideRatio;
		}
		else
		{
			Column1 = Column3 - ColumnDiff*OutsideRatio;
			Column2 = Column3 - ColumnDiff*InsideRatio;
		}
		Radius1 = Radius3 + RadiusDiff*OutsideRatio;
		Radius2 = Radius3 + RadiusDiff*InsideRatio;
		gen_circle(&Circle1_Outside, Row1, Column1, Radius1);
		gen_circle(&Circle2_Inside, Row2, Column2, Radius2);
		difference(Circle1_Outside, Circle2_Inside, &RingRegion);
		m_vRgn[rId] = RingRegion ;	
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
STDMETHODIMP CBatteryShellAlgo::IrregularRingRegionLocationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("3097$\
					 381;B$\
					 1298;LP;1;1600;1;220$\
					 1299;LP;1;1200;1;200$\
					 194;FP;1.0;2000.0;0.5;80$\
					 413;FP;1.5;2000;0.5;50$\
					 4023;LP;0;1;1;0$\
					 3098;LP;0;255;1;60$\
					 3100;FP;0.0;1.0;0.01;0.0$\
					 3099;FP;0.0;1.0;0.01;1.0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CBatteryShellAlgo::ZoningRing(Hobject Image, Hobject Ring, Hobject MiddleCircle, Hobject* ZoningReg, HTuple CenterRow, HTuple  CenterColumn, int RingWidth, HTuple* tuple_RegMean_DCoffset)
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
double CBatteryShellAlgo::Sinc(double n)
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
void CBatteryShellAlgo::Unit_Impulse_Response(int paraLength, double angFreqStart, double angFreqCutoff, int winType, std::vector<double>& vFilterParam)
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
void CBatteryShellAlgo::BandPass_Filter(std::vector<double>& vFilterParam, int ParaLength, std::vector<double>& vInputData, std::vector<double>& vOutputData)
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
void CBatteryShellAlgo::PhaseAdjust(std::vector<double>& vOutputData, int FilterWidth)
{
	int outputSz = (int)vOutputData.size();
	std::vector<double> vOutTemp1;
	vOutTemp1.reserve(outputSz);

	vOutTemp1.assign(vOutputData.begin() + (outputSz-FilterWidth),  vOutputData.begin() + outputSz);
	vOutTemp1.insert(vOutTemp1.end(), vOutputData.begin(), vOutputData.begin() + outputSz - FilterWidth);
	std::copy(vOutTemp1.begin(), vOutTemp1.begin() + outputSz, vOutputData.begin());
}
void CBatteryShellAlgo::Tuple_Replace(HTuple& tuple, HTuple Indices, HTuple ReplaceValue)
{
	HTuple  Length, SelectedIndice;
	tuple_length(Indices, &Length);
	for (int i = 0; i < Length; i++)
	{
		tuple_select(Indices, i, &SelectedIndice);
		tuple[SelectedIndice] = ReplaceValue;
	}
}

STDMETHODIMP CBatteryShellAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pWinType, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	//利用带通滤波器的卷边优化算法
	int		rId				= (int)rgnId->fltVal;
	int		WinType			= (int)pWinType->fltVal;
	int		DefectType		= (int)pDefectType->fltVal;
	int		MinArea			= (int)pMinArea->fltVal;
	double	FreqStart		= (double)pFreqStart->fltVal;
	double	FreqCutoff		= (double)pFreqCutoff->fltVal;
	float	WaveThreshold	= (float)pWaveThreshold->fltVal;
	FreqStart	= FreqStart/100;
	FreqCutoff	= FreqCutoff/100;

	Hobject rgn = m_vRgn[rId];
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_dictSpecialData[rId].clear();
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}

	std::vector<double>  vInputData;  
	std::vector<double>  vOutputData;
	std::vector<double>  vFilterParam;

	float RoiWidth = m_ringPara[rId].m_RoiWidth;
	float Radius = m_ringPara[rId].m_Radius;

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
		Hobject RegionDifference_ext, ImageReduced_ext;
		Hobject  Ring, ZoningReg, MiddleCircle;
		HTuple MeanDiffValues, ImageWidth, ImageHeight, Area, Row_test, Column_test, Length;
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
		tuple_length(fCenterY, &Length);
		if (Length <= 0)
		{
			return S_FALSE;
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
				m_dictSpecialData[rId].push_back(WaveThreshold * YScaleFactor);
				m_dictSpecialData[rId].push_back((-1)* WaveThreshold * YScaleFactor);
				break;
			case LIGHT:
				m_dictSpecialData[rId].push_back(WaveThreshold * YScaleFactor);
				m_dictSpecialData[rId].push_back(-999999.0f);
				break;
			case DARK:
				m_dictSpecialData[rId].push_back(999999.0f);
				m_dictSpecialData[rId].push_back((-1)* WaveThreshold * YScaleFactor);
				break;
			default:
				break;
			}
			for(int i = 0; i < InputDataLength; i++)
			{
				m_dictSpecialData[rId].push_back(float(BandpassMeanValue[i].D()*YScaleFactor*(-1)));
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
			m_vErrorRgn[rId] = DefectRegs;
		}
		else
		{
			retValue = 0.0; 
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}		
		retValue.Detach(pDetectNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectNumber);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBatteryShellAlgo::CurlDetectAlgo2(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
{
	//盖环形区域算法2
	int		rId				= (int)rgnId->fltVal;
	int		NumPart			= (int)pNumPart->fltVal;
	int		GrayValue		= (int)pGrayValue->fltVal;
	float	ThresholdValue	= (float)pThresholdValue->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn		= m_vRgn[rId];
	float RoiWidth	= m_ringPara[rId].m_RoiWidth;
	float Radius	= m_ringPara[rId].m_Radius;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::CurlDetectAlgo2Help(BSTR* pHelpStr)
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
STDMETHODIMP CBatteryShellAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
{
	int rId		= (int)rgnId->fltVal;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue=-1;
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pGrayValueMean);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1;<=#20$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::UVCurlAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea)
{
	//UVCurlAlgo  对应第三版UVCurlDLL2.1
	int		rId				= (int)rgnId->fltVal;
	int		NumPart			= (int)pNumPart->fltVal;
	float	ThresholdValue	= (float)pThresholdValue->fltVal;
	int		GrayValue		= (int)pGrayValue->fltVal;
	int		MeanGrayValue	= (int)pMeanGrayValue->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	HTuple Row6, Column6, RadiusOut,Row8, Column8, HalfRoiWidth;
	smallest_circle (rgn, &Row6, &Column6, &RadiusOut);
	inner_circle (rgn, &Row8, &Column8, &HalfRoiWidth);

	float RoiWidth	=(float)(2.0*HalfRoiWidth[0].D());
	float Radius	=(float)RadiusOut[0].D();
	if((Radius<RoiWidth*2.1)||(RoiWidth<1.2)||(Radius<RoiWidth*2.1)||(RoiWidth<1.6))
	{
		retValue = -1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}


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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::UVCurlAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBatteryShellAlgo::DynThresholdAlgo(VARIANT* rgnId,VARIANT* pIsShowInterImg, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int		rId							= (int)rgnId->fltVal;
	int		IsShowInterImg				= (int)pIsShowInterImg->fltVal;
	int		BlackMaskSize				= (int)pBlackMaskSize->fltVal;
	int		SeriousBlackPointSize		= (int) pSeriousBlackPointSize->fltVal;
	int		WhiteMaskSize				= (int) pWhiteMaskSize->fltVal;
	int		SeriousWhitePointSize		= (int) pSeriousWhitePointSize->fltVal;
	float	SeriousBlackPointDynThresh	= (float)pSeriousBlackPointDynThresh->fltVal;
	float	SeriousWhitePointDynThresh	= (float) pSeriousWhitePointDynThresh->fltVal;

	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
		Hobject Image,EmptyObj;
		m_vInterImgs[rId]=EmptyObj;
		Image = m_crtImg;
		if(IsShowInterImg && m_originImageMean.Id()!=H_EMPTY_REGION)
		{
			m_vInterImgs[rId]=m_originImageMean;
			Image = m_originImageMean;
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
			if(RelativeHisto.Num()>0 &&AbsoluteHisto.Num()>0)
			{
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
			dilation_circle (BlackUnion, &BlackUnion, 2.5);
			m_vErrorRgn[rId] = BlackUnion;
			dilation_circle (WhiteUnion, &WhiteUnion, 2.5);
			m_vWhiteErrorRgn[rId] = WhiteUnion;	
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("411$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 5023;LP;0;1;1;1$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;60$\
					 416;LP;1;9999;1;10$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;9999;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DynThresholdAlgoBlack(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
{
	int		rId							= (int)rgnId->fltVal;
	int		BlackMaskSize				= (int)pBlackMaskSize->fltVal;
	float	SeriousBlackPointDynThresh	= (float)pSeriousBlackPointDynThresh->fltVal;
	int		SeriousBlackPointSize		= (int) pSeriousBlackPointSize->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
			m_dictSpecialData[rId].push_back(float(SeriousBlackPointDynThresh));
			m_dictSpecialData[rId].push_back(-999999.0f);
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub;
			get_image_size (Image,& Width, &Height);
			sub_image (ImageReduceMean,ImageReduced,  &ImageSub, 1, 0);
			gray_histo (rgn, ImageSub, &AbsoluteHisto, &RelativeHisto);
			if(RelativeHisto.Num()>0 &&AbsoluteHisto.Num()>0)
			{
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

		}
		//
		if(blackArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = blackArea[0].I();
			dilation_circle (BlackUnion, &BlackUnion, 2.5);
			m_vErrorRgn[rId] = BlackUnion;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DynThresholdAlgoBlackHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("492$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 412;LP;3;101;2;7$\
					 415;LP;1;250;1;50$\
					 416;LP;1;9999;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DynThresholdAlgoWhite(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	int		rId							= (int)rgnId->fltVal;
	int		WhiteMaskSize				= (int) pWhiteMaskSize->fltVal;
	int		SeriousWhitePointSize		= (int) pSeriousWhitePointSize->fltVal;
	float	SeriousWhitePointDynThresh	= (float) pSeriousWhitePointDynThresh->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
			if(RelativeHisto.Num()>0 &&AbsoluteHisto.Num()>0)
			{
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
			m_vWhiteErrorRgn[rId] = WhiteUnion;
		}
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DynThresholdAlgoWhiteHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("493$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 417;LP;3;101;2;7$\
					 420;LP;1;250;1;60$\
					 421;LP;1;9999;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CBatteryShellAlgo::GSurfaceAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
{
	//盖面垫模算法
	int rId				= (int)rgnId->fltVal;
	int BlackThred		= (int)pBlackThred->fltVal;
	int BlackArea		= (int)pBlackAreaThred->fltVal;
	int WhiteThred		= (int)pWhiteThred->fltVal;
	int WhiteArea		= (int)pWhiteAreaThred->fltVal;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	Hobject imgRect;
	gen_rectangle1(&imgRect, 0, 0, m_imgHeight-1, m_imgWidth-1);
	intersection(imgRect, rgn, &rgn);

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
			if(Rows.Num()>0)
			{
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
				if(RelativeHisto.Num()>0 &&AbsoluteHisto.Num()>0)
				{
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GSurfaceAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBatteryShellAlgo::GSurfaceUnionAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pWhiteThred,VARIANT* pLengthFilterSize,  VARIANT* pErroreAreaThred, VARIANT* pDetectArea)
{
	//针对垫膜算法进行修改，将黑白缺陷union到一起去筛选
	int rId					= (int)rgnId->fltVal;
	int BlackThred			= (int)pBlackThred->fltVal;
	int WhiteThred			= (int)pWhiteThred->fltVal;
	int LengthFilterSize	= (int)pLengthFilterSize->fltVal;
	int ErroreAreaThred		= (int)pErroreAreaThred->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	Hobject imgRect;
	gen_rectangle1(&imgRect, 0, 0, m_imgHeight-1, m_imgWidth-1);
	intersection(imgRect, rgn, &rgn);


#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		Hobject ImageReduced, ExpandedImage, DerivGauss,DerivGaussInver,ImageGauss;
		Hobject Regions1, Regions2, RegionUnion,SelectedRegions,ConnectedRegions;
		Hobject RegionDilation,RegionIntersection,RegionIntersection1,RegionIntersection2;

		Hobject  ConnectedRegions1, SelectedRegions1, BlackUnion, WhiteUnion;
		Hobject  ConnectedRegions2, SelectedRegions2,DefectRegs,EmptyObj;
		HTuple Number;
		Hobject Image = m_crtImg;
		m_vInterImgs[rId]=EmptyObj;

		gauss_filter (Image, &ImageGauss, 5);
		reduce_domain (ImageGauss, rgn, &ImageReduced);
		expand_domain_gray(ImageReduced, &ExpandedImage, 35);
		derivate_gauss(ExpandedImage, &DerivGaussInver,9, "mean_curvature");
		invert_image (DerivGaussInver, &DerivGauss);
		reduce_domain(DerivGauss, rgn, &ImageReduced);


		threshold(ImageReduced, &Regions1, -1,(-1)* BlackThred/1000.0);
		threshold(ImageReduced, &Regions2, WhiteThred/1000.0 ,1);

		//剔除宽度大于一定值的区域
		union2 (Regions1, Regions2, &RegionUnion);
		connection (RegionUnion, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions, "outer_radius", "and", 0,LengthFilterSize);
		union1 (SelectedRegions, &RegionUnion);
		dilation_circle (RegionUnion, &RegionDilation, 15.5);
		connection (RegionDilation, &ConnectedRegions);
		intersection (ConnectedRegions, RegionUnion, &RegionIntersection);
		count_obj (RegionIntersection, &Number);
		if(Number.Num()>=1)
		{
			HTuple Area1, Row1, Column1,Area2, Row2, Column2,Greater1,Greater2;
			HTuple TupleAnd,TupleIndices;
			intersection (RegionIntersection, Regions1, &RegionIntersection1);
			area_center (RegionIntersection1, &Area1, &Row1, &Column1);
			intersection (RegionIntersection, Regions2, &RegionIntersection2);
			area_center (RegionIntersection2, &Area2, &Row2, &Column2);
			tuple_greater_elem (Area1, 0, &Greater1);
			tuple_greater_elem (Area2, 0, &Greater2);
			if(Greater1.Num()>=1 && Greater2.Num()>=1)
			{
				tuple_and (Greater1, Greater2, &TupleAnd);
				tuple_find (TupleAnd, 1, &TupleIndices);

				if(TupleIndices!=-1)
				{
					select_obj (RegionIntersection, &RegionIntersection, TupleIndices+1);
				}
			}

		}
		select_shape(RegionIntersection, &SelectedRegions, "area", "and", ErroreAreaThred, 99999);
		union1(SelectedRegions, &DefectRegs);
		if (m_bDebugImg)
		{
			HTuple Rows, Columns,Grayvals,Max,Min;
			get_region_points (rgn, &Rows, &Columns);        
			get_grayval (ImageReduced, Rows, Columns,& Grayvals);
			if(Rows.Num()>0)
			{
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
				if(RelativeHisto.Num()>0 &&AbsoluteHisto.Num()>0)
				{
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
			}	
		}

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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GSurfaceUnionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5010$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 407;LP;1;999;1;100$\
					 409;LP;1;999;1;999$\
					 5011;LP;1;9999;1;100$\
					 5012;LP;1;250;1;250");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int	rId			= (int)rgnId->fltVal;
	int EdgeThred	= (int)pEdgeThred->fltVal;
	int DefectSize	= (int)pDefectSize->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
			if(RelativeHisto.Num()>0 &&AbsoluteHisto.Num()>0)
			{
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("432$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 433;LP;1;255;1;30$\
					 434;LP;1;99999;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GContoursinkAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
{
	//盖面嵌环算法
	int rId			= (int)rgnId->fltVal;
	int BlackThred	= (int)pBlackThred->fltVal;
	int BlackArea	= (int)pBlackAreaThred->fltVal;
	int WhiteThred	= (int)pWhiteThred->fltVal;
	int WhiteArea	= (int)pWhiteAreaThred->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	Hobject imgRect;
	gen_rectangle1(&imgRect, 0, 0, m_imgHeight-1, m_imgWidth-1);
	intersection(imgRect, rgn, &rgn);

#ifdef NDEBUG	
	try
	{
#endif	
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
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
			m_dictSpecialData[rId].push_back(float( (-1)*BlackThred) );
			m_dictSpecialData[rId].push_back(float(WhiteThred));
			m_dictSpecialData[rId].push_back(float(Min[0].D()*100.0));
			m_dictSpecialData[rId].push_back(float(Max[0].D()*100.0));
			HTuple Sum,Log,Prod,AbsoluteHisto, RelativeHisto, Length;
			Hobject ImageScaleMax;
			scale_image_max (DerivGauss, &ImageScaleMax);
			gray_histo (rgn, ImageScaleMax, &AbsoluteHisto, &RelativeHisto);   
			if(RelativeHisto.Num()>0 &&AbsoluteHisto.Num()>0)
			{
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::GContoursinkAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CBatteryShellAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
	//检测小黑白点缺陷
	int		rId							= (int)rgnId->fltVal;
	float	MaskSize					= (float)fabs(pMaskSize->fltVal);
	float	SmoothSize					= (float) fabs(pSmoothSize->fltVal);
	int		DefectType					= (int)pDefectType->fltVal;
	float	SeriousBlackPointDynThresh	= (float)(pSeriousBlackPointDynThresh->fltVal/100.0);
	int		SeriousBlackPointSize		= (int) pSeriousBlackPointSize->fltVal;
	float	SeriousWhitePointDynThresh	= (float) (-pSeriousWhitePointDynThresh->fltVal/100.0);
	int		SeriousWhitePointSize		= (int) pSeriousWhitePointSize->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CBatteryShellAlgo::WaterSpotsDetectAlgo(VARIANT* rgnId, VARIANT* pGrayThred, VARIANT* pBlackThred, VARIANT* pAreaThred, VARIANT* pDetectArea)
{
	//水渍检测算法，默认水渍为斑状，有一定尺寸和圆度。
	int	rId			= (int)rgnId->fltVal;
	int GrayThred	= (int)pGrayThred->fltVal;
	int BlackThred	= (int)pBlackThred->fltVal;
	int AreaThred	= (int)pAreaThred->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
		Hobject Image = m_crtImg;
		Hobject ImageReduced, BrightRegion, ImageResult, ImageMean, DerivGauss, CandidateRegion, ConnectedRegions1, Skeletons;
		Hobject ConnectedRegions2, RegionTrans, RegionDifference, ConnectedRegions3, SelectedRegions1, SelectedRegions2, RegionDilation;
		Hobject RegionUnion, RegionIntersection, ConnectedRegions4, SelectedRegions3, SelectedRegions4, SelectedRegions5;
		HTuple Mean, Deviation, GaussMean, GaussDeviation;
		reduce_domain(Image, rgn , &ImageReduced);
		intensity(rgn, ImageReduced, &Mean, &Deviation);
		if(!(Mean.Num() > 0))
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		threshold(ImageReduced, &BrightRegion, Mean[0].D(), 255);
		paint_region(BrightRegion, ImageReduced, &ImageResult, Mean[0].D(), "fill");
		mean_image(ImageResult, &ImageMean, 5, 5);
		derivate_gauss(ImageMean, &DerivGauss, 3, "eigenvalue1");
		intensity (rgn, DerivGauss, &GaussMean, &GaussDeviation);
		threshold(DerivGauss, &CandidateRegion, 0.2, 255);
		connection(CandidateRegion, &ConnectedRegions1);
		skeleton(CandidateRegion, &Skeletons);
		connection(Skeletons, &ConnectedRegions2);
		shape_trans(ConnectedRegions2, &RegionTrans, "convex");
		difference(RegionTrans, CandidateRegion, &RegionDifference);
		connection(RegionDifference, &ConnectedRegions3);
		select_shape(ConnectedRegions3, &SelectedRegions1, "circularity", "and", 0.4, 1);
		select_shape(SelectedRegions1, &SelectedRegions2, "area", "and", 80, 99999999);
		dilation_circle(SelectedRegions2, &RegionDilation, 10.5);
		intersection(rgn, RegionDilation, &RegionDilation);
		union1(RegionDilation, &RegionUnion);
		intersection(RegionUnion, ConnectedRegions2, &RegionIntersection);
		connection(RegionIntersection, &ConnectedRegions4);
		double GrayDiffValue = GrayThred/200.0;
		select_gray(ConnectedRegions4, DerivGauss, &SelectedRegions3, "mean", "and", GrayDiffValue + GaussMean[0].D(), 255);
		select_gray(SelectedRegions3, ImageResult, &SelectedRegions4, "mean", "and", 0, BlackThred);
		select_shape(SelectedRegions4, &SelectedRegions5, "area", "and", AreaThred, 99999999);
		
		Hobject DefectRegs;
		HTuple Area, Row_test, Column_test;
		union1(SelectedRegions5, &DefectRegs);
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::WaterSpotsDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4027$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 1091;LP;0;120;1;48$\
					 405;LP;1;255;1;90$\
					 387;LP;1;9999;1;60");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::DiffRegionMaxWidthDetectAlgo(VARIANT* rgnId, VARIANT* pDetectWidth)
{
	//计算口部变形 提取的边缘和圆形的定位结果之间的差异
	int rId = (int)rgnId->fltVal;

	CComVariant retValue;
	if(m_centerLocationRegion.Id() == H_EMPTY_REGION||m_centerLocationCircle.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectWidth);
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
		Hobject RegionDifference,Circle;
		HTuple Row1, Column1, Radius1,MaxValue;
		difference (m_centerLocationCircle,m_centerLocationRegion, &RegionDifference);
		inner_circle (RegionDifference, &Row1, &Column1, &Radius1);
		if(Radius1.Num()>0)
		{
			tuple_max (Radius1, &MaxValue);
			MaxValue = MaxValue*2;
		}
		else
		{
			MaxValue = 0;
		}
		retValue = MaxValue[0].I();
		m_vErrorRgn[rId] = RegionDifference;

		retValue.Detach(pDetectWidth);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		Hobject newRgn;
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DiffRegionMaxWidthDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1279$\
					 397;R;LP;0;9000;1;>=#20$\
					 167;B"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::CircleIncisionShapeChangeDetectAlgo(VARIANT* rgnId, VARIANT* pDetectWidth)
{
	//提取口部边缘区域，检测形变缺陷。
	int rId = (int)rgnId->fltVal;

	Hobject rgn = m_vRgn[rId];
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectWidth);
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
		Hobject RegionFillU, ImageReduced, ImageMedian, Region, ConnectedRegions, RegionOpenings, ConnectedRegions1, SelectedRegion, RegionFillU1;
		Hobject RegionOpening, OuterCircleTrans, InnerCircleTrans, Circle;
		HTuple Mean, Deviation, GrayThred, OuterRow, OuterColumn, OuterRadius, innerRow, innerColumn, innerRadius, DiffRadius;
		Hobject Image = m_crtImg;
		fill_up(rgn, &RegionFillU);
		reduce_domain (Image, RegionFillU, &ImageReduced);
		intensity (ImageReduced, Image, &Mean, &Deviation);
		if (!(Mean.Num() > 0))
		{
			retValue = -1;
			retValue.Detach(pDetectWidth);
			return S_FALSE;
		}
		GrayThred = Mean[0].D() + Deviation[0].D();
		median_image(ImageReduced, &ImageMedian, "circle", 1.5, "mirrored");
		threshold(ImageMedian, &Region, 0, GrayThred);
		connection(Region, &ConnectedRegions);
		opening_circle(ConnectedRegions, &RegionOpenings, 3.5);
		connection(RegionOpenings, &ConnectedRegions1);
		select_shape_std(ConnectedRegions1, &SelectedRegion, "max_area", 70);
		fill_up(SelectedRegion, &RegionFillU1);
		opening_circle(RegionFillU1, &RegionOpening, 5.5);
		shape_trans(RegionOpening, &OuterCircleTrans, "outer_circle");
		shape_trans(RegionOpening, &InnerCircleTrans, "inner_circle");
		smallest_circle(OuterCircleTrans, &OuterRow, &OuterColumn, &OuterRadius);
		smallest_circle(InnerCircleTrans, &innerRow, &innerColumn, &innerRadius);
		if((OuterRadius.Num()>0)||(innerRadius.Num()>0))
		{
			DiffRadius = OuterRadius[0].D() - innerRadius[0].D();
			retValue = abs(DiffRadius[0].D())*2;
			m_vErrorRgn[rId] = RegionOpening;
		}
		else
		{
			retValue = 0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		retValue.Detach(pDetectWidth);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		Hobject newRgn;
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::CircleIncisionShapeChangeDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1279$\
					 397;R;FP;0;20;0.5;<=#5$\
					 167;B"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::ShoulderRingDeformationDetectAlgo(VARIANT* rgnId, VARIANT* pGrayValue, VARIANT* pRingWidth, VARIANT* pDetectWidth)
{
	//提取肩部环状区域，检测形变缺陷。
	int rId					= (int)rgnId->fltVal;
	int GrayValue			= (int)pGrayValue->fltVal;
	int RingWidth			= (int)pRingWidth->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	Hobject Image = m_crtImg;
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectWidth);
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
		
		Hobject ImageReduced, Region, ConnectedRegions, SelectedRegions, RegionFillUp, SelectedRegions1, RegionDifference, RegionOpening1, RegionOpening2, RegionTrans1, RegionTrans2;
		Hobject RegionDifference1, RegionTrans, RegionTrans3, RegionTrans4, RegionDifference2, RegionTrans5, RegionDifference3;
		HTuple Mean, Deviation, TempAre, TempRow1, TempColumn1, TempRow2, TempColumn2, TempRadius, TempAre3, TempRow3, TempColumn3, Row1, Column1, Radius1, Row2, Column2, Radius2;
		area_center(rgn, &TempAre, &TempRow1, &TempColumn1);
		if (TempAre <= 0)
		{
			return S_OK;
		}
		reduce_domain (Image, rgn, &ImageReduced);
		intensity (rgn, Image, &Mean, &Deviation);
		if (GrayValue < Mean)
		{
			threshold (ImageReduced, &Region, 0, GrayValue);
		} 
		else
		{
			threshold (ImageReduced, &Region, GrayValue, 255);
		}
		area_center(Region, &TempAre, &TempRow1, &TempColumn1);
		smallest_circle(rgn, &TempRow2, &TempColumn2, &TempRadius);
		if ((TempAre <= 0)||(RingWidth > TempRadius))
		{
			return S_OK;
		}
		connection (Region, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions, "outer_radius", "and", TempRadius-RingWidth, TempRadius);
		fill_up (SelectedRegions, &RegionFillUp);
		select_shape_std (RegionFillUp, &SelectedRegions1, "max_area", 70);
		difference (SelectedRegions1, SelectedRegions, &RegionDifference);
		area_center(RegionDifference, &TempAre3, &TempRow3, &TempColumn3);
		if (TempAre3 <= 0)
		{
			return S_OK;
		}
		opening_circle (RegionDifference, &RegionOpening1, 3.5);
		opening_circle (SelectedRegions1, &RegionOpening2, 3.5);
		difference (RegionOpening2, RegionOpening1, &RegionDifference3);
		shape_trans (RegionOpening2, &RegionTrans1, "outer_circle");
		shape_trans (RegionOpening2, &RegionTrans2, "inner_circle");
		difference (RegionTrans1, RegionTrans2, &RegionDifference1);
		shape_trans (RegionDifference1, &RegionTrans, "inner_circle");
		smallest_circle (RegionTrans, &Row1, &Column1, &Radius1);
		if (Radius1[0].D() <= 0)
		{
			Radius1 = 0;
		}
		shape_trans (RegionOpening1, &RegionTrans3, "outer_circle");
		shape_trans (RegionOpening1, &RegionTrans4, "inner_circle");
		difference (RegionTrans3, RegionTrans4, &RegionDifference2);
		shape_trans (RegionDifference2, &RegionTrans5, "inner_circle");
		smallest_circle (RegionTrans5, &Row2, &Column2, &Radius2);
		if (Radius2[0].D() <= 0)
		{
			Radius2 = 0;
		}
		retValue = Radius2[0].D() + Radius2[0].D();
		concat_obj(m_vRgnExTest[rId], RegionDifference3, &m_vRgnExTest[rId]);
		retValue.Detach(pDetectWidth);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		Hobject newRgn;
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::ShoulderRingDeformationDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4039$\
					 397;R;FP;0;50;0.5;<=#4$\
					 381;B$\
					 4040;LP;0;255;1;95$\
					 4041;LP;0;250;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::DentMeanCurveAlgo(VARIANT* rgnId, VARIANT* pFilterSize, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
{
	//压伤曲率算法
	int rId			= (int)rgnId->fltVal;
	int FilterSize	= (int)pFilterSize->fltVal;
	int BlackThred	= (int)pBlackThred->fltVal;
	int BlackArea	= (int)pBlackAreaThred->fltVal;
	int WhiteThred	= (int)pWhiteThred->fltVal;
	int WhiteArea	= (int)pWhiteAreaThred->fltVal;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	Hobject ResultRegionBlack,ResultRegionWhite;
	Hobject Image = m_crtImg;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		ImageProcessAlg.DentDetect( Image, rgn, FilterSize, BlackThred, WhiteThred,&ResultRegionBlack, &ResultRegionWhite);

		Hobject ConnectedRegions,SelectedRegions,BlackUnion,WhiteUnion,DefectRegs;
		HTuple Area, Row_test, Column_test;
		connection(ResultRegionBlack, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", BlackArea, 99999);
		union1(SelectedRegions, &BlackUnion);

		connection(ResultRegionWhite, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", WhiteArea, 99999);
		union1(SelectedRegions, &WhiteUnion);
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DentMeanCurveAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("424$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 314;LP;1;15;1;10$\
					 407;LP;1;999;1;15$\
					 408;LP;1;250;1;50$\
					 409;LP;1;999;1;50$\
					 410;LP;1;250;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::DetectTheOCRSectorRegionAlgo(VARIANT* rgnId, VARIANT* pThresholdValue, VARIANT* pCentralAngle, VARIANT* pOCRCenterDirection)
{
	//用于在非模板工具状态下准确找到OCR区域的中心位置，计算出其相对圆心的的方向，画出一个包含OCR的扇形区域并传出，显示该扇形与区域算子的交集区域和相对于圆心的方向。
	int rId				= (int)rgnId->fltVal;
	int ThresholdValue	= (int)pThresholdValue->fltVal;
	float CentralAngle	= (float)pCentralAngle->fltVal;
	CentralAngle = CentralAngle/180*PI;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_OCRRgnAngle.Reset();
	m_OCRSectorRgn.Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pOCRCenterDirection);
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
		Hobject Image, ImageGauss, ImageReasult, Region, RegionIntersection, RegionDilation, RegionClosing, RegionFillUp, ConnectedRegions, RegionOpening, ConnectedRegions1;
		Hobject SelectedRegions, SelectedRegionsMax, CircleSector, RegionIntersection1;
		HTuple Row, Column, Radius;
		HTuple AreaTemp, RowTemp, ColumnTemp, AreaTemp1, RowTemp1, ColumnTemp1, MaxRegionsArea, MaxRegionsRow, MaxRegionsColumn;
		HTuple ATan, ATAN, AngleR, AngleStar, AngleEnd;
		Image = m_crtImg;
		if( m_originImageMean.Id()!=H_EMPTY_REGION )
		{
			m_vInterImgs[rId]=m_originImageMean;
			Image = m_originImageMean;
		}
		smallest_circle(rgn, &Row, &Column, &Radius);
		gauss_filter(Image, &ImageGauss, 7);
		gray_range_rect(ImageGauss, &ImageReasult, 7, 7);
		threshold(ImageReasult, &Region, ThresholdValue, 255);
		area_center (Region, &AreaTemp, &RowTemp, &ColumnTemp);
		ATAN = 0.0;   //The initial value ： 0°
		if (AreaTemp > 0)
		{
			intersection(Region, rgn, &RegionIntersection);
			dilation_circle(RegionIntersection, &RegionDilation, 2.5);
			closing_circle(RegionDilation, &RegionClosing, 3.5);
			fill_up_shape (RegionClosing, &RegionFillUp, "area", 1, 500);
			connection (RegionFillUp, &ConnectedRegions);
			opening_circle (ConnectedRegions, &RegionOpening, 5.5);
			connection (RegionOpening, &ConnectedRegions1);
			area_center (ConnectedRegions1, &AreaTemp1, &RowTemp1, &ColumnTemp1);
			if (AreaTemp1>0)
			{
				select_shape (ConnectedRegions1, &SelectedRegions, "outer_radius", "and", 5, Radius/2);
				select_shape_std (SelectedRegions, &SelectedRegionsMax, "max_area", 70);
				area_center (SelectedRegionsMax, &MaxRegionsArea, &MaxRegionsRow, &MaxRegionsColumn);
				if((MaxRegionsArea > 0)&&(MaxRegionsArea.Num()==1))
				{
					tuple_atan2 (MaxRegionsRow-Row, MaxRegionsColumn-Column, &ATan);//Positioning success
					/***********坐标系角度转换**********/
					(ATan[0].D()<0)? ATAN = ATan.Abs() : ATAN = 2*PI-ATan;
					/********************************/
				} 
			}
		}
		m_OCRRgnAngle = ATAN[0].D();//0-2PI
		retValue = ATAN[0].D()/PI*180;
		retValue.Detach(pOCRCenterDirection);
		AngleR = CentralAngle/2;

		/**********角度区间转换**********/
		if ((ATAN - AngleR)<0)
		{
			AngleStar = ATAN + 2*PI-AngleR;
			AngleEnd = ATAN + AngleR;
		} 
		else if((ATAN + AngleR)>2*PI)
		{
			AngleStar = ATAN - AngleR;
			AngleEnd = ATAN - 2*PI+AngleR;
		}
		else
		{
			AngleStar = ATAN - AngleR;
			AngleEnd = ATAN + AngleR;
		}
		/********************************/
		gen_circle_sector (&CircleSector, Row, Column, 2*Radius, AngleStar, AngleEnd);
		intersection (rgn, CircleSector, &RegionIntersection1);
		union2(m_vRgnExTest[rId], RegionIntersection1, &m_vRgnExTest[rId]);
		m_OCRSectorRgn = CircleSector;
		
		
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pOCRCenterDirection);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DetectTheOCRSectorRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4019$\
					 4022;R;FP;0;360;1;>=#0$\
					 381;B$\
					 4020;LP;1;255;1;55$\
					 4021;FP;0;360;1;23");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::DynDetectTheOCRSectorRegionAlgo(VARIANT* rgnId, VARIANT* pThresholdValue, VARIANT* pCentralAngle, VARIANT* pOCRCenterDirection)
{
	//滤波处理后动态阈值检测OCR区域
	//用于在非模板工具状态下准确找到OCR区域的中心位置，计算出其相对圆心的的方向，画出一个包含OCR的扇形区域并传出，显示该扇形与区域算子的交集区域和相对于圆心的方向。
	int rId				= (int)rgnId->fltVal;
	int ThresholdValue	= (int)pThresholdValue->fltVal;
	float CentralAngle	= (float)pCentralAngle->fltVal;
	CentralAngle = CentralAngle/180*PI;

	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_OCRRgnAngle.Reset();
	m_OCRSectorRgn.Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		retValue.Detach(pOCRCenterDirection);
		return S_FALSE;
	}
	HTuple AreaTemp0, RowTemp0, ColumnTemp0;
	area_center (rgn, &AreaTemp0, &RowTemp0, &ColumnTemp0);
	if(AreaTemp0 <= 1)
	{
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
		Hobject Image = m_crtImg;
		if( m_originImageMean.Id()!=H_EMPTY_REGION )
		{
			m_vInterImgs[rId]=m_originImageMean;
			Image = m_originImageMean;
		}
		
		Hobject ImageReduced, PolarTransImage, ImageMedian, DerivGauss, ImageMean, RegionDynThresh, ConnectedRegions;
		Hobject RegionDilation, RegionUnion, ConnectedRegions1, SelectedRegions, CircleSector, RegionIntersection1;
		HTuple InnerRadius, OuterRadius;
		HTuple Row11, Column11, Row12, Column12;
		HTuple AreaTemp, RowTemp, ColumnTemp, AreaTemp1, RowTemp1, ColumnTemp1;
		HTuple ATAN, AngleR, AngleStar, AngleEnd;
		HTuple SmoothSize = 9;
		ATAN = 0.0;
		region_features (rgn, "inner_radius", &InnerRadius);
		region_features (rgn, "outer_radius", &OuterRadius);
		float RoiWidth = InnerRadius[0].D()*2;
		float OuterR = OuterRadius[0].D();
		if (OuterR - RoiWidth < 1)
		{
			RoiWidth = OuterR;
		}
		float InterR = OuterR - RoiWidth;
		float SpreadAngle =  PI*2.2;  //1.1 circumferential
		float OuterPerimeter = SpreadAngle*OuterR;
		float fCenterX = 0.0;
		float fCenterY = 0.0;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		reduce_domain(Image, rgn, &ImageReduced);
		polar_trans_image_ext(ImageReduced, &PolarTransImage, fCenterY, fCenterX,  0, SpreadAngle, InterR, OuterR, OuterPerimeter, RoiWidth, "nearest_neighbor");
		if (SmoothSize < InnerRadius)
		{
			median_rect(PolarTransImage, &ImageMedian, SmoothSize, SmoothSize); //Remove salt and pepper noise
			derivate_gauss(ImageMedian, &DerivGauss, 5, "gradient");
			mean_image(DerivGauss, &ImageMean, 200, 3);
			float TValue = ThresholdValue/50.0;
			dyn_threshold(DerivGauss, ImageMean, &RegionDynThresh, TValue, "light");
			area_center (RegionDynThresh, &AreaTemp, &RowTemp, &ColumnTemp);
			if (AreaTemp > 0)
			{
				connection(RegionDynThresh, &ConnectedRegions);
				dilation_rectangle1 (ConnectedRegions, &RegionDilation, 40, 20);
				union1 (RegionDilation, &RegionUnion);
				connection (RegionUnion, &ConnectedRegions1);
				select_shape_std (ConnectedRegions1, &SelectedRegions, "max_area", 70);
				area_center (SelectedRegions, &AreaTemp1, &RowTemp1, &ColumnTemp1);
				if((AreaTemp1 > 0)&&(AreaTemp1.Num()==1))
				{
					smallest_rectangle1 (SelectedRegions, &Row11, &Column11, &Row12, &Column12);
					float LocationCenter = (Column11[0].D()+Column12[0].D())/2.0;
					ATAN = SpreadAngle*LocationCenter/(OuterPerimeter-1);
					if ((ATAN>(PI*2.0))&&(ATAN<(PI*4.0)))
					{
						ATAN = ATAN-(PI*2.0);
					}
				}
			}
		}
		m_OCRRgnAngle = ATAN[0].D();//0-2PI
		retValue = ATAN[0].D()/PI*180;
		retValue.Detach(pOCRCenterDirection);
		AngleR = CentralAngle/2;

		/**********角度区间转换**********/
		if ((ATAN - AngleR)<0)
		{
			AngleStar = ATAN + 2*PI-AngleR;
			AngleEnd = ATAN + AngleR;
		} 
		else if((ATAN + AngleR)>2*PI)
		{
			AngleStar = ATAN - AngleR;
			AngleEnd = ATAN - 2*PI+AngleR;
		}
		else
		{
			AngleStar = ATAN - AngleR;
			AngleEnd = ATAN + AngleR;
		}
		/********************************/
		gen_circle_sector (&CircleSector, fCenterY, fCenterX, OuterRadius, AngleStar, AngleEnd);
		intersection (rgn, CircleSector, &RegionIntersection1);
		union2(m_vRgnExTest[rId], RegionIntersection1, &m_vRgnExTest[rId]);
		m_OCRSectorRgn = CircleSector;


#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pOCRCenterDirection);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::DynDetectTheOCRSectorRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4019$\
					 4022;R;FP;0;360;1;>=#0$\
					 381;B$\
					 4020;LP;1;255;1;55$\
					 4021;FP;0;360;1;23");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::DisOCRCircleRegionAngleAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth, VARIANT* pDriftAngle1, VARIANT* pDriftAngle2, VARIANT* pDisOCR)
{
	int		id					= (int)rgnId->fltVal;
	float	DriftAngle1			= (float)pDriftAngle1->fltVal;
	float	DriftAngle2			= (float)pDriftAngle2->fltVal;
	int		DisOCR				= (int)pDisOCR->fltVal;
	m_ringPara[id].m_Radius		= (float)pRadius->fltVal;
	m_ringPara[id].m_RoiWidth	= (float)pRoiWidth->fltVal;

	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	float m_RadiusInner = Radius - RoiWidth;

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
	Hobject  Circle1_Inner, Circle2_EXT, OuterSector;
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
		Hobject  OCRSectorRgn;
		if (DisOCR == 1)
		{
			OCRSectorRgn = m_OCRSectorRgn;
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
				HTuple StartAngle =(DriftAngle1/360.0)*2*PI;
				HTuple EndAngle =(DriftAngle2/360.0)*2*PI;


				if (ImgSize[0].I() < 5e5)
				{
					gen_rectangle1(&Rectangle1, 0, 0, RoiWidth, WidthOri-1);
					polar_trans_region_inv(Rectangle1, &RegionDetect,fCenterY, fCenterX, StartAngle, EndAngle, m_RadiusInner,Radius,  WidthOri, RoiWidth+1, WidthOri, HeightOri, "nearest_neighbor");
					difference(RegionDetect, OCRSectorRgn, &RegionDetect);
				} 
				else
				{
					//适应大尺寸图像								
					HTuple StartAngle1, EndAngle1;
					HTuple AngleDiff;
					tuple_abs(EndAngle-StartAngle, &AngleDiff);

					if ( AngleDiff >= HTuple(360).Rad())
					{
						StartAngle1 = 0;
						EndAngle1 = HTuple(360).Rad();

						gen_circle(&Circle2_EXT, fCenterY, fCenterX, Radius);
						gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusInner);
						difference(Circle2_EXT, Circle1_Inner, &RegionDetect); 
						difference(RegionDetect, OCRSectorRgn, &RegionDetect);
					} 
					else
					{
						tuple_fmod(StartAngle, 2*PI, &StartAngle1);
						tuple_fmod(EndAngle, 2*PI, &EndAngle1);	

						gen_circle_sector(&OuterSector, fCenterY, fCenterX, Radius, StartAngle1, EndAngle1);
						gen_circle(&Circle1_Inner, fCenterY, fCenterX, m_RadiusInner);
						difference(OuterSector, Circle1_Inner, &RegionDetect);
						difference(RegionDetect, OCRSectorRgn, &RegionDetect);
					}
				}
				m_sectorAngle[id].m_angleStartAbs= StartAngle[0].D() /PI*180;
				m_sectorAngle[id].m_angleEndAbs=  EndAngle[0].D() /PI*180;
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
STDMETHODIMP CBatteryShellAlgo::DisOCRCircleRegionAngleAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4025$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;185$\
					 413;FP;1.5;2000;0.5;208$\
					 614;FP;0;720.0;0.5;40$\
					 615;FP;0;720.0;0.5;320$\
					 4026;LP;0;1;1;1"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//The surrounding detection of the battery shell //设计为大写字母P开头的算子一般都代表能并行计算
STDMETHODIMP CBatteryShellAlgo::RectLocationAlgo(VARIANT* rgnId,VARIANT* pGrayThresh,VARIANT* pRectWidth,VARIANT* pShowInterRegions,VARIANT* pBlackNoiseGray,VARIANT* pWhiteNoiseGray, VARIANT* pWhiteNoiseSize)
{
	//电池壳区域的定位
	int rId              = (int)rgnId->fltVal;
	int GrayThresh       = (int)pGrayThresh->fltVal;
	int RectWidth        = (int)pRectWidth->fltVal;
	int ShowInterRegions = (int)pShowInterRegions->fltVal;
	int BlackNoiseGray   = (int)pBlackNoiseGray->fltVal;
	int WhiteNoiseGray   = (int)pWhiteNoiseGray->fltVal;
	int WhiteNoiseSize   = (int)pWhiteNoiseSize->fltVal;
	Hobject newRgn,RegionUnion,RegionDilation;
	Hobject Rectangle,ImageReduced,Region,ConnectedRegions,LocationRect, RegionErosion, ExpandedImage;
	Hobject SelectedRegions,RegionOpening,RegionClosing,RegionFillUp, ImageMean, CorrectionImage;
	HTuple Row1, Column1, Row2, Column2;
	HTuple  Width, Height;
	Hobject Image = m_crtImg;
	get_image_size(Image, &Width, &Height);

#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			m_vROI.clear();
			m_locateRow1=0;
			m_locateCol1=0;
			m_locateRow2=Height;
			m_locateCol2=Width;
			return S_OK;
		}
		m_vRgn[rId] = newRgn;
		m_imgHeight = Height[0].I();
		m_imgWidth  = Width[0].I();

		gen_rectangle1 (&Rectangle, Height/5, 0, Height*4/5, Width/3);
		reduce_domain (Image, Rectangle, &ImageReduced);
		threshold(ImageReduced, &Region, 0, GrayThresh);
		connection(Region, &ConnectedRegions);
		select_shape_std (ConnectedRegions, &SelectedRegions, "max_area", 70);
		opening_rectangle1 (SelectedRegions, &RegionOpening, 1, Height/5);
		closing_rectangle1 (RegionOpening, &RegionClosing, 25, Height/5);

		fill_up (RegionClosing, &RegionFillUp);
		connection (RegionFillUp, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions, "height","and", Height*2/5, Height*4/5);
		smallest_rectangle1 (SelectedRegions, &Row1, &Column1, &Row2, &Column2);
		if(Column2.Num()!=1 || (Column2-Width/3).Abs()<50)
		{
			m_locateRow1=0;
			m_locateCol1=0;
			m_locateRow2=Height;
			m_locateCol2=Width;
			m_originImageMean=Image;
			return S_FALSE;

		}

		select_shape_std (ConnectedRegions, &SelectedRegions, "max_area", 70);
		smallest_rectangle1 (SelectedRegions, &Row1, &Column1, &Row2, &Column2);

		m_locateRow1=0;
		m_locateCol1=Column2;
		m_locateRow2=Height;
		m_locateCol2=Column2+RectWidth;
		if(!(m_locateCol2< Width[0].L()))
		{
			m_locateCol2=Width[0].L()-1;
		}
		gen_rectangle1(&LocationRect, m_locateRow1, m_locateCol1, m_locateRow2, m_locateCol2);


		//图片进行拓展，和灰度拉平(图片大面积明暗变化矫正)
		//防止RIO区域四周有纯黑行或列，影响后面的expand 和 mean,对矩形区域做一个腐蚀
		erosion_rectangle1(LocationRect, &RegionErosion, 20, 20);
		reduce_domain(Image,RegionErosion,&ImageReduced);
		expand_domain_gray (ImageReduced, &ExpandedImage, 200);
		mean_image (ExpandedImage, &ImageMean, 80, 80);
		div_image (ExpandedImage, ImageMean, &CorrectionImage, 127, 0);
		reduce_domain (CorrectionImage, RegionErosion, &m_originImageMean);

		//
		if(BlackNoiseGray>1)
		{
			threshold (m_originImageMean, &Region, 0, BlackNoiseGray);
			dilation_circle (Region, &RegionDilation, 5.5);
			erosion_circle (RegionDilation, &m_blackNoiseRegions, 3.5);
		}
		if(WhiteNoiseGray>1)
		{
			threshold (m_originImageMean, &Region, WhiteNoiseGray, 255);
			dilation_circle (Region, &RegionDilation, 5.5);
			erosion_circle (RegionDilation, &RegionDilation , 3.5);
			connection(RegionDilation,&ConnectedRegions);
			select_shape (ConnectedRegions, &SelectedRegions, "area","and", 0,WhiteNoiseSize);
			union1(SelectedRegions,&m_whiteNoiseRegions);
		}


		if((ShowInterRegions==1 &&WhiteNoiseGray>1 )||(ShowInterRegions==1 &&BlackNoiseGray>1 ))
		{
			union2(m_whiteNoiseRegions,m_blackNoiseRegions,&RegionUnion);
			m_vRgn[rId] = RegionUnion;
		}
		else
		{
			m_vRgn[rId] = LocationRect;
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
STDMETHODIMP CBatteryShellAlgo::RectLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1313$\
					 381;B$\
					 1275;LP;1;255;1;40$\
					 1283;LP;1;2096;1;1700$\
					 5015;LP;0;1;1;1$\
					 3094;LP;0;255;1;75$\
					 3095;LP;0;255;1;220$\
					 3093;LP;1;100;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::RectRegionExtraction(VARIANT* rgnId, VARIANT* pLeftErosion,VARIANT* pRightErosion)
{
	//不带并行的矩形区域
	int rId				= (int)rgnId->fltVal;
	int LeftErosion		= (int)pLeftErosion->fltVal;
	int RightErosion	= (int)pRightErosion->fltVal;
	Hobject newRgn,Rect;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vRgn[rId] = newRgn;
		int UpperErosion = 30;
		int DownErosion  = 30;
		if(m_locateRow1+UpperErosion< m_locateRow2-DownErosion && m_locateCol1+LeftErosion<m_locateCol2-RightErosion)
		{
			gen_rectangle1(&Rect, m_locateRow1+UpperErosion, m_locateCol1+LeftErosion, m_locateRow2-DownErosion, m_locateCol2-RightErosion);
		}
		else
		{
			gen_empty_obj(&Rect);
		}
		Hobject Image,EmptyObj;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? Image = m_originImageMean:Image = m_crtImg;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? m_vInterImgs[rId]=m_originImageMean: m_vInterImgs[rId]=EmptyObj;
		m_vRgn[rId] = Rect;

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
STDMETHODIMP CBatteryShellAlgo::RectRegionExtractionHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("8000$\
					 381;B$\
					 930;LP;-2000;2000;1;10$\
					 931;LP;-2000;2000;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PRectRegionExtraction(VARIANT* rgnId,VARIANT* pRowBlock,VARIANT* pLeftErosion,VARIANT* pRightErosion)
{
	//Rect Region,Both apply in parallel algo and general algo
	int rId				= (int)rgnId->fltVal;
	int LeftErosion		= (int)pLeftErosion->fltVal;
	int RightErosion	= (int)pRightErosion->fltVal;
	m_expandedImageSize = 200;
	m_rowBlock = (int)pRowBlock->fltVal;
	Hobject newRgn;


#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			m_vROI[rId].clear();
			return S_OK;
		}

		HTuple  Rows11, Columns11, Rows12, Columns12;
		HTuple  Width,Height,R1,C1, R2, C2,Numbers;
		Hobject RectRegion,Partitioned,ImageDomain,Image;
		Hobject ObjectSelected;

		m_vRgn[rId] = newRgn;
		Image = m_crtImg;
		if(m_originImageMean.Id()==H_EMPTY_REGION)
		{
			m_originImageMean =m_crtImg;
		}
		m_vInterImgs[rId] = m_originImageMean;

		get_image_size(Image,&Width,&Height);
		int UpperErosion = 30;
		int DownErosion  = 30;
		m_imgWidth=Width[0].I();
		m_imgHeight = Height[0].I();
		get_domain(Image,&ImageDomain);

		if(m_locateRow1+UpperErosion< m_locateRow2-DownErosion && m_locateCol1+LeftErosion<m_locateCol2-RightErosion && m_locateCol2.Num() !=0)
		{
			gen_rectangle1(&RectRegion, m_locateRow1+UpperErosion, m_locateCol1+LeftErosion, m_locateRow2-DownErosion, m_locateCol2-RightErosion);
			intersection(RectRegion,ImageDomain,&RectRegion);
			smallest_rectangle1(RectRegion,&R1,  &C1, &R2, &C2);
			if(R2-R1<200 )
			{
				m_rowBlock =1;
			}
			if(R2-R1<11 || C2-C1<11)
			{
				gen_empty_obj(&RectRegion);
				return S_FALSE;
			}
			partition_rectangle (RectRegion, &Partitioned, (C2-C1), (R2-R1)/m_rowBlock);
			count_obj(Partitioned,&Numbers);
			if(Numbers!=m_rowBlock)
			{
				gen_empty_obj(&RectRegion);
				return S_FALSE;
			}

			select_obj (Partitioned, &ObjectSelected, 1);
			dilation_rectangle1(ObjectSelected,&ObjectSelected,6,5);
			smallest_rectangle1 (ObjectSelected, &Rows11, &Columns11, &Rows12, &Columns12);
			m_partImageWidthOrigin[rId] = Columns12-Columns11;
			m_partImageHeightOrigin[rId] = Rows12-Rows11-1;
			HobjectPac tmp;
			for (int i = 1; i <= Numbers; i=i+1)
			{	
				Hobject EmptyRegion;
				HTuple Rows, Columns;
				gen_empty_obj(&EmptyRegion);
				select_obj (Partitioned, &ObjectSelected, i);
				dilation_rectangle1(ObjectSelected,&ObjectSelected,6,5);
				smallest_rectangle1 (ObjectSelected, &tmp.m_cornerRow,  &tmp.m_cornerCol, &Rows, &Columns);
				tmp.m_partIndex			= i;
				tmp.m_partImageWidth	= 0;
				tmp.m_partImageHeight	= 0;
				tmp.m_imagePart			= EmptyRegion;
				tmp.m_meanImage			= EmptyRegion;
				tmp.m_expandedImage		= EmptyRegion;
				tmp.m_gaussFilters		= EmptyRegion;
				tmp.m_illuminateImage	= EmptyRegion;
				tmp.m_imageDomain				= EmptyRegion;
				tmp.m_resultRegionsWhite        = EmptyRegion;
				tmp.m_resultRegionsBlack        = EmptyRegion;
				tmp.m_convertImage              = EmptyRegion;
				m_vROI[rId].push_back(tmp);
			}
			//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& tmp)
			parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& tmp)
			{
				Hobject TiledImage;
				crop_rectangle1 (Image, &tmp.m_imagePart, tmp.m_cornerRow,  tmp.m_cornerCol,tmp.m_cornerRow+m_partImageHeightOrigin[rId]-1,  tmp.m_cornerCol+m_partImageWidthOrigin[rId]-1);
				crop_rectangle1 (m_originImageMean, &tmp.m_illuminateImage , tmp.m_cornerRow,  tmp.m_cornerCol,tmp.m_cornerRow+m_partImageHeightOrigin[rId]-1,  tmp.m_cornerCol+m_partImageWidthOrigin[rId]-1);
				get_domain (tmp.m_imagePart,&tmp.m_imageDomain);
				gauss_filter (tmp.m_illuminateImage, &tmp.m_meanImage, 7);
				get_image_size(tmp.m_imagePart,&tmp.m_partImageWidth,&tmp.m_partImageHeight);
				tile_images_offset (tmp.m_illuminateImage, &TiledImage, m_expandedImageSize, m_expandedImageSize, 0, 0, tmp.m_partImageHeight, tmp.m_partImageWidth, tmp.m_partImageWidth+m_expandedImageSize*2, tmp.m_partImageHeight+m_expandedImageSize*2);
				expand_domain_gray (TiledImage, &tmp.m_expandedImage, m_expandedImageSize);
			});
		}
		else
		{
			gen_empty_obj(&RectRegion);
		}
		m_vRgn[rId] = RectRegion;

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
STDMETHODIMP CBatteryShellAlgo::PRectRegionExtractionHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("898$\
					 381;B$\
					 899;LP;1;32;1;8$\
					 930;LP;-2000;2000;1;10$\
					 931;LP;-2000;2000;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::MoveBlockDetectAlgo(VARIANT* rgnId, VARIANT* pIsShowInterImg,VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pBlackGrayValue, VARIANT* pWhiteGrayValue, VARIANT* pDetectArea)
{
	//纵向分块算法，并对每一块的平均灰度进行检测。校正图像和原始图像都可以使用
	int		rId				= (int)rgnId->fltVal;
	int		IsShowInterImg	= (int)pIsShowInterImg->fltVal;
	int		NumPart			= (int)pNumPart->fltVal;
	float	ThresholdValue	= (float)pThresholdValue->fltVal;
	int		BlackGrayValue	= (int)pBlackGrayValue->fltVal;
	int		WhiteGrayValue	= (int)pWhiteGrayValue->fltVal;

	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
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
		HTuple MeanSorted, LocationId2, Length1, ResultID2, ResultID3, ResultID4, ResultID5, Sorted, UniqTule;
		HTuple RgnArea, Row_test, Column_test;

		Hobject PartitionedRgn,CrtImage,EmptyObj;
		Hobject BackTransRegion, DefectRegs,ResultDefect;

		m_vInterImgs[rId]=EmptyObj;
		CrtImage = m_crtImg;
		if(IsShowInterImg && m_originImageMean.Id()!=H_EMPTY_REGION)
		{
			m_vInterImgs[rId]=m_originImageMean;
			CrtImage = m_originImageMean;
		}
		smallest_rectangle1 (rgn, &Row1, &Column1, &Row2, &Column2);
		if(Row1.Num()==0)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Width  = Column2 - Column1 + 1;
		Height = Row2 - Row1 + 1;
		partition_rectangle(rgn, &PartitionedRgn,Width, Height*1.0/NumPart);
		sort_region (PartitionedRgn, &PartitionedRgn,"first_point", "true", "row");
		intensity(PartitionedRgn, CrtImage, &MeanGray, &DeviationGray);
		tuple_length(MeanGray, &MeanLength);
		MeanDeviation = HTuple();
		for (int i=0; i<=MeanLength-2; i+=1)
		{
			tuple_deviation(HTuple(MeanGray[i]).Concat(HTuple(MeanGray[i+1])), &ExpGray);
			MeanDeviation[i] = ExpGray; 
		}
		tuple_sort(MeanDeviation, &AbsSorted);
		tuple_sort_index(MeanDeviation, &LocationId);
		tuple_length(MeanDeviation, &LengthTuple);
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

		//参数设置若不合理 报错//
		if(BlackGrayValue>=WhiteGrayValue)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		//

		ResultID2 = HTuple();
		for (int i=0; i<=Length1-1; i+=1)
		{
			if (0 != (HTuple(MeanSorted[i]) < BlackGrayValue))
			{
				ResultID2[i] = HTuple(LocationId2[i]);
			}
			else
			{
				break;
			}
		}
		ResultID2 += 1;

		for (int i=Length1[0].I()-1; i>=0; i+=-1)
		{
			if (0 != (HTuple(MeanSorted[i]) < WhiteGrayValue))
			{
				if (0 != (i==(Length1-1)))
				{
					ResultID3 = HTuple();
					break;
				}
				else
				{
					tuple_last_n(LocationId2, i+1, &ResultID3);
					break;
				}
			}
		}
		ResultID3 += 1;

		tuple_concat(ResultID2, ResultID3, &ResultID4);
		tuple_concat(ResultID, ResultID4, &ResultID5);
		tuple_sort(ResultID5, &Sorted);
		tuple_uniq(Sorted, &UniqTule);
		select_obj(PartitionedRgn, &ResultDefect, UniqTule);
		union1 (ResultDefect, &DefectRegs);
		
		/********************************剔除波纹*********************************************/
		if (m_vRgnInterference.Id() != H_EMPTY_REGION)
		{
			difference(DefectRegs, m_vRgnInterference, &DefectRegs);
		}
		/*************************************************************************************/
		area_center (DefectRegs, &RgnArea, &Row_test, &Column_test);

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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::MoveBlockDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1277$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 1327;LP;0;1;1;1$\
					 403;LP;5;500;1;200$\
					 404;FP;1;100;0.1;30$\
					 191;LP;1;255;1;50$\
					 192;LP;1;255;1;255");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::MoveImageDynThreshAlgo(VARIANT* rgnId, VARIANT* pMoveMode, VARIANT* pShiftSize,  VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
{
	//垂直/水平方向移动图像，并与未移动的原始图像作对比
	int rId                = (int)rgnId->fltVal;
	int MoveMode           = (int)pMoveMode->fltVal;
	int ShiftSize          = (int)pShiftSize->fltVal;
	int BlackThredLow      = (int)pBlackThredLow->fltVal;
	int BlackDefectSize    = (int)pBlackDefectSize->fltVal;
	int WhiteThredLow      = (int)pWhiteThredLow->fltVal;
	int WhiteDefectSize    = (int)pWhiteDefectSize->fltVal;
	// Local iconic variables 
	Hobject  Image, ROI, ImageReduced,ImageMoveReduce, ErrorRegions,DefectRegions;
	Hobject  ImagePart, Rectangle, ConcatImage, ImageMove,ErrorRegionsL,ErrorRegionsD;
	Hobject  RegionDynThresh, ConnectedRegions, SelectedRegions;
	Hobject  RegionDilation, RegionUnion, RegionErosion;
	Hobject  Errors, ObjectSelected, RegionDifference;
	Hobject  DefectRegs,newRgn;
	// Local control variables 
	HTuple  Number, Area,Rows, Columns,TupleShift;
	HTuple  DifferenceValue,Width,Height,MeanGray1,MeanGray2;
	HTuple  GrayDeviation1,GrayDeviation2,DiffMeanValue,DiffDeviationValue;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
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
		Image = m_crtImg;
		ROI = m_vRgn[rId];
		CComVariant retValue;
		if(ROI.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		TupleShift = ShiftSize;
		get_image_size(Image,&Width,&Height);
		gen_empty_obj(&ErrorRegions);	
		reduce_domain(Image,ROI,&ImageReduced);
		if (MoveMode == 0)
		{
			crop_rectangle1 (ImageReduced, &ImagePart, 0, 0, Height, ShiftSize);
			concat_obj (ImageReduced, ImagePart, &ConcatImage);
			tile_images_offset(ConcatImage, &ImageMove, (HTuple(0).Append(0)), HTuple(0).Concat(Width-TupleShift), (HTuple(0).Append(0)), TupleShift.Concat(0), Height.Concat(Height), Width.Concat(TupleShift),  Width, Height);

			if(BlackThredLow<255)
			{
				Hobject BlackRegDynLight, BlackRegDynDark, BlackRegDynDarkMoved, BlackRegDyn;
				Hobject BlackRegConnected, BlackDefectsPolar;
				dyn_threshold(ImageMove, ImageReduced, &BlackRegDynLight, BlackThredLow, "light");
				dyn_threshold(ImageMove, ImageReduced, &BlackRegDynDark, BlackThredLow, "dark");	

				move_region(BlackRegDynDark, &BlackRegDynDarkMoved, 0,TupleShift);
				intersection(BlackRegDynDarkMoved, BlackRegDynLight, &BlackRegDyn);
				connection(BlackRegDyn,&ConnectedRegions);
				select_shape(ConnectedRegions, &ErrorRegionsD, "area", "and", BlackDefectSize, 9999999);
				union1(ErrorRegionsD,&ErrorRegionsD);
			}
			if(WhiteThredLow<255)
			{

				Hobject BlackRegDynLight, BlackRegDynDark, BlackRegDynDarkMoved, BlackRegDyn;
				Hobject BlackRegConnected, BlackDefectsPolar;
				dyn_threshold(ImageMove, ImageReduced, &BlackRegDynLight, WhiteThredLow, "light");
				dyn_threshold(ImageMove, ImageReduced, &BlackRegDynDark, WhiteThredLow, "dark");

				move_region(BlackRegDynLight, &BlackRegDynDarkMoved, 0,TupleShift);
				intersection(BlackRegDynDarkMoved, BlackRegDynDark, &BlackRegDyn);

				connection(BlackRegDyn,&ConnectedRegions);
				select_shape(ConnectedRegions, &ErrorRegionsL, "area", "and", WhiteDefectSize, 9999999);
				union1(ErrorRegionsL,&ErrorRegionsL);
			}
		} 
		else
		{
			crop_rectangle1 (ImageReduced, &ImagePart, 0, 0,ShiftSize, Width);
			concat_obj (ImageReduced, ImagePart, &ConcatImage);
			tile_images_offset(ConcatImage, &ImageMove, HTuple(0).Concat(Height-TupleShift), (HTuple(0).Append(0)), TupleShift.Concat(0), (HTuple(0).Append(0)), Height.Concat(TupleShift), Width.Concat(Width), Width, Height);

			if(BlackThredLow<255)
			{
				Hobject BlackRegDynLight, BlackRegDynDark, BlackRegDynDarkMoved, BlackRegDyn;
				Hobject BlackRegConnected, BlackDefectsPolar;
				dyn_threshold(ImageMove, ImageReduced, &BlackRegDynLight, BlackThredLow, "light");
				dyn_threshold(ImageMove, ImageReduced, &BlackRegDynDark, BlackThredLow, "dark");	

				move_region(BlackRegDynDark, &BlackRegDynDarkMoved, TupleShift,0);
				intersection(BlackRegDynDarkMoved, BlackRegDynLight, &BlackRegDyn);
				connection(BlackRegDyn,&ConnectedRegions);
				select_shape(ConnectedRegions, &ErrorRegionsD, "area", "and", BlackDefectSize, 9999999);
				union1(ErrorRegionsD,&ErrorRegionsD);
			}
			if(WhiteThredLow<255)
			{

				Hobject BlackRegDynLight, BlackRegDynDark, BlackRegDynDarkMoved, BlackRegDyn;
				Hobject BlackRegConnected, BlackDefectsPolar;
				dyn_threshold(ImageMove, ImageReduced, &BlackRegDynLight, WhiteThredLow, "light");
				dyn_threshold(ImageMove, ImageReduced, &BlackRegDynDark, WhiteThredLow, "dark");

				move_region(BlackRegDynLight, &BlackRegDynDarkMoved, TupleShift,0);
				intersection(BlackRegDynDarkMoved, BlackRegDynDark, &BlackRegDyn);

				connection(BlackRegDyn,&ConnectedRegions);
				select_shape(ConnectedRegions, &ErrorRegionsL, "area", "and", WhiteDefectSize, 9999999);
				union1(ErrorRegionsL,&ErrorRegionsL);
			}

		}



		union2(ErrorRegionsD,ErrorRegionsL,&ErrorRegions);

		area_center (ErrorRegions, &Area, &Rows, &Columns);
		if(Area <= 0)
		{
			retValue = 0.0;
		}
		else
		{
			retValue = Area[0].I();
		}

		dilation_circle (ErrorRegionsD, &ErrorRegionsD, 2.5);
		m_vErrorRgn[rId] = ErrorRegionsD;
		dilation_circle (ErrorRegionsL, &ErrorRegionsL, 2.5);
		m_vWhiteErrorRgn[rId] = ErrorRegionsL;	
		retValue.Detach(pDetectArea);
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
STDMETHODIMP CBatteryShellAlgo::MoveImageDynThreshAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1244$\
					 397;R;LP;1;5000;1;<=#300$\
					 381;B$\
					 4014;LP;0;1;1;0$\
					 5008;LP;1;2000;1;20$\
					 415;LP;1;255;1;35$\
					 416;LP;1;999999;1;40$\
					 420;LP;1;255;1;35$\
					 421;LP;1;99999;1;40");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PDentMeanCurveAlgo(VARIANT* rgnId, VARIANT* pFilterSize,VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pUnionSize, VARIANT* pTotalSizeFilterLow,VARIANT* pDetectArea)
{
	//压伤曲率算法
	int rId                   = (int)rgnId->fltVal;
	int FilterSize            = (int)pFilterSize->fltVal;
	int BlackThred            = (int)pBlackThred->fltVal;
	int BlackArea             = (int)pBlackAreaThred->fltVal;
	int WhiteThred            = (int)pWhiteThred->fltVal;
	int WhiteArea             = (int)pWhiteAreaThred->fltVal;
	int UnionSize             = (int)pUnionSize->fltVal;
	int TotalSizeFilterLow    = (int)pTotalSizeFilterLow->fltVal;

	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		m_vWhiteErrorRgn[rId].Reset();
		Hobject ROI = m_vRgn[rId];

		if(ROI.Id() == H_EMPTY_REGION ||m_imgWidth/m_rowBlock<100||m_imgHeight<100||m_vROI[rId].size()<=0)
		{
			//区域不对和分快数不对都返回-1
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vRgnExTest[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Hobject Image = m_crtImg;
		Hobject ResultRegionWhite,ResultRegionBlack;
		Hobject BlackRegionDilation,WhiteRegionDilation,UnionRegions;

		Hobject ConnectedRegions,SelectedRegions,BlackUnion,WhiteUnion,DefectRegs;
		HTuple Area, Row_test, Column_test;

		Concurrency::concurrent_vector<Hobject> vTargetRgnsWhite;
		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject ImageMedian;

			derivate_gauss(it.m_meanImage, &it.m_convertImage, FilterSize, "mean_curvature");
			invert_image(it.m_convertImage,&it.m_convertImage);
			HTuple HomMat2D;
			Hobject AffineRegion;
			Hobject RegionsBlack, RegionsWhite;
			vector_angle_to_rigid (0, 0, 0,it.m_cornerRow,it.m_cornerCol, 0, &HomMat2D);
			if(BlackThred>0)
			{

				threshold(it.m_convertImage, &RegionsBlack, -1, -1.0* BlackThred/1000.0);
				affine_trans_region (RegionsBlack, &AffineRegion, HomMat2D, "false");
				vTargetRgnsBlack.push_back(AffineRegion);
			}
			if(WhiteThred>0)
			{
				threshold(it.m_convertImage,&RegionsWhite,WhiteThred/1000.0,255);
				affine_trans_region (RegionsWhite, &AffineRegion, HomMat2D, "false");
				vTargetRgnsWhite.push_back(AffineRegion);
			}
		});
		gen_empty_obj(&ResultRegionWhite);
		std::for_each(vTargetRgnsWhite.begin(),vTargetRgnsWhite.end(),[&ResultRegionWhite](Hobject rgn)
		{
			union2(ResultRegionWhite, rgn, &ResultRegionWhite);
		});
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		intersection(ResultRegionBlack,ROI,&ResultRegionBlack);
		connection(ResultRegionBlack, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", BlackArea, 99999);
		union1(SelectedRegions, &BlackUnion);
		//
		intersection(ResultRegionWhite,ROI,&ResultRegionWhite);
		connection(ResultRegionWhite, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", WhiteArea, 99999);
		union1(SelectedRegions, &WhiteUnion);
		union2(WhiteUnion, BlackUnion, &DefectRegs);
		//
		if(UnionSize>0)
		{
			dilation_circle(DefectRegs,&UnionRegions,UnionSize+3.5);
			erosion_circle(UnionRegions,&DefectRegs,UnionSize);
		}
		connection(DefectRegs, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", TotalSizeFilterLow, 9999999);
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
			dilation_circle (DefectRegs, &DefectRegs, 3.5);
			m_vErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);

		//if (m_bDebugImg==true)
		//{
		//	Concurrency::concurrent_vector<HTuple> vTargetTuple;
		//	m_dictSpecialData[rId].clear();
		//	//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	{
		//		HTuple Rows, Columns,Grayvals,Max,Min;
		//		Hobject Domain;
		//		get_domain(it.m_convertImage,&Domain);
		//		get_region_points (Domain, &Rows, &Columns);        
		//		get_grayval (it.m_convertImage, Rows, Columns,& Grayvals);
		//		Grayvals= Grayvals*1000;
		//		HTuple Histo,BinSize;
		//		tuple_histo_range (Grayvals, 0, 255, 256, &Histo, &BinSize);
		//		vTargetTuple.push_back(Histo);
		//	});
		//	HTuple ResultTuple;
		//	tuple_gen_const (256, 0, &ResultTuple);
		//	std::for_each(vTargetTuple.begin(),vTargetTuple.end(),[&ResultTuple](HTuple temp)
		//	{
		//		tuple_add(ResultTuple,temp,&ResultTuple);
		//	});
		//	for (int index = 0 ; index<255;index++)
		//	{
		//		if(ResultTuple[index].I()<255)
		//		{
		//			m_dictSpecialData[rId].push_back(-ResultTuple[index].I());
		//		}
		//		else
		//		{
		//			m_dictSpecialData[rId].push_back(-255);
		//		}
		//	}
		//}
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PDentMeanCurveAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("932$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 314;LP;1;15;1;10$\
					 407;LP;0;99999;1;200$\
					 408;LP;1;99999;1;50$\
					 409;LP;0;99999;1;200$\
					 410;LP;1;99999;1;50$\
					 1322;LP;1;99999;1;50$\
					 1312;LP;0;99999;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PEdgeAlgo(VARIANT* rgnId, VARIANT* pMaskSize,  VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//盖面边沿算法
	int rId			= (int)rgnId->fltVal;
	int EdgeThred	= (int)pEdgeThred->fltVal;
	int DefectSize	= (int)pDefectSize->fltVal;
	int MaskSize	= (int)pMaskSize->fltVal;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject ROI = m_vRgn[rId];
	CComVariant retValue;
	if(ROI.Id() == H_EMPTY_REGION||m_imgWidth/m_rowBlock<100||m_imgHeight<100||m_vROI[rId].size()<=0)
	{
		//区域不对和分快数不对都返回-1
		retValue = -1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	Hobject ResultRegionBlack;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI.begin(),m_vROI.end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject  RegionErosion;
			erosion_rectangle1 (it.m_imageDomain, &RegionErosion, 4,4);
			Hobject ImageMean, ImageEdgeRobinson,ImageReduced;
			mean_image(it.m_illuminateImage, &ImageMean, MaskSize, MaskSize);
			robinson_amp(ImageMean, &ImageEdgeRobinson);
			reduce_domain(ImageEdgeRobinson, RegionErosion, &it.m_convertImage);
			threshold(it.m_convertImage, &it.m_resultRegionsBlack, EdgeThred, 255);
			HTuple HomMat2D;
			vector_angle_to_rigid (0, 0, 0,it.m_cornerRow,it.m_cornerCol, 0, &HomMat2D);
			Hobject AffineRegion;
			affine_trans_region (it.m_resultRegionsBlack, &AffineRegion, HomMat2D, "false");
			vTargetRgnsBlack.push_back(AffineRegion);
		});

		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		intersection(ResultRegionBlack,ROI,&ResultRegionBlack);
		Hobject ConnectedThresholdRegion,SelectedRegion;
		connection(ResultRegionBlack, &ConnectedThresholdRegion);
		select_shape(ConnectedThresholdRegion, &SelectedRegion, "area", "and", DefectSize, 99999);
		Hobject DefectRegs;
		union1(SelectedRegion, &DefectRegs);
		dilation_circle(DefectRegs,&DefectRegs,3.5);
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
			m_vErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);

		//if (m_bDebugImg==true)
		//{
		//	Concurrency::concurrent_vector<HTuple> vTargetTuple;
		//	m_dictSpecialData[rId].clear();
		//	//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	{
		//		HTuple Rows, Columns,Grayvals,Max,Min;
		//		Hobject Domain;
		//		get_domain(it.m_convertImage,&Domain);
		//		get_region_points (Domain, &Rows, &Columns);        
		//		get_grayval (it.m_convertImage, Rows, Columns,& Grayvals);
		//		HTuple Histo,BinSize;
		//		tuple_histo_range (Grayvals, 0, 255, 256, &Histo, &BinSize);
		//		vTargetTuple.push_back(Histo);
		//	});
		//	HTuple ResultTuple;
		//	tuple_gen_const (256, 0, &ResultTuple);
		//	std::for_each(vTargetTuple.begin(),vTargetTuple.end(),[&ResultTuple](HTuple temp)
		//	{
		//		tuple_add(ResultTuple,temp,&ResultTuple);
		//	});
		//	for (int index = 0 ; index<255;index++)
		//	{
		//		if(ResultTuple[index].I()<255)
		//		{
		//			m_dictSpecialData[rId].push_back(-ResultTuple[index].I());
		//		}
		//		else
		//		{
		//			m_dictSpecialData[rId].push_back(-255);
		//		}
		//	}
		//}
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PEdgeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("935$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 314;LP;1;25;1;11$\
					 433;LP;1;255;1;35$\
					 434;LP;1;99999;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PGDarkLineAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackThredHigh, VARIANT* pBlackDefectSize,VARIANT* pDetectArea)
{
	//盖面找线算法
	int rId = (int)rgnId->fltVal;
	float BlackMaskSize = (float)pBlackMaskSize->fltVal;
	double BlackThredLow = (double)pBlackThredLow->fltVal;
	int BlackDefectSize = (int)pBlackDefectSize->fltVal;
	double BlackThredHigh = (double)pBlackThredHigh->fltVal;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject ROI = m_vRgn[rId];
	CComVariant retValue;
	if(ROI.Id() == H_EMPTY_REGION||m_vROI[rId].size()<=0)
	{
		retValue         = -1;
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	Hobject ResultRegionBlack;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI.begin(),m_vROI.end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject RegionErosion;
			erosion_rectangle1 (it.m_imageDomain, &RegionErosion, 4,4);
			Hobject BlackLines,BlackUnion;
			lines_gauss(it.m_illuminateImage, &BlackLines, BlackMaskSize, BlackThredLow, BlackThredHigh, "dark", "false", "bar-shaped", "false");
			select_shape_xld (BlackLines, &BlackLines, "contlength", "and", 8, 998999);
			select_shape_xld (BlackLines, &BlackLines, "height", "and", 0, 8);
			Hobject ContoursSplit,UnionContours,SelectedContours;
			segment_contours_xld (BlackLines, &ContoursSplit, "lines_circles", 5, 4, 2);
			union_collinear_contours_xld (ContoursSplit, &UnionContours, 20, 1, 2, 0.1, "attr_keep");

			ImageProcessAlg.LinesToRegion(UnionContours, BlackDefectSize, 999999, BlackUnion);
			intersection (RegionErosion, BlackUnion, &BlackUnion);

			HTuple HomMat2D;
			vector_angle_to_rigid (0, 0, 0,it.m_cornerRow,it.m_cornerCol, 0, &HomMat2D);
			Hobject AffineRegion;
			affine_trans_region (BlackUnion, &AffineRegion, HomMat2D, "false");
			vTargetRgnsBlack.push_back(AffineRegion);
		});
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		HTuple Area, Row_test, Column_test;
		area_center(ResultRegionBlack, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (ResultRegionBlack, &ResultRegionBlack, 2.5);
			m_vErrorRgn[rId] = ResultRegionBlack;
		}		
		retValue.Detach(pDetectArea);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PGDarkLineAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1242$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 426;FP;0.1;20;0.1;1.5$\
					 427;FP;0;8;0.5;3$\
					 611;FP;1;255;0.5;8$\
					 428;LP;1;5000;1;250");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PGLineXAlgo(VARIANT* rgnId, VARIANT* pBlackFilterSize, VARIANT* pBlackThredLow,VARIANT* pHeightSizeFilter,VARIANT* pDetectArea)
{
	//盖面找线算法
	int rId               = (int)rgnId->fltVal;
	float BlackFilterSize = (float)pBlackFilterSize->fltVal;
	int BlackThredLow     = (int)pBlackThredLow->fltVal;
	int HeightSizeFilter  = (int)pHeightSizeFilter->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];
		Hobject Image,EmptyObj;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? Image = m_originImageMean:Image = m_crtImg;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? m_vInterImgs[rId]=m_originImageMean: m_vInterImgs[rId]=EmptyObj;
		if(ROI.Id() == H_EMPTY_REGION)
		{
			retValue         = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Hobject ResultRegionBlack,ConnectedRegions,SelectedRegions,ConnectedNoise;

		if(m_vROI[rId].size()>0)
		{
			Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
			//std::for_each(m_vROI.begin(),m_vROI.end(), [&](HobjectPac& it)
			parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			{
				Hobject RegionErosion;
				erosion_rectangle1 (it.m_imageDomain, &RegionErosion, 4,4);
				Hobject ImageMin,DerivGauss,Region,BlackUnion;
				gray_erosion_rect (it.m_meanImage, &ImageMin, 5, 1);
				derivate_gauss (ImageMin, &DerivGauss,BlackFilterSize, "xx");
				threshold (DerivGauss, &Region, BlackThredLow*1.0/100, 255);
				intersection (RegionErosion, Region, &BlackUnion);
				Hobject AffineRegion;
				move_region(BlackUnion, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
				vTargetRgnsBlack.push_back(AffineRegion);
			});
			gen_empty_obj(&ResultRegionBlack);
			std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
			{
				union2(ResultRegionBlack, rgn, &ResultRegionBlack);
			});
		}
		else
		{
			Hobject RegionErosion,ImageReduced;
			Hobject ImageMin,DerivGauss,Region,BlackUnion;
			erosion_rectangle1 (ROI, &RegionErosion, 4,4);
			reduce_domain(Image,ROI,&ImageReduced);
			gray_erosion_rect (ImageReduced, &ImageMin, 5, 1);
			derivate_gauss (ImageMin, &DerivGauss,BlackFilterSize, "xx");
			threshold (DerivGauss, &Region, BlackThredLow*1.0/100, 255);
			fill_up(Region,&Region);
			intersection (RegionErosion, Region, &ResultRegionBlack);
		}
		connection(m_blackNoiseRegions,&ConnectedNoise);
		select_shape(ConnectedNoise,&SelectedRegions, "height", "and",3.5, 9999);
		dilation_circle(SelectedRegions,&SelectedRegions,5.5);
		difference(ResultRegionBlack,SelectedRegions,&ResultRegionBlack);


		connection (ResultRegionBlack, &ConnectedRegions);
		select_shape (ConnectedRegions,&SelectedRegions, "height", "and",HeightSizeFilter, 999999999);
		union1(SelectedRegions,&ResultRegionBlack);
		HTuple Area, Row_test, Column_test;
		area_center(ResultRegionBlack, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (ResultRegionBlack, &ResultRegionBlack, 2.5);
			m_vErrorRgn[rId] = ResultRegionBlack;
		}		
		retValue.Detach(pDetectArea);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PGLineXAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("929$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 314;FP;0.1;20;0.1;2.5$\
					 407;LP;1;9999;1;80$\
					 1311;LP;1;999999;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::ReducedResolutionDynThreshAlgo(VARIANT* rgnId, VARIANT* pBlockSize, VARIANT* pDefectType, VARIANT* pFilterSize, VARIANT* pOffSet, VARIANT* pHeightThred, VARIANT* pWidthThred, VARIANT* pDetectArea)
{
	//降低分辨率的动态阈值检测算法,针对有一定宽度和高度的淡缺陷
	int rId					 = (int)rgnId->fltVal;
	int BlockSize			 = (int)pBlockSize->fltVal;
	int DefectType			 = (int)pDefectType->fltVal;
	int FilterSize           = (int)pFilterSize->fltVal;
	int OffSet				 = (int)pOffSet->fltVal;
	int HeightThred			 = (int)pHeightThred->fltVal;
	int WidthThred			 = (int)pWidthThred->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];
		Hobject Image,EmptyObj;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? Image = m_originImageMean:Image = m_crtImg;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? m_vInterImgs[rId]=m_originImageMean: m_vInterImgs[rId]=EmptyObj;
		if(ROI.Id() == H_EMPTY_REGION)
		{
			retValue         = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		HTuple Row1, Column1, Row2, Column2, Width, Height, NewWidth, NewHeight, ReviseWidth, ReviseHeight;
		HTuple HomMat2DIdentity, HomMat2DScale;
		Hobject ImageReduced, ImagePart, ReviseRectangle, ReviseImageReduced, ReviseImage, NewImage, ImageMean, ImageMedian;
		Hobject RegionDynThresh, ConnectedRegions, SelectedRegions, SelectedRegions1, RegionUnion, RegionAffineTrans, RegionMoved;
		smallest_rectangle1 (ROI, &Row1, &Column1, &Row2, &Column2);
		reduce_domain(Image, ROI, &ImageReduced);
		crop_domain (ImageReduced, &ImagePart);
		get_image_size (ImagePart, &Width, &Height);
		if ((BlockSize>Width)||(BlockSize>Height))
		{
			retValue         = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		NewWidth=Width[0].I()/BlockSize;
		NewHeight=Height[0].I()/BlockSize;
		ReviseWidth=NewWidth*BlockSize;
		ReviseHeight=NewHeight*BlockSize;
		if (((NewWidth>(2*FilterSize+1))&&((NewHeight>(2*FilterSize+1))))!=1)
		{
			retValue         = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		gen_rectangle1 (&ReviseRectangle, 0, 0, ReviseHeight-1, ReviseWidth-1);
		reduce_domain (ImagePart, ReviseRectangle, &ReviseImageReduced);
		crop_domain (ReviseImageReduced, &ReviseImage);
		zoom_image_size (ReviseImage, &NewImage, NewWidth, NewHeight, "weighted");
		mean_image (NewImage, &ImageMean, FilterSize, FilterSize);
		median_image (NewImage, &ImageMedian, "circle", FilterSize, "mirrored");
		if (DefectType==0)
		{
			dyn_threshold (NewImage, ImageMedian, &RegionDynThresh, OffSet, "dark");
		} 
		else
		{
			dyn_threshold (NewImage, ImageMedian, &RegionDynThresh, OffSet, "light");
		}
		connection (RegionDynThresh, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions, "height", "and", HeightThred/BlockSize, 99999);
		select_shape (SelectedRegions, &SelectedRegions1, "width", "and", WidthThred/BlockSize, 99999);
		union1 (SelectedRegions1, &RegionUnion);
		hom_mat2d_identity (&HomMat2DIdentity);
		hom_mat2d_scale (HomMat2DIdentity, BlockSize, BlockSize, 0, 0, &HomMat2DScale);
		affine_trans_region (RegionUnion, &RegionAffineTrans, HomMat2DScale, "nearest_neighbor");
		move_region (RegionAffineTrans, &RegionMoved, Row1, Column1);
		HTuple Area, Row_test, Column_test;
		area_center(RegionMoved, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (RegionMoved, &RegionMoved, 2.5);
			m_vErrorRgn[rId] = RegionMoved;
		}		
		retValue.Detach(pDetectArea);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::ReducedResolutionDynThreshAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4003$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 4004;LP;1;30;1;11$\
					 4005;LP;0;1;1;1$\
					 4006;LP;1;50;1;4$\
					 734;LP;1;40;1;4$\
					 1311;LP;1;999999;1;220$\
					 4007;LP;1;999999;1;22");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::TheColumnOffsetAlgo(VARIANT* rgnId, VARIANT* pColumnValue, VARIANT* pOffsetValue)
{
	//用于判断电池壳的高度偏差，检测高度偏差大的次品
	int rId					 = (int)rgnId->fltVal;
	int ColumnValue			 = (int)pColumnValue->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		HTuple locateCol1 = m_locateCol1;
		HTuple locateRow1 = m_locateRow1;
		HTuple locateRow2 = m_locateRow2;
		HTuple locateRowCenter1, locateRowCenter2;
		Hobject Image, ReferenceLine;
		m_vInterImgs[rId]=Image;
		locateRowCenter1 = locateRow1+(locateRow2-locateRow1)/3;
		locateRowCenter2 = locateRow2-(locateRow2-locateRow1)/3;
		gen_rectangle1(&ReferenceLine, locateRowCenter1, ColumnValue-1, locateRowCenter2, ColumnValue);
		union2(m_vRgnExTest[rId], ReferenceLine, &m_vRgnExTest[rId]);

		if(locateCol1 < 1)
		{
			retValue         = -1;
			retValue.Detach(pOffsetValue);
			return S_FALSE;
		}
		HTuple Offset;
		Offset=locateCol1-ColumnValue;
		if (Offset<0)
		{
			Offset=Offset-10;
		}
		retValue = Offset[0].I();
		retValue.Detach(pOffsetValue);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pOffsetValue);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::TheColumnOffsetAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4008$\
					 4010;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 4009;LP;1;800;1;50");			 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::FindingInterferenceAreaAlgo(VARIANT* rgnId, VARIANT* pDynThreshThred,VARIANT* pHeightLimit,VARIANT* pAreaLimit, VARIANT* pInterferenceArea)
{
	//寻找电池壳甩头造成的波纹干扰区域，用于其他相关算子剔除。
	int rId					 =		(int)rgnId->fltVal;
	int DynThreshThred		 =		(int)pDynThreshThred->fltVal;
	int HeightLimit			 =		(int)pHeightLimit->fltVal;
	int AreaLimit			 =		(int)pAreaLimit->fltVal;

	CComVariant retValue;
	Hobject Image = m_crtImg;
	Hobject ROI = m_vRgn[rId];
	Hobject ResultRegion;
	Hobject newRgn; 

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		m_vRgnInterference.Reset();

		if(ROI.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			retValue.Detach(pInterferenceArea);
			return S_FALSE;
		}

		//并行和非并行联合
		/*Hobject ImageReduced, ImageMean, ExpandedImage, ImageReduced1, ImageMean1;
		Hobject RegionDynThresh, ConnectedRegs, SelectedRegs, RegionUnion;
		HTuple Row1,Column1,Row2,Column2;
		Hobject RegionClosing,ConnectedRegions1,SelectedRegions1,RippleRegionFillUp;
		if(m_imgWidth/m_rowBlock>100 && m_imgHeight>100 && m_vROI[rId].size()>0)
		{
		Concurrency::concurrent_vector<Hobject> vTargetRgns;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
		Hobject PImageMean, PImageMean1, PRegionDynThresh, MoveLocalRegion;

		mean_image(it.m_expandedImage, &PImageMean, 100, 10);
		mean_image(it.m_expandedImage, &PImageMean1, 1, 100); 
		dyn_threshold(PImageMean1, PImageMean, &PRegionDynThresh, DynThreshThred, "dark");
		//将缺陷区域移动到原图位置
		move_region(PRegionDynThresh, &MoveLocalRegion, it.m_cornerRow+m_expandedImageSize, it.m_cornerCol+m_expandedImageSize);
		vTargetRgns.push_back(MoveLocalRegion);
		});
		gen_empty_obj(&ResultRegion);
		std::for_each(vTargetRgns.begin(),vTargetRgns.end(),[&ResultRegion](Hobject rgn)
		{
		union2(ResultRegion, rgn, &ResultRegion);
		});
		}
		else
		{
		reduce_domain(Image,ROI,&ImageReduced);
		mean_image(ImageReduced, &ImageMean, 100, 10);  
		expand_domain_gray(ImageMean, &ExpandedImage, 100);
		reduce_domain(ExpandedImage, ROI, &ImageReduced1);
		mean_image(ImageReduced1, &ImageMean1, 1, 100); 
		dyn_threshold(ImageMean1, ImageMean, &RegionDynThresh, DynThreshThred, "dark");

		}
		connection(RegionDynThresh, &ConnectedRegs);
		smallest_rectangle1(ROI,&Row1,&Column1,&Row2,&Column2);
		//select_shape (ConnectedRegions, &SelectedRegions, ["area","height","width"], "and", [AreaLimit,HeightLimit,(Column2-Column1)/4], [200000,200,200000]);
		select_shape(ConnectedRegs, &SelectedRegs, ((HTuple("area").Append("height")).Append("width")), "and", ((HTuple(AreaLimit).Append(HeightLimit)).Append((Column2-Column1)/5)), ((HTuple(200000).Append(200)).Append(200000)));
		union1(SelectedRegs, &RegionUnion);
		closing_rectangle1(RegionUnion, &RegionClosing, 1, HeightLimit*15);
		connection(RegionClosing, &ConnectedRegions1);
		select_shape_std(ConnectedRegions1, &SelectedRegions1, "max_area", 70);
		fill_up(SelectedRegions1, &RippleRegionFillUp);
		dilation_circle(RippleRegionFillUp,&ResultRegion,160);
		intersection(ROI, ResultRegion, &ResultRegion);
		HTuple Area, Row, Column;
		area_center(ResultRegion, &Area, &Row, &Column);
		m_vRgnInterference = ResultRegion;

		if (DisplayMode == 0)
		{
		Hobject newRgn;
		union2(m_vRgnExTest[rId], newRgn, &m_vRgnExTest[rId]);
		} 
		else
		{
		union2(m_vRgnExTest[rId], ResultRegion, &m_vRgnExTest[rId]);
		}
		//
		if(Area <= 0)
		{
		retValue = 0.0;
		Hobject newRgn; 
		}
		else
		{
		retValue = Area[0].I();
		}		
		retValue.Detach(pInterferenceArea);*/


		Hobject ImageReduced, ImageMean, ExpandedImage, ImageReduced1, ImageMean1;
		Hobject RegionDynThresh, ConnectedRegs, SelectedRegs, RegionUnion;
		HTuple Row1,Column1,Row2,Column2;
		Hobject RegionClosing,ConnectedRegions1,SelectedRegions1,RippleRegionFillUp;

		reduce_domain(Image,ROI,&ImageReduced);
		mean_image(ImageReduced, &ImageMean, 100, 10);  
		expand_domain_gray(ImageMean, &ExpandedImage, 100);
		reduce_domain(ExpandedImage, ROI, &ImageReduced1);
		mean_image(ImageReduced1, &ImageMean1, 1, 100); 
		dyn_threshold(ImageMean1, ImageMean, &RegionDynThresh, DynThreshThred, "dark");
		connection(RegionDynThresh, &ConnectedRegs);
		smallest_rectangle1(ROI,&Row1,&Column1,&Row2,&Column2);
		//select_shape (ConnectedRegions, &SelectedRegions, ["area","height","width"], "and", [AreaLimit,HeightLimit,(Column2-Column1)/4], [200000,200,200000]);
		select_shape(ConnectedRegs, &SelectedRegs, ((HTuple("area").Append("height")).Append("width")), "and", ((HTuple(AreaLimit).Append(HeightLimit)).Append((Column2-Column1)/5)), ((HTuple(200000).Append(200)).Append(200000)));
		union1(SelectedRegs, &RegionUnion);
		closing_rectangle1(RegionUnion, &RegionClosing, 1, HeightLimit*15);
		connection(RegionClosing, &ConnectedRegions1);
		select_shape_std(ConnectedRegions1, &SelectedRegions1, "max_area", 70);
		fill_up(SelectedRegions1, &RippleRegionFillUp);
		dilation_circle(RippleRegionFillUp,&ResultRegion,160);
		intersection(ROI, ResultRegion, &ResultRegion);

		HTuple Area, Row, Column;
		area_center(ResultRegion, &Area, &Row, &Column);
		if(Area <= 0)
		{
			retValue = 0.0;
			m_vRgnInterference = newRgn; 
			m_vRgnExTest[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			m_vRgnInterference = ResultRegion;
			m_vRgnExTest[rId] = ResultRegion;
		}		
		retValue.Detach(pInterferenceArea);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pInterferenceArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::FindingInterferenceAreaAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4015$\
					 4016;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 6017;LP;1;50;1;10$\
					 6018;LP;1;200;1;16$\
					 6019;LP;1;999999;1;1900");				 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
 
STDMETHODIMP CBatteryShellAlgo::PFindingMoldScratchAreaAlgo(VARIANT* rgnId, VARIANT* pThreshThred, VARIANT* pWidthMin, VARIANT* pWidthMax, VARIANT* pHeightMin, VARIANT* pHeightMax, VARIANT* pDilationSize, VARIANT* pInterferenceArea)
{
	//寻找壳身模具冲拉形成的划伤，用于其他相关算子剔除。（属于4321项目5/7号电池壳定制算子）
	int rId					 =		(int)rgnId->fltVal;
	int ThreshThred			 =		(int)pThreshThred->fltVal;
	int WidthMin			 =		(int)pWidthMin->fltVal;
	int WidthMax			 =		(int)pWidthMax->fltVal;
	int RbMin				 =		(int)pHeightMin->fltVal;
	int RbMax				 =		(int)pHeightMax->fltVal;
	int DilationSize		 =		(int)pDilationSize->fltVal;

	CComVariant retValue;
	Hobject Image = m_crtImg;
	Hobject ROI = m_vRgn[rId];
	Hobject newRgn; 

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
		if((WidthMin > WidthMax)||(RbMin > RbMax))     //参数合法性检查
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		m_vRgnInterference.Reset();

		Hobject  PhaseRegion, DefectRegs;
		HTuple ROIArea, ROIRow, ROIColumn;
		area_center(ROI, &ROIArea, &ROIRow, &ROIColumn);
		if (ROIArea<=0)
		{
			retValue = -1;
			retValue.Detach(pInterferenceArea);
			return S_FALSE;
		}
		if(ROI.Id() == H_EMPTY_REGION ) 
		{
			retValue = -1;
			retValue.Detach(pInterferenceArea);
			return S_FALSE;
		}
		if(m_vROI[rId].size()>1)
		{
			Concurrency::concurrent_vector<Hobject> vTargetRgns;
			//for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			{

				Hobject ImageMedian, EdgeAmplitude, Region, MoveLocalRegion;

				median_image(it.m_illuminateImage, &ImageMedian, "circle", 3, "mirrored");
				sobel_amp(ImageMedian, &EdgeAmplitude, "y", 9);
				threshold (EdgeAmplitude, &Region, ThreshThred, 255);
				/************************将缺陷区域移动到原图位置*************************************/
				move_region(Region, &MoveLocalRegion, it.m_cornerRow, it.m_cornerCol);
				vTargetRgns.push_back(MoveLocalRegion);
				/*************************************************************************************/
			});
			gen_empty_obj(&PhaseRegion);
			std::for_each(vTargetRgns.begin(),vTargetRgns.end(),[&PhaseRegion](Hobject rgn)
			{
				union2(PhaseRegion, rgn, &PhaseRegion);
			});
		}
		else
		{
			Hobject ImageMedian, EdgeAmplitude, Region;
			HTuple hv_Min, hv_Max, hv_Range, hv_Mean, hv_Deviation, Rangetrans, LowerValue, UpperValue;

			median_image(m_originImageMean, &ImageMedian, "circle", 3, "mirrored");
			sobel_amp(ImageMedian, &EdgeAmplitude, "y", 9);
			threshold (EdgeAmplitude, &PhaseRegion, ThreshThred, 255);

			
		}
		Hobject ConnectedRegions, WidthSelectedRegions, RbSelectedRegions, RegionDilation, RegionUnion;
		HTuple hv_Area, hv_Row, hv_Column;

		intersection(PhaseRegion, ROI, &PhaseRegion);
		area_center(PhaseRegion, &hv_Area, &hv_Row, &hv_Column);
		if (hv_Area > 0)
		{
			connection(PhaseRegion, &ConnectedRegions);
			select_shape (ConnectedRegions, &WidthSelectedRegions, "width", "and", WidthMin, WidthMax);
			select_shape (WidthSelectedRegions, &RbSelectedRegions, "rb", "and", RbMin, RbMax);
			dilation_rectangle1 (RbSelectedRegions, &RegionDilation, DilationSize, DilationSize);
			union1 (RegionDilation, &RegionUnion);
			intersection(RegionUnion, ROI, &DefectRegs);
		}

		HTuple Area, Row, Column;
		area_center(DefectRegs, &Area, &Row, &Column);
		if(Area <= 0)
		{
			retValue = 0.0;
			m_vRgnInterference = newRgn; 
			m_vRgnExTest[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			m_vRgnInterference = DefectRegs;
			m_vRgnExTest[rId] = DefectRegs;
		}		
		retValue.Detach(pInterferenceArea);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pInterferenceArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PFindingMoldScratchAreaAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4032$\
					 4016;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 4033;LP;0;55;1;12$\
					 4034;LP;1;300;1;75$\
					 4035;LP;100;500;1;250$\
					 4036;LP;0;20;1;0$\
					 4037;LP;1;50;1;10$\
					 4038;LP;10;50;1;15");				 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PDynThreshAlgoBlack(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize,  VARIANT* pDetectArea)
{
	int rId                          = (int)rgnId->fltVal;
	int BlackMaskSize                = (int)pBlackMaskSize->fltVal;
	int SeriousBlackPointDynThresh   = (int)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize        = (int) pSeriousBlackPointSize->fltVal;

	Hobject ResultRegionBlack,ROI;
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vRgnExTest[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	ROI = m_vRgn[rId];
	CComVariant retValue;
	if(ROI.Id() == H_EMPTY_REGION || m_vROI[rId].size()<=0)
	{
		retValue = -1;
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

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject ImageMean,BlackRegions;
			HTuple hv_width,hv_height;
			get_image_size(it.m_illuminateImage,&hv_width,&hv_height);
			if(BlackMaskSize>hv_width)
			{
				BlackMaskSize=hv_width[0].I()-1;
			}
			if(BlackMaskSize>hv_height)
			{
				BlackMaskSize=hv_height[0].I()-1;
			}
			mean_image ( it.m_illuminateImage, &ImageMean, BlackMaskSize, BlackMaskSize);
			dyn_threshold (it.m_illuminateImage, ImageMean, &BlackRegions, SeriousBlackPointDynThresh,"dark");
			Hobject AffineRegion;
			move_region(BlackRegions, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
			vTargetRgnsBlack.push_back(AffineRegion);
		});
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		Hobject ConnectedRegions,SelectedRegions,DefectRegs;
		HTuple Area, Row_test, Column_test;

		connection(ResultRegionBlack, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, 99999);
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PDynThreshAlgoBlackHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1243$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 412;LP;3;250;2;11$\
					 415;LP;1;250;1;50$\
					 416;LP;1;99999;1;50");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PDynThreshAlgoWhite(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize,  VARIANT* pDetectArea)
{
	int rId                          = (int)rgnId->fltVal;
	int BlackMaskSize                = (int)pBlackMaskSize->fltVal;
	int SeriousBlackPointDynThresh   = (int)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize        = (int) pSeriousBlackPointSize->fltVal;

	Hobject ResultRegionBlack,ROI;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	ROI = m_vRgn[rId];
	CComVariant retValue;
	if(ROI.Id() == H_EMPTY_REGION || m_vROI[rId].size()<=0)
	{
		retValue = -1;
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

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject ImageMean;
			HTuple hv_width,hv_height;
			get_image_size(it.m_illuminateImage,&hv_width,&hv_height);
			if(BlackMaskSize>hv_width)
			{
				BlackMaskSize=hv_width[0].I()-1;
			}
			if(BlackMaskSize>hv_height)
			{
				BlackMaskSize=hv_height[0].I()-1;
			}
			mean_image ( it.m_illuminateImage, &ImageMean, BlackMaskSize, BlackMaskSize);
			dyn_threshold (it.m_illuminateImage, ImageMean, &it.m_resultRegionsBlack, SeriousBlackPointDynThresh,"light");
			HTuple HomMat2D;
			vector_angle_to_rigid (0, 0, 0,it.m_cornerRow,it.m_cornerCol, 0, &HomMat2D);
			Hobject AffineRegion;
			affine_trans_region (it.m_resultRegionsBlack, &AffineRegion, HomMat2D, "false");
			vTargetRgnsBlack.push_back(AffineRegion);
		});
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		Hobject ConnectedRegions,SelectedRegions,DefectRegs;
		HTuple Area, Row_test, Column_test;
		connection(ResultRegionBlack, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, 99999);
		union1(SelectedRegions, &DefectRegs);
		area_center(DefectRegs, &Area, &Row_test, &Column_test);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vWhiteErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle (DefectRegs, &DefectRegs, 2.5);
			m_vWhiteErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PDynThreshAlgoWhiteHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("493$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 417;LP;3;250;2;11$\
					 420;LP;1;250;1;50$\
					 421;LP;1;99999;1;50");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PDyn2TimesAlgoBlack(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
{
	int rId = (int)rgnId->fltVal;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject ROI = m_vRgn[rId];
	CComVariant retValue;
	if(ROI.Id() == H_EMPTY_REGION ||m_vROI[rId].size()<=0)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	int BlackMaskSize = (int)pBlackMaskSize->fltVal;
	float SeriousBlackPointDynThresh = (float)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize = (int) pSeriousBlackPointSize->fltVal;
	Hobject ResultRegionBlack;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject ImageMean;
			HTuple hv_width,hv_height;
			get_image_size(it.m_illuminateImage,&hv_width,&hv_height);
			if(BlackMaskSize>hv_width)
			{
				BlackMaskSize=hv_width[0].I()-1;
			}
			if(BlackMaskSize>hv_height)
			{
				BlackMaskSize=hv_height[0].I()-1;
			}
			mean_image ( it.m_meanImage, &ImageMean, BlackMaskSize, BlackMaskSize);
			dyn_threshold (it.m_meanImage, ImageMean, &it.m_resultRegionsBlack, SeriousBlackPointDynThresh,"dark");
			HTuple HomMat2D;
			vector_angle_to_rigid (0, 0, 0,it.m_cornerRow,it.m_cornerCol, 0, &HomMat2D);
			Hobject AffineRegion;
			affine_trans_region (it.m_resultRegionsBlack, &AffineRegion, HomMat2D, "false");
			vTargetRgnsBlack.push_back(AffineRegion);
		});
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		Hobject ConnectedRegions,SelectedRegions,DefectRegs;
		HTuple Area, Row_test, Column_test;
		connection(ResultRegionBlack, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize, 99999);
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
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PDyn2TimesAlgoBlackHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1276$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 412;LP;3;250;2;11$\
					 415;LP;1;250;1;50$\
					 416;LP;1;99999;1;50");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PDynThreshDeviationBlack(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize,  VARIANT* pSeriousBlackPointSizeUpper, VARIANT* pBlackPointDeviation,VARIANT* pDetectArea)
{
	int rId                          = (int)rgnId->fltVal;
	int BlackMaskSize                = (int)pBlackMaskSize->fltVal;
	int SeriousBlackPointDynThresh   = (int)pSeriousBlackPointDynThresh->fltVal;
	int SeriousBlackPointSize        = (int)pSeriousBlackPointSize->fltVal;
	int SeriousBlackPointSizeUpper   = (int)pSeriousBlackPointSizeUpper->fltVal;
	int BlackPointDeviation          = (int)pBlackPointDeviation->fltVal;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject ROI = m_vRgn[rId];
	CComVariant retValue;
	if(ROI.Id() == H_EMPTY_REGION ||m_vROI[rId].size()<=0)
	{
		retValue = -1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}

	Hobject ResultRegionBlack;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}

		Hobject Image =m_crtImg;
		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject ImageMean,RegionDilation;
			HTuple hv_width,hv_height;
			get_image_size(it.m_illuminateImage,&hv_width,&hv_height);
			if(BlackMaskSize>hv_width)
			{
				BlackMaskSize=hv_width[0].I()-1;
			}
			if(BlackMaskSize>hv_height)
			{
				BlackMaskSize=hv_height[0].I()-1;
			}
			Hobject RegionErosion,RegionDynThresh,ConnectedRegions,SelectedRegions;
			mean_image ( it.m_illuminateImage, &ImageMean, BlackMaskSize, BlackMaskSize);
			dyn_threshold (it.m_illuminateImage, ImageMean, &RegionDynThresh, SeriousBlackPointDynThresh,"dark");
			dilation_circle (RegionDynThresh, &RegionDilation, 3.5);
			erosion_circle (RegionDilation, &RegionErosion, 3.5);
			connection (RegionErosion,&ConnectedRegions);
			select_shape(ConnectedRegions, &SelectedRegions, "area", "and", 15, 9999999);
			dilation_circle (SelectedRegions, &RegionDilation, 11.5);

			select_gray (RegionDilation, it.m_illuminateImage, &it.m_resultRegionsBlack, "deviation", "and", BlackPointDeviation, 255);
			HTuple HomMat2D;
			vector_angle_to_rigid (0, 0, 0,it.m_cornerRow,it.m_cornerCol, 0, &HomMat2D);
			Hobject AffineRegion;
			affine_trans_region (it.m_resultRegionsBlack, &AffineRegion, HomMat2D, "false");
			vTargetRgnsBlack.push_back(AffineRegion);
		});
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		Hobject ConnectedRegions,SelectedRegions,DefectRegs;
		HTuple Area, Row_test, Column_test;
		connection(ResultRegionBlack, &ConnectedRegions);

		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SeriousBlackPointSize>SeriousBlackPointSizeUpper?SeriousBlackPointSizeUpper:SeriousBlackPointSize, SeriousBlackPointSize>SeriousBlackPointSizeUpper?SeriousBlackPointSize:SeriousBlackPointSizeUpper);

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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PDynThreshDeviationBlackHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4000$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 412;LP;3;999;3;100$\
					 415;LP;1;250;1;35$\
					 4002;LP;1;999;1;50$\
					 4001;LP;1;99999;1;999$\
					 1372;LP;1;255;1;25" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PHessianMatrixAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pGrayDeviationThresh, VARIANT* pBlackAreaThred,VARIANT* pDetectArea)
{
	//盖面嵌环算法
	int rId = (int)rgnId->fltVal;
	int BlackThred         = (int)pBlackThred->fltVal;
	int BlackArea          = (int)pBlackAreaThred->fltVal;
	int GrayDeviationThresh= (int)pGrayDeviationThresh->fltVal;

	Hobject Image,ImageReduced,ExpandedImage,DefectRegs;
	Hobject RegionDilation,RegionErosion,RegionUnion,RegionIntersection;
	Hobject ConnectedRegions,SelectedRegions,Errors;
	Hobject ObjectSelected1,ObjectSelected2,RegionDifference;
	HTuple hv_Mean1,hv_Deviation1,hv_Mean2,hv_Deviation2;
	HTuple hv_Area, hv_Row, hv_Column,ObjNmuber;

	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];


		if(ROI.Id() == H_EMPTY_REGION||m_imgWidth/m_rowBlock<100||m_imgHeight<100|| m_vROI[rId].size()<=0) 
		{
			//区域不对和分快数不对都返回-1
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vRgnExTest[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Image = m_crtImg;
		reduce_domain (Image, ROI, &ImageReduced);
		//expand_domain_gray (ImageReduced, &ExpandedImage, 14);
		/*mean_image (ExpandedImage, &ImageMean, 7, 7);*/

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{

			Hobject DerivGauss;
			derivate_gauss(it.m_meanImage, &it.m_convertImage, 3, "det");
			Hobject Region;
			threshold(it.m_convertImage, &Region, BlackThred/1000.0, 255);
			HTuple HomMat2D;
			vector_angle_to_rigid (0, 0, 0,it.m_cornerRow,it.m_cornerCol, 0, &HomMat2D);
			Hobject AffineRegion;
			affine_trans_region (Region, &AffineRegion, HomMat2D, "false");
			vTargetRgnsBlack.push_back(AffineRegion);

		});
		Hobject ResultRegionBlack;
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});

		dilation_circle (ResultRegionBlack, &RegionDilation, 5.5);
		union1 (RegionDilation, &RegionUnion);
		intersection (ROI, RegionUnion, &RegionIntersection);
		connection (RegionIntersection, &ConnectedRegions);
		dilation_circle (ConnectedRegions, &RegionDilation, 10.5);
		count_obj(ConnectedRegions,&ObjNmuber);

		gen_empty_obj (&Errors);
		for(int index = 1; index<=ObjNmuber;index++)
		{
			select_obj (ConnectedRegions, &ObjectSelected1, index);
			intensity (ObjectSelected1, ImageReduced, &hv_Mean1, &hv_Deviation1);

			select_obj (RegionDilation, &ObjectSelected2, index);
			intersection (ROI,ObjectSelected2, &ObjectSelected2);
			difference (ObjectSelected2, ObjectSelected1, &RegionDifference);
			intensity (RegionDifference, ImageReduced, &hv_Mean2,&hv_Deviation2);

			if((hv_Deviation2-hv_Deviation1).Abs()>GrayDeviationThresh )
			{
				concat_obj (Errors, ObjectSelected1, &Errors);
			}
		}
		union1(Errors, &DefectRegs);
		connection(DefectRegs,& DefectRegs);
		select_shape (DefectRegs, &SelectedRegions, "area", "and", BlackArea, 99999);
		union1(SelectedRegions, &DefectRegs);
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

		//if (m_bDebugImg==true )
		//{
		//	Concurrency::concurrent_vector<HTuple> vTargetTuple;
		//	m_dictSpecialData[rId].clear();
		//	//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	{
		//		HTuple Rows, Columns,Grayvals,Max,Min;
		//		Hobject Domain;
		//		get_domain(it.m_convertImage,&Domain);
		//		get_region_points (Domain, &Rows, &Columns);        
		//		get_grayval (it.m_convertImage, Rows, Columns,& Grayvals);
		//		Grayvals= Grayvals*1000;
		//		HTuple Histo,BinSize;
		//		tuple_histo_range (Grayvals, 0, 255, 256, &Histo, &BinSize);
		//		vTargetTuple.push_back(Histo);
		//	});
		//	HTuple ResultTuple;
		//	tuple_gen_const (256, 0, &ResultTuple);
		//	std::for_each(vTargetTuple.begin(),vTargetTuple.end(),[&ResultTuple](HTuple temp)
		//	{
		//		tuple_add(ResultTuple,temp,&ResultTuple);
		//	});
		//	for (int index = 0 ; index<255;index++)
		//	{
		//		if(ResultTuple[index].I()<255)
		//		{
		//			m_dictSpecialData[rId].push_back(-ResultTuple[index].I());
		//		}
		//		else
		//		{
		//			m_dictSpecialData[rId].push_back(-255);
		//		}
		//	}
		//}
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PHessianMatrixAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("406$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 407;LP;1;9999;1;10$\
					 1285;LP;1;255;1;50$\
					 408;LP;1;9999;1;12");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PGradientDetectAlgo(VARIANT* rgnId, VARIANT* pEdgeThresh, VARIANT* pBlackAreaThred ,VARIANT* pDetectArea)
{

	int rId                   = (int)rgnId->fltVal;
	int EdgeThresh            = (int)pEdgeThresh->fltVal;
	int BlackArea             = (int)pBlackAreaThred->fltVal;


	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI   = m_vRgn[rId];
		Hobject Image = m_crtImg;
		if(ROI.Id() == H_EMPTY_REGION ||m_imgWidth/m_rowBlock<100||m_imgHeight<100 ||m_vROI[rId].size()<=0 || m_vROI[rId].size()<=0) 
		{
			//区域不对和分快数不对都返回-1
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Hobject DefectRegs;
		HTuple hv_Area, hv_Row, hv_Column;

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject DerivGauss,ImageResult,RegionDynThresh,RegionFillUp;
			Hobject OriginImageMean,NewImage,ImageResult1;

			HTuple hv_min, hv_max, hv_range,hv_MaxThresh;
			//gray_erosion_rect (it.m_illuminateImage , &OriginImageMean, 3, 3);
			median_image (it.m_illuminateImage , &OriginImageMean, "circle", 3,"mirrored");
			derivate_gauss(OriginImageMean, &it.m_convertImage,3, "gradient");
			Hobject ErrorRegions;
			threshold (it.m_convertImage, &RegionDynThresh,HTuple(EdgeThresh/10.0), 255);
			dilation_circle(RegionDynThresh,&RegionDynThresh,5.5 );

			HTuple HomMat2D;
			vector_angle_to_rigid (0, 0, 0,it.m_cornerRow,it.m_cornerCol, 0, &HomMat2D);
			Hobject AffineRegion;
			affine_trans_region (RegionDynThresh, &AffineRegion, HomMat2D, "false");
			vTargetRgnsBlack.push_back(AffineRegion);

		});
		Hobject ResultRegionBlack,SelectedRegion,ConnectedThresholdRegion,ConnectedRegions;
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});

		HTuple Area1,Row1,Col1;
		area_center(ResultRegionBlack,&Area1,&Row1,&Col1);
		if(Area1>m_imgWidth*m_imgHeight/3)
		{
			retValue = 99999;
			retValue.Detach(pDetectArea);
			Hobject ErorRgn;
			erosion_circle (ROI, &ErorRgn, 20);
			m_vErrorRgn[rId] = ErorRgn;
			return S_OK;
		}
		connection (ResultRegionBlack, &ConnectedRegions);


		select_shape(ConnectedRegions, &SelectedRegion, "area", "and", BlackArea, 99999);
		union1(SelectedRegion,&DefectRegs);
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

		//if (m_bDebugImg==true  )
		//{
		//	Concurrency::concurrent_vector<HTuple> vTargetTuple;
		//	m_dictSpecialData[rId].clear();
		//	//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	{
		//		HTuple Rows, Columns,Grayvals,hv_max,hv_min;
		//		Hobject Domain;
		//		get_domain(it.m_convertImage,&Domain);
		//		get_region_points (Domain, &Rows, &Columns);        
		//		get_grayval (it.m_convertImage, Rows, Columns,& Grayvals);
		//		Grayvals= Grayvals*10.0;
		//		HTuple Histo,BinSize;
		//		tuple_histo_range (Grayvals, 0, 255, 256, &Histo, &BinSize);
		//		vTargetTuple.push_back(Histo);
		//	});
		//	HTuple ResultTuple;
		//	tuple_gen_const (256, 0, &ResultTuple);
		//	std::for_each(vTargetTuple.begin(),vTargetTuple.end(),[&ResultTuple](HTuple temp)
		//	{
		//		tuple_add(ResultTuple,temp,&ResultTuple);
		//	});
		//	for (int index = 0 ; index<255;index++)
		//	{
		//		if(ResultTuple[index].I()<255)
		//		{
		//			m_dictSpecialData[rId].push_back(-ResultTuple[index].I());
		//		}
		//		else
		//		{
		//			m_dictSpecialData[rId].push_back(-255);
		//		}
		//	}
		//}
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PGradientDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5022$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 5020;LP;1;9999;1;25$\
					 434;LP;1;99999;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PBrushDetectAlgo(VARIANT* rgnId, VARIANT* pDynThreshold,VARIANT* pLineAreaFilter, VARIANT* pErrorWidthFilter, VARIANT* pDetectArea)
{
	//dyn检测拉丝
	int rId               = (int)rgnId->fltVal;
	int DynThreshold      = (int)pDynThreshold->fltVal;
	int LineAreaFilter    = (int)pLineAreaFilter->fltVal;
	int ErrorWidthFilter  = (int)pErrorWidthFilter->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];

		if(ROI.Id() == H_EMPTY_REGION || m_vROI[rId].size()<=0) 
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vRgnExTest[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Hobject DefectRegs;
		HTuple hv_Area, hv_Row, hv_Column;
		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject Domain;
			get_domain( it.m_imagePart,&Domain);
			HTuple hv_width,hv_height;
			get_image_size(it.m_illuminateImage,&hv_width,&hv_height);
			Hobject ImageMedian,ImageMean,RegionDynThresh,RegionDilation,ConnectedRegions;
			median_rect (it.m_illuminateImage, &ImageMedian, ((hv_width/2).Concat(20)).Min(),1);
			mean_image (it.m_illuminateImage, &ImageMean, 1,  ((hv_height/2).Concat(50)).Min());
			dyn_threshold (ImageMedian, ImageMean, &RegionDynThresh, DynThreshold,"dark");
			//
			Hobject SelectedRegions,SkeletonReg,SortedRegions,LineObject;
			connection (RegionDynThresh, &ConnectedRegions);
			select_shape (ConnectedRegions, &SelectedRegions,"width", "and", 20, 9999999);
			skeleton (SelectedRegions, &SkeletonReg);
			sort_region (SkeletonReg, &SortedRegions, "first_point", "true", "row");

			HTuple Phi,Area, Row, Column,RectWidth,RectHeight,Length,CrtRow;
			smallest_rectangle2 (SortedRegions, &Row, &Column, &Phi, &RectWidth, &RectHeight);
			tuple_length(Phi, &Length);

			gen_empty_obj(&LineObject);
			if(Length>0)
			{
				Hobject Rectangle,RegionIntersection,RegionUnion,RegionTrans;
				HTuple Area2, Row2, Column2;
				CrtRow=Row.Select(0);
				gen_rectangle2(&Rectangle, Row[0], Column[0], Phi[0], hv_width,3);
				intersection(SortedRegions,Rectangle, &RegionIntersection);
				union1(RegionIntersection, &RegionUnion);
				area_center (RegionUnion, &Area2, &Row2, &Column2);
				if(Area2>LineAreaFilter)
				{
					shape_trans(RegionUnion, &RegionTrans, "rectangle2");
					concat_obj( RegionTrans, LineObject, &LineObject) ;
				}

				for (int i=1;i<Length;i++)
				{
					//增加角度筛选，0°附近
					/*************************************************************************************/
					if ((Phi.Select(i)<-0.14)||(Phi.Select(i)>0.14))
					{
						continue;
					} 
					else
					{
						gen_rectangle2(&Rectangle, Row.Select(i), Column.Select(i), Phi[i], hv_width,3);
					}
					/*************************************************************************************/
					intersection(SortedRegions,Rectangle, &RegionIntersection);
					union1(RegionIntersection, &RegionUnion);
					area_center (RegionUnion, &Area2, &Row2, &Column2);
					if(Area2>LineAreaFilter)
					{
						shape_trans(RegionUnion, &RegionTrans, "rectangle2");

						concat_obj( RegionTrans, LineObject, &LineObject) ;
					}
					CrtRow=Row.Select(i);
				}
			}
			Hobject ExOrientRegions,KeepOrientSelectedRegions,OrientSelectedRegions,ConnectedSelectedRegions;
			connection (LineObject, &ConnectedRegions);
			/*select_shape (ConnectedRegions, &ExOrientRegions,"orientation", "and", -3, 3);
			select_shape (ConnectedRegions, &KeepOrientSelectedRegions,"orientation", "and", -0.14, 0.14);
			difference(ConnectedRegions, ExOrientRegions,&OrientSelectedRegions);
			union2(OrientSelectedRegions, KeepOrientSelectedRegions, &OrientSelectedRegions);
			connection (OrientSelectedRegions, &ConnectedSelectedRegions);*/
			select_shape (ConnectedRegions, &SelectedRegions, "width", "and", ErrorWidthFilter, 9999999999);

			Hobject AffineRegion;
			move_region(SelectedRegions, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
			vTargetRgnsBlack.push_back(AffineRegion);

		});
		Hobject ResultRegionBlack,SelectedRegion,ConnectedThresholdRegion;
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});

		/********************************剔除波纹*********************************************/
		if (m_vRgnInterference.Id() != H_EMPTY_REGION)
		{
			difference(ResultRegionBlack, m_vRgnInterference, &ResultRegionBlack);
		}
		/*************************************************************************************/
		area_center(ResultRegionBlack, &hv_Area, &hv_Row, &hv_Column);
		if(hv_Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = hv_Area[0].I();
			dilation_circle (ResultRegionBlack, &DefectRegs, 3.5);
			m_vErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);

		//if (m_bDebugImg==true)
		//{
		//	Concurrency::concurrent_vector<HTuple> vTargetTuple;
		//	m_dictSpecialData[rId].clear();
		//	//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	{
		//		HTuple Rows, Columns,Grayvals,hv_max,hv_min;
		//		Hobject Domain;
		//		get_domain(it.m_imagePart,&Domain);
		//		get_region_points (Domain, &Rows, &Columns);        
		//		get_grayval (it.m_imagePart, Rows, Columns,& Grayvals);
		//		HTuple Histo,BinSize;
		//		tuple_histo_range (Grayvals, 0, 255, 256, &Histo, &BinSize);
		//		vTargetTuple.push_back(Histo);
		//	});
		//	HTuple ResultTuple;
		//	tuple_gen_const (256, 0, &ResultTuple);
		//	std::for_each(vTargetTuple.begin(),vTargetTuple.end(),[&ResultTuple](HTuple temp)
		//	{
		//		tuple_add(ResultTuple,temp,&ResultTuple);
		//	});
		//	for (int index = 0 ; index<255;index++)
		//	{
		//		if(ResultTuple[index].I()<255)
		//		{
		//			m_dictSpecialData[rId].push_back(-ResultTuple[index].I());
		//		}
		//		else
		//		{
		//			m_dictSpecialData[rId].push_back(-255);
		//		}
		//	}
		//}
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PBrushDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1251$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 1253;LP;1;255;1;15$\
					 434;LP;1;9999;3;200$\
					 1254;LP;1;9999;1;250");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PEmphasizeDarkPointDetectAlgo(VARIANT* rgnId, VARIANT* pWindowWidth,VARIANT* pWindowHeight, VARIANT* pEmphasizeFactor, VARIANT* pAreaFilter, VARIANT* pDetectArea)
{
	int		rId					= (int)rgnId->fltVal;
	int		WindowWidth			= (int)pWindowWidth->fltVal;
	int		WindowHeight		= (int)pWindowHeight->fltVal;
	float	EmphasizeFactor		= (float)pEmphasizeFactor->fltVal;
	int		AreaFilter			= (int)pAreaFilter->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];

		if(ROI.Id() == H_EMPTY_REGION || m_vROI[rId].size()<=0) 
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vRgnExTest[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Hobject DefectRegs;
		HTuple hv_Area, hv_Row, hv_Column;
		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			HTuple hv_width,hv_height;
			get_image_size(it.m_expandedImage,&hv_width,&hv_height);
			Hobject ImageMean,RegionDynThresh,RegionDilation,RegionErosion;
			mean_image (it.m_expandedImage, &ImageMean,11, 11);
			if(hv_width*2<=WindowWidth)
			{
				WindowWidth=hv_width[0].I()*2-1;
			}
			if(hv_height*2<=WindowHeight)
			{
				WindowHeight=hv_height[0].I()*2-1;
			}
			emphasize (ImageMean, &it.m_convertImage, WindowWidth,WindowHeight,EmphasizeFactor);
			threshold (it.m_convertImage, &RegionDynThresh, 0, 10);

			Hobject rect;
			gen_rectangle1 (&rect, m_expandedImageSize, m_expandedImageSize, hv_height-m_expandedImageSize, hv_width-m_expandedImageSize);
			intersection (rect, RegionDynThresh, &RegionDynThresh);
			move_region (RegionDynThresh, &RegionDynThresh, -m_expandedImageSize, -m_expandedImageSize);
			dilation_circle (RegionDynThresh, &RegionDilation, 3.5);
			erosion_circle (RegionDilation, &RegionErosion, 1.5);

			Hobject AffineRegion;
			move_region(RegionErosion, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
			vTargetRgnsBlack.push_back(AffineRegion);

		});
		Hobject ResultRegionBlack,SelectedRegions,ConnectedRegions;
		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		connection (ResultRegionBlack, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions,"area", "and", AreaFilter, 9999999);
		union1(SelectedRegions,&DefectRegs);
		intersection(ROI,DefectRegs,&DefectRegs);
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
		/////////////////////////////////////////////
		//if (m_bDebugImg==true  )
		//{
		//	Concurrency::concurrent_vector<HTuple> vTargetTuple;
		//	m_dictSpecialData[rId].clear();
		//	//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	{
		//		HTuple Rows, Columns,Grayvals,hv_max,hv_min;
		//		Hobject Domain;
		//		get_domain(it.m_convertImage,&Domain);
		//		get_region_points (Domain, &Rows, &Columns);        
		//		get_grayval (it.m_convertImage, Rows, Columns,& Grayvals);
		//		HTuple Histo,BinSize;
		//		tuple_histo_range (Grayvals, 0, 255, 256, &Histo, &BinSize);
		//		vTargetTuple.push_back(Histo);
		//	});
		//	HTuple ResultTuple;
		//	tuple_gen_const (256, 0, &ResultTuple);
		//	std::for_each(vTargetTuple.begin(),vTargetTuple.end(),[&ResultTuple](HTuple temp)
		//	{
		//		tuple_add(ResultTuple,temp,&ResultTuple);
		//	});
		//	for (int index = 0 ; index<255;index++)
		//	{
		//		if(ResultTuple[index].I()<255)
		//		{
		//			m_dictSpecialData[rId].push_back(-ResultTuple[index].I());
		//		}
		//		else
		//		{
		//			m_dictSpecialData[rId].push_back(-255);
		//		}
		//	}
		//}
		//////////////////////////////////////////
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PEmphasizeDarkPointDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1255$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 1256;LP;3;201;1;150$\
					 1257;LP;3;201;1;21$\
					 1258;FP;1;255;0.02;2$\
					 434;LP;1;9999999;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PVarThreshDarkDetectAlgo(VARIANT* rgnId, VARIANT* pWindowWidth, VARIANT* pGrayThresh, VARIANT* pAreaFilter, VARIANT* pDetectArea)
{
	int rId					= (int)rgnId->fltVal;
	int WindowWidth			= (int)pWindowWidth->fltVal;
	int WindowHeight		=  WindowWidth;
	int GrayThresh			= (int)pGrayThresh->fltVal;
	int AreaFilter			= (int)pAreaFilter->fltVal;

	CComVariant retValue;
	Hobject DefectRegs;
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];

		if(ROI.Id() == H_EMPTY_REGION || m_vROI[rId].size()<=0) 
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vRgnExTest[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			HTuple hv_width,hv_height;
			get_image_size(it.m_meanImage,&hv_width,&hv_height);

			Hobject Region;
			if(hv_width<=WindowWidth)
			{
				WindowWidth=hv_width[0].I();
			}
			if(hv_height<=WindowHeight)
			{
				WindowHeight=hv_height[0].I();
			}
			var_threshold (it.m_meanImage, &Region, WindowWidth, WindowHeight, 0.2,GrayThresh, "dark");
			Hobject AffineRegion;
			move_region(Region, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
			vTargetRgnsBlack.push_back(AffineRegion);
		});

		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		dilation_circle(ResultRegionBlack,&ResultRegionBlack,10.5);
		erosion_circle(ResultRegionBlack,&ResultRegionBlack,5.5);
		connection (ResultRegionBlack, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions,"area", "and", AreaFilter, 9999999);
		union1(SelectedRegions,&DefectRegs);
		intersection(ROI,DefectRegs,&DefectRegs);

		/********************************剔除波纹*********************************************/
		if (m_vRgnInterference.Id() != H_EMPTY_REGION)
		{
			difference(DefectRegs, m_vRgnInterference, &DefectRegs);
		}
		/*************************************************************************************/
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PVarThreshDarkDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1290$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 412;LP;1;1000;1;150$\
					 415;LP;1;255;1;20$\
					 434;LP;1;9999999;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PVarThreshLightDetectAlgo(VARIANT* rgnId, VARIANT* pWindowWidth, VARIANT* pGrayThresh, VARIANT* pAreaFilter, VARIANT* pDetectArea)
{
	int rId                = (int)rgnId->fltVal;
	int WindowWidth        = (int)pWindowWidth->fltVal;
	int WindowHeight       =  WindowWidth;
	int GrayThresh		   = (int)pGrayThresh->fltVal;
	int AreaFilter         = (int)pAreaFilter->fltVal;

	CComVariant retValue;
	Hobject DefectRegs;
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];

		if(ROI.Id() == H_EMPTY_REGION || m_vROI[rId].size()<=0) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			HTuple hv_width,hv_height;
			get_image_size(it.m_meanImage,&hv_width,&hv_height);
			Hobject Region;
			if(hv_width<=WindowWidth)
			{
				WindowWidth=hv_width[0].I();
			}
			if(hv_height<=WindowHeight)
			{
				WindowHeight=hv_height[0].I();
			}
			var_threshold (it.m_meanImage, &Region, WindowWidth, WindowHeight, 0.2,GrayThresh, "light");
			Hobject AffineRegion;
			move_region(Region, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
			vTargetRgnsBlack.push_back(AffineRegion);
		});

		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		dilation_circle(ResultRegionBlack,&ResultRegionBlack,10.5);
		erosion_circle(ResultRegionBlack,&ResultRegionBlack,5.5);
		connection (ResultRegionBlack, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions,"area", "and", AreaFilter, 9999999);
		union1(SelectedRegions,&DefectRegs);
		intersection(ROI,DefectRegs,&DefectRegs);

		/********************************剔除波纹*********************************************/
		if (m_vRgnInterference.Id() != H_EMPTY_REGION)
		{
			difference(DefectRegs, m_vRgnInterference, &DefectRegs);
		}
		/*************************************************************************************/
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
			m_vWhiteErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PVarThreshLightDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("928$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 417;LP;1;1000;1;150$\
					 418;LP;1;255;1;20$\
					 419;LP;1;9999999;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PThreshLightDetectAlgo(VARIANT* rgnId,VARIANT* pGrayThresh,VARIANT* pAreaFilter, VARIANT* pDetectArea)
{
	int rId                = (int)rgnId->fltVal;
	int GrayThresh         = (int)pGrayThresh->fltVal;
	int AreaFilter         = (int)pAreaFilter->fltVal;

	CComVariant retValue;
	Hobject DefectRegs;
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];

		if(ROI.Id() == H_EMPTY_REGION || m_vROI[rId].size()<=0) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject ImageMeanTemp,Region;
			region_to_mean(it.m_imageDomain,it.m_meanImage,&ImageMeanTemp);
			dyn_threshold(it.m_imagePart,ImageMeanTemp,&Region,GrayThresh, "light");
			Hobject AffineRegion;
			move_region(Region, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
			vTargetRgnsBlack.push_back(AffineRegion);
		});

		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		dilation_circle(ResultRegionBlack,&ResultRegionBlack,5.5);
		erosion_circle(ResultRegionBlack,&ResultRegionBlack,5.5);
		connection (ResultRegionBlack, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions,"area", "and", AreaFilter, 99999999);
		union1(SelectedRegions,&DefectRegs);
		intersection(ROI,DefectRegs,&DefectRegs);
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
			m_vWhiteErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PThreshLightDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1368$\
					 397;R;LP;0;99999;1;<=#300$\
					 381;B$\
					 418;LP;1;255;1;20$\
					 419;LP;1;9999999;1;200");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PThreshDarkDetectAlgo(VARIANT* rgnId,VARIANT* pGrayThresh,VARIANT* pAreaFilter, VARIANT* pDetectArea)
{
	int rId                = (int)rgnId->fltVal;
	int GrayThresh         = (int)pGrayThresh->fltVal;
	int AreaFilter         = (int)pAreaFilter->fltVal;

	CComVariant retValue;
	Hobject DefectRegs;
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI = m_vRgn[rId];

		if(ROI.Id() == H_EMPTY_REGION || m_vROI[rId].size()<=0) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
		//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		{
			Hobject ImageMeanTemp,Region;
			region_to_mean(it.m_imageDomain,it.m_meanImage,&ImageMeanTemp);
			dyn_threshold(it.m_imagePart,ImageMeanTemp,&Region,GrayThresh, "dark");
			Hobject AffineRegion;
			move_region(Region, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
			vTargetRgnsBlack.push_back(AffineRegion);
		});

		gen_empty_obj(&ResultRegionBlack);
		std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
		{
			union2(ResultRegionBlack, rgn, &ResultRegionBlack);
		});
		dilation_circle(ResultRegionBlack,&ResultRegionBlack,5.5);
		erosion_circle(ResultRegionBlack,&ResultRegionBlack,5.5);
		connection (ResultRegionBlack, &ConnectedRegions);
		select_shape (ConnectedRegions, &SelectedRegions,"area", "and", AreaFilter, 99999999);
		union1(SelectedRegions,&DefectRegs);
		intersection(ROI,DefectRegs,&DefectRegs);
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
			m_vWhiteErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PThreshDarkDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1367$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 415;LP;1;255;1;20$\
					 416;LP;1;9999999;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PDentAreaAlgo(VARIANT* rgnId, VARIANT* pFilterSize,VARIANT* pSensitiveThred, VARIANT* pErrorAreaFilter, VARIANT* pDetectArea)
{
	//压伤曲率算法
	int rId				= (int)rgnId->fltVal;
	int FilterSize		= (int)pFilterSize->fltVal;
	int SensitiveThred	= (int)pSensitiveThred->fltVal;
	int ErrorAreaFilter = (int)pErrorAreaFilter->fltVal;
	m_vWhiteErrorRgn[rId].Reset();
	m_vErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	Hobject ROI = m_vRgn[rId];
	CComVariant retValue;
	Hobject Image = m_crtImg;
	Hobject ResultRegion;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bPilotImg)
		{
			return S_OK;
		}
		HTuple ROIArea, ROIRow, ROIColumn;
		area_center(ROI, &ROIArea, &ROIRow, &ROIColumn);
		if (ROIArea<=0)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		if(ROI.Id() == H_EMPTY_REGION)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		if( m_vROI[rId].size()>1)
		{
			Concurrency::concurrent_vector<Hobject> vTargetRgns;
			//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			{
				Hobject ImageMean;
				if(FilterSize>1)
				{
					mean_image(it.m_meanImage,&ImageMean,1,FilterSize);

				}
				else
				{
					ImageMean = it.m_meanImage;
				}
				derivate_gauss (ImageMean, &it.m_convertImage,9,"area");
				HTuple hv_Min, hv_Max, hv_Range,hv_MaxThresh;
				min_max_gray(it.m_imageDomain,it.m_convertImage,0,&hv_Min, &hv_Max, &hv_Range);
				tuple_max((hv_Max*0.8).Concat(SensitiveThred/100.0), &hv_MaxThresh);
				Hobject ErrorRegions;
				threshold (it.m_convertImage, &ErrorRegions, hv_MaxThresh,255);
				Hobject AffineRegion;
				move_region(ErrorRegions, &AffineRegion, it.m_cornerRow, it.m_cornerCol);
				vTargetRgns.push_back(AffineRegion);
			});
			gen_empty_obj(&ResultRegion);
			std::for_each(vTargetRgns.begin(),vTargetRgns.end(),[&ResultRegion](Hobject rgn)
			{
				union2(ResultRegion, rgn, &ResultRegion);
			});
		}
		else
		{
			Hobject ImageMean,ImageReducedE,ConvertImage;
			HTuple hv_Min, hv_Max, hv_Range,hv_MaxThresh;
			Hobject ErrorRegions,ImageReduced,RegionErosion;

			reduce_domain(Image,ROI,&ImageReduced);
			expand_domain_gray(ImageReduced,&ImageReducedE,21);
			gauss_filter (ImageReducedE, &ImageMean, 7);

			if(FilterSize>1)
			{
				mean_image(ImageMean,&ImageMean,1,FilterSize);

			}
			derivate_gauss (ImageMean, &ConvertImage,9,"area");
			min_max_gray(ROI,ConvertImage,0,&hv_Min, &hv_Max, &hv_Range);
			tuple_max((hv_Max*0.8).Concat(SensitiveThred/100.0), &hv_MaxThresh);
			threshold (ConvertImage, &ResultRegion, hv_MaxThresh,255);

		}
		Hobject ConnectedRegions,SelectedRegions,DefectRegs,NoiseRegions;
		HTuple Area, Row_test, Column_test;
		fill_up(ResultRegion,&ResultRegion);
		intersection(ResultRegion,ROI,&ResultRegion);

		union2(m_blackNoiseRegions,m_whiteNoiseRegions,&NoiseRegions);
		dilation_circle(NoiseRegions,&NoiseRegions,12.5);
		difference(ResultRegion,NoiseRegions,&ResultRegion);

		connection(ResultRegion, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", ErrorAreaFilter, 9999999);
		union1(SelectedRegions, &DefectRegs);

		/********************************剔除波纹*********************************************/
		if (m_vRgnInterference.Id() != H_EMPTY_REGION)
		{
			difference(DefectRegs, m_vRgnInterference, &DefectRegs);
		}
		/*************************************************************************************/
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

		//if (m_bDebugImg==true)
		//{
		//	Concurrency::concurrent_vector<HTuple> vTargetTuple;
		//	m_dictSpecialData[rId].clear();
		//	m_dictSpecialData[rId].push_back(float(SensitiveThred));
		//	m_dictSpecialData[rId].push_back(-999999.0f);
		//	//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
		//	{
		//		HTuple Rows, Columns,Grayvals,Max,Min;
		//		Hobject Domain;
		//		get_domain(it.m_convertImage,&Domain);
		//		get_region_points (Domain, &Rows, &Columns);        
		//		get_grayval (it.m_convertImage, Rows, Columns,& Grayvals);
		//		Grayvals= Grayvals*100;
		//		vTargetTuple.push_back(Grayvals);
		//	});
		//	HTuple ResultTuple = HTuple();
		//	std::for_each(vTargetTuple.begin()
		//		,vTargetTuple.end(),[&ResultTuple](HTuple temp)
		//	{
		//		ResultTuple=ResultTuple.Concat(temp);
		//	});
		//	HTuple Histo,BinSize;
		//	tuple_histo_range (ResultTuple, 0, 255, 256, &Histo, &BinSize);
		//	for (int index = 0 ; index<255;index++)
		//	{
		//		if(ResultTuple[index].I()<255)
		//		{
		//			m_dictSpecialData[rId].push_back(-ResultTuple[index].I());
		//		}
		//		else
		//		{
		//			m_dictSpecialData[rId].push_back(-255);
		//		}
		//	}
		//}
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PDentAreaAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("934$\
					 397;R;LP;0;9999;1;<=#20$\
					 381;B$\
					 933;LP;0;100;1;9$\
					 407;LP;10;999;1;200$\
					 408;LP;1;99999;1;50");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::PYPartialDetectAlgo(VARIANT* rgnId, VARIANT* pFilterSize, VARIANT* pMinResponse, VARIANT* pSegementRatio, VARIANT* pDeviationThred, VARIANT* pBlackNoiseThred, VARIANT* pWhiteNoiseThred, VARIANT* pWhiteNoiseSize, VARIANT* pAreaThred, VARIANT* pDetectArea)
{
	//对图像高斯滤波，再对Y求偏导，凸显硬印特征。
	int rId                                 = (int)rgnId->fltVal;
	int FilterSize						    = (int)pFilterSize->fltVal;
	float MinResponse						= (float)pMinResponse->fltVal;
	float SegementRatio						= (float)pSegementRatio->fltVal;
	float DeviationThred					= (float)pDeviationThred->fltVal;
	int BlackNoiseThred						= (int)pBlackNoiseThred->fltVal;
	int WhiteNoiseThred						= (int)pWhiteNoiseThred->fltVal;
	int WhiteNoiseSize						= (int)pWhiteNoiseSize->fltVal;
	int AreaThred							= (int)pAreaThred->fltVal;
	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}


		Hobject  ROI, Image, PhaseRegion, DefectRegs;
		HTuple ROIArea, ROIRow, ROIColumn;

		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		ROI   = m_vRgn[rId];
		area_center(ROI, &ROIArea, &ROIRow, &ROIColumn);
		if (ROIArea<=0)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Image = m_crtImg;
		if(ROI.Id() == H_EMPTY_REGION ) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		if(m_vROI[rId].size()>1)
		{
			Concurrency::concurrent_vector<Hobject> vTargetRgnsBlackAndWhite;
			//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			{

				Hobject DerivGauss, ValidRegion, ValidRegionReduced, RegionDynThreshLight, RegionDynThreshDark, RegionUnion, RegionDilation, RegionIntersection, MoveLocalRegion;
				HTuple hv_Min, hv_Max, hv_Range, hv_Mean, hv_Deviation, hv_AbsMin, hv_AbsMax, hv_LightThresh, hv_DarkThresh;

				derivate_gauss(it.m_illuminateImage, &DerivGauss, FilterSize, "y");
				erosion_rectangle1 (it.m_imageDomain, &ValidRegion, 6, 1);
				reduce_domain(it.m_illuminateImage, ValidRegion, & ValidRegionReduced);
				min_max_gray(ValidRegion, DerivGauss, 0, &hv_Min, &hv_Max, &hv_Range);
				intensity(ValidRegion, DerivGauss, &hv_Mean, &hv_Deviation);

				if (hv_Mean.Num() > 0)
				{
					if (0 != (hv_Mean>0))
					{
						hv_AbsMin = (float)abs(hv_Min[0].D() - hv_Mean[0].D());
						hv_AbsMax = (float)abs(hv_Max[0].D() - hv_Mean[0].D());
					}
					else
					{
						hv_AbsMin = (float)abs(hv_Min[0].D() + hv_Mean[0].D());
						hv_AbsMax = (float)abs(hv_Max[0].D() - hv_Mean[0].D());
					}
					if (0 != (HTuple(hv_AbsMin>MinResponse).Or(hv_AbsMax>MinResponse)))
					{
						hv_LightThresh = hv_Mean + ((1 - SegementRatio)*hv_AbsMax);
						hv_DarkThresh = hv_Mean - ((1 - SegementRatio)*hv_AbsMin);
						if (0 != (hv_AbsMin<(MinResponse - 0.5)))
						{
							hv_DarkThresh = -20;
						}
						if (0 != (hv_AbsMax<(MinResponse - 0.5)))
						{
							hv_LightThresh = 1.5*hv_Max;
						}
						threshold(DerivGauss, &RegionDynThreshLight, hv_LightThresh, 1.5*hv_Max);
						threshold(DerivGauss, &RegionDynThreshDark, -20, hv_DarkThresh);
						union2(RegionDynThreshDark, RegionDynThreshLight, &RegionUnion);
						dilation_circle(RegionUnion, &RegionDilation, 3);
						intersection(RegionDilation, ValidRegion, &RegionIntersection);
						/***************************将检测到的区域移动到原图位置*******************************/
						move_region(RegionIntersection, &MoveLocalRegion, it.m_cornerRow, it.m_cornerCol);
						vTargetRgnsBlackAndWhite.push_back(MoveLocalRegion);
						/*************************************************************************************/
					}
				}
			});
			gen_empty_obj(&PhaseRegion);
			std::for_each(vTargetRgnsBlackAndWhite.begin(),vTargetRgnsBlackAndWhite.end(),[&PhaseRegion](Hobject rgn)
			{
				union2(PhaseRegion, rgn, &PhaseRegion);
			});
		}
		else
		{
			Hobject DerivGauss, ValidRegion, ValidRegionReduced, RegionDynThreshLight, RegionDynThreshDark, RegionUnion, RegionDilation;
			HTuple hv_Min, hv_Max, hv_Range, hv_Mean, hv_Deviation, hv_AbsMin, hv_AbsMax, hv_LightThresh, hv_DarkThresh;

			derivate_gauss(m_originImageMean, &DerivGauss, FilterSize, "y");
			erosion_rectangle1 (ROI, &ValidRegion, 6, 1);
			reduce_domain(m_originImageMean, ValidRegion, & ValidRegionReduced);
			min_max_gray(ValidRegion, DerivGauss, 0, &hv_Min, &hv_Max, &hv_Range);
			intensity(ValidRegion, DerivGauss, &hv_Mean, &hv_Deviation);
			if (hv_Mean.Num() > 0)
			{
				if (0 != (hv_Mean>0))
				{
					hv_AbsMin = (float)abs(hv_Min[0].D() - hv_Mean[0].D());
					hv_AbsMax = (float)abs(hv_Max[0].D() - hv_Mean[0].D());
				}
				else
				{
					hv_AbsMin = (float)abs(hv_Min[0].D() + hv_Mean[0].D());
					hv_AbsMax = (float)abs(hv_Max[0].D() - hv_Mean[0].D());
				}
				if (0 != (HTuple(hv_AbsMin>MinResponse).Or(hv_AbsMax>MinResponse)))
				{
					hv_LightThresh = hv_Mean + ((1 - SegementRatio)*hv_AbsMax);
					hv_DarkThresh = hv_Mean - ((1 - SegementRatio)*hv_AbsMin);
					if (0 != (hv_AbsMin<(MinResponse - 0.5)))
					{
						hv_DarkThresh = -20;
					}
					if (0 != (hv_AbsMax<(MinResponse - 0.5)))
					{
						hv_LightThresh = 1.5*hv_Max;
					}
					threshold(DerivGauss, &RegionDynThreshLight, hv_LightThresh, 1.5*hv_Max);
					threshold(DerivGauss, &RegionDynThreshDark, -20, hv_DarkThresh);
					union2(RegionDynThreshDark, RegionDynThreshLight, &RegionUnion);
					dilation_circle(RegionUnion, &RegionDilation, 3);
					intersection(RegionDilation, ValidRegion, &PhaseRegion);
					
				}
			}
		}
		Hobject ConnectedRegions, SelectedRegions, RegionUnion1, LocalImage, BlackNoiseRegion, WhiteNoiseRegion, WhiteNoiseRegions, SelectedWhiteRegions, WhiteRegionUnion;
		Hobject BWRegionUnion, RegionDifference, RegionDifferences, SelectedRegions1, LocalRegion;
		Hobject SelectedRegions2,TotalRegion,TotalRegions;
		HTuple hv_Area, hv_Row, hv_Column;
		HTuple Area, Row, Column;
		/********************************剔除波纹*********************************************/
		if (m_vRgnInterference.Id() != H_EMPTY_REGION)
		{
			difference(PhaseRegion, m_vRgnInterference, &PhaseRegion);
		}
		/*************************************************************************************/
		area_center(PhaseRegion, &Area, &Row, &Column);
		if (Area > 0)
		{
			connection(PhaseRegion, &ConnectedRegions);
			select_gray(ConnectedRegions, m_originImageMean, &SelectedRegions, "deviation", "and", DeviationThred, 255);
			union1(SelectedRegions, &RegionUnion1);
			reduce_domain(m_originImageMean, RegionUnion1, &LocalImage);
			threshold(LocalImage, &BlackNoiseRegion, 0, BlackNoiseThred);
			threshold(LocalImage, &WhiteNoiseRegion, WhiteNoiseThred, 255);
			connection(WhiteNoiseRegion, &WhiteNoiseRegions);
			select_shape(WhiteNoiseRegions, &SelectedWhiteRegions, "area", "and", 0, WhiteNoiseSize);
			union1(SelectedWhiteRegions, &WhiteRegionUnion);
			union2(BlackNoiseRegion, WhiteRegionUnion, &BWRegionUnion);
			difference(RegionUnion1, BWRegionUnion, &RegionDifference);
			connection(RegionDifference, &RegionDifferences);
			select_gray(RegionDifferences, m_originImageMean, &SelectedRegions1, "deviation", "and", DeviationThred, 255);
			union1(SelectedRegions1, &TotalRegion);
			connection(TotalRegion, &TotalRegions);
			select_shape(TotalRegions, &SelectedRegions2, "area", "and", AreaThred, 9999999);
			union1(SelectedRegions2, &SelectedRegions2);
			intersection(SelectedRegions2, ROI, &DefectRegs);
		}
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
			dilation_circle (DefectRegs, &DefectRegs, 4);
			m_vErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);


#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;

}
STDMETHODIMP CBatteryShellAlgo::PYPartialDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("3087$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 3088;LP;2;11;1;6$\
					 3089;FP;0;20;0.02;2.30$\
					 3090;FP;0.1;0.9;0.05;0.60$\
					 1372;FP;0;80;0.1;31.7$\
					 3091;LP;0;127;1;60$\
					 3092;LP;128;255;1;245$\
					 3093;LP;0;50;1;30$\
					 434;LP;0;500;1;185");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;

};

STDMETHODIMP CBatteryShellAlgo::PYPartialDetectAlgo1(VARIANT* rgnId, VARIANT* pRangeValue, VARIANT* pDeviationThred, VARIANT* pBlackNoiseThred, VARIANT* pWhiteNoiseThred, VARIANT* pWhiteNoiseSize, VARIANT* pAreaThred, VARIANT* pDetectArea)
{
	//对图像高斯滤波，再对Y求偏导，凸显硬印特征。
	int rId                                 = (int)rgnId->fltVal;
	int RangeValue							= (int)pRangeValue->fltVal;
	float DeviationThred					= (float)pDeviationThred->fltVal;
	int BlackNoiseThred						= (int)pBlackNoiseThred->fltVal;
	int WhiteNoiseThred						= (int)pWhiteNoiseThred->fltVal;
	int WhiteNoiseSize						= (int)pWhiteNoiseSize->fltVal;
	int AreaThred							= (int)pAreaThred->fltVal;
	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}


		Hobject  ROI, Image, PhaseRegion, DefectRegs;
		HTuple ROIArea, ROIRow, ROIColumn;

		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		ROI   = m_vRgn[rId];
		area_center(ROI, &ROIArea, &ROIRow, &ROIColumn);
		if (ROIArea<=0)
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Image = m_crtImg;
		if(ROI.Id() == H_EMPTY_REGION ) 
		{
			retValue = -1;
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}

		if(m_vROI[rId].size()>1)
		{
			Concurrency::concurrent_vector<Hobject> vTargetRgnsBlackAndWhite;
			//for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			{

				Hobject DerivGauss, ValidRegion, ValidRegionReduced, RegionDynThreshLight, RegionDynThreshDark, RegionUnion, RegionDilation, RegionIntersection, MoveLocalRegion;
				HTuple hv_Min, hv_Max, hv_Range, hv_Mean, hv_Deviation, Rangetrans, LowerValue, UpperValue;

				derivate_gauss(it.m_illuminateImage, &DerivGauss, (HTuple(1).Append(7)), "y");
				erosion_rectangle1 (it.m_imageDomain, &ValidRegion, 6, 1);
				reduce_domain(it.m_illuminateImage, ValidRegion, & ValidRegionReduced);
				min_max_gray(ValidRegion, DerivGauss, 0, &hv_Min, &hv_Max, &hv_Range);
				intensity(ValidRegion, DerivGauss, &hv_Mean, &hv_Deviation);
				if (hv_Mean.Num() > 0)
				{
					//RangeValue/17为区间变换
					Rangetrans = RangeValue/(17.0*2.0);
					LowerValue = hv_Mean[0].D() - Rangetrans;     
					UpperValue = hv_Mean[0].D() + Rangetrans;
					if ((hv_Min<LowerValue)&&(hv_Max>UpperValue))
					{
						if (hv_Max>UpperValue)
						{
							threshold(DerivGauss, &RegionDynThreshLight, UpperValue, hv_Max);
						} 
						if (hv_Min<LowerValue)
						{
							threshold(DerivGauss, &RegionDynThreshDark, hv_Min, LowerValue);
						} 
						union2(RegionDynThreshDark, RegionDynThreshLight, &RegionUnion);
						dilation_circle(RegionUnion, &RegionDilation, 3);
						intersection(RegionDilation, ValidRegion, &RegionIntersection);
						/**************************将缺陷区域移动到原图位置***********************************/
						move_region(RegionIntersection, &MoveLocalRegion, it.m_cornerRow, it.m_cornerCol);
						vTargetRgnsBlackAndWhite.push_back(MoveLocalRegion);
						/*************************************************************************************/
					}
				}
			});
			gen_empty_obj(&PhaseRegion);
			std::for_each(vTargetRgnsBlackAndWhite.begin(),vTargetRgnsBlackAndWhite.end(),[&PhaseRegion](Hobject rgn)
			{
				union2(PhaseRegion, rgn, &PhaseRegion);
			});
		}
		else
		{
			Hobject DerivGauss, ValidRegion, ValidRegionReduced, RegionDynThreshLight, RegionDynThreshDark, RegionUnion, RegionDilation;
			HTuple hv_Min, hv_Max, hv_Range, hv_Mean, hv_Deviation, Rangetrans, LowerValue, UpperValue;

			derivate_gauss(m_originImageMean, &DerivGauss, (HTuple(1).Append(7)), "y");
			erosion_rectangle1 (ROI, &ValidRegion, 6, 1);
			reduce_domain(m_originImageMean, ValidRegion, & ValidRegionReduced);
			min_max_gray(ValidRegion, DerivGauss, 0, &hv_Min, &hv_Max, &hv_Range);
			intensity(ValidRegion, DerivGauss, &hv_Mean, &hv_Deviation);

			if (hv_Mean.Num() > 0)
			{
				Rangetrans = RangeValue/(17.0*2.0);
				LowerValue = hv_Mean[0].D() - Rangetrans;     
				UpperValue = hv_Mean[0].D() + Rangetrans;
				if ((hv_Min<LowerValue)&&(hv_Max>UpperValue))
				{
					if (hv_Max>UpperValue)
					{
						threshold(DerivGauss, &RegionDynThreshLight, UpperValue, hv_Max);
					} 
					if (hv_Min<LowerValue)
					{
						threshold(DerivGauss, &RegionDynThreshDark, hv_Min, LowerValue);
					} 
					union2(RegionDynThreshDark, RegionDynThreshLight, &RegionUnion);
					dilation_circle(RegionUnion, &RegionDilation, 3);
					intersection(RegionDilation, ValidRegion, &PhaseRegion);
				}
			}
		}
		Hobject ConnectedRegions, SelectedRegions, RegionUnion1, LocalImage, BlackNoiseRegion, WhiteNoiseRegion, WhiteNoiseRegions, SelectedWhiteRegions, WhiteRegionUnion;
		Hobject BWRegionUnion, RegionDifference, RegionDifferences, SelectedRegions1, LocalRegion;
		Hobject SelectedRegions2,TotalRegion,TotalRegions;
		HTuple hv_Area, hv_Row, hv_Column;
		HTuple Area, Row, Column;
		/********************************剔除波纹*********************************************/
		if (m_vRgnInterference.Id() != H_EMPTY_REGION)
		{
			difference(PhaseRegion, m_vRgnInterference, &PhaseRegion);
		}
		/*************************************************************************************/
		area_center(PhaseRegion, &Area, &Row, &Column);
		if (Area > 0)
		{
			connection(PhaseRegion, &ConnectedRegions);
			select_gray(ConnectedRegions, m_originImageMean, &SelectedRegions, "deviation", "and", DeviationThred, 255);
			union1(SelectedRegions, &RegionUnion1);
			reduce_domain(m_originImageMean, RegionUnion1, &LocalImage);
			threshold(LocalImage, &BlackNoiseRegion, 0, BlackNoiseThred);
			threshold(LocalImage, &WhiteNoiseRegion, WhiteNoiseThred, 255);
			connection(WhiteNoiseRegion, &WhiteNoiseRegions);
			select_shape(WhiteNoiseRegions, &SelectedWhiteRegions, "area", "and", 0, WhiteNoiseSize);
			union1(SelectedWhiteRegions, &WhiteRegionUnion);
			union2(BlackNoiseRegion, WhiteRegionUnion, &BWRegionUnion);
			difference(RegionUnion1, BWRegionUnion, &RegionDifference);
			connection(RegionDifference, &RegionDifferences);
			select_gray(RegionDifferences, m_originImageMean, &SelectedRegions1, "deviation", "and", DeviationThred, 255);
			union1(SelectedRegions1, &TotalRegion);
			connection(TotalRegion, &TotalRegions);
			select_shape(TotalRegions, &SelectedRegions2, "area", "and", AreaThred, 9999999);
			union1(SelectedRegions2, &SelectedRegions2);
			intersection(SelectedRegions2, ROI, &DefectRegs);
		}
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
			dilation_circle (DefectRegs, &DefectRegs, 4);
			m_vErrorRgn[rId] = DefectRegs;
		}		
		retValue.Detach(pDetectArea);


#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;

}
STDMETHODIMP CBatteryShellAlgo::PYPartialDetectAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("3087$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 4028;LP;25;255;1;120$\
					 1372;FP;0;80;0.1;31.7$\
					 3091;LP;0;127;1;60$\
					 3092;LP;128;255;1;245$\
					 3093;LP;0;50;1;30$\
					 434;LP;0;500;1;185");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;

};

STDMETHODIMP CBatteryShellAlgo::PExDarkPointGradientDetectAlgo(VARIANT* rgnId, VARIANT* pEdgeThresh, VARIANT* pBlackAreaThred , VARIANT* pBlackErrorFilterThresh, VARIANT* pErrorDeviation, VARIANT* pDetectArea)
{
	//Bump Defect detection without black spots and white points
	int rId                       = (int)rgnId->fltVal;
	int EdgeThresh                = (int)pEdgeThresh->fltVal;
	int BlackArea                 = (int)pBlackAreaThred->fltVal;
	int BlackErrorFilterThresh    = (int)pBlackErrorFilterThresh->fltVal;
	int ErrorDeviation            = (int)pErrorDeviation->fltVal;
	CComVariant retValue;

#ifdef NDEBUG
	try
	{
#endif	
		if(m_bPilotImg)
		{
			return S_OK;
		}
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		Hobject ROI   = m_vRgn[rId];

		Hobject Image,EmptyObj;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? Image = m_originImageMean:Image = m_crtImg;
		(m_originImageMean.Id()!=H_EMPTY_REGION)? m_vInterImgs[rId]=m_originImageMean: m_vInterImgs[rId]=EmptyObj;

		if(ROI.Id() == H_EMPTY_REGION ) 
		{
			retValue = -1;
			m_vWhiteErrorRgn[rId].Reset();
			m_vErrorRgn[rId].Reset();
			m_vRgnExTest[rId].Reset();
			retValue.Detach(pDetectArea);
			return S_FALSE;
		}
		Hobject DefectRegs,ConnectedRegions,ResultRegionBlack,SelectedRegion;
		HTuple hv_Area, hv_Row, hv_Column;

		if(m_vROI[rId].size()>1 )
		{
			Concurrency::concurrent_vector<Hobject> vTargetRgnsBlack;
			//std::for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			parallel_for_each(m_vROI[rId].begin(),m_vROI[rId].end(), [&](HobjectPac& it)
			{
				Hobject DerivGauss,ImageResult,RegionDynThresh,RegionFillUp;
				Hobject OriginImageMean,NewImage,ImageResult1;
				HTuple hv_min, hv_max, hv_range,hv_MaxThresh;
				median_image (it.m_illuminateImage , &OriginImageMean, "circle", 3,"mirrored");
				derivate_gauss(OriginImageMean, &it.m_convertImage,3, "gradient");
				Hobject AffineRegion;
				threshold (it.m_convertImage, &RegionDynThresh,HTuple(EdgeThresh/10.0), 255);
				dilation_circle(RegionDynThresh,&RegionDynThresh,5.5 );
				move_region(RegionDynThresh,&AffineRegion,it.m_cornerRow,it.m_cornerCol);
				vTargetRgnsBlack.push_back(AffineRegion);

			});
			Hobject SelectedRegion,ConnectedThresholdRegion,ConnectedRegions;
			gen_empty_obj(&ResultRegionBlack);
			std::for_each(vTargetRgnsBlack.begin(),vTargetRgnsBlack.end(),[&ResultRegionBlack](Hobject rgn)
			{
				union2(ResultRegionBlack, rgn, &ResultRegionBlack);
			});

		}
		else
		{
			Hobject ImageReduced,ExpandImage,OriginImageMean,ConvertImage,RegionDynThresh;
			Hobject ConnectedRegions,SelectedRegion;
			Hobject RegionUnion,ImageReduced11,Region,RegionDifference,SelectedRegions;

			reduce_domain(Image, ROI,&ImageReduced);
			expand_domain_gray (ImageReduced,&ExpandImage, 15);
			median_image (ExpandImage , &OriginImageMean, "circle", 3,"mirrored");
			derivate_gauss(OriginImageMean, &ConvertImage,3, "gradient");
			threshold (ConvertImage, &ResultRegionBlack,HTuple(EdgeThresh/10.0), 255);

		}
		Hobject RegionUnion,ImageReduced,Region,RegionDifference,SelectedRegions,NoiseRegions;
		reduce_domain (	m_originImageMean, ResultRegionBlack, &ImageReduced);
		threshold (ImageReduced, &Region, 0, BlackErrorFilterThresh);
		difference (ResultRegionBlack, Region, &RegionDifference);
		/*******************************剔除波纹**********************************************/
		if (m_vRgnInterference.Id() != H_EMPTY_REGION)
		{
			difference(RegionDifference, m_vRgnInterference, &RegionDifference);
		}
		/*************************************************************************************/
		connection (RegionDifference, &ConnectedRegions);
		select_gray (ConnectedRegions, m_originImageMean, &SelectedRegions, "deviation", "and", ErrorDeviation, 255);
		closing_circle(SelectedRegions,&SelectedRegions,3.5);
		fill_up(SelectedRegions,&SelectedRegions);
		difference(SelectedRegions,m_whiteNoiseRegions,&SelectedRegions);
		intersection(SelectedRegions,ROI,&SelectedRegions);
		connection (SelectedRegions, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", BlackArea, m_imgWidth*m_imgHeight);
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
		m_vWhiteErrorRgn[rId].Reset();
		m_vErrorRgn[rId].Reset();
		m_vRgnExTest[rId].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CBatteryShellAlgo::PExDarkPointGradientDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("5021$\
					 397;R;LP;0;99999;1;<=#20$\
					 381;B$\
					 5020;LP;1;9999;1;45$\
					 434;LP;1;99999;1;10$\
					 5002;LP;1;9999;1;25$\
					 5001;LP;1;99999;1;20");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


//Bottom OCR matching location/detection algorithm
STDMETHODIMP CBatteryShellAlgo::MatchingParaAlgo(VARIANT* rgnId, VARIANT* pGreediness, BSTR* bstrShm1File, VARIANT* pMatchingResult)
{
	//模板匹配算子，计算匹配区域的中心、角度等参数。
	int		rId			= (int)rgnId->fltVal;
	float	greediness	= (float)pGreediness->fltVal;
	m_vErrorRgn[rId].Reset();
	m_vWhiteErrorRgn[rId].Reset();
	m_vRgnExTest[rId].Reset();
	CComVariant retValue;
	Hobject rgn	= m_vRgn[rId];
	Hobject	Image = m_crtImg;
	HTuple Width, Height;
	get_image_size(Image, &Width, &Height);	

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
				m_vErrorRgn[rId].Reset();
				m_vWhiteErrorRgn[rId].Reset();
				retValue.Detach(pMatchingResult);
				m_modelMatchingPARA[0]=0.0;
				m_modelMatchingPARA[1]=(Width/2)[0].D();
				m_modelMatchingPARA[2]=(Height/2)[0].D();
				m_modelMatchingPARA[3]=0.0;
				return S_FALSE;
			}
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}	
		CString sModelPath1  = *bstrShm1File;
		string strModelPath1 = CT2A(sModelPath1);
		if((m_dictCrtPathModelID.find(strModelPath1) == m_dictCrtPathModelID.end())||(rgn.Id() == H_EMPTY_REGION))
		{
			retValue = -1;
			m_vErrorRgn[rId].Reset();
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=(Width/2)[0].D();
			m_modelMatchingPARA[2]=(Height/2)[0].D();
			m_modelMatchingPARA[3]=0.0;
			return S_FALSE;
		}
		HTuple modelID = m_dictCrtPathModelID[strModelPath1];
		HTuple dataLength1;
		tuple_length (modelID, &dataLength1);
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
			m_vWhiteErrorRgn[rId].Reset();
			retValue.Detach(pMatchingResult);
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=(Width/2)[0].D();
			m_modelMatchingPARA[2]=(Height/2)[0].D();
			m_modelMatchingPARA[3]=0.0;
			return S_FALSE;
		}

		Hobject ImageReduced, Circle;
		Circle = rgn;
		reduce_domain(Image, Circle, &ImageReduced);
		m_modelMatchingPARA[0] = m_imageRadius;
		//匹配计算时，搜索范围与建模板的角度范围一致！
		HTuple NumLevels,AngleStart, AngleExtent, AngleStep, ScaleMin, ScaleMax, ScaleStep, Metric, MinContrast;
		HTuple Row_M, Column_M, Angle_M, Score_M, Model_M;
		get_shape_model_params (modelID, &NumLevels, &AngleStart, &AngleExtent, &AngleStep, &ScaleMin, &ScaleMax, &ScaleStep, &Metric, &MinContrast);
		if (AngleExtent<3.14)
		{
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
			retValue = Score_M[0].D();
			retValue.Detach(pMatchingResult);
		}
		else
		{
			m_modelMatchingPARA[0]=0.0;
			m_modelMatchingPARA[1]=(Width/2)[0].D();
			m_modelMatchingPARA[2]=(Height/2)[0].D();
			m_modelMatchingPARA[3]=0.0;
			retValue = 0.0;	
			retValue.Detach(pMatchingResult);
			m_vErrorRgn[rId] = Circle;
			return S_FALSE;
		}

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_modelMatchingPARA[0]=0.0;
		m_modelMatchingPARA[1]=0.0;
		m_modelMatchingPARA[2]=0.0;
		m_modelMatchingPARA[3]=0.0;
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
STDMETHODIMP CBatteryShellAlgo::MatchingParaAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("439$\
					 440;R;FP;0.0;1.0;0.01;>=#0.5$\
					 167;B$\
					 322;FP;0.2;1.0;0.05;0.85$\
					 535;SPM");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CBatteryShellAlgo::DetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize, BSTR* bstrRgnFile)
{
	//根据模板匹配结果定位检测区域
	int id = (int)rgnId->fltVal;
	float ErosionSize = (float)pErosionSize->fltVal;

	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
	CString sRgnPath1  = *bstrRgnFile;
	string strRgnPath1 = CT2A(sRgnPath1);
	Hobject newRgn;

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
		}

		m_vRgn[id] = newRgn;
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
		if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}

		

		Hobject RegionDetect;
		Hobject roiRgn = m_dictCrtPathRgn[strRgnPath1];
		Hobject  RegionAffineTrans;
		HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted;
		vector_angle_to_rigid(0, 0,0, 0, 0, m_modelMatchingPARA[3].D(), &HomMat2DRate);
		hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
		hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
		HTuple RowModelData,ColModelData;
		Hobject rotatedRgn;
		
		/*RowModelData=m_modelPARA[1];
		ColModelData=m_modelPARA[2];
		move_region (roiRgn, &RegionMoved1,-RowModelData, -ColModelData);        */
		projective_trans_region (roiRgn, &rotatedRgn, HomMat2DAdapted, "bilinear");
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
STDMETHODIMP CBatteryShellAlgo::DetectRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4024$\
					 167;B$\
					 443;FP;-10.5;10.5;0.5;1.0$\
					 267;SPR" );

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


void CBatteryShellAlgo::my_disp_obj(const Halcon::Hobject Object)
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
	//system("pause");

	write_object(Object, "D:/Region01.hobj");
	//write_image(m_crtImg, "tiff", 0,"E:/Image01.tiff");
}
bool CBatteryShellAlgo::SecondExposureImagerotation(BYTE pIdentifier, float pPhi)
{
	//identifier标识符  phi旋转角度
	Hobject Image = m_crtImg;
	Hobject ImageRotate;
	HTuple Width, Height;
	get_image_size (Image, &Width, &Height);
	if ((Width == Height)&&(pIdentifier == 1))
	{
		rotate_image (Image, &ImageRotate, pPhi, "constant");
		m_crtImg= ImageRotate;
		return true;
	}
	return false;
}
