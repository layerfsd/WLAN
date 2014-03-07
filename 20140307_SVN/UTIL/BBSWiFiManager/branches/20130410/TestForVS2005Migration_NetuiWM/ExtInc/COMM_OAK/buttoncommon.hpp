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
#ifndef __BUTTONCOMMON_HPP_INCLUDED__
#define __BUTTONCOMMON_HPP_INCLUDED__

const int GWL_BTNSTATE =    0;
const int GWL_BTNFONT  =    4;

// Buttons
const unsigned int BS_HORZMASK =    0x00000300;
const unsigned int BS_TYPEMASK =    0x000F;     // bStyle
const unsigned int HOR_ALIGN_MASK = 0x00000300;
const unsigned int VER_ALIGN_MASK = 0x00000C00;

const unsigned short BST_CHECKMASK   =   0x0003;

const unsigned short BST_INCLICK     =   0x0010;
const unsigned short BST_CAPTURED    =   0x0020;
const unsigned short BST_MOUSE       =   0x0040;
const unsigned short BST_DONTCLICK   =   0x0080;
const unsigned short BST_INBMCLICK   =   0x0100;


/* DrawBtnText codes */
const unsigned int DBT_TEXT =  0x0001;
const unsigned int DBT_FOCUS = 0x0002;
const unsigned int PBF_DEFAULT =     0x0002;
const unsigned int PBF_PUSHABLE =    0x0001;


unsigned 
int
TestWindowFlag(
    HWND            hwnd,
    unsigned int    flag
    );


#endif /* __BUTTONCOMMON_HPP_INCLUDED__ */
