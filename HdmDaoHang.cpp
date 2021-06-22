// HdmDaoHang.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "HdmDaoHang.h"
#include "Hdmdes.h"
#include "dbmain.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern HdmDes HDM;
extern CHdmDaoHang* daohang;
extern ads_point ZOOM_P1,ZOOM_P2;
/////////////////////////////////////////////////////////////////////////////
// CHdmDaoHang dialog

CHdmDaoHang::CHdmDaoHang(CWnd* pParent /*=NULL*/)
	: CDialog(CHdmDaoHang::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHdmDaoHang)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CHdmDaoHang::~CHdmDaoHang()
{

}
void CHdmDaoHang::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHdmDaoHang)
	DDX_Control(pDX, IDC_XLNX, m_XL);
	DDX_Control(pDX, IDC_LC, m_SeletLC);
	DDX_Control(pDX, IDC_PRE, m_pre);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_LAST, m_last);
	DDX_Control(pDX, IDC_FIST, m_firt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHdmDaoHang, CDialog)
	//{{AFX_MSG_MAP(CHdmDaoHang)
	ON_BN_CLICKED(IDC_FIST, OnFist)
	ON_BN_CLICKED(IDC_LAST, OnLast)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PRE, OnPre)
	ON_CBN_EDITCHANGE(IDC_LC, OnEditchangeLc)
	ON_CBN_SELCHANGE(IDC_LC, OnSelchangeLc)
	ON_CBN_EDITCHANGE(IDC_XLNX, OnEditchangeXlnx)
	ON_CBN_SELCHANGE(IDC_XLNX, OnSelchangeXlnx)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHdmDaoHang message handlers

void CHdmDaoHang::OnFist() 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	//int index;
	CString ckml,xl;
	m_SeletLC.GetLBText(0,ckml);//得到所给索引的字符窜
	m_SeletLC.SetWindowText(ckml);
	m_XL.GetWindowText(xl);
	GengZhong(xl,ckml);	
	xh=0;

}

void CHdmDaoHang::OnLast() 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	int index;
	CString ckml,xl;
	index=m_SeletLC.GetCount();
	m_SeletLC.GetLBText(index-1,ckml);//得到所给索引的字符窜
	m_SeletLC.SetWindowText(ckml);
	m_XL.GetWindowText(xl);
	GengZhong(xl,ckml);	
	xh=index-1;

}

void CHdmDaoHang::OnNext() 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	int index;
	CString ckml,xl;
	//	m_SeletLC.GetWindowText(ckml);
	//	index=m_SeletLC.SelectString(-1,ckml);//从头往后收索
	index=xh++;
	if(index==m_SeletLC.GetCount()-1) {m_SeletLC.GetLBText(index,ckml);AfxMessageBox(L"已经到最后一个断面");}
	else m_SeletLC.GetLBText(index+1,ckml);//得到所给索引的字符窜
	m_SeletLC.SetWindowText(ckml);
	m_XL.GetWindowText(xl);

	GengZhong(xl,ckml);//跟踪显示	

}

void CHdmDaoHang::OnPre() 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	int index;
	CString ckml,xl;
	//	m_SeletLC.GetWindowText(ckml);
	//	index=m_SeletLC.SelectString(-1,ckml);//从头往后收索
	index=xh--;
	if(index==0) {m_SeletLC.GetLBText(index,ckml),AfxMessageBox(L"已经到第一断面");}
	else m_SeletLC.GetLBText(index-1,ckml);//得到所给索引的字符窜
	m_SeletLC.SetWindowText(ckml);
	m_XL.GetWindowText(xl);
	GengZhong(xl,ckml);	

}

void CHdmDaoHang::OnEditchangeLc() 
{
	// TODO: Add your control notification handler code here
	SetFocus();	
	int index;
	CString ckml,xl;
	m_SeletLC.GetWindowText(ckml);
	index=m_SeletLC.SelectString(-1,ckml);//从头往后收索
	xh=index;
	m_XL.GetWindowText(xl);
	GengZhong(xl,ckml);	
}

