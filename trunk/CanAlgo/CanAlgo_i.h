

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __CanAlgo_i_h__
#define __CanAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICanAlgo_FWD_DEFINED__
#define __ICanAlgo_FWD_DEFINED__
typedef interface ICanAlgo ICanAlgo;
#endif 	/* __ICanAlgo_FWD_DEFINED__ */


#ifndef __CanAlgo_FWD_DEFINED__
#define __CanAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class CanAlgo CanAlgo;
#else
typedef struct CanAlgo CanAlgo;
#endif /* __cplusplus */

#endif 	/* __CanAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICanAlgo_INTERFACE_DEFINED__
#define __ICanAlgo_INTERFACE_DEFINED__

/* interface ICanAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICanAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6D2F69FF-2AA7-414B-8E88-86C3419839D4")
    ICanAlgo : public IDispatch
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE GetInterImgs( 
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs) = 0;
        
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
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LocationAlgo( 
            /* [in] */ VARIANT *rgnID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE LocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoMultImg( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoMultImgHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRelocationAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadiusSet,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterRelocationAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoUsePointFittingColor( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pSearchRegRow,
            /* [in] */ VARIANT *pSearchRegCol,
            /* [in] */ VARIANT *pOuterRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pScanLineNum,
            /* [in] */ VARIANT *pScanLineDirection,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pColorChannel,
            /* [in] */ VARIANT *pTransType) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoUsePointFittingColorHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleLeftLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pWidthPlaceSet,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth,
            /* [in] */ VARIANT *pErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleLeftLocationAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BlackDetectAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BodyDirectionDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [retval][out] */ VARIANT *pMatchingResult) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BodyDirectionDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BodyDirectionDetectMinScoreAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pNumLevels,
            /* [in] */ VARIANT *pGreediness,
            /* [retval][out] */ VARIANT *pMatchingResult) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BodyDirectionDetectMinScoreAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DoubleEndRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBottomRow,
            /* [in] */ VARIANT *pRadiusWidth,
            /* [in] */ VARIANT *pRadiusHeight,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DoubleEndRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DoubleEndDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSigmaSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackThredHigh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DoubleEndDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BodyDirectionDetectVecDiffAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pVecDiffValue) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BodyDirectionDetectVecDiffAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BodyDirectionVecDiffLocationAlgo( 
            /* [in] */ VARIANT *rgnID) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BodyDirectionVecDiffLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BoundaryRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BoundaryRegionLocationAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreqCutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlOptimizeForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlMeanGrayDetectForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pGrayValueLower,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlMeanGrayDetectForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE AnnularRotaDynThresholdForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
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
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [retval][out] */ VARIANT *pGrayValueMean) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlNoCompoundForColorAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DynThresholdForColorAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
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
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE EdgeForColorAlgoHelp( 
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE VarThreshDarkDetectAlgo( 
            VARIANT *rgnId,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE VarThreshDarkDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CurlDetectAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterReLocationAlgo2( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pUsePolarity,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterReLocationAlgo2Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RingRegionDynThresholdAlgoBW( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RingRegionDynThresholdAlgoBWHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BoundaryRegionLocationAlgo1( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pClosingType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BoundaryRegionLocationAlgo1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoM( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshValue) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoMHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWidthPlaceSet,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BaseLineDetectAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pElements,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pDefectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE BaseLineDetectAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleLocation2Algo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWidthPlaceSet,
            /* [in] */ VARIANT *pHeightPlaceSet,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleLocation2AlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HeightMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE HeightMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiameterMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pElements,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DiameterMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InnerDiameterMeasureAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDefectDiff) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE InnerDiameterMeasureAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThredValueUp) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideCenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideRegionLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pErosioSizeH,
            /* [in] */ VARIANT *pErosioSizeV) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SideRegionLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoUsePointFitting1( 
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
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pDistThresh) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoUsePointFitting1Help( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldRegionSelection( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pWeldSelection,
            /* [in] */ VARIANT *pRegionErosionSize) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldRegionSelectionHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldRegionExtractionAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThresh,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE WeldRegionExtractionAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRotaDynThresholdAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SectorRotaDynThresholdAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICanAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICanAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICanAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICanAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICanAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICanAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICanAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICanAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            ICanAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            ICanAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            ICanAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetWhiteErrorRegion )( 
            ICanAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pWhiteErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInterImgs )( 
            ICanAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pInterImgs);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            ICanAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            ICanAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            ICanAlgo * This,
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            ICanAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            ICanAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            ICanAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            ICanAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            ICanAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            ICanAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            ICanAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            ICanAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            ICanAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            ICanAlgo * This,
            /* [in] */ BSTR *bstrTaskName,
            /* [in] */ BSTR *bstrChName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *LocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoMultImg )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoMultImgHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1 )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo1Help )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHS )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHSHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRelocationAlgoHS )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadiusLong,
            /* [in] */ VARIANT *pRadiusShort,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pOutlierRemoverSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRelocationAlgoHSHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRelocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadiusSet,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterRelocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFitting )( 
            ICanAlgo * This,
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
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFittingColor )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pSearchRegRow,
            /* [in] */ VARIANT *pSearchRegCol,
            /* [in] */ VARIANT *pOuterRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pScanLineNum,
            /* [in] */ VARIANT *pScanLineDirection,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pColorChannel,
            /* [in] */ VARIANT *pTransType);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFittingColorHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleLeftLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThresholdLow,
            /* [in] */ VARIANT *pWidthPlaceSet,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth,
            /* [in] */ VARIANT *pErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleLeftLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1 )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CircleRegionLocationAlgo1Help )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo1 )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pDriftAngle1,
            /* [in] */ VARIANT *pDriftAngle2);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRegionLocationAlgo1Help )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectGrayAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pAlgoSelect,
            /* [in] */ VARIANT *pWeldWidth,
            /* [in] */ VARIANT *pSmoothSize,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectGrayAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectWhiteGlueAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pAspectRationThresh,
            /* [in] */ VARIANT *pClosingMaskWidth,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectWhiteGlueAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectGlueAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackWhite,
            /* [in] */ VARIANT *pEnhancePara,
            /* [in] */ VARIANT *pDynThreshold,
            /* [in] */ VARIANT *pAspectRationThresh,
            /* [in] */ VARIANT *pClosingMaskWidth,
            /* [retval][out] */ VARIANT *pDetectAngle);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldAngleDetectGlueAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlMeanGrayDetectAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pGrayValueLower,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlMeanGrayDetectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreq_cutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBW )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdAlgoBWHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pBlackThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BlackDetectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAvoidReflectionAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdAvoidReflectionAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GLineAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BodyDirectionDetectAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGreediness,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BodyDirectionDetectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BodyDirectionDetectMinScoreAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMinScore,
            /* [in] */ VARIANT *pNumLevels,
            /* [in] */ VARIANT *pGreediness,
            /* [retval][out] */ VARIANT *pMatchingResult);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BodyDirectionDetectMinScoreAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleEndRegionLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBottomRow,
            /* [in] */ VARIANT *pRadiusWidth,
            /* [in] */ VARIANT *pRadiusHeight,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleEndRegionLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleEndDetectAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSigmaSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackThredHigh,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleEndDetectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BodyDirectionDetectVecDiffAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [retval][out] */ VARIANT *pVecDiffValue);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BodyDirectionDetectVecDiffAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BodyDirectionVecDiffLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BodyDirectionVecDiffLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldWhiteGlueRegExtractAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [retval][out] */ VARIANT *pWeldRegArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldWhiteGlueRegExtractAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldRegSelectAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWeldSelection);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldRegSelectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeForColorAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pFreq_start,
            /* [in] */ VARIANT *pFreqCutoff,
            /* [in] */ VARIANT *pDefectType,
            /* [in] */ VARIANT *pWaveThreshold,
            /* [in] */ VARIANT *pMinArea,
            /* [retval][out] */ VARIANT *pDetectNumber);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlOptimizeForColorAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectForColorAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectForColorAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlMeanGrayDetectForColorAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pGrayValueLower,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlMeanGrayDetectForColorAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AnnularRotaDynThresholdForColorAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundForColorAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [retval][out] */ VARIANT *pGrayValueMean);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlNoCompoundForColorAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pBlackMaskSize,
            /* [in] */ VARIANT *pSeriousBlackPointDynThresh,
            /* [in] */ VARIANT *pSeriousBlackPointSize,
            /* [in] */ VARIANT *pWhiteMaskSize,
            /* [in] */ VARIANT *pSeriousWhitePointDynThresh,
            /* [in] */ VARIANT *pSeriousWhitePointSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DynThresholdForColorAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pColorTransType,
            /* [in] */ VARIANT *pChannelSelect,
            /* [in] */ VARIANT *pIsShowInterImg,
            /* [in] */ VARIANT *pEdgeThred,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *EdgeForColorAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DerivateGaussDetectAlgo )( 
            ICanAlgo * This,
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
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *VarThreshDarkDetectAlgo )( 
            ICanAlgo * This,
            VARIANT *rgnId,
            /* [in] */ VARIANT *pWindowWidth,
            /* [in] */ VARIANT *pGrayThresh,
            /* [in] */ VARIANT *pAreaFilter,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *VarThreshDarkDetectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo1 )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pNumPart,
            /* [in] */ VARIANT *pThresholdValue,
            /* [in] */ VARIANT *pGrayValue,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CurlDetectAlgo1Help )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterReLocationAlgo2 )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pUsePolarity,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterReLocationAlgo2Help )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RingRegionDynThresholdAlgoBW )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pDetectType,
            /* [in] */ VARIANT *pMaskSize,
            /* [in] */ VARIANT *pBlackThredLow,
            /* [in] */ VARIANT *pBlackDefectSize,
            /* [in] */ VARIANT *pWhiteThredLow,
            /* [in] */ VARIANT *pWhiteDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RingRegionDynThresholdAlgoBWHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgo1 )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pClosingType,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BoundaryRegionLocationAlgo1Help )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoM )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThreshValue);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoMHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWidthPlaceSet,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BaseLineDetectAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredLow,
            /* [in] */ VARIANT *pElements,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold,
            /* [retval][out] */ VARIANT *pDefectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *BaseLineDetectAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleLocation2Algo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pWidthPlaceSet,
            /* [in] */ VARIANT *pHeightPlaceSet,
            /* [in] */ VARIANT *pRegionHeight,
            /* [in] */ VARIANT *pRegionWidth);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleLocation2AlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HeightMeasureAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *HeightMeasureAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiameterMeasureAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pThredValue,
            /* [in] */ VARIANT *pElements,
            /* [in] */ VARIANT *pTransition,
            /* [in] */ VARIANT *pSelect,
            /* [in] */ VARIANT *pSigma,
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DiameterMeasureAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InnerDiameterMeasureAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pSwitch,
            /* [in] */ VARIANT *pOffset,
            /* [retval][out] */ VARIANT *pDefectDiff);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *InnerDiameterMeasureAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRowStart,
            /* [in] */ VARIANT *pColStart,
            /* [in] */ VARIANT *pRegWidth,
            /* [in] */ VARIANT *pRegHeight,
            /* [in] */ VARIANT *pThredValueUp);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideCenterLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideRegionLocationAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pMoveSize,
            /* [in] */ VARIANT *pErosioSizeH,
            /* [in] */ VARIANT *pErosioSizeV);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SideRegionLocationAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFitting1 )( 
            ICanAlgo * This,
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
            /* [in] */ VARIANT *pThreshold,
            /* [in] */ VARIANT *pDistThresh);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoUsePointFitting1Help )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldRegionSelection )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pRoiWidth,
            /* [in] */ VARIANT *pWeldSelection,
            /* [in] */ VARIANT *pRegionErosionSize);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldRegionSelectionHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldRegionExtractionAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pGrayThresh,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *WeldRegionExtractionAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRotaDynThresholdAlgo )( 
            ICanAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pRotaAngle,
            /* [in] */ VARIANT *pDynThresh,
            /* [in] */ VARIANT *pDefectSize,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SectorRotaDynThresholdAlgoHelp )( 
            ICanAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        END_INTERFACE
    } ICanAlgoVtbl;

    interface ICanAlgo
    {
        CONST_VTBL struct ICanAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICanAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICanAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICanAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICanAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICanAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICanAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICanAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICanAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define ICanAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define ICanAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define ICanAlgo_GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn)	\
    ( (This)->lpVtbl -> GetWhiteErrorRegion(This,rgnID,pWhiteErrorRgn) ) 

