// LLTuLink.cpp: implementation of the LLTuLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "LLTuLink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LLTuLink::LLTuLink()
{

}

LLTuLink::~LLTuLink()
{

}

//传入三个交点，前缓，后缓
LLTuLink::LLTuLink(double n1,double e1,double n0,double e0,double n2,double e2,double a1,double a2)
{
	Set(n1,e1,n0,e0,n2,e2,a1,a2);
}	

void LLTuLink::Set(double n1,double e1,double n0,double e0,double n2,double e2,double a1,double a2)
{
	//////////////////////////////////////////
	ERRFlag=false;
	N1=n1;	E1=e1;
	N0=n0;	E0=e0;
	N2=n2;	E2=e2;
	A1=a1;	A2=a2;
	//////////////////////////////////////////////////////////////////////////
	//求方位角，转角及左右转
	xyddaa(N1,E1,N0,E0,&fwj1);
	xyddaa(N0,E0,N2,E2,&fwj2);
	double aa=fwj2-fwj1;
	if(aa<-PI)aa+=2.0*PI;
	if(aa>PI)aa-=2.0*PI;
	if(aa<0)LRFlag=-1;//左转
	else LRFlag=1;//右转
	afa=fabs(aa);//转角
	//求半径
	R=sqrt((A1*A1+A2*A2)/2.0/afa);
}

void LLTuLink::CreateSpiral()
{
	//生成缓和曲线实体
	//前后缓的转角
	afa1=0.5*A1*A1/R/R;
	afa2=0.5*A2*A2/R/R;
	//前后缓长
	Lo1=A1*A1/R;
	Lo2=A2*A2/R;

	double BPMTL[10];

	getBPMTL(dms_rad(afa),R,Lo1,Lo2,&BPMTL[0]);
	T1=BPMTL[6];//前切长
	T2=BPMTL[7];//后切长

	//求直缓点的坐标
	Nzh=N0+T1*cos(fwj1+PI);
	Ezh=E0+T1*sin(fwj1+PI);

	//求缓直点的坐标
	Nhz=N0+T2*cos(fwj2);
	Ehz=E0+T2*sin(fwj2);

	//前后缓实体
	pSPIRAL1.Set(Nzh,Ezh,fwj1,A1,VERY_MAX,R,LRFlag);
	pSPIRAL2.Set(Nhz,Ehz,fwj2+PI,A2,VERY_MAX,R,-LRFlag);
}

void LLTuLink::CreateXL()
{
	JdCenter *jdcen=new JdCenter[3];
	int i=0;
	jdcen[i].JDXZ=IsJiaoDian;
	jdcen[i].N=N1;
	jdcen[i].E=E1;
	jdcen[i].LinkAfter=NotRelation;
	jdcen[i].IsOrNotHuiTou=false;
	jdcen[i].R=0.0;
	jdcen[i].A1=0.0;
	jdcen[i].A2=0.0;

	i=1;
	jdcen[i].JDXZ=IsJiaoDian;
	jdcen[i].IsOrNotHuiTou=false;
	jdcen[i].LinkAfter=NotRelation;
	jdcen[i].N=N0;
	jdcen[i].E=E0;
	jdcen[i].R=R;
	jdcen[i].A1=A1;
	jdcen[i].A2=A2;
	jdcen[i].LeftOrRight=LRFlag;

	i=2;
	jdcen[i].JDXZ=IsJiaoDian;
	jdcen[i].N=N2;
	jdcen[i].E=E2;
	jdcen[i].LinkAfter=NotRelation;
	jdcen[i].IsOrNotHuiTou=false;
	jdcen[i].R=0.0;
	jdcen[i].A1=0.0;
	jdcen[i].A2=0.0;

	JD_CENTER *JdCenObj=new JD_CENTER;
	JdCenObj->SetJdCenterArray(jdcen,3);
	BAS_DRAW_FUN::AddEntityToDbs(JdCenObj);
	delete []jdcen;
}

/*

void LLTuLink::AppendXL(JD_CENTER *JdCenObj,int jdno)
{
	JdCenter *jdcen=new JdCenter[2];
	
	i=0;
	jdcen[i].JDXZ=IsJiaoDian;
	jdcen[i].IsOrNotHuiTou=false;
	jdcen[i].LinkAfter=NotRelation;
	jdcen[i].N=N0;
	jdcen[i].E=E0;
	jdcen[i].R=R;
	jdcen[i].A1=A1;
	jdcen[i].A2=A2;
	jdcen[i].LeftOrRight=LRFlag;
	
	i=1;
	jdcen[i].JDXZ=IsJiaoDian;
	jdcen[i].N=N2;
	jdcen[i].E=E2;
	jdcen[i].LinkAfter=NotRelation;

	JdCenObj->JdCenArray.RemoveAt(jdno+1,JdCenObj->JdCenNum-(jdno+1));
	JdCenObj->JdCenArray.Add(JdCenter[0]);
	JdCenObj->JdCenArray.Add(JdCenter[1]);
	JdCenObj->JdCenNum=JdCenObj->JdCenArray.GetSize();

	delete []jdcen;
}

*/
