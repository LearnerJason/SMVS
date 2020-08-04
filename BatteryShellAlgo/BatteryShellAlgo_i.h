

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Nov 21 15:21:28 2019
 */
/* Compiler settings for BatteryShellAlgo.idl:
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

#ifndef __BatteryShellAlgo_i_h__
#define __BatteryShellAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBatteryShellAlgo_FWD_DEFINED__
#define __IBatteryShellAlgo_FWD_DEFINED__
typedef interface IBatteryShellAlgo IBatteryShellAlgo;
#endif 	/* __IBatteryShellAlgo_FWD_DEFINED__ */


#ifndef __BatteryShellAlgo_FWD_DEFINED__
#define __BatteryShellAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class BatteryShellAlgo BatteryShellAlgo;
#else
typedef struct BatteryShellAlgo BatteryShellAlgo;
#endif /* __cplusplus */

#endif 	/* __BatteryShellAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IBatteryShellAlgo_INTERFACE_DEFINED__
#define __IBatteryShellAlgo_INTERFACE_DEFINED__

/* interface IBatteryShellAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IBatteryShellAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2216931D-BC85-4A14-8F4A-3E874C618D87")
    IBatteryShellAlgo : public IDispatch
    {
    public:
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCurrentImage( 
            /* [in] */ LONG *pImg,
            /* [in] */ BYTE crtImgIdentify,
            /* [out][in] */ LONG *pImgCorrect) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetCurrentLocationXY( 
            /* [retval][out] */ BSTR *bstrLocationXY) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetCenterLocation( 
            /* [retval][out] */ BSTR *bstrLocationXY) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetInterImgs( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetRegionEx( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetRegionExTest( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgo1( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgo1Help( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE IlluminateCenterLocationAlgoHS( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE IlluminateCenterLocationAlgoHSHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectLocationAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pRectWidth,
            /* [in] */ VARIANT *pShowInterRegions,
            /* [in] */ VARIANT *pBlackNoiseGray,
            /* [in] */ VARIANT *pWhiteNoiseGray,
            /* [in] */ VARIANT *pWhiteNoiseSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GrayLevelUniformityCorrectionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GrayLevelUniformityCorrectionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionAngleAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionAngleAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE IrregularRingRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *CandidateRadial,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pOutsideRatio,
            /* [in] */ VARIANT *pInsideRatio) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE IrregularRingRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE IrregularRingRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchRegRow,
            /* [in] */ VARIANT *pSearchRegCol,
            /* [in] */ VARIANT *pOuterRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pLocationGayThred,
            /* [in] */ VARIANT *pInsideRatio,
            /* [in] */ VARIANT *pOutsideRatio) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE IrregularRingRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectRegionExtraction( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLeftErosion,
            /* [in] */ VARIANT *pRightErosion) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectRegionExtractionHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PRectRegionExtraction( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRowBlock,
            /* [in] */ VARIANT *pLeftErosion,
            /* [in] */ VARIANT *pRightErosion) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PRectRegionExtractionHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RotaDynThresholdWithFilterAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RotaDynThresholdWithFilterAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EmphasizeDarkPointDetectAlgo( 
            VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pEmphasizeFactor,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EmphasizeDarkPointDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GDarkLineWithFilterAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GDarkLineWithFilterAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pDarkGrayThresh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GWhiteineWithFilterAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GWhiteineWithFilterAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pDarkGrayThresh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GSurfaceUnionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GSurfaceUnionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pLengthFilterSize,
            /* [in] */ VARIANT *pErroreAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GSurfaceAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GSurfaceAlgoHelp( 
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
            /* [in] */ VARIANT *pIsShowInterImg,
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoWhite( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdAlgoWhiteHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ReducedResolutionDynThreshAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlockSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pOffSet,
            /* [in] */ VARIANT *pHeightThred,
            /* [in] */ VARIANT *pWidthThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ReducedResolutionDynThreshAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TheColumnOffsetAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColumnValue,
            /* [retval][out] */ VARIANT *pOffsetValue) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE TheColumnOffsetAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FindingInterferenceAreaAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshThred,
            /* [in] */ VARIANT *pHeightLimit,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pInterferenceArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE FindingInterferenceAreaAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WaterSpotsDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WaterSpotsDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PFindingMoldScratchAreaAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshThred,
            /* [in] */ VARIANT *pWidthMin,
            /* [in] */ VARIANT *pWidthMax,
            /* [in] */ VARIANT *pHeightMin,
            /* [in] */ VARIANT *pHeightMax,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pInterferenceArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PFindingMoldScratchAreaAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ShoulderRingDeformationDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pRingWidth,
            /* [retval][out] */ VARIANT *pDetectWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ShoulderRingDeformationDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiffRegionMaxWidthDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiffRegionMaxWidthDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MoveImageDynThreshAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveMode,
            /* [in] */ VARIANT *pShiftSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MoveImageDynThreshAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MoveBlockDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pBlackGrayValue,
            /* [in] */ VARIANT *pWhiteGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MoveBlockDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleIncisionShapeChangeDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleIncisionShapeChangeDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeDetectWithLengthFilterAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeDetectWithLengthFilterAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pGaussFilterSize,
            /* [in] */ VARIANT *pContrasthredLow,
            /* [in] */ VARIANT *pContrastThredHigh,
            /* [in] */ VARIANT *pEdgeGrayTthresh,
            /* [in] */ VARIANT *pErrorLengthSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ExcludePointGradientDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThresh,
            /* [in] */ VARIANT *pBlackNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseSize,
            /* [in] */ VARIANT *pDeviationDiff,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ExcludePointGradientDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionAngleDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pBlackGayThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteGayThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CircleRegionAngleDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE OneWayDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLightDirection,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pAreaShrink,
            /* [in] */ VARIANT *pDetectionMode,
            /* [in] */ VARIANT *pSensitivity,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE OneWayDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectTheOCRSectorRegionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pCentralAngle,
            /* [retval][out] */ VARIANT *pOCRCenterDirection) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectTheOCRSectorRegionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DisOCRCircleRegionAngleAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2,
            /* [in] */ VARIANT *pDisOCR) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DisOCRCircleRegionAngleAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynDetectTheOCRSectorRegionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pCentralAngle,
            /* [retval][out] */ VARIANT *pOCRCenterDirection) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynDetectTheOCRSectorRegionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PVarThreshDarkDetectAlgo( 
            VARIANT *rgnId,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PVarThreshDarkDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PVarThreshLightDetectAlgo( 
            VARIANT *rgnId,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PVarThreshLightDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PGDarkLineAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackThredHigh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PGDarkLineAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PBrushDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pLineAreaFilter,
            /* [in] */ VARIANT *pErrorWidthFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PBrushDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PGLineXAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackFilterSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pHeightSizeFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PGLineXAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PThreshLightDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PThreshLightDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PThreshDarkDetectAlgo( 
            VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PThreshDarkDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDynThreshDeviationBlack( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousBlackPointSizeUpper,
            /* [in] */ VARIANT *pBlackPointDeviation,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDynThreshDeviationBlackHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PExDarkPointGradientDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThresh,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pBlackErrorFilterThresh,
            /* [in] */ VARIANT *pErrorDeviation,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PExDarkPointGradientDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PYPartialDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pMinResponse,
            /* [in] */ VARIANT *pSegementRatio,
            /* [in] */ VARIANT *pDeviationThred,
            /* [in] */ VARIANT *pBlackNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseSize,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PYPartialDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDentMeanCurveAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [in] */ VARIANT *pUnionSize,
            /* [in] */ VARIANT *pTotalSizeFilterLow,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDentMeanCurveAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PEdgeAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PEdgeAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDynThreshAlgoWhite( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDynThreshAlgoWhiteHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDynThreshAlgoBlack( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDynThreshAlgoBlackHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDyn2TimesAlgoBlack( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDyn2TimesAlgoBlackHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PHessianMatrixAlgo( 
            VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pGrayDeviationThresh,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PHessianMatrixAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PGradientDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThresh,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PGradientDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PEmphasizeDarkPointDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pWindowHeight,
            /* [in] */ VARIANT *pEmphasizeFactor,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PEmphasizeDarkPointDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDentAreaAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pSensitiveThred,
            /* [in] */ VARIANT *pErrorAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PDentAreaAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PYPartialDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRangeValue,
            /* [in] */ VARIANT *pDeviationThred,
            /* [in] */ VARIANT *pBlackNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseSize,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE PYPartialDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MatchingParaAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ BSTR *bstrShm1File,
            /* [retval][out] */ VARIANT *pMatchingResult) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE MatchingParaAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrRgnFile) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBatteryShellAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBatteryShellAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBatteryShellAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBatteryShellAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBatteryShellAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBatteryShellAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBatteryShellAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBatteryShellAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            IBatteryShellAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BYTE crtImgIdentify,
            /* [out][in] */ LONG *pImgCorrect);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            IBatteryShellAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            IBatteryShellAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhiteErrorRegion )( 
            IBatteryShellAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pWhiteErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            IBatteryShellAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            IBatteryShellAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            IBatteryShellAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            IBatteryShellAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetGlueInjectorID )( 
            IBatteryShellAlgo * This,
            /* [out][in] */ LONG *pInjectorID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurrentLocationXY )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetCenterLocation )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *bstrLocationXY);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInterImgs )( 
            IBatteryShellAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegionEx )( 
            IBatteryShellAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegionExTest )( 
            IBatteryShellAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            IBatteryShellAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            IBatteryShellAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            IBatteryShellAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            IBatteryShellAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            IBatteryShellAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            IBatteryShellAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            IBatteryShellAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            IBatteryShellAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            IBatteryShellAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1 )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1Help )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFitting )( 
            IBatteryShellAlgo * This,
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
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *IlluminateCenterLocationAlgoHS )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *IlluminateCenterLocationAlgoHSHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectLocationAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pRectWidth,
            /* [in] */ VARIANT *pShowInterRegions,
            /* [in] */ VARIANT *pBlackNoiseGray,
            /* [in] */ VARIANT *pWhiteNoiseGray,
            /* [in] */ VARIANT *pWhiteNoiseSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectLocationAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GrayLevelUniformityCorrectionAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GrayLevelUniformityCorrectionAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1 )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1Help )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo1 )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleAlgo1Help )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *IrregularRingRegionLocationAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *CandidateRadial,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pOutsideRatio,
            /* [in] */ VARIANT *pInsideRatio);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *IrregularRingRegionLocationAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *IrregularRingRegionLocationAlgo1 )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSearchRegRow,
            /* [in] */ VARIANT *pSearchRegCol,
            /* [in] */ VARIANT *pOuterRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pLocationGayThred,
            /* [in] */ VARIANT *pInsideRatio,
            /* [in] */ VARIANT *pOutsideRatio);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *IrregularRingRegionLocationAlgo1Help )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectRegionExtraction )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLeftErosion,
            /* [in] */ VARIANT *pRightErosion);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectRegionExtractionHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PRectRegionExtraction )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRowBlock,
            /* [in] */ VARIANT *pLeftErosion,
            /* [in] */ VARIANT *pRightErosion);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PRectRegionExtractionHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pWinType,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo2 )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo2Help )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pMeanGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *UVCurlAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RotaDynThresholdWithFilterAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RotaDynThresholdWithFilterAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EmphasizeDarkPointDetectAlgo )( 
            IBatteryShellAlgo * This,
            VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pEmphasizeFactor,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EmphasizeDarkPointDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GDarkLineWithFilterAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GDarkLineWithFilterAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pDarkGrayThresh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GWhiteineWithFilterAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GWhiteineWithFilterAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pDarkGrayThresh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceUnionAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceUnionAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pLengthFilterSize,
            /* [in] */ VARIANT *pErroreAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GSurfaceAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GContoursinkAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GContoursinkAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlack )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoBlackHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhite )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoWhiteHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo )( 
            IBatteryShellAlgo * This,
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
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReducedResolutionDynThreshAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlockSize,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pOffSet,
            /* [in] */ VARIANT *pHeightThred,
            /* [in] */ VARIANT *pWidthThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReducedResolutionDynThreshAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TheColumnOffsetAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColumnValue,
            /* [retval][out] */ VARIANT *pOffsetValue);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *TheColumnOffsetAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FindingInterferenceAreaAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshThred,
            /* [in] */ VARIANT *pHeightLimit,
            /* [in] */ VARIANT *pAreaLimit,
            /* [retval][out] */ VARIANT *pInterferenceArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *FindingInterferenceAreaAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WaterSpotsDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThred,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WaterSpotsDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PFindingMoldScratchAreaAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshThred,
            /* [in] */ VARIANT *pWidthMin,
            /* [in] */ VARIANT *pWidthMax,
            /* [in] */ VARIANT *pHeightMin,
            /* [in] */ VARIANT *pHeightMax,
            /* [in] */ VARIANT *pDilationSize,
            /* [retval][out] */ VARIANT *pInterferenceArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PFindingMoldScratchAreaAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ShoulderRingDeformationDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayValue,
            /* [in] */ VARIANT *pRingWidth,
            /* [retval][out] */ VARIANT *pDetectWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ShoulderRingDeformationDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiffRegionMaxWidthDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiffRegionMaxWidthDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MoveImageDynThreshAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveMode,
            /* [in] */ VARIANT *pShiftSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MoveImageDynThreshAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MoveBlockDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pBlackGrayValue,
            /* [in] */ VARIANT *pWhiteGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MoveBlockDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleIncisionShapeChangeDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleIncisionShapeChangeDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeDetectWithLengthFilterAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeDetectWithLengthFilterAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ VARIANT *pGaussFilterSize,
            /* [in] */ VARIANT *pContrasthredLow,
            /* [in] */ VARIANT *pContrastThredHigh,
            /* [in] */ VARIANT *pEdgeGrayTthresh,
            /* [in] */ VARIANT *pErrorLengthSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ExcludePointGradientDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThresh,
            /* [in] */ VARIANT *pBlackNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseSize,
            /* [in] */ VARIANT *pDeviationDiff,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ExcludePointGradientDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSmoothSize,
            /* [in] */ VARIANT *pBlackGayThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteGayThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionAngleDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *OneWayDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pLightDirection,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pAreaShrink,
            /* [in] */ VARIANT *pDetectionMode,
            /* [in] */ VARIANT *pSensitivity,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *OneWayDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectTheOCRSectorRegionAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pCentralAngle,
            /* [retval][out] */ VARIANT *pOCRCenterDirection);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectTheOCRSectorRegionAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DisOCRCircleRegionAngleAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2,
            /* [in] */ VARIANT *pDisOCR);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DisOCRCircleRegionAngleAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynDetectTheOCRSectorRegionAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pCentralAngle,
            /* [retval][out] */ VARIANT *pOCRCenterDirection);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynDetectTheOCRSectorRegionAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PVarThreshDarkDetectAlgo )( 
            IBatteryShellAlgo * This,
            VARIANT *rgnId,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PVarThreshDarkDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PVarThreshLightDetectAlgo )( 
            IBatteryShellAlgo * This,
            VARIANT *rgnId,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PVarThreshLightDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PGDarkLineAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackThredHigh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PGDarkLineAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PBrushDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pLineAreaFilter,
            /* [in] */ VARIANT *pErrorWidthFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PBrushDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PGLineXAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackFilterSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pHeightSizeFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PGLineXAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PThreshLightDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PThreshLightDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PThreshDarkDetectAlgo )( 
            IBatteryShellAlgo * This,
            VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PThreshDarkDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDynThreshDeviationBlack )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pSeriousBlackPointSizeUpper,
            /* [in] */ VARIANT *pBlackPointDeviation,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDynThreshDeviationBlackHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PExDarkPointGradientDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThresh,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pBlackErrorFilterThresh,
            /* [in] */ VARIANT *pErrorDeviation,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PExDarkPointGradientDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PYPartialDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pMinResponse,
            /* [in] */ VARIANT *pSegementRatio,
            /* [in] */ VARIANT *pDeviationThred,
            /* [in] */ VARIANT *pBlackNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseSize,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PYPartialDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDentMeanCurveAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [in] */ VARIANT *pWhiteThred,
            /* [in] */ VARIANT *pWhiteAreaThred,
            /* [in] */ VARIANT *pUnionSize,
            /* [in] */ VARIANT *pTotalSizeFilterLow,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDentMeanCurveAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PEdgeAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PEdgeAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDynThreshAlgoWhite )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDynThreshAlgoWhiteHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDynThreshAlgoBlack )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDynThreshAlgoBlackHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDyn2TimesAlgoBlack )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDyn2TimesAlgoBlackHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PHessianMatrixAlgo )( 
            IBatteryShellAlgo * This,
            VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackThred,
            /* [in] */ VARIANT *pGrayDeviationThresh,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PHessianMatrixAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PGradientDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThresh,
            /* [in] */ VARIANT *pBlackAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PGradientDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PEmphasizeDarkPointDetectAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pWindowHeight,
            /* [in] */ VARIANT *pEmphasizeFactor,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PEmphasizeDarkPointDetectAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDentAreaAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFilterSize,
            /* [in] */ VARIANT *pSensitiveThred,
            /* [in] */ VARIANT *pErrorAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PDentAreaAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PYPartialDetectAlgo1 )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRangeValue,
            /* [in] */ VARIANT *pDeviationThred,
            /* [in] */ VARIANT *pBlackNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseThred,
            /* [in] */ VARIANT *pWhiteNoiseSize,
            /* [in] */ VARIANT *pAreaThred,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *PYPartialDetectAlgo1Help )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [in] */ BSTR *bstrShm1File,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *MatchingParaAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgo )( 
            IBatteryShellAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pErosionSize,
            /* [in] */ BSTR *bstrRgnFile);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectRegionLocationAlgoHelp )( 
            IBatteryShellAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        END_INTERFACE
    } IBatteryShellAlgoVtbl;

    interface IBatteryShellAlgo
    {
        CONST_VTBL struct IBatteryShellAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBatteryShellAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBatteryShellAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBatteryShellAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBatteryShellAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBatteryShellAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBatteryShellAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBatteryShellAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IBatteryShellAlgo_SetCurrentImage(This,pImg,crtImgIdentify,pImgCorrect)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg,crtImgIdentify,pImgCorrect) ) 

