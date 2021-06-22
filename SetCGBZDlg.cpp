// SetCGBZDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetCGBZDlg.h"
#include "xldatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
/////////////////////////////////////////////////////////////////////////////
// SetCGBZDlg dialog


SetCGBZDlg::SetCGBZDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetCGBZDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetCGBZDlg)

	m_DV = 100;
	m_MAXSuphI = _T("8");
	m_IG = _T("2");
	NSUPH = 0;
	mdbname = "";
	//}}AFX_DATA_INIT
}


void SetCGBZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetCGBZDlg)
	DDX_Text(pDX, IDC_EDIT_DV, m_DV);
	DDX_Text(pDX, IDC_EDIT_MAXSupI, m_MAXSuphI);
	DDX_Text(pDX, IDC_EDIT_IG, m_IG);
	//	DDX_GridControl(pDX,IDC_LIST_CGdata,m_grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_CalSupH, m_grid);
}


BEGIN_MESSAGE_MAP(SetCGBZDlg, CDialog)
	//{{AFX_MSG_MAP(SetCGBZDlg)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	ON_BN_CLICKED(IDC_RADIO_MDB, OnRadioMdb)
	ON_BN_CLICKED(IDC_RADIO_NORM, OnRadioNorm)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_BuSheChaoGao, &SetCGBZDlg::OnBnClickedRadioBushechaogao)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SetCGBZDlg message handlers

void SetCGBZDlg::OnBUTTONAdd() 
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
		m_grid.AutoSize();

		m_grid.Invalidate();
	}		
}

void SetCGBZDlg::OnBUTTONdel() 
{
	// TODO: Add your control notification handler code here
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

void SetCGBZDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	ACHAR tmp[256];
	UpdateData(true);
	NSUPH = m_grid.GetRowCount() - 2;  	
	int i;
	for( i=1; i<=NSUPH; i++)
	{
		_tcscpy(tmp, m_grid.GetItemText(i, 1));
		SuphData[i-1][0] = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 2));
		SuphData[i-1][1] = _wtof(tmp);		
	}
	//写数据库
	XLDataBase xlmdb;
	xlmdb.NuphData = NSUPH+1;
	for(i=0; i< NSUPH; i++)
	{
		xlmdb.SuphData[i][0] = SuphData[i][0];
		xlmdb.SuphData[i][1] = SuphData[i][1];				
	}
	xlmdb.SuphData[NSUPH][0] = _wtof(m_IG);
	xlmdb.SuphData[NSUPH][1] = _wtof(m_MAXSuphI);
	xlmdb.Write_XLDbs(mdbname,L"超高计算标准",L"");
}

