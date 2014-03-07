// ParentTab.cpp : implementation file
//

#include "stdafx.h"
#include "SoftAP.h"
#include "ParentTab.h"


// CParentTab

IMPLEMENT_DYNAMIC (CParentTab, CTabCtrl)

CParentTab::CParentTab (ST_PARENT_ARG *pstParentArg)
{
	m_pTabDlgs [_GENERAL_DLG_INDEX] = new CGeneralDlg (pstParentArg->pConfig, &pstParentArg->stGeneralArg);
	m_pTabDlgs [_SECURITY_DLG_INDEX] = new CSecurityDlg (pstParentArg->pConfig, &pstParentArg->stSecurityArg);
	m_pTabDlgs [_MISC_DLG_INDEX] = new CMiscDlg (pstParentArg->pConfig);
	m_pTabDlgs [_ABOUT_DLG_INDEX] = new CAboutDlg ();

	m_hbnCommitMain = pstParentArg->hbnCommitMain;

	m_iCurrentTab = 0;
	m_iNoOfTabs = _MAX_TAB_DLG_COUNT;
}

CParentTab::~CParentTab ()
{
	for (int i = 0; i < _MAX_TAB_DLG_COUNT; i++)
	{
		if(m_pTabDlgs [i])
			delete m_pTabDlgs [i];
	}

	m_iCurrentTab = 0;
	m_iNoOfTabs = 0;
}

BEGIN_MESSAGE_MAP (CParentTab, CTabCtrl)
	ON_WM_LBUTTONDOWN ()
END_MESSAGE_MAP ()


// CParentTab message handlers

void CParentTab::InitDialogs ()
{
	m_iCurrentTab = 0;

	InsertItem (_GENERAL_DLG_INDEX, _T (_szGENERAL_DLG_NAME));
	InsertItem (_SECURITY_DLG_INDEX, _T (_szSECURITY_DLG_NAME));
	InsertItem (_MISC_DLG_INDEX, _T (_szMISC_DLG_NAME));
	InsertItem (_ABOUT_DLG_INDEX, _T (_szABOUT_DLG_NAME));

	m_pTabDlgs [_GENERAL_DLG_INDEX]->Create (IDD_GENERAL_DLG, this);
	m_pTabDlgs [_SECURITY_DLG_INDEX]->Create (IDD_SECURITY_DLG, this);
	m_pTabDlgs [_MISC_DLG_INDEX]->Create (IDD_MISC_DLG, this);
	m_pTabDlgs [_ABOUT_DLG_INDEX]->Create (IDD_ABOUT_DLG, this);

	m_pTabDlgs [_GENERAL_DLG_INDEX]->ShowWindow (SW_SHOW);
	m_pTabDlgs [_SECURITY_DLG_INDEX]->ShowWindow (SW_HIDE);
	m_pTabDlgs [_MISC_DLG_INDEX]->ShowWindow (SW_HIDE);
	m_pTabDlgs [_ABOUT_DLG_INDEX]->ShowWindow (SW_HIDE);

	SetDialogPosition ();
}

void CParentTab::SetDialogPosition ()
{
	CRect rectTab, rectItem;
	int iX = 0, iY = 0, icX = 0, icY = 0;

	GetClientRect (&rectTab);
	GetItemRect (0, &rectItem);

	iX = rectTab.left;
	iY = rectTab.top;

	//width
	icX = rectTab.right;

	//height
	icY = rectItem.bottom - 25;

	m_pTabDlgs [_GENERAL_DLG_INDEX]->SetWindowPos (&wndTop, iX, iY, icX, icY, SWP_SHOWWINDOW);

	for (int i = 1; i < m_iNoOfTabs; i++)
		m_pTabDlgs[i]->SetWindowPos (&wndTop, iX, iY, icX, icY, SWP_HIDEWINDOW);
}

void CParentTab::OnLButtonDown (UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown (nFlags, point);

	if (GetCurFocus () != m_iCurrentTab)
	{
		m_pTabDlgs [m_iCurrentTab]->ShowWindow (SW_HIDE);

		m_iCurrentTab = GetCurFocus ();

		m_pTabDlgs [m_iCurrentTab]->ShowWindow (SW_SHOW);
		m_pTabDlgs [m_iCurrentTab]->SetFocus ();
	}

	if (_MISC_DLG_INDEX == m_iCurrentTab ||
		_ABOUT_DLG_INDEX == m_iCurrentTab)
	{
		::EnableWindow(m_hbnCommitMain, FALSE);
	}
	else
	{
		::EnableWindow(m_hbnCommitMain, TRUE);
	}
}
