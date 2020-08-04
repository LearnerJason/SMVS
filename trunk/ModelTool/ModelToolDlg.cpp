#include "stdafx.h"
#include "ModelTool.h"
#include "ModelToolDlg.h"
#include "afxdialogex.h"
#include <io.h>
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// Default exception handler 
void CPPExpDefaultExceptionHandler(const Halcon::HException& except)
{
	throw except;
}
CModelToolDlg::CModelToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModelToolDlg::IDD, pParent),
	m_hChWnd(NULL),
	DLG_TIMER_ID(888888),
	m_bOpRslt(true),
	m_bChs(true),
	m_dZoomRatio(1.0),
	m_imgWndID(-1),
	m_nOperationType(0),
	m_bSliderMoved(false),
	m_bClockwiseRotate(false),
	m_bCounterClockwiseRotate(false),
	m_dEnlargeShrink(0.0),
	m_bFirstEnlarge(true),
	m_bFirstShrink(true),
	m_createModel(true),
	m_mouseSelectRegion(false),
	m_drawSectorRegion(false),
	m_wstrImgFilePath(L""),
	m_wstrCreateModelAngle(L""),
	m_wstrCreateModelMethod(L""),
	m_wstrProductName(L""),
	m_wstrProductNameFirst(L""),
	m_wstrProductNameSecond(L""),
	m_wstrProductNameThird(L""),
	m_wstrProductNameFifth(L""),
	m_wstrTargetFile(L""),
	m_wstrMatchingType(L"999"),
	m_rowModelCenterSet(0.0),
	m_colModelCenterSet(0.0),
	m_angleModelCenterSet(0.0),
	m_rowCenterSet(247.0),
	m_colCenterSet(330.0),
	m_angleCenterSet(0.0),
	m_wstrFileType(L"0"),
	m_wstrModelFiles(L""),
	m_bTargetFileChanged(false),
	m_bExternalRgn(false),
	m_bNewImageMatchingOK(true)
	, m_numLevels(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	int txtFontSz = 25;
	m_textFont.CreateFont(txtFontSz,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
	ANSI_CHARSET,             
	OUT_DEFAULT_PRECIS,        
	CLIP_DEFAULT_PRECIS,       
	DEFAULT_QUALITY,           
	DEFAULT_PITCH | FF_SWISS,  
	_T("Arial"));

	m_sliderTextFont.CreateFont(20,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
	ANSI_CHARSET,             
	OUT_DEFAULT_PRECIS,        
	CLIP_DEFAULT_PRECIS,       
	DEFAULT_QUALITY,           
	DEFAULT_PITCH | FF_SWISS,  
	_T("Arial"));

	m_smallFont.CreateFont(16,0, 0, 0,FW_BOLD, FALSE, FALSE,0,                         
	ANSI_CHARSET,             
	OUT_DEFAULT_PRECIS,        
	CLIP_DEFAULT_PRECIS,       
	DEFAULT_QUALITY,           
	DEFAULT_PITCH | FF_SWISS,  
	_T("Arial"));

	m_crtToolType = TOOL_INVALID;
	m_finalRgn.Reset();
	m_definedAngleRgn.Reset();
	m_modelID       = -1;
	m_modelFilePath = _T("");
	m_vManualRgnPath.clear();
	set_system("clip_region", "false");
	gen_empty_obj(&m_newModelImageRgn);
	m_numLevels=3;
	m_bDrawSector = false;
	m_bShowFill=false;

	// Install default exception handler 
	HException::InstallHHandler(&CPPExpDefaultExceptionHandler);
}

void CModelToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MSG_PANEL, m_msgPanel);
	DDX_Control(pDX, IDC_COMBO_MODEL_TYPE, m_comboModelType);
	DDX_Control(pDX, IDC_BUTTON_SAVE_MODEL, m_btnSaveModel);
	DDX_Control(pDX, IDC_STATIC_TOOL_PANEL, m_staticToolPanel);
	DDX_Control(pDX, IDC_BUTTON_CIRCLE_SEL, m_btnCircleSel);
	DDX_Control(pDX, IDC_BUTTON_ELLIPSE_SEL, m_btnEllipseSel);
	DDX_Control(pDX, IDC_BUTTON_RECTANGLE_SEL, m_btnRectSel);
	DDX_Control(pDX, IDC_BUTTON_POLYGON_SEL, m_btnPolygonSel);
	DDX_Control(pDX, IDC_BUTTON_CIRCLE_DRAW, m_btnCircleDraw);
	DDX_Control(pDX, IDC_BUTTON_ELLIPSE_DRAW, m_btnEllipseDraw);
	DDX_Control(pDX, IDC_BUTTON_RECTANGLE_DRAW, m_btnRectDraw);
	DDX_Control(pDX, IDC_BUTTON_POLYGON_DRAW, m_btnPolygonDraw);
	DDX_Control(pDX, IDC_BUTTON_UNION, m_btnUnion);
	DDX_Control(pDX, IDC_BUTTON_DIFFERENCE, m_btnDifference);
	DDX_Control(pDX, IDC_BUTTON_UNDO, m_btnUndo);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_IN, m_btnZoomIn);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_OUT, m_btnZoomOut);
	DDX_Control(pDX, IDC_SLIDER_GRAY_VAL_RANGE, m_grayValFilter);
	DDX_Control(pDX, IDC_SLIDER_REGION_SIZE_FILTER, m_rgnSzFilter);
	DDX_Control(pDX, IDC_STATIC_GRAY_LCL, m_grayValLCL);
	DDX_Control(pDX, IDC_STATIC_GRAY_UCL, m_grayValUCL);
	DDX_Control(pDX, IDC_STATIC_GRAY_MIN, m_grayValMin);
	DDX_Control(pDX, IDC_STATIC_GRAY_MAX, m_grayValMax);
	DDX_Control(pDX, IDC_STATIC_GRN_SIZE_LCL, m_rgnSzLCL);
	DDX_Control(pDX, IDC_STATIC_RGN_SIZE_UCL, m_rgnSzUCL);
	DDX_Control(pDX, IDC_STATIC_RGN_SIZE_MIN, m_rgnSzMin);
	DDX_Control(pDX, IDC_STATIC_RGN_SIZE_MAX, m_rgnSzMax);
	DDX_Control(pDX, IDC_STATIC_GRAY_VALUE_FILTER, m_grayValSliderName);
	DDX_Control(pDX, IDC_STATIC_RGN_SIZE_FILTER, m_rgnSzSliderName);
	DDX_Control(pDX, IDC_STATIC_CONTRAST, m_ctrstName);
	DDX_Control(pDX, IDC_STATIC_CONTRAST_LCL, m_ctrstLCL);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST_VALUE, m_ctrstFilter);
	DDX_Control(pDX, IDC_STATIC_CONTRAST_UCL, m_ctrstUCL);
	DDX_Control(pDX, IDC_STATIC_CONTRAST_VALUE, m_ctrstValue);
	DDX_Control(pDX, IDC_STATIC_CONTRAST_HIGH_VALUE, m_ctrstValueHigh);
	DDX_Control(pDX, IDC_BUTTON_SAVE_RGN, m_btnSaveRgn);
	DDX_Control(pDX, IDC_BUTTON_RESET_RGN, m_btnResetRgn);
	DDX_Control(pDX, IDC_BUTTON_MOVE_UP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BUTTON_MOVE_DOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_BUTTON_MOVE_LEFT, m_btnMoveLeft);
	DDX_Control(pDX, IDC_BUTTON_MOVE_RIGHT, m_btnMoveRight);
	DDX_Control(pDX, IDC_EDIT_SCALE_PIXEL, m_editScalePixel);
	DDX_Control(pDX, IDC_SPIN_SCALE_PIXEL, m_spinScalePixel);
	DDX_Control(pDX, IDC_BUTTON_ENLARGE, m_btnEnlarge);
	DDX_Control(pDX, IDC_BUTTON_SHRINK, m_btnShrink);
	DDX_Control(pDX, IDC_EDIT_DEGREE, m_editAngle);
	DDX_Control(pDX, IDC_SPIN_DEGREE, m_spinAngle);
	DDX_Control(pDX, IDC_BUTTON_ROTATE_CLOCKWISE, m_btnClockwise);
	DDX_Control(pDX, IDC_BUTTON_COUNTER_CLOCKWISE, m_btnCounterClockwise);
	DDX_Control(pDX, IDC_EDIT_MOVE_PIXEL, m_editMovePixel);
	DDX_Control(pDX, IDC_SPIN_MOVE_PIXEL, m_spinMovePixel);
	DDX_Control(pDX, IDC_STATIC_RGN_OPERATOR, m_rgnOperatorPanel);
	DDX_Control(pDX, IDC_SLIDER_DILATION_SIZE, m_dilationSzFilter);
	DDX_Control(pDX, IDC_STATIC_DILATION, m_dilationName);
	DDX_Control(pDX, IDC_STATIC_DILATION_LCL, m_dilationLCL);
	DDX_Control(pDX, IDC_STATIC_DILATION_UCL, m_dilationUCL);
	DDX_Control(pDX, IDC_STATIC_DILATION_VALUE, m_dilationValue);
	DDX_Control(pDX, IDC_SLIDER_DILATION_SIZE, m_dilationSzFilter);
	DDX_Control(pDX, IDC_STATIC_DILATION, m_dilationName);
	DDX_Control(pDX, IDC_STATIC_DILATION_LCL, m_dilationLCL);
	DDX_Control(pDX, IDC_STATIC_DILATION_UCL, m_dilationUCL);
	DDX_Control(pDX, IDC_STATIC_DILATION_VALUE, m_dilationValue);
	DDX_Control(pDX, IDC_RADIO_ALL, m_btnSelectRegionExtract);
	DDX_Control(pDX, IDC_COMBO_WORKING_FOLDER, m_comboWorkingFolder);
	DDX_Check(pDX, IDC_CHECK_DRAW_SECTOR, m_bDrawSector);
	DDX_Check(pDX, IDC_CHECK_SHOW_FILL, m_bShowFill);
	DDX_Control(pDX, IDC_EDIT_MODEL_NAME, m_editModelName);
	DDX_Control(pDX, IDC_STATIC_MODEL_NAME, m_staticModelName);
	DDX_Control(pDX, IDC_STATIC_MODEL_ITEM, m_staticModelItem);
	DDX_Text(pDX, IDC_NUMLEVELS, m_numLevels);
	DDV_MinMaxInt(pDX, m_numLevels, 0, 4);
	DDX_Control(pDX, IDC_NUMLEVELS, m_numLevelsEdit);
	DDX_Control(pDX, IDC_STATIC_NUMLEVELS, m_staticNumLevels);
	DDX_Control(pDX, IDC_BUTTON_SAVE_MODEL2, m_btnRateModelImage);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);
}

void CModelToolDlg::SetChannelWindowHandle(HWND hChWnd)
{
	m_hChWnd = hChWnd;
}

void CModelToolDlg::SetChsLangFlag(bool bChs)
{
	m_bChs = bChs;
}

void CModelToolDlg::SetImageFilePath(const wstring& wstrPath)
{
	m_wstrImgFilePath = wstrPath;
}

void CModelToolDlg::SetModelAngle(const wstring& wstrPath)
{
	m_wstrCreateModelAngle = wstrPath;
}

void CModelToolDlg::SetCreateModelMethod(const wstring& wstrPath)
{
	m_wstrCreateModelMethod = wstrPath;
}

void CModelToolDlg::GetCenterXY(double rowCenter,double colCenter)
{
   m_rowCenterTrans = rowCenter;
   m_colCenterTrans = colCenter;
   m_wstrProductNameFirst = L"BATTERY_PRODUCT";
   m_wstrProductNameSecond = L"BOTTLE_PRODUCT";
   m_wstrProductNameThird = L"PCC_PRODUCT";
   m_wstrProductNameFifth = L"AL_PLASTIC_PRODUCT";
	
}

void CModelToolDlg::SetTargetFilePath(const wstring& wstrPath)
{
	m_wstrTargetFile = wstrPath;
}

void CModelToolDlg::SetTargetFileType(const wstring& wstrFileType)
{
	m_wstrFileType = wstrFileType;
}

void CModelToolDlg::SetModelFiles(const wstring& wstrModelFile)
{
	m_wstrModelFiles = wstrModelFile;
}

void CModelToolDlg::SetTargetMatchingType(const wstring& wstrMatchingType)
{
	std::vector<wstring> vItemMatching = SplitString(wstrMatchingType, L",");
	wstring	wstrMatching ;
	wstring	wstrColCenter ;
	wstring	wstrRowCenter ;
	if(vItemMatching.size() == 3)
	{
		wstrMatching       = vItemMatching[0];
		wstrColCenter      = vItemMatching[1];
		wstrRowCenter      = vItemMatching[2];
		//@8,640,512
		//auto it = wstrRowCenter.find(L"");
		//wstrRowCenter = wstrRowCenter.substr(0, it);
		//	
		m_wstrMatchingType = wstrMatching;
		double colCenter = boost::lexical_cast< double>(wstrColCenter);
		double rowCenter = boost::lexical_cast<double>(wstrRowCenter);
		m_rowModelCenterSet=rowCenter;
		m_colModelCenterSet=colCenter;
	}

	m_angleModelCenterSet=0.0;
	//
	wstring	wstrAngle ;
	if(vItemMatching.size() == 4)
	{
		wstrMatching       = vItemMatching[0];
		wstrColCenter      = vItemMatching[1];
		wstrRowCenter      = vItemMatching[2];
		wstrAngle          = vItemMatching[3];
		//@8,640,512
		//auto it = wstrRowCenter.find(L"");
		//wstrRowCenter = wstrRowCenter.substr(0, it);
		//	
		m_wstrMatchingType = wstrMatching;
		double colCenter = boost::lexical_cast< double>(wstrColCenter);
		double rowCenter = boost::lexical_cast<double>(wstrRowCenter);
		double angleModel = boost::lexical_cast<double>(wstrAngle);
		m_rowModelCenterSet=rowCenter;
		m_colModelCenterSet=colCenter;
		m_angleModelCenterSet=angleModel;
	}
	//
	if (m_wstrFileType==L"0")
	{
		m_wstrMatchingType=m_wstrFileType;
		m_rowCenterSet=m_rowModelCenterSet;
		m_colCenterSet=m_colModelCenterSet;
		m_angleCenterSet=m_angleModelCenterSet;
	}
	//
}

void AdjustRect(CRect& rcBtn)
{
	rcBtn.top = rcBtn.bottom + 5;
	rcBtn.bottom = rcBtn.top + 32;
}

void CenterDefined(Hobject image, float circularity, HTuple& centerXY)
{
	// Local iconic variables 
	Hobject  Ellipse, ImageReduced, Edges;
	Hobject  SelectedXLD1, SelectedXLD;
	// Local control variables 
	HTuple  Width,Height, Diff, Sgn, Indices2, Row, Column,CircularityTuple;
	HTuple  Radius, StartPhi, EndPhi, PointOrder, RowSelected,ColumnSelected,CircelNum,CenterRow,CenterColumn;
	get_image_size(image, &Width, &Height);
	gen_ellipse(&Ellipse, Height/2.0, Width/2.0, 0, (Height/2.0)+70, (Height/2.0)+10);
	reduce_domain(image, Ellipse, &ImageReduced);
	edges_sub_pix(ImageReduced, &Edges, "canny", 1, 15, 30);
	select_shape_xld(Edges, &SelectedXLD1, "contlength", "and", 260, 99999);
	union_cocircular_contours_xld(SelectedXLD1, &SelectedXLD, 0.5, 0.1, 0.2, 5, 3, 3, "true", 1);
	circularity_xld(SelectedXLD, &CircularityTuple);
	tuple_sub(CircularityTuple, circularity, &Diff);
	tuple_sgn(Diff, &Sgn);
	tuple_find(Sgn, 1, &Indices2);
	//**无合适边缘
	HTuple LengthFind;
	tuple_length (Indices2, &LengthFind);
	if(LengthFind<1)
	{	
		centerXY[1]=Height/2.0;
		centerXY[0]=Width/2.0;
		return ;
	}
	//**
	if(Indices2[0].I()!=-1)
	{
		fit_circle_contour_xld(SelectedXLD, "algebraic", -1, 0, 0, 3, 2, &Row, &Column,  &Radius, &StartPhi, &EndPhi, &PointOrder);
		tuple_select(Row, Indices2, &RowSelected);
		tuple_select(Column, Indices2, &ColumnSelected);
		tuple_length(ColumnSelected,&CircelNum);
		if(CircelNum[0].I()>0)
		{
			tuple_median(RowSelected, &CenterRow);
			tuple_median(ColumnSelected, &CenterColumn);
			centerXY[1]=CenterRow;
			centerXY[0]=CenterColumn;
		}
		else
		{
			centerXY[1]=Height/2.0;
			centerXY[0]=Width/2.0;
		}
	}
	else
	{
		centerXY[1]=Height/2.0;
		centerXY[0]=Width/2.0;
	}
}

BEGIN_MESSAGE_MAP(CModelToolDlg, CDialogEx)

	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_MODEL, &CModelToolDlg::OnBnClickedButtonSaveModel)
	ON_BN_CLICKED(IDC_BUTTON_CIRCLE_SEL, &CModelToolDlg::OnBnClickedButtonCircleSel)
	ON_BN_CLICKED(IDC_BUTTON_RECTANGLE_SEL, &CModelToolDlg::OnBnClickedButtonRectangleSel)
	ON_BN_CLICKED(IDC_BUTTON_POLYGON_SEL, &CModelToolDlg::OnBnClickedButtonPolygonSel)
	ON_BN_CLICKED(IDC_BUTTON_CIRCLE_DRAW, &CModelToolDlg::OnBnClickedButtonCircleDraw)
	ON_BN_CLICKED(IDC_BUTTON_RECTANGLE_DRAW, &CModelToolDlg::OnBnClickedButtonRectangleDraw)
	ON_BN_CLICKED(IDC_BUTTON_POLYGON_DRAW, &CModelToolDlg::OnBnClickedButtonPolygonDraw)
	ON_BN_CLICKED(IDC_BUTTON_UNION, &CModelToolDlg::OnBnClickedButtonUnion)
	ON_BN_CLICKED(IDC_BUTTON_DIFFERENCE, &CModelToolDlg::OnBnClickedButtonDifference)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CModelToolDlg::OnBnClickedButtonUndo)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CModelToolDlg::OnBnClickedButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CModelToolDlg::OnBnClickedButtonZoomOut)
	ON_MESSAGE(WM_RANGESLIDER_RANGECHANGED, &CModelToolDlg::OnRangeSliderChanged)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_RGN, &CModelToolDlg::OnBnClickedButtonSaveRegion)
	ON_BN_CLICKED(IDC_BUTTON_ELLIPSE_SEL, &CModelToolDlg::OnBnClickedButtonEllipseSel)
	ON_BN_CLICKED(IDC_BUTTON_ELLIPSE_DRAW, &CModelToolDlg::OnBnClickedButtonEllipseDraw)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CModelToolDlg::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, &CModelToolDlg::OnBnClickedButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_LEFT, &CModelToolDlg::OnBnClickedButtonMoveLeft)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_RIGHT, &CModelToolDlg::OnBnClickedButtonMoveRight)
	ON_BN_CLICKED(IDC_BUTTON_ENLARGE, &CModelToolDlg::OnBnClickedButtonEnlarge)
	ON_BN_CLICKED(IDC_BUTTON_SHRINK, &CModelToolDlg::OnBnClickedButtonShrink)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_CLOCKWISE, &CModelToolDlg::OnBnClickedButtonRotateClockwise)
	ON_BN_CLICKED(IDC_BUTTON_COUNTER_CLOCKWISE, &CModelToolDlg::OnBnClickedButtonCounterClockwise)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOVE_PIXEL, &CModelToolDlg::OnDeltaposSpinMovePixel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SCALE_PIXEL, &CModelToolDlg::OnDeltaposSpinScalePixel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DEGREE, &CModelToolDlg::OnDeltaposSpinDegree)
	ON_BN_CLICKED(IDC_RADIO_PLANE, &CModelToolDlg::OnBnClickedRadioPlane)
	ON_BN_CLICKED(IDC_RADIO_RING, &CModelToolDlg::OnBnClickedRadioRing)
	ON_BN_CLICKED(IDC_RADIO_ALL, &CModelToolDlg::OnBnClickedRadioAll)
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_WORKING_FOLDER, &CModelToolDlg::OnCbnSelchangeComboWorkingFolder)
	ON_BN_CLICKED(IDC_CHECK_DRAW_SECTOR, &CModelToolDlg::OnBnClickedCheckDrawSector)
	ON_BN_CLICKED(IDC_BUTTON_RESET_RGN, &CModelToolDlg::OnBnClickedButtonResetRgn)
	ON_EN_CHANGE(IDC_NUMLEVELS, &CModelToolDlg::OnEnChangeNumlevels)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_SHOW_FILL, &CModelToolDlg::OnBnClickedCheckShowFill)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_MODEL2, &CModelToolDlg::OnBnClickedButtonSaveModel2)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CModelToolDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()




void CModelToolDlg::DisplayModelUiWidget()
{
	m_createModel = true;
	((CWnd *)GetDlgItem(IDC_STATIC_SELECT_REGION))->ShowWindow(SW_HIDE);    
	((CWnd *)GetDlgItem(IDC_RADIO_PLANE))->ShowWindow(SW_HIDE);     
	((CWnd *)GetDlgItem(IDC_RADIO_RING))->ShowWindow(SW_HIDE);   
	((CWnd *)GetDlgItem(IDC_RADIO_ALL))->ShowWindow(SW_HIDE); 
	((CWnd *)GetDlgItem(IDC_STATIC_PLANE))->ShowWindow(SW_HIDE);   
	((CWnd *)GetDlgItem(IDC_STATIC_RING))->ShowWindow(SW_HIDE);   
	((CWnd *)GetDlgItem(IDC_STATIC_ALL))->ShowWindow(SW_HIDE); 
	((CWnd *)GetDlgItem(IDC_CHECK_DRAW_SECTOR))->ShowWindow(SW_SHOW); 
	((CWnd *)GetDlgItem(IDC_STATIC_NUMLEVELS))->ShowWindow(SW_SHOW); 
	((CWnd *)GetDlgItem(IDC_NUMLEVELS))->ShowWindow(SW_SHOW); 
	m_staticModelItem.ShowWindow(SW_HIDE);
	m_comboWorkingFolder.ShowWindow(SW_HIDE);
	m_btnSaveRgn.ShowWindow(SW_HIDE);
	m_btnSaveRgn.EnableWindow(FALSE);

	m_staticModelName.ShowWindow(SW_SHOW);
	m_editModelName.ShowWindow(SW_SHOW);
	m_comboModelType.ShowWindow(SW_SHOW);
	m_btnSaveModel.ShowWindow(SW_SHOW);
	m_btnSaveModel.EnableWindow(FALSE);
	if (m_wstrMatchingType==L"0")//是否显示‘校正按钮’
	{ 
		m_btnRateModelImage.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnRateModelImage.ShowWindow(SW_HIDE);
	}
	m_rgnOperatorPanel.ShowWindow(SW_HIDE);
	m_btnMoveUp.ShowWindow(SW_HIDE);
	m_btnMoveDown.ShowWindow(SW_HIDE);
	m_btnMoveLeft.ShowWindow(SW_HIDE);
	m_btnMoveRight.ShowWindow(SW_HIDE);
	m_editMovePixel.ShowWindow(SW_HIDE);
	m_spinMovePixel.ShowWindow(SW_HIDE);
	m_editScalePixel.ShowWindow(SW_HIDE);
	m_spinScalePixel.ShowWindow(SW_HIDE);
	m_btnEnlarge.ShowWindow(SW_HIDE);
	m_btnShrink.ShowWindow(SW_HIDE);
	m_editAngle.ShowWindow(SW_HIDE);
	m_spinAngle.ShowWindow(SW_HIDE);
	m_btnClockwise.ShowWindow(SW_HIDE);
	m_btnCounterClockwise.ShowWindow(SW_HIDE);

	if(m_modelRgn.Id() != H_EMPTY_REGION)
	{
		m_finalRgn = m_modelRgn;
	}
	if(m_modelImg.Id() != H_EMPTY_REGION)
	{
		m_crtImg = m_modelImg;
	}
	wstring wstrFile = m_wstrTargetFile;
	size_t pos = wstrFile.find_last_of(L"\\");
	wstring wstrModelName = wstrFile.substr(pos + 1);
	pos = wstrModelName.find(L".");
	if(pos != wstring::npos)
	{
		wstrModelName = wstrModelName.substr(0, pos);
		m_editModelName.SetWindowTextW(wstrModelName.c_str());
		m_editModelName.EnableWindow(FALSE);
	}

	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
}

