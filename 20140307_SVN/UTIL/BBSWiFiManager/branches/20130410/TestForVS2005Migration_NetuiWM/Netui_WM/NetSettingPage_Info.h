#pragma once
#include <BBS_CommFunc.h>



#define UPDATE_TIMERID_IP_INFORMATION			(1)
#define UPDATE_TIMERID_NETWORK_INFORMATION		(2)
#define TIMER_INTERVAL_IP_INFORMATION			(2000)
#define TIMER_INTERVAL_NETWORK_INFORMATION		(4000)


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

	// ���� ��Ʈ��ũ ������ ���� ����ü
	BBS_CUR_NET_INFO m_CurNetInfo;

	// �޴���
	SHMENUBARINFO m_mbi;

public:
	// �ƴ��� ���� �� IP ���� ������Ʈ
	BOOL UpdateAdpaterIPInfo(PBBS_ADAPTER_INFO pIPInfo);

	// ���� ��Ʈ��ũ ���� ������ �Լ�
	BOOL UpdateAdpaterNetworkInfo(PBBS_CUR_NET_INFO pNetworkInfo);

	// ���� �������� ���� ���� ������
	BOOL GetCurrentFileVersion(LPTSTR tszFileVer);


protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
