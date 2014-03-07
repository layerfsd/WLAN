#include "stdafx.h"
#include "BTTestLib.h"






unsigned char rcvBuffer[1024];
unsigned char patchramBuffer[1024]={0x00,};
unsigned int baudrate = 115200;
static HANDLE fd = INVALID_HANDLE_VALUE;

void SetPcmConf( UINT8 p0, UINT8 p1, UINT8 p2, UINT8 p3, UINT8 p4 );
void SetScoConf( UINT8 p0, UINT8 p1, UINT8 p2, UINT8 p3, UINT8 p4 );
void SetAudio(void);

unsigned char vsc_for_pcm_config[5] = {0x00, 0x00, 0x03, 0x03, 0x00};
/*
                    Byte1 -- 0 for MSb first
                    Byte2 -- 0 Fill value
                    Byte3 -- 1 Fill option (1 - programmable, 0 - copy sign)
                    Byte4 -- 1 Number of fill bits
                    Byte5 -- 1 Right justified (0 for left justified)
*/

unsigned char vsc_for_sco_pcm[5]    = {0x00, 0x01, 0x00, 0x01, 0x01};
/*
                    Neverland : PCM, 256, short, master ,master
                    Volance   : PCM, 256, short, master ,master
                    
                    Byte1 -- 0 for PCM 1 for UART or USB
                    Byte2 -- 0 : 128, 1: 256, 2:512, 3:1024, 4:2048 Khz
                    Byte3 -- 0 for short frame sync 1 for long frame sync
                    Byte4 -- 0 Clock direction 0 for same as sync 1 for opposite direction
                    Byte5 -- 0 for slave 1 for master
*/                    

WCHAR szComPortName[_MAX_PATH] = L"COM2:";

void downloadPatchram()
{

	FILE *fp = NULL;
	int len=0;
//	unsigned char MINI_DRV[4] = {0x01, 0x2e, 0xfc, 0x00 };
	DWORD dwRead;
	int cnt=0;

	fp = fopen("\\Windows\\BCM4329B1.hcd", "rb");
	if (fp == NULL){
		printf("can not open patchram file\n");
		return;
	}


	SendCommand(HCI_RESET, 0, NULL);
	read_event(rcvBuffer);

	ChangeBaudRate(115200);

	printf("HCI_BRCM_DOWNLOAD_MINI_DRV\n");

	SendCommand(HCI_BRCM_DOWNLOAD_MINI_DRV, 0, NULL);
	read_event(rcvBuffer);

 	ReadFile(fd, &patchramBuffer[0], 2, &dwRead, NULL);
	Sleep(100);

	printf("patchram download start\n");
	while ( !feof(fp) ){

//		memset(patchramBuffer, 0x00, 1024);

		len = fread(&patchramBuffer[1], sizeof(unsigned char), 3, fp);

		patchramBuffer[0] = 0x01;
		len = patchramBuffer[3];

		fread(&patchramBuffer[4], sizeof(unsigned char), len, fp);

		sendMessage(patchramBuffer, len+4);
#if 0
	 	printf("patchram => %d >> ", len+4);
		packetDump(patchramBuffer, len+4);
#endif
		read_event( rcvBuffer);

		if (rcvBuffer[4] == 0x4e && rcvBuffer[5] == 0xfc) 
		{
//			printf(" 0x4e, 0xfc\n");
			break;
		}
	}
//printf(" end loop !!\n");

 	read_event(rcvBuffer);
	printf("patchram download complete!!!\n");

//	SendCommand(HCI_RESET, 0, NULL);
//	read_event(rcvBuffer);
	fclose(fp);
}

unsigned char SendCommand(unsigned short opcode, unsigned char param_len, unsigned char *p_param_buf)
{
    UINT8 pbuf[255] = {0,};
    UINT8 i=0;
    
    pbuf[0] = 0x1;
    pbuf[1] = (UINT8)(opcode);
    pbuf[2] = (UINT8)(opcode >>8);
    pbuf[3] = param_len;

    for (i=0; i<param_len; i++)
    {        
        pbuf[i+4] = *p_param_buf++;
    }

	sendMessage(pbuf, param_len+4);
#if 1
printf("write => %d >> ", param_len+4);
packetDump(pbuf, param_len+4);
#endif
	return 0;
}

