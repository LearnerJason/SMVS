

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __HdpeAlgo_i_h__
#define __HdpeAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IHdpeAlgo_FWD_DEFINED__
#define __IHdpeAlgo_FWD_DEFINED__
typedef interface IHdpeAlgo IHdpeAlgo;
#endif 	/* __IHdpeAlgo_FWD_DEFINED__ */


#ifndef __ISelfLearning_FWD_DEFINED__
#define __ISelfLearning_FWD_DEFINED__
typedef interface ISelfLearning ISelfLearning;
#endif 	/* __ISelfLearning_FWD_DEFINED__ */


#ifndef __HdpeAlgo_FWD_DEFINED__
#define __HdpeAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class HdpeAlgo HdpeAlgo;
#else
typedef struct HdpeAlgo HdpeAlgo;
#endif /* __cplusplus */

#endif 	/* __HdpeAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IHdpeAlgo_INTERFACE_DEFINED__
#define __IHdpeAlgo_INTERFACE_DEFINED__

/* interface IHdpeAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IHdpeAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D823E8B8-60C3-4011-BE03-E91A09E32F20")
    IHdpeAlgo : public IDispatch
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetSelfLearningTargetTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentTaskName( 
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetCurrentLocationXY( 
            /* [retval][out] */ BSTR *bstrLocationXY) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationUseEdgesAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pThresholdHigh,
            /* [in] */ VARIANT *pMinCircularity) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationUseEdgesAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoHSHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectLocationAlgoHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectLocationAlgoHSHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationByMatchAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationByMatchAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE OcrAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE OcrAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE OcrAngleDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaRatio,
            /* [retval][out] */ VARIANT *pDetectAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE OcrAngleDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CreateOcrModelAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaxNumber,
            /* [in] */ VARIANT *pContrastMin,
            /* [in] */ VARIANT *pContrast,
            /* [in] */ VARIANT *pEdgeSize,
            /* [in] */ VARIANT *pDilationRow,
            /* [in] */ VARIANT *pDilationCol,
            /* [in] */ VARIANT *pOcrHeight,
            /* [in] */ VARIANT *pOcrWidth,
            /* [retval][out] */ VARIANT *pSampleNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CreateOcrModelAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FindOcrNumberAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaxNumber,
            /* [in] */ VARIANT *pContrastMin,
            /* [in] */ VARIANT *pContrast,
            /* [in] */ VARIANT *pEdgeSize,
            /* [in] */ VARIANT *pDilationRow,
            /* [in] */ VARIANT *pDilationCol,
            /* [in] */ VARIANT *pOcrHeight,
            /* [in] */ VARIANT *pOcrWidth,
            /* [retval][out] */ VARIANT *pOcrNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FindOcrNumberAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRegionLocationAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRgnWidth,
            /* [in] */ VARIANT *pRgnHeight,
            /* [in] */ VARIANT *pCenterMoveR,
            /* [in] */ VARIANT *pCenterMoveC) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectExcludeEdgesAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pMinEdgeSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectExcludeEdgesAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionExclude90Algo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionExclude90AlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectUseUpSizeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pThredForeground,
            /* [in] */ VARIANT *pWhiteSize,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectUseUpSizeAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GapAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pBinThred,
            /* [in] */ VARIANT *pEdgeDist,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GapAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionEdgesAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionEdgesAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionRegularAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRegMinWidth,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionRegularAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionIrregularAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionIrregularAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoGapHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoGapHSHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHdpeAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHdpeAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHdpeAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHdpeAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHdpeAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHdpeAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHdpeAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHdpeAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            IHdpeAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            IHdpeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            IHdpeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            IHdpeAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            IHdpeAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            IHdpeAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            IHdpeAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            IHdpeAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            IHdpeAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            IHdpeAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            IHdpeAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            IHdpeAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            IHdpeAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            IHdpeAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            IHdpeAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            IHdpeAlgo * This,
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentLocationXY )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1 )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1Help )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationUseEdgesAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pThresholdHigh,
            /* [in] */ VARIANT *pMinCircularity);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationUseEdgesAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHS )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHSHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectLocationAlgoHS )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectLocationAlgoHSHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationByMatchAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationByMatchAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pNumLevels,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *OcrAngleDetectAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *OcrAngleDetectAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *OcrAngleDetectAlgo1 )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaRatio,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *OcrAngleDetectAlgo1Help )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CreateOcrModelAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaxNumber,
            /* [in] */ VARIANT *pContrastMin,
            /* [in] */ VARIANT *pContrast,
            /* [in] */ VARIANT *pEdgeSize,
            /* [in] */ VARIANT *pDilationRow,
            /* [in] */ VARIANT *pDilationCol,
            /* [in] */ VARIANT *pOcrHeight,
            /* [in] */ VARIANT *pOcrWidth,
            /* [retval][out] */ VARIANT *pSampleNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CreateOcrModelAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FindOcrNumberAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaxNumber,
            /* [in] */ VARIANT *pContrastMin,
            /* [in] */ VARIANT *pContrast,
            /* [in] */ VARIANT *pEdgeSize,
            /* [in] */ VARIANT *pDilationRow,
            /* [in] */ VARIANT *pDilationCol,
            /* [in] */ VARIANT *pOcrHeight,
            /* [in] */ VARIANT *pOcrWidth,
            /* [retval][out] */ VARIANT *pOcrNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FindOcrNumberAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoOCR )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoOCRHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectRegionLocationAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRgnWidth,
            /* [in] */ VARIANT *pRgnHeight,
            /* [in] */ VARIANT *pCenterMoveR,
            /* [in] */ VARIANT *pCenterMoveC);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectRegionLocationAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoOCR )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoOCRHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectExcludeEdgesAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pMinEdgeSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectExcludeEdgesAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo )( 
            IHdpeAlgo * This,
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
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionExclude90Algo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionExclude90AlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectUseUpSizeAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pThredForeground,
            /* [in] */ VARIANT *pWhiteSize,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectUseUpSizeAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDetectProfileAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredBackground,
            /* [in] */ VARIANT *pThredForeground,
            /* [in] */ VARIANT *pAnglePointSize,
            /* [in] */ VARIANT *pMinAngle,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TabDetectProfileAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GapAngleDetectAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pBinThred,
            /* [in] */ VARIANT *pEdgeDist,
            /* [in] */ VARIANT *pErosionSize,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GapAngleDetectAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionEdgesAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionEdgesAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionRegularAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRegMinWidth,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionRegularAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionIrregularAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionIrregularAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoGapHS )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoGapHSHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorAlgo )( 
            IHdpeAlgo * This,
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
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundForColorAlgo )( 
            IHdpeAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundForColorAlgoHelp )( 
            IHdpeAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        END_INTERFACE
    } IHdpeAlgoVtbl;

    interface IHdpeAlgo
    {
        CONST_VTBL struct IHdpeAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHdpeAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHdpeAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHdpeAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHdpeAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHdpeAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHdpeAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHdpeAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IHdpeAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define IHdpeAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define IHdpeAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define IHdpeAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define IHdpeAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define IHdpeAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define IHdpeAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define IHdpeAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define IHdpeAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define IHdpeAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define IHdpeAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define IHdpeAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define IHdpeAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define IHdpeAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define IHdpeAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#define IHdpeAlgo_SetCurrentTaskName(This,bstrTaskName,bstrChName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName,bstrChName) ) 

#define IHdpeAlgo_GetCurrentLocationXY(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCurrentLocationXY(This,bstrLocationXY) ) 

#define IHdpeAlgo_CenterLocationAlgo(This,rgnId,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnId,pRadius,pMinScore) ) 

#define IHdpeAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_CenterLocationAlgo1(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1(This,rgnID,pRadius,pMinScore) ) 

#define IHdpeAlgo_CenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1Help(This,pHelpStr) ) 

#define IHdpeAlgo_CenterLocationUseEdgesAlgo(This,rgnID,pRadius,pThresholdLow,pThresholdHigh,pMinCircularity)	\
    ( (This)->lpVtbl -> CenterLocationUseEdgesAlgo(This,rgnID,pRadius,pThresholdLow,pThresholdHigh,pMinCircularity) ) 

#define IHdpeAlgo_CenterLocationUseEdgesAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationUseEdgesAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_CenterLocationAlgoHS(This,rgnId,pDetectType,pRadius,pThresholdLow,pSeriousBlackPointSize,pBlackPointUpSize,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHS(This,rgnId,pDetectType,pRadius,pThresholdLow,pSeriousBlackPointSize,pBlackPointUpSize,pOutlierRemoverSize) ) 

#define IHdpeAlgo_CenterLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHSHelp(This,pHelpStr) ) 

#define IHdpeAlgo_RectLocationAlgoHS(This,rgnId,pDetectType,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> RectLocationAlgoHS(This,rgnId,pDetectType,pThresholdLow,pOutlierRemoverSize) ) 

#define IHdpeAlgo_RectLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectLocationAlgoHSHelp(This,pHelpStr) ) 

#define IHdpeAlgo_CenterLocationByMatchAlgo(This,rgnID,pDetectType,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationByMatchAlgo(This,rgnID,pDetectType,pRadius,pMinScore) ) 

#define IHdpeAlgo_CenterLocationByMatchAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationByMatchAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IHdpeAlgo_MatchingParaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IHdpeAlgo_MatchingParaMinScoreAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_OcrAngleDetectAlgo(This,rgnId,pDetectAngle)	\
    ( (This)->lpVtbl -> OcrAngleDetectAlgo(This,rgnId,pDetectAngle) ) 

#define IHdpeAlgo_OcrAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> OcrAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_OcrAngleDetectAlgo1(This,rgnId,pAreaRatio,pDetectAngle)	\
    ( (This)->lpVtbl -> OcrAngleDetectAlgo1(This,rgnId,pAreaRatio,pDetectAngle) ) 

#define IHdpeAlgo_OcrAngleDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> OcrAngleDetectAlgo1Help(This,pHelpStr) ) 

#define IHdpeAlgo_CreateOcrModelAlgo(This,rgnId,pMaxNumber,pContrastMin,pContrast,pEdgeSize,pDilationRow,pDilationCol,pOcrHeight,pOcrWidth,pSampleNumber)	\
    ( (This)->lpVtbl -> CreateOcrModelAlgo(This,rgnId,pMaxNumber,pContrastMin,pContrast,pEdgeSize,pDilationRow,pDilationCol,pOcrHeight,pOcrWidth,pSampleNumber) ) 

#define IHdpeAlgo_CreateOcrModelAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CreateOcrModelAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_FindOcrNumberAlgo(This,rgnId,pMaxNumber,pContrastMin,pContrast,pEdgeSize,pDilationRow,pDilationCol,pOcrHeight,pOcrWidth,pOcrNumber)	\
    ( (This)->lpVtbl -> FindOcrNumberAlgo(This,rgnId,pMaxNumber,pContrastMin,pContrast,pEdgeSize,pDilationRow,pDilationCol,pOcrHeight,pOcrWidth,pOcrNumber) ) 

#define IHdpeAlgo_FindOcrNumberAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> FindOcrNumberAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IHdpeAlgo_CircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IHdpeAlgo_SectorRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IHdpeAlgo_DetectRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DetectRegionLocationAlgoOCR(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoOCR(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IHdpeAlgo_DetectRegionLocationAlgoOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoOCRHelp(This,pHelpStr) ) 

#define IHdpeAlgo_RectRegionLocationAlgo(This,rgnId,pRgnWidth,pRgnHeight,pCenterMoveR,pCenterMoveC)	\
    ( (This)->lpVtbl -> RectRegionLocationAlgo(This,rgnId,pRgnWidth,pRgnHeight,pCenterMoveR,pCenterMoveC) ) 

#define IHdpeAlgo_RectRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_CurlNoCompoundAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgo(This,rgnId,pGrayValueMean) ) 

#define IHdpeAlgo_CurlNoCompoundAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IHdpeAlgo_CurlOptimizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea) ) 

#define IHdpeAlgo_UVCurlAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> UVCurlAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IHdpeAlgo_AnnularRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IHdpeAlgo_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DynThresholdAlgoOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoOCR(This,rgnId,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pErosionSize,pDetectArea) ) 

#define IHdpeAlgo_DynThresholdAlgoOCRHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoOCRHelp(This,pHelpStr) ) 

#define IHdpeAlgo_BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IHdpeAlgo_BlackDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_BlackDetectExcludeEdgesAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pThresholdLow,pMinEdgeSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectExcludeEdgesAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pThresholdLow,pMinEdgeSize,pDetectArea) ) 

#define IHdpeAlgo_BlackDetectExcludeEdgesAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectExcludeEdgesAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_BubbleDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IHdpeAlgo_BubbleDetectProfileAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IHdpeAlgo_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IHdpeAlgo_EdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea) ) 

#define IHdpeAlgo_LineDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectionAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_LineDetectionExclude90Algo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> LineDetectionExclude90Algo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea) ) 

#define IHdpeAlgo_LineDetectionExclude90AlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectionExclude90AlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_BlackDetectUseUpSizeAlgo(This,rgnId,pBlackThresh,pThredForeground,pWhiteSize,pSeriousBlackPointSize,pBlackPointUpSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectUseUpSizeAlgo(This,rgnId,pBlackThresh,pThredForeground,pWhiteSize,pSeriousBlackPointSize,pBlackPointUpSize,pDetectArea) ) 

#define IHdpeAlgo_BlackDetectUseUpSizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectUseUpSizeAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_TabDetectProfileAlgo(This,rgnId,pThredBackground,pThredForeground,pAnglePointSize,pMinAngle,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> TabDetectProfileAlgo(This,rgnId,pThredBackground,pThredForeground,pAnglePointSize,pMinAngle,pSeriousBlackPointSize,pDetectArea) ) 

#define IHdpeAlgo_TabDetectProfileAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TabDetectProfileAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_GapAngleDetectAlgo(This,rgnId,pDetectType,pBinThred,pEdgeDist,pErosionSize,pDetectAngle)	\
    ( (This)->lpVtbl -> GapAngleDetectAlgo(This,rgnId,pDetectType,pBinThred,pEdgeDist,pErosionSize,pDetectAngle) ) 

#define IHdpeAlgo_GapAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GapAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DetectRegionEdgesAlgo(This,rgnId,pSeriousBlackPointDynThresh,pErosionSize,bstrRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionEdgesAlgo(This,rgnId,pSeriousBlackPointDynThresh,pErosionSize,bstrRgnFile) ) 

#define IHdpeAlgo_DetectRegionEdgesAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionEdgesAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DetectRegionRegularAlgo(This,rgnId,pRegMinWidth,pErosionSize)	\
    ( (This)->lpVtbl -> DetectRegionRegularAlgo(This,rgnId,pRegMinWidth,pErosionSize) ) 

#define IHdpeAlgo_DetectRegionRegularAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionRegularAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DetectRegionIrregularAlgo(This,rgnId,pErosionSize)	\
    ( (This)->lpVtbl -> DetectRegionIrregularAlgo(This,rgnId,pErosionSize) ) 

#define IHdpeAlgo_DetectRegionIrregularAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionIrregularAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_CenterLocationAlgoGapHS(This,rgnId,pRadius,pThresholdLow,pSeriousBlackPointSize,pBlackPointUpSize,pClosingSize,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationAlgoGapHS(This,rgnId,pRadius,pThresholdLow,pSeriousBlackPointSize,pBlackPointUpSize,pClosingSize,pOutlierRemoverSize) ) 

#define IHdpeAlgo_CenterLocationAlgoGapHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoGapHSHelp(This,pHelpStr) ) 

#define IHdpeAlgo_DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IHdpeAlgo_DynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IHdpeAlgo_EdgeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeForColorAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IHdpeAlgo_AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IHdpeAlgo_CurlNoCompoundForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pGrayValueMean) ) 

#define IHdpeAlgo_CurlNoCompoundForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundForColorAlgoHelp(This,pHelpStr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IHdpeAlgo_INTERFACE_DEFINED__ */


#ifndef __ISelfLearning_INTERFACE_DEFINED__
#define __ISelfLearning_INTERFACE_DEFINED__

/* interface ISelfLearning */
/* [unique][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_ISelfLearning;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03FE5233-3BF3-4358-AA95-61E0D4AD4124")
    ISelfLearning : public IUnknown
    {
    public:
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

    typedef struct ISelfLearningVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISelfLearning * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISelfLearning * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISelfLearning * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            ISelfLearning * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            ISelfLearning * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            ISelfLearning * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            ISelfLearning * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            ISelfLearning * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            ISelfLearning * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            ISelfLearning * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            ISelfLearning * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            ISelfLearning * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            ISelfLearning * This,
            /* [in] */ BSTR *bstrTaskName);
        
        END_INTERFACE
    } ISelfLearningVtbl;

    interface ISelfLearning
    {
        CONST_VTBL struct ISelfLearningVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISelfLearning_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISelfLearning_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISelfLearning_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISelfLearning_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define ISelfLearning_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define ISelfLearning_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define ISelfLearning_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define ISelfLearning_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define ISelfLearning_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define ISelfLearning_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define ISelfLearning_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define ISelfLearning_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define ISelfLearning_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISelfLearning_INTERFACE_DEFINED__ */



#ifndef __HdpeAlgoLib_LIBRARY_DEFINED__
#define __HdpeAlgoLib_LIBRARY_DEFINED__

/* library HdpeAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_HdpeAlgoLib;

EXTERN_C const CLSID CLSID_HdpeAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("742148EA-EFB5-4125-92D6-F099E736687E")
HdpeAlgo;
#endif
#endif /* __HdpeAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


