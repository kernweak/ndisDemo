#if !defined(AFX_ADDRULE_H__75D59285_2FCE_4B06_97A9_07ED5C9AA6DD__INCLUDED_)
#define AFX_ADDRULE_H__75D59285_2FCE_4B06_97A9_07ED5C9AA6DD__INCLUDED_

#include "..\common\Filt.h"	
#include "..\COMMON\XPStyleButtonST.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddRule.h : header file
#include <afxcmn.h>

/////////////////////////////////////////////////////////////////////////////
// CAddRule dialog
#define ACL_SET_CTRL_SOUR_IP    	0
#define ACL_SET_CTRL_SOUR_MASK		1
#define ACL_SET_CTRL_DIST_IP		2
#define ACL_SET_CTRL_DIST_MASK		3

static UINT ACL_SET_CTRL[] = {
	IDC_CTRL_SOUR_IP,
	IDC_CTRL_SOUR_MASK,
	IDC_CTRL_DIST_IP,
	IDC_CTRL_DIST_MASK,
};
#define ACL_SET_CTRL_COUNT       sizeof(ACL_SET_CTRL)/sizeof(UINT)

#define ACL_SET_COMBO_DIR		0
#define ACL_SET_COMBO_ACTION	1
#define ACL_SET_COMBO_PROTOCOL	2

static UINT ACL_SET_COMBO[] = {
	IDC_COMBO_DIRECTION,
	IDC_COMBO_ACTION,
	IDC_COMBO_PROTOCAL,
};
#define ACL_SET_COMBO_COUNT		sizeof(ACL_SET_COMBO)/sizeof(UINT)

#define ACL_SET_EDIT_DIST_PORT		0
#define ACL_SET_EDIT_SOUR_PORT	    1
#define ACL_SET_EDIT_MEMO			2
static UINT ACL_SET_EDIT[] = {
	IDC_EDIT_DIST_PORT,
	IDC_EDIT_SOUR_PORT,
	IDC_EDIT_MEMO,
};
#define ACL_SET_EDIT_COUNT		sizeof(ACL_SET_EDIT)/sizeof(UINT)

/////////////////////////////////////////////////////////////////////////////

class CAddRule : public CDialog
{
// Construction
public:
	pRuleLog GetRuleLog(){return &m_RuleLog;}
	CAddRule(CWnd* pParent = NULL);   // standard constructor
	void CAddRule::ShowAcl();
	CString MakeExplain(BOOL bIsAppSelChange = FALSE);

private:
	void InitDialog();

// Dialog Data
	//{{AFX_DATA(CAddRule)
	enum { IDD = IDD_RULE_ADD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddRule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddRule)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAction();
	afx_msg void OnSelchangeComboDirection();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CXPStyleButtonST m_btnCancel;
	CXPStyleButtonST m_btnApply;
	CThemeHelperST		m_ThemeHelper;

	RuleLog m_RuleLog;
	CComboBox m_Combo[ACL_SET_COMBO_COUNT];
	CEdit	m_Edit[ACL_SET_EDIT_COUNT];
	CIPAddressCtrl m_IPCtrl[ACL_SET_CTRL_COUNT];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDRULE_H__75D59285_2FCE_4B06_97A9_07ED5C9AA6DD__INCLUDED_)
