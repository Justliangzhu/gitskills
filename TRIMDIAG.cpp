// TRIMDIAG.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TRIMDIAG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern int TKNUM;
CStringArray TKLCAy;

/////////////////////////////////////////////////////////////////////////////
// TRIMDIAG dialog


TRIMDIAG::TRIMDIAG(CWnd* pParent /*=NULL*/)
	: CDialog(TRIMDIAG::IDD, pParent)
{
	//{{AFX_DATA_INIT(TRIMDIAG)
	m_LC = _T("");
	m_TRIMFLAG = 0;
	//}}AFX_DATA_INIT
	m_START = _T("1");
	NUM=TKNUM;
	m_END.Format(L"%d",NUM);//end	
	m_NUM.Format(L"共有%d张图",NUM);//end

}

void TRIMDIAG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TRIMDIAG)
	DDX_Control(pDX, IDC_COMBO2, m_EndCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_StartCtrl);
	DDX_Control(pDX, IDCANCEL, m_CANCLE);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_CBString(pDX, IDC_COMBO1, m_START);
	DDX_CBString(pDX, IDC_COMBO2, m_END);
	DDX_Text(pDX, IDC_STATIC_NUM, m_NUM);
	DDX_GridControl(pDX, IDC_GRID,m_grid);
	DDX_Text(pDX, IDC_STATIC_LC, m_LC);
	//DDX_Radio(pDX, IDC_RADIO_BREAK_road, m_TRIMFLAG);
	DDX_Check(pDX, IDC_CHECK_DRAWZDM, m_IsDrawZDM);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_MdbName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TRIMDIAG, CDialog)
	//{{AFX_MSG_MAP(TRIMDIAG)
	ON_WM_SIZE()
	ON_CBN_EDITCHANGE(IDC_COMBO1, OnEditchangeCombo1)
	ON_CBN_EDITCHANGE(IDC_COMBO2, OnEditchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_CHECK_DRAWZDM, OnBnClickedCheckDrawzdm)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TRIMDIAG message handlers

void TRIMDIAG::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	if(_wtof(m_START)>_wtof(m_END)||_wtof(m_START)<1||_wtof(m_END)<1||_wtof(m_START)>TKNUM||_wtof(m_END)>TKNUM)
	{
		AfxMessageBox(L"输入无效!");
		return;
	}
	CDialog::OnOK();
}

BOOL TRIMDIAG::OnInitDialog() 
{

	CDialog::OnInitDialog();
	ACHAR ch[20];
	_stprintf(ch,L"1");
	for(int i=1;i<=NUM;i++)
	{
		_stprintf(ch,L"%d",NUM-i+1);
		m_EndCtrl.AddString(ch);
		_stprintf(ch,L"%d",i);
		m_StartCtrl.AddString(ch);
	}
	m_START = _T("1");
	m_END = ch;//end
	m_TRIMFLAG=0;

	CreateGrid();
	BAS_DRAW_FUN ob;	
	ob.ReadWorkdir();
	m_MdbName = Cworkdir + "\\DATA\\" + Pro + ".mdb";
	m_IsDrawZDM = TRUE;
	UpdateData(false);

	// TODO: Add extra initialization here	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void TRIMDIAG::CreateGrid()
{
	if (TKLCAy.GetSize()==0)
	{
		return;
	}
	if(TKLCAy[0]=="NO")
	{
		AfxMessageBox(L"没有里程信息!");
		return;//0项为标记，1 项不用
	}

	Rows=TKNUM+1;
	Cols=3;
	COLORREF txtbkclr=RGB(0XFF, 0XFF,0XE0);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetRowCount(Rows);
	m_grid.SetColumnCount(Cols);
	m_grid.SetFixedRowCount(1);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetEditable(FALSE);
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;

	Item.row=0;
	Item.col=0;
	Item.strText="图框编号";
	m_grid.SetItem(&Item);

	Item.row=0;
	Item.col=1;
	Item.strText="起始里程";
	m_grid.SetItem(&Item);

	Item.row=0;
	Item.col=2;
	Item.strText="终止里程";
	m_grid.SetItem(&Item);
	int nState;
	for(int i=1;i<Rows;i++)
	{
		m_grid.RowOnlyRead(i);
		Item.row=i;
		Item.col=0;
		Item.strText.Format(L"√ %d",i);
		m_grid.SetItem(&Item);

		Item.col=1;
		//ads_printf(L"test%d %d\n",TKLCAy.GetSize(),2*i);
		Item.strText=TKLCAy.GetAt(2*i);
		m_grid.SetItem(&Item);

		Item.col=2;
		Item.strText=TKLCAy.GetAt(2*i+1);
		m_grid.SetItem(&Item);        

		nState = m_grid.GetItemState(i,1);
		m_grid.SetItemState(i,1, nState | GVIS_SELECTED);
		nState = m_grid.GetItemState(i,2);
		m_grid.SetItemState(i,2, nState | GVIS_SELECTED);		

	}   
	m_LC=m_grid.GetItemText(1,1)+"------"+m_grid.GetItemText(TKNUM,2);
	GetDlgItem(IDC_STATIC_LC)->SetWindowText(m_LC);
	m_grid.AutoSize();
}

void TRIMDIAG::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	m_OK.MoveWindow(0.15*cx,cy-30,0.3*cx,25);
	m_CANCLE.MoveWindow(0.55*cx,cy-30,0.3*cx,25);
}


void TRIMDIAG::OnEditchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	Tag();
}

void TRIMDIAG::OnEditchangeCombo2() 
{
	// TODO: Add your control notification handler code here
	Tag();
}

