

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Dec 13 11:35:43 2019
 */
/* Compiler settings for BottleCapAlgo.idl:
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

#ifndef __BottleCapAlgo_i_h__
#define __BottleCapAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBottleCapAlgo_FWD_DEFINED__
#define __IBottleCapAlgo_FWD_DEFINED__
typedef interface IBottleCapAlgo IBottleCapAlgo;
#endif 	/* __IBottleCapAlgo_FWD_DEFINED__ */


#ifndef __BottleCapAlgo_FWD_DEFINED__
#define __BottleCapAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class BottleCapAlgo BottleCapAlgo;
#else
typedef struct BottleCapAlgo BottleCapAlgo;
#endif /* __cplusplus */

#endif 	/* __BottleCapAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IBottleCapAlgo_INTERFACE_DEFINED__
#define __IBottleCapAlgo_INTERFACE_DEFINED__

/* interface IBottleCapAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IBottleCapAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("88A26EAF-5D85-493E-BFB2-E2BF8AEAEE21")
    IBottleCapAlgo : public IDispatch
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
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetSelfLearningTargetTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetGlueInjectorID( 
            /* [out][in] */ LONG *pInjectorID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetInterImgs( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetDetectParam( 
            /* [out][in] */ LONG *pDetectParam) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetDetectParam( 
            /* [out][in] */ LONG *pDetectParam) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentTaskName2( 
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName,
            /* [in] */ BSTR *bstrChNameWithoutCameraIndex) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRotaDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRotaDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SubDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SubDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterTopLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThresholdValue) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterTopLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BaseEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pPointSelection,
            /* [in] */ VARIANT *pRemoveSize,
            /* [retval][out] */ VARIANT *pBaseEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BaseEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE UpCharLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMode,
            /* [in] */ VARIANT *pRegionRow,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pCharWidth,
            /* [in] */ VARIANT *pCharHeight,
            /* [in] */ VARIANT *pSlashWidth,
            /* [in] */ VARIANT *pSlashHeight,
            /* [in] */ VARIANT *pDistanceCharChar,
            /* [in] */ VARIANT *pDistanceCharSlash,
            /* [in] */ VARIANT *pMoveDistance,
            /* [retval][out] */ VARIANT *pCharArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE UpCharLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE UpCharRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelectmodel,
            /* [in] */ VARIANT *pRegionRow,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth,
            /* [in] */ VARIANT *pCharSize,
            /* [in] */ VARIANT *pSlashSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE UpCharRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoNew( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoNewHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LowCharLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pCharSize,
            /* [in] */ VARIANT *pSlashSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pMaxArea,
            /* [retval][out] */ VARIANT *pCharFlag) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LowCharLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LowCharRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelectmodel,
            /* [in] */ VARIANT *pRegionRow,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth,
            /* [in] */ VARIANT *pCharSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LowCharRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgoNew( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgoNewHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileAlgo( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileAlgoNew( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileAlgoNewHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectExcludeEdgesAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pMinEdgeSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectExcludeEdgesAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_SideCenterTopLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pAreaSize,
            /* [in] */ VARIANT *pDownPose) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_SideCenterTopLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_SideCenterBodyLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pLocationSelect,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThresholdValue) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_SideCenterBodyLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_BaseEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pPointSelection,
            /* [in] */ VARIANT *pRemoveSize,
            /* [retval][out] */ VARIANT *pBaseEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_BaseEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_SideEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_SideEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_PatternRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMode,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRegionSelect,
            /* [in] */ VARIANT *pPoseAdjustRow,
            /* [in] */ VARIANT *pPoseAdjustCol,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pRowBgn,
            /* [in] */ VARIANT *pColBgn,
            /* [in] */ VARIANT *pSmlCharHeight,
            /* [in] */ VARIANT *pSmlCharWidth,
            /* [in] */ VARIANT *pSmlCharSpace,
            /* [in] */ VARIANT *pCharDistHeight,
            /* [in] */ VARIANT *pCharDistWidth,
            /* [in] */ VARIANT *pBigCharHeight,
            /* [in] */ VARIANT *pBigCharWidth,
            /* [in] */ VARIANT *pBigCharSpace,
            /* [in] */ VARIANT *pCircleWidth,
            /* [in] */ VARIANT *pCircleHeight) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_PatternRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_PatternReferenceLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pSerchRowPos,
            /* [in] */ VARIANT *pSerchWidth,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_PatternReferenceLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_RemovePatternRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColBgn,
            /* [in] */ VARIANT *pColEnd,
            /* [in] */ VARIANT *pRowBgn,
            /* [in] */ VARIANT *pRowEnd,
            /* [in] */ VARIANT *pScaleSizeH,
            /* [in] */ VARIANT *pScaleSizeV) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_RemovePatternRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_TeethRegionDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRowBgn,
            /* [in] */ VARIANT *pRgnHeight,
            /* [in] */ VARIANT *pDarkThred,
            /* [in] */ VARIANT *pTeethPosUp,
            /* [in] */ VARIANT *pTeethPosDown,
            /* [in] */ VARIANT *pLightThred,
            /* [in] */ VARIANT *pLightAreaLimit,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_TeethRegionDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_TeethOffsetDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pTeethWidth,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_TeethOffsetDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_DeformableModelDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMatchScore,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pAreaSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_DeformableModelDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundForClolorNewAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundForClolorNewAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideWallDynThresholdAlgoNew( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideWallDynThresholdAlgoNewHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_TeethRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRegionPos,
            /* [in] */ VARIANT *pUpSize,
            /* [in] */ VARIANT *pDownSize,
            /* [in] */ VARIANT *pScaleSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_TeethRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideWallDynThresholdAlgo( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideWallDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_TeethDisappearDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pAreaSize,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_TeethDisappearDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_PatternRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pScaleSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_PatternRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_PatternDefectDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pWidthSelect,
            /* [in] */ VARIANT *pWidthThred,
            /* [in] */ VARIANT *pPatternRatio,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PS_PatternDefectDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBottleCapAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBottleCapAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBottleCapAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBottleCapAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBottleCapAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBottleCapAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBottleCapAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBottleCapAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            IBottleCapAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            IBottleCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            IBottleCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhiteErrorRegion )( 
            IBottleCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pWhiteErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            IBottleCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            IBottleCapAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            IBottleCapAlgo * This,
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            IBottleCapAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetGlueInjectorID )( 
            IBottleCapAlgo * This,
            /* [out][in] */ LONG *pInjectorID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInterImgs )( 
            IBottleCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetDetectParam )( 
            IBottleCapAlgo * This,
            /* [out][in] */ LONG *pDetectParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetDetectParam )( 
            IBottleCapAlgo * This,
            /* [out][in] */ LONG *pDetectParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            IBottleCapAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            IBottleCapAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            IBottleCapAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            IBottleCapAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            IBottleCapAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            IBottleCapAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            IBottleCapAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            IBottleCapAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            IBottleCapAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegionEx )( 
            IBottleCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegionExTest )( 
            IBottleCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHS )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHSHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoHS )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoHSHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFitting )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pNumLevels,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RotatingAngleDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pEdgeType,
            /* [in] */ VARIANT *pEdgeDist,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RotatingAngleDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoOCR )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoOCRHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationExtractOCRAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrOcrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationExtractOCRAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo1 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo1Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pWinType,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo1 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo1Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo2 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo2Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineNewAlgo )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GContoursinkAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GContoursinkAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlack )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackNumber )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackNumberHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhite )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteNumber )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteNumberHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackWhiteNumber )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackWhiteNumberHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoOCR )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoOCRHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MinMaxGrayDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MinMaxGrayDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBW )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBWHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo1 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo1Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSmoothPara,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pARMin,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectBigAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDiffWhite,
            /* [in] */ VARIANT *pARMax,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RRegionDetectBigAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectOcrDistOffsetAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistanceOffset);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectOcrDistOffsetAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoTabOCR )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoTabOCRHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoTabOCR )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoTabOCRHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MultOcrDetectAlgo )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MultOcrDetectExcludeTabAlgo )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PrintErrorDetectOcrAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pHeightContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PrintErrorDetectOcrAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorDetectAlgo )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueDetectAlgo )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorIDDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackPointThresh,
            /* [in] */ VARIANT *pWhitePointThresh,
            /* [in] */ VARIANT *pMeanGrayDiffValue,
            /* [in] */ VARIANT *pDeviationGrayDiffValue,
            /* [in] */ VARIANT *pMaxID,
            /* [retval][out] */ VARIANT *pDetectID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorIDDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorIDDetectAlgo1 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRefreshIndex,
            /* [in] */ VARIANT *pStartIndex,
            /* [in] */ VARIANT *pTotalInjectorNum,
            /* [retval][out] */ VARIANT *pDetectID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GlueInjectorIDDetectAlgo1Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EccentricDetectionAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistance);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EccentricDetectionAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InnerCircleLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowThred,
            /* [in] */ VARIANT *pHighThred,
            /* [in] */ VARIANT *pLowLength,
            /* [in] */ VARIANT *pHighLength);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InnerCircleLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ VARIANT *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReprintDetectionAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReprintDetectionAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WLineDetectionAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WLineDetectionAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WSpotDetectionAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSpotMaskSize,
            /* [in] */ VARIANT *pSpotThred,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WSpotDetectionAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WBlockDetectionAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlockThred,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WBlockDetectionAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ArrowDistOffsetAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowThred,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [retval][out] */ VARIANT *pDistanceOffset);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ArrowDistOffsetAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RingLocationForColorAlgoHS )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pRegionWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RingLocationForColorAlgoHSHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationForColorAlgoHS )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationForColorAlgoHSHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeForColorAlgo )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorAlgo )( 
            IBottleCapAlgo * This,
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
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo1 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo1Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentLocationXY )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCenterLocation )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName2 )( 
            IBottleCapAlgo * This,
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName,
            /* [in] */ BSTR *bstrChNameWithoutCameraIndex);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRotaDynThresholdAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRotaDynThresholdAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SubDynThresholdAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SubDynThresholdAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundColorAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundColorAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterTopLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThresholdValue);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterTopLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BaseEdgeDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pPointSelection,
            /* [in] */ VARIANT *pRemoveSize,
            /* [retval][out] */ VARIANT *pBaseEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BaseEdgeDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideEdgeRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideEdgeRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopEdgeRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopEdgeRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UpCharLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMode,
            /* [in] */ VARIANT *pRegionRow,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pCharWidth,
            /* [in] */ VARIANT *pCharHeight,
            /* [in] */ VARIANT *pSlashWidth,
            /* [in] */ VARIANT *pSlashHeight,
            /* [in] */ VARIANT *pDistanceCharChar,
            /* [in] */ VARIANT *pDistanceCharSlash,
            /* [in] */ VARIANT *pMoveDistance,
            /* [retval][out] */ VARIANT *pCharArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UpCharLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UpCharRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelectmodel,
            /* [in] */ VARIANT *pRegionRow,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth,
            /* [in] */ VARIANT *pCharSize,
            /* [in] */ VARIANT *pSlashSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UpCharRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoNew )( 
            IBottleCapAlgo * This,
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
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoNewHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LowCharLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pCharSize,
            /* [in] */ VARIANT *pSlashSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pMaxArea,
            /* [retval][out] */ VARIANT *pCharFlag);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LowCharLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LowCharRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelectmodel,
            /* [in] */ VARIANT *pRegionRow,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth,
            /* [in] */ VARIANT *pCharSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LowCharRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoNew )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoNewHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgo )( 
            IBottleCapAlgo * This,
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
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgoNew )( 
            IBottleCapAlgo * This,
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
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgoNewHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionColorAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionColorAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectExcludeEdgesAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pMinEdgeSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectExcludeEdgesAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_SideCenterTopLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pAreaSize,
            /* [in] */ VARIANT *pDownPose);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_SideCenterTopLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_SideCenterBodyLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pLocationSelect,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThresholdValue);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_SideCenterBodyLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_BaseEdgeDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pPointSelection,
            /* [in] */ VARIANT *pRemoveSize,
            /* [retval][out] */ VARIANT *pBaseEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_BaseEdgeDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_SideEdgeRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_SideEdgeRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_PatternRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMode,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRegionSelect,
            /* [in] */ VARIANT *pPoseAdjustRow,
            /* [in] */ VARIANT *pPoseAdjustCol,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pRowBgn,
            /* [in] */ VARIANT *pColBgn,
            /* [in] */ VARIANT *pSmlCharHeight,
            /* [in] */ VARIANT *pSmlCharWidth,
            /* [in] */ VARIANT *pSmlCharSpace,
            /* [in] */ VARIANT *pCharDistHeight,
            /* [in] */ VARIANT *pCharDistWidth,
            /* [in] */ VARIANT *pBigCharHeight,
            /* [in] */ VARIANT *pBigCharWidth,
            /* [in] */ VARIANT *pBigCharSpace,
            /* [in] */ VARIANT *pCircleWidth,
            /* [in] */ VARIANT *pCircleHeight);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_PatternRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_PatternReferenceLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pSerchRowPos,
            /* [in] */ VARIANT *pSerchWidth,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_PatternReferenceLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_RemovePatternRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColBgn,
            /* [in] */ VARIANT *pColEnd,
            /* [in] */ VARIANT *pRowBgn,
            /* [in] */ VARIANT *pRowEnd,
            /* [in] */ VARIANT *pScaleSizeH,
            /* [in] */ VARIANT *pScaleSizeV);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_RemovePatternRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_TeethRegionDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRowBgn,
            /* [in] */ VARIANT *pRgnHeight,
            /* [in] */ VARIANT *pDarkThred,
            /* [in] */ VARIANT *pTeethPosUp,
            /* [in] */ VARIANT *pTeethPosDown,
            /* [in] */ VARIANT *pLightThred,
            /* [in] */ VARIANT *pLightAreaLimit,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_TeethRegionDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_TeethOffsetDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pTeethWidth,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_TeethOffsetDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_DeformableModelDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMatchScore,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pAreaSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_DeformableModelDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundForClolorNewAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pChannelSelect,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundForClolorNewAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideWallDynThresholdAlgoNew )( 
            IBottleCapAlgo * This,
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
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideWallDynThresholdAlgoNewHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_TeethRegionLocationAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRegionPos,
            /* [in] */ VARIANT *pUpSize,
            /* [in] */ VARIANT *pDownSize,
            /* [in] */ VARIANT *pScaleSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_TeethRegionLocationAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideWallDynThresholdAlgo )( 
            IBottleCapAlgo * This,
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
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideWallDynThresholdAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_TeethDisappearDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pAreaSize,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_TeethDisappearDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_PatternRegionLocationAlgo1 )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pScaleSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_PatternRegionLocationAlgo1Help )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_PatternDefectDetectAlgo )( 
            IBottleCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pWidthSelect,
            /* [in] */ VARIANT *pWidthThred,
            /* [in] */ VARIANT *pPatternRatio,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PS_PatternDefectDetectAlgoHelp )( 
            IBottleCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        END_INTERFACE
    } IBottleCapAlgoVtbl;

    interface IBottleCapAlgo
    {
        CONST_VTBL struct IBottleCapAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBottleCapAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBottleCapAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBottleCapAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBottleCapAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBottleCapAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBottleCapAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBottleCapAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBottleCapAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define IBottleCapAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define IBottleCapAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define IBottleCapAlgo_GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn)	\
    ( (This)->lpVtbl -> GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn) ) 

