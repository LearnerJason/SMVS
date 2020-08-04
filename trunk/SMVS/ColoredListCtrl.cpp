#include "stdafx.h"
#include "ColoredListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColoredListCtrl::CColoredListCtrl()
{
	m_bkgColor = RGB(220,220,249);
}

CColoredListCtrl::~CColoredListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColoredListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

void CColoredListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	int iRow = int(lplvcd->nmcd.dwItemSpec);

	if(lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}
	if(lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		lplvcd->clrText = RGB(0,0,0);
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		return;
	}
	if(lplvcd->nmcd.dwDrawStage == (CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM))
	{
		if(lplvcd->iSubItem == 3)
		{
			m_oldColor = lplvcd->clrTextBk;
			if(m_setFailedLI.find(iRow) != m_setFailedLI.end())
			{
				lplvcd->clrTextBk = RGB(250,0,0);
				*pResult = CDRF_DODEFAULT;
				return;
			}
		}
		if(lplvcd->iSubItem == 4  && m_oldColor != NULL)
		{
			lplvcd->clrTextBk = m_oldColor;
			*pResult = CDRF_DODEFAULT;
			return;
		}
	}
}

void CColoredListCtrl::SetDictListIndexFuncIndex(map<int, int>& dictLI_FI)
{
	m_dictLI_FI = dictLI_FI;
	m_dictFI_LI.clear();
	for(auto it = m_dictLI_FI.begin(); it != m_dictLI_FI.end(); it++)
	{
		int nLI = it->first;
		int nFI = it->second;
		m_dictFI_LI[nFI] = nLI;
	}
}

void CColoredListCtrl::UpdateStatsData(std::vector<FUNC_INFO>& vFuncInfo,bool bDebug,int LastN)
{
	m_vFuncInfo        = vFuncInfo;
	
	CString strCount, strRate;
	m_setFailedLI.clear();
	for(size_t t = 0; t < m_vFuncInfo.size(); t++)
	{
		FUNC_INFO fi = m_vFuncInfo[t];
		if(m_dictFI_LI.find(fi.m_funcIndex) != m_dictFI_LI.end())
		{
			int nListIndex = m_dictFI_LI[fi.m_funcIndex];
			if(!fi.m_bPass)
			{
				m_setFailedLI.insert(nListIndex);
			}

			long nErrCount = fi.m_nErrCount;
			strCount.Format(_T("   %d"), nErrCount);

			if(fi.m_nTotalCount > 0)
			{
				strRate.Format(_T("   %.2f"), 100.0f * fi.m_nErrCount / (fi.m_nTotalCount));
			}
			else
			{
				strRate = _T("0.00");
			}
			SetItemText(nListIndex, 3, strCount);
			SetItemText(nListIndex, 4, strRate);

			long nRecErrCount = (long)fi.m_VecRecentErrCount.size();

			strCount.Format(_T("%d"),nRecErrCount);
			SetItemText(nListIndex, 5, strCount);

			if(fi.m_nTotalCount > 0)
			{
				if (fi.m_nTotalCount >=LastN)
				{
					strRate.Format(_T("   %.2f"), 100.0f * nRecErrCount /LastN );
				}
				else
				{
					strRate.Format(_T("   %.2f"), 100.0f * nRecErrCount /fi.m_nTotalCount );
				}
			}
			else
			{
				strRate = _T("0.00");
			}
			SetItemText(nListIndex, 6, strRate);
		}
	}
}

//void CColoredListCtrl::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	CListCtrl::OnMouseMove(nFlags, point);
//}


//BOOL CColoredListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	SetCursor(LoadCursor(NULL, IDC_ARROW)); 
// 	return TRUE;
//	return CListCtrl::OnSetCursor(pWnd, nHitTest, message);
//}


BOOL CColoredListCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���
	dwStyle|=WS_CHILD;
	return CListCtrl::Create(dwStyle, rect, pParentWnd, nID);
}


BOOL CColoredListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	WPARAM param = pMsg->wParam;

	if(pMsg->message == WM_MOUSEMOVE||
		pMsg->message== WM_LBUTTONUP)//����ƶ���Ϣ
	{
		CWnd *pWnd = GetParent(); //��ȡ������ָ��
		if(pWnd && pWnd->GetSafeHwnd()) //������ָ����Ч
		{
			//����˿ؼ��븸���ڵ�λ��ƫ��
			CRect this_rect;
			GetWindowRect(&this_rect);
			CRect parent_rect;
			pWnd->GetWindowRect(&parent_rect);
			CPoint offset(this_rect.left - parent_rect.left,this_rect.top - parent_rect.top);

			//��ǰ�����е����λ��
			CPoint point(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));

			//���㵽������λ��
			CPoint parentPoint = point + offset;

			//������Ϣ��������
			::PostMessage(pWnd->m_hWnd,pMsg->message,pMsg->wParam,MAKELONG(parentPoint.x,parentPoint.y));
		}    
	}

	if(pMsg->message == WM_KEYDOWN && param == VK_LEFT)
	{
		CWnd *pWnd = GetParent(); //��ȡ������ָ��
		if(pWnd && pWnd->GetSafeHwnd()) //������ָ����Ч
		{
			::PostMessage(pWnd->m_hWnd,pMsg->message,pMsg->wParam,NULL);
		}
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && param == VK_RIGHT)
	{
		CWnd *pWnd = GetParent(); //��ȡ������ָ��
		if(pWnd && pWnd->GetSafeHwnd()) //������ָ����Ч
		{
			::PostMessage(pWnd->m_hWnd,pMsg->message,pMsg->wParam,NULL);
		}
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && param == VK_UP)
	{
		CWnd *pWnd = GetParent(); //��ȡ������ָ��
		if(pWnd && pWnd->GetSafeHwnd()) //������ָ����Ч
		{
			::PostMessage(pWnd->m_hWnd,pMsg->message,pMsg->wParam,NULL);
		}
		return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN && param == VK_DOWN)
	{
		CWnd *pWnd = GetParent(); //��ȡ������ָ��
		if(pWnd && pWnd->GetSafeHwnd()) //������ָ����Ч
		{
			::PostMessage(pWnd->m_hWnd,pMsg->message,pMsg->wParam,NULL);
		}
		return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
