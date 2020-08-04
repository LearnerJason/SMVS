

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, IID_IPlugAlgo,0xAECCBEBA,0x779E,0x47E3,0xA8,0xA8,0x3C,0x2C,0xB7,0x39,0x0D,0x90);


MIDL_DEFINE_GUID(IID, IID_ISelfLearning,0xFFACCB7D,0x68BF,0x4C20,0x9C,0x56,0x0E,0xDB,0xDD,0xE7,0x3B,0xFD);


MIDL_DEFINE_GUID(IID, LIBID_PlugAlgoLib,0x6E14EFAD,0x740B,0x49A4,0xB5,0x9B,0x11,0x7A,0x25,0xF0,0xAD,0x48);


MIDL_DEFINE_GUID(CLSID, CLSID_PlugAlgo,0xC2AF78CC,0x9CC8,0x466B,0x9B,0x55,0x3E,0xB0,0x19,0xE2,0x77,0x00);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



