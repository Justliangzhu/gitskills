// BCTPP.cpp: implementation of the BCTPP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCTPP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "XLDataBase.h"
BCTPP::BCTPP()
{
	OK1=false;
	OK2=false;
	OK3=false;
	pArc=NULL;
	Len = 0.0;

	//��ȡ��Ƴ���
	/*
	if(!ReadWorkdir())
	return ;	
	XLDataBase xlmdb;
	CString prj_fname;
	prj_fname.Format(L"%s\\data\\%s.mdb",Cworkdir,Pro);
	xlmdb.Read_XLDbs(prj_fname,"��Ҫ������׼");
	DV = xlmdb.JSBZ.SDMB;*/
	//    ReadGuifanFile();
	//	DV = mGuiFanCS.SuDuMuBiao;	
}

BCTPP::~BCTPP()
{
}

void BCTPP::EraseArc()
{
	if(pArc)
	{
		if(eId && acdbOpenObject(pArc,eId,AcDb::kForWrite)==Acad::eOk)
		{
			pArc->erase();
			pArc->close();
		}

	}
}

//��������
void BCTPP::SetQX(double n1,double e1,double n2,double e2)
{
	N1=n1; E1=e1;
	N2=n2; E2=e2;
	OK1=true;
}

//���û���һ��
void BCTPP::SetFirstPointOnArc(double n,double e)
{
	NonArc1=n;
	EonArc1=e;
	OK2=true;	
}

//���û���һ��
void BCTPP::SetSecondPointOnArc(double n,double e)
{
	NonArc2=n;
	EonArc2=e;
	OK3=true;	
}
void BCTPP::UpdateData()//plh07.19
{
	if(OK1 && OK2 && OK3)
	{
		//�������������յ�
		double distToPT2,TN,TE;
		TN=NonArc1;
		TE=EonArc1;
		if(DistToLine(N1,E1,N2,E2,TN,TE,distToPT2)<0.00001)
		{
			//			acutPrintf(L"��һ������������!\n");
			return;
		}
		N1=TN;
		E1=TE;

		TN=NonArc2;
		TE=EonArc2;
		if(DistToLine(N1,E1,N2,E2,TN,TE,distToPT2)<0.00001)
		{
			//			acutPrintf(L"�ڶ�������������!\n");
			return;
		}
		N2=TN;
		E2=TE;

		double f;
		if(xyddaa(N1,E1,N2,E2,&f)<0.0001)
		{
			return;
		}
		double side1,side2;
		side1=GetSide(N1,E1,N2,E2,NonArc1,EonArc1);
		side2=GetSide(N1,E1,N2,E2,NonArc2,EonArc2);
		if(side1*side2<=0)return;
		//Բ�����������������ߵĽ���1
		double JdN1,JdE1;
		INTERS(&JdN1,&JdE1,N1,E1,N2,E2,NonArc1,EonArc1,NonArc2,EonArc2);
		//�������㹹�ɵ��ҵ��е�
		double N0,E0;
		N0=0.5*(NonArc1+NonArc2);
		E0=0.5*(EonArc1+EonArc2);
		//����1�����ڡ����е�ĶԳƵ㡡����2
		double JdN2,JdE2;
		JdN2=2.0*N0-JdN1;
		JdE2=2.0*E0-JdE1;
		//����1������2�ķ�λ��
		double FwjJd1ToJd2;
		xyddaa(JdN1,JdE1,JdN2,JdE2,&FwjJd1ToJd2);
		//���д�������һ��
		double PerpN,PerpE;
		PerpN=N0+100.0*cos(FwjJd1ToJd2+0.5*PI);
		PerpE=E0+100.0*sin(FwjJd1ToJd2+0.5*PI);
		//�м佻������
		double JdN0,JdE0;
		INTERS(&JdN0,&JdE0,N1,E1,N2,E2,N0,E0,PerpN,PerpE);
		//��뾶
		AcDbLine L1(AcGePoint3d(E1,N1,0.0),AcGePoint3d(E2,N2,0.0));
		AcDbLine L2(AcGePoint3d(JdE0,JdN0,0.0),AcGePoint3d(JdE2,JdN2,0.0));
		R=radiusfree(&L1,&L2,AcGePoint3d(EonArc1,NonArc1,0.0));
		if(R<0.1)return;
		//	R=get_R(R,DV);
		if(R<0.1)return;

		double JdToCenterFwj;//�м佻�㵽Բ�ĵķ�λ��
		double j1,j2;
		xyddaa(JdN1,JdE1,JdN0,JdE0,&j1);
		xyddaa(JdN0,JdE0,JdN2,JdE2,&j2);
		//����ת
		double aa=j2-j1;
		if(aa<-PI)aa+=2.0*PI;
		if(aa>PI)aa-=2.0*PI;

		if(aa<0.0)//��ת
		{
			startAngle=j1+0.5*PI;
			endAngle=j2+0.5*PI;
			JdToCenterFwj=j1+0.5*aa-0.5*PI;
		}
		else
		{
			startAngle=j2-0.5*PI;
			endAngle=j1-0.5*PI;
			JdToCenterFwj=j1+0.5*aa+0.5*PI;
		}		
		startAngle=FwjtoAngX(startAngle);
		endAngle=FwjtoAngX(endAngle);

		StdJiao(JdToCenterFwj);
		/*
		if(JdToCenterFwj>PI)JdToCenterFwj=2.0*PI-JdToCenterFwj;
		*/

		double afa;//Բ�Ľǵ�һ��
		afa=0.5*(PI-fabs(aa));

		CenN=JdN0+R/sin(afa)*cos(JdToCenterFwj);
		CenE=JdE0+R/sin(afa)*sin(JdToCenterFwj);

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

		//��ȡ�����Բ�������Ĺ���һ����

		AcDbLine Line(AcGePoint3d(EonArc1,NonArc1,0.0),
			AcGePoint3d(EonArc1+10.0*sin(f),NonArc1+10.0*cos(f),0.0));
		AcGePoint3dArray interPt;
		if(Acad::eOk==pArc->intersectWith(&Line,AcDb::kExtendArg,interPt)
			&& interPt.length()>0)
		{
			double dn,de;
			dn=interPt[0].y-NonArc1;
			de=interPt[0].x-EonArc1;
			if(sqrt(dn*dn+de*de)>0.00001)
			{
				/*acdbOpenObject(pArc,eId,AcDb::kForWrite);*/
				if(acdbOpenObject(pArc,eId,AcDb::kForWrite)!=Acad::eOk)	
				{
					ads_printf(L"��ʵ��ʧ�ܣ�\n");
					return;
				}
				CenN-=dn;
				CenE-=de;
				pArc->setCenter(AcGePoint3d(CenE,CenN,0.0));
				pArc->close();
				Len = R*fabs(endAngle-startAngle);
			}
		}		
	}
}