#define IBatteryShellAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define IBatteryShellAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define IBatteryShellAlgo_GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn)	\
    ( (This)->lpVtbl -> GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn) ) 

#define IBatteryShellAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define IBatteryShellAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define IBatteryShellAlgo_SetCurrentTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName) ) 

#define IBatteryShellAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#define IBatteryShellAlgo_GetGlueInjectorID(This,pInjectorID)	\
    ( (This)->lpVtbl -> GetGlueInjectorID(This,pInjectorID) ) 

#define IBatteryShellAlgo_GetCurrentLocationXY(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCurrentLocationXY(This,bstrLocationXY) ) 

#define IBatteryShellAlgo_GetCenterLocation(This,bstrLocationXY)	\
    ( (This)->lpVtbl -> GetCenterLocation(This,bstrLocationXY) ) 

#define IBatteryShellAlgo_GetInterImgs(This,rgnID,pInterImgs)	\
    ( (This)->lpVtbl -> GetInterImgs(This,rgnID,pInterImgs) ) 

#define IBatteryShellAlgo_GetRegionEx(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegionEx(This,rgnID,pRgn) ) 

#define IBatteryShellAlgo_GetRegionExTest(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegionExTest(This,rgnID,pRgn) ) 

#define IBatteryShellAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define IBatteryShellAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define IBatteryShellAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define IBatteryShellAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define IBatteryShellAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define IBatteryShellAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define IBatteryShellAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define IBatteryShellAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define IBatteryShellAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define IBatteryShellAlgo_CenterLocationAlgo(This,rgnId,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnId,pRadius,pMinScore) ) 

