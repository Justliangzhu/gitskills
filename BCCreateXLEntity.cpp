// BCCreateXLEntity.cpp: implementation of the BCCreateXLEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCCreateXLEntity.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "XLDataBase.h"
BCCreateXLEntity::BCCreateXLEntity()
{
	OK1=false;
	pGtzx=NULL;
	A1=0.0;
	A2=0.0;
}

BCCreateXLEntity::~BCCreateXLEntity()
{

}

void BCCreateXLEntity::SetARC(double cn, double ce, double r, 
							  double startangle, double endangle)
{
	OK1=true;

	CenN=cn;
	CenE=ce;
	R=r;
	startAngle=startangle;
	endAngle=endangle;
}

void BCCreateXLEntity::SetARC_A(double cn, double ce, double r, 
								double startangle, double endangle,double a1,double a2)
{
	OK1=true;

	CenN=cn;
	CenE=ce;
	R=r;
	startAngle=startangle;
	endAngle=endangle;
	A1 = a1;
	A2 = a2;
}

void BCCreateXLEntity::UpdateData(int mode)
{
	if(OK1)
	{
		//圆心与圆弧起终点连线的方位角
		double fwj1,fwj2;
		fwj1=FwjtoAngX(startAngle);
		fwj2=FwjtoAngX(endAngle);

		//求圆弧的起终点坐标
		N1=CenN+R*cos(fwj1);
		E1=CenE+R*sin(fwj1);

		N2=CenN+R*cos(fwj2);
		E2=CenE+R*sin(fwj2);

		//两条边的方位角
		fwj1-=0.5*PI;
		fwj2-=0.5*PI;

		//求线路起终点坐标/即第一个和最后一个交点的坐标
		JL1=JL2=5.0;

		JdN1=N1-JL1*cos(fwj1);
		JdE1=E1-JL1*sin(fwj1);

		JdN2=N2+JL2*cos(fwj2);
		JdE2=E2+JL2*sin(fwj2);

		//求中间一个交点的坐标
		INTERS(&JdN0,&JdE0,JdN1,JdE1,N1,E1,JdN2,JdE2,N2,E2);


		JdCenter *jdcen=new JdCenter[3];
		int i=0;
		jdcen[i].JDXZ=IsJiaoDian;
		jdcen[i].N=JdN1;
		jdcen[i].E=JdE1;
		jdcen[i].R=0.0;
		jdcen[i].A1=0.0;
		jdcen[i].A2=0.0;
		jdcen[i].IsOrNotHuiTou=false;
		jdcen[i].LinkAfter=NotRelation;


		i=1;
		jdcen[i].JDXZ=IsJiaoDian;
		jdcen[i].IsOrNotHuiTou=false;
		jdcen[i].LinkAfter=NotRelation;
		jdcen[i].N=JdN0;
		jdcen[i].E=JdE0;
		jdcen[i].R=R;
		jdcen[i].A1=A1;
		jdcen[i].A2=A2;

		i=2;
		jdcen[i].JDXZ=IsJiaoDian;
		jdcen[i].N=JdN2;
		jdcen[i].E=JdE2;	
		jdcen[i].R=0.0;
		jdcen[i].A1=0.0;
		jdcen[i].A2=0.0;
		jdcen[i].IsOrNotHuiTou=false;
		jdcen[i].LinkAfter=NotRelation;

		JD_CENTER *JdCenObj=new JD_CENTER;
		JdCenObj->SetJdCenterArray(jdcen,3);
		pGtzx = JdCenObj;
		if(mode)
			eId = BAS_DRAW_FUN::AddEntityToDbs(JdCenObj);
		delete []jdcen;
	}
}
