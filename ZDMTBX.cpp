// ZDMTBX.cpp : implementation file
//

#include "stdafx.h"
#include "ZDMTBX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
/////////////////////////////////////////////////////////////////////////////
// CZDMTBX property page

IMPLEMENT_DYNCREATE(CZDMTBX, CPropertyPage)

	CZDMTBX::CZDMTBX() : CPropertyPage(CZDMTBX::IDD)
{
	//{{AFX_DATA_INIT(CZDMTBX)
	m_drawbx = FALSE;
	//}}AFX_DATA_INIT
}

CZDMTBX::~CZDMTBX()
{
}

void CZDMTBX::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZDMTBX)
	DDX_Check(pDX, IDC_CHECK_DRAWBX, m_drawbx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZDMTBX, CPropertyPage)
	//{{AFX_MSG_MAP(CZDMTBX)
	ON_BN_CLICKED(IDC_CHECK_DRAWBX, OnCheckDrawbx)
	ON_BN_CLICKED(IDC_BUTTON_BXDEL, OnButtonBxdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZDMTBX message handlers

BOOL CZDMTBX::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;


	char  tmp[80];
	int i;

	CRect rect;
	GetClientRect(rect);
	rect.left+=5;
	rect.top+=40;
	rect.right-=5;
	rect.bottom-=10;
	m_grid.Create(rect, this, 900);

	txtbkclr=RGB(0XFF, 0XFF,0XE0);
	fixbkclr=RGB(155,188,0);


	m_grid.SetRowCount(1);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(5);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetFixedBkColor(fixbkclr);

	Item.nFormat = DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK;
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
	m_grid.SetItemText(0,1,L"支距");
	m_grid.SetItemText(0,2,L"起里程");
	m_grid.SetItemText(0,3,L"终里程");
	m_grid.SetItemText(0,4,L"颜色");	   

	GetDlgItem(IDC_BUTTON_BXDEL)->EnableWindow(false);

	m_grid.SetColumnWidth(0,39);
	m_grid.SetColumnWidth(1,55);
	m_grid.SetColumnWidth(2,78);
	m_grid.SetColumnWidth(3,78);
	m_grid.SetColumnWidth(4,39);

	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	//	   m_grid.SetFocusCell(1,1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CZDMTBX::OnCheckDrawbx() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_drawbx)
	{
		ACHAR tmp[80];
		DBS.Read_XLDbs(mdbname,L"边线地面线表",RoadName);				
		NDMX=DBS.NDMX;
		int i;
		for(i=0;i<DBS.NDMX;i++)
		{
			DmxCsArr[i].ZJ = DBS.DmxCsArr[i].ZJ;
			DmxCsArr[i].sdml = DBS.DmxCsArr[i].sdml;
			DmxCsArr[i].edml = DBS.DmxCsArr[i].edml;
			DmxCsArr[i].Color = DBS.DmxCsArr[i].Color;
		}
		m_grid.SetRowCount(NDMX+1);
		m_grid.SetFixedRowCount(1);
		m_grid.SetColumnCount(5);
		m_grid.SetFixedColumnCount(2);
		m_grid.SetTextBkColor(txtbkclr);
		m_grid.SetFixedBkColor(fixbkclr);

		Item.nFormat = DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK;
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
		m_grid.SetItemText(0,1,L"支距");
		m_grid.SetItemText(0,2,L"起里程");
		m_grid.SetItemText(0,3,L"终里程");
		m_grid.SetItemText(0,4,L"颜色");


		for(i=0;i<NDMX;i++)
		{
			_stprintf(tmp , L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);
			_stprintf(tmp , L"%0.2lf" , DmxCsArr[i].ZJ);
			m_grid.SetItemText(i+1,1,tmp);
			_stprintf(tmp , L"%0.3lf" , DmxCsArr[i].sdml);			
			m_grid.SetItemText(i+1,2,tmp);
			_stprintf(tmp , L"%0.3lf" , DmxCsArr[i].edml);	
			m_grid.SetItemText(i+1,3,tmp);
			_stprintf(tmp , L"%d" , DmxCsArr[i].Color);
			m_grid.SetItemText(i+1,4,tmp);	
		}
		_stprintf(tmp , L"%d" , i+1);
		m_grid.SetItemText(i+1,0,tmp);
		GetDlgItem(IDC_BUTTON_BXDEL)->EnableWindow();

		m_grid.SetColumnWidth(0,39);
		m_grid.SetColumnWidth(1,55);
		m_grid.SetColumnWidth(2,78);
		m_grid.SetColumnWidth(3,78);
		m_grid.SetColumnWidth(4,39);

		for( i=1;i<m_grid.GetRowCount();i++)
			m_grid.SetRowHeight(i, DEFAULTCOLHEI);
		m_grid.ResetScrollBars();
		m_grid.SetFocusCell(1,1);		
	}
	else
	{
		GetDlgItem(IDC_BUTTON_BXDEL)->EnableWindow(false);
		m_grid.SetRowCount(1);
		m_grid.SetFixedRowCount(1);
		m_grid.SetColumnCount(5);
		m_grid.SetFixedColumnCount(1);
		m_grid.SetTextBkColor(txtbkclr);
		m_grid.SetFixedBkColor(fixbkclr);

		Item.nFormat = DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK;
		Item.row = 0;
		for(int k=0;k<m_grid.GetColumnCount();k++)
		{
			for(int i=0;i<m_grid.GetRowCount();i++) 
			{
				Item.row = i;
				Item.col = k;
				m_grid.SetItem(&Item);
			}		
		}	   
		m_grid.SetItemText(0,0,L"序号");
		m_grid.SetItemText(0,1,L"支距");
		m_grid.SetItemText(0,2,L"起里程");
		m_grid.SetItemText(0,3,L"终里程");
		m_grid.SetItemText(0,4,L"颜色");	   
	}
	UpdateData(FALSE);
}

void CZDMTBX::OnButtonBxdel() 
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
		m_grid.Invalidate();
	}	
}
