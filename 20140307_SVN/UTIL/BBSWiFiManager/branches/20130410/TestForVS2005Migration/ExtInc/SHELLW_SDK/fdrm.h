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
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//

// ******************************************************************************
//  APIs for dealing with File Base Digital Rights Managed (FDRM) content
//                                  
//  FileDrmIsDRM                      Determine if an object is FDRM protected
//  FileDrmCreateFile                 Open a file containing FDRM content
//  FileDrmCreateForwardableContent   Create forwardable version of content
//  FileDrmCreateForwardableContentEx Create forwardable version of content;
//                                    decide on the extension for the output file
//                                    based on the DRM version
//  FileDrmNotifyEnable               Enable FDRM provider notifications
//  FileDrmNotifyDisable              Disable FDRM provider notifications
//  FileDrmShowLicenseInfo            Show rights associated with an object
//  FileDrmHandleError                Display error UI / try to correct problem
//  FileDrmRenewRights                Renews the rights for a FDRM object
//  FileDrmGetMetric                  Retrieve FDRM metric
//  FileDrmVerifyRights               Verify that an object can be used
//  FileDrmCommitRights               Commits the rights used by an object
//  FileDrmDelete                     Delete an object and rights
//  FileDrmStoreContent               Stores protected content on the device
// ******************************************************************************

#ifndef FDRM_H
#define FDRM_H

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

// **************************************************************************
// ERROR CODES
//
// [SUCCESS]
//      S_FDRM_NOPROVIDER
//          API call succeeded but no FDRM provider is installed.
//
//      S_FDRM_NOTDRMOBJECT
//          API call succeeded but the passed in  object is not FDRM protected.
//
// {ERROR]
//      E_FDRM_FILEDELETED
//          Object did not have rights and could not be reactived. User choose
//           to delete the object.
//
//      E_FDRM_LICENSEPENDING
//          Object cannot be used now but there is an outstanding request for 
//          some type of rights renewal
//
//      E_FDRM_CANCELLED
//          User chose not to use object with prompt based rights
//
//      E_FDRM_NOTDRMOBJECT
//          API call failed since the passed in object was not FDRM protected.
//
//      E_FDRM_NOTALLOWED
//          Requested operation is not allowed for this object or object does
//          not have sufficient rights to be used.
//
//      E_FDRM_NOFORWARDRIGHTS
//          Object does not have rights to be forwarded
//
//      E_FDRM_NONOTIFY
//          Object was never registered for notifications
//
//      E_FDRM_NOPROVIDER
//          API call failed since no FDRM provider was installed.
//
//      E_FDRM_UNKNOWNMETRIC
//          Unknown metric type requested from FDRM_GetMetric.
//
//      E_FDRM_ERRORNOTHANDLED
//          Error could not be handled by FDRM_HandleError. 
//
// **************************************************************************

// Define some names for results of HRESULT_FROM_WIN32
#define E_INSUFFICIENT_BUFFER   HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER)
#define E_FILE_NOT_FOUND        HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)

#define FACILITY_FDRM           FACILITY_ITF

// Start HRESULT codes at 512 to avoid conflicting with COM-defined codes
#define CODE_OFFSET             0x0200
#define S_FDRM_NOTDRMOBJECT     MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_FDRM, (CODE_OFFSET + 3))
#define S_FDRM_NOPROVIDER       MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_FDRM, (CODE_OFFSET + 7))

#define E_FDRM_FILEDELETED      MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 0))
#define E_FDRM_LICENSEPENDING   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 1))
#define E_FDRM_CANCELLED        MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 2))
#define E_FDRM_NOTDRMOBJECT     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 3))
#define E_FDRM_NOTALLOWED       MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 4))
#define E_FDRM_NOFORWARDRIGHTS  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 5))
#define E_FDRM_NONOTIFY         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 6))
#define E_FDRM_NOPROVIDER       MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 7))
#define E_FDRM_UNKNOWNMETRIC    MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 8))
#define E_FDRM_ERRORNOTHANDLED  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_FDRM, (CODE_OFFSET + 9))

// **************************************************************************
// Bitmasks for various rights associated with content
// **************************************************************************
#define FDRRF_PLAY           0x00000001      // Play audio/video content
#define FDRRF_DISPLAY        0x00000002      // Display images
#define FDRRF_EXECUTE        0x00000004      // Execute applications
#define FDRRF_PRINT          0x00000008      // Print content
#define FDRRF_FORWARD        0x00000010      // Content can be forwarded    

