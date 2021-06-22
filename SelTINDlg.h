#pragma once
#include "resource.h"
#include "afxwin.h"

// CSelTINDlg 对话框

class CSelTINDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelTINDlg)

public:
	CSelTINDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelTINDlg();
	CStringArray m_TINNameArray;//数模名称数组
	CString m_SelTinName;//选择的数模名称
// 对话框数据
	enum { IDD = IDD_DIALOG_SELECTTIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CombTINName;
	void IniTINArray();//初始化数模名称列表
	int AddOneName(CString FileNameStr);//将线路名加入到列表中,返回加入后所在列表的下标，无
	void CopyTin();//拷贝数模
	void SetCurTin();	//2019.3.9修改
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	CListBox m_TinNameListBox;	//2019.3.9改为listbox显示
	afx_msg void OnLbnDblclkListSeltin();
	afx_msg void OnBnClickedButtonDeletetinname();
	bool DeleteTin(CString TinName);
	CString m_CurTinName;
};
