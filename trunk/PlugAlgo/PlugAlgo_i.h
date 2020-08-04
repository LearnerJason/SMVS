

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 10:05:45 2019
 */
/* Compiler settings for PlugAlgo.idl:
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

#ifndef __PlugAlgo_i_h__
#define __PlugAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPlugAlgo_FWD_DEFINED__
#define __IPlugAlgo_FWD_DEFINED__
typedef interface IPlugAlgo IPlugAlgo;
#endif 	/* __IPlugAlgo_FWD_DEFINED__ */


#ifndef __ISelfLearning_FWD_DEFINED__
#define __ISelfLearning_FWD_DEFINED__
typedef interface ISelfLearning ISelfLearning;
#endif 	/* __ISelfLearning_FWD_DEFINED__ */


#ifndef __PlugAlgo_FWD_DEFINED__
#define __PlugAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class PlugAlgo PlugAlgo;
#else
typedef struct PlugAlgo PlugAlgo;
#endif /* __cplusplus */

#endif 	/* __PlugAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IPlugAlgo_INTERFACE_DEFINED__
#define __IPlugAlgo_INTERFACE_DEFINED__

/* interface IPlugAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IPlugAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AECCBEBA-779E-47E3-A8A8-3C2CB7390D90")
    IPlugAlgo : public IDispatch
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRelocationAlgoHS1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pStdRadius,
            /* [in] */ VARIANT *pThreshValue,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pClosingSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRelocationAlgoHS1Help( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ImageCenterLocationAlgo( 
            /* [in] */ VARIANT *rgnID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ImageCenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetCurrentLocationXY( 
            /* [retval][out] */ BSTR *bstrLocationXY) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FilmHairDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FilmHairDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentTaskName( 
            /* [in] */ BSTR *bstrTaskName) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiameterMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdDiameter,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiameterMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BarLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThreshLowValue,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pAreaLimitSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BarLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RegionLocationByModelTool( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ BSTR *bstrRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RegionLocationByModelToolHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterReLocationAlgo2( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pUsePolarity,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterReLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pABEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pLineAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_PlatformDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_PlatformDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SlopEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SlopEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideSlopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideSlopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TriangleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pTriangleSize,
            /* [in] */ VARIANT *pTopRegArea,
            /* [in] */ VARIANT *pTriangleDist,
            /* [retval][out] */ VARIANT *pTriangleArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TriangleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_NeckRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilationSizeP,
            /* [in] */ VARIANT *pDilationSizeT) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_NeckRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SealingFaceRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SealingFaceRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ScrapEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pScrapEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ScrapEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pmoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pBlackThresh,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopRegDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pTopRegionArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopRegDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SlopEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SlopEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SideSlopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SideSlopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_NeckRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_NeckRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SideTopRegAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pLineAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SideTopRegAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_PlatformDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_PlatformDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SlopEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SlopEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SideSlopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SideSlopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_StripePatternDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_StripePatternDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_NeckRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2,
            /* [in] */ VARIANT *pDilationSizeT) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_NeckRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopAngleDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshLowerValue,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopAngleDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EccentricityDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EccentricityDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [retval][out] */ VARIANT *pAnglLineArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownCenterRelocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThreshLowValue,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pAreaLimitSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownCenterRelocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ForkTotalHeightMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ForkTotalHeightMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureSAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureSAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SidePlatformRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SidePlatformRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_WholeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_WholeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_StripePatternRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_StripePatternRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_WholeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_WholeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopInwallRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopInwallRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_WholeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pDilationSizeP,
            /* [in] */ VARIANT *pDilationSizeT,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_WholeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TriangleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TriangleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopAngleDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pBlackThresh,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopAngleDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE N_DynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE N_DynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pABEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_SideTopRegAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pLineAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_SideTopRegAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_PlatformDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pPlatformArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_PlatformDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_SlopEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_SlopEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_InWallRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSizeU,
            /* [in] */ VARIANT *pDilationSizeD,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_InWallRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_MiddleEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_MiddleEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pDilationSizeH,
            /* [in] */ VARIANT *pDialtionSizeV,
            /* [in] */ VARIANT *pErosionSizeH,
            /* [in] */ VARIANT *pErosionSizeV,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_NeckRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSizeH,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pDilationSize2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_NeckRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE N_DerivateGaussDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE N_DerivateGaussDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_WholeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pDialtionSizeP,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_WholeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshValue,
            /* [retval][out] */ VARIANT *pGapAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pDilationSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_InWallRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pPos,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_InWallRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pLowerEdgeMoveSize,
            /* [in] */ VARIANT *pHErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopSealingFaceRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pRadiusDiff,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopSealingFaceRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_DerivateGaussDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_DerivateGaussDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InWallDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InWallDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ForkTotalHeightMeasureAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ForkTotalHeightMeasureAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureSAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureSAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ScrapDiameterMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pCircularityValue,
            /* [retval][out] */ VARIANT *pDiameter) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ScrapDiameterMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoM( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThresholdUp) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoMHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SealingFaceRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SealingFaceRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRubberRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitLower) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRubberRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SideTopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SideTopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_RubberDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_RubberDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SlopEdgeDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SlopEdgeDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_NeckRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowerEdgeMoveSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pVDilationSizeH2,
            /* [in] */ VARIANT *pHDilationSizeH2,
            /* [in] */ VARIANT *pDilationSizeT) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_NeckRegionLocationAlgo1Help( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopCenterDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopCenterDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopRubberRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitLower) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopRubberRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopSealingFaceRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopSealingFaceRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WarpedMembraneDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSensitivity,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WarpedMembraneDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternCoordAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pEdgeMoveSize,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternCoordAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize1,
            /* [in] */ VARIANT *pHErosionSize2,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_SideSlopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowerEdgeMoveSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pHErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_SideSlopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pmoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SlotDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SlotDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_OutEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSize2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_OutEdgeRegionLocationAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopSealingFaceEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresh,
            /* [in] */ VARIANT *pOpeningSize,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopSealingFaceEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternSearchRegAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHDilationSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pClipSizeD,
            /* [in] */ VARIANT *pClipSizeU) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternSearchRegAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV,
            /* [in] */ VARIANT *pDilationSizeP) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE N_BubbleDetectProfileAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE N_BubbleDetectProfileAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE N_LineDetectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE N_LineDetectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopSideLowRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pDilationSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopSideLowRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DoubleFoldDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DoubleFoldDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_MiddleEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_MiddleEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternDynThresholdAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternDynThresholdAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_CrownCircleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRadius1,
            /* [in] */ VARIANT *pRoiWidth1,
            /* [in] */ VARIANT *pRadius2,
            /* [in] */ VARIANT *pRoiWidth2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_CrownCircleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RegionLocationDiffByModelTool( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ BSTR *bstrRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RegionLocationDiffByModelToolHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WhiteDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WhiteDetectAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownCenterRelocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThreshLowValue,
            /* [in] */ VARIANT *pAreaLimitSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownCenterRelocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopAngleDetectAlgo11( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pThreshLow,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopAngleDetectAlgo11Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_PlatformDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_PlatformDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideSlopRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideSlopRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TriangleDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pTriangleSize,
            /* [in] */ VARIANT *pTopRegArea,
            /* [in] */ VARIANT *pTriangleDist,
            /* [retval][out] */ VARIANT *pTriangleArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TriangleDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopAngleDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopAngleDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopCenterDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopCenterDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRubberRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitLower) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_TopRubberRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopSealingFaceRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pThresh,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopSealingFaceRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegAngleDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pLineAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegAngleDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideSlopRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideSlopRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_NeckRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowerEdgeMoveSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pVDilationSizeH2,
            /* [in] */ VARIANT *pHDilationSizeH2,
            /* [in] */ VARIANT *pDilationSizeT) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_NeckRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_PlatformDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_PlatformDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_InWallRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_InWallRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_SideTopRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_StripePatternDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D13_StripePatternDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_SideTopRegAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pLineAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_SideTopRegAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_PlatformDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_PlatformDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_SideConeBiasDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pConeDeepth,
            /* [retval][out] */ VARIANT *pConeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_SideConeBiasDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_SlopEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_SlopEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_ConPatternCoordAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pEdgeMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pWidthRadiusSet,
            /* [in] */ VARIANT *pAngleT1,
            /* [in] */ VARIANT *pAngleT2,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_ConPatternCoordAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_ConPatternSearchRegUpAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHDilationSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pClipSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_ConPatternSearchRegUpAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_CirclePatternDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pThredValue,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_CirclePatternDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_ConPatternSearchRegDownAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_ConPatternSearchRegDownAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TrianglePatternDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pTriangleSize,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TrianglePatternDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_MiddleEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_MiddleEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_InWallRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pPos,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_InWallRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WholeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WholeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshValue,
            /* [retval][out] */ VARIANT *pGapAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopCenterDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopCenterDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_SideSlopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_SideSlopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_CirclePatternDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pThredValue,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_CirclePatternDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_WholeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_WholeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopRubberRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitLower) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopRubberRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ABEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ABEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternDetectProfileAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pClipSize,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_ConPatternDetectProfileAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ABEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ABEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopForkRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitUp) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_TopForkRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_StripePatternDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_StripePatternDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopForkRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopForkRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_NeckRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSizeH,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pDilationSize2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_NeckRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_DownEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_DownEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ScrapEdgeDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pScrapEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ScrapEdgeDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_NeckRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize1) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_NeckRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_DownEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_DownEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SideSlopRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SideSlopRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopRegDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pTopRegionArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopRegDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopRegDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pTopRegionArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopRegDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BarLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pAreaLimitSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BarLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiameterMeasureAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdDiameter,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiameterMeasureAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HeightMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HeightMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *Selection,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_EmbossCoordAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pEdgeMoveSize,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pWidthRadiusSet,
            /* [in] */ VARIANT *pAngleT1,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_EmbossCoordAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_EmbossSearchRegAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHDilationSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pClipSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_EmbossSearchRegAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_NeckRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSizeH,
            /* [in] */ VARIANT *pDilationSizeH,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pDilationSize2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_NeckRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_MiddleEdgeRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV,
            /* [in] */ VARIANT *pDilationSizeP) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_MiddleEdgeRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_NeckRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_NeckRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RubberMatScrapEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMoveSize1,
            /* [in] */ VARIANT *pMoveSize2,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pScrapEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RubberMatScrapEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RubberMatRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pmoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RubberMatRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RubberDotRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RubberDotRegionLocationAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectSmalCircleAngle( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pAreaLimitSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectSmalCircleAngleHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_InnerWallRegDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pTopRegionArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_InnerWallRegDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_SideSlopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_SideSlopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [retval][out] */ VARIANT *pABEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiameterMeasureAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdDiameter,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiameterMeasureAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_NeckRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_NeckRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopDefectDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pCErosionSize,
            /* [in] */ VARIANT *pRectSegmntNum,
            /* [in] */ VARIANT *pThreshAdjust,
            /* [in] */ VARIANT *pDefectAreaThresh,
            /* [in] */ VARIANT *pMeanGrayDiffThresh,
            /* [in] */ VARIANT *pMeanGrayNeighborDiff,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopDefectDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SideSlopRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SideSlopRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_FilmOrNotDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_FilmOrNotDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CountValueNumberAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pError) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CountValueNumberAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FilmEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FilmEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousBlackPointLength,
            /* [in] */ VARIANT *pRowDiffValue,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2,
            /* [in] */ VARIANT *pAreaThred) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_FilmBigErrorDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMeanSizeWidth,
            /* [in] */ VARIANT *pMeanSizeHeight,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pClosingSize1,
            /* [in] */ VARIANT *pClosingSize2,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_FilmBigErrorDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BarLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pAreaLimitSize,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pDynThred) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BarLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopGlueDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopGlueDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pDilationSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckCornerRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNeckErosionSize,
            /* [in] */ VARIANT *pDilSizePlatform,
            /* [in] */ VARIANT *pDilSizePlatform1,
            /* [in] */ VARIANT *pDilSizeTriangle,
            /* [in] */ VARIANT *pUpRegionPos,
            /* [in] */ VARIANT *pDownRegionPos,
            /* [in] */ VARIANT *pOpeningSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckCornerRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *Selection,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pCircleUpRgnPos,
            /* [in] */ VARIANT *pTriangleDownRgnPos) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_StripePatternRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopSealingFaceRegionLocationAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopSealingFaceRegionLocationAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetWhiteErrorRegion( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pWhiteErrorRgn) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetInterImgs( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopLessRubberDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pLongLengthLimit,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopLessRubberDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckRegionLocationAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2,
            /* [in] */ VARIANT *pAreaThred) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_NeckRegionLocationAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_DownEdgeDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_DownEdgeDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_DownEdgeDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pABEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [in] */ VARIANT *pLowValue,
            /* [in] */ VARIANT *pHighValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_FilmOrNotDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMask,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pAreaSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_FilmOrNotDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo33( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureAlgo33Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo33( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CrownHeightMeasureAlgo33Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pDilationSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SideSlopRegionLocationAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_SideSlopRegionLocationAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_BottomDotRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pAngleDiff,
            /* [in] */ VARIANT *pRadiusDiff,
            /* [in] */ VARIANT *pDilationSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_BottomDotRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SealingFaceRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSealDilationSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SealingFaceRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopGlueDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pClosSize,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_TopGlueDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopBaseLineDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelecttion,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopBaseLineArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopBaseLineDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelecttion,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectFromBaseTopAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelecttion,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopBaseLineArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectFromBaseTopAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSiz,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TopEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MiddleEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSiz,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MiddleEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MiddleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSiz,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MiddleRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectFromBaseDownAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelecttion,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopBaseLineArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectFromBaseDownAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DownEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSiz,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DownEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WholeRegionLocationForHelixAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WholeRegionLocationForHelixAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BarLocationAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pAreaLimitSize,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pDynThred) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BarLocationAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopCircleLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pCircleOuter,
            /* [in] */ VARIANT *pCircleWidth,
            /* [in] */ VARIANT *pSmalCircleR,
            /* [in] */ VARIANT *pAngleAdjust,
            /* [in] */ VARIANT *pScaleSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE T20_TopCircleLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectForAnnulusAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFeatureToConv,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectForAnnulusAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_BottomRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpPosiotion,
            /* [in] */ VARIANT *pDownPosition,
            /* [in] */ VARIANT *pHorizonEro,
            /* [in] */ VARIANT *pScaleSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_BottomRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_TopInwallRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpEdgePos,
            /* [in] */ VARIANT *pDownEdgePos,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_TopInwallRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_TopHighLightRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pBottomSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_TopHighLightRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectFreeModelAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFeatureToConv1,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pFeatureToConv2,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectFreeModelAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMaskDirction,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ABEdgeRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ABEdgeRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_NeckRegionLocationAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pUpSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize1) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB20_NeckRegionLocationAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_DownEdgeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pThresholdDown,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_DownEdgeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_SideTopRegAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pTopEllipseMoveSize,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pLineAngle) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_SideTopRegAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_PlatformDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_PlatformDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_SideTopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRgnSelection,
            /* [in] */ VARIANT *pEroSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_SideTopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_OutInWallRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpSizeOuterWall,
            /* [in] */ VARIANT *pDownSizeOuterWall,
            /* [in] */ VARIANT *pAreaSize,
            /* [in] */ VARIANT *pWholeAreaSize,
            /* [in] */ VARIANT *pHPlatEroSize,
            /* [in] */ VARIANT *pHEroSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_OutInWallRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_TopAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshLowerValue,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pGapArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_TopAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RegionLocationByModelTool1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ BSTR *bstrRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RegionLocationByModelTool1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_TopRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_TopRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_StripePatternDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pStripeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE F20_StripePatternDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopHighLightRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pMoveSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopHighLightRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomCharacterLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredValue,
            /* [retval][out] */ VARIANT *pCharacterArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomCharacterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomCharacterRegionAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pEroSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomCharacterRegionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_DownEdgeDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pThresholdDown,
            /* [in] */ VARIANT *pSamePointsThred,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_DownEdgeDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ABEdgeRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pMoveDiff,
            /* [in] */ VARIANT *pIntsctThred,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ABEdgeRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ABEdgeRegionLocationAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pMoveDiff,
            /* [in] */ VARIANT *pIntsctThred,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ABEdgeRegionLocationAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectFreeModelAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [in] */ VARIANT *pSearchDirec,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pABEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE NeckBaseEdgeDetectFreeModelAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_TopHighLightRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pBottomSize,
            /* [in] */ VARIANT *pUpPose,
            /* [in] */ VARIANT *pDownPose) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_TopHighLightRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_BottomBorderRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowThred,
            /* [in] */ VARIANT *pUpEdgeSize,
            /* [in] */ VARIANT *pDownEdgeSize,
            /* [in] */ VARIANT *pHClipSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_BottomBorderRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DoubleFoldDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DoubleFoldDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_DownEdgeDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pThresholdDown,
            /* [in] */ VARIANT *pSamePointsThred,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_DownEdgeDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SidePlatformRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pABEdgeMov,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SidePlatformRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideNeckBorderRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pInnerBorderSize,
            /* [in] */ VARIANT *pOuterBorderSize,
            /* [in] */ VARIANT *pUpBoderSize,
            /* [in] */ VARIANT *pDownBoderSize,
            /* [in] */ VARIANT *pStripeScale,
            /* [in] */ VARIANT *pPlatformSize,
            /* [in] */ VARIANT *pAreaSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideNeckBorderRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SideInwallRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBaseLineMoveSize,
            /* [in] */ VARIANT *pROIScaleSize,
            /* [in] */ VARIANT *pTopRgnScaleSize,
            /* [in] */ VARIANT *pSlopScaleSize,
            /* [in] */ VARIANT *pPlatformSize,
            /* [in] */ VARIANT *pInnerEdgeSize,
            /* [in] */ VARIANT *pOuterEndgeSize,
            /* [in] */ VARIANT *pLeftUpEdgeSize,
            /* [in] */ VARIANT *pAreaSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE D20_SideInwallRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureInjectorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TotalHeightMeasureInjectorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WarpedMembraneDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pSensitivity,
            /* [in] */ VARIANT *pGrayLowThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WarpedMembraneDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopSectorRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pdUpPos,
            /* [in] */ VARIANT *pdDownPos,
            /* [in] */ VARIANT *pAngleStart,
            /* [in] */ VARIANT *pAngleEnd,
            /* [in] */ VARIANT *pOuterSize,
            /* [in] */ VARIANT *pInnerSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideTopSectorRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AngleSelectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AngleSelectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomCharacterLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredValue,
            /* [retval][out] */ VARIANT *pCharacterArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomCharacterLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotRegionLocationAlgo3( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pDilationSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomDotRegionLocationAlgo3Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectShape,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LineDetectionAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectForAnnulusAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFeatureToConv,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pDefectShape,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DerivateGaussDetectForAnnulusAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileForColorFilmAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMaskDirction,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileForColorFilmAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdForColorFilmAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdForColorFilmAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_BottomEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pUpEdgeSize,
            /* [in] */ VARIANT *pDownEdgeSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pScaleSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AB32_BottomEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FilmEdgeRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FilmEdgeRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InWallDynThresholdAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pUpEdgeExclude,
            /* [in] */ VARIANT *pDownEdgeExclude,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InWallDynThresholdAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SealingEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SealingEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopCornerLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSlopSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopCornerLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopForkEdgeRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pRelativeWidth,
            /* [in] */ VARIANT *pRelativeLength) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopForkEdgeRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopHLightLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopHLightLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileForAnnulusAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskDirection,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pDefectShape,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BubbleDetectProfileForAnnulusAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_TopRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_InWallRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSizeU,
            /* [in] */ VARIANT *pDilationSizeD,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_InWallRegionLocationAlgo1Help( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ScrapEdgeDetectAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pScrapEdgeArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ScrapEdgeDetectAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_InWallRegionLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAngleSelect,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSizeU,
            /* [in] */ VARIANT *pDilationSizeD,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkSize,
            /* [in] */ VARIANT *pForkWallSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE G20_InWallRegionLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationRightOrNotAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationRightOrNotAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomCharacterLocationAlgo2( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAngleSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pCharNum,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pSelArea,
            /* [retval][out] */ VARIANT *pCharacterArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BottomCharacterLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgoForColor( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRowBgn,
            /* [in] */ VARIANT *pRgnWidth,
            /* [in] */ VARIANT *pRgnHeight,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgoForColorHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdForColorAlgoBW( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdForColorAlgoBWHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPlugAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPlugAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPlugAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPlugAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPlugAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPlugAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPlugAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPlugAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            IPlugAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            IPlugAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            IPlugAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            IPlugAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            IPlugAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHS )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHSHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRelocationAlgoHS1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pStdRadius,
            /* [in] */ VARIANT *pThreshValue,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pClosingSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRelocationAlgoHS1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrRgnFile,
            /* [in] */ BSTR *bstrExtractRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ImageCenterLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ImageCenterLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentLocationXY )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectExcludeEdgesAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pMinEdgeSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectExcludeEdgesAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FilmHairDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FilmHairDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            IPlugAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pNumLevels,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ VARIANT *pRingOrOCR,
            /* [in] */ BSTR *bstrShm1File,
            /* [in] */ BSTR *bstrShm2File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaMinScoreAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectUseUpSizeAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pThredForeground,
            /* [in] */ VARIANT *pWhiteSize,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pBlackPointUpSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectUseUpSizeAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo )( 
            IPlugAlgo * This,
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
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiameterMeasureAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdDiameter,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiameterMeasureAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BarLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThreshLowValue,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pAreaLimitSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BarLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RegionLocationByModelTool )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ BSTR *bstrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RegionLocationByModelToolHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterReLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pUsePolarity,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterReLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pABEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pLineAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_PlatformDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_PlatformDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SlopEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SlopEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideSlopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideSlopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TriangleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pTriangleSize,
            /* [in] */ VARIANT *pTopRegArea,
            /* [in] */ VARIANT *pTriangleDist,
            /* [retval][out] */ VARIANT *pTriangleArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TriangleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_NeckRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilationSizeP,
            /* [in] */ VARIANT *pDilationSizeT);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_NeckRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SealingFaceRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SealingFaceRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ScrapEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pScrapEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ScrapEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pmoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pBlackThresh,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopRegDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pTopRegionArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopRegDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SlopEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SlopEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SideSlopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SideSlopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_NeckRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_NeckRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SideTopRegAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pLineAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SideTopRegAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_PlatformDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_PlatformDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SlopEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SlopEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SideSlopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SideSlopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_StripePatternDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_StripePatternDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_NeckRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2,
            /* [in] */ VARIANT *pDilationSizeT);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_NeckRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopAngleDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshLowerValue,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopAngleDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EccentricityDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDistDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EccentricityDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [retval][out] */ VARIANT *pAnglLineArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownCenterRelocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThreshLowValue,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pAreaLimitSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownCenterRelocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ForkTotalHeightMeasureAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ForkTotalHeightMeasureAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureSAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureSAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SidePlatformRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SidePlatformRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_WholeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_WholeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_StripePatternRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_StripePatternRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_WholeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_WholeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopInwallRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopInwallRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_WholeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pDilationSizeP,
            /* [in] */ VARIANT *pDilationSizeT,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_WholeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TriangleRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TriangleRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopAngleDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pBlackThresh,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopAngleDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *N_DynThresholdAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *N_DynThresholdAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pABEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_SideTopRegAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pLineAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_SideTopRegAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_PlatformDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pPlatformArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_PlatformDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_SlopEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_SlopEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_InWallRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSizeU,
            /* [in] */ VARIANT *pDilationSizeD,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_InWallRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_MiddleEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_MiddleEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pDilationSizeH,
            /* [in] */ VARIANT *pDialtionSizeV,
            /* [in] */ VARIANT *pErosionSizeH,
            /* [in] */ VARIANT *pErosionSizeV,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_NeckRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSizeH,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pDilationSize2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_NeckRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *N_DerivateGaussDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *N_DerivateGaussDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_WholeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pDialtionSizeP,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_WholeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshValue,
            /* [retval][out] */ VARIANT *pGapAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pDilationSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_InWallRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pPos,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_InWallRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pLowerEdgeMoveSize,
            /* [in] */ VARIANT *pHErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopSealingFaceRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pRadiusDiff,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopSealingFaceRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_DerivateGaussDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_DerivateGaussDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InWallDynThresholdAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InWallDynThresholdAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ForkTotalHeightMeasureAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ForkTotalHeightMeasureAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureSAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureSAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ScrapDiameterMeasureAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pCircularityValue,
            /* [retval][out] */ VARIANT *pDiameter);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ScrapDiameterMeasureAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoM )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThresholdUp);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoMHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SealingFaceRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SealingFaceRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRubberRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitLower);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRubberRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SideTopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SideTopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_RubberDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_RubberDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SlopEdgeDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SlopEdgeDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_NeckRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowerEdgeMoveSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pVDilationSizeH2,
            /* [in] */ VARIANT *pHDilationSizeH2,
            /* [in] */ VARIANT *pDilationSizeT);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_NeckRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopCenterDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopCenterDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopRubberRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitLower);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopRubberRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopSealingFaceRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopSealingFaceRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WarpedMembraneDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSensitivity,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WarpedMembraneDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternCoordAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pEdgeMoveSize,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternCoordAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize1,
            /* [in] */ VARIANT *pHErosionSize2,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_SideSlopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowerEdgeMoveSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pHErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_SideSlopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pmoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SlotDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SlotDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_OutEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSize2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_OutEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorAlgo )( 
            IPlugAlgo * This,
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
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopSealingFaceEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresh,
            /* [in] */ VARIANT *pOpeningSize,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopSealingFaceEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternSearchRegAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHDilationSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pClipSizeD,
            /* [in] */ VARIANT *pClipSizeU);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternSearchRegAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternDynThresholdAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternDynThresholdAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV,
            /* [in] */ VARIANT *pDilationSizeP);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *N_BubbleDetectProfileAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *N_BubbleDetectProfileAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *N_LineDetectionAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *N_LineDetectionAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopSideLowRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pDilationSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopSideLowRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleFoldDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleFoldDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_MiddleEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_MiddleEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternDynThresholdAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternDynThresholdAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_CrownCircleRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRadius1,
            /* [in] */ VARIANT *pRoiWidth1,
            /* [in] */ VARIANT *pRadius2,
            /* [in] */ VARIANT *pRoiWidth2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_CrownCircleRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RegionLocationDiffByModelTool )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ BSTR *bstrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RegionLocationDiffByModelToolHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WhiteDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WhiteDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MinMaxGrayDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MinMaxGrayDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownCenterRelocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThreshLowValue,
            /* [in] */ VARIANT *pAreaLimitSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownCenterRelocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopAngleDetectAlgo11 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pThreshLow,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopAngleDetectAlgo11Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_PlatformDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_PlatformDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideSlopRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideSlopRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TriangleDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pTriangleSize,
            /* [in] */ VARIANT *pTopRegArea,
            /* [in] */ VARIANT *pTriangleDist,
            /* [retval][out] */ VARIANT *pTriangleArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TriangleDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopAngleDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopAngleDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopCenterDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThresh,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopCenterDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRubberRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitLower);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_TopRubberRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopSealingFaceRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pThresh,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopSealingFaceRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegAngleDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pLineAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegAngleDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideSlopRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideSlopRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_NeckRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowerEdgeMoveSize,
            /* [in] */ VARIANT *pUpEdgeMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pVDilationSizeH2,
            /* [in] */ VARIANT *pHDilationSizeH2,
            /* [in] */ VARIANT *pDilationSizeT);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_NeckRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_PlatformDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_PlatformDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_InWallRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_InWallRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_SideTopRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_StripePatternDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D13_StripePatternDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_SideTopRegAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pLineAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_SideTopRegAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_PlatformDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_PlatformDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_SideConeBiasDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pConeDeepth,
            /* [retval][out] */ VARIANT *pConeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_SideConeBiasDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_SlopEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopSlopEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_SlopEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_ConPatternCoordAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pEdgeMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pWidthRadiusSet,
            /* [in] */ VARIANT *pAngleT1,
            /* [in] */ VARIANT *pAngleT2,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_ConPatternCoordAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_ConPatternSearchRegUpAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHDilationSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pClipSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_ConPatternSearchRegUpAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_CirclePatternDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pThredValue,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_CirclePatternDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_ConPatternSearchRegDownAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_ConPatternSearchRegDownAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TrianglePatternDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pTriangleSize,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TrianglePatternDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_MiddleEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_MiddleEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_InWallRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pPos,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_InWallRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WholeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WholeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshValue,
            /* [retval][out] */ VARIANT *pGapAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopCenterDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopCenterDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_SideSlopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_SideSlopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_CirclePatternDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pThredValue,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_CirclePatternDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_WholeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pABHErosionSize,
            /* [in] */ VARIANT *pABVErosionSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_WholeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopRubberRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitLower);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopRubberRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ABEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ABEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternDetectProfileAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pClipSize,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_ConPatternDetectProfileAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ABEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ABEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundForColorAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundForColorAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopForkRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pAreaLimitUp);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_TopForkRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_StripePatternDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_StripePatternDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopForkRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopForkRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_NeckRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSizeH,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pDilationSize2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_NeckRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_DownEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_DownEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ScrapEdgeDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pScrapEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ScrapEdgeDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_NeckRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize1);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_NeckRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_DownEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_DownEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SideSlopRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SideSlopRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopRegDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pTopRegionArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopRegDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopRegDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThreshUpValue,
            /* [retval][out] */ VARIANT *pTopRegionArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopRegDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BarLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pAreaLimitSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BarLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiameterMeasureAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdDiameter,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiameterMeasureAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HeightMeasureAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HeightMeasureAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *Selection,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_EmbossCoordAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pEdgeMoveSize,
            /* [in] */ VARIANT *pOpeningSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pWidthRadiusSet,
            /* [in] */ VARIANT *pAngleT1,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_EmbossCoordAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_EmbossSearchRegAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHDilationSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pClipSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_EmbossSearchRegAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_NeckRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSizeH,
            /* [in] */ VARIANT *pDilationSizeH,
            /* [in] */ VARIANT *pAreaThred,
            /* [in] */ VARIANT *pDilationSize2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_NeckRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_MiddleEdgeRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV,
            /* [in] */ VARIANT *pDilationSizeP);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_MiddleEdgeRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_NeckRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_NeckRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RubberMatScrapEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMoveSize1,
            /* [in] */ VARIANT *pMoveSize2,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pScrapEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RubberMatScrapEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RubberMatRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pmoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RubberMatRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RubberDotRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RubberDotRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBW )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBWHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            IPlugAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            IPlugAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            IPlugAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            IPlugAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            IPlugAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            IPlugAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            IPlugAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            IPlugAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            IPlugAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            IPlugAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectSmalCircleAngle )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pAreaLimitSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectSmalCircleAngleHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_InnerWallRegDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [retval][out] */ VARIANT *pTopRegionArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_InnerWallRegDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_SideSlopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_SideSlopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [retval][out] */ VARIANT *pABEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiameterMeasureAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdDiameter,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiameterMeasureAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_NeckRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_NeckRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopDefectDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pCErosionSize,
            /* [in] */ VARIANT *pRectSegmntNum,
            /* [in] */ VARIANT *pThreshAdjust,
            /* [in] */ VARIANT *pDefectAreaThresh,
            /* [in] */ VARIANT *pMeanGrayDiffThresh,
            /* [in] */ VARIANT *pMeanGrayNeighborDiff,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopDefectDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SideSlopRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SideSlopRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_FilmOrNotDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_FilmOrNotDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CountValueNumberAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pError);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CountValueNumberAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FilmEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FilmEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateDynThresholdAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousBlackPointLength,
            /* [in] */ VARIANT *pRowDiffValue,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateDynThresholdAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2,
            /* [in] */ VARIANT *pAreaThred);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_FilmBigErrorDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMeanSizeWidth,
            /* [in] */ VARIANT *pMeanSizeHeight,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pClosingSize1,
            /* [in] */ VARIANT *pClosingSize2,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_FilmBigErrorDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BarLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pAreaLimitSize,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pDynThred);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BarLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopGlueDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopGlueDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pDilationSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckCornerRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNeckErosionSize,
            /* [in] */ VARIANT *pDilSizePlatform,
            /* [in] */ VARIANT *pDilSizePlatform1,
            /* [in] */ VARIANT *pDilSizeTriangle,
            /* [in] */ VARIANT *pUpRegionPos,
            /* [in] */ VARIANT *pDownRegionPos,
            /* [in] */ VARIANT *pOpeningSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckCornerRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *Selection,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pCircleUpRgnPos,
            /* [in] */ VARIANT *pTriangleDownRgnPos);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_StripePatternRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopSealingFaceRegionLocationAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopSealingFaceRegionLocationAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhiteErrorRegion )( 
            IPlugAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pWhiteErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInterImgs )( 
            IPlugAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopLessRubberDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pLongLengthLimit,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopLessRubberDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckRegionLocationAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize0,
            /* [in] */ VARIANT *pDilationSize1,
            /* [in] */ VARIANT *pDilationSizeH1,
            /* [in] */ VARIANT *pDilationSizeH2,
            /* [in] */ VARIANT *pAreaThred);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_NeckRegionLocationAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_DownEdgeDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_DownEdgeDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_DownEdgeDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pABEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [in] */ VARIANT *pLowValue,
            /* [in] */ VARIANT *pHighValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_FilmOrNotDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMask,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pAreaSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_FilmOrNotDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo33 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureAlgo33Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo33 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdCrownHeight,
            /* [in] */ VARIANT *pPlugType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pGraythresh,
            /* [in] */ VARIANT *pRectifyValue,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CrownHeightMeasureAlgo33Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pDilationSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SideSlopRegionLocationAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEllipseMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_SideSlopRegionLocationAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_BottomDotRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pAngleDiff,
            /* [in] */ VARIANT *pRadiusDiff,
            /* [in] */ VARIANT *pDilationSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_BottomDotRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SealingFaceRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSealDilationSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SealingFaceRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopGlueDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pClosSize,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_TopGlueDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopBaseLineDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelecttion,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopBaseLineArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopBaseLineDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelecttion,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectFromBaseTopAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelecttion,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopBaseLineArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectFromBaseTopAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSiz,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TopEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MiddleEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSiz,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MiddleEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MiddleRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSiz,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MiddleRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectFromBaseDownAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelecttion,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pTopBaseLineArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectFromBaseDownAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DownEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pDilSiz,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DownEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WholeRegionLocationForHelixAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WholeRegionLocationForHelixAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BarLocationAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pAreaLimitSize,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pDynThred);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BarLocationAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopCircleLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pCircleOuter,
            /* [in] */ VARIANT *pCircleWidth,
            /* [in] */ VARIANT *pSmalCircleR,
            /* [in] */ VARIANT *pAngleAdjust,
            /* [in] */ VARIANT *pScaleSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *T20_TopCircleLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectForAnnulusAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFeatureToConv,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectForAnnulusAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_BottomRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpPosiotion,
            /* [in] */ VARIANT *pDownPosition,
            /* [in] */ VARIANT *pHorizonEro,
            /* [in] */ VARIANT *pScaleSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_BottomRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_TopInwallRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpEdgePos,
            /* [in] */ VARIANT *pDownEdgePos,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_TopInwallRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_TopHighLightRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pBottomSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_TopHighLightRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectFreeModelAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFeatureToConv1,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pFeatureToConv2,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectFreeModelAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileForColorAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMaskDirction,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileForColorAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ABEdgeRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ABEdgeRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_NeckRegionLocationAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pUpSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize,
            /* [in] */ VARIANT *pDilationSize1);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB20_NeckRegionLocationAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_DownEdgeDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pThresholdDown,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_DownEdgeDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_SideTopRegAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pTopEllipseMoveSize,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pLineAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_SideTopRegAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_PlatformDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAngle,
            /* [retval][out] */ VARIANT *pPlatformArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_PlatformDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_SideTopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRgnSelection,
            /* [in] */ VARIANT *pEroSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_SideTopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_OutInWallRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pUpSizeOuterWall,
            /* [in] */ VARIANT *pDownSizeOuterWall,
            /* [in] */ VARIANT *pAreaSize,
            /* [in] */ VARIANT *pWholeAreaSize,
            /* [in] */ VARIANT *pHPlatEroSize,
            /* [in] */ VARIANT *pHEroSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_OutInWallRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_TopAngleDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshLowerValue,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [retval][out] */ VARIANT *pGapArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_TopAngleDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RegionLocationByModelTool1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ BSTR *bstrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RegionLocationByModelTool1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_TopRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_TopRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_StripePatternDetectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHSearchSize,
            /* [in] */ VARIANT *pVSearchSize,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pStripeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *F20_StripePatternDetectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopHighLightRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pMoveSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopHighLightRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomCharacterLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredValue,
            /* [retval][out] */ VARIANT *pCharacterArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomCharacterLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomCharacterRegionAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pEroSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomCharacterRegionAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_DownEdgeDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pThresholdDown,
            /* [in] */ VARIANT *pSamePointsThred,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_DownEdgeDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ABEdgeRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pMoveDiff,
            /* [in] */ VARIANT *pIntsctThred,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ABEdgeRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ABEdgeRegionLocationAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pMoveDiff,
            /* [in] */ VARIANT *pIntsctThred,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ABEdgeRegionLocationAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectFreeModelAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHclipSize,
            /* [in] */ VARIANT *pSearchDirec,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pABEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *NeckBaseEdgeDetectFreeModelAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_TopHighLightRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pOuterEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pBottomSize,
            /* [in] */ VARIANT *pUpPose,
            /* [in] */ VARIANT *pDownPose);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_TopHighLightRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_BottomBorderRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLowThred,
            /* [in] */ VARIANT *pUpEdgeSize,
            /* [in] */ VARIANT *pDownEdgeSize,
            /* [in] */ VARIANT *pHClipSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_BottomBorderRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleFoldDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAreaLimit,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleFoldDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_DownEdgeDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pThresholdDown,
            /* [in] */ VARIANT *pSamePointsThred,
            /* [in] */ VARIANT *pValueThred,
            /* [retval][out] */ VARIANT *pEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_DownEdgeDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SidePlatformRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pABEdgeMov,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pClosingSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SidePlatformRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideNeckBorderRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pInnerBorderSize,
            /* [in] */ VARIANT *pOuterBorderSize,
            /* [in] */ VARIANT *pUpBoderSize,
            /* [in] */ VARIANT *pDownBoderSize,
            /* [in] */ VARIANT *pStripeScale,
            /* [in] */ VARIANT *pPlatformSize,
            /* [in] */ VARIANT *pAreaSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideNeckBorderRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SideInwallRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBaseLineMoveSize,
            /* [in] */ VARIANT *pROIScaleSize,
            /* [in] */ VARIANT *pTopRgnScaleSize,
            /* [in] */ VARIANT *pSlopScaleSize,
            /* [in] */ VARIANT *pPlatformSize,
            /* [in] */ VARIANT *pInnerEdgeSize,
            /* [in] */ VARIANT *pOuterEndgeSize,
            /* [in] */ VARIANT *pLeftUpEdgeSize,
            /* [in] */ VARIANT *pAreaSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *D20_SideInwallRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureInjectorAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pStdTotalHeight,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TotalHeightMeasureInjectorAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WarpedMembraneDetectAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pSensitivity,
            /* [in] */ VARIANT *pGrayLowThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WarpedMembraneDetectAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopSectorRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pdUpPos,
            /* [in] */ VARIANT *pdDownPos,
            /* [in] */ VARIANT *pAngleStart,
            /* [in] */ VARIANT *pAngleEnd,
            /* [in] */ VARIANT *pOuterSize,
            /* [in] */ VARIANT *pInnerSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideTopSectorRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AngleSelectAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AngleSelectAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomCharacterLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredValue,
            /* [retval][out] */ VARIANT *pCharacterArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomCharacterLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotRegionLocationAlgo3 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadiusE,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pCirleNum,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pDilationSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomDotRegionLocationAlgo3Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pThredUpper,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pDefectShape,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LineDetectionAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectForAnnulusAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFeatureToConv,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pDefectShape,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectForAnnulusAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileForColorFilmAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pMaskDirction,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileForColorFilmAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorFilmAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pMoveSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorFilmAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_BottomEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pUpEdgeSize,
            /* [in] */ VARIANT *pDownEdgeSize,
            /* [in] */ VARIANT *pHClipSize,
            /* [in] */ VARIANT *pScaleSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AB32_BottomEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FilmEdgeRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pDialtionSizeV);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FilmEdgeRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InWallDynThresholdAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pUpEdgeExclude,
            /* [in] */ VARIANT *pDownEdgeExclude,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InWallDynThresholdAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SealingEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pHErosionSize,
            /* [in] */ VARIANT *pVErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SealingEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopCornerLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSlopSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopCornerLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopForkEdgeRegionLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge,
            /* [in] */ VARIANT *pDilationSize,
            /* [in] */ VARIANT *pRelativeWidth,
            /* [in] */ VARIANT *pRelativeLength);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopForkEdgeRegionLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopHLightLocationAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopHLightLocationAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileForAnnulusAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskDirection,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [in] */ VARIANT *pDefectShape,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BubbleDetectProfileForAnnulusAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSelection,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThredLower,
            /* [in] */ VARIANT *pAreaLimitLower,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkEdge,
            /* [in] */ VARIANT *pInnerEdge,
            /* [in] */ VARIANT *pOutEdge);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_TopRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_InWallRegionLocationAlgo1 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSizeU,
            /* [in] */ VARIANT *pDilationSizeD,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_InWallRegionLocationAlgo1Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ScrapEdgeDetectAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pSearchSize,
            /* [retval][out] */ VARIANT *pScrapEdgeArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ScrapEdgeDetectAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_InWallRegionLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAngleSelect,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pDilationSizeU,
            /* [in] */ VARIANT *pDilationSizeD,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pForkSize,
            /* [in] */ VARIANT *pForkWallSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *G20_InWallRegionLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationRightOrNotAlgo )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationRightOrNotAlgoHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomCharacterLocationAlgo2 )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAngleSelect,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pCharNum,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pSelArea,
            /* [retval][out] */ VARIANT *pCharacterArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BottomCharacterLocationAlgo2Help )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgoForColor )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRowBgn,
            /* [in] */ VARIANT *pRgnWidth,
            /* [in] */ VARIANT *pRgnHeight,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgoForColorHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgoBW )( 
            IPlugAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgoBWHelp )( 
            IPlugAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        END_INTERFACE
    } IPlugAlgoVtbl;

    interface IPlugAlgo
    {
        CONST_VTBL struct IPlugAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPlugAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPlugAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPlugAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPlugAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPlugAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPlugAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPlugAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPlugAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define IPlugAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define IPlugAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define IPlugAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define IPlugAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define IPlugAlgo_CenterLocationAlgo(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnID,pRadius,pMinScore) ) 

