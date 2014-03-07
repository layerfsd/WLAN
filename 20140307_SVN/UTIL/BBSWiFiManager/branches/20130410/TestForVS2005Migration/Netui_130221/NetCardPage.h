#pragma once





// CNetCardPage 대화 상자입니다.

class CNetCardPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetCardPage)

public:
	CNetCardPage();
	virtual ~CNetCardPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NETUI_ADAPTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
