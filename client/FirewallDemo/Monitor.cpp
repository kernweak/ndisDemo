// Monitor.cpp : implementation file
//

#include "stdafx.h"
#include "FirewallDemo.h"
#include "Monitor.h"
#include "Common.h"
#include "AddRule.h"
#include "FirewallDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitor dialog


CMonitor::CMonitor(CWnd* pParent /*=NULL*/)
	: CPasseckDialog(CMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonitor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iListIndex = -1;
}


void CMonitor::DoDataExchange(CDataExchange* pDX)
{
	CPasseckDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitor)
	DDX_Control(pDX, IDC_FILTERLIST, m_rules);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
}


BEGIN_MESSAGE_MAP(CMonitor, CPasseckDialog)
	//{{AFX_MSG_MAP(CMonitor)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitor message handlers

void CMonitor::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CPasseckDialog::OnPaint() for painting messages
}

HBRUSH CMonitor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPasseckDialog::OnCtlColor(pDC, pWnd, nCtlColor);
 
	if(nCtlColor==CTLCOLOR_STATIC)
	{ 
	pDC->SetBkColor (RGB(255,255,0));//EDIT控件文字背景为黄色 
	return m_hBrush; 
	} 
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CMonitor::OnInitDialog() 
{
	CPasseckDialog::OnInitDialog();
	
	AddListHead(&m_rules, RULE_HEADER, RULE_HEADER_COUNT, RULE_HEADER_LENTH);
	m_allrules.rules_num = 0;

	//Init Button
	m_btnAdd.SetThemeHelper(&m_ThemeHelper);
	m_btnAdd.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnAdd.SetTooltipText(_T("Add a Rule"));

	m_btnDelete.SetThemeHelper(&m_ThemeHelper);
	m_btnDelete.SetTooltipText(_T("Delete a Rule"));

	if(m_rules.GetItemCount() <= 0)
	{
		m_btnDelete.EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonitor::OnAdd() 
{
	CAddRule dlg;
	pRuleLog pRule=dlg.GetRuleLog();

//	int nIndex = m_rules.GetItemCount();
//	if(nIndex == 0)
//		pRule->id = 1;
//	else
//		pRule->id = atol(m_rules.GetItemText(nIndex - 1, 0)) + 1;
		pRule->id = m_allrules.rules_num;

	pRule->d_ip=0;
	pRule->s_ip=0;
	pRule->d_mask=4294967295;
	pRule->s_mask=4294967295;
	pRule->d_port=0;
	pRule->s_port=0;
	pRule->direct = ACL_DIRECTION_IN;
	pRule->action=ACL_ACTION_PASS;
	pRule->protocol=ACL_SERVICE_TYPE_TCP;
	CString tmpStrMemo="";
	_tcscpy(pRule->sMemo, tmpStrMemo);
	if(m_allrules.rules_num < 255)
	{
		int iRet = dlg.DoModal();
		if(iRet == IDCANCEL) return;
		else
		{
			AddRule(&m_rules, pRule, 255, FALSE, FALSE);
			m_allrules.RuleLogs[m_allrules.rules_num++]=*pRule;
			CFirewallDemoDlg* pDlg=(CFirewallDemoDlg*)AfxGetMainWnd();
			pDlg->InstallRules(pRule);
			m_btnDelete.EnableWindow(TRUE);
		}

	}
	else
		AfxMessageBox("No more Rules are allowed.");
	
}


void CMonitor::AddRule(CListCtrl *pList, RuleLog *pRuleLog, int nMaxCount, BOOL IsSeleted, BOOL IsShowDate)
{

	CString sString[RULE_HEADER_COUNT];
	sString[0].Format("%u", pRuleLog->id);
	sString[1].Format("%s/%s", GUI_SERVICE_TYPE[pRuleLog->protocol], GUI_DIRECTION[pRuleLog->direct]);
	sString[2].Format("%s/%u", DIPToSIP(&pRuleLog->s_ip), pRuleLog->s_port);		
	sString[3].Format("%s/%u", DIPToSIP(&pRuleLog->d_ip), pRuleLog->d_port);
	sString[4].Format("%s", GUI_ACTION[pRuleLog->action]);
	sString[5]=pRuleLog->sMemo;

#ifdef _debug
	OutputDebugString(sString[0]);
	OutputDebugString(sString[1]);
	OutputDebugString(sString[2]);
	OutputDebugString(sString[3]);
	OutputDebugString(sString[4]);
	OutputDebugString(sString[5]);
#endif
	
	AddLog(pList, (LPCTSTR*)sString, RULE_HEADER_COUNT, nMaxCount, IsSeleted);
}



void CMonitor::OnDelete() 
{
	int tmpIndex=m_iListIndex,id;
	m_iListIndex=m_rules.GetNextItem(-1,LVNI_SELECTED); 
	id=atol(m_rules.GetItemText(m_iListIndex, 0));

	pRuleLog pRule=&m_allrules.RuleLogs[id];
	CFirewallDemoDlg* pDlg=(CFirewallDemoDlg*)AfxGetMainWnd();
	pDlg->DelOneRule(pRule);

	m_rules.DeleteItem(m_iListIndex);
	if(m_rules.GetItemCount() <= 0)
	{
		m_btnDelete.EnableWindow(FALSE);
		return;
	}
/*
	if(tmpIndex == m_rules.GetItemCount())	
		tmpIndex -- ;

	m_rules.SetItemState(tmpIndex, LVIS_SELECTED, LVIS_SELECTED);
*/	
}
