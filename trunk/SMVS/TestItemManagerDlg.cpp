#include "stdafx.h"
#include "SMVS.h"
#include "TestItemManagerDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CTestItemManagerDlg, CDialogEx)

CTestItemManagerDlg::CTestItemManagerDlg(const int& nProductID, eTASK_OPERATION toType, int nRgnID, set<wstring> setNickname,FUNC_INFO& retFI, CWnd* pParent /*=NULL*/)
	: m_nProductID(nProductID),
	  m_taskOperation(toType),
	  m_nRgnID(nRgnID),
	  m_nFuncID(-1),
	  m_retFI(retFI),
	  m_bOpRslt(true),
	  CDialogEx(CTestItemManagerDlg::IDD, pParent)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_listFont.CreateFont(18 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
	m_setNickname = setNickname;
}

CTestItemManagerDlg::~CTestItemManagerDlg()
{
}

BOOL CTestItemManagerDlg::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	HRESULT hr = ::CoInitialize(NULL);
	if(FAILED(hr))
	{
		int nStop = 90;
	}
#ifdef CROWN_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(CrownCapAlgo));
#endif

#ifdef EOE_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(EoeAlgo));
#endif

#ifdef CAN_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(CanAlgo));
#endif

#ifdef CAN_EOE_PRODUCT
		hr = m_ptrAlgoObjEoe.CoCreateInstance(__uuidof(EoeAlgo));
		hr = m_ptrAlgoObjCan.CoCreateInstance(__uuidof(CanAlgo));
#endif

#ifdef PET_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(PetAlgo));
#endif

#ifdef PCC_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(PccAlgo));
#endif

#ifdef CAP_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(CapacitorAlgo));
#endif

#ifdef MAT_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(RubberMatAlgo));
#endif

#ifdef NEEDLE_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(NeedleAlgo));
#endif

#ifdef PLUG_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(PlugAlgo));
#endif

#ifdef HDPE_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(HdpeAlgo));
#endif

#ifdef JELLY_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(JellyAlgo));
#endif

#ifdef CAN_OUTSIDE_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(CanOutsideAlgo));
#endif

#ifdef NAIL_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(NailAlgo));
#endif

#ifdef BATTERY_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(BatteryShellAlgo));
#endif

#ifdef BOTTLE_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(BottleCapAlgo));
#endif

#ifdef AL_PLASTIC_PRODUCT
		hr = m_pImageCheckingIt.CoCreateInstance(__uuidof(AlPlasticCapAlgo));
#endif
	if(FAILED(hr))
	{
		return FALSE;
	}

	InitUiWidget(IDC_STATIC_TM_DLG_ALGO_DESC);
	InitUiWidget(IDC_STATIC_TEST_ITEM_PARAM,IDS_STRING_ALGO_PARAM);
	InitUiWidget(IDC_STATIC_JUDGE_RULE, IDS_STRING_JUDGE_RULE);
	InitUiWidget(IDC_COMBO_JUDGE_RULE, NULL, (m_taskOperation == TO_ADD_ITEM));
	InitUiWidget(IDC_STATIC_MSG_PANEL);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
	for(auto it = utils::SYMBOL_TYPE.begin(); it != utils::SYMBOL_TYPE.end();it++)
	{
		wstring str = it->m_strComp;
		pCombo->AddString(CW2T(str.c_str()));
	}
	pCombo->SetCurSel(-1);

	InitUiWidget(IDC_EDIT_JUDGE_RULE, NULL,(m_taskOperation == TO_ADD_ITEM));

	if(m_taskOperation == TO_ADD_ITEM)
	{
		InitUiWidget(IDC_STATIC_ITEM_NAME,IDS_STRING_ITEM_NAME);
	}
	if(m_taskOperation == TO_ADD_REGION)
	{
		InitUiWidget(IDC_STATIC_ITEM_NAME,IDS_STRING_REGION_NAME);
	}
	InitUiWidget(IDC_EDIT_ITEM_NAME);
	InitUiWidget(IDC_STATIC_PARAM_DESC_PANEL);

	FillUpCandidateList();
	return TRUE;
}
typedef pair<long, wstring> PAIR;

struct CmpByValue {
	bool operator()(const PAIR& lhs, const PAIR& rhs) {
		return lhs.second < rhs.second;
	}
};

