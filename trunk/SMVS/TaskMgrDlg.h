#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CTaskMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskMgrDlg)

public:
	CTaskMgrDlg(const vector<int>& vProduct,CWnd* pParent = NULL);  
	virtual ~CTaskMgrDlg();
	void SetCrtProductIndex(int nProductIndex);
	void SwitchToProduct(int nProductIndex);
	enum { IDD = IDD_DIALOG_TASK_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void FillUpUiWidget();
	void DisplayPromptMessage(CString sMsg, bool bOpRslt);
	void ListChannelTask(int nChIndex, int nProductID);
	CString SelectFiles();
	void EnterFilePath(int cCtrlID);
	void ListTaskAndModel(int nChIndex);

public:
	void PrepareTaskModelList(int nProductID);
	afx_msg void OnCbnSelchangeComboTmCh();
	afx_msg void OnCbnSelchangeComboTmTask();
	afx_msg void OnBnClickedButtonTmCreateTask();
	afx_msg void OnBnClickedButtonTmDeleteTask();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboTmTaskModel();
	afx_msg void OnBnClickedButtonTmUpdateModel();
	afx_msg void OnBnClickedButtonTmCopyTask();

private:
	vector<int> m_vProductId;
	int        m_nCrtProdudctId;
	CFont      m_textFont;
	CFont      m_listFont;
	const int  TIMER_ID_TASK_DLG;
	bool       m_bOpRslt;
	CButton	   m_btnCreateTask;
	CButton    m_btnDeleteTask;
	CButton    m_btnCopyTask;
	CButton    m_btnUpdateModel;
	int        m_nRow;
	int        m_nCol;
	int        m_nChIndex;
	CComboBox  m_comboTask;
	CComboBox  m_comboTaskModel;
	TASK       m_crtTask;
};