#define IBottleCapAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define IBottleCapAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define IBottleCapAlgo_SetCurrentTaskName(This,bstrTaskName,bstrChName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName,bstrChName) ) 

#define IBottleCapAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#define IBottleCapAlgo_GetGlueInjectorID(This,pInjectorID)	\
    ( (This)->lpVtbl -> GetGlueInjectorID(This,pInjectorID) ) 

#define IBottleCapAlgo_GetInterImgs(This,rgnID,pInterImgs)	\
    ( (This)->lpVtbl -> GetInterImgs(This,rgnID,pInterImgs) ) 

#define IBottleCapAlgo_GetDetectParam(This,pDetectParam)	\
    ( (This)->lpVtbl -> GetDetectParam(This,pDetectParam) ) 

#define IBottleCapAlgo_SetDetectParam(This,pDetectParam)	\
    ( (This)->lpVtbl -> SetDetectParam(This,pDetectParam) ) 

#define IBottleCapAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define IBottleCapAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define IBottleCapAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define IBottleCapAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define IBottleCapAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define IBottleCapAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define IBottleCapAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define IBottleCapAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define IBottleCapAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define IBottleCapAlgo_GetRegionEx(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegionEx(This,rgnID,pRgn) ) 

#define IBottleCapAlgo_GetRegionExTest(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegionExTest(This,rgnID,pRgn) ) 

