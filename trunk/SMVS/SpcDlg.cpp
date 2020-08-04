#include "stdafx.h"
#include "SMVS.h"
#include "SpcDlg.h"
#include "afxdialogex.h"
#include "utils.h"
#include "VcAdoSqlserver.h"
#include <boost\circular_buffer.hpp>

using namespace boost;//


IMPLEMENT_DYNAMIC(CSpcDlg, CDialogEx)


CSpcDlg::CSpcDlg(const int chIndex, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpcDlg::IDD, pParent),
	  m_crtChIndex(chIndex)
	  , m_nChRadioBtn(chIndex - 1)
	  , m_bOpRslt(true)
	  ,m_bBatchNumberFun(false)
	  ,m_bUpdateDataBaseFun(false)
{
	int fontSz = 22;
	m_textFont.CreateFont(fontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	fontSz = 19;
	m_comboText.CreateFont(fontSz ,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
		ANSI_CHARSET,             
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("Arial"));

	m_color1 = RGB(200,200,255);
	m_color2 = RGB(209,203,209);
}

CSpcDlg::~CSpcDlg()
{
}

BOOL CSpcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_bBatchNumberFun = gSystemMgr.IsBatchNumberFunTrue();
	m_bUpdateDataBaseFun = gSystemMgr.IsUpdateDatabaseFunTrue();
	InitUiCtrl();
	return TRUE;
}

BOOL CSpcDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSpcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CH1, m_nChRadioBtn);
	DDX_Control(pDX, IDC_LIST_TEST_ITEM, m_ListTestItem);
	DDX_Control(pDX, IDC_COMBO_CH_TASK, m_comboTask);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BGN_DATE, m_dateBgn);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END_DATE, m_dateEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BGN_TIME, m_timeBgn);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END_TIME, m_timeEnd);
	DDX_Control(pDX, IDC_COMBO_SHIFT_OPTION, m_shiftCombo);
	DDX_Control(pDX, IDC_COMBO_BATCHNUMBER, m_BatchNumberCombo);
	DDX_Control(pDX, IDC_COMBO_USERNAME, m_comboxUsername);
}

BEGIN_MESSAGE_MAP(CSpcDlg, CDialogEx)
	ON_STN_CLICKED(IDB_SPC_DLG_CLOSE,      &CSpcDlg::OnCloseScreen)
	ON_STN_CLICKED(SPC_UI_IDB_RIGHT_ARROW, &CSpcDlg::OnNextBtnClicked)
	ON_STN_CLICKED(SPC_UI_IDB_LEFT_ARROW,  &CSpcDlg::OnPreviousBtnClicked)
	ON_STN_CLICKED(SPC_UI_QUERY_BTN,       &CSpcDlg::OnQueryBtnClicked)
	ON_STN_CLICKED(SPC_UI_EXPORT_BTN,      &CSpcDlg::OnExportBtnClicked)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_CH_TASK, &CSpcDlg::OnCbnSelchangeComboChTask)
	ON_BN_CLICKED(IDC_RADIO_CH1, &CSpcDlg::OnBnClickedRadioCh1)
	ON_BN_CLICKED(IDC_RADIO_CH2, &CSpcDlg::OnBnClickedRadioCh2)
	ON_BN_CLICKED(IDC_RADIO_CH3, &CSpcDlg::OnBnClickedRadioCh3)
	ON_BN_CLICKED(IDC_RADIO_CH4, &CSpcDlg::OnBnClickedRadioCh4)
	ON_BN_CLICKED(IDC_RADIO_CH5, &CSpcDlg::OnBnClickedRadioCh5)
	ON_BN_CLICKED(IDC_RADIO_CH6, &CSpcDlg::OnBnClickedRadioCh6)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_SHIFT_OPTION, &CSpcDlg::OnCbnSelchangeComboShiftOption)
	ON_BN_CLICKED(IDC_RADIO_CH7, &CSpcDlg::OnBnClickedRadioCh7)
	ON_BN_CLICKED(IDC_RADIO_CH8, &CSpcDlg::OnBnClickedRadioCh8)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_END_TIME, &CSpcDlg::OnDtnDatetimechangeDatetimepickerEndTime)
	ON_CBN_SELCHANGE(IDC_COMBO_BATCHNUMBER, &CSpcDlg::OnCbnSelchangeComboBatchnumber)
END_MESSAGE_MAP()


void CSpcDlg::OnPaint()
{
	CPaintDC dc(this);
	LoadBackgroundImage(dc);
}

void CSpcDlg::LoadBackgroundImage(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);

	CBitmap bakImg;
	// TODO:
	if(bakImg.LoadBitmap(IDB_BMP_CH_DLG_BG))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		CBitmap* pPngOld = dcMem.SelectObject(&bakImg);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	}
}

void CSpcDlg::InitDlgHeader(CRect& rcWorkArea)
{
	m_dlgTitle.SubclassDlgItem(IDC_STATIC_DLG_TITLE,this);

	CString strTitle;
	strTitle.LoadString(IDS_STRING_PRODUCT_STATS_DATA);
	m_dlgTitle.SetWindowText(strTitle);

	m_dlgTitle.SetColor(RGB(10,10,115),RGB(171,178,185));
	double rate = rcWorkArea.Height() /1080.0;
	if(rate > 1.0f)
	{
		rate = 1.0f;
	}
	int fontSz = (int)(60 * rate);
	m_dlgTitle.SetTxtFont(fontSz,FW_BOLD);

	CRect titleRect;
	m_dlgTitle.GetClientRect(&titleRect);
	int szWidth = titleRect.Width();
	int szHeight = titleRect.Height();
	m_dlgTitle.MoveWindow((int)rcWorkArea.Width()/3 - 50,rcWorkArea.top + 4,szWidth,szHeight);

	// Close button
	int gapH = 2;
	float fRate = 880.0f / rcWorkArea.Height();
	int titleAreaH = int(52.0f / fRate);
	CRect rcClose;
	rcClose.left = rcWorkArea.right - 2* gapH - 48;
	rcClose.top = int((titleAreaH - 48) / 2.0) - 5;
	rcClose.right = rcClose.left + 48;
	rcClose.bottom = rcClose.top + 48;
	m_btnClose.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcClose, this, IDB_SPC_DLG_CLOSE);
	m_btnClose.SetLabelImage(IMG_CLOSE_DLG);
	m_btnClose.Invalidate();
}

void CSpcDlg::InitChartCtrl(CRect& rcWorkArea)
{
	float fRate = 880.0f / rcWorkArea.Height();
	int titleAreaH = int(52.0f / fRate);
	int vtBtnSz = 64; // Vertical Button Size
	int hzBtnSz = 64; //  Horizontal Button Size;
	int gapH = 5;
	int gapV = 10;
	 
	CRect rcChart;
	rcChart.left = rcWorkArea.left + 10;
	rcChart.top =  rcWorkArea.top + titleAreaH + 15;
	rcChart.right = rcWorkArea.right - 84;
	rcChart.bottom = rcChart.top + 250;

	SPC_SETTING spcSetting;
	spcSetting.m_strUnitX = L"X1000";
	spcSetting.m_nXLen    = 50;
	spcSetting.m_strUnitY = L"‰";

	m_chartCtrl.Create(NULL,NULL,NULL, rcChart, this,NULL);
	m_chartCtrl.CreateLineGraph(rcChart, 50, spcSetting.m_strUnitX, spcSetting.m_strUnitY);
	m_chartCtrl.ShowGraphLegend(FALSE);
	m_chartCtrl.SetGraphBackgroundColor(RGB(200,220,250));
	CString title;
	title = _T("");
	m_chartCtrl.SetGraphTitle(title);
	m_chartCtrl.SetGraphTitleColor(RGB(255,0,0));

	m_chartCtrl.SetGraphSubtitle(_T(""));
	m_chartCtrl.SetGraphTitleShadow(FALSE);
	m_chartCtrl.SetGraphSubtitleShadow(FALSE);
	m_chartCtrl.Invalidate();

	CRect rcNext, rcPrevious;
	rcNext.left = rcChart.right + 10;
	rcNext.top = rcChart.top + int((rcChart.Height() - 128 -10) / 2.0);
	rcNext.right = rcNext.left + 64;
	rcNext.bottom = rcNext.top + 64;
	m_btnNext.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcNext, this, SPC_UI_IDB_RIGHT_ARROW);
	m_btnNext.SetLabelImage(IMG_CH_RIGHT_ARROW_GRAY);
	m_btnNext.EnableWindow(FALSE);
	m_btnNext.Invalidate();

	rcPrevious.left = rcChart.right + 10;
	rcPrevious.top = rcNext.bottom + 10;
	rcPrevious.right = rcPrevious.left + 64;
	rcPrevious.bottom = rcPrevious.top + 64;
	m_btnPrevious.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcPrevious, this, SPC_UI_IDB_LEFT_ARROW);
	m_btnPrevious.SetLabelImage(IMG_CH_LEFT_ARROW_GRAY);
	m_btnPrevious.EnableWindow(FALSE);
	m_btnPrevious.Invalidate();
}

void CSpcDlg::InitChannelSwitch(CRect& rcWorkArea)
{
	CRect rcChart;
	m_chartCtrl.GetWindowRect(rcChart);

	InitUiWidget(IDC_RADIO_CH1);
	InitUiWidget(IDC_STATIC_CH1);
	m_vCtrlID.push_back(IDC_RADIO_CH1);
	m_vCtrlID.push_back(IDC_STATIC_CH1);

	InitUiWidget(IDC_RADIO_CH2);
	InitUiWidget(IDC_STATIC_CH2);
	m_vCtrlID.push_back(IDC_RADIO_CH2);
	m_vCtrlID.push_back(IDC_STATIC_CH2);

	InitUiWidget(IDC_RADIO_CH3);
	InitUiWidget(IDC_STATIC_CH3);
	m_vCtrlID.push_back(IDC_RADIO_CH3);
	m_vCtrlID.push_back(IDC_STATIC_CH3);

	InitUiWidget(IDC_RADIO_CH4);
	InitUiWidget(IDC_STATIC_CH4);
	m_vCtrlID.push_back(IDC_RADIO_CH4);
	m_vCtrlID.push_back(IDC_STATIC_CH4);

	InitUiWidget(IDC_RADIO_CH5);
	InitUiWidget(IDC_STATIC_CH5);
	m_vCtrlID.push_back(IDC_RADIO_CH5);
	m_vCtrlID.push_back(IDC_STATIC_CH5);

	InitUiWidget(IDC_RADIO_CH6);
	InitUiWidget(IDC_STATIC_CH6);
	m_vCtrlID.push_back(IDC_RADIO_CH6);
	m_vCtrlID.push_back(IDC_STATIC_CH6);

	InitUiWidget(IDC_RADIO_CH7);
	InitUiWidget(IDC_STATIC_CH7);
	m_vCtrlID.push_back(IDC_RADIO_CH7);
	m_vCtrlID.push_back(IDC_STATIC_CH7);

	InitUiWidget(IDC_RADIO_CH8);
	InitUiWidget(IDC_STATIC_CH8);
	m_vCtrlID.push_back(IDC_RADIO_CH8);
	m_vCtrlID.push_back(IDC_STATIC_CH8);

	auto pChConfigArray = ::gConfigMgr.GetChannelConfigPtr();
	int chCount = (int)pChConfigArray->size();
	CRect rcRadio, rcStatic;
	BOOL bVisible = TRUE;
	for(size_t i = 0; i < m_vCtrlID.size(); i += 2)
	{
		if((i/2) >= chCount) bVisible = FALSE;

		rcRadio.left = rcChart.left + long((i/2) * 80); //420 加上工号选项
		rcRadio.top = rcChart.bottom + 10;
		rcRadio.right = rcRadio.left + 20;
		rcRadio.bottom = rcRadio.top + 20;

		CWnd* pWnd = GetDlgItem(m_vCtrlID[i]);
		pWnd->MoveWindow(rcRadio);
		pWnd->ShowWindow((bVisible == TRUE)? SW_SHOW:SW_HIDE);

		rcStatic.left = rcRadio.right;
		rcStatic.top = rcRadio.top - 2;
		rcStatic.right = rcStatic.left + 40;
		rcStatic.bottom = rcRadio.bottom;

		pWnd = GetDlgItem(m_vCtrlID[i + 1]);
		pWnd->MoveWindow(rcStatic);
		pWnd->ShowWindow((bVisible == TRUE)? SW_SHOW:SW_HIDE);
	}
}
void CSpcDlg::FillUserNameCombo()
{
	std::map<wstring,UAC> m_dictUac;
	gSystemMgr.GetAllUacInfo(m_dictUac);

	for(auto it = m_dictUac.begin(); it != m_dictUac.end(); it++)
	{
		m_comboxUsername.AddString(it->first.c_str());
	}
	m_comboxUsername.AddString(_T(""));

}
void CSpcDlg::FillTaskCombo(int chIndex)
{
	m_comboTask.ResetContent();
	m_comboTask.InsertString(0, _T(""));

	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	CHANNEL_CONFIG chConfig = (*pChConfigArray)[chIndex];
	TASK crtTask;
	chConfig.GetCurrentTask(crtTask);
	m_strCrtTaskName = crtTask.m_wstrTaskName;
	int crtIndex = -1;

	int index = 1;
	for(int i = 0; i < (int)chConfig.m_vTask.size(); i++)
	{
		TASK task = chConfig.m_vTask[i];
		if(task.m_bSelfLearning)
			continue;
		m_comboTask.InsertString(index, task.m_wstrTaskName.c_str());
		if(task.m_wstrTaskName == crtTask.m_wstrTaskName)
			crtIndex = index;
		index++;
	}
	m_comboTask.SetCurSel(crtIndex);
	UpdateData(FALSE);
}