BOOL SetCGBZDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	//读当前方案数据库
	BAS_DRAW_FUN ob;
	if (!ob.ReadWorkdir())
	{
		ads_alert(L"请先设定项目名称,否则数据将无法保存!");
		return false;
	}

	//if(pzLinep)
	//	mdbname = pzLinep->mdbname;

	//if(mdbname=="")
	//{
	//	mdbname = Cworkdir; 
	//	mdbname += "\\data\\" ;
	//	mdbname += Pro;
	//	mdbname += ".mdb";//当前方案
	//}
	mdbname = Cworkdir;
	mdbname += "\\data\\";
	mdbname += Pro;
	mdbname += ".mdb";//当前方案

	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(mdbname,L"线路属性");
	m_DV = xlmdb.RoadXZ.DV;

	xlmdb.Read_XLDbs(mdbname,L"超高计算标准");
	if (xlmdb.NuphData > 1)//数据库中有超高数据
	{
		//从数据库中读取
		//设置按钮
		CButton *pbtn;
		pbtn = (CButton *)GetDlgItem(IDC_RADIO_MDB);
		pbtn->SetCheck(1);
		//将数据库中的数值赋值给成员数组
		NSUPH = xlmdb.NuphData - 1;
		ACHAR str[20];
		ads_rtos(xlmdb.SuphData[xlmdb.NuphData - 1][0], 2, 3, str);//数据库中最后一行记录路拱横坡和最大超高
		m_IG  =  str;
		ads_rtos(xlmdb.SuphData[xlmdb.NuphData - 1][1], 2, 3, str);//数据库中最后一行记录路拱横坡和最大超高
		m_MAXSuphI =  str;
		for(int i=0; i<NSUPH; i++)
		{
			SuphData[i][0] = xlmdb.SuphData[i][0];
			SuphData[i][1] = xlmdb.SuphData[i][1];
		}	   
	}
	else//依据规范设置超高标准
	{
		CButton *pbtn;
		pbtn = (CButton *)GetDlgItem(IDC_RADIO_NORM);
		pbtn->SetCheck(1);
		if(xlmdb.RoadXZ.Grade!="匝道")
			FindSuphI(m_DV,_wtof(m_MAXSuphI),_wtof(m_IG),NSUPH,SuphData);	
		else
			FindZD_SuphI(m_DV,NSUPH,SuphData);

	}
	//设置超高值
	ACHAR  tmp[80];
	int i;

	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=105;
	rect.right-=10;
	rect.bottom-=35;
	//m_grid.Create(rect, this, 900);

	Item.lfFont = font;
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE |
		DT_END_ELLIPSIS | DT_WORDBREAK;

	txtbkclr = RGB(0XFF, 0XFF, 0XE0);
	fixbkclr = RGB(155, 188, 0);

	m_grid.SetRowCount(NSUPH+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(3);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetFixedBkColor(fixbkclr);

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
	m_grid.SetItemText(0,1,L"曲线半径");
	m_grid.SetItemText(0,2,L"超高横坡");

	i=0;
	if(NSUPH>0)
	{
		for(i=0; i<NSUPH; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(SuphData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos( SuphData[i][1],2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);
		}     
	}
	_stprintf(tmp ,L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);

	m_grid.SetColumnWidth(0,40);
	m_grid.SetColumnWidth(1,75);
	m_grid.SetColumnWidth(2,75);
	m_grid.SetRowHeight(0, DEFAULTCOLHEI);
	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);
	m_grid.AutoSize();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SetCGBZDlg::OnRadioMdb() 
{
	// TODO: Add your control notification handler code here
	XLDataBase xlmdb;
	ACHAR  tmp[80];
	int i;

	xlmdb.Read_XLDbs(mdbname,L"线路属性");
	m_DV = xlmdb.RoadXZ.DV;
	xlmdb.Read_XLDbs(mdbname,L"超高计算标准");
	if(xlmdb.NuphData>1)//数据库中有超高数据
	{
		//从数据库中读取
		CButton *pbtn;
		pbtn = (CButton *)GetDlgItem(IDC_RADIO_MDB);
		pbtn->SetCheck(1);
		NSUPH = xlmdb.NuphData-1;
		ACHAR str[20];
		ads_rtos(xlmdb.SuphData[xlmdb.NuphData-1][0],2,3,str);//数据库中最后一行记录路拱横坡和最大超高
		m_IG  =  str;
		ads_rtos(xlmdb.SuphData[xlmdb.NuphData-1][1],2,3,str);//数据库中最后一行记录路拱横坡和最大超高
		m_MAXSuphI =  str;
		for(int i=0; i<NSUPH; i++)
		{
			SuphData[i][0] = xlmdb.SuphData[i][0];
			SuphData[i][1] = xlmdb.SuphData[i][1];
		}	   
	}	
	else
		NSUPH = 0;

	m_grid.SetRowCount(NSUPH+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(3);
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
	m_grid.SetItemText(0,1,L"曲线半径");
	m_grid.SetItemText(0,2,L"超高横坡");

	i=0;
	if(NSUPH>0)
	{
		for(i=0; i<NSUPH; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(SuphData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos( SuphData[i][1],2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);
		}     
	}
	_stprintf(tmp ,L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.AutoSize();
	UpdateData(false);
	m_grid.Invalidate();
}

void SetCGBZDlg::OnRadioNorm() 
{
	// TODO: Add your control notification handler code here
	ACHAR  tmp[80];
	int i;
	UpdateData(true);
	// TODO: Add your control notification handler code here
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(mdbname,L"线路属性");

	if(xlmdb.RoadXZ.Grade!="匝道")
		FindSuphI(m_DV,_wtof(m_MAXSuphI),_wtof(m_IG),NSUPH,SuphData);	
	else
		FindZD_SuphI(m_DV,NSUPH,SuphData);	

	m_grid.SetRowCount(NSUPH+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(3);
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
	m_grid.SetItemText(0,1,L"曲线半径");
	m_grid.SetItemText(0,2,L"超高横坡");

	i=0;
	if(NSUPH>0)
	{
		for(i=0; i<NSUPH; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(SuphData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos( SuphData[i][1],2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);
		}     
	}
	_stprintf(tmp ,L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.AutoSize();
	UpdateData(false);
	m_grid.Invalidate();
}


void SetCGBZDlg::OnBnClickedRadioBushechaogao()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	ACHAR  tmp[80];
	UpdateData(true);

	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_BuSheChaoGao);
	pbtn->SetCheck(1);

	m_grid.SetRowCount(NSUPH+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(3);
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
	m_grid.SetItemText(0,1,L"曲线半径");
	m_grid.SetItemText(0,2,L"超高横坡");

	i=0;
	if(NSUPH>0)
	{
		for(i=0; i<NSUPH; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(SuphData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			//ads_rtos( SuphData[i][1],2,3,tmp);
			_tcscpy(tmp,L"0");
			m_grid.SetItemText(i+1,2,tmp);
		}     
	}
	_stprintf(tmp ,L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.AutoSize();
	UpdateData(false);
	m_grid.Invalidate();
}
