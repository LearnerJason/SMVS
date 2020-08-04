#pragma once
#include "afxwin.h"

class CCrownCapTaskMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCrownCapTaskMgrDlg)

public:
	CCrownCapTaskMgrDlg(const vector<int>& vProduct,CWnd* pParent = NULL); 
	virtual ~CCrownCapTaskMgrDlg();

	enum { IDD = IDD_DIALOG_CROWN_CAP_TASK_MGR };

	void SetCrtProductIndex(int nProductIndex);
	void SwitchToProduct(int nProductIndex);
	void PrepareTaskModelList();
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:
	vector<int> m_vProductId;
	int        m_nCrtProdudctId;
	BOOL       m_bImgInside;
	CFont      m_textFont;
	bool       m_bOpRslt;
	const int  TIMER_ID_CROWN_CAP_TASK_DLG;
	CComboBox  m_comboTask;
	int        m_nTaskIndex;
	CButton    m_btnCreateTask;
	CButton    m_btnDeleteTask;
	CButton    m_btnRetraining;
	std::map<int, CHANNEL_CONFIG>* m_pDictChConfig;
private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void FillUpUiWidget();
	void DisplayPromptMessage(CString sMsg, bool bOpRslt);
public:
	afx_msg void OnBnClickedButtonCreateTask();
	afx_msg void OnBnClickedButtonDeleteTask();
	afx_msg void OnBnClickedButtonRetrainning();
	afx_msg void OnCbnSelchangeComboTask();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
