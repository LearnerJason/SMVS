#include "stdafx.h"
#include "SMVS.h"
#include "CrownCapTaskMgrDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CCrownCapTaskMgrDlg, CDialogEx)

CCrownCapTaskMgrDlg::CCrownCapTaskMgrDlg(const vector<int>& vProduct,CWnd* pParent /*=NULL*/)
	: CDialogEx(CCrownCapTaskMgrDlg::IDD, pParent)
	 , m_vProductId(vProduct)
	 , m_nCrtProdudctId(vProduct[0])
	 , m_bImgInside(FALSE)
	 , m_bOpRslt(true)
	 , TIMER_ID_CROWN_CAP_TASK_DLG(100004)
	 , m_nTaskIndex(0)
	 , m_pDictChConfig(nullptr)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CCrownCapTaskMgrDlg::~CCrownCapTaskMgrDlg()
{
}

void CCrownCapTaskMgrDlg::SetCrtProductIndex(int nProductIndex)
{
	if(nProductIndex < (int)m_vProductId.size())
		m_nCrtProdudctId = m_vProductId[nProductIndex];
}

void CCrownCapTaskMgrDlg::PrepareTaskModelList()
{
}

void CCrownCapTaskMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_IMAGE_INSIDE, m_bImgInside);
	DDX_Control(pDX, IDC_COMBO_CROWN_CAP_TASK, m_comboTask);
	DDX_Control(pDX, IDC_BUTTON_CREATE_CROWN_CAP_TASK, m_btnCreateTask);
	DDX_Control(pDX, IDC_BUTTON_DELETE_CROWN_CAP_TASK, m_btnDeleteTask);
	DDX_Control(pDX, IDC_BUTTON_RETRAINNING_CROWN_CAP_TASK, m_btnRetraining);
}

void CCrownCapTaskMgrDlg::InitUiWidget(int nCtrlID, int strID)
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
		pWnd->Invalidate();
	}
}

void CCrownCapTaskMgrDlg::FillUpUiWidget()
{
	m_pDictChConfig = gConfigMgr.GetChannelConfigPtr();
	m_comboTask.AddString(_T(""));

	int sz = (int)m_pDictChConfig->size();
	if(sz == 0) return;

	std::vector<wstring> vTaskName;
	if(m_nCrtProdudctId == 1 && sz == 2) // Crown Cap
	{
		int chIndex = 1;
		CHANNEL_CONFIG chConfig = (*m_pDictChConfig)[chIndex];
		std::vector<TASK> vCh1AlgoGp = chConfig.m_vTask;
		for(size_t t = 0; t < vCh1AlgoGp.size(); t++)
		{
			TASK task = vCh1AlgoGp[t];
			if(!task.m_bSelfLearning)
			{
				wstring taskName = task.m_wstrTaskName;
				std::vector<wstring> vStr = utils::SplitString(taskName, L"_");
				if(!vStr.empty())
					m_comboTask.AddString(vStr[0].c_str());
			}
		}
		m_comboTask.SetCurSel(0);
	}
	UpdateData(FALSE);
}

BOOL CCrownCapTaskMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);
	
	InitUiWidget(IDC_STATIC_TASK_NAME,IDS_STRING_TASK_NAME);
	InitUiWidget(IDC_COMBO_CROWN_CAP_TASK);
	InitUiWidget(IDC_STATIC_IMAGE_INSIDE,IDS_STRING_IMAGE_INSIDE);
	InitUiWidget(IDC_BUTTON_CREATE_CROWN_CAP_TASK,IDS_STRING_CREATE_TASK);
	InitUiWidget(IDC_BUTTON_DELETE_CROWN_CAP_TASK,IDS_STRING_DELETE_TASK);
	InitUiWidget(IDC_BUTTON_RETRAINNING_CROWN_CAP_TASK,IDS_STRING_RE_TRAINNING);
	InitUiWidget(IDC_STATIC_TASK_DLG_MSG_PANEL);

	m_btnCreateTask.EnableWindow(TRUE);
	m_btnDeleteTask.EnableWindow(FALSE);
	m_btnRetraining.EnableWindow(FALSE);

	FillUpUiWidget();

	return TRUE;
}