void CModelToolDlg::ProcessAndDisplayModel()
{
	if(m_wstrTargetFile.find(L".shm") == wstring::npos)
	{
		return;
	}
	string strPath, strModelName;
	if(!ParseModelPath(m_wstrTargetFile, strPath, strModelName))
	{
		return;
	}

	m_editModelName.SetWindowTextW(CA2T(strModelName.c_str()));
	m_editModelName.EnableWindow(FALSE);

	bool read_Ok=ReadWorkingFolder(strPath, strModelName);
	if (!read_Ok)
	{
		return ;
	}
	ReadRegionFiles(strPath, strModelName);
	ProcessRegionsByNewImg();

	if(m_modelRgn.Id() != H_EMPTY_REGION)
	{
		Hobject RegionNewModel;
		if(m_bNewImageMatchingOK)
		{

			if(!m_bUseScaleModel)
			{
				HTuple HomMat2DModel;
				vector_angle_to_rigid( m_rowModelCenter, m_colModelCenter, m_angleModelCenter,m_rowNewImagemMatchingCenter, m_colNewImagemMatchingCenter, m_dNewImageAngle, &HomMat2DModel);
				projective_trans_region(m_modelRgn, &RegionNewModel, HomMat2DModel, "bilinear");
			}
			else
			{
				HTuple HomMat2DS,HomMat2DScale,HomMat2DTmp,HomMat2DAdapted;
				Hobject RegionMoved1,rotatedRgn,RegionMoved2,regionOpening;
				double scaleData=0.0;
				scaleData=m_newImagemMatchingScale[0].D();
				vector_angle_to_rigid(0, 0, m_angleScaleModel, 0, 0, m_dNewImageAngle, &HomMat2DS);
				hom_mat2d_scale (HomMat2DS, scaleData, scaleData,0, 0, &HomMat2DScale);
				hom_mat2d_translate(HomMat2DScale, 0.5, 0.5, &HomMat2DTmp);
				hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
				move_region (m_modelRgn, &RegionMoved1,-m_rowScaleModelCenter, -m_colScaleModelCenter);        
				projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
				move_region (rotatedRgn,&RegionMoved2, m_rowNewImagemMatchingCenter, m_colNewImagemMatchingCenter) ;
				opening_circle(RegionMoved2, &regionOpening, 1.0);
				closing_circle(regionOpening, &RegionNewModel, 1.0);
			}
			if(m_bChs)
				DisplayPromptMessage(_T("读取旧模板区域，修改模板区域，重新建模板！"), true);
			else
				DisplayPromptMessage(_T("Read model region, modify model region, recreate model!"), true);
		}
		else
		{
			if(m_bChs)
				DisplayPromptMessage(_T("当前图片匹配失败，请重新画模板区域！"), false);
			else
				DisplayPromptMessage(_T("Matching current image failed, please recreate model!"), false);
		}
		m_finalRgn = RegionNewModel;
		m_bExternalRgn = true;
	}

	DisplayAllRegions();
}

void CModelToolDlg::DisplayRegionUiWidget()
{
	m_createModel = false;
	if(m_bDrawSector == FALSE)
	{
		((CWnd *)GetDlgItem(IDC_RADIO_PLANE))->ShowWindow(SW_SHOW);   
		((CWnd *)GetDlgItem(IDC_RADIO_RING))->ShowWindow(SW_SHOW);   
		((CWnd *)GetDlgItem(IDC_RADIO_ALL))->ShowWindow(SW_SHOW); 
		((CWnd *)GetDlgItem(IDC_STATIC_PLANE))->ShowWindow(SW_SHOW);   
		((CWnd *)GetDlgItem(IDC_STATIC_RING))->ShowWindow(SW_SHOW);   
		((CWnd *)GetDlgItem(IDC_STATIC_ALL))->ShowWindow(SW_SHOW); 
		((CWnd *)GetDlgItem(IDC_STATIC_SELECT_REGION))->ShowWindow(SW_SHOW);    
	}
	((CWnd *)GetDlgItem(IDC_CHECK_DRAW_SECTOR))->ShowWindow(SW_SHOW);    
	((CWnd *)GetDlgItem(IDC_STATIC_NUMLEVELS))->ShowWindow(SW_HIDE); 
	((CWnd *)GetDlgItem(IDC_NUMLEVELS))->ShowWindow(SW_HIDE); 
	m_staticModelItem.ShowWindow(SW_SHOW);
	m_comboWorkingFolder.ShowWindow(SW_SHOW);
	m_btnSaveRgn.ShowWindow(SW_SHOW);
	m_btnSaveRgn.EnableWindow(FALSE);

	m_staticModelName.ShowWindow(SW_HIDE);
	m_editModelName.ShowWindow(SW_HIDE);
	m_comboModelType.ShowWindow(SW_HIDE);
	m_btnSaveModel.ShowWindow(SW_HIDE);
	m_btnSaveModel.EnableWindow(FALSE);
	m_btnRateModelImage.ShowWindow(SW_HIDE);

	m_rgnOperatorPanel.ShowWindow(SW_SHOW);
	m_btnMoveUp.ShowWindow(SW_SHOW);
	m_btnMoveDown.ShowWindow(SW_SHOW);
	m_btnMoveLeft.ShowWindow(SW_SHOW);
	m_btnMoveRight.ShowWindow(SW_SHOW);
	m_editMovePixel.ShowWindow(SW_SHOW);
	m_spinMovePixel.ShowWindow(SW_SHOW);

	m_editScalePixel.ShowWindow(SW_SHOW);
	m_spinScalePixel.ShowWindow(SW_SHOW);
	m_btnEnlarge.ShowWindow(SW_SHOW);
	m_btnShrink.ShowWindow(SW_SHOW);
	m_editAngle.ShowWindow(SW_SHOW);
	m_spinAngle.ShowWindow(SW_SHOW);
	m_btnClockwise.ShowWindow(SW_SHOW);
	m_btnCounterClockwise.ShowWindow(SW_SHOW);

	if(!m_dictTestRgn.empty())
	{
		m_dictRotatedTestRgn.clear();
		m_dictRotatedTestRgn = m_dictTestRgn;
	}
	ResetInternalHalconObject();
	m_finalRgn.Reset();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
}

void CModelToolDlg::ProcessAndDisplayRegion()
{
	if(m_wstrTargetFile.find(L".reg") == wstring::npos)
	{
		return;
	}
	string strPath, strModelName;
	if(!ParseModelPath(m_wstrTargetFile, strPath, strModelName))
	{
		return;
	}
	m_comboWorkingFolder.ResetContent();
	m_comboWorkingFolder.AddString(CA2T(strModelName.c_str()));
	m_comboWorkingFolder.SetCurSel(0);
	m_comboWorkingFolder.EnableWindow(TRUE);

	bool read_Ok=ReadWorkingFolder(strPath, strModelName);
	if (!read_Ok)
	{
		return ;
	}
	ReadRegionFiles(strPath, strModelName);
	ProcessRegionsByNewImg();

	if(m_dictRotatedTestRgn.find(m_wstrTargetFile) != m_dictRotatedTestRgn.end())
	{
		Hobject retRgn = m_dictRotatedTestRgn[m_wstrTargetFile];
		if(retRgn.Id() != H_EMPTY_REGION)
		{
			m_finalRgn = retRgn;
			m_bExternalRgn = true;
		}
	}
	DisplayAllRegions();
	m_btnResetRgn.ShowWindow(SW_SHOW);
	m_btnResetRgn.EnableWindow(TRUE);
}

bool CircleShape(Hobject Image, double circularity, int MinArea)
{
	Hobject  Edges, ContoursSplit, UnionContours;
	Hobject  SelectedXLD;
	HTuple  Selection, Index, Circularity, Length;
	edges_sub_pix(Image, &Edges, "canny", 1, 8, 16);
	segment_contours_xld(Edges, &ContoursSplit, "lines_circles", 5, 4, 2);
	union_cocircular_contours_xld(ContoursSplit, &UnionContours, 0.5, 0.1, 0.2, 30, 10, 10, "true", 1);
	select_shape_xld(UnionContours, &SelectedXLD, (HTuple("circularity").Append("area")), "and", (HTuple(circularity).Append(MinArea)), (HTuple(1.0).Append(99999999)));
	circularity_xld(SelectedXLD, &Circularity);
	tuple_length(Circularity, &Length);
	if (0 != (Length<1))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void  RegionCenterAdjust(Hobject ModelRegion,Hobject *ModelRegionAdjust,Hobject *RegionDifference,HTuple *RegDiffArea)
{
	// Local iconic variables 
	Hobject  ModelRegionNew, ModelRegionMove;
	Hobject  AddRegion;


	// Local control variables 
	HTuple  Area1, Row1, Column1, Area2, Row2, Column2;
	HTuple  Row12, Column12, Row21, Column21, DistRowUp, DistRowDown;
	HTuple  DistColLeft, DistColRight, RowOffset, ColOffset;
	HTuple  NumRow, RectangleWith, RectangleHeight, NumCol;

	area_center(ModelRegion, &Area1, &Row1, &Column1);
	if (0 != (Area1<2))
	{
		copy_obj(ModelRegion, &ModelRegionNew, 1, 1);
		area_center(ModelRegionNew, &Area2, &Row2, &Column2);
	}
	else
	{
		move_region(ModelRegion, &ModelRegionMove, Row1-(Row1.Round()), Column1-(Column1.Round()));
		intersection(ModelRegion, ModelRegionMove, &ModelRegionNew);
		area_center(ModelRegionNew, &Area2, &Row2, &Column2);
	}
	copy_obj(ModelRegionNew, ModelRegionAdjust, 1, 1);
	smallest_rectangle1(ModelRegionNew, &Row12, &Column12, &Row21, &Column21);
	DistRowUp = Row2-Row12;
	DistRowDown = Row21-Row2;
	DistColLeft = Column2-Column12;
	DistColRight = Column21-Column2;

	RowOffset = Area2*(Row2-(Row2.Round()));
	ColOffset = Area2*(Column2-(Column2.Round()));
	if (0 != (RowOffset>0))
	{
		NumRow = RowOffset/DistRowUp;
		RectangleWith = ((NumRow.Abs()).Sqrt()).Round();
		if (0 != (RectangleWith>=1))
		{
			RectangleHeight = ((NumRow.Abs())/RectangleWith).Round();
			if (0 != (RectangleHeight>=1))
			{
				gen_rectangle2(&AddRegion, Row12-(RectangleHeight/2.0), Column2, 0, (RectangleWith/2.0)-0.5, 
					(RectangleHeight/2.0)-0.5);
				union2(*ModelRegionAdjust, AddRegion, ModelRegionAdjust);
				union2(*RegionDifference, AddRegion, RegionDifference);
			}
		}
	}
	else if (0 != (RowOffset<0))
	{
		NumRow = RowOffset/DistRowDown;
		RectangleWith = ((NumRow.Abs()).Sqrt()).Round();
		if (0 != (RectangleWith>=1))
		{
			RectangleHeight = ((NumRow.Abs())/RectangleWith).Round();
			if (0 != (RectangleHeight>=1))
			{
				gen_rectangle2(&AddRegion, Row21+(RectangleHeight/2.0), Column2, 0, (RectangleWith/2.0)-0.5, 
					(RectangleHeight/2.0)-0.5);
				union2(*ModelRegionAdjust, AddRegion, ModelRegionAdjust);
				union2(*RegionDifference, AddRegion, RegionDifference);
			}
		}
	}
	if (0 != (ColOffset>0))
	{
		NumCol = ColOffset/DistColLeft;
		RectangleHeight = ((NumCol.Abs()).Sqrt()).Round();
		if (0 != (RectangleHeight>=1))
		{
			RectangleWith = ((NumCol.Abs())/RectangleHeight).Round();
			if (0 != (RectangleWith>=1))
			{
				gen_rectangle2(&AddRegion, Row2, Column12-(RectangleWith/2.0), 0, (RectangleWith/2.0)-0.5, 
					(RectangleHeight/2.0)-0.5);
				union2(*ModelRegionAdjust, AddRegion, ModelRegionAdjust);
				union2(*RegionDifference, AddRegion, RegionDifference);
			}
		}
	}
	else if (0 != (ColOffset<0))
	{
		NumCol = ColOffset/DistColRight;
		RectangleHeight = ((NumCol.Abs()).Sqrt()).Round();
		if (0 != (RectangleHeight>=1))
		{
			RectangleWith = ((NumCol.Abs())/RectangleHeight).Round();
			if (0 != (RectangleWith>=1))
			{
				gen_rectangle2(&AddRegion, Row2, Column21+(RectangleWith/2.0), 0, (RectangleWith/2.0)-0.5, 
					(RectangleHeight/2.0)-0.5);
				union2(*ModelRegionAdjust, AddRegion, ModelRegionAdjust);
				union2(*RegionDifference, AddRegion, RegionDifference);
			}
		}
	}
	//
	HTuple DiffRegRow,DiffRegCol;
	area_center(*RegionDifference, RegDiffArea, &DiffRegRow,&DiffRegCol);
}

void CModelToolDlg::OnBnClickedButtonSaveModel()
{
	CString sModelName;
	m_editModelName.GetWindowTextW(sModelName);

	if (sModelName.Find(_T("#")) == -1)
	{
		sModelName += m_wstrModelFiles.c_str();
	}
	if(sModelName.GetLength() <= 0)
	{
		if(m_bChs)
			DisplayPromptMessage(_T("请输入模板名！"),false);
		else
			DisplayPromptMessage(_T("Please type in  model name!"),false);
		return;
	}
	string strPath;
	string strModelName;
	if(m_wstrTargetFile.find(L".shm") == wstring::npos) // Create a folder
	{
		wstring wstrPath = m_wstrTargetFile;
		wstrPath.append(L"\\");
		wstrPath.append(CT2W(sModelName));
		if(!CreateFullDirectory(wstrPath.c_str()))
		{
			//TODO
		}
		strPath = CW2A(wstrPath.c_str());
		strModelName = CT2A(sModelName);
	}
	else
	{
		strPath = CW2A(m_wstrTargetFile.c_str());
		size_t pos   = strPath.find_last_of("\\");
		strModelName = strPath.substr(pos + 1);
		strPath      = strPath.substr(0, pos);
		size_t pos1  = strModelName.find(".");
		strModelName = strModelName.substr(0, pos1);
	}
	ResetInternalHalconObject();
	UnionNewRegions();

	string modelFile    = strPath;
	modelFile.append("\\");
	modelFile.append(strModelName);
	modelFile.append(".shm");

	string modelRgnFile = strPath;
	modelRgnFile.append("\\");
	modelRgnFile.append(strModelName);
	modelRgnFile.append(".reg");

	string modelDatFile = strPath;
	modelDatFile.append("\\");
	modelDatFile.append(strModelName);
	modelDatFile.append(".dat");

	string modelImgFile = strPath;
	modelImgFile.append("\\");
	modelImgFile.append(strModelName);
	modelImgFile.append(".tiff");

	Hobject retImg;
	HTuple modelID;
	///////
	HTuple  centerXY,HomMat2DRotate,Mean, Deviation;
	Hobject RegionAffineTrans,ModelRegion,RegionDifference,RegionDilation,RegionDifference1;
	Hobject DupImage,ImageModel;
	if( m_wstrProductNameFirst ==L"BATTERY_PRODUCT"||m_wstrProductNameSecond ==L"BOTTLE_PRODUCT"||m_wstrProductNameThird==L"PCC_PRODUCT"||m_wstrProductNameFifth == L"AL_PLASTIC_PRODUCT")
	{
	  centerXY[1]=m_rowCenterTrans;
	  centerXY[0]=m_colCenterTrans;
	}
	else
	{
		CenterDefined(m_newImg,  (float)0.85, centerXY);
	}

	//模板修改、创建时，读取当前计算中心的方式。
	if (m_wstrMatchingType==L"0")
	{ 
		if ((m_rowCenterSet>0)&(m_colCenterSet>0))
		{
			centerXY[1]=m_rowCenterSet;
			centerXY[0]=m_colCenterSet;
		}
	}
	vector_angle_to_rigid(centerXY[1].D(), centerXY[0].D(), 0, centerXY[1].D(), centerXY[0].D(), 3.1416, &HomMat2DRotate);
	//剔除图片范围外的区域
	HTuple  Width211, Height211;
	get_image_size (m_newImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(m_finalRgn, RectangleIntersection, &m_finalRgn);
	//
	projective_trans_region (m_finalRgn, &RegionAffineTrans, HomMat2DRotate, "bilinear");
	union2(m_finalRgn, RegionAffineTrans, &ModelRegion);

	difference(ModelRegion, m_finalRgn, &RegionDifference);
	dilation_circle(RegionDifference, &RegionDilation, 2.5);
	intensity(RegionDilation, m_newImg, &Mean, &Deviation);
	copy_image(m_newImg, &DupImage);
	difference(RegionDilation, m_finalRgn, &RegionDifference1);
	//添加模板区域判断！
	HTuple RegionArea,RegionRow,RegionCol;
	area_center(m_finalRgn,&RegionArea,&RegionRow,&RegionCol);
	if(RegionArea<20)
	{
		if(m_bChs)
			DisplayPromptMessage(_T("模板区域太小，创建模板失败！"), true);
		else
			DisplayPromptMessage(_T(" Create model failed, the model region is smaller"), true);

		m_btnSaveModel.EnableWindow(FALSE);	
		return;
	}
	Herror ret ;
	bool isCircle;//CircleShape 未完成！
	if (Height211<300)
	{
		isCircle=CircleShape(m_newImg,0.3,20000);
	} 
	else
	{
		HTuple AreaSmall;
		AreaSmall=3.14*(Height211*0.16)*(Height211*0.16);
		isCircle=CircleShape(m_newImg,0.3,AreaSmall[0].I());

	}
	//
	if(m_wstrCreateModelAngle == L"CircleShape")
	{
		isCircle=true;
	}

	Hobject CenterAdjustRegion,RegionDiff;
	HTuple  AreaRegionDiff;
	//
	if(m_wstrCreateModelAngle == L"RectShape")
	{
		HTuple RegionArea2,RegionRow2,RegionCol2;
		area_center(RegionDifference,&RegionArea2,&RegionRow2,&RegionCol2);
		if(RegionArea2<100)
		{
			RegionCenterAdjust(ModelRegion,&CenterAdjustRegion,&RegionDiff,&AreaRegionDiff);
			reduce_domain(m_newImg,CenterAdjustRegion, &retImg);
		}
		else
		{
			HTuple Channels;
			count_channels (DupImage, &Channels);
			if (Channels[0].I()==1)
			{		
				paint_region(RegionDifference1, DupImage, &ImageModel, Mean, "fill");	
			}
			else if (Channels[0].I()==3)
			{
				Hobject ImageGray;
				rgb1_to_gray (DupImage, &ImageGray);
				paint_region(RegionDifference1, ImageGray, &ImageModel, Mean, "fill");	
			}
			else
			{

				Hobject ImageGray;
				mean_n (DupImage, &ImageGray);
				paint_region(RegionDifference1, ImageGray, &ImageModel, Mean, "fill");	
			}
			RegionCenterAdjust(ModelRegion,&CenterAdjustRegion,&RegionDiff,&AreaRegionDiff);
			reduce_domain(ImageModel,CenterAdjustRegion, &retImg);
		}
		ret = create_shape_model(retImg, 3, HTuple(-10).Rad(), HTuple(20).Rad(), "auto","auto", "use_polarity", "auto", "auto", &m_modelID);
	}
	else if(isCircle)
	{
		HTuple RegionArea2,RegionRow2,RegionCol2;
		area_center(RegionDifference,&RegionArea2,&RegionRow2,&RegionCol2);
		if(RegionArea2<100)
		{
			RegionCenterAdjust(ModelRegion,&CenterAdjustRegion,&RegionDiff,&AreaRegionDiff);
			reduce_domain(m_newImg,CenterAdjustRegion, &retImg);
		}
		else
		{
			HTuple Channels;
			count_channels (DupImage, &Channels);
			if (Channels[0].I()==1)
			{		
				paint_region(RegionDifference1, DupImage, &ImageModel, Mean, "fill");	
			}
			else if (Channels[0].I()==3)
			{
				Hobject ImageGray;
				rgb1_to_gray (DupImage, &ImageGray);
				paint_region(RegionDifference1, ImageGray, &ImageModel, Mean, "fill");	
			}
			else
			{

				Hobject ImageGray;
				mean_n (DupImage, &ImageGray);
				paint_region(RegionDifference1, ImageGray, &ImageModel, Mean, "fill");	
			}
			RegionCenterAdjust(ModelRegion,&CenterAdjustRegion,&RegionDiff,&AreaRegionDiff);
			reduce_domain(ImageModel,CenterAdjustRegion, &retImg);
		}
		if (m_numLevels==0)
		{
			RegionCenterAdjust(m_finalRgn,&CenterAdjustRegion,&RegionDiff,&AreaRegionDiff);
			reduce_domain(m_newImg,CenterAdjustRegion, &retImg);
			if(m_wstrCreateModelMethod == L"CSM1")    //使用忽略极性的方式创建模板
			{
				ret = create_shape_model(retImg, "auto", HTuple(0).Rad(), HTuple(360).Rad(), "auto","auto", "ignore_local_polarity", "auto", "auto", &m_modelID);
			}
			else
			{
				ret = create_shape_model(retImg, "auto", HTuple(0).Rad(), HTuple(360).Rad(), "auto","auto", "use_polarity", "auto", "auto", &m_modelID);
			}
		}
		else
		{
			if(m_wstrCreateModelMethod == L"CSM1")   //使用忽略极性的方式创建模板
			{
				ret = create_shape_model(retImg, "auto", HTuple(0).Rad(), HTuple(360).Rad(), "auto","auto", "ignore_local_polarity", "auto", "auto", &m_modelID);
			}
			else
			{
				ret = create_shape_model(retImg, "auto", HTuple(0).Rad(), HTuple(360).Rad(), "auto","auto", "use_polarity", "auto", "auto", &m_modelID);
			}
		}
	}
	else
	{

		RegionCenterAdjust(m_finalRgn,&CenterAdjustRegion,&RegionDiff,&AreaRegionDiff);
		reduce_domain(m_newImg,CenterAdjustRegion, &retImg);
		ret = create_shape_model(retImg, 3, HTuple(-10).Rad(), HTuple(20).Rad(), "auto","auto", "use_polarity", "auto", "auto", &m_modelID);
	}


	HTuple modelScore;
	HTuple Scale1;
	if(isCircle)
	{
		find_shape_model(m_newImg, m_modelID, HTuple(0).Rad(), HTuple(360).Rad(), 0.5, 1, 0.5, "least_squares", 0, 0.8, &m_rowModelCenter, &m_colModelCenter, &m_angleModelCenter, &modelScore);
	}
	else
	{
		find_shape_model(m_newImg, m_modelID, HTuple(-10).Rad(), HTuple(20).Rad(), 0.5, 1, 0.5, "least_squares", 0, 0.8, &m_rowModelCenter, &m_colModelCenter, &m_angleModelCenter, &modelScore);
	}
	if(modelScore >= 0.65)
	{
		if(ret == H_MSG_TRUE && m_finalRgn.Id() != H_EMPTY_REGION)
		{
			write_image(m_newImg, "tiff", 0, modelImgFile.c_str());
			write_shape_model(m_modelID, modelFile.c_str());
			write_region(m_finalRgn, modelRgnFile.c_str());
			//
			CString sDataFile = CA2T(modelDatFile.c_str());
			CString sValue;
			sValue.Format(_T("%f"),0.0);
			WritePrivateProfileString(_T("RegionData"), _T("Phi"), sValue, sDataFile);
			sValue.Format(_T("%f"), m_rowModelCenter[0].D());
			WritePrivateProfileString(_T("RegionData"), _T("Row"), sValue, sDataFile);
			sValue.Format(_T("%f"), m_colModelCenter[0].D());
			WritePrivateProfileString(_T("RegionData"), _T("Col"), sValue, sDataFile);
			//
			if(isCircle)
			{
				if(m_bChs)
					DisplayPromptMessage(_T("模板创建成功！"), true);
				else
					DisplayPromptMessage(_T("Model Creation successful!"), true);

				m_editModelName.EnableWindow(FALSE);
			}
			else
			{
				if(m_bChs)
					DisplayPromptMessage(_T("创建模板成功（缩小角度搜索范围）！"), true);
				else
					DisplayPromptMessage(_T("Model Creation successful, plaese decrease the search angle !"), true);
			}
			m_btnSaveModel.EnableWindow(FALSE);

			ResetInternalHalconObject();
			m_listRgns.clear();
			while(!m_oldRgns.empty())
			{
				m_oldRgns.pop();
			}
			m_modelRgn = m_finalRgn;
			DisplayAllRegions();
			SetUnionAndDiffButtonAccessbility();

			m_transferStation.Write(sizeof(char), modelFile.c_str(), modelFile.size() + 1);
			m_bTargetFileChanged = true;
			return;
		}
	}
	if(m_bChs)
		DisplayPromptMessage(_T("创建模板失败！"), true);
	else
		DisplayPromptMessage(_T(" Model creation failed!"), true);

	m_btnSaveModel.EnableWindow(FALSE);
}

void CModelToolDlg::OnBnClickedButtonSaveRegion()
{
	bool bNewRgn = false;
	if(m_wstrTargetFile.find(L".") == wstring::npos)
	{
		bNewRgn = true;
	}
	string strRgnFile;
	if(bNewRgn)
	{
		if (m_wstrMatchingType!=L"8")
		{
			int nCount = m_comboWorkingFolder.GetCount();
			if(nCount <= 0)
			{
				if(m_bChs)
					DisplayPromptMessage(_T("请先创建模板，再绘制检测区域！"), false);
				else
					DisplayPromptMessage(_T("Please create model before draw detect region!"), false);
				return;
			}
			int index = m_comboWorkingFolder.GetCurSel();
			if(index == -1)
			{
				if(m_bChs)
					DisplayPromptMessage(_T("请选择当前区域对应的模板！"), false);
				else
					DisplayPromptMessage(_T("Please choose the corresponding model for current region!"), false);

				return;
			}
			CString sRgnFile = CW2T(m_wstrTargetFile.c_str());
			CString sModelName;
			m_comboWorkingFolder.GetLBText(index, sModelName);
			if(sModelName.GetLength() > 0)
			{
				sRgnFile.Append(_T("\\"));
				sRgnFile.Append(sModelName);

				SYSTEMTIME dt;
				::GetLocalTime(&dt);
				CString sTimestamp;
				sTimestamp.Format(_T("\\%04d%02d%02d%02d%02d%02d.reg"),dt.wYear, dt.wMonth,dt.wDay, dt.wHour, dt.wMinute,dt.wSecond);
				sRgnFile.Append(sTimestamp);
				strRgnFile = CT2A(sRgnFile);
			}
		}
		else 
		{
			CString sRgnFile = CW2T(m_wstrTargetFile.c_str());
			SYSTEMTIME dt;
			::GetLocalTime(&dt);
			CString sTimestamp;
			sTimestamp.Format(_T("\\%04d%02d%02d%02d%02d%02d.reg"),dt.wYear, dt.wMonth,dt.wDay, dt.wHour, dt.wMinute,dt.wSecond);
			sRgnFile.Append(sTimestamp);
			strRgnFile = CT2A(sRgnFile);
		}
	}
	else
	{
		strRgnFile = CW2A(m_wstrTargetFile.c_str());
	}

	ResetInternalHalconObject();
	UnionNewRegions();

	if(m_finalRgn.Id() != H_EMPTY_REGION)
	{
		if (m_wstrMatchingType==L"8")
		{
			wstring wstrRgnFile = CA2W(strRgnFile.c_str());
			m_dictRotatedTestRgn[wstrRgnFile] = m_finalRgn;
			Hobject regionNewCenter;
			HTuple  Width211, Height211;
			get_image_size (m_crtImg, &Width211, &Height211);	
			Hobject regionRota;
			if (abs(m_angleModelCenterSet[0].D())>=0.001)
			{
				HTuple HomMat2DRota;
				vector_angle_to_rigid( m_rowModelCenterSet, m_colModelCenterSet, m_angleModelCenterSet,m_rowModelCenterSet, m_colModelCenterSet, 0.0, &HomMat2DRota);
				hom_mat2d_translate(HomMat2DRota, 0.5, 0.5, &HomMat2DRota);
				hom_mat2d_translate_local(HomMat2DRota, -0.5, -0.5, &HomMat2DRota);
				projective_trans_region (m_finalRgn,&regionRota, HomMat2DRota, "bilinear");
				move_region (regionRota,&regionNewCenter,-m_rowModelCenterSet,-m_colModelCenterSet);
			}
			else
			{
				move_region (m_finalRgn,&regionNewCenter,-m_rowModelCenterSet,-m_colModelCenterSet);
			}
			write_region(regionNewCenter, strRgnFile.c_str());
			if ((fabs(m_rowModelCenterSet[0].D()-Height211[0].D()/2.0-0.119)<0.005)&&(fabs(m_colModelCenterSet[0].D()-Width211[0].D()/2.0-0.119)<0.005))
			{
				CString sFilterTiff=CA2T(strRgnFile.c_str());
				sFilterTiff.Replace(_T(".reg"),_T(".tiff"));
				string strFilterTiff = CT2A(sFilterTiff);
				write_image(m_newImg, "tiff", 0, strFilterTiff.c_str());
			}
			// reset image window;@8,300.3,251.9,0.1//@8,323.5,240.9,3.31
			ResetInternalHalconObject();
			ResetParameterFilter();
			DisplayAllRegions();
			SetUnionAndDiffButtonAccessbility();
			m_btnSaveRgn.EnableWindow(FALSE);
			if(m_bChs)
				DisplayPromptMessage(_T("保存成功！"), true);
			else
				DisplayPromptMessage(_T("Sucess！"), true);
			m_bTargetFileChanged = true;
			//DisableCheckBox();

			m_transferStation.Write(sizeof(char), strRgnFile.c_str(), strRgnFile.size() + 1);
			m_mouseSelectRegion = false;
			return;
		}
		HTuple HomMat2D2,HomMat2DTmp;
		if(m_bUseScaleModel)
		{
			wstring wstrRgnFile = CA2W(strRgnFile.c_str());
			m_dictRotatedTestRgn[wstrRgnFile] = m_finalRgn;

			Hobject regionOpening,regionClosing;
			HTuple HomMat2DS,HomMat2DScale,HomMat2DTmp,HomMat2DAdapted;
			vector_angle_to_rigid(0, 0, m_dNewImageAngle,0,0, m_angleScaleModel, &HomMat2DS);
			double scaleData =1.0/(m_newImagemMatchingScale[0].D());
			hom_mat2d_scale(HomMat2DS, scaleData, scaleData,0,0, &HomMat2DScale);
			hom_mat2d_translate(HomMat2DScale, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

			Hobject RegionMoved1,rotatedRgn;	
			move_region (m_finalRgn, &RegionMoved1, -m_rowNewImagemMatchingCenter, -m_colNewImagemMatchingCenter);
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
			move_region (rotatedRgn,&m_finalRgn, m_rowScaleModelCenter,m_colScaleModelCenter);
		}
		else
		{
			m_dictRotatedTestRgn[m_wstrTargetFile] = m_finalRgn;
			vector_angle_to_rigid(0, 0, m_dNewImageAngle, 0, 0,m_angleModelCenter, &HomMat2D2);//mcAngle
			hom_mat2d_translate(HomMat2D2, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2D2);

			Hobject RegionMoved1,rotatedRgn;	
			move_region (m_finalRgn, &RegionMoved1, -m_rowNewImagemMatchingCenter, -m_colNewImagemMatchingCenter);
			projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2D2, "bilinear");
			move_region (rotatedRgn,&m_finalRgn, m_rowModelCenter,m_colModelCenter);
		}
		Hobject RegionSave;
		HTuple  HomMat2DInv;	
		//剔除图片范围外的区域
		HTuple  Width211, Height211;
		get_image_size (m_crtImg, &Width211, &Height211);
		Hobject RectangleIntersection;
		gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
		intersection(m_finalRgn, RectangleIntersection, &m_finalRgn);
		//
		move_region (m_finalRgn, &RegionSave,-m_rowModelCenter,-m_colModelCenter);
		vector_angle_to_rigid(0, 0, m_angleModelCenter, 0, 0,0.0, &HomMat2D2);//mcAngle
		hom_mat2d_translate(HomMat2D2, 0.5, 0.5, &HomMat2DTmp);
		hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DInv);
		projective_trans_region (RegionSave, &RegionSave, HomMat2DInv, "bilinear");
		write_region(RegionSave, strRgnFile.c_str());
		m_dictTestRgn[m_wstrTargetFile] = m_finalRgn;
	}
	m_finalRgn=m_dictRotatedTestRgn[m_wstrTargetFile];
	HTuple  rgnData;
	tuple_gen_const(8, 0, &rgnData);
	rgnData[0] =0.0;
	rgnData[1] =0.0;
	rgnData[2] =0.0;
	size_t index       = strRgnFile.find_last_of(".");
	string strDataFile = strRgnFile.substr(0, index);
	strDataFile.append(".dat");
	CString sDataFile = CA2T(strDataFile.c_str());
	CString sValue;
	sValue.Format(_T("%f"), rgnData[0].D());
	WritePrivateProfileString(_T("RegionData"), _T("Phi"), sValue, sDataFile);
	sValue.Format(_T("%f"), rgnData[1].D());
	WritePrivateProfileString(_T("RegionData"), _T("Row"), sValue, sDataFile);
	sValue.Format(_T("%f"), rgnData[2].D());
	WritePrivateProfileString(_T("RegionData"), _T("Col"), sValue, sDataFile);
	// reset image window;
	ResetInternalHalconObject();
	ResetParameterFilter();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(FALSE);
	DisplayPromptMessage(_T("Sucess！"), true);
	m_bTargetFileChanged = true;

	m_transferStation.Write(sizeof(char), strRgnFile.c_str(), strRgnFile.size() + 1);
	m_mouseSelectRegion = false;
}

void CModelToolDlg::SearchAvailableModels()
{
	m_comboWorkingFolder.ResetContent();

	CString sFilter  = CW2T(m_wstrTargetFile.c_str());
	sFilter.Append(_T("\\*.*"));

	CFileFind ff;
	BOOL bWorking = ff.FindFile(sFilter);
	while(bWorking)
	{
		bWorking = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())
		{
			CString sPath = ff.GetFilePath();
			wstring wstrPath = CT2W(sPath);
			size_t pos = wstrPath.find_last_of(L"\\");
			wstring wstrModelName = wstrPath.substr(pos + 1);
			if(!m_wstrModelFiles.empty())
			{
				if (wstrModelName.find(m_wstrModelFiles)!= wstring::npos)
				{
					m_comboWorkingFolder.AddString(wstrModelName.c_str());
				}
		
			}
			else
			{
				m_comboWorkingFolder.AddString(wstrModelName.c_str());
			}
	
		
		}
	}
}

