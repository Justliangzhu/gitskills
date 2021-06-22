// ZDMSJ.cpp: implementation of the ZDMSJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdarx.h"
#include "ZDMSJ.h"
#include "CProfileOpt.h"
#include "GTZDMReactor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <aced.h>
#include <dbsymtb.h>
//  #include <dbapserv.h>
#include <adslib.h>
#include <dbxrecrd.h>

#include <string.h>
#include <dbents.h>
#include <geassign.h>
#include <dbgroup.h>
#include <gearc2d.h >
#include <dbpl.h> 


#include <dbjig.h>
#include <acgi.h>

#include <assert.h>
//#include <iostream.h>
#include "rxdefs.h"
#include "ol_errno.h"
#include "STDIO.h"
#include "malloc.h"
#include "time.h"
//#include "zmsjzl.h"
#include "JD_CENTER.h"
#include "triDTM_road.H"
#include "ORI_SCALE.h"
#include "RoadDesignWorkingDatabase.h"
#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS" 
extern triDTM_road ptriDTM;
extern ORI_SCALE pORI_SCALE;
extern   JD_CENTER  *pzLinep;
extern AcDbVoidPtrArray g_reactorList;
AcDbObjectId ZpId;//记录当前纵坡
//RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库

ZDMSJ::ZDMSJ()
{
}

ZDMSJ::~ZDMSJ()
{

}



//	标注变坡点
void ZDMSJ::BZBPD1(double  BPD_array[MAXBPDNUM][4],int NBPD,double CSB[6], int NLC)
{   
	AcGePoint3d Pt1,Pt2; 
	ACHAR chrh[40],chrlc[40];
	int i;
	double HH,LC,LL,II,B,WB,yy;
	double K0,H0,X0,Y0,HBVSCALE,texth;
	ACHAR GH[8];
	K0=CSB[0];H0=CSB[1];X0=CSB[2];Y0=CSB[3];HBVSCALE=CSB[4];texth=CSB[5];

	WB=4*texth;
	WB=WB+10*texth;
	Pt2.z=0.0;  Pt2.z=0.0;

	LC=(BPD_array[0][0]-X0)*HBVSCALE+K0;
	HH=BPD_array[0][1]-Y0+H0;    // 设计高程

	Pt1.x=BPD_array[0][0];
	Pt1.y=BPD_array[0][1];
	Pt2.x=Pt1.x;
	Pt2.y=Pt1.y+WB;
	//G_makeline(pWd,Pt1,Pt2,3);
	makeline( Pt1,Pt2,3);
	B=pi*0.5;
	// 设计 高程
	ads_rtos(HH,2,NLC,chrh);
	Pt2.x=Pt1.x-texth*0.5;
	Pt2.y=Pt1.y+3*texth;
	// G_maketext(pWd,Pt2,chrh,B ,texth ,4,2);     
	maketext( Pt2,chrh,B ,texth ,4,1);   

	// 设计 里程
	_tcscpy(chrlc,LCchr(GH,LC,NLC,2));
	Pt2.x=Pt1.x+texth*1.5;
	Pt2.y=Pt1.y+3*texth;
	maketext(Pt2,chrlc,B ,texth ,4,2);  

	for (i=1;i<NBPD;i++)
	{ 
		LL=(BPD_array[i][0]-BPD_array[i-1][0])*HBVSCALE;
		if (fabs(LL)<0.001) LL=1000.0;
		II=(BPD_array[i][1]-BPD_array[i-1][1])*1000.0/LL;
		LC=(BPD_array[i][0]-X0)*HBVSCALE+K0;
		HH=BPD_array[i][1]-Y0 +H0;    // 设计高程
		yy=BPD_array[i][1]-BPD_array[i-1][1]; // 高差

		Pt1.x=BPD_array[i][0];
		Pt1.y=BPD_array[i][1];
		Pt2.x=Pt1.x;
		Pt2.y=Pt1.y+WB;
		// G_makeline(pWd,Pt1,Pt2,3);
		makeline( Pt1,Pt2,3);
		B=pi*0.5;
		// 设计 高程
		ads_rtos(HH,2,NLC,chrh);
		Pt2.x=Pt1.x-texth*0.5;
		Pt2.y=Pt1.y+3*texth;
		//G_maketext(pWd,Pt2,chrh,B ,texth ,4,2);     
		maketext( Pt2,chrh,B ,texth ,4,1); 
		// 设计 里程
		_tcscpy(chrlc,LCchr(GH,LC,NLC,2));
		Pt2.x=Pt1.x+texth*1.5;
		Pt2.y=Pt1.y+3*texth;
		// G_maketext(pWd,Pt2,chrlc,B ,texth ,4,2);     
		maketext( Pt2,chrlc,B ,texth ,4,1); 
		//（左侧）坡度 坡长
		double fb,de ;
		de=xyddaa(BPD_array[i-1][1],BPD_array[i-1][0],BPD_array[i][1],BPD_array[i][0],&fb);
		fb= 0.5*pi-fb;
		// 坡度  
		if (fabs(II-(int)(II))<0.001)  {  ads_rtos(II,2,0,chrh);}
		else  if (fabs(II*10.0-(int)(II*10.0))<0.001)  {  ads_rtos(II,2,1,chrh);}
		else  if (fabs(II*100.0-(int)(II*100.0))<0.001)  {  ads_rtos(II,2,2,chrh);}
		else   ads_rtos( II,2,3,chrh);
		_tcscat(chrh,L"(o/oo)"); 	    
		Pt2.x=Pt1.x- LL*0.5/HBVSCALE-texth;
		Pt2.y=Pt1.y+0.5*texth-yy*0.5;
		// G_maketext(pWd,Pt2,chrh,fb,texth ,4,2);     
		maketext(Pt2,chrh,fb,texth ,4,1); 
		//  坡长
		if (fabs(LL-(int)(LL))<0.001)  {  ads_rtos(LL,2,0,chrlc);}
		else ads_rtos(LL,2,3,chrlc);
		_tcscat(chrh,L"(o/oo)"); 	    
		Pt2.x=Pt1.x- LL*0.5/HBVSCALE-3*texth;
		Pt2.y=Pt1.y-1.6*texth-yy*0.5;
		//G_maketext(pWd,Pt2,chrlc,fb,texth ,4,2);     
		maketext(Pt2,chrlc,fb,texth ,4,1); 
	};  
	//   return 1;
}


