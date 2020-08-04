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
    void SetItemColor(int nLineIndex, COLORREF TextColor, COLORREF TextBkColor);   //����ĳһ�е�ǰ��ɫ�ͱ���ɫ  
    void SetAllItemColor(int nLineIndex, COLORREF TextColor, COLORREF TextBkColor);//����ȫ���е�ǰ��ɫ�ͱ���ɫ  
    void ClearColor();                                                          //�����ɫӳ���  
  
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
