#pragma once
#include "afxwin.h"
#include "LabelCtrl.h"

// CDialogUpdateDatabase 对话框
const int IDB_UPDATE_DATA_CLOSE      = 71025;

class CDialogUpdateDatabase : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogUpdateDatabase)

public:
	CDialogUpdateDatabase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogUpdateDatabase();

// 对话框数据
	enum { IDD = IDD_DIALOG_UPDATE_DATABASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCloseScreen();
	void InitUiWidget(int nCtrlID,int strID = NULL);

	CString GetRecheck();
	
	CString m_Reserved1;
	CEdit m_ditReserved1;
	CString GetReserved1();

	CFont           m_textFont;
	CFont			m_textFont1;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	BOOL m_bRecheck;
	void  SetStrInfo(CString str);
	CString m_strInfo;
	CLabelCtrl m_LabelCtrlInfo;
	CLabelCtrl m_dlgTitle;
	CLabelCtrl m_btnClose;

};
