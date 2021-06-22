// EditDMXDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditDMXDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
/////////////////////////////////////////////////////////////////////////////
// EditDMXDlg dialog


EditDMXDlg::EditDMXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EditDMXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditDMXDlg)
	//}}AFX_DATA_INIT
}


void EditDMXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditDMXDlg)
	//    DDX_GridControl(pDX, IDC_LIST1,m_grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_EDITEARTH, m_grid);
}


BEGIN_MESSAGE_MAP(EditDMXDlg, CDialog)
	//{{AFX_MSG_MAP(EditDMXDlg)
	ON_BN_CLICKED(IDC_BUTTONADD, OnButtonadd)
	ON_BN_CLICKED(IDC_BUTTONDEL, OnButtondel)
	//	ON_BN_CLICKED(IDC_BUTTONSelDMX, OnBUTTONSelDMX)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditDMXDlg message handlers
void EditDMXDlg::OnButtondel() 
{
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
		m_grid.AutoSize();
		m_NDmd--;
	}

}

void EditDMXDlg::OnButtonadd() 
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
		m_NDmd++;
	}	

}

void EditDMXDlg::OnBUTTONSelDMX() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);		// Hide our dialog
	CString workdir;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	workdir += "\\data";   

	CFileDialog fname(false);
	fname.m_ofn.lpstrDefExt = L"DMX";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"地面线文件(*.dmx)\0*.dmx\0\0";
	fname.DoModal();	
	DmxFname =fname.GetPathName(); 	

	ShowWindow(SW_SHOW); // Display our dialog again
}

void EditDMXDlg::OnOK() 
{
	if(m_eId.isNull())
	{
		CDialog::OnOK();
		return ;
	}

	CDialog::OnOK();
	UpdateData();
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, m_eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,m_eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	if (pObj->isKindOf(DMX_ROAD::desc() ) ) //是DMX实体,取数据
	{  
		pObj->close();
		acdbOpenObject(pObj, m_eId, AcDb::kForWrite);
		pdmx = DMX_ROAD::cast(pObj);
		pdmx->assertWriteEnabled();

		int NRow;
		NRow=0;

		int i;
		for(i=1; i<m_grid.GetRowCount(); i++)
		{
			int flag;
			m_grid.RowIsEpt(i,m_grid.GetColumnCount(),flag);

			if(flag)//不空
				NRow++;
			else
				break;
		}//统计非空行数
		m_NDmd = pdmx->NUM = NRow;//m_grid.GetRowCount()-2;
		for(i=1; i<=m_NDmd; i++)
		{
			ACHAR Sml[80];
			//		   _tcscpy(pdmx->DMXys[i-1].ckml,m_grid.GetItemText(i, 1));
			pdmx->DMXys[i-1].dml = _wtof(m_grid.GetItemText(i, 1));
			pdmx->DMXys[i-1].lc=pzLinep->TYLC(pdmx->DMXys[i-1].dml);
			_tcscpy(Sml,m_grid.GetItemText(i, 2));
			pdmx->DMXys[i-1].level=_wtof(Sml);
		}
		/*
		if(DmxFname!="")
		{
		FILE *fpw;
		fpw = _wfopen(DmxFname,L"w");
		fwprintf(fpw,L"%ld\n",m_NDmd);
		for(int i=0; i<m_NDmd; i++)
		fwprintf(fpw,L"%s  %0.3lf\n",pdmx->DMXys[i].ckml,pdmx->DMXys[i].level);
		fclose(fpw);
		}	  */

		pdmx->close();
	}
	else
		pObj->close();


	CDialog::OnOK();
}

BOOL EditDMXDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=50;
	rect.right-=10;
	rect.bottom-=10;
	//m_grid.Create(rect, this, 900);

	DmxFname	="";   
	AcDbObject* pObj = NULL;
	/*  acdbOpenObject(pObj,m_eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,m_eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pdmx = DMX_ROAD::cast(pObj);
	m_NDmd=pdmx->NUM;
	//daicol=1;
	m_grid.SetRowCount(m_NDmd+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(3);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	//	   m_grid.SetFixedBkColor(fixbkclr);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;
	Item.lfFont=font;

	for(int k=0;k<m_grid.GetColumnCount();k++)
	{
		int i;
		for( i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}  
		m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}

	m_grid.SetItemText(0,0,L"序 号");
	m_grid.SetItemText(0,1,L"里 程");
	m_grid.SetItemText(0,2,L"高 程");

	int i=0;
	ACHAR tmp[10];
	if(m_NDmd !=0 )  	
	{
		for(i=0; i<m_NDmd; i++)
		{
			ACHAR Sml[80];
			/*		   if(pzLine->DLB[i].BLC>=1000.0)
			_stprintf(Sml,L"%s%0.3lf",pzLine->DLB[i].BGH,pzLine->DLB[i].BLC);
			else if(pzLine->DLB[i].BLC>=100.0)
			_stprintf(Sml,L"%s0%0.3lf",pzLine->DLB[i].BGH,pzLine->DLB[i].BLC);
			else if(pzLine->DLB[i].BLC>=10.0)
			_stprintf(Sml,L"%s00%0.3lf",pzLine->DLB[i].BGH,pzLine->DLB[i].BLC);
			else
			_stprintf(Sml,L"%s000%0.3lf",pzLine->DLB[i].BGH,pzLine->DLB[i].BLC);

			putplus(Sml, '+', '.', -3); */

			//               _stprintf(Sml,L"%0.3lf",pdmx->DMXys[i].lc);
			ACHAR kml[20];
			/*
			double xlc=pzLinep->XLC(pdmx->DMXys[i].lc,GH);
			_tcscpy(pdmx->DMXys[i].ckml,L"\0");   _tcscpy(pdmx->DMXys[i].ckml,pdmx->LCchr(GH,xlc,2));*/
			pdmx->DMXys[i].dml = pzLinep->XLC1(pdmx->DMXys[i].lc);
			_stprintf(kml,L"%0.3lf",pdmx->DMXys[i].dml);
			m_grid.SetItemText(i+1,1,kml);

			/*
			if(pzLine->DLB[i].BLC+pzLine->DLB[i].DL>=1000.0)
			_stprintf(Eml,L"%s%0.3lf",pzLine->DLB[i].EGH,pzLine->DLB[i].BLC+pzLine->DLB[i].DL);
			else if(pzLine->DLB[i].BLC+pzLine->DLB[i].DL>=100.0)
			_stprintf(Eml,L"%s0%0.3lf",pzLine->DLB[i].EGH,pzLine->DLB[i].BLC+pzLine->DLB[i].DL);
			else if(pzLine->DLB[i].BLC+pzLine->DLB[i].DL>=10.0)
			_stprintf(Eml,L"%s00%0.3lf",pzLine->DLB[i].EGH,pzLine->DLB[i].BLC+pzLine->DLB[i].DL);
			else
			_stprintf(Eml,L"%s000%0.3lf",pzLine->DLB[i].EGH,pzLine->DLB[i].BLC+pzLine->DLB[i].DL);

			putplus(Eml, '+', '.', -3);*/
			_stprintf(Sml,L"%0.3lf",pdmx->DMXys[i].level);
			m_grid.SetItemText(i+1,2,Sml); 
			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}
	}
	_stprintf(tmp,L"%d",i+1);
	m_grid.SetItemText(i+1,0,tmp);

	m_grid.SetColumnWidth(0,60);
	m_grid.SetColumnWidth(1,95);
	m_grid.SetColumnWidth(2,95);
	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);
	m_grid.AutoSize();

	pdmx->close();
	//AddSzControl(m_grid,mdResize,mdResize);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
