#include "stdafx.h"
#include "SMVS.h"
#include "ChannelEventMgrDlg.h"
#include "afxdialogex.h"
#include "SystemManager.h"
#include "ConfigurationManager.h"
#include "SMVSDlg.h"
#include "WarningError.h"
#include "ChannelControlPanel.h"

extern CSystemManager gSystemMgr;
extern ConfigManager  gConfigMgr;

IMPLEMENT_DYNAMIC(CChannelEventMgrDlg, CDialogEx)

CChannelEventMgrDlg::CChannelEventMgrDlg(int nChIndex,CString& enableFlagStr,std::vector<ERROR_INFO>& vChError,CWnd* pParent)
	:m_nChIndex(nChIndex),
    m_enableFlagStr(enableFlagStr),
    m_vChError(vChError),
    m_parentWnd(pParent),
    CDialogEx(CChannelEventMgrDlg::IDD, pParent)
{
	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	float rate = rect.Height() /1080.0f;
	if(rect.Height() > 900)
		rate = 900 / 1080.0f;
	m_titleFontSz = (int)(64 * rate);
	m_titleFont.CreateFont(m_titleFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_txtFontSz = (int)(24 * rate);
	m_txtFont.CreateFont(m_txtFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
	m_txtLabelFont.CreateFont(14 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));


	m_titleBgColor = RGB(230,243,246);
	m_btnBgColor = RGB(29,106,222);
	m_ErrorDColor = RGB(255,0,0);

	m_pCurTestBtn = NULL;
	m_bSystemFlag = false;
}

CChannelEventMgrDlg::~CChannelEventMgrDlg()
{
	for (auto it = m_BtnVecSys.begin() ; it !=  m_BtnVecSys.end() ; it++)
	{
		CWinXPButtonST *p =  it->second;
		delete p;
		p= NULL;
	}
	m_BtnVecSys.clear();

	for (auto it = m_BtnVecCH.begin() ; it !=  m_BtnVecCH.end() ; it++)
	{
		CWinXPButtonST *p =  it->second;
		delete p;
		p= NULL;
	}
	m_BtnVecCH.clear();

	for (auto it = m_BtnvecCHOp.begin() ; it !=  m_BtnvecCHOp.end() ; it++)
	{
		CButtonST *p =  it->second;
		delete p;
		p= NULL;
	}
	m_BtnvecCHOp.clear();

	for (auto it = m_BtnvecCHCheck.begin() ; it !=  m_BtnvecCHCheck.end() ; it++)
	{
		CButton *p =  it->second;
		delete p;
		p= NULL;
	}
	m_BtnvecCHCheck.clear();

	for (auto it = m_BtnvecCHDetail.begin() ; it !=  m_BtnvecCHDetail.end() ; it++)
	{
		CLabelCtrl *p =  it->second;
		delete p;
		p= NULL;
	}
	m_BtnvecCHDetail.clear();
	
	for (auto it = m_BtnvecCHTitle.begin() ; it !=  m_BtnvecCHTitle.end() ; it++)
	{
		CLabelCtrl *p =  it->second;
		delete p;
		p= NULL;
	}
	m_BtnvecCHTitle.clear();
}

void CChannelEventMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GP_PCI, m_StaGpPCI);
	DDX_Control(pDX, IDC_STATIC_GP_LIGHT, m_StaGpLight);
	DDX_Control(pDX, IDC_STATIC_GP_Camera, m_StaGpCamera);
	DDX_Control(pDX, IDC_STATIC_GP_Pai, m_StaGpPai);
	DDX_Control(pDX, IDC_STATIC_GP_PaiThreshold, m_StaPaiThreshold);
	DDX_Control(pDX, IDC_STATIC_GP_CDP, m_StaGpCdp);
	DDX_Control(pDX, IDC_STATIC_GP_PAI_COMMU, m_GpPaiCommu);
	DDX_Control(pDX, IDC_STATIC_GP_PAIERROR, m_GpPaiError);
}

