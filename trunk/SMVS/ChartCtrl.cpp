#include "stdafx.h"
#include "ChartCtrl.h"
#include "SystemManager.h"
#include "ChannelControlPanel.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSystemManager gSystemMgr;

CChartCtrl::CChartCtrl()
{
	// Set default graph title and subtitle
	m_GraphTitle = "Add graph title here";
	m_GraphSubtitle = "Add graph subtitle here";

	// Set default graph type
	m_GraphType = -1;

	// Set default graph DC and bitmap
	m_GraphDC = NULL;
	m_GraphBitmap = NULL;
	// Set default gradient DC and bitmap
	m_GradientDC = NULL;
	m_GradientBitmap = NULL;

	// Set default graph animation
	m_GraphAnimation = FALSE;

	// Set default graph background fill and type
	m_GraphFillType = GB_SOLID;
	m_GraphGradientFillType = GRADIENT_FILL_RECT_V;

	// Set default graph gradient colors
	m_GraphGradientColor1 = RGB(0,0,64);
	m_GraphGradientColor2 = RGB(0,0,216);

	// Set default graph title and subtitle shadow
	m_GraphTitleShadow = FALSE;
	m_GraphSubtitleShadow = FALSE;

	// Set default graph title and subtitle color
	m_GraphTitleColor = RGB(0,0,0);
	m_GraphSubtitleColor = RGB(0,0,0);

	// Set default graph legend show
	m_ShowLegend = FALSE;
	m_bLBtnDown = false;
}

CChartCtrl::~CChartCtrl()
{
	switch ( m_GraphType )
	{
		case GT_2DBAR:
			{
				delete m_2DBarGraph;
				m_2DBarGraph = NULL;
			}
			break;
		case GT_2DLINE:
			{
				delete m_2DLineGraph;
				m_2DLineGraph = NULL;
			}
			break;

		// Delete default graph type (2D bar graph)
		default:
			break;
	}

	// Delete graph DC and bitmap
	if(m_GraphDC != nullptr)
	{
		m_GraphDC->DeleteDC();
		delete m_GraphDC;
		m_GraphDC = NULL;
	}
	if(m_GraphBitmap != nullptr)
	{
		m_GraphBitmap->DeleteObject();
		delete m_GraphBitmap;
		m_GraphBitmap = NULL;
	}
	// Delete gradient DC and bitmap
	if(m_GradientDC  != nullptr)
	{
		m_GradientDC->DeleteDC();
		delete m_GradientDC;
		m_GradientDC = NULL;
	}
	if(m_GradientBitmap != nullptr)
	{
		m_GradientBitmap->DeleteObject();
		delete m_GradientBitmap;
		m_GradientBitmap = NULL;
	}
}

