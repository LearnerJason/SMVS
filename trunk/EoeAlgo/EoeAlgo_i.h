

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:16:40 2019
 */
/* Compiler settings for EoeAlgo.idl:
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

#ifndef __EoeAlgo_i_h__
#define __EoeAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IEoeAlgo_FWD_DEFINED__
#define __IEoeAlgo_FWD_DEFINED__
typedef interface IEoeAlgo IEoeAlgo;
#endif 	/* __IEoeAlgo_FWD_DEFINED__ */


#ifndef __EoeAlgo_FWD_DEFINED__
#define __EoeAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class EoeAlgo EoeAlgo;
#else
typedef struct EoeAlgo EoeAlgo;
#endif /* __cplusplus */

#endif 	/* __EoeAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IEoeAlgo_INTERFACE_DEFINED__
#define __IEoeAlgo_INTERFACE_DEFINED__

/* interface IEoeAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IEoeAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("88A26EAF-5D85-493E-BFB2-E2BF8AEAEE21")
    IEoeAlgo : public IDispatch
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetWhiteErrorRegion( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pWhiteErrorRgn) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetInternalStats( 
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ReloadExternalFile( 
            /* [in] */ BSTR *bstrPath) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetSelfLearningTargetTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetGlueInjectorID( 
            /* [out][in] */ LONG *pInjectorID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetInterImgs( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetRegionEx( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetRegionExTest( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRectangleLocationAlgoHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRectangleLocationAlgoHSHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoUsePointFitting( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pSearchRegRow,
            /* [in] */ VARIANT *pSearchRegCol,
            /* [in] */ VARIANT *pOuterRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pScanLineNum,
            /* [in] */ VARIANT *pScanLineDirection,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pPointSelection,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoUsePointFittingHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MatchingParaAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MatchingParaAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MatchingParaMinScoreAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pNumLevels,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MatchingParaMinScoreAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RotatingAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pEdgeType,
            /* [in] */ VARIANT *pEdgeDist,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RotatingAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionAngleAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionAngleAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgoOCR( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgoOCRHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationExtractOCRAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrOcrRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationExtractOCRAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionAngleAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionAngleAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BoundaryRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BoundaryRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pWinType,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE UVCurlAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE UVCurlAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GLineNewAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackThredHigh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteThredHigh,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GLineNewAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GContoursinkAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GContoursinkAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoBlack( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoBlackHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoBlackNumber( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoBlackNumberHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoWhite( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoWhiteHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoWhiteNumber( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoWhiteNumberHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoBlackWhiteNumber( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoBlackWhiteNumberHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoOCR( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoOCRHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MinMaxGrayDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MinMaxGrayDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgoBW( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgoBWHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RRegionDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSmoothPara,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pARMin,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RRegionDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RRegionDetectBigAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDiffWhite,
            /* [in] */ VARIANT *pARMax,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RRegionDetectBigAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectOcrDistOffsetAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistanceOffset) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectOcrDistOffsetAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgoTabOCR( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgoTabOCRHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoTabOCR( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoTabOCRHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MultOcrDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [in] */ BSTR *bstrShm3File,
            /* [in] */ BSTR *bstrShm4File,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pLineDetectFlage,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MultOcrDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MultOcrDetectExcludeTabAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [in] */ BSTR *bstrShm3File,
            /* [in] */ BSTR *bstrShm4File,
            /* [in] */ BSTR *bstrExtractRgnFile,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pLineDetectFlage,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MultOcrDetectExcludeTabAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PrintErrorDetectOcrAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pHeightContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PrintErrorDetectOcrAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GlueInjectorDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pGrayDiffValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GlueInjectorDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GlueDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pGrayDiffValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GlueDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GlueInjectorIDDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackPointThresh,
            /* [in] */ VARIANT *pWhitePointThresh,
            /* [in] */ VARIANT *pMeanGrayDiffValue,
            /* [in] */ VARIANT *pDeviationGrayDiffValue,
            /* [in] */ VARIANT *pMaxID,
            /* [retval][out] */ VARIANT *pDetectID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GlueInjectorIDDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GlueInjectorIDDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRefreshIndex,
            /* [in] */ VARIANT *pStartIndex,
            /* [in] */ VARIANT *pTotalInjectorNum,
            /* [retval][out] */ VARIANT *pDetectID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GlueInjectorIDDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EccentricDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistance) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EccentricDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InnerCircleLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowThred,
            /* [in] */ VARIANT *pHighThred,
            /* [in] */ VARIANT *pLowLength,
            /* [in] */ VARIANT *pHighLength) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InnerCircleLocationAlgoHelp( 
            /* [retval][out] */ VARIANT *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ReprintDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ReprintDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WLineDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WLineDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WSpotDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSpotMaskSize,
            /* [in] */ VARIANT *pSpotThred,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WSpotDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WBlockDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlockThred,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WBlockDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ArrowDistOffsetAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowThred,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [retval][out] */ VARIANT *pDistanceOffset) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ArrowDistOffsetAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RingLocationForColorAlgoHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pRegionWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RingLocationForColorAlgoHSHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationForColorAlgoHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationForColorAlgoHSHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreqCutoff,
            /* [in] */ VARIANT *pWinType,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdForColorAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdForColorAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetCurrentLocationXY( 
            /* [retval][out] */ BSTR *bstrLocationXY) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetCenterLocation( 
            /* [retval][out] */ BSTR *bstrLocationXY) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RivetCenterLocationAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pThredScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RivetCenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RivetDiameterMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOffset,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pCircularityValue,
            /* [retval][out] */ VARIANT *pDiameter) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RivetDiameterMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RivetDiameterMeasureAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDiameter) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RivetDiameterMeasureAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldAngleDetectGrayAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAlgoSelect,
            /* [in] */ VARIANT *pWeldWidth,
            /* [in] */ VARIANT *pSmoothSize,
            /* [retval][out] */ VARIANT *pDetectAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldAngleDetectGrayAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldAngleDetectWhiteGlueAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pAspectRationThresh,
            /* [in] */ VARIANT *pClosingMaskWidth,
            /* [retval][out] */ VARIANT *pDetectAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldAngleDetectWhiteGlueAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldAngleDetectGlueAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackWhite,
            /* [in] */ VARIANT *pEnhancePara,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pAspectRationThresh,
            /* [in] */ VARIANT *pClosingMaskWidth,
            /* [retval][out] */ VARIANT *pDetectAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldAngleDetectGlueAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldWhiteGlueRegExtractAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [retval][out] */ VARIANT *pWeldRegArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldWhiteGlueRegExtractAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldRegSelectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWeldSelection) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldRegSelectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PhaseCorrelationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMode,
            /* [retval][out] */ VARIANT *pAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PhaseCorrelationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentTaskName2( 
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName,
            /* [in] */ BSTR *bstrChNameWithoutCameraIndex) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PhaseCorrelationRectangularRegionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWidthPlaceSet,
            /* [in] */ VARIANT *pHeightPlaceSet,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PhaseCorrelationRectangularRegionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PhaseCorrelationRectangularAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMode,
            /* [in] */ VARIANT *pScore,
            /* [retval][out] */ VARIANT *pCalibrationImageID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PhaseCorrelationRectangularAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRectangleLocationAlgoSobel( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSobelSize,
            /* [in] */ VARIANT *pDisplayMode,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRectangleLocationAlgoSobelHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeDetectRectanglePixelWiseAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pThresholdLight,
            /* [in] */ VARIANT *pThresholdDark,
            /* [in] */ VARIANT *pSharpness,
            /* [in] */ VARIANT *pDefectSizeLight,
            /* [in] */ VARIANT *pDefectSizeDark,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeDetectRectanglePixelWiseAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeRectangleAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pPartsWidth,
            /* [in] */ VARIANT *pFreqStart,
            /* [in] */ VARIANT *pFreqCutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeRectangleAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TabDeviationDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pMinGray,
            /* [in] */ VARIANT *pMaxGray,
            /* [in] */ VARIANT *pTabLength,
            /* [retval][out] */ VARIANT *pDistanceDifference) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TabDeviationDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectComprehensiveAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pLowerLimit,
            /* [in] */ VARIANT *pUpperLimit,
            /* [in] */ VARIANT *pDisplayMode,
            /* [in] */ VARIANT *pFeatureNumber,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectComprehensiveAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionExcludingArbitraryRegionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChangingSize,
            /* [in] */ VARIANT *pDoRegistration,
            /* [in] */ BSTR *bstrOcrRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionExcludingArbitraryRegionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionRectangleLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChangingSize,
            /* [in] */ VARIANT *pDoMatch,
            /* [in] */ BSTR *bstrExtractRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionRectangleLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RegistrateRectangleAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pCalibrationImageID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RegistrateRectangleAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRectangleLocationAlgoInnerEdge( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLidWidth,
            /* [in] */ VARIANT *pLidHeight,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pSecondThreshold,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRectangleLocationAlgoInnerEdgeHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRectangleSecondLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRectangleSecondLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AngleRegistrationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pPatternWidth,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AngleRegistrationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiscontinuousLineDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pRequiredLineLength,
            /* [retval][out] */ VARIANT *pBrokenLineNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiscontinuousLineDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TabDeviationDetectIndistinctEdgeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pMinGray,
            /* [in] */ VARIANT *pMaxGray,
            /* [in] */ VARIANT *pTabLength,
            /* [retval][out] */ VARIANT *pDistanceDifference) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TabDeviationDetectIndistinctEdgeAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleRoundRegionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChangingSize,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pChamferRadius) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleRoundRegionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleEdgeRegionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChangingSize,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pChamferRadius) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleEdgeRegionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DefectDetectionRectanglePixelWiseAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pSharpness,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DefectDetectionRectanglePixelWiseAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DefectDetectionRectanglePixelWiseDarkLightAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSharpness,
            /* [in] */ VARIANT *pThresholdLight,
            /* [in] */ VARIANT *pDefectSizeLight,
            /* [in] */ VARIANT *pThresholdDark,
            /* [in] */ VARIANT *pDefectSizeDark,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DefectDetectionRectanglePixelWiseDarkLightAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgoRotateImage( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pThresholdDark,
            /* [in] */ VARIANT *pDefectSizeDark,
            /* [in] */ VARIANT *pThresholdLight,
            /* [in] */ VARIANT *pDefectSizeLight,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgoRotateImageHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RRegionDetectExcludeEdgeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThresh,
            /* [in] */ VARIANT *pEdgeLength,
            /* [in] */ VARIANT *pErrorThresh,
            /* [in] */ VARIANT *pARMin,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RRegionDetectExcludeEdgeAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AngleRegistrationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AngleRegistrationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AngleRegistrationRotateImageAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pAreaThreshold,
            /* [in] */ VARIANT *pTabLength,
            /* [in] */ VARIANT *pParalleTab,
            /* [retval][out] */ VARIANT *pAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AngleRegistrationRotateImageAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TabDeviationDetectionHoughAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGaussThreshold,
            /* [in] */ VARIANT *pHoughThreshold,
            /* [in] */ VARIANT *pDisplayEdge,
            /* [retval][out] */ VARIANT *pAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TabDeviationDetectionHoughAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEoeAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEoeAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEoeAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEoeAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEoeAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEoeAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEoeAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEoeAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            IEoeAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            IEoeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            IEoeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhiteErrorRegion )( 
            IEoeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pWhiteErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            IEoeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            IEoeAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            IEoeAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            IEoeAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetGlueInjectorID )( 
            IEoeAlgo * This,
            /* [out][in] */ LONG *pInjectorID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInterImgs )( 
            IEoeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            IEoeAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            IEoeAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            IEoeAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            IEoeAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            IEoeAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            IEoeAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            IEoeAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            IEoeAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            IEoeAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegionEx )( 
            IEoeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegionExTest )( 
            IEoeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHS )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHSHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoHS )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoHSHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFitting )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pSearchRegRow,
            /* [in] */ VARIANT *pSearchRegCol,
            /* [in] */ VARIANT *pOuterRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pScanLineNum,
            /* [in] */ VARIANT *pScanLineDirection,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pPointSelection,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFittingHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pNumLevels,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RotatingAngleDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pEdgeType,
            /* [in] */ VARIANT *pEdgeDist,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RotatingAngleDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoOCR )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoOCRHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationExtractOCRAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrOcrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationExtractOCRAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pWinType,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo2 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo2Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineNewAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackThredHigh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteThredHigh,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineNewAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GContoursinkAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GContoursinkAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlack )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackNumber )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackNumberHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhite )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteNumber )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteNumberHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackWhiteNumber )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackWhiteNumberHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoOCR )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoOCRHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MinMaxGrayDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MinMaxGrayDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBW )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBWHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSmoothPara,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pARMin,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectBigAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDiffWhite,
            /* [in] */ VARIANT *pARMax,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectBigAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectOcrDistOffsetAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistanceOffset);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectOcrDistOffsetAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoTabOCR )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoTabOCRHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoTabOCR )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoTabOCRHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MultOcrDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [in] */ BSTR *bstrShm3File,
            /* [in] */ BSTR *bstrShm4File,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pLineDetectFlage,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MultOcrDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MultOcrDetectExcludeTabAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [in] */ BSTR *bstrShm3File,
            /* [in] */ BSTR *bstrShm4File,
            /* [in] */ BSTR *bstrExtractRgnFile,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pLineDetectFlage,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MultOcrDetectExcludeTabAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PrintErrorDetectOcrAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pHeightContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PrintErrorDetectOcrAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pGrayDiffValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pGrayDiffValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorIDDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackPointThresh,
            /* [in] */ VARIANT *pWhitePointThresh,
            /* [in] */ VARIANT *pMeanGrayDiffValue,
            /* [in] */ VARIANT *pDeviationGrayDiffValue,
            /* [in] */ VARIANT *pMaxID,
            /* [retval][out] */ VARIANT *pDetectID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorIDDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorIDDetectAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRefreshIndex,
            /* [in] */ VARIANT *pStartIndex,
            /* [in] */ VARIANT *pTotalInjectorNum,
            /* [retval][out] */ VARIANT *pDetectID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorIDDetectAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EccentricDetectionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistance);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EccentricDetectionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InnerCircleLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowThred,
            /* [in] */ VARIANT *pHighThred,
            /* [in] */ VARIANT *pLowLength,
            /* [in] */ VARIANT *pHighLength);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InnerCircleLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ VARIANT *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReprintDetectionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReprintDetectionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WLineDetectionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WLineDetectionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WSpotDetectionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSpotMaskSize,
            /* [in] */ VARIANT *pSpotThred,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WSpotDetectionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WBlockDetectionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlockThred,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WBlockDetectionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ArrowDistOffsetAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowThred,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [retval][out] */ VARIANT *pDistanceOffset);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ArrowDistOffsetAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RingLocationForColorAlgoHS )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pRegionWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RingLocationForColorAlgoHSHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationForColorAlgoHS )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationForColorAlgoHSHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeForColorAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreqCutoff,
            /* [in] */ VARIANT *pWinType,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeForColorAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentLocationXY )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCenterLocation )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RivetCenterLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pThredScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RivetCenterLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RivetDiameterMeasureAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOffset,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pCircularityValue,
            /* [retval][out] */ VARIANT *pDiameter);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RivetDiameterMeasureAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RivetDiameterMeasureAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDiameter);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RivetDiameterMeasureAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectGrayAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAlgoSelect,
            /* [in] */ VARIANT *pWeldWidth,
            /* [in] */ VARIANT *pSmoothSize,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectGrayAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectWhiteGlueAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pAspectRationThresh,
            /* [in] */ VARIANT *pClosingMaskWidth,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectWhiteGlueAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectGlueAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackWhite,
            /* [in] */ VARIANT *pEnhancePara,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pAspectRationThresh,
            /* [in] */ VARIANT *pClosingMaskWidth,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectGlueAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldWhiteGlueRegExtractAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [retval][out] */ VARIANT *pWeldRegArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldWhiteGlueRegExtractAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldRegSelectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWeldSelection);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldRegSelectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PhaseCorrelationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMode,
            /* [retval][out] */ VARIANT *pAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PhaseCorrelationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName2 )( 
            IEoeAlgo * This,
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName,
            /* [in] */ BSTR *bstrChNameWithoutCameraIndex);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PhaseCorrelationRectangularRegionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWidthPlaceSet,
            /* [in] */ VARIANT *pHeightPlaceSet,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PhaseCorrelationRectangularRegionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PhaseCorrelationRectangularAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMode,
            /* [in] */ VARIANT *pScore,
            /* [retval][out] */ VARIANT *pCalibrationImageID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PhaseCorrelationRectangularAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoSobel )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSobelSize,
            /* [in] */ VARIANT *pDisplayMode,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoSobelHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeDetectRectanglePixelWiseAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pThresholdLight,
            /* [in] */ VARIANT *pThresholdDark,
            /* [in] */ VARIANT *pSharpness,
            /* [in] */ VARIANT *pDefectSizeLight,
            /* [in] */ VARIANT *pDefectSizeDark,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeDetectRectanglePixelWiseAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeRectangleAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pPartsWidth,
            /* [in] */ VARIANT *pFreqStart,
            /* [in] */ VARIANT *pFreqCutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeRectangleAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDeviationDetectAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pMinGray,
            /* [in] */ VARIANT *pMaxGray,
            /* [in] */ VARIANT *pTabLength,
            /* [retval][out] */ VARIANT *pDistanceDifference);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDeviationDetectAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectComprehensiveAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pLowerLimit,
            /* [in] */ VARIANT *pUpperLimit,
            /* [in] */ VARIANT *pDisplayMode,
            /* [in] */ VARIANT *pFeatureNumber,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectComprehensiveAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionExcludingArbitraryRegionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChangingSize,
            /* [in] */ VARIANT *pDoRegistration,
            /* [in] */ BSTR *bstrOcrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionExcludingArbitraryRegionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionRectangleLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChangingSize,
            /* [in] */ VARIANT *pDoMatch,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionRectangleLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RegistrateRectangleAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pCalibrationImageID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RegistrateRectangleAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoInnerEdge )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLidWidth,
            /* [in] */ VARIANT *pLidHeight,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pSecondThreshold,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoInnerEdgeHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleSecondLocationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleSecondLocationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AngleRegistrationAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pPatternWidth,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AngleRegistrationAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiscontinuousLineDetectionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pRequiredLineLength,
            /* [retval][out] */ VARIANT *pBrokenLineNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiscontinuousLineDetectionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDeviationDetectIndistinctEdgeAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pMinGray,
            /* [in] */ VARIANT *pMaxGray,
            /* [in] */ VARIANT *pTabLength,
            /* [retval][out] */ VARIANT *pDistanceDifference);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDeviationDetectIndistinctEdgeAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleRoundRegionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChangingSize,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pChamferRadius);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleRoundRegionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleEdgeRegionAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChangingSize,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pChamferRadius);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleEdgeRegionAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DefectDetectionRectanglePixelWiseAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pSharpness,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DefectDetectionRectanglePixelWiseAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DefectDetectionRectanglePixelWiseDarkLightAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSharpness,
            /* [in] */ VARIANT *pThresholdLight,
            /* [in] */ VARIANT *pDefectSizeLight,
            /* [in] */ VARIANT *pThresholdDark,
            /* [in] */ VARIANT *pDefectSizeDark,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DefectDetectionRectanglePixelWiseDarkLightAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoRotateImage )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pThresholdDark,
            /* [in] */ VARIANT *pDefectSizeDark,
            /* [in] */ VARIANT *pThresholdLight,
            /* [in] */ VARIANT *pDefectSizeLight,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoRotateImageHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectExcludeEdgeAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThresh,
            /* [in] */ VARIANT *pEdgeLength,
            /* [in] */ VARIANT *pErrorThresh,
            /* [in] */ VARIANT *pARMin,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectExcludeEdgeAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AngleRegistrationAlgo1 )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AngleRegistrationAlgo1Help )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AngleRegistrationRotateImageAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pAreaThreshold,
            /* [in] */ VARIANT *pTabLength,
            /* [in] */ VARIANT *pParalleTab,
            /* [retval][out] */ VARIANT *pAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AngleRegistrationRotateImageAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDeviationDetectionHoughAlgo )( 
            IEoeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGaussThreshold,
            /* [in] */ VARIANT *pHoughThreshold,
            /* [in] */ VARIANT *pDisplayEdge,
            /* [retval][out] */ VARIANT *pAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDeviationDetectionHoughAlgoHelp )( 
            IEoeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        END_INTERFACE
    } IEoeAlgoVtbl;

    interface IEoeAlgo
    {
        CONST_VTBL struct IEoeAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEoeAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEoeAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEoeAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEoeAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEoeAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEoeAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEoeAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IEoeAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define IEoeAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define IEoeAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define IEoeAlgo_GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn)	\
    ( (This)->lpVtbl -> GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn) ) 

