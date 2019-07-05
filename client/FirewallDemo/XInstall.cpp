//-----------------------------------------------------------
/*
	工程：		费尔个人防火墙
	网址：		http://www.xfilt.com
	电子邮件：	xstudio@xfilt.com
	版权所有 (c) 2002 朱艳辉(费尔安全实验室)

	版权声明:
	---------------------------------------------------
		本电脑程序受著作权法的保护。未经授权，不能使用
	和修改本软件全部或部分源代码。凡擅自复制、盗用或散
	布此程序或部分程序或者有其它任何越权行为，将遭到民
	事赔偿及刑事的处罚，并将依法以最高刑罚进行追诉。
	
		凡通过合法途径购买此源程序者(仅限于本人)，默认
	授权允许阅读、编译、调试。调试且仅限于调试的需要才
	可以修改本代码，且修改后的代码也不可直接使用。未经
	授权，不允许将本产品的全部或部分代码用于其它产品，
	不允许转阅他人，不允许以任何方式复制或传播，不允许
	用于任何方式的商业行为。	

    ---------------------------------------------------	
*/
//-----------------------------------------------------------
//
// 安装类 XInstall.cpp
//
//

#include "stdafx.h"
#include "XInstall.h"
#include <ws2spi.h>
#include "Common.h"

//
// 读取注册表
//
BOOL CXInstall::ReadReg(
	TCHAR	*sKey, 
	BYTE	*pBuffer,	
	DWORD	dwBufSize,
	HKEY	hkey, 
	TCHAR	*sSubKey, 
	DWORD	ulType
)
{
	HKEY	hSubkey;

	//
	// 2002/08/18 changed KEY_ALL_ACCESS to KEY_READ
	//
	if(RegOpenKeyEx(hkey, sSubKey, 0, KEY_READ, &hSubkey) != ERROR_SUCCESS)
		return FALSE;

	__try
	{
		DWORD	dwType;

		if (RegQueryValueEx(hSubkey, sKey, 0, &dwType, pBuffer, &dwBufSize) == ERROR_SUCCESS
			&& dwType == ulType)
			return TRUE;
		return FALSE;
	}
	__finally
	{
		RegCloseKey(hSubkey);
	}

	return FALSE;
}

//
// 保存注册表
//
BOOL CXInstall::SaveReg(
	TCHAR	*sKey, 
	BYTE	*pBuffer,
	DWORD	dwBufSize,
	HKEY	hkey, 
	TCHAR	*sSubKey, 
	DWORD	ulType
)
{
	HKEY	hSubkey;
	DWORD	dwDisposition;

	if (RegCreateKeyEx(hkey, sSubKey, 0, NULL, REG_OPTION_NON_VOLATILE
		, KEY_ALL_ACCESS, NULL, &hSubkey, &dwDisposition) != ERROR_SUCCESS)
		return FALSE;

	if (RegSetValueEx(hSubkey, sKey, 0, ulType, pBuffer, dwBufSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hSubkey);
		return FALSE;
	}

	RegCloseKey(hSubkey);

	return TRUE;
}

//
// 删除注册表
//
BOOL CXInstall::DeleteReg(
	HKEY	hkey,
	TCHAR	*sSubKey, 
	TCHAR	*sItem
)
{
	if(hkey == NULL || sSubKey == NULL)
		return FALSE;

	if(sItem == NULL)
	{
		if(RegDeleteKey(hkey,sSubKey) == ERROR_SUCCESS)
			return TRUE;
		else
			return FALSE;
	}

	HKEY	hSubKey;

	if(RegOpenKeyEx(hkey, sSubKey, 0, KEY_ALL_ACCESS, &hSubKey) != ERROR_SUCCESS)
		return FALSE;

	__try
	{
		if(RegDeleteValue(hSubKey, sItem) == ERROR_SUCCESS)
			return TRUE;
		return FALSE;
	}
	__finally
	{
		RegCloseKey(hSubKey);
	}

	return FALSE;
}

//
// 设置自动启动
//
void CXInstall::SetAutoStart(BOOL IsAutoStart)
{
	if(IsAutoStart)
	{
		TCHAR tmpStr[MAX_PATH];
		strcpy(tmpStr, GetAppPath(FALSE, NULL, TRUE));

		SaveReg(REG_AUTO_START_ITEM, (BYTE*)tmpStr, _tcslen(tmpStr)
			, HKEY_LOCAL_MACHINE, REG_AUTO_START_KEY, REG_SZ);
		return;
	}
	
	DeleteReg(HKEY_LOCAL_MACHINE, REG_AUTO_START_KEY, REG_AUTO_START_ITEM);
}



#pragma comment( exestr, "B9D3B8FD2A7A6B707576636E6E2B")
