#pragma once
#include "afxdtctl.h"
#include "LabelCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>

using namespace std;


class CShiftMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShiftMgrDlg)

public:
	CShiftMgrDlg(CWnd* pParent = NULL); 
	virtual ~CShiftMgrDlg();

	enum { IDD = IDD_DIALOG_SHIFT_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void FillUpUiWidget();
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void EnumerateAllShift();
	bool IsShiftExist(const wstring&  wstrShiftName);
	bool GetNewShift(SHIFT& shift);
	void ResetUiWidget();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonCreateShift();
	afx_msg void OnBnClickedButtonUpdateShift();
	afx_msg void OnBnClickedButtonDeleteShift();
	afx_msg void OnNMClickListShift(NMHDR *pNMHDR, LRESULT *pResult);

private:
	const int		SHIFT_DLG_TIMER_ID;
	bool            m_bOpRslt;
	CFont           m_textFont;
	CEdit           m_editShiftName;
	CDateTimeCtrl   m_bgnTime;
	CDateTimeCtrl   m_endTime;
	CListCtrl       m_shiftListCtrl;
	CButton         m_btnCreateShift;
	CButton         m_btnUpdateShift;
	CButton         m_btnDeleteShift;
	std::vector<SHIFT> m_vShift;
	SHIFT           m_crtShift;
public:
	BOOL m_bAutoReset;
	afx_msg void OnBnClickedCheckResetCount();
};
