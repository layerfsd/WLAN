#pragma once




#define HCI_GRP_LINK_CONTROL_CMDS                 (0x01 << 10)
#define HCI_GRP_LINK_POLICY_CMDS                  (0x02 << 10)
#define HCI_GRP_HOST_CONT_BASEBAND_CMDS           (0x03 << 10)
#define HCI_GRP_INFORMATIONAL_PARAMS              (0x04 << 10)
#define HCI_GRP_STATUS_PARAMS                     (0x05 << 10)
#define HCI_GRP_TESTING_CMDS                      (0x06 << 10)
#define HCI_GRP_L2CAP_CMDS                        (0x07 << 10)
#define HCI_GRP_L2CAP_HCI_EVTS                    (0x08 << 10)
#define HCI_GRP_VENDOR_SPECIFIC                   (0x3F << 10)


#define HCI_RESET                                 (0x0003 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_READ_SCAN_ENABLE                      (0x0019 | HCI_GRP_HOST_CONT_BASEBAND_CMDS)
#define HCI_WRITE_SCAN_ENABLE                     (0x001A | HCI_GRP_HOST_CONT_BASEBAND_CMDS)

#define HCI_READ_LOCAL_VERSION_INFO               (0x0001 | HCI_GRP_INFORMATIONAL_PARAMS)

#define HCI_BRCM_SUPER_PEEK_POKE                  (0x000A | HCI_GRP_VENDOR_SPECIFIC)
#define VSC_WRITE_BD_ADDR                         (0x0001 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_UPDATE_BAUDRATE_CMD              (0x0018 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_WRITE_SCO_PCM_INT_PARAM          (0x001C | HCI_GRP_VENDOR_SPECIFIC)
#define VSC_WRITE_PCM_DATA_FORMAT_PARAM           (0x001E | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_WRITE_SLEEP_MODE                 (0x0027 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_DOWNLOAD_MINI_DRV                (0x002E | HCI_GRP_VENDOR_SPECIFIC)
#define VSC_WRITE_UART_CLOCK_SETTING              (0x0045 | HCI_GRP_VENDOR_SPECIFIC)


#define VOICE_SETTING_MU_LAW_MD                   0x0100
#define VOICE_SETTING_LINEAR_MD                   0x0060

#define HCI_ARM_MEM_PEEK                          0x04
#define HCI_ARM_MEM_POKE                          0x05

#define BTUI_MAX_STRING_LENGTH_PER_LINE           255
#define HCI_BRCM_WRITE_SLEEP_MODE_LENGTH          10

#define HCI_BRCM_UPDATE_BAUD_RATE_ENCODED_LENGTH    0x02
#define HCI_BRCM_UPDATE_BAUD_RATE_UNENCODED_LENGTH  0x06

#define VSC_WRITE_UART_CLOCK_SETTING_LEN          1


#define BTA_PRM_SUBVER_BCM2045_B2                 0x420B
#define BTA_PRM_SUBVER_BCM2048_A1                 0x2222
#define BTA_PRM_SUBVER_BCM2048_B0                 0x410B
#define BTA_PRM_SUBVER_BCM2046_B0                 0x4128
#define BTA_PRM_SUBVER_BCM2046_B1                 0x420E
#define BTA_PRM_SUBVER_BCM2070_B0                 0x4120
#define BTA_PRM_SUBVER_BCM4325_D0                 0x8107
#define BTA_PRM_SUBVER_BCM4325_D1                 0x8108 /* BE CAREFUL - Just set D1 as fake value, 0x8108 since found out D1 has same value with D0. */
#define BTA_PRM_SUBVER_BCM                        0x0000 /* NOT USED */

/* ARM Memory Read :
 * Access_Type uint8
 * ARM_Memory_Address uint32(little endian) */
#define VSC_SUPER_PEEK_POKE_CMD                  HCI_BRCM_SUPER_PEEK_POKE
#define VSC_ARM_MEM_PEEK_CMD                     VSC_SUPER_PEEK_POKE_CMD
#define VSC_ARM_MEM_PEEK_CMD_TYPE                HCI_ARM_MEM_PEEK
#define VSC_ARM_MEM_PEEK_CMD_ADDR_OFFSET         1
#define VSC_ARM_MEM_PEEK_CMD_LEN                 5


#define ROTATE_BD_ADDR(p1, p2)    \
									do		   \
									{							\
										p1[0] = p2[5];		\
										p1[1] = p2[4];		\
										p1[2] = p2[3];		\
										p1[3] = p2[2];		\
										p1[4] = p2[1];		\
										p1[5] = p2[0];		\
									} while (0)	



#if 1 // DUT

#endif


int BTTestLibMain(int argc, WCHAR **argv);

void bcmMain(HANDLE fd);
void packetDump(unsigned char *buffer, int length);

void downloadPatchram(void);
void read_event(unsigned char *buf);
void sendMessage(unsigned char *buf, int size);
unsigned char SendCommand(unsigned short opcode, unsigned char param_len, unsigned char *p_param_buf);
void ChangeBaudRate(UINT32 baudrate);
void setHandle(HANDLE FD);
void bcmMain(HANDLE FD);

int BRCMOpenComm (void);
extern unsigned char rcvBuffer[1024];
extern unsigned char patchramBuffer[1024];
extern unsigned int baudrate;