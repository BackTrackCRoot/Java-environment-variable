
// Java environment variableDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Java environment variable.h"
#include "Java environment variableDlg.h"
#include "afxdialogex.h"
#include "RegisterUtil.h"

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

	CString jdkpath = CRegisterUtil::GetJavaEnvironment(L"SOFTWARE\\JavaSoft\\Java Development Kit\\1.8", L"JavaHome");
	if (jdkpath != "")
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(jdkpath);
	else
		MessageBox(L"JDK��ȡ������ʼ��ʧ�ܣ����ֶ���д��");
	CString jrepath = CRegisterUtil::GetJavaEnvironment(L"SOFTWARE\\JavaSoft\\Java Runtime Environment\\1.8", L"JavaHome");
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