// **************************************************************************
//  Bitmask flags to control FileDrmGetForwardContent API
// **************************************************************************
#define FDRCF_BARE           0x00000001      // Bare file
#define FDRCF_MIMEENCODED    0x00000002      // Add needed MIME headers

// **************************************************************************
//  Bitmask flags to control FileDrmVerifyRights and FileDrmCommitRights
//
//      FDRVF_NOUI      Supress all UI display. This allows the calling 
//                      application to ensure that this call will not block
//                      waiting for user input.
//
//      FDRVF_NORENEW   Does not attempt to update the rights associated with
//                      a piece of content (either through user interaction
//                      or automatically). 
//                      
//      FDRVF_VERIFY_CONTINUE   Specified when FileDrmCommitRights has been
//                              called and returned success for a piece of
//                              content and the consuming app of the content
//                              is still consuming the same isntance of the 
//                              content but needs to reverify rights. For
//                              example a media player might begin playing a
//                              song but be interrrupted by a phone call. Once
//                              the phone call completes the media player
//                              will call FileDrmVerifyRights to verify that
//                              it still has rights to continue playing that
//                              content. This flag may allow content to continue
//                              being used even if there are no longer valid.
//                              For instance, if the content has restrictions on
//                              the number of times it can be used and that 
//                              usage count went to zero in the last call to
//                              FileDrmCommitRights then the user should still
//                              be allowed to use that content until is has 
//                              been completely consumed.
// **************************************************************************
#define FDRVF_NOUI              0x00000001      // Do not show any UI
#define FDRVF_NORENEW           0x00000002      // No rights renewal
#define FDRVF_VERIFY_CONTINUE   0x00000004      // Verify continued use of content

// Status values returned from FileDrmVerifyRights and FileDrmVerifyRightsEx
#define FDRVS_USER_CAN_UPDATE   0x00000001      // Right invalid but user can update
#define FDRVS_UPDATE_PENDING    0x00000002      // A rights update is pending
#define FDRVS_EXPIRABLE         0x00000004      // Rights are currently valid but can expire

// **************************************************************************
// Bitmask flags to control FileDrmHandleError
// **************************************************************************
// Set by calling app
#define FDRHF_NOCORRECT      0x00000001      // Do not try to correct error

// Set by FileDrmHandleError
#define FDRHF_RETRY          0x00010000      // Calling app should retry op.
#define FDRHF_RIGHTSPENDING  0x00020000      // Rights update pending

// **************************************************************************
// FDRMMETRIC
// 
// Purpose: Metric types that can retrieved using FileDrmGetMetric call
//
// Note: All times are returned in milliseconds
// **************************************************************************
typedef enum tagFDRMMETRIC
{
    FDRM_METRIC_PREVIEW_TIME,       // allowed preview time
    FDRM_METRIC_DELAY_TIME,         // delay before prompting to use rights
    FDRM_METRIC_REVERIFY_TIME       // How often to re-verify rights when using
                                    // content in a continuous manner
} FDRMMETRIC;


// *************************************************************************
//  FILTER Identifiers
// *************************************************************************

// {5A3A54DD-209F-48e6-91DB-6A49F9238413}
DEFINE_GUID(FDRMFT_NONE, 0x5a3a54dd, 0x209f, 0x48e6, 0x91, 0xdb, 0x6a, 0x49, 0xf9, 0x23, 0x84, 0x13);

// {35AB44BA-B7D0-4548-9C3B-6864E9D08281}
DEFINE_GUID(FDRMFT_RINGTONES, 0x35ab44ba, 0xb7d0, 0x4548, 0x9c, 0x3b, 0x68, 0x64, 0xe9, 0xd0, 0x82, 0x81);

// {B7EADA14-F373-4e12-BF17-0722B64362D5}
DEFINE_GUID(FDRMFT_WALLPAPER, 0xb7eada14, 0xf373, 0x4e12, 0xbf, 0x17, 0x7, 0x22, 0xb6, 0x43, 0x62, 0xd5);



