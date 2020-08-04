#include "stdafx.h"
#include "resource.h"
#include "EoeAlgo_i.h"
#include "dllmain.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "EoeAlgo.h"

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

STDMETHODIMP CEoeAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IEoeAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CEoeAlgo::SetCurrentImage(LONG* pImg)
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

STDMETHODIMP CEoeAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CEoeAlgo::GetRegionEx(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnEx[rgnID];
	return S_OK;
}
STDMETHODIMP CEoeAlgo::GetRegionExTest(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnExTest[rgnID];
	return S_OK;
}

STDMETHODIMP CEoeAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CEoeAlgo::GetGlueInjectorID(LONG* pInjectorID)
{
	*pInjectorID = m_nCrtInjectorID;
	return S_OK;
}
STDMETHODIMP CEoeAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CEoeAlgo::GetWhiteErrorRegion(BYTE rgnID,  LONG* pWhiteErrorRgn)
{
	Hobject* pObj = (Hobject*)pWhiteErrorRgn;
	*pObj = m_vWhiteErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CEoeAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CEoeAlgo::GetCurrentLocationXY(BSTR* bstrLocationXY)
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

STDMETHODIMP CEoeAlgo::GetCenterLocation(BSTR* bstrLocationXY)
{
	//当前定位中心点，角度默认为0度。
	CString cstrXY;
	cstrXY.Format(_T("%.2f,%.2f,%.4f"),m_vPos[0].m_x,m_vPos[0].m_y,0.0);
	CComBSTR strLocationXY = cstrXY.AllocSysString();
	HRESULT hr = strLocationXY.CopyTo(bstrLocationXY);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
bool CEoeAlgo::ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA)
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

STDMETHODIMP CEoeAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
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

STDMETHODIMP CEoeAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
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

STDMETHODIMP CEoeAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}

STDMETHODIMP CEoeAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}

STDMETHODIMP CEoeAlgo::SaveResult(BSTR* strModelName,BYTE productID)
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

STDMETHODIMP CEoeAlgo::ResetModels(BYTE productID)
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

STDMETHODIMP CEoeAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
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
STDMETHODIMP CEoeAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
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
STDMETHODIMP CEoeAlgo::ModelCreate(double Radius, HTuple * ModelID1)
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