#define IBottleCapAlgo_CenterLocationAlgo(This,rgnId,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnId,pRadius,pMinScore) ) 

#define IBottleCapAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CenterLocationAlgo1(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1(This,rgnID,pRadius,pMinScore) ) 

#define IBottleCapAlgo_CenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1Help(This,pHelpStr) ) 

#define IBottleCapAlgo_CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define IBottleCapAlgo_CenterLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHSHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CenterRectangleLocationAlgoHS(This,rgnId,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoHS(This,rgnId,pThresholdLow,pOutlierRemoverSize) ) 

#define IBottleCapAlgo_CenterRectangleLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoHSHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold) ) 

#define IBottleCapAlgo_CenterLocationAlgoUsePointFittingHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFittingHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IBottleCapAlgo_MatchingParaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IBottleCapAlgo_MatchingParaMinScoreAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_RotatingAngleDetectAlgo(This,rgnId,pMaskSize,pBlackThredLow,pEdgeType,pEdgeDist,pDefectSize,pDetectAngle)	\
    ( (This)->lpVtbl -> RotatingAngleDetectAlgo(This,rgnId,pMaskSize,pBlackThredLow,pEdgeType,pEdgeDist,pDefectSize,pDetectAngle) ) 

#define IBottleCapAlgo_RotatingAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RotatingAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IBottleCapAlgo_CircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IBottleCapAlgo_CircleRegionAngleAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IBottleCapAlgo_DetectRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DetectRegionLocationAlgoOCR(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoOCR(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IBottleCapAlgo_DetectRegionLocationAlgoOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoOCRHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DetectRegionLocationExtractOCRAlgo(This,rgnId,pRadius,pErosionSize,bstrOcrRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationExtractOCRAlgo(This,rgnId,pRadius,pErosionSize,bstrOcrRgnFile) ) 

#define IBottleCapAlgo_DetectRegionLocationExtractOCRAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationExtractOCRAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth) ) 

#define IBottleCapAlgo_CircleRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IBottleCapAlgo_CircleRegionAngleAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IBottleCapAlgo_CircleRegionAngleAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo1Help(This,pHelpStr) ) 

#define IBottleCapAlgo_BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IBottleCapAlgo_BoundaryRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IBottleCapAlgo_CurlOptimizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CurlDetectAlgo1(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo1(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define IBottleCapAlgo_CurlDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgo1Help(This,pHelpStr) ) 

#define IBottleCapAlgo_CurlDetectAlgo2(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo2(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define IBottleCapAlgo_CurlDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgo2Help(This,pHelpStr) ) 

#define IBottleCapAlgo_UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea) ) 

#define IBottleCapAlgo_UVCurlAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> UVCurlAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IBottleCapAlgo_AnnularRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CurlNoCompoundAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgo(This,rgnId,pGrayValueMean) ) 

#define IBottleCapAlgo_CurlNoCompoundAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IBottleCapAlgo_GSurfaceAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GSurfaceAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IBottleCapAlgo_GLineAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_GLineNewAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteThredHigh,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineNewAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteThredHigh,pWhiteDefectSize,pDetectArea) ) 

#define IBottleCapAlgo_GLineNewAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineNewAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IBottleCapAlgo_EdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_GContoursinkAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GContoursinkAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IBottleCapAlgo_GContoursinkAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GContoursinkAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBottleCapAlgo_DynThresholdAlgoBlackHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgoBlackNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectNumber) ) 

#define IBottleCapAlgo_DynThresholdAlgoBlackNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackNumberHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_DynThresholdAlgoWhiteHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgoWhiteNumber(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteNumber(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber) ) 

#define IBottleCapAlgo_DynThresholdAlgoWhiteNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteNumberHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgoBlackWhiteNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackWhiteNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber) ) 

#define IBottleCapAlgo_DynThresholdAlgoBlackWhiteNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackWhiteNumberHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgoOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea) ) 