#define IPlugAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CenterLocationAlgo1(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1(This,rgnID,pRadius,pMinScore) ) 

#define IPlugAlgo_CenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define IPlugAlgo_CenterLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHSHelp(This,pHelpStr) ) 

#define IPlugAlgo_CenterRelocationAlgoHS1(This,rgnId,pRadius,pStdRadius,pThreshValue,pOpeningSize,pClosingSize)	\
    ( (This)->lpVtbl -> CenterRelocationAlgoHS1(This,rgnId,pRadius,pStdRadius,pThreshValue,pOpeningSize,pClosingSize) ) 

#define IPlugAlgo_CenterRelocationAlgoHS1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRelocationAlgoHS1Help(This,pHelpStr) ) 

#define IPlugAlgo_MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaAlgo(This,rgnId,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IPlugAlgo_MatchingParaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IPlugAlgo_CircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgo(This,rgnId,pErosionSize,pRingOrOCR,bstrRgnFile,bstrExtractRgnFile) ) 

#define IPlugAlgo_DetectRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_ImageCenterLocationAlgo(This,rgnID)	\
    ( (This)->lpVtbl -> ImageCenterLocationAlgo(This,rgnID) ) 

#define IPlugAlgo_ImageCenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ImageCenterLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_GetCurrentLocationXY(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCurrentLocationXY(This,bstrLocationXY) ) 

#define IPlugAlgo_CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IPlugAlgo_CurlOptimizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea) ) 

#define IPlugAlgo_UVCurlAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> UVCurlAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CurlNoCompoundAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgo(This,rgnId,pGrayValueMean) ) 

#define IPlugAlgo_CurlNoCompoundAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BubbleDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_BubbleDetectProfileAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BubbleDetectProfileAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IPlugAlgo_AnnularRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BlackDetectExcludeEdgesAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pThresholdLow,pMinEdgeSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectExcludeEdgesAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pThresholdLow,pMinEdgeSize,pDetectArea) ) 

#define IPlugAlgo_BlackDetectExcludeEdgesAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectExcludeEdgesAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IPlugAlgo_BlackDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IPlugAlgo_EdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea) ) 

#define IPlugAlgo_LineDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectionAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_FilmHairDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> FilmHairDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_FilmHairDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> FilmHairDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SetCurrentTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName) ) 

#define IPlugAlgo_MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pRingOrOCR,bstrShm1File,bstrShm2File,pMatchingResult) ) 

#define IPlugAlgo_MatchingParaMinScoreAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaMinScoreAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BlackDetectUseUpSizeAlgo(This,rgnId,pBlackThresh,pThredForeground,pWhiteSize,pSeriousBlackPointSize,pBlackPointUpSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectUseUpSizeAlgo(This,rgnId,pBlackThresh,pThredForeground,pWhiteSize,pSeriousBlackPointSize,pBlackPointUpSize,pDetectArea) ) 

#define IPlugAlgo_BlackDetectUseUpSizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectUseUpSizeAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_TotalHeightMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_CrownHeightMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DiameterMeasureAlgo(This,rgnId,pStdDiameter,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> DiameterMeasureAlgo(This,rgnId,pStdDiameter,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_DiameterMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DiameterMeasureAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BarLocationAlgo(This,rgnId,pRadius,pRoiWidth,pMaskSize,pThreshLowValue,pThreshUpValue,pAreaLimitSize)	\
    ( (This)->lpVtbl -> BarLocationAlgo(This,rgnId,pRadius,pRoiWidth,pMaskSize,pThreshLowValue,pThreshUpValue,pAreaLimitSize) ) 

