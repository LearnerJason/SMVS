

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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

MIDL_DEFINE_GUID(IID, IID_IHdpeAlgo,0xD823E8B8,0x60C3,0x4011,0xBE,0x03,0xE9,0x1A,0x09,0xE3,0x2F,0x20);


MIDL_DEFINE_GUID(IID, IID_ISelfLearning,0x03FE5233,0x3BF3,0x4358,0xAA,0x95,0x61,0xE0,0xD4,0xAD,0x41,0x24);


MIDL_DEFINE_GUID(IID, LIBID_HdpeAlgoLib,0xA05D70C6,0xC41C,0x4F4F,0xA8,0x6E,0x45,0x7D,0x94,0x29,0xA9,0x07);


MIDL_DEFINE_GUID(CLSID, CLSID_HdpeAlgo,0x742148EA,0xEFB5,0x4125,0x92,0xD6,0xF0,0x99,0xE7,0x36,0x68,0x7E);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