#define IBottleCapAlgo_DynThresholdAlgoOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoOCRHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBottleCapAlgo_BlackDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_MinMaxGrayDetectAlgo(This,rgnId,pBlackThresh,pSeriousBlackPointSize,pWhiteThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> MinMaxGrayDetectAlgo(This,rgnId,pBlackThresh,pSeriousBlackPointSize,pWhiteThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_MinMaxGrayDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MinMaxGrayDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IBottleCapAlgo_AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IBottleCapAlgo_SectorRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_SectorRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IBottleCapAlgo_SectorRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IBottleCapAlgo_RRegionDetectAlgo(This,rgnId,pSmoothPara,pMaskSize,pThredLow,pARMin,pDetectArea)	\
    ( (This)->lpVtbl -> RRegionDetectAlgo(This,rgnId,pSmoothPara,pMaskSize,pThredLow,pARMin,pDetectArea) ) 

#define IBottleCapAlgo_RRegionDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RRegionDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_RRegionDetectBigAlgo(This,rgnId,pDiffWhite,pARMax,pDetectArea)	\
    ( (This)->lpVtbl -> RRegionDetectBigAlgo(This,rgnId,pDiffWhite,pARMax,pDetectArea) ) 

#define IBottleCapAlgo_RRegionDetectBigAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RRegionDetectBigAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DetectOcrDistOffsetAlgo(This,rgnId,pDistanceOffset)	\
    ( (This)->lpVtbl -> DetectOcrDistOffsetAlgo(This,rgnId,pDistanceOffset) ) 

#define IBottleCapAlgo_DetectOcrDistOffsetAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectOcrDistOffsetAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DetectRegionLocationAlgoTabOCR(This,rgnId,pErosionSize,bstrRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoTabOCR(This,rgnId,pErosionSize,bstrRgnFile) ) 

#define IBottleCapAlgo_DetectRegionLocationAlgoTabOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoTabOCRHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgoTabOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoTabOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea) ) 

#define IBottleCapAlgo_DynThresholdAlgoTabOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoTabOCRHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_MultOcrDetectAlgo(This,rgnId,pGreediness,bstrShm1File,bstrShm2File,bstrShm3File,bstrShm4File,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pLineDetectFlage,pLowContrast,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> MultOcrDetectAlgo(This,rgnId,pGreediness,bstrShm1File,bstrShm2File,bstrShm3File,bstrShm4File,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pLineDetectFlage,pLowContrast,pDilationSize,pDetectArea) ) 

#define IBottleCapAlgo_MultOcrDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MultOcrDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_MultOcrDetectExcludeTabAlgo(This,rgnId,pGreediness,bstrShm1File,bstrShm2File,bstrShm3File,bstrShm4File,bstrExtractRgnFile,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pLineDetectFlage,pLowContrast,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> MultOcrDetectExcludeTabAlgo(This,rgnId,pGreediness,bstrShm1File,bstrShm2File,bstrShm3File,bstrShm4File,bstrExtractRgnFile,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pLineDetectFlage,pLowContrast,pDilationSize,pDetectArea) ) 

#define IBottleCapAlgo_MultOcrDetectExcludeTabAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MultOcrDetectExcludeTabAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PrintErrorDetectOcrAlgo(This,rgnId,pLowContrast,pHeightContrast,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> PrintErrorDetectOcrAlgo(This,rgnId,pLowContrast,pHeightContrast,pDilationSize,pDetectArea) ) 

#define IBottleCapAlgo_PrintErrorDetectOcrAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PrintErrorDetectOcrAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_GlueInjectorDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pGrayDiffValue,pDetectArea)	\
    ( (This)->lpVtbl -> GlueInjectorDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pGrayDiffValue,pDetectArea) ) 

#define IBottleCapAlgo_GlueInjectorDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GlueInjectorDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_GlueDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pGrayDiffValue,pDetectArea)	\
    ( (This)->lpVtbl -> GlueDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pGrayDiffValue,pDetectArea) ) 

#define IBottleCapAlgo_GlueDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GlueDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_GlueInjectorIDDetectAlgo(This,rgnId,pBlackPointThresh,pWhitePointThresh,pMeanGrayDiffValue,pDeviationGrayDiffValue,pMaxID,pDetectID)	\
    ( (This)->lpVtbl -> GlueInjectorIDDetectAlgo(This,rgnId,pBlackPointThresh,pWhitePointThresh,pMeanGrayDiffValue,pDeviationGrayDiffValue,pMaxID,pDetectID) ) 

#define IBottleCapAlgo_GlueInjectorIDDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GlueInjectorIDDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_GlueInjectorIDDetectAlgo1(This,rgnId,pRefreshIndex,pStartIndex,pTotalInjectorNum,pDetectID)	\
    ( (This)->lpVtbl -> GlueInjectorIDDetectAlgo1(This,rgnId,pRefreshIndex,pStartIndex,pTotalInjectorNum,pDetectID) ) 

#define IBottleCapAlgo_GlueInjectorIDDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> GlueInjectorIDDetectAlgo1Help(This,pHelpStr) ) 

#define IBottleCapAlgo_EccentricDetectionAlgo(This,rgnId,pDistance)	\
    ( (This)->lpVtbl -> EccentricDetectionAlgo(This,rgnId,pDistance) ) 

#define IBottleCapAlgo_EccentricDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EccentricDetectionAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_InnerCircleLocationAlgo(This,rgnId,pLowThred,pHighThred,pLowLength,pHighLength)	\
    ( (This)->lpVtbl -> InnerCircleLocationAlgo(This,rgnId,pLowThred,pHighThred,pLowLength,pHighLength) ) 

#define IBottleCapAlgo_InnerCircleLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> InnerCircleLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_ReprintDetectionAlgo(This,rgnId,pGrayDiff)	\
    ( (This)->lpVtbl -> ReprintDetectionAlgo(This,rgnId,pGrayDiff) ) 

#define IBottleCapAlgo_ReprintDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ReprintDetectionAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_WLineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectArea)	\
    ( (This)->lpVtbl -> WLineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectArea) ) 

#define IBottleCapAlgo_WLineDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WLineDetectionAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_WSpotDetectionAlgo(This,rgnId,pSpotMaskSize,pSpotThred,pDefectArea)	\
    ( (This)->lpVtbl -> WSpotDetectionAlgo(This,rgnId,pSpotMaskSize,pSpotThred,pDefectArea) ) 

#define IBottleCapAlgo_WSpotDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WSpotDetectionAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_WBlockDetectionAlgo(This,rgnId,pBlockThred,pMinArea,pDefectArea)	\
    ( (This)->lpVtbl -> WBlockDetectionAlgo(This,rgnId,pBlockThred,pMinArea,pDefectArea) ) 

#define IBottleCapAlgo_WBlockDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WBlockDetectionAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_ArrowDistOffsetAlgo(This,rgnId,pLowThred,pErosionSize,pBlackPointUpSize,pDistanceOffset)	\
    ( (This)->lpVtbl -> ArrowDistOffsetAlgo(This,rgnId,pLowThred,pErosionSize,pBlackPointUpSize,pDistanceOffset) ) 

