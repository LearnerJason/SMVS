#include "stdafx.h"
#include "resource.h"
#include "AlPlasticCapAlgo_i.h"
#include "dllmain.h"
#include "cpp/HalconCpp.h"
#include <string>
#include "AlPlasticCapAlgo.h"

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

STDMETHODIMP CAlPlasticCapAlgo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IAlPlasticCapAlgo
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CAlPlasticCapAlgo::SetCurrentImage(LONG* pImg)
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

STDMETHODIMP CAlPlasticCapAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CAlPlasticCapAlgo::GetRegionEx(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnEx[rgnID];
	return S_OK;
}
STDMETHODIMP CAlPlasticCapAlgo::GetRegionExTest(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgnExTest[rgnID];
	return S_OK;
}

STDMETHODIMP CAlPlasticCapAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_regLearning;
	return S_OK;
}
STDMETHODIMP CAlPlasticCapAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}

STDMETHODIMP CAlPlasticCapAlgo::GetWhiteErrorRegion(BYTE rgnID,  LONG* pWhiteErrorRgn)
{
	Hobject* pObj = (Hobject*)pWhiteErrorRgn;
	*pObj = m_vWhiteErrorRgn[rgnID];
	return S_OK;
}
STDMETHODIMP CAlPlasticCapAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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
STDMETHODIMP CAlPlasticCapAlgo::GetCurrentLocationXY(BSTR* bstrLocationXY)
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

STDMETHODIMP CAlPlasticCapAlgo::GetCenterLocation(BSTR* bstrLocationXY)
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
bool CAlPlasticCapAlgo::ReadShapeModelAxieData(string strModelPath, string strExactModelPath,HTuple *ModelPARA)
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

STDMETHODIMP CAlPlasticCapAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
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

STDMETHODIMP CAlPlasticCapAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
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

STDMETHODIMP CAlPlasticCapAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	return S_FALSE;
}

STDMETHODIMP CAlPlasticCapAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_OK;
}

STDMETHODIMP CAlPlasticCapAlgo::SaveResult(BSTR* strModelName,BYTE productID)
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

STDMETHODIMP CAlPlasticCapAlgo::ResetModels(BYTE productID)
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

STDMETHODIMP CAlPlasticCapAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
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
STDMETHODIMP CAlPlasticCapAlgo::GetSelfLearningResult(LONG*  multiModelImag, long* cMax, float* vImgScore, BYTE productID)
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
STDMETHODIMP CAlPlasticCapAlgo::ModelCreate(double Radius, HTuple * ModelID1)
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