void CSpcDlg::InitTaskComboBox(CRect& rcWorkArea)
{
	CRect rcRadio;
	CWnd* pWnd = GetDlgItem(IDC_RADIO_CH1);
	pWnd->GetWindowRect(rcRadio);

	//添加工号选框
	InitUiWidget(IDC_STATIC_USERNAME,IDS_STRING_USERNAME);
	InitUiWidget(IDC_COMBO_USERNAME);

	CRect rcStatic;
	rcStatic.left = rcRadio.left;
	rcStatic.top  = rcRadio.bottom + 8;
	rcStatic.right = rcStatic.left + 110;
	rcStatic.bottom = rcStatic.top + 22;
	pWnd = GetDlgItem(IDC_STATIC_USERNAME);
	pWnd->MoveWindow(rcStatic);

	CRect rcCombo;
	rcCombo.left   = rcStatic.right + 10;
	rcCombo.top    = rcStatic.top - 2;
	rcCombo.right  = rcCombo.left + 310;
	rcCombo.bottom = rcCombo.top + 20;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_USERNAME);
	pCombo->SetFont(&m_comboText);
	pCombo->MoveWindow(rcCombo);

	FillUserNameCombo();


	pWnd = GetDlgItem(IDC_STATIC_USERNAME);
	pWnd->GetWindowRect(rcRadio);

	InitUiWidget(IDC_STATIC_TASK_NAME,IDS_STRING_TASK_CANDIDATE);
	InitUiWidget(IDC_COMBO_CH_TASK);

//	CRect rcStatic;
	rcStatic.left = rcRadio.left;
	rcStatic.top  = rcRadio.bottom + 14;
	rcStatic.right = rcStatic.left + 110;
	rcStatic.bottom = rcStatic.top + 22;
	pWnd = GetDlgItem(IDC_STATIC_TASK_NAME);
	pWnd->MoveWindow(rcStatic);

//	CRect rcCombo;
	rcCombo.left   = rcStatic.right + 10;
	rcCombo.top    = rcStatic.top - 2;
	rcCombo.right  = rcCombo.left + 310;
	rcCombo.bottom = rcCombo.top + 22;

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CH_TASK);
	pCombo->SetFont(&m_comboText);
	pCombo->MoveWindow(rcCombo);

	FillTaskCombo(m_crtChIndex);
}

void CSpcDlg::SetTimerPicker(const SHIFT& shift)
{
	SYSTEMTIME dt;
	::GetLocalTime(&dt);

	dt.wHour   = WORD(shift.m_bgnTime / 100);
	dt.wMinute = WORD(shift.m_bgnTime % 100);
	dt.wSecond = 0;
	m_timeBgn.SetTime(&dt);

	dt.wHour   = WORD(shift.m_endTime / 100);
	dt.wMinute = WORD(shift.m_endTime % 100);
	if(shift.m_bgnTime >= shift.m_endTime)
	{
		CTime cTime((int)dt.wYear, (int)dt.wMonth, (int)dt.wDay, (int)dt.wHour, (int)dt.wMinute, (int)dt.wSecond);
		CTimeSpan timeSpan(1,0,0,0);
		CTime newTime = cTime + timeSpan;
		dt.wDay = newTime.GetDay();
	}
	m_dateEnd.SetTime(&dt);
	m_timeEnd.SetTime(&dt);
}

void CSpcDlg::FillShiftComboBox()
{
	std::vector<SHIFT>* pShiftArray = ::gConfigMgr.GetShiftArrayPtr();
	if(pShiftArray == nullptr || pShiftArray->empty())
	{
		return;
	}
	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	long nNow = dt.wHour * 100 + dt.wMinute; 

	m_shiftCombo.ResetContent();
	int index = 0;
	int nCrtIndex = -1;
	for(auto it = pShiftArray->begin(); it != pShiftArray->end(); it++)
	{
		m_shiftCombo.InsertString(index, it->m_wstrShiftName.c_str());
		if(it->m_bgnTime < nNow &&  nNow <= it->m_endTime)
		{
			nCrtIndex  = index;
			SetTimerPicker(*it);
		}
		index++;
	}
	CString str;
	str.LoadString(IDS_STRING_TODAY);
	m_shiftCombo.InsertString(index, str);
	m_shiftCombo.SetCurSel(nCrtIndex);
}
void CSpcDlg::InitShiftComboBox(CRect& rcWorkArea)
{
	InitUiWidget(IDC_STATIC_SHIFT_OPTION,IDS_STRING_SHIFT_OPTION);
	InitUiWidget(IDC_COMBO_SHIFT_OPTION);

	CWnd* pWnd = GetDlgItem(IDC_COMBO_BATCHNUMBER);
	pWnd->ShowWindow(SW_HIDE);

	pWnd = GetDlgItem(IDC_STATIC_BATCH_NUMBER);
	pWnd->ShowWindow(SW_HIDE);

	CRect rc;
	 pWnd = GetDlgItem(IDC_STATIC_TASK_NAME);
	pWnd->GetWindowRect(rc);

	rc.top    = rc.bottom + 9;
	rc.bottom = rc.top + 22;

	pWnd = GetDlgItem(IDC_STATIC_SHIFT_OPTION);
	pWnd->MoveWindow(rc);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CH_TASK);
	pCombo->GetWindowRect(rc);
	rc.top    = rc.bottom + 9;
	rc.bottom = rc.top + 22;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SHIFT_OPTION);
	pCombo->SetFont(&m_comboText);
	pCombo->MoveWindow(rc);

	FillShiftComboBox();
}
void CSpcDlg::InitShiftBatchNumerComboBox(CRect& rcWorkArea)
{
	InitUiWidget(IDC_STATIC_SHIFT_OPTION,IDS_STRING_SHIFT_OPTION);
	InitUiWidget(IDC_COMBO_SHIFT_OPTION);

	CRect rc;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_TASK_NAME);
	pWnd->GetWindowRect(rc);

	rc.top    = rc.bottom + 15;
	rc.bottom = rc.top + 22;

	pWnd = GetDlgItem(IDC_STATIC_SHIFT_OPTION);
	pWnd->MoveWindow(rc);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CH_TASK);
	pCombo->GetWindowRect(rc);
	CRect rcComTask = rc;
	rc.top    = rc.bottom + 10;
	rc.bottom = rc.top + 22;
	rc.right = rc.left + 90;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SHIFT_OPTION);
	pCombo->SetFont(&m_comboText);
	pCombo->MoveWindow(rc);

	FillShiftComboBox();

	InitUiWidget(IDC_STATIC_BATCH_NUMBER,IDS_STRING_BATCH_NUMBER);
	InitUiWidget(IDC_COMBO_BATCHNUMBER);
	
	pWnd = GetDlgItem(IDC_COMBO_SHIFT_OPTION);
	pWnd->GetWindowRect(rc);

	rc.left    = rc.right + 15;
	rc.right  = rc.left + 55;

	CRect crtRt = rc;
	crtRt.top = crtRt.top +2;
	crtRt.bottom = crtRt.bottom +2;

	pWnd = GetDlgItem(IDC_STATIC_BATCH_NUMBER);
	pWnd->MoveWindow(crtRt);

	rc.left    = rc.right;
	rc.right  = rcComTask.right;

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BATCHNUMBER);
	pCombo->SetFont(&m_comboText);
	pCombo->MoveWindow(rc);

	FillBatchNumberComboBox();



	OnCbnSelchangeComboBatchnumber();
	
}
void CSpcDlg::FillBatchNumberComboBox()
{
	//获取任务名
	CString sTaskName;
	CString sMsg;
	int index = m_comboTask.GetCurSel();
	if(index == -1)
	{
		sMsg.LoadString(IDS_STRING_PLEASE_SELECT_TASK);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	m_comboTask.GetLBText(index, sTaskName);
	wstring strTaskName = CT2W(sTaskName);

	std::string strTableName = utils::Unicode2Utf8(strTaskName);

// 获取近一个月的时间
	SYSTEMTIME dt;
	::GetLocalTime(&dt);

	SYSTEMTIME bgn = dt;
	SYSTEMTIME end = dt;
	CTime tmend((int)dt.wYear, (int)dt.wMonth, (int)dt.wDay, (int)dt.wHour, (int)dt.wMinute, (int)dt.wSecond);
	CTimeSpan  timeSpan(30,0,0,0);
	CTime tmbgn = tmend - timeSpan;

	bgn.wYear = tmbgn.GetYear();
	bgn.wMonth = tmbgn.GetMonth();
	bgn.wDay = tmbgn.GetDay();

	//起始一个月的时间戳
	long long lBgn = utils::DateTime2Int64(bgn);
	long long lEnd = utils::DateTime2Int64(end);



	//运行SQL语句
	vector<CString> m_vecStrBatchNumber = gSystemMgr.GetLastMonthBatchNumber(strTableName,lBgn, lEnd);
	for ( int i = 0 ; i< m_vecStrBatchNumber.size(); ++i )
	{
		m_BatchNumberCombo.AddString(m_vecStrBatchNumber[i]);
		if (m_vecStrBatchNumber[i] == gSystemMgr.GetCurBatchNumber())
		{
			m_BatchNumberCombo.SetCurSel(i);
		}
	}
	//添加无批次号查询
	m_BatchNumberCombo.AddString(_T(""));
}
void CSpcDlg::InitDateTimePicker(CRect& rcWorkArea)
{
	InitUiWidget(IDC_STATIC_START_DATETIME, IDS_STRING_BEGIN_DATETIME);
	InitUiWidget(IDC_DATETIMEPICKER_BGN_DATE);
	InitUiWidget(IDC_DATETIMEPICKER_BGN_TIME);

	InitUiWidget(IDC_STATIC_END_DATETIME, IDS_STRING_END_DATETIME);
	InitUiWidget(IDC_DATETIMEPICKER_END_DATE);
	InitUiWidget(IDC_DATETIMEPICKER_END_TIME);

	CRect rcShift;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SHIFT_OPTION);
	pWnd->GetWindowRect(rcShift);
	
	CRect rcBgnText;
	rcBgnText.left   = rcShift.left;
	rcBgnText.top    = rcShift.bottom + 16;
	rcBgnText.right  = rcShift.left + 110;
	rcBgnText.bottom = rcBgnText.top + 22;
	pWnd = GetDlgItem(IDC_STATIC_START_DATETIME);
	pWnd->MoveWindow(rcBgnText);

	CRect rcEndText;
	rcEndText.left   = rcShift.left;
	rcEndText.top    = rcBgnText.bottom + 16;
	rcEndText.right  = rcShift.left + 110;
	rcEndText.bottom = rcEndText.top + 22;
	pWnd = GetDlgItem(IDC_STATIC_END_DATETIME);
	pWnd->MoveWindow(rcEndText);

	CRect rcShiftCombo;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CH_TASK);

	pCombo->GetWindowRect(rcShiftCombo);

	rcShiftCombo.top    = rcShiftCombo.bottom + 12;
	rcShiftCombo.bottom = rcShiftCombo.top + 22;

	// Datetime Picker
	CRect rcBgnDate, rcBgnTime;
	CRect rcEndDate, rcEndTime;

	SYSTEMTIME dt;
	::GetLocalTime(&dt);

	rcBgnDate.left = rcShiftCombo.left;
	rcBgnDate.top = rcShiftCombo.bottom + 13;
	pWnd = GetDlgItem(IDC_DATETIMEPICKER_BGN_DATE);
	CRect rcRet;
	pWnd->GetWindowRect(rcRet);
	rcBgnDate.bottom = rcBgnDate.top + rcRet.Height();
	pWnd->MoveWindow(rcBgnDate.left, rcBgnDate.top, rcRet.Width(), rcRet.Height());
	m_dateBgn.SetFormat(_T("yyyy-MM-dd"));
	m_dateBgn.SetTime(&dt);

	rcBgnTime.right = rcShiftCombo.right;
	rcBgnTime.top   = rcShiftCombo.bottom + 13;
	pWnd = GetDlgItem(IDC_DATETIMEPICKER_BGN_TIME);
	pWnd->GetWindowRect(rcRet);
	pWnd->MoveWindow(rcBgnTime.right - rcRet.Width(),rcBgnTime.top, rcRet.Width(), rcRet.Height());
	m_timeBgn.SetFormat(_T("HH:mm"));

	// End Datetime Picker
	rcEndDate.left = rcShiftCombo.left;
	rcEndDate.top  = rcBgnDate.bottom + 12;
	pWnd = GetDlgItem(IDC_DATETIMEPICKER_END_DATE);
	pWnd->GetWindowRect(rcRet);
	pWnd->MoveWindow(rcEndDate.left, rcEndDate.top, rcRet.Width(), rcRet.Height());
	m_dateEnd.SetFormat(_T("yyyy-MM-dd"));
	m_dateEnd.SetTime(&dt);

	rcEndTime.right = rcShiftCombo.right;
	rcEndTime.top   = rcEndDate.top;
	pWnd = GetDlgItem(IDC_DATETIMEPICKER_END_TIME);
	pWnd->GetWindowRect(rcRet);
	pWnd->MoveWindow(rcEndTime.right - rcRet.Width(),rcEndTime.top, rcRet.Width(), rcRet.Height());
	m_timeEnd.SetFormat(_T("HH:mm"));
}

