//------------------------------------------------------------------------------
// <copyright file="regparse.cpp" company="Atheros">
//    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved.
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
// Get configuration values from Windows CE Registry for NDIS driver.
//
// Author(s): ="Atheros"
//==============================================================================
#include <windows.h>
#include <ndis.h>
#include "athdefs.h"
#include "a_types.h"
#include "a_osapi.h"
#include "htc_api.h"
#include "hif.h"
#include "wmi.h"
#include "wlan_api.h"
#include "wmi_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
#include "common_drv.h"
extern "C" {
#include "bmi.h"
}

#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"

/*
 * Some macros for ease of readability and organization
 */

#define FIELD_SIZE(_type, _field) (sizeof(((_type *)0)->_field))
#define NUM_REG_PARAM             (sizeof(paramTable) / sizeof(CONFIG_PARAM))

#define RT_ENUM_2_NDIS(_x)                     \
      ((_x) == tDEC ? NdisParameterInteger :   \
      NdisParameterString)

#define CFG(_x)     sCONFIG, FIELD_OFFSET(WLAN_STA_CONFIG, _x), FIELD_SIZE(WLAN_STA_CONFIG, _x), 0
#define GLOBAL(_x)  sNONE,   ((UINT)&_x),                       sizeof(_x),                      0
#define RV_DUMMY    0,       0,                                 0,                               0
#define GLOBAL(_x)  sNONE,   ((UINT)&_x),                       sizeof(_x),                      0

