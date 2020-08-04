

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:17:22 2019
 */
/* Compiler settings for HdpeAlgo.idl:
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


#include "HdpeAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _HdpeAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } HdpeAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _HdpeAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } HdpeAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _HdpeAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } HdpeAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const HdpeAlgo_MIDL_TYPE_FORMAT_STRING HdpeAlgo__MIDL_TypeFormatString;
extern const HdpeAlgo_MIDL_PROC_FORMAT_STRING HdpeAlgo__MIDL_ProcFormatString;
extern const HdpeAlgo_MIDL_EXPR_FORMAT_STRING HdpeAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IHdpeAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IHdpeAlgo_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISelfLearning_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISelfLearning_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const HdpeAlgo_MIDL_PROC_FORMAT_STRING HdpeAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const HdpeAlgo_MIDL_TYPE_FORMAT_STRING HdpeAlgo__MIDL_TypeFormatString =
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


/* Object interface: IHdpeAlgo, ver. 0.0,
   GUID={0xD823E8B8,0x60C3,0x4011,{0xBE,0x03,0xE9,0x1A,0x09,0xE3,0x2F,0x20}} */

#pragma code_seg(".orpc")
static const unsigned short IHdpeAlgo_FormatStringOffsetTable[] =
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
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO IHdpeAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    HdpeAlgo__MIDL_ProcFormatString.Format,
    &IHdpeAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IHdpeAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    HdpeAlgo__MIDL_ProcFormatString.Format,
    &IHdpeAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(106) _IHdpeAlgoProxyVtbl = 
{
    &IHdpeAlgo_ProxyInfo,
    &IID_IHdpeAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* IHdpeAlgo::SetCurrentImage */ ,
    0 /* IHdpeAlgo::GetRegion */ ,
    0 /* IHdpeAlgo::GetErrorRegion */ ,
    0 /* IHdpeAlgo::GetInternalStats */ ,
    0 /* IHdpeAlgo::ReloadExternalFile */ ,
    0 /* IHdpeAlgo::SetSelfLearningParam */ ,
    0 /* IHdpeAlgo::SelectModelImage */ ,
    0 /* IHdpeAlgo::DeleteModelImage */ ,
    0 /* IHdpeAlgo::SaveResult */ ,
    0 /* IHdpeAlgo::AddModelImage */ ,
    0 /* IHdpeAlgo::GetSelfLearningResult */ ,
    0 /* IHdpeAlgo::ResetSelfLearningRegion */ ,
    0 /* IHdpeAlgo::GetSelfLearningRegion */ ,
    0 /* IHdpeAlgo::ResetSelfLearningProcedure */ ,
    0 /* IHdpeAlgo::SetSelfLearningTargetTaskName */ ,
    0 /* IHdpeAlgo::SetCurrentTaskName */ ,
    0 /* IHdpeAlgo::GetCurrentLocationXY */ ,
    0 /* IHdpeAlgo::CenterLocationAlgo */ ,
    0 /* IHdpeAlgo::CenterLocationAlgoHelp */ ,
    0 /* IHdpeAlgo::CenterLocationAlgo1 */ ,
    0 /* IHdpeAlgo::CenterLocationAlgo1Help */ ,
    0 /* IHdpeAlgo::CenterLocationUseEdgesAlgo */ ,
    0 /* IHdpeAlgo::CenterLocationUseEdgesAlgoHelp */ ,
    0 /* IHdpeAlgo::CenterLocationAlgoHS */ ,
    0 /* IHdpeAlgo::CenterLocationAlgoHSHelp */ ,
    0 /* IHdpeAlgo::RectLocationAlgoHS */ ,
    0 /* IHdpeAlgo::RectLocationAlgoHSHelp */ ,
    0 /* IHdpeAlgo::CenterLocationByMatchAlgo */ ,
    0 /* IHdpeAlgo::CenterLocationByMatchAlgoHelp */ ,
    0 /* IHdpeAlgo::MatchingParaAlgo */ ,
    0 /* IHdpeAlgo::MatchingParaAlgoHelp */ ,
    0 /* IHdpeAlgo::MatchingParaMinScoreAlgo */ ,
    0 /* IHdpeAlgo::MatchingParaMinScoreAlgoHelp */ ,
    0 /* IHdpeAlgo::OcrAngleDetectAlgo */ ,
    0 /* IHdpeAlgo::OcrAngleDetectAlgoHelp */ ,
    0 /* IHdpeAlgo::OcrAngleDetectAlgo1 */ ,
    0 /* IHdpeAlgo::OcrAngleDetectAlgo1Help */ ,
    0 /* IHdpeAlgo::CreateOcrModelAlgo */ ,
    0 /* IHdpeAlgo::CreateOcrModelAlgoHelp */ ,
    0 /* IHdpeAlgo::FindOcrNumberAlgo */ ,
    0 /* IHdpeAlgo::FindOcrNumberAlgoHelp */ ,
    0 /* IHdpeAlgo::CircleRegionLocationAlgo */ ,
    0 /* IHdpeAlgo::CircleRegionLocationAlgoHelp */ ,
    0 /* IHdpeAlgo::SectorRegionLocationAlgo */ ,
    0 /* IHdpeAlgo::SectorRegionLocationAlgoHelp */ ,
    0 /* IHdpeAlgo::DetectRegionLocationAlgo */ ,
    0 /* IHdpeAlgo::DetectRegionLocationAlgoHelp */ ,
    0 /* IHdpeAlgo::DetectRegionLocationAlgoOCR */ ,
    0 /* IHdpeAlgo::DetectRegionLocationAlgoOCRHelp */ ,
    0 /* IHdpeAlgo::RectRegionLocationAlgo */ ,
    0 /* IHdpeAlgo::RectRegionLocationAlgoHelp */ ,
    0 /* IHdpeAlgo::CurlNoCompoundAlgo */ ,
    0 /* IHdpeAlgo::CurlNoCompoundAlgoHelp */ ,
    0 /* IHdpeAlgo::CurlOptimizeAlgo */ ,
    0 /* IHdpeAlgo::CurlOptimizeAlgoHelp */ ,
    0 /* IHdpeAlgo::UVCurlAlgo */ ,
    0 /* IHdpeAlgo::UVCurlAlgoHelp */ ,
    0 /* IHdpeAlgo::AnnularRotaDynThresholdAlgo */ ,
    0 /* IHdpeAlgo::AnnularRotaDynThresholdAlgoHelp */ ,
    0 /* IHdpeAlgo::DynThresholdAlgo */ ,
    0 /* IHdpeAlgo::DynThresholdAlgoHelp */ ,
    0 /* IHdpeAlgo::DynThresholdAlgoOCR */ ,
    0 /* IHdpeAlgo::DynThresholdAlgoOCRHelp */ ,
    0 /* IHdpeAlgo::BlackDetectAlgo */ ,
    0 /* IHdpeAlgo::BlackDetectAlgoHelp */ ,
    0 /* IHdpeAlgo::BlackDetectExcludeEdgesAlgo */ ,
    0 /* IHdpeAlgo::BlackDetectExcludeEdgesAlgoHelp */ ,
    0 /* IHdpeAlgo::BubbleDetectProfileAlgo */ ,
    0 /* IHdpeAlgo::BubbleDetectProfileAlgoHelp */ ,
    0 /* IHdpeAlgo::DerivateGaussDetectAlgo */ ,
    0 /* IHdpeAlgo::DerivateGaussDetectAlgoHelp */ ,
    0 /* IHdpeAlgo::EdgeAlgo */ ,
    0 /* IHdpeAlgo::EdgeAlgoHelp */ ,
    0 /* IHdpeAlgo::LineDetectionAlgo */ ,
    0 /* IHdpeAlgo::LineDetectionAlgoHelp */ ,
    0 /* IHdpeAlgo::LineDetectionExclude90Algo */ ,
    0 /* IHdpeAlgo::LineDetectionExclude90AlgoHelp */ ,
    0 /* IHdpeAlgo::BlackDetectUseUpSizeAlgo */ ,
    0 /* IHdpeAlgo::BlackDetectUseUpSizeAlgoHelp */ ,
    0 /* IHdpeAlgo::TabDetectProfileAlgo */ ,
    0 /* IHdpeAlgo::TabDetectProfileAlgoHelp */ ,
    0 /* IHdpeAlgo::GapAngleDetectAlgo */ ,
    0 /* IHdpeAlgo::GapAngleDetectAlgoHelp */ ,
    0 /* IHdpeAlgo::DetectRegionEdgesAlgo */ ,
    0 /* IHdpeAlgo::DetectRegionEdgesAlgoHelp */ ,
    0 /* IHdpeAlgo::DetectRegionRegularAlgo */ ,
    0 /* IHdpeAlgo::DetectRegionRegularAlgoHelp */ ,
    0 /* IHdpeAlgo::DetectRegionIrregularAlgo */ ,
    0 /* IHdpeAlgo::DetectRegionIrregularAlgoHelp */ ,
    0 /* IHdpeAlgo::CenterLocationAlgoGapHS */ ,
    0 /* IHdpeAlgo::CenterLocationAlgoGapHSHelp */ ,
    0 /* IHdpeAlgo::DynThresholdForColorAlgo */ ,
    0 /* IHdpeAlgo::DynThresholdForColorAlgoHelp */ ,
    0 /* IHdpeAlgo::EdgeForColorAlgo */ ,
    0 /* IHdpeAlgo::EdgeForColorAlgoHelp */ ,
    0 /* IHdpeAlgo::AnnularRotaDynThresholdForColorAlgo */ ,
    0 /* IHdpeAlgo::AnnularRotaDynThresholdForColorAlgoHelp */ ,
    0 /* IHdpeAlgo::CurlNoCompoundForColorAlgo */ ,
    0 /* IHdpeAlgo::CurlNoCompoundForColorAlgoHelp */
};


