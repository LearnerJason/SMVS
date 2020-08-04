

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:16:55 2019
 */
/* Compiler settings for CanAlgo.idl:
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


#include "CanAlgo_i.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _CanAlgo_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } CanAlgo_MIDL_TYPE_FORMAT_STRING;

typedef struct _CanAlgo_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } CanAlgo_MIDL_PROC_FORMAT_STRING;

typedef struct _CanAlgo_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } CanAlgo_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const CanAlgo_MIDL_TYPE_FORMAT_STRING CanAlgo__MIDL_TypeFormatString;
extern const CanAlgo_MIDL_PROC_FORMAT_STRING CanAlgo__MIDL_ProcFormatString;
extern const CanAlgo_MIDL_EXPR_FORMAT_STRING CanAlgo__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICanAlgo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ICanAlgo_ProxyInfo;



#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const CanAlgo_MIDL_PROC_FORMAT_STRING CanAlgo__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const CanAlgo_MIDL_TYPE_FORMAT_STRING CanAlgo__MIDL_TypeFormatString =
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


/* Object interface: ICanAlgo, ver. 0.0,
   GUID={0x6D2F69FF,0x2AA7,0x414B,{0x8E,0x88,0x86,0xC3,0x41,0x98,0x39,0xD4}} */

#pragma code_seg(".orpc")
static const unsigned short ICanAlgo_FormatStringOffsetTable[] =
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
    (unsigned short) -1
    };