void CSpcDlg::InitQueryReport(CRect& rcWorkArea)
{
	CRect rcEndText;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_END_DATETIME);
	pWnd->GetWindowRect(rcEndText);

	CRect rcEndTime;
	pWnd = GetDlgItem(IDC_DATETIMEPICKER_END_TIME);
	pWnd->GetWindowRect(rcEndTime);

	InitUiWidget(IDC_STATIC_REPORT_GROUP,IDS_STRING_QUERY_REPORT);
	CRect rcGroup;
	rcGroup.left = rcWorkArea.left + 10;
	rcGroup.top = rcEndText.bottom + 18;
	rcGroup.right = rcEndTime.right;
	rcGroup.bottom = rcWorkArea.bottom - 10 - 200 - 45;//65;

	MoveUiWidget(IDC_STATIC_REPORT_GROUP, rcGroup);
	// Total Count;
	CRect rcText, rcEdit;
	int nTextLen = 140;
	int nGap = 18;
	int nTextHeight = 25;
	rcText.left = rcGroup.left + nGap;
	rcText.top = rcGroup.top + 35;
	rcText.right = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_TOTAL, IDS_STRING_TOTAL);
	MoveUiWidget(IDC_STATIC_TOTAL, rcText);

	rcEdit.left = rcText.right + 5;
	rcEdit.top = rcText.top;
	rcEdit.right = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_TOTAL);
	MoveUiWidget(IDC_EDIT_TOTAL, rcEdit);
	// Defect Count
	rcText.left = rcGroup.left + nGap;
	rcText.top = rcText.bottom + 10;
	rcText.right = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_DEFECT, IDS_STRING_REJECT_COUNT);
	MoveUiWidget(IDC_STATIC_DEFECT, rcText);

	rcEdit.left = rcText.right + 5;
	rcEdit.top = rcText.top;
	rcEdit.right = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_DEFECT);
	MoveUiWidget(IDC_EDIT_DEFECT, rcEdit);
	// Defect Rate
	rcText.left = rcGroup.left + nGap;
	rcText.top = rcText.bottom + 10;
	rcText.right = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_DEFECT_RATE, IDS_STRING_RATE);
	MoveUiWidget(IDC_STATIC_DEFECT_RATE, rcText);

	rcEdit.left = rcText.right + 5;
	rcEdit.top = rcText.top;
	rcEdit.right = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_DEFECT_RATE);
	MoveUiWidget(IDC_EDIT_DEFECT_RATE, rcEdit);
	//Error Count
	rcText.left = rcGroup.left + nGap;
	rcText.top = rcText.bottom + 10;
	rcText.right = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_ERROR, IDS_STRING_ERROR_COUNT);
	MoveUiWidget(IDC_STATIC_ERROR, rcText);

	rcEdit.left = rcText.right + 5;
	rcEdit.top = rcText.top;
	rcEdit.right = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_ERROR_COUNT);
	MoveUiWidget(IDC_EDIT_ERROR_COUNT, rcEdit);
	//Error/One Cap
	rcText.left = rcGroup.left + nGap;
	rcText.top = rcText.bottom + 10;
	rcText.right = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_ERROR_PER_ONE, IDS_STRING_ERROR_PER_ONE);
	MoveUiWidget(IDC_STATIC_ERROR_PER_ONE, rcText);

	rcEdit.left = rcText.right + 5;
	rcEdit.top = rcText.top;
	rcEdit.right = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_ERROR_PER_ONE);
	MoveUiWidget(IDC_EDIT_ERROR_PER_ONE, rcEdit);

	// Glue Injector Statistics
#ifdef EOE_PRODUCT

	rcText.left   = rcGroup.left + nGap;
	rcText.top    = rcText.bottom + 10;
	rcText.right  = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_GLUE_INJECTOR_STATS,IDS_STRING_GLUE_INJECTOR_STATS);
	MoveUiWidget(IDC_STATIC_GLUE_INJECTOR_STATS, rcText);
	pWnd = GetDlgItem(IDC_STATIC_GLUE_INJECTOR_STATS);
	pWnd->ShowWindow(SW_SHOW);

	rcEdit.left   = rcText.right + 5;
	rcEdit.top    = rcText.top;
	rcEdit.right  = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_GLUE_INJECTOR_STATS);
	MoveUiWidget(IDC_EDIT_GLUE_INJECTOR_STATS, rcEdit);
	pWnd = GetDlgItem(IDC_EDIT_GLUE_INJECTOR_STATS);
	pWnd->ShowWindow(SW_SHOW);
	pWnd->SetFont(&m_comboText);

#endif
	//统计数据标签lab-ctrl
	InitUiWidget(IDC_STATIC_REPORT_GROUP_SYS,IDS_STRING_QUERY_REPORT_SYS);
	rcGroup.left = rcWorkArea.left + 10;
	rcGroup.top = rcEdit.bottom + 20;
	rcGroup.right = rcEndTime.right;
	rcGroup.bottom = rcWorkArea.bottom - 10;
	MoveUiWidget(IDC_STATIC_REPORT_GROUP_SYS, rcGroup);

	//统计数据 
	rcText.left = rcGroup.left + nGap;
	rcText.top = rcText.bottom + 20 + 35;
	rcText.right = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_TOTAL_SYS, IDS_STRING_TOTAL_SYS);
	MoveUiWidget(IDC_STATIC_TOTAL_SYS, rcText);

	rcEdit.left = rcText.right + 5;
	rcEdit.top = rcText.top;
	rcEdit.right = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_TOTAL_SYS);
	MoveUiWidget(IDC_EDIT_TOTAL_SYS, rcEdit);

	rcText.left = rcGroup.left + nGap;
	rcText.top = rcText.bottom + 10;
	rcText.right = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_DEFECT_SYS, IDS_STRING_REJECT_COUNT_SYS);
	MoveUiWidget(IDC_STATIC_DEFECT_SYS, rcText);

	rcEdit.left = rcText.right + 5;
	rcEdit.top = rcText.top;
	rcEdit.right = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_DEFECT_SYS);
	MoveUiWidget(IDC_EDIT_DEFECT_SYS, rcEdit);

	rcText.left = rcGroup.left + nGap;
	rcText.top = rcText.bottom + 10;
	rcText.right = rcText.left + nTextLen;
	rcText.bottom = rcText.top + nTextHeight;
	InitUiWidget(IDC_STATIC_DEFECT_RATE_SYS, IDS_STRING_RATE_SYS);
	MoveUiWidget(IDC_STATIC_DEFECT_RATE_SYS, rcText);

	rcEdit.left = rcText.right + 5;
	rcEdit.top = rcText.top;
	rcEdit.right = rcGroup.right - nGap;
	rcEdit.bottom = rcText.bottom;
	InitUiWidget(IDC_EDIT_DEFECT_RATE_SYS);
	MoveUiWidget(IDC_EDIT_DEFECT_RATE_SYS, rcEdit);



	// Message Panel
	CRect rcMsg;
	rcMsg.left = rcGroup.left + nGap;
	rcMsg.top = rcEdit.bottom + 10;
	rcMsg.right = rcGroup.right - nGap;
	rcMsg.bottom = rcGroup.bottom - 5;
	MoveUiWidget(IDC_STATIC_MSG_PANEL, rcMsg);

	InitUiWidget(IDC_STATIC_MSG_PANEL);

// 	if (!gSystemMgr.IsTableExists("SYS_STATS"))
// 	{
// 		GetDlgItem(IDC_STATIC_REPORT_GROUP_SYS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_TOTAL_SYS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_EDIT_TOTAL_SYS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_DEFECT_SYS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_EDIT_DEFECT_SYS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_DEFECT_RATE_SYS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_EDIT_DEFECT_RATE_SYS)->ShowWindow(SW_HIDE);
// 	}
}

void CSpcDlg::FillTestItemList(const TASK& task)
{
	m_ListTestItem.DeleteAllItems();
	if(task.m_bSelfLearning)
		return;

	int index      = 0;
	int nListIndex = 1;
	CString strListIndex;
	m_vItemName.clear();
	int nColorSwitch   = 0;
	COLORREF crtColor;
	int sz = (int)task.m_vItParam.size();
	for(int i = 1; i < sz; i++)
	{
		IT_PARAM itParam = task.m_vItParam[i];
		wstring strItemName;
		strItemName = CT2W(strListIndex);
		strItemName.append(L"#");
		strItemName.append(itParam.m_itNickname);
		m_vItemName.push_back(strItemName);

		wstring wstrNickname   = itParam.m_itNickname;

		if(!itParam.IsTestAlgo())
		{
			m_ListTestItem.InsertItem(index, _T("∑"));
			m_ListTestItem.SetItemText(index, 1, W2CT(wstrNickname.c_str()));
			crtColor = (nColorSwitch % 2 == 0)? m_color1 : m_color2;
			m_ListTestItem.SetItemColor(index, RGB(0,0,0),crtColor);
			nColorSwitch++;
		}
		else
		{
			strListIndex.Format(_T("%d"), nListIndex);
			m_ListTestItem.InsertItem(index, strListIndex);
			CString strTmp;
			strTmp.Format(_T("     %s"),wstrNickname.c_str());
			m_ListTestItem.SetItemText(index, 1, strTmp);
			m_ListTestItem.SetItemColor(index, RGB(0,0,0),crtColor);
		}
		nListIndex++;
		index++;
	}
}

