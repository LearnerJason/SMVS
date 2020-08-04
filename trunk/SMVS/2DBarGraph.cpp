#include "stdafx.h"
#include "2DBarGraph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

C2DBarGraph::C2DBarGraph()
{

}

C2DBarGraph::C2DBarGraph(CPoint g_position, CSize g_size):
		m_startX(0),
		m_endX(0)
{
	m_Position = g_position;
	m_Size = g_size;

	m_Animation = FALSE;

	m_tiltedFont.CreateFont(12, 0, -300, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial" ) );

	m_pointFont.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));

	m_GraphBkColor = RGB(255,255,2555);
	m_LabelsColor = RGB(0,0,0);
	m_LegendBackgroundColor = RGB(255,255,255);

	m_okValueColor = RGB(48,128,20);
	m_ngValueColor = RGB(227,23,13);
	m_thresholdPen.CreatePen(PS_SOLID, 4, RGB(0,0,255));
}

C2DBarGraph::~C2DBarGraph()
{
	ClearGraph();
}

BEGIN_MESSAGE_MAP(C2DBarGraph,CWnd)
END_MESSAGE_MAP()

void C2DBarGraph::ClearGraph()
{
	m_vDataPoint.clear();
}

void C2DBarGraph::SetGrahpInfo(const THRESHOLD& th)
{
	m_barGraphInfo.m_fThreshold  = th.m_specV;
	m_barGraphInfo.m_fLCL        = th.m_lcl;
	m_barGraphInfo.m_fUCL        = th.m_ucl;
	m_barGraphInfo.m_valueType   = th.m_type;
	m_barGraphInfo.m_fValueSpan  = th.m_stepLen;
	m_barGraphInfo.m_compType    = th.m_compType;
	CRect rc(0,0,0,0);
	m_barGraphInfo.m_thresholdRect = rc;
	m_barGraphInfo.m_lclRect = rc;
	m_barGraphInfo.m_uclRect = rc;
}

void C2DBarGraph::SetThresholdValue(const THRESHOLD& th)
{
	m_barGraphInfo.m_fThreshold = th.m_specV;
	m_barGraphInfo.m_fLCL = th.m_lcl;
	m_barGraphInfo.m_fUCL = th.m_ucl;
	CRect rc(0,0,0,0);
	m_barGraphInfo.m_thresholdRect = rc;
	m_barGraphInfo.m_lclRect = rc;
	m_barGraphInfo.m_uclRect = rc;
}

void C2DBarGraph::RoundOffStats(std::map<float, long>& statistics)
{
	if(m_barGraphInfo.m_valueType != VT_R4)
		return;
	std::map<float, long> dictTmp = statistics;
	statistics.clear();
	float fStep = m_barGraphInfo.m_fValueSpan;
	for(auto it = dictTmp.begin(); it != dictTmp.end();it++)
	{
		float fValue = it->first;
		fValue = int(fValue * 100) / 100.0f;
		statistics[fValue] = it->second;
	}
}