BEGIN_MESSAGE_MAP(CChannelEventMgrDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(NEW_SYS_DLG_CLOSE_BTN,	&CChannelEventMgrDlg::OnCloseScreen)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STATIC_GP_PCI, &CChannelEventMgrDlg::OnBnClickedStaticGpPci)
	ON_COMMAND_RANGE(61100,61100+INTERLOCK_CLEAR,OnWarningOpButtonClick)
	ON_COMMAND_RANGE(61200,61200+5,OnWarningCheckButtonClick)  //启用与否对话框
END_MESSAGE_MAP()


BOOL CChannelEventMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundImage(IDB_BITMAP_NEW_SYS_DLG_BG);
	CBitmap bakImg;
	if(bakImg.LoadBitmap(IDB_BITMAP_NEW_SYS_DLG_BG))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		MoveWindow(0,0,bitmap.bmWidth ,bitmap.bmHeight-200);
		CenterWindow();
	}
	m_font.CreateFont(22 ,0, 0,	0,FW_NORMAL,FALSE, FALSE,0,ANSI_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("Arial"));
	SetBackgroundColor(RGB(200,220,250));
	CRect Dcrt;
	GetClientRect(&Dcrt);
	CRect rc;
	rc.left = Dcrt.right - 5 - 32;
	rc.top = Dcrt.top + 5;
	rc.right = rc.left + 32;
	rc.bottom = rc.top + 32;
	m_btnClose.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rc, this, NEW_SYS_DLG_CLOSE_BTN);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();

	rc.left = Dcrt.Width()/2-100;
	rc.top = Dcrt.top + 10;
	rc.right = rc.left + 200;
	rc.bottom = rc.top + 50;

	CString strTitle;
	strTitle.LoadString(IDS_STRING_SYS_ERROR);
	m_lblTitle.Create(strTitle, WS_CHILD|WS_VISIBLE, rc, this, NULL);
	m_lblTitle.SetTxtFont(m_titleFontSz, FW_BOLD);
	m_lblTitle.SetColor(m_btnBgColor, m_titleBgColor);
	m_lblTitle.Invalidate();
	
	int iBeginTop = 80;

	CRect xGpcrt;
	xGpcrt.left = 20;
	xGpcrt.right = Dcrt.right/2 -10;
	xGpcrt.top =  iBeginTop;
	xGpcrt.bottom = xGpcrt.top + 60 + 50;
	m_StaGpPCI.MoveWindow(xGpcrt);

	xGpcrt.top =  xGpcrt.bottom + 20;
	xGpcrt.bottom = xGpcrt.top + 60 + 50;
	m_StaGpLight.MoveWindow(xGpcrt);


	xGpcrt.top =  xGpcrt.bottom + 20;
	xGpcrt.bottom = xGpcrt.top + 60 + 50;
	m_StaGpCamera.MoveWindow(xGpcrt);

	xGpcrt.left = xGpcrt.right + 20;
 	xGpcrt.right = Dcrt.right -10;
 	xGpcrt.top =  iBeginTop;
 	m_GpPaiCommu.ShowWindow(SW_HIDE);

	xGpcrt.bottom = xGpcrt.top + 60 + 50;
	m_StaGpPai.MoveWindow(xGpcrt);

	xGpcrt.top =  xGpcrt.bottom + 20;
	xGpcrt.bottom = xGpcrt.top + 60 + 50;
	m_StaPaiThreshold.MoveWindow(xGpcrt);

	xGpcrt.top =  xGpcrt.bottom + 20;
	xGpcrt.bottom = xGpcrt.top + 60 + 50;
	m_StaGpCdp.MoveWindow(xGpcrt);

	xGpcrt.top =  xGpcrt.bottom + 20;
	xGpcrt.bottom = xGpcrt.top + 60 +60 *2;
	m_GpPaiError.ShowWindow(SW_HIDE);

	rc.left   = Dcrt.left + 5;
	rc.top    = Dcrt.top + 5;
	rc.right  = rc.left + 80;
	rc.bottom = rc.top + m_titleFontSz - 10;

	CString strCHname;
	strCHname.Format(_T("CH%d"),m_nChIndex);
	m_StaChLabel.Create(strCHname, WS_CHILD|WS_VISIBLE, rc, this, NULL);
	m_StaChLabel.SetTxtFont(m_titleFontSz - 10, FW_BOLD);
	m_StaChLabel.SetColor(m_btnBgColor, m_titleBgColor);
	m_StaChLabel.Invalidate();
	m_StaChLabel.MoveWindow(&rc);
	
	CRect crt;
	crt.bottom = iBeginTop- 25-50;
	crt.left =   50;
	crt.right =  Dcrt.right - Dcrt.Width()/2 -105;
	UAC uac = ::gSystemMgr.GetCurrentUser();
	bool bAdmin = (uac.m_nLevel == UAC_ADMIN);
	for (int i = 0 ; i< m_Errorinfo.m_CHErrorVec.size() ; i++)
	{
		if (i == 3)
		{
			crt.left = crt.right +5+150;
			crt.right = Dcrt.right - 5-100;
			crt.top = iBeginTop- 25 + 40;
		}
		else 
		{
			crt.top = crt.bottom + 40+50;
		}
		crt.bottom = crt.top + 40;

		CRect CrtA;
		CrtA.left = crt.left;
		CrtA.right=CrtA.left+50;
		CrtA.top= crt.top;
		CrtA.bottom=crt.bottom;

		CRect crtD;
		crtD.left = CrtA.left;
		crtD.right = crt.right+85;
		crtD.top = CrtA.bottom +5;
		crtD.bottom = crtD.top+60;//40;
		CLabelCtrl *pd = new CLabelCtrl();
		pd->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY|BS_LEFTTEXT, crtD, this, 61300+i);//
		pd->SetFont(&m_txtLabelFont);
		CWinXPButtonST *p  = new CWinXPButtonST();
		p->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY|BS_LEFTTEXT,CrtA, this, 61000+i);
		p->SetFont(&m_txtFont);
		p->SetStaticButton(true);
		p->ShowWindow(bAdmin?SW_SHOW:SW_HIDE);
	    p->DrawTransparent(TRUE);

		CRect crtE;
		crtE.left = CrtA.right;
		crtE.right = crt.right-100;
		crtE.top = CrtA.top+10;
		crtE.bottom = CrtA.bottom;
		CLabelCtrl  *pe  = new CLabelCtrl();
		pe->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY|BS_LEFTTEXT, crtE, this, 61500+i);//
		pe->SetFont(&m_txtFont);
		pe->SetWindowText(m_Errorinfo.GetErrorInfo(m_Errorinfo.m_CHErrorVec[i]));

		CRect crtButton(crt);
		crtButton.top = crtButton.top +5;
		crtButton.bottom = crtButton.top + 30;
		crtButton.left = crtButton.right ;
		crtButton.right = crtButton.left + 85;
		
		if(i==3)
		{
			crtButton.left = crtButton.left-70 ;
			crtButton.right = crtButton.right-60 ;
			CShadeButtonST *pInterLock = new CShadeButtonST();			
			pInterLock->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY|BS_LEFTTEXT|WS_EX_DLGMODALFRAME ,crtButton, this, 61100+INTERLOCK_CLEAR);
			CString str;
			str.LoadString(IDS_STRING_CLEAR_INTERLOCK);
			pInterLock->SetWindowText(str);
			pInterLock->SetShade(CShadeButtonST::SHS_METAL);
			pInterLock->SetFont(&m_txtFont);
			pInterLock->ShowWindow(bAdmin?SW_SHOW:SW_HIDE);
			//crtButton.top = crtButton.bottom +5;
			//crtButton.bottom = crtButton.top+30;
			crtButton.left = crtButton.left+100 ;
			crtButton.right = crtButton.left+60 ;

			if(gSystemMgr.GetCurrentUser().m_nLevel != UAC_ADMIN)// || ((CChannelControlPanel*)m_parentWnd)->GetIsInterlock()==FALSE)
			{
				//pInterLock->ShowWindow(SW_HIDE);
			}
			m_BtnvecCHOp[m_Errorinfo.m_CHErrorVec.size()+1]=pInterLock;

		}
		CShadeButtonST *pb = new CShadeButtonST();
		pb->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY|BS_LEFTTEXT|WS_EX_DLGMODALFRAME ,crtButton, this, 61100+i);
		if (m_Errorinfo.m_CHErrorVec[i] == LIGHT_CONTRL_CONNECT_ERROR||
			m_Errorinfo.m_CHErrorVec[i] == CAMERA_CONNECT_ERROR)
		{
			CString str;
			str.LoadString(IDS_STRING_REFRESH);
			pb->SetWindowText(str);
		}
		else
		{
			CString str;
			str.LoadString(IDS_STRING_Clear);
			pb->SetWindowText(str);
		}
		pb->SetShade(CShadeButtonST::SHS_METAL);
		pb->SetFont(&m_txtFont);
		pb->ShowWindow(bAdmin?SW_SHOW:SW_HIDE);
		if (i>2)
		{
			crtButton.left = 30 +Dcrt.Width()/2  ;
		}
		else
		{
			crtButton.left = 30;
		}
		crtButton.right = crtButton.left+ 14;
		crtButton.top = crtButton.top +12;
		crtButton.bottom = crtButton.top +14;
		CButton  *pc = new CButton();
		pc->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY|BS_CHECKBOX  ,crtButton, this, 61200+i);

		if (!m_enableFlagStr.IsEmpty())
		{
			if (m_enableFlagStr.Mid(i,1) == _T("1"))
			{
				p->SetIcon(IDI_ICON_GREEN);
				pc->SetCheck(BST_CHECKED);
			}
			else
			{
				p->SetIcon(IDI_ICON_GRAY);
				pc->SetCheck(BST_UNCHECKED);
			}
		}

		UAC crtUser = gSystemMgr.GetCurrentUser();
		if(crtUser.m_nLevel != UAC_ADMIN)
		{
			pb->ShowWindow(SW_HIDE);
			pc->ShowWindow(SW_HIDE);
		}
		else
		{
			if (i == 0 || i== 1 ||i == 2 ||i == 3)
			{
				pc->ShowWindow(SW_HIDE);
				if (i == 0)
				{
					pb->ShowWindow(SW_HIDE);
				}
			}
			else
			{
				pb->ShowWindow(SW_SHOW);
				pc->ShowWindow(SW_SHOW);
			}
		}

		m_BtnVecCH[m_Errorinfo.m_CHErrorVec[i]] = p;
		m_BtnvecCHOp[m_Errorinfo.m_CHErrorVec[i]] = pb;
		m_BtnvecCHCheck[m_Errorinfo.m_CHErrorVec[i]]= pc;
		m_BtnvecCHDetail[m_Errorinfo.m_CHErrorVec[i]]= pd;
		m_BtnvecCHTitle[m_Errorinfo.m_CHErrorVec[i]]= pe;
	}
	InitCtrl();
	return TRUE; 
}

