#include "stdafx.h"
#include "resource.h"
#include "PetAlgo_i.h"
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
#include "PetAlgo.h"

bool CPetAlgo::IsDebugImg(Hobject& Image)
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


STDMETHODIMP CPetAlgo::SetCurrentImage(LONG* pImg)
{
	Hobject* pObj = (Hobject*)pImg;
	//write_image(*pObj, "tiff", 0, "F:\\22.tiff");
	m_crtImg = *pObj;
	m_bDebugImg = IsDebugImg(m_crtImg);
	return S_OK;
}


STDMETHODIMP CPetAlgo::GetRegion(BYTE rgnID, LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_vRgn[rgnID];
	return S_OK;
}


STDMETHODIMP CPetAlgo::GetErrorRegion(BYTE rgnID, LONG* pErrorRgn)
{
	Hobject* pObj = (Hobject*)pErrorRgn;
	*pObj = m_vErrorRgn[rgnID];
	return S_OK;
}


STDMETHODIMP CPetAlgo::GetSelfLearningRegion(LONG* pRgn)
{
	Hobject* pObj = (Hobject*)pRgn;
	*pObj = m_regLearning;
	return S_OK;
}


STDMETHODIMP CPetAlgo::ResetSelfLearningRegion(BSTR* strRegionExtractPara)
{
	return S_FALSE;
}

STDMETHODIMP CPetAlgo::GetInternalStats(BYTE rgnID, LONG nMaxLen, FLOAT* pStatsArray, LONG* nActualLen)
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

//模板生成相关函数
STDMETHODIMP CPetAlgo::SetSelfLearningParam(DWORD sampleSz, BSTR* bstrParam)
{
	m_nSampleSize = sampleSz;
	m_nImgIndex = 0;
	return S_OK;
}

STDMETHODIMP CPetAlgo::ResetSelfLearningProcedure(void)
{
	/*gen_empty_obj(&m_multiChannelModelImage);
	Model_Num = 0;
	Models = HTuple();*/
	return S_FALSE;
}


