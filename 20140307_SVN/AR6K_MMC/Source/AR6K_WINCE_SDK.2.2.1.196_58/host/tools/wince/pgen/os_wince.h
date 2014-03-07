//------------------------------------------------------------------------------
// <copyright file="os_wince.h" company="Atheros">
//    Copyright (c) 2007 Atheros Corporation.  All rights reserved.
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
#ifndef __OS_WINCE_H__
#define __OS_WINCE_H__

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#define	MSG_DONTWAIT 		0
#define	ITIMER_REAL 		0

#define	SIGALRM				0
typedef void sigfn();
typedef sigfn *sighandler;
#define SIG_IGN				((sighandler) 1)

#define	SOL_IP		IPPROTO_IP

typedef char * caddr_t;

struct itimerval {
	struct timeval it_interval;
	struct timeval it_value;
};

struct timerhandle {
	int enable;
	int id;
	sighandler fn;
};

void gettimeofday(struct timeval *t, int dummmy);
void usleep(int usec);
void sleep(int sec);
void bzero(char *ptr, int size);
int	 setitimer(int dummy, struct itimerval *t, struct itimerval *t1);
void perror(char *msg);
int  signal(int sig, sighandler);
int  system(TCHAR *cmd);

#endif