STDMETHODIMP CAlPlasticCapAlgo::CenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationAlgoHS(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationAlgoHSHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationAlgo1Help(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationAlgoUsePointFitting(VARIANT* rgnID, VARIANT* pSearchRegRow, VARIANT* pSearchRegCol, VARIANT* pOuterRadius, VARIANT* pRoiWidth, VARIANT* pScanLineNum, VARIANT* pScanLineDirection, VARIANT* pTransition, VARIANT* pPointSelection, VARIANT* pSigma, VARIANT* pThreshold)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationAlgoUsePointFittingHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterRectangleLocationAlgoHS(VARIANT* rgnId, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
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
STDMETHODIMP CAlPlasticCapAlgo::CenterRectangleLocationAlgoHSHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CircleRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CAlPlasticCapAlgo::CircleRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CAlPlasticCapAlgo::CircleRegionLocationAlgo1Help(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::BoundaryRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
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
STDMETHODIMP CAlPlasticCapAlgo::BoundaryRegionLocationAlgoHelp(BSTR* pHelpStr)
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


//calibration 
STDMETHODIMP CAlPlasticCapAlgo::CircleRegionAngleAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
STDMETHODIMP CAlPlasticCapAlgo::CircleRegionAngleAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CircleRegionAngleAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
STDMETHODIMP CAlPlasticCapAlgo::CircleRegionAngleAlgo1Help(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::SectorRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
STDMETHODIMP CAlPlasticCapAlgo::SectorRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::SectorRegionLocationAlgo1(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth,VARIANT* pDriftAngle1,VARIANT* pDriftAngle2)
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
STDMETHODIMP CAlPlasticCapAlgo::SectorRegionLocationAlgo1Help(BSTR* pHelpStr)
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


//Matching
STDMETHODIMP CAlPlasticCapAlgo::MatchingParaAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
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
				find_shape_model(ImageReduced,modelID, HTuple(-10).Rad(), HTuple(20).Rad(), 0.5, 1,0.5, "none",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M);
			}
			else
			{
				find_shape_model(ImageReduced,modelID, HTuple(0).Rad(), HTuple(360).Rad(), 0.5, 1,0.5, "none",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M);	
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
STDMETHODIMP CAlPlasticCapAlgo::MatchingParaAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::MatchingParaMinScoreAlgo(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
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
STDMETHODIMP CAlPlasticCapAlgo::MatchingParaMinScoreAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::DetectRegionLocationAlgo(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
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
STDMETHODIMP CAlPlasticCapAlgo::DetectRegionLocationAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DetectRegionLocationExtractOCRAlgo(VARIANT* rgnId,VARIANT* pRadius,VARIANT* pErosionSize, BSTR* bstrOcrRgnFile)
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
STDMETHODIMP CAlPlasticCapAlgo::DetectRegionLocationExtractOCRAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DetectRegionLocationAlgoOCR(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile)
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
STDMETHODIMP CAlPlasticCapAlgo::DetectRegionLocationAlgoOCRHelp(BSTR* pHelpStr)
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


//Threshold
STDMETHODIMP CAlPlasticCapAlgo::BlackDetectAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::BlackDetectAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::MinMaxGrayDetectAlgo(VARIANT* rgnId, VARIANT* pBlackThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteThresh,VARIANT* pSeriousWhitePointSize,VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::MinMaxGrayDetectAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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

STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoBlackHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoBlack(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectArea)
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

STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoBlackNumberHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoBlackNumber(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pDetectNumber)
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

STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoWhiteHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoWhite(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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

STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoWhiteNumberHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoWhiteNumber(VARIANT* rgnId, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectNumber)
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

STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoBlackWhiteNumberHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoBlackWhiteNumber(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectNumber)
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

STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoOCR(VARIANT* rgnId, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pErosionSize, VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdAlgoOCRHelp(BSTR* pHelpStr)
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



//Rotating Detect
STDMETHODIMP CAlPlasticCapAlgo::RotatingAngleDetectAlgo(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pBlackThredLow,VARIANT* pEdgeType,VARIANT* pEdgeDist, VARIANT* pDefectSize, VARIANT* pDetectAngle)
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
STDMETHODIMP CAlPlasticCapAlgo::RotatingAngleDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::AnnularRotaDynThresholdAlgo(VARIANT* rgnId,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::AnnularRotaDynThresholdAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::AnnularRotaDynThresholdAlgoBW(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::AnnularRotaDynThresholdAlgoBWHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::GSurfaceAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::GSurfaceAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
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

STDMETHODIMP CAlPlasticCapAlgo::GLineAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::GLineAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
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

STDMETHODIMP CAlPlasticCapAlgo::GLineNewAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::GLineNewAlgo(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackThredHigh, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteThredHigh, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea)
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

STDMETHODIMP CAlPlasticCapAlgo::EdgeAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::EdgeAlgo(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
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

STDMETHODIMP CAlPlasticCapAlgo::GContoursinkAlgo(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea)
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

STDMETHODIMP CAlPlasticCapAlgo::GContoursinkAlgoHelp(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CurlOptimizeAlgoHelp(BSTR* pHelpStr)
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

void CAlPlasticCapAlgo::ZoningRing(Hobject Image, Hobject Ring, Hobject MiddleCircle, Hobject* ZoningReg, HTuple CenterRow, HTuple  CenterColumn, int RingWidth, HTuple* tuple_RegMean_DCoffset)
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

double CAlPlasticCapAlgo::Sinc(double n)
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

void CAlPlasticCapAlgo::Unit_Impulse_Response(int paraLength, double angFreqStart, double angFreqCutoff, int winType, std::vector<double>& vFilterParam)
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

void CAlPlasticCapAlgo::BandPass_Filter(std::vector<double>& vFilterParam, int ParaLength, std::vector<double>& vInputData, std::vector<double>& vOutputData)
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

void CAlPlasticCapAlgo::PhaseAdjust(std::vector<double>& vOutputData, int FilterWidth)
{
	int outputSz = (int)vOutputData.size();
	std::vector<double> vOutTemp1;
	vOutTemp1.reserve(outputSz);

	vOutTemp1.assign(vOutputData.begin() + (outputSz-FilterWidth),  vOutputData.begin() + outputSz);
	vOutTemp1.insert(vOutTemp1.end(), vOutputData.begin(), vOutputData.begin() + outputSz - FilterWidth);
	std::copy(vOutTemp1.begin(), vOutTemp1.begin() + outputSz, vOutputData.begin());
}

void CAlPlasticCapAlgo::Tuple_Replace(HTuple& tuple, HTuple Indices, HTuple ReplaceValue)
{
	HTuple  Length, SelectedIndice;
	tuple_length(Indices, &Length);
	for (int i = 0; i < Length; i++)
	{
		tuple_select(Indices, i, &SelectedIndice);
		tuple[SelectedIndice] = ReplaceValue;
	}
}

STDMETHODIMP CAlPlasticCapAlgo::CurlOptimizeAlgo(VARIANT* rgnId, VARIANT* pFreqStart, VARIANT* pFreqCutoff, VARIANT* pWinType, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber)
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

STDMETHODIMP CAlPlasticCapAlgo::CurlDetectAlgo2Help(BSTR* pHelpStr)
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
STDMETHODIMP CAlPlasticCapAlgo::CurlDetectAlgo2(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue, VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::CurlNoCompoundAlgo(VARIANT* rgnId,VARIANT*pGrayValueMean )
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
STDMETHODIMP CAlPlasticCapAlgo::CurlNoCompoundAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("436$\
					 200;R;LP;1;255;1;<=#20$\
					 167;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CAlPlasticCapAlgo::ResetSelfLearningProcedure(void)
{
	// TODO: 在此添加实现代码

	return S_OK;
}


void CAlPlasticCapAlgo::ImgEnhancement(Hobject crtImg, Hobject rgn, Hobject &imgReduced)
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

STDMETHODIMP CAlPlasticCapAlgo::ReloadExternalFile(BSTR* bstrPath)
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

void CAlPlasticCapAlgo::ErrorDetect(Hobject ImageReduced, int MaskSize, float SeriousPointDynThresh, int SeriousPointSize, Hobject& DetectUnion, bool defectType)
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

STDMETHODIMP CAlPlasticCapAlgo::UVCurlAlgo(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::UVCurlAlgoHelp(BSTR* pHelpStr)
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

bool CAlPlasticCapAlgo::ReadModelsForOcr( BSTR* bstrShm1File, BSTR* bstrShm2File, BSTR* bstrShm3File, BSTR* bstrShm4File,bool errorInfoLanguage,HTuple &m_modelsForOcr,HTuple& m_matrixForOcr,Hobject& m_modelImagesForOcr)
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

STDMETHODIMP CAlPlasticCapAlgo::DetectOcrDistOffsetAlgo(VARIANT* rgnId,VARIANT* pDistanceOffset)
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
STDMETHODIMP CAlPlasticCapAlgo::DetectOcrDistOffsetAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("587$\
					 247;R;LP;0;200;1;<=#20$\
					 167;B" );
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CAlPlasticCapAlgo::DerivateGaussDetectAlgo(VARIANT* rgnId,  VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::DerivateGaussDetectAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::PrintErrorDetectOcrAlgo(VARIANT* rgnId,VARIANT* pLowContrast,VARIANT* pHeightContrast,VARIANT* pDilationSize,VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::PrintErrorDetectOcrAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
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
STDMETHODIMP CAlPlasticCapAlgo::SetCurrentTaskName2(BSTR* bstrTaskName,BSTR* bstrChName,BSTR* bstrChNameWithoutCameraIndex)
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

STDMETHODIMP CAlPlasticCapAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO: 在此添加实现代码

	return S_OK;
}



//Color image ：process Algo.
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationForColorAlgoHS(VARIANT* rgnId,  VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize)
{
	//利用边缘轮廓来定位圆
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
STDMETHODIMP CAlPlasticCapAlgo::CenterLocationForColorAlgoHSHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::RingLocationForColorAlgoHS(VARIANT* rgnId,  VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pRegionWidth)
{
	//利用边缘轮廓来定位圆
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
STDMETHODIMP CAlPlasticCapAlgo::RingLocationForColorAlgoHSHelp(BSTR* pHelpStr)
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

//side
void CAlPlasticCapAlgo::SkeletonSmooth(Hobject SkeletonReg, HTuple SmoothSize, Hobject* SkeletonSmoothReg)
{
	//对输入的骨架区域进行相应尺度的平滑 获得新区域
	//输入：骨架区域、平滑尺寸
	//输出：平滑后的骨架区域
	Hobject SkeletonRegPartitioned, SortedPartitioned, Contours, SmoothedContours;
	HTuple PartitionedNum, SkRegArea, SkRegRow,SkRegCol, EdgeRow, EdgeCol, XLDRow, XLDCol;
	EdgeRow = HTuple();
	EdgeCol = HTuple();
	area_center(SkeletonReg, &SkRegArea, &SkRegRow, &SkRegCol);
	if (SkRegArea == 0)
	{
		return;
	}
	partition_rectangle(SkeletonReg, &SkeletonRegPartitioned, 1, 50);
	sort_region(SkeletonRegPartitioned, &SortedPartitioned, "first_point", "true", "column");
	count_obj(SortedPartitioned, &PartitionedNum);
	for (int i=0; i<=PartitionedNum-1; i++)
	{
		Hobject ObjectSelected;
		HTuple ObjectArea, ObjectRow, ObjectColumn;
		select_obj (SortedPartitioned, &ObjectSelected, i+1);
		area_center (ObjectSelected, &ObjectArea, &ObjectRow, &ObjectColumn);
		EdgeRow = EdgeRow.Concat(ObjectRow.Select(0));
		EdgeCol = EdgeCol.Concat(ObjectColumn.Select(0));
	}
	gen_contour_polygon_xld (&Contours, EdgeRow, EdgeCol);
	SmoothSize = SmoothSize*2 + 1;
	smooth_contours_xld (Contours, &SmoothedContours, SmoothSize);
	get_contour_xld (SmoothedContours, &XLDRow, &XLDCol);
	if (XLDRow.Num()==0)
	{
		return;
	}
	gen_region_polygon (&(*SkeletonSmoothReg), XLDRow, XLDCol);//可将因平滑丢失的骨架像素点区域自动补充，不能用gen_region_points（）
	return;
}
/*********************************************************************************
** 函数名：SkeletonSmooth
** 功  能：骨架平滑函数
** 时  间：2019年12月19日
**********************************************************************************/
void CAlPlasticCapAlgo::SkeletonClosedRegion(Hobject Skeleton1, Hobject Skeleton2, Hobject* ClosedRegion)
{
	//输入区域必须为连续单个区域
	//将上下分布的骨架两侧进行闭合获得新区域
	//输入：两个区域
	//输出：一个闭合区域
	HTuple Area1, Row1, Col1, Area2, Row2, Col2, Area3, Row3, Col3, Number1, Number2;
	Hobject Skeletons1, Skeletons2;
	area_center(Skeleton1, &Area1, &Row1, &Col1);
	area_center(Skeleton2, &Area2, &Row2, &Col2);
	connection(Skeleton1, &Skeletons1);
	connection(Skeleton2, &Skeletons2);
	count_obj (Skeletons1, &Number1);
	count_obj (Skeletons2, &Number2);
	if ((Area1 == 0)||(Area2 == 0)||(Number1[0].D() != 1)||(Number2[0].D() != 1))
	{
		return;
	}
	Hobject Contours1, Contours2, LeftRegionLines, RightRegionLines, LRUnion, UDUnion, RegionUnion, FillRegionUnion;
	HTuple row1, col1, col2, row2, LeftX1, RightX1, LeftY1, RightY1, LeftX2, RightX2, LeftY2, RightY2, P0, P1, P2, P3;
	gen_contour_region_xld(Skeleton1, &Contours1, "border");
	gen_contour_region_xld(Skeleton2, &Contours2, "border");
	get_contour_xld(Contours1, &row1, &col1);
	get_contour_xld(Contours2, &row2, &col2);
	//
	//my_disp_obj(Contours1);
	tuple_min(col1, &LeftX1);
	tuple_max(col1, &RightX1);
	tuple_find_first(col1, LeftX1, &P0);
	LeftY1 = row1.Select(P0);
	tuple_find_first(col1, RightX1, &P1);
	RightY1 = row1.Select(P1);
	tuple_min(col2, &LeftX2);
	tuple_max(col2, &RightX2);
	tuple_find_first(col2, LeftX2, &P2);
	LeftY2 = row2.Select(P2);
	tuple_find_first(col2, RightX2, &P3);
	RightY2 = row2.Select(P3);
	gen_region_line(&LeftRegionLines, LeftY2-0.5, LeftX2, LeftY1-0.5, LeftX1);
	gen_region_line(&RightRegionLines, RightY2, RightX2-0.5, RightY1, RightX1-0.5);
	union2(LeftRegionLines, RightRegionLines, &LRUnion);
	union2(Skeleton1, Skeleton2, &UDUnion);
	union2(LRUnion, UDUnion, &RegionUnion);
	fill_up(RegionUnion, &FillRegionUnion);
	area_center(FillRegionUnion, &Area3, &Row3, &Col3);
	if ((Area3 == 0)||(Area3.Num()==0))
	{
		return;
	}
	(* ClosedRegion) = FillRegionUnion;
	return;
}
/*********************************************************************************
** 函数名：SkeletonClosedRegion
** 功  能：将上下分布的骨架两侧进行闭合 获得新区域
** 时  间：2019年12月19日
**********************************************************************************/
STDMETHODIMP CAlPlasticCapAlgo::SideCenterLocationAlgoForColor(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect,VARIANT* pDividingLine,VARIANT* pBackgroundGray,VARIANT* pAlThreshold,VARIANT* pPlasticThreshold, VARIANT* pSmoothSize)
{
	//铝塑盖侧面工站中心定位 定位铝片上边沿和塑片下边沿
	//铝片部分定位直接采用灰度图 塑片部分提供通道选择
	//获取中心坐标、斜率倒数、上下轮廓
	int id = (int)rgnId->fltVal;
	int ColorTransType     = (int)pColorTransType->fltVal;
	int ChannelSelect      = (int)pChannelSelect->fltVal;
	int DividingLine       = (int)pDividingLine->fltVal;
	int BackgroundGray     = (int)pBackgroundGray->fltVal;
	int AlThreshold        = (int)pAlThreshold->fltVal;
	int PlasticThreshold   = (int)pPlasticThreshold->fltVal;
	int SmoothSize		   = (int)pSmoothSize->fltVal;

	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject newRgn;
	m_vRgn[id] = newRgn;

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
			m_dictSpecialData[id].clear();
		}
		Hobject Image, GrayImage;
		HTuple  ChannelNum = 1;
		HTuple  iWidth, iHeight;
		get_image_size(m_crtImg,&iWidth,&iHeight );
		m_imgWidth  = (long)iWidth[0].I();
		m_imgHeight = (long)iHeight[0].I();
		count_channels(m_crtImgMult,&ChannelNum);
		if (ChannelNum[0].I()==3)
		{
			if(!H_ColorImageTransAll(m_crtImgMult,&Image,ColorTransType, ChannelSelect))
			{
				Hobject Circle,newRgn;
				gen_circle(&Circle, m_imgHeight/2.0, m_imgWidth/2.0, 100);
				m_vPos[0].m_x = (float)(m_imgWidth/2.0);
				m_vPos[0].m_y = (float)(m_imgHeight/2.0);
				m_fReciprocal = 0.0;
				m_vRgn[id] = newRgn;
				m_TopEdge = newRgn;
				m_BottomEdge = newRgn;
				return S_FALSE;
			}
		}
		else
		{
			Image = m_crtImg;
		}
		GrayImage = m_crtImg;

		HTuple  CenterH, YDtum, Number;
		Hobject Ellipse, Rectangle, LeftRectangle, RightRectangle, UnionRectangle, DtumRectangle, RegionDifference, ConnectedRegions, SortedRegions, UpRegion, DownRegion, UpImageReduced, DownImageReduced;
		Hobject Region, UpRegionFillUp, UpRegionOpening, UpRegionDilation, UpRegionMoved, UpRegionDifference, UpConnectedRegions, UpSelectedRegions;
		Hobject Region1, DownRegionFillUp, DownRegionOpening, ConnectedRegions1, SelectedRegions1, DownRegionDilation, DownRegionMoved, DownRegionDifference, DownConnectedRegions, DownSelectedRegions;
		Hobject UpSkeleton, DownSkeleton, tempUpSkeleton, tempDownSkeleton;
		HTuple  UpArea, UpCenterRow, UpCenterColumn, DownArea, DownCenterRow, DownCenterColumn;
		
		gen_ellipse (&Ellipse, iHeight/2.0, iWidth/2.0, 0.0, iWidth/1.6, iHeight/2.0);   //默认图片的大小有要求(/0.1)
		gen_rectangle1(&Rectangle, 0, 0, iHeight-1, iWidth-1);
		intersection (Ellipse, Rectangle, &Ellipse);
		CenterH = iHeight/2.0;
		YDtum = CenterH+DividingLine;
		if((YDtum < 5)||(YDtum>(iHeight-6)))
		{
			YDtum = CenterH;
		}
		gen_rectangle1 (&LeftRectangle, YDtum, 0, YDtum+1, iWidth/20.0);
		gen_rectangle1 (&RightRectangle, YDtum, iWidth*19.0/20.0, YDtum+1, iWidth-1);
		union2(LeftRectangle, RightRectangle, &UnionRectangle);
		gen_rectangle1 (&DtumRectangle, YDtum, 0, YDtum+1, iWidth-1);
		concat_obj(m_vRgnEx[id], UnionRectangle, &m_vRgnEx[id]);
		difference (Ellipse, DtumRectangle, &RegionDifference);
		connection (RegionDifference, &ConnectedRegions);
		sort_region (ConnectedRegions, &SortedRegions, "first_point", "true", "row");
		count_obj (SortedRegions, &Number);
		select_obj (SortedRegions, &UpRegion, 1);
		select_obj (SortedRegions, &DownRegion, Number);
		reduce_domain (GrayImage, UpRegion, &UpImageReduced);
		reduce_domain (Image, DownRegion, &DownImageReduced);
		threshold (UpImageReduced, &Region, AlThreshold, 255);
		fill_up (Region, &UpRegionFillUp);
		opening_rectangle1 (UpRegionFillUp, &UpRegionOpening, 21, 1);
		dilation_rectangle1 (UpRegionOpening, &UpRegionDilation, 11, 1);
		move_region (UpRegionDilation, &UpRegionMoved, 5, 0);
		difference (UpRegionOpening, UpRegionMoved, &UpRegionDifference);
		connection (UpRegionDifference, &UpConnectedRegions);
 		select_shape_std (UpConnectedRegions, &UpSelectedRegions, "max_area", 70);
		if (BackgroundGray == 0)
		{
			threshold (DownImageReduced, &Region1, 0, PlasticThreshold);
		} 
		else
		{
			threshold (DownImageReduced, &Region1, PlasticThreshold, 255);
		}
		fill_up (Region1, &DownRegionFillUp);
		opening_rectangle1 (DownRegionFillUp, &DownRegionOpening, 15, 1);
		connection (DownRegionOpening, &ConnectedRegions1);
		select_shape_std (ConnectedRegions1, &SelectedRegions1, "max_area", 70);
		dilation_rectangle1 (SelectedRegions1, &DownRegionDilation, 11, 1);
		move_region (DownRegionDilation, &DownRegionMoved, -5, 0);
		difference (SelectedRegions1, DownRegionMoved, &DownRegionDifference);
		connection (DownRegionDifference, &DownConnectedRegions);
		select_shape_std (DownConnectedRegions, &DownSelectedRegions, "max_area", 70);
		
		skeleton (UpSelectedRegions, &UpSkeleton);
		SkeletonSmooth(UpSkeleton, SmoothSize, &UpSkeleton);
		area_center (UpSkeleton, &UpArea, &UpCenterRow, &UpCenterColumn);
		skeleton (DownSelectedRegions, &DownSkeleton);
		SkeletonSmooth(DownSkeleton, SmoothSize, &DownSkeleton);
		area_center (DownSkeleton, &DownArea, &DownCenterRow, &DownCenterColumn);
		gen_rectangle1 (&tempUpSkeleton, iHeight/4.0, iWidth/5.0, iHeight/4.0+1, iWidth*4/5.0);
		gen_rectangle1 (&tempDownSkeleton, iHeight*3/4.0, iWidth/5.0, iHeight*3/4.0+1, iWidth*4/5.0);
		if (1 != (UpArea > 0))
		{
			UpSkeleton = tempUpSkeleton;
			area_center (UpSkeleton, &UpArea, &UpCenterRow, &UpCenterColumn);
		}
		if(1 != (DownArea > 0)) 
		{
			DownSkeleton = tempDownSkeleton;
			area_center (DownSkeleton, &DownArea, &DownCenterRow, &DownCenterColumn);
		}
		
		HTuple fReciprocal;  //斜率倒数
		if (UpCenterColumn == DownCenterColumn)
		{
			fReciprocal = 0.0;
		} 
		else
		{
			fReciprocal = (UpCenterColumn-DownCenterColumn)/((UpCenterRow-DownCenterRow)*1.0);
		}
		
		m_vPos[0].m_x = (UpCenterColumn[0].D() + DownCenterColumn[0].D())/2.0;
		m_vPos[0].m_y = (UpCenterRow[0].D() + DownCenterRow[0].D())/2.0;
		m_fReciprocal = fReciprocal;
		m_TopEdgeCenterY = UpCenterRow;
		m_BottomCenterY = DownCenterRow[0].D();
		m_TopEdge = UpSkeleton;
		m_BottomEdge = DownSkeleton;
		concat_obj(UpSkeleton,  DownSkeleton, &m_vRgn[id]);

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
STDMETHODIMP CAlPlasticCapAlgo::SideCenterLocationAlgoForColorHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4042$\
					 381;B$\
					 657;LP;0;6;1;0$\
					 658;LP;1;3;1;3$\
					 4043;LP;-200;200;1;0$\
					 4044;LP;0;1;1;0$\
					 4045;LP;1;255;1;40$\
					 4046;LP;1;255;1;78$\
					 4058;LP;1;30;1;5");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*********************************************************************************
** 函数名：SideCenterLocationAlgoForColor
** 功  能：针对彩色铝塑盖的侧面中心定位算子
** 时  间：2019年12月10日
**********************************************************************************/
STDMETHODIMP CAlPlasticCapAlgo::PlasticRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRowMove, VARIANT* pRelativeHeight, VARIANT* pWidthShrink)
{
	//得到塑片区域
	int id					= (int)rgnId->fltVal;
	int RowMove				= (int)pRowMove->fltVal;
	int RelativeHeight		= (int)pRelativeHeight->fltVal;
	float WidthShrink		= (float)pWidthShrink->fltVal;

	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject DownSkeleton = m_BottomEdge;
	Hobject  RegionDetect;
	Hobject newRgn;
	HTuple Area0, CenterRow0, CenterColumn0;
	m_vRgn[id] = newRgn;
	area_center(DownSkeleton, &Area0, &CenterRow0, &CenterColumn0);
	if (1 != (Area0 > 0))
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

		Hobject Rectangle, RegionDilation0, RegionShrink, DownSkeletonShrink, SkeletonMove, SkeletonMove1, SkeletonUnion;
		Hobject RegionUnion, RegionDilation, RegionDifference, ConnectedRegions, SortedRegions, ObjectSelected;
		HTuple ColumnMove, RowMove1, ColumnMove1, Number;
		HTuple  iWidth, iHeight, Area1, Row1, Column1, Area2, Row2, Column2;
		get_image_size(m_crtImg, &iWidth, &iHeight);
		m_imgWidth  = (long)iWidth[0].I();
		m_imgHeight = (long)iHeight[0].I();
		gen_rectangle1(&Rectangle, 0, 0, iHeight-1, iWidth-1);
		ColumnMove = m_fReciprocal*RowMove;
		if ((m_StepCenterY.Num()==0)||(m_BottomCenterY.Num()==0))
		{
			RowMove1 = -iHeight/4.0; 
		} 
		else
		{
			RowMove1 =  m_StepCenterY[0].D() - m_BottomCenterY[0].D();
		}
		RowMove1 = RowMove1-RelativeHeight;
		ColumnMove1 = m_fReciprocal * RowMove1;
		WidthShrink = WidthShrink + 1;
		dilation_rectangle1(DownSkeleton,  &RegionDilation0, 1, 20);
		erosion_rectangle1 (RegionDilation0, &RegionShrink, WidthShrink, 1);
		intersection(DownSkeleton, RegionShrink, &DownSkeletonShrink);
		area_center(DownSkeletonShrink, &Area1, & Row1, &Column1);
		if (1 != (Area1 > 0))
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		move_region(DownSkeletonShrink, &SkeletonMove, RowMove, ColumnMove);
		move_region(SkeletonMove, &SkeletonMove1, RowMove1, ColumnMove1);
		SkeletonClosedRegion(SkeletonMove, SkeletonMove1, &ObjectSelected);
		area_center(ObjectSelected, &Area2, & Row2, &Column2);
		if (1 != (Area2 > 0))
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		/*union2(SkeletonMove, SkeletonMove1, &SkeletonUnion);
		shape_trans (SkeletonUnion, &RegionUnion, "convex");
		dilation_rectangle1 (SkeletonMove1, &RegionDilation, 1, 2);
		difference (RegionUnion, RegionDilation, &RegionDifference);
		connection (RegionDifference, &ConnectedRegions);
		sort_region (ConnectedRegions, &SortedRegions, "last_point", "false", "row");
		count_obj (SortedRegions, &Number);
		if (Number[0].D() < 1)
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		select_obj (SortedRegions, &ObjectSelected, 1);*/
		opening_circle(ObjectSelected, &ObjectSelected, 10);
		intersection(ObjectSelected, Rectangle, &RegionDetect);
		m_vRgn[id] = RegionDetect ;	
		HTuple Area, CenterRow, CenterColumn;
		area_center(RegionDetect, &Area, &CenterRow, &CenterColumn);
		if (1 != (Area > 0))
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
STDMETHODIMP CAlPlasticCapAlgo::PlasticRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4047$\
					 167;B$\
					 4048;LP;-50;50;1;0$\
					 4049;LP;-200;200;1;0$\
					 4050;FP;0.0;200.0;0.5;20.0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*********************************************************************************
** 函数名：PlasticRegionLocationAlgo
** 功  能：铝塑盖的塑片区域定位算子
** 时  间：2019年12月13日
**********************************************************************************/
STDMETHODIMP CAlPlasticCapAlgo::ExpansionContourGenerationRegionAlgo(VARIANT* rgnId, VARIANT* pType, VARIANT* pRowMove, VARIANT* pDilationHeight, VARIANT* pWidthZoom)
{
	//轮廓膨胀得到区域
	int id					= (int)rgnId->fltVal;
	int Type				= (int)pType->fltVal;
	int RowMove				= (int)pRowMove->fltVal;
	float DilationHeight	= (float)pDilationHeight->fltVal;
	float WidthZoom			= (float)pWidthZoom->fltVal;

	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject SkeletonType;
	Type == 0?SkeletonType = m_TopEdge:SkeletonType = m_BottomEdge;
	HTuple Area0, CenterRow0, CenterColumn0;
	Hobject  RegionDetect, newRgn;
	area_center(SkeletonType, &Area0, &CenterRow0, &CenterColumn0);
	if (1 != (Area0 > 0))
	{
		m_vRgn[id] = newRgn ;
		return S_FALSE;
	}
	m_vRgn[id] = newRgn;
#ifdef NDEBUG
	try
	{
#endif
		//
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}

		Hobject Rectangle, SkeletonMove, RegionDilation0, RegionShrink, SkeletonShrink, Region;
		HTuple ColumnMove, iWidth, iHeight;
		get_image_size(m_crtImg, &iWidth, &iHeight);
		m_imgWidth  = (long)iWidth[0].I();
		m_imgHeight = (long)iHeight[0].I();
		gen_rectangle1(&Rectangle, 0, 0, iHeight-1, iWidth-1);
		RowMove = RowMove;
		ColumnMove = m_fReciprocal*RowMove;
		move_region(SkeletonType, &SkeletonMove, RowMove, ColumnMove);
		if (WidthZoom <= 0)
		{
			WidthZoom = -WidthZoom+1;
			dilation_rectangle1(SkeletonMove,  &RegionDilation0, 1, 20);
			erosion_rectangle1 (RegionDilation0, &RegionShrink, WidthZoom, 1);
			intersection(SkeletonMove, RegionShrink, &SkeletonShrink);
			dilation_rectangle1(SkeletonShrink,  &Region, 1, DilationHeight);
		}
		else
		{
			dilation_rectangle1(SkeletonMove,  &Region, WidthZoom, DilationHeight);
		}
		intersection(Region, Rectangle, &RegionDetect);
		m_vRgn[id] = RegionDetect ;	
		HTuple Area, CenterRow, CenterColumn;
		area_center(RegionDetect, &Area, &CenterRow, &CenterColumn);
		if (1 != (Area > 0))
		{
			gen_rectangle1(&m_vRgn[id], m_vPos[0].m_y-5.0, iWidth/4.0, m_vPos[0].m_y+5.0, iWidth*3.0/4.0);
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
STDMETHODIMP CAlPlasticCapAlgo::ExpansionContourGenerationRegionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4051$\
					 167;B$\
					 4052;LP;0;1;1;1$\
					 4053;LP;-200;200;1;0$\
					 4054;LP;1;100;1;10$\
					 4055;LP;-100;100;1;20"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*********************************************************************************
** 函数名：ExpansionContourGenerationRegionAlgo
** 功  能：通过膨胀轮廓线生成区域
** 时  间：2019年12月16日
**********************************************************************************/
void CAlPlasticCapAlgo::P_EdgePointsFitCurve(Hobject SearchReg, Hobject Image,HTuple PartInterval, HTuple LineLength, HTuple RemoveSize, HTuple * DefinedRow, HTuple *DefinedCol)
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
/*********************************************************************************
** 函数名：P_EdgePointsFitCurve
** 功  能：边缘点拟合曲线函数
** 时  间：2019年12月16日PlugAlgo移植
**********************************************************************************/
STDMETHODIMP CAlPlasticCapAlgo::StepEdgeDetectAlgo(VARIANT* rgnId, VARIANT* pSearchSize, VARIANT* pChannelSelect, VARIANT* pSmoothSize, VARIANT* pEdgeHeight)
{
	//检测台阶位置 计算台阶与下边缘的高度差
	int id					= (int)rgnId->fltVal;
	int SearchSize			= (int)pSearchSize->fltVal;
	int ChannelSelect		= (int)pChannelSelect->fltVal;
	int SmoothSize			= (int)pSmoothSize->fltVal;
	CComVariant retValue;
	m_vErrorRgn[id].Reset();
	m_vWhiteErrorRgn[id].Reset();
	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject rgn = m_vRgn[id];
	HTuple area, row, col;
	area_center(rgn,&area,&row,&col);
	if(rgn.Id() == H_EMPTY_REGION||area.Num()==0||area==0)
	{
		retValue = -1;
		retValue.Detach(pEdgeHeight);
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
		Hobject crtImg;
		Hobject ImageReduced, StepEdgePoints, Contour, SmoothedContours; 
		HTuple ChannelNum, DefinedRow, DefinedCol, Area1, Row1, Col1, EdgeArea, EdgeRow, EdgeCol;
		count_channels(m_crtImgMult,&ChannelNum);
		if ((ChannelNum[0].I()==3)&&(ChannelSelect != 0))
		{
			if(!H_ColorImageTransAll(m_crtImgMult,&crtImg, 0 , ChannelSelect))
			{
				retValue = -1;
				retValue.Detach(pEdgeHeight);
				return S_FALSE;
			}
		}
		else
		{
			crtImg = m_crtImg;
		}
		reduce_domain(crtImg, rgn, &ImageReduced);
		//PartInterval=3;SearchSize:=膨胀尺寸;RemoveSize=2
		P_EdgePointsFitCurve(rgn, ImageReduced, 3, SearchSize, 2, &DefinedRow, &DefinedCol);
		if (DefinedRow.Num()==0)
		{
			HTuple Area2, Row2, Col2;
			area_center(rgn, &Area2, &Row2, &Col2);
			retValue = Row2[0].D();
			retValue.Detach(pEdgeHeight);
			return S_OK;
		}
		gen_contour_polygon_xld (&Contour, DefinedRow, DefinedCol);
		area_center_points_xld (Contour, &Area1, &Row1, &Col1);
		/*if (Area1 < 1)
		{
			HTuple Area2, Row2, Col2;
			area_center(rgn, &Area2, &Row2, &Col2);
			retValue = Row2[0].D();
			retValue.Detach(pEdgeHeight);
			return S_OK;
		}*/
		SmoothSize = SmoothSize*2 + 1;
		smooth_contours_xld (Contour, &SmoothedContours, SmoothSize);   //SmoothSize=3,5,7,9....; 
		area_center_points_xld (SmoothedContours, &EdgeArea, &EdgeRow, &EdgeCol);
		m_StepCenterY = EdgeRow;
		float EdgeHeight = m_BottomCenterY[0].D() - m_StepCenterY[0].D();
		if (EdgeArea < 1)
		{
			retValue = -1;
			retValue.Detach(pEdgeHeight);
			return S_FALSE;
		}
		m_vRgnExTest[id] = SmoothedContours;
		retValue = fabs(EdgeHeight);
		retValue.Detach(pEdgeHeight);

#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		m_vErrorRgn[id].Reset();
		m_vWhiteErrorRgn[id].Reset();
		retValue.Detach(pEdgeHeight);
		return S_FALSE;
	}
#endif
	return S_OK;
}
STDMETHODIMP CAlPlasticCapAlgo::StepEdgeDetectAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4056$\
					 4059;R;LP;0;1000;1;>=#0$\
					 381;B$\
					 4057;LP;1;100;1;20$\
					 4060;LP;0;3;1;0$\
					 4058;LP;1;30;1;5");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*********************************************************************************
** 函数名：StepEdgeDetectAlgo
** 功  能：台阶边缘检测算子
** 时  间：2019年12月18日
**********************************************************************************/
STDMETHODIMP CAlPlasticCapAlgo::AlRegionLocationAlgo(VARIANT* rgnId, VARIANT* pUpRowMove, VARIANT* pDownRowMove, VARIANT* pWidthShrink)
{
	//得到铝片区域
	int id					= (int)rgnId->fltVal;
	int UpRowMove			= (int)pUpRowMove->fltVal;
	int DownRowMove			= (int)pDownRowMove->fltVal;
	float WidthShrink		= (float)pWidthShrink->fltVal;

	m_vRgnExTest[id].Reset();
	m_vRgnEx[id].Reset();
	m_vInterImgs[id].Reset();
	Hobject UpSkeleton = m_TopEdge;
	Hobject DownSkeleton = m_BottomEdge;
	Hobject  RegionDetect, newRgn;
	HTuple Area0, Row0, Col0, Area1, Row1, Col1;
	m_vRgn[id] = newRgn;
	area_center(UpSkeleton, &Area0, &Row0, &Col0);
	area_center(DownSkeleton, &Area1, &Row1, &Col1);
	if ((1 != (Area0 > 0))||(1 != (Area1 > 0)))
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

		Hobject Rectangle, DownDilation, DownShrink, ConnectionedRegions, DownSkeletonMove, RegionAffineTrans, ClosedRegion, RegionErosion;
		HTuple ColumnMove, RowMove1, UpWidth, DownWidth, DiffWidth, ColumnMove1, Phi, HomMat2D;
		HTuple  iWidth, iHeight, Area2, Row2, Col2, Area3, Row3, Col3;
		get_image_size(m_crtImg, &iWidth, &iHeight);
		m_imgWidth  = (long)iWidth[0].I();
		m_imgHeight = (long)iHeight[0].I();
		gen_rectangle1(&Rectangle, 0, 0, iHeight-1, iWidth-1);
		
		if ((m_TopEdgeCenterY.Num()==0)||(m_StepCenterY.Num()==0)||(m_BottomCenterY.Num()==0))
		{
			RowMove1 = -iHeight/4.0; 
		} 
		else
		{
			RowMove1 =  m_StepCenterY[0].D() - m_BottomCenterY[0].D();
		}
		region_features (m_TopEdge, "width", &UpWidth);
		region_features (m_BottomEdge, "width", &DownWidth);
		if ((DownWidth > UpWidth)||(UpWidth > 5))
		{
			DiffWidth = DownWidth - UpWidth;
			dilation_rectangle1(DownSkeleton,  &DownDilation, 1, 20);
			erosion_rectangle1 (DownDilation, &DownShrink, DiffWidth, 1);
			intersection(DownSkeleton, DownShrink, &DownSkeleton);
			connection(DownSkeleton, &ConnectionedRegions);
			select_shape_std (ConnectionedRegions, &DownSkeleton, "max_area", 70);
			area_center(DownSkeleton, &Area2, & Row2, &Col2);
			if (1 != (Area2 > 0))
			{
				m_vRgn[id] = newRgn;
				return S_FALSE;
			}
		}//将骨架宽度调整为一致
		RowMove1 = RowMove1 + DownRowMove;
		ColumnMove1 = m_fReciprocal * RowMove1;
		ColumnMove = m_fReciprocal * UpRowMove;
		move_region(DownSkeleton, &DownSkeletonMove, RowMove1, ColumnMove1);
		orientation_region (UpSkeleton, &Phi);
		vector_angle_to_rigid (Row0, Col0, Phi, Row0+UpRowMove, Col0+ColumnMove, Phi+3.14159, &HomMat2D);
		affine_trans_region(UpSkeleton, &RegionAffineTrans, HomMat2D, "nearest_neighbor");
		area_center(DownSkeletonMove, &Area3, & Row3, &Col3);
		if (1 != (Area3 > 0))
		{
			m_vRgn[id] = newRgn ;
			return S_FALSE;
		}
		SkeletonClosedRegion(RegionAffineTrans, DownSkeletonMove, &ClosedRegion);
		erosion_rectangle1 (ClosedRegion, &RegionErosion, WidthShrink, 1);
		opening_circle(RegionErosion, &RegionErosion, 10);
		intersection(RegionErosion, Rectangle, &RegionDetect);
		m_vRgn[id] = RegionDetect ;	
		HTuple Area, CenterRow, CenterColumn;
		area_center(RegionDetect, &Area, &CenterRow, &CenterColumn);
		if (1 != (Area > 0))
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
STDMETHODIMP CAlPlasticCapAlgo::AlRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("4061$\
					 167;B$\
					 4062;LP;-50;50;1;0$\
					 4048;LP;-50;50;1;0$\
					 4050;FP;0.0;200.0;0.5;20.0"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}
/*********************************************************************************
** 函数名：AlRegionLocationAlgo
** 功  能：铝塑盖的铝片区域定位算子
** 时  间：2019年12月25日
**********************************************************************************/
STDMETHODIMP CAlPlasticCapAlgo::CurlOptimizeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pFreqStart, VARIANT* pFreqCutoff,VARIANT* pWinType, VARIANT* pDefectType,VARIANT* pWaveThreshold,VARIANT* pMinArea, VARIANT* pDetectNumber)
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
STDMETHODIMP CAlPlasticCapAlgo::CurlOptimizeForColorAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::DynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::DynThresholdForColorAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::EdgeForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::EdgeForColorAlgoHelp(BSTR* pHelpStr)
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

STDMETHODIMP CAlPlasticCapAlgo::AnnularRotaDynThresholdForColorAlgo(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRotaAngle,VARIANT* pDynThresh, VARIANT* pDefectSize, VARIANT* pDetectArea)
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
STDMETHODIMP CAlPlasticCapAlgo::AnnularRotaDynThresholdForColorAlgoHelp(BSTR* pHelpStr)
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





STDMETHODIMP CAlPlasticCapAlgo::GetInterImgs(BYTE rgnID, LONG* pInterImgs)
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

void CAlPlasticCapAlgo::GrayValueContrast(int DetectType, HTuple Diff, int Threshold, HTuple* Indices)
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

void CAlPlasticCapAlgo::GenerateDefectRegions(HTuple RowsDefect, HTuple ColumnsDefect, int DefectSize, Hobject* SelectedEdges, HTuple* Area)
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




void CAlPlasticCapAlgo::my_disp_obj(const Halcon::Hobject Object)
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