/*------------------------------------------------------------------------------ */
/* <copyright file="athdrv.h" company="Atheros"> */
/*    Copyright (c) 2004-2008 Atheros Corporation.  All rights reserved. */
/*  */
/* This program is free software; you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License version 2 as */
/* published by the Free Software Foundation; */
/* */
/* Software distributed under the License is distributed on an "AS */
/* IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or */
/* implied. See the License for the specific language governing */
/* rights and limitations under the License. */
/* */
/* */
/*------------------------------------------------------------------------------ */
/*============================================================================== */
/* Author(s): ="Atheros" */
/*============================================================================== */
#ifndef _ATHDRV_H_
#define _ATHDRV_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <ntddndis.h>
/*                         ====WinCE OIDS for BMI====                        */
#define OID_CAR6K_BMI_DONE 0xFF000001
#define OID_CAR6K_BMI_WRITE_MEMORY 0xFF000002
#define OID_CAR6K_BMI_TEST 0xFF000003
#define OID_CAR6K_BMI_EXECUTE 0xFF000004
#define OID_CAR6K_BMI_SET_APP_START 0xFF000005
#define OID_CAR6K_BMI_WRITE_SOC_REGISTER 0xFF000006
#define OID_CAR6K_BMI_READ_MEMORY 0xFF000007
#define OID_CAR6K_BMI_READ_SOC_REGISTER 0xFF000008

/*                         ====WinCE OIDS for HTC RAW I/F====                */
#ifdef HTC_RAW_INTERFACE
#define OID_CAR6K_HTC_RAW_OPEN 0xFF000009
#define OID_CAR6K_HTC_RAW_CLOSE 0xFF000010
#define OID_CAR6K_HTC_RAW_READ 0xFF000011
#define OID_CAR6K_HTC_RAW_WRITE 0xFF000012
#endif //HTC_RAW_INTERFACE

#define OID_CAR6K_802_11_AUTH_ALG 0xFF000020
#define OID_CAR6K_GET_PNP_POWER   0xFF000021
#define OID_CAR6K_RESUME_WLAN_STATE 0xFF000022
#define OID_CAR6K_FIRMWARE_VERSION	0xFF000023




#ifdef __cplusplus
}
#endif

#endif /* _ATHDRV_H_ */