#define IBatteryShellAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CenterLocationAlgo1(This,rgnID,pDetectType,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1(This,rgnID,pDetectType,pRadius,pMinScore) ) 

#define IBatteryShellAlgo_CenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1Help(This,pHelpStr) ) 

#define IBatteryShellAlgo_CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold) ) 

#define IBatteryShellAlgo_CenterLocationAlgoUsePointFittingHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFittingHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_IlluminateCenterLocationAlgoHS(This,rgnId,pIsShowInterImg,pDetectType,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> IlluminateCenterLocationAlgoHS(This,rgnId,pIsShowInterImg,pDetectType,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define IBatteryShellAlgo_IlluminateCenterLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> IlluminateCenterLocationAlgoHSHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_RectLocationAlgo(This,rgnID,pGrayThresh,pRectWidth,pShowInterRegions,pBlackNoiseGray,pWhiteNoiseGray,pWhiteNoiseSize)	\
    ( (This)->lpVtbl -> RectLocationAlgo(This,rgnID,pGrayThresh,pRectWidth,pShowInterRegions,pBlackNoiseGray,pWhiteNoiseGray,pWhiteNoiseSize) ) 

#define IBatteryShellAlgo_RectLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectLocationAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_GrayLevelUniformityCorrectionAlgo(This,rgnId,pFilterSize,pDetectArea)	\
    ( (This)->lpVtbl -> GrayLevelUniformityCorrectionAlgo(This,rgnId,pFilterSize,pDetectArea) ) 

