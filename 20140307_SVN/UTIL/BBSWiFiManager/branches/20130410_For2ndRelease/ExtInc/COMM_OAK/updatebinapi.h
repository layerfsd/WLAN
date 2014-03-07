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
/*++


Module Name:

    updatebinAPI.h

Abstract:

    external definitions, macros, etc for UpdateBin 

Author:

    lmcneill

Revision History:

--*/

#ifndef _UPDATEBINAPI
#define _UPDATEBINAPI

typedef struct _UBBitmapInfo
{
	DWORD		cbSize;				// size of this structure 

    DWORD       dwOffset;           // Offset to this bitmap from the beginning
                                    // of the update bin bitmap file.
    DWORD       dwStrideWidth;      // Stride width of this bitmap
    RECT        rc;                 // Draw rectangle - This drawing
                                    // rectangle represents the location where
                                    // this bitmap should be drawn on the screen
                                    // during the update.  The width
                                    // and height of the rect must also match
                                    // the width and height of the Bitmap.

} UBBitmapInfo;



// Note: currently these must be in the same order as the string IDs in resource.h
typedef enum _UBUpdateStates
{
    UPDATEBIN_STATE_PROCESSING = 0,
    UPDATEBIN_STATE_VALIDATING,
    UPDATEBIN_STATE_VERIFYING,
	UPDATEBIN_STATE_UPDATING,

	/* add new states here */
	UPDATEBIN_NUMSTATES	/* must be last item */
} UBUpdateStates;

// rem - update if new base bitmaps are added to the UBBitmapFileHdr
#define NUM_BASE_BITMAPS 4

// default progress bar color is green
#define DEFAULT_PROGRESSBARCOLOR 0x07e0

// avoid failing on zero-sized array (UBBitmapInfo	bmiPackage[0];), because it makes the code simpler:
#pragma warning( disable : 4200 )	
typedef struct _UBBitmapFileHdr
{
	DWORD cbSize; // The size of the structure.

	// Bitmaps for each individual state in the Update process
	UBBitmapInfo bmiStatus[UPDATEBIN_NUMSTATES];	// the string bitmaps for each stage of the update eg "validating"

	UBBitmapInfo bmiBaseUpdateProgress;		        // the ‘base’ bitmap used during update

	UBBitmapInfo	bmiBaseError;	                // bitmap to be displayed in case of an error during update

	UBBitmapInfo	bmiBaseResume;	                // bitmap to be displayed in case of update being resumed
	   	
	UBBitmapInfo	bmiBaseExternal;	            // bitmap to be displayed in case of required card being removed. 
										            // In the case that this is not an external update, this structure will be unused (zeroed)
	RECT rcProgress;			                    // location where the progress bar should go

	// following 3 members for verification/debugging only, display driver already knows this info:
	WORD wBitDepth;			                        // display bit depth

	DWORD	dwNumPackages;	                        // number of packages being updated (for verification, updldr already knows this)

	// width and height of the screen when the bitmaps were created.
	DWORD	dwScreenWidth;
	DWORD	dwScreenHeight;
    DWORD   dwScreenOrientation;
	
	// Progress bar color
	DWORD	dwProgressPixelColor;
	
	// this array will depend on the number of packages being updated at runtime
	UBBitmapInfo	bmiPackage[0];	               // package name bitmaps 

} UBBitmapFileHdr;
#pragma warning(default:4200)

#define GET_UBBMPFILE_HDRSIZE(numpkgs) (sizeof(UBBitmapFileHdr) + ((numpkgs) * sizeof(UBBitmapInfo)))

typedef struct _UBBitmapInfoEx
{
    DWORD cbSize;              // Size of this structure
    DWORD dwPaletteOffset;     // Offset of palette in file
    DWORD cbPalette;           // Size of palette
    DWORD dwDataOffset;        // Offset of compressed data
    DWORD cbData;              // Size of data
    DWORD dwStrideWidth;       // Specifies the number of bytes in each scan line.  
    RECT      rc;              // Draw rectangle: This drawing rectangle represents the location where this bitmap should be drawn on the screen during the update.
                               // The width and height of the rect must also match the width and height of the bitmap.
} UBBitmapInfoEx;

typedef enum _UB_BITMAP_COMPRESSION
{
    UB_BITMAP_COMPRESSION_NONE = 0,
    UB_BITMAP_COMPRESSION_RLE8
}UB_BITMAP_COMPRESSION;

typedef enum _UB_BITMAP
{
    UB_BITMAP_VALIDATING = 0,
    UB_BITMAP_VERIFYING,
    UB_BITMAP_UPDATING,
    UB_BITMAP_BASE_UPDATEPROGRESS,
    UB_BITMAP_BASE_ERROR,
    UB_BITMAP_BASE_RESUME
} UB_BITMAP;

// avoid failing on zero-sized array (UBBitmapFileHdrEx    bmiBitMaps[0];), 
// because it makes the code simpler:
#pragma warning(disable : 4200)
typedef struct _UBBitmapFileHdrEx
{
    DWORD cbSize;                        // Size of this structure
    DWORD dwBitDepth;                    // Display bit depth

    // Width and height of the screen when the bitmaps were created.
    DWORD dwScreenWidth;
    DWORD dwScreenHeight;
    DWORD dwScreenOrientation;

    RECT  rcProgress;                    // Location where the progress bar should go
    DWORD dwProgressPixelColor;          // Progress bar color

    UB_BITMAP_COMPRESSION compression;   // Compression algorithm used

    // Array of bitmaps
    DWORD cBitmaps;                      // Number of bitmaps in array 
    UBBitmapInfoEx bmiBitmaps[0];        // Array of bitmaps. Will be indexed 
                                         // like this: pUBBmi->bmiBitmaps[UB_IMAGE_BASE_ERROR].
} UBBitmapFileHdrEx;
#pragma warning(default:4200)

#endif // _UPDATEBINUI
