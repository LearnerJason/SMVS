#pragma once
#include "afxwin.h"
#include "LabelCtrl.h"
// CDialogCTypeOper 对话框

class CDialogCTypeOper : public CDialogEx
{
	CString m_Curtaskname1;
	CString m_Curtaskname2;

	double	m_iCapHeight;
	bool	m_bRollNeck;
	int		m_e_Exptype;

	DECLARE_DYNAMIC(CDialogCTypeOper)
public:

	int GetiCapHeight();
	bool GetbRollNeck();
	int GetExpType();
	int GetDiameter();
	void SetCurTaskName(CString strname1,CString strname2);
	CDialogCTypeOper(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogCTypeOper();

	bool    m_bTaskReady;
// 对话框数据
	enum { IDD = IDD_DIALOG_CTypeOper };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeComboDiameter();
	CComboBox m_comDiameter;
	CEdit m_editCapHeight;
	CComboBox m_comRollneck;
	CComboBox m_comExpType;
	virtual BOOL OnInitDialog();
	CStatic m_staOldinfo;
	CStatic m_staNewinfo;
	CStatic m_staNewTaskInfo;
	afx_msg void OnBnClickedButtonTypechange();
	afx_msg void OnBnClickedButtonQurey();

	bool m_bTaskChanging;
	vector<CString> m_vecTaskname;

	CFont m_font;

	CStatic m_StaCapDiameter;
	CStatic m_staCapHeight;
	CStatic m_staBrollNeck;
	CStatic m_staExpppType;
	CButton m_btnQUeryTask;
	CButton m_btnTypeChange;
	CButton m_btnCancle;
	
	int m_titleFontSz;
	CFont m_titleFont ;

	CLabelCtrl m_dlgTitle;
	COLORREF m_titleBgColor;
	COLORREF m_btnBgColor;
	COLORREF m_TaskinfoColor;


	int m_txtFontSz;
	CFont m_txtFont; // Ok/Cancel button text and warning message text font
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CLabelCtrl  m_btnClose;
	afx_msg void OnStnClickedClose();

	vector<CString> m_vecExpppType;

	double m_diameter;

	double Light1;
	double Light2;

	double NLight1;
	double NLight2;

};
