#pragma once

const int TIMER_ID_NM_DLG   = 14141414;

class CNameModifierDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNameModifierDlg)

public:
	CNameModifierDlg(wstring wstrOldName, set<wstring> setNickname, wstring& retNewName, CWnd* pParent = NULL);  
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual ~CNameModifierDlg();

	enum { IDD = IDD_DIALOG_NAME_MODIFIER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()

private:
	void InitUiWidget(int nCtrlID,int strID = NULL);
	void DisplayPromptMessage(CString sMsg);

private:
	CFont           m_textFont;
	wstring         m_wstrOldName;
	set<wstring>    m_setNickname;
	wstring&        m_retNewName;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
