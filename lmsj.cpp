// LMSJ.cpp : implementation file
//

#include "stdafx.h"
#include "LMSJ.h"
#include "LMSheetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;

/////////////////////////////////////////////////////////////////////////////
CLMKGridCtrl::CLMKGridCtrl()
{

}

CLMKGridCtrl::~CLMKGridCtrl()
{
}

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////
void CLMKGridCtrl::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{	

	if(nRow==0) return;
	CGridCtrl::OnEditCell( nRow, nCol, point, nChar);
	CStringArray Items;
	CString strCol=GetItemText(0,nCol);
	CString strCell=GetItemText(nRow,nCol);
	CString strCol1=GetItemText(nRow,1);

	if(nCol==8)
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"是");
		Items.Add(L"否");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);

	}

	if(nCol==11)
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"0-线性");
		Items.Add(L"1-抛物线");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);

	}
	else if(nCol==12)
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"0-分隔带边缘");
		Items.Add(L"1-路肩");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);				
	}

	Invalidate();
	//GetParent()->GetParent()->Invalidate();

}
///////
BEGIN_MESSAGE_MAP(CLMKGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(CLMkGridCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LMSJ property page

IMPLEMENT_DYNCREATE(LMSJ, CPropertyPage)

	LMSJ::LMSJ() : CPropertyPage(LMSJ::IDD,1)
{
	//{{AFX_DATA_INIT(LMSJ)
	// NOTE: the ClassWizard will add member initialization here




	NLMK = 0;
	LMK = NULL;
	flag1 = 0;
	//	SetGrid(); 



	//}}AFX_DATA_INIT
}

LMSJ::~LMSJ()
{
}

void LMSJ::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LMSJ)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//	DDX_GridControl(pDX, IDC_LIST_LM,m_grid); 
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_LM, m_grid);
}


BEGIN_MESSAGE_MAP(LMSJ, CPropertyPage)
	//{{AFX_MSG_MAP(LMSJ)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	ON_BN_CLICKED(IDC_BUTTON_refresh, OnBUTTONrefresh)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LMSJ message handlers

void LMSJ::OnBUTTONAdd() 
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

void LMSJ::OnButtonCopy() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	LMSJ lmk;
	ACHAR tmp[256];
	LMSheetDlg *parent;
	parent = (LMSheetDlg *)this->GetParent();
	if(ZorY==-1)//当前为左侧
	{

		parent->ylmk.NLMK = m_grid.GetRowCount() - 2;  
		if(parent->ylmk.NLMK<1)
			return;
		if(parent->ylmk.LMK) delete []parent->ylmk.LMK;
		parent->ylmk.LMK = NULL;

		parent->ylmk.LMK = new LMKdata[parent->ylmk.NLMK];

		for(int i=1; i<=parent->ylmk.NLMK; i++)
		{

			_tcscpy(tmp, m_grid.GetItemText(i, 1));
			parent->ylmk.LMK[i-1].xlc = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 2));
			parent->ylmk.LMK[i-1].fgdk = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 3));
			parent->ylmk.LMK[i-1].fgdhp = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 4));
			parent->ylmk.LMK[i-1].lydk = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 5));
			parent->ylmk.LMK[i-1].cxdk = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 6));
			parent->ylmk.LMK[i-1].lmhp = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 7));
			parent->ylmk.LMK[i-1].yljk = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 8));
			if(_tcscmp(tmp,L"否")==0) parent->ylmk.LMK[i-1].yljhp=1;
			else parent->ylmk.LMK[i-1].yljhp=0;
			//parent->ylmk.LMK[i-1].yljhp= _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 9));
			parent->ylmk.LMK[i-1].tljk= _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 10));
			parent->ylmk.LMK[i-1].tljhp= _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 11));
			if(_tcscmp(tmp,L"0-线性")==0) parent->ylmk.LMK[i-1].bkmode=0;
			else parent->ylmk.LMK[i-1].bkmode=1;

			_tcscpy(tmp, m_grid.GetItemText(i, 12));
			if(_tcscmp(tmp,L"0-分隔带边缘")==0) parent->ylmk.LMK[i-1].sjbgmode=0;
			else parent->ylmk.LMK[i-1].sjbgmode=1;

			_tcscpy(tmp, m_grid.GetItemText(i, 13));
			parent->ylmk.LMK[i-1].OffsetByXLZX = _wtof(tmp);

		}
		//		parent->ylmk.SetGrid(); 

	}
	else
	{
		parent->zlmk.NLMK = m_grid.GetRowCount() - 2;  
		if(parent->zlmk.NLMK<1)
			return;
		if(parent->zlmk.LMK) delete []parent->zlmk.LMK;
		parent->zlmk.LMK = new LMKdata[parent->zlmk.NLMK];

		for(int i=1; i<=parent->zlmk.NLMK; i++)
		{
			_tcscpy(tmp, m_grid.GetItemText(i, 1));
			parent->zlmk.LMK[i-1].xlc = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 2));
			parent->zlmk.LMK[i-1].fgdk = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 3));
			parent->zlmk.LMK[i-1].fgdhp = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 4));
			parent->zlmk.LMK[i-1].lydk = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 5));
			parent->zlmk.LMK[i-1].cxdk = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 6));
			parent->zlmk.LMK[i-1].lmhp = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 7));
			parent->zlmk.LMK[i-1].yljk = _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 8));
			//	parent->zlmk.LMK[i-1].yljhp= _wtof(tmp);

			if(_tcscmp(tmp,L"否")==0) parent->zlmk.LMK[i-1].yljhp=1;
			else parent->zlmk.LMK[i-1].yljhp=0;

			_tcscpy(tmp, m_grid.GetItemText(i, 9));
			parent->zlmk.LMK[i-1].tljk= _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 10));
			parent->zlmk.LMK[i-1].tljhp= _wtof(tmp);

			_tcscpy(tmp, m_grid.GetItemText(i, 11));
			if(_tcscmp(tmp,L"0-线性")==0) parent->zlmk.LMK[i-1].bkmode=0;
			else parent->zlmk.LMK[i-1].bkmode=1;

			_tcscpy(tmp, m_grid.GetItemText(i, 12));
			if(_tcscmp(tmp,L"0-分隔带边缘")==0) parent->zlmk.LMK[i-1].sjbgmode=0;
			else parent->zlmk.LMK[i-1].sjbgmode=1;

			_tcscpy(tmp, m_grid.GetItemText(i, 13));
			parent->zlmk.LMK[i-1].OffsetByXLZX = _wtof(tmp);
		}
		//		parent->zlmk.SetGrid(); 

	}

}

