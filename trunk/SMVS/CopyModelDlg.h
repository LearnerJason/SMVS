#pragma once

class CCopyModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCopyModelDlg)

public:
	CCopyModelDlg(CWnd* pParent = NULL); 
	virtual ~CCopyModelDlg();

	enum { IDD = IDD_DIALOG_ENTER_NEW_MODEL_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	void DisplayPromptMessage(CString sMsg, bool bOpRslt);
	void InitUiWidget(int nCtrlID, int strID = NULL);
private:
	const int  TIMER_ID_TASK_TEMPLATE_DLG;
	CFont m_textFont;
	bool  m_bOpRslt;
};
