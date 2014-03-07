/*
 * File Name: wlu_server_shared.c
 * Common server specific functions for linux and win32
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: wlu_server_shared.c,v 1.1.6.9.2.6 2009/09/25 00:32:03 Exp $
 */

/* Revision History:
 *
 * Date        Author         Description
 *
 * 10-March-2008 Suganthi    version 0.1
 *
 */

/* 
 * Description: Main Server specific wrappers
 * This module implements all the server specific functions
 * for Win32 and Linux
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef SYMBIAN
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <malloc.h>
#endif /* SYMBIAN */
#include <assert.h>
#include <typedefs.h>

#include <winsock2.h>
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#endif /* WIN32 */

#ifdef WIN32
#include <windows.h>
#include <winioctl.h>
#include <ntddndis.h>
#include <typedefs.h>
#include <epictrl.h>
#include <irelay.h>
#include <proto/ethernet.h>
#include <nuiouser.h>
#include <bcmendian.h>
#include <bcmutils.h>
#include <proto/802.11.h>
#endif

#include <bcmcdc.h>
#include <wlioctl.h>
#include "wlu_remote.h"
#include "wlu_pipe.h"

#ifdef RWLASD
/* streams' buffers */
BYTE *xcCmdBuf = NULL, *parmsVal = NULL;
BYTE *trafficBuf = NULL, *respBuf = NULL;
struct timeval *toutvalp = NULL;
#endif

#define POLLING_TIME      			200
#define DONGLE_TX_FRAME_SIZE   		1024
#define MESSAGE_LENGTH				1024
#define MAX_SHELL_FILE_LENGTH       50

int remote_type = NO_REMOTE;
static int g_rwl_hndle;
static rem_ioctl_t *g_rem_ptr;

#if defined(SYMBIAN)
extern int wl_ioctl(void *wl, int cmd, void *buf, int len, bool set);
#endif

/* Function prototypes from shellpoc_linux.c/shell_ce.c */
extern int rwl_create_dir(void);
extern int remote_shell_execute(char *buf_ptr);
extern int remote_shell_get_resp(char* shell_fname, char *buf_ptr, int msg_len);
extern void rwl_wifi_find_server_response(void *wl, dot11_action_wifi_vendor_specific_t *rec_frame);
extern dot11_action_wifi_vendor_specific_t *rwl_wifi_allocate_actionframe();

/* Common code for serial and wifi */
#if defined(RWL_DONGLE) || defined(RWL_WIFI) || defined(RWL_SERIAL)
typedef struct rem_packet {
	rem_ioctl_t rem_cdc;
	uchar message[MESSAGE_LENGTH];
} rem_packet_t;
#define REMOTE_PACKET_SIZE sizeof(rem_packet_t)

rem_packet_t *g_rem_pkt_ptr;
rem_packet_t g_rem_pkt;
#endif

static union {
	uchar bufdata[WLC_IOCTL_MEDLEN];
	uint32 alignme;
} bufstruct_wlu;
static uchar* rwl_buf = (uchar*) &bufstruct_wlu.bufdata;

#ifdef RWL_DONGLE
static rem_ioctl_t loc_cdc;
static const char* cmdname = "remote";
#endif


/* Function: rwl_transport_setup
 * This will do the initialization for
 * for all the transports
 */
