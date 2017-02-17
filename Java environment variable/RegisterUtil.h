#pragma once
class CRegisterUtil
{
public:
	CRegisterUtil();
	~CRegisterUtil();
	static CString GetJavaEnvironment(CString rPath, CString rString);//Query Info for java Path
	static CString GetSystemEnvironment(CString EnvironmentName);//Qyery 
	static LSTATUS SetSystemEnvironment(CString EnvironmentName, CString EnvironmentValue);
	static LSTATUS CreateSystemEnvironment(CString EnvironmentName, CString EnvironmentValue);
};

