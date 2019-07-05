/****************************************************************/
/*																*/
/*  DialogResize.cpp											*/
/*																*/
/*  Implementation of the CDialogResize class.					*/
/*	Provides support for resizing dialog controls.				*/
/*																*/
/*  Based on the WTL class CDialogResize from Microsoft			*/
/*  Modified for use with MFC by Pablo van der Meer				*/
/*																*/
/*  http://www.pablovandermeer.nl								*/
/*																*/
/*  Last updated: 01 march 2002									*/
/*																*/
/****************************************************************/


#include "stdafx.h"
#include "DialogResize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDialogResize::CDialogResize()
{
	CommonConstruct();
}


CDialogResize::CDialogResize(UINT uResource, CWnd* pParent)
	: CDialog(uResource, pParent)
{
	CommonConstruct();
}


CDialogResize::CDialogResize(LPCTSTR pszResource, CWnd* pParent)
	: CDialog(pszResource, pParent)
{
	CommonConstruct();
}


void CDialogResize::CommonConstruct()
{
	//{{AFX_DATA_INIT(CDialogResize)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bGripper = FALSE;
	m_sizeDialog.cx = 0;
	m_sizeDialog.cy = 0;
	m_ptMinTrackSize.x = -1;
	m_ptMinTrackSize.y = -1;
}


void CDialogResize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogResize)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogResize, CDialog)
	//{{AFX_MSG_MAP(CDialogResize)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CDialogResize)
END_DLGRESIZE_MAP()

