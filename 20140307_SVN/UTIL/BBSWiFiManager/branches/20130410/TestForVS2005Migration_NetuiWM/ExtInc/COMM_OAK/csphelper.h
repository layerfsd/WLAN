/// <company>Microsoft Corporation</company>
/// <copyright>
///     Copyright (c) Microsoft Corporation.  All rights reserved.
/// </copyright>
/// <summary>
///     CSPConfig.h
///         Defines functions to create an in-memory representation of a CSP
///         config file. Also reads and writes it to the registry.
/// </summary>
/// <remarks>
///     <para>
///         Use of this source code is subject to the terms of your Windows Mobile
///         Software Shared Source Premium Derivatives License Form or other
///         applicable Microsoft license agreement for the software. If you did not
///         accept the terms of such a license, you are not authorized to use this
///         source code.
///     </para>
/// </remarks>


#pragma once

#pragma warning( push )
// all these for public\common\sdk\inc\svsutil.hxx
#pragma warning( disable:4100 )
#pragma warning( disable:4127 )
#pragma warning( disable:4189 )
#pragma warning( disable:4201 )
#pragma warning( disable:4310 )
#include <list.hxx>
#include <creg.hxx>
#pragma warning( pop )

// this is how we determine which registry setting to use.
enum TreeType
{
    CSP_CONFIG_APP_DISABLE,
    CSP_CONFIG_SDP,
};
enum enumValueType
{
    CSP_HELPER_PARM_VALUE_TYPE_STRING = 0,
    CSP_HELPER_PARM_VALUE_TYPE_BINARY
};
const DWORD c_cchMaxCspString = MAX_PATH;
const DWORD c_cbMaxCspBinary = 500;
const DWORD c_cchMaxRegNodeId = 30;
const DWORD c_cchMaxSubKey = MAX_PATH;

extern const WCHAR c_pwszCharacteristicList[];
extern const WCHAR c_pwszParmList[];

class Parm;

class Characteristic
{
public:
    Characteristic();
    ~Characteristic();

    // creation
    static Characteristic *Characteristic::CreateCharacteristic(
                                                        __in Characteristic *pParent, 
                                                        __in PCWSTR pwszType);
    Characteristic *CreateChildCharacteristic(__in PCWSTR pwszType);

    Parm *CreateChildParm(__in PCWSTR pwszName, __in PCWSTR pwszValue);
    Parm *CreateChildParm(__in PCWSTR pwszName, 
                            __in PBYTE pbValue, 
                            __in enumValueType eValueType,
                            __in DWORD dwValueLength);

    // deletion
    HRESULT DeleteChildCharacteristic(__in PCWSTR pwszType);
    HRESULT DeleteChildParm(__in PCWSTR pwszName);

    //Characteristic query
    Characteristic *GetFirstChildCharacteristic();
    Characteristic *GetNextChildCharacteristic();
    Characteristic *GetChildCharacteristic(__in PCWSTR pwszType);    
    PCWSTR GetType();

    // parm query
    Parm *GetFirstChildParm();
    Parm *GetNextChildParm();
    Parm *GetChildParm(__in PCWSTR pwszName);

    // parent query
    Characteristic *GetParentCharacteristic();

    // registry functions
    HRESULT SaveToRegistry( CReg * pkey );
    HRESULT LoadFromRegistry( CReg * pkey );

private:

    // variables
    PWCHAR m_pwszType;
    Characteristic *m_pcharacteristicParent;

    // iterators
    ce::list<Characteristic*>::const_iterator m_itCharEnum;
    ce::list<Parm*>::const_iterator m_itParmEnum;

    // child lists 
    ce::list<Characteristic *>  m_ChildCharList;
    ce::list<Parm *>            m_ChildParmList;

    // statics
    static PCWSTR Characteristic::ms_pwszLiteralCharacteristic;
    static PCWSTR Characteristic::ms_pwszLiteralType;
    static PCWSTR Characteristic::ms_pwszLiteralParms;

    // internal child creation
    Parm *CreateChildParm();

};

class Parm
{
public:
    Parm(Characteristic *Parent);
    ~Parm();

    static PCWSTR ms_pwszLiteralParm;
    static PCWSTR ms_pwszLiteralName;
    static PCWSTR ms_pwszLiteralValue;

    Characteristic *GetParentCharacteristic();
    PCWSTR GetName();
    PVOID GetValue();
    void SetName(__in PWCHAR pwszName);
    void SetValue(__in PWCHAR pwszValue);
    void SetValue(__in PBYTE pbValue, DWORD cbValueLength);
    enumValueType GetValueType();
    DWORD GetValueLength();

    HRESULT SaveToRegistry( CReg * pkey );
    HRESULT LoadFromRegistry( CReg * pkey );

private:
    PWCHAR m_pwszName;
    PVOID m_pValue;
    enumValueType m_eValueType;
    DWORD m_cbValueLength;
    Characteristic  *m_characteristicParent;
};

class CSPHelper
{
public:
    static HRESULT GetRootCharacteristic(__inout Characteristic **Root, __in TreeType ttType);
    static HRESULT FlushRootCharacteristic(__in Characteristic *Root, __in TreeType ttType);

private:
    static HRESULT GetCSPKey( CReg &key, PCWSTR ppwszTreeType, BOOL fCreate, TreeType ttType );
    static HRESULT GetTreeTypeString( __inout PCWSTR * ppwszTreeType, __in TreeType ttType );

    static PCWSTR ms_pwszCSPRoot;
    static PCWSTR ms_pwszAppDisable;
    static PCWSTR ms_pwszEncryptedFileSystem;
};   
