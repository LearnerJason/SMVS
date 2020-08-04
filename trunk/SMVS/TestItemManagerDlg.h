#pragma once

#include <map>
#include <set>
#include <vector>
#include "TypeDef.h"
#include "afxcmn.h"
#include "afxwin.h"

#ifdef CROWN_PRODUCT
#import "CrownCapAlgo.dll" no_namespace
#endif

#ifdef EOE_PRODUCT
#import "EoeAlgo.dll" no_namespace
#endif

#ifdef PET_PRODUCT
#import "PetAlgo.dll" no_namespace
#endif

#ifdef PCC_PRODUCT
#import "PccAlgo.dll" no_namespace
#endif

#ifdef CAP_PRODUCT
#import "CapacitorAlgo.dll" no_namespace
#endif

#ifdef MAT_PRODUCT
#import "RubberMatAlgo.dll" no_namespace
#endif

#ifdef NEEDLE_PRODUCT
#import "NeedleAlgo.dll" no_namespace
#endif

#ifdef PLUG_PRODUCT
#import "PlugAlgo.dll" no_namespace
#endif

#ifdef HDPE_PRODUCT
#import "HdpeAlgo.dll" no_namespace
#endif

#ifdef CAN_PRODUCT
#import "CanAlgo.dll" no_namespace
#endif

#ifdef CAN_EOE_PRODUCT
#import "EoeAlgo.dll" no_namespace
#import "CanAlgo.dll" no_namespace
#endif

#ifdef JELLY_PRODUCT
#import "JellyAlgo.dll" no_namespace
#endif

#ifdef CAN_OUTSIDE_PRODUCT
#import "CanOutsideAlgo.dll" no_namespace
#endif

#ifdef NAIL_PRODUCT
#import "NailAlgo.dll" no_namespace
#endif

#ifdef BATTERY_PRODUCT
#import "BatteryShellAlgo.dll" no_namespace
#endif

#ifdef BOTTLE_PRODUCT
#import "BottleCapAlgo.dll" no_namespace
#endif

#ifdef AL_PLASTIC_PRODUCT
#import "AlPlasticCapAlgo.dll" no_namespace
#endif

using namespace std;

const int IDC_EDIT_CTRL_TIM  = 654321;
const int TIMER_ID_TIM_DLG   = 666666;

class CTestItemManagerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestItemManagerDlg)

public:
	CTestItemManagerDlg(const int& nProductID, eTASK_OPERATION toType, int nRgnID, set<wstring> setNickname,FUNC_INFO& retFI, CWnd* pParent = NULL);   
	virtual ~CTestItemManagerDlg();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	enum { IDD = IDD_DIALOG_CH_TASK_MODIFIER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()

private:
	void InitUiWidget(int nCtrlID,int strID = NULL, bool bEnabled = true);
	void FillUpCandidateList();
	void DisplayPromptMessage(CString sMsg, bool bOpRslt);
	void FilterAlgoItem(eTASK_OPERATION toType);
	void DisplayFuncHelpString(wstring& strHelp);
	void DeleteAllParamCtrls();
	void CreateParamCtrl(long funcId);
	void SetDefaultParamValue();
	bool GetCrtAlgoParameters(IT_PARAM& retItParam);
	bool ProduceFuncInfoList(wstring strHelp,IT_PARAM& itParams, FUNC_INFO& funcInfo);
private:
	CFont           m_textFont;
	CFont           m_listFont;
	CListCtrl       m_candidateList;
    eTASK_OPERATION m_taskOperation;
	int             m_nProductID;
	int             m_nRgnID;
	long            m_nFuncID;
	bool            m_bOpRslt;
	FUNC_INFO&      m_retFI;

#ifdef CROWN_PRODUCT
	CComPtr<ICrownCapAlgo>      m_pImageCheckingIt;
#endif

#ifdef EOE_PRODUCT
	CComPtr<IEoeAlgo>           m_pImageCheckingIt;
#endif

#ifdef CAN_PRODUCT
	CComPtr<ICanAlgo>           m_pImageCheckingIt;
#endif

#ifdef CAN_EOE_PRODUCT
	CComPtr<IEoeAlgo>           m_ptrAlgoObjEoe;
	CComPtr<ICanAlgo>           m_ptrAlgoObjCan;
#endif

#ifdef PET_PRODUCT
	CComPtr<IPetAlgo>           m_pImageCheckingIt;
#endif

#ifdef PCC_PRODUCT
	CComPtr<IPccAlgo>           m_pImageCheckingIt;
#endif

#ifdef CAP_PRODUCT
	CComPtr<ICapacitorAlgo>           m_pImageCheckingIt;
#endif

#ifdef MAT_PRODUCT
	CComPtr<IRubberMatAlgo>           m_pImageCheckingIt;
#endif



#ifdef NEEDLE_PRODUCT
	CComPtr<INeedleAlgo>           m_pImageCheckingIt;
#endif

#ifdef PLUG_PRODUCT
	CComPtr<IPlugAlgo>          m_pImageCheckingIt;
#endif

#ifdef HDPE_PRODUCT
	CComPtr<IHdpeAlgo>          m_pImageCheckingIt;
#endif

#ifdef JELLY_PRODUCT
	CComPtr<IJellyAlgo>        m_pImageCheckingIt;
#endif

#ifdef CAN_OUTSIDE_PRODUCT
	CComPtr<ICanOutsideAlgo>   m_pImageCheckingIt;
#endif

#ifdef NAIL_PRODUCT
	CComPtr<INailAlgo>  m_pImageCheckingIt;
#endif

#ifdef BATTERY_PRODUCT
	CComPtr<IBatteryShellAlgo>  m_pImageCheckingIt;
#endif

#ifdef BOTTLE_PRODUCT
	CComPtr<IBottleCapAlgo>  m_pImageCheckingIt;
#endif

#ifdef AL_PLASTIC_PRODUCT
	CComPtr<IAlPlasticCapAlgo>  m_pImageCheckingIt;
#endif

	map<long, wstring>  m_algoCandidate;       // Function ID -- Function Name
	map<int, long>		m_mapIndexFuncId;      // List Index -- Function ID
	map<long, wstring>  m_mapFuncIdHelpStr;
	vector<CWnd*>	    m_vStaticCtrl;
	vector<CWnd*>       m_vEditCtrl;
	map<long, CEdit*>   m_dictID_WndPtr;       // Edit control's ID <--> it's pointer
	map<CWnd*, wstring>	m_dictParamHelpStr;
	set<wstring>        m_setNickname;
public:
	afx_msg void OnStnClickedStaticApply();
	afx_msg void OnStnClickedStaticClose();
	afx_msg void OnNMClickListAlgoCandidate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditParameter();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
