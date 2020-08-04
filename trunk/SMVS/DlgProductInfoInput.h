#pragma once
#include "SystemManager.h"
#include "LabelCtrl.h"
// CDlgProductInfoInput 对话框
#include <vector>
#include "afxwin.h"
using namespace std;

class CDlgProductInfoInput : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProductInfoInput)

public:
	CDlgProductInfoInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProductInfoInput();

// 对话框数据
	enum { IDD = IDD_DIALOG_INPUT_PRODUCTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFont m_txtFont;
	//动态判断字段
	std::vector<CString> m_vecProductInfo;
	std::vector<CString> m_vecProductInfoValue;

	//批次号
	CLabelCtrl	 m_btnClose;
	afx_msg void OnCloseScreen();

	BOOL SetProductInfoKey(std::vector<CString> m_vecKey);

	BOOL GetProductInfoValue(std::vector<CString> &m_vecValue);
	BOOL SetSysForeignKey(CString SysForeignKey);
	CString GetSysForeignKey();

	std::vector<CStatic *> m_vecCStatic;
	std::vector<CEdit *> m_vecCEdit;
 
	CButton m_btnOk;
	CButton m_btnCancle;

	CString m_sysForeignKey;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedUpdate();
};
