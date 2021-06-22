// EditBP.cpp : implementation file
//

#include "StdAfx.h"
//#include "resource.h"
#include "EditBP.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditBP dialog

//extern double TM[30],TH[30],TPTW[30];
//extern int nBPJS;

CEditBP::CEditBP(CWnd* pParent /*=NULL*/)
	: CDialog(CEditBP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditBP)
	// NOTE: the ClassWizard will add member initialization here
	ifCur=ifPart=ifAll=false;
	//}}AFX_DATA_INIT
}


void CEditBP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditBP)
	//	DDX_GridControl(pDX, IDC_GRID,m_grid);//分段
	// NOTE: the ClassWizard will add DDX and DDV calls here

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditBP, CDialog)
	//{{AFX_MSG_MAP(CEditBP)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_BN_CLICKED(IDC_Cur, OnCur)
	ON_BN_CLICKED(IDC_SomeLCD, OnSomeLCD)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DElETE, OnDElETE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditBP message handlers

void CEditBP::OnOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	int i,j;
	BPJS=m_grid.GetRowCount()-1;
	for(i=1;i<lRows;i++)
	{
		m_M[i-1]=_wtof(m_grid.GetItemText(i,1));
		m_H[i-1]=_wtof(m_grid.GetItemText(i,2));
		m_PTW[i-1]=_wtof(m_grid.GetItemText(i,3));
		ads_printf(L"m_M%f\n",m_M[0]);


	}
	/*
	if(nLTBPJS>0)//路堤
	{
	nLTBPJS=BPJS;
	for(i=0; i<nLTBPJS; i++)
	{
	WM[i] = m_M[i];
	PH[i] = m_H[i];
	ZPTW[i]=m_PTW[i];
	}	

	}
	if(nLZBPJS>0)//路崭
	{
	nLZBPJS=BPJS;
	for(i=0; i<nLZBPJS; i++)
	{
	m_M[i] = WM[i];
	m_H[i] = PH[i];
	m_PTW[i]=ZPTW[i];
	}
	}*/


}

void CEditBP::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CEditBP::OnAll() 
{
	// TODO: Add your control notification handler code here
	m_Button_Cur->SetCheck(0);//当前断面
	m_Button_All->SetCheck(1);
	m_Button_Part->SetCheck(0);
	ifCur=ifPart=false;
	ifAll=true;
}

void CEditBP::OnCur() 
{
	// TODO: Add your control notification handler code here
	m_Button_Cur->SetCheck(1);//当前断面
	m_Button_All->SetCheck(0);
	m_Button_Part->SetCheck(0);
	ifCur=true;
	ifPart=ifAll=false;
}

void CEditBP::OnSomeLCD() 
{
	// TODO: Add your control notification handler code here
	m_Button_Cur->SetCheck(0);//当前断面
	m_Button_All->SetCheck(0);
	m_Button_Part->SetCheck(1);
	ifCur=ifAll=false;
	ifPart=true;
}

