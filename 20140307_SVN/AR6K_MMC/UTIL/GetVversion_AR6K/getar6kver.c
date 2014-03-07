#include <ndis.h>
#include <nuiouser.h>

#define AR6K_ADAPTERNAME _T("AR6K_SD1")
#define IOCTL_CAR6K_GET_FIRMWARE_VERSION    CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF1,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_CAR6K_GET_HOST_VERSION        CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0xF2,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define VER_MAJOR_BIT_OFFSET			28
#define VER_MINOR_BIT_OFFSET			24
#define VER_PATCH_BIT_OFFSET			16
#define VER_MMC_BUILD_NUM_BIT_OFFSET	8
#define VER_BUILD_NUM_BIT_OFFSET		0

HRESULT getVersion( DWORD* verF, DWORD* verD )
{
	BOOL fRetVal = FALSE;
	HRESULT hr = S_OK;
	HANDLE hdrgAdapter = NULL;
	DWORD dwReturnedBytes = 0;
	
	*verF = *verD = 0;
	
    hdrgAdapter = CreateFile(TEXT("DRG1:"),GENERIC_READ | GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                    INVALID_HANDLE_VALUE);


    if( INVALID_HANDLE_VALUE != hdrgAdapter )
    	{
        fRetVal = DeviceIoControl(
                        hdrgAdapter,
                        IOCTL_CAR6K_GET_FIRMWARE_VERSION,
                        NULL,
                        0,
                        verF,
                        sizeof (DWORD),
                        &dwReturnedBytes,
                        NULL);

        if( !fRetVal )
        	{
	        //wprintf(L"getar6kver: IOCTL_CAR6K_GET_FIRMWARE_VERSION ioctl failed!\n");
	        MessageBox(NULL,_T("IOCTL_CAR6K_GET_FIRMWARE_VERSION failed"),NULL,MB_OK);
            hr = E_FAIL;
        	}

        dwReturnedBytes = 0;

        fRetVal = DeviceIoControl(
                        hdrgAdapter,
                        IOCTL_CAR6K_GET_HOST_VERSION,
                        NULL,
                        0,
                        verD,
                        sizeof (DWORD),
                        &dwReturnedBytes,
                        NULL);

        if( !fRetVal )
        	{
	        //wprintf(L"getar6kver: IOCTL_CAR6K_GET_HOST_VERSION ioctl failed!\n");
	        MessageBox(NULL,_T("IOCTL_CAR6K_GET_HOST_VERSION failed"),NULL,MB_OK);
            hr = E_FAIL;
        	}
    }

    if( hdrgAdapter )
    	{
        CloseHandle( hdrgAdapter );
        hdrgAdapter = NULL;
    	}
    	
    return hr;
}

int wmain( int argc, WCHAR** argv )
{
	WCHAR wszVersion[256];
	DWORD verFirmware, verDriver;
	DWORD vfMajor, vfMinor, vfPatch, vfBuild;
	DWORD vdMajor, vdMinor, vdPatch, vdBuild;
    DWORD vdMMCBuild;
	DWORD dwReturnedBytes   = 0;
    BOOL  fRetVal           = FALSE;
    HANDLE hdrgAdapter     = NULL;

    if( getVersion(&verFirmware,&verDriver) == S_OK )
    	{
	    vdMajor = (verDriver >> VER_MAJOR_BIT_OFFSET) & 0xF;
		vdMinor = (verDriver >> VER_MINOR_BIT_OFFSET) & 0xF;
		vdPatch = (verDriver >> VER_PATCH_BIT_OFFSET) & 0xFF;
		vdBuild = (verDriver >> VER_BUILD_NUM_BIT_OFFSET) & 0xFF;
		vdMMCBuild = (verDriver >> VER_MMC_BUILD_NUM_BIT_OFFSET) & 0xFF;
		vfMajor = (verFirmware >> VER_MAJOR_BIT_OFFSET) & 0xF;
		vfMinor = (verFirmware >> VER_MINOR_BIT_OFFSET) & 0xF;
		vfPatch = (verFirmware >> VER_PATCH_BIT_OFFSET) & 0xFF;
		vfBuild = (verFirmware >> VER_BUILD_NUM_BIT_OFFSET) & 0xFFFF;
		
		wsprintf( wszVersion, L"Firmware: %d.%d.%d.%d\r\nDriver: %d.%d.%d.%d(b%d)\r\n", 
				vfMajor, vfMinor, vfPatch, vfBuild, vdMajor, vdMinor, vdPatch, vdBuild, vdMMCBuild );
		
		MessageBox(NULL,wszVersion,L"AR6K version",MB_OK);
    	}
   
	
	return 0;
}