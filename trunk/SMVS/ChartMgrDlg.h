#pragma once

class CChartMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChartMgrDlg)

public:
	CChartMgrDlg(CWnd* pParent = NULL); 
	virtual ~CChartMgrDlg();

	enum { IDD = IDD_DIALOG_GRAPH_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);  

	DECLARE_MESSAGE_MAP()

private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void FillUpUiWidget();
	void DisplayChartSettings(int chIndex);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);

private:
	bool  m_bOpRslt;
	CFont m_textFont;
	std::map<int, CHANNEL_CONFIG>* m_pDictChConfig;
	const int TIMER_ID_CHART_MGR_DLG;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStaticApplyChartParam();
	afx_msg void OnStnClickedStaticApplySpcRule();
	afx_msg void OnCbnSelchangeComboCrtChannelId();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
