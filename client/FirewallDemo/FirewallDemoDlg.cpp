// FirewallDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FirewallDemo.h"
#include "FirewallDemoDlg.h"
#include "DDKLBLInc.h"
#include "Splash.h"
#include "condef.h"
#include "common.h"
//#include "Config.h"
//#include <winsock2.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirewallDemoDlg dialog

CFirewallDemoDlg::CFirewallDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFirewallDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFirewallDemoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
/*
	m_OldStat.ip.tcp.allow_in_bytes=0;
	m_OldStat.ip.tcp.allow_in_pkts=0;
	m_OldStat.ip.udp.allow_in_bytes=0;
	m_OldStat.ip.udp.allow_in_pkts=0;
	m_OldStat.ip.icmp.allow_in_bytes=0;
	m_OldStat.ip.icmp.allow_in_pkts=0;
	m_OldStat.notip.other_stat.allow_in_bytes=0;
	m_OldStat.notip.other_stat.allow_in_pkts=0;
*/
}

void CFirewallDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFirewallDemoDlg)
	DDX_Control(pDX, IDC_URL, m_ctlUrl);
	DDX_Control(pDX, IDC_SHOWHELP, m_btnHelp);
	DDX_Control(pDX, IDC_HIDE, m_btnHide);
	DDX_Control(pDX, IDC_CONFIG, m_btnConfig);
	DDX_Control(pDX, IDC_LOG, m_btnLog);
	DDX_Control(pDX, IDC_NETSTATE, m_btnNetstate);
	DDX_Control(pDX, IDC_MONITOR, m_btnMonitor); 
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_RED, m_btnRed);
	DDX_Control(pDX, IDC_YELLOW, m_btnYellow);
	DDX_Control(pDX, IDC_GREEN, m_btnGreen);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFirewallDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CFirewallDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_NETSTATE, OnNetstate)
	ON_BN_CLICKED(IDC_MONITOR, OnMonitor)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	ON_BN_CLICKED(IDC_LOG, OnLog)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_BN_CLICKED(IDC_SHOWHELP, OnShowhelp)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_GREEN, OnPassAll)
	ON_BN_CLICKED(IDC_YELLOW, OnFilter)
	ON_BN_CLICKED(IDC_RED, OnDenyAll)
	ON_COMMAND(ID_EXIT, OnOK)
	ON_COMMAND(ID_MAINFRAME, OnControlFrame)
	ON_COMMAND(ID_DENYALL, OnDenyAll)
	ON_COMMAND(ID_FILTER, OnFilter)
	ON_COMMAND(ID_PASSALL, OnPassAll)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFirewallDemoDlg message handlers

BOOL CFirewallDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//init
 	CSplashWnd::ShowSplashScreen(this);
	SetCaption();

	CString sUrl;
	sUrl.LoadString(IDS_COMPANY_URL);
	m_ctlUrl.SetHyperlink(sUrl);

	CFirewallDemoApp*	pApp = NULL;
	pApp = (CFirewallDemoApp*)AfxGetApp();

	InitButton(pApp);
// Create Tray
	CString sTitle;
	sTitle.LoadString(IDS_APP_NAME);
	m_Tray.Create(NULL,WM_USER+201,sTitle,m_hIcon,IDR_MAINFRAME);

//	if(m_Config.m_bAutoStart)
//		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);

	m_SubParent.SubclassDlgItem(IDC_FRAME, this);
	m_NetState.Create(IDD_NETSTATE, &m_SubParent);
	m_Monitor.Create(IDD_MONITOR, &m_SubParent);
	m_Config.Create(IDD_CONFIG, &m_SubParent);
	m_Log.Create(IDD_LOG, &m_SubParent);
	m_NetState.ShowWindow(SW_SHOW);

	m_btnGreen.SetCheck(TRUE);
    OnPassAll();

	//初始化监视进程和驱动！
	if(InitDialog())
	{
		memset(&m_OldStat, 0, sizeof(Stat));
		SetTimer(1, 1000, NULL);

		return TRUE;  // return TRUE  unless you set the focus to a control
	}
	else
		return FALSE;
}

void CFirewallDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFirewallDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
/*      //改变对话框背景颜色
		CPaintDC dc(this);
		RECT rect;
		GetClientRect(&rect);
		CBrush brush(RGB(253, 253, 253));
		dc.FillRect(&rect, &brush);
*/

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFirewallDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CFirewallDemoDlg::SetCaption()
{
	CString sCaption,sVer;
	sCaption.LoadString(IDS_APP_NAME);
	sVer.Format(_T("V%2.2f"),((float)APP_VERSION)/100);
	SetWindowText(sCaption+sVer);
	return;
}

