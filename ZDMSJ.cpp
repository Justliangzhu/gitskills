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
AcDbObjectId ZpId;//��¼��ǰ����
//RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//���Ƶ�·��ƻ���ݿ�

ZDMSJ::ZDMSJ()
{
}

ZDMSJ::~ZDMSJ()
{

}



//	��ע���µ�
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
	HH=BPD_array[0][1]-Y0+H0;    // ��Ƹ߳�

	Pt1.x=BPD_array[0][0];
	Pt1.y=BPD_array[0][1];
	Pt2.x=Pt1.x;
	Pt2.y=Pt1.y+WB;
	//G_makeline(pWd,Pt1,Pt2,3);
	makeline( Pt1,Pt2,3);
	B=pi*0.5;
	// ��� �߳�
	ads_rtos(HH,2,NLC,chrh);
	Pt2.x=Pt1.x-texth*0.5;
	Pt2.y=Pt1.y+3*texth;
	// G_maketext(pWd,Pt2,chrh,B ,texth ,4,2);     
	maketext( Pt2,chrh,B ,texth ,4,1);   

	// ��� ���
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
		HH=BPD_array[i][1]-Y0 +H0;    // ��Ƹ߳�
		yy=BPD_array[i][1]-BPD_array[i-1][1]; // �߲�

		Pt1.x=BPD_array[i][0];
		Pt1.y=BPD_array[i][1];
		Pt2.x=Pt1.x;
		Pt2.y=Pt1.y+WB;
		// G_makeline(pWd,Pt1,Pt2,3);
		makeline( Pt1,Pt2,3);
		B=pi*0.5;
		// ��� �߳�
		ads_rtos(HH,2,NLC,chrh);
		Pt2.x=Pt1.x-texth*0.5;
		Pt2.y=Pt1.y+3*texth;
		//G_maketext(pWd,Pt2,chrh,B ,texth ,4,2);     
		maketext( Pt2,chrh,B ,texth ,4,1); 
		// ��� ���
		_tcscpy(chrlc,LCchr(GH,LC,NLC,2));
		Pt2.x=Pt1.x+texth*1.5;
		Pt2.y=Pt1.y+3*texth;
		// G_maketext(pWd,Pt2,chrlc,B ,texth ,4,2);     
		maketext( Pt2,chrlc,B ,texth ,4,1); 
		//����ࣩ�¶� �³�
		double fb,de ;
		de=xyddaa(BPD_array[i-1][1],BPD_array[i-1][0],BPD_array[i][1],BPD_array[i][0],&fb);
		fb= 0.5*pi-fb;
		// �¶�  
		if (fabs(II-(int)(II))<0.001)  {  ads_rtos(II,2,0,chrh);}
		else  if (fabs(II*10.0-(int)(II*10.0))<0.001)  {  ads_rtos(II,2,1,chrh);}
		else  if (fabs(II*100.0-(int)(II*100.0))<0.001)  {  ads_rtos(II,2,2,chrh);}
		else   ads_rtos( II,2,3,chrh);
		_tcscat(chrh,L"(o/oo)"); 	    
		Pt2.x=Pt1.x- LL*0.5/HBVSCALE-texth;
		Pt2.y=Pt1.y+0.5*texth-yy*0.5;
		// G_maketext(pWd,Pt2,chrh,fb,texth ,4,2);     
		maketext(Pt2,chrh,fb,texth ,4,1); 
		//  �³�
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

	if ( (ads_getfiled( L"����������¶��ļ��� *.PD ",NULL,L"PD",2+4,&result))!=RTNORM) return ;
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
	pzLinep = pzLine;//һ�廯��Ƶ�ȫ�ֱ���

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

	//ads_prompt(L"\n\n ���� 1-��̱��.�����2λС���߳���3λ 2-��2λС�� 3-3λС��");
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
	// ads_getstring(0,L"\n ���� ��̹ں� Enter GH:",GH);
	// ads_initget(1+2,NULL);
	// ads_getstring(0,L"\n ���� ����ں� Enter GH:",JDGH);

	AcGePoint3d pkPt;
	acedInitGet(NULL, NULL);
	if(acedGetPoint(NULL,L"������Ļ��ָ����ͼԭ��,L",asDblArray(pkPt))==RTCAN)
	return;
	pORI_SCALE.X_ORI = pkPt.x+30;
	pORI_SCALE.Y_ORI = pkPt.y+pORI_SCALE.PQXHIGH+pORI_SCALE.LCBHigh+20;

	_tcscpy(GH,L"AK"); 
	GTZDM  *pzdmLine= new GTZDM(trow,PD_array,&GH[0]);
	pzdmLine->setINIT(NLC,NH,1,Texth,10);

	AcDbObjectId  LIdzdm;
	LIdzdm=AddEntityToDbs(pzdmLine);

	ZpId=LIdzdm;//��ǰ����*///szf

}
//�¶�����
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

	rc = ads_entsel(L"\n��ѡ���һ���¶���: ", en, asDblArray(PT));
	if (rc != RTNORM) {  ads_printf(L"\nError during object selection"); return;   }
	pzLinep=GlobalZDMTK.GetPM(PT);
	acdbGetObjectId(eId1, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId1, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId1, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc()))
	{             
		pObj->close();
		/*acdbOpenObject(pGtzdm1,eId1, AcDb::kForRead);*/
		if(acdbOpenObject(pGtzdm1,eId1, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
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

	rc = ads_entsel(L"\n��ѡ��ڶ����¶���: ", en, asDblArray(PT));
	if (rc != RTNORM) {  ads_printf(L"\nError during object selection"); return;   }

	acdbGetObjectId(eId2, en);
	/* acdbOpenObject(pObj, eId2, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId2, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc()))
	{             
		pObj->close();
		/*acdbOpenObject(pGtzdm2,eId2, AcDb::kForRead);	*/
		if(acdbOpenObject(pGtzdm2,eId2, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
	int trow=trow1;//�ظ�������
	int k;
	if(eId2==eId1)
	{
		ads_alert(L"�����¶�ʵ��Ϊͬһʵ��,�޷�����!");
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
	//����
	queue(PD_array1 , trow, 0);
	//Ϊ��֤���Ӻ�ʵ���¶Ȳ��䣬������Ӧ�¶�
	if(trow1>0)
	{
		int inf=1;
		int Xsw=(int)(pORI_SCALE.XSW+0.001); 
		double dh,LL,unit,ii;

		dh = fabs(PD_array1[trow1][1]-PD_array1[trow1-1][1]);
		LL = fabs(PD_array1[trow1][0]-PD_array1[trow1-1][0])*pORI_SCALE.HBVSCALE;
		unit = 100.0;

		//����trow1-1 -----trow1���¶�
		if(PD_array1[trow1][1]-PD_array1[trow1-1][1]<0)
			inf = -1;	
		if(Xsw==1)//С��λȡһλ
			ii=inf*((int)(dh*unit*10/LL+0.5))/10.0;//�¶�
		else if(Xsw==2)//С��λȡ��λ
			ii=inf*((int)(dh*unit*100/LL+0.5))/100.0;//�¶�
		else if(Xsw==3)//С��λȡ��λ
			ii=inf*((int)(dh*unit*1000/LL+0.5))/1000.0;//�¶�
		else//С��λȡ��λ
			ii=inf*(int)(dh*unit/LL+0.5);//�¶�*/

		PD_array1[trow1][3]=ii;

		//����trow1 -----trow1+trow2�ı��
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

	ZpId=LId;//��ǰ����
	return  ;      
} 

void  ZDMSJ::changeDL()
{	 
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;

	rc = ads_entsel(L"\nSelect a  ��· : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); return;   }

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{   pObj->close();

	GTZDM_ROAD  *pzLine;
	int iDL;
	double BLC,DL;
	ACHAR GH[6];
	// ȡ��Ҫ�޸ĵ�ĳ������
	ads_prompt(L"\n\n ���� �������");
	ads_initget(1+2+4,NULL);
	ads_getint(L"\n  Please intput IDL=",&iDL);
	ads_prompt(L"\n\n ���� �ö������BLC,DL,GH");
	ads_initget(1+2+4,NULL);
	ads_getreal(L"\n  Please intput BLC=",&BLC);

	ads_getreal(L"\n  Please intput DL=",&DL);
	ads_initget(1+2+4,NULL);
	ads_getstring(0,L"\n  Please intput GH=",GH);

	/* acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
	if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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

	rc = ads_entsel(L"\nSelect a  ��·�ݶ�������� : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }
	pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*  acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{    pObj->close();

	GTZDM_ROAD  *pzLine;
	int iDL;
	double BLC,DL;
	ACHAR GH[6];
	// ȡ��Ҫ�ӵ�ĳ������
	ads_prompt(L"\n\n ���� �������");
	ads_initget(1+2+4,NULL);
	ads_getint(L"\n  Please intput IDL",&iDL);
	ads_prompt(L"\n\n ���� �ö������BLC,DL,GH");
	ads_initget(1+2+4,NULL);
	ads_getreal(L"\n  Please intput BLC=",&BLC);

	ads_getreal(L"\n  Please intput DL=",&DL);
	ads_initget(1+2+4,NULL);
	ads_getstring(0,L"\n  Please intput GH=",GH);

	/*  acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
	if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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

	rc = ads_entsel(L"\nSelect a  ��· : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); return;   }

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDM_ROAD::desc())) 
	{   pObj->close();

	GTZDM_ROAD  *pzLine;
	int iDL;

	// ȡ��Ҫɾ����ĳ������
	ads_prompt(L"\n\n ���� �������");
	ads_initget(1+2+4,NULL);
	ads_getint(L"\n  Please intput IDL=",&iDL);      

	/* acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
	if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
//���ӱ��µ�
void  ZDMSJ::add_BPD()
{	 
	//���ò���
	int rc,NBPD,   BPDi;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	int track,type ;
	ads_point cpt ,pt;
	double X0,Y0,K0,H0,HBVSCALE;
	struct resbuf *result ;

	//��ʾ�û�ѡ���ݶ��������ʵ��
	rc = ads_entsel(L"\nѡ���ݶ��������: ", en, asDblArray(PT));
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
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
			return;
		}

		//20191217	
		int izdm = pzLine->m_Frame_Index;
		if (izdm < 0 || izdm >= GlobalZDMTK.NZDM)
		{
			ads_printf(L"ͼ��ż�¼��������������CAD�ٳ��Ը�����\n");
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

		//	dd/dtemp;  //ѡȡ�㵽����㣩ĳ����ľ���(ƽ����
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
					ads_printf(L"��̣�%s  �̣߳�%lf\n",ckml,HH);
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
			_stprintf(mes,L"\n ���� ��%d���µ�������߰뾶R=<%0.0lf��>",BPDi,Rshu);
			rc = ads_getreal(mes,&R);
			if(rc==RTCAN)
				return;
			if(rc==RTNONE)
				R = Rshu;

			ads_grdraw(spt ,pt,-1,0);
			ads_grdraw(ept ,pt,-1,0);
			// ȡ��Ҫ�ӵ�ĳ������	
			/* acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
			if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
			{
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
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

	rc = ads_entsel(L"\nSelect a   �ݶ�������Ҫɾ���ı��µ� : ", en, asDblArray(PT));

	if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }

	pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/* acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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

		//	dd/dtemp;  //ѡȡ�㵽����㣩ĳ����ľ���(ƽ����
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
		ads_printf(L"\n\n  Ҫɾ���� BPDi=%d R=%10.2f ",BPDi,R);       
		/* acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
	rc = ads_entsel(L"\nSelect a  �ݶ�������Ҫ�޸ĵı��µ� : ", en, asDblArray(PT));
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection"); return;   }

	pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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

	//	dd/dtemp;  //ѡȡ�㵽����㣩ĳ����ľ���(ƽ����
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
		ads_printf(L"\n\n  Ҫ�޸ĵ� BPDi=%d R=%6.0f LC=%10.2f  HH=%8.2f",BPDi,R,LC,HH);       


		ads_getreal(L"\n  Please intput R=",&R);

		ads_getreal(L"\n  Please intput ���LC=",&LC);

		ads_getreal(L"\n  Please intput ��Ƹ߳�HH=",&HH);

		XX1=(LC-K0)/HBVSCALE+X0;
		YY1=HH-H0+Y0;

		/*acdbOpenObject(pzLine, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pzLine,eId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
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

	rc = ads_entsel(L"\nSelect a  ѡ��һ���ݶ����¶���: ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{    
		ads_printf(L"\n�ݶ����¶���ѡ�����,δ����!"); return;   
	}
	pzLinep=GlobalZDMTK.GetPM(PT);
	AcDbObjectId eId;
	acdbGetObjectId(eId, en);
	
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
		fnamedlg.m_ofn.lpstrFilter=L"�����ļ�(*.PD)\0*.PD\0\0";
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
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
//ֱ��ת�¶���
void ZDMSJ::createGTZDM_INPUT()//ֱ��ת�¶���
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
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
				return;
			}
			AcDbObjectIterator *pVertIter= pPline->vertexIterator();

			pPline->close(); 

			for ( i= 0; !pVertIter->done(); i++, pVertIter->step())
			{  //Ѱ�� ��ĳ����
				trow=i;
				vertexObjId = pVertIter->objectId();
				/*acdbOpenObject(pVertex, vertexObjId, AcDb::kForRead);*/
				if(acdbOpenObject(pVertex,vertexObjId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"��ʵ��ʧ�ܣ�\n");
					return;
				}
				location = pVertex->position();
				PD_array[trow][0]=location[X];
				PD_array[trow][1]=location[Y]; 
				//PD_array[trow][2]=10000;		//20190709	��ʱ�ĳ�700���Ժ���ݹ淶����Ĭ��ֵ
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
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
				return;
			}
			numverts=pPline->numVerts();

			for ( i= 0; i<numverts; i++)
			{  
				//Ѱ�ҵ�ĳ����
				trow=i;
				pPline-> getPointAt( i, location);
				PD_array[trow][0]=location[X]; 
				PD_array[trow][1]=location[Y]; 
				//PD_array[trow][2]=10000;		//20190709	��ʱ�ĳ�700���Ժ���ݹ淶����Ĭ��ֵ
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
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
		ads_printf(L"\n �������x=%10.3f ��Ӧ�����",PD_array[0][0]);
		ads_initget(1+2+4,NULL);
		ads_getreal(L"\n  Please select ����������LCB:",&LCB);
		ads_printf(L"\n �������y=%10.3f ��Ӧ�ĸ߳�",PD_array[0][1]);
		ads_initget(1+2+4,NULL);
		ads_getreal(L"\n  Please select ���������h:",&HH);
		*/
		LCB = (PD_array[0][0] - pORI_SCALE.X_ORI) * pORI_SCALE.HSCALE;	//20190709	�޸Ĳ���˫�������
		HH=(PD_array[0][1]-pORI_SCALE.Y_ORI) * pORI_SCALE.VSCALE + pORI_SCALE.ZMIN;	//20190709	�޸Ĳ���˫�������
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

		ZpId= LId;//��ǰ����
		GlobalZDMTK.ZDMTK[izdm].zdm = pzLine;
	}
	else 
	{
		pObj->close();
		ads_printf(L"\nSelected entity is not a line/polyline");
	}
	return  ;      
}

//�������
void ZDMSJ::CZmLPSJ()
{
	//���ò���
	int type,track=1,nbpd;
	struct resbuf entdata;// ����϶�ʱ�Ķ�̬����
	double PD_array[MAXBPDNUM][4];
	ACHAR  GH[8];
	int rc;
	AcGePoint3d PT1;
	ads_name en;
	AcDbObjectId eId;
	GTZDM_ROAD  *pzLine;
	AcDbObject *pObj;

	//��ʾ�û�ѡ������ʵ��
	rc = ads_entsel(L"\nѡ��һ�������¶��߻�����ͼ��ʵ�� : ", en, asDblArray(PT1));
	if(rc==RTCAN)
		return;
	int izdm;
	if (rc == RTNORM) 
	{    
		acdbGetObjectId(eId, en);    
		if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
			return;
		}
		pObj->close();

		if (pObj->isKindOf(GTZDM_ROAD::desc())) //��GTZDMʵ��,ȡ����
		{		
			if(acdbOpenObject(pzLine,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
				ads_printf(L"ͼ��Ŵ���\n");
				EarthLine->close();
				return;
			}
			if (GlobalZDMTK.ZDMTK[izdm].zdm)
			{
				ZpId = GlobalZDMTK.ZDMTK[izdm].zdm->id();
				nbpd = GlobalZDMTK.ZDMTK[izdm].zdm->NBPD;

				if (acdbOpenObject(pObj, ZpId, AcDb::kForRead) != Acad::eOk)
				{
					ads_printf(L"��ʵ��ʧ�ܣ�\n");
					EarthLine->close();
					return;
				}
				pObj->close();
			}

			EarthLine->close();
		}
	}
	else if (rc == RTCAN)	//����û�ȡ��������ʾ�κ���ʾ
	{
		return;
	}
	else		//�����������ʾ�û�
	{
		ads_printf(L"δѡ���������ʵ�壡\n");
		return;
	}
	//else if(fabs(PT1.x)<0.001 &&fabs(PT1.y)<0.001)	//20191217	���ٲ��òɵ��ȡͼ�򣬴��ڻ�ȡ����ͼ�������
	//{
	//	ads_grread (1, &type, &entdata); 
	//	PT1.x=entdata.resval.rpoint[0];
	//	PT1.y=entdata.resval.rpoint[1];
	//}
	//izdm = GlobalZDMTK.FindWichZdm(PT1);

	if (izdm < 0 || izdm >= GlobalZDMTK.NZDM)
	{
		ads_printf(L"ͼ��Ŵ���\n");
		return;
	}

	//pzLinep = GlobalZDMTK.GetPM(PT1);
	pzLinep = GlobalZDMTK.ZDMTK[izdm].pm;
	if(!pzLinep)
	{
		ads_printf(L"ƽ�淽��Ϊ��\n");
		return;
	}

	pORI_SCALE.STARTLC = pzLinep->DLArray[0].ELC;
	pORI_SCALE.ENDLC = pzLinep->XLLength;
	pORI_SCALE.X_ORI = GlobalZDMTK.ZDMTK[izdm].X0;
	pORI_SCALE.Y_ORI = GlobalZDMTK.ZDMTK[izdm].Y0;
	pORI_SCALE.ZMIN = GlobalZDMTK.ZDMTK[izdm].H0;

	if(!pObj->isKindOf(GTZDM_ROAD::desc()))
	{
		ads_printf(L"\n��ѡ���ݶ���ʵ��,ϵͳ��Ϊ���½��ݶ���ʵ��...\n");

		//������ʼ���
		ACHAR mes[80];
		double dml = -1000;
		_stprintf(mes,L"\n  ��������ʼ���µ���� = <��·���:%0.3lf> :  ",pzLinep->DLArray[0].ELC);
		ads_initget(2 + 4, NULL);
		ads_getreal(mes, &dml);
		if(dml < -900)
			dml = pzLinep->DLArray[0].ELC;
		double cml = pzLinep->TYLC(dml);

		//������ʼ�߳�
		double starth = 0.0;
		double sh = GlobalZDMTK.ZDMTK[izdm].dmx->DMXys[0].level;
		_stprintf(mes,L"\n  ��������ʼ���µ���Ʊ�� = <%0.3lf> :  ",sh);
		if (ads_getreal(mes, &starth) != RTNORM)
			starth = sh;

		//����ͼ�������
		PD_array[0][0] = (cml - pzLinep->DLArray[0].ELC) / pORI_SCALE.HSCALE + GlobalZDMTK.ZDMTK[izdm].X0;	//20190709	�޸Ĳ���˫�������
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
	{  //����϶�ѭ��
	if(ads_grread (track, &type, &entdata)!=RTNORM)
	break;
	acdbOpenObject(pzLine,ZpId, AcDb::kForWrite);
	pzLine->assertWriteEnabled();

	pzLine->BPD_array[nbpd-1][0]=entdata.resval.rpoint[0];
	pzLine->BPD_array[nbpd-1][1]=entdata.resval.rpoint[1];

	pzLine->NBPD = nbpd;

	pzLine->close();
	if (type==3)  //  ��������������һ��
	{  
	nbpd++;	   
	ads_initget(RSG_OTHER,L"C c E e");
	if(ads_getkword(L"\n���� (C) / ���� (E): <C>  ", choice)==RTNONE)
	_tcscpy(choice ,L"C");	 
	if(_tcscmp(choice ,L"E")==0 || _tcscmp(choice ,L"e")==0)//����
	track=0;
	}
	}*/


	CString mes;
	ads_printf(L"ע�⣺���\"i\"������ȷ���¶�ֵ�����������ʰȡ��һ���µ㣬����Ҽ�����!\n");
	while (track>0) 
	{  
		if(ads_grread (track, &type, &entdata)!=RTNORM)
		{
			if(acdbOpenObject(pzLine,ZpId,AcDb::kForWrite)!=Acad::eOk)
			{
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
				return;
			}
			pzLine->assertWriteEnabled();
			pzLine->NBPD--;
			pzLine->close();
			break;
		}
		if(acdbOpenObject(pzLine,ZpId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
		if (type==3)  //��������������һ��
		{      			

			nbpd++;
			track=1;

			ads_printf(L"ע�⣺���\"i\"������ȷ���¶�ֵ�����������ʰȡ��һ���µ㣬����Ҽ�����!\n");
		}
		else if(type==2)//i��
		{
			nbpd = pzLine->NBPD;
			double SPD,LL;

			mes.Format(L"\n�������¶�ֵ<%0.2lf>",pzLine->BPD_array[nbpd-1][3]);		
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
	//	ads_printf(L"�����߲ɼ�����!\n");
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

//һ������
void ZDMSJ::AutoSlopeData()
{
	int rc;
	AcGePoint3d PT1;
	ads_name en;
	AcDbObjectId eId;
	AcDbObject *pObj;
	BAS_DRAW_FUN BasFun;
	RoadDesignWorkingDatabase WorkingData;
	int  NBPD; 		//�¶�����  
	int deshxsw, pdxsw;
	//[i][0]��� [i][1]�߳� [i][2]������ [i][3]�¶�
	double  BPD_Array[MAXBPDNUM][4], *dmx = NULL;
	NBPD = 0;	pdxsw = 3; 	deshxsw = 3;
	GTZDM_ROAD *pzdmLine = NULL; 
	DMX_ROAD  *pdmx = NULL;
	AcDbObjectId  LIdzdm,ZdmPdId = NULL;
	ACHAR GH[8];
	_tcscpy(GH, L"K");
	PDTAB_ROAD tempPD;
	Is_NeedInitPro = true;//Ĭ����Ҫ������·�ܳ�����ʱֱ�����ϱ��µ�

	m_JPInforArray.clear(); 
	m_LevelPtArray.clear();
	m_SlopeArray.clear();
	m_EarthLine.clear();
	m_BPDArray.clear();
	m_LastPDArray.clear();

	rc = ads_entsel(L"\n��ѡ�������ʵ�� : ", en, asDblArray(PT1));
	if (rc == RTCAN)
		return;

	acdbGetObjectId(eId, en);
	if (acdbOpenObject(pObj, eId, AcDb::kForRead) == Acad::eOk)
		pObj->close();
	else
		return;

	if (pObj->isKindOf(DMX_ROAD::desc())) //��DMXʵ��,ȡ����
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
		acutPrintf(L"��ѡʵ�岻�ǵ�����ʵ��!\n");
		return;
	}
	//��ǰͼ���
	if (izdm < 0)
		return;
	//��ȡƽ��ָ��
	JD_CENTER* pzLinep = WorkingData.CreateHorizontalRoad(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname);
	if (!pzLinep)
		return;
	//�淶������ȡ
	pORI_SCALE.STARTLC = pzLinep->DLArray[0].ELC;
	pORI_SCALE.ENDLC = pzLinep->XLLength;
	pORI_SCALE.X_ORI = GlobalZDMTK.ZDMTK[izdm].X0;
	pORI_SCALE.Y_ORI = GlobalZDMTK.ZDMTK[izdm].Y0;
	pORI_SCALE.ZMIN = GlobalZDMTK.ZDMTK[izdm].H0;

	//�����ݶ�����Ʊ�׼
	BasFun.SetZdmMinPC(pzLinep->DV, m_PrfDesData.MAXPD, m_PrfDesData.YBPCMin, m_PrfDesData.JXPCMin, m_PrfDesData.T_YBRshuMin,
		m_PrfDesData.T_JXRShu, m_PrfDesData.A_YBRshuMin, m_PrfDesData.A_JXRShu, m_PrfDesData.YBLRshuMin, m_PrfDesData.JXLRshuMin);

	WorkingData.GetLevelCtrlData(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname, m_LevelPtArray);	//��ȡ���Ƹ߳�����
	WorkingData.GetJPData(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname, m_JPInforArray); //��ȡ��������

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
	//���ǽ����
	for (int i=0;i<m_LevelPtArray.size();i++)
	{
		if (m_LevelPtArray[i].Note.Find(L"����") >= 0)
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

	//�����Ƿ��н�����Ϣ��ȡ���ձ��µ���Ϣ
	GetBPDStAndEdfromJP(NBPD, pdmx);

	//�����Ż����յ���̡��߳�
	GetStAndEdLC(m_StartLC,m_EndLC, m_StartH,m_EndH);

	//��ý���Լ����Χ�ڵĵ�����
	std::vector<DMXYS> LSDMX; //��ʱ�����߱�
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

	//��ó�ʼ����
	if (Is_NeedInitPro)
		GetInitSlope(pzLinep, m_EarthLine);
	else //����·�������³�Ҫ��ʱ�����¶ȣ���ʼ����ֱ�Ӹ�ֵ
		m_SlopeArray = m_BPDArray;

	//�Ż���������
	CProfileOpt* profileOpt = new CProfileOpt();
	profileOpt->m_pPlain = pzLinep; //ƽ��ָ��
	profileOpt->m_DMXLine = m_EarthLine; //����������
	if (m_JPInforArray.size() >0)
		profileOpt->m_JPInforArray = m_JPInforArray; //������Ϣ
	profileOpt->m_SlopeArray = m_SlopeArray; //��ʼ����
	profileOpt->m_BPDArray = m_BPDArray; //���µ���Ϣ�����ж���ʼ���
	profileOpt->StartOptimization(m_LastPDArray,m_BriArray,m_TunArray);

	////δ�Ż�(��ʼ����)
	//for (int i = 0;i<m_SlopeArray.size();i++)
	//	m_BPDArray.push_back(m_SlopeArray[i]);

	//�Ż��γ����շ���
	if (m_JPInforArray.size() > 0)
	{
		//����ʱ���Ż���̴ӽ��µ㿪ʼ��������Ϣ��Ҫ�����շ����ϲ�
		if ((m_JPInforArray[0].Is_JP_Bef == 1 || m_JPInforArray[0].Is_JP_Aft == 1) && Is_NeedInitPro)
		{
			for (int i = 0; i < m_LastPDArray.size(); i++)
				m_BPDArray.push_back(m_LastPDArray[i]);
		}
		else
		{
			//����Ҫ����Լ��,���⵱��·���϶�ʱ��ֱ���󽻵�Ҳ����������Ż�ֱ�Ӹ�ֵ
			m_BPDArray.clear();
			m_BPDArray = m_LastPDArray;
		}
	}
	else
	{
		//û�н�����Ϣ�����ֱ�Ӹ�ֵ
		m_BPDArray.clear();
		m_BPDArray = m_LastPDArray;
	}
		
	//���Ż�����µ����鰴�������
	SortData(m_BPDArray);

	////�γ������ݶ��淽��
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
	
	//����ʵ���������շ������µ����
	NBPD = (int)m_BPDArray.size();

	//����ʵ��
	//����
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
	//���
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
		//����Ӧ��
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(TunId, 'T');
		AcDbObject *pObj;
		if (acdbOpenObject(pObj, ZdmPdId, AcDb::kForWrite) != Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
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
	//����
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
		//����Ӧ��
		GTZDMReactor *pGTZDMReactor = new GTZDMReactor(BriId, 'B');
		AcDbObject *pObj;
		if (acdbOpenObject(pObj, ZdmPdId, AcDb::kForRead) != Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
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

	WorkingData.SetJPData(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname, m_JPInforArray); //�����������
}


//�Ż����µ�����
void ZDMSJ::GetInitSlope(JD_CENTER* pRoad, std::vector<DMXYS>& EarthLine)
{
	//���ò���
	std::vector<DMXYS> SmoothEarthLine;	//ƽ˳������
	std::vector<DMXYS> ThreeDiffQuotientPt, OneDiffQuotientPt;
	std::vector<DMXYS> ChangePt;	//���µ�����
	RoadDesignWorkingDatabase WorkingData;
	
	//���׾������
	GetThreeDiffQuotientPt_ZMSJ(m_EarthLine, SmoothEarthLine, ThreeDiffQuotientPt); 

	//һ�׾������
	GetOneDiffQuotientPt_ZMSJ(ThreeDiffQuotientPt, OneDiffQuotientPt);

	//ȷ��������ͻ��㣬��Ϊ���µĳ�ֵ
	GetChangePt_ZMSJ(pRoad, OneDiffQuotientPt, ChangePt);

	//��ʼ���¶�����
	IniSlopeArrray_ZMSJ(ChangePt);

	//������ʼ�����ķֶ�
	JudgeSection_ZMSJ();

	//�Ե�������ϵ����������ɸ����µ���Ʊ��
	CreatPlanMiu_ZMSJ();

	//�γɳ�ʼ����
	FormInitSlope_ZMSJ();

	//�������ݿ�
	if (!m_SlopeArray.size())
		return;

	//ת�����ֳ���̣������ںţ���ʽ���Դ洢���ݿ�
	CString str;
	for (int i = 0; i < m_SlopeArray.size(); i++)
	{
		double xlc = pRoad->XLC1(m_SlopeArray[i].cml);
		str.Format(L"%0.3lf", xlc);
		m_SlopeArray[i].ml = _wtof(str);
		if (i + 1 < m_SlopeArray.size())
			m_SlopeArray[i].degree = (m_SlopeArray[i + 1].Level - m_SlopeArray[i].Level) / (m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml);
	}

	////��������
	//m_SlopeArray[0].Rshu = 3.0;		//�������С��λ��
	//m_SlopeArray[0].RLen = 3.0;		//���ø߳�С��λ��
	//WorkingData.SetSlopeData(GlobalZDMTK.ZDMTK[izdm].mdbname, GlobalZDMTK.ZDMTK[izdm].RXDname,m_SlopeArray);

}
/*===================================================================
������:JudgeSection
------------------------------------------------------------
��������: ������ʼ�����ķֶ�
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����: �Ժ��н�������鰴�³����¶ȹ淶Ҫ���������
------------------------------------------------------------
��д����:
------------------------------------------------------------
��ע��	
=====================================================================*/
void ZDMSJ::JudgeSection_ZMSJ()//�����¶ȷֶ�
{
	double TempFrontLenth = 0.0; //ǰһ��
	double TempNowLenth = 0.0; //��ǰ��
	double TempBackLenth = 0.0; //��һ��
	double MinLongitudinalSlopeL = _wtof(m_PrfDesData.YBPCMin);

	if (m_SlopeArray.size() == 2)
		return;

	for (int i = 0; i + 1 < (int)m_SlopeArray.size() ; i++)
	{
		//��һ��
		if (i == 0)
		{
			TempNowLenth = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml; //0-1
			TempBackLenth = m_SlopeArray[i + 2].cml - m_SlopeArray[i + 1].cml; //1-2
			if (TempNowLenth < MinLongitudinalSlopeL) //��ǰ���³�С����С�³�
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //��ֵ
				if (TempBackLenth - tempLenth >= MinLongitudinalSlopeL)
				{
					//��һ�¶����ֵ��ȴ��ڵ�����С�³� 1-����
					m_SlopeArray[i + 1].cml = m_SlopeArray[i + 1].cml + tempLenth; 
				}
				else
				{
					//С����ɾ�� i + 1 ���µ�
					m_SlopeArray.erase(m_SlopeArray.begin() + i + 1); 
					i--;
				}
			}
		}
		else if (i == m_SlopeArray.size() - 2) 
		{
			TempNowLenth = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml; // num - 1
			TempFrontLenth = m_SlopeArray[i].cml - m_SlopeArray[i - 1].cml; //num - 2

			if (TempNowLenth < MinLongitudinalSlopeL) //��ǰ���³�С����С�³�
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //��ֵ
				if (TempFrontLenth - tempLenth >= MinLongitudinalSlopeL)
				{
					//ǰһ�¶����ֵ��ȴ��ڵ�����С�³�
					m_SlopeArray[i].cml = m_SlopeArray[i].cml - tempLenth;
				}
				else
				{
					//С����ɾ�� i + 1 ���µ�
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
			if (TempNowLenth < MinLongitudinalSlopeL) //��ǰ���³�С�ڹ淶һ����С�³�
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //��ֵ
				double maxLenth = max(TempFrontLenth, TempBackLenth); //���¶���ǰ�¶αȽ�ȡ���
				if (maxLenth - tempLenth >= MinLongitudinalSlopeL)//���Ե��� ǰ����һ�δ�����С�³�
				{
					if (TempFrontLenth > TempBackLenth)//ǰһ�¶αȺ��¶γ�����ǰ�¶���ǰ�ƶ�
					{
						m_SlopeArray[i].cml = m_SlopeArray[i].cml - tempLenth;
					}
					else//��ǰ�¶������ƶ�
					{
						m_SlopeArray[i + 1].cml = m_SlopeArray[i + 1].cml + tempLenth;
					}
				}
				else //����������С�³�
				{
					if (TempFrontLenth > TempBackLenth)//ǰһ�¶αȺ��¶γ����ϲ�����һ�¶�
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
������:JudgeSection
------------------------------------------------------------
��������: ������ʼ�����ķֶ�
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����: �Ժ��н�������鰴�³����¶ȹ淶Ҫ���������
------------------------------------------------------------
��д����:
------------------------------------------------------------
��ע��
=====================================================================*/
void ZDMSJ::JudgeSection_ZMSJ(std::vector<PDTAB_ROAD>BPDarray)
{
	double TempFrontLenth = 0.0; //ǰһ��
	double TempNowLenth = 0.0; //��ǰ��
	double TempBackLenth = 0.0; //��һ��
	double MinLongitudinalSlopeL = _wtof(m_PrfDesData.YBPCMin);

	if (m_SlopeArray.size() == 2)
		return;

	for (int i = 0; i < (int)BPDarray.size() - 1; i++)
	{
		//��һ��
		if (i == 0)
		{
			TempNowLenth = BPDarray[i + 1].cml - BPDarray[i].cml; //0-1
			TempBackLenth = BPDarray[i + 2].cml - BPDarray[i + 1].cml; //1-2
			if (TempNowLenth < MinLongitudinalSlopeL) //��ǰ���³�С����С�³�
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //��ֵ
				if (TempBackLenth - tempLenth >= MinLongitudinalSlopeL)
				{
					//��һ�¶����ֵ��ȴ��ڵ�����С�³� 1-����
					BPDarray[i + 1].cml = BPDarray[i + 1].cml + tempLenth;
				}
				else
				{
					//С����ɾ�� i + 1 ���µ�
					BPDarray.erase(BPDarray.begin() + i + 1);
					i--;
				}
			}
		}
		else if (i == BPDarray.size() - 2)
		{
			TempNowLenth = BPDarray[i + 1].cml - BPDarray[i].cml; // num - 1
			TempFrontLenth = BPDarray[i].cml - BPDarray[i - 1].cml; //num - 2

			if (TempNowLenth < MinLongitudinalSlopeL) //��ǰ���³�С����С�³�
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //��ֵ
				if (TempFrontLenth - tempLenth >= MinLongitudinalSlopeL)
				{
					//ǰһ�¶����ֵ��ȴ��ڵ�����С�³�
					BPDarray[i].cml = BPDarray[i].cml - tempLenth;
				}
				else
				{
					//С����ɾ�� i + 1 ���µ�
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
			if (TempNowLenth < MinLongitudinalSlopeL) //��ǰ���³�С�ڹ淶һ����С�³�
			{
				double tempLenth = MinLongitudinalSlopeL - TempNowLenth; //��ֵ
				double maxLenth = max(TempFrontLenth, TempBackLenth); //���¶���ǰ�¶αȽ�ȡ���
				if (maxLenth - tempLenth >= MinLongitudinalSlopeL)//���Ե��� ǰ����һ�δ�����С�³�
				{
					if (TempFrontLenth > TempBackLenth)//ǰһ�¶αȺ��¶γ�����ǰ�¶���ǰ�ƶ�
					{
						BPDarray[i].cml = BPDarray[i].cml - tempLenth;
					}
					else//��ǰ�¶������ƶ�
					{
						BPDarray[i + 1].cml = BPDarray[i + 1].cml + tempLenth;
					}
				}
				else //����������С�³�
				{
					if (TempFrontLenth > TempBackLenth)//ǰһ�¶αȺ��¶γ����ϲ�����һ�¶�
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

//����׮���������
//����ͳһ��̸��������򣬲�ɾ����ͬ��׮��
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

	//ɾ����ͬ��׮��
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
������:GetThreeDiffQuotientPt
------------------------------------------------------------
��������:����ƽ˳�����ߵ����ײ���
------------------------------------------------------------
����:	std::vector<DMXYS>&		EarthLine	�����ߵ�
------------------------------------------------------------
���:	std::vector<DMXYS>& SmoothedEarthLine	ƽ˳��ĵ�
		std::vector<DMXYS>& ThreeDiffQuotientPt	���ײ��̵�
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:
------------------------------------------------------------
��ע����Automatic.cpp����ֲ���޸���Ӧ����Ŀ���ݽṹ
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
������:SmoothEarthline
------------------------------------------------------------
��������:ƽ˳������
------------------------------------------------------------
����:	double	MinSlopeLen		��С�³�
		int		SmoothNum		ƽ˳����
		std::vector<DMXYS>& EarthLine	ԭ�����ߵ�����
------------------------------------------------------------
���:	std::vector<DMXYS>& SmoothedEarthLine	ƽ˳��ĵ����ߵ�����
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:
------------------------------------------------------------
��ע����Automatic.cpp��PsDmx()��ֲ���޸���Ӧ����Ŀ���ݽṹ
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

	for (i = 0; i < EarthLine.size(); i++)    //��һ��ƽ˳
	{
		tmpPt.lc = EarthLine[i].lc;
		tmpPt.level = GetSmoothedHigh_ZMSJ(i, MinSlopeLen, EarthLine);
		SmoothedEarthLine.push_back(tmpPt);
	}

	for (i = 1; i < SmoothNum; i++)                 //���ƽ˳��������1������ƽ˳
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
������:GetSmoothedHigh
------------------------------------------------------------
��������:��ȡƽ˳��ĵ����߸߳�
------------------------------------------------------------
����:	int i	������
		double MinSlopeLen	��С�³�
		std::vector<DMXYS>& EarthLine	�����ߵ�����
------------------------------------------------------------
���:	double ����ֵ	ƽ˳��ĵ����߸߳�
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:
------------------------------------------------------------
��ע��	��Automatic.cpp��
		GetPsHigh(int i,double R,UltraCArray_T<gridPoint,gridPoint> &ZMdmxPt)
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
double ZDMSJ::GetSmoothedHigh_ZMSJ(int i, double MinSlopeLen, std::vector<DMXYS>& EarthLine)
{
	double totalL(0.0), totalR(0.0), dltX(0.0);
	double FLeft(0.0), FRight(0.0);
	double high;
	int j;
	//�������
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
	//�����Ҳ�
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

	//��ø߳�
	high = (totalL + totalR + EarthLine[i].level) / (FLeft + FRight + 1.0);

	return high;
}

/*===================================================================
������:GetSmoothedHigh
------------------------------------------------------------
��������:��ȡƽ˳��ĵ����߸߳�
------------------------------------------------------------
����:	int i	������
		double MinSlopeLen	��С�³�
		int tp
		std::vector<DMXYS>& EarthLine	�����ߵ�����
------------------------------------------------------------
���:	double ����ֵ	ƽ˳��ĵ����߸߳�
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:
------------------------------------------------------------
��ע��	��Automatic.cpp��
		GetPsHigh(int i,double R,UltraCArray_T<gridPoint,gridPoint> &ZMdmxPt)
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
double ZDMSJ::GetSmoothedHigh_ZMSJ(int i, double MinSlopeLen, int tp, std::vector<DMXYS>& EarthLine)
{
	return 0.0;
}
/*===================================================================
������:GetOneDiffQuotientPt
------------------------------------------------------------
��������:����һ�ײ���
------------------------------------------------------------
����:	std::vector<DMXYS>& ThreeDiffQuotientPt	���ײ�������
------------------------------------------------------------
���:	std::vector<DMXYS>& OneDiffQuotientPt	һ�ײ�������
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		GetOneDiffQuotientPt(UltraCArray_T<gridPoint,gridPoint> &ThreeDiffQuotientPt,UltraCArray_T<gridPoint,gridPoint> &OneDiffQuotientPt);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
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
������:GetChangePt
------------------------------------------------------------
��������:��ȡ���µ�
------------------------------------------------------------
����:	JD_CENTER* pRoad	��·ƽ��ָ��
		std::vector<DMXYS>& OneDiffQuotientPt	һ�ײ��̵�
------------------------------------------------------------
���:	std::vector<DMXYS>& ChangePt	���µ�����
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void GetChangePt(PlaneCAL* pGTZX,UltraCArray_T<gridPoint,gridPoint> &OneDiffQuotientPt,UltraCArray_T<gridPoint,gridPoint> &ChangePt);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
void ZDMSJ::GetChangePt_ZMSJ(JD_CENTER* pRoad, std::vector<DMXYS>& OneDiffQuotientPt, std::vector<DMXYS>& ChangePt)
{
	ChangePt.clear();
	DMXYS temppt;

	/////////////////////////////////�������////////////////////////////
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

	////////////////////////////////�����յ�///////////////////////////////
	temppt.lc = pRoad->LenOfLine;
	temppt.level = m_EndH;
	ChangePt.push_back(temppt);

}

/*===================================================================
������:IniSlopeArrray
------------------------------------------------------------
��������:��ʼ���¶�����
------------------------------------------------------------
����:	std::vector<DMXYS>& OneDiffQuotientPt	���µ�����
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��
=====================================================================*/
void ZDMSJ::IniSlopeArrray_ZMSJ(std::vector<DMXYS>& ChangePt)
{
	//���ò���
	PDTAB_ROAD tempSlope;

	//��ʼ���¶�����
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
������:CreatPlanMiu
------------------------------------------------------------
��������:���ɸ����µ���
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		bool CreatPlanMiu(PlanePlan &oneplane, bool IsMinTW,bool YorN);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
bool ZDMSJ::CreatPlanMiu_ZMSJ()
{
	for (int SlopePtIndex = 0; SlopePtIndex < m_SlopeArray.size(); SlopePtIndex++)
	{
		if (SlopePtIndex == 0)//���
			m_SlopeArray[SlopePtIndex].Level = m_StartH;
		else if (SlopePtIndex == m_SlopeArray.size() - 1)//�յ�
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
������:CreateSloptPtDesH
------------------------------------------------------------
��������:���ɵ�����ΪSlopePtIndex�ı��µ���Ƹ߳�
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		bool CreateSloptPtDesH(PlanePlan &oneplane,int SlopePtIndex, bool IsMinTW,bool YorN );
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
bool ZDMSJ::CreateSloptPtDesH_ZMSJ(int SlopePtIndex)
{
	double dmxnumber;
	double minH, maxH;
	double temp;
	double FrontMileage = 0.0;//ǰһ�����µ�����
	double Fronthigh = 0.0;//ǰһ�����µ�ĸ߳�
	double Nowhigh = 0.0;//��ǰ���µ�ĸ߳�
	double NowMileage = 0.0;//��ǰ���µ�����
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
	/* �����¶ȴ�����ȷ����ǰ���µ����С���߳� */
	/************************************************************************/
	double miu = 0.0;//����
	bool cheekKZD = true;//���ں����жϱ��µ��ڸ̱߳ؾ�����ǰ��
	bool cheek = true;//�߳̿��Ƶ��ڸ��¶���ʱΪfalse
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
		//double sigma = (MaxRoundGrade - MinRoundGrade) / 1.5;//���� 
		double sigma = min(maxH - MiuDesH, MiuDesH - minH) / 3.0;//����
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

	///////////////////////////////�����¶ȣ�����������Ƹ߳�///////////////////////////////////
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
������:AdjustGCKZD
------------------------------------------------------------
��������:�����߳̿��Ƶ�
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void AdjustGCKZD(PlanePlan &oneplane, double StartCml, double StartH, double EndCml, double EndH, bool IsAdjustDMX=true);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
void ZDMSJ::AdjustGCKZD_ZMSJ(double StartCml, double StartH, double EndCml, double EndH)
{
	std::vector<GCKZPt> SX_GCKZDArry, XX_GCKZDArry;	//�����޸߳̿��Ƶ�����
	GCKZPt TempGCKZD;
	std::vector<GCKZPt> Temp_SX_GCKZDArry, Temp_XX_GCKZDArry;//��ʱ�߳̿��Ƶ�����
	double MinH, MaxH;

	//���
	TempGCKZD.Cml = StartCml;
	TempGCKZD.DesHSX = StartH;
	TempGCKZD.DesHXX = StartH;

	TempGCKZD.Style = 1;
	SX_GCKZDArry.push_back(TempGCKZD);

	TempGCKZD.Style = 2;
	XX_GCKZDArry.push_back(TempGCKZD);

	//�յ�
	TempGCKZD.Cml = EndCml;
	TempGCKZD.DesHSX = EndH;
	TempGCKZD.DesHXX = EndH;

	TempGCKZD.Style = 1;
	SX_GCKZDArry.push_back(TempGCKZD);

	TempGCKZD.Style = 2;
	XX_GCKZDArry.push_back(TempGCKZD);

	//���յ��γ��ŷ�����
	GetEnvelop_ZMSJ(SX_GCKZDArry, XX_GCKZDArry);

	//������û����õ������޿��Ƶ�,�ȿ������ޣ��ٿ�������
	InsertGCKZDArray_ZMSJ(SX_GCKZDArry, XX_GCKZDArry, m_UserSetGCKZDArray_SX, true);
	InsertGCKZDArray_ZMSJ(SX_GCKZDArry, XX_GCKZDArry, m_UserSetGCKZDArray_XX, false);

	GetEnvelop_ZMSJ(SX_GCKZDArry, XX_GCKZDArry);
}

/*===================================================================
������:GetEnvelop
------------------------------------------------------------
��������:�����߳̿��Ƶ�
------------------------------------------------------------
����:	std::vector<GCKZPt>& SX_GCKZDArry	���п��Ƶ�����
		std::vector<GCKZPt>& XX_GCKZDArry	���п��Ƶ�����
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void GetEnvelop(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
void ZDMSJ::GetEnvelop_ZMSJ(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry)
{
	GCKZPt TempGCKZD;
	//�����޸߳̿��Ƶ���������޳���ͬ��̴��Ŀ��Ƶ�
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

	//�����޸߳̿��Ƶ���������޳���ͬ��̴��Ŀ��Ƶ�
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

	//�γ��ŷ�����
	MaxEnvPtArray.clear();
	MinEnvPtArray.clear();
	EnvPt TempEnvPt;
	//ѭ���������߳̿��Ƶ���룬ÿ����һ���߳̿��Ƶ�ͬʱ����Ҫ����ǰ���Ƶ���ǰһ���߳̿��Ƶ�֮��ĵ����
	//��ֱ���󽻼��������Ƶ��м�ĵ�
	double BefLineEnd_Mileage, BefLineEnd_Lever;//ǰһ���Ƶ����������С��������Lengh֮�����̺͸߳�
	double AftLine_Start_Mileage, AftLine_Start_Mileage_Lever;//��ǰ���Ƶ����������С����ǰ��Lengh֮�����̺͸߳�
	int Res = 0;
	double MaxSlope_SX = _wtof(m_PrfDesData.MAXPD);
	double MaxSlope_XX = _wtof(m_PrfDesData.MAXPD);
	double MAX_Cml, MAX_H, MIN_Cml, MIN_H;
	//�������޵�
	//��������
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
	//�����������޵�����
	TempEnvPt.Mileage = XX_GCKZDArry[0].Cml;
	TempEnvPt.Lever = XX_GCKZDArry[0].DesHXX;
	MinEnvPtArray.push_back(TempEnvPt);
	//�������޵�
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
������:GetJD
------------------------------------------------------------
��������:��ȡ����
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void GetEnvelop(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
int ZDMSJ::GetJD_ZMSJ(double Scml, double SLever, double Ecml, double ELever,
	double& MAX_cml, double& MAX_H, double& MIN_cml, double& MIN_H)
{
	///////////////////////////////////////�õ��ŷ�����Ľ���//////////////////////////////////

	double QZminH = 0.0;//���յ�ȷ������С�߳�
	double QZmaxH = 0.0;//���յ�ȷ�������߳�
	double x1 = Scml;
	double y1 = SLever;

	double maxx2 = Ecml;
	double maxy2 = y1 + (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	double x3 = Ecml;
	double y3 = ELever;

	double maxx4 = x1;
	double maxy4 = y3 + (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	BAS_DRAW_FUN::INTERS(&MAX_cml, &MAX_H, x1, y1, maxx2, maxy2, x3, y3, maxx4, maxy4);//�����ŷ��������ߵ�
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
	BAS_DRAW_FUN::INTERS(&MIN_cml, &MIN_H, x1, y1, minx2, miny2, x3, y3, minx4, miny4);//�����ŷ��������͵�
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
������:InsertGCKZDArray
------------------------------------------------------------
��������:������Ƶ�����
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void InsertGCKZDArray(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry,
		UltraCArray_T<GCKZPt,GCKZPt>& GCKZDArray, bool IsSX);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
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
������:GetEnvelopRange
------------------------------------------------------------
��������:�õ��ŷ�������ȷ������̷�Χ
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	��Automatic.cpp��
		bool GetEnvelopRange(double Mileage,double& MinH, double& MaxH);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
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
		if (Mileage < MaxEnvPtArray[i].Mileage)//�ҵ���ǰ�¶εĺ�һ���ŷ������
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
		if (Mileage < MinEnvPtArray[i].Mileage)//�ҵ���һ�����ڵ�ǰ��̵��ŷ������
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
������:GetEnvelopRange
------------------------------------------------------------
��������:�õ��ŷ�������ȷ������̷�Χ
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
			bool GetEnvelopRange(double PreDesH, double PreMileage, double Mileage,double& MinH, double& MaxH);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
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
		if (Mileage < MaxEnvPtArray[i].Mileage)//�ҵ���ǰ�¶εĺ�һ���ŷ������
		{
			//for (j = i-1; j>=0; j--)
			//{
			//	if (MaxEnvPtArray[j].Mileage < PreMileage+0.01)//�ҵ���ǰ�¶ε�ǰһ���ŷ������
			//		break;
			//}

			//if (j>=0 && (i-j)>1)//���¶������ŷ������j+1��i�����е�ȡ��С��
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
		if (Mileage < MinEnvPtArray[i].Mileage)//�ҵ���һ�����ڵ�ǰ��̵��ŷ������
		{
			//for (j = i-1; j>=0; j--)
			//{
			//	if (MinEnvPtArray[j].Mileage < PreMileage+0.01)//�ҵ���ǰ�¶ε�ǰһ���ŷ������
			//		break;
			//}

			//if ((j>=0) && (i-j>1))//���¶������ŷ������j+1��i�����е�ȡ��С��
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
������:InsertGCKZD
------------------------------------------------------------
��������:���Ҹ߳̿��Ƶ��������е�λ��
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190807
------------------------------------------------------------
��ע��	1.��Automatic.cpp��
		void InsertGCKZD(UltraCArray_T<GCKZPt,GCKZPt>& GCKZDArry, GCKZPt KZD);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
		2.INTERS������ʱ��Ч������ֱ�Ӵ�BAS_DRAW_FUN������һ������
=====================================================================*/
void ZDMSJ::InsertGCKZD_ZMSJ(std::vector<GCKZPt>& GCKZDArry, GCKZPt KZD)
{
	int Index = 0;
	GCKZPt PreKZD, NextKZD;
	double InterCml, InterH;
	double x1, y1, x2, y2, x3, y3, x4, y4;
	double Slope = _wtof(m_PrfDesData.MAXPD);
	double SlopeLen = 0.0;//�³�
	bool IsInsertStart = false, IsInsertEnd = false;
	for (int i = 0; i < GCKZDArry.size() - 1; i++)
	{
		if ((KZD.Cml > GCKZDArry[i].Cml + 0.1) && (KZD.Cml < GCKZDArry[i + 1].Cml + 0.1))//�ҵ����Ƶ�KZD�Ĳ���λ��
		{
			PreKZD = GCKZDArry[i];
			NextKZD = GCKZDArry[i + 1];
			//���ÿ��Ƶ㡢ǰ/��һ�����Ƶ��Ƿ��ж����
			if (KZD.Style == 1)//���޿��Ƶ�
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

				//if (0 == INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHSX, x1, y1, x2, y2, KZD.Cml, KZD.DesHSX))//��ʵ���㣬����ÿ��Ƶ�
				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHSX, x1, y1, x2, y2, KZD.Cml, KZD.DesHSX))//��ʵ���㣬����ÿ��Ƶ�
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

				if (!IsInsertStart)//����ǰһ�����Ƶ㲻�ܲ���
				{
					if (KZD.DesHSX < PreKZD.DesHSX)//�¼����С�����ǵ�ǰ���Ǹ������򱣳ֲ���
						GCKZDArry[i] = KZD;
				}

				if (!IsInsertEnd)//���Ǻ�һ�����Ƶ㲻�ܲ���
				{
					if (KZD.DesHSX < NextKZD.DesHSX)//�¼����С�����ǵ�ǰ���Ǹ������򱣳ֲ���
						GCKZDArry[i + 1] = KZD;
				}
			}

			if (KZD.Style == 2)//���޿��Ƶ�
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

				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHXX, x1, y1, x2, y2, KZD.Cml, KZD.DesHXX))//��ʵ���㣬����ÿ��Ƶ�
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

				if (!IsInsertStart)//����ǰһ�����Ƶ㲻�ܲ���
				{
					if (KZD.DesHXX > PreKZD.DesHXX)//�¼����С�����ǵ�ǰ���Ǹ������򱣳ֲ���
						GCKZDArry[i] = KZD;
				}

				if (!IsInsertEnd)//���Ǻ�һ�����Ƶ㲻�ܲ���
				{
					if (KZD.DesHXX > NextKZD.DesHXX)//�¼����С�����ǵ�ǰ���Ǹ������򱣳ֲ���
						GCKZDArry[i + 1] = KZD;
				}
			}
		}
	}
}

/*===================================================================
������:CalSloptPtDesHRange
------------------------------------------------------------
��������:������µ�̷߳�Χ
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		bool CalSloptPtDesHRange(bool YorN,PlanePlan &oneplane,int SlopePtIndex,double& minH,double& maxH);
		��ֲ���޸���Ӧ����Ŀ���ݽṹ
=====================================================================*/
bool ZDMSJ::CalSloptPtDesHRange_ZMSJ(int SlopePtIndex, double& minH, double& maxH)
{
	//��������Ӵ���
	double FrontfrontMileage = 0.0;//ǰ�������µ�����
	double Frontfronthigh = 0.0;//ǰ�������µ�ĸ߳�
	double FrontMileage = 0.0;//ǰһ�����µ�����
	double Fronthigh = 0.0;//ǰһ�����µ�ĸ߳�
	double Nowhigh = 0.0;//��ǰ���µ�ĸ߳�
	double NowMileage = 0.0;//��ǰ���µ�����
	double FrontPD = 0.0;//ǰһ���¶�ֵ
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
		minH = Fronthigh - (NowMileage - FrontMileage) * temppd;//��С�߳�
		maxH = Fronthigh + (NowMileage - FrontMileage) * temppd;//���߳�
	}
	else
	{

		FrontfrontMileage = m_SlopeArray[SlopePtIndex - 2].cml;
		Frontfronthigh = m_SlopeArray[SlopePtIndex - 2].Level;
		FrontPD = (Fronthigh - Frontfronthigh) / (FrontMileage - FrontfrontMileage);
		tempPD = min(temppd, (FrontPD + MaxSlopeDifference));//����������¶ȴ�����
		maxH = Fronthigh + (NowMileage - FrontMileage) * tempPD;//���߳�(����������)
		//maxH = Fronthigh+(NowMileage-FrontMileage)*temppd;

		tempPD = max(-temppd, (FrontPD - MaxSlopeDifference));//����������¶ȴ�����
		minH = Fronthigh + (NowMileage - FrontMileage) * tempPD;//��С�߳�
		//minH = Fronthigh-(NowMileage-FrontMileage)*temppd;//��С�߳�
	}

	////////////////////////////�����ŷ�����//////////////////////////////////////////
	double TempMinH;//��ʱ��С�߳�
	double TempMaxH;//��ʱ���߳�
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
	//ads_printf("$$$$$$$$$4���%lf,�ŷ�����%lf,�ŷ�����%lf\n",NowMileage,TempMinH,TempMaxH);
	minH = max(minH, TempMinH);
	maxH = min(TempMaxH, maxH);
	if (maxH < minH - 0.01)//�ŷ����򶥵㴦���޷�ͬʱ�����¶ȴ��������ŷ�����ֻ�����ŷ�����
	{
		//��Ϊֻ�����¶ȴ�������һ��
		maxH = TempMaxH;
		minH = TempMinH;
	}
	return true;
}
/*===================================================================
������:NormalRandom
------------------------------------------------------------
��������:�������������ȡֵ��Χ���õ�����̬�ֲ��������
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		double NormalRandom(double miu, double sigma, double min, double max);
		��ֲ
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
������:AverageRandom
------------------------------------------------------------
��������:�õ����ȷֲ������
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		double AverageRandom(double min, double max);
		��ֲ
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
������:Normal
------------------------------------------------------------
��������:����x�ĸ����ܶȺ���
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		double Normal(double x, double miu, double sigma);
		��ֲ
=====================================================================*/
double ZDMSJ::Normal_ZMSJ(double x, double miu, double sigma)
{
	return 1.0 / sqrt(2 * PI * sigma) * exp(-1 * (x - miu) * (x - miu) / (2 * sigma * sigma));
}

/*===================================================================
������:Env_GetMiu
------------------------------------------------------------
��������:���ݵ����߼������ڷ���С�߳�
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void Env_GetMiu(PlanePlan &oneplane,int PDNum,double &Miu);
		��ֲ
=====================================================================*/
void ZDMSJ::Env_GetMiu_ZMSJ(int PDNum, double& Miu)
{
	double FrontMileage;//ǰһ�����µ�����
	double Fronthigh;//ǰһ�����µ�ĸ߳�
	double NowMileage;//��ǰ���µ�����
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
		K = -sum1 / sum2;//б��
	}
	Miu = K * (m_SlopeArray[PDNum].cml - FrontMileage) + Fronthigh;
}

/*===================================================================
������:GetDMX1
------------------------------------------------------------
��������:��ȡ������
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void GetDMX1(PlanePlan &oneplane,double StratM, double EndM);
		��ֲ
=====================================================================*/
void ZDMSJ::GetDMX1_ZMSJ(double StratM, double EndM)
{
	/////////////////////////////////////��ȡ��Ч������/////////////////////////////
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
������:RoundSlope
------------------------------------------------------------
��������:�¶�ȡ��
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void RoundSlope(ProfilePlan& profile);
		��ֲ
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
������:MergeSlope
------------------------------------------------------------
��������:�ϲ��¶�
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		void MergeSlope(PlanePlan& Profile);
		��ֲ
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
		if (fabs(AftGrade - BefGrade) < 0.02)		//��ʱ��0.02��2%���¶ȴ���
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
������:SlopePtCanDel
------------------------------------------------------------
��������:�ж�Profile������SlopetIndex���µ��ܷ�ɾ��
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		bool SlopePtCanDel(PlanePlan& Profile, int StartIndex, int EndIndex);
		��ֲ
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

		if (fabs(Slope - BefSlope) > _wtof(m_PrfDesData.MAXPD) / 100.0 * 2.0 + 0.0000001)	//����¶ȴ�����
			return false;
	}

	if (EndIndex < SlopePtSum - 2)
	{
		AftSlope = (m_SlopeArray[EndIndex + 2].Level - m_SlopeArray[EndIndex + 1].Level) /
			(m_SlopeArray[EndIndex + 2].cml - m_SlopeArray[EndIndex + 1].cml);

		if (fabs(AftSlope - Slope) > _wtof(m_PrfDesData.MAXPD) / 100.0 * 2.0 + 0.0000001)	//����¶ȴ�����
			return false;
	}

	return true;
}

/*===================================================================
������:FormFinalSlope
------------------------------------------------------------
��������:�γɳ�ʼ�ݶ��淽��
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		bool FormFinalSlope(PlanePlan& plan);
		��ֲ
=====================================================================*/
void ZDMSJ::FormInitSlope_ZMSJ()
{
	MergeSlope_ZMSJ(m_SlopeArray);
	RoundSlope_ZMSJ(m_SlopeArray);
}

/*===================================================================
������:FormFinalSlope
------------------------------------------------------------
��������:�γ������ݶ��淽��
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	��Automatic.cpp��
		bool FormFinalSlope(PlanePlan& plan);
		��ֲ
=====================================================================*/
void ZDMSJ::FormFinalSlope_ZMSJ()
{
	MergeSlope_ZMSJ(m_BPDArray);
	RoundSlope_ZMSJ(m_BPDArray);
}

//��ȡ��ʼ����������̸߳�
void ZDMSJ::GetStAndEdLC(double &StartLC, double &EndLC, double &StLevel, double &EdLevel)
{
	if (m_JPInforArray.size() > 0)
	{
		if (m_JPInforArray[0].Is_JP_Bef == 1 && m_JPInforArray[0].Is_JP_Aft == 1) //ǰ��Լ��
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
				//�����³����ƣ��󽻱��µ�ʱ����
				StartLC = m_BPDArray[0].ml; StLevel = m_BPDArray[0].Level;
				EndLC = m_BPDArray[PDnum -1].ml; EdLevel = m_BPDArray[PDnum - 1].Level;
			}
		}
		else if (m_JPInforArray[0].Is_JP_Bef == 1 && m_JPInforArray[0].Is_JP_Aft == 0) //ǰԼ����Լ��
		{
			//BPDnum = 3
			StartLC = m_BPDArray[1].ml;  StLevel = m_BPDArray[1].Level;
			EndLC = m_BPDArray[2].ml;  EdLevel = m_BPDArray[2].Level;
		}
		else if (m_JPInforArray[0].Is_JP_Bef == 0 && m_JPInforArray[0].Is_JP_Aft == 1) //ǰ��Լ����Լ��
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
������:GetBPDStAndEdfromJP
------------------------------------------------------------
��������:���ݽ�����Ϣ�γ�ԭʼ���µ�����
------------------------------------------------------------
����:
------------------------------------------------------------
���:
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20190808
------------------------------------------------------------
��ע��	[i][0]��� [i][1]�߳� [i][2]������ [i][3]�¶�
=====================================================================*/
void ZDMSJ::GetBPDStAndEdfromJP(int&BPDnum, DMX_ROAD *pdmx)
{
	double PD_array[MAXBPDNUM][4];
	bool Is_Constr_Start = true; 	//����Ƿ���ҪԼ��
	bool Is_Constr_End = true; 	//�յ��Ƿ���ҪԼ��
	double JXPCMin = _wtof(m_PrfDesData.JXPCMin);
	double YBPCMin = _wtof(m_PrfDesData.YBPCMin);//һ���³���Ϊ�ж�
	double  ml = 0.0,level=0.0;


	for (int i = 0; i < MAXBPDNUM; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			PD_array[i][j] = 0.0;
		}
	}


	//���ڽ�����Ϣ����̳�ʼֵ
	if (m_JPInforArray.size() > 0)
	{
		m_JPInforArray[0].Stcml = pzLinep->DLArray[0].ELC;
		m_JPInforArray[0].Edcml = pzLinep->XLLength;
	}

	if (m_JPInforArray.size() > 0)
	{
		//����ǰ��Լ�����,������С�³�Ҫ��
		if (m_JPInforArray[0].Bef_PC > YBPCMin)
			Is_Constr_Start = false;
		if (m_JPInforArray[0].Aft_PC > YBPCMin)
			Is_Constr_End = false;


		if (Is_Constr_Start && Is_Constr_End) //ǰ����Լ��
		{
			//����·�ܳ�С��������С�³����Һ���ǰ��˽���Լ������ʱֱ�����ϱ��µ�
			if (YBPCMin < pzLinep->XLLength && pzLinep->XLLength < 2.5 * YBPCMin)
			{
				//��ʼ�¶Ρ�0��
				PD_array[0][0] = m_JPInforArray[0].Stcml;//��� 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //�߳�

				//���С��Χ�����н������Ͻ���
				if (AutoCalPVI(ml, level))
				{
					Is_NeedInitPro = false;
					PD_array[1][0] = ml;
					PD_array[1][1] = level;
					//��ֹ�¶Ρ�2��
					PD_array[2][0] = m_JPInforArray[0].Edcml;
					PD_array[2][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 3;
				}
				else //�޽��������յ���Ϣ
				{
					PD_array[1][0] = m_JPInforArray[0].Edcml;
					PD_array[1][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 2;
				}

				//AutoCalPVI(PD_array[1][0], PD_array[1][1]);
			}
			else
			{
				//��ʼ�¶Ρ�0��
				PD_array[0][0] = m_JPInforArray[0].Stcml;//��� 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //�߳�

				//��1��
				if (m_JPInforArray[0].Bef_PC < YBPCMin)
					PD_array[1][0] = PD_array[0][0] + YBPCMin - m_JPInforArray[0].Bef_PC;
				PD_array[1][1] = (PD_array[1][0] - PD_array[0][0])*m_JPInforArray[0].Bef_PD*0.01 + PD_array[0][1]; //�߳�

				//��ֹ�¶Ρ�2��
				PD_array[3][0] = m_JPInforArray[0].Edcml;
				PD_array[3][1] = m_JPInforArray[0].Aft_H;

				//��3��
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
		else if (Is_Constr_Start && Is_Constr_End == false) //ǰԼ����Լ��
		{
			//��ʼ�¶Ρ�0��
			PD_array[0][0] = m_JPInforArray[0].Stcml;//��� 
			PD_array[0][1] = m_JPInforArray[0].Bef_H; //�߳�

			//��1��
			if (m_JPInforArray[0].Bef_PC < YBPCMin)
				PD_array[1][0] = PD_array[0][0] + YBPCMin - m_JPInforArray[0].Bef_PC;

			PD_array[1][1] = (PD_array[1][0] - PD_array[0][0])*m_JPInforArray[0].Bef_PD*0.01 + PD_array[0][1]; //�߳�

			//��ֹ�¶Ρ�2��
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
		else if (Is_Constr_Start == false && Is_Constr_End) //ǰ��Լ����Լ��
		{
			//��ʼ�¶Ρ�0��
			PD_array[0][0] = m_JPInforArray[0].Stcml;//��� 
			PD_array[0][1] = m_JPInforArray[0].Bef_H; //�߳�

			//��ֹ�¶Ρ�2��
			PD_array[2][0] = m_JPInforArray[0].Edcml;
			PD_array[2][1] = m_JPInforArray[0].Aft_H;

			//��1��
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
		else //ǰ�󶼲�Լ��
		{
			if (YBPCMin < pzLinep->XLLength && pzLinep->XLLength < 2.5 * YBPCMin)
			{
				//��ʼ�¶Ρ�0��
				PD_array[0][0] = m_JPInforArray[0].Stcml;//��� 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //�߳�

				/*AutoCalPVI(PD_array[1][0], PD_array[1][1]);*/

				//���С��Χ�����н������Ͻ���
				if (AutoCalPVI(ml, level))
				{
					Is_NeedInitPro = false;
					PD_array[1][0] = ml;
					PD_array[1][1] = level;
					//��ֹ�¶Ρ�2��
					PD_array[2][0] = m_JPInforArray[0].Edcml;
					PD_array[2][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 3;
				}
				else //�޽��������յ���Ϣ
				{
					PD_array[1][0] = m_JPInforArray[0].Edcml;
					PD_array[1][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 2;
				}
			}
			else
			{
				//��ʼ�¶Ρ�0��
				PD_array[0][0] = m_JPInforArray[0].Stcml;//��� 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //�߳�

				//��ֹ�¶Ρ�1��
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
		//��ʼ�¶Ρ�0��
		PD_array[0][0] = pzLinep->DLArray[0].ELC;//��� 
		PD_array[0][1] = pdmx->DMXys[0].level; //�߳�

		//��ֹ�¶Ρ�1��
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
		//ǰ���Լ����ֵ
		if (Is_Constr_Start)
			m_JPInforArray[0].Is_JP_Bef = 1;
		if (Is_Constr_End)
			m_JPInforArray[0].Is_JP_Aft = 1;
	}

}

//���Զ��ϸ��¶�,�н��㷵��true���޽��㷵��false
bool ZDMSJ::AutoCalPVI(double &ml,double &H)
{
	ads_point from1, to1;//����¶��߶����˵�
	ads_point from2, to2;//�յ��¶��߶����˵�
	ads_point result;	 //�����¶ν���

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

	//�����û�н������ƽ��
	if (iTest >= 0)
		return true;
	else
		return false;

}