void CModelToolDlg::HandleInputFile(const wstring& wstrPath)
{
	ResetInternalHalconObject();
	ResetParameterFilter();
	m_finalRgn.Reset();
	std::string strImgFile = CW2A(m_wstrImgFilePath.c_str());
	read_image(&m_newImg, strImgFile.c_str());
	//
	//黑白图片
	HTuple Channels;
	Hobject MedianCrtImage;
	count_channels(m_newImg,&Channels);
	if(Channels[0].I()==3)
	{
		rgb1_to_gray(m_newImg,&m_newImg);
		median_image(m_newImg,&m_newImg,"circle",1,"continued");
	}
	else if(Channels[0].I()==1)
	{
		m_newImg  = m_newImg;
	}
	else
	{
		mean_n(m_newImg, &m_newImg);
	}
	//
	HTuple nColEnd,nRowEnd;
	get_image_size(m_newImg, &nColEnd, &nRowEnd);
	clear_window(m_imgWndID);
	set_part(m_imgWndID, 0, 0, nRowEnd-1, nColEnd-1);
	disp_obj(m_newImg, m_imgWndID);

	m_nOperationType = (m_wstrFileType == L"0") ? 0 : 1;

	if(m_wstrTargetFile.find(L".reg") == wstring::npos &&
	   m_wstrTargetFile.find(L".shm") == wstring::npos)
	{
		if(!CreateFullDirectory(m_wstrTargetFile.c_str()))
		{
			//TODO
		}
		m_btnSaveModel.EnableWindow(FALSE);
		DisplayImage(strImgFile);
		SearchAvailableModels();
	}
	if(m_nOperationType == 0)
	{
		//多相机工位，模板同名扩展！
	
		if (INVALID_FILE_ATTRIBUTES ==::GetFileAttributes(m_wstrTargetFile.c_str()))
		{
			if(m_wstrModelFiles != L"")
			{
				CString str(m_wstrTargetFile.c_str());

				int i = str.ReverseFind('\\');
				int j = str.ReverseFind('#');
				CString sModelName = str.Mid(i+1,j-i-1);
				str  = str.Left(i);
				i = str.ReverseFind('\\');
				str  = str.Left(i);
				m_wstrTargetFile = str.GetString();
				m_editModelName.SetWindowText(sModelName);

				if(!CreateFullDirectory(m_wstrTargetFile.c_str()))
				{
					//TODO
				}
				m_editModelName.EnableWindow(FALSE);
				m_btnSaveModel.EnableWindow(FALSE);
			}

			DisplayImage(strImgFile);
			SearchAvailableModels();
		}
		else
		{
			DisplayModelUiWidget();
			ProcessAndDisplayModel();
		}
	}
	if(m_nOperationType == 1)
	{
		DisplayRegionUiWidget();
		ProcessAndDisplayRegion();
	}
	RedrawWindow();
}

void CModelToolDlg::ChangeWidgetText()
{
	SetWidgetSring(IDC_BUTTON_SAVE_MODEL,_T("Save"));
	SetWidgetSring(IDC_STATIC_TOOL_PANEL, _T("Tool"));
	SetWidgetSring(IDC_STATIC_GRAY_VALUE_FILTER, _T("Gray Filter"));
	SetWidgetSring(IDC_STATIC_RGN_SIZE_FILTER, _T("Edges Length"));
	SetWidgetSring(IDC_STATIC_CONTRAST, _T("Contrast"));
	SetWidgetSring(IDC_BUTTON_SAVE_RGN, _T("Save"));
	SetWidgetSring(IDC_STATIC_DILATION, _T("Edges Dilation"));
	SetWidgetSring(IDC_STATIC_SELECT_REGION, _T("Select Region"));
	SetWidgetSring(IDC_STATIC_PLANE, _T("Gentle Region"));
	SetWidgetSring(IDC_STATIC_RING, _T("Pattern Region"));
	SetWidgetSring(IDC_STATIC_ALL, _T("All Region"));
	SetWidgetSring(IDC_CHECK_DRAW_SECTOR, _T("Draw Sector"));
	SetWidgetSring(IDC_CHECK_SHOW_FILL,_T("Show Fill"));
	SetWidgetSring(IDC_STATIC_MODEL_NAME, _T("Name"));
	SetWidgetSring(IDC_STATIC_MODEL_ITEM, _T("Models"));
	SetWidgetSring(IDC_BUTTON_RESET_RGN, _T("Reset"));
	SetWidgetSring(IDC_STATIC_NUMLEVELS, _T("Levels"));
	SetWidgetSring(IDC_BUTTON_SAVE_MODEL2,_T("RateImage"));
}

BOOL CModelToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	CRect rcScreen;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcScreen,0);
	::SetWindowPos(m_hWnd,HWND_TOP,0,0,rcScreen.Width(), rcScreen.Height(), SWP_SHOWWINDOW);

	CreateDialogTitleBar(rcScreen);
	CreateImageWindow(rcScreen);
	CreateFileOperationWidget();
	CreateMessagePanel(rcScreen);
	CreateToolPanel();
	CreateSliderWidget();
	CreateTestRegionOperator(rcScreen);
	m_btnSelectRegionExtract.SetCheck(1);
	if(m_wstrImgFilePath != L"" && m_wstrTargetFile != L"")
	{
		HandleInputFile(m_wstrTargetFile);
	}
	m_transferStation.Init(L"SMVS-MODEL_TOOL");
	m_selectRegionExtract = 2;
	if(!m_bChs)
	{
		ChangeWidgetText();
		ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOW);
	}
	return TRUE;
}

void CModelToolDlg::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
//		LoadBackgroundImage(dc);
	}
}

HCURSOR CModelToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CModelToolDlg::LoadBackgroundImage(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CRect rect;
	GetClientRect(&rect);

	CBitmap bakImg;
	if(bakImg.LoadBitmap(IDB_BITMAP_BAK))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		CBitmap* pPngOld = dcMem.SelectObject(&bakImg);
		dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
	}
}

void CModelToolDlg::DisplayImage(std::string& strFilePath)
{
	Hobject obj;
	read_image(&m_crtImg, strFilePath.c_str());

	if(m_crtImg.Id() == H_EMPTY_REGION)
	{
		return;
	}
	HTuple height, width;
	get_image_size(m_crtImg, &width, &height);
	set_part(m_imgWndID, 0, 0, height-1, width-1);
	disp_obj(m_crtImg, m_imgWndID);
}

void CModelToolDlg::SetUnionAndDiffButtonAccessbility()
{
	bool bEnableUnion = false;
	if((m_crtToolType == TOOL_LASSO_CIRCLE || m_crtToolType == TOOL_LASSO_ELLIPSE || m_crtToolType == TOOL_LASSO_RECTANGLE || m_crtToolType == TOOL_LASSO_POLYGON||m_bDrawSector == TRUE)
		&& m_bSliderMoved)
	{
		bEnableUnion = true;
	}
	if(m_crtToolType == TOOL_DRAW_CIRCLE || m_crtToolType == TOOL_DRAW_ELLIPSE || m_crtToolType == TOOL_DRAW_RECTANGLE || m_crtToolType == TOOL_DRAW_POLYGON)
	{
		bEnableUnion = true;
	}
	if(m_crtToolType == TOOL_ZOOM_IN || m_crtToolType ==  TOOL_ZOOM_OUT)
	{
		if(!m_listRgns.empty())
		{
			bEnableUnion = true;
		}
	}
	CBitmap bmpUnion;
	if(bEnableUnion)
	{
		bmpUnion.LoadBitmapW(IDB_BITMAP_UNION);
	}
	else
	{
		bmpUnion.LoadBitmapW(IDB_BITMAP_UNION_DISABLED);
	}
	m_btnUnion.SetBitmap(bmpUnion);
	m_btnUnion.EnableWindow(bEnableUnion);

	bool bEnableDiff = false;
	if(bEnableUnion && m_finalRgn.Id() != H_EMPTY_REGION)
	{
		bEnableDiff = true;
	}
	CBitmap bmpDiff;
	if(bEnableDiff)
	{
		bmpDiff.LoadBitmapW(IDB_BITMAP_DIFFERENCE);
	}
	else
	{
		bmpDiff.LoadBitmapW(IDB_BITMAP_DIFFERENCE_DISABLED);
	}
	m_btnDifference.SetBitmap(bmpDiff);
	m_btnDifference.EnableWindow(bEnableDiff);
}

void ShowFill( Halcon::Hobject DrawRegion,Halcon::HTuple  widthClip,Halcon::Hobject *RegionShow)
{
	Hobject Partitioned;
	HTuple  Area, Row, Column;
	partition_rectangle(DrawRegion, &Partitioned, widthClip, widthClip);
	area_center(Partitioned, &Area, &Row, &Column);
	gen_region_points(RegionShow, Row, Column);
}

void CModelToolDlg::DisplayAllRegions()
{
	if(m_newImg.Id() != H_EMPTY_REGION)
	{
		disp_obj(m_newImg, m_imgWndID);
	}
	set_color(m_imgWndID, "red");
	set_line_style(m_imgWndID,HTuple());
	if(m_lassoedRgn.Id() != H_EMPTY_REGION)
	{
		disp_region(m_lassoedRgn, m_imgWndID);
	}
	for(auto it = m_listRgns.begin(); it != m_listRgns.end(); it++)
	{
		Hobject obj = *it;
		disp_region(obj, m_imgWndID);
	}
	if(m_finalRgn.Id() != H_EMPTY_REGION)
	{
		set_color(m_imgWndID, "blue");
		disp_region(m_finalRgn, m_imgWndID);
		if (m_bShowFill)
		{
			Hobject RegionShow;
			ShowFill( m_finalRgn,3,&RegionShow);
			disp_region(RegionShow, m_imgWndID);
		}
	}
}

void CModelToolDlg::RestoreButtonImage()
{
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_CIRCLE_SEL);
	m_btnCircleSel.SetBitmap(bmp);

	CBitmap bmpEllipseSel;
	bmpEllipseSel.LoadBitmapW(IDB_BITMAP_ELLIPSE_SEL);
	m_btnEllipseSel.SetBitmap(bmpEllipseSel);

	CBitmap bmpRectSel;
	bmpRectSel.LoadBitmapW(IDB_BITMAP_RECTANGLE_SEL);
	m_btnRectSel.SetBitmap(bmpRectSel);

	CBitmap bmpPolygonSel;
	bmpPolygonSel.LoadBitmapW(IDB_BITMAP_POLYGON_SEL);
	m_btnPolygonSel.SetBitmap(bmpPolygonSel);

	CBitmap bmpCircleDraw;
	bmpCircleDraw.LoadBitmapW(IDB_BITMAP_CIRCLE_DRAW);
	m_btnCircleDraw.SetBitmap(bmpCircleDraw);

	CBitmap bmpEllipseDraw;
	bmpEllipseDraw.LoadBitmapW(IDB_BITMAP_ELLIPSE_DRAW);
	m_btnEllipseDraw.SetBitmap(bmpEllipseDraw);

	CBitmap bmpRectDraw;
	bmpRectDraw.LoadBitmapW(IDB_BITMAP_RECTANGLE_DRAW);
	m_btnRectDraw.SetBitmap(bmpRectDraw);

	CBitmap bmpPolygonDraw;
	bmpPolygonDraw.LoadBitmapW(IDB_BITMAP_POLYGON_DRAW);
	m_btnPolygonDraw.SetBitmap(bmpPolygonDraw);

	CBitmap bmpUnion;
	bmpUnion.LoadBitmapW(IDB_BITMAP_UNION);
	m_btnUnion.SetBitmap(bmpUnion);

	CBitmap bmpDiff;
	bmpDiff.LoadBitmapW(IDB_BITMAP_DIFFERENCE);
	m_btnDifference.SetBitmap(bmpDiff);

	CBitmap bmpZoomIn;
	bmpZoomIn.LoadBitmapW(IDB_BITMAP_ZOOM_IN);
	m_btnZoomIn.SetBitmap(bmpZoomIn);

	CBitmap bmpZoomOut;
	bmpZoomOut.LoadBitmapW(IDB_BITMAP_ZOOM_OUT);
	m_btnZoomOut.SetBitmap(bmpZoomOut);

	CBitmap bmpUndo;
	bmpUndo.LoadBitmapW(IDB_BITMAP_UNDO);
	m_btnUndo.SetBitmap(bmpUndo);
}

void CModelToolDlg::ResetInternalHalconObject()
{
	m_reducedImgROI.Reset();
	m_reducedImgStep1.Reset();
	m_lassoedRgn.Reset();
	m_bSliderMoved = false;

	m_dDeltaAngle = 0.0;
	m_oldRgn = m_finalRgn;
	m_bClockwiseRotate = false;
	m_bCounterClockwiseRotate = false;
}

void CModelToolDlg::ResetParameterFilter()
{
	UpdateSliderLclAndUcl(IDC_SLIDER_GRAY_VAL_RANGE, 0, 255);
	UpdateSliderThumb(IDC_SLIDER_GRAY_VAL_RANGE, 0, 255);

	UpdateSliderLclAndUcl(IDC_SLIDER_CONTRAST_VALUE, 2, 120);
	UpdateSliderThumb(IDC_SLIDER_CONTRAST_VALUE,20, 40);

	UpdateSliderLclAndUcl(IDC_SLIDER_REGION_SIZE_FILTER, 0, 500);
	UpdateSliderThumb(IDC_SLIDER_REGION_SIZE_FILTER, 0, 500);

	UpdateSliderLclAndUcl(IDC_SLIDER_DILATION_SIZE, 1, 20);
	UpdateSliderThumb(IDC_SLIDER_DILATION_SIZE, 1, 20);
}

void XLDsToRegion (Halcon::Hobject edges, Halcon::Hobject *region, Halcon::HTuple dilSize)
{
	using namespace Halcon;
	// Local iconic variables 
	Hobject  ObjectSelected2;
	// Local control variables 
	HTuple  Number, RowXLD, ColXLD, Index4, Row11;
	HTuple  Col11;

	count_obj(edges, &Number);
	gen_empty_obj(&(*region));

	RowXLD = HTuple();
	ColXLD = HTuple();
	for (Index4=1; Index4<=Number; Index4+=1)
	{
		select_obj(edges, &ObjectSelected2, Index4);
		get_contour_xld(ObjectSelected2, &Row11, &Col11);
		tuple_concat(RowXLD, Row11, &RowXLD);
		tuple_concat(ColXLD, Col11, &ColXLD);
	}
	gen_region_points(&(*region), RowXLD, ColXLD);
	if (0 != (dilSize>1))
	{
		dilation_circle((*region), &(*region), dilSize);
	}
}