static const MIDL_STUBLESS_PROXY_INFO ICanAlgo_ProxyInfo =
    {
    &Object_StubDesc,
    CanAlgo__MIDL_ProcFormatString.Format,
    &ICanAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO ICanAlgo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    CanAlgo__MIDL_ProcFormatString.Format,
    &ICanAlgo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(153) _ICanAlgoProxyVtbl = 
{
    &ICanAlgo_ProxyInfo,
    &IID_ICanAlgo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* ICanAlgo::SetCurrentImage */ ,
    0 /* ICanAlgo::GetRegion */ ,
    0 /* ICanAlgo::GetErrorRegion */ ,
    0 /* ICanAlgo::GetWhiteErrorRegion */ ,
    0 /* ICanAlgo::GetInterImgs */ ,
    0 /* ICanAlgo::GetInternalStats */ ,
    0 /* ICanAlgo::ReloadExternalFile */ ,
    0 /* ICanAlgo::SetCurrentTaskName */ ,
    0 /* ICanAlgo::SetSelfLearningParam */ ,
    0 /* ICanAlgo::SelectModelImage */ ,
    0 /* ICanAlgo::DeleteModelImage */ ,
    0 /* ICanAlgo::SaveResult */ ,
    0 /* ICanAlgo::AddModelImage */ ,
    0 /* ICanAlgo::GetSelfLearningResult */ ,
    0 /* ICanAlgo::ResetSelfLearningRegion */ ,
    0 /* ICanAlgo::GetSelfLearningRegion */ ,
    0 /* ICanAlgo::ResetSelfLearningProcedure */ ,
    0 /* ICanAlgo::SetSelfLearningTargetTaskName */ ,
    0 /* ICanAlgo::LocationAlgo */ ,
    0 /* ICanAlgo::LocationAlgoHelp */ ,
    0 /* ICanAlgo::CenterLocationAlgoMultImg */ ,
    0 /* ICanAlgo::CenterLocationAlgoMultImgHelp */ ,
    0 /* ICanAlgo::CenterLocationAlgo */ ,
    0 /* ICanAlgo::CenterLocationAlgoHelp */ ,
    0 /* ICanAlgo::CenterLocationAlgo1 */ ,
    0 /* ICanAlgo::CenterLocationAlgo1Help */ ,
    0 /* ICanAlgo::CenterLocationAlgoHS */ ,
    0 /* ICanAlgo::CenterLocationAlgoHSHelp */ ,
    0 /* ICanAlgo::CenterRelocationAlgoHS */ ,
    0 /* ICanAlgo::CenterRelocationAlgoHSHelp */ ,
    0 /* ICanAlgo::CenterRelocationAlgo */ ,
    0 /* ICanAlgo::CenterRelocationAlgoHelp */ ,
    0 /* ICanAlgo::CenterLocationAlgoUsePointFitting */ ,
    0 /* ICanAlgo::CenterLocationAlgoUsePointFittingHelp */ ,
    0 /* ICanAlgo::CenterLocationAlgoUsePointFittingColor */ ,
    0 /* ICanAlgo::CenterLocationAlgoUsePointFittingColorHelp */ ,
    0 /* ICanAlgo::RectangleLeftLocationAlgo */ ,
    0 /* ICanAlgo::RectangleLeftLocationAlgoHelp */ ,
    0 /* ICanAlgo::CircleRegionLocationAlgo */ ,
    0 /* ICanAlgo::CircleRegionLocationAlgoHelp */ ,
    0 /* ICanAlgo::CircleRegionLocationAlgo1 */ ,
    0 /* ICanAlgo::CircleRegionLocationAlgo1Help */ ,
    0 /* ICanAlgo::SectorRegionLocationAlgo */ ,
    0 /* ICanAlgo::SectorRegionLocationAlgoHelp */ ,
    0 /* ICanAlgo::SectorRegionLocationAlgo1 */ ,
    0 /* ICanAlgo::SectorRegionLocationAlgo1Help */ ,
    0 /* ICanAlgo::WeldAngleDetectAlgo */ ,
    0 /* ICanAlgo::WeldAngleDetectAlgoHelp */ ,
    0 /* ICanAlgo::WeldAngleDetectGrayAlgo */ ,
    0 /* ICanAlgo::WeldAngleDetectGrayAlgoHelp */ ,
    0 /* ICanAlgo::WeldAngleDetectWhiteGlueAlgo */ ,
    0 /* ICanAlgo::WeldAngleDetectWhiteGlueAlgoHelp */ ,
    0 /* ICanAlgo::WeldAngleDetectGlueAlgo */ ,
    0 /* ICanAlgo::WeldAngleDetectGlueAlgoHelp */ ,
    0 /* ICanAlgo::CurlDetectAlgo */ ,
    0 /* ICanAlgo::CurlDetectAlgoHelp */ ,
    0 /* ICanAlgo::CurlMeanGrayDetectAlgo */ ,
    0 /* ICanAlgo::CurlMeanGrayDetectAlgoHelp */ ,
    0 /* ICanAlgo::CurlOptimizeAlgo */ ,
    0 /* ICanAlgo::CurlOptimizeAlgoHelp */ ,
    0 /* ICanAlgo::AnnularRotaDynThresholdAlgo */ ,
    0 /* ICanAlgo::AnnularRotaDynThresholdAlgoHelp */ ,
    0 /* ICanAlgo::AnnularRotaDynThresholdAlgoBW */ ,
    0 /* ICanAlgo::AnnularRotaDynThresholdAlgoBWHelp */ ,
    0 /* ICanAlgo::BlackDetectAlgo */ ,
    0 /* ICanAlgo::BlackDetectAlgoHelp */ ,
    0 /* ICanAlgo::DynThresholdAlgo */ ,
    0 /* ICanAlgo::DynThresholdAlgoHelp */ ,
    0 /* ICanAlgo::DynThresholdAvoidReflectionAlgo */ ,
    0 /* ICanAlgo::DynThresholdAvoidReflectionAlgoHelp */ ,
    0 /* ICanAlgo::GLineAlgo */ ,
    0 /* ICanAlgo::GLineAlgoHelp */ ,
    0 /* ICanAlgo::EdgeAlgo */ ,
    0 /* ICanAlgo::EdgeAlgoHelp */ ,
    0 /* ICanAlgo::CurlNoCompoundAlgo */ ,
    0 /* ICanAlgo::CurlNoCompoundAlgoHelp */ ,
    0 /* ICanAlgo::BodyDirectionDetectAlgo */ ,
    0 /* ICanAlgo::BodyDirectionDetectAlgoHelp */ ,
    0 /* ICanAlgo::BodyDirectionDetectMinScoreAlgo */ ,
    0 /* ICanAlgo::BodyDirectionDetectMinScoreAlgoHelp */ ,
    0 /* ICanAlgo::DoubleEndRegionLocationAlgo */ ,
    0 /* ICanAlgo::DoubleEndRegionLocationAlgoHelp */ ,
    0 /* ICanAlgo::DoubleEndDetectAlgo */ ,
    0 /* ICanAlgo::DoubleEndDetectAlgoHelp */ ,
    0 /* ICanAlgo::BodyDirectionDetectVecDiffAlgo */ ,
    0 /* ICanAlgo::BodyDirectionDetectVecDiffAlgoHelp */ ,
    0 /* ICanAlgo::BodyDirectionVecDiffLocationAlgo */ ,
    0 /* ICanAlgo::BodyDirectionVecDiffLocationAlgoHelp */ ,
    0 /* ICanAlgo::BoundaryRegionLocationAlgo */ ,
    0 /* ICanAlgo::BoundaryRegionLocationAlgoHelp */ ,
    0 /* ICanAlgo::WeldWhiteGlueRegExtractAlgo */ ,
    0 /* ICanAlgo::WeldWhiteGlueRegExtractAlgoHelp */ ,
    0 /* ICanAlgo::WeldRegSelectAlgo */ ,
    0 /* ICanAlgo::WeldRegSelectAlgoHelp */ ,
    0 /* ICanAlgo::CurlOptimizeForColorAlgo */ ,
    0 /* ICanAlgo::CurlOptimizeForColorAlgoHelp */ ,
    0 /* ICanAlgo::CurlDetectForColorAlgo */ ,
    0 /* ICanAlgo::CurlDetectForColorAlgoHelp */ ,
    0 /* ICanAlgo::CurlMeanGrayDetectForColorAlgo */ ,
    0 /* ICanAlgo::CurlMeanGrayDetectForColorAlgoHelp */ ,
    0 /* ICanAlgo::AnnularRotaDynThresholdForColorAlgo */ ,
    0 /* ICanAlgo::AnnularRotaDynThresholdForColorAlgoHelp */ ,
    0 /* ICanAlgo::CurlNoCompoundForColorAlgo */ ,
    0 /* ICanAlgo::CurlNoCompoundForColorAlgoHelp */ ,
    0 /* ICanAlgo::DynThresholdForColorAlgo */ ,
    0 /* ICanAlgo::DynThresholdForColorAlgoHelp */ ,
    0 /* ICanAlgo::EdgeForColorAlgo */ ,
    0 /* ICanAlgo::EdgeForColorAlgoHelp */ ,
    0 /* ICanAlgo::DerivateGaussDetectAlgo */ ,
    0 /* ICanAlgo::DerivateGaussDetectAlgoHelp */ ,
    0 /* ICanAlgo::VarThreshDarkDetectAlgo */ ,
    0 /* ICanAlgo::VarThreshDarkDetectAlgoHelp */ ,
    0 /* ICanAlgo::CurlDetectAlgo1 */ ,
    0 /* ICanAlgo::CurlDetectAlgo1Help */ ,
    0 /* ICanAlgo::CenterReLocationAlgo2 */ ,
    0 /* ICanAlgo::CenterReLocationAlgo2Help */ ,
    0 /* ICanAlgo::RingRegionDynThresholdAlgoBW */ ,
    0 /* ICanAlgo::RingRegionDynThresholdAlgoBWHelp */ ,
    0 /* ICanAlgo::BoundaryRegionLocationAlgo1 */ ,
    0 /* ICanAlgo::BoundaryRegionLocationAlgo1Help */ ,
    0 /* ICanAlgo::CenterLocationAlgoM */ ,
    0 /* ICanAlgo_CenterLocationAlgoMHelp_Proxy */ ,
    0 /* ICanAlgo_RectangleLocationAlgo_Proxy */ ,
    0 /* ICanAlgo_RectangleLocationAlgoHelp_Proxy */ ,
    0 /* ICanAlgo_BaseLineDetectAlgo_Proxy */ ,
    0 /* ICanAlgo_BaseLineDetectAlgoHelp_Proxy */ ,
    0 /* ICanAlgo_RectangleLocation2Algo_Proxy */ ,
    0 /* ICanAlgo_RectangleLocation2AlgoHelp_Proxy */ ,
    0 /* ICanAlgo_HeightMeasureAlgo_Proxy */ ,
    0 /* ICanAlgo_HeightMeasureAlgoHelp_Proxy */ ,
    0 /* ICanAlgo_DiameterMeasureAlgo_Proxy */ ,
    0 /* ICanAlgo_DiameterMeasureAlgoHelp_Proxy */ ,
    0 /* ICanAlgo_InnerDiameterMeasureAlgo_Proxy */ ,
    0 /* ICanAlgo_InnerDiameterMeasureAlgoHelp_Proxy */ ,
    0 /* ICanAlgo_SideCenterLocationAlgo_Proxy */ ,
    0 /* ICanAlgo_SideCenterLocationAlgoHelp_Proxy */ ,
    0 /* ICanAlgo_SideRegionLocationAlgo_Proxy */ ,
    0 /* ICanAlgo_SideRegionLocationAlgoHelp_Proxy */ ,
    0 /* ICanAlgo_CenterLocationAlgoUsePointFitting1_Proxy */ ,
    0 /* ICanAlgo_CenterLocationAlgoUsePointFitting1Help_Proxy */ ,
    0 /* ICanAlgo_WeldRegionSelection_Proxy */ ,
    0 /* ICanAlgo_WeldRegionSelectionHelp_Proxy */ ,
    0 /* ICanAlgo_WeldRegionExtractionAlgo_Proxy */ ,
    0 /* ICanAlgo_WeldRegionExtractionAlgoHelp_Proxy */ ,
    0 /* ICanAlgo_SectorRotaDynThresholdAlgo_Proxy */ ,
    0 /* ICanAlgo_SectorRotaDynThresholdAlgoHelp_Proxy */
};


static const PRPC_STUB_FUNCTION ICanAlgo_table[] =
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
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ICanAlgoStubVtbl =
{
    &IID_ICanAlgo,
    &ICanAlgo_ServerInfo,
    153,
    &ICanAlgo_table[-3],
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
    CanAlgo__MIDL_TypeFormatString.Format,
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

const CInterfaceProxyVtbl * const _CanAlgo_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ICanAlgoProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _CanAlgo_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ICanAlgoStubVtbl,
    0
};

PCInterfaceName const _CanAlgo_InterfaceNamesList[] = 
{
    "ICanAlgo",
    0
};

const IID *  const _CanAlgo_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _CanAlgo_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _CanAlgo, pIID, n)

int __stdcall _CanAlgo_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_CanAlgo_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo CanAlgo_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _CanAlgo_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _CanAlgo_StubVtblList,
    (const PCInterfaceName * ) & _CanAlgo_InterfaceNamesList,
    (const IID ** ) & _CanAlgo_BaseIIDList,
    & _CanAlgo_IID_Lookup, 
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