#define IPlugAlgo_BarLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BarLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_RegionLocationByModelTool(This,rgnId,bstrRgnFile)	\
    ( (This)->lpVtbl -> RegionLocationByModelTool(This,rgnId,bstrRgnFile) ) 

#define IPlugAlgo_RegionLocationByModelToolHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RegionLocationByModelToolHelp(This,pHelpStr) ) 

#define IPlugAlgo_CenterReLocationAlgo2(This,rgnID,pUsePolarity,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterReLocationAlgo2(This,rgnID,pUsePolarity,pRadius,pMinScore) ) 

#define IPlugAlgo_CenterReLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterReLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_SideCenterLocationAlgo(This,rgnId,pThresholdLow,pOutlierRemoverSize,pErosionSize)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgo(This,rgnId,pThresholdLow,pOutlierRemoverSize,pErosionSize) ) 

#define IPlugAlgo_SideCenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_NeckBaseEdgeDetectAlgo(This,rgnId,pSearchSize,pHclipSize,pMaskSize,pThredLow,pThredUpper,pABEdgeArea)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectAlgo(This,rgnId,pSearchSize,pHclipSize,pMaskSize,pThredLow,pThredUpper,pABEdgeArea) ) 

#define IPlugAlgo_NeckBaseEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pLineAngle)	\
    ( (This)->lpVtbl -> D13_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pLineAngle) ) 

#define IPlugAlgo_D13_SideTopRegAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SideTopRegAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea)	\
    ( (This)->lpVtbl -> D13_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea) ) 

#define IPlugAlgo_D13_PlatformDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_PlatformDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pTopSlopEdgeArea)	\
    ( (This)->lpVtbl -> D13_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pTopSlopEdgeArea) ) 

#define IPlugAlgo_D13_SlopEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SlopEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_SideSlopRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> D13_SideSlopRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_D13_SideSlopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SideSlopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_TriangleDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pWhiteThred,pBlackThred,pTriangleSize,pTopRegArea,pTriangleDist,pTriangleArea)	\
    ( (This)->lpVtbl -> D13_TriangleDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pWhiteThred,pBlackThred,pTriangleSize,pTopRegArea,pTriangleDist,pTriangleArea) ) 

#define IPlugAlgo_D13_TriangleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TriangleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_NeckRegionLocationAlgo(This,rgnId,pHErosionSize,pUpEdgeMoveSize,pMoveSize,pDilationSizeP,pDilationSizeT)	\
    ( (This)->lpVtbl -> D13_NeckRegionLocationAlgo(This,rgnId,pHErosionSize,pUpEdgeMoveSize,pMoveSize,pDilationSizeP,pDilationSizeT) ) 

#define IPlugAlgo_D13_NeckRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_NeckRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SealingFaceRegionLocationAlgo(This,rgnId,pThredValue,pDilationSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> SealingFaceRegionLocationAlgo(This,rgnId,pThredValue,pDilationSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_SealingFaceRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SealingFaceRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_ScrapEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pScrapEdgeArea)	\
    ( (This)->lpVtbl -> ScrapEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pScrapEdgeArea) ) 

#define IPlugAlgo_ScrapEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ScrapEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CrownRegionLocationAlgo(This,rgnId,pSelection,pDilationSize,pmoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> CrownRegionLocationAlgo(This,rgnId,pSelection,pDilationSize,pmoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_CrownRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopAngleDetectAlgo(This,rgnId,pRadius,pThreshUpValue,pBlackThresh,pGapArea)	\
    ( (This)->lpVtbl -> D13_TopAngleDetectAlgo(This,rgnId,pRadius,pThreshUpValue,pBlackThresh,pGapArea) ) 

#define IPlugAlgo_D13_TopAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB20_TopRegDetectAlgo(This,rgnId,pMoveSize,pThredLower,pTopRegionArea)	\
    ( (This)->lpVtbl -> AB20_TopRegDetectAlgo(This,rgnId,pMoveSize,pThredLower,pTopRegionArea) ) 

#define IPlugAlgo_AB20_TopRegDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_TopRegDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB20_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pTopSlopEdgeArea)	\
    ( (This)->lpVtbl -> AB20_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pTopSlopEdgeArea) ) 

#define IPlugAlgo_AB20_SlopEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_SlopEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB20_SideSlopRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> AB20_SideSlopRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_AB20_SideSlopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_SideSlopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB20_NeckRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> AB20_NeckRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_AB20_NeckRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_NeckRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pAreaLimitLower,pLineAngle)	\
    ( (This)->lpVtbl -> D20_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pAreaLimitLower,pLineAngle) ) 

#define IPlugAlgo_D20_SideTopRegAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_SideTopRegAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea)	\
    ( (This)->lpVtbl -> D20_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea) ) 

#define IPlugAlgo_D20_PlatformDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_PlatformDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pTopSlopEdgeArea)	\
    ( (This)->lpVtbl -> D20_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pTopSlopEdgeArea) ) 

#define IPlugAlgo_D20_SlopEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_SlopEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_SideSlopRegionLocationAlgo(This,rgnId,pMoveSize,pBlackThred,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> D20_SideSlopRegionLocationAlgo(This,rgnId,pMoveSize,pBlackThred,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_D20_SideSlopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_SideSlopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_StripePatternDetectAlgo(This,rgnId,pSwitch,pMoveSize,pSearchSize,pWhiteThred,pStripeArea)	\
    ( (This)->lpVtbl -> D20_StripePatternDetectAlgo(This,rgnId,pSwitch,pMoveSize,pSearchSize,pWhiteThred,pStripeArea) ) 

#define IPlugAlgo_D20_StripePatternDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_StripePatternDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_NeckRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize,pDilationSizeH1,pDilationSizeH2,pDilationSizeT)	\
    ( (This)->lpVtbl -> D20_NeckRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize,pDilationSizeH1,pDilationSizeH2,pDilationSizeT) ) 

#define IPlugAlgo_D20_NeckRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_NeckRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_TopAngleDetectAlgo(This,rgnId,pRadius,pThreshUpValue,pWhiteThresh,pAreaLimitLower,pGapArea)	\
    ( (This)->lpVtbl -> D20_TopAngleDetectAlgo(This,rgnId,pRadius,pThreshUpValue,pWhiteThresh,pAreaLimitLower,pGapArea) ) 

#define IPlugAlgo_D20_TopAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_TopAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_TopAngleDetectAlgo1(This,rgnId,pRadius,pThreshLowerValue,pAreaLimit,pAreaLimitLower,pGapArea)	\
    ( (This)->lpVtbl -> D20_TopAngleDetectAlgo1(This,rgnId,pRadius,pThreshLowerValue,pAreaLimit,pAreaLimitLower,pGapArea) ) 

#define IPlugAlgo_D20_TopAngleDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_TopAngleDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_EccentricityDetectAlgo(This,rgnId,pDistDiff)	\
    ( (This)->lpVtbl -> EccentricityDetectAlgo(This,rgnId,pDistDiff) ) 

#define IPlugAlgo_EccentricityDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EccentricityDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BottomDotDetectAlgo(This,rgnId,pRadius,pCirleNum,pMinScore,pAnglLineArea)	\
    ( (This)->lpVtbl -> BottomDotDetectAlgo(This,rgnId,pRadius,pCirleNum,pMinScore,pAnglLineArea) ) 

#define IPlugAlgo_BottomDotDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomDotDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CrownCenterRelocationAlgo(This,rgnId,pRadius,pMaskSize,pThreshLowValue,pThreshUpValue,pAreaLimitSize)	\
    ( (This)->lpVtbl -> CrownCenterRelocationAlgo(This,rgnId,pRadius,pMaskSize,pThreshLowValue,pThreshUpValue,pAreaLimitSize) ) 

#define IPlugAlgo_CrownCenterRelocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownCenterRelocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_ForkTotalHeightMeasureAlgo(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> ForkTotalHeightMeasureAlgo(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_ForkTotalHeightMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ForkTotalHeightMeasureAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CrownHeightMeasureSAlgo(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> CrownHeightMeasureSAlgo(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_CrownHeightMeasureSAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownHeightMeasureSAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SidePlatformRegionLocationAlgo(This,rgnId,pClosingSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> SidePlatformRegionLocationAlgo(This,rgnId,pClosingSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_SidePlatformRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SidePlatformRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_WholeRegionLocationAlgo(This,rgnId,pSelection,pErosionSize,pMoveSize,pDilationSize,pHErosionSize,pUpEdgeMoveSize,pABHErosionSize,pABVErosionSize)	\
    ( (This)->lpVtbl -> D20_WholeRegionLocationAlgo(This,rgnId,pSelection,pErosionSize,pMoveSize,pDilationSize,pHErosionSize,pUpEdgeMoveSize,pABHErosionSize,pABVErosionSize) ) 

#define IPlugAlgo_D20_WholeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_WholeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_StripePatternRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> D20_StripePatternRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_D20_StripePatternRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_StripePatternRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> D20_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_D20_TopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_TopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB20_WholeRegionLocationAlgo(This,rgnId,pABHErosionSize,pABVErosionSize,pErosionSize)	\
    ( (This)->lpVtbl -> AB20_WholeRegionLocationAlgo(This,rgnId,pABHErosionSize,pABVErosionSize,pErosionSize) ) 

#define IPlugAlgo_AB20_WholeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_WholeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB20_TopInwallRegionLocationAlgo(This,rgnId,pOpeningSize,pClosingSize,pErosionSize)	\
    ( (This)->lpVtbl -> AB20_TopInwallRegionLocationAlgo(This,rgnId,pOpeningSize,pClosingSize,pErosionSize) ) 

#define IPlugAlgo_AB20_TopInwallRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_TopInwallRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_WholeRegionLocationAlgo(This,rgnId,pSelection,pDilationSizeP,pDilationSizeT,pOpeningSize,pErosionSize)	\
    ( (This)->lpVtbl -> D13_WholeRegionLocationAlgo(This,rgnId,pSelection,pDilationSizeP,pDilationSizeT,pOpeningSize,pErosionSize) ) 

#define IPlugAlgo_D13_WholeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_WholeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_TriangleRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> D13_TriangleRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_D13_TriangleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TriangleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopAngleDetectAlgo1(This,rgnId,pRadius,pThreshUpValue,pBlackThresh,pGapArea)	\
    ( (This)->lpVtbl -> D13_TopAngleDetectAlgo1(This,rgnId,pRadius,pThreshUpValue,pBlackThresh,pGapArea) ) 

#define IPlugAlgo_D13_TopAngleDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopAngleDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> D13_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_D13_TopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_N_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> N_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_N_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> N_DynThresholdAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_NeckBaseEdgeDetectAlgo1(This,rgnId,pSearchSize,pHclipSize,pThreshold,pABEdgeArea)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectAlgo1(This,rgnId,pSearchSize,pHclipSize,pThreshold,pABEdgeArea) ) 

#define IPlugAlgo_NeckBaseEdgeDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pLineAngle)	\
    ( (This)->lpVtbl -> G20_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pLineAngle) ) 

#define IPlugAlgo_G20_SideTopRegAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_SideTopRegAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pPlatformArea)	\
    ( (This)->lpVtbl -> G20_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pPlatformArea) ) 

#define IPlugAlgo_G20_PlatformDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_PlatformDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pTopSlopEdgeArea)	\
    ( (This)->lpVtbl -> G20_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pTopSlopEdgeArea) ) 

#define IPlugAlgo_G20_SlopEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_SlopEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_InWallRegionLocationAlgo(This,rgnId,pGrayValue,pDilationSizeU,pDilationSizeD,pErosionSize)	\
    ( (This)->lpVtbl -> G20_InWallRegionLocationAlgo(This,rgnId,pGrayValue,pDilationSizeU,pDilationSizeD,pErosionSize) ) 

#define IPlugAlgo_G20_InWallRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_InWallRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_MiddleEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pEdgeArea)	\
    ( (This)->lpVtbl -> G20_MiddleEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pEdgeArea) ) 

#define IPlugAlgo_G20_MiddleEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_MiddleEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_DownEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea)	\
    ( (This)->lpVtbl -> G20_DownEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea) ) 

#define IPlugAlgo_G20_DownEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_DownEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_ConPatternDetectAlgo(This,rgnId,pAreaLimit,pGrayValue,pOpeningSize,pDilationSizeH,pDialtionSizeV,pErosionSizeH,pErosionSizeV,pStripeArea)	\
    ( (This)->lpVtbl -> G20_ConPatternDetectAlgo(This,rgnId,pAreaLimit,pGrayValue,pOpeningSize,pDilationSizeH,pDialtionSizeV,pErosionSizeH,pErosionSizeV,pStripeArea) ) 

#define IPlugAlgo_G20_ConPatternDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_ConPatternDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_NeckRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pSwitch,pMoveSizeH,pAreaThred,pDilationSize2)	\
    ( (This)->lpVtbl -> G20_NeckRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pSwitch,pMoveSizeH,pAreaThred,pDilationSize2) ) 

#define IPlugAlgo_G20_NeckRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_NeckRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_N_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> N_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_N_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> N_DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_WholeRegionLocationAlgo(This,rgnId,pUpEdgeMoveSize,pDialtionSizeP,pABHErosionSize,pABVErosionSize,pErosionSize)	\
    ( (This)->lpVtbl -> G20_WholeRegionLocationAlgo(This,rgnId,pUpEdgeMoveSize,pDialtionSizeP,pABHErosionSize,pABVErosionSize,pErosionSize) ) 

#define IPlugAlgo_G20_WholeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_WholeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopAngleDetectAlgo(This,rgnId,pThreshValue,pGapAngle)	\
    ( (This)->lpVtbl -> G20_TopAngleDetectAlgo(This,rgnId,pThreshValue,pGapAngle) ) 

#define IPlugAlgo_G20_TopAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> G20_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_G20_TopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BottomDotRegionLocationAlgo(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pCirleNum,pMinScore,pDilationSize)	\
    ( (This)->lpVtbl -> BottomDotRegionLocationAlgo(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pCirleNum,pMinScore,pDilationSize) ) 

#define IPlugAlgo_BottomDotRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomDotRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CircleRegionLocationAlgo1(This,rgnId,pMoveSize,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1(This,rgnId,pMoveSize,pRadius,pRoiWidth) ) 

#define IPlugAlgo_CircleRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_InWallRegionLocationAlgo(This,rgnId,pGrayThred,pPos,pErosionSize)	\
    ( (This)->lpVtbl -> D13_InWallRegionLocationAlgo(This,rgnId,pGrayThred,pPos,pErosionSize) ) 

#define IPlugAlgo_D13_InWallRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_InWallRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_NeckRegionLocationAlgo1(This,rgnId,pUpEdgeMoveSize,pLowerEdgeMoveSize,pHErosionSize)	\
    ( (This)->lpVtbl -> NeckRegionLocationAlgo1(This,rgnId,pUpEdgeMoveSize,pLowerEdgeMoveSize,pHErosionSize) ) 

#define IPlugAlgo_NeckRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> NeckRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_TopSealingFaceRegionLocationAlgo(This,rgnId,pRadius,pGrayThred,pClosingSize,pRadiusDiff,pErosionSize)	\
    ( (This)->lpVtbl -> TopSealingFaceRegionLocationAlgo(This,rgnId,pRadius,pGrayThred,pClosingSize,pRadiusDiff,pErosionSize) ) 

#define IPlugAlgo_TopSealingFaceRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopSealingFaceRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> D13_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDilationSize,pDetectArea) ) 

#define IPlugAlgo_D13_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_InWallDynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> InWallDynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_InWallDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> InWallDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo1(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pGraythresh,pDefectDiff)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo1(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pGraythresh,pDefectDiff) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo1(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pGraythresh,pDefectDiff)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo1(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pGraythresh,pDefectDiff) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_ForkTotalHeightMeasureAlgo1(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pGraythresh,pDefectDiff)	\
    ( (This)->lpVtbl -> ForkTotalHeightMeasureAlgo1(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pGraythresh,pDefectDiff) ) 

#define IPlugAlgo_ForkTotalHeightMeasureAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> ForkTotalHeightMeasureAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_CrownHeightMeasureSAlgo1(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pGraythresh,pDefectDiff)	\
    ( (This)->lpVtbl -> CrownHeightMeasureSAlgo1(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pGraythresh,pDefectDiff) ) 

#define IPlugAlgo_CrownHeightMeasureSAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownHeightMeasureSAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_ScrapDiameterMeasureAlgo(This,rgnId,pSigma,pThredLower,pThredUpper,pCircularityValue,pDiameter)	\
    ( (This)->lpVtbl -> ScrapDiameterMeasureAlgo(This,rgnId,pSigma,pThredLower,pThredUpper,pCircularityValue,pDiameter) ) 

#define IPlugAlgo_ScrapDiameterMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ScrapDiameterMeasureAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CenterLocationAlgoM(This,rgnId,pRowStart,pColStart,pRegWidth,pRegHeight,pThresholdUp)	\
    ( (This)->lpVtbl -> CenterLocationAlgoM(This,rgnId,pRowStart,pColStart,pRegWidth,pRegHeight,pThresholdUp) ) 

#define IPlugAlgo_CenterLocationAlgoMHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoMHelp(This,pHelpStr) ) 

#define IPlugAlgo_SealingFaceRegionLocationAlgo1(This,rgnId,pThredValue,pDilationSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> SealingFaceRegionLocationAlgo1(This,rgnId,pThredValue,pDilationSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_SealingFaceRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SealingFaceRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopRubberRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pAreaLimitLower)	\
    ( (This)->lpVtbl -> D13_TopRubberRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pAreaLimitLower) ) 

#define IPlugAlgo_D13_TopRubberRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopRubberRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_SideTopRegionLocationAlgo(This,rgnId,pMoveSize,pThredValue,pClosingSize,pErosionSize)	\
    ( (This)->lpVtbl -> D13_SideTopRegionLocationAlgo(This,rgnId,pMoveSize,pThredValue,pClosingSize,pErosionSize) ) 

#define IPlugAlgo_D13_SideTopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SideTopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_SideTopRegionLocationAlgo(This,rgnId,pMoveSize,pErosionSize)	\
    ( (This)->lpVtbl -> D20_SideTopRegionLocationAlgo(This,rgnId,pMoveSize,pErosionSize) ) 

#define IPlugAlgo_D20_SideTopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_SideTopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_RubberDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pOpeningSize,pDilationSize,pDetectArea)	\
    ( (This)->lpVtbl -> D20_RubberDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pOpeningSize,pDilationSize,pDetectArea) ) 

#define IPlugAlgo_D20_RubberDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_RubberDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_SlopEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pThreshold,pTopSlopEdgeArea)	\
    ( (This)->lpVtbl -> D20_SlopEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pThreshold,pTopSlopEdgeArea) ) 

#define IPlugAlgo_D20_SlopEdgeDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_SlopEdgeDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D20_NeckRegionLocationAlgo1(This,rgnId,pLowerEdgeMoveSize,pUpEdgeMoveSize,pHErosionSize,pMoveSize,pDilationSizeH1,pVDilationSizeH2,pHDilationSizeH2,pDilationSizeT)	\
    ( (This)->lpVtbl -> D20_NeckRegionLocationAlgo1(This,rgnId,pLowerEdgeMoveSize,pUpEdgeMoveSize,pHErosionSize,pMoveSize,pDilationSizeH1,pVDilationSizeH2,pHDilationSizeH2,pDilationSizeT) ) 

#define IPlugAlgo_D20_NeckRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_NeckRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IPlugAlgo_GSurfaceAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GSurfaceAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_TopCenterDetectAlgo(This,rgnId,pThreshUpValue,pGapArea)	\
    ( (This)->lpVtbl -> D20_TopCenterDetectAlgo(This,rgnId,pThreshUpValue,pGapArea) ) 

#define IPlugAlgo_D20_TopCenterDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_TopCenterDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_TopRubberRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pAreaLimitLower)	\
    ( (This)->lpVtbl -> D20_TopRubberRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pAreaLimitLower) ) 

#define IPlugAlgo_D20_TopRubberRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_TopRubberRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_TopSealingFaceRegionLocationAlgo1(This,rgnId,pRadius,pErosionSize)	\
    ( (This)->lpVtbl -> TopSealingFaceRegionLocationAlgo1(This,rgnId,pRadius,pErosionSize) ) 

#define IPlugAlgo_TopSealingFaceRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopSealingFaceRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_WarpedMembraneDetectAlgo(This,rgnId,pSensitivity,pDetectArea)	\
    ( (This)->lpVtbl -> WarpedMembraneDetectAlgo(This,rgnId,pSensitivity,pDetectArea) ) 

#define IPlugAlgo_WarpedMembraneDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WarpedMembraneDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_ConPatternCoordAlgo(This,rgnId,pEllipseMoveSize,pEdgeMoveSize,pOpeningSize,pSearchSize,pGrayValue,pStripeArea)	\
    ( (This)->lpVtbl -> G20_ConPatternCoordAlgo(This,rgnId,pEllipseMoveSize,pEdgeMoveSize,pOpeningSize,pSearchSize,pGrayValue,pStripeArea) ) 

#define IPlugAlgo_G20_ConPatternCoordAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_ConPatternCoordAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_ConPatternRegionLocationAlgo(This,rgnId,pHErosionSize1,pHErosionSize2,pVErosionSize)	\
    ( (This)->lpVtbl -> G20_ConPatternRegionLocationAlgo(This,rgnId,pHErosionSize1,pHErosionSize2,pVErosionSize) ) 

#define IPlugAlgo_G20_ConPatternRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_ConPatternRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_DownEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV)	\
    ( (This)->lpVtbl -> G20_DownEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV) ) 

#define IPlugAlgo_G20_DownEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_DownEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_SideSlopRegionLocationAlgo(This,rgnId,pLowerEdgeMoveSize,pUpEdgeMoveSize,pForkEdge,pHErosionSize)	\
    ( (This)->lpVtbl -> G20_SideSlopRegionLocationAlgo(This,rgnId,pLowerEdgeMoveSize,pUpEdgeMoveSize,pForkEdge,pHErosionSize) ) 

#define IPlugAlgo_G20_SideSlopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_SideSlopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CrownRegionLocationAlgo1(This,rgnId,pSelection,pDilationSize,pmoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> CrownRegionLocationAlgo1(This,rgnId,pSelection,pDilationSize,pmoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_CrownRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_SlotDetectAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> SlotDetectAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea) ) 

#define IPlugAlgo_SlotDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SlotDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_OutEdgeRegionLocationAlgo(This,rgnId,pDilationSize1,pDilationSize2)	\
    ( (This)->lpVtbl -> G20_OutEdgeRegionLocationAlgo(This,rgnId,pDilationSize1,pDilationSize2) ) 

#define IPlugAlgo_G20_OutEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_OutEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_DynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IPlugAlgo_EdgeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeForColorAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IPlugAlgo_AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopSealingFaceEdgeDetectAlgo(This,rgnId,pThresh,pOpeningSize,pGapArea)	\
    ( (This)->lpVtbl -> G20_TopSealingFaceEdgeDetectAlgo(This,rgnId,pThresh,pOpeningSize,pGapArea) ) 

#define IPlugAlgo_G20_TopSealingFaceEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopSealingFaceEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_ConPatternSearchRegAlgo(This,rgnId,pMoveSize,pHDilationSize,pSearchSize,pClipSizeD,pClipSizeU)	\
    ( (This)->lpVtbl -> G20_ConPatternSearchRegAlgo(This,rgnId,pMoveSize,pHDilationSize,pSearchSize,pClipSizeD,pClipSizeU) ) 

