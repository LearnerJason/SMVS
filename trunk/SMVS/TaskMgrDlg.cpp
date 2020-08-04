#include "stdafx.h"
#include "SMVS.h"
#include "TaskMgrDlg.h"
#include "afxdialogex.h"
#include "CopyTaskDlg.h"


IMPLEMENT_DYNAMIC(CTaskMgrDlg, CDialogEx)

CTaskMgrDlg::CTaskMgrDlg(const vector<int>& vProduct,CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskMgrDlg::IDD, pParent)
	, m_vProductId(vProduct)
	, m_nCrtProdudctId(vProduct[0])
	,TIMER_ID_TASK_DLG(100006)
	,m_bOpRslt(true)
	,m_nChIndex(-1)
	, m_nRow(-1)
	, m_nCol(-1)
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
}

CTaskMgrDlg::~CTaskMgrDlg()
{
}

void CTaskMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TM_CREATE_TASK, m_btnCreateTask);
	DDX_Control(pDX, IDC_BUTTON_TM_DELETE_TASK, m_btnDeleteTask);
	DDX_Control(pDX, IDC_COMBO_TM_TASK_MODEL, m_comboTaskModel);
	DDX_Control(pDX, IDC_COMBO_TM_TASK, m_comboTask);
	DDX_Control(pDX, IDC_BUTTON_TM_UPDATE_MODEL, m_btnUpdateModel);
	DDX_Control(pDX, IDC_BUTTON_TM_COPY_TASK, m_btnCopyTask);
}

BEGIN_MESSAGE_MAP(CTaskMgrDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TM_CH, &CTaskMgrDlg::OnCbnSelchangeComboTmCh)
	ON_CBN_SELCHANGE(IDC_COMBO_TM_TASK, &CTaskMgrDlg::OnCbnSelchangeComboTmTask)
	ON_BN_CLICKED(IDC_BUTTON_TM_CREATE_TASK, &CTaskMgrDlg::OnBnClickedButtonTmCreateTask)
	ON_BN_CLICKED(IDC_BUTTON_TM_DELETE_TASK, &CTaskMgrDlg::OnBnClickedButtonTmDeleteTask)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_TM_TASK_MODEL, &CTaskMgrDlg::OnCbnSelchangeComboTmTaskModel)
	ON_BN_CLICKED(IDC_BUTTON_TM_UPDATE_MODEL, &CTaskMgrDlg::OnBnClickedButtonTmUpdateModel)
	ON_BN_CLICKED(IDC_BUTTON_TM_COPY_TASK, &CTaskMgrDlg::OnBnClickedButtonTmCopyTask)
END_MESSAGE_MAP()

BOOL CTaskMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);
	
	InitUiWidget(IDC_STATIC_TM_CRT_CH,IDS_STRING_SYS_DLG_CRT_CH);
	InitUiWidget(IDC_COMBO_TM_CH);

	InitUiWidget(IDC_STATIC_TM_TASK_NAME,IDS_STRING_TASK_NAME);
	InitUiWidget(IDC_COMBO_TM_TASK);

	InitUiWidget(IDC_STATIC_TM_TASK_MODEL, IDS_STRING_TASK_MODEL);
	InitUiWidget(IDC_COMBO_TM_TASK_MODEL);

	InitUiWidget(IDC_BUTTON_TM_CREATE_TASK,IDS_STRING_CREATE_TASK);
	InitUiWidget(IDC_BUTTON_TM_DELETE_TASK,IDS_STRING_DELETE_TASK);
	InitUiWidget(IDC_BUTTON_TM_COPY_TASK, IDS_STRING_COPY_TASK);
	InitUiWidget(IDC_BUTTON_TM_UPDATE_MODEL,IDS_STRING_TM_UPDATE_MODEL);
	
	InitUiWidget(IDC_STATIC_TM_MSG_PANEL);

	m_btnCreateTask.EnableWindow(TRUE);
	m_btnDeleteTask.EnableWindow(FALSE);
	m_btnCopyTask.EnableWindow(FALSE);
	m_btnUpdateModel.EnableWindow(FALSE);
	FillUpUiWidget();
	UpdateData(FALSE);

	return TRUE;
}

