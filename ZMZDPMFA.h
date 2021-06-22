#if !defined(AFX_ZMZDPMFA_H__2D773241_F736_11D7_AF19_006067772FF4__INCLUDED_)
#define AFX_ZMZDPMFA_H__2D773241_F736_11D7_AF19_006067772FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZMZDPMFA.h : header file
//
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// ZMZDPMFA dialog

class ZMZDPMFA : public CDialog
{
	// Construction
public:
	ZMZDPMFA(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	CString pmFname;
	JD_CENTER  *pzLine;
	int m_mode;//平面方案方式 0 文件方式 new GTZX 要释放；  1 图选方式
	AcDbObjectId GTZXId;//GTZXId
	XLDataBase DBS;
	// Dialog Data
	//{{AFX_DATA(ZMZDPMFA)
	enum { IDD = IDD_DIALOGZMzdpm };
	CComboBox	m_SelRXD;
	BOOL	m_pzld;
	CString Roadname;
	CString mdbname;
	//}}AFX_DATA
	void OnOK();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ZMZDPMFA)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ZMZDPMFA)
	//void OnOK();
	afx_msg void OnBUTTONSet();
	afx_msg void OnCheckpzld();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMdb();
	afx_msg void OnRadiozxpm();
	afx_msg void OnRadiozrxpm();
	afx_msg void OnRadioyrxpm();
	afx_msg void OnSelchangeComboRxd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZMZDPMFA_H__2D773241_F736_11D7_AF19_006067772FF4__INCLUDED_)
