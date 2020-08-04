

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PetAlgo_i_h__
#define __PetAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPetAlgo_FWD_DEFINED__
#define __IPetAlgo_FWD_DEFINED__
typedef interface IPetAlgo IPetAlgo;
#endif 	/* __IPetAlgo_FWD_DEFINED__ */


#ifndef __PetAlgo_FWD_DEFINED__
#define __PetAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class PetAlgo PetAlgo;
#else
typedef struct PetAlgo PetAlgo;
#endif /* __cplusplus */

#endif 	/* __PetAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IPetAlgo_INTERFACE_DEFINED__
#define __IPetAlgo_INTERFACE_DEFINED__

/* interface IPetAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPetAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6F83A499-C0BD-4878-9CB6-085230E377DE")
    IPetAlgo : public IDispatch
    {
    public:
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentImage( 
            /* [in] */ LONG *pImg) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetRegion( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetErrorRegion( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetInternalStats( 
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ReloadExternalFile( 
            /* [in] */ BSTR *bstrPath) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetSelfLearningParam( 
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SelectModelImage( 
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DeleteModelImage( 
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SaveResult( 
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AddModelImage( 
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetSelfLearningResult( 
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ResetSelfLearningRegion( 
            /* [in] */ BSTR *strRegionExtractPara) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetSelfLearningRegion( 
            /* [out][in] */ LONG *pRgn) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ResetSelfLearningProcedure( void) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CapLocationAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pAreaThredHigh) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CapLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LevelLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLevelRef,
            /* [in] */ VARIANT *pLevelRange,
            /* [in] */ VARIANT *pLevelWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LevelLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LevelDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThr,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pGrayThred,
            /* [retval][out] */ VARIANT *pAreaThred) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LevelDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CapSkewOrHighLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pCapLevelDiff,
            /* [in] */ VARIANT *pCapRectDiff,
            /* [in] */ VARIANT *pCapRectHeight,
            /* [in] */ VARIANT *pCapRectWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CapSkewOrHighLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CapSkewDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pHeightDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CapSkewDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CapHighDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pHeightDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CapHighDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottleNeckLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLevelNeckDiff,
            /* [in] */ VARIANT *pNeckWidth,
            /* [in] */ VARIANT *pNeckHeight) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottleNeckLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottleNeckDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottleNeckDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SecurityRingDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SecurityRingDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LabelLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pSmoothFactor,
            /* [in] */ VARIANT *pAreaThred) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LabelLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InverseLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRectHeight,
            /* [in] */ VARIANT *pRectWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InverseLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InverseDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [retval][out] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InverseDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HighOrLowLabelLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNormalLevel) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HighOrLowLabelLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HighOrLowLabelDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pHeightDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SlantLabelLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRectHeight,
            /* [in] */ VARIANT *pRectWidth,
            /* [in] */ VARIANT *pCenterDiff,
            /* [in] */ VARIANT *pLabelRectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SlantLabelLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SlantLabelDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pHeightDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SlantLabelDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HighOrLowLabelDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalBottlesLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalBottlesLocationAlgo( 
            /* [in] */ VARIANT *rgnId) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottlesLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottlesLocationAlgo( 
            /* [in] */ VARIANT *rgnId) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottlesDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottlesDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pAreaSelect,
            /* [retval][out] */ VARIANT *pNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetCenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetCenterLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetCircleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetCircleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetCircleRegionDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetCircleRegionDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pSelectArea,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CodeLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CodeLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pWidth,
            /* [in] */ VARIANT *pHeight,
            /* [in] */ VARIANT *pArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CodingLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CodingLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pDilationCircle,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pWidth,
            /* [in] */ VARIANT *pHeight) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CodingDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CodingDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE KunRunShanLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE KunRunShanLocationAlgo( 
            /* [in] */ VARIANT *rgnId) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetMatchingLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetMatchingLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRow1,
            /* [in] */ VARIANT *pColumn1,
            /* [in] */ VARIANT *pRow2,
            /* [in] */ VARIANT *pColumn2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetMatchingParaAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetMatchingParaAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE KunRunShanRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE KunRunShanRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRow1Length,
            /* [in] */ VARIANT *pColumn1Length,
            /* [in] */ VARIANT *pRow2Length,
            /* [in] */ VARIANT *pColumn2Length) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetMatchingDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PetMatchingDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pMask,
            /* [in] */ VARIANT *pSelectMin,
            /* [retval][out] */ VARIANT *pNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetSelfLearningTargetTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPetAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPetAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPetAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPetAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPetAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPetAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPetAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPetAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            IPetAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            IPetAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            IPetAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            IPetAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            IPetAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            IPetAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            IPetAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            IPetAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            IPetAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            IPetAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            IPetAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            IPetAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            IPetAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            IPetAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CapLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pAreaThredHigh);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CapLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LevelLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLevelRef,
            /* [in] */ VARIANT *pLevelRange,
            /* [in] */ VARIANT *pLevelWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LevelLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LevelDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThr,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pGrayThred,
            /* [retval][out] */ VARIANT *pAreaThred);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LevelDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CapSkewOrHighLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pCapLevelDiff,
            /* [in] */ VARIANT *pCapRectDiff,
            /* [in] */ VARIANT *pCapRectHeight,
            /* [in] */ VARIANT *pCapRectWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CapSkewOrHighLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CapSkewDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pHeightDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CapSkewDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CapHighDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pHeightDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CapHighDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottleNeckLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLevelNeckDiff,
            /* [in] */ VARIANT *pNeckWidth,
            /* [in] */ VARIANT *pNeckHeight);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottleNeckLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottleNeckDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottleNeckDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SecurityRingDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SecurityRingDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LabelLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pSmoothFactor,
            /* [in] */ VARIANT *pAreaThred);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LabelLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InverseLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRectHeight,
            /* [in] */ VARIANT *pRectWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InverseLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InverseDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [retval][out] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InverseDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HighOrLowLabelLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNormalLevel);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HighOrLowLabelLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HighOrLowLabelDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pHeightDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SlantLabelLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRectHeight,
            /* [in] */ VARIANT *pRectWidth,
            /* [in] */ VARIANT *pCenterDiff,
            /* [in] */ VARIANT *pLabelRectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SlantLabelLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SlantLabelDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pHeightDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SlantLabelDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HighOrLowLabelDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalBottlesLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalBottlesLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottlesLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottlesLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottlesDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottlesDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pAreaSelect,
            /* [retval][out] */ VARIANT *pNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetCenterLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetCenterLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetCircleRegionLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetCircleRegionLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetCircleRegionDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetCircleRegionDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pSelectArea,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CodeLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CodeLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pWidth,
            /* [in] */ VARIANT *pHeight,
            /* [in] */ VARIANT *pArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CodingLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CodingLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pDilationCircle,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pWidth,
            /* [in] */ VARIANT *pHeight);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CodingDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CodingDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *KunRunShanLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *KunRunShanLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetMatchingLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetMatchingLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRow1,
            /* [in] */ VARIANT *pColumn1,
            /* [in] */ VARIANT *pRow2,
            /* [in] */ VARIANT *pColumn2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetMatchingParaAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetMatchingParaAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *KunRunShanRegionLocationAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *KunRunShanRegionLocationAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRow1Length,
            /* [in] */ VARIANT *pColumn1Length,
            /* [in] */ VARIANT *pRow2Length,
            /* [in] */ VARIANT *pColumn2Length);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetMatchingDetectionAlgoHelp )( 
            IPetAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PetMatchingDetectionAlgo )( 
            IPetAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pMask,
            /* [in] */ VARIANT *pSelectMin,
            /* [retval][out] */ VARIANT *pNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            IPetAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            IPetAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        END_INTERFACE
    } IPetAlgoVtbl;

    interface IPetAlgo
    {
        CONST_VTBL struct IPetAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPetAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPetAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPetAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPetAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPetAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPetAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPetAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPetAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define IPetAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define IPetAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define IPetAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define IPetAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define IPetAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define IPetAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define IPetAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define IPetAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define IPetAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define IPetAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define IPetAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define IPetAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define IPetAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define IPetAlgo_CapLocationAlgo(This,rgnID,pGrayThred,pAreaThred,pAreaThredHigh)	\
    ( (This)->lpVtbl -> CapLocationAlgo(This,rgnID,pGrayThred,pAreaThred,pAreaThredHigh) ) 

#define IPetAlgo_CapLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CapLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_LevelLocationAlgo(This,rgnId,pLevelRef,pLevelRange,pLevelWidth)	\
    ( (This)->lpVtbl -> LevelLocationAlgo(This,rgnId,pLevelRef,pLevelRange,pLevelWidth) ) 

#define IPetAlgo_LevelLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LevelLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_LevelDetectionAlgo(This,rgnId,pDynThr,pAreaLimit,pGrayThred,pAreaThred)	\
    ( (This)->lpVtbl -> LevelDetectionAlgo(This,rgnId,pDynThr,pAreaLimit,pGrayThred,pAreaThred) ) 

#define IPetAlgo_LevelDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LevelDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_CapSkewOrHighLocationAlgo(This,rgnId,pCapLevelDiff,pCapRectDiff,pCapRectHeight,pCapRectWidth)	\
    ( (This)->lpVtbl -> CapSkewOrHighLocationAlgo(This,rgnId,pCapLevelDiff,pCapRectDiff,pCapRectHeight,pCapRectWidth) ) 

#define IPetAlgo_CapSkewOrHighLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CapSkewOrHighLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_CapSkewDetectionAlgo(This,rgnId,pHeightDiff)	\
    ( (This)->lpVtbl -> CapSkewDetectionAlgo(This,rgnId,pHeightDiff) ) 

#define IPetAlgo_CapSkewDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CapSkewDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_CapHighDetectionAlgo(This,rgnId,pHeightDiff)	\
    ( (This)->lpVtbl -> CapHighDetectionAlgo(This,rgnId,pHeightDiff) ) 

#define IPetAlgo_CapHighDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CapHighDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_BottleNeckLocationAlgo(This,rgnId,pLevelNeckDiff,pNeckWidth,pNeckHeight)	\
    ( (This)->lpVtbl -> BottleNeckLocationAlgo(This,rgnId,pLevelNeckDiff,pNeckWidth,pNeckHeight) ) 

#define IPetAlgo_BottleNeckLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottleNeckLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_BottleNeckDetectionAlgo(This,rgnId,pGrayThred,pAreaLimit,pDefectArea)	\
    ( (This)->lpVtbl -> BottleNeckDetectionAlgo(This,rgnId,pGrayThred,pAreaLimit,pDefectArea) ) 

#define IPetAlgo_BottleNeckDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottleNeckDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_SecurityRingDetectionAlgo(This,rgnId,pGrayThred,pAreaLimit,pDefectArea)	\
    ( (This)->lpVtbl -> SecurityRingDetectionAlgo(This,rgnId,pGrayThred,pAreaLimit,pDefectArea) ) 

#define IPetAlgo_SecurityRingDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SecurityRingDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_LabelLocationAlgo(This,rgnId,pGrayThred,pSmoothFactor,pAreaThred)	\
    ( (This)->lpVtbl -> LabelLocationAlgo(This,rgnId,pGrayThred,pSmoothFactor,pAreaThred) ) 

#define IPetAlgo_LabelLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LabelLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_InverseLocationAlgo(This,rgnId,pRectHeight,pRectWidth)	\
    ( (This)->lpVtbl -> InverseLocationAlgo(This,rgnId,pRectHeight,pRectWidth) ) 

#define IPetAlgo_InverseLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> InverseLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_InverseDetectionAlgo(This,rgnId,pGreediness,pMinScore)	\
    ( (This)->lpVtbl -> InverseDetectionAlgo(This,rgnId,pGreediness,pMinScore) ) 

#define IPetAlgo_InverseDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> InverseDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_HighOrLowLabelLocationAlgo(This,rgnId,pNormalLevel)	\
    ( (This)->lpVtbl -> HighOrLowLabelLocationAlgo(This,rgnId,pNormalLevel) ) 

#define IPetAlgo_HighOrLowLabelLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> HighOrLowLabelLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_HighOrLowLabelDetectionAlgo(This,rgnId,pHeightDiff)	\
    ( (This)->lpVtbl -> HighOrLowLabelDetectionAlgo(This,rgnId,pHeightDiff) ) 

#define IPetAlgo_SlantLabelLocationAlgo(This,rgnId,pRectHeight,pRectWidth,pCenterDiff,pLabelRectDiff)	\
    ( (This)->lpVtbl -> SlantLabelLocationAlgo(This,rgnId,pRectHeight,pRectWidth,pCenterDiff,pLabelRectDiff) ) 

#define IPetAlgo_SlantLabelLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SlantLabelLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_SlantLabelDetectionAlgo(This,rgnId,pHeightDiff)	\
    ( (This)->lpVtbl -> SlantLabelDetectionAlgo(This,rgnId,pHeightDiff) ) 

#define IPetAlgo_SlantLabelDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SlantLabelDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_HighOrLowLabelDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> HighOrLowLabelDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_TotalBottlesLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TotalBottlesLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_TotalBottlesLocationAlgo(This,rgnId)	\
    ( (This)->lpVtbl -> TotalBottlesLocationAlgo(This,rgnId) ) 

#define IPetAlgo_BottlesLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottlesLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_BottlesLocationAlgo(This,rgnId)	\
    ( (This)->lpVtbl -> BottlesLocationAlgo(This,rgnId) ) 

#define IPetAlgo_BottlesDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottlesDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_BottlesDetectionAlgo(This,rgnId,pThreshold,pAreaSelect,pNumber)	\
    ( (This)->lpVtbl -> BottlesDetectionAlgo(This,rgnId,pThreshold,pAreaSelect,pNumber) ) 

#define IPetAlgo_PetCenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PetCenterLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_PetCenterLocationAlgo(This,rgnId,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> PetCenterLocationAlgo(This,rgnId,pRadius,pMinScore) ) 

#define IPetAlgo_PetCircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PetCircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_PetCircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> PetCircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IPetAlgo_PetCircleRegionDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PetCircleRegionDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_PetCircleRegionDetectionAlgo(This,rgnId,pDynThreshold,pSelectArea,pDefectArea)	\
    ( (This)->lpVtbl -> PetCircleRegionDetectionAlgo(This,rgnId,pDynThreshold,pSelectArea,pDefectArea) ) 

#define IPetAlgo_CodeLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CodeLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_CodeLocationAlgo(This,rgnId,pThreshold,pWidth,pHeight,pArea)	\
    ( (This)->lpVtbl -> CodeLocationAlgo(This,rgnId,pThreshold,pWidth,pHeight,pArea) ) 

#define IPetAlgo_CodingLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CodingLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_CodingLocationAlgo(This,rgnId,pThreshold,pDilationCircle,pSelect,pWidth,pHeight)	\
    ( (This)->lpVtbl -> CodingLocationAlgo(This,rgnId,pThreshold,pDilationCircle,pSelect,pWidth,pHeight) ) 

#define IPetAlgo_CodingDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CodingDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_CodingDetectionAlgo(This,rgnId,pNumber)	\
    ( (This)->lpVtbl -> CodingDetectionAlgo(This,rgnId,pNumber) ) 

#define IPetAlgo_KunRunShanLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> KunRunShanLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_KunRunShanLocationAlgo(This,rgnId)	\
    ( (This)->lpVtbl -> KunRunShanLocationAlgo(This,rgnId) ) 

#define IPetAlgo_PetMatchingLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PetMatchingLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_PetMatchingLocationAlgo(This,rgnId,pRow1,pColumn1,pRow2,pColumn2)	\
    ( (This)->lpVtbl -> PetMatchingLocationAlgo(This,rgnId,pRow1,pColumn1,pRow2,pColumn2) ) 

#define IPetAlgo_PetMatchingParaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PetMatchingParaAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_PetMatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> PetMatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IPetAlgo_KunRunShanRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> KunRunShanRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_KunRunShanRegionLocationAlgo(This,rgnId,pRow1Length,pColumn1Length,pRow2Length,pColumn2Length)	\
    ( (This)->lpVtbl -> KunRunShanRegionLocationAlgo(This,rgnId,pRow1Length,pColumn1Length,pRow2Length,pColumn2Length) ) 

#define IPetAlgo_PetMatchingDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PetMatchingDetectionAlgoHelp(This,pHelpStr) ) 

#define IPetAlgo_PetMatchingDetectionAlgo(This,rgnId,pThreshold,pMask,pSelectMin,pNumber)	\
    ( (This)->lpVtbl -> PetMatchingDetectionAlgo(This,rgnId,pThreshold,pMask,pSelectMin,pNumber) ) 

#define IPetAlgo_SetCurrentTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName) ) 

#define IPetAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPetAlgo_INTERFACE_DEFINED__ */



#ifndef __PetAlgoLib_LIBRARY_DEFINED__
#define __PetAlgoLib_LIBRARY_DEFINED__

/* library PetAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_PetAlgoLib;

EXTERN_C const CLSID CLSID_PetAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("BE648697-B4B6-4651-B833-23538C3A6743")
PetAlgo;
#endif
#endif /* __PetAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


