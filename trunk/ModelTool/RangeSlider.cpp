// RangeSlider.cpp : 实现文件
//

#include "stdafx.h"
#include "RangeSlider.h"
#include "memdc.h"

#include <algorithm>
using namespace std;

// CRangeSlider
//两个滑块之间最小允许的逻辑间距
const int MINLP = 1;

//使用中键滑轮放大时的放大因子
const double FACTOR_ZOOMIN = 1.2;

//使用中键滑轮缩小时的缩小因子
const double FACTOR_ZOOMOUT = 0.8;

IMPLEMENT_DYNAMIC(CRangeSlider, CWnd)

CRangeSlider::CRangeSlider()
{
	m_nMin = 0;
	m_nMax = 100;
	m_nThumbMin = 0;
	m_nThumbMax = 50;
	m_bThumbMinChanged = true;
	m_bThumbMaxChanged = true;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nMargin = 0;
	m_nPt = 0;
	m_bDragMin = false;
	m_bDragMax = false;
	m_bEnableMouseWheel = true;//false;
	m_bVertical = false;
	m_bSingleSlider = false;
	m_bCurrentBlock = false;
}

CRangeSlider::~CRangeSlider()
{
}

//创建窗口
BOOL CRangeSlider::CreateWnd(CWnd* pWndParent, const CRect& winRect, UINT nID /* = 0x00 */)
{
	BOOL nResult = FALSE;
	do 
	{
		if ( NULL == pWndParent)
		{
			nResult = FALSE;
		}
		else
		{
			//类型为msctls_trackbar32才能使鼠标离开控件后，依然可以监控鼠标移动状态
			nResult =  Create(_T("msctls_trackbar32"), _T(""), WS_VISIBLE | WS_CHILD | SS_NOTIFY, winRect, pWndParent ,nID);
		}
	} while (false);

	return nResult;
}

//设置控件是否为竖直
void CRangeSlider::SetVertical(bool bVertical /* = true */)
{
	m_bVertical = bVertical;
	if ( bVertical)
	{
		ModifyStyle(NULL, TBS_VERT);//添加竖直属性，才能使控件竖直时鼠标离开控件时继续监控
	}
	else
	{
		ModifyStyle(TBS_VERT, NULL);//去除竖直属性
	}
}

void CRangeSlider::SetChannelMin(int nChannelMin)
{
	m_nMin = nChannelMin;
	m_bThumbMaxChanged = true;
	m_bThumbMinChanged = true;
}

int CRangeSlider::GetChannelMin()
{
	return m_nMin;
}
void CRangeSlider::SetChannelMax(int nChannelMax)
{
	m_nMax = nChannelMax;
	m_bThumbMaxChanged = true;
	m_bThumbMinChanged = true;
}

int CRangeSlider::GetChannelMax()
{
	return m_nMax;
}

void CRangeSlider::SetThumbMin(int nThumbMin)
{
	m_nThumbMin = nThumbMin;
}

int CRangeSlider::GetThumbMin()
{
	return m_nThumbMin;
}

void CRangeSlider::SetThumbMax(int nThumbMax)
{
	m_nThumbMax = nThumbMax;
}

int CRangeSlider::GetThumbMax()
{
	return m_nThumbMax;
}

void CRangeSlider::SetThumbMargin(int nMargin)
{
	m_nMargin = nMargin;
}

void CRangeSlider::SendStopSliderMessage()
{
	m_bDragMax = false;
	m_bDragMin = false;
	m_nPt = 0;
}
//加载channel图片资源
void CRangeSlider::LoadChannelImage(UINT nRecourceID, const CString& cstrResourceType)
{
	m_imageChannel.Destroy();
	LoadImageFromResource(nRecourceID, cstrResourceType, &m_imageChannel);
	DealAlphaChannel(m_imageChannel);
}

