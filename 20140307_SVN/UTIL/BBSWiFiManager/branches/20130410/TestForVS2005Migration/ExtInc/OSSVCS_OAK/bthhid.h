#pragma once

#include <windows.h>
#include <bt_api.h>

#define BTM_IOCTL_REGISTRY_INSTALL      0x1100
#define BTM_IOCTL_REGISTRY_UNINSTALL    0x1101
#define BTM_IOCTL_NEW_DEVICE            0x1102
#define BTM_IOCTL_DEVICE_REMOVED        0x1103
#define BTM_IOCTL_DEVICE_SHUTDOWN       0x1104
#define BTM_IOCTL_GET_HANDLE            0x1105
#define BTM_IOCTL_SET_HANDLE            0x1106

#define BTD_IOCTL_NEWCONNECTION         0x1200
#define BTD_IOCTL_SHUTDOWN              0x1201

union BTHHIDMANAGER_PARAMS
{
    struct 
    {
        BT_ADDR ba;
    } IOCTL_NEW_DEVICE_p;

    struct 
    {
        BT_ADDR ba;
    } IOCTL_DEVICE_REMOVED_p;

    struct 
    {
        BT_ADDR ba;
    } IOCTL_DEVICE_SHUTDOWN_p;

    struct 
    {
        HANDLE hHandle;
    } BTM_IOCTL_SET_HANDLE_p;
};

union BTHHIDMANAGER_OUTPARAMS
{
    struct
    {
        HANDLE hHandle;
    } BTM_IOCTL_GET_HANDLE_p;
};


union BTHHIDDEVICE_PARAMS
{
    struct 
    {
        unsigned short usCIDControl;
        unsigned short usCIDInterrupt;
        unsigned short usMTUControl;
        unsigned short usMTUInterrupt;
    } IOCTL_NEWCONNECTION_p;
};

// $TODO: moveto bt_sdp.h
DEFINE_GUID(HIDServiceClass_UUID,  0x00001124, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB);
#define HIDServiceClass_UUID16     0x1124
