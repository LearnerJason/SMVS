#pragma once
#include "afxcmn.h"


class CCameraMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraMgrDlg)

public:
	CCameraMgrDlg(CWnd* pParent = NULL); 
	virtual ~CCameraMgrDlg();

	enum { IDD = IDD_DIALOG_CAMERA_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
private:
	const int TIMER_ID_CAMERA_DLG;
	int       m_delayType;
	int       m_nCrtChIndex;
	bool      m_bOpRslt;
	CFont     m_textFont;
	CListCtrl m_cameraList;
	std::map<int, CHANNEL_CONFIG>* m_pDictChConfig;
	std::map<wstring,bool> m_dictCamera_State;
	std::map<wstring,bool> m_CurdictCamera_State;
private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void FillUpUiWidget(int CameraNumID = 0);
	void DisplayCameraSettings(int chIndex,int CameraNumID = 0);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void DisplayAllAvailableCamera();
	void DisPlayCurCHCamera(int chIndex = 1); 
	void EnableAllWidge(bool bEnable);
	void EnableUiWidget(int nCtrlID, BOOL bEnable);

	int m_ListIndex;
	int m_CurCHListIndex;
public:
	afx_msg void OnCbnSelchangeComboCrtChannelId();
	afx_msg void OnStnClickedStaticBtnRefreshCameraList();
	afx_msg void OnStnClickedStaticApplyCameraParam();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickListCameraList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboSysDlgCameraMode();
	CListCtrl m_CurCHcameraList;
	afx_msg void OnClickListCameraListCh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedStaticBtnRefreshCameraListAdd();
	afx_msg void OnStnClickedStaticBtnRefreshCameraListDel();
};