void CTestItemManagerDlg::FillUpCandidateList()
{
	DWORD dwStyleEx = m_candidateList.GetExtendedStyle();
	dwStyleEx |= (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES); 
	m_candidateList.SetExtendedStyle(dwStyleEx);

	m_candidateList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strIndex(MAKEINTRESOURCE(IDS_STRING_INDEX));
	CString strAlgoIt(MAKEINTRESOURCE(IDS_STRING_ALGO_ITERFACE));
	m_candidateList.InsertColumn(0, strIndex, LVCFMT_CENTER, 50);
	m_candidateList.InsertColumn(1, strAlgoIt, LVCFMT_LEFT, 280);
	m_candidateList.InsertColumn(2, strAlgoIt, LVCFMT_LEFT, 600);
	m_candidateList.SetFont(&m_listFont);
	HRESULT hr = S_FALSE;

#ifdef CAN_EOE_PRODUCT
	if(m_nProductID == EOE)
	{
		hr = utils::GetIDispatchMethods(m_ptrAlgoObjEoe, m_algoCandidate, m_mapFuncIdHelpStr);
	}
	if(m_nProductID == CAN)
	{
		hr = utils::GetIDispatchMethods(m_ptrAlgoObjCan, m_algoCandidate, m_mapFuncIdHelpStr);
	}
#else
	hr = utils::GetIDispatchMethods(m_pImageCheckingIt, m_algoCandidate, m_mapFuncIdHelpStr);
#endif
	FilterAlgoItem(m_taskOperation);

	if(SUCCEEDED(hr))
	{
		CString strIdx;
		int index = 0;
		std::wstring strHelp;

		vector<PAIR> name_score_vec(m_algoCandidate.begin(), m_algoCandidate.end());
		sort(name_score_vec.begin(), name_score_vec.end(), CmpByValue());

//		std::for_each(m_algoCandidate.begin(), m_algoCandidate.end(), [&](std::pair<long, wstring> p)
		for(auto p = name_score_vec.begin(); p!= name_score_vec.end();p++)
		{
			strIdx.Format(_T("%d"), index + 1);
			m_candidateList.InsertItem(index,strIdx);
			m_candidateList.SetItemText(index, 1, W2CT(p->second.c_str()));
			CString helpStr;
			int nfuncID = p->first;
			if(m_mapFuncIdHelpStr.find(nfuncID) != m_mapFuncIdHelpStr.end())
			{	
				wstring wstrHelp = m_mapFuncIdHelpStr[nfuncID];
				std::vector<wstring> vStr = utils::SplitString(wstrHelp, L"$");
				if(!vStr.empty())
				{
					long strID = boost::lexical_cast<long>(vStr[0]);
					CString str;
					str.LoadString(strID);
					int t = str.Find(_T("$"));
					helpStr = str.Right(str.GetLength() - t - 1);
					m_candidateList.SetItemText(index, 2, helpStr);
				}
			}

			m_mapIndexFuncId[index] = p->first;
			index++;
		}
	}
}

void CTestItemManagerDlg::FilterAlgoItem(eTASK_OPERATION toType)
{
	if(m_algoCandidate.empty())
	{
		return;
	}

	map<long, wstring>  dictFuncId_Name = m_algoCandidate;
	m_algoCandidate.clear();

	for(auto it = dictFuncId_Name.begin(); it != dictFuncId_Name.end(); it++)
	{
		long funcID = it->first;
		wstring wstrHelp = m_mapFuncIdHelpStr[funcID];
		if(toType == TO_ADD_REGION)
		{
			if(wstrHelp.find(L";R;") == wstring::npos)
			{
				m_algoCandidate[funcID] = it->second;
			}
		}
		if(toType == TO_ADD_ITEM)
		{
			if(wstrHelp.find(L";R;") != wstring::npos)
			{
				m_algoCandidate[funcID] = it->second;
			}
		}
	}
}

BOOL CTestItemManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTestItemManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALGO_CANDIDATE, m_candidateList);
}

void CTestItemManagerDlg::InitUiWidget(int nCtrlID, int strID, bool bEnabled)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	if(pWnd != nullptr)
	{
		pWnd->SetFont(&m_textFont);
		if(strID != NULL)
		{
			CString str;
			str.LoadString(strID);
			pWnd->SetWindowText(str);
		}
		pWnd->EnableWindow(bEnabled);
		pWnd->Invalidate();
	}
}

BEGIN_MESSAGE_MAP(CTestItemManagerDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_APPLY, &CTestItemManagerDlg::OnStnClickedStaticApply)
	ON_STN_CLICKED(IDC_STATIC_CLOSE, &CTestItemManagerDlg::OnStnClickedStaticClose)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALGO_CANDIDATE, &CTestItemManagerDlg::OnNMClickListAlgoCandidate)
	ON_EN_SETFOCUS(IDC_EDIT_CTRL_TIM,&CTestItemManagerDlg::OnEnSetfocusEditParameter)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void CTestItemManagerDlg::DeleteAllParamCtrls()
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

	SetDlgItemTextW(IDC_STATIC_PARAM_DESC_PANEL, _T(""));
	CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_PARAM_DESC_PANEL);
	pPanel->ShowWindow(SW_HIDE);
}

