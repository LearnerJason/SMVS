#include "stdafx.h"
#include "SMVS.h"
#include "ModelMgrDlg.h"
#include "CopyModelDlg.h"
#include "afxdialogex.h"
#include "TaskMgrDlg.h"

IMPLEMENT_DYNAMIC(CModelMgrDlg, CDialogEx)

CModelMgrDlg::CModelMgrDlg(const std::vector<int>& vProduct,CWnd* pParent /*=NULL*/)
	: CDialogEx(CModelMgrDlg::IDD, pParent)
	, m_vProductId(vProduct)
	, m_nModelIndex(0)
	, m_bSlModel(FALSE)
	, m_pDictChConfig(nullptr)
	, m_pDictTaskModel(nullptr)
	, TIMER_ID_TM_DLG(100003)
	, m_crtIndex_L(-1)
	, m_crtIndex_R(-1)
	, m_bOpRslt(true)
	,m_nCrtProdudctId(vProduct[0])
	,m_bClickTreeItem(false)
	,m_nMaxRgnID(0)
	,m_crtSelectedItem(nullptr)
	,m_pTaskDlg(nullptr)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_smallFont.CreateFont(17 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

#ifdef CAN_EOE_PRODUCT
	m_ptrAlgoObjEoe = nullptr;
	m_ptrAlgoObjCan = nullptr;
#else
	m_ptrAlgoObj   = nullptr;
#endif

	m_vSelecteeAlgo.clear();
}

CModelMgrDlg::~CModelMgrDlg()
{

#ifdef CAN_EOE_PRODUCT
	m_ptrAlgoObjEoe = nullptr;
	m_ptrAlgoObjCan = nullptr;
#else
	m_ptrAlgoObj   = nullptr;
#endif
	::CoUninitialize();
	DeleteAllParamCtrls();
}

void CModelMgrDlg::SetCrtProductIndex(int nProductIndex)
{
	if(nProductIndex < (int)m_vProductId.size())
		m_nCrtProdudctId = m_vProductId[nProductIndex];
}

void CModelMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SL_MODEL, m_bSlModel);
	DDX_Control(pDX, IDC_LIST_ALGO_CANDIDATE, m_candidateList);
	DDX_Control(pDX, IDC_BUTTON_DELETE_MODEL, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_MODIFY_MODEL, m_btnModify);
	DDX_Control(pDX, IDC_BUTTON_CREATE_MODEL, m_btnCreate);
	DDX_Control(pDX, IDC_COMBO_SYS_DLG_MODEL, m_comboTaskModel);
	DDX_Control(pDX, IDC_TREE_TASK_MODEL, m_treeTaskModel);
	DDX_Control(pDX, IDC_BUTTON_COPY_MODEL, m_btnCopy);
	DDX_Control(pDX, IDC_STATIC_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_STATIC_PARAM_DESC_PANEL, m_paramHelpPanel);
}

void CModelMgrDlg::InitUiWidget(int nCtrlID, int strID, CFont* pFont)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	if(pWnd != nullptr)
	{
		if(pFont == nullptr)
			pWnd->SetFont(&m_textFont);
		else
			pWnd->SetFont(pFont);
		if(strID != NULL)
		{
			CString str;
			str.LoadString(strID);
			pWnd->SetWindowText(str);
		}
		pWnd->Invalidate();
	}
}

void CModelMgrDlg::FillUpUiWidget()
{
	m_pDictTaskModel = gConfigMgr.GetTaskTemplateArrayPtr();
	if(m_pDictTaskModel == nullptr)
		return;
	m_comboTaskModel.ResetContent();

	m_comboTaskModel.AddString(_T(""));
	for(auto it = m_pDictTaskModel->begin(); it != m_pDictTaskModel->end(); it++)
	{
		TASK_MODEL tm = it->second;
		if(tm.m_nProductId == m_nCrtProdudctId)
		{
			m_comboTaskModel.AddString(tm.m_strModelName.c_str());
		}
	}
	m_comboTaskModel.SetCurSel(0);

	size_t sz = m_pDictChConfig->size();
	if(sz == 0) return;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	if(pCombo == nullptr)
		return;
	pCombo->ResetContent();

	CString strCh;
	for(int t = 1; t <= sz; t++)
	{
		strCh.Format(_T("CH%d"), t);
		pCombo->AddString(strCh);
	}
	pCombo->SetCurSel(-1);
}

void CModelMgrDlg::FillUpCandidateList()
{
	HRESULT hr = S_FALSE;

#ifdef CAN_EOE_PRODUCT
	if(m_nCrtProdudctId == EOE && m_ptrAlgoObjEoe != nullptr)
	{
		hr = utils::GetIDispatchMethods(m_ptrAlgoObjEoe, m_algoCandidate, m_mapFuncIdHelpStr);
	}
	if(m_nCrtProdudctId == CAN && m_ptrAlgoObjCan != nullptr)
	{
		hr = utils::GetIDispatchMethods(m_ptrAlgoObjCan, m_algoCandidate, m_mapFuncIdHelpStr);
	}
#else
	if(m_ptrAlgoObj != nullptr)
	{
		hr = utils::GetIDispatchMethods(m_ptrAlgoObj, m_algoCandidate, m_mapFuncIdHelpStr);
	}
#endif
	if(SUCCEEDED(hr))
	{
		CString strIdx;
		int index = 0;
		m_mapIndexFuncId_L.clear();
		std::wstring strHelp;
		std::for_each(m_algoCandidate.begin(), m_algoCandidate.end(), [&](std::pair<long, wstring> p)
		{
			strIdx.Format(_T("%d"), index + 1);
			m_candidateList.InsertItem(index,strIdx);
			m_candidateList.SetItemText(index, 1, W2CT(p.second.c_str()));
			m_mapIndexFuncId_L[index] = p.first;
			index++;
		});
	}
}

long CModelMgrDlg::GetFuncId(wstring funcName)
{
	if(m_algoCandidate.empty())
		return -1;
	for(auto it = m_algoCandidate.begin(); it != m_algoCandidate.end();it++)
	{
		if(it->second == funcName)
			return it->first;
	}
	return -1;
}

void CModelMgrDlg::FillUpTaskModelTree(const TASK_MODEL& tm)
{
	size_t sz = tm.m_vItParam.size();
	if(sz == 0) return;

	m_vSelecteeAlgo.clear();
	m_dictTI_FI.clear();

	HTREEITEM hItem, hSubItem;
	int nIndex = 0;
	for(size_t t = 0; t < sz; t++)
	{
		IT_PARAM itParam = tm.m_vItParam[t];
		int nRgnId = boost::lexical_cast<int>(itParam.m_vParam[1]);
		bool bTestFunc = itParam.IsTestAlgo();
		m_setAlgoName.insert(itParam.m_itNickname);
		if(!bTestFunc)
		{
			wstring wstrNodeName = itParam.m_itNickname;
			hItem = m_treeTaskModel.InsertItem(wstrNodeName.c_str(), TVI_ROOT);
			FI tagFi;
			tagFi.m_nRgnId = nRgnId;
			tagFi.m_nIndex = nIndex++;
			m_dictTI_FI[hItem] = tagFi;
			m_vSelecteeAlgo.push_back(tagFi);
			hSubItem = m_treeTaskModel.InsertItem(itParam.m_itName.c_str(), hItem);
		}
		else
		{
			hSubItem = m_treeTaskModel.InsertItem(itParam.m_itName.c_str(), hItem, hSubItem);
		}
		if(nRgnId > m_nMaxRgnID)
		{
			m_nMaxRgnID = nRgnId;
		}

		long funcId = GetFuncId(itParam.m_itName);
		FI fi(nIndex,funcId, nRgnId, itParam.m_itName, itParam.m_itNickname, bTestFunc);
		nIndex++;
		fi.m_vParam = itParam.m_vParam;
		m_dictTI_FI[hSubItem] = fi;
		m_vSelecteeAlgo.push_back(fi);
	}
	m_nMaxRgnID++;
	m_crtSelectedItem = nullptr;
}

BOOL CModelMgrDlg::OnInitDialog()
{
	::CoInitialize(NULL);

	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);	
	HRESULT hr;
#ifdef CROWN_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(CrownCapAlgo));
#endif

#ifdef EOE_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(EoeAlgo));
#endif

#ifdef CAN_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(CanAlgo));
#endif

