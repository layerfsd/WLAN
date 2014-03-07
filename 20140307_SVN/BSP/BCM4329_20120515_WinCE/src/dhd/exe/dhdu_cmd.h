/*
 * Command structure for dhd command line utility, copied from wl utility
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: dhdu_cmd.h,v 1.3.102.2 2008/05/07 00:19:34 Exp $
 */

#ifndef _dhdu_cmd_h_
#define _dhdu_cmd_h_

typedef struct cmd cmd_t;
typedef int (cmd_func_t)(void *dhd, cmd_t *cmd, char **argv);

/* generic command line argument handler */
struct cmd {
	char *name;
	cmd_func_t *func;
	int get;
	int set;
	char *help;
};

/* list of command line arguments */
extern cmd_t dhd_cmds[];
extern cmd_t dhd_varcmd;

/* per-port ioctl handlers */
extern int dhd_get(void *dhd, int cmd, void *buf, int len);
extern int dhd_set(void *dhd, int cmd, void *buf, int len);

#endif /* _dhdu_cmd_h_ */
