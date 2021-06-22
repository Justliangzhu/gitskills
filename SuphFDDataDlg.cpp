// SuphFDDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SuphFDDataDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;

/////////////////////////////////////////////////////////////////////////////
// SuphFDDataDlg property page

IMPLEMENT_DYNCREATE(SuphFDDataDlg, CPropertyPage)

	SuphFDDataDlg::SuphFDDataDlg() : CPropertyPage(SuphFDDataDlg::IDD,1)
{
	//{{AFX_DATA_INIT(SuphFDDataDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}	int NCG;
	NCG = 0;
}

SuphFDDataDlg::~SuphFDDataDlg()
{
}

void SuphFDDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SuphFDDataDlg)
	//	DDX_GridControl(pDX,IDC_LIST_CGdata,m_grid); 
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_SuphFD, m_grid);
}


BEGIN_MESSAGE_MAP(SuphFDDataDlg, CPropertyPage)
	//{{AFX_MSG_MAP(SuphFDDataDlg)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SuphFDDataDlg message handlers

void SuphFDDataDlg::OnBUTTONAdd() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		Item.row=nRow;
		m_grid.InsertRow(_T("NEW"), nRow);
		int i;
		for( i=0;i<m_grid.GetColumnCount();i++)
		{
			Item.col=i;
			m_grid.SetItem(&Item);
		}
		for(i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		for( i=1;i<m_grid.GetRowCount();i++)
			m_grid.SetRowHeight(i, DEFAULTCOLHEI);

		if(m_grid.GetRowCount()>3)
		{
			if(nRow==m_grid.GetRowCount()-2)
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow-1,i));
			}
			else
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.AutoSize();
		m_grid.Invalidate();
	}	

}

void SuphFDDataDlg::OnBUTTONdel() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	if  (nRow==m_grid.GetRowCount()-1) return;
	if (nRow >= 0)
	{
		m_grid.DeleteRow(nRow);
		for(int i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.AutoSize();
		m_grid.Invalidate();
	}	


}

BOOL SuphFDDataDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;

	ACHAR  tmp[80];
	int i;

	txtbkclr=RGB(0XFF, 0XFF,0XE0);
	fixbkclr=RGB(155,188,0);
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=15;
	//rect.right-=10;
	rect.right+=15;
	rect.bottom-=60;	//20190712	修改成60
	//m_grid.Create(rect, this, 900);

	m_grid.SetRowCount(NCG+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(3);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetFixedBkColor(fixbkclr);

	//Item.nFormat = DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK;
	Item.row = 0;
	for(int k=0;k<m_grid.GetColumnCount();k++)
	{
		for( i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}		
	}

	m_grid.SetItemText(0,0,L"序号");
	m_grid.SetItemText(0,1,L"分段终里程");
	if(CgJkInf==0)
		m_grid.SetItemText(0,2,L"路面横坡%");
	else
		m_grid.SetItemText(0,2,L"路面加宽");

	i=0;
	if(NCG > 0)
	{
		for(i=0; i<NCG; i++)
		{
			ads_rtos(CG[i].xlc,2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos(CG[i].cg,2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);

			_stprintf(tmp , L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}     
	}
	_stprintf(tmp , L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);

	m_grid.SetColumnWidth(0,40);
	m_grid.SetColumnWidth(1,95);
	m_grid.SetColumnWidth(2,75);
	m_grid.SetRowHeight(0, DEFAULTCOLHEI);
	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);
	m_grid.AutoSize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SuphFDDataDlg::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertyPage::OnOK();

	ACHAR tmp[80];
	NCG = m_grid.GetRowCount() - 2;  	

	for(int i=1; i<=NCG; i++)
	{
		_tcscpy(tmp, m_grid.GetItemText(i, 1));
		CG[i-1].xlc = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 2));
		CG[i-1].cg = _wtof(tmp);		
	}
	//写数据库
}