void CChannelEventMgrDlg::OnCloseScreen()
{
	CDialogEx::OnOK();
}

void CChannelEventMgrDlg::InitCtrl()
{

	for (auto it = m_BtnvecCHOp.begin() ; it !=  m_BtnvecCHOp.end(); it++)
	{
		CButton *pb = it->second;
		pb->EnableWindow(TRUE);
	}

	vector<int> m_vecFlag;

	for (int i = 0 ; i< m_vChError.size() ; i++)
	{
		bool btrue = false;
		for (auto it = m_vecFlag.begin();it != m_vecFlag.end();it++)
		{
			if((*it) == m_vChError[i].m_errorID)
			{
				btrue = true;
				break;
			}
		}

		if (btrue)
		{
			continue;
		}

		if (m_BtnVecCH.find(m_vChError[i].m_errorID) != m_BtnVecCH.end())
		{
			m_vecFlag.push_back(m_vChError[i].m_errorID);

			CWinXPButtonST *p =  m_BtnVecCH[m_vChError[i].m_errorID];
			CLabelCtrl *pd =  m_BtnvecCHDetail[m_vChError[i].m_errorID];
			CShadeButtonST *pb = m_BtnvecCHOp[m_vChError[i].m_errorID];
			int iErrorqueue = (m_vChError[i].m_errorID - PCI_CREATE_FAILED_ERROR);
			if (m_enableFlagStr.Mid(iErrorqueue,1) == _T("1"))
			{
				p->SetIcon(IDI_ICON_ERROR);
				
				if (m_vChError[i].m_strMsg != _T(""))
				{
					CString str;
					str.Format(_T("[%d]%s"),m_vChError[i].m_Counter,m_vChError[i].m_strMsg.c_str());
					if(str.Find('\n')>0)
						pd->SetFont(&m_txtLabelFont);
					else
						pd->SetFont(&m_txtFont);
					pd->SetWindowText(str);
					pd->Invalidate();
				
					if(m_vChError[i].m_errorID == CAMERA_CONNECT_ERROR)
					{	
						CString str;
						str.LoadString(IDS_STRING_REFRESH);
						pb->SetWindowText(str);
						if (pb->IsWindowVisible())
						{
							pb->ShowWindow(SW_HIDE);
							pb->ShowWindow(SW_SHOW);
						}
					}
				}
			}

		}
	}
	
}

