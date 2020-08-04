

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __AlPlasticCapAlgo_i_h__
#define __AlPlasticCapAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAlPlasticCapAlgo_FWD_DEFINED__
#define __IAlPlasticCapAlgo_FWD_DEFINED__
typedef interface IAlPlasticCapAlgo IAlPlasticCapAlgo;
#endif 	/* __IAlPlasticCapAlgo_FWD_DEFINED__ */


#ifndef __AlPlasticCapAlgo_FWD_DEFINED__
#define __AlPlasticCapAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class AlPlasticCapAlgo AlPlasticCapAlgo;
#else
typedef struct AlPlasticCapAlgo AlPlasticCapAlgo;
#endif /* __cplusplus */

#endif 	/* __AlPlasticCapAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IAlPlasticCapAlgo_INTERFACE_DEFINED__
#define __IAlPlasticCapAlgo_INTERFACE_DEFINED__

/* interface IAlPlasticCapAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IAlPlasticCapAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("88A26EAF-5D85-493E-BFB2-E2BF8AEAEE21")
    IAlPlasticCapAlgo : public IDispatch
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectOcrDistOffsetAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistanceOffset) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectOcrDistOffsetAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetCurrentLocationXY( 
            /* [retval][out] */ BSTR *bstrLocationXY) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetCenterLocation( 
            /* [retval][out] */ BSTR *bstrLocationXY) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PrintErrorDetectOcrAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pHeightContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PrintErrorDetectOcrAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgoForColor( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pDividingLine,
            /* [in] */ VARIANT *BackgroundGray,
            /* [in] */ VARIANT *AlThreshold,
            /* [in] */ VARIANT *PlasticThreshold,
            /* [in] */ VARIANT *pSmoothSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgoForColorHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PlasticRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRowMove,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pWidthShrink) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PlasticRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ExpansionContourGenerationRegionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pType,
            /* [in] */ VARIANT *pRowMove,
            /* [in] */ VARIANT *pDilationHeight,
            /* [in] */ VARIANT *pWidthZoom) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ExpansionContourGenerationRegionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE StepEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pSmoothSize,
            /* [retval][out] */ VARIANT *pEdgeHeight) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE StepEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AlRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpRowMove,
            /* [in] */ VARIANT *pDownRowMove,
            /* [in] */ VARIANT *pWidthShrink) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AlRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAlPlasticCapAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAlPlasticCapAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAlPlasticCapAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAlPlasticCapAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhiteErrorRegion )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pWhiteErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInterImgs )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            IAlPlasticCapAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            IAlPlasticCapAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            IAlPlasticCapAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegionEx )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegionExTest )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1 )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1Help )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHS )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHSHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoHS )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRectangleLocationAlgoHSHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFitting )( 
            IAlPlasticCapAlgo * This,
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
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pNumLevels,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RotatingAngleDetectAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pEdgeType,
            /* [in] */ VARIANT *pEdgeDist,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RotatingAngleDetectAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoOCR )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoOCRHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationExtractOCRAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrOcrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationExtractOCRAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1 )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1Help )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo1 )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo1Help )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pWinType,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo2 )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo2Help )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineNewAlgo )( 
            IAlPlasticCapAlgo * This,
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
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GContoursinkAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GContoursinkAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlack )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackNumber )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackNumberHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhite )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteNumber )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteNumberHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackWhiteNumber )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackWhiteNumberHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoOCR )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoOCRHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo )( 
            IAlPlasticCapAlgo * This,
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
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MinMaxGrayDetectAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MinMaxGrayDetectAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBW )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBWHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo1 )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo1Help )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectOcrDistOffsetAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistanceOffset);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectOcrDistOffsetAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RingLocationForColorAlgoHS )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pRegionWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RingLocationForColorAlgoHSHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationForColorAlgoHS )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationForColorAlgoHSHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeForColorAlgo )( 
            IAlPlasticCapAlgo * This,
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
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorAlgo )( 
            IAlPlasticCapAlgo * This,
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
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentLocationXY )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCenterLocation )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PrintErrorDetectOcrAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowContrast,
            /* [in] */ VARIANT *pHeightContrast,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PrintErrorDetectOcrAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgoForColor )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pDividingLine,
            /* [in] */ VARIANT *BackgroundGray,
            /* [in] */ VARIANT *AlThreshold,
            /* [in] */ VARIANT *PlasticThreshold,
            /* [in] */ VARIANT *pSmoothSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgoForColorHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PlasticRegionLocationAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRowMove,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pWidthShrink);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PlasticRegionLocationAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ExpansionContourGenerationRegionAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pType,
            /* [in] */ VARIANT *pRowMove,
            /* [in] */ VARIANT *pDilationHeight,
            /* [in] */ VARIANT *pWidthZoom);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ExpansionContourGenerationRegionAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *StepEdgeDetectAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pSmoothSize,
            /* [retval][out] */ VARIANT *pEdgeHeight);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *StepEdgeDetectAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AlRegionLocationAlgo )( 
            IAlPlasticCapAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpRowMove,
            /* [in] */ VARIANT *pDownRowMove,
            /* [in] */ VARIANT *pWidthShrink);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AlRegionLocationAlgoHelp )( 
            IAlPlasticCapAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        END_INTERFACE
    } IAlPlasticCapAlgoVtbl;

    interface IAlPlasticCapAlgo
    {
        CONST_VTBL struct IAlPlasticCapAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAlPlasticCapAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAlPlasticCapAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAlPlasticCapAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAlPlasticCapAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAlPlasticCapAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAlPlasticCapAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAlPlasticCapAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAlPlasticCapAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define IAlPlasticCapAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define IAlPlasticCapAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define IAlPlasticCapAlgo_GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn)	\
    ( (This)->lpVtbl -> GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn) ) 

