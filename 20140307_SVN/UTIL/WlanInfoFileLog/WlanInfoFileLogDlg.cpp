// WlanInfoFileLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WlanInfoFileLog.h"
#include "WlanInfoFileLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <Sipapi.h>
#include "eapol.h"
#include "wzcsapi.h"
#include <icmpapi.h>
#include <Winsock2.h>
#include "Reg.h"
#include "nuiouser.h"

#pragma comment(lib, "wzcsapi.lib")
#pragma comment(lib, "Ws2.lib")
#pragma comment(lib, "Iphlpapi.lib")

#define TIMER_WZC_NETWORK_REFRESH	(1)

#define MAX_LOGBUF_SIZE					(0xA000) // 40960
#define LOG_FLUSH_SIZE					(0x2800) // 10240

#define NETWORK_REFRESH_INTERVAL		(3000) // 3sec

BOOL g_bIsDetecingWLInfo;
BOOL g_bIsAlivePingThread;


CString g_strPingDestIP = L"";
TCHAR g_tszWirelessName[256] = {'\0',};
BOOL g_bFileLogEnable = FALSE;



#define RK_FLASH_STORE_PATH				L"Drivers\\BuiltIn\\FlshDrv"
#define RV_FLASH_STORE_NAME				L"FolderName"



static char g_szFileLogBuf[MAX_LOGBUF_SIZE] = {'\0',};


static void FileLogBufFlush()
{
	DWORD	dwBytesWritten = 0;
	HANDLE	hLogFile = NULL;    
	SYSTEMTIME	sysTime = {0,};


	GetLocalTime(&sysTime);

	CString strLogFilePath = _T("");






	// Get Log Folder Name
	HKEY hKey = NULL;
	DWORD dwDataSize = 0;
	LONG lRet = 0;
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, RK_FLASH_STORE_PATH, 0, 0, &hKey) == ERROR_SUCCESS )
	{
		TCHAR tszData[128] = {'\0',};
		dwDataSize = sizeof(tszData);
		lRet = RegQueryValueEx(hKey, RV_FLASH_STORE_NAME, NULL, NULL, (LPBYTE)&tszData, &dwDataSize);


		if( (lRet != ERROR_SUCCESS) || ( _tcslen(tszData) <= 0 ) )
		{
			strLogFilePath.Format(_T("WifiLog_%02d%02d.txt"), sysTime.wMonth, sysTime.wDay);
		}
		else
		{
			strLogFilePath.Format(_T("\\%s\\WifiLog_%02d%02d.txt"),tszData, sysTime.wMonth, sysTime.wDay);
		}

		RegCloseKey(hKey);
		hKey = NULL;
	}


	SetFilePointer(hLogFile , 0, NULL, FILE_END);


	hLogFile = CreateFile(strLogFilePath, GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	SetFilePointer(hLogFile , 0, NULL, FILE_END);
	

	// 파일 로그 기록
	WriteFile(hLogFile, g_szFileLogBuf, strlen(g_szFileLogBuf), &dwBytesWritten, 0);


	memset(g_szFileLogBuf, '\0', sizeof(g_szFileLogBuf));

	CloseHandle(hLogFile);
}

