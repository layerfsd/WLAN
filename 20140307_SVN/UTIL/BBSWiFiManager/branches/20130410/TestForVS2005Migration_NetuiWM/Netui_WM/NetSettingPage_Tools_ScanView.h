#pragma once
#include <BBS_CommFunc.h>
#include <BBS_NdisFuncs.h>
#include "afxcmn.h"
#include "afxwin.h"



#define TIMERID_REFRESH_SCANNER			(1)
#define DEFAULT_TIMER_REFRESH_INTERVAL	(3000)

// CNetSettingPage_Tools_ScanView �� ���Դϴ�.

class CNetSettingPage_Tools_ScanView : public CFormView
{
	DECLARE_DYNCREATE(CNetSettingPage_Tools_ScanView)

protected:
	CNetSettingPage_Tools_ScanView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:


	void UpdateScanAPList();

	// ����Ʈ�ڽ��� Column �ʺ� ����
	void CalculateListColumnWidth(DWORD* pdwListColumnWidthArr, DWORD dwNumofColumns);


private:
	// Adapter ������ ���� ����ü
	BBS_ADAPTER_INFO m_AdapterInfo;

	// ���� BSSID ����
	TCHAR m_tszBSSID[18];

	DWORD m_dwScreenRate;

	// ����Ʈ�ڽ� ��Ʈ
	CFont m_listfont;

	// ��ĵ�� AP ����Ʈ ��Ʈ��
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


