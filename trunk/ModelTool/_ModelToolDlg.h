#pragma once

#include <cpp\HalconCpp.h>
#include <vector>
#include  <map>
#include <list>
#include <stack>
#include <string>
#include <algorithm>
#include "LabelCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "RangeSlider.h"
#include "utf8conv.h"
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>

using namespace boost;
using namespace std;
using namespace Halcon;
using namespace utf8util;

#define WM_RGN_FILE_UPDATED	 WM_USER + 22

const int TILTE_HEIGHT            = 80;
const int IDB_DLG_CLOSE           = 70000;

enum REGION_TYPE
{
	REGION_INVALID = -1,
	REGION_LOCAL   = 0,
	REGION_SPECIFIED
};

enum TOOL_TYPE
{
	TOOL_INVALID = -1,
	TOOL_LASSO_CIRCLE = 0,
	TOOL_LASSO_ELLIPSE,
	TOOL_LASSO_RECTANGLE,
	TOOL_LASSO_POLYGON,
	TOOL_DRAW_CIRCLE,
	TOOL_DRAW_ELLIPSE,
	TOOL_DRAW_RECTANGLE,
	TOOL_DRAW_POLYGON,
	TOOL_UNION,
	TOOL_DIFFERENCE,
	TOOL_ZOOM_IN,
	TOOL_ZOOM_OUT,
	TOOL_UNDO
};

enum ACTION_TYPE
{
	ACTION_INVALID = -1,
	ACTION_UNION   = 0,
	ACTION_DIFFERENCE
};

struct ACTION_STEP
{
	ACTION_STEP()
	{
		m_rgn.Reset();
		m_actionType = ACTION_INVALID;
	}
	ACTION_STEP(Hobject obj, ACTION_TYPE type)
		:m_rgn(obj), m_actionType(type)
	{
	}
	ACTION_STEP(const ACTION_STEP& other)
	{
		m_rgn        = other.m_rgn;
		m_actionType = other.m_actionType;
	}
	ACTION_STEP(ACTION_STEP&& other)
	{
		m_rgn        = other.m_rgn;
		m_actionType = other.m_actionType;
	}
	ACTION_STEP& operator=(const ACTION_STEP& other)
	{
		if(&other == this)
			return *this;
		m_rgn        = other.m_rgn;
		m_actionType = other.m_actionType;
		return *this;
	}
	ACTION_STEP& operator=(ACTION_STEP&& other)
	{
		if(&other == this)
			return *this;
		m_rgn        = other.m_rgn;
		m_actionType = other.m_actionType;
		return *this;
	}
	Hobject     m_rgn;
	ACTION_TYPE m_actionType;

};

class CModelToolDlg : public CDialogEx
{
public:
	CModelToolDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_MODELTOOL_DIALOG };

	void SetChannelWindowHandle(HWND hChWnd);
	void SetImageFilePath(const wstring& wstrPath);
	void SetManualRegionFilePath(const wstring& wstrPath);

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void LoadBackgroundImage(CPaintDC& dc);
	void CreateDialogTitleBar(CRect& rcScreen);
	void CreateImageWindow(CRect& rcScreen);
	void CreateFileOperationWidget();
	void CreateMessagePanel(CRect& rcScreen);
	void CreateToolPanel();
	void CreateTestRegionOperator(CRect& rcScreen);
	void CreateSliderWidget();
	void CreateRegionListAndButton();

	void DisplayImage(std::string& strFilePath);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void DisplayAllRegions();
	void SetUnionAndDiffButtonAccessbility();
	void RestoreButtonImage();
	void AdjustSlider(int nSliderID);
	void UpdateSliderLclAndUcl(int nSliderID, int nLCL, int nUCL);
	void UpdateSliderThumb(int nSliderID, int nMin, int nMax);
	Hobject GenerateOutstandingRegion(Hobject& inputRgns, int nSzMin, int nSzMax);
	void CalculateRegionSizeMinMax(Hobject& inputRgns, int& nSzMin, int& nSzMax);
	void ResetInternalHalconObject();
	void ResetParameterFilter();
	void CollectRegion();
	void UnionNewRegions();
	void DifferenceNewRegions();
	void ProcessLassoedRegion(Hobject& targetRgn);
	void SetLassoingStyle();
	void SetPlottingStyle();
	void UndoOneStep();
	void UpdateTestingRegionTable();
	void ReadImageFile();
	void ReadWorkingFolder(const wstring& wstrManualPath = L"");
	void ReadLocalRegionFiles(const wstring& wstrPath);
	void ReadSpecifiedRegionFiles(const wstring& wstrPath);
	void ZoomImageWindow(bool bZoomIn, CPoint pt);
	bool CheckRegionValidity(Hobject& rgn);
	int GetMoveStep();
	CString ParseModelPath(wstring wstrSpecifiedPath);
	void HandleSpecifiedRegionFile(const wstring& wstrPath);
	bool FromSameWorkingFolder(const wstring& wstrPath, std::vector<wstring>& vPath);
	void InitializeWorkingFolderComboBox(std::vector<wstring>& vPath);
