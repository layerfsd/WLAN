#pragma once
#include <BBS_CommFunc.h>
#include <BBS_NdisFuncs.h>
#include "afxcmn.h"
#include "afxwin.h"



#define TIMERID_REFRESH_SCANNER			(1)
#define DEFAULT_TIMER_REFRESH_INTERVAL	(3000)

// CNetSettingPage_Tools_ScanView 폼 뷰입니다.

class CNetSettingPage_Tools_ScanView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Tools_ScanView)

protected:
	CNetSettingPage_Tools_ScanView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CNetSettingPage_Tools_ScanView();

public:
	enum { IDD = IDD_NETSETTINGPAGE_TOOLS_SCANVIEW };
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


	void UpdateScanAPList();

	// 리스트박스의 Column 너비를 지정
	void CalculateListColumnWidth(DWORD* pdwListColumnWidthArr, DWORD dwNumofColumns);


private:
	// Adapter 정보를 담은 구조체
	BBS_ADAPTER_INFO m_AdapterInfo;

	// 현재 BSSID 저장
	TCHAR m_tszBSSID[18];

	DWORD m_dwScreenRate;

	// 리스트박스 폰트
	CFont m_listfont;

	// 스캔된 AP 리스트 컨트롤
	CListCtrl m_listScanedAP;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMCustomdrawListApscan(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
private:
	CComboBox m_comboScanPeriod;
public:
	afx_msg void OnCbnSelendokComboScannerPeriod();
};