// **************************************************************************
// Function Name:   FileDrmIsFDRM
// 
// Purpose: Quickly determines if an object is FDRM protected
//
// Arguments:
//      [IN] pszFile
//          The fully qualified path to the file being checked, including the
//          file extension.
//
//      [OUT] pfDRM
//          A pointer to a BOOL variable which be set to TRUE if the object
//          pointed to by the file is DRM protected; FALSE otherwise
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [SUCCESS]
//          S_FDRM_NOPROVIDER
//              The value pointed to by pfDRM will be FALSE in this case
//
//          S_FDRM_NOTDRMOBJECT
//              The value pointed to by pfDRM will be FALSE in this case
//
// Description:     This function allows an application to quickly determine if
//                  an object is DRM protected. This allows an application to
//                  display an indication that content is DRM protected.
// **************************************************************************
HRESULT FileDrmIsDRM (LPCTSTR pszFile, 
                      PBOOL pfDRM);


// **************************************************************************
// Function Name:   FileDrmCreateFile
// 
// Purpose:         Allow a DRM-aware application to open a DRM protected
//                  object. This function is to be used in place of the 
//                  standard WIN32 CreateFile API.
//
// Arguments:       See CreateFile() for full details of each parameter.
//      [IN] pszFileName
//          The fully qualified path to the file to be opened
//
//      [IN] dwDesiredAccess
//          Type of access to the object.
//
//      [IN] dwShareMode
//          Share mode for the object.
//
//      [IN] lpSecurityAttributes
//          Ignored; set to NULL
//
//      [IN] dwCreationDisposition
//          How to handle existing/new files
//
//      [IN] dwFlagsAndAttributes
//          File attributes for the file.
//
//      [IN] hTemplateFile
//          Ignored; set to NULL
//
//      [OUT] phFile
//          Pointer to a handle variable that will hold the file handle if the
//          call is successful.    
//
// Return Values:
//      In addition to the HRESULT return code the value pointed to by phFile
//      must be set to INVALID_HANDLE_VALUE if an error is returned.
//
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [SUCCESS]
//          S_FDRM_NOPROVIDER
//              The returned rights structure will allow for all rights.
//
//          S_FDRM_NOTDRMOBJECT
//              The returned rights structure will allow for all rights.
//
// Side effects:    File is opened or created as appropriate.
//
// Notes: Use CloseHandle to close the handle returned from this function.
// 
// Description:
//      This function is to be used in place of the standard WIN32 CreateFile
//      API by applications that are "FDRM Aware". With some exceptions as
//      detailed below It behaves the same way as CreateFile and returns any 
//      error that occurs during the call as an the HRESULT created by 
//      HRESULT_FROM_WIN32 called on the value returned from GetLastError().
//
//      When the following conditions are true the FileDrmCreateFile works
//      differently than CreateFile:
//
//      Case 1: Opening FDRM content for use
//
//          1. An FDRM provider is installed.
//          2. The file being created already exists and contains FDRM
//             protected content.
//          3. dwDesiredAccess is set to GENERIC_READ
//          4. dwCreationDisposition is set to OPEN_EXISTING
//
//          In this case FileDrmCreateFile opens the file in such a way that 
//          subsequent calls to ReadFile will return the unencrypted version
//          of the files content such that it can be used by the application.
//          
//      Case 2: Attempting to open FDRM content for write access
//
//          1. An FDRM provider is installed.
//          2. The file being created already exists and contains FDRM
//             protected content
//          3. dwDesiredAccess is set to GENERIC_WRITE
//
//          In this case FileDrmCreateFile will fail to open the file (to 
//          prevent applications from inadvertently overwriting protected
//          content).
//
//      Case 3: Attempting to delete/truncate FDRM content
//
//          1. An FDRM provider is installed.
//          2. The file being created already exists and contains FDRM
//             protected content
//          3. dwCreationDisposition is set to CREATE_ALWAYS or 
//             TRUNCATE_EXISTING.
//
//          In this case FileDrmCreateFile will fail to open the file (to 
//          prevent applications from inadvertently overwriting protected
//          content).
// **************************************************************************
HRESULT FileDrmCreateFile (LPCTSTR pszFileName,
                           DWORD dwDesiredAccess,
                           DWORD dwShareMode,
                           LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                           DWORD dwCreationDisposition,
                           DWORD dwFlagsAndAttributes,
                           HANDLE hTemplateFile,
                           PHANDLE phFile);


