#pragma once
#include <BBS_CommFunc.h>
#include <BBS_Version.h>
#include <BBS_NdisFuncs.h>
#include <BBS_Ping.h>


#define UPDATE_TIMERID_INFORMATION				(1)
#define TIMER_INTERVAL_INFORMATION				(1000)


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


	// 메뉴바
	SHMENUBARINFO m_mbi;

	// 통신가능상태 체크(Ping Check) 결과
	BOOL bPingResult;

	// BSSID 가져오기 성공 여부 (접속된 MAC 확인가능?)
	BOOL bSuccessGettingBSSID;

public:
	// 아답터 정보 및 IP 정보 업데이트
	BOOL UpdateAdpaterIPInfo(PBBS_ADAPTER_INFO pIPInfo);

	// 네트워크 정보 얻어오는 함수
	BOOL UpdateAdpaterNetworkInfo();

	// 모든 컨트롤 내용 지우기
	void ClearAllControls();

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
};
