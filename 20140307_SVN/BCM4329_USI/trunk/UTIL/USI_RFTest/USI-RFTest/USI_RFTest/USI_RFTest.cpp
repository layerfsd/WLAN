// USI_RFTest.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "USI_RFTest.h"
#include "USI_RFTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUSI_RFTestApp

BEGIN_MESSAGE_MAP(CUSI_RFTestApp, CWinApp)
END_MESSAGE_MAP()


// CUSI_RFTestApp ����
CUSI_RFTestApp::CUSI_RFTestApp()
	: CWinApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CUSI_RFTestApp ��ü�Դϴ�.
CUSI_RFTestApp theApp;

// CUSI_RFTestApp �ʱ�ȭ

BOOL CUSI_RFTestApp::InitInstance()
{
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
    // CAPEDIT �� SIPPREF�� ���� ��� Windows Mobile ���� ��Ʈ���� �ʱ�ȭ�ϱ� ����
    // ���� ���α׷��� �ʱ�ȭ�ϴ� ���� SHInitExtraControls�� �� �� ȣ���ؾ� �մϴ�.
    SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ 
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	CUSI_RFTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.
	return FALSE;
}
