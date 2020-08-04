#include "stdafx.h"
#include "SMVS.h"
#include "CopyTaskDlg.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CCopyTaskDlg, CDialogEx)

CCopyTaskDlg::CCopyTaskDlg(int nChCount, int nCrtChIndex, CString sSrcTaskName, const set<wstring>& setTaskName, std::map<int, wstring>& dictTarget,CWnd* pParent /*=NULL*/)
	: CDialogEx(CCopyTaskDlg::IDD, pParent),
	  TIMER_ID_TASK_DLG(106666),
	  m_nChCount(nChCount),
	  m_nCrtChIndex(nCrtChIndex),
	  m_bOpRslt(true),
	  m_sSrcTaskName(sSrcTaskName)
	  ,m_setTaskName(setTaskName)
	  ,m_dictTarget(dictTarget)
	  , m_bCh1Checked(FALSE)
	  , m_bCh2Checked(FALSE)
	  , m_bCh3Checked(FALSE)
	  , m_bCh4Checked(FALSE)
	  , m_bCh5Checked(FALSE)
	  , m_bCh6Checked(FALSE)
	  , m_bCh7Checked(FALSE)
	  , m_bCh8Checked(FALSE)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CCopyTaskDlg::~CCopyTaskDlg()
{
}

void CCopyTaskDlg::InitUiWidget(int nCtrlID, BOOL bVisible, BOOL nEnable)
{
}

BOOL CCopyTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_FROM);
	pStatic->SetFont(&m_textFont);

	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_TO);
	pStatic->SetFont(&m_textFont);

	pStatic = (CStatic*)GetDlgItem(IDC_STATIC_SRC_CH);
	pStatic->SetFont(&m_textFont);
	CString sCh;
	sCh.Format(_T("CH%d"),m_nCrtChIndex); 
	pStatic->SetWindowText(sCh);

	CEdit* pEditSrc = (CEdit*)GetDlgItem(IDC_EDIT_SRC_TASK_NAME);
	pEditSrc->SetFont(&m_textFont);
	pEditSrc->SetWindowText(m_sSrcTaskName);
	pEditSrc->EnableWindow(FALSE);

	std::map<CWnd*, CWnd*> tmpPair;
	CWnd* pCheck = (CWnd*)GetDlgItem(IDC_CHECK1);
	CWnd* pEdit  = (CWnd*)GetDlgItem(IDC_EDIT_CH1_NAME);
	tmpPair[pCheck] = pEdit;
	m_dictCtrlPtr[1] = tmpPair;

	tmpPair.clear();
	pCheck = (CWnd*)GetDlgItem(IDC_CHECK2);
	pEdit  = (CWnd*)GetDlgItem(IDC_EDIT_CH2_NAME);
	tmpPair[pCheck] = pEdit;
	m_dictCtrlPtr[2] = tmpPair;

	tmpPair.clear();
	pCheck = (CWnd*)GetDlgItem(IDC_CHECK3);
	pEdit  = (CWnd*)GetDlgItem(IDC_EDIT_CH3_NAME);
	tmpPair[pCheck] = pEdit;
	m_dictCtrlPtr[3] = tmpPair;

	tmpPair.clear();
	pCheck = (CWnd*)GetDlgItem(IDC_CHECK4);
	pEdit  = (CWnd*)GetDlgItem(IDC_EDIT_CH4_NAME);
	tmpPair[pCheck] = pEdit;
	m_dictCtrlPtr[4] = tmpPair;

	tmpPair.clear();
	pCheck = (CWnd*)GetDlgItem(IDC_CHECK5);
	pEdit  = (CWnd*)GetDlgItem(IDC_EDIT_CH5_NAME);
	tmpPair[pCheck] = pEdit;
	m_dictCtrlPtr[5] = tmpPair;

	tmpPair.clear();
	pCheck = (CWnd*)GetDlgItem(IDC_CHECK6);
	pEdit  = (CWnd*)GetDlgItem(IDC_EDIT_CH6_NAME);
	tmpPair[pCheck] = pEdit;
	m_dictCtrlPtr[6] = tmpPair;

	tmpPair.clear();
	pCheck = (CWnd*)GetDlgItem(IDC_CHECK7);
	pEdit  = (CWnd*)GetDlgItem(IDC_EDIT_CH7_NAME);
	tmpPair[pCheck] = pEdit;
	m_dictCtrlPtr[7] = tmpPair;

	tmpPair.clear();
	pCheck = (CWnd*)GetDlgItem(IDC_CHECK8);
	pEdit  = (CWnd*)GetDlgItem(IDC_EDIT_CH8_NAME);
	tmpPair[pCheck] = pEdit;
	m_dictCtrlPtr[8] = tmpPair;

	for(int i = 1; i <= m_nChCount; i++)
	{
		if(m_dictCtrlPtr.find(i) != m_dictCtrlPtr.end())
		{
			bool bCameraRun = ::gSystemMgr.IsChannelCameraRunning(i);
			auto dictCtrls = m_dictCtrlPtr[i];
			CWnd* pCheck = dictCtrls.begin()->first;
			CWnd* pEdit  = dictCtrls.begin()->second;
			pCheck->SetFont(&m_textFont);
			pCheck->ShowWindow(SW_SHOW);
			pCheck->EnableWindow(!bCameraRun);
			pEdit->SetFont(&m_textFont);
			pEdit->ShowWindow(SW_SHOW);
			pEdit->EnableWindow(!bCameraRun);
		}
	}

	CStatic* pPanel = (CStatic*)GetDlgItem(IDC_STATIC_WARNING);
	pPanel->SetFont(&m_textFont);

	CButton* pBtn = (CButton*)GetDlgItem(IDOK);
	pBtn->SetFont(&m_textFont);
	pBtn = (CButton*)GetDlgItem(IDCANCEL);
	pBtn->SetFont(&m_textFont);

	return TRUE;
}

void CCopyTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bCh1Checked);
	DDX_Check(pDX, IDC_CHECK2, m_bCh2Checked);
	DDX_Check(pDX, IDC_CHECK3, m_bCh3Checked);
	DDX_Check(pDX, IDC_CHECK4, m_bCh4Checked);
	DDX_Check(pDX, IDC_CHECK5, m_bCh5Checked);
	DDX_Check(pDX, IDC_CHECK6, m_bCh6Checked);
	DDX_Check(pDX, IDC_CHECK7, m_bCh7Checked);
	DDX_Check(pDX, IDC_CHECK8, m_bCh8Checked);
}

BEGIN_MESSAGE_MAP(CCopyTaskDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CCopyTaskDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CCopyTaskDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CCopyTaskDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CCopyTaskDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CCopyTaskDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CCopyTaskDlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CCopyTaskDlg::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &CCopyTaskDlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDOK, &CCopyTaskDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCopyTaskDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CCopyTaskDlg::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CH1_NAME);
	pEdit->EnableWindow(m_bCh1Checked);
	UpdateData(FALSE);
}

void CCopyTaskDlg::OnBnClickedCheck2()
{
	UpdateData(TRUE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CH2_NAME);
	pEdit->EnableWindow(m_bCh2Checked);
	UpdateData(FALSE);
}

void CCopyTaskDlg::OnBnClickedCheck3()
{
	UpdateData(TRUE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CH3_NAME);
	pEdit->EnableWindow(m_bCh3Checked);
	UpdateData(FALSE);
}

void CCopyTaskDlg::OnBnClickedCheck4()
{
	UpdateData(TRUE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CH4_NAME);
	pEdit->EnableWindow(m_bCh4Checked);
	UpdateData(FALSE);
}

void CCopyTaskDlg::OnBnClickedCheck5()
{
	UpdateData(TRUE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CH5_NAME);
	pEdit->EnableWindow(m_bCh5Checked);
	UpdateData(FALSE);
}

void CCopyTaskDlg::OnBnClickedCheck6()
{
	UpdateData(TRUE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CH6_NAME);
	pEdit->EnableWindow(m_bCh6Checked);
	UpdateData(FALSE);
}

void CCopyTaskDlg::OnBnClickedCheck7()
{
	UpdateData(TRUE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CH7_NAME);
	pEdit->EnableWindow(m_bCh7Checked);
	UpdateData(FALSE);
}

void CCopyTaskDlg::OnBnClickedCheck8()
{
	UpdateData(TRUE);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CH8_NAME);
	pEdit->EnableWindow(m_bCh8Checked);
	UpdateData(FALSE);
}

