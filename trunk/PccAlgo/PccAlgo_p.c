

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:17:16 2019
 */
/* Compiler settings for PccAlgo.idl:
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


#include "PccAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _PccAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } PccAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _PccAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } PccAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _PccAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } PccAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const PccAlgo_MIDL_TYPE_FORMAT_STRING PccAlgo__MIDL_TypeFormatString;
extern const PccAlgo_MIDL_PROC_FORMAT_STRING PccAlgo__MIDL_ProcFormatString;
extern const PccAlgo_MIDL_EXPR_FORMAT_STRING PccAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPccAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPccAlgo_ProxyInfo;


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


static const PccAlgo_MIDL_PROC_FORMAT_STRING PccAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const PccAlgo_MIDL_TYPE_FORMAT_STRING PccAlgo__MIDL_TypeFormatString =
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


/* Object interface: IPccAlgo, ver. 0.0,
   GUID={0x2855E408,0xC382,0x48B6,{0xA5,0xCA,0xD5,0x5F,0xE0,0xB5,0x03,0x62}} */

#pragma code_seg(".orpc")
static const unsigned short IPccAlgo_FormatStringOffsetTable[] =
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
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO IPccAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    PccAlgo__MIDL_ProcFormatString.Format,
    &IPccAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPccAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    PccAlgo__MIDL_ProcFormatString.Format,
    &IPccAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(75) _IPccAlgoProxyVtbl = 
{
    &IPccAlgo_ProxyInfo,
    &IID_IPccAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* IPccAlgo::SetCurrentImage */ ,
    0 /* IPccAlgo::GetRegion */ ,
    0 /* IPccAlgo::GetErrorRegion */ ,
    0 /* IPccAlgo::GetInternalStats */ ,
    0 /* IPccAlgo::ReloadExternalFile */ ,
    0 /* IPccAlgo::CenterLocationAlgo */ ,
    0 /* IPccAlgo::CenterLocationAlgoHelp */ ,
    0 /* IPccAlgo::CenterLocationAlgo1 */ ,
    0 /* IPccAlgo::CenterLocationAlgo1Help */ ,
    0 /* IPccAlgo::CenterLocationUseEdgesAlgo */ ,
    0 /* IPccAlgo::CenterLocationUseEdgesAlgoHelp */ ,
    0 /* IPccAlgo::CenterLocationUseEdgesAlgo1 */ ,
    0 /* IPccAlgo::CenterLocationUseEdgesAlgo1Help */ ,
    0 /* IPccAlgo::MatchingParaAlgo */ ,
    0 /* IPccAlgo::MatchingParaAlgoHelp */ ,
    0 /* IPccAlgo::MatchingParaMinScoreAlgo */ ,
    0 /* IPccAlgo::MatchingParaMinScoreAlgoHelp */ ,
    0 /* IPccAlgo::CircleRegionLocationAlgo */ ,
    0 /* IPccAlgo::CircleRegionLocationAlgoHelp */ ,
    0 /* IPccAlgo::DetectRegionLocationAlgo */ ,
    0 /* IPccAlgo::DetectRegionLocationAlgoHelp */ ,
    0 /* IPccAlgo::CurlOptimizeAlgo */ ,
    0 /* IPccAlgo::CurlOptimizeAlgoHelp */ ,
    0 /* IPccAlgo::UVCurlAlgo */ ,
    0 /* IPccAlgo::UVCurlAlgoHelp */ ,
    0 /* IPccAlgo::AnnularRotaDynThresholdAlgo */ ,
    0 /* IPccAlgo::AnnularRotaDynThresholdAlgoHelp */ ,
    0 /* IPccAlgo::CurlNoCompoundAlgo */ ,
    0 /* IPccAlgo::CurlNoCompoundAlgoHelp */ ,
    0 /* IPccAlgo::DynThresholdAlgo */ ,
    0 /* IPccAlgo::DynThresholdAlgoHelp */ ,
    0 /* IPccAlgo::BubbleDetectProfileAlgo */ ,
    0 /* IPccAlgo::BubbleDetectProfileAlgoHelp */ ,
    0 /* IPccAlgo::BlackDetectAlgo */ ,
    0 /* IPccAlgo::BlackDetectAlgoHelp */ ,
    0 /* IPccAlgo::BlackDetectExcludeEdgesAlgo */ ,
    0 /* IPccAlgo::BlackDetectExcludeEdgesAlgoHelp */ ,
    0 /* IPccAlgo::DerivateGaussDetectAlgo */ ,
    0 /* IPccAlgo::DerivateGaussDetectAlgoHelp */ ,
    0 /* IPccAlgo::EdgeAlgo */ ,
    0 /* IPccAlgo::EdgeAlgoHelp */ ,
    0 /* IPccAlgo::LineDetectionAlgo */ ,
    0 /* IPccAlgo::LineDetectionAlgoHelp */ ,
    0 /* IPccAlgo::LineDetectionExclude90Algo */ ,
    0 /* IPccAlgo::LineDetectionExclude90AlgoHelp */ ,
    0 /* IPccAlgo::TabDetectProfileAlgo */ ,
    0 /* IPccAlgo::TabDetectProfileAlgoHelp */ ,
    0 /* IPccAlgo::BlackDetectUseUpSizeAlgo */ ,
    0 /* IPccAlgo::BlackDetectUseUpSizeAlgoHelp */ ,
    0 /* IPccAlgo::DeformationDetectAlgo */ ,
    0 /* IPccAlgo::DeformationDetectAlgoHelp */ ,
    0 /* IPccAlgo::CircleRegionLocationAlgo1 */ ,
    0 /* IPccAlgo::CircleRegionLocationAlgo1Help */ ,
    0 /* IPccAlgo::BubbleDetectProfileAlgo1 */ ,
    0 /* IPccAlgo::BubbleDetectProfileAlgo1Help */ ,
    0 /* IPccAlgo::AnnularRotaDynThresholdAlgoBW */ ,
    0 /* IPccAlgo::AnnularRotaDynThresholdAlgoBWHelp */ ,
    0 /* IPccAlgo::SetCurrentTaskName */ ,
    0 /* IPccAlgo::SetSelfLearningParam */ ,
    0 /* IPccAlgo::SelectModelImage */ ,
    0 /* IPccAlgo::DeleteModelImage */ ,
    0 /* IPccAlgo::SaveResult */ ,
    0 /* IPccAlgo::AddModelImage */ ,
    0 /* IPccAlgo::GetSelfLearningResult */ ,
    0 /* IPccAlgo::ResetSelfLearningRegion */ ,
    0 /* IPccAlgo::GetSelfLearningRegion */ ,
    0 /* IPccAlgo::ResetSelfLearningProcedure */ ,
    0 /* IPccAlgo::SetSelfLearningTargetTaskName */
};


