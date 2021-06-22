// ZDKINPUT.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "ZDKINPUT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COLORREF txtbkclrNE=RGB(185,255,255);
COLORREF readonlykclr=RGB(255,255,185);

extern AcDbCircle* pCircle1;
extern AcDbCircle* pCircle2;
/////////////////////////////////////////////////////////////////////////////
// CZDKINPUT property page

IMPLEMENT_DYNCREATE(CZDKINPUT, CPropertyPage)

	CZDKINPUT::CZDKINPUT(GTZDK_ROAD *pzdk,AcDbObjectId zid) : CPropertyPage(CZDKINPUT::IDD)
{
	//{{AFX_DATA_INIT(CZDKINPUT)
	m_GZXS = FALSE;
	//}}AFX_DATA_INIT
	m_ZdkId = zid;
	m_pzdk = pzdk;

	if(m_pzdk!=NULL)
	{

		m_Nzdk=m_pzdk->NZDK;	   
		m_grid.pzLine=m_pzdk->pLine;
	}
	else
	{
		m_Nzdk = 0;
		m_grid.pzLine=NULL;
	}
	m_grid.mode = 0;

	//打开编辑用地桩的对话框时，将定位圆赋值为NULL
	pCircle1 = NULL;
	pCircle2 = NULL;
}

CZDKINPUT::~CZDKINPUT()
{
	ShanChuDingWeiYuan();
}

void CZDKINPUT::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZDKINPUT)
	//	DDX_Control(pDX, IDC_LIST1, m_grid);
	DDX_Check(pDX, IDC_CHECK_GZXS, m_GZXS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZDKINPUT, CPropertyPage)
	//{{AFX_MSG_MAP(CZDKINPUT)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnButtonDel)
	ON_BN_CLICKED(IDC_CHECK_GZXS, OnCheckGzxs)
	ON_BN_CLICKED(IDC_BUTTON_APP, OnButtonApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZDKINPUT message handlers

void CZDKINPUT::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	//m_GZXS=FALSE;
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

void CZDKINPUT::OnButtonDel() 
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
	ShanChuDingWeiYuan();
	acedGetAcadDwgView()->SetFocus();	
}

void CZDKINPUT::OnCheckGzxs() 
{
	// TODO: Add your control notification handler code here
	m_GZXS = !m_GZXS;
	m_grid.Gzxs=m_GZXS;	
}

void CZDKINPUT::OnButtonApp() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);	

	int i;
	CString str;
	ACHAR ckml[80];
	double dml;

	/*acdbOpenObject(m_pzdk,m_ZdkId,AcDb::kForWrite);*/
	if(acdbOpenObject(m_pzdk,m_ZdkId,AcDb::kForWrite)!=Acad::eOk)	
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	m_pzdk->assertWriteEnabled();

	m_pzdk->NZDK= m_grid.GetRowCount() - 2;
	for(i=0;i<m_pzdk->NZDK;i++)
	{
		str=m_grid.GetItemText(i+1,1);
		dml = _wtof(str);
		m_pzdk->pZDK[i].cml=m_pzdk->pLine->TYLC(dml);
		str=m_grid.GetItemText(i+1,2);
		m_pzdk->pZDK[i].LZDK=_wtof(str);		
		str=m_grid.GetItemText(i+1,3);
		m_pzdk->pZDK[i].RZDK=_wtof(str);		
	}


	m_pzdk->close();
	//	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"regen\n",false,true);
	acDocManager->unlockDocument(acDocManager->curDocument());

	SetFocus();		
}



