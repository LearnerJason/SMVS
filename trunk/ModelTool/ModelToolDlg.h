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
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>

#include "FileMappingProxy.h"

using namespace boost;
using namespace std;
using namespace Halcon;

#define WM_RGN_FILE_UPDATED	 WM_USER + 22

const int TILTE_HEIGHT     = 80;
const int IDB_DLG_CLOSE    = 70000;

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
	void SetChsLangFlag(bool bChs);
	void SetImageFilePath(const wstring& wstrPath);
	void SetModelAngle(const wstring& wstrPath);
	void SetCreateModelMethod(const wstring& wstrPath);
	void GetCenterXY(double rowCenter,double colCenter);
	
	void SetTargetFilePath(const wstring& wstrPath);
	void SetTargetFileType(const wstring& wstrFileType);
	void SetModelFiles(const wstring& wstrModelFiles);
	void SetTargetMatchingType(const wstring& wstrMatchingType);
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
	void SetWidgetSring(int nCtrlID, CString str);
	void CreateDialogTitleBar(CRect& rcScreen);
	void CreateImageWindow(CRect& rcScreen);
	void CreateFileOperationWidget();
	void CreateMessagePanel(CRect& rcScreen);
	void CreateToolPanel();
	void CreateTestRegionOperator(CRect& rcScreen);
	void CreateSliderWidget();
	void ChangeWidgetText();

	void DisplayImage(std::string& strFilePath);
	void DisplayPromptMessage(CString sMsg, bool bOpRslt = true);
	void DisplayAllRegions();
	void SetUnionAndDiffButtonAccessbility();
	void RestoreButtonImage();
	void AdjustSlider(int nSliderID);
	void UpdateSliderLclAndUcl(int nSliderID, int nLCL, int nUCL);
	void UpdateSliderThumb(int nSliderID, int nMin, int nMax);
	void ResetInternalHalconObject();
	void ResetParameterFilter();
	void UnionNewRegions();
	void DifferenceNewRegions();
	void ProcessLassoedRegion(Hobject& targetRgn);
	void SetLassoingStyle();
	void SetPlottingStyle();
	void UndoOneStep();
	bool ReadWorkingFolder(string& strPath, string& strModelName);
	void ReadRegionFiles(string& strPath, string& strModelName);
	void ZoomImageWindow(bool bZoomIn, CPoint pt);
	bool CheckRegionValidity(Hobject& rgn);
	int GetMoveStep();
	bool ParseModelPath(wstring wstrTargetFile, string& strModelPath, string& strModelName);
	void HandleInputFile(const wstring& wstrPath);
	void SearchAvailableModels();
	void DisplayModelUiWidget();
	void ProcessAndDisplayModel();
	void DisplayRegionUiWidget();
	void ProcessAndDisplayRegion();
	void ProcessRegionsByNewImg();