void CChannelEventMgrDlg::OnWarningCheckButtonClick(UINT uID)
{
	int i = uID - 61200;
	int ErrorID = PCI_CREATE_FAILED_ERROR + i;
	CButton *p =  m_BtnvecCHCheck[ErrorID];

	if (ErrorID== PCI_CREATE_FAILED_ERROR||ErrorID ==INIT_REJECTOR_SYS_FAILED)
	{
		m_bSystemFlag = true;
	}

	CString  ch = m_enableFlagStr.Mid(i,1);
	if(ch == _T("1"))
	{
		m_BtnVecCH[ErrorID]->SetIcon(IDI_ICON_GRAY);
		p->SetCheck(BST_UNCHECKED);
		m_enableFlagStr.SetAt(i,'0');
	}
	else
	{
		for (int j = 0 ; j< m_vChError.size() ; j++)
		{
			if(m_vChError[j].m_errorID == ErrorID)
			{
				m_BtnVecCH[ErrorID]->SetIcon(IDI_ICON_ERROR);
				p->SetCheck(BST_CHECKED);
				m_enableFlagStr.SetAt(i,'1');
				return;
			}
		}
				
		m_BtnVecCH[ErrorID]->SetIcon(IDI_ICON_GREEN);
		p->SetCheck(BST_CHECKED);
		m_enableFlagStr.SetAt(i,'1');
		return;
	}
}

