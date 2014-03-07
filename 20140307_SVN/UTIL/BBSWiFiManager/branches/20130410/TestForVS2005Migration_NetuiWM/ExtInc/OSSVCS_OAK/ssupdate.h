#ifndef _ssupdate_h_
#define _ssupdate_h_

#define CALUPD_MUTEX_NAME L"CALSTORE!ExternalNotifyCalstore"
#ifdef __cplusplus
extern "C" {
#endif

HRESULT SSUpdateInit();
HRESULT SSUpdateDeinit();
HRESULT SSSuspendDatabaseMonitoring();
HRESULT SSResumeDatabaseMonitoring();

#ifdef __cplusplus
}
#endif

#endif // _ssupdate_h_

