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
#ifndef _PCIREG_H
#define _PCIREG_H

#include <devload.h>
#include <ceddk.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PCI_REG_LIST {
    DWORD Num;
    DWORD Reg[PCI_TYPE0_ADDRESSES];
} PCI_REG_LIST, *PPCI_REG_LIST;

typedef struct _PCI_REG_INFO {
    WCHAR RegPath[DEVKEY_LEN];
    DWORD Bus;
    DWORD Device;
    DWORD Function;
    PCI_COMMON_CONFIG Cfg;
    PCI_REG_LIST MemBase;
    PCI_REG_LIST MemLen;
    PCI_REG_LIST IoBase;
    PCI_REG_LIST IoLen;
    DWORD SysIntr;
} PCI_REG_INFO, *PPCI_REG_INFO;

BOOL
PCIReg(
    PPCI_REG_INFO pInfo
    );

void
PCIInitInfo(
    LPCWSTR RegPath,
    DWORD Bus,
    DWORD Device,
    DWORD Function,
    DWORD SysIntr,
    PPCI_COMMON_CONFIG pCfg,
    PPCI_REG_INFO pInfo
    );
#ifdef __cplusplus
};
#endif

#endif // _PCIREG_H
