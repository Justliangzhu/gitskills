// BCTPR.cpp: implementation of the BCTPR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCTPR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BCTPR::BCTPR()
{
	OK1=false;
	OK2=false;
	OK3=false;
	pArc=NULL;
	Len = 0.0;
}

BCTPR::~BCTPR()
{
}

void BCTPR::EraseArc()
{
	if(pArc && !pArc->isErased())
	{
		if(eId && acdbOpenObject(pArc,eId,AcDb::kForWrite)==Acad::eOk)
		{
			pArc->erase();
			pArc->close();
		}

	}
}

//��������
void BCTPR::SetQX(double n1,double e1,double n2,double e2)
{
	N1=n1; E1=e1;
	N2=n2; E2=e2;
	OK1=true;
}

//���û���һ��
void BCTPR::SetPointOnArc(double n,double e)
{
	NonArc=n;
	EonArc=e;
	OK2=true;
}

//���ð뾶
void BCTPR::SetRadius(double r)
{
	R=r;
	OK3=true;
}

//����
void BCTPR::UpdateData()
{
	if(OK1 && OK2 && OK3)
	{
		double tmpN,tmpE,tmpdist;
		tmpN=NonArc;
		tmpE=EonArc;
		double dist;//����һ�㵽���ߵľ���
		dist=DistToLine(N1,E1,N2,E2,tmpN,tmpE,tmpdist);
		double QXFwj;//���߷�λ��
		xyddaa(N1,E1,N2,E2,&QXFwj);
		double side;//�����ߵ��Ĳ�
		side=GetSide(N1,E1,N2,E2,NonArc,EonArc);
		double ProjDist;//���ϵ��Բ���������ϵ�ͶӰ����
		ProjDist=sqrt( R*R - (R-dist)*(R-dist) );
		//����ת��
		double afa;
		afa=2.0*atan(dist/ProjDist);
		//��Բ��
		tmpN-=ProjDist*cos(QXFwj);
		tmpE-=ProjDist*sin(QXFwj);

		if(side>0.0)//���
		{			
			startAngle=FwjtoAngX(QXFwj)-0.5*PI;
			StdJiao(startAngle);
			endAngle=startAngle+afa;
			StdJiao(endAngle);
			afa*=-1.0;
			CenN=tmpN+R*cos(QXFwj-0.5*PI);
			CenE=tmpE+R*sin(QXFwj-0.5*PI);
		}
		else
		{
			endAngle=FwjtoAngX(QXFwj)+0.5*PI;
			StdJiao(endAngle);
			startAngle=endAngle-afa;
			CenN=tmpN+R*cos(QXFwj+0.5*PI);
			CenE=tmpE+R*sin(QXFwj+0.5*PI);
		}

		//�޸�ͼ��
		if(pArc==NULL)
		{
			pArc=new AcDbArc(AcGePoint3d(CenE,CenN,0.0),R,startAngle,endAngle);
			eId=BAS_DRAW_FUN::AddEntityToDbs(pArc);
			Len = R*fabs(endAngle-startAngle);
		}
		else
		{
			/*acdbOpenObject(pArc,eId,AcDb::kForWrite);*/
			if(acdbOpenObject(pArc,eId,AcDb::kForWrite)!=Acad::eOk)	
			{
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
				return;
			}
			pArc->setCenter(AcGePoint3d(CenE,CenN,0.0));
			pArc->setRadius(R);
			pArc->setStartAngle(startAngle);
			pArc->setEndAngle(endAngle);
			pArc->close();
			Len = R*fabs(endAngle-startAngle);
		}
	}
}
