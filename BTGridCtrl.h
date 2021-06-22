// BTGridCtrl.h: interface for the BTGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BTGRIDCTRL_H__CDAAF4F4_5E66_4D81_BC66_BD9659917628__INCLUDED_)
#define AFX_BTGRIDCTRL_H__CDAAF4F4_5E66_4D81_BC66_BD9659917628__INCLUDED_
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "stdarx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BTGridCtrl : public CGridCtrl 
{
public:
	BTGridCtrl();
	virtual ~BTGridCtrl();

	virtual void  OnEndEditCell(int nRow, int nCol, CString str);
private:
	CStringArray Items;	
	virtual void  OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);	
	
	//{{AFX_MSG(JdCenGridCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
    //}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

#endif // !defined(AFX_BTGRIDCTRL_H__CDAAF4F4_5E66_4D81_BC66_BD9659917628__INCLUDED_)