static int
rwl_transport_setup(int argc, char** argv)
{
	int transport_descriptor = -1;

	UNUSED_PARAMETER(argc);
	UNUSED_PARAMETER(argv);

#ifdef RWL_SOCKET
	/* This function will parse the socket command line arguments
	 * & open the socket in listen mode
	 */
	remote_type = REMOTE_SOCKET;
	transport_descriptor = rwl_init_server_socket_setup(argc, argv, remote_type);

	if (transport_descriptor < 0) {
		DPRINT_ERR(ERR, "wl_socket_server:Transport setup failed \n");
	}
#endif /* RWL_SOCKET */
#if defined(RWL_DONGLE) || defined(RWL_WIFI) || defined(RWL_SERIAL)
	g_rem_pkt_ptr = &g_rem_pkt;
	transport_descriptor = 0;

#ifdef RWL_WIFI
	remote_type = REMOTE_WIFI;
#endif

#ifdef RWL_DONGLE
	remote_type = REMOTE_DONGLE;
#endif /* RWL_DONGLE */
#ifdef RWL_SERIAL
	remote_type = REMOTE_SERIAL;
	if (argc < 2) {
		DPRINT_ERR(ERR, "Port name is required from the command line\n");
	} else {
		*argv++;
		DPRINT_DBG(OUTPUT, "Port name is %s\n", *argv);
		transport_descriptor = *(int*) rwl_open_transport(remote_type, *argv, 0, 0);
	}
#endif  /* RWL_SERIAL */
#endif /* RWL_DONGLE ||RWL_SERIAL ||RWL_WIFI */

	return transport_descriptor;

}

/* Function: remote_rx_header
 * This function will receive the CDC header from client
 * for socket transport
 * It will receive the command or ioctl from dongle driver for
 * dongle UART serial transport and wifi transport.
 * Arguments: wl - handle to driver
 *          : Des - Socket Descriptor to pass in AcceptConnection
 *          : g_rwl_hndle - Return socket handle that is used for transmission
 *          :           and reception of data in case of socket
 *                        In case of serial, it is just a return value
 */
static int
remote_rx_header(void *wl, int trans_Des)
{
#ifdef RWL_SOCKET
	struct sockaddr_in ClientAddress;
	int SizeOfCliAdd;
	SizeOfCliAdd = sizeof(ClientAddress);

	UNUSED_PARAMETER(wl);

	/* Get the socket handle g_rwl_hndle for transmission & reception */
	if ((g_rwl_hndle = rwl_acceptconnection(trans_Des, (struct sockaddr*)&ClientAddress,
	                                                 &SizeOfCliAdd)) == FAIL) {
		return FAIL;
	}

	/* Get CDC header in order to determine buffer requirements */
	if ((g_rem_ptr = remote_CDC_rx_hdr((void *)&g_rwl_hndle, 0)) == NULL) {
		DPRINT_DBG(OUTPUT, "\n Waiting for client to transmit command\n");
		return FAIL;
	}

#endif /* RWL_SOCKET */

#ifdef RWL_DONGLE
	void *pkt_ptr = NULL;
	int error;

	UNUSED_PARAMETER(trans_Des);

	/* wl driver is polled after every 200 ms (POLLING_TIME) */
	rwl_sleep(POLLING_TIME);

	if ((error = rwl_var_getbuf(wl, cmdname, NULL, 0, &pkt_ptr)) < 0) {
		DPRINT_ERR(ERR, "No packet in wl driver\r\n");
		return FAIL;
	}

	DPRINT_DBG(OUTPUT, "Polling the wl driver, error status=%d\n", error);

	if ((*(int *)pkt_ptr) == 0) {
		DPRINT_DBG(ERR, "packet not received\n");
		return FAIL;
	}

	DPRINT_DBG(OUTPUT, "packet received\n");

	/* Extract CDC header in order to determine buffer requirements */
	memcpy((char*)g_rem_pkt_ptr, (char*)pkt_ptr, sizeof(rem_packet_t));
	g_rem_ptr = &(g_rem_pkt_ptr->rem_cdc);

#endif /* RWL_DONGLE */

#ifdef RWL_SERIAL
	UNUSED_PARAMETER(wl);
	UNUSED_PARAMETER(trans_Des);
	if (g_rwl_hndle == -1) {
		DPRINT_ERR(ERR, "failed to open com port.\r\n");
		return FAIL;
	}

	if ((g_rem_ptr = remote_CDC_rx_hdr((void*)&g_rwl_hndle, 1)) == NULL) {
		DPRINT_DBG(OUTPUT, "\n Waiting for client to transmit command\n");
		return FAIL;
	}
#endif  /* RWL_SERIAL */


#ifdef RWL_WIFI
	/* Poll the driver for the valid action frame and update the CDC + data  */
	dot11_action_wifi_vendor_specific_t *list;

	UNUSED_PARAMETER(trans_Des);

	if ((list = rwl_wifi_allocate_actionframe()) == NULL) {
		DPRINT_DBG(OUTPUT, "remote_rx_header: Failed to allocate frame \n");
		return FAIL;
	}

	if (remote_CDC_DATA_wifi_rx((void*)wl, list) < 0) {
		free(list);
		return FAIL;
	}

	/* copy the valid length of the data to the g_rem_pkt_ptr */
	memcpy((char*)g_rem_pkt_ptr, (char*)&list->data[0], sizeof(rem_packet_t));
	g_rem_ptr = &(g_rem_pkt_ptr->rem_cdc);
	free(list);
#endif /* RWL_WIFI */

	DPRINT_INFO(OUTPUT, "%d %d %d %d\r\n", g_rem_ptr->msg.cmd,
	g_rem_ptr->msg.len, g_rem_ptr->msg.flags, g_rem_ptr->data_len);

	return SUCCESS;

}

