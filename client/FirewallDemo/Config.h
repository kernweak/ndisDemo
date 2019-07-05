#if !defined(AFX_CONFIG_H__43E1843C_1522_421B_96A9_2B6AB33605EC__INCLUDED_)
#define AFX_CONFIG_H__43E1843C_1522_421B_96A9_2B6AB33605EC__INCLUDED_

#include "..\COMMON\XPStyleButtonST.h"	
#include "..\common\Filt.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Config.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfig dialog

class CConfig : public CPasseckDialog
{
// Construction
public:
	CConfig(CWnd* pParent = NULL);   // standard constructor
	pConfig GetSysConfig(){return &m_SysConfig;}

// Dialog Data
	//{{AFX_DATA(CConfig)
	enum { IDD = IDD_CONFIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfig)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CXPStyleButtonST m_btnCancel;
	CXPStyleButtonST m_btnApply;

	CButton		 m_CheckAutoStart;
	CButton		 m_CheckMode;
	CButton		 m_CheckLog;
	CButton		 m_CheckAlertSpeaker;
	CButton		 m_CheckAlertDialog;
	CButton		 m_CheckDenyTcp;
	CButton		 m_CheckDenyUdp;
	CButton		 m_CheckDenyIcmp;
	Config m_SysConfig;

	CThemeHelperST		m_ThemeHelper;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIG_H__43E1843C_1522_421B_96A9_2B6AB33605EC__INCLUDED_)
