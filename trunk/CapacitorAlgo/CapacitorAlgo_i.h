

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __CapacitorAlgo_i_h__
#define __CapacitorAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICapacitorAlgo_FWD_DEFINED__
#define __ICapacitorAlgo_FWD_DEFINED__
typedef interface ICapacitorAlgo ICapacitorAlgo;
#endif 	/* __ICapacitorAlgo_FWD_DEFINED__ */


#ifndef __CapacitorAlgo_FWD_DEFINED__
#define __CapacitorAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class CapacitorAlgo CapacitorAlgo;
#else
typedef struct CapacitorAlgo CapacitorAlgo;
#endif /* __cplusplus */

#endif 	/* __CapacitorAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICapacitorAlgo_INTERFACE_DEFINED__
#define __ICapacitorAlgo_INTERFACE_DEFINED__

/* interface ICapacitorAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICapacitorAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("72725E53-9909-4658-A2A1-DAAAF5D290FA")
    ICapacitorAlgo : public IDispatch
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgo1( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoHSHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRelocationAlgoHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadiusLong,
            /* [in] */ VARIANT *pRadiusShort,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRelocationAlgoHSHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationHeightAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pLeftCol,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pContrast) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationHeightAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLeftColOffset,
            /* [in] */ VARIANT *pRightCol,
            /* [in] */ VARIANT *pRegionHeight) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleFixedWidthLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLeftColOffset,
            /* [in] */ VARIANT *pRegionWidth,
            /* [in] */ VARIANT *pRegionHeight) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleFixedWidthLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlMeanGrayDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pGrayValueLower,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlMeanGrayDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GLineAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GLineAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAvoidReflectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAvoidReflectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HeightDiffDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HeightDiffDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SlotDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SlotDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GSurfaceAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GSurfaceAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TabDetectProfileAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredBackground,
            /* [in] */ VARIANT *pThredForeground,
            /* [in] */ VARIANT *pAnglePointSize,
            /* [in] */ VARIANT *pMinAngle,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TabDetectProfileAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GapDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GapDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pClosingCircleSize,
            /* [in] */ VARIANT *pClosingRectSize,
            /* [in] */ VARIANT *pDetectMinArea,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoWhite( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoWhiteHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CCenterLocationAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CCenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetSelfLearningTargetTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICapacitorAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICapacitorAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICapacitorAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICapacitorAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICapacitorAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICapacitorAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICapacitorAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICapacitorAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            ICapacitorAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            ICapacitorAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            ICapacitorAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            ICapacitorAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            ICapacitorAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            ICapacitorAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1 )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1Help )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHS )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHSHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRelocationAlgoHS )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadiusLong,
            /* [in] */ VARIANT *pRadiusShort,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRelocationAlgoHSHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationHeightAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pLeftCol,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pContrast);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationHeightAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1 )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1Help )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleRegionLocationAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLeftColOffset,
            /* [in] */ VARIANT *pRightCol,
            /* [in] */ VARIANT *pRegionHeight);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleRegionLocationAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleFixedWidthLocationAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLeftColOffset,
            /* [in] */ VARIANT *pRegionWidth,
            /* [in] */ VARIANT *pRegionHeight);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleFixedWidthLocationAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlMeanGrayDetectAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pGrayValueLower,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlMeanGrayDetectAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAvoidReflectionAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAvoidReflectionAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HeightDiffDetectionAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HeightDiffDetectionAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SlotDetectAlgo1 )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SlotDetectAlgo1Help )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDetectProfileAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredBackground,
            /* [in] */ VARIANT *pThredForeground,
            /* [in] */ VARIANT *pAnglePointSize,
            /* [in] */ VARIANT *pMinAngle,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDetectProfileAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GapDetectAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GapDetectAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pClosingCircleSize,
            /* [in] */ VARIANT *pClosingRectSize,
            /* [in] */ VARIANT *pDetectMinArea,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhite )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CCenterLocationAlgo )( 
            ICapacitorAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CCenterLocationAlgoHelp )( 
            ICapacitorAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            ICapacitorAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            ICapacitorAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            ICapacitorAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            ICapacitorAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            ICapacitorAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            ICapacitorAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            ICapacitorAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            ICapacitorAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            ICapacitorAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            ICapacitorAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        END_INTERFACE
    } ICapacitorAlgoVtbl;

    interface ICapacitorAlgo
    {
        CONST_VTBL struct ICapacitorAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICapacitorAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICapacitorAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICapacitorAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICapacitorAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICapacitorAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICapacitorAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICapacitorAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICapacitorAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define ICapacitorAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define ICapacitorAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define ICapacitorAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define ICapacitorAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define ICapacitorAlgo_SetCurrentTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName) ) 

#define ICapacitorAlgo_CenterLocationAlgo(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnID,pRadius,pMinScore) ) 

#define ICapacitorAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CenterLocationAlgo1(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1(This,rgnID,pRadius,pMinScore) ) 

#define ICapacitorAlgo_CenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1Help(This,pHelpStr) ) 

