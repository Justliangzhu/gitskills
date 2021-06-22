// BTGridCtrl.cpp: implementation of the BTGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LMKGridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(LMKGridCtrl, CGridCtrl)
//{{AFX_MSG_MAP(JdCenGridCtrl)
ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LMKGridCtrl::LMKGridCtrl()
{

}

LMKGridCtrl::~LMKGridCtrl()
{

}

void LMKGridCtrl::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
	if(  nCol>1 )return;
	CGridCtrl::OnEditCell( nRow, nCol, point, nChar);
	CStringArray Items;
	CString strCol=GetItemText(0,nCol);
	CString strCell=GetItemText(nRow,nCol);
	
    if(strCol=="��������")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"�зִ�");
		Items.Add(L"·Ե��");
		Items.Add(L"���е�");
		Items.Add(L"Ӳ·��");
		Items.Add(L"��·��");
//      new CInPlaceList(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,crFore,crBack,Items,strCell,nChar);
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	Invalidate();
}

void LMKGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CGridCtrl::OnLButtonDown(nFlags,point);
	//������ʾ

}

void LMKGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
	CGridCtrl::OnEndEditCell(nRow,nCol,str);
	
/*	if(nRow>0 && nCol==CTRLPT_COL)
	{
		if(str=="����")
		{
			SetItemText(nRow,ZX_COL,"����ͷ");
			SetItemText(nRow,LINK_COL,"�޹�");
		}
		else if(str=="Բ��")
		{
			SetItemText(nRow,ZX_COL,"��ת");
			SetItemText(nRow,LINK_COL,"�޹�");
		}
		UpdateData(FALSE);
		Invalidate();
	}*/
}