#define IBatteryShellAlgo_GrayLevelUniformityCorrectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GrayLevelUniformityCorrectionAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CircleRegionLocationAlgo(This,rgnId,pIsShowInterImg,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo(This,rgnId,pIsShowInterImg,pRadius,pRoiWidth) ) 

#define IBatteryShellAlgo_CircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth) ) 

#define IBatteryShellAlgo_CircleRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IBatteryShellAlgo_CircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IBatteryShellAlgo_CircleRegionAngleAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CircleRegionAngleAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define IBatteryShellAlgo_CircleRegionAngleAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleAlgo1Help(This,pHelpStr) ) 

#define IBatteryShellAlgo_BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define IBatteryShellAlgo_BoundaryRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_IrregularRingRegionLocationAlgo(This,rgnId,CandidateRadial,pDetectType,pRadius,pMinScore,pOutsideRatio,pInsideRatio)	\
    ( (This)->lpVtbl -> IrregularRingRegionLocationAlgo(This,rgnId,CandidateRadial,pDetectType,pRadius,pMinScore,pOutsideRatio,pInsideRatio) ) 

#define IBatteryShellAlgo_IrregularRingRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> IrregularRingRegionLocationAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_IrregularRingRegionLocationAlgo1(This,rgnId,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pTransition,pLocationGayThred,pInsideRatio,pOutsideRatio)	\
    ( (This)->lpVtbl -> IrregularRingRegionLocationAlgo1(This,rgnId,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pTransition,pLocationGayThred,pInsideRatio,pOutsideRatio) ) 

#define IBatteryShellAlgo_IrregularRingRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> IrregularRingRegionLocationAlgo1Help(This,pHelpStr) ) 

#define IBatteryShellAlgo_RectRegionExtraction(This,rgnId,pLeftErosion,pRightErosion)	\
    ( (This)->lpVtbl -> RectRegionExtraction(This,rgnId,pLeftErosion,pRightErosion) ) 

#define IBatteryShellAlgo_RectRegionExtractionHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectRegionExtractionHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PRectRegionExtraction(This,rgnId,pRowBlock,pLeftErosion,pRightErosion)	\
    ( (This)->lpVtbl -> PRectRegionExtraction(This,rgnId,pRowBlock,pLeftErosion,pRightErosion) ) 

#define IBatteryShellAlgo_PRectRegionExtractionHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PRectRegionExtractionHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pWinType,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define IBatteryShellAlgo_CurlOptimizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CurlDetectAlgo2(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo2(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define IBatteryShellAlgo_CurlDetectAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgo2Help(This,pHelpStr) ) 

