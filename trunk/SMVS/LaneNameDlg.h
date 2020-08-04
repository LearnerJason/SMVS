#pragma once


class CLaneNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLaneNameDlg)

public:
	CLaneNameDlg(CRect rcPanel,wstring& wstrRetName,CWnd* pParent = NULL);
	virtual ~CLaneNameDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	enum { IDD = IDD_DIALOG_LANE_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CFont    m_textFont;
    CRect    m_rcPanel;
	wstring& m_wstrNewName;
};
