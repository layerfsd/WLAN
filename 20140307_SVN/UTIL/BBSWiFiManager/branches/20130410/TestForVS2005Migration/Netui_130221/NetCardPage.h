#pragma once





// CNetCardPage ��ȭ �����Դϴ�.

class CNetCardPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CNetCardPage)

public:
	CNetCardPage();
	virtual ~CNetCardPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NETUI_ADAPTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