void CreateModelRegion(Hobject imageExtract,HTuple thredLow,HTuple thredHigh,HTuple contrastLow,HTuple contrastHigh,HTuple dilationSize,HTuple minArea,Hobject& modelRegion)
{
	Hobject  ImageReduced, Edges;
	Hobject  SelectedXLD, RegionModel,ExtractRegion,ExtractRegionDilation,DilateRegion;
	// Local control variables 
	threshold(imageExtract, &ExtractRegion, thredLow, thredHigh);
	dilation_circle(ExtractRegion, &ExtractRegionDilation, 4.5);
	reduce_domain(imageExtract, ExtractRegionDilation, &ImageReduced);
	//
	//提取出凹陷区域作为模板
	//*参数
	edges_sub_pix(ImageReduced, &Edges, "canny", 1.5,contrastLow,contrastHigh);
	select_shape_xld(Edges, &SelectedXLD, "contlength", "and", minArea, 99999);
	XLDsToRegion(SelectedXLD, &RegionModel, 1.5);
	intersection(ExtractRegion, RegionModel, &DilateRegion);
	dilation_circle(DilateRegion,&RegionModel,dilationSize);
	modelRegion=RegionModel;
}

void DetectRegion(Hobject image,Hobject regionExtract,HTuple thredLow,HTuple thredHigh,HTuple contrastLow,HTuple contrastHigh,HTuple dilationSize,HTuple minLength,int extInner,Hobject& detectRegion)
{
	// Local iconic variables 
	Hobject  GaussFilter, ImageFFT, ImageConvol;
	Hobject  ImageFiltered, ImageReducedR, ExpandedImage;
	Hobject  ImageReduced,ImageReducedMean, Edges1, RegionTest, RegionFillUp2;
	Hobject  RegionCenterL, RegionDetect1, Edges2;
	Hobject  RegionTest2, RegionExtract, RegionDetect2, ConnectedRegions;
	Hobject  RegionErosion11, RegionDilation, SelectedRegions,LaHuanRegion,RegionDetect_All;
	// Local control variables 
	HTuple  Width, Height, DetectRadius,area1,row1,col1,area2,row2,col2;
	get_image_size(image, &Width, &Height);
	//测试单张图片 去除纹理影响，提取检测区域。
	//*参数
	double smooth_Para=0.3;
	gen_gauss_filter(&GaussFilter,smooth_Para ,smooth_Para, 0, "n", "rft", Width, Height);
	rft_generic(image, &ImageFFT, "to_freq", "none", "complex", Width);
	convol_fft(ImageFFT, GaussFilter, &ImageConvol);
	rft_generic(ImageConvol, &ImageFiltered, "from_freq", "none", "byte", Width);
	//*参数	
	HTuple dilationInt;
	tuple_round (dilationSize, &dilationInt);
	//根据阈值计算有效提取区域
	Hobject  imageExtract,ExtractRegion,ExtractRegionUse;
	reduce_domain(image, regionExtract, &imageExtract);
	threshold(imageExtract, &ExtractRegion, thredLow, thredHigh);
	dilation_circle(ExtractRegion, &ExtractRegionUse, dilationInt+1.5);
	//
	reduce_domain(ImageFiltered, regionExtract, &ImageReducedR);
	expand_domain_gray(ImageReducedR, &ExpandedImage, 15);
	reduce_domain(ExpandedImage, regionExtract, &ImageReducedMean);
	//*参数
	edges_sub_pix(ImageReducedMean, &Edges1, "canny", 1.5,contrastLow,contrastHigh);

	XLDsToRegion(Edges1, &RegionTest,dilationInt+1.5);
	fill_up(RegionTest, &RegionFillUp2);
	//*参数
	erosion_circle(RegionFillUp2, &RegionCenterL, 2.5);
	difference(regionExtract, RegionCenterL, &RegionDetect1);
	//*参数
	reduce_domain(image, RegionCenterL, &ImageReduced);
	edges_sub_pix(ImageReduced, &Edges2, "canny", 1.5, contrastLow,contrastHigh);
	XLDsToRegion(Edges2, &RegionTest2, dilationInt+0.5);
	erosion_circle(RegionTest2, &RegionExtract,0.5);
	difference(RegionCenterL, RegionExtract, &RegionDetect2);
	connection(RegionDetect2, &ConnectedRegions);
	erosion_circle(ConnectedRegions, &RegionErosion11, 1.5);
	dilation_circle(RegionErosion11, &RegionDilation, 1.5);
	select_shape(RegionDilation, &SelectedRegions, "contlength", "and", minLength, 99999);
	union1(SelectedRegions,&LaHuanRegion);
	area_center(RegionDetect1,&area1,&row1,&col1);
	area_center(LaHuanRegion,&area2,&row2,&col2);
	if(extInner==2)
	{
		union2(RegionDetect1,LaHuanRegion,&RegionDetect_All);
		detectRegion=RegionDetect_All;
		intersection(detectRegion, ExtractRegionUse, &detectRegion);
	}
	else if((area1>0)&&(extInner==1))
	{
		detectRegion=RegionDetect1;
		intersection(detectRegion, ExtractRegionUse, &detectRegion);
	}
	else if((area1>0)&&(extInner==0))
	{
		difference(regionExtract, RegionDetect1, &detectRegion);
		intersection(detectRegion, ExtractRegionUse, &detectRegion);
	}
}

void CModelToolDlg::ProcessLassoedRegion(Hobject& targetRgn)
{
	if(m_crtImg.Id() == H_EMPTY_REGION)
	{
		return;
	}
	HTuple  row, col, grayVal;
	reduce_domain(m_crtImg, targetRgn, &m_reducedImgROI);
	Herror ret = get_region_points(targetRgn, &row, &col);
	if(ret != H_MSG_TRUE)
	{
		return;
	}
	get_grayval(m_reducedImgROI, row, col, &grayVal);
	double dUcl, dLcl;
	tuple_max(grayVal, &dUcl);
	tuple_min(grayVal, &dLcl);
	int nLcl, nUcl, nMin, nMax;
	nLcl = nMin = int(dLcl);
	nUcl = nMax = int(dUcl);
	UpdateSliderLclAndUcl(IDC_SLIDER_GRAY_VAL_RANGE, 0, 255);
	UpdateSliderThumb(IDC_SLIDER_GRAY_VAL_RANGE, 0, 255);

	UpdateSliderLclAndUcl(IDC_SLIDER_CONTRAST_VALUE, 2, 120);
	UpdateSliderThumb(IDC_SLIDER_CONTRAST_VALUE, 20, 40);

	UpdateSliderLclAndUcl(IDC_SLIDER_REGION_SIZE_FILTER, 0, 500);
	UpdateSliderThumb(IDC_SLIDER_REGION_SIZE_FILTER, 0,500);
	
	UpdateSliderLclAndUcl(IDC_SLIDER_DILATION_SIZE, 1, 20);
	UpdateSliderThumb(IDC_SLIDER_DILATION_SIZE, 1, 20);
	threshold(m_reducedImgROI, &m_lassoedRgn, nMin, nMax);
	reduce_domain(m_crtImg, m_lassoedRgn, &m_reducedImgStep1);
}

void CModelToolDlg::SetLassoingStyle()
{
	set_color(m_imgWndID, "green");
	set_line_style(m_imgWndID, (HTuple(10).Append(10)));
}

void CModelToolDlg::SetPlottingStyle()
{
	set_color(m_imgWndID, "green");
	set_line_style(m_imgWndID,HTuple());
}

bool CModelToolDlg::CheckRegionValidity(Hobject& rgn)
{
	if(rgn.Id() == H_EMPTY_REGION)
	{
		return false;
	}

	HTuple area, row, column;
	Herror ret = area_center(rgn, &area,&row, &column);
	if(ret != H_MSG_TRUE || area <= 5)
	{
		return false;
	}
	return true;
}

void CModelToolDlg::OnBnClickedButtonCircleSel()
{
	m_btnSaveModel.EnableWindow(false);
	m_btnSaveRgn.EnableWindow(false);
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = false;

	m_crtToolType = TOOL_LASSO_CIRCLE;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_CIRCLE_SEL_CLICKED);
	m_btnCircleSel.SetBitmap(bmp);
	//
	SetLassoingStyle();

	HTuple row, col, radius;
	Hobject targetRgn;
	draw_circle(m_imgWndID, &row, &col, &radius);
	gen_circle(&targetRgn, row, col, radius);
	HTuple  Width211, Height211;
	get_image_size (m_crtImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(targetRgn, RectangleIntersection, &targetRgn);
	if(!CheckRegionValidity(targetRgn))
	{
		return;
	}
	m_firstSetlRgn=targetRgn;
	disp_region(targetRgn, m_imgWndID);
	ProcessLassoedRegion(targetRgn);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
}

void CModelToolDlg::OnBnClickedButtonEllipseSel()
{
	m_btnSaveModel.EnableWindow(false);
	m_btnSaveRgn.EnableWindow(false);
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = false;

	m_crtToolType = TOOL_LASSO_ELLIPSE;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_ELLIPSE_SEL_CLICKED);
	m_btnEllipseSel.SetBitmap(bmp);
	SetLassoingStyle();

	HTuple row, col, radius1, phi, radius2;
	Hobject targetRgn;
	draw_ellipse(m_imgWndID, &row, &col, &phi, &radius1, &radius2);
	gen_ellipse(&targetRgn, row, col, phi, radius1, radius2);
	HTuple  Width211, Height211;
	get_image_size (m_crtImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(targetRgn, RectangleIntersection, &targetRgn);
	if(!CheckRegionValidity(targetRgn))
	{
		return;
	}
	m_firstSetlRgn=targetRgn;
	disp_region(targetRgn, m_imgWndID);
	ProcessLassoedRegion(targetRgn);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
}

void CModelToolDlg::OnBnClickedButtonRectangleSel()
{
	m_btnSaveModel.EnableWindow(false);
	m_btnSaveRgn.EnableWindow(false);
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = false;

	m_crtToolType = TOOL_LASSO_RECTANGLE;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_RECTANGLE_SEL_CLICKED);
	m_btnRectSel.SetBitmap(bmp);

	SetLassoingStyle();
	HTuple row1, col1, row2, col2, phi;
	HTuple length1, length2;
	Hobject targetRgn;
	draw_rectangle2(m_imgWndID, &row1, &col1, &phi, &length1, &length2);
	gen_rectangle2(&targetRgn, row1, col1, phi, length1, length2);
	HTuple  Width211, Height211;
	get_image_size (m_crtImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(targetRgn, RectangleIntersection, &targetRgn);
	if(!CheckRegionValidity(targetRgn))
	{
		return;
	}
	m_firstSetlRgn=targetRgn;
	disp_region(targetRgn, m_imgWndID);
	ProcessLassoedRegion(targetRgn);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
}

void CModelToolDlg::OnBnClickedButtonPolygonSel()
{
	m_btnSaveModel.EnableWindow(false);
	m_btnSaveRgn.EnableWindow(false);
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = false;

	m_crtToolType = TOOL_LASSO_POLYGON;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_POLYGON_SEL_CLICKED);
	m_btnPolygonSel.SetBitmap(bmp);

	SetLassoingStyle();

	Hobject targetRgn,contOut;
	HTuple rows, cols, weights,knots1, rows1, cols1, tangents;
	draw_nurbs_interp(&contOut, m_imgWndID, "true", "true", "true", "true", 3, &rows, &cols,&knots1, &rows1, &cols1, &tangents);
	gen_region_contour_xld(contOut, &targetRgn, "filled");
	HTuple  Width211, Height211;
	get_image_size (m_crtImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(targetRgn, RectangleIntersection, &targetRgn);
	if(!CheckRegionValidity(targetRgn))
	{
		return;
	}
	m_firstSetlRgn=targetRgn;
	disp_region(targetRgn, m_imgWndID);
	ProcessLassoedRegion(targetRgn);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
}

void CModelToolDlg::OnBnClickedButtonCircleDraw()
{
	m_btnSaveModel.EnableWindow(false);
	m_btnSaveRgn.EnableWindow(false);
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = true;

	m_crtToolType = TOOL_DRAW_CIRCLE;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_CIRCLE_DRAW_CLICKED);
	m_btnCircleDraw.SetBitmap(bmp);
	SetPlottingStyle();

	HTuple row, col, radius;
	Hobject targetRgn;
	draw_circle(m_imgWndID, &row, &col, &radius);
	gen_circle(&targetRgn, row, col, radius);
	HTuple  Width211, Height211;
	get_image_size (m_crtImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(targetRgn, RectangleIntersection, &targetRgn);
	if(!CheckRegionValidity(targetRgn))
	{
		return;
	}
	m_listRgns.push_back(targetRgn);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
	//角度定位区域
	m_definedAngleRgn=targetRgn;
	//
}

void CModelToolDlg::OnBnClickedButtonEllipseDraw()
{
	m_btnSaveModel.EnableWindow(false);
	m_btnSaveRgn.EnableWindow(false);
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = true;

	m_crtToolType = TOOL_DRAW_ELLIPSE;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_ELLIPSE_DRAW_CLICKED);
	m_btnEllipseDraw.SetBitmap(bmp);
	SetPlottingStyle();

	HTuple row, col, radius1, phi, radius2;
	Hobject targetRgn;
	draw_ellipse(m_imgWndID, &row, &col, &phi, &radius1, &radius2);
	gen_ellipse(&targetRgn, row, col, phi, radius1, radius2);
	HTuple  Width211, Height211;
	get_image_size (m_crtImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(targetRgn, RectangleIntersection, &targetRgn);
	if(!CheckRegionValidity(targetRgn))
	{
		return;
	}
	m_listRgns.push_back(targetRgn);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
	//角度定位区域
	m_definedAngleRgn=targetRgn;
	//
}

void CModelToolDlg::OnBnClickedButtonRectangleDraw()
{
	m_btnSaveModel.EnableWindow(false);
	m_btnSaveRgn.EnableWindow(false);
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = true;

	m_crtToolType = TOOL_DRAW_RECTANGLE;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_RECTANGLE_DRAW_CLICKED);
	m_btnRectDraw.SetBitmap(bmp);
	SetPlottingStyle();

	HTuple row1, col1, row2, col2, phi;
	HTuple length1, length2;
	Hobject targetRgn;
	draw_rectangle2(m_imgWndID, &row1, &col1, &phi, &length1, &length2);
	gen_rectangle2(&targetRgn, row1, col1, phi, length1, length2);
	HTuple  Width211, Height211;
	get_image_size (m_crtImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(targetRgn, RectangleIntersection, &targetRgn);
	if(!CheckRegionValidity(targetRgn))
	{
		return;
	}
	m_listRgns.push_back(targetRgn);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
	//角度定位区域
	m_definedAngleRgn=targetRgn;
	//
}

void CModelToolDlg::OnBnClickedButtonPolygonDraw()
{
	m_btnSaveModel.EnableWindow(false);
	m_btnSaveRgn.EnableWindow(false);
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = true;
	SetPlottingStyle();

	m_crtToolType = TOOL_DRAW_POLYGON;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_POLYGON_DRAW_CLICKED);
	m_btnPolygonDraw.SetBitmap(bmp);

	Hobject targetRgn,contOut;
	HTuple rows, cols, weights,knots1, rows1, cols1, tangents;
	draw_nurbs_interp(&contOut, m_imgWndID, "true", "true", "true", "true", 3, &rows, &cols,&knots1, &rows1, &cols1, &tangents);
	gen_region_contour_xld(contOut, &targetRgn, "filled");
	HTuple  Width211, Height211;
	get_image_size (m_crtImg, &Width211, &Height211);
	Hobject RectangleIntersection;
	gen_rectangle1 (&RectangleIntersection, 0, 0, Height211-1, Width211-1);
	intersection(targetRgn, RectangleIntersection, &targetRgn);
	if(!CheckRegionValidity(targetRgn))
	{
		return;
	}
	m_listRgns.push_back(targetRgn);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
	//角度定位区域
	m_definedAngleRgn=targetRgn;
	//
}

void CModelToolDlg::OnBnClickedButtonUnion()
{
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = true;
	m_crtToolType = TOOL_UNION;

	UnionNewRegions();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_bFirstEnlarge      = true;
	m_bFirstShrink       = true;
	m_lassoedRgn.Reset();
}

void CModelToolDlg::OnBnClickedButtonDifference()
{
	RestoreButtonImage();
	ResetInternalHalconObject();
	m_bLassoFinished = true;
	m_crtToolType = TOOL_DIFFERENCE;

	DifferenceNewRegions();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_bFirstEnlarge      = true;
	m_bFirstShrink       = true;
	m_lassoedRgn.Reset();
}

void CModelToolDlg::ZoomImageWindow(bool bZoomIn, CPoint pt)
{
	HTuple imgWidth, imgHeight;
	get_image_size(m_crtImg, &imgWidth, &imgHeight);

	HTuple wndRow, wndCol, wndWidth, wndHeight;
	get_window_extents(m_imgWndID, &wndRow, &wndCol, &wndWidth, &wndHeight);

	int nWndHeight = wndHeight[0].I();
	int nWndWidth  = wndWidth[0].I();

	double xRatio = (double)nWndWidth / imgWidth[0].I();
	double yRatio = (double)nWndHeight / imgHeight[0].I();
	int nDeltaCol = int((pt.x - wndCol[0].I()) / xRatio);
	int nDeltaRow = int((pt.y - wndRow[0].I()) / yRatio);

	if(bZoomIn)
	{
		if(m_dZoomRatio < 10.4)
		{
			m_dZoomRatio += 0.4;
		}
		if(m_dZoomRatio > 10.4)
		{
			m_dZoomRatio = 10.4;
		}
	}
	else
	{
		if(m_dZoomRatio > 1.0)
		{
			m_dZoomRatio -= 0.4;
		}
		if(m_dZoomRatio < 1.0)
		{
			m_dZoomRatio = 1.0;
		}
	}
	int nRowBgn = int(nDeltaRow * (1.0 - 1.0 / m_dZoomRatio));
	int nRowEnd = int(nDeltaRow + (imgHeight[0].I() - nDeltaRow) * (1.0 / m_dZoomRatio));

	int nColBgn = int(nDeltaCol * (1.0 - 1.0 / m_dZoomRatio));
	int nColEnd = int(nDeltaCol + (imgWidth[0].I() - nDeltaCol) * (1.0 / m_dZoomRatio));

	clear_window(m_imgWndID);
	set_part(m_imgWndID, nRowBgn, nColBgn, nRowEnd-1, nColEnd-1);
	disp_obj(m_crtImg, m_imgWndID);
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
}

void CModelToolDlg::OnBnClickedButtonZoomIn()
{
	RestoreButtonImage();
	m_crtToolType = TOOL_ZOOM_IN;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_ZOOM_IN_CLICKED);
	m_btnZoomIn.SetBitmap(bmp);
	SetUnionAndDiffButtonAccessbility();
}

void CModelToolDlg::OnBnClickedButtonZoomOut()
{
	RestoreButtonImage();
	m_crtToolType = TOOL_ZOOM_OUT;
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_ZOOM_OUT_CLICKED);
	m_btnZoomOut.SetBitmap(bmp);
	SetUnionAndDiffButtonAccessbility();
}

BOOL CModelToolDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(nFlags != MK_CONTROL || m_crtImg.Id() == H_EMPTY_REGION || !m_rcImgWnd.PtInRect(pt))
	{
		return FALSE;
	}

	ZoomImageWindow((zDelta > 0), pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CModelToolDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_crtImg.Id() != H_EMPTY_REGION && m_rcImgWnd.PtInRect(point) == TRUE)
	{
		if(m_crtToolType == TOOL_ZOOM_IN)
		{
			ZoomImageWindow(true, point);
		}
		if(m_crtToolType == TOOL_ZOOM_OUT)
		{
			ZoomImageWindow(false, point);
		}
	}
	if(!m_rcGrayValFilter.PtInRect(point))
	{
		m_grayValFilter.SendStopSliderMessage();
	}
	if(!m_rcCtrstFilter.PtInRect(point))
	{
		m_ctrstFilter.SendStopSliderMessage();
	}
	if(!m_rcRgnSzFilter.PtInRect(point))
	{
		m_rgnSzFilter.SendStopSliderMessage();
	}
	if(!m_rcDilationFilter.PtInRect(point))
	{
		m_dilationSzFilter.SendStopSliderMessage();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CModelToolDlg::OnBnClickedButtonUndo()
{
	RestoreButtonImage();
	m_crtToolType = TOOL_UNDO;
	ResetInternalHalconObject();

	UndoOneStep();

	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	RestoreButtonImage();
}

HBRUSH CModelToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int ctrlID = pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetBkColor(RGB(255,255,255));
	}
	else
	{
		if(ctrlID == IDC_STATIC_DLG_TITLE ||
			ctrlID == IDC_STATIC_MSG_PANEL ||
			ctrlID == IDC_STATIC_GRAY_VALUE_FILTER ||
			ctrlID == IDC_STATIC_GRAY_LCL ||
			ctrlID == IDC_STATIC_GRAY_MIN ||
			ctrlID == IDC_STATIC_GRAY_MAX ||
			ctrlID == IDC_STATIC_GRAY_UCL ||
			ctrlID == IDC_STATIC_CONTRAST ||
			ctrlID == IDC_STATIC_CONTRAST_LCL ||
			ctrlID == IDC_STATIC_CONTRAST_UCL ||
			ctrlID == IDC_STATIC_CONTRAST_VALUE ||
			ctrlID == IDC_STATIC_CONTRAST_HIGH_VALUE ||
			ctrlID == IDC_STATIC_RGN_SIZE_FILTER ||
			ctrlID == IDC_STATIC_GRN_SIZE_LCL ||
			ctrlID == IDC_STATIC_RGN_SIZE_MIN ||
			ctrlID == IDC_STATIC_RGN_SIZE_UCL || 
			ctrlID == IDC_STATIC_DILATION ||
			ctrlID == IDC_STATIC_DILATION_LCL ||
			ctrlID == IDC_STATIC_DILATION_UCL ||
			ctrlID == IDC_STATIC_DILATION_VALUE ||
			ctrlID == IDC_STATIC_SELECT_REGION ||
			ctrlID == IDC_RADIO_PLANE ||
			ctrlID == IDC_RADIO_RING ||
			ctrlID == IDC_RADIO_ALL ||
			ctrlID == IDC_STATIC_PLANE ||
			ctrlID == IDC_STATIC_RING ||
			ctrlID == IDC_STATIC_ALL ||
			ctrlID == IDC_CHECK_DRAW_SECTOR ||
			ctrlID == IDC_CHECK_SHOW_FILL ||
			ctrlID == IDC_STATIC_MODEL_NAME ||
			ctrlID == IDC_STATIC_NUMLEVELS ||
			ctrlID == IDC_STATIC_MODEL_ITEM)
		{
			pDC->SetBkMode(TRANSPARENT);
			CRect rc;
			pWnd->GetWindowRect(rc);
			ScreenToClient(&rc);
			CDC* dc = GetDC();
			if(ctrlID == IDC_STATIC_MSG_PANEL)
			{
				if(m_bOpRslt)
					pDC->SetTextColor(RGB(20,250,100));
				else
					pDC->SetTextColor(RGB(200,20,20));
			}
			if(ctrlID != IDC_STATIC_DLG_TITLE && ctrlID != IDC_STATIC_MSG_PANEL)
			{
				pDC->SetTextColor(RGB(250,250,250));
			}
			pDC->BitBlt(0,0, rc.Width(), rc.Height(), dc, rc.left, rc.top, SRCCOPY);
			ReleaseDC(dc);
			return HBRUSH(GetStockObject(NULL_BRUSH));
		}
	}
	return hbr;
}

void CModelToolDlg::UpdateSliderLclAndUcl(int nSliderID, int nLCL, int nUCL)
{
	CRangeSlider* pRangeSlider = (CRangeSlider*)GetDlgItem(nSliderID);
	if(pRangeSlider == nullptr)
		return;

	if(nLCL > nUCL)
	{
		return;
	}
	if(nLCL == nUCL)
		nLCL = 0;

	int nOldLcl = pRangeSlider->GetChannelMin();
	int nOldUcl = pRangeSlider->GetChannelMax();
	int nDelta = nOldUcl - nOldLcl;

	if(nDelta == 0)
		return;

	pRangeSlider->SetChannelMin(nLCL);
	pRangeSlider->SetChannelMax(nUCL);

	CString sLcl, sUcl;
	sLcl.Format(_T("%d"), nLCL);
	sUcl.Format(_T("%d"), nUCL);
}

void CModelToolDlg::UpdateSliderThumb(int nSliderID, int nMin, int nMax)
{
	CRangeSlider* pRangeSlider = (CRangeSlider*)GetDlgItem(nSliderID);
	if(pRangeSlider == nullptr)
		return;

	pRangeSlider->SetThumbMin(nMin);
	pRangeSlider->SetThumbMax(nMax);
	AdjustSlider(nSliderID);

	CRect rc;
	pRangeSlider->GetWindowRect(rc);
	InvalidateRect(rc);
}

