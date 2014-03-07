//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
// Copyright (c) Microsoft Corporation.  All rights reserved. 
//
// Use of this source code is subject to the terms of your Windows Mobile 
// Software Shared Source Premium Derivatives License Form or other 
// applicable Microsoft license agreement for the software. If you did not 
// accept the terms of such a license, you are not authorized to use this 
// source code.

#pragma once

// IWebBrowser
#define DISPID_WEBBROWSER_GOBACK                1
#define DISPID_WEBBROWSER_GOFORWARD             2
#define DISPID_WEBBROWSER_GOHOME                3
#define DISPID_WEBBROWSER_GOSEARCH              4
#define DISPID_WEBBROWSER_NAVIGATE              5
#define DISPID_WEBBROWSER_REFRESH               6
#define DISPID_WEBBROWSER_REFRESH2              7
#define DISPID_WEBBROWSER_STOP                  8
#define DISPID_WEBBROWSER_APPLICATION           10
#define DISPID_WEBBROWSER_PARENT                11
#define DISPID_WEBBROWSER_CONTAINER             12
#define DISPID_WEBBROWSER_DOCUMENT              13
#define DISPID_WEBBROWSER_TOPLEVELCONTAINER     14
#define DISPID_WEBBROWSER_TYPE                  15
#define DISPID_WEBBROWSER_LEFT                  16
#define DISPID_WEBBROWSER_TOP                   17
#define DISPID_WEBBROWSER_WIDTH                 18
#define DISPID_WEBBROWSER_HEIGHT                19
#define DISPID_WEBBROWSER_LOCATIONNAME          20
#define DISPID_WEBBROWSER_LOCATIONURL           21
#define DISPID_WEBBROWSER_BUSY                  22

// IWebBrowserApp
#define DISPID_WEBBROWSER_QUIT                  30
#define DISPID_WEBBROWSER_CLIENTTOWINDOW        31
#define DISPID_WEBBROWSER_PUTPROPERTY           32
#define DISPID_WEBBROWSER_GETPROPERTY           33
#define DISPID_WEBBROWSER_NAME                  34
#define DISPID_WEBBROWSER_HWND                  25
#define DISPID_WEBBROWSER_FULLNAME              36
#define DISPID_WEBBROWSER_PATH                  37
#define DISPID_WEBBROWSER_VISIBLE               38
#define DISPID_WEBBROWSER_STATUSBAR             39
#define DISPID_WEBBROWSER_STATUSTEXT            40
#define DISPID_WEBBROWSER_TOOLBAR               41
#define DISPID_WEBBROWSER_MENUBAR               42
#define DISPID_WEBBROWSER_FULLSCREEN            43

// IWebBrowser2
#define DISPID_WEBBROWSER_NAVIGATE2             50
#define DISPID_WEBBROWSER_QUERYSATUSWB          51
#define DISPID_WEBBROWSER_EXECWB                52
#define DISPID_WEBBROWSER_SHOWBROWSERBAR        53
#define DISPID_WEBBROWSER_READYSTATE            54
#define DISPID_WEBBROWSER_OFFLINE               55
#define DISPID_WEBBROWSER_SILENT                56
#define DISPID_WEBBROWSER_REGISTERASBROWSER     57
#define DISPID_WEBBROWSER_REGISTERASDROPTARGET  58
#define DISPID_WEBBROWSER_THEATERMODE           59
#define DISPID_WEBBROWSER_ADDRESSBAR            60
#define DISPID_WEBBROWSER_RESIZABLE             61

// DWebBrowserEvents
#define DISPID_BEFORENAVIGATE               100     // This is sent before navigation to give a chance to abort
#define DISPID_NAVIGATECOMPLETE             101     // in async, this is sent when we have enough to show
#define DISPID_STATUSTEXTCHANGE             102
#define DISPID_QUIT                         103
#define DISPID_DOWNLOADCOMPLETE             104
#define DISPID_COMMANDSTATECHANGE           105
#define DISPID_DOWNLOADBEGIN                106
#define DISPID_NEWWINDOW                    107     // sent when a new window should be created
#define DISPID_PROGRESSCHANGE               108     // sent when download progress is updated
#define DISPID_WINDOWMOVE                   109     // sent when main window has been moved
#define DISPID_WINDOWRESIZE                 110     // sent when main window has been sized
#define DISPID_WINDOWACTIVATE               111     // sent when main window has been activated
#define DISPID_PROPERTYCHANGE               112     // sent when the PutProperty method is called
#define DISPID_TITLECHANGE                  113     // sent when the document title changes
#define DISPID_TITLEICONCHANGE              114     // sent when the top level window icon may have changed.

#define DISPID_FRAMEBEFORENAVIGATE          200
#define DISPID_FRAMENAVIGATECOMPLETE        201
#define DISPID_FRAMENEWWINDOW               204

