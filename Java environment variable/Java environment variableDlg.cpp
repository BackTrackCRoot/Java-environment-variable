
// Java environment variableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Java environment variable.h"
#include "Java environment variableDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJavaenvironmentvariableDlg 对话框



CJavaenvironmentvariableDlg::CJavaenvironmentvariableDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJavaenvironmentvariableDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJavaenvironmentvariableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CJavaenvironmentvariableDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CJavaenvironmentvariableDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CJavaenvironmentvariableDlg 消息处理程序

BOOL CJavaenvironmentvariableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_SHOW);

	// TODO:  在此添加额外的初始化代码
	// Get Java environment variable install path
	/*CRegKey key;
	wchar_t JavaHome[40];
	ULONG szJavaHome = 40;
	if (key.Open(HKEY_LOCAL_MACHINE, L"\\SOFTWARE\\JavaSoft\\Java Development Kit\\1.8") == ERROR_SUCCESS)
	{
		//
		;
	}
	else
	{
		auto err = GetLastError()
		;
		MessageBox(L"");
	}
	key.QueryStringValue(L"JavaHome", JavaHome, &szJavaHome);*/
	//MessageBox(L"SOFTWARE\\JavaSoft\\Java Development Kit\\1.8");


	CString jdkpath = GetJavaEnvironment(L"SOFTWARE\\JavaSoft\\Java Development Kit\\1.8", L"JavaHome");
	if (jdkpath != "")
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(jdkpath);
	else
		MessageBox(L"JDK获取环境初始化失败，请手动填写！");
	CString jrepath = GetJavaEnvironment(L"SOFTWARE\\JavaSoft\\Java Runtime Environment\\1.8", L"JavaHome");
	if (jrepath != "")
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(jrepath);
	else
		MessageBox(L"JRE获取环境初始化失败，请手动填写！");

	//GetDlgItem(IDC_BUTTON1)->SetFocus();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJavaenvironmentvariableDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CJavaenvironmentvariableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CJavaenvironmentvariableDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	// config button--
	//JAVA_HOME
	//CLASSPATH:.;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar
	//PARH:%JAVA_HOME%\bin;
	CString envPath;
	CString javahome;

	if (CreateSystemEnvironment(L"CLASSPATH", L".;%JAVA_HOME%\\lib\\dt.jar;%JAVA_HOME%\\lib\\tools.jar") != ERROR_SUCCESS)
		goto JmpError;
	
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(javahome);
	if (javahome != "")
	{
		if (CreateSystemEnvironment(L"JAVA_HOME", javahome) != ERROR_SUCCESS)
			goto JmpError;
	}
	else
		MessageBox(L"请填写环境变量！");
	envPath = GetSystemEnvironment(L"Path");
	if (envPath.IsEmpty())
		goto JmpError;
	envPath += L";%JAVA_HOME%\\bin;";
	if(SetSystemEnvironment(L"Path", envPath))
		goto JmpError;

	//Let all program to refresh own Environment variable
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment", SMTO_ABORTIFHUNG, 3000, NULL);
	MessageBox(L"设置环境变量失败！");
	goto OK;
JmpError:MessageBox(L"设置环境变量失败！");
OK:;
}
CString CJavaenvironmentvariableDlg::GetJavaEnvironment(CString rPath,CString rString)
{
	CString retPath;
	HKEY hKey = NULL;
	DWORD dw = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, rPath, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	if (dw == ERROR_SUCCESS)
	{
		DWORD Type = REG_SZ;
		DWORD sizeJzp;
		//Get Reg key length
		::RegQueryValueEx(hKey, rString, NULL, NULL, NULL, &sizeJzp);
		wchar_t *jzp = (wchar_t *)malloc(sizeJzp + sizeof(wchar_t));
		//Query
		dw = ::RegQueryValueEx(hKey, rString, 0, &Type, (LPBYTE)jzp, &sizeJzp);
		if (dw == ERROR_SUCCESS)
		{
			//Add Reg back
			HANDLE hFile = CreateFile(L"Path.bak", GENERIC_READ || GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD downWriteSize = 0;
			WriteFile(hFile, jzp, sizeJzp, &downWriteSize, NULL);
			FlushFileBuffers(hFile);
			return jzp;
			free(jzp);
		}
		else
		{
			/*LPVOID lpMsgBuf;
			TCHAR szBuf[128];
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
			wsprintf(szBuf, _T("%s 出错信息 (出错码=%d): %s"), _T("RegOpenKeyEx"), dw, lpMsgBuf);
			LocalFree(lpMsgBuf);
			MessageBox(szBuf);*/
			::RegCloseKey(hKey);
			free(jzp);
			return L"";
		}
	}
	else
	{

		/*LPVOID lpMsgBuf;
		TCHAR szBuf[128];
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		wsprintf(szBuf, _T("%s 出错信息 (出错码=%d): %s"), _T("RegOpenKeyEx"), dw, lpMsgBuf);
		LocalFree(lpMsgBuf);
		MessageBox(szBuf);*/
		return L"";
	}
}
CString CJavaenvironmentvariableDlg::GetSystemEnvironment(CString EnvironmentName)
{
	HKEY hKey = NULL;
	DWORD dw = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	if (dw == ERROR_SUCCESS)
	{
		DWORD Type = REG_EXPAND_SZ;
		DWORD sizeJzp;
		::RegQueryValueEx(hKey, EnvironmentName, NULL, NULL, NULL, &sizeJzp);
		wchar_t *jzp = (wchar_t *)malloc(sizeJzp + sizeof(wchar_t));
		dw = ::RegQueryValueEx(hKey, EnvironmentName, 0, &Type, (LPBYTE)jzp, &sizeJzp);
		if (dw == ERROR_SUCCESS)
		{
			return jzp;
			free(jzp);
		}
		else
		{
			::RegCloseKey(hKey);
			free(jzp);
			return L"";
		}
	}
	else
	{
		return L"";
	}	
}

LSTATUS CJavaenvironmentvariableDlg::SetSystemEnvironment(CString EnvironmentName, CString EnvironmentValue)
{
	HKEY hKey = NULL;
	DWORD dw = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, &hKey);
	if (dw == ERROR_SUCCESS)
	{
		DWORD Type = REG_EXPAND_SZ;
		dw = ::RegSetValueEx(hKey, EnvironmentName, 0, Type, (LPBYTE)EnvironmentValue.GetBuffer(), EnvironmentValue.GetLength() * 2);
		if (dw == ERROR_SUCCESS)
		{
			return ERROR_SUCCESS;
		}
		else
			return dw;
	}
	else 
		return dw;
	::RegCloseKey(hKey);
}

LSTATUS CJavaenvironmentvariableDlg::CreateSystemEnvironment(CString EnvironmentName, CString EnvironmentValue)
{
	HKEY hKey = NULL;
	DWORD dw = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager", 0, KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, &hKey);
	if (dw == ERROR_SUCCESS)
	{
		HKEY newhkey;
		dw = ::RegCreateKey(hKey, L"Environment", &newhkey);
		if (dw == ERROR_SUCCESS)
		{
			DWORD Type = REG_EXPAND_SZ;
			dw = ::RegSetValueEx(newhkey, EnvironmentName, 0, Type, (LPBYTE)EnvironmentValue.GetBuffer(), EnvironmentValue.GetLength()*2);
			
			return dw;
		}
		else
			return dw;
		::RegCloseKey(newhkey);
	}
	else
		return dw;
	::RegCloseKey(hKey);
}