void CFirewallDemoDlg::InitButton(CWinApp *pApp)
{
	m_btnHelp.SetThemeHelper(&m_ThemeHelper);
	m_btnHelp.SetIcon(pApp->LoadIcon(IDI_HELP));
	m_btnHelp.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnHelp.DrawAsToolbar(TRUE);
	m_btnHelp.SetTooltipText(_T("帮助文档"));

	m_btnLog.SetThemeHelper(&m_ThemeHelper);
	m_btnLog.SetIcon(pApp->LoadIcon(IDI_LOG));
	m_btnLog.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnLog.DrawAsToolbar(TRUE);
	m_btnLog.SetTooltipText(_T("日志"));

	m_btnHide.SetThemeHelper(&m_ThemeHelper);
	m_btnHide.SetIcon(pApp->LoadIcon(IDI_HIDE));
	m_btnHide.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnHide.DrawAsToolbar(TRUE);
	m_btnHide.SetTooltipText(_T("隐藏主程序"));

	m_btnConfig.SetThemeHelper(&m_ThemeHelper);
	m_btnConfig.SetIcon(pApp->LoadIcon(IDR_CONFIG));
	m_btnConfig.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnConfig.DrawAsToolbar(TRUE);
	m_btnConfig.SetTooltipText(_T("系统配置"));

	m_btnMonitor.SetThemeHelper(&m_ThemeHelper);
	m_btnMonitor.SetIcon(pApp->LoadIcon(IDR_MONITOR));
	m_btnMonitor.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnMonitor.DrawAsToolbar(TRUE);
	m_btnMonitor.SetTooltipText(_T("监控面板"));

	m_btnOk.SetThemeHelper(&m_ThemeHelper);
	m_btnOk.SetIcon(pApp->LoadIcon(IDR_EXIT));
	m_btnOk.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnOk.DrawAsToolbar(TRUE);
	m_btnOk.SetTooltipText(_T("退出"));

	m_btnNetstate.SetThemeHelper(&m_ThemeHelper);
	m_btnNetstate.SetIcon(pApp->LoadIcon(IDR_NETSTATE));
	m_btnNetstate.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnNetstate.DrawAsToolbar(TRUE);
	m_btnNetstate.SetTooltipText(_T("网络状态"));

	m_btnRed.SetThemeHelper(&m_ThemeHelper);
	m_btnRed.SetBitmaps(IDB_RED_FOCUS, RGB(255, 0, 255),IDB_RED_NORMAL,RGB(255, 0, 255));;
	m_btnRed.DrawAsToolbar(TRUE);
	m_btnRed.SetCheckBox(TRUE);
	m_btnRed.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 60);
	m_btnRed.SetTooltipText(_T("拦截所有数据包"));


	m_btnGreen.SetThemeHelper(&m_ThemeHelper);
	m_btnGreen.SetBitmaps(IDB_GREEN_SELECT, RGB(255, 0, 255),IDB_GREEN_NORMAL,RGB(255, 0, 255));;
	m_btnGreen.DrawAsToolbar(TRUE);
	m_btnGreen.SetCheckBox(TRUE);
	m_btnGreen.SetTooltipText(_T("放行所有数据包"));

	m_btnYellow.SetThemeHelper(&m_ThemeHelper);
	m_btnYellow.SetBitmaps(IDB_YELLOW_SELECT, RGB(255, 0, 255),IDB_YELLOW_NORMAL,RGB(255, 0, 255));;
	m_btnYellow.DrawAsToolbar(TRUE);
	m_btnYellow.SetCheckBox(TRUE);
	m_btnYellow.SetTooltipText(_T("过滤数据包"));
}

void CFirewallDemoDlg::OnNetstate() 
{
	// TODO: Add your control notification handler code here
	m_NetState.ShowWindow(SW_SHOW);
	m_Config.ShowWindow(SW_HIDE);
	m_Monitor.ShowWindow(SW_HIDE);
	m_Log.ShowWindow(SW_HIDE);
}

void CFirewallDemoDlg::OnMonitor() 
{
	// TODO: Add your control notification handler code here
	m_Monitor.ShowWindow(SW_SHOW);
	m_Config.ShowWindow(SW_HIDE);
	m_NetState.ShowWindow(SW_HIDE);
	m_Log.ShowWindow(SW_HIDE);
}

