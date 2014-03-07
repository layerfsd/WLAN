// USI_RFTestDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"

// CUSI_RFTestDlg 대화 상자
class CUSI_RFTestDlg : public CDialog
{
// 생성입니다.
public:
	CUSI_RFTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USI_RFTEST_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_tabMain;
	DWORD m_dwScreenRate;
	CView *m_pView;
public:
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	void ChangeView(INT nID);

	BOOL GetCurrentFileVersion(LPTSTR tszFileVer);
};