#define ICanAlgo_GetInterImgs(This,rgnID,pInterImgs)	\
    ( (This)->lpVtbl -> GetInterImgs(This,rgnID,pInterImgs) ) 

#define ICanAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define ICanAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define ICanAlgo_SetCurrentTaskName(This,bstrTaskName,bstrChName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName,bstrChName) ) 

#define ICanAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define ICanAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define ICanAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define ICanAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define ICanAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define ICanAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define ICanAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define ICanAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define ICanAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define ICanAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName,bstrChName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName,bstrChName) ) 

#define ICanAlgo_LocationAlgo(This,rgnID)	\
    ( (This)->lpVtbl -> LocationAlgo(This,rgnID) ) 

#define ICanAlgo_LocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> LocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CenterLocationAlgoMultImg(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgoMultImg(This,rgnID,pRadius,pMinScore) ) 

#define ICanAlgo_CenterLocationAlgoMultImgHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoMultImgHelp(This,pHelpStr) ) 

#define ICanAlgo_CenterLocationAlgo(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnID,pRadius,pMinScore) ) 

#define ICanAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CenterLocationAlgo1(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1(This,rgnID,pRadius,pMinScore) ) 

#define ICanAlgo_CenterLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgo1Help(This,pHelpStr) ) 

#define ICanAlgo_CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHS(This,rgnId,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define ICanAlgo_CenterLocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHSHelp(This,pHelpStr) ) 

