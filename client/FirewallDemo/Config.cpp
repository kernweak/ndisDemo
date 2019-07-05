// Config.cpp : implementation file
//

#include "stdafx.h"
#include "FirewallDemo.h"
#include "FirewallDemoDlg.h"
#include "Config.h"
#include "condef.h"
#include "XInstall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfig dialog


CConfig::CConfig(CWnd* pParent /*=NULL*/)
	: CPasseckDialog(CConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfig::DoDataExchange(CDataExchange* pDX)
{
	CPasseckDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDAPPLY, m_btnApply);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_AUTOSTART, m_CheckAutoStart);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_LOG, m_CheckLog);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_MODE, m_CheckMode);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_ALERT_PCSPEAKER, m_CheckAlertSpeaker);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_ALERT_DIALOG, m_CheckAlertDialog);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_DENYTCP, m_CheckDenyTcp);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_DENYUDP, m_CheckDenyUdp);
	DDX_Control(pDX, IDC_SYSTEM_SET_CHECK_DENYICMP, m_CheckDenyIcmp);
}


BEGIN_MESSAGE_MAP(CConfig, CPasseckDialog)
	//{{AFX_MSG_MAP(CConfig)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfig message handlers

void CConfig::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CPasseckDialog::OnPaint() for painting messages
}

BOOL CConfig::OnInitDialog() 
{
	CPasseckDialog::OnInitDialog();
	
	//Init Button
	m_btnApply.SetThemeHelper(&m_ThemeHelper);
	m_btnApply.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnApply.SetTooltipText(_T("Apply the change"));

	m_btnCancel.SetThemeHelper(&m_ThemeHelper);
	m_btnCancel.SetTooltipText(_T("Cancel the change"));

	m_CheckAutoStart.EnableWindow(TRUE);
	m_CheckAlertDialog.EnableWindow(FALSE);
	m_CheckAlertSpeaker.EnableWindow(FALSE);

	    // 初始化config规则
	m_SysConfig.firewall_status=FWS_ALLOWALL;
	m_SysConfig.log_disable	 = 0;
	m_SysConfig.mode = 0;
	m_SysConfig.proto.tcp_disable= 0;
	m_SysConfig.proto.udp_disable= 0;
	m_SysConfig.proto.icmp_disable= 0;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfig::OnApply() 
{
//	if(!m_ButtonApply.IsWindowEnabled())
//		return;

//	pAclHeader->bAutoStart	 = m_CheckAutoStart.GetCheck();
//	pAclHeader->bAudioAlert  = m_CheckAlertSpeaker.GetCheck();
//	pAclHeader->bSplashAlert = m_CheckAlertDialog.GetCheck();

	m_SysConfig.log_disable	 = m_CheckLog.GetCheck();
	m_SysConfig.mode = m_CheckMode.GetCheck();
	m_SysConfig.proto.tcp_disable= m_CheckDenyTcp.GetCheck();
	m_SysConfig.proto.udp_disable= m_CheckDenyUdp.GetCheck();
	m_SysConfig.proto.icmp_disable= m_CheckDenyIcmp.GetCheck();

	CXInstall Install;
	Install.SetAutoStart(m_CheckAutoStart.GetCheck());

	CFirewallDemoDlg* pDlg=(CFirewallDemoDlg*)AfxGetMainWnd();
	pDlg->AddConfig(&m_SysConfig);
}
