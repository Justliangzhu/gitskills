#pragma once

#include "resource.h"
#include "XLDataBase.h"
// DataTrans �Ի���

class DataTrans : public CDialog
{
	DECLARE_DYNAMIC(DataTrans)

public:
	DataTrans(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DataTrans();

// �Ի�������
	enum { IDD = IDD_DIALOG_DataTrans };

	void Empty3Rows(FILE *ReadData);//�ն�3��


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_WDfilepath;
	CString m_DRfilepath;
	afx_msg void OnBnClickedButtonbrowsewd();
	afx_msg void OnBnClickedButtonbrowsedr();
	afx_msg void OnBnClickedButtontrans();
};
