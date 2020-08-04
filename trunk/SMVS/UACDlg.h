#pragma once
#include "LabelCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"

#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class CUACDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUACDlg)

public:
	CUACDlg(CWnd* pParent = NULL); 
	virtual ~CUACDlg();
	enum { IDD = IDD_DIALOG_UAC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void LoadBackgroundImage(CPaintDC& dc);
	void CreateLabels();
	void InitUserList();
	void FillUserList();
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void AddNewUser2Table(CString sUsername, CString sPassword, int nLevel);
	void ResetUiWidget();
	void DisplayUac(const UAC& uac);

	const int UAC_DLG_TIMER_ID;
	CLabelCtrl m_dlgTitle;
	COLORREF m_titleBgColor;
	int m_titleFontSz;
	CFont m_titleFont;
	int m_txtFontSz;
	CFont m_txtFont; // Ok/Cancel button text and warning message text font
	CFont m_listFont;
	CLabelCtrl m_btnClose;
	COLORREF m_btnBgColor;

	CButton m_btnCreate;
	CButton m_btnUpdate;
	CButton m_btnDelete;
	int BTN_ID_CREATE;
	int BTN_ID_CREATE_DISABLE;
	int BTN_ID_UPDATE;
	int BTN_ID_UPDATE_DISABLE;
	int BTN_ID_DELETE;
	int BTN_ID_DELETE_DISABLE;

	CListCtrl m_listUsers;
	CEdit m_editUsername;
	CEdit m_editPassword;
	CComboBox m_comboGroup;

	bool                  m_bOpRslt;
	CString               ADMIN_STR;
	CString               ENGNIEER_STR;
	UAC                   m_crtSelectedUac;
	std::map<wstring,UAC> m_dictUac;
public:
	afx_msg void OnPaint();
	afx_msg void OnStnClickedClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMClickListUser(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonDelete();
};
