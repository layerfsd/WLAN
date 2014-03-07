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
/*++


Module Name:

    rpcnsip.h

Abstract:

    This file contains the types and function definitions to use the
    to implement the autohandle features of the runtime.

--*/

#ifndef __RPCNSIP_H__
#define __RPCNSIP_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   RPC_NS_HANDLE        LookupContext;
   RPC_BINDING_HANDLE   ProposedHandle;
   RPC_BINDING_VECTOR * Bindings;

} RPC_IMPORT_CONTEXT_P, * PRPC_IMPORT_CONTEXT_P;


/* Stub Auto Binding routines. */

RPC_STATUS RPC_ENTRY
I_RpcNsGetBuffer(
    IN PRPC_MESSAGE Message
    );

RPC_STATUS RPC_ENTRY
I_RpcNsSendReceive(
    IN PRPC_MESSAGE Message,
    OUT RPC_BINDING_HANDLE __RPC_FAR * Handle
    );

void RPC_ENTRY
I_RpcNsRaiseException(
    IN PRPC_MESSAGE Message,
    IN RPC_STATUS Status
    );

RPC_STATUS RPC_ENTRY
I_RpcReBindBuffer(
    IN PRPC_MESSAGE Message
    );

RPC_STATUS RPC_ENTRY
I_NsServerBindSearch(
    );

RPC_STATUS RPC_ENTRY
I_NsClientBindSearch(
    );

void RPC_ENTRY
I_NsClientBindDone(
    );

#ifdef __cplusplus
}
#endif

#endif /* __RPCNSIP_H__ */
