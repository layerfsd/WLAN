//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/*
    Copyright (c) Microsoft Corporation. All rights reserved.
*/

#ifndef _connprv_h_
#define _connprv_h_
#include <windows.h>
#include <connmgr.h>
#include <Winsock2.h>

//$ALWAYS ON
#define WM_CLOSECONNREQ             (WM_USER)
#define WM_ALWAYSON_STATUSCHANGE    (WM_USER + 1)

// Returns:
// <0 if Obj1 < Obj2
//  0 if Obj1 == Obj2
// >0 if Obj1 > Obj2
typedef int (* LISTCLS_SORT_CALLBACK)(void *Obj1, void *Obj2);

typedef struct _LISTCLS_ENTRY
{
    struct _LISTCLS_ENTRY *pNext;
    struct _LISTCLS_ENTRY *pPrev;
    void * pObj;
} LISTCLS_ENTRY;

typedef LISTCLS_ENTRY *ListPos;

class IListCls
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;
    STDMETHOD_(void,Clear)() PURE;
    STDMETHOD_(void,Reset)(ListPos &Pos) PURE;
    STDMETHOD_(void*, Current)(ListPos Pos) PURE;
    STDMETHOD_(void*, First)(ListPos &Pos) PURE;
    STDMETHOD_(void*, Last)(ListPos &Pos) PURE;
    STDMETHOD_(void*, Next)(ListPos &Pos) PURE;
    STDMETHOD_(void*, Prev)(ListPos &Pos) PURE;
    STDMETHOD_(ULONG, Count)() PURE;
    STDMETHOD_(BOOL, Find)(void *pObj, ListPos &Pos) PURE;
    STDMETHOD (Remove)(ListPos &Pos) PURE;
    STDMETHOD_(BOOL, FindAndRemove)(void *pObj) PURE;
    STDMETHOD (AddAfter)(void * pObj, ListPos Pos) PURE;
    STDMETHOD (AddBefore)(void * pObj, ListPos Pos) PURE;
    STDMETHOD (AddFirst)(void * pObj) PURE;
    STDMETHOD (AddLast)(void * pObj) PURE;
    STDMETHOD (Swap)(ListPos Pos1, ListPos Pos2) PURE;
    STDMETHOD (Sort)(LISTCLS_SORT_CALLBACK pCallback) PURE;
    STDMETHOD (AddSorted)(void *pObj, LISTCLS_SORT_CALLBACK pCallback) PURE;
};


// GUIDs for standard enabling networks
//
// "My Isp" 
// {ADB0B001-10B5-3F39-27C6-9742E785FCD4}
DEFINE_GUID(IID_EnablingNetMyIsp,    0xadb0b001, 0x10b5, 0x3F39, 0x27, 0xC6, 0x97, 0x42, 0xE7, 0x85, 0xFC, 0xD4);

// "My Work Network"
// {18AD9FBD-F716-ACB6-FD8A-1965DB95B814}
DEFINE_GUID(IID_EnablingNetCorp,    0x18ad9fbd, 0xf716, 0xacb6, 0xfd, 0x8a, 0x19, 0x65, 0xdb, 0x95, 0xb8, 0x14);



#define CONNMGR_RESOURCE_PHONE         0x0001
#define CONNMGR_RESOURCE_NETROUTE      0x0002
#define CONNMGR_RESOURCE_SHARED_PHONE  0xFF00

#define CONNMGR_RESOURCETYPE_HWRESOURCE  0x01
#define CONNMGR_RESOURCETYPE_CONNECTION  0x02
#define CONNMGR_RESOURCETYPE_EXCLUSIVE   0x04
#define CONNMGR_RESOURCETYPE_ALL         0x07
#define CONNMGR_RESOURCETYPE_SHAREDPHONE_ANY    0x10
#define CONNMGR_RESOURCETYPE_SHAREDPHONE_ALL    0x20

#define CONNMGR_CONNECTION_CAN_END             0x00
#define CONNMGR_CONNECTION_SHOULD_NOT_TIMEOUT  0x01