BOOL CEditBP::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int i,j,k,xh=0;
	int iIndex=0;
	//	if(BPJS>0)
	//	{
	lRows=BPJS+1;
	iCols=4;
	//	BPJS=0;
	/*
	for(int i=0; i<BPJS; i++)
	{
	m_M[i] = TM[i];
	m_H[i] = TH[i];
	m_PTW[i]=TPTW[i];
	}
	*/

	//	}
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=40;
	rect.right-=100;
	rect.bottom-=30;
	m_grid.Create(rect, this, 900);

	COLORREF txtbkclr=RGB(0XFF, 0XFF,0XE0);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetRowCount(lRows);         //数据库的行数
	m_grid.SetColumnCount(iCols);
	m_grid.SetFixedRowCount(1);
	//m_grid.SetFixedColumnCount(1);
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;

	m_grid.SetItemText(0,0,L"边坡级数");
	m_grid.SetItemText(0,1,L"边坡系数");
	m_grid.SetItemText(0,2,L"限制高(m)");
	m_grid.SetItemText(0,3,L"平台宽(m)");

	for(j=1;j<lRows;j++)
	{
		for (k=0; k<4; k++)
		{
			Item.col=k;
			Item.row=j;
			Item.strText=" ";
			m_grid.SetItem(&Item);   //初始化" "
		}
	}
	for(i=1;i<lRows;i++)
	{
		Item.row=i;
		Item.col=0;
		CString JS;
		JS.Format(L"%d",xh+1);
		Item.strText=JS;
		m_grid.SetItem(&Item);

		Item.col=1;
		Item.strText.Format(L"%0.2lf",m_M[i-1]);
		m_grid.SetItem(&Item);

		Item.col=2;
		Item.strText.Format(L"%0.2lf",m_H[i-1]);
		m_grid.SetItem(&Item);

		Item.col=3;
		Item.strText.Format(L"%0.2lf",m_PTW[i-1]);
		m_grid.SetItem(&Item);
		xh++;

	}
	double hbp=100.00;
	Item.row=lRows-1;Item.col=2;
	Item.strText.Format(L"%0.2lf",hbp);
	m_grid.SetItem(&Item);//最后一级边坡高设为100

	for(k=0; k<4; k++)
		m_grid.SetColumnWidth(k,70);
	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.Invalidate();

	m_Button_Cur=(CButton*)GetDlgItem(IDC_Cur);
	m_Button_Part=(CButton*)GetDlgItem(IDC_SomeLCD);
	m_Button_All=(CButton*)GetDlgItem(IDC_ALL);
	///////////////////所有线路土石头///////////////
	m_Button_Cur->SetCheck(1);//当前断面
	m_Button_All->SetCheck(0);
	m_Button_Part->SetCheck(0);
	ifCur=true;
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditBP::OnAdd() 
{
	// TODO: Add your control notification handler code here
	int i;
	CString tr;
	int nRow;
	if(m_grid.GetRowCount()==1)          // 新建表格里没有行时，加1行
	{
		m_grid.SetRowCount(2);    
		for(i=1;i<m_grid.GetColumnCount();i++)
		{
			m_grid.SetItemText(1,i,L" ");
		}
		m_grid.SetItemText(1,0,L"1");
		m_grid.SetColumnWidth(1,70);
		m_grid.SetRowHeight(1, DEFAULTCOLHEI);
		m_grid.SetFocusCell(0,1);
		m_grid.Invalidate();

	}
	else
	{
		nRow = m_grid.GetFocusCell().row;

		if (nRow > 0)
		{ 
			Item.row=nRow;
			m_grid.InsertRow(_T("NEW"), nRow);
			for( i=0;i<m_grid.GetColumnCount();i++)
			{
				Item.col=i;
				m_grid.SetItem(&Item);
			}
			for(i=nRow;i<m_grid.GetRowCount();i++)
			{
				CString str;
				Item.row=i;
				Item.col=0;
				str.Format(L"%d",i);
				Item.strText=str;
				m_grid.SetItem(&Item);
			}

			for( i=1;i<m_grid.GetColumnCount();i++) 
			{

				m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
				m_grid.SetItemText(nRow+1,i,L" ");
			}
			m_grid.SetFocusCell(nRow+1,1);
			m_grid.Invalidate();
		}
	}
	m_grid.SetColumnWidth(nRow+1,70);
	m_grid.SetRowHeight(nRow+1, DEFAULTCOLHEI);
}

void CEditBP::OnDElETE() 
{
	// TODO: Add your control notification handler code here
	CCellRange selectcell;
	selectcell=m_grid.GetSelectedCellRange();
	int minRow=selectcell.GetMinRow();
	int maxRow=selectcell.GetMaxRow();
	//int nRow = m_grid.GetFocusCell().row;
	if(minRow>=0 && maxRow<=m_grid.GetRowCount())
	{
		for(int nRow=minRow;nRow<=maxRow;nRow++)
			m_grid.DeleteRow(minRow);

	}
	m_grid.Invalidate();
	CString str;
	for(int i=minRow;i<m_grid.GetRowCount();i++)
	{
		str.Format(L"%d",i);
		m_grid.SetItemText(i,0,str);
	}
	m_grid.SetFocusCell(minRow,1);
	m_grid.Invalidate();		

	if(minRow>=1)
	{		
		m_grid.SetSelectedRange(minRow,0,minRow,m_grid.GetColumnCount()-1,true);
		m_grid.SetFocusCell(minRow,1);
		//if((nRow--)>=1)
		//Report_input.Move(nRow,adBookmarkFirst);		
	}
	else
	{
		int nRows=m_grid.GetRowCount()-1;
		if(nRows>=1)
		{
			minRow=1;
			m_grid.SetSelectedRange(minRow,0,minRow,m_grid.GetColumnCount()-1,true);
			m_grid.SetFocusCell(minRow,1);
			//Report_input.Move(0,adBookmarkFirst);
		}
		//else
		// m_btn_del.EnableWindow(false);
	}

}