#define IEoeAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define IEoeAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define IEoeAlgo_SetCurrentTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName) ) 

#define IEoeAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#define IEoeAlgo_GetGlueInjectorID(This,pInjectorID)	\
    ( (This)->lpVtbl -> GetGlueInjectorID(This,pInjectorID) ) 

#define IEoeAlgo_GetInterImgs(This,rgnID,pInterImgs)	\
    ( (This)->lpVtbl -> GetInterImgs(This,rgnID,pInterImgs) ) 

#define IEoeAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define IEoeAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define IEoeAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define IEoeAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define IEoeAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define IEoeAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define IEoeAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define IEoeAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define IEoeAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define IEoeAlgo_GetRegionEx(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegionEx(This,rgnID,pRgn) ) 

#define IEoeAlgo_GetRegionExTest(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegionExTest(This,rgnID,pRgn) ) 

#define IEoeAlgo_CenterLocationAlgo(This,rgnId,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnId,pRadius,pMinScore) ) 

#define IEoeAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CenterLocationAlgo1(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1(This,rgnID,pRadius,pMinScore) ) 

#define IEoeAlgo_CenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define IEoeAlgo_CenterLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHSHelp(This,pHelpStr) ) 

#define IEoeAlgo_CenterRectangleLocationAlgoHS(This,rgnId,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoHS(This,rgnId,pThresholdLow,pOutlierRemoverSize) ) 