void CSpcDlg::InitTestItemList(CRect& rcWorkArea)
{
	m_ListTestItem.SetFont(&m_comboText);

	CRect rcCombo;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_USERNAME);
	pCombo->GetWindowRect(rcCombo);

	CRect rcEndText;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_END_DATETIME);
	pWnd->GetWindowRect(rcEndText);

	CRect rcChart;
	m_chartCtrl.GetWindowRect(rcChart);

	CRect rcList;
	rcList.left = rcCombo.right + 20;
	rcList.top = rcCombo.top;
	rcList.right = rcChart.right;
	rcList.bottom = rcWorkArea.bottom - 10;
	m_ListTestItem.MoveWindow(rcList);

	CRect rcQueryBtn;
	rcQueryBtn.left = rcList.right + 10;
	rcQueryBtn.top = rcList.top + int((rcList.Height() - 128 - 10) / 2.0f);
	rcQueryBtn.right = rcQueryBtn.left + 64;
	rcQueryBtn.bottom = rcQueryBtn.top + 64;
	m_btnQuery.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcQueryBtn, this, SPC_UI_QUERY_BTN);
	m_btnQuery.SetLabelImage(IMG_SPC_QUERY);
	m_btnQuery.Invalidate();

	CRect rcExport;
	rcExport.left = rcQueryBtn.left;
	rcExport.top = rcQueryBtn.bottom + 10;
	rcExport.right = rcExport.left + 64;
	rcExport.bottom = rcExport.top + 64;
	m_btnExport.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_NOTIFY, rcExport, this, SPC_UI_EXPORT_BTN);
	m_btnExport.SetLabelImage(IMG_SPC_EXPORT);
	//m_btnExport.EnableWindow(FALSE);
	m_btnExport.Invalidate();
	// Initalize the table header of the Test Item List;
	DWORD dwStyleEx = m_ListTestItem.GetExtendedStyle();
	dwStyleEx |= LVS_EX_FULLROWSELECT; 
	dwStyleEx |= LVS_EX_GRIDLINES;     
	dwStyleEx |= LVS_EX_CHECKBOXES;   
	m_ListTestItem.SetExtendedStyle(dwStyleEx);

	m_ListTestItem.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strIndex(MAKEINTRESOURCE(IDS_STRING_INDEX));
	CString strAlgoName(MAKEINTRESOURCE(IDS_STRING_ALGO_NAME));
	CString strErrorCount(MAKEINTRESOURCE(IDS_STRING_ERROR_COUNT));
	CString strErrorRate(MAKEINTRESOURCE(IDS_STRING_ERROR_RATE));
	CString strDisplayLine(MAKEINTRESOURCE(IDS_STRING_DISPLAY_GRAPH));
	CString strLineColor(MAKEINTRESOURCE(IDS_STRING_GRAPH_COLOR));

	m_ListTestItem.InsertColumn(0, strIndex, LVCFMT_CENTER, 65);
	m_ListTestItem.InsertColumn(1, strAlgoName, LVCFMT_LEFT, 300);
	m_ListTestItem.InsertColumn(2, strErrorCount, LVCFMT_LEFT, 160);
	m_ListTestItem.InsertColumn(3, strErrorRate, LVCFMT_LEFT, 160);
	m_ListTestItem.InsertColumn(4, strDisplayLine, LVCFMT_LEFT, 150);
	m_ListTestItem.InsertColumn(5, strLineColor, LVCFMT_LEFT, 150);

	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	CHANNEL_CONFIG chConfig = (*pChConfigArray)[m_crtChIndex];
	TASK crtTask;
	chConfig.GetCurrentTask(crtTask);
	FillTestItemList(crtTask);
	UpdateData(FALSE);
}

void CSpcDlg::InitUiWidget(int nCtrlID, int strID)
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

void CSpcDlg::MoveUiWidget(int nCtrlID, CRect& rc)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	if(pWnd != nullptr)
	{
		pWnd->MoveWindow(rc);
	}
}

void CSpcDlg::InitUiCtrl()
{
	CRect rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
	CRect rcClient;
	GetClientRect(&rcClient);

	int x_sz = rcWorkArea.Width();  
	int y_sz = rcWorkArea.Height();
	::SetWindowPos(m_hWnd,HWND_TOP,0,0,x_sz, y_sz, SWP_SHOWWINDOW);

	InitDlgHeader(rcWorkArea);
	InitChartCtrl(rcWorkArea);
	InitChannelSwitch(rcWorkArea);
	InitTaskComboBox(rcWorkArea);
	if (m_bBatchNumberFun)
	{
		InitShiftBatchNumerComboBox(rcWorkArea);
	}
	else
	{
		InitShiftComboBox(rcWorkArea);
	}

	InitDateTimePicker(rcWorkArea);
	InitQueryReport(rcWorkArea);
	InitTestItemList(rcWorkArea);
}

void CSpcDlg::OnCloseScreen()
{
	//TODO:
	VcAdoSqlserver::DestoryInstance();
	CDialogEx::OnOK();
}

void CSpcDlg::OnNextBtnClicked()
{
	int sz = (int)m_vDefectRatio.size();
	if(sz <= 50)
		return;
	boost::circular_buffer<float> vDataPoint;

	long long nDis = std::distance(m_vDefectRatio.begin() + (m_nHitCount + 1) * 10, m_vDefectRatio.end());
	if(nDis <= 50)
	{
		vDataPoint.assign(m_vDefectRatio.begin() + (m_nHitCount + 1) * 10, m_vDefectRatio.end());
		m_btnNext.SetLabelImage(IMG_CH_RIGHT_ARROW_GRAY);
		m_btnNext.EnableWindow(FALSE);
		RedrawButton(m_btnNext);
	}
	else
		vDataPoint.assign(m_vDefectRatio.begin() + (m_nHitCount*10 + 1) * 10, m_vDefectRatio.begin() + (m_nHitCount + 1) * 10 + 51);
	m_nHitCount++;
	m_chartCtrl.SetXIndex(m_nHitCount);

	m_nHitCount++;
	m_chartCtrl.SetXIndex(m_nHitCount);

	m_chartCtrl.Receive2DLineGraphData(vDataPoint);
	m_chartCtrl.Invalidate();


	m_btnPrevious.SetLabelImage(IMG_CH_LEFT_ARROW);
	m_btnPrevious.EnableWindow(TRUE);
	RedrawButton(m_btnPrevious);

	UpdateData(FALSE);
}

void CSpcDlg::OnPreviousBtnClicked()
{
	boost::circular_buffer<float> vDataPoint;
	if(m_nHitCount == 0)
	{
		m_btnPrevious.SetLabelImage(IMG_CH_LEFT_ARROW_GRAY);
		m_btnPrevious.EnableWindow(FALSE);
		RedrawButton(m_btnPrevious);
		return;
	}
	m_nHitCount--;

	int nLen = (m_nHitCount == 0)? 50 : 51;
	vDataPoint.assign(m_vDefectRatio.begin() + (m_nHitCount * 10), m_vDefectRatio.begin() + (m_nHitCount * 10) + nLen);
	m_chartCtrl.SetXIndex(m_nHitCount);
	m_chartCtrl.Receive2DLineGraphData(vDataPoint);
	m_chartCtrl.Invalidate();

	if(m_nHitCount == 0)
	{
		m_btnPrevious.SetLabelImage(IMG_CH_LEFT_ARROW_GRAY);
		m_btnPrevious.EnableWindow(FALSE);
		RedrawButton(m_btnPrevious);
		return;
	}

	m_btnNext.SetLabelImage(IMG_CH_RIGHT_ARROW);
	m_btnNext.EnableWindow(TRUE);
	RedrawButton(m_btnNext);

	UpdateData(FALSE);
}

void CSpcDlg::ResetQueryReport()
{
	m_nTotalCount = 0;
	m_nNgCount    = 0;
	m_nErrorCount = 0;
	m_nHitCount   = 0;
	m_vDefectRatio.clear();
	m_vItemErrCount.clear();
	m_dictQueryRslt.clear();
}

void CSpcDlg::OnQueryBtnClicked()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CH_TASK);
	int crtIndex = pCombo->GetCurSel();

	ResetUiWidget();
	ResetQueryReport();

	SYSTEMTIME bgn, end, tmpDT;

	m_dateBgn.GetTime(&tmpDT);
	bgn.wYear  = tmpDT.wYear;
	bgn.wMonth = tmpDT.wMonth;
	bgn.wDay   = tmpDT.wDay;

	m_timeBgn.GetTime(&tmpDT);
	bgn.wHour   = tmpDT.wHour;
	bgn.wMinute = tmpDT.wMinute;
	bgn.wSecond = tmpDT.wSecond;

	m_dateEnd.GetTime(&tmpDT);
	end.wYear  = tmpDT.wYear;
	end.wMonth = tmpDT.wMonth;
	end.wDay   = tmpDT.wDay;

	m_timeEnd.GetTime(&tmpDT);
	end.wHour   = tmpDT.wHour;
	end.wMinute = tmpDT.wMinute;
	end.wSecond = tmpDT.wSecond;

	long long lBgn = utils::DateTime2Int64(bgn);
	long long lEnd = utils::DateTime2Int64(end);
	CString sMsg;
	if(lBgn > lEnd)
	{
		sMsg.LoadString(IDS_STRING_QUERY_CONDITION_ERROR);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	if((end.wMonth - bgn.wMonth >= 2) || (end.wMonth - bgn.wMonth == 1 && end.wDay > bgn.wDay))
	{
		sMsg.LoadString(IDS_STRING_QUERY_ERROR_MORE_THAN_ONE_MONTH);
		DisplayPromptMessage(sMsg, false);
		return;
	}

	CString sTaskName;
	int index = m_comboTask.GetCurSel();
	if(index == -1)
	{
		index =0;

// 		sMsg.LoadString(IDS_STRING_PLEASE_SELECT_TASK);
// 		DisplayPromptMessage(sMsg, false);
// 		return;
	}
	m_comboTask.GetLBText(index, sTaskName);
	wstring strTaskName = CT2W(sTaskName);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BATCHNUMBER);

	CString strbatchnumber;
	pCombo->GetWindowText(strbatchnumber);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_USERNAME);
	CString strUserName;
	pCombo->GetWindowText(strUserName);

	/////////////////////////////////////////////////////生产数据统计
	m_TotalSysData.Element.clear();
	gSystemMgr.QueryProductStats(m_crtChIndex, strTaskName, lBgn, lEnd, m_dictQueryRslt,strbatchnumber,strUserName);
	gSystemMgr.QuerySystemProductStats(lBgn, lEnd, m_dictQueryRJectDataRslt,strbatchnumber,strUserName);

	bool bP1 = ParseQueryResult(strTaskName, m_dictQueryRslt, bgn, end); //已经经过批次删选!
	bool bP2 = ParseQueryRjectResult(strTaskName, m_dictQueryRJectDataRslt, bgn, end);

	if (bP1 || bP2)
	{
		if (index <=0)
		{
			GenerateQueryReport_Ex();
		}
		GenerateQueryReport();
		m_btnExport.SetLabelImage(IMG_SPC_EXPORT);
		m_btnExport.EnableWindow(TRUE);
		RedrawButton(m_btnExport);
	}
#ifdef EOE_PRODUCT
	CString strGIIT = GenerateGlueInjectorStats(); // GIIT -- Glue Injector ID Tracing
	if(strGIIT.GetLength()==0)
		strGIIT=_T("0");
	SetDlgItemTextW(IDC_EDIT_GLUE_INJECTOR_STATS, strGIIT);