void ZDMSJ::createGTZDM_JDDATAFILE()
{ 
	/*
	double T0=0.0,texth=4,interal=18;
	char fname[140],GH[8] ;
	FILE *fpc;
	int i=0,j=0,k,trow,color=2,iq=2;
	double  Texth;
	double PD_array[MAXBPDNUM][4];

	struct resbuf result;
	int BZW,NH,NLC; 

	for (i=0;i<MAXBPDNUM;i++) {    for (j=0;j<4;j++) PD_array[i][j]=0.0;  };

	if ( (ads_getfiled( L"请输入设计坡度文件名 *.PD ",NULL,L"PD",2+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	_tcscpy(fname,result.resval.rstring);
	//  zmsjzl ls;
	//szf  ls.DoModal ();   

	char JDGH[8];
	double arrayd[MAXBPDNUM][6];

	iq=2;
	for (i=0;i<MAXBPDNUM;i++) {    for (j=0;j<6;j++) arrayd[i][j]=0.0;  };


	if((fpc=_wfopen(ls.m_filename,L"r"))==NULL)
	{ trow=0; return ; }
	else
	{
	fwscanf(fpc,L"%d%s",&j,JDGH); 
	trow=0;
	do
	{ fwscanf(fpc,L"%d",&j); arrayd[trow][0]=j;
	for (k=1;k<=5;k++) fwscanf(fpc,L"%lf",&arrayd[trow][k]);
	ads_printf(L"\n");
	for (k=1;k<=5;k++)  ads_printf(L" %15.6f",arrayd[trow][k]);

	fwscanf(fpc,L"\n");trow++;
	if (trow>98) break;
	} while(!feof(fpc));
	fclose(fpc);
	};


	if (trow< 2) return ;

	BZW=3;
	NLC=3; 
	NH=3;
	Texth=4;

	_tcscpy(GH,L"AK");
	JD_CENTER  *pzLine= new GTZX(trow,arrayd,&GH[0],&JDGH[0]);
	pzLine->setINIT(NLC,NH,1,Texth);
	pzLinep = pzLine;//一体化设计的全局变量

	//    AcDbObjectId  LId;
	//   LId=AddEntityToDbs(pzLine);

	xlpoint PZ;
	double x1,y1,x2,y2,tmp;
	ads_point pp;
	PZ.lc = pzLine->QXB[0][3];
	x1=0;
	y1=0;          
	while (PZ.lc  < pzLine->QXB[k+1][3])
	{
	xlpoint_pz(pzLine->array,pzLine->tarray,&PZ) ;
	pp[0] = PZ.x ;
	pp[1] = PZ.y ;
	tmp=ptriDTM->interpolate(pp);
	x2 = int(PZ.lc -pzLine->QXB[k-1][0]);
	y2 = int(tmp)-70;
	PZ.lc = PZ.lc + 10.0;
	x1=x2;
	y1=y2;
	}; 

	if((fpc=_wfopen(fname,L"r"))==NULL)
	{
	trow=0;
	return ; 
	}
	else
	{
	trow=0;
	do
	{ 
	if ( trow==0) {  for (k=0;k<2;k++) fwscanf(fpc,L"%lf",&PD_array[trow][k]); }
	else { for (k=0;k<3;k++) fwscanf(fpc,L"%lf",&PD_array[trow][k]);  };
	ads_printf(L"\n");
	for (k=0;k<3;k++)  ads_printf(L" %15.6f",PD_array[trow][k]);

	fwscanf(fpc,L"\n");trow++;
	if (trow>98) break;
	} while(!feof(fpc));
	fclose(fpc);
	};

	if (trow< 2) return ;

	//ads_prompt(L"\n\n 请输 1-里程标高.里程留2位小数高程留3位 2-留2位小数 3-3位小数");
	//ads_initget(1+2+4,NULL);
	//ads_getint(L"\n  Please select 1-LC.xx/H.xxx   2- X.xx 3-X.xxx:",&BZW);
	BZW=3;
	if(BZW<1||BZW>3) BZW=2;
	if (BZW==1) { NLC=2; NH=3;}
	else if (BZW==2) { NLC=2; NH=2;}
	else { NLC=3; NH=3;};

	//ads_initget(1+2+4,NULL);
	//ads_getreal(L"\n  Please texth=:",&Texth);
	Texth=4;
	//ads_initget(1+2,NULL);
	// ads_getstring(0,L"\n 请输 里程冠号 Enter GH:",GH);
	// ads_initget(1+2,NULL);
	// ads_getstring(0,L"\n 请输 交点冠号 Enter GH:",JDGH);

	AcGePoint3d pkPt;
	acedInitGet(NULL, NULL);
	if(acedGetPoint(NULL,L"请在屏幕上指定绘图原点,L",asDblArray(pkPt))==RTCAN)
	return;
	pORI_SCALE.X_ORI = pkPt.x+30;
	pORI_SCALE.Y_ORI = pkPt.y+pORI_SCALE.PQXHIGH+pORI_SCALE.LCBHigh+20;

	_tcscpy(GH,L"AK"); 
	GTZDM  *pzdmLine= new GTZDM(trow,PD_array,&GH[0]);
	pzdmLine->setINIT(NLC,NH,1,Texth,10);

	AcDbObjectId  LIdzdm;
	LIdzdm=AddEntityToDbs(pzdmLine);

	ZpId=LIdzdm;//当前纵坡*///szf

}
//坡段连接
void ZDMSJ::LINK_PDLINE()
{ 
	int rc;
	double LCB;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	double T0=0.0,texth=4,interal=18;
	ACHAR  GH[8] ;
	GTZDM_ROAD *pGtzdm1;
	GTZDM_ROAD *pGtzdm2;
	AcDbObjectId eId1,eId2;


	int i=0,j=0, trow1,trow2,color=2,iq=2;
	double  HH,Texth,HBVSCALE,X0,Y0,K0,H0;
	double PD_array1[MAXBPDNUM][4],PD_array2[MAXBPDNUM][4];

	int BZW,NH,NLC; 
	BZW=3;
	if(BZW<1||BZW>3) BZW=2;
	if (BZW==1) { NLC=2; NH=3;}
	else if (BZW==2) { NLC=2; NH=2;}
	else { NLC=3; NH=3;};
	Texth=5;_tcscpy(GH,L"K");HBVSCALE=pORI_SCALE.HBVSCALE;

	for (i=0;i<MAXBPDNUM;i++) 
	{
		for (j=0;j<4;j++) 
		{
			PD_array1[i][j]=0.0;
			PD_array2[i][j]=0.0;
		}
	}

	rc = ads_entsel(L"\n请选择第一条坡度线: ", en, asDblArray(PT));
	if (rc != RTNORM) {  ads_printf(L"\nError during object selection"); return;   }
	pzLinep=GlobalZDMTK.GetPM(PT);
	acdbGetObjectId(eId1, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId1, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId1, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc()))
	{             
		pObj->close();
		/*acdbOpenObject(pGtzdm1,eId1, AcDb::kForRead);*/
		if(acdbOpenObject(pGtzdm1,eId1, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		trow1=pGtzdm1->NBPD;
		for (i=0;i<trow1;i++) 
		{
			for (j=0;j<4;j++) 
			{
				PD_array1[i][j]=pGtzdm1->BPD_array[i][j];
			}
			//acutPrintf(L"l=%lf,h=%lf,r=%lf\n",PD_array1[i][0],PD_array1[i][1],PD_array1[i][2]);
		}
		pGtzdm1->close();
	}
	else {
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
		return;
	} 

	rc = ads_entsel(L"\n请选择第二条坡度线: ", en, asDblArray(PT));
	if (rc != RTNORM) {  ads_printf(L"\nError during object selection"); return;   }

	acdbGetObjectId(eId2, en);
	/* acdbOpenObject(pObj, eId2, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId2, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc()))
	{             
		pObj->close();
		/*acdbOpenObject(pGtzdm2,eId2, AcDb::kForRead);	*/
		if(acdbOpenObject(pGtzdm2,eId2, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		trow2=pGtzdm2->NBPD;
		for (i=0;i<trow2;i++) 
		{
			for (j=0;j<4;j++) 
			{
				PD_array2[i][j]=pGtzdm2->BPD_array[i][j];
			}
		}
		pGtzdm2->close();		
	}
	else {
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
		return;
	} 
	int trow=trow1;//重复点数；
	int k;
	if(eId2==eId1)
	{
		ads_alert(L"两个坡度实体为同一实体,无法连接!");
		return;
	}
	PD_array1[trow1-1][2]=700;
	PD_array2[0][2]=700;
	for(i=trow1,k=0;k<trow2;k++)
	{
		if(fabs(PD_array1[i-1][0]-PD_array2[k][0])>0.001)
		{
			for(j=0;j<4;j++)
				PD_array1[i][j]=PD_array2[k][j];
			i++;trow++;
		}
	}
	//排序
	queue(PD_array1 , trow, 0);
	//为保证连接后实体坡度不变，调整相应坡度
	if(trow1>0)
	{
		int inf=1;
		int Xsw=(int)(pORI_SCALE.XSW+0.001); 
		double dh,LL,unit,ii;

		dh = fabs(PD_array1[trow1][1]-PD_array1[trow1-1][1]);
		LL = fabs(PD_array1[trow1][0]-PD_array1[trow1-1][0])*pORI_SCALE.HBVSCALE;
		unit = 100.0;

		//计算trow1-1 -----trow1的坡度
		if(PD_array1[trow1][1]-PD_array1[trow1-1][1]<0)
			inf = -1;	
		if(Xsw==1)//小数位取一位
			ii=inf*((int)(dh*unit*10/LL+0.5))/10.0;//坡度
		else if(Xsw==2)//小数位取两位
			ii=inf*((int)(dh*unit*100/LL+0.5))/100.0;//坡度
		else if(Xsw==3)//小数位取三位
			ii=inf*((int)(dh*unit*1000/LL+0.5))/1000.0;//坡度
		else//小数位取零位
			ii=inf*(int)(dh*unit/LL+0.5);//坡度*/

		PD_array1[trow1][3]=ii;

		//计算trow1 -----trow1+trow2的标高
		for(i=trow1;i<trow;i++)
		{
			LL = (PD_array1[i][0] - PD_array1[i-1][0])*pORI_SCALE.HBVSCALE;
			PD_array1[i][1]=PD_array1[i-1][1]+PD_array1[i][3]*LL/unit;
		}

	}

	LCB = (PD_array1[0][0] - pORI_SCALE.X_ORI) * HBVSCALE;
	HH = (PD_array1[0][1] - pORI_SCALE.Y_ORI) + pORI_SCALE.ZMIN;
	X0 = long int(PD_array1[0][0] / 10); X0 = X0 * 10;
	K0 = LCB + (X0 - PD_array1[0][0]) * HBVSCALE;
	Y0 = long int(PD_array1[0][1] / 10); Y0 = Y0 * 10;
	H0 = HH + (Y0 - PD_array1[0][1]);

	if(acdbOpenObject(pGtzdm1,eId1, AcDb::kForWrite)==Acad::eOk)
	{
		pGtzdm1->erase(Adesk::kTrue);
		pGtzdm1->close();
	}

	if(acdbOpenObject(pGtzdm2,eId2, AcDb::kForWrite)==Acad::eOk)
	{
		pGtzdm2->erase(Adesk::kTrue);
		pGtzdm2->close();
	}


	GTZDM_ROAD  *pzLine= new GTZDM_ROAD(trow,PD_array1,GH,1,pGtzdm1->DesHXSW);
	pzLine->setINIT(NLC,NH,1,Texth,HBVSCALE,X0,Y0,K0,H0);

	AcDbObjectId  LId;
	LId=AddEntityToDbs(pzLine);

	ZpId=LId;//当前纵坡
	return  ;      
} 

void  ZDMSJ::changeDL()
{	 
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;

	rc = ads_entsel(L"\nSelect a  线路 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); return;   }

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{   pObj->close();

	GTZDM_ROAD  *pzLine;
	int iDL;
	double BLC,DL;
	ACHAR GH[6];
	// 取得要修改的某断链点
	ads_prompt(L"\n\n 请输 断链点号");
	ads_initget(1+2+4,NULL);
	ads_getint(L"\n  Please intput IDL=",&iDL);
	ads_prompt(L"\n\n 请输 该断链点的BLC,DL,GH");
	ads_initget(1+2+4,NULL);
	ads_getreal(L"\n  Please intput BLC=",&BLC);

	ads_getreal(L"\n  Please intput DL=",&DL);
	ads_initget(1+2+4,NULL);
	ads_getstring(0,L"\n  Please intput GH=",GH);

	/* acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
	if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pzLine->setDLM(iDL,BLC,DL,GH,GH);
	pzLine->close();  // Finished with the pline header.     

	}
	else {
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
	} ; 
	return  ; 
} 

void  ZDMSJ::addDL()
{	 
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;

	rc = ads_entsel(L"\nSelect a  线路纵断面设计线 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }
	pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*  acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{    pObj->close();

	GTZDM_ROAD  *pzLine;
	int iDL;
	double BLC,DL;
	ACHAR GH[6];
	// 取得要加的某断链点
	ads_prompt(L"\n\n 请输 断链点号");
	ads_initget(1+2+4,NULL);
	ads_getint(L"\n  Please intput IDL",&iDL);
	ads_prompt(L"\n\n 请输 该断链点的BLC,DL,GH");
	ads_initget(1+2+4,NULL);
	ads_getreal(L"\n  Please intput BLC=",&BLC);

	ads_getreal(L"\n  Please intput DL=",&DL);
	ads_initget(1+2+4,NULL);
	ads_getstring(0,L"\n  Please intput GH=",GH);

	/*  acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
	if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pzLine->addDLM(iDL,BLC,DL,GH);
	pzLine->close();  // Finished with the pline header.     

	}
	else {
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
	} ; 
	return  ;   
} 

void  ZDMSJ::delDL()
{	 
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;

	rc = ads_entsel(L"\nSelect a  线路 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); return;   }

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{   pObj->close();

	GTZDM_ROAD  *pzLine;
	int iDL;

	// 取得要删除的某断链点
	ads_prompt(L"\n\n 请输 断链点号");
	ads_initget(1+2+4,NULL);
	ads_getint(L"\n  Please intput IDL=",&iDL);      

	/* acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
	if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pzLine->DelDLM(iDL);
	pzLine->close();  // Finished with the pline header.     

	}
	else {
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
	} ;
	return  ;  
} 
//增加变坡点
void  ZDMSJ::add_BPD()
{	 
	//设置参数
	int rc,NBPD,   BPDi;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	int track,type ;
	ads_point cpt ,pt;
	double X0,Y0,K0,H0,HBVSCALE;
	struct resbuf *result ;

	//提示用户选择纵断面设计线实体
	rc = ads_entsel(L"\n选择纵断面设计线: ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;
	if (rc != RTNORM) 
	{    
		ads_printf(L"\nError during object selection");  
		return;   
	}

	//int izdm = GlobalZDMTK.FindWichZdm(PT);
	//if(izdm<0)
	//	return;
	//pzLinep=GlobalZDMTK.GetPM(PT);

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{    
		pObj->close(); 
		GTZDM_ROAD  *pzLine;
		int  n,i,bz=-99;
		double R, XX1,XX2,YY1,YY2,dtemp,dd,ss;

		/*acdbOpenObject(pzLine, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pzLine,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}

		//20191217	
		int izdm = pzLine->m_Frame_Index;
		if (izdm < 0 || izdm >= GlobalZDMTK.NZDM)
		{
			ads_printf(L"图框号记录错误！请重新启动CAD再尝试该命令\n");
			pzLine->close();
			return;
		}
		pzLinep = GlobalZDMTK.ZDMTK[izdm].pm;

		pzLine->getK0H0(&X0,&Y0,&K0,&H0,&HBVSCALE);
		NBPD=pzLine->getBPD(0,&R,&XX1,&YY1);
		BPDi=NBPD;
		i=1;
		do 
		{	dd=sqrt((PT[X]-XX1)*(PT[X]-XX1)+(PT[Y]-YY1)*(PT[Y]-YY1));
		n=pzLine->getBPD(i,&R,&XX2,&YY2);
		dtemp=sqrt((PT[X]-XX2)*(PT[X]-XX2)+(PT[Y]-YY2)*(PT[Y]-YY2));
		ss=sqrt((YY1-YY2)*(YY1-YY2)+(XX1-XX2)*(XX1-XX2));

		//	dd/dtemp;  //选取点到（起点）某顶点的距离(平方）
		if (fabs(dtemp+dd-ss)<0.1) 
		{ 
			//				if (dd<dtemp) {  BPDi=i-1; }
			BPDi=i; 
			bz=99; 	
		} 			
		else { YY1=YY2;XX1=XX2; };

		i=i+1;
		if (i>=NBPD) bz=99;
		}while (bz<0);

		pzLine->close();  // Finished with the pline header.   

		if (BPDi>0 && BPDi<=NBPD)
		{
			result = ads_buildlist(RTSTR,L"",0);
			track = 1;
			type = 5;
			ads_point spt,ept;
			spt[X]=XX1,spt[Y]=YY1;
			ept[X]=XX2, ept[Y]=YY2;
			cpt[X]=PT[X],cpt[Y]=PT[Y];
			ads_grdraw(spt ,cpt,-1,0);
			ads_grdraw(ept ,cpt,-1,0);
			double LC,HH,dml;
			ACHAR gh[10],ckml[80];
			CString GH;
			while ( type != 3  )
			{
				if(ads_grread(track,&type,result )==RTCAN)
					return;
				pt[X]=result->resval.rpoint[X];
				pt[Y]=result->resval.rpoint[Y];		   
				if(pt[X]!=cpt[X] || pt[Y]!=cpt[Y])
				{
					ads_grdraw(spt ,cpt,-1,0);
					ads_grdraw(ept ,cpt,-1,0);
					ads_grdraw(spt ,pt,-1,0);
					ads_grdraw(ept ,pt,-1,0);

					LC=(pt[X]-X0)*HBVSCALE+K0;
					HH=(pt[Y]-Y0)+H0;
					dml=pzLine->XLC(LC,GH);		
					pzLine->dmltockml(GH,dml,ckml);
					ads_printf(L"里程：%s  高程：%lf\n",ckml,HH);
				}
				cpt[X] = pt[X] , cpt[Y]=pt[Y];
			}
			PT1[X]=pt[X], PT1[Y]=pt[Y];
			ads_initget(2+4,NULL);
			double Rshu,i1,i2,deltai;
			i1 = (PT1.y- pzLine->BPD_array[BPDi-1][1])*1000.0/((PT1.x-pzLine->BPD_array[BPDi-1][0])*HBVSCALE);
			i2 = (pzLine->BPD_array[BPDi][1] - PT1.y )*1000.0/((pzLine->BPD_array[BPDi][0]-PT1.x)*HBVSCALE);
			deltai = i1-i2;
			if(deltai > 0)
				Rshu = pzLine->A_YBRshu;
			else
				Rshu = pzLine->T_YBRshuMin;
			//		 Rshu = Get_Rshu(pzLine->DV,deltai);
			ACHAR mes[256];
			_stprintf(mes,L"\n 请输 第%d变坡点的竖曲线半径R=<%0.0lf米>",BPDi,Rshu);
			rc = ads_getreal(mes,&R);
			if(rc==RTCAN)
				return;
			if(rc==RTNONE)
				R = Rshu;

			ads_grdraw(spt ,pt,-1,0);
			ads_grdraw(ept ,pt,-1,0);
			// 取得要加的某断链点	
			/* acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
			if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pzLine->addBPD(BPDi,R,PT1.x,PT1.y);
			pzLine->close();  // Finished with the pline header.     
		}; 
	}
	else 
	{
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
	}
	return;   
} 

void  ZDMSJ::del_BPD()
{	 
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;

	rc = ads_entsel(L"\nSelect a   纵断面线上要删除的变坡点 : ", en, asDblArray(PT));

	if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }

	pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/* acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{    pObj->close(); 
	GTZDM_ROAD  *pzLine;
	int BPDi, NBPD,n,i,bz=-99;
	double R,XX1,YY1,XX2,YY2,dtemp;

	/* acdbOpenObject(pzLine, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pzLine,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}

	NBPD=pzLine->getBPD(0,&R,&XX1,&YY1);
	BPDi=NBPD;
	i=0;
	do 
	{	
		//			 dd=sqrt((PT[X]-XX1)*(PT[X]-XX1)+(PT[Y]-YY1)*(PT[Y]-YY1));
		n=pzLine->getBPD(i,&R, &XX2,&YY2);
		dtemp=sqrt((PT[X]-XX2)*(PT[X]-XX2)+(PT[Y]-YY2)*(PT[Y]-YY2));
		//            ss=sqrt((XX1-XX2)*(XX1-XX2)+(YY1-YY2)*(YY1-YY2));

		//	dd/dtemp;  //选取点到（起点）某顶点的距离(平方）
		if (fabs(dtemp)<3.0) 
		{   BPDi=i; 
		bz=99; 
		} 
		else { XX1=XX2;YY1=YY2; };

		//		    ads_printf(L"\n %d PT.x=%10.2f  PT.y=%10.2f  ss=%10.2f  dd=%10.2f  dtemp=%10.2f",i,PT.x,PT.y,ss,dd,dtemp);
		i=i+1;
		if (i>=NBPD) bz=99;
	}while (bz<0);
	if (BPDi<NBPD) 	n=pzLine->getBPD(i,&R,&XX2,&YY2);
	pzLine->close();  // Finished with the pline header.   

	if (BPDi>=0 && BPDi<=NBPD-1)
	{    		
		ads_printf(L"\n\n  要删除的 BPDi=%d R=%10.2f ",BPDi,R);       
		/* acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pzLine->DelBPD(BPDi);
		pzLine->close();  // Finished with the pline header.                
	}; 
	}
	else {
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
	} ; 
	return  ;   
} 

void  ZDMSJ::changeBPD()
{	 
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	double X0,Y0,K0,H0,HBVSCALE;
	rc = ads_entsel(L"\nSelect a  纵断面线上要修改的变坡点 : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); return;   }

	pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{   pObj->close();      

	GTZDM_ROAD  *pzLine;
	int BPDi, NBPD,n,i,bz=-99;
	double R,XX1,YY1,XX2,YY2,dtemp,dd,ss;

	/*acdbOpenObject(pzLine, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pzLine,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}

	NBPD=pzLine->getBPD(0,&R,&XX1,&YY1);
	BPDi=NBPD;
	i=0;
	do 
	{	dd=sqrt((PT[X]-XX1)*(PT[X]-XX1)+(PT[Y]-YY1)*(PT[Y]-YY1));
	n=pzLine->getBPD(i,&R, &XX2,&YY2);
	dtemp=sqrt((PT[X]-XX2)*(PT[X]-XX2)+(PT[Y]-YY2)*(PT[Y]-YY2));
	ss=sqrt((XX1-XX2)*(XX1-XX2)+(YY1-YY2)*(YY1-YY2));

	//	dd/dtemp;  //选取点到（起点）某顶点的距离(平方）
	if (fabs(dtemp)<2.1) 
	{   BPDi=i;  bz=99;
	ads_printf(L"\n %d PT.x=%8.2f  PT.y=%8.2f  ss=%8.2f  dd=%8.2f  dtemp=%8.2f",i,PT.x,PT.y,ss,dd,dtemp);
	}
	else { XX1=XX2;YY1=YY2; };

	i=i+1;
	if (i>=NBPD) bz=99;
	}while (bz<0);
	if (BPDi<NBPD) {	n=pzLine->getBPD(i,&R,&XX2,&YY2);
	n=pzLine->getK0H0(&X0,&Y0,&K0,&H0,&HBVSCALE);
	};
	pzLine->close();  // Finished with the pline header.   

	if (BPDi>=0 && BPDi<=NBPD-1)
	{    		

		double LC,HH;
		LC=(XX2-X0)*HBVSCALE+K0;
		HH=(YY2-Y0)+H0;
		ads_printf(L"\n\n  要修改的 BPDi=%d R=%6.0f LC=%10.2f  HH=%8.2f",BPDi,R,LC,HH);       


		ads_getreal(L"\n  Please intput R=",&R);

		ads_getreal(L"\n  Please intput 里程LC=",&LC);

		ads_getreal(L"\n  Please intput 设计高程HH=",&HH);

		XX1=(LC-K0)/HBVSCALE+X0;
		YY1=HH-H0+Y0;

		/*acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pzLine->setBPD(BPDi,R,XX1,YY1);
		pzLine->close();  // Finished with the pline header.                
	}; 
	}
	else {
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
	} ; 
	return  ;  
} 

void ZDMSJ::SAVEPD(void)
{  
	int bz=-9;
	ACHAR fname[256];
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;

	rc = ads_entsel(L"\nSelect a  选择一个纵断面坡度线: ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{    
		ads_printf(L"\n纵断面坡度线选择错误,未保存!"); return;   
	}
	pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);
	
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{   pObj->close();
		GTZDM_ROAD  *pzLine;
		
		CString workdir,curpm;
		FILE *fpr;
		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		workdir=Cworkdir;
		workdir += "\\data";   
		
		CFileDialog fnamedlg(false);
		fnamedlg.m_ofn.lpstrDefExt = L"PD";
		fnamedlg.m_ofn.lpstrInitialDir=workdir;
		fnamedlg.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
		fnamedlg.m_ofn.lpstrFilter=L"纵坡文件(*.PD)\0*.PD\0\0";
		fnamedlg.DoModal();	
		_tcscpy(fname,fnamedlg.GetPathName()); 	
		
		workdir+="\\CurZMPD";
		if((fpr=_wfopen(workdir,L"w"))!=NULL)
		{
			fwprintf(fpr,L"%s\n",fname);
			fclose(fpr);
		}	
		
		/*acdbOpenObject(pzLine, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pzLine,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
			pzLine->saveBPD(fname);
			pzLine->close();  // Finished with the pline header.      
	}
	else 
	{
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDM");
	} ; 
	return  ;

}
//直线转坡度线
void ZDMSJ::createGTZDM_INPUT()//直线转坡度线
{ 
	int rc;
	double LCB;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	double T0=0.0,texth=4,interal=18;
	ACHAR  GH[8] ;

	int i=0,j=0, trow,color=2,iq=2;
	double  HH,Texth,HBVSCALE,X0,Y0,K0,H0;
	double PD_array[MAXBPDNUM][4];

	int BZW,NH,NLC; 
	int izdm=0;
	for (i=0;i<MAXBPDNUM;i++) {    for (j=0;j<4;j++) PD_array[i][j]=0.0;  };

	rc = ads_entsel(L"\nSelect a    LINE/Polyline : ", en, asDblArray(PT));
	if (rc != RTNORM) {  ads_printf(L"\nError during object selection"); return;   }

	izdm = GlobalZDMTK.FindWichZdm(PT);
	if(izdm<0)
		return;

	//   pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/* acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if ((pObj->isKindOf(AcDb2dPolyline::desc())) || (pObj->isKindOf(AcDbPolyline::desc()))||
		(pObj->isKindOf(AcDbLine::desc())))
	{             
		BZW=3;
		if(BZW<1||BZW>3) BZW=2;
		if (BZW==1) { NLC=2; NH=3;}
		else if (BZW==2) { NLC=2; NH=2;}
		else { NLC=3; NH=3;};
		Texth=5;_tcscpy(GH,L"K");HBVSCALE=pORI_SCALE.HBVSCALE;
		// 2Dployline
		if (pObj->isKindOf(AcDb2dPolyline::desc())) 
		{ 
			pObj->close();
			AcDb2dPolyline *pPline;

			AcDb2dVertex *pVertex;
			AcGePoint3d location;
			AcDbObjectId vertexObjId;

			/*acdbOpenObject(pPline, eId, AcDb::kForRead);*/
			if(acdbOpenObject(pPline,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			AcDbObjectIterator *pVertIter= pPline->vertexIterator();

			pPline->close(); 

			for ( i= 0; !pVertIter->done(); i++, pVertIter->step())
			{  //寻找 的某顶点
				trow=i;
				vertexObjId = pVertIter->objectId();
				/*acdbOpenObject(pVertex, vertexObjId, AcDb::kForRead);*/
				if(acdbOpenObject(pVertex,vertexObjId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				location = pVertex->position();
				PD_array[trow][0]=location[X];
				PD_array[trow][1]=location[Y]; 
				//PD_array[trow][2]=10000;		//20190709	暂时改成700，以后根据规范设置默认值
				PD_array[trow][2] = 700;
			};
			trow=trow+1;
			pPline->close();
			if(acdbOpenObject(pPline, eId, AcDb::kForWrite)==Acad::eOk)
			{
				pPline->erase(Adesk::kTrue);
				pPline->close();
			}

		}
		//  polyline
		else if (pObj->isKindOf(AcDbPolyline::desc())) 
		{  
			pObj->close();
			AcDbPolyline *pPline;

			AcGePoint3d location;
			AcDbObjectId ObjId;
			int numverts;

			/*acdbOpenObject(pPline, eId, AcDb::kForRead);*/
			if(acdbOpenObject(pPline,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			numverts=pPline->numVerts();

			for ( i= 0; i<numverts; i++)
			{  
				//寻找的某顶点
				trow=i;
				pPline-> getPointAt( i, location);
				PD_array[trow][0]=location[X]; 
				PD_array[trow][1]=location[Y]; 
				//PD_array[trow][2]=10000;		//20190709	暂时改成700，以后根据规范设置默认值
				PD_array[trow][2] = 700;
				ads_printf(L"\n x=%10.3f y=%10.3f z=%10.3f",location[X],location[Y],location[Z]);
			}
			trow=trow+1;
			pPline->close();
			if(acdbOpenObject(pPline, eId, AcDb::kForWrite)==Acad::eOk)
			{
				pPline->erase(Adesk::kTrue);
				pPline->close();
			}


		}
		else if  (pObj->isKindOf(AcDbLine::desc())) 
		{     
			pObj->close();
			AcDbLine *pLine;
			AcGePoint3d PT1,PT2;
			/*acdbOpenObject(pLine, eId, AcDb::kForRead);*/
			if(acdbOpenObject(pLine,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			PT1=pLine->startPoint();
			PT2=pLine->endPoint();        
			pLine->close(); 

			PD_array[0][0]=PT1[X];
			PD_array[0][1]=PT1[Y]; 
			//PD_array[0][2]=10000;
			PD_array[0][2] = 700;
			PD_array[1][0]=PT2[X]; 
			PD_array[1][1]=PT2[Y]; 
			//PD_array[1][2]=10000;
			PD_array[1][2] = 700;
			trow=2;

			if(acdbOpenObject(pLine, eId, AcDb::kForWrite)==Acad::eOk)
			{
				pLine->erase(Adesk::kTrue);
				pLine->close();
			}

		};

		/*
		ads_printf(L"\n 输入起点x=%10.3f 对应的里程",PD_array[0][0]);
		ads_initget(1+2+4,NULL);
		ads_getreal(L"\n  Please select 输入起点里程LCB:",&LCB);
		ads_printf(L"\n 输入起点y=%10.3f 对应的高程",PD_array[0][1]);
		ads_initget(1+2+4,NULL);
		ads_getreal(L"\n  Please select 输入起点标高h:",&HH);
		*/
		LCB = (PD_array[0][0] - pORI_SCALE.X_ORI) * pORI_SCALE.HSCALE;	//20190709	修改采用双轴比例尺
		HH=(PD_array[0][1]-pORI_SCALE.Y_ORI) * pORI_SCALE.VSCALE + pORI_SCALE.ZMIN;	//20190709	修改采用双轴比例尺
		X0 = long int(PD_array[0][0] / 10); 
		X0 = X0 * 10;
		K0 = LCB + (X0 - PD_array[0][0]) * pORI_SCALE.HSCALE;
		Y0 = long int(PD_array[0][1] / 10); 
		Y0 = Y0 * 10;
		H0 = HH + (Y0 - PD_array[0][1]) * pORI_SCALE.VSCALE;

		GTZDM_ROAD  *pzLine= new GTZDM_ROAD(trow,PD_array,GH,1);
		pzLine->setINIT(NLC,NH,1,Texth,HBVSCALE,X0,Y0,K0,H0);

		AcDbObjectId  LId;
		LId=AddEntityToDbs(pzLine);

		ZpId= LId;//当前纵坡
		GlobalZDMTK.ZDMTK[izdm].zdm = pzLine;
	}
	else 
	{
		pObj->close();
		ads_printf(L"\nSelected entity is not a line/polyline");
	}
	return  ;      
}

//拉坡设计
void ZDMSJ::CZmLPSJ()
{
	//设置参数
	int type,track=1,nbpd;
	struct resbuf entdata;// 鼠标拖动时的动态坐标
	double PD_array[MAXBPDNUM][4];
	ACHAR  GH[8];
	int rc;
	AcGePoint3d PT1;
	ads_name en;
	AcDbObjectId eId;
	GTZDM_ROAD  *pzLine;
	AcDbObject *pObj;

	//提示用户选择纵面实体
	rc = ads_entsel(L"\n选择一个纵面坡度线或纵面图框实体 : ", en, asDblArray(PT1));
	if(rc==RTCAN)
		return;
	int izdm;
	if (rc == RTNORM) 
	{    
		acdbGetObjectId(eId, en);    
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();

		if (pObj->isKindOf(GTZDM_ROAD::desc())) //是GTZDM实体,取数据
		{		
			if(acdbOpenObject(pzLine,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			ZpId = eId;
			nbpd = pzLine->NBPD;
			izdm = pzLine->m_Frame_Index;
			pzLine->close();			
		}
		else if (pObj->isKindOf(DMX_ROAD::desc()))
		{
			DMX_ROAD* EarthLine = NULL;
			EarthLine = DMX_ROAD::cast(pObj);
			izdm = EarthLine->Zdmno;

			if (izdm < 0 || izdm >= GlobalZDMTK.NZDM)
			{
				ads_printf(L"图框号错误！\n");
				EarthLine->close();
				return;
			}
			if (GlobalZDMTK.ZDMTK[izdm].zdm)
			{
				ZpId = GlobalZDMTK.ZDMTK[izdm].zdm->id();
				nbpd = GlobalZDMTK.ZDMTK[izdm].zdm->NBPD;

				if (acdbOpenObject(pObj, ZpId, AcDb::kForRead) != Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					EarthLine->close();
					return;
				}
				pObj->close();
			}

			EarthLine->close();
		}
	}
	else if (rc == RTCAN)	//如果用户取消，则不显示任何提示
	{
		return;
	}
	else		//其余情况则提示用户
	{
		ads_printf(L"未选中纵面相关实体！\n");
		return;
	}
	//else if(fabs(PT1.x)<0.001 &&fabs(PT1.y)<0.001)	//20191217	不再采用采点获取图框，存在获取错误图框的隐患
	//{
	//	ads_grread (1, &type, &entdata); 
	//	PT1.x=entdata.resval.rpoint[0];
	//	PT1.y=entdata.resval.rpoint[1];
	//}
	//izdm = GlobalZDMTK.FindWichZdm(PT1);

	if (izdm < 0 || izdm >= GlobalZDMTK.NZDM)
	{
		ads_printf(L"图框号错误！\n");
		return;
	}

	//pzLinep = GlobalZDMTK.GetPM(PT1);
	pzLinep = GlobalZDMTK.ZDMTK[izdm].pm;
	if(!pzLinep)
	{
		ads_printf(L"平面方案为空\n");
		return;
	}

	pORI_SCALE.STARTLC = pzLinep->DLArray[0].ELC;
	pORI_SCALE.ENDLC = pzLinep->XLLength;
	pORI_SCALE.X_ORI = GlobalZDMTK.ZDMTK[izdm].X0;
	pORI_SCALE.Y_ORI = GlobalZDMTK.ZDMTK[izdm].Y0;
	pORI_SCALE.ZMIN = GlobalZDMTK.ZDMTK[izdm].H0;

	if(!pObj->isKindOf(GTZDM_ROAD::desc()))
	{
		ads_printf(L"\n所选非纵断面实体,系统将为您新建纵断面实体...\n");

		//设置起始里程
		ACHAR mes[80];
		double dml = -1000;
		_stprintf(mes,L"\n  请输入起始变坡点里程 = <线路起点:%0.3lf> :  ",pzLinep->DLArray[0].ELC);
		ads_initget(2 + 4, NULL);
		ads_getreal(mes, &dml);
		if(dml < -900)
			dml = pzLinep->DLArray[0].ELC;
		double cml = pzLinep->TYLC(dml);

		//设置起始高程
		double starth = 0.0;
		double sh = GlobalZDMTK.ZDMTK[izdm].dmx->DMXys[0].level;
		_stprintf(mes,L"\n  请输入起始变坡点设计标高 = <%0.3lf> :  ",sh);
		if (ads_getreal(mes, &starth) != RTNORM)
			starth = sh;

		//计算图上坐标点
		PD_array[0][0] = (cml - pzLinep->DLArray[0].ELC) / pORI_SCALE.HSCALE + GlobalZDMTK.ZDMTK[izdm].X0;	//20190709	修改采用双轴比例尺
		//PD_array[0][0] = cml;
		PD_array[0][1] = (starth - GlobalZDMTK.ZDMTK[izdm].H0) / pORI_SCALE.VSCALE + GlobalZDMTK.ZDMTK[izdm].Y0;
		//PD_array[0][1] = starth;

		_tcscpy(GH, L"K");
		GTZDM_ROAD  *pzLine = new GTZDM_ROAD(1, PD_array, GH, 1, 2);
		pzLine->m_Frame_Index = izdm;
		pzLine->setINIT(3, 3, 1, 4, 100);
		ZpId = AddEntityToDbs(pzLine);
		nbpd = 1;
	}

	track=1;
	nbpd++;

	/*
	while (track>0) 
	{  //鼠标拖动循环
	if(ads_grread (track, &type, &entdata)!=RTNORM)
	break;
	acdbOpenObject(pzLine,ZpId, AcDb::kForWrite);
	pzLine->assertWriteEnabled();

	pzLine->BPD_array[nbpd-1][0]=entdata.resval.rpoint[0];
	pzLine->BPD_array[nbpd-1][1]=entdata.resval.rpoint[1];

	pzLine->NBPD = nbpd;

	pzLine->close();
	if (type==3)  //  单击鼠标左键输入一点
	{  
	nbpd++;	   
	ads_initget(RSG_OTHER,L"C c E e");
	if(ads_getkword(L"\n继续 (C) / 结束 (E): <C>  ", choice)==RTNONE)
	_tcscpy(choice ,L"C");	 
	if(_tcscmp(choice ,L"E")==0 || _tcscmp(choice ,L"e")==0)//结束
	track=0;
	}
	}*/


	CString mes;
	ads_printf(L"注意：点击\"i\"键输入确切坡度值，点击鼠标左键拾取下一变坡点，鼠标右键结束!\n");
	while (track>0) 
	{  
		if(ads_grread (track, &type, &entdata)!=RTNORM)
		{
			if(acdbOpenObject(pzLine,ZpId,AcDb::kForWrite)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pzLine->assertWriteEnabled();
			pzLine->NBPD--;
			pzLine->close();
			break;
		}
		if(acdbOpenObject(pzLine,ZpId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pzLine->assertWriteEnabled();
		if(type!=2)
		{
			pzLine->BPD_array[nbpd-1][0]=entdata.resval.rpoint[0];
			pzLine->BPD_array[nbpd-1][1]=entdata.resval.rpoint[1];		 
			pzLine->NBPD = nbpd;
		}
		pzLine->close();		
		if (type==3)  //单击鼠标左键输入一点
		{      			

			nbpd++;
			track=1;

			ads_printf(L"注意：点击\"i\"键输入确切坡度值，点击鼠标左键拾取下一变坡点，鼠标右键结束!\n");
		}
		else if(type==2)//i键
		{
			nbpd = pzLine->NBPD;
			double SPD,LL;

			mes.Format(L"\n请输入坡度值<%0.2lf>",pzLine->BPD_array[nbpd-1][3]);		
			rc = ads_getreal(mes,&SPD);
			if(rc==RTCAN) 
				;
			else
			{
				if(rc==RTNONE)
					SPD = pzLine->BPD_array[nbpd-1][3];
				LL=(pzLine->BPD_array[nbpd-1][0]-pzLine->BPD_array[nbpd-2][0])*pzLine->HBVSCALE;
				if (fabs(LL)<0.001) LL=100.0;
				pzLine->BPD_array[nbpd-1][1]=pzLine->BPD_array[nbpd-2][1]+SPD*LL/100;

				nbpd++;
				track=1;
			}				
		}
		else if((type!=5) && (type!=12))
		{
			pzLine->NBPD--;
			break;
		}
	}

	GlobalZDMTK.ZDMTK[izdm].zdm = pzLine;
	pzLine->close();
	//	 acedGetAcadDwgView()->SetFocus();
	//	ads_printf(L"地面线采集结束!\n");
	ads_printf(L"\nCommand: ");
}

void  ZDMSJ::queue(double a[][4] , int nrow, int inf )
{
	int i,j,k;
	double t;

	for(i=0; i<nrow; i++)
		for(j=i+1; j<nrow; j++)
		{
			if(a[i][inf] > a[j][inf])
			{
				for(k=0; k<4; k++)
				{
					t = a[i][k];
					a[i][k] = a[j][k];
					a[j][k] = t;
				}
			}
		}
}

//一键纵坡
void ZDMSJ::AutoSlopeData()
{
	int rc;
	AcGePoint3d PT1;
	ads_name en;
	AcDbObjectId eId;
	AcDbObject *pObj;
	BAS_DRAW_FUN BasFun;
	RoadDesignWorkingDatabase WorkingData;
	int  NBPD; 		//坡度数组  
	int deshxsw, pdxsw;
	//[i][0]里程 [i][1]高程 [i][2]竖曲线 [i][3]坡度
	double  BPD_Array[MAXBPDNUM][4], *dmx = NULL;
	NBPD = 0;	pdxsw = 3; 	deshxsw = 3;
	GTZDM_ROAD *pzdmLine = NULL; 
	DMX_ROAD  *pdmx = NULL;
	AcDbObjectId  LIdzdm,ZdmPdId = NULL;
	ACHAR GH[8];
	_tcscpy(GH, L"K");
	PDTAB_ROAD tempPD;
	Is_NeedInitPro = true;//默认需要，当道路总长过短时直接求交上变坡点

	m_JPInforArray.clear(); 
	m_LevelPtArray.clear();
	m_SlopeArray.clear();
	m_EarthLine.clear();
	m_BPDArray.clear();
	m_LastPDArray.clear();

	rc = ads_entsel(L"\n请选择地面线实体 : ", en, asDblArray(PT1));
	if (rc == RTCAN)
		return;

	acdbGetObjectId(eId, en);
	if (acdbOpenObject(pObj, eId, AcDb::kForRead) == Acad::eOk)
		pObj->close();
	else
		return;

	if (pObj->isKindOf(DMX_ROAD::desc())) //是DMX实体,取数据
	{
		pdmx = DMX_ROAD::cast(pObj);
		izdm = pdmx->Zdmno;
		m_EarthLine.resize(pdmx->NUM);
		for (int i =0;i<pdmx->NUM;i++)
		{
			m_EarthLine[i].lc = pdmx->DMXys[i].lc;
			m_EarthLine[i].level = pdmx->DMXys[i].level;
		}
		//pdmx->close();
	}
	else
	{
		acutPrintf(L"所选实体不是地面线实体!\n");
		return;
	}
	//当前图框号
	if (izdm < 0)
		return;
	//获取平面指针
	JD_CENTER* pzLinep = WorkingData.CreateHorizontalRoad(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname);
	if (!pzLinep)
		return;
	//规范参数获取
	pORI_SCALE.STARTLC = pzLinep->DLArray[0].ELC;
	pORI_SCALE.ENDLC = pzLinep->XLLength;
	pORI_SCALE.X_ORI = GlobalZDMTK.ZDMTK[izdm].X0;
	pORI_SCALE.Y_ORI = GlobalZDMTK.ZDMTK[izdm].Y0;
	pORI_SCALE.ZMIN = GlobalZDMTK.ZDMTK[izdm].H0;

	//计算纵断面设计标准
	BasFun.SetZdmMinPC(pzLinep->DV, m_PrfDesData.MAXPD, m_PrfDesData.YBPCMin, m_PrfDesData.JXPCMin, m_PrfDesData.T_YBRshuMin,
		m_PrfDesData.T_JXRShu, m_PrfDesData.A_YBRshuMin, m_PrfDesData.A_JXRShu, m_PrfDesData.YBLRshuMin, m_PrfDesData.JXLRshuMin);

	WorkingData.GetLevelCtrlData(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname, m_LevelPtArray);	//获取控制高程数组
	WorkingData.GetJPData(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname, m_JPInforArray); //获取接坡数组

	for (int i = 0; i < MAXBPDNUM; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			BPD_Array[i][j] = 0.0;
		}
	}

	m_UserSetGCKZDArray_SX.clear();
	m_UserSetGCKZDArray_XX.clear();
	GCKZPt tempGCKZD;
	//考虑交叉点
	for (int i=0;i<m_LevelPtArray.size();i++)
	{
		if (m_LevelPtArray[i].Note.Find(L"交叉") >= 0)
		{
			if (m_LevelPtArray[i].Style == 1)
			{
				tempGCKZD.Cml = m_LevelPtArray[i].Cml;
				tempGCKZD.DesHSX = m_LevelPtArray[i].DesHSX;
				m_UserSetGCKZDArray_SX.push_back(tempGCKZD);
			}
			else if (m_LevelPtArray[i].Style == 2)
			{
				tempGCKZD.Cml = m_LevelPtArray[i].Cml;
				tempGCKZD.DesHXX = m_LevelPtArray[i].DesHXX;
				m_UserSetGCKZDArray_XX.push_back(tempGCKZD);
			}
		}
	}

	//根据是否有接坡信息获取起终变坡点信息
	GetBPDStAndEdfromJP(NBPD, pdmx);

	//设置优化起终点里程、高程
	GetStAndEdLC(m_StartLC,m_EndLC, m_StartH,m_EndH);

	//获得接坡约束范围内的地面线
	std::vector<DMXYS> LSDMX; //临时地面线表
	LSDMX.clear();
	LSDMX = m_EarthLine;
	m_EarthLine.clear();
	DMXYS tempDMXYS;
	for (int i =0;i< LSDMX.size();i++)
	{
		if (LSDMX[i].lc >= m_StartLC && LSDMX[i].lc <= m_EndLC)
		{
			tempDMXYS.lc = LSDMX[i].lc;
			tempDMXYS.level = LSDMX[i].level;
			m_EarthLine.push_back(tempDMXYS);
		}
	}

	//获得初始方案
	if (Is_NeedInitPro)
		GetInitSlope(pzLinep, m_EarthLine);
	else //当道路不满足坡长要求时求交上坡度，初始方案直接赋值
		m_SlopeArray = m_BPDArray;

	//优化参数设置
	CProfileOpt* profileOpt = new CProfileOpt();
	profileOpt->m_pPlain = pzLinep; //平面指针
	profileOpt->m_DMXLine = m_EarthLine; //地面线数组
	if (m_JPInforArray.size() >0)
		profileOpt->m_JPInforArray = m_JPInforArray; //接坡信息
	profileOpt->m_SlopeArray = m_SlopeArray; //初始方案
	profileOpt->m_BPDArray = m_BPDArray; //接坡点信息用于判断起始里程
	profileOpt->StartOptimization(m_LastPDArray,m_BriArray,m_TunArray);

	////未优化(初始方案)
	//for (int i = 0;i<m_SlopeArray.size();i++)
	//	m_BPDArray.push_back(m_SlopeArray[i]);

	//优化形成最终方案
	if (m_JPInforArray.size() > 0)
	{
		//接坡时，优化里程从接坡点开始，接坡信息需要与最终方案合并
		if ((m_JPInforArray[0].Is_JP_Bef == 1 || m_JPInforArray[0].Is_JP_Aft == 1) && Is_NeedInitPro)
		{
			for (int i = 0; i < m_LastPDArray.size(); i++)
				m_BPDArray.push_back(m_LastPDArray[i]);
		}
		else
		{
			//不需要接坡约束,此外当道路长较短时，直接求交点也按起终里程优化直接赋值
			m_BPDArray.clear();
			m_BPDArray = m_LastPDArray;
		}
	}
	else
	{
		//没有接坡信息，则可直接赋值
		m_BPDArray.clear();
		m_BPDArray = m_LastPDArray;
	}
		
	//对优化后变坡点数组按里程排序
	SortData(m_BPDArray);

	////形成最终纵断面方案
	//FormFinalSlope_ZMSJ();

	for (int i = 0; i < m_BPDArray.size(); i++)
	{
		if (i > 0 && i + 1 < m_BPDArray.size())
			m_BPDArray[i].Rshu = _wtof(m_PrfDesData.A_YBRshuMin);
		else
			m_BPDArray[i].Rshu = 0.0;
	}

	for (int i =0;i< m_BPDArray.size();i++)
	{
		BPD_Array[i][0] = m_BPDArray[i].ml;
		BPD_Array[i][1] = m_BPDArray[i].Level;
		BPD_Array[i][2] = m_BPDArray[i].Rshu;
	}
	
	//创建实体所需最终方案变坡点个数
	NBPD = (int)m_BPDArray.size();

	//创建实体
	//纵坡
	pzdmLine = new GTZDM_ROAD(NBPD, BPD_Array/*PD_array*/, &GH[0], 0, deshxsw);
	if (m_JPInforArray.size() > 0 && m_JPInforArray[0].Is_JP == 1)
	{
		pzdmLine->m_Is_JP = 1;
		pzdmLine->m_JPInforArray_slope = m_JPInforArray;
		pzdmLine->m_LevelCtrlPtArray = m_LevelPtArray;
	}
	pzdmLine->m_Frame_Index = izdm;
	ZdmPdId = ZpId = pzdmLine->AddEntityToDbs(pzdmLine);
	GlobalZDMTK.ZDMTK[izdm].zdm = pzdmLine;

	AcDbObjectId TunId = NULL;
	//隧道
	int num = m_TunArray.size();
	double TunMl[500][2];
	if (num > 0)
	{
		for (int i = 0;i<num;i++)
		{
			TunMl[i][0] = m_TunArray[i].StDml;
			TunMl[i][1] = m_TunArray[i].EdDml;
		}

		TUN_ROAD *pTun;
		pTun = new TUN_ROAD(num, TunMl, pzdmLine, 1, izdm);
		GlobalZDMTK.ZDMTK[izdm].ptun = pTun;
		TunId = BasFun.AddEntityToDbs(pTun);
	}
	if (num > 0 && ZdmPdId)
	{
		//加响应器
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(TunId, 'T');
		AcDbObject *pObj;
		if (acdbOpenObject(pObj, ZdmPdId, AcDb::kForWrite) != Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		GTZDM_ROAD *Zm;
		Zm = GTZDM_ROAD::cast(pObj);
		Zm->assertWriteEnabled();
		Zm->addReactor(pGTZDMReactor);
		Zm->close();
		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId = ZdmPdId;
		pPair->pEntReactor = pGTZDMReactor;
		g_reactorList.append(pPair);
	}
	//桥梁
	double BriMl[500][2];
	num = m_BriArray.size();
	AcDbObjectId BriId = NULL;
	if (num > 0)
	{
		for (int i = 0; i < num; i++)
		{
			BriMl[i][0] = m_BriArray[i].StDml;
			BriMl[i][1] = m_BriArray[i].EdDml;
		}

		BRI_ROAD *pBri;
		pBri = new BRI_ROAD(num, BriMl, pzdmLine, 1, izdm);
		GlobalZDMTK.ZDMTK[izdm].pbri = pBri;
		BriId = BasFun.AddEntityToDbs(pBri);
	}
	if (num > 0 && ZdmPdId)
	{
		//加响应器
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(BriId, 'B');
		AcDbObject *pObj;
		if (acdbOpenObject(pObj, ZdmPdId, AcDb::kForRead) != Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		GTZDM_ROAD *Zm;
		Zm = GTZDM_ROAD::cast(pObj);
		Zm->addReactor(pGTZDMReactor);
		Zm->close();

		ReactorEntityPair *pPair = new ReactorEntityPair;
		pPair->entityId = ZdmPdId;
		pPair->pEntReactor = pGTZDMReactor;
		g_reactorList.append(pPair);
	}

	//pdmx->close();
	//pzdmLine->close();

	WorkingData.SetJPData(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname, m_JPInforArray); //保存接坡数组
}


//优化变坡点数组
void ZDMSJ::GetInitSlope(JD_CENTER* pRoad, std::vector<DMXYS>& EarthLine)
{
	//设置参数
	std::vector<DMXYS> SmoothEarthLine;	//平顺地面线
	std::vector<DMXYS> ThreeDiffQuotientPt, OneDiffQuotientPt;
	std::vector<DMXYS> ChangePt;	//变坡点数组
	RoadDesignWorkingDatabase WorkingData;
	
	//三阶均差计算
	GetThreeDiffQuotientPt_ZMSJ(m_EarthLine, SmoothEarthLine, ThreeDiffQuotientPt); 

	//一阶均差计算
	GetOneDiffQuotientPt_ZMSJ(ThreeDiffQuotientPt, OneDiffQuotientPt);

	//确定地面线突变点，作为纵坡的初值
	GetChangePt_ZMSJ(pRoad, OneDiffQuotientPt, ChangePt);

	//初始化坡度数组
	IniSlopeArrray_ZMSJ(ChangePt);

	//调整初始方案的分段
	JudgeSection_ZMSJ();

	//以地面线拟合的期望来生成各变坡点设计标高
	CreatPlanMiu_ZMSJ();

	//形成初始方案
	FormInitSlope_ZMSJ();

	//存入活动数据库
	if (!m_SlopeArray.size())
		return;

	//转换成现场里程（不带冠号）形式，以存储数据库
	CString str;
	for (int i = 0; i < m_SlopeArray.size(); i++)
	{
		double xlc = pRoad->XLC1(m_SlopeArray[i].cml);
		str.Format(L"%0.3lf", xlc);
		m_SlopeArray[i].ml = _wtof(str);
		if (i + 1 < m_SlopeArray.size())
			m_SlopeArray[i].degree = (m_SlopeArray[i + 1].Level - m_SlopeArray[i].Level) / (m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml);
	}

	////设置首行
	//m_SlopeArray[0].Rshu = 3.0;		//设置里程小数位数
	//m_SlopeArray[0].RLen = 3.0;		//设置高程小数位数
	//WorkingData.SetSlopeData(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname,m_SlopeArray);

}
/*===================================================================
函数名:JudgeSection
------------------------------------------------------------
函数功能: 调整初始方案的分段
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述: 对含有交叉点数组按坡长、坡度规范要求分类讨论
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：	
=====================================================================*/
void ZDMSJ::JudgeSection_ZMSJ()//调整坡度分段
{
	double TempFrontLenth = 0.0; //前一段
	double TempNowLenth = 0.0; //当前段
	double TempBackLenth = 0.0; //后一段
	double MinLongitudinalSlopeL = _wtof(m_PrfDesData.YBPCMin);

	if (m_SlopeArray.size() == 2)
		return;

	for (int i = 0; i + 1 < (int)m_SlopeArray.size() ; i++)
	{
		//第一段
		if (i == 0)
		{
			TempNowLenth = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml; //0-1
			TempBackLenth = m_SlopeArray[i + 2].cml - m_SlopeArray[i + 1].cml; //1-2
			if (TempNowLenth < MinLongitudinalSlopeL) //当前段坡长小于最小坡长
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //差值
				if (TempBackLenth - tempLenth >= MinLongitudinalSlopeL)
				{
					//后一坡段与差值相比大于等于最小坡长 1-后移
					m_SlopeArray[i + 1].cml = m_SlopeArray[i + 1].cml + tempLenth; 
				}
				else
				{
					//小于则删除 i + 1 变坡点
					m_SlopeArray.erase(m_SlopeArray.begin() + i + 1); 
					i--;
				}
			}
		}
		else if (i == m_SlopeArray.size() - 2) 
		{
			TempNowLenth = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml; // num - 1
			TempFrontLenth = m_SlopeArray[i].cml - m_SlopeArray[i - 1].cml; //num - 2

			if (TempNowLenth < MinLongitudinalSlopeL) //当前段坡长小于最小坡长
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //差值
				if (TempFrontLenth - tempLenth >= MinLongitudinalSlopeL)
				{
					//前一坡段与差值相比大于等于最小坡长
					m_SlopeArray[i].cml = m_SlopeArray[i].cml - tempLenth;
				}
				else
				{
					//小于则删除 i + 1 变坡点
					m_SlopeArray.erase(m_SlopeArray.begin() + i);
					i--;
				}
			}
		}
		else
		{
			TempFrontLenth = m_SlopeArray[i].cml - m_SlopeArray[i - 1].cml;
			TempNowLenth = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml;
			TempBackLenth = m_SlopeArray[i + 2].cml - m_SlopeArray[i + 1].cml;
			double temp = m_SlopeArray[i].cml;
			if (TempNowLenth < MinLongitudinalSlopeL) //当前段坡长小于规范一般最小坡长
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //差值
				double maxLenth = max(TempFrontLenth, TempBackLenth); //后坡段与前坡段比较取最大
				if (maxLenth - tempLenth >= MinLongitudinalSlopeL)//可以调整 前后有一段大于最小坡长
				{
					if (TempFrontLenth > TempBackLenth)//前一坡段比后坡段长，则当前坡段往前移动
					{
						m_SlopeArray[i].cml = m_SlopeArray[i].cml - tempLenth;
					}
					else//当前坡段往后移动
					{
						m_SlopeArray[i + 1].cml = m_SlopeArray[i + 1].cml + tempLenth;
					}
				}
				else //都不大于最小坡长
				{
					if (TempFrontLenth > TempBackLenth)//前一坡段比后坡段长，合并到后一坡段
					{
						m_SlopeArray.erase(m_SlopeArray.begin() + i + 1);
					}
					else
					{
						m_SlopeArray.erase(m_SlopeArray.begin() + i);
					}
					i--;
				}
			}
		}

		int num = (int)m_SlopeArray.size();
		if (num <= 2)
			return;
	}
}
/*===================================================================
函数名:JudgeSection
------------------------------------------------------------
函数功能: 调整初始方案的分段
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述: 对含有交叉点数组按坡长、坡度规范要求分类讨论
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：
=====================================================================*/
void ZDMSJ::JudgeSection_ZMSJ(std::vector<PDTAB_ROAD>BPDarray)
{
	double TempFrontLenth = 0.0; //前一段
	double TempNowLenth = 0.0; //当前段
	double TempBackLenth = 0.0; //后一段
	double MinLongitudinalSlopeL = _wtof(m_PrfDesData.YBPCMin);

	if (m_SlopeArray.size() == 2)
		return;

	for (int i = 0; i < (int)BPDarray.size() - 1; i++)
	{
		//第一段
		if (i == 0)
		{
			TempNowLenth = BPDarray[i + 1].cml - BPDarray[i].cml; //0-1
			TempBackLenth = BPDarray[i + 2].cml - BPDarray[i + 1].cml; //1-2
			if (TempNowLenth < MinLongitudinalSlopeL) //当前段坡长小于最小坡长
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //差值
				if (TempBackLenth - tempLenth >= MinLongitudinalSlopeL)
				{
					//后一坡段与差值相比大于等于最小坡长 1-后移
					BPDarray[i + 1].cml = BPDarray[i + 1].cml + tempLenth;
				}
				else
				{
					//小于则删除 i + 1 变坡点
					BPDarray.erase(BPDarray.begin() + i + 1);
					i--;
				}
			}
		}
		else if (i == BPDarray.size() - 2)
		{
			TempNowLenth = BPDarray[i + 1].cml - BPDarray[i].cml; // num - 1
			TempFrontLenth = BPDarray[i].cml - BPDarray[i - 1].cml; //num - 2

			if (TempNowLenth < MinLongitudinalSlopeL) //当前段坡长小于最小坡长
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //差值
				if (TempFrontLenth - tempLenth >= MinLongitudinalSlopeL)
				{
					//前一坡段与差值相比大于等于最小坡长
					BPDarray[i].cml = BPDarray[i].cml - tempLenth;
				}
				else
				{
					//小于则删除 i + 1 变坡点
					BPDarray.erase(BPDarray.begin() + i);
					i--;
				}
			}
		}
		else
		{
			TempFrontLenth = BPDarray[i].cml - BPDarray[i - 1].cml;
			TempNowLenth = BPDarray[i + 1].cml - BPDarray[i].cml;
			TempBackLenth = BPDarray[i + 2].cml - BPDarray[i + 1].cml;
			double temp = BPDarray[i].cml;
			if (TempNowLenth < MinLongitudinalSlopeL) //当前段坡长小于规范一般最小坡长
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //差值
				double maxLenth = max(TempFrontLenth, TempBackLenth); //后坡段与前坡段比较取最大
				if (maxLenth - tempLenth >= MinLongitudinalSlopeL)//可以调整 前后有一段大于最小坡长
				{
					if (TempFrontLenth > TempBackLenth)//前一坡段比后坡段长，则当前坡段往前移动
					{
						BPDarray[i].cml = BPDarray[i].cml - tempLenth;
					}
					else//当前坡段往后移动
					{
						BPDarray[i + 1].cml = BPDarray[i + 1].cml + tempLenth;
					}
				}
				else //都不大于最小坡长
				{
					if (TempFrontLenth > TempBackLenth)//前一坡段比后坡段长，合并到后一坡段
					{
						BPDarray.erase(BPDarray.begin() + i + 1);
					}
					else
					{
						BPDarray.erase(BPDarray.begin() + i);
					}
					i--;
				}
			}
		}

		int num = (int)m_SlopeArray.size();
		if (num <= 2)
			return;
	}
}

//根据桩号里程排序
//根据统一里程给数据排序，并删除相同的桩号
template <typename compara>
void ZDMSJ::SortData(vector<compara> &dataAry)
{

	compara tepArray;
	for (int i = 0; i < dataAry.size(); i++)
	{
		for (int j = i + 1; j < dataAry.size(); j++)
		{
			if (dataAry[i].ml > dataAry[j].ml)
			{
				tepArray = dataAry[j];
				dataAry[j] = dataAry[i];
				dataAry[i] = tepArray;
			}
		}
	}

	//删除相同的桩号
	for (int i = 0; i + 1 < dataAry.size(); i++)
	{
		if (fabs(dataAry[i + 1].ml - dataAry[i].ml) < 0.1)
		{
			dataAry.erase(dataAry.begin() + i);
			i--;
		}
	}
}

/*===================================================================
函数名:GetThreeDiffQuotientPt
------------------------------------------------------------
函数功能:计算平顺地面线的三阶差商
------------------------------------------------------------
输入:	std::vector<DMXYS>&		EarthLine	地面线点
------------------------------------------------------------
输出:	std::vector<DMXYS>& SmoothedEarthLine	平顺后的点
		std::vector<DMXYS>& ThreeDiffQuotientPt	三阶差商点
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：从Automatic.cpp中移植并修改适应本项目数据结构
=====================================================================*/

void ZDMSJ::GetThreeDiffQuotientPt_ZMSJ(std::vector<DMXYS>& EarthLine, std::vector<DMXYS>& SmoothedEarthLine, std::vector<DMXYS>& ThreeDiffQuotientPt)
{
	SmoothEarthline_ZMSJ(_wtof(m_PrfDesData.YBPCMin), 5, EarthLine, SmoothedEarthLine);
	ThreeDiffQuotientPt.clear();
	DMXYS temp;
	for (int i = 0; i < SmoothedEarthLine.size(); i++)
	{
		if (i == 0)
		{
			temp.level = (SmoothedEarthLine[i + 1].level - SmoothedEarthLine[i].level) / (SmoothedEarthLine[i + 1].lc - SmoothedEarthLine[i].lc);
			temp.lc = SmoothedEarthLine[i].lc;
		}
		else if (i == SmoothedEarthLine.size() - 1 || i == SmoothedEarthLine.size() - 2)
		{
			temp.level = (SmoothedEarthLine[i].level - SmoothedEarthLine[i - 1].level) / (SmoothedEarthLine[i].lc - SmoothedEarthLine[i - 1].lc);
			temp.lc = SmoothedEarthLine[i].lc;
		}
		else
		{
			//double H0 = SmoothedEarthLine[i - 1].level;
			//double H1 = SmoothedEarthLine[i].level;
			//double H2 = SmoothedEarthLine[i + 1].level;
			//double H3 = SmoothedEarthLine[i + 2].level;
			//double L1 = SmoothedEarthLine[i].lc - SmoothedEarthLine[i - 1].lc;
			//double L2 = SmoothedEarthLine[i + 1].lc - SmoothedEarthLine[i].lc;
			//double L3 = SmoothedEarthLine[i + 2].lc - SmoothedEarthLine[i + 1].lc;

			//double BefH = ((H2 - H1) / L2 - (H1 - H0) / L1) / (L1 + L2);
			//double AftH = ((H3 - H2) / L3 - (H2 - H1) / L2) / (L2 + L3);
			//double CurH = (AftH - BefH) / (L1 + L2 + L3);
			//temp.level = CurH;
			//temp.lc = SmoothedEarthLine[i].lc;

			double temp1 = SmoothedEarthLine[i].level - SmoothedEarthLine[i - 1].level;
			double temp2 = SmoothedEarthLine[i + 1].lc - SmoothedEarthLine[i].lc;
			double temp3 = SmoothedEarthLine[i + 1].level - SmoothedEarthLine[i].level;
			double temp4 = SmoothedEarthLine[i].lc - SmoothedEarthLine[i - 1].lc;
			double temp5 = SmoothedEarthLine[i + 1].lc - SmoothedEarthLine[i - 1].lc;
			temp.level = (temp1 * temp2 / temp4 + temp3 * temp4 / temp2) / temp5;
			temp.lc = SmoothedEarthLine[i].lc;
		}
		ThreeDiffQuotientPt.push_back(temp);
	}
}
/*===================================================================
函数名:SmoothEarthline
------------------------------------------------------------
函数功能:平顺地面线
------------------------------------------------------------
输入:	double	MinSlopeLen		最小坡长
		int		SmoothNum		平顺次数
		std::vector<DMXYS>& EarthLine	原地面线点数组
------------------------------------------------------------
输出:	std::vector<DMXYS>& SmoothedEarthLine	平顺后的地面线点数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：从Automatic.cpp中PsDmx()移植并修改适应本项目数据结构
=====================================================================*/
void ZDMSJ::SmoothEarthline_ZMSJ(double MinSlopeLen, int SmoothNum, std::vector<DMXYS>& EarthLine, std::vector<DMXYS>& SmoothedEarthLine)
{
	if (SmoothNum == 0)
	{
		SmoothedEarthLine = EarthLine;
		return;
	}

	int i, j;
	DMXYS tmpPt;
	tmpPt.lc = 0.00; tmpPt.level = 0.00;

	SmoothedEarthLine.clear();

	for (i = 0; i < EarthLine.size(); i++)    //第一次平顺
	{
		tmpPt.lc = EarthLine[i].lc;
		tmpPt.level = GetSmoothedHigh_ZMSJ(i, MinSlopeLen, EarthLine);
		SmoothedEarthLine.push_back(tmpPt);
	}

	for (i = 1; i < SmoothNum; i++)                 //如果平顺次数大于1，继续平顺
	{
		for (j = 0; j < SmoothedEarthLine.size(); j++)
		{
			tmpPt.lc = SmoothedEarthLine[j].lc;
			//tmpPt.level = GetSmoothedHigh(j, MinSlopeLen, 1, PSdmxPt);
			tmpPt.level = GetSmoothedHigh_ZMSJ(j, MinSlopeLen, SmoothedEarthLine);
			SmoothedEarthLine[j] = tmpPt;
		}
	}
}
/*===================================================================
函数名:GetSmoothedHigh
------------------------------------------------------------
函数功能:获取平顺后的地面线高程
------------------------------------------------------------
输入:	int i	索引号
		double MinSlopeLen	最小坡长
		std::vector<DMXYS>& EarthLine	地面线点数组
------------------------------------------------------------
输出:	double 返回值	平顺后的地面线高程
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：	从Automatic.cpp中
		GetPsHigh(int i,double R,UltraCArray_T<gridPoint,gridPoint> &ZMdmxPt)
		移植并修改适应本项目数据结构
=====================================================================*/
double ZDMSJ::GetSmoothedHigh_ZMSJ(int i, double MinSlopeLen, std::vector<DMXYS>& EarthLine)
{
	double totalL(0.0), totalR(0.0), dltX(0.0);
	double FLeft(0.0), FRight(0.0);
	double high;
	int j;
	//计算左侧
	j = 1;

	if (i - j > 0)
	{
		dltX = fabs(EarthLine[i - j].lc - EarthLine[i].lc);
	}

	while (i - j > 0 && dltX <= MinSlopeLen)
	{
		totalL += EarthLine[i - j].level * (MinSlopeLen - dltX) / MinSlopeLen;
		FLeft += (MinSlopeLen - dltX) / MinSlopeLen;
		j++;
		if (i - j > 0)
		{
			dltX = fabs(EarthLine[i - j].lc - EarthLine[i].lc);
		}
	}

	dltX = 0.0;
	//计算右侧
	j = 1;
	if (i + j <= EarthLine.size() - 1)
	{
		dltX = fabs(EarthLine[i + j].lc - EarthLine[i].lc);
	}

	while (i + j <= EarthLine.size() - 1 && dltX <= MinSlopeLen)
	{
		totalR += EarthLine[i + j].level * (MinSlopeLen - dltX) / MinSlopeLen;
		FRight += (MinSlopeLen - dltX) / MinSlopeLen;
		j++;
		if (i + j <= EarthLine.size() - 1)
		{
			dltX = fabs(EarthLine[i + j].lc - EarthLine[i].lc);
		}
	}

	//求得高程
	high = (totalL + totalR + EarthLine[i].level) / (FLeft + FRight + 1.0);

	return high;
}

/*===================================================================
函数名:GetSmoothedHigh
------------------------------------------------------------
函数功能:获取平顺后的地面线高程
------------------------------------------------------------
输入:	int i	索引号
		double MinSlopeLen	最小坡长
		int tp
		std::vector<DMXYS>& EarthLine	地面线点数组
------------------------------------------------------------
输出:	double 返回值	平顺后的地面线高程
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：	从Automatic.cpp中
		GetPsHigh(int i,double R,UltraCArray_T<gridPoint,gridPoint> &ZMdmxPt)
		移植并修改适应本项目数据结构
=====================================================================*/
double ZDMSJ::GetSmoothedHigh_ZMSJ(int i, double MinSlopeLen, int tp, std::vector<DMXYS>& EarthLine)
{
	return 0.0;
}
/*===================================================================
函数名:GetOneDiffQuotientPt
------------------------------------------------------------
函数功能:计算一阶差商
------------------------------------------------------------
输入:	std::vector<DMXYS>& ThreeDiffQuotientPt	三阶差商数组
------------------------------------------------------------
输出:	std::vector<DMXYS>& OneDiffQuotientPt	一阶插商数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		GetOneDiffQuotientPt(UltraCArray_T<gridPoint,gridPoint> &ThreeDiffQuotientPt,UltraCArray_T<gridPoint,gridPoint> &OneDiffQuotientPt);
		移植并修改适应本项目数据结构
=====================================================================*/
void ZDMSJ::GetOneDiffQuotientPt_ZMSJ(std::vector<DMXYS>& ThreeDiffQuotientPt, std::vector<DMXYS>& OneDiffQuotientPt)
{
	OneDiffQuotientPt.clear();
	DMXYS temp;
	for (int i = 0; i < ThreeDiffQuotientPt.size(); i++)
	{
		if (i == 0)
		{
			temp.level = (ThreeDiffQuotientPt[i + 1].level - ThreeDiffQuotientPt[i].level) / (ThreeDiffQuotientPt[i + 1].lc - ThreeDiffQuotientPt[i].lc);
			temp.lc = ThreeDiffQuotientPt[i].lc;
		}
		else if (i == ThreeDiffQuotientPt.size() - 1)
		{
			temp.level = (ThreeDiffQuotientPt[i].level - ThreeDiffQuotientPt[i - 1].level) / (ThreeDiffQuotientPt[i].lc - ThreeDiffQuotientPt[i - 1].lc);
			temp.lc = ThreeDiffQuotientPt[i].lc;
		}
		else
		{
			double temp1 = ThreeDiffQuotientPt[i + 1].level - ThreeDiffQuotientPt[i - 1].level;
			double temp2 = ThreeDiffQuotientPt[i + 1].lc - ThreeDiffQuotientPt[i - 1].lc;
			temp.level = temp1 / temp2;
			temp.lc = ThreeDiffQuotientPt[i].lc;
		}
		OneDiffQuotientPt.push_back(temp);
	}
}


/*===================================================================
函数名:GetChangePt
------------------------------------------------------------
函数功能:获取变坡点
------------------------------------------------------------
输入:	JD_CENTER* pRoad	道路平面指针
		std::vector<DMXYS>& OneDiffQuotientPt	一阶差商点
------------------------------------------------------------
输出:	std::vector<DMXYS>& ChangePt	变坡点数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void GetChangePt(PlaneCAL* pGTZX,UltraCArray_T<gridPoint,gridPoint> &OneDiffQuotientPt,UltraCArray_T<gridPoint,gridPoint> &ChangePt);
		移植并修改适应本项目数据结构
=====================================================================*/
void ZDMSJ::GetChangePt_ZMSJ(JD_CENTER* pRoad, std::vector<DMXYS>& OneDiffQuotientPt, std::vector<DMXYS>& ChangePt)
{
	ChangePt.clear();
	DMXYS temppt;

	/////////////////////////////////加入起点////////////////////////////
	temppt.lc = 0.0;
	temppt.level = m_StartH;
	ChangePt.push_back(temppt);

	for (int Crsi = 0; Crsi + 1 < OneDiffQuotientPt.size() ; Crsi++)
	{
		if (OneDiffQuotientPt[Crsi].level * 1.0e10 * OneDiffQuotientPt[Crsi + 1].level * 1.0e10 < 0.000001)
		{
			ChangePt.push_back(OneDiffQuotientPt[Crsi]);
		}
	}

	////////////////////////////////加入终点///////////////////////////////
	temppt.lc = pRoad->LenOfLine;
	temppt.level = m_EndH;
	ChangePt.push_back(temppt);

}

/*===================================================================
函数名:IniSlopeArrray
------------------------------------------------------------
函数功能:初始化坡度数组
------------------------------------------------------------
输入:	std::vector<DMXYS>& OneDiffQuotientPt	变坡点数组
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：
=====================================================================*/
void ZDMSJ::IniSlopeArrray_ZMSJ(std::vector<DMXYS>& ChangePt)
{
	//设置参数
	PDTAB_ROAD tempSlope;

	//初始化坡度数组
	m_SlopeArray.clear();
	for (int i = 0; i < ChangePt.size(); i++)
	{
		tempSlope.cml = ChangePt[i].lc;
		tempSlope.degree = 0.0;
		tempSlope.Level = 0.0;
		tempSlope.length = _wtof(m_PrfDesData.YBPCMin);
		tempSlope.RLen = 0.0;

		if (i > 0 && i + 1 < ChangePt.size())
			tempSlope.Rshu = _wtof(m_PrfDesData.A_YBRshuMin);
		else
			tempSlope.Rshu = 0.0;

		m_SlopeArray.push_back(tempSlope);
	}
}

/*===================================================================
函数名:CreatPlanMiu
------------------------------------------------------------
函数功能:生成各变坡点标高
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool CreatPlanMiu(PlanePlan &oneplane, bool IsMinTW,bool YorN);
		移植并修改适应本项目数据结构
=====================================================================*/
bool ZDMSJ::CreatPlanMiu_ZMSJ()
{
	for (int SlopePtIndex = 0; SlopePtIndex < m_SlopeArray.size(); SlopePtIndex++)
	{
		if (SlopePtIndex == 0)//起点
			m_SlopeArray[SlopePtIndex].Level = m_StartH;
		else if (SlopePtIndex == m_SlopeArray.size() - 1)//终点
			m_SlopeArray[SlopePtIndex].Level = m_EndH;
		else
		{
			bool tempD;
			tempD = CreateSloptPtDesH_ZMSJ(SlopePtIndex);

			if (tempD == false)
				return false;
		}

	}
	return true;
}
/*===================================================================
函数名:CreateSloptPtDesH
------------------------------------------------------------
函数功能:生成第索引为SlopePtIndex的变坡点设计高程
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool CreateSloptPtDesH(PlanePlan &oneplane,int SlopePtIndex, bool IsMinTW,bool YorN );
		移植并修改适应本项目数据结构
=====================================================================*/
bool ZDMSJ::CreateSloptPtDesH_ZMSJ(int SlopePtIndex)
{
	double dmxnumber;
	double minH, maxH;
	double temp;
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	FrontMileage = m_SlopeArray[SlopePtIndex - 1].cml;
	Fronthigh = m_SlopeArray[SlopePtIndex - 1].Level;
	NowMileage = m_SlopeArray[SlopePtIndex].cml;

	double MiuDesH = 0.0;
	RoadDesignWorkingDatabase WorkingBase;
	JD_CENTER* pRoad = WorkingBase.CreateHorizontalRoad(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname);

	AdjustGCKZD_ZMSJ(m_SlopeArray[SlopePtIndex - 1].cml,
		m_SlopeArray[SlopePtIndex - 1].Level,
		pRoad->LenOfLine, m_EndH);

	Env_GetMiu_ZMSJ(SlopePtIndex, MiuDesH);

	bool tempDR = CalSloptPtDesHRange_ZMSJ(SlopePtIndex, minH, maxH);

	/************************************************************************/
	/* 依据坡度代数差确定当前变坡点的最小最大高程 */
	/************************************************************************/
	double miu = 0.0;//期望
	bool cheekKZD = true;//用于后面判断变坡点在高程必经点以前否
	bool cheek = true;//高程控制点在该坡段内时为false
	double cml = 0.0;
	double Gckzd = 0.0;
	double tempPD = 0.0;
	if (maxH < minH - 0.01)
		return false;
	if (maxH < minH)
	{
		double templever = minH;
		minH = maxH;
		maxH = templever;
	}
	//////////////////////////////////////////////////////////////////////////
	//double scale = pow(10.0, 3);
	double scale = 1.0;
	int MaxRoundGrade = floor((maxH - Fronthigh) / (NowMileage - FrontMileage) * scale * 100.0 + 0.001);
	int MinRoundGrade = ceil((minH - Fronthigh) / (NowMileage - FrontMileage) * scale * 100.0 - 0.001);
	if ((MaxRoundGrade - MinRoundGrade) < 2)
		tempPD = MinRoundGrade / scale / 100.0;
	else
	{
		//miu = floor((MiuDesH - Fronthigh) / (NowMileage - FrontMileage) * scale * 100.0 + 0.5);
		//double sigma = (MaxRoundGrade - MinRoundGrade) / 1.5;//方差 
		double sigma = min(maxH - MiuDesH, MiuDesH - minH) / 3.0;//方差
		double DesH = NormalRandom_ZMSJ(MiuDesH, sigma, Fronthigh + (NowMileage - FrontMileage)* MinRoundGrade / 100, Fronthigh + (NowMileage - FrontMileage)*MaxRoundGrade / 100);
		miu = (DesH - Fronthigh) / (NowMileage - FrontMileage) * scale * 100.0;
		if (miu < MinRoundGrade)
		{
			tempPD = MinRoundGrade / scale / 100.0;
		}
		else if (miu > MaxRoundGrade)
		{
			tempPD = MaxRoundGrade / scale / 100.0;
		}
		else
		{
			//tempPD = NormalRandom_ZMSJ(miu, sigma, MinRoundGrade, MaxRoundGrade);
			tempPD = miu;
			//tempPD = floor(tempPD + 0.5);

			tempPD = tempPD / scale / 100.0;

		}
	}
	temp = Fronthigh + (NowMileage - FrontMileage) * tempPD;

	///////////////////////////////调整坡度，计算最终设计高程///////////////////////////////////
	double TempH = temp;
	if (cheek == false)
	{
		TempH = (NowMileage - FrontMileage) * (Gckzd - Fronthigh) / (cml - FrontMileage) + Fronthigh;
		tempPD = (TempH - Fronthigh) / (NowMileage - FrontMileage) * 100.0;
		tempPD = floor(tempPD * scale + 0.5) / scale;
		TempH = Fronthigh + (NowMileage - FrontMileage) * tempPD / 100.0;
		m_SlopeArray[SlopePtIndex].Level = TempH;
	}
	else
	{
		m_SlopeArray[SlopePtIndex].Level = TempH;
	}
	if (m_SlopeArray[SlopePtIndex].Level > maxH)
	{
		m_SlopeArray[SlopePtIndex].Level = maxH;
		m_SlopeArray[SlopePtIndex].degree = (maxH - Fronthigh) / (NowMileage - FrontMileage);
	}
	if (m_SlopeArray[SlopePtIndex].Level < minH)
	{
		m_SlopeArray[SlopePtIndex].Level = minH;
		m_SlopeArray[SlopePtIndex].degree = (minH - Fronthigh) / (NowMileage - FrontMileage);
	}

	return true;
}

/*===================================================================
函数名:AdjustGCKZD
------------------------------------------------------------
函数功能:调整高程控制点
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void AdjustGCKZD(PlanePlan &oneplane, double StartCml, double StartH, double EndCml, double EndH, bool IsAdjustDMX=true);
		移植并修改适应本项目数据结构
=====================================================================*/
void ZDMSJ::AdjustGCKZD_ZMSJ(double StartCml, double StartH, double EndCml, double EndH)
{
	std::vector<GCKZPt> SX_GCKZDArry, XX_GCKZDArry;	//上下限高程控制点数组
	GCKZPt TempGCKZD;
	std::vector<GCKZPt> Temp_SX_GCKZDArry, Temp_XX_GCKZDArry;//临时高程控制点数组
	double MinH, MaxH;

	//起点
	TempGCKZD.Cml = StartCml;
	TempGCKZD.DesHSX = StartH;
	TempGCKZD.DesHXX = StartH;

	TempGCKZD.Style = 1;
	SX_GCKZDArry.push_back(TempGCKZD);

	TempGCKZD.Style = 2;
	XX_GCKZDArry.push_back(TempGCKZD);

	//终点
	TempGCKZD.Cml = EndCml;
	TempGCKZD.DesHSX = EndH;
	TempGCKZD.DesHXX = EndH;

	TempGCKZD.Style = 1;
	SX_GCKZDArry.push_back(TempGCKZD);

	TempGCKZD.Style = 2;
	XX_GCKZDArry.push_back(TempGCKZD);

	//起终点形成信封区域
	GetEnvelop_ZMSJ(SX_GCKZDArry, XX_GCKZDArry);

	//整理出用户设置的上下限控制点,先考虑下限，再考虑上限
	InsertGCKZDArray_ZMSJ(SX_GCKZDArry, XX_GCKZDArry, m_UserSetGCKZDArray_SX, true);
	InsertGCKZDArray_ZMSJ(SX_GCKZDArry, XX_GCKZDArry, m_UserSetGCKZDArray_XX, false);

	GetEnvelop_ZMSJ(SX_GCKZDArry, XX_GCKZDArry);
}

/*===================================================================
函数名:GetEnvelop
------------------------------------------------------------
函数功能:调整高程控制点
------------------------------------------------------------
输入:	std::vector<GCKZPt>& SX_GCKZDArry	上行控制点数组
		std::vector<GCKZPt>& XX_GCKZDArry	下行控制点数组
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void GetEnvelop(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry);
		移植并修改适应本项目数据结构
=====================================================================*/
void ZDMSJ::GetEnvelop_ZMSJ(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry)
{
	GCKZPt TempGCKZD;
	//对上限高程控制点进行排序，剔除相同里程处的控制点
	int KZDSum = SX_GCKZDArry.size();
	for (int i = 0; i < KZDSum - 1; i++)
	{
		for (int j = i + 1; j < KZDSum; j++)
		{
			if (SX_GCKZDArry[i].Cml > SX_GCKZDArry[j].Cml)
			{
				TempGCKZD = SX_GCKZDArry[i];
				SX_GCKZDArry[i] = SX_GCKZDArry[j];
				SX_GCKZDArry[j] = TempGCKZD;
			}
		}
	}
	for (int i = 0; i < SX_GCKZDArry.size() - 1; i++)
	{
		if (fabs(SX_GCKZDArry[i].Cml - SX_GCKZDArry[i + 1].Cml) < 0.01)
		{
			if (SX_GCKZDArry[i].DesHSX > SX_GCKZDArry[i + 1].DesHSX)
				SX_GCKZDArry.erase(SX_GCKZDArry.begin() + i);
			else
				SX_GCKZDArry.erase(SX_GCKZDArry.begin() + i + 1);
			i--;
		}
	}

	//对下限高程控制点进行排序，剔除相同里程处的控制点
	KZDSum = XX_GCKZDArry.size();
	for (int i = 0; i < KZDSum - 1; i++)
	{
		for (int j = i + 1; j < KZDSum; j++)
		{
			if (XX_GCKZDArry[i].Cml > XX_GCKZDArry[j].Cml)
			{
				TempGCKZD = XX_GCKZDArry[i];
				XX_GCKZDArry[i] = XX_GCKZDArry[j];
				XX_GCKZDArry[j] = TempGCKZD;
			}
		}
	}
	for (int i = 0; i < XX_GCKZDArry.size() - 1; i++)
	{
		if (fabs(XX_GCKZDArry[i].Cml - XX_GCKZDArry[i + 1].Cml) < 0.01)
		{
			if (XX_GCKZDArry[i].DesHSX < XX_GCKZDArry[i + 1].DesHSX)
				XX_GCKZDArry.erase(XX_GCKZDArry.begin() + i);
			else
				XX_GCKZDArry.erase(XX_GCKZDArry.begin() + i + 1);
			i--;
		}
	}

	//形成信封区域
	MaxEnvPtArray.clear();
	MinEnvPtArray.clear();
	EnvPt TempEnvPt;
	//循环将各个高程控制点加入，每加入一个高程控制点同时，需要将当前控制点与前一个高程控制点之间的点加入
	//两直线求交计算两控制点中间的点
	double BefLineEnd_Mileage, BefLineEnd_Lever;//前一控制点沿着最大最小坡往后走Lengh之后的里程和高程
	double AftLine_Start_Mileage, AftLine_Start_Mileage_Lever;//当前控制点沿着最大最小坡往前走Lengh之后的里程和高程
	int Res = 0;
	double MaxSlope_SX = _wtof(m_PrfDesData.MAXPD);
	double MaxSlope_XX = _wtof(m_PrfDesData.MAXPD);
	double MAX_Cml, MAX_H, MIN_Cml, MIN_H;
	//计算上限点
	//将起点加入
	TempEnvPt.Mileage = SX_GCKZDArry[0].Cml;
	TempEnvPt.Lever = SX_GCKZDArry[0].DesHSX;
	MaxEnvPtArray.push_back(TempEnvPt);
	for (int iKZD = 1; iKZD < SX_GCKZDArry.size(); iKZD++)
	{
		GetJD_ZMSJ(SX_GCKZDArry[iKZD - 1].Cml, SX_GCKZDArry[iKZD - 1].DesHSX, SX_GCKZDArry[iKZD].Cml, SX_GCKZDArry[iKZD].DesHSX, MAX_Cml, MAX_H, MIN_Cml, MIN_H);
		TempEnvPt.Mileage = MAX_Cml;
		TempEnvPt.Lever = MAX_H;
		MaxEnvPtArray.push_back(TempEnvPt);

		TempEnvPt.Mileage = SX_GCKZDArry[iKZD].Cml;
		TempEnvPt.Lever = SX_GCKZDArry[iKZD].DesHSX;
		MaxEnvPtArray.push_back(TempEnvPt);
	}
	//将起点加入下限点数组
	TempEnvPt.Mileage = XX_GCKZDArry[0].Cml;
	TempEnvPt.Lever = XX_GCKZDArry[0].DesHXX;
	MinEnvPtArray.push_back(TempEnvPt);
	//处理下限点
	for (int iKZD = 1; iKZD < XX_GCKZDArry.size(); iKZD++)
	{
		GetJD_ZMSJ(XX_GCKZDArry[iKZD - 1].Cml, XX_GCKZDArry[iKZD - 1].DesHXX, XX_GCKZDArry[iKZD].Cml, XX_GCKZDArry[iKZD].DesHXX, MAX_Cml, MAX_H, MIN_Cml, MIN_H);
		TempEnvPt.Mileage = MIN_Cml;
		TempEnvPt.Lever = MIN_H;
		MinEnvPtArray.push_back(TempEnvPt);

		TempEnvPt.Mileage = XX_GCKZDArry[iKZD].Cml;
		TempEnvPt.Lever = XX_GCKZDArry[iKZD].DesHXX;
		MinEnvPtArray.push_back(TempEnvPt);
	}
}

/*===================================================================
函数名:GetJD
------------------------------------------------------------
函数功能:获取交点
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void GetEnvelop(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry);
		移植并修改适应本项目数据结构
=====================================================================*/
int ZDMSJ::GetJD_ZMSJ(double Scml, double SLever, double Ecml, double ELever,
	double& MAX_cml, double& MAX_H, double& MIN_cml, double& MIN_H)
{
	///////////////////////////////////////得到信封区域的交点//////////////////////////////////

	double QZminH = 0.0;//起终点确定的最小高程
	double QZmaxH = 0.0;//起终点确定的最大高程
	double x1 = Scml;
	double y1 = SLever;

	double maxx2 = Ecml;
	double maxy2 = y1 + (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	double x3 = Ecml;
	double y3 = ELever;

	double maxx4 = x1;
	double maxy4 = y3 + (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	BAS_DRAW_FUN::INTERS(&MAX_cml, &MAX_H, x1, y1, maxx2, maxy2, x3, y3, maxx4, maxy4);//计算信封区域的最高点
	if (MAX_cml < Scml)
	{
		MAX_cml = Scml;
		MAX_H = SLever;
		return -1;
	}
	if (MAX_cml > Ecml)
	{
		MAX_cml = Ecml;
		MAX_H = ELever;
		return 1;
	}


	double minx2 = Ecml;
	double miny2 = y1 - (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	double minx4 = x1;
	double miny4 = y3 - (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	double MinY;
	BAS_DRAW_FUN::INTERS(&MIN_cml, &MIN_H, x1, y1, minx2, miny2, x3, y3, minx4, miny4);//计算信封区域的最低点
	if (MIN_cml < Scml)
	{
		MIN_cml = Scml;
		MIN_H = SLever;
		return -1;
	}
	if (MIN_cml > Ecml)
	{
		MIN_cml = Ecml;
		MIN_H = ELever;
		return 1;
	}

	return 0;
}

/*===================================================================
函数名:InsertGCKZDArray
------------------------------------------------------------
函数功能:插入控制点数组
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void InsertGCKZDArray(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry,
		UltraCArray_T<GCKZPt,GCKZPt>& GCKZDArray, bool IsSX);
		移植并修改适应本项目数据结构
=====================================================================*/
void ZDMSJ::InsertGCKZDArray_ZMSJ(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry, std::vector<GCKZPt>& GCKZDArray, bool IsSX)
{
	GCKZPt TempGCKZD;
	double MinH, MaxH;
	for (int i = 0; i < GCKZDArray.size(); i++)
	{
		TempGCKZD = GCKZDArray[i];
		GetEnvelopRange_ZMSJ(TempGCKZD.Cml, MinH, MaxH);
		if (IsSX)
		{
			if (TempGCKZD.DesHSX<MinH - 00.1 || TempGCKZD.DesHSX>MaxH + 0.01)
				continue;
			else
			{
				InsertGCKZD_ZMSJ(SX_GCKZDArry, TempGCKZD);
				GetEnvelop_ZMSJ(SX_GCKZDArry, XX_GCKZDArry);
			}
		}
		else
		{
			if (TempGCKZD.DesHXX<MinH - 00.1 || TempGCKZD.DesHXX>MaxH + 0.01)
				continue;
			else
			{
				InsertGCKZD_ZMSJ(XX_GCKZDArry, TempGCKZD);
				GetEnvelop_ZMSJ(SX_GCKZDArry, XX_GCKZDArry);
			}
		}
	}
}

/*===================================================================
函数名:GetEnvelopRange
------------------------------------------------------------
函数功能:得到信封区域所确定的里程范围
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool GetEnvelopRange(double Mileage,double& MinH, double& MaxH);
		移植并修改适应本项目数据结构
=====================================================================*/
bool ZDMSJ::GetEnvelopRange_ZMSJ(double Mileage, double& MinH, double& MaxH)
{
	MinH = 1e99;
	MaxH = -1e99;
	double a, b, c, d;
	int MaxEnvPtSum = MaxEnvPtArray.size();
	int MinEnvPtSum = MinEnvPtArray.size();
	if (MaxEnvPtSum == 0 || MinEnvPtSum == 0)
		return false;

	int i = 0;
	int j = 0;

	if (Mileage <= MaxEnvPtArray[0].Mileage || Mileage <= MinEnvPtArray[0].Mileage)
	{
		MaxH = m_StartH;
		MinH = m_StartH;

		return false;
	}

	if (Mileage >= MaxEnvPtArray[MaxEnvPtSum - 1].Mileage + 0.001 || Mileage >= MinEnvPtArray[MinEnvPtSum - 1].Mileage + 0.001)
	{
		MaxH = m_EndH;
		MinH = m_EndH;

		return false;
	}

	for (i = 1; i < MaxEnvPtSum; i++)
	{
		if (Mileage < MaxEnvPtArray[i].Mileage)//找到当前坡段的后一个信封区域点
		{
			if (fabs(MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage) < 0.1)
			{
				MaxH = MaxEnvPtArray[i].Lever;
			}
			else
			{
				a = MaxEnvPtArray[i - 1].Mileage;
				b = MaxEnvPtArray[i - 1].Lever;
				c = MaxEnvPtArray[i].Mileage;
				d = MaxEnvPtArray[i].Lever;
				MaxH = MaxEnvPtArray[i - 1].Lever +
					(Mileage - MaxEnvPtArray[i - 1].Mileage) * (MaxEnvPtArray[i].Lever - MaxEnvPtArray[i - 1].Lever) / (MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage);
			}
			break;
		}
	}
	for (i = 1; i < MinEnvPtSum; i++)
	{
		if (Mileage < MinEnvPtArray[i].Mileage)//找到第一个大于当前里程的信封区域点
		{
			if (fabs(MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage) < 0.1)
			{
				MinH = MinEnvPtArray[i].Lever;
			}
			else
			{
				a = MinEnvPtArray[i - 1].Mileage;
				b = MinEnvPtArray[i - 1].Lever;
				c = MinEnvPtArray[i].Mileage;
				d = MinEnvPtArray[i].Lever;
				MinH = MinEnvPtArray[i - 1].Lever +
					(Mileage - MinEnvPtArray[i - 1].Mileage) * (MinEnvPtArray[i].Lever - MinEnvPtArray[i - 1].Lever) / (MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage);
			}

			break;
		}
	}
	return true;
}

/*===================================================================
函数名:GetEnvelopRange
------------------------------------------------------------
函数功能:得到信封区域所确定的里程范围
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
			bool GetEnvelopRange(double PreDesH, double PreMileage, double Mileage,double& MinH, double& MaxH);
		移植并修改适应本项目数据结构
=====================================================================*/
bool ZDMSJ::GetEnvelopRange_ZMSJ(double PreDesH, double PreMileage, double Mileage, double& MinH, double& MaxH)
{
	MinH = 1e99;
	MaxH = -1e99;
	double a, b, c, d;
	int MaxEnvPtSum = MaxEnvPtArray.size();
	int MinEnvPtSum = MinEnvPtArray.size();
	if (MaxEnvPtSum == 0 || MinEnvPtSum == 0)
		return false;

	int i = 0;
	int j = 0;

	if (Mileage < MaxEnvPtArray[0].Mileage || Mileage < MinEnvPtArray[0].Mileage)
	{
		MaxH = m_StartH;
		MinH = m_StartH;
		return false;
	}

	if (Mileage > MaxEnvPtArray[MaxEnvPtSum - 1].Mileage + 0.001 || Mileage > MinEnvPtArray[MinEnvPtSum - 1].Mileage + 0.001)
	{
		MaxH = m_EndH;
		MinH = m_EndH;
		return false;
	}

	for (i = 1; i < MaxEnvPtSum; i++)
	{
		if (Mileage < MaxEnvPtArray[i].Mileage)//找到当前坡段的后一个信封区域点
		{
			//for (j = i-1; j>=0; j--)
			//{
			//	if (MaxEnvPtArray[j].Mileage < PreMileage+0.01)//找到当前坡段的前一个信封区域点
			//		break;
			//}

			//if (j>=0 && (i-j)>1)//该坡度内有信封区域点j+1到i，所有点取最小数
			//{
			//	double Slope, MinSlope = 100000.0;
			//	for (int k=j+1; k<=i; k++)
			//	{
			//		Slope = (MaxEnvPtArray[k].Lever - PreDesH) / (MaxEnvPtArray[k].Mileage-PreMileage);
			//		if (Slope < MinSlope)
			//			MinSlope = Slope;
			//	}
			//	MaxH = PreDesH + (Mileage-PreMileage) * MinSlope;
			//}

			//else
			{
				if (fabs(MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage) < 0.1)
				{
					MaxH = MaxEnvPtArray[i].Lever;
				}
				else
				{
					a = MaxEnvPtArray[i - 1].Mileage;
					b = MaxEnvPtArray[i - 1].Lever;
					c = MaxEnvPtArray[i].Mileage;
					d = MaxEnvPtArray[i].Lever;

					MaxH = MaxEnvPtArray[i - 1].Lever +
						(Mileage - MaxEnvPtArray[i - 1].Mileage) * (MaxEnvPtArray[i].Lever - MaxEnvPtArray[i - 1].Lever) / (MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage);
				}
			}

			break;
		}
	}
	for (i = 1; i < MinEnvPtSum; i++)
	{
		if (Mileage < MinEnvPtArray[i].Mileage)//找到第一个大于当前里程的信封区域点
		{
			//for (j = i-1; j>=0; j--)
			//{
			//	if (MinEnvPtArray[j].Mileage < PreMileage+0.01)//找到当前坡段的前一个信封区域点
			//		break;
			//}

			//if ((j>=0) && (i-j>1))//该坡度内有信封区域点j+1到i，所有点取最小数
			//{
			//	double Slope, MaxSlope = -100000.0;
			//	for (int k=j+1; k<=i; k++)
			//	{
			//		Slope = (MinEnvPtArray[k].Lever - PreDesH) / (MinEnvPtArray[k].Mileage-PreMileage);
			//		if (Slope > MaxSlope)
			//			MaxSlope = Slope;
			//	}
			//	MinH = PreDesH + (Mileage-PreMileage) * MaxSlope;
			//}

			//else
			{
				if (fabs(MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage) < 0.1)
				{
					MinH = MinEnvPtArray[i].Lever;
				}
				else
				{
					a = MaxEnvPtArray[i - 1].Mileage;
					b = MaxEnvPtArray[i - 1].Lever;
					c = MaxEnvPtArray[i].Mileage;
					d = MaxEnvPtArray[i].Lever;
					MinH = MinEnvPtArray[i - 1].Lever +
						(Mileage - MinEnvPtArray[i - 1].Mileage) * (MinEnvPtArray[i].Lever - MinEnvPtArray[i - 1].Lever) / (MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage);
				}
			}

			break;
		}
	}
	return true;
}


/*===================================================================
函数名:InsertGCKZD
------------------------------------------------------------
函数功能:查找高程控制点在数组中的位置
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	1.从Automatic.cpp中
		void InsertGCKZD(UltraCArray_T<GCKZPt,GCKZPt>& GCKZDArry, GCKZPt KZD);
		移植并修改适应本项目数据结构
		2.INTERS函数暂时无效，这是直接从BAS_DRAW_FUN中找了一个函数
=====================================================================*/
void ZDMSJ::InsertGCKZD_ZMSJ(std::vector<GCKZPt>& GCKZDArry, GCKZPt KZD)
{
	int Index = 0;
	GCKZPt PreKZD, NextKZD;
	double InterCml, InterH;
	double x1, y1, x2, y2, x3, y3, x4, y4;
	double Slope = _wtof(m_PrfDesData.MAXPD);
	double SlopeLen = 0.0;//坡长
	bool IsInsertStart = false, IsInsertEnd = false;
	for (int i = 0; i < GCKZDArry.size() - 1; i++)
	{
		if ((KZD.Cml > GCKZDArry[i].Cml + 0.1) && (KZD.Cml < GCKZDArry[i + 1].Cml + 0.1))//找到控制点KZD的插入位置
		{
			PreKZD = GCKZDArry[i];
			NextKZD = GCKZDArry[i + 1];
			//检查该控制点、前/后一个控制点是否有多余的
			if (KZD.Style == 1)//上限控制点
			{
				SlopeLen = (KZD.Cml - PreKZD.Cml);
				x1 = KZD.Cml;
				y1 = PreKZD.DesHSX - SlopeLen * Slope;
				x2 = PreKZD.Cml;
				y2 = KZD.DesHSX - SlopeLen * Slope;

				SlopeLen = (NextKZD.Cml - KZD.Cml);
				x3 = KZD.Cml;
				y3 = NextKZD.DesHSX - SlopeLen * Slope;
				x4 = NextKZD.Cml;
				y4 = KZD.DesHSX - SlopeLen * Slope;

				//if (0 == INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHSX, x1, y1, x2, y2, KZD.Cml, KZD.DesHSX))//有实交点，插入该控制点
				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHSX, x1, y1, x2, y2, KZD.Cml, KZD.DesHSX))//有实交点，插入该控制点
					IsInsertStart = true;
				else
					IsInsertStart = false;
				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, KZD.Cml, KZD.DesHSX, x4, y4, x3, y3, NextKZD.Cml, NextKZD.DesHSX))
					IsInsertEnd = true;
				else
					IsInsertEnd = false;

				if (IsInsertStart && IsInsertEnd)
				{
					//GCKZDArry.InsertAt(i + 1, KZD);
					GCKZDArry.insert(GCKZDArry.begin() + i + 1, KZD);
					return;
				}

				if (!IsInsertStart)//考虑前一个控制点不能插入
				{
					if (KZD.DesHSX < PreKZD.DesHSX)//新加入的小，覆盖掉前面那个，否则保持不变
						GCKZDArry[i] = KZD;
				}

				if (!IsInsertEnd)//考虑后一个控制点不能插入
				{
					if (KZD.DesHSX < NextKZD.DesHSX)//新加入的小，覆盖掉前面那个，否则保持不变
						GCKZDArry[i + 1] = KZD;
				}
			}

			if (KZD.Style == 2)//下限控制点
			{
				SlopeLen = (KZD.Cml - PreKZD.Cml);
				x1 = KZD.Cml;
				y1 = PreKZD.DesHXX + SlopeLen * Slope;
				x2 = PreKZD.Cml;
				y2 = KZD.DesHXX + SlopeLen * Slope;

				SlopeLen = (NextKZD.Cml - KZD.Cml);
				x3 = KZD.Cml;
				y3 = NextKZD.DesHXX + SlopeLen * Slope;
				x4 = NextKZD.Cml;
				y4 = KZD.DesHXX + SlopeLen * Slope;

				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHXX, x1, y1, x2, y2, KZD.Cml, KZD.DesHXX))//有实交点，插入该控制点
					IsInsertStart = true;
				else
					IsInsertStart = false;
				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, KZD.Cml, KZD.DesHXX, x4, y4, x3, y3, NextKZD.Cml, NextKZD.DesHXX))
					IsInsertEnd = true;
				else
					IsInsertEnd = false;

				if (IsInsertStart && IsInsertEnd)
				{
					//GCKZDArry.InsertAt(i + 1, KZD);
					GCKZDArry.insert(GCKZDArry.begin() + i + 1, KZD);
					return;
				}

				if (!IsInsertStart)//考虑前一个控制点不能插入
				{
					if (KZD.DesHXX > PreKZD.DesHXX)//新加入的小，覆盖掉前面那个，否则保持不变
						GCKZDArry[i] = KZD;
				}

				if (!IsInsertEnd)//考虑后一个控制点不能插入
				{
					if (KZD.DesHXX > NextKZD.DesHXX)//新加入的小，覆盖掉前面那个，否则保持不变
						GCKZDArry[i + 1] = KZD;
				}
			}
		}
	}
}

/*===================================================================
函数名:CalSloptPtDesHRange
------------------------------------------------------------
函数功能:计算边坡点高程范围
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool CalSloptPtDesHRange(bool YorN,PlanePlan &oneplane,int SlopePtIndex,double& minH,double& maxH);
		移植并修改适应本项目数据结构
=====================================================================*/
bool ZDMSJ::CalSloptPtDesHRange_ZMSJ(int SlopePtIndex, double& minH, double& maxH)
{
	//在这里添加代码
	double FrontfrontMileage = 0.0;//前两个变坡点的里程
	double Frontfronthigh = 0.0;//前两个变坡点的高程
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	double FrontPD = 0.0;//前一个坡度值
	double tempPD = 0.0;
	double dmxnumber = 0.0;
	double aftermaxH = 0.0;
	double afterminH = 0.0;
	double endMileage = 0.0;
	double tempmaxH = 0.0;
	double tempminH = 0.0;
	FrontMileage = m_SlopeArray[SlopePtIndex - 1].cml;
	Fronthigh = m_SlopeArray[SlopePtIndex - 1].Level;
	NowMileage = m_SlopeArray[SlopePtIndex].cml;

	double temppd = 0.0;
	//temppd = oneplane.m_ProfilePlan.SlopePtArray[SlopePtIndex].LimitGrade;
	temppd = _wtof(m_PrfDesData.MAXPD) / 100.0;
	double MaxSlopeDifference = _wtof(m_PrfDesData.MAXPD) * 2 / 100.0;	//20190808	
	if (SlopePtIndex == 1)
	{
		minH = Fronthigh - (NowMileage - FrontMileage) * temppd;//最小高程
		maxH = Fronthigh + (NowMileage - FrontMileage) * temppd;//最大高程
	}
	else
	{

		FrontfrontMileage = m_SlopeArray[SlopePtIndex - 2].cml;
		Frontfronthigh = m_SlopeArray[SlopePtIndex - 2].Level;
		FrontPD = (Fronthigh - Frontfronthigh) / (FrontMileage - FrontfrontMileage);
		tempPD = min(temppd, (FrontPD + MaxSlopeDifference));//考虑了最大坡度代数差
		maxH = Fronthigh + (NowMileage - FrontMileage) * tempPD;//最大高程(可能有问题)
		//maxH = Fronthigh+(NowMileage-FrontMileage)*temppd;

		tempPD = max(-temppd, (FrontPD - MaxSlopeDifference));//考虑了最大坡度代数差
		minH = Fronthigh + (NowMileage - FrontMileage) * tempPD;//最小高程
		//minH = Fronthigh-(NowMileage-FrontMileage)*temppd;//最小高程
	}

	////////////////////////////考虑信封区域//////////////////////////////////////////
	double TempMinH;//临时最小高程
	double TempMaxH;//临时最大高程
	RoadDesignWorkingDatabase workingbase;
	JD_CENTER* pRoad = workingbase.CreateHorizontalRoad(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname);

	GetEnvelopRange_ZMSJ(Fronthigh, FrontMileage, NowMileage, TempMinH, TempMaxH);
	double MaxPD = _wtof(m_PrfDesData.MAXPD) / 100.0;	//20190808
	double StaMaxH = m_EndH + MaxPD * (pRoad->LenOfLine - NowMileage);
	double StaMinH = m_EndH - MaxPD * (pRoad->LenOfLine - NowMileage);
	TempMinH = max(TempMinH, StaMinH);
	TempMaxH = min(TempMaxH, StaMaxH);
	/*double maxH1 = maxH;
	double minH1 = minH;*/
	//ads_printf("$$$$$$$$$4里程%lf,信封下限%lf,信封上限%lf\n",NowMileage,TempMinH,TempMaxH);
	minH = max(minH, TempMinH);
	maxH = min(TempMaxH, maxH);
	if (maxH < minH - 0.01)//信封区域顶点处，无法同时考虑坡度代数差与信封区域，只考虑信封区域
	{
		//改为只考虑坡度代数差试一试
		maxH = TempMaxH;
		minH = TempMinH;
	}
	return true;
}
/*===================================================================
函数名:NormalRandom
------------------------------------------------------------
函数功能:传入期望、方差、取值范围，得到按正态分布的随机数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		double NormalRandom(double miu, double sigma, double min, double max);
		移植
=====================================================================*/
double ZDMSJ::NormalRandom_ZMSJ(double miu, double sigma, double min, double max)
{
	srand(1);
	double x;
	double dScope;
	double y;
	do
	{
		x = AverageRandom_ZMSJ(min, max);
		y = Normal_ZMSJ(x, miu, sigma);
		double temp = Normal_ZMSJ(miu, miu, sigma);
		dScope = AverageRandom_ZMSJ(0.0, Normal_ZMSJ(miu, miu, sigma));
	} while (dScope > y);
	return x;
}

/*===================================================================
函数名:AverageRandom
------------------------------------------------------------
函数功能:得到均匀分布随机数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		double AverageRandom(double min, double max);
		移植
=====================================================================*/
double ZDMSJ::AverageRandom_ZMSJ(double min, double max)
{
	long minInteger = (long)(min * 100);
	long maxInteger = (long)(max * 100);
	if (maxInteger == minInteger)
		return (min + max) / 2.0;

	long randInteger = rand() * rand();

	long diffInteger = maxInteger - minInteger;

	long resultInteger = randInteger % diffInteger + minInteger;

	return resultInteger / 100.0;
}

/*===================================================================
函数名:Normal
------------------------------------------------------------
函数功能:计算x的概率密度函数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		double Normal(double x, double miu, double sigma);
		移植
=====================================================================*/
double ZDMSJ::Normal_ZMSJ(double x, double miu, double sigma)
{
	return 1.0 / sqrt(2 * PI * sigma) * exp(-1 * (x - miu) * (x - miu) / (2 * sigma * sigma));
}

/*===================================================================
函数名:Env_GetMiu
------------------------------------------------------------
函数功能:根据地面线计算填挖方最小高程
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		void Env_GetMiu(PlanePlan &oneplane,int PDNum,double &Miu);
		移植
=====================================================================*/
void ZDMSJ::Env_GetMiu_ZMSJ(int PDNum, double& Miu)
{
	double FrontMileage;//前一个变坡点的里程
	double Fronthigh;//前一个变坡点的高程
	double NowMileage;//当前变坡点的里程
	FrontMileage = m_SlopeArray[PDNum - 1].cml;
	Fronthigh = m_SlopeArray[PDNum - 1].Level;
	NowMileage = m_SlopeArray[PDNum].cml;
	GetDMX1_ZMSJ(FrontMileage, NowMileage);
	double sum1 = 0.0;
	double sum2 = 0.0;
	for (int i = 0; i < m_CRS_PT.size(); i++)
	{
		sum1 = sum1 + (Fronthigh - m_CRS_PT[i].Earh) * (m_CRS_PT[i].Cml - FrontMileage);
		sum2 = sum2 + (m_CRS_PT[i].Cml - FrontMileage) * (m_CRS_PT[i].Cml - FrontMileage);
	}

	double K = 0.0;
	if (sum2 <= 1.0)
	{
		K = 0.0;
	}
	else
	{
		K = -sum1 / sum2;//斜率
	}
	Miu = K * (m_SlopeArray[PDNum].cml - FrontMileage) + Fronthigh;
}

/*===================================================================
函数名:GetDMX1
------------------------------------------------------------
函数功能:获取地面线
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		void GetDMX1(PlanePlan &oneplane,double StratM, double EndM);
		移植
=====================================================================*/
void ZDMSJ::GetDMX1_ZMSJ(double StratM, double EndM)
{
	/////////////////////////////////////提取有效地面线/////////////////////////////
	m_CRS_PT.clear();
	int EndIndex = m_EarthLine.size() - 1;
	CRSPT crs_pt;
	for (int Index = 0; Index <= EndIndex; Index++)
	{
		if (m_EarthLine[Index].lc > StratM - 0.01 && m_EarthLine[Index].lc < EndM + 0.01)
		{
			crs_pt.Style = 0;
			crs_pt.Cml = m_EarthLine[Index].lc;
			crs_pt.Earh = m_EarthLine[Index].level;
			m_CRS_PT.push_back(crs_pt);
		}
	}
}

/*===================================================================
函数名:RoundSlope
------------------------------------------------------------
函数功能:坡度取整
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		void RoundSlope(ProfilePlan& profile);
		移植
=====================================================================*/
void ZDMSJ::RoundSlope_ZMSJ(vector<PDTAB_ROAD>&SlopeArray)
{
	int SlopePtNum = SlopeArray.size();
	double Grade = 0.0;
	double SlopeLen = 0.0;
	double scale = pow(10.0, 3);
	for (int i = 0; i < SlopePtNum - 1; i++)
	{
		SlopeLen = SlopeArray[i + 1].cml - SlopeArray[i].cml;
		Grade = 100.0 * (SlopeArray[i + 1].Level - SlopeArray[i].Level) / SlopeLen;
		Grade = floor(Grade * scale + 0.5) / scale / 100.0;
		SlopeArray[i].degree = Grade;
		SlopeArray[i + 1].Level = SlopeArray[i].Level + Grade * SlopeLen;
	}
}

/*===================================================================
函数名:MergeSlope
------------------------------------------------------------
函数功能:合并坡度
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		void MergeSlope(PlanePlan& Profile);
		移植
=====================================================================*/
void ZDMSJ::MergeSlope_ZMSJ(vector<PDTAB_ROAD>&SlopeArray)
{
	double BefGrade = 0.0;
	double AftGrade = 0.0;
	for (int i = 1; i < SlopeArray.size() - 2; i++)
	{
		AftGrade = (SlopeArray[i + 1].Level - SlopeArray[i].Level)
			/ (SlopeArray[i + 1].cml - SlopeArray[i].cml);
		BefGrade = (SlopeArray[i].Level - SlopeArray[i - 1].Level)
			/ (SlopeArray[i].cml - SlopeArray[i - 1].cml);
		if (fabs(AftGrade - BefGrade) < 0.02)		//暂时按0.02（2%）坡度处理
		{
			if (SlopePtCanDel_ZMSJ(i, i))
			{
				SlopeArray.erase(SlopeArray.begin() + i);
				i--;
			}
		}
	}
}

/*===================================================================
函数名:SlopePtCanDel
------------------------------------------------------------
函数功能:判断Profile方案的SlopetIndex变坡点能否被删除
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool SlopePtCanDel(PlanePlan& Profile, int StartIndex, int EndIndex);
		移植
=====================================================================*/
bool ZDMSJ::SlopePtCanDel_ZMSJ(int StartIndex, int EndIndex)
{
	int SlopePtSum = m_SlopeArray.size();
	if (StartIndex < 1 || EndIndex > SlopePtSum - 2)
	{
		return false;
	}

	double MAXSlope_SX, MAXSlope_XX;
	double Slope, BefSlope, AftSlope;

	MAXSlope_SX = _wtof(m_PrfDesData.MAXPD) / 100.0;
	MAXSlope_XX = _wtof(m_PrfDesData.MAXPD) / 100.0;

	Slope = (m_SlopeArray[EndIndex + 1].Level - m_SlopeArray[StartIndex - 1].Level) /
		(m_SlopeArray[EndIndex + 1].cml - m_SlopeArray[StartIndex - 1].cml);
	if (Slope > 0.0)
	{
		if (Slope > MAXSlope_SX + 0.0000001)
			return false;
	}
	else
	{
		if (-Slope > MAXSlope_XX + 0.0000001)
			return false;
	}

	if (StartIndex > 1)
	{
		BefSlope = (m_SlopeArray[StartIndex - 1].Level - m_SlopeArray[StartIndex - 2].Level) /
			(m_SlopeArray[StartIndex - 1].cml - m_SlopeArray[StartIndex - 2].cml);

		if (fabs(Slope - BefSlope) > _wtof(m_PrfDesData.MAXPD) / 100.0 * 2.0 + 0.0000001)	//最大坡度代数差
			return false;
	}

	if (EndIndex < SlopePtSum - 2)
	{
		AftSlope = (m_SlopeArray[EndIndex + 2].Level - m_SlopeArray[EndIndex + 1].Level) /
			(m_SlopeArray[EndIndex + 2].cml - m_SlopeArray[EndIndex + 1].cml);

		if (fabs(AftSlope - Slope) > _wtof(m_PrfDesData.MAXPD) / 100.0 * 2.0 + 0.0000001)	//最大坡度代数差
			return false;
	}

	return true;
}

/*===================================================================
函数名:FormFinalSlope
------------------------------------------------------------
函数功能:形成初始纵断面方案
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool FormFinalSlope(PlanePlan& plan);
		移植
=====================================================================*/
void ZDMSJ::FormInitSlope_ZMSJ()
{
	MergeSlope_ZMSJ(m_SlopeArray);
	RoundSlope_ZMSJ(m_SlopeArray);
}

/*===================================================================
函数名:FormFinalSlope
------------------------------------------------------------
函数功能:形成最终纵断面方案
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool FormFinalSlope(PlanePlan& plan);
		移植
=====================================================================*/
void ZDMSJ::FormFinalSlope_ZMSJ()
{
	MergeSlope_ZMSJ(m_BPDArray);
	RoundSlope_ZMSJ(m_BPDArray);
}

//获取初始方案起终里程高程
void ZDMSJ::GetStAndEdLC(double &StartLC, double &EndLC, double &StLevel, double &EdLevel)
{
	if (m_JPInforArray.size() > 0)
	{
		if (m_JPInforArray[0].Is_JP_Bef == 1 && m_JPInforArray[0].Is_JP_Aft == 1) //前后约束
		{
			//BPDnum = 4
			int PDnum;
			PDnum = (int)m_BPDArray.size();
			if (PDnum == 4)
			{
				StartLC = m_BPDArray[1].ml; StLevel = m_BPDArray[1].Level;
				EndLC = m_BPDArray[2].ml; EdLevel = m_BPDArray[2].Level;
			}
			else
			{
				//由于坡长限制，求交变坡点时考虑
				StartLC = m_BPDArray[0].ml; StLevel = m_BPDArray[0].Level;
				EndLC = m_BPDArray[PDnum -1].ml; EdLevel = m_BPDArray[PDnum - 1].Level;
			}
		}
		else if (m_JPInforArray[0].Is_JP_Bef == 1 && m_JPInforArray[0].Is_JP_Aft == 0) //前约束后不约束
		{
			//BPDnum = 3
			StartLC = m_BPDArray[1].ml;  StLevel = m_BPDArray[1].Level;
			EndLC = m_BPDArray[2].ml;  EdLevel = m_BPDArray[2].Level;
		}
		else if (m_JPInforArray[0].Is_JP_Bef == 0 && m_JPInforArray[0].Is_JP_Aft == 1) //前不约束后约束
		{
			//BPDnum = 3
			StartLC = m_BPDArray[0].ml; StLevel = m_BPDArray[0].Level;
			EndLC = m_BPDArray[1].ml; EdLevel = m_BPDArray[1].Level;
		}
		else
		{
			//BPDnum = 2
			StartLC = pzLinep->DLArray[0].ELC;  StLevel = m_BPDArray[0].Level;
			EndLC = pzLinep->XLLength;  EdLevel = m_BPDArray[1].Level;
		}
	}
	else
	{
		StartLC = pzLinep->DLArray[0].ELC;  StLevel = m_BPDArray[0].Level;
		EndLC = pzLinep->XLLength;  EdLevel = m_BPDArray[1].Level;
	}
}

/*===================================================================
函数名:GetBPDStAndEdfromJP
------------------------------------------------------------
函数功能:根据接坡信息形成原始变坡点数组
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	[i][0]里程 [i][1]高程 [i][2]竖曲线 [i][3]坡度
=====================================================================*/
void ZDMSJ::GetBPDStAndEdfromJP(int&BPDnum, DMX_ROAD *pdmx)
{
	double PD_array[MAXBPDNUM][4];
	bool Is_Constr_Start = true; 	//起点是否需要约束
	bool Is_Constr_End = true; 	//终点是否需要约束
	double JXPCMin = _wtof(m_PrfDesData.JXPCMin);
	double YBPCMin = _wtof(m_PrfDesData.YBPCMin);//一般坡长作为判断
	double  ml = 0.0,level=0.0;


	for (int i = 0; i < MAXBPDNUM; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			PD_array[i][j] = 0.0;
		}
	}


	//存在接坡信息赋里程初始值
	if (m_JPInforArray.size() > 0)
	{
		m_JPInforArray[0].Stcml = pzLinep->DLArray[0].ELC;
		m_JPInforArray[0].Edcml = pzLinep->XLLength;
	}

	if (m_JPInforArray.size() > 0)
	{
		//关于前后约束情况,满足最小坡长要求
		if (m_JPInforArray[0].Bef_PC > YBPCMin)
			Is_Constr_Start = false;
		if (m_JPInforArray[0].Aft_PC > YBPCMin)
			Is_Constr_End = false;


		if (Is_Constr_Start && Is_Constr_End) //前后都需约束
		{
			//当道路总长小于两倍最小坡长，且含有前后端接坡约束，这时直接求交上变坡点
			if (YBPCMin < pzLinep->XLLength && pzLinep->XLLength < 2.5 * YBPCMin)
			{
				//起始坡段【0】
				PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

				//如果小范围内求交有交点则上交点
				if (AutoCalPVI(ml, level))
				{
					Is_NeedInitPro = false;
					PD_array[1][0] = ml;
					PD_array[1][1] = level;
					//终止坡段【2】
					PD_array[2][0] = m_JPInforArray[0].Edcml;
					PD_array[2][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 3;
				}
				else //无交点则赋起终点信息
				{
					PD_array[1][0] = m_JPInforArray[0].Edcml;
					PD_array[1][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 2;
				}

				//AutoCalPVI(PD_array[1][0], PD_array[1][1]);
			}
			else
			{
				//起始坡段【0】
				PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

				//【1】
				if (m_JPInforArray[0].Bef_PC < YBPCMin)
					PD_array[1][0] = PD_array[0][0] + YBPCMin - m_JPInforArray[0].Bef_PC;
				PD_array[1][1] = (PD_array[1][0] - PD_array[0][0])*m_JPInforArray[0].Bef_PD*0.01 + PD_array[0][1]; //高程

				//终止坡段【2】
				PD_array[3][0] = m_JPInforArray[0].Edcml;
				PD_array[3][1] = m_JPInforArray[0].Aft_H;

				//【3】
				if (m_JPInforArray[0].Aft_PC < YBPCMin)
					PD_array[2][0] = PD_array[3][0] - (YBPCMin - m_JPInforArray[0].Aft_PC);

				PD_array[2][1] = PD_array[3][1] - (PD_array[3][0] - PD_array[2][0])*m_JPInforArray[0].Aft_PD*0.01;
				BPDnum = 4;
			}
		
			m_BPDArray.resize(BPDnum);
			for (int i = 0; i < BPDnum; i++)
			{
				m_BPDArray[i].ml = PD_array[i][0];
				m_BPDArray[i].Level = PD_array[i][1];
			}
		}
		else if (Is_Constr_Start && Is_Constr_End == false) //前约束后不约束
		{
			//起始坡段【0】
			PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
			PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

			//【1】
			if (m_JPInforArray[0].Bef_PC < YBPCMin)
				PD_array[1][0] = PD_array[0][0] + YBPCMin - m_JPInforArray[0].Bef_PC;

			PD_array[1][1] = (PD_array[1][0] - PD_array[0][0])*m_JPInforArray[0].Bef_PD*0.01 + PD_array[0][1]; //高程

			//终止坡段【2】
			PD_array[2][0] = m_JPInforArray[0].Edcml;
			PD_array[2][1] = m_JPInforArray[0].Aft_H;

			BPDnum = 3;

			m_BPDArray.resize(BPDnum);
			for (int i = 0; i < BPDnum; i++)
			{
				m_BPDArray[i].ml = PD_array[i][0];
				m_BPDArray[i].Level = PD_array[i][1];
			}
		}
		else if (Is_Constr_Start == false && Is_Constr_End) //前不约束后约束
		{
			//起始坡段【0】
			PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
			PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

			//终止坡段【2】
			PD_array[2][0] = m_JPInforArray[0].Edcml;
			PD_array[2][1] = m_JPInforArray[0].Aft_H;

			//【1】
			if (m_JPInforArray[0].Aft_PC < YBPCMin)
				PD_array[1][0] = PD_array[2][0] - (YBPCMin - m_JPInforArray[0].Aft_PC);

			PD_array[1][1] = PD_array[2][1] - (PD_array[2][0] - PD_array[1][0])*m_JPInforArray[0].Aft_PD*0.01;

			BPDnum = 3;

			m_BPDArray.resize(BPDnum);
			for (int i = 0; i < BPDnum; i++)
			{
				m_BPDArray[i].ml = PD_array[i][0];
				m_BPDArray[i].Level = PD_array[i][1];
			}
		}
		else //前后都不约束
		{
			if (YBPCMin < pzLinep->XLLength && pzLinep->XLLength < 2.5 * YBPCMin)
			{
				//起始坡段【0】
				PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

				/*AutoCalPVI(PD_array[1][0], PD_array[1][1]);*/

				//如果小范围内求交有交点则上交点
				if (AutoCalPVI(ml, level))
				{
					Is_NeedInitPro = false;
					PD_array[1][0] = ml;
					PD_array[1][1] = level;
					//终止坡段【2】
					PD_array[2][0] = m_JPInforArray[0].Edcml;
					PD_array[2][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 3;
				}
				else //无交点则赋起终点信息
				{
					PD_array[1][0] = m_JPInforArray[0].Edcml;
					PD_array[1][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 2;
				}
			}
			else
			{
				//起始坡段【0】
				PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

				//终止坡段【1】
				PD_array[1][0] = m_JPInforArray[0].Edcml;
				PD_array[1][1] = m_JPInforArray[0].Aft_H;
				BPDnum = 2;
			}

			m_BPDArray.resize(BPDnum);
			for (int i = 0; i < BPDnum; i++)
			{
				m_BPDArray[i].ml = PD_array[i][0];
				m_BPDArray[i].Level = PD_array[i][1];
			}
			
		}
	}
	else
	{
		//起始坡段【0】
		PD_array[0][0] = pzLinep->DLArray[0].ELC;//里程 
		PD_array[0][1] = pdmx->DMXys[0].level; //高程

		//终止坡段【1】
		PD_array[1][0] = pzLinep->XLLength;
		PD_array[1][1] = pdmx->DMXys[pdmx->NUM - 1].level;
		BPDnum = 2;

		m_BPDArray.resize(BPDnum);
		for (int i = 0; i < BPDnum; i++)
		{
			m_BPDArray[i].ml = PD_array[i][0];
			m_BPDArray[i].Level = PD_array[i][1];
		}
	}

	if (m_JPInforArray.size() > 0)
	{
		//前后端约束赋值
		if (Is_Constr_Start)
			m_JPInforArray[0].Is_JP_Bef = 1;
		if (Is_Constr_End)
			m_JPInforArray[0].Is_JP_Aft = 1;
	}

}

//求交自动上个坡度,有交点返回true，无交点返回false
bool ZDMSJ::AutoCalPVI(double &ml,double &H)
{
	ads_point from1, to1;//起点坡段线段两端点
	ads_point from2, to2;//终点坡段线段两端点
	ads_point result;	 //起终坡段交点

	from1[Z] = 0.0; to1[Z] = 0.0; from2[Z] = 0.0; to2[Z] = 0.0;

	from1[X] = m_JPInforArray[0].Stcml - 20.0;
	from1[Y] = m_JPInforArray[0].Bef_H - 20.0*m_JPInforArray[0].Bef_PD*0.01;

	to1[X] = m_JPInforArray[0].Stcml;
	to1[Y] = m_JPInforArray[0].Bef_H;

	from2[X] = m_JPInforArray[0].Edcml;
	from2[Y] = m_JPInforArray[0].Aft_H;

	to2[X] = m_JPInforArray[0].Edcml + 20.0;
	to2[Y] = m_JPInforArray[0].Aft_H + 20.0*m_JPInforArray[0].Aft_PD*0.01;

	//int iTest = ads_inters(from1, to1, from2, to2, 0, result);

	//ml = result[X];
	//H = result[Y];

	int iTest = BAS_DRAW_FUN::INTERS(&ml, &H, from1[X], from1[Y], to1[X], to1[Y], from2[X], from2[Y], to2[X], to2[Y]);

	//如果求交没有结果两线平行
	if (iTest >= 0)
		return true;
	else
		return false;

}