#define ICanAlgo_CenterRelocationAlgoHS(This,rgnId,pRadiusLong,pRadiusShort,pRadius,pThresholdLow,pOutlierRemoverSize)	\
    ( (This)->lpVtbl -> CenterRelocationAlgoHS(This,rgnId,pRadiusLong,pRadiusShort,pRadius,pThresholdLow,pOutlierRemoverSize) ) 

#define ICanAlgo_CenterRelocationAlgoHSHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRelocationAlgoHSHelp(This,pHelpStr) ) 

#define ICanAlgo_CenterRelocationAlgo(This,rgnID,pRadiusSet,pRoiWidth,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterRelocationAlgo(This,rgnID,pRadiusSet,pRoiWidth,pRadius,pMinScore) ) 

#define ICanAlgo_CenterRelocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterRelocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFitting(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold) ) 

#define ICanAlgo_CenterLocationAlgoUsePointFittingHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFittingHelp(This,pHelpStr) ) 

#define ICanAlgo_CenterLocationAlgoUsePointFittingColor(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pSigma,pThreshold,pColorChannel,pTransType)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFittingColor(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pSigma,pThreshold,pColorChannel,pTransType) ) 

#define ICanAlgo_CenterLocationAlgoUsePointFittingColorHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFittingColorHelp(This,pHelpStr) ) 

