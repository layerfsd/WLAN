/*
 * MsgQueueUtil.h
 * Windows CE message queue utilities
 *
 *
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: MsgQueueUtil.h,v 13.4.2.1 2009/09/09 01:25:22 Exp $
 *
 */

#ifndef __MsgQueueUtil_H
#define __MsgQueueUtil_H

#include <typedefs.h>
#include <windows.h>
#include <MsgQueue.h>
#define BCMSDD_PRI_QUEUE	"BCMSDD_PRI_QUEUE"
#define BCMSDD_VIF_QUEUE		"BCMSDD_VIF_QUEUE"
HANDLE MsgQueueCreate(char *stringIn, bool rw);
void MsgQueueDelete(HANDLE qhandle);

BOOL MsgQueueSend(HANDLE hMsgQueue, LPVOID pMsgData, DWORD dwMsgLen);
BOOL MsgQueueRead(HANDLE hMsgQueue, LPVOID pMsgData, LPDWORD pdwBytesRead);
BOOL MsgQueueFlush(HANDLE qhandle);
BOOL MsgQueueGetInfo(HANDLE hMsgQueue, LPMSGQUEUEINFO pMsgQueueInfo);

#endif	/* __MsgQueueUtil_H */
