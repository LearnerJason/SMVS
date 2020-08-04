

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:16:40 2019
 */
/* Compiler settings for PetAlgo.idl:
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


#include "PetAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _PetAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } PetAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _PetAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } PetAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _PetAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } PetAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const PetAlgo_MIDL_TYPE_FORMAT_STRING PetAlgo__MIDL_TypeFormatString;
extern const PetAlgo_MIDL_PROC_FORMAT_STRING PetAlgo__MIDL_ProcFormatString;
extern const PetAlgo_MIDL_EXPR_FORMAT_STRING PetAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPetAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IPetAlgo_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const PetAlgo_MIDL_PROC_FORMAT_STRING PetAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const PetAlgo_MIDL_TYPE_FORMAT_STRING PetAlgo__MIDL_TypeFormatString =
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


/* Object interface: IPetAlgo, ver. 0.0,
   GUID={0x6F83A499,0xC0BD,0x4878,{0x9C,0xB6,0x08,0x52,0x30,0xE3,0x77,0xDE}} */

#pragma code_seg(".orpc")
static const unsigned short IPetAlgo_FormatStringOffsetTable[] =
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
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO IPetAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    PetAlgo__MIDL_ProcFormatString.Format,
    &IPetAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IPetAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    PetAlgo__MIDL_ProcFormatString.Format,
    &IPetAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(83) _IPetAlgoProxyVtbl = 
{
    &IPetAlgo_ProxyInfo,
    &IID_IPetAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* IPetAlgo::SetCurrentImage */ ,
    0 /* IPetAlgo::GetRegion */ ,
    0 /* IPetAlgo::GetErrorRegion */ ,
    0 /* IPetAlgo::GetInternalStats */ ,
    0 /* IPetAlgo::ReloadExternalFile */ ,
    0 /* IPetAlgo::SetSelfLearningParam */ ,
    0 /* IPetAlgo::SelectModelImage */ ,
    0 /* IPetAlgo::DeleteModelImage */ ,
    0 /* IPetAlgo::SaveResult */ ,
    0 /* IPetAlgo::AddModelImage */ ,
    0 /* IPetAlgo::GetSelfLearningResult */ ,
    0 /* IPetAlgo::ResetSelfLearningRegion */ ,
    0 /* IPetAlgo::GetSelfLearningRegion */ ,
    0 /* IPetAlgo::ResetSelfLearningProcedure */ ,
    0 /* IPetAlgo::CapLocationAlgo */ ,
    0 /* IPetAlgo::CapLocationAlgoHelp */ ,
    0 /* IPetAlgo::LevelLocationAlgo */ ,
    0 /* IPetAlgo::LevelLocationAlgoHelp */ ,
    0 /* IPetAlgo::LevelDetectionAlgo */ ,
    0 /* IPetAlgo::LevelDetectionAlgoHelp */ ,
    0 /* IPetAlgo::CapSkewOrHighLocationAlgo */ ,
    0 /* IPetAlgo::CapSkewOrHighLocationAlgoHelp */ ,
    0 /* IPetAlgo::CapSkewDetectionAlgo */ ,
    0 /* IPetAlgo::CapSkewDetectionAlgoHelp */ ,
    0 /* IPetAlgo::CapHighDetectionAlgo */ ,
    0 /* IPetAlgo::CapHighDetectionAlgoHelp */ ,
    0 /* IPetAlgo::BottleNeckLocationAlgo */ ,
    0 /* IPetAlgo::BottleNeckLocationAlgoHelp */ ,
    0 /* IPetAlgo::BottleNeckDetectionAlgo */ ,
    0 /* IPetAlgo::BottleNeckDetectionAlgoHelp */ ,
    0 /* IPetAlgo::SecurityRingDetectionAlgo */ ,
    0 /* IPetAlgo::SecurityRingDetectionAlgoHelp */ ,
    0 /* IPetAlgo::LabelLocationAlgo */ ,
    0 /* IPetAlgo::LabelLocationAlgoHelp */ ,
    0 /* IPetAlgo::InverseLocationAlgo */ ,
    0 /* IPetAlgo::InverseLocationAlgoHelp */ ,
    0 /* IPetAlgo::InverseDetectionAlgo */ ,
    0 /* IPetAlgo::InverseDetectionAlgoHelp */ ,
    0 /* IPetAlgo::HighOrLowLabelLocationAlgo */ ,
    0 /* IPetAlgo::HighOrLowLabelLocationAlgoHelp */ ,
    0 /* IPetAlgo::HighOrLowLabelDetectionAlgo */ ,
    0 /* IPetAlgo::SlantLabelLocationAlgo */ ,
    0 /* IPetAlgo::SlantLabelLocationAlgoHelp */ ,
    0 /* IPetAlgo::SlantLabelDetectionAlgo */ ,
    0 /* IPetAlgo::SlantLabelDetectionAlgoHelp */ ,
    0 /* IPetAlgo::HighOrLowLabelDetectionAlgoHelp */ ,
    0 /* IPetAlgo::TotalBottlesLocationAlgoHelp */ ,
    0 /* IPetAlgo::TotalBottlesLocationAlgo */ ,
    0 /* IPetAlgo::BottlesLocationAlgoHelp */ ,
    0 /* IPetAlgo::BottlesLocationAlgo */ ,
    0 /* IPetAlgo::BottlesDetectionAlgoHelp */ ,
    0 /* IPetAlgo::BottlesDetectionAlgo */ ,
    0 /* IPetAlgo::PetCenterLocationAlgoHelp */ ,
    0 /* IPetAlgo::PetCenterLocationAlgo */ ,
    0 /* IPetAlgo::PetCircleRegionLocationAlgoHelp */ ,
    0 /* IPetAlgo::PetCircleRegionLocationAlgo */ ,
    0 /* IPetAlgo::PetCircleRegionDetectionAlgoHelp */ ,
    0 /* IPetAlgo::PetCircleRegionDetectionAlgo */ ,
    0 /* IPetAlgo::CodeLocationAlgoHelp */ ,
    0 /* IPetAlgo::CodeLocationAlgo */ ,
    0 /* IPetAlgo::CodingLocationAlgoHelp */ ,
    0 /* IPetAlgo::CodingLocationAlgo */ ,
    0 /* IPetAlgo::CodingDetectionAlgoHelp */ ,
    0 /* IPetAlgo::CodingDetectionAlgo */ ,
    0 /* IPetAlgo::KunRunShanLocationAlgoHelp */ ,
    0 /* IPetAlgo::KunRunShanLocationAlgo */ ,
    0 /* IPetAlgo::PetMatchingLocationAlgoHelp */ ,
    0 /* IPetAlgo::PetMatchingLocationAlgo */ ,
    0 /* IPetAlgo::PetMatchingParaAlgoHelp */ ,
    0 /* IPetAlgo::PetMatchingParaAlgo */ ,
    0 /* IPetAlgo::KunRunShanRegionLocationAlgoHelp */ ,
    0 /* IPetAlgo::KunRunShanRegionLocationAlgo */ ,
    0 /* IPetAlgo::PetMatchingDetectionAlgoHelp */ ,
    0 /* IPetAlgo::PetMatchingDetectionAlgo */ ,
    0 /* IPetAlgo::SetCurrentTaskName */ ,
    0 /* IPetAlgo::SetSelfLearningTargetTaskName */
};


static const PRPC_STUB_FUNCTION IPetAlgo_table[] =
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
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IPetAlgoStubVtbl =
{
    &IID_IPetAlgo,
    &IPetAlgo_ServerInfo,
    83,
    &IPetAlgo_table[-3],
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
    PetAlgo__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _PetAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IPetAlgoProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _PetAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IPetAlgoStubVtbl,
    0
};

PCInterfaceName const _PetAlgo_InterfaceNamesList[] = 
{
    "IPetAlgo",
    0
};

const IID *  const _PetAlgo_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _PetAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _PetAlgo, pIID, n)

int __stdcall _PetAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_PetAlgo_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo PetAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _PetAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _PetAlgo_StubVtblList,
    (const PCInterfaceName * ) & _PetAlgo_InterfaceNamesList,
    (const IID ** ) & _PetAlgo_BaseIIDList,
    & _PetAlgo_IID_Lookup, 
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

