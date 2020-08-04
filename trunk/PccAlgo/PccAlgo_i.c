

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Dec 26 09:17:16 2019
 */
/* Compiler settings for PccAlgo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IPccAlgo,0x2855E408,0xC382,0x48B6,0xA5,0xCA,0xD5,0x5F,0xE0,0xB5,0x03,0x62);


MIDL_DEFINE_GUID(IID, IID_ISelfLearning,0x7EE73789,0x334E,0x45C6,0x86,0x05,0x02,0x67,0x55,0x66,0x42,0x82);


MIDL_DEFINE_GUID(IID, LIBID_PccAlgoLib,0x8F17DEFB,0x4561,0x4FAC,0x97,0x7A,0x14,0x5B,0x3A,0x62,0x0B,0xF5);


MIDL_DEFINE_GUID(CLSID, CLSID_PccAlgo,0xB6526FAD,0x08FA,0x46AE,0xB3,0xDB,0xCE,0xBC,0x39,0x70,0xD3,0x4B);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



