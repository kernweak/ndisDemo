#if !defined(AFX_LOG_H__D1A642EF_9756_443D_AFB3_2503EC0DE0E9__INCLUDED_)
#define AFX_LOG_H__D1A642EF_9756_443D_AFB3_2503EC0DE0E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Log.h : header file
//
#include "fw_log.h"

/////////////////////////////////////////////////////////////////////////////
// CLog dialog

class CLog : public CPasseckDialog
{
// Construction
public:
	CLog(CWnd* pParent = NULL);   // standard constructor
	void addlist(Log *pLog);
	CListCtrl m_logs;

// Dialog Data
	//{{AFX_DATA(CLog)
	enum { IDD = IDD_LOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLog)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOG_H__D1A642EF_9756_443D_AFB3_2503EC0DE0E9__INCLUDED_)
