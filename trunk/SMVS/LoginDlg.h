#pragma once

#include "LabelCtrl.h"
#include "afxwin.h"

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)
public:
	static BOOL m_bIsSlacAdmin;
	CLoginDlg(CWnd* pParent = NULL);
	virtual ~CLoginDlg();
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void LoadBackgroundImage(CPaintDC& dc);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt);
	void CreateLabels();
	CLabelCtrl      m_dlgTitle;
	COLORREF        m_titleBgColor;
	int             m_titleFontSz;
	CFont           m_titleFont;
	int             m_txtFontSz;
	CFont           m_txtFont; // Ok/Cancel button text and warning message text font
	CFont           m_smallFont;
	CEdit           m_usernameEdit;
	CEdit           m_passwordEdit;
	CLabelCtrl      m_okBtn;
	CLabelCtrl      m_cancelBtn;
	COLORREF        m_btnBgColor;
	const int       TIMER_ID_LOGON_DLG;
	bool            m_bOpRslt;

	int m_iLoginFlag;

public:
	afx_msg void OnPaint();
	afx_msg void OnStnClickedOk();
	afx_msg void OnStnDblClickedOk();
	afx_msg void OnStnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