#define IAlPlasticCapAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define IAlPlasticCapAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define IAlPlasticCapAlgo_SetCurrentTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName) ) 

#define IAlPlasticCapAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#define IAlPlasticCapAlgo_GetInterImgs(This,rgnID,pInterImgs)	\
    ( (This)->lpVtbl -> GetInterImgs(This,rgnID,pInterImgs) ) 

#define IAlPlasticCapAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define IAlPlasticCapAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define IAlPlasticCapAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define IAlPlasticCapAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define IAlPlasticCapAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define IAlPlasticCapAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define IAlPlasticCapAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define IAlPlasticCapAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define IAlPlasticCapAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define IAlPlasticCapAlgo_GetRegionEx(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegionEx(This,rgnID,pRgn) ) 

#define IAlPlasticCapAlgo_GetRegionExTest(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegionExTest(This,rgnID,pRgn) ) 

#define IAlPlasticCapAlgo_CenterLocationAlgo(This,rgnId,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnId,pRadius,pMinScore) ) 

#define IAlPlasticCapAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CenterLocationAlgo1(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1(This,rgnID,pRadius,pMinScore) ) 

#define IAlPlasticCapAlgo_CenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1Help(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define IAlPlasticCapAlgo_CenterLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHSHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CenterRectangleLocationAlgoHS(This,rgnId,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoHS(This,rgnId,pThresholdLow,pOutlierRemoverSize) ) 

#define IAlPlasticCapAlgo_CenterRectangleLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRectangleLocationAlgoHSHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold) ) 

#define IAlPlasticCapAlgo_CenterLocationAlgoUsePointFittingHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFittingHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IAlPlasticCapAlgo_MatchingParaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IAlPlasticCapAlgo_MatchingParaMinScoreAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_RotatingAngleDetectAlgo(This,rgnId,pMaskSize,pBlackThredLow,pEdgeType,pEdgeDist,pDefectSize,pDetectAngle)	\
    ( (This)->lpVtbl -> RotatingAngleDetectAlgo(This,rgnId,pMaskSize,pBlackThredLow,pEdgeType,pEdgeDist,pDefectSize,pDetectAngle) ) 