#define IPlugAlgo_G20_ConPatternSearchRegAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_ConPatternSearchRegAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_ConPatternDynThresholdAlgo(This,rgnId,pAreaLimit,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> G20_ConPatternDynThresholdAlgo(This,rgnId,pAreaLimit,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_G20_ConPatternDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_ConPatternDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_DownEdgeRegionLocationAlgo1(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV,pDilationSizeP)	\
    ( (This)->lpVtbl -> G20_DownEdgeRegionLocationAlgo1(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV,pDilationSizeP) ) 

#define IPlugAlgo_G20_DownEdgeRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_DownEdgeRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_N_BubbleDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> N_BubbleDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_N_BubbleDetectProfileAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> N_BubbleDetectProfileAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_N_LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> N_LineDetectionAlgo(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea) ) 

#define IPlugAlgo_N_LineDetectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> N_LineDetectionAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopSideLowRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pDilationSize)	\
    ( (This)->lpVtbl -> G20_TopSideLowRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pDilationSize) ) 

#define IPlugAlgo_G20_TopSideLowRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopSideLowRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DoubleFoldDetectAlgo(This,rgnId,pAreaLimit,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea)	\
    ( (This)->lpVtbl -> DoubleFoldDetectAlgo(This,rgnId,pAreaLimit,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea) ) 

#define IPlugAlgo_DoubleFoldDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DoubleFoldDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV)	\
    ( (This)->lpVtbl -> G20_MiddleEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV) ) 

#define IPlugAlgo_G20_MiddleEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_MiddleEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_ConPatternDynThresholdAlgo1(This,rgnId,pAreaLimit,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> G20_ConPatternDynThresholdAlgo1(This,rgnId,pAreaLimit,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_G20_ConPatternDynThresholdAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_ConPatternDynThresholdAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_CrownCircleRegionLocationAlgo(This,rgnId,pRadius,pRadius1,pRoiWidth1,pRadius2,pRoiWidth2)	\
    ( (This)->lpVtbl -> AB20_CrownCircleRegionLocationAlgo(This,rgnId,pRadius,pRadius1,pRoiWidth1,pRadius2,pRoiWidth2) ) 

#define IPlugAlgo_AB20_CrownCircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_CrownCircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_RegionLocationDiffByModelTool(This,rgnId,pRadius,bstrRgnFile)	\
    ( (This)->lpVtbl -> RegionLocationDiffByModelTool(This,rgnId,pRadius,bstrRgnFile) ) 

#define IPlugAlgo_RegionLocationDiffByModelToolHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RegionLocationDiffByModelToolHelp(This,pHelpStr) ) 

#define IPlugAlgo_WhiteDetectAlgo(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pWhiteThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> WhiteDetectAlgo(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pWhiteThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_WhiteDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WhiteDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_MinMaxGrayDetectAlgo(This,rgnId,pBlackThresh,pSeriousBlackPointSize,pWhiteThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> MinMaxGrayDetectAlgo(This,rgnId,pBlackThresh,pSeriousBlackPointSize,pWhiteThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_MinMaxGrayDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MinMaxGrayDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CrownCenterRelocationAlgo1(This,rgnId,pRadius,pMaskSize,pThreshLowValue,pAreaLimitSize)	\
    ( (This)->lpVtbl -> CrownCenterRelocationAlgo1(This,rgnId,pRadius,pMaskSize,pThreshLowValue,pAreaLimitSize) ) 

#define IPlugAlgo_CrownCenterRelocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownCenterRelocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopAngleDetectAlgo11(This,rgnId,pRadius,pThreshUpValue,pBlackThresh,pThreshLow,pGapArea)	\
    ( (This)->lpVtbl -> D13_TopAngleDetectAlgo11(This,rgnId,pRadius,pThreshUpValue,pBlackThresh,pThreshLow,pGapArea) ) 

#define IPlugAlgo_D13_TopAngleDetectAlgo11Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopAngleDetectAlgo11Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopRegionLocationAlgo1(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> D13_TopRegionLocationAlgo1(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_D13_TopRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_PlatformDetectAlgo1(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea)	\
    ( (This)->lpVtbl -> D13_PlatformDetectAlgo1(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea) ) 

#define IPlugAlgo_D13_PlatformDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_PlatformDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_SideSlopRegionLocationAlgo1(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> D13_SideSlopRegionLocationAlgo1(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_D13_SideSlopRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SideSlopRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_SideTopRegionLocationAlgo1(This,rgnId,pMoveSize,pThredValue,pClosingSize,pErosionSize)	\
    ( (This)->lpVtbl -> D13_SideTopRegionLocationAlgo1(This,rgnId,pMoveSize,pThredValue,pClosingSize,pErosionSize) ) 

#define IPlugAlgo_D13_SideTopRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SideTopRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_TriangleDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pWhiteThred,pBlackThred,pTriangleSize,pTopRegArea,pTriangleDist,pTriangleArea)	\
    ( (This)->lpVtbl -> D13_TriangleDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pWhiteThred,pBlackThred,pTriangleSize,pTopRegArea,pTriangleDist,pTriangleArea) ) 

#define IPlugAlgo_D13_TriangleDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TriangleDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopAngleDetectAlgo2(This,rgnId,pRadius,pBlackThresh,pThreshUpValue,pGapArea)	\
    ( (This)->lpVtbl -> D13_TopAngleDetectAlgo2(This,rgnId,pRadius,pBlackThresh,pThreshUpValue,pGapArea) ) 

#define IPlugAlgo_D13_TopAngleDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopAngleDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopCenterDetectAlgo(This,rgnId,pBlackThresh,pGapArea)	\
    ( (This)->lpVtbl -> D13_TopCenterDetectAlgo(This,rgnId,pBlackThresh,pGapArea) ) 

#define IPlugAlgo_D13_TopCenterDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopCenterDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopRegionLocationAlgo2(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> D13_TopRegionLocationAlgo2(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_D13_TopRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_TopRubberRegionLocationAlgo2(This,rgnId,pInnerEdge,pOutEdge,pAreaLimitLower)	\
    ( (This)->lpVtbl -> D13_TopRubberRegionLocationAlgo2(This,rgnId,pInnerEdge,pOutEdge,pAreaLimitLower) ) 

#define IPlugAlgo_D13_TopRubberRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_TopRubberRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_TopSealingFaceRegionLocationAlgo2(This,rgnId,pRadius,pRoiWidth,pThresh,pErosionSize)	\
    ( (This)->lpVtbl -> TopSealingFaceRegionLocationAlgo2(This,rgnId,pRadius,pRoiWidth,pThresh,pErosionSize) ) 

#define IPlugAlgo_TopSealingFaceRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopSealingFaceRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_SideTopRegAngleDetectAlgo2(This,rgnId,pMoveSize,pThredLower,pLineAngle)	\
    ( (This)->lpVtbl -> D13_SideTopRegAngleDetectAlgo2(This,rgnId,pMoveSize,pThredLower,pLineAngle) ) 

#define IPlugAlgo_D13_SideTopRegAngleDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SideTopRegAngleDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_SideSlopRegionLocationAlgo2(This,rgnId,pMoveSize,pBlackThred,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> D13_SideSlopRegionLocationAlgo2(This,rgnId,pMoveSize,pBlackThred,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_D13_SideSlopRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SideSlopRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_NeckRegionLocationAlgo2(This,rgnId,pLowerEdgeMoveSize,pUpEdgeMoveSize,pHErosionSize,pMoveSize,pDilationSizeH1,pVDilationSizeH2,pHDilationSizeH2,pDilationSizeT)	\
    ( (This)->lpVtbl -> D13_NeckRegionLocationAlgo2(This,rgnId,pLowerEdgeMoveSize,pUpEdgeMoveSize,pHErosionSize,pMoveSize,pDilationSizeH1,pVDilationSizeH2,pHDilationSizeH2,pDilationSizeT) ) 

#define IPlugAlgo_D13_NeckRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_NeckRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_PlatformDetectAlgo2(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea)	\
    ( (This)->lpVtbl -> D13_PlatformDetectAlgo2(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea) ) 

#define IPlugAlgo_D13_PlatformDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_PlatformDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_InWallRegionLocationAlgo2(This,rgnId,pGrayThred,pBlackThred,pErosionSize)	\
    ( (This)->lpVtbl -> D13_InWallRegionLocationAlgo2(This,rgnId,pGrayThred,pBlackThred,pErosionSize) ) 

#define IPlugAlgo_D13_InWallRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_InWallRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_SideTopRegionLocationAlgo2(This,rgnId,pMoveSize,pErosionSize)	\
    ( (This)->lpVtbl -> D13_SideTopRegionLocationAlgo2(This,rgnId,pMoveSize,pErosionSize) ) 

#define IPlugAlgo_D13_SideTopRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_SideTopRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_D13_StripePatternDetectAlgo2(This,rgnId,pSwitch,pMoveSize,pSearchSize,pWhiteThred,pStripeArea)	\
    ( (This)->lpVtbl -> D13_StripePatternDetectAlgo2(This,rgnId,pSwitch,pMoveSize,pSearchSize,pWhiteThred,pStripeArea) ) 

#define IPlugAlgo_D13_StripePatternDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D13_StripePatternDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_T20_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pAreaLimitLower,pLineAngle)	\
    ( (This)->lpVtbl -> T20_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pAreaLimitLower,pLineAngle) ) 

#define IPlugAlgo_T20_SideTopRegAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_SideTopRegAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pOpeningSize,pAngle,pPlatformArea)	\
    ( (This)->lpVtbl -> T20_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pOpeningSize,pAngle,pPlatformArea) ) 

#define IPlugAlgo_T20_PlatformDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_PlatformDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_SideConeBiasDetectAlgo(This,rgnId,pConeDeepth,pConeArea)	\
    ( (This)->lpVtbl -> T20_SideConeBiasDetectAlgo(This,rgnId,pConeDeepth,pConeArea) ) 

#define IPlugAlgo_T20_SideConeBiasDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_SideConeBiasDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pThreshold,pTopSlopEdgeArea)	\
    ( (This)->lpVtbl -> T20_SlopEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pThreshold,pTopSlopEdgeArea) ) 

#define IPlugAlgo_T20_SlopEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_SlopEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_ConPatternCoordAlgo(This,rgnId,pEllipseMoveSize,pEdgeMoveSize,pSearchSize,pGrayValue,pWidthRadiusSet,pAngleT1,pAngleT2,pStripeArea)	\
    ( (This)->lpVtbl -> T20_ConPatternCoordAlgo(This,rgnId,pEllipseMoveSize,pEdgeMoveSize,pSearchSize,pGrayValue,pWidthRadiusSet,pAngleT1,pAngleT2,pStripeArea) ) 

#define IPlugAlgo_T20_ConPatternCoordAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_ConPatternCoordAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_ConPatternSearchRegUpAlgo(This,rgnId,pMoveSize,pHDilationSize,pSearchSize,pClipSize)	\
    ( (This)->lpVtbl -> T20_ConPatternSearchRegUpAlgo(This,rgnId,pMoveSize,pHDilationSize,pSearchSize,pClipSize) ) 

#define IPlugAlgo_T20_ConPatternSearchRegUpAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_ConPatternSearchRegUpAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_CirclePatternDetectAlgo(This,rgnId,pMaskSize,pSeriousBlackPointDynThresh,pSeriousWhitePointDynThresh,pThredValue,pStripeArea)	\
    ( (This)->lpVtbl -> T20_CirclePatternDetectAlgo(This,rgnId,pMaskSize,pSeriousBlackPointDynThresh,pSeriousWhitePointDynThresh,pThredValue,pStripeArea) ) 

#define IPlugAlgo_T20_CirclePatternDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_CirclePatternDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_ConPatternSearchRegDownAlgo(This,rgnId,pMoveSize,pSearchSize)	\
    ( (This)->lpVtbl -> T20_ConPatternSearchRegDownAlgo(This,rgnId,pMoveSize,pSearchSize) ) 

#define IPlugAlgo_T20_ConPatternSearchRegDownAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_ConPatternSearchRegDownAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_TrianglePatternDetectAlgo(This,rgnId,pGrayValue,pTriangleSize,pStripeArea)	\
    ( (This)->lpVtbl -> T20_TrianglePatternDetectAlgo(This,rgnId,pGrayValue,pTriangleSize,pStripeArea) ) 

#define IPlugAlgo_T20_TrianglePatternDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_TrianglePatternDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_MiddleEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pEdgeArea)	\
    ( (This)->lpVtbl -> T20_MiddleEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pEdgeArea) ) 

#define IPlugAlgo_T20_MiddleEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_MiddleEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_NeckRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pDilationSizeH1,pDilationSizeH2)	\
    ( (This)->lpVtbl -> T20_NeckRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pDilationSizeH1,pDilationSizeH2) ) 

#define IPlugAlgo_T20_NeckRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_NeckRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_InWallRegionLocationAlgo(This,rgnId,pPos,pGrayThred,pAreaLimitLower,pErosionSize)	\
    ( (This)->lpVtbl -> T20_InWallRegionLocationAlgo(This,rgnId,pPos,pGrayThred,pAreaLimitLower,pErosionSize) ) 

#define IPlugAlgo_T20_InWallRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_InWallRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SideTopRegionLocationAlgo(This,rgnId,pMoveSize,pErosionSize)	\
    ( (This)->lpVtbl -> SideTopRegionLocationAlgo(This,rgnId,pMoveSize,pErosionSize) ) 

#define IPlugAlgo_SideTopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideTopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_WholeRegionLocationAlgo(This,rgnId,pABHErosionSize,pABVErosionSize,pDilationSize,pErosionSize)	\
    ( (This)->lpVtbl -> WholeRegionLocationAlgo(This,rgnId,pABHErosionSize,pABVErosionSize,pDilationSize,pErosionSize) ) 

#define IPlugAlgo_WholeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WholeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_TopAngleDetectAlgo(This,rgnId,pThreshValue,pGapAngle)	\
    ( (This)->lpVtbl -> T20_TopAngleDetectAlgo(This,rgnId,pThreshValue,pGapAngle) ) 

#define IPlugAlgo_T20_TopAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_TopAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_TopCenterDetectAlgo(This,rgnId,pThreshUpValue,pGapArea)	\
    ( (This)->lpVtbl -> TopCenterDetectAlgo(This,rgnId,pThreshUpValue,pGapArea) ) 

#define IPlugAlgo_TopCenterDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopCenterDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> T20_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_T20_TopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_TopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_SideSlopRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> T20_SideSlopRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_T20_SideSlopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_SideSlopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_StripePatternDetectAlgo(This,rgnId,pGrayValue,pStripeArea)	\
    ( (This)->lpVtbl -> T20_StripePatternDetectAlgo(This,rgnId,pGrayValue,pStripeArea) ) 

#define IPlugAlgo_T20_StripePatternDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_StripePatternDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_CirclePatternDetectAlgo1(This,rgnId,pMaskSize,pSeriousBlackPointDynThresh,pSeriousWhitePointDynThresh,pThredValue,pStripeArea)	\
    ( (This)->lpVtbl -> T20_CirclePatternDetectAlgo1(This,rgnId,pMaskSize,pSeriousBlackPointDynThresh,pSeriousWhitePointDynThresh,pThredValue,pStripeArea) ) 

#define IPlugAlgo_T20_CirclePatternDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_CirclePatternDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_T20_WholeRegionLocationAlgo(This,rgnId,pABHErosionSize,pABVErosionSize,pDilationSize,pDilationSize0,pErosionSize)	\
    ( (This)->lpVtbl -> T20_WholeRegionLocationAlgo(This,rgnId,pABHErosionSize,pABVErosionSize,pDilationSize,pDilationSize0,pErosionSize) ) 

#define IPlugAlgo_T20_WholeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_WholeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_TopRubberRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pAreaLimitLower)	\
    ( (This)->lpVtbl -> T20_TopRubberRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pAreaLimitLower) ) 

#define IPlugAlgo_T20_TopRubberRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_TopRubberRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_StripePatternRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> T20_StripePatternRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_T20_StripePatternRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_StripePatternRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_ABEdgeRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> G20_ABEdgeRegionLocationAlgo(This,rgnId,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_G20_ABEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_ABEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_ConPatternDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pClipSize,pAreaLimit,pDetectArea)	\
    ( (This)->lpVtbl -> G20_ConPatternDetectProfileAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pClipSize,pAreaLimit,pDetectArea) ) 

#define IPlugAlgo_G20_ConPatternDetectProfileAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_ConPatternDetectProfileAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SideTopRegionLocationAlgo1(This,rgnId,pMoveSize,pThredValue,pAreaLimitLower,pErosionSize)	\
    ( (This)->lpVtbl -> SideTopRegionLocationAlgo1(This,rgnId,pMoveSize,pThredValue,pAreaLimitLower,pErosionSize) ) 

#define IPlugAlgo_SideTopRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideTopRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_ABEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> ABEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_ABEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ABEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SideTopEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pThredValue,pDilationSize,pAreaLimitLower,pErosionSize)	\
    ( (This)->lpVtbl -> SideTopEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pThredValue,pDilationSize,pAreaLimitLower,pErosionSize) ) 

#define IPlugAlgo_SideTopEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideTopEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CurlNoCompoundForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pGrayValueMean) ) 

#define IPlugAlgo_CurlNoCompoundForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundForColorAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_TopForkRegionLocationAlgo(This,rgnId,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge,pAreaLimitUp)	\
    ( (This)->lpVtbl -> D20_TopForkRegionLocationAlgo(This,rgnId,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge,pAreaLimitUp) ) 

#define IPlugAlgo_D20_TopForkRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_TopForkRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_StripePatternDetectAlgo1(This,rgnId,pSwitch,pMoveSize,pSearchSize,pWhiteThred,pDilationSize,pStripeArea)	\
    ( (This)->lpVtbl -> D20_StripePatternDetectAlgo1(This,rgnId,pSwitch,pMoveSize,pSearchSize,pWhiteThred,pDilationSize,pStripeArea) ) 

#define IPlugAlgo_D20_StripePatternDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_StripePatternDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_T20_TopForkRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pThredLower,pAreaLimitLower,pThreshUpValue,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> T20_TopForkRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pThredLower,pAreaLimitLower,pThreshUpValue,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_T20_TopForkRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_TopForkRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_StripePatternDetectAlgo1(This,rgnId,pGrayValue,pDilationSize,pStripeArea)	\
    ( (This)->lpVtbl -> T20_StripePatternDetectAlgo1(This,rgnId,pGrayValue,pDilationSize,pStripeArea) ) 

#define IPlugAlgo_T20_StripePatternDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_StripePatternDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_T20_NeckRegionLocationAlgo1(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pDilationSizeH1,pDilationSizeH2)	\
    ( (This)->lpVtbl -> T20_NeckRegionLocationAlgo1(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pDilationSizeH1,pDilationSizeH2) ) 

#define IPlugAlgo_T20_NeckRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_NeckRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_DownEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea)	\
    ( (This)->lpVtbl -> G20_DownEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea) ) 

#define IPlugAlgo_G20_DownEdgeDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_DownEdgeDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_NeckRegionLocationAlgo1(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pSwitch,pMoveSizeH,pAreaThred,pDilationSize2)	\
    ( (This)->lpVtbl -> G20_NeckRegionLocationAlgo1(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pSwitch,pMoveSizeH,pAreaThred,pDilationSize2) ) 

#define IPlugAlgo_G20_NeckRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_NeckRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_DownEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea)	\
    ( (This)->lpVtbl -> AB20_DownEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea) ) 

#define IPlugAlgo_AB20_DownEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_DownEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_ScrapEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pScrapEdgeArea)	\
    ( (This)->lpVtbl -> ScrapEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pScrapEdgeArea) ) 

#define IPlugAlgo_ScrapEdgeDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> ScrapEdgeDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_NeckRegionLocationAlgo1(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize1)	\
    ( (This)->lpVtbl -> AB20_NeckRegionLocationAlgo1(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize1) ) 

#define IPlugAlgo_AB20_NeckRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_NeckRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_DownEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV)	\
    ( (This)->lpVtbl -> AB20_DownEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV) ) 

#define IPlugAlgo_AB20_DownEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_DownEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB20_SideSlopRegionLocationAlgo1(This,rgnId,pDilationSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> AB20_SideSlopRegionLocationAlgo1(This,rgnId,pDilationSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_AB20_SideSlopRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_SideSlopRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_TopRegDetectAlgo1(This,rgnId,pMoveSize,pThreshUpValue,pTopRegionArea)	\
    ( (This)->lpVtbl -> AB20_TopRegDetectAlgo1(This,rgnId,pMoveSize,pThreshUpValue,pTopRegionArea) ) 

#define IPlugAlgo_AB20_TopRegDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_TopRegDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_TopRegDetectAlgo2(This,rgnId,pMoveSize,pThreshUpValue,pTopRegionArea)	\
    ( (This)->lpVtbl -> AB20_TopRegDetectAlgo2(This,rgnId,pMoveSize,pThreshUpValue,pTopRegionArea) ) 

#define IPlugAlgo_AB20_TopRegDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_TopRegDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_BarLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pGrayValue,pAreaLimitSize)	\
    ( (This)->lpVtbl -> BarLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pGrayValue,pAreaLimitSize) ) 

#define IPlugAlgo_BarLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BarLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo2(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo2(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo2(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo2(This,rgnId,pStdCrownHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_DiameterMeasureAlgo2(This,rgnId,pStdDiameter,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> DiameterMeasureAlgo2(This,rgnId,pStdDiameter,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_DiameterMeasureAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> DiameterMeasureAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_HeightMeasureAlgo(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> HeightMeasureAlgo(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_HeightMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> HeightMeasureAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_StripePatternRegionLocationAlgo1(This,rgnId,Selection,pHErosionSize,pVErosionSize,pDilationSize)	\
    ( (This)->lpVtbl -> T20_StripePatternRegionLocationAlgo1(This,rgnId,Selection,pHErosionSize,pVErosionSize,pDilationSize) ) 

#define IPlugAlgo_T20_StripePatternRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_StripePatternRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_EmbossCoordAlgo(This,rgnId,pEllipseMoveSize,pEdgeMoveSize,pOpeningSize,pSearchSize,pGrayValue,pWidthRadiusSet,pAngleT1,pStripeArea)	\
    ( (This)->lpVtbl -> G20_EmbossCoordAlgo(This,rgnId,pEllipseMoveSize,pEdgeMoveSize,pOpeningSize,pSearchSize,pGrayValue,pWidthRadiusSet,pAngleT1,pStripeArea) ) 

#define IPlugAlgo_G20_EmbossCoordAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_EmbossCoordAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_EmbossSearchRegAlgo(This,rgnId,pMoveSize,pHDilationSize,pSearchSize,pClipSize)	\
    ( (This)->lpVtbl -> G20_EmbossSearchRegAlgo(This,rgnId,pMoveSize,pHDilationSize,pSearchSize,pClipSize) ) 

#define IPlugAlgo_G20_EmbossSearchRegAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_EmbossSearchRegAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_NeckRegionLocationAlgo2(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pSwitch,pMoveSizeH,pDilationSizeH,pAreaThred,pDilationSize2)	\
    ( (This)->lpVtbl -> G20_NeckRegionLocationAlgo2(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pSwitch,pMoveSizeH,pDilationSizeH,pAreaThred,pDilationSize2) ) 

