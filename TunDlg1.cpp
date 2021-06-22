// TunDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "TunDlg1.h"
#include "GTZDM.h"
#include "xldatabase.h"
#include "stdarx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
/////////////////////////////////////////////////////////////////////////////
// TunDlg dialog


TunDlg::TunDlg(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(TunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(TunDlg)
	// NOTE: the ClassWizard will add member initialization here
	DXorRXD = 1;
	rxdno = -1;
	zdm = NULL;
	//}}AFX_DATA_INIT
}


void TunDlg::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TunDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//	DDX_GridControl(pDX, IDC_List_Tun,m_grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TunDlg, cdxCSizingDialog)
	//{{AFX_MSG_MAP(TunDlg)
	ON_BN_CLICKED(IDC_BUTTONADD, OnButtonadd)
	ON_BN_CLICKED(IDC_BUTTONDEL, OnButtondel)
	ON_BN_CLICKED(IDC_BUTTON_InBase, OnBUTTONInBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TunDlg message handlers

void TunDlg::OnButtonInDatabase() 
{
	// TODO: Add your control notification handler code here

}

void TunDlg::OnButtonadd() 
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
		m_TunNum++;
	}	
}

void TunDlg::OnButtondel() 
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
		m_grid.Invalidate();
		m_TunNum--;
	}
}

