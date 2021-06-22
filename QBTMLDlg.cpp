// QBTMLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QBTMLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
/////////////////////////////////////////////////////////////////////////////
// QBTMLDlg dialog


QBTMLDlg::QBTMLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(QBTMLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(QBTMLDlg)
	m_mdbname = _T("");
	NFD = 0;
	FDML = NULL;
	//}}AFX_DATA_INIT
}


void QBTMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(QBTMLDlg)
	DDX_Control(pDX, IDC_COMBO_qbtroadname, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_mdbname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(QBTMLDlg, CDialog)
	//{{AFX_MSG_MAP(QBTMLDlg)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_qbtdel, OnBUTTONqbtdel)
	ON_BN_CLICKED(IDC_BUTTON_qbtInput, OnBUTTONqbtInput)
	ON_BN_CLICKED(IDC_BUTTON_qbtsave, OnBUTTONqbtsave)
	ON_BN_CLICKED(IDC_BUTTON_browserMdb, OnBUTTONbrowserMdb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// QBTMLDlg message handlers

void QBTMLDlg::OnButtonInsert() 
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

void QBTMLDlg::OnBUTTONqbtdel() 
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

void QBTMLDlg::OnBUTTONqbtInput() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//选择文件
	CString workdir,curpm;
	char str[256];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	//	if(SYorMDB==1)
	//	{	
	workdir += "\\data";   
	CString DefaultName = workdir + "\\ml.qbt";
	CFileDialog fname(true,NULL,DefaultName);
	fname.m_ofn.lpstrDefExt = L"qbt";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"里程分段文件(*.qbt)\0*.qbt\0\0";
	CString mlfname;
	if(fname.DoModal()==IDOK)
	{
		mlfname =fname.GetPathName();
		UpdateData();

		FILE *fpw;
		fpw = _wfopen(mlfname,L"r");
		if(fpw==NULL)
		{
			ads_alert(L"打开文件失败!");
			return;
		}

		int nfd;
		double *fdml=NULL;
		ACHAR tmp[256];
		fwscanf(fpw,L"%d",&nfd);
		if(nfd>0)
		{
			fdml = new double[nfd*2];
			for(int i=0; i<nfd; i++)
				fwscanf(fpw,L"%lf%lf",&fdml[i*2],&fdml[i*2+1]);

			m_grid.SetRowCount(nfd+2);
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
			m_grid.SetItemText(0,1,L"起始里程");
			m_grid.SetItemText(0,2,L"终止里程");
			int i;
			for(i=0; i<nfd; i++)
			{
				_stprintf(tmp,L"%d",i+1);
				m_grid.SetItemText(i+1,0,tmp);

				acdbRToS(fdml[i*2],2,3,tmp);
				m_grid.SetItemText(i+1,1,tmp);

				acdbRToS(fdml[i*2+1],2,3,tmp);
				m_grid.SetItemText(i+1,2,tmp);
			}
			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
			if(fdml) delete []fdml;

		}
		else
		{
			m_grid.SetRowCount(nfd+2);
			m_grid.SetItemText(1,0,L"1");
		}


		fclose(fpw);
	}
	m_grid.UpdateData(false);
}

void QBTMLDlg::OnBUTTONqbtsave() 
{
	// TODO: Add your control notification handler code here
	//选择文件
	CString workdir,curpm;
	char str[256];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	//	if(SYorMDB==1)
	//	{	
	workdir += "\\data";   
	CString DefaultName = workdir + "\\ml.qbt";
	CFileDialog fname(true,NULL,DefaultName);
	fname.m_ofn.lpstrDefExt = L"qbt";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"里程分段文件(*.qbt)\0*.qbt\0\0";
	CString mlfname;
	if(fname.DoModal()==IDOK)
	{
		mlfname =fname.GetPathName();
		UpdateData();

		FILE *fpw;
		fpw = _wfopen(mlfname,L"w");
		if(fpw==NULL)
		{
			ads_alert(L"打开文件失败!");
			return;
		}

		int nfd;
		double *fdml=NULL;
		nfd =m_grid.GetRowCount()-2;
		fwprintf(fpw,L"%d\n",nfd);
		if(nfd>0)
		{
			fdml = new double[(nfd+1)*2];

			if(fdml==NULL)
			{
				ads_printf(L"分配空间失败!\n");
				return;
			}
			for(int i=1; i<=nfd; i++)
			{
				fdml[i*2] = _wtof(m_grid.GetItemText(i, 1));
				fdml[i*2+1] = _wtof(m_grid.GetItemText(i, 2));
				fwprintf(fpw,L"%lf %lf\n",fdml[i*2],fdml[i*2+1]);
			}
		}
		fclose(fpw);
	}
	CString mes;
	mes.Format(L"清表土里程分段数据已保存在%s文件中!",mlfname);
	ads_alert(mes);
}

void QBTMLDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	//	UpdateData(true);

	int iroad=m_RoadName.GetCurSel();
	RoadName = mdb.Road[iroad].RoadName;


	NFD =m_grid.GetRowCount()-2;

	if(NFD>0)
	{
		if(FDML) delete []FDML;
		FDML =new double[NFD*2];
		if(FDML==NULL)
		{
			ads_printf(L"分配空间失败!\n");
			return;
		}
		for(int i=1; i<=NFD; i++)
		{
			FDML[(i-1)*2] = _wtof(m_grid.GetItemText(i, 1));
			FDML[(i-1)*2+1] = _wtof(m_grid.GetItemText(i, 2));
		}

	}
}