#define IBottleCapAlgo_ArrowDistOffsetAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ArrowDistOffsetAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_RingLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize,pRegionWidth)	\
    ( (This)->lpVtbl -> RingLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize,pRegionWidth) ) 

#define IBottleCapAlgo_RingLocationForColorAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RingLocationForColorAlgoHSHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CenterLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define IBottleCapAlgo_CenterLocationForColorAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationForColorAlgoHSHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CurlOptimizeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pFreq_start,pFreqCutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pFreq_start,pFreqCutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IBottleCapAlgo_CurlOptimizeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeForColorAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_DynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IBottleCapAlgo_EdgeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeForColorAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo1(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo1(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo1Help(This,pHelpStr) ) 

#define IBottleCapAlgo_GetCurrentLocationXY(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCurrentLocationXY(This,bstrLocationXY) ) 

#define IBottleCapAlgo_GetCenterLocation(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCenterLocation(This,bstrLocationXY) ) 

#define IBottleCapAlgo_SetCurrentTaskName2(This,bstrTaskName,bstrChName,bstrChNameWithoutCameraIndex)	\
    ( (This)->lpVtbl -> SetCurrentTaskName2(This,bstrTaskName,bstrChName,bstrChNameWithoutCameraIndex) ) 

#define IBottleCapAlgo_SectorRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> SectorRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDilationSize,pDetectArea) ) 

#define IBottleCapAlgo_SectorRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_SubDynThresholdAlgo(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> SubDynThresholdAlgo(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBottleCapAlgo_SubDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SubDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CurlNoCompoundColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pGrayValueMean) ) 

#define IBottleCapAlgo_CurlNoCompoundColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundColorAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CenterTopLocationAlgo(This,rgnId,pChannelSelect,pRowStart,pColStart,pRegWidth,pRegHeight,pThresholdValue)	\
    ( (This)->lpVtbl -> CenterTopLocationAlgo(This,rgnId,pChannelSelect,pRowStart,pColStart,pRegWidth,pRegHeight,pThresholdValue) ) 

#define IBottleCapAlgo_CenterTopLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterTopLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_BaseEdgeDetectAlgo(This,rgnId,pChannelSelect,pMoveSize,pSearchSize,pThreshold,pTransition,pPointSelection,pRemoveSize,pBaseEdgeArea)	\
    ( (This)->lpVtbl -> BaseEdgeDetectAlgo(This,rgnId,pChannelSelect,pMoveSize,pSearchSize,pThreshold,pTransition,pPointSelection,pRemoveSize,pBaseEdgeArea) ) 

#define IBottleCapAlgo_BaseEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BaseEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_SideEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> SideEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IBottleCapAlgo_SideEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_TopEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> TopEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IBottleCapAlgo_TopEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_UpCharLocationAlgo(This,rgnId,pMode,pRegionRow,pThreshold,pCharWidth,pCharHeight,pSlashWidth,pSlashHeight,pDistanceCharChar,pDistanceCharSlash,pMoveDistance,pCharArea)	\
    ( (This)->lpVtbl -> UpCharLocationAlgo(This,rgnId,pMode,pRegionRow,pThreshold,pCharWidth,pCharHeight,pSlashWidth,pSlashHeight,pDistanceCharChar,pDistanceCharSlash,pMoveDistance,pCharArea) ) 

#define IBottleCapAlgo_UpCharLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> UpCharLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_UpCharRegionLocationAlgo(This,rgnId,pSelectmodel,pRegionRow,pRegionHeight,pRegionWidth,pCharSize,pSlashSize)	\
    ( (This)->lpVtbl -> UpCharRegionLocationAlgo(This,rgnId,pSelectmodel,pRegionRow,pRegionHeight,pRegionWidth,pCharSize,pSlashSize) ) 

#define IBottleCapAlgo_UpCharRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> UpCharRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_DynThresholdAlgoNew(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoNew(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_DynThresholdAlgoNewHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoNewHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_LowCharLocationAlgo(This,rgnId,pMoveSize,pCharSize,pSlashSize,pThreshold,pMaxArea,pCharFlag)	\
    ( (This)->lpVtbl -> LowCharLocationAlgo(This,rgnId,pMoveSize,pCharSize,pSlashSize,pThreshold,pMaxArea,pCharFlag) ) 

#define IBottleCapAlgo_LowCharLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LowCharLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_LowCharRegionLocationAlgo(This,rgnId,pSelectmodel,pRegionRow,pRegionHeight,pRegionWidth,pCharSize)	\
    ( (This)->lpVtbl -> LowCharRegionLocationAlgo(This,rgnId,pSelectmodel,pRegionRow,pRegionHeight,pRegionWidth,pCharSize) ) 

#define IBottleCapAlgo_LowCharRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LowCharRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CurlNoCompoundAlgoNew(This,rgnId,pSeriousWhitePointDynThresh,pDetectArea)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoNew(This,rgnId,pSeriousWhitePointDynThresh,pDetectArea) ) 

#define IBottleCapAlgo_CurlNoCompoundAlgoNewHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoNewHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea) ) 

#define IBottleCapAlgo_LineDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectionAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_BubbleDetectProfileAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_BubbleDetectProfileAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_BubbleDetectProfileAlgoNew(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgoNew(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_BubbleDetectProfileAlgoNewHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgoNewHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_LineDetectionColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> LineDetectionColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea) ) 

#define IBottleCapAlgo_LineDetectionColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectionColorAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_BlackDetectExcludeEdgesAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pThresholdLow,pMinEdgeSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectExcludeEdgesAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pThresholdLow,pMinEdgeSize,pDetectArea) ) 

#define IBottleCapAlgo_BlackDetectExcludeEdgesAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectExcludeEdgesAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_SideCenterTopLocationAlgo(This,rgnId,pChannelSelect,pRowStart,pColStart,pRegWidth,pRegHeight,pThresholdValue,pAreaSize,pDownPose)	\
    ( (This)->lpVtbl -> PS_SideCenterTopLocationAlgo(This,rgnId,pChannelSelect,pRowStart,pColStart,pRegWidth,pRegHeight,pThresholdValue,pAreaSize,pDownPose) ) 

#define IBottleCapAlgo_PS_SideCenterTopLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_SideCenterTopLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_SideCenterBodyLocationAlgo(This,rgnId,pChannelSelect,pLocationSelect,pRowStart,pColStart,pRegWidth,pRegHeight,pThresholdValue)	\
    ( (This)->lpVtbl -> PS_SideCenterBodyLocationAlgo(This,rgnId,pChannelSelect,pLocationSelect,pRowStart,pColStart,pRegWidth,pRegHeight,pThresholdValue) ) 

#define IBottleCapAlgo_PS_SideCenterBodyLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_SideCenterBodyLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_BaseEdgeDetectAlgo(This,rgnId,pSelection,pChannelSelect,pMoveSize,pSearchSize,pHSearchSize,pThreshold,pTransition,pPointSelection,pRemoveSize,pBaseEdgeArea)	\
    ( (This)->lpVtbl -> PS_BaseEdgeDetectAlgo(This,rgnId,pSelection,pChannelSelect,pMoveSize,pSearchSize,pHSearchSize,pThreshold,pTransition,pPointSelection,pRemoveSize,pBaseEdgeArea) ) 