void CChannelEventMgrDlg::OnWarningOpButtonClick(UINT uID)
{
	int i = uID - 61100;
	int ErrorID = PCI_CREATE_FAILED_ERROR + i;	

	if (ErrorID == PCI_CREATE_FAILED_ERROR)
	{
		OnBnClickedStaticGpPci();
	}	
	else if (ErrorID == LIGHT_CONTRL_CONNECT_ERROR)
	{
		HandleLightControllerErrorEvent();
	}
	else if (ErrorID == CAMERA_CONNECT_ERROR)
	{
		HandleCameraErrorEvent();
	}
	else if (ErrorID == INIT_REJECTOR_SYS_FAILED)
	{
		HandleRejectorCardErrorEvent();
	}
	else if (ErrorID == THRESHOLD_BLOW_ERROR)
	{
		HandleThresholdValueErrorEvent();
	}
	else if (ErrorID == CDP_ERROR)
	{
		HandleContigousDefectErrorEvent();
	}	
	else //if (ErrorID == INTERLOCK_CLEAR)
	{
		//clear interlock
		HandleClearInterlock();
	}
}

BOOL CChannelEventMgrDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CChannelEventMgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_GP_PCI||
		id == IDC_STATIC_GP_LIGHT||
		id == IDC_STATIC_GP_Camera||
		id == IDC_STATIC_GP_PaiThreshold||
		id == IDC_STATIC_GP_CDP||
		id == IDC_STATIC_GP_Pai||
		id == IDC_STATIC_GP_PAIERROR||
		id == IDC_STATIC_GP_PAI_COMMU|| 
		(id >= 61300 && id < 61400)||
 		(id >= 61500 && id < 61600))
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		if ((id >= 61300 && id < 61400))
		{
			pDC->SetTextColor(RGB(255,0,0));
		}

		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}

	return hbr;
}