#define IEoeAlgo_CenterRectangleLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoHSHelp(This,pHelpStr) ) 

#define IEoeAlgo_CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold) ) 

#define IEoeAlgo_CenterLocationAlgoUsePointFittingHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFittingHelp(This,pHelpStr) ) 

#define IEoeAlgo_MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IEoeAlgo_MatchingParaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IEoeAlgo_MatchingParaMinScoreAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_RotatingAngleDetectAlgo(This,rgnId,pMaskSize,pBlackThredLow,pEdgeType,pEdgeDist,pDefectSize,pDetectAngle)	\
    ( (This)->lpVtbl -> RotatingAngleDetectAlgo(This,rgnId,pMaskSize,pBlackThredLow,pEdgeType,pEdgeDist,pDefectSize,pDetectAngle) ) 

#define IEoeAlgo_RotatingAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RotatingAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IEoeAlgo_CircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IEoeAlgo_CircleRegionAngleAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IEoeAlgo_DetectRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DetectRegionLocationAlgoOCR(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoOCR(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IEoeAlgo_DetectRegionLocationAlgoOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoOCRHelp(This,pHelpStr) ) 

#define IEoeAlgo_DetectRegionLocationExtractOCRAlgo(This,rgnId,pRadius,pErosionSize,bstrOcrRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationExtractOCRAlgo(This,rgnId,pRadius,pErosionSize,bstrOcrRgnFile) ) 

#define IEoeAlgo_DetectRegionLocationExtractOCRAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationExtractOCRAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth) ) 