void CTestItemManagerDlg::CreateParamCtrl(long funcId)
{
	bool bTestFunc = (m_taskOperation == TO_ADD_ITEM);
	if(m_mapFuncIdHelpStr.find(funcId) != m_mapFuncIdHelpStr.end())
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_ITEM_NAME);
		CRect rcTestName;
		pWnd->GetWindowRect(rcTestName);
		ScreenToClient(&rcTestName);

		CRect rcJudgeValue;
		pWnd = GetDlgItem(IDC_EDIT_JUDGE_RULE);
		pWnd->GetWindowRect(rcJudgeValue);
		ScreenToClient(&rcJudgeValue);

		wstring helpStr = m_mapFuncIdHelpStr[funcId];
		std::vector<wstring> vStr = utils::SplitString(helpStr, L"$");
		if(vStr.size() > 1)
		{
			int i = ((bTestFunc == true) ? 2 : 1);
			CRect rcName,rcEdit;
			rcName.left  = rcTestName.left;
			rcName.bottom = rcTestName.bottom;
			rcName.right = rcName.left + 128;
			rcEdit.left  = rcJudgeValue.left;
			rcEdit.right = rcJudgeValue.right;

			int nPathIndex = 0;
			m_dictID_WndPtr.clear();
			for(; i < vStr.size(); i++)
			{
				CStatic* pStatic = new CStatic;
				rcName.top    = rcName.bottom + 8;
				rcName.bottom = rcName.top + 22;
				std::vector<wstring> vElement = utils::SplitString(vStr[i], L";");
				if(vElement.empty())
					continue;
				long strId = boost::lexical_cast<long>(vElement[0]);
				CString strName;
				strName.LoadString(strId);
				int idx = strName.Find(_T("$"));
				strName = strName.Left(idx);
				pStatic->Create(strName, WS_CHILD|WS_VISIBLE, rcName, this, NULL);
				pStatic->SetFont(&m_listFont);
				m_vStaticCtrl.push_back(pStatic);

				CEdit* pEdit = new CEdit;
				rcEdit.top    = rcName.top;
				rcEdit.bottom = rcEdit.top + 22;
				pEdit->Create(WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcEdit, this, IDC_EDIT_CTRL_TIM);
				
				pEdit->SetFont(&m_listFont);
				if(vElement[1].find(L"SP") != wstring::npos)
				{
					pEdit->EnableWindow(FALSE);
				}
				m_vEditCtrl.push_back(pEdit);
				m_dictParamHelpStr[pEdit] = vStr[i];
			}
		}
	}
}

void CTestItemManagerDlg::DisplayFuncHelpString(wstring& strHelp)
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

HBRUSH CTestItemManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_TM_DLG_ALGO_DESC ||
	   id == IDC_STATIC_PARAM_DESC_PANEL ||
	   id == IDC_STATIC_MSG_PANEL)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		if(id == IDC_STATIC_MSG_PANEL)
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

void CTestItemManagerDlg::OnEnSetfocusEditParameter()
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
}

void CTestItemManagerDlg::OnStnClickedStaticApply()
{
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MSG_PANEL);
	pWnd->SetWindowText(_T(""));
	pWnd->ShowWindow(SW_HIDE);
	Sleep(2);
	pWnd->ShowWindow(SW_SHOW);

	IT_PARAM retItParam;
	if(GetCrtAlgoParameters(retItParam))
	{
		CString sMsg;
		if(utils::CheckSpecialCharacter(retItParam.m_itNickname))
		{
			sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
			DisplayPromptMessage(sMsg,false);
			return;
		}
		if(m_setNickname.find(retItParam.m_itNickname) != m_setNickname.end())
		{
			sMsg.LoadString(IDS_STRING_WARNING_NICKNAME_EXISTED);
			DisplayPromptMessage(sMsg,false);
			return;
		}
		ProduceFuncInfoList(m_mapFuncIdHelpStr[m_nFuncID],retItParam,m_retFI);

		CString strLog;
		strLog.Format(_T("Testing item %s is added!"),retItParam.m_itNickname.c_str());
		gLogger.QueueUpLog(strLog.GetString(),true);
	}
	else
	{
		return;
	}

#ifdef CAN_EOE_PRODUCT
	if(m_nProductID == EOE)
	{
		m_ptrAlgoObjEoe = nullptr;
	}
	if(m_nProductID == CAN)
	{
		m_ptrAlgoObjCan = nullptr;
	}