BOOL CTaskMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTaskMgrDlg::SetCrtProductIndex(int nProductIndex)
{
	if(nProductIndex < (int)m_vProductId.size())
		m_nCrtProdudctId = m_vProductId[nProductIndex];
}

void CTaskMgrDlg::SwitchToProduct(int nProductIndex)
{
	if(nProductIndex >= (int)m_vProductId.size())
		return;
	int tmpID = m_vProductId[nProductIndex];
	if(m_nCrtProdudctId == tmpID)
		return;
	BOOL bVisible = IsWindowVisible();
	m_nCrtProdudctId = tmpID;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_CH);
	if(pCombo != nullptr)
	{
		pCombo->ResetContent();
	}
	m_comboTask.ResetContent();
	m_comboTaskModel.ResetContent();
	m_btnCreateTask.EnableWindow(TRUE);
	m_btnDeleteTask.EnableWindow(FALSE);
	m_btnCopyTask.EnableWindow(FALSE);
	m_btnUpdateModel.EnableWindow(FALSE);
	FillUpUiWidget();

	if(bVisible == TRUE)
	{
		UpdateData(FALSE);
	}
}

void CTaskMgrDlg::InitUiWidget(int nCtrlID, int strID)
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

void CTaskMgrDlg::ListChannelTask(int nChIndex, int nProductID)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK);
	if(pCombo == nullptr) return;
	pCombo->ResetContent();

	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	if(pChConfigArray->find(nChIndex) != pChConfigArray->end())
	{
		int index = 1;
		CString sTaskName;
		pCombo->InsertString(0, _T(""));
		CHANNEL_CONFIG chConfig = (*pChConfigArray)[nChIndex];
		for(auto it = chConfig.m_vTask.begin(); it != chConfig.m_vTask.end(); it++)
		{
			if(it->m_bSelfLearning) continue;
			if(it->m_nProductId != m_nCrtProdudctId) continue;

			sTaskName = CW2T(it->m_wstrTaskName.c_str());
			pCombo->InsertString(index, sTaskName);
			index++;
		}
		pCombo->SetCurSel(0);

		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK_MODEL);
		pCombo->SetCurSel(-1);
	}
}

void CTaskMgrDlg::ListTaskAndModel(int nChIndex)
{
	m_btnCopyTask.EnableWindow(FALSE);

	if(gSystemMgr.IsChannelCameraRunning(nChIndex))
	{
		m_btnCreateTask.EnableWindow(FALSE);
		m_btnDeleteTask.EnableWindow(FALSE);
		m_btnUpdateModel.EnableWindow(FALSE);
		CString sMsg;
		sMsg.LoadString(IDS_STRING_WARNING_CAMERA_IS_RUNNING);
		CString sIndex;
		sIndex.Format(_T("%d"), nChIndex);
		sMsg.Replace(_T("#1"),sIndex);
		DisplayPromptMessage(sMsg, false);
	}
	else
	{
		m_btnCreateTask.EnableWindow(TRUE);
		m_btnDeleteTask.EnableWindow(FALSE);
		m_btnUpdateModel.EnableWindow(FALSE);
	}
	ListChannelTask(nChIndex, m_nCrtProdudctId);
	PrepareTaskModelList(m_nCrtProdudctId);
}

void CTaskMgrDlg::FillUpUiWidget()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_CH);

	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	int index = 0;
	if(pChConfigArray->empty() == false)
	{
		CString sCh;
		for(auto it = pChConfigArray->begin(); it != pChConfigArray->end(); it++)
		{
			sCh.Format(_T("CH%d"), it->first);
			pCombo->InsertString(index, sCh);
			index++;
		}
		pCombo->SetCurSel(0);
		m_nChIndex = 1;
		ListTaskAndModel(m_nChIndex);
	}
	UpdateData(FALSE);
}

void CTaskMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_TM_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_TM_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_TASK_DLG, 8000, NULL);
}

void CTaskMgrDlg::OnCbnSelchangeComboTmCh()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_CH);
	if(pCombo == nullptr)
		return;
	int nChIndex = pCombo->GetCurSel();
	if(nChIndex == -1) 
		return;
	m_nChIndex = nChIndex + 1;
	ListTaskAndModel(m_nChIndex);

	UpdateData(FALSE);
}

void CTaskMgrDlg::OnCbnSelchangeComboTmTask()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK);	
	if(pCombo == nullptr)
		return;

	bool bCameraRun = gSystemMgr.IsChannelCameraRunning(m_nChIndex);

	int index = pCombo->GetCurSel();
	bool bCreateNew = (index <= 0);
	m_btnCreateTask.EnableWindow(!bCameraRun && bCreateNew);
	m_btnDeleteTask.EnableWindow(!bCameraRun && !bCreateNew);
	m_btnCopyTask.EnableWindow(!bCreateNew);
	m_btnUpdateModel.EnableWindow(!bCreateNew);
	if(bCreateNew)
	{
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK_MODEL);
		pCombo->SetCurSel(-1);
	}
	else
	{
		int nCrtIndex = pCombo->GetCurSel();
		CString sTaskName;
		pCombo->GetLBText(nCrtIndex, sTaskName);
		wstring wstrTaskName = CT2W(sTaskName);
		if(gConfigMgr.GetTask(m_nChIndex, wstrTaskName, m_crtTask))
		{
			wstring wstrModelName = gConfigMgr.GetModelNameOfThisTask(m_nChIndex, wstrTaskName);
			CString sModelName = CW2T(wstrModelName.c_str());
			pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK_MODEL);
			if(pCombo != nullptr && !wstrModelName.empty())
			{
				int count = pCombo->GetCount();
				for(int i = 0; i < count; i++)
				{
					CString str;
					pCombo->GetLBText(i, str);
					if(str == sModelName)
					{
						pCombo->SetCurSel(i);
					}
				}
			}
		}
	}
	UpdateData(FALSE);
}

void CTaskMgrDlg::OnCbnSelchangeComboTmTaskModel()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK_MODEL);	
	if(pCombo == nullptr)
		return;
	int index = pCombo->GetCurSel();
	if(index < 0)
	{
		return;
	}
	UpdateData(FALSE);
}

