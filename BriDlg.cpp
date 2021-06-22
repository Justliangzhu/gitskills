// BriDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdarx.h"
#include "BriDlg.h"
#include "GTZDM.h"
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
// BriDlg dialog
BriDlg::BriDlg(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(BriDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(BriDlg)
	// NOTE: the ClassWizard will add member initialization here
	DXorRXD = 1;
	rxdno = -1;
	zdm = NULL;
	//}}AFX_DATA_INIT
}


void BriDlg::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BriDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//	DDX_GridControl(pDX,IDC_List_Bri,m_grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BriDlg, cdxCSizingDialog)
	//{{AFX_MSG_MAP(BriDlg)
	ON_BN_CLICKED(IDC_BUTTONADD, OnButtonadd)
	ON_BN_CLICKED(IDC_BUTTONDEL, OnButtondel)
	ON_BN_CLICKED(IDC_BUTTON_InBase, OnBUTTONInBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void BriDlg::OnButtonadd() //增加
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		CString str;
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

		str="全幅";
		m_grid.SetItemText(nRow,6,str);
		str = "90";
		m_grid.SetItemText(nRow,7,str);
		m_grid.SetItemText(nRow,8,str);
		str = "1";
		m_grid.SetItemText(nRow,9,str);
		m_grid.SetItemText(nRow,10,str);

		m_grid.SetFocusCell(nRow,1);

		m_grid.Invalidate();
		m_BriNum++;
	}	
}

void BriDlg::OnButtondel() //删除
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
		m_BriNum--;
	}	
}

