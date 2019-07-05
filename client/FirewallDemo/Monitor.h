#if !defined(AFX_MONITOR_H__E4535F2A_D3A1_456D_9602_74F76D151659__INCLUDED_)
#define AFX_MONITOR_H__E4535F2A_D3A1_456D_9602_74F76D151659__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Monitor.h : header file
//
#include "..\COMMON\XPStyleButtonST.h"
#include "..\common\Filt.h"	

/////////////////////////////////////////////////////////////////////////////
// CMonitor dialog

class CMonitor : public CPasseckDialog
{
// Construction
public:
	All_Rules m_allrules;
	HBRUSH m_hBrush;

	CMonitor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMonitor)
	enum { IDD = IDD_MONITOR };
	CListCtrl	m_rules;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonitor)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_iListIndex;

	CXPStyleButtonST m_btnAdd;
	CXPStyleButtonST m_btnDelete;
	CThemeHelperST		m_ThemeHelper;

	void AddRule(CListCtrl* pList, RuleLog* pRuleLog, int nMaxCount, BOOL IsSeleted, BOOL IsShowDate);
//	CListCtrl m_rules;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITOR_H__E4535F2A_D3A1_456D_9602_74F76D151659__INCLUDED_)
