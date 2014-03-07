/*
 * Shell command implementation for WinCE
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: shellproc_ce.c,v 1.1.6.1 2008/06/06 13:12:12 Exp $
 */

#define NEED_IR_TYPES

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <bcmcdc.h>
#include <proto/802.11.h>
#include "wlu_remote.h"

__inline wchar_t *wmemset(wchar_t *_S, wchar_t _C, size_t _N)
	{wchar_t *_Su = _S;
	for (; 0 < _N; ++_Su, --_N)
		*_Su = _C;
	return (_S); }

#define stricmp _stricmp
#define strnicmp _strnicmp
#define strrev _strrev

#define MAX_ARGS_LENGTH          4096

#define MAX_FILE_NAME_INDEX      2
#define MAX_FILE_NAME_SIZE	         30
#define SHELL_RESP_PATH			 "Temp\\RWL"
#define SHELL_FILE_PATH			 "Temp\\RWL\\tmp1x2x.txt"

/* Some WIN CE APIS need widecharacters strings as arguments */
#define WC_SHELL_RESP_PATH		 L"Temp\\RWL"
#define WC_SHELL_FILE_PATH		 L"Temp\\RWL\\tmp1x2x.txt"

/* Function prototypes */

static int remote_shell_sync_exec(char *cmd_buf_ptr);
static int rwl_get_file_size(char *buf_ptr);

/* Global variables */

static char shell_fname[MAX_FILE_NAME_SIZE];

/* Open the command processor and execute the shell command
 * passed in the buf_ptr
 */
static int
rwl_shell_exe(char *buf_ptr)
{
	PROCESS_INFORMATION ProcessInfo;
	char Args[MAX_ARGS_LENGTH];
	wchar_t wc_Args[MAX_ARGS_LENGTH];
	wchar_t *pDefaultCMD = L"CMD.EXE";
	int errno;

	Args[0] = 0;
	wmemset(wc_Args, 0, MAX_ARGS_LENGTH);

	/*  "/C" option - Do the command and EXIT the command processor */
	sprintf(Args, "/C %s", (const char *)buf_ptr);
	/* Convert multibyte string to wide character */
	mbstowcs(wc_Args, Args, strlen(Args));

	/* Invoke the command processor and execute the shell command */
	if (!CreateProcess(pDefaultCMD, wc_Args, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, \
		NULL, NULL, NULL, &ProcessInfo)) {
		errno = GetLastError();
		DPRINT_DBG(OUTPUT, "rwl_shell_exe:Error in createprocess under_ce\n");
		return errno;
	}

	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	errno = GetLastError();
	DPRINT_DBG(OUTPUT, "rwl_shell_exe:errno=%d\n", errno);
	return errno;
}

/* 
 * Given a file name this function returns the size of the file
 */
static int
rwl_get_file_size(char* buf_ptr)
{
	DWORD filesize = 0;
	HANDLE hndFile;

	if ((hndFile = CreateFile(WC_SHELL_FILE_PATH, GENERIC_READ, FILE_SHARE_READ, NULL, \
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		DPRINT_DBG(OUTPUT, "\n Error opening async file\n");
		strcpy((char*)buf_ptr, "File open error\n");
		return filesize;
	}

	filesize = GetFileSize(hndFile, NULL);

	CloseHandle(hndFile);

	return filesize;
}

/* 
 * Function to get the response of the shell cmd from the file and
 * copies the result in buf_ptr for WinCE OS
 */
int
remote_shell_get_resp(char* shell_fname, char *buf_ptr, int msg_len)
{
	int resp_size;
	FILE *shell_fpt;

	if ((shell_fpt = fopen(SHELL_FILE_PATH, "rb")) == NULL) {
		DPRINT_DBG(OUTPUT, "\n Error opening async file\n");
		strcpy((char*)buf_ptr, "File open error\n");
		return FAIL;
	}
	/* copy the file into the buffer: */
	resp_size  = fread(buf_ptr, sizeof(char), msg_len, shell_fpt);

	DPRINT_DBG(OUTPUT, "remote_shell_get_resp:resp_size after read=%d\n", resp_size);

	fclose(shell_fpt);

	/* DeleteFile() requires Wide-Character String for the File path */
	if (!DeleteFile(WC_SHELL_FILE_PATH)) {
		DPRINT_DBG(OUTPUT, "\n Unable to delete file %d\n", GetLastError());
		return FAIL;
	}
	if (resp_size != msg_len) {
		strcpy((char*)buf_ptr, "Shell Resp:Reading error\n");
		return FAIL;
	}

	return resp_size;
}

int
rwl_create_dir(void)
{
	/* CreateDirectory() requires the arguement to be in Wide-Character Format */
	if (!CreateDirectory(WC_SHELL_RESP_PATH, NULL)) {
		/* If directory already exists then it is not a failure */
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			DPRINT_ERR(ERR, "\n Error Creating the Directory = %d\n", GetLastError());
			return FAIL;
		}
	}
	return SUCCESS;
}

/*
 * Function for executing synchronous shell comamnd
 */
static int
remote_shell_sync_exec(char *buf_ptr)
{
	int sts, filesize;
	char *loc_buf_ptr;

	/* Copy buf_ptr into loc_buf_ptr */
	if ((loc_buf_ptr = (char*)malloc(strlen(buf_ptr))) == NULL) {
		DPRINT_DBG(OUTPUT, "\n Not able to allocate memory for local buffer\n");
		return FAIL;
	}

	strncpy(loc_buf_ptr, buf_ptr, strlen(buf_ptr));
	/* Process for other commands */
	strcat((char*)buf_ptr, " >");
	strcat((char*)buf_ptr, SHELL_FILE_PATH);

	if ((sts = rwl_shell_exe((char*)buf_ptr)) == -1) {
		DPRINT_DBG(OUTPUT, "\n Not able to execute shell cmd\n");
		free(loc_buf_ptr);
		return FAIL;
	}

	strcpy((char*)buf_ptr, SHELL_FILE_PATH);

	/* The filesize would be zero in the case of commands not suppoted or if
	 * commands do not have any response in WinCE 5.0.
	 * In such case No shell response string will be sent back to client.
	 */
	if ((filesize = rwl_get_file_size(buf_ptr)) == 0) {
		strcpy((char*)buf_ptr, "NO SHELL RESPONSE\n");
		/* DeleteFile() requires Wide-Character String for the File path */
		if (!DeleteFile(WC_SHELL_FILE_PATH)) {
			DPRINT_DBG(OUTPUT, "\n Unable to delete file %d\n", GetLastError());
			return FAIL;
		}
	}

	/* Free the loc_buf_ptr */
	free(loc_buf_ptr);
	return filesize;
}

/* Main function for shell command execution */
int
remote_shell_execute(char *buf_ptr)
{
	int msg_len;

	msg_len = remote_shell_sync_exec(buf_ptr);

	return msg_len;
}