void CRangeSlider::LoadChannelImage(const CString& cstrImageFilePath)
{
	m_imageChannel.Destroy();
	m_imageChannel.Load(cstrImageFilePath);
	DealAlphaChannel(m_imageChannel);
}
//加载thumb图片资源
void CRangeSlider::LoadThumbImage(UINT nRecourceID, const CString& cstrResourceType)
{
	m_imageThumb.Destroy();
	LoadImageFromResource(nRecourceID, cstrResourceType, &m_imageThumb);
	DealAlphaChannel(m_imageThumb);
}

void CRangeSlider::LoadThumbImage(const CString& cstrImageFilePath)
{
	m_imageThumb.Destroy();
	m_imageThumb.Load(cstrImageFilePath);
	DealAlphaChannel(m_imageThumb);
}

//从资源加载图片
void CRangeSlider::LoadImageFromResource(UINT nRecourceID, const CString& cstrResourceType, CImage* pImage)
{
	if (NULL == pImage)
	{
		return;
	}

	pImage->Destroy();
	
	HINSTANCE hInst = ::AfxGetInstanceHandle();
	if ("bmp" == cstrResourceType)
	{
		pImage->LoadFromResource(hInst, nRecourceID);
	}
	else
	{
		HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(nRecourceID),cstrResourceType); // type
		if (!hRsrc)
			return ;

		BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
		if (!lpRsrc) 
		{
			return ;  
		}
		
		// Allocate global memory on which to create stream  
		DWORD imageSize = ::SizeofResource(hInst, hRsrc);  
		HGLOBAL hBuffer = ::GlobalAlloc(GMEM_FIXED, imageSize);  

		BYTE* pBuffer = (BYTE*)GlobalLock(hBuffer);  
		memcpy(pBuffer,lpRsrc,imageSize);  

		IStream* pstm = NULL;  
		::CreateStreamOnHGlobal(hBuffer,FALSE,&pstm);  

		// load from stream  
		pImage->Load(pstm);  

		// free/release stuff  
		::GlobalUnlock(hBuffer);
		::GlobalFree(hBuffer);

		pstm->Release();  

		FreeResource(lpRsrc);
	}
}

//对透明图片进行处理
void CRangeSlider::DealAlphaChannel(ATL::CImage& image)
{
	if (image.GetBPP() == 32)
	{
		for (int i = 0; i < image.GetWidth(); i++)
		{
			for (int j = 0; j < image.GetHeight(); j++)
			{
				unsigned char* pucColor = reinterpret_cast<unsigned char*>(image.GetPixelAddress(i,j));
				pucColor[0] = pucColor[0] * pucColor[3] / 255;
				pucColor[1] = pucColor[1] * pucColor[3] / 255;
				pucColor[2] = pucColor[2] * pucColor[3] / 255;
			}
		}
	}
}