#define ICanAlgo_RectangleLeftLocationAlgo(This,rgnId,pThresholdLow,pWidthPlaceSet,pRegionHeight,pRegionWidth,pErosionSize)	\
    ( (This)->lpVtbl -> RectangleLeftLocationAlgo(This,rgnId,pThresholdLow,pWidthPlaceSet,pRegionHeight,pRegionWidth,pErosionSize) ) 

#define ICanAlgo_RectangleLeftLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectangleLeftLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define ICanAlgo_CircleRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth) ) 

#define ICanAlgo_CircleRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CircleRegionLocationAlgo1Help(This,pHelpStr) ) 

#define ICanAlgo_SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define ICanAlgo_SectorRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_SectorRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo1(This,rgnId,pRadius,pRoiWidth,pDriftAngle1,pDriftAngle2) ) 

#define ICanAlgo_SectorRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRegionLocationAlgo1Help(This,pHelpStr) ) 

#define ICanAlgo_WeldAngleDetectAlgo(This,rgnId,pDetectAngle)	\
    ( (This)->lpVtbl -> WeldAngleDetectAlgo(This,rgnId,pDetectAngle) ) 

#define ICanAlgo_WeldAngleDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldAngleDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_WeldAngleDetectGrayAlgo(This,rgnId,pAlgoSelect,pWeldWidth,pSmoothSize,pDetectAngle)	\
    ( (This)->lpVtbl -> WeldAngleDetectGrayAlgo(This,rgnId,pAlgoSelect,pWeldWidth,pSmoothSize,pDetectAngle) ) 

#define ICanAlgo_WeldAngleDetectGrayAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldAngleDetectGrayAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_WeldAngleDetectWhiteGlueAlgo(This,rgnId,pDynThreshold,pAspectRationThresh,pClosingMaskWidth,pDetectAngle)	\
    ( (This)->lpVtbl -> WeldAngleDetectWhiteGlueAlgo(This,rgnId,pDynThreshold,pAspectRationThresh,pClosingMaskWidth,pDetectAngle) ) 

#define ICanAlgo_WeldAngleDetectWhiteGlueAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldAngleDetectWhiteGlueAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_WeldAngleDetectGlueAlgo(This,rgnId,pBlackWhite,pEnhancePara,pDynThreshold,pAspectRationThresh,pClosingMaskWidth,pDetectAngle)	\
    ( (This)->lpVtbl -> WeldAngleDetectGlueAlgo(This,rgnId,pBlackWhite,pEnhancePara,pDynThreshold,pAspectRationThresh,pClosingMaskWidth,pDetectAngle) ) 