/* Function: remote_rx_data
 * This function will receive the data from client
 * for different transports
 * In case of socket the data comes from a open TCP socket
 * However in case of dongle UART or wi-fi the data is accessed
 * from the driver buffers.
 */
int
remote_rx_data(void* buf_ptr)
{
#if defined(RWL_SOCKET) || defined(RWL_SERIAL)

	if ((remote_CDC_rx((void *)&g_rwl_hndle, g_rem_ptr, buf_ptr,
	                             g_rem_ptr->msg.len, 0)) == FAIL) {
		DPRINT_ERR(ERR, "Reading CDC %d data bytes failed\n", g_rem_ptr->msg.len);
		return FAIL;
	}
#elif defined(RWL_DONGLE) || defined(RWL_WIFI)
	if (g_rem_ptr->data_len != 0) {
		int length = g_rem_ptr->data_len;
		if (g_rem_ptr->data_len > g_rem_ptr->msg.len) {
			length = g_rem_ptr->msg.len;
		}
		memcpy(buf_ptr, g_rem_pkt_ptr->message, length);
	}
	else
		buf_ptr = NULL;
#endif /* RWL_SOCKET || RWL_SERIAL */
	return SUCCESS;
}

#ifdef RWL_DONGLE
/*
 * Function to send the serial response to wl driver
 * The function calculates the no of frames based on the DATA_FRAME_LEN
 * adds cdc header to every frame, copies the header and fragmented frame
 * into rem_buf_ptr and sends the packet down to wl driver
 */

