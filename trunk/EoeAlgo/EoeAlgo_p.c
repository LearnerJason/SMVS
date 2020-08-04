

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:16:40 2019
 */
/* Compiler settings for EoeAlgo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "EoeAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _EoeAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } EoeAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _EoeAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } EoeAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _EoeAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } EoeAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const EoeAlgo_MIDL_TYPE_FORMAT_STRING EoeAlgo__MIDL_TypeFormatString;
extern const EoeAlgo_MIDL_PROC_FORMAT_STRING EoeAlgo__MIDL_ProcFormatString;
extern const EoeAlgo_MIDL_EXPR_FORMAT_STRING EoeAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IEoeAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IEoeAlgo_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const EoeAlgo_MIDL_PROC_FORMAT_STRING EoeAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const EoeAlgo_MIDL_TYPE_FORMAT_STRING EoeAlgo__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IEoeAlgo, ver. 0.0,
   GUID={0x88A26EAF,0x5D85,0x493E,{0xBF,0xB2,0xE2,0xBF,0x8A,0xEA,0xEE,0x21}} */

#pragma code_seg(".orpc")
static const unsigned short IEoeAlgo_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO IEoeAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    EoeAlgo__MIDL_ProcFormatString.Format,
    &IEoeAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IEoeAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    EoeAlgo__MIDL_ProcFormatString.Format,
    &IEoeAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(231) _IEoeAlgoProxyVtbl = 
{
    &IEoeAlgo_ProxyInfo,
    &IID_IEoeAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* IEoeAlgo::SetCurrentImage */ ,
    0 /* IEoeAlgo::GetRegion */ ,
    0 /* IEoeAlgo::GetErrorRegion */ ,
    0 /* IEoeAlgo::GetWhiteErrorRegion */ ,
    0 /* IEoeAlgo::GetInternalStats */ ,
    0 /* IEoeAlgo::ReloadExternalFile */ ,
    0 /* IEoeAlgo::SetCurrentTaskName */ ,
    0 /* IEoeAlgo::SetSelfLearningTargetTaskName */ ,
    0 /* IEoeAlgo::GetGlueInjectorID */ ,
    0 /* IEoeAlgo::GetInterImgs */ ,
    0 /* IEoeAlgo::SetSelfLearningParam */ ,
    0 /* IEoeAlgo::SelectModelImage */ ,
    0 /* IEoeAlgo::DeleteModelImage */ ,
    0 /* IEoeAlgo::SaveResult */ ,
    0 /* IEoeAlgo::AddModelImage */ ,
    0 /* IEoeAlgo::GetSelfLearningResult */ ,
    0 /* IEoeAlgo::ResetSelfLearningRegion */ ,
    0 /* IEoeAlgo::GetSelfLearningRegion */ ,
    0 /* IEoeAlgo::ResetSelfLearningProcedure */ ,
    0 /* IEoeAlgo::GetRegionEx */ ,
    0 /* IEoeAlgo::GetRegionExTest */ ,
    0 /* IEoeAlgo::CenterLocationAlgo */ ,
    0 /* IEoeAlgo::CenterLocationAlgoHelp */ ,
    0 /* IEoeAlgo::CenterLocationAlgo1 */ ,
    0 /* IEoeAlgo::CenterLocationAlgo1Help */ ,
    0 /* IEoeAlgo::CenterLocationAlgoHS */ ,
    0 /* IEoeAlgo::CenterLocationAlgoHSHelp */ ,
    0 /* IEoeAlgo::CenterRectangleLocationAlgoHS */ ,
    0 /* IEoeAlgo::CenterRectangleLocationAlgoHSHelp */ ,
    0 /* IEoeAlgo::CenterLocationAlgoUsePointFitting */ ,
    0 /* IEoeAlgo::CenterLocationAlgoUsePointFittingHelp */ ,
    0 /* IEoeAlgo::MatchingParaAlgo */ ,
    0 /* IEoeAlgo::MatchingParaAlgoHelp */ ,
    0 /* IEoeAlgo::MatchingParaMinScoreAlgo */ ,
    0 /* IEoeAlgo::MatchingParaMinScoreAlgoHelp */ ,
    0 /* IEoeAlgo::RotatingAngleDetectAlgo */ ,
    0 /* IEoeAlgo::RotatingAngleDetectAlgoHelp */ ,
    0 /* IEoeAlgo::CircleRegionLocationAlgo */ ,
    0 /* IEoeAlgo::CircleRegionLocationAlgoHelp */ ,
    0 /* IEoeAlgo::CircleRegionAngleAlgo */ ,
    0 /* IEoeAlgo::CircleRegionAngleAlgoHelp */ ,
    0 /* IEoeAlgo::DetectRegionLocationAlgo */ ,
    0 /* IEoeAlgo::DetectRegionLocationAlgoHelp */ ,
    0 /* IEoeAlgo::DetectRegionLocationAlgoOCR */ ,
    0 /* IEoeAlgo::DetectRegionLocationAlgoOCRHelp */ ,
    0 /* IEoeAlgo::DetectRegionLocationExtractOCRAlgo */ ,
    0 /* IEoeAlgo::DetectRegionLocationExtractOCRAlgoHelp */ ,
    0 /* IEoeAlgo::CircleRegionLocationAlgo1 */ ,
    0 /* IEoeAlgo::CircleRegionLocationAlgo1Help */ ,
    0 /* IEoeAlgo::CircleRegionAngleAlgo1 */ ,
    0 /* IEoeAlgo::CircleRegionAngleAlgo1Help */ ,
    0 /* IEoeAlgo::BoundaryRegionLocationAlgo */ ,
    0 /* IEoeAlgo::BoundaryRegionLocationAlgoHelp */ ,
    0 /* IEoeAlgo::CurlOptimizeAlgo */ ,
    0 /* IEoeAlgo::CurlOptimizeAlgoHelp */ ,
    0 /* IEoeAlgo::CurlDetectAlgo1 */ ,
    0 /* IEoeAlgo::CurlDetectAlgo1Help */ ,
    0 /* IEoeAlgo::CurlDetectAlgo2 */ ,
    0 /* IEoeAlgo::CurlDetectAlgo2Help */ ,
    0 /* IEoeAlgo::UVCurlAlgo */ ,
    0 /* IEoeAlgo::UVCurlAlgoHelp */ ,
    0 /* IEoeAlgo::AnnularRotaDynThresholdAlgo */ ,
    0 /* IEoeAlgo::AnnularRotaDynThresholdAlgoHelp */ ,
    0 /* IEoeAlgo::CurlNoCompoundAlgo */ ,
    0 /* IEoeAlgo::CurlNoCompoundAlgoHelp */ ,
    0 /* IEoeAlgo::GSurfaceAlgo */ ,
    0 /* IEoeAlgo::GSurfaceAlgoHelp */ ,
    0 /* IEoeAlgo::GLineAlgo */ ,
    0 /* IEoeAlgo::GLineAlgoHelp */ ,
    0 /* IEoeAlgo::GLineNewAlgo */ ,
    0 /* IEoeAlgo::GLineNewAlgoHelp */ ,
    0 /* IEoeAlgo::EdgeAlgo */ ,
    0 /* IEoeAlgo::EdgeAlgoHelp */ ,
    0 /* IEoeAlgo::GContoursinkAlgo */ ,
    0 /* IEoeAlgo::GContoursinkAlgoHelp */ ,
    0 /* IEoeAlgo::DynThresholdAlgo */ ,
    0 /* IEoeAlgo::DynThresholdAlgoHelp */ ,
    0 /* IEoeAlgo::DynThresholdAlgoBlack */ ,
    0 /* IEoeAlgo::DynThresholdAlgoBlackHelp */ ,
    0 /* IEoeAlgo::DynThresholdAlgoBlackNumber */ ,
    0 /* IEoeAlgo::DynThresholdAlgoBlackNumberHelp */ ,
    0 /* IEoeAlgo::DynThresholdAlgoWhite */ ,
    0 /* IEoeAlgo::DynThresholdAlgoWhiteHelp */ ,
    0 /* IEoeAlgo::DynThresholdAlgoWhiteNumber */ ,
    0 /* IEoeAlgo::DynThresholdAlgoWhiteNumberHelp */ ,
    0 /* IEoeAlgo::DynThresholdAlgoBlackWhiteNumber */ ,
    0 /* IEoeAlgo::DynThresholdAlgoBlackWhiteNumberHelp */ ,
    0 /* IEoeAlgo::DynThresholdAlgoOCR */ ,
    0 /* IEoeAlgo::DynThresholdAlgoOCRHelp */ ,
    0 /* IEoeAlgo::DerivateGaussDetectAlgo */ ,
    0 /* IEoeAlgo::DerivateGaussDetectAlgoHelp */ ,
    0 /* IEoeAlgo::BlackDetectAlgo */ ,
    0 /* IEoeAlgo::BlackDetectAlgoHelp */ ,
    0 /* IEoeAlgo::MinMaxGrayDetectAlgo */ ,
    0 /* IEoeAlgo::MinMaxGrayDetectAlgoHelp */ ,
    0 /* IEoeAlgo::AnnularRotaDynThresholdAlgoBW */ ,
    0 /* IEoeAlgo::AnnularRotaDynThresholdAlgoBWHelp */ ,
    0 /* IEoeAlgo::SectorRegionLocationAlgo */ ,
    0 /* IEoeAlgo::SectorRegionLocationAlgoHelp */ ,
    0 /* IEoeAlgo::SectorRegionLocationAlgo1 */ ,
    0 /* IEoeAlgo::SectorRegionLocationAlgo1Help */ ,
    0 /* IEoeAlgo::RRegionDetectAlgo */ ,
    0 /* IEoeAlgo::RRegionDetectAlgoHelp */ ,
    0 /* IEoeAlgo::RRegionDetectBigAlgo */ ,
    0 /* IEoeAlgo::RRegionDetectBigAlgoHelp */ ,
    0 /* IEoeAlgo::DetectOcrDistOffsetAlgo */ ,
    0 /* IEoeAlgo::DetectOcrDistOffsetAlgoHelp */ ,
    0 /* IEoeAlgo::DetectRegionLocationAlgoTabOCR */ ,
    0 /* IEoeAlgo::DetectRegionLocationAlgoTabOCRHelp */ ,
    0 /* IEoeAlgo::DynThresholdAlgoTabOCR */ ,
    0 /* IEoeAlgo::DynThresholdAlgoTabOCRHelp */ ,
    0 /* IEoeAlgo::MultOcrDetectAlgo */ ,
    0 /* IEoeAlgo::MultOcrDetectAlgoHelp */ ,
    0 /* IEoeAlgo::MultOcrDetectExcludeTabAlgo */ ,
    0 /* IEoeAlgo::MultOcrDetectExcludeTabAlgoHelp */ ,
    0 /* IEoeAlgo::PrintErrorDetectOcrAlgo */ ,
    0 /* IEoeAlgo::PrintErrorDetectOcrAlgoHelp */ ,
    0 /* IEoeAlgo::GlueInjectorDetectAlgo */ ,
    0 /* IEoeAlgo::GlueInjectorDetectAlgoHelp */ ,
    0 /* IEoeAlgo::GlueDetectAlgo */ ,
    0 /* IEoeAlgo::GlueDetectAlgoHelp */ ,
    0 /* IEoeAlgo_GlueInjectorIDDetectAlgo_Proxy */ ,
    0 /* IEoeAlgo_GlueInjectorIDDetectAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_GlueInjectorIDDetectAlgo1_Proxy */ ,
    0 /* IEoeAlgo_GlueInjectorIDDetectAlgo1Help_Proxy */ ,
    0 /* IEoeAlgo_EccentricDetectionAlgo_Proxy */ ,
    0 /* IEoeAlgo_EccentricDetectionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_InnerCircleLocationAlgo_Proxy */ ,
    0 /* IEoeAlgo_InnerCircleLocationAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_ReprintDetectionAlgo_Proxy */ ,
    0 /* IEoeAlgo_ReprintDetectionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_WLineDetectionAlgo_Proxy */ ,
    0 /* IEoeAlgo_WLineDetectionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_WSpotDetectionAlgo_Proxy */ ,
    0 /* IEoeAlgo_WSpotDetectionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_WBlockDetectionAlgo_Proxy */ ,
    0 /* IEoeAlgo_WBlockDetectionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_ArrowDistOffsetAlgo_Proxy */ ,
    0 /* IEoeAlgo_ArrowDistOffsetAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_RingLocationForColorAlgoHS_Proxy */ ,
    0 /* IEoeAlgo_RingLocationForColorAlgoHSHelp_Proxy */ ,
    0 /* IEoeAlgo_CenterLocationForColorAlgoHS_Proxy */ ,
    0 /* IEoeAlgo_CenterLocationForColorAlgoHSHelp_Proxy */ ,
    0 /* IEoeAlgo_CurlOptimizeForColorAlgo_Proxy */ ,
    0 /* IEoeAlgo_CurlOptimizeForColorAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_DynThresholdForColorAlgo_Proxy */ ,
    0 /* IEoeAlgo_DynThresholdForColorAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_EdgeForColorAlgo_Proxy */ ,
    0 /* IEoeAlgo_EdgeForColorAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_AnnularRotaDynThresholdForColorAlgo_Proxy */ ,
    0 /* IEoeAlgo_AnnularRotaDynThresholdForColorAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_AnnularRotaDynThresholdForColorAlgo1_Proxy */ ,
    0 /* IEoeAlgo_AnnularRotaDynThresholdForColorAlgo1Help_Proxy */ ,
    0 /* IEoeAlgo_GetCurrentLocationXY_Proxy */ ,
    0 /* IEoeAlgo_GetCenterLocation_Proxy */ ,
    0 /* IEoeAlgo_RivetCenterLocationAlgo_Proxy */ ,
    0 /* IEoeAlgo_RivetCenterLocationAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_RivetDiameterMeasureAlgo_Proxy */ ,
    0 /* IEoeAlgo_RivetDiameterMeasureAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_RivetDiameterMeasureAlgo1_Proxy */ ,
    0 /* IEoeAlgo_RivetDiameterMeasureAlgo1Help_Proxy */ ,
    0 /* IEoeAlgo_WeldAngleDetectAlgo_Proxy */ ,
    0 /* IEoeAlgo_WeldAngleDetectAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_WeldAngleDetectGrayAlgo_Proxy */ ,
    0 /* IEoeAlgo_WeldAngleDetectGrayAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_WeldAngleDetectWhiteGlueAlgo_Proxy */ ,
    0 /* IEoeAlgo_WeldAngleDetectWhiteGlueAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_WeldAngleDetectGlueAlgo_Proxy */ ,
    0 /* IEoeAlgo_WeldAngleDetectGlueAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_WeldWhiteGlueRegExtractAlgo_Proxy */ ,
    0 /* IEoeAlgo_WeldWhiteGlueRegExtractAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_WeldRegSelectAlgo_Proxy */ ,
    0 /* IEoeAlgo_WeldRegSelectAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_PhaseCorrelationAlgo_Proxy */ ,
    0 /* IEoeAlgo_PhaseCorrelationAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_SetCurrentTaskName2_Proxy */ ,
    0 /* IEoeAlgo_PhaseCorrelationRectangularRegionAlgo_Proxy */ ,
    0 /* IEoeAlgo_PhaseCorrelationRectangularRegionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_PhaseCorrelationRectangularAlgo_Proxy */ ,
    0 /* IEoeAlgo_PhaseCorrelationRectangularAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_CenterRectangleLocationAlgoSobel_Proxy */ ,
    0 /* IEoeAlgo_CenterRectangleLocationAlgoSobelHelp_Proxy */ ,
    0 /* IEoeAlgo_EdgeDetectRectanglePixelWiseAlgo_Proxy */ ,
    0 /* IEoeAlgo_EdgeDetectRectanglePixelWiseAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_CurlOptimizeRectangleAlgo_Proxy */ ,
    0 /* IEoeAlgo_CurlOptimizeRectangleAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_TabDeviationDetectAlgo_Proxy */ ,
    0 /* IEoeAlgo_TabDeviationDetectAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_DerivateGaussDetectComprehensiveAlgo_Proxy */ ,
    0 /* IEoeAlgo_DerivateGaussDetectComprehensiveAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_DetectRegionExcludingArbitraryRegionAlgo_Proxy */ ,
    0 /* IEoeAlgo_DetectRegionExcludingArbitraryRegionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_DetectRegionRectangleLocationAlgo_Proxy */ ,
    0 /* IEoeAlgo_DetectRegionRectangleLocationAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_RegistrateRectangleAlgo_Proxy */ ,
    0 /* IEoeAlgo_RegistrateRectangleAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_CenterRectangleLocationAlgoInnerEdge_Proxy */ ,
    0 /* IEoeAlgo_CenterRectangleLocationAlgoInnerEdgeHelp_Proxy */ ,
    0 /* IEoeAlgo_CenterRectangleSecondLocationAlgo_Proxy */ ,
    0 /* IEoeAlgo_CenterRectangleSecondLocationAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_AngleRegistrationAlgo_Proxy */ ,
    0 /* IEoeAlgo_AngleRegistrationAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_DiscontinuousLineDetectionAlgo_Proxy */ ,
    0 /* IEoeAlgo_DiscontinuousLineDetectionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_TabDeviationDetectIndistinctEdgeAlgo_Proxy */ ,
    0 /* IEoeAlgo_TabDeviationDetectIndistinctEdgeAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_RectangleRoundRegionAlgo_Proxy */ ,
    0 /* IEoeAlgo_RectangleRoundRegionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_RectangleEdgeRegionAlgo_Proxy */ ,
    0 /* IEoeAlgo_RectangleEdgeRegionAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_DefectDetectionRectanglePixelWiseAlgo_Proxy */ ,
    0 /* IEoeAlgo_DefectDetectionRectanglePixelWiseAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_DefectDetectionRectanglePixelWiseDarkLightAlgo_Proxy */ ,
    0 /* IEoeAlgo_DefectDetectionRectanglePixelWiseDarkLightAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_AnnularRotaDynThresholdAlgoRotateImage_Proxy */ ,
    0 /* IEoeAlgo_AnnularRotaDynThresholdAlgoRotateImageHelp_Proxy */ ,
    0 /* IEoeAlgo_RRegionDetectExcludeEdgeAlgo_Proxy */ ,
    0 /* IEoeAlgo_RRegionDetectExcludeEdgeAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_AngleRegistrationAlgo1_Proxy */ ,
    0 /* IEoeAlgo_AngleRegistrationAlgo1Help_Proxy */ ,
    0 /* IEoeAlgo_AngleRegistrationRotateImageAlgo_Proxy */ ,
    0 /* IEoeAlgo_AngleRegistrationRotateImageAlgoHelp_Proxy */ ,
    0 /* IEoeAlgo_TabDeviationDetectionHoughAlgo_Proxy */ ,
    0 /* IEoeAlgo_TabDeviationDetectionHoughAlgoHelp_Proxy */
};


static const PRPC_STUB_FUNCTION IEoeAlgo_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IEoeAlgoStubVtbl =
{
    &IID_IEoeAlgo,
    &IEoeAlgo_ServerInfo,
    231,
    &IEoeAlgo_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    EoeAlgo__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x700022b, /* MIDL Version 7.0.555 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _EoeAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IEoeAlgoProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _EoeAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IEoeAlgoStubVtbl,
    0
};

PCInterfaceName const _EoeAlgo_InterfaceNamesList[] = 
{
    "IEoeAlgo",
    0
};

const IID *  const _EoeAlgo_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _EoeAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _EoeAlgo, pIID, n)

int __stdcall _EoeAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_EoeAlgo_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo EoeAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _EoeAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _EoeAlgo_StubVtblList,
    (const PCInterfaceName * ) & _EoeAlgo_InterfaceNamesList,
    (const IID ** ) & _EoeAlgo_BaseIIDList,
    & _EoeAlgo_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