static void WriteFileLog(BOOL bTimeStamp, void* strFormat, ...)
{
	TCHAR   tszMsgBuff[1024] = {'\0',};
	CHAR    szMsgBuff[1024] = {'\0',};
	DWORD	dwStrLen = 0;
	SYSTEMTIME	sysTime = {0,};
	CHAR    szTime[32] = {'\0',};


	if(g_bFileLogEnable)
	{

		va_list lpStart;
		va_start(lpStart, strFormat);
		vsprintf(szMsgBuff, (LPSTR)strFormat, lpStart);
		va_end(lpStart);

		dwStrLen = strlen(szMsgBuff);

		// WideChar 인경우
		if(dwStrLen < 2)
		{
			va_list lpStart;
			va_start(lpStart, strFormat);
			vswprintf(tszMsgBuff, (LPTSTR)strFormat, lpStart);
			va_end(lpStart);  

			WideCharToMultiByte( CP_ACP, 0, tszMsgBuff, _tcslen(tszMsgBuff), szMsgBuff, sizeof(szMsgBuff), &szMsgBuff[0], NULL);
		}

		if(bTimeStamp)
		{
			// 현재 시간 기록
			GetLocalTime(&sysTime);
			sprintf(szTime, "[%02d/%02d, %02d:%02d:%02d] ", sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			//strcat(g_szFileLogBuf, szTime);	
		}


		strcat(g_szFileLogBuf, szMsgBuff);

		if( strlen(g_szFileLogBuf) >= LOG_FLUSH_SIZE ) 
		{
			FileLogBufFlush();
		}		

	}

}




void GetWirelessDriverName(WCHAR* szBuff);
void PrintWifiInfo(CWlanInfoFileLogDlg* context);
void APInfoThread(CWlanInfoFileLogDlg* context);
void PingThread(CWlanInfoFileLogDlg* context);
BOOL PingTest(LPCWSTR strIpAddr, ICMP_ECHO_REPLY& reply);
/////////////////////////////////////////////////////////////////////////////
// CWlanInfoFileLogDlg dialog

CWlanInfoFileLogDlg::CWlanInfoFileLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWlanInfoFileLogDlg::IDD, pParent)
	, m_bIsCheckLogEn(FALSE)
{
	//{{AFX_DATA_INIT(CWlanInfoFileLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWlanInfoFileLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWlanInfoFileLogDlg)
	DDX_Control(pDX, IDC_EDIT_PING_RESULT, m_EditPingResult);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_FILELOG, m_bIsCheckLogEn);
}

BEGIN_MESSAGE_MAP(CWlanInfoFileLogDlg, CDialog)
	//{{AFX_MSG_MAP(CWlanInfoFileLogDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOG_START, OnButtonLogStart)
	ON_BN_CLICKED(IDC_BUTTON_LOG_END, OnButtonLogEnd)
	ON_EN_SETFOCUS(IDC_EDIT_IPADRESS, OnSetfocusEditIpadress)
	ON_EN_KILLFOCUS(IDC_EDIT_IPADRESS, OnKillfocusEditIpadress)
	ON_EN_CHANGE(IDC_EDIT_IPADRESS, OnChangeEditIpadress)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()
	
	
CString GetIPAddress(void)
{
	WORD wVersionRequested = MAKEWORD(2,2);
	WSADATA wsaData = {0,};
	char pcHostName[255] = {'\0',};
	CString strIPAddress = _T("");
	PHOSTENT pHostInfo = NULL;
	
	if( WSAStartup(wVersionRequested, &wsaData) == 0 )
	{
		if( gethostname(pcHostName, sizeof(pcHostName)) == 0 )
		{
			pHostInfo=gethostbyname(pcHostName);
			
			if( pHostInfo != NULL )
			{
				strIPAddress=inet_ntoa(*(struct in_addr *)*pHostInfo->h_addr_list);
			}
		}
		WSACleanup();
	}

	WriteFileLog(1, _T("[IPADDRESS] : %s \r\n"), strIPAddress);
	return strIPAddress;
	
}
/////////////////////////////////////////////////////////////////////////////
// CWlanInfoFileLogDlg message handlers

