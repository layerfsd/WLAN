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
#ifndef __DWNLDAPP_H_
#define __DWNLDAPP_H_

#include <string.hxx>

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////
//
// registry keys used by the download app
//
/////////////////////////////////////////////////////////////////
#define DWNLDAPP_ROOT_KEY                       HKEY_LOCAL_MACHINE
#define DWNLDAPP_REGKEY_PATH                    TEXT("System\\ImageUpdate\\DwnldApp")
//dir on the device where pkgs are temporarily downloaded to
#define DWNLDAPP_REGVAL_PKGSDESTDIRPATH         TEXT("PkgsDestDirPath")
//path of the bit map displayed when update app applies packages
#define DWNLDAPP_REGVAL_PKGSPROGBITMAPPATH      TEXT("PkgsProgBitMapPath")
//list of dirs to search for pkgs.lst file
#define DWNLDAPP_REGVAL_MANIFESTFILEDIRS        TEXT("ManifestFileDirs")
//for each dir in DWNLDAPP_REGVAL_MANIFESTFILEDIRS, download app will
//wait seconds specified in this regval, for the dir's FSD to load
//before accessing the next dir in the list. Default is 30sec.
#define DWNLDAPP_REGVAL_MANIFESTFSDTIMEOUT      TEXT("ManifestFSDTimeOut")
//REG_HEX(1 byte length): 1->download only signed images 0->allow download of unsigned images also
#define DWNLDAPP_REGVAL_DWNLDSIGNEDIMGS         TEXT("DwnldSignedImages")
//For raw unsigned images - size of the packet downloaded and written to the flash.
//Specify 0 to download whole file before flashing. Usually this will be a multiple
//of blocksize for NOR and a multiple of (blocksize+blockmetadatasize) for NAND.
#define DWNLDAPP_REGVAL_RAWIMGDWNLDPKTSIZE      TEXT("DwnldPktSize")
//Target device - For disk images
#define DWNLDAPP_REGVAL_IMGTARGETDEVICE         TEXT("ImgTargetDevice")
//Starting block offset when writing raw images to the target device
#define DWNLDAPP_REGVAL_RAWIMGBLOCKOFFSET       TEXT("RawImgBlockOffset")
//list of trusted public keys - used to verify signed images
#define DWNLDAPP_TRUSTEDPUBKEYS_REGKEY_PATH     TEXT("System\\ImageUpdate\\DwnldApp\\TrustedPubKeys")


/////////////////////////////////////////////////////////////////
//
// error/message macro used by the download app
//
/////////////////////////////////////////////////////////////////
/* DWNLDAPPMSG macro is used for error, warning and status messages. By default DWNLDAPPMSG
*  is defined as a RETAILMSG. OEMs can route DWNLDAPPMSG messages to their custom function by
*  setting g_pfnOEMDwnldAppMsg.
*/
#define DWNLDAPPMSG(cond, debugparams_in_parens) \
    ((cond)?((g_pfnOEMDwnldAppMsg!=NULL)?(g_pfnOEMDwnldAppMsg debugparams_in_parens),1: \
                                 (NKDbgPrintfW debugparams_in_parens),1):0)

/////////////////////////////////////////////////////////////////
//
// Main Entry point into the dlappcommon
//
/////////////////////////////////////////////////////////////////
HRESULT DwnldAppMain(LPVOID lpvParam);


/////////////////////////////////////////////////////////////////
//
// optional OEM functions
//
/////////////////////////////////////////////////////////////////

/* Function called before downloads start.
*  pwstrManifestSrcDir - IN - directory where download app found pkgs.lst
*  pwstrImgsList       - IN - list of images to download delimited by '\0'. Ends with '\0\0'
*  pwstrPkgsList       - IN - list of packages to download delimited by '\0'. Ends with '\0\0'
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPREDOWNLOAD)  (ce::wstring* pwstrManifestSrcDir, ce::wstring* pwstrImgsList, ce::wstring* pwstrPkgsList);

/* Function called after downloads are done. 
*  This function is always called; even if there was an error and download app is aborting.
*  fError              - IN - set to true if there was an error during the download process
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPOSTDOWNLOAD) (BOOL fError);

/* prototype for g_pfnOEMDwnldAppMsg. If g_pfnOEMDwnldAppMsg!=NULL DWNLDAPPMSG macro calls
*  g_pfnOEMDwnldAppMsg(..). See DWNLDAPPMSG macro above.
*/
typedef void    (* PFN_OEMDWNLDAPPMSG) (LPCWSTR lpszFmt, ...);

