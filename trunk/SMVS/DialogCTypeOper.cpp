// DialogCTypeOper.cpp : 实现文件
//

#include "stdafx.h"
#include "SMVS.h"
#include "DialogCTypeOper.h"
#include "afxdialogex.h"


// CDialogCTypeOper 对话框
#define TASKCHANGE_DLG_CLOSE_BTN 61044
IMPLEMENT_DYNAMIC(CDialogCTypeOper, CDialogEx)

CDialogCTypeOper::CDialogCTypeOper(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogCTypeOper::IDD, pParent)
{
	m_bTaskReady = false;
	m_bTaskChanging =false;

	CRect rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
	float rate = rect.Height() /1080.0f;
	m_titleFontSz = (int)(40 * rate);

	m_titleBgColor = RGB(242,250,252);
	m_btnBgColor = RGB(29,106,222);
	m_TaskinfoColor = RGB(0,0,0);

	m_titleFont.CreateFont(m_titleFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_txtFontSz = (int)(20 * rate);
	m_txtFont.CreateFont(m_txtFontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));
}
int CDialogCTypeOper::GetiCapHeight()
{
	return m_iCapHeight;
}
bool CDialogCTypeOper::GetbRollNeck()
{
	return m_bRollNeck;
}
int CDialogCTypeOper::GetExpType()
{
	return m_e_Exptype;
}
int CDialogCTypeOper::GetDiameter()
{
	return m_diameter;
}
void CDialogCTypeOper::SetCurTaskName(CString strname1,CString strname2)
{
	m_Curtaskname1 = strname1;
	m_Curtaskname2 = strname2;
}
CDialogCTypeOper::~CDialogCTypeOper()
{
	
}

void CDialogCTypeOper::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Diameter, m_comDiameter);
	DDX_Control(pDX, IDC_EDIT_CapHeight, m_editCapHeight);
	DDX_Control(pDX, IDC_COMBO_RollNeck, m_comRollneck);
	DDX_Control(pDX, IDC_COMBO_ExpType, m_comExpType);
	DDX_Control(pDX, IDC_STATIC_OldInfo, m_staOldinfo);
	DDX_Control(pDX, IDC_STATIC_NewInfo, m_staNewinfo);
	DDX_Control(pDX, IDC_STATIC_NewTaskInfo, m_staNewTaskInfo);
	DDX_Control(pDX, IDC_STATIC_CapDiameter, m_StaCapDiameter);
	DDX_Control(pDX, IDC_STATIC_CapHeight, m_staCapHeight);
	DDX_Control(pDX, IDC_STATIC_BrollNeck, m_staBrollNeck);
	DDX_Control(pDX, IDC_STATIC_ExpppType, m_staExpppType);
	DDX_Control(pDX, IDC_BUTTON_Qurey, m_btnQUeryTask);
	DDX_Control(pDX, ID_Button_TypeChange, m_btnTypeChange);
	DDX_Control(pDX, IDCANCEL, m_btnCancle);
}


BEGIN_MESSAGE_MAP(CDialogCTypeOper, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_Diameter, &CDialogCTypeOper::OnSelchangeComboDiameter)
	ON_BN_CLICKED(ID_Button_TypeChange, &CDialogCTypeOper::OnBnClickedButtonTypechange)
	ON_BN_CLICKED(IDC_BUTTON_Qurey, &CDialogCTypeOper::OnBnClickedButtonQurey)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(TASKCHANGE_DLG_CLOSE_BTN, &CDialogCTypeOper::OnStnClickedClose)
END_MESSAGE_MAP()


// CDialogCTypeOper 消息处理程序