static const PRPC_STUB_FUNCTION IHdpeAlgo_table[] =
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
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IHdpeAlgoStubVtbl =
{
    &IID_IHdpeAlgo,
    &IHdpeAlgo_ServerInfo,
    106,
    &IHdpeAlgo_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ISelfLearning, ver. 0.0,
   GUID={0x03FE5233,0x3BF3,0x4358,{0xAA,0x95,0x61,0xE0,0xD4,0xAD,0x41,0x24}} */

#pragma code_seg(".orpc")
static const unsigned short ISelfLearning_FormatStringOffsetTable[] =
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
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO ISelfLearning_ProxyInfo =
    {
    &Object_StubDesc,
    HdpeAlgo__MIDL_ProcFormatString.Format,
    &ISelfLearning_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISelfLearning_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    HdpeAlgo__MIDL_ProcFormatString.Format,
    &ISelfLearning_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _ISelfLearningProxyVtbl = 
{
    &ISelfLearning_ProxyInfo,
    &IID_ISelfLearning,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* ISelfLearning::SetSelfLearningParam */ ,
    0 /* ISelfLearning::SelectModelImage */ ,
    0 /* ISelfLearning::DeleteModelImage */ ,
    0 /* ISelfLearning::SaveResult */ ,
    0 /* ISelfLearning::AddModelImage */ ,
    0 /* ISelfLearning::GetSelfLearningResult */ ,
    0 /* ISelfLearning::ResetSelfLearningRegion */ ,
    0 /* ISelfLearning::GetSelfLearningRegion */ ,
    0 /* ISelfLearning::ResetSelfLearningProcedure */ ,
    0 /* ISelfLearning::SetSelfLearningTargetTaskName */
};

const CInterfaceStubVtbl _ISelfLearningStubVtbl =
{
    &IID_ISelfLearning,
    &ISelfLearning_ServerInfo,
    13,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
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
    HdpeAlgo__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _HdpeAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ISelfLearningProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IHdpeAlgoProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _HdpeAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ISelfLearningStubVtbl,
    ( CInterfaceStubVtbl *) &_IHdpeAlgoStubVtbl,
    0
};

PCInterfaceName const _HdpeAlgo_InterfaceNamesList[] = 
{
    "ISelfLearning",
    "IHdpeAlgo",
    0
};

const IID *  const _HdpeAlgo_BaseIIDList[] = 
{
    0,
    &IID_IDispatch,
    0
};


#define _HdpeAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _HdpeAlgo, pIID, n)

int __stdcall _HdpeAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _HdpeAlgo, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _HdpeAlgo, 2, *pIndex )
    
}

const ExtendedProxyFileInfo HdpeAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _HdpeAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _HdpeAlgo_StubVtblList,
    (const PCInterfaceName * ) & _HdpeAlgo_InterfaceNamesList,
    (const IID ** ) & _HdpeAlgo_BaseIIDList,
    & _HdpeAlgo_IID_Lookup, 
    2,
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