void LMSJ::OnBUTTONdel() 
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

void LMSJ::OnBUTTONrefresh() 
{
	// TODO: Add your control notification handler code here
	int CurSel = m_grid.GetFocusCell().row;
	if (CurSel >= 0)
		Invalidate();
	else
		AfxMessageBox(L"请选中表格中相应行进行刷新！");
}

BOOL LMSJ::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=15;
	rect.right-=10;
	rect.bottom-=200;
	//m_grid.Create(rect, this, 900);

	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | 
		DT_END_ELLIPSIS | DT_WORDBREAK;
	SetGrid();
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE	
}

void LMSJ::DrawLine(LMKdata &tmplmk)
{

	CWnd *pWnddlg=GetDlgItem(IDC_DRAW_LM);
	CPaintDC draw(pWnddlg);

	CRect rect;
	pWnddlg->GetClientRect(&rect);
	// ****************************************
	CBrush bkBrush(RGB(128,128,128));  
	draw.SetBkColor(RGB(128,128,128));
	//draw.Rectangle(&rect);
	draw.FillRect(&rect,&bkBrush);
	CBrush *oldbr=draw.SelectObject(&bkBrush);

	// *****************************************
	CPen pen(PS_SOLID, 1, RGB(180,180,40));
	CPen* pOldPen = draw.SelectObject(&pen);
	// *****************************************

	int i;
	double xmax;
	double xb,yb;
	//	double sj[8]={0.0};

	xmax = tmplmk.fgdk + tmplmk.cxdk + tmplmk.yljk + tmplmk.tljk + tmplmk.lydk + 0.1;

	xb = (rect.Width() - 20) / xmax;
	yb = (rect.Height() - 5) / 2.0;
	double x;
	if(ZorY==-1)
		x = rect.Width() - 10;
	else
		x = 10;
	double y = rect.Height() / 2.0 - yb*0.2;
	/////////路中线
	CPen pen1(PS_DOT, 1, RGB(0,255,0));
	draw.SelectObject(&pen1);	
	draw.MoveTo(x,3);
	draw.LineTo(x,rect.Height()-3);
	//////
	CPen pen2(PS_SOLID, 3, RGB(0,0,255));
	draw.SelectObject(&pen2);
	draw.MoveTo(x,y);
	if(ZorY==-1)
		x -= xb*tmplmk.fgdk;
	else
		x += xb*tmplmk.fgdk;
	draw.LineTo(x,y);
	y+=yb*0.2;
	draw.LineTo(x,y);

	CPen pen3(PS_SOLID,3, RGB(255,255,9));
	draw.SelectObject(&pen3);
	if(ZorY==-1)
		x-=xb*tmplmk.lydk;
	else
		x+=xb*tmplmk.lydk;

	draw.LineTo(x,y);

	CPen pen4(PS_SOLID, 3, RGB(255,0,0));
	draw.SelectObject(&pen4);
	double x1,y1;
	y1=y+yb*0.5;
	draw.LineTo(x,y1);
	if(ZorY==-1)
		x1=x-xb*tmplmk.cxdk;
	else
		x1=x+xb*tmplmk.cxdk;

	draw.LineTo(x1,y1);
	y1-=yb*0.5/2.0;
	draw.LineTo(x1,y1);
	draw.MoveTo(x,y);
	x=x1;
	draw.LineTo(x,y);

	CPen pen5(PS_SOLID, 3, RGB(255,128,64));
	draw.SelectObject(&pen5);
	if(ZorY==-1)
		x-=xb*tmplmk.yljk;
	else
		x+=xb*tmplmk.yljk;

	draw.LineTo(x,y);
	y1=y+yb*0.5/2.0;
	draw.LineTo(x,y1);
	draw.LineTo(x1,y1);	

	CPen pen6(PS_SOLID, 3, RGB(201,78,191));
	draw.SelectObject(&pen6);
	draw.MoveTo(x,y);
	if(ZorY==-1)
		x-=xb*tmplmk.tljk;
	else
		x+=xb*tmplmk.tljk;

	draw.LineTo(x,y);

	draw.SelectObject(pOldPen);
	draw.SelectObject(oldbr);

}