void CTaskMgrDlg::OnBnClickedButtonTmCreateTask()
{
	CString sMsg;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK);
	CString sTaskName;
	pCombo->GetWindowText(sTaskName);
	if(sTaskName.GetLength() <= 0)
	{
		sMsg.LoadString(IDS_STRING_PLEASE_INPUT_TASK_NAME);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	wstring wstrTaskName = CT2W(sTaskName);

	if(utils::CheckSpecialCharacter(wstrTaskName))
	{
		sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	if(!gConfigMgr.CheckTaskNameUniqueness(wstrTaskName))
	{
		sMsg.LoadString(IDS_STRING_WARNING_TASK_NAME_INUSED);
		sMsg.Replace(_T("#1"),sTaskName);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK_MODEL);
	int index = pCombo->GetCurSel();
	if(index == -1)
	{
		sMsg.LoadString(IDS_STRING_PLEASE_SELECT_TASK_MODEL);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	CString sModelName;
	pCombo->GetLBText(index, sModelName);
	wstring wstrModelName = CT2W(sModelName);
	bool bRet = gConfigMgr.CreateNewTask(m_nChIndex, wstrTaskName, wstrModelName);
	if(bRet)
	{
		CHANNEL_CONFIG chConfig = gConfigMgr.GetChannelConfig(m_nChIndex);
		gSystemMgr.UpdateChConfig(m_nChIndex,chConfig);

		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		ListChannelTask(m_nChIndex, m_nCrtProdudctId);
		m_btnCreateTask.EnableWindow(TRUE);
		m_btnDeleteTask.EnableWindow(FALSE);
		m_btnCopyTask.EnableWindow(FALSE);
		m_btnUpdateModel.EnableWindow(FALSE);
		gSystemMgr.SetChannelTaskChangedFlag(m_nChIndex,TA_ADD);
	}
	else
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
	}
	DisplayPromptMessage(sMsg, bRet);
	UpdateData(FALSE);
}

void CTaskMgrDlg::OnBnClickedButtonTmDeleteTask()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK);
	int nCrtIndex = pCombo->GetCurSel();
	CString sMsg;
	if(nCrtIndex < 0)
	{
		sMsg.LoadString(IDS_STRING_PLEASE_SELECT_EXISTED_TASK);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	else
	{
		CString sTaskName;
		pCombo->GetLBText(nCrtIndex, sTaskName);
		wstring wstrTaskName = CT2W(sTaskName);
		bool bRet = false;
		if(gConfigMgr.DeleteLightControllerParam(m_nChIndex, wstrTaskName) && gConfigMgr.DeleteTask(m_nChIndex, wstrTaskName,true,true)&&gSystemMgr.DeleteTable(wstrTaskName))
		{
			wstring exepath;
			utils::GetExePath(exepath);
			wstring wstrNewChTaskName = L"CH";
			wstrNewChTaskName.append(boost::lexical_cast<wstring>(m_nChIndex));
			wstrNewChTaskName.append(L"\\");
			wstrNewChTaskName.append(wstrTaskName.c_str());
			wstring wstrTaskModelPath =  exepath +L"\\data\\"+ wstrNewChTaskName;

			utils::DeleteDirectory(wstrTaskModelPath.c_str());

			for (int i = 0 ; i < 6 ;i++)
			{
				wstrNewChTaskName = L"CH";
				wstrNewChTaskName.append(boost::lexical_cast<wstring>(m_nChIndex));
				wstrNewChTaskName.append(boost::lexical_cast<wstring>(i));
				wstrNewChTaskName.append(L"\\");
				wstrNewChTaskName.append(wstrTaskName.c_str());
				wstrTaskModelPath =  exepath +L"\\data\\"+ wstrNewChTaskName;

				utils::DeleteDirectory(wstrTaskModelPath.c_str());
			}


			CHANNEL_CONFIG chConfig = gConfigMgr.GetChannelConfig(m_nChIndex);
			gSystemMgr.UpdateChConfig(m_nChIndex,chConfig);

			sMsg.LoadString(IDS_STRING_OPERATION_OK);
			ListChannelTask(m_nChIndex, m_nCrtProdudctId);
			m_comboTask.SetCurSel(-1);
			m_comboTaskModel.SetCurSel(-1);
			m_btnCreateTask.EnableWindow(TRUE);
			m_btnDeleteTask.EnableWindow(FALSE);
			m_btnCopyTask.EnableWindow(FALSE);
			m_btnUpdateModel.EnableWindow(FALSE);
			gSystemMgr.SetChannelTaskChangedFlag(m_nChIndex,TA_DELETE);
			bRet = true;
		}
		else
		{
			sMsg.LoadString(IDS_STRING_OPERATION_NG);
		}
		DisplayPromptMessage(sMsg, bRet);
		UpdateData(FALSE);
	}
}

void CTaskMgrDlg::OnBnClickedButtonTmCopyTask()
{
	int nChCount = gConfigMgr.GetChannelCount();
	CString sSrcTaskName = m_crtTask.m_wstrTaskName.c_str();
	CString sMsg;

	std::map<int, wstring> dictTarget;
	CCopyTaskDlg dlg(nChCount, m_nChIndex,sSrcTaskName,gConfigMgr.m_setTaskName,dictTarget);
	if(dlg.DoModal() == IDOK)
	{
		if(!dictTarget.empty())
		{
			wstring wstrSrcTaskName = CT2W(sSrcTaskName);
			bool bRet = gConfigMgr.DuplicateTask(m_nChIndex, wstrSrcTaskName, dictTarget);
			if(bRet)
			{
				sMsg.LoadString(IDS_STRING_OPERATION_OK);
				for(auto it = dictTarget.begin();it != dictTarget.end(); it++)
				{
					CHANNEL_CONFIG chConfig = gConfigMgr.GetChannelConfig(it->first);
					gSystemMgr.UpdateChConfig(it->first,chConfig);
					gSystemMgr.SetChannelTaskChangedFlag(it->first,TA_ADD);
				}
			}
			else
			{
				sMsg.LoadString(IDS_STRING_OPERATION_NG);
			}
			DisplayPromptMessage(sMsg, bRet);
			UpdateData(FALSE);
		}
		OnCbnSelchangeComboTmCh();
	}
}

