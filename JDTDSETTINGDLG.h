#if !defined(AFX_JDTDSETTINGDLG_H__DB67AB22_97C4_4D82_AF2D_A15BF51EEC4D__INCLUDED_)
#define AFX_JDTDSETTINGDLG_H__DB67AB22_97C4_4D82_AF2D_A15BF51EEC4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JDTDSETTINGDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// JDTDSETTINGDLG dialog
//交点拖动方式设置对话框
class JDTDSETTINGDLG : public CDialog
{
// Construction
public:
	JDTDSETTINGDLG(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(JDTDSETTINGDLG)
	enum { IDD = IDD_DIALOG_JDTDSETTING };
	int		m_TDFS;
	int		m_PRE;
	CString	m_R;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(JDTDSETTINGDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(JDTDSETTINGDLG)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JDTDSETTINGDLG_H__DB67AB22_97C4_4D82_AF2D_A15BF51EEC4D__INCLUDED_)
