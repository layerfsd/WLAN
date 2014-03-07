

/**
	
*/
#ifndef __HTC_WINCE_H__
#define __HTC_WINCE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


//bluebird  ar6001 wince6.0 src
typedef struct htc_event_info HTC_EVENT_INFO;

/* WMI layer extracts the relevant information from this data structure */
struct htc_event_info
{
    A_UCHAR             *buffer;
    void                *cookie;
    A_UINT32             bufferLength;
    A_UINT32             actualLength;
    A_STATUS             status;
};


#if 0
/* ------ MBOX ID ------ */
typedef enum
{
    ENDPOINT_UNUSED = -1,
    ENDPOINT1 = 0,
    ENDPOINT2,
    ENDPOINT3,
    ENDPOINT4,
} HTC_ENDPOINT_ID;
#endif


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


#define WMI_CONTROL_MBOX        ENDPOINT1


#ifdef __cplusplus
}
#endif
#endif