#define RV(_regName, _type, _struct_info, _default, _min, _max) \
    { NDIS_STRING_CONST(#_regName), #_regName, _type, _struct_info, _default, _min, _max }

/*
 * Enum and struct definitions
 */

typedef enum {
    sCONFIG,
    sNONE
} STRUCT_ENUM;

typedef enum {
    tDEC,         // Decimal integer, Currently only integer type is supported.
    tSTR
} REG_TYPE_ENUM;

typedef struct ConfigParam {
    NDIS_STRING   RegVarName;     // variable name text, NDIS-style
    char          *RegAscName;    // variable name text
    REG_TYPE_ENUM RegVarType;     // integer, string, etc.
    STRUCT_ENUM   StructureName;  // name of structure where the field is located
    UINT          FieldOffset;    // offset to structure field to be loaded
    UINT          FieldSize;      // size (in bytes) of the field
    UINT          Field2Offset;   // second offset field used for strings with separate length storage
    UINT          Default;        // default value if not in registry
    UINT          Min;            // minimum value allowed
    UINT          Max;            // maximum value allowed
    char          *DefaultStr;    // defaults to NULL in initializer list below`
} CONFIG_PARAM;


/****************************************************************************
 * Registry Parameters Table
 *
 * This table contains a list of all of the configuration parameters
 * that the driver supports.   The driver will attempt to find these
 * parameters in the registry and use the registry value for these
 * parameters. If the parameter is not found in the registry, then the
 * default value is used.
 */
 //buildercheck:registry params
static CONFIG_PARAM paramTable[] = {
/*------------------------------------------------------------------------------
 *- Registry Name --------- Type -- Where to store it --------- Deflt Min  Max -
 *------------------------------------------------------------------------------*/

	RV(dbgSetOid,        tDEC,   CFG(dbgSetOid),          0,     0,   1),
	RV(dbgQueryOid,        tDEC,   CFG(dbgQueryOid),          0,     0,   1),
	RV(dbgBSSEvent,        tDEC,   CFG(dbgBSSEvent),          0,     0,   1),
	RV(dbgBSSList,        tDEC,   CFG(dbgBSSList),          0,     0,   1),
	RV(dbgErrorLog,        tDEC,   CFG(dbgErrorLog),          0,     0,   1),
	RV(dbgFileLog,        tDEC,   CFG(dbgFileLog),          0,     0,   1),
	
     /* Connection Services */
     RV(bkScanEnable,        tDEC,   CFG(bkScanEnable),          1,     0,   1),
     RV(bkScanPeriod,        tDEC,   CFG(bkScanPeriod),          10,     0,  65535),

     /* Initialization Phase */
     RV(byPassWmi  ,         tDEC,   CFG(byPassWmi),             0,     0,   1),
     RV(discTimeout,         tDEC,   CFG(discTimeout),           6,    0,   60),
     RV(resetPowerState,     tDEC,   CFG(resetPowerState),       1,     0,   2),
     RV(suspendMode,         tDEC,   CFG(suspendMode),           0,     0,   2),
     RV(wifiOnOffMode,       tDEC,   CFG(wifiOnOffMode),         1,     0,   2),   //builder: org=0
     RV(currentPowerState,   tDEC,   CFG(currentPowerState),     1,     0,   1),
     RV(ibssChannel,         tDEC,   CFG(ibssChannel),           0,     0,   6000000),
     RV(powerSaveMode,       tDEC,   CFG(powerSaveMode),         2,     0,   2),	//builder: CAM=0, MAX=1, FAST=2, org=2
     RV(wmmConfig,           tDEC,   CFG(wmmConfig),             1,     0,   1),	//builderorg=1
     RV(enableUARTprint,     tDEC,   CFG(enableUARTprint),       0,     0,   1),
     RV(connectCtrlFlags,    tDEC,   CFG(connectCtrlFlags),      0,     0,   0xFFFF),
     RV(wowEnable,           tDEC,   CFG(wowEnable),             0,     0,  1),
     RV(tspecCompliance,     tDEC,   CFG(tspecCompliance),       1,     0,  1),

     RV(reduceCreditDribbleFlags, tDEC,  CFG(reduceCreditDribbleFlags), 0,  0, 4),

     RV(atimWindow,          tDEC,   CFG(atimWindow),            10,          0,   0xFF),
     RV(tcmd,                tDEC,   CFG(tcmd),                   0,          0,   0xFF),
     RV(BtCoexAntConfig,     tDEC,   CFG(BtCoexAntConfig),        0,          0,   6),
     RV(BtCoex11aDifAnt,     tDEC,   CFG(BtCoex11aDifAnt),        0,          0,   1),
     RV(BtDevType,           tDEC,   CFG(BtDevType),              0,          0,   6),
     RV(nodeAge,             tDEC,   CFG(nodeAge),                12000,     0,   120000),
     RV(clkFreq,             tDEC,   CFG(clkFreq),                26000000,   0,   40000000),
     RV(enableDbglog,        tDEC,   CFG(enableDbglog),           0,          0,   1),
     RV(ibssPSEnable,        tDEC,   CFG(ibssPSEnable),           0,          0,   1),
     RV(resumeDelay,         tDEC,   CFG(resumeDelay),            0,          0,   5000000),
     RV(fgScanEnable,        tDEC,   CFG(fgScanEnable),           1,          0,   1),
     RV(txPower,             tDEC,   CFG(txPower),                0,          0,   0x3F),
     RV(DbglogFlags,         tDEC,   CFG(DbglogFlags),            0xFFFF,     0,   0xFFFFFFFF),
     RV(hostAssistedRoaming, tDEC,   CFG(hostAssistedRoaming),    0,          0,   2),
     RV(suspendDelay,        tDEC,   CFG(suspendDelay),           0,          0,   5000000),
     RV(rekeyingPeriod,      tDEC,   CFG(rekeyingPeriod),         0,         10,  1000000),
     RV(DbgPrintMask,        tDEC,   CFG(DbgPrintMask),           0x00000020, 0,   0xFFFFFFFF),
     RV(delayedRxIndThresh,  tDEC,   CFG(delayedRxIndThresh),     0,          0,   16),
     RV(intrabssEnable,      tDEC,   CFG(intrabssEnable),         1,          0,   1),
     RV(enableDirectRx2Ndis, tDEC,   CFG(enableDirectRx2Ndis),    0,          0,   1),
     RV(enableActiveScan,      tDEC,   CFG(enableActiveScan),         1,          0,   1),
     RV(firmwareRoaming,      tDEC,   CFG(firmwareRoaming),         1,       0,   1),
     RV(actDwellTime,           tDEC,   CFG(actDwellTime),                  0,       0,   105),
     RV(pasDwellTime,            tDEC,   CFG(pasDwellTime),                  0,       0,   105),
     RV(drainTime,              tDEC,       CFG(drainTime),                  0,       0,   50),
     RV(forceScanInterval,     tDEC,   CFG(forceScanInterval),               0,    0,   105),
     RV(BmissCount,		tDEC,   CFG(BmissCount),   10,   0,   65535),
     RV(RoamEnable,		tDEC,   CFG(RoamEnable),   0,   0,   1),
     RV(ScanThreshold,		tDEC,   CFG(ScanThreshold),   70,   0,   95),
     RV(RoamThreshold,	tDEC,   CFG(RoamThreshold),   75,   0,   95),
     RV(RoamFloorThre,	tDEC,   CFG(RoamFloorThre),   65,   0,   95),
	RV(channelList,		tSTR,   CFG(channelList),   0,   0,   0),
	RV(ChannelMode,		tDEC,   CFG(ChannelMode),   2,   0,   5), // 0=11a only, 1=11a/g , 2=11bg , 3= 11g only, 4=11b ,  5=user def
	RV(dbgLogDir,		tSTR,   CFG(dbgLogDir),   0,   0,   0),
	RV(probeSSID,		tSTR,   CFG(probeSSID),   0,   0,   0),
	RV(probeSSIDa,		tSTR,   CFG(probeSSIDa),   0,   0,   0),
	RV(probeEnable,		tDEC,   CFG(probeEnable),   0,   0,   1),
	RV(tp,		tDEC,   CFG(tp),   0,   0,   248),
	RV(DwellTime,		tDEC,   CFG(DwellTime),   110,   0,   500),
	RV(lockSSID,		tSTR,   CFG(lockSSID),   0,   0,   0),
	RV(lockEnable,		tDEC,   CFG(lockEnable),   0,   0,   1),
};


void swap_string(char *temp , int length)
{
	int i,j;
	char swap;

	for(i=0;i<(length/2);i++) {
		j = length - i - 1;
		swap = temp[i];
		temp[i] = temp[j];
		temp[j] = swap;
	}
}

void parse_channel_List(PWSTR read_str ,WLAN_STA_CONFIG *pConfig ,int length)
{
	int i , j , find;
	char temp[6];

//	RETAILMSG(1, (TEXT("AR6K: parse_channel_List = [%d] \r\n"),length));

	memset(temp , '\0' , 8);
	j = 0;
	find = 0;

	for(i=0;i<=length;i++) {
		if(read_str[i] == ',' || i == length) { // find one char;
			temp[j] = '\0';
			// ascii to hex.
//			swap_string(temp , j);
			pConfig->channelList[find] = atoi(temp);
//			RETAILMSG(1, (TEXT("AR6K: one channel = [%d] \r\n"),pConfig->channelList[find]));

			find++;
			memset(temp , '\0' , 8);
			j =0;
		}
		else if(read_str[i] == ' ') {
			// white space 
			continue;
		}
		else {
			if(j < 8) {
				temp[j] = (char) read_str[i];
				j++;
			}
		}
	}

	pConfig->channel_cnt = find;
}

//-----------------------------------------------------------------------------
// Procedure:   ParseRegistryParameters
//
// Description: This routine will parse all of the parameters out of the
//              registry and store the values in the passed config structure.
//              Structure.  If the parameter is not present in the registry,
//              then the default value for the parameter will be placed into
//              the config structure.  This routine also checks the validity
//              of the parameter value.  If the value is out of range, the
//              default value will be used.
//-----------------------------------------------------------------------------
NDIS_STATUS
CAR6KMini::ParseRegistryParameters(NDIS_HANDLE ConfigHandle,
                        WLAN_STA_CONFIG *pConfig)
{
    NDIS_STATUS  status;
    CONFIG_PARAM *pParam;
    UINT         i;
    ULONG        value;
    PUCHAR       basePtr;
    PUCHAR       fieldPtr;
    PVOID        macAddr;
    UINT         macAddrLen;

    PNDIS_CONFIGURATION_PARAMETER pParamValue;

	PWSTR	 read_str;
	pConfig->channel_cnt = 0;


    /* Loop through the registry values specified in the above array */
    for (i = 0, pParam = paramTable; i < NUM_REG_PARAM; i++, pParam++) {
        BOOLEAN found;
        BOOLEAN useDefault = FALSE;

        switch (pParam->StructureName) {
        case sCONFIG:
            ASSERT(pConfig);
            basePtr = (PUCHAR)pConfig;
            break;
        case sNONE:
            basePtr = (PUCHAR)0;
            break;
        default:
            ASSERT(0);
        }

        fieldPtr = basePtr + pParam->FieldOffset;
        if (!fieldPtr)
        {
            continue;
        }

        /*
         * All of the registry parameters are stored as strings.
         * On NT 4, using NdisReadConfiguration with parameterType ==
         * NdisParameterInteger on a string will succeed (status wise), but
         * the parameter type returned will be string and the
         * buffer contents will be invalid.
         * To fix this, force NdisReadConfiguration to read all
         * parameters as strings, and then convert to integers as needed.
         */

        /* Get the configuration value for the parameter. */
        NdisReadConfiguration(&status, &pParamValue, ConfigHandle,
                              &pParam->RegVarName,
                              RT_ENUM_2_NDIS(pParam->RegVarType));

        found = (status == NDIS_STATUS_SUCCESS);

        /* Process the registry value based on type, currently
                only Integer type is supported  */

    switch (pParam->RegVarType) {
#if 0	 //--builder	
	case tSTR:
			if(found) {
				read_str = pParamValue->ParameterData.StringData.Buffer;
				//RETAILMSG(1, (TEXT("AR6K: parse channel List length = [%d] \r\n"),pParamValue->ParameterData.StringData.Length));
				parse_channel_List(read_str , pConfig , pParamValue->ParameterData.StringData.Length/2);
			}
			break;
#endif			
        case tDEC:
        default:
            if (found) {
                    value = pParamValue->ParameterData.IntegerData;


        /* Validate that the value falls within the specified range */
                if (!useDefault &&
                    (value < pParam->Min || value > pParam->Max))
                {
                    useDefault = TRUE;
                }
            }
            else {
                useDefault = TRUE;
            }

            if (useDefault) {
                /* A parameter wasn't present or was invalid */
                value = pParam->Default;
            }


            /* Store away the value into its proper spot */
            switch (pParam->FieldSize) {
            case sizeof(UCHAR):
                *((PUCHAR)fieldPtr)  = (UCHAR)value;
                break;

            case sizeof(USHORT):
                *((PUSHORT)fieldPtr) = (USHORT)value;
                break;

            case sizeof(ULONG):
                *((PULONG)fieldPtr)  = (ULONG)value;
                break;

            default:
                /* Needs to be one of the sizes above */
                ASSERT(0);
                break;
            }
            break;

        } // switch on overall type
    } // for loop for each config parameter

    {

	NDIS_STRING chList[] = NDIS_STRING_CONST("channelList");
	NdisReadConfiguration (&status, &pParamValue, ConfigHandle,
                               chList,
                               NdisParameterString);
	if (status == NDIS_STATUS_SUCCESS)
        {
		read_str = pParamValue->ParameterData.StringData.Buffer;
		parse_channel_List(read_str , pConfig , pParamValue->ParameterData.StringData.Length/2);
        }


	NDIS_STRING dbgLogRoot[] = NDIS_STRING_CONST("dbgLogDir");
        NdisReadConfiguration (&status, &pParamValue, ConfigHandle,
                               dbgLogRoot,
                               NdisParameterString);
	SYSTEMTIME sTime;
	char sdbgLogRoot[512];
	
	GetSystemTime(&sTime);
	
        if (status == NDIS_STATUS_SUCCESS)
        {
        	if( (pParamValue->ParameterData.StringData.Length/2) < 6 )
        		{
        		wcscpy (pConfig->dbgLogDir, L"\\Windows");
        		}
		else
			{
			wcscpy (pConfig->dbgLogDir, pParamValue->ParameterData.StringData.Buffer);
			}
        }
        else
        {
		wcscpy (pConfig->dbgLogDir, L"\\Windows");
        }
	wcstombs(sdbgLogRoot, pConfig->dbgLogDir, 128);
	sprintf( pConfig->dbgLogFName, "%s\\ar6klog%02d%02d%02d%04d.txt", sdbgLogRoot, sTime.wHour, sTime.wDay, sTime.wMonth, sTime.wYear );

	//RETAILMSG(1, (TEXT("builder:ar6k2:regparse: dbgLogPath=%S\r\n"), pConfig->dbgLogFName));    

	// probe SSID(hidden SSID)
	NDIS_STRING probessid[] = NDIS_STRING_CONST("probeSSID");
        NdisReadConfiguration (&status, &pParamValue, ConfigHandle,
                               probessid,
                               NdisParameterString);
	wchar_t pssid[128];
	
	if (status == NDIS_STATUS_SUCCESS)
        {
		wcscpy (pssid, pParamValue->ParameterData.StringData.Buffer);
		wcstombs(pConfig->probeSSID, pssid, 128);
		//RETAILMSG(1, (TEXT("builder:ar6k2:regparse: probeSSID=%S\r\n"), pConfig->probeSSID));
        }

	// probe SSIDa(hidden SSID)
	NDIS_STRING probessida[] = NDIS_STRING_CONST("probeSSIDa");
        NdisReadConfiguration (&status, &pParamValue, ConfigHandle,
                               probessida,
                               NdisParameterString);
	wchar_t pssida[128];
	
	if (status == NDIS_STATUS_SUCCESS)
        {
		wcscpy (pssida, pParamValue->ParameterData.StringData.Buffer);
		wcstombs(pConfig->probeSSIDa, pssida, 128);
		//RETAILMSG(1, (TEXT("builder:ar6k2:regparse: probeSSID=%S\r\n"), pConfig->probeSSID));
        }

	// probe SSIDa(hidden SSID)
	NDIS_STRING lockssid[] = NDIS_STRING_CONST("lockSSID");
        NdisReadConfiguration (&status, &pParamValue, ConfigHandle,
                               lockssid,
                               NdisParameterString);
	wchar_t plockssid[128];
	
	if (status == NDIS_STATUS_SUCCESS)
        {
		wcscpy (plockssid, pParamValue->ParameterData.StringData.Buffer);
		wcstombs(pConfig->lockSSID, plockssid, 128);
		//RETAILMSG(1, (TEXT("builder:ar6k2:regparse: lockSSID=%S\r\n"), pConfig->lockSSID));
        }
	
		
        // Binary file path
        NDIS_STRING strPath[] = NDIS_STRING_CONST("binRoot");

        /* Get the configuration value for the parameter. */
        NdisReadConfiguration (&status, &pParamValue, ConfigHandle,
                               strPath,
                               NdisParameterString);

        if (status == NDIS_STATUS_SUCCESS)
        {
            // use the registry entry
            wcscpy (pConfig->binRoot, pParamValue->ParameterData.StringData.Buffer);
        }
        else
        {
            // use the default one
            wcscpy (pConfig->binRoot, L"\\Windows");
        }


        /* Get the configuration value for the parameter. */
        NDIS_STRING eepFileStr[] = NDIS_STRING_CONST("eepromFile");
        NdisReadConfiguration (&status, &pParamValue, ConfigHandle,
                               eepFileStr,
                               NdisParameterString);

        if (status == NDIS_STATUS_SUCCESS)
        {
            // use the registry entry
            wcscpy (pConfig->eepromFile, pParamValue->ParameterData.StringData.Buffer);
        }
        else
        {
            // use the default one
            wcscpy (pConfig->eepromFile, TEXT("\0"));
        }

        NdisReadNetworkAddress(&status, &macAddr, &macAddrLen, ConfigHandle);
        if ((status == NDIS_STATUS_SUCCESS) &&
            (macAddrLen == ETHERNET_MAC_ADDRESS_LENGTH))
        {
            NdisMoveMemory(pConfig->swMacAddr, macAddr, ETHERNET_MAC_ADDRESS_LENGTH);
        } else {
#ifdef GENERATE_MAC_ADDRESS
            if ( !wcscmp(pConfig->eepromFile, TEXT("\0")) )
#endif
            NdisMoveMemory(pConfig->swMacAddr, NullMacAddr, ETHERNET_MAC_ADDRESS_LENGTH);
#ifdef GENERATE_MAC_ADDRESS
            else
            {
                //generate MAC address when using eepromFile
                srand(GetTickCount());
                pConfig->swMacAddr[0] = 0x00;
                pConfig->swMacAddr[1] = 0x03;
                pConfig->swMacAddr[2] = 0x7f;
                pConfig->swMacAddr[3] = (unsigned char) rand();
                pConfig->swMacAddr[4] = (unsigned char) rand();
                pConfig->swMacAddr[5] = (unsigned char) rand();
            }
#endif
        }
    }

   return NDIS_STATUS_SUCCESS;
}