void CDialogCTypeOper::OnSelchangeComboDiameter()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CDialogCTypeOper::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	m_font.CreateFont(m_txtFontSz ,0, 0,	0,FW_NORMAL,FALSE, FALSE,0,ANSI_CHARSET, OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("Arial"));
	m_staOldinfo.SetFont(&m_txtFont);
	m_staNewinfo.SetFont(&m_txtFont);
	m_staNewTaskInfo.SetFont(&m_txtFont);

	m_StaCapDiameter.SetFont(&m_txtFont);
	str.LoadStringW(IDS_STRING_Cap_diameter);
	m_StaCapDiameter.SetWindowText(str);

	m_staCapHeight.SetFont(&m_txtFont);
	str.LoadStringW(IDS_STRING_Cap_Height);
	m_staCapHeight.SetWindowText(str);

	m_staBrollNeck.SetFont(&m_txtFont);
	str.LoadStringW(IDS_STRING_Cap_bRollneck);
	m_staBrollNeck.SetWindowText(str);

	m_staExpppType.SetFont(&m_txtFont);
	str.LoadStringW(IDS_STRING_Cap_expppType);
	m_staExpppType.SetWindowText(str);

	m_btnQUeryTask.SetFont(&m_txtFont);
	m_btnTypeChange.SetFont(&m_txtFont);
	m_btnCancle.SetFont(&m_txtFont);
	m_comDiameter.SetFont(&m_txtFont);
	m_comExpType.SetFont(&m_txtFont);
	m_comRollneck.SetFont(&m_txtFont);
	m_editCapHeight.SetFont(&m_txtFont);

	SetBackgroundImage(IDB_BITMAP_HELP_DLG_BG);

	CRect rcClient;
	GetClientRect(&rcClient);
	int left = (int)(rcClient.Width() / 8.0);
	int top = 30;
	int right = rcClient.right - 100;
	int bottom = 35;
	CRect rc(left,top,right,bottom);
	CString strTitle;
	strTitle.LoadStringW(IDS_STRING_CapFastType);
	m_dlgTitle.Create(strTitle,WS_CHILD|WS_VISIBLE|SS_NOTIFY,rc,this, NULL);
	m_dlgTitle.SetTxtFont(m_titleFontSz, FW_BOLD);
	m_dlgTitle.SetColor(m_btnBgColor, m_titleBgColor);
	m_dlgTitle.Invalidate();

	CRect rcCloseBtn;
	rcCloseBtn.left = rcClient.right - 5 - 32;
	rcCloseBtn.top = rcClient.top + 5;
	rcCloseBtn.right = rcCloseBtn.left + 32;
	rcCloseBtn.bottom = rcCloseBtn.top + 32;

	m_btnClose.Create(_T(""),WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcCloseBtn, this, TASKCHANGE_DLG_CLOSE_BTN);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();


	// TODO:  在此添加额外的初始化


	m_vecExpppType = gSystemMgr.GetExpppType();

	m_diameter = gSystemMgr.GetCapDiameter();
	CString strdiameter;
	strdiameter.Format(_T("%d"),(int)m_diameter);
	m_comDiameter.AddString(strdiameter);//_T("30")


	str.LoadStringW(IDS_STRING_AYES);
	m_comRollneck.AddString(str);
	str.LoadStringW(IDS_STRING_BNO);
	m_comRollneck.AddString(str);

	for (size_t i=0; i< m_vecExpppType.size();i++)
	{
		m_comExpType.AddString(m_vecExpppType[i]);
	}

	m_staNewinfo.ShowWindow(SW_HIDE);
	m_staNewTaskInfo.ShowWindow(SW_HIDE);

	//滚颈初始
	str = m_Curtaskname1.Left(1);
	if (str == _T("A"))
	{
		m_comRollneck.SetCurSel(0);
	}
	else
	{
		m_comRollneck.SetCurSel(1);
	}

	//直径初始
	str = m_Curtaskname1.Mid(1,2);
	if (str == strdiameter)
	{
		m_comDiameter.SetCurSel(0);
	}

	//
	int iFirst = m_Curtaskname1.Find(_T("_"));
	int iSecond = m_Curtaskname1.Find(_T("_"),iFirst+1);

	str = m_Curtaskname1.Mid(iFirst+1,iSecond-iFirst-1);
	double Capheight;
	Capheight = _tstof(str);
	Light1 = gSystemMgr.GetLightHeight1(Capheight);

	int iThird = m_Curtaskname1.Find(_T("_"),iSecond+1);
	str = m_Curtaskname1.Mid(iSecond+1,iThird-iSecond-1);

	for (size_t i = 0; i < m_vecExpppType.size(); i++)
	{
		if (str == m_vecExpppType[i])
		{
			m_comExpType.SetCurSel(i);
		}
	}

	iFirst = m_Curtaskname2.Find(_T("_"));
	iSecond = m_Curtaskname2.Find(_T("_"),iFirst+1);
	str = m_Curtaskname2.Mid(iFirst+1,iSecond-iFirst-1);
	
	Capheight = _tstof(str);
	Light2 = gSystemMgr.GetLightHeight2(Capheight);
	
	str.LoadStringW(IDS_STRING_OLDTASK_LIGHTINFO);
	CString strOldInfo;
	strOldInfo.Format(str,Light1,Light2);
	m_staOldinfo.SetWindowText(strOldInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialogCTypeOper::OnBnClickedButtonTypechange()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bTaskChanging)
	{
		return;
	}
	CString str;
	if (!m_bTaskReady)
	{
		OnBnClickedButtonQurey();
		if (!m_bTaskReady)
		{	
			str.LoadStringW(IDS_STRING_FINDTASK_FAILED);
			MessageBox(str);
			return;
		}
	}

	m_bTaskChanging = true;
	str.LoadStringW(IDS_STRING_TASK_SWITCHING);
	m_btnTypeChange.SetWindowText(str);
	m_btnTypeChange.RedrawWindow();
	if (m_bTaskReady)
	{
		if (m_vecTaskname.size()== 5)
		{
			gSystemMgr.PanelAutoChangeTask( m_vecTaskname);
		}
		m_bTaskReady =false;
	}

	m_bTaskChanging = false;
	str.LoadStringW(IDS_STRING_TASK_SWITCH);
	m_btnTypeChange.SetWindowText(str);

	str.LoadStringW(IDS_STRING_TASK_SWITCH_OK);
	if (Light1 != NLight1)
	{
		CString info;
		CString str1;
		str1.LoadStringW(IDS_STRING_ADAPT_LIGHT1);
		info.Format(str1,NLight1);
		str += info;
	}
	if (Light2 != NLight2)
	{
		CString info;
		CString str1;
		str1.LoadStringW(IDS_STRING_ADAPT_LIGHT2);
		info.Format(str1,NLight2);
		str += info;
	}
	AfxMessageBox(str);
	m_btnTypeChange.RedrawWindow();
}


