/*
 * Atheros log viewer application
 * 
 *  This application provides a GUI to view driver logs when a debugger interface is not available.
 *  This viewer currently supports viewing the BT filter debug logs.
 * 
 * Copyright (c) 2008 Atheros Communications Inc.
 * All rights reserved.
 *
 * 
// The software source and binaries included in this development package are
// licensed, not sold. You, or your company, received the package under one
// or more license agreements. The rights granted to you are specifically
// listed in these license agreement(s). All other rights remain with Atheros
// Communications, Inc., its subsidiaries, or the respective owner including
// those listed on the included copyright notices.  Distribution of any
// portion of this package must be in strict compliance with the license
// agreement(s) terms.
// </copyright>
// 
// <summary>
// 	Wifi driver for AR6002
// </summary>
//
 * 
 */
 
#include <windows.h>
#include "resource.h"
#include "resrc1.h"
#include "msgqueue.h"
#include "drvmsgqlogging.h"
#ifdef WINDOWS_MOBILE
#include "aygshell.h"
#endif

#define APPNAME TEXT("AthMsmqLogView")
#define LOG_FILE_PATH

HINSTANCE g_hInst;
HANDLE    g_hLogMsgQueue = NULL;
HANDLE    g_hLoggingThread = NULL;
HANDLE    g_hExitAppEvent = NULL;
BOOL      g_ShutDown = FALSE;
HWND      g_hDialog;

static void OutputStringToLogDisplay(HWND hWnd, TCHAR *pString)
{
    HWND hCtrl;
    
    hCtrl = GetDlgItem(hWnd, IDC_SPYLOG);
    
    if (hCtrl) {
        SendMessage(hCtrl,
                    EM_REPLACESEL, 
                    0, 
                    (LPARAM)pString);
    }
    
}
    
static void SetOutputDisplayMaxChars(HWND hWnd)
{
    HWND hCtrl;
    
    hCtrl = GetDlgItem(hWnd, IDC_SPYLOG);
    
    g_hDialog = hWnd;
    
    if (hCtrl) {
        SendMessage(hCtrl,
                    EM_LIMITTEXT,  
                    0,  /* set for 64K characters for multi-line edit controls */
                    0);
    }
    
}    
 
     
static void ClearLogDisplay(HWND hWnd)
{
    HWND hCtrl;
    
    hCtrl = GetDlgItem(hWnd, IDC_SPYLOG);
    
    if (hCtrl) {
        SetWindowText(hCtrl,TEXT(" ")); 
    }
    
}

#define BTFILTER_LOG_FILE_NAME TEXT("\\My Documents\\BtFilterLog.txt")

char  g_CharBuffer[ATHMSGQ_LOG_MAX_CHARS_PER_MSG + 1];
WCHAR g_WideCharBuffer[ATHMSGQ_LOG_MAX_CHARS_PER_MSG + 1];

static void WriteLogToFile(HWND hWnd)
{
    HWND        hCtrl;
    HANDLE      hFile = INVALID_HANDLE_VALUE;
    LONG        lineCount;
    LRESULT     charactersRead;
    WORD        *pMaxCharParam;
    LONG        i,j;
    DWORD       written;
        
    memset(&g_WideCharBuffer, 0, sizeof(g_WideCharBuffer));
    
    hCtrl = GetDlgItem(hWnd, IDC_SPYLOG);
        
    do {
    
        if (!hCtrl) {
            break;    
        }       
           
        hFile = CreateFile(BTFILTER_LOG_FILE_NAME,
                           GENERIC_WRITE,
                           FILE_SHARE_WRITE,
                           NULL,
                           CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
                           
        if (INVALID_HANDLE_VALUE == hFile) {
            break;    
        }
        
        lineCount = (LONG)SendMessage(hCtrl,EM_GETLINECOUNT,0,0);
                    
        if (lineCount <= 1) {
                /* a return value of 1, indicates no lines */
            break;    
        }
        
        lineCount--;
        
        for (i = 0; i < lineCount; i++) {
            pMaxCharParam = (WORD *)&g_WideCharBuffer[0];
            *pMaxCharParam = ATHMSGQ_LOG_MAX_CHARS_PER_MSG;
            charactersRead = SendMessage(hCtrl, EM_GETLINE, (WPARAM)i, (LPARAM)g_WideCharBuffer); 
            if (charactersRead == 0) {
                break;    
            }  
                /* convert to ASCII */ 
            for (j = 0; j < charactersRead; j++) {
                g_CharBuffer[j] = (char)g_WideCharBuffer[j];    
            }
            
            if (!WriteFile(hFile,g_CharBuffer,charactersRead,&written,NULL)) {
                break;    
            }
        }
        
        
    } while (FALSE);
    
    if (hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hFile);
        MessageBox(hWnd, TEXT("Log written to file."), APPNAME, MB_OK);    
    }
}