#else
	m_pImageCheckingIt = nullptr;
#endif
	
    DeleteAllParamCtrls();
	::CoUninitialize();
	CDialogEx::OnOK();
}

void CTestItemManagerDlg::OnStnClickedStaticClose()
{

#ifdef CAN_EOE_PRODUCT
	if(m_nProductID == EOE)
	{
		m_ptrAlgoObjEoe = nullptr;
	}
	if(m_nProductID == CAN)
	{
		m_ptrAlgoObjCan = nullptr;
	}
#else
	m_pImageCheckingIt = nullptr;
#endif
    DeleteAllParamCtrls();
	::CoUninitialize();
	CDialogEx::OnCancel();
}

void CTestItemManagerDlg::OnNMClickListAlgoCandidate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int index = pNMItemActivate->iItem;
	if(-1 != index)
	{
		m_nFuncID = m_mapIndexFuncId[index];
		wstring wstrHelp = m_mapFuncIdHelpStr[m_nFuncID];
		DisplayFuncHelpString(wstrHelp);
		DeleteAllParamCtrls();
		CComboBox* pCb = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
		if(pCb != nullptr)
		{
			pCb->SetCurSel(-1);
		}
		SetDlgItemText(IDC_EDIT_JUDGE_RULE, _T(""));
		SetDlgItemText(IDC_EDIT_ITEM_NAME, _T(""));
		CreateParamCtrl(m_nFuncID);
		SetDefaultParamValue();
	}
	*pResult = 0;
}

void CTestItemManagerDlg::SetDefaultParamValue()
{
	int szEdit = (int)m_vEditCtrl.size();
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
		if(i == 0)
		{
			CString sRgnID;
			sRgnID.Format(_T("%d"), m_nRgnID);
			pEdit->SetWindowText(sRgnID);
			pEdit->EnableWindow(FALSE);
		}
	}
}

void CTestItemManagerDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_MSG_PANEL, _T(""));
	CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_MSG_PANEL);
	pPanel->ShowWindow(SW_SHOW);
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_TIM_DLG, 10000, NULL);
}

void CTestItemManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_TIM_DLG)
	{
		SetDlgItemTextW(IDC_STATIC_MSG_PANEL, _T(""));
		CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_MSG_PANEL);
		pPanel->ShowWindow(SW_HIDE);
		KillTimer(TIMER_ID_TIM_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}

bool CTestItemManagerDlg::GetCrtAlgoParameters(IT_PARAM& retItParam)
{
	if(m_vStaticCtrl.empty())
		return false;

	FI retFI;
	CString sMsg;
	retFI.m_bTestFunc = (m_taskOperation == TO_ADD_ITEM);
	if(retFI.m_bTestFunc)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_JUDGE_RULE);
		int index = pCombo->GetCurSel();
		if(index != -1)
		{
			CString strRet;
			pCombo->GetLBText(index,strRet);

			CString strValue;
			GetDlgItemTextW(IDC_EDIT_JUDGE_RULE, strValue);
			if(strValue.GetLength()  <= 0)
			{
				retFI.m_vParam.clear();
				sMsg.LoadString(IDS_STRING_PARAM_VALUE_CANNOT_BE_EMPTY);
				DisplayPromptMessage(sMsg, false);
				return false;
			}
			if(index <= 5)
			{
				bool isNumeric = utils::IsNumericString(strValue);
				if(!isNumeric)
				{
					retFI.m_vParam.clear();
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
			CString strRule;
			strRule.Append(strRet);
			strRule.Append(_T("$"));
			strRule.Append(strValue);
			retFI.m_vParam.push_back(strRule.GetString());
		}
		else
		{
			sMsg.LoadString(IDS_STRING_MSG_PLZ_SET_JUDGEMENT_RULE);
			DisplayPromptMessage(sMsg, false);
			return false;
		}
	}
	else
	{
		retFI.m_vParam.push_back(L"N/A");
	}
	CString sNickname;
	GetDlgItemTextW(IDC_EDIT_ITEM_NAME, sNickname);
	if(sNickname.GetLength() <= 0)
	{
		sMsg.LoadString(IDS_STRING_WARNING_ALGO_NICKNAME_EMPTY);
		DisplayPromptMessage(sMsg, false);
		return false;
	}
	wstring wstrNickname = CT2W(sNickname);
	if(utils::CheckSpecialCharacter(wstrNickname))
	{
		sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
		DisplayPromptMessage(sMsg, false);
		return false;
	}
	retFI.m_strNickname = wstrNickname;

	int n = 0;
	bool bRet = true;
	for(int i = 0; i < m_vEditCtrl.size(); i++)
	{
		CWnd* pCtrl = m_vEditCtrl[i];
		CString str;
		pCtrl->GetWindowText(str);
		if(str.GetLength()  <= 0)
		{
			retFI.m_vParam.push_back(L" ");
		}
		else
		{
			wstring wStr = str.GetString();
			retFI.m_vParam.push_back(wStr);
		}
	}
	retItParam.m_itName     = m_algoCandidate[m_nFuncID];
	retItParam.m_itNickname = retFI.m_strNickname;
	retItParam.m_vParam     = retFI.m_vParam;

	return true;
}