void CFirewallDemoDlg::OnConfig() 
{
	m_Config.ShowWindow(SW_SHOW);
	m_NetState.ShowWindow(SW_HIDE);
	m_Monitor.ShowWindow(SW_HIDE);
	m_Log.ShowWindow(SW_HIDE);
}

void CFirewallDemoDlg::OnLog() 
{
	m_Log.ShowWindow(SW_SHOW);
	m_Config.ShowWindow(SW_HIDE);
	m_NetState.ShowWindow(SW_HIDE);
	m_Monitor.ShowWindow(SW_HIDE);	
	
}

void CFirewallDemoDlg::OnHide() 
{
	// TODO: Add your control notification handler code here
//	PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);

	ShowWindow(SW_HIDE);
	
}

void CFirewallDemoDlg::OnOK() 
{
	DWORD dwReturn;
	// TODO: Add extra validation here
	if(int Y = AfxMessageBox("确认退出吗?退出计算机将失去保护!",MB_YESNO)==IDYES)
	{
		// 关闭设备句柄，清除事件对象。
		if(m_hDevice)
		{
			if(m_hEvent)
			{
				DeviceIoControl(m_hDevice, 
							IO_STOP_MONITOR_EVENT, 
							NULL,
							0, 
							NULL, 
							0, 
							&dwReturn, 
							NULL);
				CloseHandle(m_hEvent);
			}
			CloseHandle(m_hDevice);
		}
		CDialog::OnOK();
	}
}

void CFirewallDemoDlg::OnShowhelp() 
{
	CString		m_sAppPath,m_sAppName;
	int n = 0;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, _MAX_PATH);//AfxGetInstanceHandle()
	m_sAppPath = szPath;
	n = m_sAppPath.ReverseFind(_T('\\'));
	if(n>0)
	{
		m_sAppName = m_sAppPath.Right(m_sAppPath.GetLength()-n-1);
		m_sAppName = m_sAppName.Left(m_sAppName.GetLength()-4);
		m_sAppPath = m_sAppPath.Left(n+1);
	}

	CString sHelp = m_sAppPath+_T("help\\");
	sHelp += _T("help.mht");
	if((access(sHelp, 0) == -1))
		AfxMessageBox(IDS_ERROR_OPEN_HELP);
	else
		ShellExecute(NULL,_T("open"),sHelp,NULL,NULL,SW_MAXIMIZE);
	
}

int CFirewallDemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return 0;
}

void CFirewallDemoDlg::SetSelectButton(BYTE bSelectButton)
{
/*	if(m_bSelectedButton == bSelectButton) return;
	if(m_bSelectedButton != 255)
	{
		m_Button[m_bSelectedButton].SetSelect(FALSE);
	}
	m_Button[bSelectButton].SetSelect(TRUE);

	m_bSelectedButton = bSelectButton;

	m_ParameterSub.ShowWindow(bSelectButton == BUTTON_PARAMETER ? SW_SHOW : SW_HIDE);
	m_MonitorSub.ShowWindow(bSelectButton == BUTTON_MONITOR ? SW_SHOW : SW_HIDE);
	m_LogSub.ShowWindow(bSelectButton == BUTTON_LOG ? SW_SHOW : SW_HIDE);
	m_AclSub.ShowWindow(bSelectButton == BUTTON_ACL ? SW_SHOW : SW_HIDE);
	m_SystemSet.ShowWindow(bSelectButton == BUTTON_SYSTEMSET ? SW_SHOW : SW_HIDE);
	m_OnLine.ShowWindow(bSelectButton == BUTTON_ONLINE ? SW_SHOW : SW_HIDE);
	m_About.ShowWindow(bSelectButton == BUTTON_ABOUT ? SW_SHOW : SW_HIDE);
*/
}

void CFirewallDemoDlg::OnControlFrame()
{
	ShowWindow(TRUE);
}

LRESULT CFirewallDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
/*
	switch(message){

	case WM_SYSCOMMAND: 
		//如果是系统消息
		if(wParam==SC_MINIMIZE){ 
		//接收到最小化消息时主窗口隐藏
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			return 0;
		}
		break;
	}
*/	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CFirewallDemoDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CFirewallDemoDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}