#ifdef CAN_EOE_PRODUCT
		hr = m_ptrAlgoObjEoe.CoCreateInstance(__uuidof(EoeAlgo));
		hr = m_ptrAlgoObjCan.CoCreateInstance(__uuidof(CanAlgo));
#endif

#ifdef PET_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(PetAlgo));
#endif

#ifdef JELLY_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(JellyAlgo));
#endif

#ifdef PCC_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(PccAlgo));
#endif

#ifdef CAP_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(CapacitorAlgo));
#endif

#ifdef MAT_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(RubberMatAlgo));
#endif

#ifdef NEEDLE_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(NeedleAlgo));
#endif

#ifdef PLUG_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(PlugAlgo));
#endif

#ifdef HDPE_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(HdpeAlgo));
#endif

#ifdef CAN_OUTSIDE_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(CanOutsideAlgo));
#endif

#ifdef NAIL_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(NailAlgo));
#endif

#ifdef BATTERY_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(BatteryShellAlgo));
#endif

#ifdef BOTTLE_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(BottleCapAlgo));
#endif

#ifdef AL_PLASTIC_PRODUCT
		hr = m_ptrAlgoObj.CoCreateInstance(__uuidof(AlPlasticCapAlgo));
#endif

	m_pDictChConfig = gConfigMgr.GetChannelConfigPtr(); 
	if(m_pDictChConfig == nullptr)
		return FALSE;

	InitUiWidget(IDC_STATIC_MODEL_NAME, IDS_STRING_MODEL_NAME);
	InitUiWidget(IDC_COMBO_SYS_DLG_MODEL);
	InitUiWidget(IDC_CHECK_SL_MODEL,IDS_STRING_SL_MODEL);

	InitUiWidget(IDC_STATIC_SAMPLE_COUNT,IDS_STRING_SAMPLE_COUNT);
	InitUiWidget(IDC_EDIT_SAMPLE_COUNT);

	InitUiWidget(IDC_STATIC_SYS_DLG_CRT_CH, IDS_STRING_SYS_DLG_CRT_CH);
	InitUiWidget(IDC_COMBO_CRT_CHANNEL_ID);

	InitUiWidget(IDC_BUTTON_CREATE_MODEL,IDS_STRING_CREATE_MODEL);
	InitUiWidget(IDC_BUTTON_DELETE_MODEL,IDS_STRING_DELETE_MODEL);
	InitUiWidget(IDC_BUTTON_MODIFY_MODEL,IDS_STRING_MODIFY_MODEL);
	InitUiWidget(IDC_BUTTON_COPY_MODEL,IDS_STRING_MM_COPY_MODEL);
	InitUiWidget(IDC_STATIC_MODEL_DLG_MSG_PANEL,NULL, &m_smallFont);

	InitUiWidget(IDC_STATIC_TM_DLG_ALGO_DESC, NULL, &m_smallFont);
	InitUiWidget(IDC_STATIC_TEST_ITEM_PARAM,IDS_STRING_ALGO_PARAM,&m_smallFont);
	InitUiWidget(IDC_STATIC_JUDGE_RULE, IDS_STRING_JUDGE_RULE,&m_smallFont);
	InitUiWidget(IDC_COMBO_JUDGE_RULE, NULL, &m_smallFont);
	InitUiWidget(IDC_EDIT_JUDGE_RULE, NULL, &m_smallFont);
	InitUiWidget(IDC_STATIC_PARAM_DESC_PANEL, NULL, &m_smallFont);

	InitUiWidget(IDC_STATIC_ITEM_NAME,IDS_STRING_ITEM_NAME, &m_smallFont);
	InitUiWidget(IDC_EDIT_ITEM_NAME, NULL, &m_smallFont);

	DWORD dwStyleEx = m_candidateList.GetExtendedStyle();
	dwStyleEx |= (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES); 
	m_candidateList.SetExtendedStyle(dwStyleEx);

	m_candidateList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strIndex(MAKEINTRESOURCE(IDS_STRING_INDEX));
	CString strAlgoIt(MAKEINTRESOURCE(IDS_STRING_ALGO_ITERFACE));
	m_candidateList.InsertColumn(0, strIndex, LVCFMT_CENTER, 48);
	m_candidateList.InsertColumn(1, strAlgoIt, LVCFMT_LEFT, 250);
	m_candidateList.SetFont(&m_smallFont);

	m_treeTaskModel.SetFont(&m_smallFont);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
	for(auto it = utils::SYMBOL_TYPE.begin(); it != utils::SYMBOL_TYPE.end();it++)
	{
		wstring str = it->m_strComp;
		pCombo->AddString(CW2T(str.c_str()));
	}
	pCombo->SetCurSel(-1);

	CWnd* pWnd = GetDlgItem(IDC_EDIT_SAMPLE_COUNT);
	pWnd->EnableWindow(m_bSlModel);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	pCombo->EnableWindow(m_bSlModel);

	m_btnDelete.EnableWindow(FALSE);
	m_btnModify.EnableWindow(FALSE);
	m_btnCopy.EnableWindow(FALSE);
	FillUpUiWidget();
	FillUpCandidateList();
	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CModelMgrDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_MODEL, &CModelMgrDlg::OnBnClickedButtonCreateModel)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MODEL, &CModelMgrDlg::OnBnClickedButtonDeleteModel)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_MODEL, &CModelMgrDlg::OnBnClickedButtonModifyModel)
	ON_CBN_SELCHANGE(IDC_COMBO_CRT_CHANNEL_ID, &CModelMgrDlg::OnCbnSelchangeComboCrtChannelId)
	ON_STN_CLICKED(IDC_STATIC_BTN_ADD_ALGO, &CModelMgrDlg::OnStnClickedStaticBtnAddAlgo)
	ON_STN_CLICKED(IDC_STATIC_BTN_DELETE_ALGO, &CModelMgrDlg::OnStnClickedStaticBtnDeleteAlgo)
	ON_STN_CLICKED(IDC_STATIC_APPLY, &CModelMgrDlg::OnStnClickedStaticApply)
	ON_BN_CLICKED(IDC_CHECK_SL_MODEL, &CModelMgrDlg::OnBnClickedCheckSlModel)
	ON_CBN_SELCHANGE(IDC_COMBO_SYS_DLG_MODEL, &CModelMgrDlg::OnCbnSelchangeComboSysDlgTaskModel)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALGO_CANDIDATE, &CModelMgrDlg::OnNMClickListAlgoCandidate)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TASK_MODEL, &CModelMgrDlg::OnTvnSelchangedTreeTaskModel)
	ON_EN_SETFOCUS(IDC_EDIT_CTRL,&CModelMgrDlg::OnEnSetfocusEditParameter)
	ON_BN_CLICKED(IDC_BUTTON_COPY_MODEL, &CModelMgrDlg::OnBnClickedButtonCopyModel)
	ON_NOTIFY(NM_CLICK, IDC_TREE_TASK_MODEL, &CModelMgrDlg::OnNMClickTreeTaskModel)
END_MESSAGE_MAP()

TASK_MODEL CModelMgrDlg::GetTaskModel(wstring wstrModelName)
{
	TASK_MODEL retModel;
	if(m_pDictTaskModel == nullptr)
		return retModel;
	for(auto it = m_pDictTaskModel->begin(); it != m_pDictTaskModel->end(); it++)
	{
		if(it->second.m_strModelName == wstrModelName)
		{
			retModel = it->second;
		}
	}
	return retModel;
}

void CModelMgrDlg::OnCbnSelchangeComboSysDlgTaskModel()
{
	int index = m_comboTaskModel.GetCurSel();
	if(index < 0)
		return;

	m_vSelecteeAlgo.clear();
	m_dictTI_FI.clear();

	m_nModelIndex = index;
	m_nMaxRgnID   = 0;
	if(m_nModelIndex == 0)
	{
		m_btnCreate.EnableWindow(TRUE);
		m_btnDelete.EnableWindow(FALSE);
		m_btnModify.EnableWindow(FALSE);
		m_btnCopy.EnableWindow(FALSE);
		ResetAllUiWidget();
		return;
	}
	CString strRet;
	m_comboTaskModel.GetLBText(index, strRet);
	wstring strModelName = CT2W(strRet);
	if(m_pDictTaskModel != nullptr && m_pDictTaskModel->find(strModelName) != m_pDictTaskModel->end())
	{
		m_btnCreate.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(TRUE);
		m_btnModify.EnableWindow(TRUE);
		m_btnCopy.EnableWindow(TRUE);
		TASK_MODEL tm = GetTaskModel(strModelName);
		m_crtTaskModel = tm;
		ResetAllUiWidget();
		FillUpTaskModelTree(tm);
	}
	UpdateData(FALSE);
}

