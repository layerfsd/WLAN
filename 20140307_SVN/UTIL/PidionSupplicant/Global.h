
//************************************************************************************************//
// Defines

// Setting ������ ����Ǵ� ���
#define PIDSUP_XMLDOC_PATH			L"\\ProgramStore\\data\\PidSupSetting.xml"

// Password ���� path
#define PIDSUP_PASSWORD_PATH		L"\\Windows\\PidSupLogin.dat"

// Default login password
#define PIDSUP_DEFAULT_PASSWORD		L"PIDION"

// �Ϲ� ����� ��忡�� ������ ���� ����
#define PIDSUP_USERMODE_TABCNT		(3)

// Admin ��忡�� ������ ���� ����
#define PIDSUP_FULL_TABCNT			(5)


//************************************************************************************************//
// Enum ����

// Tab ����
enum// PidSup_TabPage : int
{
    PidSup_Tab_ProfilePage = 0,
    PidSup_Tab_StatusPage,
    PidSup_Tab_PingPage,
    PidSup_Tab_RoamingPage,
	PidSup_Tab_SettingPage
};

// ������ ��忩�θ� Ȯ��
enum// PidSup_AdminMode : int
{
	PidSup_AdminMode_User           = 0x00,
	PidSup_AdminMode_ManualAdmin    = 0x01,
	PidSup_AdminMode_AutoAdmin      = 0x02,
	PidSup_AdminMode_IsAdmin        = PidSup_AdminMode_ManualAdmin | PidSup_AdminMode_AutoAdmin
};



//************************************************************************************************//
// ��������
extern DWORD	g_dwScreenRate;

// ������ ��� Ȯ�� ����
extern DWORD	g_dwAdminMode;

// Tab title�� ��Ÿ����.
extern  LPWSTR  PIDSUP_TABTITLE[PIDSUP_FULL_TABCNT];


//************************************************************************************************//
// �Լ�

// ���� ���������� File Version�� �о�´�.
BOOL GetCurrentFileVersion(LPTSTR tszFileVer);

