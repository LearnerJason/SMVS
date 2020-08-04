#pragma once
#include "ui_decorator.h"

enum TEXT_ALIGH
{
	CENTER = 0,
	LEFT,
	RIGHT
};

class CLabelCtrl : public CStatic
{
public:
	CLabelCtrl();
	virtual ~CLabelCtrl();
	void SetTxtFont(int height,int weight, TEXT_ALIGH alignType = CENTER);
	void ClearColor();
	void SetColor(COLORREF Color,COLORREF BgColor);
	void SetLabelImage(eIMAGE_TYPE imgType = IMG_INVALID);
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
private:
	CString    m_strText;
	BOOL	   m_bOver;
	BOOL	   m_bIsColor;
	COLORREF   m_Color;
	COLORREF   m_BgColor;
	CFont      m_font;
	int        m_iTxtHeight;
	int        m_iTxtWeight;
	CImage*    m_ptrImage;
	TEXT_ALIGH m_txtAlignType;

};