#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <set>

using namespace std;

#ifdef CROWN_PRODUCT
#import "CrownCapAlgo.dll" no_namespace
#endif

#ifdef EOE_PRODUCT
#import "EoeAlgo.dll" no_namespace
#endif

#ifdef PET_PRODUCT
#import "PetAlgo.dll" no_namespace
#endif

#ifdef CAN_PRODUCT
#import "CanAlgo.dll" no_namespace
#endif

#ifdef CAN_EOE_PRODUCT
#import "EoeAlgo.dll" no_namespace
#import "CanAlgo.dll" no_namespace
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

const int IDC_EDIT_CTRL         = 123456;
const int IDB_OPEN_FOLDER0      = 990000;
const int IDB_OPEN_FOLDER1      = 990001;
const int IDB_OPEN_FOLDER2      = 990002;
const int IDB_OPEN_FOLDER3      = 990003;
const int TIMER_ID_TM_EDIT_CTRL = 80808;

class CTaskMgrDlg;

class CModelMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModelMgrDlg)

public:
	CModelMgrDlg(const std::vector<int>& vProduct,CWnd* pParent = NULL); 
	virtual ~CModelMgrDlg();

	enum { IDD = IDD_DIALOG_MODEL_MGR };

	void SetCrtProductIndex(int nProductIndex);
	void SwitchToProduct(int nProductIndex);
	CTaskMgrDlg*  m_pTaskDlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_comboTaskModel;
	int       m_nModelIndex;
	CFont     m_textFont;
	CFont     m_smallFont;
	BOOL      m_bSlModel;
	CListCtrl m_candidateList;
	CTreeCtrl m_treeTaskModel;
	HTREEITEM m_crtSelectedItem;
	bool      m_bClickTreeItem;
	int       m_nMaxRgnID;
	CButton   m_btnDelete;
	CButton   m_btnModify;
	CButton   m_btnCreate;
	CButton   m_btnCopy;
	CStatic   m_btnApply;
	CStatic   m_paramHelpPanel;

	TASK_MODEL                     m_crtTaskModel;
	TASK_MODEL                     m_newTaskModel;
	std::map<int, CHANNEL_CONFIG>* m_pDictChConfig;
	std::map<wstring, TASK_MODEL>* m_pDictTaskModel;

#ifdef CROWN_PRODUCT
	CComPtr<ICrownCapAlgo> m_ptrAlgoObj;
#endif

#ifdef EOE_PRODUCT
	CComPtr<IEoeAlgo> m_ptrAlgoObj;
#endif

#ifdef CAN_PRODUCT
	CComPtr<ICanAlgo> m_ptrAlgoObj;
#endif

#ifdef CAN_EOE_PRODUCT
	CComPtr<IEoeAlgo> m_ptrAlgoObjEoe;
	CComPtr<ICanAlgo> m_ptrAlgoObjCan;
#endif

#ifdef PET_PRODUCT
	CComPtr<IPetAlgo> m_ptrAlgoObj;
#endif

#ifdef PCC_PRODUCT
	CComPtr<IPccAlgo> m_ptrAlgoObj;
#endif

#ifdef CAP_PRODUCT
	CComPtr<ICapacitorAlgo> m_ptrAlgoObj;
#endif

#ifdef MAT_PRODUCT
	CComPtr<IRubberMatAlgo> m_ptrAlgoObj;
#endif

#ifdef NEEDLE_PRODUCT
	CComPtr<INeedleAlgo> m_ptrAlgoObj;
#endif

#ifdef PLUG_PRODUCT
	CComPtr<IPlugAlgo> m_ptrAlgoObj;
#endif

#ifdef HDPE_PRODUCT
	CComPtr<IHdpeAlgo> m_ptrAlgoObj;
#endif




#ifdef JELLY_PRODUCT
	CComPtr<IJellyAlgo>  m_ptrAlgoObj;
#endif

#ifdef CAN_OUTSIDE_PRODUCT
	CComPtr<ICanOutsideAlgo>  m_ptrAlgoObj;
#endif

#ifdef NAIL_PRODUCT
	CComPtr<INailAlgo>  m_ptrAlgoObj;
#endif

