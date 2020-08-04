#include "stdafx.h"
#include "2DLineGraph.h"
#include <math.h>
#include "SystemManager.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#pragma warning(disable : 4244)

extern CSystemManager gSystemMgr;
C2DLineGraph::C2DLineGraph(CPoint g_position, CSize g_size, SPC_SETTING* pSpcSetting):
        m_dataBuffer(pSpcSetting->m_nXLen + 1),
		m_fMaxValue(0.0f),
		m_pSpcSetting(pSpcSetting),
		m_fThreshold(3),
		m_index(0),
		m_XIndex(0)
{
	// Set graph position and size
	m_Position = g_position;
	m_Size = g_size;

	// Set graph segments and series number
	m_SegmentsNumber = 0;

	// Set default graph background color
	m_GraphBkColor = RGB(255,255,2555);

	// Set default graph labels color
	m_LabelsColor = RGB(0,0,0);

	// Set default graph legend background color
	m_LegendBackgroundColor = RGB(255,255,255);
	m_LegendTextColor = RGB(0,0,0);

	// Set default graph draw series points
	m_DrawSeriesPoints = NULL;

	m_unit_Y       = pSpcSetting->m_strUnitY.c_str();
	m_nPointCount  = pSpcSetting->m_nXLen;
	m_unit_X       = pSpcSetting->m_strUnitX.c_str();
	m_axisFont.CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 
		FALSE,DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));

	m_axisPen.CreatePen(PS_SOLID, 1, RGB(20, 20, 20));
}

C2DLineGraph::~C2DLineGraph()
{
	ClearGraph();

	// Delete series points
	delete []m_DrawSeriesPoints;
}

void C2DLineGraph::ClearGraph()
{
	m_SegmentsNumber = 0;
	m_dataBuffer.clear();
}

void C2DLineGraph::GetAxisRangeY(int& startY, int& endY)
{
	startY = m_Position.y + int(m_Size.cy*0.92) - 3;
	endY = m_Position.y + 3;

}
void C2DLineGraph::GetThresholdRect(CRect &crt)
{
	crt = m_fThresholdRect;
}
int C2DLineGraph::SetThresholdPoint(int pointY)
{

	int startY = m_Position.y + int(m_Size.cy*0.92) - 3;
	int endY   = m_Position.y + 3;
	int yLen = startY - endY;
	m_fThreshold =(int)((float)(startY - pointY)/yLen *m_fMaxValue + 0.5f);
	return m_fThreshold;
}

void C2DLineGraph::SetbFresh(bool bfresh)
{
	m_bfresh = bfresh;
}
void C2DLineGraph::SetXIndex(int Xindex)
{
	m_XIndex = Xindex;
}
void C2DLineGraph::SetChIndex(int index)
{
	m_index = index;
}
int C2DLineGraph::GetBlowThreshold()
{
	return m_fThreshold;
}
void C2DLineGraph::SetControlLimitValue(int th)
{
	m_fThreshold = th;
}	
void C2DLineGraph::ReceiveGraphData(boost::circular_buffer<float>& dataBuffer)
{
	m_dataBuffer.clear();
	m_dataBuffer = dataBuffer;
}