#define IAlPlasticCapAlgo_RotatingAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RotatingAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IAlPlasticCapAlgo_CircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IAlPlasticCapAlgo_CircleRegionAngleAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IAlPlasticCapAlgo_DetectRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DetectRegionLocationAlgoOCR(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoOCR(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IAlPlasticCapAlgo_DetectRegionLocationAlgoOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoOCRHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DetectRegionLocationExtractOCRAlgo(This,rgnId,pRadius,pErosionSize,bstrOcrRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationExtractOCRAlgo(This,rgnId,pRadius,pErosionSize,bstrOcrRgnFile) ) 

#define IAlPlasticCapAlgo_DetectRegionLocationExtractOCRAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationExtractOCRAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth) ) 

#define IAlPlasticCapAlgo_CircleRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CircleRegionAngleAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IAlPlasticCapAlgo_CircleRegionAngleAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo1Help(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IAlPlasticCapAlgo_BoundaryRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IAlPlasticCapAlgo_CurlOptimizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CurlDetectAlgo2(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo2(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define IAlPlasticCapAlgo_CurlDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgo2Help(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea) ) 

#define IAlPlasticCapAlgo_UVCurlAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> UVCurlAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_AnnularRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CurlNoCompoundAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgo(This,rgnId,pGrayValueMean) ) 

#define IAlPlasticCapAlgo_CurlNoCompoundAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IAlPlasticCapAlgo_GSurfaceAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GSurfaceAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_GLineAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_GLineNewAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteThredHigh,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineNewAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteThredHigh,pWhiteDefectSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_GLineNewAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineNewAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_EdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_GContoursinkAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GContoursinkAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IAlPlasticCapAlgo_GContoursinkAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GContoursinkAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoBlackHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoBlackNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectNumber) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoBlackNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackNumberHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoWhiteHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoWhiteNumber(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteNumber(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoWhiteNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteNumberHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoBlackWhiteNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackWhiteNumber(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectNumber) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoBlackWhiteNumberHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackWhiteNumberHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_DynThresholdAlgoOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoOCRHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_BlackDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_MinMaxGrayDetectAlgo(This,rgnId,pBlackThresh,pSeriousBlackPointSize,pWhiteThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> MinMaxGrayDetectAlgo(This,rgnId,pBlackThresh,pSeriousBlackPointSize,pWhiteThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_MinMaxGrayDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MinMaxGrayDetectAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IAlPlasticCapAlgo_SectorRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_SectorRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IAlPlasticCapAlgo_SectorRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DetectOcrDistOffsetAlgo(This,rgnId,pDistanceOffset)	\
    ( (This)->lpVtbl -> DetectOcrDistOffsetAlgo(This,rgnId,pDistanceOffset) ) 

#define IAlPlasticCapAlgo_DetectOcrDistOffsetAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectOcrDistOffsetAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_RingLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize,pRegionWidth)	\
    ( (This)->lpVtbl -> RingLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize,pRegionWidth) ) 

#define IAlPlasticCapAlgo_RingLocationForColorAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RingLocationForColorAlgoHSHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CenterLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationForColorAlgoHS(This,rgnId,pColorTransType,pChannelSelect,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define IAlPlasticCapAlgo_CenterLocationForColorAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationForColorAlgoHSHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_CurlOptimizeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pFreq_start,pFreqCutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pFreq_start,pFreqCutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IAlPlasticCapAlgo_CurlOptimizeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeForColorAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_DynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_EdgeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeForColorAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_GetCurrentLocationXY(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCurrentLocationXY(This,bstrLocationXY) ) 

#define IAlPlasticCapAlgo_GetCenterLocation(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCenterLocation(This,bstrLocationXY) ) 

#define IAlPlasticCapAlgo_PrintErrorDetectOcrAlgo(This,rgnId,pLowContrast,pHeightContrast,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> PrintErrorDetectOcrAlgo(This,rgnId,pLowContrast,pHeightContrast,pDilationSize,pDetectArea) ) 