#endif	
}

#ifdef EOE_PRODUCT
CString CSpcDlg::GenerateGlueInjectorStats()
{
	CString strReport(_T(""));
	std::map<long, long> dictStats = gSystemMgr.GetGlueInjectorErrorStats(m_crtChIndex);
	size_t sz = dictStats.size();
	if(sz > 0)
	{
		std::vector<GIID_STATS> vStats;
		long nTotalErrCount = 0;
		for(auto it = dictStats.begin(); it != dictStats.end(); it++)
		{
			GIID_STATS stats;
			stats.m_nID       = it->first;
			stats.m_nErrCount = it->second;
			nTotalErrCount += stats.m_nErrCount;
			vStats.push_back(stats);
		}
		std::sort(vStats.begin(), vStats.end(), [&](GIID_STATS lhs,GIID_STATS rhs)->bool
		{
			return lhs.m_nErrCount < rhs.m_nErrCount;
		});
		std::reverse(vStats.begin(), vStats.end());

		int szIDs = (sz > 3) ? 3: (int)sz;
		if(nTotalErrCount > 0)
		{
			for(int i = 0;i < szIDs; i++)
			{
				CString str;
				str.Format(_T("#%d(%.1f%%),"),int(vStats[i].m_nID + 1), 1.0f * vStats[i].m_nErrCount/nTotalErrCount);
				strReport.Append(str);
			}
			strReport.Delete(strReport.GetLength() - 1);
		}
	}
	return strReport;
}
#endif

bool CSpcDlg::ParseQueryRjectResult(wstring wstrTaskName, const std::map<long long, SYS_STAT_DATA>& queryRslt, const SYSTEMTIME& bgn, const SYSTEMTIME& end)
{	
	m_nTotalCount2 =0;
	m_nTotalOKCount2 =0;

	long oldtotalCount = 0;
	long oldtotalOkCount = 0;
	long oldleftFlag = 0;
	int i = 0;

	CString m_LastUserName;

	long long m_lasttimestamp=0;
	long m_nTotaltmpCount2 = 0;
	long m_nTotalOKtmpCount2 =0;
	CString m_laststrBatchNumber;
	CString m_laststrUser;
	CString m_lastSysforeignKey;

	for(auto it = queryRslt.begin(); it != queryRslt.end(); it++)
	{
		SYS_STAT_DATA data  = it->second;

		long totalCount = data.m_totalCount;
		long totalOkCount = data.m_totalOkCount;
		int  leftFlag = data.m_leftFlag;
		CString strBatchNumber = data.m_strBatchNumber;
		long long timestamp = data.m_timestamp/1000000;
		CString strUser =  data.m_strUser;
		CString strSysforeignKey = data.m_strSysForeignKey;

		//系统统计数据，当出现 良品数大于 总数的时候，则认为脏数据
		//原因有 第一通道和四通道没有同步清除，此条数据忽略
		if(totalOkCount >totalCount)
		{
			continue;
		}

		int j =0;
		if (timestamp != m_lasttimestamp || strBatchNumber != m_laststrBatchNumber|| strUser != m_laststrUser || strSysforeignKey != m_lastSysforeignKey)
		{
			m_nTotaltmpCount2 = 0;
			m_nTotalOKtmpCount2 =0;
			j=0;
			m_TotalSysData.Element[timestamp].Element[strBatchNumber].Element[strUser].Element[strSysforeignKey].m_timestampBgn =data.m_timestamp;
		}
		if (j == 0)
		{
			//由于排盖板第二个感应器技术延时的问题，第一组数据有可能从990-1005直接波动，
			//从概率的角度上来说，设置小于1005为第一组计数值相对比较合理！
			if (totalCount<=1005) 
			{
				m_nTotaltmpCount2 += totalCount;
				m_nTotalOKtmpCount2 += totalOkCount;
			}
		}

		if (i == 0)
		{
			//由于排盖板第二个感应器技术延时的问题，第一组数据有可能从990-1005直接波动，
			//从概率的角度上来说，设置小于1005为第一组计数值相对比较合理！
			if (totalCount<=1005) 
			{
				m_nTotalCount2 += totalCount;
				m_nTotalOKCount2 += totalOkCount;

				m_nTotaltmpCount2 = totalCount;
				m_nTotalOKtmpCount2 = totalOkCount;
			}
		}
		else
		{
			if (oldleftFlag == -1)
			{
				m_nTotalCount2 += totalCount;
				m_nTotalOKCount2 += totalOkCount;

				m_nTotaltmpCount2 = totalCount;
				m_nTotalOKtmpCount2 = totalOkCount;
			}
			else
			{
				if (totalCount <= oldtotalCount )
				{
					m_nTotalCount2 += totalCount;
					m_nTotalOKCount2 += totalOkCount;

					m_nTotaltmpCount2 = totalCount;
					m_nTotalOKtmpCount2 = totalOkCount;
				}
				else
				{
					m_nTotalCount2 = m_nTotalCount2 + totalCount - oldtotalCount;
					m_nTotalOKCount2 = m_nTotalOKCount2 + totalOkCount - oldtotalOkCount;

					m_nTotaltmpCount2 = totalCount - oldtotalCount;
					m_nTotalOKtmpCount2 = totalOkCount - oldtotalOkCount;

				}
			}
		}

		oldtotalCount = totalCount;
		oldtotalOkCount = totalOkCount;
		oldleftFlag = leftFlag;

		m_TotalSysData.Element[timestamp].Element[strBatchNumber].Element[strUser].Element[strSysforeignKey].m_totalCount +=m_nTotaltmpCount2;
		m_TotalSysData.Element[timestamp].Element[strBatchNumber].Element[strUser].Element[strSysforeignKey].m_totalOkCount +=m_nTotalOKtmpCount2;
		m_TotalSysData.Element[timestamp].Element[strBatchNumber].Element[strUser].Element[strSysforeignKey].m_timestampEnd =data.m_timestamp;
		m_TotalSysData.Element[timestamp].Element[strBatchNumber].Element[strUser].Element[strSysforeignKey].m_sysforeignkey =data.m_strSysForeignKey;

		m_lasttimestamp = timestamp;
		m_laststrBatchNumber = strBatchNumber;
		m_laststrUser = strUser;
		m_lastSysforeignKey = strSysforeignKey;
		i++;
	}
	return true;
}

bool CSpcDlg::ParseQueryResult(wstring wstrTaskName, const std::map<long long, SPC_DATA_PT>& queryRslt, const SYSTEMTIME& bgn, const SYSTEMTIME& end)
{
	if (wstrTaskName.empty())
	{
		return false;
	}
	if(queryRslt.empty()) return false;

	std::vector<std::map<long long, SPC_DATA_PT>> vSegment;
	std::map<long long, SPC_DATA_PT> dictTmp;

	for(auto it = queryRslt.begin(); it != queryRslt.end(); it++)
	{
		dictTmp[it->first] = it->second;
		if(it->second.m_fNgRatio == -1.0f)
		{
			vSegment.push_back(dictTmp);
			dictTmp.clear();
		}
	}
	if(!dictTmp.empty())
	{
		vSegment.push_back(dictTmp);
		dictTmp.clear();
	}
	if(vSegment.empty()) return false;

	auto itBgn      = vSegment.begin();
	int nItemCount  = int(itBgn->begin()->second.m_vItemStats.size());
	m_vItemErrCount = std::vector<long>(nItemCount, 0);

	CHANNEL_CONFIG chConfig = gConfigMgr.GetChannelConfig(m_crtChIndex);
	std::wstring strUnitX = chConfig.m_spcSetting.m_strUnitX;
	std::wstring strSampleSz;
	strSampleSz.assign(strUnitX.begin() + 1, strUnitX.end());
	int nSampleSz = boost::lexical_cast<int>(strSampleSz);

	for(int i = 0; i < vSegment.size(); i++)
	{
		std::map<long long, SPC_DATA_PT> dict = vSegment[i];
		if(dict.empty()) continue;

		auto first = dict.begin();
		auto last = dict.end(); last--;

		for(auto it = dict.begin(); it != dict.end(); it++)
		{
			if(it->second.m_fNgRatio != -1.0f)
				m_vDefectRatio.push_back(it->second.m_fNgRatio);

			std::vector<long> vItemErrCount;
			std::for_each(it->second.m_vItemStats.begin(), it->second.m_vItemStats.end(), [&](ITEM_STATS& itStats)
			{
				vItemErrCount.push_back(itStats.m_nItemErrorCount);
			});
			std::transform(m_vItemErrCount.begin(), m_vItemErrCount.end(), vItemErrCount.begin(), m_vItemErrCount.begin(), std::plus<long>());
		}

		if(first->second.m_totalImgCount / nSampleSz <= 1)
		{
			m_nTotalCount += last->second.m_totalImgCount;
			m_nNgCount    += last->second.m_totalNgImgCount;
			m_nErrorCount += last->second.m_totalErrCount;
		}
		if(first->second.m_totalImgCount / nSampleSz >= 2)
		{
			m_nTotalCount += last->second.m_totalImgCount   - first->second.m_totalImgCount;
			m_nNgCount    += last->second.m_totalNgImgCount - first->second.m_totalNgImgCount;
			m_nErrorCount += last->second.m_totalErrCount   - first->second.m_totalErrCount;
		}
	}

	return true;
}
//工号相关统计输出
void CSpcDlg::GenerateQueryReport_Ex()
{
	int index =0;
	m_ListTestItem.DeleteAllItems();
	for (auto it1= m_TotalSysData.Element.begin() ; it1 != m_TotalSysData.Element.end();it1++)
	{
		CString strtimestamp;
		strtimestamp.Format(_T("%d"),it1->first);
		SYS_STAT_MAP3 m_TotalSysData2= it1->second;
		for (auto it2= m_TotalSysData2.Element.begin(); it2 != m_TotalSysData2.Element.end();it2++)
		{
			CString strBatchnumber = it2->first;
			SYS_STAT_MAP2 m_TotalSysData3 = it2->second;
			for (auto it3 = m_TotalSysData3.Element.begin();it3!= m_TotalSysData3.Element.end();it3++)
			{
				CString user = it3->first;
				SYS_STAT_MAP1 m_TotalSysData3 = it3->second;

				for (auto it4 = m_TotalSysData3.Element.begin();it4!= m_TotalSysData3.Element.end();it4++)
				{
					CString SysforeignKey = it4 ->first;
					SYS_STAT_DATA_EX m_data= it4->second;


					CString strIndex;
					strIndex.Format(_T("%d"),index);
					m_ListTestItem.InsertItem(index,strIndex);

					//时间段，用户，批次号
					m_ListTestItem.SetItemText(index, 1, strtimestamp);
					m_ListTestItem.SetItemText(index, 2, user);
					m_ListTestItem.SetItemText(index, 3, strBatchnumber);

					//开始时间 结束时间
					CString strtimestampBgn;
					strtimestampBgn.Format(_T("%.6d"),m_data.m_timestampBgn%1000000);
					m_ListTestItem.SetItemText(index,4,strtimestampBgn);

					CString strtimestampEnd;
					strtimestampEnd.Format(_T("%.6d"),m_data.m_timestampEnd%1000000);
					m_ListTestItem.SetItemText(index,5,strtimestampEnd);


					//总数，合格数，合格率
					CString strTotal;
					strTotal.Format(_T("%d"),m_data.m_totalCount);

					CString strTotalOk;
					strTotalOk.Format(_T("%d"),m_data.m_totalOkCount);

					CString strTotalOkRate;
					strTotalOkRate.Format(_T("%.3f"),(float)m_data.m_totalOkCount/m_data.m_totalCount);

					m_ListTestItem.SetItemText(index, 6, strTotal);
					m_ListTestItem.SetItemText(index, 7, strTotalOk);
					m_ListTestItem.SetItemText(index, 8, strTotalOkRate);

					std::vector<CString> m_vec;
					gSystemMgr.GetCurSysForeignKeyInfo(SysforeignKey,m_vec);

					int ID = 9;
					if (m_vec.size()>=2)
					{
						for(int i = 1 ; i < m_vec.size();i++)
						{
							m_ListTestItem.SetItemText(index, ID+i-1,m_vec[i]);
						}
					}

					COLORREF crtColor = (index % 2 == 0)? m_color1 : m_color2;
					m_ListTestItem.SetItemColor(index, RGB(0,0,0),crtColor);
					index++;
				}
			}
		}
	}
}
void CSpcDlg::GenerateQueryReport()
{
	CString strValue;
	strValue.Format(_T("%d"), m_nTotalCount);
	SetDlgItemTextW(IDC_EDIT_TOTAL, strValue);

	strValue.Format(_T("%d"), m_nNgCount);
	SetDlgItemTextW(IDC_EDIT_DEFECT, strValue);

	if(m_nTotalCount != 0)
		strValue.Format(_T("%.3f%%"), (float)100.0f * m_nNgCount/m_nTotalCount);
	else
		strValue = _T("0.00%");
	SetDlgItemTextW(IDC_EDIT_DEFECT_RATE, strValue);

	strValue.Format(_T("%d"), m_nErrorCount);
	SetDlgItemTextW(IDC_EDIT_ERROR_COUNT, strValue);

	if(m_nNgCount != 0)
		strValue.Format(_T("%.3f"), (float)m_nErrorCount / m_nNgCount);
	else
		strValue = _T("0");
	SetDlgItemTextW(IDC_EDIT_ERROR_PER_ONE, strValue);

	//系统统计数据
	
		strValue.Format(_T("%d"), m_nTotalCount2);
	SetDlgItemTextW(IDC_EDIT_TOTAL_SYS, strValue);

	strValue.Format(_T("%d"), m_nTotalCount2-m_nTotalOKCount2);
	SetDlgItemTextW(IDC_EDIT_DEFECT_SYS, strValue);

	if(m_nTotalCount2 != 0)
		strValue.Format(_T("%.3f%%"), 100*(float)(m_nTotalCount2-m_nTotalOKCount2) / m_nTotalCount2);
	else
		strValue = _T("0");
	SetDlgItemTextW(IDC_EDIT_DEFECT_RATE_SYS, strValue);

	int index = m_comboTask.GetCurSel();
	if ( index <=0 )
	{
		return;
	}

	// Update Test Item List
	CString sErrCount, sErrRatio;
	for(size_t t = 0; t < m_vItemErrCount.size(); t++)
	{
		long nErrCount = m_vItemErrCount[t];
		sErrCount.Format(_T("%d"), nErrCount);
		if(m_nTotalCount != 0)
			sErrRatio.Format(_T("%.3f"), 100.0f * nErrCount / m_nTotalCount);
		else
			sErrRatio = _T("0.00");
		m_ListTestItem.SetItemText((int)t, 2, sErrCount);
		m_ListTestItem.SetItemText((int)t, 3, sErrRatio);
	}
	// Update Statistics Chart
	boost::circular_buffer<float> vDataPoint;
	if(m_vDefectRatio.size() <= 50)
	{
		vDataPoint.assign(m_vDefectRatio.begin(), m_vDefectRatio.end());
	}
	else
	{
		m_btnNext.SetLabelImage(IMG_CH_RIGHT_ARROW);
		m_btnNext.EnableWindow(TRUE);
		RedrawButton(m_btnNext);

		vDataPoint.assign(m_vDefectRatio.begin(), m_vDefectRatio.begin() + 50);
	}

	m_chartCtrl.Receive2DLineGraphData(vDataPoint);
	m_chartCtrl.Invalidate();

	m_btnExport.SetLabelImage(IMG_SPC_EXPORT);
	m_btnExport.EnableWindow(TRUE);
	RedrawButton(m_btnExport);

	UpdateData(FALSE);
}

