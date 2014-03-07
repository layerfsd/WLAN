//------------------------------------------------------------------------------
// <copyright file="MacUpdater.cpp" company="Atheros">
//    Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
// 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include <stdlib.h>
#include <winioctl.h>

// Driver specific includes
#include <a_types.h>
#include <athdefs.h>
#include <a_drv.h>

#define FLEC_SUCCESS 0
#define MAC_ADDR_LEN 6
#define RD_LEN 4

#define MAC_FIELD                   __T("-mac")
#define RD_FIELD                    __T("-rd")

static int
wmic_ether_aton(const TCHAR *orig, A_UINT8 *eth)
{
   const TCHAR *bufp;
   int i;

   i = 0;
   for (bufp = orig; *bufp != '\0'; ++bufp) {
      unsigned int val;
      TCHAR c = *bufp++;

      if (_istdigit(c)) val = c - __T('0');
      else if (c >= __T('a') && c <= __T('f')) val = c - __T('a') + 10;
      else if (c >= __T('A') && c <= __T('F')) val = c - __T('A') + 10;
      else break;

      val <<= 4;
      c = *bufp++;
      if (_istdigit(c)) val |= c - __T('0');
      else if (c >= __T('a') && c <= __T('f')) val |= c - __T('a') + 10;
      else if (c >= __T('A') && c <= __T('F')) val |= c - __T('A') + 10;
      else break;

      eth[i] = (unsigned char) (val & 0377);
      if (++i == MAC_ADDR_LEN) {
         return 0;
      }
      if (*bufp != ':')
         break;
   }
   return(-1);
}

static int
wmic_rd_aton(const TCHAR *orig,  DWORD *rd)
{
   const TCHAR *bufp;
   int i;

   i = 0;
   *rd = 0;
   for (bufp = orig; *bufp != '\0'; bufp++) {
      unsigned int val;
      TCHAR c = *bufp;

      if (_istdigit(c)) val = c - __T('0');
      else if (c >= __T('a') && c <= __T('f')) val = c - __T('a') + 10;
      else if (c >= __T('A') && c <= __T('F')) val = c - __T('A') + 10;
      else break;

      *rd = (*rd << 4) | (val & 0xF);
      if (++i == RD_LEN) {
         return 0;
      }
   }
   return(-1);
}

void printUsage()
{
		MessageBox(NULL,_T("Usage: eepromUpdater <Adapter Name> <-mac | -rd> <MacAddr | rd_in_hex > e.g 'eepromUpdater AR6K_SD1 -mac aa:bb:cc:dd:ee:ff' or 'eepromUpdater AR6K_SD1 -rd abcd'"),NULL,MB_OK);
}

void _tmain(int argc, TCHAR *argv[]) {

	BYTE macAddr[6];
    TCHAR adapterName[256];
    TCHAR field[12];
    HANDLE hAdapter = NULL;
    DWORD dwReturnedBytes   = 0;
    DWORD rd;
    // Update MAC addr in buffer

	HCURSOR curr = GetCursor();
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	if ((argc != 4)) {
        printUsage();
        exit(-1);
	}

    _tcsncpy(adapterName, argv[1], sizeof(adapterName)/sizeof(TCHAR));
    _tcsncpy(field, argv[2], sizeof(field)/sizeof(TCHAR));

    hAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                INVALID_HANDLE_VALUE);
    if (INVALID_HANDLE_VALUE == hAdapter) {
        MessageBox(NULL,_T("Unable to open the device "),NULL,MB_OK);
        exit(-1);
    }
   
    do { 
        if ( _tcscmp( field, MAC_FIELD) == 0 ) {
	        // Parse the MAC Addr in to a BYTE array.
            if (wmic_ether_aton(argv[3], macAddr) == -1) {
                MessageBox(NULL,_T("Invalid Mac addr format"),NULL,MB_OK);
                break;
            }
      
            if (!DeviceIoControl(
                            hAdapter,
                            IOCTL_CAR6K_MACADDR_UPDATE,
                            macAddr, 
                            6,
                            NULL,
                            0,
                            &dwReturnedBytes,
                            NULL) )
            {
                MessageBox(NULL,_T("MACADDR UPDATE FAILED"),_T("eepromUpdater"),MB_OK);
            } else {
                MessageBox(NULL,_T("MACADDR UPDATE SUCCESS"),_T("eepromUpdater"),MB_OK);
            } 

        } else if ( _tcscmp( field, RD_FIELD) == 0 ) {
            if (wmic_rd_aton(argv[3], &rd) == -1) {
                MessageBox(NULL,_T("Invalid RD format"),NULL,MB_OK);
                break;
            }
            if (!DeviceIoControl(
                            hAdapter,
                            IOCTL_CAR6K_RD_UPDATE,
                            &rd, 
                            2,
                            NULL,
                            0,
                            &dwReturnedBytes,
                            NULL) )
            {
                MessageBox(NULL,_T("RD UPDATE FAILED"),_T("eepromUpdater"),MB_OK);
            } else {
                MessageBox(NULL,_T("RD UPDATE SUCCESS"),_T("eepromUpdater"),MB_OK);
            }
        } else {
            printUsage();
        }
    } while(0);

    CloseHandle (hAdapter);
    SetCursor(curr);
}