/* Function called before any packages are downloaded.
*  pwstrPkgsList       - IN - list of packages to download delimited by '\0'. Ends with '\0\0'
*  pwstrPkgsSrcDir     - IN - directory where download app found pkgs.lst and will downloaded pkgs from.
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPREPKGSDOWNLOAD) (ce::wstring* pwstrPkgsList, ce::wstring* pwstrPkgsSrcDir);

/* Function called after packages have been downloaded (or if there was an error)
*  fError              - IN - set to true if there was an error during the pkgs download process
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPOSTPKGSDOWNLOAD) (BOOL fError);

/* Function called after a package has been downloaded to the temporary dir on the device.
*  Gives OEMs the ability to show packages download progress.
*  pwstrPkgSrcPath    - IN - full path of the downloaded package
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMSHOWPKGDWNLDPROGRESS) (ce::wstring* pwstrPkgSrcPath);

/* Function called before disk imageS are downloaded.
*  pwstrImgsList       - IN - list of images to download delimited by '\0'. Ends with '\0\0'
*  pwstrImgsSrcDir     - IN - directory where download app found pkgs.lst and will downloaded disk images from.
*  pwstrTargetDevice   - IN - name of the target disk/partition/device where images are supposed to be written to.
*                             Dlappcommon looks up this value from the key "ImgTargetDevice" in registry and 
*                             passes it as is to this function.
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPREIMGSDOWNLOAD) (ce::wstring* pwstrImgsList, ce::wstring* pwstrImgsSrcDir, ce::wstring* pwstrTargetDevice);

/* Function called after images have been downloaded (or if there was an error)
*  fError              - IN - set to true if there was an error during the image download and writing process
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPOSTIMGSDOWNLOAD) (BOOL fError);

/* Function called before downloading a .bin image
*  imgHdr              - IN - usually the 7-byte image header. Please refer to slimg.h for image headers.
*  cbimgHdrLen         - IN - length of the image header. usually 7-bytes.
*  hImgFile            - IN - handle to the image being downloaded.
*  pwstrImgSrcPath     - IN - full source path of the image being downloaded.
*  dwImageAddress      - IN - starting address of image on the device.
*  dwImageLength       - IN - length of the image (in bytes).
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPREBINIMAGEDOWNLOAD) (BYTE* imgHdr, DWORD cbimgHdrLen,
                                                HANDLE hImgFile, ce::wstring* pwstrImgSrcPath,
                                                DWORD dwImageAddress, DWORD dwImageLength);

/* Function called before downloading a raw image
*  imgHdr              - IN - If signed raw image: usually the 7-byte signed image header. Please refer to slimg.h for image headers.
*                             If unsigned raw image then this is NULL.
*  cbimgHdrLen         - IN - If signed raw image: length of the image header. usually 7-bytes.
*                             If unsigned raw image then this is 0.
*  hImgFile            - IN - handle to the image being downloaded.
*  pwstrImgSrcPath     - IN - full source path of the image being downloaded.
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPRERAWIMAGEDOWNLOAD) (BYTE* imgHdr, DWORD cbimgHdrLen,
                                                HANDLE hImgFile, ce::wstring* pwstrImgSrcPath);

/* Function called after an image has been downloaded (or if there was an error)
*  fError              - IN - set to true if there was an error during the image download and writing process
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMPOSTIMAGEDOWNLOAD)(BOOL fError);

/* Function called to write a downloaded raw image packet to the target disk/partition.
*  This is the function that should implement writing of the image to its final location on the device.
*  pbImgBuffer         - IN - pointer to the buffer holding the downloaded raw image packet
*  cbDataLen           - IN - length of the downloaded packet.
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMRAWIMAGEWRITE) (BYTE* pbImgBuffer, DWORD cbDataLen);

/* Function called to write a downloaded .bin image record to the target disk/partition.
*  This is the function that should implement writing of a .bin record to its final location on the device.
*  dwRecAddr           - IN - starting address of the record
*  dwRecLen            - IN - length of the record.
*  dwRecChkSum         - IN - record checksum
*  pbImgBuf            - IN - buffer holding the downloaded .bin record
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMBINIMAGEWRITE) (DWORD dwRecAddr, DWORD dwRecLen, DWORD dwRecChkSum, BYTE* pbImgBuffer);

/* Function called after a packet/record of the disk image has been downloaded and written to the device.
*  Gives OEMs the ability to show image download progress.
*  cbDataLen           - IN - length of the packet downloaded and written.
*  Return value             - return S_OK if no error.
*                             Download App aborts if the return value is an error.
*/
typedef HRESULT (* PFN_OEMSHOWIMGDWNLDPROGRESS) (DWORD cbDataLen);

/* extern variables for all the function pointers */
/* general download app functions */
extern PFN_OEMPREDOWNLOAD           g_pfnOEMPreDownload;            /* function called before downloads start*/  
extern PFN_OEMPOSTDOWNLOAD          g_pfnOEMPostDownload;           /* function called after download.

/* error/message function */
extern PFN_OEMDWNLDAPPMSG           g_pfnOEMDwnldAppMsg;

/* pkgs OEM functions */
extern PFN_OEMPREPKGSDOWNLOAD       g_pfnOEMPrePkgsDownload;
extern PFN_OEMPOSTPKGSDOWNLOAD      g_pfnOEMPostPkgsDownload;
extern PFN_OEMSHOWPKGDWNLDPROGRESS  g_pfnOEMShowPkgDwnldProgress;

/* disk image OEM functions */
extern PFN_OEMPREIMGSDOWNLOAD       g_pfnOEMPreImgsDownload;
extern PFN_OEMPOSTIMGSDOWNLOAD      g_pfnOEMPostImgsDownload;
extern PFN_OEMPREBINIMAGEDOWNLOAD   g_pfnOEMPreBinImageDownload;
extern PFN_OEMPRERAWIMAGEDOWNLOAD   g_pfnOEMPreRawImageDownload;
extern PFN_OEMPOSTIMAGEDOWNLOAD     g_pfnOEMPostImageDownload;
extern PFN_OEMRAWIMAGEWRITE         g_pfnOEMRawImageWrite;
extern PFN_OEMBINIMAGEWRITE         g_pfnOEMBinImageWrite;
extern PFN_OEMSHOWIMGDWNLDPROGRESS  g_pfnOEMShowImgDwnldProgress;

#ifdef __cplusplus
}
#endif

#endif