class ConnPrv_IConnection;
class ConnPrv_IEnumConnections;
class ConnPrv_IProvider;
class ConnPrv_IExtension;

class ConnMgr;
class ConnMgr_IAbsConnection;
class ConnMgr_IConnectionRequest;
class ConnMgr_IConnection;
class ConnMgr_IEnumConnections;
class ConnMgr_IResourceSet;
class ConnPln_IPlanner;
class ConnMgr_IEnumConnectionPaths;
class ConnMgr_IConnectionPath;

typedef enum _CONNECTIONINFOTYPE
{
    ConnectionInfoCost = 1,
    ConnectionInfoBandwidth,
    ConnectionInfoLatency,
    ConnectionInfoPower,
    ConnectionInfoSignal,
    ConnectionInfoLastError,
    ConnectionInfoMeans,
    ConnectionInfoConnectionType,
    ConnectionInfoSSID,
    ConnectionInfoDetailedType,     // Requests one of CM_CONNTYPE_*.
    ConnectionInfoDetailedSubtype,  // Requests one of CM_CONNSUBTYPE_*.
    ConnectionInfoAdapter,          // Requests name of the adapter.
    ConnectionInfoConnectionFlags,  // Requests bitmask of CM_DSF_* flags.
    ConnectionInfoAccessString,     // Requests access string for connection (APN, dial number).
    ConnectionInfoDeviceName,       // Requests connection device name.
    ConnectionInfoDeviceType,       // Requests connection device type (as a string).
    ConnectionInfoWakeOnIncoming,   // Used to determine if the connection can wake up device on incoming traffic.
    ConnectionInfoSuspendResume,    // Used to determine if the connection can be suspend resume or not.
    ConnectionInfoTypeMax
} CONNECTIONINFOTYPE;

// The type of network connected to.
// It seems that requirements are typically expressed in terms of
// the type of network we are connected to, rather than in terms of
// more abstract properties of a network such as latency.
// 
// When a CSP handles more than one kind of network connection,
// just name them after the CSP concerned unless we need to
// expose more.  
typedef enum _CONNECTIONINFOCONNECTIONTYPE
{
    ConnectionInfoConnectionTypeUndefined = 1, 
    ConnectionInfoConnectionTypeCspNet, 
    ConnectionInfoConnectionTypeCspProxy, 
    ConnectionInfoConnectionTypeCspRas, 
    ConnectionInfoConnectionTypeCspWwan,
    ConnectionInfoConnectionTypeMax
} CONNECTIONINFOCONNECTIONTYPE;


// Calculates cost information about connection
typedef struct _CONNECTIONINFOCOST
{
    ULONG Duration; // In, seconds
    ULONG Bytes;    // In, bytes
    ULONG Cost;     // Out
} CONNECTIONINFOCOST;

// Calculates bandwidth information about connection
typedef struct _CONNECTIONINFOBANDWIDTH
{
    ULONG XMTBandwidth;     // Out, bytes/sec
    ULONG XMTBandwidthMul;  // Out, fraction from 0 - 0x10000
    ULONG RCVBandwidth;     // Out, bytes/sec
    ULONG RCVBandwidthMul;  // Out, fraction from 0 - 0x10000
} CONNECTIONINFOBANDWIDTH;

typedef struct _CONNECTIONINFOLATENCY
{
    ULONG ConnectLatency;   // Out, msec
    ULONG PacketLatency;    // Out, msec
} CONNECTIONINFOLATENCY;

typedef struct _CONNECTIONINFOPOWER
{
    ULONG IdlePower;        // Out, mW
    ULONG XmitPowerTime;    // Out, mWh/sec
    ULONG XmitPowerData;    // Out, mWh/byte
} CONNECTIONINFOPOWER;

typedef struct _CONNECTIONINFOSIGNAL
{
    ULONG SignalStrength;   // Out, dB above noise floor
    ULONG SignalVariance;   // Out, dB
    ULONG BitErrorRate;     // Out, %errors * 1000
} CONNECTIONINFOSIGNAL;

