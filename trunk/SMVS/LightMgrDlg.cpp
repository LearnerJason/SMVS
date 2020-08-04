#include "stdafx.h"
#include "SMVS.h"
#include "LightMgrDlg.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CLightMgrDlg, CDialogEx)

CLightMgrDlg::CLightMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightMgrDlg::IDD, pParent)
	, m_nCrtChIndex(-1)
	, m_bOpRslt(true)
	, m_pDictChConfig(nullptr)
	, TIMER_ID_LIGHT_DLG(100022)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_vCheckCtrlID.clear();
	m_nLcCount = 0;
}

CLightMgrDlg::~CLightMgrDlg()
{
}

void CLightMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERIAL_PORT, m_comboSerialPort);
}

BEGIN_MESSAGE_MAP(CLightMgrDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_CRT_CHANNEL_ID, &CLightMgrDlg::OnCbnSelchangeComboCrtChannelId)
	ON_STN_CLICKED(IDC_STATIC_APPLY_LIGHT_PARAM, &CLightMgrDlg::OnStnClickedStaticApplyLightParam)
	ON_CBN_SELCHANGE(IDC_COMBO_SERIAL_PORT, &CLightMgrDlg::OnCbnSelchangeComboSerialPort)
	ON_BN_CLICKED(IDC_CHECK_LC0, &CLightMgrDlg::OnBnClickedCheckLc0)
	ON_BN_CLICKED(IDC_CHECK_LC1, &CLightMgrDlg::OnBnClickedCheckLc1)
	ON_BN_CLICKED(IDC_CHECK_LC2, &CLightMgrDlg::OnBnClickedCheckLc2)
	ON_BN_CLICKED(IDC_CHECK_LC3, &CLightMgrDlg::OnBnClickedCheckLc3)
	ON_BN_CLICKED(IDC_CHECK_LC4, &CLightMgrDlg::OnBnClickedCheckLc4)
	ON_BN_CLICKED(IDC_CHECK_LC5, &CLightMgrDlg::OnBnClickedCheckLc5)
	ON_BN_CLICKED(IDC_CHECK_LC6, &CLightMgrDlg::OnBnClickedCheckLc6)
	ON_BN_CLICKED(IDC_CHECK_LC7, &CLightMgrDlg::OnBnClickedCheckLc7)
	ON_BN_CLICKED(IDC_CHECK_LC8, &CLightMgrDlg::OnBnClickedCheckLc8)
	ON_BN_CLICKED(IDC_CHECK_LC9, &CLightMgrDlg::OnBnClickedCheckLc9)
	ON_BN_CLICKED(IDC_CHECK_LC10, &CLightMgrDlg::OnBnClickedCheckLc10)
	ON_BN_CLICKED(IDC_CHECK_LC11, &CLightMgrDlg::OnBnClickedCheckLc11)
	ON_BN_CLICKED(IDC_CHECK_LC12, &CLightMgrDlg::OnBnClickedCheckLc12)
	ON_BN_CLICKED(IDC_CHECK_LC13, &CLightMgrDlg::OnBnClickedCheckLc13)
	ON_BN_CLICKED(IDC_CHECK_LC14, &CLightMgrDlg::OnBnClickedCheckLc14)
	ON_BN_CLICKED(IDC_CHECK_LC15, &CLightMgrDlg::OnBnClickedCheckLc15)
	ON_BN_CLICKED(IDC_CHECK_L11, &CLightMgrDlg::OnBnClickedCheckL11)
	ON_BN_CLICKED(IDC_CHECK_L12, &CLightMgrDlg::OnBnClickedCheckL12)
	ON_BN_CLICKED(IDC_CHECK_L13, &CLightMgrDlg::OnBnClickedCheckL13)
	ON_BN_CLICKED(IDC_CHECK_L14, &CLightMgrDlg::OnBnClickedCheckL14)
	ON_BN_CLICKED(IDC_CHECK_L21, &CLightMgrDlg::OnBnClickedCheckL21)
	ON_BN_CLICKED(IDC_CHECK_L22, &CLightMgrDlg::OnBnClickedCheckL22)
	ON_BN_CLICKED(IDC_CHECK_L23, &CLightMgrDlg::OnBnClickedCheckL23)
	ON_BN_CLICKED(IDC_CHECK_L24, &CLightMgrDlg::OnBnClickedCheckL24)

END_MESSAGE_MAP()

