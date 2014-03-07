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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

mwindbas.h

Abstract:

private portion of windbase.h

Notes: 


--*/
   
#ifndef _MACRO_WINDBASE_H_
#define _MACRO_WINDBASE_H_

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#ifndef WINCEMACRO
#error WINCEMACRO not defined __FILE__
#endif

#ifdef WIN32_FS_CALL
// SDK functions
#define CeOidGetInfoEx2       	+++ Do Not Use +++
#define CeOidGetInfoEx2_Trap       WIN32_FS_CALL(BOOL,   12, (PCEGUID pguid, CEOID oid, CEOIDINFOEX *oidInfo))

#define CeFindFirstDatabaseEx	+++ Do Not Use +++
#define CeFindFirstDatabaseEx_Trap WIN32_FS_CALL(HANDLE, 13, (PCEGUID pguid, DWORD dwClassID))

#define CeCreateDatabaseEx2         +++ Do Not Use +++
#define CeCreateDatabaseEx2_Trap   WIN32_FS_CALL(CEOID,  14, (PCEGUID pguid, CEDBASEINFOEX *pInfo))

#define CeSetDatabaseInfoEx2         +++ Do Not Use +++
#define CeSetDatabaseInfoEx2_Trap  WIN32_FS_CALL(BOOL,   15, (PCEGUID pguid, CEOID oidDbase, CEDBASEINFOEX *pNewInfo))

#define CeOpenDatabaseEx2         +++ Do Not Use +++
#define CeOpenDatabaseEx2_Trap     WIN32_FS_CALL(HANDLE, 16, (PCEGUID pguid, PCEOID poid, LPWSTR lpszName, SORTORDERSPECEX* pSort, DWORD dwFlags, CENOTIFYREQUEST *pRequest))

#define CeDeleteDatabaseEx         +++ Do Not Use +++
#define CeDeleteDatabaseEx_Trap    WIN32_FS_CALL(BOOL,   28, (PCEGUID pguid, CEOID oid))

#define CeMountDBVol               +++ Do Not Use +++
#define CeMountDBVol_Trap          WIN32_FS_CALL(BOOL,   58, (PCEGUID pguid, LPWSTR lpszVol, DWORD dwFlags))

#define CeUnmountDBVol             +++ Do Not Use +++
#define CeUnmountDBVol_Trap        WIN32_FS_CALL(BOOL,   60, (PCEGUID pguid))

#define CeFlushDBVol               +++ Do Not Use +++
#define CeFlushDBVol_Trap          WIN32_FS_CALL(BOOL,   61, (PCEGUID pguid))

#define CeEnumDBVolumes            +++ Do Not Use +++
#define CeEnumDBVolumes_Trap       WIN32_FS_CALL(BOOL,   59, (PCEGUID pguid, LPWSTR lpBuf, DWORD dwSize))

#define GetStoreInformation         +++ Do Not Use +++
#define GetStoreInformation_Trap   WIN32_FS_CALL(BOOL,   32, (LPSTORE_INFORMATION lpsi))

#define CeFreeNotification         +++ Do Not Use +++
#define CeFreeNotification_Trap    WIN32_FS_CALL(BOOL,   62, (PCENOTIFYREQUEST pRequest, PCENOTIFICATION pNotify))

#define CeCreateDatabaseWithProps      +++ Do Not Use +++
#define CeCreateDatabaseWithProps_Trap WIN32_FS_CALL(CEOID,  122, (PCEGUID pGuid, CEDBASEINFOEX* pInfo, DWORD cProps, VOID* prgProps))

#define CeAddDatabaseProps         +++ Do Not Use +++
#define CeAddDatabaseProps_Trap    WIN32_FS_CALL(BOOL,   123, (PCEGUID pGuid, CEOID oidDb, DWORD cProps, VOID* prgProps))

#define CeRemoveDatabaseProps      +++ Do Not Use +++
#define CeRemoveDatabaseProps_Trap WIN32_FS_CALL(BOOL,   124, (PCEGUID pGuid, CEOID oidDb, DWORD cPropID, CEPROPID* prgPropID))

#define CeMountDBVolEx             +++ Do Not Use +++
#define CeMountDBVolEx_Trap        WIN32_FS_CALL(BOOL,   125, (PCEGUID pGuid, LPWSTR pwszDBVol, VOID* pOptions, DWORD dwFlags))