void CModelToolDlg::AdjustSlider(int nSliderID)
{
	CRangeSlider* pRangeSlider = (CRangeSlider*)GetDlgItem(nSliderID);

	CString cstrMin;
	CString cstrMax;
	int nMin = pRangeSlider->GetThumbMin();
	int nMax = pRangeSlider->GetThumbMax();
	cstrMin.Format(_T("%d"),nMin);
	cstrMax.Format(_T("%d"),nMax);

	int nLCL = pRangeSlider->GetChannelMin();
	int nUCL = pRangeSlider->GetChannelMax();
	CString sLcl, sUcl;
	sLcl.Format(_T("%d"), nLCL);
	sUcl.Format(_T("%d"), nUCL);

	CRect rcSlider, rcMin, rcMax, rc;
	if(nSliderID == IDC_SLIDER_GRAY_VAL_RANGE)
	{
		m_grayValMin.GetWindowRect(rc);
		rcSlider = m_rcGrayValFilter;
		if(pRangeSlider->GetThumbMinChangedFlag())
		{
			int nDelta = (nUCL == nLCL) ? 0 : (nMin - nLCL) * rcSlider.Width() / (nUCL - nLCL); 
			rcMin.left   = rcSlider.left + nDelta;
			rcMin.top    = rc.top;
			rcMin.right  = rcMin.left + 30;
			rcMin.bottom = rc.bottom;
			m_grayValMin.MoveWindow(rcMin);
			m_grayValMin.SetWindowTextW(cstrMin);
			InvalidateRect(rcMin);
			pRangeSlider->SetThumbMinChangedFlag(false);
		}
		if(pRangeSlider->GetThumbMaxChangedFlag())
		{
			int nDelta = (nUCL == nLCL) ? 0 : int((nUCL - nMax) * rcSlider.Width() / (nUCL - nLCL));
			rcMax.left   = rcSlider.right - nDelta;
			rcMax.top    = rc.top;
			rcMax.right  = rcMax.left + 30;
			rcMax.bottom = rc.bottom;
			m_grayValMax.MoveWindow(rcMax);
			m_grayValMax.SetWindowTextW(cstrMax);
			InvalidateRect(rcMax);
			pRangeSlider->SetThumbMaxChangedFlag(false);
		}
	}
	if(nSliderID == IDC_SLIDER_CONTRAST_VALUE)
	{
		m_ctrstValue.GetWindowRect(rc);
		rcSlider = m_rcCtrstFilter;
		if(pRangeSlider->GetThumbMinChangedFlag())
		{
			int nDelta = (nUCL == nLCL) ? 0 : (nMin - nLCL) * rcSlider.Width() / (nUCL - nLCL);
			rcMin.left   = rcSlider.left + nDelta;
			rcMin.top    = rc.top;
			rcMin.right  = rcMin.left + 30;
			rcMin.bottom = rc.bottom;
			m_ctrstValue.MoveWindow(rcMin);
			m_ctrstValue.SetWindowTextW(cstrMin);
			InvalidateRect(rcMin);
			pRangeSlider->SetThumbMinChangedFlag(false);
		}
		if(pRangeSlider->GetThumbMaxChangedFlag())
		{
			int nDelta = (nUCL == nLCL) ? 0 : int((nUCL - nMax) * rcSlider.Width() / (nUCL - nLCL));
			rcMax.left   = rcSlider.right - nDelta;
			rcMax.top    = rc.top;
			rcMax.right  = rcMax.left + 45;
			rcMax.bottom = rc.bottom;
			m_ctrstValueHigh.MoveWindow(rcMax);
			m_ctrstValueHigh.SetWindowTextW(cstrMax);
			InvalidateRect(rcMax);
			pRangeSlider->SetThumbMinChangedFlag(false);
		}
	}
	if(nSliderID == IDC_SLIDER_DILATION_SIZE)
	{
		m_dilationValue.GetWindowRect(rc);
		rcSlider = m_rcDilationFilter;
		if(pRangeSlider->GetThumbMinChangedFlag())
		{
			int nDelta = (nUCL == nLCL) ? 0 : (nMin - nLCL) * rcSlider.Width() / (nUCL - nLCL);
			rcMin.left   = rcSlider.left + nDelta;
			rcMin.top    = rc.top;
			rcMin.right  = rcMin.left + 30;
			rcMin.bottom = rc.bottom;
			m_dilationValue.MoveWindow(rcMin);
			m_dilationValue.SetWindowTextW(cstrMin);
			InvalidateRect(rcMin);
			pRangeSlider->SetThumbMinChangedFlag(false);
		}
	}
	if(nSliderID == IDC_SLIDER_REGION_SIZE_FILTER)
	{
		m_rgnSzMin.GetWindowRect(rc);
		rcSlider = m_rcRgnSzFilter;
		if(pRangeSlider->GetThumbMinChangedFlag())
		{
			int nDelta = (nUCL == nLCL) ? 0 : int((nMin - nLCL) * rcSlider.Width() / (nUCL - nLCL));
			rcMin.left   = rcSlider.left + nDelta;
			rcMin.top    = rc.top;
			rcMin.right  = rcMin.left + 45;
			rcMin.bottom = rc.bottom;
			m_rgnSzMin.MoveWindow(rcMin);
			m_rgnSzMin.SetWindowTextW(cstrMin);
			InvalidateRect(rcMin);
			pRangeSlider->SetThumbMinChangedFlag(false);
		}
	}
}

LRESULT CModelToolDlg::OnRangeSliderChanged(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);

	int nSliderID = int(wParam);
	AdjustSlider(nSliderID);
	m_bSliderMoved = true;
	//
	int thredLow     = m_grayValFilter.GetThumbMin();
	int thredLowHigh = m_grayValFilter.GetThumbMax();
	int contrastLow  = m_ctrstFilter.GetThumbMin();
	int contrastHigh = m_ctrstFilter.GetThumbMax();
	int rgnExtractLow= m_rgnSzFilter.GetThumbMin();
	float dilationSize = (float)(m_dilationSzFilter.GetThumbMin()/2.0);
	//
	if(m_bDrawSector == FALSE)
	{
		if(m_createModel)
		{
			if(m_newImg.Id() != H_EMPTY_REGION && m_reducedImgROI.Id() != H_EMPTY_REGION)
			{
				Hobject modelRegion,m_reducedNewImg;
				reduce_domain(m_newImg, m_firstSetlRgn, &m_reducedNewImg);
				CreateModelRegion(m_reducedNewImg,thredLow,thredLowHigh,contrastLow,contrastHigh,dilationSize,rgnExtractLow,modelRegion);
				if(m_listRgns.empty())
				{
					m_listRgns.push_back(modelRegion);
				}
				else
				{
					if(!m_bLassoFinished)
					{
						m_listRgns.pop_back();
						m_listRgns.push_back(modelRegion);
					}
				}
				DisplayAllRegions();
				SetUnionAndDiffButtonAccessbility();
			}
		}
		else
		{
			if(m_newImg.Id() != H_EMPTY_REGION && m_reducedImgROI.Id() != H_EMPTY_REGION)
			{
				Hobject modelRegion;
				if(!m_mouseSelectRegion)
				{
					gen_empty_obj(&m_selectRegion);	
					DetectRegion(m_newImg,m_firstSetlRgn,thredLow,thredLowHigh,contrastLow,contrastHigh,dilationSize,rgnExtractLow,	m_selectRegionExtract,modelRegion);
					connection(modelRegion, &m_connectedRegions);
					if(m_listRgns.empty())
					{
						m_listRgns.push_back(modelRegion);
					}
					else
					{
						if(!m_bLassoFinished)
						{
							m_listRgns.pop_back();
							m_listRgns.push_back(modelRegion);
						}
					}
					DisplayAllRegions();
					SetUnionAndDiffButtonAccessbility();
				}
			}
		}
	}
	else
	{
		if(m_newImg.Id() != H_EMPTY_REGION )
		{
			HTuple  ImageWidth,ImageHeight;
			get_image_size(m_newImg, &ImageWidth, &ImageHeight);
			Hobject Rectangle1,XYTransRegion;
			HTuple centerXY;
			if( m_wstrProductNameFirst ==L"BATTERY_PRODUCT"||m_wstrProductNameSecond ==L"BOTTLE_PRODUCT"||m_wstrProductNameThird==L"PCC_PRODUCT"||m_wstrProductNameFifth == L"AL_PLASTIC_PRODUCT")
			{
				centerXY[1]=m_rowCenterTrans;
				centerXY[0]=m_colCenterTrans;
			}
			else
			{
				CenterDefined(m_newImg,  (float)0.85, centerXY);
			}
			
			if(thredLowHigh<=thredLow+1)
			{
				thredLowHigh=thredLow+2;
			}
			if(contrastHigh<=contrastLow)
			{
				contrastHigh=contrastLow+1;
			}
			if((centerXY[0].D()>10)&&(centerXY[1].D()>10))
			{
				gen_rectangle1(&Rectangle1, 0, 0, thredLowHigh-thredLow, ImageWidth);
				polar_trans_region_inv(Rectangle1, &XYTransRegion, centerXY[1].D(), centerXY[0].D(),(contrastLow/180.0)*3.14, (contrastHigh/180.0)*3.14, thredLow, thredLowHigh, ImageWidth, thredLowHigh-thredLow, ImageWidth, ImageHeight, "bilinear");
			}
			else
			{
				gen_rectangle1(&Rectangle1, 0, 0, thredLowHigh-thredLow, ImageWidth);
				polar_trans_region_inv(Rectangle1, &XYTransRegion, ImageHeight/2.0, ImageWidth/2.0,(contrastLow/180.0)*3.14, (contrastHigh/180.0)*3.14, thredLow, thredLowHigh, ImageWidth, thredLowHigh-thredLow, ImageWidth, ImageHeight, "bilinear");
			}
			if(1)//m_createModel
			{
				if(m_listRgns.empty())
				{
					m_listRgns.push_back(XYTransRegion);
				}
				else
				{
					m_listRgns.pop_back();
					m_listRgns.push_back(XYTransRegion);
				}
			}
			DisplayAllRegions();
			SetUnionAndDiffButtonAccessbility();
			RestoreButtonImage();
		}
	}
	return 0;
}

void CModelToolDlg::UnionNewRegions()
{
	if(m_bExternalRgn && m_finalRgn.Id() != H_EMPTY_REGION)
	{
		m_listRgns.push_front(m_finalRgn);
		m_bExternalRgn = false;
		m_finalRgn.Reset();
	}
	if(m_listRgns.empty())
	{
		return;
	}
	for(auto it = m_listRgns.begin(); it != m_listRgns.end(); it++)
	{
		Hobject obj = *it;
		union2(m_finalRgn, obj, &m_finalRgn); 
		ACTION_STEP actionStep(obj, ACTION_UNION);
		m_oldRgns.push(actionStep);
	}
	m_listRgns.clear();
	if(m_bNewImageMatchingOK)
	{
		m_btnSaveRgn.EnableWindow(TRUE);
	}
	else
	{
		if(!m_createModel)
		{
			if(m_bChs)
				DisplayPromptMessage(_T("区域保存启动失败:请先检查模板匹配是否成功，再绘制检测区域!"), false);
			else
				DisplayPromptMessage(_T("Save region failed.Please check if the model creation sucessful!"), false);
		}
	}
	m_btnSaveModel.EnableWindow(TRUE);

	UpdateData(TRUE);
}

void CModelToolDlg::DifferenceNewRegions()
{
	if(m_listRgns.empty() || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}
	if(m_bExternalRgn)
	{
		ACTION_STEP actionStep(m_finalRgn, ACTION_UNION);
		m_oldRgns.push(actionStep);
		m_bExternalRgn = false;
	}
	for(auto it = m_listRgns.begin(); it != m_listRgns.end(); it++)
	{
		Hobject obj = *it;
		difference(m_finalRgn, obj, &m_finalRgn);
		ACTION_STEP actionStep(obj, ACTION_DIFFERENCE);
		m_oldRgns.push(actionStep);
	}

	if(m_bNewImageMatchingOK)
	{
		m_btnSaveRgn.EnableWindow(TRUE);
	}
	else
	{
		if(!m_createModel)
		{
			if(m_bChs)
				DisplayPromptMessage(_T("区域保存启动失败:请先检查模板匹配是否成功，再绘制检测区域!"), false);
			else
				DisplayPromptMessage(_T("Save region failed.Please check if the model creation sucessful!"), false);
		}
	}
	m_btnSaveModel.EnableWindow(TRUE);

	m_listRgns.clear();
}

void CModelToolDlg::UndoOneStep()
{
	if(!m_listRgns.empty())
	{
		m_listRgns.pop_back();
		return;
	}

	size_t sz = m_oldRgns.size();
	if(sz == 0)
	{
		m_finalRgn.Reset();
		return;
	}
	if(m_finalRgn.Id() != H_EMPTY_REGION && sz == 1)
	{
		ACTION_STEP as = m_oldRgns.top();
		if(as.m_actionType == ACTION_UNION)
		{
			difference(m_finalRgn, as.m_rgn, &m_finalRgn);
		}
		if(as.m_actionType == ACTION_DIFFERENCE)
		{
			union2(m_finalRgn, as.m_rgn, &m_finalRgn);
		}
		m_oldRgns.pop();
		return;
	}

	m_oldRgns.pop();
	std::stack<ACTION_STEP> tmpSteps = m_oldRgns;
	std::stack<ACTION_STEP> steps;
	while(!tmpSteps.empty())
	{
		ACTION_STEP as = tmpSteps.top();
		tmpSteps.pop();
		steps.push(as);
	}
	m_finalRgn.Reset();
	while(!steps.empty())
	{
		ACTION_STEP as = steps.top();
		steps.pop();
		if(as.m_actionType == ACTION_UNION)
		{
			union2(m_finalRgn, as.m_rgn, &m_finalRgn);
		}
		if(as.m_actionType == ACTION_DIFFERENCE)
		{
			difference(m_finalRgn, as.m_rgn, &m_finalRgn);
		}
	}
}

void CModelToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == DLG_TIMER_ID)
	{
		SetDlgItemTextW(IDC_STATIC_MSG_PANEL, _T(""));
		RedrawWindow();
		KillTimer(DLG_TIMER_ID);
	}
	CDialogEx::OnTimer(nIDEvent);
}

int CModelToolDlg::GetMoveStep()
{
	UpdateData(TRUE);
	CString sValue;
	m_editMovePixel.GetWindowTextW(sValue);
	wstring wstrValue = CT2W(sValue);
	int nValue = (int)wcstol(wstrValue.c_str(), nullptr, 10);
	return nValue;
}

void CModelToolDlg::OnBnClickedButtonMoveUp()
{
	if(m_crtImg.Id() == H_EMPTY_REGION || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}
	Hlong nWidth, nHeight;
	get_image_size(m_crtImg, &nWidth, &nHeight);
	HTuple length,dRow1, dColumn1,dRow2 , dColumn2;
	smallest_rectangle1(m_finalRgn,&dRow1, &dColumn1,&dRow2,&dColumn2);
	tuple_length(dRow1,&length);
	if(length<1)
	{
		return;
	}
	int nValue = GetMoveStep();
	int nRow = (int)dRow1[0].I();
	if(nRow- nValue <= 0 || nValue > 512)
	{
		return;
	}
	move_region(m_finalRgn, &m_finalRgn,(Hlong)nValue*-1, 0);
	m_dictRotatedTestRgn[m_wstrCrtRgnName] = m_finalRgn;

	ResetInternalHalconObject();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(TRUE);
}

void CModelToolDlg::OnBnClickedButtonMoveDown()
{
	if(m_crtImg.Id() == H_EMPTY_REGION || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}
	Hlong nWidth, nHeight;
	get_image_size(m_crtImg, &nWidth, &nHeight);
	HTuple length,dRow1, dColumn1,dRow2 , dColumn2;
	smallest_rectangle1(m_finalRgn,&dRow1, &dColumn1,&dRow2,&dColumn2);
	tuple_length(dRow1,&length);
	if(length<1)
	{
		return;
	}
	int nValue = GetMoveStep();
	int nRow = (int)dRow2[0].I();
	if(nRow+nValue >= nHeight|| nValue > 512)
	{
		return;
	}
	move_region(m_finalRgn, &m_finalRgn,(Hlong)nValue, 0);
	m_dictRotatedTestRgn[m_wstrCrtRgnName] = m_finalRgn;
	ResetInternalHalconObject();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(TRUE);
}

void CModelToolDlg::OnBnClickedButtonMoveLeft()
{
	if(m_crtImg.Id() == H_EMPTY_REGION || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}
	Hlong nWidth, nHeight;
	get_image_size(m_crtImg, &nWidth, &nHeight);
	HTuple length,dRow1, dColumn1,dRow2 , dColumn2;
	smallest_rectangle1(m_finalRgn,&dRow1, &dColumn1,&dRow2,&dColumn2);
	tuple_length(dRow1,&length);
	if(length<1)
	{
		return;
	}
	int nValue = GetMoveStep();
	int nColumn = (int)dColumn1[0].I();
	if(nColumn-nValue <= 0|| nValue > 512)
	{
		return;
	}
	move_region(m_finalRgn, &m_finalRgn, 0, (Hlong)nValue*-1);
	m_dictRotatedTestRgn[m_wstrCrtRgnName] = m_finalRgn;
	ResetInternalHalconObject();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(TRUE);
}

void CModelToolDlg::OnBnClickedButtonMoveRight()
{
	if(m_crtImg.Id() == H_EMPTY_REGION || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}
	Hlong nWidth, nHeight;
	get_image_size(m_crtImg, &nWidth, &nHeight);
	HTuple length,dRow1, dColumn1,dRow2 , dColumn2;
	smallest_rectangle1(m_finalRgn,&dRow1, &dColumn1,&dRow2,&dColumn2);
	tuple_length(dRow1,&length);
	if(length<1)
	{
		return;
	}
	int nValue = GetMoveStep();
	int nColumn = (int)dColumn2[0].I();
	if(nColumn+nValue >= nWidth|| nValue > 512)
	{
		return;
	}
	move_region(m_finalRgn, &m_finalRgn, 0, (Hlong)nValue);

	m_dictRotatedTestRgn[m_wstrCrtRgnName] = m_finalRgn;
	ResetInternalHalconObject();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(TRUE);
}

void CModelToolDlg::OnBnClickedButtonEnlarge()
{

	if(m_crtImg.Id() == H_EMPTY_REGION || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}
	if(m_bFirstEnlarge)
	{
		m_oldRgnEnlargeShrink = m_finalRgn;
		m_bFirstEnlarge=false;
		m_bFirstShrink=true;
		m_dEnlargeShrink=0.0;
	}
	CString sValue;
	m_editScalePixel.GetWindowTextW(sValue);
	wstring wstrValue = CT2W(sValue);
	double dValue = wcstod(wstrValue.c_str(), nullptr);
	m_dEnlargeShrink += dValue;
	if(m_dEnlargeShrink > 511.5)
	{
		return;
	}
	double dEnlarge=0.0;
	int dEnlargeInt=(int)m_dEnlargeShrink;
	if(m_dEnlargeShrink-dEnlargeInt<0.2)
	{
		dEnlarge=m_dEnlargeShrink+0.5;
	}
	else
		dEnlarge=m_dEnlargeShrink;
	dilation_circle(m_oldRgnEnlargeShrink, &m_finalRgn, dEnlarge);
	m_dictRotatedTestRgn[m_wstrCrtRgnName] = m_finalRgn;
	ResetInternalHalconObject();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(TRUE);
}

void CModelToolDlg::OnBnClickedButtonShrink()
{
	if(m_crtImg.Id() == H_EMPTY_REGION || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}
	if(m_bFirstShrink)
	{
		m_oldRgnEnlargeShrink = m_finalRgn;
		m_bFirstEnlarge=true;
		m_bFirstShrink=false;
		m_dEnlargeShrink=0.0;
	}
	CString sValue;
	m_editScalePixel.GetWindowTextW(sValue);
	wstring wstrValue = CT2W(sValue);
	double dValue = wcstod(wstrValue.c_str(), nullptr);
	m_dEnlargeShrink += dValue;
	if(m_dEnlargeShrink > 511.5)
	{
		return;
	}
	double dEnlarge=0.0;
	int dEnlargeInt=(int)m_dEnlargeShrink;
	if(m_dEnlargeShrink-dEnlargeInt<0.2)
	{
		dEnlarge=m_dEnlargeShrink+0.5;
	}
	else
		dEnlarge=m_dEnlargeShrink;
	erosion_circle(m_oldRgnEnlargeShrink, &m_finalRgn, dEnlarge);

	m_dictRotatedTestRgn[m_wstrCrtRgnName] = m_finalRgn;
	ResetInternalHalconObject();
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(TRUE);
}

void CModelToolDlg::OnBnClickedButtonRotateClockwise()
{
	if(m_crtImg.Id() == H_EMPTY_REGION || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}

	CString sValue;
	m_editAngle.GetWindowTextW(sValue);
	wstring wstrValue = CT2W(sValue);
	double dValue = wcstod(wstrValue.c_str(), nullptr);
	if(!m_bCounterClockwiseRotate)
	{
		m_oldRgn = m_finalRgn;
		m_bCounterClockwiseRotate=true;
		m_bClockwiseRotate=false;
		m_dDeltaAngle = 0.0;
	}
	m_dDeltaAngle += dValue;
	Hlong nArea;
	double dRow = 0.0, dColumn = 0.0;
	area_center(m_oldRgn, &nArea, &dRow, &dColumn);
	//
	HTuple HomMat2DS,HomMat2DTmp,HomMat2DAdapted;
	vector_angle_to_rigid(0, 0, 0, 0, 0,  -1.0*(m_dDeltaAngle/180.0)*3.1415926, &HomMat2DS);
	hom_mat2d_translate(HomMat2DS, 0.5, 0.5, &HomMat2DTmp);
	hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

	Hobject rotatedRgn,RegionMoved1;	
	move_region (m_oldRgn, &RegionMoved1,  -dRow, -dColumn);
	projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DS, "bilinear");
	move_region (rotatedRgn,&m_finalRgn, dRow, dColumn) ;
	//
	m_dictRotatedTestRgn[m_wstrCrtRgnName] = m_finalRgn;
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(TRUE);
}

void CModelToolDlg::OnBnClickedButtonCounterClockwise()
{
	if(m_crtImg.Id() == H_EMPTY_REGION || m_finalRgn.Id() == H_EMPTY_REGION)
	{
		return;
	}
	CString sValue;
	m_editAngle.GetWindowTextW(sValue);
	wstring wstrValue = CT2W(sValue);
	double dValue = wcstod(wstrValue.c_str(), nullptr);
	if(!m_bClockwiseRotate)
	{
		m_oldRgn = m_finalRgn;
		m_bCounterClockwiseRotate=false;
		m_bClockwiseRotate=true;
		m_dDeltaAngle = 0.0;
	}
	m_dDeltaAngle += dValue;
	Hlong nArea;
	double dRow = 0.0, dColumn = 0.0;
	area_center(m_oldRgn, &nArea, &dRow, &dColumn);

	HTuple HomMat2DS,HomMat2DTmp,HomMat2DAdapted;
	vector_angle_to_rigid(0, 0, 0, 0, 0,  (m_dDeltaAngle/180.0)*3.1415926, &HomMat2DS);
	hom_mat2d_translate(HomMat2DS, 0.5, 0.5, &HomMat2DTmp);
	hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);

	Hobject rotatedRgn,RegionMoved1;	
	move_region (m_oldRgn, &RegionMoved1,  -dRow, -dColumn);
	projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DS, "bilinear");
	move_region (rotatedRgn,&m_finalRgn, dRow, dColumn) ;
	//
	m_dictRotatedTestRgn[m_wstrCrtRgnName] = m_finalRgn;
	DisplayAllRegions();
	SetUnionAndDiffButtonAccessbility();
	m_btnSaveRgn.EnableWindow(TRUE);
}

void CModelToolDlg::OnDeltaposSpinMovePixel(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	CString sValue;
	m_editMovePixel.GetWindowTextW(sValue);
	wstring wstrValue = CT2W(sValue);
	wchar_t* pEnd = nullptr;
	long nValue = wcstol(wstrValue.c_str(), &pEnd, 10);

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta == -1) // decrease
	{
		if(nValue > 1)
		{
			sValue.Format(_T("%d"), nValue - 1);
		}
	}
	else // Increase
	{
		sValue.Format(_T("%d"), nValue + 1);
	}
	m_editMovePixel.SetWindowTextW(sValue);
	UpdateData(FALSE);
	*pResult = 0;
}

void CModelToolDlg::OnDeltaposSpinScalePixel(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	CString sValue;
	m_editScalePixel.GetWindowTextW(sValue);
	wstring wstrValue = CT2W(sValue);
	wchar_t* pEnd = nullptr;
	double dValue = wcstod(wstrValue.c_str(), &pEnd);

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta == -1) // decrease
	{
		if(dValue >= 1.0)
		{
			sValue.Format(_T("%.1f"), dValue - 0.5);
		}
	}
	else // Increase
	{
		sValue.Format(_T("%.1f"), dValue + 0.5);
	}
	m_editScalePixel.SetWindowTextW(sValue);
	UpdateData(FALSE);
	*pResult = 0;
}

