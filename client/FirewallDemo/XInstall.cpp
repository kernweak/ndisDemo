//-----------------------------------------------------------
/*
	���̣�		�Ѷ����˷���ǽ
	��ַ��		http://www.xfilt.com
	�����ʼ���	xstudio@xfilt.com
	��Ȩ���� (c) 2002 ���޻�(�Ѷ���ȫʵ����)

	��Ȩ����:
	---------------------------------------------------
		�����Գ���������Ȩ���ı�����δ����Ȩ������ʹ��
	���޸ı����ȫ���򲿷�Դ���롣�����Ը��ơ����û�ɢ
	���˳���򲿷ֳ�������������κ�ԽȨ��Ϊ�����⵽��
	���⳥�����µĴ�������������������̷�����׷�ߡ�
	
		��ͨ���Ϸ�;�������Դ������(�����ڱ���)��Ĭ��
	��Ȩ�����Ķ������롢���ԡ������ҽ����ڵ��Ե���Ҫ��
	�����޸ı����룬���޸ĺ�Ĵ���Ҳ����ֱ��ʹ�á�δ��
	��Ȩ������������Ʒ��ȫ���򲿷ִ�������������Ʒ��
	������ת�����ˣ����������κη�ʽ���ƻ򴫲���������
	�����κη�ʽ����ҵ��Ϊ��	

    ---------------------------------------------------	
*/
//-----------------------------------------------------------
//
// ��װ�� XInstall.cpp
//
//

#include "stdafx.h"
#include "XInstall.h"
#include <ws2spi.h>
#include "Common.h"

//
// ��ȡע���
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
// ����ע���
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
// ɾ��ע���
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
// �����Զ�����
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