#define CeCreateSession            +++ Do Not Use +++
#define CeCreateSession_Trap       WIN32_FS_CALL(HANDLE, 126, (CEGUID* pGuid))

#define CeAddSyncPartner           +++ Do Not Use +++
#define CeAddSyncPartner_Trap      WIN32_FS_CALL(BOOL,   127, (PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, LPCWSTR pwszFriendlyName, LPCWSTR pwszFullName))

#define CeRemoveSyncPartner        +++ Do Not Use +++
#define CeRemoveSyncPartner_Trap   WIN32_FS_CALL(BOOL,   128, (PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid))

#define CeTrackDatabase            +++ Do Not Use +++
#define CeTrackDatabase_Trap       WIN32_FS_CALL(BOOL,   129, (PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, CEOID oidDB, DWORD dwTrackingFlags))

#define CeRemoveDatabaseTracking      +++ Do Not Use +++
#define CeRemoveDatabaseTracking_Trap WIN32_FS_CALL(BOOL,   130, (PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, CEOID oidDB))

#define CeTrackProperty            +++ Do Not Use +++
#define CeTrackProperty_Trap       WIN32_FS_CALL(BOOL,   131, (PCEGUID pVolGuid, CEOID oidDB, CEPROPID propid, BOOL fAddToScheme))

#define CePurgeTrackingData        +++ Do Not Use +++
#define CePurgeTrackingData_Trap   WIN32_FS_CALL(BOOL,   132, (PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, SYSTEMTIME* pstThreshold))

#define CePurgeTrackingGenerations      +++ Do Not Use +++
#define CePurgeTrackingGenerations_Trap WIN32_FS_CALL(BOOL,   133, (PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, ULONGLONG genThreshold))

#define CeFindNextDatabaseEx         +++ Do Not Use +++
#define CeFindNextDatabaseEx_Trap    WIN32_DBFIND_CALL(CEOID, 2, (HANDLE hEnum, PCEGUID pguid))

#define CeSeekDatabaseEx         +++ Do Not Use +++
#define CeSeekDatabaseEx_Trap           WIN32_DBFILE_CALL(CEOID, 2, (HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue, WORD wNumVals, LPDWORD lpdwIndex))

#define CeDeleteRecord           +++ Do Not Use +++
#define CeDeleteRecord_Trap             WIN32_DBFILE_CALL(BOOL,  3, (HANDLE hDatabase, CEOID oidRecord))

#define CeReadRecordPropsEx         +++ Do Not Use +++
#define CeReadRecordPropsEx_Trap        WIN32_DBFILE_CALL(CEOID, 4, (HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID, CEPROPID *rgPropID, LPBYTE *lplpBuffer, LPDWORD lpcbBuffer, HANDLE hHeap))

#define CeWriteRecordProps          +++ Do Not Use +++
#define CeWriteRecordProps_Trap         WIN32_DBFILE_CALL(CEOID, 5, (HANDLE hDbase, CEOID oidRecord, WORD cPropID, CEPROPVAL *rgPropVal))

#define CeGetDBInformationByHandle         +++ Do Not Use +++
#define CeGetDBInformationByHandle_Trap WIN32_DBFILE_CALL(BOOL,  6, (HANDLE hDatabase, LPBY_HANDLE_DB_INFORMATION lpDBInfo))

#define CeGetDatabaseProps          +++ Do Not Use +++
#define CeGetDatabaseProps_Trap     WIN32_DBFILE_CALL(BOOL,  7, (HANDLE hHandle, WORD* pcPropId, CEPROPID* prgPropId, CEPROPSPEC* prgProps))

#define CeOpenDatabaseInSession      +++ Do Not Use +++
#define CeOpenDatabaseInSession_Trap WIN32_DBFILE_CALL(HANDLE, 8, (HANDLE hSession, PCEGUID pGuid, PCEOID poid, LPWSTR pwszName, SORTORDERSPECEX* pSort, DWORD dwFlags, CENOTIFYREQUEST* pRequest))

#define CeSetSessionOption          +++ Do Not Use +++
#define CeSetSessionOption_Trap     WIN32_DBFILE_CALL(BOOL, 9, (HANDLE hSession, ULONG ulOptionId, DWORD dwValue))