#define IBottleCapAlgo_PS_BaseEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_BaseEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_SideEdgeRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> PS_SideEdgeRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IBottleCapAlgo_PS_SideEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_SideEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_PatternRegionLocationAlgo(This,rgnId,pMode,pChannelSelect,pRegionSelect,pPoseAdjustRow,pPoseAdjustCol,pMoveSize,pRowBgn,pColBgn,pSmlCharHeight,pSmlCharWidth,pSmlCharSpace,pCharDistHeight,pCharDistWidth,pBigCharHeight,pBigCharWidth,pBigCharSpace,pCircleWidth,pCircleHeight)	\
    ( (This)->lpVtbl -> PS_PatternRegionLocationAlgo(This,rgnId,pMode,pChannelSelect,pRegionSelect,pPoseAdjustRow,pPoseAdjustCol,pMoveSize,pRowBgn,pColBgn,pSmlCharHeight,pSmlCharWidth,pSmlCharSpace,pCharDistHeight,pCharDistWidth,pBigCharHeight,pBigCharWidth,pBigCharSpace,pCircleWidth,pCircleHeight) ) 

#define IBottleCapAlgo_PS_PatternRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_PatternRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_PatternReferenceLocationAlgo(This,rgnId,pColorTransType,pChannelSelect,pSerchRowPos,pSerchWidth,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> PS_PatternReferenceLocationAlgo(This,rgnId,pColorTransType,pChannelSelect,pSerchRowPos,pSerchWidth,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBottleCapAlgo_PS_PatternReferenceLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_PatternReferenceLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_RemovePatternRegionLocationAlgo(This,rgnId,pColBgn,pColEnd,pRowBgn,pRowEnd,pScaleSizeH,pScaleSizeV)	\
    ( (This)->lpVtbl -> PS_RemovePatternRegionLocationAlgo(This,rgnId,pColBgn,pColEnd,pRowBgn,pRowEnd,pScaleSizeH,pScaleSizeV) ) 

#define IBottleCapAlgo_PS_RemovePatternRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_RemovePatternRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_TeethRegionDetectAlgo(This,rgnId,pChannelSelect,pRowBgn,pRgnHeight,pDarkThred,pTeethPosUp,pTeethPosDown,pLightThred,pLightAreaLimit,pDetectArea)	\
    ( (This)->lpVtbl -> PS_TeethRegionDetectAlgo(This,rgnId,pChannelSelect,pRowBgn,pRgnHeight,pDarkThred,pTeethPosUp,pTeethPosDown,pLightThred,pLightAreaLimit,pDetectArea) ) 

#define IBottleCapAlgo_PS_TeethRegionDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_TeethRegionDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_TeethOffsetDetectAlgo(This,rgnId,pChannelSelect,pTeethWidth,pOffset,pDetectArea)	\
    ( (This)->lpVtbl -> PS_TeethOffsetDetectAlgo(This,rgnId,pChannelSelect,pTeethWidth,pOffset,pDetectArea) ) 

#define IBottleCapAlgo_PS_TeethOffsetDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_TeethOffsetDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_DeformableModelDetectAlgo(This,rgnId,pChannelSelect,pMatchScore,pSmoothSize,pThreshold,pAreaSize,pDetectArea)	\
    ( (This)->lpVtbl -> PS_DeformableModelDetectAlgo(This,rgnId,pChannelSelect,pMatchScore,pSmoothSize,pThreshold,pAreaSize,pDetectArea) ) 

#define IBottleCapAlgo_PS_DeformableModelDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_DeformableModelDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_CurlNoCompoundForClolorNewAlgo(This,rgnId,pChannelSelect,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundForClolorNewAlgo(This,rgnId,pChannelSelect,pGrayValueMean) ) 

#define IBottleCapAlgo_CurlNoCompoundForClolorNewAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundForClolorNewAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_SideWallDynThresholdAlgoNew(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> SideWallDynThresholdAlgoNew(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_SideWallDynThresholdAlgoNewHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideWallDynThresholdAlgoNewHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_TeethRegionLocationAlgo(This,rgnId,pRegionPos,pUpSize,pDownSize,pScaleSize)	\
    ( (This)->lpVtbl -> PS_TeethRegionLocationAlgo(This,rgnId,pRegionPos,pUpSize,pDownSize,pScaleSize) ) 

#define IBottleCapAlgo_PS_TeethRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_TeethRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_SideWallDynThresholdAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> SideWallDynThresholdAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_SideWallDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideWallDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_TeethDisappearDetectAlgo(This,rgnId,pColorTransType,pChannelSelect,pAreaSize,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> PS_TeethDisappearDetectAlgo(This,rgnId,pColorTransType,pChannelSelect,pAreaSize,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBottleCapAlgo_PS_TeethDisappearDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_TeethDisappearDetectAlgoHelp(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_PatternRegionLocationAlgo1(This,rgnId,pSelect,pScaleSize)	\
    ( (This)->lpVtbl -> PS_PatternRegionLocationAlgo1(This,rgnId,pSelect,pScaleSize) ) 

#define IBottleCapAlgo_PS_PatternRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_PatternRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IBottleCapAlgo_PS_PatternDefectDetectAlgo(This,rgnId,pColorTransType,pChannelSelect,pWidthSelect,pWidthThred,pPatternRatio,pAreaThred,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> PS_PatternDefectDetectAlgo(This,rgnId,pColorTransType,pChannelSelect,pWidthSelect,pWidthThred,pPatternRatio,pAreaThred,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBottleCapAlgo_PS_PatternDefectDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PS_PatternDefectDetectAlgoHelp(This,pHelpStr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_GlueDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pGrayDiffValue,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_GlueDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_GlueDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_GlueDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_GlueInjectorIDDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackPointThresh,
    /* [in] */ VARIANT *pWhitePointThresh,
    /* [in] */ VARIANT *pMeanGrayDiffValue,
    /* [in] */ VARIANT *pDeviationGrayDiffValue,
    /* [in] */ VARIANT *pMaxID,
    /* [retval][out] */ VARIANT *pDetectID);


void __RPC_STUB IBottleCapAlgo_GlueInjectorIDDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_GlueInjectorIDDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_GlueInjectorIDDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_GlueInjectorIDDetectAlgo1_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRefreshIndex,
    /* [in] */ VARIANT *pStartIndex,
    /* [in] */ VARIANT *pTotalInjectorNum,
    /* [retval][out] */ VARIANT *pDetectID);


void __RPC_STUB IBottleCapAlgo_GlueInjectorIDDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_GlueInjectorIDDetectAlgo1Help_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_GlueInjectorIDDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_EccentricDetectionAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [retval][out] */ VARIANT *pDistance);