// **************************************************************************
// Function Name:   FileDrmCreateForwardableContent
// 
// Purpose: Create a file containing a DRM protected object in a way that 
//          allows it to be forwarded to another user.
//
// Arguments:
//      [IN] pszFileName
//          The fully qualified path to the file to be forwarded.
//
//      [IN] dwFlags
//          Flags to control the format of the generated document. See the 
//          DRCF_* flags for defined values
//
//      [IN] pszOutputFile
//          The fully qualified path for a file to hold the document
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [FAILURE]
//          E_FDRM_NOPROVIDER
//          E_FDRM_NOTDRMOBJECT
//          E_FDRM_NOFORWARDRIGHTS
//
// Side effects:    A new file is created holding the DRM protected content
// 
// Description:
//      Applications that want to forward content can use this API to package
//      format in such a way that it can be sent to another device.
// **************************************************************************
HRESULT FileDrmCreateForwardableContent (LPCTSTR pszFileName, 
                                         DWORD dwFlags,
                                         LPCTSTR pszOutputFile);



// **************************************************************************
// Function Name:   FileDrmNotifyEnable
// 
// Purpose: Register an application with the DRM provider for updates when
//          updated rights are received on the device.
//
// Arguments:
//      [IN] pszFileName 
//          Fully qualified path to the file to receive notifications for
//
//      [OUT] pszValueName
//          Pointer to a buffer to be filled with the name of a registry 
//          DWORD value that will be updated when the rights associated with 
//          the object change. The returned string must always be terminated
//          with a NUL character as long as cchValueLen is >= 1
//
//      [IN] cchValueLen 
//          Length of the pszValueName buffer. Uaing a buffer of size 
//          MAX_PATH will always be sufficient. Passing in a smaller buffer
//          will cause this function to return E_INSUFFICIENT_BUFFER if the
//          buffer is too small to hold the returned string (including the
//          NUL terminator).
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [FAILURE]
//          E_FDRM_NOPROVIDER
//          E_FDRM_NOTDRMOBJECT
//          E_INSUFFICIENTBUFFER
//
// Side effects:
//      On success the psValueName will be filled in with the name of a 
//      registry value that can be monitored by the application. A new 
//      registry value may have been created in the registry. These values 
//      will be under the key HKEY_CURRENT_USER\FDRM\Monitor.
// 
// Note:
//      Notification of expiration of time or count based rights is not
//      supported.
//
// Description:
//      An application that needs to know when a rights renewal has arrived
//      can use this function along with the RegistryNotifyxxxx functions to 
//      receive notifications from the DRM provider.
//
//      The DRM provider will take the passed in filename and generate the 
//      name of a registy value that will be used as a proxy for the file. 
//      The calling application can then register for registry notifications 
//      using this value.  When the DRM provider changes the rights store it 
//      will also write the new rights value (using the DRRF_* values) 
//      to that registry value.
//
//      The DRM provider will reference count calls to FileDrmNotifyEnable
//      and FileDrmNotifyDisable and only stop updating and remove the 
//      registry value when the reference count goes to 0. As well it will
//      monitor processes and remove all outstanding references to the 
//      notification upon process exit.
//
//      Registrations by multiple processes or threads within a single 
//      process that refer to the same filename must return the same registry
//      value name to be monitored.
//
//      The provider will also monitor calls to DeleteFile and remove any
//      outstanding notifications when a FDRM protected file is deleted.
// **************************************************************************
HRESULT FileDrmNotifyEnable (LPCTSTR pszFileName, 
                             LPTSTR pszValueName,
                             DWORD cchValueNameLen);


// **************************************************************************
// Function Name:   FileDrmNotifyDisable
// 
// Purpose: Allows the DRM provider to stop receiving notifications of
//          changes to the rights associated with a file.
//
// Arguments:
//      [IN] pszFileName 
//          Fully qualified path to the file that the application no longer 
//          wants to receive notifications for.
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [FAILURE]
//          E_FDRM_NOPROVIDER
//          E_FDRM_NOTDRMOBJECT
//          E_FDRM_NONOTIFY
//
// Description:
//      See description of FileDrmNotifyEnable
// **************************************************************************
HRESULT FileDrmNotifyDisable (LPCTSTR pszFileName);


