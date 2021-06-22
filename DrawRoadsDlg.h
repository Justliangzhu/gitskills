#pragma once


// CDrawRoadsDlg 对话框

class CDrawRoadsDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawRoadsDlg)

public:
	CDrawRoadsDlg(CWnd* pParent = acedGetAcadFrame());   // 标准构造函数
	virtual ~CDrawRoadsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DRAWROADS };
	CListBox m_ListBox_UnSel;//未选择线路
	CListBox m_ListBox_Sel;//已选择线路
	CStringArray m_UnSelRoadArray;//未选择数组
	CStringArray m_SelRoadArray;//已选择线路数组
	CStringArray m_AllRoadArray;//所有线路数组

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonSetmoban();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnLbnDblclkListUnsel();
	afx_msg void OnLbnDblclkListSel();
	afx_msg void OnBnClickedOk();
};
