#pragma once
#include "XLDataBase.h"

// DMXDataTrans 对话框

class DMXDataTrans : public CDialog
{
	DECLARE_DYNAMIC(DMXDataTrans)

public:
	DMXDataTrans(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DMXDataTrans();



// 对话框数据
	enum { IDD = IDD_DIALOG_DMXtrans };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonbrowswd();
	CString m_WDdmxPath;
	CString m_DRmdbPath;
	afx_msg void OnBnClickedButtonbrowsdr();
	afx_msg void OnBnClickedButtonDmxtrans();
	CString m_WDzdmPath;
	afx_msg void OnBnClickedButtonbrowszdm();
	afx_msg void OnBnClickedButtonZdmtrans();
};
