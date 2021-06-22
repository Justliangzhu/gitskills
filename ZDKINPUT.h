#if !defined(AFX_ZDKINPUT_H__A93FDB97_F420_47D8_AD6A_823A0E6F3D25__INCLUDED_)
#define AFX_ZDKINPUT_H__A93FDB97_F420_47D8_AD6A_823A0E6F3D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZDKINPUT.h : header file
//
#include "GTZDK.h"
//#include "ZDKGridCtrl.h"
#include "JdCenGridCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CZDKINPUT dialog

class CZDKINPUT : public CPropertyPage
{
	DECLARE_DYNCREATE(CZDKINPUT)

// Construction
public:
	CZDKINPUT(GTZDK_ROAD *pzdk=NULL,AcDbObjectId zid=0);
	~CZDKINPUT();
	JdCenGridCtrl m_grid;

	AcDbObjectId m_ZdkId;
	GTZDK_ROAD *m_pzdk;
	BOOL	m_GZXS;	
	GV_ITEM Item;	
	int m_Nzdk;
// Dialog Data
	//{{AFX_DATA(CZDKINPUT)
	enum { IDD = IDD_DIALOG_ZDK };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CZDKINPUT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CZDKINPUT)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnCheckGzxs();
	afx_msg void OnButtonApp();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void ShanChuDingWeiYuan();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDKINPUT_H__A93FDB97_F420_47D8_AD6A_823A0E6F3D25__INCLUDED_)