void CModelMgrDlg::OnBnClickedCheckSlModel()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_MODEL);
	pCombo->EnableWindow(!m_bSlModel);
	pCombo->SetCurSel(-1);
	m_nModelIndex = -1;

	CWnd* pWnd = GetDlgItem(IDC_EDIT_SAMPLE_COUNT);
	pWnd->EnableWindow(m_bSlModel);
	if(m_bSlModel)
		SetDlgItemTextW(IDC_EDIT_SAMPLE_COUNT, _T(""));

	pWnd = GetDlgItem(IDC_BUTTON_CREATE_MODEL);
	pWnd->EnableWindow(!m_bSlModel && m_nModelIndex >=0);

	pWnd = GetDlgItem(IDC_BUTTON_DELETE_MODEL);
	pWnd->EnableWindow(!m_bSlModel && m_nModelIndex >=0);

	pWnd = GetDlgItem(IDC_BUTTON_MODIFY_MODEL);
	pWnd->EnableWindow(!m_bSlModel && m_nModelIndex >=0);

	pWnd = GetDlgItem(IDC_STATIC_BTN_ADD_ALGO);
	pWnd->EnableWindow(!m_bSlModel);

	pWnd = GetDlgItem(IDC_STATIC_BTN_DELETE_ALGO);
	pWnd->EnableWindow(!m_bSlModel);

	m_candidateList.EnableWindow(!m_bSlModel);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	pCombo->EnableWindow(m_bSlModel);
	pCombo->SetCurSel(-1);
	ResetAllUiWidget();

	UpdateData(FALSE);
}

void CModelMgrDlg::OnCbnSelchangeComboCrtChannelId()
{
	ResetAllUiWidget();

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	if(pCombo == nullptr)
		return;
	int index = pCombo->GetCurSel();
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_MODIFY_MODEL);
	pWnd->EnableWindow(index != -1);

	if(m_pDictChConfig->find(index + 1) != m_pDictChConfig->end())
	{
		CHANNEL_CONFIG chConfig = (*m_pDictChConfig)[index + 1];
		for(auto it = chConfig.m_vTask.begin(); it != chConfig.m_vTask.end(); it++)
		{
			if(it->m_bSelfLearning)
			{
				CString strName = CW2T(it->m_wstrTaskName.c_str());
				SetDlgItemTextW(IDC_COMBO_SYS_DLG_MODEL, strName);
				SetDlgItemInt(IDC_EDIT_SAMPLE_COUNT, it->m_nSampleCount);
			}
		}
	}
	UpdateData(FALSE);
}

bool CModelMgrDlg::ExtractModelSettingFromUi(TASK_MODEL& newModel)
{
	UpdateData(TRUE);
	CString sModelName;
	GetDlgItemTextW(IDC_COMBO_SYS_DLG_MODEL, sModelName);
	sModelName.Trim();
	CString sMsg;
	if(sModelName.GetLength() <= 0)
	{
		sMsg.LoadString(IDS_STRING_MSG_MODEL_NAME_EMPTY);
		DisplayPromptMessage(sMsg, false);
		return false;
	}
	wstring wstrModelName = CT2W(sModelName);
	if(utils::CheckSpecialCharacter(wstrModelName))
	{
		sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
		DisplayPromptMessage(sMsg, false);
		return false;
	}
	newModel.m_strModelName = CT2W(sModelName);
	if(m_vSelecteeAlgo.empty())
	{
		sMsg.LoadString(IDS_STRING_EMPTY_TASK_TEMPLATE);
		DisplayPromptMessage(sMsg, false);
		return false;
	}
	// Prepare Initialization Settings
	CString sValue;
	int index = 0;
	newModel.m_nProductId = m_nCrtProdudctId;
	for(size_t i = 0; i < m_vSelecteeAlgo.size(); i++)
	{
		FI fi = m_vSelecteeAlgo[i];
		if(fi.m_funcId == -1)
		{
			continue;
		}
		IT_PARAM itParam;
		itParam.m_itIndex    = boost::lexical_cast<wstring>(index++);
		itParam.m_itName     = fi.m_strFuncName;
		itParam.m_itNickname = fi.m_strNickname;
		if(fi.m_vParam.empty())
		{
			sMsg.LoadString(IDS_STRING_WARNING_EMPTY_PARAM);
			CString sIndex;
			sIndex.Format(_T("%d"), i + 1);
			sMsg.Replace(_T("#1"), sIndex);
			sMsg.Replace(_T("#2"), fi.m_strFuncName.c_str());
			DisplayPromptMessage(sMsg, false);
			return false;
		}
		itParam.m_vParam = fi.m_vParam;
		newModel.m_vItParam.push_back(itParam);
	}
	return true;
}

bool CModelMgrDlg::ExtractSlTaskSettingFromUi(TASK& slTask)
{
	UpdateData(TRUE);
	CString sMsg;

	CString slTaskName;
	GetDlgItemTextW(IDC_COMBO_SYS_DLG_MODEL, slTaskName);
	slTask.m_wstrTaskName = CT2W(slTaskName);

	CString strCount;
	GetDlgItemTextW(IDC_EDIT_SAMPLE_COUNT, strCount);
	if(!utils::IsNumericStringEx(strCount,utils::Type_Positive_integer))
	{
		sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_POSITIVE_INTEGER);
		DisplayPromptMessage(sMsg, false);
		return false;
	}
	wstring wstrCount = CT2W(strCount);
	slTask.m_nSampleCount = boost::lexical_cast<long>(wstrCount);
	return true;
}

void CModelMgrDlg::OnBnClickedButtonCreateModel()
{
	TASK_MODEL newModel;
	bool bRet = ExtractModelSettingFromUi(newModel);
	if(!bRet)
		return;

	CString sMsg;
	bool bRslt = true;
	if(gConfigMgr.IsTaskModelExisted(newModel.m_strModelName))
	{
		sMsg.LoadString(IDS_STRING_MSG_MODEL_EXISTED);
		sMsg.Replace(_T("#1"), newModel.m_strModelName.c_str());
		DisplayPromptMessage(sMsg, false);
		return;
	}
	if(gConfigMgr.AddTaskTemplate(newModel))
	{
		m_pDictTaskModel = gConfigMgr.GetTaskTemplateArrayPtr();
		m_comboTaskModel.AddString(newModel.m_strModelName.c_str());
		int count = m_comboTaskModel.GetCount();
		m_comboTaskModel.SetCurSel(count - 1);
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		m_crtTaskModel = newModel;

		m_nModelIndex = m_comboTaskModel.GetCurSel();
		m_btnCreate.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(TRUE);
		m_btnModify.EnableWindow(TRUE);
		m_btnCopy.EnableWindow(TRUE);

		if(m_pTaskDlg != nullptr)
		{
			m_pTaskDlg->PrepareTaskModelList(m_nCrtProdudctId);
		}
	}
	else
	{
		bRslt = false;
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
	}
	DisplayPromptMessage(sMsg, bRslt);
}

void CModelMgrDlg::OnBnClickedButtonDeleteModel()
{
	int index = m_nModelIndex;
	if(index <= 0)
		return;

	CString crtModelName;
	m_comboTaskModel.GetLBText(index, crtModelName);
	m_comboTaskModel.DeleteString(index);
	m_comboTaskModel.SetCurSel(0);
	wstring strName = CT2W(crtModelName);
	CString sMsg;
	bool bRslt = true;
	if(gConfigMgr.DeleteTaskTemplate(strName))
	{
		m_pDictTaskModel = gConfigMgr.GetTaskTemplateArrayPtr();
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		ResetAllUiWidget();
		DeleteAllParamCtrls();
		m_vSelecteeAlgo.clear();
		m_dictTI_FI.clear();
		if(m_pTaskDlg != nullptr)
		{
			m_pTaskDlg->PrepareTaskModelList(m_nCrtProdudctId);
		}
	}
	else
	{
		bRslt = false;
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
	}
	DisplayPromptMessage(sMsg, bRslt);
	m_btnCreate.EnableWindow(TRUE);
	m_btnDelete.EnableWindow(FALSE);
	m_btnModify.EnableWindow(FALSE);
	m_btnCopy.EnableWindow(FALSE);
}

