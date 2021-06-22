// SetJKNorm.cpp : implementation file
//

#include "stdafx.h"
#include "SetJKNorm.h"
#include "xldatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetJKNorm dialog

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
CSetJKNorm::CSetJKNorm(CWnd* pParent /*=NULL*/)
	: CDialog(CSetJKNorm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetJKNorm)
	m_GLDJ = _T("高速");
	m_DV = 100;
	mdbname = "";
	//}}AFX_DATA_INIT
}


void CSetJKNorm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetJKNorm)
	DDX_Control(pDX, IDC_COMBOJKLB, m_JKLBCrl);
	DDX_Text(pDX, IDC_EDITJKGLDJ, m_GLDJ);
	DDX_Text(pDX, IDC_EDITJKDV, m_DV);
	DDX_CBString(pDX, IDC_COMBOJKLB, m_JKLB);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LISTJK, m_grid);
}


BEGIN_MESSAGE_MAP(CSetJKNorm, CDialog)
	//{{AFX_MSG_MAP(CSetJKNorm)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	ON_BN_CLICKED(IDC_RADIO_JKMDB, OnRadioJkmdb)
	ON_BN_CLICKED(IDC_RADIO_JKNORM, OnRadioJknorm)
	ON_CBN_SELCHANGE(IDC_COMBOJKLB, OnSelchangeCombojklb)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_BuSheJiaKuan, &CSetJKNorm::OnBnClickedRadioBushejiakuan)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetJKNorm message handlers