void CFirewallDemoDlg::OnDenyAll() 
{
	m_btnYellow.SetCheck(FALSE);
	m_btnGreen.SetCheck(FALSE);
	if(m_btnRed.GetCheck()==FALSE)
		m_btnRed.SetCheck(TRUE);

	m_bWorkMode=FWS_DENYALL;
	SetTrayIcon(m_bWorkMode);
	OutputDebugString("FW_Mode:DENY_ALL");
	DWORD dwReturn;

	if(m_hDevice!=0||m_hEvent!=0)
	{
		pConfig m_pConfig = m_Config.GetSysConfig();
		m_pConfig->firewall_status = FWS_DENYALL;
		DeviceIoControl(m_hDevice, 
						IO_DENY_ALL, 
						NULL,
						0, 
						NULL, 
						0, 
						&dwReturn, 
						NULL);
	}
}

void CFirewallDemoDlg::OnFilter() 
{
	m_btnRed.SetCheck(FALSE);
	m_btnGreen.SetCheck(FALSE);
	if(m_btnYellow.GetCheck()==FALSE)
		m_btnYellow.SetCheck(TRUE);

	m_bWorkMode=FWS_NORMAL;
	SetTrayIcon(m_bWorkMode);
	OutputDebugString("FW_Mode:FWS_NORMAL");
	if(m_hDevice!=0||m_hEvent!=0)
	{
		pConfig m_pConfig = m_Config.GetSysConfig();
		m_pConfig->firewall_status = FWS_NORMAL;
		AddConfig(m_pConfig);
	}
}

void CFirewallDemoDlg::OnPassAll() 
{
	m_btnRed.SetCheck(FALSE);
	m_btnYellow.SetCheck(FALSE);
	if(m_btnGreen.GetCheck()==FALSE)
		m_btnGreen.SetCheck(TRUE);

	m_bWorkMode=FWS_ALLOWALL;
	SetTrayIcon(m_bWorkMode);
	OutputDebugString("FW_Mode:FWS_ALLOWALL");
	DWORD dwReturn;

	if(m_hDevice!=0||m_hEvent!=0)
	{
		pConfig m_pConfig = m_Config.GetSysConfig();
		m_pConfig->firewall_status = FWS_ALLOWALL;
		DeviceIoControl(m_hDevice, 
						IO_PASS_ALL, 
						NULL,
						0, 
						NULL, 
						0, 
						&dwReturn, 
						NULL);
	}
}

void CFirewallDemoDlg::InstallRules(RuleLog *pRuleLog)
{
	DWORD dwReturn;

#ifdef _DEBUG
	CString str;
	str.Format("AddOneRule ID:%d \nProtocal:%x \nDirect:%x \nAction:%x \n",pRuleLog->id,pRuleLog->protocol
				,pRuleLog->direct,pRuleLog->action);
	OutputDebugString(str);
#endif

	// 把RuleLog传递给驱动程序
	DeviceIoControl(m_hDevice, 
					IO_ADD_RULE, 
					pRuleLog,
					sizeof(struct _RuleLog), 
					NULL, 
					0, 
					&dwReturn, 
					NULL);

}


void CFirewallDemoDlg::DelOneRule(RuleLog *pRuleLog)
{
	DWORD dwReturn;

#ifdef _DEBUG
	CString str;
	str.Format("DelOneRule ID:%d \nProtocal:%x \nDirect:%x \nAction:%x \n",pRuleLog->id,pRuleLog->protocol
				,pRuleLog->direct,pRuleLog->action);
	OutputDebugString(str);
#endif

	// 把RuleLog传递给驱动程序
	DeviceIoControl(m_hDevice, 
					IO_DEL_RULE, 
					pRuleLog,
					sizeof(struct _RuleLog), 
					NULL, 
					0, 
					&dwReturn, 
					NULL);

}


/////////////////////////////add by xiaoqiang mar 20 2006//////////////////////
unsigned long __stdcall CFirewallDemoDlg::MyThreadEntry(LPVOID lpParam)
{
	DWORD dwReturn;
	OutputDebugString("==>MyThreadEntry(...)\n");

	CFirewallDemoDlg* pThis = (CFirewallDemoDlg*)lpParam;
	
	while(pThis->m_Config.GetSysConfig()->firewall_status=FWS_NORMAL)
	{
		if(pThis->m_hEvent)
		{
			
			WaitForSingleObject(pThis->m_hEvent, INFINITE);
			// 在列表控件中显示
			struct Log *pLog;
			pLog = (Log *)pThis->psharememory;
			pThis->m_Log.addlist(pLog);


			// 设置事件对象为非信号态
			DeviceIoControl(pThis->m_hDevice, 
					IO_CLEAR_EVENT, 
					NULL,
					0, 
					NULL, 
					0, 
					&dwReturn, 
					NULL);
		}
	}
	OutputDebugString("<==MyThreadEntry(...)");

	return true;
}

