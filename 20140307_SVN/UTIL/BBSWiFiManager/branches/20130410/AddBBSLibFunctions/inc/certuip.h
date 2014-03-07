//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
typedef
BOOL (WINAPI *PFNCRYPTENUMPROVIDERSW)(
    DWORD   dwIndex,
    DWORD * pdwReserved,
    DWORD   dwFlags,
    DWORD * pdwProvType,
    LPWSTR pszProvName,
    DWORD * pcbProvName
    );

typedef
BOOL (WINAPI *PFNCRYPTGETPROVPARAM)
(
    HCRYPTPROV hProv,
    DWORD dwParam,
    BYTE*pbData,
    DWORD *pdwDataLen,
    DWORD dwFlags
    );
typedef  BOOL (WINAPI *PFNCRYPTACQUIRECONTEXTW)(
    HCRYPTPROV *phProv,
    LPCWSTR pszContainer,
    LPCWSTR pszProvider,
    DWORD dwProvType,
    DWORD dwFlags
    );
typedef BOOL (WINAPI *PFNCRYPTRELEASECONTEXT)(
    HCRYPTPROV hProv,
    ULONG_PTR dwFlags
    );

typedef
BOOL (WINAPI *PFNCRYPTDESTROYKEY)(
    HCRYPTKEY hKey
    );

typedef
BOOL (WINAPI *PFNCRYPTGETUSERKEY)(
    HCRYPTPROV hProv,
    DWORD dwKeySpec,
    HCRYPTKEY *phUserKey
    );

typedef BOOL (WINAPI *PFNCRYPTGETKEYPARAM)(
    HCRYPTKEY hKey,
    DWORD dwParam,
    BYTE *pbData,
    DWORD *pdwDataLen,
    DWORD dwFlags
    );

typedef
PCCERT_CONTEXT (WINAPI *PFNCERTCREATECERTIFICATECONTEXT) (
    IN DWORD dwCertEncodingType,
    IN const BYTE *pbCertEncoded,
    IN DWORD cbCertEncoded
    );
    
typedef
BOOL (WINAPI *PFNCERTFREECERTIFICATECONTEXT)(
    IN PCCERT_CONTEXT pCertContext
    );
typedef
PCCERT_CONTEXT (WINAPI *PFNCERTDUPLICATECERTIFICATECONTEXT)(
    IN PCCERT_CONTEXT pCertContext
    );

typedef
HCERTSTORE (WINAPI *PFNCERTOPENSTORE)(
    IN LPCSTR lpszStoreProvider,
    IN DWORD dwEncodingType,
    IN HCRYPTPROV hCryptProv,
    IN DWORD dwFlags,
    IN const void *pvPara
    );

typedef
BOOL (WINAPI *PFNCERTCLOSESTORE)(
    IN HCERTSTORE hCertStore,
    DWORD dwFlags
    );

typedef
PCCERT_CONTEXT  (WINAPI *PFNCERTENUMCERTIFICATESINSTORE)(
    IN HCERTSTORE hCertStore,
    IN PCCERT_CONTEXT pPrevCertContext
    );

typedef
PCCERT_CONTEXT (WINAPI *PFNCERTFINDCERTIFICATEINSTORE)(
    IN HCERTSTORE hCertStore,
    IN DWORD dwCertEncodingType,
    IN DWORD dwFindFlags,
    IN DWORD dwFindType,
    IN const void *pvFindPara,
    IN PCCERT_CONTEXT pPrevCertContext
    );

typedef
DWORD (WINAPI *PFNCERTGETNAMESTRINGW)(
    IN PCCERT_CONTEXT pCertContext,
    IN DWORD dwType,
    IN DWORD dwFlags,
    IN void *pvTypePara,
    OUT OPTIONAL LPWSTR pszNameString,
    IN DWORD cchNameString
    );
typedef
BOOL (WINAPI *PFNCERTGETCERTIFICATECONTEXTPROPERTY)(
    IN PCCERT_CONTEXT pCertContext,
    IN DWORD dwPropId,
    OUT void *pvData,
    IN OUT DWORD *pcbData
    );
typedef
BOOL (WINAPI *PFNCERTSETCERTIFICATECONTEXTPROPERTY)(
    IN PCCERT_CONTEXT pCertContext,
    IN DWORD dwPropId,
    IN DWORD dwFlags,
    IN const void *pvData
    );
typedef
BOOL (WINAPI * PFNCERTADDCERTIFICATECONTEXTTOSTORE)(
    IN HCERTSTORE hCertStore,
    IN PCCERT_CONTEXT pCertContext,
    IN DWORD dwAddDisposition, 
    OUT OPTIONAL PCCERT_CONTEXT * ppStoreContext);

    
