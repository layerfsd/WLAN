#define FILE_DEVICE_GSMRADIO            0x200

#define IOCTL_SPKR_ENABLE               CTL_CODE(FILE_DEVICE_GSMRADIO, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SPKR_DISABLE              CTL_CODE(FILE_DEVICE_GSMRADIO, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)