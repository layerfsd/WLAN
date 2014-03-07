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

#pragma once


#ifdef RIL_WATSON_REPORT

static const TCHAR* gc_szDWATLogName           = TEXT("\\windows\\rilatcmd.log");
static const TCHAR* gc_szDWInfoLogName         = TEXT("\\windows\\rilinfo.log");
static const TCHAR* gc_szDWManifestFilename    = TEXT("\\windows\\rilmanifest.txt");
static const TCHAR* gc_szDWManifestVersion     = TEXT("131072");
static const TCHAR* gc_szDWEventType           = TEXT("WinCE420RIL");
static const TCHAR* gc_szDWAppName             = TEXT("RilGSM.dll");
static const TCHAR* gc_szDWBasicManifestFormat = TEXT("\r\nVersion=%s\r\nEventType=%s\r\nGeneral_AppName=%s\r\n");
static const TCHAR* gc_szDWParamFormat         = TEXT("P1=%s\r\nP2=%s\r\nP3=%s\r\nP4=%s\r\nP5=%s\r\nP6=%s\r\nP7=%s\r\nP8=%s\r\n");
static const TCHAR* gc_szDWFilesToDeleteFormat = TEXT("FilesToDelete=%s\r\n");
static const TCHAR* gc_szDWCmdName             = TEXT("dw.exe");
static const TCHAR* gc_szDWCmdLineFormat       = TEXT("-d %s");
static const TCHAR* gc_szDWCategoryHang        = TEXT("Hang");
static const TCHAR* gc_szDWCategoryReset       = TEXT("Reset");
static const TCHAR* gc_szDWCategoryInfo        = TEXT("Information");
static const TCHAR* gc_szDWCategoryGPRS        = TEXT("GPRS");
static const TCHAR* gc_szDWTimeFormat          = TEXT("%d/%d/%d %d:%.2d:%.2d");
static const TCHAR* gc_szDWParamBlank          = TEXT("NULL");


static const TCHAR* gc_szDWInfoLogCategoryFormat     = TEXT("Category=%s\r\n");
static const TCHAR* gc_szDWInfoLogRadioBuildFormat   = TEXT("RadioBuild=%s\r\n");
static const TCHAR* gc_szDWInfoLogCEBuildFormat      = TEXT("CEBuild=%s\r\n");
static const TCHAR* gc_szDWInfoLogDeviceFormat       = TEXT("DeviceType=%s\r\n");
static const TCHAR* gc_szDWInfoLogAtCMDFormat        = TEXT("ATCmd=%s\r\n");
static const TCHAR* gc_szDWInfoLogSIMNetIDFormat     = TEXT("SIMNetID=%s\r\n");
static const TCHAR* gc_szDWInfoLogGSMRegFormat       = TEXT("GSMReg=%s\r\n");
static const TCHAR* gc_szDWInfoLogGPRSRegFormat      = TEXT("GPRSReg=%s\r\n");
static const TCHAR* gc_szDWInfoLogLastAPNFormat      = TEXT("LastAPN=%s\r\n");
static const TCHAR* gc_szDWInfoLogNetIDFormat        = TEXT("NetID=%s\r\n");
static const TCHAR* gc_szDWInfoLogLastSSFormat       = TEXT("SignalStrength=%s\r\n");
static const TCHAR* gc_szDWInfoLogErrorFormat        = TEXT("Error=%s\r\n");
static const TCHAR* gc_szDWInfoLogRadioResetFormat   = TEXT("RadioReset=%s\r\n");


#define MAXLENGTH_DEVICETYPE        (256)
#define MAXLENGTH_CATEGORY          (64)
#define MAXLENGTH_BUILDVERSION      (64)
#define MAXLENGTH_REGSTATUS         (16)
#define MAXLENGTH_SIGNALSTRENGTH    (16)
#define MAXLENGTH_RESETTIME         (32)

#define IMSI_NETIDSIZE              (6)

#define LOGBUFFER_NODATA            ((BYTE*)-1)
#define RILATCMDLOGSIZE             (2048)
#define WATSONREPORT_WAITFORSHELL   (300000)
#define ALLOW_DRWATSON_TORUN_TIME_MS (5000)
#define DELAY_BEFORE_WATSON_LOG_MS (10000)

typedef struct RilWatsonParameters_tag
{
    TCHAR szDeviceType[MAXLENGTH_DEVICETYPE];
    TCHAR szRadioBuild[MAXLENGTH_EQUIPINFO];
    TCHAR szCategory[MAXLENGTH_CATEGORY];
    TCHAR szATCmd[MAXLENGTH_CMD];
    TCHAR szError[MAXLENGTH_CMD];
    TCHAR szCEBuild[MAXLENGTH_BUILDVERSION];
    TCHAR szSIMNetID[MAXLENGTH_USERID];
    TCHAR szGSMReg[MAXLENGTH_REGSTATUS];
    TCHAR szGPRSReg[MAXLENGTH_REGSTATUS];
    TCHAR szAPN[MAXLENGTH_GPRSACCESSPOINTNAME];
    TCHAR szNetID[MAXLENGTH_OPERATOR_LONG];
    TCHAR szSignal[MAXLENGTH_SIGNALSTRENGTH];
    TCHAR szRadioReset[MAXLENGTH_RESETTIME];
} RilWatsonParameters;

typedef struct RilInfoCache_tag
{
    RILEQUIPMENTINFO rileiEquipmentInfo;
    char szIMSI[MAXLENGTH_USERID];
    char szLastCmd[MAXLENGTH_CMD];
    BOOL fLastCmdSuppress;
    char szLastError[MAXLENGTH_CMD];
    DWORD dwRegStatus;
    DWORD dwGPRSRegStatus;
    char szAPN[MAXLENGTH_GPRSACCESSPOINTNAME];
    char szOperatorName[MAXLENGTH_OPERATOR_LONG];
    int nSignalStrength;
    SYSTEMTIME stRadioReset;
    BOOL fRadioWasReset;
} RilInfoCache;


class CRilATCmdLogBuffer
{
public:
    CRilATCmdLogBuffer::CRilATCmdLogBuffer();
    CRilATCmdLogBuffer::~CRilATCmdLogBuffer();

    HRESULT Init(DWORD dwSize);
    HRESULT Write(BYTE* pbData, DWORD dwSize);
    BOOLEAN DumpToFile(TCHAR* szFilename);
    BOOLEAN CRilATCmdLogBuffer::AdvanceReadPastDelimiter(BYTE bDelimiter);

private:
    BYTE* m_pbBuffer;
    DWORD m_dwSize;
    BYTE* m_pbWrite;
    BYTE* m_pbRead;
    CRITICAL_SECTION m_csLogAccess;

    BOOLEAN IncrementPointers(DWORD dwSize);
    BOOLEAN ReadWillBePassed(DWORD dwSize);
    DWORD LengthFromBeginning(BYTE* pbPosition);
    DWORD LengthToEnd(BYTE* pbPosition);
};



HRESULT WatsonReport(CRilATCmdLogBuffer* pATLog, RilInfoCache* pInfoCache, DWORD dwEventId, const TCHAR* szDeviceType);


extern RilInfoCache g_RilInfoCache;
extern CRilATCmdLogBuffer g_RilATCmdLogBuffer;
extern const TCHAR* gc_szWatsonDeviceType;
extern DWORD g_WatsonThreadId;
extern CRITICAL_SECTION g_csRilInfoCache;
extern BOOL g_fWatsonCalled;
#endif //RIL_WATSON_REPORT