#define IPlugAlgo_G20_NeckRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_NeckRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_DownEdgeRegionLocationAlgo2(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV)	\
    ( (This)->lpVtbl -> G20_DownEdgeRegionLocationAlgo2(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV) ) 

#define IPlugAlgo_G20_DownEdgeRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_DownEdgeRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo1(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV,pDilationSizeP)	\
    ( (This)->lpVtbl -> G20_MiddleEdgeRegionLocationAlgo1(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV,pDilationSizeP) ) 

#define IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_MiddleEdgeRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_NeckRegionLocationAlgo2(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> AB20_NeckRegionLocationAlgo2(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_AB20_NeckRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_NeckRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_RubberMatScrapEdgeDetectAlgo(This,rgnId,pThredValue,pGrayValue,pMoveSize1,pMoveSize2,pSearchSize,pScrapEdgeArea)	\
    ( (This)->lpVtbl -> RubberMatScrapEdgeDetectAlgo(This,rgnId,pThredValue,pGrayValue,pMoveSize1,pMoveSize2,pSearchSize,pScrapEdgeArea) ) 

#define IPlugAlgo_RubberMatScrapEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RubberMatScrapEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_RubberMatRegionLocationAlgo(This,rgnId,pSelection,pDilationSize,pmoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> RubberMatRegionLocationAlgo(This,rgnId,pSelection,pDilationSize,pmoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_RubberMatRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RubberMatRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_RubberDotRegionLocationAlgo(This,rgnId,pSelection,pThredValue,pRadiusE,pRoiWidth,pGrayValue,pDilationSize)	\
    ( (This)->lpVtbl -> RubberDotRegionLocationAlgo(This,rgnId,pSelection,pThredValue,pRadiusE,pRoiWidth,pGrayValue,pDilationSize) ) 

#define IPlugAlgo_RubberDotRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RubberDotRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IPlugAlgo_AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr) ) 

#define IPlugAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define IPlugAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define IPlugAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define IPlugAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define IPlugAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define IPlugAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define IPlugAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define IPlugAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define IPlugAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define IPlugAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#define IPlugAlgo_DetectSmalCircleAngle(This,rgnId,pRadius,pRoiWidth,pGrayValue,pAreaLimitSize)	\
    ( (This)->lpVtbl -> DetectSmalCircleAngle(This,rgnId,pRadius,pRoiWidth,pGrayValue,pAreaLimitSize) ) 

#define IPlugAlgo_DetectSmalCircleAngleHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectSmalCircleAngleHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB32_InnerWallRegDetectAlgo(This,rgnId,pMoveSize,pThredLower,pTopRegionArea)	\
    ( (This)->lpVtbl -> AB32_InnerWallRegDetectAlgo(This,rgnId,pMoveSize,pThredLower,pTopRegionArea) ) 

#define IPlugAlgo_AB32_InnerWallRegDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_InnerWallRegDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB32_SideSlopRegionLocationAlgo(This,rgnId,pDilationSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> AB32_SideSlopRegionLocationAlgo(This,rgnId,pDilationSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_AB32_SideSlopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_SideSlopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_NeckBaseEdgeDetectAlgo2(This,rgnId,pSearchSize,pHclipSize,pABEdgeArea)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectAlgo2(This,rgnId,pSearchSize,pHclipSize,pABEdgeArea) ) 

#define IPlugAlgo_NeckBaseEdgeDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_DiameterMeasureAlgo3(This,rgnId,pStdDiameter,pPlugType,pMaskSize,pThredLow,pThredUpper,pRectifyValue,pDefectDiff)	\
    ( (This)->lpVtbl -> DiameterMeasureAlgo3(This,rgnId,pStdDiameter,pPlugType,pMaskSize,pThredLow,pThredUpper,pRectifyValue,pDefectDiff) ) 

#define IPlugAlgo_DiameterMeasureAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> DiameterMeasureAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo3(This,rgnId,pStdCrownHeight,pPlugType,pMaskSize,pThredLow,pThredUpper,pRectifyValue,pDefectDiff)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo3(This,rgnId,pStdCrownHeight,pPlugType,pMaskSize,pThredLow,pThredUpper,pRectifyValue,pDefectDiff) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo3(This,rgnId,pStdTotalHeight,pPlugType,pMaskSize,pThredLow,pThredUpper,pGraythresh,pRectifyValue,pDefectDiff)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo3(This,rgnId,pStdTotalHeight,pPlugType,pMaskSize,pThredLow,pThredUpper,pGraythresh,pRectifyValue,pDefectDiff) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_AB32_NeckRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> AB32_NeckRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_AB32_NeckRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_NeckRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_TopDefectDetectAlgo(This,rgnId,pErosionSize,pCErosionSize,pRectSegmntNum,pThreshAdjust,pDefectAreaThresh,pMeanGrayDiffThresh,pMeanGrayNeighborDiff,pDefectArea)	\
    ( (This)->lpVtbl -> T20_TopDefectDetectAlgo(This,rgnId,pErosionSize,pCErosionSize,pRectSegmntNum,pThreshAdjust,pDefectAreaThresh,pMeanGrayDiffThresh,pMeanGrayNeighborDiff,pDefectArea) ) 

#define IPlugAlgo_T20_TopDefectDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_TopDefectDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopRegionLocationAlgo1(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> G20_TopRegionLocationAlgo1(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_G20_TopRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_SideSlopRegionLocationAlgo2(This,rgnId,pEllipseMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> AB20_SideSlopRegionLocationAlgo2(This,rgnId,pEllipseMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_AB20_SideSlopRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_SideSlopRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_FilmOrNotDetectAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AB20_FilmOrNotDetectAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define IPlugAlgo_AB20_FilmOrNotDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_FilmOrNotDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_CountValueNumberAlgo(This,rgnId,pError)	\
    ( (This)->lpVtbl -> CountValueNumberAlgo(This,rgnId,pError) ) 

#define IPlugAlgo_CountValueNumberAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CountValueNumberAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_FilmEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV)	\
    ( (This)->lpVtbl -> FilmEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV) ) 

#define IPlugAlgo_FilmEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> FilmEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DerivateDynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousBlackPointLength,pRowDiffValue,pAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateDynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousBlackPointLength,pRowDiffValue,pAreaThred,pDetectArea) ) 

#define IPlugAlgo_DerivateDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateDynThresholdAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DerivateGaussDetectAlgo1(This,rgnId,pMaskSize,pSmoothSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo1(This,rgnId,pMaskSize,pSmoothSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_DerivateGaussDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_T20_NeckRegionLocationAlgo2(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pDilationSizeH1,pDilationSizeH2,pAreaThred)	\
    ( (This)->lpVtbl -> T20_NeckRegionLocationAlgo2(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pDilationSizeH1,pDilationSizeH2,pAreaThred) ) 

#define IPlugAlgo_T20_NeckRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_NeckRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_LineDetectionAlgo1(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> LineDetectionAlgo1(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectSize,pDefectArea) ) 

#define IPlugAlgo_LineDetectionAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectionAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_FilmBigErrorDetectAlgo(This,rgnId,pMeanSizeWidth,pMeanSizeHeight,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pClosingSize1,pClosingSize2,pAreaLimit,pDetectArea)	\
    ( (This)->lpVtbl -> AB20_FilmBigErrorDetectAlgo(This,rgnId,pMeanSizeWidth,pMeanSizeHeight,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pClosingSize1,pClosingSize2,pAreaLimit,pDetectArea) ) 

#define IPlugAlgo_AB20_FilmBigErrorDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_FilmBigErrorDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BarLocationAlgo2(This,rgnId,pRadius,pRoiWidth,pGrayValue,pAreaLimitSize,pMaskSize,pDynThred)	\
    ( (This)->lpVtbl -> BarLocationAlgo2(This,rgnId,pRadius,pRoiWidth,pGrayValue,pAreaLimitSize,pMaskSize,pDynThred) ) 

#define IPlugAlgo_BarLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BarLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_TopGlueDetectAlgo(This,rgnId,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> AB20_TopGlueDetectAlgo(This,rgnId,pSeriousBlackPointSize,pDetectArea) ) 

#define IPlugAlgo_AB20_TopGlueDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_TopGlueDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BottomDotRegionLocationAlgo1(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pCirleNum,pMinScore,pDilationSize)	\
    ( (This)->lpVtbl -> BottomDotRegionLocationAlgo1(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pCirleNum,pMinScore,pDilationSize) ) 

#define IPlugAlgo_BottomDotRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomDotRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_T20_NeckCornerRegionLocationAlgo(This,rgnId,pNeckErosionSize,pDilSizePlatform,pDilSizePlatform1,pDilSizeTriangle,pUpRegionPos,pDownRegionPos,pOpeningSize)	\
    ( (This)->lpVtbl -> T20_NeckCornerRegionLocationAlgo(This,rgnId,pNeckErosionSize,pDilSizePlatform,pDilSizePlatform1,pDilSizeTriangle,pUpRegionPos,pDownRegionPos,pOpeningSize) ) 

#define IPlugAlgo_T20_NeckCornerRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_NeckCornerRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_StripePatternRegionLocationAlgo2(This,rgnId,Selection,pHErosionSize,pVErosionSize,pDilationSize,pCircleUpRgnPos,pTriangleDownRgnPos)	\
    ( (This)->lpVtbl -> T20_StripePatternRegionLocationAlgo2(This,rgnId,Selection,pHErosionSize,pVErosionSize,pDilationSize,pCircleUpRgnPos,pTriangleDownRgnPos) ) 

#define IPlugAlgo_T20_StripePatternRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_StripePatternRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_TopSealingFaceRegionLocationAlgo3(This,rgnId,pRadius,pErosionSize)	\
    ( (This)->lpVtbl -> TopSealingFaceRegionLocationAlgo3(This,rgnId,pRadius,pErosionSize) ) 

#define IPlugAlgo_TopSealingFaceRegionLocationAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopSealingFaceRegionLocationAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn)	\
    ( (This)->lpVtbl -> GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn) ) 

#define IPlugAlgo_GetInterImgs(This,rgnID,pInterImgs)	\
    ( (This)->lpVtbl -> GetInterImgs(This,rgnID,pInterImgs) ) 

#define IPlugAlgo_T20_TopLessRubberDetectAlgo(This,rgnId,pAreaLimit,pLongLengthLimit,pDetectArea)	\
    ( (This)->lpVtbl -> T20_TopLessRubberDetectAlgo(This,rgnId,pAreaLimit,pLongLengthLimit,pDetectArea) ) 

#define IPlugAlgo_T20_TopLessRubberDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_TopLessRubberDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_T20_NeckRegionLocationAlgo3(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pDilationSizeH1,pDilationSizeH2,pAreaThred)	\
    ( (This)->lpVtbl -> T20_NeckRegionLocationAlgo3(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize0,pDilationSize1,pDilationSizeH1,pDilationSizeH2,pAreaThred) ) 

#define IPlugAlgo_T20_NeckRegionLocationAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_NeckRegionLocationAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_DynThresholdAlgo1(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo1(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_DynThresholdAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_DownEdgeDetectAlgo2(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea)	\
    ( (This)->lpVtbl -> G20_DownEdgeDetectAlgo2(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea) ) 

#define IPlugAlgo_G20_DownEdgeDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_DownEdgeDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_DownEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea)	\
    ( (This)->lpVtbl -> AB20_DownEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pValueThred,pEdgeArea) ) 

#define IPlugAlgo_AB20_DownEdgeDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_DownEdgeDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_NeckBaseEdgeDetectAlgo3(This,rgnId,pSearchSize,pHclipSize,pThreshold,pABEdgeArea)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectAlgo3(This,rgnId,pSearchSize,pHclipSize,pThreshold,pABEdgeArea) ) 

#define IPlugAlgo_NeckBaseEdgeDetectAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_AnnularRotaDynThresholdAlgo1(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pLowValue,pHighValue,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo1(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pLowValue,pHighValue,pDetectArea) ) 

#define IPlugAlgo_AnnularRotaDynThresholdAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_FilmOrNotDetectAlgo1(This,rgnId,pWhiteMask,pDynThresh,pAreaSize,pDetectArea)	\
    ( (This)->lpVtbl -> AB20_FilmOrNotDetectAlgo1(This,rgnId,pWhiteMask,pDynThresh,pAreaSize,pDetectArea) ) 

#define IPlugAlgo_AB20_FilmOrNotDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_FilmOrNotDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo33(This,rgnId,pStdTotalHeight,pPlugType,pMaskSize,pThredLow,pThredUpper,pGraythresh,pRectifyValue,pDefectDiff)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo33(This,rgnId,pStdTotalHeight,pPlugType,pMaskSize,pThredLow,pThredUpper,pGraythresh,pRectifyValue,pDefectDiff) ) 

#define IPlugAlgo_TotalHeightMeasureAlgo33Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> TotalHeightMeasureAlgo33Help(This,pHelpStr) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo33(This,rgnId,pStdCrownHeight,pPlugType,pMaskSize,pThredLow,pThredUpper,pGraythresh,pRectifyValue,pDefectDiff)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo33(This,rgnId,pStdCrownHeight,pPlugType,pMaskSize,pThredLow,pThredUpper,pGraythresh,pRectifyValue,pDefectDiff) ) 

#define IPlugAlgo_CrownHeightMeasureAlgo33Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CrownHeightMeasureAlgo33Help(This,pHelpStr) ) 

#define IPlugAlgo_BottomDotRegionLocationAlgo2(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pCirleNum,pMinScore,pDilationSize)	\
    ( (This)->lpVtbl -> BottomDotRegionLocationAlgo2(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pCirleNum,pMinScore,pDilationSize) ) 

#define IPlugAlgo_BottomDotRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomDotRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_SideSlopRegionLocationAlgo3(This,rgnId,pEllipseMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> AB20_SideSlopRegionLocationAlgo3(This,rgnId,pEllipseMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_AB20_SideSlopRegionLocationAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_SideSlopRegionLocationAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_BottomDotRegionLocationAlgo(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pAngleDiff,pRadiusDiff,pDilationSize)	\
    ( (This)->lpVtbl -> AB20_BottomDotRegionLocationAlgo(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pAngleDiff,pRadiusDiff,pDilationSize) ) 

#define IPlugAlgo_AB20_BottomDotRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_BottomDotRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SealingFaceRegionLocationAlgo2(This,rgnId,pSealDilationSize,pThredValue,pDilationSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> SealingFaceRegionLocationAlgo2(This,rgnId,pSealDilationSize,pThredValue,pDilationSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_SealingFaceRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SealingFaceRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_TopGlueDetectAlgo1(This,rgnId,pClosSize,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> AB20_TopGlueDetectAlgo1(This,rgnId,pClosSize,pSeriousBlackPointSize,pDetectArea) ) 

#define IPlugAlgo_AB20_TopGlueDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_TopGlueDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_TopBaseLineDetectAlgo(This,rgnId,pSelecttion,pMoveSize,pDilSize,pHClipSize,pThreshold,pTopBaseLineArea)	\
    ( (This)->lpVtbl -> TopBaseLineDetectAlgo(This,rgnId,pSelecttion,pMoveSize,pDilSize,pHClipSize,pThreshold,pTopBaseLineArea) ) 

#define IPlugAlgo_TopBaseLineDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopBaseLineDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_TopRegionLocationAlgo(This,rgnId,pSelecttion,pThresholdLow,pOutlierRemoverSize,pErosionSize)	\
    ( (This)->lpVtbl -> TopRegionLocationAlgo(This,rgnId,pSelecttion,pThresholdLow,pOutlierRemoverSize,pErosionSize) ) 

#define IPlugAlgo_TopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SideCenterLocationAlgo1(This,rgnId,pThresholdLow,pOutlierRemoverSize,pErosionSize)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgo1(This,rgnId,pThresholdLow,pOutlierRemoverSize,pErosionSize) ) 

#define IPlugAlgo_SideCenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_LineDetectFromBaseTopAlgo(This,rgnId,pSelecttion,pMoveSize,pDilSize,pHClipSize,pThreshold,pTopBaseLineArea)	\
    ( (This)->lpVtbl -> LineDetectFromBaseTopAlgo(This,rgnId,pSelecttion,pMoveSize,pDilSize,pHClipSize,pThreshold,pTopBaseLineArea) ) 

#define IPlugAlgo_LineDetectFromBaseTopAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectFromBaseTopAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_TopEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pDilSiz,pHClipSize,pErosionSize)	\
    ( (This)->lpVtbl -> TopEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pDilSiz,pHClipSize,pErosionSize) ) 

#define IPlugAlgo_TopEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TopEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_MiddleEdgeRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pDilSiz,pHClipSize,pErosionSize)	\
    ( (This)->lpVtbl -> MiddleEdgeRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pDilSiz,pHClipSize,pErosionSize) ) 

#define IPlugAlgo_MiddleEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MiddleEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_MiddleRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pDilSiz,pHClipSize,pErosionSize)	\
    ( (This)->lpVtbl -> MiddleRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pDilSiz,pHClipSize,pErosionSize) ) 

#define IPlugAlgo_MiddleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MiddleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_LineDetectFromBaseDownAlgo(This,rgnId,pSelecttion,pMoveSize,pDilSize,pHClipSize,pThreshold,pTopBaseLineArea)	\
    ( (This)->lpVtbl -> LineDetectFromBaseDownAlgo(This,rgnId,pSelecttion,pMoveSize,pDilSize,pHClipSize,pThreshold,pTopBaseLineArea) ) 

#define IPlugAlgo_LineDetectFromBaseDownAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectFromBaseDownAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DownEdgeRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pDilSiz,pHClipSize,pErosionSize)	\
    ( (This)->lpVtbl -> DownEdgeRegionLocationAlgo(This,rgnId,pSelection,pMoveSize,pDilSiz,pHClipSize,pErosionSize) ) 

#define IPlugAlgo_DownEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DownEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BottomRegionLocationAlgo(This,rgnId,pMoveSize,pHClipSize,pErosionSize)	\
    ( (This)->lpVtbl -> BottomRegionLocationAlgo(This,rgnId,pMoveSize,pHClipSize,pErosionSize) ) 

#define IPlugAlgo_BottomRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_WholeRegionLocationForHelixAlgo(This,rgnId,pSelection,pErosionSize)	\
    ( (This)->lpVtbl -> WholeRegionLocationForHelixAlgo(This,rgnId,pSelection,pErosionSize) ) 

#define IPlugAlgo_WholeRegionLocationForHelixAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WholeRegionLocationForHelixAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BarLocationAlgo3(This,rgnId,pRadius,pRoiWidth,pAreaLimitSize,pMaskSize,pDynThred)	\
    ( (This)->lpVtbl -> BarLocationAlgo3(This,rgnId,pRadius,pRoiWidth,pAreaLimitSize,pMaskSize,pDynThred) ) 

#define IPlugAlgo_BarLocationAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BarLocationAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_T20_TopCircleLocationAlgo(This,rgnId,pCircleOuter,pCircleWidth,pSmalCircleR,pAngleAdjust,pScaleSize)	\
    ( (This)->lpVtbl -> T20_TopCircleLocationAlgo(This,rgnId,pCircleOuter,pCircleWidth,pSmalCircleR,pAngleAdjust,pScaleSize) ) 

#define IPlugAlgo_T20_TopCircleLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> T20_TopCircleLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DerivateGaussDetectForAnnulusAlgo(This,rgnId,pFeatureToConv,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectForAnnulusAlgo(This,rgnId,pFeatureToConv,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_DerivateGaussDetectForAnnulusAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectForAnnulusAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB32_BottomRegionLocationAlgo(This,rgnId,pUpPosiotion,pDownPosition,pHorizonEro,pScaleSize)	\
    ( (This)->lpVtbl -> AB32_BottomRegionLocationAlgo(This,rgnId,pUpPosiotion,pDownPosition,pHorizonEro,pScaleSize) ) 

#define IPlugAlgo_AB32_BottomRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_BottomRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB32_TopInwallRegionLocationAlgo(This,rgnId,pUpEdgePos,pDownEdgePos,pErosionSize)	\
    ( (This)->lpVtbl -> AB32_TopInwallRegionLocationAlgo(This,rgnId,pUpEdgePos,pDownEdgePos,pErosionSize) ) 

#define IPlugAlgo_AB32_TopInwallRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_TopInwallRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB32_TopHighLightRegionLocationAlgo(This,rgnId,pOuterEdge,pInnerEdge,pBottomSize)	\
    ( (This)->lpVtbl -> AB32_TopHighLightRegionLocationAlgo(This,rgnId,pOuterEdge,pInnerEdge,pBottomSize) ) 

#define IPlugAlgo_AB32_TopHighLightRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_TopHighLightRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DerivateGaussDetectFreeModelAlgo(This,rgnId,pFeatureToConv1,pMaskSize,pFeatureToConv2,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectFreeModelAlgo(This,rgnId,pFeatureToConv1,pMaskSize,pFeatureToConv2,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea) ) 

#define IPlugAlgo_DerivateGaussDetectFreeModelAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectFreeModelAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BubbleDetectProfileForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pMaskDirction,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BubbleDetectProfileForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pMaskDirction,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IPlugAlgo_BubbleDetectProfileForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BubbleDetectProfileForColorAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_ABEdgeRegionLocationAlgo1(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> ABEdgeRegionLocationAlgo1(This,rgnId,pSelection,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_ABEdgeRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> ABEdgeRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB20_NeckRegionLocationAlgo3(This,rgnId,pSelection,pUpSize,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize1)	\
    ( (This)->lpVtbl -> AB20_NeckRegionLocationAlgo3(This,rgnId,pSelection,pUpSize,pMoveSize,pHErosionSize,pVErosionSize,pDilationSize1) ) 

#define IPlugAlgo_AB20_NeckRegionLocationAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB20_NeckRegionLocationAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_AB32_DownEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pThresholdDown,pValueThred,pEdgeArea)	\
    ( (This)->lpVtbl -> AB32_DownEdgeDetectAlgo(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pThresholdDown,pValueThred,pEdgeArea) ) 

#define IPlugAlgo_AB32_DownEdgeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_DownEdgeDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_F20_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pTopEllipseMoveSize,pAreaLimitLower,pLineAngle)	\
    ( (This)->lpVtbl -> F20_SideTopRegAngleDetectAlgo(This,rgnId,pMoveSize,pThredLower,pTopEllipseMoveSize,pAreaLimitLower,pLineAngle) ) 

#define IPlugAlgo_F20_SideTopRegAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> F20_SideTopRegAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_F20_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea)	\
    ( (This)->lpVtbl -> F20_PlatformDetectAlgo(This,rgnId,pMoveSize,pVSearchSize,pHSearchSize,pThredLower,pAngle,pPlatformArea) ) 

#define IPlugAlgo_F20_PlatformDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> F20_PlatformDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_F20_SideTopRegionLocationAlgo(This,rgnId,pRgnSelection,pEroSize)	\
    ( (This)->lpVtbl -> F20_SideTopRegionLocationAlgo(This,rgnId,pRgnSelection,pEroSize) ) 

#define IPlugAlgo_F20_SideTopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> F20_SideTopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_F20_OutInWallRegionLocationAlgo(This,rgnId,pUpSizeOuterWall,pDownSizeOuterWall,pAreaSize,pWholeAreaSize,pHPlatEroSize,pHEroSize)	\
    ( (This)->lpVtbl -> F20_OutInWallRegionLocationAlgo(This,rgnId,pUpSizeOuterWall,pDownSizeOuterWall,pAreaSize,pWholeAreaSize,pHPlatEroSize,pHEroSize) ) 