STDMETHODIMP CEoeAlgo::CenterLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
		if( Height-m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_x<m_locationRadius||Width-m_vPos[0].m_x<m_locationRadius)
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, Radius);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Circle;
			m_centerLocationRegion=m_vRgn[id];
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
STDMETHODIMP CEoeAlgo::CenterLocationAlgo1Help(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CenterLocationAlgoUsePointFitting(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold)
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
	gen_empty_obj(&newRgn);
	m_vRgn[id] = newRgn;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
		if (m_bPilotImg)
		{
			m_oldSearchRegRow = SearchRegRow;
			m_oldSearchRegCol = SearchRegCol;
			m_oldOuterRadius = OuterRadius;
			m_oldRoiWidth = RoiWidth;
			m_oldScanLineNum = ScanLineNum;
			m_oldScanLineDirection = ScanLineDirection;
			m_bIsRegParamChanged = false;
			return S_OK;
		}
		m_ImgIndex++;

		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);

		Hobject OuterCircle, InnerCircle, Donut, CenterPtCross;

		SearchRegRow= SearchRegRow + int(Height) / 2;
		SearchRegCol= SearchRegCol + int(Width) / 2;

		gen_circle(&OuterCircle,SearchRegRow, SearchRegCol, OuterRadius);			

		int InnerRadius = OuterRadius - RoiWidth;
		if (InnerRadius <= 0)
		{
			InnerRadius = 0;
			concat_obj(m_vRgnEx[id], OuterCircle, &m_vRgnEx[id]);
		}
		else
		{
			gen_circle(&InnerCircle, SearchRegRow, SearchRegCol, InnerRadius);
			difference(OuterCircle, InnerCircle, &Donut);
			concat_obj(m_vRgnEx[id], Donut, &m_vRgnEx[id]);
		}

		if (m_bDebugImg)
		{
			gen_cross_contour_xld(&CenterPtCross, SearchRegRow, SearchRegCol, 16, HTuple(45).Rad());				
			concat_obj(m_vRgnEx[id], CenterPtCross, &m_vRgnEx[id]);

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


		if (m_bPilotImg || m_ImgIndex <= 3 || (m_bDebugImg && m_bIsRegParamChanged))
		{
			m_measureHandles[id].CreateHandles(Width, Height, SearchRegRow, SearchRegCol, OuterRadius, RoiWidth, ScanLineNum, ScanLineDirection);

			m_bIsRegParamChanged = false;
		}

		bool result = m_measureHandles[id].LocateEdgePoints(Image, Transition, PointSelection, Sigma, ThresholdVal);


		HTuple CenterPtRow, CenterPtCol, CenterRadius;
		Hobject EdgePointCross,Contour;
		if (result)
		{
			m_measureHandles[id].FitCircleContour(m_measureHandles[id].EdgePtRows, m_measureHandles[id].EdgePtCols, &CenterPtRow, &CenterPtCol, &CenterRadius,&Contour);

			Hobject CircleFitted;
			gen_circle(&CircleFitted, CenterPtRow, CenterPtCol, CenterRadius);
			m_vRgn[id] = CircleFitted;
			m_locationRadius=CenterRadius;
			m_CenterRadius=CenterRadius;
			if (m_bDebugImg)
			{
				gen_cross_contour_xld(&EdgePointCross, m_measureHandles[id].EdgePtRows, m_measureHandles[id].EdgePtCols, 16, HTuple(45).Rad());				
				concat_obj(m_vRgnEx[id], EdgePointCross, &m_vRgnEx[id]);
				Hobject FitCircleCenterCross;
				gen_cross_contour_xld(&FitCircleCenterCross, CenterPtRow, CenterPtCol, 16, 0);
				concat_obj(m_vRgnEx[id], FitCircleCenterCross, &m_vRgnEx[id]);
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
		if( Height-m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_x<m_locationRadius||Width-m_vPos[0].m_x<m_locationRadius)
		{
			gen_circle(&Circle, Height/2.0, Width/2.0, m_locationRadius);
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
STDMETHODIMP CEoeAlgo:: CenterLocationAlgoUsePointFittingHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CEoeAlgo::CircleRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CEoeAlgo::CircleRegionLocationAlgo1Help(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::MatchingParaAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
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
STDMETHODIMP CEoeAlgo::MatchingParaAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::MatchingParaMinScoreAlgo(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
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
STDMETHODIMP CEoeAlgo::MatchingParaMinScoreAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::RotatingAngleDetectAlgo(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pBlackThredLow,VARIANT* pEdgeType,VARIANT* pEdgeDist, VARIANT* pDefectSize, VARIANT* pDetectAngle)
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
STDMETHODIMP CEoeAlgo::RotatingAngleDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::DetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
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
STDMETHODIMP CEoeAlgo::DetectRegionLocationAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::DetectRegionLocationExtractOCRAlgo(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pErosionSize, BSTR* bstrOcrRgnFile)
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
STDMETHODIMP CEoeAlgo::DetectRegionLocationExtractOCRAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::DetectOcrDistOffsetAlgo(VARIANT* rgnId,VARIANT* pDistanceOffset)
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

		if(m_imageNumber>1)
		{
			distNew=sqrt((m_modelMatchingPARA[1].D()-fCenterY)*(m_modelMatchingPARA[1].D()-fCenterY)+(m_modelMatchingPARA[2].D()-fCenterX)*(m_modelMatchingPARA[2].D()-fCenterX));
			m_meanDistAdd=m_meanDistAdd+distNew;
			m_addNumber=m_addNumber+1.0;			
			retValue=fabs((m_meanDistAdd/(m_addNumber+0.01))-distNew);			
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
STDMETHODIMP CEoeAlgo::DetectOcrDistOffsetAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::DetectRegionLocationAlgoOCR(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
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

STDMETHODIMP CEoeAlgo::DetectRegionLocationAlgoOCRHelp(BSTR* pHelpStr)
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


STDMETHODIMP CEoeAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
{
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
STDMETHODIMP CEoeAlgo::BlackDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::BlackDetectAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::MinMaxGrayDetectAlgo(VARIANT* rgnId, VARIANT* pBlackThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteThresh,VARIANT* pSeriousWhitePointSize,VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::MinMaxGrayDetectAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoBlackHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoBlack(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
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

STDMETHODIMP CEoeAlgo::DynThresholdAlgoBlackNumberHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoBlackNumber(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectNumber)
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

STDMETHODIMP CEoeAlgo::DynThresholdAlgoWhiteHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoWhite(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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


STDMETHODIMP CEoeAlgo::DynThresholdAlgoWhiteNumberHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoWhiteNumber(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectNumber)
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

STDMETHODIMP CEoeAlgo::DynThresholdAlgoBlackWhiteNumberHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoBlackWhiteNumber(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectNumber)
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

STDMETHODIMP CEoeAlgo::DynThresholdAlgoOCRHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoOCR(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize, VARIANT* pDetectArea)
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

STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("609$\
					 397;R;LP;0;1000;1;<=#20$\
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

STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdAlgoBW(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdAlgoBWHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdAlgoRotateImage(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pThresholdDark, VARIANT* pDefectSizeDark, VARIANT* pThresholdLight, VARIANT* pDefectSizeLight,VARIANT* pDefectArea)
{
	int id = (int)rgnId->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	HRegion WorkingRegion = m_vRgn[id];
	CComVariant retValue;
	if(WorkingRegion.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
	int ThresholdDark = (int)pThresholdDark->fltVal;
	int DefectSizeDark = (int) pDefectSizeDark->fltVal;
	float RotaAngle = (float)pRotaAngle->fltVal;
	int ThresholdLight = (int)pThresholdLight->fltVal;
	int DefectSizeLight = (int) pDefectSizeLight->fltVal;

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		float fCenterX = m_vPos[0].m_x;
		float fCenterY = m_vPos[0].m_y;
		float RoiWidth = m_ringPara[id].m_RoiWidth;
		float Radius = m_ringPara[id].m_Radius;

		HTuple HomMat2D, HomMat2DBack;
		vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, HTuple(RotaAngle).Rad(), &HomMat2D);
		vector_angle_to_rigid(fCenterY, fCenterX, HTuple(RotaAngle).Rad(), fCenterY, fCenterX, 0, &HomMat2DBack);
		HImage Image = m_crtImg;
		HImage ImageReduced = Image.ReduceDomain(WorkingRegion);
		HImage ImageAffinTrans = Image.AffineTransImage(HomMat2D, "nearest_neighbor", "false");

		//Dark Defect
		HRegion EdgeDark = ImageReduced.DynThreshold(ImageAffinTrans, ThresholdDark, "dark");
		HRegion EdgeDarkComparison = ImageReduced.DynThreshold(ImageAffinTrans, ThresholdDark, "light");
		HRegion EdgeLightRota = EdgeDarkComparison.AffineTransRegion(HomMat2DBack, "nearest_neighbor");
		HRegion RealEdgeDark = EdgeLightRota.Intersection(EdgeDark);
		HRegion ClosedDark = RealEdgeDark.ClosingCircle(2.5);
		HRegionArray EdgesDark = ClosedDark.Connection();
		HRegionArray BigEdgesDark = EdgesDark.SelectShape("area", "and", DefectSizeDark, 9999999);
		HRegion DarkDefect = DarkDefect.GenEmptyRegion();
		DarkDefect = DarkDefect.Union2(BigEdgesDark);

		//Light Defect
		HRegion EdgeLight = ImageReduced.DynThreshold(ImageAffinTrans, ThresholdLight, "light");
		HRegion EdgeLightComparison = ImageReduced.DynThreshold(ImageAffinTrans, ThresholdLight, "dark");
		HRegion EdgeDarkRota = EdgeLightComparison.AffineTransRegion(HomMat2DBack, "nearest_neighbor");
		HRegion RealEdgeLight = EdgeDarkRota.Intersection(EdgeLight);
		HRegion ClosedLight = RealEdgeLight.ClosingCircle(2.5);
		HRegionArray EdgesLight = ClosedLight.Connection();
		HRegionArray BigEdgesLight = EdgesLight.SelectShape("area", "and", DefectSizeLight, 9999999);
		HRegion LightDefect = LightDefect.GenEmptyRegion();
		LightDefect = LightDefect.Union2(BigEdgesLight);

		/*	GetLocalTime(&SysTime);
		long long Interval3 =  SysTime.wHour * 3600000 + SysTime.wMinute * 60000 + SysTime.wSecond * 1000 + SysTime.wMilliseconds - StartSec;*/

		//HTuple Columns, RowTrans, ColTrans;
		//HTuple Rows = WorkingRegion.GetRegionPoints(&Columns);
		//
		//affine_trans_pixel(Hom, Rows, Columns, &RowTrans, &ColTrans);
		////time1
		//GetLocalTime(&SysTime);
		//long long Time5 = SysTime.wHour * 3600000 + SysTime.wMinute * 60000 + SysTime.wSecond * 1000 + SysTime.wMilliseconds;
		//long long Interval5 =  Time5 - StartSec;

		//HTuple GrayVal = Image.GetGrayval(Rows, Columns);
		//HTuple GrayValRotated = Image.GetGrayval(RowTrans, ColTrans);
		//HTuple Diff = GrayVal - GrayValRotated;
		//HTuple LightIndices = GrayValueDifferenceSegment(Diff, LowerThresholdLight, LIGHT);
		//HTuple LightComparison = GrayValueDifferenceSegment(Diff, LowerThresholdLight, DARK);
		//HRegion EdgeLight = EdgeLight.GenRegionPoints(Rows.Select(LightIndices), Columns.Select(LightIndices));
		//HRegion EdgeLightCom = EdgeLightCom.GenRegionPoints(Rows.Select(LightComparison), Columns.Select(LightComparison));
		//HRegion AffineLight = EdgeLightCom.AffineTransRegion(HomBack, "nearest_neighbor");
		//HRegion Light = EdgeLight.Intersection(AffineLight);
		//HRegionArray LightEdges = Light.Connection();
		//HRegionArray SelectedLight = LightEdges.SelectShape("area", "and", DefectSizeLight, MAX_AREA);
		//HRegion LightDefect = LightDefect.GenEmptyRegion();
		//LightDefect = LightDefect.Union2(SelectedLight);	

		////dark
		//HTuple DarkIndices = GrayValueDifferenceSegment(Diff, LowerThresholdDark, DARK);
		//HTuple DarkComparison = GrayValueDifferenceSegment(Diff, LowerThresholdDark, LIGHT);
		//HRegion EdgeDark = EdgeDark.GenRegionPoints(Rows.Select(DarkIndices), Columns.Select(DarkIndices));
		//HRegion EdgeDarkCom = EdgeDarkCom.GenRegionPoints(Rows.Select(DarkComparison), Columns.Select(DarkComparison));
		//HRegion AffineDark = EdgeDarkCom.AffineTransRegion(HomBack, "nearest_neighbor");
		//HRegion Dark = EdgeDark.Intersection(AffineDark);			
		//HRegionArray DarkEdges = Dark.Connection();
		//HRegionArray SelectedDark = DarkEdges.SelectShape("area", "and", DefectSizeDark, MAX_AREA);
		//HRegion DarkDefect = DarkDefect.GenEmptyRegion();
		//DarkDefect = DarkDefect.Union2(SelectedDark);

		//		}	

		if(LightDefect.Area() <= 0)
			m_vWhiteErrorRgn[id] = Hobject();
		else
		{			
			HRegion DiaLight = LightDefect.DilationCircle(2.5);
			m_vWhiteErrorRgn[id] = DiaLight.Id();			
		}
		if(DarkDefect.Area() <= 0)
			m_vErrorRgn[id] = Hobject();
		else
		{
			HRegion DiaDark = DarkDefect.DilationCircle(2.5);
			m_vErrorRgn[id] = DiaDark.Id();
		}
		retValue = LightDefect.Area() + DarkDefect.Area();
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdAlgoRotateImageHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2014$\
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

STDMETHODIMP CEoeAlgo::GSurfaceAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::GSurfaceAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
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

STDMETHODIMP CEoeAlgo::GLineAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::GLineAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
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

STDMETHODIMP CEoeAlgo::GLineNewAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::GLineNewAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackThredHigh, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteThredHigh, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
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

STDMETHODIMP CEoeAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("432$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 433;LP;1;255;1;30$\
					 434;LP;1;500;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
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

STDMETHODIMP CEoeAlgo::GContoursinkAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
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

STDMETHODIMP CEoeAlgo::GContoursinkAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
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

void CEoeAlgo::ZoningRing(Hobject Image, Hobject Ring, Hobject MiddleCircle, Hobject* ZoningReg, HTuple CenterRow, HTuple  CenterColumn, int RingWidth, HTuple* tuple_RegMean_DCoffset)
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

double CEoeAlgo::Sinc(double n)
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

void CEoeAlgo::Unit_Impulse_Response(int paraLength, double angFreqStart, double angFreqCutoff, int winType, std::vector<double>& vFilterParam)
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

void CEoeAlgo::BandPass_Filter(std::vector<double>& vFilterParam, int ParaLength, std::vector<double>& vInputData, std::vector<double>& vOutputData)
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

void CEoeAlgo::PhaseAdjust(std::vector<double>& vOutputData, int FilterWidth)
{
	int outputSz = (int)vOutputData.size();
	std::vector<double> vOutTemp1;
	vOutTemp1.reserve(outputSz);

	vOutTemp1.assign(vOutputData.begin() + (outputSz-FilterWidth),  vOutputData.begin() + outputSz);
	vOutTemp1.insert(vOutTemp1.end(), vOutputData.begin(), vOutputData.begin() + outputSz - FilterWidth);
	std::copy(vOutTemp1.begin(), vOutTemp1.begin() + outputSz, vOutputData.begin());
}

void CEoeAlgo::Tuple_Replace(HTuple& tuple, HTuple Indices, HTuple ReplaceValue)
{
	HTuple  Length, SelectedIndice;
	tuple_length(Indices, &Length);
	for (int i = 0; i < Length; i++)
	{
		tuple_select(Indices, i, &SelectedIndice);
		tuple[SelectedIndice] = ReplaceValue;
	}
}

STDMETHODIMP CEoeAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pWinType, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
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

STDMETHODIMP CEoeAlgo::CurlDetectAlgo2Help(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CurlDetectAlgo2(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
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
STDMETHODIMP CEoeAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1;<=#20$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

STDMETHODIMP CEoeAlgo::EccentricDetectionAlgo(VARIANT* rgnId, VARIANT* pDistance)
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

STDMETHODIMP CEoeAlgo::EccentricDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("576$\
					 577;R;LP;0;100;1;<=#10$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::InnerCircleLocationAlgo(VARIANT* rgnId, VARIANT* pLowThred, VARIANT* pHighThred, VARIANT* pLowLength, VARIANT* pHighLength)
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

STDMETHODIMP CEoeAlgo::InnerCircleLocationAlgoHelp(VARIANT* pHelpStr)
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

STDMETHODIMP CEoeAlgo::ReprintDetectionAlgo(VARIANT* rgnId, VARIANT* pGrayDiff)
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

STDMETHODIMP CEoeAlgo::ReprintDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("583$\
					 586;R;LP;0;100;1;<=#20$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CEoeAlgo::ImgEnhancement(Hobject crtImg, Hobject rgn, Hobject &imgReduced)
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
STDMETHODIMP CEoeAlgo::WLineDetectionAlgo(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper, VARIANT* pDefectArea)
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

STDMETHODIMP CEoeAlgo::WLineDetectionAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::WSpotDetectionAlgo(VARIANT* rgnId, VARIANT* pSpotMaskSize, VARIANT* pSpotThred, VARIANT* pDefectArea)
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

STDMETHODIMP CEoeAlgo::WSpotDetectionAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::WBlockDetectionAlgo(VARIANT* rgnId, VARIANT* pBlockThred, VARIANT* pMinArea, VARIANT* pDefectArea)
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

STDMETHODIMP CEoeAlgo::WBlockDetectionAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::ReloadExternalFile(BSTR* bstrPath)
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

STDMETHODIMP CEoeAlgo::RRegionDetectAlgo(VARIANT* rgnId,VARIANT* pSmoothPara,VARIANT* pMaskSize,VARIANT* pThredLow,VARIANT* pARMin,VARIANT*pDetectArea )
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
STDMETHODIMP CEoeAlgo::RRegionDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::RRegionDetectBigAlgo(VARIANT* rgnId,VARIANT* pDiffWhite,VARIANT* pARMax,VARIANT*pDetectArea )
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
STDMETHODIMP CEoeAlgo::RRegionDetectBigAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::RRegionDetectExcludeEdgeAlgo(VARIANT* rgnId,VARIANT* pEdgeThresh,VARIANT* pEdgeLength,VARIANT* pErrorThresh,VARIANT* pARMin,VARIANT*pDetectArea )
{
	int id = (int)rgnId->fltVal;
	int EdgeThresh = (int)pEdgeThresh->fltVal;
	int EdgeLength=(int)pEdgeLength->fltVal;
	int ErrorThresh=(int)pErrorThresh->fltVal;
	int m_ARMin=(int)pARMin->fltVal;

	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	if(rgn_copy.Area() <= 0) 
	{
		retValue=-1;
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

		Hobject ImageReduced,Region,RegionClosing,ConnectedRegions;
		Hobject EdgeRegions,ErrorRegion,RegionDifference;

		Hobject Image=m_crtImg;
		RegionDifference=rgn;


		reduce_domain(Image,RegionDifference,&ImageReduced);
		threshold (ImageReduced, &Region, EdgeThresh, 255);
		connection (Region, &ConnectedRegions);
		select_shape (ConnectedRegions, &EdgeRegions, "outer_radius", "and", EdgeLength, 9999999);
		union1(EdgeRegions,&EdgeRegions );
		m_vRgnExTest[id]=EdgeRegions;
		threshold (ImageReduced, &Region, ErrorThresh, 255);
		difference(Region,EdgeRegions,&Region);
		connection(Region,&ConnectedRegions);
		select_shape (ConnectedRegions, &ErrorRegion, "area", "and", m_ARMin, 9999999);

		union1(ErrorRegion,&ErrorRegion);

		HTuple Area_White, Row_Test, Column_Test;
		area_center(ErrorRegion, &Area_White, &Row_Test, &Column_Test);
		/////////
		if(Area_White<=0)
		{
			retValue = 0;
		}
		else
		{
			retValue = Area_White[0].I();
		}
		dilation_circle (ErrorRegion, &ErrorRegion, 2.5);
		m_vErrorRgn[id] = ErrorRegion;
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
STDMETHODIMP CEoeAlgo::RRegionDetectExcludeEdgeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("8004$\
					 200;R;LP;1;1000;1;<=#20$\
					 167;B$\
					 8005;LP;1;255;1;200$\
					 8006;LP;1;999;1;15$\
					 8007;LP;1;255;1;240$\
					 421;LP;1;9999;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CEoeAlgo::ErrorDetect(Hobject ImageReduced, int MaskSize, float SeriousPointDynThresh, int SeriousPointSize, Hobject& DetectUnion, bool defectType)
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
void CEoeAlgo::SurfaceDent(Hobject Image)
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
STDMETHODIMP CEoeAlgo::GlueInjectorDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pGrayDiffValue, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::GlueInjectorDetectAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::GlueDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pGrayDiffValue, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::GlueDetectAlgoHelp(BSTR* pHelpStr)
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
void CEoeAlgo::SurfaceDent(Hobject Image,HTuple ThresholdWhite,HTuple ThresholdBlack,HTuple MeanDiffSet,HTuple DeviationDiffSet, Hobject *AnnularRegion, Hobject *InjectRegion)
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
STDMETHODIMP CEoeAlgo::GlueInjectorIDDetectAlgo(VARIANT* rgnId,VARIANT* pBlackPointThresh, VARIANT* pWhitePointThresh, VARIANT* pMeanGrayDiffValue, VARIANT* pDeviationGrayDiffValue, VARIANT* pMaxID, VARIANT* pDetectID)
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
STDMETHODIMP CEoeAlgo::GlueInjectorIDDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::GlueInjectorIDDetectAlgo1(VARIANT* rgnId,VARIANT* pRefreshIndex,VARIANT* pStartIndex, VARIANT* pTotalInjectorNum,VARIANT* pDetectID)
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
STDMETHODIMP CEoeAlgo::GlueInjectorIDDetectAlgo1Help(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::UVCurlAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::UVCurlAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
		if( Height-m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_x<m_locationRadius||Width-m_vPos[0].m_x<m_locationRadius)
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
STDMETHODIMP CEoeAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CenterLocationAlgoHS(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
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
		if( Height-m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_y< m_locationRadius || m_vPos[0].m_x<m_locationRadius||Width-m_vPos[0].m_x<m_locationRadius)
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
STDMETHODIMP CEoeAlgo::CenterLocationAlgoHSHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CircleRegionAngleAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
						if (StartAngle1 < 0)
							StartAngle1 += HTuple(360).Rad();
						if (EndAngle1 < 0)
							EndAngle1 += HTuple(360).Rad();

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
STDMETHODIMP CEoeAlgo::CircleRegionAngleAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::CircleRegionAngleAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
STDMETHODIMP CEoeAlgo::CircleRegionAngleAlgo1Help(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::SectorRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
STDMETHODIMP CEoeAlgo::SectorRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::SectorRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
STDMETHODIMP CEoeAlgo::SectorRegionLocationAlgo1Help(BSTR* pHelpStr)
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

bool CEoeAlgo::ReadModelsForOcr( BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,bool errorInfoLanguage,HTuple &m_modelsForOcr,HTuple& m_matrixForOcr,Hobject& m_modelImagesForOcr)
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
STDMETHODIMP CEoeAlgo::MultOcrDetectAlgo(VARIANT* rgnId, VARIANT* pGreediness, BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize,VARIANT* pLineDetectFlage,VARIANT* pLowContrast,VARIANT* pDilationSize,VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::MultOcrDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::PrintErrorDetectOcrAlgo(VARIANT* rgnId,VARIANT* pLowContrast,VARIANT* pHeightContrast,VARIANT* pDilationSize,VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::PrintErrorDetectOcrAlgoHelp(BSTR* pHelpStr)
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


STDMETHODIMP CEoeAlgo::MultOcrDetectExcludeTabAlgo(VARIANT* rgnId, VARIANT* pGreediness, BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File, BSTR* bstrExtractRgnFile,VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize,VARIANT* pLineDetectFlage,VARIANT* pLowContrast,VARIANT* pDilationSize,VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::MultOcrDetectExcludeTabAlgoHelp(BSTR* pHelpStr)
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


STDMETHODIMP CEoeAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

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
STDMETHODIMP CEoeAlgo::SetCurrentTaskName2(BSTR* bstrTaskName,BSTR* bstrChName,BSTR* bstrChNameWithoutCameraIndex)
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

STDMETHODIMP CEoeAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}

//Color image process, Algo.
STDMETHODIMP CEoeAlgo::CenterLocationForColorAlgoHS(VARIANT* rgnId,  VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
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
STDMETHODIMP CEoeAlgo::CenterLocationForColorAlgoHSHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::RingLocationForColorAlgoHS(VARIANT* rgnId,  VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pRegionWidth)
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
				m_ringPara[id].m_RoiWidth = (float)RadiusExt[0].D() ;
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
STDMETHODIMP CEoeAlgo::RingLocationForColorAlgoHSHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::CurlOptimizeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pFreqStart, VARIANT* pFreqCutoff,VARIANT* pWinType, VARIANT* pDefectType,VARIANT* pWaveThreshold,VARIANT* pMinArea, VARIANT* pDetectNumber)
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
STDMETHODIMP CEoeAlgo::CurlOptimizeForColorAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::DynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::DynThresholdForColorAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::EdgeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::EdgeForColorAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdForColorAlgoHelp(BSTR* pHelpStr)
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


STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdForColorAlgo1(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::AnnularRotaDynThresholdForColorAlgo1Help(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::DetectRegionLocationAlgoTabOCR(VARIANT* rgnId,VARIANT* pErosionSize, BSTR* bstrRgnFile)
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
STDMETHODIMP CEoeAlgo::DetectRegionLocationAlgoTabOCRHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoTabOCRHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::DynThresholdAlgoTabOCR(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize, VARIANT* pDetectArea)
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

STDMETHODIMP CEoeAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CEoeAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CEoeAlgo::ArrowDistOffsetAlgo(VARIANT* rgnId,VARIANT* pLowThred,VARIANT* pErosionSize,VARIANT* pBlackPointUpSize,VARIANT* pDistanceOffset)
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
STDMETHODIMP CEoeAlgo::ArrowDistOffsetAlgoHelp(BSTR* pHelpStr)
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

//铆钉尺寸检测
STDMETHODIMP CEoeAlgo::RivetModelCreate(double Radius, HTuple * ModelID1)
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
		create_scaled_shape_model(SyntheticModelImage, "auto", 0, 0, 0, 0.8, 1.2, "auto","auto", "use_polarity", 40, "auto", ModelID1);
		return S_OK;
	}
}
STDMETHODIMP CEoeAlgo::RivetCenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore,VARIANT* pThredScore)
{
	/*利用形状模板来定位圆*/
	int id = (int)rgnId->fltVal;
	double Radius = (double)pRadius->fltVal;
	float MinScore = (float)pMinScore->fltVal;

	float ThredScore = (float)pThredScore->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
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
		gen_ellipse (&Ellipse, Height/2.0, Width/2.0, 0.0, Width/2.0, Height/2.0);   //默认图片的大小有要求
		reduce_domain (Image, Ellipse, &ImageReduced);
		if(m_isFirst)
		{
			RivetModelCreate (Radius, &m_modelId);
			m_isFirst = false;
		}
		if(m_oldRadius != Radius)
		{
			clear_shape_model(m_modelId);
			RivetModelCreate (Radius, &m_modelId);
			m_oldRadius = Radius;
		}
		get_shape_model_contours (&ShapeModel, m_modelId, 1);
		find_scaled_shape_model (ImageReduced, m_modelId, 0, 0, 0.8, 1.2, MinScore, 1, 0.5, "least_squares", (HTuple(3).Append(-2)), 0.7, &CenterRow, &CenterColumn, &Angle, &Scale, &Score);
		if (0 != ((Score.Num()>0)&&(Score>ThredScore)))
		{
			vector_angle_to_rigid(0, 0, 0, CenterRow[0].D(), CenterColumn[0].D(), Angle[0].D(), &MovementOfObject);
			hom_mat2d_scale(MovementOfObject, Scale[0].D(), Scale[0].D(),CenterRow[0].D(), CenterColumn[0].D(), &MoveAndScaleOfObject);
			affine_trans_contour_xld(ShapeModel, &ModelAtNewPosition, MoveAndScaleOfObject);
			gen_region_contour_xld(ModelAtNewPosition, &Region, "filled");

			m_ShapeModelRegion=Region;
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

			Hobject EmptyRegion;
			gen_empty_region(&EmptyRegion);
			m_ShapeModelRegion=EmptyRegion;

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
STDMETHODIMP CEoeAlgo::RivetCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("890$\
					 381;B$\
					 374;LP;10;2000;1;192$\
					 377;FP;0.5;1.0;0.05;0.85$\
					 440;FP;0.5;1.0;0.01;0.80");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::RivetDiameterMeasureAlgo(VARIANT* rgnId,  VARIANT* pOffset, VARIANT* pSigma,VARIANT* pThredLower, VARIANT* pThredUpper,VARIANT* pCircularityValue,VARIANT* pDiameter)
{
	int id           = (int)rgnId->fltVal;
	float Offset   = (float)pOffset->fltVal;
	float Sigma   = (float)pSigma->fltVal;
	float ThredLower   = (float)pThredLower->fltVal;
	float ThredUpper = (float)pThredUpper->fltVal;
	if(ThredUpper<=ThredLower)
	{
		ThredUpper=ThredLower+3;
	}
	float CircularityValue = (float)pCircularityValue->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vInterImgs[id].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[id];
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		Hobject crtImg = m_crtImg;
		Hobject ImageReduced,ImageGauss,ExpandedImage,ImageGaussReduced,Edges;

		reduce_domain(crtImg, rgn, &ImageReduced);
		gauss_filter(ImageReduced, &ImageGauss, 5);
		expand_domain_gray(ImageGauss, &ExpandedImage, 2);
		reduce_domain(ExpandedImage, rgn, &ImageGaussReduced);
		edges_sub_pix(ImageGaussReduced, &Edges, "canny", double(Sigma), Hlong(ThredLower), Hlong(ThredUpper));
		Hobject UnionAdjacentContour,UnionCoCircleContour;
		union_adjacent_contours_xld(Edges, &UnionAdjacentContour, 10, 1, "attr_keep");
		union_cocircular_contours_xld(UnionAdjacentContour, &UnionCoCircleContour, 0.5, 0.1, 0.2, 30, 10, 10, "true", 1);
		HTuple Number;
		Hobject SelectedXLD,ObjectSelected;
		count_obj(UnionCoCircleContour, &Number);
		if (Number[0].I()==0)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDiameter);
			return S_FALSE;
		}
		HTuple Circularity;
		gen_empty_obj(&SelectedXLD);
		for (int i=1;i<=Number[0].I();i++)
		{
			select_obj(UnionCoCircleContour, &ObjectSelected, i);
			circularity_xld(ObjectSelected, &Circularity);
			if (0 != (Circularity<CircularityValue))
			{
				continue;
			}
			else
			{
				concat_obj(SelectedXLD, ObjectSelected, &SelectedXLD);
			}
		}
		HTuple Length,Ln,Num;
		count_obj(SelectedXLD,&Num);
		if (Num[0].I()==0)
		{
			retValue = -2;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDiameter);
			return S_FALSE;
		}
		length_xld(SelectedXLD,&Length);
		tuple_sort_index(Length,&Ln);
		Hobject OutXld;
		select_obj(SelectedXLD,&OutXld,(Ln.Select(Num-1))+1);
		Hobject SmoothedXld,ContEllipse;
		HTuple EllipseRow, EllipseColumn, EllipsePhi, EllipseRadius1, EllipseRadius2, StartPhi, EndPhi, PointOrder;
		smooth_contours_xld(OutXld, &SmoothedXld, 5);
		fit_ellipse_contour_xld(SmoothedXld, "fitzgibbon", -1, 0, 0, 200, 3, 2, &EllipseRow, &EllipseColumn, &EllipsePhi, &EllipseRadius1, &EllipseRadius2, &StartPhi, &EndPhi, &PointOrder);
		//gen_ellipse_contour_xld(&ContEllipse, EllipseRow, EllipseColumn, EllipsePhi, EllipseRadius1, EllipseRadius2, 0, 6.28318, "positive", 1.5);
		gen_ellipse(&ContEllipse, EllipseRow, EllipseColumn, EllipsePhi, EllipseRadius1, EllipseRadius2);

		//debug辅助显
		if(m_bDebugImg)
		{
			 concat_obj(m_vRgnExTest[id], ContEllipse, &m_vRgnExTest[id]);
		}

		HTuple Perimeter,DiameterPixel;
		Perimeter = ((2*PI)*EllipseRadius2)+(4*(EllipseRadius1-EllipseRadius2));
		DiameterPixel = Perimeter/PI;
		//像素当量
		HTuple PixelAccuracy,Diameter;
		PixelAccuracy = 0.0180582;
		Diameter = DiameterPixel*PixelAccuracy+Offset;

		retValue = Diameter[0].D();
		m_vErrorRgn[id] = ContEllipse;

		retValue.Detach(pDiameter);

#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDiameter);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::RivetDiameterMeasureAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("888$\
					 397;R;FP;0;300;1$\
					 381;B$\
					 889;FP;0.0;10.0;0.01;0.282$\
					 680;FP;0.1;10.0;0.1;1.0$\
					 762;FP;1;200;1;20$\
					 763;FP;1;254;1;40$\
					 633;FP;0.1;1;0.1;0.7");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CEoeAlgo::RivetDiameterMeasureAlgo1(VARIANT* rgnId,  VARIANT* pOffset, VARIANT* pDiameter)
{

	int id           = (int)rgnId->fltVal;
	float Offset   = (float)pOffset->fltVal;
	//float Sigma   = (float)pSigma->fltVal;
	//float ThredLower   = (float)pThredLower->fltVal;
	//float ThredUpper = (float)pThredUpper->fltVal;
	//if(ThredUpper<=ThredLower)
	//{
	//ThredUpper=ThredLower+3;
	//}
	//float CircularityValue = (float)pCircularityValue->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vInterImgs[id].Reset();
	CComVariant retValue;
	Hobject rgn = m_vRgn[id];
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
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
			m_dictSpecialData[id].clear();
		}
		//
		HTuple Area,Row,Col;
		area_center(m_ShapeModelRegion,&Area,&Row,&Col);
		if (Area==0)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDiameter);
			return S_FALSE;
		}
		HTuple Row1, Column1, Radius;
		smallest_circle(m_ShapeModelRegion, &Row1, &Column1, &Radius);

		if(m_bDebugImg)
		{
		    concat_obj(m_vRgnExTest[id], m_ShapeModelRegion, &m_vRgnExTest[id]);
		}

		HTuple DiameterPixel;
		DiameterPixel=Radius*2;
		//像素当量
		HTuple PixelAccuracy,Diameter;
		PixelAccuracy = 0.0180582;
		Diameter = DiameterPixel*PixelAccuracy+Offset;

		retValue = Diameter[0].D();
		m_vErrorRgn[id] = m_ShapeModelRegion;
		retValue.Detach(pDiameter);

#ifdef NDEBUG
	}
	catch (HException &except)
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDiameter);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::RivetDiameterMeasureAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("1241$\
					 397;R;FP;0;300;1$\
					 381;B$\
					 889;FP;0.0;10.0;0.01;0.282");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//方盖检测
STDMETHODIMP CEoeAlgo::CenterRectangleLocationAlgoHS(VARIANT* rgnId, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
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
STDMETHODIMP CEoeAlgo::CenterRectangleLocationAlgoHSHelp(BSTR* pHelpStr)
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


STDMETHODIMP CEoeAlgo::BoundaryRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CEoeAlgo::BoundaryRegionLocationAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CEoeAlgo::WeldAngleDetectAlgo(VARIANT* rgnId, VARIANT* pDetectAngle)
{
	int id = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	m_vErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	Hobject rgn = m_vRgn[id];
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
	if(RoiWidth<11.0)
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

		///////////
		Hobject  ImageReduced,RegionDifference, ConnectedRegions, RegionFillUp1;
		Hobject  SelectedRegions, DEMO, PolarTransImage;
		Hobject  DerivGauss, ImageScaleMax, ImageEmphasize1, ImageMean;
		Hobject  Rectangle, Region1, RegionDilation, RegionIntersection,RegionDilation2,RectangleT,ConnectedRegions2,ObjectSelectedE,RegionIntersection1;
		Hobject  ConnectedRegions1, SelectedRegions1, RegionLines,RegionUnion1;
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
		derivate_gauss(PolarTransImage, &DerivGauss, 2.5, "x");
		scale_image_max(DerivGauss, &ImageScaleMax);
		emphasize(ImageScaleMax, &ImageEmphasize1, 11, 3, 1.6);
		mean_image(ImageEmphasize1, &ImageMean, 3, 21);
		gen_rectangle2(&Rectangle, 0, 0, 0, Width1, Height1);
		gray_histo(Rectangle, ImageMean, &AbsoluteHisto, &RelativeHisto);
		Thred_Result = 0;  
		Thred_gray = 0;
		for (Index1=255; Index1>=0; Index1+=-1)
		{
			Thred_Result += HTuple(RelativeHisto[Index1]);
			if (0 != (Thred_Result>0.004))
			{
				Thred_gray = Index1;
				break;
			}
		}
		threshold(ImageMean, &Region1, Thred_gray, 255);
		dilation_rectangle1(Region1, &RegionDilation, 3, 11);
		intersection(Rectangle, RegionDilation, &RegionIntersection);
		connection(RegionIntersection, &ConnectedRegions1);
		dilation_rectangle1(ConnectedRegions1, &RegionDilation2, 2, HightResult/4.0);
		union1(RegionDilation2, &RegionUnion1);
		connection(RegionUnion1, &ConnectedRegions2);
		//area_center(ConnectedRegions2, &Area, &Row, &Column);
		//tuple_length(Area, &Length);
		//选择最长的特征区域，避免因个别区域比较宽，引起面积较大导致选择错误。
		smallest_circle(ConnectedRegions2, &Row3, &Column3, &Radius3);
		tuple_length(Radius3, &Length);
		tuple_sort_index(Radius3, &Area_sort);
		gen_rectangle1(&RectangleT, (3*Height1)/4, 0, Height1, Width1);
		//如果两个区域相差很小时，选择出两个区域？
		//select_shape(ConnectedRegions2, &SelectedRegions2, "area", "and", HTuple(Area[HTuple(Area_sort[Length-1])])-10, 	99999);
		//直接读取，不用面积选择，避免可能出现的“选择两个区域”情况
		select_obj(ConnectedRegions2, &ObjectSelectedE, HTuple(Area_sort[Length-1])+1);
		intersection(RectangleT, ObjectSelectedE, &RegionIntersection1);
		area_center(RegionIntersection1, &Area4, &Row4, &Column4);
		//With_Center := Column[Area_sort[Length-1]]
		With_Center = Column4;
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
			m_vErrorRgn[id] = RegionLines;
			//
			concat_obj(rgn,RegionLines,&rgn);
			m_vRgn[id]=rgn;
		}
		else
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
		}
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
STDMETHODIMP CEoeAlgo::WeldAngleDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("617$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CEoeAlgo::WeldAngleDetectGrayAlgo(VARIANT* rgnId, VARIANT* pAlgoSelect, VARIANT* pWeldWidth, VARIANT* pSmoothSize, VARIANT* pDetectAngle)
{
	int id = (int)rgnId->fltVal;
	int RoiWidth =(int) m_ringPara[id].m_RoiWidth;
	int Radius =(int) m_ringPara[id].m_Radius;
	int AlgoSelect = (int)pAlgoSelect->fltVal;
	int WeldWidth = (int)pWeldWidth->fltVal;
	int SmoothSize = (int)pSmoothSize->fltVal;
	m_vErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	Hobject rgn = m_vRgn[id];
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

		///////////
		Hobject  ImageReduced;
		Hobject  PolarTransImage;
		Hobject  Rectangle;

		//
		HTuple  RadiusResult;
		HTuple  WithResult, HightResult, WithResult_ext, Width1,Height1;
		HTuple  With_Center, Rate_Result, With_Point, Height_Point;

		//***************************************************************************************
		reduce_domain(Image, rgn, &ImageReduced);
		if(Radius<=RoiWidth)
		{
			RoiWidth=(int)(Radius-1.0);
		}
		////////////////////////////////////////////
		RadiusResult =Radius-(RoiWidth)/2.0;
		WithResult = (RadiusResult*3.14159*2).Round();
		HightResult =RoiWidth;
		WithResult_ext = WithResult*(6.9/(3.14159*2));
		polar_trans_image_ext(ImageReduced, &PolarTransImage, fCenterY, fCenterX, 0, 6.9,  Radius-RoiWidth,Radius, WithResult_ext, HightResult, "nearest_neighbor");
		get_image_size(PolarTransImage, &Width1, &Height1);
		//倒数和灰度值定位方法测试、、、、、、、、、、、、、
		Hobject Partitioned;
		HTuple Mean,Deviation,FunctionData,GrayDiff;
		gen_rectangle1(&Rectangle, 0, 0, Height1-1, Width1-1);
		partition_rectangle(Rectangle, &Partitioned, 1, Height1);
		intensity(Partitioned, PolarTransImage, &Mean, &Deviation);
		create_funct_1d_array(Mean, &FunctionData);

		if (0 != (AlgoSelect==0))
		{
			HTuple SmoothedFunction,SmoothedSelected;
			smooth_funct_1d_mean(FunctionData, SmoothSize, 2, &SmoothedFunction);
			tuple_select_range(SmoothedFunction, 3, Width1+2, &SmoothedSelected);
			tuple_sub(Mean, SmoothedSelected, &GrayDiff);
		}
		else
		{
			HTuple Derivative;
			derivate_funct_1d(FunctionData, "first", &Derivative);
			tuple_select_range(Derivative, 3, Width1+2, &GrayDiff);
		}
		HTuple Min,Indices1,LinePlaceX,ClipTuple,Mean1,MeanGray,Indices2;
		tuple_min(GrayDiff, &Min);
		tuple_find(GrayDiff, Min, &Indices1);
		if (0 != (HTuple(Indices1[0])<(WeldWidth*3)))
		{
			LinePlaceX = HTuple(Indices1[0])+WithResult;
		}
		else if (0 != ((HTuple(Indices1[0])+(WeldWidth*3))>WithResult_ext))
		{
			LinePlaceX = HTuple(Indices1[0])-WithResult;
		}
		else
		{
			LinePlaceX = Indices1[0];
		}
		MeanGray = HTuple();
		for (int Index1=-2*WeldWidth; Index1<=2*WeldWidth; Index1+=1)
		{
			tuple_select_range(Mean, (LinePlaceX+Index1)-(WeldWidth/2), ((LinePlaceX+Index1)+(WeldWidth/2))-1, &ClipTuple);
			tuple_mean(ClipTuple, &Mean1);
			MeanGray[Index1+(2*WeldWidth)] = Mean1;
		}
		tuple_sort_index(MeanGray, &Indices2);
		With_Center = (LinePlaceX-(2*WeldWidth))+((((HTuple(Indices2[0])+HTuple(Indices2[1]))+HTuple(Indices2[2]))/3.0).Round());
		/////////////////////////////////////////////////////////////
		if (0 != (With_Center<WithResult))
		{
			Rate_Result = ((With_Center*3.14159*2)/(WithResult+0.000001));
		}
		else if (0 != (With_Center>=WithResult))
		{
			Rate_Result = (((With_Center-WithResult)*3.14159*2)/(WithResult+0.000001));
		}
		///////////
		With_Point = fCenterX+(180*(Rate_Result.Cos()));
		Hobject RegionLines;
		Height_Point = fCenterY-(180*(Rate_Result.Sin()));
		gen_region_line(&RegionLines, Height_Point,With_Point,  fCenterY, fCenterX);

		m_angleDetect=Rate_Result[0].D();

		if((m_angleDetect>=0)&(m_angleDetect <6.284))
		{
			retValue =180.0*(m_angleDetect/(3.14159));
			m_vErrorRgn[id] = RegionLines;
			//
			concat_obj(rgn,RegionLines,&rgn);
			m_vRgn[id]=rgn;
		}
		else
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
		}
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
STDMETHODIMP CEoeAlgo::WeldAngleDetectGrayAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("690$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B$\
					 691;LP;0;1;1;0$\
					 692;LP;1;50;1;8$\
					 412;LP;1;50;1;8");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::WeldAngleDetectWhiteGlueAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("618$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B$\
					 433;LP;1;250;1;30$\
					 619;FP;0.1;50;0.1;5$\
					 620;LP;1;300;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CEoeAlgo::WeldAngleDetectWhiteGlueAlgo(VARIANT* rgnId, VARIANT* pDynThreshold, VARIANT* pAspectRationThresh, VARIANT* pClosingMaskWidth, VARIANT* pDetectAngle)
{
	int id = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	m_vErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	Hobject rgn = m_vRgn[id];
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
	//
	int DynThreshold = (int)pDynThreshold->fltVal;
	int AspectRationThresh = (int)pAspectRationThresh->fltVal;
	int ClosingMaskWidth = (int)pClosingMaskWidth->fltVal;
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
		if(Radius<=RoiWidth)
		{
			RoiWidth=(float)(Radius-1.0);
		}
		///////////
		HTuple  OuterRadius, InnerRadius, MiddleRadius, Circumference_ext, Circumference;
		HTuple  CenterRow, CenterColumn, RingWidth; //圆环参数
		HTuple  PolarImageWidth, PolarImageHeight;
		HTuple  MaskScale;
		HTuple  RectRow1,RectCol1, RectRow2, RectCol2; //区域外接最小矩形的相关参数
		HTuple  RectWidth, RectHeight, AspectRatio, Indices;

		Hobject PolarTransImage, ImageScaleMax, ImageMean;
		Hobject RegionDynThresh, ConnectedRegion, SelectedRegion;
		HTuple   ImageHeight, ImageWidth; 
		get_image_size(Image, &ImageWidth, &ImageHeight);
		//***************************************************************************************
		OuterRadius =Radius;
		InnerRadius = Radius-RoiWidth;
		CenterRow = m_vPos[0].m_y;
		CenterColumn =m_vPos[0].m_x;
		MiddleRadius = (OuterRadius + InnerRadius)/2;
		Circumference = MiddleRadius*2*PI;
		Circumference_ext = MiddleRadius * 6.88319;   //设定圆环展开的周长，展开角度大于360度
		RingWidth = OuterRadius - InnerRadius;

		polar_trans_image_ext(Image, &PolarTransImage, CenterRow, CenterColumn, 0, 6.68319, InnerRadius, OuterRadius, Circumference_ext, RingWidth, "nearest_neighbor");
		get_image_size(PolarTransImage, &PolarImageWidth, &PolarImageHeight);
		if(PolarImageHeight<5)
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		//new,用于去除三片罐焊缝区域中间的焊线对定位的影响（2015.07.10修改）
		gray_closing_rect (PolarTransImage, &PolarTransImage, PolarImageHeight*0.2, 5);

		//平滑模版尺寸比例，可兼容大尺寸的图像（如1280*1024）
		MaskScale = (ImageHeight * ImageWidth)/300000.0;
		scale_image_max(PolarTransImage, &ImageScaleMax);

		//修改参数值81为241，用于准确定位焊缝（2015.07.10修改）
		mean_image(ImageScaleMax, &ImageMean, MaskScale * 241, RingWidth*0.75); //25

		dyn_threshold(ImageScaleMax, ImageMean, &RegionDynThresh, DynThreshold, "light");  //可能要设置一个参数？
		//筛掉小的区域
		connection(RegionDynThresh, &ConnectedRegion);
		select_shape(ConnectedRegion, &SelectedRegion, "height", "and", 0.4*PolarImageHeight, PolarImageHeight);
		smallest_rectangle1(SelectedRegion, &RectRow1, &RectCol1, &RectRow2, &RectCol2);
		HTuple Length;
		tuple_length (RectRow2,& Length);
		if(Length<1)
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		RectWidth = (RectCol2 - RectCol1).Abs();
		RectHeight = (RectRow2 - RectRow1).Abs();
		AspectRatio =  1.0*RectWidth/RectHeight;
		tuple_find((AspectRatio - AspectRationThresh).Sgn(), -1, &Indices);  //阈值4可能会设置为调节参数？
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(DynThreshold));
			m_dictSpecialData[id].push_back(-999999.0f);
			HTuple Width, Height,AbsoluteHisto, RelativeHisto, Length,rateMax;
			Hobject ImageSub,Rectangle;
			get_image_size (ImageScaleMax,& Width, &Height);
			gen_rectangle1 (&Rectangle, 0, 0, Height-1, Width-1);
			abs_diff_image (ImageScaleMax, ImageMean, &ImageSub, 1);
			gray_histo (Rectangle, ImageSub, &AbsoluteHisto, &RelativeHisto);
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
		Hobject SelectedRegion1, RegionUnion, RegionClosing;
		Hobject ConnectedRegion1, ResultRegion, ResultRegion1, XYTransRegion, StdXYTransRegion;
		HTuple  WeldPolarCenterRow, WeldPolarCenterCol, WeldArea;
		HTuple  WeldCenterRow, WeldCenterCol, WeldCenterPosRad;

		//去掉横向的“长条”区域
		if (Indices != -1)
		{
			select_obj(SelectedRegion, &SelectedRegion1, Indices + 1);
			union1(SelectedRegion1, &RegionUnion);
			closing_rectangle1(RegionUnion, &RegionClosing, ClosingMaskWidth, 3);  //调节参数？
			connection(RegionClosing, &ConnectedRegion1);
			select_shape_std(ConnectedRegion1, &ResultRegion, "max_area", 0);
			shape_trans(ResultRegion, &ResultRegion1, "rectangle1");

			Hobject WeldCenterLine,StdWeldRect;
			HTuple  ULRow,ULCol,LRRow, LRCol;
			HTuple  WeldAngleDetected, StdWeldWidth, StdWeldAngle;

			//计算焊缝中心线的位置，要用展开的角度（6.68319）来算
			area_center(ResultRegion1, &WeldArea, &WeldPolarCenterRow, &WeldPolarCenterCol);

			WeldCenterPosRad = (WeldPolarCenterCol/Circumference_ext)*6.68319; 
			m_angleDetect = (WeldCenterPosRad.Fmod(2*3.14159)) [0].D();
			WeldCenterRow = CenterRow -(MiddleRadius*(WeldCenterPosRad.Sin()));
			WeldCenterCol = CenterColumn + (MiddleRadius*(WeldCenterPosRad.Cos()));
			gen_region_line(&WeldCenterLine,  WeldCenterRow, WeldCenterCol, CenterRow, CenterColumn);
			retValue =180.0*(m_angleDetect/(3.14159));
			m_vErrorRgn[id] = WeldCenterLine;
			//	
			concat_obj(rgn,WeldCenterLine,&rgn);
			//			m_vRgn[id]=rgn;
			//计算焊缝区域边界及其对应的焊缝中心点，起始角度、终止角度,2018.05.06, LukeChen添加
			/*			Hobject RegionBorder;
			HTuple  BorderRows, BorderCols;

			boundary(ResultRegion, &RegionBorder, "inner");
			get_region_points(RegionBorder, &BorderRows, &BorderCols);

			//根据ROI宽度来确定相关的节点线， >12时，选取1/3, 2/3处，小于12时，选取首尾点
			int FirstRowVal, SecondRowVal;

			if (RoiWidth >= 12.0)
			{
			FirstRowVal = (int)(RoiWidth/3.0);
			SecondRowVal =  (int)(RoiWidth*2.0/3.0);
			} 
			else
			{
			FirstRowVal = 0;
			SecondRowVal =  PolarImageHeight[0].I() -1;
			}


			HTuple Indices1, Indices2, Indices3, Indices4;
			tuple_find(BorderRows, FirstRowVal, &Indices1);
			tuple_find(BorderRows, SecondRowVal, &Indices2);

			if (Indices1.Num() > 2)
			{
			Indices1 = (Indices1.Select(0)).Concat(Indices1.Select(Indices1.Num() -1));
			}

			if (Indices2.Num() > 2)
			{
			Indices2 = (Indices2.Select(0)).Concat(Indices2.Select(Indices2.Num() -1));
			}

			tuple_sort_index(BorderCols.Select(Indices1), &Indices3);
			tuple_sort_index(BorderCols.Select(Indices2), &Indices4);

			HTuple PointRows1, PointCols1, Rho1;
			PointRows1.Reset();
			PointRows1.Append(BorderRows.Select(Indices1.Select(Indices3)));
			PointRows1.Append(BorderRows.Select(Indices2.Select(Indices4)));

			PointCols1.Reset();
			PointCols1.Append(BorderCols.Select(Indices1.Select(Indices3)));
			PointCols1.Append(BorderCols.Select(Indices2.Select(Indices4)));

			Rho1 = ((1.0*PointCols1)/PolarImageWidth)*6.68319;  //6.68319与polar_trans_image_ext保持一致

			//边缘点在原始图片上的位置
			HTuple PointRowsInOrigImg1, PointColsInOrigImg1;
			PointRowsInOrigImg1 = CenterRow-((OuterRadius-PointRows1)*(Rho1.Sin()));
			PointColsInOrigImg1 = CenterColumn+((OuterRadius-PointRows1)*(Rho1.Cos()));

			//求焊缝区域边界的交点
			HTuple WeldLineIntersectPointRow, WeldLineIntersectPointCol, IsLineOverlapping;
			intersection_lines(PointRowsInOrigImg1.Select(0), PointColsInOrigImg1.Select(0), 
			PointRowsInOrigImg1.Select(2), PointColsInOrigImg1.Select(2), PointRowsInOrigImg1.Select(1), 
			PointColsInOrigImg1.Select(1), PointRowsInOrigImg1.Select(3), PointColsInOrigImg1.Select(3), 
			&WeldLineIntersectPointRow, &WeldLineIntersectPointCol, &IsLineOverlapping);


			//////////////////////////////////////////////////////////////////////////
			Hobject PointCross;
			gen_cross_contour_xld(&PointCross, PointRowsInOrigImg1, PointColsInOrigImg1,  20, HTuple(45).Rad());
			concat_obj(rgn,PointCross,&rgn);

			Hobject Line1, Line2;
			gen_region_line(&Line1, WeldLineIntersectPointRow, WeldLineIntersectPointCol, PointRowsInOrigImg1.Select(0), PointColsInOrigImg1.Select(0));
			gen_region_line(&Line2, WeldLineIntersectPointRow, WeldLineIntersectPointCol, PointRowsInOrigImg1.Select(1), PointColsInOrigImg1.Select(1));
			concat_obj(rgn,Line1,&rgn);
			concat_obj(rgn,Line2,&rgn);
			//////////////////////////////////////////////////////////////////////////
			*/

			/*			HTuple RegRun_Row, RegRun_ColBegin, RegRun_ColEnd;
			get_region_runs(ResultRegion, &RegRun_Row, &RegRun_ColBegin, &RegRun_ColEnd);

			Hobject LeftContour, RightContour;
			Hobject LeftLineFitted, RightLineFitted;

			HTuple  LeftLineRowBegion, LeftLineColBegion, LeftLineRowEnd, LeftLineColEnd;
			HTuple  hv_Nr, hv_Nc, hv_Dist;
			HTuple  RightLineRowBegion, RightLineColBegion, RightLineRowEnd, RightLineColEnd;
			HTuple  hv_Nr1, hv_Nc1, hv_Dist1;

			gen_contour_polygon_xld(&LeftContour, RegRun_Row, RegRun_ColBegin);
			fit_line_contour_xld(LeftContour, "tukey", -1, 0, 5, 2, &LeftLineRowBegion, &LeftLineColBegion, &LeftLineRowEnd, &LeftLineColEnd, &hv_Nr, &hv_Nc, &hv_Dist);

			gen_contour_polygon_xld(&RightContour, RegRun_Row, RegRun_ColEnd);
			fit_line_contour_xld(RightContour, "tukey", -1, 0, 5, 2, &RightLineRowBegion, &RightLineColBegion, &RightLineRowEnd, &RightLineColEnd, &hv_Nr1, &hv_Nc1, &hv_Dist1);

			//转换到原始图片中
			HTuple PointRows1, PointCols1, Rho1;
			PointRows1.Reset();
			PointRows1.Append(LeftLineRowBegion);
			PointRows1.Append(LeftLineRowEnd);
			PointRows1.Append(RightLineRowBegion);
			PointRows1.Append(RightLineRowEnd);

			PointCols1.Reset();
			PointCols1.Append(LeftLineColBegion);
			PointCols1.Append(LeftLineColEnd);
			PointCols1.Append(RightLineColBegion);
			PointCols1.Append(RightLineColEnd);

			Rho1 = ((1.0*PointCols1)/PolarImageWidth)*6.68319;  //6.68319与polar_trans_image_ext保持一致

			//边缘点在原始图片上的位置
			HTuple PointRowsInOrigImg1, PointColsInOrigImg1;
			PointRowsInOrigImg1 = CenterRow-((OuterRadius-PointRows1)*(Rho1.Sin()));
			PointColsInOrigImg1 = CenterColumn+((OuterRadius-PointRows1)*(Rho1.Cos()));

			HTuple WeldLineIntersectPointRow, WeldLineIntersectPointCol, IsLineOverlapping;
			intersection_lines(PointRowsInOrigImg1.Select(0), PointColsInOrigImg1.Select(0), 
			PointRowsInOrigImg1.Select(1), PointColsInOrigImg1.Select(1), PointRowsInOrigImg1.Select(2), 
			PointColsInOrigImg1.Select(2), PointRowsInOrigImg1.Select(3), PointColsInOrigImg1.Select(3), 
			&WeldLineIntersectPointRow, &WeldLineIntersectPointCol, &IsLineOverlapping);

			//////////////////////////////////////////////////////////////////////////
			Hobject Line1, Line2;
			gen_region_line(&Line1, WeldLineIntersectPointRow, WeldLineIntersectPointCol, PointRowsInOrigImg1.Select(0), PointColsInOrigImg1.Select(0));
			gen_region_line(&Line2, WeldLineIntersectPointRow, WeldLineIntersectPointCol, PointRowsInOrigImg1.Select(2), PointColsInOrigImg1.Select(2));
			concat_obj(rgn,Line1,&rgn);
			concat_obj(rgn,Line2,&rgn);

			Hobject Cross1;
			gen_cross_contour_xld(&Cross1, PointRowsInOrigImg1, PointColsInOrigImg1, 16, HTuple(45).Rad());
			concat_obj(rgn,Cross1,&rgn);
			//////////////////////////////////////////////////////////////////////////
			*/			
			m_vRgn[id]=rgn;

		}
		else
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
		}
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
STDMETHODIMP CEoeAlgo::WeldAngleDetectGlueAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("937$\
					 397;R;FP;-10;370;1;>=#0$\
					 381;B$\
					 385;LP;-1;1;1;1$\
					 197;LP;0;21;1;4$\
					 433;LP;1;250;1;30$\
					 619;FP;0.1;50;0.1;5$\
					 620;LP;1;300;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
STDMETHODIMP CEoeAlgo::WeldAngleDetectGlueAlgo(VARIANT* rgnId, VARIANT* pBlackWhite,VARIANT* pEnhancePara,VARIANT* pDynThreshold, VARIANT* pAspectRationThresh, VARIANT* pClosingMaskWidth, VARIANT* pDetectAngle)
{
	int id = (int)rgnId->fltVal;
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	float Radius = m_ringPara[id].m_Radius;
	m_vErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	Hobject rgn = m_vRgn[id];
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
	//
	int BlackWhite = (int)pBlackWhite->fltVal;
	int EnhancePara = (int)pEnhancePara->fltVal;
	int DynThreshold = (int)pDynThreshold->fltVal;
	int AspectRationThresh = (int)pAspectRationThresh->fltVal;
	int ClosingMaskWidth = (int)pClosingMaskWidth->fltVal;
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
		if(Radius<=RoiWidth)
		{
			RoiWidth=(float)(Radius-1.0);
		}
		///////////
		HTuple  OuterRadius, InnerRadius, MiddleRadius, Circumference_ext, Circumference;
		HTuple  CenterRow, CenterColumn, RingWidth; //圆环参数
		HTuple  RectRow1,RectCol1, RectRow2, RectCol2; //区域外接最小矩形的相关参数
		HTuple  RectWidth, RectHeight, AspectRatio, Indices;


		HTuple   ImageHeight, ImageWidth; 
		get_image_size(Image, &ImageWidth, &ImageHeight);
		//***************************************************************************************
		OuterRadius =Radius;
		InnerRadius = Radius-RoiWidth;
		if(RoiWidth<5)
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		CenterRow = m_vPos[0].m_y;
		CenterColumn =m_vPos[0].m_x;
		MiddleRadius = (OuterRadius + InnerRadius)/2;
		Circumference = MiddleRadius*2*PI;
		double AngleExt=7.065;
		Circumference_ext = MiddleRadius *AngleExt;   //设定圆环展开的周长，展开角度大于360度
		RingWidth = OuterRadius - InnerRadius;
		//
		Hobject Circle1,Circle2,RegionDifference,RegionDynThresh;
		Hobject PolarTransRegion,RegionFillUp1,RegionClosing,ConnectedRegion, SelectedRegion,RegionOpening;
		Hobject ImageReduced,ImageMean,ImageResult,ImageAffinTrans;
		HTuple Mean, Deviation;
		gen_circle(&Circle1, CenterRow, CenterColumn, InnerRadius);
		gen_circle(&Circle2,CenterRow, CenterColumn, OuterRadius);
		difference(Circle2, Circle1, &RegionDifference);
		reduce_domain(Image, RegionDifference, &ImageReduced);
		//*
		mean_image(ImageReduced, &ImageMean, 3, 3);
		intensity(RegionDifference, ImageMean, &Mean, &Deviation);
		//*
		HTuple GammaLUT,HomMat2D;
		GammaLUT = HTuple();
		HTuple NewGray;
		NewGray = 0;
		for (int G=0; G<=255; G+=1)
		{
			NewGray = (((G/Mean).Pow(EnhancePara))*G).Round();
			if (0 != (NewGray>255))
			{
				NewGray = 255;
			}
			GammaLUT = GammaLUT.Concat(NewGray);
		}
		lut_trans(ImageMean, &ImageResult, GammaLUT);
		//*
		vector_angle_to_rigid(CenterRow, CenterColumn,0, CenterRow, CenterColumn, HTuple(45).Rad(), &HomMat2D);
		affine_trans_image(ImageResult, &ImageAffinTrans, HomMat2D, "constant", "false");
		if (BlackWhite==1)
		{
			dyn_threshold(ImageResult, ImageAffinTrans, &RegionDynThresh, DynThreshold, "light");
		}
		else if (BlackWhite==-1)
		{
			dyn_threshold(ImageResult, ImageAffinTrans, &RegionDynThresh, DynThreshold, "dark");
		}
		else if (BlackWhite==0)
		{
			dyn_threshold(ImageResult, ImageAffinTrans, &RegionDynThresh, DynThreshold, "not_equal");
		}
		else
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		//
		if (m_bDebugImg)
		{
			m_dictSpecialData[id].push_back(float(DynThreshold));
			m_dictSpecialData[id].push_back(-999999.0f);
			HTuple AbsoluteHisto, RelativeHisto, Length,rateMax,RegionArea,Row12,Col12;
			Hobject ImageSub;
			area_center(RegionDifference,&RegionArea,&Row12,&Col12);
			abs_diff_image (ImageResult, ImageAffinTrans, &ImageSub, 1);
			gray_histo (RegionDifference, ImageSub, &AbsoluteHisto, &RelativeHisto);
			tuple_length (AbsoluteHisto, &Length);
			tuple_max(RelativeHisto,&rateMax);
			int histoLength = Length[0].I();
			double scale=100.0/rateMax[0].D();
			float histoAddAr[256]={0.0};
			float histoAdd=0.0;
			for (int i=histoLength-1; i>=0; i--)
			{
				if(histoAdd*RegionArea[0].D()<200)
				{	
					histoAdd=histoAdd+(float)(RelativeHisto[i].D());
					histoAddAr[i]=histoAdd;
				}
				else
					histoAddAr[i]=histoAdd;
			}
			for (int i=0; i<histoLength; i++)
			{
				histoAdd=(float)(histoAddAr[i]*RegionArea[0].D());
				if(histoAdd<=200)
					m_dictSpecialData[id].push_back(-histoAdd);
				else
					m_dictSpecialData[id].push_back(-200.0);
			}
		}
		//
		polar_trans_region(RegionDynThresh, &PolarTransRegion,  CenterRow, CenterColumn, 0, AngleExt, InnerRadius, OuterRadius, Circumference_ext, RingWidth, "nearest_neighbor");
		fill_up(PolarTransRegion, &RegionFillUp1);	
		//用于去除三片罐焊缝区域中间的焊线(可能存在的黑条)对定位的影响
		if (ClosingMaskWidth<=5)
		{
			closing_rectangle1(RegionFillUp1, &RegionClosing, 1, 3);
		}
		else
		{
			closing_rectangle1(RegionFillUp1, &RegionClosing, Hlong(ClosingMaskWidth * 0.2), 3);
		}
		//筛掉小的区域
		connection(RegionClosing, &ConnectedRegion);
		select_shape(ConnectedRegion, &SelectedRegion, "height", "and", 0.6*RingWidth, RingWidth+8);
		if (ClosingMaskWidth<2)
		{
			ClosingMaskWidth=2;
			opening_rectangle1(SelectedRegion, &RegionOpening,1, 0.6*RingWidth);
		}
		else
		{
			opening_rectangle1(SelectedRegion, &RegionOpening, ClosingMaskWidth*0.6, 0.6*RingWidth);
		}
		smallest_rectangle1(RegionOpening, &RectRow1, &RectCol1, &RectRow2, &RectCol2);
		HTuple Length;
		tuple_length (RectRow2,& Length);
		if(Length<1)
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
			retValue.Detach(pDetectAngle);
			return S_FALSE;
		}
		RectWidth = (RectCol2 - RectCol1).Abs();
		RectHeight = (RectRow2 - RectRow1).Abs();
		AspectRatio =  1.0*RectWidth/RectHeight;
		tuple_find((AspectRatio - AspectRationThresh).Sgn(), -1, &Indices);  //阈值4可能会设置为调节参数？
		Hobject SelectedRegion1, RegionUnion, RegionClosing2;
		Hobject ConnectedRegion1, ResultRegion, XYTransRegion, StdXYTransRegion;
		HTuple  WeldPolarCenterRow, WeldPolarCenterCol, WeldArea;
		HTuple  WeldCenterRow, WeldCenterCol, WeldCenterPosRad;

		//去掉横向的“长条”区域
		if (Indices != -1)
		{
			select_obj(RegionOpening, &SelectedRegion1, Indices + 1);
			union1(SelectedRegion1, &RegionUnion);
			closing_rectangle1(RegionUnion, &RegionClosing2, Hlong(ClosingMaskWidth * 0.6), 3);  //调节参数？
			connection(RegionClosing2, &ConnectedRegion1);
			select_shape_std(ConnectedRegion1, &ResultRegion, "max_area", 0);
			shape_trans(ResultRegion, &ResultRegion, "rectangle1");

			Hobject WeldCenterLine,StdWeldRect;
			HTuple  ULRow,ULCol,LRRow, LRCol;
			HTuple  WeldAngleDetected, StdWeldWidth, StdWeldAngle;

			//计算焊缝中心线的位置，要用展开的角度（AngleExt）来算
			area_center(ResultRegion, &WeldArea, &WeldPolarCenterRow, &WeldPolarCenterCol);

			WeldCenterPosRad = (WeldPolarCenterCol/Circumference_ext)*AngleExt; 
			m_angleDetect = (WeldCenterPosRad.Fmod(2*3.14159)) [0].D();
			WeldCenterRow = CenterRow -(MiddleRadius*(WeldCenterPosRad.Sin()));
			WeldCenterCol = CenterColumn + (MiddleRadius*(WeldCenterPosRad.Cos()));
			gen_region_line(&WeldCenterLine,  WeldCenterRow, WeldCenterCol, CenterRow, CenterColumn);
			retValue =180.0*(m_angleDetect/(3.14159));
			m_vErrorRgn[id] = WeldCenterLine;
			//
			concat_obj(rgn,WeldCenterLine,&rgn);
			m_vRgn[id]=rgn;
		}
		else
		{
			retValue =-1;
			m_vErrorRgn[id] = rgn;
		}
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

STDMETHODIMP CEoeAlgo::WeldWhiteGlueRegExtractAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pWeldRegArea)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	Hobject rgn = m_vRgn[id];
	CComVariant retValue;
	HRegion rgn_copy = rgn;
	if(rgn_copy.Area() <= 0) 
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pWeldRegArea);
		return S_FALSE;
	}

	int BlackMaskSize = (int)pBlackMaskSize->fltVal;

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
		Hobject ImageReduced, WeldRegTemp, WeldRegTempFillUp;
		HTuple  UsedThreshold;
		Hobject WeldRegTempClosing;

		HTuple  WeldRegArea;

		reduce_domain(Image, rgn, &ImageReduced);
		binary_threshold(ImageReduced, &WeldRegTemp, "smooth_histo", "light", &UsedThreshold);
		fill_up(WeldRegTemp, &WeldRegTempFillUp);
		closing_circle(WeldRegTempFillUp, &WeldRegTempClosing, BlackMaskSize);
		//		opening_circle(WeldRegTempClosing, &WeldRegTempClosing, BlackMaskSize);
		union1(WeldRegTempClosing, &WeldRegTempClosing);

		region_features(WeldRegTempClosing, "area", &WeldRegArea);

		retValue = WeldRegArea[0].I();

		Hobject WeldNeighborReg;
		difference(rgn, WeldRegTempClosing, &WeldNeighborReg);

		HTuple DilationRadius = 3.5;
		Hobject Temp1, Temp2;
		Hobject WeldBoundary;

		dilation_circle(WeldRegTempClosing, &Temp1, DilationRadius);
		dilation_circle(WeldNeighborReg, &Temp2, DilationRadius);
		intersection(Temp1, Temp2, &WeldBoundary);
		intersection(WeldBoundary, rgn, &WeldBoundary);

		//		m_vErrorRgn[id] = WeldRegTempClosing;
		concat_obj(m_vRgn[id], WeldRegTempClosing, &m_vRgn[id]);	//显示定位结果

		m_WeldLocatedAlgoRegIndex = id;
		gen_empty_obj(&m_WeldRegRelated[id]);
		concat_obj(m_WeldRegRelated[id], WeldRegTempClosing, &m_WeldRegRelated[id]);
		concat_obj(m_WeldRegRelated[id], WeldNeighborReg, &m_WeldRegRelated[id]);
		concat_obj(m_WeldRegRelated[id], WeldBoundary, &m_WeldRegRelated[id]);
		retValue.Detach(pWeldRegArea);		

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pWeldRegArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::WeldWhiteGlueRegExtractAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1323$\
					 1324;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 412;LP;3;101;2;7");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::WeldRegSelectAlgo(VARIANT* rgnId, VARIANT* pWeldSelection)
{
	int id = (int)rgnId->fltVal;
	Hobject newRgn;
	int RegIndex = (int)pWeldSelection->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
#ifdef NDEBUG
	try
	{
#endif

		if (m_WeldLocatedAlgoRegIndex == -1)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}

		Hobject SelectReg;
		select_obj(m_WeldRegRelated[m_WeldLocatedAlgoRegIndex], &SelectReg, RegIndex+1);

		m_vRgn[id] = SelectReg ;	
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
STDMETHODIMP CEoeAlgo::WeldRegSelectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1325$\
					 167;B$\
					 1326;LP;0;1;1;0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::PhaseCorrelationAlgo(VARIANT* rgnId, VARIANT* pMode, VARIANT* pAngle)
{
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();

	int Mode = (int) pMode->fltVal;
	CComVariant retValue;
	HRegion WorkRegion  = m_vRgn[id];
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}	

		string ModelImgFile = m_strTaskPath;
		if(WorkRegion.Area() <= 0)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}

		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		HImage Image = m_crtImg;		
		HImage ImageReduced = Image.ReduceDomain(WorkRegion);
		float fCenterX;
		float fCenterY;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;

		double StartRadius = m_ringPara[id].m_Radius;
		double RingWidth = m_ringPara[id].m_RoiWidth;
		double EndRadius = StartRadius - RingWidth;
		double Circumference = 2 * PI * (StartRadius - 0.5 * RingWidth);
		double Angle;
		//将图像从极坐标转换到直角坐标
		/*write_region(WorkRegion.Id(), "C:\\workregion.reg");
		write_image(Image.Id(),"tiff",0,"C:\\image.tiff");
		write_image(ImageReduced.Id(),"tiff",0,"C:\\imageredu.tiff");*/
		HImage PolarTransImage = ImageReduced.PolarTransImageExt(fCenterY, fCenterX, 0,  HTuple(360).Rad(), StartRadius, EndRadius, Circumference, RingWidth, "nearest_neighbor");
		PolarTransImage = PolarTransImage.SobelAmp("sum_abs", 3);
		HTuple ChannelFolder,TaskFolder;
		ChannelFolder = HTuple(m_strChannelPath.c_str());
		if (_access(ChannelFolder[0].S(), 0))
		{
			CString  FolderDir( ChannelFolder[0].S());
			CreateDirectory(FolderDir, NULL);
		}
		//Generate the task directory if not exist
		if(_access(ModelImgFile.c_str(),0))
			CreateDirectoryA(ModelImgFile.c_str(), NULL);

		ModelImgFile.append("\\ModelImagePC.tiff");
		HRegion Show;
		if(Mode == 0)
		{         
			//创建模板图片      	
			//test show
			HRegion Line1 = Line1.GenRegionLine(250, 189, Hlong(fCenterY), Hlong(fCenterX));
			HRegion Line2 = Line2.GenRegionLine(274, 198, Hlong(fCenterY), Hlong(fCenterX));
			Show = Show.GenRegionLine(250, 189, 274, 198);
			Show = Show.Union2(Line1);
			Show = Show.Union2(Line2);

			/*	union2 (Show, Line1, &Show);
			union2 (Show, Line2, &Show);*/
			write_image(PolarTransImage.Id(), "tiff", 0, ModelImgFile.c_str());	

			m_StandardLine = Show.Id();
			m_vRgn[id] = Show.Id();
			m_OldCenterY = fCenterY;
			m_OldCenterX = fCenterX;
		}
		else
		{
			if(_access(ModelImgFile.c_str(),0))
			{
				retValue = -1;
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
				return S_FALSE;
			}
			//相位相关计算
			HImage ModelImage = ModelImage.ReadImage(ModelImgFile.c_str());
			ModelImage = ModelImage.SobelAmp("sum_abs", 3);//test sobel
			Hlong Width, Height, ModelWidth, ModelHeight;
			Width = PolarTransImage.GetImageSize(&Height);
			ModelWidth = ModelImage.GetImageSize(&ModelHeight);

			if(Width != ModelWidth || Height != ModelHeight)
			{
				retValue = -1;
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
				return S_FALSE;
			}
			HImage ImageFFT = PolarTransImage.RftGeneric("to_freq", "none", "complex", Width);
			HImage ImageFFTModel = ModelImage.RftGeneric("to_freq", "none", "complex", Width);
			HImage ImagePhaseCorrelationFFT = ImageFFTModel.PhaseCorrelationFft(ImageFFT);
			HImage ImagePhaseCorrelation = ImagePhaseCorrelationFFT.RftGeneric( "from_freq", "n", "real", Width);
			HRegionArray Region = Region.GenRectangle1(0, 0, (double)Height, (double)Width);
			HTuple Maximum = Region.GrayFeatures(ImagePhaseCorrelation, "max");

			if(Maximum.Num() == 1)
			{
				HRegion Maxi = ImagePhaseCorrelation.Threshold(Maximum, Maximum);
				Hlong Area;
				double RowMax, ColMax;
				Area = Maxi.AreaCenter(&RowMax, &ColMax);
				Angle = ColMax / Circumference * 360;
				HTuple HomMat2D;
				vector_angle_to_rigid(m_OldCenterY,m_OldCenterX,0,fCenterY, fCenterX, Angle,&HomMat2D);
				Show = HRegion(m_StandardLine);
				Show = Show.AffineTransRegion(HomMat2D, "nearest_neighbor");
				m_vRgn[id] = Show.Id();
				retValue = Angle;
			}
			else
			{
				retValue = -1;
				m_vErrorRgn[id] = WorkRegion.Id();
				return S_FALSE;
			}
			m_modelMatchingPARA[0] = m_imageRadius;
			m_modelMatchingPARA[1] = fCenterY;
			m_modelMatchingPARA[2] = fCenterX;
			m_modelMatchingPARA[3] = Angle  / 180 * PI;
		}	
		retValue.Detach(pAngle);	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::PhaseCorrelationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1337$\
					 1339;R;FP;-2;360;0.01;<=#0$\
					 381;B$\
					 1340;LP;0;1;1;1"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}



STDMETHODIMP CEoeAlgo::GetInterImgs(BYTE rgnID, LONG* pInterImgs)
{
	Hobject* pObj = (Hobject*)pInterImgs;
	*pObj = m_vInterImgs[rgnID];
	return S_OK;
}

void GetLineEnds(HRegion LineRegion, HTuple* RowStart, HTuple* ColStart, HTuple* RowEnd, HTuple* ColEnd)
{
	HTuple Rows, Cols, Indices, Length;
	Rows = LineRegion.GetRegionPoints(&Cols);

	tuple_find(Rows, Rows.Max(), &Indices);
	tuple_length(Indices, &Length);

	HTuple  RowsEnd, ColsEnd;
	HTuple  Distance;
	RowsEnd = HTuple();
	ColsEnd = HTuple();
	RowsEnd = RowsEnd.Concat(Rows.Max());
	ColsEnd = ColsEnd.Concat((Cols.Select(Indices)).Min());
	if (Length > 1)
	{
		RowsEnd = RowsEnd.Concat(Rows.Max());
		ColsEnd = ColsEnd.Concat((Cols.Select(Indices)).Max());
	}
	tuple_find(Rows, Rows.Min(), &Indices);
	tuple_length(Indices, &Length);
	RowsEnd = RowsEnd.Concat(Rows.Min());
	ColsEnd = ColsEnd.Concat((Cols.Select(Indices)).Min());
	if (Length > 1)
	{
		RowsEnd = RowsEnd.Concat(Rows.Min());
		ColsEnd = ColsEnd.Concat((Cols.Select(Indices)).Max());
	}
	tuple_find(Cols, Cols.Min(), &Indices);
	tuple_length(Indices, &Length);
	RowsEnd = RowsEnd.Concat((Rows.Select(Indices)).Max());
	ColsEnd = ColsEnd.Concat(Cols.Min());
	if (Length > 1)
	{
		RowsEnd = RowsEnd.Concat((Rows.Select(Indices)).Min());
		ColsEnd = ColsEnd.Concat(Cols.Min());
	}
	tuple_find(Cols, Cols.Max(), &Indices);
	tuple_length(Indices, &Length);
	RowsEnd = RowsEnd.Concat((Rows.Select(Indices)).Max());
	ColsEnd = ColsEnd.Concat(Cols.Max());
	if (Length > 1)
	{
		RowsEnd = RowsEnd.Concat((Rows.Select(Indices)).Min());
		ColsEnd = ColsEnd.Concat(Cols.Max());
	}
	tuple_length(RowsEnd, &Length);
	HTuple MaxDistance = 0;
	for (int i = 0; i <= Length - 2; i++)
	{
		for (int j = 1; j <= Length - 1; j++)
		{
			Distance = ((((RowsEnd.Select(i)) - (RowsEnd.Select(j))).Pow(2)) + (((ColsEnd.Select(i))-(ColsEnd.Select(j))).Pow(2))).Sqrt();
			if (Distance > MaxDistance)
			{
				MaxDistance = Distance;
				*RowStart = RowsEnd.Select(i);
				*ColStart = ColsEnd.Select(i);
				*RowEnd = RowsEnd.Select(j);
				*ColEnd = ColsEnd.Select(j);
			}
		}
	}
	return;
}

STDMETHODIMP CEoeAlgo::TabDeviationDetectAlgo(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pMinGray, VARIANT* pMaxGray, VARIANT* pTabLength, VARIANT* pDistanceDifference)
{
	//
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	HRegion WorkRegion = m_vRgn[id];
	CComVariant retValue;
	if(WorkRegion.Area() <= 0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistanceDifference);
		return S_FALSE;
	}
	float Threshold = (float)pThreshold->fltVal;
	int MinGray = (int)pMinGray->fltVal;
	int MaxGray = (int)pMaxGray->fltVal;
	int TabLength = (int)pTabLength->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;	
	if (Threshold > 255)
		Threshold = 255;

	if (MinGray > MaxGray)
		MinGray = MaxGray;

	HImage Image = m_crtImg;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		Hobject DisplayRegion;
		gen_empty_obj (&DisplayRegion);
		HImage ReducedImage = Image.ReduceDomain (WorkRegion);
		HImage ExpandedImage = ReducedImage.ExpandDomainGray(10);
		HImage DerivGauss = ExpandedImage.DerivateGauss(3, "gradient");
		HImage ReducedImageGauss = DerivGauss.ReduceDomain(WorkRegion);
		HRegion Edges = ReducedImageGauss.Threshold(Threshold, 255);
		HImage BoundaryImage = Image.ReduceDomain(Edges);
		HImage BoundaryMean = BoundaryImage.MeanImage(5, 5);
		HRegion BoundaryRough = BoundaryMean.Threshold(MinGray, MaxGray);
		HRegion BoundaryFill = BoundaryRough.FillUp();
		HRegionArray TargetRegions = BoundaryFill.Connection();
		HRegion TargetRegion = TargetRegions.SelectShapeStd("max_area", 1);
		Hlong Area;
		Area = TargetRegion.Area();
		if (Area < 3)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDistanceDifference);
			return S_FALSE;
		}
		HTuple RowsCont, ColsCont;
		HXLDContArray Contours = TargetRegion.GenContourRegionXld ("border");
		HRegion Border = TargetRegion.Boundary("inner");
		double RowInner, ColInner, RadiusInner;
		RowInner = TargetRegion.InnerCircle(&ColInner, &RadiusInner);
		HTuple RowFit, ColFit, RadiusFit, StartPhi, EndPhi, PointOrder;
		RowFit = Contours.FitCircleContourXld("atukey", -1, 0, 0, 4, 4, &ColFit, &RadiusFit, &StartPhi, &EndPhi, &PointOrder);
		double RowSmall, ColSmall, RadiusSmall;
		RowSmall = TargetRegion.SmallestCircle(&ColSmall, &RadiusSmall);
		HRegion FitCircle;
		FitCircle = FitCircle.GenCircle(RowFit, ColFit, (RadiusSmall + RadiusFit) * 0.5);
		HRegionArray TabRegions = Border.Difference(FitCircle);
		TabRegions = TabRegions.Connection();
		HTuple RowTab, ColTab, RadiusTab;
		RowTab = TabRegions.SmallestCircle(&ColTab, &RadiusTab);
		if(RadiusTab == HTuple(0))
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDistanceDifference);
			return S_FALSE;
		}
		HTuple RadiusMax;
		tuple_max(RadiusTab, &RadiusMax);
		HTuple GreaterIndices;
		if (RadiusMax < 3)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDistanceDifference);
			return S_FALSE;
		}
		else if (RadiusMax < TabLength * 0.3)
		{
			Hlong NumLump;
			count_obj(TabRegions.Id(), &NumLump);	
			HTuple HalfMaxRadius;
			tuple_gen_const (NumLump, 0.3 * RadiusMax, &HalfMaxRadius);
			HTuple GreaterRadius;
			tuple_greater_elem (RadiusTab, HalfMaxRadius, &GreaterRadius);		
			tuple_find (GreaterRadius, 1, &GreaterIndices);		
		}
		else
			tuple_find(RadiusTab, RadiusMax, &GreaterIndices);
		Hobject o_TabSelected;
		select_obj(TabRegions.Id(), &o_TabSelected, GreaterIndices + 1);
		HRegionArray TabSelected(o_TabSelected);
		HRegion LargestLump = TabSelected.Union1();
		HRegion TabSkeleton = LargestLump.Skeleton();
		HTuple RowSkeleton, ColSkeleton;
		RowSkeleton = TabSkeleton.GetRegionPoints(&ColSkeleton);
		HXLDCont TabContour;
		TabContour = TabContour.GenContourPolygonXld(RowSkeleton, ColSkeleton);
		double RowBegin, ColBegin, RowEnd, ColEnd, Nr, Nc, Dist;
		RowBegin = TabContour.FitLineContourXld("drop", -1, 0, 2, 1, &ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
		HRegion LineRegion = LineRegion.GenRegionLine(HTuple(RowBegin * 7 - RowEnd * 6), HTuple(ColBegin * 7 - ColEnd * 6), HTuple(RowEnd * 7 - RowBegin * 6), HTuple(ColEnd * 7 - ColBegin * 6));

		//4th revision
		HRegion LineDilation = LineRegion.DilationCircle(1.5);
		HRegion LineIntersection = LineDilation.Intersection(TargetRegion);
		HRegion LineSkeleton = LineIntersection.Skeleton();
		HTuple LineRowStart, LineColStart, LineRowEnd, LineColEnd;
		GetLineEnds(LineSkeleton, &LineRowStart, &LineColStart, &LineRowEnd, &LineColEnd);

		//determinant
		HTuple DistanceA = ((LineRowStart - RowFit).Pow(2) + (LineColStart - ColFit).Pow(2)).Sqrt();
		HTuple DistanceB = ((LineRowEnd - RowFit).Pow(2) + (LineColEnd - ColFit).Pow(2)).Sqrt();
		double Difference = (DistanceA - DistanceB).Abs()[0];
		//	m_vRgn[id] = DisplayRegion;
		retValue = Difference;
		retValue.Detach(pDistanceDifference);

		//display
		HRegion LineA, LineB;
		//TabLine = TabLine.GenRegionLine(RowStart, ColStart, RowEnd, ColEnd);
		LineA = LineA.GenRegionLine(LineRowStart, LineColStart, fCenterY, fCenterX);
		LineB = LineB.GenRegionLine(LineRowEnd, LineColEnd, fCenterY, fCenterX);
		concat_obj (DisplayRegion, LineSkeleton.Id(), &DisplayRegion);
		concat_obj (DisplayRegion, LineA.Id(), &DisplayRegion);
		concat_obj (DisplayRegion, LineB.Id(), &DisplayRegion);
		m_vRgnExTest[id] = DisplayRegion;
		//m_vRgn[id] = DisplayRegion;
		//pass parameters
		HTuple MiddlePointRow = (LineRowStart + LineRowEnd) * 0.5;
		HTuple MiddlePointCol = (LineColStart + LineColEnd) * 0.5;
		HTuple CenterAngle = (RowFit - MiddlePointRow).Atan2(MiddlePointCol - ColFit);
		m_modelMatchingPARA[0] = m_imageRadius;
		m_modelMatchingPARA[1] = RowFit[0];
		m_modelMatchingPARA[2] = ColFit[0];
		m_modelMatchingPARA[3] = CenterAngle[0];
		m_vPos[0].m_y = (float) RowFit[0].D();
		m_vPos[0].m_x = (float) ColFit[0].D();

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistanceDifference);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CEoeAlgo::TabDeviationDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1394$\
					 1396;R;FP;0;180;0.1;<=#20$\
					 381;B$\
					 1397;FP;0;20;0.05;1.5$\
					 191;LP;0;255;1;120$\
					 192;LP;0;255;1;200$\
					 1395;LP;3;200;1;150");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::TabDeviationDetectIndistinctEdgeAlgo(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pMinGray, VARIANT* pMaxGray, VARIANT* pTabLength, VARIANT* pDistanceDifference)
{
	//针对盖面边缘不明显的情况，只提取拉环区域，选取距离定位中心最远的若干点，拟合直线。
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	HRegion WorkRegion = m_vRgn[id];
	CComVariant retValue;
	if(WorkRegion.Area() <= 0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistanceDifference);
		return S_FALSE;
	}
	float Threshold = (float)pThreshold->fltVal;
	int MinGray = (int)pMinGray->fltVal;
	int MaxGray = (int)pMaxGray->fltVal;
	int TabLength = (int)pTabLength->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;	
	if (Threshold > 255)
		Threshold = 255;

	if (MinGray > MaxGray)
		MinGray = MaxGray;

	HImage Image = m_crtImg;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		Hobject DisplayRegion;
		gen_empty_obj (&DisplayRegion);
		HImage ReducedImage = Image.ReduceDomain (WorkRegion);
		HImage ExpandedImage = ReducedImage.ExpandDomainGray(10);
		HImage DerivGauss = ExpandedImage.DerivateGauss(3, "gradient");
		HImage ReducedImageGauss = DerivGauss.ReduceDomain(WorkRegion);
		HRegion Edges = ReducedImageGauss.Threshold(Threshold, 255);
		HRegion DilationRegion = Edges.DilationCircle(2.5);
		HImage BoundaryImage = Image.ReduceDomain(DilationRegion);
		//	HImage BoundaryMean = BoundaryImage.MeanImage(5, 5);
		HRegion BoundaryRough = BoundaryImage.Threshold(MinGray, MaxGray);
		HRegion BoundaryFill = BoundaryRough.FillUp();
		HRegionArray TargetRegions = BoundaryFill.Connection();
		HRegionArray SelectedRegions = TargetRegions.SelectShape("outer_radius", "and", TabLength * 0.4, TabLength * 0.6);
		Hlong Number;
		count_obj(SelectedRegions.Id(), &Number);
		if (Number < 1)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDistanceDifference);
			return S_FALSE;
		}
		HRegion SelectedRegion = SelectedRegions.Union1();
		HRegion SkeletonRegion = SelectedRegion.Skeleton();
		HTuple RowSkeleton, ColSkeleton, Indices, Length, SelectedIndices;
		RowSkeleton = SkeletonRegion.GetRegionPoints(&ColSkeleton);
		HTuple DistanceSquared = (RowSkeleton - HTuple(fCenterY)).Pow(2) + (ColSkeleton - HTuple(fCenterX)).Pow(2);
		tuple_sort_index(DistanceSquared, &Indices);
		HTuple SortedRow = RowSkeleton.Select(Indices);
		HTuple SortedCol = ColSkeleton.Select(Indices);	
		tuple_length(DistanceSquared, &Length);
		tuple_gen_sequence(Length - 30, Length - 1, 1, &SelectedIndices);
		HRegion LineEnds = LineEnds.GenRegionPoints(SortedRow.Select(SelectedIndices), SortedCol.Select(SelectedIndices));
		HTuple LineRowStart, LineColStart, LineRowEnd, LineColEnd;
		GetLineEnds(LineEnds, &LineRowStart, &LineColStart, &LineRowEnd, &LineColEnd);

		//determinant
		HTuple DistanceA = ((LineRowStart - fCenterY).Pow(2) + (LineColStart - fCenterX).Pow(2)).Sqrt();
		HTuple DistanceB = ((LineRowEnd - fCenterY).Pow(2) + (LineColEnd - fCenterX).Pow(2)).Sqrt();
		double Difference = (DistanceA - DistanceB).Abs()[0];
		m_vRgn[id] = DisplayRegion;
		retValue = Difference;
		retValue.Detach(pDistanceDifference);

		//display
		HRegion TabLine = TabLine.GenRegionLine(LineRowStart, LineColStart, LineRowEnd, LineColEnd);
		HRegion LineA = LineA.GenRegionLine(LineRowStart, LineColStart, fCenterY, fCenterX);
		HRegion LineB = LineB.GenRegionLine(LineRowEnd, LineColEnd, fCenterY, fCenterX);

		concat_obj (DisplayRegion, TabLine.Id(), &DisplayRegion);
		concat_obj (DisplayRegion, LineA.Id(), &DisplayRegion);
		concat_obj (DisplayRegion, LineB.Id(), &DisplayRegion);
		m_vRgn[id] = DisplayRegion;
		//pass parameters
		HTuple MiddlePointRow = (LineRowStart + LineRowEnd) * 0.5;
		HTuple MiddlePointCol = (LineColStart + LineColEnd) * 0.5;
		HTuple CenterAngle = (fCenterY - MiddlePointRow).Atan2(MiddlePointCol - fCenterX);
		m_modelMatchingPARA[0] = m_imageRadius;
		m_modelMatchingPARA[1] = fCenterY;
		m_modelMatchingPARA[2] = fCenterX;
		m_modelMatchingPARA[3] = CenterAngle[0];
		/*	m_vPos[0].m_y = (float) RowFit[0].D();
		m_vPos[0].m_x = (float) ColFit[0].D();
		*/
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDistanceDifference);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CEoeAlgo::TabDeviationDetectIndistinctEdgeAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2005$\
					 1396;R;FP;0;180;0.1;<=#20$\
					 381;B$\
					 1397;FP;0;20;0.05;1.5$\
					 191;LP;0;255;1;120$\
					 192;LP;0;255;1;200$\
					 1395;LP;3;200;1;150");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::AngleRegistrationAlgo(VARIANT* rgnId, VARIANT* pPatternWidth, VARIANT* pThreshold, VARIANT* pAngle)
{
	//奶粉盖拉环角度定位
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	int PatternWidth = (int) pPatternWidth->fltVal / 2;
	float Threshold = (float)pThreshold->fltVal;
	CComVariant retValue;
	HRegion WorkRegion  = m_vRgn[id];
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}	
		if(WorkRegion.Area() <= 0)
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}

		float fCenterX = m_vPos[0].m_x;
		float fCenterY = m_vPos[0].m_y;
		double StartRadius = m_ringPara[id].m_Radius;
		double RingWidth = m_ringPara[id].m_RoiWidth;
		double EndRadius = StartRadius - RingWidth;
		double R0 = StartRadius - RingWidth * 0.5;

		//提取盖面边缘区域，根据区域重心粗略确定拉环角度
		HImage Image = m_crtImg;
		HImage ImageReduced = Image.ReduceDomain(WorkRegion);
		HImage ImageAmp = ImageReduced.SobelAmp("sum_abs", 3);
		HRegion Edge = ImageAmp.Threshold(Threshold, 255);
		HRegionArray Edges = Edge.Connection();
		HRegionArray Patterns = Edges.SelectShape("area", "and", 5, MAX_AREA);

		HTuple RowPatternUnused, ColPatternUnused;
		HTuple AreaPattern = Patterns.AreaCenter(&RowPatternUnused, &ColPatternUnused);
		if(AreaPattern == HTuple())
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}

		HRegion Pattern = Patterns.Union1();
		double RowPattern, ColPattern;
		Hlong Area = Pattern.AreaCenter(&RowPattern, &ColPattern);
		double CenterAngle = atan2(fCenterY - RowPattern, ColPattern - fCenterX);
		//在附近生成多个矩形，边缘最多的矩形即为拉环精确位置
		HTuple SearchAngle = HTuple(5).Rad();
		HTuple SearchStep = HTuple(1).Rad();
		HTuple Phis, Length, CenterRow, CenterCol, Width, LengthRec, Number, Areas, LengthMax;
		tuple_gen_sequence(CenterAngle - SearchAngle, CenterAngle + SearchAngle, SearchStep, &Phis);
		tuple_length(Phis, &Length);
		tuple_gen_const(Length, fCenterY, &CenterRow);
		CenterRow = CenterRow - R0 * Phis.Sin();
		tuple_gen_const(Length, fCenterX, &CenterCol);
		CenterCol = CenterCol + R0 * Phis.Cos();
		tuple_gen_const(Length, RingWidth * 0.5, &Width);
		tuple_gen_const(Length, PatternWidth * 0.5, &LengthRec);
		HRegionArray Rectangles = Rectangles.GenRectangle2(CenterRow, CenterCol, Phis, Width, LengthRec);
		count_obj(Rectangles.Id(), &Number);
		for(int i = 1; i < Number; i++)
		{
			Hobject SelectedObj;
			select_obj(Rectangles.Id(), &SelectedObj, i);
			HRegion SelectedRegion = SelectedObj;
			HRegion Edge = Pattern.Intersection(SelectedRegion);
			Hlong AreaEdge = Edge.Area();
			Areas.Append(AreaEdge);
		}
		HTuple MaxIndex = Areas.Find(Areas.Max());
		tuple_length(MaxIndex, &LengthMax);
		Hobject TargetObj;
		select_obj(Rectangles.Id(), &TargetObj, MaxIndex + 1);
		HRegionArray TargetRegion = TargetObj;
		HTuple Unused, RowRegis, ColRegis;
		Unused = TargetRegion.AreaCenter(&RowRegis, &ColRegis);	
		HTuple AngleRad = (HTuple(fCenterY) - HTuple(RowRegis).Mean()).Atan2(HTuple(ColRegis).Mean() - HTuple(fCenterX));
		if (AngleRad < 0)
			AngleRad += HTuple(360).Rad();

		HRegion DisplayRec = DisplayRec.GenRectangle2(fCenterY - R0 * AngleRad.Sin(), R0 * AngleRad.Cos() + fCenterX, AngleRad, RingWidth*0.5, PatternWidth);
		HRegion DisplayEdge = DisplayRec.Intersection(Pattern);
		Hobject DisplayRegion;
		concat_obj(DisplayRegion, DisplayRec.Id(), &DisplayRegion);
		concat_obj(DisplayRegion, DisplayEdge.Id(), &DisplayRegion);

		m_vRgnExTest[id] = DisplayRegion;
		retValue = AngleRad[0].D() / PI * 180;
		m_modelMatchingPARA[0] = m_imageRadius;
		m_modelMatchingPARA[1] = fCenterY;
		m_modelMatchingPARA[2] = fCenterX;
		m_modelMatchingPARA[3] = AngleRad; 	
		retValue.Detach(pAngle);	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::AngleRegistrationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1414$\
					 1339;R;FP;-2;360;0.01;<=#0$\
					 381;B$\
					 1413;LP;0;200;1;16$\
					 1397;FP;0;200;0.1;15"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::AngleRegistrationAlgo1(VARIANT* rgnId, VARIANT* pClosingSize, VARIANT* pThreshold, VARIANT* pAngle)
{
	//通过边缘找角度的算法
	int id = (int)rgnId->fltVal;
	float ClosingSize = (float) pClosingSize->fltVal;
	float Threshold = (float)pThreshold->fltVal;
	CComVariant retValue;
	HRegion WorkRegion  = m_vRgn[id];
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}	
		if(WorkRegion.Area() <= 0)
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}

		float fCenterX = m_vPos[0].m_x;
		float fCenterY = m_vPos[0].m_y;



		//提取盖面边缘区域，根据区域重心粗略确定拉环角度

		HImage Image = m_crtImg;
		HImage ImageReduced = Image.ReduceDomain(WorkRegion);
		HImage ImageAmp = ImageReduced.SobelAmp("sum_abs", 3);
		HRegion Edge = ImageAmp.Threshold(Threshold, 255);
		HRegionArray Edges = Edge.Connection();
		HRegionArray Patterns = Edges.SelectShape("area", "and", 5, MAX_AREA);
		HRegionArray DilationRegion=Patterns.DilationCircle(ClosingSize);
		HRegion UnionRegion=DilationRegion.Union1();
		HRegion ErosionRegion=UnionRegion.ErosionCircle(ClosingSize);
		HRegionArray RegionConnected=ErosionRegion.Connection();
		HRegionArray RegionSelected = RegionConnected.SelectShapeStd("max_area",70);
		HTuple CenterRow,CenterCol;
		HTuple RegionArea=RegionSelected.AreaCenter(&CenterRow,&CenterCol);
		if(CenterRow.Num()>0 &&RegionArea>0)
		{
			Hobject RegionLines;
		    HTuple AngleLine = (HTuple(CenterRow) - HTuple(fCenterY)).Atan2(HTuple(fCenterX) - HTuple(CenterCol));
			if (AngleLine < 0)
				AngleLine += HTuple(360).Rad();
			gen_region_line(&RegionLines,fCenterY, fCenterX,CenterRow,CenterCol);
			//orientation_region(RegionLines,&AngleLine);
			m_vRgnExTest[id] = RegionLines;
			retValue = AngleLine[0].D() / PI * 180;
			m_modelMatchingPARA[0] = m_imageRadius;
			m_modelMatchingPARA[1] = fCenterY;
			m_modelMatchingPARA[2] = fCenterX;
			m_modelMatchingPARA[3] = AngleLine; 	
			retValue.Detach(pAngle);	
		}
		else
		{
			retValue = -1;
			m_vRgnExTest[id] = Edge.Id();
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;

		}

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::AngleRegistrationAlgo1Help(BSTR* pHelpStr)
{
	CComBSTR strHelp("8008$\
					 1339;R;FP;-2;360;0.01;<=#0$\
					 381;B$\
					 8009;FP;0;100;0.5;16$\
					 433;FP;0;254;0.1;15"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


double regulateAngleRange(double inputAngle)
{
	while(1)
	{
		if(inputAngle < 0)
			inputAngle += HTuple(360).Rad()[0].D();
		else if(inputAngle >= HTuple(360).Rad()[0].D())
			inputAngle -= HTuple(360).Rad()[0].D();
		else 
			return inputAngle;
	}
	
}
STDMETHODIMP CEoeAlgo::AngleRegistrationRotateImageAlgo(VARIANT* rgnId, VARIANT* pDynThreshold, VARIANT* pAreaThreshold, VARIANT* pTabLength, VARIANT* pParalleTab, VARIANT* pAngle)
{
	//小盖型奶粉盖拉环角度定位，用旋转动态灰度值提取拉环一端，再根据拉环宽度确定另一端。
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
//	int PatternWidth = (int) pPatternWidth->fltVal / 2;
	int AreaThreshold = (int)pAreaThreshold->fltVal;
	int DynThreshold = (int)pDynThreshold->fltVal;
	int ParallelTab = (int)pParalleTab->fltVal;
	int TabLength = (int)pTabLength->fltVal;
	CComVariant retValue;
	HRegion WorkRegion  = m_vRgn[id];
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}	
		if(WorkRegion.Area() <= 0)
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		float fCenterX = m_vPos[0].m_x;
		float fCenterY = m_vPos[0].m_y;
		double StartRadius = m_ringPara[id].m_Radius;
		double RingWidth = m_ringPara[id].m_RoiWidth;
		double EndRadius = StartRadius - RingWidth;
		//
		HImage Image = m_crtImg;
		HImage ImageReduced = Image.ReduceDomain(WorkRegion);
		HTuple Hom;
		vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY,fCenterX, HTuple(0.7).Rad(), &Hom);
        HImage AffineImage = ImageReduced.AffineTransImage(Hom, "bilinear", "false");
		HRegion RegionDynThresh = ImageReduced.DynThreshold(AffineImage, DynThreshold, "not_equal");
		HRegionArray RegionsDynThresh = RegionDynThresh.Connection();
		HRegionArray BigRegions = RegionsDynThresh.SelectShape("area", "and", AreaThreshold, MAX_AREA);

		Hobject TabLine;
		if(BigRegions.Num() > 1)
		{
			HTuple Rows, Cols, ATan2;
			HTuple Phis = BigRegions.OrientationRegion();
			HTuple Area = BigRegions.AreaCenter(&Rows, &Cols);
			tuple_atan2((Rows-fCenterY) * (-1), Cols - fCenterX, &ATan2);
			HTuple Angle0 = (Phis - ATan2).Abs();
			for(int i = 0; i<Angle0.Num(); ++i)
			{
				if(Angle0[i] > HTuple(180).Rad())
					Angle0[i] = Angle0[i] - HTuple(180).Rad();
				else if(Angle0[i] > HTuple(90).Rad())
					Angle0[i] = HTuple(180).Rad() - Angle0[i].D();
			}
			HTuple CompareTuple;
			if(ParallelTab)
			{
				HTuple Asin = HTuple(TabLength*1.0/(StartRadius+EndRadius)).Asin();
				CompareTuple = (Angle0 - Asin).Abs();
			}
			else
				 CompareTuple = Angle0;

			HTuple Less, IndLess;
			tuple_less_elem(CompareTuple, HTuple(10), &Less);
			tuple_find(Less, 1, &IndLess);
			if(IndLess != -1)
				select_obj(BigRegions.Id(), &TabLine, IndLess + 1);
			else
				TabLine = BigRegions.Id();
		}
		else if(BigRegions.Num() == 1)
		{
			TabLine = BigRegions.Id();
		}
		else
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}		
		HRegionArray TabLineRegion = TabLine;
		Hobject End;
		if (TabLineRegion.Num() > 1)
		{
			HTuple Indice;
			HTuple Circularity = TabLineRegion.Circularity();
			tuple_find(Circularity, Circularity.Min(), &Indice);
			select_obj(TabLineRegion.Id(), &End, Indice + 1);
		}
		else if(TabLineRegion.Num() == 1)
		{
			End = TabLineRegion.Id();			
		}
		else
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		union1(End, &End);
		HRegion EndRegion(End);
		double RowEnd, ColEnd;
		Hlong Area = EndRegion.AreaCenter(&RowEnd, &ColEnd);
		
		HRegion EdgeShallow = ImageReduced.DynThreshold(AffineImage, DynThreshold - 2, "not_equal");
		HTuple Angle;
		tuple_atan2(fCenterY - RowEnd, ColEnd - fCenterX, &Angle);
		HTuple Asin;
		tuple_asin(TabLength * 1.0 / (StartRadius + EndRadius), &Asin);
		double StartAngle1 = regulateAngleRange(Angle[0].D() + Asin[0].D() * 1.9);
		double EndAngle1 = regulateAngleRange(Angle[0].D() + Asin[0].D() * 2.1);
		HRegion Sector1 = Sector1.GenCircleSector(fCenterY, fCenterX, StartRadius, StartAngle1, EndAngle1);
		HRegion Reg1 = Sector1.Intersection(WorkRegion);
		HRegion Edge1 = Reg1.Intersection(EdgeShallow);

		double StartAngle2 = regulateAngleRange(Angle[0].D() - Asin[0].D() * 2.1);
		double EndAngle2 = regulateAngleRange(Angle[0].D() - Asin[0].D() * 1.9);
		HRegion Sector2 = Sector2.GenCircleSector(fCenterY, fCenterX, StartRadius, StartAngle2, EndAngle2);
		HRegion Reg2 = Sector2.Intersection(WorkRegion);
		HRegion Edge2 = Reg2.Intersection(EdgeShallow);
		if(Edge1.Area() > Edge2.Area())
		{
			m_modelMatchingPARA[3] = regulateAngleRange(Angle[0].D() + Asin[0].D());
		    m_vRgnExTest[id] = Edge1.Id();
			m_vRgn[id] = Reg1.Id();
		//	concat_obj(m_vRgnExTest[id], Reg1.Id(), &m_vRgnExTest[id]);
		}
		else
		{
			m_modelMatchingPARA[3] =  regulateAngleRange(Angle[0].D() - Asin[0].D());
			m_vRgnExTest[id] = Edge2.Id();
			m_vRgn[id] = Reg2.Id();
		//	concat_obj(m_vRgnExTest[id], Reg2.Id(), &m_vRgnExTest[id]);
		}
		concat_obj(m_vRgnExTest[id], End.Id(), &m_vRgnExTest[id]);
        
		m_modelMatchingPARA[0] = m_imageRadius;
		m_modelMatchingPARA[1] = fCenterY;
		m_modelMatchingPARA[2] = fCenterX;
		retValue = HTuple(m_modelMatchingPARA[3]).Deg()[0].D();
		retValue.Detach(pAngle);	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::AngleRegistrationRotateImageAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1414$\
					 1339;R;FP;-2;360;0.01;<=#0$\
					 381;B$\
					 586;LP;5;255;1;12$\
					 735;LP;0;200;1;16$\
					 1395;LP;0;200;1;125$\
					 2015;LP;0;1;1;0"
					); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::TabDeviationDetectionHoughAlgo(VARIANT* rgnId, VARIANT* pGaussThreshold, VARIANT* pHoughThreshold, VARIANT* pDisplayEdge, VARIANT* pAngle)
{
	//根据拉环纹理方向，确定拉环是否偏移
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	float GaussThreshold = (float)pGaussThreshold->fltVal;
	int HoughThreshold = (int)pHoughThreshold->fltVal;
	int DisplayEdge = (int)pDisplayEdge->fltVal;
	CComVariant retValue;
	HRegion WorkRegion  = m_vRgn[id];
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}	
		if(WorkRegion.Area() <= 0)
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		float fCenterX = m_vPos[0].m_x;
		float fCenterY = m_vPos[0].m_y;
		double Radius = m_locationRadius[0].D();
		double theta = m_modelMatchingPARA[3].D();
		//
		HImage Image = m_crtImg;
		HImage ImageReduced = Image.ReduceDomain(WorkRegion);
		HImage Derivate = ImageReduced.DerivateGauss(1, "gradient");
		HRegion Flat = Derivate.Threshold(GaussThreshold, 255);
		HRegion Fill = Flat.FillUp();
		HRegionArray Fills = Fill.Connection();
		HRegionArray BigLines = Fills.SelectShape("area", "and", 15, MAX_AREA);
		if(BigLines.Num() == 0)
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		HRegion BigLine = BigLines.Union1();
		HTuple Angle, Dist;
		Angle = BigLine.HoughLines(4, 50, 3, 3, &Dist);
		HTuple AngleOut, DistOut;
		if(Angle == HTuple())
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		HRegionArray RegionLines = BigLine.SelectMatchingLines(Angle, Dist, 2, HoughThreshold, &AngleOut, &DistOut);
		if (RegionLines.Num() == 0)
		{
			retValue = -1;
			retValue.Detach(pAngle);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;
		}
		double Perpendicular(0);
		HRegion HoughLine;
		for (int i = 0; i < RegionLines.Num(); ++i)
		{
			Hobject Selected;
			double Ori = RegionLines[i].OrientationRegion();
			double Angle0 = regulateAngleRange(Ori - theta);
			if(Angle0 > HTuple(270).Rad()[0].D())
				Angle0 = HTuple(360).Rad()[0].D() - Angle0;
			else if(Angle0 > HTuple(180).Rad()[0].D())
				Angle0 -= HTuple(180).Rad()[0].D();
			else if (Angle0 > HTuple(90).Rad()[0].D())
				Angle0 = HTuple(180).Rad()[0].D() - Angle0;

			if (Angle0 > Perpendicular)
			{
				Perpendicular = Angle0;
				HoughLine = RegionLines[i].Intersection(WorkRegion);
			}
		}
		//Central Line
		HRegion CentLine = CentLine.GenRegionLine(Hlong(fCenterY), Hlong(fCenterX), Hlong(fCenterY - Radius * sin(theta)), Hlong(fCenterX + Radius * cos(theta)));
		if(DisplayEdge)
		{
			m_vRgnExTest[id] = BigLines.Id();
		}
		else
		{
			m_vRgnExTest[id] = HoughLine.Id();
		    concat_obj(m_vRgnExTest[id], CentLine.Id(), &m_vRgnExTest[id]);
		}
		retValue = HTuple(Perpendicular).Deg()[0].D();
		retValue.Detach(pAngle);	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pAngle);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::TabDeviationDetectionHoughAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2016$\
					 1339;R;FP;-2;360;0.01;<=#0$\
					 381;B$\
					 2018;FP;1;255;0.5;22$\
					 2017;LP;0;200;1;16$\
					 2019;LP;0;1;1;1"
					 ); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::DerivateGaussDetectComprehensiveAlgo(VARIANT* rgnId,VARIANT* pSigma, VARIANT* pLowerLimit, VARIANT* pUpperLimit, VARIANT* pDisplayMode, VARIANT* pFeatureNumber, VARIANT* pDefectSize, VARIANT* pDetectArea)
{
	//
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vInterImgs[id].Reset();

	HRegion WorkRegion = m_vRgn[id];
	CComVariant retValue;
	if(WorkRegion.Area() <= 0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDetectArea);
		return S_FALSE;
	}
	float Sigma = (float)pSigma->fltVal;	
	float LowerLimit = (float)pLowerLimit->fltVal;
	float UpperLimit = (float)pUpperLimit->fltVal;
	int DisplayMode = (int)pDisplayMode->fltVal;
	int FeatureNumber = (int)pFeatureNumber->fltVal;
	int DefectSize = (int) pDefectSize->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;	
	HImage Image = m_crtImg;
	if (Sigma < 0.5)
		Sigma = 0.5;

	if (LowerLimit > UpperLimit)
		LowerLimit = UpperLimit;

	if (FeatureNumber > 26 || FeatureNumber < 1)
		FeatureNumber = 1;

	if (DefectSize > MAX_AREA)
		DefectSize = MAX_AREA;

#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		HRegion DisplayDefect = DisplayDefect.GenEmptyRegion();
		HRegion RegionDialated = WorkRegion.DilationCircle(20);
		HImage ImageReduced = Image.ReduceDomain(RegionDialated);
		char *str[] = {"2nd_ddg", "area", "de_saint_venant", "det", "eigenvalue1", "eigenvalue2", "eigenvec_dir", "gauss_curvature", "gradient", "gradient_dir", "kitchen_rosenfeld", "laplace", "main1_curvature", "main2_curvature", "mean_curvature", "none", "x", "xx", "xxx", "xxy", "xy", "xyy", "y", "yy", "yyy", "zuniga_haralick"};
		const char* Component = str[FeatureNumber - 1];
		HImage DeriveGauss = ImageReduced.DerivateGauss(Sigma, Component);
		HImage ReducedGauss = DeriveGauss.ReduceDomain(WorkRegion);
		if (DisplayMode == 1)	
			m_vInterImgs[id] = DeriveGauss.Id();

		HRegion Edge = ReducedGauss.Threshold(LowerLimit, UpperLimit);
		Hlong Area;
		HRegionArray Edges = Edge.Connection();
		HRegionArray Defects = Edges.SelectShape("area", "and", DefectSize, MAX_AREA);
		HTuple AreaDefects, RowDefects, ColDefects;
		AreaDefects = Defects.AreaCenter(&RowDefects, &ColDefects);
		if (AreaDefects == HTuple())
			Area = 0;
		else
		{
			HRegion Defect = Defects.Union1();
			Area = Defect.Area();
			DisplayDefect = Defect.DilationCircle(2.5);
		}
		m_vErrorRgn[id] = DisplayDefect.Id();
		retValue = Area;
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
STDMETHODIMP CEoeAlgo::DerivateGaussDetectComprehensiveAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1401$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 680;FP;0;50.0;0.1;1.0$\
					 191;FP;-255;255;0.1;10$\
					 192;FP;-255;255;0.1;255$\
					 1399;LP;0;1;1;1$\
					 1400;LP;1;26;1;9$\
					 434;LP;1;5000;1;100");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::DetectRegionExcludingArbitraryRegionAlgo(VARIANT* rgnId, VARIANT* pChangingSize, VARIANT* pDoRegistration, BSTR* bstrOcrRgnFile)
{
	//从中心定位区域中抠除任意形状区域
	int id = (int)rgnId->fltVal;
	int ChangingSize = (int)pChangingSize->fltVal;
	int DoRegistration = (int)pDoRegistration->fltVal;
	HRegion BaseRegion = m_centerLocationRegion;
	CString sRgnPath1  = *bstrOcrRgnFile;
	string strRgnPath1 = CT2A(sRgnPath1);
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
#ifdef NDEBUG
	try
	{
#endif
		m_vRgnExTest[id].Reset();
		m_vRgnEx[id].Reset();
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
		}
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
			m_vRgn[id] = BaseRegion.Id() ;
			return S_FALSE;
		}

		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}

		if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
		{
			m_vRgn[id] = BaseRegion.Id() ;
			return S_FALSE;
		}

		HRegion TargetRegion = m_dictCrtPathRgn[strRgnPath1];
		HRegion TargetRegionMoved;
		//	HRegion TargetRegionMoved = TargetRegion.MoveRegion(Hlong(fCenterY), Hlong(fCenterX));
		if(DoRegistration)
		{
			HTuple HomMat2;
			vector_angle_to_rigid(0, 0, 0, m_modelMatchingPARA[1], m_modelMatchingPARA[2], m_modelMatchingPARA[3], &HomMat2);
			TargetRegionMoved = TargetRegion.AffineTransRegion(HomMat2, "nearest_neighbor");
		}
		else
		{
			TargetRegionMoved = TargetRegion.MoveRegion(Hlong(fCenterY), Hlong(fCenterX));

		}
		HRegion BaseRegionChanged;
		if(ChangingSize >= 1)
			BaseRegionChanged = BaseRegion.DilationCircle(ChangingSize + 0.5);
		else if(ChangingSize <= -1)
			BaseRegionChanged = BaseRegion.ErosionCircle(-ChangingSize + 0.5);
		else
			BaseRegionChanged = BaseRegion;

		HRegion DetectRegion = BaseRegionChanged.Difference(TargetRegionMoved);
		m_vRgn[id] = DetectRegion.Id() ;	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = BaseRegion.Id();
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::DetectRegionExcludingArbitraryRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1402$\
					 167;B$\
					 1403;LP;-100;100;1;2$\
					 1412;LP;0;1;1;1$\
					 232;SPR" );
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::DiscontinuousLineDetectionAlgo(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pRequiredLineLength, VARIANT* pBrokenLineNumber)
{
	//防爆纹断裂检测算子
	int id = (int)rgnId->fltVal;
	float Threshold = (float)pThreshold->fltVal;
	int RequiredLineLength = (int)pRequiredLineLength->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	HImage Image = m_crtImg;
	HRegion WorkRegion = m_vRgn[id];
	CComVariant retValue;
	if(WorkRegion.Area() <= 0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pBrokenLineNumber);
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
		HRegionArray WorkRegions = WorkRegion.Connection();
		HTuple Number;
		count_obj(WorkRegions.Id(), &Number);
		double ShortestLength(0);
		Hobject ShortestLine;
		gen_empty_obj(&ShortestLine);
		int BrokenLineNumber(0);
		//int BrokenLineNumber(0);
		for(int i = 1; i <= Number; ++i)
		{
			Hobject ObjectSelected;
			select_obj(WorkRegions.Id(), &ObjectSelected, i);
			HRegion RegionSelected = ObjectSelected;
			if(RegionSelected.Area() > 10)
			{
				HImage ImageReduced = Image.ReduceDomain(RegionSelected);
				HImage DerivedGauss = ImageReduced.DerivateGauss(0.7, "eigenvalue1");
				HRegion Edge = DerivedGauss.Threshold(Threshold, 255);
				HRegion FillUp = Edge.FillUp();
				HRegionArray FillUps = FillUp.Connection();
				HRegionArray ClosedRegion = FillUps.ClosingCircle(14.5);
				HRegionArray Skeletons = ClosedRegion.Skeleton();
				HTuple RowUnused, ColUnused;
				HTuple Length = Skeletons.AreaCenter(&RowUnused, &ColUnused);

				//	HRegion Skeleton = Skeletons.Union1();
				//	HXLDContArray Contours = Skeleton.GenContoursSkeletonXld(2, "filter");
				//	HXLDContArray UniLines = Contours.UnionAdjacentContoursXld(3, 3, "attr_forget");
				//	HTuple Length = UniLines.LengthXld();
				Hobject LongestObj;
				select_obj(Skeletons.Id(), &LongestObj, Length.Find(Length.Max()) + 1);
				/*	if(Length.Max() < ShortestLength)
				ShortestLength = Length.Max();*/
				if(Length.Max() < RequiredLineLength)
				{
					concat_obj(m_vErrorRgn[id], LongestObj, &m_vErrorRgn[id]);
					BrokenLineNumber++;
				}
			}	
		}

		if(0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[id] = newRgn;
		}
		retValue = BrokenLineNumber;	
		retValue.Detach(pBrokenLineNumber);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pBrokenLineNumber);
		return S_FALSE;
	}
#endif
	return S_OK;



}
STDMETHODIMP CEoeAlgo::DiscontinuousLineDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2001$\
					 2004;R;LP;-2;50;1;<=#0$\
					 381;B$\
					 2002;FP;0;255;0.1;6.2$\
					 2003;LP;2;200;1;70"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


/*********************************
*               
*          方盖算子    
*
*********************************/
//定位算子

STDMETHODIMP CEoeAlgo::CenterRectangleLocationAlgoSobel(VARIANT* rgnId, VARIANT* pSobelSize, VARIANT* pDisplayMode, VARIANT* pThresholdLow, VARIANT* pOutlierRemoverSize)
{
	//Sobel预处理提取盖面，定位中心
	int id = (int)rgnId->fltVal;
	int SobelSize = (int)pSobelSize->fltVal;
	int DisplayMode = (int)pDisplayMode->fltVal;
	int ThresholdLow = (int)pThresholdLow->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	if (ThresholdLow > 255)
		ThresholdLow = 255;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	Hobject newRgn;
	m_vRgn[id] = newRgn;
	Hlong Width, Height;
	HTuple Row1, Column1, Phi1, Length1, Length2, Area, Row, Column;
	Hobject EdgeAmplitude, ImageThresh,RegionFillUp,RegionOpening,RegionClosing;
	Hobject  Ellipse, ImageReduced;
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
		sobel_amp(ImageReduced, &EdgeAmplitude, "sum_abs", SobelSize);
		if (DisplayMode == 1)
			m_vInterImgs[id] = EdgeAmplitude;
		else
			m_vInterImgs[id] = Image;

		threshold(EdgeAmplitude, &ImageThresh,ThresholdLow, 255);
		fill_up(ImageThresh, &RegionFillUp);
		opening_circle(RegionFillUp, &RegionOpening, OutlierRemoverSize + 0.5);
		closing_circle(RegionOpening, &RegionClosing, OutlierRemoverSize * 2);
		area_center(RegionClosing, &Area, &Row, &Column);
		if (Area > 30000 ) 
		{	
			smallest_rectangle2(RegionClosing, &Row1, &Column1, &Phi1, &Length1, &Length2); 
			m_centerLocationRegion = RegionClosing;
			m_vPos[0].m_y = (float)Row1[0].D();
			m_vPos[0].m_x = (float)Column1[0].D();
			m_vRgn[id] = RegionClosing;
		}
		else
		{		
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[id] = Ellipse;
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
STDMETHODIMP CEoeAlgo::CenterRectangleLocationAlgoSobelHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("639$\
					 381;B$\
					 1346;LP;3;13;2;5$\
					 1399;LP;0;1;1;1$\
					 640;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::CenterRectangleLocationAlgoInnerEdge(VARIANT* rgnId, VARIANT* pLidWidth, VARIANT* pLidHeight, VARIANT* pThreshold, VARIANT* pSecondThreshold, VARIANT* pOutlierRemoverSize)
{
	//Sobel预处理提取盖面，定位中心
	int id = (int)rgnId->fltVal;
	int LidWidth = (int)pLidWidth->fltVal;
	int LidHeight = (int)pLidHeight->fltVal;
	int Threshold = (int)pThreshold->fltVal;
	int SecondThreshold = (int)pSecondThreshold->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	if (Threshold > 255)
		Threshold = 255;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id] = Hobject();
	m_vRgn[id] = Hobject();

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		HImage Image = m_crtImg;
		Hlong Width, Height;
		get_image_size (Image.Id(), &Width, &Height);
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
		/*HRegion Ellipse = Ellipse.GenEllipse(Height/2.0, Width/2.0, 0.0, Width/1.8, Height/1.6);
		HRegion Domain = Image.GetDomain();
		HRegion EllipseReduced = Domain.Intersection(Domain);

		HImage ImageReduced = Image.ReduceDomain(EllipseReduced);*/
		//		HImage MeanImage = ImageReduced.MeanImage(3, 3);
		HRegion Region = Image.Threshold(Threshold, 255);
		//	HRegion RegionFillUp = Region.FillUp();
		HRegionArray Regions = Region.Connection();
		HTuple Rows, Columns, Phis, Length1, Length2;
		Rows = Regions.SmallestRectangle2(&Columns, &Phis, &Length1, &Length2);
		HTuple SizeMatch = ((Length1 - LidWidth).Abs())+((Length2 - LidHeight).Abs());
		HTuple MinIndex;
		tuple_find(SizeMatch, SizeMatch.Min(), &MinIndex);
		//	Hlong TupleLength;
		//	tuple_length(MinIndex, &TupleLength);
		if (MinIndex == HTuple(0))
		{
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			HRegion Circle = Circle.GenCircle(Height * 0.5, Width * 0.5, 5);
			m_centerLocationRegion = Circle.Id();
			m_vRgn[id] = Circle.Id();
			return S_FALSE;
		}
		Hobject SelectedObj;
		select_obj(Regions.Id(), &SelectedObj, MinIndex.Select(0) + 1);
		HRegion SelectedRegion = SelectedObj;
		HRegion SelectedRegionClose = SelectedRegion.ClosingCircle(150);
		HRegion RegionFillUp = SelectedRegionClose.FillUp();
		//HRegion SelectedRegion = Regions.SelectShapeStd("max_area", 70);
		/*HTuple Value = Regions.RegionFeatures("width");
		HTuple Index, Length;
		tuple_find(Value, Value.Max(), &Index);
		tuple_length(Index, &Length);
		if (Length != 1)
		{
		m_vPos[0].m_x = 0.0;
		m_vPos[0].m_y = 0.0;
		m_vRgn[id] = Ellipse.Id();
		m_centerLocationRegion=m_vRgn[id];
		return S_FALSE;
		}*/
		//	Hobject SelectedObj;
		//	select_obj(Regions.Id(), &SelectedObj, (Hlong)Index[0].D() + 1);
		//
		//HRegion OpenRegion = SelectedRegion.OpeningCircle(OutlierRemoverSize + 0.5);
		//HRegion CloseRegion = OpenRegion.ClosingCircle(OutlierRemoverSize * 2 + 0.5);
		HRegion ErosedRegion = RegionFillUp.ErosionCircle(10.5);
		HRegion DifferRegion = RegionFillUp.Difference(ErosedRegion);
		HImage SlimImage = Image.ReduceDomain(DifferRegion);
		HRegion SlimRegion = SlimImage.Threshold(0, SecondThreshold);
		HRegion WholeRegion = SlimRegion.Union2(ErosedRegion);
		HRegionArray WholeRegions = WholeRegion.Connection();
		HRegionArray TargetRegion = WholeRegions.SelectShapeStd("max_area", 70);
		HRegion RegionOpening2 = TargetRegion.OpeningCircle(OutlierRemoverSize + 0.5);
		HRegion RegionClosing2 = RegionOpening2.ClosingCircle(OutlierRemoverSize * 2 + 0.5);

		Hlong Area = RegionClosing2.Area();
		if (Area > 30 ) 
		{	
			HTuple Row1, Column1, Phi1, Length1, Length2;
			smallest_rectangle2(RegionClosing2.Id(), &Row1, &Column1, &Phi1, &Length1, &Length2); 
			m_centerLocationRegion = RegionClosing2.Id();
			m_vPos[0].m_y = (float)Row1[0].D();
			m_vPos[0].m_x = (float)Column1[0].D();
			m_vRgn[id] = RegionClosing2.Id();
		}
		else
		{		
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			HRegion Circle = Circle.GenCircle(Height * 0.5, Width * 0.5, 5);
			m_centerLocationRegion = Circle.Id();
			m_vRgn[id] = Circle.Id();
			return S_FALSE;
		}	
		m_vInterImgs[id] = Image.Id();
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = Hobject();
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::CenterRectangleLocationAlgoInnerEdgeHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1406$\
					 381;B$\
					 1410;LP;100;1280;1;505$\
					 1411;LP;50;1024;1;282$\
					 640;LP;0;255;1;50$\
					 1407;LP;0;255;1;10$\
					 641;LP;1;200;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::CenterRectangleSecondLocationAlgo(VARIANT* rgnId, VARIANT* pDilationSize, VARIANT* pThreshold, VARIANT* pOutlierRemoverSize)
{
	//Sobel预处理提取盖面，定位中心
	int id = (int)rgnId->fltVal;
	int DilationSize = (int)pDilationSize->fltVal;
	//	int SobelSize = (int)pSobelSize->fltVal;
	//	int DisplayMode = (int)pDisplayMode->fltVal;
	int Threshold = (int)pThreshold->fltVal;
	int OutlierRemoverSize = (int)pOutlierRemoverSize->fltVal;
	if (Threshold > 255)
		Threshold = 255;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	HRegion WorkRegion = m_centerLocationRegion;
	if(WorkRegion.Area() <= 0)
	{
		m_vPos[0].m_x = 0.0;
		m_vPos[0].m_y = 0.0;
		m_vRgn[id] = WorkRegion.Id();
		m_centerLocationRegion = m_vRgn[id];
		return S_FALSE;
	}

#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		HImage Image = m_crtImg;
		m_vInterImgs[id] = Hobject();
		Hlong Width, Height;
		get_image_size (Image.Id(), &Width, &Height);
		m_imgWidth  = (long)Width;
		m_imgHeight = (long)Height;
		HRegion DilationRegion = WorkRegion.DilationCircle(DilationSize - 11.5);
		HRegion DilationRegionOut = WorkRegion.DilationCircle(DilationSize);
		HRegion Differ = DilationRegionOut.Difference(DilationRegion);
		HImage ImageReduced = Image.ReduceDomain(Differ);
		//		HImage Amp = ImageReduced.SobelAmp("sum_abs", SobelSize);
		//		HRegion Region = Amp.Threshold(0, Threshold);
		HRegion Region = ImageReduced.Threshold(0, Threshold);
		HRegion FlatRegion = Region.Union2(DilationRegion);
		HRegionArray Regions = FlatRegion.Connection();
		HRegion TargetRegion = Regions.SelectShapeStd("max_area", 70);
		HRegion RegionOpen = TargetRegion.OpeningCircle(OutlierRemoverSize + 0.5);
		HRegion RegionClose = RegionOpen.ClosingCircle(OutlierRemoverSize * 2 + 0.5);
		Hlong Area = RegionClose.Area();
		HTuple Row1, Column1, Phi1, Length1, Length2;
		if (Area > 30 ) 
		{	
			smallest_rectangle2(RegionClose.Id(), &Row1, &Column1, &Phi1, &Length1, &Length2); 
			m_centerLocationRegion = RegionClose.Id();
			m_vPos[0].m_y = (float)Row1[0].D();
			m_vPos[0].m_x = (float)Column1[0].D();
			m_vRgn[id] = RegionClose.Id();
		}
		else
		{		
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_centerLocationRegion = m_vRgn[id];
			return S_FALSE;
		}	
		//m_vInterImgs[id] = Image.Id();
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = Hobject();
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::CenterRectangleSecondLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1409$\
					 381;B$\
					 1408;LP;2;200;1;40$\
					 192;LP;1;255;1;50$\
					 641;LP;1;800;1;70");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//区域算子

STDMETHODIMP CEoeAlgo::PhaseCorrelationRectangularRegionAlgo(VARIANT* rgnId, VARIANT* pWidthPlaceSet, VARIANT* pHeightPlaceSet, VARIANT* pRegionHeight, VARIANT* pRegionWidth)
{

	//得到矩形检测区域
	int id = (int)rgnId->fltVal;
	int WidthPlaceSet =(int)pWidthPlaceSet->fltVal;
	int HeightSet =(int)pRegionHeight->fltVal;
	int WidthSet =(int)pRegionWidth->fltVal;
	int HeightPlaceSet = (int)pHeightPlaceSet->fltVal;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	float fCenterX, fCenterY;
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
			m_dictSpecialData[id].clear();

		Hobject Image = m_crtImg;
		Hobject BaseRect, RectLeft, ImageDomain, InterImg;	
		HTuple Row1, Col1, Phi, Length1, Length2, HomMat2D;
		smallest_rectangle2(m_centerLocationRegion, &Row1, &Col1, &Phi, &Length1, &Length2);
		m_RectangleAngle=Phi[0];
		gen_rectangle2(&RectLeft, 0.5 * m_imgHeight + HeightPlaceSet, 0.5 * m_imgWidth - WidthPlaceSet, 0, WidthSet, HeightSet);
		gen_rectangle2(&BaseRect, 0.5 * m_imgHeight + HeightPlaceSet, 0.5 * m_imgWidth + WidthPlaceSet, 0, WidthSet, HeightSet);
		concat_obj(RectLeft, BaseRect, &BaseRect);
		vector_angle_to_rigid(fCenterY, fCenterX, Phi, 0.5 * m_imgHeight, 0.5 * m_imgWidth, 0, &HomMat2D);
		affine_trans_image(Image, &InterImg, HomMat2D, "bilinear", "false");
		affine_trans_region(m_centerLocationRegion, &m_centerLocationRegion, HomMat2D, "nearest_neighbor");
		Hobject StructElement;
		gen_rectangle1(&StructElement, 0, 0, 100, 300);
		opening(m_centerLocationRegion, StructElement, &m_centerLocationRegion);
		opening_circle(m_centerLocationRegion, &m_centerLocationRegion, 70);
		m_ImageAffinTrans = InterImg;		
		Hobject DetectRegion;
		get_domain(InterImg,&ImageDomain);
		intersection(ImageDomain, BaseRect, &DetectRegion);
		union1(DetectRegion, &DetectRegion);
		m_vRgn[id] = DetectRegion;		

		//修改中心点坐标
		m_vPos[0].m_y = (float)(m_imgHeight/2.0);
		m_vPos[0].m_x = (float)(m_imgWidth/2.0);

		m_crtImgMult = InterImg;
		m_vInterImgs[id] = InterImg;
		//区分通道
		HTuple  Channels;
		count_channels(m_crtImgMult, &Channels);
		//黑白图片
		if(Channels[0].I() == 3)
			rgb1_to_gray(m_crtImgMult, &m_crtImg);
		else if(Channels[0].I() == 1)
			m_crtImg  = m_crtImgMult;
		else
			mean_n(m_crtImgMult, &m_crtImg);

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
STDMETHODIMP CEoeAlgo::PhaseCorrelationRectangularRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2009$\
					 167;B$\
					 825;LP;0;1000;1;300$\
					 826;LP;-100;100;1;0$\
					 667;LP;1;50;1;50$\
					 668;LP;5;400;1;380"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::RectangleRoundRegionAlgo(VARIANT* rgnId, VARIANT* pChangingSize, VARIANT* pRoiWidth, VARIANT* pChamferRadius)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	int RoiWidth = (int)pRoiWidth->fltVal;
	int ChangingSize =(int)pChangingSize->fltVal;
	int ChamferRadius = (int)pChamferRadius->fltVal;
	float LocationRadius=(float)(m_locationRadius[0].I());
	m_ringPara[id].m_Radius = LocationRadius + ChangingSize;
	m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = Hobject() ;
		return S_FALSE;
	}
	if (RoiWidth < 1)
		RoiWidth = 1;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//Hobject RegionFill;
		HRegion WorkRegion = m_centerLocationRegion;
		HRegion FillUp = WorkRegion.FillUp();
		HRegion OuterRegion;
		if(ChangingSize >= 1)
			OuterRegion = FillUp.DilationCircle(ChangingSize + 0.5);
		else if(ChangingSize <= -1)
			OuterRegion = FillUp.ErosionCircle(-ChangingSize + 0.5);
		else		
			OuterRegion = FillUp;

		HRegion InnerRegion = OuterRegion.ErosionCircle(RoiWidth + 0.5);
		HRegion Ring = OuterRegion.Difference(InnerRegion);
		Hlong Row1, Column1, Row2, Column2;
		Row1 = OuterRegion.SmallestRectangle1(&Column1, &Row2, &Column2);
		HRegionArray Sub = Sub.GenRectangle1(HTuple(Row1 + ChamferRadius).Append(Row1), HTuple(Column1).Append(Column1 + ChamferRadius), HTuple(Row2 - ChamferRadius).Append(Row2), HTuple(Column2).Append(Column2 - ChamferRadius));
		HRegion DetectRegion = Ring.Difference(Sub);
		m_vRgn[id] = DetectRegion.Id();	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = Hobject();
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::RectangleRoundRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2007$\
					 167;B$\
					 693;LP;-1000;1000;1;10$\
					 413;LP;1;800;1;100$\
					 2006;LP;2;500;1;150"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::RectangleEdgeRegionAlgo(VARIANT* rgnId, VARIANT* pChangingSize, VARIANT* pRoiWidth, VARIANT* pChamferRadius)
{
	//得到环形区域
	int id = (int)rgnId->fltVal;
	int RoiWidth = (int)pRoiWidth->fltVal;
	int ChangingSize =(int)pChangingSize->fltVal;
	int ChamferRadius = (int)pChamferRadius->fltVal;
	float LocationRadius=(float)(m_locationRadius[0].I());
	m_ringPara[id].m_Radius = LocationRadius + ChangingSize;
	m_ringPara[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
	if((fCenterX<2.0)||(fCenterY<2.0))
	{
		m_vRgn[id] = Hobject() ;
		return S_FALSE;
	}
	if (RoiWidth < 1)
		RoiWidth = 1;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		//Hobject RegionFill;
		HRegion WorkRegion = m_centerLocationRegion;
		HRegion FillUp = WorkRegion.FillUp();
		HRegion OuterRegion;
		if(ChangingSize >= 1)
			OuterRegion = FillUp.DilationCircle(ChangingSize + 0.5);
		else if(ChangingSize <= -1)
			OuterRegion = FillUp.ErosionCircle(-ChangingSize + 0.5);
		else		
			OuterRegion = FillUp;
		HRegion InnerRegion = OuterRegion.ErosionCircle(RoiWidth + 0.5);
		HRegion Ring = OuterRegion.Difference(InnerRegion);
		Hlong Row1, Column1, Row2, Column2;
		Row1 = OuterRegion.SmallestRectangle1(&Column1, &Row2, &Column2);
		HRegionArray Sub = Sub.GenRectangle1(HTuple(Row1 + ChamferRadius).Append(Row1), HTuple(Column1).Append(Column1 + ChamferRadius), HTuple(Row2 - ChamferRadius).Append(Row2), HTuple(Column2).Append(Column2 - ChamferRadius));
		HRegion DetectRegion = Ring.Intersection(Sub);
		m_vRgn[id] = DetectRegion.Id();	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = Hobject();
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::RectangleEdgeRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2008$\
					 167;B$\
					 693;LP;-1000;1000;1;10$\
					 413;LP;1;800;1;100$\
					 2006;LP;2;500;1;150"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

//检测算子

STDMETHODIMP CEoeAlgo::PhaseCorrelationRectangularAlgo(VARIANT* rgnId, VARIANT* pMode, VARIANT* pScore, VARIANT* pCalibrationImageID)
{
	//相位相关法校正方盖拉环方向
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id] = Hobject();
	Hobject rgn  = m_vRgn[id];
	int Mode = (int)pMode->fltVal;
	float Score = (float)pScore->fltVal;
	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
		{
			return S_OK;
		}		
		string ModelImgFile = m_strTaskPath;
		HRegion rgn_copy = rgn;
		if(rgn_copy.Area() <= 0) 
		{
			retValue = -1;
			retValue.Detach(pCalibrationImageID);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;			
		}
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		Hobject Image = m_crtImg;	
		Hobject ImageReduced, ImageCropped;
		float fCenterX, fCenterY;
		fCenterX = m_vPos[0].m_x;
		fCenterY = m_vPos[0].m_y;
		HTuple Length;
		Hobject SelectedObj, ImageRotated, Patches, TiledImage;
		connection(rgn, &rgn);
		count_obj(rgn, &Length);
		gen_empty_obj(&Patches);
		if(Length != 2)
		{
			retValue = -1;
			retValue.Detach(pCalibrationImageID);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;			
		}
		for(int i = 0; i < 2; i++)
		{
			select_obj(rgn, &SelectedObj, i + 1);
			reduce_domain(Image, SelectedObj, &ImageReduced);
			crop_domain(ImageReduced, &ImageCropped);
			rotate_image(ImageCropped, &ImageRotated, 270 - 180 * i, "nearest_neighbor");
			concat_obj(Patches, ImageRotated, &Patches);
		}
		tile_images(Patches, &TiledImage, 2, "horizontal");  
		//Generate the channel directory if not exist
		HTuple ChannelFolder,TaskFolder;
		ChannelFolder=HTuple(m_strChannelPath.c_str());
		if (_access(ChannelFolder[0].S(), 0))
		{
			CString  FolderDir(ChannelFolder[0].S());
			CreateDirectory(FolderDir, NULL);
		}
		//Generate the task directory if not exist
		if(_access(ModelImgFile.c_str(),0))
		{
			CreateDirectoryA(ModelImgFile.c_str(), NULL);
		}
		ModelImgFile.append("\\");
		ModelImgFile.append("ModelImagePC");
		ModelImgFile.append(".tiff");
		if(Mode == 0)
		{         
			//创建模板图片      	
			write_image(TiledImage, "tiff", 0, ModelImgFile.c_str());	
			m_OldCenterY = fCenterY;
			m_OldCenterX = fCenterX;
			retValue = 0;
		}
		else if(Mode == 1)
		{
			if(_access(ModelImgFile.c_str(),0))
			{
				retValue = -1;
				retValue.Detach(pCalibrationImageID);
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
				return S_FALSE;
			}
			//相位相关计算
			Hobject ModelImage;
			read_image(&ModelImage, ModelImgFile.c_str());
			HTuple Maximum, Area, RowMax, ColMax, Shifting;
			HTuple Width, Height, ModelWidth, ModelHeight,HomMat2DFinal;
			Hobject ImageFFT, ImagePhaseCorrelationFFT, ImageFFTModel, ImagePhaseCorrelation;
			get_image_size(TiledImage, &Width, &Height);
			get_image_size(ModelImage,&ModelWidth,&ModelHeight);
			if(Width[0].D() != ModelWidth[0].D() || Height[0].D() != ModelHeight[0].D())
			{ 
				retValue = -1;
				retValue.Detach(pCalibrationImageID);
				m_vErrorRgn[id].Reset();
				m_vWhiteErrorRgn[id].Reset();
				return S_FALSE;
			}
			rft_generic(TiledImage, &ImageFFT, "to_freq", "none", "complex", Width);
			rft_generic(ModelImage, &ImageFFTModel, "to_freq", "none", "complex", Width);
			phase_correlation_fft(ImageFFTModel, ImageFFT, &ImagePhaseCorrelationFFT);
			rft_generic(ImagePhaseCorrelationFFT, &ImagePhaseCorrelation, "from_freq", "n", "real", Width);
			Hobject Region, Maxi;
			gen_rectangle1(&Region, 0, 0, 0, Width);
			gray_features(Region, ImagePhaseCorrelation, "max", &Maximum);
			if(Maximum.Num() == 1)
			{
				threshold(ImagePhaseCorrelation, &Maxi, Maximum, Maximum);
				area_center(Maxi, &Area, &RowMax, &ColMax);
				Shifting = ColMax / Width;
			}
			else
			{
				retValue = -1;
				retValue.Detach(pCalibrationImageID);
				m_vErrorRgn[id] = rgn;
				m_vWhiteErrorRgn[id] = rgn;
				return S_FALSE;
			}			
			if((Shifting - 0.5).Abs() < Score)
			{
				rotate_image(Image, &m_ImageAffinTrans, 180, "constant");
				vector_angle_to_rigid(0.5 * m_imgHeight, 0.5 * m_imgWidth, 0, 0.5 * m_imgHeight, 0.5 * m_imgWidth, HTuple(180).Rad(), &HomMat2DFinal);
				affine_trans_region(m_centerLocationRegion, &m_centerLocationRegion, HomMat2DFinal, "constant");
			}			
		}

		/*Hobject StructElement;
		gen_rectangle1(&StructElement, 0, 0, 100, 300);
		opening(m_centerLocationRegion, StructElement, &m_centerLocationRegion);
		opening_circle(m_centerLocationRegion, &m_centerLocationRegion, 70);*/
		//匹配参数，画图软件需要。匹配算子计算后会被刷新。
		m_modelMatchingPARA[0] = m_imageRadius;
		m_modelMatchingPARA[1] = m_imgHeight/2.0;
		m_modelMatchingPARA[2] = m_imgWidth/2.0;
		m_modelMatchingPARA[3] = 0.0;
		//
		m_angleMatchingPARA[1] = m_imgHeight/2.0;
		m_angleMatchingPARA[2] = m_imgWidth/2.0;
		m_angleMatchingPARA[3] = 0.0;
		m_angleDetect = m_modelMatchingPARA[3].D();

		//内存地址作为返回数值
		retValue = (long)&m_ImageAffinTrans;	
		//retValue = 0;
		//
		m_crtImgMult = m_ImageAffinTrans;
		m_vInterImgs[id] = m_ImageAffinTrans;
		//区分通道
		HTuple  Channels;
		count_channels(m_crtImgMult, &Channels);
		//黑白图片
		if(Channels[0].I() == 3)
		{
			rgb1_to_gray(m_crtImgMult, &m_crtImg);
		}
		else if(Channels[0].I() == 1)
		{
			m_crtImg  = m_crtImgMult;
		}
		else
		{
			mean_n(m_crtImgMult, &m_crtImg);
		}
		retValue.Detach(pCalibrationImageID);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		retValue.Detach(pCalibrationImageID);
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();	
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::PhaseCorrelationRectangularAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1342$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 1340;LP;0;1;1;1$\
					 1362;FP;0;1;0.01;0.1"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::DetectRegionRectangleLocationAlgo(VARIANT* rgnId,VARIANT* pChangingSize, VARIANT* pDoMatch, BSTR* bstrExcludedRgnFile)
{
	//BSTR* bstrRgnFile
	int id = (int)rgnId->fltVal;
	int ChangingSize = (int)pChangingSize->fltVal;
	int DoMatch = (int)pDoMatch->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_bOcrRegionFlag = true;
	if(m_bDebugImg)
	{
		m_dictSpecialData[id].clear();
	}
	HRegion BaseRegion = m_centerLocationRegion;
	m_vRgn[id] = BaseRegion.Id();
	/*if(RingOrOCR < 0 || RingOrOCR > 1)
	{
	m_vRgn[id] = BaseRegion.Id();
	return S_FALSE;
	}*/
	//,strRgnPath2;
	CString sRgnPath1  = *bstrExcludedRgnFile;
	string strRgnPath1 = CT2A(sRgnPath1);

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
				m_vRgn[id] = BaseRegion.Id();
				return S_FALSE;
			}

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
				m_vRgn[id] = BaseRegion.Id();
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
				m_vRgn[id] = BaseRegion.Id();
				return S_FALSE;
			}

			m_modelCenterRow  = m_modelPARA[1];
			m_modelCenterCol  = m_modelPARA[2];
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
			m_vRgn[id] = BaseRegion.Id();
			return S_FALSE;
		}
		//
		if(	m_modelMatchingPARA[0].D()<1.0)
		{
			m_vRgn[id] = BaseRegion.Id();
			return S_FALSE;
		}

		//base region
		HRegion BaseRegionChanged, BaseRegionTrans;
		//HRegion BaseRegionTrans;
		if(ChangingSize >= 1)
			BaseRegionChanged = BaseRegion.DilationCircle(ChangingSize + 0.5);
		else if(ChangingSize <= -1)
			BaseRegionChanged = BaseRegion.ErosionCircle(-ChangingSize + 0.5);
		else
			BaseRegionChanged = BaseRegion;

		HRegion ArbitraryRegion = m_dictCrtPathRgn[strRgnPath1];
		HRegion ArbitraryRegionTrans;
		if(DoMatch == 0)
		{
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = BaseRegion.Id();
				return S_FALSE;
			}

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
					m_vRgn[id] = BaseRegion.Id();
					return S_FALSE;
				}
			}
			////
			//			m_modelPARA=modelPARAID;
			//	BaseRegionTrans = BaseRegionChanged.MoveRegion(Hlong(fCenterY), Hlong(fCenterX));
			ArbitraryRegionTrans = ArbitraryRegion.MoveRegion(Hlong(fCenterY), Hlong(fCenterX));
			BaseRegionTrans = BaseRegionChanged;
		}
		else if (DoMatch == 1)
		{
			if(m_dictCrtPathRgn.find(strRgnPath1) == m_dictCrtPathRgn.end())
			{
				m_vRgn[id] = BaseRegion.Id();
				return S_FALSE;
			}

			HTuple HomMat2DBase;
			vector_angle_to_rigid(fCenterY, fCenterX, 0, m_modelMatchingPARA[4], m_modelMatchingPARA[5], m_modelMatchingPARA[6], &HomMat2DBase);
			BaseRegionTrans = BaseRegionChanged.AffineTransRegion(HomMat2DBase, "nearest_neighbor");

			//arbitrary region
			HTuple HomMat2DArbitrary;
			vector_angle_to_rigid(0, 0, 0, m_modelMatchingPARA[1], m_modelMatchingPARA[2], m_modelMatchingPARA[3], &HomMat2DArbitrary);
			HRegion ArbitraryRegionTrans = ArbitraryRegion.AffineTransRegion(HomMat2DBase, "nearest_neighbor");

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
					m_vRgn[id] = BaseRegion.Id();
					return S_FALSE;
				}
				//
			}
			//	HTuple modelPARAExtract=m_vRgnReadData[id];

			m_modelPARA[0] = 0;
			m_modelPARA[1] = 0;
			m_modelPARA[2] = 0;
			m_modelPARA[3] = 0;
			m_modelPARA[4] = 0;
			m_modelPARA[5] = 0;
			m_modelPARA[6] = 0;
			//
			//Hobject  RegionAffineTrans,OCRRegionAffineTrans,RegionDiff;

			//HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted;
			//vector_angle_to_rigid(0, 0, 0, 0, 0, m_modelMatchingPARA[6].D(), &HomMat2DRate);
			//hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			//hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

			//Hobject rotatedRgn,RegionMoved1;	
			//HTuple RowModelData,ColModelData;
			//RowModelData=m_modelPARA[1];
			//ColModelData=m_modelPARA[2];
			//move_region (roiRgn, &RegionMoved1,-RowModelData, -ColModelData);        
			//projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			//RowModelData=m_modelMatchingPARA[4];
			//ColModelData=m_modelMatchingPARA[5];
			//move_region (rotatedRgn,&RegionAffineTrans,RowModelData,ColModelData) ;

			////
			////
			//HTuple imageHomMat2D;
			//vector_angle_to_rigid(m_modelCenterRow,m_modelCenterCol,0.0,m_modelMatchingPARA[4].D(),m_modelMatchingPARA[5].D(),m_modelMatchingPARA[6].D(),&imageHomMat2D);
			//affine_trans_image (m_modelImageOCR,&m_modelImageTransOCR, imageHomMat2D,"bilinear","false");
			//
			//	CString sRgnPath2  = *bstrExtractRgnFile;
			//	strRgnPath2 = CT2A(sRgnPath2);
			/*	if(m_dictCrtPathRgn.find(strRgnPath2) == m_dictCrtPathRgn.end())
			{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
			}*/
			//Hobject extractRgn = m_dictCrtPathRgn[strRgnPath2];
			////affine_trans_region(extractRgn,&OCRRegionAffineTrans,ringHomMat2D,"false");
			////
			//vector_angle_to_rigid(0, 0, m_modelPARA[6].D(), 0, 0, m_modelMatchingPARA[3].D(), &HomMat2DRate);
			//hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			//hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
			//RowModelData=m_modelPARA[4];
			//ColModelData=m_modelPARA[5];
			//move_region (extractRgn, &RegionMoved1,-RowModelData, -ColModelData);        
			//projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			//RowModelData=m_modelMatchingPARA[1];
			//ColModelData=m_modelMatchingPARA[2];
			//move_region (rotatedRgn,&OCRRegionAffineTrans,RowModelData,ColModelData) ;
			////
			//difference (RegionAffineTrans, OCRRegionAffineTrans, &RegionDiff);
			//RegionAffineTrans=RegionDiff;
			/*if(ChangingSize>=0.5)
			{
			erosion_circle (RegionAffineTrans, &RegionDetect,ChangingSize);
			}
			else if(ChangingSize<-0.5)
			{
			dilation_circle(RegionAffineTrans, &RegionDetect,abs(ChangingSize));
			}
			else
			{
			RegionDetect=RegionAffineTrans;
			}*/
		}
		HRegion DetectRegion = BaseRegionTrans.Difference(ArbitraryRegionTrans);

		m_vRgn[id] = DetectRegion.Id();	
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		m_vRgn[id] = BaseRegion.Id();
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::DetectRegionRectangleLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("442$\
					 167;B$\
					 443;LP;-30;30;1;0$\
					 441;LP;0;1;1;1$\
					 267;SPR");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::RegistrateRectangleAlgo(VARIANT* rgnId, VARIANT* pSigma, VARIANT* pThreshold, VARIANT* pCalibrationImageID)
{
	//对比特征区域面积校正方盖
	int id = (int)rgnId->fltVal;
	float Sigma = (float)pSigma->fltVal;
	float Threshold = (float)pThreshold->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;	
	m_vInterImgs[id] = Hobject();
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	HImage Image = m_crtImg;
	HRegion WorkingRegion = m_vRgn[id];
	CComVariant retValue;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bPilotImg)
			return S_OK;

		if(WorkingRegion.Area() <= 0)
		{
			retValue = -1;
			retValue.Detach(pCalibrationImageID);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;			
		}
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		HRegionArray WorkingRegions = WorkingRegion.Connection();
		HTuple Length;
		count_obj(WorkingRegions.Id(), &Length);
		if(Length != 2)
		{
			retValue = -1;
			retValue.Detach(pCalibrationImageID);
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			return S_FALSE;	
		}
		HTuple Areas;
		Hobject Edges;
		for(int i = 0; i < 2; i++)
		{
			Hobject SelectedObj;
			select_obj(WorkingRegions.Id(), &SelectedObj, i + 1);
			HImage ReducedImage = Image.ReduceDomain(HRegion(SelectedObj));
			HImage GaussImage = ReducedImage.DerivateGauss(1,"gradient");
			HRegion Edge = GaussImage.Threshold(Threshold, 255);
			concat_obj(Edges, Edge.Id(), &Edges);
			Hlong Area = Edge.Area();
			Areas.Append(Area);
		}
		HImage FinalImage;
		if (Areas.Select(0) > Areas.Select(1))
		{
			HTuple HomMat2D;
			vector_angle_to_rigid(fCenterY, fCenterX, 0, fCenterY, fCenterX, HTuple(180).Rad(), &HomMat2D);
			FinalImage = Image.AffineTransImage(HomMat2D, "nearest_neighbor", "false");
			affine_trans_region(Edges, &Edges, HomMat2D, "nearest_neighbor");
			affine_trans_region(m_centerLocationRegion, &m_centerLocationRegion, HomMat2D, "nearest_neighbor");
		}
		else
			FinalImage = Image;

		m_vRgnExTest[id] = Edges;
		//匹配参数，画图软件需要。匹配算子计算后会被刷新。
		m_modelMatchingPARA[0] = m_imageRadius;
		m_modelMatchingPARA[1] = m_imgHeight/2.0;
		m_modelMatchingPARA[2] = m_imgWidth/2.0;
		m_modelMatchingPARA[3] = 0.0;
		//
		m_angleMatchingPARA[1] = m_imgHeight/2.0;
		m_angleMatchingPARA[2] = m_imgWidth/2.0;
		m_angleMatchingPARA[3] = 0.0;
		m_angleDetect = m_modelMatchingPARA[3].D();

		m_crtImgMult = FinalImage.Id();
		m_vInterImgs[id] = FinalImage.Id();
		retValue = (long)&m_crtImgMult;
		//区分通道
		HTuple  Channels;
		count_channels(m_crtImgMult, &Channels);
		//黑白图片
		if(Channels[0].I() == 3)
			rgb1_to_gray(m_crtImgMult, &m_crtImg);
		else if(Channels[0].I() == 1)
			m_crtImg  = m_crtImgMult;
		else
			mean_n(m_crtImgMult, &m_crtImg);

		retValue.Detach(pCalibrationImageID);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		retValue.Detach(pCalibrationImageID);
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();	
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CEoeAlgo::RegistrateRectangleAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1404$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 680;FP;0;20;1;1$\
					 1405;FP;0;255;0.1;10"); 
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void CEoeAlgo::GrayValueContrast(int DetectType, HTuple Diff, int Threshold, HTuple* Indices)
{
	HTuple TupleSub, Sgn;
	//Hobject
	tuple_sub(Diff, Threshold * DetectType, &TupleSub);
	tuple_sgn(TupleSub, &Sgn);
	tuple_find(Sgn, DetectType, Indices);
	if(*Indices == -1)
		*Indices = HTuple();
	//	
}

void CEoeAlgo::GenerateDefectRegions(HTuple RowsDefect, HTuple ColumnsDefect, int DefectSize, Hobject* SelectedEdges, HTuple* Area)
{
	Hobject Edges;
	HTuple RowsEdge, ColsEdge, Length;
	gen_region_points(&Edges, RowsDefect, ColumnsDefect);
	union1(Edges, &Edges);
	connection(Edges, &Edges);
	select_shape(Edges, SelectedEdges, "area", "and", DefectSize, 999999);
	area_center(*SelectedEdges, Area, &RowsEdge, &ColsEdge);
	tuple_length(*Area, &Length);
	if(Length >=1)
		tuple_sum(*Area, Area);
	else
		*Area = 0;
	return;
}

STDMETHODIMP CEoeAlgo::EdgeDetectRectanglePixelWiseAlgo(VARIANT* rgnId, VARIANT* pDefectType, VARIANT* pThresholdLight, VARIANT* pThresholdDark, VARIANT* pSharpness, VARIANT* pDefectSizeLight, VARIANT* pDefectSizeDark, VARIANT* pDetectArea)
{
	//
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
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
	int DefectType = (int)pDefectType->fltVal;
	int ThresholdLight = (int)pThresholdLight->fltVal;
	int ThresholdDark = (int)pThresholdDark->fltVal;
	int Sharpness = (int)pSharpness->fltVal;
	int DefectSizeLight = (int)pDefectSizeLight->fltVal;
	int DefectSizeDark = (int)pDefectSizeDark->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
	HImage Image = m_crtImg;
	Hobject  EdgesDark, SelectedEdgesDark, SelectedEdgesLight;
	HTuple GrayValNew, Diff, IndicesLight, IndicesDark, RowsDefectLight, ColumnsDefectLight, RowsDefectDark, ColumnsDefectDark;
	HRegion Domain = Image.GetDomain();
	WorkingRegion = Domain.Intersection(WorkingRegion);
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		HRegion RegionFill	= WorkingRegion.FillUp();
		HImage ReducedImage = Image.ReduceDomain(WorkingRegion);
		HRegion Present = RegionFill;
		//	HImage MeanImage = ReducedImage.MeanImage(3, 3);
		for(int i = 0; i < RoiWidth; i++)
		{
			HTuple Columns;
			HTuple Rows = Present.GetRegionContour(&Columns);
			if (Rows.Num() < Sharpness)
				break;

			Present = RegionFill.ErosionCircle(i + 1.5);
			HTuple GrayVal = ReducedImage.GetGrayval(Rows, Columns);
			HTuple GrayValShifted = GrayVal(Sharpness, Rows.Num() - 1);
			GrayValShifted.Append(GrayVal(0, Sharpness - 1));
			tuple_sub(GrayVal, GrayValShifted, &Diff);
			if(DefectType == 1)
				IndicesLight = GrayValueDifferenceSegment(Diff, ThresholdLight, LIGHT);
			else if(DefectType == -1)
				IndicesDark = GrayValueDifferenceSegment(Diff, ThresholdDark, DARK);
			else
			{
				IndicesLight = GrayValueDifferenceSegment(Diff, ThresholdLight, LIGHT);
				IndicesDark = GrayValueDifferenceSegment(Diff, ThresholdDark, DARK);
			}
			RowsDefectLight.Append(Rows.Select(IndicesLight));
			ColumnsDefectLight.Append(Columns.Select(IndicesLight));
			RowsDefectDark.Append(Rows.Select(IndicesDark));
			ColumnsDefectDark.Append(Columns.Select(IndicesDark));
		}
		HTuple LightDefectArea = 0;
		HTuple DarkDefectArea = 0;
		if(DefectType == LIGHT)
			GenerateDefectRegions(RowsDefectLight, ColumnsDefectLight, DefectSizeLight, &SelectedEdgesLight,&LightDefectArea);
		else if(DefectType == DARK)
			GenerateDefectRegions(RowsDefectDark, ColumnsDefectDark, DefectSizeDark, &SelectedEdgesDark, &DarkDefectArea);
		else
		{
			GenerateDefectRegions(RowsDefectLight, ColumnsDefectLight, DefectSizeLight, &SelectedEdgesLight, &LightDefectArea);
			GenerateDefectRegions(RowsDefectDark, ColumnsDefectDark, DefectSizeDark, &SelectedEdgesDark, &DarkDefectArea);		
		}
		Hlong Area = LightDefectArea[0].I() + DarkDefectArea[0].I();
		if(Area <= 0)
		{
			retValue = 0.0;
			m_vErrorRgn[id] = Hobject();
		}
		else
		{
			retValue = Area;
			dilation_circle (SelectedEdgesLight, &SelectedEdgesLight, 2.5);
			dilation_circle(SelectedEdgesDark, &SelectedEdgesDark, 2.5);
			m_vErrorRgn[id] = SelectedEdgesDark;
			m_vWhiteErrorRgn[id] = SelectedEdgesLight;
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
STDMETHODIMP CEoeAlgo::EdgeDetectRectanglePixelWiseAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("1387$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 385;LP;-1;1;1;0$\
					 409;LP;0;255;1;20$\
					 407;LP;0;255;1;20$\
					 1388;LP;1;50;1;5$\
					 410;LP;1;100;1;10$\
					 408;LP;1;100;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::DefectDetectionRectanglePixelWiseAlgo(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pSharpness, VARIANT* pDefectSize, VARIANT* pDefectArea)
{
	//
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	HRegion WorkingRegion = m_vRgn[id];
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	CComVariant retValue;
	if(WorkingRegion.Area() <= 0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
	int Threshold = (int)pThreshold->fltVal;
	int Sharpness = (int)pSharpness->fltVal;
	int DefectSize = (int)pDefectSize->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;

	HImage Image = m_crtImg;
	HTuple RowsDefect, ColumnsDefect;
	//in case working region is out of image
	HRegion Domain = Image.GetDomain();
	WorkingRegion = Domain.Intersection(WorkingRegion);
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		HRegion RegionFill	= WorkingRegion.FillUp();
		HImage ReducedImage = Image.ReduceDomain(WorkingRegion);
		HRegion Present = RegionFill;
		//	HImage MeanImage = ReducedImage.MeanImage(3, 3);	
		for(int i = 0; i < RoiWidth; i++)
		{
			HTuple Columns;
			HTuple Rows = Present.GetRegionContour(&Columns);
			Present = RegionFill.ErosionCircle(i + 1.5);
			HTuple GrayVal = ReducedImage.GetGrayval(Rows, Columns);
			HTuple GrayValShifted = GrayVal(Sharpness, Rows.Num() - 1);
			GrayValShifted.Append(GrayVal(0, Sharpness - 1));
			HTuple Diff;
			tuple_sub(GrayVal, GrayValShifted, &Diff);
			HTuple EdgeIndices = GrayValueDifferenceSegment(Diff, Threshold, NOT_EQUAL);
			RowsDefect.Append(Rows.Select(EdgeIndices));
			ColumnsDefect.Append(Columns.Select(EdgeIndices));
		}
		Hobject Edges;
		HTuple RowsEdge, ColsEdge, Length;
		gen_region_points(&Edges, RowsDefect, ColumnsDefect);
		union1(Edges, &Edges);
		connection(Edges, &Edges);
		Hobject SelectedEdges;
		select_shape(Edges, &SelectedEdges, "area", "and", DefectSize, 999999);
		HTuple Area;

		area_center(SelectedEdges, &Area, &RowsEdge, &ColsEdge);
		tuple_length(Area, &Length);
		if(Length >=1)
			tuple_sum(Area, &Area);
		else
			Area = 0;

		if(Area == 0)
		{
			retValue = 0.0;
			m_vErrorRgn[id] = Hobject();
		}
		else
		{
			retValue = Area[0].I();
			Hobject dia;
			dilation_circle(SelectedEdges, &dia, 2.5);
			m_vErrorRgn[id] = dia;
		}		
		retValue.Detach(pDefectArea);

		/*HRegion Edge = Edge.GenRegionPoints(RowsDefect, ColumnsDefect);
		HRegionArray Edges = Edge.Connection();
		HRegionArray SelectedEdges = Edges.SelectShape("area", "and", DefectSize, MAX_AREA);
		HRegion Defect = Defect.GenEmptyRegion();
		Defect = Defect.Union2(SelectedEdges);
		if(Defect.Area() == 0)
		{
		retValue = 0.0;
		m_vErrorRgn[id] = Hobject();
		}
		else
		{
		retValue = Defect.Area();
		HRegion DilatedDefect = Defect.DilationCircle(2.5);
		m_vErrorRgn[id] = DilatedDefect.Id();
		}		
		retValue.Detach(pDefectArea);*/

#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CEoeAlgo::DefectDetectionRectanglePixelWiseAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2011$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 2010;LP;0;255;1;20$\
					 1388;LP;1;50;1;5$\
					 434;LP;1;100;1;10");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::DefectDetectionRectanglePixelWiseDarkLightAlgo(VARIANT* rgnId, VARIANT* pSharpness, VARIANT* pThresholdLight, VARIANT* pDefectSizeLight, VARIANT* pThresholdDark, VARIANT* pDefectSizeDark, VARIANT* pDefectArea)
{
	//
	int id = (int)rgnId->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	HRegion WorkingRegion = m_vRgn[id];
	float RoiWidth = m_ringPara[id].m_RoiWidth;
	CComVariant retValue;
	if(WorkingRegion.Area() <= 0)
	{
		retValue = -1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}

	int Sharpness = (int)pSharpness->fltVal;
	int ThresholdLight = (int)pThresholdLight->fltVal;
	int DefectSizeLight = (int)pDefectSizeLight->fltVal;
	int ThresholdDark = (int)pThresholdDark->fltVal;
	int DefectSizeDark = (int)pDefectSizeDark->fltVal;
	float fCenterX = m_vPos[0].m_x;
	float fCenterY = m_vPos[0].m_y;
	HImage Image = m_crtImg;
	//in case working region is out of image
	HRegion Domain = Image.GetDomain();
	WorkingRegion = Domain.Intersection(WorkingRegion);
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
			m_dictSpecialData[id].clear();

		HRegion RegionFill	= WorkingRegion.FillUp();
		HImage ReducedImage = Image.ReduceDomain(WorkingRegion);
		HRegion Present = RegionFill;
		//	HImage MeanImage = ReducedImage.MeanImage(3, 3);
		//	HRegion Ring = Ring.GenEmptyRegion();	
		HTuple RowsDefectLight, ColsDefectLight, RowsDefectDark, ColsDefectDark;
		for(int i = 0; i < RoiWidth; i++)
		{
			HTuple Columns;
			HTuple Rows = Present.GetRegionContour(&Columns);
			Present = RegionFill.ErosionCircle(i + 1.5);
			HTuple GrayVal = ReducedImage.GetGrayval(Rows, Columns);
			HTuple GrayValShifted = GrayVal(Sharpness, Rows.Num() - 1);
			GrayValShifted.Append(GrayVal(0, Sharpness - 1));
			HTuple Diff;
			tuple_sub(GrayVal, GrayValShifted, &Diff);
			HTuple LightIndices = GrayValueDifferenceSegment(Diff, ThresholdLight, LIGHT);
			HTuple LightComparison = GrayValueDifferenceSegment(Diff, ThresholdLight, DARK);
			LightComparison = LightComparison + Sharpness;
			HTuple Over = LightComparison.GreaterElem(HTuple(Rows.Num())).Find(1);
			if(Over == -1)
				Over = HTuple();

			LightComparison.ReplaceElements(Over, LightComparison.Select(Over) - Rows.Num());
			HTuple LightInter = LightComparison.Intersection(LightIndices);
			RowsDefectLight.Append(Rows.Select(LightInter));
			ColsDefectLight.Append(Columns.Select(LightInter));
			HTuple DarkIndices = GrayValueDifferenceSegment(Diff, ThresholdDark, DARK);
			HTuple DarkComparison = GrayValueDifferenceSegment(Diff, ThresholdDark, LIGHT);
			DarkComparison = DarkComparison + Sharpness;
			Over = DarkComparison.GreaterElem(Rows.Num()).Find(1);
			if(Over == -1)
				Over = HTuple();

			DarkComparison.ReplaceElements(Over, DarkComparison.Select(Over) - Rows.Num());
			HTuple DarkInter = DarkComparison.Intersection(DarkIndices);	        
			RowsDefectDark.Append(Rows.Select(DarkInter));
			ColsDefectDark.Append(Columns.Select(DarkInter));
		}
		HRegion EdgeLight = EdgeLight.GenRegionPoints(RowsDefectLight, ColsDefectLight);
		HRegionArray EdgesLight = EdgeLight.Connection();
		HRegionArray SelectedEdgesLight = EdgesLight.SelectShape("area", "and", DefectSizeLight, 999999);
		HRegion DefectLight = DefectLight.GenEmptyRegion();
		DefectLight = DefectLight.Union2(SelectedEdgesLight);
		HRegion EdgeDark = EdgeDark.GenRegionPoints(RowsDefectDark, ColsDefectDark);
		HRegionArray EdgesDark = EdgeDark.Connection();
		HRegionArray SelectedEdgesDark = EdgesDark.SelectShape("area", "and", DefectSizeDark, 999999);
		HRegion DefectDark = DefectDark.GenEmptyRegion();
		DefectDark = DefectDark.Union2(SelectedEdgesDark);
		if(DefectLight.Area() == 0)
			m_vWhiteErrorRgn[id] = Hobject();
		else
		{
			HRegion DilatedDefectLight = DefectLight.DilationCircle(2.5);
			m_vWhiteErrorRgn[id] = DilatedDefectLight.Id();
		}
		if(DefectDark.Area() == 0)
			m_vErrorRgn[id] = Hobject();
		else
		{
			HRegion DilatedDefectDark = DefectDark.DilationCircle(2.5);
			m_vErrorRgn[id] = DilatedDefectDark.Id();
		}		
		retValue = DefectLight.Area() + DefectDark.Area();
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif

	return S_OK;
}
STDMETHODIMP CEoeAlgo::DefectDetectionRectanglePixelWiseDarkLightAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("2012$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 1388;LP;1;50;1;5$\
					 409;LP;0;255;1;20$\
					 410;LP;1;100;1;10$\
					 407;LP;0;255;1;20$\
					 408;LP;1;100;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CEoeAlgo::CurlOptimizeRectangleAlgo(VARIANT* rgnId, VARIANT* pPartsWidth, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
{
	//Apply bandpass filter on rectangular lids
	//Written by Jessie Sheng
	int id = (int)rgnId->fltVal;
	float PartsWidth = (float)pPartsWidth->fltVal;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	Hobject rgn = m_vRgn[id];
	float RoiWidth = m_ringPara[id].m_RoiWidth;	
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
		if(FreqStart > FreqCutoff)
		{
			retValue = -1;
			m_vErrorRgn[id].Reset();
			m_vWhiteErrorRgn[id].Reset();
			retValue.Detach(pDetectNumber);
			return S_FALSE;
		}
		Hobject Image = m_crtImg;
		Hobject OuterBorder, RegionFilled, InnerRegion, InnerBorder, RegionLines, CurlPartitioned;
		HTuple RowsOuter, ColsOuter, RowsOuterAdopted, ColsOuterAdopted, RowsInner, ColsInner, RowsInnerAdopted, ColsInnerAdopted;
		HTuple LengthOuter, LengthInner, PartsNum, OuterIndices, InnerIndices;
		HTuple MeanCurl, DeviationCurl, MeanValueCurl, Row_Defect, Column_Defect, MeanDiffValues, Area; 
		boundary(rgn, &OuterBorder, "inner_filled");
		fill_up(rgn, &RegionFilled);
		erosion_circle(RegionFilled, &InnerRegion, m_ringPara[id].m_RoiWidth -1);
		boundary(InnerRegion, &InnerBorder, "inner_filled");
		get_region_contour(OuterBorder, &RowsOuter, &ColsOuter);
		get_region_contour(InnerBorder, &RowsInner, &ColsInner);
		tuple_length(RowsOuter, &LengthOuter);
		tuple_length(RowsInner, &LengthInner);
		LengthOuter = LengthOuter / 1.0;
		tuple_gen_sequence(0, LengthOuter - 1, 2, &OuterIndices);
		RowsOuterAdopted = RowsOuter.Select(OuterIndices);
		ColsOuterAdopted = ColsOuter.Select(OuterIndices);
		InnerIndices = (OuterIndices / LengthOuter * LengthInner).Floor();
		RowsInnerAdopted = RowsInner.Select(InnerIndices);
		ColsInnerAdopted = ColsInner.Select(InnerIndices);
		gen_region_line(&RegionLines, RowsOuterAdopted, ColsOuterAdopted, RowsInnerAdopted, ColsInnerAdopted);
		dilation_circle(RegionLines, &CurlPartitioned, PartsWidth / 2);
		intersection(CurlPartitioned, rgn, &CurlPartitioned);
		intensity(CurlPartitioned, Image, &MeanCurl, &DeviationCurl);
		tuple_mean(MeanCurl, &MeanValueCurl);
		tuple_sub(MeanCurl, MeanValueCurl, &MeanDiffValues);
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
		ParamLength = (int)((6.6*PI)/(angFreqCutoff0 - angFreqCutoff)+0.5);
		if(ParamLength>=0.5*InputDataLength)
		{
			ParamLength = (int)(0.5*InputDataLength);
		}
		if (0 == ParamLength%2)
			ParamLength++;
		Unit_Impulse_Response(ParamLength, angFreqStart, angFreqCutoff, 11, vFilterParam);
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
			select_obj(CurlPartitioned, &DefectReg, DefectIndices+1);

			Hobject DefectUnionReg;
			union1(DefectReg, &DefectUnionReg);
			connection(DefectUnionReg, &DeftectRegConnection);
			fill_up(DeftectRegConnection, &DeftectRegConnection);
			select_shape(DeftectRegConnection, &DefectReg, "area", "and", MinArea, 99999);
			count_obj(DefectReg, &DefectNum);
			union1 (DefectReg, &DefectRegs);
			area_center (DefectRegs, &Area, &Row_Defect, &Column_Defect);
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
STDMETHODIMP CEoeAlgo::CurlOptimizeRectangleAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("380$\
					 397;R;LP;0;1000;1;<=#20$\
					 381;B$\
					 748;FP;1;20;1;3$\
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

STDMETHODIMP CEoeAlgo::CurlDetectAlgo1(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
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
		write_region(Rectangles.Id(), "C://rectangles.reg");
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
STDMETHODIMP CEoeAlgo::CurlDetectAlgo1Help(BSTR* pHelpStr)
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