BOOL CLightMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);
	

	InitUiWidget(IDC_STATIC_SYS_DLG_CRT_CH, IDS_STRING_SYS_DLG_CRT_CH);
	InitUiWidget(IDC_COMBO_CRT_CHANNEL_ID);

	InitUiWidget(IDC_STATIC_SERIAL_PORT, IDS_STRING_SERIAL_PORT);
	InitUiWidget(IDC_COMBO_SERIAL_PORT);

	InitUiWidget(IDC_STATIC_GP_CONTROLLER,IDS_STRING_LC_OPTION);
	InitUiWidget(IDC_CHECK_LC0);  m_vCheckCtrlID.push_back(IDC_CHECK_LC0); 
	InitUiWidget(IDC_CHECK_LC1);  m_vCheckCtrlID.push_back(IDC_CHECK_LC1); 
	InitUiWidget(IDC_CHECK_LC2);  m_vCheckCtrlID.push_back(IDC_CHECK_LC2); 
	InitUiWidget(IDC_CHECK_LC3);  m_vCheckCtrlID.push_back(IDC_CHECK_LC3); 
	InitUiWidget(IDC_CHECK_LC4);  m_vCheckCtrlID.push_back(IDC_CHECK_LC4); 
	InitUiWidget(IDC_CHECK_LC5);  m_vCheckCtrlID.push_back(IDC_CHECK_LC5); 
	InitUiWidget(IDC_CHECK_LC6);  m_vCheckCtrlID.push_back(IDC_CHECK_LC6);
	InitUiWidget(IDC_CHECK_LC7);  m_vCheckCtrlID.push_back(IDC_CHECK_LC7); 
	InitUiWidget(IDC_CHECK_LC8);  m_vCheckCtrlID.push_back(IDC_CHECK_LC8); 
	InitUiWidget(IDC_CHECK_LC9);  m_vCheckCtrlID.push_back(IDC_CHECK_LC9); 
	InitUiWidget(IDC_CHECK_LC10); m_vCheckCtrlID.push_back(IDC_CHECK_LC10); 
	InitUiWidget(IDC_CHECK_LC11); m_vCheckCtrlID.push_back(IDC_CHECK_LC11);
	InitUiWidget(IDC_CHECK_LC12); m_vCheckCtrlID.push_back(IDC_CHECK_LC12); 
	InitUiWidget(IDC_CHECK_LC13); m_vCheckCtrlID.push_back(IDC_CHECK_LC13); 
	InitUiWidget(IDC_CHECK_LC14); m_vCheckCtrlID.push_back(IDC_CHECK_LC14); 
	InitUiWidget(IDC_CHECK_LC15); m_vCheckCtrlID.push_back(IDC_CHECK_LC15); 
	InitUiWidget(IDC_STATIC_LIGHT_DLG_MSG_PANEL);

	InitUiWidget(IDC_STATIC_TAG1);
	InitUiWidget(IDC_STATIC_T1L1); InitUiWidget(IDC_EDIT_TAG1_LINE1);
	InitUiWidget(IDC_STATIC_T1L2); InitUiWidget(IDC_EDIT_TAG1_LINE2);
	InitUiWidget(IDC_STATIC_T1L3); InitUiWidget(IDC_EDIT_TAG1_LINE3);
	InitUiWidget(IDC_STATIC_T1L4); InitUiWidget(IDC_EDIT_TAG1_LINE4);

	InitUiWidget(IDC_STATIC_TAG2);
	InitUiWidget(IDC_STATIC_T2L1); InitUiWidget(IDC_EDIT_TAG2_LINE1);
	InitUiWidget(IDC_STATIC_T2L2); InitUiWidget(IDC_EDIT_TAG2_LINE2);
	InitUiWidget(IDC_STATIC_T2L3); InitUiWidget(IDC_EDIT_TAG2_LINE3);
	InitUiWidget(IDC_STATIC_T2L4); InitUiWidget(IDC_EDIT_TAG2_LINE4);

	m_pDictChConfig = gConfigMgr.GetChannelConfigPtr(); 
	if(m_pDictChConfig == nullptr)
		return FALSE;

	FillUpUiWidget();

	return TRUE;
}

void CLightMgrDlg::InitUiWidget(int nCtrlID, int strID)
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

void CLightMgrDlg::DisplayWidget(int nCtrlID, BOOL bVisible)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	if(pWnd != nullptr)
	{
		if(nCtrlID != NULL)
		{
			if(bVisible)
				pWnd->ShowWindow(SW_SHOW);
			else
			    pWnd->ShowWindow(SW_HIDE);
		}
		pWnd->Invalidate();
	}
}

void CLightMgrDlg::HideAllTagCtrl()
{
	DisplayWidget(IDC_STATIC_TAG1, SW_HIDE);

	DisplayWidget(IDC_STATIC_T1L1, SW_HIDE);
	DisplayWidget(IDC_CHECK_L11, SW_HIDE);
	DisplayWidget(IDC_EDIT_TAG1_LINE1, SW_HIDE);

	DisplayWidget(IDC_STATIC_T1L2, SW_HIDE);
	DisplayWidget(IDC_CHECK_L12, SW_HIDE);
	DisplayWidget(IDC_EDIT_TAG1_LINE2, SW_HIDE);

	DisplayWidget(IDC_STATIC_T1L3, SW_HIDE);
	DisplayWidget(IDC_CHECK_L13, SW_HIDE);
	DisplayWidget(IDC_EDIT_TAG1_LINE3, SW_HIDE);

	DisplayWidget(IDC_STATIC_T1L4, SW_HIDE);
	DisplayWidget(IDC_CHECK_L14, SW_HIDE);
	DisplayWidget(IDC_EDIT_TAG1_LINE4, SW_HIDE);

	DisplayWidget(IDC_STATIC_TAG2, SW_HIDE);

	DisplayWidget(IDC_STATIC_T2L1, SW_HIDE);
	DisplayWidget(IDC_CHECK_L21, SW_HIDE);
	DisplayWidget(IDC_EDIT_TAG2_LINE1, SW_HIDE);

	DisplayWidget(IDC_STATIC_T2L2, SW_HIDE);
	DisplayWidget(IDC_CHECK_L22, SW_HIDE);
	DisplayWidget(IDC_EDIT_TAG2_LINE2, SW_HIDE);

	DisplayWidget(IDC_STATIC_T2L3, SW_HIDE);
	DisplayWidget(IDC_CHECK_L23, SW_HIDE);
	DisplayWidget(IDC_EDIT_TAG2_LINE3, SW_HIDE);

	DisplayWidget(IDC_STATIC_T2L4, SW_HIDE);
	DisplayWidget(IDC_CHECK_L24, SW_HIDE);
	DisplayWidget(IDC_EDIT_TAG2_LINE4, SW_HIDE);
}

