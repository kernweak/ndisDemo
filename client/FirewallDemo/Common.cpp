/////////////////////////////////////////////////////////////
//
// 界面操作相关函数
//
/////////////////////////////////////////////////////////

#include "stdafx.h"
#include "common.h"

#ifdef GUI_COMMON_FUNCTION

//
// 得到应用程序路径
//
CString GetAppPath(BOOL IsDLL, HINSTANCE instance, BOOL IsFullPathName) 
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];

	if(IsDLL)
		GetModuleFileName(instance, sFilename, _MAX_PATH);
	else
		GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);

	if(IsFullPathName)
		return sFilename;

	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	CString rVal(CString(sDrive) + CString(sDir));
	int nLen = rVal.GetLength();

	if (rVal.GetAt(nLen-1) != _T('\\'))
		rVal += _T("\\");

	return rVal;
}

//
// 将数值IP转化为字符串IP
//
CString WINAPI DIPToSIP(DWORD* pIP)
{
	if(pIP == NULL)
		return _T("");

	CString	s;
	BYTE	*b = (BYTE*)pIP;
	s.Format(_T("%d.%d.%d.%d"),b[3],b[2],b[1],b[0]);

	return s;
}

//
// 为 ComboBox 增加字符串
//
void WINAPI AddComboStrings(CComboBox* pCombo, TCHAR** pString, int nCount)
{
	int nItemCount = pCombo->GetCount();

	for(int i = 0; i < nItemCount; i ++)
	{
		pCombo->DeleteString(0);
	}
	for(i = 0; i < nCount; i++)
	{
		pCombo->AddString(pString[i]);
	}
}

//
// 为 ListCtrl 增加表头
//
void WINAPI AddListHead(CListCtrl* pListCtrl, TCHAR** pString, int nCount, int* ppLenth)
{
//	pListCtrl->SetBkColor(PASSECK_DIALOG_BKCOLOR);
//	pListCtrl->SetTextColor(COLOR_TEXT_NORMAL);

	pListCtrl->SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);

	int nColumnCount = pListCtrl->GetHeaderCtrl()->GetItemCount();

	for (int i = 0; i < nColumnCount; i++)
	{
	   pListCtrl->DeleteColumn(0);
	}
	for(i = 0; i < nCount; i++)
	{
		pListCtrl->InsertColumn(i, pString[i], LVCFMT_LEFT, ppLenth == NULL ? strlen(pString[i])*CHAR_WIDTH : ppLenth[i]);
	}
}


//
// 为 ListCtrl 增加记录
//
int WINAPI AddList(CListCtrl *pList, const TCHAR** pString
	, int nCount, BOOL bIsSelect, BOOL bIsEdit, int iIndex, int iIcon)
{
	if(pString == NULL) return -1;

	if(!bIsEdit)
	{
		iIndex = pList->GetItemCount();
		pList->InsertItem(iIndex, pString[0], iIcon);
	}

	for(int i = 0; i < nCount; i++)
	{
		pList->SetItemText(iIndex, i, pString[i]);
	}
	if(bIsSelect && !bIsEdit)
	{
		pList->EnsureVisible(iIndex, TRUE);
		pList->SetItemState(iIndex,	LVIS_SELECTED,LVIS_SELECTED);
	}

	return iIndex;
}


//
// 为应用程序监视列表增加记录
//
void AddApp(CListCtrl* pList, Log* pLog, int nMaxCount, BOOL IsSeleted, BOOL IsShowDate)
{
	CString sString[LOG_HEADER_COUNT];
	sString[0].Format("%u", pLog->ID);
	sString[1].Format("%s/%s", GUI_SERVICE_TYPE[pLog->protocol], GUI_DIRECTION[pLog->direct]);
	sString[2].Format("%s/%u", DIPToSIP(&pLog->s_ip), pLog->s_port);		
	sString[3].Format("%s/%u", DIPToSIP(&pLog->d_ip), pLog->d_port);
	sString[4].Format("%s", GUI_ACTION[pLog->action]);
	sString[5].Format("%s", pLog->time.Format(IsShowDate ? "%Y-%m-%d %H:%M:%S" : "%H:%M:%S"));
	sString[6]=pLog->sMemo;

	CString str;
	str.Format("direct:%d \naction:%d",pLog->direct,pLog->action);
	OutputDebugString(str);

	#ifdef DEBUG_NEW
		OutputDebugString(sString[0]);
		OutputDebugString(sString[1]);
		OutputDebugString(sString[2]);
		OutputDebugString(sString[3]);
		OutputDebugString(sString[4]);
		OutputDebugString(sString[5]);
		OutputDebugString(sString[6]);
	#endif
	AddLog(pList, (LPCTSTR*)sString, LOG_HEADER_COUNT, nMaxCount, IsSeleted);

}


void AddLog(CListCtrl *pList, LPCTSTR* pString, int nCount, int nMaxCount, BOOL IsSeleted)
{
	if(nMaxCount!= -1 && pList->GetItemCount() > nMaxCount)
		pList->DeleteItem(0);
	AddList(pList, pString, nCount, IsSeleted, FALSE, -1);


}

#endif// GUI_COMMON_FUNCTION