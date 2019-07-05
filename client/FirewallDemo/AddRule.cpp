// AddRule.cpp : implementation file
//

#include "stdafx.h"
#include "FirewallDemo.h"
#include "AddRule.h"
#include "common.h"
#include "..\common\Filt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddRule dialog

CAddRule::CAddRule(CWnd* pParent /*=NULL*/)
	: CDialog(CAddRule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddRule)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddRule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddRule)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDAPPLY, m_btnApply);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CAddRule, CDialog)
	//{{AFX_MSG_MAP(CAddRule)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION, OnSelchangeComboAction)
	ON_CBN_SELCHANGE(IDC_COMBO_DIRECTION, OnSelchangeComboDirection)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddRule message handlers

BOOL CAddRule::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for(int i = 0; i < ACL_SET_CTRL_COUNT; i++)
	m_IPCtrl[i].SubclassDlgItem(ACL_SET_CTRL[i], this);
	for(i = 0; i < ACL_SET_COMBO_COUNT; i++)
	m_Combo[i].SubclassDlgItem(ACL_SET_COMBO[i], this);
	for(i = 0; i < ACL_SET_EDIT_COUNT; i++)
	m_Edit[i].SubclassDlgItem(ACL_SET_EDIT[i], this);

	InitDialog();
	ShowAcl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddRule::InitDialog()
{
	//Init Button
	m_btnApply.SetThemeHelper(&m_ThemeHelper);
	m_btnApply.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnApply.SetTooltipText(_T("Apply the change"));

	m_btnCancel.SetThemeHelper(&m_ThemeHelper);
	m_btnCancel.SetTooltipText(_T("Cancel the change"));

	AddComboStrings(&m_Combo[ACL_SET_COMBO_DIR], GUI_DIRECTION, GUI_DIRECTION_COUNT);
	AddComboStrings(&m_Combo[ACL_SET_COMBO_ACTION], GUI_ACTION, GUI_ACTION_COUNT);
	AddComboStrings(&m_Combo[ACL_SET_COMBO_PROTOCOL], GUI_SERVICE_TYPE, GUI_SERVICE_TYPE_COUNT);

	m_Edit[ACL_SET_EDIT_SOUR_PORT].SetLimitText(5);
	m_Edit[ACL_SET_EDIT_DIST_PORT].SetLimitText(5);
	m_Edit[ACL_SET_EDIT_MEMO].SetLimitText(50);

	CString Title;
	Title.Format("控管规则 %d",m_RuleLog.id);
	SetWindowText(Title);

}

void CAddRule::ShowAcl()
{
	CString sString;
	m_Combo[ACL_SET_COMBO_ACTION].SetCurSel(m_RuleLog.action);
	m_Combo[ACL_SET_COMBO_DIR].SetCurSel(m_RuleLog.direct);
	m_Combo[ACL_SET_COMBO_PROTOCOL].SetCurSel(m_RuleLog.protocol);

	m_IPCtrl[ACL_SET_CTRL_DIST_IP].SetAddress(m_RuleLog.d_ip);
	m_IPCtrl[ACL_SET_CTRL_DIST_MASK].SetAddress(m_RuleLog.d_mask);
	m_IPCtrl[ACL_SET_CTRL_SOUR_IP].SetAddress(m_RuleLog.s_ip);
	m_IPCtrl[ACL_SET_CTRL_SOUR_MASK].SetAddress(m_RuleLog.s_mask);

	sString.Format("%u",m_RuleLog.s_port);
	m_Edit[ACL_SET_EDIT_SOUR_PORT].	SetWindowText(sString);
	sString.Format("%u",m_RuleLog.d_port);
	m_Edit[ACL_SET_EDIT_DIST_PORT].	SetWindowText(sString);

	m_Edit[ACL_SET_EDIT_MEMO].SetWindowText(m_RuleLog.sMemo);

}

void CAddRule::OnSelchangeComboAction() 
{
	MakeExplain();	
}