void CModelToolDlg::OnDeltaposSpinDegree(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	CString sValue;
	m_editAngle.GetWindowTextW(sValue);
	wstring wstrValue = CT2W(sValue);
	wchar_t* pEnd = nullptr;
	double dValue = wcstod(wstrValue.c_str(), &pEnd);

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta == -1) // decrease
	{
		if(dValue > 0.5)
		{
			sValue.Format(_T("%.1f"), dValue - 0.5);
		}
	}
	else // Increase
	{
		sValue.Format(_T("%.1f"), dValue + 0.5);
	}
	m_dDeltaAngle = 0.0;
	m_editAngle.SetWindowTextW(sValue);
	UpdateData(FALSE);
	*pResult = 0;
}

void CModelToolDlg::OnBnClickedRadioPlane()
{
	m_selectRegionExtract=1;
	UpdateData(TRUE);
	OnRangeSliderChanged(IDC_SLIDER_GRAY_VAL_RANGE,0);
}

void CModelToolDlg::OnBnClickedRadioRing()
{
	m_selectRegionExtract=0;
	UpdateData(TRUE);
	OnRangeSliderChanged(IDC_SLIDER_GRAY_VAL_RANGE,0);
}

void CModelToolDlg::OnBnClickedRadioAll()
{
	m_selectRegionExtract=2;
	UpdateData(TRUE);
	OnRangeSliderChanged(IDC_SLIDER_GRAY_VAL_RANGE,0);
}

void CModelToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_mouseSelectRegion)
	{	
		HTuple Class2;
		get_obj_class(m_crtImg, &Class2);
		if(Class2==HTuple("image"))
		{
			HTuple height, width;
			get_image_size(m_crtImg, &width, &height);
			double scaleRow,scaleCol;
			scaleRow=height[0].D()/494.0;
			scaleCol=width[0].D()/659.0;
			//为保证放缩比例，同样调整为X*494或者659*Y
			if (scaleCol>scaleRow)
			{
				scaleRow=scaleCol;
			} 
			else
			{
				scaleCol=scaleRow;
			}
			Hobject DestRegions;
			HTuple  rectTopRow, rectTopCol, rectWidth, rectHeight;
			get_window_extents(m_imgWndID,&rectTopRow, &rectTopCol,&rectWidth, &rectHeight);
			if(( point.y-rectTopRow>0)&& (point.x-rectTopCol>0))
			{
				select_region_point(m_connectedRegions, &DestRegions, (point.y-rectTopRow)*scaleRow, (point.x-rectTopCol)*scaleCol);
				union2 (m_selectRegion, DestRegions, &m_selectRegion);
				HTuple Class;
				get_obj_class(m_selectRegion, &Class);
				if(Class==HTuple("region"))
				{
					set_color(m_imgWndID,"green");
					disp_obj(DestRegions, m_imgWndID);
					//m_finalRgn = m_selectRegion;
				}	
			}
		}

	}


	CDialogEx::OnLButtonDown(nFlags, point);
}

void CModelToolDlg::OnCbnSelchangeComboWorkingFolder()
{
	CString sModelName;
	int index = m_comboWorkingFolder.GetCurSel();
	m_comboWorkingFolder.GetLBText(index, sModelName);

	string strPath      = CW2A(m_wstrTargetFile.c_str());
	string strModelName = CT2A(sModelName);
	strPath.append("\\");
	strPath.append(strModelName);

	bool read_Ok=ReadWorkingFolder(strPath, strModelName);
	if (!read_Ok)
	{
		return ;
	}
	ReadRegionFiles(strPath, strModelName);
	ProcessRegionsByNewImg();
}

void CModelToolDlg::OnBnClickedCheckDrawSector()
{
	UpdateData(TRUE);
	if(m_bDrawSector)
	{
		if(m_bChs)
		{
			((CWnd *)GetDlgItem(IDC_STATIC_GRAY_VALUE_FILTER))->SetWindowTextW(_T("扇形半径"));   
			((CWnd *)GetDlgItem(IDC_STATIC_CONTRAST))->SetWindowTextW(_T("扇形角度")); 
		}
		else
		{
			((CWnd *)GetDlgItem(IDC_STATIC_GRAY_VALUE_FILTER))->SetWindowTextW(_T("Sector Radius"));   
			((CWnd *)GetDlgItem(IDC_STATIC_CONTRAST))->SetWindowTextW(_T("Sector Angle")); 
		}

		UpdateSliderLclAndUcl(IDC_SLIDER_GRAY_VAL_RANGE, 0, 1000);
		UpdateSliderThumb(IDC_SLIDER_GRAY_VAL_RANGE, 100,180);
		m_grayValLCL.SetWindowTextW(_T("0"));
		m_grayValUCL.SetWindowTextW(_T("1000"));
		UpdateSliderLclAndUcl(IDC_SLIDER_CONTRAST_VALUE, -360, 360);
		UpdateSliderThumb(IDC_SLIDER_CONTRAST_VALUE, 0, 360);
		m_ctrstLCL.SetWindowTextW(_T("-360"));
		m_ctrstUCL.SetWindowTextW(_T("360"));
		//
		((CWnd *)GetDlgItem(IDC_STATIC_RGN_SIZE_FILTER))->ShowWindow( SW_HIDE );    
		((CWnd *)GetDlgItem(IDC_STATIC_GRN_SIZE_LCL))->ShowWindow( SW_HIDE );   
		((CWnd *)GetDlgItem(IDC_STATIC_RGN_SIZE_UCL))->ShowWindow( SW_HIDE );   
		((CWnd *)GetDlgItem(IDC_SLIDER_REGION_SIZE_FILTER))->ShowWindow( SW_HIDE );   
		((CWnd *)GetDlgItem(IDC_STATIC_RGN_SIZE_MIN))->ShowWindow( SW_HIDE );    
		((CWnd *)GetDlgItem(IDC_STATIC_DILATION))->ShowWindow( SW_HIDE );    
		((CWnd *)GetDlgItem(IDC_STATIC_DILATION_LCL))->ShowWindow(SW_HIDE);  
		((CWnd *)GetDlgItem(IDC_STATIC_DILATION_UCL))->ShowWindow(SW_HIDE);    
		((CWnd *)GetDlgItem(IDC_SLIDER_DILATION_SIZE))->ShowWindow(SW_HIDE);   
		((CWnd *)GetDlgItem(IDC_STATIC_DILATION_VALUE))->ShowWindow(SW_HIDE);   
		((CWnd *)GetDlgItem(IDC_STATIC_SELECT_REGION))->ShowWindow(SW_HIDE);   
		((CWnd *)GetDlgItem(IDC_RADIO_PLANE))->ShowWindow( SW_HIDE );     
		((CWnd *)GetDlgItem(IDC_RADIO_RING))->ShowWindow( SW_HIDE );     
		((CWnd *)GetDlgItem(IDC_RADIO_ALL))->ShowWindow( SW_HIDE );    
		((CWnd *)GetDlgItem(IDC_STATIC_PLANE))->ShowWindow( SW_HIDE );     
		((CWnd *)GetDlgItem(IDC_STATIC_RING))->ShowWindow( SW_HIDE );     
		((CWnd *)GetDlgItem(IDC_STATIC_ALL))->ShowWindow( SW_HIDE );   
		//
		((CWnd *)GetDlgItem(IDC_BUTTON_CIRCLE_SEL))->EnableWindow(false);
		((CWnd *)GetDlgItem(IDC_BUTTON_ELLIPSE_SEL))->EnableWindow(false);
		((CWnd *)GetDlgItem(IDC_BUTTON_RECTANGLE_SEL))->EnableWindow(false);
		((CWnd *)GetDlgItem(IDC_BUTTON_POLYGON_SEL))->EnableWindow(false);
	}
	else
	{
		if(m_bChs)
		{
			((CWnd *)GetDlgItem(IDC_STATIC_GRAY_VALUE_FILTER))->SetWindowTextW(_T("灰度值过滤"));    
			((CWnd *)GetDlgItem(IDC_STATIC_CONTRAST))->SetWindowTextW(_T("对比度设置")); 
		}
		else
		{
			((CWnd *)GetDlgItem(IDC_STATIC_GRAY_VALUE_FILTER))->SetWindowTextW(_T("Gray Filter"));    
			((CWnd *)GetDlgItem(IDC_STATIC_CONTRAST))->SetWindowTextW(_T("Contrast Set"));  
		}
 
		UpdateSliderLclAndUcl(IDC_SLIDER_GRAY_VAL_RANGE, 0, 255);
		UpdateSliderThumb(IDC_SLIDER_GRAY_VAL_RANGE, 0, 255);
		m_grayValLCL.SetWindowTextW(_T("0"));
		m_grayValUCL.SetWindowTextW(_T("250"));
		UpdateSliderLclAndUcl(IDC_SLIDER_CONTRAST_VALUE, 2, 120);
		UpdateSliderThumb(IDC_SLIDER_CONTRAST_VALUE, 20, 40);
		m_ctrstLCL.SetWindowTextW(_T("2"));
		m_ctrstUCL.SetWindowTextW(_T("120"));
		//
		((CWnd *)GetDlgItem(IDC_STATIC_RGN_SIZE_FILTER))->ShowWindow( SW_SHOW ); 
		((CWnd *)GetDlgItem(IDC_STATIC_GRN_SIZE_LCL))->ShowWindow( SW_SHOW );  
		((CWnd *)GetDlgItem(IDC_STATIC_RGN_SIZE_UCL))->ShowWindow( SW_SHOW );   
		((CWnd *)GetDlgItem(IDC_SLIDER_REGION_SIZE_FILTER))->ShowWindow( SW_SHOW );    
		((CWnd *)GetDlgItem(IDC_STATIC_RGN_SIZE_MIN))->ShowWindow( SW_SHOW );     
		((CWnd *)GetDlgItem(IDC_STATIC_DILATION))->ShowWindow( SW_SHOW );     
		((CWnd *)GetDlgItem(IDC_STATIC_DILATION_LCL))->ShowWindow( SW_SHOW );    
		((CWnd *)GetDlgItem(IDC_STATIC_DILATION_UCL))->ShowWindow( SW_SHOW );  
		((CWnd *)GetDlgItem(IDC_SLIDER_DILATION_SIZE))->ShowWindow( SW_SHOW ); 
		((CWnd *)GetDlgItem(IDC_STATIC_DILATION_VALUE))->ShowWindow( SW_SHOW );  

		if(!m_createModel)
		{
			((CWnd *)GetDlgItem(IDC_STATIC_SELECT_REGION))->ShowWindow(SW_SHOW);    
			((CWnd *)GetDlgItem(IDC_RADIO_PLANE))->ShowWindow(SW_SHOW);   
			((CWnd *)GetDlgItem(IDC_RADIO_RING))->ShowWindow(SW_SHOW);  
			((CWnd *)GetDlgItem(IDC_RADIO_ALL))->ShowWindow(SW_SHOW);  
			((CWnd *)GetDlgItem(IDC_STATIC_PLANE))->ShowWindow(SW_SHOW);     
			((CWnd *)GetDlgItem(IDC_STATIC_RING))->ShowWindow(SW_SHOW);     
			((CWnd *)GetDlgItem(IDC_STATIC_ALL))->ShowWindow(SW_SHOW);   
		}
		//
		((CWnd *)GetDlgItem(IDC_BUTTON_CIRCLE_SEL))->EnableWindow(true);
		((CWnd *)GetDlgItem(IDC_BUTTON_ELLIPSE_SEL))->EnableWindow(true);
		((CWnd *)GetDlgItem(IDC_BUTTON_RECTANGLE_SEL))->EnableWindow(true);
		((CWnd *)GetDlgItem(IDC_BUTTON_POLYGON_SEL))->EnableWindow(true);
	}

	UpdateData(FALSE);
	Invalidate();
}

bool CModelToolDlg::ParseModelPath(wstring wstrTargetFile, string& strModelPath, string& strModelName)
{
	if(wstrTargetFile == L"")
	{
		return false;
	}
	string strTmp = CW2A(wstrTargetFile.c_str());
	size_t index = strTmp.find_last_of("\\");
	if(index != string::npos)
	{
		strModelPath = strTmp.substr(0, index);
	}

	index = strModelPath.find_last_of("\\");
	if(index != string::npos)
	{
		strModelName = strModelPath.substr(index + 1);
	}
	return true;
}