private:
	CFileMappingProxy<char, 128> m_transferStation;
	HWND       m_hChWnd;
	const int  DLG_TIMER_ID;
	bool       m_bChs;
	bool       m_bOpRslt;
	bool       m_bCtrlKeyPressed;
	CLabelCtrl m_dlgTitle;
	long long  m_imgWndID;
	CStatic    m_staticModelName;
	CEdit      m_editModelName;
	CComboBox  m_comboModelType;
	CButton    m_btnSaveModel;
	CStatic    m_staticModelItem;
	CComboBox  m_comboWorkingFolder;
    CButton    m_btnSaveRgn;
    CButton    m_btnResetRgn;
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
	Hobject    m_newImg;
	Hobject    m_crtImg;
	Hobject    m_modelImg;
	Hobject    m_reducedImgROI;
	Hobject    m_reducedImgStep1;
	Hobject    m_finalRgn;
	Hobject    m_modelRgn;
	Hobject    m_definedAngleRgn;
	Hlong      m_modelID;
	CString    m_modelFilePath;
	Hobject    m_firstSetlRgn;
	std::wstring              m_wstrCrtRgnName;
	std::map<wstring,Hobject> m_dictTestRgn; // Region Name  <---> Region
	std::map<wstring,Hobject> m_dictRotatedTestRgn;
	std::list<Hobject>        m_listRgns;
	std::stack<ACTION_STEP>   m_oldRgns;

	Hobject      m_newModelImageRgn;
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

	HTuple       m_rowModelCenter;
	HTuple       m_colModelCenter;
	HTuple       m_angleModelCenter;
	HTuple       m_rowScaleModelCenter;
	HTuple       m_colScaleModelCenter;
	HTuple       m_angleScaleModel;
	HTuple       m_rowNewImagemMatchingCenter;
	HTuple       m_colNewImagemMatchingCenter;
	HTuple       m_newImagemMatchingScale;
	double       m_dNewImageAngle;
	bool         m_bSliderMoved;
	bool         m_createModel;
	HTuple       m_modelScore;
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
	CButton          m_btnEllipseSel;
	CButton          m_btnEllipseDraw;
	CStatic          m_rgnOperatorPanel;
	BOOL             m_bDrawSector;
	BOOL             m_bShowFill;
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
	Hobject          m_lassoedRgn;
	bool             m_bLassoFinished;
	bool             m_bExternalRgn;
	Hobject     	 m_connectedRegions;
	bool             m_drawSectorRegion;
	wstring          m_wstrImgFilePath;
	wstring          m_wstrCreateModelAngle;
	wstring          m_wstrCreateModelMethod;
	wstring          m_wstrProductName;
	wstring          m_wstrProductNameFirst;
	wstring          m_wstrProductNameSecond;
    wstring	         m_wstrProductNameThird;
	wstring	         m_wstrProductNameFifth;
	wstring          m_wstrTargetFile;
	wstring          m_wstrFileType;
	wstring          m_wstrMatchingType;
	wstring			 m_wstrModelFiles;

	HTuple           m_rowModelCenterSet;
	HTuple           m_colModelCenterSet;
	HTuple           m_rowCenterTrans;
	HTuple           m_colCenterTrans;
	HTuple           m_angleModelCenterSet;
	HTuple           m_rowCenterSet;
	HTuple           m_colCenterSet;
	HTuple           m_angleCenterSet;	
	vector<wstring>  m_vManualRgnPath;
	bool             m_bTargetFileChanged;
	bool             m_bUseScaleModel;
	bool             m_bNewImageMatchingOK;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSaveModel();
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
	afx_msg void OnBnClickedButtonSaveRegion();
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
	afx_msg void OnCbnSelchangeComboWorkingFolder();
	afx_msg void OnBnClickedCheckDrawSector();
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
	bool CheckFileExists(const wstring& path)
	{
		if(INVALID_FILE_ATTRIBUTES == (::GetFileAttributes(path.c_str())))
		{
			return false;
		}
		return true;
	}
	bool  CreateFullDirectory(std::wstring wstrDir)
	{
		std::vector<wstring> vItems = SplitString(wstrDir, L"\\");

		size_t sz = vItems.size();
		wstring wstrCrtPath;
		for(size_t t = 0; t < sz; t++)
		{
			wstrCrtPath.append(vItems[t]);
			if(!CheckFileExists(wstrCrtPath))
			{
				::_wmkdir(wstrCrtPath.c_str());
			}
			if(t != sz - 1)
				wstrCrtPath.append(L"\\");
		}
		return CheckFileExists(wstrCrtPath);
	}
	afx_msg void OnBnClickedButtonResetRgn();
	afx_msg void OnStnClickedStaticGrayValueFilter();
	afx_msg void OnStnClickedStaticContrast();
	afx_msg void OnStnClickedStaticModelItem();
	afx_msg void OnStnClickedStaticModelName();
	afx_msg void OnStnClickedStaticDlgTitle();
	// Set the number levels of shape model.
	int m_numLevels;
	afx_msg void OnEnChangeNumlevels();
	// 设置模板级数，默认为3，参数为0时算子自动选择级数。
	CEdit m_numLevelsEdit;
	// 模板级数设置相关注释。
	CStatic m_staticNumLevels;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCheckShowFill();
	afx_msg void OnBnClickedButtonSaveModel2();
	// 校正模板图片位置和角度。
	CButton m_btnRateModelImage;
	CButton m_btnClose;
	afx_msg void OnBnClickedButtonClose();
};
