#if !defined(AFX_LINECOLORLISTCTRL_H__92EB1CD4_CD33_4E02_A098_367A8FEFF38E__INCLUDED_)  
#define AFX_LINECOLORLISTCTRL_H__92EB1CD4_CD33_4E02_A098_367A8FEFF38E__INCLUDED_  
  
#if _MSC_VER > 1000  
#pragma once  
#endif // _MSC_VER > 1000   
  
#include <afxtempl.h>  
#include <map>

using namespace std;
  
  
struct TEXT_BK  
{  
    COLORREF colText;  
    COLORREF colTextBk;  
};  
   
  
class CLineColorListCtrl : public CListCtrl  
{  
public:  
    void SetItemColor(int nLineIndex, COLORREF TextColor, COLORREF TextBkColor);   //设置某一行的前景色和背景色  
    void SetAllItemColor(int nLineIndex, COLORREF TextColor, COLORREF TextBkColor);//设置全部行的前景色和背景色  
    void ClearColor();                                                          //清除颜色映射表  
  
public:  
    CLineColorListCtrl();  
   
public:  
	std::map<int, TEXT_BK> m_dictLine_Color;  
    virtual ~CLineColorListCtrl();  
   
protected:  
    void CLineColorListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);  
  
    DECLARE_MESSAGE_MAP()  
};    
  
#endif
