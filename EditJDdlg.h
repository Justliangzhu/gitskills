#if !defined(AFX_EDITJDDLG_H__944567E5_FAFF_11D7_AF19_006067772FF4__INCLUDED_)
#define AFX_EDITJDDLG_H__944567E5_FAFF_11D7_AF19_006067772FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditJDdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditJDdlg dialog
#include "JD_CENTER.h"
#include "resource.h"
class EditJDdlg : public CDialog
{
// Construction
public:
	EditJDdlg(CWnd* pParent = acedGetAcadFrame(),AcDbObjectId eId=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EditJDdlg)
	enum { IDD = IDD_DIALOGEDITJD };
	CComboBox	m_jdsel;
	AcDbObjectId m_zxId;
	BOOL	m_gzxs;
	JD_CENTER *pzx;
	//}}AFX_DATA

    void drawdata(int i);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditJDdlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EditJDdlg)
	afx_msg void OnButtonapply();
	afx_msg void OnCheckgz();
	afx_msg void OnSelchangeCombojd();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITJDDLG_H__944567E5_FAFF_11D7_AF19_006067772FF4__INCLUDED_)
