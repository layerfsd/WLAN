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
static CONFIG_PARAM paramTable[] = {
/*------------------------------------------------------------------------------
 *- Registry Name --------- Type -- Where to store it --------- Deflt Min  Max -
 *------------------------------------------------------------------------------*/

     /* Connection Services */
     RV(bkScanEnable,        tDEC,   CFG(bkScanEnable),          1,     0,   1),
     RV(bkScanPeriod,        tDEC,   CFG(bkScanPeriod),          60,     0,  65535),

     /* Initialization Phase */
     RV(byPassWmi  ,         tDEC,   CFG(byPassWmi),             0,     0,   1),
     RV(discTimeout,         tDEC,   CFG(discTimeout),           10,    0,   60),
     RV(resetPowerState,     tDEC,   CFG(resetPowerState),       1,     0,   2),
     RV(suspendMode,         tDEC,   CFG(suspendMode),           0,     0,   2),
     RV(wifiOnOffMode,       tDEC,   CFG(wifiOnOffMode),         0,     0,   2),
     RV(currentPowerState,   tDEC,   CFG(currentPowerState),     1,     0,   1),
     RV(ibssChannel,         tDEC,   CFG(ibssChannel),           0,     0,   6000000),
     RV(powerSaveMode,       tDEC,   CFG(powerSaveMode),         2,     0,   2),
     RV(wmmConfig,           tDEC,   CFG(wmmConfig),             1,     0,   1),
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
     RV(nodeAge,             tDEC,   CFG(nodeAge),                120000,     0,   120000),
     RV(clkFreq,             tDEC,   CFG(clkFreq),                26000000,   0,   40000000),
     RV(enableDbglog,        tDEC,   CFG(enableDbglog),           1,          0,   1),
     RV(ibssPSEnable,        tDEC,   CFG(ibssPSEnable),           0,          0,   1),
     RV(resumeDelay,         tDEC,   CFG(resumeDelay),            0,          0,   5000000),
     RV(fgScanEnable,        tDEC,   CFG(fgScanEnable),           1,          0,   1),
     RV(txPower,             tDEC,   CFG(txPower),                0,          0,   0x3F),
     RV(DbglogFlags,         tDEC,   CFG(DbglogFlags),            0xFFFF,     0,   0xFFFFFFFF),
     RV(hostAssistedRoaming, tDEC,   CFG(hostAssistedRoaming),    0,          0,   1),
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

// Added By psh
	RV(DisconnectIfNoDataPkt,			tDEC,	CFG(DisconnectIfNoDataPkt),				1,	0,	1),
	RV(DisconnectIfNoDataPktCheckIntv,	tDEC,	CFG(DisconnectIfNoDataPktCheckIntv),	60,	5,	180),

};


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