void CSpcDlg::OnExportBtnClicked()
{
	if (m_bUpdateDataBaseFun)
	{
		if (m_crtChIndex == 2)
		{
			//MessageBox(_T("此通道不支持数据上传"));
			//return;
		}

		//数据库不导出
	int index = m_comboTask.GetCurSel();
	if(index > 0)
	{


			SYSTEMTIME bgn, end, tmpDT;

			m_dateBgn.GetTime(&tmpDT);
			bgn.wYear  = tmpDT.wYear;
			bgn.wMonth = tmpDT.wMonth;
			bgn.wDay   = tmpDT.wDay;

			m_timeBgn.GetTime(&tmpDT);
			bgn.wHour   = tmpDT.wHour;
			bgn.wMinute = tmpDT.wMinute;
			bgn.wSecond = tmpDT.wSecond;
			bgn.wMilliseconds = 0;     //查询时间精确到秒，毫秒级忽略

			m_dateEnd.GetTime(&tmpDT);
			end.wYear  = tmpDT.wYear;
			end.wMonth = tmpDT.wMonth;
			end.wDay   = tmpDT.wDay;

			m_timeEnd.GetTime(&tmpDT);
			end.wHour   = tmpDT.wHour;
			end.wMinute = tmpDT.wMinute;
			end.wSecond = tmpDT.wSecond;
			end.wMilliseconds = 0;    //查询时间精确到秒，毫秒级忽略

			long long lBgn = utils::DateTime2Int64(bgn);
			long long lEnd = utils::DateTime2Int64(end);
			CString strIbgn;
			CString strIEnd;
			CString sMsg;
			if(lBgn > lEnd)
			{
				sMsg.LoadString(IDS_STRING_QUERY_CONDITION_ERROR);
				DisplayPromptMessage(sMsg, false);
				return;
			}

			if((end.wMonth - bgn.wMonth >= 2) || (end.wMonth - bgn.wMonth == 1 && end.wDay > bgn.wDay))
			{
				sMsg.LoadString(IDS_STRING_QUERY_ERROR_MORE_THAN_ONE_MONTH);
				DisplayPromptMessage(sMsg, false);
				return;
			}

			CString sTaskName;
			index = m_comboTask.GetCurSel();
			if(index == -1)
			{
				sMsg.LoadString(IDS_STRING_PLEASE_SELECT_TASK);
				DisplayPromptMessage(sMsg, false);
				return;
			}
			m_comboTask.GetLBText(index, sTaskName);
			wstring strTaskName = CT2W(sTaskName);

			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_BATCHNUMBER);

			CString strbatchnumber;
			pCombo->GetWindowText(strbatchnumber);

			//获取通道数目
			auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
			int chCount =  (int)pChConfigArray->size();

			std::map<CString, long long> m_MapErrorTotalCount;
			std::map<CString, int > m_MapErrorItemCount;
			vector<CString> m_vecColumnName;
			CString ProductName;
			CString str;
			str.Format(_T("当前任务:%s\n    批次号:%s\n起始时间:%s\n结束时间:%s"),sTaskName,strbatchnumber,utils::DateTime2CString(bgn),utils::DateTime2CString(end));
			CDialogUpdateDatabase dlg;
			dlg.SetStrInfo(str);
			if (dlg.DoModal()!=IDOK )
			{
				return;
			}
			UpdateData(TRUE);

			DATA_BASE_DATA databasedata;
			databasedata.Recheck = dlg.GetRecheck();
			databasedata.Reserved1 = dlg.GetReserved1();

			UAC crtUser = gSystemMgr.GetCurrentUser();
			databasedata.UserName = crtUser.m_strUsername.c_str();

			for (int curChIndex = 1; curChIndex <= chCount; curChIndex++)
			{
				//todo:strTaskName
				int CameraCount = (*pChConfigArray)[curChIndex].m_vecCameraSetting.size();

				CString strTaskNameNew;
				gSystemMgr.FindCHFullTaskName(curChIndex,strTaskName,strTaskNameNew);

				if (curChIndex== 2)
				{
					//gSystemMgr.GetProductNameFromCH1(strTaskNameNew,ProductName);
					ProductName = strTaskNameNew; //add by hzh
				}

				//查询列表项
				if (strTaskNameNew.IsEmpty())
				{
					continue;
				}
				m_dictQueryRslt.clear();
				gSystemMgr.QueryProductColumn(curChIndex, strTaskNameNew.GetString(),m_vecColumnName);
				//查询和处理统计数据
				gSystemMgr.QueryProductStats(curChIndex, strTaskNameNew.GetString(), lBgn, lEnd, m_dictQueryRslt,strbatchnumber);
				bool bP1 = ParseQueryResult(strTaskNameNew.GetString(), m_dictQueryRslt, bgn, end); //已经经过批次删选!
				if (!bP1)
				{
					continue;
				}
				//处理关联缺陷=》黑点检测(黑点)
				for (int i = 0 ; i < m_vecColumnName.size();i++)
				{
					//	std::map<CString, int > m_MapErrorItemCount;
					CString TestAgloName=m_vecColumnName[i];
					int Ti = TestAgloName.Find(_T("_"));
					int Tj = TestAgloName.Find(_T("检测"));
					if (Tj!=-1)//&&(m_vecColumnName.size() >= m_vItemErrCount.size()+7))
					{
						CString testname;
						if (Tj>Ti)
						{
							if (Ti !=-1)
							{
								testname  = TestAgloName.Mid(Ti+1,Tj-Ti-1);
							}
							else
							{
								testname  = TestAgloName.Mid(0,Tj-Ti-1);
							}
						}
						else
						{
							if (Ti !=-1)
							{
								testname  = TestAgloName.Mid(Ti+1,TestAgloName.GetLength()-Ti-1);
							}
							else
							{
								testname  = TestAgloName.Mid(0,TestAgloName.GetLength()-Ti-1);
							}
						}
						m_MapErrorTotalCount[testname] +=m_vItemErrCount[i-7];
						m_MapErrorItemCount[testname]  +=1;//CameraCount;
				
					}
				}
			}

			for (auto it = m_MapErrorItemCount.begin(); it!= m_MapErrorItemCount.end();it++)
			{
				databasedata.m_mapErrorCount[it->first] = m_MapErrorTotalCount[it->first];
			}

			gSystemMgr.QuerySystemProductStats(lBgn, lEnd, m_dictQueryRJectDataRslt,strbatchnumber);
			bool bP2 = ParseQueryRjectResult(strTaskName, m_dictQueryRJectDataRslt, bgn, end);

			databasedata.ProductName = ProductName;
			databasedata.ProductBatch = strbatchnumber;
			databasedata.StartTime=utils::DateTime2CString(bgn);
			databasedata.EndTime=utils::DateTime2CString(end);
			databasedata.Total = m_nTotalCount2;
			databasedata.Qualified = m_nTotalOKCount2;
			databasedata.Rejected = m_nTotalCount2-m_nTotalOKCount2;
			if (m_nTotalCount2!=0)
			{
				databasedata.Passrate = (double)m_nTotalOKCount2/m_nTotalCount2;
			}

			if(VcAdoSqlserver::GetInstance()->UpdateDataBaseData(databasedata))
			{
				AfxMessageBox(_T("上传成功!"));
			}
			return;
		}
	}
////////////////////////////////////

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CH_TASK);
	int crtIndex = pCombo->GetCurSel();

