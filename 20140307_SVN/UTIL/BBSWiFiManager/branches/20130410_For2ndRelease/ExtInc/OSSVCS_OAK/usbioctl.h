#ifndef USBIOCTL_H
#define USBIOCTL_H

#define FILE_DEVICE_USB			32768
#define IOCTL_SET_MODEM_MODE	CTL_CODE( FILE_DEVICE_USB, 2049, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IOCTL_SET_SYNC_MODE		CTL_CODE( FILE_DEVICE_USB, 2050, METHOD_BUFFERED, FILE_ANY_ACCESS )


#endif	//USBIOCTL_H
