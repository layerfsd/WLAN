#ifndef _NETUIDEBUGLOG_H_
#define _NETUIDEBUGLOG_H_

#include <windows.h>


#define TIME_STAMP_ON		(TRUE)
#define TIME_STAMP_OFF		(FALSE)

#ifdef __cplusplus
extern "C" {
#endif

void NetuiDbgMsg(BOOL bTimeStamp, LPVOID strFormat, ...);

#ifdef __cplusplus
}
#endif

#endif //_NETUIDEBUGLOG_H_