#define IPlugAlgo_F20_OutInWallRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> F20_OutInWallRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_F20_TopAngleDetectAlgo(This,rgnId,pThreshLowerValue,pOuterEdge,pInnerEdge,pAreaLimitLower,pGapArea)	\
    ( (This)->lpVtbl -> F20_TopAngleDetectAlgo(This,rgnId,pThreshLowerValue,pOuterEdge,pInnerEdge,pAreaLimitLower,pGapArea) ) 

#define IPlugAlgo_F20_TopAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> F20_TopAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_RegionLocationByModelTool1(This,rgnId,pRadius,pRoiWidth,bstrRgnFile)	\
    ( (This)->lpVtbl -> RegionLocationByModelTool1(This,rgnId,pRadius,pRoiWidth,bstrRgnFile) ) 

#define IPlugAlgo_RegionLocationByModelTool1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> RegionLocationByModelTool1Help(This,pHelpStr) ) 

#define IPlugAlgo_F20_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pRoiWidth,pThredLower,pBlackThred,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> F20_TopRegionLocationAlgo(This,rgnId,pSelection,pRadius,pRoiWidth,pThredLower,pBlackThred,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_F20_TopRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> F20_TopRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_F20_StripePatternDetectAlgo(This,rgnId,pSwitch,pMoveSize,pHSearchSize,pVSearchSize,pWhiteThred,pDilationSize,pStripeArea)	\
    ( (This)->lpVtbl -> F20_StripePatternDetectAlgo(This,rgnId,pSwitch,pMoveSize,pHSearchSize,pVSearchSize,pWhiteThred,pDilationSize,pStripeArea) ) 

#define IPlugAlgo_F20_StripePatternDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> F20_StripePatternDetectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_SideTopHighLightRegionLocationAlgo(This,rgnId,pOuterEdge,pMoveSize)	\
    ( (This)->lpVtbl -> SideTopHighLightRegionLocationAlgo(This,rgnId,pOuterEdge,pMoveSize) ) 

#define IPlugAlgo_SideTopHighLightRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideTopHighLightRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BottomCharacterLocationAlgo(This,rgnId,pRadius,pRoiWidth,pMaskSize,pThredValue,pCharacterArea)	\
    ( (This)->lpVtbl -> BottomCharacterLocationAlgo(This,rgnId,pRadius,pRoiWidth,pMaskSize,pThredValue,pCharacterArea) ) 

#define IPlugAlgo_BottomCharacterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomCharacterLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BottomCharacterRegionAlgo(This,rgnID,pOuterEdge,pInnerEdge,pEroSize)	\
    ( (This)->lpVtbl -> BottomCharacterRegionAlgo(This,rgnID,pOuterEdge,pInnerEdge,pEroSize) ) 

#define IPlugAlgo_BottomCharacterRegionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomCharacterRegionAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB32_DownEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pThresholdDown,pSamePointsThred,pValueThred,pEdgeArea)	\
    ( (This)->lpVtbl -> AB32_DownEdgeDetectAlgo1(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pThresholdDown,pSamePointsThred,pValueThred,pEdgeArea) ) 

#define IPlugAlgo_AB32_DownEdgeDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_DownEdgeDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_ABEdgeRegionLocationAlgo2(This,rgnId,pSelection,pMoveSize,pMoveDiff,pIntsctThred,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> ABEdgeRegionLocationAlgo2(This,rgnId,pSelection,pMoveSize,pMoveDiff,pIntsctThred,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_ABEdgeRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> ABEdgeRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_ABEdgeRegionLocationAlgo3(This,rgnId,pSelection,pGrayThred,pMoveSize,pMoveDiff,pIntsctThred,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> ABEdgeRegionLocationAlgo3(This,rgnId,pSelection,pGrayThred,pMoveSize,pMoveDiff,pIntsctThred,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_ABEdgeRegionLocationAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> ABEdgeRegionLocationAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_NeckBaseEdgeDetectFreeModelAlgo(This,rgnId,pMoveSize,pSearchSize,pHclipSize,pSearchDirec,pSelect,pThreshold,pABEdgeArea)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectFreeModelAlgo(This,rgnId,pMoveSize,pSearchSize,pHclipSize,pSearchDirec,pSelect,pThreshold,pABEdgeArea) ) 

#define IPlugAlgo_NeckBaseEdgeDetectFreeModelAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> NeckBaseEdgeDetectFreeModelAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB32_TopHighLightRegionLocationAlgo1(This,rgnId,pOuterEdge,pInnerEdge,pBottomSize,pUpPose,pDownPose)	\
    ( (This)->lpVtbl -> AB32_TopHighLightRegionLocationAlgo1(This,rgnId,pOuterEdge,pInnerEdge,pBottomSize,pUpPose,pDownPose) ) 

#define IPlugAlgo_AB32_TopHighLightRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_TopHighLightRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB32_BottomBorderRegionLocationAlgo(This,rgnId,pLowThred,pUpEdgeSize,pDownEdgeSize,pHClipSize)	\
    ( (This)->lpVtbl -> AB32_BottomBorderRegionLocationAlgo(This,rgnId,pLowThred,pUpEdgeSize,pDownEdgeSize,pHClipSize) ) 

#define IPlugAlgo_AB32_BottomBorderRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_BottomBorderRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DoubleFoldDetectAlgo1(This,rgnId,pAreaLimit,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea)	\
    ( (This)->lpVtbl -> DoubleFoldDetectAlgo1(This,rgnId,pAreaLimit,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea) ) 

#define IPlugAlgo_DoubleFoldDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> DoubleFoldDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_AB32_DownEdgeDetectAlgo2(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pThresholdDown,pSamePointsThred,pValueThred,pEdgeArea)	\
    ( (This)->lpVtbl -> AB32_DownEdgeDetectAlgo2(This,rgnId,pMoveSize,pSearchSize,pHClipSize,pThreshold,pThresholdDown,pSamePointsThred,pValueThred,pEdgeArea) ) 

#define IPlugAlgo_AB32_DownEdgeDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_DownEdgeDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_SidePlatformRegionLocationAlgo1(This,rgnId,pABEdgeMov,pThredLow,pClosingSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> SidePlatformRegionLocationAlgo1(This,rgnId,pABEdgeMov,pThredLow,pClosingSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_SidePlatformRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SidePlatformRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_SideNeckBorderRegionLocationAlgo(This,rgnId,pSelection,pInnerBorderSize,pOuterBorderSize,pUpBoderSize,pDownBoderSize,pStripeScale,pPlatformSize,pAreaSize)	\
    ( (This)->lpVtbl -> SideNeckBorderRegionLocationAlgo(This,rgnId,pSelection,pInnerBorderSize,pOuterBorderSize,pUpBoderSize,pDownBoderSize,pStripeScale,pPlatformSize,pAreaSize) ) 

#define IPlugAlgo_SideNeckBorderRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideNeckBorderRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_D20_SideInwallRegionLocationAlgo(This,rgnId,pBaseLineMoveSize,pROIScaleSize,pTopRgnScaleSize,pSlopScaleSize,pPlatformSize,pInnerEdgeSize,pOuterEndgeSize,pLeftUpEdgeSize,pAreaSize)	\
    ( (This)->lpVtbl -> D20_SideInwallRegionLocationAlgo(This,rgnId,pBaseLineMoveSize,pROIScaleSize,pTopRgnScaleSize,pSlopScaleSize,pPlatformSize,pInnerEdgeSize,pOuterEndgeSize,pLeftUpEdgeSize,pAreaSize) ) 

#define IPlugAlgo_D20_SideInwallRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> D20_SideInwallRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_TotalHeightMeasureInjectorAlgo(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff)	\
    ( (This)->lpVtbl -> TotalHeightMeasureInjectorAlgo(This,rgnId,pStdTotalHeight,pMaskSize,pThredLow,pThredUpper,pDefectDiff) ) 

#define IPlugAlgo_TotalHeightMeasureInjectorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TotalHeightMeasureInjectorAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_WarpedMembraneDetectAlgo1(This,rgnId,pNumPart,pSensitivity,pGrayLowThred,pDetectArea)	\
    ( (This)->lpVtbl -> WarpedMembraneDetectAlgo1(This,rgnId,pNumPart,pSensitivity,pGrayLowThred,pDetectArea) ) 

#define IPlugAlgo_WarpedMembraneDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> WarpedMembraneDetectAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_SideTopSectorRegionLocationAlgo(This,rgnId,pSelection,pdUpPos,pdDownPos,pAngleStart,pAngleEnd,pOuterSize,pInnerSize)	\
    ( (This)->lpVtbl -> SideTopSectorRegionLocationAlgo(This,rgnId,pSelection,pdUpPos,pdDownPos,pAngleStart,pAngleEnd,pOuterSize,pInnerSize) ) 

#define IPlugAlgo_SideTopSectorRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideTopSectorRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AngleSelectAlgo(This,rgnId,pSelection)	\
    ( (This)->lpVtbl -> AngleSelectAlgo(This,rgnId,pSelection) ) 

#define IPlugAlgo_AngleSelectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AngleSelectAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BottomCharacterLocationAlgo1(This,rgnId,pSelection,pRadius,pRoiWidth,pMaskSize,pThredValue,pCharacterArea)	\
    ( (This)->lpVtbl -> BottomCharacterLocationAlgo1(This,rgnId,pSelection,pRadius,pRoiWidth,pMaskSize,pThredValue,pCharacterArea) ) 

#define IPlugAlgo_BottomCharacterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomCharacterLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_BottomDotRegionLocationAlgo3(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pCirleNum,pMinScore,pDilationSize)	\
    ( (This)->lpVtbl -> BottomDotRegionLocationAlgo3(This,rgnId,pSelection,pRadiusE,pRoiWidth,pRadius,pCirleNum,pMinScore,pDilationSize) ) 

#define IPlugAlgo_BottomDotRegionLocationAlgo3Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomDotRegionLocationAlgo3Help(This,pHelpStr) ) 

#define IPlugAlgo_LineDetectionAlgo2(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectShape,pDefectSize,pDefectArea)	\
    ( (This)->lpVtbl -> LineDetectionAlgo2(This,rgnId,pMaskSize,pThredLow,pThredUpper,pDefectType,pDefectShape,pDefectSize,pDefectArea) ) 

#define IPlugAlgo_LineDetectionAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> LineDetectionAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_DerivateGaussDetectForAnnulusAlgo1(This,rgnId,pFeatureToConv,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDefectShape,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectForAnnulusAlgo1(This,rgnId,pFeatureToConv,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDefectShape,pDetectArea) ) 

#define IPlugAlgo_DerivateGaussDetectForAnnulusAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectForAnnulusAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_BubbleDetectProfileForColorFilmAlgo(This,rgnId,pColorTransType,pChannelSelect,pMaskDirction,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea)	\
    ( (This)->lpVtbl -> BubbleDetectProfileForColorFilmAlgo(This,rgnId,pColorTransType,pChannelSelect,pMaskDirction,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea) ) 

#define IPlugAlgo_BubbleDetectProfileForColorFilmAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BubbleDetectProfileForColorFilmAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_DynThresholdForColorFilmAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdForColorFilmAlgo(This,rgnId,pColorTransType,pChannelSelect,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pMoveSize,pDetectArea) ) 

#define IPlugAlgo_DynThresholdForColorFilmAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdForColorFilmAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_AB32_BottomEdgeRegionLocationAlgo(This,rgnId,pSelection,pUpEdgeSize,pDownEdgeSize,pHClipSize,pScaleSize)	\
    ( (This)->lpVtbl -> AB32_BottomEdgeRegionLocationAlgo(This,rgnId,pSelection,pUpEdgeSize,pDownEdgeSize,pHClipSize,pScaleSize) ) 

#define IPlugAlgo_AB32_BottomEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AB32_BottomEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_FilmEdgeRegionLocationAlgo1(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV)	\
    ( (This)->lpVtbl -> FilmEdgeRegionLocationAlgo1(This,rgnId,pMoveSize,pHErosionSize,pDialtionSizeV) ) 

#define IPlugAlgo_FilmEdgeRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> FilmEdgeRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_InWallDynThresholdAlgo1(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pUpEdgeExclude,pDownEdgeExclude,pDetectArea)	\
    ( (This)->lpVtbl -> InWallDynThresholdAlgo1(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pUpEdgeExclude,pDownEdgeExclude,pDetectArea) ) 

#define IPlugAlgo_InWallDynThresholdAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> InWallDynThresholdAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_SealingEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize)	\
    ( (This)->lpVtbl -> SealingEdgeRegionLocationAlgo(This,rgnId,pMoveSize,pHErosionSize,pVErosionSize) ) 

#define IPlugAlgo_SealingEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SealingEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopCornerLocationAlgo(This,rgnId,pSlopSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> G20_TopCornerLocationAlgo(This,rgnId,pSlopSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_G20_TopCornerLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopCornerLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopForkEdgeRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pDilationSize,pRelativeWidth,pRelativeLength)	\
    ( (This)->lpVtbl -> G20_TopForkEdgeRegionLocationAlgo(This,rgnId,pInnerEdge,pOutEdge,pDilationSize,pRelativeWidth,pRelativeLength) ) 

#define IPlugAlgo_G20_TopForkEdgeRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopForkEdgeRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopHLightLocationAlgo(This,rgnId,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge)	\
    ( (This)->lpVtbl -> G20_TopHLightLocationAlgo(This,rgnId,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge) ) 

#define IPlugAlgo_G20_TopHLightLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopHLightLocationAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BubbleDetectProfileForAnnulusAlgo(This,rgnId,pMaskDirection,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDefectShape,pDetectArea)	\
    ( (This)->lpVtbl -> BubbleDetectProfileForAnnulusAlgo(This,rgnId,pMaskDirection,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDefectShape,pDetectArea) ) 

#define IPlugAlgo_BubbleDetectProfileForAnnulusAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BubbleDetectProfileForAnnulusAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_G20_TopRegionLocationAlgo2(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge)	\
    ( (This)->lpVtbl -> G20_TopRegionLocationAlgo2(This,rgnId,pSelection,pRadius,pThredLower,pAreaLimitLower,pErosionSize,pForkEdge,pInnerEdge,pOutEdge) ) 

#define IPlugAlgo_G20_TopRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_TopRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_InWallRegionLocationAlgo1(This,rgnId,pGrayValue,pDilationSizeU,pDilationSizeD,pErosionSize,pForkSize)	\
    ( (This)->lpVtbl -> G20_InWallRegionLocationAlgo1(This,rgnId,pGrayValue,pDilationSizeU,pDilationSizeD,pErosionSize,pForkSize) ) 

#define IPlugAlgo_G20_InWallRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_InWallRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IPlugAlgo_GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IPlugAlgo_GLineAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_ScrapEdgeDetectAlgo2(This,rgnId,pMoveSize,pSearchSize,pScrapEdgeArea)	\
    ( (This)->lpVtbl -> ScrapEdgeDetectAlgo2(This,rgnId,pMoveSize,pSearchSize,pScrapEdgeArea) ) 

#define IPlugAlgo_ScrapEdgeDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> ScrapEdgeDetectAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_G20_InWallRegionLocationAlgo2(This,rgnId,pAngleSelect,pGrayValue,pDilationSizeU,pDilationSizeD,pErosionSize,pForkSize,pForkWallSize)	\
    ( (This)->lpVtbl -> G20_InWallRegionLocationAlgo2(This,rgnId,pAngleSelect,pGrayValue,pDilationSizeU,pDilationSizeD,pErosionSize,pForkSize,pForkWallSize) ) 

#define IPlugAlgo_G20_InWallRegionLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> G20_InWallRegionLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_CenterLocationRightOrNotAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CenterLocationRightOrNotAlgo(This,rgnId,pGrayValueMean) ) 

#define IPlugAlgo_CenterLocationRightOrNotAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationRightOrNotAlgoHelp(This,pHelpStr) ) 

#define IPlugAlgo_BottomCharacterLocationAlgo2(This,rgnId,pAngleSelect,pRadius,pRoiWidth,pCharNum,pMaskSize,pThredValue,pSelArea,pCharacterArea)	\
    ( (This)->lpVtbl -> BottomCharacterLocationAlgo2(This,rgnId,pAngleSelect,pRadius,pRoiWidth,pCharNum,pMaskSize,pThredValue,pSelArea,pCharacterArea) ) 

#define IPlugAlgo_BottomCharacterLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BottomCharacterLocationAlgo2Help(This,pHelpStr) ) 

#define IPlugAlgo_SideCenterLocationAlgoForColor(This,rgnId,pColorTransType,pChannelSelect,pRowBgn,pRgnWidth,pRgnHeight,pThresholdLow,pOutlierRemoverSize,pErosionSize)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgoForColor(This,rgnId,pColorTransType,pChannelSelect,pRowBgn,pRgnWidth,pRgnHeight,pThresholdLow,pOutlierRemoverSize,pErosionSize) ) 

#define IPlugAlgo_SideCenterLocationAlgoForColorHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgoForColorHelp(This,pHelpStr) ) 

#define IPlugAlgo_AnnularRotaDynThresholdForColorAlgoBW(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgoBW(This,rgnId,pColorTransType,pChannelSelect,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IPlugAlgo_AnnularRotaDynThresholdForColorAlgoBWHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgoBWHelp(This,pHelpStr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SidePlatformRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pClosingSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_SidePlatformRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SidePlatformRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SidePlatformRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_WholeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pUpEdgeMoveSize,
    /* [in] */ VARIANT *pABHErosionSize,
    /* [in] */ VARIANT *pABVErosionSize);


void __RPC_STUB IPlugAlgo_D20_WholeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_WholeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_WholeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_StripePatternRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_D20_StripePatternRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_StripePatternRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_StripePatternRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_TopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_D20_TopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_TopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_TopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_WholeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pABHErosionSize,
    /* [in] */ VARIANT *pABVErosionSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_AB20_WholeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_WholeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_WholeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopInwallRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pOpeningSize,
    /* [in] */ VARIANT *pClosingSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_AB20_TopInwallRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopInwallRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_TopInwallRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_WholeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pDilationSizeP,
    /* [in] */ VARIANT *pDilationSizeT,
    /* [in] */ VARIANT *pOpeningSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_D13_WholeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_WholeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_WholeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TriangleRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_D13_TriangleRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TriangleRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TriangleRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopAngleDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThreshUpValue,
    /* [in] */ VARIANT *pBlackThresh,
    /* [retval][out] */ VARIANT *pGapArea);


void __RPC_STUB IPlugAlgo_D13_TopAngleDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopAngleDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopAngleDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_D13_TopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_N_DynThresholdAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_N_DynThresholdAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_N_DynThresholdAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_N_DynThresholdAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckBaseEdgeDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHclipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pABEdgeArea);


void __RPC_STUB IPlugAlgo_NeckBaseEdgeDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckBaseEdgeDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_NeckBaseEdgeDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_SideTopRegAngleDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredLower,
    /* [retval][out] */ VARIANT *pLineAngle);


void __RPC_STUB IPlugAlgo_G20_SideTopRegAngleDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_SideTopRegAngleDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_SideTopRegAngleDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_PlatformDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pVSearchSize,
    /* [in] */ VARIANT *pHSearchSize,
    /* [in] */ VARIANT *pThredLower,
    /* [retval][out] */ VARIANT *pPlatformArea);


void __RPC_STUB IPlugAlgo_G20_PlatformDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_PlatformDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_PlatformDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_SlopEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [retval][out] */ VARIANT *pTopSlopEdgeArea);


void __RPC_STUB IPlugAlgo_G20_SlopEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_SlopEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_SlopEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_InWallRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pDilationSizeU,
    /* [in] */ VARIANT *pDilationSizeD,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_G20_InWallRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_InWallRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_InWallRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_MiddleEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_G20_MiddleEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_MiddleEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_MiddleEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pValueThred,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_G20_DownEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_DownEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAreaLimit,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pOpeningSize,
    /* [in] */ VARIANT *pDilationSizeH,
    /* [in] */ VARIANT *pDialtionSizeV,
    /* [in] */ VARIANT *pErosionSizeH,
    /* [in] */ VARIANT *pErosionSizeV,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_G20_ConPatternDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_ConPatternDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_NeckRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize0,
    /* [in] */ VARIANT *pDilationSize1,
    /* [in] */ VARIANT *pSwitch,
    /* [in] */ VARIANT *pMoveSizeH,
    /* [in] */ VARIANT *pAreaThred,
    /* [in] */ VARIANT *pDilationSize2);


void __RPC_STUB IPlugAlgo_G20_NeckRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_NeckRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_NeckRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_N_DerivateGaussDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pSmoothSize,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_N_DerivateGaussDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_N_DerivateGaussDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_N_DerivateGaussDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_WholeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pUpEdgeMoveSize,
    /* [in] */ VARIANT *pDialtionSizeP,
    /* [in] */ VARIANT *pABHErosionSize,
    /* [in] */ VARIANT *pABVErosionSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_G20_WholeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_WholeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_WholeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopAngleDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshValue,
    /* [retval][out] */ VARIANT *pGapAngle);


void __RPC_STUB IPlugAlgo_G20_TopAngleDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopAngleDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopAngleDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_G20_TopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomDotRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadiusE,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pCirleNum,
    /* [in] */ VARIANT *pMinScore,
    /* [in] */ VARIANT *pDilationSize);


void __RPC_STUB IPlugAlgo_BottomDotRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomDotRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomDotRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CircleRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth);


void __RPC_STUB IPlugAlgo_CircleRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CircleRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CircleRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_InWallRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayThred,
    /* [in] */ VARIANT *pPos,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_D13_InWallRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_InWallRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_InWallRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pUpEdgeMoveSize,
    /* [in] */ VARIANT *pLowerEdgeMoveSize,
    /* [in] */ VARIANT *pHErosionSize);


void __RPC_STUB IPlugAlgo_NeckRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_NeckRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopSealingFaceRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pGrayThred,
    /* [in] */ VARIANT *pClosingSize,
    /* [in] */ VARIANT *pRadiusDiff,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_TopSealingFaceRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopSealingFaceRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TopSealingFaceRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_DerivateGaussDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pSmoothSize,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pDilationSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_D13_DerivateGaussDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_DerivateGaussDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_DerivateGaussDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_InWallDynThresholdAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_InWallDynThresholdAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_InWallDynThresholdAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_InWallDynThresholdAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdTotalHeight,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pGraythresh,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdCrownHeight,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pGraythresh,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ForkTotalHeightMeasureAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdTotalHeight,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pGraythresh,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_ForkTotalHeightMeasureAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ForkTotalHeightMeasureAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_ForkTotalHeightMeasureAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureSAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdCrownHeight,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pGraythresh,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureSAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureSAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureSAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ScrapDiameterMeasureAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSigma,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pCircularityValue,
    /* [retval][out] */ VARIANT *pDiameter);


void __RPC_STUB IPlugAlgo_ScrapDiameterMeasureAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ScrapDiameterMeasureAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_ScrapDiameterMeasureAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CenterLocationAlgoM_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRowStart,
    /* [in] */ VARIANT *pColStart,
    /* [in] */ VARIANT *pRegWidth,
    /* [in] */ VARIANT *pRegHeight,
    /* [in] */ VARIANT *pThresholdUp);


void __RPC_STUB IPlugAlgo_CenterLocationAlgoM_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CenterLocationAlgoMHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CenterLocationAlgoMHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SealingFaceRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_SealingFaceRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SealingFaceRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SealingFaceRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRubberRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge,
    /* [in] */ VARIANT *pAreaLimitLower);


