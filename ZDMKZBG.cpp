// ZDMKZBG.cpp : implementation file
//

#include "stdafx.h"
#include "ZDMKZBG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ZDMKZBG dialog
int iSelZdm=-1;
extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
ZDMKZBG::ZDMKZBG(CWnd* pParent /*=NULL*/)
	: CDialog(ZDMKZBG::IDD, pParent)
{
	//{{AFX_DATA_INIT(ZDMKZBG)
	m_gzxs = FALSE;
	//}}AFX_DATA_INIT
	m_grid.mode = 2;
	pm = NULL;
}

void ZDMKZBG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ZDMKZBG)
	DDX_Check(pDX, IDC_CHECK_GZXS, m_gzxs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ZDMKZBG, CDialog)
	//{{AFX_MSG_MAP(ZDMKZBG)
	ON_BN_CLICKED(IDC_BUTTON_PICKZDM, OnButtonPickzdm)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, OnButtonDraw)
	ON_BN_CLICKED(IDC_CHECK_GZXS, OnCheckGzxs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZDMKZBG message handlers

BOOL ZDMKZBG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=40;
	rect.right-=10;
	rect.bottom-=45;
	m_grid.Create(rect, this, 900);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ZDMKZBG::OnButtonPickzdm() 
{
	// TODO: Add your control notification handler code here
	AcGePoint3d PT;
	ads_point resPt;

	//	GetParent()->EnableWindow(TRUE);
	acedGetAcadDwgView()->SetFocus();
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.
	int izdm,i;
	CString mes;
	ACHAR tmp[30];

	iSelZdm = -1;
	if(acedGetPoint(NULL,L"请在纵面图内拾取一点!\n",resPt)==RTNORM)
	{
		PT.x = resPt[X];
		PT.y = resPt[Y];
		izdm = GlobalZDMTK.FindWichZdm(PT);
		if(izdm>=0)
		{
			X0 = GlobalZDMTK.ZDMTK[izdm].X0;
			Y0 = GlobalZDMTK.ZDMTK[izdm].Y0;
			H0 = GlobalZDMTK.ZDMTK[izdm].H0;
			iSelZdm = izdm;
			mdbname = GlobalZDMTK.ZDMTK[izdm].mdbname;
			m_RoadName = GlobalZDMTK.ZDMTK[izdm].RXDname;
			pm = GlobalZDMTK.ZDMTK[izdm].pm;
			K0 = pm->DLArray[0].ELC;
			DBS.Read_XLDbs(mdbname,L"控制标高",m_RoadName);
			m_grid.SetRowCount(DBS.CRNUM+2);
			m_grid.SetFixedRowCount(1);
			m_grid.SetColumnCount(4);
			m_grid.SetFixedColumnCount(1);
			m_grid.SetTextBkColor(txtbkclr);
			//	   m_grid.SetFixedBkColor(fixbkclr);

			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
				DT_END_ELLIPSIS|DT_WORDBREAK;
			Item.lfFont=font;
			for(int k=0;k<m_grid.GetColumnCount();k++)
			{
				for(int i=0;i<m_grid.GetRowCount();i++) 
				{
					Item.row = i;
					Item.col = k;
					m_grid.SetItem(&Item);
				}  
				m_grid.SetItemState(i-1, k,GVIS_READONLY);
			}

			m_grid.SetItemText(0,0,L"序号");
			m_grid.SetItemText(0,1,L"里程");
			m_grid.SetItemText(0,2,L"控制标高");
			m_grid.SetItemText(0,3,L"属性");

			for(i=0;i<DBS.CRNUM;i++)
			{
				_stprintf(tmp,L"%0.3lf",DBS.pCtrlHt[i].dml);
				m_grid.SetItemText(i+1,1,tmp);
				_stprintf(tmp,L"%0.3lf",DBS.pCtrlHt[i].Ht);
				m_grid.SetItemText(i+1,2,tmp);
				_tcscpy(tmp,DBS.pCtrlHt[i].Att);
				m_grid.SetItemText(i+1,3,tmp);
				_stprintf(tmp,L"%d",i+1);
				m_grid.SetItemText(i+1,0,tmp);
			}

			m_grid.SetColumnWidth(0,40);
			m_grid.SetColumnWidth(1,90);
			m_grid.SetColumnWidth(2,90);
			m_grid.SetColumnWidth(3,90);
			for( i=0;i<m_grid.GetRowCount();i++)
				m_grid.SetRowHeight(i, DEFAULTCOLHEI);
			//	   m_grid.ResetScrollBars();
			m_grid.SetFocusCell(1,1);
		}
		else
			ads_alert(L"拾取失败!");
	}   
	UpdateData(false);
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/	
}

void ZDMKZBG::OnBUTTONAdd() 
{
	// TODO: Add your control notification handler code here
	m_gzxs=FALSE;
	UpdateData(FALSE);
	int i;
	CString tr;
	if(m_grid.GetRowCount()==1)          // 新建表格里没有行时，加2行
	{
		m_grid.SetRowCount(1); 
		for(i=0;i<m_grid.GetColumnCount();i++)
		{
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
			m_grid.SetItemText(1,i,L" ");
		}

		m_grid.SetFocusCell(0,1);
		m_grid.Invalidate();
	}
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		Item.row=nRow;
		m_grid.InsertRow(_T("NEW"), nRow);
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
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
		m_grid.SetItemText(0,0,tr);
		for( i=1;i<m_grid.GetColumnCount();i++) 
		{
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
			m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
			m_grid.SetItemText(nRow+1,i,L" ");
		}
		m_grid.SetFocusCell(nRow+1,1);
		m_grid.Invalidate();
	}
	Item.col=0;
	Item.row=0;
	Item.strText="行号";
	m_grid.SetItem(&Item);
	Invalidate();	
}

