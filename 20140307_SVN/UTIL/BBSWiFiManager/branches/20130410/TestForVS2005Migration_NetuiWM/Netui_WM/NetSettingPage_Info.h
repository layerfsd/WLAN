#pragma once
#include <BBS_CommFunc.h>



#define UPDATE_TIMERID_IP_INFORMATION			(1)
#define UPDATE_TIMERID_NETWORK_INFORMATION		(2)
#define TIMER_INTERVAL_IP_INFORMATION			(2000)
#define TIMER_INTERVAL_NETWORK_INFORMATION		(4000)


// CNetSettingPage_Info 대화 상자입니다.

class CNetSettingPage_Info : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetSettingPage_Info)

public:
	CNetSettingPage_Info();
	virtual ~CNetSettingPage_Info();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NETSETTINGPAGE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


private :

	// Adapter 정보를 담은 구조체
	BBS_ADAPTER_INFO m_AdapterInfo;

	// 현재 네트워크 정보를 담은 구조체
	BBS_CUR_NET_INFO m_CurNetInfo;

	// 메뉴바
	SHMENUBARINFO m_mbi;

public:
	// 아답터 정보 및 IP 정보 업데이트
	BOOL UpdateAdpaterIPInfo(PBBS_ADAPTER_INFO pIPInfo);

	// 현재 네트워크 정보 얻어오는 함수
	BOOL UpdateAdpaterNetworkInfo(PBBS_CUR_NET_INFO pNetworkInfo);

	// 현재 실행모듈의 파일 버전 얻어오기
	BOOL GetCurrentFileVersion(LPTSTR tszFileVer);


protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