// Operations
void CDialogResize::InitResizing(BOOL bAddGripper, BOOL bUseMinTrackSize, DWORD dwForceStyle)
{
	// Force specified styles (default WS_THICKFRAME | WS_CLIPCHILDREN to enable resizing border and reduce flicker)
	if(dwForceStyle && (GetStyle() & dwForceStyle) != dwForceStyle)
		ModifyStyle(0, dwForceStyle);

	// Cleanup in case of multiple initialization
	// block: first check for the gripper control, destroy it if needed
	{
		CWnd *wndGripper = GetDlgItem(AFX_IDW_STATUS_BAR);
		if (wndGripper != NULL)
		{

		}
	}
	// clear out everything else
	m_arrData.RemoveAll();
	m_sizeDialog.cx = 0;
	m_sizeDialog.cy = 0;
	m_ptMinTrackSize.x = -1;
	m_ptMinTrackSize.y = -1;

	// Get initial dialog client size
	RECT rectDlg;
	GetClientRect(&rectDlg);
	m_sizeDialog.cx = rectDlg.right;
	m_sizeDialog.cy = rectDlg.bottom;

	// Create gripper if requested
	m_bGripper = false;
	if(bAddGripper)
	{
		// shouldn't exist already
		ASSERT(!::IsWindow(::GetDlgItem(m_hWnd, AFX_IDW_STATUS_BAR)));
		if(!::IsWindow(::GetDlgItem(m_hWnd, AFX_IDW_STATUS_BAR)))
		{
			m_wndGripper.Create(_T("SCROLLBAR"), "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SBS_SIZEBOX | SBS_SIZEGRIP | SBS_SIZEBOXBOTTOMRIGHTALIGN, rectDlg, this, AFX_IDW_STATUS_BAR);
			if(IsWindow(m_wndGripper.m_hWnd))
			{
				m_bGripper = true;
				RECT rectCtl;
				m_wndGripper.GetWindowRect(&rectCtl);
				ScreenToClient(&rectCtl);
				AfxResizeData data = {AFX_IDW_STATUS_BAR, DLSZ_MOVE_X | DLSZ_MOVE_Y | DLSZ_REPAINT | _DLSZ_GRIPPER, { rectCtl.left, rectCtl.top, rectCtl.right, rectCtl.bottom } };
				m_arrData.Add(data);
			}
		}
	}

	// Get min track position if requested
	if(bUseMinTrackSize)
	{
		RECT rect;
		GetWindowRect(&rect);
		m_ptMinTrackSize.x = rect.right - rect.left;
		m_ptMinTrackSize.y = rect.bottom - rect.top;
	}

	// Walk the map and initialize data
	const AfxResizeMap* pMap = GetDlgResizeMap();
	ASSERT(pMap != NULL);
	int nGroupStart = -1;
	for(int nCount = 1; !(pMap->m_nCtlID == -1 && pMap->m_dwResizeFlags == 0); nCount++, pMap++)
	{
		if(pMap->m_nCtlID == -1)
		{
			switch(pMap->m_dwResizeFlags)
			{
			case _DLSZ_BEGIN_GROUP:
				ASSERT(nGroupStart == -1);
				nGroupStart = m_arrData.GetSize();
				break;
			case _DLSZ_END_GROUP:
			{
				ASSERT(nGroupStart != -1);
				int nGroupCount = m_arrData.GetSize() - nGroupStart;
				m_arrData[nGroupStart].SetGroupCount(nGroupCount);
				nGroupStart = -1;
			}
			break;

			default:
				ASSERT(FALSE && _T("Invalid DLGRESIZE Map Entry"));
				break;
			}
		}
		else
		{
			// this ID conflicts with the default gripper one
			ASSERT(m_bGripper ? (pMap->m_nCtlID != AFX_IDW_STATUS_BAR) : TRUE);

			CWnd *ctl = GetDlgItem(pMap->m_nCtlID);
			if (ctl)
			{
				RECT rectCtl;
				ctl->GetWindowRect(&rectCtl);
				ScreenToClient(&rectCtl);

				DWORD dwGroupFlag = (nGroupStart != -1 && m_arrData.GetSize() == nGroupStart) ? _DLSZ_BEGIN_GROUP : 0;
				AfxResizeData data = { pMap->m_nCtlID, pMap->m_dwResizeFlags | dwGroupFlag, { rectCtl.left, rectCtl.top, rectCtl.right, rectCtl.bottom } };
				m_arrData.Add(data);
			}
		}
	}
	ASSERT((nGroupStart == -1) && _T("No End Group Entry in the DLGRESIZE Map"));
}


void CDialogResize::UpdateLayout(int cxWidth, int cyHeight)
{
	ASSERT(::IsWindow(m_hWnd));
//	SetRedraw(FALSE);
	RECT rectGroup = { 0, 0, 0, 0 };
	for(int i = 0; i < m_arrData.GetSize(); i++)
	{
		if((m_arrData[i].m_dwResizeFlags & _DLSZ_BEGIN_GROUP) != 0)	// start of a group
		{
			int nGroupCount = m_arrData[i].GetGroupCount();
			ASSERT(nGroupCount > 0 && i + nGroupCount - 1 < m_arrData.GetSize());
			rectGroup = m_arrData[i].m_rect;
			int j;
			for(j = 1; j < nGroupCount; j++)
			{
				rectGroup.left = min(rectGroup.left, m_arrData[i + j].m_rect.left);
				rectGroup.top = min(rectGroup.top, m_arrData[i + j].m_rect.top);
				rectGroup.right = max(rectGroup.right, m_arrData[i + j].m_rect.right);
				rectGroup.bottom = max(rectGroup.bottom, m_arrData[i + j].m_rect.bottom);
			}
			RECT rcThis;
			RECT rcNext;
			for(j = 0; j < nGroupCount; j++)
			{
				int xyStartNext = -1;
				if((j < (nGroupCount - 1)) && ((m_arrData[i + j].m_dwResizeFlags & (DLSZ_SIZE_X | DLSZ_SIZE_Y)) != 0) && ((m_arrData[i + j + 1].m_dwResizeFlags & (DLSZ_SIZE_X | DLSZ_SIZE_Y)) != 0))
				{
					CWnd *ctlThis = GetDlgItem(m_arrData[i + j].m_nCtlID);
					ctlThis->GetWindowRect(&rcThis);
					ScreenToClient(&rcThis);
					CWnd *ctlNext = GetDlgItem(m_arrData[i + j + 1].m_nCtlID);
					ctlNext->GetWindowRect(&rcNext);
					ScreenToClient(&rcNext);
					if((m_arrData[i + j].m_dwResizeFlags & DLSZ_SIZE_X) == DLSZ_SIZE_X && (m_arrData[i + j + 1].m_dwResizeFlags & DLSZ_SIZE_X) == DLSZ_SIZE_X)
					{
						if(rcNext.left >= rcThis.right)
							xyStartNext = m_arrData[i + j + 1].m_rect.left;
					}
					else 
					if((m_arrData[i + j].m_dwResizeFlags & DLSZ_SIZE_Y) == DLSZ_SIZE_Y && (m_arrData[i + j + 1].m_dwResizeFlags & DLSZ_SIZE_Y) == DLSZ_SIZE_Y)
					{
						if(rcNext.top >= rcThis.bottom)
							xyStartNext = m_arrData[i + j + 1].m_rect.top;
					}
				}
				PositionControl(cxWidth, cyHeight, rectGroup, m_arrData[i + j], true, xyStartNext);
			}
			// increment to skip all group controls
			i += nGroupCount - 1;
		}
		else		// one control entry
		{
			PositionControl(cxWidth, cyHeight, rectGroup, m_arrData[i], false);
		}
	}
//	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

void CDialogResize::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_bGripper)
	{
		if(nType == SIZE_MAXIMIZED)
			m_wndGripper.ShowWindow(SW_HIDE);
		else 
		if(nType == SIZE_RESTORED)
			m_wndGripper.ShowWindow(SW_SHOW);
	}
	if(nType != SIZE_MINIMIZED)
	{
		ASSERT(::IsWindow(m_hWnd));
		UpdateLayout(cx, cy);
	}
}