void CHdmDaoHang::OnSelchangeLc() 
{
	// TODO: Add your control notification handler code here
	//SetFocus();
	int index;
	CString ckml,xl;
	index=m_SeletLC.GetCurSel();
	xh=index;
	m_SeletLC.GetLBText(index,ckml);
	m_XL.GetWindowText(xl);
	GengZhong(xl,ckml);	
}
//跟踪显示
void CHdmDaoHang::GengZhong(CString roadname,CString ckml)
{
	int i;
	//	char mes[80];
	//	char cksml2[40];
	//找到道路
	for(i=0; i<HDM.RoadSum; i++)
		if(HDM.road[i].roadname==roadname)
			break;
	//找到断面
	if(i<HDM.RoadSum)
		for(int j=0; j<HDM.road[i].NCRS; j++)
		{
			CString mlstr;
			mlstr = HDM.road[i].hdmdes[j].ckml;
			if(mlstr==ckml)
			{
				Zoom(HDM.road[i].hdmdes[j].ZXo,HDM.road[i].hdmdes[j].ZYo,HDM.road[i].hdmdes[j].ZX1,HDM.road[i].hdmdes[j].ZY1);
				break;
			}
		}
}

void CHdmDaoHang::OnEditchangeXlnx() 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	CString text,rxd,ckml;
	int i;
	m_XL.GetWindowText(text);
	m_SeletLC.ResetContent();//清空
	//找到道路
	for(i=0; i<HDM.RoadSum; i++)
		if(HDM.road[i].roadname==text)
			break;

	if(i<HDM.RoadSum)
		for(int j=0; j<HDM.road[i].NCRS; j++)
		{
			if((HDM.road[i].hdmdes[j].ZDMXZ==3&&HDM.road[i].hdmdes[j].YDMXZ==3)
				||
				(HDM.road[i].hdmdes[j].ZDMXZ==4&&HDM.road[i].hdmdes[j].YDMXZ==4)
				)
				continue;
			m_SeletLC.AddString(HDM.road[i].hdmdes[j].ckml);
		}				
		m_SeletLC.SetCurSel(0);
		m_SeletLC.GetLBText(0,ckml);

		GengZhong(text,ckml);//跟踪	
		UpdateData(false);
}

void CHdmDaoHang::OnSelchangeXlnx() 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	CString text,rxd,ckml;
	int i,index;
	index=m_XL.GetCurSel();
	m_XL.GetLBText(index,text);
	m_SeletLC.ResetContent();//清空

	//找到道路
	for(i=0; i<HDM.RoadSum; i++)
		if(HDM.road[i].roadname==text)
			break;

	if(i<HDM.RoadSum)
		for(int j=0; j<HDM.road[i].NCRS; j++)
		{
			if(HDM.road[i].hdmdes[j].Lnpt>0 || HDM.road[i].hdmdes[j].Rnpt>0 ||
				HDM.road[i].hdmdes[j].Y_Lnpt>0 || HDM.road[i].hdmdes[j].Y_Rnpt>0)
				m_SeletLC.AddString(HDM.road[i].hdmdes[j].ckml);
		}				
		m_SeletLC.SetCurSel(0);
		m_SeletLC.GetLBText(0,ckml);
		GengZhong(text,ckml);//跟踪	
		//	UpdateData(false);
}

