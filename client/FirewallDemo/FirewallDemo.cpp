// FirewallDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FirewallDemo.h"
#include "FirewallDemoDlg.h"

#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFirewallDemoApp

BEGIN_MESSAGE_MAP(CFirewallDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CFirewallDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirewallDemoApp construction

CFirewallDemoApp::CFirewallDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFirewallDemoApp object

CFirewallDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFirewallDemoApp initialization

BOOL CFirewallDemoApp::InitInstance()
{
	AfxEnableControlContainer();

	if(IsAlreadyRun())
	{
		AfxMessageBox(IDS_INFO_ALREADY_RUN);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSplashWnd::EnableSplashScreen(TRUE);

	CFirewallDemoDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
//-	int nResponse = dlg.DoModal();
//-	if (nResponse == IDOK)
//-	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
//-	}
//-	else if (nResponse == IDCANCEL)
//-	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
//-	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CFirewallDemoApp::IsAlreadyRun()
{
	LPCTSTR szGuid = _T("38A86EFE-9482-4838-9458-0DB0F5DEB378");
	static const UINT WM_CHECK_ITS_ME = RegisterWindowMessage(szGuid);
	HANDLE hMutexOneInstance = CreateMutex( NULL, FALSE, szGuid);
	return ( GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED);
}
