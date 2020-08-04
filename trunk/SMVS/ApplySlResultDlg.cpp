#include "stdafx.h"
#include "SMVS.h"
#include "ApplySlResultDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CApplySlResultDlg, CDialogEx)

CApplySlResultDlg::CApplySlResultDlg(int nChIndex, const std::vector<wstring>& vTaskName, wstring& retTaskName,CWnd* pParent /*=NULL*/)
	: CDialogEx(CApplySlResultDlg::IDD, pParent),
	m_nChIndex(nChIndex),
	m_vTaskName(vTaskName),
	m_wstrRetTaskName(retTaskName)
{
	m_textFont.CreateFont(20 ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}

CApplySlResultDlg::~CApplySlResultDlg()
{
}

void CApplySlResultDlg::InitUiWidget(int nCtrlID, int strID)
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

BOOL CApplySlResultDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rcScreen;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcScreen,0);
	CRect rc;
	GetWindowRect(rc);
	int x = (rcScreen.Width() - rc.Width()) / 2 ;
	int y = (rcScreen.Height() - rc.Height()) / 2;
	::SetWindowPos(m_hWnd,HWND_TOP, x ,y, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
	CDialogEx::SetBackgroundImage(IDB_BITMAP_SYS_CHILD_DLG_BG);

	CString sMsg;
	sMsg.Format(_T("CH%d - "), m_nChIndex);
	CString sTmp;
	sTmp.LoadString(IDS_STRING_SELF_LEARNING_COMPLETE);
	sMsg.Append(sTmp);

	InitUiWidget(IDC_STATIC_APPLY_TO,IDS_STRING_APPLY_SL_RESULT_TO);
	InitUiWidget(IDC_COMBO_CH_TASK,NULL);
	InitUiWidget(IDOK,NULL);
	for(size_t t = 0; t < m_vTaskName.size();t++)
	{
		wstring wstrName = m_vTaskName[t];
		m_listTaskName.AddString(wstrName.c_str());
	}
	return TRUE;
}

BOOL CApplySlResultDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CApplySlResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CH_TASK, m_listTaskName);
}


BEGIN_MESSAGE_MAP(CApplySlResultDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CH_TASK, &CApplySlResultDlg::OnCbnSelchangeComboChTask)
END_MESSAGE_MAP()



void CApplySlResultDlg::OnCbnSelchangeComboChTask()
{
	int nIndex = m_listTaskName.GetCurSel();
	if(nIndex != -1)
	{
		CString sTaskName;
		m_listTaskName.GetLBText(nIndex,sTaskName);
		m_wstrRetTaskName = sTaskName.GetString();
	}
}