#define IBatteryShellAlgo_UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> UVCurlAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pMeanGrayValue,pDetectArea) ) 

#define IBatteryShellAlgo_UVCurlAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> UVCurlAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_RotaDynThresholdWithFilterAlgo(This,rgnId,pIsShowInterImg,pFilterSize,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> RotaDynThresholdWithFilterAlgo(This,rgnId,pIsShowInterImg,pFilterSize,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IBatteryShellAlgo_RotaDynThresholdWithFilterAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RotaDynThresholdWithFilterAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_EmphasizeDarkPointDetectAlgo(This,rgnId,pFilterSize,pWindowWidth,pEmphasizeFactor,pAreaFilter,pDetectArea)	\
    ( (This)->lpVtbl -> EmphasizeDarkPointDetectAlgo(This,rgnId,pFilterSize,pWindowWidth,pEmphasizeFactor,pAreaFilter,pDetectArea) ) 

#define IBatteryShellAlgo_EmphasizeDarkPointDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EmphasizeDarkPointDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CurlNoCompoundAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgo(This,rgnId,pGrayValueMean) ) 

#define IBatteryShellAlgo_CurlNoCompoundAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_GDarkLineWithFilterAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GDarkLineWithFilterAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_GDarkLineWithFilterAlgo(This,rgnId,pIsShowInterImg,pBlackMaskSize,pBlackThredLow,pDarkGrayThresh,pBlackDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GDarkLineWithFilterAlgo(This,rgnId,pIsShowInterImg,pBlackMaskSize,pBlackThredLow,pDarkGrayThresh,pBlackDefectSize,pDetectArea) ) 

#define IBatteryShellAlgo_GWhiteineWithFilterAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GWhiteineWithFilterAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_GWhiteineWithFilterAlgo(This,rgnId,pIsShowInterImg,pBlackMaskSize,pBlackThredLow,pDarkGrayThresh,pBlackDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GWhiteineWithFilterAlgo(This,rgnId,pIsShowInterImg,pBlackMaskSize,pBlackThredLow,pDarkGrayThresh,pBlackDefectSize,pDetectArea) ) 

#define IBatteryShellAlgo_GSurfaceUnionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GSurfaceUnionAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_GSurfaceUnionAlgo(This,rgnId,pBlackThred,pWhiteThred,pLengthFilterSize,pErroreAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GSurfaceUnionAlgo(This,rgnId,pBlackThred,pWhiteThred,pLengthFilterSize,pErroreAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GSurfaceAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_GSurfaceAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GSurfaceAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IBatteryShellAlgo_EdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_GContoursinkAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> GContoursinkAlgo(This,rgnId,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_GContoursinkAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GContoursinkAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DynThresholdAlgo(This,rgnId,pIsShowInterImg,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo(This,rgnId,pIsShowInterImg,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBatteryShellAlgo_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DynThresholdAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBatteryShellAlgo_DynThresholdAlgoBlackHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoBlackHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhite(This,rgnId,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBatteryShellAlgo_DynThresholdAlgoWhiteHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoWhiteHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define IBatteryShellAlgo_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_ReducedResolutionDynThreshAlgo(This,rgnId,pBlockSize,pDefectType,pFilterSize,pOffSet,pHeightThred,pWidthThred,pDetectArea)	\
    ( (This)->lpVtbl -> ReducedResolutionDynThreshAlgo(This,rgnId,pBlockSize,pDefectType,pFilterSize,pOffSet,pHeightThred,pWidthThred,pDetectArea) ) 

#define IBatteryShellAlgo_ReducedResolutionDynThreshAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ReducedResolutionDynThreshAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_TheColumnOffsetAlgo(This,rgnId,pColumnValue,pOffsetValue)	\
    ( (This)->lpVtbl -> TheColumnOffsetAlgo(This,rgnId,pColumnValue,pOffsetValue) ) 

#define IBatteryShellAlgo_TheColumnOffsetAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> TheColumnOffsetAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_FindingInterferenceAreaAlgo(This,rgnId,pDynThreshThred,pHeightLimit,pAreaLimit,pInterferenceArea)	\
    ( (This)->lpVtbl -> FindingInterferenceAreaAlgo(This,rgnId,pDynThreshThred,pHeightLimit,pAreaLimit,pInterferenceArea) ) 

#define IBatteryShellAlgo_FindingInterferenceAreaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> FindingInterferenceAreaAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_WaterSpotsDetectAlgo(This,rgnId,pGrayThred,pBlackThred,pAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> WaterSpotsDetectAlgo(This,rgnId,pGrayThred,pBlackThred,pAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_WaterSpotsDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WaterSpotsDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PFindingMoldScratchAreaAlgo(This,rgnId,pThreshThred,pWidthMin,pWidthMax,pHeightMin,pHeightMax,pDilationSize,pInterferenceArea)	\
    ( (This)->lpVtbl -> PFindingMoldScratchAreaAlgo(This,rgnId,pThreshThred,pWidthMin,pWidthMax,pHeightMin,pHeightMax,pDilationSize,pInterferenceArea) ) 

#define IBatteryShellAlgo_PFindingMoldScratchAreaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PFindingMoldScratchAreaAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_ShoulderRingDeformationDetectAlgo(This,rgnId,pGrayValue,pRingWidth,pDetectWidth)	\
    ( (This)->lpVtbl -> ShoulderRingDeformationDetectAlgo(This,rgnId,pGrayValue,pRingWidth,pDetectWidth) ) 

#define IBatteryShellAlgo_ShoulderRingDeformationDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ShoulderRingDeformationDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DiffRegionMaxWidthDetectAlgo(This,rgnId,pDetectWidth)	\
    ( (This)->lpVtbl -> DiffRegionMaxWidthDetectAlgo(This,rgnId,pDetectWidth) ) 