#define IEoeAlgo_CircleRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_CircleRegionAngleAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IEoeAlgo_CircleRegionAngleAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IEoeAlgo_BoundaryRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IEoeAlgo_CurlOptimizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CurlDetectAlgo1(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo1(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define IEoeAlgo_CurlDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_CurlDetectAlgo2(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo2(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define IEoeAlgo_CurlDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgo2Help(This,pHelpStr) ) 

#define IEoeAlgo_UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea) ) 

#define IEoeAlgo_UVCurlAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> UVCurlAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IEoeAlgo_AnnularRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CurlNoCompoundAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgo(This,rgnId,pGrayValueMean) ) 

#define IEoeAlgo_CurlNoCompoundAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IEoeAlgo_GSurfaceAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GSurfaceAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IEoeAlgo_GLineAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_GLineNewAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteThredHigh,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineNewAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteThredHigh,pWhiteDefectSize,pDetectArea) ) 

#define IEoeAlgo_GLineNewAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineNewAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IEoeAlgo_EdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_GContoursinkAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GContoursinkAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IEoeAlgo_GContoursinkAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GContoursinkAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IEoeAlgo_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IEoeAlgo_DynThresholdAlgoBlackHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdAlgoBlackNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectNumber) ) 

#define IEoeAlgo_DynThresholdAlgoBlackNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackNumberHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IEoeAlgo_DynThresholdAlgoWhiteHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdAlgoWhiteNumber(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteNumber(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber) ) 

#define IEoeAlgo_DynThresholdAlgoWhiteNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteNumberHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdAlgoBlackWhiteNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackWhiteNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber) ) 

#define IEoeAlgo_DynThresholdAlgoBlackWhiteNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackWhiteNumberHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdAlgoOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea) ) 

#define IEoeAlgo_DynThresholdAlgoOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoOCRHelp(This,pHelpStr) ) 

#define IEoeAlgo_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IEoeAlgo_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IEoeAlgo_BlackDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_MinMaxGrayDetectAlgo(This,rgnId,pBlackThresh,pSeriousBlackPointSize,pWhiteThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> MinMaxGrayDetectAlgo(This,rgnId,pBlackThresh,pSeriousBlackPointSize,pWhiteThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IEoeAlgo_MinMaxGrayDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MinMaxGrayDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IEoeAlgo_AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr) ) 

#define IEoeAlgo_SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IEoeAlgo_SectorRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_SectorRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IEoeAlgo_SectorRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_RRegionDetectAlgo(This,rgnId,pSmoothPara,pMaskSize,pThredLow,pARMin,pDetectArea)	\
    ( (This)->lpVtbl -> RRegionDetectAlgo(This,rgnId,pSmoothPara,pMaskSize,pThredLow,pARMin,pDetectArea) ) 

#define IEoeAlgo_RRegionDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RRegionDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_RRegionDetectBigAlgo(This,rgnId,pDiffWhite,pARMax,pDetectArea)	\
    ( (This)->lpVtbl -> RRegionDetectBigAlgo(This,rgnId,pDiffWhite,pARMax,pDetectArea) ) 

#define IEoeAlgo_RRegionDetectBigAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RRegionDetectBigAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DetectOcrDistOffsetAlgo(This,rgnId,pDistanceOffset)	\
    ( (This)->lpVtbl -> DetectOcrDistOffsetAlgo(This,rgnId,pDistanceOffset) ) 

#define IEoeAlgo_DetectOcrDistOffsetAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectOcrDistOffsetAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DetectRegionLocationAlgoTabOCR(This,rgnId,pErosionSize,bstrRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoTabOCR(This,rgnId,pErosionSize,bstrRgnFile) ) 

#define IEoeAlgo_DetectRegionLocationAlgoTabOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoTabOCRHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdAlgoTabOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoTabOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea) ) 

#define IEoeAlgo_DynThresholdAlgoTabOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoTabOCRHelp(This,pHelpStr) ) 

#define IEoeAlgo_MultOcrDetectAlgo(This,rgnId,pGreediness,bstrShm1File,bstrShm2File,bstrShm3File,bstrShm4File,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pLineDetectFlage,pLowContrast,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> MultOcrDetectAlgo(This,rgnId,pGreediness,bstrShm1File,bstrShm2File,bstrShm3File,bstrShm4File,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pLineDetectFlage,pLowContrast,pDilationSize,pDetectArea) ) 

#define IEoeAlgo_MultOcrDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MultOcrDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_MultOcrDetectExcludeTabAlgo(This,rgnId,pGreediness,bstrShm1File,bstrShm2File,bstrShm3File,bstrShm4File,bstrExtractRgnFile,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pLineDetectFlage,pLowContrast,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> MultOcrDetectExcludeTabAlgo(This,rgnId,pGreediness,bstrShm1File,bstrShm2File,bstrShm3File,bstrShm4File,bstrExtractRgnFile,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pLineDetectFlage,pLowContrast,pDilationSize,pDetectArea) ) 

#define IEoeAlgo_MultOcrDetectExcludeTabAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MultOcrDetectExcludeTabAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_PrintErrorDetectOcrAlgo(This,rgnId,pLowContrast,pHeightContrast,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> PrintErrorDetectOcrAlgo(This,rgnId,pLowContrast,pHeightContrast,pDilationSize,pDetectArea) ) 

#define IEoeAlgo_PrintErrorDetectOcrAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PrintErrorDetectOcrAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_GlueInjectorDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pGrayDiffValue,pDetectArea)	\
    ( (This)->lpVtbl -> GlueInjectorDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pGrayDiffValue,pDetectArea) ) 

#define IEoeAlgo_GlueInjectorDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GlueInjectorDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_GlueDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pGrayDiffValue,pDetectArea)	\
    ( (This)->lpVtbl -> GlueDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pGrayDiffValue,pDetectArea) ) 

#define IEoeAlgo_GlueDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GlueDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_GlueInjectorIDDetectAlgo(This,rgnId,pBlackPointThresh,pWhitePointThresh,pMeanGrayDiffValue,pDeviationGrayDiffValue,pMaxID,pDetectID)	\
    ( (This)->lpVtbl -> GlueInjectorIDDetectAlgo(This,rgnId,pBlackPointThresh,pWhitePointThresh,pMeanGrayDiffValue,pDeviationGrayDiffValue,pMaxID,pDetectID) ) 

#define IEoeAlgo_GlueInjectorIDDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GlueInjectorIDDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_GlueInjectorIDDetectAlgo1(This,rgnId,pRefreshIndex,pStartIndex,pTotalInjectorNum,pDetectID)	\
    ( (This)->lpVtbl -> GlueInjectorIDDetectAlgo1(This,rgnId,pRefreshIndex,pStartIndex,pTotalInjectorNum,pDetectID) ) 

#define IEoeAlgo_GlueInjectorIDDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> GlueInjectorIDDetectAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_EccentricDetectionAlgo(This,rgnId,pDistance)	\
    ( (This)->lpVtbl -> EccentricDetectionAlgo(This,rgnId,pDistance) ) 

#define IEoeAlgo_EccentricDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EccentricDetectionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_InnerCircleLocationAlgo(This,rgnId,pLowThred,pHighThred,pLowLength,pHighLength)	\
    ( (This)->lpVtbl -> InnerCircleLocationAlgo(This,rgnId,pLowThred,pHighThred,pLowLength,pHighLength) ) 

#define IEoeAlgo_InnerCircleLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> InnerCircleLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_ReprintDetectionAlgo(This,rgnId,pGrayDiff)	\
    ( (This)->lpVtbl -> ReprintDetectionAlgo(This,rgnId,pGrayDiff) ) 

#define IEoeAlgo_ReprintDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ReprintDetectionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_WLineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectArea)	\
    ( (This)->lpVtbl -> WLineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectArea) ) 

#define IEoeAlgo_WLineDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WLineDetectionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_WSpotDetectionAlgo(This,rgnId,pSpotMaskSize,pSpotThred,pDefectArea)	\
    ( (This)->lpVtbl -> WSpotDetectionAlgo(This,rgnId,pSpotMaskSize,pSpotThred,pDefectArea) ) 