BEGIN_MESSAGE_MAP(CChartCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BOOL CChartCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// Minimal graph size is (200x200)
	CRect rectGraph( rect );
	if ((rectGraph.right - rectGraph.left) < 100)
		rectGraph.right = rectGraph.left + 100;
	if ((rectGraph.bottom - rectGraph.top) < 100)
		rectGraph.bottom = rectGraph.top + 100;

	// Set graph size and position
	m_Position = CPoint( rectGraph.left, rectGraph.top );
	m_Size = CSize( rectGraph.right - rectGraph.left, rectGraph.bottom - rectGraph.top );

	m_GraphBackgroundColor = RGB(255,255,255);
	m_Update = TRUE;
	m_GraphTitleFont.CreateFont( int(m_Size.cy*0.1), int(m_Size.cx*0.01), 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial" ));
	m_GraphSubtitleFont.CreateFont( int(m_Size.cy*0.07), int(m_Size.cx*0.03), 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial") );
	
	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS, rectGraph, pParentWnd, nID, NULL);
}

void CChartCtrl::CreateLineGraph(CRect& rcClient, int nXLen, wstring strXUnit, wstring strYUnit)
{
	m_GraphType = GT_2DLINE;
	m_Size.cx = rcClient.Width();
	m_Size.cy = rcClient.Height();
	SPC_SETTING spcSetting;
	spcSetting.m_strUnitX = strXUnit;
	spcSetting.m_nXLen    = nXLen;
	spcSetting.m_strUnitY = strYUnit;

	CPoint g_point = CPoint(int(m_Size.cx*0.01), int(m_Size.cy*0.07));
	CSize g_size;
	g_size.cx = m_Size.cx - 35;
	g_size.cy = m_Size.cy - 30;
	m_2DLineGraph = new C2DLineGraph(g_point, g_size, &spcSetting);
	m_2DLineGraph->SetFullSize(m_Size);


	CDC* pDC = GetDC();
	// Create graph DC and bitmap
	m_GraphDC = new CDC();
	m_GraphDC->CreateCompatibleDC(pDC);
	m_GraphBitmap = new CBitmap();
	m_GraphBitmap->CreateCompatibleBitmap(pDC, m_Size.cx, m_Size.cy);
	m_GraphDC->SelectObject(m_GraphBitmap);
	// Create gradient DC and bitmap
	m_GradientDC = new CDC();
	m_GradientDC->CreateCompatibleDC(pDC);
	m_GradientBitmap = new CBitmap();
	m_GradientBitmap->CreateCompatibleBitmap(pDC, m_Size.cx, m_Size.cy);
	m_GradientDC->SelectObject(m_GradientBitmap);

	// Draw graph gradient background
	GRADIENT_RECT gRct;
	TRIVERTEX gVert[2];

	gVert[0].x = 0;
	gVert[0].y = 0;
	gVert[0].Red = (GetRValue(m_GraphGradientColor1)<<8);
	gVert[0].Green = (GetGValue(m_GraphGradientColor1)<<8);
	gVert[0].Blue = (GetBValue(m_GraphGradientColor1)<<8);
	gVert[0].Alpha = 0x0000;

	gVert[1].x = m_Size.cx;
	gVert[1].y = m_Size.cy;
	gVert[1].Red = (GetRValue(m_GraphGradientColor2)<<8);
	gVert[1].Green = (GetGValue(m_GraphGradientColor2)<<8);
	gVert[1].Blue = (GetBValue(m_GraphGradientColor2)<<8);
	gVert[1].Alpha = 0x0000;

	gRct.UpperLeft = 0;
	gRct.LowerRight = 1;
	::GradientFill(m_GradientDC->m_hDC, gVert, 2, &gRct, 1, m_GraphGradientFillType);

	// Release DC
	CWnd::ReleaseDC(pDC);
}
void CChartCtrl::SetChIndex(int index)
{
	switch (m_GraphType)
	{
	case GT_2DBAR:
		break;
	case GT_2DLINE:
		{
			m_2DLineGraph->SetChIndex(index);
		}
		break;

		// Create default graph type (2D bar graph)
	default:
		break;
	}

}



void CChartCtrl::CreateGraph(int g_type, SPC_SETTING* pSpcSetting)
{
	m_GraphType = g_type;

	switch (m_GraphType)
	{
		case GT_2DBAR:
			{
				CPoint g_point = CPoint(int(m_Size.cx*0.012), int(m_Size.cy*0.03));
				CSize g_size = CSize(int(m_Size.cx*0.96), int(m_Size.cy*0.92));
				m_2DBarGraph = new C2DBarGraph(g_point, g_size);
				m_2DBarGraph->SetFullSize(m_Size);
			}
			break;

		case GT_2DLINE:
			{
				// Set graph size and position
				CPoint g_point = CPoint(int(m_Size.cx*0.01), int(m_Size.cy*0.07));
				CSize g_size = CSize(int(m_Size.cx*0.95), int(m_Size.cy*0.9));
				m_2DLineGraph = new C2DLineGraph(g_point, g_size, pSpcSetting);
				m_2DLineGraph->SetFullSize(m_Size);
			}
			break;

		// Create default graph type (2D bar graph)
		default:
			break;
	}

	CDC* pDC = GetDC();

	// Create graph DC and bitmap
	m_GraphDC = new CDC();
	m_GraphDC->CreateCompatibleDC( pDC );
	m_GraphBitmap = new CBitmap();
	m_GraphBitmap->CreateCompatibleBitmap( pDC, m_Size.cx, m_Size.cy );
	m_GraphDC->SelectObject( m_GraphBitmap );
	// Create gradient DC and bitmap
	m_GradientDC = new CDC();
	m_GradientDC->CreateCompatibleDC( pDC );
	m_GradientBitmap = new CBitmap();
	m_GradientBitmap->CreateCompatibleBitmap( pDC, m_Size.cx, m_Size.cy );
	m_GradientDC->SelectObject( m_GradientBitmap );

	// Draw graph gradient background
	GRADIENT_RECT gRct;
	TRIVERTEX gVert[2];

	gVert[0].x = 0;
	gVert[0].y = 0;
	gVert[0].Red = (GetRValue(m_GraphGradientColor1)<<8);
	gVert[0].Green = (GetGValue(m_GraphGradientColor1)<<8);
	gVert[0].Blue = (GetBValue(m_GraphGradientColor1)<<8);
	gVert[0].Alpha = 0x0000;

	gVert[1].x = m_Size.cx;
	gVert[1].y = m_Size.cy;
	gVert[1].Red = (GetRValue(m_GraphGradientColor2)<<8);
	gVert[1].Green = (GetGValue(m_GraphGradientColor2)<<8);
	gVert[1].Blue = (GetBValue(m_GraphGradientColor2)<<8);
	gVert[1].Alpha = 0x0000;

	gRct.UpperLeft = 0;
	gRct.LowerRight = 1;
	::GradientFill( m_GradientDC->m_hDC, gVert, 2, &gRct, 1, m_GraphGradientFillType );

	// Release DC
	CWnd::ReleaseDC( pDC );
}

void CChartCtrl::Receive2DLineGraphData(boost::circular_buffer<float>& dataBuffer)
{
	m_2DLineGraph->ReceiveGraphData(dataBuffer);
}

void CChartCtrl::Receive2DLineGraphData(std::vector<float>& vDataPoint)
{
	//TODO:
}
void CChartCtrl::SetXIndex(int xIndex)
{
	switch ( m_GraphType )
	{
		// 	case GT_2DBAR:
		// 		m_2DBarGraph->SetXIndex( xIndex );
		// 		break;
	case GT_2DLINE:
		m_2DLineGraph->SetXIndex( xIndex );
		break;
	default:
		break;
	}

	Invalidate( FALSE );
}

void CChartCtrl::ClearGraph()
{
	if(m_GraphType == GT_2DBAR)
	{
		m_2DBarGraph->ClearGraph();
	}
	if(	m_GraphType == GT_2DLINE)
	{
		m_2DLineGraph->ClearGraph();
	}
}

void CChartCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = GetDC();

	CRect gRect;
	if(m_GraphType == GT_2DBAR)
	{
		gRect = CRect( int(m_Size.cx*0.05), int(m_Size.cy*0.35), int(m_Size.cx*0.05) + int(m_Size.cx*0.6), int(m_Size.cy*0.35) + int(m_Size.cy*0.6) );
	}
    if(m_GraphType == GT_2DLINE)
	{
		gRect = CRect( int(m_Size.cx*0.05), int(m_Size.cy*0.35), int(m_Size.cx*0.05) + int(m_Size.cx*0.6), int(m_Size.cy*0.35) + int(m_Size.cy*0.6) );
	}
	if ( m_GraphFillType == GB_SOLID )
	{
		CBrush bgBrush(m_GraphBackgroundColor);
		::FillRect(m_GraphDC->m_hDC, gRect, (HBRUSH)bgBrush.GetSafeHandle());
		bgBrush.DeleteObject();
	}
	else
	{
		::BitBlt( m_GraphDC->m_hDC, 0, 0, m_Size.cx, m_Size.cy, m_GradientDC->m_hDC, 0, 0, SRCCOPY );
	}

	if(m_GraphType == GT_2DBAR)
	{
		m_2DBarGraph->BuildGraph(m_GraphDC->m_hDC);
	}

	if(m_GraphType == GT_2DLINE)
	{
		m_2DLineGraph->BuildGraph(m_GraphDC->m_hDC);
	}

	if ( m_GraphFillType == GB_GRADIENT )
	{
		CFont* pOldFont = m_GraphDC->SelectObject(&m_GraphTitleFont );
		COLORREF oldTextColor;
		m_GraphDC->SetBkMode( TRANSPARENT );
		if ( m_GraphTitleShadow == TRUE )
		{
			oldTextColor = m_GraphDC->SetTextColor( RGB(128,128,128) );
			m_GraphDC->TextOut( int(m_Size.cx*0.065), int(m_Size.cy*0.037), m_GraphTitle, m_GraphTitle.GetLength() );
			m_GraphDC->SetTextColor( oldTextColor );
		}
		oldTextColor = m_GraphDC->SetTextColor( m_GraphTitleColor );
		m_GraphDC->TextOut( int(m_Size.cx*0.05), int(m_Size.cy*0.025), m_GraphTitle, m_GraphTitle.GetLength() );
		m_GraphDC->SetBkMode( OPAQUE );
		m_GraphDC->SelectObject( pOldFont );
		m_GraphDC->SetTextColor( oldTextColor );

		// Draw graph subtitle
		pOldFont = m_GraphDC->SelectObject(&m_GraphSubtitleFont );
		m_GraphDC->SetBkMode( TRANSPARENT );
		if ( m_GraphSubtitleShadow == TRUE )
		{
			oldTextColor = m_GraphDC->SetTextColor( RGB(128,128,128) );
			m_GraphDC->TextOut( int(m_Size.cx*0.06), int(m_Size.cy*0.208), m_GraphSubtitle, m_GraphSubtitle.GetLength() );
			m_GraphDC->SetTextColor( oldTextColor );
		}
		oldTextColor = m_GraphDC->SetTextColor( m_GraphSubtitleColor );
		m_GraphDC->TextOut( int(m_Size.cx*0.05), int(m_Size.cy*0.2), m_GraphSubtitle, m_GraphSubtitle.GetLength() );
		m_GraphDC->SetBkMode( OPAQUE );
		m_GraphDC->SelectObject( pOldFont );
		m_GraphDC->SetTextColor( oldTextColor );
	}
	else
	{
		if ( m_Update == TRUE )
		{
			// Draw graph title
			COLORREF oldBkColor = m_GraphDC->SetBkColor( m_GraphBackgroundColor );
			CFont* pOldFont = m_GraphDC->SelectObject(&m_GraphTitleFont );
			COLORREF oldTextColor;
			m_GraphDC->SetBkMode( TRANSPARENT );
			if ( m_GraphTitleShadow == TRUE )
			{
				oldTextColor = m_GraphDC->SetTextColor( RGB(128,128,128) );
				m_GraphDC->TextOut( int(m_Size.cx*0.065), int(m_Size.cy*0.037), m_GraphTitle, m_GraphTitle.GetLength() );
				m_GraphDC->SetTextColor( oldTextColor );
			}
			oldTextColor = m_GraphDC->SetTextColor( m_GraphTitleColor );
			m_GraphDC->TextOut( int(m_Size.cx*0.05), int(m_Size.cy*0.025), m_GraphTitle, m_GraphTitle.GetLength() );
			m_GraphDC->SetBkMode( OPAQUE );
			m_GraphDC->SelectObject( pOldFont );
			m_GraphDC->SetBkColor( oldBkColor );
			m_GraphDC->SetTextColor( oldTextColor );

			// Draw graph subtitle
			oldBkColor = m_GraphDC->SetBkColor( m_GraphBackgroundColor );
			pOldFont = m_GraphDC->SelectObject(&m_GraphSubtitleFont );
			m_GraphDC->SetBkMode( TRANSPARENT );
			if ( m_GraphSubtitleShadow == TRUE )
			{
				oldTextColor = m_GraphDC->SetTextColor( RGB(128,128,128) );
				m_GraphDC->TextOut( int(m_Size.cx*0.06), int(m_Size.cy*0.208), m_GraphSubtitle, m_GraphSubtitle.GetLength() );
				m_GraphDC->SetTextColor( oldTextColor );
			}
			oldTextColor = m_GraphDC->SetTextColor( m_GraphSubtitleColor );
			m_GraphDC->TextOut( int(m_Size.cx*0.05), int(m_Size.cy*0.2), m_GraphSubtitle, m_GraphSubtitle.GetLength() );
			m_GraphDC->SetBkMode( OPAQUE );
			m_GraphDC->SelectObject( pOldFont );
			m_GraphDC->SetBkColor( oldBkColor );
			m_GraphDC->SetTextColor( oldTextColor );
			m_Update = FALSE;
		}
	}
	CRect rect;
	GetClientRect( &rect );
	pDC->BitBlt( 0, 0, rect.Width(), rect.Height(), m_GraphDC, 0, 0, SRCCOPY );

	// Relese graph window DC
	CWnd::ReleaseDC( pDC );
	// Do not call CWnd::OnPaint() for painting messages
}

