#pragma once

#include <vector>
#include <set>
#include <map>
#include "TypeDef.h"

using namespace std;

class CColoredListCtrl : public CListCtrl
{
public:
	CColoredListCtrl();

public:
	COLORREF m_bkgColor;
	COLORREF m_oldColor;
	virtual ~CColoredListCtrl();

protected:
	afx_msg void CColoredListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	void SetDictListIndexFuncIndex(map<int, int>& dictLI_FI);
	void UpdateStatsData(std::vector<FUNC_INFO>& vFuncInfo,bool bDebug,int LastN);

private:
	set<int>          m_setFailedLI;    // Failed List Index
	vector<FUNC_INFO> m_vFuncInfo;
	map<int, int>     m_dictLI_FI;      // Function Index <--> List Index
	map<int, int>     m_dictFI_LI;
public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