#define CeGetDatabaseSession        +++ Do Not Use +++
#define CeGetDatabaseSession_Trap   WIN32_DBFILE_CALL(HANDLE, 10, (HANDLE hDatabase))

#define CeBeginTransaction          +++ Do Not Use +++
#define CeBeginTransaction_Trap     WIN32_DBFILE_CALL(BOOL, 11, (HANDLE hSession, int isoLevel))

#define CeEndTransaction            +++ Do Not Use +++
#define CeEndTransaction_Trap       WIN32_DBFILE_CALL(BOOL, 12, (HANDLE hSession, BOOL fCommit))

#define CeOpenStream                +++ Do Not Use +++
#define CeOpenStream_Trap           WIN32_DBFILE_CALL(HANDLE, 13, (HANDLE hDatabase, CEPROPID propid, DWORD dwMode))

#define CeStreamRead                +++ Do Not Use +++
#define CeStreamRead_Trap           WIN32_DBFILE_CALL(BOOL, 14, (HANDLE hStream, BYTE* prgbBuffer, const DWORD cbRead, DWORD* pcbRead))

#define CeStreamSaveChanges         +++ Do Not Use +++
#define CeStreamSaveChanges_Trap    WIN32_DBFILE_CALL(BOOL, 15, (HANDLE hStream))

#define CeStreamSeek                +++ Do Not Use +++
#define CeStreamSeek_Trap           WIN32_DBFILE_CALL(BOOL, 16, (HANDLE hStream, DWORD cbMove, DWORD dwOrigin, DWORD* pcbNewOffset))

#define CeStreamSetSize             +++ Do Not Use +++
#define CeStreamSetSize_Trap        WIN32_DBFILE_CALL(BOOL, 17, (HANDLE hStream, const DWORD cbSize))

#define CeStreamWrite               +++ Do Not Use +++
#define CeStreamWrite_Trap          WIN32_DBFILE_CALL(BOOL, 18, (HANDLE hStream, BYTE* prgbBuffer, DWORD cbWrite, DWORD* pcbWritten))

#define CeBeginSyncSession          +++ Do Notb Use +++
#define CeBeginSyncSession_Trap     WIN32_DBFILE_CALL(BOOL, 19, (HANDLE hSession, PCEGUID pSyncPartnerGuid, ULONGLONG genFrom, ULONGLONG genTo, DWORD dwFlags, ULONGLONG* pGenCur))

#define CeEndSyncSession            +++ Do Not Use +++
#define CeEndSyncSession_Trap       WIN32_DBFILE_CALL(BOOL, 20, (HANDLE hSession, DWORD dwOutcome))

#define CeGetChangedRecords         +++ Do Not Use +++
#define CeGetChangedRecords_Trap    WIN32_DBFILE_CALL(HANDLE, 21, (HANDLE hSession, CEOID oidDB, DWORD dwChangeType))

#define CeGetChangedRecordCnt       +++ Do Not Use +++
#define CeGetChangedRecordCnt_Trap  WIN32_DBFILE_CALL(BOOL, 22, (HANDLE hSession, CEOID oidDB, DWORD* pdwCnt))

#define CeFindNextChangedRecord      +++ Do Not Use +++
#define CeFindNextChangedRecord_Trap WIN32_DBFILE_CALL(CEOID, 23, (HANDLE hChangeEnum))

#define CeGetPropChangeInfo         +++ Do Not Use +++
#define CeGetPropChangeInfo_Trap    WIN32_DBFILE_CALL(BOOL, 24, (HANDLE hChangeEnum, CEOID oidChangedRecord, CEPROPID propid, BOOL* pfPropChanged))

#define CeGetRecordChangeInfo       +++ Do Not Use +++
#define CeGetRecordChangeInfo_Trap  WIN32_DBFILE_CALL(BOOL, 25, (HANDLE hChangeEnum, CEOID oidChangedRecord, VOID* pInfo))

#define CeMarkRecord                +++ Do Not Use +++
#define CeMarkRecord_Trap           WIN32_DBFILE_CALL(BOOL, 26, (HANDLE hChangeEnum, CEOID oidChangedRecord, BOOL fChanged))