void CFirewallDemoDlg::CreateMyThread() 
{
	// TODO: Add your control notification handler code here
	DWORD dwThreadID;

	HANDLE hThread = CreateThread(NULL, 
			0, 
			MyThreadEntry, 
			this, 
			0, 
			&dwThreadID);

	CloseHandle(hThread);
}

void WINAPI CFirewallDemoDlg::MyGetLastError(void)
{
	LPVOID lpMsgBuf = NULL; 
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, 
		NULL, 
		GetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language 
		(LPTSTR) &lpMsgBuf, 
		0, 
		NULL 
		); 
		
	// Display the string. 
	MessageBox((LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION ); 
		
	// Free the buffer. 
	LocalFree(lpMsgBuf);
}

/////////////////////////////////////////////////////////////////////////////


//DEL void CFirewallDemoDlg::addlist(Log *pLog)
//DEL {
//DEL 	AddApp(&(m_Log.m_logs), pLog, 1000, FALSE, FALSE);
//DEL }



void CFirewallDemoDlg::AddConfig(Config* pConfig)
{

	DWORD dwReturn;
	OutputDebugString("AddConfig\n");
		// 把Config通过IRP送到底层
	DeviceIoControl(m_hDevice, 
					IO_ADD_CONFIG, 
					pConfig,
					sizeof(Config), 
					NULL, 
					0, 
					&dwReturn, 
					NULL);
}