#ifdef BATTERY_PRODUCT
	CComPtr<IBatteryShellAlgo>  m_ptrAlgoObj;
#endif

#ifdef BOTTLE_PRODUCT
	CComPtr<IBottleCapAlgo>  m_ptrAlgoObj;
#endif

#ifdef AL_PLASTIC_PRODUCT
	CComPtr<IAlPlasticCapAlgo>  m_ptrAlgoObj;
#endif

	const int					TIMER_ID_TM_DLG;
    std::map<HTREEITEM, FI>		m_dictTI_FI;           // Tree Item <==> Function Information;
	int							m_crtIndex_L;          // Left List
	std::map<long, wstring>		m_algoCandidate;       // Function ID -- Function Name
	std::map<int, long>			m_mapIndexFuncId_L;    // List Index -- Function ID
	std::map<long, CEdit*>      m_dictID_WndPtr;       // Edit control's ID <--> it's pointer

	int							m_crtIndex_R;          // Right List
	std::vector<FI>				m_vSelecteeAlgo; 
	std::map<long, wstring>		m_mapFuncIdHelpStr;
	std::vector<CWnd*>			m_vStaticCtrl;
	std::vector<CWnd*>          m_vEditCtrl;
	std::map<CWnd*, wstring>	m_dictParamHelpStr;  // Edit control <===> parameter help string
	std::set<CWnd*>             m_setFilePathCtrl;
	bool						m_bOpRslt;
	std::vector<int>			m_vProductId;
	int                         m_nCrtProdudctId;
	std::set<wstring>           m_setAlgoName;
private:
	void InitUiWidget(int nCtrlID, int strID = NULL, CFont* pFont = nullptr);
	void FillUpUiWidget();
	void FillUpCandidateList();
	void FillUpTaskModelTree(const TASK_MODEL& tm);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void ResetAllUiWidget();
	bool SelectFiles(CString& sNewFile);
	void EnterFilePath(int cCtrlID);
	void DeleteAllParamCtrls();
	void CreateParamCtrl(long funcId, bool bEnabled = true);
	void DisplayFuncHelpString(wstring& strHelp);
	void EnableUiCtrl(int nCtrlID, bool bEnabled = true, bool bClearTxt = true);
	void ResetJudgeRuleUiCtrl(bool bEnable);
	void EnableAllParamCtrls(bool bTestFunc);
	bool DetermingTestingFuncFlag(long funcId);
	long GetFuncId(wstring funcName);
	void RestoreParamValue(FI& fi);
	void SelectJudgeRule(wstring& strComp);
	bool UpdateAlgoParameters(int nIndex);
	bool ExtractModelSettingFromUi(TASK_MODEL& newModel);
	bool ExtractSlTaskSettingFromUi(TASK& slTask);
	TASK_MODEL GetTaskModel(wstring wstrModelName);
	void DeleteAllChildNode(HTREEITEM& hParentItem);
	void SetRegionIDForNewFunc(int nRgnID);
	int FindInsertIndex(int nCrtSelIndex);
	int FindLocationAlgoIndex(int nCrtSelIndex, const HTREEITEM& hCrtItem, HTREEITEM& hInsertAfter);
	void DeleteAlgoNode(const HTREEITEM& hAlgoItem);
	bool CheckInvalidRoiRegion(const std::vector<FI>& vFi);
public:
	afx_msg void OnEnSetfocusEditParameter();
	afx_msg void OnBnClickedButtonCreateModel();
	afx_msg void OnBnClickedButtonDeleteModel();
	afx_msg void OnBnClickedButtonModifyModel();
	afx_msg void OnCbnSelchangeComboCrtChannelId();
	afx_msg void OnStnClickedStaticBtnAddAlgo();
	afx_msg void OnStnClickedStaticBtnDeleteAlgo();
	afx_msg void OnStnClickedStaticApply();
	afx_msg void OnBnClickedCheckSlModel();
	afx_msg void OnCbnSelchangeComboSysDlgTaskModel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMClickListAlgoCandidate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeTaskModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCopyModel();
	afx_msg void OnNMClickTreeTaskModel(NMHDR *pNMHDR, LRESULT *pResult);
};
