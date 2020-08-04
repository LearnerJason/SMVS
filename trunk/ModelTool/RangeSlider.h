#pragma once

/********************************************************************
 * File name      : RangeSlider.h
 * Create time    : 2013-10-08 
 * Brief          : 双滑块滑动条控件，可以选择范围的上下限
 * Detail         : 可加载多种格式图片（bmp，jpg，png等），从资源或者文件路径加载都可以；滑块位置改变时，可通知父窗口；支持竖直滑动（要求资源为竖直的）
 * Other          : 创建时可动态创建，可静态创建：
 *                1、静态创建：
 *                     从工具箱中随便拖一个控件到界面，绑定变量，修改变量类型为CRangeSlider，调用加载channel和thumb图片的接口，设置逻辑值范围即可；
 *                2、动态创建：
 *                     在头文件中声明CRangeSlider类型变量，在实现文件的初始化函数中，调用CreateWnd创建窗口，接着加载图片、设置范围即可；
 * Modify history :
 * .................2013-10-10 detail:添加鼠标中键滑轮放大、缩小选择的范围
 * .................2013-10-12 detail:添加对透明图片的alpha通道处理，创建窗口函数增加ID参数
 *********************************************************************/

#ifndef RANGESLIDER_H__
#define RANGESLIDER_H__

// CRangeSlider
#include <atlimage.h>

#define WM_RANGESLIDER_RANGECHANGED WM_USER + 150

class CRangeSlider : public CWnd
{
	DECLARE_DYNAMIC(CRangeSlider)

public:
	CRangeSlider();
	virtual ~CRangeSlider();
	BOOL CreateWnd(CWnd* pWndParent, const CRect& winRect, UINT nID = 0x00);
	void LoadChannelImage(UINT nRecourceID, const CString& cstrResourceType);
	void LoadChannelImage(const CString& cstrImageFilePath);
	void LoadThumbImage(UINT nRecourceID, const CString& cstrResourceType);
	void LoadThumbImage(const CString& cstrImageFilePath);
	void SetVertical(bool bVertical = true);
	void SetChannelMin(int nChannelMin);
	int GetChannelMin();
	void SetChannelMax(int nChannelMax);
	int GetChannelMax();
	void SetThumbMin(int nThumbMin);
	int GetThumbMin();
	bool GetThumbMinChangedFlag();
	void SetThumbMinChangedFlag(bool flag);
	void SetThumbMax(int nThumbMax);
	int GetThumbMax();
	bool GetThumbMaxChangedFlag();
	void SetThumbMaxChangedFlag(bool flag);
	void SetThumbMargin(int nMargin);
	void SetSingleSliderFlag(bool flag)
	{
		m_bSingleSlider = flag;
	}
	bool GetSingleSliderFlag()
	{
		return m_bSingleSlider;
	}

	void SendStopSliderMessage();

	void SetEnableMouseWheelFlag(bool bEnable)
	{
		m_bEnableMouseWheel = bEnable;
	}

protected:
	DECLARE_MESSAGE_MAP()
	void LoadImageFromResource(UINT nRecourceID, const CString& cstrResourceType, ATL::CImage* pImage);

	void DealAlphaChannel(ATL::CImage& image);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void IncreaseStep(int iStepValue = 1);
	void DecreaseStep(int iStepValue = 1);

private:
	//在内存中保存图像
	CImage m_imageChannel;
	CImage m_imageThumb;

	bool m_bCurrentBlock;  //记录当前拖动滑块  flase:min  true:max

	//控件边界对应的逻辑最小值和最大值
	int m_nMin;
	int m_nMax;

	//标记范围的双滑块的逻辑最小值和最大值
	int m_nThumbMin;
	int m_nThumbMax;
	bool m_bThumbMinChanged;
	bool m_bThumbMaxChanged;

	//控件的尺寸
	int m_nWidth;
	int m_nHeight;

	//控件边距（相当于滑块占用的空间）
	int m_nMargin;

	//拖动滑块前，记录鼠标与滑块之间的物理距离
	int m_nPt;

	//是否拖动最小值或最大值滑块
	bool m_bDragMin;
	bool m_bDragMax;

	//是否开启鼠标中键放大、缩小最值间距
	bool m_bEnableMouseWheel;

	//控件是否竖直
	bool m_bVertical;

	bool m_bSingleSlider;

	//坐标转换时要考虑滑块占据的位置
	//物理位置转换为逻辑位置
	inline int Pixel2Pos(int nPixel)
	{
		if (m_bVertical)
		{
			return m_nMin + double( double( (m_nMax - m_nMin) * (nPixel - m_nMargin) ) / (m_nHeight - 2*m_nMargin) + 0.5 );
		}
		else
		{
			return m_nMin + double( double( (m_nMax - m_nMin) * (nPixel - m_nMargin) ) / (m_nWidth - 2*m_nMargin) + 0.5 );
		}
	}

	//逻辑位置转化为物理位置
	inline int Pos2Pixel(int nPos)
	{
		if (m_bVertical)
		{
			return m_nMargin + double( double( (nPos - m_nMin) * (m_nHeight - 2*m_nMargin) ) / (m_nMax - m_nMin) + 0.5 );
		}
		else
		{
			return m_nMargin + double( double( (nPos - m_nMin) * (m_nWidth - 2*m_nMargin) ) / (m_nMax - m_nMin) + 0.5 );
		}
	}
};

#endif // RANGESLIDER_H__
