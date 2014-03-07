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
#pragma once

#ifndef __TODAYPLUGIN_HPP__
#define __TODAYPLUGIN_HPP__

#include <windows.h>
#include <Todaycmn.h>


class TodayVoIPPlugin_t
{
public:
    //constants
    static const WCHAR sc_PluginName[];     // plugin name

private:
    WCHAR m_VoIPServiceStatus[MAX_PATH];    // current status of the VoIP service
    HICON m_Icon;                           // icon displayed for this plugin
    HWND m_TodayItemWindow;                 // window
    HFONT m_TodayItemFont;                  // font to be used for today screen items
    int m_TodayItemHeight;                  // height of the today item
    BOOL m_Selected;

public:
    //static WNDPROC function for the today item window
    static
    LRESULT WINAPI
    s_TodayItemWindowProc(
        HWND Window,
        UINT Message,
        WPARAM wParam,
        LPARAM lParam
        );

    // Constructor/Destructor
    TodayVoIPPlugin_t();
    ~TodayVoIPPlugin_t();

    // Initialize the plugin
    void
    InitializePlugin(
        HWND TodayItemWindow
        );

private:
    // Handle user action 
    void
    OnAction(
        void
        );

    // Handler for WM_PAINT messages
    BOOL
    OnPaint(
        void
        );

    // Handler for WM_TODAYCUSTOM_QUERYREFRESHCACHE messages
    BOOL
    OnQueryRefreshCache(
        __in TODAYLISTITEM* pTodayListItem
        );
};

#endif /* __TODAYPLUGIN_HPP__ */