void CChannelEventMgrDlg::ReSetCtrl(int iErrorID)
{
	int ii = 0;
	for (auto it = m_BtnVecCH.begin() ; it !=  m_BtnVecCH.end() ; it++)
	{ 
		CWinXPButtonST *p  = it->second;
		int i = it->first;
			if (m_enableFlagStr.Mid(ii,1) == _T("1"))
			{
				if (iErrorID == 0)
				{
					p->SetIcon(IDI_ICON_GREEN);
					m_BtnvecCHDetail[i]->SetWindowText(_T(""));//
					m_BtnvecCHDetail[i]->ShowWindow(SW_HIDE);
					m_BtnvecCHDetail[i]->ShowWindow(SW_SHOW);

				}
				if (iErrorID!=0 &&(it->first ==iErrorID))
				{
					p->SetIcon(IDI_ICON_GREEN);
					m_BtnvecCHDetail[i]->SetWindowText(_T(""));
					m_BtnvecCHDetail[i]->ShowWindow(SW_HIDE);
					m_BtnvecCHDetail[i]->ShowWindow(SW_SHOW);
				}
			}
			else
			{
				p->SetIcon(IDI_ICON_GRAY);
			}
		ii++;
	}
}

void CChannelEventMgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString str;
	str.LoadString(IDS_STRING_REFRESH);
	switch (nIDEvent)
	{
		case  TIMER_LIGHT_CONTROL_TEST:
            {
                m_BtnVecCH[LIGHT_CONTRL_CONNECT_ERROR]->SetIcon(IDI_ICON_GREEN);
                m_BtnvecCHDetail[LIGHT_CONTRL_CONNECT_ERROR]->SetWindowText(_T(""));
                m_BtnvecCHDetail[LIGHT_CONTRL_CONNECT_ERROR]->ShowWindow(SW_HIDE);
                m_BtnvecCHDetail[LIGHT_CONTRL_CONNECT_ERROR]->ShowWindow(SW_SHOW);
                CChannelControlPanel* pChWnd = (CChannelControlPanel*)m_parentWnd;
                pChWnd->VerifyLightControllerTestResult(gSystemMgr.m_setCheckedLcID);
                InitCtrl();
                m_BtnvecCHOp[LIGHT_CONTRL_CONNECT_ERROR]->SetWindowText(str);
                KillTimer(TIMER_LIGHT_CONTROL_TEST);
                break;
            }
		case TIMER_CAMERA_TEST:
			m_BtnVecCH[CAMERA_CONNECT_ERROR]->SetIcon(IDI_ICON_GREEN);
			m_BtnvecCHDetail[CAMERA_CONNECT_ERROR]->SetWindowText(_T(""));
			m_BtnvecCHDetail[CAMERA_CONNECT_ERROR]->ShowWindow(SW_HIDE);
			m_BtnvecCHDetail[CAMERA_CONNECT_ERROR]->ShowWindow(SW_SHOW);

			InitCtrl();
			m_BtnvecCHOp[CAMERA_CONNECT_ERROR]->SetWindowText(str);
			KillTimer(TIMER_CAMERA_TEST);
			break;
		default:
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CChannelEventMgrDlg::OnBnClickedStaticGpPci()
{
	CChannelControlPanel* pChWnd = (CChannelControlPanel*)m_parentWnd;
	pChWnd->RemoveChFirstError(PCI_CREATE_FAILED_ERROR);
	ReSetCtrl();
	InitCtrl();
}

void CChannelEventMgrDlg::HandleLightControllerErrorEvent()
{
	CChannelControlPanel* pChWnd = (CChannelControlPanel*)m_parentWnd;
	pChWnd->RemoveChError(LIGHT_CONTRL_CONNECT_ERROR,ERRORSUBID_LIGHT_CONNECT_FAILED); //com口错误，不删除

	if (m_parentWnd != NULL)
	{
		for (auto it = m_BtnvecCHOp.begin() ; it !=  m_BtnvecCHOp.end(); it++)
		{
			CButton *pb = it->second;
			if (it->first == LIGHT_CONTRL_CONNECT_ERROR)
			{
				CString str;
				str.LoadString(IDS_STRING_TEST);
				pb->SetWindowText(str);
			}
			else
			{
				pb->EnableWindow(false);
			}
		}
		m_BtnvecCHDetail[LIGHT_CONTRL_CONNECT_ERROR]->SetWindowText(_T(""));
		pChWnd->ReTestLightController();
		
		SetTimer(TIMER_LIGHT_CONTROL_TEST,2000,NULL);
	}
}

void CChannelEventMgrDlg::HandleCameraErrorEvent()
{
	CChannelControlPanel* pChWnd = (CChannelControlPanel*)m_parentWnd;
	pChWnd->RemoveAllChError(CAMERA_CONNECT_ERROR);

	if(pChWnd != NULL)
	{	
		for (auto it = m_BtnvecCHOp.begin() ; it !=  m_BtnvecCHOp.end(); it++)
		{
			CButton *pb = it->second;
			if (it->first == CAMERA_CONNECT_ERROR)
			{
				CString str;
				str.LoadString(IDS_STRING_TEST);
				pb->SetWindowText(str);
			}
			else
			{
				pb->EnableWindow(false);
			}
		}
		m_BtnvecCHDetail[CAMERA_CONNECT_ERROR]->SetWindowText(_T(""));
		pChWnd->ReConnectChannelCamera();
		SetTimer(TIMER_CAMERA_TEST,5000,NULL);
	}
}

void CChannelEventMgrDlg::HandleRejectorCardErrorEvent()
{
	CChannelControlPanel* pChWnd = (CChannelControlPanel*)m_parentWnd;
	pChWnd->RemoveChFirstError(INIT_REJECTOR_SYS_FAILED);
	pChWnd->ExecuteCmd(QOP_DATA_RESET,RESET_STATS_ERR);
	ReSetCtrl();
	InitCtrl();
}

void CChannelEventMgrDlg::HandleClearInterlock()
{
	CChannelControlPanel* pChWnd = (CChannelControlPanel*)m_parentWnd;
	pChWnd->RemoveChFirstError(INIT_REJECTOR_SYS_FAILED);
	pChWnd->ExecuteCmd(QOP_UNLOCK_SET,UNLOCK_INTERLOCK);
	ReSetCtrl();
	InitCtrl();
}

void CChannelEventMgrDlg::HandleThresholdValueErrorEvent()
{
	CChannelControlPanel* pChWnd = (CChannelControlPanel*)m_parentWnd;
	pChWnd->RemoveChFirstError(THRESHOLD_BLOW_ERROR);
	ReSetCtrl();
	InitCtrl();
}

void CChannelEventMgrDlg::HandleContigousDefectErrorEvent()
{
	CChannelControlPanel* pChWnd = (CChannelControlPanel*)m_parentWnd;
	pChWnd->RemoveChFirstError(CDP_ERROR);
	ReSetCtrl();
	InitCtrl();
}