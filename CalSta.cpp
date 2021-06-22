// CalSta.cpp : implementation file
//

#include "stdafx.h"
#include "CalSta.h"
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
// CCalSta dialog

IMPLEMENT_DYNCREATE(CCalSta, CPropertyPage)
	CCalSta::CCalSta(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CCalSta::IDD ,1)
{
	//{{AFX_DATA_INIT(CCalSta)
	m_IfBri = true;
	m_IfQX = true;
	m_IfTun = true;
	m_JG = 20.0;
	pm = NULL;
	NSTA = 0;
	//}}AFX_DATA_INIT
}


void CCalSta::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalSta)
	DDX_Check(pDX, IDC_CHECK_IfBri, m_IfBri);
	DDX_Check(pDX, IDC_CHECK_IfQX, m_IfQX);
	DDX_Check(pDX, IDC_CHECK_IfTun, m_IfTun);
	DDX_Text(pDX, IDC_EDIT_JG, m_JG);
	//	DDX_GridControl(pDX,IDC_LIST_Sta,m_grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_Sta, m_grid);
}


BEGIN_MESSAGE_MAP(CCalSta, CPropertyPage)
	//{{AFX_MSG_MAP(CCalSta)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	ON_BN_CLICKED(IDC_BUTTON_CAL, OnButtonCal)
	//	ON_BN_CLICKED(IDC_BUTTON_adjust, OnBUTTONadjust)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalSta message handlers

void CCalSta::OnOK() //取出数据
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();

	ACHAR tmp[30];
	CString tmp1;
	UpdateData();
	NSTA =m_grid.GetRowCount()-2;
	if(NSTA>0)
	{
		LCBdata sta;
		lcb.RemoveAll();
		for(int i=1; i<=NSTA; i++)
		{
			_tcscpy(tmp,m_grid.GetItemText(i, 1));
			sta.lc = pm->TYLC(_wtof(tmp));//写统一里程
			//	sta.lc = _wtof(tmp);//写现场里程
			_tcscpy(tmp,m_grid.GetItemText(i, 2));
			sta.gldmlc = _wtof(tmp);
			sta.glRoadName=m_grid.GetItemText(i, 3);
			lcb.Add(sta);			
		}
	}
}

void CCalSta::OnBUTTONAdd() 
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
		m_grid.Invalidate();
	}		
}

void CCalSta::OnBUTTONdel() 
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