void CChartCtrl::SetGraphBackgroundColor(COLORREF g_bgcolor)
{
	m_GraphBackgroundColor = g_bgcolor;

	switch ( m_GraphType )
	{
		case GT_2DBAR:
			m_2DBarGraph->SetGraphBkColor( g_bgcolor );
			break;

		case GT_2DLINE:
			m_2DLineGraph->SetGraphBkColor( g_bgcolor );
			break;
		default:
			break;
	}

	Invalidate( FALSE );
}

COLORREF CChartCtrl::GetGraphBackgroundColor()
{
	return m_GraphBackgroundColor;
}

BOOL CChartCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	m_Update = TRUE;

	CBrush bgBrush( m_GraphBackgroundColor );
	CBrush* pOldBrush = pDC->SelectObject( &bgBrush );
	m_GraphDC->SelectObject( &bgBrush );

	CRect rect;
	pDC->GetClipBox( &rect );
	pDC->PatBlt( rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY );
	m_GraphDC->GetClipBox( &rect );
	m_GraphDC->PatBlt( rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY );
	pDC->SelectObject( pOldBrush );
	m_GraphDC->SelectObject( pOldBrush );
	bgBrush.DeleteObject();
	return TRUE;
}

void CChartCtrl::OnTimer(UINT_PTR nIDEvent) 
{
	int result;
	switch ( m_GraphType )
	{
		case GT_2DBAR:
			{
				result = m_2DBarGraph->GetGraphAnimationPercent();
				if ( result >= 100 )
				{
					CBrush bgBrush( m_GraphBackgroundColor );
					CRect rect = CRect( int(m_Size.cx*0.05), int(m_Size.cy*0.35), int(m_Size.cx*0.05) + int(m_Size.cx*0.5), int(m_Size.cy*0.35) + int(m_Size.cy*0.5) );
					::FillRect( m_GraphDC->m_hDC, rect, (HBRUSH)bgBrush.GetSafeHandle() );
					bgBrush.DeleteObject();
				}
			}
			break;
		case GT_2DLINE:
			{
				result = m_2DLineGraph->GetGraphAnimationPercent();
				if ( result >= 100 )
				{
					CBrush bgBrush( m_GraphBackgroundColor );
					CRect rect = CRect( int(m_Size.cx*0.05), int(m_Size.cy*0.35), int(m_Size.cx*0.05) + int(m_Size.cx*0.5), int(m_Size.cy*0.35) + int(m_Size.cy*0.5) );
					::FillRect( m_GraphDC->m_hDC, rect, (HBRUSH)bgBrush.GetSafeHandle() );
					bgBrush.DeleteObject();
				}
			}
			break;
		default:
			break;
	}
	m_Update = FALSE;

	Invalidate( FALSE );
	CWnd::OnTimer(nIDEvent);
}