// **************************************************************************
// Function Name:   FileDrmShowLicenseInfo
// 
// Purpose: Shows the rights associated with a DRM protected object
//
// Arguments:
//      [IN] hwndParent
//          Handle to the window that owns any UI elements (windows, dialog
//          boxes, or message boxes) displayed by this API.
//
//      [IN] pszFileName 
//          The path of the file to show license information for.
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [SUCCESS]
//          S_FDRM_NOPROVIDER
//          S_FDRM_NOTDRMOBJECT
//
// Description:
//      A dialog is presented to the user showing the rights associated with 
//      the file. This function returns when the user dismisses the dialog.
// **************************************************************************
HRESULT FileDrmShowLicenseInfo (HWND hwndParent,
                                LPCTSTR pszFileName);


// **************************************************************************
// Function Name:   FileDrmHandleError
// 
// Purpose: Displays an error dialog to the user when an error occurs while 
//          trying to use DRM content.
//
// Arguments:
//      [IN] hwndParent
//          Handle to the window that owns any UI elements (windows, dialog
//          boxes, or message boxes) displayed by this API.
//
//      [IN] hrError
//          The HRESULT error to display/handle
//
//      [IN] pszFileName 
//          The file associated with the error
//
//      [IN] dwFlags 
//          Flags controlling the API behavior. A bitmask comprised of FDRHF_* flags.
//
//      [OUT] pdwStatus
//          Status returned by FileDRMHandleError. A bitmask comprised ofr FDRHF_* flags.
//
// Return Values:
//      HRESULT
//          The function will only return the following HRESULTS. The 
//          application must use SUCCEEDED/FAILED macros to check the return.
//
//      [SUCCESS]
//          S_OK
//              The application may wish to check the FDRHF_RETRY and 
//              FDRHF_RIGHTSPENDING bits in *pdwFlags to control additional 
//              handling of the error.
//          
//      [FAILURE]
//          E_FDRM_ERRORNOTHANDLED
//              Applicaton must handle the error. This will occur either 
//              because the FDRM provider cannot handle the error or does not
//              recognize the error.
//
// Side effects:
//      If a rights renewal is requested by the user a network connection may
//      be established to send the renewal. Once the response is received a 
//      notification will be sent to registered applications.
// 
// Description:
//      When an error occurs while using one of the FDRM APIs this API can be
//      called to display error information to the user and (optionally) 
//      attempt to correct the error (most likely by offering the user the 
//      option to acquire additional rights or extend existing rights. If the
//      user requests new/updated rights the new/updated rights will arrive 
//      asynchronously. If the FDRHF_RETRY flag is set upon return from this 
//      function the application may retry the operation that failed.
// **************************************************************************
HRESULT FileDrmHandleError (HWND            hwndParent, 
                            HRESULT         hrError,   
                            LPCTSTR         pszFileName,
                            DWORD           dwFlags,
                            PDWORD          pdwStatus);


// **************************************************************************
// Function Name:   FileDrmRenewRights
// 
// Purpose: Renew the rights associated with DRM protected object
//
// Arguments:
//      [IN] hwndParent
//          Handle to the window that owns any UI elements (windows, dialog
//          boxes, or message boxes) displayed by this API.
//
//      [IN] pszFileName
//          Path to the file containing the DRM protected object
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [SUCCESS]
//          S_FDRM_NOPROVIDER
//          S_FDRM_NOTDRMOBJECT
//
//      [FAILURE]
//          E_FDRM_NOTALLOWED
//              Rights renewal is not allowed for this object type
//
// Side effects:
//      A rights renewal request is generated. This may cause a network 
//      connection to be opened
// 
// Description:
//      This API allows an application to renew rights associated with a DRM 
//      protected object. In most cases this will be handled by the DRM 
//      provider when FileDrmHandleError is called. New/updated rights will 
//      arrive asynchronously.
// **************************************************************************
HRESULT FileDrmRenewRights (HWND hwndParent,
                            LPCTSTR pszFileName);


// **************************************************************************
// Function Name:   FileDrmGetMetric
// 
// Purpose: Retrieves metrics related to using DRM protected objects
//
// Arguments:
//      [IN] drmID
//          Identifier to select which metric you want.
//
//      [OUT] pdwMetric
//          Pointer to a dword variable that will hold the metric
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [FAILURE]
//          E_FDRM_UNKNOWNMETRIC
//
// **************************************************************************
HRESULT FileDrmGetMetric (FDRMMETRIC drmID,
                          DWORD *pdwMetric);

