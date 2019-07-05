// FirewallDemoDlg.h : header file
//

#if !defined(AFX_FIREWALLDEMODLG_H__72AF6402_9F78_4F81_9214_7B46FEA29CDB__INCLUDED_)
#define AFX_FIREWALLDEMODLG_H__72AF6402_9F78_4F81_9214_7B46FEA29CDB__INCLUDED_

#include "..\common\ColorStatic.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winioctl.h>
#include "..\common\systemtray.h"
#include "..\common\XPStyleButtonST.h"
#include "..\common\ThemeHelperST.h"
#include "..\common\hyperlinkstatic.h"
#include "NetState.h"	// Added by ClassView
#include "Monitor.h"
#include "Config.h"
#include "Log.h"


/////////////////////////////////////////////////////////////////////////////
// CFirewallDemoDlg dialog

class CFirewallDemoDlg : public CDialog
{
// Construction
public:
	void DelOneRule(RuleLog *pRuleLog);
	void SetTrayIcon(int iWorkMode = 255);
	void InstallRules(RuleLog *pRuleLog);
	CFirewallDemoDlg(CWnd* pParent = NULL);	// standard constructor
	void SetSplash(BOOL bIsSplash){m_IsSplash = bIsSplash;}
	void WINAPI MyGetLastError(void);
	void AddConfig(Config* pConfig);

// Dialog Data
	//{{AFX_DATA(CFirewallDemoDlg)
	enum { IDD = IDD_FIREWALLDEMO_DIALOG };
	CHyperlinkStatic	m_ctlUrl;
//	CHistogramCtrl		m_ctlHistogram;
	CXPStyleButtonST	m_btnNetstate;
	CXPStyleButtonST	m_btnConfig;
	CXPStyleButtonST	m_btnHide;
	CXPStyleButtonST	m_btnMonitor;
	CXPStyleButtonST	m_btnOk;
	CXPStyleButtonST	m_btnLog;
	CXPStyleButtonST	m_btnHelp;
	CXPStyleButtonST	m_btnRed;
	CXPStyleButtonST    m_btnYellow;
	CXPStyleButtonST    m_btnGreen;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFirewallDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnCancel();
    void OnControlFrame();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CSystemTray	        m_Tray;
	CThemeHelperST		m_ThemeHelper;
	HICON               m_hIcon;
	
	HANDLE	m_hDevice;		// 设备句柄
	HANDLE	m_hEvent;		// 与驱动通信的事件对象
	PVOID	psharememory;	// 共享内存地址



	void SetCaption();
	void InitButton(CWinApp* pApp);
	static unsigned long __stdcall MyThreadEntry(LPVOID lpParam);	// 监视线程入口
	void CreateMyThread();

	// Generated message map functions
	//{{AFX_MSG(CFirewallDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNetstate();
	afx_msg void OnMonitor();
	afx_msg void OnConfig();
	afx_msg void OnLog();
	afx_msg void OnHide();
	virtual void OnOK();
	afx_msg void OnShowhelp();
	afx_msg void OnClose();
	afx_msg void OnPassAll();
	afx_msg void OnFilter();
	afx_msg void OnDenyAll();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BYTE m_bWorkMode;
	BOOL InitDialog();
	CColorStatic m_SubParent;
	void SetSelectButton(BYTE bSelectButton);
	BOOL   m_IsSplash;
	CNetState m_NetState;
	CMonitor  m_Monitor;
	CLog      m_Log;
	CConfig  m_Config;
	Stat m_Stat,m_OldStat;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALLDEMODLG_H__72AF6402_9F78_4F81_9214_7B46FEA29CDB__INCLUDED_)
