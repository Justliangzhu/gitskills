// NCDMXCS.cpp : implementation file
//

#include "stdafx.h"
#include "NCDMXCS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNCDMXCS dialog
extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;

CNCDMXCS::CNCDMXCS(double sdml, double edml,CWnd* pParent /*=NULL*/)
	: CDialog(CNCDMXCS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNCDMXCS)
	m_ncspacer = _T("20");
	m_ifdtm = 0;
	//}}AFX_DATA_INIT
	m_sdml=sdml;
	m_edml=edml;
	NDMX = 0;
}


void CNCDMXCS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNCDMXCS)
	DDX_Text(pDX, IDC_EDIT_NCSPACER, m_ncspacer);
	DDX_Radio(pDX, IDC_RADIO_DTM, m_ifdtm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNCDMXCS, CDialog)
	//{{AFX_MSG_MAP(CNCDMXCS)
	ON_BN_CLICKED(IDC_BUTTON_NCADD, OnButtonNcadd)
	ON_BN_CLICKED(IDC_BUTTON_NCDEL, OnButtonNcdel)
	ON_BN_CLICKED(IDC_BUTTON_PICKZDMTK, OnButtonPickzdmtk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNCDMXCS message handlers

BOOL CNCDMXCS::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;


	ACHAR  tmp[80];
	int i;

	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=97;
	rect.right-=10;
	rect.bottom-=45;
	m_grid.Create(rect, this, 900);

	txtbkclr=RGB(0XFF, 0XFF,0XE0);
	fixbkclr=RGB(155,188,0);


	m_grid.SetRowCount(NDMX+2);
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

	if(NDMX==0)
	{
		_stprintf(tmp ,L"%d" , 1);
		m_grid.SetItemText(1,0,tmp);
		_stprintf(tmp ,L"%d" , 10);
		m_grid.SetItemText(1,1,tmp);
		_stprintf(tmp ,L"%0.3lf" , m_sdml);			
		m_grid.SetItemText(1,2,tmp);
		_stprintf(tmp ,L"%0.3lf" , m_edml);	
		m_grid.SetItemText(1,3,tmp);
		_stprintf(tmp ,L"%d" , 3);
		m_grid.SetItemText(1,4,tmp);	
		_stprintf(tmp ,L"%d" , 2);
		m_grid.SetItemText(2,0,tmp);
		GetDlgItem(IDC_BUTTON_NCADD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_NCDEL)->EnableWindow(false);	
		GetDlgItem(IDOK)->EnableWindow(false);
	}
	else
	{
		for(i=0;i<NDMX;i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);
			_stprintf(tmp ,L"%0.2lf" , DmxCsArr[i].ZJ);
			m_grid.SetItemText(i+1,1,tmp);
			_stprintf(tmp ,L"%0.3lf" , DmxCsArr[i].sdml);			
			m_grid.SetItemText(i+1,2,tmp);
			_stprintf(tmp ,L"%0.3lf" , DmxCsArr[i].edml);	
			m_grid.SetItemText(i+1,3,tmp);
			_stprintf(tmp ,L"%d" , DmxCsArr[i].Color);
			m_grid.SetItemText(i+1,4,tmp);	
		}
		_stprintf(tmp ,L"%d" , i+1);
		m_grid.SetItemText(i+1,0,tmp);
		GetDlgItem(IDC_EDIT_NCSPACER)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_PICKZDMTK)->EnableWindow(false);
	}

	m_grid.SetColumnWidth(0,39);
	m_grid.SetColumnWidth(1,45);
	m_grid.SetColumnWidth(2,80);
	m_grid.SetColumnWidth(3,80);
	m_grid.SetColumnWidth(4,39);

	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNCDMXCS::OnButtonNcadd() 
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

		if(m_grid.GetRowCount()>3)
		{
			if(nRow==m_grid.GetRowCount()-2)
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow-1,i));
			}
			else
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
			}
		}
		m_grid.SetFocusCell(nRow,1);

		m_grid.Invalidate();
	}			
}

void CNCDMXCS::OnButtonNcdel() 
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

void CNCDMXCS::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	int NRow,i;
	NRow=0;
	for(i=1; i<m_grid.GetRowCount(); i++)
	{
		int flag;
		m_grid.RowIsEpt(i,m_grid.GetColumnCount(),flag);

		if(flag)//不空
			NRow++;
		else
			break;
	}//统计非空行数

	NDMX = NRow;

	for(i=1; i<=NDMX;i++)
	{
		DmxCsArr[i-1].ZJ =_wtof(m_grid.GetItemText(i, 1));
		DmxCsArr[i-1].sdml=_wtof(m_grid.GetItemText(i, 2));
		DmxCsArr[i-1].edml=_wtof(m_grid.GetItemText(i, 3));
		DmxCsArr[i-1].Color=_wtoi(m_grid.GetItemText(i, 4));
	}
	CDialog::OnOK();
}

void CNCDMXCS::OnButtonPickzdmtk() 
{
	// TODO: Add your control notification handler code here

	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	CString mes;
	ACHAR tmp[80];
	AcGePoint3d PT;
	ads_point resPt;
	if(acedGetPoint(NULL,L"请在纵面图内拾取一点!\n",resPt)==RTNORM)
	{
		PT.x = resPt[X];
		PT.y = resPt[Y];
		izdm = GlobalZDMTK.FindWichZdm(PT);
		if(izdm>=0)
		{
			GetDlgItem(IDC_BUTTON_NCADD)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_NCDEL)->EnableWindow(true);	
			GetDlgItem(IDOK)->EnableWindow(true);
			if(GlobalZDMTK.ZDMTK[izdm].pm)
			{
				_stprintf(tmp ,L"%0.3lf" , GlobalZDMTK.ZDMTK[izdm].pm->DLArray[0].ELC);			
				m_grid.SetItemText(1,2,tmp);
				_stprintf(tmp ,L"%0.3lf" , GlobalZDMTK.ZDMTK[izdm].pm->DLArray[0].ELC+GlobalZDMTK.ZDMTK[izdm].pm->LenOfLine);	
				m_grid.SetItemText(1,3,tmp);
			}
			UpdateData(false);	
		}
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/	
}