BOOL CCrownCapTaskMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CCrownCapTaskMgrDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_CROWN_CAP_TASK, &CCrownCapTaskMgrDlg::OnBnClickedButtonCreateTask)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_CROWN_CAP_TASK, &CCrownCapTaskMgrDlg::OnBnClickedButtonDeleteTask)
	ON_BN_CLICKED(IDC_BUTTON_RETRAINNING_CROWN_CAP_TASK, &CCrownCapTaskMgrDlg::OnBnClickedButtonRetrainning)
	ON_CBN_SELCHANGE(IDC_COMBO_CROWN_CAP_TASK,      &CCrownCapTaskMgrDlg::OnCbnSelchangeComboTask)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CCrownCapTaskMgrDlg::OnCbnSelchangeComboTask()
{
	UpdateData(TRUE);
	m_bImgInside = FALSE;
	CWnd* pCheck = GetDlgItem(IDC_CHECK_IMAGE_INSIDE);
	m_nTaskIndex = m_comboTask.GetCurSel();
	if(m_nTaskIndex >= 1)
	{
		m_btnCreateTask.EnableWindow(FALSE);
		m_btnDeleteTask.EnableWindow(TRUE);
		m_btnRetraining.EnableWindow(FALSE);
		CString sTaskName;
		m_comboTask.GetLBText(m_nTaskIndex,sTaskName);
		wstring strTaskName = CT2W(sTaskName);
		strTaskName += L"_CH1";
		int chIndex = 1;
		CHANNEL_CONFIG chConfig = (*m_pDictChConfig)[chIndex];
		std::vector<TASK> vCh1AlgoGp = chConfig.m_vTask;
		std::for_each(vCh1AlgoGp.begin(), vCh1AlgoGp.end(), [&](TASK& task)
		{
			if(task.m_wstrTaskName == strTaskName && task.m_bImgInside)
				m_bImgInside = TRUE;
		});
	}
	else
	{
		m_btnCreateTask.EnableWindow(TRUE);
		m_btnDeleteTask.EnableWindow(FALSE);
		m_btnRetraining.EnableWindow(FALSE);
	}

	if(pCheck != nullptr)
		pCheck->EnableWindow(m_nTaskIndex <= 0);

	UpdateData(FALSE);
}