typedef struct _CONNECTIONINFOLASTERROR
{
    BOOL ClearError;        // In, if true, clear error after reporting
    TCHAR ComponentName[64];    // Name of component reporting the error
    ULONG Error;                // Error code
    TCHAR ErrorDescription[MAX_PATH];   // Optional description of error
} CONNECTIONINFOLASTERROR;


DEFINE_GUID(IID_ConnPrv_IConnection, 0x979d0424, 0x1c6f, 0x4126, 0xa5, 0x38, 0xbb, 0x87, 0x9, 0xf9, 0x99, 0x2c);
class ConnPrv_IConnection
{
private:
    CONNECTIONINFOCONNECTIONTYPE m_ConnectionType;
protected:
    ConnPrv_IConnection()
        {
            m_ConnectionType = ConnectionInfoConnectionTypeUndefined;
        }
    void SetConnectionType(CONNECTIONINFOCONNECTIONTYPE nc)
        {
            m_ConnectionType = nc;
        }
public:
    CONNECTIONINFOCONNECTIONTYPE GetConnectionType()
        {
            return m_ConnectionType;
        }
    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;

    STDMETHOD (SetCallback)(ConnMgr_IConnection *pICallback) PURE;
    STDMETHOD_(ConnMgr_IConnection *,GetCallback)() PURE;

    // Opens connection
    STDMETHOD (Connect)() PURE;

    // Closes connection
    STDMETHOD (Disconnect)() PURE;

    STDMETHOD (GetEntryName)(__out_ecount(cchSize) PTSTR szDesc, ULONG cchSize) PURE;
    STDMETHOD (GetDescription)(__out_ecount(cchSize) PTSTR szDesc, ULONG cchSize) PURE;
    STDMETHOD_(GUID *,GetSourceId)() PURE;
    STDMETHOD_(GUID *,GetDestId)  () PURE;
    STDMETHOD_(GUID *,GetConnectionId)() PURE;
    STDMETHOD_(DWORD,GetStatus)() PURE;
    STDMETHOD_(DWORD,GetFlags)() PURE;
    STDMETHOD_(DWORD,GetResources)() PURE;
    STDMETHOD_(BOOL,ChildIsExclusive)() PURE;
    STDMETHOD_(BOOL,IsAvailable)() PURE;
    STDMETHOD_(BOOL,IsRegisteredHome)() PURE;
    STDMETHOD_(BOOL,IsEnabled)() PURE;          // NOTE: Depricated. All configured connections are assumed to be enabled.
    STDMETHOD_(BOOL,IsAutoConnected)() PURE;
    STDMETHOD_(DWORD,ShouldEnd)() PURE;
    STDMETHOD_(DWORD,IsAlwaysOn)() PURE;
    STDMETHOD_(DWORD,GetSecureLevel)() PURE;
    STDMETHOD_(PSOCKADDR_STORAGE ,GetIPAddress)()PURE;

    // Returns all other info about the connection
    STDMETHOD (GetInfo)(CONNECTIONINFOTYPE InfoType, void *Buffer, ULONG cbSize) PURE;
};

class ConnPrv_IEnumConnections
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;

    STDMETHOD(Reset)() PURE;
    STDMETHOD_(ConnPrv_IConnection *,Next)() PURE;
    STDMETHOD_(ConnPrv_IEnumConnections *,Clone)() PURE;
};

class ConnPrv_IExtension : public IUnknown
{
public:
    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;

    STDMETHOD (Message)(ConnMgr_IConnectionRequest *pCR, DWORD dwMsg1, DWORD dwMsg2, PBYTE Params, ULONG ParamSize) PURE;
};

DEFINE_GUID(IID_ConnPrv_IProvider, 0xb86d549f, 0x9bf0, 0x44e0, 0x90, 0xac, 0xfa, 0x9f, 0xc9, 0x3e, 0xdf, 0x11);
class ConnPrv_IProvider : public IUnknown
{
public:
    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;

    STDMETHOD(Init)(ConnMgr *pIConnMgr) PURE;
    STDMETHOD(Deinit)() PURE;

