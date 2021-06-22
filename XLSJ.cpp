 // XLSJ.cpp : implementation file
//

#include "stdafx.h"
#include "XLSJ.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// XLSJ dialog
//extern int CheckProInProA(int pro);

extern XLSJ xlsjDlg;


XLSJ::XLSJ(CWnd* pParent /*=NULL*/)
	: CDialog(XLSJ::IDD, pParent)
{
	//{{AFX_DATA_INIT(XLSJ)
	m_A2L2 = 0.0;
	m_A1L1 = 0.0;
	m_R = 0.0;
	m_IfA = true;
	m_curActiveJD=0;
	m_ModifyOldGTZX=1;
	pGtzx=NULL;
	pGtzxIN=NULL;
	IsModifyOldGTZX=false;//plh07.19a
	m_LinkRelation = _T("无关");
	//}}AFX_DATA_INIT
}

void XLSJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XLSJ)
	DDX_Control(pDX, IDC_INXL, m_ButtonInXL);
	DDX_Control(pDX, IDC_OUTXL, m_ButtonOutXL);	
	DDX_Text(pDX, IDC_EDIT_Lo2, m_A2L2);
	DDX_Text(pDX, IDC_EDIT_Lo1, m_A1L1);
	DDX_Text(pDX, IDC_EDIT_R, m_R);
	DDX_Radio(pDX, IDC_RADIO_REPLACE,m_ModifyOldGTZX);
	DDX_CBString(pDX, IDC_COMBO_relation, m_LinkRelation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XLSJ, CDialog)
	//{{AFX_MSG_MAP(XLSJ)
	ON_BN_CLICKED(IDC_SEL_CIR, OnSelCir)
	ON_BN_CLICKED(IDC_SEL_QX, OnSelQx)
	ON_BN_CLICKED(IDC_INXL, OnInxl)
	ON_BN_CLICKED(IDC_OUTXL, OnOutxl)
	ON_BN_CLICKED(IDC_RADIO_REPLACE, OnRadioReplace)
	ON_BN_CLICKED(IDC_RADIO_NEWGTZX, OnRadioNewgtzx)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_BUTTON_update, OnBUTTONupdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XLSJ message handlers