#define IBatteryShellAlgo_DiffRegionMaxWidthDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DiffRegionMaxWidthDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_MoveImageDynThreshAlgo(This,rgnId,pMoveMode,pShiftSize,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> MoveImageDynThreshAlgo(This,rgnId,pMoveMode,pShiftSize,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define IBatteryShellAlgo_MoveImageDynThreshAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MoveImageDynThreshAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_MoveBlockDetectAlgo(This,rgnId,pIsShowInterImg,pNumPart,pThresholdValue,pBlackGrayValue,pWhiteGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> MoveBlockDetectAlgo(This,rgnId,pIsShowInterImg,pNumPart,pThresholdValue,pBlackGrayValue,pWhiteGrayValue,pDetectArea) ) 

#define IBatteryShellAlgo_MoveBlockDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MoveBlockDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CircleIncisionShapeChangeDetectAlgo(This,rgnId,pDetectWidth)	\
    ( (This)->lpVtbl -> CircleIncisionShapeChangeDetectAlgo(This,rgnId,pDetectWidth) ) 

#define IBatteryShellAlgo_CircleIncisionShapeChangeDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleIncisionShapeChangeDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_EdgeDetectWithLengthFilterAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeDetectWithLengthFilterAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_EdgeDetectWithLengthFilterAlgo(This,rgnId,pErosionSize,pGaussFilterSize,pContrasthredLow,pContrastThredHigh,pEdgeGrayTthresh,pErrorLengthSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeDetectWithLengthFilterAlgo(This,rgnId,pErosionSize,pGaussFilterSize,pContrasthredLow,pContrastThredHigh,pEdgeGrayTthresh,pErrorLengthSize,pDetectArea) ) 

#define IBatteryShellAlgo_ExcludePointGradientDetectAlgo(This,rgnId,pEdgeThresh,pBlackNoiseThred,pWhiteNoiseThred,pWhiteNoiseSize,pDeviationDiff,pBlackAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> ExcludePointGradientDetectAlgo(This,rgnId,pEdgeThresh,pBlackNoiseThred,pWhiteNoiseThred,pWhiteNoiseSize,pDeviationDiff,pBlackAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_ExcludePointGradientDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> ExcludePointGradientDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_CircleRegionAngleDetectAlgo(This,rgnId,pSmoothSize,pBlackGayThred,pBlackAreaThred,pWhiteGayThred,pWhiteAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> CircleRegionAngleDetectAlgo(This,rgnId,pSmoothSize,pBlackGayThred,pBlackAreaThred,pWhiteGayThred,pWhiteAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_CircleRegionAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionAngleDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_OneWayDetectAlgo(This,rgnId,pLightDirection,pFilterSize,pAreaShrink,pDetectionMode,pSensitivity,pAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> OneWayDetectAlgo(This,rgnId,pLightDirection,pFilterSize,pAreaShrink,pDetectionMode,pSensitivity,pAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_OneWayDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> OneWayDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DetectTheOCRSectorRegionAlgo(This,rgnId,pThresholdValue,pCentralAngle,pOCRCenterDirection)	\
    ( (This)->lpVtbl -> DetectTheOCRSectorRegionAlgo(This,rgnId,pThresholdValue,pCentralAngle,pOCRCenterDirection) ) 

#define IBatteryShellAlgo_DetectTheOCRSectorRegionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectTheOCRSectorRegionAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DisOCRCircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2,pDisOCR)	\
    ( (This)->lpVtbl -> DisOCRCircleRegionAngleAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2,pDisOCR) ) 

#define IBatteryShellAlgo_DisOCRCircleRegionAngleAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DisOCRCircleRegionAngleAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DynDetectTheOCRSectorRegionAlgo(This,rgnId,pThresholdValue,pCentralAngle,pOCRCenterDirection)	\
    ( (This)->lpVtbl -> DynDetectTheOCRSectorRegionAlgo(This,rgnId,pThresholdValue,pCentralAngle,pOCRCenterDirection) ) 

#define IBatteryShellAlgo_DynDetectTheOCRSectorRegionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynDetectTheOCRSectorRegionAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PVarThreshDarkDetectAlgo(This,rgnId,pWindowWidth,pGrayThresh,pAreaFilter,pDetectArea)	\
    ( (This)->lpVtbl -> PVarThreshDarkDetectAlgo(This,rgnId,pWindowWidth,pGrayThresh,pAreaFilter,pDetectArea) ) 

#define IBatteryShellAlgo_PVarThreshDarkDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PVarThreshDarkDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PVarThreshLightDetectAlgo(This,rgnId,pWindowWidth,pGrayThresh,pAreaFilter,pDetectArea)	\
    ( (This)->lpVtbl -> PVarThreshLightDetectAlgo(This,rgnId,pWindowWidth,pGrayThresh,pAreaFilter,pDetectArea) ) 

#define IBatteryShellAlgo_PVarThreshLightDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PVarThreshLightDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PGDarkLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> PGDarkLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pDetectArea) ) 

#define IBatteryShellAlgo_PGDarkLineAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PGDarkLineAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PBrushDetectAlgo(This,rgnId,pDynThreshold,pLineAreaFilter,pErrorWidthFilter,pDetectArea)	\
    ( (This)->lpVtbl -> PBrushDetectAlgo(This,rgnId,pDynThreshold,pLineAreaFilter,pErrorWidthFilter,pDetectArea) ) 

#define IBatteryShellAlgo_PBrushDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PBrushDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PGLineXAlgo(This,rgnId,pBlackFilterSize,pBlackThredLow,pHeightSizeFilter,pDetectArea)	\
    ( (This)->lpVtbl -> PGLineXAlgo(This,rgnId,pBlackFilterSize,pBlackThredLow,pHeightSizeFilter,pDetectArea) ) 

#define IBatteryShellAlgo_PGLineXAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PGLineXAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PThreshLightDetectAlgo(This,rgnId,pGrayThresh,pAreaFilter,pDetectArea)	\
    ( (This)->lpVtbl -> PThreshLightDetectAlgo(This,rgnId,pGrayThresh,pAreaFilter,pDetectArea) ) 

#define IBatteryShellAlgo_PThreshLightDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PThreshLightDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PThreshDarkDetectAlgo(This,rgnId,pGrayThresh,pAreaFilter,pDetectArea)	\
    ( (This)->lpVtbl -> PThreshDarkDetectAlgo(This,rgnId,pGrayThresh,pAreaFilter,pDetectArea) ) 

