// Netui_130221.cpp : �ش� DLL�� �ʱ�ȭ ��ƾ�� �����մϴ�.
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
//TODO: �� DLL�� MFC DLL�� ���� �������� ��ũ�Ǿ� �ִ� ���
//		MFC�� ȣ��Ǵ� �� DLL���� ���������� ��� �Լ���
//		���� �κп� AFX_MANAGE_STATE ��ũ�ΰ�
//		��� �־�� �մϴ�.
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �Ϲ����� �Լ� ������ ���⿡ �ɴϴ�.
//		}
//
//		�� ��ũ�δ� MFC�� ȣ���ϱ� ����
//		�� �Լ��� �ݵ�� ��� �־�� �մϴ�.
//		��, ��ũ�δ� �Լ��� ù ��° ���̾�� �ϸ� 
//		��ü ������ �����ڰ� MFC DLL��
//		ȣ���� �� �����Ƿ� ��ü ������ ����Ǳ� ����
//		���;� �մϴ�.
//
//		�ڼ��� ������
//		MFC Technical Note 33 �� 58�� �����Ͻʽÿ�.
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



// CNetui_130221App ����

CNetui_130221App::CNetui_130221App()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CNetui_130221App ��ü�Դϴ�.

CNetui_130221App theApp;


// CNetui_130221App �ʱ�ȭ
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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	RETAILMSG(1, (TEXT("[NETUI] ExitInstance \r\n") ));
    DeleteWZCQCFG();

	return CWinApp::ExitInstance();
}
