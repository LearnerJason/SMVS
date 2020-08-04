#include "StdAfx.h"
#include "DisplayingManager.h"
#include "ConfigurationManager.h"
#include "ChannelControlPanel.h"
#include "SystemManager.h"
#include "Logger.h"
#include <limits>

extern ConfigManager  gConfigMgr;
extern CLogger        gLogger;
extern CSystemManager gSystemMgr;

CDisplayingManager::CDisplayingManager(void)
	:PENDING_BUFFER_SZ(8),
	m_nExitFlag(0),
	m_bEnableDisplay(true),
	m_nArrayCount(0)
{
	QueryPerformanceFrequency(&m_freq);
	QueryPerformanceCounter(&m_timestamp);
}


CDisplayingManager::~CDisplayingManager(void)
{
	for(auto it = m_dictCameraImgBuffer.begin(); it != m_dictCameraImgBuffer.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	for(auto it = m_dictNgImgBuffer.begin(); it != m_dictNgImgBuffer.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	for(auto it = m_dictNgImgDispBuffer.begin(); it != m_dictNgImgDispBuffer.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_dictCameraImgBuffer.clear();
	m_dictNgImgBuffer.clear();
	m_dictNgImgDispBuffer.clear();
	m_dictSaveImgBuffer.clear();
	m_dictSaveImgDispBuffer.clear();
}

void CDisplayingManager::CreateImageBuffer()
{
	auto ptrConfig = gConfigMgr.GetChannelConfigPtr();
	for(auto it = ptrConfig->begin(); it != ptrConfig->end(); it++)
	{
		m_dictCameraImgBuffer[it->first] = new bounded_buffer<Hobject>(PENDING_BUFFER_SZ);
		m_dictNgImgBuffer[it->first] = new bounded_buffer<IMG_PRODUCT>(10000);
		m_dictNgImgDispBuffer[it->first] = new bounded_buffer<DisplayResult>(10000);
		m_dictSaveImgBuffer[it->first] = new bounded_buffer<IMG_PRODUCT>(10000);
		m_dictSaveImgDispBuffer[it->first] = new bounded_buffer<DisplayResult>(10000);
		m_nArrayCount++;
	}
}

void CDisplayingManager::SetImageSize(int nChIndex, IMG_SZ sz)
{
	m_dictChImgSz[nChIndex] = sz;
}

void CDisplayingManager::RegisterRealTimeImgWindowID(int nChIndex, LONGLONG wndID)
{
	m_dictCameraWndID[nChIndex] = wndID;
}

void CDisplayingManager::ReceiveRealTimeImage(int nChIndex, Hobject img)
{
	if(m_bEnableDisplay)
	{
		m_dictCameraImgBuffer[nChIndex]->push_back(img);
	}
}

void CDisplayingManager::ReceiveNgImage(int nChIndex,IMG_PRODUCT img_product)
{
	if(m_bEnableDisplay)
	{
		img_product.m_ImageTimeCount=0;
		m_dictNgImgBuffer[nChIndex]->push_back(img_product);
	}
	m_dictSaveImgBuffer[nChIndex]->push_back(img_product);
}

void CDisplayingManager::ReceiveNgImage(int nChIndex, std::vector<IMG_PRODUCT> v_img,std::vector<DisplayResult> v_imgDispRst)
{
	if(m_bEnableDisplay && gSystemMgr.GetPanel(nChIndex)->GetErrorImageStayTime()>0)
	{
		for(auto it=v_img.begin();it!=v_img.end();it++)
		{
			it->m_ImageTimeCount=0;
			if(m_dictNgImgBuffer[nChIndex]->is_not_full())
				m_dictNgImgBuffer[nChIndex]->push_back(*it);
		}
		for(auto it=v_imgDispRst.begin();it!=v_imgDispRst.end();it++)
		{			
			if(m_dictNgImgDispBuffer[nChIndex]->is_not_full())
				m_dictNgImgDispBuffer[nChIndex]->push_back(*it);
		}		
	}
	else
	{
		while(m_dictNgImgBuffer[nChIndex]->is_not_empty())
		{
			IMG_PRODUCT NgImg;
			m_dictNgImgBuffer[nChIndex]->pop_back(&NgImg);
		}
		while(m_dictNgImgDispBuffer[nChIndex]->is_not_empty())
		{
			DisplayResult NgDispRst;
			m_dictNgImgDispBuffer[nChIndex]->pop_back(&NgDispRst);
		}
	}
	if(gSystemMgr.GetPanel(nChIndex)->IsSaveNgImge())
	{
		for(auto it=v_img.begin();it!=v_img.end();it++)
		{
			if(m_dictSaveImgBuffer[nChIndex]->is_not_full())
				m_dictSaveImgBuffer[nChIndex]->push_back(*it);
		}
		for(auto it=v_imgDispRst.begin();it!=v_imgDispRst.end();it++)
		{
			if(m_dictSaveImgDispBuffer[nChIndex]->is_not_full())
				m_dictSaveImgDispBuffer[nChIndex]->push_back(*it);
		}
	}
}

bool CDisplayingManager::IsAnyNgImage(int& nChIndex)
{
	for(size_t t = 1; t <= m_nArrayCount;t++)
	{
		if(m_dictNgImgBuffer[t]->is_not_empty())
		{
			nChIndex = t;
			return true;
		}
	}
	return false;
}

void CDisplayingManager::StartImageDisplayThread()
{
	Proc.MemProc = &CDisplayingManager::ImageDisplayhreadFunc;
	UINT threadId;
	HANDLE hDbThread = (HANDLE)_beginthreadex(NULL, 0, Proc.ThreadProc, (LPVOID)this, 0, &threadId);
	if(hDbThread == NULL)
	{
		AfxMessageBox(_T("Starting Image Boost thread failed"), MB_OK);
	}
	//::SetThreadPriority(hDbThread, THREAD_PRIORITY_NORMAL);
}

bool CDisplayingManager::IsAnyImage(int& nChIndex)
{
	for(size_t t = 1; t <= m_nArrayCount;t++)
	{
		if(m_dictCameraImgBuffer[t]->is_not_empty())
		{
			nChIndex = t;
			return true;
		}
	}
	return false;
}

unsigned int __stdcall CDisplayingManager::ImageDisplayhreadFunc()
{
	bool bIsAnyImg = false;
	int nChIndex    = -1;
	while(m_nExitFlag == 0)
	{
		for(nChIndex = 1; nChIndex <= m_nArrayCount;nChIndex++)
			if(m_dictSaveImgBuffer[nChIndex]->is_not_empty())
				SaveImage(nChIndex);
		if(!m_bEnableDisplay)
		{
			for(size_t t = 1; t <= m_nArrayCount;t++)
			{
				while(m_dictCameraImgBuffer[t]->is_not_empty())
				{
					Hobject retImg;
					m_dictCameraImgBuffer[t]->pop_back(&retImg);
				}
			}
			Sleep(1);
			continue;
		}

		for( nChIndex=1;nChIndex<=m_nArrayCount;nChIndex++)
		{
			if(m_dictNgImgBuffer[nChIndex]->is_not_empty() && gSystemMgr.GetPanel(nChIndex)->GetErrorImageStayTime()>0)
			{
				if(gSystemMgr.GetPanel(nChIndex)->GetChannelState()!=CH_ON)
				{
					while(m_dictNgImgBuffer[nChIndex]->is_not_empty())
					{
						IMG_PRODUCT NgImg;
						m_dictNgImgBuffer[nChIndex]->pop_back(&NgImg);
					}
					while(m_dictNgImgDispBuffer[nChIndex]->is_not_empty())
					{
						DisplayResult NgDispRst;
						m_dictNgImgDispBuffer[nChIndex]->pop_back(&NgDispRst);
					}
				}
				else
					DispNgImage(nChIndex);
			}
			else
			{
				LONGLONG wndID = m_dictCameraWndID[nChIndex];
				if(m_dictCameraImgBuffer[nChIndex]->is_not_empty())
				{
					try
					{
						Hobject crtImg;
						m_dictCameraImgBuffer[nChIndex]->pop_back(&crtImg);
						if(m_dictChImgSz.find(nChIndex) != m_dictChImgSz.end())
						{
							set_part(wndID, 0, 0, m_dictChImgSz[nChIndex].m_nHeight - 1, m_dictChImgSz[nChIndex].m_nWidth - 1);
						}
						else
						{
							HTuple height, width;
							get_image_size(crtImg, &width, &height);
							IMG_SZ sz;
							sz.m_nWidth  = width[0].I();
							sz.m_nHeight = height[0].I();
							m_dictTmpImgSz[nChIndex] = sz;
							set_part(wndID, 0, 0, m_dictTmpImgSz[nChIndex].m_nHeight - 1, m_dictTmpImgSz[nChIndex].m_nWidth - 1);

						}
						disp_obj(crtImg, wndID);
					}
					catch(HException &except)
					{
						gLogger.QueueUpLog(L"disp_obj timeout EXCEPTION", true);
					}					
				}
			}			
		}
		Sleep(5);		
	}
	return 0;
}

bool CDisplayingManager::SaveImage(int nChIndex)
{	
	IMG_PRODUCT pNgImg;
	DisplayResult pNgDispRst;
	m_dictSaveImgBuffer[nChIndex]->pop_front(&pNgImg);
	m_dictSaveImgDispBuffer[nChIndex]->pop_front(&pNgDispRst);
	
	Hlong imgWidth,imgHeight;
	get_image_size(pNgImg.m_img,&imgWidth,&imgHeight);	
	LONGLONG wndID = m_dictCameraWndID[nChIndex];
	Hobject crtImg   = pNgImg.m_img;	
	Hobject crtTargetRgn = pNgImg.m_crtTargetRgn;
	Hobject CrtCvInterImgs = pNgImg.m_cvInterImgs;
	Hobject CrtCorrectImg = pNgImg.m_crtCorrectImg;
	Hobject crtTargetRgnEx = pNgImg.m_crtTargetRgnEx;
	
	if(pNgImg.m_unionErrRgn.size()>0)
	{
		for (auto it = pNgImg.m_unionErrRgn.begin(); it != pNgImg.m_unionErrRgn.end();it++)
		{			
			if(it->second.Id() == H_EMPTY_REGION)
				continue;
			paint_region(it->second,pNgImg.m_img,&crtImg,255.0,"margin");
		}
	}
	if(crtTargetRgn.Id() != H_EMPTY_REGION)
	{
		paint_region(crtTargetRgn,crtImg,&crtImg,255.0,"margin");
	}
	if(crtTargetRgnEx.Id() != H_EMPTY_REGION)
	{
		paint_region(crtTargetRgnEx,crtImg,&crtImg,255.0,"margin");
	}
	if(crtImg.Id()!=H_EMPTY_REGION)
	{
		Hlong num;
		count_obj(crtImg,&num);
		if(num>0)
			compose3(crtImg,pNgImg.m_img,crtImg,&crtImg);
	}
	if(crtImg.Id()!=H_EMPTY_REGION)
	{
		Hlong num;
		count_obj(crtImg,&num);
		if(num>0)
			gSystemMgr.GetPanel(nChIndex)->AddImage(crtImg,FALSE);
	}
	return TRUE;
}
/*
unsigned int __stdcall CDisplayingManager::ImageDisplayhreadFunc()
{
	bool bIsAnyImg = false;
	int nChIndex    = -1;
	while((bIsAnyImg = IsAnyImage(nChIndex)) || m_nExitFlag == 0)
	{
		if(!m_bEnableDisplay)
		{
			for(size_t t = 1; t <= m_nArrayCount;t++)
			{
				while(m_dictCameraImgBuffer[t]->is_not_empty())
				{
					Hobject retImg;
					m_dictCameraImgBuffer[t]->pop_back(&retImg);
				}
			}
		}
		if(!bIsAnyImg)
		{
			if(IsAnyNgImage(nChIndex))				
				DispNgImage(nChIndex);
			Sleep(5);			
			continue;
		}
		else
		{
			/////////////////////// hzh /////////////////////////////////
			if(m_dictNgImgBuffer[nChIndex]->is_not_empty())
			{
				DispNgImage(nChIndex);
			}
			else{
			/////////////////////////////////////////////////////////////
				LONGLONG wndID = m_dictCameraWndID[nChIndex];
				if(m_dictCameraImgBuffer[nChIndex]->is_not_empty())
				{
					try
					{
						Hobject crtImg;
						m_dictCameraImgBuffer[nChIndex]->pop_back(&crtImg);
						if(m_dictChImgSz.find(nChIndex) != m_dictChImgSz.end())
						{
							set_part(wndID, 0, 0, m_dictChImgSz[nChIndex].m_nHeight - 1, m_dictChImgSz[nChIndex].m_nWidth - 1);
						}
						else
						{
							HTuple height, width;
							get_image_size(crtImg, &width, &height);
							IMG_SZ sz;
							sz.m_nWidth  = width[0].I();
							sz.m_nHeight = height[0].I();
							m_dictTmpImgSz[nChIndex] = sz;
							set_part(wndID, 0, 0, m_dictTmpImgSz[nChIndex].m_nHeight - 1, m_dictTmpImgSz[nChIndex].m_nWidth - 1);

						}
						disp_obj(crtImg, wndID);
					}
					catch(HException &except)
					{
						gLogger.QueueUpLog(L"disp_obj timeout EXCEPTION", true);
					}
					Sleep(5);
				}
			}
		}
	}
	return 0;
}*/

void CDisplayingManager::DisplayErrorRegionOnChWnd(std::map<Hlong,Hobject>& unionErrRgn,long long chImgWndID)
{
	// 	if(unionErrRgn.Id() == H_EMPTY_REGION)
	// 		return;

	if(::GetKeyState(VK_SPACE) < 0)
	{	
		return;
	}

	if(chImgWndID == -1)	
		return;
	int i = 0;
	for (auto it = unionErrRgn.begin(); it != unionErrRgn.end();it++)
	{
		if (it->first==0)
		{
			set_color(chImgWndID, "red");
		}
		else
		{
			set_color(chImgWndID, "orange");
		}
		Hobject ChunionErrRgn = it->second;
		if(ChunionErrRgn.Id() == H_EMPTY_REGION)
			return;
		disp_obj(ChunionErrRgn,chImgWndID);
		i ++;
	}
}

void CDisplayingManager::DisplayErrImgTargetRegionOnChWnd(Hobject& crtTargetRgn,long long chImgWndID,int nChIndex,int iRegionColor) 
{ 
	Hlong nModelCount;
	Hobject TargetRgn;
	count_obj(crtTargetRgn, &nModelCount);
	TargetRgn= crtTargetRgn;
	//if(m_bMultiChannnelImg)
	{
		//if (m_nCrtImgChannel<=nModelCount)
		{
			if (TargetRgn.Id()!=H_EMPTY_REGION)
			{
				select_obj(crtTargetRgn,&TargetRgn,nChIndex);
			}
		}
	}

	if(crtTargetRgn.Id() == H_EMPTY_REGION)
		return;

	if(chImgWndID == -1)
		return;

	if (iRegionColor == 0)
	{
		set_color(chImgWndID, "green");
	}
	else
	{
		set_color(chImgWndID, "yellow");
	}

	disp_obj(crtTargetRgn, chImgWndID);

}

void CDisplayingManager::DisplayInnerStatisticsOnChWnd(Hobject& crtImg,std::vector<float>	m_vInnerStatsData,int nChIndex,long long chImgWndID)
{
	size_t sz = m_vInnerStatsData.size();
	if(sz < 2) return;

	long long wndId = (long long)chImgWndID;

	if(wndId == -1/* || m_pChConfigWnd == nullptr*/) 
		return;
	if(crtImg.Id() == H_EMPTY_REGION) return;

	CChannelControlPanel *pPanel=gSystemMgr.GetPanel(nChIndex);
	if(pPanel==NULL)
		return;
	UI_SETTING us = pPanel->m_channelConfig.m_uiSetting;
	Hlong nWidth, nHeight;
	get_image_size(crtImg, &nWidth, &nHeight);

	float xRate = 1.0f * nWidth / us.m_width;
	float yRate = 1.0f * nHeight / us.m_height;


	if(sz==260)
	{
		HTuple featureValue = HTuple();
		int nDispType = 0; // Display all data
		float fThresholdOrg =m_vInnerStatsData[0];
		float hThresholdOrg =m_vInnerStatsData[1];
		float minDataX = m_vInnerStatsData[2];
		float maxDataX = m_vInnerStatsData[3];
		float minMaxDistX =maxDataX-minDataX;
		float fThreshold = (float)(255.0*((fThresholdOrg-minDataX)/minMaxDistX));
		float hThreshold = (float)(255.0*((hThresholdOrg-minDataX)/minMaxDistX));
		if(m_vInnerStatsData[0] == 999999.0f)
		{
			nDispType = -1;
			fThreshold = 0;
			hThreshold = ::abs(m_vInnerStatsData[1]);
		}
		if(m_vInnerStatsData[1] == -999999.0f)
		{
			nDispType = 1;
			fThreshold = ::abs(m_vInnerStatsData[0]);
			hThreshold = 0;
		}
		for (size_t i = 4; i < sz; i++)
		{
			featureValue.Append((HTuple)m_vInnerStatsData[i]*yRate);
		}
		auto itMinMax = std::minmax_element(m_vInnerStatsData.begin() + 4, m_vInnerStatsData.end());
		float fMaxValue = 1.0f;
		float fMinValue = 1.0f;
		if(itMinMax.first != m_vInnerStatsData.end())
			fMinValue = *(itMinMax.first);
		if(itMinMax.second != m_vInnerStatsData.end())
			fMaxValue = *(itMinMax.second);

		//test for showing the filter result
		HTuple XValue, xStringCut,xString;
		int xIndex = 0;
		xStringCut= minDataX;
		if(abs(minDataX)<1000)
		{
			xString[0] =xStringCut.ToString(".3");
		}
		else
		{
			xString[0] =xStringCut.ToString(".4");
		}
		for (int i = 0; i < sz - 4; i ++)
		{
			XValue[i] = i+1;
			if((i+1)%((sz-2)/8) == 0)
			{
				xIndex ++;
				xStringCut= ((i+1.000001)/256.0)*minMaxDistX+minDataX;
				if(abs(xStringCut[0].D())<1000)
				{
					xString[xIndex] =xStringCut.ToString(".3");
				}
				else
				{
					xString[xIndex] =xStringCut.ToString(".4");
				}
			}
		}

		Hobject contour11, baseLine, PosThreshLine, NegThreshLine;
		HTuple XStartValue = 140 * xRate;
		HTuple XRange = 400 * xRate;
		HTuple XScaleFactor = 400 * xRate/(sz - 4);
		HTuple YStartValue = nHeight - 125 * yRate; //TODO
		HTuple YRange = 100 * yRate;

		HTuple lineX;
		lineX.Append(XStartValue);
		lineX.Append(XStartValue + XRange);
		HTuple lineY;
		lineY.Append(YStartValue);
		lineY.Append(YStartValue);

		HTuple PosLineRow, PosLineCol;
		HTuple NegLineRow, NegLineCol;

		PosLineRow.Append(YStartValue);
		PosLineRow.Append(YStartValue - YRange);
		PosLineCol.Append(XStartValue + fThreshold * XScaleFactor);
		PosLineCol.Append(XStartValue + fThreshold * XScaleFactor);

		NegLineRow.Append(YStartValue);
		NegLineRow.Append(YStartValue - YRange);
		NegLineCol.Append(XStartValue + hThreshold * XScaleFactor);//
		NegLineCol.Append(XStartValue + hThreshold * XScaleFactor);

		gen_contour_polygon_xld(&baseLine, lineY, lineX);
		gen_contour_polygon_xld(&contour11, YStartValue + featureValue, XStartValue + XValue * XScaleFactor);
		gen_contour_polygon_xld(&PosThreshLine, PosLineRow, PosLineCol);
		gen_contour_polygon_xld(&NegThreshLine, NegLineRow, NegLineCol);

		set_color(wndId, "blue");
		disp_obj(contour11, wndId);
		disp_obj(baseLine, wndId);
		set_color(wndId, "yellow");
		for(int i = 0; i <= xIndex; i ++)   
		{
			set_tposition(wndId, YStartValue + 3 * yRate, XStartValue + i * xRate * 50);
			write_string(wndId, xString[i]);
		}
		switch(nDispType)
		{
		case 1:
			set_color(wndId, "yellow");
			disp_obj(PosThreshLine, wndId);
			break;
		case -1:
			set_color(wndId, "yellow");
			disp_obj(NegThreshLine, wndId);
			break; 
		case 0:
			set_color(wndId, "yellow");
			disp_obj(PosThreshLine, wndId);
			set_color(wndId, "red");
			disp_obj(NegThreshLine, wndId);
			break;
		default:
			break;
		}
		return;
	}
	HTuple featureValue = HTuple();
	int nDispType = 0; // Display all data
	float fThreshold = ::abs(m_vInnerStatsData[0]);
	float hThreshold = ::abs(m_vInnerStatsData[1]);
	if(m_vInnerStatsData[0] == 999999.0f)
	{
		nDispType = -1;
		fThreshold = 0;
		hThreshold = ::abs(m_vInnerStatsData[1]);
	}
	if(m_vInnerStatsData[1] == -999999.0f)
	{
		nDispType = 1;
		fThreshold = ::abs(m_vInnerStatsData[0]);
		hThreshold = 0;
	}
	for (size_t i = 2; i < sz; i++)
	{
		featureValue.Append((HTuple)m_vInnerStatsData[i]*yRate);
	}
	auto itMinMax = std::minmax_element(m_vInnerStatsData.begin() + 2, m_vInnerStatsData.end());
	float fMaxValue = 1.0f;
	float fMinValue = 1.0f;
	if(itMinMax.first != m_vInnerStatsData.end())
		fMinValue = *(itMinMax.first);
	if(itMinMax.second != m_vInnerStatsData.end())
		fMaxValue = *(itMinMax.second);

	//test for showing the filter result
	HTuple XValue, xString;
	int xIndex = 0;
	xString[0] = 0;
	for (int i = 0; i < sz - 2; i ++)
	{
		XValue[i] = i+1;
		if((i+1)%((sz-2)/8) == 0)
		{
			xIndex ++;
			xString[xIndex] = i+1;
		}
	}

	Hobject contour11, baseLine, PosThreshLine, NegThreshLine;
	HTuple XStartValue = 140 * xRate;
	HTuple XRange = 400 * xRate;
	HTuple XScaleFactor = 400 * xRate/(sz - 2);
	HTuple YStartValue = nHeight - 125 * yRate; //TODO
	HTuple YRange = 100 * yRate;

	HTuple lineX;
	lineX.Append(XStartValue);
	lineX.Append(XStartValue + XRange);
	HTuple lineY;
	lineY.Append(YStartValue);
	lineY.Append(YStartValue);

	HTuple PosLineRow, PosLineCol;
	HTuple NegLineRow, NegLineCol;

	if (sz > 500)    
	{
		if(YStartValue[0].D() - fThreshold*yRate > nHeight - 125*yRate - 120*yRate)
		{
			PosLineRow.Append(YStartValue - fThreshold * yRate);
			PosLineRow.Append(YStartValue - fThreshold * yRate);
		}
		else
		{
			PosLineRow.Append(HTuple(nHeight - 125 * yRate - 120 * yRate));
			PosLineRow.Append(HTuple(nHeight - 125 * yRate - 120 * yRate));
		}
		PosLineCol.Append(XStartValue);
		PosLineCol.Append(XStartValue+XRange);

		if(YStartValue[0].D() + hThreshold*yRate < nHeight - 25*yRate)
		{
			NegLineRow.Append(YStartValue + hThreshold * yRate);
			NegLineRow.Append(YStartValue + hThreshold * yRate);
		}
		else
		{
			NegLineRow.Append(HTuple(nHeight - 5 * yRate));
			NegLineRow.Append(HTuple(nHeight - 5 * yRate));
		}
		NegLineCol.Append(XStartValue);
		NegLineCol.Append(XStartValue+XRange);
	}
	else         
	{
		PosLineRow.Append(YStartValue);
		PosLineRow.Append(YStartValue - YRange);
		PosLineCol.Append(XStartValue + fThreshold * XScaleFactor);
		PosLineCol.Append(XStartValue + fThreshold * XScaleFactor);

		NegLineRow.Append(YStartValue);
		NegLineRow.Append(YStartValue - YRange);
		NegLineCol.Append(XStartValue + hThreshold * XScaleFactor);//
		NegLineCol.Append(XStartValue + hThreshold * XScaleFactor);
	}

	gen_contour_polygon_xld(&baseLine, lineY, lineX);
	gen_contour_polygon_xld(&contour11, YStartValue + featureValue, XStartValue + XValue * XScaleFactor);
	gen_contour_polygon_xld(&PosThreshLine, PosLineRow, PosLineCol);
	gen_contour_polygon_xld(&NegThreshLine, NegLineRow, NegLineCol);

	set_color(wndId, "blue");
	disp_obj(contour11, wndId);
	disp_obj(baseLine, wndId);
	set_color(wndId, "yellow");
	for(int i = 0; i <= xIndex; i ++)   
	{
		set_tposition(wndId, YStartValue + 3 * yRate, XStartValue + i * xRate * 50);
		write_string(wndId, xString[i]);
	}
	switch(nDispType)
	{
	case 1:
		set_color(wndId, "yellow");
		disp_obj(PosThreshLine, wndId);
		break;
	case -1:
		set_color(wndId, "yellow");
		disp_obj(NegThreshLine, wndId);
		break; 
	case 0:
		set_color(wndId, "yellow");
		disp_obj(PosThreshLine, wndId);
		set_color(wndId, "red");
		disp_obj(NegThreshLine, wndId);
		break;
	default:
		break;
	}
}

void CDisplayingManager::DispNgImage(int nChIndex)
{
	Hobject crtImg;
	if(m_dictCameraImgBuffer[nChIndex]->is_not_empty())
		m_dictCameraImgBuffer[nChIndex]->pop_back(&crtImg);
	IMG_PRODUCT *pNgImg=NULL;
	DisplayResult *pNgDispRst=NULL;
	m_dictNgImgBuffer[nChIndex]->front(&pNgImg);
	m_dictNgImgDispBuffer[nChIndex]->front(&pNgDispRst);
	if(pNgImg==NULL) 
		return;
	if(pNgImg->m_ImageTimeCount<gSystemMgr.GetPanel(nChIndex)->GetErrorImageStayTime())
	{
		LONGLONG wndID = m_dictCameraWndID[nChIndex];
		Hobject crtImg   = pNgImg->m_img;
		std::map<Hlong,Hobject> crtimgError = pNgImg->m_unionErrRgn;
		Hobject crtTargetRgn = pNgImg->m_crtTargetRgn;
		Hobject CrtCvInterImgs = pNgImg->m_cvInterImgs;
		Hobject CrtCorrectImg = pNgImg->m_crtCorrectImg;
		Hobject crtTargetRgnEx = pNgImg->m_crtTargetRgnEx;
		pNgImg->m_ImageTimeCount+=5;
		disp_obj(crtImg, wndID);

		gSystemMgr.GetPanel(nChIndex)->UpdateMainScreen(false,false);
		if(crtimgError.size()>0)
			DisplayErrorRegionOnChWnd(crtimgError,wndID);		
		if(pNgDispRst!=NULL)
			DisplayInnerStatisticsOnChWnd(crtImg,pNgDispRst->m_vInnerStatsData,nChIndex,wndID);
		DisplayErrImgTargetRegionOnChWnd(crtTargetRgn,wndID,nChIndex);

		DisplayErrImgTargetRegionOnChWnd(crtTargetRgnEx,wndID,nChIndex,1);
	}
	else
	{
		IMG_PRODUCT NgImg;
		m_dictNgImgBuffer[nChIndex]->pop_front(&NgImg);
		DisplayResult NgDispRst;
		m_dictNgImgDispBuffer[nChIndex]->pop_front(&NgDispRst);
	}
}

void CDisplayingManager::EnableDisplaying(bool bDisplay)
{
	m_bEnableDisplay = bDisplay;
}