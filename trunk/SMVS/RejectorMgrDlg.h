#pragma once
#include "afxwin.h"

class CRejectorMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRejectorMgrDlg)

public:
	CRejectorMgrDlg(CWnd* pParent = NULL); 
	virtual ~CRejectorMgrDlg();

	enum { IDD = IDD_DIALOG_REJECTOR_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
private:
	CFont     m_textFont;
	CFont     m_smallFont;
	int       m_nCrtChIndex;
	std::map<int, CHANNEL_CONFIG>* m_pDictChConfig;
	const int TIMER_ID_RJ_DLG;
	bool      m_bOpRslt;

	int       m_rjMode;
	BOOL	  m_bEnableTest;
	int       m_nTestMode;
	int       m_nActionType;
private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void FillUpUiWidget();
	void ListAvailablePort();
	void DisplayRejectorSettings(int chIndex);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void EnableAllWidge(bool bEnable);
	void EnableUiWidget(int nCtrlID, BOOL bEnable);
public:
	afx_msg void OnCbnSelchangeComboCrtChannelId();
	afx_msg void OnStnClickedStaticApplyIoParam();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckEnableTest();
	afx_msg void OnBnClickedRadioRejectAll();
	afx_msg void OnBnClickedRadioOptionalReject();
	afx_msg void OnBnClickedRadioOdd();
	afx_msg void OnBnClickedRadioEven();
	afx_msg void OnBnClickedHDTest();

	bool m_bReReject;
	bool m_bReRejectFunc;
	afx_msg void OnBnClickedCheckEnableReReject();
private:
    BOOL m_bEnableLastNAlarm;
};