    // Create a list of connections available on this provider
    STDMETHOD (EnumConnections)(GUID *pSource, GUID *pDest, DWORD Flags, ConnPrv_IEnumConnections **pIEnumProviderConnections) PURE;
};

class ConnMgr_IAbsConnection
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;
    STDMETHOD_(void,UpdateChildStatus)(DWORD Status) PURE;
    STDMETHOD_(DWORD,GetStatus)() PURE;

    STDMETHOD_(BOOL,IsChildExclusive)() PURE;
    STDMETHOD (GetConnectionRequests)(IListCls *pList) PURE;
};

class ConnMgr_IConnectionRequest : public ConnMgr_IAbsConnection
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;
    STDMETHOD_(void,UpdateChildStatus)(DWORD Status) PURE;
    STDMETHOD_(DWORD,GetStatus)() PURE;

    STDMETHOD_(BOOL,IsChildExclusive)() PURE;
    STDMETHOD (GetConnectionRequests)(IListCls *pList) PURE;

    //******ConnMgr_IConnectionRequest specific
    STDMETHOD_(void, UpdateClientStatus)(DWORD Status,  BOOL SignalAlways, BOOL SignalIfChanged) PURE;
    STDMETHOD_(DWORD, GetClientStatus)() PURE;
    STDMETHOD_(void, ClearClientSignal)() PURE;

    STDMETHOD_(void,SetStatus)(DWORD Status) PURE;

    STDMETHOD_(void,SetPlannerContext)(void *pContext) PURE;
    STDMETHOD_(void *,GetPlannerContext)() PURE;

    STDMETHOD_(CONNMGR_CONNECTIONINFO *,GetConnInfoPtr)() PURE;
    STDMETHOD (SetConnInfo)(CONNMGR_CONNECTIONINFO *pConnInfo) PURE;

    STDMETHOD_(GUID *,GetDestId)  () PURE;
    STDMETHOD_(void,SetDestId)(GUID *pDestinationNetworkId) PURE;

    STDMETHOD_(LONG,CacheEnable)() PURE;
    STDMETHOD_(void,SetCacheEnable)(LONG lCacheEnable) PURE;

    STDMETHOD_(DWORD,Priority)() PURE;
    STDMETHOD_(void,SetPriority)(DWORD Priority) PURE;

    STDMETHOD_(DWORD,Flags)() PURE;
    STDMETHOD_(void,SetFlags)(DWORD Flags) PURE;

    STDMETHOD_(HANDLE,EventHandle)() PURE;
    STDMETHOD_(ULONG,RefCount)() PURE;
    STDMETHOD_(BOOL,PathIsFixed)() PURE;

    STDMETHOD (SetConnectionPath)(ConnMgr_IConnectionPath *pConnectionPath, BOOL bFixed) PURE;
    STDMETHOD_(ConnMgr_IConnectionPath *,GetConnectionPath)() PURE;

    STDMETHOD(OpenConnection)() PURE;
    STDMETHOD(CloseConnection)() PURE;

    STDMETHOD_(ULONG, GetRefCount)(void) PURE;
    
    STDMETHOD_(void,SetTemporaryCacheDisable)(LONG lCacheEnable) PURE;
    STDMETHOD_(LONG,GetTemporaryCacheDisable)(void) PURE;
    STDMETHOD_(void,SetCallerPID)(DWORD dwCallerPID) PURE;
    STDMETHOD_(DWORD,GetCallerPID)() PURE;
};