void __RPC_STUB IBottleCapAlgo_EccentricDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_EccentricDetectionAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_EccentricDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_InnerCircleLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLowThred,
    /* [in] */ VARIANT *pHighThred,
    /* [in] */ VARIANT *pLowLength,
    /* [in] */ VARIANT *pHighLength);


void __RPC_STUB IBottleCapAlgo_InnerCircleLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_InnerCircleLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ VARIANT *pHelpStr);


void __RPC_STUB IBottleCapAlgo_InnerCircleLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_ReprintDetectionAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [retval][out] */ VARIANT *pGrayDiff);


void __RPC_STUB IBottleCapAlgo_ReprintDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_ReprintDetectionAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_ReprintDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_WLineDetectionAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IBottleCapAlgo_WLineDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_WLineDetectionAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_WLineDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_WSpotDetectionAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSpotMaskSize,
    /* [in] */ VARIANT *pSpotThred,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IBottleCapAlgo_WSpotDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_WSpotDetectionAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_WSpotDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_WBlockDetectionAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlockThred,
    /* [in] */ VARIANT *pMinArea,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IBottleCapAlgo_WBlockDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_WBlockDetectionAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_WBlockDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_ArrowDistOffsetAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLowThred,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pBlackPointUpSize,
    /* [retval][out] */ VARIANT *pDistanceOffset);


void __RPC_STUB IBottleCapAlgo_ArrowDistOffsetAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_ArrowDistOffsetAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_ArrowDistOffsetAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_RingLocationForColorAlgoHS_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pOutlierRemoverSize,
    /* [in] */ VARIANT *pRegionWidth);


void __RPC_STUB IBottleCapAlgo_RingLocationForColorAlgoHS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_RingLocationForColorAlgoHSHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_RingLocationForColorAlgoHSHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CenterLocationForColorAlgoHS_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pOutlierRemoverSize);


void __RPC_STUB IBottleCapAlgo_CenterLocationForColorAlgoHS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CenterLocationForColorAlgoHSHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_CenterLocationForColorAlgoHSHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CurlOptimizeForColorAlgo_Proxy( 
    IBottleCapAlgo * This,
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


void __RPC_STUB IBottleCapAlgo_CurlOptimizeForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CurlOptimizeForColorAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_CurlOptimizeForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_DynThresholdForColorAlgo_Proxy( 
    IBottleCapAlgo * This,
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


void __RPC_STUB IBottleCapAlgo_DynThresholdForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_DynThresholdForColorAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_DynThresholdForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_EdgeForColorAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pEdgeThred,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_EdgeForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_EdgeForColorAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_EdgeForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo1_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo1Help_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_AnnularRotaDynThresholdForColorAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_GetCurrentLocationXY_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *bstrLocationXY);


void __RPC_STUB IBottleCapAlgo_GetCurrentLocationXY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_GetCenterLocation_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *bstrLocationXY);


void __RPC_STUB IBottleCapAlgo_GetCenterLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SetCurrentTaskName2_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ BSTR *bstrTaskName,
    /* [in] */ BSTR *bstrChName,
    /* [in] */ BSTR *bstrChNameWithoutCameraIndex);


void __RPC_STUB IBottleCapAlgo_SetCurrentTaskName2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SectorRotaDynThresholdAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [in] */ VARIANT *pDilationSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_SectorRotaDynThresholdAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SectorRotaDynThresholdAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_SectorRotaDynThresholdAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SubDynThresholdAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_SubDynThresholdAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SubDynThresholdAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_SubDynThresholdAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CurlNoCompoundColorAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [retval][out] */ VARIANT *pGrayValueMean);


void __RPC_STUB IBottleCapAlgo_CurlNoCompoundColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CurlNoCompoundColorAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_CurlNoCompoundColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CenterTopLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRowStart,
    /* [in] */ VARIANT *pColStart,
    /* [in] */ VARIANT *pRegWidth,
    /* [in] */ VARIANT *pRegHeight,
    /* [in] */ VARIANT *pThresholdValue);


void __RPC_STUB IBottleCapAlgo_CenterTopLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CenterTopLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_CenterTopLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_BaseEdgeDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pTransition,
    /* [in] */ VARIANT *pPointSelection,
    /* [in] */ VARIANT *pRemoveSize,
    /* [retval][out] */ VARIANT *pBaseEdgeArea);


void __RPC_STUB IBottleCapAlgo_BaseEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_BaseEdgeDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_BaseEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SideEdgeRegionLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IBottleCapAlgo_SideEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SideEdgeRegionLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_SideEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_TopEdgeRegionLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IBottleCapAlgo_TopEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_TopEdgeRegionLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_TopEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_UpCharLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMode,
    /* [in] */ VARIANT *pRegionRow,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pCharWidth,
    /* [in] */ VARIANT *pCharHeight,
    /* [in] */ VARIANT *pSlashWidth,
    /* [in] */ VARIANT *pSlashHeight,
    /* [in] */ VARIANT *pDistanceCharChar,
    /* [in] */ VARIANT *pDistanceCharSlash,
    /* [in] */ VARIANT *pMoveDistance,
    /* [retval][out] */ VARIANT *pCharArea);


void __RPC_STUB IBottleCapAlgo_UpCharLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_UpCharLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_UpCharLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_UpCharRegionLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelectmodel,
    /* [in] */ VARIANT *pRegionRow,
    /* [in] */ VARIANT *pRegionHeight,
    /* [in] */ VARIANT *pRegionWidth,
    /* [in] */ VARIANT *pCharSize,
    /* [in] */ VARIANT *pSlashSize);


void __RPC_STUB IBottleCapAlgo_UpCharRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_UpCharRegionLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_UpCharRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_DynThresholdAlgoNew_Proxy( 
    IBottleCapAlgo * This,
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


void __RPC_STUB IBottleCapAlgo_DynThresholdAlgoNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_DynThresholdAlgoNewHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_DynThresholdAlgoNewHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_LowCharLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pCharSize,
    /* [in] */ VARIANT *pSlashSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pMaxArea,
    /* [retval][out] */ VARIANT *pCharFlag);


void __RPC_STUB IBottleCapAlgo_LowCharLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_LowCharLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_LowCharLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_LowCharRegionLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelectmodel,
    /* [in] */ VARIANT *pRegionRow,
    /* [in] */ VARIANT *pRegionHeight,
    /* [in] */ VARIANT *pRegionWidth,
    /* [in] */ VARIANT *pCharSize);


