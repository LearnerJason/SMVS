#if !defined(AFX_2DBARGRAPH_H__62AE2E10_96DE_4410_87A1_2D2CDE38A601__INCLUDED_)
#define AFX_2DBARGRAPH_H__62AE2E10_96DE_4410_87A1_2D2CDE38A601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include <algorithm>
#include "TypeDef.h"
#include <GdiPlus.h>

#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

#define AT_BAR_DRAW_ALL			20
#define AT_BAR_DRAW_SEGMENT		21
#define AT_BAR_DRAW_SERIES		22
#define AT_BAR_BLEND_SEGMENT	25
#define AT_BAR_BLEND_SERIES		26

struct BarGraphDataPoint
{
	BarGraphDataPoint():m_xValue(0.0f),m_xValueSpan(1.0f), m_yValue(0.0f)
	{
	}
	BarGraphDataPoint(float xValue, float xSpan, float yValue):m_xValue(xValue), m_xValueSpan(xSpan), m_yValue(yValue)
	{
	}
	BarGraphDataPoint(const BarGraphDataPoint& other)
	{
		m_xValue = other.m_xValue;
		m_xValueSpan = other.m_xValueSpan;
		m_yValue = other.m_yValue;
	}
	BarGraphDataPoint(BarGraphDataPoint&& other)
	{
		m_xValue = other.m_xValue;
		m_xValueSpan = other.m_xValueSpan;
		m_yValue = other.m_yValue;
	}
	BarGraphDataPoint& operator=(const BarGraphDataPoint& other)
	{
		if(this == &other)
			return *this;
		m_xValue = other.m_xValue;
		m_xValueSpan = other.m_xValueSpan;
		m_yValue = other.m_yValue;
		return *this;
	}
	BarGraphDataPoint& operator=(BarGraphDataPoint&& other)
	{
		if(this == &other)
			return *this;
		m_xValue = other.m_xValue;
		m_xValueSpan = other.m_xValueSpan;
		m_yValue = other.m_yValue;
		return *this;
	}
	float   m_xValue; // X axis value,
	float   m_xValueSpan; 
	float   m_yValue; // Y axis value, it is a percent value
};

class C2DBarGraph : public CWnd
{
public:
	C2DBarGraph();
	C2DBarGraph(CPoint g_position, CSize g_size);
	 ~C2DBarGraph();
	COLORREF GetLabelsColor();
	void SetLabelsColor(COLORREF l_color);
	void SetFullSize(CSize full_size);
	void AddDataPoint(float xValue, float xSpan, float yValue);
	void SetGraphPosition(CPoint g_position);
	void SetGraphBkColor(COLORREF g_bkColor);
	int GetGraphAnimationPercent();
	void GetAxisRangeX(int& startX, int& endX,float& fValueRange);
	void BuildGraph(HDC hDC);
	void ClearGraph();
	void SetGrahpInfo(const THRESHOLD& th);
	void SetThresholdValue(const THRESHOLD& th);
	void DisplayStatisticsData(std::map<float, long>& statistics);
	BarGraphInfo* GetBarGraphInfoPtr();
	COLORREF SelectBarColor(COMPARE_TYPE compType, float xValue);
	void RoundOffStats(std::map<float, long>& statistics);
protected:
	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_LegendBackgroundColor;
	COLORREF m_LabelsColor;
	BOOL m_AnimationPauseStart;
	int m_AnimationPause;
	int m_DrawSeriesCompleted;
	int m_DrawSegmentsCompleted;
	int m_AnimationType;
	int m_AnimationPercent;
	BOOL m_Animation;
	COLORREF m_GraphBkColor;
	CFont m_tiltedFont;
	CFont m_pointFont;
	CSize m_FullSize;
	CSize m_Size;
	CPoint m_Position;

	std::vector<BarGraphDataPoint> m_vDataPoint;
	COLORREF m_okValueColor;
	COLORREF m_ngValueColor;
	CPen       m_thresholdPen;
	BarGraphInfo m_barGraphInfo;
	int   m_startX;
	int   m_endX;
	float m_fValueRange;
};

#endif