BEGIN_MESSAGE_MAP(CRangeSlider, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CRangeSlider 消息处理程序
void CRangeSlider::IncreaseStep(int iStepValue)
{
	if (!m_bCurrentBlock)
	{
		if (m_nThumbMin >= m_nThumbMax)
		{
			return;
		}
		m_nThumbMin += iStepValue;
		m_bThumbMinChanged = true;
	}
	else
	{
		if (m_nThumbMax>= m_nMax)
		{
			return;
		}
		m_nThumbMax += iStepValue;
		m_bThumbMaxChanged = true;
	}
	Invalidate();
	::PostMessage(GetParent()->m_hWnd,
		WM_RANGESLIDER_RANGECHANGED,
		GetDlgCtrlID(),//传递窗口ID，处理消息时获取响应的窗口，则可以通过接口获取滑块的逻辑值
		NULL);
}

void CRangeSlider::DecreaseStep(int iStepValue)
{
	if (!m_bCurrentBlock)
	{
		if (m_nThumbMin <= m_nMin)
		{
			return;
		}
		m_nThumbMin -=  iStepValue;
		m_bThumbMinChanged = true;

		
//		nCurPosLP = m_nThumbMin;
	}
	else
	{
		if (m_nThumbMax <= m_nThumbMin)
		{
			return;
		}
		m_nThumbMax -= iStepValue;
		m_bThumbMaxChanged = true;
		

//		nCurPosLP = m_nThumbMax;
	}
	Invalidate();
	::PostMessage(GetParent()->m_hWnd,
		WM_RANGESLIDER_RANGECHANGED,
		GetDlgCtrlID(),//传递窗口ID，处理消息时获取响应的窗口，则可以通过接口获取滑块的逻辑值
		NULL);
}

//鼠标左键按下时的消息响应函数
void CRangeSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nPt = 0;//鼠标当前点击的位置
	int nMinDP = Pos2Pixel(m_nThumbMin);//最小值对应的物理位置
	int nMaxDP = Pos2Pixel(m_nThumbMax);//最大值对应的物理位置
	if (m_bVertical)
	{
		nPt = point.y;//控件竖直时，只关心鼠标点击位置的y坐标与最小值或者最大值之间的物理距离
	}
	else
	{
		nPt = point.x;//控件水平时，只关心鼠标点击位置的x坐标与最小值或者最大值之间的物理距离
	}

	if (nPt < nMinDP && nPt > nMinDP - m_nMargin)//点击最小值滑块上的时候开启拖动最小值功能，并记录点击处与最小值滑块的物理距离
	{
		m_bDragMin = true;
		m_nPt = nPt - nMinDP;
		m_bCurrentBlock = false;
	}
	else if (nPt > nMaxDP && nPt < nMaxDP + m_nMargin)//点击最大值滑块上的时候开启拖动最大值功能，并记录点击处与最大值滑块的物理距离
	{
		if(m_bSingleSlider == false)
		{
			m_bDragMax = true;
			m_nPt = nPt - nMaxDP;
		}
		else
		{
			m_bDragMax = false;
		}
		m_bCurrentBlock = true;
	}
	else
	{
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//鼠标左键弹起时的消息响应函数
void CRangeSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDragMax = false;
	m_bDragMin = false;
	m_nPt = 0;

	::PostMessage(GetParent()->m_hWnd,
		WM_RANGESLIDER_RANGECHANGED,
		GetDlgCtrlID(),//传递窗口ID，处理消息时获取响应的窗口，则可以通过接口获取滑块的逻辑值
		NULL);
	CWnd::OnLButtonUp(nFlags, point);
}

//鼠标移动时的消息响应函数
void CRangeSlider::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rcClient;
	this->GetClientRect(rcClient);
	if(!rcClient.PtInRect(point))
	{
		m_bDragMax = false;
		m_bDragMin = false;
		m_nPt = 0;
	}
	int nCurPos = 0;
	if (m_bVertical)
	{
		nCurPos = point.y;
	}
	else
	{
		nCurPos = point.x;
	}

	//计算出滑块新的逻辑位置
	int nCurPosLP = Pixel2Pos(nCurPos - m_nPt);

	//需要同时拖动最小值滑块和最大值滑块
	if (m_bDragMin && m_bDragMax)
	{
		int deltaLP = m_nThumbMax - m_nThumbMin;//记录拖动前，两个滑块间的逻辑距离

		//出界处理
		if (nCurPosLP < m_nMin)
		{
			nCurPosLP = m_nMin;
		}
		else if (nCurPosLP + deltaLP > m_nMax)
		{
			nCurPosLP = m_nMax - deltaLP;
		}

		//两个滑块移动到新的位置
		m_nThumbMin = nCurPosLP;
		m_bThumbMinChanged = true;

		m_nThumbMax = nCurPosLP + deltaLP;
		m_bThumbMaxChanged = true;
		Invalidate();
	}
	//需要拖动最小值
	else if (m_bDragMin)
	{
		//出界处理
		if (nCurPosLP > m_nThumbMax - MINLP)
		{
			nCurPosLP = m_nThumbMax - MINLP;
		}
		else if (nCurPosLP < m_nMin)
		{
			nCurPosLP = m_nMin;
		}

		//最小值滑块移动到新的位置
		m_nThumbMin = nCurPosLP;
		m_bThumbMinChanged = true;

		Invalidate();
	}
	//需要拖动最大值
	else if (m_bDragMax)
	{
		//出界处理
		if (nCurPosLP > m_nMax)
		{
			nCurPosLP = m_nMax;
		}
		else if (nCurPosLP < m_nThumbMin + MINLP)
		{
			nCurPosLP = m_nThumbMin + MINLP;
		}

		m_nThumbMax = nCurPosLP;
		m_bThumbMaxChanged = true;
		Invalidate();
	}

	//CWnd::OnMouseMove(nFlags, point);
}

