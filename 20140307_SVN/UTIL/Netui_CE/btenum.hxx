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
#if ! defined (__btenum_H__)

#define __btenum_H__		1

#define BTENUM_CLASSES		10

#define BTENUM_DEVICE_MODEM			0
#define BTENUM_DEVICE_PRINTER		1
#define BTENUM_DEVICE_LAP			2
#define BTENUM_DEVICE_OBEX_FTP		3
#define BTENUM_DEVICE_OBEX_OPP		4
#define BTENUM_DEVICE_HEADSET		5
#define BTENUM_DEVICE_ASYNC			6
#define BTENUM_DEVICE_HID			7
#define BTENUM_DEVICE_PAN			8
#define BTENUM_DEVICE_HANDSFREE		9

#define BTENUM_CHANNEL_NOT_USED		0xfc
#define BTENUM_MAX_NAME				248
#define BTENUM_MAX_PORT_NAME		16

#define BTENUM_PORT_NAME			L"BSP"
#define BTENUM_RAS_NAME				L"`Bluetooth"

int BthEnumGetClassDefaults (int iDevClass, struct BTDEV *pbt);

struct BTDEV {
    int			iDeviceClass;

	HANDLE		hDevHandle;

	WCHAR		szDeviceName[BTENUM_MAX_NAME+1];
	WCHAR		szPortName[BTENUM_MAX_PORT_NAME];

	BT_ADDR     	b;			// Address of the item

	unsigned int fActive   : 1;		// currently active
	unsigned int fTrusted  : 1;		// trusted
	unsigned int fAuth     : 1;		// connection must be authenticated
	unsigned int fEncrypt  : 1;		// connection must be encrypted

	unsigned char	ucChannel;
	DWORD		dwHidDevClass;
	GUID		service_id;

	int             imtu;

	unsigned char   *psdp;
	unsigned int    csdp;

	BTDEV (BT_ADDR bt, int iclass) {
		memset (this, 0, sizeof(*this));
		ucChannel = BTENUM_CHANNEL_NOT_USED;
		iDeviceClass = iclass;
		b = bt;

		BthEnumGetClassDefaults (iclass, this);
	}

	BTDEV (BTDEV &btd) {
		*this = btd;
	}

	~BTDEV (void) {
		if (psdp)
			LocalFree (psdp);
	}
};

typedef int (*BthEnumCallback) (void *pContext, BTDEV *pDev);

int BthEnumDevices (void *pContext, BthEnumCallback pCallback);
int BthEnumUpdate (BTDEV *bt);
int BthEnumRemove (BTDEV *bt);
int BthEnumActivate (BTDEV *bt, int fFirstTime);
int BthEnumDeactivate (BTDEV *bt);

#endif