// 	if(m_dictQueryRslt.empty() || m_nTotalCount == 0)
// 		return;

	TCHAR sPath[MAX_PATH];
	SHGetSpecialFolderPath(0, sPath, CSIDL_DESKTOPDIRECTORY, 0);
	CString strPath = sPath;

	SYSTEMTIME dt;
	::GetLocalTime(&dt);
	CString strDate;
	strDate.Format(_T("query report_%d-%02d-%02d-%02d-%02d-%02d"), dt.wYear, dt.wMonth, dt.wDay,dt.wHour, dt.wMinute, dt.wSecond);
	CString sFile = strPath + _T("\\") + strDate + _T(".csv");

	FILE* fp = nullptr;
	wstring strFile = CT2W(sFile);
	_wfopen_s(&fp, strFile.c_str(), _T("a"));

	::setlocale(LC_ALL, "chinese-simplified");

	//导出扩展
	if (crtIndex <=0)
	{
		CString strValue(_T("序号,日期,工号,批次号,开始时间,结束时间,总数,合格数,合格率"));
		std::vector<CString> m_vec;
		if (gSystemMgr.GetProductInfoVecter(m_vec))
		{
			for(int i = 0 ; i < m_vec.size();i++)
			{
				if (i!=m_vec.size())
				{
					strValue+=",";
				}
				strValue+=m_vec[i];
			}
		}
		strValue+=_T("\n");
		wstring wstrValue = CT2W(strValue);
		fputws(wstrValue.c_str(), fp);
		int index=0;
		for (auto it1= m_TotalSysData.Element.begin() ; it1 != m_TotalSysData.Element.end();it1++)
		{
			CString strtimestamp;
			strtimestamp.Format(_T("%d"),it1->first);
			SYS_STAT_MAP3 m_TotalSysData2= it1->second;
			for (auto it2= m_TotalSysData2.Element.begin(); it2 != m_TotalSysData2.Element.end();it2++)
			{
				CString strBatchnumber = it2->first;
				SYS_STAT_MAP2 m_TotalSysData3 = it2->second;
				for (auto it3 = m_TotalSysData3.Element.begin();it3!= m_TotalSysData3.Element.end();it3++)
				{
					CString user = it3->first;
					SYS_STAT_MAP1 m_TotalSysData4 = it3->second;

						for (auto it4 = m_TotalSysData4.Element.begin() ; it4!= m_TotalSysData4.Element.end();it4++)
						{
							SYS_STAT_DATA_EX m_data = it4->second;

							CString strIndex;
							strIndex.Format(_T("%d"),index);
				
							//开始时间 结束时间
							CString strtimestampBgn;
							strtimestampBgn.Format(_T("%.6d"),m_data.m_timestampBgn%1000000);
							CString strtimestampEnd;
							strtimestampEnd.Format(_T("%.6d"),m_data.m_timestampEnd%1000000);


							//总数，合格数，合格率
							CString strTotal;
							strTotal.Format(_T("%d"),m_data.m_totalCount);

							CString strTotalOk;
							strTotalOk.Format(_T("%d"),m_data.m_totalOkCount);

							CString strTotalOkRate;
							strTotalOkRate.Format(_T("%.3f"),(float)m_data.m_totalOkCount/m_data.m_totalCount);

							CString strSysforeignKey = m_data.m_sysforeignkey;

							//时间段，用户，批次号
							CString strData;
							strData.Format(_T("%s,%s,%s,%s,%s,%s,%s,%s,%s"),strIndex
																										,strtimestamp
																										,user
																										,strBatchnumber
																										,strtimestampBgn
																										,strtimestampEnd
																										,strTotal
																										,strTotalOk
																										,strTotalOkRate);
							wstring wstrData = strData.GetString();
							fputws(wstrData.c_str(), fp);

							std::vector<CString> m_vec;
							CString strValue;

							gSystemMgr.GetCurSysForeignKeyInfo(strSysforeignKey,m_vec);
							if (m_vec.size()>=1)
							{
								for(int i = 1 ; i < m_vec.size();i++)
								{
									if (i!=m_vec.size())
									{
										strValue+=",";
									}
									strValue+=m_vec[i];
								}

							}

							strValue+=_T("\n");
							wstrData = strValue.GetString();
							fputws(wstrData.c_str(), fp);

							index++;
					}
				}
			}
		}
		fclose(fp);

		CString sMsg;
		sMsg.LoadString(IDS_STRING_EXPORT_FILE_SUCCEEDED);
		CString sFileName = strDate + _T(".csv");
		sMsg.Replace(_T("#1"), sFileName); 
		AfxMessageBox(sMsg, MB_OK, NULL);

		return;
	}
	// Task Name
	CString str;
	str.LoadString(IDS_STRING_TASK_CANDIDATE);
	wstring strValue = CT2W(str);
	strValue.append(L",");
	strValue.append(m_strCrtTaskName);
	strValue.append(L",\r\n");
	fputws(strValue.c_str(), fp);
	// Query Datetime
	// Begin datetime
	str.LoadString(IDS_STRING_BEGIN_DATETIME);
	strValue = CT2W(str);
	strValue.append(L",");

	CString strDatetime;
	m_dateBgn.GetWindowText(strDatetime);
	strValue.append(CT2W(strDatetime));
	strValue.append(L" - ");
	m_timeBgn.GetWindowText(strDatetime);
	strValue.append(CT2W(strDatetime));
	strValue.append(L",\r\n");
	fputws(strValue.c_str(), fp);
	// End datetime
	str.LoadString(IDS_STRING_END_DATETIME);
	strValue = CT2W(str);
	strValue.append(L",");

	m_dateEnd.GetWindowText(strDatetime);
	strValue.append(CT2W(strDatetime));
	strValue.append(L" - ");
	m_timeEnd.GetWindowText(strDatetime);
	strValue.append(CT2W(strDatetime));
	strValue.append(L",\r\n");
	fputws(strValue.c_str(), fp);

	// Query report
	str.LoadString(IDS_STRING_TOTAL);
	strValue = CT2W(str);
	strValue.append(L",");
	wstring strCount = boost::lexical_cast<wstring>(m_nTotalCount);
	strValue.append(strCount);
	strValue.append(L",\r\n");
	fputws(strValue.c_str(), fp);

	str.LoadString(IDS_STRING_FAILED);
	strValue = CT2W(str);
	strValue.append(L",");
	strCount = boost::lexical_cast<wstring>(m_nNgCount);
	strValue.append(strCount);
	strValue.append(L",\r\n");
	fputws(strValue.c_str(), fp);
	// NG ratio
	str.LoadString(IDS_STRING_RATE);
	strValue = CT2W(str);
	strValue.append(L",");
	CString strRatio;
	strRatio.Format(_T("%.2f‰"), m_nNgCount*1000.0f / m_nTotalCount);
	std::wstring strTmp = CT2W(strRatio);
	strValue.append(strTmp);
	strValue.append(L",\r\n");
	fputws(strValue.c_str(), fp);
	// Error Count;
	str.LoadString(IDS_STRING_REJECT_COUNT);
	strValue = CT2W(str);
	strValue.append(L",");
	strCount = boost::lexical_cast<wstring>(m_nErrorCount);
	strValue.append(strCount);
	strValue.append(L",\r\n");
	fputws(strValue.c_str(), fp);
	// Error Count / One Cap
	str.LoadString(IDS_STRING_ERROR_PER_ONE);
	strValue = CT2W(str);
	strValue.append(L",");
	strRatio.Format(_T("%.2f"), m_nErrorCount*1.0f / m_nNgCount);
	strTmp = CT2W(strRatio);
	strValue.append(strTmp);
	strValue.append(L",\r\n");
	fputws(strValue.c_str(), fp);

	// Item Test List
	wstring strTitle;
	CString strIndex;
	strIndex.LoadString(IDS_STRING_INDEX);
	CString strAlgoName;
	strAlgoName.LoadString(IDS_STRING_ALGO_NAME);
	CString strErrorCount;
	strErrorCount.LoadString(IDS_STRING_REJECT_COUNT);
	CString strErrorRate;
	strErrorRate.LoadString(IDS_STRING_ERROR_RATE);
	strTitle.append(CT2W(strIndex));      strTitle.append(L",");
	strTitle.append(CT2W(strAlgoName));   strTitle.append(L",");
	strTitle.append(CT2W(strErrorCount)); strTitle.append(L",");
	strTitle.append(CT2W(strErrorRate));  strTitle.append(L",\r\n");
	fputws(strTitle.c_str(), fp);

	int sz1 = (int)m_vItemName.size();
	int sz2 = (int)m_vItemErrCount.size();
	if(sz1 == sz2)
	{
		CString strRatio;
		for(int i = 0; i < sz1; i++)
		{
			wstring strName = m_vItemName[i];
			std::vector<wstring> vStr;
			if(strName.find(L"#") != wstring::npos)
			{
				vStr = utils::SplitString(strName, L"#");
			}
			wstring strRow;
			if(vStr.size() == 2)
			{
				strRow.append(vStr[0]); strRow.append(L",");
				strRow.append(vStr[1]); strRow.append(L",");

				strValue = boost::lexical_cast<wstring>(m_vItemErrCount[i]);
				strRow.append(strValue), strRow.append(L",");

				float fRatio = m_vItemErrCount[i] * 100.0f/ m_nTotalCount;
				if(fRatio > 100.0f) 
					fRatio = 100.0f;
				strRatio.Format(_T("%.2f"), fRatio);
				strRow.append(CT2W(strRatio));
				strRow.append(L",\r\n");
				fputws(strRow.c_str(), fp);
			}
			
		}
	}
	fclose(fp);

	CString sMsg;
	sMsg.LoadString(IDS_STRING_EXPORT_FILE_SUCCEEDED);
	CString sFileName = strDate + _T(".csv");
	sMsg.Replace(_T("#1"), sFileName); 
	AfxMessageBox(sMsg, MB_OK, NULL);

	m_btnExport.SetLabelImage(IMG_SPC_EXPORT_GRAY);
	m_btnExport.EnableWindow(FALSE);
	RedrawButton(m_btnExport);

}

HBRUSH CSpcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		auto it = std::find(m_vCtrlID.begin(), m_vCtrlID.end(), id);
		if(id == IDC_STATIC_START_DATETIME || 
		   id == IDC_STATIC_END_DATETIME || 
		   id == IDC_STATIC_TASK_NAME ||
		   id == IDC_STATIC_USERNAME ||
		   id == IDC_STATIC_SHIFT_OPTION ||
		   id == IDC_STATIC_BATCH_NUMBER ||
		   id == IDC_STATIC_REPORT_GROUP ||
		   id == IDC_STATIC_TOTAL ||
		   id == IDC_STATIC_DEFECT ||
		   id == IDC_STATIC_TOTAL_SYS ||
		   id == IDC_STATIC_DEFECT_SYS ||
		   id == IDC_STATIC_DEFECT_RATE ||
		   id == IDC_STATIC_DEFECT_RATE_SYS ||
		   id == IDC_STATIC_ERROR ||
		   id == IDC_STATIC_ERROR_PER_ONE ||
		   id == IDC_STATIC_MSG_PANEL ||
		   id == IDC_STATIC_PRODUCTION_SPEED ||
		   id == IDC_STATIC_MAX_SPEED || 
		   id == IDC_STATIC_GLUE_INJECTOR_STATS ||
		   it != m_vCtrlID.end())
		{
			pDC->SetBkMode(TRANSPARENT);
			CRect rc;
			pWnd->GetWindowRect(rc);
			ScreenToClient(&rc);
			CDC* dc = GetDC();
			pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
			if(id == IDC_STATIC_MSG_PANEL)
			{
				if(m_bOpRslt)
					pDC->SetTextColor(RGB(56,200,15));
				else
					pDC->SetTextColor(RGB(200,20,20));
			}
			else
			{
				pDC->SetTextColor(RGB(250,250,250));
			}
			ReleaseDC(dc);
			return HBRUSH(GetStockObject(NULL_BRUSH));
		}
	}
	return hbr;
}

void CSpcDlg::OnCbnSelchangeComboChTask()
{
	m_ListTestItem.DeleteAllItems();

	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	CHANNEL_CONFIG chConfig = (*pChConfigArray)[m_crtChIndex];

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_CH_TASK);
	int crtIndex = pCombo->GetCurSel();

#ifdef EOE_PRODUCT
	if (crtIndex == 0)
	{
		return;
	}