private:
	HWND       m_hChWnd;
	const int  DLG_TIMER_ID;
	bool       m_bOpRslt;
	bool       m_bCtrlKeyPressed;
	CLabelCtrl m_dlgTitle;
	CLabelCtrl m_btnClose;
	CEdit      m_editFilePath;
	long       m_imgWndID;
	CButton    m_btnOpenFile;
	CButton    m_btnSwitchImg;
	BOOL       m_bNewModelRgn;
	CComboBox  m_comboModelType;
	CStatic    m_cbName;
	CButton    m_btnCreateModel;
    CButton    m_btnSaveRgn;
	CButton    m_btnUpdateRgn;
	int        m_nOperationType;
	CStatic    m_msgPanel;
	CFont      m_textFont;
	CFont      m_smallFont;
	CRect      m_rcImgWnd;
	//Tool Panel
	TOOL_TYPE  m_crtToolType;
	double     m_dZoomRatio;
	CStatic    m_staticToolPanel;
	CButton    m_btnCircleSel;
	CButton	   m_btnRectSel;
	CButton    m_btnPolygonSel;
	CButton    m_btnCircleDraw;
	CButton    m_btnRectDraw;
	CButton    m_btnPolygonDraw;
	CButton    m_btnUnion;
	CButton    m_btnDifference;
	CButton    m_btnZoomIn;
	CButton    m_btnZoomOut;
	CButton    m_btnUndo;
	Hobject    m_crtImg;
	Hobject    m_modelImg;
	Hobject    m_reducedImgROI;
	Hobject    m_reducedImgStep1;
	Hobject    m_step1Rgns;
	Hobject    m_step3Rgns;
	Hobject    m_finalRgn;
	Hobject    m_modelRgn;
	Hlong      m_modelID;
	CString    m_modelFilePath;
	Hobject    m_crtRgn;
	Hobject    m_firstSetlRgn;
	Hobject    m_allModelRgns;
	Hobject    m_modelImageRead;
	std::wstring              m_wstrCrtRgnName;
	std::map<wstring,Hobject> m_dictTestRgn; // Region Name  <---> Region
	std::map<wstring,Hobject> m_dictRotatedTestRgn;
	std::list<Hobject>        m_listRgns;
	std::stack<ACTION_STEP>   m_oldRgns;

	CFont        m_sliderTextFont;

	CStatic      m_grayValSliderName;
	CRect        m_rcGrayValFilter;
	CRangeSlider m_grayValFilter;
	CStatic      m_grayValLCL;
	CStatic      m_grayValUCL;
	CStatic      m_grayValMin;
	CStatic      m_grayValMax;
	// Contrast value
	CStatic      m_ctrstName;
	CStatic      m_ctrstLCL;
	CRect        m_rcCtrstFilter;
	CRangeSlider m_ctrstFilter;
	CStatic      m_ctrstUCL;
	CStatic      m_ctrstValue;
	CStatic      m_ctrstValueHigh;
	CStatic      m_rgnSzSliderName;
	CRect        m_rcRgnSzFilter;
	CRangeSlider m_rgnSzFilter;
	CStatic      m_rgnSzLCL;
	CStatic      m_rgnSzUCL;
	CStatic      m_rgnSzMin;
	CStatic      m_rgnSzMax;
	// dilation value
	CStatic      m_dilationName;
	CStatic      m_dilationLCL;
	CRect        m_rcDilationFilter;
	CRangeSlider m_dilationSzFilter;
	CStatic      m_dilationUCL;
	CStatic      m_dilationValue;

	CListCtrl    m_listRegion;
	CButton      m_btnDeleteRegion;
	HTuple       m_rowModelCenter;
	HTuple       m_colModelCenter;
	HTuple       m_angleModelCenter;
	int          m_nFilterStep;
	bool         m_bSliderMoved;
	bool         m_createModel;
	CButton          m_btnMoveUp;
	CButton          m_btnMoveDown;
	CButton          m_btnMoveLeft;
	CButton          m_btnMoveRight;
	CEdit            m_editMovePixel;
	CSpinButtonCtrl  m_spinMovePixel;

	CEdit            m_editScalePixel;
	CSpinButtonCtrl  m_spinScalePixel;
	CButton          m_btnEnlarge;
	CButton          m_btnShrink;
	CEdit            m_editAngle;
	CSpinButtonCtrl  m_spinAngle;

	CButton m_btnEllipseSel;
	CButton m_btnEllipseDraw;
	CStatic m_rgnOperatorPanel;
	BOOL    m_bDrawSector;

	Hobject          m_oldRgn;
	Hobject          m_oldRgnEnlargeShrink;
	double           m_dDeltaAngle;
	double           m_dEnlargeShrink;
	bool             m_bClockwiseRotate;
	bool             m_bCounterClockwiseRotate;
	bool             m_bFirstEnlarge;
	bool             m_bFirstShrink;
	CButton          m_btnClockwise;
	CButton          m_btnCounterClockwise;
	CButton          m_btnSelectRegionExtract;
	int              m_selectRegionExtract;
	bool             m_mouseSelectRegion;
	Hobject          m_selectRegion;
	Hobject     	 m_connectedRegions;
	bool             m_drawSectorRegion;
	wstring          m_wstrImgFilePath;
	wstring          m_wstrManualRegionPath;
	vector<wstring>  m_vManualRgnPath;
	CComboBox        m_comboWorkingFolder;
	bool             m_bSpecifiedRgnChanged;
	std::map<wstring, wstring> m_dictFolder_Path;
