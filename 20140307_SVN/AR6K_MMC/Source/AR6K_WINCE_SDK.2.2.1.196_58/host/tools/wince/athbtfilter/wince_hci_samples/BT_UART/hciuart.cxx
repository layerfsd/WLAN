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
//------------------------------------------------------------------------------
// 
//      Bluetooth HCI UART interface
// 
// 
// Module Name:
// 
//      hciuart.cxx
// 
// Abstract:
// 
//      This file implements Bluetooth HCI UART interface
// 
// 
//------------------------------------------------------------------------------
//#define BT_USE_CELOG		1

#include <windows.h>

#if ! defined (UNDER_CE)
#include <assert.h>
#define ASSERT assert
#endif

#include <bt_buffer.h>
#include <bt_hcip.h>
#include <bt_os.h>
#include <bt_debug.h>
#include <bt_tdbg.h>

#define ATHBT_FILTER

#ifdef ATHBT_FILTER
#include "athbtfilter.h"
#endif

#if defined (SDK_BUILD) && defined (BT_USE_CELOG)
#define CELOGSDK_FILELOG	L"uart"
#define CELOGSDK_DEFINE		1

#include "../../../../../../../private/osinternal/pbtools/privdbg/celog_sdk.h"
#endif

static HANDLE hFile = INVALID_HANDLE_VALUE;
static HANDLE hWriteEvent = NULL;
static HANDLE hReadEvent = NULL;
static HCI_TransportCallback gCallback = NULL;

#ifdef ATHBT_FILTER
static ATH_BT_FILTER_INSTANCE g_AthBtFilterInstance = {0};
#endif

#define PACKET_SIZE_R		(64 * 1024 + 128)
#define PACKET_SIZE_W		(255 + 3)

#define DEFAULT_COM_NAME	L"COM1:"

#define DEBUG_READ_BUFFER_HEADER	4
#define DEBUG_WRITE_BUFFER_HEADER	8
#define DEBUG_READ_BUFFER_TRAILER	1
#define DEBUG_WRITE_BUFFER_TRAILER	3

DECLARE_DEBUG_VARS();

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    switch (dwReason) 
    {
    case DLL_PROCESS_ATTACH:
#ifdef ATHBT_FILTER        
        memset(&g_AthBtFilterInstance,0,sizeof(g_AthBtFilterInstance));
#endif
        break;
    case DLL_PROCESS_DETACH:
        break;

    default:
        break;
    }
    return TRUE ;
}


static BOOL SetHighSpeedMode(TCHAR *DevName, DWORD *pBaudRate);
static BOOL RestoreDefaultBaudRate();

#if defined (BT_USE_CELOG)
static int				g_fCeLog = FALSE;
#endif

int HCI_ReadHciParameters (HCI_PARAMETERS *pParms) {
	if (pParms->uiSize < sizeof (*pParms))
		return FALSE;

	memset (pParms, 0, sizeof(*pParms));

	pParms->uiSize             = sizeof(*pParms);
	pParms->fInterfaceVersion  = HCI_INTERFACE_VERSION_1_1;
	pParms->iMaxSizeRead       = PACKET_SIZE_R;
	pParms->iMaxSizeWrite      = PACKET_SIZE_W;
	pParms->iWriteBufferHeader = 4;
	pParms->iReadBufferHeader  = 4;
	pParms->fHardwareVersion   = HCI_HARDWARE_VERSION_V_1_0_B;
	pParms->uiWriteTimeout     = HCI_DEFAULT_WRITE_TIMEOUT;
	pParms->uiDriftFactor      = HCI_DEFAULT_DRIFT;
	pParms->uiFlags            = 0;

	HKEY hk;

	if (RegOpenKeyEx (HKEY_BASE, L"Software\\Microsoft\\Bluetooth\\hci", 0, KEY_READ, &hk) == ERROR_SUCCESS) { 
		DWORD dwType = 0;
		DWORD dwData = 0;
		DWORD dwSize = sizeof(dwData);
		if ((RegQueryValueEx (hk, L"ResetDelay", NULL, &dwType, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dwData)) && dwData)
			pParms->uiResetDelay = dwData;

		dwData = 0;
		dwSize = sizeof(dwData);
		if ((RegQueryValueEx (hk, L"SpecV10a", NULL, &dwType, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dwData)) && dwData)
			pParms->fHardwareVersion = HCI_HARDWARE_VERSION_V_1_0_A;

		dwData = 0;
		dwSize = sizeof(dwData);
		if ((RegQueryValueEx (hk, L"SpecV11", NULL, &dwType, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dwData)) && dwData)
			pParms->fHardwareVersion = HCI_HARDWARE_VERSION_V_1_1;

		dwData = 0;
		dwSize = sizeof(dwData);
		if ((RegQueryValueEx (hk, L"WriteTimeout", NULL, &dwType, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dwData)) && dwData)
			pParms->uiWriteTimeout = dwData;

		dwData = 0;
		dwSize = sizeof(dwData);
		if ((RegQueryValueEx (hk, L"Flags", NULL, &dwType, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dwData)) && dwData)
			pParms->uiFlags = dwData;

		dwData = 0;
		dwSize = sizeof(dwData);
		if ((RegQueryValueEx (hk, L"Drift", NULL, &dwType, (LPBYTE)&dwData, &dwSize) == ERROR_SUCCESS) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dwData)) && dwData)
			pParms->uiDriftFactor = dwData;

		RegCloseKey (hk);
	}