void QBTMLDlg::OnBUTTONbrowserMdb() 
{
	// TODO: Add your control notification handler code here

	// TODO: Add your control notification handler code here
	CString workdir,curpm;
	char str[256];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	//	if(SYorMDB==1)
	//	{	
	workdir += "\\data";   
	CString DefaultName = m_mdbname;
	CFileDialog fname(true,NULL,DefaultName);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	if(fname.DoModal()==IDOK)
		m_mdbname =fname.GetPathName();
	mdb.m_RoadSum=0;
	mdb.ReadAllRoadName(m_mdbname);//读取线路数据库中断链表的数据
	//对所有道路循环，读取地面线，
	if(mdb.m_RoadSum<1)
	{
		ads_printf(L"数据库中没有有效的道路名!\n");
		return ;
	}
	m_RoadName.ResetContent();
	for(int i=0; i<mdb.m_RoadSum; i++)
		m_RoadName.AddString(mdb.Road[i].RoadName);
	m_RoadName.SetCurSel(0);
	//	m_Sdml = mdb.Road[0].BLC;
	//	m_Edml = mdb.Road[0].ELC;
	UpdateData(false);

}

BOOL QBTMLDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString workdir,curpm,Path;
	char str[256];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	workdir += "\\data\\";   
	m_mdbname = workdir + Pro + ".mdb";


	mdb.ReadAllRoadName(m_mdbname);//读取线路数据库中断链表的数据
	//对所有道路循环，读取地面线，
	if(mdb.m_RoadSum<1)
	{
		ads_printf(L"数据库中没有有效的道路名!\n");
		return 0;
	}

	m_RoadName.ResetContent();
	for(int i=0; i<mdb.m_RoadSum; i++)
	{
		m_RoadName.AddString(mdb.Road[i].RoadName);
	}
	m_RoadName.SetCurSel(0);


	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=90;
	rect.right-=100;
	rect.bottom-=10;
	m_grid.Create(rect, this, 900);
	//设置m_grid
	m_grid.SetRowCount(3);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(3);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);


	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;
	Item.lfFont=font;
	int i;
	for(int k=0;k<m_grid.GetColumnCount();k++)
	{
		for(i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}  
		m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}

	m_grid.SetItemText(0,0,L"序 号");
	m_grid.SetItemText(0,1,L"起始里程");
	m_grid.SetItemText(0,2,L"终止里程");

	m_grid.SetItemText(1,0,L"1");
	double Sdml,Edml;
	Sdml = mdb.Road[0].BLC;
	Edml = mdb.Road[0].ELC;
	ACHAR tmp[256];
	acdbRToS(Sdml,2,3,tmp);
	m_grid.SetItemText(1,1,tmp);
	acdbRToS(Edml,2,3,tmp);
	m_grid.SetItemText(1,2,tmp);

	m_grid.SetColumnWidth(0,50);
	m_grid.SetColumnWidth(1,92);
	m_grid.SetColumnWidth(2,92);
	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	//	   m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