#define IEoeAlgo_WSpotDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WSpotDetectionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_WBlockDetectionAlgo(This,rgnId,pBlockThred,pMinArea,pDefectArea)	\
    ( (This)->lpVtbl -> WBlockDetectionAlgo(This,rgnId,pBlockThred,pMinArea,pDefectArea) ) 

#define IEoeAlgo_WBlockDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WBlockDetectionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_ArrowDistOffsetAlgo(This,rgnId,pLowThred,pErosionSize,pBlackPointUpSize,pDistanceOffset)	\
    ( (This)->lpVtbl -> ArrowDistOffsetAlgo(This,rgnId,pLowThred,pErosionSize,pBlackPointUpSize,pDistanceOffset) ) 

#define IEoeAlgo_ArrowDistOffsetAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ArrowDistOffsetAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_RingLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize,pRegionWidth)	\
    ( (This)->lpVtbl -> RingLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize,pRegionWidth) ) 

#define IEoeAlgo_RingLocationForColorAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RingLocationForColorAlgoHSHelp(This,pHelpStr) ) 

#define IEoeAlgo_CenterLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define IEoeAlgo_CenterLocationForColorAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationForColorAlgoHSHelp(This,pHelpStr) ) 

#define IEoeAlgo_CurlOptimizeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pFreq_start,pFreqCutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pFreq_start,pFreqCutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IEoeAlgo_CurlOptimizeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeForColorAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IEoeAlgo_DynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IEoeAlgo_EdgeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeForColorAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IEoeAlgo_AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_AnnularRotaDynThresholdForColorAlgo1(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo1(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IEoeAlgo_AnnularRotaDynThresholdForColorAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_GetCurrentLocationXY(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCurrentLocationXY(This,bstrLocationXY) ) 

#define IEoeAlgo_GetCenterLocation(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCenterLocation(This,bstrLocationXY) ) 

#define IEoeAlgo_RivetCenterLocationAlgo(This,rgnID,pRadius,pMinScore,pThredScore)	\
    ( (This)->lpVtbl -> RivetCenterLocationAlgo(This,rgnID,pRadius,pMinScore,pThredScore) ) 

#define IEoeAlgo_RivetCenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RivetCenterLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_RivetDiameterMeasureAlgo(This,rgnId,pOffset,pSigma,pThredLower,pThredUpper,pCircularityValue,pDiameter)	\
    ( (This)->lpVtbl -> RivetDiameterMeasureAlgo(This,rgnId,pOffset,pSigma,pThredLower,pThredUpper,pCircularityValue,pDiameter) ) 

#define IEoeAlgo_RivetDiameterMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RivetDiameterMeasureAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_RivetDiameterMeasureAlgo1(This,rgnId,pOffset,pDiameter)	\
    ( (This)->lpVtbl -> RivetDiameterMeasureAlgo1(This,rgnId,pOffset,pDiameter) ) 

#define IEoeAlgo_RivetDiameterMeasureAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> RivetDiameterMeasureAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_WeldAngleDetectAlgo(This,rgnId,pDetectAngle)	\
    ( (This)->lpVtbl -> WeldAngleDetectAlgo(This,rgnId,pDetectAngle) ) 

#define IEoeAlgo_WeldAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_WeldAngleDetectGrayAlgo(This,rgnId,pAlgoSelect,pWeldWidth,pSmoothSize,pDetectAngle)	\
    ( (This)->lpVtbl -> WeldAngleDetectGrayAlgo(This,rgnId,pAlgoSelect,pWeldWidth,pSmoothSize,pDetectAngle) ) 

#define IEoeAlgo_WeldAngleDetectGrayAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldAngleDetectGrayAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_WeldAngleDetectWhiteGlueAlgo(This,rgnId,pDynThreshold,pAspectRationThresh,pClosingMaskWidth,pDetectAngle)	\
    ( (This)->lpVtbl -> WeldAngleDetectWhiteGlueAlgo(This,rgnId,pDynThreshold,pAspectRationThresh,pClosingMaskWidth,pDetectAngle) ) 

#define IEoeAlgo_WeldAngleDetectWhiteGlueAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldAngleDetectWhiteGlueAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_WeldAngleDetectGlueAlgo(This,rgnId,pBlackWhite,pEnhancePara,pDynThreshold,pAspectRationThresh,pClosingMaskWidth,pDetectAngle)	\
    ( (This)->lpVtbl -> WeldAngleDetectGlueAlgo(This,rgnId,pBlackWhite,pEnhancePara,pDynThreshold,pAspectRationThresh,pClosingMaskWidth,pDetectAngle) ) 

#define IEoeAlgo_WeldAngleDetectGlueAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldAngleDetectGlueAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_WeldWhiteGlueRegExtractAlgo(This,rgnId,pBlackMaskSize,pWeldRegArea)	\
    ( (This)->lpVtbl -> WeldWhiteGlueRegExtractAlgo(This,rgnId,pBlackMaskSize,pWeldRegArea) ) 

#define IEoeAlgo_WeldWhiteGlueRegExtractAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldWhiteGlueRegExtractAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_WeldRegSelectAlgo(This,rgnId,pWeldSelection)	\
    ( (This)->lpVtbl -> WeldRegSelectAlgo(This,rgnId,pWeldSelection) ) 

#define IEoeAlgo_WeldRegSelectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldRegSelectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_PhaseCorrelationAlgo(This,rgnId,pMode,pAngle)	\
    ( (This)->lpVtbl -> PhaseCorrelationAlgo(This,rgnId,pMode,pAngle) ) 

#define IEoeAlgo_PhaseCorrelationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PhaseCorrelationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_SetCurrentTaskName2(This,bstrTaskName,bstrChName,bstrChNameWithoutCameraIndex)	\
    ( (This)->lpVtbl -> SetCurrentTaskName2(This,bstrTaskName,bstrChName,bstrChNameWithoutCameraIndex) ) 

#define IEoeAlgo_PhaseCorrelationRectangularRegionAlgo(This,rgnId,pWidthPlaceSet,pHeightPlaceSet,pRegionHeight,pRegionWidth)	\
    ( (This)->lpVtbl -> PhaseCorrelationRectangularRegionAlgo(This,rgnId,pWidthPlaceSet,pHeightPlaceSet,pRegionHeight,pRegionWidth) ) 

#define IEoeAlgo_PhaseCorrelationRectangularRegionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PhaseCorrelationRectangularRegionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_PhaseCorrelationRectangularAlgo(This,rgnId,pMode,pScore,pCalibrationImageID)	\
    ( (This)->lpVtbl -> PhaseCorrelationRectangularAlgo(This,rgnId,pMode,pScore,pCalibrationImageID) ) 

#define IEoeAlgo_PhaseCorrelationRectangularAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PhaseCorrelationRectangularAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CenterRectangleLocationAlgoSobel(This,rgnId,pSobelSize,pDisplayMode,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoSobel(This,rgnId,pSobelSize,pDisplayMode,pThresholdLow,pOutlierRemoverSize) ) 

#define IEoeAlgo_CenterRectangleLocationAlgoSobelHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoSobelHelp(This,pHelpStr) ) 

#define IEoeAlgo_EdgeDetectRectanglePixelWiseAlgo(This,rgnId,pDefectType,pThresholdLight,pThresholdDark,pSharpness,pDefectSizeLight,pDefectSizeDark,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeDetectRectanglePixelWiseAlgo(This,rgnId,pDefectType,pThresholdLight,pThresholdDark,pSharpness,pDefectSizeLight,pDefectSizeDark,pDetectArea) ) 

#define IEoeAlgo_EdgeDetectRectanglePixelWiseAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeDetectRectanglePixelWiseAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CurlOptimizeRectangleAlgo(This,rgnId,pPartsWidth,pFreqStart,pFreqCutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeRectangleAlgo(This,rgnId,pPartsWidth,pFreqStart,pFreqCutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IEoeAlgo_CurlOptimizeRectangleAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeRectangleAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_TabDeviationDetectAlgo(This,rgnId,pThreshold,pMinGray,pMaxGray,pTabLength,pDistanceDifference)	\
    ( (This)->lpVtbl -> TabDeviationDetectAlgo(This,rgnId,pThreshold,pMinGray,pMaxGray,pTabLength,pDistanceDifference) ) 

