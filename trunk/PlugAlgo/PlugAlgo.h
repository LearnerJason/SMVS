#pragma once
#include <vector>
#include <io.h>
#include <map>
#include <numeric>
#include <algorithm>
#include <atlstr.h>
#include <string>
#include <boost\algorithm\string.hpp>
#ifndef   DLLEXPT
#define DLLEXPT extern "C" __declspec (dllexport )
#endif

#include <boost\lexical_cast.hpp>
#include "cpp/HalconCpp.h"
#include "resource.h"  
#include "PlugAlgo_i.h"

#include "../AlgoUtils/AlgoSpy.h"
#include "../AlgoUtils/ImageProcess.h"
#include <string>
using namespace std;
using namespace boost;
using namespace Halcon;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "ATL framework error!"
#endif

using namespace ATL;

void MyHalconExceptionHandler(const Halcon::HException& except)
{
	throw except;
}

class ATL_NO_VTABLE CPlugAlgo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPlugAlgo, &CLSID_PlugAlgo>,
	public IDispatchImpl<IPlugAlgo, &IID_IPlugAlgo, &LIBID_PlugAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<ISelfLearning, &IID_ISelfLearning, &LIBID_PlugAlgoLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPlugAlgo():m_vPos(128),m_vRgn(128),m_vRgnReadData(128),m_vWhiteErrorRgn(128),m_vInterImgs(128),m_vErrorRgn(128),m_ringPara(128),m_sectorAngle(128)
	{
		m_imageNumber     = 0;
		m_isFirst         = true;
		m_bDebugImg       = false;
	    m_bPilotImg       = false;
		m_isFirstMatchingErrorMessage=true;
		m_angleDetect     =1.57;
		m_oldRadius       = -1;
		m_imageRadius     = 100;
		m_modelAngleTmp = HTuple();
		//m_centerCol       =320;
		m_Value     = 0;

	    //newly
		m_Num             =4;
		m_isFirstBot      = true;
		m_oldRadiusBot    = -1;
		m_pixelSize       =0.024766355;
		m_WindowFirst     =true;

		m_oldRadiusDot	  =-1;
		m_isFirstDot	  =true;

		m_modelAngle      =0;
		m_OutCircleRow    =0;
		m_OutCircleCol	  =0;
		m_InnerCircleRadius  =50;
		m_InnerCircleRadius1  =50;

		m_InnerCircleRow=0;
		m_InnerCircleCol=0;
		m_InnerCircleRow1=0;
		m_InnerCircleCol1=0;
		m_GapRow=0;
		m_GapCol=0;
		m_SideTopMoveCol=145;
		m_fCenterX = 0.0;
	    m_fCenterY = 0.0;
		m_Pswitch=1;
		m_CutOff=1;
		m_locationRadius=100;
		m_Dist = 0.0;
		m_TopRow = 0.0;
		m_TopCol = 0.0;
		m_UniqueCirclePhi= 0.0;
		m_TopEdgeRow = 0.0;
		m_BottomEdgeRow = 0.0;
		m_TopDownEdgeRow = 0.0;  
		m_dRowMidUp = 0.0; 
		m_dRowMidMM = 0.0;  
		m_dRowCenterDU = 0.0;
		m_dRowCenterDM = 0.0;
		m_dRowTopBlack = 0.0;
		m_dColTopBlack = 0.0;
		m_FirstRow     = 0.0;
		m_FirstCol     = 0.0;
		m_bSecondFlag  = false;
		m_AngleFirstCir = 0.0;
		m_AngleSecondCir = 0.0;
		m_rgnCharacterArea1 = 0.0;
		m_rgnCharacterArea2 = 0.0;

		gen_empty_region(&m_StripeReg);
		gen_empty_region(&m_InnerCircleT);
		gen_empty_region(&m_TopRegsClosing);
		gen_empty_region(&m_TopRegs);
		gen_empty_region(&m_OutCircle);
		gen_empty_region(&m_ABEdgeM);
		gen_empty_region(&m_ABEdge);
		gen_empty_region(&m_MiddleEdge);
		gen_empty_region(&m_UpReg);
		gen_empty_region(&m_LowerReg);
		gen_empty_region(&m_ABEdgeReg);
		gen_empty_region(&m_SideSlopRegAll);		
		gen_empty_region(&m_ScrapEdgeReg);
		gen_empty_region(&m_TopBaseLine);
		gen_empty_region(&m_BottomBaseLine);     
        gen_empty_region(&m_rgnTopDownEdge); 
		gen_empty_region(&m_TopEdgeROI);   
        gen_empty_region(&m_MidEdgeUpROI);
		gen_empty_region(&m_MidEdgeDownROI); 
        gen_empty_region(&m_DownEdgeUpROI);
		gen_empty_region(&m_DownEdgeDownROI); 
		gen_empty_region(&m_TopFourCircles);  
		gen_empty_region(&m_rgnTopSel);
		gen_empty_region(&m_rgnBottom);
		gen_empty_region(&m_rgnTopBlack);   
		gen_empty_region(&m_rgnTopBorder);
		gen_empty_region(&m_rgnTopFork);
		gen_empty_region(&m_rgnCharacter);  
		gen_empty_region(&m_DownEdgeReg2);
		gen_empty_region(&m_DownEdgeReg); 
		gen_empty_region(&m_rgnPlatForm);  
		gen_empty_region(&m_TopRegionUnion);     
        gen_empty_region(&m_BlackReg);
		gen_empty_region(&m_SlopRegDiff);  
		gen_empty_region(&m_rgnSideTop);
		gen_empty_region(&m_rgnAngleLine1);
		gen_empty_region(&m_rgnAngleLine2);  
		gen_empty_region(&m_rgnSideTopBorder);  
		gen_empty_region(&m_rgnBorder);
		gen_empty_region(&m_TReg );
		gen_empty_region(&m_TopWholeRgn ); 
        gen_empty_region(&m_rgnTop ); 
	    gen_empty_region(&m_rgnForkOri );
		gen_empty_region(&m_rgnTopSlop );
		LCID lcid ;
		lcid=GetUserDefaultUILanguage();
		if((lcid==2052)||(lcid==1028))
		{
			m_isChineseLanguage=true;
		}
		else
		{
			m_isChineseLanguage=false;
		}

	}

DECLARE_REGISTRY_RESOURCEID(IDR_PCCALGO1)


BEGIN_COM_MAP(CPlugAlgo)
	COM_INTERFACE_ENTRY(IPlugAlgo)
	COM_INTERFACE_ENTRY(ISelfLearning)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		HException::InstallHHandler(&MyHalconExceptionHandler);
		return S_OK;
	}

	void FinalRelease()
	{
	}
