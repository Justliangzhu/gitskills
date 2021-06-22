// JdCenGridCtrl.cpp: implementation of the JdCenGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "JdCenGridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern ads_point ZOOM_P1,ZOOM_P2;
extern AcDbCircle* pCircle;
extern AcDbCircle* pCircle1;
extern AcDbCircle* pCircle2;
BEGIN_MESSAGE_MAP(JdCenGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(JdCenGridCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

JdCenGridCtrl::JdCenGridCtrl()
{
	mode =1;
	m_pzdk = NULL;
}

JdCenGridCtrl::~JdCenGridCtrl()
{

}

void JdCenGridCtrl::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
	//	if(mode==1)
	//	   if( (nRow==1 || nRow==GetRowCount()-1)
	//	  	    && (nCol==1 || nCol>=5 ) )return;
	CGridCtrl::OnEditCell( nRow, nCol, point, nChar);
	CStringArray Items;
	CString strCol=GetItemText(0,nCol);
	CString strCell=GetItemText(nRow,nCol);
	CString strCol1=GetItemText(nRow,1);
	if(strCol=="控制点属性")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"交点");
		Items.Add(L"圆心");
		//      new CInPlaceList(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,crFore,crBack,Items,strCell,nChar);
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	else if(strCol=="转向")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		if(strCol1=="交点")
		{
			Items.Add(L"不回头");
			Items.Add(L"顺向左转回头");
			Items.Add(L"顺向右转回头");
			Items.Add(L"逆向左转回头");
			Items.Add(L"逆向右转回头");
		}
		else
		{
			Items.Add(L"左转");
			Items.Add(L"右转");
		}

		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);		
		//       new CInPlaceList(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,crFore,crBack,Items,strCell,nChar);
	}
	else if(strCol=="后接关系")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"无关");
		Items.Add(L"S型");
		Items.Add(L"C型");
		Items.Add(L"卵型");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
		//        new CInPlaceList(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,crFore,crBack,Items,strCell,nChar);
	}
	else if(strCol=="属性")
	{
		CRect rect;
		GetCellRect(nRow, nCol, rect);		
		Items.RemoveAll();
		Items.Add(L"上限标高");
		Items.Add(L"下限标高");
		Items.Add(L"必经标高");
		new CInPlaceListBox(this, rect,CBS_DROPDOWNLIST,8,nRow, nCol,Items,strCell,nChar);
	}
	Invalidate();
}

void JdCenGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CGridCtrl::OnLButtonDown(nFlags,point);
	//跟踪显示
	if(Gzxs)
	{
		int nRow = GetFocusCell().row;
		int nCol = GetFocusCell().col;
		if(nCol<1)return;
		double x,y;
		CString temp;
		double cml;
		if(mode==1)//交点跟踪
		{
			temp=GetItemText(nRow,N_COL);
			x=_wtof(temp);
			temp=GetItemText(nRow,E_COL);
			y=_wtof(temp);
			if(x==0.0&&y==0.0)return;
			double x1,y1,x2,y2;
			x1=x-100;	y1=y-100;
			x2=x+100;	y2=y+100;

			ZOOM_P1[Y]=x1;ZOOM_P1[X]=y1;
			ZOOM_P2[Y]=x2;ZOOM_P2[X]=y2;
			//acutPrintf(L"...Y=%lf,X=%lf\n",x,y);
			//绘制白色圆标注  半径为3.0
			AcGePoint3d ptCenter;
			ptCenter.set(y,x,0.0);
			double R = 3.0;
			acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
			//直接打开该圆的实体修改圆心即可
			if (pCircle != NULL)
			{
				AcDbEntity * pEnt;
				if (acdbOpenObject(pEnt, pCircle->id(), AcDb::kForWrite) != Acad::eOk)
				{
					pCircle = NULL;
					return;
				}
				else
				{
					if (pEnt->isKindOf(AcDbCircle::desc()))
					{
						pCircle = AcDbCircle::cast(pEnt);
						pCircle->assertWriteEnabled();
						pCircle->setCenter(ptCenter);
						pCircle->close();
					}
				}
				pEnt->close();
			}
			//创建圆  半径为3.0
			else
			{
				AcGeVector3d normal(0,0,1);
				pCircle = new AcDbCircle(ptCenter,normal,R);
				pCircle->setColorIndex(7);
				BAS_DRAW_FUN::AddEntityToDbs(pCircle);
			}

			acDocManager->unlockDocument(acDocManager->curDocument());
		}
		else if(mode==0)//战地点跟踪
		{
			temp=GetItemText(nRow,1);
			double dml = _wtof(temp);

			BAS_DRAW_FUN::xlpoint PZ;
			if(pzLine==NULL)
				return;
			PZ.lc = pzLine->TYLC(dml);
			pzLine->xlpoint_pz(&PZ);
			x=PZ.x,y=PZ.y;
			if(x==0.0&&y==0.0)return;
			ZOOM_P1[Y]=x-50;ZOOM_P1[X]=y-50;
			ZOOM_P2[Y]=x+50;ZOOM_P2[X]=y+50;

			//计算用地桩的左右点
			AcGePoint3d Pt1,Pt2;
			Pt1.set(m_pzdk->pZDK[nRow-1].Lpt[1],m_pzdk->pZDK[nRow-1].Lpt[0],0.0);
			Pt2.set(m_pzdk->pZDK[nRow-1].Rpt[1],m_pzdk->pZDK[nRow-1].Rpt[0],0.0);
			//cml = m_pzdk->pZDK[nRow].cml;
			//绘制白色圆标注  半径为3.0
			double R = 3.0;
			acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
			//直接打开该圆的实体修改圆心即可
			if (pCircle1 != NULL && pCircle2 != NULL)
			{
				AcDbEntity * pEnt;
				if (acdbOpenObject(pEnt, pCircle1->id(), AcDb::kForWrite) != Acad::eOk)
				{
					pCircle1 = NULL;
					return;
				}
				else
				{
					if (pEnt->isKindOf(AcDbCircle::desc()))
					{
						pCircle1 = AcDbCircle::cast(pEnt);
						pCircle1->assertWriteEnabled();
						pCircle1->setCenter(Pt1);
						pCircle1->close();
					}
				}
				pEnt->close();

				if (acdbOpenObject(pEnt, pCircle2->id(), AcDb::kForWrite) != Acad::eOk)
				{
					pCircle2 = NULL;
					return;
				}
				else
				{
					if (pEnt->isKindOf(AcDbCircle::desc()))
					{
						pCircle2 = AcDbCircle::cast(pEnt);
						pCircle2->assertWriteEnabled();
						pCircle2->setCenter(Pt2);
						pCircle2->close();
					}
				}
				pEnt->close();
			}
			//创建圆  半径为3.0
			else
			{
				AcGeVector3d normal(0,0,1);
				pCircle1 = new AcDbCircle(Pt1,normal,R);
				pCircle1->setColorIndex(7);
				BAS_DRAW_FUN::AddEntityToDbs(pCircle1);

				pCircle2 = new AcDbCircle(Pt2,normal,R);
				pCircle2->setColorIndex(7);
				BAS_DRAW_FUN::AddEntityToDbs(pCircle2);
			}
		}
		else if(mode==2)//纵断面控制点
		{
			if(iSelZdm<0)
				return;

			temp=GetItemText(nRow,1);
			double dml = _wtof(temp);
			double Ht = _wtof(GetItemText(nRow,2));
			dml = GlobalZDMTK.ZDMTK[iSelZdm].pm->TYLC(dml);			
			x = (dml-GlobalZDMTK.ZDMTK[iSelZdm].pm->DLArray[0].ELC)/pORI_SCALE.HBVSCALE + GlobalZDMTK.ZDMTK[iSelZdm].X0 ;
			y = Ht-GlobalZDMTK.ZDMTK[iSelZdm].H0 + GlobalZDMTK.ZDMTK[iSelZdm].Y0;	
			ZOOM_P1[Y]=y-20;ZOOM_P1[X]=x-20;
			ZOOM_P2[Y]=y+20;ZOOM_P2[X]=x+20;
		}
		acedGetAcadDockCmdLine()->SetFocus();
		acDocManager->sendStringToExecute(acDocManager->curDocument(),L"PZOOM\n",0);
		SetFocus();
	}
}

void JdCenGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
	CGridCtrl::OnEndEditCell(nRow,nCol,str);
	if(nRow>0 && nCol==CTRLPT_COL)
	{
		if(str=="交点")
		{
			SetItemText(nRow,ZX_COL,L"不回头");
			SetItemText(nRow,LINK_COL,L"无关");
		}
		else if(str=="圆心")
		{
			SetItemText(nRow,ZX_COL,L"左转");
			SetItemText(nRow,LINK_COL,L"无关");
		}
		UpdateData(FALSE);
		Invalidate();
	}
}