BOOL CCalSta::OnInitDialog() 
{
	// TODO: Add extra initialization here
	//	   OnInitDialog(0);
	CRect rect;
	GetClientRect(rect);
	rect.left+=12;
	rect.top+=150;
	rect.right-=10;
	rect.bottom-=10;
	//m_grid.Create(rect, this, 900);

	m_grid.SetRowCount(NSTA+2);
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
	m_grid.SetItemText(0,1,L"桩 号");
	m_grid.SetItemText(0,2,L"关联桩号");
	m_grid.SetItemText(0,3,L"关联道路");

	int i=0;
	ACHAR tmp[30];
	CString GH;
	if(NSTA> 0 )  	
	{
		for(i=0; i<NSTA; i++)
		{
			acdbRToS(pm->XLC(lcb[i].lc,GH),2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			acdbRToS(lcb[i].gldmlc,2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);

			m_grid.SetItemText(i+1,3,lcb[i].glRoadName);
			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}
		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);
		m_grid.SetColumnWidth(0,50);
		m_grid.ResetScrollBars();
		m_grid.SetFocusCell(1,1);
		m_grid.Invalidate();

	}
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSEreturn TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCalSta::OnButtonCal() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	m_grid.DeleteAllItems();

	pm->FormLCB(m_JG,pm->DLArray[0].ELC,pm->XLLength);

	if(m_IfQX)
		pm->AddTZDtoLCB();
	if(m_IfBri)
		pm->AddBritoLCB(NBRI,BriAry);
	if(m_IfTun)
	{
		pm->AddTuntoLCB(NTUN,TunAry);
	}
	if(NGLSTA>0)
		pm->AddBritoLCB(NGLSTA,GLSTA);

	NSTA = pm->pLCB.GetSize();

	m_grid.SetRowCount(NSTA+2);
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
	m_grid.SetItemText(0,1,L"桩 号");
	m_grid.SetItemText(0,2,L"性质");
	m_grid.SetItemText(0,3,L"备注");
	//m_grid.SetItemText(0,2,L"关联桩号");
	//m_grid.SetItemText(0,3,L"关联道路");

	int i=0;
	ACHAR tmp[30];
	CString GH;
	if(NSTA> 0 )  	
	{
		lcb.RemoveAll();
		for(i=0; i<NSTA; i++)
		{
			pm->pLCB[i].gldmlc=-1;
			pm->pLCB[i].glRoadName="";

			lcb.Add(pm->pLCB[i]);
			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);

			acdbRToS(pm->XLC(lcb[i].lc,GH),2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

		}
	}
	_stprintf(tmp,L"%d",i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.SetColumnWidth(0,40);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

	UpdateData(false);

}

void CCalSta::DMXZtoStr(int BTxz,CString &tmp)
{
	if(BTxz==0)
		tmp = "一般断面";
	if(BTxz==-1)
		tmp = "左桥起始桩";
	if(BTxz==-2)
		tmp = "右桥起始桩";
	if(BTxz==-3)
		tmp = "全桥起始桩";
	if(BTxz==-4)
		tmp = "左隧起始桩";
	if(BTxz==-5)
		tmp = "右隧起始桩";
	if(BTxz==-6)
		tmp = "全隧起始桩";

	if(BTxz==1)
		tmp = "左桥终止桩";
	if(BTxz==2)
		tmp = "右桥终止桩";
	if(BTxz==3)
		tmp = "全桥终止桩";
	if(BTxz==4)
		tmp = "左隧终止桩";
	if(BTxz==5)
		tmp = "右隧终止桩";
	if(BTxz==6)
		tmp = "全隧终止桩";
}

void CCalSta::StrtoDMXZ(CString tmp,int &BTxz)
{
	if(tmp == "一般断面")
		BTxz=0;
	if(tmp == "左桥起始桩")
		BTxz=-1;
	if(tmp == "右桥起始桩")
		BTxz=-2;
	if(tmp == "全桥起始桩")
		BTxz=-3;
	if(tmp == "左隧起始桩")
		BTxz=-4;
	if(tmp == "右隧起始桩")
		BTxz=-5;
	if(tmp == "全隧起始桩")
		BTxz=-6;
	if(tmp == "左桥终止桩")
		BTxz=1;
	if(tmp == "右桥终止桩")
		BTxz=2;
	if(tmp == "全桥终止桩")
		BTxz=3;
	if(tmp == "左隧终止桩")
		BTxz=4;
	if(tmp == "右隧终止桩")
		BTxz=5;
	if(tmp == "全隧终止桩")
		BTxz=6;
}

/*
#include "StaDataSheet.h"
void CCalSta::OnBUTTONadjust() 
{
// TODO: Add your control notification handler code here
((StaDataSheet *)GetParent())->Adjust();

m_grid.DeleteAllItems();
m_grid.SetRowCount(NSTA+2);
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

m_grid.SetItemText(0,0,L"序 号");
m_grid.SetItemText(0,1,L"桩 号");
//m_grid.SetItemText(0,2,L"关联桩号");
//m_grid.SetItemText(0,3,L"关联道路");
m_grid.SetItemText(0,2,L"性质");
m_grid.SetItemText(0,3,L"备注");

int i=0;
char tmp[30];
CString GH;
if(NSTA> 0 )  	
{
for(i=0; i<NSTA; i++)
{
acdbRToS(pm->XLC(lcb[i].lc,GH),2,3,tmp);
m_grid.SetItemText(i+1,1,tmp);

// acdbRToS(lcb[i].gldmlc,2,3,tmp);
// m_grid.SetItemText(i+1,2,tmp);

// m_grid.SetItemText(i+1,3,lcb[i].glRoadName);
_stprintf(tmp,L"%d",i+1);
m_grid.SetItemText(i+1,0,tmp);
}
_stprintf(tmp,L"%d",i+1);
m_grid.SetItemText(i+1,0,tmp);
m_grid.SetColumnWidth(0,50);
m_grid.ResetScrollBars();
m_grid.SetFocusCell(1,1);
m_grid.Invalidate();

}
UpdateData(false);


}
*/

void CCalSta::OnPaint() 
{
	CPaintDC dc(this); // device context for painting


}

BOOL CCalSta::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_grid.DeleteAllItems();
	m_grid.SetRowCount(NSTA+2);
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
	m_grid.SetItemText(0,1,L"桩 号");
	m_grid.SetItemText(0,2,L"性质");
	m_grid.SetItemText(0,3,L"备注");
	//  m_grid.SetItemText(0,2,L"关联桩号");
	//  m_grid.SetItemText(0,3,L"关联道路");

	int i=0;
	ACHAR tmp[30];
	CString GH;
	if(NSTA> 0 )  	
	{
		for(i=0; i<NSTA; i++)
		{
			acdbRToS(pm->XLC(lcb[i].lc,GH),2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			//   acdbRToS(lcb[i].gldmlc,2,3,tmp);
			//   m_grid.SetItemText(i+1,2,tmp);

			//	   m_grid.SetItemText(i+1,3,lcb[i].glRoadName);
			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}
		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);
		m_grid.SetColumnWidth(0,50);
		m_grid.ResetScrollBars();
		m_grid.SetFocusCell(1,1);
		m_grid.Invalidate();

	}
	UpdateData(false);

	return CPropertyPage::OnSetActive();
}
