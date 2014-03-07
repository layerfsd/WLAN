// PingCheckDlg.h : header file
//

#pragma once



// CPingCheckDlg dialog
class CPingCheckDlg : public CDialog
{
// Construction
public:
	CPingCheckDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PINGCHECK_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnStopCheck(WPARAM wParam, LPARAM lParam);


private :
	DWORD m_dwCheckNBreakInterval;
	DWORD m_dwPingInterval;
	DWORD m_dwMaxCheckCount;
	TCHAR m_tszPrevCheckDestIP[18];
	TCHAR m_tszCustomDestIP[18];
	BOOL m_bPinging;
	HANDLE hPingThread;
	CBBS_Ping *m_pPing;
	BBS_ADAPTER_INFO m_adapterInfo;
	BOOL m_bEnableFileLog;
	CString m_strLogFilePath;

public:
	static DWORD PingCheckThreadHandler(LPVOID pvarg);
	void PingCheckThread(void);
	afx_msg void OnDestroy();
	void PingCheckDbgMsg(BOOL bTimeStamp, LPVOID strFormat, ...);
};