void ZDMKZBG::OnBUTTONdel() 
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
	Invalidate();			
}

void ZDMKZBG::OnButtonDraw() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString Att;
	double Ht,dml;
	AcGePoint3d cpt;
	ACHAR Str[20];
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	for(int i=1; i<m_grid.GetRowCount(); i++)
	{
		int flag;
		m_grid.RowIsEpt(i,m_grid.GetColumnCount(),flag);		   
		if(flag)//不空
		{
			Att=m_grid.GetItemText(i, 3);
			Ht=_wtof(m_grid.GetItemText(i, 2));
			dml=_wtof(m_grid.GetItemText(i, 1));
			dml = pm->TYLC(dml);
			cpt.x = (dml-K0)/pORI_SCALE.HBVSCALE + X0;
			cpt.y = Ht-H0 + Y0;
			_stprintf(Str,L"%.3lf",Ht);	
			if(Att=="上限标高")
				NoteBG(cpt,2.0,Str,6,2,4,2);
			else if(Att=="下限标高")
				NoteBG(cpt,2.0,Str,6,2,4,2,NULL,0);
			else
				NoteBG(cpt,2.0,Str,6,2,4,2,NULL,-1);
		}
		else
			break;
	}//统计非空行数
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus(); // Reset the focus back to ourselves
}

void ZDMKZBG::OnOK() 
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
	DBS.CRNUM = NRow;//m_grid.GetRowCount()-2;  
	if(DBS.pCtrlHt) {delete []DBS.pCtrlHt,DBS.pCtrlHt=NULL;}
	if(DBS.CRNUM>0)
	{
		DBS.pCtrlHt = new XLDataBase::CTRLHT[DBS.CRNUM];
		for(i=1; i<=DBS.CRNUM;i++)
		{
			DBS.pCtrlHt[i-1].Att=m_grid.GetItemText(i, 3);
			DBS.pCtrlHt[i-1].Ht=_wtof(m_grid.GetItemText(i, 2));
			DBS.pCtrlHt[i-1].dml=_wtof(m_grid.GetItemText(i, 1));	
		}

		DBS.Write_XLDbs(mdbname,L"控制标高",m_RoadName);
		ACHAR mes[256];
		_stprintf(mes,L"控制标高数据已写入%s!",mdbname);
		ads_alert(mes);	   
	}	
	//	CDialog::OnOK();
}

void ZDMKZBG::OnCheckGzxs() 
{
	// TODO: Add your control notification handler code here
	m_gzxs = !m_gzxs;
	m_grid.Gzxs=m_gzxs;		
}
