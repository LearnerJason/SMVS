#pragma once

#include <vector>
#include <map>
#include <set>

using namespace std;

class CCopyTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCopyTaskDlg)

public:
	CCopyTaskDlg(int nChCount, int nCrtChIndex, CString sSrcTaskName,const set<wstring>& setTaskName, std::map<int, wstring>& dictTarget,CWnd* pParent = NULL);  
	virtual ~CCopyTaskDlg();
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DIALOG_COPY_TASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	void InitUiWidget(int nCtrlID, BOOL bVisible, BOOL nEnable = TRUE);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt);
	bool CollectNewTaskName(int nCtrlID, int nIndex);
private:
	const int  TIMER_ID_TASK_DLG;
	int     m_nChCount;
	int     m_nCrtChIndex;
	bool    m_bOpRslt;
	CString m_sSrcTaskName;
	CFont   m_textFont;
	std::map<int, map<CWnd*, CWnd*>> m_dictCtrlPtr; //Channel ID <====> two controls
	std::map<int, wstring>&          m_dictTarget;
	std::set<wstring>                m_setTaskName;

	BOOL m_bCh1Checked;
	BOOL m_bCh2Checked;
	BOOL m_bCh3Checked;
	BOOL m_bCh4Checked;
	BOOL m_bCh5Checked;
	BOOL m_bCh6Checked;
	BOOL m_bCh7Checked;
	BOOL m_bCh8Checked;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