void TRIMDIAG::Tag()
{
	if(TKLCAy[0]=="NO")
	{
		return;//0项为标记，1 项不用
	}


	UpdateData(TRUE);
	int stk=int(_wtof(m_START)+0.001);
	int etk=int(_wtof(m_END)+0.001);
	if(stk<1||stk>TKNUM||etk<stk)
	{
		GetDlgItem(IDC_STATIC_LC)->SetWindowText(L"");		
		return;
	}
	int i;
	COLORREF txtbkclr=RGB(0,0,0);
	CString str;
	int nState;
	for(i=1;i<stk;i++)
	{
		Item.row=i;
		Item.col=0;
		str.Format(L"  %d",i);
		Item.strText=str;
		m_grid.SetItem(&Item);
		m_grid.SetItemState(i,1,GVIS_READONLY);		
		m_grid.SetItemState(i,2,GVIS_READONLY);		
	}
	for(i=etk+1;i<=TKNUM;i++)
	{
		Item.row=i;
		Item.col=0;
		str.Format(L"  %d",i);
		Item.strText=str;
		m_grid.SetItem(&Item);
		m_grid.SetItemState(i,1,GVIS_READONLY);		
		m_grid.SetItemState(i,2,GVIS_READONLY);			
	}
	for(i=stk;i<=etk;i++)
	{
		Item.row=i;
		Item.col=0;
		str.Format(L"√ %d",i);
		Item.strText=str;
		m_grid.SetItem(&Item);

		nState = m_grid.GetItemState(i,1);
		m_grid.SetItemState(i,1, nState | GVIS_SELECTED);
		nState = m_grid.GetItemState(i,2);
		m_grid.SetItemState(i,2, nState | GVIS_SELECTED);		
	}
	m_LC=m_grid.GetItemText(stk,1)+"------"+m_grid.GetItemText(etk,2);
	GetDlgItem(IDC_STATIC_LC)->SetWindowText(m_LC);
	m_grid.Invalidate();
}

void TRIMDIAG::TagCombobox()
{
	if(TKLCAy[0]=="NO")
	{
		return;//0项为标记，1 项不用
	}

	CString rString;
	int CurSel,stk,etk;

	CurSel=m_StartCtrl.GetCurSel();
	m_StartCtrl.GetLBText(CurSel,rString);
	stk=int(_wtof(rString)+0.001);

	CurSel=m_EndCtrl.GetCurSel();
	m_EndCtrl.GetLBText(CurSel,rString);
	etk=int(_wtof(rString)+0.001);

	if(stk<1||stk>TKNUM||etk<stk)
	{
		GetDlgItem(IDC_STATIC_LC)->SetWindowText(L"");		
		return;
	}
	int i;
	COLORREF txtbkclr=RGB(0,0,0);
	CString str;
	int nState;
	for(i=1;i<stk;i++)
	{
		Item.row=i;
		Item.col=0;
		str.Format(L"  %d",i);
		Item.strText=str;
		m_grid.SetItem(&Item);
		m_grid.SetItemState(i,1,GVIS_READONLY);		
		m_grid.SetItemState(i,2,GVIS_READONLY);		
	}
	for(i=etk+1;i<=TKNUM;i++)
	{
		Item.row=i;
		Item.col=0;
		str.Format(L"  %d",i);
		Item.strText=str;
		m_grid.SetItem(&Item);
		m_grid.SetItemState(i,1,GVIS_READONLY);		
		m_grid.SetItemState(i,2,GVIS_READONLY);			
	}
	for(i=stk;i<=etk;i++)
	{
		Item.row=i;
		Item.col=0;
		str.Format(L"√ %d",i);
		Item.strText=str;
		m_grid.SetItem(&Item);

		nState = m_grid.GetItemState(i,1);
		m_grid.SetItemState(i,1, nState | GVIS_SELECTED);
		nState = m_grid.GetItemState(i,2);
		m_grid.SetItemState(i,2, nState | GVIS_SELECTED);		
	}
	m_grid.Invalidate();
	m_LC=m_grid.GetItemText(stk,1)+"------"+m_grid.GetItemText(etk,2);
	GetDlgItem(IDC_STATIC_LC)->SetWindowText(m_LC);
}

void TRIMDIAG::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	TagCombobox();
}

void TRIMDIAG::OnSelchangeCombo2() 
{
	// TODO: Add your control notification handler code here
	TagCombobox();
}

void TRIMDIAG::OnBnClickedButtonScan()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	struct resbuf result;
	int rc=RTNORM;
	ACHAR fname[256];

	//////////////////////////////////////////////////////////////////////////
	ACHAR DefaultPath[256],wordir[256],pro[256];
	BAS_DRAW_FUN ob;	
	ob.ReadWorkdir();
	_tcscpy(DefaultPath,Cworkdir);
	_tcscat(DefaultPath,L"\\DATA\\");
	_tcscat(DefaultPath,Pro);
	_tcscat(DefaultPath,L".mdb");
	//获到缺省图框名
	//////////////////////////////////////////////////////////////////////////	

	if ( (ads_getfiled( L"请输入数据库名 *.mdb",DefaultPath,L"mdb",2+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	_tcscpy(fname,result.resval.rstring); 
	m_MdbName=fname;
	if(m_MdbName==""){AfxMessageBox(L"未指定数据库!");return;}
	UpdateData(FALSE);
}

void TRIMDIAG::OnBnClickedCheckDrawzdm()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_IsDrawZDM)
	{
		GetDlgItem(IDC_EDIT_MDBNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_MDBNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
