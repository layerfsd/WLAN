//------------------------------------------------------------------------------
// <copyright file="htc.h" company="Atheros">
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
// HTC Module specific declarations and prototypes
//
// Author(s): ="Atheros"
//==============================================================================


#ifndef _HTC_H_
#define _HTC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ------ MBOX ID ------ */
typedef enum
{
    ENDPOINT_UNUSED = -1,
    ENDPOINT1 = 0,
    ENDPOINT2,
    ENDPOINT3,
    ENDPOINT4,
} HTC_ENDPOINT_ID;

/* ------ Event Types ------ */
typedef enum
{
    HTC_BUFFER_RECEIVED = 0,
    HTC_SKB_RECEIVED,
    HTC_BUFFER_SENT,
    HTC_SKB_SENT,
    HTC_DATA_AVAILABLE,
    HTC_TARGET_AVAILABLE,
    HTC_TARGET_UNAVAILABLE,
} HTC_EVENT_ID;

#define HTC_MAILBOX_NUM_MAX                4
#define HTC_HEADER_LEN                     2
#define HTC_DATA_REQUEST_RING_BUFFER_SIZE  30 

/* ------- Target Specific Data structures ------- */
typedef struct htc_target HTC_TARGET;

/* ------- Event Specific Data Structures ------- */
typedef struct htc_event_info HTC_EVENT_INFO;
typedef void   (*HTC_EVENT_HANDLER) (HTC_TARGET *, 
                                     HTC_ENDPOINT_ID, 
                                     HTC_EVENT_ID, 
                                     HTC_EVENT_INFO *, 
                                     void *);

/* WMI layer extracts the relevant information from this data structure */
struct htc_event_info
{
    A_UCHAR             *buffer;
    void                *cookie;
    A_UINT32             bufferLength;
    A_UINT32             actualLength;
    A_STATUS             status;
};

/* ------ Function Prototypes ------ */
A_STATUS
HTCInit(void);

A_STATUS
HTCStart(HTC_TARGET *target);
/* target - IN */

A_STATUS 
HTCEventReg(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId, 
            HTC_EVENT_ID eventId, HTC_EVENT_HANDLER eventHandler, 
            void *param);
/* target - IN, endPointId - IN, eventId - IN, eventHandler - IN, param - IN */

A_STATUS 
HTCBufferReceive(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId, 
                 A_UCHAR *buffer, A_UINT32 length, void *cookie);
/* target - IN, endPointId - IN, buffer - IN, length - IN, cookie - IN */

#if 0
A_STATUS 
HTCSkbReceive(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId, 
              struct sk_buff *skb, void *cookie);
#endif

A_STATUS 
HTCBufferSend(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId, 
              A_UCHAR *buffer, A_UINT32 length, void *cookie);
/* target - IN, endPointId - IN, buffer - IN, length - IN, cookie - IN */

#if 0
A_STATUS 
HTCSkbSend(HTC_TARGET *target, HTC_ENDPOINT_ID endPointId, 
           struct sk_buff *skb, void *cookie);
#endif

void
HTCStop(HTC_TARGET *target);
/* target - IN */

void
HTCShutDown(HTC_TARGET *target);

#ifdef __cplusplus
}
#endif

#endif /* _HTC_H_ */
