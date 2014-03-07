// Netui_130221.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "Netui_130221.h"



#include <types.h>
#include <netui.h>
#include <netuip.h>
#include <resource.h>
#include <commctrl.h>
#include <wcommctl.h>
#include "wzcuip.h"
#include <wpcpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며 
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//


// CNetui_130221App

BEGIN_MESSAGE_MAP(CNetui_130221App, CWinApp)
END_MESSAGE_MAP()





// ----------------------------------------------------------------
//
// Global Data
//
// ----------------------------------------------------------------

HINSTANCE g_hInst;       // code
HINSTANCE g_hInstRes;    // resources
HINSTANCE g_hInstResDPI;


// 
// ----------------------------------------------------------------
//
//  GetNetString
//
// ----------------------------------------------------------------
int
GetNetString (UINT uID, LPTSTR lpBuffer, int nBufferMax)
{
    return LoadString (HINST_RESDLL, (uID+GETNETSTR_BASE), lpBuffer, nBufferMax);
}

// 
// ----------------------------------------------------------------
//
//  NetMsgBox
//
// ----------------------------------------------------------------
BOOL WINAPIV NetMsgBox (HWND hParent, DWORD dwFlags, TCHAR *szStr)
{
    TCHAR szTitle[200];
    DWORD dwStyle, dwId;
    int iRet;
    HCURSOR hCur;

    // Default title is "Windows CE Networking"
    if (dwFlags & NMB_FL_TITLEUSB)
        dwId = IDS_NETUI_USB_CAPTION;
    else
        dwId = IDS_NETUI_NETMSGBOX_TITLE;

    if (!LoadString(HINST_RESDLL, dwId,szTitle,
                    sizeof(szTitle)/sizeof(szTitle[0])))
        return FALSE;
    dwStyle = MB_SETFOREGROUND | MB_APPLMODAL;
    if (dwFlags & NMB_FL_OK)
        dwStyle |= MB_OK;
    if (dwFlags & NMB_FL_EXCLAIM)
        dwStyle |= MB_ICONEXCLAMATION;
    if (dwFlags & NMB_FL_YESNO)
        dwStyle |= MB_YESNO;
    if (dwFlags & NMB_FL_TOPMOST)
        dwStyle |= MB_TOPMOST;

    // Disable cursor, and save old value
    hCur = SetCursor(NULL);
    iRet = MessageBox( hParent, szStr, szTitle,dwStyle | MB_SETFOREGROUND);
    SetCursor(hCur);

    if (dwFlags & NMB_FL_YESNO)
        return ((iRet == IDYES) ? TRUE : FALSE);
    else
        return (iRet? TRUE:FALSE);
}



// CNetui_130221App 생성

CNetui_130221App::CNetui_130221App()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CNetui_130221App 개체입니다.

CNetui_130221App theApp;


// CNetui_130221App 초기화
DECLARE_COMMCTRL_TABLE;
BOOL CNetui_130221App::InitInstance()
{
	CWinApp::InitInstance();
	RETAILMSG(1, (TEXT("[NETUI] InitInstance \r\n") ));
	
	HINSTANCE hInstDll = AfxGetInstanceHandle();

	DisableThreadLibraryCalls(hInstDll);

        g_hInstResDPI = hInstDll;
		g_hInstRes = hInstDll;
		g_hInstResDPI = hInstDll;

		RETAILMSG(1, (TEXT("[NETUI] hInstDll : 0x%X \r\n"), hInstDll));
		
        InitCommCtrlTable();
        DEBUGMSG (ZONE_INIT, (TEXT("NETUI process attach\r\n")));
        InitializeWZCQCFG();

	return TRUE;
}



int CNetui_130221App::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	RETAILMSG(1, (TEXT("[NETUI] ExitInstance \r\n") ));
    DeleteWZCQCFG();

	return CWinApp::ExitInstance();
}