void CLightMgrDlg::DisplayTagGroup(int nCtrlID, int nLcID , wstring wstrTags, BOOL bVisible)
{
	CString sGpName;
	sGpName.Format(_T("LC%d"), nLcID);

	m_nLcCount++;
	std::vector<wstring> vTags;
	if(wstrTags == L"")
	{
		vTags.push_back(L"Line1");
		vTags.push_back(L"Line2");
		vTags.push_back(L"Line3");
		vTags.push_back(L"Line4");
	}
	else
	{
		vTags = utils::SplitString(wstrTags, L"$",token_compress_off,false);
	}
	
	assert(vTags.size() == 4);

	if(nCtrlID == IDC_STATIC_TAG1)
	{
		DisplayWidget(IDC_STATIC_TAG1, bVisible);
		SetDlgItemTextW(IDC_STATIC_TAG1, sGpName);

		DisplayWidget(IDC_STATIC_T1L1, bVisible);
		DisplayWidget(IDC_CHECK_L11, bVisible);
		DisplayWidget(IDC_EDIT_TAG1_LINE1, bVisible);
		SetDlgItemTextW(IDC_EDIT_TAG1_LINE1, vTags[0].c_str());

		DisplayWidget(IDC_STATIC_T1L2, bVisible);
		DisplayWidget(IDC_CHECK_L12, bVisible);
		DisplayWidget(IDC_EDIT_TAG1_LINE2, bVisible);
		SetDlgItemTextW(IDC_EDIT_TAG1_LINE2, vTags[1].c_str());

		DisplayWidget(IDC_STATIC_T1L3, bVisible);
		DisplayWidget(IDC_CHECK_L13, bVisible);
		DisplayWidget(IDC_EDIT_TAG1_LINE3, bVisible);
		SetDlgItemTextW(IDC_EDIT_TAG1_LINE3, vTags[2].c_str());

		DisplayWidget(IDC_STATIC_T1L4, bVisible);
		DisplayWidget(IDC_CHECK_L14, bVisible);
		DisplayWidget(IDC_EDIT_TAG1_LINE4, bVisible);
		SetDlgItemTextW(IDC_EDIT_TAG1_LINE4, vTags[3].c_str());

		if (vTags[0]==L"")
		{
			((CButton*)GetDlgItem(IDC_CHECK_L11))->SetCheck(false);
		}
		else
		{
			if ( CheckLCLine(nLcID,0,false))
			{
				((CButton*)GetDlgItem(IDC_CHECK_L11))->SetCheck(true);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_L11))->SetCheck(false);
			}
		}
		if (vTags[1]==L"")
		{
			((CButton*)GetDlgItem(IDC_CHECK_L12))->SetCheck(false);
		}
		else
		{
			if ( CheckLCLine(nLcID,1,false))
			{
				((CButton*)GetDlgItem(IDC_CHECK_L12))->SetCheck(true);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_L12))->SetCheck(false);
			}
		}
		if (vTags[2]==L"")
		{
			((CButton*)GetDlgItem(IDC_CHECK_L13))->SetCheck(false);
		}
		else
		{
			if ( CheckLCLine(nLcID,2,false))
			{
				((CButton*)GetDlgItem(IDC_CHECK_L13))->SetCheck(true);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_L13))->SetCheck(false);
			}
		}
		if (vTags[3]==L"")
		{
			((CButton*)GetDlgItem(IDC_CHECK_L14))->SetCheck(false);
		}
		else
		{
			if ( CheckLCLine(nLcID,3,false))
			{
				((CButton*)GetDlgItem(IDC_CHECK_L14))->SetCheck(true);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_L14))->SetCheck(false);
			}
		}
	}
	if(nCtrlID == IDC_STATIC_TAG2)
	{
		DisplayWidget(IDC_STATIC_TAG2, bVisible);
		SetDlgItemTextW(IDC_STATIC_TAG2, sGpName);

		DisplayWidget(IDC_STATIC_T2L1, bVisible);
		DisplayWidget(IDC_CHECK_L21, bVisible);
		DisplayWidget(IDC_EDIT_TAG2_LINE1, bVisible);
		SetDlgItemTextW(IDC_EDIT_TAG2_LINE1, vTags[0].c_str());

		DisplayWidget(IDC_STATIC_T2L2, bVisible);
		DisplayWidget(IDC_CHECK_L22, bVisible);
		DisplayWidget(IDC_EDIT_TAG2_LINE2, bVisible);
		SetDlgItemTextW(IDC_EDIT_TAG2_LINE2, vTags[1].c_str());

		DisplayWidget(IDC_STATIC_T2L3, bVisible);
		DisplayWidget(IDC_CHECK_L23, bVisible);
		DisplayWidget(IDC_EDIT_TAG2_LINE3, bVisible);
		SetDlgItemTextW(IDC_EDIT_TAG2_LINE3, vTags[2].c_str());

		DisplayWidget(IDC_STATIC_T2L4, bVisible);
		DisplayWidget(IDC_CHECK_L24, bVisible);
		DisplayWidget(IDC_EDIT_TAG2_LINE4, bVisible);
		SetDlgItemTextW(IDC_EDIT_TAG2_LINE4, vTags[3].c_str());

		if (vTags[0]==L"")
		{
			((CButton*)GetDlgItem(IDC_CHECK_L21))->SetCheck(false);
		}
		else
		{
			if ( CheckLCLine(nLcID,0,false))
			{
				((CButton*)GetDlgItem(IDC_CHECK_L21))->SetCheck(true);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_L21))->SetCheck(false);
			}
		}
		if (vTags[1]==L"")
		{
			((CButton*)GetDlgItem(IDC_CHECK_L22))->SetCheck(false);
		}
		else
		{
			if ( CheckLCLine(nLcID,1,false))
			{
				((CButton*)GetDlgItem(IDC_CHECK_L22))->SetCheck(true);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_L22))->SetCheck(false);
			}
		}
		if (vTags[2]==L"")
		{
			((CButton*)GetDlgItem(IDC_CHECK_L23))->SetCheck(false);
		}
		else
		{
			if ( CheckLCLine(nLcID,2,false))
			{
				((CButton*)GetDlgItem(IDC_CHECK_L23))->SetCheck(true);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_L23))->SetCheck(false);
			}
		}
		if (vTags[3]==L"")
		{
			((CButton*)GetDlgItem(IDC_CHECK_L24))->SetCheck(false);
		}
		else
		{
			if ( CheckLCLine(nLcID,3,false))
			{
				((CButton*)GetDlgItem(IDC_CHECK_L24))->SetCheck(true);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_L24))->SetCheck(false);
			}
		}
	}
}
void CLightMgrDlg::InitializeSerialPortCombo()
{
	m_comboSerialPort.ResetContent();
	std::vector<wstring> vPort = utils::GetAllSerialPort();

	for(size_t t = 0; t < vPort.size(); t++)
	{
		m_comboSerialPort.AddString(vPort[t].c_str());
	}
}

