/*
 * NDIS port of dhd command line utility.
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: dhdu_ndis.c,v 1.3.50.2 2009/10/20 03:02:31 Exp $
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <winioctl.h>
#include <malloc.h>
#include <assert.h>
#include <ntddndis.h>

#include <typedefs.h>
#include <wlioctl.h>
#include <epictrl.h>
#include <irelay.h>
#include <proto/ethernet.h>
#include <nuiouser.h>
#include <bcmendian.h>
#include <bcmutils.h>
#include <proto/802.11.h>
#include <oidencap.h>

#include <typedefs.h>
#include <dhdioctl.h>
#include "dhdu.h"

#define DEV_TYPE_LEN 4 /* length for devtype 'dhd' */

/* This handle needs to be passed to all "ir_" fuinctions in the
 * shared DLL.  It is global because most of the APIs here were written
 * when the handle was maintained as a global in the shared dll.
 */

static DWORD ndevs;
static ADAPTER devlist[10];

static int
dhd_ioctl(void *dhd, int cmd, void *buf, int len, bool set)
{
	DWORD dwlen;
	int error;

	dwlen = len;

	if (set) {
		error = (int)ir_setinformation((HANDLE)dhd,
			OID_DHD_IOCTLS + cmd, (PBYTE)buf, &dwlen);
		if (error != 0) {
			return IOCTL_ERROR;
		}
	} else {
		error = (int)ir_queryinformation((HANDLE)dhd,
			OID_DHD_IOCTLS + cmd, (PBYTE)buf, &dwlen);
		if (error != 0) {
			return IOCTL_ERROR;
		}
	}

	return 0;
}

int
dhd_get(void *dhd, int cmd, void *buf, int len)
{
	return dhd_ioctl(dhd, cmd, buf, len, FALSE);
}

int
dhd_set(void *dhd, int cmd, void *buf, int len)
{
	return dhd_ioctl(dhd, cmd, buf, len, TRUE);
}

void
display_err(PCHAR prefix, WINERR status)
{
	PCHAR   ErrStr;
	DWORD   ErrLen;

	ErrLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, status,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &ErrStr, 0, NULL);
	if (ErrLen > 0) {
		fprintf(stderr, "%s: Error 0x%x: %s -- %s\n", dhdu_av0, status, prefix, ErrStr);
		LocalFree(ErrStr);
	} else
		fprintf(stderr, "%s: Error 0x%x: %s -- Unknown error\n", dhdu_av0, status, prefix);
}

int
geterror(void)
{
	return (int)GetLastError();
}

char *
strerror(WINERR status)
{
	static char errmsg[128];
	PCHAR   ErrStr;
	DWORD   ErrLen;

	ErrLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, status,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &ErrStr, 0, NULL);
	if (ErrLen > 0) {
		_snprintf(errmsg, sizeof(errmsg), "Error 0x%x (%s)", status, ErrStr);
		LocalFree(ErrStr);
	} else
		_snprintf(errmsg, sizeof(errmsg), "Error 0x%x (Unknown)", status);

	return errmsg;
}

/* select the numbered adapter */
static WINERR
select_adapter(HANDLE irh, int adapter)
{
	WINERR status;
	ULONG i;
	PADAPTER		pdev;

	/* If adapter == -1, choose the first appropriate adapter. */
	if (adapter == -1) {
		for (i = 0; i < ndevs; i++) {
			pdev = &devlist[i];
			if (pdev->type == IR_WIRELESS) {
				adapter = i;
				break;
			}
		}

		if (i == ndevs) {
			fprintf(stderr, "%s: No wireless adapters were found\n", dhdu_av0);
			return ERROR_INVALID_HANDLE;
		}
	}

	if (adapter < 0 || (ULONG) adapter >= ndevs) {
		fprintf(stderr, "%s: Cannot find wireless adapter #%d\n", dhdu_av0, adapter);
		status = ERROR_INVALID_HANDLE;
	} else {
		pdev = &devlist[adapter];
		if (pdev->type != IR_WIRELESS) {
			fprintf(stderr,
			        "%s: Selected adapter #%d is not an BRCM wireless adapter\n",
			        dhdu_av0, adapter);
			status = ERROR_INVALID_HANDLE;
		} else {
			status = ir_bind(irh, pdev->name);
			if (status != ERROR_SUCCESS)
				display_err("select_adapter:ir_bind:", status);
		}
	}

	return status;
}

int
main(int argc, char **argv)
{
	HANDLE irh;
	WINERR err = ERROR_SUCCESS;
	int ap_mode = 0;
	cmd_t *cmd = NULL;
	int init_err = TRUE;
	char *ifname = NULL;
	int help = 0;
	int status;

	/* initialize irelay and select default adapter */
	irh = INVALID_HANDLE_VALUE;
	if ((err = ir_init(&irh)) != ERROR_SUCCESS)
		goto done;
	ndevs = ARRAYSIZE(devlist);
	if (((err = ir_adapter_list(irh, &devlist[0], &ndevs)) != ERROR_SUCCESS) ||
		((err = select_adapter(irh, -1)) != ERROR_SUCCESS))
		goto done;

	init_err = FALSE;

	for (++argv; *argv;) {

		/* command line option */
		if ((status = dhd_option(&argv, &ifname, &help)) == CMD_OPT) {
			/* print usage */
			if (help)
				break;
			/* select different adapter */
			if (ifname) {
				ir_unbind(irh);
				if ((err = select_adapter(irh, atoi(ifname))) != ERROR_SUCCESS)
					break;
				if (dhd_check((void *)irh))
					break;
			}
			continue;
		}

		if (dhd_check((void *)irh))
			break;

		/* search for command */
		for (cmd = dhd_cmds; cmd->name && strcmp(cmd->name, *argv); cmd++);

		/* defaults to using the set_var and get_var commands */
		if (cmd->name == NULL)
			cmd = &dhd_varcmd;

		/* do command */
		if (cmd->name)
			err = (WINERR) (*cmd->func)((void *) irh, cmd, argv);

		break;
	}

done:
	/* In case of COMMAND_ERROR, command has already printed an error message */
	if (!cmd)
		dhd_usage(NULL);
	else if (err == USAGE_ERROR)
		dhd_cmd_usage(cmd);
	else if (err == IOCTL_ERROR)
		display_err("main:", err);

	if (irh != INVALID_HANDLE_VALUE) {
		ir_unbind(irh);
		ir_exit(irh);
	}

	return err;
}
