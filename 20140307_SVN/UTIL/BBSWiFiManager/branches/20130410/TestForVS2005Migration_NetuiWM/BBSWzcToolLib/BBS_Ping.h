#pragma once

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <icmpapi.h>



#define MAX_BUFFER_SIZE				(sizeof(ICMP_ECHO_REPLY) + 0xfff7 + MAX_OPT_SIZE)
#define DEFAULT_BUFFER_SIZE         (0x2000 - 8)
#define DEFAULT_SEND_SIZE           32
#define DEFAULT_TTL                 32
#define DEFAULT_TOS                 0
#define DEFAULT_TIMEOUT             1000L
#define MIN_INTERVAL                1000L




// Ping Option을 설정하기 위한 Input용 구조체
typedef struct _BBS_PING_OPTION {
	TCHAR	tszDestIP[64];
	DWORD	dwSendBufSize;
	UCHAR	ucTtl;
	UCHAR	ucTos;
	DWORD	dwTimeOut;
} BBS_PING_OPTION, *PBBS_PING_OPTION;


// Ping 결과에 대한 데이터를 저장하기 위한 Output용 구조체
typedef struct _BBS_PING_RESULT {
	DWORD	dwErrCode;
	DWORD	dwDataSize;
	DWORD	dwTime;
	UCHAR	ucTtl;
} BBS_PING_RESULT, *PBBS_PING_RESULT;



class CBBS_Ping
{
public:
	CBBS_Ping(void);
	~CBBS_Ping(void);
	
	// Set Ping Option and ready for ping test
	BOOL InitPing(BBS_PING_OPTION pingOption, LPTSTR strLiteralIP);

	// Real Ping Test
	// Must call InitPingTest() Before call this function
	BOOL PingStartAndGetResult(PBBS_PING_RESULT pResult);
	
	// Ping Test
	BOOL StartPing(PBBS_PING_RESULT pPingResult);


	// return resource and cleanUp
	void DeinitPing();

	// IP주소 혹은 도메인 주소로 타겟 어드레스 구하기
	BOOL ResolveTarget( wchar_t *wTargetString, SOCKADDR *TargetAddress, socklen_t *TargetAddressLen );


private:
	UCHAR					*m_pucSendBuffer;
	UCHAR					*m_pucRcvBuffer;
	DWORD					m_dwSendSize;
	DWORD					m_dwRecvSize;
	UCHAR					m_ucTtl;
	UCHAR					m_ucTos;
	DWORD					m_dwTimeOut;
	WSADATA					m_wsaData;
	char					m_pcDestIPAddr[INET_ADDRSTRLEN];
	SOCKADDR_STORAGE		m_sockAddress;
	socklen_t				m_sockAddressLen;
	HANDLE					m_hIcmp;
	IP_OPTION_INFORMATION	m_sendOption;
	char					m_pcLiteralIP[INET_ADDRSTRLEN];
};
