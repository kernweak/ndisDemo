// NetState.cpp : implementation file
//

#include "stdafx.h"
#include "FirewallDemo.h"
#include "NetState.h"
#include "..\common\HistogramCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetState dialog


CNetState::CNetState(CWnd* pParent /*=NULL*/)
	: CPasseckDialog(CNetState::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetState)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNetState::DoDataExchange(CDataExchange* pDX)
{
	CPasseckDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetState)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetState, CPasseckDialog)
	//{{AFX_MSG_MAP(CNetState)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetState message handlers

void CNetState::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	RECT rect;
//	GetClientRect(&rect);
//	CBrush brush(PASSECK_DIALOG_BKCOLOR);
//	dc.FillRect(&rect, &brush);
	
	// Do not call CPasseckDialog::OnPaint() for painting messages
}

BOOL CNetState::OnInitDialog() 
{
	CPasseckDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    //ÏÔÊ¾ÍøÂç×´¿öÇúÏßÍ¼
	CRect rect;
	GetDlgItem(IDC_NETSTATE)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_ctlHistogram.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, rect, this, 1000);
	m_ctlHistogram.SetRange(1, 100);
	m_ctlHistogram.SetSpeed(CHistogramCtrl::NORMAL_SPEED);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