static int
rwl_serial_fragmented_tx(void* wl, rem_ioctl_t *rem_ptr, uchar *buf_ptr, int error)
{
	rem_ioctl_t *loc_ptr = &loc_cdc;
	uchar* rem_buf_ptr;
	uint noframes = 1;    /* Default noframes = 1 */
	uint count;
	uint frame_count;
	uint rem_bytes;

	loc_ptr->msg.cmd = error;
	loc_ptr->msg.flags = REMOTE_REPLY;
	loc_ptr->msg.len = rem_ptr->msg.len;
	loc_ptr->data_len = rem_ptr->data_len;

	/* Fragment the result if it is more than DATA_FRAME_LEN (960) */
	if (loc_ptr->msg.len > DATA_FRAME_LEN) {
		/* Calculate no of frames */
		noframes = (loc_ptr->msg.len)/DATA_FRAME_LEN;
		if ((loc_ptr->msg.len) % DATA_FRAME_LEN > 0) {
			noframes += 1;
			rem_bytes = (loc_ptr->msg.len) % DATA_FRAME_LEN;
		} else {
			rem_bytes = DATA_FRAME_LEN;
		}
	} else {
		rem_bytes = loc_ptr->msg.len;
	}
	DPRINT_INFO(OUTPUT, "No of frames = %d, rem_bytes:%d\n", noframes, rem_bytes);
	count = 0;
	frame_count = noframes;
	rem_buf_ptr = (uchar*)malloc(DONGLE_TX_FRAME_SIZE + REMOTE_SIZE);

	while (count < noframes) {
		memset(rem_buf_ptr, 0, DONGLE_TX_FRAME_SIZE + REMOTE_SIZE);
		/* Send reply to client */
		rem_ptr->msg.cmd = loc_ptr->msg.cmd;
		rem_ptr->msg.flags = loc_ptr->msg.flags;
		rem_ptr->msg.len = loc_ptr->msg.len;

		if (frame_count == 1)
			rem_ptr->data_len = rem_bytes;
		else
			rem_ptr->data_len = DATA_FRAME_LEN;

		DPRINT_DBG(OUTPUT, "GET--rem_ptr->data_len=%d\n", rem_ptr->data_len);

		/* Copy CDC Header */
		memcpy(rem_buf_ptr, (uchar*)rem_ptr, REMOTE_SIZE);

		/* Copy Data now */
		memcpy(&rem_buf_ptr[REMOTE_SIZE], &buf_ptr[count*DATA_FRAME_LEN],
		                                              rem_ptr->data_len);
		count++;
		frame_count--;

		DPRINT_INFO(OUTPUT, "FRAME %d\n", count);
		DPRINT_INFO(OUTPUT, "%d %d  %d  %d\n", rem_ptr->msg.cmd, rem_ptr->msg.len,
			rem_ptr->msg.flags, rem_ptr->data_len);

		rwl_sync_delay(noframes);

		if ((error = rwl_var_setbuf(wl, cmdname, rem_buf_ptr,
		            DONGLE_TX_FRAME_SIZE+REMOTE_SIZE)) < 0) {
			DPRINT_INFO(OUTPUT, "Unable to send to wl driver,error=%d\n", error);
			if (rem_buf_ptr)
				free(rem_buf_ptr);
			return FAIL;
		}
		else
			DPRINT_INFO(OUTPUT, "Packet sent to wl driver,error=%d\n", error);
	}

	if (rem_buf_ptr)
		free(rem_buf_ptr);

	return error;
}

/* This function transmits the response to the dongle driver in the case 
 * of serial dongle transport.
 * In the case of big response, it calls the rwl_serial_fragmented_response
 * function to fragment the response and sends down to the driver.
 */

static int
remote_CDC_dongle_tx(void *wl, uint cmd, uchar *buf, uint buf_len, uint data_len, uint flags)
{
	int error;
	rem_ioctl_t resp_rem_cdc;

	if (flags & REMOTE_SET_IOCTL) {
		uchar* rem_buf_ptr;
		/* for set commands message length and data length should be set to zero 
		 * unlike Get Ioctl which will have valid data and message length
		 */
		resp_rem_cdc.msg.len = 0;
		resp_rem_cdc.data_len = 0;
		resp_rem_cdc.msg.cmd = cmd;
		resp_rem_cdc.msg.flags = REMOTE_REPLY;

		DPRINT_INFO(OUTPUT, "Set:Resp packet:%d %d %d %d\n", resp_rem_cdc.msg.cmd,
		resp_rem_cdc.msg.len, resp_rem_cdc.msg.flags, resp_rem_cdc.data_len);

		if ((rem_buf_ptr = (uchar*)malloc(DONGLE_TX_FRAME_SIZE + REMOTE_SIZE)) == NULL) {
			DPRINT_ERR(ERR, "malloc failed for remote_CDC_dongle_tx\n");
			return FAIL;
		}

		/* Send reply to client here */
		memcpy(rem_buf_ptr, (char*)(&resp_rem_cdc), REMOTE_SIZE);

		if ((error = rwl_var_setbuf((void*)wl, cmdname, rem_buf_ptr,
		                 DONGLE_TX_FRAME_SIZE + REMOTE_SIZE)) < 0) {
			DPRINT_ERR(ERR, "unable to send SET results to driver=%d\n", error);
		} else
			DPRINT_INFO(OUTPUT, "Packet sent to wl driver, error=%d\n", error);

		if (rem_buf_ptr)
			free(rem_buf_ptr);

	} else { /* GET_IOCTL */
		resp_rem_cdc.msg.cmd = cmd;
		resp_rem_cdc.msg.len = buf_len;
		resp_rem_cdc.msg.flags = flags;
		resp_rem_cdc.data_len = data_len;
		if ((error = rwl_serial_fragmented_tx(wl, &resp_rem_cdc, buf, cmd)) < 0)
			DPRINT_ERR(ERR, "wl_server_serial: Return error code failed\n");
	}
	return error;
}
#endif /* RWL_DONGLE */