void C2DBarGraph::DisplayStatisticsData(std::map<float, long>& statistics)
{
	ClearGraph();
	RoundOffStats(statistics);
	int sz = (int)statistics.size();
	if(sz == 0) return;

	float thValue = m_barGraphInfo.m_fThreshold;
	VARENUM type = m_barGraphInfo.m_valueType;
	float fSpan = m_barGraphInfo.m_fValueSpan;

	bool bFind = false;
	long nTotal = 0;
	for(auto it = statistics.begin(); it != statistics.end(); it++)
	{
		nTotal += it->second;
	}
	if(nTotal == 0) return;

	auto bgn = *(statistics.begin());
	auto endIt = statistics.end();
	endIt--;
	auto end = *endIt;
	float fFirstValue = bgn.first;
	
	int nTickCount = (int)((end.first - bgn.first) / fSpan);
	const int nBarCount = 40;
	if(nTickCount <= nBarCount)
	{
		long nSpan = long(fSpan * 100.0f);
		std::map<long, long> tmp;
		long nBgn = int(bgn.first * 100.0f);
		long nEnd = int(end.first * 100.0f);
		for(auto it = statistics.begin(); it != statistics.end();it++)
		{
			tmp[long(it->first * 100.0f)] = it->second;
		}
		for(long l = nBgn; l <= nEnd; l += nSpan)
		{
			auto it = tmp.find(l);
			float fPercent = (it != tmp.end()) ? (1.0f * it->second / nTotal) : 0.0f;
			AddDataPoint(l / 100.0f, nSpan / 100.0f, fPercent);
		}
	}
	else
	{
		int nTickPerBar = (int)::ceil(nTickCount / (1.0 * nBarCount));
		m_barGraphInfo.m_fValueSpan = m_barGraphInfo.m_fValueSpan * nTickPerBar;

		int nTCBT = int((thValue - bgn.first) / fSpan); //TCBT : Tick Count Before Threshold
		int tailPrior = nTCBT % nTickPerBar; // bar count before threshold line
		int indexBgn = int(fFirstValue / fSpan);
		int indexSplit = indexBgn + tailPrior;
		int subTotal = 0;
		if(indexBgn != indexSplit)
		{
			for(int t = indexBgn; t < indexSplit; t++)
			{
				float tmpV = t * fSpan;
				auto it = statistics.find(tmpV);
				if(it != statistics.end())
				{
					subTotal += it->second;
				}
			}
			AddDataPoint(fFirstValue, fSpan*tailPrior, 1.0f * subTotal / nTotal);
		}
		subTotal = 0;
		float fBgnValue = 0.0f;
		for(int t = indexSplit; t * fSpan < thValue; t++)
		{
			if((t - indexSplit) % nTickPerBar == 0)
			{
				fBgnValue = t * fSpan;
			}

			float tmpV = t * fSpan;
			auto it = statistics.find(tmpV);
			if(it != statistics.end())
			{
				subTotal += it->second;
			}

			if((t - indexSplit + 1) % nTickPerBar == 0)
			{
				AddDataPoint(fBgnValue, fSpan * nTickPerBar, 1.0f * subTotal / nTotal);
				subTotal = 0;
			}
		}
		int indexEnd = (int)(thValue/ fSpan);
		float fValue = 0.0f;
		subTotal = 0;
		for(int i = indexEnd; i * fSpan <= end.first; i++)
		{
			if((i - indexEnd) % nTickPerBar == 0)
			{
				fValue = i * fSpan;
			}
			float tmpValue = i * fSpan;
			auto it = statistics.find(tmpValue);
			if(it != statistics.end())
			{
				subTotal += it->second;
			}

			if((i - indexEnd + 1) % nTickPerBar == 0)
			{
				AddDataPoint(fValue, fSpan * nTickPerBar, 1.0f * subTotal / nTotal);
				subTotal = 0;
			}
		}
		if(subTotal  != 0 && fValue != 0.0f)
		{
			AddDataPoint(fValue, fSpan * nTickPerBar, 1.0f * subTotal / nTotal);
		}
	}
	//delete all data points which y value is zero
	std::vector<BarGraphDataPoint> tmpPtArray = m_vDataPoint;
	m_vDataPoint.clear();
	auto itBgn = std::find_if(tmpPtArray.begin(), tmpPtArray.end(), [&](BarGraphDataPoint pt) -> bool
	{
		return pt.m_yValue > 0.0f;
	});

	auto rItEnd = std::find_if(tmpPtArray.rbegin(), tmpPtArray.rend(), [&](BarGraphDataPoint pt) -> bool
	{
		return pt.m_yValue > 0.0f;
	});
	auto itEnd = rItEnd.base();
	
	__int64 nDis = std::distance(itBgn, itEnd);
	if(itBgn != tmpPtArray.end() && nDis >= 0)
	{
		m_vDataPoint.assign(itBgn, itEnd);
	}
}

void C2DBarGraph::GetAxisRangeX(int& startX, int& endX,float& fValueRange)
{
	startX      = m_startX;
	endX        = m_endX;
	fValueRange = m_fValueRange;
}