STDMETHODIMP CPetAlgo::SelectModelImage(LONG* pCrtImg, BYTE productID)
{
	if(productID == 6)
	{
		Hobject* pObj = (Hobject*)pCrtImg;
		m_crtImg = *pObj;
		Hobject crtImage = m_crtImg;
		m_nImgIndex ++;
#ifdef NDEBUG
		try
		{
#endif
			Hobject imgScaleMax, thredRegion, rgnFillUp, rgnOpening, connectedReg, selectedReg, rgnTrans, modelRect, modelImage, imageRect;
			HTuple rectRow1, rectRow2, rectColumn1, rectColumn2, ModelID;
			get_image_size (crtImage, &m_imgWidth, &m_imgHeight);
			gen_rectangle1(&imageRect, 0, 0, m_imgHeight, m_imgWidth);
			scale_image_max (crtImage, &imgScaleMax);
			threshold (imgScaleMax, &thredRegion, 0, 160);

			fill_up (thredRegion, &rgnFillUp);
			opening_rectangle1 (rgnFillUp, &rgnOpening, 9, 9);
			connection (rgnOpening, &connectedReg);
			select_shape (connectedReg, &selectedReg, "area", "and", 30000, 999999);
			shape_trans (selectedReg, &rgnTrans, "convex");
			smallest_rectangle1(rgnTrans, &rectRow1, &rectColumn1, &rectRow2, &rectColumn2);

			//取标签底部的部分区域来创建模板
			HTuple rectRow11, rectRow12, rectColumn11, rectColumn12;
			rectColumn11    = (rectColumn1 * 5)/6.0 + rectColumn2/6.0;
			rectColumn12    = rectColumn1/6.0 + (rectColumn2 * 5)/6.0;
			rectRow11       = (rectRow2 * 5)/8.0 + (rectRow1 * 3)/8.0;
			rectRow12       = (rectRow2 * 7)/8.0 + rectRow1/8.0;
			gen_rectangle1 (&modelRect, rectRow11, rectColumn11, rectRow12, rectColumn12);

			Hobject DupImage, regDifference;
			copy_image(crtImage, &DupImage);
			difference(imageRect, modelRect, &regDifference);
			overpaint_region(DupImage, regDifference, 0, "fill");
			if(m_nImgIndex == 1)
			{
				reduce_domain(crtImage, modelRect, &modelImage);
				create_shape_model(modelImage, "auto", HTuple(-30).Rad(), HTuple(30).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID);
				m_modelNum = 1;
				m_models[m_modelNum-1] = ModelID;
				channels_to_image(DupImage, &m_multiChannelModelImage);
			}
			else
			{
				reduce_domain(crtImage, modelRect, &modelImage);
				HTuple matchRow, matchColumn, matchAngle, matchScore, matchModel, ModelID_add;
				find_shape_models(modelImage, m_models, HTuple(-30).Rad(), HTuple(30).Rad(), 0.5, 1, 0, "least_squares", 0, 0.8, &matchRow, &matchColumn, &matchAngle, &matchScore, &matchModel);
				if (matchScore < 0.8)    //添加模板
				{
					append_channel(m_multiChannelModelImage, DupImage, &m_multiChannelModelImage);
					create_shape_model(modelImage, "auto", HTuple(-30).Rad(), HTuple(30).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID_add);
					m_modelNum += 1;
					m_models[m_modelNum-1] = ModelID_add;
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
	return S_OK;
}


STDMETHODIMP CPetAlgo::DeleteModelImage(BYTE nIndex, BYTE productID)
{
	return S_FALSE;
}


STDMETHODIMP CPetAlgo::SaveResult(BSTR* strModelName, BYTE productID)
{
	if(productID == 6)
	{
#ifdef NDEBUG
		try
		{
#endif
			CString m_modelParaPath, m_strModelName, m_modelParaPath1, m_modelParaPath2, m_modelParaPath3;
			wstring m_path;
			string m_path_str;
			m_strModelName = *strModelName;
			HTuple  Channels, i, Result_Path;
			m_modelParaPath1 = (m_strModelName + _T("\\"));                //保存多通道图像
			m_modelParaPath2 = (m_modelParaPath1 + _T("Model\\"));         //保存模板
			m_modelParaPath3 = (m_modelParaPath1 + _T("ModelImage\\"));    //保存模板对应的多通道图像

			for (i = 1; i <= m_modelNum; i += 1)
			{
				Result_Path = ("model_"+(i.ToString(".3")))+".shm";
				string m_name = Result_Path[0].S();
				m_modelParaPath = m_modelParaPath2;
				CString sName = CA2T(m_name.c_str());
				m_modelParaPath.Append(sName);

				if(CreateDirectory(m_modelParaPath2, NULL))
				{
					CreateDirectory(m_modelParaPath2, NULL);
				}
				m_path_str = CT2A(m_modelParaPath.GetString());
				write_shape_model(HTuple(m_models[i-1]),m_path_str.c_str());
			}
			if(m_modelNum >= 1)       // 保存模板时，添加模板数量判断。
			{
				m_modelParaPath.Format(_T("%s%s"),m_modelParaPath1,_T("ModelImages.tiff"));
				m_path_str = CT2A(m_modelParaPath.GetString());
				write_image(m_multiChannelModelImage, "tiff", 0, m_path_str.c_str());
				Hobject modelImage;
				for(i = 1; i <= m_modelNum; i += 1)
				{
					access_channel(m_multiChannelModelImage, &modelImage, i);
					Result_Path = ("modelImage_"+(i.ToString(".3")))+".tiff";
					string m_name = Result_Path[0].S();
					m_modelParaPath = m_modelParaPath3;
					CString sName = CA2T(m_name.c_str());
					m_modelParaPath.Append(sName);
					if(CreateDirectory(m_modelParaPath3, NULL))
					{
						CreateDirectory(m_modelParaPath3, NULL);
					}
					m_path_str = CT2A(m_modelParaPath.GetString());
					write_image(modelImage, "tiff", 0, m_path_str.c_str());
				}
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

STDMETHODIMP CPetAlgo::AddModelImage(LONG* pImg, BSTR* strFolder, BYTE productID)
{
	if(productID == 6 )
	{
#ifdef NDEBUG
		try
		{
#endif
			Hobject* pObj = (Hobject*)pImg;			
			Hobject crtImage =  *pObj;
			count_channels(m_multiChannelModelImage, &m_modelNum);
			Hobject imgRect;
			gen_rectangle1(&imgRect, 0, 0, m_imgHeight, m_imgWidth);

			CString m_modelParaPath, m_modelParaPath1, m_modelParaPath2, m_modelParaPath3, m_strFileName;
			string m_path_str;
			m_strFileName = *strFolder;
			m_modelParaPath1.Format(_T("%s%s"),m_strFileName,_T("\\"));
			m_modelParaPath2.Format(_T("%s%s"),m_modelParaPath1,_T("Model\\"));
			m_modelParaPath3.Format(_T("%s%s"),m_modelParaPath1,_T("ModelImage\\"));

			Hobject imgScaleMax, thredRegion, rgnFillUp, rgnOpening, connectedReg, selectedReg, rgnTrans, modelRect;			
			HTuple  tranArea, tranRow, tranCol;
			
			scale_image_max (crtImage, &imgScaleMax);
			threshold (imgScaleMax, &thredRegion, 0, 160);
			fill_up (thredRegion, &rgnFillUp);
			opening_rectangle1 (rgnFillUp, &rgnOpening, 9, 9);
			connection (rgnOpening, &connectedReg);
			select_shape (connectedReg, &selectedReg, "area", "and", 30000, 999999);
			shape_trans (selectedReg, &rgnTrans, "convex");
			area_center(rgnTrans, &tranArea, &tranRow, &tranCol);
			if(tranArea > 0)
			{
				//取标签底部的部分区域来创建模板
				HTuple rectRow1, rectColumn1, rectRow2, rectColumn2, rectRow11, rectRow12, rectColumn11, rectColumn12;
				smallest_rectangle1 (rgnTrans, &rectRow1, &rectColumn1, &rectRow2, &rectColumn2);
				rectColumn11    = (rectColumn1 * 5)/6.0 + rectColumn2/6.0;
				rectColumn12    = rectColumn1/6.0 + (rectColumn2 * 5)/6.0;
				rectRow11       = (rectRow2 * 5)/8.0 + (rectRow1 * 3)/8.0;
				rectRow12       = (rectRow2 * 7)/8.0 + rectRow1/8.0;
				gen_rectangle1 (&modelRect, rectRow11, rectColumn11, rectRow12, rectColumn12);
			}
			
			Hobject dupImage, detectRegion, detectImage, modelRegion, modelImage, regDiff;
			HTuple ModelID, modelRow, modelColumn, modelAngle, modelScore;
			reduce_domain(crtImage, modelRect, &modelImage);
			create_shape_model(modelImage, "auto", HTuple(-30).Rad(), HTuple(30).Rad(), "auto", "auto", "use_polarity", "auto", "auto", &ModelID);
			find_shape_model(crtImage, ModelID, 0, 0.5, 0.5, 1, 0.5, "least_squares", 0, 0.9, &modelRow, &modelColumn, &modelAngle, &modelScore);	
			if(modelScore > 0.5)
			{
				copy_image(crtImage, &dupImage);
				difference(imgRect, modelRect, &regDiff);
				overpaint_region(dupImage, regDiff, 0, "fill");
				HTuple Class;
				get_obj_class(m_multiChannelModelImage, &Class);
				if(Class != HTuple("image"))
				{
					m_modelNum = 0;
					channels_to_image(dupImage, &m_multiChannelModelImage);
				}
				else
				{
					append_channel(m_multiChannelModelImage, dupImage, &m_multiChannelModelImage);
				}
				m_modelNum = m_modelNum + 1;
				m_models[m_modelNum - 1] = ModelID;
				//重新保存模板相关数据
				if(CreateDirectory(m_modelParaPath2, NULL))
				{
					CreateDirectory(m_modelParaPath2, NULL);
				}
				if(CreateDirectory(m_modelParaPath3, NULL))
				{
					CreateDirectory(m_modelParaPath3, NULL);
				}
				HTuple Result_Path = ("model_"+(m_modelNum.ToString(".3")))+".shm";
				string m_name = Result_Path[0].S();
				m_modelParaPath = m_modelParaPath2;
				CString sName = CA2T(m_name.c_str());
				m_modelParaPath.Append(sName);
				m_path_str = CT2A(m_modelParaPath.GetString());
				write_shape_model(ModelID, m_path_str.c_str());

				HTuple Result_Path1 = ("modelImage_"+(m_modelNum.ToString(".3")))+".tiff";
				string m_name1 = Result_Path1[0].S();
				m_modelParaPath = m_modelParaPath3;
				CString sName1 = CA2T(m_name1.c_str());
				m_modelParaPath.Append(sName1);
				m_path_str = CT2A(m_modelParaPath.GetString());
				write_image(dupImage, "tiff", 0, m_path_str.c_str());
			}
			if(m_modelNum >= 1)
			{
				m_modelParaPath.Format(_T("%s%s"), m_modelParaPath1, _T("ModelImages.tiff"));
				m_path_str = CT2A(m_modelParaPath.GetString());
				write_image(m_multiChannelModelImage, "tiff", 0, m_path_str.c_str());
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


STDMETHODIMP CPetAlgo::GetSelfLearningResult(LONG* multiModelImag, long* cMax, float* vImgScore, BYTE productID)
{
	if(productID == 6)
	{
#ifdef NDEBUG
		try
		{
#endif
			if(m_modelNum < 1)
				return S_FALSE;
			//替换训练结果
#ifdef NDEBUG
		}
		catch (HException &except) 
		{
			m_algoSpy.QueueUpLogMsg(except, m_crtImg);
			return S_FALSE;
		}
#endif
	}
	Hobject* pObj = (Hobject*)multiModelImag;
	*pObj = m_multiChannelModelImage;

	*cMax = (long)(m_modelNum[0].I());//m_vImgScore.size();
	for(size_t t = 0; t < m_modelNum[0].I(); t ++)
	{
		vImgScore[t] = 85.0;
	}
	return S_OK;
}


STDMETHODIMP CPetAlgo::CapLocationAlgo(VARIANT* rgnID, VARIANT* pGrayThred, VARIANT* pAreaThred,VARIANT* pAreaThredHigh)
{
	int rId = (int)rgnID->fltVal;
	float grayThred = (float)pGrayThred->fltVal;
	float areaThred = (float)pAreaThred->fltVal;
	float areaThredHigh=(float)pAreaThredHigh->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;

	HTuple imgWidth, imgHeight, Row1, Column1, Phi, Length1, Length2, Area, Row, Column;
	Hobject Regions, rgnOpening, ConnectedRegions, SelectedRegions, RegionFillUp, RegionTrans, Rectangle;
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImage = m_crtImg;
		get_image_size (crtImage, &imgWidth, &imgHeight);
		threshold(crtImage, &Regions, 0, grayThred);
		opening_circle(Regions, &rgnOpening, 9.5);
		connection(rgnOpening, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", areaThred, areaThredHigh);
		fill_up(SelectedRegions, &RegionFillUp);
		shape_trans (RegionFillUp, &RegionTrans, "convex");
		smallest_rectangle2 (RegionTrans, &Row1, &Column1, &Phi, &Length1, &Length2);
		area_center(RegionTrans, &Area, &Row, &Column);
		m_capRgn = RegionTrans;
		if(Area > 0)
		{
			gen_rectangle2 (&Rectangle, Row1, Column1, Phi, Length1, Length2);
			m_vRgn[rId]   = Rectangle;
			m_vPos[0].m_y = (float)Row1[0].D();
			m_vPos[0].m_x = (float)Column1[0].D();
		}
		else
		{
			Hobject errorRect;
			gen_rectangle1(&errorRect, 1, 1, 20, 20);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[rId] = errorRect;
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


STDMETHODIMP CPetAlgo::CapLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("461$\
					 381;B$\
					 463;LP;0;255;1;60$\
					 462;LP;1000;9999;1;5000$\
					 518;LP;1000;99999;1;32237 ");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::LevelLocationAlgo(VARIANT* rgnId, VARIANT* pLevelRef, VARIANT* pLevelRange, VARIANT* pLevelWidth)
{
	//得到液位区域
	int rId           = (int)rgnId->fltVal;
	float levelRange  = pLevelRange->fltVal;
	float levelWidth  = pLevelWidth->fltVal;
    m_levelRef          = pLevelRef->fltVal;

	float capRow      = 0.0;
	float capColumn   = 0.0;
	capColumn         = m_vPos[0].m_x;
	capRow            = m_vPos[0].m_y;
	
	Hobject newRgn;
	m_vRgn[rId] = newRgn;

	if((capRow < 2.0)||(capColumn < 2.0))
	{
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
	Hobject  levelRect, levelRefReg, levelUnion;
	HTuple   imgWidth, imgHeight;
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImage = m_crtImg;
		get_image_size(crtImage, &imgWidth, &imgHeight);
		gen_rectangle1(&levelRect, m_levelRef-levelRange, capColumn-levelWidth, m_levelRef+levelRange, capColumn+levelWidth);
		gen_region_line(&levelRefReg, m_levelRef, 0, m_levelRef, imgWidth);
		union2(levelRect, levelRefReg, &levelUnion);
		m_vRgn[rId] = levelUnion;
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


STDMETHODIMP CPetAlgo::LevelLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("465$\
					 381;B$\
					 471;LP;0;1000;1;183$\
					 466;FP;0;500;0.5;30$\
					 467;FP;0;500;0.5;40");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::LevelDetectionAlgo(VARIANT* rgnId, VARIANT* pDynThr,VARIANT* pAreaLimit, VARIANT* pGrayThred, VARIANT* pAreaThred)
{
	int rId           = (int)rgnId->fltVal;
	float areaLimit   = pAreaLimit->fltVal;
	double grayThred  = (double)pGrayThred->fltVal;
	float DynThr=pDynThr->fltVal;

	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pAreaThred);
		return S_FALSE;
	}
	
	Hobject  levelRect, levelImg, levelImgMean, levelRegDyn, levelConnectedReg, levelSelectedReg, levelRegUnion;
	Hobject  levelImgReduced, levelRegions, levelRegTrans;
	HTuple   levelArea, levelRow, levelColumn;
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImage = m_crtImg;
		opening_circle(rgn, &levelRect, 2.5);
		reduce_domain(crtImage, levelRect, &levelImg);
		mean_image(levelImg, &levelImgMean, 10, 10);
		dyn_threshold(levelImg, levelImgMean, &levelRegDyn, DynThr, "dark");
		connection(levelRegDyn, &levelConnectedReg);
		select_shape (levelConnectedReg, &levelSelectedReg, "area", "and", areaLimit, 99999);
		union1(levelSelectedReg, &levelRegUnion);
		reduce_domain(crtImage, levelRegUnion, &levelImgReduced);
		threshold (levelImgReduced, &levelRegions, 0.0, grayThred);
		area_center(levelRegions, &levelArea, &levelRow, &levelColumn);
		if(levelArea > 3)     //液位正常
		{		
			retValue = levelArea[0].I();
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = 0;
			dilation_circle(levelRect, &levelRect, 1.5);
			m_vErrorRgn[rId] = levelRect;	
		}		
		retValue.Detach(pAreaThred);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		retValue.Detach(pAreaThred);
		return S_FALSE;
	}
#endif
	return S_OK;
}


STDMETHODIMP CPetAlgo::LevelDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("468$\
					 470;R;LP;0;1000;1$\
					 381;B$\
					 519;FP;1;60;1;10$\
					 469;FP;0;5000;1;200$\
					 463;FP;0;255;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;

	return S_OK;
}


STDMETHODIMP CPetAlgo::CapSkewOrHighLocationAlgo(VARIANT* rgnId, VARIANT* pCapLevelDiff, VARIANT* pCapRectDiff, VARIANT* pCapRectHeight, VARIANT* pCapRectWidth)
{
	int rId              = (int)rgnId->fltVal;
	m_capLevelDiff       = (double)pCapLevelDiff->fltVal;
	m_capRectDiff   = (double)pCapRectDiff->fltVal;
	double capRectHeight = (double)pCapRectHeight->fltVal;
	double capRectWidth  = (double)pCapRectWidth->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;

	float capRow      = 0.0;
	float capColumn   = 0.0;
	capColumn         = m_vPos[0].m_x;
	capRow            = m_vPos[0].m_y;
	if((capRow < 2.0)||(capColumn < 2.0))
	{
		Hobject newRgn;
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
	Hobject  capRect1, capRect2, capRect, capCross1, capCross2, crossRegion1, crossRegion2, crossRegion;
	HTuple   rowRect11, rowRect12, colRect11, colRect12, colRect21, colRect22;
	HTuple   crossRow1, crossRow2, crossCol1, crossCol2;
#ifdef NDEBUG
	try
	{
#endif
		rowRect11 = m_levelRef - m_capLevelDiff - capRectHeight/2.0;
		rowRect12 = m_levelRef - m_capLevelDiff + capRectHeight/2.0;
		colRect11 = capColumn - m_capRectDiff;
		colRect12 = capColumn - m_capRectDiff + capRectWidth;
		colRect21 = capColumn + m_capRectDiff - capRectWidth;
		colRect22 = capColumn + m_capRectDiff;
		gen_cross_contour_xld (&capCross1, m_levelRef - m_capLevelDiff, capColumn - m_capRectDiff + capRectWidth/2.0, 2, 0);
		get_contour_xld (capCross1, &crossRow1, &crossCol1);
		gen_region_points (&crossRegion1, crossRow1, crossCol1);

		gen_cross_contour_xld (&capCross2, m_levelRef - m_capLevelDiff, capColumn + m_capRectDiff - capRectWidth/2.0, 2, 0);
		get_contour_xld (capCross2, &crossRow2, &crossCol2);
		gen_region_points (&crossRegion2, crossRow2, crossCol2);
		union2(crossRegion1, crossRegion2, &crossRegion);
		if((rowRect11 > 0)&&(rowRect12 > 0)&&(colRect11 > 0)&&(colRect12 > 0)&&(colRect21 > 0))
		{
			gen_rectangle1(&capRect1, rowRect11, colRect11, rowRect12, colRect12);
			gen_rectangle1(&capRect2, rowRect11, colRect21, rowRect12, colRect22);
			union2(capRect1, capRect2, &capRect);
			difference(capRect, crossRegion, &capRect);
			m_vRgn[rId] = capRect;
			m_capRect   = capRect;
		}
		else
		{
			Hobject newRgn;
			m_vRgn[rId] = newRgn;
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


STDMETHODIMP CPetAlgo::CapSkewOrHighLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("472$\
					 381;B$\
					 473;FP;0;500;1;130$\
					 474;FP;0;500;1;40$\
					 476;FP;0;500;1;50$\
					 475;FP;0;500;1;10");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::CapSkewDetectionAlgo(VARIANT* rgnId, VARIANT* pHeightDiff)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}

	Hobject  rectConnected, capRect1, capRect2, capRectImg1, capRectImg2, capRectImgMean1, capRectDynThresh1, capRectConnected1;
	Hobject  fillReg, capRectSelected1, capRectImgMean2, capRectDynThresh2, capRectConnected2, capRectSelected2;
	HTuple   rectArea1, rectRow1, rectColumn1, rectArea2, rectRow2, rectColumn2;
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImage = m_crtImg;
		connection(rgn, &rectConnected);
		fill_up(rectConnected, &fillReg);
		select_obj(fillReg, &capRect1, 1);
		reduce_domain(crtImage, capRect1, &capRectImg1);
		mean_image(capRectImg1, &capRectImgMean1, 3, 3);
		dyn_threshold(capRectImg1, capRectImgMean1, &capRectDynThresh1, 5, "light");
		connection(capRectDynThresh1, &capRectConnected1);
		select_shape_std(capRectConnected1, &capRectSelected1, "max_area", 0);
		area_center(capRectSelected1, &rectArea1, &rectRow1, &rectColumn1);

		select_obj(rectConnected, &capRect2, 2);
		reduce_domain(crtImage, capRect2, &capRectImg2);
		mean_image(capRectImg2, &capRectImgMean2, 3, 3);
		dyn_threshold(capRectImg2, capRectImgMean2, &capRectDynThresh2, 5, "light");
		connection(capRectDynThresh2, &capRectConnected2);
		select_shape_std(capRectConnected2, &capRectSelected2, "max_area", 0);
		area_center(capRectSelected2, &rectArea2, &rectRow2, &rectColumn2);
		double heightDiff = abs(rectRow2[0].D() - rectRow1[0].D());
		retValue = heightDiff;
		Hobject capRect;
		dilation_circle(rgn, &capRect, 1.5);
		m_vErrorRgn[rId] = capRect;		
		retValue.Detach(pHeightDiff);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}


STDMETHODIMP CPetAlgo::CapSkewDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("477$\
					 470;R;FP$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;

	return S_OK;
}


STDMETHODIMP CPetAlgo::CapHighDetectionAlgo(VARIANT* rgnId, VARIANT* pHeightDiff)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}

	Hobject  rectConnected, capRect1, capRect2, capRectImg1, capRectImg2, capRectImgMean1, capRectDynThresh1, capRectConnected1;
	Hobject  capRectSelected1, capRectImgMean2, capRectDynThresh2, capRectConnected2, capRectSelected2;
	HTuple   rectArea1, rectRow1, rectColumn1, rectArea2, rectRow2, rectColumn2;
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImage = m_crtImg;
		connection(rgn, &rectConnected);
		select_obj(rectConnected, &capRect1, 1);
		reduce_domain(crtImage, capRect1, &capRectImg1);
		mean_image(capRectImg1, &capRectImgMean1, 3, 3);
		dyn_threshold(capRectImg1, capRectImgMean1, &capRectDynThresh1, 5, "light");
		connection(capRectDynThresh1, &capRectConnected1);
		select_shape_std(capRectConnected1, &capRectSelected1, "max_area", 0);
		area_center(capRectSelected1, &rectArea1, &rectRow1, &rectColumn1);

		select_obj(rectConnected, &capRect2, 2);
		reduce_domain(crtImage, capRect2, &capRectImg2);
		mean_image(capRectImg2, &capRectImgMean2, 3, 3);
		dyn_threshold(capRectImg2, capRectImgMean2, &capRectDynThresh2, 5, "light");
		connection(capRectDynThresh2, &capRectConnected2);
		select_shape_std(capRectConnected2, &capRectSelected2, "max_area", 0);
		area_center(capRectSelected2, &rectArea2, &rectRow2, &rectColumn2);
		double heightDiff = abs(m_levelRef-m_capLevelDiff-(rectRow2[0].D()+rectRow2[0].D())/2.0);
		retValue = heightDiff;
		Hobject capRect;
		dilation_circle(rgn, &capRect, 1.5);
		m_vErrorRgn[rId] = capRect;		
		retValue.Detach(pHeightDiff);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}


STDMETHODIMP CPetAlgo::CapHighDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("490$\
					 470;R;FP$\
					 381;B");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::BottleNeckLocationAlgo(VARIANT* rgnId,VARIANT* pLevelNeckDiff, VARIANT* pNeckWidth, VARIANT* pNeckHeight)
{
	int rId               = (int)rgnId->fltVal;
	double levelNeckDiff  = (double)pLevelNeckDiff->fltVal;
	double neckWidth      = (double)pNeckWidth->fltVal;
	double neckHeight     = (double)pNeckHeight->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;

	float capRow      = 0.0;
	float capColumn   = 0.0;
	capColumn         = m_vPos[0].m_x;
	capRow            = m_vPos[0].m_y;
	if((capRow < 2.0)||(capColumn < 2.0))
	{
		Hobject newRgn;
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject  neckRect;
		Hobject  rectConnected, capRect1, capRect2, capRectImg1, capRectImg2, capRectImgMean1, capRectDynThresh1, capRectConnected1;
		Hobject  capRectSelected1, capRectImgMean2, capRectDynThresh2, capRectConnected2, capRectSelected2;
		Hobject  capRect;
		HTuple   rowRect11, rowRect12, colRect11, colRect12, colRect21, colRect22, RowDifference, ColumnDifference;
		HTuple   rectArea1, rectRow1, rectColumn1, rectArea2, rectRow2, rectColumn2, phi, Row11, Rectangle5, neckRow11;
		Hobject  crtImage = m_crtImg;
		rowRect11 = m_levelRef - m_capLevelDiff - 25;
		rowRect12 = m_levelRef - m_capLevelDiff + 25;
		colRect11 = capColumn - m_capRectDiff;
		colRect12 = capColumn - m_capRectDiff + 10;
		colRect21 = capColumn + m_capRectDiff - 10;
		colRect22 = capColumn + m_capRectDiff;
		
		gen_rectangle1(&capRect1, rowRect11, colRect11, rowRect12, colRect12);
		gen_rectangle1(&capRect2, rowRect11, colRect21, rowRect12, colRect22);

		reduce_domain(crtImage, capRect1, &capRectImg1);
		mean_image(capRectImg1, &capRectImgMean1, 3, 3);
		dyn_threshold(capRectImg1, capRectImgMean1, &capRectDynThresh1, 5, "light");
		connection(capRectDynThresh1, &capRectConnected1);
		select_shape_std(capRectConnected1, &capRectSelected1, "max_area", 0);
		area_center(capRectSelected1, &rectArea1, &rectRow1, &rectColumn1);

		reduce_domain(crtImage, capRect2, &capRectImg2);
		mean_image(capRectImg2, &capRectImgMean2, 3, 3);
		dyn_threshold(capRectImg2, capRectImgMean2, &capRectDynThresh2, 5, "light");
		connection(capRectDynThresh2, &capRectConnected2);
		select_shape_std(capRectConnected2, &capRectSelected2, "max_area", 0);
		area_center(capRectSelected2, &rectArea2, &rectRow2, &rectColumn2);

		RowDifference = rectRow1 - rectRow2;
		ColumnDifference = rectColumn1-rectColumn2;
		tuple_atan2(ColumnDifference, RowDifference, &phi);

		Row11 = (rectRow1 + rectRow2)/2;		
		neckRow11 = Row11 + levelNeckDiff;
		if((neckRow11 > 0))
		{
			gen_rectangle2(&neckRect, neckRow11, capColumn, phi, neckHeight, neckWidth);
			m_vRgn[rId] = neckRect;
		}
		else
		{
			Hobject newRgn;
			m_vRgn[rId] = newRgn;
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


STDMETHODIMP CPetAlgo::BottleNeckLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("483$\
					 381;B$\
					 484;FP;0;500;1;66$\
					 485;FP;0;500;1;90$\
					 486;FP;0;500;1;9");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::BottleNeckDetectionAlgo(VARIANT* rgnId, VARIANT* pGrayThred, VARIANT* pAreaLimit, VARIANT* pDefectArea)
{
	int rId = (int)rgnId->fltVal;
	int grayThred = (int)pGrayThred->fltVal;
	double areaLimit = pAreaLimit->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject imgNeck, ImageAffinTrans1, ImageAffinTrans2, imgNeckLeft, imgNeckRight, neckRegion;
		HTuple neckArea, neckRow, neckColumn, HomMat2D1, HomMat2D2, defectArea, defectRow, defectColumn;
		Hobject rgnAffineTrans1, rgnAffineTrans2, defectRegs, neckRegion1, neckRegion2, dynThredReg;
		Hobject crtImage = m_crtImg;
		reduce_domain(crtImage, rgn, &imgNeck);
		area_center (rgn, &neckArea, &neckRow, &neckColumn);
		vector_angle_to_rigid (neckRow, neckColumn, 0, neckRow, neckColumn-5, 0, &HomMat2D1); //左移
		affine_trans_region (rgn, &rgnAffineTrans1, HomMat2D1, "false");
		affine_trans_image (crtImage, &ImageAffinTrans1, HomMat2D1, "constant", "false");

		vector_angle_to_rigid (neckRow, neckColumn, 0, neckRow, neckColumn+5, 0, &HomMat2D2);
		affine_trans_region (rgn, &rgnAffineTrans2, HomMat2D2, "false");
		affine_trans_image (crtImage, &ImageAffinTrans2, HomMat2D2, "constant", "false");

		intersection (rgnAffineTrans1, rgnAffineTrans2, &neckRegion);
		reduce_domain(ImageAffinTrans1, neckRegion, &imgNeckLeft);
		reduce_domain(ImageAffinTrans2, neckRegion, &imgNeckRight);
		dyn_threshold (imgNeckLeft, imgNeckRight, &dynThredReg, grayThred, "light");
		connection(dynThredReg, &dynThredReg);
		select_shape(dynThredReg, &defectRegs, "area", "and", areaLimit, 99999);
		union1(defectRegs, &defectRegs);
		area_center (defectRegs, &defectArea, &defectRow, &defectColumn);
		if(defectArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = defectArea[0].I();
			dilation_circle (defectRegs, &defectRegs, 2.5);
			m_vErrorRgn[rId] = defectRegs;	
		}		
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif	
	return S_OK;
}


STDMETHODIMP CPetAlgo::BottleNeckDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("487$\
					 470;R;FP;0;5000;1$\
					 381;B$\
					 488;LP;0;255;1;100$\
					 489;FP;0;3000;1;30");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;

	return S_OK;
}


STDMETHODIMP CPetAlgo::SecurityRingDetectionAlgo(VARIANT* rgnId, VARIANT* pGrayThred, VARIANT* pAreaLimit, VARIANT* pDefectArea)
{
	int rId = (int)rgnId->fltVal;
	int grayThred = (int)pGrayThred->fltVal;
	int areaLimit = (int)pAreaLimit->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject ringImg, ringThredreg, dynThredReg, defectRegs;
		HTuple defectArea, defectRow, defectColumn;
		Hobject crtImage = m_crtImg;
		reduce_domain(crtImage, rgn, &ringImg);
		threshold(ringImg, &ringThredreg, grayThred, 255);
		connection(ringThredreg, &dynThredReg);
		select_shape(dynThredReg, &defectRegs, "area", "and", areaLimit, 99999);
		union1(defectRegs, &defectRegs);
		area_center (defectRegs, &defectArea, &defectRow, &defectColumn);
		if(defectArea <= 0)
		{
			retValue = 0.0;
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = defectArea[0].I();
			dilation_circle (defectRegs, &defectRegs, 2.5);
			m_vErrorRgn[rId] = defectRegs;	
		}		
		retValue.Detach(pDefectArea);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		retValue.Detach(pDefectArea);
		return S_FALSE;
	}
#endif
	return S_OK;
}


STDMETHODIMP CPetAlgo::SecurityRingDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("487$\
					 470;R;FP;0;5000;1$\
					 381;B$\
					 488;LP;0;255;1;100$\
					 491;LP;0;3000;1;20");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;

	return S_OK;
}


STDMETHODIMP CPetAlgo::LabelLocationAlgo(VARIANT* rgnId, VARIANT* pGrayThred, VARIANT* pSmoothFactor, VARIANT* pAreaThred)
{
	int rId = (int)rgnId->fltVal;
	int grayThred = (int)pGrayThred->fltVal;
	long areaThred = (long)pAreaThred->fltVal;
	int smoothFactor = (int)pSmoothFactor->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
#ifdef NDEBUG
	try
	{
#endif
		Hobject imgScaleMax, thredRegion, rgnFillUp, rgnOpening, connectedReg, selectedReg, rgnTrans;
		HTuple Area, Row, Column;
		Hobject crtImage = m_crtImg;
		get_image_size(crtImage, &m_imgWidth, &m_imgHeight);
		scale_image_max (crtImage, &imgScaleMax);
		threshold (imgScaleMax, &thredRegion, 0, grayThred);

		fill_up (thredRegion, &rgnFillUp);
		opening_rectangle1 (rgnFillUp, &rgnOpening, smoothFactor, smoothFactor);
		connection (rgnOpening, &connectedReg);
		select_shape (connectedReg, &selectedReg, "area", "and", areaThred, 999999);
		shape_trans (selectedReg, &rgnTrans, "convex");
		area_center (rgnTrans, &Area, &Row, &Column);
		if(Area > 0)
		{
			m_vRgn[rId] = rgnTrans;
			m_vPos[0].m_y = (float)Row[0].D();
			m_vPos[0].m_x = (float)Column[0].D();
			m_labelRgn    = m_vRgn[rId];
		}
		else             //without label
		{
			Hobject errorRect;
			gen_rectangle1(&errorRect, 1, 1, 20, 20);
			m_vPos[0].m_x = 0.0;
			m_vPos[0].m_y = 0.0;
			m_vRgn[rId] = errorRect;
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


STDMETHODIMP CPetAlgo::LabelLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("497$\
					 381;B$\
					 463;LP;0;255;1;160$\
					 314;LP;1;200;1;9$\
					 510;LP;1000;999999;1;50000");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::InverseLocationAlgo(VARIANT* rgnId, VARIANT* pRectHeight, VARIANT* pRectWidth)
{
	//进行模板匹配的区域定位
	int rId = (int)rgnId->fltVal;
	float rectHeight = pRectHeight->fltVal;
	float rectWidth = pRectWidth->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	float labelRow      = m_vPos[0].m_y;
	float labelColumn   = m_vPos[0].m_x;
	if((labelRow < 2.0)||(labelColumn < 2.0))
	{
		Hobject newRgn;
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		HTuple rectRow1, rectColumn1, rectRow2, rectColumn2, rowRect11, rowRect12, colRect21, colRect22;
		Hobject detectRgn;
		smallest_rectangle1(m_labelRgn, &rectRow1, &rectColumn1, &rectRow2, &rectColumn2);
		rowRect11 = (rectRow1 + rectRow2 * 3)/4.0 - rectHeight/2;
		rowRect12 = (rectRow1 + rectRow2 * 3)/4.0 + rectHeight/2;
		colRect21 = labelColumn - rectWidth/2;
		colRect22 = labelColumn + rectWidth/2;
		if(rowRect11 > 0 && rowRect12 < m_imgHeight && colRect21 > 0 && colRect22 <m_imgWidth)
		{
			gen_rectangle1(&detectRgn, rowRect11, colRect21, rowRect12, colRect22);
			m_vRgn[rId] = detectRgn;
		}
		else
		{
			Hobject newRgn;
			m_vRgn[rId] = newRgn;
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


STDMETHODIMP CPetAlgo::InverseLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("498$\
					 381;B$\
					 499;FP;1;500;1;100$\
					 500;FP;1;500;1;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::InverseDetectionAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pMinScore)
{
	int rId          = (int)rgnId->fltVal;
	float greediness = pGreediness->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pMinScore);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject detectImg, inverseRect;
		HTuple matchRow, matchColumn, matchAngle, matchScore, matchModel;
		Hobject crtImage = m_crtImg;
		reduce_domain(m_crtImg, rgn, &detectImg);
		find_shape_models (detectImg, m_models, HTuple(-15).Rad(), HTuple(15).Rad(), 0.4, 1, 0, "least_squares", 0, greediness, &matchRow, &matchColumn, &matchAngle, &matchScore, &matchModel);
		if(matchScore < 0.05)
		{
			matchScore = 0.05;
		}
		retValue = matchScore[0].D();
		dilation_circle(rgn, &inverseRect, 1.5);
		m_vErrorRgn[rId] = inverseRect;		
		retValue.Detach(pMinScore);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		retValue.Detach(pMinScore);
		return S_FALSE;
	}
#endif	
	return S_OK;
}


STDMETHODIMP CPetAlgo::InverseDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("502$\
					 440;R;FP;0.0;1.0;0.1$\
					 167;B$\
					 322;FP;0;1.0;0.05;0.9");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::HighOrLowLabelLocationAlgo(VARIANT* rgnId, VARIANT* pNormalLevel)
{
	int rId          = (int)rgnId->fltVal;
	int normalLevel  = (int)pNormalLevel->fltVal;
	m_labelRefLevel  = normalLevel;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	float labelRow      = m_vPos[0].m_y;
	float labelColumn   = m_vPos[0].m_x;
	if((labelRow < 2.0)||(labelColumn < 2.0))
	{
		Hobject newRgn;
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject levelRefReg, regUnion;
		gen_region_line(&levelRefReg, normalLevel, 0, normalLevel, m_imgWidth);
		union2(levelRefReg, m_labelRgn, &regUnion);
		m_vRgn[rId] = regUnion;
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


STDMETHODIMP CPetAlgo::HighOrLowLabelLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("503$\
					 381;B$\
					 504;FP;1.0;1000.0;0.5;300");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::HighOrLowLabelDetectionAlgo(VARIANT* rgnId, VARIANT* pHeightDiff)
{
	int rId          = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}
#ifdef NDEBUG
	try
	{
#endif
		Hobject imgLabel, levelRefReg, RegionUnion;
		HTuple labelRow, labelColumn, labelArea, heightDiff;
		opening_circle(rgn, &rgn, 1.5);	
		area_center (rgn, &labelArea, &labelRow, &labelColumn);
		heightDiff = (labelRow - m_labelRefLevel).Abs();
		retValue = heightDiff[0].I();
		gen_region_line(&levelRefReg, m_labelRefLevel, 0, m_labelRefLevel, m_imgWidth);
		m_vErrorRgn[rId] = levelRefReg;
		retValue.Detach(pHeightDiff);
#ifdef NDEBUG
	}
	catch (HException &except)  
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue=-1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}


STDMETHODIMP CPetAlgo::HighOrLowLabelDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("507$\
					 470;R;FP$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::SlantLabelLocationAlgo(VARIANT* rgnId, VARIANT* pRectHeight, VARIANT* pRectWidth, VARIANT* pCenterDiff, VARIANT* pLabelRectDiff)
{
	int rId                = (int)rgnId->fltVal;
	float rectHeight      = pRectHeight->fltVal;
	float rectWidth       = pRectWidth->fltVal;
	float centerDiff      = pCenterDiff->fltVal;
	float labelRectDiff   = pLabelRectDiff->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	float labelRow      = m_vPos[0].m_y;
	float labelColumn   = m_vPos[0].m_x;
	if((labelRow < 2.0)||(labelColumn < 2.0))
	{
		Hobject newRgn;
		m_vRgn[rId] = newRgn ;
		return S_FALSE;
	}
	Hobject  labelRect1, labelRect2, labelRect;
	HTuple   rowRect11, rowRect12, colRect11, colRect12, colRect21, colRect22;
#ifdef NDEBUG
	try
	{
#endif
		rowRect11 = m_labelRefLevel - centerDiff - rectHeight/2.0;
		rowRect12 = m_labelRefLevel - centerDiff + rectHeight/2.0;
		colRect11 = labelColumn - labelRectDiff - rectWidth/2.0;
		colRect12 = labelColumn - labelRectDiff + rectWidth/2.0;
		colRect21 = labelColumn + labelRectDiff - rectWidth/2.0;
		colRect22 = labelColumn + labelRectDiff + rectWidth/2.0;
		if((rowRect11 > 0)&&(rowRect12 > 0)&&(colRect11 > 0)&&(colRect12 > 0)&&(colRect21 < m_imgWidth)&&(colRect22 < m_imgWidth))
		{
			gen_rectangle1(&labelRect1, rowRect11, colRect11, rowRect12, colRect12);
			gen_rectangle1(&labelRect2, rowRect11, colRect21, rowRect12, colRect22);
			union2(labelRect1, labelRect2, &labelRect);
			m_vRgn[rId] = labelRect;
		}
		else
		{
			Hobject newRgn;
			m_vRgn[rId] = newRgn;
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


STDMETHODIMP CPetAlgo::SlantLabelLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("505$\
					 381;B$\
					 476;FP;1;1000;0.5;30$\
					 475;FP;1;1000;0.5;100$\
					 509;FP;1;1000;0.5;230$\
					 508;FP;1;1000;0.5;100");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::SlantLabelDetectionAlgo(VARIANT* rgnId, VARIANT* pHeightDiff)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}
	Hobject  rectConnected, capRect1, capRect2, labelRgn, capRectConnected1, rgnDiff;
	Hobject  capRectSelected1, capRectImgMean2, capRectDynThresh2, capRectConnected2, capRectSelected2, labelRect;
	HTuple   rectArea1, rectRow1, rectColumn1, rectArea2, rectRow2, rectColumn2,Area,Row,Column;
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImage = m_crtImg;
		dilation_rectangle1(m_labelRgn, &labelRgn, 3, 3);
		difference(labelRgn, m_labelRgn, &rgnDiff);

		connection(rgn, &rectConnected);
		select_obj(rectConnected, &capRect1, 1);
		intersection(rgnDiff, capRect1, &capRectSelected1);
		area_center(capRectSelected1, &rectArea1, &rectRow1, &rectColumn1);

		select_obj(rectConnected, &capRect2, 2);
		intersection(rgnDiff, capRect2, &capRectSelected2);
		area_center(capRectSelected2, &rectArea2, &rectRow2, &rectColumn2);
		double HeightDiff = abs(rectRow2[0].D() - rectRow1[0].D());
		retValue = HeightDiff;
		dilation_circle(rgn, &labelRect, 1.5);
		m_vErrorRgn[rId] = labelRect;
		retValue.Detach(pHeightDiff);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}


STDMETHODIMP CPetAlgo::SlantLabelDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("506$\
					 470;R;FP$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}



STDMETHODIMP CPetAlgo::TotalBottlesLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("533$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::TotalBottlesLocationAlgo(VARIANT* rgnId)
{
	int rId = (int)rgnId->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	return S_OK;
}


STDMETHODIMP CPetAlgo::BottlesLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("512$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::BottlesLocationAlgo(VARIANT* rgnId)
{
	int rId = (int)rgnId->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	#ifdef NDEBUG
	try
	{
#endif
		HTuple Width,Height;
		Hobject labelRect1,labelRect,crtImage;
		crtImage=m_crtImg;
		get_image_size(crtImage,&Width,&Height);
		gen_rectangle1(&labelRect1, 1, 1,Height, Width);
		m_vRgn[rId] = labelRect1;
		
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



STDMETHODIMP CPetAlgo::BottlesDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("516$\
					 517;R;LP$\
					 381;B$\
					 517;LP;0;255;1;69$\
					 518;LP;0;10000;1;1932");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::BottlesDetectionAlgo(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pAreaSelect, VARIANT* pNumber)
{
	int rId = (int)rgnId->fltVal;
	int Threshold=(int)pThreshold->fltVal;
	int AreaSelect=(int)pAreaSelect->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION) 
	{
		retValue = -1;
		retValue.Detach(pNumber);
		return S_FALSE;
	}

	Hobject  Regions,ConnectedRegions,SelectedRegions,Rectangle,ImageReduced;
	HTuple Number,Area,Row,Column;
#ifdef NDEBUG
	try
	{
#endif
		Hobject crtImage = m_crtImg;
		reduce_domain(crtImage,rgn,&ImageReduced);
		threshold(ImageReduced,&Regions,Threshold,255);
		area_center(Regions,&Area,&Row,&Column);
		connection(Regions,&ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", AreaSelect, 8728);
		count_obj(SelectedRegions, &Number);
		if(0 != (Number>0))
		{
			retValue = Number[0].I();
			Hobject newRgn;
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = 0;
			gen_rectangle1(&Rectangle,1,1,5,5);
			m_vErrorRgn[rId] =Rectangle;
		}
		retValue.Detach(pNumber);
#ifdef NDEBUG
	}
	catch (HException &except) 
	{
		m_algoSpy.QueueUpLogMsg(except, m_crtImg);
		retValue = -1;
		retValue.Detach(pHeightDiff);
		return S_FALSE;
	}
#endif
	return S_OK;
}


STDMETHODIMP CPetAlgo::ModelCreate(double Radius, HTuple * ModelID1)
{
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

STDMETHODIMP CPetAlgo::PetCenterLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("372$\
					 381;B$\
					 374;LP;50;2000;1;206$\
					 377;FP;0.5;1.0;0.1;0.80");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::PetCenterLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pMinScore)
{
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
	Hobject  ImageReduced, ModelAtNewPosition, Region, Circle1;
#ifdef NDEBUG
	try
	{
#endif
	
		if(m_bDebugImg)
		{
			m_dictSpecialData.clear();
		}
		Hobject Image = m_crtImg;
		get_image_size (Image, &Width, &Height);
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
		get_shape_model_contours (&ShapeModel,m_modelId, 1);
		find_scaled_shape_model (Image,m_modelId, 0, 0, 0.95, 1.05, MinScore, 1, 0.5, "least_squares", (HTuple(5).Append(-2)), 0.7, &CenterRow, &CenterColumn, &Angle, &Scale, &Score);
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


STDMETHODIMP CPetAlgo::PetCircleRegionLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("307$\
					 167;B$\
					 194;FP;1.0;2000.0;0.5;200.00$\
					 413;FP;1.5;2000;0.5;20.00"); 

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::PetCircleRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth)
{
	int id = (int)rgnId->fltVal;
	m_ring[id].m_Radius = (float)pRadius->fltVal;
	m_ring[id].m_RoiWidth = (float)pRoiWidth->fltVal;
	float RoiWidth = m_ring[id].m_RoiWidth;
	float Radius = m_ring[id].m_Radius;
	float m_RadiusExt = Radius - RoiWidth;

	float m_center_x = 0.0;
	float m_center_y = 0.0;
	m_center_x = m_vPos[0].m_x;
	m_center_y = m_vPos[0].m_y;
	
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
		if(m_bDebugImg)
		{
			m_dictSpecialData[id].clear();
		}
		if (RoiWidth<0)
		{
			gen_circle(&Circle2_EXT, m_center_y, m_center_x, m_RadiusExt);
			gen_circle(&Circle1_Inner, m_center_y, m_center_x, Radius);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		}
		else if (RoiWidth==0)
		{
			gen_circle(&Circle2_EXT, m_center_y, m_center_x, Radius+1);
			gen_circle(&Circle1_Inner, m_center_y, m_center_x, m_RadiusExt);
			difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
		}
		else
		{
			if(m_RadiusExt>0)
			{
				gen_circle(&Circle2_EXT, m_center_y, m_center_x, Radius);
				gen_circle(&Circle1_Inner, m_center_y, m_center_x, m_RadiusExt);
				difference(Circle2_EXT,Circle1_Inner,&RegionDetect);
			}
			else
			{
				gen_circle(&RegionDetect, m_center_y, m_center_x, Radius);
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


STDMETHODIMP CPetAlgo::PetCircleRegionDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("492$\
					 397;R;LP;0;1000;1$\
					 381;B$\
					 414;LP;1;250;2;30$\
					 411;LP;1;200;1;30");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::PetCircleRegionDetectionAlgo(VARIANT* rgnId, VARIANT* pDynThreshold, VARIANT* pSelectArea, VARIANT* pDetectArea)
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

	int DynThreshold = (int)pDynThreshold->fltVal;
	float SelectArea = (float)pSelectArea->fltVal;

		Hobject Image = m_crtImg;
	    HTuple  HomMat2D,Area,Row,Column;
		Hobject SearchImage,ImageAffinTrans,RegionDynThresh,ConnectedRegions,SelectedRegions,RegionDilation,ImageReduced,unionRegion;
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}

		vector_angle_to_rigid(m_vPos[0].m_y, m_vPos[0].m_x, 0, m_vPos[0].m_y,m_vPos[0].m_x, HTuple(5).Rad(), &HomMat2D);
		affine_trans_image(Image, &ImageAffinTrans, HomMat2D, "constant", "false");
		reduce_domain(Image,rgn,&ImageReduced);
		dyn_threshold(ImageAffinTrans, ImageReduced, &RegionDynThresh, DynThreshold, "light");
		connection(RegionDynThresh, &ConnectedRegions);
		select_shape(ConnectedRegions, &SelectedRegions, "area", "and", SelectArea, 64000);
		union1(SelectedRegions,&unionRegion);
		area_center(unionRegion,&Area,&Row,&Column);
		if(Area <= 0)
		{
			retValue = 0.0;
			Hobject newRgn; 
			m_vErrorRgn[rId] = newRgn;
		}
		else
		{
			retValue = Area[0].I();
			dilation_circle(SelectedRegions, &RegionDilation, 3);
			m_vErrorRgn[rId] = RegionDilation;
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


STDMETHODIMP CPetAlgo::CodeLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("532$\
					 381;B$\
					 522;LP;0;255;1;100$\
					 523;LP;0;100;1;30$\
					 524;LP;0;100;1;8$\
					 525;LP;200;100000;10;1000");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::CodeLocationAlgo(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pWidth, VARIANT* pHeight, VARIANT* pArea)
{
	int id = (int)rgnId->fltVal;
	int Threshold = (int)pThreshold->fltVal;
	int Width = (int)pWidth->fltVal;
	int Height = (int)pHeight->fltVal;
	int Area = (int)pArea->fltVal;

	Hobject newRgn;
	m_vRgn[id] = newRgn;
	
	Hobject  Image, Regions, RegionDilation2, ConnectedRegions3,SelectedRegions,Rectangle2,Union;
	HTuple  Row12, Column12, Row22, Column22, Number1,Areas,Row,Column;
#ifdef NDEBUG
	try
	{
#endif
	Image = m_crtImg;
	threshold(Image, &Regions, 0, Threshold);
	dilation_rectangle1(Regions, &RegionDilation2, Width, Height);
	connection(RegionDilation2, &ConnectedRegions3);
	select_shape(ConnectedRegions3, &SelectedRegions, "area", "and", Area, 460056);
	smallest_rectangle1(SelectedRegions, &Row12, &Column12, &Row22, &Column22);
	gen_rectangle1(&Rectangle2,Row12, Column12, Row22,Column22);
	union1(Rectangle2,&Union);
	area_center(Union,&Areas,&Row,&Column);
	if(Area>0)
	{
		m_vPos[0].m_x = (float)Column[0].D();
		m_vPos[0].m_y =(float) Row[0].D();
		m_vRgn[id] = Union;
		m_Rect=m_vRgn[id];
	}
	else
	{
		m_vPos[0].m_x =0.0;
		m_vPos[0].m_y =0.0;
		Hobject newRgn;
		m_vRgn[id] = newRgn;
	}
	return S_FALSE;
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


STDMETHODIMP CPetAlgo::CodingLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("526$\
					 381;B$\
					 527;LP;0;255;1;180$\
					 528;LP;0;10;1;2$\
					 525;LP;0;30000;10;100$\
					 529;LP;0;100;1;30$\
					 530;LP;0;100;1;60");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::CodingLocationAlgo(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pDilationCircle,VARIANT* pSelect, VARIANT* pWidth, VARIANT* pHeight)
{
	int rId = (int)rgnId->fltVal;
	int Threshold = (int)pThreshold->fltVal;
	int DilationCircle = (int)pDilationCircle->fltVal;
	int Select = (int)pSelect->fltVal;
	int Width = (int)pWidth->fltVal;
	int Height = (int)pHeight->fltVal;

	m_vRgn[rId] = m_Rect;
	
	Hobject Image = m_crtImg;

	Hobject  ConnectedRegion;
	Hobject  ObjectSelected1, Rectangle, ImageReduced1;
	Hobject  Regions1, RegionDilation1, ConnectedRegions1, SelectedRegions1;
	Hobject  RegionTrans, Partitioned, RegionIntersection, RegionClosing;
	Hobject  EmptyObject, ObjectSelected, ConnectedRegions2;
	Hobject  SelectedRegions2, RegionUnion, SortedRegions,Emptyobject1;

    HTuple  Row12, Column12, Row22, Column22, Number1;
	HTuple   i, Row1, Column1, Row2, Column2;
	HTuple  OCRHandle, Class, Confidence;
#ifdef NDEBUG
	try
	{
#endif
	connection(m_vRgn[rId],&ConnectedRegion);
	count_obj(ConnectedRegion, &Number1);
	gen_empty_obj(&Emptyobject1);
	for (i=1; i<=Number1; i+=1)
	{
	    select_obj(ConnectedRegion, &ObjectSelected1, i);

		smallest_rectangle1(ObjectSelected1, &Row1, &Column1, &Row2, &Column2);
		gen_rectangle1(&Rectangle, Row1, Column1, Row2, Column2);
		reduce_domain(Image, Rectangle, &ImageReduced1);

		threshold(ImageReduced1, &Regions1, 0, Threshold);
		dilation_circle(Regions1, &RegionDilation1, DilationCircle);
		connection(RegionDilation1, &ConnectedRegions1);
		select_shape(ConnectedRegions1, &SelectedRegions1, "area", "and", Select, 310900);
		shape_trans(SelectedRegions1, &RegionTrans, "rectangle1");
		partition_rectangle(RegionTrans, &Partitioned, Width, Height);
		intersection(Partitioned, Regions1, &RegionIntersection);
		closing_circle(RegionIntersection, &RegionClosing, 3);
		concat_obj(Emptyobject1,Partitioned,&Emptyobject1);
	  }
		m_vRgn[rId] = Emptyobject1;
	return S_FALSE;
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


STDMETHODIMP CPetAlgo::CodingDetectionAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("526$\
					 531;R;LP;0;1000;1$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::CodingDetectionAlgo(VARIANT* rgnId, VARIANT* pNumber)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pNumber);
		return S_FALSE;
	}


	Hobject Image = m_crtImg;

	Hobject  ConnectedRegion,Rectangle;

    HTuple  Row12, Column12, Row22, Column22, Number1;
	HTuple   Number;
#ifdef NDEBUG
	try
	{
#endif

	connection(m_vRgn[rId],&ConnectedRegion);
	count_obj(ConnectedRegion, &Number);
  
	 if(0 != (Number>0))
	 {
		retValue =Number[0].I();
		Hobject newRgn;
		m_vErrorRgn[rId] = newRgn;
	  }
	  else
	  {
		retValue = 0;
		gen_rectangle1(&Rectangle,1,1,5,5);
		m_vErrorRgn[rId] =Rectangle;
	 }
		retValue.Detach(pNumber);
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

STDMETHODIMP CPetAlgo::KunRunShanLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("533$\
					 381;B");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::KunRunShanLocationAlgo(VARIANT* rgnId)
{
	int rId = (int)rgnId->fltVal;
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	return S_OK;
}


STDMETHODIMP CPetAlgo::PetMatchingLocationAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("533$\
					 381;B$\
					 561;FP;1;1000;1;100$\
					 562;FP;1;1000;1;100$\
					 563;FP;1;1000;1;300$\
					 564;FP;1;3000;1;300");
	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}


STDMETHODIMP CPetAlgo::PetMatchingLocationAlgo(VARIANT* rgnId,VARIANT* pRow1, VARIANT* pColumn1, VARIANT* pRow2, VARIANT* pColumn2)
{
	int rId = (int)rgnId->fltVal;
	float Row1Length  = pRow1->fltVal;
	float Column1Length  = pColumn1->fltVal;
	float Row2Length  = pRow2->fltVal;
	float Column2Length  = pColumn2->fltVal;
	Hobject Rectangle;
	gen_rectangle1(&Rectangle, Row1Length,Column1Length, Row2Length, Column2Length);
	m_vRgn[rId] = Rectangle;
	return S_OK;
}


STDMETHODIMP CPetAlgo::PetMatchingParaAlgoHelp(BSTR* pHelpStr)
{
	CComBSTR strHelp("559$\
					 440;R;FP;0.0;1.0;0.01$\
					 167;B$\
					 322;FP;0.2;1.0;0.05;0.85$\
					 441;LP;1;2;1;1$\
					 535;SPM$\
					 535;SPM");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPetAlgo::PetMatchingParaAlgo(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult)
{
	int rId = (int)rgnId->fltVal;
	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	float greediness = (float)pGreediness->fltVal;
	int ringOrOCR = (int)pRingOrOCR->fltVal;

		if(ringOrOCR==1)
		{
			CString sModelPath1  = *bstrShm1File;
			string strModelPath1 = CT2A(sModelPath1);
			if (!_access(strModelPath1.c_str(),0))
			{
				read_shape_model(strModelPath1.c_str(), &m_ringModelID);
			}
			else
			{
				if(m_fileReadPathRight)
				{
					MessageBox(NULL,_T("请检查模板初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					m_fileReadPathRight=false;
				}
				return S_FALSE;
			}
		}

	//
	Hobject ImageReduced,Circle;
	HTuple Row_M,Column_M,Angle_M,Score_M,Model_M;
	m_modelMatchingPARA[0]=0.0;
	CComVariant retValue;
	//if(rgn.Id() == H_EMPTY_REGION)
	//{
	//	retValue = -1;
	//	m_vErrorRgn[rId].Reset();
	//	retValue.Detach(pMatchingResult);
	//	m_modelMatchingPARA[0]=0.0;
	//	m_modelMatchingPARA[1]=0.0;
	//	m_modelMatchingPARA[2]=0.0;
	//	m_modelMatchingPARA[3]=0.0;
	//	m_modelMatchingPARA[4]=0.0;
	//	m_modelMatchingPARA[5]=0.0;
	//	m_modelMatchingPARA[6]=0.0;
	//	return S_FALSE;
	//}
#ifdef NDEBUG
	try
	{
#endif
		if(m_bDebugImg)
		{
			m_dictSpecialData[rId].clear();
		}
		bool b_isOK;
		Hobject Image=m_crtImg;
		b_isOK=true;
		//
		Circle=rgn;
		reduce_domain(Image, Circle, &ImageReduced);

		if(ringOrOCR==1)
		{
			HTuple dataLength1;
			tuple_length (m_ringModelID, &dataLength1);
			if(dataLength1<1)
			{
				if(m_fileReadPathRight)
				{
					MessageBox(NULL,_T("请检查模板初始化路径是否正确."),_T("模板读取错误："), MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL );
					m_fileReadPathRight=false;
				}
				return S_FALSE;
			}
			//
			find_shape_model(ImageReduced,m_ringModelID, HTuple(0).Rad(), HTuple(360).Rad(), 0.5, 1,
				0.5, "least_squares",0,greediness, &Row_M, &Column_M, &Angle_M, &Score_M);

			if (0 != (Score_M>0.3))
			{
				Score = (float)Score_M[0].D();
				m_modelMatchingPARA[1]=Row_M;
				m_modelMatchingPARA[2]=Column_M;
				m_modelMatchingPARA[3]=Angle_M;
				m_modelMatchingPARA[4]=0.0;
				m_modelMatchingPARA[5]=0.0;
				m_modelMatchingPARA[6]=0.0;
				retValue =Score_M[0].D();
			}
			else
			{
				b_isOK=false;
			}
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
			retValue =0.0;	
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

STDMETHODIMP CPetAlgo::KunRunShanRegionLocationAlgoHelp(BSTR* pHelpStr)
{
		CComBSTR strHelp("560$\
					 167;B$\
					 561;FP;1;500;1;300$\
					 562;FP;1;500;1;300$\
					 563;FP;1;500;1;300$\
					 564;FP;1;500;1;300");

	HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPetAlgo::KunRunShanRegionLocationAlgo(VARIANT* rgnId, VARIANT* pRow1Length, VARIANT* pColumn1Length, VARIANT* pRow2Length, VARIANT* pColumn2Length)
{
	int rId           = (int)rgnId->fltVal;
	float Row1Length  = pRow1Length->fltVal;
	float Column1Length  = pColumn1Length->fltVal;
	float Row2Length  = pRow2Length->fltVal;
	float Column2Length  = pColumn2Length->fltVal;
	
	Hobject newRgn;
	m_vRgn[rId] = newRgn;
	Hobject Image = m_crtImg;
	Hobject Rectangle2;
#ifdef NDEBUG
	try
	{
#endif
		if(Score>0.3)
		{
			gen_rectangle1(&Rectangle2, m_modelMatchingPARA[1].D()-Row1Length,m_modelMatchingPARA[2].D()-Column1Length, m_modelMatchingPARA[1].D()+Row2Length, m_modelMatchingPARA[2].D()-Column2Length);
			m_vRgn[rId] =Rectangle2;
		}
		else
		{
			Hobject newRgn;
			m_vRgn[rId] = newRgn;
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

STDMETHODIMP CPetAlgo::PetMatchingDetectionAlgoHelp(BSTR* pHelpStr)
{
CComBSTR strHelp("565$\
					 566;R;FP;0;100;21$\
					 167;B$\
					567;LP;0;255;1;27$\
					568;LP;1;10;1;2$\
					569;LP;0;10000;10;90");
HRESULT hr = strHelp.CopyTo(pHelpStr);
	if(FAILED(hr))
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CPetAlgo::PetMatchingDetectionAlgo(VARIANT* rgnId, VARIANT* pThreshold, VARIANT* pMask, VARIANT* pSelectMin, VARIANT* pNumber)
{
	int rId = (int)rgnId->fltVal;
	int Threshold=(int)pThreshold->fltVal;
	int Mask=(int)pMask->fltVal;
	int SelectMin=(int)pSelectMin->fltVal;

	m_vErrorRgn[rId].Reset();
	Hobject rgn = m_vRgn[rId];
	CComVariant retValue;
	if(rgn.Id() == H_EMPTY_REGION)
	{
		retValue = -1;
		m_vErrorRgn[rId].Reset();
		retValue.Detach(pNumber);
		return S_FALSE;
	}


    Hobject  ImageReduced2, EdgeAmplitude1, RegionDilation1;
    Hobject  ConnectedRegions1, SelectedRegions, Regions,Rectangle;
	HTuple Number;
#ifdef NDEBUG
	try
	{
#endif
	 Hobject Image = m_crtImg;
	  reduce_domain(Image, rgn, &ImageReduced2);
      sobel_amp(ImageReduced2, &EdgeAmplitude1, "sum_abs", 3);
      threshold(EdgeAmplitude1, &Regions, Threshold, 255);
      dilation_circle(Regions, &RegionDilation1, Mask);
      connection(RegionDilation1, &ConnectedRegions1);

      select_shape(ConnectedRegions1, &SelectedRegions, "area", "and", SelectMin, 808600);
      count_obj(SelectedRegions, &Number);
  
	 if (0 != (Number==21))
	 {
		retValue =Number[0].I();
		Hobject newRgn;
		m_vErrorRgn[rId] = newRgn;
	  }
	  else
	  {
		retValue = 0;
		gen_rectangle1(&Rectangle,1,1,5,5);
		m_vErrorRgn[rId] =Rectangle;
	 }
		retValue.Detach(pNumber);
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

STDMETHODIMP CPetAlgo::ReloadExternalFile(BSTR* bstrPath)
{
	return S_FALSE;
}

STDMETHODIMP CPetAlgo::SetCurrentTaskName(BSTR* bstrTaskName)
{
	//TODO
	return S_OK;
}

STDMETHODIMP CPetAlgo::SetSelfLearningTargetTaskName(BSTR* bstrTaskName)
{
	// TODO
	return S_OK;
}