bool ReadShapeModelDatas(string strModelPath,HTuple *ModelCenterRow,HTuple *ModelCenterCol,HTuple *ModelCenterAngle)
{
	(*ModelCenterRow).Reset();
	(*ModelCenterCol).Reset();
	(*ModelCenterAngle).Reset();

	*ModelCenterRow=0.0;
	*ModelCenterCol=0.0;
	*ModelCenterAngle=0.0;

	USES_CONVERSION;

	string datname = strModelPath;
	if (!_access(datname.c_str(),0))
	{
		TCHAR vv[64];
		GetPrivateProfileString(_T("RegionData"),_T("Row"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
		*ModelCenterRow= _tstof(vv);
		GetPrivateProfileString(_T("RegionData"),_T("Col"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
		*ModelCenterCol= _tstof(vv);
		GetPrivateProfileString(_T("RegionData"),_T("Phi"),_T("0.0"),vv,sizeof(vv),A2W(datname.c_str()));
		*ModelCenterAngle= _tstof(vv);
	}
	else 
	{
		return false;
	}
	return true;
}

bool CModelToolDlg::ReadWorkingFolder(string& strPath, string& strModelName)
{
	m_nOperationType = 0;
	if (m_wstrMatchingType==L"8")
	{
		m_rowModelCenter=m_rowModelCenterSet;
		m_colModelCenter=m_colModelCenterSet;
		m_angleModelCenter=m_angleModelCenterSet;
		m_modelScore=0.95;
		string strModelImg = "C:\\tmp.tiff";
		if (!_access(strModelImg.c_str(),0))
		{
			Hobject TmpImage;
			read_image(&m_crtImg,strModelImg.c_str());
			m_modelImg = m_crtImg;
		}
		return true;
	}
	m_modelFilePath = CA2T(strPath.c_str());
	wstring wstrModelName = CA2W(strModelName.c_str());
	//Read Image File
	string strImgFile   =  strPath;
	strImgFile.append("\\");
	strImgFile.append(strModelName);
	strImgFile.append(".tiff");
	if(PathFileExistsA(strImgFile.c_str()) == FALSE)
	{

		if(m_bChs)
		{
			CString sMsg(_T("当前工作目录无效 - 缺少:"));
			sMsg.Append(wstrModelName.c_str());
			sMsg.Append(_T(".tiff文件"));
			DisplayPromptMessage(sMsg, false);
		}
		else
		{
			CString sMsg(_T("The path is enable - the file"));
			sMsg.Append(wstrModelName.c_str());
			sMsg.Append(_T(".tiff does not exist!"));

			DisplayPromptMessage(sMsg, false);
		}


		return false;
	}
	try
	{
		read_image(&m_crtImg, strImgFile.c_str());
		m_modelImg = m_crtImg;
		// Read Model Region File
		string strRgnFile = strPath;
		strRgnFile.append("\\");
		strRgnFile.append(strModelName);
		strRgnFile.append(".reg");
		if(PathFileExistsA(strRgnFile.c_str()) == FALSE)
		{

			if(m_bChs)
			{
				CString sMsg(_T("当前工作目录无效 - 缺少:"));
				sMsg.Append(wstrModelName.c_str());
				sMsg.Append(_T(".reg文件"));

				DisplayPromptMessage(sMsg, false);
			}
			else
			{
				CString sMsg(_T("The path is enable - the file"));
				sMsg.Append(wstrModelName.c_str());
				sMsg.Append(_T(".reg does not exist!"));

				DisplayPromptMessage(sMsg, false);
			}

			return false;
		}
		read_region(&m_modelRgn, strRgnFile.c_str());

		//Read Model File
		string strShmFile = strPath;
		strShmFile.append("\\");
		strShmFile.append(strModelName);
		strShmFile.append(".shm");
		if(PathFileExistsA(strShmFile.c_str()) == FALSE)
		{
			if(m_bChs)
			{
				CString sMsg(_T("当前工作目录无效 - 缺少:"));
				sMsg.Append(wstrModelName.c_str());
				sMsg.Append(_T(".shm文件"));
				DisplayPromptMessage(sMsg, false);
			}
			else
			{
				CString sMsg(_T("The path is enable - the file"));
				sMsg.Append(wstrModelName.c_str());
				sMsg.Append(_T(".shm does not exist!"));
				DisplayPromptMessage(sMsg, false);
			}


			return false;
		}
		read_shape_model(strShmFile.c_str(), &m_modelID);
	}
	catch (...)
	{
		if(m_bChs)
		{
			CString sMsg(_T("模板相关文件:"));
			sMsg.Append(wstrModelName.c_str());
			sMsg.Append(_T("异常。"));
			DisplayPromptMessage(sMsg, false);
		}
		else
		{
			CString sMsg(_T("The model file:"));
			sMsg.Append(wstrModelName.c_str());
			sMsg.Append(_T(" read failed!"));
			DisplayPromptMessage(sMsg, false);
		}
		return false;
	}
	//Read Model dat File
	string strDatFile = strPath;
	strDatFile.append("\\");
	strDatFile.append(strModelName);
	strDatFile.append(".dat");
	if(! ReadShapeModelDatas(strDatFile, &m_rowModelCenter, &m_colModelCenter, &m_angleModelCenter))
	{
		if(m_bChs)
		{
			CString sMsg(_T("当前工作目录无效 - 缺少:"));
			sMsg.Append(wstrModelName.c_str());
			sMsg.Append(_T(".dat文件"));
			DisplayPromptMessage(sMsg, false);
		}
		else
		{
			CString sMsg(_T("The path is enable - the file"));
			sMsg.Append(wstrModelName.c_str());
			sMsg.Append(_T(".dat does not exist!"));
			DisplayPromptMessage(sMsg, false);
		}
		return false;
	}
	m_modelScore=0.95;
	if ((m_rowModelCenter<1.0)||(m_colModelCenter<1.0))
	{
		if(m_bChs)
		{
			CString sMsg(_T("当前模板数据文件 - 内容缺失:"));
			sMsg.Append(wstrModelName.c_str());
			sMsg.Append(_T(".dat文件"));
			DisplayPromptMessage(sMsg, false);
		}
		else
		{
			CString sMsg(_T("The dat file is error - the file"));
			sMsg.Append(wstrModelName.c_str());
			sMsg.Append(_T(".dat data is error!"));
			DisplayPromptMessage(sMsg, false);
		}
		m_modelScore=0.0;
		HTuple ReMatNumLevels, ReMatAngleStart, ReMatAngleExtent, ReMatAngleStep, ReMatScaleMin, ReMatScaleMax, ReMatScaleStep, ReMatMetric, ReMatMinContrast;
		get_shape_model_params (m_modelID,&ReMatNumLevels, &ReMatAngleStart, &ReMatAngleExtent, &ReMatAngleStep, &ReMatScaleMin, &ReMatScaleMax, &ReMatScaleStep, &ReMatMetric, &ReMatMinContrast);
		//
		if (ReMatAngleExtent>3.2)
		{
			find_shape_model(m_crtImg, m_modelID, HTuple(0).Rad(), HTuple(360).Rad(), 0.5, 1, 0.5, "least_squares", 0, 0.8, &m_rowModelCenter, &m_colModelCenter, &m_angleModelCenter, &m_modelScore);
		} 
		else
		{
			find_shape_model(m_crtImg, m_modelID, HTuple(-10).Rad(), HTuple(20).Rad(), 0.5, 1, 0.5, "least_squares", 0, 0.8, &m_rowModelCenter, &m_colModelCenter, &m_angleModelCenter, &m_modelScore);
		}
		if (m_modelScore<0.05)
		{
			return false;
		}
	} 
	return true;
}

void CModelToolDlg::ReadRegionFiles(string& strPath, string& strModelName)
{
	m_bTargetFileChanged = false;
	CString sFilter  = CA2T(strPath.c_str());
	sFilter.Append(_T("\\*.*"));
	CString sModelName = CA2T(strModelName.c_str());
	sModelName.Append(_T(".reg"));

	CString sFileName;
	CFileFind ff;
	BOOL bWorking = ff.FindFile(sFilter);

	m_dictTestRgn.clear();
	m_dictRotatedTestRgn.clear();
	gen_empty_obj(&m_newModelImageRgn);	

	HTuple HomMat2DS;
	vector_angle_to_rigid(0.0, 0.0, 0.0, m_rowModelCenter, m_colModelCenter, m_angleModelCenter, &HomMat2DS);

	while(bWorking)
	{
		bWorking = ff.FindNextFile();
		if(ff.IsDirectory())
		{
			if(!ff.IsDots())
			{
				CString sPath = ff.GetFilePath();
				int stop = 90;
			}
			continue;
		}
		sFileName = ff.GetFileName();
		int index = sFileName.ReverseFind(_T('.'));
		CString sFileExt = sFileName.Right(sFileName.GetLength() - index - 1);
		if(sFileExt != _T("reg"))
			continue;
		if(sFileName == sModelName)
			continue;

		CString sPath = CA2T(strPath.c_str());
		CString sRgnFile = sPath + _T("\\") + ff.GetFileName();
		string strRgnfile = CT2A(sRgnFile);
		// Default settings used in HDevelop (can be omitted) 
		//set_system("do_low_error","disabled");
		try
		{
			Hobject rgn,RegionMoved;
			read_region(&rgn, strRgnfile.c_str());
			wstring wstrRgnFile = CT2W(sRgnFile);
			//
			if (m_wstrMatchingType==L"8")
			{
				HTuple  Width211, Height211;
				get_image_size (m_crtImg, &Width211, &Height211);
				move_region (rgn,&RegionMoved,m_rowModelCenterSet,m_colModelCenterSet);
				if (abs(m_angleModelCenterSet[0].D())>=0.001)
				{
					HTuple HomMat2DRota;
					vector_angle_to_rigid(m_rowModelCenterSet, m_colModelCenterSet, 0.0, m_rowModelCenterSet, m_colModelCenterSet, m_angleModelCenterSet, &HomMat2DRota);
					hom_mat2d_translate(HomMat2DRota, 0.5, 0.5, &HomMat2DRota);
					hom_mat2d_translate_local(HomMat2DRota, -0.5, -0.5, &HomMat2DRota);
					projective_trans_region (RegionMoved,&RegionMoved,HomMat2DRota, "bilinear");
				}
			}
			else
			{
				projective_trans_region(rgn, &RegionMoved, HomMat2DS, "bilinear");
			}

			m_dictTestRgn[wstrRgnFile]        = RegionMoved;
			m_dictRotatedTestRgn[wstrRgnFile] = RegionMoved;
		}
		catch (...)
		{
			//
			Hobject RegionMoved;
			HTuple  Width211, Height211;
			get_image_size (m_crtImg, &Width211, &Height211);
			gen_circle(&RegionMoved,Height211/2.0,Width211/2.0,Height211/4.0);
			wstring wstrRgnFile = CT2W(sRgnFile);
			m_dictTestRgn[wstrRgnFile]        = RegionMoved;
			m_dictRotatedTestRgn[wstrRgnFile] = RegionMoved;
			//
			wstring wstrFailName = CA2W(strRgnfile.c_str());
			if(m_bChs)
			{
				CString sMsg(_T("区域文件:"));
				sMsg.Append(wstrFailName.c_str());
				sMsg.Append(_T("异常。"));
				DisplayPromptMessage(sMsg, false);
			}
			else
			{
				CString sMsg(_T("The file:"));
				sMsg.Append(wstrFailName.c_str());
				sMsg.Append(_T(" read failed!"));
				DisplayPromptMessage(sMsg, false);
			}
		}
	}
}

void CModelToolDlg::ProcessRegionsByNewImg()
{
	m_bUseScaleModel = true;
	m_bNewImageMatchingOK=true;
	if(!m_createModel)
	{
		m_mouseSelectRegion = false;
		gen_empty_obj(&m_selectRegion);
		gen_empty_obj(&m_connectedRegions);
		gen_empty_obj(&m_newModelImageRgn);
	}
	if (m_wstrMatchingType==L"8")
	{
		for(auto it = m_dictTestRgn.begin(); it != m_dictTestRgn.end(); it++)
		{
			Hobject rgn = it->second;
			m_dictRotatedTestRgn[it->first] = rgn;
			concat_obj(rgn, m_newModelImageRgn, &m_newModelImageRgn);
		}
		return ;
	}
	if(m_modelID == -1)
	{

		if(m_bChs)
			DisplayPromptMessage(_T("请首先创建或加载模板！"), false);
		else
			DisplayPromptMessage(_T("Please to create or load model firstly ! "), false);
		return;
	}
	if(m_crtImg.Id() == H_EMPTY_REGION)
	{
		return;
	}
	m_dictRotatedTestRgn.clear();
	HTuple modelImageWidth,modelImageHeight;
	Hobject modelFindRegion,templateImage;
	get_image_size(m_crtImg, &modelImageWidth, &modelImageHeight);
	gen_circle(&modelFindRegion, modelImageHeight/2.0,modelImageWidth/2.0 , modelImageHeight/2.4); 
	if(m_modelScore >= 0.62) // 
	{
		//
		HTuple row42, column42, angle42,score2;
		reduce_domain(m_newImg, modelFindRegion, &templateImage);
		//	
		HTuple ReMatNumLevels, ReMatAngleStart, ReMatAngleExtent, ReMatAngleStep, ReMatScaleMin, ReMatScaleMax, ReMatScaleStep, ReMatMetric, ReMatMinContrast;
		get_shape_model_params (m_modelID,&ReMatNumLevels, &ReMatAngleStart, &ReMatAngleExtent, &ReMatAngleStep, &ReMatScaleMin, &ReMatScaleMax, &ReMatScaleStep, &ReMatMetric, &ReMatMinContrast);
		//
		if (ReMatAngleExtent>3.2)
		{
			find_shape_model(templateImage, m_modelID, HTuple(0).Rad(), HTuple(360).Rad(), 0.5, 1, 0.5, "least_squares", 0, 0.8, &row42, &column42, &angle42, &score2);
		} 
		else
		{
			find_shape_model(templateImage, m_modelID, HTuple(-10).Rad(), HTuple(20).Rad(), 0.5, 1, 0.5, "least_squares", 0, 0.8, &row42, &column42, &angle42, &score2);
		}

		if(score2>0.45)
		{
			m_bUseScaleModel = false;
			m_rowNewImagemMatchingCenter=row42;
			m_colNewImagemMatchingCenter=column42;
			m_newImagemMatchingScale=1.0;	
			m_dNewImageAngle=angle42[0].D();
			//
			Hobject regionOpening,regionClosing;	
			HTuple HomMat2DRate,HomMat2DTmp,HomMat2DAdapted;
			vector_angle_to_rigid(0, 0, m_angleModelCenter, 0, 0, angle42, &HomMat2DRate);
			hom_mat2d_translate(HomMat2DRate, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
			//
			for(auto it = m_dictTestRgn.begin(); it != m_dictTestRgn.end(); it++)
			{
				Hobject rgn = it->second;
				Hobject rotatedRgn,RegionMoved1,RegionMoved2;	
				//
				move_region (rgn, &RegionMoved1,-m_rowModelCenter, -m_colModelCenter);        
				projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
				move_region (rotatedRgn,&RegionMoved2, row42, column42) ;
				opening_circle(RegionMoved2, &regionOpening, 1.0);
				closing_circle(regionOpening, &regionClosing, 1.0);
				m_dictRotatedTestRgn[it->first] = regionClosing;
				concat_obj(regionClosing, m_newModelImageRgn, &m_newModelImageRgn);
			}
		}
		else
		{
			//
			Hobject  modelContours,contoursForNewModel;
			HTuple HomMat2D2,modelID2,rowSC,columnSC,angleSC,scaleSC,score2;
			reduce_domain(m_crtImg, modelFindRegion, &templateImage);
			get_shape_model_contours(&modelContours, m_modelID, 1);
			vector_angle_to_rigid(0, 0, 0, m_rowModelCenter, m_colModelCenter, m_angleModelCenter, &HomMat2D2);
			affine_trans_contour_xld(modelContours, &contoursForNewModel, HomMat2D2);
			create_scaled_shape_model_xld(contoursForNewModel,3, 0, 6.28, "auto", 0.95, 1.05, "auto",   "auto", "ignore_local_polarity", 20, &modelID2);
			find_scaled_shape_model(templateImage, modelID2, 0, 6.28, 0.98, 1.02, 0.5, 1, 0.5, "least_squares",  0, 0.6, &rowSC, &columnSC, &angleSC, &scaleSC, &score2);
			if (0 != (score2>0.6))
			{
				m_rowScaleModelCenter=rowSC;
				m_colScaleModelCenter=columnSC;
				m_angleScaleModel=angleSC;
				HTuple row4, column4, angle4, scale, score1;
				reduce_domain(m_newImg, modelFindRegion, &templateImage);
				find_scaled_shape_model(templateImage, modelID2, 0, 6.28, 0.95, 1.05, 0.45, 1, 0.5, "least_squares", 0, 0.6, &row4, &column4, &angle4, &scale, &score1);

				if (0 != (score1>0.6))
				{
					m_rowNewImagemMatchingCenter = row4;
					m_colNewImagemMatchingCenter = column4;
					m_newImagemMatchingScale     = scale;	
					m_dNewImageAngle=angle4[0].D();	
					Hobject regionOpening,regionClosing;
					HTuple HomMat2DS,HomMat2DScale,HomMat2DTmp,HomMat2DAdapted;
					double scaleData=0.0;
					scaleData=m_newImagemMatchingScale[0].D();
					vector_angle_to_rigid(0, 0, m_angleScaleModel, 0, 0, angle4, &HomMat2DS);
					hom_mat2d_scale (HomMat2DS, scaleData, scaleData,0, 0, &HomMat2DScale);
					hom_mat2d_translate(HomMat2DScale, 0.5, 0.5, &HomMat2DTmp);
					hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2DAdapted);
					for(auto it = m_dictTestRgn.begin(); it != m_dictTestRgn.end(); it++)
					{
						Hobject rgn = it->second;
						Hobject rotatedRgn,RegionMoved1,RegionMoved2;	
						move_region (rgn, &RegionMoved1,-rowSC, -columnSC);        
						projective_trans_region (RegionMoved1, &rotatedRgn, HomMat2DAdapted, "bilinear");
						move_region (rotatedRgn,&RegionMoved2, row4, column4) ;
						opening_circle(RegionMoved2, &regionOpening, 1.0);
						closing_circle(regionOpening, &regionClosing, 1.0);
						m_dictRotatedTestRgn[it->first] = regionClosing;
						concat_obj(regionClosing, m_newModelImageRgn, &m_newModelImageRgn);
					}

				}
				else
				{
					m_dictRotatedTestRgn = m_dictTestRgn;
					m_bNewImageMatchingOK=false;
					if(m_bChs)
						DisplayPromptMessage(_T("新建可变模板匹配模板图片失败！"), false);
					else
						DisplayPromptMessage(_T("Creating variable model failed!"), false);
				}
			}
			else
			{
				m_bNewImageMatchingOK=false;
				if(m_bChs)
					DisplayPromptMessage(_T("新建可变模板匹配模板图片失败！"), false);
				else
					DisplayPromptMessage(_T("Creating variable model failed!"), false);

			}
			if(m_bChs)
				DisplayPromptMessage(_T("新建可变模板匹配模板图片！"), true);
			else
				DisplayPromptMessage(_T("Creating variable model!"), true);
		}

	}
	else
	{
		m_bNewImageMatchingOK=false;
		if(m_bChs)
			DisplayPromptMessage(_T("当前模板匹配模板图片失败！"), false);
		else
			DisplayPromptMessage(_T("The current model matches failed!"), false);
	}
	if((!m_bNewImageMatchingOK)&(!m_createModel))
	{
		if(m_bChs)
			MessageBox(_T("模板匹配失败，请先修改模板再进行检测区域相关操作。"));
		else
			MessageBox(_T("Model matches failed, please modifiy the model first!"));
		return;
	}
}

void CModelToolDlg::SetWidgetSring(int nCtrlID, CString str)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	pWnd->SetWindowText(str);
	pWnd->Invalidate();
}

void CModelToolDlg::CreateDialogTitleBar(CRect& rcScreen)
{
	// Dialog Title
	m_dlgTitle.SubclassDlgItem(IDC_STATIC_DLG_TITLE,this);
	CString str(_T("模 板 绘 制 工 具"));
	if(!m_bChs)
		str = _T("Model Tool");

	m_dlgTitle.SetWindowTextW(str);
	m_dlgTitle.SetColor(RGB(10,10,115),RGB(171,178,185));
	double rate = rcScreen.Height() /1080.0;
	int fontSz = (int)(60 * rate);
	m_dlgTitle.SetTxtFont(fontSz,FW_BOLD);

	CRect titleRect;
	m_dlgTitle.GetClientRect(&titleRect);
	int szClientX = rcScreen.Width();
	int szTitleX = titleRect.Width();
	int szTitleY = titleRect.Height();
	m_dlgTitle.MoveWindow((int)rcScreen.Width()/3 + 20,rcScreen.top + 15,szTitleX,szTitleY);

	CRect rcTitle;
	m_dlgTitle.GetWindowRect(rcTitle);

	// Close Button
	int titleAreaH = int(TILTE_HEIGHT * rcScreen.Height() / 900.0f);
	CRect rcClose;
	rcClose.left = rcScreen.right - 10 - 48;
	rcClose.top = int((titleAreaH - 48) / 2.0) - 5;
	rcClose.right = rcClose.left + 48;
	rcClose.bottom = rcClose.top + 48;

	m_btnClose.MoveWindow(rcClose);
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_CLOSE);
	m_btnClose.SetBitmap(bmp);
 }

void CModelToolDlg::CreateImageWindow(CRect& rcScreen)
{
	int titleAreaH = int(TILTE_HEIGHT * rcScreen.Height() / 900.0f);
	int gapH = 5;
	int gapV = 10;

	CRect rcClient;
	GetClientRect(&rcClient);

	long col = rcClient.left + gapH + 10;
	long row = rcClient.top + titleAreaH + 10;
	//	
	int width = 659;
	int height = 494;
	string strModelImg = "C:\\tmp.tiff";
	if (!_access(strModelImg.c_str(),0))
	{
		Hobject TmpImage;
		read_image(&TmpImage,strModelImg.c_str());
		HTuple  Width,Height;
		get_image_size(TmpImage, &Width, &Height);

		double scaleRow,scaleCol;
		scaleRow=Height[0].D()/494.0;
		scaleCol=Width[0].D()/659.0;
		//为保证放缩比例，同样调整为X*494或者659*Y
		if (scaleCol>scaleRow)
		{
			width =659;
			height =(int) (Height[0].D()/scaleCol);
		} 
		else
		{
			width = (int) (Width[0].D()/scaleRow);
			height = 494;
		}
	}
	try
	{
		Hobject  initIimage;
		std::vector<unsigned char>  image(height * width, 0);
		for(int r = 0; r < height; r++)
		{
			for(int c = 0; c < width; c++)
				image[r * width + c] = 10 + r % 48 + c % 32;
		}
		gen_image1(&initIimage,"byte",width,height,(Hlong)&image[0]);

		if(m_imgWndID == -1)
		{
			set_check("~father");
			std::string mode("visible");
			std::string strMachine("");
			open_window(row, col, width, height,(Hlong)m_hWnd, mode.c_str(),strMachine.c_str(), (Hlong*)&m_imgWndID);
			set_part(m_imgWndID,0, 0, height - 1, width - 1);
			set_check("father");
		}
		set_draw(m_imgWndID,"margin"); //margin or fill
		set_color(m_imgWndID, "red");
		disp_obj(initIimage, m_imgWndID);
		set_line_width(m_imgWndID,HTuple(1));
	}
	catch (...) 
	{
		//TODO:
		return;
	}
	m_rcImgWnd.left   = col;
	m_rcImgWnd.top    = row;
	m_rcImgWnd.right  = m_rcImgWnd.left + 659;
	m_rcImgWnd.bottom = m_rcImgWnd.top + 494;
}

void CModelToolDlg::CreateFileOperationWidget()
{
	CRect rcBtn;
	rcBtn.right  = m_rcImgWnd.right;
	rcBtn.left   = rcBtn.right - 118;
	rcBtn.top    = m_rcImgWnd.bottom + 15;
	rcBtn.bottom = rcBtn.top + 30;

	m_btnSaveModel.SetFont(&m_textFont);
	m_btnSaveModel.MoveWindow(rcBtn);

	m_btnSaveRgn.SetFont(&m_textFont);
	m_btnSaveRgn.MoveWindow(rcBtn);

	CRect rcBtnResetRgn;
	rcBtnResetRgn.left   = 15;
	rcBtnResetRgn.right  = rcBtnResetRgn.left+118;
	rcBtnResetRgn.top    = m_rcImgWnd.bottom + 15;
	rcBtnResetRgn.bottom = rcBtnResetRgn.top + 30;

	m_btnResetRgn.SetFont(&m_textFont);
	m_btnResetRgn.MoveWindow(rcBtnResetRgn);

	m_comboModelType.SetFont(&m_textFont);
	CRect rcCombo;
	rcCombo.right  = rcBtn.left - 10;
	rcCombo.left   = rcCombo.right - 160;
	rcCombo.top    = rcBtn.top;
	rcCombo.bottom = rcBtn.bottom;
	m_comboModelType.MoveWindow(rcCombo);
	if(m_bChs)
	{
		m_comboModelType.AddString(_T("形状模板"));
		m_comboModelType.AddString(_T("字符模板"));
		m_comboModelType.AddString(_T("灰度模板"));
	}
	else
	{

		m_comboModelType.AddString(_T("Shape Model"));
		m_comboModelType.AddString(_T("OCR Model"));
		m_comboModelType.AddString(_T("Gray Model"));
	}

	m_comboModelType.SetCurSel(0);
	InvalidateRect(rcCombo);

	m_comboWorkingFolder.SetFont(&m_textFont);
	m_comboWorkingFolder.MoveWindow(&rcCombo);

	CRect rcEdit;
	rcEdit.right = rcCombo.left - 10;
	rcEdit.top   = rcCombo.top;
	rcEdit.left  = rcEdit.right - 160;
	rcEdit.bottom = rcBtn.bottom;
	m_editModelName.SetFont(&m_textFont);
	m_editModelName.MoveWindow(rcEdit);

	m_staticModelItem.SetFont(&m_textFont);
	CRect rcStatic1 = rcEdit; 
	rcStatic1.top    += 3;
	rcStatic1.bottom += 3;
	m_staticModelItem.MoveWindow(rcStatic1);

	CRect rcStatic;
	rcStatic.right = rcEdit.left - 10;
	rcStatic.top   = rcEdit.top + 3;
	rcStatic.left  = rcStatic.right - 55;
	rcStatic.bottom = rcBtn.bottom + 3;
	m_staticModelName.SetFont(&m_textFont);
	m_staticModelName.MoveWindow(rcStatic);

	CRect rcNumLevels;
	rcNumLevels.right = rcStatic.left - 10;
	rcNumLevels.top   = rcCombo.top ;
	rcNumLevels.left  = rcNumLevels.right - 30;
	rcNumLevels.bottom = rcCombo.bottom ;
	m_numLevelsEdit.SetFont(&m_textFont);
	m_numLevelsEdit.MoveWindow(rcNumLevels);

	CRect rcStaticNumLevels;
	rcStaticNumLevels.right = rcNumLevels.left - 10;
	rcStaticNumLevels.top   = rcNumLevels.top + 3;
	rcStaticNumLevels.left  = rcStaticNumLevels.right -70;
	rcStaticNumLevels.bottom = rcBtn.bottom + 3;
	m_staticNumLevels.SetFont(&m_textFont);
	m_staticNumLevels.MoveWindow(rcStaticNumLevels);

	CRect rcRateImage;
	rcRateImage.right = rcNumLevels.left +53;
	rcRateImage.top   = rcNumLevels.bottom+50;
	rcRateImage.left  = rcRateImage.right -115;
	rcRateImage.bottom =rcRateImage.top + 30;
	m_btnRateModelImage.SetFont(&m_textFont);
	m_btnRateModelImage.MoveWindow(rcRateImage);
}

void CModelToolDlg::CreateMessagePanel(CRect& rcScreen)
{
	CRect rcPanel;
	rcPanel.left   = rcScreen.left + 15;
	rcPanel.top    = rcScreen.bottom - 35;
	rcPanel.right  = rcScreen.right - 15;
	rcPanel.bottom = rcScreen.bottom - 5;

	m_msgPanel.SetFont(&m_textFont);
	m_msgPanel.MoveWindow(rcPanel);
}

void CModelToolDlg::CreateToolPanel()
{
	CRect rcPanel;
	rcPanel.left   = m_rcImgWnd.right + 5;
	rcPanel.top    = m_rcImgWnd.top - 8;
	rcPanel.right  = rcPanel.left + 56;
	rcPanel.bottom = m_rcImgWnd.bottom;
	m_staticToolPanel.MoveWindow(rcPanel);

	CRect rcBtn;
	rcBtn.left   = rcPanel.left + 12;
	rcBtn.top    = rcPanel.top + 20;
	rcBtn.right  = rcBtn.left + 32;
	rcBtn.bottom = rcBtn.top + 32;
	m_btnCircleSel.MoveWindow(rcBtn);
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP_CIRCLE_SEL);
	m_btnCircleSel.SetBitmap(bmp);

	AdjustRect(rcBtn);
	m_btnEllipseSel.MoveWindow(rcBtn);
	CBitmap bmpEllipseSel;
	bmpEllipseSel.LoadBitmapW(IDB_BITMAP_ELLIPSE_SEL);
	m_btnEllipseSel.SetBitmap(bmpEllipseSel);

	AdjustRect(rcBtn);
	m_btnRectSel.MoveWindow(rcBtn);
	CBitmap bmpRectSel;
	bmpRectSel.LoadBitmapW(IDB_BITMAP_RECTANGLE_SEL);
	m_btnRectSel.SetBitmap(bmpRectSel);

	AdjustRect(rcBtn);
	m_btnPolygonSel.MoveWindow(rcBtn);
	CBitmap bmpPolygonSel;
	bmpPolygonSel.LoadBitmapW(IDB_BITMAP_POLYGON_SEL);
	m_btnPolygonSel.SetBitmap(bmpPolygonSel);

	AdjustRect(rcBtn);
	m_btnCircleDraw.MoveWindow(rcBtn);
	CBitmap bmpCircleDraw;
	bmpCircleDraw.LoadBitmapW(IDB_BITMAP_CIRCLE_DRAW);
	m_btnCircleDraw.SetBitmap(bmpCircleDraw);

	AdjustRect(rcBtn);
	m_btnEllipseDraw.MoveWindow(rcBtn);
	CBitmap bmpEllipseDraw;
	bmpEllipseDraw.LoadBitmapW(IDB_BITMAP_ELLIPSE_DRAW);
	m_btnEllipseDraw.SetBitmap(bmpEllipseDraw);

	AdjustRect(rcBtn);
	m_btnRectDraw.MoveWindow(rcBtn);
	CBitmap bmpRectDraw;
	bmpRectDraw.LoadBitmapW(IDB_BITMAP_RECTANGLE_DRAW);
	m_btnRectDraw.SetBitmap(bmpRectDraw);

	AdjustRect(rcBtn);
	m_btnPolygonDraw.MoveWindow(rcBtn);
	CBitmap bmpPolygonDraw;
	bmpPolygonDraw.LoadBitmapW(IDB_BITMAP_POLYGON_DRAW);
	m_btnPolygonDraw.SetBitmap(bmpPolygonDraw);

	AdjustRect(rcBtn);
	m_btnUnion.MoveWindow(rcBtn);
	CBitmap bmpUnion;
	bmpUnion.LoadBitmapW(IDB_BITMAP_UNION_DISABLED);
	m_btnUnion.SetBitmap(bmpUnion);
	m_btnUnion.EnableWindow(FALSE);

	AdjustRect(rcBtn);
	m_btnDifference.MoveWindow(rcBtn);
	CBitmap bmpDiff;
	bmpDiff.LoadBitmapW(IDB_BITMAP_DIFFERENCE_DISABLED);
	m_btnDifference.SetBitmap(bmpDiff);
	m_btnDifference.EnableWindow(FALSE);

	AdjustRect(rcBtn);
	m_btnZoomIn.MoveWindow(rcBtn);
	CBitmap bmpZoomIn;
	bmpZoomIn.LoadBitmapW(IDB_BITMAP_ZOOM_IN);
	m_btnZoomIn.SetBitmap(bmpZoomIn);

	AdjustRect(rcBtn);
	m_btnZoomOut.MoveWindow(rcBtn);
	CBitmap bmpZoomOut;
	bmpZoomOut.LoadBitmapW(IDB_BITMAP_ZOOM_OUT);
	m_btnZoomOut.SetBitmap(bmpZoomOut);

	AdjustRect(rcBtn);
	m_btnUndo.MoveWindow(rcBtn);
	CBitmap bmpUndo;
	bmpUndo.LoadBitmapW(IDB_BITMAP_UNDO);
	m_btnUndo.SetBitmap(bmpUndo);
}

void CModelToolDlg::CreateTestRegionOperator(CRect& rcScreen)
{
	CRect rcPanel;
	rcPanel.left    = m_rcImgWnd.right - 48 * 5 - 30 * 2 - 10 * 2 - 20;
	rcPanel.top     = m_rcImgWnd.bottom + 60;
	rcPanel.right   = m_rcImgWnd.right;
	rcPanel.bottom  = rcPanel.top + 48*3 + 20 + 23;

	CWnd* pPanel = (CWnd*)GetDlgItem(IDC_STATIC_RGN_OPERATOR);
	pPanel->MoveWindow(rcPanel);

	CRect rc;
	rc.left   = m_rcImgWnd.right - 48 * 4 - 30 * 2 - 10 - 10;
	rc.top    = rcPanel.top + 15;
	rc.right  = rc.left + 48;
	rc.bottom = rc.top + 48;

	CRect rcTop = rc;

	CBitmap bmpMoveUp;
	bmpMoveUp.LoadBitmapW(IDB_BITMAP_MOVE_UP);
	m_btnMoveUp.SetBitmap(bmpMoveUp);
	m_btnMoveUp.MoveWindow(rc);

	rc.left   = rc.left - 10 - 48;
	rc.top    = rc.bottom + 10;
	rc.right  = rc.left + 48;
	rc.bottom = rc.top + 48;
	CBitmap bmpMoveLeft;
	bmpMoveLeft.LoadBitmapW(IDB_BITMAP_MOVE_LEFT);
	m_btnMoveLeft.SetBitmap(bmpMoveLeft);
	m_btnMoveLeft.MoveWindow(rc);

	CRect rcEdit, rcSpin;
	rcEdit.left   = rcTop.left;
	rcEdit.top    = rcTop.bottom + 10 + 12;
	rcEdit.right  = rcEdit.left + 48;
	rcEdit.bottom = rcEdit.top + 24;
	m_editMovePixel.SetFont(&m_smallFont);
	m_editMovePixel.MoveWindow(rcEdit);
	m_editMovePixel.SetWindowTextW(_T("1"));
	m_spinMovePixel.SetBuddy((CWnd*)&m_editMovePixel);
	m_spinMovePixel.SetRange(1,20);

	rc.left   = rc.right + 10;
	rc.top    = rc.bottom + 10;
	rc.right  = rc.left + 48;
	rc.bottom = rc.top + 48;
	CBitmap bmpMoveDown;
	bmpMoveDown.LoadBitmapW(IDB_BITMAP_MOVE_DOWN);
	m_btnMoveDown.SetBitmap(bmpMoveDown);
	m_btnMoveDown.MoveWindow(rc);
	CRect rcBottom = rc;

	rc.left   = rc.right + 10;
	rc.bottom = rc.top - 10;
	rc.right  = rc.left + 48;
	rc.top    = rc.bottom - 48;
	CBitmap bmpMoveRight;
	bmpMoveRight.LoadBitmapW(IDB_BITMAP_MOVE_RIGHT);
	m_btnMoveRight.SetBitmap(bmpMoveRight);
	m_btnMoveRight.MoveWindow(rc);

	rc.left   = rc.right + 30;
	rc.top    = rcTop.top;
	rc.right  = rc.left + 48;
	rc.bottom = rc.top + 48;
	CBitmap bmpEnlarge;
	bmpEnlarge.LoadBitmapW(IDB_BITMAP_ENLARGE);
	m_btnEnlarge.SetBitmap(bmpEnlarge);
	m_btnEnlarge.MoveWindow(rc);
	CRect rcEnlarge = rc;

	rcEdit.left  = rc.left-2;
	rcEdit.top   = rc.bottom + 10 + 12;
	rcEdit.right = rcEdit.left + 50;
	rcEdit.bottom = rcEdit.top + 24;

	m_editScalePixel.SetFont(&m_smallFont);
	m_editScalePixel.MoveWindow(rcEdit);
	m_editScalePixel.SetWindowTextW(_T("0.5"));
	m_spinScalePixel.SetBuddy((CWnd*)&m_editScalePixel);
	m_spinScalePixel.SetRange(1,20);

	rc.top    = rcBottom.top;
	rc.bottom = rcBottom.bottom;
	CBitmap bmpShrink;
	bmpShrink.LoadBitmapW(IDB_BITMAP_SHRINK);
	m_btnShrink.SetBitmap(bmpShrink);
	m_btnShrink.MoveWindow(rc);
	CRect rcShrink = rc;

	rc.left   = rcEnlarge.right + 30;
	rc.top    = rcEnlarge.top;
	rc.right  = rc.left + 48;
	rc.bottom = rcTop.top + 48;
	CBitmap bmpClockwise;
	bmpClockwise.LoadBitmapW(IDB_BITMAP_ROTATE_CLOCKWISE);
	m_btnClockwise.SetBitmap(bmpClockwise);
	m_btnClockwise.MoveWindow(rc);

	rcEdit.left   = rc.left-2;
	rcEdit.top    = rc.bottom + 10 + 12;
	rcEdit.right  = rcEdit.left + 50;
	rcEdit.bottom = rcEdit.top + 24;
	m_editAngle.SetFont(&m_smallFont);
	m_editAngle.MoveWindow(rcEdit);
	m_editAngle.SetWindowTextW(_T("0.5"));
	m_spinAngle.SetBuddy((CWnd*)&m_editAngle);
	m_spinAngle.SetRange(0.5,180);

	rc.top    = rcShrink.top;
	rc.bottom = rcShrink.bottom;
	CBitmap bmpCounterClockwise;
	bmpCounterClockwise.LoadBitmapW(IDB_BITMAP_ROTATE_COUNTER_CLOCKWISE);
	m_btnCounterClockwise.SetBitmap(bmpCounterClockwise);
	m_btnCounterClockwise.MoveWindow(rc);
	//
	((CWnd *)GetDlgItem(IDC_RADIO_PLANE))->SetFont(&m_sliderTextFont);
	((CWnd *)GetDlgItem(IDC_RADIO_RING))->SetFont(&m_sliderTextFont);
	((CWnd *)GetDlgItem(IDC_RADIO_ALL))->SetFont(&m_sliderTextFont);
	((CWnd *)GetDlgItem(IDC_STATIC_PLANE))->SetFont(&m_sliderTextFont);
	((CWnd *)GetDlgItem(IDC_STATIC_RING))->SetFont(&m_sliderTextFont);
	((CWnd *)GetDlgItem(IDC_STATIC_ALL))->SetFont(&m_sliderTextFont);
	CRect rcRadioPanel,rcRadio, rcLabel;
	rcRadioPanel.right  = rcPanel.left - 15;
	rcRadioPanel.bottom = rcPanel.bottom;
	rcRadioPanel.left   = rcRadioPanel.right - 160;
	rcRadioPanel.top    = rcPanel.top;

	((CWnd *)GetDlgItem(IDC_STATIC_SELECT_REGION))->MoveWindow(rcRadioPanel);

	rcRadio.left   = rcRadioPanel.left + 8;
	rcRadio.top    = rcRadioPanel.top + 35;
	rcRadio.right  = rcRadio.left + 15;
	rcRadio.bottom = rcRadio.top + 30;
	((CWnd *)GetDlgItem(IDC_RADIO_PLANE))->MoveWindow(rcRadio);
	rcLabel.left   = rcRadio.right + 6;
	rcLabel.top    = rcRadio.top + 6;
	rcLabel.right  = rcLabel.left + 130;
	rcLabel.bottom = rcRadio.bottom + 6;
	((CWnd *)GetDlgItem(IDC_STATIC_PLANE))->MoveWindow(rcLabel);

	rcRadio.top    = rcRadio.bottom + 10;
	rcRadio.bottom = rcRadio.top + 30;
	((CWnd *)GetDlgItem(IDC_RADIO_RING))->MoveWindow(rcRadio);
	rcLabel.top    = rcRadio.top + 6;
	rcLabel.bottom = rcRadio.bottom + 6;
	((CWnd *)GetDlgItem(IDC_STATIC_RING))->MoveWindow(rcLabel);

	rcRadio.top    = rcRadio.bottom + 10;
	rcRadio.bottom = rcRadio.top + 30;
	((CWnd *)GetDlgItem(IDC_RADIO_ALL))->MoveWindow(rcRadio);
	rcLabel.top    = rcRadio.top + 6;
	rcLabel.bottom = rcRadio.bottom + 6;
	((CWnd *)GetDlgItem(IDC_STATIC_ALL))->MoveWindow(rcLabel);
}

void CModelToolDlg::CreateSliderWidget() 
{
	m_grayValFilter.LoadChannelImage(IDB_PNG_CHANNEL, _T("png"));
	m_grayValFilter.LoadThumbImage(IDB_PNG_THUMB, _T("png"));
	m_grayValFilter.SetThumbMargin(14);
	m_grayValFilter.SetChannelMax(255);
	m_grayValFilter.SetThumbMax(0);
	m_grayValFilter.SetChannelMin(0);
	m_grayValFilter.SetThumbMin(255);

	m_grayValSliderName.SetFont(&m_sliderTextFont);
	m_grayValLCL.SetFont(&m_sliderTextFont);
	m_grayValMin.SetFont(&m_sliderTextFont);
	m_grayValMax.SetFont(&m_sliderTextFont);
	m_grayValUCL.SetFont(&m_sliderTextFont);
	int highDist=30;
	int nStaticWidth = 120;
	int nSliderWidth = 450;


	CRect rcCheck;
	rcCheck.left   = m_rcImgWnd.right + 80;
	rcCheck.top    = m_rcImgWnd.top;
	rcCheck.right  = rcCheck.left + nStaticWidth +0;
	rcCheck.bottom = rcCheck.top + 30;
	CWnd* pWnd = GetDlgItem(IDC_CHECK_DRAW_SECTOR);
	if(pWnd != nullptr)
	{
		pWnd->SetFont(&m_sliderTextFont);
		pWnd->MoveWindow(rcCheck);
	}
	//
	CRect rcShow;
	rcShow.left   = m_rcImgWnd.right + 255;
	rcShow.top    = m_rcImgWnd.top;
	rcShow.right  = rcShow.left + nStaticWidth + 25;
	rcShow.bottom = rcShow.top + 30;
	pWnd = GetDlgItem(IDC_CHECK_SHOW_FILL);
	if(pWnd != nullptr)
	{
		pWnd->SetFont(&m_sliderTextFont);
		pWnd->MoveWindow(rcShow);
	}
	//

	CRect rc;
	rc.left   = m_rcImgWnd.right + 80;
	rc.top    = rcCheck.bottom + 30;
	rc.right  = rc.left + nStaticWidth;
	rc.bottom = rc.top + 18;
	m_grayValSliderName.MoveWindow(rc);

	rc.left  = rc.right + 10;
	rc.right = rc.left + 40;
	m_grayValLCL.MoveWindow(rc);
	int nLCL = m_grayValFilter.GetChannelMin();
	CString sLcl;
	sLcl.Format(_T("%d"), nLCL);
	m_grayValLCL.SetWindowTextW(sLcl);

	rc.left           = rc.right + 8;
	rc.right          = rc.left + nSliderWidth;
	m_rcGrayValFilter = rc;
	m_grayValFilter.MoveWindow(rc);

	rc.left   = rc.right + 6;
	rc.right  = rc.left + 40;
	m_grayValUCL.MoveWindow(rc);
	int nUCL  = m_grayValFilter.GetChannelMax();
	CString sUcl;
	sUcl.Format(_T("%d"), nUCL);
	m_grayValUCL.SetWindowTextW(sUcl);

	rc.top    = rc.bottom + 6;
	rc.bottom = rc.top + 18;
	int nMin  = m_grayValFilter.GetThumbMin();
	rc.left   = m_rcGrayValFilter.left + int((nMin - nLCL) * m_rcGrayValFilter.Width() / (nUCL - nLCL));
	rc.right  = rc.left + 30;
	m_grayValMin.MoveWindow(rc);
	CString sMin;
	sMin.Format(_T("%d"), nMin);
	m_grayValMin.SetWindowTextW(sMin);

	int nMax = m_grayValFilter.GetThumbMax();
	rc.left  = m_rcGrayValFilter.right - int((nUCL - nMax) * m_rcGrayValFilter.Width() / (nUCL - nLCL));
	rc.right = rc.left + 30;
	m_grayValMax.MoveWindow(rc);
	CString sMax;
	sMax.Format(_T("%d"), nMax);
	m_grayValMax.SetWindowTextW(sMax);

	// Contrast Value Slider
	m_ctrstFilter.LoadChannelImage(IDB_PNG_CHANNEL, _T("png"));
	m_ctrstFilter.LoadThumbImage(IDB_PNG_THUMB, _T("png"));
	m_ctrstFilter.SetThumbMargin(14);
	m_ctrstFilter.SetChannelMax(120);
	m_ctrstFilter.SetThumbMax(40);
	m_ctrstFilter.SetChannelMin(2);
	m_ctrstFilter.SetThumbMin(20);

	m_ctrstName.SetFont(&m_sliderTextFont);
	m_ctrstLCL.SetFont(&m_sliderTextFont);
	m_ctrstValue.SetFont(&m_sliderTextFont);
	m_ctrstUCL.SetFont(&m_sliderTextFont);
	m_ctrstValueHigh.SetFont(&m_sliderTextFont);
	rc.left   = m_rcImgWnd.right + 80;
	rc.top    = rc.bottom +highDist;
	rc.right  = rc.left + nStaticWidth;
	rc.bottom = rc.top + 18;
	m_ctrstName.MoveWindow(rc);

	rc.left   = rc.right + 10;
	rc.right  = rc.left + 40;
	m_ctrstLCL.MoveWindow(rc);
	nLCL =2;
	sLcl.Format(_T("%d"), nLCL);
	m_ctrstLCL.SetWindowTextW(sLcl);

	rc.left         = rc.right + 8;
	rc.right        = rc.left + nSliderWidth;
	m_rcCtrstFilter = rc;
	m_ctrstFilter.MoveWindow(rc);

	rc.left  = rc.right + 6;
	rc.right = rc.left + 40;
	m_ctrstUCL.MoveWindow(rc);
	nUCL = 120;
	sUcl.Format(_T("%d"), nUCL);
	m_ctrstUCL.SetWindowTextW(sUcl);

	rc.top = rc.bottom + 6;
	rc.bottom = rc.top + 18;
	nMin = m_ctrstFilter.GetThumbMin();
	rc.left = m_rcCtrstFilter.left + int((nMin - nLCL) * m_rcCtrstFilter.Width() / (nUCL - nLCL));
	rc.right = rc.left + 30;
	m_ctrstValue.MoveWindow(rc);
	sMin.Format(_T("%d"), nMin);
	m_ctrstValue.SetWindowTextW(sMin);

	nMax = m_ctrstFilter.GetThumbMax();
	rc.left =  m_rcCtrstFilter.right - int((nUCL - nMax) * m_rcCtrstFilter.Width() / (nUCL - nLCL));
	rc.right = rc.left + 30;
	m_ctrstValueHigh.MoveWindow(rc);
	sMax.Format(_T("%d"), nMax);
	m_ctrstValueHigh.SetWindowTextW(sMax);

	// Dilation Value Slider
	m_dilationSzFilter.LoadChannelImage(IDB_PNG_CHANNEL, _T("png"));
	m_dilationSzFilter.LoadThumbImage(IDB_PNG_THUMB, _T("png"));
	m_dilationSzFilter.SetThumbMargin(14);
	m_dilationSzFilter.SetSingleSliderFlag(true);
	m_dilationSzFilter.SetChannelMax(20);
	m_dilationSzFilter.SetThumbMax(20);
	m_dilationSzFilter.SetChannelMin(1);
	m_dilationSzFilter.SetThumbMin(1);

	m_dilationName.SetFont(&m_sliderTextFont);
	m_dilationLCL.SetFont(&m_sliderTextFont);
	m_dilationValue.SetFont(&m_sliderTextFont);
	m_dilationUCL.SetFont(&m_sliderTextFont);
	rc.left   = m_rcImgWnd.right + 80;
	rc.top    = rc.bottom + highDist;
	rc.right  = rc.left + nStaticWidth;
	rc.bottom = rc.top + 18;
	m_dilationName.MoveWindow(rc);

	rc.left   = rc.right + 10;
	rc.right  = rc.left + 40;
	m_dilationLCL.MoveWindow(rc);
	nLCL = m_dilationSzFilter.GetThumbMin();
	sLcl.Format(_T("%d"), nLCL);
	m_dilationLCL.SetWindowTextW(sLcl);

	rc.left         = rc.right + 8;
	rc.right        = rc.left + nSliderWidth;
	m_rcDilationFilter = rc;
	m_dilationSzFilter.MoveWindow(rc);

	rc.left  = rc.right + 6;
	rc.right = rc.left + 40;
	m_dilationUCL.MoveWindow(rc);
	nUCL = m_dilationSzFilter.GetThumbMax();
	sUcl.Format(_T("%d"), nUCL);
	m_dilationUCL.SetWindowTextW(sUcl);

	rc.top = rc.bottom + 6;
	rc.bottom = rc.top + 18;
	nMin = m_dilationSzFilter.GetThumbMin();
	rc.left = m_rcDilationFilter.left + int((nMin - nLCL) * m_rcDilationFilter.Width() / (nUCL - nLCL));
	rc.right = rc.left + 30;
	m_dilationValue.MoveWindow(rc);
	sMin.Format(_T("%d"), nMin);
	m_dilationValue.SetWindowTextW(sMin);
	// Region Size Filter
	m_rgnSzFilter.LoadChannelImage(IDB_PNG_CHANNEL, _T("png"));
	m_rgnSzFilter.LoadThumbImage(IDB_PNG_THUMB, _T("png"));
	m_rgnSzFilter.SetThumbMargin(14);
	m_rgnSzFilter.SetSingleSliderFlag(true);
	m_rgnSzFilter.SetChannelMin(0);
	m_rgnSzFilter.SetThumbMin(0);
	m_rgnSzFilter.SetChannelMax(500);
	m_rgnSzFilter.SetThumbMax(500);

	m_rgnSzSliderName.SetFont(&m_sliderTextFont);
	m_rgnSzLCL.SetFont(&m_sliderTextFont);
	m_rgnSzMin.SetFont(&m_sliderTextFont);
	m_rgnSzUCL.SetFont(&m_sliderTextFont);

	rc.left   = m_rcImgWnd.right + 80;
	rc.top    = rc.bottom + highDist;
	rc.right  = rc.left + nStaticWidth;
	rc.bottom = rc.top + 18;
	m_rgnSzSliderName.MoveWindow(rc);

	rc.left   = rc.right + 10;
	rc.right  = rc.left + 40;
	m_rgnSzLCL.MoveWindow(rc);
	nLCL = m_rgnSzFilter.GetChannelMin();
	sLcl.Format(_T("%d"), nLCL);
	m_rgnSzLCL.SetWindowTextW(sLcl);

	rc.left         = rc.right + 8;
	rc.right        = rc.left + nSliderWidth;
	m_rcRgnSzFilter = rc;
	m_rgnSzFilter.MoveWindow(rc);

	rc.left  = rc.right + 6;
	rc.right = rc.left + 50;
	m_rgnSzUCL.MoveWindow(rc);
	nUCL = m_rgnSzFilter.GetChannelMax();
	sUcl.Format(_T("%d"), nUCL);
	m_rgnSzUCL.SetWindowTextW(sUcl);

	rc.top = rc.bottom + 6;
	rc.bottom = rc.top + 18;
	nMin = m_rgnSzFilter.GetThumbMin();
	rc.left = m_rcRgnSzFilter.left + int((nMin - nLCL) * m_rcRgnSzFilter.Width() / (nUCL - nLCL));
	rc.right = rc.left + 45;
	m_rgnSzMin.MoveWindow(rc);
	sMin.Format(_T("%d"), nMin);
	m_rgnSzMin.SetWindowTextW(sMin);
}

void CModelToolDlg::DisplayPromptMessage(CString sMsg, bool bOpRslt)
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_MSG_PANEL);
	if(pWnd == nullptr)
	{
		return;
	}
	pWnd->ShowWindow(SW_HIDE);
	SetDlgItemTextW(IDC_STATIC_MSG_PANEL, _T(""));
	m_bOpRslt = bOpRslt;
	SetDlgItemTextW(IDC_STATIC_MSG_PANEL, sMsg);
	GetDlgItem(IDC_STATIC_MSG_PANEL)->Invalidate();
	pWnd->ShowWindow(SW_SHOW);
	SetTimer(DLG_TIMER_ID, 10000, NULL);
}