#define ICanAlgo_WeldAngleDetectGlueAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldAngleDetectGlueAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlDetectAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define ICanAlgo_CurlDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlMeanGrayDetectAlgo(This,rgnId,pNumPart,pGrayValueLower,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlMeanGrayDetectAlgo(This,rgnId,pNumPart,pGrayValueLower,pGrayValue,pDetectArea) ) 

#define ICanAlgo_CurlMeanGrayDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlMeanGrayDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgo(This,rgnId,pFreq_start,pFreq_cutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define ICanAlgo_CurlOptimizeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define ICanAlgo_AnnularRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBW(This,rgnId,pRotaAngle,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define ICanAlgo_AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdAlgoBWHelp(This,pHelpStr) ) 

#define ICanAlgo_BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea)	\
    ( (This)->lpVtbl -> BlackDetectAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pBlackThresh,pSeriousBlackPointSize,pDetectArea) ) 

#define ICanAlgo_BlackDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BlackDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define ICanAlgo_DynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_DynThresholdAvoidReflectionAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdAvoidReflectionAlgo(This,rgnId,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define ICanAlgo_DynThresholdAvoidReflectionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdAvoidReflectionAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> GLineAlgo(This,rgnId,pBlackMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteMaskSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define ICanAlgo_GLineAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> GLineAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeAlgo(This,rgnId,pEdgeThred,pDefectSize,pDetectArea) ) 

#define ICanAlgo_EdgeAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlNoCompoundAlgo(This,rgnId,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgo(This,rgnId,pGrayValueMean) ) 

#define ICanAlgo_CurlNoCompoundAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_BodyDirectionDetectAlgo(This,rgnId,pGreediness,pMatchingResult)	\
    ( (This)->lpVtbl -> BodyDirectionDetectAlgo(This,rgnId,pGreediness,pMatchingResult) ) 

#define ICanAlgo_BodyDirectionDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BodyDirectionDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_BodyDirectionDetectMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pMatchingResult)	\
    ( (This)->lpVtbl -> BodyDirectionDetectMinScoreAlgo(This,rgnId,pMinScore,pNumLevels,pGreediness,pMatchingResult) ) 

#define ICanAlgo_BodyDirectionDetectMinScoreAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BodyDirectionDetectMinScoreAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_DoubleEndRegionLocationAlgo(This,rgnId,pBottomRow,pRadiusWidth,pRadiusHeight,pRegionHeight,pRegionWidth)	\
    ( (This)->lpVtbl -> DoubleEndRegionLocationAlgo(This,rgnId,pBottomRow,pRadiusWidth,pRadiusHeight,pRegionHeight,pRegionWidth) ) 

#define ICanAlgo_DoubleEndRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DoubleEndRegionLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_DoubleEndDetectAlgo(This,rgnId,pSigmaSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> DoubleEndDetectAlgo(This,rgnId,pSigmaSize,pBlackThredLow,pBlackThredHigh,pBlackDefectSize,pDetectArea) ) 

#define ICanAlgo_DoubleEndDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DoubleEndDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_BodyDirectionDetectVecDiffAlgo(This,rgnId,pVecDiffValue)	\
    ( (This)->lpVtbl -> BodyDirectionDetectVecDiffAlgo(This,rgnId,pVecDiffValue) ) 

#define ICanAlgo_BodyDirectionDetectVecDiffAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BodyDirectionDetectVecDiffAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_BodyDirectionVecDiffLocationAlgo(This,rgnID)	\
    ( (This)->lpVtbl -> BodyDirectionVecDiffLocationAlgo(This,rgnID) ) 

#define ICanAlgo_BodyDirectionVecDiffLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BodyDirectionVecDiffLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgo(This,rgnId,pRadius,pRoiWidth) ) 

#define ICanAlgo_BoundaryRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_WeldWhiteGlueRegExtractAlgo(This,rgnId,pBlackMaskSize,pWeldRegArea)	\
    ( (This)->lpVtbl -> WeldWhiteGlueRegExtractAlgo(This,rgnId,pBlackMaskSize,pWeldRegArea) ) 

#define ICanAlgo_WeldWhiteGlueRegExtractAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldWhiteGlueRegExtractAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_WeldRegSelectAlgo(This,rgnId,pWeldSelection)	\
    ( (This)->lpVtbl -> WeldRegSelectAlgo(This,rgnId,pWeldSelection) ) 

