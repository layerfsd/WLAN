// Reg.h: interface for the CReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REG_H__7D84F1A8_B561_47C0_BE6E_565A49477128__INCLUDED_)
#define AFX_REG_H__7D84F1A8_B561_47C0_BE6E_565A49477128__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SafeFree(p)		{ if(p) LocalFree(p); }

class CReg
{
private:
	HKEY	m_hKey;
	int		m_Index;
	LPBYTE	m_lpbValue; // last value read, if any

public:
	BOOL Create(HKEY hkRoot, LPCTSTR pszKey) {
		DWORD dwDisp;
		return ERROR_SUCCESS==RegCreateKeyEx(hkRoot, pszKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisp);
	}

	BOOL Open(HKEY hkRoot, LPCTSTR pszKey, REGSAM sam=KEY_READ) {
		return ERROR_SUCCESS==RegOpenKeyEx(hkRoot, pszKey, 0, sam, &m_hKey);
	}

	CReg(HKEY hkRoot, LPCTSTR pszKey) {
		m_hKey = NULL;
		m_Index = 0;
		m_lpbValue = NULL;
		Open(hkRoot, pszKey);
	}

	CReg() {
		m_hKey = NULL;
		m_Index = 0;
		m_lpbValue = NULL;
	}

	~CReg() { 
		if(m_hKey) RegCloseKey(m_hKey); 
		SafeFree(m_lpbValue);
	}

	void Reset() { 
		if(m_hKey) RegCloseKey(m_hKey); 
		SafeFree(m_lpbValue);
		m_hKey = NULL;
		m_Index = 0;
		m_lpbValue = NULL;
	}

	operator HKEY() { return m_hKey; }

	BOOL IsOK(void) { return m_hKey!=NULL; }


	BOOL EnumKey(LPTSTR psz, DWORD dwLen) {
		if(!m_hKey) return FALSE;
		return ERROR_SUCCESS==RegEnumKeyEx(m_hKey, m_Index++, psz, &dwLen, NULL, NULL, NULL, NULL);
	}

	BOOL EnumValue(LPTSTR pszName, DWORD dwLenName, LPTSTR pszValue, DWORD dwLenValue) {
		DWORD dwType;
		if(!m_hKey) return FALSE;
		dwLenValue *= sizeof(TCHAR); // convert length in chars to bytes
		return ERROR_SUCCESS==RegEnumValue(m_hKey, m_Index++, pszName, &dwLenName, NULL, &dwType, (LPBYTE)pszValue, &dwLenValue);
	}

	BOOL ValueSZ(LPCTSTR szName, LPTSTR szValue, DWORD dwLen) {
		if(!m_hKey) return FALSE;
		dwLen *= sizeof(TCHAR); // convert length in chars to bytes
		return ERROR_SUCCESS==RegQueryValueEx(m_hKey, szName, NULL, NULL, (LPBYTE)szValue, &dwLen);
	}

	DWORD ValueBinary(LPCTSTR szName, LPBYTE lpbValue, DWORD dwLen) {
		if(!m_hKey) return FALSE;
		DWORD dwLenWant = dwLen;
		if(ERROR_SUCCESS==RegQueryValueEx(m_hKey, szName, NULL, NULL, lpbValue, &dwLen))
			return dwLen;
		else
			return 0;
	}

	LPCTSTR ValueSZ(LPCTSTR szName);

	LPBYTE ValueBinary(LPCTSTR szName) {
		return (LPBYTE)ValueSZ(szName);
	}

	DWORD ValueDW(LPCTSTR szName, DWORD dwDefault=0) {
		if(!m_hKey) return FALSE;
		DWORD dwValue = dwDefault;
		DWORD dwLen = sizeof(DWORD);
		RegQueryValueEx(m_hKey, szName, NULL, NULL, (LPBYTE)&dwValue, &dwLen);
		return dwValue;
	}

	BOOL SetSZ(LPCTSTR szName, LPCTSTR szValue, DWORD dwLen) {
		//Prefix
		if(!m_hKey) return FALSE;
		//
		return ERROR_SUCCESS==RegSetValueEx(m_hKey, szName, 0, REG_SZ, (LPBYTE)szValue, sizeof(TCHAR)*dwLen);
	}
	
	BOOL SetSZ(LPCTSTR szName, LPCTSTR szValue) {
		return SetSZ(szName, szValue, 1+lstrlen(szValue));
	}

	BOOL SetDW(LPCTSTR szName, DWORD dwValue) {
		//Prefix
		if(!m_hKey) return FALSE;
		//
		return ERROR_SUCCESS==RegSetValueEx(m_hKey, szName, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
	}
	
	BOOL SetBinary(LPCTSTR szName, LPBYTE lpbValue, DWORD dwLen) {
		//Prefix
		if(!m_hKey) return FALSE;
		//
		return ERROR_SUCCESS==RegSetValueEx(m_hKey, szName, 0, REG_BINARY, lpbValue, dwLen);
	}

	BOOL SetMultiSZ(LPCTSTR szName, LPCTSTR lpszValue, DWORD dwLen) {
		return ERROR_SUCCESS==RegSetValueEx(m_hKey, szName, 0, REG_MULTI_SZ, (LPBYTE)lpszValue, sizeof(TCHAR)*dwLen);
	}

	BOOL DeleteValue(LPCTSTR szName) {
		//Prefix
		if(!m_hKey) return FALSE;
		//
		return ERROR_SUCCESS==RegDeleteValue(m_hKey, szName);
	}

};
#endif // !defined(AFX_REG_H__7D84F1A8_B561_47C0_BE6E_565A49477128__INCLUDED_)
