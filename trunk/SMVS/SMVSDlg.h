
#pragma once
#include <set>
#include "LabelCtrl.h"
#include "afxwin.h"
#include "DialogWarningEx.h"
#include "ButtonST/WinXPButtonST.h"

const int PRIOR_BTN   = 40001;
const int NEXT_BTN    = 40002;
const int REFRESH_BTN = 40003;
const int CLOSE_BTN   = 40004;
const int IDB_BTN_LOGIN   = 40005;
const int IDB_BTN_SETTING = 40006;
const int IDB_BTN_UAC     = 40007;
const int IDB_BTN_HELP    = 40008;
const int IDB_BTN_LANGUAGE = 40009;
const int IDB_BTN_TASKCHANGE  = 40010;
const int START_GRAB_BTN	=40011;

const int TIMER_ID_CH_STATS        = 40005;
const int TIMER_ID_START_UP_REPORT = 90909;
const int TIMER_ID_DATE_TIME       = 90919;
const int TIMER_ID_SYS_CH_WARNING  = 90929;
const int TIMER_ID_RT_SPEED        = 90939; 
const int  TIMER_ID_BATCH_WARNING = 90949;

using namespace std;

class CSMVSDlg : public CDialogEx
{
public:
	CSMVSDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_SMVS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CLabelCtrl m_btnLogin;
	CLabelCtrl m_btnSetting;
	CLabelCtrl m_btnUAC;
	CLabelCtrl m_btnHelp;
	CLabelCtrl m_btnLanguage;

	CLabelCtrl m_title;
	CLabelCtrl m_startBtn;
	CLabelCtrl m_priorBtn;
	CLabelCtrl m_nextBtn;
	CLabelCtrl m_refreshBtn;
	CLabelCtrl m_closeBtn;
	CFont m_txtFont;
	CFont m_warningFont;
	CFont m_rjectFont;
	int   m_oldLen;

	CBrush m_brush;
	int m_crtIndex; // Current Warning Message Index
	int m_warningCount;
	BOOL m_bStartGrab;
	CDialogWarningEx m_DlgWarningEx;
public:
	afx_msg void OnStnClickedLogin();
	afx_msg void OnStnClickedConfiguration();
	afx_msg void OnStnClickedUAC();
	afx_msg void OnStnClickedHelp();
	afx_msg void OnStnClickedLanguage();
	afx_msg void  OnStnClickedNext();
	afx_msg	void OnStnClickedStart();
	afx_msg void  OnStnClickedPrior();
	afx_msg void  OnStnClickedRefresh();
	afx_msg void OnStnClickedClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ShowSystemStats(SYS_STAT_DATA m_crtSysStats);

	void UpdateErrorMsg();
	LRESULT OnChanneOnOffOperation(WPARAM wParam,LPARAM lParam);
	LRESULT  OnCommunication(WPARAM ch, LPARAM port);
	void LoadBackgroundImage(CPaintDC& dc);
	void UpdateWarningCount(int count);
private:
	void CreateDialogTitlePanel(CRect& rcScreen);
	void CreateDateTimePanel(CRect& rcScreen);
	void CreateRjectInfoPanel(CRect& rcScreen);
	void CreateImageControlPanel(CRect& rcScreen);
	void CreateStatusBar(CRect& rcScreen, CRect& rcChart);
	void SetCurrentDatetime();
	void SetButtonAccessibility();
	CString GetDayOfWeekStr(WORD dayOfWeek);
	void BackupUserData();
	void ChangeBatchNumber();
	void ProductInfoDlgShow();
	CString m_strRjectInfo;

public:
	CLabelCtrl m_StaRjectInfo;
	CLabelCtrl m_staBatchNumber;
	CEdit m_editBatchNumber;
	afx_msg void OnStnClickedStaticBatchnumber();

	CLabelCtrl m_staBatchNumberID;
	afx_msg void OnBnClickedButtonAutotaskchange();

		CLabelCtrl m_btnAutoTaskChange;
		afx_msg void OnStnDblclickStaticRjectInfo();
};
