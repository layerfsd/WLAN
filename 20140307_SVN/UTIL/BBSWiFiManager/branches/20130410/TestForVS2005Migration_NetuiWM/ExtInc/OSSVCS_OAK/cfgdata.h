//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//

#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Wed Oct 27 01:22:12 2004
 */
/* Compiler settings for .\cfgdata.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __cfgdata_h__
#define __cfgdata_h__

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_cfgdata_0000 */
/* [local] */ 

typedef 
enum ConfigDataType
    {	CFG_DATATYPE_INTEGER	= 0,
	CFG_DATATYPE_STRING	= CFG_DATATYPE_INTEGER + 1,
	CFG_DATATYPE_FLOAT	= CFG_DATATYPE_STRING + 1,
	CFG_DATATYPE_DATE	= CFG_DATATYPE_FLOAT + 1,
	CFG_DATATYPE_TIME	= CFG_DATATYPE_DATE + 1,
	CFG_DATATYPE_BOOLEAN	= CFG_DATATYPE_TIME + 1,
	CFG_DATATYPE_BINARY	= CFG_DATATYPE_BOOLEAN + 1,
	CFG_DATATYPE_MULTIPLE_STRING	= CFG_DATATYPE_BINARY + 1,
	CFG_DATATYPE_NODE	= CFG_DATATYPE_MULTIPLE_STRING + 1,
	CFG_DATATYPE_NULL	= CFG_DATATYPE_NODE + 1,
	CFG_DATATYPE_UNKNOWN	= CFG_DATATYPE_NULL + 1
    }	CFG_DATATYPE;

typedef enum ConfigDataType __RPC_FAR *PCFG_DATATYPE;



extern RPC_IF_HANDLE __MIDL_itf_cfgdata_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_cfgdata_0000_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