void CDialogResize::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if(m_ptMinTrackSize.x != -1 && m_ptMinTrackSize.y != -1)
	{
		lpMMI->ptMinTrackSize =  m_ptMinTrackSize;
	}
	
	CDialog::OnGetMinMaxInfo(lpMMI);
}



// Implementation
BOOL CDialogResize::PositionControl(int cxWidth, int cyHeight, RECT& rectGroup, AfxResizeData& data, bool bGroup, int xyStartNext)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd *ctl;
	CRect rectCtl;

	ctl = GetDlgItem(data.m_nCtlID);
	
	ctl->GetWindowRect(rectCtl);

	ScreenToClient(rectCtl);

	if(bGroup)
	{
		if((data.m_dwResizeFlags & (DLSZ_SIZE_X | DLSZ_MOVE_X)) != 0)
		{
			rectCtl.left = rectGroup.left + ::MulDiv(data.m_rect.left - rectGroup.left, rectGroup.right - rectGroup.left + (cxWidth - m_sizeDialog.cx), rectGroup.right - rectGroup.left);

			if((data.m_dwResizeFlags & DLSZ_SIZE_X) != 0)
			{
				if(xyStartNext != -1)
					rectCtl.right = rectGroup.left + ::MulDiv(xyStartNext - rectGroup.left, rectGroup.right - rectGroup.left + (cxWidth - m_sizeDialog.cx), rectGroup.right - rectGroup.left) - (xyStartNext - data.m_rect.right);
				else
					rectCtl.right = rectGroup.left + ::MulDiv(data.m_rect.right - rectGroup.left, rectGroup.right - rectGroup.left + (cxWidth - m_sizeDialog.cx), rectGroup.right - rectGroup.left);
			}
			else
			{
				rectCtl.right = rectCtl.left + (data.m_rect.right - data.m_rect.left);
			}
		}

		if((data.m_dwResizeFlags & (DLSZ_SIZE_Y | DLSZ_MOVE_Y)) != 0)
		{
			rectCtl.top = rectGroup.top + ::MulDiv(data.m_rect.top - rectGroup.top, rectGroup.bottom - rectGroup.top + (cyHeight - m_sizeDialog.cy), rectGroup.bottom - rectGroup.top);

			if((data.m_dwResizeFlags & DLSZ_SIZE_Y) != 0)
			{
				if(xyStartNext != -1)
					rectCtl.bottom = rectGroup.top + ::MulDiv(xyStartNext - rectGroup.top, rectGroup.bottom - rectGroup.top + (cyHeight - m_sizeDialog.cy), rectGroup.bottom - rectGroup.top)  - (xyStartNext - data.m_rect.bottom);
				else
					rectCtl.bottom = rectGroup.top + ::MulDiv(data.m_rect.bottom - rectGroup.top, rectGroup.bottom - rectGroup.top + (cyHeight - m_sizeDialog.cy), rectGroup.bottom - rectGroup.top);
			}
			else
			{
				rectCtl.bottom = rectCtl.top + (data.m_rect.bottom - data.m_rect.top);
			}
		}
	}
	else
	{
		if((data.m_dwResizeFlags & (DLSZ_SIZE_X | DLSZ_MOVE_X)) != 0)
		{
			rectCtl.right = data.m_rect.right + (cxWidth - m_sizeDialog.cx);

			if((data.m_dwResizeFlags & DLSZ_MOVE_X) != 0)
				rectCtl.left = rectCtl.right - (data.m_rect.right - data.m_rect.left);
		}

		if((data.m_dwResizeFlags & (DLSZ_SIZE_Y | DLSZ_MOVE_Y)) != 0)
		{
			rectCtl.bottom = data.m_rect.bottom + (cyHeight - m_sizeDialog.cy);

			if((data.m_dwResizeFlags & DLSZ_MOVE_Y) != 0)
				rectCtl.top = rectCtl.bottom - (data.m_rect.bottom - data.m_rect.top);
		}
	}

	if((data.m_dwResizeFlags & DLSZ_REPAINT) != 0)
		ctl->Invalidate();

	if((data.m_dwResizeFlags & (DLSZ_SIZE_X | DLSZ_SIZE_Y | DLSZ_MOVE_X | DLSZ_MOVE_Y | DLSZ_REPAINT)) != 0)
		ctl->SetWindowPos(NULL, rectCtl.left, rectCtl.top, rectCtl.right - rectCtl.left, rectCtl.bottom - rectCtl.top, SWP_NOZORDER | SWP_NOACTIVATE);

	return TRUE;
}
