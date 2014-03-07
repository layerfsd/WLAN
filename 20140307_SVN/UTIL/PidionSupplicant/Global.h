
//************************************************************************************************//
// Defines

// Setting 파일이 저장되는 경로
#define PIDSUP_XMLDOC_PATH			L"\\ProgramStore\\data\\PidSupSetting.xml"

// Password 저장 path
#define PIDSUP_PASSWORD_PATH		L"\\Windows\\PidSupLogin.dat"

// Default login password
#define PIDSUP_DEFAULT_PASSWORD		L"PIDION"

// 일반 사용자 모드에서 나오는 탭의 갯수
#define PIDSUP_USERMODE_TABCNT		(3)

// Admin 모드에서 나오는 탭의 갯수
#define PIDSUP_FULL_TABCNT			(5)


//************************************************************************************************//
// Enum 선언

// Tab 순서
enum// PidSup_TabPage : int
{
    PidSup_Tab_ProfilePage = 0,
    PidSup_Tab_StatusPage,
    PidSup_Tab_PingPage,
    PidSup_Tab_RoamingPage,
	PidSup_Tab_SettingPage
};

// 관리자 모드여부를 확인
enum// PidSup_AdminMode : int
{
	PidSup_AdminMode_User           = 0x00,
	PidSup_AdminMode_ManualAdmin    = 0x01,
	PidSup_AdminMode_AutoAdmin      = 0x02,
	PidSup_AdminMode_IsAdmin        = PidSup_AdminMode_ManualAdmin | PidSup_AdminMode_AutoAdmin
};



//************************************************************************************************//
// 전역변수
extern DWORD	g_dwScreenRate;

// 관리자 모드 확인 변수
extern DWORD	g_dwAdminMode;

// Tab title을 나타낸다.
extern  LPWSTR  PIDSUP_TABTITLE[PIDSUP_FULL_TABCNT];


//************************************************************************************************//
// 함수

// 현재 실행파일의 File Version을 읽어온다.
BOOL GetCurrentFileVersion(LPTSTR tszFileVer);