void CModelMgrDlg::OnBnClickedButtonModifyModel()
{
	CString sMsg;
	bool bRslt = true;

	if(!m_bSlModel)
	{
		int index = m_nModelIndex;
		if(index <= 0)
			return;

		TASK_MODEL newModel;
		if(!ExtractModelSettingFromUi(newModel)) 
			return;

		if(gConfigMgr.ModifyTaskTemplate(m_crtTaskModel,newModel))
		{
			m_pDictTaskModel = gConfigMgr.GetTaskTemplateArrayPtr();
			m_comboTaskModel.DeleteString(m_nModelIndex);
			m_comboTaskModel.InsertString(m_nModelIndex, newModel.m_strModelName.c_str());
			m_comboTaskModel.SetCurSel(m_nModelIndex);
			m_crtTaskModel = newModel;
			ResetAllUiWidget();
			FillUpTaskModelTree(newModel);
			sMsg.LoadString(IDS_STRING_OPERATION_OK);
		    if(m_pTaskDlg != nullptr)
			{
				m_pTaskDlg->PrepareTaskModelList(m_nCrtProdudctId);
			}
		}
		else
		{
			bRslt = false;
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
		}
		DisplayPromptMessage(sMsg, bRslt);	
	}
	else
	{
		TASK slTask;
		if(!ExtractSlTaskSettingFromUi(slTask))
			return;
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
		int chIndex = pCombo->GetCurSel() + 1;

		if(chIndex >= 1 && gConfigMgr.ModifySlTask(chIndex, slTask))
		{
			m_pDictChConfig = gConfigMgr.GetChannelConfigPtr();
			sMsg.LoadString(IDS_STRING_OPERATION_OK);
		}
		else
		{
			bRslt = false;
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
		}
		DisplayPromptMessage(sMsg, bRslt);
	}
}

void CModelMgrDlg::OnBnClickedButtonCopyModel()
{
	gSystemMgr.SetNewModelName(_T(""));

	CCopyModelDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString sMsg;
		CString sNewModelName = gSystemMgr.GetNewModelName();
		wstring wstrNewName = CT2W(sNewModelName);
		if(m_pDictTaskModel->find(wstrNewName) != m_pDictTaskModel->end())
		{
			sMsg.LoadString(IDS_STRING_MSG_MODEL_EXISTED);
			DisplayPromptMessage(sMsg, false);
			return;
		}

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SYS_DLG_MODEL);
		int nIndex = pCombo->GetCurSel();
		if(nIndex != -1)
		{
			CString sSrcModelName;
			pCombo->GetLBText(nIndex,sSrcModelName);
			wstring wstrSrcName = CT2W(sSrcModelName);

			if(!gConfigMgr.CopyModel(wstrSrcName, wstrNewName))
			{
				sMsg.LoadString(IDS_STRING_OPERATION_NG);
				DisplayPromptMessage(sMsg, false);
				return;
			}
			else
			{
				m_pDictTaskModel = gConfigMgr.GetTaskTemplateArrayPtr();
				m_comboTaskModel.AddString(wstrNewName.c_str());
				int count = m_comboTaskModel.GetCount();
				m_comboTaskModel.SetCurSel(count - 1);
				m_nModelIndex = m_comboTaskModel.GetCurSel();

				if(m_pDictTaskModel->find(wstrNewName) != m_pDictTaskModel->end())
				{
					m_crtTaskModel = (*m_pDictTaskModel)[wstrNewName];
				}
				sMsg.LoadString(IDS_STRING_OPERATION_OK);
				DisplayPromptMessage(sMsg, true);

			    if(m_pTaskDlg != nullptr)
				{
					m_pTaskDlg->PrepareTaskModelList(m_nCrtProdudctId);
				}
			}
		}
	}
}

void CModelMgrDlg::EnableAllParamCtrls(bool bTestFunc)
{
	std::for_each(m_vEditCtrl.begin(), m_vEditCtrl.end(),[&](CWnd* pCtrl)
	{
		pCtrl->EnableWindow(TRUE);
	});
	EnableUiCtrl(IDC_EDIT_ITEM_NAME, true);
	EnableUiCtrl(IDC_COMBO_JUDGE_RULE, bTestFunc);
	EnableUiCtrl(IDC_EDIT_JUDGE_RULE, bTestFunc);
}

void CModelMgrDlg::SetRegionIDForNewFunc(int nRgnID)
{
	size_t sz = m_vEditCtrl.size();
	if(sz < 1)
	{
		return;
	}
	CEdit* pRgnEdit = (CEdit*)m_vEditCtrl[0];
	wstring wstrID = utils::to_wstring(nRgnID);
	pRgnEdit->SetWindowText(wstrID.c_str());
}

int CModelMgrDlg::FindInsertIndex(int nCrtSelIndex)
{
	int sz = (int)m_vSelecteeAlgo.size();
	if(sz == 0 || nCrtSelIndex < 0 || nCrtSelIndex >= sz)
	{
		return -1;
	}
	for(int i = 0; i < sz; i++)
	{
		if(i <= nCrtSelIndex)
		{
			continue;
		}
		if(m_vSelecteeAlgo[i].m_funcId == -1)
		{
			return i;
		}
	}

	return sz;
}

int CModelMgrDlg::FindLocationAlgoIndex(int nCrtSelIndex, const HTREEITEM& hCrtItem, HTREEITEM& hInsertAfter)
{
	hInsertAfter = TVI_LAST;
	int sz = (int)m_vSelecteeAlgo.size();
	if(sz == 0 || nCrtSelIndex < 0 || nCrtSelIndex >= sz)
	{
		return sz;
	}
	if(nCrtSelIndex == -1 || hCrtItem == nullptr)
	{
		return sz;
	}
	else
	{

		if(m_treeTaskModel.ItemHasChildren(hCrtItem))
		{
			hInsertAfter = hCrtItem;
		}
		else
		{
			hInsertAfter = m_treeTaskModel.GetParentItem(hCrtItem);
		}
		for(int i = nCrtSelIndex + 1; i < sz; i++)
		{
			if(m_vSelecteeAlgo[i].m_funcId == -1)
			{
				return i;
			}
		}
	}
	return sz;
}

bool CModelMgrDlg::CheckInvalidRoiRegion(const std::vector<FI>& vFi)
{
	size_t sz = vFi.size();
	if(sz <= 3) return true;

	std::set<int> setRoiRgnID;
	std::set<int> setTestRgnID;
	for(size_t t = 2; t < vFi.size();t++)
	{
		FI fi = vFi[t];
		if(fi.m_funcId != -1)
		{
			bool bTestFunc = DetermingTestingFuncFlag(fi.m_funcId);
			if(bTestFunc)
			{
				setTestRgnID.insert(fi.m_nRgnId);
			}
			else
			{
				setRoiRgnID.insert(fi.m_nRgnId);
			}
		}
	}
	if(setRoiRgnID.size() - setTestRgnID.size() == 1)
	{
		CString sMsg;
		sMsg.LoadString(IDS_STRING_EMPTY_ROI);
		DisplayPromptMessage(sMsg,false);
		return false;
	}
	return true;
}

