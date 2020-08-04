

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Dec 13 11:35:43 2019
 */
/* Compiler settings for BottleCapAlgo.idl:
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


#include "BottleCapAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _BottleCapAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } BottleCapAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _BottleCapAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } BottleCapAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _BottleCapAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } BottleCapAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const BottleCapAlgo_MIDL_TYPE_FORMAT_STRING BottleCapAlgo__MIDL_TypeFormatString;
extern const BottleCapAlgo_MIDL_PROC_FORMAT_STRING BottleCapAlgo__MIDL_ProcFormatString;
extern const BottleCapAlgo_MIDL_EXPR_FORMAT_STRING BottleCapAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IBottleCapAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IBottleCapAlgo_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const BottleCapAlgo_MIDL_PROC_FORMAT_STRING BottleCapAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const BottleCapAlgo_MIDL_TYPE_FORMAT_STRING BottleCapAlgo__MIDL_TypeFormatString =
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


/* Object interface: IBottleCapAlgo, ver. 0.0,
   GUID={0x88A26EAF,0x5D85,0x493E,{0xBF,0xB2,0xE2,0xBF,0x8A,0xEA,0xEE,0x21}} */

#pragma code_seg(".orpc")
static const unsigned short IBottleCapAlgo_FormatStringOffsetTable[] =
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
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO IBottleCapAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    BottleCapAlgo__MIDL_ProcFormatString.Format,
    &IBottleCapAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IBottleCapAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    BottleCapAlgo__MIDL_ProcFormatString.Format,
    &IBottleCapAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(235) _IBottleCapAlgoProxyVtbl = 
{
    &IBottleCapAlgo_ProxyInfo,
    &IID_IBottleCapAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* IBottleCapAlgo::SetCurrentImage */ ,
    0 /* IBottleCapAlgo::GetRegion */ ,
    0 /* IBottleCapAlgo::GetErrorRegion */ ,
    0 /* IBottleCapAlgo::GetWhiteErrorRegion */ ,
    0 /* IBottleCapAlgo::GetInternalStats */ ,
    0 /* IBottleCapAlgo::ReloadExternalFile */ ,
    0 /* IBottleCapAlgo::SetCurrentTaskName */ ,
    0 /* IBottleCapAlgo::SetSelfLearningTargetTaskName */ ,
    0 /* IBottleCapAlgo::GetGlueInjectorID */ ,
    0 /* IBottleCapAlgo::GetInterImgs */ ,
    0 /* IBottleCapAlgo::GetDetectParam */ ,
    0 /* IBottleCapAlgo::SetDetectParam */ ,
    0 /* IBottleCapAlgo::SetSelfLearningParam */ ,
    0 /* IBottleCapAlgo::SelectModelImage */ ,
    0 /* IBottleCapAlgo::DeleteModelImage */ ,
    0 /* IBottleCapAlgo::SaveResult */ ,
    0 /* IBottleCapAlgo::AddModelImage */ ,
    0 /* IBottleCapAlgo::GetSelfLearningResult */ ,
    0 /* IBottleCapAlgo::ResetSelfLearningRegion */ ,
    0 /* IBottleCapAlgo::GetSelfLearningRegion */ ,
    0 /* IBottleCapAlgo::ResetSelfLearningProcedure */ ,
    0 /* IBottleCapAlgo::GetRegionEx */ ,
    0 /* IBottleCapAlgo::GetRegionExTest */ ,
    0 /* IBottleCapAlgo::CenterLocationAlgo */ ,
    0 /* IBottleCapAlgo::CenterLocationAlgoHelp */ ,
    0 /* IBottleCapAlgo::CenterLocationAlgo1 */ ,
    0 /* IBottleCapAlgo::CenterLocationAlgo1Help */ ,
    0 /* IBottleCapAlgo::CenterLocationAlgoHS */ ,
    0 /* IBottleCapAlgo::CenterLocationAlgoHSHelp */ ,
    0 /* IBottleCapAlgo::CenterRectangleLocationAlgoHS */ ,
    0 /* IBottleCapAlgo::CenterRectangleLocationAlgoHSHelp */ ,
    0 /* IBottleCapAlgo::CenterLocationAlgoUsePointFitting */ ,
    0 /* IBottleCapAlgo::CenterLocationAlgoUsePointFittingHelp */ ,
    0 /* IBottleCapAlgo::MatchingParaAlgo */ ,
    0 /* IBottleCapAlgo::MatchingParaAlgoHelp */ ,
    0 /* IBottleCapAlgo::MatchingParaMinScoreAlgo */ ,
    0 /* IBottleCapAlgo::MatchingParaMinScoreAlgoHelp */ ,
    0 /* IBottleCapAlgo::RotatingAngleDetectAlgo */ ,
    0 /* IBottleCapAlgo::RotatingAngleDetectAlgoHelp */ ,
    0 /* IBottleCapAlgo::CircleRegionLocationAlgo */ ,
    0 /* IBottleCapAlgo::CircleRegionLocationAlgoHelp */ ,
    0 /* IBottleCapAlgo::CircleRegionAngleAlgo */ ,
    0 /* IBottleCapAlgo::CircleRegionAngleAlgoHelp */ ,
    0 /* IBottleCapAlgo::DetectRegionLocationAlgo */ ,
    0 /* IBottleCapAlgo::DetectRegionLocationAlgoHelp */ ,
    0 /* IBottleCapAlgo::DetectRegionLocationAlgoOCR */ ,
    0 /* IBottleCapAlgo::DetectRegionLocationAlgoOCRHelp */ ,
    0 /* IBottleCapAlgo::DetectRegionLocationExtractOCRAlgo */ ,
    0 /* IBottleCapAlgo::DetectRegionLocationExtractOCRAlgoHelp */ ,
    0 /* IBottleCapAlgo::CircleRegionLocationAlgo1 */ ,
    0 /* IBottleCapAlgo::CircleRegionLocationAlgo1Help */ ,
    0 /* IBottleCapAlgo::CircleRegionAngleAlgo1 */ ,
    0 /* IBottleCapAlgo::CircleRegionAngleAlgo1Help */ ,
    0 /* IBottleCapAlgo::BoundaryRegionLocationAlgo */ ,
    0 /* IBottleCapAlgo::BoundaryRegionLocationAlgoHelp */ ,
    0 /* IBottleCapAlgo::CurlOptimizeAlgo */ ,
    0 /* IBottleCapAlgo::CurlOptimizeAlgoHelp */ ,
    0 /* IBottleCapAlgo::CurlDetectAlgo1 */ ,
    0 /* IBottleCapAlgo::CurlDetectAlgo1Help */ ,
    0 /* IBottleCapAlgo::CurlDetectAlgo2 */ ,
    0 /* IBottleCapAlgo::CurlDetectAlgo2Help */ ,
    0 /* IBottleCapAlgo::UVCurlAlgo */ ,
    0 /* IBottleCapAlgo::UVCurlAlgoHelp */ ,
    0 /* IBottleCapAlgo::AnnularRotaDynThresholdAlgo */ ,
    0 /* IBottleCapAlgo::AnnularRotaDynThresholdAlgoHelp */ ,
    0 /* IBottleCapAlgo::CurlNoCompoundAlgo */ ,
    0 /* IBottleCapAlgo::CurlNoCompoundAlgoHelp */ ,
    0 /* IBottleCapAlgo::GSurfaceAlgo */ ,
    0 /* IBottleCapAlgo::GSurfaceAlgoHelp */ ,
    0 /* IBottleCapAlgo::GLineAlgo */ ,
    0 /* IBottleCapAlgo::GLineAlgoHelp */ ,
    0 /* IBottleCapAlgo::GLineNewAlgo */ ,
    0 /* IBottleCapAlgo::GLineNewAlgoHelp */ ,
    0 /* IBottleCapAlgo::EdgeAlgo */ ,
    0 /* IBottleCapAlgo::EdgeAlgoHelp */ ,
    0 /* IBottleCapAlgo::GContoursinkAlgo */ ,
    0 /* IBottleCapAlgo::GContoursinkAlgoHelp */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgo */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoHelp */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoBlack */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoBlackHelp */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoBlackNumber */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoBlackNumberHelp */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoWhite */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoWhiteHelp */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoWhiteNumber */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoWhiteNumberHelp */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoBlackWhiteNumber */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoBlackWhiteNumberHelp */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoOCR */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoOCRHelp */ ,
    0 /* IBottleCapAlgo::DerivateGaussDetectAlgo */ ,
    0 /* IBottleCapAlgo::DerivateGaussDetectAlgoHelp */ ,
    0 /* IBottleCapAlgo::BlackDetectAlgo */ ,
    0 /* IBottleCapAlgo::BlackDetectAlgoHelp */ ,
    0 /* IBottleCapAlgo::MinMaxGrayDetectAlgo */ ,
    0 /* IBottleCapAlgo::MinMaxGrayDetectAlgoHelp */ ,
    0 /* IBottleCapAlgo::AnnularRotaDynThresholdAlgoBW */ ,
    0 /* IBottleCapAlgo::AnnularRotaDynThresholdAlgoBWHelp */ ,
    0 /* IBottleCapAlgo::SectorRegionLocationAlgo */ ,
    0 /* IBottleCapAlgo::SectorRegionLocationAlgoHelp */ ,
    0 /* IBottleCapAlgo::SectorRegionLocationAlgo1 */ ,
    0 /* IBottleCapAlgo::SectorRegionLocationAlgo1Help */ ,
    0 /* IBottleCapAlgo::RRegionDetectAlgo */ ,
    0 /* IBottleCapAlgo::RRegionDetectAlgoHelp */ ,
    0 /* IBottleCapAlgo::RRegionDetectBigAlgo */ ,
    0 /* IBottleCapAlgo::RRegionDetectBigAlgoHelp */ ,
    0 /* IBottleCapAlgo::DetectOcrDistOffsetAlgo */ ,
    0 /* IBottleCapAlgo::DetectOcrDistOffsetAlgoHelp */ ,
    0 /* IBottleCapAlgo::DetectRegionLocationAlgoTabOCR */ ,
    0 /* IBottleCapAlgo::DetectRegionLocationAlgoTabOCRHelp */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoTabOCR */ ,
    0 /* IBottleCapAlgo::DynThresholdAlgoTabOCRHelp */ ,
    0 /* IBottleCapAlgo::MultOcrDetectAlgo */ ,
    0 /* IBottleCapAlgo::MultOcrDetectAlgoHelp */ ,
    0 /* IBottleCapAlgo::MultOcrDetectExcludeTabAlgo */ ,
    0 /* IBottleCapAlgo::MultOcrDetectExcludeTabAlgoHelp */ ,
    0 /* IBottleCapAlgo::PrintErrorDetectOcrAlgo */ ,
    0 /* IBottleCapAlgo::PrintErrorDetectOcrAlgoHelp */ ,
    0 /* IBottleCapAlgo::GlueInjectorDetectAlgo */ ,
    0 /* IBottleCapAlgo::GlueInjectorDetectAlgoHelp */ ,
    0 /* IBottleCapAlgo_GlueDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_GlueDetectAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_GlueInjectorIDDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_GlueInjectorIDDetectAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_GlueInjectorIDDetectAlgo1_Proxy */ ,
    0 /* IBottleCapAlgo_GlueInjectorIDDetectAlgo1Help_Proxy */ ,
    0 /* IBottleCapAlgo_EccentricDetectionAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_EccentricDetectionAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_InnerCircleLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_InnerCircleLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_ReprintDetectionAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_ReprintDetectionAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_WLineDetectionAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_WLineDetectionAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_WSpotDetectionAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_WSpotDetectionAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_WBlockDetectionAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_WBlockDetectionAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_ArrowDistOffsetAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_ArrowDistOffsetAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_RingLocationForColorAlgoHS_Proxy */ ,
    0 /* IBottleCapAlgo_RingLocationForColorAlgoHSHelp_Proxy */ ,
    0 /* IBottleCapAlgo_CenterLocationForColorAlgoHS_Proxy */ ,
    0 /* IBottleCapAlgo_CenterLocationForColorAlgoHSHelp_Proxy */ ,
    0 /* IBottleCapAlgo_CurlOptimizeForColorAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_CurlOptimizeForColorAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_DynThresholdForColorAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_DynThresholdForColorAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_EdgeForColorAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_EdgeForColorAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo1_Proxy */ ,
    0 /* IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo1Help_Proxy */ ,
    0 /* IBottleCapAlgo_GetCurrentLocationXY_Proxy */ ,
    0 /* IBottleCapAlgo_GetCenterLocation_Proxy */ ,
    0 /* IBottleCapAlgo_SetCurrentTaskName2_Proxy */ ,
    0 /* IBottleCapAlgo_SectorRotaDynThresholdAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_SectorRotaDynThresholdAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_SubDynThresholdAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_SubDynThresholdAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_CurlNoCompoundColorAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_CurlNoCompoundColorAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_CenterTopLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_CenterTopLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_BaseEdgeDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_BaseEdgeDetectAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_SideEdgeRegionLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_SideEdgeRegionLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_TopEdgeRegionLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_TopEdgeRegionLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_UpCharLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_UpCharLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_UpCharRegionLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_UpCharRegionLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_DynThresholdAlgoNew_Proxy */ ,
    0 /* IBottleCapAlgo_DynThresholdAlgoNewHelp_Proxy */ ,
    0 /* IBottleCapAlgo_LowCharLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_LowCharLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_LowCharRegionLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_LowCharRegionLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_CurlNoCompoundAlgoNew_Proxy */ ,
    0 /* IBottleCapAlgo_CurlNoCompoundAlgoNewHelp_Proxy */ ,
    0 /* IBottleCapAlgo_LineDetectionAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_LineDetectionAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_BubbleDetectProfileAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_BubbleDetectProfileAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_BubbleDetectProfileAlgoNew_Proxy */ ,
    0 /* IBottleCapAlgo_BubbleDetectProfileAlgoNewHelp_Proxy */ ,
    0 /* IBottleCapAlgo_LineDetectionColorAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_LineDetectionColorAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_BlackDetectExcludeEdgesAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_BlackDetectExcludeEdgesAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_SideCenterTopLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_SideCenterTopLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_SideCenterBodyLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_SideCenterBodyLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_BaseEdgeDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_BaseEdgeDetectAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_SideEdgeRegionLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_SideEdgeRegionLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_PatternRegionLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_PatternRegionLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_PatternReferenceLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_PatternReferenceLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_RemovePatternRegionLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_RemovePatternRegionLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_TeethRegionDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_TeethRegionDetectAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_TeethOffsetDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_TeethOffsetDetectAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_DeformableModelDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_DeformableModelDetectAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_CurlNoCompoundForClolorNewAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_CurlNoCompoundForClolorNewAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_SideWallDynThresholdAlgoNew_Proxy */ ,
    0 /* IBottleCapAlgo_SideWallDynThresholdAlgoNewHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_TeethRegionLocationAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_TeethRegionLocationAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_SideWallDynThresholdAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_SideWallDynThresholdAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_TeethDisappearDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_TeethDisappearDetectAlgoHelp_Proxy */ ,
    0 /* IBottleCapAlgo_PS_PatternRegionLocationAlgo1_Proxy */ ,
    0 /* IBottleCapAlgo_PS_PatternRegionLocationAlgo1Help_Proxy */ ,
    0 /* IBottleCapAlgo_PS_PatternDefectDetectAlgo_Proxy */ ,
    0 /* IBottleCapAlgo_PS_PatternDefectDetectAlgoHelp_Proxy */
};


static const PRPC_STUB_FUNCTION IBottleCapAlgo_table[] =
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
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IBottleCapAlgoStubVtbl =
{
    &IID_IBottleCapAlgo,
    &IBottleCapAlgo_ServerInfo,
    235,
    &IBottleCapAlgo_table[-3],
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
    BottleCapAlgo__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _BottleCapAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IBottleCapAlgoProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _BottleCapAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IBottleCapAlgoStubVtbl,
    0
};

PCInterfaceName const _BottleCapAlgo_InterfaceNamesList[] = 
{
    "IBottleCapAlgo",
    0
};

const IID *  const _BottleCapAlgo_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _BottleCapAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _BottleCapAlgo, pIID, n)

int __stdcall _BottleCapAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_BottleCapAlgo_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo BottleCapAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _BottleCapAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _BottleCapAlgo_StubVtblList,
    (const PCInterfaceName * ) & _BottleCapAlgo_InterfaceNamesList,
    (const IID ** ) & _BottleCapAlgo_BaseIIDList,
    & _BottleCapAlgo_IID_Lookup, 
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

