#pragma once
#include <vector>
#include <string>
#include "afxwin.h"

using namespace std;

class CApplySlResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CApplySlResultDlg)

public:
	CApplySlResultDlg(int nChIndex, const std::vector<wstring>& vTaskName, wstring& retTaskName,CWnd* pParent = NULL);
	virtual ~CApplySlResultDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	enum { IDD = IDD_DIALOG_SL_SELECT_TASK };
	void InitUiWidget(int nCtrlID, int strID);
private:
	CFont                m_textFont;
	int                  m_nChIndex;
	wstring&             m_wstrRetTaskName;
	std::vector<wstring> m_vTaskName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_listTaskName;
	afx_msg void OnCbnSelchangeComboChTask();
};
