#include "stdafx.h"  
#include "LineColorListCtrl.h"  
  
#ifdef _DEBUG  
#define new DEBUG_NEW  
#undef THIS_FILE  
static char THIS_FILE[] = __FILE__;  
#endif  
 
  
CLineColorListCtrl::CLineColorListCtrl()  
{  
}  
  
CLineColorListCtrl::~CLineColorListCtrl()  
{  
}  
  
  
BEGIN_MESSAGE_MAP(CLineColorListCtrl, CListCtrl)  
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)  
END_MESSAGE_MAP()  
  
  
void CLineColorListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)    
{        
    *pResult = CDRF_DODEFAULT;  

	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	int iRow = int(lplvcd->nmcd.dwItemSpec);

    switch(lplvcd->nmcd.dwDrawStage)//判断状态     
    {    
    case CDDS_PREPAINT:    
        {    
            *pResult = CDRF_NOTIFYITEMDRAW;    
            break;    
        }    
    case CDDS_ITEMPREPAINT://如果为画ITEM之前就要进行颜色的改变     
        {     
			if(m_dictLine_Color.find(iRow) != m_dictLine_Color.end())         
            { 
				TEXT_BK tb = m_dictLine_Color[iRow]; 
                lplvcd->clrText   = tb.colText;     
                lplvcd->clrTextBk = tb.colTextBk;    
                *pResult = CDRF_DODEFAULT;    
            }    
        }    
        break;    
    }    
}    
  
void CLineColorListCtrl::ClearColor()  
{  
	m_dictLine_Color.clear();  
}  
  
void CLineColorListCtrl::SetItemColor(int nLineIndex, COLORREF TextColor, COLORREF TextBkColor)  
{    
   TEXT_BK tb;  
   tb.colText = TextColor;  
   tb.colTextBk = TextBkColor;  
  
  
    m_dictLine_Color[nLineIndex] = tb;     
    RedrawItems(nLineIndex, nLineIndex);     
          
    SetFocus();    
    UpdateWindow();    
}    
  
  
void CLineColorListCtrl::SetAllItemColor(int nLineIndex, COLORREF TextColor, COLORREF TextBkColor)    
{     
    TEXT_BK tb;  
    tb.colText = TextColor;  
    tb.colTextBk = TextBkColor;  
  
    if(nLineIndex > 0)    
    {    
        for(int i = 0; i < nLineIndex ;i ++)    
        {       
            m_dictLine_Color[i] = tb;    
            RedrawItems(i, i);    
        }    
    }     
}

  