void C2DBarGraph::AddDataPoint(float xValue, float xSpan, float yValue)
{
	BarGraphDataPoint dataPoint(xValue, xSpan, yValue);
	m_vDataPoint.push_back(dataPoint);
}

BarGraphInfo* C2DBarGraph::GetBarGraphInfoPtr()
{
	return &m_barGraphInfo;
}

COLORREF C2DBarGraph::SelectBarColor(COMPARE_TYPE compType, float xValue)
{
	float fThreshold = m_barGraphInfo.m_fThreshold;
	COLORREF color;
	if(compType == GREAT || compType == GREAT_EQUAL)
	{
		color = (xValue >= fThreshold) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == LESS || compType == LESS_EQUAL)
	{
		color = (xValue < fThreshold) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == IN_RANGE_CLOSE)
	{
		color = (xValue >= m_barGraphInfo.m_fLCL && xValue <= m_barGraphInfo.m_fUCL) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == IN_RANGE_LC_RO)
	{
		color = (xValue >= m_barGraphInfo.m_fLCL && xValue < m_barGraphInfo.m_fUCL) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == IN_RANGE_LO_RC)
	{
		color = (xValue > m_barGraphInfo.m_fLCL && xValue <= m_barGraphInfo.m_fUCL) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == IN_RANGE_OPEN)
	{
		color = (xValue > m_barGraphInfo.m_fLCL && xValue < m_barGraphInfo.m_fUCL) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == OUT_RANGE_CLOSE)
	{
		color = (xValue <= m_barGraphInfo.m_fLCL || xValue >= m_barGraphInfo.m_fUCL) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == OUT_RANGE_LC_RO)
	{
		color = (xValue <= m_barGraphInfo.m_fLCL || xValue > m_barGraphInfo.m_fUCL) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == OUT_RANGE_LO_RC)
	{
		color = (xValue < m_barGraphInfo.m_fLCL || xValue >= m_barGraphInfo.m_fUCL) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == OUT_RANGE_OPEN)
	{
		color = (xValue < m_barGraphInfo.m_fLCL || xValue > m_barGraphInfo.m_fUCL) ? m_okValueColor : m_ngValueColor;
	}
	else if(compType == EQUAL)
	{
		color = (xValue == fThreshold) ? m_okValueColor : m_ngValueColor;
	}
	else
	{
		color = (xValue != fThreshold) ? m_okValueColor : m_ngValueColor;
	}
	return color;
}