void __RPC_STUB IPlugAlgo_D13_TopRubberRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRubberRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopRubberRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideTopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pClosingSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_D13_SideTopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideTopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_SideTopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_SideTopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_D20_SideTopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_SideTopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_SideTopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_RubberDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pSmoothSize,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pOpeningSize,
    /* [in] */ VARIANT *pDilationSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_D20_RubberDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_RubberDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_RubberDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_SlopEdgeDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pTopSlopEdgeArea);


void __RPC_STUB IPlugAlgo_D20_SlopEdgeDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_SlopEdgeDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_SlopEdgeDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_NeckRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLowerEdgeMoveSize,
    /* [in] */ VARIANT *pUpEdgeMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilationSizeH1,
    /* [in] */ VARIANT *pVDilationSizeH2,
    /* [in] */ VARIANT *pHDilationSizeH2,
    /* [in] */ VARIANT *pDilationSizeT);


void __RPC_STUB IPlugAlgo_D20_NeckRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_NeckRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_NeckRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_GSurfaceAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackThred,
    /* [in] */ VARIANT *pBlackAreaThred,
    /* [in] */ VARIANT *pWhiteThred,
    /* [in] */ VARIANT *pWhiteAreaThred,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_GSurfaceAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_GSurfaceAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_GSurfaceAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_TopCenterDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshUpValue,
    /* [retval][out] */ VARIANT *pGapArea);


void __RPC_STUB IPlugAlgo_D20_TopCenterDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_TopCenterDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_TopCenterDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_TopRubberRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge,
    /* [in] */ VARIANT *pAreaLimitLower);


void __RPC_STUB IPlugAlgo_D20_TopRubberRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_TopRubberRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_TopRubberRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopSealingFaceRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_TopSealingFaceRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopSealingFaceRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TopSealingFaceRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WarpedMembraneDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSensitivity,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_WarpedMembraneDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WarpedMembraneDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_WarpedMembraneDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternCoordAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pEllipseMoveSize,
    /* [in] */ VARIANT *pEdgeMoveSize,
    /* [in] */ VARIANT *pOpeningSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pGrayValue,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_G20_ConPatternCoordAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternCoordAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_ConPatternCoordAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pHErosionSize1,
    /* [in] */ VARIANT *pHErosionSize2,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_G20_ConPatternRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_ConPatternRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pDialtionSizeV);


void __RPC_STUB IPlugAlgo_G20_DownEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_DownEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_SideSlopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLowerEdgeMoveSize,
    /* [in] */ VARIANT *pUpEdgeMoveSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pHErosionSize);


void __RPC_STUB IPlugAlgo_G20_SideSlopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_SideSlopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_SideSlopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pmoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_CrownRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CrownRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SlotDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pNumPart,
    /* [in] */ VARIANT *pThresholdValue,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pMeanGrayValue,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_SlotDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SlotDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SlotDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_OutEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pDilationSize1,
    /* [in] */ VARIANT *pDilationSize2);


void __RPC_STUB IPlugAlgo_G20_OutEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_OutEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_OutEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DynThresholdForColorAlgo_Proxy( 
    IPlugAlgo * This,
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


void __RPC_STUB IPlugAlgo_DynThresholdForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DynThresholdForColorAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DynThresholdForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_EdgeForColorAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pEdgeThred,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_EdgeForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_EdgeForColorAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_EdgeForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AnnularRotaDynThresholdForColorAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AnnularRotaDynThresholdForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AnnularRotaDynThresholdForColorAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AnnularRotaDynThresholdForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopSealingFaceEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThresh,
    /* [in] */ VARIANT *pOpeningSize,
    /* [retval][out] */ VARIANT *pGapArea);


void __RPC_STUB IPlugAlgo_G20_TopSealingFaceEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopSealingFaceEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopSealingFaceEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternSearchRegAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHDilationSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pClipSizeD,
    /* [in] */ VARIANT *pClipSizeU);


void __RPC_STUB IPlugAlgo_G20_ConPatternSearchRegAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternSearchRegAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_ConPatternSearchRegAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternDynThresholdAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAreaLimit,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_G20_ConPatternDynThresholdAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternDynThresholdAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_ConPatternDynThresholdAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pDialtionSizeV,
    /* [in] */ VARIANT *pDilationSizeP);


void __RPC_STUB IPlugAlgo_G20_DownEdgeRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_DownEdgeRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_N_BubbleDetectProfileAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_N_BubbleDetectProfileAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_N_BubbleDetectProfileAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_N_BubbleDetectProfileAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_N_LineDetectionAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IPlugAlgo_N_LineDetectionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_N_LineDetectionAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_N_LineDetectionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopSideLowRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge,
    /* [in] */ VARIANT *pDilationSize);


void __RPC_STUB IPlugAlgo_G20_TopSideLowRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopSideLowRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopSideLowRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DoubleFoldDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAreaLimit,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DoubleFoldDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DoubleFoldDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DoubleFoldDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pDialtionSizeV);


void __RPC_STUB IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_MiddleEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_MiddleEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternDynThresholdAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAreaLimit,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_G20_ConPatternDynThresholdAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternDynThresholdAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_ConPatternDynThresholdAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_CrownCircleRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRadius1,
    /* [in] */ VARIANT *pRoiWidth1,
    /* [in] */ VARIANT *pRadius2,
    /* [in] */ VARIANT *pRoiWidth2);


void __RPC_STUB IPlugAlgo_AB20_CrownCircleRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_CrownCircleRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_CrownCircleRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RegionLocationDiffByModelTool_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ BSTR *bstrRgnFile);


void __RPC_STUB IPlugAlgo_RegionLocationDiffByModelTool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RegionLocationDiffByModelToolHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_RegionLocationDiffByModelToolHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WhiteDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pWhiteThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_WhiteDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WhiteDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_WhiteDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_MinMaxGrayDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_MinMaxGrayDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_MinMaxGrayDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_MinMaxGrayDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownCenterRelocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThreshLowValue,
    /* [in] */ VARIANT *pAreaLimitSize);


void __RPC_STUB IPlugAlgo_CrownCenterRelocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownCenterRelocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CrownCenterRelocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopAngleDetectAlgo11_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThreshUpValue,
    /* [in] */ VARIANT *pBlackThresh,
    /* [in] */ VARIANT *pThreshLow,
    /* [retval][out] */ VARIANT *pGapArea);


void __RPC_STUB IPlugAlgo_D13_TopAngleDetectAlgo11_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopAngleDetectAlgo11Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopAngleDetectAlgo11Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_D13_TopRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_PlatformDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pVSearchSize,
    /* [in] */ VARIANT *pHSearchSize,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAngle,
    /* [retval][out] */ VARIANT *pPlatformArea);


void __RPC_STUB IPlugAlgo_D13_PlatformDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_PlatformDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_PlatformDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideSlopRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_D13_SideSlopRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideSlopRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_SideSlopRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideTopRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pClosingSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_D13_SideTopRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideTopRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_SideTopRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TriangleDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pWhiteThred,
    /* [in] */ VARIANT *pBlackThred,
    /* [in] */ VARIANT *pTriangleSize,
    /* [in] */ VARIANT *pTopRegArea,
    /* [in] */ VARIANT *pTriangleDist,
    /* [retval][out] */ VARIANT *pTriangleArea);


void __RPC_STUB IPlugAlgo_D13_TriangleDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TriangleDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TriangleDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopAngleDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pBlackThresh,
    /* [in] */ VARIANT *pThreshUpValue,
    /* [retval][out] */ VARIANT *pGapArea);


void __RPC_STUB IPlugAlgo_D13_TopAngleDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopAngleDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopAngleDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopCenterDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackThresh,
    /* [retval][out] */ VARIANT *pGapArea);


void __RPC_STUB IPlugAlgo_D13_TopCenterDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopCenterDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopCenterDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_D13_TopRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRubberRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge,
    /* [in] */ VARIANT *pAreaLimitLower);


void __RPC_STUB IPlugAlgo_D13_TopRubberRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_TopRubberRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_TopRubberRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopSealingFaceRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pThresh,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_TopSealingFaceRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopSealingFaceRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TopSealingFaceRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideTopRegAngleDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredLower,
    /* [retval][out] */ VARIANT *pLineAngle);


void __RPC_STUB IPlugAlgo_D13_SideTopRegAngleDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideTopRegAngleDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_SideTopRegAngleDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideSlopRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pBlackThred,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_D13_SideSlopRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideSlopRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_SideSlopRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_NeckRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLowerEdgeMoveSize,
    /* [in] */ VARIANT *pUpEdgeMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilationSizeH1,
    /* [in] */ VARIANT *pVDilationSizeH2,
    /* [in] */ VARIANT *pHDilationSizeH2,
    /* [in] */ VARIANT *pDilationSizeT);


void __RPC_STUB IPlugAlgo_D13_NeckRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_NeckRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_NeckRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_PlatformDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pVSearchSize,
    /* [in] */ VARIANT *pHSearchSize,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAngle,
    /* [retval][out] */ VARIANT *pPlatformArea);


void __RPC_STUB IPlugAlgo_D13_PlatformDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_PlatformDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_PlatformDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_InWallRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayThred,
    /* [in] */ VARIANT *pBlackThred,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_D13_InWallRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_InWallRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_InWallRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideTopRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_D13_SideTopRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_SideTopRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_SideTopRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_StripePatternDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSwitch,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pWhiteThred,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_D13_StripePatternDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D13_StripePatternDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D13_StripePatternDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_SideTopRegAngleDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [retval][out] */ VARIANT *pLineAngle);


void __RPC_STUB IPlugAlgo_T20_SideTopRegAngleDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_SideTopRegAngleDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_SideTopRegAngleDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_PlatformDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pVSearchSize,
    /* [in] */ VARIANT *pHSearchSize,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pOpeningSize,
    /* [in] */ VARIANT *pAngle,
    /* [retval][out] */ VARIANT *pPlatformArea);


void __RPC_STUB IPlugAlgo_T20_PlatformDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_PlatformDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_PlatformDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_SideConeBiasDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pConeDeepth,
    /* [retval][out] */ VARIANT *pConeArea);


void __RPC_STUB IPlugAlgo_T20_SideConeBiasDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_SideConeBiasDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_SideConeBiasDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_SlopEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pTopSlopEdgeArea);


void __RPC_STUB IPlugAlgo_T20_SlopEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_SlopEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_SlopEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_ConPatternCoordAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pEllipseMoveSize,
    /* [in] */ VARIANT *pEdgeMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pWidthRadiusSet,
    /* [in] */ VARIANT *pAngleT1,
    /* [in] */ VARIANT *pAngleT2,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_T20_ConPatternCoordAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_ConPatternCoordAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_ConPatternCoordAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_ConPatternSearchRegUpAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHDilationSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pClipSize);


void __RPC_STUB IPlugAlgo_T20_ConPatternSearchRegUpAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_ConPatternSearchRegUpAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_ConPatternSearchRegUpAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_CirclePatternDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pThredValue,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_T20_CirclePatternDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_CirclePatternDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_CirclePatternDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_ConPatternSearchRegDownAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize);


void __RPC_STUB IPlugAlgo_T20_ConPatternSearchRegDownAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_ConPatternSearchRegDownAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_ConPatternSearchRegDownAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TrianglePatternDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pTriangleSize,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_T20_TrianglePatternDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TrianglePatternDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_TrianglePatternDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_MiddleEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_T20_MiddleEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_MiddleEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_MiddleEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize0,
    /* [in] */ VARIANT *pDilationSize1,
    /* [in] */ VARIANT *pDilationSizeH1,
    /* [in] */ VARIANT *pDilationSizeH2);


void __RPC_STUB IPlugAlgo_T20_NeckRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_NeckRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_InWallRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pPos,
    /* [in] */ VARIANT *pGrayThred,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_T20_InWallRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_InWallRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_InWallRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_SideTopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SideTopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WholeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pABHErosionSize,
    /* [in] */ VARIANT *pABVErosionSize,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_WholeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WholeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_WholeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopAngleDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshValue,
    /* [retval][out] */ VARIANT *pGapAngle);


void __RPC_STUB IPlugAlgo_T20_TopAngleDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopAngleDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_TopAngleDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopCenterDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshUpValue,
    /* [retval][out] */ VARIANT *pGapArea);


void __RPC_STUB IPlugAlgo_TopCenterDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopCenterDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TopCenterDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_T20_TopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_TopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_SideSlopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_T20_SideSlopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_SideSlopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_SideSlopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayValue,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_T20_StripePatternDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_StripePatternDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_CirclePatternDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pThredValue,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_T20_CirclePatternDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_CirclePatternDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_CirclePatternDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_WholeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pABHErosionSize,
    /* [in] */ VARIANT *pABVErosionSize,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pDilationSize0,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_T20_WholeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_WholeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_WholeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopRubberRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge,
    /* [in] */ VARIANT *pAreaLimitLower);


void __RPC_STUB IPlugAlgo_T20_TopRubberRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopRubberRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_TopRubberRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_T20_StripePatternRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_StripePatternRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ABEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_G20_ABEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ABEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_ABEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternDetectProfileAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pClipSize,
    /* [in] */ VARIANT *pAreaLimit,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_G20_ConPatternDetectProfileAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_ConPatternDetectProfileAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_ConPatternDetectProfileAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_SideTopRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SideTopRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ABEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_ABEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ABEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_ABEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_SideTopEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SideTopEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CurlNoCompoundForColorAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [retval][out] */ VARIANT *pGrayValueMean);


void __RPC_STUB IPlugAlgo_CurlNoCompoundForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CurlNoCompoundForColorAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CurlNoCompoundForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_TopForkRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge,
    /* [in] */ VARIANT *pAreaLimitUp);


void __RPC_STUB IPlugAlgo_D20_TopForkRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_TopForkRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_TopForkRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_StripePatternDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSwitch,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pWhiteThred,
    /* [in] */ VARIANT *pDilationSize,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_D20_StripePatternDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_StripePatternDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_StripePatternDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopForkRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pThreshUpValue,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_T20_TopForkRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopForkRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_TopForkRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pDilationSize,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_T20_StripePatternDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_StripePatternDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize0,
    /* [in] */ VARIANT *pDilationSize1,
    /* [in] */ VARIANT *pDilationSizeH1,
    /* [in] */ VARIANT *pDilationSizeH2);


void __RPC_STUB IPlugAlgo_T20_NeckRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_NeckRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pValueThred,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_G20_DownEdgeDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_DownEdgeDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_NeckRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize0,
    /* [in] */ VARIANT *pDilationSize1,
    /* [in] */ VARIANT *pSwitch,
    /* [in] */ VARIANT *pMoveSizeH,
    /* [in] */ VARIANT *pAreaThred,
    /* [in] */ VARIANT *pDilationSize2);


void __RPC_STUB IPlugAlgo_G20_NeckRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_NeckRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_NeckRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_DownEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pValueThred,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_AB20_DownEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_DownEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_DownEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ScrapEdgeDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [retval][out] */ VARIANT *pScrapEdgeArea);


void __RPC_STUB IPlugAlgo_ScrapEdgeDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ScrapEdgeDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_ScrapEdgeDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_NeckRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize1);


void __RPC_STUB IPlugAlgo_AB20_NeckRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_NeckRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_NeckRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_DownEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pDialtionSizeV);


void __RPC_STUB IPlugAlgo_AB20_DownEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_DownEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_DownEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_SideSlopRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_AB20_SideSlopRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_SideSlopRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_SideSlopRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopRegDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThreshUpValue,
    /* [retval][out] */ VARIANT *pTopRegionArea);


void __RPC_STUB IPlugAlgo_AB20_TopRegDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopRegDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_TopRegDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopRegDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThreshUpValue,
    /* [retval][out] */ VARIANT *pTopRegionArea);


void __RPC_STUB IPlugAlgo_AB20_TopRegDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopRegDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_TopRegDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BarLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pAreaLimitSize);


void __RPC_STUB IPlugAlgo_BarLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BarLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BarLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdTotalHeight,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdCrownHeight,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DiameterMeasureAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdDiameter,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_DiameterMeasureAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DiameterMeasureAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DiameterMeasureAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_HeightMeasureAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdTotalHeight,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_HeightMeasureAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_HeightMeasureAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_HeightMeasureAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *Selection,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize);


void __RPC_STUB IPlugAlgo_T20_StripePatternRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_StripePatternRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_EmbossCoordAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pEllipseMoveSize,
    /* [in] */ VARIANT *pEdgeMoveSize,
    /* [in] */ VARIANT *pOpeningSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pWidthRadiusSet,
    /* [in] */ VARIANT *pAngleT1,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_G20_EmbossCoordAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_EmbossCoordAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_EmbossCoordAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_EmbossSearchRegAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHDilationSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pClipSize);


void __RPC_STUB IPlugAlgo_G20_EmbossSearchRegAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_EmbossSearchRegAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_EmbossSearchRegAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_NeckRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize0,
    /* [in] */ VARIANT *pDilationSize1,
    /* [in] */ VARIANT *pSwitch,
    /* [in] */ VARIANT *pMoveSizeH,
    /* [in] */ VARIANT *pDilationSizeH,
    /* [in] */ VARIANT *pAreaThred,
    /* [in] */ VARIANT *pDilationSize2);


void __RPC_STUB IPlugAlgo_G20_NeckRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_NeckRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_NeckRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pDialtionSizeV);


void __RPC_STUB IPlugAlgo_G20_DownEdgeRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_DownEdgeRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pDialtionSizeV,
    /* [in] */ VARIANT *pDilationSizeP);


void __RPC_STUB IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_MiddleEdgeRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_NeckRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_AB20_NeckRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_NeckRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_NeckRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RubberMatScrapEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pMoveSize1,
    /* [in] */ VARIANT *pMoveSize2,
    /* [in] */ VARIANT *pSearchSize,
    /* [retval][out] */ VARIANT *pScrapEdgeArea);


void __RPC_STUB IPlugAlgo_RubberMatScrapEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RubberMatScrapEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_RubberMatScrapEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RubberMatRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pmoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_RubberMatRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RubberMatRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_RubberMatRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RubberDotRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pRadiusE,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pDilationSize);


void __RPC_STUB IPlugAlgo_RubberDotRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RubberDotRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_RubberDotRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AnnularRotaDynThresholdAlgoBW_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pBlackThredLow,
    /* [in] */ VARIANT *pBlackDefectSize,
    /* [in] */ VARIANT *pWhiteThredLow,
    /* [in] */ VARIANT *pWhiteDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AnnularRotaDynThresholdAlgoBW_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AnnularRotaDynThresholdAlgoBWHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AnnularRotaDynThresholdAlgoBWHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SetSelfLearningParam_Proxy( 
    IPlugAlgo * This,
    /* [in] */ DWORD sampleSz,
    /* [in] */ BSTR *bstrParam);


void __RPC_STUB IPlugAlgo_SetSelfLearningParam_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SelectModelImage_Proxy( 
    IPlugAlgo * This,
    /* [in] */ LONG *pCrtImg,
    /* [in] */ BYTE productID);


void __RPC_STUB IPlugAlgo_SelectModelImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DeleteModelImage_Proxy( 
    IPlugAlgo * This,
    /* [in] */ BYTE nIndex,
    /* [in] */ BYTE productID);


void __RPC_STUB IPlugAlgo_DeleteModelImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SaveResult_Proxy( 
    IPlugAlgo * This,
    /* [in] */ BSTR *strModelName,
    /* [in] */ BYTE productID);


void __RPC_STUB IPlugAlgo_SaveResult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AddModelImage_Proxy( 
    IPlugAlgo * This,
    /* [in] */ LONG *pImg,
    /* [in] */ BSTR *strFolder,
    /* [in] */ BYTE productID);


void __RPC_STUB IPlugAlgo_AddModelImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_GetSelfLearningResult_Proxy( 
    IPlugAlgo * This,
    /* [out][in] */ LONG *multiModelImag,
    /* [out][in] */ long *cMax,
    /* [out][in] */ float *vImgScore,
    /* [in] */ BYTE productID);


void __RPC_STUB IPlugAlgo_GetSelfLearningResult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ResetSelfLearningRegion_Proxy( 
    IPlugAlgo * This,
    /* [in] */ BSTR *strRegionExtractPara);


void __RPC_STUB IPlugAlgo_ResetSelfLearningRegion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_GetSelfLearningRegion_Proxy( 
    IPlugAlgo * This,
    /* [out][in] */ LONG *pRgn);


void __RPC_STUB IPlugAlgo_GetSelfLearningRegion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ResetSelfLearningProcedure_Proxy( 
    IPlugAlgo * This);


void __RPC_STUB IPlugAlgo_ResetSelfLearningProcedure_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SetSelfLearningTargetTaskName_Proxy( 
    IPlugAlgo * This,
    /* [in] */ BSTR *bstrTaskName);


void __RPC_STUB IPlugAlgo_SetSelfLearningTargetTaskName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DetectSmalCircleAngle_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pAreaLimitSize);


void __RPC_STUB IPlugAlgo_DetectSmalCircleAngle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DetectSmalCircleAngleHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DetectSmalCircleAngleHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_InnerWallRegDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredLower,
    /* [retval][out] */ VARIANT *pTopRegionArea);


void __RPC_STUB IPlugAlgo_AB32_InnerWallRegDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_InnerWallRegDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_InnerWallRegDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_SideSlopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_AB32_SideSlopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_SideSlopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_SideSlopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckBaseEdgeDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHclipSize,
    /* [retval][out] */ VARIANT *pABEdgeArea);


void __RPC_STUB IPlugAlgo_NeckBaseEdgeDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckBaseEdgeDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_NeckBaseEdgeDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DiameterMeasureAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdDiameter,
    /* [in] */ VARIANT *pPlugType,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pRectifyValue,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_DiameterMeasureAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DiameterMeasureAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DiameterMeasureAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdCrownHeight,
    /* [in] */ VARIANT *pPlugType,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pRectifyValue,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdTotalHeight,
    /* [in] */ VARIANT *pPlugType,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pGraythresh,
    /* [in] */ VARIANT *pRectifyValue,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_NeckRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_AB32_NeckRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_NeckRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_NeckRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopDefectDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pCErosionSize,
    /* [in] */ VARIANT *pRectSegmntNum,
    /* [in] */ VARIANT *pThreshAdjust,
    /* [in] */ VARIANT *pDefectAreaThresh,
    /* [in] */ VARIANT *pMeanGrayDiffThresh,
    /* [in] */ VARIANT *pMeanGrayNeighborDiff,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IPlugAlgo_T20_TopDefectDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopDefectDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_TopDefectDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_G20_TopRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_SideSlopRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pEllipseMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_AB20_SideSlopRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_SideSlopRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_SideSlopRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_FilmOrNotDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AB20_FilmOrNotDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_FilmOrNotDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_FilmOrNotDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CountValueNumberAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [retval][out] */ VARIANT *pError);


void __RPC_STUB IPlugAlgo_CountValueNumberAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CountValueNumberAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CountValueNumberAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_FilmEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pDialtionSizeV);