void sendMessage(unsigned char *buf, int size)
{
	DWORD dwWrit;
//	printf("%x, send -> ", fd);
//	packetDump(buf, size);
	if (!WriteFile(fd, buf, size, &dwWrit, NULL))
	{
		printf("sendMessage Write failed!!\n");
	}

	return;

}


void read_event(unsigned char *buf)
{
	DWORD dwRead;
	int len=3;
	int i=0;
	int count=0;
	
    ReadFile(fd, &buf[i], len, &dwRead, NULL);
	i+= dwRead;
	len = buf[2];
    ReadFile(fd, &buf[i], len, &dwRead, NULL);
	i+=dwRead;
 	len = i;

  	printf("rcv =%d >> ", 3 + buf[2]);
packetDump(buf,  3 + buf[2]);
#if 0
	
	for(i=0;i<len;++i){
		printf("[%x] ",buf[i]); 
		}
	printf("\n");
#endif
}


void packetDump(unsigned char *buffer, int length)
{
#if 1
	int i=0;
//	printf("PACKET DUMP length = %d=====\n", length);
	for(i=0;i<length;++i)
		{
			printf("[%x] ",buffer[i]);
		}
	printf("\n");
#endif
}

void ChangeBaudRate(UINT32 baud_rate)
{
    unsigned char hci_data[HCI_BRCM_UPDATE_BAUD_RATE_UNENCODED_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    unsigned char uart_clock_24 = 0x2;    /* 0x1 - UART Clock 48MHz, 0x2 - UART Clock 24MHz */    
    unsigned char uart_clock_48 = 0x1;    /* 0x1 - UART Clock 48MHz, 0x2 - UART Clock 24MHz */

	return;
#if 0
    /* Write UART Clock setting of 48MHz */
    SendCommand( VSC_WRITE_UART_CLOCK_SETTING, VSC_WRITE_UART_CLOCK_SETTING_LEN, (UINT8 *)&uart_clock);
    read_event(fd, buffer);
#endif

    switch(baud_rate)
    {
        case 115200:
        case 230400:
        case 460800:
        case 921600:
        case 1000000:
        case 1500000:
        case 2000000:
        case 2500000:
            /* Write UART Clock setting of 24MHz */
            SendCommand(VSC_WRITE_UART_CLOCK_SETTING, VSC_WRITE_UART_CLOCK_SETTING_LEN, (UINT8 *)&uart_clock_24);
            read_event(rcvBuffer);        
            hci_data[2] = baud_rate & 0xFF;
            hci_data[3] = (baud_rate >> 8) & 0xFF;
            hci_data[4] = (baud_rate >> 16) & 0xFF;
            hci_data[5] = (baud_rate >> 24) & 0xFF;
            break;        
        case 3000000:
        case 4000000:
            /* Write UART Clock setting of 48MHz */
            SendCommand(VSC_WRITE_UART_CLOCK_SETTING, VSC_WRITE_UART_CLOCK_SETTING_LEN, (UINT8 *)&uart_clock_48);
            read_event(rcvBuffer);        
            hci_data[2] = baud_rate & 0xFF;
            hci_data[3] = (baud_rate >> 8) & 0xFF;
            hci_data[4] = (baud_rate >> 16) & 0xFF;
            hci_data[5] = (baud_rate >> 24) & 0xFF;
            break;
            
        default:
            printf("Not Support baudrate = %d", baud_rate);
//            exit_err(1);
            break;
    }

    SendCommand(HCI_BRCM_UPDATE_BAUDRATE_CMD, HCI_BRCM_UPDATE_BAUD_RATE_UNENCODED_LENGTH, (UINT8 *)hci_data);
    read_event(rcvBuffer);
	printf("Change BaudRate :: %d!!! \n", baud_rate);
}
 

void setHandle(HANDLE FD)
{
	fd = FD;
	if (fd == INVALID_HANDLE_VALUE)
	{
		printf("setHandle :: invalid_handle_value\nplease check comport!!!\n");
		return;
	}
}

void bcmMain(HANDLE FD)
{
	unsigned char scopcm[10] = {0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00}; //0,4,0,0,0,0,0,3,3,0
	if (FD == INVALID_HANDLE_VALUE)
	{
		printf("invalid_handle_value\nplease check comport!!!\n");
		return;
	}
 	fd = FD;

//	loadParam();
	downloadPatchram();

	// bdaddr.

	// pcm/sco setting
  //  SetScoConf( scopcm[0],scopcm[1],scopcm[2],scopcm[3],scopcm[4] );
  //  SetPcmConf( scopcm[5],scopcm[6],scopcm[7],scopcm[8],scopcm[9] );
 //   SetAudio();

	// lpm setting.

}



int BRCMOpenComm (void) {

	HANDLE hFile = INVALID_HANDLE_VALUE;
	int i=0;
	printf("BRCMOpenComm START\n");
	if (hFile != INVALID_HANDLE_VALUE)
		return FALSE;
#define DEFAULT_COM_NAME	L"COM2:"


//	wcscpy (szComPortName, DEFAULT_COM_NAME);

	DWORD dwBaud = 115200;


#if 0
		HKEY hk;
	if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_BASE, L"software\\microsoft\\bluetooth\\hci", 0, KEY_READ, &hk)) {
		DWORD dwType;
		DWORD dwSize = sizeof(szComPortName);
		if ((ERROR_SUCCESS == RegQueryValueEx (hk, L"Name", NULL, &dwType, (LPBYTE)szComPortName, &dwSize)) &&
			(dwType == REG_SZ) && (dwSize > 0) && (dwSize < _MAX_PATH))
			;
		else
			wcscpy (szComPortName, DEFAULT_COM_NAME);

		dwSize = sizeof(dwBaud);
		if ((ERROR_SUCCESS == RegQueryValueEx (hk, L"baud", NULL, &dwType, (LPBYTE)&dwBaud, &dwSize)) &&
			(dwType == REG_DWORD) && (dwSize == sizeof(dwBaud)))
			;
		else
			dwBaud = 115200;
			RegCloseKey (hk);
		}
