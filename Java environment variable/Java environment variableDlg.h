
// Java environment variableDlg.h : ͷ�ļ�
//

#pragma once


// CJavaenvironmentvariableDlg �Ի���
class CJavaenvironmentvariableDlg : public CDialogEx
{
// ����
public:
	CJavaenvironmentvariableDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_JAVAENVIRONMENTVARIABLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	struct retFunMSG
	{
		LSTATUS MSG;
		CString FunName;
	};
};