#define IBatteryShellAlgo_PThreshDarkDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PThreshDarkDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PDynThreshDeviationBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousBlackPointSizeUpper,pBlackPointDeviation,pDetectArea)	\
    ( (This)->lpVtbl -> PDynThreshDeviationBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousBlackPointSizeUpper,pBlackPointDeviation,pDetectArea) ) 

#define IBatteryShellAlgo_PDynThreshDeviationBlackHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PDynThreshDeviationBlackHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PExDarkPointGradientDetectAlgo(This,rgnId,pEdgeThresh,pBlackAreaThred,pBlackErrorFilterThresh,pErrorDeviation,pDetectArea)	\
    ( (This)->lpVtbl -> PExDarkPointGradientDetectAlgo(This,rgnId,pEdgeThresh,pBlackAreaThred,pBlackErrorFilterThresh,pErrorDeviation,pDetectArea) ) 

#define IBatteryShellAlgo_PExDarkPointGradientDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PExDarkPointGradientDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PYPartialDetectAlgo(This,rgnId,pFilterSize,pMinResponse,pSegementRatio,pDeviationThred,pBlackNoiseThred,pWhiteNoiseThred,pWhiteNoiseSize,pAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> PYPartialDetectAlgo(This,rgnId,pFilterSize,pMinResponse,pSegementRatio,pDeviationThred,pBlackNoiseThred,pWhiteNoiseThred,pWhiteNoiseSize,pAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_PYPartialDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PYPartialDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PDentMeanCurveAlgo(This,rgnId,pFilterSize,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pUnionSize,pTotalSizeFilterLow,pDetectArea)	\
    ( (This)->lpVtbl -> PDentMeanCurveAlgo(This,rgnId,pFilterSize,pBlackThred,pBlackAreaThred,pWhiteThred,pWhiteAreaThred,pUnionSize,pTotalSizeFilterLow,pDetectArea) ) 

#define IBatteryShellAlgo_PDentMeanCurveAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PDentMeanCurveAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PEdgeAlgo(This,rgnId,pMaskSize,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> PEdgeAlgo(This,rgnId,pMaskSize,pEdgeThred,pDefectSize,pDetectArea) ) 

#define IBatteryShellAlgo_PEdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PEdgeAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PDynThreshAlgoWhite(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> PDynThreshAlgoWhite(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBatteryShellAlgo_PDynThreshAlgoWhiteHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PDynThreshAlgoWhiteHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PDynThreshAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> PDynThreshAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBatteryShellAlgo_PDynThreshAlgoBlackHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PDynThreshAlgoBlackHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PDyn2TimesAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> PDyn2TimesAlgoBlack(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define IBatteryShellAlgo_PDyn2TimesAlgoBlackHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PDyn2TimesAlgoBlackHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PHessianMatrixAlgo(This,rgnId,pBlackThred,pGrayDeviationThresh,pBlackAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> PHessianMatrixAlgo(This,rgnId,pBlackThred,pGrayDeviationThresh,pBlackAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_PHessianMatrixAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PHessianMatrixAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PGradientDetectAlgo(This,rgnId,pEdgeThresh,pBlackAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> PGradientDetectAlgo(This,rgnId,pEdgeThresh,pBlackAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_PGradientDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PGradientDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PEmphasizeDarkPointDetectAlgo(This,rgnId,pWindowWidth,pWindowHeight,pEmphasizeFactor,pAreaFilter,pDetectArea)	\
    ( (This)->lpVtbl -> PEmphasizeDarkPointDetectAlgo(This,rgnId,pWindowWidth,pWindowHeight,pEmphasizeFactor,pAreaFilter,pDetectArea) ) 

#define IBatteryShellAlgo_PEmphasizeDarkPointDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PEmphasizeDarkPointDetectAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PDentAreaAlgo(This,rgnId,pFilterSize,pSensitiveThred,pErrorAreaFilter,pDetectArea)	\
    ( (This)->lpVtbl -> PDentAreaAlgo(This,rgnId,pFilterSize,pSensitiveThred,pErrorAreaFilter,pDetectArea) ) 

#define IBatteryShellAlgo_PDentAreaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> PDentAreaAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_PYPartialDetectAlgo1(This,rgnId,pRangeValue,pDeviationThred,pBlackNoiseThred,pWhiteNoiseThred,pWhiteNoiseSize,pAreaThred,pDetectArea)	\
    ( (This)->lpVtbl -> PYPartialDetectAlgo1(This,rgnId,pRangeValue,pDeviationThred,pBlackNoiseThred,pWhiteNoiseThred,pWhiteNoiseSize,pAreaThred,pDetectArea) ) 

#define IBatteryShellAlgo_PYPartialDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> PYPartialDetectAlgo1Help(This,pHelpStr) ) 

#define IBatteryShellAlgo_MatchingParaAlgo(This,rgnId,pGreediness,bstrShm1File,pMatchingResult)	\
    ( (This)->lpVtbl -> MatchingParaAlgo(This,rgnId,pGreediness,bstrShm1File,pMatchingResult) ) 

#define IBatteryShellAlgo_MatchingParaAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> MatchingParaAlgoHelp(This,pHelpStr) ) 

#define IBatteryShellAlgo_DetectRegionLocationAlgo(This,rgnId,pErosionSize,bstrRgnFile)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgo(This,rgnId,pErosionSize,bstrRgnFile) ) 