void C2DBarGraph::BuildGraph(HDC hDC)
{
	int ptCount = (int)m_vDataPoint.size();
	m_barGraphInfo.m_nBarCount = ptCount;
	if(m_barGraphInfo.m_nBarCount == 0) return;
	int chartWidth = int(m_Size.cx*0.98);
	// Calculate graph x and y coordinate
	int startX = m_Position.x + int(m_Size.cx*0.018);
	int startY = m_Position.y + int(m_Size.cy*0.94) - 4;
	int endX = m_Size.cx  + 15;
	int endY = m_Position.y + int(m_Size.cy*0.05);
	// Draw x coordinate axis
	Graphics graphics(hDC);
	Pen myPen(Color::Black, 2);
	graphics.DrawLine(&myPen, startX, startY, endX, startY);
	graphics.DrawLine(&myPen,endX, startY,endX - 8, startY - 4);
	graphics.DrawLine(&myPen,endX, startY,endX - 8, startY + 4);
	// Draw y coordinate axis
	graphics.DrawLine(&myPen,startX, startY,startX, endY);
	graphics.DrawLine(&myPen,startX, endY,startX - 4, endY + 8);
	graphics.DrawLine(&myPen,startX, endY,startX + 4, endY + 8);

	HFONT hOldValFont = (HFONT)::SelectObject(hDC, m_pointFont.GetSafeHandle());
	::SetBkMode(hDC, TRANSPARENT);
	::SetTextAlign(hDC, TA_RIGHT);
	COLORREF oldTextColor = ::SetTextColor(hDC, m_LabelsColor);

	FontFamily fontF(L"Arial");
	Gdiplus::Font font(&fontF,12, FontStyleBold, UnitPixel);
	graphics.TranslateTransform(0, font.GetHeight(0.0f));
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics.DrawString(L"%", -1, &font,PointF(REAL(m_Position.x - 8), REAL(m_Position.y - 10)),&SolidBrush(Color::Black));

	if(m_Animation != FALSE) return;
	if(m_vDataPoint.empty()) return;

	m_barGraphInfo.m_nBarWidth = int((chartWidth - 2 * 5) * 1.0/m_barGraphInfo.m_nBarCount);
	CPoint startPt = CPoint(startX + 15, startY);
	m_startX       = startX + 15;
	m_endX         = endX;

	float maxValueY = -1000000.0f;
	float maxValueX = -1000000.0f;
	float minValueX = 9999999999.9f;
	for(int t = 0; t < ptCount; t++)
	{
		float valueY = m_vDataPoint[t].m_yValue;
		float valueX = m_vDataPoint[t].m_xValue;
		if(valueY > maxValueY)
			maxValueY = valueY;
		if(valueX > maxValueX)
			maxValueX = valueX;
		if(valueX < minValueX)
			minValueX = valueX;
	}
	m_fValueRange = maxValueX - minValueX;

	CString strMax;
	int maxPercent = (int)::ceilf(maxValueY * 100.0f);
	strMax.Format(_T("%d"), maxPercent);
	int xText = startX-int(m_Size.cx*0.01) + 6;
	::TextOut(hDC, xText, startPt.y-int(m_Size.cy*0.85), strMax, strMax.GetLength());

	strMax.Format(_T("%d"), int(maxPercent / 2.0f));
	::TextOut(hDC, xText, startPt.y-int(m_Size.cy*0.45), strMax, strMax.GetLength());

	strMax = _T("0");
	::TextOut(hDC, xText, startPt.y-int(m_Size.cy*0.05), strMax, strMax.GetLength());

	::SetTextAlign(hDC, TA_LEFT);
	::SetBkMode(hDC, OPAQUE);
	::SelectObject(hDC, hOldValFont);

	float fThreshold = m_barGraphInfo.m_fThreshold;
	float fSpan = m_barGraphInfo.m_fValueSpan;
	if(fThreshold >= maxValueX + fSpan)
	{
		fThreshold = maxValueX + fSpan;
		m_barGraphInfo.m_fThreshold = fThreshold;
	}
	m_barGraphInfo.m_fMaxValueX = maxValueX + fSpan;

	int nBarWidth = m_barGraphInfo.m_nBarWidth;
	COMPARE_TYPE compType = m_barGraphInfo.m_compType;

	int nDeltaY = int(m_Size.cy*0.01) + 1;

	CString strMaxX;
	bool bTiltedText = false;
	if(m_barGraphInfo.m_valueType == VT_I4)
	{
		strMaxX.Format(_T("%d"), (int)maxValueX);
		int nLen = strMaxX.GetLength();
		if(nLen == 3 && ptCount >= 30) bTiltedText = true;
		if(nLen == 4 && ptCount >= 20) bTiltedText = true;
		if(nLen >= 5 && ptCount >= 15) bTiltedText = true;
	}
	else
	{
		if(m_barGraphInfo.m_fValueSpan >= 0.1)
		{
			strMaxX.Format(_T("%.1f"), maxValueX);
		}
		else
		{
			strMaxX.Format(_T("%.2f"), maxValueX);
		}
		
		int nLen = strMaxX.GetLength();
		if(nLen == 4 && ptCount >= 30) bTiltedText = true;
		if(nLen == 5 && ptCount >= 20) bTiltedText = true;
		if(nLen >= 6 && ptCount >= 15) bTiltedText = true;
	}

	float fLcl = m_barGraphInfo.m_fLCL;
	float fUcl = m_barGraphInfo.m_fUCL;
	for(int index = 0; index < ptCount; index++)
	{
		BarGraphDataPoint point  = m_vDataPoint[index];
		BarGraphDataPoint nextPt = point;
		if(index != ptCount -1)
			nextPt = m_vDataPoint[index + 1];

		CString strX, strY;
		if(m_barGraphInfo.m_valueType == VT_I4)
		{
			strX.Format(_T("%d"), (int)point.m_xValue);
		}
		else
		{
			if(m_barGraphInfo.m_fValueSpan >= 0.1)
			{
				strX.Format(_T("%.1f"), point.m_xValue);
			}
			else
			{
				strX.Format(_T("%.2f"), point.m_xValue);
			}
		}
		strY.Format(_T("%.1f"), point.m_yValue);

		HFONT hOldSegFont;
		if(bTiltedText)
			hOldSegFont = (HFONT)::SelectObject(hDC, m_tiltedFont.GetSafeHandle());
		else
			hOldSegFont = (HFONT)::SelectObject(hDC, m_pointFont.GetSafeHandle());
		::SetBkMode(hDC, TRANSPARENT);
		::SetTextAlign(hDC, TA_CENTER);
		int nLen = strX.GetLength();
		::TextOut(hDC, startPt.x + int(nBarWidth/2.0), startY+nDeltaY, strX, strX.GetLength());
		::SetTextAlign(hDC, TA_LEFT);
		::SetBkMode(hDC, OPAQUE);
		::SelectObject(hDC, hOldSegFont);

		int barHeight = int(point.m_yValue/maxValueY * m_Size.cy*0.8);

		HBRUSH hSpecialBrush;
		HBRUSH hOldSpeBrush;
		int nSplit = 0;
		COLORREF color;
		float fBarEnd = point.m_xValue + point.m_xValueSpan;
		float fActualBarWidth = float(nBarWidth * (point.m_xValueSpan / fSpan));

		bool bSplit = false;
		float fRatio = 0.0f;
		if(compType <= LESS_EQUAL && fThreshold > point.m_xValue && fThreshold <= fBarEnd)
		{
			bSplit = true;
			fRatio = (fThreshold - point.m_xValue) / point.m_xValueSpan;
			nSplit = int(fRatio * fActualBarWidth);
		}
		if(compType > LESS_EQUAL && fLcl > point.m_xValue && fLcl <= fBarEnd)
		{
			bSplit = true;
			fRatio = (fLcl - point.m_xValue) / point.m_xValueSpan;
			nSplit = int(fRatio * fActualBarWidth);
		}
		if(compType > LESS_EQUAL && fUcl > point.m_xValue && fUcl <= fBarEnd)
		{
			bSplit = true;
			fRatio = (fUcl - point.m_xValue) / point.m_xValueSpan;
			nSplit = int(fRatio * fActualBarWidth);
		}
		if(bSplit)
		{
			color = SelectBarColor(compType, point.m_xValue + 0.5f*fRatio*fSpan);
			hSpecialBrush = ::CreateSolidBrush(color);
			hOldSpeBrush = (HBRUSH)::SelectObject(hDC, hSpecialBrush);
			::Rectangle(hDC, startPt.x, startPt.y - barHeight, startPt.x + nSplit, startPt.y);
			::SelectObject(hDC, hOldSpeBrush);
			::DeleteObject(hSpecialBrush);

			color = SelectBarColor(compType, point.m_xValue + fRatio*fSpan + 1.0f);
			hSpecialBrush = ::CreateSolidBrush(color);
			hOldSpeBrush = (HBRUSH)::SelectObject(hDC, hSpecialBrush);
			::Rectangle(hDC, startPt.x + nSplit, startPt.y - barHeight, int(startPt.x + fActualBarWidth), startPt.y);
			::SelectObject(hDC, hOldSpeBrush);
			::DeleteObject(hSpecialBrush);
		}
		else
		{		
			color = SelectBarColor(compType, point.m_xValue) ;
			HBRUSH hBrush = ::CreateSolidBrush(color);
			HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
			::Rectangle(hDC, startPt.x, startPt.y - barHeight, int(startPt.x + fActualBarWidth), startPt.y);
			::SelectObject(hDC, hOldBrush);
			::DeleteObject(hBrush);
		}
		// Draw threshold line
		CPoint ptBgn, ptEnd;
		ptBgn.y = m_Position.y + 5;
		ptEnd.y = startY + 15;

		if(m_barGraphInfo.m_compType <= LESS_EQUAL)
		{

			if((fThreshold > point.m_xValue )&& ((point.m_xValue + point.m_xValueSpan -fThreshold )>= -0.00001f))
			{
				float ratio = (fThreshold - point.m_xValue) / point.m_xValueSpan;
				ptBgn.x = ptEnd.x = int(startPt.x + ratio * fActualBarWidth);

				HPEN oldPen = (HPEN)::SelectObject(hDC, m_thresholdPen.GetSafeHandle());
				::MoveToEx(hDC, ptBgn.x, ptBgn.y, NULL);
				::LineTo(hDC, ptEnd.x, ptEnd.y);
				CRect rc;
				rc.left = ptBgn.x - 15;
				rc.top  = ptBgn.y;
				rc.right = ptBgn.x + 15;
				rc.bottom = ptEnd.y;
				m_barGraphInfo.m_thresholdRect = rc;
				::SelectObject(hDC, oldPen);
			}
		}
		else
		{
			if(fLcl > point.m_xValue && fLcl <= point.m_xValue + point.m_xValueSpan)
			{
				float ratio = (fLcl - point.m_xValue) / point.m_xValueSpan;
				ptBgn.x = ptEnd.x = int(startPt.x + ratio * fActualBarWidth);
				HPEN oldPen = (HPEN)::SelectObject(hDC, m_thresholdPen.GetSafeHandle());
				::MoveToEx(hDC, ptBgn.x, ptBgn.y, NULL);
				::LineTo(hDC, ptEnd.x, ptEnd.y);
				CRect rc;
				rc.left = ptBgn.x - 15;
				rc.top  = ptBgn.y;
				rc.right = ptBgn.x + 15;
				rc.bottom = ptEnd.y;
				m_barGraphInfo.m_lclRect = rc;
				::SelectObject(hDC, oldPen);
			}
			if(fUcl > point.m_xValue && fUcl <= point.m_xValue + point.m_xValueSpan)
			{
				float ratio = (fUcl - point.m_xValue) / point.m_xValueSpan;
				ptBgn.x = ptEnd.x = int(startPt.x + ratio * fActualBarWidth);
				HPEN oldPen = (HPEN)::SelectObject(hDC, m_thresholdPen.GetSafeHandle());
				::MoveToEx(hDC, ptBgn.x, ptBgn.y, NULL);
				::LineTo(hDC, ptEnd.x, ptEnd.y);
				CRect rc;
				rc.left = ptBgn.x - 15;
				rc.top  = ptBgn.y;
				rc.right = ptBgn.x + 15;
				rc.bottom = ptEnd.y;
				m_barGraphInfo.m_uclRect = rc;
				::SelectObject(hDC, oldPen);
			}
		}

		if(point.m_xValueSpan == fSpan)
		{
			startPt.x += nBarWidth;
		}
		else
		{
			startPt.x += (int)fActualBarWidth;
		}
	}
	::SetTextColor(hDC, oldTextColor);
}

int C2DBarGraph::GetGraphAnimationPercent()
{
	return m_AnimationPercent;
}

void C2DBarGraph::SetGraphBkColor(COLORREF g_bkColor)
{
	// Set graph background color
	m_GraphBkColor = g_bkColor;
}

void C2DBarGraph::SetGraphPosition(CPoint g_position)
{
	// Set new graph position
	m_Position = g_position;
}

void C2DBarGraph::SetFullSize(CSize full_size)
{
	// Set full size
	m_FullSize = full_size;
}

void C2DBarGraph::SetLabelsColor(COLORREF l_color)
{
	// Set graph labels color
	m_LabelsColor = l_color;
}

COLORREF C2DBarGraph::GetLabelsColor()
{
	return m_LabelsColor;
}