#define CeAttachCustomTrackingData      +++ Do Not Use +++
#define CeAttachCustomTrackingData_Trap WIN32_DBFILE_CALL(BOOL, 27, (HANDLE hDB, CEOID oidChangedRecord, BYTE* rgbData, DWORD ccb))

#define CeGetCustomTrackingData      +++ Do Not Use +++
#define CeGetCustomTrackingData_Trap WIN32_DBFILE_CALL(BOOL, 28, (HANDLE hDB, CEOID oidChangedRecord, BYTE* rgbData, DWORD* pccb))

// OAK functions  
#define CeGetReplChangeMask         +++ Do Not Use +++
#define CeGetReplChangeMask_Trap     WIN32_FS_CALL(BOOL, 33, (LPDWORD lpmask))

#define CeSetReplChangeMask         +++ Do Not Use +++
#define CeSetReplChangeMask_Trap     WIN32_FS_CALL(BOOL, 34, (DWORD mask))

#define CeGetReplChangeBitsEx         +++ Do Not Use +++
#define CeGetReplChangeBitsEx_Trap   WIN32_FS_CALL(BOOL, 35, (PCEGUID pguid, CEOID oid, LPDWORD lpbits, DWORD dwFlags))

#define CeSetReplChangeBitsEx         +++ Do Not Use +++
#define CeSetReplChangeBitsEx_Trap   WIN32_FS_CALL(BOOL, 51, (PCEGUID pguid, CEOID oid, DWORD mask))

#define CeClearReplChangeBitsEx         +++ Do Not Use +++
#define CeClearReplChangeBitsEx_Trap WIN32_FS_CALL(BOOL, 36, (PCEGUID pguid, CEOID oid, DWORD mask))

#define CeGetReplOtherBitsEx            +++ Do Not Use +++
#define CeGetReplOtherBitsEx_Trap    WIN32_FS_CALL(BOOL, 37, (PCEGUID pguid, CEOID oid, LPDWORD lpbits))

#define CeSetReplOtherBitsEx            +++ Do Not Use +++
#define CeSetReplOtherBitsEx_Trap    WIN32_FS_CALL(BOOL, 38, (PCEGUID pguid, CEOID oid, DWORD bits))

#define ReplOpenSync                    +++ Do Not Use +++
#define ReplOpenSync_Trap            WIN32_FS_CALL(BOOL, 72, (CEGUID* pguid, LPBYTE lpSyncState, DWORD dwSize, DWORD dwSyncFlags, HREPL* phRepl))

#define ReplCheckpoint                  +++ Do Not Use +++
#define ReplCheckpoint_Trap          WIN32_FS_CALL(BOOL, 73, (HREPL hRepl, HANDLE hChangeEvent))

#define ReplCloseSync                   +++ Do Not Use +++
#define ReplCloseSync_Trap           WIN32_FS_CALL(BOOL, 74, (HREPL hRepl))

#define ReplGetSyncState                +++ Do Not Use +++
#define ReplGetSyncState_Trap        WIN32_FS_CALL(BOOL, 75, (HREPL hRepl, LPBYTE lpSyncState, DWORD* lpdwSize, BOOL fFailIfUnchanged))

#define ReplChangeSyncSettings          +++ Do Not Use +++
#define ReplChangeSyncSettings_Trap  WIN32_FS_CALL(BOOL, 76, (HREPL hRepl, DWORD dwSyncFlags, DWORD* pOldFlags))

#define ReplFindNextChange              +++ Do Not Use +++
#define ReplFindNextChange_Trap      WIN32_FS_CALL(BOOL, 77, (HREPL hRepl, REPLCHANGEINFO* pReplInfo, BOOL fRestart))

#define ReplGetOidStatus                +++ Do Not Use +++
#define ReplGetOidStatus_Trap        WIN32_FS_CALL(BOOL, 78, (HREPL hRepl, CEOID oid, REPLCHANGEINFO* pReplInfo))

#define CeChangeDatabaseLCID            +++ Do Not Use +++
#define CeChangeDatabaseLCID_Trap WIN32_FS_CALL(VOID, 56, (PCEGUID pguid, DWORD LCID))

#define CeRegisterFileSystemNotification         +++ Do Not Use +++
#define CeRegisterFileSystemNotification_Trap WIN32_FS_CALL(BOOL, 10, (HWND hWnd))