#define IAlPlasticCapAlgo_PrintErrorDetectOcrAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PrintErrorDetectOcrAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_SideCenterLocationAlgoForColor(This,rgnId,pColorTransType,pChannelSelect,pDividingLine,BackgroundGray,AlThreshold,PlasticThreshold,pSmoothSize)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgoForColor(This,rgnId,pColorTransType,pChannelSelect,pDividingLine,BackgroundGray,AlThreshold,PlasticThreshold,pSmoothSize) ) 

#define IAlPlasticCapAlgo_SideCenterLocationAlgoForColorHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgoForColorHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_PlasticRegionLocationAlgo(This,rgnId,pRowMove,pRegionHeight,pWidthShrink)	\
    ( (This)->lpVtbl -> PlasticRegionLocationAlgo(This,rgnId,pRowMove,pRegionHeight,pWidthShrink) ) 

#define IAlPlasticCapAlgo_PlasticRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PlasticRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_ExpansionContourGenerationRegionAlgo(This,rgnId,pType,pRowMove,pDilationHeight,pWidthZoom)	\
    ( (This)->lpVtbl -> ExpansionContourGenerationRegionAlgo(This,rgnId,pType,pRowMove,pDilationHeight,pWidthZoom) ) 

#define IAlPlasticCapAlgo_ExpansionContourGenerationRegionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ExpansionContourGenerationRegionAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_StepEdgeDetectAlgo(This,rgnId,pSearchSize,pChannelSelect,pSmoothSize,pEdgeHeight)	\
    ( (This)->lpVtbl -> StepEdgeDetectAlgo(This,rgnId,pSearchSize,pChannelSelect,pSmoothSize,pEdgeHeight) ) 

#define IAlPlasticCapAlgo_StepEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> StepEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IAlPlasticCapAlgo_AlRegionLocationAlgo(This,rgnId,pUpRowMove,pDownRowMove,pWidthShrink)	\
    ( (This)->lpVtbl -> AlRegionLocationAlgo(This,rgnId,pUpRowMove,pDownRowMove,pWidthShrink) ) 

#define IAlPlasticCapAlgo_AlRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AlRegionLocationAlgoHelp(This,pHelpStr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local][id] */ HRESULT STDMETHODCALLTYPE IAlPlasticCapAlgo_ExpansionContourGenerationRegionAlgoHelp_Proxy( 
    IAlPlasticCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IAlPlasticCapAlgo_ExpansionContourGenerationRegionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IAlPlasticCapAlgo_StepEdgeDetectAlgo_Proxy( 
    IAlPlasticCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pSmoothSize,
    /* [retval][out] */ VARIANT *pEdgeHeight);


void __RPC_STUB IAlPlasticCapAlgo_StepEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IAlPlasticCapAlgo_StepEdgeDetectAlgoHelp_Proxy( 
    IAlPlasticCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IAlPlasticCapAlgo_StepEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IAlPlasticCapAlgo_AlRegionLocationAlgo_Proxy( 
    IAlPlasticCapAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pUpRowMove,
    /* [in] */ VARIANT *pDownRowMove,
    /* [in] */ VARIANT *pWidthShrink);


void __RPC_STUB IAlPlasticCapAlgo_AlRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IAlPlasticCapAlgo_AlRegionLocationAlgoHelp_Proxy( 
    IAlPlasticCapAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IAlPlasticCapAlgo_AlRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAlPlasticCapAlgo_INTERFACE_DEFINED__ */



#ifndef __AlPlasticCapAlgoLib_LIBRARY_DEFINED__
#define __AlPlasticCapAlgoLib_LIBRARY_DEFINED__

/* library AlPlasticCapAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_AlPlasticCapAlgoLib;

EXTERN_C const CLSID CLSID_AlPlasticCapAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("2B5AEFC9-EA40-4F76-8064-E72C3425C4D9")
AlPlasticCapAlgo;
#endif
#endif /* __AlPlasticCapAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


