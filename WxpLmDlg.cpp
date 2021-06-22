// WxpLmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WxpLmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;

void NumberToString(double xz , ACHAR str[]);
double  StringToNumber( CString str);      

//////////////////////
void WxpLmDlg::NumberToString(int xz , ACHAR str[])
{
	switch(xz)
	{
	case 0:
		_tcscpy(str ,L"0-地质概况");
		break;
	case 1:
		_tcscpy(str ,L"1-设计高程");
		break;
	case 2:
		_tcscpy(str ,L"2-地面高程");
		break;
	case 3:
		_tcscpy(str ,L"3-坡度/坡长");
		break;
	case 4:
		_tcscpy(str ,L"4-里程桩号");
		break;
	case 5:
		_tcscpy(str ,L"5-直线及平曲线");
		break;
	case 6:
		_tcscpy(str ,L"6-超高");
		break;  
	case 7:
		_tcscpy(str ,L"7-填挖高");
		break;  
	case 8:
		_tcscpy(str ,L"8-竖曲线");
		break; 
	default:
		_tcscpy(str ,L"0-地质概况");
		break;
	}
}

//---------------------------------------------------------------
double  WxpLmDlg::StringToNumber( CString  str)      
{
	int xz;

	if(str == "0-地质概况")
		xz = 0;
	else if(str == "1-设计高程")
		xz = 1;
	else if(str  == "2-地面高程")
		xz = 2;
	else if(str == "3-坡度/坡长")
		xz = 3;
	else if(str == "4-里程桩号")
		xz = 4;
	else if(str == "5-直线及平曲线")
		xz = 5;
	else if(str == "6-超高")
		xz = 6;
	else if(str == "7-填挖高")
		xz = 7;
	else if(str == "8-竖曲线")
		xz = 8;
	else
		xz = 0;

	return xz;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CComGridCtrl::CComGridCtrl()
{
}

CComGridCtrl::~CComGridCtrl()
{
}

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

void CComGridCtrl::CreateInPlaceEditControl(CRect& rect, DWORD dwStyle, UINT nID,
int nRow, int nCol,
LPCTSTR szText, int nChar)
{
CStringArray Items;
if(nCol==1)
{
Items.Add(L"0-地质特征");
Items.Add(L"1-线间距");
Items.Add(L"2-路肩设计高程");
Items.Add(L"3-变坡点高程");
Items.Add(L"4-路肩设计坡度");
Items.Add(L"5-地面高程");
Items.Add(L"6-加桩");
Items.Add(L"7-里程");
Items.Add(L"8-线路平面");
Items.Add(L"9-连续里程");
Items.Add(L"10-竖曲线");
Items.Add(L"11-轨道结构类型");
Items.Add(L"12-轨道结构高度");
Items.Add(L"13-轨面设计高程");
new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,nID,nRow, nCol, 
Items, szText, nChar);
}    
else
{
new CInPlaceEdit(this, rect, dwStyle, nID, nRow, nCol, szText, nChar);
}

}
void CComGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{

CGridCtrl::OnEndEditCell(nRow,nCol,str);
}*/


// WxpLmDlg property page
IMPLEMENT_DYNCREATE(WxpLmDlg, CPropertyPage)

	WxpLmDlg::WxpLmDlg() : CPropertyPage(WxpLmDlg::IDD)
{
	//{{AFX_DATA_INIT(WxpLmDlg)
	WayorRamp = 0;
	gxinf = false;
	m_infP1 = FALSE;
	m_infP2 = FALSE;
	m_infP3 = FALSE;
	m_infP4 = FALSE;
	m_infP5 = FALSE;
	m_infP6 = FALSE;
	m_infP7 = FALSE;
	m_infP8 = FALSE;
	m_p1name = _T("P1");
	m_p2name = _T("P2");
	m_p3name = _T("P3");
	m_p4name = _T("P4");
	m_p5name = _T("P5");
	m_p6name = _T("P6");
	m_p7name = _T("P7");
	m_p8name = _T("P8");
	//}}AFX_DATA_INIT
	Ispaint = false;
}

WxpLmDlg::~WxpLmDlg()
{
}

void WxpLmDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WxpLmDlg)
	DDX_Check(pDX, IDC_CHECK_P1, m_infP1);
	DDX_Check(pDX, IDC_CHECK_P2, m_infP2);
	DDX_Check(pDX, IDC_CHECK_P3, m_infP3);
	DDX_Check(pDX, IDC_CHECK_P4, m_infP4);
	DDX_Check(pDX, IDC_CHECK_P5, m_infP5);
	DDX_Check(pDX, IDC_CHECK_P6, m_infP6);
	DDX_Check(pDX, IDC_CHECK_P7, m_infP7);
	DDX_Check(pDX, IDC_CHECK_P8, m_infP8);
	DDX_Text(pDX, IDC_EDIT_P1NAME, m_p1name);
	DDX_Text(pDX, IDC_EDIT_P2NAME, m_p2name);
	DDX_Text(pDX, IDC_EDIT_P3NAME, m_p3name);
	DDX_Text(pDX, IDC_EDIT_P4NAME, m_p4name);
	DDX_Text(pDX, IDC_EDIT_P5NAME, m_p5name);
	DDX_Text(pDX, IDC_EDIT_P6NAME, m_p6name);
	DDX_Text(pDX, IDC_EDIT_P7NAME, m_p7name);
	DDX_Text(pDX, IDC_EDIT_P8NAME, m_p8name);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(WxpLmDlg, CPropertyPage)
	//{{AFX_MSG_MAP(WxpLmDlg)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_HIGHWAY, OnRadioHighway)
	ON_BN_CLICKED(IDC_RADIO_RAMP, OnRadioRamp)
	ON_BN_CLICKED(IDC_CHECK_P4, OnCheckP4)
	ON_BN_CLICKED(IDC_CHECK_P3, OnCheckP3)
	ON_BN_CLICKED(IDC_CHECK_P2, OnCheckP2)
	ON_BN_CLICKED(IDC_CHECK_P1, OnCheckP1)
	ON_BN_CLICKED(IDC_CHECK_P5, OnCheckP5)
	ON_BN_CLICKED(IDC_CHECK_P6, OnCheckP6)
	ON_BN_CLICKED(IDC_CHECK_P7, OnCheckP7)
	ON_BN_CLICKED(IDC_CHECK_P8, OnCheckP8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WxpLmDlg message handlers
void WxpLmDlg::UpdateWxp() 
{
	// TODO: Add your specialized code here and/or call the base class
	int i;
	ACHAR tmp[30];
	UpdateData();
	wxpnum= m_wxp.GetRowCount() - 2;
	if(wxpnum>14)
		wxpnum=14;
	for(i=1; i<=wxpnum; i++)
	{
		wxp[i-1].wxpinf = StringToNumber(m_wxp.GetItemText(i, 1));
		_tcscpy(tmp ,m_wxp.GetItemText(i, 2));
		wxp[i-1].Lht = _wtof(tmp);
		_tcscpy(tmp ,m_wxp.GetItemText(i, 3));
		wxp[i-1].texth = _wtof(tmp);
	}
}

BOOL WxpLmDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here	
	int i,j;
	ACHAR tmp[50];
	CRect rect;
	GetClientRect(rect);
	rect.left+=1;
	rect.top+=10;
	rect.right-=1;
	rect.bottom-=260;
	m_wxp.Create(rect, this, 900);

	m_wxp.SetRowCount(wxpnum+2);
	m_wxp.SetFixedRowCount(1);
	m_wxp.SetColumnCount(4);
	m_wxp.SetFixedColumnCount(1);
	m_wxp.SetTextBkColor(txtbkclr);
	m_wxp.SetFixedBkColor(fixbkclr);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;
	Item.lfFont=font;

	for(int k=0;k<m_wxp.GetColumnCount();k++)
	{
		for( i=0;i<m_wxp.GetRowCount();i++)
		{  
			Item.row = i;
			Item.col = k;
			m_wxp.SetItem(&Item);
		}
		//       m_wxp.SetItemState(i-1, k,GVIS_READONLY);
	}
	//	Item.nFormat =DT_CENTER| DT_VCENTER|DT_WORDBREAK;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;


	m_wxp.SetItemText(0,0,L"序 号");
	m_wxp.SetItemText(0,1,L"栏 目");
	m_wxp.SetItemText(0,2,L"栏 高(mm)");
	m_wxp.SetItemText(0,3,L"字 高(mm)");

	_tcscpy(tmp ,L"第1栏");
	m_wxp.SetItemText(1, 0 , tmp);

	CWnd *pcwnd; 
	CRect Rect; 
	int width;

	pcwnd = GetDlgItem(IDC_LIST_WXP);
	pcwnd->GetClientRect(&Rect);
	width=Rect.Width();

	m_wxp.SetColumnWidth(0,int(54*width/350));
	m_wxp.SetColumnWidth(1,int(110*width/350));
	m_wxp.SetColumnWidth(2,int(85*width/350));
	m_wxp.SetColumnWidth(3,int(85*width/350));

	for(i=0; i<wxpnum; i++)
	{
		//		ads_printf(L"%d %lf %lf\n",wxp[i].wxpinf,wxp[i].Lht,wxp[i].texth);
		NumberToString(wxp[i].wxpinf , tmp);
		m_wxp.SetItemText(i+1 , 1 , tmp);
		_stprintf(tmp,L"%0.1lf",wxp[i].Lht);   
		m_wxp.SetItemText(i+1, 2 , tmp);
		_stprintf(tmp,L"%0.1lf",wxp[i].texth);   
		m_wxp.SetItemText(i+1, 3 , tmp);

		_stprintf(tmp ,L"第%d栏" , i+1);
		m_wxp.SetItemText(i+1, 0 , tmp);
	}

	_stprintf(tmp ,L"第%d栏" , i+1);
	m_wxp.SetItemText(i+1, 0 , tmp);
	m_wxp.SetRowHeight(0, DEFAULTCOLHEI);
	for( i=1;i<m_wxp.GetRowCount();i++)
		m_wxp.SetRowHeight(i, DEFAULTCOLHEI);

	m_wxp.ResetScrollBars();
	m_wxp.SetFocusCell(1,1); 

	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_HIGHWAY);
	pbtn->SetCheck(1);
	GetDlgItem(IDC_CHECK_P1)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P2)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P3)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P4)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P5)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P6)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P7)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P8)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P1NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P2NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P3NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P4NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P5NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P6NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P7NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P8NAME)->EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void WxpLmDlg::OnBUTTONAdd() 
{
	// TODO: Add your control notification handler code here
	ACHAR tmp[80];

	int nRow = m_wxp.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		Item.row=nRow;
		m_wxp.InsertRow(_T("NEW"), nRow);
		int i;
		for( i=0;i<m_wxp.GetColumnCount();i++)
		{
			Item.col=i;
			m_wxp.SetItem(&Item);
		}
		for(i=nRow;i<m_wxp.GetRowCount();i++)
		{
			CString str;
			str.Format(L"第%d栏",i);
			m_wxp.SetItemText(i,0,str);
		}
		for( i=1;i<m_wxp.GetRowCount();i++)
			m_wxp.SetRowHeight(i, DEFAULTCOLHEI);

		if(m_wxp.GetRowCount()>3)
		{
			if(nRow==m_wxp.GetRowCount()-2)
			{
				for( i=1;i<m_wxp.GetColumnCount();i++) 
					m_wxp.SetItemText(nRow,i,m_wxp.GetItemText(nRow-1,i));
			}
			else
				for( i=1;i<m_wxp.GetColumnCount();i++) 
					m_wxp.SetItemText(nRow,i,m_wxp.GetItemText(nRow+1,i));
		}
		m_wxp.SetFocusCell(nRow,1);		
		wxpnum= m_wxp.GetRowCount() - 2;
		if(wxpnum>14)
			wxpnum=14;		
		m_wxp.Invalidate();
	}		
}