BOOL CWlanInfoFileLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here
	g_bIsDetecingWLInfo = FALSE;
	g_bIsAlivePingThread = FALSE;

	// Get FileVer
	GetCurrentFileVersion();


	// Get Wireless Name
	GetWirelessDriverName(g_tszWirelessName);
	
	// Get Current IP Address
	CString strCurIPAddr = GetIPAddress();
	SetDlgItemText(IDC_EDIT_IPADDR, strCurIPAddr);

	// Get Default Gateway
	
	DWORD   dwDHCP  = -1;
	CString strKey  = L"";
	TCHAR tszGateway[128] = _T("");
	strKey.Format(L"\\Comm\\%s\\Parms\\TcpIp", g_tszWirelessName);

	CReg *regGateWay = new CReg(HKEY_LOCAL_MACHINE, strKey);

	dwDHCP = regGateWay->ValueDW(_T("EnableDHCP"));

	if (dwDHCP == 0)
	{
		regGateWay->ValueSZ(_T("DefaultGateway"), tszGateway, 32);
	}
	else if(dwDHCP == 1)
	{
		regGateWay->ValueSZ(_T("DhcpDefaultGateway"), tszGateway, 32);
	}
	
	SetDlgItemText(IDC_EDIT_IPADRESS, tszGateway);
	
	delete regGateWay;



	GetDlgItem(IDC_BUTTON_LOG_END)->EnableWindow(FALSE);
	



	return TRUE;  // return TRUE  unless you set the focus to a control
}


void APInfoThread(CWlanInfoFileLogDlg* context)
{
	while(g_bIsDetecingWLInfo)
	{

		// Update Current IP Address
		CString strCurIPAddr = GetIPAddress();
		context->SetDlgItemText(IDC_EDIT_IPADDR, strCurIPAddr);
		
		// Print WifiInfo
		PrintWifiInfo(context);

		Sleep(2000);
	}
}

void PingThread(CWlanInfoFileLogDlg* context)
{
	ICMP_ECHO_REPLY     reply;

	while(g_bIsAlivePingThread)
	{
		WriteFileLog(1, _T("[PING] IP : %s\r\n"), g_strPingDestIP);
		CString strPingResult = _T("");

		if( PingTest(g_strPingDestIP, reply) == TRUE )
		{
			if (reply.RoundTripTime < 1)
			{
				strPingResult.Format(_T("[PING] RTT<1ms, TTL=%d, Data=%dbytes\r\n"), reply.Options.Ttl, reply.DataSize);
			}
			else
			{
				strPingResult.Format(_T("[PING] RTT=%dms, TTL=%d, Data=%dbytes\r\n"), reply.RoundTripTime, reply.Options.Ttl, reply.DataSize);
			}
		}
		else
		{
			strPingResult.Format(_T("[PING] Request timeout.\r\n") );
		}

		WriteFileLog(0, strPingResult.GetBuffer(strPingResult.GetLength()) );

		int nLen = context->m_EditPingResult.GetWindowTextLength();
		context->m_EditPingResult.SetSel(nLen, nLen);
		context->m_EditPingResult.ReplaceSel(strPingResult, TRUE);

		Sleep(1000);
	}
}


void CWlanInfoFileLogDlg::OnButtonLogStart() 
{
	// TODO: Add your control notification handler code here

	UpdateData();
	g_bFileLogEnable = m_bIsCheckLogEn;


	WriteFileLog(1, _T("[WLAN Adaptor Name] : %s \r\n"), g_tszWirelessName);

	// for USI module
	// Get RoamingTrigger Setting Value 
	if( _tcscmp(g_tszWirelessName, _T("BCMSDDHD1")) == 0 )
	{
		CReg *regIP = new CReg(HKEY_LOCAL_MACHINE, _T("\\Comm\\BCMSDDHD1\\Parms"));
		TCHAR tszTriggerValue[32] = {'\0',};

		regIP->ValueSZ(_T("RoamTrigger"), tszTriggerValue, 32);

		WriteFileLog(1, _T("[TRIGGER] tszTriggerValue : %s \r\n"), tszTriggerValue);

		delete regIP;
	}

	// Network Refresh Timer (Default 3sec)
	SetTimer(TIMER_WZC_NETWORK_REFRESH, NETWORK_REFRESH_INTERVAL, NULL);

	g_bIsDetecingWLInfo = TRUE;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)APInfoThread, this, 0, NULL);
	CloseHandle(hThread);


	// Control Disable / Enable
	GetDlgItem(IDC_BUTTON_LOG_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_IPADRESS)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_FILELOG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOG_END)->EnableWindow(TRUE);
		
	
	GetDlgItemText(IDC_EDIT_IPADRESS, g_strPingDestIP);
	

	if(g_strPingDestIP.GetLength() > 7) // Vaild IP주소값인지 체크해야하나, 일단 지금은 대략 7자 이상만 되면 진행시작
	{

		g_bIsAlivePingThread = TRUE;
		HANDLE hPingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PingThread, this, 0, NULL);
		CloseHandle(hPingThread);
	}
	
	
}