void CLightMgrDlg::FillUpUiWidget()
{
	size_t sz = m_pDictChConfig->size();
	if(sz == 0) return;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	if(pCombo == nullptr)
		return;

	CString strCh;
	for(int t = 1; t <= sz; t++)
	{
		strCh.Format(_T("CH%d"), t);
		pCombo->AddString(strCh);
	}
	pCombo->SetCurSel(0);

	InitializeSerialPortCombo();

	m_nCrtChIndex = 1;
	DisplayLightSettings(m_nCrtChIndex);
}

void CLightMgrDlg::ResetUIWidget()
{
	m_comboSerialPort.SetCurSel(-1);
	for(size_t t = 0; t < m_vCheckCtrlID.size(); t++)
	{
		CButton* pWnd = (CButton*)GetDlgItem(m_vCheckCtrlID[t]);
		if(pWnd != nullptr)
		{
			pWnd->SetCheck(BST_UNCHECKED);
		}
	}
	m_nLcCount = 0;
	m_dictLcID_GpCrtlID.clear();
	HideAllTagCtrl();
	UpdateData(FALSE);
}

void CLightMgrDlg::EnableUiWidget(int nCtrlID, BOOL bEnable)
{
	CWnd* pWnd = (CWnd*)GetDlgItem(nCtrlID);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
}

void CLightMgrDlg::EnableAllWidge(bool bEnable)
{
	EnableUiWidget(IDC_COMBO_SHIFT_OPTION,bEnable);
	EnableUiWidget(IDC_CHECK_LC0,bEnable);
	EnableUiWidget(IDC_CHECK_LC1,bEnable);
	EnableUiWidget(IDC_CHECK_LC2,bEnable);
	EnableUiWidget(IDC_CHECK_LC3,bEnable);
	EnableUiWidget(IDC_CHECK_LC4,bEnable);
	EnableUiWidget(IDC_CHECK_LC5,bEnable);
	EnableUiWidget(IDC_CHECK_LC6,bEnable);
	EnableUiWidget(IDC_CHECK_LC7,bEnable);
	EnableUiWidget(IDC_CHECK_LC8,bEnable);
	EnableUiWidget(IDC_CHECK_LC9,bEnable);
	EnableUiWidget(IDC_CHECK_LC10,bEnable);
	EnableUiWidget(IDC_CHECK_LC11,bEnable);
	EnableUiWidget(IDC_CHECK_LC12,bEnable);
	EnableUiWidget(IDC_CHECK_LC13,bEnable);
	EnableUiWidget(IDC_CHECK_LC14,bEnable);
	EnableUiWidget(IDC_CHECK_LC15,bEnable);
	EnableUiWidget(IDC_EDIT_TAG1_LINE1,bEnable);
	EnableUiWidget(IDC_EDIT_TAG1_LINE2,bEnable);
	EnableUiWidget(IDC_EDIT_TAG1_LINE3,bEnable);
	EnableUiWidget(IDC_EDIT_TAG1_LINE4,bEnable);
	EnableUiWidget(IDC_EDIT_TAG2_LINE1,bEnable);
	EnableUiWidget(IDC_EDIT_TAG2_LINE2,bEnable);
	EnableUiWidget(IDC_EDIT_TAG2_LINE3,bEnable);
	EnableUiWidget(IDC_EDIT_TAG2_LINE4,bEnable);
	EnableUiWidget(IDC_STATIC_APPLY_LIGHT_PARAM,bEnable);
}