bool CTestItemManagerDlg::ProduceFuncInfoList(wstring strHelp,IT_PARAM& itParams, FUNC_INFO& funcInfo)
{
	wstring funcName = itParams.m_itName;
	CComBSTR bstrFuncName(funcName.c_str());
	funcInfo.m_dispId      = m_nFuncID;
	funcInfo.m_strFuncName = itParams.m_itName;

	std::vector<wstring> vValue = itParams.m_vParam;
	int index = -1;
	std::vector<wstring> vDesc = utils::SplitString(strHelp, L"$");
	CString strTmp;
	if(vDesc.size() > 0)
	{
		for(size_t i = 0; i < vDesc.size();++i)
		{
			wstring str = vDesc[i];
			vector<wstring> vInfo = utils::SplitString(str,L";");
			utils::TrimString(vInfo);
			size_t sz = vInfo.size();
			if(sz >= 3 && vInfo[1] == L"R")
			{
				funcInfo.m_bTestFunc = true;
				funcInfo.m_threshold.m_type = utils::GetType(vInfo[2]);
				funcInfo.m_threshold.m_helpId = boost::lexical_cast<long>(vInfo[0]);
				if(!utils::ParseThresholdSpec(funcInfo.m_threshold, vValue[0]))
				{
					return false;
				}
				if(sz == 7) // Parse range restriction of the spec value
				{
					funcInfo.m_threshold.m_min     = boost::lexical_cast<float>(vInfo[3]);
					funcInfo.m_threshold.m_max     = boost::lexical_cast<float>(vInfo[4]);
					funcInfo.m_threshold.m_stepLen = boost::lexical_cast<float>(vInfo[5]);
				}
			}
			else
			{
				if(sz == 1) // function help string ID
				{
					funcInfo.m_strCustomizedName = itParams.m_itNickname;
					funcInfo.m_funcDescId = boost::lexical_cast<long>(vInfo[0].c_str());
					strTmp.LoadString(funcInfo.m_funcDescId);
					wstring strHelp = strTmp.GetString();
					std::vector<wstring> vStr = utils::SplitString(strHelp, L"$");
					utils::TrimString(vStr);
					size_t strSz = vStr.size();
					if(strSz == 1)
					{
						funcInfo.m_strHelp = vStr[0];
					}
					if(strSz == 2)
					{
						funcInfo.m_strOutputName = vStr[0];
						funcInfo.m_strHelp       = vStr[1];
					}
				}
				if(sz == 2 && vInfo[1] == L"B")
				{
					index = 1;
					INPUT_PARAM param;
					param.m_descID = boost::lexical_cast<long>(vInfo[0]);
					param.m_type   = utils::GetType(vInfo[1]);
					param.m_value  = utils::ConvertStr2Value(vValue[index]);
					funcInfo.m_vParam.push_back(param);
					index++;
				}
				if(sz == 2 && vInfo[1].find(L"SP") != wstring::npos)
				{
					INPUT_PARAM param;
					param.m_descID   = boost::lexical_cast<long>(vInfo[0]);
					param.m_type     = utils::GetType(vInfo[1]);
					param.m_wstrFile = vValue[index];
					param.m_nFileType = ((vInfo[1] == L"SPM")? 0 : 1);
					funcInfo.m_vParam.push_back(param);
					index++;
				}
				if(sz == 6)
				{
					INPUT_PARAM param;
					param.m_descID = boost::lexical_cast<long>(vInfo[0]);
					param.m_type = utils::GetType(vInfo[1]);
					param.m_value = utils::ConvertStr2Value(vValue[index]);
					param.m_min = boost::lexical_cast<float>(vInfo[2]);
					param.m_max = boost::lexical_cast<float>(vInfo[3]);
					param.m_step = boost::lexical_cast<float>(vInfo[4]);
					funcInfo.m_vParam.push_back(param);
					index++;
				}
			}
		}
	}
	return funcInfo.CreateComParamArray();
}

void CTestItemManagerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}