static const PRPC_STUB_FUNCTION IPccAlgo_table[] =
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
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IPccAlgoStubVtbl =
{
    &IID_IPccAlgo,
    &IPccAlgo_ServerInfo,
    75,
    &IPccAlgo_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ISelfLearning, ver. 0.0,
   GUID={0x7EE73789,0x334E,0x45C6,{0x86,0x05,0x02,0x67,0x55,0x66,0x42,0x82}} */

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
    PccAlgo__MIDL_ProcFormatString.Format,
    &ISelfLearning_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ISelfLearning_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    PccAlgo__MIDL_ProcFormatString.Format,
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
    PccAlgo__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _PccAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IPccAlgoProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ISelfLearningProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _PccAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IPccAlgoStubVtbl,
    ( CInterfaceStubVtbl *) &_ISelfLearningStubVtbl,
    0
};

PCInterfaceName const _PccAlgo_InterfaceNamesList[] = 
{
    "IPccAlgo",
    "ISelfLearning",
    0
};

const IID *  const _PccAlgo_BaseIIDList[] = 
{
    &IID_IDispatch,
    0,
    0
};


#define _PccAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _PccAlgo, pIID, n)

int __stdcall _PccAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _PccAlgo, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _PccAlgo, 2, *pIndex )
    
}

const ExtendedProxyFileInfo PccAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _PccAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _PccAlgo_StubVtblList,
    (const PCInterfaceName * ) & _PccAlgo_InterfaceNamesList,
    (const IID ** ) & _PccAlgo_BaseIIDList,
    & _PccAlgo_IID_Lookup, 
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