CString CAddRule::MakeExplain(BOOL bIsAppSelChange)
{

	CString sExplain;
	CString sCombo[ACL_SET_COMBO_COUNT];
/*
	for(int i = 0; i < ACL_SET_COMBO_COUNT; i++)
	{
		m_Combo[i].GetWindowText(sCombo[i]);
	}
	
	if(m_Combo[ACL_SET_COMBO_DIR].GetCurSel() == ACL_DIRECTION_IN)
	{
		// _T("%s%s通过%s协议的%s端口向本机%s%s端口发出的连接请求将被%s。%s")
		sExplain.Format(GUI_ACL_EXPLAIN_OUT_FORMAT
			, sCombo[ACL_SET_COMBO_NET]
			, sCombo[ACL_SET_COMBO_TIME]
			, sCombo[ACL_SET_COMBO_PROTOCOL]
			, sEdit[ACL_SET_EDIT_SERVICE_PORT].Compare("0") == 0 ? GUI_ACL_EXPLAIN_PORT_ALL : sEdit[ACL_SET_EDIT_SERVICE_PORT] 
			, sCombo[ACL_SET_COMBO_APP].Compare("*") == 0 ? _T("") : sCombo[ACL_SET_COMBO_APP]
			, sEdit[ACL_SET_EDIT_LOCAL_PORT].Compare("0") == 0 ? GUI_ACL_EXPLAIN_PORT_ALL : sEdit[ACL_SET_EDIT_LOCAL_PORT] 
			, sCombo[ACL_SET_COMBO_ACTION]
			, GUI_ACL_EXPLAIN_CONST
			);
	}
	else
	{
		// _T("%s%s通过%s协议的%s端口访问%s的%s端口将被%s。%s")
		sExplain.Format(GUI_ACL_EXPLAIN_OUT_FORMAT
			, sCombo[ACL_SET_COMBO_APP].Compare("*") == 0 ? _T("") : sCombo[ACL_SET_COMBO_APP]
			, sCombo[ACL_SET_COMBO_TIME]
			, sCombo[ACL_SET_COMBO_PROTOCOL]
			, sEdit[ACL_SET_EDIT_LOCAL_PORT].Compare("0") == 0 ? GUI_ACL_EXPLAIN_PORT_ALL : sEdit[ACL_SET_EDIT_LOCAL_PORT] 
			, sCombo[ACL_SET_COMBO_NET]
			, sEdit[ACL_SET_EDIT_SERVICE_PORT].Compare("0") == 0 ? GUI_ACL_EXPLAIN_PORT_ALL : sEdit[ACL_SET_EDIT_SERVICE_PORT] 
			, sCombo[ACL_SET_COMBO_ACTION]
			, GUI_ACL_EXPLAIN_CONST
			);
		if(m_Combo[ACL_SET_COMBO_DIR].GetCurSel() == ACL_DIRECTION_IN_OUT)
			sExplain += '.';
	}

	char sSpace[201]; memset(sSpace, ' ', 200); sSpace[200] = 0;
	m_Label[ACL_SET_LABEL_EXPAIN].SetWindowText(sSpace);
	m_sLastExplain = sExplain;
	m_Label[ACL_SET_LABEL_EXPAIN].SetWindowText(sExplain);
*/
	return sExplain;
}

void CAddRule::OnSelchangeComboDirection() 
{
	MakeExplain();
	
}

void CAddRule::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}


void CAddRule::OnOK() 
{
}

void CAddRule::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CAddRule::OnApply() 
{
	CString tmpStrMemo, tmpStrDistPort, tmpStrSourPort;

	m_Edit[ACL_SET_EDIT_MEMO].GetWindowText(tmpStrMemo);
	m_Edit[ACL_SET_EDIT_SOUR_PORT].GetWindowText(tmpStrSourPort);
	m_Edit[ACL_SET_EDIT_DIST_PORT].GetWindowText(tmpStrDistPort);

	UINT tmpSourPort = _ttoi(tmpStrSourPort);
	if(tmpSourPort > 65535 || tmpSourPort < 0)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INAVALID_PORT);
		m_Edit[ACL_SET_EDIT_SOUR_PORT].SetFocus();
		return;
	}

	UINT tmpDistPort = _ttoi(tmpStrDistPort);
	if(tmpDistPort > 65535 || tmpDistPort < 0)
	{
		AfxMessageBox(GUI_ACL_MESSAGE_INAVALID_PORT);
		m_Edit[ACL_SET_EDIT_DIST_PORT].SetFocus();
		return;
	}

//	if(tmpStrMemo.Compare(m_RuleLog.sMemo) != 0)
	if(1)
	{
		m_IPCtrl[ACL_SET_CTRL_DIST_IP].GetAddress(m_RuleLog.d_ip);
		m_IPCtrl[ACL_SET_CTRL_DIST_MASK].GetAddress(m_RuleLog.d_mask);
		m_IPCtrl[ACL_SET_CTRL_SOUR_IP].GetAddress(m_RuleLog.s_ip);
		m_IPCtrl[ACL_SET_CTRL_SOUR_MASK].GetAddress(m_RuleLog.s_mask);

		m_RuleLog.protocol   	= m_Combo[ACL_SET_COMBO_PROTOCOL].GetCurSel();	
		m_RuleLog.action		= m_Combo[ACL_SET_COMBO_ACTION].GetCurSel();
		m_RuleLog.direct		= m_Combo[ACL_SET_COMBO_DIR].GetCurSel();

		m_RuleLog.s_port		= tmpSourPort;	
		m_RuleLog.d_port		= tmpDistPort;		

		_tcscpy(m_RuleLog.sMemo, tmpStrMemo);

//		m_bIsChange = TRUE;
	}

	CDialog::OnOK();
	
}