#define IEoeAlgo_TabDeviationDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TabDeviationDetectAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DerivateGaussDetectComprehensiveAlgo(This,rgnId,pSigma,pLowerLimit,pUpperLimit,pDisplayMode,pFeatureNumber,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectComprehensiveAlgo(This,rgnId,pSigma,pLowerLimit,pUpperLimit,pDisplayMode,pFeatureNumber,pDefectSize,pDetectArea) ) 

#define IEoeAlgo_DerivateGaussDetectComprehensiveAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectComprehensiveAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DetectRegionExcludingArbitraryRegionAlgo(This,rgnId,pChangingSize,pDoRegistration,bstrOcrRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionExcludingArbitraryRegionAlgo(This,rgnId,pChangingSize,pDoRegistration,bstrOcrRgnFile) ) 

#define IEoeAlgo_DetectRegionExcludingArbitraryRegionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionExcludingArbitraryRegionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DetectRegionRectangleLocationAlgo(This,rgnId,pChangingSize,pDoMatch,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionRectangleLocationAlgo(This,rgnId,pChangingSize,pDoMatch,bstrExtractRgnFile) ) 

#define IEoeAlgo_DetectRegionRectangleLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionRectangleLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_RegistrateRectangleAlgo(This,rgnId,pSigma,pThreshold,pCalibrationImageID)	\
    ( (This)->lpVtbl -> RegistrateRectangleAlgo(This,rgnId,pSigma,pThreshold,pCalibrationImageID) ) 

#define IEoeAlgo_RegistrateRectangleAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RegistrateRectangleAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_CenterRectangleLocationAlgoInnerEdge(This,rgnId,pLidWidth,pLidHeight,pThreshold,pSecondThreshold,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoInnerEdge(This,rgnId,pLidWidth,pLidHeight,pThreshold,pSecondThreshold,pOutlierRemoverSize) ) 

#define IEoeAlgo_CenterRectangleLocationAlgoInnerEdgeHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoInnerEdgeHelp(This,pHelpStr) ) 

#define IEoeAlgo_CenterRectangleSecondLocationAlgo(This,rgnId,pDilationSize,pThreshold,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterRectangleSecondLocationAlgo(This,rgnId,pDilationSize,pThreshold,pOutlierRemoverSize) ) 

#define IEoeAlgo_CenterRectangleSecondLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRectangleSecondLocationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_AngleRegistrationAlgo(This,rgnId,pPatternWidth,pThreshold,pAngle)	\
    ( (This)->lpVtbl -> AngleRegistrationAlgo(This,rgnId,pPatternWidth,pThreshold,pAngle) ) 

#define IEoeAlgo_AngleRegistrationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AngleRegistrationAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DiscontinuousLineDetectionAlgo(This,rgnId,pThreshold,pRequiredLineLength,pBrokenLineNumber)	\
    ( (This)->lpVtbl -> DiscontinuousLineDetectionAlgo(This,rgnId,pThreshold,pRequiredLineLength,pBrokenLineNumber) ) 

#define IEoeAlgo_DiscontinuousLineDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DiscontinuousLineDetectionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_TabDeviationDetectIndistinctEdgeAlgo(This,rgnId,pThreshold,pMinGray,pMaxGray,pTabLength,pDistanceDifference)	\
    ( (This)->lpVtbl -> TabDeviationDetectIndistinctEdgeAlgo(This,rgnId,pThreshold,pMinGray,pMaxGray,pTabLength,pDistanceDifference) ) 

#define IEoeAlgo_TabDeviationDetectIndistinctEdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TabDeviationDetectIndistinctEdgeAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_RectangleRoundRegionAlgo(This,rgnId,pChangingSize,pRoiWidth,pChamferRadius)	\
    ( (This)->lpVtbl -> RectangleRoundRegionAlgo(This,rgnId,pChangingSize,pRoiWidth,pChamferRadius) ) 

#define IEoeAlgo_RectangleRoundRegionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectangleRoundRegionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_RectangleEdgeRegionAlgo(This,rgnId,pChangingSize,pRoiWidth,pChamferRadius)	\
    ( (This)->lpVtbl -> RectangleEdgeRegionAlgo(This,rgnId,pChangingSize,pRoiWidth,pChamferRadius) ) 

#define IEoeAlgo_RectangleEdgeRegionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectangleEdgeRegionAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DefectDetectionRectanglePixelWiseAlgo(This,rgnId,pThreshold,pSharpness,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> DefectDetectionRectanglePixelWiseAlgo(This,rgnId,pThreshold,pSharpness,pDefectSize,pDefectArea) ) 

#define IEoeAlgo_DefectDetectionRectanglePixelWiseAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DefectDetectionRectanglePixelWiseAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_DefectDetectionRectanglePixelWiseDarkLightAlgo(This,rgnId,pSharpness,pThresholdLight,pDefectSizeLight,pThresholdDark,pDefectSizeDark,pDefectArea)	\
    ( (This)->lpVtbl -> DefectDetectionRectanglePixelWiseDarkLightAlgo(This,rgnId,pSharpness,pThresholdLight,pDefectSizeLight,pThresholdDark,pDefectSizeDark,pDefectArea) ) 

#define IEoeAlgo_DefectDetectionRectanglePixelWiseDarkLightAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DefectDetectionRectanglePixelWiseDarkLightAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_AnnularRotaDynThresholdAlgoRotateImage(This,rgnId,pRotaAngle,pThresholdDark,pDefectSizeDark,pThresholdLight,pDefectSizeLight,pDefectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoRotateImage(This,rgnId,pRotaAngle,pThresholdDark,pDefectSizeDark,pThresholdLight,pDefectSizeLight,pDefectArea) ) 

#define IEoeAlgo_AnnularRotaDynThresholdAlgoRotateImageHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoRotateImageHelp(This,pHelpStr) ) 

#define IEoeAlgo_RRegionDetectExcludeEdgeAlgo(This,rgnId,pEdgeThresh,pEdgeLength,pErrorThresh,pARMin,pDetectArea)	\
    ( (This)->lpVtbl -> RRegionDetectExcludeEdgeAlgo(This,rgnId,pEdgeThresh,pEdgeLength,pErrorThresh,pARMin,pDetectArea) ) 

#define IEoeAlgo_RRegionDetectExcludeEdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RRegionDetectExcludeEdgeAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_AngleRegistrationAlgo1(This,rgnId,pClosingSize,pThreshold,pAngle)	\
    ( (This)->lpVtbl -> AngleRegistrationAlgo1(This,rgnId,pClosingSize,pThreshold,pAngle) ) 

#define IEoeAlgo_AngleRegistrationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AngleRegistrationAlgo1Help(This,pHelpStr) ) 

#define IEoeAlgo_AngleRegistrationRotateImageAlgo(This,rgnId,pDynThreshold,pAreaThreshold,pTabLength,pParalleTab,pAngle)	\
    ( (This)->lpVtbl -> AngleRegistrationRotateImageAlgo(This,rgnId,pDynThreshold,pAreaThreshold,pTabLength,pParalleTab,pAngle) ) 

#define IEoeAlgo_AngleRegistrationRotateImageAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AngleRegistrationRotateImageAlgoHelp(This,pHelpStr) ) 

#define IEoeAlgo_TabDeviationDetectionHoughAlgo(This,rgnId,pGaussThreshold,pHoughThreshold,pDisplayEdge,pAngle)	\
    ( (This)->lpVtbl -> TabDeviationDetectionHoughAlgo(This,rgnId,pGaussThreshold,pHoughThreshold,pDisplayEdge,pAngle) ) 

#define IEoeAlgo_TabDeviationDetectionHoughAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TabDeviationDetectionHoughAlgoHelp(This,pHelpStr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_GlueInjectorIDDetectAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackPointThresh,
    /* [in] */ VARIANT *pWhitePointThresh,
    /* [in] */ VARIANT *pMeanGrayDiffValue,
    /* [in] */ VARIANT *pDeviationGrayDiffValue,
    /* [in] */ VARIANT *pMaxID,
    /* [retval][out] */ VARIANT *pDetectID);