void CModelMgrDlg::OnStnClickedStaticBtnAddAlgo()
{
	if(m_bClickTreeItem)
	{
		return;
	}
	if(m_crtIndex_L != -1)
	{
		CString sMsg;
		HTREEITEM hItem, hSubItem;

		long funcId      = m_mapIndexFuncId_L[m_crtIndex_L];
		wstring funcName = m_algoCandidate[funcId];
		bool bTestFunc   = DetermingTestingFuncFlag(funcId);
		int nRgnID       = m_nMaxRgnID;
		int nInsertIndex = (int)m_vSelecteeAlgo.size();
		if(!bTestFunc) // Add a location function
		{
			if(!CheckInvalidRoiRegion(m_vSelecteeAlgo))
			{
				return;
			}
			HTREEITEM hInsertAfter = TVI_LAST;
			nInsertIndex = FindLocationAlgoIndex(m_crtIndex_R, m_crtSelectedItem, hInsertAfter);
			hItem = m_treeTaskModel.InsertItem(_T("NewRegion"), TVI_ROOT, hInsertAfter);

			FI tagFi; 
			tagFi.m_nRgnId      = m_nMaxRgnID;
			tagFi.m_nIndex      = nInsertIndex;

			//insert in the middle, update the m_dictTI_FI.m_index
			for (auto it = m_dictTI_FI.begin(); it != m_dictTI_FI.end(); it++)
			{
				if (it->second.m_nIndex >= tagFi.m_nIndex)
				{
					it->second.m_nIndex++;
				}
			}

			m_dictTI_FI[hItem] = tagFi;

			
	
			if(nInsertIndex == m_vSelecteeAlgo.size())
			{
				m_vSelecteeAlgo.push_back(tagFi);
				nInsertIndex = (int)m_vSelecteeAlgo.size();
			}
			else
			{
				//insert in the middle, update the m_vSelecteeAlgo.m_index
				for (auto it = m_vSelecteeAlgo.begin(); it != m_vSelecteeAlgo.end(); it++)
				{
					if (it->m_nIndex >= tagFi.m_nIndex)
					{
						it->m_nIndex++;
					}
				}
				m_vSelecteeAlgo.insert(m_vSelecteeAlgo.begin() + nInsertIndex, tagFi);
				nInsertIndex++;
			}
			hSubItem = m_treeTaskModel.InsertItem(funcName.c_str(), hItem);
			m_nMaxRgnID++;
			m_treeTaskModel.Expand(hItem, TVE_EXPAND);
			m_crtSelectedItem = hSubItem;
			
		}
		else // Add a testing function
		{
			HTREEITEM hInsertAfter = TVI_LAST;
			if(m_crtSelectedItem != nullptr)
			{ 
				if(m_crtIndex_R == 0 || m_crtIndex_R == 1)
				{
					sMsg.LoadString(IDS_STRING_MSG_CENTER_ALGO_CANNOT_BE_FOLLOWED_BY_TEST_ALGO);
					DisplayPromptMessage(sMsg, false);
					return;
				}
				if(m_crtIndex_R != -1 && m_crtIndex_R < m_vSelecteeAlgo.size())
				{
					nRgnID = m_vSelecteeAlgo[m_crtIndex_R].m_nRgnId;
				}
				if(m_treeTaskModel.ItemHasChildren(m_crtSelectedItem))
				{
					hSubItem = m_treeTaskModel.InsertItem(funcName.c_str(), m_crtSelectedItem);
					m_treeTaskModel.Expand(m_crtSelectedItem, TVE_EXPAND);
					nInsertIndex = FindInsertIndex(m_crtIndex_R);
				}
				else
				{
					HTREEITEM hParent = m_treeTaskModel.GetParentItem(m_crtSelectedItem);
					hSubItem = m_treeTaskModel.InsertItem(funcName.c_str(), hParent, m_crtSelectedItem);
					nInsertIndex = m_crtIndex_R + 1;
				}
				m_crtSelectedItem = hSubItem;
			}
			else
			{
				sMsg.LoadString(IDS_STRING_MSG_PLZ_SELECT_TARGET_NODE);
				DisplayPromptMessage(sMsg, false);
				return;
			}
		}
		FI fi(nInsertIndex,funcId, nRgnID, funcName, L"", bTestFunc);
		int nTmpIndex = 0;
		for(auto it = m_dictTI_FI.begin(); it != m_dictTI_FI.end(); it++)
		{
			if(it->second.m_nIndex >= nInsertIndex)
			{
				it->second.m_nIndex++;
			}
			if(m_vSelecteeAlgo[nTmpIndex].m_nIndex >= nInsertIndex)
			{
				m_vSelecteeAlgo[nTmpIndex].m_nIndex++;
			}
			nTmpIndex++;
		}
		m_dictTI_FI[hSubItem] = fi;
		if(nInsertIndex == m_vSelecteeAlgo.size())
		{
			m_vSelecteeAlgo.push_back(fi); //TODO:
		}
		else
		{
			m_vSelecteeAlgo.insert(m_vSelecteeAlgo.begin() + nInsertIndex, fi);
		}
		
		EnableAllParamCtrls(bTestFunc);
		EnableUiCtrl(IDC_STATIC_APPLY, true, false);
		m_crtIndex_L = -1;
		m_treeTaskModel.SelectItem(hSubItem);
	}
}

void CModelMgrDlg::DeleteAlgoNode(const HTREEITEM& hAlgoItem)
{
	FI fi = m_dictTI_FI[hAlgoItem];
	int index = fi.m_nIndex;
	if(m_setAlgoName.find(fi.m_strNickname) != m_setAlgoName.end())
	{
		m_setAlgoName.erase(fi.m_strNickname);
	}
	
	auto vAlgo = m_vSelecteeAlgo;
	m_vSelecteeAlgo.clear();
	for(size_t t = 0; t < vAlgo.size(); t++)
	{
		if(vAlgo[t].m_nIndex < index)
		{
			m_vSelecteeAlgo.push_back(vAlgo[t]);
		}
		if(vAlgo[t].m_nIndex > index)
		{
			vAlgo[t].m_nIndex--;
			m_vSelecteeAlgo.push_back(vAlgo[t]);
		}
	}
	for(auto dictIt = m_dictTI_FI.begin(); dictIt != m_dictTI_FI.end(); dictIt++)
	{
		if(dictIt->second.m_nIndex > index)
		{
			dictIt->second.m_nIndex--;
		}
	}
	m_dictTI_FI.erase(hAlgoItem);
}

void CModelMgrDlg::DeleteAllChildNode(HTREEITEM& hParentItem)
{
	int sz = (int)m_dictTI_FI.size();
	if(hParentItem == nullptr || sz == 0)
	{
		return;
	}
	BOOL bHasChild = m_treeTaskModel.ItemHasChildren(hParentItem);
	if(bHasChild == FALSE)
	{
		DeleteAlgoNode(hParentItem);
	}
	else
	{
		HTREEITEM hChild = m_treeTaskModel.GetChildItem(hParentItem);
		while(hChild)
		{
			DeleteAlgoNode(hChild);
			hChild = m_treeTaskModel.GetNextItem(hChild,TVGN_NEXT);
		}
		DeleteAlgoNode(hParentItem);
	}
	m_treeTaskModel.DeleteItem(hParentItem);
}

void CModelMgrDlg::OnStnClickedStaticBtnDeleteAlgo()
{
	if(m_bClickTreeItem == false)
	{
		return;
	}

	FI fi = m_dictTI_FI[m_crtSelectedItem];
	int index = fi.m_nIndex;
	//delete locationalgo  <==> delete all
	if(fi.m_bTestFunc ==NULL)
	{
		HTREEITEM hParent = m_treeTaskModel.GetParentItem(m_crtSelectedItem);
		if (hParent != NULL)
		{
			DeleteAllChildNode(hParent);
		}
		else
		{
			DeleteAllChildNode(m_crtSelectedItem);
		}
	}
	else
	{
		DeleteAllChildNode(m_crtSelectedItem);
	}


	DeleteAllParamCtrls();
	wstring strMsg(L"");
	DisplayFuncHelpString(strMsg);
	EnableUiCtrl(IDC_COMBO_JUDGE_RULE,false);
	EnableUiCtrl(IDC_EDIT_JUDGE_RULE,false);
	EnableUiCtrl(IDC_EDIT_ITEM_NAME, false);
	EnableUiCtrl(IDC_STATIC_APPLY, false, false);
}