//鼠标中键滚动时的消息响应函数
//BOOL CRangeSlider::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	if(nFlags == MK_CONTROL)
//		return TRUE;
//
//	if (m_bEnableMouseWheel)
//	{
//		int nHalf = double(m_nThumbMax - m_nThumbMin)/2 + 0.5;//(m_nThumbMax - m_nThumbMin)/2;
//		if (zDelta > 0)
//		{
//			//放大
//			m_nThumbMin = m_nThumbMin - nHalf * (FACTOR_ZOOMIN - 1) - 0.5;//注意double型数据转换为int型数据时造成的损失，+ 0.5解决
//			if(!m_bSingleSlider)
//				m_nThumbMax = m_nThumbMax + nHalf * (FACTOR_ZOOMIN - 1) + 0.5;
//		}
//		else
//		{
//			//缩小
//			m_nThumbMin = m_nThumbMin + nHalf * (1 - FACTOR_ZOOMOUT) + 0.5;
//			if(!m_bSingleSlider)
//				m_nThumbMax = m_nThumbMax - nHalf * (1 - FACTOR_ZOOMOUT) - 0.5;
//		}
//
//		//出界处理
//		if (m_nThumbMax - m_nThumbMin < MINLP)
//		{
//			m_nThumbMax = m_nThumbMin + MINLP;
//		}
//		if ( m_nThumbMin < m_nMin)
//		{
//			m_nThumbMin = m_nMin;
//		}
//		if (m_nThumbMax > m_nMax)
//		{
//			m_nThumbMax = m_nMax;
//		}
//		m_bThumbMinChanged = true;
//		m_bThumbMaxChanged = true;
//
//		::PostMessage(
//			GetParent()->m_hWnd,
//			WM_RANGESLIDER_RANGECHANGED,
//			GetDlgCtrlID(),//传递窗口ID，处理消息时获取响应的窗口，则可以通过接口获取滑块的逻辑值
//			NULL);
//
//		Invalidate();
//	}
//
//	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
//}

