

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Nov 21 15:21:28 2019
 */
/* Compiler settings for BatteryShellAlgo.idl:
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


#include "BatteryShellAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _BatteryShellAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } BatteryShellAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _BatteryShellAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } BatteryShellAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _BatteryShellAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } BatteryShellAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const BatteryShellAlgo_MIDL_TYPE_FORMAT_STRING BatteryShellAlgo__MIDL_TypeFormatString;
extern const BatteryShellAlgo_MIDL_PROC_FORMAT_STRING BatteryShellAlgo__MIDL_ProcFormatString;
extern const BatteryShellAlgo_MIDL_EXPR_FORMAT_STRING BatteryShellAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IBatteryShellAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IBatteryShellAlgo_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const BatteryShellAlgo_MIDL_PROC_FORMAT_STRING BatteryShellAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const BatteryShellAlgo_MIDL_TYPE_FORMAT_STRING BatteryShellAlgo__MIDL_TypeFormatString =
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


/* Object interface: IBatteryShellAlgo, ver. 0.0,
   GUID={0x2216931D,0xBC85,0x4A14,{0x8F,0x4A,0x3E,0x87,0x4C,0x61,0x8D,0x87}} */

#pragma code_seg(".orpc")
static const unsigned short IBatteryShellAlgo_FormatStringOffsetTable[] =
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
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO IBatteryShellAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    BatteryShellAlgo__MIDL_ProcFormatString.Format,
    &IBatteryShellAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IBatteryShellAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    BatteryShellAlgo__MIDL_ProcFormatString.Format,
    &IBatteryShellAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(170) _IBatteryShellAlgoProxyVtbl = 
{
    &IBatteryShellAlgo_ProxyInfo,
    &IID_IBatteryShellAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* IBatteryShellAlgo::SetCurrentImage */ ,
    0 /* IBatteryShellAlgo::GetRegion */ ,
    0 /* IBatteryShellAlgo::GetErrorRegion */ ,
    0 /* IBatteryShellAlgo::GetWhiteErrorRegion */ ,
    0 /* IBatteryShellAlgo::GetInternalStats */ ,
    0 /* IBatteryShellAlgo::ReloadExternalFile */ ,
    0 /* IBatteryShellAlgo::SetCurrentTaskName */ ,
    0 /* IBatteryShellAlgo::SetSelfLearningTargetTaskName */ ,
    0 /* IBatteryShellAlgo::GetGlueInjectorID */ ,
    0 /* IBatteryShellAlgo::GetCurrentLocationXY */ ,
    0 /* IBatteryShellAlgo::GetCenterLocation */ ,
    0 /* IBatteryShellAlgo::GetInterImgs */ ,
    0 /* IBatteryShellAlgo::GetRegionEx */ ,
    0 /* IBatteryShellAlgo::GetRegionExTest */ ,
    0 /* IBatteryShellAlgo::SetSelfLearningParam */ ,
    0 /* IBatteryShellAlgo::SelectModelImage */ ,
    0 /* IBatteryShellAlgo::DeleteModelImage */ ,
    0 /* IBatteryShellAlgo::SaveResult */ ,
    0 /* IBatteryShellAlgo::AddModelImage */ ,
    0 /* IBatteryShellAlgo::GetSelfLearningResult */ ,
    0 /* IBatteryShellAlgo::ResetSelfLearningRegion */ ,
    0 /* IBatteryShellAlgo::GetSelfLearningRegion */ ,
    0 /* IBatteryShellAlgo::ResetSelfLearningProcedure */ ,
    0 /* IBatteryShellAlgo::CenterLocationAlgo */ ,
    0 /* IBatteryShellAlgo::CenterLocationAlgoHelp */ ,
    0 /* IBatteryShellAlgo::CenterLocationAlgo1 */ ,
    0 /* IBatteryShellAlgo::CenterLocationAlgo1Help */ ,
    0 /* IBatteryShellAlgo::CenterLocationAlgoUsePointFitting */ ,
    0 /* IBatteryShellAlgo::CenterLocationAlgoUsePointFittingHelp */ ,
    0 /* IBatteryShellAlgo::IlluminateCenterLocationAlgoHS */ ,
    0 /* IBatteryShellAlgo::IlluminateCenterLocationAlgoHSHelp */ ,
    0 /* IBatteryShellAlgo::RectLocationAlgo */ ,
    0 /* IBatteryShellAlgo::RectLocationAlgoHelp */ ,
    0 /* IBatteryShellAlgo::GrayLevelUniformityCorrectionAlgo */ ,
    0 /* IBatteryShellAlgo::GrayLevelUniformityCorrectionAlgoHelp */ ,
    0 /* IBatteryShellAlgo::CircleRegionLocationAlgo */ ,
    0 /* IBatteryShellAlgo::CircleRegionLocationAlgoHelp */ ,
    0 /* IBatteryShellAlgo::CircleRegionLocationAlgo1 */ ,
    0 /* IBatteryShellAlgo::CircleRegionLocationAlgo1Help */ ,
    0 /* IBatteryShellAlgo::CircleRegionAngleAlgo */ ,
    0 /* IBatteryShellAlgo::CircleRegionAngleAlgoHelp */ ,
    0 /* IBatteryShellAlgo::CircleRegionAngleAlgo1 */ ,
    0 /* IBatteryShellAlgo::CircleRegionAngleAlgo1Help */ ,
    0 /* IBatteryShellAlgo::BoundaryRegionLocationAlgo */ ,
    0 /* IBatteryShellAlgo::BoundaryRegionLocationAlgoHelp */ ,
    0 /* IBatteryShellAlgo::IrregularRingRegionLocationAlgo */ ,
    0 /* IBatteryShellAlgo::IrregularRingRegionLocationAlgoHelp */ ,
    0 /* IBatteryShellAlgo::IrregularRingRegionLocationAlgo1 */ ,
    0 /* IBatteryShellAlgo::IrregularRingRegionLocationAlgo1Help */ ,
    0 /* IBatteryShellAlgo::RectRegionExtraction */ ,
    0 /* IBatteryShellAlgo::RectRegionExtractionHelp */ ,
    0 /* IBatteryShellAlgo::PRectRegionExtraction */ ,
    0 /* IBatteryShellAlgo::PRectRegionExtractionHelp */ ,
    0 /* IBatteryShellAlgo::CurlOptimizeAlgo */ ,
    0 /* IBatteryShellAlgo::CurlOptimizeAlgoHelp */ ,
    0 /* IBatteryShellAlgo::CurlDetectAlgo2 */ ,
    0 /* IBatteryShellAlgo::CurlDetectAlgo2Help */ ,
    0 /* IBatteryShellAlgo::UVCurlAlgo */ ,
    0 /* IBatteryShellAlgo::UVCurlAlgoHelp */ ,
    0 /* IBatteryShellAlgo::RotaDynThresholdWithFilterAlgo */ ,
    0 /* IBatteryShellAlgo::RotaDynThresholdWithFilterAlgoHelp */ ,
    0 /* IBatteryShellAlgo::EmphasizeDarkPointDetectAlgo */ ,
    0 /* IBatteryShellAlgo::EmphasizeDarkPointDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::CurlNoCompoundAlgo */ ,
    0 /* IBatteryShellAlgo::CurlNoCompoundAlgoHelp */ ,
    0 /* IBatteryShellAlgo::GDarkLineWithFilterAlgoHelp */ ,
    0 /* IBatteryShellAlgo::GDarkLineWithFilterAlgo */ ,
    0 /* IBatteryShellAlgo::GWhiteineWithFilterAlgoHelp */ ,
    0 /* IBatteryShellAlgo::GWhiteineWithFilterAlgo */ ,
    0 /* IBatteryShellAlgo::GSurfaceUnionAlgoHelp */ ,
    0 /* IBatteryShellAlgo::GSurfaceUnionAlgo */ ,
    0 /* IBatteryShellAlgo::GSurfaceAlgo */ ,
    0 /* IBatteryShellAlgo::GSurfaceAlgoHelp */ ,
    0 /* IBatteryShellAlgo::EdgeAlgo */ ,
    0 /* IBatteryShellAlgo::EdgeAlgoHelp */ ,
    0 /* IBatteryShellAlgo::GContoursinkAlgo */ ,
    0 /* IBatteryShellAlgo::GContoursinkAlgoHelp */ ,
    0 /* IBatteryShellAlgo::DynThresholdAlgo */ ,
    0 /* IBatteryShellAlgo::DynThresholdAlgoHelp */ ,
    0 /* IBatteryShellAlgo::DynThresholdAlgoBlack */ ,
    0 /* IBatteryShellAlgo::DynThresholdAlgoBlackHelp */ ,
    0 /* IBatteryShellAlgo::DynThresholdAlgoWhite */ ,
    0 /* IBatteryShellAlgo::DynThresholdAlgoWhiteHelp */ ,
    0 /* IBatteryShellAlgo::DerivateGaussDetectAlgo */ ,
    0 /* IBatteryShellAlgo::DerivateGaussDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::ReducedResolutionDynThreshAlgo */ ,
    0 /* IBatteryShellAlgo::ReducedResolutionDynThreshAlgoHelp */ ,
    0 /* IBatteryShellAlgo::TheColumnOffsetAlgo */ ,
    0 /* IBatteryShellAlgo::TheColumnOffsetAlgoHelp */ ,
    0 /* IBatteryShellAlgo::FindingInterferenceAreaAlgo */ ,
    0 /* IBatteryShellAlgo::FindingInterferenceAreaAlgoHelp */ ,
    0 /* IBatteryShellAlgo::WaterSpotsDetectAlgo */ ,
    0 /* IBatteryShellAlgo::WaterSpotsDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::PFindingMoldScratchAreaAlgo */ ,
    0 /* IBatteryShellAlgo::PFindingMoldScratchAreaAlgoHelp */ ,
    0 /* IBatteryShellAlgo::ShoulderRingDeformationDetectAlgo */ ,
    0 /* IBatteryShellAlgo::ShoulderRingDeformationDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::DiffRegionMaxWidthDetectAlgo */ ,
    0 /* IBatteryShellAlgo::DiffRegionMaxWidthDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::MoveImageDynThreshAlgo */ ,
    0 /* IBatteryShellAlgo::MoveImageDynThreshAlgoHelp */ ,
    0 /* IBatteryShellAlgo::MoveBlockDetectAlgo */ ,
    0 /* IBatteryShellAlgo::MoveBlockDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::CircleIncisionShapeChangeDetectAlgo */ ,
    0 /* IBatteryShellAlgo::CircleIncisionShapeChangeDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::EdgeDetectWithLengthFilterAlgoHelp */ ,
    0 /* IBatteryShellAlgo::EdgeDetectWithLengthFilterAlgo */ ,
    0 /* IBatteryShellAlgo::ExcludePointGradientDetectAlgo */ ,
    0 /* IBatteryShellAlgo::ExcludePointGradientDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::CircleRegionAngleDetectAlgo */ ,
    0 /* IBatteryShellAlgo::CircleRegionAngleDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::OneWayDetectAlgo */ ,
    0 /* IBatteryShellAlgo::OneWayDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo::DetectTheOCRSectorRegionAlgo */ ,
    0 /* IBatteryShellAlgo::DetectTheOCRSectorRegionAlgoHelp */ ,
    0 /* IBatteryShellAlgo::DisOCRCircleRegionAngleAlgo */ ,
    0 /* IBatteryShellAlgo::DisOCRCircleRegionAngleAlgoHelp */ ,
    0 /* IBatteryShellAlgo::DynDetectTheOCRSectorRegionAlgo */ ,
    0 /* IBatteryShellAlgo::DynDetectTheOCRSectorRegionAlgoHelp */ ,
    0 /* IBatteryShellAlgo::PVarThreshDarkDetectAlgo */ ,
    0 /* IBatteryShellAlgo::PVarThreshDarkDetectAlgoHelp */ ,
    0 /* IBatteryShellAlgo_PVarThreshLightDetectAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PVarThreshLightDetectAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PGDarkLineAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PGDarkLineAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PBrushDetectAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PBrushDetectAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PGLineXAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PGLineXAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PThreshLightDetectAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PThreshLightDetectAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PThreshDarkDetectAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PThreshDarkDetectAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PDynThreshDeviationBlack_Proxy */ ,
    0 /* IBatteryShellAlgo_PDynThreshDeviationBlackHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PExDarkPointGradientDetectAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PExDarkPointGradientDetectAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PYPartialDetectAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PYPartialDetectAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PDentMeanCurveAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PDentMeanCurveAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PEdgeAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PEdgeAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PDynThreshAlgoWhite_Proxy */ ,
    0 /* IBatteryShellAlgo_PDynThreshAlgoWhiteHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PDynThreshAlgoBlack_Proxy */ ,
    0 /* IBatteryShellAlgo_PDynThreshAlgoBlackHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PDyn2TimesAlgoBlack_Proxy */ ,
    0 /* IBatteryShellAlgo_PDyn2TimesAlgoBlackHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PHessianMatrixAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PHessianMatrixAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PGradientDetectAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PGradientDetectAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PEmphasizeDarkPointDetectAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PEmphasizeDarkPointDetectAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PDentAreaAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_PDentAreaAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_PYPartialDetectAlgo1_Proxy */ ,
    0 /* IBatteryShellAlgo_PYPartialDetectAlgo1Help_Proxy */ ,
    0 /* IBatteryShellAlgo_MatchingParaAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_MatchingParaAlgoHelp_Proxy */ ,
    0 /* IBatteryShellAlgo_DetectRegionLocationAlgo_Proxy */ ,
    0 /* IBatteryShellAlgo_DetectRegionLocationAlgoHelp_Proxy */
};


static const PRPC_STUB_FUNCTION IBatteryShellAlgo_table[] =
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
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IBatteryShellAlgoStubVtbl =
{
    &IID_IBatteryShellAlgo,
    &IBatteryShellAlgo_ServerInfo,
    170,
    &IBatteryShellAlgo_table[-3],
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
    BatteryShellAlgo__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _BatteryShellAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IBatteryShellAlgoProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _BatteryShellAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IBatteryShellAlgoStubVtbl,
    0
};

PCInterfaceName const _BatteryShellAlgo_InterfaceNamesList[] = 
{
    "IBatteryShellAlgo",
    0
};

const IID *  const _BatteryShellAlgo_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _BatteryShellAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _BatteryShellAlgo, pIID, n)

int __stdcall _BatteryShellAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_BatteryShellAlgo_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo BatteryShellAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _BatteryShellAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _BatteryShellAlgo_StubVtblList,
    (const PCInterfaceName * ) & _BatteryShellAlgo_InterfaceNamesList,
    (const IID ** ) & _BatteryShellAlgo_BaseIIDList,
    & _BatteryShellAlgo_IID_Lookup, 
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