private:
	std::vector<PIXEL_POS>             m_vPos;          // max size 128
	std::vector<Hobject>               m_vRgn;          // max size 128
	std::vector<HTuple>                m_vRgnReadData;  // max size 128
	std::vector<Hobject>               m_vErrorRgn;     // max size 128
	std::map<int, std::vector<float>>  m_dictSpecialData;  // 改参数用于界面显示上的直方图的数据
	std::vector<RING_PARA>             m_ringPara;
	std::vector<SECTOR_PARA>           m_sectorAngle;

	std::vector<Hobject>               m_vWhiteErrorRgn;     // max size 128
	std::vector<Hobject>               m_vInterImgs;          // max size 128, 中间图像

	Hobject                            m_crtImg;    // current image.
	Hobject                            m_crtImgMult;    // current image,mult.
	Hobject                            m_crtcolorImg;
	Hobject							   m_ImageScaleMax;
	HTuple                             m_imageRadius;
	CAlgoSpy                           m_algoSpy;

	bool                               m_bDebugImg;
	bool                               m_isFirst;
	bool                               m_bPilotImg;
	long int                           m_imageNumber;
	long                               m_imgWidth;
	long                               m_imgHeight;
	volatile bool                      m_bFileUpdated;
	double                             m_angleDetect;
	double                             m_oldRadius;
	HTuple                             m_modelId;
	bool                               m_isChineseLanguage;
	bool                               m_isFirstMatchingErrorMessage;
	HTuple                             m_modelAngle;
	HTuple								m_modelAngleTmp;
	HTuple                             m_locationRadius;

	///plug
	long long						   m_CHImgWndID;
	bool							   m_WindowFirst;

	Hobject                            m_BarRegs;
	HTuple                             m_Num;
	bool                               m_isFirstBot;
	double                             m_oldRadiusBot;
	HTuple                             m_modelIdBot;
	int                                m_oldPalarity;
	double                             m_pixelSize;

	Hobject							   m_ROI;
	Hobject							   m_ABEdge;
	HTuple							   m_ABEdgeRowS;
	HTuple                             m_ABEdgeRow;
	HTuple							   m_Angle;
	HTuple							   m_TopRow;
	HTuple							   m_TopCol;
	HTuple							   m_TopAngleRow;
	HTuple							   m_TopAngleCol;
	Hobject							   m_PlatformReg;
	Hobject							   m_TopRegion;
	HTuple							   m_TopSlopEdgeRow;
	Hobject							   m_ARegionT;
	Hobject							   m_LowerReg;
	Hobject							   m_ContEllipse;
	Hobject							   m_ABEllipse;

	Hobject							   m_CrownRegionT;
	Hobject							   m_ScrapEdgeReg;
	Hobject							   m_BaseRegionLine;
	Hobject							   m_AroiTClosing;
	Hobject							   m_TopInnerRgion;
	
	HTuple							   m_SymProngRowR;
	HTuple							   m_SymProngColR;
	HTuple							   m_SymProngRowL;
	HTuple							   m_SymProngColL;

	Hobject							   m_TopRegions;
	Hobject							   m_TopSlopEdgePoints;
	Hobject							   m_StripeReg1;
	Hobject							   m_StripeReg2;
	
	HTuple							   m_InnerCircleRow;
	HTuple							   m_InnerCircleCol;
	HTuple							   m_OutCircleRow;
	HTuple							   m_OutCircleCol;
	HTuple							   m_InnerCircleRadius;

	HTuple							   m_InnerCircleRow1;
	HTuple							   m_InnerCircleCol1;
	HTuple							   m_InnerCircleRadius1;

	bool							   m_isFirstDot;
	HTuple							   m_modelIdDot;
	double							   m_oldRadiusDot;
	Hobject							   m_smallCircleContour;
	Hobject							   m_Platform0;
	Hobject							   m_Platform;
	Hobject							   m_WholeRoi;
	Hobject							   m_TopInwallRegion;

	Hobject							   m_ABEdgeM;
	HTuple							   m_ABEdgeRowM;
	Hobject							   m_MiddleEdge;
	Hobject							   m_DownEdge;
	Hobject							   m_StripeReg;
	HTuple							   m_AngleP;
	Hobject							   m_ImageReducedAroi;
	HTuple							   m_RowDiff;
	HTuple							   m_ColDiff;
	HTuple							   m_TopRadius;
	Hobject							   m_RubberReg0;
	Hobject							   m_StripeRegD;
	Hobject							   m_InnerCircleT;
	Hobject							   m_TopRegsClosing;
	Hobject							   m_TopRegs;
	Hobject							   m_OutCircle;
	HTuple							   m_GapRow;
	HTuple							   m_GapCol;
	HTuple	                           m_SideTopMoveCol;

	Hobject							   m_InnerEdge;
	Hobject							   m_FMpoints;
    HTuple							   m_FMrows;
	HTuple							   m_FMcols;
	HTuple							   m_ColT1;
	HTuple							   m_ColT2;
	HTuple							   m_ColT3;
	HTuple							   m_ColT4;
	HTuple							   m_EdgeAngleT1;
	HTuple							   m_EdgeAngleT2;
	HTuple							   m_EdgeAngleT3;
	HTuple							   m_EdgeAngleT4;
	HTuple					           m_DownEdgeArea;
	Hobject						       m_MiddleNeckRegion;
	Hobject						       m_TopRegsDilationXY;
	Hobject						       m_TopSlopReg;
	Hobject						       m_InwallSlopReg;
	Hobject						       m_XYTransLowRegs;
	Hobject							   m_TopLowRegs;
	HTuple							   m_CenterNewX;
	HTuple							   m_CenterNewY;
	Hobject	                           m_ContRegionDilation;
	HTuple							   m_ColDiffMax;

	Hobject                            m_rgnHighLight;
	Hobject                            m_rgnGap;
	//冠部二次定位前，保存原定位中心
	float                              m_fCenterX;
	float                              m_fCenterY;                        

	Hobject							   m_ConeEllipse;
	int								   m_Switch;
	HTuple							   m_WholeColumnL;
	HTuple							   m_WholeColumnR;
	HTuple							   m_PlatColumnL;
	HTuple							   m_PlatColumnR;
	Hobject							   m_PatternEdge;
	Hobject							   m_UpReg;
	Hobject							   m_TopRect;
	Hobject							   m_RectL;
	Hobject							   m_RectR;
	int 							   m_Pswitch;
	Hobject							   m_TriangleReg;
	Hobject							   m_TriangleRoi;
	int								   m_CutOff;
	Hobject							   m_ABEdgeReg;
	Hobject							   m_SideSlopRegAll;
	Hobject							   m_DownEdgeX;

	float                             m_Value;
	Hobject                           m_DownEdgePoints1;
	Hobject							  m_DownEdgePoints2;
	Hobject                           m_TopInwallRegionOrigin;
	Hobject                           m_ARegionDiff1;                    // 双叉覆膜颈部上区域
	HTuple                           m_UniqueCirclePhi;             // 外小圆角度
	HTuple                           m_Dist;                     // 外小圆半径
	HTuple                           TopRow;                     // 外Top小圆 Row
	HTuple                           TopCol;                     // 外Top小圆 Col
	Hobject                          m_TopBaseLine;              // 顶部基准线
	Hobject                          m_BottomBaseLine;           // 底部基准线  
	HTuple                           m_TopEdgeRow;               // 顶部基准线中心Row     
	HTuple                           m_BottomEdgeRow;            // 底部基准线中心Row
	Hobject                          m_rgnTopDownEdge;           // 顶部下边缘线 
	Hobject                          m_TopEdgeROI;               // 顶部区域
	HTuple                           m_TopDownEdgeRow;           //  顶部下边缘线中心Row   
	HTuple                           m_dRowMidUp;                //  中部上边缘线中心Row  
	HTuple                           m_dRowMidMM;                //  中部中心边缘线中心Row 
	Hobject                          m_MidEdgeUpROI;             //  中部边缘上区域  
	Hobject                          m_MidEdgeDownROI;             //  中部边缘下区域  
	HTuple                           m_dRowCenterDU;             //  下部上边缘线中心Row 
	HTuple                           m_dRowCenterDM;             //  下部中边缘线中心Row  
	Hobject                          m_DownEdgeUpROI;             //  下部边缘上区域  
	Hobject                          m_DownEdgeDownROI;           //  下部边缘下区域  

	Hobject                          m_TopFourCircles;            // 双叉裸塞顶部四个圆区域
	Hobject                          m_rgnTopSel;                 // 内底区域+高亮区域 + 内壁区域
	Hobject                          m_rgnBottom;                 // 内底区域
	HTuple                           m_dRowTopBlack;               // 顶部内孔椭圆中心 (四叉)
	HTuple                           m_dColTopBlack;               // 顶部内孔椭圆中心 （四叉）
	HTuple                           m_FirstRow;                   // 顶部高亮下区域中心Row（四叉）
	HTuple                           m_FirstCol;                   // 顶部高亮下区域中心Col（四叉）
	Hobject                          m_rgnTopBlack;                // 頂部內孔橢圓區域 （四叉側面）
	Hobject                          m_rgnTopBorder;               // 顶部高亮边缘区域 （四叉顶部）
	Hobject                          m_rgnTopFork;                 // 顶部叉口区域 （四叉)
	Hobject                          m_rgnCharacter ;              // 底部字符区域
	Hobject                          m_DownEdgeReg2;               // 覆膜下边缘线
	Hobject                          m_DownEdgeReg;                // 覆膜上边缘线
	bool                             m_bSecondFlag;
	Hobject                          m_rgnPlatForm;                // 二次提取的三叉叉口区域
	Hobject                          m_TopRegionUnion;             // 三叉顶部区域
	Hobject                          m_BlackReg;                   // 三叉斜坡间黑色区域
	Hobject                          m_SlopRegDiff;                // 三叉斜坡区域（未经过缩放）
	Hobject                          m_rgnSideTop;                 // 注射器胶塞侧面顶部高亮区域
	HTuple                           m_AngleFirstCir;              // 第一个小圆的角度
	HTuple                           m_AngleSecondCir;             // 第二个小圆的角度
	Hobject                          m_rgnAngleLine1;
	Hobject                          m_rgnAngleLine2;
	Hobject                          m_rgnSideTopBorder;           // 注射器胶塞顶部边缘区域
	HTuple                           m_rgnCharacterArea1;           // 文字区域面积1
	HTuple                           m_rgnCharacterArea2;           // 文字区域面积2 
	Hobject                          m_rgnBorder;                   //  32胶塞内底边缘区域
	Hobject                          m_WhiteStripeReg;              // 花纹上的白条区域
	Hobject                          m_TReg;                        // 三角花纹
	Hobject                          m_TopWholeRgn;                 // 顶部整体区域
	Hobject                          m_rgnTop;                      // 顶部工位的顶部高亮区域
	Hobject                          m_rgnForkOri;                  // 叉口初始定位
	Hobject                          m_rgnTopSlop;                  // 顶部相机拍摄的胶塞斜坡区域

	void(ExtractEdgesPoints)(Hobject Image, HTuple Elements, HTuple DetectHeight, HTuple DetectWidth, HTuple Sigma, HTuple Threshold, HTuple Transition, HTuple Select, 
		HTuple Row1, HTuple Column1,HTuple Row2, HTuple Column2, HTuple *ResultRow, HTuple *ResultColumn);
	void(FitLineUseEdgesPoints)(HTuple Rows,HTuple Cols,HTuple ActiveNum,HTuple *Row1,HTuple *Col1,HTuple *Row2,HTuple *Col2);
	//
	void P_GetEllipseLongAxis(Halcon::Hobject ABEllipse, Halcon::Hobject * BaseRegionLine);
	void P_PlatformSelect(Hobject PlatformAB,Hobject PlatformRegDilation,HTuple SearchSize,HTuple TopRow,HTuple TopCol,HTuple TopAngleRow,HTuple TopAngleCol, Hobject * Platform,Hobject * PlatformReg,Hobject *PlatformRegDilation0);
	//目前未修改 与P_PlatformSelect同
	void P_PlatformSelectNoAngle(Hobject PlatformAB,Hobject PlatformRegDilation,HTuple SearchSize,HTuple TopRow,HTuple TopCol,HTuple TopAngleRow,HTuple TopAngleCol, Hobject * Platform,Hobject * PlatformReg,Hobject *PlatformRegDilation0);
	void P_EdgePointsFitCurve(Hobject TopSlopSearchReg, Hobject Image,HTuple PartInterval, HTuple LineLength, HTuple RemoveSize, HTuple * DefinedRow, HTuple *DefinedCol);
	//
	void P_20D_PlatformLocationRAlgo (Hobject PlatformABIntersection, Hobject PlatformRegDilation, HTuple SearchSize, HTuple TopCenTerCol, HTuple SymProngRowR, HTuple SymProngColR,Hobject *PlatformRegR, Hobject *PlatformRegRDilation,Hobject *PlatformRegRDilation0);
	void P_20D_PlatformLocationLRAlgo (Hobject PlatformABIntersection, Hobject PlatformRegDilation, HTuple SearchSize, HTuple TopCenTerCol, HTuple SymProngRowL, HTuple SymProngColL, HTuple SymProngRowR, HTuple SymProngColR,
		Hobject *PlatformRegL, Hobject *PlatformRegR, Hobject *PlatformRegLDilation, Hobject *PlatformRegRDilation,Hobject *PlatformRegLDilation0, Hobject *PlatformRegRDilation0);
	void P_20D_PlatformLocationLAlgo (Hobject PlatformABIntersection, Hobject PlatformRegDilation, HTuple SearchSize, HTuple TopCenTerCol, HTuple SymProngRowL, HTuple SymProngColL,Hobject *PlatformRegL, Hobject *PlatformRegLDilation,Hobject *PlatformRegLDilation0);

	void P_20D_BlackRegRSelectAlgo (Hobject BlackRegsConnected, HTuple BlackRegsNumber, HTuple TopCenTerCol, HTuple SymProngRowR, HTuple SymProngColR, Hobject *BlackRegR);
	void P_20D_BlackRegLRSelectAlgo (Hobject BlackRegsConnected, HTuple BlackRegsNumber, HTuple TopCenTerCol, HTuple SymProngRowL, HTuple SymProngColL, HTuple SymProngRowR, HTuple SymProngColR, Hobject *BlackRegL, Hobject *BlackRegR);
	void P_20D_BlackRegLSelectAlgo (Hobject BlackRegsConnected, HTuple BlackRegsNumber, HTuple TopCenTerCol, HTuple SymProngRowL, HTuple SymProngColL, Hobject *BlackRegL);

	void R_EdgePointsFitCurve(Hobject SearchReg, Hobject Image,HTuple PartInterval, HTuple RemoveSize, HTuple Threshold,HTuple Transition, HTuple Select,HTuple * DefinedRow, HTuple *DefinedCol);
	void P_G20_InWallRegionDetect (Hobject AroiTSelected, Hobject GrayImage, Hobject RegionMoved, Hobject ABEllipseAffineTrans, Hobject ROI, Hobject *InWallRegion, HTuple GrayValue, HTuple DilationSizeU, 
		HTuple DilationSizeD, HTuple ErosionSize);
	void P_13D_InWallRegionDetect(Hobject ARegionT, Hobject PlatformReg,Hobject AroiTSelected, Hobject ROI,HTuple BaseRow,HTuple BaseColumn, HTuple RowPos,HTuple ErosionSize,Hobject *InWallRegion);
	//TopRegion 顶部区域；//PlatformReg叉口平台区域；//PlatformRegV  叉口 竖直区域；//ThresholdBlack 背景阈值设置；//ThresholdPlug，胶塞阈值设置，提取胶塞顶部亮区域
    void P_13_InWallRegionDetect2 (Hobject GrayImage, Hobject ROI, Hobject ABEllipseAffineTrans,Hobject TopRegion,Hobject PlatformReg, Hobject PlatformRegV, Halcon::Hobject *InWallRegion, HTuple ThresholdBlack, HTuple ThresholdPlug);
	//开口边 斜线边缘
	void P_AllSideConeBiasDection (Hobject ConeReg,Hobject Platform, Hobject *ConeEdge, HTuple RadiusSet, HTuple CenterCol, HTuple RowSet,HTuple ColSet);

	//20双叉侧面顶部角度
	void P_SideDetectAngle (Halcon::Hobject Region, Halcon::HTuple Row1, Halcon::HTuple Col1, Halcon::HTuple Row2, Halcon::HTuple Col2, Halcon::HTuple *Angle,Halcon::Hobject *LineRegion);
	void P_20T_PlatformLocationRAlgo (Hobject PlatformABIntersection, Hobject PlatformRegs, HTuple SearchSize, HTuple TopCenTerCol,Hobject *PlatformRegR, Hobject *PlatformRegRDilation,Hobject *PlatformRegRDilation0);
	void P_20T_PlatformLocationLAlgo (Hobject PlatformABIntersection, Hobject PlatformRegs, HTuple SearchSize, HTuple TopCenTerCol,Hobject *PlatformRegL, Hobject *PlatformRegLDilation,Hobject *PlatformRegLDilation0);
	void P_20T_PlatformLocationLRAlgo (Hobject PlatformAB, Hobject PlatformRegs, HTuple SearchSize, HTuple TopCenTerCol,Hobject *PlatformReg, Hobject *PlatformRegDilation, Hobject *PlatformRegDilation0);
	void P_AsideConeBiasDection (Halcon::Hobject RegsConnected, Halcon::Hobject ConeReg, Halcon::Hobject Platform, Halcon::Hobject *ConeEdge, Halcon::HTuple TopCenTerRow);
	void P_BsideConeBiasDection (Halcon::Hobject RegsConnected, Halcon::Hobject ConeReg, Halcon::Hobject Platform, Halcon::Hobject *ConeEdge, Halcon::HTuple TopCenTerRow);
	void P_ABsideConeBiasDection (Halcon::Hobject RegsConnected, Halcon::Hobject ConeReg, Halcon::Hobject Platform, Halcon::Hobject *ConeEdge, Halcon::HTuple TopCenTerCol);
	void P_20T_InWallRegionDetect (Halcon::HTuple Pos,Halcon::Hobject TopRect, Halcon::Hobject PlatformReg, Halcon::Hobject ARegionM, Halcon::Hobject ROI, Halcon::Hobject ABEllipseAffineTrans, Halcon::Hobject ImageReducedAroi,Halcon::HTuple ThredLower,Halcon::HTuple AreaLimitLower, Halcon::HTuple Angle,Halcon::Hobject *InWallRegion);
  
	void GaussConvolution(Hobject imgSrc,Hobject& imgOut,HTuple iConvolWay,HTuple dConvolSize);

	void my_disp_obj(const Halcon::Hobject Object);
	void AnnulusRegionLocation (Hobject *rgnAnnulus, HTuple dCenterRow, HTuple dCenterColumn, HTuple dRadius, HTuple dAnnulusWidth);

	// 三叉内壁区域算子
	void P_20D_SideInWallLocationL (Hobject rgnPlatForm, Hobject rgnTopUni, Hobject rgnSlopBlack, Hobject ABEdgeMoved1, Hobject *rgnInWallLeft, Hobject *rgnLineLimitL, 
		                            HTuple m_TopRow, HTuple m_TopCol, HTuple dRowDownPos, HTuple dInnerEdgeSize, HTuple dOuterEndgeSize, HTuple dLeftUpEdgeSize);
	void P_20D_SideInWallLocationR (Hobject rgnPlatForm, Hobject rgnTopUni, Hobject rgnSlopBlack,Hobject ABEdgeMoved1, Hobject *rgnInWallRight, Hobject *rgnLineLimitR, 
		                            HTuple m_TopCol, HTuple dRowDownPos, HTuple dInnerEdgeSize, HTuple dOuterEndgeSize, HTuple dLeftUpEdgeSize);

	void RemoveSpecialAngleRegion (Hobject rgnInput, Hobject *rgnOutPut, HTuple dAngleStart, HTuple dAngleEnd);
	void GetSpecialAngleRegion (Hobject rgnInput, Hobject *rgnOutPut, HTuple dAngleStart, HTuple dAngleEnd);