#endif
		//重置m_listTestItem标题栏
	CHeaderCtrl * pHeader = (CHeaderCtrl *)m_ListTestItem.GetHeaderCtrl();
	int nCount = pHeader->GetItemCount();
	for(int i=0;i<nCount;i++)
	{
	m_ListTestItem.DeleteColumn(0);
	}

	if (crtIndex == 0)
	{
		m_ListTestItem.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 65);
		m_ListTestItem.InsertColumn(1, _T("日期"), LVCFMT_CENTER, 100);
		m_ListTestItem.InsertColumn(2, _T("工号"), LVCFMT_CENTER, 120);
		m_ListTestItem.InsertColumn(3, _T("批次号"), LVCFMT_CENTER, 120);
		m_ListTestItem.InsertColumn(4, _T("开始时间"), LVCFMT_CENTER, 140);
		m_ListTestItem.InsertColumn(5, _T("结束时间"), LVCFMT_CENTER, 140);
		m_ListTestItem.InsertColumn(6, _T("总数"), LVCFMT_CENTER, 120);
		m_ListTestItem.InsertColumn(7, _T("合格数"), LVCFMT_CENTER, 120);
		m_ListTestItem.InsertColumn(8, _T("合格率"), LVCFMT_CENTER, 120);
		int index = 9;
		std::vector<CString> m_vec;
		if (gSystemMgr.GetProductInfoVecter(m_vec))
		{
			for(int i = 0 ; i < m_vec.size();i++)
			{
				m_ListTestItem.InsertColumn(index+i, m_vec[i], LVCFMT_LEFT, 120);
			}
		}
	}
	else
	{
		CString strIndex(MAKEINTRESOURCE(IDS_STRING_INDEX));
		CString strAlgoName(MAKEINTRESOURCE(IDS_STRING_ALGO_NAME));
		CString strErrorCount(MAKEINTRESOURCE(IDS_STRING_ERROR_COUNT));
		CString strErrorRate(MAKEINTRESOURCE(IDS_STRING_ERROR_RATE));
		CString strDisplayLine(MAKEINTRESOURCE(IDS_STRING_DISPLAY_GRAPH));
		CString strLineColor(MAKEINTRESOURCE(IDS_STRING_GRAPH_COLOR));

		m_ListTestItem.InsertColumn(0, strIndex, LVCFMT_CENTER, 65);
		m_ListTestItem.InsertColumn(1, strAlgoName, LVCFMT_LEFT, 300);
		m_ListTestItem.InsertColumn(2, strErrorCount, LVCFMT_LEFT, 160);
		m_ListTestItem.InsertColumn(3, strErrorRate, LVCFMT_LEFT, 160);
		m_ListTestItem.InsertColumn(4, strDisplayLine, LVCFMT_LEFT, 150);
		m_ListTestItem.InsertColumn(5, strLineColor, LVCFMT_LEFT, 150);
	}

	/////
	CString sTaskName;
	pCombo->GetLBText(crtIndex,sTaskName);
	m_strCrtTaskName = CT2W(sTaskName);

	TASK crtTask = chConfig.GetTask(m_strCrtTaskName);
	FillTestItemList(crtTask);

	m_BatchNumberCombo.ResetContent();
	FillBatchNumberComboBox();

	ResetUiWidget();
	UpdateData(FALSE);

}

void CSpcDlg::SwitchToChannel(int chIndex)
{
	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	if(pChConfigArray->empty())
		return;

	if(chIndex > (int)pChConfigArray->size())
		return;

	m_crtChIndex = chIndex;

	FillTaskCombo(m_crtChIndex);

	m_ListTestItem.DeleteAllItems();
	CHANNEL_CONFIG chConfig = (*pChConfigArray)[m_crtChIndex];
	TASK crtTask;
	chConfig.GetCurrentTask(crtTask);
	if(crtTask.m_bSelfLearning)
		return;
	FillTestItemList(crtTask);

	UpdateData(FALSE);
}

void CSpcDlg::OnBnClickedRadioCh1()
{
	UpdateData(TRUE);
	ResetUiWidget();
	SwitchToChannel(m_nChRadioBtn + 1);
}

void CSpcDlg::OnBnClickedRadioCh2()
{
	UpdateData(TRUE);
	ResetUiWidget();
	SwitchToChannel(m_nChRadioBtn + 1);
}

void CSpcDlg::OnBnClickedRadioCh3()
{
	UpdateData(TRUE);
	ResetUiWidget();
	SwitchToChannel(m_nChRadioBtn + 1);
}

void CSpcDlg::OnBnClickedRadioCh4()
{
	UpdateData(TRUE);
	ResetUiWidget();
	SwitchToChannel(m_nChRadioBtn + 1);
}

void CSpcDlg::OnBnClickedRadioCh5()
{
	UpdateData(TRUE);
	ResetUiWidget();
	SwitchToChannel(m_nChRadioBtn + 1);
}

void CSpcDlg::OnBnClickedRadioCh6()
{
	UpdateData(TRUE);
	ResetUiWidget();
	SwitchToChannel(m_nChRadioBtn + 1);
}

void CSpcDlg::OnBnClickedRadioCh7()
{
	UpdateData(TRUE);
	ResetUiWidget();
	SwitchToChannel(m_nChRadioBtn + 1);
}

void CSpcDlg::OnBnClickedRadioCh8()
{
	UpdateData(TRUE);
	ResetUiWidget();
	SwitchToChannel(m_nChRadioBtn + 1);
}

void CSpcDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_MSG_PANEL);
	pWnd->ShowWindow(SW_HIDE);
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_MSG_PANEL, sMsg);
	pWnd->ShowWindow(SW_SHOW);
	SetTimer(MSG_DISP_TIMER_ID, 8000, NULL);
}

void CSpcDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == MSG_DISP_TIMER_ID)
	{
		SetDlgItemTextW(IDC_STATIC_MSG_PANEL, _T(""));
	    CWnd* pWnd = GetDlgItem(IDC_STATIC_MSG_PANEL);
		CRect rc;
		pWnd->GetWindowRect(rc);
		InvalidateRect(rc);
		KillTimer(MSG_DISP_TIMER_ID);
	}
	CDialogEx::OnTimer(nIDEvent);
	UpdateData(FALSE);
}

void CSpcDlg::RedrawButton(CLabelCtrl& btn)
{
	CRect rc;
	btn.GetWindowRect(rc);
	InvalidateRect(rc, 0);
}

void CSpcDlg::ResetUiWidget()
{
	boost::circular_buffer<float> dataBuffer; // Empty data buffer;
	m_chartCtrl.Receive2DLineGraphData(dataBuffer);
	m_chartCtrl.Invalidate();

	SetDlgItemTextW(IDC_EDIT_TOTAL, _T(""));
	SetDlgItemTextW(IDC_EDIT_DEFECT, _T(""));
	SetDlgItemTextW(IDC_EDIT_DEFECT_RATE, _T(""));
	SetDlgItemTextW(IDC_EDIT_ERROR_COUNT, _T(""));
	SetDlgItemTextW(IDC_EDIT_ERROR_PER_ONE, _T(""));
	SetDlgItemTextW(IDC_EDIT_PRODUCTION_SPEED, _T(""));
	SetDlgItemTextW(IDC_EDIT_MAX_SPEED, _T(""));
	SetDlgItemTextW(IDC_EDIT_GLUE_INJECTOR_STATS, _T(""));
	SetDlgItemTextW(IDC_EDIT_DEFECT_RATE_SYS, _T(""));
	SetDlgItemTextW(IDC_EDIT_TOTAL_SYS, _T(""));
	SetDlgItemTextW(IDC_EDIT_DEFECT_SYS, _T(""));

	m_btnNext.SetLabelImage(IMG_CH_RIGHT_ARROW_GRAY);
	m_btnNext.EnableWindow(FALSE);
	RedrawButton(m_btnNext);

	m_btnPrevious.SetLabelImage(IMG_CH_LEFT_ARROW_GRAY);
	m_btnPrevious.EnableWindow(FALSE);
	RedrawButton(m_btnPrevious);

	m_btnExport.SetLabelImage(IMG_SPC_EXPORT);
//	m_btnExport.EnableWindow(FALSE);
	RedrawButton(m_btnExport);

	m_ListTestItem.DeleteAllItems();

	auto pChConfigArray = gConfigMgr.GetChannelConfigPtr();
	CHANNEL_CONFIG chConfig = (*pChConfigArray)[m_crtChIndex];
	TASK crtTask = chConfig.GetTask(m_strCrtTaskName);
	FillTestItemList(crtTask);
}

void CSpcDlg::OnCbnSelchangeComboShiftOption()
{
	CString sShiftName;
	int index = m_shiftCombo.GetCurSel();
	if(index == -1)
	{
		return;
	}
	m_shiftCombo.GetLBText(index, sShiftName);

	std::vector<SHIFT>* pShiftArray = ::gConfigMgr.GetShiftArrayPtr();
	if(pShiftArray == nullptr || pShiftArray->empty())
	{
		return;
	}
	wstring wstrShiftName = CT2W(sShiftName);
	bool bGet = false;
	for(auto it = pShiftArray->begin(); it != pShiftArray->end(); it++)
	{
		if(wstrShiftName == it->m_wstrShiftName)
		{
			bGet = true;
			SetTimerPicker(*it);
		}
	}
	if(index == m_shiftCombo.GetCount() - 1)
	{
		SHIFT shift;
		shift.m_bgnTime = 0;
		shift.m_endTime = 2359;
		SetTimerPicker(shift);
	}

	UpdateData(FALSE);
}

void CSpcDlg::OnDtnDatetimechangeDatetimepickerEndTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CSpcDlg::OnCbnSelchangeComboBatchnumber()
{
	//获取任务名
	CString sTaskName;
	CString sMsg;
	int index = m_comboTask.GetCurSel();
	if(index == -1)
	{
		sMsg.LoadString(IDS_STRING_PLEASE_SELECT_TASK);
		DisplayPromptMessage(sMsg, false);
		return;
	}
	m_comboTask.GetLBText(index, sTaskName);
	wstring strTaskName = CT2W(sTaskName);

	std::string strTableName = utils::Unicode2Utf8(strTaskName);

	//获取批次号
	CString cstrBatchNumber;
	index = m_BatchNumberCombo.GetCurSel();
	if(index == -1)
	{
		return;
	}
	m_BatchNumberCombo.GetLBText(index, cstrBatchNumber);
	if (cstrBatchNumber.IsEmpty())
	{
		return;
	}

	//获取当前批次号
    vector<CString> m_vecStrTimeMinMax;
	bool bTimeMinMax = gSystemMgr.GetCurBatctNumberMinMaxTime(strTableName,cstrBatchNumber,m_vecStrTimeMinMax);
	if (bTimeMinMax && m_vecStrTimeMinMax.size()==2)
	{
		CString  strTimeBgn = m_vecStrTimeMinMax[0];
		CString  strTimeEnd = m_vecStrTimeMinMax[1];

		SYSTEMTIME dt;
		dt.wYear = _ttoi(strTimeBgn.Mid(0,4));
		dt.wMonth = _ttoi(strTimeBgn.Mid(4,2));
		dt.wDay = _ttoi(strTimeBgn.Mid(6,2));
		dt.wHour= _ttoi(strTimeBgn.Mid(8,2));
		dt.wMinute= _ttoi(strTimeBgn.Mid(10,2));
		dt.wSecond = _ttoi(strTimeBgn.Mid(12,2));
		m_dateBgn.SetTime(&dt);
		m_timeBgn.SetTime(&dt);

		dt.wYear = _ttoi(strTimeEnd.Mid(0,4));
		dt.wMonth = _ttoi(strTimeEnd.Mid(4,2));
		dt.wDay = _ttoi(strTimeEnd.Mid(6,2));
		dt.wHour= _ttoi(strTimeEnd.Mid(8,2));
		dt.wMinute= _ttoi(strTimeEnd.Mid(10,2));
		dt.wSecond = _ttoi(strTimeEnd.Mid(12,2));
		m_dateEnd.SetTime(&dt);
		m_timeEnd.SetTime(&dt);

	}

	//查询一个月之内的批次号
	// TODO: 在此添加控件通知处理程序代码
}