void BriDlg::OnOK() 
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
	pObj->close();

	if (pObj->isKindOf(BRI_ROAD::desc() ) ) //是BRI实体
	{  
		/* acdbOpenObject(pObj, m_eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,m_eId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pBri = BRI_ROAD::cast(pObj);
		pBri->assertWriteEnabled();


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
		// m_NDmd = NRow;  
		m_BriNum = pBri->NBRI = NRow; 
		for(i=1; i<=m_BriNum; i++)
		{
			pBri->Bri[i-1].BriName=m_grid.GetItemText(i, 1);
			//		   _tcscpy(pBri->Bri[i-1].SKml,m_grid.GetItemText(i, 2));
			//		   _tcscpy(pBri->Bri[i-1].EKml,m_grid.GetItemText(i, 3));
			pBri->Bri[i-1].sdml=_wtof(m_grid.GetItemText(i, 2));
			pBri->Bri[i-1].edml=_wtof(m_grid.GetItemText(i, 3));
			pBri->Bri[i-1].KKSY=m_grid.GetItemText(i, 4);
			pBri->Bri[i-1].SWHt=m_grid.GetItemText(i, 5);
			pBri->Bri[i-1].XZ=m_grid.GetItemText(i, 6);
			pBri->Bri[i-1].HeadAng=_wtof(m_grid.GetItemText(i, 7));
			pBri->Bri[i-1].TailAng=_wtof(m_grid.GetItemText(i, 8));
			pBri->Bri[i-1].ZZDK = _wtof(m_grid.GetItemText(i, 9));
			pBri->Bri[i-1].YZDK = _wtof(m_grid.GetItemText(i, 10));
			pBri->Bri[i-1].sml = pzLinep->TYLC(pBri->Bri[i-1].sdml);
			pBri->Bri[i-1].eml = pzLinep->TYLC(pBri->Bri[i-1].edml);
			pBri->Bri[i-1].cml = 0.5*(pBri->Bri[i-1].sml+pBri->Bri[i-1].eml);
			pBri->Bri[i-1].cdml=pzLinep->XLC1(pBri->Bri[i-1].cml);
			CString  GH;
			double xlc = pzLinep->XLC(pBri->Bri[i-1].cml,GH);
			_tcscpy(pBri->Bri[i-1].CKml,pzLinep->LCchr(GH,xlc,3));

		}
		pBri->close();
	}
	acedGetAcadDwgView()->SetFocus();
}

BOOL BriDlg::OnInitDialog() //初始化
{
	cdxCSizingDialog::OnInitDialog(0);

	// TODO: Add extra initialization here
	AcDbObject* pObj = NULL;
	/* acdbOpenObject(pObj,m_eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,m_eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pObj->close();
	pBri = BRI_ROAD::cast(pObj);

	//zdm = pBri->zmLine;
	m_BriNum=pBri->NBRI;

	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=30;
	rect.right-=10;
	rect.bottom-=10;
	m_grid.Create(rect, this, 900);

	m_grid.SetRowCount(m_BriNum+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(11);
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
	m_grid.SetItemText(0,1,L"桥 名");
	m_grid.SetItemText(0,2,L"起始里程");
	m_grid.SetItemText(0,3,L"终止里程");
	m_grid.SetItemText(0,4,L"孔跨式样");
	m_grid.SetItemText(0,5,L"100年水位高程");
	m_grid.SetItemText(0,6,L"性质");
	m_grid.SetItemText(0,7,L"桥头交角");	   
	m_grid.SetItemText(0,8,L"桥尾交角");
	m_grid.SetItemText(0,9,L"左占地宽");
	m_grid.SetItemText(0,10,L"右占地宽");
	int i=0;
	ACHAR tmp[30];
	if(m_BriNum > 0 )  	
	{
		for(i=0; i<m_BriNum; i++)
		{
			m_grid.SetItemText(i+1,1,pBri->Bri[i].BriName);
			_tcscpy(tmp,zdm->DoubleToStr(pBri->Bri[i].sdml));
			m_grid.SetItemText(i+1,2,tmp);
			_tcscpy(tmp,zdm->DoubleToStr(pBri->Bri[i].edml));
			m_grid.SetItemText(i+1,3,tmp);
			m_grid.SetItemText(i+1,4,pBri->Bri[i].KKSY);
			m_grid.SetItemText(i+1,5,pBri->Bri[i].SWHt);
			m_grid.SetItemText(i+1,6,pBri->Bri[i].XZ);
			_tcscpy(tmp,zdm->DoubleToStr(pBri->Bri[i].HeadAng));
			m_grid.SetItemText(i+1,7,tmp);
			_tcscpy(tmp,zdm->DoubleToStr(pBri->Bri[i].TailAng));
			m_grid.SetItemText(i+1,8,tmp);
			_tcscpy(tmp,zdm->DoubleToStr(pBri->Bri[i].ZZDK));
			m_grid.SetItemText(i+1,9,tmp);
			_tcscpy(tmp,zdm->DoubleToStr(pBri->Bri[i].YZDK));
			m_grid.SetItemText(i+1,10,tmp);			   
			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}
	}
	_stprintf(tmp,L"%d",i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.SetColumnWidth(0,50);

	for(i=1; i<8; i++)
		m_grid.SetColumnWidth(i,95);

	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

	AddSzControl(m_grid,mdResize,mdResize);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BriDlg::OnBUTTONInBase() 
{
	// TODO: Add your control notification handler code here
	XLDataBase xlmdb;
	int i;
	CString curprj;
	FILE *fpr;
	//----输入mdb文件名
	xlmdb.ReadWorkdir();

	curprj = Cworkdir + "\\project";   
	if((fpr = _wfopen(curprj,L"r"))==NULL)   
		curprj="";
	else
	{
		fwscanf(fpr,L"%s",curprj);
		fclose(fpr);
		curprj = Cworkdir + "\\data\\" + curprj + ".mdb";//当前方案
	}

	CFileDialog fname(false,NULL,curprj);
	fname.m_ofn.lpstrDefExt = L"MDB";
	fname.m_ofn.lpstrInitialDir=Cworkdir+"\\data";
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库文件(*.MDB)\0*.mdb\0\0";
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
	xlmdb.BriNum = NRow;//m_grid.GetRowCount()-2;  
	if(xlmdb.Bri) {delete []xlmdb.Bri,xlmdb.Bri=NULL;}
	if(xlmdb.BriNum>0)
	{
		xlmdb.Bri = new BRITAB[xlmdb.BriNum];
		for(i=1; i<=xlmdb.BriNum;i++)
		{
			xlmdb.Bri[i-1].BriName=m_grid.GetItemText(i, 1);
			CString str;
			double sml,eml;
			/*			_tcscpy(ckml,m_grid.GetItemText(i, 2));
			double sml = pzLinep->TYLC(ckml);
			zdm->split_ckml(ckml,&ml,GH);
			xlmdb.Bri[i-1].BGH = GH;
			str.Format(L"%0.2lf",ml);*/
			xlmdb.Bri[i-1].StDml = _wtof(m_grid.GetItemText(i, 2));
			/*
			_tcscpy(ckml,m_grid.GetItemText(i, 3));
			double eml = pzLinep->TYLC(ckml);
			xlmdb.split_ckml(ckml,&ml,GH);
			xlmdb.Bri[i-1].EGH = GH;
			str.Format(L"%0.2lf",ml);*/

			xlmdb.Bri[i-1].EdDml = _wtof(m_grid.GetItemText(i, 3));
			sml = pzLinep->TYLC(xlmdb.Bri[i-1].StDml);
			eml = pzLinep->TYLC(xlmdb.Bri[i-1].EdDml);
			double cml = 0.5*(sml+eml);
			//		    double xlc = pzLinep->XLC(cml,gh);
			//			xlmdb.Bri[i-1].CGH = gh;
			//		  	str.Format(L"%0.2lf",xlc);

			xlmdb.Bri[i-1].CenDml = pzLinep->XLC1(cml);
			xlmdb.Bri[i-1].KKSY = m_grid.GetItemText(i, 4);
			xlmdb.Bri[i-1].SWHt = m_grid.GetItemText(i, 5);
			xlmdb.Bri[i-1].XZ=m_grid.GetItemText(i, 6);
			xlmdb.Bri[i-1].HeadAng=_wtof(m_grid.GetItemText(i, 7));
			xlmdb.Bri[i-1].TailAng=_wtof(m_grid.GetItemText(i, 8));
			xlmdb.Bri[i-1].ZZDK=_wtof(m_grid.GetItemText(i, 9));
			xlmdb.Bri[i-1].YZDK=_wtof(m_grid.GetItemText(i, 10));
			double len = eml - sml;
			str.Format(L"%0.2lf",len);
			xlmdb.Bri[i-1].BriLen = _wtof(str);
		}
		ACHAR mes[256];

		if(zdm==NULL)
			xlmdb.Write_XLDbs(curprj,L"大中桥",pzLinep->RoadName,L"全幅");
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
			xlmdb.Write_XLDbs(curprj,L"大中桥",pzLinep->RoadName,zdm->Property);
		}

		_stprintf(mes,L"桥梁数据已写入%s—大中桥表!",curprj);
		ads_alert(mes);
	}
}
