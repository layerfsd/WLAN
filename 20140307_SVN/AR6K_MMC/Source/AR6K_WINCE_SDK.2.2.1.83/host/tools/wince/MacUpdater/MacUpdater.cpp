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

TCHAR adapterName[256];
TCHAR cmdLine[256];

void _tmain(int argc, TCHAR *argv[]) {

	HCURSOR curr = GetCursor();
	DWORD   dwErrorCode = 0x00;
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	BOOL status=FALSE;

	PROCESS_INFORMATION pi;
	DWORD dwExitCode = 0;

	BYTE macAddr[6];

	FILE *fd=NULL;
	int pos=0;
    TCHAR AR6KPart[25]=_T("eeprom");
    BOOL UpdateEeprom = TRUE;
    BOOL UpdateFlash = FALSE;

    A_UINT8 buffer[512];
    A_UINT16 *ptr=NULL;
    A_UINT16 count=0;
    A_UINT16 checksum=0;
#ifdef SST_FLASH
	TCHAR AltFlashPart[]=_T(" -p 0x80004000 ") ;
#else
	TCHAR AltFlashPart[]=_T(" ") ;
#endif //SST_FLASH

	if (argc < 3 || argc > 5) {
		MessageBox(NULL,_T("Usage: MacUpdater <Adapter Name> <MacAddr> [<eeprom|flash|eeprom flash>] e.g 'MacUpdater AR6K_SD1 00:03:7f:01:e0:27 eeprom'"),NULL,MB_OK);
		exit(0);
	}

    _tcsncpy(adapterName, argv[1],255);

	// Parse the MAC Addr in to a BYTE array.
    if (wmic_ether_aton(argv[2], macAddr) == -1) {
        MessageBox(NULL,_T("Invalid Mac addr format"),NULL,MB_OK);
	    exit(0);
    }

        // Get the part to update EEPROM | Flash
        if (argc >= 4) {

			UpdateEeprom = FALSE;

            _tcsncpy(AR6KPart, argv[3], 24);
            AR6KPart[24] = __T('\0');
            _tcslwr(AR6KPart);

            if (_tcscmp(AR6KPart,_T("eeprom")) && _tcscmp(AR6KPart,_T("flash"))) {
                MessageBox(NULL,_T("Usage: MacUpdater <Adapter Name> <MacAddr> [<eeprom|flash|eeprom flash>] e.g 'MacUpdater AR6K_SD1 00:03:7f:01:e0:27 flash' for SD, 'MacUpdater PCCARD\\pnp-id 00:03:7f:01:e0:27 flash' for CF"),NULL,MB_OK);
		exit(0);
            } else if (!_tcscmp(AR6KPart,_T("eeprom")) ) {

                UpdateEeprom = TRUE;
            } else if (!_tcscmp(AR6KPart,_T("flash")) ) {

                UpdateFlash = TRUE;
            }
        }

        if (argc == 5) {
            _tcsncpy(AR6KPart, argv[4], 24);
            AR6KPart[24] = __T('\0');
            _tcslwr(AR6KPart);

            if (_tcscmp(AR6KPart,_T("eeprom")) && _tcscmp(AR6KPart,_T("flash"))) {
                MessageBox(NULL,_T("Usage: MacUpdater <Adapter Name> <MacAddr> [<eeprom|flash|eeprom flash>] e.g 'MacUpdater AR6K_SD1 00:03:7f:01:e0:27 flash' for SD, 'MacUpdater PCCARD\\pnp-id 00:03:7f:01:e0:27 flash' for CF"),NULL,MB_OK);
		exit(0);
            } else if (!_tcscmp(AR6KPart,_T("eeprom")) ) {

                UpdateEeprom = TRUE;
            } else if (!_tcscmp(AR6KPart,_T("flash")) ) {

                UpdateFlash = TRUE;
            }
        }


    if (UpdateEeprom) {
        // Update MAC addr in buffer
        HANDLE hAdapter = NULL;
        DWORD dwReturnedBytes   = 0;

        hAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                    INVALID_HANDLE_VALUE);

        if (NULL == hAdapter) {
            MessageBox(NULL,_T("Unable to open the driver"),NULL,MB_OK);
            exit(-1);
        }
        
        if ( !DeviceIoControl(
                hAdapter,
                IOCTL_CAR6K_MACADDR_UPDATE,
                macAddr, 
                6 ,
                NULL,
                0,
                &dwReturnedBytes,
                NULL) )
        {
            MessageBox(NULL,_T("IOCTL_CAR6K_MACADDR_UPDATE failed"),NULL,MB_OK);
            exit(-1);
        }

        MessageBox(NULL,_T("EEPROM Mac Address Update Successful"),NULL,MB_OK);
        CloseHandle (hAdapter);
    } // EEPROM

	//-------------Update MAC Addr in Flash-------
	// Load flash.ram.bin app on to the target.

   if (UpdateFlash) {

    _tcscpy(cmdLine, _T("-i "));
    _tcscat(cmdLine,adapterName);

    _tcscat(cmdLine,_T(" -w -a 0x80008c00 -f ecos.ram.bin"));

	if(!CreateProcess(_T("bmiloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: bmiloader.exe -w -a 0x80009000 -f ecos.ram.bin"),NULL,MB_OK);
		exit(0);
	}

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("01 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	//------------- BREAK LINE --------------------

    _tcscpy(cmdLine, _T("-i "));
    _tcscat(cmdLine,adapterName);

	_tcscat(cmdLine,_T(" -w -a 0x80002000 -f flash.ram.bin"));

	if(!CreateProcess(_T("bmiloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: bmiloader.exe -w -a 0x80002000 -f flash.ram.bin"),NULL,MB_OK);
		exit(0);
	}

	//Reset the status variables
	status = FALSE;
	dwExitCode = 0;

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("03 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

//------------- BREAK LINE --------------------
#ifdef SST_FLASH

    _tcscpy(cmdLine, _T("-i "));
    _tcscat(cmdLine,adapterName);

	_tcscat(cmdLine,_T(" -w -a 0x80004000 -f flashpart.ram.bin"));

	if(!CreateProcess(_T("bmiloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: bmiloader.exe -w -a 0x80002000 -f flash.ram.bin"),NULL,MB_OK);
		exit(0);
	}

	//Reset the status variables
	status = FALSE;
	dwExitCode = 0;

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("03 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

#endif //SST_FLASH

	//------------- BREAK LINE --------------------

    _tcscpy(cmdLine, _T("-i "));
    _tcscat(cmdLine,adapterName);

	_tcscat(cmdLine,_T(" -s -a 0xac0140c0 -p 0x9"));

	if (!CreateProcess(_T("bmiloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: bmiloader.exe -s -a 0xac0140c0 -p 0x9"),NULL,MB_OK);
		exit(0);
	}

	//Reset the status variables
	status = FALSE;
	dwExitCode = 0;

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("05 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	//------------- BREAK LINE --------------------
    _tcscpy(cmdLine, _T("-i "));
    _tcscat(cmdLine,adapterName);

	_tcscat(cmdLine,_T(" -b -a 0xa0008c00"));

	if(!CreateProcess(_T("bmiloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: bmiloader.exe -b -a 0xa0009000"),NULL,MB_OK);
		exit(0);
	}

	//Reset the status variables
	status = FALSE;
	dwExitCode = 0;

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("07 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	//--------------Read boarddata from Flash into file------------

	_tcscpy(cmdLine,_T("-i "));
    _tcscat(cmdLine, adapterName);
	_tcscat(cmdLine, AltFlashPart);

	_tcscat(cmdLine,_T(" -r -b -f flashboarddata.bin"));

        if(!CreateProcess(_T("flashloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: flashloader.exe reading boarddata failed"),NULL,MB_OK);
		exit(0);
	}

	//Reset the status variables
	status = FALSE;
	dwExitCode = 0;

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("11 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

//--------------Read & Save Dataset index pointer------------

	_tcscpy(cmdLine,_T("-i "));
    _tcscat(cmdLine, adapterName);
	_tcscat(cmdLine, AltFlashPart);

	_tcscat(cmdLine,_T(" -r -a 0xa207fdfc -l 4 -f temp.bin"));

        if(!CreateProcess(_T("flashloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: flashloader.exe reading Dataset Index pointer failed"),NULL,MB_OK);
		exit(0);
	}

	//Reset the status variables
	status = FALSE;
	dwExitCode = 0;

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("11 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);



    //----------------Modify the MAC Addr in the file---------------

    if ((fd = _tfopen(_T("\\Windows\\flashboarddata.bin"),_T("r+b"))) == NULL) {
        SetCursor(curr);
		MessageBox(NULL,_T("03 Unable to open flashboardata.bin"),NULL,MB_OK);
		exit(0);
	}

        // Read the 512 bytes of calib data from the file.
        if ( fread(buffer, 1, 512, fd) != 512 ) {
            SetCursor(curr);
	    MessageBox(NULL,_T("06 unable to read flashboardata.bin"),NULL,MB_OK);
	    exit(0);
        }

        // Update MAC addr in buffer
        memcpy(buffer+6, macAddr, 6);

        checksum = 0;
        // Calculate new CRC
        for (ptr = (A_UINT16 *)buffer+1,count=0;count<255;count++) {

             checksum ^= *ptr++;
        }

        checksum = ~checksum;

        ptr = (A_UINT16 *)buffer;
        *ptr = checksum;

        if (fseek(fd, 0, SEEK_SET) != 0) {
            SetCursor(curr);
            MessageBox(NULL,_T("08 failed fseek"),NULL,MB_OK);
            exit(0);
        }

        // Write back buffer to file
        if ( fwrite(buffer, 1, 512, fd) != 512) {
            SetCursor(curr);
	    MessageBox(NULL,_T("07 unable to write flashboardata.bin"),NULL,MB_OK);
	    exit(0);
        }

	fclose(fd);

	//----------Write back the modified board data from file to the FLASH-------


    _tcscpy(cmdLine,_T("-i "));
    _tcscat(cmdLine, adapterName);

	_tcscat(cmdLine, AltFlashPart);

	_tcscat(cmdLine,_T(" -w -b -f flashboarddata.bin"));

	if(!CreateProcess(_T("flashloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: flashloader.exe writing boarddata failed"),NULL,MB_OK);
		exit(0);
	}

	//Reset the status variables
	status = FALSE;
	dwExitCode = 0;

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("11 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

//--------------Restore Dataset index pointer------------

	_tcscpy(cmdLine,_T("-i "));
    _tcscat(cmdLine, adapterName);
	_tcscat(cmdLine, AltFlashPart);

	_tcscat(cmdLine,_T(" -w -a 0xa207fdfc -f temp.bin"));

        if(!CreateProcess(_T("flashloader.exe\0"),cmdLine,NULL,NULL,FALSE,0,NULL,NULL,NULL,&pi))
	{
		SetCursor(curr);
		MessageBox(NULL,_T("FAILED: flashloader.exe restoring DataSet Inder pointer failed"),NULL,MB_OK);
		exit(0);
	}

	//Reset the status variables
	status = FALSE;
	dwExitCode = 0;

	while ((status = GetExitCodeProcess(pi.hProcess, &dwExitCode)))
	{
		// Busy Wait till the process completes.
		if (STILL_ACTIVE != dwExitCode)
		{
			break;
		}
	}

	if ( status == FALSE ) {
        dwErrorCode = 0x00;
		dwErrorCode = GetLastError();
		RETAILMSG (1,(L"GetExitCodeProcess Failed, Error code %x\n", dwErrorCode));
	}

	if (FLEC_SUCCESS != dwExitCode)
	{
		SetCursor(curr);
		MessageBox(NULL,_T("11 Please reboot and run MacUpdater again"),NULL,MB_OK);
		exit(0);
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

    } //FLASH


	SetCursor(curr);
	MessageBox(NULL,_T("MACAddress Updated !"),_T("DONE"),MB_OK);
}
