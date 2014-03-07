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

//
// This file contains prototypes for PCI helper functions.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

#include <pkfuncs.h>
#include <devload.h>
#include <giisr.h>

#define	MAX_DEVICE_WINDOWS				6

	// this structure describes a base/length pair
typedef struct _DEVICEWINDOW_tag {
	DWORD dwBase;					// window base address
	DWORD dwLen;					// window
} DEVICEWINDOW, *PDEVICEWINDOW;

// this structure contains address window information
typedef struct _DDKWINDOWINFO_tag {
	DWORD	cbSize;					// size of this structure
	DWORD	dwBusNumber;			// PCI bus number
	DWORD	dwInterfaceType;		// INTERFACE_TYPE value (probably PCI == 5)
	DWORD	dwNumIoWindows;			// number of I/O windows
	DEVICEWINDOW ioWindows[MAX_DEVICE_WINDOWS];
	DWORD	dwNumMemWindows;		// number of memory windows
	DEVICEWINDOW memWindows[MAX_DEVICE_WINDOWS];
} DDKWINDOWINFO, *PDDKWINDOWINFO;

// This value indicates no IRQ was specified
#define	IRQ_UNSPECIFIED						0xFFFFFFFF

// this structure contains ISR information
typedef struct _DDKISRINFO_tag {
	DWORD	cbSize;					// size of this structure
	DWORD	dwIrq;					// interrupt number
	DWORD	dwSysintr;				// windows CE interrupt ID
    WCHAR	szIsrDll[DEVDLL_LEN];	// installable ISR dll
    WCHAR	szIsrHandler[DEVENTRY_LEN];	// installable ISR dll entry point
} DDKISRINFO, *PDDKISRINFO;

// ID word indices
#define	PCIID_CLASS							0
#define	PCIID_SUBCLASS						1
#define	PCIID_PROGIF						2
#define	PCIID_VENDORID						3
#define	PCIID_DEVICEID						4
#define	PCIID_REVISIONID					5
#define	PCIID_SUBSYSTEMVENDORID				6
#define	PCIID_SUBSYSTEMID					7
#define	PCIID_MAXNUMIDS						(PCIID_SUBSYSTEMID + 1)

// ID word masks
#define	PCIIDM_CLASS						(1 << PCIID_CLASS)
#define	PCIIDM_SUBCLASS						(1 << PCIID_SUBCLASS)
#define	PCIIDM_PROGIF						(1 << PCIID_PROGIF)
#define	PCIIDM_VENDORID						(1 << PCIID_VENDORID)
#define	PCIIDM_DEVICEID						(1 << PCIID_DEVICEID)
#define	PCIIDM_REVISIONID					(1 << PCIID_REVISIONID)
#define	PCIIDM_SUBSYSTEMVENDORID			(1 << PCIID_SUBSYSTEMVENDORID)
#define	PCIIDM_SUBSYSTEMID					(1 << PCIID_SUBSYSTEMID)

// this structure keeps track of standard PCI device instance information
typedef struct _DDKPCIINFO_tag {
	DWORD	cbSize;					// size of this structure

	// instancing information
	DWORD	dwDeviceNumber;
	DWORD	dwFunctionNumber;
	DWORD	dwInstanceIndex;

	// device ID information
	DWORD	dwWhichIds;				// idVals[PCIID_XXX] present if (dwWhichIds & (1 << PCIID_XXX)) != 0
	DWORD	idVals[PCIID_MAXNUMIDS]; // class, subclass, etc, populated as described in dwWhichIds
} DDKPCIINFO, *PDDKPCIINFO;

// function prototypes
DWORD WINAPI DDKReg_GetWindowInfo(HKEY hk, PDDKWINDOWINFO pwi);
DWORD WINAPI DDKReg_GetIsrInfo(HKEY hk, PDDKISRINFO pii);
DWORD WINAPI DDKReg_GetPciInfo(HKEY hk, PDDKPCIINFO ppi);

#ifdef __cplusplus
};
#endif	// __cplusplus
