#pragma once


// CDescriptionTMTxDialog ��ȭ �����Դϴ�.

class CDescriptionTMTxDialog : public CDialog
{
	DECLARE_DYNAMIC(CDescriptionTMTxDialog)

public:
	CDescriptionTMTxDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDescriptionTMTxDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DESCRIPTIONTMTXDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
