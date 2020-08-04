#pragma once
#include "afxwin.h"

#define WARNING_COUNT_NUM 15

class CDialogWarningEx : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogWarningEx)

public:
	CDialogWarningEx(CWnd* pParent = NULL);   
	virtual ~CDialogWarningEx();

	enum { IDD = IDD_DIALOG_WARNING_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_CStaticWarningEx;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void SetWarningFont(CFont *font);
	void SetWarningText(CString str);
	bool IsExtended();
	void DisplayWarningMsg(int index,bool bUpdate = true);
	void UpdateWarningMsg();
	void DisplayWarningMsgNext();
	void DisplayWarningMsgPrior();
private:
	bool m_bWarningBigFlag;
	int  m_WarIndex;
    std::vector<ERROR_INFO> m_vAllErrorInfo;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
