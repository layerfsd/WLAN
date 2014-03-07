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

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    pndt_dev.h

Abstract:

    Public interface/definitiions for pendant devices


Revision History:

--*/

#pragma once

#include <windows.h>
#include <windev.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************

    INDICATOR DEVICES

  *********************************************************************************/


//
// Indicator Capabilities
//

#define     INDDEV_CAP_ALLMASK      0x1FFFFFFF

#define     INDDEV_CAP_MICMUTE      0x10000000
#define     INDDEV_CAP_CALLHOLD     0x08000000
#define     INDDEV_CAP_CALLFWD      0x04000000
#define     INDDEV_CAP_CALLMISSED   0x02000000
#define     INDDEV_CAP_CALLLINE2    0x01000000
#define     INDDEV_CAP_CALLANADIG   0x00800000
#define     INDDEV_CAP_CALLROAMING  0x00400000
#define     INDDEV_CAP_CALLRING     0x00200000
#define     INDDEV_CAP_SYNC         0x00100000
#define     INDDEV_CAP_IMBUDDY      0x00080000
#define     INDDEV_CAP_IMMESSAGE    0x00040000
#define     INDDEV_CAP_USERNOTIF    0x00020000
#define     INDDEV_CAP_SYSNOTIF     0x00010000
#define     INDDEV_CAP_ALARMAPPT    0x00008000

// these span more than on bit
#define     INDDEV_CAP_MEDIASTATE   0x00006000

#define     INDDEV_CAP_BATTERYLEV   0x00001C00
#define     INDDEV_CAP_BATTERYLEV_0 0x00000000
#define     INDDEV_CAP_BATTERYLEV_1 0x00000400
#define     INDDEV_CAP_BATTERYLEV_2 0x00000800
#define     INDDEV_CAP_BATTERYLEV_3 0x00000C00
#define     INDDEV_CAP_BATTERYLEV_4 0x00001000
#define     INDDEV_CAP_BATTERYLEV_5 0x00001400
#define     INDDEV_CAP_BATTERYLEV_6 0x00001800
#define     INDDEV_CAP_BATTERYLEV_7 0x00001C00

#define     INDDEV_CAP_VOLUMELEV    0x00000380

#define     INDDEV_CAP_MESSAGE          0x00000060
#define     INDDEV_CAP_MESSAGE_EMAIL    0x00000020
#define     INDDEV_CAP_MESSAGE_VMAIL    0x00000040
#define     INDDEV_CAP_MESSAGE_SMS      0x00000060

#define     INDDEV_CAP_RADIOSTATE       0x00000018
#define     INDDEV_CAP_RADIOSTATE_STAND 0x00000000
#define     INDDEV_CAP_RADIOSTATE_VOICE 0x00000008
#define     INDDEV_CAP_RADIOSTATE_DATA  0x00000010
#define     INDDEV_CAP_RADIOSTATE_BOTH  0x00000018

#define     INDDEV_CAP_RADSIGNAL        0x00000007
#define     INDDEV_CAP_RADSIGNAL_NONE   0x00000000
#define     INDDEV_CAP_RADSIGNAL_1      0x00000001
#define     INDDEV_CAP_RADSIGNAL_2      0x00000002
#define     INDDEV_CAP_RADSIGNAL_3      0x00000003
#define     INDDEV_CAP_RADSIGNAL_4      0x00000004
#define     INDDEV_CAP_RADSIGNAL_5      0x00000005
#define     INDDEV_CAP_RADSIGNAL_OFF    0x00000006

//$ TODO (RandyRam) finish out the multiple bit definitions

#define     FILE_DEVICE_INDICATOR   81102

typedef struct  tagIndDevCaps
{
    DWORD   dwSize;

    DWORD   dwDevCaps;

}   INDICATOR_DEVCAPS, *PINDICATOR_DEVCAPS;