void CLightMgrDlg::DisplayLightSettings(int chIndex)
{
	if(m_pDictChConfig->find(chIndex) != m_pDictChConfig->end())
	{
		ResetUIWidget();

		CHANNEL_CONFIG chConfig  = (*m_pDictChConfig)[chIndex];
		m_crtLightSetting = chConfig.m_lightCtrlSetting;
		
		CString strSerialID;
		strSerialID.Format(_T("COM%d"),gSystemMgr.m_nSerialPort);
		int nRetIndex = m_comboSerialPort.FindStringExact(-1,strSerialID);
		m_comboSerialPort.SetCurSel(nRetIndex);

		size_t sz = m_crtLightSetting.m_vTags.size();
		for(size_t t = 0; t< m_crtLightSetting.m_vLcID.size(); t++)
		{
			int nLcID = m_crtLightSetting.m_vLcID[t];
			CButton* pCheck = (CButton*)GetDlgItem(m_vCheckCtrlID[nLcID]);
			if(pCheck != nullptr)
			{
				pCheck->SetCheck(BST_CHECKED);
			}
			CString sGroupName;
			sGroupName.Format(_T("LC%d"), nLcID);

			wstring strTags(L"");
			if(t < sz)
			{
				strTags = m_crtLightSetting.m_vTags[t];
			}
			if(t == 0)
			{
				m_dictLcID_GpCrtlID[nLcID] = IDC_STATIC_TAG1;
				DisplayTagGroup(IDC_STATIC_TAG1, nLcID, strTags, SW_SHOW);
			}
			if(t == 1)
			{
				m_dictLcID_GpCrtlID[nLcID] = IDC_STATIC_TAG2;
				DisplayTagGroup(IDC_STATIC_TAG2, nLcID, strTags, SW_SHOW);
			}
		}
		bool bChCameraRun = ::gSystemMgr.IsChannelCameraRunning(chIndex);
		EnableAllWidge(!bChCameraRun);
	}
	UpdateData(FALSE);
}

void CLightMgrDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_LIGHT_DLG_MSG_PANEL, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_LIGHT_DLG_MSG_PANEL, sMsg);
	SetTimer(TIMER_ID_LIGHT_DLG,5000, NULL);
}

BOOL CLightMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CLightMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID_LIGHT_DLG)
	{
		SetDlgItemTextW(IDC_STATIC_LIGHT_DLG_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_LIGHT_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CLightMgrDlg::OnCbnSelchangeComboCrtChannelId()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	if(pCombo == nullptr)
		return;

	m_nCrtChIndex = pCombo->GetCurSel();
	m_nCrtChIndex += 1;
	DisplayLightSettings(m_nCrtChIndex);
	UpdateData(FALSE);
}

void CLightMgrDlg::OnCbnSelchangeComboSerialPort()
{
	int nIndex = m_comboSerialPort.GetCurSel();
	if(nIndex != -1)
	{
		CString sCOM;
		m_comboSerialPort.GetLBText(nIndex, sCOM);
		wstring wstrCom = CT2W(sCOM);
		wstring wstrPort = wstrCom.substr(3, wstring::npos);
		if(utils::IsNumericString(wstrPort))
		{
			gSystemMgr.m_nSerialPort = boost::lexical_cast<int>(wstrPort);
			gSystemMgr.InitSerialPort();
			gConfigMgr.SetSerialPort(gSystemMgr.m_nSerialPort);
		}
	}
}

HBRUSH CLightMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_LIGHT_DLG_MSG_PANEL ||
	   id == IDC_CHECK_LC0 ||
	   id == IDC_CHECK_LC1 ||
	   id == IDC_CHECK_LC2 ||
	   id == IDC_CHECK_LC3 ||
	   id == IDC_CHECK_LC4 ||
	   id == IDC_CHECK_LC5 ||
	   id == IDC_CHECK_LC6 ||
       id == IDC_CHECK_LC7 ||
	   id == IDC_CHECK_LC8 ||
	   id == IDC_CHECK_LC9 ||
	   id == IDC_CHECK_LC10 ||
	   id == IDC_CHECK_LC11 ||
	   id == IDC_CHECK_LC12 ||
	   id == IDC_CHECK_LC13 ||
	   id == IDC_CHECK_LC14 ||
	   id == IDC_CHECK_LC15)
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


