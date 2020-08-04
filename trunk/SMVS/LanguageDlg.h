#pragma once
#include "LabelCtrl.h"

class CLanguageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLanguageDlg)

public:
	CLanguageDlg(CWnd* pParent = NULL);
	virtual ~CLanguageDlg();
	enum { IDD = IDD_DIALOG_LANG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void LoadBackgroundImage(CPaintDC& dc);
	void CreateLabels();
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void SwitchLanguage();

	const int  TIMER_ID_LANG_DLG;
	CLabelCtrl m_dlgTitle;
	COLORREF m_titleBgColor;
	int m_titleFontSz;
	CFont m_titleFont;
	int m_txtFontSz;
	CFont m_txtFont; // Ok/Cancel button text and warning message text font
	CLabelCtrl m_okBtn;
	CLabelCtrl m_cancelBtn;
	COLORREF m_btnBgColor;

public:
	afx_msg void OnPaint();
	afx_msg void OnStnClickedOk();
	afx_msg void OnStnClickedCancel();
	int m_radioCHS;
	int m_radioCHT;
	int m_radioENU;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int m_nLangID;
	afx_msg void OnBnClickedRadioChs();
	afx_msg void OnBnClickedRadioCht();
	afx_msg void OnBnClickedRadioEnu();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
