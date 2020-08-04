

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:16:56 2019
 */
/* Compiler settings for JellyAlgo.idl:
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

#ifndef __JellyAlgo_i_h__
#define __JellyAlgo_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IJellyAlgo_FWD_DEFINED__
#define __IJellyAlgo_FWD_DEFINED__
typedef interface IJellyAlgo IJellyAlgo;
#endif 	/* __IJellyAlgo_FWD_DEFINED__ */


#ifndef __JellyAlgo_FWD_DEFINED__
#define __JellyAlgo_FWD_DEFINED__

#ifdef __cplusplus
typedef class JellyAlgo JellyAlgo;
#else
typedef struct JellyAlgo JellyAlgo;
#endif /* __cplusplus */

#endif 	/* __JellyAlgo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IJellyAlgo_INTERFACE_DEFINED__
#define __IJellyAlgo_INTERFACE_DEFINED__

/* interface IJellyAlgo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IJellyAlgo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19B6B16D-E776-462B-9787-E962E11AC5DC")
    IJellyAlgo : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IJellyAlgoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IJellyAlgo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IJellyAlgo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IJellyAlgo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IJellyAlgo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IJellyAlgo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IJellyAlgo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IJellyAlgo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IJellyAlgoVtbl;

    interface IJellyAlgo
    {
        CONST_VTBL struct IJellyAlgoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IJellyAlgo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IJellyAlgo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IJellyAlgo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IJellyAlgo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IJellyAlgo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IJellyAlgo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IJellyAlgo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IJellyAlgo_INTERFACE_DEFINED__ */



#ifndef __JellyAlgoLib_LIBRARY_DEFINED__
#define __JellyAlgoLib_LIBRARY_DEFINED__

/* library JellyAlgoLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_JellyAlgoLib;

EXTERN_C const CLSID CLSID_JellyAlgo;

#ifdef __cplusplus

class DECLSPEC_UUID("A7066785-70AF-4C33-BE9F-F34057E4574B")
JellyAlgo;
#endif
#endif /* __JellyAlgoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