static TCHAR g_LogBuffer[ATHMSGQ_LOG_MAX_CHARS_PER_MSG + 1];

static void ProcessMsgQueue()
{
    DWORD                       bytesRead;
    DWORD                       flags;
    
    while (1) {
        
        bytesRead = 0;
        flags = 0;
        
        if (!ReadMsgQueue(g_hLogMsgQueue,
                          g_LogBuffer,
                          ATHMSGQ_LOG_MAX_MESSAGE_SIZE,
                          &bytesRead,
                          0,
                          &flags)) {
            LONG lastError = GetLastError();
            if (lastError != ERROR_TIMEOUT) {                  
                RETAILMSG(1,(TEXT("FilterSpy: ReadMsgQueue (BT) failed (%d) \r\n"),lastError));
            }
            break;                    
        }  
        
        if (0 == bytesRead) {
            break;    
        }        
        
        if (flags == MSGQUEUE_MSGALERT) {
            continue;    
        }
        
        /* we have a message, queue it up to the display */
        if (g_hDialog != NULL) {
            OutputStringToLogDisplay(g_hDialog, (TCHAR *)g_LogBuffer);
        } 
        
    }

}

      
static DWORD BtFiltLoggingThread(LPVOID pContext)
{
    HANDLE              eventArray[2];
    DWORD               waitStatus;
    LONG                index;
    int                 eventObjectCount;
             
        /* setup events to wait on */    
    eventArray[0] = g_hExitAppEvent;
    eventArray[1] = g_hLogMsgQueue;
    eventObjectCount = 2;

        /* wait for dialog to fully initialize up otherwise sending text to the dialog will
         * result in garbage */
    Sleep(1000);
    
    while (!g_ShutDown) {
    
        waitStatus = WaitForMultipleObjects(eventObjectCount, eventArray, FALSE, INFINITE);
        
        if (WAIT_FAILED == waitStatus) {
            break;    
        }
    
        index = (LONG)waitStatus - (LONG)WAIT_OBJECT_0;
        
        if ((index < 0) || (index >= eventObjectCount)) {
            break;            
        }
        
        
        if (1 == index) {
            /* message queue was signalled */
            ProcessMsgQueue();    
        }
        
        if (0 == index) {
            if (g_ShutDown) {
                break;    
            }        
        }
            
    
    }
    
    return 0;
}

#define BUTTON_WIDTH   65
#define BUTTON_HEIGHT  40

