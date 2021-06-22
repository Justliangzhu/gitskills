// BCARC.cpp: implementation of the BCARC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCARC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BCARC::BCARC()
{
	OK1=false;
	OK2=false;
	OK3=false;
	pArc=NULL;
	Len = 0.0;
}

BCARC::~BCARC()
{
}
void BCARC::EraseArc()
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
//���û����ĵ�����
void BCARC::SetMidPoint(double n0,double e0)
{
	N0=n0;	E0=e0;
	OK1=true;
}

//���ð뾶
void BCARC::SetRadius(double r)
{
	R=r;
	OK2=true;
}

//���û��е���������һ������
void BCARC::SetOutPoint(double outn,double oute)
{
	OutN=outn;	OutE=oute;
	OK3=true;
}

//����
void BCARC::UpdateData()
{
	if(OK1&&OK2&&OK3)//��������
	{
		double QXFwj;//���߷�λ��
		double CenToMidFwj;//Բ�ĵ������ĵķ�λ��
		if(xyddaa(N0,E0,OutN,OutE,&QXFwj)<0.001)return;
		CenToMidFwj=QXFwj+0.5*PI;
		CenN=N0-R*cos(CenToMidFwj);
		CenE=E0-R*sin(CenToMidFwj);
		startAngle=CenToMidFwj+0.25*PI;
		endAngle=CenToMidFwj-0.25*PI;
		startAngle=FwjtoAngX(startAngle);
		endAngle=FwjtoAngX(endAngle);
		//�޸�ͼ��
		if(pArc==NULL)
		{
			pArc=new AcDbArc(AcGePoint3d(CenE,CenN,0.0),R,startAngle,endAngle);
			eId=BAS_DRAW_FUN::AddEntityToDbs(pArc);
			Len = R*fabs(endAngle - startAngle);
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
			Len = R*fabs(endAngle - startAngle);
		}
	}
}