/* This function gets the command send by the client from the dongle driver */
int
rwl_var_getbuf(void* wl, const char* iovar, void* param, int param_len, void** buf_ptr)
{
	int len;

	memset(rwl_buf, 0, WLC_IOCTL_MEDLEN);
	strcpy((char*)rwl_buf, iovar);
	/* include the null */
	len = strlen(iovar) + 1;

	if (param_len)
		memcpy(&rwl_buf[len], param, param_len);

	*buf_ptr = rwl_buf;

	return wl_get(wl, WLC_GET_VAR, &rwl_buf[0], WLC_IOCTL_MEDLEN);
}

/* This function will send the buffer to the dongle driver */
int
rwl_var_setbuf(void* wl, const char* iovar, void* param, int param_len)
{
	int len;

	memset(rwl_buf, 0, WLC_IOCTL_MEDLEN);
	strcpy((char*)rwl_buf, iovar);

	/* include the null */
	len = strlen(iovar) + 1;

	if (param_len)
		memcpy(&rwl_buf[len], param, param_len);

	len  += param_len;

	DPRINT_DBG(OUTPUT, "setbuf:%s, len:%d\n", rwl_buf, len);

	return wl_set(wl, WLC_SET_VAR, &rwl_buf[0], len);
}

/* 
 * This function is used for transmitting the response over different
 * transports.
 * In case of socket the data is directly sent to the client which is waiting on a open socket.
 * In case of socket the data is sent in one big chunk unlike other transports
 *
 * In case of serial the data is sent to the driver using remote_CDC_dongle_tx function
 * which in turn may fragment the data and send it in chunks to the client.
 *
 * In case of wi-fi the data is sent to the driver using the remote_CDC_tx. However
 * in this case the data is converted into 802.11 Action frames and sent using wi-fi driver.
 * Arguments: wl - Driver handle
 *            hndle - Socket handle for socket transport.
 */
static int
remote_tx_response(void *wl, void* buf_ptr, int cmd)
{
	int error = -1;

#if defined(RWL_SOCKET) || defined(RWL_SERIAL)
	UNUSED_PARAMETER(wl);
	if ((error = remote_CDC_tx((void*)&g_rwl_hndle, cmd, buf_ptr, g_rem_ptr->msg.len,
	                                  g_rem_ptr->msg.len, REMOTE_REPLY, 0)) < 0)
		DPRINT_ERR(ERR, "wl_server: Return results failed\n");
#ifdef RWL_SOCKET
	/* Transmitted to client. Then close the handle & get the new handle
	 * for next transmission & reception.
	 */
	 rwl_close_pipe(remote_type, (void*)&g_rwl_hndle);
#endif /* RWL_SOCKET */
#endif /* RWL_SOCKET || RWL_SERIAL */

#ifdef RWL_DONGLE
	if ((error = remote_CDC_dongle_tx(wl, cmd, buf_ptr, g_rem_ptr->msg.len,
	                          g_rem_ptr->data_len, g_rem_ptr->msg.flags)) < 0)
		DPRINT_ERR(ERR, "wl_server: Return results failed\n");
#endif /* RWL_DONGLE */

#ifdef RWL_WIFI
	/* Purge all the queued cmd's , this to ensure late response time out at */
	/* client side and client might issue the next cmd if server is slow */
	rwl_wifi_purge_actionframes(wl);
	if ((g_rem_ptr->msg.flags & REMOTE_SHELL_CMD) ||
		(g_rem_ptr->msg.flags & REMOTE_GET_IOCTL)||
		(g_rem_ptr->msg.flags & REMOTE_ASD_CMD)) {
		if ((error = remote_CDC_tx(wl, cmd, buf_ptr, g_rem_ptr->msg.len,
			g_rem_ptr->msg.len, REMOTE_REPLY, 0)) < 0)
			DPRINT_ERR(ERR, "wl_wifi_server_ce: Return results failed\n");
	} else {
		if ((error = remote_CDC_tx(wl, cmd, buf_ptr, 0, 0, REMOTE_REPLY, 0)) < 0)
			DPRINT_ERR(ERR, "wl_wifi_server_ce: Return results failed\n");
	}
#endif /* RWL_WIFI */
	return error;
}