#endif
		dwBaud = 115200;
//		wcscpy (szComPortName, DEFAULT_COM_NAME);

//		baudrate = dwBaud; // kdongsuk.



	{
		for(i=0;i<5;++i)
			printf("[%c]",szComPortName[i]);
		printf("\n");
	}

	printf("Opening port %s (rate %d) for I/O with unit\n", szComPortName, dwBaud);

    hFile = CreateFile(szComPortName,
                GENERIC_READ | GENERIC_WRITE,
                0,    // comm devices must be opened w/exclusive-access
                NULL, // no security attrs
                OPEN_EXISTING, // comm devices must use OPEN_EXISTING
#if ! defined (UNDER_CE)
				FILE_FLAG_OVERLAPPED |
#endif
                FILE_ATTRIBUTE_NORMAL,    // overlapped I/O 
                NULL  // hTemplate must be NULL for comm devices  
				);

    if (hFile == INVALID_HANDLE_VALUE) {
		printf("Failed CreateFile in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ());
		return FALSE;

    }

    if (! SetupComm (hFile, 20000, 20000)) {
		CloseHandle (hFile);
		printf("Failed SetupComm in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ());
		hFile = INVALID_HANDLE_VALUE;
		return FALSE;
    } 

    // purge any information in the buffer

    if ( ! PurgeComm (hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR )) {
		CloseHandle (hFile);
		hFile = INVALID_HANDLE_VALUE;
		printf("Failed PurgeComm in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ());
		return FALSE;
    }

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;

    if (! SetCommTimeouts (hFile, &CommTimeOuts)) {

		CloseHandle (hFile);
		hFile = INVALID_HANDLE_VALUE;
		return FALSE;
    }


    DCB dcb;

    dcb.DCBlength = sizeof(dcb);
    dcb.BaudRate = 115200; // dwBaud // default baudrate is 115200.
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;
    dcb.fOutxCtsFlow = TRUE;
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fOutxDsrFlow = FALSE;    
    dcb.fDsrSensitivity = FALSE;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fTXContinueOnXoff = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
    dcb.fAbortOnError = TRUE;
//    dcb.wReserved = 0;
    dcb.ByteSize =8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.XonChar = 0x11;
    dcb.XoffChar = 0x13;
    dcb.XonLim = 3000 ;
    dcb.XoffLim = 9000 ;
    if (! SetCommState(hFile, &dcb)) {
		printf("Failed SetCommState in UART HCI Interface. GetLastError = 0x%08x\n", GetLastError ());
		CloseHandle (hFile);
		hFile = INVALID_HANDLE_VALUE;
		return FALSE;
    }

//	ChangeBaudRate(dwBaud);
	setHandle(hFile);

// 	SendCommand(HCI_RESET, 0, NULL);
// 	read_event(rcvBuffer);

//	CloseHandle (hFile);
	printf("BRCMOpenComm END\n");

	return TRUE;
}


void SetPcmConf( UINT8 p0, UINT8 p1, UINT8 p2, UINT8 p3, UINT8 p4 )
{
    vsc_for_pcm_config[0] = p0;
    vsc_for_pcm_config[1] = p1;
    vsc_for_pcm_config[2] = p2;
    vsc_for_pcm_config[3] = p3;
    vsc_for_pcm_config[4] = p4;
}

void SetScoConf( UINT8 p0, UINT8 p1, UINT8 p2, UINT8 p3, UINT8 p4 )
{
    vsc_for_sco_pcm[0] = p0;
    vsc_for_sco_pcm[1] = p1;
    vsc_for_sco_pcm[2] = p2;
    vsc_for_sco_pcm[3] = p3;
    vsc_for_sco_pcm[4] = p4;
}

void SetAudio(void)
{

//    DEBUG5( "vsc_for_sco_pcm = {%d,%d,%d,%d,%d}\n", vsc_for_sco_pcm[0],
//                                         vsc_for_sco_pcm[1],vsc_for_sco_pcm[2],
//                                         vsc_for_sco_pcm[3],vsc_for_sco_pcm[4]);

    SendCommand(HCI_BRCM_WRITE_SCO_PCM_INT_PARAM, 5, (UINT8 *)vsc_for_sco_pcm);
    read_event(rcvBuffer);

//   DEBUG5( "vsc_for_pcm_config = {%d,%d,%d,%d,%d}\n", vsc_for_pcm_config[0],
//                                        vsc_for_pcm_config[1], vsc_for_pcm_config[2],
//                                        vsc_for_pcm_config[3], vsc_for_pcm_config[4]);

    SendCommand(VSC_WRITE_PCM_DATA_FORMAT_PARAM, 5, (UINT8 *)vsc_for_pcm_config);
    read_event(rcvBuffer);

}



int enableDUTmode()
{
    unsigned char hci_write_scan_enable[] = { 0x01, 0x1a, 0x0c, 0x01, 0x03 };
    unsigned char hci_set_event_filter[]={0x01, 0x05, 0x0C, 0x03, 0x02, 0x00, 0x02};
    unsigned char hci_enable_device_under_test[] = { 0x01, 0x03, 0x18, 0x00 };
    unsigned char hci_sleep_disable[] = { 0x01, 0x27, 0xfc, 0x0c, 0x0,0x0,0x0, 0x0,0x0,0x0, 0x0,0x0,0x0, 0x0,0x0,0x0 };
//		sendMessage(patchramBuffer, len+4);
printf("enableDUTmode start");
	sendMessage(hci_sleep_disable, sizeof(hci_sleep_disable));
	read_event(rcvBuffer);
	sendMessage(hci_write_scan_enable, sizeof(hci_write_scan_enable));
	read_event(rcvBuffer);
	sendMessage(hci_set_event_filter, sizeof(hci_set_event_filter));
	read_event(rcvBuffer);
	sendMessage(hci_enable_device_under_test, sizeof(hci_enable_device_under_test));
	read_event(rcvBuffer);
printf("enableDUTmode end");

	return 0;
}
int setBDAddr(UINT8 *bd_Addr)
{
	
#define ROTATE_BD_ADDR(p1, p2)    \
                  do       \
                  {             \
                    p1[0] = p2[5];    \
                    p1[1] = p2[4];    \
                    p1[2] = p2[3];    \
                    p1[3] = p2[2];    \
                    p1[4] = p2[1];    \
                    p1[5] = p2[0];    \
                  } while (0)

	UINT8 addr[6];
	ROTATE_BD_ADDR(addr,bd_Addr);
//		UINT8 bd_Addr[6] = {0x55, 0x44, 0x33, 0x22, 0x11, 0x00};
		SendCommand(VSC_WRITE_BD_ADDR, 6,(UINT8 *) addr);
		read_event(rcvBuffer);
	return 0;
}
int BTTestLibMain(int argc, WCHAR **argv)
{

 	//BRCMOpenComm();

//	SendCommand(HCI_RESET, 0, NULL);
//	read_event(rcvBuffer);
	if (argc == 1)
	{
		printf("Usage : bttest COM[1-9]: [ up | dut | addr | cmd | reset]\n");
		return 0;
	}

	wcscpy (szComPortName, argv[1]);

	if (BRCMOpenComm() == FALSE)// open comport.
		return 0;
//	printf("argv[1] = %s\n",argv[1]);

	if (!wcscmp(argv[2], L"up"))
	{
		downloadPatchram();
	}
	else if (!wcscmp(argv[2], L"addr"))
	{
		UINT8 BD_ADDR[6];
		int i;
		for(i=0;i<6;++i)
			BD_ADDR[i] = (UINT8)wcstol(argv[3+i], NULL, 16);
		setBDAddr(BD_ADDR);
	}
	else if (!wcscmp(argv[2], L"dut"))
	{
		enableDUTmode();
	}
	else if (!wcscmp(argv[2], L"reset"))
	{
		SendCommand(HCI_RESET, 0, NULL);
		read_event(rcvBuffer);
	}
	else if (!wcscmp(argv[2], L"cmd"))
	{
		DWORD ocf;
		DWORD ogf;
		DWORD cmd[32];
		unsigned char sendBuf[32];
		int argcnt, i;
		unsigned short opcode;
		unsigned char *tmp;

		ogf =  wcstol(argv[3], NULL, 16);
		ocf =  wcstol(argv[4], NULL, 16);
		opcode = (unsigned short)(ocf&0x03ff | ogf << 10); // get opcode.

		tmp = (unsigned char *)&opcode;
		printf("ogf = %x, ocf = %x, opcode = %x\n",ogf, ocf, opcode);
		argcnt = argc-5;
		for(i=0;i<argcnt;++i)
		{
			cmd[i] = wcstol(argv[5+i], NULL, 16);
		}
		sendBuf[0] = 0x01; //cmd
		sendBuf[1] = tmp[0]; // opcode1
		sendBuf[2] = tmp[1]; // opcode2
		sendBuf[3] = argcnt; // len/

		for(i=0;i<argcnt;++i)
			sendBuf[4+i] = (unsigned char)cmd[i];
		for(i=0;i<argcnt+4;++i)
			printf("[%x] ",sendBuf[i]);
		printf("\n");
		sendMessage(sendBuf, argcnt+4);
		Sleep(100);
		read_event( rcvBuffer);
	}
	CloseHandle (fd);
	return 0;
}