void CTaskMgrDlg::OnBnClickedButtonTmUpdateModel()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK);
	int nCrtIndex = pCombo->GetCurSel();
	CString sMsg;
	if(nCrtIndex < 0)
	{
		sMsg.LoadString(IDS_STRING_PLEASE_SELECT_EXISTED_TASK);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	CString sTaskName;
	pCombo->GetLBText(nCrtIndex, sTaskName);
	wstring wstrTaskName = CT2W(sTaskName);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK_MODEL);
	nCrtIndex = pCombo->GetCurSel();
	if(nCrtIndex < 0)
	{
		return;
	}
	CString sModelName;
	pCombo->GetLBText(nCrtIndex, sModelName);
	wstring wstrModelName = CT2W(sModelName);
	
	bool bRet = gConfigMgr.SynchronizeParamFromTask2Template(m_nChIndex, wstrTaskName, wstrModelName);
	if(bRet)
	{
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		m_btnCreateTask.EnableWindow(FALSE);
		m_btnDeleteTask.EnableWindow(TRUE);
		m_btnCopyTask.EnableWindow(TRUE);
		m_btnUpdateModel.EnableWindow(TRUE);
	}
	else
	{
		sMsg.LoadString(IDS_STRING_OPERATION_NG);
	}
	DisplayPromptMessage(sMsg, bRet);
	UpdateData(FALSE);
}

void CTaskMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(TIMER_ID_TASK_DLG == nIDEvent)
	{
		SetDlgItemTextW(IDC_STATIC_TM_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_TASK_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CTaskMgrDlg::PrepareTaskModelList(int nProductID)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TM_TASK_MODEL);
	pCombo->ResetContent();

	auto pTaskModelArray = gConfigMgr.GetTaskTemplateArrayPtr();
	if(pTaskModelArray->empty() == false)
	{
		CString sModelName;
		int index = 0;
		for(auto it = pTaskModelArray->begin(); it != pTaskModelArray->end();it++)
		{
			if(it->second.m_nProductId == nProductID)
			{
				wstring strModelName = it->second.m_strModelName;
				sModelName = CW2T(strModelName.c_str());
				pCombo->InsertString(index, sModelName);
				index++;
			}
		}
		if(pCombo->GetCount() == 0)
		{
			CString sMsg;
			sMsg.LoadString(IDS_STRING_NO_TASK_MODEL);
			DisplayPromptMessage(sMsg, false);
		}
		pCombo->SetCurSel(-1);
	}
}

HBRUSH CTaskMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_TM_MSG_PANEL)
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

CString CTaskMgrDlg::SelectFiles()
{
	CString strFilter = _T("All Files (*.*)|*.*|Model Files (*.shm)|*.shm|TUP File (*.tup)|*.tup|TIFF File (*.tiff)|*.tiff|PNG File (*.png)|*.png||");
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
	}
	return strFilePath;
}

void CTaskMgrDlg::EnterFilePath(int cCtrlID)
{
	UpdateData(TRUE);
	CString strValue;
	GetDlgItemTextW(cCtrlID,strValue);
	CString newFiles = SelectFiles();
	if(strValue.GetLength() > 0)
		strValue.Append(_T(";"));
	strValue.Append(newFiles);
	SetDlgItemTextW(cCtrlID, strValue);
	UpdateData(FALSE);
}