#define DISPID_BEFORENAVIGATE2              250     // hyperlink clicked on
#define DISPID_NEWWINDOW2                   251
#define DISPID_NAVIGATECOMPLETE2            252     // UIActivate new document
#define DISPID_ONQUIT                       253
#define DISPID_ONVISIBLE                    254     // sent when the window goes visible/hidden
#define DISPID_ONTOOLBAR                    255     // sent when the toolbar should be shown/hidden
#define DISPID_ONMENUBAR                    256     // sent when the menubar should be shown/hidden
#define DISPID_ONSTATUSBAR                  257     // sent when the statusbar should be shown/hidden
#define DISPID_ONFULLSCREEN                 258     // sent when kiosk mode should be on/off
#define DISPID_DOCUMENTCOMPLETE             259     // new document goes ReadyState_Complete
#define DISPID_ONTHEATERMODE                260     // sent when theater mode should be on/off
#define DISPID_ONADDRESSBAR                 261     // sent when the address bar should be shown/hidden
#define DISPID_WINDOWSETRESIZABLE           262     // sent to set the style of the host window frame
#define DISPID_WINDOWCLOSING                263     // sent before script window.close closes the window 
#define DISPID_WINDOWSETLEFT                264     // sent when the put_left method is called on the WebOC
#define DISPID_WINDOWSETTOP                 265     // sent when the put_top method is called on the WebOC
#define DISPID_WINDOWSETWIDTH               266     // sent when the put_width method is called on the WebOC
#define DISPID_WINDOWSETHEIGHT              267     // sent when the put_height method is called on the WebOC 
#define DISPID_CLIENTTOHOSTWINDOW           268     // sent during window.open to request conversion of dimensions
#define DISPID_SETSECURELOCKICON            269     // sent to suggest the appropriate security icon to show
#define DISPID_FILEDOWNLOAD                 270     // Fired to indicate the File Download dialog is opening
#define DISPID_NAVIGATEERROR                271     // Fired to indicate the a binding error has occured
#define DISPID_PRIVACYIMPACTEDSTATECHANGE   272     // Fired when the user's browsing experience is impacted

// Printing events
#define DISPID_PRINTTEMPLATEINSTANTIATION   225   // Fired to indicate that a print template is instantiated
#define DISPID_PRINTTEMPLATETEARDOWN        226   // Fired to indicate that a print templete is completely gone 
#define DISPID_UPDATEPAGESTATUS             227   // Fired to indicate that the spooling status has changed

// IWebBrowserExternal
#define DISPID_WEBBROWSEREXT_FAVORITES      228
#define DISPID_WEBBROWSEREXT_HISTORY        229
#define DISPID_WEBBROWSEREXT_SEARCH         230
#define DISPID_WEBBROWSEREXT_OPENFAVORITES  231
#define DISPID_WEBBROWSEREXT_OPENHISTORY    232

// IWebBrowserFavorite
#define DISPID_WEBBROWSERFAV_NAME           233
#define DISPID_WEBBROWSERFAV_URL            234
#define DISPID_WEBBROWSERFAV_GOTO           235

// IWebBrowserHistoryEntry
#define DISPID_WEBBROWSERHIST_NAME          236
#define DISPID_WEBBROWSERHIST_URL           237
#define DISPID_WEBBROWSERHIST_GOTO          238

#define DISPID_AMBIENT_DLCONTROL            (-5512)
#define DLCTL_DLIMAGES                      0x00000010
#define DLCTL_VIDEOS                        0x00000020
#define DLCTL_BGSOUNDS                      0x00000040
#define DLCTL_NO_SCRIPTS                    0x00000080
#define DLCTL_NO_JAVA                       0x00000100
#define DLCTL_NO_RUNACTIVEXCTLS             0x00000200
#define DLCTL_NO_DLACTIVEXCTLS              0x00000400
#define DLCTL_DOWNLOADONLY                  0x00000800
#define DLCTL_NO_FRAMEDOWNLOAD              0x00001000
#define DLCTL_RESYNCHRONIZE                 0x00002000
#define DLCTL_PRAGMA_NO_CACHE               0x00004000
#define DLCTL_NO_BEHAVIORS                  0x00008000
#define DLCTL_NO_METACHARSET                0x00010000
#define DLCTL_URL_ENCODING_DISABLE_UTF8     0x00020000
#define DLCTL_URL_ENCODING_ENABLE_UTF8      0x00040000
#define DLCTL_FORCEOFFLINE                  0x10000000
#define DLCTL_NO_CLIENTPULL                 0x20000000
#define DLCTL_SILENT                        0x40000000
#define DLCTL_OFFLINEIFNOTCONNECTED         0x80000000
#define DLCTL_OFFLINE                       DLCTL_OFFLINEIFNOTCONNECTED