void CWlanInfoFileLogDlg::OnButtonLogEnd() 
{
	// TODO: Add your control notification handler code here
	g_bIsDetecingWLInfo = FALSE;
	g_bIsAlivePingThread = FALSE;
	GetDlgItem(IDC_BUTTON_LOG_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOG_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_IPADRESS)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_FILELOG)->EnableWindow(TRUE);
	

	// Stop Network Refresh Timer
	KillTimer(TIMER_WZC_NETWORK_REFRESH);

	// Log Flush
	FileLogBufFlush();

	Sleep(1000);
}

void GetWirelessDriverName(TCHAR* tszNameBuf)
{
    INTFS_KEY_TABLE IntfsTable;
    IntfsTable.dwNumIntfs = 0;
    IntfsTable.pIntfs = NULL;

    DWORD dwStatus = WZCEnumInterfaces(NULL, &IntfsTable);

    if(dwStatus != ERROR_SUCCESS)
    {
        RETAILMSG(1,(L"WZCEnumInterfaces() error 0x%08X\r\n", dwStatus));
        return;        
    }

    if(!IntfsTable.dwNumIntfs)
    {
        RETAILMSG(1,(L"system has no wireless card.\r\n"));
        wcscpy(tszNameBuf, (L"has no Wlan card.\r\n"));
        return;
    }

    wcscpy(tszNameBuf, IntfsTable.pIntfs[0].wszGuid);
    // need to free memory allocated by WZC for us.
    LocalFree(IntfsTable.pIntfs);
}   // GetFirstWirelessNetworkCard


