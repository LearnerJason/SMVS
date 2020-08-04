#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CLightMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLightMgrDlg)

public:
	CLightMgrDlg(CWnd* pParent = NULL); 
	virtual ~CLightMgrDlg();
	enum { IDD = IDD_DIALOG_LIGHT_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboCrtChannelId();
private:
	int             m_nCrtChIndex;
	LIGHT_SETTING   m_crtLightSetting;
	bool            m_bOpRslt;
	CFont           m_textFont;
	const int       TIMER_ID_LIGHT_DLG;
	CComboBox	    m_comboSerialPort;
	vector<long>    m_vCheckCtrlID;
	map<int,int>    m_dictLcID_GpCrtlID;
	int             m_nLcCount;
	map<int, CHANNEL_CONFIG>* m_pDictChConfig;
private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void DisplayWidget(int nCtrlID, BOOL bVisible);
	void HideAllTagCtrl();
	void DisplayTagGroup(int nGpCtrlID, int nLcID, BOOL bVisible);
	void DisplayTagGroup(int nCtrlID, int nLcID , wstring wstrTags, BOOL bVisible);
	void FillUpUiWidget();
	void ResetUIWidget();
	void DisplayLightSettings(int chIndex);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void InitializeSerialPortCombo();
	void CheckLC(int nLcID, int nLcCtrID); 
	void CheckLCALL();
	BOOL CheckLCLine(int nLcID,int nLineID,bool bWarnDispaly = true);
	void EnableAllWidge(bool bEnable);
	void EnableUiWidget(int nCtrlID, BOOL bEnable);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticApplyLightParam();
	afx_msg void OnCbnSelchangeComboSerialPort();
	afx_msg void OnBnClickedCheckLc0();
	afx_msg void OnBnClickedCheckLc1();
	afx_msg void OnBnClickedCheckLc2();
	afx_msg void OnBnClickedCheckLc3();
	afx_msg void OnBnClickedCheckLc4();
	afx_msg void OnBnClickedCheckLc5();
	afx_msg void OnBnClickedCheckLc6();
	afx_msg void OnBnClickedCheckLc7();
	afx_msg void OnBnClickedCheckLc8();
	afx_msg void OnBnClickedCheckLc9();
	afx_msg void OnBnClickedCheckLc10();
	afx_msg void OnBnClickedCheckLc11();
	afx_msg void OnBnClickedCheckLc12();
	afx_msg void OnBnClickedCheckLc13();
	afx_msg void OnBnClickedCheckLc14();
	afx_msg void OnBnClickedCheckLc15();
	afx_msg void OnBnClickedCheckL11();
	afx_msg void OnBnClickedCheckL12();
	afx_msg void OnBnClickedCheckL13();
	afx_msg void OnBnClickedCheckL14();
	afx_msg void OnBnClickedCheckL21();
	afx_msg void OnBnClickedCheckL22();
	afx_msg void OnBnClickedCheckL23();
	afx_msg void OnBnClickedCheckL24();

};