#define CeRegisterReplNotification               +++ Do Not Use +++
#define CeRegisterReplNotification_Trap WIN32_FS_CALL(BOOL, 11, (CENOTIFYREQUEST *pRequest))

#define RegisterAFSEx                            +++ Do Not Use +++
#define RegisterAFSEx_Trap WIN32_FS_CALL (BOOL, 45, (int index, HANDLE h, DWORD dw, DWORD version, DWORD flags))

#define DeregisterAFS                            +++ Do Not Use +++
#define DeregisterAFS_Trap WIN32_FS_CALL (BOOL, 46, (int index))

#define GetSystemMemoryDivision                  +++ Do Not Use +++
#define GetSystemMemoryDivision_Trap WIN32_FS_CALL(BOOL, 39, (LPDWORD lpdwStorePages, LPDWORD lpdwRamPages, LPDWORD lpdwPageSize))

#define SetSystemMemoryDivision                  +++ Do Not Use +++
#define SetSystemMemoryDivision_Trap WIN32_FS_CALL(DWORD, 40, (DWORD dwStorePages))

#define RegisterAFSName                          +++ Do Not Use +++
#define RegisterAFSName_Trap WIN32_FS_CALL(int,52,(LPCWSTR pName))

#define DeregisterAFSName                        +++ Do Not Use +++
#define DeregisterAFSName_Trap WIN32_FS_CALL(BOOL,53,(int index))

#define DumpFileSystemHeap                       +++ Do Not Use +++
#define DumpFileSystemHeap_Trap                  +++ Do Not Use +++

#elif defined(COREDLL)

