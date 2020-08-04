#if !defined(AFX_2DLINEGRAPH_H__6E24F5B7_34EC_44F6_9B2A_2C7E899DF73F__INCLUDED_)
#define AFX_2DLINEGRAPH_H__6E24F5B7_34EC_44F6_9B2A_2C7E899DF73F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AT_LINE_DRAW_ALL		30
#define AT_LINE_DRAW_SERIES		31

#include <algorithm>
#include <vector>
#include <boost\circular_buffer.hpp>
#include "TypeDef.h"

using namespace std;
using namespace boost;

struct _2DLineGraphSegments
{
	int index;
	CString text;
	struct _2DLineGraphSegments* next;
};

class C2DLineGraph  
{
public:
	C2DLineGraph(CPoint g_position, CSize g_size, SPC_SETTING* pSpcSetting);
	~C2DLineGraph();
public:
	int m_fThreshold;
	CRect m_fThresholdRect;
	int m_index;
	int m_XIndex; //起始标签

	void SetXIndex(int Xindex);
	void SetThreshold(int th);
	void SetbFresh(bool bfresh);
	void SetControlLimitValue(int th);
	int SetThresholdPoint(int pointY);  //通道折线图获取阈值，返回阈值大小！
	void GetAxisRangeY(int& startY, int& endY);
	void GetThresholdRect(CRect &crt);
	int  GetBlowThreshold();
	void SetChIndex(int index);
	void SetLegendTextColor(COLORREF t_color);
	COLORREF GetLabelsColor();
	void SetLabelsColor(COLORREF l_color);
	void SetFullSize(CSize full_size);
	void SetGraphPosition(CPoint g_position);
	void SetGraphBkColor(COLORREF g_bkColor);
	int GetGraphAnimationPercent();
	void BuildGraph(HDC hDC);
	void ClearGraph();
	void SetAxisUnit_X(CString strUnit);
	void SetAxisUnit_Y(CString strUnit);

	void ReceiveGraphData(boost::circular_buffer<float>& dataBuffer);

private:
	COLORREF m_LegendTextColor;
	COLORREF m_LegendBackgroundColor;
	COLORREF m_LabelsColor;
	int m_SegmentsPercent;
	CPoint* m_DrawSeriesPoints;
	int m_BlendSeriesCompleted;
	int m_BlendSegmentsCompleted;
	HBITMAP m_hOldBlendBitmap;
	HBITMAP m_hBlendBitmap;
	HDC m_hBlendDC;
	BOOL m_AnimationPauseStart;
	int m_AnimationPause;
	int m_DrawSeriesCompleted;
	int m_DrawSegmentsCompleted;
	int m_AnimationType;
	int m_AnimationPercent;
	BOOL m_Animation;
	COLORREF m_GraphBkColor;
	int m_SegmentsNumber;
	CSize m_FullSize;
	CSize m_Size;
	CPoint m_Position;
	CString m_unit_X;
	CString m_unit_Y;
	int     m_nPointCount;
	CFont m_axisFont;
	CPen m_axisPen; 
	boost::circular_buffer<float> m_dataBuffer;
	float m_fMaxValue;
	SPC_SETTING* m_pSpcSetting;
	bool 	m_bfresh;
};

#endif // !defined(AFX_2DLINEGRAPH_H__6E24F5B7_34EC_44F6_9B2A_2C7E899DF73F__INCLUDED_)
