// BTGridCtrl.cpp: implementation of the BTGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BTGridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(BTGridCtrl, CGridCtrl)
//{{AFX_MSG_MAP(JdCenGridCtrl)
ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BTGridCtrl::BTGridCtrl()
{

}

BTGridCtrl::~BTGridCtrl()
{

}

void BTGridCtrl::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
	CGridCtrl::OnEditCell( nRow, nCol, point, nChar);
	CStringArray Items;
	CString strCol=GetItemText(0,nCol);
	CString strCell=GetItemText(nRow,nCol);
	
    if(strCol=="性质")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"全幅");
		Items.Add(L"左幅");
		Items.Add(L"右幅");
//      new CInPlaceList(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,crFore,crBack,Items,strCell,nChar);
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
    if(strCol=="鼻端位置")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"匝道起端");
		Items.Add(L"匝道终端");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
   else if(strCol=="路面横坡")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"同坡");
		Items.Add(L"反坡");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	
    else if(strCol=="栏 目")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"0-地质概况");
		Items.Add(L"1-设计高程");
		Items.Add(L"2-地面高程");
		Items.Add(L"3-坡度/坡长");
		Items.Add(L"4-里程桩号");
		Items.Add(L"5-直线及平曲线");
		Items.Add(L"6-超高");
		Items.Add(L"7-填挖高");
		Items.Add(L"8-竖曲线");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	else if(strCol=="桩号性质")		//20190911新增
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"1-桥头");
		Items.Add(L"2-桥尾");
		Items.Add(L"3-隧头");
		Items.Add(L"4-隧尾");
		Items.Add(L"5-涵洞头");
		Items.Add(L"6-涵洞尾");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	Invalidate();
}

void BTGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CGridCtrl::OnLButtonDown(nFlags,point);
	//跟踪显示

}

void BTGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
	CGridCtrl::OnEndEditCell(nRow,nCol,str);
	
/*	if(nRow>0 && nCol==CTRLPT_COL)
	{
		if(str=="交点")
		{
			SetItemText(nRow,ZX_COL,"不回头");
			SetItemText(nRow,LINK_COL,"无关");
		}
		else if(str=="圆心")
		{
			SetItemText(nRow,ZX_COL,"左转");
			SetItemText(nRow,LINK_COL,"无关");
		}
		UpdateData(FALSE);
		Invalidate();
	}*/
}
