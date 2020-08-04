#pragma once

#include <map>
#include <vector>
#include "LabelCtrl.h"
#include "ChartCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "LineColorListCtrl.h"
#include "DialogUpdateDatabase.h"

using namespace std;

const int IDB_SPC_DLG_CLOSE      = 70025;
const int SPC_UI_IDB_RIGHT_ARROW = 70026;
const int SPC_UI_IDB_LEFT_ARROW  = 70027;
const int SPC_UI_QUERY_BTN       = 70028;
const int SPC_UI_EXPORT_BTN      = 70029;

const int MSG_DISP_TIMER_ID = 10009;

struct SYS_STAT_MAP1 {  
	std::map<CString, SYS_STAT_DATA_EX> Element;  
};  

struct SYS_STAT_MAP2 {  
	std::map<CString, SYS_STAT_MAP1> Element;  
};  

struct SYS_STAT_MAP3 {  
	std::map<CString, SYS_STAT_MAP2> Element;  
};  

struct SYS_STAT_MAP4 {  
	std::map<long long, SYS_STAT_MAP3> Element;  
};

 typedef std::map<CString,std::map<CString,SYS_STAT_DATA_EX> > Element;

class CSpcDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpcDlg)

public:
	CSpcDlg(const int chIndex, CWnd* pParent = NULL);
	virtual ~CSpcDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	enum { IDD = IDD_DIALOG_SPC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
private:
	void LoadBackgroundImage(CPaintDC& dc);
    void InitUiCtrl();
	void InitDlgHeader(CRect& rcWorkArea);
	void InitUiWidget(int nCtrlID, int strID = NULL);
	void MoveUiWidget(int nCtrlID, CRect& rc);
	void InitChartCtrl(CRect& rcWorkArea);
	void InitChannelSwitch(CRect& rcWorkArea);
	void FillTaskCombo(int chIndex);
	void FillUserNameCombo();
	void InitTaskComboBox(CRect& rcWorkArea);
	void InitShiftBatchNumerComboBox(CRect& rcWorkArea);
	void InitShiftComboBox(CRect& rcWorkArea);
	void FillShiftComboBox();
	void FillBatchNumberComboBox();
	void InitDateTimePicker(CRect& rcWorkArea);
	void InitQueryReport(CRect& rcWorkArea);
	void InitTestItemList(CRect& rcWorkArea);
	void FillTestItemList(const TASK& task);
	void SwitchToChannel(int chIndex);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	bool ParseQueryResult(wstring wstrTaskName, const std::map<long long, SPC_DATA_PT>& queryRslt, const SYSTEMTIME& bgn, const SYSTEMTIME& end);
	bool ParseQueryRjectResult(wstring wstrTaskName, const std::map<long long, SYS_STAT_DATA>& queryRslt, const SYSTEMTIME& bgn, const SYSTEMTIME& end);

	void GenerateQueryReport();
	void GenerateQueryReport_Ex();
	void ResetQueryReport();
	void RedrawButton(CLabelCtrl& btn);
	void ResetUiWidget();
	void SetTimerPicker(const SHIFT& shift);
	void AddRegionSummationData(wstring wstrTaskName, std::vector<long>& vItemErrCount);
#ifdef EOE_PRODUCT
	CString GenerateGlueInjectorStats();
#endif
private:
	int        m_crtChIndex;
	bool       m_bOpRslt;
	CLabelCtrl m_dlgTitle;
	CLabelCtrl m_btnClose;
	CFont      m_textFont;
	CFont      m_comboText;
	CFont      m_giitText; // Glue Injector ID Tracing Font
	CChartCtrl m_chartCtrl;
	CLabelCtrl m_btnNext;
	CLabelCtrl m_btnPrevious;
	CLabelCtrl m_btnQuery;
	CLabelCtrl m_btnExport;
	CLabelCtrl m_btnResetChData;
	COLORREF   m_color1;
	COLORREF   m_color2;

	std::map<long long, SPC_DATA_PT> m_dictQueryRslt;
	std::map<long long, SYS_STAT_DATA> m_dictQueryRJectDataRslt;
	std::vector<int> m_vCtrlID;

	int					m_nChRadioBtn;
	CComboBox			m_comboTask;
	CComboBox			m_shiftCombo;
	CLineColorListCtrl  m_ListTestItem;
	CDateTimeCtrl		m_dateBgn;
	CDateTimeCtrl		m_timeBgn;
	CDateTimeCtrl		m_dateEnd;
	CDateTimeCtrl		m_timeEnd;

	// Query Report2
	long m_nTotalCount2;
	long m_nTotalOKCount2;

	// Query Report
	long m_nTotalCount;
	long m_nNgCount;
	long m_nErrorCount;
	int  m_nHitCount;
	std::vector<float> m_vDefectRatio;
	std::vector<long>  m_vItemErrCount;
	wstring            m_strCrtTaskName;
	std::vector<std::wstring> m_vItemName;
	bool m_bBatchNumberFun;
	bool m_bUpdateDataBaseFun;
public:
	afx_msg void OnPaint();
	afx_msg void OnCloseScreen();
	afx_msg void OnNextBtnClicked();
	afx_msg void OnPreviousBtnClicked();
	afx_msg void OnQueryBtnClicked();
	afx_msg void OnExportBtnClicked();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboChTask();
	afx_msg void OnBnClickedRadioCh1();
	afx_msg void OnBnClickedRadioCh2();
	afx_msg void OnBnClickedRadioCh3();
	afx_msg void OnBnClickedRadioCh4();
	afx_msg void OnBnClickedRadioCh5();
	afx_msg void OnBnClickedRadioCh6();
	afx_msg void OnBnClickedRadioCh7();
	afx_msg void OnBnClickedRadioCh8();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboShiftOption();
	afx_msg void OnDtnDatetimechangeDatetimepickerEndTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboBatchnumber();
	CComboBox m_BatchNumberCombo;
	CComboBox m_comboxUsername;

	SYS_STAT_MAP4 m_TotalSysData;
};