void LMSJ::OnPaint() 
{	

	CPaintDC dc(this); // device context for painting
	int i,j;
	LMKdata tmplmk;
	ACHAR tmp[40];
	// TODO: Add your message handler code here
	if(flag1) 
	{
		i=m_grid.GetFocusCell().row;

		_tcscpy(tmp , m_grid.GetItemText(i, 1));
		tmplmk.xlc = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 2));
		tmplmk.fgdk = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 3));
		tmplmk.fgdhp = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 4));
		tmplmk.lydk = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 5));
		tmplmk.cxdk = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 6));
		tmplmk.lmhp = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 7));
		tmplmk.yljk = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 8));
		tmplmk.yljhp = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 9));
		tmplmk.tljk = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 10));
		tmplmk.tljhp = _wtof(tmp);
		_tcscpy(tmp , m_grid.GetItemText(i, 12));
		if(_tcscmp(tmp,L"0-分隔带边缘")) 
			tmplmk.sjbgmode=0;
		else
			tmplmk.sjbgmode=1;

		DrawLine(tmplmk);
	}
	else if(NLMK>0) 
		DrawLine(LMK[0]);

	flag1=1;
	// Do not call CPropertyPage::OnPaint() for painting messages* /

}

void LMSJ::SetLmk(int nlmk,LMKdata lmk[]) 
{
	NLMK = nlmk;
	if(LMK) delete []LMK;
	LMK = NULL;
	LMK = new LMKdata[nlmk];
	for(int i=0; i<nlmk; i++)
		LMK[i] = lmk[i];	
}


BOOL LMSJ::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class

	SetGrid(); 
	CPropertyPage::OnSetActive();

	return true;
}