BOOL xxx_CeGetDBInformationByHandle();
#define CeGetDBInformationByHandle xxx_CeGetDBInformationByHandle
BOOL xxx_CeOidGetInfoEx2 (PCEGUID pguid, CEOID oid, CEOIDINFOEX *oidInfo);
#define CeOidGetInfoEx2 xxx_CeOidGetInfoEx2
HANDLE xxx_CeFindFirstDatabaseEx (PCEGUID pguid, DWORD dwClassID);
#define CeFindFirstDatabaseEx xxx_CeFindFirstDatabaseEx
CEOID xxx_CeCreateDatabaseEx2 (PCEGUID pguid, CEDBASEINFOEX *pInfo);
#define CeCreateDatabaseEx2 xxx_CeCreateDatabaseEx2
BOOL xxx_CeSetDatabaseInfoEx2 (PCEGUID pguid, CEOID oidDbase, CEDBASEINFOEX *pNewInfo);
#define CeSetDatabaseInfoEx2 xxx_CeSetDatabaseInfoEx2
HANDLE xxx_CeOpenDatabaseEx2 (PCEGUID pguid, PCEOID poid, LPWSTR lpszName, SORTORDERSPECEX* pSort, DWORD dwFlags, CENOTIFYREQUEST *pRequest);
#define CeOpenDatabaseEx2 xxx_CeOpenDatabaseEx2
BOOL xxx_CeDeleteDatabaseEx (PCEGUID pguid, CEOID oid);
#define CeDeleteDatabaseEx xxx_CeDeleteDatabaseEx
BOOL xxx_CeMountDBVol (PCEGUID pguid, LPWSTR lpszVol, DWORD dwFlags);
#define CeMountDBVol xxx_CeMountDBVol
BOOL xxx_CeUnmountDBVol (PCEGUID pguid);
#define CeUnmountDBVol xxx_CeUnmountDBVol
BOOL xxx_CeFlushDBVol (PCEGUID pguid);
#define CeFlushDBVol xxx_CeFlushDBVol
BOOL xxx_CeEnumDBVolumes (PCEGUID pguid, LPWSTR lpBuf, DWORD dwSize);
#define CeEnumDBVolumes xxx_CeEnumDBVolumes
BOOL xxx_GetStoreInformation (LPSTORE_INFORMATION lpsi);
#define GetStoreInformation xxx_GetStoreInformation
BOOL xxx_CeFreeNotification (PCENOTIFYREQUEST pRequest, PCENOTIFICATION pNotify);
#define CeFreeNotification xxx_CeFreeNotification
CEOID xxx_CeFindNextDatabaseEx(HANDLE hEnum, PCEGUID pguid);
#define CeFindNextDatabaseEx xxx_CeFindNextDatabaseEx
CEOID xxx_CeSeekDatabaseEx(HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue, WORD wNumVals, LPDWORD lpdwIndex);
#define CeSeekDatabaseEx xxx_CeSeekDatabaseEx
BOOL xxx_CeDeleteRecord(HANDLE hDatabase, CEOID oidRecord);
#define CeDeleteRecord xxx_CeDeleteRecord
CEOID xxx_CeReadRecordPropsEx(HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID, CEPROPID *rgPropID, LPBYTE *lplpBuffer, LPDWORD lpcbBuffer, HANDLE hHeap);
#define CeReadRecordPropsEx xxx_CeReadRecordPropsEx
CEOID xxx_CeWriteRecordProps(HANDLE hDbase, CEOID oidRecord, WORD cPropID, CEPROPVAL *rgPropVal);
#define CeWriteRecordProps xxx_CeWriteRecordProps
CEOID xxx_CeReadRecordProps(HANDLE hDbase, DWORD dwFlags, LPWORD lpcPropID, CEPROPID* rgPropID, LPBYTE* lplpBuffer, LPDWORD lpcbBuffer);
#define CeReadRecordProps xxx_CeReadRecordProps
CEOID xxx_CeSeekDatabase(HANDLE hDatabase, DWORD dwSeekType, DWORD dwValue, LPDWORD lpdwIndex);
#define CeSeekDatabase xxx_CeSeekDatabase
CEOID xxx_CeCreateDatabaseWithProps(PCEGUID pGuid, CEDBASEINFOEX* pInfo, DWORD cProps, VOID* prgProps);
#define CeCreateDatabaseWithProps xxx_CeCreateDatabaseWithProps
BOOL xxx_CeAddDatabaseProps(PCEGUID pGuid, CEOID oidDb, DWORD cProps, VOID* prgProps);
#define CeAddDatabaseProps xxx_CeAddDatabaseProps
BOOL xxx_CeRemoveDatabaseProps(PCEGUID pGuid, CEOID oidDb, DWORD cPropID, CEPROPID* prgPropID);
#define CeRemoveDatabaseProps xxx_CeRemoveDatabaseProps
BOOL xxx_CeMountDBVolEx(PCEGUID pGuid, LPWSTR pwszDBVol, VOID* pOptions, DWORD dwFlags);
#define CeMountDBVolEx xxx_CeMountDBVolEx
HANDLE xxx_CeCreateSession(CEGUID* pGuid);
#define CeCreateSession xxx_CeCreateSession
BOOL xxx_CeAddSyncPartner(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, LPCWSTR pwszFriendlyName, LPCWSTR pwszFullName);
#define CeAddSyncPartner xxx_CeAddSyncPartner
BOOL xxx_CeRemoveSyncPartner(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid);
#define CeRemoveSyncPartner xxx_CeRemoveSyncPartner
BOOL xxx_CeTrackDatabase(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, CEOID oidDB, DWORD  dwTrackingFlags);
#define CeTrackDatabase xxx_CeTrackDatabase
BOOL xxx_CeRemoveDatabaseTracking(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, CEOID oidDB);
#define CeRemoveDatabaseTracking xxx_CeRemoveDatabaseTracking
BOOL xxx_CeTrackProperty(PCEGUID pVolGuid, CEOID oidDB, CEPROPID propid, BOOL fAddToScheme);
#define CeTrackProperty xxx_CeTrackProperty
BOOL xxx_CePurgeTrackingData(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, SYSTEMTIME* pstThreshold);
#define CePurgeTrackingData xxx_CePurgeTrackingData
BOOL xxx_CePurgeTrackingGenerations(PCEGUID pVolGuid, PCEGUID pSyncPartnerGuid, ULONGLONG genThreshold);
#define CePurgeTrackingGenerations xxx_CePurgeTrackingGenerations
BOOL xxx_CeGetDatabaseProps(HANDLE hHandle, WORD* pcPropId, CEPROPID* prgPropId, VOID* prgProps);
#define CeGetDatabaseProps xxx_CeGetDatabaseProps
BOOL xxx_CeSetSessionOption(HANDLE hSession, ULONG ulOptionId, DWORD dwValue);
#define CeSetSessionOption xxx_CeSetSessionOption
HANDLE xxx_CeGetDatabaseSession(HANDLE hDatabase);
#define CeGetDatabaseSession xxx_CeGetDatabaseSession
BOOL xxx_CeBeginTransaction(HANDLE hSession, int isoLevel);
#define CeBeginTransaction xxx_CeBeginTransaction
BOOL xxx_CeEndTransaction(HANDLE hSession, BOOL fCommit);
#define CeEndTransaction xxx_CeEndTransaction
HANDLE xxx_CeOpenDatabaseInSession(HANDLE hSession, PCEGUID pGuid, PCEOID poid, LPWSTR pwszName, SORTORDERSPECEX* pSort, DWORD dwFlags, CENOTIFYREQUEST* pRequest);
#define CeOpenDatabaseInSession xxx_CeOpenDatabaseInSession
HANDLE xxx_CeOpenStream(HANDLE hDatabase, CEPROPID propid, DWORD dwMode);
#define CeOpenStream xxx_CeOpenStream
BOOL xxx_CeStreamRead(HANDLE hStream, BYTE* prgbBuffer, const DWORD cbRead, DWORD* pcbRead);
#define CeStreamRead xxx_CeStreamRead
BOOL xxx_CeStreamWrite(HANDLE hStream, BYTE* prgbBuffer, DWORD cbWrite, DWORD* pcbWritten);
#define CeStreamWrite xxx_CeStreamWrite
BOOL xxx_CeStreamSaveChanges(HANDLE hStream);
#define CeStreamSaveChanges xxx_CeStreamSaveChanges
BOOL xxx_CeStreamSeek(HANDLE hStream, DWORD cbMove, DWORD dwOrigin, DWORD* pcbNewOffset);
#define CeStreamSeek xxx_CeStreamSeek
BOOL xxx_CeStreamSetSize(HANDLE hStream, const DWORD cbSize);
#define CeStreamSetSize xxx_CeStreamSetSize
BOOL xxx_CeBeginSyncSession(HANDLE hSession, PCEGUID pSyncPartnerGuid, ULONGLONG genFrom, ULONGLONG genTo, DWORD dwFlags, ULONGLONG* pGenCur);
#define CeBeginSyncSession xxx_CeBeginSyncSession
BOOL xxx_CeEndSyncSession(HANDLE hSession, DWORD dwOutcome);
#define CeEndSyncSession xxx_CeEndSyncSession
BOOL xxx_CeGetChangedRecordCnt(HANDLE hSession, CEOID oidDB, DWORD* pdwCnt);
#define CeGetChangedRecordCnt xxx_CeGetChangedRecordCnt
HANDLE xxx_CeGetChangedRecords(HANDLE hSession, CEOID oidDB, DWORD dwChangeType);
#define CeGetChangedRecords xxx_CeGetChangedRecords
CEOID xxx_CeFindNextChangedRecord(HANDLE hChangeEnum);
#define CeFindNextChangedRecord xxx_CeFindNextChangedRecord
BOOL xxx_CeGetPropChangeInfo(HANDLE hChangeEnum, CEOID oidChangedRecord, CEPROPID propid, BOOL* pfPropChanged);
#define CeGetPropChangeInfo
BOOL xxx_CeGetRecordChangeInfo(HANDLE hChangeEnum, CEOID oidChangedRecord, VOID* pInfo);
#define CeGetRecordChangeInfo xxx_CeGetRecordChangeInfo
BOOL xxx_CeMarkRecord(HANDLE hChangeEnum, CEOID oidChangedRecord, BOOL fChanged);
#define CeMarkRecord xxx_CeMarkRecord
BOOL xxx_CeAttachCustomTrackingData(HANDLE hDB, CEOID oidChangedRecord, BYTE* rgbData, DWORD ccb);
#define CeAttachCustomTrackingData xxx_CeAttachCustomTrackingData
BOOL xxx_CeGetCustomTrackingData(HANDLE hDB, CEOID oidChangedRecord, BYTE* rgbData, DWORD* pccb);
#define CeGetCustomTrackingData xxx_CeGetCustomTrackingData