//single ch to do
void CLightMgrDlg::OnStnClickedStaticApplyLightParam()
{
	UpdateData(TRUE);
	LIGHT_SETTING oldSetting = m_crtLightSetting;
	m_crtLightSetting.m_vLcID.clear();
	m_crtLightSetting.m_vTags.clear();

	for(size_t t = 0; t < m_vCheckCtrlID.size(); t++)
	{
		CButton* pWnd = (CButton*)GetDlgItem(m_vCheckCtrlID[t]);
		if(pWnd != nullptr && pWnd->GetCheck() == BST_CHECKED)
		{
			m_crtLightSetting.m_vLcID.push_back((int)t);
		}
	}
	size_t sz = m_crtLightSetting.m_vLcID.size();
	for(size_t i = 0; i < sz; i++)
	{
		int nLcID = m_crtLightSetting.m_vLcID[i];
		if(m_dictLcID_GpCrtlID.find(nLcID) != m_dictLcID_GpCrtlID.end())
		{
			int nGpCtrlID = m_dictLcID_GpCrtlID[nLcID];
			CString sTag;
			wstring wstrTag;
			if(nGpCtrlID == IDC_STATIC_TAG1)
			{
				GetDlgItemTextW(IDC_EDIT_TAG1_LINE1, sTag); 
				if(((CButton*)GetDlgItem(IDC_CHECK_L11))->GetCheck()== BST_UNCHECKED)
				{
					sTag =_T("");
				}
				else
				{
					if(sTag.GetLength() <= 0) sTag = _T(" ");	
				}
				wstrTag.append(CT2W(sTag));wstrTag.append(L"$");

				GetDlgItemTextW(IDC_EDIT_TAG1_LINE2, sTag); 
				if(((CButton*)GetDlgItem(IDC_CHECK_L12))->GetCheck()== BST_UNCHECKED)
				{
					sTag =_T("");
				}
				else
				{
					if(sTag.GetLength() <= 0) sTag = _T(" ");
				}
				wstrTag.append(CT2W(sTag));wstrTag.append(L"$");
				GetDlgItemTextW(IDC_EDIT_TAG1_LINE3, sTag); 
				if(((CButton*)GetDlgItem(IDC_CHECK_L13))->GetCheck()== BST_UNCHECKED)
				{
					sTag =_T("");
				}
				else
				{
					if(sTag.GetLength() <= 0) sTag = _T(" ");
				}
				wstrTag.append(CT2W(sTag));wstrTag.append(L"$");
				GetDlgItemTextW(IDC_EDIT_TAG1_LINE4, sTag); 
				if(((CButton*)GetDlgItem(IDC_CHECK_L14))->GetCheck()== BST_UNCHECKED)
				{
					sTag =_T("");
				}
				else
				{
					if(sTag.GetLength() <= 0) sTag = _T(" ");		
				}
				wstrTag.append(CT2W(sTag));

				m_crtLightSetting.m_vTags.push_back(wstrTag);
			}
			if(nGpCtrlID == IDC_STATIC_TAG2)
			{
				GetDlgItemTextW(IDC_EDIT_TAG2_LINE1, sTag); 
				if(((CButton*)GetDlgItem(IDC_CHECK_L21))->GetCheck()== BST_UNCHECKED)
				{
					sTag =_T("");
				}
				else
				{
					if(sTag.GetLength() <= 0) sTag = _T(" ");	
				}
				wstrTag.append(CT2W(sTag));wstrTag.append(L"$");

				GetDlgItemTextW(IDC_EDIT_TAG2_LINE2, sTag); 
				if(((CButton*)GetDlgItem(IDC_CHECK_L22))->GetCheck()== BST_UNCHECKED)
				{
					sTag =_T("");
				}
				else
				{
					if(sTag.GetLength() <= 0) sTag = _T(" ");
				}
				wstrTag.append(CT2W(sTag));wstrTag.append(L"$");
				GetDlgItemTextW(IDC_EDIT_TAG2_LINE3, sTag); 
				if(((CButton*)GetDlgItem(IDC_CHECK_L23))->GetCheck()== BST_UNCHECKED)
				{
					sTag =_T("");
				}
				else
				{
					if(sTag.GetLength() <= 0) sTag = _T(" ");
				}
				wstrTag.append(CT2W(sTag));wstrTag.append(L"$");
				GetDlgItemTextW(IDC_EDIT_TAG2_LINE4, sTag); 
				if(((CButton*)GetDlgItem(IDC_CHECK_L24))->GetCheck()== BST_UNCHECKED)
				{
					sTag =_T("");
				}
				else
				{
					if(sTag.GetLength() <= 0) sTag = _T(" ");		
				}
				wstrTag.append(CT2W(sTag));
				m_crtLightSetting.m_vTags.push_back(wstrTag);
			}
		}
	}

	m_crtLightSetting.CorrectContrllerID();

	CString sMsg;
	if(gConfigMgr.UpdateLightControllerParam(m_nCrtChIndex, &m_crtLightSetting,true))
	{
		gSystemMgr.UpdateChLightController(m_nCrtChIndex, m_crtLightSetting); 
		sMsg.LoadString(IDS_STRING_OPERATION_OK);
		DisplayPromptMessage(sMsg, true);
		return;
	}
	sMsg.LoadString(IDS_STRING_OPERATION_NG);
	DisplayPromptMessage(sMsg, false);
}

