#if !defined(AFX_LMSJ_H__B8B5B192_35E3_4270_A8C8_E50490D59799__INCLUDED_)
#define AFX_LMSJ_H__B8B5B192_35E3_4270_A8C8_E50490D59799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LMSJ.h : header file
//
#include "BAS_DRAW_FUN.h"
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// LMSJ dialog
class CLMKGridCtrl : public CGridCtrl 
{
public:
	CLMKGridCtrl();
	virtual ~CLMKGridCtrl();
	
protected:
    CStringArray Items;	
	virtual void  OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);	
	//{{AFX_MSG(CComboGridCtrl)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

class LMSJ : public CPropertyPage, BAS_DRAW_FUN 
{
	DECLARE_DYNCREATE(LMSJ)

// Construction
public:
	LMSJ();
	~LMSJ();

	void DrawLine(LMKdata &tmplmk);
// Dialog Data
	//{{AFX_DATA(LMSJ)
	enum { IDD = IDD_DIALOG_LM };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
   
	GV_ITEM Item;
	int NLMK;
	LMKdata *LMK;
	int ZorY;
	int flag1;
	void SetLmk(int nlmk,LMKdata lmk[]) ;
	void SetGrid(); 

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(LMSJ)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(LMSJ)
	afx_msg void OnBUTTONAdd();
	afx_msg void OnButtonCopy();
	afx_msg void OnBUTTONdel();
	afx_msg void OnBUTTONrefresh();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CLMKGridCtrl m_grid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMSJ_H__B8B5B192_35E3_4270_A8C8_E50490D59799__INCLUDED_)