#if defined (DEBUG) || defined (_DEBUG)
	pParms->iReadBufferHeader   = DEBUG_READ_BUFFER_HEADER;
	pParms->iReadBufferTrailer  = DEBUG_READ_BUFFER_TRAILER;
	pParms->iWriteBufferHeader  = DEBUG_WRITE_BUFFER_HEADER;
	pParms->iWriteBufferTrailer = DEBUG_WRITE_BUFFER_TRAILER;
#endif

	return TRUE;
}

int HCI_StartHardware (void) {
	IFDBG(DebugOut (DEBUG_HCI_INIT, L"+HCI_StartHardware\n"));
	if (hFile != INVALID_HANDLE_VALUE) {
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"-HCI_StartHardware (already started)\n"));
		return TRUE;
	}

	if (! gCallback) {
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"-HCI_StartHardware (not registered)\n"));
		return FALSE;
	}

	return ERROR_SUCCESS == gCallback (DEVICE_UP, NULL);
}

int HCI_StopHardware (void) {
	IFDBG(DebugOut (DEBUG_HCI_INIT, L"+HCI_StopHardware\n"));
	if (hFile == INVALID_HANDLE_VALUE) {
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"-HCI_StopHardware (already stopped)\n"));
		return TRUE;
	}

	if (! gCallback) {
		IFDBG(DebugOut (DEBUG_HCI_INIT, L"-HCI_StopHardware (not registered)\n"));
		return FALSE;
	}

	return ERROR_SUCCESS == gCallback (DEVICE_DOWN, NULL);
}

int HCI_SetCallback (HCI_TransportCallback pfCallback) {
	gCallback = pfCallback;

	if (gCallback)
		DebugInit();
	else
		DebugDeInit();

	return ERROR_SUCCESS;
}