void CChartCtrl::SetGraphTitle(CString g_title)
{
	m_GraphTitle = g_title;
	m_Update = TRUE;
	Invalidate( FALSE );
}

void CChartCtrl::SetGraphSubtitle(CString g_subtitle)
{
	m_GraphSubtitle = g_subtitle;
	m_Update = TRUE;
	Invalidate( FALSE );
}

void CChartCtrl::SetGraphPosition(CPoint g_position)
{
	if ( m_GraphAnimation == TRUE )
	{
		KillTimer( IDT_ANIMATION );
	}
	m_Position = g_position;

	::SetWindowPos( this->m_hWnd, HWND_NOTOPMOST, m_Position.x, m_Position.y, m_Size.cx, m_Size.cy, SWP_NOZORDER );

	m_Update = TRUE;
	if ( m_GraphAnimation == TRUE )
	{
		SetTimer( IDT_ANIMATION, ANIMATION_TIME, NULL );
	}

	Invalidate( FALSE );
}

void CChartCtrl::Add2DBarGraphDataPoint(float xValue, float xSpan, float yValue)
{
	m_2DBarGraph->AddDataPoint(xValue, xSpan, yValue);
}

void CChartCtrl::SetGrahpInfo(const THRESHOLD& th)
{
	m_2DBarGraph->SetGrahpInfo(th);
}