BOOL xxx_CeGetReplChangeMask (LPDWORD lpmask);
#define CeGetReplChangeMask xxx_CeGetReplChangeMask
BOOL xxx_CeSetReplChangeMask (DWORD mask);
#define CeSetReplChangeMask xxx_CeSetReplChangeMask
BOOL xxx_CeGetReplChangeBitsEx (PCEGUID pguid, CEOID oid, LPDWORD lpbits, DWORD dwFlags);
#define CeGetReplChangeBitsEx xxx_CeGetReplChangeBitsEx
BOOL xxx_CeSetReplChangeBitsEx (PCEGUID pguid, CEOID oid, DWORD mask);
#define CeSetReplChangeBitsEx xxx_CeSetReplChangeBitsEx
BOOL xxx_CeClearReplChangeBitsEx (PCEGUID pguid, CEOID oid, DWORD mask);
#define CeClearReplChangeBitsEx xxx_CeClearReplChangeBitsEx
BOOL xxx_CeGetReplOtherBitsEx (PCEGUID pguid, CEOID oid, LPDWORD lpbits);
#define CeGetReplOtherBitsEx xxx_CeGetReplOtherBitsEx
BOOL xxx_CeSetReplOtherBitsEx (PCEGUID pguid, CEOID oid, DWORD bits);
#define CeSetReplOtherBitsEx xxx_CeSetReplOtherBitsEx