BOOL CHdmDaoHang::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str,m_ckml,xl;
	CString ComStr,rxd,cml;
	CString  fz;
	int i;

	m_pre.SubclassDlgItem(IDC_PRE,this);
	m_pre.SetIcon(IDI_PRE);
	str="向前移动一个断面";
	m_pre.SetTooltipText(&str,TRUE);

	m_firt.SubclassDlgItem(IDC_FIST,this);
	m_firt.SetIcon(IDI_FIRST);
	str="移动到第一个断面";
	m_firt.SetTooltipText(&str,TRUE);

	m_next.SubclassDlgItem(IDC_NEXT,this);
	m_next.SetIcon(IDI_NEXT);
	str="向后移动一个断面";
	m_next.SetTooltipText(&str,TRUE);

	m_last.SubclassDlgItem(IDC_LAST,this);
	m_last.SetIcon(IDI_LAST);
	str="移动到最后一个断面";
	m_last.SetTooltipText(&str,TRUE);
	this->SetWindowText(L"横断面追踪显示");

	for(i=0; i<HDM.RoadSum; i++)
		m_XL.AddString(HDM.road[i].roadname);

	m_XL.SetCurSel(0);
	if(HDM.RoadSum>0)
	{
		i=0;
		for(int j=0;j<HDM.road[0].NCRS;j++)
		{
			if((HDM.road[i].hdmdes[j].ZDMXZ==3&&HDM.road[i].hdmdes[j].YDMXZ==3)
				||
				(HDM.road[i].hdmdes[j].ZDMXZ==4&&HDM.road[i].hdmdes[j].YDMXZ==4)
				)
				continue;
			m_SeletLC.AddString(HDM.road[i].hdmdes[j].ckml);
		}
		m_SeletLC.SetCurSel(0);
		xh=0;

		if(HDM.road[0].NCRS>0)
		{
			//20190715	后面会导航到第一个平面，这里全部视窗操作会被覆盖，不需要
//#ifdef VERSION_CAD2016
//			acedCommandS(RTSTR, "zoom", RTSTR, "e", RTNONE);
//#endif // VERSION_CAD2016
//#ifdef VERSION_CAD2010
//			ads_command(RTSTR, L"zoom", RTSTR, L"e", RTNONE);
//#endif // VERSION_CAD2010

			Zoom(HDM.road[0].hdmdes[0].ZXo,HDM.road[0].hdmdes[0].ZYo,HDM.road[0].hdmdes[0].ZX1,HDM.road[0].hdmdes[0].ZY1);
		}

	}

	return TRUE;  
	// TODO: Add extra initialization here

	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*
void CHdmDaoHang::SurePrj(CString &PrjFname)
{
CString WorkDir,Pro;
FILE *fpr;
fpr=_wfopen(L"c:\\workdir","r");
if(!fpr)
{
AfxMessageBox(L"您还没有设定项目名称!");
return ;
}

char fname1[256];
fwscanf(fpr,"%s",fname1);
WorkDir = fname1;
fclose(fpr);

CString ProFname;
char pro[256];
ProFname = WorkDir+"\\project"; 
fpr = _wfopen(ProFname,"r");
if(!fpr)
{
AfxMessageBox(L"您还没有指定横断面设计方案编号!");
return ;
}
fwscanf(fpr,"%s",pro);
Pro = pro;
fclose(fpr);
//ads_printf(L"当前横断面方案号: %s\n",Pro);
PrjFname =  WorkDir+ "\\data\\" + Pro + ".mdb";
return;
}*/

//得到一线里程
//DEL void CHdmDaoHang::SetYXCkml()
//DEL {
//DEL 	int i;
//DEL 	SurePrj(PrjFname);
//DEL 	if(xlmdb.Read_XLDbs(PrjFname,"填挖表"))
//DEL 	{
//DEL 		hdmdes.NCRS=xlmdb.NTW;
//DEL 		for(i=0;i<hdmdes.NCRS;i++)
//DEL 		{
//DEL 			_tcscpy(hdmdes.Crs[i].GH,xlmdb.TWB[i].GH);
//DEL 			hdmdes.Crs[i].dml=xlmdb.TWB[i].ml;
//DEL 			dmltockml(hdmdes.Crs[i].GH,hdmdes.Crs[i].dml,hdmdes.Crs[i].ckml);	
//DEL 		}
//DEL 	}
//DEL 	if(xlmdb.TWB){delete []xlmdb.TWB;xlmdb.TWB=NULL;}
//DEL }
void CHdmDaoHang::Zoom(double x1,double y1,double x2,double y2)
{
	BAS_DRAW_FUN::Zoom(x1, y1, x2, y2);
}

//DEL void CHdmDaoHang::OnDestroy() 
//DEL {
//DEL 	CDialog::OnDestroy();
//DEL 	if(daohang){daohang=NULL;}
//DEL 	EnableWindow(false);
//DEL 	acedGetAcadDwgView()->SetFocus;
//DEL 	
//DEL 	// TODO: Add your message handler code here
//DEL 	
//DEL }

void CHdmDaoHang::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
	if(daohang){daohang=NULL;}
	EnableWindow(false);
	acedGetAcadDwgView()->SetFocus();
	DestroyWindow();	
}
void CHdmDaoHang::Close()
{
	OnClose();
}
void CHdmDaoHang::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
	delete this;
}
