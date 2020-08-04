

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Dec 30 10:06:00 2019
 */
/* Compiler settings for AlPlasticCapAlgo.idl:
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


#include "AlPlasticCapAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _AlPlasticCapAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } AlPlasticCapAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _AlPlasticCapAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } AlPlasticCapAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _AlPlasticCapAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } AlPlasticCapAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const AlPlasticCapAlgo_MIDL_TYPE_FORMAT_STRING AlPlasticCapAlgo__MIDL_TypeFormatString;
extern const AlPlasticCapAlgo_MIDL_PROC_FORMAT_STRING AlPlasticCapAlgo__MIDL_ProcFormatString;
extern const AlPlasticCapAlgo_MIDL_EXPR_FORMAT_STRING AlPlasticCapAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAlPlasticCapAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAlPlasticCapAlgo_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const AlPlasticCapAlgo_MIDL_PROC_FORMAT_STRING AlPlasticCapAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const AlPlasticCapAlgo_MIDL_TYPE_FORMAT_STRING AlPlasticCapAlgo__MIDL_TypeFormatString =
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


/* Object interface: IAlPlasticCapAlgo, ver. 0.0,
   GUID={0x88A26EAF,0x5D85,0x493E,{0xBF,0xB2,0xE2,0xBF,0x8A,0xEA,0xEE,0x21}} */

