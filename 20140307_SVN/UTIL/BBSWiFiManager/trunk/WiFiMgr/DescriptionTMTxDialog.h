#pragma once


// CDescriptionTMTxDialog 대화 상자입니다.

class CDescriptionTMTxDialog : public CDialog
{
	DECLARE_DYNAMIC(CDescriptionTMTxDialog)

public:
	CDescriptionTMTxDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDescriptionTMTxDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DESCRIPTIONTMTXDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