void WxpLmDlg::OnBUTTONdel() 
{
	// TODO: Add your control notification handler code here
	ACHAR tmp[80];

	int nRow = m_wxp.GetFocusCell().row;
	if  (nRow==m_wxp.GetRowCount()-1) return;
	if (nRow >= 0)
	{
		m_wxp.DeleteRow(nRow);
		int i;
		for( i=nRow;i<m_wxp.GetRowCount();i++)
		{
			CString str;
			str.Format(L"第%d栏",i);
			m_wxp.SetItemText(i,0,str);
		}
		m_wxp.SetFocusCell(nRow,1);
		wxpnum= m_wxp.GetRowCount() - 2;
		if(wxpnum>14)
			wxpnum=14;
		for(i=1; i<=wxpnum; i++)
		{
			wxp[i-1].wxpinf = StringToNumber(m_wxp.GetItemText(i, 1));
			_tcscpy(tmp ,m_wxp.GetItemText(i, 2));
			wxp[i-1].Lht = _wtof(tmp);
			_tcscpy(tmp ,m_wxp.GetItemText(i, 3));
			wxp[i-1].texth = _wtof(tmp);
		}	
		m_wxp.Invalidate();
	}
}

void WxpLmDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	int i,j;
	ACHAR tmp[30];
	Ispaint = true;
	if(gxinf)
	{
		m_wxp.SetRowCount(wxpnum+2); 

		for(int k=0;k<m_wxp.GetColumnCount();k++)
		{
			for( i=0;i<m_wxp.GetRowCount();i++)
			{  
				Item.row = i;
				Item.col = k;
				m_wxp.SetItem(&Item);
			}
		}
		m_wxp.SetItemText(0,0,L"序 号");
		m_wxp.SetItemText(0,1,L"栏 目");
		m_wxp.SetItemText(0,2,L"栏 高(mm)");
		m_wxp.SetItemText(0,3,L"字 高(mm)");		
		for(i=0; i<wxpnum; i++)
		{
			NumberToString(wxp[i].wxpinf , tmp);
			m_wxp.SetItemText(i+1 , 1 , tmp);
			_stprintf(tmp,L"%0.1lf",wxp[i].Lht);   
			m_wxp.SetItemText(i+1, 2 , tmp);
			_stprintf(tmp,L"%0.1lf",wxp[i].texth);   
			m_wxp.SetItemText(i+1, 3 , tmp);

			_stprintf(tmp ,L"第%d栏" , i+1);
			m_wxp.SetItemText(i+1, 0 , tmp);
		}

		_stprintf(tmp ,L"第%d栏" , i+1);
		m_wxp.SetItemText(i+1, 0 , tmp);
		m_wxp.SetItemText(i+1, 1 ,L""),m_wxp.SetItemText(i+1, 2 ,L""),m_wxp.SetItemText(i+1, 3 ,L"");
		m_wxp.SetRowHeight(0, DEFAULTCOLHEI);
		for( i=1;i<m_wxp.GetRowCount();i++)
			m_wxp.SetRowHeight(i, DEFAULTCOLHEI);

		// m_wxp.ResetScrollBars();
		m_wxp.SetFocusCell(1,1); 	
	}	

	CWnd *pcwnd; 
	CRect Rect; 
	int width;
	pcwnd = GetDlgItem(IDC_LIST_WXP);
	pcwnd->GetClientRect(&Rect);
	width=Rect.Width();	
	m_wxp.SetColumnWidth(0,int(54*width/350));
	m_wxp.SetColumnWidth(1,int(110*width/350));
	m_wxp.SetColumnWidth(2,int(85*width/350));
	m_wxp.SetColumnWidth(3,int(85*width/350));
	gxinf=false;	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void WxpLmDlg::OnRadioHighway() 
{
	// TODO: Add your control notification handler code here
	WayorRamp = 0;
	GetDlgItem(IDC_CHECK_P1)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P2)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P3)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P4)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P5)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P6)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P7)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P8)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_P1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P1NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P2NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P3NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P4NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P5NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P6NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P7NAME)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_P8NAME)->EnableWindow(false);
}