BOOL CSetJKNorm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;

	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=110;
	rect.right-=10;
	rect.bottom-=40;
	//m_grid.Create(rect, this, 900);

	// TODO: Add extra initialization here
	//读当前方案数据库
	BAS_DRAW_FUN ob;
	if (!ob.ReadWorkdir())
	{
		ads_alert(L"请先设定项目名称,否则数据将无法保存!");
		return false;
	}

	if(pzLinep)
		mdbname = pzLinep->mdbname;

	if(mdbname=="")
	{
		mdbname = Cworkdir; 
		mdbname += "\\data\\" ;
		mdbname += Pro;
		mdbname += ".mdb";//当前方案
	}
	mdbname = Cworkdir;
	mdbname += "\\data\\";
	mdbname += Pro;
	mdbname += ".mdb";//当前方案

	//若存在超高计算标准，则读取
	bool IfSetByNorm=true;

	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(mdbname,L"线路属性");
	m_DV = xlmdb.RoadXZ.DV;
	m_GLDJ = xlmdb.RoadXZ.Grade;
	if(m_GLDJ=="匝道")
	{
		m_JKLBCrl.AddString(L"按单车道加宽");
		m_JKLBCrl.AddString(L"按双车道加宽");
	}
	else
	{
		m_JKLBCrl.AddString(L"一类加宽");
		m_JKLBCrl.AddString(L"二类加宽");
		m_JKLBCrl.AddString(L"三类加宽");
	}
	xlmdb.Read_XLDbs(mdbname,L"加宽计算标准");
	if(xlmdb.NJK>1)//数据库中有超高数据
	{
		//从数据库中读取
		CButton *pbtn;
		pbtn = (CButton *)GetDlgItem(IDC_RADIO_JKMDB);
		pbtn->SetCheck(1);
		MdborNorm  = 0;
		pbtn = (CButton *)GetDlgItem(IDC_COMBOJKLB);
		pbtn->EnableWindow(false);
		NJK = xlmdb.NJK-1;

		if(m_GLDJ!="匝道")
		{
			if(xlmdb.JKData[xlmdb.NJK-1][0]<1.1)//数据库中最后一行记录加宽类别
				m_JKLB  =  "一类加宽";
			else if(xlmdb.JKData[xlmdb.NJK-1][0]<2.1)
				m_JKLB  =  "二类加宽";
			else
				m_JKLB  =  "三类加宽";

			for(int i=0; i<NJK; i++)
			{
				JKData[i][0] = xlmdb.JKData[i][0];
				JKData[i][1] = xlmdb.JKData[i][1];
			}	   
		}
		else
		{
			if(xlmdb.JKData[xlmdb.NJK-1][0]<1.1)//数据库中最后一行记录加宽类别
				m_JKLB  =  "按单车道加宽";
			else if(xlmdb.JKData[xlmdb.NJK-1][0]<2.1)
				m_JKLB  =  "按双车道加宽";
			for(int i=0; i<NJK; i++)
			{
				JKData[i][0] = xlmdb.JKData[i][0];
				JKData[i][1] = xlmdb.JKData[i][1];
			}	   
		}
	}
	else//依据规范设置超高标准
	{
		CButton *pbtn;
		pbtn = (CButton *)GetDlgItem(IDC_RADIO_JKNORM);
		pbtn->SetCheck(1);
		pbtn = (CButton *)GetDlgItem(IDC_COMBOJKLB);
		pbtn->EnableWindow(true);
		MdborNorm = 1;
		if(m_GLDJ!="匝道")
		{
			if(m_GLDJ=="四级"||(m_GLDJ=="三级"&&m_DV < 31))
				m_JKLB = "一类加宽";
			else
				m_JKLB = "三类加宽";
			FindJK(m_JKLB,NJK,JKData);	
		}
		else
		{
			m_JKLB = "按单车道加宽";
			FindJK(m_JKLB,NJK,JKData);	
		}

	}
	//设置超高值
	ACHAR  tmp[80];
	int i;

	txtbkclr=RGB(0XFF, 0XFF,0XE0);
	fixbkclr=RGB(155,188,0);

	m_grid.SetRowCount(NJK+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(3);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetFixedBkColor(fixbkclr);

	Item.nFormat = DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_WORDBREAK | DT_SINGLELINE;;
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
	m_grid.SetItemText(0,2,L"曲线加宽");

	i=0;
	if(NJK>0)
	{
		for(i=0; i<NJK; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(JKData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos(JKData[i][1],2,3,tmp);
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

void CSetJKNorm::OnBUTTONAdd() 
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

void CSetJKNorm::OnBUTTONdel() 
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

void CSetJKNorm::OnOK() 
{
	// TODO: Add extra validation here
	ACHAR tmp[256];
	UpdateData(true);
	NJK = m_grid.GetRowCount() - 2;  	
	int i;
	for( i=1; i<=NJK; i++)
	{
		_tcscpy(tmp, m_grid.GetItemText(i, 1));
		JKData[i-1][0] = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 2));
		JKData[i-1][1] = _wtof(tmp);		
	}
	//写数据库
	XLDataBase xlmdb;
	xlmdb.NJK = NJK+1;
	for(i=0; i< NJK; i++)
	{
		xlmdb.JKData[i][0] = JKData[i][0];
		xlmdb.JKData[i][1] = JKData[i][1];				
	}

	if(m_GLDJ != "匝道")
	{
		if(m_JKLB  ==  "一类加宽")
			xlmdb.JKData[NJK][0] = 1;
		else if(m_JKLB  ==  "二类加宽")
			xlmdb.JKData[NJK][0] = 2;
		else
			xlmdb.JKData[NJK][0] = 3;
	}
	else
	{
		if(m_JKLB  ==  "按单车道加宽")
			xlmdb.JKData[NJK][0] = 1;
		else if(m_JKLB  ==  "按双车道加宽")
			xlmdb.JKData[NJK][0] = 2;
	}

	xlmdb.JKData[NJK][1] = 0;
	xlmdb.Write_XLDbs(mdbname,L"加宽计算标准",L"");	
	CDialog::OnOK();
}

void CSetJKNorm::OnRadioJkmdb() 
{
	// TODO: Add your control notification handler code here
	XLDataBase xlmdb;
	ACHAR  tmp[80];
	int i;
	MdborNorm =0;
	xlmdb.Read_XLDbs(mdbname,L"线路属性");
	m_DV = xlmdb.RoadXZ.DV;
	m_GLDJ = xlmdb.RoadXZ.Grade;
	xlmdb.Read_XLDbs(mdbname,L"加宽计算标准");
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_COMBOJKLB);
	pbtn->EnableWindow(false);
	if(xlmdb.NJK>1)//数据库中有超高数据
	{
		//从数据库中读取
		NJK = xlmdb.NJK-1;
		if(m_GLDJ != "匝道")
		{
			if(xlmdb.JKData[xlmdb.NJK-1][0]<1.1)//数据库中最后一行记录加宽类别
				m_JKLB  =  "一类加宽";
			else if(xlmdb.JKData[xlmdb.NJK-1][0]<2.1)
				m_JKLB  =  "二类加宽";
			else
				m_JKLB  =  "三类加宽";
		}
		else
		{
			if(xlmdb.JKData[xlmdb.NJK-1][0]<1.1)//数据库中最后一行记录加宽类别
				m_JKLB  =  "按单车道加宽";
			else if(xlmdb.JKData[xlmdb.NJK-1][0]<2.1)
				m_JKLB  =  "按双车道加宽";
		}


		for(int i=0; i<NJK; i++)
		{
			JKData[i][0] = xlmdb.JKData[i][0];
			JKData[i][1] = xlmdb.JKData[i][1];
		}	   
	}	
	else
		NJK = 0;

	m_grid.SetRowCount(NJK+2);
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
	m_grid.SetItemText(0,2,L"曲线加宽");

	i=0;
	if(NJK>0)
	{
		for(i=0; i<NJK; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(JKData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos(JKData[i][1],2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);
		}     
	}
	_stprintf(tmp ,L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.AutoSize();
	UpdateData(false);
	m_grid.Invalidate();	
}

void CSetJKNorm::OnRadioJknorm() 
{
	// TODO: Add your control notification handler code here
	int i;
	ACHAR tmp[80];

	UpdateData();
	MdborNorm =1;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_COMBOJKLB);
	pbtn->EnableWindow(true);

	if(m_GLDJ=="匝道" && (m_JKLB!="按单车道加宽" && m_JKLB!="按双车道加宽"))
		m_JKLB="按单车道加宽";
	FindJK(m_JKLB,NJK,JKData);	
	m_grid.SetRowCount(NJK+2);
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
	m_grid.SetItemText(0,2,L"曲线加宽");

	i=0;
	if(NJK>0)
	{
		for(i=0; i<NJK; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(JKData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos(JKData[i][1],2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);
		}     
	}
	_stprintf(tmp ,L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.AutoSize();
	UpdateData(false);
	m_grid.Invalidate();	
}


void CSetJKNorm::OnSelchangeCombojklb() 
{
	// TODO: Add your control notification handler code here
	ACHAR tmp[80];
	int i = m_JKLBCrl.GetCurSel();   
	m_JKLBCrl.GetLBText(i,m_JKLB);

	FindJK(m_JKLB,NJK,JKData);	
	m_grid.SetRowCount(NJK+2);
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
	m_grid.SetItemText(0,2,L"曲线加宽");

	i=0;
	if(NJK>0)
	{
		for(i=0; i<NJK; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(JKData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos(JKData[i][1],2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);
		}     
	}
	_stprintf(tmp ,L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.AutoSize();
	UpdateData(false);
	m_grid.Invalidate();	
}


void CSetJKNorm::OnBnClickedRadioBushejiakuan()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	ACHAR tmp[80];

	UpdateData();
	MdborNorm =1;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_BuSheJiaKuan);
	pbtn->EnableWindow(true);

	if(m_GLDJ=="匝道" && (m_JKLB!="按单车道加宽" && m_JKLB!="按双车道加宽"))
		m_JKLB="按单车道加宽";
	FindJK(m_JKLB,NJK,JKData);	
	m_grid.SetRowCount(NJK+2);
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
	m_grid.SetItemText(0,2,L"曲线加宽");

	i=0;
	if(NJK>0)
	{
		for(i=0; i<NJK; i++)
		{
			_stprintf(tmp ,L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(JKData[i][0],2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			//ads_rtos(JKData[i][1],2,3,tmp);
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