/* Close the Socket handle */
void close_sock_handle(int hndle)
{
#ifdef RWL_SOCKET
	rwl_close_pipe(remote_type, (void*)&hndle);
#else
	UNUSED_PARAMETER(hndle);
#endif
}

#ifdef RWL_WIFI
/* 
 * This function runs a set of commands before running the wi-fi server
 * This is avoids packet drops and improves performance.
 * We run the following wl commands
 * up, mpc 0, wsec 0, slow_timer 999999, fast_timer 999999, glacial_timer 999999
 * legacylink 1, monitor 1.
 */
void remote_wifi_ser_init_cmds(void *wl)
{
	int count, err;
	char bigbuf[RWL_WIFI_BUF_LEN];
	/* The array stores command, length and then data format */
	remote_wifi_cmds_t wifi_cmds[RWL_WIFI_SERV_COUNT] = {
						{WLC_UP, 0x0, NULL, 0x0},
						{WLC_SET_VAR, 0x8, "mpc", 0},
						{WLC_SET_WSEC, 0x4, NULL, 0x0},
						{WLC_SET_VAR, 0xf, "slow_timer", 999999},
						{WLC_SET_VAR, 0xf, "fast_timer", 999999},
						{WLC_SET_VAR, 0x12, "glacial_timer", 999999},
						{WLC_LEGACY_LINK_BEHAVIOR, 0x04, NULL, 0x1},
						{WLC_SET_MONITOR, 0x4, NULL, 0x1}
	};

	for (count = 0; count < RWL_WIFI_SERV_COUNT; count++) {
		if (wifi_cmds[count].data != NULL) {
			strcpy(bigbuf, wifi_cmds[count].data);
			memcpy(&bigbuf[strlen(wifi_cmds[count].data)+1],
			(char*)&wifi_cmds[count].value, sizeof(int));
		} else {
			memcpy(&bigbuf[0], (char*)&wifi_cmds[count].value, sizeof(int));
		}
#ifdef WIN32
		/* Add OID base for NDIS commands */

		err = (int)ir_setinformation(wl, wifi_cmds[count].cmd + WL_OID_BASE,
		bigbuf, &wifi_cmds[count].len);
#endif

#if defined(SYMBIAN)
		if (wifi_cmds[count].len == 0)
			/* NULL needs to be passed to the driver if len is 0.
			 * Otherwise driver hangs
			 */
			err = wl_ioctl(wl, wifi_cmds[count].cmd,
				NULL, wifi_cmds[count].len, TRUE);
		else
			err = wl_ioctl(wl, wifi_cmds[count].cmd,
			(void*)&bigbuf, wifi_cmds[count].len, TRUE);
#endif
		rwl_sleep(1000);
	}
}
#endif /* RWL_WIFI */

/*
 * Send the response to the remote if the channel of the server matches with the
 * server channel.
 */
