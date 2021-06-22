// EDITLMK.cpp : implementation file
//

#include "stdafx.h"
#include "EDITLMK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEDITLMK dialog
extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;

CEDITLMK::CEDITLMK(CWnd* pParent /*=NULL*/)
	: CDialog(CEDITLMK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEDITLMK)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	BxNum = 0;
	pm=NULL;
}


void CEDITLMK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEDITLMK)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEDITLMK, CDialog)
	//{{AFX_MSG_MAP(CEDITLMK)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEDITLMK message handlers

BOOL CEDITLMK::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=10;
	rect.right-=10;
	rect.bottom-=35;
	m_grid.Create(rect, this, 900);

	COLORREF Readonlyclr=RGB(113,189,193);

	m_grid.SetRowCount(BxNum+1);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(5);
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
		//	   m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}


	m_grid.SetItemText(0,0,L"序 号");
	m_grid.SetItemText(0,1,L"边线性质");
	m_grid.SetItemText(0,2,L"边 侧");
	m_grid.SetItemText(0,3,L"起点里程");
	m_grid.SetItemText(0,4,L"终点里程");

	int i=0,j;
	ACHAR tmp[30];
	double xylen,dms;
	if(BxNum > 0 )  	
	{	
		for(i=0; i<BxNum; i++)
		{
			if(pBXArr[i].bxinf==0)
				m_grid.SetItemText(i+1,1,L"中分带");
			else if(pBXArr[i].bxinf==1)
				m_grid.SetItemText(i+1,1,L"路缘带");
			else if(pBXArr[i].bxinf==2)
				m_grid.SetItemText(i+1,1,L"车行道");
			else if(pBXArr[i].bxinf==3)
				m_grid.SetItemText(i+1,1,L"硬路肩");
			else if(pBXArr[i].bxinf==4)
				m_grid.SetItemText(i+1,1,L"土路肩");
			else
				m_grid.SetItemText(i+1,1,L"硬路肩");

			if(pBXArr[i].LorR==-1)
				m_grid.SetItemText(i+1,2,L"左侧");
			else
				m_grid.SetItemText(i+1,2,L"右侧");

			_stprintf(tmp,L"%0.3lf",pm->XLC1(pBXArr[i].bxsml));
			m_grid.SetItemText(i+1,3,tmp);
			_stprintf(tmp,L"%0.3lf",pm->XLC1(pBXArr[i].bxeml));
			m_grid.SetItemText(i+1,4,tmp);

			_stprintf(tmp,L"%d",i);
			m_grid.SetItemText(i+1,0,tmp);
		}
		_stprintf(tmp,L"%d",i);
		m_grid.SetItemText(i,0,tmp);
	}


	m_grid.SetColumnWidth(0,45);
	for(i=1; i<5; i++)
		m_grid.SetColumnWidth(i,65);
	m_grid.SetColumnWidth(2,60);

	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);	   
	//	   AddSzControl(m_grid,mdResize,mdResize);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEDITLMK::OnBUTTONdel() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	//	if  (nRow==m_grid.GetRowCount()-1) return;
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

