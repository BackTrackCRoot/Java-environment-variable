
// Java environment variableDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Java environment variable.h"
#include "Java environment variableDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJavaenvironmentvariableDlg �Ի���



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


// CJavaenvironmentvariableDlg ��Ϣ�������

BOOL CJavaenvironmentvariableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_SHOW);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
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
		MessageBox(L"JDK��ȡ������ʼ��ʧ�ܣ����ֶ���д��");
	CString jrepath = GetJavaEnvironment(L"SOFTWARE\\JavaSoft\\Java Runtime Environment\\1.8", L"JavaHome");
	if (jrepath != "")
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(jrepath);
	else
		MessageBox(L"JRE��ȡ������ʼ��ʧ�ܣ����ֶ���д��");

	//GetDlgItem(IDC_BUTTON1)->SetFocus();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CJavaenvironmentvariableDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CJavaenvironmentvariableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CJavaenvironmentvariableDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(L"����д����������");
	envPath = GetSystemEnvironment(L"Path");
	if (envPath.IsEmpty())
		goto JmpError;
	envPath += L";%JAVA_HOME%\\bin;";
	if(SetSystemEnvironment(L"Path", envPath))
		goto JmpError;

	//Let all program to refresh own Environment variable
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment", SMTO_ABORTIFHUNG, 3000, NULL);
	MessageBox(L"���û�������ʧ�ܣ�");
	goto OK;
JmpError:MessageBox(L"���û�������ʧ�ܣ�");
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
			wsprintf(szBuf, _T("%s ������Ϣ (������=%d): %s"), _T("RegOpenKeyEx"), dw, lpMsgBuf);
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
		wsprintf(szBuf, _T("%s ������Ϣ (������=%d): %s"), _T("RegOpenKeyEx"), dw, lpMsgBuf);
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