public:
	afx_msg void OnCloseScreen();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonOpenFile();
	afx_msg void OnBnClickedButtonLoadNewImg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonCreateModel();
	afx_msg void OnBnClickedButtonCircleSel();
	afx_msg void OnBnClickedButtonRectangleSel();
	afx_msg void OnBnClickedButtonPolygonSel();
	afx_msg void OnBnClickedButtonCircleDraw();
	afx_msg void OnBnClickedButtonRectangleDraw();
	afx_msg void OnBnClickedButtonPolygonDraw();
	afx_msg void OnBnClickedButtonUnion();
	afx_msg void OnBnClickedButtonDifference();
	afx_msg void OnBnClickedButtonUndo();
	afx_msg void OnBnClickedButtonZoomIn();
	afx_msg void OnBnClickedButtonZoomOut();
	afx_msg LRESULT OnRangeSliderChanged(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButtonSaveRgn();
	afx_msg void OnBnClickedRadioModel();
	afx_msg void OnBnClickedRadioRegion();
	afx_msg void OnBnClickedButtonDeleteRegion();
	afx_msg void OnNMClickListRegion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonEllipseSel();
	afx_msg void OnBnClickedButtonEllipseDraw();
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonMoveDown();
	afx_msg void OnBnClickedButtonMoveLeft();
	afx_msg void OnBnClickedButtonMoveRight();
	afx_msg void OnBnClickedButtonEnlarge();
	afx_msg void OnBnClickedButtonShrink();
	afx_msg void OnBnClickedButtonRotateClockwise();
	afx_msg void OnBnClickedButtonCounterClockwise();
	afx_msg void OnDeltaposSpinMovePixel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinScalePixel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinDegree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioPlane();
	afx_msg void OnBnClickedRadioRing();
	afx_msg void OnBnClickedRadioAll();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonUpdateRgn();

	std::vector<wstring> SplitString(wstring strTarget, const wstring& strSeperator)
	{
		std::vector<wstring> vSplitRslt;
		boost::split(vSplitRslt, strTarget, is_any_of(strSeperator.c_str()),token_compress_on);

		for(size_t t = 0; t < vSplitRslt.size(); ++t)
		{
			boost::trim(vSplitRslt[t]);
		}
		return vSplitRslt;
	}
	afx_msg void OnCbnSelchangeComboWorkingFolder();
	afx_msg void OnBnClickedCheckDrawSector();
};
