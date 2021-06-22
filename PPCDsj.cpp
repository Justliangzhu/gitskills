// PPCDsj.cpp : implementation file
//

#include "stdafx.h"
#include "PPCDsj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PPCDsj property page

IMPLEMENT_DYNCREATE(PPCDsj, CPropertyPage)

	PPCDsj::PPCDsj() : CPropertyPage(PPCDsj::IDD,1)
{
	//{{AFX_DATA_INIT(PPCDsj)
	// NOTE: the ClassWizard will add member initialization here
	NPPCD=0;//爬坡车道分段数
	PPCD=NULL;//爬坡车道数
	//}}AFX_DATA_INIT
}

PPCDsj::~PPCDsj()
{
}

void PPCDsj::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PPCDsj)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PPCDsj, CPropertyPage)
	//{{AFX_MSG_MAP(PPCDsj)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PPCDsj message handlers

void PPCDsj::OnBUTTONAdd() 
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

		if(m_grid.IsEpt(nRow,3))
			m_grid.SetItemText(nRow,3,L"4");

		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();
	}		
}

BOOL PPCDsj::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=15;
	rect.right-=10;
	rect.bottom-=40;
	m_grid.Create(rect, this, 9900);

	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;
	SetGrid();
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control

}

void PPCDsj::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertyPage::OnOK();
	int i,j;
	ACHAR tmp[80];
	NPPCD = m_grid.GetRowCount() - 2;  
	if(PPCD) delete [] PPCD;
	if(NPPCD>0)
		PPCD = new  PPCDdata[NPPCD];
	else
		return;
	if(PPCD == NULL)
	{
		AfxMessageBox(L"分配内存出错!");
		return;
	}      
	for(i=1; i<=NPPCD; i++)
	{
		_tcscpy(tmp, m_grid.GetItemText(i, 1));
		PPCD[i-1].sdml = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 2));
		PPCD[i-1].edml = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 3));
		PPCD[i-1].max_hp = _wtof(tmp);
	}
}

void PPCDsj::OnBUTTONdel() 
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
void PPCDsj::SetGrid() 
{
	ACHAR  tmp[80];
	int i,j;

	txtbkclr=RGB(0XFF, 0XFF,0XE0);
	fixbkclr=RGB(155,188,0);

	m_grid.SetRowCount(NPPCD+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(4);
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
	m_grid.SetItemText(0,1,L"分段起始里程");
	m_grid.SetItemText(0,2,L"分段终止里程");
	m_grid.SetItemText(0,3,L"最大横坡(%)");


	i=0;
	if(NPPCD != 0)
	{
		for(i=0; i<NPPCD; i++)
		{
			ads_rtos(PPCD[i].sdml,2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos(PPCD[i].edml,2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);

			ads_rtos(PPCD[i].max_hp,2,3,tmp);
			m_grid.SetItemText(i+1,3,tmp);


			_stprintf(tmp , L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}     
	}
	_stprintf(tmp , L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);

	m_grid.SetColumnWidth(0,40);
	m_grid.SetColumnWidth(1,100);
	m_grid.SetColumnWidth(2,100);
	m_grid.SetColumnWidth(3,100);

	m_grid.SetRowHeight(0, DEFAULTCOLHEI);
	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

}
void PPCDsj::SetPPCD(int nppcd,PPCDdata ppcd[]) 
{
	NPPCD = nppcd;
	if(PPCD) delete []PPCD;
	PPCD = NULL;
	if(NPPCD>0)
	{
		PPCD = new PPCDdata[NPPCD];
		for(int i=0; i<NPPCD; i++)
			PPCD[i] = ppcd[i];	
	}

}
