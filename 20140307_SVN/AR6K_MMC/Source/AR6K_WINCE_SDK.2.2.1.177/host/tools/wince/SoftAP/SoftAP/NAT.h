#ifndef __NAT__
#define __NAT__

extern BOOL EnableDHCP(BOOL bEnable);
extern BOOL EnableIPRouting(BOOL bEnable);
extern BOOL RebindAdapter(TCHAR *pszAdapterName);

#if 0
extern DWORD EnableNAT(HANDLE NATHandler);//Replaced by BOOL EnableDHCP(BOOL bEnable);
extern DWORD DisableNAT(HANDLE NATHandler);//Replaced by BOOL EnableDHCP(BOOL bEnable);
extern DWORD ActDev(HANDLE *DevHandler);//Replaced by BOOL EnableDHCP(BOOL bEnable);
extern DWORD OpenNATDrv(HANDLE *NATHandler);//Replaced by BOOL EnableDHCP(BOOL bEnable);
extern void CloseNAT(HANDLE DevHandler,HANDLE NATHandler);//Replaced by BOOL EnableDHCP(BOOL bEnable);
extern DWORD OpenIPDriver(HANDLE *IPDHandler);//Replaced by BOOL EnableIPRouting(BOOL bEnable);
extern DWORD DisableIPRouting(HANDLE IPDHandler);//Replaced by BOOL EnableIPRouting(BOOL bEnable);
extern DWORD EnableIPRouting(HANDLE IPDHandler);//Replaced by BOOL EnableIPRouting(BOOL bEnable);
extern DWORD AtheroWiFiCheck();
extern DWORD RebindWiFiAdapter();//Replaced by BOOL RebindAdapter(TCHAR *pszAdapterName);
extern DWORD RebindGPRSIP();//Replaced by BOOL RebindAdapter(TCHAR *pszAdapterName);
#endif
#endif