void WxpLmDlg::OnRadioRamp() 
{
	// TODO: Add your control notification handler code here
	WayorRamp = 1; 
	GetDlgItem(IDC_CHECK_P1)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_P2)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_P3)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_P4)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_P5)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_P6)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_P7)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_P8)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_P1)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_P1NAME)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_P2NAME)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_P3NAME)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_P4NAME)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_P5NAME)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_P6NAME)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_P7NAME)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_P8NAME)->EnableWindow(true);
}

void WxpLmDlg::OnCheckP4() 
{
	// TODO: Add your control notification handler code here
	m_infP4 = !m_infP4;
}

void WxpLmDlg::OnCheckP3() 
{
	// TODO: Add your control notification handler code here
	m_infP3 = !m_infP3;
}

void WxpLmDlg::OnCheckP2() 
{
	// TODO: Add your control notification handler code here
	m_infP2 = !m_infP2;
}

void WxpLmDlg::OnCheckP1() 
{
	// TODO: Add your control notification handler code here
	m_infP1 = !m_infP1;
}

void WxpLmDlg::OnCheckP5() 
{
	// TODO: Add your control notification handler code here
	m_infP5 = !m_infP5;
}

void WxpLmDlg::OnCheckP6() 
{
	// TODO: Add your control notification handler code here
	m_infP6 = !m_infP6;
}

void WxpLmDlg::OnCheckP7() 
{
	// TODO: Add your control notification handler code here
	m_infP7 = !m_infP7;
}

void WxpLmDlg::OnCheckP8() 
{
	// TODO: Add your control notification handler code here
	m_infP8 = !m_infP8;
}