void TunDlg::OnOK() 
{
	// TODO: Add extra validation here
	cdxCSizingDialog::OnOK();
	UpdateData();
	AcDbObject *pObj;

	/*acdbOpenObject(pObj, m_eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,m_eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	if (pObj->isKindOf(TUN_ROAD::desc() ) ) //是Tun实体
	{  
		pObj->close();
		/*  acdbOpenObject(pObj, m_eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,m_eId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pTun = TUN_ROAD::cast(pObj);
		pTun->assertWriteEnabled();
		int NRow;
		NRow=0;
		int i;
		for(i=1; i<m_grid.GetRowCount(); i++)
		{
			int flag;
			m_grid.RowIsEpt(i,m_grid.GetColumnCount(),flag);

			if(flag)//不空
				NRow++;
			else
				break;
		}//统计非空行数
		m_TunNum = pTun->NTun = NRow;//m_grid.GetRowCount()-2;
		for(i=1; i<=m_TunNum; i++)
		{
			pTun->Tun[i-1].TunName=m_grid.GetItemText(i, 1);
			//		   _tcscpy(pTun->Tun[i-1].SKml,m_grid.GetItemText(i, 2));
			//		   _tcscpy(pTun->Tun[i-1].EKml,m_grid.GetItemText(i, 3));
			pTun->Tun[i-1].sdml = _wtof(m_grid.GetItemText(i, 2));
			pTun->Tun[i-1].edml = _wtof(m_grid.GetItemText(i, 3));
			pTun->Tun[i-1].XZ = m_grid.GetItemText(i, 4);
			pTun->Tun[i-1].ZZDK = _wtof(m_grid.GetItemText(i, 5));
			pTun->Tun[i-1].YZDK = _wtof(m_grid.GetItemText(i, 6));
			pTun->Tun[i-1].sml = pzLinep->TYLC(pTun->Tun[i-1].sdml);
			pTun->Tun[i-1].eml = pzLinep->TYLC(pTun->Tun[i-1].edml);
			pTun->Tun[i-1].cml = 0.5*(pTun->Tun[i-1].sml+pTun->Tun[i-1].eml);

			//		   CString  GH;
			//		   double xlc = pzLinep->XLC(pTun->Tun[i-1].cml,GH);
			//		   _tcscpy(pTun->Tun[i-1].CKml,pzLinep->LCchr(GH,xlc,3));
		}
		pTun->close();
	}
}

BOOL TunDlg::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog(0);

	// TODO: Add extra initialization here

	AcDbObject* pObj = NULL;
	/*acdbOpenObject(pObj,m_eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,m_eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pObj->close();
	pTun = TUN_ROAD::cast(pObj);
	m_TunNum=pTun->NTun;
	//daicol=1;
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=50;
	rect.right-=10;
	rect.bottom-=10;
	m_grid.Create(rect, this, 900);

	m_grid.SetRowCount(m_TunNum+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(7);
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
	m_grid.SetItemText(0,1,L"隧道名");
	m_grid.SetItemText(0,2,L"进洞里程");
	m_grid.SetItemText(0,3,L"出洞里程");
	m_grid.SetItemText(0,4,L"性质");
	m_grid.SetItemText(0,5,L"左占地宽");
	m_grid.SetItemText(0,6,L"右占地宽");
	int i=0;
	ACHAR tmp[20];
	if(m_TunNum > 0 )  	
	{
		for(i=0; i<m_TunNum; i++)
		{
			m_grid.SetItemText(i+1,1,pTun->Tun[i].TunName);
			_stprintf(tmp,L"%0.3lf",pTun->Tun[i].sdml);
			m_grid.SetItemText(i+1,2,tmp);
			_stprintf(tmp,L"%0.3lf",pTun->Tun[i].edml);
			m_grid.SetItemText(i+1,3,tmp);

			m_grid.SetItemText(i+1,4,pTun->Tun[i].XZ);
			_stprintf(tmp,L"%0.1lf",pTun->Tun[i].ZZDK);
			m_grid.SetItemText(i+1,5,tmp);
			_stprintf(tmp,L"%0.1lf",pTun->Tun[i].YZDK);
			m_grid.SetItemText(i+1,6,tmp);
			//			   m_grid.SetItemText(i+1,2,pTun->Tun[i].SKml);
			//			   m_grid.SetItemText(i+1,3,pTun->Tun[i].EKml);

			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}
	}
	_stprintf(tmp,L"%d",i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.SetColumnWidth(0,50);

	for(i=1; i<5; i++)
		m_grid.SetColumnWidth(i,90);

	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

	AddSzControl(m_grid,mdResize,mdResize);
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void TunDlg::OnBUTTONInBase() //隧道数据入库
{
	// TODO: Add your control notification handler code here
	XLDataBase xlmdb;
	int i;
	CString curprj;
	//----输入mdb文件名
	CString workdir;
	ACHAR str1[180];
	FILE *fpr;

	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	curprj = workdir + L"\\project";   
	if((fpr = _wfopen(curprj,L"r"))==NULL)   
		curprj="";
	else
	{
		fwscanf(fpr,L"%s",str1);
		curprj=str1;
		fclose(fpr);
		curprj = workdir + "\\data\\" + curprj + ".mdb";//当前方案
	}

	CFileDialog fname(false,NULL,curprj);
	fname.m_ofn.lpstrDefExt = L"MDB";
	fname.m_ofn.lpstrInitialDir=workdir+L"\\data";
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库(*.MDB)\0*.mdb\0\0";
	fname.DoModal();	
	curprj =fname.GetPathName(); 

	int NRow;
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
	xlmdb.TunNum = NRow;//m_grid.GetRowCount()-2;  
	if(xlmdb.Tun) {delete []xlmdb.Tun,xlmdb.Tun=NULL;}
	if(xlmdb.TunNum>0)
	{
		xlmdb.Tun = new TUNTAB[xlmdb.TunNum];
		for(i=1; i<=xlmdb.TunNum;i++)
		{
			xlmdb.Tun[i-1].TunName=m_grid.GetItemText(i, 1);
			CString str;


			//			_tcscpy(ckml,m_grid.GetItemText(i, 2));
			//			_tcscpy(xlmdb.Tun[i-1].SKml,ckml);
			//			zdm->split_ckml(ckml,&ml,GH);
			//  			xlmdb.Tun[i-1].BGH = GH;
			//			str.Format(L"%0.2lf",ml);

			xlmdb.Tun[i-1].StDml = _wtof(m_grid.GetItemText(i, 2));

			//			_tcscpy(ckml,m_grid.GetItemText(i, 3));
			//			_tcscpy(xlmdb.Tun[i-1].EKml,ckml);
			//			xlmdb.split_ckml(ckml,&ml,GH);
			//			xlmdb.Tun[i-1].EGH = GH;
			//			str.Format(L"%0.2lf",ml);

			xlmdb.Tun[i-1].EdDml = _wtof(m_grid.GetItemText(i, 3));
			xlmdb.Tun[i-1].XZ = m_grid.GetItemText(i, 4);
			xlmdb.Tun[i-1].ZZDK = _wtof(m_grid.GetItemText(i, 5));
			xlmdb.Tun[i-1].YZDK = _wtof(m_grid.GetItemText(i, 6));
			xlmdb.Tun[i-1].flag = L"1";//0-表示明洞，1-表示隧道
		}
		ACHAR mes[256];

		if(zdm==NULL)
			xlmdb.Write_XLDbs(curprj,L"隧道",pzLinep->RoadName,L"全幅");
		else
		{
			ACHAR choice[5];
			int rc;
			GetParent()->EnableWindow(TRUE);
			ShowWindow(SW_HIDE);		// Hide our dialog
			GetParent()->SetFocus();	// Give AutoCAD the focus.             
			if(zdm->Property=="")
			{
				ads_initget(RSG_OTHER,L"A a Z z Y y");
				rc = ads_getkword(L"\n全幅坡 (A) / 左幅坡 (Z) /右幅坡: <A>  ", choice);
				if(rc==RTCAN)
					return;
				if(rc==RTNONE)
					_tcscpy(choice ,L"A");
				CString str;
				str = choice;
				str.MakeUpper();
				if(str=="A")
					zdm->Property="全幅";
				else if(str=="Z")
					zdm->Property="左幅";
				else 
					zdm->Property="右幅";
			}
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
			EnableWindow(TRUE); // Enable our dialog*/
			xlmdb.Write_XLDbs(curprj,L"隧道",pzLinep->RoadName,zdm->Property);
		}
		_stprintf(mes,L"隧道数据已写入%s—隧道表!",curprj);
		ads_alert(mes);	   
	}
}