bool CModelMgrDlg::UpdateAlgoParameters(int nIndex)
{
	if(m_vStaticCtrl.empty())
		return false;
	CString sMsg;
	int sz = (int)m_vSelecteeAlgo.size();
	if(nIndex >= 0 && nIndex < sz)
	{
		CString sNickname;
		GetDlgItemTextW(IDC_EDIT_ITEM_NAME, sNickname);
		if(sNickname.GetLength() <= 0)
		{
			sMsg.LoadString(IDS_STRING_WARNING_ALGO_NICKNAME_EMPTY);
			DisplayPromptMessage(sMsg,false);
			return false;
		}
		wstring wstrNewName = CT2W(sNickname);
		if(utils::CheckSpecialCharacter(wstrNewName))
		{
			sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
			DisplayPromptMessage(sMsg, false);
			return false;
		}
		FI& fi = m_vSelecteeAlgo[nIndex];
		FI  fiBk = fi; // backup
		wstring wstrOldName = fi.m_strNickname;
		bool isNumeric = false;

		if(m_setAlgoName.find(wstrNewName) != m_setAlgoName.end())
		{
			sMsg.LoadString(IDS_STRING_DUPLICATED_TEST_ALGO_NAME);
			sMsg.Replace(_T("#1"), wstrNewName.c_str());
			m_setAlgoName.insert(wstrOldName);
			DisplayPromptMessage(sMsg,false);
			return false;
		}
		m_setAlgoName.erase(wstrOldName);
		m_setAlgoName.insert(fi.m_strNickname);

		if(fi.m_bTestFunc)
		{
			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
			int index = pCombo->GetCurSel();
			if(index == -1)
			{
				sMsg.LoadString(IDS_STRING_MSG_PLZ_SET_JUDGEMENT_RULE);
				DisplayPromptMessage(sMsg, false);
				return false;
			}
			CString strValue;
			GetDlgItemTextW(IDC_EDIT_JUDGE_RULE, strValue);
			if(strValue.GetLength()  <= 0)
			{
				sMsg.LoadString(IDS_STRING_PARAM_VALUE_CANNOT_BE_EMPTY);
				DisplayPromptMessage(sMsg, false);
				return false;
			}
			
			if(index <= 5)
			{
				isNumeric = utils::IsNumericString(strValue);
				if(!isNumeric)
				{
					sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
					DisplayPromptMessage(sMsg, false);
					return false;
				}
			}
			else
			{
				if(!utils::CheckRangeParameter(strValue))
				{
					sMsg.LoadString(IDS_STRING_INVALID_RANGE);
					DisplayPromptMessage(sMsg, false);
					return false;
				}
			}
			fi.m_strNickname = wstrNewName;
			fi.m_vParam.clear();
			CString strRet;
			pCombo->GetLBText(index,strRet);

			CString strRule;
			strRule.Append(strRet);
			strRule.Append(_T("$"));
			strRule.Append(strValue);
			fi.m_vParam.push_back(strRule.GetString());
		}
		else
		{
			fi.m_strNickname = CT2W(sNickname);
			fi.m_vParam.clear();
			fi.m_vParam.push_back(L"N/A");
			// Update node name;
			HTREEITEM hParent = m_treeTaskModel.GetParentItem(m_crtSelectedItem);
			if(hParent != NULL)
			{
				m_treeTaskModel.SetItemText(hParent,sNickname);
			}
		}
		for(int i = 0; i < m_vEditCtrl.size(); i++)
		{
			CWnd* pCtrl = m_vEditCtrl[i];
			CString sParam;
			pCtrl->GetWindowText(sParam);
			if(sParam.GetLength() <= 0 || m_setFilePathCtrl.find(pCtrl) != m_setFilePathCtrl.end())
			{
				fi.m_vParam.push_back(L" ");
			}
			else
			{
			    isNumeric = utils::IsNumericString(sParam);
				if(!isNumeric)
				{
					sMsg.LoadString(IDS_STRING_PARAM_MUST_BE_VALUE);
					DisplayPromptMessage(sMsg, false);
					fi = fiBk; // Restore original data.
					return false;
				}
				wstring wstrParam = sParam.GetString();
				fi.m_vParam.push_back(wstrParam);
			}
		}
		m_dictTI_FI[m_crtSelectedItem] = fi;
	}
	return true;
}

void CModelMgrDlg::OnStnClickedStaticApply()
{
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MODEL_DLG_MSG_PANEL);
	pWnd->SetWindowText(_T(""));
	pWnd->ShowWindow(SW_HIDE);
	Sleep(2);
	pWnd->ShowWindow(SW_SHOW);

	bool ret = UpdateAlgoParameters(m_crtIndex_R);
	CString sMsg;
	if(ret)
	{
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		DisplayPromptMessage(sMsg);
	}
}

HBRUSH CModelMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_CHECK_SL_MODEL || 
	   id == IDC_STATIC_MODEL_DLG_MSG_PANEL ||
	   id == IDC_STATIC_TM_DLG_ALGO_DESC ||
	   id == IDC_STATIC_PARAM_DESC_PANEL ||
	   id == IDC_SLIDER_LUM1)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		if(id == IDC_STATIC_MODEL_DLG_MSG_PANEL)
		{
			if(m_bOpRslt)
				pDC->SetTextColor(RGB(50,200,20));
			else
				pDC->SetTextColor(RGB(200,20,20));
		}
		if(id == IDC_STATIC_TM_DLG_ALGO_DESC || id == IDC_STATIC_PARAM_DESC_PANEL)
		{
			pDC->SetTextColor(RGB(50,50,250));
		}
		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CModelMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_TM_DLG)
	{
		SetDlgItemTextW(IDC_STATIC_MODEL_DLG_MSG_PANEL, _T(""));
		CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_MODEL_DLG_MSG_PANEL);
		pPanel->ShowWindow(SW_HIDE);
		KillTimer(TIMER_ID_TM_DLG);
	}
	if(nIDEvent == TIMER_ID_TM_EDIT_CTRL)
	{
		SetDlgItemTextW(IDC_STATIC_PARAM_DESC_PANEL, _T(""));
		CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_PARAM_DESC_PANEL);
		pPanel->ShowWindow(SW_HIDE);
		KillTimer(TIMER_ID_TM_EDIT_CTRL);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CModelMgrDlg::OnNMClickListAlgoCandidate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int index = pNMItemActivate->iItem;
	if(-1 != index)
	{
		m_crtIndex_L = index;
		long funcId = m_mapIndexFuncId_L[index];
		if(funcId != -1)
		{
			DisplayFuncHelpString(m_mapFuncIdHelpStr[funcId]);
		}
		DeleteAllParamCtrls();
		m_bClickTreeItem = false;
		CreateParamCtrl(funcId, false);
		EnableUiCtrl(IDC_COMBO_JUDGE_RULE,false);
		EnableUiCtrl(IDC_EDIT_JUDGE_RULE,false);
		EnableUiCtrl(IDC_EDIT_ITEM_NAME, false);
		EnableUiCtrl(IDC_STATIC_APPLY, false, false);
	}
	*pResult = 0;
}

void CModelMgrDlg::OnEnSetfocusEditParameter()
{
	size_t sz = m_vEditCtrl.size();
	if(sz <= 0)
	{
		return;
	}
	SetDlgItemTextW(IDC_STATIC_PARAM_DESC_PANEL, _T(""));
	CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_PARAM_DESC_PANEL);
	pPanel->ShowWindow(SW_HIDE);
	CRect rcPanel;
	pPanel->GetWindowRect(rcPanel);
	InvalidateRect(rcPanel);

	CWnd* pEdit = CWnd::GetFocus();
	if(m_dictParamHelpStr.find(pEdit) != m_dictParamHelpStr.end())
	{
		wstring wstrHelp = m_dictParamHelpStr[pEdit];
		std::vector<wstring> vItem = utils::SplitString(wstrHelp, L";");
		size_t sz = vItem.size();
		if(sz >= 5)
		{
			CString sHelp;
			int nStrID = boost::lexical_cast<int>(vItem[0]);
			sHelp.LoadString(nStrID);
			int nIndex = sHelp.Find(_T("$"));
			if(nIndex != -1)
			{
				sHelp = sHelp.Right(sHelp.GetLength() - nIndex - 1);
			}
			sHelp.Append(_T(" -- ["));
			sHelp.Append(vItem[2].c_str());
			sHelp.Append(_T(","));
			sHelp.Append(vItem[3].c_str());
			sHelp.Append(_T("]"));
			SetDlgItemText(IDC_STATIC_PARAM_DESC_PANEL, sHelp);
		}
	}
	pPanel->ShowWindow(SW_SHOW);
	KillTimer(TIMER_ID_TM_EDIT_CTRL);
	SetTimer(TIMER_ID_TM_EDIT_CTRL, 30000, NULL);
}

void CModelMgrDlg::ResetJudgeRuleUiCtrl(bool bEnable)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
	pCombo->SetCurSel(-1);
	pCombo->EnableWindow(bEnable);

	CWnd* pWnd = GetDlgItem(IDC_EDIT_JUDGE_RULE);
	pWnd->SetWindowText(_T(""));
	pWnd->EnableWindow(bEnable);
}

void CModelMgrDlg::SelectJudgeRule(wstring& strComp)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
	int index = pCombo->FindStringExact(-1, CW2T(strComp.c_str()));
	if(index != CB_ERR)
	{
		pCombo->SetCurSel(index);
	}
}