class ConnMgr_IConnection : public ConnMgr_IAbsConnection
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;
    STDMETHOD_(void,UpdateChildStatus)(DWORD Status) PURE;
    STDMETHOD_(DWORD,GetStatus)() PURE;

    STDMETHOD_(BOOL,IsChildExclusive)() PURE;
    STDMETHOD (GetConnectionRequests)(IListCls *pList) PURE;

    //******ConnMgr_IConnection specific
    STDMETHOD_(GUID *,GetDestId)  () PURE;
    STDMETHOD_(GUID *,GetSourceId)() PURE;

    STDMETHOD (SetChildConnection)(ConnMgr_IConnection *pConnection) PURE;
    STDMETHOD_(ConnMgr_IConnection *,GetChildConnection)() PURE;

    STDMETHOD_(void, UpdateStatus)(DWORD Status) PURE;
    STDMETHOD (Connect)(ConnMgr_IAbsConnection *pParent) PURE;
    STDMETHOD (Disconnect)(ConnMgr_IAbsConnection *pParent) PURE;

    STDMETHOD_(SYSTEMTIME *,GetLastConnectTime)() PURE;

    // Get associated IConnection interface
    STDMETHOD_(ConnPrv_IConnection *,GetIConnection)() PURE;
    STDMETHOD_(BOOL,IsExclusive)() PURE;
    STDMETHOD_(BOOL,HigherPrioritiesUsingThis)() PURE;
    STDMETHOD_(DWORD,GetHighestRequestPriority)() PURE;
};

class ConnMgr_IEnumConnections
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;
    STDMETHOD(Reset)() PURE;
    STDMETHOD_(ConnMgr_IConnection *,Next)() PURE;
};

#define CONNMGR_CONNPATH_USES_ALWAYSON          0x01
#define CONNMGR_CONNPATH_USES_ETHERNET          0x02
#define CONNMGR_CONNPATH_USES_DTPT              0x04
#define CONNMGR_CONNPATH_USES_VPN               0x08
#define CONNMGR_CONNPATH_USES_VOICE_OR_ALIEN    0x10
#define CONNMGR_CONNPATH_USES_WAKE_ON_INCOMING  0x20
#define CONNMGR_CONNPATH_USES_CELLULAR          0x40
#define CONNMGR_CONNPATH_USES_SUSPEND_RESUME    0x80

class ConnMgr_IConnectionPath
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;

    STDMETHOD_(BOOL,ReserveResources)  (DWORD ResourceTypes, ConnMgr_IResourceSet *pIRS, ConnMgr_IConnectionRequest *pReq, ConnMgr_IConnection **ppOwnerConnection = NULL) PURE;
    //$ALWAYS ON
    STDMETHOD_(BOOL,UnreserveResources)(DWORD ResourceTypes, ConnMgr_IResourceSet *pIRS, ConnMgr_IConnectionRequest *pReq) PURE;
    STDMETHOD_(DWORD,ResourceTypesReserved)() PURE;
    //$ALWAYS ON
    STDMETHOD_(DWORD,GetResources)() PURE;

    STDMETHOD(Connect)(ConnMgr_IConnectionRequest *pReq) PURE;
    STDMETHOD(Disconnect)(ConnMgr_IConnectionRequest *pReq) PURE;
    STDMETHOD_(IListCls *,GetList)() PURE;
    STDMETHOD(Clone)(ConnMgr_IConnectionPath **ppIRS) PURE;
    STDMETHOD(CloneNonExclusive)(GUID * pGuidDest, ConnMgr_IConnectionPath **ppIRS) PURE;

    STDMETHOD_(DWORD,GetFlags)() PURE;
    STDMETHOD(GetInfoCost)(CONNECTIONINFOCOST *pCICost) PURE;
    STDMETHOD(GetInfoBandwidth)(CONNECTIONINFOBANDWIDTH *pCICost) PURE;
    STDMETHOD(GetInfoLatency)(CONNECTIONINFOLATENCY *pCICost) PURE;
    STDMETHOD(GetInfoLastError)(CONNECTIONINFOLASTERROR *pCIError) PURE;
    STDMETHOD(GetInfoIPAddress)(SOCKADDR_STORAGE *pCIError) PURE;    
    STDMETHOD(GetConnectionRequests)(IListCls *pIList) PURE;
    STDMETHOD_(BOOL,IsAvailable)() PURE;
    STDMETHOD_(BOOL,IsRegisteredHome)() PURE;
    STDMETHOD_(BOOL,IsEnabled)() PURE;          // NOTE: Depricated.
    STDMETHOD_(BOOL,IsAutoConnected)() PURE;
    STDMETHOD_(BOOL,IsLowBkgndOkToConnect)() PURE;
    STDMETHOD_(DWORD,ShouldEnd)() PURE;
    STDMETHOD_(BOOL,IsExclusive)() PURE;
    STDMETHOD_(DWORD,GetSecureLevel)() PURE;
    STDMETHOD_(DWORD,GetUsedConnectionsInfo)() PURE;
    STDMETHOD_(BOOL,IsConnected)(BOOL bAllConnectionsConnected) PURE;

    STDMETHOD_(void,Dump)(DWORD dwStatus) PURE;
};

