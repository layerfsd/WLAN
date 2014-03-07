/****************************************************************************
 *  (c) Copyright 2007 Wi-Fi Alliance.  All Rights Reserved
 *
 *
 *  LICENSE
 *
 *  License is granted only to Wi-Fi Alliance members and designated
 *  contractors ($B!H(BAuthorized Licensees$B!I(B)..AN  Authorized Licensees are granted
 *  the non-exclusive, worldwide, limited right to use, copy, import, export
 *  and distribute this software:
 *  (i) solely for noncommercial applications and solely for testing Wi-Fi
 *  equipment; and
 *  (ii) solely for the purpose of embedding the software into Authorized
 *  Licensee$B!G(Bs proprietary equipment and software products for distribution to
 *  its customers under a license with at least the same restrictions as
 *  contained in this License, including, without limitation, the disclaimer of
 *  warranty and limitation of liability, below..AN  The distribution rights
 *  granted in clause
 *  (ii), above, include distribution to third party companies who will
 *  redistribute the Authorized Licensee$B!G(Bs product to their customers with or
 *  without such third party$B!G(Bs private label. Other than expressly granted
 *  herein, this License is not transferable or sublicensable, and it does not
 *  extend to and may not be used with non-Wi-Fi applications..AN  Wi-Fi Alliance
 *  reserves all rights not expressly granted herein..AN 
 *.AN 
 *  Except as specifically set forth above, commercial derivative works of
 *  this software or applications that use the Wi-Fi scripts generated by this
 *  software are NOT AUTHORIZED without specific prior written permission from
 *  Wi-Fi Alliance.
 *.AN 
 *  Non-Commercial derivative works of this software for internal use are
 *  authorized and are limited by the same restrictions; provided, however,
 *  that the Authorized Licensee shall provide Wi-Fi Alliance with a copy of
 *  such derivative works under a perpetual, payment-free license to use,
 *  modify, and distribute such derivative works for purposes of testing Wi-Fi
 *  equipment.
 *.AN 
 *  Neither the name of the author nor "Wi-Fi Alliance" may be used to endorse
 *  or promote products that are derived from or that use this software without
 *  specific prior written permission from Wi-Fi Alliance.
 *
 *  THIS SOFTWARE IS PROVIDED BY WI-FI ALLIANCE "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A.AN PARTICULAR PURPOSE,
 *  ARE DISCLAIMED. IN NO EVENT SHALL WI-FI ALLIANCE BE LIABLE FOR ANY DIRECT,
 *  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, THE COST OF PROCUREMENT OF SUBSTITUTE
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE) ARISING IN ANY WAY OUT OF
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. ******************************************************************************
 */

/*
 * * APTS messages/tests
 * */
#define APTS_DEFAULT    1               // message codes
#define APTS_HELLO      2
#define APTS_HELLO_RESP 3
#define APTS_CONFIRM    4
#define APTS_STOP       5
#define APTS_CK_BE      6
#define APTS_CK_BK      7
#define APTS_CK_VI      8
#define APTS_CK_VO      9
#define APTS_TESTS      10              // test codes begin after APTS_TESTS
#define B_D             11
#define B_2             12
#define B_H             13
#define B_4             14
#define B_5             15
#define B_6             16
#define B_B             17
#define B_E             18
#define B_G             19
#define B_M             20
#define M_D             21
#define M_G             22
#define M_I             23
#define B_Z             24
#define M_Y             25
#define L_1             26
#define DLOAD           27
#define ULOAD           28
#define APTS_PASS       29
#define APTS_FAIL       30
#define A_Y             31              // active mode version of M_Y
#define B_W             32              //
#define A_J             33              // Active test of sending 4 down
#define M_V             34
#define M_U             35
#define A_U             36
#define M_L             37
#define B_K             38
#define M_B             39
#define M_K             40
#define M_W             41
#define APTS_LAST       42              // reminder: update APTS_LAST when adding tests
#define APTS_BCST       99

#define PORT            12345          // use a common port for sending/receiving
#define NTARG           32

#define WFA_DEFAULT_CODEC_SEC         0
#define WFA_DEFAULT_CODEC_USEC        10000

#define WFA_WMMPS_UDP_PORT            12345        // it must sync with console

/* wmm defs */
#define TOS_VO7     0xE0                // 111 0  0000 (7)  AC_VO tos/dscp values
#define TOS_VO      0xD0                // 110 0  0000 (6)  AC_VO tos/dscp values
//Atheros Madwifi use 0x88 for UPSD/Voice
//#define TOS_VO      0x88               // 110 0  0000 (6)  AC_VO tos/dscp values

#define TOS_VI      0xA0                // 101 0  0000 (5)  AC_VI
#define TOS_VI4     0x80                // 100 0  0000 (4)  AC_VI

#define TOS_BE      0x00                // 000 0  0000 (0)  AC_BE
#define TOS_EE      0x60                // 011 0  0000 (3)  AC_BE

#define TOS_BK      0x20                // 001 0  0000 (1)  AC_BK
#define TOS_LE      0x40                // 010 0  0000 (2)  AC_BK


/*
 * * power management
 * */
#define PS_ON    1
#define PS_OFF   0

/*
 * * internal table
 * */
struct apts_msg {                       //
    char *name;                     // name of test
    int cmd;                        // msg num
    int param0;                     // number of packet exchanges
    int param1;                     // number of uplink frames
    int param2;                     // number of downlink frames
    int param3;
};

/*
 * * Wait/Timer states
 * */
typedef enum {
    WFA_WAIT_NEXT_CODEC,
    WFA_WAIT_FOR_AP_RESPONSE,
    WFA_WAIT_STAUT_00,
    WFA_WAIT_STAUT_01,
    WFA_WAIT_STAUT_02,
    WFA_WAIT_STAUT_03,
    WFA_WAIT_STAUT_04,
    WFA_WAIT_STAUT_0E,
    WFA_WAIT_STAUT_VOLOAD,
    WFA_WAIT_STAUT_SEQ,
} WAIT_MODE;

#ifdef WFA_WMM_EXT
#ifdef WFA_WMM_PS_EXT
typedef struct wfa_wmmps
{
    int my_sta_id;
    int my_group_cookie;
    int my_cookie;
    int thr_flag;
    int sta_state;
    int sta_test;
    int wait_state;
    int nextsleep;
    int nsent;
    int msgno;
    struct sockaddr_in psToAddr;
    pthread_t thr;
    pthread_cond_t thr_flag_cond;
    pthread_mutex_t thr_flag_mutex;

}wfaWmmPS_t;
#endif
#endif