void CDialogCTypeOper::OnBnClickedButtonQurey()
{
	// TODO: 在此添加控件通知处理程序代码
	m_staNewTaskInfo.SetWindowText(_T(""));
	m_staNewinfo.SetWindowText(_T(""));


 	CString str;
 	m_comDiameter.GetWindowText(str);
	m_diameter = _ttoi(str);

	m_editCapHeight.GetWindowText(str);
	m_iCapHeight = _tstof(str);

	m_e_Exptype = m_comExpType.GetCurSel();

	int i = m_comRollneck.GetCurSel();
	if ( i== 0)
	{
		m_bRollNeck = true;
	}
	else if ( i== 1)
	{
		m_bRollNeck = false;
	}

	TypeData DataPara;
	CString myErrorInfo;
	bool ret = gSystemMgr.CheckAutoTaskName(m_iCapHeight,m_bRollNeck,m_e_Exptype,m_diameter,DataPara,m_vecTaskname,myErrorInfo);

	CString strTaskInfo;
	if(ret)
	{
		if(m_vecTaskname.size()==5)
		{
			str.LoadStringW(IDS_STRING_TASK_RESULT);
			strTaskInfo.Format(str,m_vecTaskname[0],m_vecTaskname[1],m_vecTaskname[2],m_vecTaskname[3],m_vecTaskname[4]);
			m_TaskinfoColor = RGB(20,20,200);
			
		}
		else
		{
			m_TaskinfoColor = RGB(200,20,20);
		}
	}
	else
	{	
		m_TaskinfoColor = RGB(200,20,20);
		strTaskInfo.LoadStringW(IDS_STRING_TASK_AUOT_FAILED);
		if(!myErrorInfo.IsEmpty())
		{
			strTaskInfo += myErrorInfo;
		}
		else
		{
			str.LoadStringW(IDS_STRING_TASK_NOTEXSIT);
			strTaskInfo+=str;
		}
		m_staNewTaskInfo.SetWindowText(strTaskInfo);
		m_staNewTaskInfo.ShowWindow(SW_HIDE);
		m_staNewTaskInfo.ShowWindow(SW_SHOW);
		m_staNewinfo.ShowWindow(SW_HIDE);
		m_staNewinfo.ShowWindow(SW_SHOW);
		m_bTaskReady = false;
		return;
	}
	m_staNewTaskInfo.SetWindowText(strTaskInfo);
	m_staNewTaskInfo.ShowWindow(SW_HIDE);
	m_staNewTaskInfo.ShowWindow(SW_SHOW);
	m_staNewTaskInfo.RedrawWindow();
	CString strNewInfo;
	str.LoadStringW(IDS_STRING_NEWTASK_LIGHTINFO);
	strNewInfo.Format(str,DataPara.S1HLight,DataPara.S2HLight);
	NLight1 = DataPara.S1HLight;
	NLight2 = DataPara.S2HLight;

	m_staNewinfo.SetWindowText(strNewInfo);
	m_staNewinfo.ShowWindow(SW_HIDE);
	m_staNewinfo.ShowWindow(SW_SHOW);
	m_staNewinfo.RedrawWindow();
	m_bTaskReady = true;

}


HBRUSH CDialogCTypeOper::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

 	int id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_NewTaskInfo||id== IDC_STATIC_NewInfo)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_TaskinfoColor);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	return hbr;
}
void CDialogCTypeOper::OnStnClickedClose()
{
	// TODO:
	CDialogEx::OnCancel();
}
