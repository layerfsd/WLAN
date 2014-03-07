#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// for REBOOT
extern "C" { BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);}

#define FILE_DEVICE_HAL 0x00000101
#define METHOD_BUFFERED                 0
#define FILE_ANY_ACCESS                 0

#define CTL_CODE( DeviceType, Function, Method, Access ) ( ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) )

#define IOCTL_HAL_REBOOT                        CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)





// CNetSettingPage_Setting_USIView 폼 뷰입니다.

class CNetSettingPage_Setting_USIView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Setting_USIView)

protected:
	CNetSettingPage_Setting_USIView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CNetSettingPage_Setting_USIView();

public:
	enum { IDD = IDD_NETSETTINGPAGE_SETTING_USIVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();


private:

	CString m_strRoamTrigger;
	DWORD	m_dwRoamScanFreq;
	DWORD	m_dwRoamScanPrd;
	DWORD	m_dwFullRoamScanInterval;
	BOOL	m_bRoamEnable;

public:
	afx_msg void OnDefault();
	afx_msg void OnApply();
	// Spin 컨트롤 : RoamDelta
	CSpinButtonCtrl m_spinRoamDelta;
	// Spin 컨트롤 : PowerSaveMode
	CSpinButtonCtrl m_spinPowerSaveMode;
	afx_msg void OnDestroy();
	// 콤보박스 컨트롤 변수 : RoamTrigger
	CComboBox m_comboRoamTrigger;
};


