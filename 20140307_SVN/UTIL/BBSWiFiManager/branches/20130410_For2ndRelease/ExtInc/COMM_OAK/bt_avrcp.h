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
#ifndef __BT_AVRCP_H__
#define __BT_AVRCP_H__

typedef struct _AvrcpMsg {
    uint    OpCode;
    uint    OpId;
    uint    Reserved[4];
} AvrcpMsg;

enum AVCOpCodes {
    UnitInfo_t = 0x30,
    SubunitInfo_t,
    PassThru_t = 0x7C
};

enum PassThruOpId {
    Select_t = 0x00,
    Up_t,
    Down_t,
    Left_t,
    Right_t,
    RightUp_t,
    RightDown_t,
    LeftUp_t,
    LeftDown_t,
    RootMenu_t,
    SetupMenu_t,
    ContentsMenu_t,
    FavMenu_t,
    Exit_t,

    ChannelUp_t = 0x30,
    ChannelDown_t,
    PrevChannel_t,
    SoundSel_t,
    InputSel_t,
    DisplayInfo_t,
    Help_t,
    PageUp_t,
    PageDown_t,
    
    
    Power_t = 0x40,
    VolUp_t,
    VolDown_t,
    Mute_t,
    Play_t,
    Stop_t,
    Pause_t,
    Record_t,
    Rewind_t,
    FastFwd_t,
    Eject_t,
    Forward_t,
    Backward_t

};

extern "C" __declspec (selectany) const WCHAR szAvrcpMsgQName[] = L"Avrcp10MsgQ";


#endif // __BT_AVRCP_H__
