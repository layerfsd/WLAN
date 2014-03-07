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
// RDMSched.h -- routines exported from the RDM Scheduler libary.
// This file is copyright (c) 2000, Microsoft Corporation, All Rights 
// Reserved.

#ifndef _RDMSCHED_LIB_EXPORTS_
#define _RDMSCHED_LIB_EXPORTS_

//****************************************************************************
// Function Name: RunRDMScheduler
//
// Purpose: Start the RDM Scheduler.  Since the scheduler is 
// implemented through its very own threads, this returns quickly.
//
// Arguments:
//    NONE
//
// Return Values:
//    HANDLE - handle to the RDM object.  NULL denotes failure.
//
//****************************************************************************
HANDLE RunRDMScheduler();

//****************************************************************************
// Function Name: StopRDMScheduler
//
// Purpose: Stop an RDM Schduler.
//
// Arguments:
//    HANDLE hRDMSched - RDM Scheduler to halt
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.  Call GetLastError() for details.
//
//****************************************************************************
BOOL StopRDMScheduler(HANDLE hRDMSched);

//****************************************************************************
// Function Name: AddRDMTask
//
// Purpose: Add a new RDM task.
//
// Arguments:
//    HANDLE hRDMSched - RDM Scheduler to which the add should be added
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.  Call GetLastError() for details.
//
//****************************************************************************
BOOL AddRDMTask(HANDLE hRDMSched,
                LPTSTR mwzNames,
                DWORD * pdwTypes,
                DWORD * pcbItems,
                PBYTE mpbItems,
                HANDLE * phTask);

//****************************************************************************
// Function Name: DeleteRDMTask
//
// Purpose: Halt, tear down, and delete an RDM task.
//
// Arguments:
//    HANDLE hRDMSched - RDM Scheduler from with the task should be
// removed
//    HANDLE hTask - Handle to the task to be deleted (obtained from
// AddRDMTask)
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.  Call GetLastError() for details.
//
//****************************************************************************
BOOL DeleteRDMTask(HANDLE hRDMSched, HANDLE hTask);

//****************************************************************************
// Function Name: SetRDMTaskEnabled
//
// Purpose: Enable or disable processing of an RDM task.
//
// Arguments:
//    HANDLE hRDMSched - RDM Scheduler from with the task should be
// removed
//    HANDLE hTask - Handle to the task to be deleted (obtained from
// AddRDMTask)
//    BOOL bEnabled - new state
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.  Call GetLastError() for details.
//
//****************************************************************************
BOOL SetRDMTaskEnabled(HANDLE hRDMSched,
                       HANDLE hTask,
                       BOOL bEnabled);

//****************************************************************************
// Function Name: GetRDMTaskStatus
//
// Purpose: Gets global information about a task: is it enabled?  Is
// it ineligible, pending, or active?
//
// Arguments:
//    HANDLE hRDMSched - RDM Scheduler containing the task
//    HANDLE hTask - Handle to the task to be queried.
//    DWORD * pdwStatus - 
//    0 == disabled, >0 == enabled
//    1 == ineligible, 2 == pending, 3 == active
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.  Call GetLastError() for details.
//
//****************************************************************************
BOOL GetRDMTaskStatus(HANDLE hRDMSched,
                      HANDLE hTask,
                      DWORD * pdwStatus);                       

//****************************************************************************
// Function Name: UpdateRDMTask
//
// Purpose: Update the RDM Task schedule information on a given task
//
// Arguments:
//    HANDLE hRDMSched - RDM Scheduler containing the task
//    HANDLE hTask - Handle to the task to be altered
//    DWORD dwNewFlags - New scheduler flags for the task
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.  Call GetLastError() for details.
//
//****************************************************************************
BOOL UpdateRDMTask(HANDLE hRDMSched,
                   HANDLE hTask,
                   DWORD dwNewFlags);

//****************************************************************************
// Function Name: AddTrustedSource
//
// Purpose: Add a new trusted source to the TSL
//
// Arguments:
//    HANDLE hRDMSched - RDM Scheduler to which the source should be added
//    LPWSTR szTSName - The name for the trusted source (in full X.500
// RDN form!)
//    DWORD cchTSName - The number of characters in the name (w/o the
// terminal NULL)
//    BYTE * pbSerializedCert - The serialized form of the putative
// cert for szTSName
//    DWORD cbSerializedCert - The number of bytes in the
// serialized cert (w/o the terminal NULL)
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.  Call GetLastError() for details.
//
//****************************************************************************
BOOL AddTrustedSource(HANDLE hRDMSched,
                      LPWSTR szTSName,
                      DWORD cchTSName,
                      BYTE * pbSerializedCert,
                      DWORD cchSerializedCert);

