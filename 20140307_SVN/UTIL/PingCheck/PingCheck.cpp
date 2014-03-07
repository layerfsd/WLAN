// PingCheck.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PingCheck.h"
#include "PingCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif






// CPingCheckApp

BEGIN_MESSAGE_MAP(CPingCheckApp, CWinApp)
END_MESSAGE_MAP()


// CPingCheckApp construction
CPingCheckApp::CPingCheckApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPingCheckApp object
CPingCheckApp theApp;

// CPingCheckApp initialization

BOOL CPingCheckApp::InitInstance()
{
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the Windows Mobile specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));



	HANDLE hMutex = CreateMutex(NULL, TRUE, DEFAULT_MUTEX_NAME);


	if(!hMutex) 
    {
        return FALSE;
    }


	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		HWND hWnd = FindWindow(NULL, _T("PingCheck") );

		if( MessageBox(GetForegroundWindow(), _T("This Program is Already running, Do you want to exit previous program?"), _T("Warning"), MB_YESNO) == IDYES)
		{

			ReleaseMutex(hMutex);

			DestroyWindow(hWnd);
			
			SendMessage(hWnd, WM_STOP_CHECK, 0, 0);

			MessageBox(GetForegroundWindow(), _T("Previous program is closed, Please restart the program"), _T("Complete"), MB_OK);
		}

		CloseHandle(hMutex);
		return S_OK;
	}
	else
	{

		CPingCheckDlg dlg;
		m_pMainWnd = &dlg;

		INT_PTR nResponse = dlg.DoModal();


		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
