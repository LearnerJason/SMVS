

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:16:41 2019
 */
/* Compiler settings for CanOutsideAlgo.idl:
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

#ifndef __CanOutsideAlgo_i_h__
#define __CanOutsideAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICanOutsideAlgo_FWD_DEFINED__
#define __ICanOutsideAlgo_FWD_DEFINED__
typedef interface ICanOutsideAlgo ICanOutsideAlgo;
#endif 	/* __ICanOutsideAlgo_FWD_DEFINED__ */


#ifndef __CanOutsideAlgo_FWD_DEFINED__
#define __CanOutsideAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class CanOutsideAlgo CanOutsideAlgo;
#else
typedef struct CanOutsideAlgo CanOutsideAlgo;
#endif /* __cplusplus */

#endif 	/* __CanOutsideAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICanOutsideAlgo_INTERFACE_DEFINED__
#define __ICanOutsideAlgo_INTERFACE_DEFINED__

/* interface ICanOutsideAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICanOutsideAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6214CB4A-7672-436E-B883-196C2265DB85")
    ICanOutsideAlgo : public IDispatch
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
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetChannelIndex( 
            /* [in] */ LONG nChIndex) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE ReceivePrerequisiteParam( 
            /* [in] */ LONG *pCameraIndex,
            /* [in] */ LONG *pHomMat2DForFlag,
            /* [in] */ LONG *pLeftCol4ImgsInStd,
            /* [in] */ LONG *pRightCol4ImgsInStd) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetCommonParameters( 
            /* [in] */ LONG *pStandardImg,
            /* [in] */ LONG *pFlagModelRegion,
            /* [in] */ LONG *pSeamRegionInStdImg,
            /* [in] */ LONG *pStdModelRegion,
            /* [in] */ LONG *pStitchedImgWidthUnion,
            /* [in] */ LONG *pModelIDs,
            /* [in] */ LONG *pModelRows,
            /* [in] */ LONG *pModelCols,
            /* [in] */ LONG *pFlagModelID,
            /* [in] */ LONG *pSortedFlagColsInStdImg,
            /* [in] */ LONG *pStichingParamCol,
            /* [in] */ LONG *pCanRegWidth,
            /* [in] */ LONG *pMappedImgWidths) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetDetectROI( 
            /* [in] */ LONG *pDetectRoi) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE SetPreprocessedImg( 
            /* [in] */ LONG *pMappedImg,
            /* [in] */ LONG *pMappedGrayImg) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgo( 
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE CenterLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleLocationAlgo( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *centerRow,
            /* [in] */ VARIANT *centerCol,
            /* [in] */ VARIANT *nHalfWid,
            /* [in] */ VARIANT *nHalfHeight) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE RectangleLocationAlgoHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectError( 
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFlatRgnThresh,
            /* [in] */ VARIANT *pFlatRgnMinArea,
            /* [in] */ VARIANT *pTextRgnThresh,
            /* [in] */ VARIANT *pTextRgnMinArea,
            /* [retval][out] */ VARIANT *pDetectArea) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE DetectErrorHelp( 
            /* [retval][out] */ BSTR *pHelpStr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICanOutsideAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICanOutsideAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICanOutsideAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICanOutsideAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICanOutsideAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICanOutsideAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICanOutsideAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICanOutsideAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentImage )( 
            ICanOutsideAlgo * This,
            /* [in] */ LONG *pImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetRegion )( 
            ICanOutsideAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetErrorRegion )( 
            ICanOutsideAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [out][in] */ LONG *pErrorRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetInternalStats )( 
            ICanOutsideAlgo * This,
            /* [in] */ BYTE rgnID,
            /* [in] */ LONG nMaxLen,
            /* [size_is][out] */ FLOAT *pStatsArray,
            /* [out][in] */ LONG *nActualLen);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReloadExternalFile )( 
            ICanOutsideAlgo * This,
            /* [in] */ BSTR *bstrPath);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrentTaskName )( 
            ICanOutsideAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningParam )( 
            ICanOutsideAlgo * This,
            /* [in] */ DWORD sampleSz,
            /* [in] */ BSTR *bstrParam);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SelectModelImage )( 
            ICanOutsideAlgo * This,
            /* [in] */ LONG *pCrtImg,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteModelImage )( 
            ICanOutsideAlgo * This,
            /* [in] */ BYTE nIndex,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SaveResult )( 
            ICanOutsideAlgo * This,
            /* [in] */ BSTR *strModelName,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *AddModelImage )( 
            ICanOutsideAlgo * This,
            /* [in] */ LONG *pImg,
            /* [in] */ BSTR *strFolder,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningResult )( 
            ICanOutsideAlgo * This,
            /* [out][in] */ LONG *multiModelImag,
            /* [out][in] */ long *cMax,
            /* [out][in] */ float *vImgScore,
            /* [in] */ BYTE productID);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningRegion )( 
            ICanOutsideAlgo * This,
            /* [in] */ BSTR *strRegionExtractPara);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *GetSelfLearningRegion )( 
            ICanOutsideAlgo * This,
            /* [out][in] */ LONG *pRgn);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ResetSelfLearningProcedure )( 
            ICanOutsideAlgo * This);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetSelfLearningTargetTaskName )( 
            ICanOutsideAlgo * This,
            /* [in] */ BSTR *bstrTaskName);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetChannelIndex )( 
            ICanOutsideAlgo * This,
            /* [in] */ LONG nChIndex);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *ReceivePrerequisiteParam )( 
            ICanOutsideAlgo * This,
            /* [in] */ LONG *pCameraIndex,
            /* [in] */ LONG *pHomMat2DForFlag,
            /* [in] */ LONG *pLeftCol4ImgsInStd,
            /* [in] */ LONG *pRightCol4ImgsInStd);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetCommonParameters )( 
            ICanOutsideAlgo * This,
            /* [in] */ LONG *pStandardImg,
            /* [in] */ LONG *pFlagModelRegion,
            /* [in] */ LONG *pSeamRegionInStdImg,
            /* [in] */ LONG *pStdModelRegion,
            /* [in] */ LONG *pStitchedImgWidthUnion,
            /* [in] */ LONG *pModelIDs,
            /* [in] */ LONG *pModelRows,
            /* [in] */ LONG *pModelCols,
            /* [in] */ LONG *pFlagModelID,
            /* [in] */ LONG *pSortedFlagColsInStdImg,
            /* [in] */ LONG *pStichingParamCol,
            /* [in] */ LONG *pCanRegWidth,
            /* [in] */ LONG *pMappedImgWidths);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetDetectROI )( 
            ICanOutsideAlgo * This,
            /* [in] */ LONG *pDetectRoi);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *SetPreprocessedImg )( 
            ICanOutsideAlgo * This,
            /* [in] */ LONG *pMappedImg,
            /* [in] */ LONG *pMappedGrayImg);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgo )( 
            ICanOutsideAlgo * This,
            /* [in] */ VARIANT *rgnID,
            /* [in] */ VARIANT *pRadius,
            /* [in] */ VARIANT *pMinScore);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *CenterLocationAlgoHelp )( 
            ICanOutsideAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleLocationAlgo )( 
            ICanOutsideAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *centerRow,
            /* [in] */ VARIANT *centerCol,
            /* [in] */ VARIANT *nHalfWid,
            /* [in] */ VARIANT *nHalfHeight);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *RectangleLocationAlgoHelp )( 
            ICanOutsideAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectError )( 
            ICanOutsideAlgo * This,
            /* [in] */ VARIANT *rgnId,
            /* [in] */ VARIANT *pFlatRgnThresh,
            /* [in] */ VARIANT *pFlatRgnMinArea,
            /* [in] */ VARIANT *pTextRgnThresh,
            /* [in] */ VARIANT *pTextRgnMinArea,
            /* [retval][out] */ VARIANT *pDetectArea);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *DetectErrorHelp )( 
            ICanOutsideAlgo * This,
            /* [retval][out] */ BSTR *pHelpStr);
        
        END_INTERFACE
    } ICanOutsideAlgoVtbl;

    interface ICanOutsideAlgo
    {
        CONST_VTBL struct ICanOutsideAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICanOutsideAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICanOutsideAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICanOutsideAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICanOutsideAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICanOutsideAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICanOutsideAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICanOutsideAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICanOutsideAlgo_SetCurrentImage(This,pImg)	\
    ( (This)->lpVtbl -> SetCurrentImage(This,pImg) ) 

#define ICanOutsideAlgo_GetRegion(This,rgnID,pRgn)	\
    ( (This)->lpVtbl -> GetRegion(This,rgnID,pRgn) ) 

#define ICanOutsideAlgo_GetErrorRegion(This,rgnID,pErrorRgn)	\
    ( (This)->lpVtbl -> GetErrorRegion(This,rgnID,pErrorRgn) ) 

#define ICanOutsideAlgo_GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen)	\
    ( (This)->lpVtbl -> GetInternalStats(This,rgnID,nMaxLen,pStatsArray,nActualLen) ) 

#define ICanOutsideAlgo_ReloadExternalFile(This,bstrPath)	\
    ( (This)->lpVtbl -> ReloadExternalFile(This,bstrPath) ) 

#define ICanOutsideAlgo_SetCurrentTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetCurrentTaskName(This,bstrTaskName) ) 

#define ICanOutsideAlgo_SetSelfLearningParam(This,sampleSz,bstrParam)	\
    ( (This)->lpVtbl -> SetSelfLearningParam(This,sampleSz,bstrParam) ) 

#define ICanOutsideAlgo_SelectModelImage(This,pCrtImg,productID)	\
    ( (This)->lpVtbl -> SelectModelImage(This,pCrtImg,productID) ) 

#define ICanOutsideAlgo_DeleteModelImage(This,nIndex,productID)	\
    ( (This)->lpVtbl -> DeleteModelImage(This,nIndex,productID) ) 

#define ICanOutsideAlgo_SaveResult(This,strModelName,productID)	\
    ( (This)->lpVtbl -> SaveResult(This,strModelName,productID) ) 

#define ICanOutsideAlgo_AddModelImage(This,pImg,strFolder,productID)	\
    ( (This)->lpVtbl -> AddModelImage(This,pImg,strFolder,productID) ) 

#define ICanOutsideAlgo_GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID)	\
    ( (This)->lpVtbl -> GetSelfLearningResult(This,multiModelImag,cMax,vImgScore,productID) ) 

#define ICanOutsideAlgo_ResetSelfLearningRegion(This,strRegionExtractPara)	\
    ( (This)->lpVtbl -> ResetSelfLearningRegion(This,strRegionExtractPara) ) 

#define ICanOutsideAlgo_GetSelfLearningRegion(This,pRgn)	\
    ( (This)->lpVtbl -> GetSelfLearningRegion(This,pRgn) ) 

#define ICanOutsideAlgo_ResetSelfLearningProcedure(This)	\
    ( (This)->lpVtbl -> ResetSelfLearningProcedure(This) ) 

#define ICanOutsideAlgo_SetSelfLearningTargetTaskName(This,bstrTaskName)	\
    ( (This)->lpVtbl -> SetSelfLearningTargetTaskName(This,bstrTaskName) ) 

#define ICanOutsideAlgo_SetChannelIndex(This,nChIndex)	\
    ( (This)->lpVtbl -> SetChannelIndex(This,nChIndex) ) 

#define ICanOutsideAlgo_ReceivePrerequisiteParam(This,pCameraIndex,pHomMat2DForFlag,pLeftCol4ImgsInStd,pRightCol4ImgsInStd)	\
    ( (This)->lpVtbl -> ReceivePrerequisiteParam(This,pCameraIndex,pHomMat2DForFlag,pLeftCol4ImgsInStd,pRightCol4ImgsInStd) ) 

#define ICanOutsideAlgo_SetCommonParameters(This,pStandardImg,pFlagModelRegion,pSeamRegionInStdImg,pStdModelRegion,pStitchedImgWidthUnion,pModelIDs,pModelRows,pModelCols,pFlagModelID,pSortedFlagColsInStdImg,pStichingParamCol,pCanRegWidth,pMappedImgWidths)	\
    ( (This)->lpVtbl -> SetCommonParameters(This,pStandardImg,pFlagModelRegion,pSeamRegionInStdImg,pStdModelRegion,pStitchedImgWidthUnion,pModelIDs,pModelRows,pModelCols,pFlagModelID,pSortedFlagColsInStdImg,pStichingParamCol,pCanRegWidth,pMappedImgWidths) ) 

#define ICanOutsideAlgo_SetDetectROI(This,pDetectRoi)	\
    ( (This)->lpVtbl -> SetDetectROI(This,pDetectRoi) ) 

#define ICanOutsideAlgo_SetPreprocessedImg(This,pMappedImg,pMappedGrayImg)	\
    ( (This)->lpVtbl -> SetPreprocessedImg(This,pMappedImg,pMappedGrayImg) ) 

#define ICanOutsideAlgo_CenterLocationAlgo(This,rgnID,pRadius,pMinScore)	\
    ( (This)->lpVtbl -> CenterLocationAlgo(This,rgnID,pRadius,pMinScore) ) 

#define ICanOutsideAlgo_CenterLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> CenterLocationAlgoHelp(This,pHelpStr) ) 

#define ICanOutsideAlgo_RectangleLocationAlgo(This,rgnId,centerRow,centerCol,nHalfWid,nHalfHeight)	\
    ( (This)->lpVtbl -> RectangleLocationAlgo(This,rgnId,centerRow,centerCol,nHalfWid,nHalfHeight) ) 

#define ICanOutsideAlgo_RectangleLocationAlgoHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> RectangleLocationAlgoHelp(This,pHelpStr) ) 

#define ICanOutsideAlgo_DetectError(This,rgnId,pFlatRgnThresh,pFlatRgnMinArea,pTextRgnThresh,pTextRgnMinArea,pDetectArea)	\
    ( (This)->lpVtbl -> DetectError(This,rgnId,pFlatRgnThresh,pFlatRgnMinArea,pTextRgnThresh,pTextRgnMinArea,pDetectArea) ) 

#define ICanOutsideAlgo_DetectErrorHelp(This,pHelpStr)	\
    ( (This)->lpVtbl -> DetectErrorHelp(This,pHelpStr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICanOutsideAlgo_INTERFACE_DEFINED__ */



#ifndef __CanOutsideAlgoLib_LIBRARY_DEFINED__
#define __CanOutsideAlgoLib_LIBRARY_DEFINED__

/* library CanOutsideAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_CanOutsideAlgoLib;

EXTERN_C const CLSID CLSID_CanOutsideAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("C7CF32D0-5A06-4172-B2D8-6A715C63657F")
CanOutsideAlgo;
#endif
#endif /* __CanOutsideAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