//****************************************************************************
// Function Name: DeleteTrustedSource
//
// Purpose: Delete a cert belonging to a trusted source
//
// Arguments:
//    HANDLE hRDMSched - RDM Scheduler from with the source should be
// deleted
//    LPWSTR szTSName - The name for the trusted source (in full X.500
// RDN form!)
//    DWORD cchTSName - The number of characters in the name (w/o the
// terminal NULL)
//    BYTE * pbSerializedCert - The serialized form of the putative
// cert for szTSName
//    DWORD cbSerializedCert - The number of bytes in the
// serialized cert (w/o the terminal NULL)
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.  Call GetLastError() for details.
//
//****************************************************************************
BOOL DeleteTrustedSource(HANDLE hRDMSched,
                         LPWSTR szTSName,
                         DWORD cchTSName,
                         BYTE * pbSerializedCert,
                         DWORD cbSerializedCert);

//****************************************************************************
// Function name: OpenTaskInfoSrc
// 
// Purpose: Gets a handle to a task information source
//
// Arguments:
//    IN HANDLE hRDMSched - the scheduler within which the task is running
//    IN HANDLE hTask - the task we're trying to get information about
//    OUT HANDLE * phTaskInfo - pointer to the handle to the new source
//
// Return values:
//    TRUE - Success
//    FALSE - Failure (Call GetLastError() for details.)
//
//****************************************************************************

BOOL OpenTaskInfoSrc(IN HANDLE hRDMSched,
                     IN HANDLE hTask,
                     OUT HANDLE * phTaskInfo);

//****************************************************************************
// Function name: GetTaskInfoItem
// 
// Purpose: Get the name, size, and value of a data item.
//
// Arguments:
//    IN HANDLE hRDMSched - the scheduler within which the task is running
//    IN HANDLE hTask - handle to the task
//    IN HANDLE hTaskInfo - handle to the information source
//    IN DWORD nItem - index to the data item to return
//    OUT LPWSTR * pwzItemName - name of the item
//    OUT PDWORD pdwItemType - type of the item data
//    OUT PDWORD pcbItemData - size of the item data
//    OUT PBYTE const ppbItemData - item data itself
//
// Return values:
//    TRUE - Success
//    FALSE - Failure (call GetLastError() for details.)
//
//****************************************************************************

BOOL GetTaskItemInfo(HANDLE hRDMSched,
                     HANDLE hTask,
                     HANDLE hTaskInfo,
                     DWORD nItem,
                     LPWSTR * pwzItemName,
                     PDWORD pdwItemType,
                     PDWORD pcbItemData,
                     PBYTE * ppbItemData);

//****************************************************************************
// Function name: FreeTaskInfoSrc
// 
// Purpose: Release the information stored inside a task information source
//
// Arguments:
//    IN HANDLE hRDMSched - the scheduler within which the task is running
//    IN HANDLE hTask - handle to the task
//    IN HANDLE hTaskInfo - handle to the information source
//
// Return values:
//    TRUE - Success
//    FALSE - Failure (call GetLastError() for details.)
//
// After the call, hTaskInfo is no longer a valid handle.
//
//****************************************************************************

BOOL FreeTaskInfoSrc(HANDLE hRDMSched,
                     HANDLE hTask,
                     HANDLE hTaskInfo);

//****************************************************************************
//
// Function Name: RegisterCallbackWindow
//
// Purpose: Register a callback window
//
// Arguments:
//    HANDLE hRDMSched -- [IN] Handle to the scheduler
//    HWND hWndCallback -- [IN] Window handle
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.
//
//****************************************************************************

BOOL RegisterCallbackWindow(HANDLE hRDMSched,
                            HWND hWndCallback);

//****************************************************************************
//
// Function Name: UnregisterCallbackWindow
//
// Purpose: Unregister a previously registered callback window
//
// Arguments:
//    HANDLE hRDMSched -- [IN] Handle to the scheduler
//    HWND hWndCallback -- [IN] Window handle
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.
//
//****************************************************************************

BOOL UnregisterCallbackWindow(HANDLE hRDMSched,
                              HWND hWndCallback);

//****************************************************************************
//
// Function Name: UpdateTaskLists
//
// Purpose: Called to update the contents of the task lists
//
// Arguments:
//    HANDLE hSched -- [IN] Handle to the RDM Scheduler instance
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.
//
//****************************************************************************
BOOL UpdateTaskLists(HANDLE hSched);

//****************************************************************************
//
// Function Name: FillEventsVector
//
// Purpose: Add events to the beginning of a vector of events
//
// Arguments:
//    HANDLE hSched -- [IN] Handle to the RDM Scheduler instance
//    HANDLE * ahActions -- [OUT] Vector of events
//    DWORD * pnhActions -- [OUT] Number of elements of the vector
//    DWORD * pdwWait -- [OUT] Time to wait
//
// Return Values:
//    TRUE - Success
//    FALSE - Failure.
//
//****************************************************************************
BOOL FillEventsVector(HANDLE hSched,
                      HANDLE * ahActions,
                      DWORD * pnhActions,
                      DWORD * pdwWait);


#endif // !defined(_RDMSCHED_LIB_EXPORTS_)
