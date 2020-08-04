#pragma once

#include "LabelCtrl.h"
#include "CrownCapTaskMgrDlg.h"
#include "TaskMgrDlg.h"
#include "ModelMgrDlg.h"
#include "CameraMgrDlg.h"
#include "LightMgrDlg.h"
#include "RejectorMgrDlg.h"
#include "ChartMgrDlg.h"
#include "ConfigFileMgrDlg.h"
#include "ShiftMgrDlg.h"


const int NEW_SYS_DLG_CLOSE_BTN		= 80001;
const int SYS_DLG_TASK_BTN			= 80002;
const int SYS_DLG_MODEL_BTN			= 80003;
const int SYS_DLG_CAMERA_BTN		= 80004;
const int SYS_DLG_LIGHT_BTN         = 80005;
const int SYS_DLG_REJECTOR_BTN		= 80006;
const int SYS_DLG_CHART_MGR_BTN		= 80007;
const int SYS_DLG_CONFIG_FILE_BTN	= 80008;
const int SYS_DLG_SHIFT_BTN         = 80009;

class CSysConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSysConfigDlg)

public:
	CSysConfigDlg(CWnd* pParent = NULL); 
	virtual ~CSysConfigDlg();

	enum { IDD = IDD_DIALOG_NEW_SYS_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void CreateHeadCtrls(CRect& rcWorkArea);
	void CreateCommonInfoCtrls(CRect& rcWorkArea);
	void CreateNavigationButtons(CRect& rcWorkArea);
	void LoadAllChannelConfig();
	void ResetUiWidget(SETTING_TYPE oldType, SETTING_TYPE newType);
	void UpdateUiWidget(SETTING_TYPE newType);

private:
	int			 m_titleFontSz;
	CFont		 m_titleFont;
	int			 m_subTitleFontSz;
	CFont		 m_subTitleFont;
	int			 m_txtFontSz;
	CFont		 m_txtFont;
	SETTING_TYPE m_crtSettingType;
	CLabelCtrl	 m_lblTitle;
	CLabelCtrl	 m_btnClose;
	COLORREF	 m_titleBgColor;
	COLORREF	 m_btnBgColor;

	CLabelCtrl m_btnTaskMgr;
	CLabelCtrl m_btnModelMgr;
	CLabelCtrl m_btnCameraConfig;
	CLabelCtrl m_btnLightConfig;
	CLabelCtrl m_btnRejectorConfig;
	CLabelCtrl m_btnChartMgr;
	CLabelCtrl m_btnConfigFileMgr;
	CLabelCtrl m_btnShiftMgr;
	std::map<int, CHANNEL_CONFIG>* m_pDictChConfig; //
	// Child dialogs
#ifdef CROWN_PRODUCT
	CCrownCapTaskMgrDlg  m_taskMgrDlg;
#else
	CTaskMgrDlg          m_taskMgrDlg;
#endif

    CModelMgrDlg         m_modelMgrDlg;
	CCameraMgrDlg		 m_cameraMgrDlg;
	CLightMgrDlg         m_lightMgrDlg;
    CRejectorMgrDlg		 m_rjMgrDlg;
	CChartMgrDlg		 m_chartMgrDlg;
	CConfigFileMgrDlg	 m_configFileMgrDlg;
	CShiftMgrDlg         m_shiftMgrDlg;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCloseScreen();
	afx_msg void OnBtnClickTask();
	afx_msg void OnBtnClickModel();
	afx_msg void OnBtnClickCamera();
	afx_msg void OnBtnClickLight();
	afx_msg void OnBtnClickRejector();
	afx_msg void OnBtnClickChartMgr();
	afx_msg void OnBtnClickConfigFileMgr();
	afx_msg void OnBtnClickShiftMgr();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboProductList();
};