#define ICapacitorAlgo_CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define ICapacitorAlgo_CenterLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHSHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CenterRelocationAlgoHS(This,rgnId,pRadiusLong,pRadiusShort,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterRelocationAlgoHS(This,rgnId,pRadiusLong,pRadiusShort,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define ICapacitorAlgo_CenterRelocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRelocationAlgoHSHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CenterLocationHeightAlgo(This,rgnID,pLeftCol,pRegWidth,pContrast)	\
    ( (This)->lpVtbl -> CenterLocationHeightAlgo(This,rgnID,pLeftCol,pRegWidth,pContrast) ) 

#define ICapacitorAlgo_CenterLocationHeightAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationHeightAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define ICapacitorAlgo_CircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth) ) 

#define ICapacitorAlgo_CircleRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1Help(This,pHelpStr) ) 

#define ICapacitorAlgo_RectangleRegionLocationAlgo(This,rgnId,pLeftColOffset,pRightCol,pRegionHeight)	\
    ( (This)->lpVtbl -> RectangleRegionLocationAlgo(This,rgnId,pLeftColOffset,pRightCol,pRegionHeight) ) 

#define ICapacitorAlgo_RectangleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectangleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_RectangleFixedWidthLocationAlgo(This,rgnId,pLeftColOffset,pRegionWidth,pRegionHeight)	\
    ( (This)->lpVtbl -> RectangleFixedWidthLocationAlgo(This,rgnId,pLeftColOffset,pRegionWidth,pRegionHeight) ) 

#define ICapacitorAlgo_RectangleFixedWidthLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectangleFixedWidthLocationAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_BubbleDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define ICapacitorAlgo_BubbleDetectProfileAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CurlDetectAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define ICapacitorAlgo_CurlDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CurlMeanGrayDetectAlgo(This,rgnId,pNumPart,pGrayValueLower,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlMeanGrayDetectAlgo(This,rgnId,pNumPart,pGrayValueLower,pGrayValue,pDetectArea) ) 

#define ICapacitorAlgo_CurlMeanGrayDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlMeanGrayDetectAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define ICapacitorAlgo_CurlOptimizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define ICapacitorAlgo_AnnularRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define ICapacitorAlgo_GLineAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea) ) 

#define ICapacitorAlgo_LineDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectionAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea) ) 

#define ICapacitorAlgo_EdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CurlNoCompoundAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgo(This,rgnId,pGrayValueMean) ) 

#define ICapacitorAlgo_CurlNoCompoundAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define ICapacitorAlgo_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_DynThresholdAvoidReflectionAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAvoidReflectionAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define ICapacitorAlgo_DynThresholdAvoidReflectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAvoidReflectionAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_HeightDiffDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectSize,pDefectDiff)	\
    ( (This)->lpVtbl -> HeightDiffDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectSize,pDefectDiff) ) 

#define ICapacitorAlgo_HeightDiffDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> HeightDiffDetectionAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_SlotDetectAlgo1(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> SlotDetectAlgo1(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea) ) 

#define ICapacitorAlgo_SlotDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SlotDetectAlgo1Help(This,pHelpStr) ) 

#define ICapacitorAlgo_GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define ICapacitorAlgo_GSurfaceAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GSurfaceAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_TabDetectProfileAlgo(This,rgnId,pThredBackground,pThredForeground,pAnglePointSize,pMinAngle,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> TabDetectProfileAlgo(This,rgnId,pThredBackground,pThredForeground,pAnglePointSize,pMinAngle,pSeriousBlackPointSize,pDetectArea) ) 

#define ICapacitorAlgo_TabDetectProfileAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TabDetectProfileAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_GapDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GapDetectAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_GapDetectAlgo(This,rgnId,pGrayThresh,pClosingCircleSize,pClosingRectSize,pDetectMinArea,pDetectArea)	\
    ( (This)->lpVtbl -> GapDetectAlgo(This,rgnId,pGrayThresh,pClosingCircleSize,pClosingRectSize,pDetectMinArea,pDetectArea) ) 

#define ICapacitorAlgo_DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define ICapacitorAlgo_DynThresholdAlgoWhiteHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_CCenterLocationAlgo(This,rgnID,pMinScore)	\
    ( (This)->lpVtbl -> CCenterLocationAlgo(This,rgnID,pMinScore) ) 

#define ICapacitorAlgo_CCenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CCenterLocationAlgoHelp(This,pHelpStr) ) 

#define ICapacitorAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define ICapacitorAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define ICapacitorAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define ICapacitorAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define ICapacitorAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define ICapacitorAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define ICapacitorAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define ICapacitorAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define ICapacitorAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define ICapacitorAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICapacitorAlgo_INTERFACE_DEFINED__ */



#ifndef __CapacitorAlgoLib_LIBRARY_DEFINED__
#define __CapacitorAlgoLib_LIBRARY_DEFINED__

/* library CapacitorAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_CapacitorAlgoLib;

EXTERN_C const CLSID CLSID_CapacitorAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("ABE20C3B-8803-423D-8824-38FCE021DAF4")
CapacitorAlgo;
#endif
#endif /* __CapacitorAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