#define ICanAlgo_WeldRegSelectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldRegSelectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlOptimizeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pFreq_start,pFreqCutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber)	\
    ( (This)->lpVtbl -> CurlOptimizeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pFreq_start,pFreqCutoff,pDefectType,pWaveThreshold,pMinArea,pDetectNumber) ) 

#define ICanAlgo_CurlOptimizeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlOptimizeForColorAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlDetectForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define ICanAlgo_CurlDetectForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectForColorAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlMeanGrayDetectForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pNumPart,pGrayValueLower,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlMeanGrayDetectForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pNumPart,pGrayValueLower,pGrayValue,pDetectArea) ) 

#define ICanAlgo_CurlMeanGrayDetectForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlMeanGrayDetectForColorAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define ICanAlgo_AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> AnnularRotaDynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlNoCompoundForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pGrayValueMean)	\
    ( (This)->lpVtbl -> CurlNoCompoundForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pGrayValueMean) ) 

#define ICanAlgo_CurlNoCompoundForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlNoCompoundForColorAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pBlackMaskSize,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pWhiteMaskSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define ICanAlgo_DynThresholdForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DynThresholdForColorAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pEdgeThred,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> EdgeForColorAlgo(This,rgnId,pColorTransType,pChannelSelect,pIsShowInterImg,pEdgeThred,pDefectSize,pDetectArea) ) 

#define ICanAlgo_EdgeForColorAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> EdgeForColorAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgo(This,rgnId,pMaskSize,pSmoothSize,pDefectType,pSeriousBlackPointDynThresh,pSeriousBlackPointSize,pSeriousWhitePointDynThresh,pSeriousWhitePointSize,pDetectArea) ) 

#define ICanAlgo_DerivateGaussDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DerivateGaussDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_VarThreshDarkDetectAlgo(This,rgnId,pWindowWidth,pGrayThresh,pAreaFilter,pDetectArea)	\
    ( (This)->lpVtbl -> VarThreshDarkDetectAlgo(This,rgnId,pWindowWidth,pGrayThresh,pAreaFilter,pDetectArea) ) 

#define ICanAlgo_VarThreshDarkDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> VarThreshDarkDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CurlDetectAlgo1(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea)	\
    ( (This)->lpVtbl -> CurlDetectAlgo1(This,rgnId,pNumPart,pThresholdValue,pGrayValue,pDetectArea) ) 

#define ICanAlgo_CurlDetectAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CurlDetectAlgo1Help(This,pHelpStr) ) 

#define ICanAlgo_CenterReLocationAlgo2(This,rgnID,pUsePolarity,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterReLocationAlgo2(This,rgnID,pUsePolarity,pRadius,pMinScore) ) 

#define ICanAlgo_CenterReLocationAlgo2Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterReLocationAlgo2Help(This,pHelpStr) ) 

#define ICanAlgo_RingRegionDynThresholdAlgoBW(This,rgnId,pDetectType,pMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> RingRegionDynThresholdAlgoBW(This,rgnId,pDetectType,pMaskSize,pBlackThredLow,pBlackDefectSize,pWhiteThredLow,pWhiteDefectSize,pDetectArea) ) 

#define ICanAlgo_RingRegionDynThresholdAlgoBWHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RingRegionDynThresholdAlgoBWHelp(This,pHelpStr) ) 

#define ICanAlgo_BoundaryRegionLocationAlgo1(This,rgnId,pClosingType,pRadius,pRoiWidth)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgo1(This,rgnId,pClosingType,pRadius,pRoiWidth) ) 

#define ICanAlgo_BoundaryRegionLocationAlgo1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> BoundaryRegionLocationAlgo1Help(This,pHelpStr) ) 

#define ICanAlgo_CenterLocationAlgoM(This,rgnId,pThreshValue)	\
    ( (This)->lpVtbl -> CenterLocationAlgoM(This,rgnId,pThreshValue) ) 

#define ICanAlgo_CenterLocationAlgoMHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoMHelp(This,pHelpStr) ) 

#define ICanAlgo_RectangleLocationAlgo(This,rgnId,pWidthPlaceSet,pRegionHeight,pRegionWidth)	\
    ( (This)->lpVtbl -> RectangleLocationAlgo(This,rgnId,pWidthPlaceSet,pRegionHeight,pRegionWidth) ) 

#define ICanAlgo_RectangleLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectangleLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_BaseLineDetectAlgo(This,rgnId,pThredLow,pElements,pTransition,pSelect,pSigma,pThreshold,pDefectArea)	\
    ( (This)->lpVtbl -> BaseLineDetectAlgo(This,rgnId,pThredLow,pElements,pTransition,pSelect,pSigma,pThreshold,pDefectArea) ) 

