#pragma once

/********************************************************************
 * File name      : RangeSlider.h
 * Create time    : 2013-10-08 
 * Brief          : ˫���黬�����ؼ�������ѡ��Χ��������
 * Detail         : �ɼ��ض��ָ�ʽͼƬ��bmp��jpg��png�ȣ�������Դ�����ļ�·�����ض����ԣ�����λ�øı�ʱ����֪ͨ�����ڣ�֧����ֱ������Ҫ����ԴΪ��ֱ�ģ�
 * Other          : ����ʱ�ɶ�̬�������ɾ�̬������
 *                1����̬������
 *                     �ӹ������������һ���ؼ������棬�󶨱������޸ı�������ΪCRangeSlider�����ü���channel��thumbͼƬ�Ľӿڣ������߼�ֵ��Χ���ɣ�
 *                2����̬������
 *                     ��ͷ�ļ�������CRangeSlider���ͱ�������ʵ���ļ��ĳ�ʼ�������У�����CreateWnd�������ڣ����ż���ͼƬ�����÷�Χ���ɣ�
 * Modify history :
 * .................2013-10-10 detail:�������м����ַŴ���Сѡ��ķ�Χ
 * .................2013-10-12 detail:��Ӷ�͸��ͼƬ��alphaͨ�������������ں�������ID����
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
	//���ڴ��б���ͼ��
	CImage m_imageChannel;
	CImage m_imageThumb;

	bool m_bCurrentBlock;  //��¼��ǰ�϶�����  flase:min  true:max

	//�ؼ��߽��Ӧ���߼���Сֵ�����ֵ
	int m_nMin;
	int m_nMax;

	//��Ƿ�Χ��˫������߼���Сֵ�����ֵ
	int m_nThumbMin;
	int m_nThumbMax;
	bool m_bThumbMinChanged;
	bool m_bThumbMaxChanged;

	//�ؼ��ĳߴ�
	int m_nWidth;
	int m_nHeight;

	//�ؼ��߾ࣨ�൱�ڻ���ռ�õĿռ䣩
	int m_nMargin;

	//�϶�����ǰ����¼����뻬��֮����������
	int m_nPt;

	//�Ƿ��϶���Сֵ�����ֵ����
	bool m_bDragMin;
	bool m_bDragMax;

	//�Ƿ�������м��Ŵ���С��ֵ���
	bool m_bEnableMouseWheel;

	//�ؼ��Ƿ���ֱ
	bool m_bVertical;

	bool m_bSingleSlider;

	//����ת��ʱҪ���ǻ���ռ�ݵ�λ��
	//����λ��ת��Ϊ�߼�λ��
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

	//�߼�λ��ת��Ϊ����λ��
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