class ConnMgr_IEnumConnectionPaths
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;
    STDMETHOD(Reset)() PURE;
    STDMETHOD_(ConnMgr_IConnectionPath *,Next)() PURE;
};

class ConnMgr_IResourceSet
{
public:
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;
    STDMETHOD_(BOOL,IsResourceReserved)(DWORD ResSet, ConnMgr_IConnection *pConnection, ConnMgr_IConnection **ppOwnerConnection = NULL) PURE;
    STDMETHOD_(BOOL,IsSharedPhoneResourceReserved)(DWORD ResourceTypes, DWORD ResSet, ConnMgr_IConnection *pConnection, ConnMgr_IConnection **ppOwnerConnection = NULL) PURE;
    STDMETHOD_(BOOL,IsReserved)(DWORD ResourceTypes, DWORD ResSet, ConnMgr_IConnection *pConnection, ConnMgr_IAbsConnection *pOwner, ConnMgr_IConnection *pChild, ConnMgr_IConnection **ppOwnerConnection = NULL) PURE;
    STDMETHOD_(BOOL,Reserve)   (DWORD ResourceTypes, DWORD ResSet, ConnMgr_IConnection *pConnection, ConnMgr_IAbsConnection *pOwner, ConnMgr_IConnection *pChild, ConnMgr_IConnection **ppOwnerConnection = NULL) PURE;
    //$ALWAYS ON
    STDMETHOD_(BOOL,Unreserve) (DWORD ResourceTypes, DWORD ResSet, ConnMgr_IConnection *pConnection, ConnMgr_IAbsConnection *pOwner, ConnMgr_IConnection *pChild) PURE;
    STDMETHOD (Clear)() PURE;
    STDMETHOD(Clone)(ConnMgr_IResourceSet **ppIRS) PURE;
    STDMETHOD(UpdateResources)(DWORD dwContexts) PURE;
};

struct _STC_INFO;
typedef void (*PSAFETHREADFN)(struct _STC_INFO *);
typedef struct _STC_INFO
{
    DWORD dwSize;
    PSAFETHREADFN pFn;
} STC_INFO;

class ConnMgr
{
public:
    STDMETHOD_(void,Lock)() PURE;
    STDMETHOD_(void,Unlock)() PURE;
    STDMETHOD_(IListCls *,GetProviderList)() PURE;
    STDMETHOD_(IListCls *,GetRequestList)() PURE;
    STDMETHOD_(IListCls *,GetConnectionList)() PURE;

    STDMETHOD (CreateIEnumConnections)(GUID *pSource, GUID *pDest, DWORD Flags, ConnMgr_IEnumConnections **ppIEC) PURE;

    STDMETHOD (CreateConnectionRequest)(CONNMGR_CONNECTIONINFO *pConnInfo,
                                        ConnMgr_IConnectionPath *pIConnectionPath,
                                        ConnMgr_IConnectionRequest **ppCR) PURE;

    STDMETHOD (CreateNewConnectionRequest)(ConnMgr_IConnectionRequest **ppCR) PURE;
    STDMETHOD (ReleaseConnectionRequest)(ConnMgr_IConnectionRequest *pCR, LONG lCacheEnable) PURE;

    STDMETHOD (CreateConnection)(ConnPrv_IConnection *pIConnection,
                                 ConnMgr_IConnection **ppC ) PURE;