typedef struct  tagIndState
{
    DWORD   dwSize;

    DWORD   dwIndicatorState;

}   INDICATOR_STATE, *PINDICATOR_STATE;

/*++

	IOCTL_INDICATOR_GETDEVCAPS

	Obtains the device capabilities of the indicator
    It is a bitmask of the INDDEV_CAP_* defintions above
	
	Output Buffer is a INDICATOR_DEVCAPS structure
    Be sure to set the dwSize parameter to the size of the structure
    for versioning purposes

	Input buffer is unused.

--*/

#define IOCTL_INDICATOR_GETDEVCAPS       \
            CTL_CODE(FILE_DEVICE_INDICATOR, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)

/*++

	IOCTL_INDICATOR_GETSTATE

	Obtains the current state of the indicator
	
	Output Buffer is a INDICATOR_STATE structure
    Be sure to set the dwSize parameter to the size of the structure
    for versioning purposes

	Input buffer is unused.

--*/

#define IOCTL_INDICATOR_GETSTATE         \
            CTL_CODE(FILE_DEVICE_INDICATOR, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

/*++

	IOCTL_INDICATOR_SETSTATE

	Sets the current state of the indicator
	
	Input Buffer is a INDICATOR_STATE structure
    Be sure to set the dwSize parameter to the size of the structure
    for versioning purposes

	Output buffer is unused.

--*/

#define IOCTL_INDICATOR_SETSTATE         \
            CTL_CODE(FILE_DEVICE_INDICATOR, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)

/*++

	IOCTL_INDICATOR_GETDEVICEID

	Gets the device identifier
	
	Output Buffer is a DWORD 

	Input buffer is unused.

--*/

#define IOCTL_INDICATOR_GETDEVICEID         \
            CTL_CODE(FILE_DEVICE_INDICATOR, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)



/**********************************************************************************

    CHARACTER TEXT DEVICES

  *********************************************************************************/

//
// CharacterText Capabilities
//

//      Supported languages bitmasks

//      Mask for all of the currently supported chartext capabilities
#define     CHTDEV_CAP_LANG_ALLMASK         0x0000007F

#define     CHTDEV_CAP_LANG_EXT                 0x00080000
#define     CHTDEV_CAP_LANG_KATA               0x00000040
#define     CHTDEV_CAP_LANG_HIRA                0x00000020
#define     CHTDEV_CAP_LANG_BRAIL               0x00000010
#define     CHTDEV_CAP_LANG_DING                0x00000008
#define     CHTDEV_CAP_LANG_CUR                 0x00000004
#define     CHTDEV_CAP_LANG_SUP                 0x00000002
#define     CHTDEV_CAP_LANG_LAT                 0x00000001

#define     CHTDEV_CAP_LANG_COUNT   7

#define     FILE_DEVICE_CHARTEXT        81103


typedef struct  tagCharTextDevCaps
{
    DWORD   dwSize;

    DWORD   dwLangCaps;

    DWORD   dwBufferSize;

}   CHARTEXT_DEVCAPS, *PCHARTEXT_DEVCAPS;


/*++

    IOCTL_CHARTEXT_GETDEVCAPS

    Obtains the device capabilities of the indicator
    It is a bitmask of the CHTDEV_CAP_* defintions above

    Output Buffer is a CHARTEXT_DEVCAPS structure
    Be sure to set the dwSize parameter to the size of the structure
    for versioning purposes

    Input buffer is unused.

--*/
#define IOCTL_CHARTEXT_GETDEVCAPS       \
            CTL_CODE(FILE_DEVICE_CHARTEXT, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)



/*++

        IOCTL_CHARTEXT_GETDEVICEID

        Gets the device identifier

        Output Buffer is a DWORD 

        Input buffer is unused.

--*/
#define IOCTL_CHARTEXT_GETDEVICEID         \
            CTL_CODE(FILE_DEVICE_CHARTEXT, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)





#ifdef __cplusplus
}
#endif