void CChartCtrl::SetThresholdValue(const THRESHOLD& th)
{
	m_2DBarGraph->SetThresholdValue(th);
}
void CChartCtrl::SetBlowThresholdValue(int &iBthreshold)
{
	m_2DLineGraph->SetControlLimitValue(iBthreshold);
}
int CChartCtrl::GetBlowThresholdValue()
{
	return m_2DLineGraph->GetBlowThreshold();
}
void CChartCtrl::DisplayStatisticsData(std::map<float, long>& statistics)
{
	m_2DBarGraph->DisplayStatisticsData(statistics);
}

void CChartCtrl::Add2DLineGraphSegment(CString s_text)
{
}

void CChartCtrl::SetGraphTitleShadow(BOOL t_shadow)
{
	m_GraphTitleShadow = t_shadow;
}

void CChartCtrl::SetGraphSubtitleShadow(BOOL s_shadow)
{
	m_GraphSubtitleShadow = s_shadow;
}

void CChartCtrl::SetGraphTitleColor(COLORREF t_color)
{
	m_GraphTitleColor = t_color;
}

void CChartCtrl::ShowGraphLegend(BOOL legend)
{
	// Set graph legend show
	m_ShowLegend = legend;
}

void CChartCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_GraphType == GT_2DLINE)
	{
		//获取当前排盖率阈值
		UAC crtUser = gSystemMgr.GetCurrentUser();
		
		//管理员权限
		if (crtUser.m_nLevel != UAC_ADMIN)
		return;

		CRect ThresholdRect;
		m_2DLineGraph->GetThresholdRect(ThresholdRect);
		//	GetWindowRect(ThresholdRect);
		if(ThresholdRect.PtInRect(point))
		{
			m_bLBtnDown = true;
			::SetCursor(::LoadCursor(NULL, IDC_HAND));
			m_bgnPt = point;
		}
		return;
	}
	m_bThresholdChanged = false;
	m_bLclChanged = false;
	m_bUclChanged = false;

	m_pBarGraphInfo = m_2DBarGraph->GetBarGraphInfoPtr();
	if(m_pBarGraphInfo->m_thresholdRect.PtInRect(point))
	{
		m_bThresholdChanged = true;
		m_bLBtnDown         = true;
	}
	if(m_pBarGraphInfo->m_lclRect.PtInRect(point))
	{
		m_bLclChanged = true;
		m_bLBtnDown   = true;
	}
	if(m_pBarGraphInfo->m_uclRect.PtInRect(point))
	{
		m_bUclChanged = true;
		m_bLBtnDown   = true;
	}
	if(m_bLBtnDown)
	{
		m_bgnPt = point;
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
	}
	else
	{
		m_bLBtnDown = false;
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CChartCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_GraphType == GT_2DLINE)
	{
		int startY, endY;
		m_2DLineGraph->GetAxisRangeY(startY,endY);
		if(startY == 0 || endY == 0)
			return;

		if(m_bLBtnDown)
		{
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			if(point.y == m_bgnPt.y)
				return;
			if(point.y > startY)
				point.y = startY;
			if(point.y < endY)
				point.y = endY;

			int ChThreshold = m_2DLineGraph ->SetThresholdPoint(point.y);
			((CChannelControlPanel*)GetParent())->SetBlowThresholdValue(ChThreshold,false);
			Invalidate(TRUE);
			m_bLBtnDown = false;
		}
		return;

	}
	int startX = 0;
	int endX   = 0;
	float fValueRange = 0.0f;
	m_2DBarGraph->GetAxisRangeX(startX, endX, fValueRange);
	if(startX == 0 || endX == 0)
		return;
	int nChartRangeX = endX - startX;
	if(m_bLBtnDown)
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		if(point.x == m_bgnPt.x)
			return;
		if(point.x < startX)
			point.x = startX;
		if(point.x > endX)
			point.x = endX;

		float deltaTh = abs(point.x - m_bgnPt.x) * fValueRange / nChartRangeX;
		if(m_pBarGraphInfo->m_valueType == VT_I4)
		{
			deltaTh = ::ceilf(deltaTh);
		}
		else
		{
			int nDelta = (int)deltaTh;
			float tmp = deltaTh - nDelta;
			if(tmp < 0.01)
				deltaTh = nDelta + 0.01f;
		}

		float newThreshold = 999999.9f;
		if(m_bThresholdChanged)
			newThreshold = m_pBarGraphInfo->m_fThreshold;
		if(m_bLclChanged)
			newThreshold = m_pBarGraphInfo->m_fLCL;
		if(m_bUclChanged)
			newThreshold = m_pBarGraphInfo->m_fUCL;

		if(point.x > m_bgnPt.x)
			{
				newThreshold += deltaTh;
				if (m_bLclChanged&&m_bUclChanged)
				{
					m_bLclChanged =false;
				}
			}
		else
			{
				newThreshold -= deltaTh;
				if (m_bLclChanged&&m_bUclChanged)
				{
					m_bUclChanged =false;
				}
			}

		int th    = int(newThreshold * 100.0f);
		int maxTh = int(m_pBarGraphInfo->m_fMaxValueX * 100.0f);
		if(m_bThresholdChanged)
		{
			::PostMessage(GetParent()->m_hWnd, MSG_BAR_GRAPH_CHANGED, (WPARAM)maxTh, (LPARAM)th);
		}
		if(m_bLclChanged || m_bUclChanged)
		{
			int nFlag = (m_bLclChanged) ? 0 : 1;
			if (m_bUclChanged)
			{
				if (newThreshold < m_pBarGraphInfo->m_fLCL)
				{
					m_bLBtnDown = false;
					return;
				}
			}

			if (m_bLclChanged)
			{
				if (newThreshold > m_pBarGraphInfo->m_fUCL)
				{
					m_bLBtnDown = false;
					return;
				}
			}
			::PostMessage(GetParent()->m_hWnd, MSG_BAR_GRAPH_RANGE_CHANGED, (WPARAM)nFlag, (LPARAM)th);
		}
		m_bLBtnDown = false;
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CChartCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_GraphType == GT_2DLINE)
		return;

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CChartCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_bLBtnDown)
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CChartCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_MOUSEMOVE||
		pMsg->message== WM_LBUTTONUP)//鼠标移动消息
	{
		CWnd *pWnd = GetParent(); //获取父窗口指针
		if(pWnd && pWnd->GetSafeHwnd()) //父窗口指针有效
		{
			//计算此控件与父窗口的位置偏差
			CRect this_rect;
			GetWindowRect(&this_rect);
			CRect parent_rect;
			pWnd->GetWindowRect(&parent_rect);
			CPoint offset(this_rect.left - parent_rect.left,this_rect.top - parent_rect.top);

			//当前窗口中的鼠标位置
			CPoint point(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));

			//换算到父窗口位置
			CPoint parentPoint = point + offset;

			//传递消息给父窗口
			::PostMessage(pWnd->m_hWnd,pMsg->message,pMsg->wParam,MAKELONG(parentPoint.x,parentPoint.y));
		}    
	}
	return CWnd::PreTranslateMessage(pMsg);
}