int HCI_OpenConnection (void) {
	IFDBG(DebugOut (DEBUG_HCI_INIT, L"HCI_OpenConnection - Started\n"));
    
	if (hFile != INVALID_HANDLE_VALUE)
		return FALSE;

	WCHAR szComPortName[_MAX_PATH];
	wcscpy (szComPortName, DEFAULT_COM_NAME);

	DWORD dwBaud = 115200;

	HKEY hk;
	if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_BASE, L"software\\microsoft\\bluetooth\\hci", 0, KEY_READ, &hk)) {
		DWORD dwType;
		DWORD dwSize = sizeof(szComPortName);
		if ((ERROR_SUCCESS == RegQueryValueEx (hk, L"Name", NULL, &dwType, (LPBYTE)szComPortName, &dwSize)) &&
			(dwType == REG_SZ) && (dwSize > 0) && (dwSize < _MAX_PATH))
			;
		else
			wcscpy (szComPortName, DEFAULT_COM_NAME);

		dwSize = sizeof(dwBaud);
		if ((ERROR_SUCCESS == RegQueryValueEx (hk, L"baud", NULL, &dwType, (LPBYTE)&dwBaud, &dwSize)) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dwBaud)))
			;
		else
			dwBaud = 115200;

		RegCloseKey (hk);
	}

	IFDBG(DebugOut (DEBUG_HCI_INIT, L"Opening port %s (rate %d) for I/O with unit\n", szComPortName, dwBaud));

    //DebugBreak();

    if (!SetHighSpeedMode(szComPortName, &dwBaud)) {
		return FALSE;    
    }

    hFile = CreateFile(szComPortName,
                GENERIC_READ | GENERIC_WRITE,
                0,    // comm devices must be opened w/exclusive-access
                NULL, // no security attrs
                OPEN_EXISTING, // comm devices must use OPEN_EXISTING
#if ! defined (UNDER_CE)
				FILE_FLAG_OVERLAPPED |
#endif
                FILE_ATTRIBUTE_NORMAL,    // overlapped I/O 
                NULL  // hTemplate must be NULL for comm devices  
				);

    if (hFile == INVALID_HANDLE_VALUE) {
		IFDBG(DebugOut (DEBUG_ERROR, L"Failed CreateFile in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ()));
		return FALSE;

    }

    if (! SetupComm (hFile, 20000, 20000)) {
		IFDBG(DebugOut (DEBUG_ERROR, L"Failed SetupComm in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ()));
		CloseHandle (hFile);
		hFile = INVALID_HANDLE_VALUE;
		return FALSE;
    } 

    // purge any information in the buffer

    if ( ! PurgeComm (hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR )) {
		IFDBG(DebugOut (DEBUG_ERROR, L"Failed PurgeComm in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ()));
		CloseHandle (hFile);
		hFile = INVALID_HANDLE_VALUE;
		return FALSE;
    }

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;

    if (! SetCommTimeouts (hFile, &CommTimeOuts)) {
		IFDBG(DebugOut (DEBUG_ERROR, L"Failed SetCommTimeouts in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ()));
		CloseHandle (hFile);
		hFile = INVALID_HANDLE_VALUE;
		return FALSE;
    }

    DCB dcb;

    dcb.DCBlength = sizeof(dcb);
    dcb.BaudRate = dwBaud;
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;
    dcb.fOutxCtsFlow = TRUE;
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fOutxDsrFlow = FALSE;    
    dcb.fDsrSensitivity = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fTXContinueOnXoff = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
    dcb.fAbortOnError = TRUE;
//    dcb.wReserved = 0;
    dcb.ByteSize =8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.XonChar = 0x11;
    dcb.XoffChar = 0x13;
    dcb.XonLim = 3000 ;
    dcb.XoffLim = 9000 ;

    if (! SetCommState(hFile, &dcb)) {
		IFDBG(DebugOut (DEBUG_ERROR, L"Failed SetCommState in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ()));
		CloseHandle (hFile);
		hFile = INVALID_HANDLE_VALUE;
		return FALSE;
    }

	hWriteEvent = CreateEvent (NULL, FALSE, FALSE, NULL);
	hReadEvent = CreateEvent (NULL, FALSE, FALSE, NULL);

#if defined (BT_USE_CELOG)
	if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_BASE, L"software\\microsoft\\bluetooth\\debug", 0, KEY_READ, &hk)) {
		DWORD dwType;
		DWORD dw;
		DWORD dwSize = sizeof(dw);

		if ((ERROR_SUCCESS == RegQueryValueEx (hk, L"celog", NULL, &dwType, (LPBYTE)&dw, &dwSize)) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dw)) && dw)
			g_fCeLog = IsCeLogStatus (CELOGSTATUS_ENABLED_GENERAL);

		RegCloseKey (hk);
	}

	if (g_fCeLog) {
#if defined (SDK_BUILD)
		CELOGSDK_START ();
#endif

		BTH_CELOG_START_DATA sd;

		sd.eTransport = UART;
		GetLocalTime (&sd.st);
		wsprintf (sd.szDriverString, L"UART Driver v. 0x%08x on %s", HCI_INTERFACE_VERSION_1_1, szComPortName);

		CELOGDATAFLAGGED(TRUE, CELID_RAW_UCHAR, &sd, sizeof(sd), 0, CELZONE_ALWAYSON, CELOG_FLAG_START);
	}
#endif
    
#ifdef ATHBT_FILTER
    AthBtFilter_Attach(&g_AthBtFilterInstance);  
#endif 

	IFDBG(DebugOut (DEBUG_HCI_INIT, L"HCI_OpenConnection - Successful\n"));

	return TRUE;
}

void HCI_CloseConnection (void) {
	
    DWORD lineErrors;

    IFDBG(DebugOut (DEBUG_HCI_INIT, L"HCI_CloseConnection - Started\n"));

	if (hFile == INVALID_HANDLE_VALUE) {
		IFDBG(DebugOut (DEBUG_ERROR, L"HCI_CloseConnection - not active\n"));
		return;
	}

     if (!ClearCommError(hFile,
                         &lineErrors,
                         NULL)) {
            
        RETAILMSG(1,(TEXT("HCI_CloseConnection - Failed to clear comm error \r\n"), GetLastError));                        
            
    } else {
            
        RETAILMSG(1,(TEXT("HCI_CloseConnection - line error mask : 0x%X  \r\n"), 
                            lineErrors)); 

    }


	CloseHandle (hFile);
	CloseHandle (hWriteEvent);
	CloseHandle (hReadEvent);

	hFile = INVALID_HANDLE_VALUE;
	hWriteEvent = hReadEvent = NULL;

#if defined (BT_USE_CELOG)
	if (g_fCeLog) {
		BTH_CELOG_STOP_DATA sd;
		GetLocalTime (&sd.st);

		CELOGDATAFLAGGED(TRUE, CELID_RAW_UCHAR, &sd, sizeof(sd), 0, CELZONE_ALWAYSON, CELOG_FLAG_STOP);

#if defined (SDK_BUILD)
		CELOGSDK_STOP ();
#endif

		g_fCeLog = FALSE;
	}
#endif

	IFDBG(DebugOut (DEBUG_HCI_INIT, L"HCI_CloseConnection - Successful\n"));

#ifdef ATHBT_FILTER
    AthBtFilter_Detach(&g_AthBtFilterInstance);  
#endif 

    RestoreDefaultBaudRate();

	return;
}

static BOOL WriteCommPort (unsigned char *pBuffer, unsigned int cSize) {
	DWORD dwFilledSoFar = 0;
	while ((int)dwFilledSoFar < cSize) {
		DWORD dwWrit = 0;
#if ! defined (UNDER_CE)
		OVERLAPPED o;
		memset (&o, 0, sizeof(o));
		o.hEvent = hWriteEvent;
		while (! WriteFile (hFile, &(pBuffer[dwFilledSoFar]), cSize - dwFilledSoFar, &dwWrit, &o)) {
			if ((GetLastError() == ERROR_IO_PENDING) &&
					GetOverlappedResult (hFile, &o, &dwWrit, TRUE))
				break;

			if (hFile != INVALID_HANDLE_VALUE)
				IFDBG(DebugOut (DEBUG_ERROR, L"Error writing COM port: GetLastError = 0x%08x (%d)\n", GetLastError (), GetLastError ()));
			return FALSE;
		}
#else
		if ((! WriteFile (hFile, &(pBuffer[dwFilledSoFar]), cSize - dwFilledSoFar, &dwWrit, NULL)) &&
												(dwWrit == 0)) {

#if defined (DEBUG) || defined (_DEBUG) || defined (RETAILLOG)
			if (hFile != INVALID_HANDLE_VALUE)
				IFDBG(DebugOut (DEBUG_ERROR, L"Error writing COM port: GetLastError = 0x%08x (%d)\n", GetLastError (), GetLastError ()));
#endif
			return FALSE;
		}
#endif

#if defined (BT_USE_CELOG)
		if (g_fCeLog && dwWrit) {
			CELOGDATAFLAGGED(TRUE, CELID_RAW_UCHAR, &pBuffer[dwFilledSoFar], (unsigned short)dwWrit, 0, CELZONE_ALWAYSON, CELOG_FLAG_RAW_OUT);
		}
#endif
		dwFilledSoFar += dwWrit;
	}

	return TRUE;
}

static BOOL ReadCommPort (unsigned char *pBuffer, DWORD dwLen) {
	DWORD dwFilledSoFar = 0;
	while (dwFilledSoFar < dwLen) {
		OVERLAPPED o;
		memset (&o, 0, sizeof(o));
		o.hEvent = hReadEvent;

		DWORD dwRead = 0;
#if ! defined (UNDER_CE)
		while (! ReadFile (hFile, &pBuffer[dwFilledSoFar], dwLen - dwFilledSoFar, &dwRead, &o)) {
			if ((GetLastError() == ERROR_IO_PENDING) &&
						GetOverlappedResult (hFile, &o, &dwRead, TRUE))
				break;

			if (hFile != INVALID_HANDLE_VALUE)
				IFDBG(DebugOut (DEBUG_ERROR, L"Error reading COM port: GetLastError = 0x%08x (%d)\n", GetLastError (), GetLastError ()));
			return FALSE;
		}
#else
		if ((! ReadFile (hFile, &pBuffer[dwFilledSoFar], dwLen - dwFilledSoFar, &dwRead, NULL)) &&
			(dwRead == 0)) {
#if defined (DEBUG) || defined (_DEBUG) || defined (RETAILLONG)
			if (hFile != INVALID_HANDLE_VALUE)
				IFDBG(DebugOut (DEBUG_ERROR, L"Error reading COM port: GetLastError = 0x%08x (%d)\n", GetLastError (), GetLastError ()));
#endif
			return FALSE;
		}
#endif
#if defined (BT_USE_CELOG)
		if (g_fCeLog && dwRead) {
			CELOGDATAFLAGGED(TRUE, CELID_RAW_UCHAR, pBuffer + dwFilledSoFar, (unsigned short)dwRead, 0, CELZONE_ALWAYSON, CELOG_FLAG_RAW_IN);
		}
#endif
		dwFilledSoFar += dwRead;
	}

	return TRUE;
}

int HCI_WritePacket (HCI_TYPE eType, BD_BUFFER *pBuff) {
	IFDBG(DebugOut (DEBUG_HCI_TRANSPORT, L"HCI_WritePacket type 0x%02x len %d\n", eType, BufferTotal (pBuff)));

	IFDBG(DumpBuff (DEBUG_HCI_DUMP, pBuff->pBuffer + pBuff->cStart, BufferTotal (pBuff)));

#if defined (DEBUG) || defined (_DEBUG)
	ASSERT (pBuff->cStart == DEBUG_WRITE_BUFFER_HEADER);
	ASSERT (pBuff->cSize - pBuff->cEnd >= DEBUG_WRITE_BUFFER_TRAILER);
#endif

	ASSERT (! (pBuff->cStart & 0x3));

	if (((int)BufferTotal (pBuff) > PACKET_SIZE_W) || (pBuff->cStart < 1)) {
		IFDBG(DebugOut (DEBUG_ERROR, L"[UART] Packet too big (%d, should be <= %d), or no space for header!\n", BufferTotal (pBuff), PACKET_SIZE_W));
		return FALSE;
	}

	if (hFile == INVALID_HANDLE_VALUE) {
		DebugOut (DEBUG_ERROR, L"HCI_WritePacket - not active\n");
		return FALSE;
	}

#ifdef ATHBT_FILTER

    if (eType == COMMAND_PACKET) {
        AthBtFilterHciCommand(&g_AthBtFilterInstance, 
                              (pBuff->pBuffer + pBuff->cStart),
                              BufferTotal(pBuff));
    }  else if (eType == DATA_PACKET_ACL) {
                /* for CE5 we filter ACL packets to detect application traffic flows like A2DP */
        AthBtFilterHciAclDataOut(&g_AthBtFilterInstance,
                                 (pBuff->pBuffer + pBuff->cStart),
                                  BufferTotal(pBuff));
    }
    
#endif

	pBuff->pBuffer[--pBuff->cStart] = (unsigned char)eType;


    if (eType == 0x1) {
        USHORT command = pBuff->pBuffer[pBuff->cStart + 1] | (((USHORT)pBuff->pBuffer[pBuff->cStart + 2]) << 8);     
        if (command == ((0x3 << 10) | 0x3)) {
            RETAILMSG(1,(TEXT("**** HCI Reset Detected \r\n")));        
        }
    }


	if (! WriteCommPort (pBuff->pBuffer + pBuff->cStart, BufferTotal (pBuff)))
		return FALSE;

	IFDBG(DebugOut (DEBUG_HCI_TRANSPORT, L"HCI_WritePacket : DONE type 0x%02x len %d\n", eType, BufferTotal (pBuff)));
	return TRUE;
}

int HCI_ReadPacket (HCI_TYPE *peType, BD_BUFFER *pBuff) {
	IFDBG(DebugOut (DEBUG_HCI_TRANSPORT, L"HCI_ReadPacket\n"));
	if (hFile == INVALID_HANDLE_VALUE) {
		IFDBG(DebugOut (DEBUG_ERROR, L"HCI_ReadPacket - not active\n"));
		return FALSE;
	}

	pBuff->cStart = 3;
	pBuff->cEnd = pBuff->cSize;

	if (BufferTotal (pBuff) < 257) {
		IFDBG(DebugOut (DEBUG_ERROR, L"HCI_ReadPacket - failed buffer too small (%d bytes)\n", BufferTotal (pBuff)));
		return FALSE;
	}

	if (! ReadCommPort (pBuff->pBuffer + pBuff->cStart, 1))
		return FALSE;

	++pBuff->cStart;

	int dwPacketSize = 0;

	switch (pBuff->pBuffer[pBuff->cStart - 1]) {
	case 0x01:			// Command packet
		IFDBG(DebugOut (DEBUG_ERROR, L"HCI_ReadPacket - unexpected packet type (command)\n"));
		break;

	case 0x02:			// ACL packet
		if (! ReadCommPort (pBuff->pBuffer + pBuff->cStart, 4))
			return FALSE;

		pBuff->cEnd = pBuff->cStart + 4 + (pBuff->pBuffer[pBuff->cStart + 2] | (pBuff->pBuffer[pBuff->cStart + 3] << 8));

		if (pBuff->cEnd > pBuff->cSize) {
			IFDBG(DebugOut (DEBUG_ERROR, L"HCI_ReadPacket - failed: buffer too small\n"));
			return FALSE;
		}

		if (! ReadCommPort (pBuff->pBuffer + pBuff->cStart + 4, BufferTotal (pBuff) - 4))
			return FALSE;

		*peType = DATA_PACKET_ACL;
   
        AthBtFilterHciAclDataIn(&g_AthBtFilterInstance,
                                 (pBuff->pBuffer + pBuff->cStart),
                                 BufferTotal(pBuff));
         
         
		IFDBG(DebugOut (DEBUG_HCI_DUMP, L"HCI_ReadPacket: ACL\n"));
		IFDBG(DumpBuff (DEBUG_HCI_DUMP, pBuff->pBuffer + pBuff->cStart, BufferTotal (pBuff)));
		return TRUE;

	case 0x03:			// SCO packet
		if (! ReadCommPort (pBuff->pBuffer + pBuff->cStart, 3))
			return FALSE;

		pBuff->cEnd = pBuff->cStart + 3 + pBuff->pBuffer[pBuff->cStart + 2];

		if (pBuff->cEnd > pBuff->cSize) {
			IFDBG(DebugOut (DEBUG_ERROR, L"HCI_ReadPacket - failed: buffer too small\n"));
			return FALSE;
		}

		if (! ReadCommPort (pBuff->pBuffer + pBuff->cStart + 3, BufferTotal (pBuff) - 3))
			return FALSE;

		*peType = DATA_PACKET_SCO;

		IFDBG(DebugOut (DEBUG_HCI_DUMP, L"HCI_ReadPacket: SCO\n"));
		IFDBG(DumpBuff (DEBUG_HCI_DUMP, pBuff->pBuffer + pBuff->cStart, BufferTotal (pBuff)));
		return TRUE;

	case 0x04:			// HCI Event
		if (! ReadCommPort (pBuff->pBuffer + pBuff->cStart, 2))
			return FALSE;

		pBuff->cEnd = pBuff->cStart + 2 + pBuff->pBuffer[pBuff->cStart + 1];

		if (pBuff->cEnd > pBuff->cSize) {
			IFDBG(DebugOut (DEBUG_ERROR, L"HCI_ReadPacket - failed: buffer too small\n"));
			return FALSE;
		}

		if (! ReadCommPort (pBuff->pBuffer + pBuff->cStart + 2, BufferTotal (pBuff) - 2))
			return FALSE;

		*peType = EVENT_PACKET;

#ifdef ATHBT_FILTER
        AthBtFilterHciEvent(&g_AthBtFilterInstance, 
                            (pBuff->pBuffer + pBuff->cStart),
                            BufferTotal(pBuff));
#endif

		IFDBG(DebugOut (DEBUG_HCI_DUMP, L"HCI_ReadPacket: Event\n"));
		IFDBG(DumpBuff (DEBUG_HCI_DUMP, pBuff->pBuffer + pBuff->cStart, BufferTotal (pBuff)));
		return TRUE;
	}

	IFDBG(DebugOut (DEBUG_ERROR, L"HCI_ReadPacket - unknown packet type\n"));

#if defined (DEBUG) || defined (_DEBUG)
	IFDBG(DebugOut (DEBUG_ERROR, L"Dumping the failing buffer...\n"));
	unsigned char buff[128];
	buff[0] = pBuff->pBuffer[pBuff->cStart - 1];
	int buff_size = 128;
	int buff_offset = 1;

	while (buff_offset < buff_size) {
		OVERLAPPED o;
		memset (&o, 0, sizeof(o));
		o.hEvent = hReadEvent;

		DWORD dwRead = 0;
#if ! defined (UNDER_CE)
		while (! ReadFile (hFile, &buff[buff_offset], buff_size - buff_offset, &dwRead, &o)) {
			if ((GetLastError() == ERROR_IO_PENDING) &&
						GetOverlappedResult (hFile, &o, &dwRead, TRUE))
				break;

			if (hFile != INVALID_HANDLE_VALUE)
				IFDBG(DebugOut (DEBUG_ERROR, L"Error reading COM port: GetLastError = 0x%08x (%d)\n", GetLastError (), GetLastError ()));
			break;
		}
#else
		if ((! ReadFile (hFile, &buff[buff_offset], buff_size - buff_offset, &dwRead, NULL)) &&
			(dwRead == 0)) {
			if (hFile != INVALID_HANDLE_VALUE)
				IFDBG(DebugOut (DEBUG_ERROR, L"Error writing COM port: GetLastError = 0x%08x (%d)\n", GetLastError (), GetLastError ()));
			break;
		}
#endif
		if (dwRead == 0)
			break;

		buff_offset += dwRead;
	}

	IFDBG(DumpBuff (DEBUG_ERROR, buff, buff_offset));
#endif

	return FALSE;
}



/*

Atheros BT UART HCI Baud rate change for high-speed mode :

HCI packet type (1 byte) + HCI Command (2 bytes) + length (1 byte) + baud (2 bytes)

    For 3Mbps
    01 0c fc 02 30 75 (0x7530 = 30000)

*/

#define HCI_HS_BAUD_DEFAULT   (115200 * 10)  /* roughly 1 Mb/s */ 


static BOOL ChangeBaudRate(TCHAR *DevName, DWORD CurrentBaudRate, DWORD NewBaudRate)
{
    BOOL    success = FALSE;
    UCHAR   hciCommand[] =  {0x01,0x0c,0xfc,0x2,0,0};
    DCB     dcb;
    DWORD   written,read;
    COMMTIMEOUTS commTimeOuts;
    UCHAR   hciEvent[1 + 1 + 1 + 3 + 1];
    HANDLE  hDevice = INVALID_HANDLE_VALUE;
    USHORT  baudVal;
    
    baudVal = (USHORT)(NewBaudRate / 100);
    hciCommand[4] = (UCHAR)baudVal;
    hciCommand[5] = (UCHAR)(baudVal >> 8);
    
    memset(&dcb,0,sizeof(dcb));

    dcb.DCBlength = sizeof(dcb);
    
    dcb.BaudRate = CurrentBaudRate; 
    dcb.fBinary  = TRUE;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.fParity  = FALSE;
    dcb.StopBits = ONESTOPBIT;
    dcb.fOutxCtsFlow = 1;    
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    
    memset(&commTimeOuts, 0, sizeof(commTimeOuts));
    commTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    commTimeOuts.ReadTotalTimeoutMultiplier = 0;
    commTimeOuts.ReadTotalTimeoutConstant = 2000;
    commTimeOuts.WriteTotalTimeoutMultiplier = 0;
    commTimeOuts.WriteTotalTimeoutConstant = 0;

    do {


        hDevice = CreateFile(DevName,
                             GENERIC_READ | GENERIC_WRITE,
                             0,    
                             NULL, 
                             OPEN_EXISTING, 
                             FILE_ATTRIBUTE_NORMAL,    
                             NULL);

        if (hDevice == INVALID_HANDLE_VALUE) {
            RETAILMSG(1,(TEXT("*** Failed to open device %s  (%d) \r\n"),DevName,GetLastError()));
            break;
        }

        if (!SetCommState(hDevice, &dcb)) {
            RETAILMSG(1,(TEXT("*** Failed to set DCB (%d) \r\n"),GetLastError()));
            break;    
        } 
       
        if (! SetCommTimeouts (hDevice, &commTimeOuts)) {
            RETAILMSG(1,(TEXT("***WriteFile Failed (%d) \r\n"), GetLastError()));
	        CloseHandle(hDevice);
            return FALSE;
        }

        PurgeComm(hDevice, PURGE_TXCLEAR | PURGE_RXCLEAR); 

        if (!WriteFile(hDevice,
                       hciCommand, 
                       sizeof(hciCommand), 
                       &written, 
                       NULL)) {
            RETAILMSG(1,(TEXT("***WriteFile Failed (%d) \r\n"), GetLastError()));
            break;                
        }


        if (!ReadFile(hDevice,
                      hciEvent, 
                       sizeof(hciEvent), 
                      &read, 
                      NULL)) {
            RETAILMSG(1,(TEXT("***Read Failed (%d) \r\n"), GetLastError()));
            break;                
        }
        
        
        if (read < sizeof(hciEvent)) {
                /* read timeout is set VERY high, so all bytes of the event should
                  be received */
            RETAILMSG(1,(TEXT("*** Not enough bytes read (%d) \r\n"), read));
            break;
        }

        if (hciEvent[0] != 0x4) {
            RETAILMSG(1,(TEXT("*** Unknown HCI UART tag (0x%X) \r\n"), hciEvent[0]));
            break;
        }

        if (hciEvent[1] != 0xe) {
            RETAILMSG(1,(TEXT("*** Unknown HCI event (0x%X) \r\n"), hciEvent[1]));
            break;
        }

       
        RETAILMSG(1,(TEXT(" HCI Baud Rate change Command complete received, new baud: %d \r\n"),
                NewBaudRate));

        success = TRUE;
            
            /* give target some time to switch over */
        Sleep(500);
        
    } while (FALSE);

    if (hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(hDevice);
    }

    return success;
}

/* function to place UART into high speed mode */
static TCHAR g_DevName[64];
static DWORD g_DefaultBaud = 0;
static DWORD g_CurrentBaud = 0;

static BOOL SetHighSpeedMode(TCHAR *DevName, DWORD *pBaudRate)
{
    BOOL    success;
    DWORD   highSpeedBaud = 0;
    HKEY    hk;
        
    if (ERROR_SUCCESS == 
        RegOpenKeyEx (HKEY_BASE, L"\\Software\\Microsoft\\Bluetooth\\HCI", 0, KEY_READ, &hk)) {
        DWORD dwType;
        DWORD dwSize; 
        DWORD value;
        
        dwSize = sizeof(value);
            /* see if hsbaud exists */
        if (ERROR_SUCCESS ==
            RegQueryValueEx (hk, L"hsbaud", NULL, &dwType, (LPBYTE)&value, &dwSize)) {
            highSpeedBaud = value;        
        }
        RegCloseKey (hk);
    }
        
    if (0 == highSpeedBaud) {
            /* high speed mode is not desired, we allow driver to use defaults */
        return TRUE;    
    }
    
        /* save defaults */
    _tcsncpy(g_DevName, DevName, 64);
    g_DefaultBaud = *pBaudRate;
    g_CurrentBaud = g_DefaultBaud;
    
    success = ChangeBaudRate(g_DevName, g_CurrentBaud, highSpeedBaud);
    
    if (success) {
        *pBaudRate = highSpeedBaud;
        g_CurrentBaud = highSpeedBaud;
    }
    
    return success;
}

static BOOL RestoreDefaultBaudRate()
{
    if ((g_CurrentBaud != 0) && (g_DefaultBaud != 0)) {
        return ChangeBaudRate(g_DevName, g_CurrentBaud, g_DefaultBaud);
    }
    
    return FALSE;
}