#define ICanAlgo_BaseLineDetectAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> BaseLineDetectAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_RectangleLocation2Algo(This,rgnId,pWidthPlaceSet,pHeightPlaceSet,pRegionHeight,pRegionWidth)	\
    ( (This)->lpVtbl -> RectangleLocation2Algo(This,rgnId,pWidthPlaceSet,pHeightPlaceSet,pRegionHeight,pRegionWidth) ) 

#define ICanAlgo_RectangleLocation2AlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectangleLocation2AlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_HeightMeasureAlgo(This,rgnId,pThredValue,pOffset,pDefectDiff)	\
    ( (This)->lpVtbl -> HeightMeasureAlgo(This,rgnId,pThredValue,pOffset,pDefectDiff) ) 

#define ICanAlgo_HeightMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> HeightMeasureAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_DiameterMeasureAlgo(This,rgnId,pThredValue,pElements,pTransition,pSelect,pSigma,pThreshold,pOffset,pDefectDiff)	\
    ( (This)->lpVtbl -> DiameterMeasureAlgo(This,rgnId,pThredValue,pElements,pTransition,pSelect,pSigma,pThreshold,pOffset,pDefectDiff) ) 

#define ICanAlgo_DiameterMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DiameterMeasureAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_InnerDiameterMeasureAlgo(This,rgnId,pSwitch,pOffset,pDefectDiff)	\
    ( (This)->lpVtbl -> InnerDiameterMeasureAlgo(This,rgnId,pSwitch,pOffset,pDefectDiff) ) 

#define ICanAlgo_InnerDiameterMeasureAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> InnerDiameterMeasureAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_SideCenterLocationAlgo(This,rgnID,pRowStart,pColStart,pRegWidth,pRegHeight,pThredValueUp)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgo(This,rgnID,pRowStart,pColStart,pRegWidth,pRegHeight,pThredValueUp) ) 

#define ICanAlgo_SideCenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideCenterLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_SideRegionLocationAlgo(This,rgnId,pMoveSize,pErosioSizeH,pErosioSizeV)	\
    ( (This)->lpVtbl -> SideRegionLocationAlgo(This,rgnId,pMoveSize,pErosioSizeH,pErosioSizeV) ) 

#define ICanAlgo_SideRegionLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SideRegionLocationAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_CenterLocationAlgoUsePointFitting1(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold,pDistThresh)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFitting1(This,rgnID,pSearchRegRow,pSearchRegCol,pOuterRadius,pRoiWidth,pScanLineNum,pScanLineDirection,pTransition,pPointSelection,pSigma,pThreshold,pDistThresh) ) 

#define ICanAlgo_CenterLocationAlgoUsePointFitting1Help(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoUsePointFitting1Help(This,pHelpStr) ) 

#define ICanAlgo_WeldRegionSelection(This,rgnId,pRadius,pRoiWidth,pWeldSelection,pRegionErosionSize)	\
    ( (This)->lpVtbl -> WeldRegionSelection(This,rgnId,pRadius,pRoiWidth,pWeldSelection,pRegionErosionSize) ) 

#define ICanAlgo_WeldRegionSelectionHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldRegionSelectionHelp(This,pHelpStr) ) 

#define ICanAlgo_WeldRegionExtractionAlgo(This,rgnId,pGrayThresh,pDetectArea)	\
    ( (This)->lpVtbl -> WeldRegionExtractionAlgo(This,rgnId,pGrayThresh,pDetectArea) ) 

#define ICanAlgo_WeldRegionExtractionAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> WeldRegionExtractionAlgoHelp(This,pHelpStr) ) 

#define ICanAlgo_SectorRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea)	\
    ( (This)->lpVtbl -> SectorRotaDynThresholdAlgo(This,rgnId,pRotaAngle,pDynThresh,pDefectSize,pDetectArea) ) 

#define ICanAlgo_SectorRotaDynThresholdAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> SectorRotaDynThresholdAlgoHelp(This,pHelpStr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_CenterLocationAlgoMHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_CenterLocationAlgoMHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_RectangleLocationAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pWidthPlaceSet,
    /* [in] */ VARIANT *pRegionHeight,
    /* [in] */ VARIANT *pRegionWidth);


