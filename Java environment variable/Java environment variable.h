
// Java environment variable.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CJavaenvironmentvariableApp: 
// �йش����ʵ�֣������ Java environment variable.cpp
//

class CJavaenvironmentvariableApp : public CWinApp
{
public:
	CJavaenvironmentvariableApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CJavaenvironmentvariableApp theApp;