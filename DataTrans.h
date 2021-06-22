#pragma once

#include "resource.h"
#include "XLDataBase.h"
// DataTrans 对话框

class DataTrans : public CDialog
{
	DECLARE_DYNAMIC(DataTrans)

public:
	DataTrans(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DataTrans();

// 对话框数据
	enum { IDD = IDD_DIALOG_DataTrans };

	void Empty3Rows(FILE *ReadData);//空读3行


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_WDfilepath;
	CString m_DRfilepath;
	afx_msg void OnBnClickedButtonbrowsewd();
	afx_msg void OnBnClickedButtonbrowsedr();
	afx_msg void OnBnClickedButtontrans();
};
