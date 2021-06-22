#pragma once

#include "GridCtrl.h"
//#include "Inplaceedit.h"
//#include "GridCellCombo.h"
#include "BAS_DRAW_FUN_RAIL.h"
#include "XLDataBase.h"
#include "ResizableDialog.h"

// CEditAllRoads 对话框

class CEditAllRoads : public  CResizableDialog
{
	DECLARE_DYNAMIC(CEditAllRoads)

public:
	CEditAllRoads(CWnd* pParent = acedGetAcadFrame());   // 标准构造函数
	virtual ~CEditAllRoads();

	CGridCtrl m_grid;
	GV_ITEM Item;
	CArray<BAS_DRAW_FUN::ModifyRoadTab,BAS_DRAW_FUN::ModifyRoadTab> m_ModifyRoadArray;
	XLDataBase m_xlmdb;
	CString m_MdbName;
	void SetGrid();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDITALLROAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonIns();
	afx_msg void OnBnClickedButtondel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