//绘图函数
void CRangeSlider::OnPaint()
{
	CPaintDC dcOrigin(this);

	CRect winRect;
	GetClientRect(&winRect);

	m_nWidth = winRect.Width();
	m_nHeight = winRect.Height();

	//使用双缓冲减除闪烁
	CMemoryDC dc(&dcOrigin,&winRect, TRUE);

	//绘图前进行范围检查
	if (m_nMin > m_nMax)
	{
		swap(m_nMin, m_nMax);
	}
	if (m_nThumbMin > m_nThumbMax)
	{
		swap(m_nThumbMin, m_nThumbMax);
	}
	if (m_nThumbMin < m_nMin)
	{
		m_nThumbMin = m_nMin;
	}
	if (m_nThumbMax > m_nMax)
	{
		m_nThumbMax = m_nMax;
	}

	//根据控件是否竖直确定channel和thumb的物理位置和尺寸
	//channel
	int nChannelDestX = 0;
	int nChannelDestY = 0;

	int nChannelSrcX = 0;
	int nChannelSrcY = 0;

	int nChannelWidth = m_imageChannel.GetWidth();
	int nChannelHeight = m_imageChannel.GetHeight();

	//thumb
	int nThumbWidth_half = 0;
	int nThumbHeight_half = 0;

	//thumb分成两段绘制，分别从资源两端向内绘制
	int nThumbDestX_min = 0;
	int nThumbDestY_min = 0;
	int nThumbDestX_max = 0;
	int nThumbDestY_max = 0;

	int nThumbSrcX_min = 0;
	int nThumbSrcY_min = 0;
	int nThumbSrcX_max = 0;
	int nThumbSrcY_max = 0;

	int nThumbSpanDP_half = ( Pos2Pixel(m_nThumbMax) - Pos2Pixel(m_nThumbMin) ) / 2; //两个滑块间距的一半

	if (m_bVertical)//控件竖直
	{
		if (m_nMargin <= 0)
		{
			m_nMargin = m_imageThumb.GetWidth();
		}

		nChannelDestX = m_nWidth / 2 - m_imageChannel.GetWidth() / 2;

		nThumbWidth_half = m_imageThumb.GetWidth();
		nThumbHeight_half = nThumbSpanDP_half + m_nMargin;

		nThumbDestX_min = m_nWidth / 2 - m_imageThumb.GetWidth() / 2;
		nThumbDestY_min = Pos2Pixel(m_nThumbMin) - m_nMargin;

		nThumbDestX_max = nThumbDestX_min;
		//根据最大值（而不是最小值）的位置来绘制下半段，避免拖动最小值滑块时出现抖动现象
		nThumbDestY_max = Pos2Pixel(m_nThumbMax) - double(nThumbSpanDP_half) - 0.5;

		//绘制下半段的时候，提取资源的下半段
		nThumbSrcY_max = m_imageThumb.GetHeight() - nThumbSpanDP_half - m_nMargin;
	}
	else//控件为水平
	{
		if (m_nMargin <= 0)
		{
			m_nMargin = m_imageThumb.GetHeight();
		}

		nChannelDestY = m_nHeight / 2 - m_imageChannel.GetHeight() / 2;

		nThumbWidth_half = nThumbSpanDP_half + m_nMargin;
		nThumbHeight_half = m_imageThumb.GetHeight();

		nThumbDestX_min = Pos2Pixel(m_nThumbMin) - m_nMargin;
		nThumbDestY_min = m_nHeight / 2 - m_imageThumb.GetHeight() / 2;

		//根据最大值（而不是最小值）的位置来绘制右半段，避免拖动最小值滑块时出现抖动现象
		nThumbDestX_max = int(Pos2Pixel(m_nThumbMax) - double(nThumbSpanDP_half) - 0.5);
		nThumbDestY_max = nThumbDestY_min;

		//绘制右半段的时候，提取资源的右半段
		nThumbSrcX_max = m_imageThumb.GetWidth() - nThumbSpanDP_half - m_nMargin;
	}

	//绘制channel
	m_imageChannel.Draw(dc->m_hDC,
		nChannelDestX, 
		nChannelDestY, 
		nChannelWidth, 
		nChannelHeight,
		nChannelSrcX, 
		nChannelSrcY, 
		nChannelWidth, 
		nChannelHeight);

	//绘制thumb，先从最小值滑块处向中间绘制一半
	m_imageThumb.Draw(dc->m_hDC, 
		nThumbDestX_min,
		nThumbDestY_min,
		nThumbWidth_half,
		nThumbHeight_half,
		nThumbSrcX_min,
		nThumbSrcY_min,
		nThumbWidth_half,
		nThumbHeight_half);

	//绘制thumb，再从最大值滑块处向中间绘制一半
	m_imageThumb.Draw(dc->m_hDC, 
		nThumbDestX_max,
		nThumbDestY_max,
		nThumbWidth_half,
		nThumbHeight_half,
		nThumbSrcX_max,
		nThumbSrcY_max,
		nThumbWidth_half,
		nThumbHeight_half);
}

void CRangeSlider::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

BOOL CRangeSlider::OnEraseBkgnd(CDC* pDC)
{
	//返回TRUE，避免刷新频繁时画面闪烁
	return TRUE;
}

bool CRangeSlider::GetThumbMinChangedFlag()
{
	return m_bThumbMinChanged;
}

bool CRangeSlider::GetThumbMaxChangedFlag()
{
	return m_bThumbMaxChanged;
}

void CRangeSlider::SetThumbMinChangedFlag(bool flag)
{
	m_bThumbMinChanged = flag;
}

void CRangeSlider::SetThumbMaxChangedFlag(bool flag)
{
	m_bThumbMaxChanged = flag;
}
