// Log.cpp : implementation file
//

#include "stdafx.h"
#include "FirewallDemo.h"
#include "Log.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLog dialog


CLog::CLog(CWnd* pParent /*=NULL*/)
	: CPasseckDialog(CLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLog::DoDataExchange(CDataExchange* pDX)
{
	CPasseckDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLog)
		DDX_Control(pDX, IDC_LOG, m_logs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLog, CPasseckDialog)
	//{{AFX_MSG_MAP(CLog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLog message handlers

void CLog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	
	// Do not call CPasseckDialog::OnPaint() for painting messages
}



BOOL CLog::OnInitDialog() 
{
	CPasseckDialog::OnInitDialog();
	
	AddListHead(&m_logs, LOG_HEADER, LOG_HEADER_COUNT, LOG_HEADER_LENTH);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}






void CLog::addlist(Log *pLog)
{
	AddApp(&m_logs, pLog, 1000, FALSE, TRUE);
}