static void ResizeUICtrls(HWND hDialog)
{
    RECT rect;
    int  posX;
    int  posY;
    int  width;
    int  height;
    int  clientAreaWidth;
    int  clientAreaHeight;
    HWND hCtrl;

    do { 
        
            /* adjust main dialog window to take up a portion of the screen in order
             * to allow access to the today-screen in order to switch to the WLAN manager */

        if (!GetWindowRect(hDialog, &rect)) {
            break;    
        }

        width = rect.right - rect.left;
        height = rect.bottom - rect.top;

        if (width > height) {
            /* landscape aspect ratio 
               take up the left half of the screen */
            width = width / 2;
        } else {
            /* portrait mode, take the bottom half of the screen */
            height = height / 2;
            rect.top += height;
        }

        MoveWindow(hDialog, 
                   rect.left, 
                   rect.top, 
                   width, 
                   height, 
                   TRUE);
                
        /* resize edit control */
        
        if (!GetClientRect(hDialog, &rect)) {
            break;    
        }
        
        
        posX = 10;
        posY = 20;
        
        clientAreaWidth = rect.right - rect.left;
        clientAreaHeight = rect.bottom - rect.top;
        
        width = clientAreaWidth - (posX * 2);
        height = clientAreaHeight - (posX * 2) - BUTTON_WIDTH - 10;
        hCtrl = GetDlgItem(hDialog, IDC_SPYLOG);
  
        if ((width > 0) && (height > 0)) {
            MoveWindow(hCtrl, posX, posY, width, height, TRUE);
        }
    
        hCtrl = GetDlgItem(hDialog, IDC_CLEARLOG);
        
        posX = posX + 10;
        posY = posY + height + 10;
        
        MoveWindow(hCtrl, posX, posY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
            
        posX = posX + 10 + BUTTON_WIDTH;
        hCtrl = GetDlgItem(hDialog, IDC_PRINT_FILE);
        MoveWindow(hCtrl, posX, posY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
        
        posX = posX + 10 + BUTTON_WIDTH;
        hCtrl = GetDlgItem(hDialog, IDC_EXIT);
        MoveWindow(hCtrl, posX, posY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
        
        
    } while (FALSE);
     
}


static void InitializeDialog(HWND hWnd)
{
    BOOL               success = FALSE;
    PTCHAR             pErrorString = NULL;   
    MSGQUEUEOPTIONS    msgOptions;   
    DWORD              threadId;    
    
    do {
 
#ifdef SHIDIF_SIZEDLGFULLSCREEN   
        {
            SHINITDLGINFO shidi;
            memset(&shidi,0,sizeof(shidi));  
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_SIZEDLGFULLSCREEN | SHIDIF_DONEBUTTON;
            shidi.hDlg = hWnd;
            SHInitDialog(&shidi);              
        }
#endif
        ResizeUICtrls(hWnd); 
        
        ATHMSGQ_INIT_QUEUE_OPTIONS(msgOptions,TRUE);
            
        g_hLogMsgQueue = CreateMsgQueue(ATHBTFILTER_LOG_MSG_QUEUE_NAME, &msgOptions);
   
        if (NULL == g_hLogMsgQueue) {
            pErrorString = TEXT(" Failed to create logging message queue. \r\n"); 
            break;
        }
     
        g_hExitAppEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        
        if (NULL == g_hExitAppEvent) {
            break;    
        }
        
        g_hLoggingThread = CreateThread(NULL,
                                        0,
                                        BtFiltLoggingThread,
                                        NULL,
                                        0,
                                        &threadId);
        
        success = TRUE;
            
    } while (FALSE);
    
    if (pErrorString != NULL) {
        MessageBox(hWnd, pErrorString, APPNAME, MB_OK | MB_ICONEXCLAMATION);
    }
}   

static BOOL CALLBACK DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL retVal = 0;

    switch (uMsg) {
        case WM_INITDIALOG:
            InitializeDialog(hWnd);
            SetOutputDisplayMaxChars(hWnd);   
            OutputStringToLogDisplay(hWnd,TEXT("DbgLog Viewer Started \r\n"));   
            break;        
        case WM_COMMAND:
            {
                int wID = LOWORD(wParam);
                switch (wID) {
                    case IDC_PRINT_FILE:
                        WriteLogToFile(hWnd);
                        break; 
                    case IDC_CLEARLOG:
                        ClearLogDisplay(hWnd);
                        break;
                    case IDC_EXIT:
                    case IDOK:    
                    case IDCANCEL:
                        EndDialog (hWnd, TRUE);
                        return 0;

                }
            }
            break;        
        default:
            break;
    }

    return retVal;
}


int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPWSTR lpszCmdLine, int nCmdShow) {
	

    HANDLE hMutex;
    
    g_hInst = hInst;
    
    hMutex = CreateMutex(NULL,TRUE,TEXT("FilterSpyApp"));

    if (NULL == hMutex) {
        return 0;        
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);
        return 0;
    }
    
    memset(g_LogBuffer, 0, sizeof(g_LogBuffer));
    
    InitCommonControls();
  
    do {
        
        DialogBox(g_hInst, 
                  MAKEINTRESOURCE(IDD_MAINBOX), 
                  NULL, 
                  DlgProc);

    } while (0);

    g_ShutDown = TRUE;
    
    if (g_hExitAppEvent != NULL) {
        SetEvent(g_hExitAppEvent);    
    }
    
    if (g_hLoggingThread != NULL) {
        WaitForSingleObject(g_hLoggingThread, INFINITE);
        CloseHandle(g_hLoggingThread);         
    }
    
    if (g_hExitAppEvent != NULL) {
        CloseHandle(g_hExitAppEvent);    
    }
        
    if (g_hLogMsgQueue != NULL) {
        CloseMsgQueue(g_hLogMsgQueue);   
    }
    
    CloseHandle(hMutex);
	return 0;
}