// **************************************************************************
// Function Name:   FileDrmVerifyRights
// 
// Purpose: Allows an application to verify that an object has needed rights 
//          before using DRM protected objects
//
// Arguments:
//      [IN] hwndParent
//          Handle to the window that owns any UI elements (windows, dialog
//          boxes, or message boxes) displayed by this API.
//
//      [IN] pszFileName
//          Path to the file containing the DRM protected object
//
//      [IN] dwRight
//          The right the application wants to use. Only one right may be specified.
//
//      [IN] dwFlags
//          Flags to control the behavior of FDRM_VerifyRights. This is a 
//          bitmap of FDRVF_* flag values.
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [SUCCESS]
//          S_FDRM_NOPROVIDER
//          S_FDRM_NOTDRMOBJECT
//
//      [FAILURE]
//          E_FDRM_FILEDELETED
//          E_FDRM_CANCELLED
//          E_FDRM_LICENSEPENDING
//          E_FDRM_NOTALLOWED
//
// Description:
//      Before an app can use a DRM protected object it must call this API
//      to ensure that the object can be used as intended. This API will
//      verify if the object has the appropriate rights and if necessary will
//      prompt the user if they want to consume rights or renew expired
//      rights. 
// **************************************************************************
HRESULT FileDrmVerifyRights (HWND           hwndParent,
                             LPCTSTR        pszFileName, 
                             DWORD          dwRight,
                             DWORD          dwFlags,
                             PDWORD         pdwRightsStatus);


// **************************************************************************
// Function Name:   FileDrmCommitRights
// 
// Purpose: Allows an application to commits the rights used
//
// Arguments:
//      [IN] hFile
//          Handle to the file containing the object to be used. File must 
//          have been opened using FileDrmCreateFile ().
//
//      [IN] dwRight
//          The right the application wants to use. Only one right may be specified.
//
//      [IN] dwFlags
//          Flags to control the behavior of FileDrmCommitRights. This is a 
//          bitmap of FDRVF_* flag values.
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [SUCCESS]
//          S_FDRM_NOPROVIDER
//          S_FDRM_NOTDRMOBJECT
//
//      [FAILURE]
//          E_FDRM_NOTALLOWED
//          E_FDRM_LICENSEPENDING
//
// Description: Commits the rights associated with an object. The calling
//              application must call FileDrmVerifyRights before this call.
// **************************************************************************
HRESULT FileDrmCommitRights (HANDLE hFile, 
                             DWORD dwRight,
                             DWORD dwFlags);


// **************************************************************************
// Function Name:   FileDrmDeleteFile
// 
// Purpose:         Delete a DRM object as well as the associated rights
//
// Arguments:
//      [IN] pszFileName
//          Path to the file containing the DRM protected object
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [SUCCESS]
//          S_FDRM_NOPROVIDER
//          S_FDRM_NOTDRMOBJECT
//
// Side effects:
//      Rights store updated. File containing object deleted.
// 
// Description:
//      This API deletes both a DRM object and the associated rights.
// **************************************************************************
HRESULT FileDrmDeleteFile (LPCTSTR pszFileName);


// **************************************************************************
// Function Name:   FileDrmStoreContent
// 
// Purpose: Read a file containing MIME-encoded content to be DRM protected 
//          and create the device-locked version of that content as well as 
//          any associated rights or pending rights.
//
// Arguments:
//      [IN] pszInputFileName
//          Fully qualified path to the file containing MIME to be cracked. 
//          This file starts with the Content-Type ("Content-type: …")
//          optionally preceeded by linear-white-space
//
//      [IN] pszOutputFileName
//          Fully qualified path to the file to hold the object contained 
//          within the MIME message
//
//      pvReserved
//          Reserved. Must be set to NULL
//
//      [OUT] pszMimeType
//          A pointer to the buffer to be filled in with the type of the 
//          embedded object ("image/jpeg"). If this pointer is NULL then this
//          call is to be treated as a request to obtain the size of the
//          buffer needed to hold the MIME type string.
//
//      [IN/OUT] pcchMimeTypeLen
//          A pointer to a DWORD holding the length of the pszMimeType buffer
//          in characters. On entry this value will be the size of the passed
//          buffer. On exit this will be set to the length of the MIME type
//          string including the NUL terminator.
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [FAILURE]
//          E_FDRM_NOPROVIDER
//          E_INSUFFICIENT_BUFFER
//              Output file not created and *pcchMimeTypeLen updated with 
//              needed buffer size
//
// Side effects:
//      A new file is created holding the DRM protected content. The original
//      file is deleted.
// **************************************************************************
HRESULT FileDrmStoreContent (LPCTSTR pszInputFileName, 
                             LPCTSTR pszOutputFileName,
                             LPVOID  pvReserved,
                             LPTSTR  pszMimeType,
                             DWORD  *pcchMimeTypeLen);

