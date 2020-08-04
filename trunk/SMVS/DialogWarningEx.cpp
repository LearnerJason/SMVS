#include "stdafx.h"
#include "SMVS.h"
#include "DialogWarningEx.h"
#include "afxdialogex.h"

extern CSystemManager gSystemMgr;

IMPLEMENT_DYNAMIC(CDialogWarningEx, CDialogEx)

CDialogWarningEx::CDialogWarningEx(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogWarningEx::IDD, pParent)
{
	m_bWarningBigFlag = false;
}

CDialogWarningEx::~CDialogWarningEx()
{
}

void CDialogWarningEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WARNING_EX, m_CStaticWarningEx);
}

BEGIN_MESSAGE_MAP(CDialogWarningEx, CDialogEx)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEWHEEL()
ON_WM_ACTIVATE()
END_MESSAGE_MAP()


void CDialogWarningEx::SetWarningFont(CFont *font)
{
	m_CStaticWarningEx.SetFont(font);
}

bool CDialogWarningEx::IsExtended()
{
	return m_bWarningBigFlag;
}

void CDialogWarningEx::SetWarningText(CString str)
{
	m_CStaticWarningEx.SetWindowText(str);
}

void CDialogWarningEx::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	if (m_CStaticWarningEx)
	{
		m_CStaticWarningEx.MoveWindow(0,0,cx,cy);
	}
}
void CDialogWarningEx::DisplayWarningMsgNext()
{
	int iErrorCount =  (int)m_vAllErrorInfo.size();

	if (m_WarIndex >= iErrorCount -1)
	{
		return;
	}
	DisplayWarningMsg(++m_WarIndex);
}

void CDialogWarningEx::DisplayWarningMsgPrior()
{
	if (m_bWarningBigFlag)
	{
		if (m_WarIndex < WARNING_COUNT_NUM)
		{
			return;
		}
		else
		{
			DisplayWarningMsg(--m_WarIndex);
		}
	}
	else
	{
		if (m_WarIndex > 0)
		{
			DisplayWarningMsg(--m_WarIndex);
		}
	}
}

void CDialogWarningEx::UpdateWarningMsg()
{
    std::vector<ERROR_INFO> vErrorArray;
    m_vAllErrorInfo = gSystemMgr.GetAllErrorInfo();
	int iErrorCount =  (int)m_vAllErrorInfo.size();
	int m_WarIndex = iErrorCount - 1;
	if (iErrorCount >= 0)
	{
		if ( m_WarIndex>=0)
		{
			DisplayWarningMsg(m_WarIndex);
		}
		else
		{
			SetWarningText(_T(""));
		}
	}
}

void CDialogWarningEx::DisplayWarningMsg(int index,bool bUpdate)
{
	if (bUpdate)
	{
		m_WarIndex = index;
	}
    int iErrorCount =  (int)m_vAllErrorInfo.size();
	
	m_WarIndex = index;
	if (index  < 0 || index > iErrorCount-1)
	{
		return;
	}

	ERROR_INFO crtError = m_vAllErrorInfo[index];
	if(crtError.m_nChIndex == -1 && crtError.m_strMsg.size() == 0)
	{
		return;
	}


    int iErrorNum = 1; 
	CString strMsg;
	CString strDisPlayMsg;


	if(m_bWarningBigFlag)
	{
		iErrorNum = WARNING_COUNT_NUM;
		int bigindex;
		
		if (m_WarIndex - WARNING_COUNT_NUM + 1 < 0)
		{
			bigindex = 0;
		}
		else
		{
			bigindex = m_WarIndex - WARNING_COUNT_NUM + 1;
		}

		int num = iErrorCount - bigindex;

		if (WARNING_COUNT_NUM > num)
		{
			iErrorNum = num;
		}

		for (int i = 0; i < iErrorNum ; i++)
		{
            //TODO:ZJL  m_vAllErrorInfo[bigindex+i] 是否越界？
			crtError = m_vAllErrorInfo[bigindex+i];

			if(crtError.m_nChIndex != 0)
			{
				strMsg.Format(_T(" [%d]CH%d - "), bigindex+i+1,crtError.m_nChIndex);
			}
			else
			{
				strMsg.Format(_T(" [%d]SYS - "), bigindex+i+1);
			}
			
			strDisPlayMsg.Append(strMsg);
			strDisPlayMsg.Append(crtError.m_strMsg.c_str());
			if( i != iErrorNum )
			{
				strDisPlayMsg.Append(_T("\n"));
			}
		}
	}
	else
	{
		if (iErrorCount == 0)
		{
			return;
		}
		int num = iErrorCount - index;

		if (WARNING_COUNT_NUM > num)
		{
			iErrorNum = num;
		}

		for (int i = 0; i < iErrorNum ; i++)
		{
            //TOTO:ZJL --- m_vAllErrorInfo[index+i] 是否越界？
			crtError = m_vAllErrorInfo[index+i];
			strMsg.Format(_T("[%d]CH%d - "), index+i+1,crtError.m_nChIndex);
			strDisPlayMsg.Append(strMsg);
			strDisPlayMsg.Append(crtError.m_strMsg.c_str());
			if( i != iErrorNum )
			{
				strDisPlayMsg.Append(_T("\n"));
			}
		}
	}
	
	SetWarningText(strDisPlayMsg);
}

void CDialogWarningEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect rc;
	CRect crt;
	GetWindowRect(crt);	

	int iErrorCount =  (int)m_vAllErrorInfo.size();

	if (!m_bWarningBigFlag)
	{
		rc.left = crt.left;
		rc.top = crt.bottom - 6-27*WARNING_COUNT_NUM;
		rc.right = crt.right;
		rc.bottom = crt.bottom;
		MoveWindow(rc);
		m_bWarningBigFlag =!m_bWarningBigFlag;

		if (m_WarIndex < WARNING_COUNT_NUM -1)
		{
			m_WarIndex = WARNING_COUNT_NUM -1;
			if (iErrorCount <  WARNING_COUNT_NUM)
			{
				m_WarIndex = iErrorCount - 1;
			}
		}
	}
	else
	{
		rc.left = crt.left;
		rc.top = crt.bottom - 33;
		rc.right = crt.right;
		rc.bottom = crt.bottom;
		MoveWindow(rc);
		m_bWarningBigFlag =!m_bWarningBigFlag;
	}

	DisplayWarningMsg(m_WarIndex);
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

HBRUSH CDialogWarningEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{	
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int ctrlID = pWnd->GetDlgCtrlID();
	if(ctrlID == IDC_STATIC_WARNING_EX)
	{ 
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(245,50,10));

	}
	return hbr;
}

BOOL CDialogWarningEx::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(200,220,250));

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CDialogWarningEx::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDialogWarningEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ( zDelta <0)
		DisplayWarningMsgNext();
	else
		DisplayWarningMsgPrior();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CDialogWarningEx::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	return;
	if (WA_INACTIVE == nState)
	{
		CRect rc;
		CRect crt;
		GetWindowRect(crt);	

		int iErrorCount =  (int)m_vAllErrorInfo.size();

		if (m_bWarningBigFlag)
		{
			rc.left = crt.left;
			rc.top = crt.bottom - 33;
			rc.right = crt.right;
			rc.bottom = crt.bottom;
			MoveWindow(rc);
			m_bWarningBigFlag =!m_bWarningBigFlag;
		}

		DisplayWarningMsg(m_WarIndex);
	}
}
