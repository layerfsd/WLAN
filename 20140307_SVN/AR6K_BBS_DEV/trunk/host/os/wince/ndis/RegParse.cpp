//------------------------------------------------------------------------------
// <copyright file="RegParse.cpp" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// Get configuration values from Windows CE Registry for NDIS driver
//
// Author(s): ="Atheros"
//==============================================================================

#include <windows.h>
#include <ndis.h>

#include "htc_internal.h"
#include "htc.h"
#include "hif.h"
#include "wmi_api.h"
#include "netbuf.h"
#include "ndisnetbuf.h"
extern "C" {
#include "bmi.h"
}


#include "cmini.hpp"
#include "c802_3mini.hpp"
#include "c802_11mini.hpp"
#include "car6k.hpp"


typedef enum {
	DBG_ERR		= 1,
	DBG_TRACE	= 0
}RegParser;


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
    tSTR,
} REG_TYPE_ENUM;

//#define RV(_regName, _type, _struct_info, _default, _min, _max) \
//   { NDIS_STRING_CONST(#_regName), #_regName, _type, _struct_info, _default, _min, _max }


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
	RV(bkScanEnable,		tDEC,	CFG(bkScanEnable),			1,		0,		1),
	RV(bkScanPeriod,		tDEC,	CFG(bkScanPeriod),			5,		0,	65535),

	/* Security */
	RV(wpaEnabled,			tDEC,	CFG(wpaEnabled),			0,		0,		1),
	RV(wpa2Enabled,			tDEC,	CFG(wpa2Enabled),			0,		0,		1),

	/* Initialization Phase */
	RV(defaultApp,			tDEC,	CFG(defaultApp),			1,		0,		1),
	RV(byPassWmi  ,			tDEC,	CFG(byPassWmi),				0,		0,		1),
	RV(discTimeout,			tDEC,	CFG(discTimeout),			5,		0,		60),
	RV(resetPowerState,		tDEC,	CFG(resetPowerState),		1,		0,		1),
	RV(ibssChannel,			tDEC,	CFG(ibssChannel),			0,		0,		6000000),
	RV(powerSaveMode,		tDEC,	CFG(powerSaveMode),			2,		0,		2),
	RV(nodeAge,				tDEC,	CFG(nodeAge),				12000,	3000,	15000),
	RV(tcmd,				tDEC,	CFG(tcmd),					0,		0,		1),
	RV(dwellTime,			tDEC,	CFG(dwellTime),				120,	10,		5000),

	/* hidden ap */
	RV(probeEnable,			tDEC,	CFG(probeEnable),			0,		0,		1),
	RV(probeSSID,			tSTR,	CFG(probeSSID),				0,		0,		32),
	RV(probeSSIDa,			tSTR,	CFG(probeSSIDa),			0,		0,		32),

	/* specific AP */
	RV(apEnable,			tDEC,	CFG(apEnable), 				0,		0, 		1),
	RV(apSSID,				tSTR,	CFG(apSSID)	,				0,		0,		32),
	RV(apSSIDa,				tSTR,	CFG(apSSIDa),				0,		0,		32),

	/* Support Channel list */
	RV(channelList,			tSTR,	CFG(channelList),			0,		0,		96),

	/* Roaming */
	RV(hostAssistedRoaming, tDEC,	CFG(hostAssistedRoaming),	1,		0,		2),
	RV(RoamEnable,			tDEC,	CFG(RoamEnable),			1,		0,		1),
	RV(RoamFloorThre,		tDEC,	CFG(RoamFloorThre),			70,		0,		100),
	RV(RoamThreshold,		tDEC,	CFG(RoamThreshold),			75,		0,		100),
	RV(ScanThreshold,		tDEC,	CFG(ScanThreshold),			70,		0,		100),

	/* Debug */
	RV(dbgSetOid,			tDEC,	CFG(dbgSetOid),				0,		0,		1),
	RV(dbgQueryOid,			tDEC,	CFG(dbgQueryOid),			0,		0,		1),
	RV(dbgBSSEvent,			tDEC,	CFG(dbgBSSEvent),			0,		0,		1),
	RV(dbgBSSList,			tDEC,	CFG(dbgBSSList),			0,		0,		1),
	RV(dbgErrorLog,			tDEC,	CFG(dbgErrorLog),			0,		0,		1),
	RV(dbgFileLog,			tDEC,	CFG(dbgFileLog),			0,		0,		1),
	RV(dbgLogLevel,			tDEC,	CFG(dbgLogLevel),			1,		0,		4),
	RV(dbgLogDir,			tSTR,	CFG(dbgLogDir),				0,		0,		32),
	
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
	NDIS_STATUS		status;
	CONFIG_PARAM	*pParam;
	UINT			i;
	ULONG			value;
	NDIS_STRING		strVal;
	PUCHAR			basePtr;
	PUCHAR			fieldPtr;
	char			regName[32];

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

        strcpy(regName, pParam->RegAscName);

		NDIS_DEBUG_PRINTF(DBG_TRACE,"[%02d] %s .. \r\n", i, regName); 

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
        
	switch (pParam->RegVarType) 
	{
		case tSTR:
			if(found)
			{
				strVal = pParamValue->ParameterData.StringData;

				strVal.Buffer[strVal.Length+1] = 0;
				wcstombs((char *)fieldPtr, strVal.Buffer, wcslen(strVal.Buffer));
			}
			else
			{
				memset((char*)fieldPtr, 0, pParam->Max);
			}
			break;
			
		case tDEC:
			default:
			
            if (found) 
			{
				value = pParamValue->ParameterData.IntegerData;
            
				/* Validate that the value falls within the specified range */
                if (!useDefault && (value < pParam->Min || value > pParam->Max))
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

   return NDIS_STATUS_SUCCESS;
}