void CModelMgrDlg::RestoreParamValue(FI& fi)
{
	int szParam  = (int)fi.m_vParam.size();
	int szEdit = (int)m_vEditCtrl.size();
	if(szParam != 0 && szParam == szEdit + 1) // Restore original value
	{
		if(fi.m_bTestFunc && !fi.m_vParam.empty())
		{
			std::vector<wstring> vElement = utils::SplitString(fi.m_vParam[0], L"$");
			if(vElement.size() == 2)
			{
				SetDlgItemTextW(IDC_EDIT_JUDGE_RULE, CW2T(vElement[1].c_str()));
				SelectJudgeRule(vElement[0]);
			}
		}
		SetDlgItemTextW(IDC_EDIT_ITEM_NAME, fi.m_strNickname.c_str());

		for(int i = 1; i < szParam; i++)
		{
			wstring strParam = fi.m_vParam[i];
			CWnd* pEdit = m_vEditCtrl[i-1];
			pEdit->SetWindowText(CW2T(strParam.c_str()));
		}
	}
	if(szParam == 0)  // Restore parameters' default value.
	{
		if(fi.m_bTestFunc) //Set default judgement rule
		{
			wstring helpStr = m_mapFuncIdHelpStr[fi.m_funcId];
			std::vector<wstring> vStr = utils::SplitString(helpStr, L"$");
			if(vStr.size() >= 2)
			{
				wstring wstrRule = vStr[1];
				std::vector<wstring> vItem = utils::SplitString(wstrRule, L";");
				if(!vItem.empty())
				{
					wstring wstrRule = vItem[vItem.size() - 1];
					std::vector<wstring> vRule = utils::SplitString(wstrRule, L"#");
					if(vRule.size() == 2)
					{
						SelectJudgeRule(vRule[0]);
						SetDlgItemTextW(IDC_EDIT_JUDGE_RULE, CW2T(vRule[1].c_str()));
					}
				}
			}
		}
		for(int i = 0; i < szEdit; i++)
		{
			CEdit* pEdit = (CEdit*)m_vEditCtrl[i];
			if(m_dictParamHelpStr.find(pEdit) != m_dictParamHelpStr.end())
			{
				wstring wstrParamHelp = m_dictParamHelpStr[pEdit];
				std::vector<wstring> vItem = utils::SplitString(wstrParamHelp, L";");
				if(vItem.size() == 6)
				{
					pEdit->SetWindowText(CW2T(vItem[5].c_str()));
				} 
			}
		}
	}
}

void CModelMgrDlg::ResetAllUiWidget()
{
	m_setAlgoName.clear();
	m_crtSelectedItem = nullptr;
	m_crtIndex_L = -1;
	m_crtIndex_R = -1;
	SetDlgItemTextW(IDC_EDIT_SAMPLE_COUNT, _T(""));
	m_treeTaskModel.DeleteAllItems();

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
	pCombo->SetCurSel(-1);
	pCombo->EnableWindow(FALSE);
	SetDlgItemTextW(IDC_EDIT_JUDGE_RULE, _T(""));
	EnableUiCtrl(IDC_EDIT_JUDGE_RULE, false);
	SetDlgItemTextW(IDC_EDIT_ITEM_NAME, _T(""));
	EnableUiCtrl(IDC_EDIT_ITEM_NAME, false);
	SetDlgItemTextW(IDC_STATIC_TM_DLG_ALGO_DESC, _T(""));
	SetDlgItemTextW(IDC_STATIC_PARAM_DESC_PANEL, _T(""));
	DeleteAllParamCtrls();
	UpdateData(FALSE);
	RedrawWindow();
}

void CModelMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_MODEL_DLG_MSG_PANEL);
	pPanel->ShowWindow(SW_HIDE);
	SetDlgItemTextW(IDC_STATIC_MODEL_DLG_MSG_PANEL, _T(""));
	pPanel->ShowWindow(SW_SHOW);
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_MODEL_DLG_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_TM_DLG, 5000, NULL);
}

BOOL CModelMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

bool CModelMgrDlg::SelectFiles(CString& sNewFile)
{
	CString strFilter = _T("Model Files (*.shm)|*.shm|TUP File (*.tup)|*.tup|TIFF File (*.tiff)|*.tiff|PNG File (*.png)|*.png|All Files (*.*)|*.*||");
	CFileDialog dlgFile(TRUE, _T("shm"), _T("*.shm"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT, strFilter, this);
	const unsigned long numberOfFileName = 256;
	const unsigned long fileNameMaxLen = 128;
	const unsigned long bufferSz = (numberOfFileName * fileNameMaxLen) + 1;
	std::vector<TCHAR> fileNameBuffer(bufferSz, 0);
	dlgFile.m_ofn.lpstrFile = &fileNameBuffer[0];
	dlgFile.m_ofn.nMaxFile = bufferSz;

	CString strFilePath = L"";
	if(dlgFile.DoModal() == IDOK)
	{
		POSITION pos = dlgFile.GetStartPosition();
		CString path;
		USES_CONVERSION;
		while(pos)
		{
			path = dlgFile.GetNextPathName(pos);
			strFilePath.Append(path);
			strFilePath.Append(_T(";"));
		}
		if(strFilePath.GetLength() > 0)
			strFilePath.TrimRight(_T(";"));

		sNewFile = strFilePath;
		return true;
	}
	return false;
}

void CModelMgrDlg::EnterFilePath(int cCtrlID)
{
	UpdateData(TRUE);
	CString strValue;
	GetDlgItemTextW(cCtrlID,strValue);
	CString newFiles(_T(""));
	if(SelectFiles(newFiles))
	{
		if(strValue.GetLength() > 0)
			strValue.Append(_T(";"));
		strValue.Append(newFiles);
		SetDlgItemTextW(cCtrlID, strValue);
	}
	UpdateData(FALSE);
}

void CModelMgrDlg::EnableUiCtrl(int nCtrlID, bool bEnabled, bool bClearTxt)
{
	CWnd* pCtrl = GetDlgItem(nCtrlID);
	if(pCtrl != nullptr)
	{
		if(bClearTxt)
		{
			pCtrl->SetWindowText(_T(""));
		}
		pCtrl->EnableWindow(bEnabled);
	}
	UpdateData(FALSE);
}

void CModelMgrDlg::DisplayFuncHelpString(wstring& strHelp)
{
	CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_TM_DLG_ALGO_DESC);
	pPanel->ShowWindow(SW_HIDE);
	CString helpStr;
	if(strHelp.empty())
	{
		helpStr = _T("");
	}
	else
	{
		std::vector<wstring> vStr = utils::SplitString(strHelp, L"$");
		if(!vStr.empty())
		{
			long strID = boost::lexical_cast<long>(vStr[0]);
			CString str;
			str.LoadString(strID);
			int t = str.Find(_T("$"));
			helpStr = str.Right(str.GetLength() - t - 1);
		}
	}
	SetDlgItemTextW(IDC_STATIC_TM_DLG_ALGO_DESC, helpStr);
	pPanel->ShowWindow(SW_SHOW);
}

void CModelMgrDlg::DeleteAllParamCtrls()
{
	if(!m_vStaticCtrl.empty())
	{
		std::for_each(m_vStaticCtrl.begin(), m_vStaticCtrl.end(), [&](CWnd* pCtrl)
		{
			delete pCtrl;
			pCtrl = nullptr;
		});
		m_vStaticCtrl.clear();
	}
	if(!m_vEditCtrl.empty())
	{
		std::for_each(m_vEditCtrl.begin(), m_vEditCtrl.end(), [&](CWnd* pCtrl)
		{
			delete pCtrl;
			pCtrl = nullptr;
		});
		m_vEditCtrl.clear();
	}
	m_dictParamHelpStr.clear();
}

bool CModelMgrDlg::DetermingTestingFuncFlag(long funcId)
{
	if(m_mapFuncIdHelpStr.find(funcId) != m_mapFuncIdHelpStr.end())
	{
		wstring helpStr = m_mapFuncIdHelpStr[funcId];
		if(helpStr.find(L";R;") != wstring::npos)
		{
			return true;
		}
	}
	return false;
}

