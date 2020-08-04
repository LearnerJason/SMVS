

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Oct 14 09:34:10 2019
 */
/* Compiler settings for CapacitorAlgo.idl:
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


#include "CapacitorAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _CapacitorAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } CapacitorAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _CapacitorAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } CapacitorAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _CapacitorAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } CapacitorAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const CapacitorAlgo_MIDL_TYPE_FORMAT_STRING CapacitorAlgo__MIDL_TypeFormatString;
extern const CapacitorAlgo_MIDL_PROC_FORMAT_STRING CapacitorAlgo__MIDL_ProcFormatString;
extern const CapacitorAlgo_MIDL_EXPR_FORMAT_STRING CapacitorAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICapacitorAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ICapacitorAlgo_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const CapacitorAlgo_MIDL_PROC_FORMAT_STRING CapacitorAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const CapacitorAlgo_MIDL_TYPE_FORMAT_STRING CapacitorAlgo__MIDL_TypeFormatString =
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


/* Object interface: ICapacitorAlgo, ver. 0.0,
   GUID={0x72725E53,0x9909,0x4658,{0xA2,0xA1,0xDA,0xAA,0xF5,0xD2,0x90,0xFA}} */

#pragma code_seg(".orpc")
static const unsigned short ICapacitorAlgo_FormatStringOffsetTable[] =
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
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO ICapacitorAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    CapacitorAlgo__MIDL_ProcFormatString.Format,
    &ICapacitorAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ICapacitorAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    CapacitorAlgo__MIDL_ProcFormatString.Format,
    &ICapacitorAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(77) _ICapacitorAlgoProxyVtbl = 
{
    &ICapacitorAlgo_ProxyInfo,
    &IID_ICapacitorAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* ICapacitorAlgo::SetCurrentImage */ ,
    0 /* ICapacitorAlgo::GetRegion */ ,
    0 /* ICapacitorAlgo::GetErrorRegion */ ,
    0 /* ICapacitorAlgo::GetInternalStats */ ,
    0 /* ICapacitorAlgo::ReloadExternalFile */ ,
    0 /* ICapacitorAlgo::SetCurrentTaskName */ ,
    0 /* ICapacitorAlgo::CenterLocationAlgo */ ,
    0 /* ICapacitorAlgo::CenterLocationAlgoHelp */ ,
    0 /* ICapacitorAlgo::CenterLocationAlgo1 */ ,
    0 /* ICapacitorAlgo::CenterLocationAlgo1Help */ ,
    0 /* ICapacitorAlgo::CenterLocationAlgoHS */ ,
    0 /* ICapacitorAlgo::CenterLocationAlgoHSHelp */ ,
    0 /* ICapacitorAlgo::CenterRelocationAlgoHS */ ,
    0 /* ICapacitorAlgo::CenterRelocationAlgoHSHelp */ ,
    0 /* ICapacitorAlgo::CenterLocationHeightAlgo */ ,
    0 /* ICapacitorAlgo::CenterLocationHeightAlgoHelp */ ,
    0 /* ICapacitorAlgo::CircleRegionLocationAlgo */ ,
    0 /* ICapacitorAlgo::CircleRegionLocationAlgoHelp */ ,
    0 /* ICapacitorAlgo::CircleRegionLocationAlgo1 */ ,
    0 /* ICapacitorAlgo::CircleRegionLocationAlgo1Help */ ,
    0 /* ICapacitorAlgo::RectangleRegionLocationAlgo */ ,
    0 /* ICapacitorAlgo::RectangleRegionLocationAlgoHelp */ ,
    0 /* ICapacitorAlgo::RectangleFixedWidthLocationAlgo */ ,
    0 /* ICapacitorAlgo::RectangleFixedWidthLocationAlgoHelp */ ,
    0 /* ICapacitorAlgo::BubbleDetectProfileAlgo */ ,
    0 /* ICapacitorAlgo::BubbleDetectProfileAlgoHelp */ ,
    0 /* ICapacitorAlgo::CurlDetectAlgo */ ,
    0 /* ICapacitorAlgo::CurlDetectAlgoHelp */ ,
    0 /* ICapacitorAlgo::CurlMeanGrayDetectAlgo */ ,
    0 /* ICapacitorAlgo::CurlMeanGrayDetectAlgoHelp */ ,
    0 /* ICapacitorAlgo::CurlOptimizeAlgo */ ,
    0 /* ICapacitorAlgo::CurlOptimizeAlgoHelp */ ,
    0 /* ICapacitorAlgo::AnnularRotaDynThresholdAlgo */ ,
    0 /* ICapacitorAlgo::AnnularRotaDynThresholdAlgoHelp */ ,
    0 /* ICapacitorAlgo::GLineAlgo */ ,
    0 /* ICapacitorAlgo::GLineAlgoHelp */ ,
    0 /* ICapacitorAlgo::LineDetectionAlgo */ ,
    0 /* ICapacitorAlgo::LineDetectionAlgoHelp */ ,
    0 /* ICapacitorAlgo::EdgeAlgo */ ,
    0 /* ICapacitorAlgo::EdgeAlgoHelp */ ,
    0 /* ICapacitorAlgo::CurlNoCompoundAlgo */ ,
    0 /* ICapacitorAlgo::CurlNoCompoundAlgoHelp */ ,
    0 /* ICapacitorAlgo::DynThresholdAlgo */ ,
    0 /* ICapacitorAlgo::DynThresholdAlgoHelp */ ,
    0 /* ICapacitorAlgo::DynThresholdAvoidReflectionAlgo */ ,
    0 /* ICapacitorAlgo::DynThresholdAvoidReflectionAlgoHelp */ ,
    0 /* ICapacitorAlgo::HeightDiffDetectionAlgo */ ,
    0 /* ICapacitorAlgo::HeightDiffDetectionAlgoHelp */ ,
    0 /* ICapacitorAlgo::SlotDetectAlgo1 */ ,
    0 /* ICapacitorAlgo::SlotDetectAlgo1Help */ ,
    0 /* ICapacitorAlgo::GSurfaceAlgo */ ,
    0 /* ICapacitorAlgo::GSurfaceAlgoHelp */ ,
    0 /* ICapacitorAlgo::TabDetectProfileAlgo */ ,
    0 /* ICapacitorAlgo::TabDetectProfileAlgoHelp */ ,
    0 /* ICapacitorAlgo::GapDetectAlgoHelp */ ,
    0 /* ICapacitorAlgo::GapDetectAlgo */ ,
    0 /* ICapacitorAlgo::DynThresholdAlgoWhite */ ,
    0 /* ICapacitorAlgo::DynThresholdAlgoWhiteHelp */ ,
    0 /* ICapacitorAlgo::CCenterLocationAlgo */ ,
    0 /* ICapacitorAlgo::CCenterLocationAlgoHelp */ ,
    0 /* ICapacitorAlgo::SetSelfLearningParam */ ,
    0 /* ICapacitorAlgo::SelectModelImage */ ,
    0 /* ICapacitorAlgo::DeleteModelImage */ ,
    0 /* ICapacitorAlgo::SaveResult */ ,
    0 /* ICapacitorAlgo::AddModelImage */ ,
    0 /* ICapacitorAlgo::GetSelfLearningResult */ ,
    0 /* ICapacitorAlgo::ResetSelfLearningRegion */ ,
    0 /* ICapacitorAlgo::GetSelfLearningRegion */ ,
    0 /* ICapacitorAlgo::ResetSelfLearningProcedure */ ,
    0 /* ICapacitorAlgo::SetSelfLearningTargetTaskName */
};


static const PRPC_STUB_FUNCTION ICapacitorAlgo_table[] =
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
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ICapacitorAlgoStubVtbl =
{
    &IID_ICapacitorAlgo,
    &ICapacitorAlgo_ServerInfo,
    77,
    &ICapacitorAlgo_table[-3],
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
    CapacitorAlgo__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _CapacitorAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ICapacitorAlgoProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _CapacitorAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ICapacitorAlgoStubVtbl,
    0
};

PCInterfaceName const _CapacitorAlgo_InterfaceNamesList[] = 
{
    "ICapacitorAlgo",
    0
};

const IID *  const _CapacitorAlgo_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _CapacitorAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _CapacitorAlgo, pIID, n)

int __stdcall _CapacitorAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_CapacitorAlgo_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo CapacitorAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _CapacitorAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _CapacitorAlgo_StubVtblList,
    (const PCInterfaceName * ) & _CapacitorAlgo_InterfaceNamesList,
    (const IID ** ) & _CapacitorAlgo_BaseIIDList,
    & _CapacitorAlgo_IID_Lookup, 
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

