
#pragma once
#include "TypeDef.h"
#include "ui_decorator.h"

struct ui_decorator;
class CChannelControlPanel;

class CBtnCtrl : public CButton
{
public:
	CBtnCtrl();
	virtual ~CBtnCtrl();

	void ClearColor();
	void SetColor(COLORREF fgColor, COLORREF bgColor);
	void SetButtonType(eBUTTON_TYPE btnType);
	eBUTTON_TYPE GetButtonType();
	void SetUiDecoratorPtr(ui_decorator* pUiDecorator);
	void SetButtonImage(eIMAGE_TYPE imgType = IMG_INVALID);
	void SetChCtrlPanelPtr(CChannelControlPanel* pChCtrlPanel);

protected:
	CHANNEL_CONFIG*   m_chConfig; // Channel Configuration
	eBUTTON_TYPE      m_btnType;
	bool m_bIsColor;
	COLORREF m_fgColor;
	COLORREF m_bgColor;

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void  OnClicked();
	DECLARE_MESSAGE_MAP()

private:
	CFont m_font;
	CImage* m_pImage;
	ui_decorator* m_ptrUiDecorator;
	CChannelControlPanel* m_pChCtrlPanel;
};