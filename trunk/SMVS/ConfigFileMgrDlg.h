#pragma once


class CConfigFileMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigFileMgrDlg)

public:
	CConfigFileMgrDlg(CWnd* pParent = NULL); 
	virtual ~CConfigFileMgrDlg();

	enum { IDD = IDD_DIALOG_CONFIG_FILE_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);  

	DECLARE_MESSAGE_MAP()

private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);

private:
	bool  m_bOpRslt;
	CFont m_textFont;
	const int TIMER_ID_CONFIG_FILE_DLG;
	BOOL m_bReplaceOldConfig;
	BOOL m_bImportTaskTemplate;
	BOOL m_bImportTask;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticBtnExport();
	afx_msg void OnStnClickedStaticBtnImport();
	afx_msg void OnBnClickedCheckReplaceAll();
	afx_msg void OnBnClickedCheckImportTaskTemplate();
	afx_msg void OnBnClickedCheckImportTask();
};