void __RPC_STUB IPlugAlgo_FilmEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_FilmEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_FilmEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateDynThresholdAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pSeriousBlackPointLength,
    /* [in] */ VARIANT *pRowDiffValue,
    /* [in] */ VARIANT *pAreaThred,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DerivateDynThresholdAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateDynThresholdAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DerivateDynThresholdAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateGaussDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pSmoothSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DerivateGaussDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateGaussDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DerivateGaussDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize0,
    /* [in] */ VARIANT *pDilationSize1,
    /* [in] */ VARIANT *pDilationSizeH1,
    /* [in] */ VARIANT *pDilationSizeH2,
    /* [in] */ VARIANT *pAreaThred);


void __RPC_STUB IPlugAlgo_T20_NeckRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_NeckRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_LineDetectionAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IPlugAlgo_LineDetectionAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_LineDetectionAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_LineDetectionAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_FilmBigErrorDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMeanSizeWidth,
    /* [in] */ VARIANT *pMeanSizeHeight,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pClosingSize1,
    /* [in] */ VARIANT *pClosingSize2,
    /* [in] */ VARIANT *pAreaLimit,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AB20_FilmBigErrorDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_FilmBigErrorDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_FilmBigErrorDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BarLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pAreaLimitSize,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pDynThred);


void __RPC_STUB IPlugAlgo_BarLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BarLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BarLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopGlueDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AB20_TopGlueDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopGlueDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_TopGlueDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomDotRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadiusE,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pCirleNum,
    /* [in] */ VARIANT *pMinScore,
    /* [in] */ VARIANT *pDilationSize);


void __RPC_STUB IPlugAlgo_BottomDotRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomDotRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomDotRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckCornerRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pNeckErosionSize,
    /* [in] */ VARIANT *pDilSizePlatform,
    /* [in] */ VARIANT *pDilSizePlatform1,
    /* [in] */ VARIANT *pDilSizeTriangle,
    /* [in] */ VARIANT *pUpRegionPos,
    /* [in] */ VARIANT *pDownRegionPos,
    /* [in] */ VARIANT *pOpeningSize);


void __RPC_STUB IPlugAlgo_T20_NeckCornerRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckCornerRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_NeckCornerRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *Selection,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pCircleUpRgnPos,
    /* [in] */ VARIANT *pTriangleDownRgnPos);


void __RPC_STUB IPlugAlgo_T20_StripePatternRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_StripePatternRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_StripePatternRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopSealingFaceRegionLocationAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_TopSealingFaceRegionLocationAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopSealingFaceRegionLocationAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TopSealingFaceRegionLocationAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_GetWhiteErrorRegion_Proxy( 
    IPlugAlgo * This,
    /* [in] */ BYTE rgnID,
    /* [out][in] */ LONG *pWhiteErrorRgn);


void __RPC_STUB IPlugAlgo_GetWhiteErrorRegion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_GetInterImgs_Proxy( 
    IPlugAlgo * This,
    /* [in] */ BYTE rgnID,
    /* [out][in] */ LONG *pInterImgs);


void __RPC_STUB IPlugAlgo_GetInterImgs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopLessRubberDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAreaLimit,
    /* [in] */ VARIANT *pLongLengthLimit,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_T20_TopLessRubberDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopLessRubberDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_TopLessRubberDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckRegionLocationAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize0,
    /* [in] */ VARIANT *pDilationSize1,
    /* [in] */ VARIANT *pDilationSizeH1,
    /* [in] */ VARIANT *pDilationSizeH2,
    /* [in] */ VARIANT *pAreaThred);


void __RPC_STUB IPlugAlgo_T20_NeckRegionLocationAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_NeckRegionLocationAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_NeckRegionLocationAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DynThresholdAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DynThresholdAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DynThresholdAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DynThresholdAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pValueThred,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_G20_DownEdgeDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_DownEdgeDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_DownEdgeDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_DownEdgeDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pValueThred,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_AB20_DownEdgeDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_DownEdgeDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_DownEdgeDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckBaseEdgeDetectAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHclipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pABEdgeArea);


void __RPC_STUB IPlugAlgo_NeckBaseEdgeDetectAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckBaseEdgeDetectAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_NeckBaseEdgeDetectAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AnnularRotaDynThresholdAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [in] */ VARIANT *pLowValue,
    /* [in] */ VARIANT *pHighValue,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AnnularRotaDynThresholdAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AnnularRotaDynThresholdAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AnnularRotaDynThresholdAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_FilmOrNotDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pWhiteMask,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pAreaSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AB20_FilmOrNotDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_FilmOrNotDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_FilmOrNotDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureAlgo33_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdTotalHeight,
    /* [in] */ VARIANT *pPlugType,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pGraythresh,
    /* [in] */ VARIANT *pRectifyValue,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureAlgo33_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureAlgo33Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureAlgo33Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureAlgo33_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdCrownHeight,
    /* [in] */ VARIANT *pPlugType,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pGraythresh,
    /* [in] */ VARIANT *pRectifyValue,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureAlgo33_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CrownHeightMeasureAlgo33Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CrownHeightMeasureAlgo33Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomDotRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadiusE,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pCirleNum,
    /* [in] */ VARIANT *pMinScore,
    /* [in] */ VARIANT *pDilationSize);


void __RPC_STUB IPlugAlgo_BottomDotRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomDotRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomDotRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_SideSlopRegionLocationAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pEllipseMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_AB20_SideSlopRegionLocationAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_SideSlopRegionLocationAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_SideSlopRegionLocationAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_BottomDotRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadiusE,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pAngleDiff,
    /* [in] */ VARIANT *pRadiusDiff,
    /* [in] */ VARIANT *pDilationSize);


void __RPC_STUB IPlugAlgo_AB20_BottomDotRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_BottomDotRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_BottomDotRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SealingFaceRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSealDilationSize,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_SealingFaceRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SealingFaceRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SealingFaceRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopGlueDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pClosSize,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AB20_TopGlueDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_TopGlueDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_TopGlueDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopBaseLineDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelecttion,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pTopBaseLineArea);


void __RPC_STUB IPlugAlgo_TopBaseLineDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopBaseLineDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TopBaseLineDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelecttion,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pOutlierRemoverSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_TopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideCenterLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pOutlierRemoverSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_SideCenterLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideCenterLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SideCenterLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_LineDetectFromBaseTopAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelecttion,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pTopBaseLineArea);


void __RPC_STUB IPlugAlgo_LineDetectFromBaseTopAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_LineDetectFromBaseTopAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_LineDetectFromBaseTopAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilSiz,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_TopEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TopEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TopEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_MiddleEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilSiz,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_MiddleEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_MiddleEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_MiddleEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_MiddleRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilSiz,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_MiddleRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_MiddleRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_MiddleRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_LineDetectFromBaseDownAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelecttion,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pTopBaseLineArea);


void __RPC_STUB IPlugAlgo_LineDetectFromBaseDownAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_LineDetectFromBaseDownAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_LineDetectFromBaseDownAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DownEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pDilSiz,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_DownEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DownEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DownEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_BottomRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WholeRegionLocationForHelixAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_WholeRegionLocationForHelixAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WholeRegionLocationForHelixAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_WholeRegionLocationForHelixAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BarLocationAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pAreaLimitSize,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pDynThred);


void __RPC_STUB IPlugAlgo_BarLocationAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BarLocationAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BarLocationAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopCircleLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pCircleOuter,
    /* [in] */ VARIANT *pCircleWidth,
    /* [in] */ VARIANT *pSmalCircleR,
    /* [in] */ VARIANT *pAngleAdjust,
    /* [in] */ VARIANT *pScaleSize);


void __RPC_STUB IPlugAlgo_T20_TopCircleLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_T20_TopCircleLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_T20_TopCircleLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateGaussDetectForAnnulusAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pFeatureToConv,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pSmoothSize,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DerivateGaussDetectForAnnulusAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateGaussDetectForAnnulusAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DerivateGaussDetectForAnnulusAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_BottomRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pUpPosiotion,
    /* [in] */ VARIANT *pDownPosition,
    /* [in] */ VARIANT *pHorizonEro,
    /* [in] */ VARIANT *pScaleSize);


void __RPC_STUB IPlugAlgo_AB32_BottomRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_BottomRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_BottomRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_TopInwallRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pUpEdgePos,
    /* [in] */ VARIANT *pDownEdgePos,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_AB32_TopInwallRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_TopInwallRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_TopInwallRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_TopHighLightRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pOuterEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pBottomSize);


void __RPC_STUB IPlugAlgo_AB32_TopHighLightRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_TopHighLightRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_TopHighLightRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateGaussDetectFreeModelAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pFeatureToConv1,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pFeatureToConv2,
    /* [in] */ VARIANT *pSmoothSize,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DerivateGaussDetectFreeModelAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateGaussDetectFreeModelAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DerivateGaussDetectFreeModelAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BubbleDetectProfileForColorAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pMaskDirction,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_BubbleDetectProfileForColorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BubbleDetectProfileForColorAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BubbleDetectProfileForColorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ABEdgeRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_ABEdgeRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ABEdgeRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_ABEdgeRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_NeckRegionLocationAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pUpSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize,
    /* [in] */ VARIANT *pDilationSize1);


void __RPC_STUB IPlugAlgo_AB20_NeckRegionLocationAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB20_NeckRegionLocationAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB20_NeckRegionLocationAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_DownEdgeDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pThresholdDown,
    /* [in] */ VARIANT *pValueThred,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_AB32_DownEdgeDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_DownEdgeDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_DownEdgeDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_SideTopRegAngleDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pTopEllipseMoveSize,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [retval][out] */ VARIANT *pLineAngle);


void __RPC_STUB IPlugAlgo_F20_SideTopRegAngleDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_SideTopRegAngleDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_F20_SideTopRegAngleDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_PlatformDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pVSearchSize,
    /* [in] */ VARIANT *pHSearchSize,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAngle,
    /* [retval][out] */ VARIANT *pPlatformArea);


void __RPC_STUB IPlugAlgo_F20_PlatformDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_PlatformDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_F20_PlatformDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_SideTopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRgnSelection,
    /* [in] */ VARIANT *pEroSize);


void __RPC_STUB IPlugAlgo_F20_SideTopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_SideTopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_F20_SideTopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_OutInWallRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pUpSizeOuterWall,
    /* [in] */ VARIANT *pDownSizeOuterWall,
    /* [in] */ VARIANT *pAreaSize,
    /* [in] */ VARIANT *pWholeAreaSize,
    /* [in] */ VARIANT *pHPlatEroSize,
    /* [in] */ VARIANT *pHEroSize);


void __RPC_STUB IPlugAlgo_F20_OutInWallRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_OutInWallRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_F20_OutInWallRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_TopAngleDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThreshLowerValue,
    /* [in] */ VARIANT *pOuterEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [retval][out] */ VARIANT *pGapArea);


void __RPC_STUB IPlugAlgo_F20_TopAngleDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_TopAngleDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_F20_TopAngleDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RegionLocationByModelTool1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ BSTR *bstrRgnFile);


void __RPC_STUB IPlugAlgo_RegionLocationByModelTool1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_RegionLocationByModelTool1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_RegionLocationByModelTool1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_TopRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pBlackThred,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_F20_TopRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_TopRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_F20_TopRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_StripePatternDetectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSwitch,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHSearchSize,
    /* [in] */ VARIANT *pVSearchSize,
    /* [in] */ VARIANT *pWhiteThred,
    /* [in] */ VARIANT *pDilationSize,
    /* [retval][out] */ VARIANT *pStripeArea);


void __RPC_STUB IPlugAlgo_F20_StripePatternDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_F20_StripePatternDetectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_F20_StripePatternDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopHighLightRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pOuterEdge,
    /* [in] */ VARIANT *pMoveSize);


void __RPC_STUB IPlugAlgo_SideTopHighLightRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopHighLightRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SideTopHighLightRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomCharacterLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredValue,
    /* [retval][out] */ VARIANT *pCharacterArea);


void __RPC_STUB IPlugAlgo_BottomCharacterLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomCharacterLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomCharacterLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomCharacterRegionAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnID,
    /* [in] */ VARIANT *pOuterEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pEroSize);


void __RPC_STUB IPlugAlgo_BottomCharacterRegionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomCharacterRegionAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomCharacterRegionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_DownEdgeDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pThresholdDown,
    /* [in] */ VARIANT *pSamePointsThred,
    /* [in] */ VARIANT *pValueThred,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_AB32_DownEdgeDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_DownEdgeDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_DownEdgeDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ABEdgeRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pMoveDiff,
    /* [in] */ VARIANT *pIntsctThred,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_ABEdgeRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ABEdgeRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_ABEdgeRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ABEdgeRegionLocationAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pGrayThred,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pMoveDiff,
    /* [in] */ VARIANT *pIntsctThred,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_ABEdgeRegionLocationAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ABEdgeRegionLocationAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_ABEdgeRegionLocationAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckBaseEdgeDetectFreeModelAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHclipSize,
    /* [in] */ VARIANT *pSearchDirec,
    /* [in] */ VARIANT *pSelect,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pABEdgeArea);


void __RPC_STUB IPlugAlgo_NeckBaseEdgeDetectFreeModelAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_NeckBaseEdgeDetectFreeModelAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_NeckBaseEdgeDetectFreeModelAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_TopHighLightRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pOuterEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pBottomSize,
    /* [in] */ VARIANT *pUpPose,
    /* [in] */ VARIANT *pDownPose);


void __RPC_STUB IPlugAlgo_AB32_TopHighLightRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_TopHighLightRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_TopHighLightRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_BottomBorderRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pLowThred,
    /* [in] */ VARIANT *pUpEdgeSize,
    /* [in] */ VARIANT *pDownEdgeSize,
    /* [in] */ VARIANT *pHClipSize);


void __RPC_STUB IPlugAlgo_AB32_BottomBorderRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_BottomBorderRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_BottomBorderRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DoubleFoldDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAreaLimit,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DoubleFoldDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DoubleFoldDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DoubleFoldDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_DownEdgeDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pThresholdDown,
    /* [in] */ VARIANT *pSamePointsThred,
    /* [in] */ VARIANT *pValueThred,
    /* [retval][out] */ VARIANT *pEdgeArea);


void __RPC_STUB IPlugAlgo_AB32_DownEdgeDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_DownEdgeDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_DownEdgeDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SidePlatformRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pABEdgeMov,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pClosingSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_SidePlatformRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SidePlatformRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SidePlatformRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideNeckBorderRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pInnerBorderSize,
    /* [in] */ VARIANT *pOuterBorderSize,
    /* [in] */ VARIANT *pUpBoderSize,
    /* [in] */ VARIANT *pDownBoderSize,
    /* [in] */ VARIANT *pStripeScale,
    /* [in] */ VARIANT *pPlatformSize,
    /* [in] */ VARIANT *pAreaSize);


void __RPC_STUB IPlugAlgo_SideNeckBorderRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideNeckBorderRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SideNeckBorderRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_SideInwallRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBaseLineMoveSize,
    /* [in] */ VARIANT *pROIScaleSize,
    /* [in] */ VARIANT *pTopRgnScaleSize,
    /* [in] */ VARIANT *pSlopScaleSize,
    /* [in] */ VARIANT *pPlatformSize,
    /* [in] */ VARIANT *pInnerEdgeSize,
    /* [in] */ VARIANT *pOuterEndgeSize,
    /* [in] */ VARIANT *pLeftUpEdgeSize,
    /* [in] */ VARIANT *pAreaSize);


void __RPC_STUB IPlugAlgo_D20_SideInwallRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_D20_SideInwallRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_D20_SideInwallRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureInjectorAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pStdTotalHeight,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureInjectorAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_TotalHeightMeasureInjectorAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_TotalHeightMeasureInjectorAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WarpedMembraneDetectAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pNumPart,
    /* [in] */ VARIANT *pSensitivity,
    /* [in] */ VARIANT *pGrayLowThred,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_WarpedMembraneDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_WarpedMembraneDetectAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_WarpedMembraneDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopSectorRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pdUpPos,
    /* [in] */ VARIANT *pdDownPos,
    /* [in] */ VARIANT *pAngleStart,
    /* [in] */ VARIANT *pAngleEnd,
    /* [in] */ VARIANT *pOuterSize,
    /* [in] */ VARIANT *pInnerSize);


void __RPC_STUB IPlugAlgo_SideTopSectorRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideTopSectorRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SideTopSectorRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AngleSelectAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection);


void __RPC_STUB IPlugAlgo_AngleSelectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AngleSelectAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AngleSelectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomCharacterLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredValue,
    /* [retval][out] */ VARIANT *pCharacterArea);


void __RPC_STUB IPlugAlgo_BottomCharacterLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomCharacterLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomCharacterLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomDotRegionLocationAlgo3_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadiusE,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pCirleNum,
    /* [in] */ VARIANT *pMinScore,
    /* [in] */ VARIANT *pDilationSize);


void __RPC_STUB IPlugAlgo_BottomDotRegionLocationAlgo3_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomDotRegionLocationAlgo3Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomDotRegionLocationAlgo3Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_LineDetectionAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pThredUpper,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pDefectShape,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB IPlugAlgo_LineDetectionAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_LineDetectionAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_LineDetectionAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateGaussDetectForAnnulusAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pFeatureToConv,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pSmoothSize,
    /* [in] */ VARIANT *pDefectType,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pDefectShape,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DerivateGaussDetectForAnnulusAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DerivateGaussDetectForAnnulusAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DerivateGaussDetectForAnnulusAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BubbleDetectProfileForColorFilmAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pMaskDirction,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_BubbleDetectProfileForColorFilmAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BubbleDetectProfileForColorFilmAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BubbleDetectProfileForColorFilmAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DynThresholdForColorFilmAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pMoveSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_DynThresholdForColorFilmAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_DynThresholdForColorFilmAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_DynThresholdForColorFilmAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_BottomEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pUpEdgeSize,
    /* [in] */ VARIANT *pDownEdgeSize,
    /* [in] */ VARIANT *pHClipSize,
    /* [in] */ VARIANT *pScaleSize);


void __RPC_STUB IPlugAlgo_AB32_BottomEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AB32_BottomEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AB32_BottomEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_FilmEdgeRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pDialtionSizeV);


void __RPC_STUB IPlugAlgo_FilmEdgeRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_FilmEdgeRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_FilmEdgeRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_InWallDynThresholdAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pUpEdgeExclude,
    /* [in] */ VARIANT *pDownEdgeExclude,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_InWallDynThresholdAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_InWallDynThresholdAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_InWallDynThresholdAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SealingEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pHErosionSize,
    /* [in] */ VARIANT *pVErosionSize);


void __RPC_STUB IPlugAlgo_SealingEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SealingEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SealingEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopCornerLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSlopSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_G20_TopCornerLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopCornerLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopCornerLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopForkEdgeRegionLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge,
    /* [in] */ VARIANT *pDilationSize,
    /* [in] */ VARIANT *pRelativeWidth,
    /* [in] */ VARIANT *pRelativeLength);


void __RPC_STUB IPlugAlgo_G20_TopForkEdgeRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopForkEdgeRegionLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopForkEdgeRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopHLightLocationAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge);


void __RPC_STUB IPlugAlgo_G20_TopHLightLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopHLightLocationAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopHLightLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BubbleDetectProfileForAnnulusAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskDirection,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
    /* [in] */ VARIANT *pSeriousWhitePointSize,
    /* [in] */ VARIANT *pDefectShape,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_BubbleDetectProfileForAnnulusAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BubbleDetectProfileForAnnulusAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BubbleDetectProfileForAnnulusAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSelection,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pThredLower,
    /* [in] */ VARIANT *pAreaLimitLower,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkEdge,
    /* [in] */ VARIANT *pInnerEdge,
    /* [in] */ VARIANT *pOutEdge);


void __RPC_STUB IPlugAlgo_G20_TopRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_TopRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_TopRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_InWallRegionLocationAlgo1_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pDilationSizeU,
    /* [in] */ VARIANT *pDilationSizeD,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkSize);


void __RPC_STUB IPlugAlgo_G20_InWallRegionLocationAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_InWallRegionLocationAlgo1Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_InWallRegionLocationAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_GLineAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pBlackThredLow,
    /* [in] */ VARIANT *pBlackDefectSize,
    /* [in] */ VARIANT *pWhiteMaskSize,
    /* [in] */ VARIANT *pWhiteThredLow,
    /* [in] */ VARIANT *pWhiteDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_GLineAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_GLineAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_GLineAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ScrapEdgeDetectAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pSearchSize,
    /* [retval][out] */ VARIANT *pScrapEdgeArea);


void __RPC_STUB IPlugAlgo_ScrapEdgeDetectAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_ScrapEdgeDetectAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_ScrapEdgeDetectAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_InWallRegionLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAngleSelect,
    /* [in] */ VARIANT *pGrayValue,
    /* [in] */ VARIANT *pDilationSizeU,
    /* [in] */ VARIANT *pDilationSizeD,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ VARIANT *pForkSize,
    /* [in] */ VARIANT *pForkWallSize);


void __RPC_STUB IPlugAlgo_G20_InWallRegionLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_G20_InWallRegionLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_G20_InWallRegionLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CenterLocationRightOrNotAlgo_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [retval][out] */ VARIANT *pGrayValueMean);


void __RPC_STUB IPlugAlgo_CenterLocationRightOrNotAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_CenterLocationRightOrNotAlgoHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_CenterLocationRightOrNotAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomCharacterLocationAlgo2_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pAngleSelect,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pCharNum,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pSelArea,
    /* [retval][out] */ VARIANT *pCharacterArea);


void __RPC_STUB IPlugAlgo_BottomCharacterLocationAlgo2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_BottomCharacterLocationAlgo2Help_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_BottomCharacterLocationAlgo2Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideCenterLocationAlgoForColor_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRowBgn,
    /* [in] */ VARIANT *pRgnWidth,
    /* [in] */ VARIANT *pRgnHeight,
    /* [in] */ VARIANT *pThresholdLow,
    /* [in] */ VARIANT *pOutlierRemoverSize,
    /* [in] */ VARIANT *pErosionSize);


void __RPC_STUB IPlugAlgo_SideCenterLocationAlgoForColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_SideCenterLocationAlgoForColorHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_SideCenterLocationAlgoForColorHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AnnularRotaDynThresholdForColorAlgoBW_Proxy( 
    IPlugAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pColorTransType,
    /* [in] */ VARIANT *pChannelSelect,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pBlackThredLow,
    /* [in] */ VARIANT *pBlackDefectSize,
    /* [in] */ VARIANT *pWhiteThredLow,
    /* [in] */ VARIANT *pWhiteDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IPlugAlgo_AnnularRotaDynThresholdForColorAlgoBW_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IPlugAlgo_AnnularRotaDynThresholdForColorAlgoBWHelp_Proxy( 
    IPlugAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IPlugAlgo_AnnularRotaDynThresholdForColorAlgoBWHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPlugAlgo_INTERFACE_DEFINED__ */


#ifndef __ISelfLearning_INTERFACE_DEFINED__
#define __ISelfLearning_INTERFACE_DEFINED__

/* interface ISelfLearning */
/* [unique][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_ISelfLearning;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FFACCB7D-68BF-4C20-9C56-0EDBDDE73BFD")
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



#ifndef __PlugAlgoLib_LIBRARY_DEFINED__
#define __PlugAlgoLib_LIBRARY_DEFINED__

/* library PlugAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_PlugAlgoLib;

EXTERN_C const CLSID CLSID_PlugAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("C2AF78CC-9CC8-466B-9B55-3EB019E27700")
PlugAlgo;
#endif
#endif /* __PlugAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