void LMSJ::SetGrid() 
{
	ACHAR  tmp[80];
	int i,j;

	txtbkclr=RGB(0XFF, 0XFF,0XE0);
	fixbkclr=RGB(155,188,0);

	m_grid.SetRowCount(NLMK+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(14);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetFixedBkColor(fixbkclr);

	Item.nFormat = DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_WORDBREAK | DT_SINGLELINE;
	Item.row = 0;
	for(int k=0;k<m_grid.GetColumnCount();k++)
	{
		for( i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}		
		m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}

	m_grid.SetItemText(0,0,L"序号");
	m_grid.SetItemText(0,1,L"分段终里程");
	m_grid.SetItemText(0,2,L"分隔带半宽");
	m_grid.SetItemText(0,3,L"分隔带横坡%");
	m_grid.SetItemText(0,4,L"路缘带宽");
	m_grid.SetItemText(0,5,L"车行道宽");
	m_grid.SetItemText(0,6,L"路面横坡%");
	m_grid.SetItemText(0,7,L"硬路肩宽");
	//	m_grid.SetItemText(0,8,L"硬路肩横坡%");
	m_grid.SetItemText(0,8,L"是否爬坡道");
	m_grid.SetItemText(0,9,L"土路肩宽");
	m_grid.SetItemText(0,10,L"土路肩横坡%");
	m_grid.SetItemText(0,11,L"变宽方式");
	m_grid.SetItemText(0,12,L"设计标高位置");
	m_grid.SetItemText(0,13,L"路基中线偏移量");

	i=0;
	if(NLMK != 0)
	{
		for(i=0; i<NLMK; i++)
		{
			ads_rtos(LMK[i].xlc,2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			ads_rtos(LMK[i].fgdk,2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);

			ads_rtos(LMK[i].fgdhp,2,3,tmp);
			m_grid.SetItemText(i+1,3,tmp);

			ads_rtos(LMK[i].lydk,2,3,tmp);
			m_grid.SetItemText(i+1,4,tmp);

			ads_rtos(LMK[i].cxdk,2,3,tmp);
			m_grid.SetItemText(i+1,5,tmp);

			ads_rtos(LMK[i].lmhp,2,3,tmp);
			m_grid.SetItemText(i+1,6,tmp);

			ads_rtos(LMK[i].yljk,2,3,tmp);
			m_grid.SetItemText(i+1,7,tmp);

			if(LMK[i].yljhp>0.1) m_grid.SetItemText(i+1,8,L"否");
			else m_grid.SetItemText(i+1,8,L"是");
			//ads_rtos(LMK[i].yljhp,2,3,tmp);
			//m_grid.SetItemText(i+1,8,tmp);

			ads_rtos(LMK[i].tljk,2,3,tmp);
			m_grid.SetItemText(i+1,9,tmp);

			ads_rtos(LMK[i].tljhp,2,3,tmp);
			m_grid.SetItemText(i+1,10,tmp);

			if(LMK[i].bkmode==0) m_grid.SetItemText(i+1,11,L"0-线性");
			else m_grid.SetItemText(i+1,11,L"1-抛物线");

			if(LMK[i].sjbgmode==0) m_grid.SetItemText(i+1,12,L"0-分隔带边缘");
			else m_grid.SetItemText(i+1,12,L"1-路肩");
			_stprintf(tmp , L"%d" , i+1);
			m_grid.SetItemText(i+1,0,tmp);

			ads_rtos(LMK[i].OffsetByXLZX,2,3,tmp);
			m_grid.SetItemText(i+1,13,tmp);
		}     
	}
	_stprintf(tmp , L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);

	m_grid.SetColumnWidth(0,40);
	m_grid.SetColumnWidth(1,75);
	m_grid.SetColumnWidth(2,75);
	m_grid.SetColumnWidth(3,85);
	for( i=4;i<14;i++)
		m_grid.SetColumnWidth(i,85);
	m_grid.SetColumnWidth(11,90);
	m_grid.SetColumnWidth(12,95);
	m_grid.SetColumnWidth(13,120);
	m_grid.SetRowHeight(0, DEFAULTCOLHEI);
	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);
	m_grid.AutoSize();
}

void LMSJ::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertyPage::OnOK();
	int i,j;
	ACHAR tmp[80];
	NLMK = m_grid.GetRowCount() - 2;  
	if(LMK) delete [] LMK;
	if(NLMK>0)
		LMK = new  LMKdata[NLMK];
	if(LMK == NULL)
	{
		AfxMessageBox(L"分配内存出错!");
		return;
	}      
	for(i=1; i<=NLMK; i++)
	{
		_tcscpy(tmp, m_grid.GetItemText(i, 1));
		LMK[i-1].xlc = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 2));
		LMK[i-1].fgdk = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 3));
		LMK[i-1].fgdhp = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 4));
		LMK[i-1].lydk = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 5));
		LMK[i-1].cxdk = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 6));
		LMK[i-1].lmhp = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 7));
		LMK[i-1].yljk = _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 8));
		if(_tcscmp(tmp,L"否")==0) LMK[i-1].yljhp=1;
		else LMK[i-1].yljhp=0;

		//_tcscpy(tmp, m_grid.GetItemText(i, 8));
		//LMK[i-1].yljhp= _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 9));
		LMK[i-1].tljk= _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 10));
		LMK[i-1].tljhp= _wtof(tmp);

		_tcscpy(tmp, m_grid.GetItemText(i, 11));
		if(_tcscmp(tmp,L"0-线性")==0) LMK[i-1].bkmode=0;
		else LMK[i-1].bkmode=1;

		_tcscpy(tmp, m_grid.GetItemText(i, 12));
		if(_tcscmp(tmp,L"0-分隔带边缘")==0) LMK[i-1].sjbgmode=0;
		else LMK[i-1].sjbgmode=1;

		_tcscpy(tmp, m_grid.GetItemText(i, 13));
		LMK[i-1].OffsetByXLZX = _wtof(tmp);

	}

}	

BOOL LMSJ::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnOK(); 
	return CPropertyPage::OnKillActive();
}
