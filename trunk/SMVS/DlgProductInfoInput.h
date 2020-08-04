#pragma once
#include "SystemManager.h"
#include "LabelCtrl.h"
// CDlgProductInfoInput �Ի���
#include <vector>
#include "afxwin.h"
using namespace std;

class CDlgProductInfoInput : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProductInfoInput)

public:
	CDlgProductInfoInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProductInfoInput();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUT_PRODUCTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFont m_txtFont;
	//��̬�ж��ֶ�
	std::vector<CString> m_vecProductInfo;
	std::vector<CString> m_vecProductInfoValue;

	//���κ�
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