void remote_wifi_response(void* wl)
{
#ifdef RWL_WIFI
	dot11_action_wifi_vendor_specific_t *list;

	if ((list = rwl_wifi_allocate_actionframe()) == NULL) {
		DPRINT_DBG(OUTPUT, "remote_wifi_response: Failed to allocate frame \n");
		return;
	}

	/* it's sync frame and received from client */
	memcpy((char*)&list->data[RWL_WIFI_CDC_HEADER_OFFSET],
	&g_rem_pkt_ptr[RWL_WIFI_CDC_HEADER_OFFSET], sizeof(rem_packet_t));
	memcpy((char*)&list->data[REMOTE_SIZE], g_rem_pkt_ptr->message,
	RWL_WIFI_FRAG_DATA_SIZE);
	list->type = RWL_WIFI_FIND_MY_PEER;
	/* send the response to client if server is on the same channel */
	rwl_wifi_find_server_response(wl, list);

	free(list);
#else
	UNUSED_PARAMETER(wl);
#endif /* RWL_WIFI */
	return;
}

/* Main server module common for all transports
 * This module will do the initial transport setups.
 * Then it receives the command from client in CDC format
 * and transmits the response back to the client.
 * In the case of socket, it receives the command from the client
 * and sends the response directly to the client via TCP socket.
 *
 * In the case of serial & wifi , it receives the command from the driver
 * and sends the response to the driver.
 */