void CCrownCapTaskMgrDlg::OnBnClickedButtonCreateTask()
{
	UpdateData(TRUE);
	if(m_nTaskIndex != 0)
		return;
	CString sTaskName;
	GetDlgItemTextW(IDC_COMBO_CROWN_CAP_TASK, sTaskName);
	wstring strTaskName = CT2W(sTaskName);
	CString sMsg;
	if(strTaskName.empty())
	{
		sMsg.LoadString(IDS_STRING_TASK_NAME_CANNOT_BE_EMPTY);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	bool bBgnWithNum = utils::CheckSpecialCharacter(strTaskName);
	if(bBgnWithNum)
	{
		sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	if(strTaskName.find(L"_") != wstring::npos)
	{
		sMsg.LoadString(IDS_STRING_INVALID_TASK_NAME);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	if(gConfigMgr.IsTaskNameExisted(strTaskName))
	{
		sMsg.LoadString(IDS_STRING_WARNING_TASK_NAME_DUPLICATED);
		AfxMessageBox(sMsg, MB_OK|MB_ICONWARNING, NULL);
		return;
	}
	bool bRslt = true;
	if(gConfigMgr.CreateNewCrownCapTask(strTaskName, m_bImgInside))
	{
		m_pDictChConfig = gConfigMgr.GetChannelConfigPtr();
		int count = m_comboTask.GetCount();
		m_comboTask.InsertString(count, strTaskName.c_str());
		m_comboTask.SetCurSel(count);
		m_nTaskIndex = count;
		CWnd* pCheck = GetDlgItem(IDC_CHECK_IMAGE_INSIDE);
		pCheck->EnableWindow(FALSE);
		m_btnCreateTask.EnableWindow(FALSE);
		m_btnDeleteTask.EnableWindow(TRUE);
		m_btnRetraining.EnableWindow(FALSE);
		gSystemMgr.SetChannelTaskChangedFlag(1, TA_ADD);
		gSystemMgr.SetChannelTaskChangedFlag(2, TA_ADD);

		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		CString sPrompt;
		sPrompt.LoadString(IDS_STRING_BACK_TO_MAIN_UI);
		sMsg += sPrompt;
		UpdateData(FALSE);
	}
	else
	{
		bRslt = false;
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
	}
	DisplayPromptMessage(sMsg, bRslt);
}

void CCrownCapTaskMgrDlg::OnBnClickedButtonDeleteTask()
{
	if(m_nTaskIndex <= 0)
		return;

	UpdateData(TRUE);
	CString sTaskName;
	m_comboTask.GetLBText(m_nTaskIndex, sTaskName);
	if(m_nCrtProdudctId == 1) // Crown Cap
	{
		CString sMsg;
		wstring strTaskName = CT2W(sTaskName);
		if(gConfigMgr.IsTaskInused(strTaskName))
		{
			sMsg.LoadString(IDS_STRING_WARNING_DELETE_TASK);
			AfxMessageBox(sMsg, MB_OK, NULL);
		    return;
		}
		bool bRslt = true;
		if(gConfigMgr.DeleteCrownCapTask(strTaskName))
		{
			m_pDictChConfig = gConfigMgr.GetChannelConfigPtr();
			m_comboTask.DeleteString(m_nTaskIndex);
			m_comboTask.SetCurSel(0);
			m_nTaskIndex = 0;
			CWnd* pCheck = GetDlgItem(IDC_CHECK_IMAGE_INSIDE);
			pCheck->EnableWindow(TRUE);
			m_bImgInside = FALSE;
			m_btnCreateTask.EnableWindow(TRUE);
			m_btnDeleteTask.EnableWindow(FALSE);
			m_btnRetraining.EnableWindow(FALSE);
			gSystemMgr.SetChannelTaskChangedFlag(1, TA_DELETE);
			gSystemMgr.SetChannelTaskChangedFlag(2, TA_DELETE);
			UpdateData(FALSE);
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

void CCrownCapTaskMgrDlg::OnBnClickedButtonRetrainning()
{
	if(m_nTaskIndex <= 0)
		return;
	CString sTaskName;
	m_comboTask.GetLBText(m_nTaskIndex,sTaskName);
	wstring strTaskName = CT2W(sTaskName);
	CString sMsg;
	bool bRslt = true;
	if(gConfigMgr.RetrainningTask(strTaskName, m_bImgInside))
	{
		m_btnRetraining.EnableWindow(FALSE);
		gSystemMgr.SetChannelTaskChangedFlag(1, TA_TRAINING);
		gSystemMgr.SetChannelTaskChangedFlag(2, TA_TRAINING);

		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		CString sPrompt;
		sPrompt.LoadString(IDS_STRING_BACK_TO_MAIN_UI);
		sMsg += sPrompt;
	}
	else
	{
		bRslt = false;
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
	}
	DisplayPromptMessage(sMsg, bRslt);
}

HBRUSH CCrownCapTaskMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_TASK_DLG_MSG_PANEL)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();

		if(m_bOpRslt)
			pDC->SetTextColor(RGB(50,200,20));
		else
			pDC->SetTextColor(RGB(200,20,20));

		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CCrownCapTaskMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_TASK_DLG_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_TASK_DLG_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_CROWN_CAP_TASK_DLG, 5000, NULL);
}

void CCrownCapTaskMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(TIMER_ID_CROWN_CAP_TASK_DLG == nIDEvent)
	{
		SetDlgItemTextW(IDC_STATIC_TASK_DLG_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_CROWN_CAP_TASK_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CCrownCapTaskMgrDlg::SwitchToProduct(int nProductIndex)
{
	//TODO:
}
