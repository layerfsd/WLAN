/**

*/

#ifndef __AR6K_DRV_H__
#define __AR6K_DRV_H__

#include <a_config.h>
#include <athdefs.h>
#include "a_types.h"
#include "a_osapi.h"
#include "htc_api.h"
#include "wmi.h"
#include "a_drv.h"
#include "bmi.h"

#include "htc_packet.h"


#include <ieee80211.h>
//#include <ieee80211_ioctl.h>

#include "targaddrs.h"
#include "common_drv.h"
#include "dbglog_api.h"




#ifdef __cplusplus
extern "C" {
#endif

#define MAX_AR6000                        1
#define AR6000_TX_TIMEOUT                 10
#define AR6000_ETH_ADDR_LEN               6
#define AR6000_MAX_ENDPOINTS              4
#define MAX_NODE_NUM                      15
#define MAX_COOKIE_NUM                    60
#define AR6000_HB_CHALLENGE_RESP_FREQ_DEFAULT        1
#define AR6000_HB_CHALLENGE_RESP_MISS_THRES_DEFAULT  1
#define A_DISCONNECT_TIMER_INTERVAL       5 * 1000
#define AR6000_BUFFER_SIZE					1664

enum {
    DRV_HB_CHALLENGE = 0,
    APP_HB_CHALLENGE
};


/* HTC RAW streams */
typedef enum _HTC_RAW_STREAM_ID {
    HTC_RAW_STREAM_NOT_MAPPED = -1,
    HTC_RAW_STREAM_0 = 0,
    HTC_RAW_STREAM_1 = 1,
    HTC_RAW_STREAM_2 = 2,
    HTC_RAW_STREAM_3 = 3,
    HTC_RAW_STREAM_NUM_MAX
} HTC_RAW_STREAM_ID;


#define RAW_HTC_READ_BUFFERS_NUM    4
#define RAW_HTC_WRITE_BUFFERS_NUM   4

typedef struct {
    int currPtr;
    int length;
    unsigned char data[AR6000_BUFFER_SIZE];
    HTC_PACKET    HTCPacket;
} raw_htc_buffer;

/* used by AR6000_IOCTL_WMI_GETREV */
struct ar6000_version {
    A_UINT32        host_ver;
    A_UINT32        target_ver;
    A_UINT32        wlan_ver;
};


struct ar_node_mapping {
    A_UINT8                 macAddress[6];
    A_UINT8                 epId;
    A_UINT8                 txPending;
};


struct ar_cookie {
    A_UINT32               arc_bp[2];    /* Must be first field */
    HTC_PACKET             HtcPkt;       /* HTC packet wrapper */
    struct ar_cookie *arc_list_next;
};

struct ar_hb_chlng_resp {
    A_TIMER                 timer;
    A_UINT32                frequency;
    A_UINT32                seqNum;
    A_BOOL                  outstanding;
    A_UINT8                 missCnt;
    A_UINT8                 missThres;
};

typedef struct {
    A_UINT16                flags;
    A_UINT8                 mac[ATH_MAC_LEN];
    A_UINT8                 aid;
    A_UINT8                 wpa_ie[IEEE80211_MAX_IE];
  //  A_NETBUF_QUEUE_T        psq;    /* power save q */	//bluebird
    A_MUTEX_T               psqLock;
} sta_t;


#define ATH_DHCP_PKT_SIZE             342
#define ATH_DHCP_OPCODE_MSG_TYPE      53
#define ATH_DHCP_MSG_TYPE_LEN         1

#define ATH_DHCP_DISCOVER             1
#define ATH_DHCP_REQUEST              3
#define ATH_DHCP_ACK                  5

#define ATH_DHCP_INVALID_MSG          99
#define A_DHCP_TIMER_INTERVAL       5 * 1000

#pragma pack(push,1)
typedef PREPACK struct ether2_hdr {
    A_UINT8     destMAC[ATH_MAC_LEN];
    A_UINT8     srcMAC[ATH_MAC_LEN];
    A_UINT16    etherType;
} POSTPACK ETHERII_HDR;
#pragma pack(pop)

#pragma pack(push,1)
typedef PREPACK struct ip_hdr {
    A_UINT8     version_HdrLength;     /* [Version (4 bits)][Header Length (4 bits)] */
    A_UINT8     TOS;
    A_UINT16    totalLength;
    A_UINT16    identification;
    A_UINT16    flags_FragmentOffset;  /* [Flags (3bits)][Fragment Offset 13 bits] */
    A_UINT8     TTL;
    A_UINT8     protocol;
    A_UINT16    headerCheckSum;
    A_UINT32    sourceIP;
    A_UINT32    destIP;
} POSTPACK IP_HDR;
#pragma pack(pop)

#pragma pack(push,1)
typedef PREPACK struct udp_hdr{
    A_UINT16 sourcePort;
    A_UINT16 destPort;
    A_UINT16 length;
    A_UINT16 checkSum;
} POSTPACK UDP_HDR;
#pragma pack(pop)

#pragma pack(push,1)
typedef PREPACK struct dhcp_msg{

    A_UINT8     opCode;
    A_UINT8     hwAddressType;
    A_UINT8     hwAddressLength;
    A_UINT8     hops;
    A_UINT32    transactionID;
    A_UINT16    seconds;
    A_UINT16    flags;
    A_UINT32    clientIPAddress;       
    A_UINT32    yourIPAddress;        
    A_UINT32    serverIPAddress;       
    A_UINT32    relayIPAddress;        
    A_UINT16    clientHwAddress[8];     
    A_UINT8     bootP_Data[192];
    A_UINT8     magicCookie[4];
} POSTPACK DHCP_MSG;
#pragma pack(pop)

#pragma pack(push,1)
typedef PREPACK struct dhcp_packet{
    ETHERII_HDR  ether2Hdr;
    IP_HDR       ipHdr;
    UDP_HDR      udpHdr;
    DHCP_MSG     dhcpMsg;
    A_UINT8      dhcpOptions[256];
} POSTPACK DHCP_PACKET;
#pragma pack(pop)




#ifdef __cplusplus
}
#endif

#endif