#define IBatteryShellAlgo_DetectRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectRegionLocationAlgoHelp(This,pHelpStr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PVarThreshLightDetectAlgo_Proxy( 
    IBatteryShellAlgo * This,
    VARIANT *rgnId,
    /* [in] */ VARIANT *pWindowWidth,
    /* [in] */ VARIANT *pGrayThresh,
    /* [in] */ VARIANT *pAreaFilter,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PVarThreshLightDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PVarThreshLightDetectAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PVarThreshLightDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PGDarkLineAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pBlackThredLow,
    /* [in] */ VARIANT *pBlackThredHigh,
    /* [in] */ VARIANT *pBlackDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PGDarkLineAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PGDarkLineAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PGDarkLineAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PBrushDetectAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pDynThreshold,
    /* [in] */ VARIANT *pLineAreaFilter,
    /* [in] */ VARIANT *pErrorWidthFilter,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PBrushDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PBrushDetectAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PBrushDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PGLineXAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackFilterSize,
    /* [in] */ VARIANT *pBlackThredLow,
    /* [in] */ VARIANT *pHeightSizeFilter,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PGLineXAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PGLineXAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PGLineXAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PThreshLightDetectAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayThresh,
    /* [in] */ VARIANT *pAreaFilter,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PThreshLightDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PThreshLightDetectAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PThreshLightDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PThreshDarkDetectAlgo_Proxy( 
    IBatteryShellAlgo * This,
    VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayThresh,
    /* [in] */ VARIANT *pAreaFilter,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PThreshDarkDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PThreshDarkDetectAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PThreshDarkDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDynThreshDeviationBlack_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [in] */ VARIANT *pSeriousBlackPointSizeUpper,
    /* [in] */ VARIANT *pBlackPointDeviation,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PDynThreshDeviationBlack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDynThreshDeviationBlackHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PDynThreshDeviationBlackHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PExDarkPointGradientDetectAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pEdgeThresh,
    /* [in] */ VARIANT *pBlackAreaThred,
    /* [in] */ VARIANT *pBlackErrorFilterThresh,
    /* [in] */ VARIANT *pErrorDeviation,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PExDarkPointGradientDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PExDarkPointGradientDetectAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PExDarkPointGradientDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PYPartialDetectAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pFilterSize,
    /* [in] */ VARIANT *pMinResponse,
    /* [in] */ VARIANT *pSegementRatio,
    /* [in] */ VARIANT *pDeviationThred,
    /* [in] */ VARIANT *pBlackNoiseThred,
    /* [in] */ VARIANT *pWhiteNoiseThred,
    /* [in] */ VARIANT *pWhiteNoiseSize,
    /* [in] */ VARIANT *pAreaThred,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PYPartialDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PYPartialDetectAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PYPartialDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDentMeanCurveAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pFilterSize,
    /* [in] */ VARIANT *pBlackThred,
    /* [in] */ VARIANT *pBlackAreaThred,
    /* [in] */ VARIANT *pWhiteThred,
    /* [in] */ VARIANT *pWhiteAreaThred,
    /* [in] */ VARIANT *pUnionSize,
    /* [in] */ VARIANT *pTotalSizeFilterLow,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PDentMeanCurveAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDentMeanCurveAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PDentMeanCurveAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PEdgeAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMaskSize,
    /* [in] */ VARIANT *pEdgeThred,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PEdgeAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PEdgeAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PEdgeAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDynThreshAlgoWhite_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PDynThreshAlgoWhite_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDynThreshAlgoWhiteHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PDynThreshAlgoWhiteHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDynThreshAlgoBlack_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PDynThreshAlgoBlack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDynThreshAlgoBlackHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PDynThreshAlgoBlackHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDyn2TimesAlgoBlack_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackMaskSize,
    /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
    /* [in] */ VARIANT *pSeriousBlackPointSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PDyn2TimesAlgoBlack_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDyn2TimesAlgoBlackHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PDyn2TimesAlgoBlackHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PHessianMatrixAlgo_Proxy( 
    IBatteryShellAlgo * This,
    VARIANT *rgnId,
    /* [in] */ VARIANT *pBlackThred,
    /* [in] */ VARIANT *pGrayDeviationThresh,
    /* [in] */ VARIANT *pBlackAreaThred,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PHessianMatrixAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PHessianMatrixAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PHessianMatrixAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PGradientDetectAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pEdgeThresh,
    /* [in] */ VARIANT *pBlackAreaThred,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PGradientDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PGradientDetectAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PGradientDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PEmphasizeDarkPointDetectAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pWindowWidth,
    /* [in] */ VARIANT *pWindowHeight,
    /* [in] */ VARIANT *pEmphasizeFactor,
    /* [in] */ VARIANT *pAreaFilter,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PEmphasizeDarkPointDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PEmphasizeDarkPointDetectAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PEmphasizeDarkPointDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDentAreaAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pFilterSize,
    /* [in] */ VARIANT *pSensitiveThred,
    /* [in] */ VARIANT *pErrorAreaFilter,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PDentAreaAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PDentAreaAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PDentAreaAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PYPartialDetectAlgo1_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRangeValue,
    /* [in] */ VARIANT *pDeviationThred,
    /* [in] */ VARIANT *pBlackNoiseThred,
    /* [in] */ VARIANT *pWhiteNoiseThred,
    /* [in] */ VARIANT *pWhiteNoiseSize,
    /* [in] */ VARIANT *pAreaThred,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB IBatteryShellAlgo_PYPartialDetectAlgo1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_PYPartialDetectAlgo1Help_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_PYPartialDetectAlgo1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_MatchingParaAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGreediness,
    /* [in] */ BSTR *bstrShm1File,
    /* [retval][out] */ VARIANT *pMatchingResult);


void __RPC_STUB IBatteryShellAlgo_MatchingParaAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_MatchingParaAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_MatchingParaAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_DetectRegionLocationAlgo_Proxy( 
    IBatteryShellAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pErosionSize,
    /* [in] */ BSTR *bstrRgnFile);


void __RPC_STUB IBatteryShellAlgo_DetectRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE IBatteryShellAlgo_DetectRegionLocationAlgoHelp_Proxy( 
    IBatteryShellAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB IBatteryShellAlgo_DetectRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBatteryShellAlgo_INTERFACE_DEFINED__ */



#ifndef __BatteryShellAlgoLib_LIBRARY_DEFINED__
#define __BatteryShellAlgoLib_LIBRARY_DEFINED__

/* library BatteryShellAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_BatteryShellAlgoLib;

EXTERN_C const CLSID CLSID_BatteryShellAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("6A0C10CA-4D87-46CB-B4DE-D341D46E2B6B")
BatteryShellAlgo;
#endif
#endif /* __BatteryShellAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