void __RPC_STUB ICanAlgo_RectangleLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_RectangleLocationAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_RectangleLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_BaseLineDetectAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThredLow,
    /* [in] */ VARIANT *pElements,
    /* [in] */ VARIANT *pTransition,
    /* [in] */ VARIANT *pSelect,
    /* [in] */ VARIANT *pSigma,
    /* [in] */ VARIANT *pThreshold,
    /* [retval][out] */ VARIANT *pDefectArea);


void __RPC_STUB ICanAlgo_BaseLineDetectAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_BaseLineDetectAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_BaseLineDetectAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_RectangleLocation2Algo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pWidthPlaceSet,
    /* [in] */ VARIANT *pHeightPlaceSet,
    /* [in] */ VARIANT *pRegionHeight,
    /* [in] */ VARIANT *pRegionWidth);


void __RPC_STUB ICanAlgo_RectangleLocation2Algo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_RectangleLocation2AlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_RectangleLocation2AlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_HeightMeasureAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pOffset,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB ICanAlgo_HeightMeasureAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_HeightMeasureAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_HeightMeasureAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_DiameterMeasureAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pThredValue,
    /* [in] */ VARIANT *pElements,
    /* [in] */ VARIANT *pTransition,
    /* [in] */ VARIANT *pSelect,
    /* [in] */ VARIANT *pSigma,
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pOffset,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB ICanAlgo_DiameterMeasureAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_DiameterMeasureAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_DiameterMeasureAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_InnerDiameterMeasureAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pSwitch,
    /* [in] */ VARIANT *pOffset,
    /* [retval][out] */ VARIANT *pDefectDiff);


void __RPC_STUB ICanAlgo_InnerDiameterMeasureAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_InnerDiameterMeasureAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_InnerDiameterMeasureAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_SideCenterLocationAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnID,
    /* [in] */ VARIANT *pRowStart,
    /* [in] */ VARIANT *pColStart,
    /* [in] */ VARIANT *pRegWidth,
    /* [in] */ VARIANT *pRegHeight,
    /* [in] */ VARIANT *pThredValueUp);


void __RPC_STUB ICanAlgo_SideCenterLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_SideCenterLocationAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_SideCenterLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_SideRegionLocationAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pMoveSize,
    /* [in] */ VARIANT *pErosioSizeH,
    /* [in] */ VARIANT *pErosioSizeV);


void __RPC_STUB ICanAlgo_SideRegionLocationAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_SideRegionLocationAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_SideRegionLocationAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_CenterLocationAlgoUsePointFitting1_Proxy( 
    ICanAlgo * This,
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
    /* [in] */ VARIANT *pThreshold,
    /* [in] */ VARIANT *pDistThresh);


void __RPC_STUB ICanAlgo_CenterLocationAlgoUsePointFitting1_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_CenterLocationAlgoUsePointFitting1Help_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_CenterLocationAlgoUsePointFitting1Help_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_WeldRegionSelection_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRadius,
    /* [in] */ VARIANT *pRoiWidth,
    /* [in] */ VARIANT *pWeldSelection,
    /* [in] */ VARIANT *pRegionErosionSize);


void __RPC_STUB ICanAlgo_WeldRegionSelection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_WeldRegionSelectionHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_WeldRegionSelectionHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_WeldRegionExtractionAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pGrayThresh,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB ICanAlgo_WeldRegionExtractionAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_WeldRegionExtractionAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_WeldRegionExtractionAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_SectorRotaDynThresholdAlgo_Proxy( 
    ICanAlgo * This,
    /* [in] */ VARIANT *rgnId,
    /* [in] */ VARIANT *pRotaAngle,
    /* [in] */ VARIANT *pDynThresh,
    /* [in] */ VARIANT *pDefectSize,
    /* [retval][out] */ VARIANT *pDetectArea);


void __RPC_STUB ICanAlgo_SectorRotaDynThresholdAlgo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ HRESULT STDMETHODCALLTYPE ICanAlgo_SectorRotaDynThresholdAlgoHelp_Proxy( 
    ICanAlgo * This,
    /* [retval][out] */ BSTR *pHelpStr);


void __RPC_STUB ICanAlgo_SectorRotaDynThresholdAlgoHelp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICanAlgo_INTERFACE_DEFINED__ */



#ifndef __CanAlgoLib_LIBRARY_DEFINED__
#define __CanAlgoLib_LIBRARY_DEFINED__

/* library CanAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_CanAlgoLib;

EXTERN_C const CLSID CLSID_CanAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("C0C3D6C7-F6DB-4487-9C81-7086B68E5143")
CanAlgo;
#endif
#endif /* __CanAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