bool CCopyTaskDlg::CollectNewTaskName(int nCtrlID, int nIndex)
{
	CString sNewTaskName, sMsg;

	GetDlgItemTextW(nCtrlID,sNewTaskName);
	sNewTaskName.Trim();

	if(sNewTaskName.GetLength() > 0)
	{
		wstring wstrTaskName = CT2W(sNewTaskName);
		wstring wstrTmp = wstrTaskName;
		boost::algorithm::to_lower(wstrTmp);
		if(!::gConfigMgr.CheckTaskNameUniqueness(wstrTmp))
		{
			sMsg.LoadString(IDS_STRING_WARNING_TASK_NAME_INUSED);
			sMsg.Replace(_T("#1"),sNewTaskName);
			DisplayPromptMessage(sMsg, false);
			return false;
		}
		if(utils::CheckSpecialCharacter(wstrTaskName))
		{
			sMsg.LoadString(IDS_STRING_SPECIAL_CHAR);
			DisplayPromptMessage(sMsg, false);
			return false;
		}
		if(!m_dictTarget.empty())
		{
			for(auto it = m_dictTarget.begin(); it != m_dictTarget.end(); it++)
			{
				wstring wstr = it->second;
				boost::algorithm::to_lower(wstr);
				if(wstrTmp.compare(wstr) == 0)
				{
					sMsg.LoadString(IDS_STRING_WARNING_TASK_NAME_INUSED);
					sMsg.Replace(_T("#1"),sNewTaskName);
					DisplayPromptMessage(sMsg, false);
					return false;
				}
			}
		}
		m_dictTarget[nIndex] = wstrTaskName;
	}
	else
	{
		sMsg.LoadString(IDS_STRING_TASK_NAME_CANNOT_BE_EMPTY);
		DisplayPromptMessage(sMsg, false);
		return false;
	}

	return true;
}

void CCopyTaskDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	bool bOk = true;
	if(m_bCh1Checked)
	{
		bOk = CollectNewTaskName(IDC_EDIT_CH1_NAME, 1);
		if(!bOk)
			return;
	}
	if(m_bCh2Checked)
	{
		bOk = CollectNewTaskName(IDC_EDIT_CH2_NAME, 2);
		if(!bOk)
			return;
	}
	if(m_bCh3Checked)
	{
		bOk = CollectNewTaskName(IDC_EDIT_CH3_NAME, 3);
		if(!bOk)
			return;
	}
	if(m_bCh4Checked)
	{
		bOk = CollectNewTaskName(IDC_EDIT_CH4_NAME,4);
	}
	if(m_bCh5Checked)
	{
		bOk = CollectNewTaskName(IDC_EDIT_CH5_NAME,5);
		if(!bOk)
			return;
	}
	if(m_bCh6Checked)
	{
		bOk = CollectNewTaskName(IDC_EDIT_CH6_NAME,6);
		if(!bOk)
			return;
	}
	if(m_bCh7Checked)
	{
		bOk = CollectNewTaskName(IDC_EDIT_CH7_NAME,7);
		if(!bOk)
			return;
	}
	if(m_bCh8Checked)
	{
		bOk = CollectNewTaskName(IDC_EDIT_CH8_NAME,8);
		if(!bOk)
			return;
	}
	CDialogEx::OnOK();
}

void CCopyTaskDlg::OnBnClickedCancel()
{
	m_dictTarget.clear();
	CDialogEx::OnCancel();
}

HBRUSH CCopyTaskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_SRC_CH ||
	   id == IDC_CHECK1 ||
	   id == IDC_CHECK2 ||
	   id == IDC_CHECK3 ||
	   id == IDC_CHECK4 ||
	   id == IDC_CHECK5 ||
	   id == IDC_CHECK6 ||
	   id == IDC_CHECK7 ||
	   id == IDC_CHECK8 ||
	   id == IDC_STATIC_WARNING)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		if(id == IDC_STATIC_WARNING)
		{
			if(m_bOpRslt)
				pDC->SetTextColor(RGB(50,200,20));
			else
				pDC->SetTextColor(RGB(200,20,20));
		}
		pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
		ReleaseDC(dc);
		return HBRUSH(GetStockObject(NULL_BRUSH));
	}
	return hbr;
}

void CCopyTaskDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	SetDlgItemTextW(IDC_STATIC_WARNING, _T(""));
	RedrawWindow();
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_WARNING, sMsg);
	SetTimer(TIMER_ID_TASK_DLG, 8000, NULL);
}

void CCopyTaskDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(TIMER_ID_TASK_DLG == nIDEvent)
	{
		SetDlgItemTextW(IDC_STATIC_WARNING, _T(""));
		RedrawWindow();
		KillTimer(TIMER_ID_TASK_DLG);
	}
	CDialogEx::OnTimer(nIDEvent);
}
