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
/*
Evidence Generator Header File
*/

#ifndef _EVGEN_H

#define _EVGEN_H

#ifdef __cplusplus
extern "C" {
#endif

// Constants

typedef enum {
	EVGEN_FILE_FORMAT_INVALID = 0,
	EVGEN_FILE_FORMAT_PE,
	EVGEN_FILE_FORMAT_CAB,
	EVGEN_FILE_FORMAT_MAX
}EVGEN_FILE_FORMAT_TYPE, *PEVGEN_FILE_FORMAT_TYPE;

typedef enum {
	EVGEN_HASH_TYPE_INVALID = 0,
	EVGEN_HASH_TYPE_CERT,
	EVGEN_HASH_TYPE_IMAGE_HASH,
	EVGEN_HASH_TYPE_PUBLISHER_HASH,
	EVGEN_HASH_TYPE_MAX
}EVGEN_HASH_TYPE, *PEVGEN_HASH_TYPE;

typedef enum {
	EVGEN_LOG_LEVEL_INVALID = 0,
	EVGEN_LOG_LEVEL_ERROR,
	EVGEN_LOG_LEVEL_WARNING,
	EVGEN_LOG_LEVEL_INFO,
	EVGEN_LOG_LEVEL_MAX
}EVGEN_LOG_LEVEL, *PEVGEN_LOG_LEVEL;

#define COLLECTED_EVIDENCE_NONE 0x0
#define COLLECTED_EVIDENCE_FILE_MAP 0x1
#define COLLECTED_EVIDENCE_SIG_INFO 0x2
#define COLLECTED_EVIDENCE_PKCS7_INFO 0x4
#define COLLECTED_EVIDENCE_IMAGE_HASH_IN_SIG 0x8
#define COLLECTED_EVIDENCE_CHAIN_INFO 0x10
#define COLLECTED_EVIDENCE_PUB_HASH 0x20
#define COLLECTED_EVIDENCE_IMAGE_HASH 0x40

#define SIG_VERIFY_FAILED 0x1
#define IMAGE_HASH_VERIFY_FAILED 0x2
#define CHAIN_VERIFY_FAILED 0x4

#define IMAGE_HASH_REVOKED 0x1
#define PUB_HASH_REVOKED 0x2
#define CHAIN_REVOKED 0x4

#define TRUST_FAILED 0x0
#define TRUST_SUCCESS 0x1

#define EVIDENCE_MISSING 0x0
#define EVIDENCE_PRESENT 0x1

// Structures

typedef struct {
	DWORD status;
	DWORD storeIndex;
	DWORD certIndex;
	PCERT_CONTEXT pCertContext;
}TRUST_STATUS, *PTRUST_STATUS;

typedef struct {
	DWORD status;
	DWORD certIndex;
}REVOKE_STATUS, *PREVOKE_STATUS;

typedef struct {
	DWORD status;
	DWORD chainErr;
}VERIFY_STATUS, *PVERIFY_STATUS;

typedef struct {
	DWORD evidenceStatus;
	VERIFY_STATUS verifyStatus;
	REVOKE_STATUS revokeStatus;
	TRUST_STATUS trustStatus;
}WINMOBILE_RESULT, *PWINMOBILE_RESULT;

// Callbacks
typedef BOOL (*PFN_IS_HASH_REVOKED)(EVGEN_HASH_TYPE, PBYTE, DWORD);

typedef void (*PFN_LOG_FUNC)(EVGEN_LOG_LEVEL,PWCHAR);

// Functions

DWORD
EvgenInit(
	void
);

DWORD
EvgenDeInit(
	void
);

DWORD
EvgenSetLogFunc(
	PFN_LOG_FUNC logFunc
);

DWORD
EvgenOpenHandle(
	PHANDLE pHandle
);

DWORD
EvgenCloseHandle(
	HANDLE handle
);

DWORD
EvgenSetMemFileInfo(
	HANDLE handle,
	PBYTE fileView,
	DWORD fileSize,
	EVGEN_FILE_FORMAT_TYPE fileType
);

DWORD
EvgenSetFileInfo(
	HANDLE handle,
	HANDLE fileHandle,
	EVGEN_FILE_FORMAT_TYPE fileType
);

DWORD 
EvgenSetTrustedStores(
	HANDLE handle,
	DWORD numStores,
	HCERTSTORE *pStores
);

DWORD 
EvgenSetRevokeFunction(
	HANDLE handle,
	PFN_IS_HASH_REVOKED pIsHashRevoked
);

DWORD
EvgenCollectEvidence(
	HANDLE handle
);

DWORD
EvgenEvaluateEvidence(
	HANDLE handle,
	PWINMOBILE_RESULT pResult
);

DWORD
EvgenGetChain(
	HANDLE handle,
	PCERT_SIMPLE_CHAIN *ppChain
);

DWORD
EvgenGetImageHash(
	HANDLE handle,
	PCRYPT_HASH_BLOB  *ppHash
);

DWORD
EvgenGetImageSHA1Hash(
	HANDLE handle,
	PCRYPT_HASH_BLOB  *ppHash
);

DWORD
EvgenGetPubHash(
	HANDLE handle,
	PCRYPT_HASH_BLOB  *ppHash
);

DWORD
EvgenGetCollectedEvidence(
	HANDLE handle,
	PDWORD pEvidence
);

#ifdef __cplusplus
}
#endif

#endif



