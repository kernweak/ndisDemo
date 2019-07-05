// FirewallDemo.h : main header file for the FIREWALLDEMO application
//

#if !defined(AFX_FIREWALLDEMO_H__6A3A19BB_08FD_4D81_96CC_64AEE2D0CADD__INCLUDED_)
#define AFX_FIREWALLDEMO_H__6A3A19BB_08FD_4D81_96CC_64AEE2D0CADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <winioctl.h>
#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CFirewallDemoApp:
// See FirewallDemo.cpp for the implementation of this class
//

class CFirewallDemoApp : public CWinApp
{
public:
	CFirewallDemoApp();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFirewallDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFirewallDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsAlreadyRun();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALLDEMO_H__6A3A19BB_08FD_4D81_96CC_64AEE2D0CADD__INCLUDED_)