public:
	//
	ImageProcess ImageProcessAlg;
	HTuple  m_modelMatchingPARA;
	HTuple  m_modelPARA;
		// For updating external files
	std::map<string, Hobject> m_dictNewPathRgn;     // File Path <===> Region
	std::map<string, Hobject> m_dictCrtPathRgn;     // File Path <===> Region

	std::map<string, HTuple>  m_dictNewPathModelID; // File Path <== Model ID
	std::map<string, HTuple>  m_dictCrtPathModelID; // File Path <== Model ID
	
	//
	STDMETHOD(GetInterImgs)(BYTE rgnID,  LONG* pInterImgs);
	STDMETHOD(SetCurrentImage)(LONG* pImg);
	STDMETHOD(GetRegion)(BYTE rgnID, LONG* pRgn);
	STDMETHOD(GetErrorRegion)(BYTE rgnID, LONG* pErrorRgn);
	STDMETHOD(GetWhiteErrorRegion)(BYTE rgnID, LONG* pWhiteErrorRgn);
	STDMETHOD(GetInternalStats)(BYTE rgnID, LONG nMaxLen,FLOAT* pStatsArray, LONG* nActualLen);
	STDMETHOD(ReloadExternalFile)(BSTR* bstrPath);
	// self-learning interface methods
	STDMETHOD(SetSelfLearningParam)(DWORD sampleSz, BSTR* bstrParam);
	STDMETHOD(SelectModelImage)(LONG* pCrtImg, BYTE productID);
	STDMETHOD(DeleteModelImage)(BYTE nIndex, BYTE productID);
	STDMETHOD(SaveResult)(BSTR* strModelNameD, BYTE productID);
	STDMETHOD(AddModelImage)(LONG* pImg, BSTR* strFolder, BYTE productID);
	STDMETHOD(GetSelfLearningResult)(LONG*  multiModelImag, long* cMax, float* vImgScore,BYTE productID);
	STDMETHOD(ResetSelfLearningRegion)(BSTR* strRegionExtractPara);
	STDMETHOD(GetSelfLearningRegion)(LONG* pRgn);
	STDMETHOD(ResetSelfLearningProcedure)(void);

	STDMETHOD (GetCurrentLocationXY)(BSTR* bstrLocationXY);
	//
	STDMETHOD(CenterLocationAlgo)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ImageCenterLocationAlgo)(VARIANT* rgnID);
	STDMETHOD(ImageCenterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ModelCreate)(double Radius, HTuple * ModelID);

	STDMETHOD(CenterLocationAlgo1)(VARIANT* rgnID, VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterLocationAlgo1Help)(BSTR* pHelpStr);
	
	STDMETHOD(CenterLocationAlgoHS)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize);
	STDMETHOD(CenterLocationAlgoHSHelp)( BSTR* pHelpStr);
	STDMETHOD(CenterRelocationAlgoHS1)(VARIANT* rgnId,  VARIANT* pRadius, VARIANT* pStdRadius, VARIANT* pThreshValue, VARIANT* pOpeningSize, VARIANT* pClosingSize);
	STDMETHOD(CenterRelocationAlgoHS1Help)( BSTR* pHelpStr);
	
	STDMETHOD(CenterLocationAlgoM)(VARIANT* rgnId, VARIANT* pRowStart,VARIANT* pColStart,VARIANT* pRegWidth,VARIANT* pRegHeight, VARIANT* pThresholdUp);
	STDMETHOD(CenterLocationAlgoMHelp)( BSTR* pHelpStr);

	STDMETHOD(MatchingParaAlgo)(VARIANT* rgnId, VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MatchingParaMinScoreAlgo)(VARIANT* rgnId, VARIANT* pMinScore, VARIANT* pNumLevels,VARIANT* pGreediness, VARIANT* pRingOrOCR, BSTR* bstrShm1File, BSTR* bstrShm2File, VARIANT* pMatchingResult);
	STDMETHOD(MatchingParaMinScoreAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	
	STDMETHOD(CircleRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pRadius, VARIANT* pRoiWidth);	
	STDMETHOD(CircleRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(DetectRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pErosionSize, VARIANT* pRingOrOCR, BSTR* bstrRgnFile, BSTR* bstrExtractRgnFile);
	STDMETHOD(DetectRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CurlOptimizeAlgo)(VARIANT* rgnId, VARIANT* pFreq_start, VARIANT* pFreq_cutoff, VARIANT* pDefectType, VARIANT* pWaveThreshold, VARIANT* pMinArea, VARIANT* pDetectNumber);
	STDMETHOD(CurlOptimizeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdAlgo1)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(AB20_FilmBigErrorDetectAlgo)(VARIANT* rgnId, VARIANT* pMeanSizeWidth,VARIANT* pMeanSizeHeight, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pClosingSize1, VARIANT* pClosingSize2, VARIANT* pAreaLimit,VARIANT* pDetectArea);
	STDMETHOD(AB20_FilmBigErrorDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(BubbleDetectProfileAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(BubbleDetectProfileAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BubbleDetectProfileForAnnulusAlgo)(VARIANT* rgnId, VARIANT* pMaskDirection,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDefectShape,VARIANT* pDetectArea);
	STDMETHOD(BubbleDetectProfileForAnnulusAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BubbleDetectProfileForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pMaskDirction,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(BubbleDetectProfileForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BubbleDetectProfileForColorFilmAlgo)(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pMaskDirction,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pMoveSize,VARIANT* pDetectArea);
	STDMETHOD(BubbleDetectProfileForColorFilmAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(N_BubbleDetectProfileAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(N_BubbleDetectProfileAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SmallCircleDefectDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskDirection,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDefectShape,VARIANT* pDetectArea);
	STDMETHOD(SmallCircleDefectDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CurlNoCompoundAlgo)(VARIANT* rgnId,VARIANT*pGrayValueMean );
	STDMETHOD(CurlNoCompoundAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(CenterLocationRightOrNotAlgo)(VARIANT* rgnId,VARIANT*pGrayValueMean );
	STDMETHOD(CenterLocationRightOrNotAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(UVCurlAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue, VARIANT* pDetectArea);
	STDMETHOD(UVCurlAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdAlgo)( VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdAlgo1)( VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize,VARIANT* pLowValue,VARIANT* pHighValue, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdAlgo1Help)( BSTR* pHelpStr);

	STDMETHOD (BlackDetectExcludeEdgesAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize, VARIANT* pThresholdLow,  VARIANT* pMinEdgeSize, VARIANT* pDetectArea);
	STDMETHOD (BlackDetectExcludeEdgesAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (BlackDetectAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pBlackThresh,VARIANT* pSeriousBlackPointSize,  VARIANT* pDetectArea);
	STDMETHOD (BlackDetectAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (BlackDetectUseUpSizeAlgo)(VARIANT* rgnId, VARIANT* pBlackThresh,VARIANT* pThredForeground,VARIANT* pWhiteSize,VARIANT* pSeriousBlackPointSize,VARIANT* pBlackPointUpSize,VARIANT* pDetectArea);
	STDMETHOD (BlackDetectUseUpSizeAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (WhiteDetectAlgo)(VARIANT* rgnId,  VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pWhiteThresh,VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD (WhiteDetectAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD (MinMaxGrayDetectAlgo)(VARIANT* rgnId,  VARIANT* pBlackThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteThresh,VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD (MinMaxGrayDetectAlgoHelp)( BSTR* pHelpStr);

	STDMETHOD(EdgeAlgo)(VARIANT* rgnId, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(LineDetectionAlgo)(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize, VARIANT* pDefectArea);
	STDMETHOD(LineDetectionAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(LineDetectionAlgo1)(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize, VARIANT* pDefectArea);
	STDMETHOD(LineDetectionAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(LineDetectionAlgo2)(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectShape,VARIANT* pDefectSize, VARIANT* pDefectArea);
	STDMETHOD(LineDetectionAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(GLineAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteMaskSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize, VARIANT* pDetectArea);
	STDMETHOD(GLineAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(N_LineDetectionAlgo)(VARIANT* rgnId, VARIANT* pMaskSize, VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectType,VARIANT* pDefectSize, VARIANT* pDefectArea);
	STDMETHOD(N_LineDetectionAlgoHelp)(BSTR* pHelpStr);
	
	STDMETHOD(DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DerivateGaussDetectAlgo1)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize,VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(DerivateGaussDetectForAnnulusAlgo)(VARIANT* rgnId, VARIANT* pFeatureToConv,VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectForAnnulusAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DerivateGaussDetectForAnnulusAlgo1)(VARIANT* rgnId, VARIANT* pFeatureToConv,VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pDefectShape, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectForAnnulusAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(DerivateGaussDetectFreeModelAlgo)(VARIANT* rgnId, VARIANT* pFeatureToConv,VARIANT* pMaskSize,VARIANT* pFeatureToConv2,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pMoveSize, VARIANT* pDetectArea);
	STDMETHOD(DerivateGaussDetectFreeModelAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(N_DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(N_DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_DerivateGaussDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDilationSize,VARIANT* pDetectArea);
	STDMETHOD(D13_DerivateGaussDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(GSurfaceAlgo)(VARIANT* rgnId, VARIANT* pBlackThred, VARIANT* pBlackAreaThred, VARIANT* pWhiteThred, VARIANT* pWhiteAreaThred, VARIANT* pDetectArea);
	STDMETHOD(GSurfaceAlgoHelp)(BSTR* pHelpStr);	

	STDMETHOD(SetCurrentTaskName)(BSTR* bstrTaskName);
	STDMETHOD(SetSelfLearningTargetTaskName)(BSTR* bstrTaskName);

	//newly
	STDMETHOD (TotalHeightMeasureAlgo)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
    STDMETHOD (TotalHeightMeasureAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (TotalHeightMeasureAlgo2)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (TotalHeightMeasureAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD (TotalHeightMeasureInjectorAlgo)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (TotalHeightMeasureInjectorAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (TotalHeightMeasureAlgo3)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pPlugType,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pGraythresh,VARIANT* pRectifyValue,VARIANT* pDefectDiff);
	STDMETHOD (TotalHeightMeasureAlgo3Help)(BSTR* pHelpStr);
	STDMETHOD (TotalHeightMeasureAlgo33)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pPlugType,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pGraythresh,VARIANT* pRectifyValue,VARIANT* pDefectDiff);
	STDMETHOD (TotalHeightMeasureAlgo33Help)(BSTR* pHelpStr);

	STDMETHOD (TotalHeightMeasureAlgo1)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pGraythresh,VARIANT* pDefectDiff);
	STDMETHOD (TotalHeightMeasureAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD (ForkTotalHeightMeasureAlgo)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (ForkTotalHeightMeasureAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (ForkTotalHeightMeasureAlgo1)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pGraythresh,VARIANT* pDefectDiff);
	STDMETHOD (ForkTotalHeightMeasureAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD (CrownHeightMeasureAlgo)(VARIANT* rgnId, VARIANT* pStdCrownHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (CrownHeightMeasureAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (CrownHeightMeasureAlgo2)(VARIANT* rgnId, VARIANT* pStdCrownHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (CrownHeightMeasureAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD (CrownHeightMeasureAlgo1)(VARIANT* rgnId, VARIANT* pStdCrownHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pGraythresh,VARIANT* pDefectDiff);
	STDMETHOD (CrownHeightMeasureAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD (CrownHeightMeasureAlgo3)(VARIANT* rgnId, VARIANT* pStdCrownHeight,VARIANT* pPlugType,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pRectifyValue,VARIANT* pDefectDiff);
	STDMETHOD (CrownHeightMeasureAlgo3Help)(BSTR* pHelpStr);
	STDMETHOD (CrownHeightMeasureAlgo33)(VARIANT* rgnId, VARIANT* pStdCrownHeight,VARIANT* pPlugType,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pGraythresh,VARIANT* pRectifyValue,VARIANT* pDefectDiff);
	STDMETHOD (CrownHeightMeasureAlgo33Help)(BSTR* pHelpStr);

	STDMETHOD (CrownHeightMeasureSAlgo)(VARIANT* rgnId, VARIANT* pStdCrownHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (CrownHeightMeasureSAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (CrownHeightMeasureSAlgo1)(VARIANT* rgnId, VARIANT* pStdCrownHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pGraythresh,VARIANT* pDefectDiff);
	STDMETHOD (CrownHeightMeasureSAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD (DiameterMeasureAlgo)(VARIANT* rgnId, VARIANT* pStdDiameter,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (DiameterMeasureAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (DiameterMeasureAlgo3)(VARIANT* rgnId, VARIANT* pStdDiameter,VARIANT* pPlugType,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pRectifyValue,VARIANT* pDefectDiff);
	STDMETHOD (DiameterMeasureAlgo3Help)(BSTR* pHelpStr); 

	STDMETHOD (DiameterMeasureAlgo2)(VARIANT* rgnId, VARIANT* pStdDiameter,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (DiameterMeasureAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD (HeightMeasureAlgo)(VARIANT* rgnId, VARIANT* pStdTotalHeight,VARIANT* pMaskSize,VARIANT* pThredLow, VARIANT* pThredUpper,VARIANT* pDefectDiff);
	STDMETHOD (HeightMeasureAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (BarLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pMaskSize,VARIANT* pThreshLowValue,VARIANT* pThreshUpValue,VARIANT* pAreaLimitSize);
	STDMETHOD (BarLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (BarLocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pGrayValue,VARIANT* pAreaLimitSize);
	STDMETHOD (BarLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD (BarLocationAlgo2)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pGrayValue,VARIANT* pAreaLimitSize,VARIANT* pMaskSize,VARIANT* pDynThred);
	STDMETHOD (BarLocationAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD (BarLocationAlgo3)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pAreaLimitSize,VARIANT* pMaskSize,VARIANT* pDynThred);
	STDMETHOD (BarLocationAlgo3Help)(BSTR* pHelpStr);
	STDMETHOD (BottomCharacterLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT*pMaskSize,VARIANT*pThredValue,VARIANT*pCharacterArea );
	STDMETHOD (BottomCharacterLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (BottomCharacterLocationAlgo1)(VARIANT* rgnId,VARIANT* pSelection, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT*pMaskSize,VARIANT*pThredValue,VARIANT*pCharacterArea );
	STDMETHOD (BottomCharacterLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD (BottomCharacterLocationAlgo2)(VARIANT* rgnId, VARIANT* pAngleSelect,VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT*pCharNum,VARIANT*pMaskSize,VARIANT*pThredValue,VARIANT*pSelArea,VARIANT*pCharacterArea );
	STDMETHOD (BottomCharacterLocationAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD (BottomCharacterRegionAlgo)(VARIANT* rgnID,  VARIANT* pOuterEdge,VARIANT* pInnerEdge,VARIANT* pEroSize);
	STDMETHOD (BottomCharacterRegionAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (RegionLocationByModelTool)(VARIANT*rgnId,BSTR* bstrRgnFile);
	STDMETHOD (RegionLocationByModelToolHelp)(BSTR* pHelpStr);
	STDMETHOD (RegionLocationDiffByModelTool)(VARIANT*rgnId,VARIANT* pRadius,BSTR* bstrRgnFile);
	STDMETHOD (RegionLocationDiffByModelToolHelp)(BSTR* pHelpStr);
	STDMETHOD (RegionLocationByModelTool1)(VARIANT*rgnId,VARIANT* pRadius,VARIANT* pRoiWidth,BSTR* bstrRgnFile);
	STDMETHOD (RegionLocationByModelTool1Help)(BSTR* pHelpStr);

	STDMETHOD(ModelCreate2)(double Radius, int UsePolarity,HTuple * ModelID);
	STDMETHOD(CenterReLocationAlgo2)(VARIANT* rgnID, VARIANT* pUsePolarity,VARIANT* pRadius, VARIANT* pMinScore);
	STDMETHOD(CenterReLocationAlgo2Help)(BSTR* pHelpStr);

	//Special_Shaped_Plug
	STDMETHOD(SideCenterLocationAlgo)(VARIANT* rgnId, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pErosionSize);
	STDMETHOD(SideCenterLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SideCenterLocationAlgoForColor)(VARIANT* rgnId,VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pRowBgn,VARIANT* pRgnWidth,VARIANT* pRgnHeight,VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pErosionSize);
	STDMETHOD(SideCenterLocationAlgoForColorHelp)(BSTR* pHelpStr);

	STDMETHOD(NeckBaseEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pSearchSize, VARIANT* pHclipSize,VARIANT* pMaskSize, VARIANT* pThredLower, VARIANT* pThredUpper,VARIANT* pABEdgeArea);
	STDMETHOD(NeckBaseEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_SideTopRegAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower, VARIANT* pLineAngle);
	STDMETHOD(D13_SideTopRegAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_SideTopRegAngleDetectAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower, VARIANT* pLineAngle);
	STDMETHOD(D13_SideTopRegAngleDetectAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(D13_PlatformDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pVSearchSize,VARIANT* pHSearchSize,VARIANT* pThredLower,VARIANT* pAngle, VARIANT* pPlatformArea);
	STDMETHOD(D13_PlatformDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_PlatformDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pVSearchSize,VARIANT* pHSearchSize,VARIANT* pThredLower,VARIANT* pAngle, VARIANT* pPlatformArea);
	STDMETHOD(D13_PlatformDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D13_PlatformDetectAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pVSearchSize,VARIANT* pHSearchSize,VARIANT* pThredLower,VARIANT* pAngle, VARIANT* pPlatformArea);
	STDMETHOD(D13_PlatformDetectAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(D13_SlopEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pTopSlopEdgeArea);
	STDMETHOD(D13_SlopEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_SideSlopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(D13_SideSlopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	//角度定位时，顶部区域提取不准，包含部分斜坡区域时！
	STDMETHOD(D13_SideSlopRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(D13_SideSlopRegionLocationAlgo1Help)(BSTR* pHelpStr);
	//海华 双叉 无覆膜
	STDMETHOD(D13_SideSlopRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pBlackThred, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(D13_SideSlopRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(D13_SideTopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredValue, VARIANT* pClosingSize, VARIANT* pErosionSize);
	STDMETHOD(D13_SideTopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_SideTopRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredValue, VARIANT* pClosingSize, VARIANT* pErosionSize);
	STDMETHOD(D13_SideTopRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D13_SideTopRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pErosionSize);
	STDMETHOD(D13_SideTopRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(D13_TriangleDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pSearchSize,VARIANT* pWhiteThred,VARIANT* pBlackThred,VARIANT* pTriangleSize,VARIANT* pTopRegArea,VARIANT* pTriangleDist,VARIANT* pTriangleArea);
	STDMETHOD(D13_TriangleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_TriangleDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pSearchSize,VARIANT* pWhiteThred,VARIANT* pBlackThred,VARIANT* pTriangleSize,VARIANT* pTopRegArea,VARIANT* pTriangleDist,VARIANT* pTriangleArea);
	STDMETHOD(D13_TriangleDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D13_StripePatternDetectAlgo2)(VARIANT* rgnId,VARIANT* pSwitch, VARIANT* pMoveSize,VARIANT* pSearchSize,VARIANT* pWhiteThred,VARIANT* pStripeArea);
	STDMETHOD(D13_StripePatternDetectAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(D13_NeckRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pHErosionSize, VARIANT* pUpEdgeMoveSize,VARIANT* pMoveSize,VARIANT* pDilationSizeP,VARIANT* pDilationSizeT);
	STDMETHOD(D13_NeckRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_NeckRegionLocationAlgo2)(VARIANT* rgnId,VARIANT* pLowerEdgeMoveSize, VARIANT* pUpEdgeMoveSize,VARIANT* pHErosionSize, VARIANT* pMoveSize,VARIANT* pDilationSizeH1,VARIANT* pVDilationSizeH2,VARIANT* pHDilationSizeH2,VARIANT* pDilationSizeT);
	STDMETHOD(D13_NeckRegionLocationAlgo2Help)(BSTR* pHelpStr);


	STDMETHOD(SealingFaceRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pThredValue,VARIANT* pDilationSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(SealingFaceRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SealingFaceRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pThredValue,VARIANT* pDilationSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(SealingFaceRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(SealingFaceRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pSealDilationSize,VARIANT* pThredValue,VARIANT* pDilationSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(SealingFaceRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(ScrapEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pSearchSize,VARIANT* pScrapEdgeArea);
	STDMETHOD(ScrapEdgeDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ScrapEdgeDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pSearchSize,VARIANT* pScrapEdgeArea);
	STDMETHOD(ScrapEdgeDetectAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(ScrapEdgeDetectAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pSearchSize,VARIANT* pScrapEdgeArea);
	STDMETHOD(ScrapEdgeDetectAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(CrownRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelection,VARIANT* pDilationSize, VARIANT* pmoveSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(CrownRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(CrownRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pSelection,VARIANT* pDilationSize, VARIANT* pmoveSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(CrownRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D13_TopAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThreshUpValue,VARIANT* pBlackThresh,VARIANT* pGapArea);
	STDMETHOD(D13_TopAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_TopAngleDetectAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThreshUpValue,VARIANT* pBlackThresh,VARIANT* pGapArea);
	STDMETHOD(D13_TopAngleDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D13_TopAngleDetectAlgo11)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThreshUpValue,VARIANT* pBlackThresh,VARIANT* pThreshLow,VARIANT* pGapArea);
	STDMETHOD(D13_TopAngleDetectAlgo11Help)(BSTR* pHelpStr);

	STDMETHOD(D13_TopAngleDetectAlgo2)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pBlackThresh, VARIANT* pThreshUpValue,VARIANT* pGapArea);
	STDMETHOD(D13_TopAngleDetectAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(D13_TopCenterDetectAlgo)(VARIANT* rgnId,  VARIANT* pBlackThresh,VARIANT* pGapArea);
	STDMETHOD(D13_TopCenterDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(BottomDotDetectAlgo)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pCirleNum,VARIANT* pMinScore,VARIANT* pAnglLineArea);
	STDMETHOD(BottomDotDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(BottomDotRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadiusE,VARIANT* pRoiWidth,VARIANT* pRadius,VARIANT* pCirleNum,VARIANT* pMinScore,VARIANT* pDilationSize);
	STDMETHOD(BottomDotRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(BottomDotRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadiusE,VARIANT* pRoiWidth,VARIANT* pRadius,VARIANT* pCirleNum,VARIANT* pMinScore,VARIANT* pDilationSize);
	STDMETHOD(BottomDotRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(BottomDotRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadiusE,VARIANT* pRoiWidth,VARIANT* pRadius,VARIANT* pCirleNum,VARIANT* pMinScore,VARIANT* pDilationSize);
	STDMETHOD(BottomDotRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(BottomDotRegionLocationAlgo3)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadiusE,VARIANT* pRoiWidth,VARIANT* pRadius,VARIANT* pCirleNum,VARIANT* pMinScore,VARIANT* pDilationSize);
	STDMETHOD(BottomDotRegionLocationAlgo3Help)(BSTR* pHelpStr);

	STDMETHOD(AngleSelectAlgo)(VARIANT* rgnId,VARIANT* pSelection );
	STDMETHOD(AngleSelectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(AB20_BottomDotRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadiusE,VARIANT* pRoiWidth,VARIANT* pRadius,VARIANT* pAngleDiff,VARIANT* pRadiusDiff,VARIANT* pDilationSize);
	STDMETHOD(AB20_BottomDotRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_WholeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pDilationSizeP,VARIANT* pDilationSizeT,VARIANT* pOpeningSize,VARIANT* pErosionSize);
	STDMETHOD(D13_WholeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_TriangleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(D13_TriangleRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_TopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(D13_TopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_TopRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(D13_TopRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D13_TopRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(D13_TopRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(D13_TopRubberRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pInnerEdge,VARIANT* pOutEdge, VARIANT*pAreaLimitLower);
	STDMETHOD(D13_TopRubberRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_TopRubberRegionLocationAlgo2)(VARIANT* rgnId,VARIANT* pInnerEdge,VARIANT* pOutEdge, VARIANT*pAreaLimitLower);
	STDMETHOD(D13_TopRubberRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(D13_InWallRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pGrayThred,VARIANT* pPos,VARIANT* pErosionSize);
	STDMETHOD(D13_InWallRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D13_InWallRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pGrayThred,VARIANT* pBlackThred,VARIANT* pErosionSize);
	STDMETHOD(D13_InWallRegionLocationAlgo2Help)(BSTR* pHelpStr);

	//AB20
	STDMETHOD(AB20_TopRegDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower, VARIANT* pTopRegionArea);
	STDMETHOD(AB20_TopRegDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB20_TopRegDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThreshUpValue, VARIANT* pTopRegionArea);
	STDMETHOD(AB20_TopRegDetectAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(AB20_TopRegDetectAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThreshUpValue, VARIANT* pTopRegionArea);
	STDMETHOD(AB20_TopRegDetectAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(AB20_SlopEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pTopSlopEdgeArea);
	STDMETHOD(AB20_SlopEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(AB20_FilmOrNotDetectAlgo)( VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(AB20_FilmOrNotDetectAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD(AB20_FilmOrNotDetectAlgo1)( VARIANT* rgnId, VARIANT* pWhiteMask, VARIANT* pDynThresh,VARIANT* pAreaSize, VARIANT* pDetectArea);
	STDMETHOD(AB20_FilmOrNotDetectAlgo1Help)( BSTR* pHelpStr);

	STDMETHOD(AB20_SideSlopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB20_SideSlopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB20_SideSlopRegionLocationAlgo1)(VARIANT* rgnId, VARIANT*pDilationSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB20_SideSlopRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(AB20_SideSlopRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pEllipseMoveSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB20_SideSlopRegionLocationAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD(AB20_SideSlopRegionLocationAlgo3)(VARIANT* rgnId, VARIANT* pEllipseMoveSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB20_SideSlopRegionLocationAlgo3Help)(BSTR* pHelpStr);

	STDMETHOD(AB20_NeckRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB20_NeckRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB20_NeckRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB20_NeckRegionLocationAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD(AB32_NeckRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB32_NeckRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB32_NeckRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pUpSize,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB32_NeckRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(AB20_NeckRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize1);
	STDMETHOD(AB20_NeckRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(AB20_NeckRegionLocationAlgo3)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pUpSize,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize1);
	STDMETHOD(AB20_NeckRegionLocationAlgo3Help)(BSTR* pHelpStr);

	STDMETHOD(AB20_WholeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pABHErosionSize, VARIANT* pABVErosionSize,VARIANT* pErosionSize);
	STDMETHOD(AB20_WholeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(AB20_TopInwallRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pOpeningSize,VARIANT* pClosingSize,VARIANT* pErosionSize);
	STDMETHOD(AB20_TopInwallRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB32_TopInwallRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pUpEdgePos,VARIANT* pDownEdgePos,VARIANT* pErosionSize);
	STDMETHOD(AB32_TopInwallRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB32_TopHighLightRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pOuterEdge,VARIANT* pInnerEdge,VARIANT* pBottomSize);
	STDMETHOD(AB32_TopHighLightRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB32_TopHighLightRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pOuterEdge,VARIANT* pInnerEdge,VARIANT* pBottomSize,VARIANT* pUpPose,VARIANT* pDownPose);
	STDMETHOD(AB32_TopHighLightRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(SideTopHighLightRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pOuterEdge,VARIANT* pMoveSize);
	STDMETHOD(SideTopHighLightRegionLocationAlgoHelp)(BSTR* pHelpStr);

	//D20
	STDMETHOD(D20_SideTopRegAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower, VARIANT* pAreaLimitLower,VARIANT* pLineAngle);
	STDMETHOD(D20_SideTopRegAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_SideTopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pErosionSize);
	STDMETHOD(D20_SideTopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SideTopRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pThredValue,VARIANT* pAreaLimitLower,VARIANT* pErosionSize);
	STDMETHOD(SideTopRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(SideTopEdgeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pThredValue,VARIANT* pDilationSize,VARIANT* pAreaLimitLower,VARIANT* pErosionSize);
	STDMETHOD(SideTopEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_PlatformDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pVSearchSize,VARIANT* pHSearchSize,VARIANT* pThredLower,VARIANT* pAngle,VARIANT* pPlatformArea);
	STDMETHOD(D20_PlatformDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_SlopEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pTopSlopEdgeArea);
	STDMETHOD(D20_SlopEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_SlopEdgeDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pThreshold,VARIANT* pTopSlopEdgeArea);
	STDMETHOD(D20_SlopEdgeDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D20_SideSlopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pBlackThred, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(D20_SideSlopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_SideSlopRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pBlackThred, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(D20_SideSlopRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D20_StripePatternDetectAlgo)(VARIANT* rgnId,VARIANT* pSwitch, VARIANT* pMoveSize,VARIANT* pSearchSize,VARIANT* pWhiteThred,VARIANT* pStripeArea);
	STDMETHOD(D20_StripePatternDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20_StripePatternDetectAlgo1)(VARIANT* rgnId,VARIANT* pSwitch, VARIANT* pMoveSize,VARIANT* pSearchSize,VARIANT* pWhiteThred,VARIANT* pDilationSize,VARIANT* pStripeArea);
	STDMETHOD(D20_StripePatternDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D20_NeckRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSizeH1,VARIANT* pDilationSizeH2,VARIANT* pDilationSizeT);
	STDMETHOD(D20_NeckRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_NeckRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pLowerEdgeMoveSize, VARIANT* pUpEdgeMoveSize,VARIANT* pHErosionSize, VARIANT* pMoveSize,VARIANT* pDilationSizeH1,VARIANT* pVDilationSizeH2,VARIANT* pHDilationSizeH2,VARIANT* pDilationSizeT);
	STDMETHOD(D20_NeckRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D20_TopAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pThreshUpValue,VARIANT* pWhiteThresh,VARIANT*pAreaLimitLower,VARIANT* pGapArea);
	STDMETHOD(D20_TopAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_TopAngleDetectAlgo1)(VARIANT* rgnId, VARIANT* pRadius,  VARIANT* pThreshLowerValue, VARIANT* pAreaLimit,VARIANT*pAreaLimitLower,VARIANT* pGapArea);
	STDMETHOD(D20_TopAngleDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(D20_TopCenterDetectAlgo)(VARIANT* rgnId,  VARIANT* pThreshUpValue,VARIANT* pGapArea);
	STDMETHOD(D20_TopCenterDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_TopRubberRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pInnerEdge,VARIANT* pOutEdge, VARIANT*pAreaLimitLower);
	STDMETHOD(D20_TopRubberRegionLocationAlgoHelp)(BSTR* pHelpStr);

	// F20 四叉胶塞（顶部角度定位、叉口平台提取）
	STDMETHOD(F20_SideTopRegAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower,VARIANT* pTopEllipseMoveSize, VARIANT* pAreaLimitLower,VARIANT* pLineAngle);
	STDMETHOD(F20_SideTopRegAngleDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(F20_PlatformDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pVSearchSize,VARIANT* pHSearchSize,VARIANT* pThredLower,VARIANT* pAngle,VARIANT* pPlatformArea);
	STDMETHOD(F20_PlatformDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(F20_SideTopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRgnSelection,VARIANT* pEroSize);
	STDMETHOD(F20_SideTopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(F20_OutInWallRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pUpSizeOuterWall,VARIANT* pDownSizeOuterWall,VARIANT* pAreaSize,VARIANT* pWholeAreaSize,VARIANT* pHPlatEroSize,VARIANT* pHEroSize);
	STDMETHOD(F20_OutInWallRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(F20_TopAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pThreshLowerValue, VARIANT* pOuterEdge,VARIANT* pInnerEdge,VARIANT*pAreaLimitLower,VARIANT* pGapArea);
	STDMETHOD(F20_TopAngleDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(F20_TopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius,VARIANT* pRoiWidth, VARIANT* pThredLower,VARIANT* pBlackThred,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(F20_TopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(F20_StripePatternDetectAlgo)(VARIANT* rgnId,VARIANT* pSwitch, VARIANT* pMoveSize,VARIANT* pHSearchSize,VARIANT* pVSearchSize,VARIANT* pWhiteThred,VARIANT* pDilationSize,VARIANT* pStripeArea);
	STDMETHOD(F20_StripePatternDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20N_TopAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pRadius,  VARIANT* pThreshLowerValue, VARIANT* pGapArea);
	STDMETHOD(D20N_TopAngleDetectAlgoHelp)(BSTR* pHelpStr);

	// D20N 新三叉叉胶塞（顶部角度定位、叉口平台提取）
	STDMETHOD(D20N_SideTopRegAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower, VARIANT* pAreaLimitLower,VARIANT* pLineAngle);
	STDMETHOD(D20N_SideTopRegAngleDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20N_TrianglePatternDetectAlgo)(VARIANT* rgnId,VARIANT* pWhiteBarSize,VARIANT* pTriangleSize,VARIANT* pStripeArea);
	STDMETHOD(D20N_TrianglePatternDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20N_StripePatternDetectAlgo)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHSearchSize,VARIANT* pVSearchSize,VARIANT* pWhiteThred,VARIANT* pDilationSize,VARIANT* pStripeArea);
	STDMETHOD(D20N_StripePatternDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20N_NeckRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pLowerEdgeMoveSize, VARIANT* pUpEdgeMoveSize,VARIANT* pHErosionSize,VARIANT* pDilationSizeH1,VARIANT* pDilationSizeT);
	STDMETHOD(D20N_NeckRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20N_OutInWallRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pOuterSize, VARIANT* pUpSizeOuterWall,VARIANT* pDownSizeOuterWall,VARIANT* pAreaSize,VARIANT* pWholeAreaSize,VARIANT* pHPlatEroSize,VARIANT* pHEroSize);
	STDMETHOD(D20N_OutInWallRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20N_StripePatternRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelect, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(D20N_StripePatternRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20N_SideSlopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pVEroSizeDown,VARIANT* pOuterEdgeSize,VARIANT* pBlackThred, VARIANT* pHErosionSize);
	STDMETHOD(D20N_SideSlopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20N_TopForkRegionLocationIniAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pTopDilSize1,VARIANT* pTopDilSize2);	
	STDMETHOD(D20N_TopForkRegionLocationIniAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20N_TopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection, VARIANT* pRadius,VARIANT* pTopDilSize,VARIANT* pTopForkSize,VARIANT* pSlopDilSize,VARIANT* pAreaSize,VARIANT* pScaleSize);	
	STDMETHOD(D20N_TopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(EccentricityDetectAlgo)(VARIANT* rgnId, VARIANT* pDistDiff);
	STDMETHOD(EccentricityDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (CrownCenterRelocationAlgo)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pMaskSize,VARIANT* pThreshLowValue,VARIANT* pThreshUpValue,VARIANT* pAreaLimitSize);
	STDMETHOD (CrownCenterRelocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD (CrownCenterRelocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pMaskSize,VARIANT* pThreshLowValue,VARIANT* pAreaLimitSize);
	STDMETHOD (CrownCenterRelocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(SidePlatformRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pClosingSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(SidePlatformRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SidePlatformRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pABEdgeMov,VARIANT* pThredLow,VARIANT* pClosingSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(SidePlatformRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(SidePlatformRegionLocationAlgo2)(VARIANT* rgnId,VARIANT* pUpEdgeSize,VARIANT* pABEdgeMov,VARIANT* pThredLow,VARIANT* pClosingSize,VARIANT* pAreaSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(SidePlatformRegionLocationAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD(SidePlatformTopEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pLengthSel,VARIANT* pOuterSize,VARIANT* pMoveSize,VARIANT* pHClipSize, VARIANT* pVDilSize);
	STDMETHOD(SidePlatformTopEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(N_DynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(N_DynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(InWallDynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(InWallDynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(InWallDynThresholdAlgo1)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pUpEdgeExclude,VARIANT* pDownEdgeExclude,VARIANT* pDetectArea);
	STDMETHOD(InWallDynThresholdAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(SideNeckBorderRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelect,VARIANT* pInnerBorderSize,VARIANT* pOuterBorderSize, VARIANT* pUpBoderSize,VARIANT* pDownBoderSize,VARIANT* pStripeScale,VARIANT* pPlatformSize,VARIANT* pAreaSize);
	STDMETHOD(SideNeckBorderRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20_SideInwallRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pBaseLineMoveSize,VARIANT* pROIScaleSize, VARIANT* pTopRgnScaleSize,VARIANT* pSlopScaleSize,VARIANT* pPlatformSize,VARIANT* pInnerEdgeSize,VARIANT* pOuterEndgeSize,VARIANT* pLeftUpEdgeSize,VARIANT* pAreaSize);
	STDMETHOD(D20_SideInwallRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_WholeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pErosionSize,VARIANT* pMoveSize,VARIANT* pHErosionSize, VARIANT* pDilationSize,VARIANT* pUpEdgeMoveSize,VARIANT* pABHErosionSize, VARIANT* pABVErosionSize);
	STDMETHOD(D20_WholeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_StripePatternRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(D20_StripePatternRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_TopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(D20_TopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(D20_TopForkRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge,VARIANT* pAreaLimitUp);
	STDMETHOD(D20_TopForkRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(D20_RubberDetectAlgo)(VARIANT* rgnId, VARIANT* pMaskSize,VARIANT* pSmoothSize, VARIANT* pDefectType, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pOpeningSize, VARIANT* pDilationSize,VARIANT* pDetectArea);
	STDMETHOD(D20_RubberDetectAlgoHelp)(BSTR* pHelpStr);

	//G20_
	STDMETHOD(NeckBaseEdgeDetectAlgo1)(VARIANT* rgnId, VARIANT* pSearchSize, VARIANT* pHclipSize,VARIANT* pThreshold, VARIANT* pABEdgeArea);
	STDMETHOD(NeckBaseEdgeDetectAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(NeckBaseEdgeDetectAlgo3)(VARIANT* rgnId, VARIANT* pSearchSize, VARIANT* pHclipSize,VARIANT* pThreshold, VARIANT* pABEdgeArea);
	STDMETHOD(NeckBaseEdgeDetectAlgo3Help)(BSTR* pHelpStr);
	STDMETHOD(NeckBaseEdgeDetectFreeModelAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pSearchSize, VARIANT* pSearchDirec,VARIANT* pSelect,VARIANT* pHclipSize,VARIANT* pThreshold, VARIANT* pABEdgeArea);
	STDMETHOD(NeckBaseEdgeDetectFreeModelAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_SideTopRegAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower, VARIANT* pLineAngle);
	STDMETHOD(G20_SideTopRegAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_PlatformDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pVSearchSize,VARIANT* pHSearchSize,VARIANT* pThredLower, VARIANT* pPlatformArea);
	STDMETHOD(G20_PlatformDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_SlopEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pTopSlopEdgeArea);
	STDMETHOD(G20_SlopEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_InWallRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pGrayValue,VARIANT* pDilationSizeU,VARIANT* pDilationSizeD,VARIANT* pErosionSize);
	STDMETHOD(G20_InWallRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_InWallRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pGrayValue,VARIANT* pDilationSizeU,VARIANT* pDilationSizeD,VARIANT* pErosionSize,VARIANT* pForkSize);
	STDMETHOD(G20_InWallRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(G20_InWallRegionLocationAlgo2)(VARIANT* rgnId,  VARIANT* pAngleSelect,VARIANT* pGrayValue,VARIANT* pDilationSizeU,VARIANT* pDilationSizeD,VARIANT* pErosionSize,VARIANT* pForkSize,VARIANT* pForkWallSize);
	STDMETHOD(G20_InWallRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(G20_MiddleEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pEdgeArea);
	STDMETHOD(G20_MiddleEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_DownEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pValueThred, VARIANT* pEdgeArea);
	STDMETHOD(G20_DownEdgeDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_DownEdgeDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pValueThred, VARIANT* pEdgeArea);
	STDMETHOD(G20_DownEdgeDetectAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(G20_DownEdgeDetectAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pValueThred, VARIANT* pEdgeArea);
	STDMETHOD(G20_DownEdgeDetectAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(G20_ConPatternDetectAlgo)(VARIANT* rgnId,VARIANT* pAreaLimit,VARIANT* pGrayValue,VARIANT* pOpeningSize,VARIANT* pDilationSizeH,VARIANT* pDialtionSizeV,VARIANT* pErosionSizeH,VARIANT* pErosionSizeV,VARIANT* pStripeArea);
	STDMETHOD(G20_ConPatternDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_NeckRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize0,VARIANT* pDilationSize1,VARIANT* pSwitch,VARIANT* pMoveSizeH,VARIANT* pAreaThred,VARIANT* pDilationSize2);
	STDMETHOD(G20_NeckRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_NeckRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize0,VARIANT* pDilationSize1,VARIANT* pSwitch,VARIANT* pMoveSizeH,VARIANT* pAreaThred,VARIANT* pDilationSize2);
	STDMETHOD(G20_NeckRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(G20_ABEdgeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(G20_ABEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(ABEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(ABEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(ABEdgeRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(ABEdgeRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(ABEdgeRegionLocationAlgo2)(VARIANT* rgnId,VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pMoveDiff, VARIANT* pIntsctThred,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(ABEdgeRegionLocationAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD(ABEdgeRegionLocationAlgo3)(VARIANT* rgnId,VARIANT* pSelection,VARIANT* pGrayThred,VARIANT* pMoveSize, VARIANT* pMoveDiff, VARIANT* pIntsctThred,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(ABEdgeRegionLocationAlgo3Help)(BSTR* pHelpStr);
	STDMETHOD(SealingEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(SealingEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_WholeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pUpEdgeMoveSize,VARIANT* pDialtionSizeP,VARIANT* pABHErosionSize, VARIANT* pABVErosionSize, VARIANT* pErosionSize);
	STDMETHOD(G20_WholeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_TopAngleDetectAlgo)(VARIANT* rgnId,VARIANT* pThreshValue,VARIANT* pGapAngle);
	STDMETHOD(G20_TopAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_TopSealingFaceEdgeDetectAlgo)(VARIANT* rgnId,VARIANT* pThresh,VARIANT* pOpeningSize,VARIANT* pGapArea);
	STDMETHOD(G20_TopSealingFaceEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_TopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(G20_TopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_TopCornerLocationAlgo)(VARIANT* rgnId, VARIANT* pSlopeSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(G20_TopCornerLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_TopHLightLocationAlgo)(VARIANT* rgnId,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge);
	STDMETHOD(G20_TopHLightLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_TopRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(G20_TopRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(G20_TopRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(G20_TopRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(G20_ConPatternCoordAlgo)(VARIANT* rgnId, VARIANT* pEllipseMoveSize,VARIANT* pEdgeMoveSize,VARIANT* pOpeningSize,VARIANT* pSearchSize,VARIANT* pGrayValue, VARIANT* pStripeArea);
	STDMETHOD(G20_ConPatternCoordAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_ConPatternSearchRegAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pHDilationSize, VARIANT* pSearchSize,VARIANT* pClipSizeD,VARIANT* pClipSizeU);
	STDMETHOD(G20_ConPatternSearchRegAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_ConPatternRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pHErosionSize1, VARIANT* pHErosionSize2, VARIANT* pVErosionSize);
	STDMETHOD(G20_ConPatternRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(NeckRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pUpEdgeMoveSize,VARIANT* pLowerEdgeMoveSize,VARIANT* pHErosionSize);
	STDMETHOD(NeckRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(TopSealingFaceRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pGrayThred,VARIANT* pClosingSize,VARIANT* pRadiusDiff,VARIANT* pErosionSize);
	STDMETHOD(TopSealingFaceRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(TopSealingFaceRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pErosionSize);
	STDMETHOD(TopSealingFaceRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(TopSealingFaceRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pThresh, VARIANT* pErosionSize);
	STDMETHOD(TopSealingFaceRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(TopSealingFaceRegionLocationAlgo3)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pErosionSize);
	STDMETHOD(TopSealingFaceRegionLocationAlgo3Help)(BSTR* pHelpStr);

	STDMETHOD(ScrapDiameterMeasureAlgo)(VARIANT* rgnId, VARIANT* pSigma,VARIANT* pThredLower, VARIANT* pThredUpper,VARIANT* pCircularityValue,VARIANT* pDiameter);
	STDMETHOD(ScrapDiameterMeasureAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(WarpedMembraneDetectAlgo)(VARIANT* rgnId, VARIANT* pSensitivity,VARIANT* pDetectArea);
	STDMETHOD(WarpedMembraneDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WarpedMembraneDetectAlgo1)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pSensitivity,VARIANT* pGrayLowThred,VARIANT* pDetectArea);
	STDMETHOD(WarpedMembraneDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(G20_DownEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHErosionSize,VARIANT* pDialtionSizeV);
	STDMETHOD(G20_DownEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_DownEdgeRegionLocationAlgo2)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHErosionSize,VARIANT* pDialtionSizeV);
	STDMETHOD(G20_DownEdgeRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(G20_DownEdgeRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHErosionSize,VARIANT* pDialtionSizeV,VARIANT* pDilationSizeP);
	STDMETHOD(G20_DownEdgeRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(G20_SideSlopRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pLowerEdgeMoveSize, VARIANT* pUpEdgeMoveSize, VARIANT* pForkEdge,VARIANT* pHErosionSize);
	STDMETHOD(G20_SideSlopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SlotDetectAlgo)(VARIANT* rgnId, VARIANT* pNumPart, VARIANT* pThresholdValue, VARIANT* pGrayValue,VARIANT* pMeanGrayValue,VARIANT* pDetectArea);
	STDMETHOD(SlotDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_OutEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pDilationSize1,VARIANT* pDilationSize2);
	STDMETHOD(G20_OutEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_ConPatternDynThresholdAlgo)(VARIANT* rgnId,  VARIANT* pAreaLimit,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(G20_ConPatternDynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_ConPatternDynThresholdAlgo1)(VARIANT* rgnId,  VARIANT* pAreaLimit,VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(G20_ConPatternDynThresholdAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(G20_TopSideLowRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pInnerEdge,VARIANT* pOutEdge,VARIANT* pDilationSize);
	STDMETHOD(G20_TopSideLowRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_TopForkEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pInnerEdge,VARIANT* pOutEdge,VARIANT* pDilationSize,VARIANT* pRelativeWidth,VARIANT* pRelativeLength);
	STDMETHOD(G20_TopForkEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(DoubleFoldDetectAlgo)(VARIANT* rgnId,VARIANT* pAreaLimit, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pMoveSize,VARIANT* pDetectArea);
	STDMETHOD(DoubleFoldDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DoubleFoldDetectAlgo1)(VARIANT* rgnId,VARIANT* pAreaLimit, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pMoveSize,VARIANT* pDetectArea);
	STDMETHOD(DoubleFoldDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(G20_MiddleEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHErosionSize,VARIANT* pDialtionSizeV);
	STDMETHOD(G20_MiddleEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_MiddleEdgeRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHErosionSize,VARIANT* pDialtionSizeV,VARIANT*pDilationSizeP);
	STDMETHOD(G20_MiddleEdgeRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(FilmHairDetectAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(FilmHairDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_ConPatternDetectProfileAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pClipSize,VARIANT* pAreaLimit,VARIANT* pDetectArea);
	STDMETHOD(G20_ConPatternDetectProfileAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_TopDefectDetectAlgo)(VARIANT* rgnId, VARIANT* pErosionSize,VARIANT* pCErosionSize,VARIANT* pRectSegmntNum,VARIANT*  pThreshAdjust,VARIANT* pDefectAreaThresh, VARIANT* pMeanGrayDiffThresh, VARIANT* pMeanGrayNeighborDiff,VARIANT* pDefectArea);
	STDMETHOD(T20_TopDefectDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(AB20_CrownCircleRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pRadius, VARIANT* pRadius1, VARIANT* pRoiWidth1, VARIANT* pRadius2, VARIANT* pRoiWidth2);
	STDMETHOD(AB20_CrownCircleRegionLocationAlgoHelp)(BSTR* pHelpStr);

	//T20_
	STDMETHOD(T20_SideTopRegAngleDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower, VARIANT* pAreaLimitLower,VARIANT* pLineAngle);
	STDMETHOD(T20_SideTopRegAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_PlatformDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pVSearchSize,VARIANT* pHSearchSize,VARIANT* pThredLower,VARIANT* pOpeningSize,VARIANT* pAngle,VARIANT* pPlatformArea);
	STDMETHOD(T20_PlatformDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_SideConeBiasDetectAlgo)(VARIANT* rgnId, VARIANT* pConeDeepth, VARIANT* pConeArea);
	STDMETHOD(T20_SideConeBiasDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_SlopEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pThreshold,VARIANT* pTopSlopEdgeArea);
	STDMETHOD(T20_SlopEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_ConPatternCoordAlgo)(VARIANT* rgnId, VARIANT* pEllipseMoveSize,VARIANT* pEdgeMoveSize,VARIANT* pSearchSize,VARIANT* pGrayValue,VARIANT* pWidthRadiusSet,VARIANT* pAngleT1,VARIANT* pAngleT2, VARIANT* pStripeArea);
	STDMETHOD(T20_ConPatternCoordAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_ConPatternSearchRegUpAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pHDilationSize,VARIANT* pSearchSize,VARIANT*pClipSize);
	STDMETHOD(T20_ConPatternSearchRegUpAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_CirclePatternDetectAlgo)(VARIANT* rgnId,VARIANT* pMaskSize,VARIANT* pSeriousBlackPointDynThresh,VARIANT* pSeriousWhitePointDynThresh,VARIANT* pThredValue,VARIANT* pStripeArea);
	STDMETHOD(T20_CirclePatternDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_CirclePatternDetectAlgo1)(VARIANT* rgnId,VARIANT* pMaskSize,VARIANT* pSeriousBlackPointDynThresh,VARIANT* pSeriousWhitePointDynThresh,VARIANT* pThredValue,VARIANT* pStripeArea);
	STDMETHOD(T20_CirclePatternDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(T20_ConPatternSearchRegDownAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pSearchSize);
	STDMETHOD(T20_ConPatternSearchRegDownAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_TrianglePatternDetectAlgo)(VARIANT* rgnId,VARIANT* pGrayValue,VARIANT* pTriangleSize,VARIANT* pStripeArea);
	STDMETHOD(T20_TrianglePatternDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_StripePatternDetectAlgo)(VARIANT* rgnId,VARIANT* pGrayValue,VARIANT* pStripeArea);
	STDMETHOD(T20_StripePatternDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_MiddleEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pEdgeArea);
	STDMETHOD(T20_MiddleEdgeDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_NeckRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize0,VARIANT* pDilationSize1,VARIANT* pDilationSizeH1,VARIANT* pDilationSizeH2);
	STDMETHOD(T20_NeckRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(T20_NeckRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize0,VARIANT* pDilationSize1,VARIANT* pDilationSizeH1,VARIANT* pDilationSizeH2);
	STDMETHOD(T20_NeckRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(T20_NeckRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize0,VARIANT* pDilationSize1,VARIANT* pDilationSizeH1,VARIANT* pDilationSizeH2,VARIANT*pAreaThred);
	STDMETHOD(T20_NeckRegionLocationAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD(T20_NeckRegionLocationAlgo3)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize0,VARIANT* pDilationSize1,VARIANT* pDilationSizeH1,VARIANT* pDilationSizeH2,VARIANT*pAreaThred);
	STDMETHOD(T20_NeckRegionLocationAlgo3Help)(BSTR* pHelpStr);

	STDMETHOD(T20_NeckCornerRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pNeckErosionSize,VARIANT* pDilSizePlatform,VARIANT* pDilSizePlatform1,VARIANT* pDilSizeTriangle,VARIANT* pUpRegionPos, VARIANT* pDownRegionPos,VARIANT*pOpeningSize);
	STDMETHOD(T20_NeckCornerRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_InWallRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pPos,VARIANT* pGrayThred,VARIANT* pAreaLimitLower,VARIANT* pErosionSize);
	STDMETHOD(T20_InWallRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(SideTopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pErosionSize);
	STDMETHOD(SideTopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WholeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pABHErosionSize, VARIANT* pABVErosionSize,VARIANT* pDilationSize,VARIANT* pErosionSize);
	STDMETHOD(WholeRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(TopCenterDetectAlgo)(VARIANT* rgnId,  VARIANT* pThreshUpValue,VARIANT* pGapArea);
	STDMETHOD(TopCenterDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_TopAngleDetectAlgo)(VARIANT* rgnId,VARIANT* pThreshValue,VARIANT* pGapAngle);
	STDMETHOD(T20_TopAngleDetectAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_TopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pRadius, VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pErosionSize,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(T20_TopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(T20_TopForkRegionLocationAlgo)(VARIANT* rgnId,  VARIANT* pRadius,VARIANT*pRoiWidth,VARIANT* pThredLower,VARIANT*pAreaLimitLower,VARIANT* pThreshUpValue,VARIANT* pForkEdge,VARIANT* pInnerEdge,VARIANT* pOutEdge);
	STDMETHOD(T20_TopForkRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_StripePatternDetectAlgo1)(VARIANT* rgnId,VARIANT* pGrayValue,VARIANT* pDilationSize,VARIANT* pStripeArea);
	STDMETHOD(T20_StripePatternDetectAlgo1Help)(BSTR* pHelpStr);							

	STDMETHOD(T20_SideSlopRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(T20_SideSlopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_WholeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pABHErosionSize, VARIANT* pABVErosionSize,VARIANT*pDilationSize,VARIANT*pDilationSize0,VARIANT* pErosionSize);
	STDMETHOD(T20_WholeRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(T20_TopCircleLocationAlgo)(VARIANT* rgnId, VARIANT* pCircleOuter, VARIANT* pCircleWidth,VARIANT*pSmalCircleR,VARIANT*pAngleAdjust,VARIANT* pScaleSize);
	STDMETHOD(T20_TopCircleLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(T20_TopRubberRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pInnerEdge,VARIANT* pOutEdge, VARIANT*pAreaLimitLower);
	STDMETHOD(T20_TopRubberRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(T20_StripePatternRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(T20_StripePatternRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(T20_StripePatternRegionLocationAlgo1)(VARIANT* rgnId, VARIANT*Selection,VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT*pDilationSize);
	STDMETHOD(T20_StripePatternRegionLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(T20_StripePatternRegionLocationAlgo2)(VARIANT* rgnId, VARIANT*Selection,VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT*pDilationSize,VARIANT*pCircleUpRgnPos,VARIANT* pTriangleDownRgnPos);
	STDMETHOD(T20_StripePatternRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(AB20_DownEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pValueThred, VARIANT* pEdgeArea);
	STDMETHOD(AB20_DownEdgeDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB20_DownEdgeDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pValueThred, VARIANT* pEdgeArea);
	STDMETHOD(AB20_DownEdgeDetectAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(AB32_DownEdgeDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pThresholdDown,VARIANT* pValueThred, VARIANT* pEdgeArea);
	STDMETHOD(AB32_DownEdgeDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB32_DownEdgeDetectAlgo1)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pThresholdDown,VARIANT* pSamePointsThred,VARIANT* pValueThred, VARIANT* pEdgeArea);
	STDMETHOD(AB32_DownEdgeDetectAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(AB32_DownEdgeDetectAlgo2)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pSearchSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pThresholdDown,VARIANT* pSamePointsThred,VARIANT* pValueThred, VARIANT* pEdgeArea);
	STDMETHOD(AB32_DownEdgeDetectAlgo2Help)(BSTR* pHelpStr);
	STDMETHOD(AB20_DownEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHErosionSize,VARIANT* pDialtionSizeV);
	STDMETHOD(AB20_DownEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_EmbossCoordAlgo)(VARIANT* rgnId, VARIANT* pEllipseMoveSize,VARIANT* pEdgeMoveSize,VARIANT* pOpeningSize,VARIANT* pSearchSize,VARIANT* pGrayValue,VARIANT* pWidthRadiusSet,VARIANT* pAngleT1,VARIANT* pStripeArea);
	STDMETHOD(G20_EmbossCoordAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(G20_EmbossSearchRegAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pHDilationSize,VARIANT* pSearchSize,VARIANT*pClipSize);
	STDMETHOD(G20_EmbossSearchRegAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(G20_NeckRegionLocationAlgo2)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pMoveSize, VARIANT* pHErosionSize, VARIANT* pVErosionSize,VARIANT* pDilationSize0,VARIANT* pDilationSize1,VARIANT* pSwitch,VARIANT* pMoveSizeH,VARIANT*pDilationSizeH,VARIANT* pAreaThred,VARIANT* pDilationSize2);
	STDMETHOD(G20_NeckRegionLocationAlgo2Help)(BSTR* pHelpStr);

	STDMETHOD(DynThresholdForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize, VARIANT* pDetectArea);
	STDMETHOD(DynThresholdForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DynThresholdForColorFilmAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pWhiteMaskSize, VARIANT* pSeriousWhitePointDynThresh, VARIANT* pSeriousWhitePointSize,VARIANT* pMoveSize,VARIANT* pDetectArea);
	STDMETHOD(DynThresholdForColorFilmAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(EdgeForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pEdgeThred, VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD(EdgeForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgo)( VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect, VARIANT* pRotaAngle, VARIANT* pDynThresh,VARIANT* pDefectSize, VARIANT* pDetectArea);
	STDMETHOD (AnnularRotaDynThresholdForColorAlgoHelp)( BSTR* pHelpStr);
	STDMETHOD(CurlNoCompoundForColorAlgo)(VARIANT* rgnId, VARIANT* pColorTransType,VARIANT* pChannelSelect,VARIANT* pGrayValueMean);
	STDMETHOD(CurlNoCompoundForColorAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AnnularRotaDynThresholdAlgoBW)(VARIANT* rgnId, VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea);
	STDMETHOD(AnnularRotaDynThresholdAlgoBWHelp)(BSTR* pHelpStr);
	STDMETHOD(AnnularRotaDynThresholdForColorAlgoBW)(VARIANT* rgnId, VARIANT* pColorTransType, VARIANT* pChannelSelect,VARIANT* pRotaAngle, VARIANT* pBlackThredLow, VARIANT* pBlackDefectSize, VARIANT* pWhiteThredLow, VARIANT* pWhiteDefectSize,VARIANT* pDetectArea);
	STDMETHOD(AnnularRotaDynThresholdForColorAlgoBWHelp)(BSTR* pHelpStr);

	//胶垫切边测试
	STDMETHOD(RubberMatScrapEdgeDetectAlgo)(VARIANT* rgnId,VARIANT*pThredValue,VARIANT*pGrayValue, VARIANT* pMoveSize1, VARIANT* pMoveSize2,VARIANT* pSearchSize,VARIANT* pScrapEdgeArea);
	STDMETHOD(RubberMatScrapEdgeDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RubberMatRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelection,VARIANT* pDilationSize, VARIANT* pmoveSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(RubberMatRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RubberDotRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pThredValue,VARIANT* pRadiusE,VARIANT* pRoiWidth,VARIANT*pGrayValue,VARIANT* pDilationSize);
	STDMETHOD(RubberDotRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(RubberDotRegionLocationAlgo1)(VARIANT* rgnId, VARIANT* pSelection,VARIANT* pAngleRota,VARIANT* pThredValue,VARIANT* pCirDiameter,VARIANT* pRadiusE,VARIANT* pRoiWidth,VARIANT*pGrayValue,VARIANT* pDilationSize);
	STDMETHOD(RubberDotRegionLocationAlgo1Help)(BSTR* pHelpStr);

	// 通过小圆查找角度（32）
	STDMETHOD(DetectSmalCircleAngleHelp)(BSTR* pHelpStr);
	STDMETHOD(DetectSmalCircleAngle)(VARIANT* rgnId, VARIANT* pRadius,VARIANT* pRoiWidth,VARIANT* pGrayValue,VARIANT* pAreaLimitSize);

	// 内壁区域提取
	STDMETHOD (AB32_InnerWallRegDetectAlgo)(VARIANT* rgnId, VARIANT* pMoveSize, VARIANT* pThredLower, VARIANT* pTopRegionArea) ;
	STDMETHOD (AB32_InnerWallRegDetectAlgoHelp)(BSTR* pHelpStr);
	// 内底区域提取
	STDMETHOD (AB32_BottomRegionLocationAlgo)(VARIANT* rgnId,  VARIANT* pUpPosiotion,VARIANT* pDownPosition, VARIANT* pHorizonEro,VARIANT* pScaleSize) ;
	STDMETHOD (AB32_BottomRegionLocationAlgoHelp)(BSTR* pHelpStr);
	// 内底边缘区域提取
	STDMETHOD (AB32_BottomBorderRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pLowThred, VARIANT* pUpEdgeSize,VARIANT* pDownEdgeSize,VARIANT* pHClipSize) ;
	STDMETHOD (AB32_BottomBorderRegionLocationAlgoHelp)(BSTR* pHelpStr);
	
	// 内底边缘高亮等相关区域提取
	STDMETHOD (AB32_BottomEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelection, VARIANT* pUpEdgeSize,VARIANT* pDownEdgeSize,VARIANT* pHClipSize,VARIANT* pScaleSize) ;
	STDMETHOD (AB32_BottomEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	// 顶部斜坡区域提取
	STDMETHOD(AB32_SideSlopRegionLocationAlgo)(VARIANT* rgnId, VARIANT*pDilationSize,VARIANT* pHErosionSize, VARIANT* pVErosionSize);
	STDMETHOD(AB32_SideSlopRegionLocationAlgoHelp)(BSTR* pHelpStr);

	// 颈部基准线检测
	STDMETHOD(NeckBaseEdgeDetectAlgo2)(VARIANT* rgnId, VARIANT* pSearchSize, VARIANT* pHclipSize, VARIANT* pABEdgeArea);
	STDMETHOD(NeckBaseEdgeDetectAlgo2Help)(BSTR* pHelpStr);

	// 平台计数
	STDMETHOD(CountValueNumberAlgo)(VARIANT* rgnId, VARIANT* pError);
	STDMETHOD(CountValueNumberAlgoHelp)(BSTR* pHelpStr);
	
	//覆膜边缘检测区域
	STDMETHOD(FilmEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHErosionSize,VARIANT* pDialtionSizeV);
	STDMETHOD(FilmEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(FilmEdgeRegionLocationAlgo1)(VARIANT* rgnId,VARIANT* pMoveSize,VARIANT* pHErosionSize,VARIANT* pDialtionSizeV);
	STDMETHOD(FilmEdgeRegionLocationAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(DerivateDynThresholdAlgo)(VARIANT* rgnId, VARIANT* pBlackMaskSize, VARIANT* pSeriousBlackPointDynThresh, VARIANT* pSeriousBlackPointSize, VARIANT* pSeriousBlackPointLength,VARIANT*pRowDiffValue,VARIANT*pAreaThred,VARIANT* pDetectArea);
	STDMETHOD(DerivateDynThresholdAlgoHelp)(BSTR* pHelpStr);

	STDMETHOD(AB20_TopGlueDetectAlgo)(VARIANT* rgnId, VARIANT*pSeriousBlackPointSize,VARIANT* pDetectArea);
	STDMETHOD(AB20_TopGlueDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(AB20_TopGlueDetectAlgo1)(VARIANT* rgnId, VARIANT* pClosSize,VARIANT*pSeriousBlackPointSize,VARIANT* pDetectArea);
	STDMETHOD(AB20_TopGlueDetectAlgo1Help)(BSTR* pHelpStr);

	STDMETHOD(T20_TopLessRubberDetectAlgo)(VARIANT* rgnId,VARIANT* pAreaLimit,VARIANT* pLongLengthLimit,VARIANT* pDetectArea);
	STDMETHOD(T20_TopLessRubberDetectAlgoHelp)(BSTR* pHelpStr);

	// 螺旋状胶塞
	STDMETHOD(SideCenterLocationAlgo1)(VARIANT* rgnId, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pErosionSize);
	STDMETHOD(SideCenterLocationAlgo1Help)(BSTR* pHelpStr);
	STDMETHOD(TopBaseLineDetectAlgo)(VARIANT* rgnId,VARIANT* pSelecttion, VARIANT* pMoveSize,VARIANT* pDilSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pTopBaseLineArea);	
	STDMETHOD(TopBaseLineDetectAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(LineDetectFromBaseTopAlgo)(VARIANT* rgnId,VARIANT* pSelecttion, VARIANT* pMoveSize,VARIANT* pDilSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pTopBaseLineArea);	
	STDMETHOD(LineDetectFromBaseTopAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(TopRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelecttion, VARIANT* pThresholdLow,VARIANT* pOutlierRemoverSize,VARIANT* pErosionSize);
	STDMETHOD(TopRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(SideTopSectorRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelection,VARIANT* pdUpPos,VARIANT* pdDownPos,VARIANT* pAngleStart,VARIANT* pAngleEnd,VARIANT* pOuterSize,VARIANT* pInnerSize);
	STDMETHOD(SideTopSectorRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(TopEdgeRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pDilSiz,VARIANT* pHClipSize,VARIANT* pErosionSize);
	STDMETHOD(TopEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MiddleEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelection, VARIANT* pMoveSize,VARIANT* pDilSiz,VARIANT* pHClipSize,VARIANT* pErosionSize);
	STDMETHOD(MiddleEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(MiddleRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelection, VARIANT* pMoveSize,VARIANT* pDilSiz,VARIANT* pHClipSize,VARIANT* pErosionSize);
	STDMETHOD(MiddleRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(LineDetectFromBaseDownAlgo)(VARIANT* rgnId,VARIANT* pSelecttion, VARIANT* pMoveSize,VARIANT* pDilSize,VARIANT* pHClipSize,VARIANT* pThreshold,VARIANT* pTopBaseLineArea);	
	STDMETHOD(LineDetectFromBaseDownAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(DownEdgeRegionLocationAlgo)(VARIANT* rgnId,VARIANT* pSelection, VARIANT* pMoveSize,VARIANT* pDilSiz,VARIANT* pHClipSize,VARIANT* pErosionSize);
	STDMETHOD(DownEdgeRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(BottomRegionLocationAlgo)(VARIANT* rgnId, VARIANT* pMoveSize,VARIANT* pHClipSize,VARIANT* pErosionSize);
	STDMETHOD(BottomRegionLocationAlgoHelp)(BSTR* pHelpStr);
	STDMETHOD(WholeRegionLocationForHelixAlgo)(VARIANT* rgnId,VARIANT* pSelection, VARIANT* pErosionSize);
	STDMETHOD(WholeRegionLocationForHelixAlgoHelp)(BSTR* pHelpStr);
};

OBJECT_ENTRY_AUTO(__uuidof(PlugAlgo), CPlugAlgo)
