#pragma once
#include <vector>
#include "TypeDef.h"
#include "WarningError.h"
#include "ButtonST/WinXPButtonST.h"
#include "ButtonST/ShadeButtonST.h"
#include "LabelCtrl.h"
#include "afxwin.h"
using namespace std;

#define TIMER_LIGHT_CONTROL_TEST 90002
#define TIMER_CAMERA_TEST 90003
const int NEW_SYS_DLG_CLOSE_BTN		= 90001;


class CChannelEventMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChannelEventMgrDlg)

public:
	CChannelEventMgrDlg(int nChIndex,CString& enableFlagStr,std::vector<ERROR_INFO>& vChError,CWnd* pParent);   
	virtual ~CChannelEventMgrDlg();

	enum { IDD = IDD_DIALOG_ERROR_CENTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	std::vector<ERROR_INFO>& m_vChError;

	void InitCtrl();
	void ReSetCtrl(int iErrorID = 0);
	std::map<int,CWinXPButtonST*>	m_BtnVecSys;  //系统错误界面控件指针
	std::map<int,CWinXPButtonST*>	m_BtnVecCH;   //通道错误界面控件指针
	std::map<int,CShadeButtonST*>	m_BtnvecCHOp;
	std::map<int,CButton*>			m_BtnvecCHCheck;
	std::map<int,CLabelCtrl*>		m_BtnvecCHDetail;
	std::map<int,CLabelCtrl*>		m_BtnvecCHTitle;

    int      m_nChIndex;
	CButton* m_pCurTestBtn;
	CString& m_enableFlagStr;

	CFont m_font;
	int			 m_titleFontSz;
	CFont		 m_titleFont;
	int			m_txtFontSz;
	CFont		m_txtFont;
	CFont		m_txtLabelFont;
public:
	SMVS_ERROR_INFO m_Errorinfo;

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnWarningOpButtonClick(UINT uID);
	afx_msg void OnWarningCheckButtonClick(UINT uID);

	CStatic m_StaGpPCI;
	CStatic m_StaGpLight;
	CStatic m_StaGpCamera;
	CStatic m_StaGpPai;
    CLabelCtrl	 m_btnClose;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCloseScreen();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedStaticGpPci();
    afx_msg void OnCbnSelchangeComboChList();

	CLabelCtrl	 m_lblTitle;
	COLORREF	 m_titleBgColor;
	COLORREF	 m_btnBgColor;
	COLORREF	 m_ErrorDColor;
	
	CStatic m_StaPaiThreshold;
	CStatic m_StaGpCdp;

	CWnd* m_parentWnd;
	
	bool m_bSystemFlag;
	CStatic m_GpPaiCommu;
	CStatic m_GpPaiError;
	CLabelCtrl m_StaChLabel;
private:
    void HandleLightControllerErrorEvent();
    void HandleCameraErrorEvent();
    void HandleRejectorCardErrorEvent();
    void HandleThresholdValueErrorEvent();
    void HandleContigousDefectErrorEvent();
	void HandleClearInterlock();
};