void XLSJ::OnSelCir()//选择圆
{
	//	ShowWindow(FALSE);	
	m_A1L1=m_A2L2=0.0;
	m_LinkRelation = _T("无关");
	UpdateData(false);
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	// TODO: Add your control notification handler code here
	if(pGtzx==NULL)
	{
		MessageBox(L"请先选择线路或已选该项目!!!",NULL,MB_OK);		
		return;
	}
	//input JDGH,LCGH//plh07.19a
	ACHAR JDGH[20],LCGH[20];
	xjj=0.0;
	if(!IsModifyOldGTZX)
	{	
		acedGetReal(L"请输入偏距(0.0):",&xjj);
		acedGetString(0,L"请输入交点冠号(JD):",JDGH);
		acedGetString(0,L"请输入里程冠号(AK):",LCGH);

		if(fabs(xjj)<PRECISION)xjj=0.0;
		if(_tcscmp(JDGH,L"")==0)_tcscpy(JDGH,L"JD");				
		if(_tcscmp(LCGH,L"")==0)_tcscpy(LCGH,L"K");
	}

	AcGePoint3d Pt1,Pt2;//最后两个交点
	AcDbObject *pObj;
	if(Acad::eOk==acdbOpenObject(pObj, lId, AcDb::kForWrite))
	{		
		pGtzx=JD_CENTER::cast(pObj);
		pGtzx->assertWriteEnabled();

		double OutX,OutY,TX,TY;//传入一切线起始点，同时会返回另一切线起始点
		int iJD1,iJD2;//前后交点
		iJD1=pGtzx->GetPreJD(PT.y,PT.x);
		iJD2=iJD1+1;

		Pt1.x=pGtzx->JdCenArray[iJD1].JDE;
		Pt1.y=pGtzx->JdCenArray[iJD1].JDN;
		Pt2.x=pGtzx->JdCenArray[iJD2].JDE;
		Pt2.y=pGtzx->JdCenArray[iJD2].JDN;

		double fwj=pGtzx->JdCenArray[iJD1].AftFwj;
		fwj+=0.5*PI;
		Pt1.x+=xjj*sin(fwj);
		Pt1.y+=xjj*cos(fwj);

		Pt2.x+=xjj*sin(fwj);
		Pt2.y+=xjj*cos(fwj);

		OutX=Pt1.x;
		OutY=Pt1.y;
		TX=Pt2.x;
		TY=Pt2.y;

		pGtzx->close();
		double xs,ys,x0,y0,xe,ye,cenX,cenY;		
		AcDbObjectIdArray eraseId;

		double rad=GetRFromT_P_P(Pt1.x,Pt1.y,Pt2.x,Pt2.y,xs,ys,x0,y0,xe,ye,cenX,cenY,eraseId,false);

		if(rad>0)//plh04.14
		{
			m_R = rad;
			UpdateData(false);		

			GetTangent(cenX,cenY,rad,OutX,OutY,TX,TY);

			AcGePoint3dArray interPt;
			AcDbLine L1(Pt1,Pt2);
			AcDbLine L2(AcGePoint3d(OutX,OutY,0.0),AcGePoint3d(TX,TY,0.0));
			if(Acad::eOk==L1.intersectWith(&L2,AcDb::kExtendBoth,interPt,0,0)&&interPt.length()>0)
			{
				UpdateData();
				if(IsModifyOldGTZX)//修改原方案
				{
					/*acdbOpenObject(pGtzx, lId, AcDb::kForWrite);*/

					if(acdbOpenObject(pObj,lId,AcDb::kForWrite)!=Acad::eOk)	
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					pGtzx=JD_CENTER::cast(pObj);
					pGtzx->assertWriteEnabled();
					pGtzx->JdCenArray.RemoveAt(iJD2,pGtzx->JdCenNum-iJD2);
					//尾部增加两个交点

					JdCenter JD1;
					JD1.JDXZ=IsJiaoDian;
					JD1.N=interPt[0].y;
					JD1.E=interPt[0].x;
					JD1.IsOrNotHuiTou=false;
					//JD1.R=rad;
					//JD1.A1=0.0;
					//JD1.A2=0.0;

					JD1.R=m_R;
					if(m_IfA)//A
					{
						JD1.A1=m_A1L1;
						JD1.A2=m_A2L2;
					}
					else
					{
						JD1.A1=sqrt(m_A1L1*m_R);
						JD1.A2=sqrt(m_A2L2*m_R);
					}

					JD1.LinkAfter=NotRelation;
					pGtzx->JdCenArray.Add(JD1);
					JdCenter JD2;
					JD2.JDXZ=IsJiaoDian;
					JD2.N=OutY;
					JD2.E=OutX;
					pGtzx->JdCenArray.Add(JD2);
					pGtzx->JdCenNum=pGtzx->JdCenArray.GetSize();

					if(pGtzx->JdCenNum>2)
					{
						if(m_LinkRelation=="S形")//与前一曲线S
							pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = S_Link;
						else if(m_LinkRelation=="C形")
							pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = C_Link;
						else if(m_LinkRelation=="卵形")
							pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = Luan_Link;

					}
					pGtzx->close();

					PT.x=0.5*(interPt[0].x+OutX);
					PT.y=0.5*(interPt[0].y+OutY);
				}
				else//新建方案
				{
					JdCenter *JDAy=new JdCenter[3];
					int i=0;

					JDAy[i].JDXZ=IsJiaoDian;
					JDAy[i].N=Pt1.y;
					JDAy[i].E=Pt1.x;
					JDAy[i].A1=0.0;
					JDAy[i].A2=0.0;
					JDAy[i].R=0.0;
					JDAy[i].IsOrNotHuiTou=false;
					JDAy[i].LinkAfter=NotRelation;


					i=1;
					JDAy[i].JDXZ=IsJiaoDian;
					JDAy[i].N=interPt[0].y;
					JDAy[i].E=interPt[0].x;
					//	JDAy[i].A1=0.0;
					//	JDAy[i].A2=0.0;
					//	JDAy[i].R=rad;
					JDAy[i].R=m_R;
					if(m_IfA)//A
					{
						JDAy[i].A1=m_A1L1;
						JDAy[i].A2=m_A2L2;
					}
					else
					{
						JDAy[i].A1=sqrt(m_A1L1*m_R);
						JDAy[i].A2=sqrt(m_A2L2*m_R);
					}

					JDAy[i].IsOrNotHuiTou=false;
					JDAy[i].LinkAfter=NotRelation;

					i=2;
					JDAy[i].JDXZ=IsJiaoDian;
					JDAy[i].N=OutY;
					JDAy[i].E=OutX;
					JDAy[i].A1=0.0;
					JDAy[i].A2=0.0;
					JDAy[i].R=0.0;
					JDAy[i].IsOrNotHuiTou=false;
					JDAy[i].LinkAfter=NotRelation;

					JD_CENTER *newGtzx=new JD_CENTER;
					newGtzx->SetJDGH(JDGH);
					newGtzx->SetLCGH(LCGH);
					newGtzx->SetJdCenterArray(JDAy,3);
					newGtzx->CalXYArray();
					newGtzx->setLayer(L"公路平面设计专用层");

					AcDbObjectId tempId=AddEntityToDbs(newGtzx);

					//起点里程取整
					/*acdbOpenObject(pGtzx,lId,AcDb::kForRead);*/

					if(acdbOpenObject(pObj, lId, AcDb::kForRead)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					pGtzx=JD_CENTER::cast(pObj); 
					/*acdbOpenObject(newGtzx,tempId,AcDb::kForWrite);*/
					
					if(acdbOpenObject(pObj,tempId,AcDb::kForWrite)!=Acad::eOk)	
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					newGtzx=JD_CENTER::cast(pObj);
					newGtzx->assertWriteEnabled();

					xlpoint tempPt;
					double startN,startE;

					if(newGtzx->XYArray[1][0]>1.5)
					{
						startN=newGtzx->XYArray[1][8];
						startE=newGtzx->XYArray[1][9];
					}
					else
					{
						startN=newGtzx->XYArray[2][8];
						startE=newGtzx->XYArray[2][9];
					}
					double tylc=pGtzx->PROJ_ML(startN,startE);
					tempPt.lc=pGtzx->LocateHundredMeterOnLine(tylc,true);//找ZH前面整百米
					pGtzx->xlpoint_pz(&tempPt);

					tempPt.x+=xjj*cos(fwj);
					tempPt.y+=xjj*sin(fwj);

					newGtzx->JdCenArray[0].N=tempPt.x;
					newGtzx->JdCenArray[0].E=tempPt.y;

					newGtzx->DLArray[0].BLC=pGtzx->XLC(tempPt.lc,pGtzx->DLArray[0].BGH);
					newGtzx->DLArray[0].ELC=pGtzx->DLArray[0].BLC;
					newGtzx->DLArray[0].EGH=pGtzx->DLArray[0].BGH;
					newGtzx->JdCenArray[0].A1=pGtzx->DLArray[0].ELC;
					newGtzx->close();					
					pGtzx->close();
					pGtzx=newGtzx;
					lId=tempId;
					PT.x=0.5*(interPt[0].x+OutX);
					PT.y=0.5*(interPt[0].y+OutY);
				}
			}
		}
		//plh07.19;
		/*		acdbOpenObject(pText,eraseId[0],AcDb::kForWrite,Adesk::kFalse);
		pText->erase();
		pText->close();
		acdbOpenObject(pArc,eraseId[1],AcDb::kForWrite,Adesk::kFalse);
		pArc->erase();
		pArc->close();
		*/		
		if(eraseId.length()>0)
		{
			for(int k=0;k<eraseId.length();k++)
			{
				AcDbEntity *pEnt;
				if(acdbOpenObject(pEnt,eraseId[k],AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
				{
					pEnt->erase();
					pEnt->close();
				}

			}
		}
	}
	//	ShowWindow(TRUE);
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
//	IsModifyOldGTZX=true;
	m_ModifyOldGTZX=0;
	UpdateData();	
}

void XLSJ::OnSelQx() //选择切线
{
	//	ShowWindow(FALSE);
	m_A1L1=m_A2L2=0.0;
	UpdateData(false);
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	// TODO: Add your control notification handler code here
	AcGePoint3d PT1,PT2;
	if(pGtzx==NULL)
	{
		MessageBox(L"请先选择线路或已选该项目!!!",NULL,MB_OK);		
		return;
	}

	//input JDGH,LCGH//plh07.19a
	ACHAR JDGH[20],LCGH[20];
	xjj=0.0;
	if(!IsModifyOldGTZX)
	{
		acedGetReal(L"请输入偏距(0.0):",&xjj);
		acedGetString(0,L"请输入交点冠号(JD):",JDGH);
		acedGetString(0,L"请输入里程冠号(AK):",LCGH);

		if(fabs(xjj)<PRECISION)xjj=0.0;
		if(_tcscmp(JDGH,L"")==0)_tcscpy(JDGH,L"JD");				
		if(_tcscmp(LCGH,L"")==0)_tcscpy(LCGH,L"K");
	}
	//plh04.14
	if(	RTNORM==acedGetPoint(NULL,L"确定切线起点:\n",asDblArray(PT1))
		&&RTNORM==acedGetPoint(asDblArray(PT1),L"确定切线终点:\n",asDblArray(PT2)))
	{		
		AcGePoint3d Pt1,Pt2;//后两个交点

		AcDbObject *pObj;
		if(acdbOpenObject(pObj,lId,AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pGtzx=JD_CENTER::cast(pObj);


		int iJD1,iJD2;//前后交点
		iJD1=pGtzx->GetPreJD(PT.y,PT.x);
		iJD2=iJD1+1;
		Pt1.x=pGtzx->JdCenArray[iJD1].JDE;
		Pt1.y=pGtzx->JdCenArray[iJD1].JDN;
		Pt2.x=pGtzx->JdCenArray[iJD2].JDE;
		Pt2.y=pGtzx->JdCenArray[iJD2].JDN;
		double T2=pGtzx->JdCenArray[iJD1].T2;//后切长
		double fwj1=fwj(Pt1.x,Pt1.y,Pt2.x,Pt2.y);
		getLineEndPt(Pt1,T2,fwj1,Pt1);
		pGtzx->close();
		AcGePoint3d interPt;
		double fwj=pGtzx->JdCenArray[iJD1].AftFwj;
		fwj+=0.5*PI;
		Pt1.x+=xjj*sin(fwj);
		Pt1.y+=xjj*cos(fwj);

		Pt2.x+=xjj*sin(fwj);
		Pt2.y+=xjj*cos(fwj);

		AcDbLine L1(Pt1,Pt2);
		AcDbLine L2(PT1,PT2);
		if(BAS_DRAW_FUN::INTERS(&interPt.x,&interPt.y,Pt1.x,Pt1.y,Pt2.x,Pt2.y,PT1.x,PT1.y,PT2.x,PT2.y)>=0)
		{
			double rad=GetRFromT1T2(Pt1.x,Pt1.y,interPt.x,interPt.y,PT2.x,PT2.y,TRUE,350,0,1);
			if(rad>0)
			{
				m_R = rad;
				UpdateData(false);
				UpdateData();
				if(IsModifyOldGTZX)//修改原方案
				{			
					//修改交点参数
					AcDbObject *pObj;
					if(acdbOpenObject(pObj,lId,AcDb::kForWrite)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					pGtzx=JD_CENTER::cast(pObj);
					
					pGtzx->assertWriteEnabled();

					pGtzx->JdCenArray.RemoveAt(iJD2,pGtzx->JdCenNum-iJD2);
					//尾部增加两个交点
					JdCenter JD1;
					JD1.JDXZ=IsJiaoDian;
					JD1.N=interPt.y;
					JD1.E=interPt.x;
					JD1.IsOrNotHuiTou=false;
					//	JD1.R=rad;
					//	JD1.A1=0.0;
					//	JD1.A2=0.0;
					JD1.R=m_R;
					if(m_IfA)//A
					{
						JD1.A1=m_A1L1;
						JD1.A2=m_A2L2;
					}
					else
					{
						JD1.A1=sqrt(m_A1L1*m_R);
						JD1.A2=sqrt(m_A2L2*m_R);
					}



					JD1.LinkAfter=NotRelation;
					pGtzx->JdCenArray.Add(JD1);
					JdCenter JD2;
					JD2.JDXZ=IsJiaoDian;
					JD2.N=PT2.y;
					JD2.E=PT2.x;
					pGtzx->JdCenArray.Add(JD2);
					pGtzx->JdCenNum=pGtzx->JdCenArray.GetSize();
					if(pGtzx->JdCenNum>2)
					{
						if(m_LinkRelation=="S形")//与前一曲线S
							pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = S_Link;
						else if(m_LinkRelation=="C形")
							pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = C_Link;
						else if(m_LinkRelation=="卵形")
							pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = Luan_Link;

					} 
					pGtzx->close();

					PT.x=0.5*(interPt.x+PT2.x);
					PT.y=0.5*(interPt.y+PT2.y);
					
				}
				else//新建方案
				{
					JdCenter *JDAy=new JdCenter[3];
					int i=0;

					JDAy[i].JDXZ=IsJiaoDian;
					JDAy[i].N=Pt1.y;
					JDAy[i].E=Pt1.x;
					JDAy[i].A1=0.0;
					JDAy[i].A2=0.0;
					JDAy[i].R=0.0;
					JDAy[i].IsOrNotHuiTou=false;
					JDAy[i].LinkAfter=NotRelation;

					i=1;
					JDAy[i].JDXZ=IsJiaoDian;
					JDAy[i].N=interPt.y;
					JDAy[i].E=interPt.x;
					//	JDAy[i].A1=0.0;
					//	JDAy[i].A2=0.0;
					//	JDAy[i].R=rad;
					JDAy[i].R=m_R;
					if(m_IfA)//A
					{
						JDAy[i].A1=m_A1L1;
						JDAy[i].A2=m_A2L2;
					}
					else
					{
						JDAy[i].A1=sqrt(m_A1L1*m_R);
						JDAy[i].A2=sqrt(m_A2L2*m_R);
					}

					JDAy[i].IsOrNotHuiTou=false;
					JDAy[i].LinkAfter=NotRelation;

					i=2;
					JDAy[i].JDXZ=IsJiaoDian;
					JDAy[i].N=PT2.y;
					JDAy[i].E=PT2.x;		
					JDAy[i].A1=0.0;
					JDAy[i].A2=0.0;
					JDAy[i].R=0.0;
					JDAy[i].IsOrNotHuiTou=false;
					JDAy[i].LinkAfter=NotRelation;


					JD_CENTER *newGtzx=new JD_CENTER;
					newGtzx->SetJDGH(JDGH);
					newGtzx->SetLCGH(LCGH);
					newGtzx->SetJdCenterArray(JDAy,3);
					newGtzx->CalXYArray();
					newGtzx->setLayer(L"公路平面设计专用层");

					AcDbObjectId tempId=AddEntityToDbs(newGtzx);
					delete []JDAy;
					/*
					for(i=0;i<3;i++)
					{
					ads_printf(L"%lf %lf %lf %d\n",JDAy[i].N,JDAy[i].E,JDAy[i].R,JDAy[i].IsOrNotHuiTou);
					}*/

					/*
					//起点里程取整
					acdbOpenObject(newGtzx,tempId,AcDb::kForWrite);
					xlpoint tempPt;
					double tylc=pGtzx->PROJ_ML(newGtzx->XYArray[2][8],newGtzx->XYArray[2][9]);
					tempPt.lc=pGtzx->LocateHundredMeterOnLine(tylc,true);//找ZH前面整百米
					pGtzx->xlpoint_pz(&tempPt);

					tempPt.x+=xjj*cos(fwj);
					tempPt.y+=xjj*sin(fwj);

					newGtzx->JdCenArray[0].N=tempPt.x;
					newGtzx->JdCenArray[0].E=tempPt.y;

					newGtzx->DLArray[0].BLC=pGtzx->XLC(tempPt.lc,newGtzx->DLArray[0].BGH);
					newGtzx->DLArray[0].ELC=newGtzx->DLArray[0].BLC;
					newGtzx->DLArray[0].EGH=newGtzx->DLArray[0].BGH;
					newGtzx->JdCenArray[0].A1=newGtzx->DLArray[0].ELC;
					newGtzx->close();*/


					pGtzx =newGtzx;
					lId=tempId;//plh04.27

					PT.x=0.5*(interPt.x+PT2.x);
					PT.y=0.5*(interPt.y+PT2.y);
				}
			}
		}

	}
	//	ShowWindow(TRUE);
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();	
	IsModifyOldGTZX=true;
	m_ModifyOldGTZX=0;
	UpdateData();	
}


void XLSJ::OnInxl() 
{
	//	ShowWindow(FALSE);	
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	// TODO: Add your control notification handler code here
	int rc;
	ads_name en;
	AcGePoint3d PTnew;
	rc = ads_entsel(L"选择接入的目标线路实体\n", en, asDblArray(PTnew));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }
	acdbGetObjectId(lIdIN, en);    
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, lIdIN, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, lIdIN, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();

	if (pObj->isKindOf(JD_CENTER::desc())) 
	{
		/*acdbOpenObject(pGtzxIN, lIdIN, AcDb::kForRead);*/

		if(acdbOpenObject(pObj, lIdIN, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pGtzxIN=JD_CENTER::cast(pObj);
		pGtzxIN->close();  // Finished with the pline header.

		xjj=0.0;
		acedGetReal(L"请输入偏距(0.0):",&xjj);
		if(fabs(xjj)<PRECISION)xjj=0.0;

		int iJD1,iJD2;
		int INiJD1,INiJD2;

		iJD1=pGtzx->GetPreJD(PT.y,PT.x);
		iJD2=iJD1+1;

		INiJD1=pGtzxIN->GetPreJD(PTnew.y,PTnew.x);
		INiJD2=INiJD1+1;
		///
		AcGePoint3d Pt1(pGtzx->JdCenArray[iJD1].JDE,pGtzx->JdCenArray[iJD1].JDN,0.0);
		AcGePoint3d Pt2(pGtzx->JdCenArray[iJD2].JDE,pGtzx->JdCenArray[iJD2].JDN,0.0);
		AcGePoint3d Pt3(pGtzxIN->JdCenArray[INiJD1].JDE,pGtzxIN->JdCenArray[INiJD1].JDN,0.0);
		AcGePoint3d Pt4(pGtzxIN->JdCenArray[INiJD2].JDE,pGtzxIN->JdCenArray[INiJD2].JDN,0.0);

		double T2=pGtzx->JdCenArray[iJD1].T2;//后切长
		double fwj1=fwj(Pt1.x,Pt1.y,Pt2.x,Pt2.y);
		getLineEndPt(Pt1,T2,fwj1,Pt1);
		double T1=pGtzxIN->JdCenArray[INiJD2].T1;//前切长//
		double fwj2=fwj(Pt4.x,Pt4.y,Pt3.x,Pt3.y);
		getLineEndPt(Pt4,T1,fwj2,Pt4);

		AcDbLine L1(Pt1,Pt2);
		AcDbLine L2(Pt3,Pt4);

		double fwj=pGtzxIN->JdCenArray[INiJD1].AftFwj;
		fwj+=0.5*PI;
		Pt2.x+=xjj*sin(fwj);
		Pt3.y+=xjj*cos(fwj);

		Pt4.x+=xjj*sin(fwj);
		Pt4.y+=xjj*cos(fwj);

		AcGePoint3d interPt;
		if(BAS_DRAW_FUN::INTERS(&interPt.x,&interPt.y,Pt1.x,Pt1.y,Pt2.x,Pt2.y,Pt3.x,Pt3.y,Pt4.x,Pt4.y)>=0)		
		{

			double rad=GetRFromT1T2(Pt1.x,Pt1.y,interPt.x,interPt.y,Pt4.x,Pt4.y,true,350.0,0,12);

			if(rad>0)
			{
				m_R = rad;
				UpdateData(false);
				//	if(IsModifyOldGTZX)//修改原方案
				UpdateData();
				{			
					//修改交点参数
					if(Acad::eOk==acdbOpenObject(pObj, lId, AcDb::kForWrite))
					{
						pGtzx=JD_CENTER::cast(pObj);
						pGtzx->assertWriteEnabled();
						JdCenter *JdCen=&(pGtzx->JdCenArray[pGtzx->JdCenNum-1]);
						JdCen->JDXZ=IsJiaoDian;
						JdCen->N=interPt.y;
						JdCen->E=interPt.x;
						JdCen->JDN=interPt.y;
						JdCen->JDE=interPt.x;
						//	JdCen->A1=0.0;
						//	JdCen->R=rad;
						//	JdCen->A2=0.0;

						JdCen->R=m_R;
						if(m_IfA)//A
						{
							JdCen->A1=m_A1L1;
							JdCen->A2=m_A2L2;
						}
						else
						{
							JdCen->A1=sqrt(m_A1L1*m_R);
							JdCen->A2=sqrt(m_A2L2*m_R);
						}

						JdCen->IsOrNotHuiTou=false;
						JdCen->LinkAfter=NotRelation;
						//尾部增加一个交点
						JdCenter TailJdCen;
						TailJdCen.JDXZ=IsJiaoDian;
						TailJdCen.N=Pt4.y;
						TailJdCen.E=Pt4.x;
						TailJdCen.A1=0.0;
						TailJdCen.A2=0.0;
						TailJdCen.R=0.0;
						TailJdCen.IsOrNotHuiTou=false;
						TailJdCen.LinkAfter=NotRelation;
						pGtzx->JdCenArray.Add(TailJdCen);
						pGtzx->JdCenNum=pGtzx->JdCenArray.GetSize();
						if(pGtzx->JdCenNum>2)
						{
							if(m_LinkRelation=="S形")//与前一曲线S
								pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = S_Link;
							else if(m_LinkRelation=="C形")
								pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = C_Link;
							else if(m_LinkRelation=="卵形")
								pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = Luan_Link;

						}

						pGtzx->CalXYArray();
						/*acdbOpenObject(pGtzxIN, lIdIN, AcDb::kForRead);*/
						if(acdbOpenObject(pObj, lIdIN, AcDb::kForRead)!=Acad::eOk)
						{
							ads_printf(L"打开实体失败！\n");
							return;
						}
						pGtzxIN=JD_CENTER::cast(pObj);
						//终点里程取整
						xlpoint tempPt;
						double endN,endE;
						if(pGtzx->XYArray[pGtzx->XYNum-1][0]>1.5)
						{
							endN=pGtzx->XYArray[pGtzx->XYNum][8];
							endE=pGtzx->XYArray[pGtzx->XYNum][9];
						}
						else
						{
							endN=pGtzx->XYArray[pGtzx->XYNum-1][8];
							endE=pGtzx->XYArray[pGtzx->XYNum-1][9];
						}
						double tylc=pGtzxIN->PROJ_ML(endN,endE);
						tempPt.lc=pGtzxIN->LocateHundredMeterOnLine(tylc,false);//找HZ后面整百米
						pGtzxIN->xlpoint_pz(&tempPt);

						tempPt.x+=xjj*cos(fwj);
						tempPt.y+=xjj*sin(fwj);

						JdCen=&(pGtzx->JdCenArray[pGtzx->JdCenNum-1]);

						JdCen->N=tempPt.x;
						JdCen->E=tempPt.y;

						pGtzx->DLArray[pGtzx->DLNum-1].ELC=pGtzxIN->XLC(tempPt.lc,pGtzx->DLArray[pGtzx->DLNum-1].EGH);

						pGtzxIN->close();  // Finished with the pline header.						

						pGtzx->close();
						/*
						char kw[8];
						_tcscpy(kw,L"");
						acedInitGet(RSG_NOLIM,L"J j F f");
						if(RTNORM==acedGetKword(L"\n作为主线(J)或[分离段(F)](J):",kw))
						{
						if(_tcscmp(kw,L"F")==0||_tcscmp(kw,L"f")==0)
						{
						if(Acad::eOk==acdbOpenObject(pGtzx, lId, AcDb::kForWrite))
						{
						pGtzx->assertWriteEnabled();
						pGtzx->FLDNo=1;
						pGtzx->close();
						}
						}
						}						*/

					}
				}
			}
		}    
	}

	else
	{
		ads_printf(L"\nSelected entity is not a gtzx");
	} ;
	//	ShowWindow(TRUE);
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();	
	UpdateData();
}



void XLSJ::OnOutxl() //从哪里引出
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	// TODO: Add your control notification handler code here
	//	ShowWindow(FALSE);
	int rc;
	ads_name en;

	rc = ads_entsel(NULL, en, asDblArray(PT));
	if (rc != RTNORM) {    
		ads_printf(L"\nError during object selection");  
		//	ShowWindow(TRUE);	
		acDocManager->unlockDocument(acDocManager->curDocument());
		SetFocus();		
		return;   
	}
	acdbGetObjectId(lId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, lId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, lId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	if (pObj->isKindOf(JD_CENTER::desc())) 
	{
		pObj->close(); 
		/*acdbOpenObject(pGtzx, lId, AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,lId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pGtzx=JD_CENTER::cast(pObj);
		
		pGtzx->TranJD();
		int iJD=pGtzx->GetPreJD(PT.y,PT.x);//plh04.27
		/*
		if(iJD<pGtzx->JdCenNum-2)//如果所选的边不为最后一条，缺省为新建方案
		{
		IsModifyOldGTZX=false;
		m_ModifyOldGTZX=1;
		UpdateData(FALSE);
		}
		else//如果所选的边为最后一条，缺省为修改方案
		{
		IsModifyOldGTZX=true;
		m_ModifyOldGTZX=0;
		UpdateData(FALSE);
		}*/

		pGtzx->close();  // Finished with the pline header.  

	}
	/*
	else if (pObj->isKindOf(GTZX2::desc())) //从二线接出
	{
	pObj->close(); 
	iZx=0;
	double (*jdarray)[6];
	jdarray=new double[3][6];
	int njd=3;
	//input JDGH,LCGH//plh07.19a
	char JDGH[20],LCGH[20];

	acedGetString(0,L"请输入交点冠号(JD):",JDGH);
	acedGetString(0,L"请输入里程冠号(AK):",LCGH);

	if(_tcscmp(JDGH,L"")==0)_tcscpy(JDGH,L"JD");				
	if(_tcscmp(LCGH,L"")==0)_tcscpy(LCGH,L"AK");

	GTZX2  *Gtzx2x;
	acdbOpenObject(Gtzx2x, lId, AcDb::kForRead);		
	Gtzx2x->AddRXD(PT,jdarray,njd);
	Gtzx2x->close();
	if(njd==3&&jdarray[1][4]>5.0)//交径不为0
	{
	//07.17;

	GTZX *newGtzx=new GTZX(njd,jdarray,LCGH,JDGH);
	//07.17;
	newGtzx->mode_display=0;
	newGtzx->DV=Gtzx2x->m_pm1->DV;//添加对应设计车速
	lId=AddEntityToDbs(newGtzx);

	acdbOpenObject(newGtzx,lId,AcDb::kForWrite);
	BAS_DRAW_FUN::xlpoint tempPt;
	//一线上里程
	//plh06.01
	if(Gtzx2x->GetiRXDFromNE(newGtzx->array[2][8],newGtzx->array[2][9])<0)
	{//在并行段才搜索整百米
	//ZH点
	double tylc=PROJ_ML(Gtzx2x->m_pm1->array,Gtzx2x->m_pm1->tarray,newGtzx->array[2][8],newGtzx->array[2][9],newGtzx->array[2][8],newGtzx->array[2][9]);
	//一线上里程
	tempPt.lc=Gtzx2x->m_pm1->LocateHundredMeterOnLine(tylc,true);//找ZH前面整百米
	tylc=tempPt.lc;
	//			acutPrintf(L"1xlc=%lf\n",tylc);
	//一线上点
	Gtzx2x->m_pm1->xlpoint_pz(Gtzx2x->m_pm1->array,Gtzx2x->m_pm1->tarray,&tempPt);
	//			acutPrintf(L"1:x=%lf,y=%lf\n",tempPt.y,tempPt.x);
	//二线上里程
	tempPt.lc=PROJ_ML(Gtzx2x->array,Gtzx2x->tarray,tempPt.x,tempPt.y,tempPt.x,tempPt.y);
	//二线上点			
	xlpoint_pz(Gtzx2x->array,Gtzx2x->tarray,&tempPt);
	//			acutPrintf(L"2:x=%lf,y=%lf\n",tempPt.y,tempPt.x);
	newGtzx->JDarray[0][1]=tempPt.x;
	newGtzx->JDarray[0][2]=tempPt.y;

	newGtzx->DLB[0].BLC=Gtzx2x->m_pm1->XLC1(tylc,newGtzx->DLB[0].BGH,Gtzx2x->m_pm1->NDL,Gtzx2x->m_pm1->DLB);
	newGtzx->DLB[0].DL=0.0;
	newGtzx->JDarray[0][3]=newGtzx->DLB[0].BLC;
	}
	else//落在绕行段
	{
	//二线上的统一里程
	double tylc2x=Gtzx2x->PROJ_ML(Gtzx2x->array,Gtzx2x->tarray,newGtzx->array[2][8],newGtzx->array[2][9],newGtzx->array[2][8],newGtzx->array[2][9]);

	char LCGH[20];
	double xlc2x=Gtzx2x->XCLC(tylc2x,LCGH,Gtzx2x->DLB2x,Gtzx2x->NDL);

	//向前推个整百米
	double dlc;
	dlc=xlc2x-((int)(xlc2x/100.0))*100.0;

	//返推二线统一里程
	tylc2x-=dlc;
	xlpoint PtQD;
	PtQD.lc=tylc2x;
	Gtzx2x->xlpoint_pz(Gtzx2x->array,Gtzx2x->tarray,&PtQD);

	newGtzx->JDarray[0][1]=PtQD.x;
	newGtzx->JDarray[0][2]=PtQD.y;

	newGtzx->DLB[0].BLC=xlc2x-dlc;
	newGtzx->DLB[0].DL=0.0;
	newGtzx->JDarray[0][3]=newGtzx->DLB[0].BLC;
	}

	pGtzx=newGtzx;			
	PT.x=0.5*(newGtzx->JDarray[newGtzx->NJD-2][2]+newGtzx->JDarray[newGtzx->NJD-1][2]);
	PT.y=0.5*(newGtzx->JDarray[newGtzx->NJD-2][1]+newGtzx->JDarray[newGtzx->NJD-1][1]);
	newGtzx->close();
	pZhfa[iZx]=newGtzx;
	//				acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ADDXL\n",false,true);				
	}
	}*/

	else
	{
		pObj->close();
		ads_printf(L"\nSelected entity is not a gtzx");
	}

	//	ShowWindow(TRUE);	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	UpdateData();
}

//plh07.17
void XLSJ::OnRadioReplace() 
{
	// TODO: Add your control notification handler code here
	IsModifyOldGTZX=true;
	m_ModifyOldGTZX=0;
	UpdateData(FALSE);
	ButtonUpdate();
}

void XLSJ::OnRadioNewgtzx() 
{
	// TODO: Add your control notification handler code here
	IsModifyOldGTZX=false;
	m_ModifyOldGTZX=1;
	UpdateData(FALSE);
	ButtonUpdate();
}

void XLSJ::ButtonUpdate()
{
	UpdateData(TRUE);

	CRect Rect;
	Rect.left=93;
	Rect.top=20;
	Rect.bottom=40;

	if(IsModifyOldGTZX)//plh07.18
	{
		Rect.right=Rect.left+64;
		m_ButtonOutXL.MoveWindow(Rect);
		m_ButtonOutXL.SetWindowText(L"添加线元");
		m_ButtonInXL.ShowWindow(FALSE);		
	}
	else
	{
		Rect.right=Rect.left+32;
		m_ButtonOutXL.MoveWindow(Rect);
		m_ButtonOutXL.SetWindowText(L"引出");
		m_ButtonInXL.ShowWindow(TRUE);
	}
	m_ButtonOutXL.ShowWindow(TRUE);
	UpdateData(FALSE);
}

BOOL XLSJ::OnInitDialog() //plh07.19a
{
	CDialog::OnInitDialog();	
	// TODO: Add extra initialization here
	ButtonUpdate();
	CButton *pBtn;
	pBtn = (CButton *)GetDlgItem(IDC_RADIO1);
	pBtn->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//plh07.17

void XLSJ::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	if(m_IfA==false)
	{
		//原来为Lo,计算出新的A
		UpdateData();
		m_A1L1 = sqrt(m_A1L1*m_R);
		m_A2L2 = sqrt(m_A2L2*m_R);
	}
	m_IfA = true;	
	UpdateData(false);
}

void XLSJ::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	if(m_IfA==true)
	{
		//原来为A,计算出新的L0
		UpdateData();
		if(m_R>0.001)
		{
			m_A1L1 = m_A1L1*m_A1L1/m_R;
			m_A2L2 = m_A2L2*m_A2L2/m_R;
		}
		else
		{
			m_A1L1 = 0.0;
			m_A2L2 = 0.0;
		}

	}
	m_IfA = false;	
	UpdateData(false);

}
double XLSJ::GetRFromT_P_P(double x1,double y1,double x2,double y2,
						   double &xs,double &ys,double &x0,double &y0,
						   double &xe,double &ye,double &cenX,double &cenY,
						   AcDbObjectIdArray &eraseId,bool IsStandard, double DV)
						   //传入切线方向，返回半径,前一个切点，交点，后一个切点
{
	// TODO: Implement the command
	double jiao12;
	double dist=xyddaa(x1,y1,x2,y2,&jiao12);
	dist*=1000;
	x2+=dist*cos(jiao12);
	y2+=dist*sin(jiao12);
	x1-=dist*cos(jiao12);
	y1-=dist*sin(jiao12);
	double rad=-1.0;//半径
	double pxj,fwj1,fwj2,fwjr,fwjt,jiao,afa;
	AcGePoint3d center;

	AcGePoint3d sPt1(x1,y1,0.0);//切线的起点
	AcGePoint3d ePt1(x2,y2,0.0);//切线的终点
	AcDbLine *pl1=new AcDbLine(sPt1,ePt1);

	double side1,side2;	
	double tmpR=0.0;
	//select point.
	ads_point pk00;
	//	AfxMessageBox(L"1");
	if(RTNORM!=acedGetPoint(NULL,L"请选择圆弧上第一个点:\n",pk00))return -1.0;
	//	AfxMessageBox(L"2");
	AcGePoint3d Pk1(pk00[X],pk00[Y],pk00[Z]);
	ads_point pt1,pt2;
	AcGePoint3d pk;
	int type;
	struct resbuf result;
	double dT;
	double angle1;
	double ang1,ang2;
	//	AcDbArc *pArc= new AcDbArc();//弧
	AcDbCircle *pArc= new AcDbCircle();//圆
	AcDbObjectId arcId=AddEntityToDbs(pArc);
	/*	char Rchr[80];
	double texth=8;
	AcDbText *pText=new AcDbText(pk,Rchr);
	pText->setColorIndex(3);
	pText->setHeight(texth);
	AcDbObjectId textId=AddEntityToDbs(pText);
	*/
	bool ISOK=true;
	for(;;){

		if(ads_grread(1 , &type , &result)!=RTNORM)
		{ 
			ISOK=false;
			break;
		}
		if(type == 3)break;
		else if(type==25)
		{
			if(rad<0.00001)continue;			
			double cenX,cenY,dx,dy;	
			cenX=center.x;//plh06.03
			cenY=center.y;
			if(GetCircleFromT_P_R(x1,y1,x2,y2,rad,cenX,cenY)!=true)continue;
			dx=cenX-center.x;
			dy=cenY-center.y;
			//acutPrintf(L"dx=%lf,dy=%lf\n",dx,dy);
			center.x+=dx;
			center.y+=dy;
			Pk1.x+=dx;
			Pk1.y+=dy;
		}
		else if(type == 5)
		{
			pk.x=result.resval.rpoint[X];
			pk.y=result.resval.rpoint[Y];
			pk.z=result.resval.rpoint[Z];


			side1=GetSide(x1,y1,x2,y2,pk.x,pk.y);
			side2=GetSide(x1,y1,x2,y2,Pk1.x,Pk1.y);
			side1/=fabs(side1);
			side2/=fabs(side2);

			if(side1==0||side2==0)continue;//plh05.27??????????????????
			else
			{
				if(side1*side2<=0)continue;
			}
			AcGePoint3d Pk2(pk.x,pk.y,pk.z);
			if(Pk1.distanceTo(Pk2)!=0)
			{
				AcDbLine *pLinet=new AcDbLine(Pk1,Pk2);

				AcGePoint3dArray interPt;
				pLinet->intersectWith(pl1,AcDb::kExtendBoth,interPt,0,0); 
				delete pLinet;///
				AcGePoint3d Pt1(interPt[0].x,interPt[0].y,interPt[0].z);
				AcGePoint3d Pt0((Pk1.x+Pk2.x)/2,(Pk1.y+Pk2.y)/2,(Pk1.z+Pk2.z)/2);
				AcGePoint3d Pt2((2*Pt0.x-Pt1.x),(2*Pt0.y-Pt1.y),(2*Pt0.z-Pt1.z));
				fwjt=fwj(Pk1.x,Pk1.y,Pk2.x,Pk2.y);
				AcGePoint3d Ptx((Pt0.x+100*sin(fwjt+PI/2)),(Pt0.y+100*cos(fwjt+PI/2)),Pt0.z);

				AcDbLine *pLine3=new AcDbLine(Pt0,Ptx);
				//				if(fwjr!=fwj1&&fwj1+fwjr!=PI){ 

				AcGePoint3dArray interPtd;
				pLine3->intersectWith(pl1,AcDb::kExtendBoth,interPtd,0,0);
				delete pLine3;
				AcDbLine *pl2=new AcDbLine(interPtd[0],Pt2);
				AcDbLine *pl11=new AcDbLine(Pt1,interPtd[0]);

				fwj1=fwj(Pt1.x,Pt1.y,interPtd[0].x,interPtd[0].y);
				fwj2=fwj(interPtd[0].x,interPtd[0].y,Pt2.x,Pt2.y);
				pxj=PXJ(fwj1,fwj2);
				afa=AFA(fwj1,fwj2);
				fwjr=(PI-afa)/2+fwj2;
				double temprad=radiusfree(pl11,pl2,Pk2);				
				//				if(temprad<0.00001||temprad>1.2*get_R(temprad,DV))continue;
				rad=temprad;
				//				if(IsStandard)rad = get_R(rad,DV);

				if(fabs(rad-tmpR)>0.001)
				{
					ACHAR pszLabel[256];
					_stprintf(pszLabel,L"当前半径为: %.2lf",rad);
					acedGrText(-1,pszLabel,1);    
					tmpR=rad;
				}

				/*				_stprintf(Rchr,L"%.2lf\n",rad);
				texth=0.1*rad;
				acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
				pText->setTextString(Rchr);
				pText->setHeight(texth);
				pText->setPosition(pk);
				pText->close();
				*/				
				delete pl11;
				delete pl2;

				center.x=interPtd[0].x+(rad/cos(pxj/2))*sin(fwjr);
				center.y=interPtd[0].y+(rad/cos(pxj/2))*cos(fwjr);

				dT=rad*tan(0.5*pxj);
				pt1[X]=Pt1.x;
				pt1[Y]=Pt1.y;
				pt2[X]=interPtd[0].x;
				pt2[Y]=interPtd[0].y;
				angle1=ads_angle(pt2,pt1);

				ads_point qpt1,qpt2,cen;
				qpt1[X]=interPtd[0].x+dT*cos(angle1);
				qpt1[Y]=interPtd[0].y+dT*sin(angle1);
				qpt2[X]=Pk2.x;
				qpt2[Y]=Pk2.y;
				cen[X]=center.x;
				cen[Y]=center.y;

				ang1=ads_angle(cen,qpt1);
				ang2=ads_angle(cen,qpt2);

				/*	acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return 0.0;
				}
				jiao=fwj2-fwj1;
				if(jiao<0)jiao+=2*PI;
				pArc->setCenter(center);
				pArc->setRadius(rad);
				/*
				if(jiao>PI){
				pArc->setStartAngle(ang1);
				pArc->setEndAngle(ang2);
				}
				else{
				pArc->setStartAngle(ang2);
				pArc->setEndAngle(ang1);
				}
				*/
				pArc->close();
				//显示半径
				m_R = rad;
				UpdateData(false);
				//}
			}
		}
	}
	cenX=center.x;
	cenY=center.y;
	//	eraseId.append(textId);//顺序不能变
	eraseId.append(arcId);

	/*//delete 对象
	acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
	pText->erase();
	pText->close();
	acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);
	pArc->erase();
	pArc->close();
	*/

	//	double &xs,double &ys,double &x0,double &y0,double &xe,double &ye,
	delete pl1;
	pl1=NULL;
	//	acutPrintf(L"rad=%lf\n",rad);
	acedGrText(-5,L"完成",0);    
	acutPrintf(L"\n命令:");

	if(ISOK)return rad;
	else return -1.0;
}
double XLSJ::GetRFromT1T2(double &x1, double &y1, double &x0,double &y0,double &x2,double &y2, bool IsStandard, double DV,int INTRadius,int JUDGET012)
	//给定一个交点，直始x1,y1,交点x0,y0,终止x2,y2,是否遵守半径系列，返回半径
	//int JUDGET012=0//不进行夹直线判断？int JUDGET012=1//与前面夹直线判断int JUDGET012=2//与后面夹直线判断int JUDGET012=12//与前后夹直线判断
{
	//	acutPrintf(L"x1=%.2lf,y1=%.2lf,x0=%.2lf,y0=%.2lf,x2=%.2lf,y2=%.2lf\n",x1,y1,x0,y0,x2,y2);

	bool ISERROR=false;
	double T0min,Lrmin;
	//	GetT0min_Lrmin(DV,T0min,Lrmin);
	T0min=0.0;
	Lrmin=0.0;
	double T1=99999,T2=99999;
	//由x1,y1,x0,y0,x2,y2确定最大切长以保证最小夹直线长
	double Tmax,Tmax1,Tmax2;
	if(JUDGET012==0)
	{
		Tmax=99999999;
	}
	else if(JUDGET012==1)
	{
		Tmax=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))-T0min;
	}
	else if(JUDGET012==2)
	{
		Tmax=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))-T0min;
	}
	else//12
	{
		Tmax1=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))-T0min;
		Tmax2=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))-T0min;
		Tmax=min(Tmax1,Tmax2);
	}
	ads_point pt1,pt2;
	AcDbObjectId eId,eId1,arcId,LId1,LId2;
	AcGePoint3d center;//圆心

	AcGePoint3d sPt(x1,y1,0.0),sPt1(x0,y0,0.0);
	AcGePoint3d ePt(x0,y0,0.0),ePt1(x2,y2,0.0);
	AcGePoint2d spoint1(x1,y1),epoint1(x0,y0),spoint2(x0,y0),epoint2(x2,y2);
	AcGeLine2d Line1(spoint1,epoint1);
	AcGeLine2d Line2(spoint2,epoint2);
	AcDbLine *pl1=new AcDbLine(sPt,ePt);
	AcDbLine *pl2=new AcDbLine(sPt1,ePt1);
	/*
	pl1->setLineWeight(AcDb::kLnWt080);
	pl2->setLineWeight(AcDb::kLnWt080);
	*/
	LId1=AddEntityToDbs(pl1);
	LId2=AddEntityToDbs(pl2);

	double fwj1,fwj2;
	fwj1=fwj(sPt.x,sPt.y,ePt.x,ePt.y);
	fwj2=fwj(sPt1.x,sPt1.y,ePt1.x,ePt1.y);
	double pxj=PXJ(fwj1,fwj2);
	double afa=AFA(fwj1,fwj2);
	double fwjr=(pi-afa)/2+fwj2;
	double rad=-1.0,dT,jiao;
	double angle1,angle2;
	double ang1,ang2;
	AcGePoint3d pk(0,0,0);
	int type;
	struct resbuf result;
	AcDbArc *pArc= new AcDbArc;//(center,rad,angle1,angle2);
	arcId=AddEntityToDbs(pArc);	
	/*acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	pArc->setColorIndex(3);
	pArc->close();
	/*	char Rchr[256];
	_tcscpy(Rchr,L"");
	double texth=8.0,textroate=0.0;
	texth=0.02*sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));
	AcDbText *pText=new AcDbText(pk,Rchr);
	pText->setColorIndex(3);
	pText->setHeight(texth);
	AcDbObjectId textId=AddEntityToDbs(pText);
	*/	
	AcDbLine *perLine3d1=new AcDbLine();
	perLine3d1->setColorIndex(3);
	AcDbObjectId perLId1=AddEntityToDbs(perLine3d1);

	AcDbLine *perLine3d2=new AcDbLine();
	perLine3d2->setColorIndex(3);
	AcDbObjectId perLId2=AddEntityToDbs(perLine3d2);
	bool ISOK=true;
	double tmpR=0.0;
	for(;;)
	{	
		if(ads_grread(1 , &type , &result)!=RTNORM)
		{ 
			ISOK=false;
			break;
		}
		if(type==2)
		{
			short KEYCODE=result.resval.rint;
			//acutPrintf(L"%d",KEYCODE);
			if(KEYCODE==32)
			{
			}
			else if(KEYCODE==13)
			{
				break;
			}
			else {}
		}
		else if(type==25)
		{
			if(rad<0.00001)continue;			
			double cenX,cenY,dx,dy;
			cenX=center.x;
			cenY=center.y;
			if(GetCircleFromT_P_R(x1,y1,x0,y0,rad,cenX,cenY)!=true)continue;
			dx=cenX-center.x;
			dy=cenY-center.y;
			//			acutPrintf(L"dx=%lf,dy=%lf\n",dx,dy);
			x0+=dx;
			y0+=dy;
			//			x1+=dx;
			//			y1+=dy;
			x2+=dx;
			y2+=dy;
			/*	acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			/*acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			pl1->setStartPoint(AcGePoint3d(x1,y1,0.0));
			pl1->setEndPoint(AcGePoint3d(x0,y0,0.0));
			pl2->setStartPoint(AcGePoint3d(x0,y0,0.0));
			pl2->setEndPoint(AcGePoint3d(x2,y2,0.0));
			pl1->close();
			pl2->close();
			if(JUDGET012==0)
			{
				Tmax=99999999;
			}
			else if(JUDGET012==1)
			{
				Tmax=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))-T0min;
			}
			else if(JUDGET012==2)
			{
				Tmax=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))-T0min;
			}
			else//12
			{
				Tmax1=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))-T0min;
				Tmax2=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))-T0min;
				Tmax=min(Tmax1,Tmax2);
			}
		}
		else if(type == 3)
			break;
		else if(type == 5)
		{
			pk.x=result.resval.rpoint[X];
			pk.y=result.resval.rpoint[Y];
			pk.z=result.resval.rpoint[Z];

			//			E=pk.distanceTo(interPt[0]);
			double tempr=radiusfree(pl1,pl2,pk);
			if(tempr>5.0)rad=tempr;

			if(IsStandard)
			{
				//				tempr=get_R(rad,DV);
				if(tempr>5.0)rad=tempr;
				double l0=0.0;//get_l0(rad,DV);
				double BPMTL[10];
				double f1=fwj(x1,y1,x0,y0);
				double f2=fwj(x0,y0,x2,y2);
				double aa=AFA(f1,f2);
				if (aa>pi)aa=2*pi-aa;
				getBPMTL(dms_rad(aa),rad,l0,l0,&BPMTL[0]);
				T1=BPMTL[6];
				T2=BPMTL[7];
				//				double L=BPMTL[8];
				//				acutPrintf(L"rad=%lf,DV=%lf,AFA=%lf,l0=%lf,T1=%lf,T2=%lf,Tmax=%lf\n",rad,DV,aa,l0,T1,T2,Tmax);
				//				acutPrintf(L"最小夹直线长为: %.2lf,L",T0min);

				if(JUDGET012==0)
				{
					ISERROR=false;
				}
				else if(JUDGET012==12)
				{					
					//					acutPrintf(L"当前夹直线长为: L1=%.2lf,L2=%.2lf(右键平移!)\n",Tmax1-T1+T0min,Tmax2-T2+T0min);
					//					acutPrintf(L"Tmax1=%.2lf,Tmax2=%.2lf\n",Tmax1,Tmax2);
					//					acutPrintf(L"x1=%.2lf,y1=%.2lf,x2=%.2lf,y2=%.2lf\n",x1,y1,x2,y2);
					if(T1>Tmax1||T2>Tmax2)ISERROR=true;
					else ISERROR=false;
				}
				else if(JUDGET012==1)
				{
					acutPrintf(L"当前夹直线长为: L1=%.2lf(右键平移!)\n",Tmax-T1+T0min);
					if(T1>Tmax)ISERROR=true;
					else ISERROR=false;
				}
				else if(JUDGET012==2)
				{
					acutPrintf(L"当前夹直线长为: L2=%.2lf(右键平移!)\n",Tmax-T2+T0min);
					if(T2>Tmax)ISERROR=true;
					else ISERROR=false;
				}
			}
			else if(INTRadius>0)rad=(int)(rad/INTRadius)*INTRadius;

			if(fabs(rad-tmpR)>0.001)
			{
				ACHAR pszLabel[256];
				if(ISERROR)_stprintf(pszLabel,L"当前半径为: %.2lf  不合适!",rad);
				else _stprintf(pszLabel,L"当前半径为: %.2lf",rad);				
				acedGrText(-1,pszLabel,1);    
				tmpR=rad;
			}			

			center.x=x0+(rad/cos(pxj/2))*sin(fwjr);
			center.y=y0+(rad/cos(pxj/2))*cos(fwjr);

			dT=rad*tan(0.5*pxj);
			pt1[X]=sPt.x;
			pt1[Y]=sPt.y;
			pt2[X]=ePt.x;
			pt2[Y]=ePt.y;
			angle1=ads_angle(pt2,pt1);

			pt1[X]=sPt1.x;
			pt1[Y]=sPt1.y;
			pt2[X]=ePt1.x;
			pt2[Y]=ePt1.y;
			angle2=ads_angle(pt1,pt2);

			ads_point qpt1,qpt2,cen;
			qpt1[X]=x0+dT*cos(angle1);
			qpt1[Y]=y0+dT*sin(angle1);

			qpt2[X]=x0+dT*cos(angle2);
			qpt2[Y]=y0+dT*sin(angle2);

			cen[X]=center.x;
			cen[Y]=center.y;

			ang1=ads_angle(cen,qpt1);
			ang2=ads_angle(cen,qpt2);


			/*acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			jiao=fwj2-fwj1;
			if(jiao<0)jiao+=2*PI;
			pArc->setCenter(center);
			pArc->setRadius(rad);
			if(jiao>PI){
				pArc->setStartAngle(ang1);
				pArc->setEndAngle(ang2);
			}
			else{
				pArc->setStartAngle(ang2);
				pArc->setEndAngle(ang1);
			}
			if(ISERROR)
			{
				pArc->setColorIndex(1);
				acutPrintf(L"夹直线长不够!\n");
			}
			else pArc->setColorIndex(3);			
			pArc->close();

			m_R = rad;
			UpdateData(false);

			/*
			x1=center.x+rad*cos(ang1);
			y1=center.y+rad*sin(ang1);
			x2=center.x+rad*cos(ang2);
			y2=center.y+rad*sin(ang2);
			acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse);
			acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse);
			pl1->setStartPoint(AcGePoint3d(x1,y1,0.0));
			pl1->setEndPoint(AcGePoint3d(x0,y0,0.0));
			pl2->setStartPoint(AcGePoint3d(x0,y0,0.0));
			pl2->setEndPoint(AcGePoint3d(x2,y2,0.0));
			pl1->close();
			pl2->close();
			*/
			double perX,perY;   
			GetPerPoint(center.x,center.y,x1,y1,x0,y0,perX,perY);
			/*	acdbOpenObject(perLine3d1,perLId1,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(perLine3d1,perLId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			perLine3d1->setStartPoint(AcGePoint3d(perX,perY,0.0));
			perLine3d1->setEndPoint(center);
			perLine3d1->close();//切点与圆心连线				

			GetPerPoint(center.x,center.y,x2,y2,x0,y0,perX,perY);
			/*acdbOpenObject(perLine3d2,perLId2,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(perLine3d2,perLId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			perLine3d2->setStartPoint(AcGePoint3d(perX,perY,0.0));
			perLine3d2->setEndPoint(center);
			perLine3d2->close();//切点与圆心连线		
		}
	}
	if(pArc && !pArc->isErased())
	{
		if(acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
		{
			pArc->erase();
			pArc->close();
		}	
	}

	/*	acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
	pText->erase();	
	pText->close();
	*/
	/*acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	/*acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}

	double jiao1,jiao2,dist1,dist2;
	jiao1=acutAngle(asDblArray(pl1->endPoint()),asDblArray(pl1->startPoint()));
	jiao2=acutAngle(asDblArray(pl2->startPoint()),asDblArray(pl2->endPoint()));
	dist1=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	dist2=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));
	if(T1>dist1)
	{
		dist1=1.2*T1;
		x1=x0+dist1*cos(jiao1);
		y1=y0+dist1*sin(jiao1);
	}
	if(T2>dist2)
	{
		dist2=1.2*T2;
		x2=x0+dist2*cos(jiao2);
		y2=y0+dist2*sin(jiao2);
	}
	//	acutPrintf(L"jiao1=%lf,jiao2=%lf,x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",jiao1,jiao2,x1,y1,x2,y2);
	if(pl1 && !pl1->isErased())
	{
		pl1->erase();	
		pl1->close();
	}
	if(pl2 && !pl2->isErased())
	{
		pl2->erase();	
		pl2->close();
	}

	if(acdbOpenObject(perLine3d1,perLId1,AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
	{
		perLine3d1->erase();
		perLine3d1->close();//切点与圆心连线				
	}

	if(acdbOpenObject(perLine3d2,perLId2,AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
	{
		perLine3d2->erase();
		perLine3d2->close();//切点与圆心连线				
	}

	if(ISERROR)rad=0;		
	//	acutPrintf(L"返回半径:%.2lf\n",rad);
	//	if(ISERROR)return -1.0;
	acedGrText(-5,L"完成",0);    
	acutPrintf(L"\n命令:");
	if(ISOK)return rad;
	else return -1.0;
}

void XLSJ::OnBUTTONupdate() //刷新
{
	// TODO: Add your control notification handler code here

	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	// TODO: Add your control notification handler code here
	UpdateData();

	//修改交点参数
	AcDbObject* pObj;
	if(Acad::eOk==acdbOpenObject(pObj, lId, AcDb::kForWrite))
	{
		pGtzx=JD_CENTER::cast(pObj);
		pGtzx->assertWriteEnabled();

		if(pGtzx->JdCenNum<3)
			return;

		JdCenter *JdCen=&(pGtzx->JdCenArray[pGtzx->JdCenNum-2]);

		JdCen->R=m_R;
		if(m_IfA)//A
		{
			JdCen->A1=m_A1L1;
			JdCen->A2=m_A2L2;
		}
		else
		{
			JdCen->A1=sqrt(m_A1L1*m_R);
			JdCen->A2=sqrt(m_A2L2*m_R);
		}
		if(pGtzx->JdCenNum>2)
		{
			if(m_LinkRelation=="S形")//与前一曲线S
				pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = S_Link;
			else if(m_LinkRelation=="C形")
				pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = C_Link;
			else if(m_LinkRelation=="卵形")
				pGtzx->JdCenArray[pGtzx->JdCenNum-3].LinkAfter = Luan_Link;

		}
		pGtzx->close();

	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();	
	IsModifyOldGTZX=true;
	m_ModifyOldGTZX=0;
	UpdateData();	

}
