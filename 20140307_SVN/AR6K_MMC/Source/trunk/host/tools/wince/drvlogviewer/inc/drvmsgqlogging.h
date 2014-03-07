/*
 * Definitions for logging via a Windows CE message queue
 * 
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 * 
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
 * 
 */

#ifndef _ATH_MSGQ_LOG_H_
#define _ATH_MSGQ_LOG_H_

#define ATHMSGQ_LOG_MAX_CHARS_PER_MSG  (384)
#define ATHMSGQ_LOG_MAX_MESSAGE_SIZE   ((sizeof(TCHAR)) * ATHMSGQ_LOG_MAX_CHARS_PER_MSG)
#define ATHMSGQ_LOG_MAX_MESSAGES       32

    /* this form of logging uses Windows CE's point-to-point message queue support
     * The logging involves a single reader and single writer
     * Both readers and writers must use an agreed-upon name for the message
     * queue. Reserved message queue names are provided below. */
#define ATHMSGQ_INIT_QUEUE_OPTIONS(options,readAccess)                 \
{                                                                      \
    memset(&(options),0,sizeof(MSGQUEUEOPTIONS));                      \
    (options).dwSize                 = sizeof(MSGQUEUEOPTIONS);        \
    (options).dwFlags                = MSGQUEUE_ALLOW_BROKEN;          \
    (options).dwMaxMessages          = ATHMSGQ_LOG_MAX_MESSAGES;       \
    (options).cbMaxMessage           = ATHMSGQ_LOG_MAX_MESSAGE_SIZE;   \
    (options).bReadAccess            = (readAccess);                   \
}   

    /* name of message queue used by btfilter */
#define ATHBTFILTER_LOG_MSG_QUEUE_NAME TEXT("athbtfilter_logmsg")

#endif /* _ATH_MSGQ_LOG_H_ */
