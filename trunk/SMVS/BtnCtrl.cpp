#include "stdafx.h"
#include "BtnCtrl.h"
#include "ChannelControlPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBtnCtrl::CBtnCtrl()
{
	m_btnType = BT_INVALID;
	m_bIsColor = false;
	m_ptrUiDecorator = nullptr;
	m_pChCtrlPanel = nullptr;

	m_font.CreateFont(
	20,                        // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,	                // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	ANSI_CHARSET,              // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("Arial"));	
}

CBtnCtrl::~CBtnCtrl()
{
}

BEGIN_MESSAGE_MAP(CBtnCtrl, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CBtnCtrl::SetButtonType(eBUTTON_TYPE btnType)
{
	m_btnType = btnType;
}

eBUTTON_TYPE CBtnCtrl::GetButtonType()
{
	return m_btnType;
}

void CBtnCtrl::SetColor(COLORREF fgColor, COLORREF bgColor)
{
	m_bIsColor = true;
	m_fgColor = fgColor;
	m_bgColor = bgColor;
}

void CBtnCtrl::SetUiDecoratorPtr(ui_decorator* pUiDecorator)
{
	m_ptrUiDecorator = pUiDecorator;
}

void CBtnCtrl::SetChCtrlPanelPtr(CChannelControlPanel* pChCtrlPanel)
{
	m_pChCtrlPanel = pChCtrlPanel;
}

void CBtnCtrl::SetButtonImage(eIMAGE_TYPE imgType)
{
	if(IMG_INVALID == imgType)
	{
		return;
	}
	SetColor(m_fgColor,m_bgColor);
	if(m_ptrUiDecorator == nullptr)
		return;

	m_pImage = &m_ptrUiDecorator->m_imgs[imgType];

	Invalidate();
}

void CBtnCtrl::OnClicked() 
{
	if(m_pChCtrlPanel == nullptr)
		return;

	if(m_btnType == BT_SPC)
	{
		m_pChCtrlPanel->OpenSpcScreen();
	}
	if(m_btnType == BT_PARAM)
	{
		m_pChCtrlPanel->OpenChannelScreen();
	}
}

void CBtnCtrl::OnPaint()
{
	if(!m_bIsColor || m_pImage == nullptr || m_btnType == BT_INVALID)
	{
		CButton::OnPaint();
		return;
	}

	CPaintDC dc(this);
	dc.SetTextColor(RGB(10,10,10));
	CRect rc;
	GetClientRect(&rc);
	if(m_pImage != nullptr)
	{
		m_pImage->TransparentBlt(dc, rc, RGB(0,0,0));
		return;
	}
	dc.FillSolidRect(0,0,rc.Width(),rc.Height(),m_bgColor);
}

BOOL CBtnCtrl::OnEraseBkgnd(CDC* pDC)
{
	return CButton::OnEraseBkgnd(pDC);
}


