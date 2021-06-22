// JdCenGridCtrl.h: interface for the JdCenGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JDCENGRIDCTRL_H__9FF7D961_339A_48F7_8C14_FE814E984EEF__INCLUDED_)
#define AFX_JDCENGRIDCTRL_H__9FF7D961_339A_48F7_8C14_FE814E984EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#define XH_COL 0
#define CTRLPT_COL 1
#define N_COL 2
#define E_COL 3
#define R_COL 5
#define A1_COL 4
#define A2_COL 6
#define ZX_COL 7
#define LINK_COL 8
#define YQXLEN_COL 9
#define JIAZXLEN_COL 10
#define T1_COL 11
#define T2_COL 12
#define ZH_COL 13
#define HY_COL 14
#define YH_COL 15
#define HZ_COL 16

#include "stdarx.h"
#include "GTZDK.h"
class JdCenGridCtrl : public CGridCtrl  
{
public:
	JdCenGridCtrl();
	virtual ~JdCenGridCtrl();

	BOOL Gzxs;
	JD_CENTER *pzLine;
	GTZDK_ROAD *m_pzdk;
	int mode;//1 对交点  0 占地宽
protected:
	virtual void  OnEndEditCell(int nRow, int nCol, CString str);
private:
	CStringArray Items;	
	virtual void  OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);	

	//{{AFX_MSG(JdCenGridCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
    //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
};

#endif // !defined(AFX_JDCENGRIDCTRL_H__9FF7D961_339A_48F7_8C14_FE814E984EEF__INCLUDED_)
