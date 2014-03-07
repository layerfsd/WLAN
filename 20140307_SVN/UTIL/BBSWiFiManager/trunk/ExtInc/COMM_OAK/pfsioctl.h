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
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:

    pfsioctl.h

Abstract:

    This module contains the constant, type and structure definitions for
    private file system control codes.

Notes:


--*/
#include <winioctl.h>

#ifndef _PFSIOCTL_H_
#define _PFSIOCTL_H_

#ifdef __cplusplus
extern "C" {
#endif

//
// The following FSCTL-s are for external volume replication
//

// Direction  : Volume -> Host
// Description: This FSCTL allows an external volume to register as a
//              replication host
#define FSCTL_EXTREPL_REGISTER_HOST \
    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 37, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Direction  : Host -> Volume
// Description: This FSCTL allows filesys.exe to instruct the replication
//              host to perform a replication action.
#define FSCTL_EXTREPL_EXECUTE_COMMAND \
    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 38, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Direction  : Volume -> Host
// Description: This FSCTL allows the replication to instruct filesys.exe to
//              issue a replication notification.
#define FSCTL_EXTREPL_SEND_NOTIFICATION \
    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 39, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Direction  : Host -> Volume
// Description: This FSCTL allows filesys.exe to inform a replication host that
//              it has completed initialization and to request the GUID of the
//              replication store so that it can filter internal notifications
//              regarding changes to the replication store.
#define FSCTL_EXTREPL_START \
    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 40, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Direction  : Host -> Volume
// Description: This FSCTL allows filesys.exe to instruct a replication host
//              that it is to stop replicating.
#define FSCTL_EXTREPL_STOP \
    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 41, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// The initialization of external replication occurs in two (2) stages.  The
// target storage volume becomes available before the replication store can
// be used, because CEDB depends on InitLocale to complete.  So, filesys.exe
// has to explicitly inform an external volume (FSD/FSF) that it can start
// replicating.  Moreover, an FSD/FSF can't start replicating until it registers
// its replication store with filesys.exe, because filesys.exe has to discard
// notifications regarding the replicaton store.
//
// Stage 1
//     - FSD/FSF completes initialization;
//     - FSD/FSF sends filesys.exe FSCTL_EXTREPL_REGISTER_HOST to register;
//       itself as the external replication host (and so filesys.exe can send
//       it a subsequen FSCT; and,
//     - [in response to FSCTL_REGISTER_HOST] filesys.exe returns the OID mask
//       (type) the external volume is to apply to its artificial OIDs and the
//       OID of its root directory.
//
// Stage 2
//     - filesys.exe completes InitLocale;
//     - filesys.exe sends registered replication host FSCTL_EXTREPL_START to
//       inform replicatio host that it can start replicating; and,
//     - [in response to FSCTL_EXTREPL_START] replication host returns GUID of
//       replication store.
//

//
// Structures for FSCTL_EXTREPL_REGISTER_HOST
//

// Input structure
typedef struct _EXTREPL_HOST {
    BOOL   fRegister;                          // has the replication store been registered?
    TCHAR  lpszTargetStorageVolName[MAX_PATH]; // target storage volume name
} EXTREPL_HOST, * LPEXTREPL_HOST;

// Output structure
typedef struct _EXTREPL_HOST_RETURN {
    DWORD dwOIDType;          // high 4-bits of OID type
    CEOID ceoidRootDirectory; // OID of root directory
} EXTREPL_HOST_RETURN, * LPEXTREPL_HOST_RETURN;

//
// Structures and defines for FSCTL_EXTREPL_EXECUTE_COMMAND
//

// External replication command IDs
#define EXTREPL_CID_CEOIDGETINFOEX2         0x00000001 // CeOidGetInfoEx2
#define EXTREPL_CID_CEGETREPLCHANGEBITSEX   0x00000002 // CeGetReplChangeBitsEx
#define EXTREPL_CID_CECLEARREPLCHANGEBITSEX 0x00000003 // CeClearReplChangeBitsEx
#define EXTREPL_CID_CESETREPLCHANGEBITSEX   0x00000004 // CeSetReplChangeBitsEx
#define EXTREPL_CID_CEGETREPLOTHERBITSEX    0x00000005 // CeGetReplOtherBitsEx
#define EXTREPL_CID_CESETREPLOTHERBITSEX    0x00000006 // CeSetReplOtherBitsEx

// Generic external replication command.  In general, [in] arguments are placed
// in the argument packet and [out] arguments are placed in the return packet.
// An [in/out] argument should have a member in both the argument packet and the
// return packet.
typedef struct _EXTREPL_COMMAND {
    DWORD  dwCid; // command ID
    DWORD  cbAp;  // size of argument packet
    LPVOID lpvAp; // argument packet
} EXTREPL_COMMAND, * LPEXTREPL_COMMAND;

// Argument and return packets for CeOidGetInfoEx2
typedef struct _EXTREPL_AP_CEOIDGETINFOEX2 {
    CEGUID ceguid;
    CEOID  ceoid;
} EXTREPL_AP_CEOIDGETINFOEX2, * LPEXTREPL_AP_CEOIDGETINFOEX2;

typedef struct _EXTREPL_RP_CEOIDGETINFOEX2 {
    CEOIDINFOEX ceoidinfoex;
    BOOL        fReturn;
} EXTREPL_RP_CEOIDGETINFOEX2, * LPEXTREPL_RP_CEOIDGETINFOEX2;

// Argument and return packets for CeGetReplChangeBitsEx
typedef struct _EXTREPL_AP_CEGETREPLCHANGEBITSEX {
    CEGUID ceguid;
    CEOID  ceoid;
    DWORD  dwFlags;
} EXTREPL_AP_CEGETREPLCHANGEBITSEX, * LPEXTREPL_AP_CEGETREPLCHANGEBITSEX;

typedef struct _EXTREPL_RP_CEGETREPLCHANGEBITSEX {
    DWORD dwBits;
    BOOL  fReturn;
} EXTREPL_RP_CEGETREPLCHANGEBITSEX, * LPEXTREPL_RP_CEGETREPLCHANGEBITSEX;

// Argument and return packets for CeClearReplChangeBitsEx
typedef struct _EXTREPL_AP_CECLEARREPLCHANGEBITSEX {
    CEGUID ceguid;
    CEOID  ceoid;
    DWORD  dwMask;
} EXTREPL_AP_CECLEARREPLCHANGEBITSEX, * LPEXTREPL_AP_CECLEARREPLCHANGEBITSEX;

typedef struct _EXTREPL_RP_CECLEARREPLCHANGEBITSEX {
    BOOL fReturn;
} EXTREPL_RP_CECLEARREPLCHANGEBITSEX, * LPEXTREPL_RP_CECLEARREPLCHANGEBITSEX;

// Argument and return packets for CeSetReplChangeBitsEx
typedef struct _EXTREPL_AP_CECSETREPLCHANGEBITSEX {
    CEGUID ceguid;
    CEOID  ceoid;
    DWORD  dwMask;
} EXTREPL_AP_CESETREPLCHANGEBITSEX, * LPEXTREPL_AP_CESETREPLCHANGEBITSEX;

typedef struct _EXTREPL_RP_CESETREPLCHANGEBITSEX {
    BOOL fReturn;
} EXTREPL_RP_CESETREPLCHANGEBITSEX, * LPEXTREPL_RP_CESETREPLCHANGEBITSEX;

// Argument and return packets for CeGetReplOtherBitsEx
typedef struct _EXTREPL_AP_CEGETREPLOTHERBITSEX {
    CEGUID ceguid;
    CEOID  ceoid;
} EXTREPL_AP_CEGETREPLOTHERBITSEX, * LPEXTREPL_AP_CEGETREPLOTHERBITSEX;

typedef struct _EXTREPL_RP_CEGETREPLOTHERBITSEX {
    DWORD dwBits;
    BOOL  fReturn;
} EXTREPL_RP_CEGETREPLOTHERBITSEX, * LPEXTREPL_RP_CEGETREPLOTHERBITSEX;

// Argument and return packets for CeSetReplOtherBitsEx
typedef struct _EXTREPL_AP_CESETREPLOTHERBITSEX {
    CEGUID ceguid;
    CEOID  ceoid;
    DWORD  dwBits;
} EXTREPL_AP_CESETREPLOTHERBITSEX, * LPEXTREPL_AP_CESETREPLOTHERBITSEX;

typedef struct _EXTREPL_RP_CESETREPLOTHERBITSEX {
    BOOL  fReturn;
} EXTREPL_RP_CESETREPLOTHERBITSEX, * LPEXTREPL_RP_CESETREPLOTHERBITSEX;

//
// Structures for FSCTL_EXTREPL_SEND_NOTIFICATION
//

// Input structure
typedef struct _EXTREPL_NOTIFICATION {
    HPROCESS       hProcess;
    CENOTIFICATION cenotification;
} EXTREPL_NOTIFICATION, *LPEXTREPL_NOTIFICATION;

// No output structure

//
// Structures for FSCTL_EXTREPL_START
//

// No input structure

// Output structure
typedef struct _EXTREPL_START_RETURN {
    CEGUID ceguid; // GUID of replication store
} EXTREPL_START_RETURN, *LPEXTREPL_START_RETURN;

//
// Structures for FSCTL_EXTREPL_STOP
//

// No input structure

// No output structure

#ifdef __cplusplus
}
#endif

#endif // _PFSIOCTL_H_