// AP LIst 갱신
BOOL RefreshNetworkList()
{
    HANDLE hNdio = CreateFile(_T("UIO1:"),
                              GENERIC_READ | GENERIC_WRITE, 
                              0x00, 
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                              (HANDLE)INVALID_HANDLE_VALUE);

    if (hNdio == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    // LIST_SCAN
    PNDISUIO_SET_OID    pSetOid;
    TCHAR               SetQueryBuffer[1024] = {0, };
    DWORD               dwBytesReturned = 0;
    BOOL                bl = FALSE;

    pSetOid = (PNDISUIO_SET_OID) &SetQueryBuffer[0];
    pSetOid->ptcDeviceName = g_tszWirelessName;
    pSetOid->Oid = OID_802_11_BSSID_LIST_SCAN;

    bl = DeviceIoControl(hNdio,
                         IOCTL_NDISUIO_SET_OID_VALUE,
                         (LPVOID) pSetOid,
                         sizeof(OID_802_11_BSSID_LIST_SCAN) + sizeof(SetQueryBuffer) ,
                         (LPVOID) pSetOid,
                         1024* sizeof(TCHAR),
                         &dwBytesReturned,
                         NULL);
    if (!bl)
    {
        CloseHandle(hNdio);
        return FALSE;
    }

    CloseHandle(hNdio);

    return TRUE;
}

void PrintWifiInfo(CWlanInfoFileLogDlg* context)
{       
    INTF_ENTRY_EX Intf;
    DWORD dwOutFlags;
    memset(&Intf, 0x00, sizeof(INTF_ENTRY_EX));
    Intf.wszGuid = g_tszWirelessName;
    

    DWORD dwStatus = WZCQueryInterfaceEx(NULL, INTF_ALL, &Intf, &dwOutFlags);

    if (dwStatus != ERROR_SUCCESS)
    {
        RETAILMSG(1,(L"WZCQueryInterfaceEx() error 0x%08X\r\n", dwStatus));
        return;
    }

    
	// Get Current MAC Address
	PRAW_DATA prdMAC = &Intf.rdBSSID;
	WCHAR tszMac[128] = {'\0',};
	
	// MAC	
	if (prdMAC->dwDataLen != 0)
	{
		swprintf(tszMac,(TEXT("%02X:%02X:%02X:%02X:%02X:%02X")), prdMAC->pData[0], prdMAC->pData[1], prdMAC->pData[2], prdMAC->pData[3], prdMAC->pData[4], prdMAC->pData[5]);
	}
   
	// Get Around SSID List
    PRAW_DATA prdBSSIDList = &Intf.rdBSSIDList;
	
	NDIS_802_11_RSSI rssi = 0;
	NDIS_802_11_MAC_ADDRESS APmacAddr = {0,};
	NDIS_802_11_SSID ssid = {0,};

	TCHAR tszSSID[256] = {'\0'};
	TCHAR tszAPMAC[128] = {'\0'};

    if (!(prdBSSIDList == NULL || prdBSSIDList->dwDataLen == 0))
    {
        PWZC_802_11_CONFIG_LIST pConfigList = (PWZC_802_11_CONFIG_LIST)prdBSSIDList->pData;

        for (UINT i = 0; i < pConfigList->NumberOfItems; i++)
        {
            PWZC_WLAN_CONFIG pConfig = &(pConfigList->Config[i]);
			wsprintf(tszAPMAC, _T("%02X:%02X:%02X:%02X:%02X:%02X"), pConfig->MacAddress[0], pConfig->MacAddress[1], pConfig->MacAddress[2], pConfig->MacAddress[3], pConfig->MacAddress[4], pConfig->MacAddress[5] );

			// Is same with current SSID Macaddr?
			if(wcscmp(tszMac, tszAPMAC) == 0)
			{
				WriteFileLog(0, "SSID : [%s] / ", pConfig->Ssid.Ssid);
				WriteFileLog(0, _T("[%d] / (%s) \r\n"), pConfig->Rssi, tszAPMAC);


				// for TCHAR SSID
				WCHAR tszSsid[128] = {'\0',};

				for (UINT i = 0; i < pConfig->Ssid.SsidLength; i++)
				{
					tszSsid[i] = pConfig->Ssid.Ssid[i];
					tszSsid[i+1] = L'\0';
				}
				// Display Current Info
				SetDlgItemText(context->m_hWnd, IDC_EDIT_CUR_SSID, tszSsid);
				SetDlgItemInt(context->m_hWnd, IDC_EDIT_CUR_RSSI, pConfig->Rssi, TRUE);
				SetDlgItemText(context->m_hWnd, IDC_EDIT_CUR_MAC, tszAPMAC);
			}
			else
			{
				WriteFileLog(0, "SSID : %s / ", pConfig->Ssid.Ssid);
				WriteFileLog(0, _T("[%d] / (%s) \r\n"), pConfig->Rssi, tszAPMAC);
			}
			
        }
		WriteFileLog(0, _T("\r\n") );
    }

    WZCDeleteIntfObjEx(&Intf);

}



// Ping test
BOOL PingTest(LPCWSTR strIpAddr, ICMP_ECHO_REPLY& reply)
{
    CString strIP       = strIpAddr;
    LPWSTR  addr        = strIP.GetBuffer(strIP.GetLength());
	CHAR    cIPString[128] = {'\0',};

    WideCharToMultiByte(CP_ACP, 0, addr, -1, cIPString, sizeof(cIPString), NULL, NULL);

    IPAddr		ipaddr;	 
	WSADATA		wsadata;
	HANDLE		hIcmp;

    BOOL        bResult = FALSE;

    if(strlen(cIPString))
	{
		// start winsock, requesting min version of 1.01
		WSAStartup(0x0101, &wsadata );
		
		//*******************************************************************
		// Now we ping the IP address resolved above
		//*******************************************************************
		
		// convert the IP char string to a IPAddr representation
		ipaddr = inet_addr((const char*)cIPString);
		
		if(ipaddr == INADDR_NONE)
		{
			AfxMessageBox(TEXT("Invalid Host or IP Entered"));
			WriteFileLog(1, _T("[PING] Invalid Host or IP Entered\r\n") );
		}
		else
		{
			INT		iPacketSize		= 0;
			
            // ping test을 위한 핸들 생성
			hIcmp = IcmpCreateFile();
			
			if(hIcmp == INVALID_HANDLE_VALUE)
			{
				AfxMessageBox(TEXT("Could not create ICMP handle"));
				WriteFileLog(1, _T("[PING] Could not create ICMP handle\r\n") );
			}
			else
			{
				LPVOID	lpData			= NULL;
				LPVOID	lpRevBuffer		= NULL;

				// determine the size of the data packet to send
				iPacketSize = 32;
				
				// allocate memory for the ping function call return and packet size
				lpData = LocalAlloc(LPTR, iPacketSize);
				lpRevBuffer = LocalAlloc(LPTR, sizeof(ICMP_ECHO_REPLY) * iPacketSize);
				
				if(lpData && lpRevBuffer)
				{
					// send the ping
					if( NULL == IcmpSendEcho( hIcmp,
                                              ipaddr,
                                              lpData,
                                              iPacketSize,
                                              NULL,
                                              lpRevBuffer,
                                              (sizeof(ICMP_ECHO_REPLY) * iPacketSize),
                                              1000))
					{					
                        // ping test 실패
                        bResult = FALSE;
						WriteFileLog(1, _T("[PING] IcmpSendEcho Failed(%d)\r\n"), GetLastError() );
					}
					else
					{
                        // ping test 성공
                        //&reply = (ICMP_ECHO_REPLY*)lpRevBuffer;
                        memcpy(&reply, lpRevBuffer, sizeof(ICMP_ECHO_REPLY));
                        bResult = TRUE;
					}
				}

				free(lpData);
				free(lpRevBuffer);
				IcmpCloseHandle(hIcmp);
			}					
		}
		WSACleanup();
	}
	else
	{
		WriteFileLog(1, _T("[PING] Invalid IP Address\r\n") );
	}

    return bResult;
}

void CWlanInfoFileLogDlg::OnSetfocusEditIpadress() 
{
	// TODO: Add your control notification handler code here
	SipShowIM(SIPF_ON);
}

void CWlanInfoFileLogDlg::OnKillfocusEditIpadress() 
{
	// TODO: Add your control notification handler code here
	SipShowIM(SIPF_OFF);
}

void CWlanInfoFileLogDlg::OnChangeEditIpadress() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	//SipShowIM(SIPF_OFF);	
}


