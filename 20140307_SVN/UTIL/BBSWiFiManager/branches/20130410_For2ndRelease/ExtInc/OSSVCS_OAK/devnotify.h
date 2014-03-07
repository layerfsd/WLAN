#ifndef DEVNOTIFY_H
#define DEVNOTIFY_H

#define FILE_DEVICE_HEADSET                 32768
#define IOCTL_HEADSET_ADD_NOTIFY_HWND       CTL_CODE( FILE_DEVICE_HEADSET, 2048, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IOCTL_HEADSET_REMOVE_NOTIFY_HWND    CTL_CODE( FILE_DEVICE_HEADSET, 2049, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define FILE_DEVICE_CARKIT                  32769
#define IOCTL_CARKIT_ADD_NOTIFY_HWND	    CTL_CODE( FILE_DEVICE_CARKIT, 2050, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IOCTL_CARKIT_REMOVE_NOTIFY_HWND     CTL_CODE( FILE_DEVICE_CARKIT, 2051, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define FILE_DEVICE_SPEAKER                 32770
#define IOCTL_SPEAKER_ADD_NOTIFY_HWND	    CTL_CODE( FILE_DEVICE_SPEAKER, 2052, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IOCTL_SPEAKER_REMOVE_NOTIFY_HWND    CTL_CODE( FILE_DEVICE_SPEAKER, 2053, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IOCTL_SPEAKER_GETMODE            CTL_CODE( FILE_DEVICE_SPEAKER, 2054, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IOCTL_SPEAKER_SETMODE            CTL_CODE( FILE_DEVICE_SPEAKER, 2055, METHOD_BUFFERED, FILE_ANY_ACCESS )

typedef enum
{
    SPK_NORMAL,
    SPK_SPEAKER,
    
    SPK_LAST // for bounds checking
} SPEAKERMODE;

typedef struct tag_DEVNOTIFY_EVENT
{
	HWND							hWndNotify;
	UINT							Msg;
}DEVNOTIFY_EVENT, *PDEVNOTIFY_EVENT;

#endif	//DEVNOTIFY_H