typedef
BOOL (WINAPI *PFNCERTADDENCODEDCERTIFICATETOSTORE)(
    IN HCERTSTORE hCertStore,
    IN DWORD dwCertEncodingType,
    IN const BYTE *pbCertEncoded,
    IN DWORD cbCertEncoded,
    IN DWORD dwAddDisposition,
    OUT OPTIONAL PCCERT_CONTEXT *ppCertContext
    );
typedef 
BOOL (WINAPI * PFNCERTCOMPARECERTIFICATENAME) (
    IN DWORD dwCertEncodingType,
    IN PCERT_NAME_BLOB pCertName1,
    IN PCERT_NAME_BLOB pCertName2
    );
typedef
DWORD (WINAPI * PFNCERTNAMETOSTRW) (
    IN DWORD dwCertEncodingType,
    IN PCERT_NAME_BLOB pName,
    IN DWORD dwStrType,
    OUT OPTIONAL LPWSTR psz,
    IN DWORD csz
    );

typedef
BOOL (WINAPI  *PFNCERTGETENHANCEDKEYUSAGE) (
                IN     PCCERT_CONTEXT     pCertContext,
                IN     DWORD              dwFlags,
                OUT    PCERT_ENHKEY_USAGE pUsage,
                IN OUT DWORD*             pcbUsage
                );

typedef
PCCRYPT_OID_INFO (WINAPI *PFNCRYPTFINDOIDINFO)(
    IN DWORD dwKeyType,
    IN void *pvKey,
    IN DWORD dwGroupId
    );

typedef
PCERT_EXTENSION (WINAPI *PFNCERTFINDEXTENSION)(
    IN LPCSTR pszObjId,
    IN DWORD cExtensions,
    IN CERT_EXTENSION rgExtensions[]
    );

typedef
BOOL (WINAPI *PFNCRYPTDECODEOBJECTEX)(
    IN DWORD dwCertEncodingType,
    IN LPCSTR lpszStructType,
    IN const BYTE *pbEncoded,
    IN DWORD cbEncoded,
    IN DWORD dwFlags,
    IN OPTIONAL PCRYPT_DECODE_PARA pDecodePara,
    OUT OPTIONAL void *pvStructInfo,
    IN OUT DWORD *pcbStructInfo
    );

typedef
DWORD (WINAPI *PFNCERTRDNVALUETOSTRW)(
    IN DWORD dwValueType,
    IN PCERT_RDN_VALUE_BLOB pValue,
    OUT OPTIONAL LPWSTR psz,
    IN DWORD csz
    );

typedef
BOOL (WINAPI *PFNCRYPTVERIFYCERTIFICATESIGNATURE)(
    IN HCRYPTPROV hCryptProv,
    IN DWORD dwCertEncodingType,
    IN BYTE* pbEncoded,
    IN DWORD cbEncoded,
    IN PCERT_PUBLIC_KEY_INFO pPublicKey
    );

#define CBRA_LOAD(hinst, name, type) do{pfn##name=(type)GetProcAddressW((hinst),L#name);CBRA(NULL!=pfn##name);}while(0,0)

extern PFNCERTOPENSTORE pfnCertOpenStore;
extern PFNCERTENUMCERTIFICATESINSTORE pfnCertEnumCertificatesInStore;
extern PFNCERTFINDCERTIFICATEINSTORE pfnCertFindCertificateInStore;
extern PFNCERTGETCERTIFICATECONTEXTPROPERTY pfnCertGetCertificateContextProperty;
extern PFNCERTFREECERTIFICATECONTEXT pfnCertFreeCertificateContext;
extern PFNCERTGETENHANCEDKEYUSAGE pfnCertGetEnhancedKeyUsage;
extern PFNCERTGETNAMESTRINGW pfnCertGetNameStringW;
extern PFNCERTCLOSESTORE pfnCertCloseStore;
extern PFNCRYPTVERIFYCERTIFICATESIGNATURE pfnCryptVerifyCertificateSignature;
extern PFNCERTCOMPARECERTIFICATENAME pfnCertCompareCertificateName;

HRESULT LoadCAPI2();
void    FreeCAPI2();  // must be called once for each SUCCESSFUL call to LoadCAPI2

HRESULT LoadCAPI2_I(HINSTANCE hinstCrypt32);
void    FreeCAPI2_I();

void InitCertList(HWND hCtlList);
HRESULT InsertCertName(HWND hCtlList, PTCHAR szCertName, int iIndex);
void SetSelectedCert(HWND hCtlList, int iSelected);
int GetSelectedCert(HWND hCtlList);

DWORD GetPickCertDialogFlags();
