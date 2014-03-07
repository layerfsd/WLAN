
#include "GeneralDlg.h"
#include "SecurityDlg.h"
#include "MiscDlg.h"
#include "AboutDlg.h"
#include "Config.h"

#pragma once

#define _MAX_TAB_DLG_COUNT	4

#define _GENERAL_DLG_INDEX	0
#define _SECURITY_DLG_INDEX	1
#define _MISC_DLG_INDEX		2
#define _ABOUT_DLG_INDEX	3

#define _szGENERAL_DLG_NAME		"General"
#define _szSECURITY_DLG_NAME	"Security"
#define _szMISC_DLG_NAME		"Misc"
#define _szABOUT_DLG_NAME		"About"

typedef struct _ST_PARENT_ARG{
	CConfig *pConfig;
	ST_GENERAL_ARG stGeneralArg;
	ST_SECURITY_ARG stSecurityArg;
	HWND hbnCommitMain;
}ST_PARENT_ARG;

// CParentTab

class CParentTab : public CTabCtrl
{
	DECLARE_DYNAMIC (CParentTab)
	CParentTab ();
	void SetDialogPosition ();

public:
	CDialog *m_pTabDlgs [_MAX_TAB_DLG_COUNT];
	int m_iCurrentTab;
	int m_iNoOfTabs;
	HWND m_hbnCommitMain;

public:
	CParentTab (ST_PARENT_ARG *pstParentArg);
	virtual ~CParentTab ();

	void InitDialogs ();
protected:

	afx_msg void OnLButtonDown (UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP ()
};


