#pragma once
//#include "GridCtrl.h"
#include "NEWGRIDCTRL\GridCtrl.h"
// CTRIMPMT �Ի���

class CTRIMPMT : public CDialog
{
	DECLARE_DYNAMIC(CTRIMPMT)

public:
	CTRIMPMT(CWnd* pParent = acedGetAcadFrame());   // ��׼���캯��
	virtual ~CTRIMPMT();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };
	//CComboBox	m_EndCtrl;
	//CComboBox	m_StartCtrl;
	//CButton	m_CANCLE;
	//CButton	m_OK;
	//CString	m_START;
	//CString	m_END;
	//int NUM;
	//CString	m_NUM;
	//CGridCtrl m_grid;
	//GV_ITEM Item;
	//int Rows;
	//int Cols;
	//CString	m_LC;
	//int		m_TRIMFLAG;
	//BOOL m_IsDrawZDM;
	//CString m_MdbName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
