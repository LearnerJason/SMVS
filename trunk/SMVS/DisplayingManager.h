#pragma once

#include <map>
#include <cpp/HalconCpp.h>
#include "bounded_buffer.h"
#include "TypeDef.h"

using namespace std;
using namespace Halcon;

class CDisplayingManager
{
public:
	CDisplayingManager(void);
	~CDisplayingManager(void);

	volatile unsigned int m_nExitFlag;

	void CreateImageBuffer();
	void RegisterRealTimeImgWindowID(int nChIndex, LONGLONG wndID);
	void ReceiveRealTimeImage(int nChIndex, Hobject img);
	void ReceiveNgImage(int nChIndex,IMG_PRODUCT img_product);
	void ReceiveNgImage(int nChIndex, std::vector<IMG_PRODUCT> v_img,std::vector<DisplayResult> v_imgDispRst);
	void DispNgImage(int nChIndex);
	void SetImageSize(int nChIndex, IMG_SZ sz);

	void StartImageDisplayThread();
	unsigned int __stdcall ImageDisplayhreadFunc();
	void EnableDisplaying(bool bDisplay);
private:
	bool IsAnyImage(int& nChIndex);
	bool IsAnyNgImage(int& nChIndex);
	bool SaveImage(int nChIndex);
	void DisplayInnerStatisticsOnChWnd(Hobject& crtImg,std::vector<float>	m_vInnerStatsData,int nChIndex,long long chImgWndID);
	void DisplayErrImgTargetRegionOnChWnd(Hobject& crtTargetRgn,long long chImgWndID,int nChIndex,int iRegionColor=0);
	void DisplayErrorRegionOnChWnd(std::map<Hlong,Hobject>& unionErrRgn,long long chImgWndID);
private:
	const int                                PENDING_BUFFER_SZ;
	volatile bool                            m_bEnableDisplay;
	int                                      m_nArrayCount;
	std::map<int,IMG_SZ>                     m_dictChImgSz;
	std::map<int,IMG_SZ>                     m_dictTmpImgSz;
	std::map<int, bounded_buffer<Hobject>*>  m_dictCameraImgBuffer;
	std::map<int, bounded_buffer<IMG_PRODUCT>*>  m_dictNgImgBuffer;
	std::map<int, bounded_buffer<IMG_PRODUCT>*>  m_dictSaveImgBuffer;
	std::map<int, bounded_buffer<DisplayResult>*> m_dictNgImgDispBuffer;
	std::map<int, bounded_buffer<DisplayResult>*> m_dictSaveImgDispBuffer;
	std::map<int, long long>                 m_dictCameraWndID;
	//Timing
	LARGE_INTEGER			                 m_freq;
	LARGE_INTEGER			                 m_timestamp;
	union _Proc
	{
		unsigned (_stdcall * ThreadProc)(void*);
		unsigned (_stdcall CDisplayingManager::*MemProc)();
	} Proc;
};