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

    PCIrsrc.h
--*/

#ifndef _PCIRSRC_H_
#define _PCIRSRC_H_




//
// ConfigEntry commands
//
#define PCIBUS_CONFIG_RSRC 0
#define PCIBUS_CONFIG_SET 1
#define PCIBUS_CONFIG_SIZE 2
#define PCIBUS_CONFIG_INIT 3

typedef struct _PCI_CFG_INFO {
    HMODULE hDll;
    PVOID ConfigEntryFn;
    struct _PCI_CFG_INFO *Next;
} PCI_CFG_INFO, *PPCI_CFG_INFO;

typedef struct _PCI_RSRC {
    DWORD Bus;
    DWORD Device;
    DWORD Function;
    DWORD Offset;
    DWORD Base;
    DWORD Size;
    BOOL Bridge;
    DWORD SecBus;
    BOOL Placed;
    PPCI_CFG_INFO ConfigInfo;
    struct _PCI_RSRC *Next;
    struct _PCI_RSRC *Prev;
} PCI_RSRC, *PPCI_RSRC;

#ifdef __cplusplus
extern "C" {
#endif
//
// Function prototypes
//

PPCI_RSRC
PCIRsrc_New(
    DWORD Bus,
    DWORD Device,
    DWORD Function,
    DWORD Offset,
    DWORD Base,
    DWORD Size,
    BOOL Bridge,
    DWORD SecBus,
    BOOL Placed,
    PPCI_CFG_INFO ConfigInfo
    );

void
PCIRsrc_Add(
    PPCI_RSRC Head,
    PPCI_RSRC Rsrc
    );

PPCI_RSRC
PCIRsrc_GetNext(
    PPCI_RSRC Head,
    DWORD Bus
    );

BOOL
PCIRsrc_Place(
    PPCI_RSRC Head,
    PPCI_RSRC Rsrc
    );

void
PCIRsrc_DelList(
    PPCI_RSRC Head
    );

void
PCIRsrc_PrintList(
    PPCI_RSRC Head
    );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _PCIRSRC_H_