    STDMETHOD (CreateListCls)(IListCls **ppIListCls) PURE;
    STDMETHOD (CreateIEnumConnectionPaths)(GUID *pDest, DWORD Flags,
                                          ConnMgr_IEnumConnectionPaths **ppIECP) PURE;

    STDMETHOD (CreateConnectionPath)(ConnMgr_IConnectionPath **ppICP) PURE;

    STDMETHOD (CreateResourceSet)(ConnMgr_IResourceSet **ppIRS) PURE;
    STDMETHOD_(ConnMgr_IResourceSet *,GetAllocatedResourceSet)() PURE;

    STDMETHOD (RunInSafeThread)(STC_INFO *pSTCInfo, BOOL FilterDups, BOOL OkToRunNow) PURE;
    STDMETHOD_(ConnPln_IPlanner *,GetPlanner)() PURE;

    STDMETHOD (ProviderMessage)(ConnMgr_IConnectionRequest *pCR, GUID *ProviderGuid, DWORD *pdwIndex, DWORD dwMsg1, DWORD dwMsg2, PBYTE Params, ULONG ParamSize) PURE;
    STDMETHOD (EnumDestinations)(int i, CONNMGR_DESTINATION_INFO *pDestInfo) PURE;
    STDMETHOD_(HWND,CreateWorkerWindow)(WNDPROC pfnWndProc, void * p) PURE;
    //$ALWAYS ON
    STDMETHOD_(HWND,GetWorkerWindow)() PURE;

    STDMETHOD (Reevaluate)() PURE;
    STDMETHOD (RegisterScheduledConnection)(SCHEDULEDCONNECTIONINFO *pSCI) PURE;
    STDMETHOD (UnregisterScheduledConnection)(LPCWSTR Token) PURE;
    STDMETHOD (NotifyDestinationAvailable)(const GUID *pDestId, const DWORD dwConnectionPriority) PURE;
    STDMETHOD_(DWORD,GetNetworkSecureLevel)(GUID *pGUID) PURE;

    STDMETHOD_(HRESULT,RegisterForStatusChangeNotification)(BOOL fEnable, HWND hWnd) PURE;
    STDMETHOD (StatusChangeNotify)(DWORD dwStatus) PURE;

    STDMETHOD_(BOOL,GetNetworkSimultaneousVoiceDataSupport)() PURE;
};

DEFINE_GUID(IID_ConnPln_IPlanner, 0x1c2919ee, 0x2e91, 0x40b3, 0xa1, 0xfa, 0xf0, 0x5b, 0xfe, 0x0, 0x29, 0x85);
class ConnPln_IPlanner : public IUnknown
{
public:
    STDMETHOD(GetDestMetaNetwork)(GUID *pguidEnabling, GUID *pguidMetaNetwork) PURE;

    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv) PURE;
    STDMETHOD_(ULONG, AddRef)(void) PURE;
    STDMETHOD_(ULONG, Release)(void) PURE;

    STDMETHOD(Init)(ConnMgr *pIConnMgr) PURE;
    STDMETHOD(Deinit)() PURE;

    STDMETHOD(CreateConnectionRequest)( CONNMGR_CONNECTIONINFO *pConnInfo,
                                        ConnMgr_IConnectionPath *pIConnectionPath,
                                        ConnMgr_IConnectionRequest **ppCR)  PURE;
    STDMETHOD(ReleaseConnectionRequest)(ConnMgr_IConnectionRequest *pRequest, LONG lCacheEnable) PURE;
    STDMETHOD(DeleteConnectionRequest)(ConnMgr_IConnectionRequest *pRequest) PURE;
    STDMETHOD(UpdateConnectionRequest)(ConnMgr_IConnectionRequest *pRequest, DWORD Status) PURE;
    STDMETHOD(ChangePriority)(ConnMgr_IConnectionRequest *pCR, DWORD dwNewPriority) PURE;
    STDMETHOD (Reevaluate)() PURE;
    //$ALWAYS ON
    STDMETHOD (HandleAlwaysOnStatusChange)(LPARAM lParam) PURE;
    STDMETHOD (HandleAlwaysOnTimer)() PURE;
    
};

#endif //_connprv_h_


