#pragma once
class CComboBoxCtrl:public CComboBox
{
public:
	CComboBoxCtrl();
public:
public:
	VOID SetTxtFont(int height,int weight);
	VOID ClearColor();
	VOID SetColor(COLORREF Color,COLORREF BgColor);
	virtual ~CComboBoxCtrl();
protected:
	BOOL	 m_bIsColor;
	COLORREF m_fgColor;
	COLORREF m_bgColor;
	int      m_iTxtHeight;
	int      m_iTxtWeight;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};