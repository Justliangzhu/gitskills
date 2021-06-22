#pragma once

#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "XLDataBase.h"

// CEditZDKDlg 对话框

class CEditZDKDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditZDKDlg)

public:
	CEditZDKDlg(CWnd* pParent = acedGetAcadFrame());   // 标准构造函数
	virtual ~CEditZDKDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDITZDK };

	typedef struct
	{
		double cml;
		double dml;
		double LZDK,RZDK;
		int IsInsertHdm;//是否为增加横断面
	}ZDKDAT;

	CGridCtrl m_grid;
	GV_ITEM Item;
	CString m_RoadName;
	CArray<ZDKDAT,ZDKDAT> m_ZDKArray;
	XLDataBase m_DBS;

	void SetGrid();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonIns();
	afx_msg void OnBnClickedButtondel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
