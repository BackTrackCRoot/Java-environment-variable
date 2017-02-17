#include "stdafx.h"
#include "RegisterUtil.h"



LSTATUS CRegisterUtil::CreateSystemEnvironment(CString EnvironmentName, CString EnvironmentValue)
{
	HKEY hKey = NULL,NhKey = NULL;
	DWORD dw;
	//Open register
	dw = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager", 0, KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, &hKey);
	if (dw != ERROR_SUCCESS)
		return dw;
	//Create new key
	dw = ::RegCreateKey(hKey, L"Environment", &NhKey);
	if (dw != ERROR_SUCCESS)
		return dw;
	//Set Key
	dw = ::RegSetValueEx(NhKey, EnvironmentName, 0, REG_EXPAND_SZ, (LPBYTE)EnvironmentValue.GetBuffer(), EnvironmentValue.GetLength() * 2);
	if (dw != ERROR_SUCCESS)
		return dw;
	::RegCloseKey(NhKey);
	::RegCloseKey(hKey);
	return ERROR_SUCCESS;
}


LSTATUS CRegisterUtil::SetSystemEnvironment(CString EnvironmentName, CString EnvironmentValue)
{
	HKEY hKey = NULL;
	DWORD dw;
	dw = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, &hKey);
	if (dw != ERROR_SUCCESS)
		return dw;
	dw = ::RegSetValueEx(hKey, EnvironmentName, 0, REG_EXPAND_SZ, (LPBYTE)EnvironmentValue.GetBuffer(), EnvironmentValue.GetLength() * 2);
	if (dw != ERROR_SUCCESS)
		return dw;
	RegCloseKey(hKey);
	return ERROR_SUCCESS;
}


CString CRegisterUtil::GetSystemEnvironment(CString EnvironmentName)
{
	HKEY hKey = NULL;
	DWORD dw, sizeJzp, Type = REG_EXPAND_SZ;;
	dw = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	if (dw != ERROR_SUCCESS)
		return NULL;
	::RegQueryValueEx(hKey, EnvironmentName, NULL, NULL, NULL, &sizeJzp);
	wchar_t *jzp = (wchar_t *)malloc(sizeJzp + sizeof(wchar_t));
	dw = ::RegQueryValueEx(hKey, EnvironmentName, 0, &Type, (LPBYTE)jzp, &sizeJzp);
	if (dw != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		free(jzp);
		return NULL;
	}
	RegCloseKey(hKey);
	return jzp;
}

CString CRegisterUtil::GetJavaEnvironment(CString rPath, CString rString)
{
	CString retPath;
	HKEY hKey = NULL;
	DWORD dw, sizeJzp, Type = REG_SZ;
	dw = RegOpenKeyExW(HKEY_LOCAL_MACHINE, rPath, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	if (dw != ERROR_SUCCESS)
		return NULL;
	//Get Reg key length
	::RegQueryValueEx(hKey, rString, NULL, NULL, NULL, &sizeJzp);
	wchar_t *jzp = (wchar_t *)malloc(sizeJzp + sizeof(wchar_t));
	//Query
	dw = ::RegQueryValueEx(hKey, rString, 0, &Type, (LPBYTE)jzp, &sizeJzp);
	if (dw != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		free(jzp);
		return NULL;
	}
	else
	{
		//Backup path old date to 'Path.bak'
		HANDLE hFile = CreateFile(L"Path.bak", GENERIC_READ || GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD downWriteSize = 0;
		WriteFile(hFile, jzp, sizeJzp, &downWriteSize, NULL);
		FlushFileBuffers(hFile);
		CloseHandle(hFile);
		RegCloseKey(hKey);
		return jzp;
	}
}