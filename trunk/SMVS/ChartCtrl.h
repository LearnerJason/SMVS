#if !defined(AFX_GRAPHOBJECT_H__45515655_438B_444F_A8F0_77DAC449F81B__INCLUDED_)
#define AFX_GRAPHOBJECT_H__45515655_438B_444F_A8F0_77DAC449F81B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2DBarGraph.h"
#include "2DLineGraph.h"
#include "TypeDef.h"
#include <map>
#include <math.h>
#include <boost\circular_buffer.hpp>

using namespace std;
using namespace boost;

#define GT_2DBAR		0
#define GT_2DLINE		1

#define IDT_ANIMATION	50000
#define ANIMATION_TIME	50

#define GB_SOLID		10
#define GB_GRADIENT		11
#define FT_HORIZONTAL	20
#define FT_VERTICAL		21

#define IDC_HAND MAKEINTRESOURCE(32649)

class CChartCtrl : public CWnd
{
public:
	CChartCtrl();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	void ClearGraph();
	void ShowGraphLegend( BOOL legend );
	void SetGraphTitleColor( COLORREF t_color );
	void SetGraphSubtitleShadow( BOOL s_shadow );
	void SetGraphTitleShadow( BOOL t_shadow );
	void Add2DLineGraphSegment( CString s_text );

	void Add2DBarGraphDataPoint(float xValue, float xSpan, float yValue);
	void SetGrahpInfo(const THRESHOLD& th);
	void SetThresholdValue(const THRESHOLD& th);
	void SetBlowThresholdValue(int &iBthreshold);
	int GetBlowThresholdValue();
	void DisplayStatisticsData(std::map<float, long>& statistics);

	void SetGraphPosition( CPoint g_position );
	void SetGraphSubtitle( CString g_subtitle );
	void SetGraphTitle( CString g_title );
	COLORREF GetGraphBackgroundColor();
	void SetGraphBackgroundColor(COLORREF g_bgcolor);
	void CreateGraph(int g_type, SPC_SETTING* pSpcSetting = nullptr);
	void SetChIndex(int index);
	void CreateLineGraph(CRect& rcClient, int nXLen, wstring strXUnit, wstring strYUnit);
	virtual ~CChartCtrl();

	void Receive2DLineGraphData(boost::circular_buffer<float>& dataBuffer);
	void Receive2DLineGraphData(std::vector<float>& vDataPoint);
	void SetXIndex(int xIndex);
	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_ShowLegend;
	COLORREF m_GraphSubtitleColor;
	COLORREF m_GraphTitleColor;
	BOOL m_GraphSubtitleShadow;
	BOOL m_GraphTitleShadow;
	DWORD m_GraphGradientFillType;
	COLORREF m_GraphGradientColor2;
	COLORREF m_GraphGradientColor1;
	int m_GraphFillType;
	BOOL m_GraphAnimation;
	CFont m_GraphSubtitleFont;
	CString m_GraphSubtitle;
	CBitmap* m_GradientBitmap;
	CDC* m_GradientDC;
	CBitmap* m_GraphBitmap;
	CDC* m_GraphDC;
	BOOL m_Update;
	CFont m_GraphTitleFont;
	CString m_GraphTitle;
	COLORREF m_GraphBackgroundColor;
	int m_GraphType;
	CSize m_Size;
	CPoint m_Position;
	C2DLineGraph* m_2DLineGraph;
	C2DBarGraph* m_2DBarGraph;
	BarGraphInfo* m_pBarGraphInfo;
	bool m_bLBtnDown;
	bool m_bThresholdChanged;
	bool m_bLclChanged;
	bool m_bUclChanged;
	CPoint m_bgnPt;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif // !defined(AFX_GRAPHOBJECT_H__45515655_438B_444F_A8F0_77DAC449F81B__INCLUDED_)
