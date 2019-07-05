#if !defined(AFX_NETSTATE_H__4BA60D25_C00F_4142_BAE0_E61C6A8B30AF__INCLUDED_)
#define AFX_NETSTATE_H__4BA60D25_C00F_4142_BAE0_E61C6A8B30AF__INCLUDED_

#include "..\common\HistogramCtrl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetState.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetState dialog

class CNetState : public CPasseckDialog
{
// Construction
public:
	CNetState(CWnd* pParent = NULL);   // standard constructor
	CHistogramCtrl* GetCtlHistogram(){return &m_ctlHistogram;}

// Dialog Data
	//{{AFX_DATA(CNetState)
	enum { IDD = IDD_NETSTATE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetState)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetState)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CHistogramCtrl m_ctlHistogram;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETSTATE_H__4BA60D25_C00F_4142_BAE0_E61C6A8B30AF__INCLUDED_)