#pragma code_seg(".orpc")
static const unsigned short IAlPlasticCapAlgo_FormatStringOffsetTable[] =
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
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO IAlPlasticCapAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    AlPlasticCapAlgo__MIDL_ProcFormatString.Format,
    &IAlPlasticCapAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAlPlasticCapAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    AlPlasticCapAlgo__MIDL_ProcFormatString.Format,
    &IAlPlasticCapAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(133) _IAlPlasticCapAlgoProxyVtbl = 
{
    &IAlPlasticCapAlgo_ProxyInfo,
    &IID_IAlPlasticCapAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* IAlPlasticCapAlgo::SetCurrentImage */ ,
    0 /* IAlPlasticCapAlgo::GetRegion */ ,
    0 /* IAlPlasticCapAlgo::GetErrorRegion */ ,
    0 /* IAlPlasticCapAlgo::GetWhiteErrorRegion */ ,
    0 /* IAlPlasticCapAlgo::GetInternalStats */ ,
    0 /* IAlPlasticCapAlgo::ReloadExternalFile */ ,
    0 /* IAlPlasticCapAlgo::SetCurrentTaskName */ ,
    0 /* IAlPlasticCapAlgo::SetSelfLearningTargetTaskName */ ,
    0 /* IAlPlasticCapAlgo::GetInterImgs */ ,
    0 /* IAlPlasticCapAlgo::SetSelfLearningParam */ ,
    0 /* IAlPlasticCapAlgo::SelectModelImage */ ,
    0 /* IAlPlasticCapAlgo::DeleteModelImage */ ,
    0 /* IAlPlasticCapAlgo::SaveResult */ ,
    0 /* IAlPlasticCapAlgo::AddModelImage */ ,
    0 /* IAlPlasticCapAlgo::GetSelfLearningResult */ ,
    0 /* IAlPlasticCapAlgo::ResetSelfLearningRegion */ ,
    0 /* IAlPlasticCapAlgo::GetSelfLearningRegion */ ,
    0 /* IAlPlasticCapAlgo::ResetSelfLearningProcedure */ ,
    0 /* IAlPlasticCapAlgo::GetRegionEx */ ,
    0 /* IAlPlasticCapAlgo::GetRegionExTest */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationAlgo */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationAlgo1 */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationAlgo1Help */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationAlgoHS */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationAlgoHSHelp */ ,
    0 /* IAlPlasticCapAlgo::CenterRectangleLocationAlgoHS */ ,
    0 /* IAlPlasticCapAlgo::CenterRectangleLocationAlgoHSHelp */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationAlgoUsePointFitting */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationAlgoUsePointFittingHelp */ ,
    0 /* IAlPlasticCapAlgo::MatchingParaAlgo */ ,
    0 /* IAlPlasticCapAlgo::MatchingParaAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::MatchingParaMinScoreAlgo */ ,
    0 /* IAlPlasticCapAlgo::MatchingParaMinScoreAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::RotatingAngleDetectAlgo */ ,
    0 /* IAlPlasticCapAlgo::RotatingAngleDetectAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::CircleRegionLocationAlgo */ ,
    0 /* IAlPlasticCapAlgo::CircleRegionLocationAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::CircleRegionAngleAlgo */ ,
    0 /* IAlPlasticCapAlgo::CircleRegionAngleAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::DetectRegionLocationAlgo */ ,
    0 /* IAlPlasticCapAlgo::DetectRegionLocationAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::DetectRegionLocationAlgoOCR */ ,
    0 /* IAlPlasticCapAlgo::DetectRegionLocationAlgoOCRHelp */ ,
    0 /* IAlPlasticCapAlgo::DetectRegionLocationExtractOCRAlgo */ ,
    0 /* IAlPlasticCapAlgo::DetectRegionLocationExtractOCRAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::CircleRegionLocationAlgo1 */ ,
    0 /* IAlPlasticCapAlgo::CircleRegionLocationAlgo1Help */ ,
    0 /* IAlPlasticCapAlgo::CircleRegionAngleAlgo1 */ ,
    0 /* IAlPlasticCapAlgo::CircleRegionAngleAlgo1Help */ ,
    0 /* IAlPlasticCapAlgo::BoundaryRegionLocationAlgo */ ,
    0 /* IAlPlasticCapAlgo::BoundaryRegionLocationAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::CurlOptimizeAlgo */ ,
    0 /* IAlPlasticCapAlgo::CurlOptimizeAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::CurlDetectAlgo2 */ ,
    0 /* IAlPlasticCapAlgo::CurlDetectAlgo2Help */ ,
    0 /* IAlPlasticCapAlgo::UVCurlAlgo */ ,
    0 /* IAlPlasticCapAlgo::UVCurlAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::AnnularRotaDynThresholdAlgo */ ,
    0 /* IAlPlasticCapAlgo::AnnularRotaDynThresholdAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::CurlNoCompoundAlgo */ ,
    0 /* IAlPlasticCapAlgo::CurlNoCompoundAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::GSurfaceAlgo */ ,
    0 /* IAlPlasticCapAlgo::GSurfaceAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::GLineAlgo */ ,
    0 /* IAlPlasticCapAlgo::GLineAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::GLineNewAlgo */ ,
    0 /* IAlPlasticCapAlgo::GLineNewAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::EdgeAlgo */ ,
    0 /* IAlPlasticCapAlgo::EdgeAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::GContoursinkAlgo */ ,
    0 /* IAlPlasticCapAlgo::GContoursinkAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgo */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoBlack */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoBlackHelp */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoBlackNumber */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoBlackNumberHelp */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoWhite */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoWhiteHelp */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoWhiteNumber */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoWhiteNumberHelp */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoBlackWhiteNumber */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoBlackWhiteNumberHelp */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoOCR */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdAlgoOCRHelp */ ,
    0 /* IAlPlasticCapAlgo::DerivateGaussDetectAlgo */ ,
    0 /* IAlPlasticCapAlgo::DerivateGaussDetectAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::BlackDetectAlgo */ ,
    0 /* IAlPlasticCapAlgo::BlackDetectAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::MinMaxGrayDetectAlgo */ ,
    0 /* IAlPlasticCapAlgo::MinMaxGrayDetectAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::AnnularRotaDynThresholdAlgoBW */ ,
    0 /* IAlPlasticCapAlgo::AnnularRotaDynThresholdAlgoBWHelp */ ,
    0 /* IAlPlasticCapAlgo::SectorRegionLocationAlgo */ ,
    0 /* IAlPlasticCapAlgo::SectorRegionLocationAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::SectorRegionLocationAlgo1 */ ,
    0 /* IAlPlasticCapAlgo::SectorRegionLocationAlgo1Help */ ,
    0 /* IAlPlasticCapAlgo::DetectOcrDistOffsetAlgo */ ,
    0 /* IAlPlasticCapAlgo::DetectOcrDistOffsetAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::RingLocationForColorAlgoHS */ ,
    0 /* IAlPlasticCapAlgo::RingLocationForColorAlgoHSHelp */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationForColorAlgoHS */ ,
    0 /* IAlPlasticCapAlgo::CenterLocationForColorAlgoHSHelp */ ,
    0 /* IAlPlasticCapAlgo::CurlOptimizeForColorAlgo */ ,
    0 /* IAlPlasticCapAlgo::CurlOptimizeForColorAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdForColorAlgo */ ,
    0 /* IAlPlasticCapAlgo::DynThresholdForColorAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::EdgeForColorAlgo */ ,
    0 /* IAlPlasticCapAlgo::EdgeForColorAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::AnnularRotaDynThresholdForColorAlgo */ ,
    0 /* IAlPlasticCapAlgo::AnnularRotaDynThresholdForColorAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::GetCurrentLocationXY */ ,
    0 /* IAlPlasticCapAlgo::GetCenterLocation */ ,
    0 /* IAlPlasticCapAlgo::PrintErrorDetectOcrAlgo */ ,
    0 /* IAlPlasticCapAlgo::PrintErrorDetectOcrAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::SideCenterLocationAlgoForColor */ ,
    0 /* IAlPlasticCapAlgo::SideCenterLocationAlgoForColorHelp */ ,
    0 /* IAlPlasticCapAlgo::PlasticRegionLocationAlgo */ ,
    0 /* IAlPlasticCapAlgo::PlasticRegionLocationAlgoHelp */ ,
    0 /* IAlPlasticCapAlgo::ExpansionContourGenerationRegionAlgo */ ,
    0 /* IAlPlasticCapAlgo_ExpansionContourGenerationRegionAlgoHelp_Proxy */ ,
    0 /* IAlPlasticCapAlgo_StepEdgeDetectAlgo_Proxy */ ,
    0 /* IAlPlasticCapAlgo_StepEdgeDetectAlgoHelp_Proxy */ ,
    0 /* IAlPlasticCapAlgo_AlRegionLocationAlgo_Proxy */ ,
    0 /* IAlPlasticCapAlgo_AlRegionLocationAlgoHelp_Proxy */
};


static const PRPC_STUB_FUNCTION IAlPlasticCapAlgo_table[] =
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
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IAlPlasticCapAlgoStubVtbl =
{
    &IID_IAlPlasticCapAlgo,
    &IAlPlasticCapAlgo_ServerInfo,
    133,
    &IAlPlasticCapAlgo_table[-3],
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
    AlPlasticCapAlgo__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _AlPlasticCapAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IAlPlasticCapAlgoProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _AlPlasticCapAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IAlPlasticCapAlgoStubVtbl,
    0
};

PCInterfaceName const _AlPlasticCapAlgo_InterfaceNamesList[] = 
{
    "IAlPlasticCapAlgo",
    0
};

const IID *  const _AlPlasticCapAlgo_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _AlPlasticCapAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _AlPlasticCapAlgo, pIID, n)

int __stdcall _AlPlasticCapAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_AlPlasticCapAlgo_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo AlPlasticCapAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _AlPlasticCapAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _AlPlasticCapAlgo_StubVtblList,
    (const PCInterfaceName * ) & _AlPlasticCapAlgo_InterfaceNamesList,
    (const IID ** ) & _AlPlasticCapAlgo_BaseIIDList,
    & _AlPlasticCapAlgo_IID_Lookup, 
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