BOOL CFirewallDemoDlg::InitDialog()
{
	TCHAR szOutputBuffer[20]; 

	m_hDevice = CreateFile("\\\\.\\MyPassthru", 
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	
	if(m_hDevice == INVALID_HANDLE_VALUE)
	{
		MyGetLastError();
		return FALSE;
	}

	DWORD dwReturn;

	// 创建 Event 对象，初始化状态为FALSE。
	m_hEvent = CreateEvent(NULL, false, false, NULL);
	
	// 把 Event 对象传递给驱动程序
	DeviceIoControl(m_hDevice, 
					IO_REFERENCE_EVENT, 
					(LPVOID)m_hEvent,
					0, 
					NULL, 
					0, 
					&dwReturn, 
					NULL);


	// 取得共享内存的地址
	if(!DeviceIoControl(m_hDevice, 
						IO_GET_SHAREMEMORY_ADDR, 
						NULL, 
						NULL, 
						szOutputBuffer, 
						sizeof(szOutputBuffer), 
						&dwReturn, 
						NULL 
						))
		MyGetLastError();
	psharememory = *((PVOID *)szOutputBuffer);

	// 启动监视线程
	CreateMyThread();
	//定时器是为了返回firewall statistics数据


	return TRUE;
}

void CFirewallDemoDlg::SetTrayIcon(int iWorkMode)
{
	if(iWorkMode == 255)
		iWorkMode = m_bWorkMode;

	if(iWorkMode == FWS_ALLOWALL)
		m_Tray.SetIcon(IDI_PASS_ALL);
	else if(iWorkMode == FWS_NORMAL)
		m_Tray.SetIcon(IDI_FILTER);
	else if(iWorkMode == FWS_DENYALL)
		m_Tray.SetIcon(IDI_DENY_ALL);
	else
		m_Tray.SetIcon(IDI_PASS_ALL);
}



void CFirewallDemoDlg::OnTimer(UINT nIDEvent) 
{

//	TCHAR szOutputBuffer[20];
	DWORD dwReturn;
	// 取得驱动Firewall Statistics变量的地址
	if(!DeviceIoControl(m_hDevice, 
						IO_GET_Stat, 
						NULL, 
						NULL, 
						&m_Stat, 
						sizeof(Stat), 
						&dwReturn, 
						NULL 
						))
		MyGetLastError();

	DWORD n = m_Stat.ip.tcp.allow_in_bytes-m_OldStat.ip.tcp.allow_in_bytes+
//		m_Stat.ip.tcp.allow_in_pkts-m_OldStat.ip.tcp.allow_in_pkts+
		m_Stat.ip.udp.allow_in_bytes-m_OldStat.ip.udp.allow_in_bytes+
//		m_Stat.ip.udp.allow_in_pkts-m_OldStat.ip.udp.allow_in_pkts+
		m_Stat.ip.tcp.allow_out_bytes-m_OldStat.ip.tcp.allow_out_bytes+
//		m_Stat.ip.tcp.allow_out_pkts-m_OldStat.ip.tcp.allow_out_pkts+
		m_Stat.ip.udp.allow_out_bytes-m_OldStat.ip.udp.allow_out_bytes+
//		m_Stat.ip.udp.allow_out_pkts-m_OldStat.ip.udp.allow_out_pkts+
		m_Stat.ip.tcp.deny_in_bytes-m_OldStat.ip.tcp.deny_in_bytes+
//		m_Stat.ip.tcp.deny_in_pkts-m_OldStat.ip.tcp.deny_in_pkts+
		m_Stat.ip.udp.deny_in_bytes-m_OldStat.ip.udp.deny_in_bytes+
//		m_Stat.ip.udp.deny_in_pkts-m_OldStat.ip.udp.deny_in_pkts+
		m_Stat.ip.tcp.deny_out_bytes-m_OldStat.ip.tcp.deny_out_bytes+
//		m_Stat.ip.tcp.deny_out_pkts-m_OldStat.ip.tcp.deny_out_pkts+
		m_Stat.ip.udp.deny_out_bytes-m_OldStat.ip.udp.deny_out_bytes;
//		m_Stat.ip.udp.deny_out_pkts-m_OldStat.ip.udp.deny_out_pkts;

	n /= 0x400;		// 1k
	if(n <2)	n = 2;
	if(n> 100)	n = 100;

	m_NetState.GetCtlHistogram()->SetPos(n);
		
#ifdef _DEBUG
	CString str;
	str.Format("Firewall Statistics in one Second\nInAllBytes:%d \ntcpAllowInBytes:%d \ntcpAllowInPacketNUM:%d \nudpAllowInBytes:%d \nudpAllowInPacketNUM:%d \ntcpAllowOutBytes:%d \ntcpAllowOutPacketNUM:%d \nudpAllowOutBytes:%d \nudpAllowOutPacketNUM:%d \ntcpDenyInBytes:%d \ntcpDenyInPacketNUM:%d \nudpDenyInBytes:%d \nudpDenyInPacketNUM:%d \ntcpDenyOutBytes:%d \ntcpDenyOutPacketNUM:%d \nudpDenyOutBytes:%d \nudpDenyOutPacketNUM:%d \n",
		n,
		m_Stat.ip.tcp.allow_in_bytes-m_OldStat.ip.tcp.allow_in_bytes,
		m_Stat.ip.tcp.allow_in_pkts-m_OldStat.ip.tcp.allow_in_pkts,
		m_Stat.ip.udp.allow_in_bytes-m_OldStat.ip.udp.allow_in_bytes,
		m_Stat.ip.udp.allow_in_pkts-m_OldStat.ip.udp.allow_in_pkts,
		m_Stat.ip.tcp.allow_out_bytes-m_OldStat.ip.tcp.allow_out_bytes,
		m_Stat.ip.tcp.allow_out_pkts-m_OldStat.ip.tcp.allow_out_pkts,
		m_Stat.ip.udp.allow_out_bytes-m_OldStat.ip.udp.allow_out_bytes,
		m_Stat.ip.udp.allow_out_pkts-m_OldStat.ip.udp.allow_out_pkts,
		m_Stat.ip.tcp.deny_in_bytes-m_OldStat.ip.tcp.deny_in_bytes,
		m_Stat.ip.tcp.deny_in_pkts-m_OldStat.ip.tcp.deny_in_pkts,
		m_Stat.ip.udp.deny_in_bytes-m_OldStat.ip.udp.deny_in_bytes,
		m_Stat.ip.udp.deny_in_pkts-m_OldStat.ip.udp.deny_in_pkts,
		m_Stat.ip.tcp.deny_out_bytes-m_OldStat.ip.tcp.deny_out_bytes,
		m_Stat.ip.tcp.deny_out_pkts-m_OldStat.ip.tcp.deny_out_pkts,
		m_Stat.ip.udp.deny_out_bytes-m_OldStat.ip.udp.deny_out_bytes,
		m_Stat.ip.udp.deny_out_pkts-m_OldStat.ip.udp.deny_out_pkts);
	OutputDebugString(str);		
#endif
	m_OldStat=m_Stat;
	
	CDialog::OnTimer(nIDEvent);
}
