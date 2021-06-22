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
	
    if(strCol=="����")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"ȫ��");
		Items.Add(L"���");
		Items.Add(L"�ҷ�");
//      new CInPlaceList(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,crFore,crBack,Items,strCell,nChar);
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
    if(strCol=="�Ƕ�λ��")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"�ѵ����");
		Items.Add(L"�ѵ��ն�");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
   else if(strCol=="·�����")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"ͬ��");
		Items.Add(L"����");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	
    else if(strCol=="�� Ŀ")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"0-���ʸſ�");
		Items.Add(L"1-��Ƹ߳�");
		Items.Add(L"2-����߳�");
		Items.Add(L"3-�¶�/�³�");
		Items.Add(L"4-���׮��");
		Items.Add(L"5-ֱ�߼�ƽ����");
		Items.Add(L"6-����");
		Items.Add(L"7-���ڸ�");
		Items.Add(L"8-������");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	else if(strCol=="׮������")		//20190911����
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"1-��ͷ");
		Items.Add(L"2-��β");
		Items.Add(L"3-��ͷ");
		Items.Add(L"4-��β");
		Items.Add(L"5-����ͷ");
		Items.Add(L"6-����β");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	Invalidate();
}

void BTGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CGridCtrl::OnLButtonDown(nFlags,point);
	//������ʾ

}

void BTGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
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
