// DescriptionTMTxDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WifiMgr.h"
#include "DescriptionTMTxDialog.h"


// CDescriptionTMTxDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDescriptionTMTxDialog, CDialog)

CDescriptionTMTxDialog::CDescriptionTMTxDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDescriptionTMTxDialog::IDD, pParent)
{

}

CDescriptionTMTxDialog::~CDescriptionTMTxDialog()
{
}

void CDescriptionTMTxDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDescriptionTMTxDialog, CDialog)
END_MESSAGE_MAP()


// CDescriptionTMTxDialog 메시지 처리기입니다.