void CModelMgrDlg::CreateParamCtrl(long funcId, bool bEnabled)
{
	bool bTestFunc = DetermingTestingFuncFlag(funcId);

	m_setFilePathCtrl.clear();
	if(m_mapFuncIdHelpStr.find(funcId) != m_mapFuncIdHelpStr.end())
	{
		CString strItemName;
		CWnd* pWnd = GetDlgItem(IDC_STATIC_ITEM_NAME);
		pWnd->ShowWindow(SW_HIDE);
		if(bTestFunc)
		{
			strItemName.LoadString(IDS_STRING_ITEM_NAME);
		}
		else
		{
			strItemName.LoadString(IDS_STRING_REGION_NAME);
		}
		pWnd->SetWindowText(strItemName);
		pWnd->ShowWindow(SW_SHOW);
		CRect rcTestName;
		pWnd->GetWindowRect(rcTestName);
		ScreenToClient(&rcTestName);


		CRect rcJudgeValue;
		pWnd = GetDlgItem(IDC_EDIT_JUDGE_RULE);
		pWnd->GetWindowRect(rcJudgeValue);
		ScreenToClient(&rcJudgeValue);

		wstring helpStr = m_mapFuncIdHelpStr[funcId];
		std::vector<wstring> vStr = utils::SplitString(helpStr, L"$");
		CRect rcName,rcEdit;

		rcName.left  = rcTestName.left;
		rcName.bottom = rcTestName.bottom;
		rcName.right = rcName.left + 128;
		rcEdit.left  = rcJudgeValue.left;
		rcEdit.right = rcJudgeValue.right;
		rcEdit.top  = rcName.top;
		rcEdit.bottom = rcName.bottom;

		rcName.bottom += 14;
		if(vStr.size() > 1)
		{
			int i = ((bTestFunc == true) ? 2 : 1);
			int nPathIndex = 0;
			int nIdFlag    = 0;
			m_dictID_WndPtr.clear();
			for(; i < vStr.size(); i++)
			{
				CStatic* pStatic = new CStatic;
				rcName.top    = rcName.bottom + 0;
				rcName.bottom = rcName.top + 30;
				std::vector<wstring> vElement = utils::SplitString(vStr[i], L";");
				if(vElement.empty())
					continue;
				long strId = boost::lexical_cast<long>(vElement[0]);
				CString strName;
				strName.LoadString(strId);
				int idx = strName.Find(_T("$"));
				strName = strName.Left(idx);
				pStatic->Create(strName, WS_CHILD|WS_VISIBLE, rcName, this, NULL);
				pStatic->SetFont(&m_smallFont);
				m_vStaticCtrl.push_back(pStatic);

				CEdit* pEdit = new CEdit;
				rcEdit.top    = rcName.top;
				rcEdit.bottom = rcEdit.top + 22;
				pEdit->Create(WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcEdit, this, IDC_EDIT_CTRL);
				
				pEdit->SetFont(&m_smallFont);
				pEdit->EnableWindow(nIdFlag != 0 && bEnabled);
				if(vElement[1].find(L"SP") != wstring::npos)
				{
					pEdit->EnableWindow(FALSE);
					m_setFilePathCtrl.insert(pEdit);
				}
				m_vEditCtrl.push_back(pEdit);
				m_dictParamHelpStr[pEdit] = vStr[i];
				nIdFlag++;
			}
		}
		CStatic* pGp = (CStatic*)GetDlgItem(IDC_STATIC_TEST_ITEM_PARAM);
		CRect rcGp;
		pGp->GetWindowRect(rcGp);
		ScreenToClient(&rcGp);

		CRect rcApply;
		m_btnApply.GetWindowRect(rcApply);
		ScreenToClient(&rcApply);
		int nHeight = rcApply.Height();
		rcApply.top    = rcEdit.bottom + 3;
		rcApply.bottom = rcApply.top + nHeight;
		m_btnApply.MoveWindow(rcApply);
		m_btnApply.Invalidate();

		CRect rcPanel;
		rcPanel.top    = rcApply.bottom + 3;
		rcPanel.left   = rcGp.left + 2;
		rcPanel.right  = rcGp.right - 2;
		rcPanel.bottom = rcGp.bottom;
		m_paramHelpPanel.MoveWindow(rcPanel);
		m_paramHelpPanel.Invalidate();

		InvalidateRect(rcGp);
	}
}

void CModelMgrDlg::SwitchToProduct(int nProductIndex)
{
	if(nProductIndex >= (int)m_vProductId.size())
		return;
	int tmpID = m_vProductId[nProductIndex];
	if(m_nCrtProdudctId == tmpID)
		return;
	BOOL bVisible = IsWindowVisible();
	m_nCrtProdudctId = tmpID;

	m_crtSelectedItem = nullptr;
	m_crtIndex_L = -1;
	m_crtIndex_R = -1;

	m_vSelecteeAlgo.clear();
	m_dictTI_FI.clear();
	m_dictParamHelpStr.clear();
	FillUpUiWidget();

	m_bSlModel = FALSE;
	SetDlgItemTextW(IDC_EDIT_SAMPLE_COUNT, _T(""));

	SetDlgItemTextW(IDC_STATIC_TM_DLG_ALGO_DESC, _T(""));
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
	pCombo->SetCurSel(-1);
	SetDlgItemTextW(IDC_EDIT_JUDGE_RULE, _T(""));
	SetDlgItemTextW(IDC_EDIT_ITEM_NAME, _T(""));
	DeleteAllParamCtrls();

	m_btnCreate.EnableWindow(TRUE);
	m_btnDelete.EnableWindow(FALSE);
	m_btnModify.EnableWindow(FALSE);
	m_btnCopy.EnableWindow(FALSE);
	m_candidateList.DeleteAllItems();
	m_treeTaskModel.DeleteAllItems();
	FillUpCandidateList();
	if(bVisible == TRUE)
	{
		UpdateData(FALSE);
	}
}

void CModelMgrDlg::OnTvnSelchangedTreeTaskModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	UpdateData(TRUE);

	SetDlgItemTextW(IDC_STATIC_PARAM_DESC_PANEL, _T(""));
	RedrawWindow();
	KillTimer(TIMER_ID_TM_EDIT_CTRL);

	m_bClickTreeItem = true;
	m_crtSelectedItem = m_treeTaskModel.GetSelectedItem();
	if(m_dictTI_FI.find(m_crtSelectedItem) != m_dictTI_FI.end())
	{
		FI fi = m_dictTI_FI[m_crtSelectedItem];
		m_crtIndex_R = fi.m_nIndex;
		if(fi.m_funcId != -1)
		{
			DisplayFuncHelpString(m_mapFuncIdHelpStr[fi.m_funcId]);
		}
		ResetJudgeRuleUiCtrl(fi.m_funcId != -1 && fi.m_bTestFunc);
		EnableUiCtrl(IDC_EDIT_ITEM_NAME, fi.m_funcId != -1);
		DeleteAllParamCtrls();
		CreateParamCtrl(fi.m_funcId);
		RestoreParamValue(fi);
		SetRegionIDForNewFunc(fi.m_nRgnId);
		EnableUiCtrl(IDC_STATIC_APPLY, true, false);
	}
	else
	{
		DeleteAllParamCtrls();
	}
	*pResult = 0;
}

void CModelMgrDlg::OnNMClickTreeTaskModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	UpdateData(TRUE);

	CPoint point;
	GetCursorPos(&point);
	m_treeTaskModel.ScreenToClient(&point);

	UINT uFlags;
	HTREEITEM item  = m_treeTaskModel.GetSelectedItem();;
	m_crtSelectedItem =m_treeTaskModel.HitTest(point,&uFlags);

	if (m_crtSelectedItem != item)
	{
		return;
	}

	SetDlgItemTextW(IDC_STATIC_PARAM_DESC_PANEL, _T(""));
	RedrawWindow();

	KillTimer(TIMER_ID_TM_EDIT_CTRL);

	m_bClickTreeItem = true;

	if(m_dictTI_FI.find(m_crtSelectedItem) != m_dictTI_FI.end())
	{
		FI fi = m_dictTI_FI[m_crtSelectedItem];
		m_crtIndex_R = fi.m_nIndex;
		if(fi.m_funcId != -1)
		{
			DisplayFuncHelpString(m_mapFuncIdHelpStr[fi.m_funcId]);
		}
		ResetJudgeRuleUiCtrl(fi.m_funcId != -1 && fi.m_bTestFunc);
		EnableUiCtrl(IDC_EDIT_ITEM_NAME, fi.m_funcId != -1);
		DeleteAllParamCtrls();
		CreateParamCtrl(fi.m_funcId);
		RestoreParamValue(fi);
		SetRegionIDForNewFunc(fi.m_nRgnId);
		EnableUiCtrl(IDC_STATIC_APPLY, true, false);
	}
	else
	{
		DeleteAllParamCtrls();
	}
}