BOOL CZDKINPUT::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	CPropertyPage::OnInitDialog();
	m_GZXS = FALSE;
	m_grid.Gzxs=m_GZXS;		

	CRect rect;
	GetClientRect(rect);
	rect.left+=5;
	rect.top+=45;
	rect.right-=5;
	rect.bottom-=5;
	m_grid.Create(rect, this, 900);

	COLORREF txtbkclr=RGB(0XFF, 0XFF,0XE0);
	m_grid.SetRowCount(m_Nzdk+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(4);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;
	LOGFONT font={9, 0, 0, 0, 400, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,L"宋体"};

	Item.lfFont=font;

	int i;
	for( i=0;i<m_grid.GetRowCount();i++) 
	{
		int k;
		for(k=0;k<m_grid.GetColumnCount();k++)
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		} 
		m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}

	CWnd *pcwnd; 
	CRect Rect; 
	int width;
	pcwnd = GetDlgItem(IDC_LIST1);
	pcwnd->GetClientRect(&Rect);
	width=Rect.Width()-1.0;	
	m_grid.SetColumnWidth(0,int(width*2/9));
	m_grid.SetColumnWidth(1,int(width*3/9));
	m_grid.SetColumnWidth(2,int(width*2/9));
	m_grid.SetColumnWidth(3,int(width*2/9));

	m_grid.SetItemText(0,0,L"序号");
	m_grid.SetItemText(0,1,L"里程");
	m_grid.SetItemText(0,2,L"左用地宽");
	m_grid.SetItemText(0,3,L"右用地宽");

	m_grid.SetRowHeight(0, DEFAULTCOLHEI);
	ACHAR gh[10],ckml[80],tmp[80];
	CString GH;
	double dml;
	for(i=0; i<m_Nzdk; i++)
	{
		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);	

		dml=m_pzdk->pLine->XLC1(m_pzdk->pZDK[i].cml);
		_stprintf(tmp,L"%0.2lf",dml);
		m_grid.SetItemText(i+1,1,tmp);	
		_stprintf(tmp,L"%0.2lf",m_pzdk->pZDK[i].LZDK);
		m_grid.SetItemText(i+1,2,tmp);		
		_stprintf(tmp,L"%0.2lf",m_pzdk->pZDK[i].RZDK);
		m_grid.SetItemText(i+1,3,tmp);		
	}
	m_grid.Invalidate();
	m_grid.m_pzdk = m_pzdk;
	//	m_grid.AutoSize();	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CZDKINPUT::OnCancel()
{
	//删除定位的圆
	if (pCircle1 != NULL && pCircle2 != NULL)
	{
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		AcDbEntity * pEnt;
		if(acdbOpenObject(pEnt,pCircle1->id(),AcDb::kForWrite)!=Acad::eOk)
		{
			pCircle1 = NULL;
			pCircle2 = NULL;
			CDialog::OnCancel();
			return;
		}
		if (pEnt->isKindOf(AcDbCircle::desc()))
		{
			pCircle1 = AcDbCircle::cast(pEnt);
			pCircle1->assertWriteEnabled();
			pCircle1->erase();
			pCircle1->close();
		}
		pEnt->close();

		if(acdbOpenObject(pEnt,pCircle2->id(),AcDb::kForWrite)!=Acad::eOk)
			return;
		if (pEnt->isKindOf(AcDbCircle::desc()))
		{
			pCircle2 = AcDbCircle::cast(pEnt);
			pCircle2->assertWriteEnabled();
			pCircle2->erase();
			pCircle2->close();
		}
		pEnt->close();
		acDocManager->unlockDocument(acDocManager->curDocument());
		pCircle1 = NULL;
		pCircle2 = NULL;
	}
	CDialog::OnCancel();
}
//删除定位圆
void CZDKINPUT::ShanChuDingWeiYuan()
{
	//删除定位的圆
	if (pCircle1 != NULL && pCircle2 != NULL)
	{
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		AcDbEntity * pEnt;
		if(acdbOpenObject(pEnt,pCircle1->id(),AcDb::kForWrite)!=Acad::eOk)
			return;
		if (pEnt->isKindOf(AcDbCircle::desc()))
		{
			pCircle1 = AcDbCircle::cast(pEnt);
			pCircle1->assertWriteEnabled();
			pCircle1->erase();
			pCircle1->close();
		}
		pEnt->close();

		if(acdbOpenObject(pEnt,pCircle2->id(),AcDb::kForWrite)!=Acad::eOk)
			return;
		if (pEnt->isKindOf(AcDbCircle::desc()))
		{
			pCircle2 = AcDbCircle::cast(pEnt);
			pCircle2->assertWriteEnabled();
			pCircle2->erase();
			pCircle2->close();
		}
		pEnt->close();
		acDocManager->unlockDocument(acDocManager->curDocument());
		pCircle1 = NULL;
		pCircle2 = NULL;
	}
}
