#pragma once

#include "stdafx.h"
#include "LabelCtrl.h"
#include "ui_decorator.h"

extern ui_decorator gUiDecorator;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMTILabel

CLabelCtrl::CLabelCtrl()
{
	m_bIsColor = FALSE;
	m_bOver = FALSE;
	m_iTxtHeight = 14;
	m_iTxtWeight = FW_NORMAL;
	m_ptrImage = nullptr;
	m_txtAlignType = CENTER;
}

CLabelCtrl::~CLabelCtrl()
{
}


BEGIN_MESSAGE_MAP(CLabelCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CLabelCtrl::OnPaint() 
{
	if (!m_bIsColor)
		CStatic::OnPaint();
	
	CPaintDC dc(this); // device context for painting
	dc.SetTextColor(m_Color);
	CRect rect;
	GetClientRect(&rect);
	if(m_ptrImage != nullptr)
	{
		m_ptrImage->Draw(dc,rect);
		return;
	}
	dc.FillSolidRect(0,0,rect.Width(),rect.Height(),m_BgColor);
	
	GetWindowText(m_strText);
	if(m_strText.IsEmpty())
		return;

	CFont* def_font = dc.SelectObject(&m_font);
	CSize sz = dc.GetTextExtent(m_strText);

	int x = rect.left;
	int y = rect.right;
	if(m_txtAlignType == LEFT)
	{
		y = int((rect.Height() - sz.cy) / 2);
	}
	if(m_txtAlignType == CENTER)
	{
		x = int((rect.Width() - sz.cx) / 2.0);
		y = int((rect.Height() - sz.cy) / 2.0);
	}
	if(m_txtAlignType == RIGHT)
	{
		x = rect.right - sz.cx - 2;
		y = int((rect.Height() - sz.cy) / 2);
	}
	dc.TextOut(x,y, m_strText);
	dc.SelectObject(def_font);
}

void CLabelCtrl::SetColor(COLORREF Color,COLORREF BgColor)
{
	m_bIsColor = TRUE;
	m_Color = Color;
	m_BgColor = BgColor;	
}

void CLabelCtrl::SetTxtFont(int height,int weight,TEXT_ALIGH alignType)
{
	m_iTxtWeight = weight;
	m_txtAlignType = alignType;
	m_font.CreateFont(
		height,                      // nHeight
		0,                           // nWidth
		0,                           // nEscapement
		0,                           // nOrientation
		m_iTxtWeight,	             // nWeight
		FALSE,                       // bItalic
		FALSE,                       // bUnderline
		0,                           // cStrikeOut
		DEFAULT_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,          // nOutPrecision
		CLIP_DEFAULT_PRECIS,         // nClipPrecision
		PROOF_QUALITY,             // nQuality
		DEFAULT_PITCH | FF_DONTCARE, // nPitchAndFamily
		_T("Arial"));

}

BOOL CLabelCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return CStatic::OnEraseBkgnd(pDC);
}

void CLabelCtrl::SetLabelImage(eIMAGE_TYPE imgType)
{
	SetColor(RGB(10,10,115),RGB(200,220,250));

	if(imgType == IMG_INVALID)
		m_ptrImage = nullptr;
	else
		m_ptrImage = &(gUiDecorator.m_imgs[imgType]);
}