void __RPC_STUB IBottleCapAlgo_LowCharRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_LowCharRegionLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_LowCharRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CurlNoCompoundAlgoNew_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_CurlNoCompoundAlgoNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CurlNoCompoundAlgoNewHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_CurlNoCompoundAlgoNewHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_LineDetectionAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IBottleCapAlgo_LineDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_LineDetectionAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_LineDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_BubbleDetectProfileAlgo_Proxy( 
    IBottleCapAlgo * This,
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


void __RPC_STUB IBottleCapAlgo_BubbleDetectProfileAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_BubbleDetectProfileAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_BubbleDetectProfileAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_BubbleDetectProfileAlgoNew_Proxy( 
    IBottleCapAlgo * This,
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


void __RPC_STUB IBottleCapAlgo_BubbleDetectProfileAlgoNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_BubbleDetectProfileAlgoNewHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_BubbleDetectProfileAlgoNewHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_LineDetectionColorAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IBottleCapAlgo_LineDetectionColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_LineDetectionColorAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_LineDetectionColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_BlackDetectExcludeEdgesAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pBlackThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pMinEdgeSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_BlackDetectExcludeEdgesAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_BlackDetectExcludeEdgesAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_BlackDetectExcludeEdgesAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_SideCenterTopLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRowStart,
    /* [in] */ VARIANT *pColStart,
    /* [in] */ VARIANT *pRegWidth,
    /* [in] */ VARIANT *pRegHeight,
    /* [in] */ VARIANT *pThresholdValue,
    /* [in] */ VARIANT *pAreaSize,
    /* [in] */ VARIANT *pDownPose);


void __RPC_STUB IBottleCapAlgo_PS_SideCenterTopLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_SideCenterTopLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_SideCenterTopLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_SideCenterBodyLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pLocationSelect,
    /* [in] */ VARIANT *pRowStart,
    /* [in] */ VARIANT *pColStart,
    /* [in] */ VARIANT *pRegWidth,
    /* [in] */ VARIANT *pRegHeight,
    /* [in] */ VARIANT *pThresholdValue);


void __RPC_STUB IBottleCapAlgo_PS_SideCenterBodyLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_SideCenterBodyLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_SideCenterBodyLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_BaseEdgeDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHSearchSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pTransition,
    /* [in] */ VARIANT *pPointSelection,
    /* [in] */ VARIANT *pRemoveSize,
    /* [retval][out] */ VARIANT *pBaseEdgeArea);


void __RPC_STUB IBottleCapAlgo_PS_BaseEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_BaseEdgeDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_BaseEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_SideEdgeRegionLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IBottleCapAlgo_PS_SideEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_SideEdgeRegionLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_SideEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_PatternRegionLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMode,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRegionSelect,
    /* [in] */ VARIANT *pPoseAdjustRow,
    /* [in] */ VARIANT *pPoseAdjustCol,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pRowBgn,
    /* [in] */ VARIANT *pColBgn,
    /* [in] */ VARIANT *pSmlCharHeight,
    /* [in] */ VARIANT *pSmlCharWidth,
    /* [in] */ VARIANT *pSmlCharSpace,
    /* [in] */ VARIANT *pCharDistHeight,
    /* [in] */ VARIANT *pCharDistWidth,
    /* [in] */ VARIANT *pBigCharHeight,
    /* [in] */ VARIANT *pBigCharWidth,
    /* [in] */ VARIANT *pBigCharSpace,
    /* [in] */ VARIANT *pCircleWidth,
    /* [in] */ VARIANT *pCircleHeight);


void __RPC_STUB IBottleCapAlgo_PS_PatternRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_PatternRegionLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_PatternRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_PatternReferenceLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pSerchRowPos,
    /* [in] */ VARIANT *pSerchWidth,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_PS_PatternReferenceLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_PatternReferenceLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_PatternReferenceLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_RemovePatternRegionLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColBgn,
    /* [in] */ VARIANT *pColEnd,
    /* [in] */ VARIANT *pRowBgn,
    /* [in] */ VARIANT *pRowEnd,
    /* [in] */ VARIANT *pScaleSizeH,
    /* [in] */ VARIANT *pScaleSizeV);


void __RPC_STUB IBottleCapAlgo_PS_RemovePatternRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_RemovePatternRegionLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_RemovePatternRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_TeethRegionDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRowBgn,
    /* [in] */ VARIANT *pRgnHeight,
    /* [in] */ VARIANT *pDarkThred,
    /* [in] */ VARIANT *pTeethPosUp,
    /* [in] */ VARIANT *pTeethPosDown,
    /* [in] */ VARIANT *pLightThred,
    /* [in] */ VARIANT *pLightAreaLimit,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_PS_TeethRegionDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_TeethRegionDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_TeethRegionDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_TeethOffsetDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pTeethWidth,
    /* [in] */ VARIANT *pOffset,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_PS_TeethOffsetDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_TeethOffsetDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_TeethOffsetDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_DeformableModelDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pMatchScore,
    /* [in] */ VARIANT *pSmoothSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pAreaSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_PS_DeformableModelDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_DeformableModelDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_DeformableModelDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CurlNoCompoundForClolorNewAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pChannelSelect,
    /* [retval][out] */ VARIANT *pGrayValueMean);


void __RPC_STUB IBottleCapAlgo_CurlNoCompoundForClolorNewAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_CurlNoCompoundForClolorNewAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_CurlNoCompoundForClolorNewAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SideWallDynThresholdAlgoNew_Proxy( 
    IBottleCapAlgo * This,
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


void __RPC_STUB IBottleCapAlgo_SideWallDynThresholdAlgoNew_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SideWallDynThresholdAlgoNewHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_SideWallDynThresholdAlgoNewHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_TeethRegionLocationAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRegionPos,
    /* [in] */ VARIANT *pUpSize,
    /* [in] */ VARIANT *pDownSize,
    /* [in] */ VARIANT *pScaleSize);


void __RPC_STUB IBottleCapAlgo_PS_TeethRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_TeethRegionLocationAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_TeethRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SideWallDynThresholdAlgo_Proxy( 
    IBottleCapAlgo * This,
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


void __RPC_STUB IBottleCapAlgo_SideWallDynThresholdAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_SideWallDynThresholdAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_SideWallDynThresholdAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_TeethDisappearDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pAreaSize,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_PS_TeethDisappearDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_TeethDisappearDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_TeethDisappearDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_PatternRegionLocationAlgo1_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelect,
    /* [in] */ VARIANT *pScaleSize);


void __RPC_STUB IBottleCapAlgo_PS_PatternRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_PatternRegionLocationAlgo1Help_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_PatternRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_PatternDefectDetectAlgo_Proxy( 
    IBottleCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pWidthSelect,
    /* [in] */ VARIANT *pWidthThred,
    /* [in] */ VARIANT *pPatternRatio,
    /* [in] */ VARIANT *pAreaThred,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBottleCapAlgo_PS_PatternDefectDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBottleCapAlgo_PS_PatternDefectDetectAlgoHelp_Proxy( 
    IBottleCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBottleCapAlgo_PS_PatternDefectDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBottleCapAlgo_INTERFACE_DEFINED__ */



#ifndef __BottleCapAlgoLib_LIBRARY_DEFINED__
#define __BottleCapAlgoLib_LIBRARY_DEFINED__

/* library BottleCapAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_BottleCapAlgoLib;

EXTERN_C const CLSID CLSID_BottleCapAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("2B5AEFC9-EA40-4F76-8064-E72C3425C4D9")
BottleCapAlgo;
#endif
#endif /* __BottleCapAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


