
// Java environment variableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Java environment variable.h"
#include "Java environment variableDlg.h"
#include "afxdialogex.h"
#include "RegisterUtil.h"

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

	CString jdkpath = CRegisterUtil::GetJavaEnvironment(L"SOFTWARE\\JavaSoft\\Java Development Kit\\1.8", L"JavaHome");
	if (jdkpath != "")
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(jdkpath);
	else
		MessageBox(L"JDK获取环境初始化失败，请手动填写！");
	CString jrepath = CRegisterUtil::GetJavaEnvironment(L"SOFTWARE\\JavaSoft\\Java Runtime Environment\\1.8", L"JavaHome");
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
	//Path:%JAVA_HOME%\bin;


	boolean isInstall = false;//Check environment instll status
	CString javahome,path;
	int updateError = 0;
	isInstall = !CRegisterUtil::GetSystemEnvironment(L"CLASSPATH").IsEmpty() || isInstall;//check CLASSPATH exeist
	isInstall = !CRegisterUtil::GetSystemEnvironment(L"JAVA_HOME").IsEmpty() || isInstall;//check JAVA_HOME exeist
	if (isInstall)
	{
		if (!MessageBox(L"Your environment was exists, Do you want overwrite it? ", L"Warrning", MB_YESNO))
			return;
	}
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(javahome);
	if (!javahome.IsEmpty())
	{
		if (CRegisterUtil::CreateSystemEnvironment(L"JAVA_HOME", javahome) == ERROR_SUCCESS)
		{
			if (CRegisterUtil::CreateSystemEnvironment(L"CLASSPATH", L".;%JAVA_HOME%\\lib\\dt.jar;%JAVA_HOME%\\lib\\tools.jar") == ERROR_SUCCESS)
			{
				if (!(path = CRegisterUtil::GetSystemEnvironment(L"Path")).IsEmpty())
				{
					path += path.Right(1) == L";" ? L"%JAVA_HOME%\\bin;" : L";%JAVA_HOME%\\bin;";
					if (CRegisterUtil::SetSystemEnvironment(L"Path", path) == ERROR_SUCCESS)
					{
						MessageBox(L"Successful! Please use java -version to check.", L"Successful", MB_OK);
						return;
					}
					updateError++;
				}
				updateError++;
			}
			updateError++;
		}
		updateError++;
	}
	if (updateError == 0)
		MessageBox(L"Please input your java install path!", L"Error", MB_OK);
	else
		MessageBox(L"Config failed,error code is: " + updateError, L"Error", MB_OK);
}