BOOL CModelToolDlg::PreTranslateMessage(MSG* pMsg)
{
	WPARAM param = pMsg->wParam;
	if(param == VK_ESCAPE  || param == VK_RETURN || param == VK_SPACE)
		return TRUE;
	if(pMsg->message == WM_SYSKEYDOWN && param == VK_F4)
		return TRUE;
	if(param == VK_F3 )
	{
		m_mouseSelectRegion=true;
		m_listRgns.clear();
	}
	if((param == VK_F10 )&&m_mouseSelectRegion)
	{
		HTuple dArea, dRow, dColumn,dLength;
		area_center (m_connectedRegions, &dArea, &dRow, &dColumn);
		tuple_length (dRow, &dLength);
		if(dLength>0)
		{
			gen_empty_obj(&m_selectRegion);
			//m_mouseSelectRegion=false;
			disp_obj(m_newImg, m_imgWndID);
			set_color(m_imgWndID, "red");
			disp_obj(m_connectedRegions, m_imgWndID);
			set_color(m_imgWndID, "blue");
			disp_obj(m_finalRgn, m_imgWndID);
		}
	}
	if((param == VK_F7)&&m_mouseSelectRegion)
	{
		m_listRgns.push_back(m_selectRegion);
		m_mouseSelectRegion=false;

		CBitmap bmpUnion;
		bmpUnion.LoadBitmapW(IDB_BITMAP_UNION);
		m_btnUnion.SetBitmap(bmpUnion);
		m_btnUnion.EnableWindow(TRUE);
		m_btnSaveRgn.EnableWindow(TRUE);
	}

	if(pMsg->message == WM_KEYDOWN && (param == VK_LEFT||param == VK_UP))
	{
		int iFocusID = GetFocus()->GetDlgCtrlID();
		//灰度值过滤
		if (iFocusID == IDC_SLIDER_GRAY_VAL_RANGE)
		{
			m_grayValFilter.DecreaseStep();
		}
		//对比度设置
		if (iFocusID == IDC_SLIDER_CONTRAST_VALUE)
		{
			m_ctrstFilter.DecreaseStep();
		}
		//边缘长度过滤
		if (iFocusID == IDC_SLIDER_REGION_SIZE_FILTER)
		{
			m_rgnSzFilter.DecreaseStep();
		}
		//边缘膨胀
		if (iFocusID == IDC_SLIDER_DILATION_SIZE)
		{
			m_dilationSzFilter.DecreaseStep();
		}
	}

	if(pMsg->message == WM_KEYDOWN && (param == VK_RIGHT||param == VK_DOWN))
	{
		int iFocusID = GetFocus()->GetDlgCtrlID();
		//灰度值过滤
		if (iFocusID == IDC_SLIDER_GRAY_VAL_RANGE)
		{
			m_grayValFilter.IncreaseStep();
		}
		//对比度设置
		if (iFocusID == IDC_SLIDER_CONTRAST_VALUE)
		{
			m_ctrstFilter.IncreaseStep();
		}
		//边缘长度过滤
		if (iFocusID == IDC_SLIDER_REGION_SIZE_FILTER)
		{
			m_rgnSzFilter.IncreaseStep();
		}
		//边缘膨胀
		if (iFocusID == IDC_SLIDER_DILATION_SIZE)
		{
			m_dilationSzFilter.IncreaseStep();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CModelToolDlg::OnBnClickedButtonResetRgn()
{
	size_t posPath = m_wstrTargetFile.find_last_of(L"\\");
	wstring wstrModelName = m_wstrTargetFile.substr(0,posPath);
	posPath = wstrModelName.find_last_of(L"\\");
	wstring wstrModelPath = wstrModelName.substr(0,posPath);

	//
	CString sFilterReg  = CW2T(m_wstrTargetFile.c_str());
	::DeleteFile(sFilterReg);
	sFilterReg.Replace(_T(".reg"),_T(".dat"));
	::DeleteFile(sFilterReg);
	//
	CString sFilter  = CW2T(wstrModelPath.c_str());
	m_wstrTargetFile=wstrModelPath;
	sFilter.Append(_T("\\*.*"));
	CFileFind ff;
	BOOL bWorking = ff.FindFile(sFilter);
	m_comboWorkingFolder.ResetContent();
	while(bWorking)
	{
		bWorking = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())
		{
			CString sPath = ff.GetFilePath();
			wstring wstrPath = CT2W(sPath);
			size_t pos = wstrPath.find_last_of(L"\\");
			wstring wstrModelName = wstrPath.substr(pos + 1);
			m_comboWorkingFolder.AddString(wstrModelName.c_str());
		}
	}
	gen_empty_obj(&m_finalRgn);
	DisplayAllRegions();
}

void CModelToolDlg::OnEnChangeNumlevels()
{
	UpdateData(TRUE);
}

BOOL CModelToolDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CRect rect;
	GetClientRect(&rect);

	CBitmap bakImg;
	if(bakImg.LoadBitmap(IDB_BITMAP_BAK))
	{
		BITMAP bitmap;
		bakImg.GetBitmap(&bitmap);
		CBitmap* pPngOld = dcMem.SelectObject(&bakImg);
		int newWidth = bitmap.bmWidth;
		int newHeight = bitmap.bmHeight;
		pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, newWidth ,newHeight,SRCCOPY);
	}
	return TRUE;
//return CDialogEx::OnEraseBkgnd(pDC);
}

void CModelToolDlg::OnBnClickedCheckShowFill()
{
	UpdateData(TRUE);
	DisplayAllRegions();
}

void CModelToolDlg::OnBnClickedButtonSaveModel2()
{
	//校正模板图片
	HTuple Area1,Row1,Col1;
	Hobject DefinedRgn;
	union1(m_definedAngleRgn,&DefinedRgn);
	area_center(m_definedAngleRgn,&Area1,&Row1,&Col1);
	if (Area1>10)
	{
		if (((Row1-m_rowCenterSet).Abs()<5)&((Col1-m_colCenterSet).Abs()<5))
		{
			if(m_bChs)
				DisplayPromptMessage(_T("区域中心与盖面中心距离太近，位置校正失败！"), true);
			else
				DisplayPromptMessage(_T(" Redefined image failed, the distance is too smaller"), true);
		}
		else
		{
			HTuple Angle2,HomMat2D,HomMat2DTmp;
			HTuple  Width211, Height211;
			get_image_size (m_newImg, &Width211, &Height211);
			angle_lx(m_rowCenterSet, m_colCenterSet, Row1, Col1, &Angle2);
			bool isCircle;//CircleShape 未完成！
			if (Height211<300)
			{
				isCircle=CircleShape(m_newImg,0.3,20000);
			} 
			else
			{
				HTuple AreaSmall;
				AreaSmall=3.14*(Height211*0.16)*(Height211*0.16);
				isCircle=CircleShape(m_newImg,0.3,AreaSmall[0].I());
			}
			//
			if(isCircle)
			{
				vector_angle_to_rigid(m_rowCenterSet, m_colCenterSet, 0, Height211/2.0, Width211/2.0, (-Angle2)+(HTuple(90).Rad()), &HomMat2D);
			}
			else
			{
				vector_angle_to_rigid(m_rowCenterSet, m_colCenterSet, 0, Height211/2.0, Width211/2.0, (-Angle2), &HomMat2D);
			}
			hom_mat2d_translate(HomMat2D, 0.5, 0.5, &HomMat2DTmp);
			hom_mat2d_translate_local(HomMat2DTmp, -0.5, -0.5, &HomMat2D);
			affine_trans_image(m_newImg, &m_newImg, HomMat2D, "bilinear", "false");
			affine_trans_region(m_finalRgn,&m_finalRgn, HomMat2D,"constant");
			//重新赋值
			m_colCenterSet= Width211/2.0;
			m_rowCenterSet=Height211/2.0;
		}
	}
	else
	{
		if(m_bChs)
			DisplayPromptMessage(_T("定位区域太小，位置校正失败！"), true);
		else
			DisplayPromptMessage(_T("Redefined image failed, the region is too smaller"), true);
	}
	//清空所画区域列表
	gen_empty_obj(&m_definedAngleRgn);
	m_listRgns.clear();
	CBitmap bmpUnion;
	bmpUnion.LoadBitmapW(IDB_BITMAP_UNION_DISABLED);
	m_btnUnion.SetBitmap(bmpUnion);
	m_btnUnion.EnableWindow(false);
	CBitmap bmpDiff;
	bmpDiff.LoadBitmapW(IDB_BITMAP_DIFFERENCE_DISABLED);
	m_btnDifference.SetBitmap(bmpDiff);
	m_btnDifference.EnableWindow(false);
	//
	DisplayAllRegions();
	//
}


void CModelToolDlg::OnBnClickedButtonClose()
{
	if(m_hChWnd != NULL && m_bTargetFileChanged)
	{
		::PostMessage(m_hChWnd, WM_RGN_FILE_UPDATED, NULL, NULL);
	}
	CDialogEx::OnOK();
}
