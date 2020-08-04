#pragma once
#include "LabelCtrl.h"
#include "afxwin.h"

class CHelpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHelpDlg)

public:
	CHelpDlg(CWnd* pParent = NULL);  
	virtual ~CHelpDlg();
	BOOL OnInitDialog();
	enum { IDD = IDD_DIALOG_HELP };

private:
	void CreateLabels();
	void LoadBackgroundImage(CPaintDC& dc);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);

	const int   HELP_DLG_TIMER_ID;
	CLabelCtrl  m_dlgTitle;
	COLORREF    m_titleBgColor;
	int         m_titleFontSz;
	CFont       m_titleFont;
	int         m_txtFontSz;
	CFont       m_txtFont; 
	CLabelCtrl  m_btnClose;
	COLORREF    m_btnBgColor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnStnClickedClose();
	CStatic m_StrBlowVersion;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_StrSmvsVersion;
};