void CLightMgrDlg::DisplayTagGroup(int nGpCtrlID, int nLcID, BOOL bVisible)
{
	m_crtLightSetting;
	CString sGpName;
	sGpName.Format(_T("LC%d"), nLcID);
	if(nGpCtrlID == IDC_STATIC_TAG1)
	{
		DisplayWidget(IDC_STATIC_TAG1, bVisible);
		SetDlgItemTextW(IDC_STATIC_TAG1, sGpName);

		DisplayWidget(IDC_STATIC_T1L1, bVisible);
		DisplayWidget(IDC_CHECK_L11, bVisible);
		DisplayWidget(IDC_EDIT_TAG1_LINE1, bVisible);

		DisplayWidget(IDC_STATIC_T1L2, bVisible);
		DisplayWidget(IDC_CHECK_L12, bVisible);
		DisplayWidget(IDC_EDIT_TAG1_LINE2, bVisible);

		DisplayWidget(IDC_STATIC_T1L3, bVisible);
		DisplayWidget(IDC_CHECK_L13, bVisible);
		DisplayWidget(IDC_EDIT_TAG1_LINE3, bVisible);

		DisplayWidget(IDC_STATIC_T1L4, bVisible);
			DisplayWidget(IDC_CHECK_L14, bVisible);
		DisplayWidget(IDC_EDIT_TAG1_LINE4, bVisible);

	}
	if(nGpCtrlID == IDC_STATIC_TAG2)
	{
		DisplayWidget(IDC_STATIC_TAG2, bVisible);
		SetDlgItemTextW(IDC_STATIC_TAG2, sGpName);

		DisplayWidget(IDC_STATIC_T2L1, bVisible);
		DisplayWidget(IDC_CHECK_L21, bVisible);
		DisplayWidget(IDC_EDIT_TAG2_LINE1, bVisible);

		DisplayWidget(IDC_STATIC_T2L2, bVisible);
		DisplayWidget(IDC_CHECK_L22, bVisible);
		DisplayWidget(IDC_EDIT_TAG2_LINE2, bVisible);

		DisplayWidget(IDC_STATIC_T2L3, bVisible);
		DisplayWidget(IDC_CHECK_L23, bVisible);
		DisplayWidget(IDC_EDIT_TAG2_LINE3, bVisible);

		DisplayWidget(IDC_STATIC_T2L4, bVisible);
		DisplayWidget(IDC_CHECK_L24, bVisible);
		DisplayWidget(IDC_EDIT_TAG2_LINE4, bVisible);
	}
}
BOOL CLightMgrDlg::CheckLCLine(int nLcID,int nLineID,bool bWarnDispaly)
{
	//获取当前通道CH id
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CRT_CHANNEL_ID);
	if(pCombo == nullptr)
		return TRUE;

	m_nCrtChIndex = pCombo->GetCurSel();
	m_nCrtChIndex += 1;

	//遍历其他通道 光源设置参数
	for (int i = 0 ; i < m_pDictChConfig->size(); i++)
	{
		if (i+1== m_nCrtChIndex)
			continue;
		else
		{
			CHANNEL_CONFIG chConfig  = (*m_pDictChConfig)[i+1];
			size_t TagSize = chConfig.m_lightCtrlSetting.m_vTags.size();
			for (int j = 0 ; j < TagSize ; j++ )
			{
				int LCID = chConfig.m_lightCtrlSetting.m_vLcID[j];//
				if (LCID != nLcID)
				{
					continue;
				}
				else
				{
					std::vector<wstring> vTags;
					wstring wstrTags =  chConfig.m_lightCtrlSetting.m_vTags[j];
					vTags = utils::SplitString(wstrTags, L"$",token_compress_off,false);
					assert(vTags.size() == 4);

					if (vTags[nLineID]!=_T(""))
					{
						if (bWarnDispaly)
						{
							CString ErrorMsg;
							ErrorMsg.LoadString(IDS_STRING_CHLINEUSED);
							CString strCH;
							strCH.Format(_T("CH%d"),i+1);
							ErrorMsg.Replace(_T("#1"),strCH);
							DisplayPromptMessage(ErrorMsg, false);
						}	
						return FALSE;
					}
				}

		
			}
				
		}
	}
	return TRUE;
}
void CLightMgrDlg::CheckLC(int nLcID, int nLcCtrID)
{
	CButton* pCheck = (CButton*)GetDlgItem(nLcCtrID);
	int nFlag = pCheck->GetCheck();
	if(pCheck->GetCheck() == BST_UNCHECKED)
	{
		if(m_dictLcID_GpCrtlID.find(nLcID) != m_dictLcID_GpCrtlID.end())
		{
			DisplayTagGroup(m_dictLcID_GpCrtlID[nLcID],nLcID,SW_HIDE);
			m_dictLcID_GpCrtlID.erase(nLcID);
			m_nLcCount--;
		}
		else
		{
			return;
		}
	}
	else
	{
		if(m_nLcCount == 2)
		{
			CString sMsg;
			sMsg.LoadString(IDS_STRING_WARMING_LIGHT_CONTROLLER_COUNT);
			DisplayPromptMessage(sMsg, false);
			pCheck->SetCheck(BST_UNCHECKED);
			return;
		}
		int nTgGpID = -1;
		if(m_nLcCount == 0)
		{
			nTgGpID = IDC_STATIC_TAG1;
		}
		bool bResort = false;
		if(m_nLcCount == 1)
		{
			auto it = m_dictLcID_GpCrtlID.begin();
		
			if(it->second == IDC_STATIC_TAG1)
			{
				nTgGpID = IDC_STATIC_TAG2;
			}
			if(it->second == IDC_STATIC_TAG2)
			{
				nTgGpID = IDC_STATIC_TAG1;
			}
		}

		CString sGroupName;
		sGroupName.Format(_T("LC%d"), nLcID);
		

		m_dictLcID_GpCrtlID[nLcID] = nTgGpID;
	//	DisplayTagGroup(nTgGpID, nLcID, SW_SHOW);

		bool bfind = false;
		int  i= 0; //记录tag位置
		for (auto iter = m_crtLightSetting.m_vLcID.begin(); iter != m_crtLightSetting.m_vLcID.end();iter++)
		{
			if (*iter ==  nLcID)
			{
				bfind = true;
				break;
			}
			 i++;
		}
		if (bfind)
		{
			if (nTgGpID == IDC_STATIC_TAG1)
			{
				DisplayTagGroup(IDC_STATIC_TAG1, nLcID,m_crtLightSetting.m_vTags[i], SW_SHOW);
			}
			else if(nTgGpID == IDC_STATIC_TAG2)
			{
				DisplayTagGroup(IDC_STATIC_TAG2, nLcID,m_crtLightSetting.m_vTags[i], SW_SHOW);

			}
		}
		else
		{
			DisplayTagGroup(nTgGpID, nLcID, _T(""), SW_SHOW);

		}
		

		//m_nLcCount++; //DisplayTagGroup 内部有m_nLcCount++
	}
}
void CLightMgrDlg::CheckLCALL()
{
	UpdateData(TRUE);
	HideAllTagCtrl();
	m_dictLcID_GpCrtlID.clear();
	m_nLcCount=0;
	CheckLC(0, IDC_CHECK_LC0);
	CheckLC(1, IDC_CHECK_LC1);
	CheckLC(2, IDC_CHECK_LC2);
	CheckLC(3, IDC_CHECK_LC3);
	CheckLC(4, IDC_CHECK_LC4);
	CheckLC(5, IDC_CHECK_LC5);
	CheckLC(6, IDC_CHECK_LC6);
	CheckLC(7, IDC_CHECK_LC7);
	CheckLC(8, IDC_CHECK_LC8);
	CheckLC(9, IDC_CHECK_LC9);
	CheckLC(10, IDC_CHECK_LC10);
	CheckLC(11, IDC_CHECK_LC11);
	CheckLC(12, IDC_CHECK_LC12);
	CheckLC(13, IDC_CHECK_LC13);
	CheckLC(14, IDC_CHECK_LC14);
	CheckLC(15, IDC_CHECK_LC15);

}
void CLightMgrDlg::OnBnClickedCheckLc0()
{
	CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc1()
{
	CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc2()
{
	CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc3()
{
	CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc4()
{	CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc5()
{
	CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc6()
{
	CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc7()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc8()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc9()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc10()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc11()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc12()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc13()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc14()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckLc15()
{
		CheckLCALL();
}

void CLightMgrDlg::OnBnClickedCheckL11()
{
	if (((CButton*)GetDlgItem(IDC_CHECK_L11))->GetCheck()==BST_CHECKED)
	{

		for (int i = 0;i < m_vCheckCtrlID.size(); i++)
		{
			int ichecked  =  ((CButton*)GetDlgItem(m_vCheckCtrlID[i]))->GetCheck();
			if (ichecked == BST_CHECKED)
			{
				if (!CheckLCLine(i,0))
				{
					((CButton*)GetDlgItem(IDC_CHECK_L11))->SetCheck(BST_UNCHECKED);
				}
			}
		}	// TODO: 在此添加控件通知处理程序代码
	}
}


void CLightMgrDlg::OnBnClickedCheckL12()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_L12))->GetCheck()==BST_CHECKED)
	{

		for (int i = 0;i < m_vCheckCtrlID.size(); i++)
		{
			int ichecked  =  ((CButton*)GetDlgItem(m_vCheckCtrlID[i]))->GetCheck();
			if (ichecked == BST_CHECKED)
			{
				
				if (!CheckLCLine(i,1))
				{
					((CButton*)GetDlgItem(IDC_CHECK_L12))->SetCheck(BST_UNCHECKED);
				}

			}
		}	// TODO: 在此添加控件通知处理程序代码
	}
}


void CLightMgrDlg::OnBnClickedCheckL13()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_L13))->GetCheck()==BST_CHECKED)
	{

		for (int i = 0;i < m_vCheckCtrlID.size(); i++)
		{
			int ichecked  =  ((CButton*)GetDlgItem(m_vCheckCtrlID[i]))->GetCheck();
			if (ichecked == BST_CHECKED)
			{
				if (!CheckLCLine(i,2))
				{
					((CButton*)GetDlgItem(IDC_CHECK_L13))->SetCheck(BST_UNCHECKED);
				}
			}
		}	// TODO: 在此添加控件通知处理程序代码
	}
}