void CEDITLMK::OnOK() 
{
	// TODO: Add extra validation here
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);	
	int num;

	int NRow,i,LorR,j;
	NRow=0;
	double scml,ecml,dis;
	CString bxxz;

	for(i=1; i<m_grid.GetRowCount(); i++)
	{
		if(m_grid.GetItemText(i, 2)=="左侧")
			LorR = -1;
		else
			LorR = 1;
		scml = pm->TYLC(_wtof(m_grid.GetItemText(i, 3)));
		ecml = pm->TYLC(_wtof(m_grid.GetItemText(i, 4)));
		bxxz = m_grid.GetItemText(i, 1);
		for(j=0;j<BxNum;j++)
			if(LorR==pBXArr[j].LorR&&fabs(pBXArr[j].bxsml-scml)<0.01&&fabs(pBXArr[j].bxeml-ecml)<0.01)
			{
				AcDbObject *pObj=NULL;
				Acad::ErrorStatus es=acdbOpenObject(pObj, pBXArr[j].eID, AcDb::kForRead);
				if(pObj)
				{
					pObj->close();
					if(pObj->isKindOf(AcDbPolyline::desc()))				
					{  
						BAS_DRAW_FUN::BXLMKD OneBx;
						AcDbPolyline *pPolyline;
						AcGePoint3d pt;
						pPolyline=AcDbPolyline::cast(pObj);				
						int VertCount = pPolyline->numVerts();
						if(VertCount>0)
						{
							double sml,eml,dis,sx,sy,ex,ey;
							pPolyline->getPointAt(0,pt);
							sx = pt.y, sy=pt.x;
							pBXArr[j].bxsml = pm->PROJ_ML(sx, sy);
							pPolyline->getPointAt(VertCount-1,pt);
							ex = pt.y, ey=pt.x;
							pBXArr[j].bxeml = pm->PROJ_ML(ex, ey);

							if(bxxz=="中分带")
								pBXArr[j].bxinf=0;
							else if(bxxz=="路缘带")
								pBXArr[j].bxinf=1;
							else if(bxxz=="车行道")
								pBXArr[j].bxinf=2;
							else if(bxxz=="硬路肩")
								pBXArr[j].bxinf=3;
							else if(bxxz=="土路肩")
								pBXArr[j].bxinf=4;
							else
								pBXArr[j].bxinf=3;
							pBXArr[j].ptnum = VertCount;
							pBXArr[j].ptarr =  new double[pBXArr[j].ptnum*3];
							for(int k=0; k<VertCount;k++)
							{
								pPolyline->getPointAt(k,pt);	
								pBXArr[j].ptarr[k*3] = pt.y;
								pBXArr[j].ptarr[k*3+1] = pt.x;
								pBXArr[j].ptarr[k*3+2] = pm->PROJ_ML(pBXArr[j].ptarr[k*3],pBXArr[j].ptarr[k*3+1]);

							}																
							pBXArr[j].LorR = pm->JudgePTLorR(pBXArr[j].ptarr[0],pBXArr[j].ptarr[1],dis,pm->XYArray,pm->XYNum);									
							NRow++;
						}
					} 
				}
			}	
	}
	FILE *fpw;
	CString mdbname=pm->mdbname;
	mdbname.MakeUpper();
	int pos,len,bxnum,k;
	pos = mdbname.Find(L"DATA");
	len = mdbname.GetLength();
	mdbname=mdbname.Right(len-pos-5);
	CString fname;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	fname =Cworkdir + "\\DATA\\" + "BXLMKD." + mdbname + pm->RoadName;

	fpw = _wfopen(fname, L"w");
	fwprintf(fpw,L"%d\n",NRow);
	for(k=0;k<pBXArr.GetSize();k++)
	{	
		if(pBXArr[k].ptarr)
		{
			fwprintf(fpw,L"%0.3lf %0.3lf %d %d %d\n",pBXArr[k].bxsml,pBXArr[k].bxeml,pBXArr[k].LorR,pBXArr[k].bxinf,pBXArr[k].ptnum);
			for(j=0;j<pBXArr[k].ptnum;j++)
				fwprintf(fpw,L"%0.3lf %0.3lf %0.3lf ",pBXArr[k].ptarr[j*3],pBXArr[k].ptarr[j*3+1],pBXArr[k].ptarr[j*3+2]);
			fwprintf(fpw,L"\n");
		}
	}
	fclose(fpw);
	for(j=0;j<pBXArr.GetSize();j++)
	{
		if(pBXArr[j].ptarr)
		{
			delete []pBXArr[j].ptarr;
			pBXArr[j].ptarr=NULL;
		}
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	CDialog::OnOK();
}
