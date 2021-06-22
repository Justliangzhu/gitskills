#pragma once

#include "GridCtrl.h"
//#include "Inplaceedit.h"
//#include "GridCellCombo.h"
#include "BAS_DRAW_FUN_RAIL.h"
#include "XLDataBase.h"
#include "ResizableDialog.h"

// CEditAllRoads �Ի���

class CEditAllRoads : public  CResizableDialog
{
	DECLARE_DYNAMIC(CEditAllRoads)

public:
	CEditAllRoads(CWnd* pParent = acedGetAcadFrame());   // ��׼���캯��
	virtual ~CEditAllRoads();

	CGridCtrl m_grid;
	GV_ITEM Item;
	CArray<BAS_DRAW_FUN::ModifyRoadTab,BAS_DRAW_FUN::ModifyRoadTab> m_ModifyRoadArray;
	XLDataBase m_xlmdb;
	CString m_MdbName;
	void SetGrid();

// �Ի�������
	enum { IDD = IDD_DIALOG_EDITALLROAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonIns();
	afx_msg void OnBnClickedButtondel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