void CLightMgrDlg::OnBnClickedCheckL14()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_L14))->GetCheck()==BST_CHECKED)
	{

		for (int i = 0;i < m_vCheckCtrlID.size(); i++)
		{
			int ichecked  =  ((CButton*)GetDlgItem(m_vCheckCtrlID[i]))->GetCheck();
			if (ichecked == BST_CHECKED)
			{
				if (!CheckLCLine(i,3))
				{
					((CButton*)GetDlgItem(IDC_CHECK_L14))->SetCheck(BST_UNCHECKED);
				}
			}
		}	// TODO: 在此添加控件通知处理程序代码
	}
}


void CLightMgrDlg::OnBnClickedCheckL21()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_L21))->GetCheck()==BST_CHECKED)
	{
		bool bFirst = true;
		for (int i = 0;i < m_vCheckCtrlID.size(); i++)
		{
			int ichecked  =  ((CButton*)GetDlgItem(m_vCheckCtrlID[i]))->GetCheck();
			if (ichecked == BST_CHECKED)
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					if (!CheckLCLine(i,0))
					{
						((CButton*)GetDlgItem(IDC_CHECK_L21))->SetCheck(BST_UNCHECKED);
					}
				}
		
			}
		}	// TODO: 在此添加控件通知处理程序代码
	}
}


void CLightMgrDlg::OnBnClickedCheckL22()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_L22))->GetCheck()==BST_CHECKED)
	{
		bool bFirst = true;
		for (int i = 0;i < m_vCheckCtrlID.size(); i++)
		{
			int ichecked  =  ((CButton*)GetDlgItem(m_vCheckCtrlID[i]))->GetCheck();
			if (ichecked == BST_CHECKED)
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					if (!CheckLCLine(i,1))
					{
						((CButton*)GetDlgItem(IDC_CHECK_L22))->SetCheck(BST_UNCHECKED);
					}
				}

			}
		}	// TODO: 在此添加控件通知处理程序代码
	}
}


void CLightMgrDlg::OnBnClickedCheckL23()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_L23))->GetCheck()==BST_CHECKED)
	{
		bool bFirst = true;
		for (int i = 0;i < m_vCheckCtrlID.size(); i++)
		{
			int ichecked  =  ((CButton*)GetDlgItem(m_vCheckCtrlID[i]))->GetCheck();
			if (ichecked == BST_CHECKED)
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					if (!CheckLCLine(i,2))
					{
						((CButton*)GetDlgItem(IDC_CHECK_L23))->SetCheck(BST_UNCHECKED);
					}
				}

			}
		}	// TODO: 在此添加控件通知处理程序代码
	}
}


void CLightMgrDlg::OnBnClickedCheckL24()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_L24))->GetCheck()==BST_CHECKED)
	{
		bool bFirst = true;
		for (int i = 0;i < m_vCheckCtrlID.size(); i++)
		{
			int ichecked  =  ((CButton*)GetDlgItem(m_vCheckCtrlID[i]))->GetCheck();
			if (ichecked == BST_CHECKED)
			{
				if (bFirst)
				{
					bFirst = false;
				}
				else
				{
					if (!CheckLCLine(i,3))
					{
						((CButton*)GetDlgItem(IDC_CHECK_L24))->SetCheck(BST_UNCHECKED);
					}
				}

			}
		}	// TODO: 在此添加控件通知处理程序代码
	}
}