void __RPC_STUB IEoeAlgo_GlueInjectorIDDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_GlueInjectorIDDetectAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_GlueInjectorIDDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_GlueInjectorIDDetectAlgo1_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRefreshIndex,
    /* [in] */ VARIANT *pStartIndex,
    /* [in] */ VARIANT *pTotalInjectorNum,
    /* [retval][out] */ VARIANT *pDetectID);


void __RPC_STUB IEoeAlgo_GlueInjectorIDDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_GlueInjectorIDDetectAlgo1Help_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_GlueInjectorIDDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_EccentricDetectionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [retval][out] */ VARIANT *pDistance);


void __RPC_STUB IEoeAlgo_EccentricDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_EccentricDetectionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_EccentricDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_InnerCircleLocationAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLowThred,
    /* [in] */ VARIANT *pHighThred,
    /* [in] */ VARIANT *pLowLength,
    /* [in] */ VARIANT *pHighLength);


void __RPC_STUB IEoeAlgo_InnerCircleLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_InnerCircleLocationAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ VARIANT *pHelpStr);


void __RPC_STUB IEoeAlgo_InnerCircleLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_ReprintDetectionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [retval][out] */ VARIANT *pGrayDiff);


void __RPC_STUB IEoeAlgo_ReprintDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_ReprintDetectionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_ReprintDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WLineDetectionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IEoeAlgo_WLineDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WLineDetectionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WLineDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WSpotDetectionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSpotMaskSize,
    /* [in] */ VARIANT *pSpotThred,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IEoeAlgo_WSpotDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WSpotDetectionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WSpotDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WBlockDetectionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlockThred,
    /* [in] */ VARIANT *pMinArea,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IEoeAlgo_WBlockDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WBlockDetectionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WBlockDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_ArrowDistOffsetAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLowThred,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pBlackPointUpSize,
    /* [retval][out] */ VARIANT *pDistanceOffset);


void __RPC_STUB IEoeAlgo_ArrowDistOffsetAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_ArrowDistOffsetAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_ArrowDistOffsetAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RingLocationForColorAlgoHS_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pOutlierRemoverSize,
    /* [in] */ VARIANT *pRegionWidth);


void __RPC_STUB IEoeAlgo_RingLocationForColorAlgoHS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RingLocationForColorAlgoHSHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_RingLocationForColorAlgoHSHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CenterLocationForColorAlgoHS_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pOutlierRemoverSize);


void __RPC_STUB IEoeAlgo_CenterLocationForColorAlgoHS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CenterLocationForColorAlgoHSHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_CenterLocationForColorAlgoHSHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CurlOptimizeForColorAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pFreq_start,
    /* [in] */ VARIANT *pFreqCutoff,
    /* [in] */ VARIANT *pWinType,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pWaveThreshold,
    /* [in] */ VARIANT *pMinArea,
    /* [retval][out] */ VARIANT *pDetectNumber);


void __RPC_STUB IEoeAlgo_CurlOptimizeForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CurlOptimizeForColorAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_CurlOptimizeForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DynThresholdForColorAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IEoeAlgo_DynThresholdForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DynThresholdForColorAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_DynThresholdForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_EdgeForColorAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pEdgeThred,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IEoeAlgo_EdgeForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_EdgeForColorAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_EdgeForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AnnularRotaDynThresholdForColorAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IEoeAlgo_AnnularRotaDynThresholdForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AnnularRotaDynThresholdForColorAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_AnnularRotaDynThresholdForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AnnularRotaDynThresholdForColorAlgo1_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IEoeAlgo_AnnularRotaDynThresholdForColorAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AnnularRotaDynThresholdForColorAlgo1Help_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_AnnularRotaDynThresholdForColorAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_GetCurrentLocationXY_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *bstrLocationXY);


void __RPC_STUB IEoeAlgo_GetCurrentLocationXY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_GetCenterLocation_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *bstrLocationXY);


void __RPC_STUB IEoeAlgo_GetCenterLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RivetCenterLocationAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnID,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pMinScore,
    /* [in] */ VARIANT *pThredScore);


void __RPC_STUB IEoeAlgo_RivetCenterLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RivetCenterLocationAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_RivetCenterLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RivetDiameterMeasureAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pOffset,
    /* [in] */ VARIANT *pSigma,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pCircularityValue,
    /* [retval][out] */ VARIANT *pDiameter);


void __RPC_STUB IEoeAlgo_RivetDiameterMeasureAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RivetDiameterMeasureAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_RivetDiameterMeasureAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RivetDiameterMeasureAlgo1_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pOffset,
    /* [retval][out] */ VARIANT *pDiameter);


void __RPC_STUB IEoeAlgo_RivetDiameterMeasureAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RivetDiameterMeasureAlgo1Help_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_RivetDiameterMeasureAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldAngleDetectAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [retval][out] */ VARIANT *pDetectAngle);


void __RPC_STUB IEoeAlgo_WeldAngleDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldAngleDetectAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WeldAngleDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldAngleDetectGrayAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAlgoSelect,
    /* [in] */ VARIANT *pWeldWidth,
    /* [in] */ VARIANT *pSmoothSize,
    /* [retval][out] */ VARIANT *pDetectAngle);


void __RPC_STUB IEoeAlgo_WeldAngleDetectGrayAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldAngleDetectGrayAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WeldAngleDetectGrayAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldAngleDetectWhiteGlueAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pDynThreshold,
    /* [in] */ VARIANT *pAspectRationThresh,
    /* [in] */ VARIANT *pClosingMaskWidth,
    /* [retval][out] */ VARIANT *pDetectAngle);


void __RPC_STUB IEoeAlgo_WeldAngleDetectWhiteGlueAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldAngleDetectWhiteGlueAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WeldAngleDetectWhiteGlueAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldAngleDetectGlueAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackWhite,
    /* [in] */ VARIANT *pEnhancePara,
    /* [in] */ VARIANT *pDynThreshold,
    /* [in] */ VARIANT *pAspectRationThresh,
    /* [in] */ VARIANT *pClosingMaskWidth,
    /* [retval][out] */ VARIANT *pDetectAngle);


void __RPC_STUB IEoeAlgo_WeldAngleDetectGlueAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldAngleDetectGlueAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WeldAngleDetectGlueAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldWhiteGlueRegExtractAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [retval][out] */ VARIANT *pWeldRegArea);


void __RPC_STUB IEoeAlgo_WeldWhiteGlueRegExtractAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldWhiteGlueRegExtractAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WeldWhiteGlueRegExtractAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldRegSelectAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pWeldSelection);


void __RPC_STUB IEoeAlgo_WeldRegSelectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_WeldRegSelectAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_WeldRegSelectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_PhaseCorrelationAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMode,
    /* [retval][out] */ VARIANT *pAngle);


void __RPC_STUB IEoeAlgo_PhaseCorrelationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_PhaseCorrelationAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_PhaseCorrelationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_SetCurrentTaskName2_Proxy( 
    IEoeAlgo * This,
    /* [in] */ BSTR *bstrTaskName,
    /* [in] */ BSTR *bstrChName,
    /* [in] */ BSTR *bstrChNameWithoutCameraIndex);


void __RPC_STUB IEoeAlgo_SetCurrentTaskName2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_PhaseCorrelationRectangularRegionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pWidthPlaceSet,
    /* [in] */ VARIANT *pHeightPlaceSet,
    /* [in] */ VARIANT *pRegionHeight,
    /* [in] */ VARIANT *pRegionWidth);


void __RPC_STUB IEoeAlgo_PhaseCorrelationRectangularRegionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_PhaseCorrelationRectangularRegionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_PhaseCorrelationRectangularRegionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_PhaseCorrelationRectangularAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMode,
    /* [in] */ VARIANT *pScore,
    /* [retval][out] */ VARIANT *pCalibrationImageID);


void __RPC_STUB IEoeAlgo_PhaseCorrelationRectangularAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_PhaseCorrelationRectangularAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_PhaseCorrelationRectangularAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CenterRectangleLocationAlgoSobel_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSobelSize,
    /* [in] */ VARIANT *pDisplayMode,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pOutlierRemoverSize);


