#pragma once
#include <BBS_CommFunc.h>
#include <BBS_Version.h>
#include <BBS_NdisFuncs.h>
#include <BBS_Ping.h>


#define UPDATE_TIMERID_INFORMATION				(1)
#define TIMER_INTERVAL_INFORMATION				(1000)


// CNetSettingPage_Info ��ȭ �����Դϴ�.

class CNetSettingPage_Info : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Info)

public:
	CNetSettingPage_Info();
	virtual ~CNetSettingPage_Info();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NETSETTINGPAGE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


private :

	// Adapter ������ ���� ����ü
	BBS_ADAPTER_INFO m_AdapterInfo;


	// �޴���
	SHMENUBARINFO m_mbi;

	// ��Ű��ɻ��� üũ(Ping Check) ���
	BOOL bPingResult;

	// BSSID �������� ���� ���� (���ӵ� MAC Ȯ�ΰ���?)
	BOOL bSuccessGettingBSSID;

public:
	// �ƴ��� ���� �� IP ���� ������Ʈ
	BOOL UpdateAdpaterIPInfo(PBBS_ADAPTER_INFO pIPInfo);

	// ��Ʈ��ũ ���� ������ �Լ�
	BOOL UpdateAdpaterNetworkInfo();

	// ��� ��Ʈ�� ���� �����
	void ClearAllControls();

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
};