// **************************************************************************
// Function Name:   FileDrmVerifyRightsEx
// 
// Purpose: Allows an application to verify that an object has needed rights 
//          before using DRM protected objects. Also allows the user to filter
//          content by its intended use.
//
// Arguments:
//      [IN] hwndParent
//          Handle to the window that owns any UI elements (windows, dialog
//          boxes, or message boxes) displayed by this API.
//
//      [IN] pszFileName
//          Path to the file containing the DRM protected object
//
//      [IN] dwRight
//          The right the application wants to use. Only one right may be
//          specified.
//
//      [IN] dwFlags
//          Flags to control the behavior of FDRM_VerifyRights. This is a 
//          bitmap of FDRVF_* flag values.
//
//      [OUT} pdwRightsStatus
//          Additional information about the status of the right
//          associated with the content. One of the FDRVS_* values
//
//          FDRVS_USER_CAN_UPDATE   Right invalid but user can update
//          FDRVS_UPDATE_PENDING    A rights update is pending
//          FDRVS_EXPIRABLE         Right is currently valid but can
//                                  expire later
//      [IN] pGuidFilter
//          An indication of the intended purpose of the content. If
//          the content pointed to by pszFileName is DRM protected this
//          filter value is checked as well as the rights associated
//          with the content to determine if the content can be used. 
//  
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [SUCCESS]
//          S_FDRM_NOPROVIDER
//          S_FDRM_NOTDRMOBJECT
//
//      [FAILURE]
//          E_FDRM_FILEDELETED
//          E_FDRM_CANCELLED
//          E_FDRM_LICENSEPENDING
//          E_FDRM_NOTALLOWED
//
// Description:
//      Either this or the FileDrmVerifyRights API must be called before an
//      app can use a DRM protected object. This API will verify if the
//      object has the appropriate rights and if necessary will prompt the
//      user if they want to consume rights or renew expired rights.
// **************************************************************************
HRESULT FileDrmVerifyRightsEx (HWND     hwndParent,
                               LPCTSTR  pszFileName, 
                               DWORD    dwRight,
                               DWORD    dwFlags,
                               PDWORD   pdwRightsStatus,
                               LPGUID   pguidFilter);


// **************************************************************************
// Function Name:   FileDrmCreateForwardableContentEx
// 
// Purpose: Create a file containing a DRM protected object in a way that 
//          allows it to be forwarded to another user. Decide on the extension
//          to use for the forwardable file based on the DRM version.
//
// Arguments:
//      [IN] pszFileName
//          The fully qualified path to the file to be forwarded.
//
//      [IN] dwFlags
//          Flags to control the format of the generated document. See the 
//          DRCF_* flags for defined values.
//
//      [IN] pszOutputFolder
//           The fully qualified path to the folder in which to store the
//           generated forwardable file. (Does not include the file name.)
//
//      [OUT] pszOutputFile
//            Pointer to a buffer to be filled with the fully qualified path 
//            for the generated forwardable file. Should be large enough to 
//            hold pszOutputFolder concatenated with the file name portion
//            on pszFileName.
//
//      [IN]  dwOutputFileLen
//            Length of the buffer pszOutputFile in characters. A length of 
//            MAX_PATH (in characters) should always suffice.
//            
//
// Return Values:
//      HRESULT
//          The function is free to return any HRESULT and the application 
//          should use the SUCCEEDED/FAILED macros to check the results. In
//          addition the following HRESULT codes may be returned
//
//      [FAILURE]
//          E_FDRM_NOPROVIDER
//          E_FDRM_NOTDRMOBJECT
//          E_FDRM_NOFORWARDRIGHTS
//
// Side effects:    A new file is created holding the DRM protected content
// 
// Description:
//      Applications that want to forward content can use this API to package
//      format in such a way that it can be sent to another device.
// **************************************************************************
HRESULT FileDrmCreateForwardableContentEx (LPCTSTR pszFileName, 
                                           DWORD dwFlags,
                                           LPCTSTR pszOutputFolder,
                                           LPTSTR pszOutputFile,
                                           DWORD dwOutputFileLen);


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // FDRM_H