void __RPC_STUB IEoeAlgo_CenterRectangleLocationAlgoSobel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CenterRectangleLocationAlgoSobelHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_CenterRectangleLocationAlgoSobelHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_EdgeDetectRectanglePixelWiseAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pThresholdLight,
    /* [in] */ VARIANT *pThresholdDark,
    /* [in] */ VARIANT *pSharpness,
    /* [in] */ VARIANT *pDefectSizeLight,
    /* [in] */ VARIANT *pDefectSizeDark,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IEoeAlgo_EdgeDetectRectanglePixelWiseAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_EdgeDetectRectanglePixelWiseAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_EdgeDetectRectanglePixelWiseAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CurlOptimizeRectangleAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pPartsWidth,
    /* [in] */ VARIANT *pFreqStart,
    /* [in] */ VARIANT *pFreqCutoff,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pWaveThreshold,
    /* [in] */ VARIANT *pMinArea,
    /* [retval][out] */ VARIANT *pDetectNumber);


void __RPC_STUB IEoeAlgo_CurlOptimizeRectangleAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CurlOptimizeRectangleAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_CurlOptimizeRectangleAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_TabDeviationDetectAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pMinGray,
    /* [in] */ VARIANT *pMaxGray,
    /* [in] */ VARIANT *pTabLength,
    /* [retval][out] */ VARIANT *pDistanceDifference);


void __RPC_STUB IEoeAlgo_TabDeviationDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_TabDeviationDetectAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_TabDeviationDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DerivateGaussDetectComprehensiveAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSigma,
    /* [in] */ VARIANT *pLowerLimit,
    /* [in] */ VARIANT *pUpperLimit,
    /* [in] */ VARIANT *pDisplayMode,
    /* [in] */ VARIANT *pFeatureNumber,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IEoeAlgo_DerivateGaussDetectComprehensiveAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DerivateGaussDetectComprehensiveAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_DerivateGaussDetectComprehensiveAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DetectRegionExcludingArbitraryRegionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChangingSize,
    /* [in] */ VARIANT *pDoRegistration,
    /* [in] */ BSTR *bstrOcrRgnFile);


void __RPC_STUB IEoeAlgo_DetectRegionExcludingArbitraryRegionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DetectRegionExcludingArbitraryRegionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_DetectRegionExcludingArbitraryRegionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DetectRegionRectangleLocationAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChangingSize,
    /* [in] */ VARIANT *pDoMatch,
    /* [in] */ BSTR *bstrExtractRgnFile);


void __RPC_STUB IEoeAlgo_DetectRegionRectangleLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DetectRegionRectangleLocationAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_DetectRegionRectangleLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RegistrateRectangleAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSigma,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pCalibrationImageID);


void __RPC_STUB IEoeAlgo_RegistrateRectangleAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RegistrateRectangleAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_RegistrateRectangleAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CenterRectangleLocationAlgoInnerEdge_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLidWidth,
    /* [in] */ VARIANT *pLidHeight,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pSecondThreshold,
    /* [in] */ VARIANT *pOutlierRemoverSize);


void __RPC_STUB IEoeAlgo_CenterRectangleLocationAlgoInnerEdge_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CenterRectangleLocationAlgoInnerEdgeHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_CenterRectangleLocationAlgoInnerEdgeHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CenterRectangleSecondLocationAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pOutlierRemoverSize);


void __RPC_STUB IEoeAlgo_CenterRectangleSecondLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_CenterRectangleSecondLocationAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_CenterRectangleSecondLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AngleRegistrationAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pPatternWidth,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pAngle);


void __RPC_STUB IEoeAlgo_AngleRegistrationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AngleRegistrationAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_AngleRegistrationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DiscontinuousLineDetectionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pRequiredLineLength,
    /* [retval][out] */ VARIANT *pBrokenLineNumber);


void __RPC_STUB IEoeAlgo_DiscontinuousLineDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DiscontinuousLineDetectionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_DiscontinuousLineDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_TabDeviationDetectIndistinctEdgeAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pMinGray,
    /* [in] */ VARIANT *pMaxGray,
    /* [in] */ VARIANT *pTabLength,
    /* [retval][out] */ VARIANT *pDistanceDifference);


void __RPC_STUB IEoeAlgo_TabDeviationDetectIndistinctEdgeAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_TabDeviationDetectIndistinctEdgeAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_TabDeviationDetectIndistinctEdgeAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RectangleRoundRegionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChangingSize,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pChamferRadius);


void __RPC_STUB IEoeAlgo_RectangleRoundRegionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RectangleRoundRegionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_RectangleRoundRegionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RectangleEdgeRegionAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChangingSize,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pChamferRadius);


void __RPC_STUB IEoeAlgo_RectangleEdgeRegionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RectangleEdgeRegionAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_RectangleEdgeRegionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DefectDetectionRectanglePixelWiseAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pSharpness,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IEoeAlgo_DefectDetectionRectanglePixelWiseAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DefectDetectionRectanglePixelWiseAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_DefectDetectionRectanglePixelWiseAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DefectDetectionRectanglePixelWiseDarkLightAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSharpness,
    /* [in] */ VARIANT *pThresholdLight,
    /* [in] */ VARIANT *pDefectSizeLight,
    /* [in] */ VARIANT *pThresholdDark,
    /* [in] */ VARIANT *pDefectSizeDark,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IEoeAlgo_DefectDetectionRectanglePixelWiseDarkLightAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_DefectDetectionRectanglePixelWiseDarkLightAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_DefectDetectionRectanglePixelWiseDarkLightAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AnnularRotaDynThresholdAlgoRotateImage_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pThresholdDark,
    /* [in] */ VARIANT *pDefectSizeDark,
    /* [in] */ VARIANT *pThresholdLight,
    /* [in] */ VARIANT *pDefectSizeLight,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IEoeAlgo_AnnularRotaDynThresholdAlgoRotateImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AnnularRotaDynThresholdAlgoRotateImageHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_AnnularRotaDynThresholdAlgoRotateImageHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RRegionDetectExcludeEdgeAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pEdgeThresh,
    /* [in] */ VARIANT *pEdgeLength,
    /* [in] */ VARIANT *pErrorThresh,
    /* [in] */ VARIANT *pARMin,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IEoeAlgo_RRegionDetectExcludeEdgeAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_RRegionDetectExcludeEdgeAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_RRegionDetectExcludeEdgeAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AngleRegistrationAlgo1_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pClosingSize,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pAngle);


void __RPC_STUB IEoeAlgo_AngleRegistrationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AngleRegistrationAlgo1Help_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_AngleRegistrationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AngleRegistrationRotateImageAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pDynThreshold,
    /* [in] */ VARIANT *pAreaThreshold,
    /* [in] */ VARIANT *pTabLength,
    /* [in] */ VARIANT *pParalleTab,
    /* [retval][out] */ VARIANT *pAngle);


void __RPC_STUB IEoeAlgo_AngleRegistrationRotateImageAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_AngleRegistrationRotateImageAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_AngleRegistrationRotateImageAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_TabDeviationDetectionHoughAlgo_Proxy( 
    IEoeAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGaussThreshold,
    /* [in] */ VARIANT *pHoughThreshold,
    /* [in] */ VARIANT *pDisplayEdge,
    /* [retval][out] */ VARIANT *pAngle);


void __RPC_STUB IEoeAlgo_TabDeviationDetectionHoughAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IEoeAlgo_TabDeviationDetectionHoughAlgoHelp_Proxy( 
    IEoeAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IEoeAlgo_TabDeviationDetectionHoughAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEoeAlgo_INTERFACE_DEFINED__ */



#ifndef __EoeAlgoLib_LIBRARY_DEFINED__
#define __EoeAlgoLib_LIBRARY_DEFINED__

/* library EoeAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_EoeAlgoLib;

EXTERN_C const CLSID CLSID_EoeAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("2B5AEFC9-EA40-4F76-8064-E72C3425C4D9")
EoeAlgo;
#endif
#endif /* __EoeAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