BOOL xxx_ReplOpenSync(CEGUID* pguid, LPBYTE lpSyncState, DWORD dwSize, DWORD dwSyncFlags, HREPL* phRepl);
#define ReplOpenSync xxx_ReplOpenSync
BOOL xxx_ReplCheckpoint(HREPL hRepl, HANDLE hChangeEvent);
#define ReplCheckpoint xxx_ReplCheckpoint
BOOL xxx_ReplCloseSync(HREPL hRepl);
#define ReplCloseSync xxx_ReplCloseSync
BOOL xxx_ReplGetSyncState(HREPL hRepl, LPBYTE lpSyncState, DWORD* lpdwSize, BOOL fFailIfUnchanged);
#define ReplGetSyncState xxx_ReplGetSyncState
BOOL xxx_ReplChangeSyncSettings(HREPL hRepl, DWORD dwSyncFlags, DWORD* pOldFlags);
#define ReplChangeSyncSettings xxx_ReplChangeSyncSettings
BOOL xxx_ReplFindNextChange(HREPL hRepl, REPLCHANGEINFO* pReplInfo, BOOL fRestart);
#define ReplFindNextChange xxx_ReplFindNextChange
BOOL xxx_ReplGetOidStatus(HREPL hRepl, CEOID oid, REPLCHANGEINFO* pReplInfo);
#define ReplGetOidStatus xxx_ReplGetOidStatus

VOID xxx_CeChangeDatabaseLCID (PCEGUID pguid, DWORD LCID);
#define CeChangeDatabaseLCID xxx_CeChangeDatabaseLCID
BOOL xxx_CeRegisterFileSystemNotification (HWND hWnd);
#define CeRegisterFileSystemNotification xxx_CeRegisterFileSystemNotification
BOOL xxx_CeRegisterReplNotification (CENOTIFYREQUEST *pRequest);
#define CeRegisterReplNotification xxx_CeRegisterReplNotification
BOOL xxx_RegisterAFSEx (int index, HANDLE h, DWORD dw, DWORD version, DWORD flags);
#define RegisterAFSEx xxx_RegisterAFSEx
BOOL xxx_DeregisterAFS (int index);
#define DeregisterAFS xxx_DeregisterAFS
BOOL xxx_GetSystemMemoryDivision (LPDWORD lpdwStorePages, LPDWORD lpdwRamPages, LPDWORD lpdwPageSize);
#define GetSystemMemoryDivision xxx_GetSystemMemoryDivision
DWORD xxx_SetSystemMemoryDivision (DWORD dwStorePages);
#define SetSystemMemoryDivision xxx_SetSystemMemoryDivision
int xxx_RegisterAFSName (LPCWSTR pName);
#define RegisterAFSName xxx_RegisterAFSName
BOOL xxx_DeregisterAFSName (int index);
#define DeregisterAFSName xxx_DeregisterAFSName
void xxx_DumpFileSystemHeap (void);
#define DumpFileSystemHeap xxx_DumpFileSystemHeap
		 
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //_MACRO_WINDBASE_H_

