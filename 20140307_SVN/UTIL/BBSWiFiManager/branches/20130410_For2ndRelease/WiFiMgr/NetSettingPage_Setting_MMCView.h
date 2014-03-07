#pragma once
#include "afxwin.h"



// for REBOOT
extern "C" { BOOL KernelIoControl(DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned);}

#define FILE_DEVICE_HAL 0x00000101
#define METHOD_BUFFERED                 0
#define FILE_ANY_ACCESS                 0

#define CTL_CODE( DeviceType, Function, Method, Access ) ( ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) )

#define IOCTL_HAL_REBOOT                        CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)







// CNetSettingPage_Setting_MMCView 폼 뷰입니다.

class CNetSettingPage_Setting_MMCView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Setting_MMCView)

protected:
	CNetSettingPage_Setting_MMCView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CNetSettingPage_Setting_MMCView();

public:
	enum { IDD = IDD_NETSETTINGPAGE_SETTING_MMCVIEW };
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
	afx_msg void OnDestroy();
	afx_msg void OnDefault();
	afx_msg void OnApply();

	BOOL m_bRoamEnable;
	BOOL m_bBKScanEnable;

	DWORD m_dwScan_BKScanPrd;
	DWORD m_dwScan_NodeAge;

	INT m_nRoam_ScanStart;
	INT m_nRoam_RoamStart;
	INT m_nRoam_TargetRSSI;

	// 콤보박스 컨트롤 변수 : ScanThreshold
	CComboBox m_comboRoam_ScanStart;
	// 콤보박스 컨트롤 변수 : RoamThreshold
	CComboBox m_comboRoam_RoamStart;
	// 콤보박스 컨트롤 변수 : RoamFloorThre
	CComboBox m_comboRoam_TargetRSSI;
	// 콤보박스 컨트롤 변수 : bkScanPeriod
	CComboBox m_comboScan_BKScanPrd;
	// 콤보박스 컨트롤 변수 : nodeAge
	CComboBox m_comboScan_NodeAge;
	// 컨트롤 내용 업데이트
	BOOL UpdateControlState();
};