void C2DLineGraph::BuildGraph(HDC hDC)
{
	int sz = (int)m_dataBuffer.size();
	
	int startX = m_Position.x + 30;
	int endX   = m_Size.cx - 15;
	int xLen = endX - startX;
	int xStep = int(1.0f * xLen / m_nPointCount);

	int startY = m_Position.y + int(m_Size.cy*0.92) - 3;
	int endY   = m_Position.y + 3;
	int yLen = startY - endY;

	HPEN hOldPen = (HPEN)::SelectObject(hDC, m_axisPen.GetSafeHandle());
	HFONT hOldFont = (HFONT)::SelectObject(hDC, m_axisFont.GetSafeHandle());
	// X axis
	::MoveToEx(hDC, startX, startY, NULL);
	::LineTo(hDC, endX + 5, startY);
	::MoveToEx(hDC, endX + 5, startY, NULL);
	::LineTo(hDC, endX-3, startY-3);
	::MoveToEx(hDC, endX + 5, startY, NULL);
	::LineTo(hDC, endX-3, startY+3);

	::SetBkMode(hDC, TRANSPARENT);
	::SetTextAlign(hDC, TA_RIGHT);
	::SetTextColor(hDC, RGB(255,0,0));
	::TextOut(hDC, m_Size.cx + 30, startY + 4, m_unit_X, m_unit_X.GetLength());
		// Y axis
	::MoveToEx(hDC, startX, startY, NULL);
	::LineTo(hDC, startX, endY - 8);
	::MoveToEx(hDC, startX, endY - 8, NULL);
	::LineTo(hDC, startX + 3, endY);
	::MoveToEx(hDC, startX, endY - 8, NULL);
	::LineTo(hDC, startX - 3, endY);
	
	::SetBkMode(hDC, TRANSPARENT);
	::SetTextAlign(hDC, TA_RIGHT);
	::SetTextColor(hDC, RGB(255,0,0));
	::TextOut(hDC, startX - 6, m_Position.y - 10, m_unit_Y, m_unit_Y.GetLength());

	::SelectObject(hDC, hOldPen);
	::SelectObject(hDC, hOldFont);
	

	::SetTextColor(hDC, RGB(0,0,0));
	::SetTextAlign(hDC, TA_LEFT);
	::SetBkMode(hDC, OPAQUE);

	CPoint xPt(startX + xStep, startY);
	hOldFont = (HFONT)::SelectObject(hDC, m_axisFont.GetSafeHandle());
	CPen gridPen(PS_SOLID, 1, RGB(10,10,250));
	hOldPen = (HPEN)::SelectObject(hDC, gridPen.GetSafeHandle());
	for(int i = 0; i < m_nPointCount; i++)
	{
		::SetBkMode(hDC, TRANSPARENT);
		::SetTextAlign(hDC, TA_CENTER);
		CString strIndex;
		strIndex.Format(_T("%d"), i + 1);
		::TextOut(hDC, xPt.x, startY+3, strIndex, strIndex.GetLength());

		::SetTextAlign(hDC, TA_LEFT);
		::SetBkMode(hDC, OPAQUE);

		::MoveToEx(hDC, xPt.x, startY, NULL);
		::LineTo(hDC, xPt.x, endY);

		xPt.x += xStep;
	}
	::SelectObject(hDC, hOldFont);
	::SelectObject(hDC, hOldPen);

	::SetTextColor(hDC, RGB(0,0,0));
	if(sz > 0)
	{
		std::vector<float> vData(m_dataBuffer.begin(), m_dataBuffer.end());
		float maxValue = *(std::max_element(vData.begin(), vData.end()));

		if((maxValue > m_fThreshold))
		{
			m_fMaxValue = maxValue;
		}
	}
	else
		m_fMaxValue = 3;

	CString strMax;
	float fMax =m_fMaxValue;
	if(fMax  < 3.0f)
	{
		fMax = 3.0f;
	}
	int Tmax = (int)((float)m_fThreshold /5 *6 +0.5f);

	if(Tmax >= fMax)
	{
		fMax = Tmax;
		m_fMaxValue =fMax;
	}
	CString strMin;
	float fMin = 0.0f;

	hOldFont = (HFONT)::SelectObject(hDC, m_axisFont.GetSafeHandle());
	hOldPen = (HPEN)::SelectObject(hDC, gridPen.GetSafeHandle());
	::SetBkMode(hDC, TRANSPARENT);
	::SetTextAlign(hDC, TA_RIGHT);
	strMax.Format(_T("%d"), (int)(fMax+0.5f));
	::TextOut( hDC, startX-6, endY, strMax, strMax.GetLength());
	::MoveToEx(hDC, startX, endY, NULL);
	::LineTo(hDC, endX, endY);

	CString strMid;
	strMid .Format(_T("%d"), int(fMin + 2*(fMax-fMin)/3.0f+0.5f));
	::TextOut( hDC, startX-6, endY + int(yLen / 3.0f), strMid, strMid.GetLength());
	::MoveToEx(hDC, startX, endY + int(yLen / 3.0f), NULL);
	::LineTo(hDC, endX, endY + int(yLen / 3.0f));
	strMid .Format(_T("%d"), int(fMin + (fMax-fMin)/3.0f+0.5f));

	::TextOut( hDC, startX-6, endY + int(2.0f * yLen / 3.0f), strMid, strMid.GetLength());
	::MoveToEx(hDC, startX, endY + int(2.0f * yLen / 3.0f), NULL);
	::LineTo(hDC, endX, endY + int(2.0f * yLen / 3.0f));

	strMin.Format(_T("%d"), (int)fMin);
	::TextOut( hDC, startX-6, startY, strMin, strMin.GetLength());

	if (m_index > 0)
	{
		int y = m_Position.y + 3;

		CPen ThresholdPen(PS_SOLID, 1, RGB(255, 0, 0)); 
		hOldPen = (HPEN)::SelectObject(hDC, ThresholdPen.GetSafeHandle());

		//阈值坐标
		CPoint point1(startX+1,y +yLen - (float)m_fThreshold /fMax* yLen-4);
		CPoint point2(startX+10,y +yLen - (float)m_fThreshold /fMax* yLen);
		CPoint point3(startX+1,y +yLen - (float)m_fThreshold /fMax* yLen+4);

		//阈值游标
		CBrush brush1;
		brush1.CreateSolidBrush(RGB(255, 0, 0));
		::SelectObject(hDC,brush1);    
		::BeginPath(hDC);  
		::MoveToEx(hDC,point1.x,point1.y,NULL);  
		::LineTo(hDC,point2.x,point2.y);  
		::LineTo(hDC,point3.x,point3.y);  
		::LineTo(hDC,point1.x,point1.y);  
		::EndPath(hDC);  
		::FillPath(hDC);


		CPen linePen1(PS_DASH, 1, RGB(255, 0, 0)); 
		hOldPen = (HPEN)::SelectObject(hDC, linePen1.GetSafeHandle());

		::MoveToEx(hDC, startX+1, y +yLen - (float)m_fThreshold /fMax* yLen, NULL);
		::LineTo(hDC, endX, y +yLen - (float)m_fThreshold /fMax* yLen );

		::SetTextColor(hDC, RGB(255,0,0));
		CString strthreshold;
		strthreshold .Format(_T("%d"), m_fThreshold);
		::TextOut( hDC, endX+12, y +yLen - (float)m_fThreshold /fMax* yLen-8, strthreshold, strthreshold.GetLength());

		//阈值响应区域
		m_fThresholdRect.left = startX;
		m_fThresholdRect.right = endX;
		m_fThresholdRect.top = y +yLen - (float)m_fThreshold /fMax* yLen -3;
		m_fThresholdRect.bottom = y +yLen - (float)m_fThreshold /fMax* yLen +3;

		::SelectObject(hDC, hOldFont);
		::SelectObject(hDC, hOldPen);
	}
	CPen linePen(PS_SOLID, 1, RGB(50, 50, 50)); 
	hOldPen = (HPEN)::SelectObject(hDC, linePen.GetSafeHandle());

	CBrush brush, *pOldBrush;
	brush.CreateSolidBrush(RGB(255, 0, 0));
	pOldBrush = (CBrush*)::SelectObject(hDC, brush.GetSafeHandle());

	if(sz <= 0) return;
	std::vector<float> vPoints(m_dataBuffer.begin(), m_dataBuffer.end());

	//画点和折线
	//	CPen ThresholdPen;
	CBrush ThresholdBrush;

	CPoint dataPt;
	dataPt.x = (sz < m_nPointCount + 1) ? (startX + xStep) : startX;
	float crtValue = vPoints[0];

	if (crtValue < m_fThreshold )
	{
		ThresholdBrush.CreateSolidBrush(RGB(0, 255, 0));
		pOldBrush = (CBrush*)::SelectObject(hDC, ThresholdBrush.GetSafeHandle());
	}
	else
	{
        ThresholdBrush.CreateSolidBrush(RGB(255, 0, 0));
        pOldBrush = (CBrush*)::SelectObject(hDC, ThresholdBrush.GetSafeHandle());
	}

	dataPt.y = endY +  int(1.0f * yLen * (fMax - crtValue) / fMax);
	::MoveToEx(hDC, dataPt.x, dataPt.y, NULL);
	if(sz < m_nPointCount + 1)
		::Ellipse(hDC, dataPt.x - 3, dataPt.y - 3, dataPt.x + 3, dataPt.y + 3);
	
	for(int t = 1; t < sz; t++)
	{
		crtValue = vPoints[t];
		dataPt.x += xStep;
		dataPt.y = endY + int(1.0f * yLen * (fMax - crtValue) / fMax);

		CPen ThresholdPen;
		CBrush ThresholdBrush;
		if (vPoints[t] < m_fThreshold )
		{
			ThresholdBrush.CreateSolidBrush(RGB(0, 255, 0));
			pOldBrush = (CBrush*)::SelectObject(hDC, ThresholdBrush.GetSafeHandle());
		}
		else
		{
            ThresholdBrush.CreateSolidBrush(RGB(255, 0, 0));
            pOldBrush = (CBrush*)::SelectObject(hDC, ThresholdBrush.GetSafeHandle());
		}

		::Ellipse(hDC, dataPt.x - 3, dataPt.y - 3, dataPt.x + 3, dataPt.y + 3);
		::LineTo(hDC, dataPt.x, dataPt.y);
		::MoveToEx(hDC, dataPt.x, dataPt.y, NULL);
	}
	::SelectObject(hDC, hOldPen);
	::SelectObject(hDC, pOldBrush);


	if ( sz == m_nPointCount+1 )
	{
		m_bfresh = true;
	}
}

int C2DLineGraph::GetGraphAnimationPercent()
{
	return m_AnimationPercent;
}

void C2DLineGraph::SetGraphBkColor(COLORREF g_bkColor)
{
	// Set graph background color
	m_GraphBkColor = g_bkColor;
}

void C2DLineGraph::SetGraphPosition(CPoint g_position)
{
	// Set new graph position
	m_Position = g_position;
}

void C2DLineGraph::SetFullSize(CSize full_size)
{
	// Set full size
	m_FullSize = full_size;
}

void C2DLineGraph::SetLabelsColor(COLORREF l_color)
{
	// Set graph labels color
	m_LabelsColor = l_color;
}

COLORREF C2DLineGraph::GetLabelsColor()
{
	return m_LabelsColor;
}

void C2DLineGraph::SetLegendTextColor(COLORREF t_color)
{
	// Set legend text color
	m_LegendTextColor = t_color;
}