void CWlanInfoFileLogDlg::GetCurrentFileVersion()
{
	DWORD infoSize = 0;
	CString msg;
	CString strVer = _T("");
	TCHAR tszFilePath[MAX_PATH] = {'\0',};

	char *pBuf = NULL;

	GetModuleFileName(NULL, tszFilePath, MAX_PATH);
	
	infoSize = GetFileVersionInfoSize(tszFilePath, 0);
	
	pBuf = new char[infoSize];
	
	if(GetFileVersionInfo(tszFilePath, 0, infoSize, pBuf) !=0 )
	{
		VS_FIXEDFILEINFO* pFineInfo = NULL;
		UINT bufLen = 0;
		if(VerQueryValue(pBuf, _T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
		{   
			WORD majorVer, minorVer, buildNum, revisionNum;
			majorVer = HIWORD(pFineInfo->dwFileVersionMS);
			minorVer = LOWORD(pFineInfo->dwFileVersionMS);
			buildNum = HIWORD(pFineInfo->dwFileVersionLS);
			revisionNum = LOWORD(pFineInfo->dwFileVersionLS);
			strVer.Format(_T("v%d.%d.%d.%d"), majorVer, minorVer, buildNum, revisionNum);
			SetDlgItemText(IDC_STATIC_FILEVER, strVer);
		}
	}
	delete pBuf;

}


void CWlanInfoFileLogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( nIDEvent == TIMER_WZC_NETWORK_REFRESH )
	{
		RefreshNetworkList();
	}


	CDialog::OnTimer(nIDEvent);
}