int
remote_server_exec(int argc, char **argv, void *wl)
{
	int err;
	int transport_descriptor;
	char shell_fname[MAX_SHELL_FILE_LENGTH];
	int shell_resp_length;

#ifdef WIN32
	DWORD dwlen;
#endif
	/* Initialise for all the transports - socket, serial, and wifi
	 * In Socket transport, main socket handler will be returned.
	 */
	if ((transport_descriptor = rwl_transport_setup(argc, argv)) < 0)
		return FAIL;

#ifdef RWL_WIFI
	remote_wifi_ser_init_cmds(wl);
#endif
	/* Create a directory /tmp/RWL for the shell response files */
	if (rwl_create_dir() < 0)
		return FAIL;


#ifdef RWLASD
	/* DUT initialization function */
	wfa_dut_init(&trafficBuf, &respBuf, &parmsVal, &xcCmdBuf, &toutvalp);
#endif

	while (1) {
		uchar *buf_ptr;
#if defined(RWL_SERIAL)
		g_rwl_hndle = transport_descriptor;
#else
		g_rwl_hndle = -1;
#endif

		/* Receive the CDC header */
		if ((remote_rx_header(wl, transport_descriptor)) == FAIL) {
			DPRINT_DBG(OUTPUT, "\n Waiting for client to transmit command\n");
			continue;
		}

		DPRINT_INFO(OUTPUT, "REC : cmd %d\t msg len %d  msg flag %d\t msg status %d\n",
		g_rem_ptr->msg.cmd, g_rem_ptr->msg.len,
		g_rem_ptr->msg.flags, g_rem_ptr->msg.status);

#ifdef RWL_WIFI
		/* send the response to remote if it is findserver cmd, this is specific to wifi */
		if (g_rem_ptr->msg.flags & REMOTE_FINDSERVER_IOCTL) {
			remote_wifi_response(wl);
			continue;
		}
#endif /* RWL_WIFI */

		if ((buf_ptr = malloc(g_rem_ptr->msg.len)) == NULL) {
			DPRINT_ERR(ERR, "malloc of %d bytes failed\n", g_rem_ptr->msg.len);
			continue;
		}

		/* Receive the data */
		if ((err = remote_rx_data(buf_ptr)) == FAIL) {
			if (buf_ptr)
				free(buf_ptr);
			continue;
		}

		/* Process command */
		if (g_rem_ptr->msg.flags & REMOTE_SHELL_CMD) {
		/* Get the response length first and get the response buffer in case of
		 * synchronous shell commands and the buf_ptr will have the response file name.
		 * In case of asynchronous shell commands, buf_ptr
		 * will be get updated by the remote_shell_execute function.
		 */
			if ((shell_resp_length = remote_shell_execute((char*)buf_ptr)) > 0) {
				strncpy(shell_fname, (char*)buf_ptr, MAX_SHELL_FILE_LENGTH - 1);
				shell_fname[MAX_SHELL_FILE_LENGTH - 1] = 0;
				free(buf_ptr);
				g_rem_ptr->msg.len = shell_resp_length;
				if ((buf_ptr = malloc(g_rem_ptr->msg.len)) == NULL) {
					DPRINT_ERR(ERR, "malloc of %d bytes failed\n",
					g_rem_ptr->msg.len);
					continue;
				}
				/* Get the response from the temporary file */
				err = remote_shell_get_resp(shell_fname, (char*)buf_ptr,
				                               g_rem_ptr->msg.len);
			}
		} /* REMOTE_SHELL_CMD */

#ifdef RWLASD
		if (g_rem_ptr->msg.flags & REMOTE_ASD_CMD) {
			if ((err = remote_asd_exec(buf_ptr, (int *)&g_rem_ptr->msg.len)) < 0) {
				DPRINT_ERR(ERR, "Error in executing asd command\n");
			}
		} /* REMOTE_ASD_CMD */
#endif

		if (g_rem_ptr->msg.flags & REMOTE_SET_IOCTL) {
#ifdef WIN32
#if defined(RWL_DONGLE) || defined(RWL_WIFI)
			/* For commands with msg length as zero initialize the buffer to null */
			if (g_rem_ptr->msg.len == 0)
				buf_ptr = NULL;
#endif
#else
			if (g_rem_ptr->msg.len == 0)
				buf_ptr = NULL;
#endif /* WIN32 */

#if defined(SYMBIAN)
			err = wl_ioctl(wl, g_rem_ptr->msg.cmd,
			       (void *)buf_ptr, g_rem_ptr->msg.len, TRUE);
#else
			dwlen = g_rem_ptr->msg.len;

			err = (int)ir_setinformation(wl, g_rem_ptr->msg.cmd,
			                       buf_ptr, &dwlen);
#endif

		} /* REMOTE_SET_IOCTL */

		if (g_rem_ptr->msg.flags & REMOTE_GET_IOCTL) {
#if defined(SYMBIAN)
			if (g_rem_ptr->msg.cmd == WLC_GET_VAR && buf_ptr &&
				strncmp((const char *)buf_ptr, "exit", g_rem_ptr->msg.len) == 0) {
					/* exit command from remote client terminates server */
						free(buf_ptr);
					break;
			}

			err = wl_ioctl(wl, g_rem_ptr->msg.cmd, (void *)buf_ptr,
			                             g_rem_ptr->msg.len, FALSE);
#else
			if (g_rem_ptr->msg.cmd == (WL_OID_BASE + WLC_GET_VAR) &&
				strncmp(buf_ptr, "exit", g_rem_ptr->msg.len) == 0) {
					/* exit command from remote client terminates server */
					if (buf_ptr) {
						free(buf_ptr);
					}
					break;
			}

			dwlen = g_rem_ptr->msg.len;

			err = (int)ir_queryinformation(wl,
			g_rem_ptr->msg.cmd, buf_ptr, &dwlen);

#endif 
		} /* REMOTE_GET_IOCTL */
		DPRINT_INFO(OUTPUT, "RESP : cmd %d\t msg len %d\n",
		g_rem_ptr->msg.cmd, g_rem_ptr->msg.len);

		/* Transmit the response results */
		if (remote_tx_response(wl, buf_ptr, err) < 0) {
			DPRINT_ERR(ERR, "\nReturn results failed\n");
		}

		if (buf_ptr) {
			free(buf_ptr);
		}
	} /* end of while */
#if defined(RWL_SOCKET)
	/* Close the main handle for socket */
	close_sock_handle(transport_descriptor);
#elif defined(RWL_SERIAL)
	/* Close the main handle for serial pipe  */
	rwl_close_pipe(remote_type, (void*)&transport_descriptor);
#endif

#ifdef RWLASD
	wfa_dut_deinit();
#endif

	return err;
}
