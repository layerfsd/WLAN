Atheros MTE support library Readme

Copyright 2008 Atheros Communications Inc.


INTRODUCTION
============

MTE (Manufacturing Test Environment) library provides a light weight driver implementation that is
suitable for use in bootloaders.  The implementation uses a subset of BMI, HTC, WMI functionality for
non-threaded (non-interrupt based) applications in the bootloader.  The implementation assumes 
very minimal support in the bootloader.  Only some basic C-library functions 
(memset,memcpy,strlen etc) are required.

The MTE library performs a basic test with minimal foot print requirements. This test can be extended
by the OEM. The test currently performs the following operations:

1. Initializes the SDIO interface (currently SDIO supported)
2. BMI phase handling (download patches, set configuration, firmware download, eeprom transfer, boot)
3. HTC setup
4. WMI setup
5. Connection to user supplied SSID and channel number. No security. 
6. DHCP request (to AP) and DHCP Offer (from AP) checking (timeout of 5 seconds)
7. Capture of RSSI value during the DHCP test.


IMPLEMENTATION
==============

The MTE library defines the following test entry point function:

A_BOOL RunAR6kTest(A_UINT32             Channel, 
                   A_CHAR               *pSSID, 
                   MTE_WIFI_TEST_STATUS *pTestStatus);

The entire test runs to completion within this API call.  The caller must provide channel  number
and SSID string.  The test will return a test status structure containing the results of the test.
(see wifitest.h)
 
 
OEM CUSTOMIZATION
=================

The core code uses some standard APIs provided in most Windows CE bootloaders such as:

    OEMEthGetSecs()
    EdbgOutputDebugString()

The OEM must provide some platform specific APIs in the bootloader. 

    bootldrlib\bootldr.c - OEM must implement platform APIs for such things as millisecond delay and 
                           obtaining the firmware (as a blob or some other means).
                       
    bootldrlib\sdbus.c - OEM must provide SD APIs to issue commands to the host controller.
   
When linking to the bootloader executable, the OEM must link:

   ar6k_mte_bootlib.lib - OEM customized library (automatically includes ar6k_mte_core.lib)


MTE TEST EMULATION
==================

Debugging new MTE test features can be challenging in the bootloader. The MTE library can be compiled
into a special test application that can access the SDIO card directly.  This test application can run
directly on the Windows CE system.  The driver code must be compiled with /DAR6K_MTE_DRV_TEST added 
to sources.inc.  This enables a special bus pass-thru mechanism to allow  applications to directly 
issue SDIO bus commands to the underlying device. 

To run this test, the Atheros service must be told not to load any applications.  The following
registry key can be used.

[HKEY_LOCAL_MACHINE\Services\ATHSRVC]
   "appId"=dword:0 ; no app

Once the AR6K driver loads the driver will remain idle without loading any WLAN firmware
or mounting an NDIS adapter.  The user can then execute mte_test.exe from a windows CE command shell.
The source code to this test is located in \tools\wince\mte\testapp.



