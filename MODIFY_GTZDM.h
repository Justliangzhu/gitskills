#if !defined(AFX_MODIFY_GTZDM_H__EEA35C0C_DBD6_468A_B2A3_08E492BBCF17__INCLUDED_)
#define AFX_MODIFY_GTZDM_H__EEA35C0C_DBD6_468A_B2A3_08E492BBCF17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MODIFY_GTZDM.h : header file
/////////////////////////////////////////////////////////////////////////////
// MODIFY_GTZDM dialog
#include "NEWGRIDCTRL\GridCtrl.h"
#include "GTZDM.h"
#include "cdxCSizingDialog.h"
#include "resource.h"
///////////////////
class CZDMGridCtrl : public CGridCtrl  
{
public:
	CZDMGridCtrl();
	virtual ~CZDMGridCtrl();
protected:
    virtual void  OnEndEditCell(int nRow, int nCol, CString str);
	
    //{{AFX_MSG(CZjGridCtrl)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

class MODIFY_GTZDM : public cdxCSizingDialog
{
	DECLARE_DYNCREATE(MODIFY_GTZDM)

// Construction
public:
	void tabletoxy(int iBPD,double xlc,double h,double R);
	double xytotable(double x, ACHAR *xlc, double y);
	MODIFY_GTZDM();
	~MODIFY_GTZDM();
	CZDMGridCtrl m_grid;
	GV_ITEM Item;
	GTZDM_ROAD *pzdmLine;
	AcDbObjectId eId;	
	
// Dialog Data
	//{{AFX_DATA(MODIFY_GTZDM)
	enum { IDD = IDD_DIALOG_MODIFY_GTZDM };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(MODIFY_GTZDM)
public:
	virtual void OnCancel();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(MODIFY_GTZDM)
	afx_msg void OnAddbpd();
	afx_msg void OnDelbpd();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBUTTONdraw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void ShuaXin();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFY_GTZDM_H__EEA35C0C_DBD6_468A_B2A3_08E492BBCF17__INCLUDED_)
