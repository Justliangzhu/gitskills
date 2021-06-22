// GTZX2.cpp: implementation of the GTZX2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTZX2.h"
#include "PmPointer.h"
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//#include "Cmymonitor.h"

GTZX_ROAD *pzLinepTemp;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//��֪��������
GTZX2_ROAD::GTZX2_ROAD(GTZX_ROAD *pm1,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[])
	//һ�����ݣ����߽����������߽���ںţ���ʼ������
{
	//��ʼ��
	isReFresh=true;
	int i=0,j;
	for (i=0;i<100;i++)
	{   
		for ( j=0;j<6;j++) JDarray[i][j]=0.0;
		for (  j=0;j<9;j++)   QXB[i][j]=0.0;
		//  for (  j=0;j<3;j++)   QXLRL[i][j]=0.0;
		JD2[i].JDXZ = 0;//����
		JD2[i].jdno1 = 0;//��Ӧһ�߽������
		JD2[i].jdno = 0;// ���߽����
		JD2[i].xjj= 0.0;
		JD2[i].x= 0.0;
		JD2[i].y= 0.0;

		JD2[i].R = 0.0;
		JD2[i].Lo1=JD2[i].Lo2=0.0;
		JD2[i].NYDL=0.0;
		JD2[i].qxjk=0.0;	   
	};
	NJD=0; 
	for (i=0;i<30;i++) {   DLB[i].DL=0.0; DLB[i].BLC=0.0;
	_tcscpy(DLB[i].BGH,L"\0"); _tcscpy(DLB[i].EGH,L"\0");		
	};

	//��ֵ	
	m_pm1=pm1;//һ��ƽ��
	texth=m_pm1->texth;
	GetT0min_Lrmin();

	NJD = njd2;
	JD0 = jdo;
	NLC = 2;
	_tcscpy(JDGH,jdgh);
	for(i=0; i<NJD; i++)
	{
		JD2[i].JDXZ = jd2[i].JDXZ;//����......
		JD2[i].jdno1 = jd2[i].jdno1/*-pm1->JD0*/;//��Ӧһ�߽������(���ǽ����)
		JD2[i].jdno = JD0+i;// ���߽����
		JD2[i].xjj =  jd2[i].xjj;	
		JD2[i].x =  jd2[i].x;	
		JD2[i].y =  jd2[i].y;	
		JD2[i].R =  jd2[i].R;	
		JD2[i].Lo1 =  jd2[i].Lo1;	
		JD2[i].Lo2 =  jd2[i].Lo2;	
		JD2[i].NYDL =  jd2[i].NYDL;	
		JD2[i].qxjk =  jd2[i].qxjk;	
		//		ads_printf(L"%d jdxz=%d\n",i,JD2[i].JDXZ);
	}
	draw_zybz=m_pm1->draw_zybz;
	if(JD2[0].xjj>0)//����Ϊ����
		draw_zybz=1;//����Ҫ�ر������
	else
		draw_zybz=-1;//����Ҫ�ر����Ҳ�
	qxbbz=1;
	for(i=0;i<300;i++)
	{
		DLB2x[i].no=-1;
		DLB2x[i].TYLC=0.0;
		DLB2x[i].BGH="";
		DLB2x[i].EGH="";
		DLB2x[i].BLC=0.0;
		DLB2x[i].ELC=0.0;		
	}
	NDL=0;
	ModifyOriExJDXY();
	CalJDdata1();
	SetJDdata();
	getpxy_DATA();
	CalNYDL();	
	m_LJorGM = 0;
	m_Xsw = 1;
	m_DesHXSW = 2;
	m_BPDArray.RemoveAll();
	m_NCM = 0;
	_tcscpy(m_XLRelate.CurXLNme, L"");
	_tcscpy(m_XLRelate.InXLName, L"");
	_tcscpy(m_XLRelate.OutXLName, L"");
	m_XLRelate.EXLDCnum = 0;
	m_XLRelate.SXLDCnum = 0;
	m_XLRelate.InterXLSum = 0;
}

GTZX2_ROAD::GTZX2_ROAD(GTZX_ROAD *pm1,int njd2,BAS_DRAW_FUN::DoubleLineData jd2[],int jdo,ACHAR jdgh[])
	//һ�����ݣ����߽����������߽���ںţ���ʼ������
{
	//��ʼ��
	isReFresh=true;
	int i=0,j;
	for (i=0;i<100;i++)
	{   
		for ( j=0;j<6;j++) JDarray[i][j]=0.0;
		for (  j=0;j<9;j++)   QXB[i][j]=0.0;
		//  for (  j=0;j<3;j++)   QXLRL[i][j]=0.0;
		JD2[i].JDXZ = 0;//����
		JD2[i].jdno1 = 0;//��Ӧһ�߽������
		JD2[i].jdno = 0;// ���߽����
		JD2[i].xjj= 0.0;
		JD2[i].x= 0.0;
		JD2[i].y= 0.0;

		JD2[i].R = 0.0;
		JD2[i].Lo1=JD2[i].Lo2=0.0;
		JD2[i].NYDL=0.0;
		JD2[i].qxjk=0.0;	   
	};
	NJD=0; 
	for (i=0;i<30;i++) {   DLB[i].DL=0.0; DLB[i].BLC=0.0;
	_tcscpy(DLB[i].BGH,L"\0"); _tcscpy(DLB[i].EGH,L"\0");		
	};

	//��ֵ	
	m_pm1=pm1;//һ��ƽ��
	texth=m_pm1->texth;
	GetT0min_Lrmin();

	NJD = njd2;
	JD0 = jdo;
	NLC = 2;
	_tcscpy(JDGH,jdgh);
	for(i=0; i<NJD; i++)
	{
		JD2[i].JDXZ = jd2[i].JDXZ;//����......
		JD2[i].jdno1 = jd2[i].jdno1/*-pm1->JD0*/;//��Ӧһ�߽������(���ǽ����)
		JD2[i].jdno = JD0+i;// ���߽����
		JD2[i].xjj =  jd2[i].xjj;	
		JD2[i].x =  jd2[i].x;	
		JD2[i].y =  jd2[i].y;	
		JD2[i].R =  jd2[i].R;	
		JD2[i].Lo1 =  jd2[i].Lo1;	
		JD2[i].Lo2 =  jd2[i].Lo2;	
		JD2[i].NYDL =  jd2[i].NYDL;	
		JD2[i].qxjk =  jd2[i].qxjk;	
		//		ads_printf(L"%d jdxz=%d\n",i,JD2[i].JDXZ);
	}

	std::vector< DoubleLineData> watchvec1, watchvec2, watchvec3, watchvec4, watchvec5;
	for (int i = 0; i < NJD; i++)
		watchvec1.push_back(JD2[i]);

	draw_zybz=m_pm1->draw_zybz;
	if(JD2[0].xjj>0)//����Ϊ����
		draw_zybz=1;//����Ҫ�ر������
	else
		draw_zybz=-1;//����Ҫ�ر����Ҳ�
	qxbbz=1;
	for (i = 0; i < MAXDLNUM2x; i++)
	{
		DLB2x[i].no = -1;
		DLB2x[i].TYLC = 0.0;
		DLB2x[i].BGH = L"";
		DLB2x[i].EGH = L"";
		DLB2x[i].BLC = 0.0;
		DLB2x[i].ELC = 0.0;
	}
	NDL=0;
	//ModifyOriExJDXY();
	for (int i = 0; i < NJD; i++)
		watchvec2.push_back(JD2[i]);


	CalJDdata1();
	for (int i = 0; i < NJD; i++)
		watchvec3.push_back(JD2[i]);

	SetJDdata();

	for (int i = 0; i < NJD; i++)
		watchvec4.push_back(JD2[i]);

	getpxy_DATA();

	for (int i = 0; i < NJD; i++)
		watchvec5.push_back(JD2[i]);
	CalNYDL();	
	m_LJorGM = 0;
	m_Xsw = 1;
	m_DesHXSW = 2;
	m_BPDArray.RemoveAll();
	m_NCM = 0;
	_tcscpy(m_XLRelate.CurXLNme, L"");
	_tcscpy(m_XLRelate.InXLName, L"");
	_tcscpy(m_XLRelate.OutXLName, L"");
	m_XLRelate.EXLDCnum = 0;
	m_XLRelate.SXLDCnum = 0;
	m_XLRelate.InterXLSum = 0;
}

GTZX2_ROAD::GTZX2_ROAD()
{
	//m_pm1=NULL;
	isReFresh=true;
	for(int i=0;i< MAXDLNUM2x;i++)
	{
		DLB2x[i].no=-1;
		DLB2x[i].TYLC=0.0;
		DLB2x[i].BGH="";
		DLB2x[i].EGH="";
		DLB2x[i].BLC=0.0;
		DLB2x[i].ELC=0.0;		
	}
	NDL=0;
	CalJDdata();
	SetJDdata();
	getpxy_DATA();
	CalNYDL();  
	GetDLTYLC();
	m_LJorGM = 0;
	m_Xsw = 1;
	m_DesHXSW = 2;
	m_BPDArray.RemoveAll();
	m_NCM = 0;
	_tcscpy(m_XLRelate.CurXLNme, L"");
	_tcscpy(m_XLRelate.InXLName, L"");
	_tcscpy(m_XLRelate.OutXLName, L"");
	m_XLRelate.EXLDCnum = 0;
	m_XLRelate.SXLDCnum = 0;
	m_XLRelate.InterXLSum = 0;
}

GTZX2_ROAD::~GTZX2_ROAD()
{

}

GTZX2_ROAD::GTZX2_ROAD(AcDbObjectId Id1x, double xjj)//plh04.30
	//xjj=-1,��������,xjj=1,��������
{
	isReFresh=true;
	AcDbObject *pObj;
	/*acdbOpenObject(pObj,Id1x,AcDb::kForRead);*/
	if(acdbOpenObject(pObj,Id1x, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	m_pm1 = GTZX_ROAD::cast(pObj);
	m_pm1->close();
	NJD=m_pm1->NJD;
	JD0=m_pm1->JD0;

	for(int i=0;i<NJD;i++)
	{
		JD2[i].JDXZ=B_X;
		JD2[i].jdno=i;
		JD2[i].jdno1=i;//��Ӧһ�߽������;
		JD2[i].xjj=xjj;
	}
	//���ս�����һ���ϵ�ͶӰ���//plh05.06
	ACHAR *lcstr=LCchr(m_pm1->DLB[0].EGH,
		m_pm1->DLB[0].BLC +	m_pm1->DLB[0].DL,3,1);
	_tcscpy(JD2[0].ZHLC,lcstr);
	lcstr=LCchr(m_pm1->DLB[m_pm1->NDL-1].EGH,
		m_pm1->DLB[m_pm1->NDL-1].BLC + m_pm1->DLB[m_pm1->NDL-1].DL,3,1);
	_tcscpy(JD2[NJD-1].ZHLC,lcstr);
	CalJDdata();//��������߲��н�������
	//plh05.10
	SetJDdata();
	getpxy_DATA();
	Create2XDLB();//plh05.10
	m_LJorGM = 0;
	m_Xsw = 1;
	m_DesHXSW = 2;
	m_BPDArray.RemoveAll();
	m_NCM = 0;
	_tcscpy(m_XLRelate.CurXLNme, L"");
	_tcscpy(m_XLRelate.InXLName, L"");
	_tcscpy(m_XLRelate.OutXLName, L"");
	m_XLRelate.EXLDCnum = 0;
	m_XLRelate.SXLDCnum = 0;
	m_XLRelate.InterXLSum = 0;
}

GTZX2_ROAD::GTZX2_ROAD(GTZX_ROAD *gtzx, double xjj)//plh04.30
	//xjj=-1,��������,xjj=1,��������
{
	isReFresh=true;
	m_pm1=gtzx;
	NJD=m_pm1->NJD;
	JD0=m_pm1->JD0;

	int i;
	for( i=0;i<NJD;i++)
	{
		JD2[i].JDXZ=B_X;
		JD2[i].jdno=i;
		JD2[i].jdno1=i;//��Ӧһ�߽������;
		JD2[i].xjj=xjj;
	}
	//���ս�����һ���ϵ�ͶӰ���//plh05.06
	ACHAR *lcstr=LCchr(m_pm1->DLB[0].EGH,m_pm1->DLB[0].BLC +	m_pm1->DLB[0].DL,3,1);
	_tcscpy(JD2[0].ZHLC,lcstr);
	//	lcstr=LCchr(m_pm1->DLB[m_pm1->NDL-1].EGH,m_pm1->DLB[m_pm1->NDL-1].BLC + m_pm1->DLB[m_pm1->NDL-1].DL,3,1);
	lcstr=LCchr(m_pm1->DLB[m_pm1->NDL-1].BGH,m_pm1->DLB[m_pm1->NDL-1].BLC,3,1);//2005-11-15
	_tcscpy(JD2[NJD-1].ZHLC,lcstr);
	for(i=0;i<MAXDLNUM2x;i++)
	{
		DLB2x[i].no=-1;
		DLB2x[i].TYLC=0.0;
		DLB2x[i].BGH="";
		DLB2x[i].EGH="";
		DLB2x[i].BLC=0.0;
		DLB2x[i].ELC=0.0;
	}
	NDL=0;
	CalJDdata();
	SetJDdata();
	getpxy_DATA();
	CalNYDL();
	m_LJorGM = 0;
	m_Xsw = 1;
	m_DesHXSW = 2;
	m_BPDArray.RemoveAll();
	m_NCM = 0;
	_tcscpy(m_XLRelate.CurXLNme, L"");
	_tcscpy(m_XLRelate.InXLName, L"");
	_tcscpy(m_XLRelate.OutXLName, L"");
	m_XLRelate.EXLDCnum = 0;
	m_XLRelate.SXLDCnum = 0;
	m_XLRelate.InterXLSum = 0;
}

//plh
//double GTZX2::T0min=mGuiFanCS.MinZhi;
//double GTZX2::Lrmin=mGuiFanCS.MinYuan;//��СԲ���߳�

void GTZX2_ROAD::GetT0min_Lrmin()//plh
{
	// 	acutPrintf(L"DV=%lf\n",m_pm1->DV);
	if(m_pm1==NULL){T0min=-1000.0;Lrmin=-1000.0;}
	else
	{	
		BAS_DRAW_FUN_RAIL::GetT0min_Lrmin(m_pm1->DV,T0min,Lrmin,mGuiFanCS);
	}
}

void GTZX2_ROAD::INIT()
{

}
void GTZX2_ROAD::UpdateData(bool IsTheFirst)
{
	assertWriteEnabled();
	if(JD2[0].xjj>0)//����Ϊ����
		draw_zybz=1;//����Ҫ�ر������
	else
		draw_zybz=-1;//����Ҫ�ر����Ҳ�
	if (!IsTheFirst)
	{
		ModifyOriExJDXY();//���ɶ��߽�������
		CalJDdata1();
	}
	else
	{
		CalJDdata();
	}
	SetJDdata();
	getpxy_DATA();//���ɶ�����Ԫ����
	Create2XDLB();
	CalNYDL();	//������ҵ����1
}

double GTZX2_ROAD::Lh_Rtop(double Lh,double R)
{
	double AA=sqrt(R*Lh);
	double y=spiral_x(AA,Lh);
	double x=spiral_y(AA,Lh);
	double t=Lh/(2*R);
	double dr=x+R*sin(t+0.5*pi);	
	return dr-R;
}

//�ɰ뾶R������Lh,��ֱ�߳���ֱ�߱�࣬�����߳�T,Afa
double GTZX2_ROAD::GetAfaFromR_Lh_T0_dBJ(double R,double Lh,double T0,double dBJ,double &T)
{
	if(R<=0){AfxMessageBox(L"�뾶С�ڻ����0!");return 0;}
	double Afa=0.0;
	double A,B,C,p,m,Seta;
	m=0.5*Lh-Lh*Lh*Lh/(240*R*R);
	p=Lh*Lh/(24*R)-Lh*Lh*Lh*Lh/(2688*R*R*R);

	A=2*m+T0;
	B=2*(R+p);
	C=dBJ-2*(R+p);
	double K=sqrt(A*A+B*B);
	if(K==0)return 0;

	if(B/K<-1)Seta=pi;
	else if(B/K>1)Seta=0.0;
	else Seta=acos(B/K);//

	double TempJ;
	if(-1*C/K<-1)TempJ=pi;
	else if(-1*C/K>1)TempJ=0.0;
	else TempJ=acos(-1*C/K);//

	Afa=TempJ-Seta;
	T=m+(R+p)*tan(0.5*Afa);
	return Afa;
}

//�ɰ뾶R0��Բ���߳�Lr������L0�����߳�T
double GTZX2_ROAD::CalTFromR_Lr_Lh(double R,double Lr,double Lh)
{
	double bata0,m,p,afa,T;
	bata0=0.5*Lh/R;
	afa=Lr/R+2.0*bata0;
	m=0.5*Lh-Lh*Lh*Lh/(240*R*R);
	p=Lh*Lh/(24*R)-Lh*Lh*Lh*Lh/(2688*R*R*R);
	T=m+(R+p)*tan(0.5*afa);
	return T;
}

//�ɰ뾶R0��Բ���߳�Lr������L0��ֱ�߱��dbj,�����߳���ֱ�߳����г�
double GTZX2_ROAD::CalT0andLjiaFromR_Lr_Lh_d(double R,double Lr,double Lh,double dbj,double &T0)
{
	double bata0,m,p,afa;
	bata0=0.5*Lh/R;
	afa=Lr/R+2.0*bata0;
	m=0.5*Lh-Lh*Lh*Lh/(240*R*R);
	p=Lh*Lh/(24*R)-Lh*Lh*Lh*Lh/(2688*R*R*R);
	T0=m+(R+p)*tan(0.5*afa);	
	return dbj/cos(afa)-2.0*T0;
}

Adesk::Boolean GTZX2_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{
	//acedAlert(L"3");	
	AcGePoint3d Pt,*verts=new AcGePoint3d[NJD];
	AcGeVector3d normal(0.0,0.0,1.0);
	int i=0,j=0,k=0;
	double INDATA[5],LCB;
	//	ModifyOriExJDXY();//pxb 2005-11-17���¼��㶼Ӧ�����������
	//	CalJDdata();//2005.2.1
	//	CalJDdata1();
	//
	//	TagEXJDIsShow();
	//	SetJDdata();
	//	getpxy_DATA();
	//	GetDLTYLC();
	//	CalNYDL();
	xlpoint PB,PE;
	if (IsdrawLC)//�����
	{
		BZRXD(pWd);
		BZXJJ(pWd);
	}

	

	// �潻������
	if (IsdrawJDLX)
	{
		for (i = 0; i < NJD; i++)
		{
			verts[i].x=JDarray[i][2];
			verts[i].y=JDarray[i][1];
			verts[i].z=0.0;
		}
		if (pWd!= NULL) 
		{
			pWd->subEntityTraits().setSelectionMarker(1);
			pWd->subEntityTraits().setColor(7);	
			pWd->geometry().polyline(NJD,verts);
		} 
		else
		{
			assert(Adesk::kFalse);//?
		}
		// �潻�㴦Բ
		for (i = 1; i < NJD-1; i++) //plh05.07
		{
			if(!JD2[i].ISShow)
				continue;
			Pt[X]=verts[i].x;
			Pt[Y]=verts[i].y;
			//		verts[i].z=0.0;
			pWd->subEntityTraits().setColor(7);	
			pWd->geometry().circle(Pt,3.6,normal);			
		}
	}



	if (IsdrawJD)
	{
		BZJD(pWd,JDarray,QXB,NJD,JD0,JDGH);
	}



	// ����λ
	PB.x=array_pxy[0][0][1]; //N
	PB.y=array_pxy[0][0][2]; // E
	PB.a=rad(array_pxy[0][0][3]);// FA
	PB.lc=array_pxy[0][0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for (i =1; i < NJD; i++) 
	{
		for (k =0; k <4; k++) 	
		{
			for (j=0; j <5; j++)
			{
				INDATA[j]=array_pxy[i][k][j]; 
			}
			G_WXY(pWd,INDATA,&PB,&PE,TRUE); 
			// GUXWXY(INDATA,&PB,&PE,1); 
			PB=PE;
		}
	}

	return (pWd->regenType() == kAcGiSaveWorldDrawForProxy);

	LCB=array_pxy[0][0][4];  // ������
	if (IsdrawTZD)
	{
		BZTZD(pWd,draw_zybz,array,tarray,true);//��ע���������
	}


	if (verts != NULL)
	{
		delete [] verts;
		verts = NULL;
	}

	Create2XGTDLB();
	Create2XGTBPDArray();
	if (IsdrawBPD)
	{
		BZCM(pWd);//��ע���µ�
	}


	ads_printf(m_XLRelate.CurXLNme);
	ads_printf(L"%d\n",m_XLRelate.InterXLSum);
	ads_printf(L"%d\n",m_NeedInterXLNum);
	CheckAllXLConstraint();
	DrawXLConstraint(pWd);
	if (isReFresh)
	{
		RefreshRelateXL();
	}
	isReFresh=true;
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}

void GTZX2_ROAD::CalJDdata(BOOL full,int qdi,int zdi)
{
	double lo,lo1,D;
	int QDi,ZDi;
	if (!full)
	{
		QDi = 0;
		ZDi = NJD;
	}
	else
	{
		QDi = qdi; 
		ZDi = zdi;
	}
	for(int i=QDi; i<ZDi; i++)
	{
		if(JD2[i].JDXZ==B_X)//���жν���
		{
			JDarray[i][0] = JD0 + i;
			int jdn1 = JD2[i].jdno1;//��Ӧһ�߽������
			double xjj = JD2[i].xjj;
			double EXJD[2];
			CalEXJD(jdn1,i,EXJD);
			JD2[i].x = EXJD[0];
			JD2[i].y = EXJD[1];//��������߽���
			double R = m_pm1->JDarray[jdn1][4];
			double JK;
			JK = get_XJJK(m_pm1->DV,R,mGuiFanCS);//�߼��ӿ�
			JD2[i].qxjk = JK;
			JD2[i].Lo1 = m_pm1->JDarray[jdn1][3];
			JD2[i].R = R;
			JD2[i].Lo2 = m_pm1->JDarray[jdn1][5];           
			lo1 = m_pm1->JDarray[jdn1][3];//һ�߻���
			D = fabs(xjj);
			//���߰뾶
			if(i>0&&i<NJD-1)
			{
				double xjj2;
				xjj2 = JD2[i-1].xjj;//��һ�����߼��
				if(fabs(xjj-xjj2)<0.001)//ǰ���߼�����
				{
					//xjj�������߼ӿ�............		   
					if(xjj>0)//һ��Ϊ����
					{
						if(m_pm1->QXB[jdn1][8]<0)//��ת,II������࣬���ٻ���
						{
							//���仺��
							lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//���ȱ� 2003.12.4�ṩ
							JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
							D = -1.0*D;
						}
						else
						{
							//���仺��
							lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
							JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
						}
					}
					else//һ��Ϊ����
					{
						if(m_pm1->QXB[jdn1][8]<0)//��ת,II�����ڲ�
						{
							lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
							JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
						}
						else
						{   //���仺��
							lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//�� 2003.12.4�ṩ
							JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
							D  = -1.0*D;
						}
					}
					//��뾶
					double B = R - D + (lo1*lo1)/24/R;
					JD2[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//���߰뾶				
				}
			}
			JD2[i].jdno = JD0 + i;
		}//����//�Զ����ö��ߵĳ�ʼ��ʱ��ȫ�����ǲ��У���Ӧ�������³���
		else if(JD2[i].JDXZ==S_StartPoint)//ֱ�߱�����
		{
			CalS(JD2[i].R,JD2[i].Lo1,T0min,JD2[i].xjj,JD2[i+1].xjj,JD2[i].ZHLC,JD2[i].x,JD2[i].y,JD2[i+1].x,JD2[i+1].y);
			JD2[i].jdno = JD0 + i;		   
		}
		else if(JD2[i].JDXZ==S_EndPoint)//ֱ�߱�����
		{
			JD2[i].jdno = JD0 + i;		   
		}
		else //���л��в�ƽ��
		{
			JD2[i].jdno = JD0 + i;		   
		}
	} 
}

double GTZX2_ROAD::CalS(double R,double Lh,double T0,double xjj1,double xjj2,ACHAR ZHLC[80],double &x1,double &y1,double &x2,double &y2)
	//�뾶����������ֱ�߳����߼��1���߼��2��JD1xy��JD2xy,����Բ���߳�
{
	if(m_pm1==NULL)return -9999;
	double T;//�г�
	double afa=GetAfaFromR_Lh_T0_dBJ(R,Lh,T0,fabs(xjj2-xjj1),T);
	double xzh,yzh;//������x,y,������߷�λ��
	xlpoint Pt;
	Pt.lc=TYLC1(ZHLC,m_pm1->NDL,m_pm1->DLB);
	xlpoint_pz(m_pm1->array,m_pm1->tarray,&Pt);

	xzh=Pt.x+xjj1*cos(Pt.a+0.5*pi);
	yzh=Pt.y+xjj1*sin(Pt.a+0.5*pi);
	x1=xzh+T*cos(Pt.a);
	y1=yzh+T*sin(Pt.a);
	double flag=1.0;
	if(xjj2<xjj1)flag=-1.0;
	double dd;
	dd=(2*T+T0min);
	x2=x1+dd*cos(Pt.a+flag*afa);
	y2=y1+dd*sin(Pt.a+flag*afa);
	return afa*R-Lh;
}

void GTZX2_ROAD::CalJDdata1()
{
	for(int i=0; i<NJD; i++)
	{
		if(JD2[i].JDXZ==B_X)//���жν���
		{
			JD2[i].jdno = JD0 + i;
			JDarray[i][0] = JD0 + i;
			int jdn1 = JD2[i].jdno1;//��Ӧһ�߽������
			if(JD2[i].ISXJJFirst)
			{
				if(i==NJD-1)
					JD2[i].xjj = JD2[i-1].xjj;//���һ������߼�����ǰһ�����
				double xjj = JD2[i].xjj;
				//ads_printf(L"i=%d xjj=%lf\n",i,xjj);
				double EXJD[2];
				CalEXJD(jdn1,i,EXJD);
				JD2[i].x = EXJD[0];
				JD2[i].y = EXJD[1];//��������߽���
			}
		}//����
		else if(JD2[i].JDXZ==S_StartPoint)//ֱ�߱�����
		{
			if(JD2[i].ISXJJFirst)
			{
				//CalS(JD2[i].R,JD2[i].Lo1,T0min,JD2[i].xjj,JD2[i+1].xjj,JD2[i].ZHLC,JD2[i].x,JD2[i].y,JD2[i+1].x,JD2[i+1].y);
			}
			JD2[i].jdno = JD0 + i;
		}
		else if(JD2[i].JDXZ==S_EndPoint)//ֱ�߱�����
		{
			JD2[i].jdno = JD0 + i;
		}
		else //����
		{
			JD2[i].jdno = JD0 + i;
		}
	}	   
}

Adesk::Boolean  GTZX2_ROAD::G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,bool ISShow)
{     
	int j;
	double l = 0.0;
	AcGePoint3d ptb, pt, pte;
	if (INDATA[1] < 0.00000001) return 1;
	l = Get_INDATA_L(INDATA, &j);
	// ads_printf(L"\n  ����=%d l=%10.3f" ,j,l );
	ptb[X] = P0->y; ptb[Y] = P0->x; ptb[Z] = 0.0;
	// ads_printf(L"\n PO=%10.3f %10.3f %10.3f %10.3f",P0->y,P0->x,dms_rad(P0->a),P0->lc) ;
	if ((j <= 3 && j >= 6 && INDATA[1] <= 0.000000000000001) || (l <= 0.00000000001)) 
		return 1;

	if (j > 2 && j <= 6)
	{ //( ������)
		if (ISShow)G_drawspi(pWd, INDATA, l, P0, PZ, 2.0);
		xlzbxy(P0, PZ, l, INDATA, 0);// ĳ��Ԫ�յ�
	}
	else if (j <= 2)  // R L
	{//( ԰�� ֱ��)
		xlzbxy(P0, PZ, l * 0.5, INDATA, 0); // ĳ��Ԫ�е�
		pt[X] = PZ->y; pt[Y] = PZ->x; pt[Z] = 0.0;
		xlzbxy(P0, PZ, l, INDATA, 0); // ĳ��Ԫ�յ�
		pte[X] = PZ->y; pte[Y] = PZ->x; pte[Z] = 0.0; //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
		if (j == 1) //ֱ��
		{
			AcGePoint3d pcArr[2];
			pcArr[0] = ptb; pcArr[1] = pte;
			if (sqrt((P0->x - PZ->x) * (P0->x - PZ->x) + (P0->y - PZ->y) * (P0->y - PZ->y)) >= T0min)
			{
				pWd->subEntityTraits().setColor(7);
			}
			else
			{
				pWd->subEntityTraits().setColor(1);
			}
			if (ISShow)pWd->geometry().polyline(2, pcArr);
			// ads_printf(L"\n L ����= 1"  );
		}
		else if (j == 2) // ԰�� 
		{
			pWd->subEntityTraits().setColor(7);
			if (ISShow)pWd->geometry().circularArc(ptb, pt, pte);
			// ads_printf(L"\n r ����= 2"  );
		};
	};  //J
	// ads_printf(L"\n PZ=%10.3f %10.3f %10.3f %10.3f",PZ->y,PZ->x,dms_rad(PZ->a),PZ->lc) ;
	return 0;
}

//	��ע����� �����߲���
void GTZX2_ROAD::BZJD(AcGiWorldDraw* pWd,double arrayd[100][6],double qxb[100][9],int njd,int JD0,ACHAR *JDchr)
{  double fb,fe,de,da,aa,dd,dt ;
AcGePoint3d  p1; 
ACHAR ch[10],ch1[60],chrtmp[80];
int i,c,n;
de=xyddaa(arrayd[0][1],arrayd[0][2],arrayd[1][1],arrayd[1][2],&fb);
for (i=1;i<njd-1;i++)
{  
	if (arrayd[i][4]<=0) break;
	de=xyddaa(arrayd[i][1],arrayd[i][2],arrayd[i+1][1],arrayd[i+1][2],&fe);
	aa= fe - fb ;
	if (aa<-pi) aa=aa+2*pi;
	if  (aa>pi) aa=aa-2*pi;
	if (aa>0) {  c=1; dd=2*texth;  }  //��ƫ
	else { c=-1; dd= 2*texth; }; //��ƫ
	de=fb+aa*0.5;

	ads_rtos(JD0+i,2,0,ch);
	_tcscpy(ch1,JDchr);
	_tcscat(ch1,ch); 
	n=_tcslen(ch1);
	p1[Z]=0.0;

	p1[X]=arrayd[i][2]-n*0.5*texth*sin(de)+dd*sin(de+c*pi*0.5);
	p1[Y]=arrayd[i][1]-n*0.5*texth*cos(de)+dd*cos(de+c*pi*0.5);

	da= 0.5*pi-de;
	if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,7,1);    // �����  
	// maketext( p1,ch1,de,texth ,4,1);  
	//	 if(JD2[i].jdno1==0||JD2[i].jdno1==m_pm1->NJD-1)continue;
	if( qxbbz==1)
	{
		// �����߲��� begin
		double R,T,T1,T2,L1,L2,LL,NYDL;
		R=arrayd[i][4]; L1= arrayd[i][3];  L2=arrayd[i][5]; 
		T1=qxb[i][5]; T2=qxb[i][6];  LL=qxb[i][7];
		T=(T1+T2)*0.5-(L1+L2)*0.25;

		dd=sqrt(R*R+T*T)-R;
		// ads_printf(L"\n dd=%8.2f",dd);
		if (draw_zybz==0) 
		{  dd=dd+16*texth; dt=c*texth*1.5;
		if (c==-1) dd=dd+8*texth;
		}
		else {   dd=dd-draw_zybz*16*texth; dt=c*texth*1.5; 
		if(draw_zybz==-1) dd=dd-8*c*texth;
		}
		// ads_printf(L"\ dd=%8.2f",dd);
		p1[X]=arrayd[i][2]-dd*sin(de-c*pi*0.5) ; 
		p1[Y]=arrayd[i][1]-dd*cos(de-c*pi*0.5);
		_tcscpy(chrtmp,L"  "); _tcscat(chrtmp,ch1);
		if(JD2[i].ISShow)G_maketext(pWd,p1,chrtmp,da,texth ,7,1);   // �����

		ads_rtos(qxb[i][4],2,4,ch);
		if (c==1) { _tcscpy(ch1,L"ay--"); }
		else _tcscpy(ch1,L"az--"); 
		_tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,7,1);   //  ƫ��

		if (fabs(R-(long int)(R))<0.001)  { ads_rtos(R,2,0,ch); }
		else { ads_rtos(R,2,NLC,ch); };
		_tcscpy(ch1,L"R--"); _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,7,1);   //   �뾶

		if (L1>0||L2>0)  //  ������Ϊ��
		{
			if (fabs(L1-(long int)(L1))<0.001)  { ads_rtos(L1,2,NLC,ch); }
			else { ads_rtos(L1,2,NLC,ch); };
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if (fabs(L1-L2)<0.001) 
			{ _tcscpy(ch1,L"Lo--");  _tcscat(ch1,ch);
			if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,7,1);   //   ���� ���
			} else
			{ _tcscpy(ch1,L"L1--");  _tcscat(ch1,ch);
			if (fabs(L2-(long int)(L2))<0.001)  { ads_rtos(L2,2,NLC,ch); }
			else { ads_rtos(L2,2,NLC,ch); };
			_tcscat(ch1,L"  L2--"); _tcscat(ch1,ch);
			if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,7,1);   //   ���Ȼ���
			};
		};

		if (fabs(T1-(long int)(T1))<0.001)  { ads_rtos(T1,2,0,ch); }
		else { ads_rtos(T1,2,NLC,ch); };
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if (fabs(L1-L2)<0.001) 
		{   _tcscpy(ch1,L"T--");  _tcscat(ch1,ch);
		if(JD2[i].ISShow) G_maketext(pWd,p1,ch1,da,texth ,7,1);   //   �г� ���
		} else
		{   _tcscpy(ch1,L"T1--");  _tcscat(ch1,ch);
		ads_rtos(T2,2,NLC,ch);
		_tcscat(ch1,L"  T2--"); _tcscat(ch1,ch);
		if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,7,1);   //   �����г�
		};

		ads_rtos(LL,2,NLC,ch);
		_tcscpy(ch1,L"L--");  _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,7,1);   //  ���߳�
		//��ע��ҵ����
		if(JD2[i].JDXZ!=R_X)//���н���/S��
		{
			NYDL = JD2[i].NYDL;
			ads_rtos(NYDL,2,NLC,ch1);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,7,1);   // ��ҵ����
		}
		// �����߲��� end
	};
	fb=fe; 
};   
}

void GTZX2_ROAD::CalEXJD(int jdn1,int njd2,double EXJD[2])//����һ�߽���ţ����߶�Ӧ����ţ����ز��жζ��߽�������
{
	double xjj1,xjj2,fwj;

	if(jdn1<m_pm1->NJD-1)
		xyddaa(m_pm1->JDarray[jdn1][1],m_pm1->JDarray[jdn1][2],m_pm1->JDarray[jdn1+1][1],m_pm1->JDarray[jdn1+1][2],&fwj);
	else
		xyddaa(m_pm1->JDarray[jdn1-1][1],m_pm1->JDarray[jdn1-1][2],m_pm1->JDarray[jdn1][1],m_pm1->JDarray[jdn1][2],&fwj);

	if(njd2==0|| njd2==NJD-1)//��ʼ����//plh05.06
	{
		//�ɸõ���һ���ϵ�ͶӰ��̺��߼�����
		//Ҫ��GTZX2�����յ��ZHLCҪ��

		if(jdn1==0|| jdn1==m_pm1->NJD-1)
		{
			double xjj;
			xjj = JD2[njd2].xjj;
			EXJD[0]  =  m_pm1->JDarray[jdn1][1] + xjj * cos(fwj+0.5*pi);
			EXJD[1]  =  m_pm1->JDarray[jdn1][2] + xjj * sin(fwj+0.5*pi);//��������߽���
		}
		else
		{
			EXJD[0]=JD2[njd2].x;
			EXJD[1]=JD2[njd2].y;
		}
	}
	else 
	{
		double fwj;
		xjj1 = JD2[njd2-1].xjj;//njd2-1-njd2�ߵ��߼��
		xjj2 = JD2[njd2].xjj;//njd2-njd2+1�ߵ��߼��

		xyddaa(m_pm1->JDarray[jdn1-1][1],m_pm1->JDarray[jdn1-1][2],m_pm1->JDarray[jdn1][1],m_pm1->JDarray[jdn1][2],&fwj);
		double pt1[2],pt2[2];
		pt1[0] = m_pm1->JDarray[jdn1-1][1] + xjj1 * cos(fwj+0.5*pi);
		pt1[1] = m_pm1->JDarray[jdn1-1][2] + xjj1 * sin(fwj+0.5*pi);
		pt2[0] = m_pm1->JDarray[jdn1][1] + xjj1 * cos(fwj+0.5*pi);
		pt2[1] = m_pm1->JDarray[jdn1][2] + xjj1 * sin(fwj+0.5*pi);
		double pt3[2],pt4[2];
		xyddaa(m_pm1->JDarray[jdn1][1],m_pm1->JDarray[jdn1][2],m_pm1->JDarray[jdn1+1][1],m_pm1->JDarray[jdn1+1][2],&fwj);
		pt3[0] = m_pm1->JDarray[jdn1][1] + xjj2 * cos(fwj+0.5*pi);
		pt3[1] = m_pm1->JDarray[jdn1][2] + xjj2 * sin(fwj+0.5*pi);
		pt4[0] = m_pm1->JDarray[jdn1+1][1] + xjj2 * cos(fwj+0.5*pi);
		pt4[1] = m_pm1->JDarray[jdn1+1][2] + xjj2 * sin(fwj+0.5*pi);
		//�ҽ���

		INTERS(&EXJD[0],&EXJD[1],pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

	}
}
int GTZX2_ROAD::INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4)
{
	double A11,A12,A21,A22,B1,B2,D,D1,D2;
	int INF;
	double X1min,Y1min,X2min,Y2min,X1max,Y1max,X2max,Y2max;

	A11 = Y2-Y1;
	A12 = X1-X2;
	A21 = Y4-Y3;
	A22 = X3-X4;
	B1 = X1*Y2 - X2*Y1;
	B2 = X3*Y4 - X4*Y3;
	D = A11*A22-A21*A12;
	if(fabs(D)<1.0E-5)
		INF = -1;
	else
	{
		D1 = B1*A22-B2*A12;
		D2 = B2*A11-B1*A21;

		*x  = D1/D;
		*y  = D2/D;
		X1min=min(X1,X2)-0.00001;
		X1max=max(X1,X2)+0.00001;
		Y1min=min(Y1,Y2)-0.00001;
		Y1max=max(Y1,Y2)+0.00001;
		X2min=min(X3,X4)-0.00001;
		X2max=max(X3,X4)+0.00001;
		Y2min=min(Y3,Y4)-0.00001;
		Y2max=max(Y3,Y4)+0.00001;

		if(X1min<=*x && *x<=X1max && Y1min<=*y && *y<=Y1max
			&& X2min<=*x && *x<=X2max
			&& Y2min<=*y && *y<=Y2max)
			INF = 0;
		else
			INF = 1;
	}

	return INF; 
} 
//----------------------------------------------------------------------------------------------------------------------------- 
void  GTZX2_ROAD::CalNYDL()//plh05.14
{
	for(int i=0; i<NJD; i++)
	{
		if(JD2[i].JDXZ==B_X)//���жν���
		{
			//1.ȷ�����㷶Χ
			double ZHml2,HZml2;//�����ϵ�ֱ�����
			ZHml2 = QXB[i][0];
			HZml2 = QXB[i][3];
			double ZHpt2[2],HZpt2[2];
			xlpoint PZ;
			PZ.lc = ZHml2;
			xlpoint_pz(array,tarray,&PZ);
			ZHpt2[0] = PZ.x ,ZHpt2[1] = PZ.y;
			PZ.lc = HZml2;
			xlpoint_pz(array,tarray,&PZ);
			HZpt2[0] = PZ.x ,HZpt2[1] = PZ.y;
			double ZHml2XIn1X,HZml2XIn1X;
			ZHml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,ZHpt2[0],ZHpt2[1],ZHpt2[0],ZHpt2[1]);
			HZml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,HZpt2[0],HZpt2[1],HZpt2[0],HZpt2[1]);
			double ZHml1,HZml1;//1��ZH,HZ���
			ZHml1 = m_pm1->QXB[JD2[i].jdno1][0];
			HZml1 = m_pm1->QXB[JD2[i].jdno1][3];
			double sml,eml;
			sml = ZHml2XIn1X < ZHml1 ?  ZHml2XIn1X : ZHml1;
			eml = HZml2XIn1X > HZml1 ?  HZml2XIn1X : HZml1;
			//2.������һ�߳���
			double len1 = eml - sml;
			//3.�����ڶ��߳���
			double len2;
			double spt1[2],ept1[2];//1��sml,eml����
			double sml2,eml2,spt2[2],ept2[2];
			int LorR;

			PZ.lc = sml;
			xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);
			spt1[0] = PZ.x ,spt1[1] = PZ.y;//һ������
			LorR = Judge2XLorR(PZ,array,tarray);
			if(LorR==0)//������һ���غ�518
			{
				sml2 = PROJ_ML(array,tarray,spt1[0],spt1[1],spt1[0],spt1[1]);//ͶӰ��2��
				spt2[0]=spt1[0];
				spt2[1]=spt1[1];
			}
			else
			{
				sml2 = Search_EXPt(array,tarray,spt1,PZ.a+LorR*0.5*pi,spt2);//���߶�Ӧ���
			}

			PZ.lc = eml;
			xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);
			ept1[0] = PZ.x ,ept1[1] = PZ.y;//һ������
			LorR = Judge2XLorR(PZ,array,tarray);
			if(LorR==0)//������һ���غ�518
			{
				eml2 = PROJ_ML(array,tarray,ept1[0],ept1[1],ept1[0],ept1[1]);//ͶӰ��2��
				ept2[0]=ept1[0];
				ept2[1]=ept1[1];
			}
			else
			{
				eml2 = Search_EXPt(array,tarray,ept1,PZ.a+LorR*0.5*pi,ept2);//���߶�Ӧ���
			}

			len2 = eml2 - sml2;

			//		   acutPrintf(L"jd=%d,sml=%lf,eml=%lf,sml2=%lf,eml2=%lf,ept2x=%lf,ept2y=%lf\n",i,sml,eml,sml2,eml2,ept2[0],ept2[1]);
			//4.��ҵ����
			if(i>0&&i<NJD-1&&fabs(len2-len1)>0.01)//��
				JD2[i].NYDL = len2 - len1 + 100.0;
			else
				JD2[i].NYDL = 0.0;
		}//����
		else if(JD2[i].JDXZ==S_StartPoint||JD2[i].JDXZ==S_EndPoint||JD2[i].JDXZ==B_XNOTP_X)//S��
		{
			double ZHml2,HZml2;//�����ϵ�ֱ�����
			ZHml2 = QXB[i][0] , HZml2 = QXB[i][3];
			double ZHpt2[2],HZpt2[2];
			xlpoint PZ;
			PZ.lc = ZHml2;
			xlpoint_pz(array,tarray,&PZ);
			ZHpt2[0] = PZ.x ,ZHpt2[1] = PZ.y;
			PZ.lc = HZml2;
			xlpoint_pz(array,tarray,&PZ);
			HZpt2[0] = PZ.x ,HZpt2[1] = PZ.y;
			double ZHml2XIn1X,HZml2XIn1X;
			ZHml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,ZHpt2[0],ZHpt2[1],ZHpt2[0],ZHpt2[1]);
			HZml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,HZpt2[0],HZpt2[1],HZpt2[0],HZpt2[1]);
			JD2[i].NYDL=(HZml2-ZHml2)-(HZml2XIn1X-ZHml2XIn1X);
			if(fabs(JD2[i].NYDL)<0.01)
				JD2[i].NYDL=0.0;
			else
				JD2[i].NYDL+=100.0;
		}
		else
			JD2[i].NYDL=0.0;
	}
}

void GTZX2_ROAD::SetJDdata()//���ɶ���JDarray
{
	TagEXJDIsShow();
	for(int i=0; i<NJD; i++)
	{
		if(fabs(JD2[i].xjj)>0.5 )
		{
			if(JD2[i].xjj>0.0)
				LEFT_OR_RIGHT=RIGHTLINE;//�߼�����0����
			else
				LEFT_OR_RIGHT=LEFTLINE;//�߼��С��0����			
		}
		JDarray[i][0] = JD0 + i;//�����
		JDarray[i][1] = JD2[i].x; //x->N,
		JDarray[i][2] = JD2[i].y;//y->E
		JDarray[i][3] = JD2[i].Lo1;
		JDarray[i][4] = JD2[i].R;
		JDarray[i][5] = JD2[i].Lo2;	
		//	ads_printf(L"2:%d %lf %lf %lf %lf %lf %lf\n",i,JDarray[i][0],JDarray[i][1],JDarray[i][2],JDarray[i][3],JDarray[i][4],JDarray[i][5]);
	}   
}
// Files data out to a DWG file.
//
Acad::ErrorStatus GTZX2_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{ 
	assertReadEnabled();
	Acad::ErrorStatus es;
	if((es=AcDbEntity::dwgOutFields(pFiler))!=Acad::eOk)
	{
		return es;
	}

	//	pFiler->writeAddress(m_pm1);
	pzLinepTemp = m_pm1;
	pFiler->writeItem(NJD);
	pFiler->writeItem(NDL);
	pFiler->writeItem(NLC);
	pFiler->writeItem(NH);
	pFiler->writeItem(Chinese_Engbz_type );
	pFiler->writeItem(draw_zybz);
	pFiler->writeItem(JDTDBZ); 
	pFiler->writeItem(qxbbz); 
	pFiler->writeItem(JD0);	
	pFiler->writeItem(texth);

	int i;
	for(i=0;i<NJD;i++)
	{
		pFiler->writeItem(JD2[i].JDXZ);
		pFiler->writeItem(JD2[i].jdno);
		pFiler->writeDouble(JD2[i].x);
		pFiler->writeDouble(JD2[i].y);
		pFiler->writeDouble(JD2[i].Lo1);
		pFiler->writeDouble(JD2[i].R);
		pFiler->writeDouble(JD2[i].Lo2);
		pFiler->writeItem(JD2[i].jdno1);
		pFiler->writeDouble(JD2[i].xjj);
		pFiler->writeDouble(JD2[i].qxjk);
		pFiler->writeDouble(JD2[i].NYDL);
		const ACHAR *chr=NULL;
		chr=_tcsdup(JD2[i].ZHLC);  pFiler->writeItem(chr );
		//pFiler->writeString(JD2[i].ZHLC);
		pFiler->writeBool(JD2[i].ISXJJFirst);
	}
	for(i=0;i<NDL;i++)
	{
		pFiler->writeItem(DLB2x[i].no);
		pFiler->writeString((LPCTSTR)DLB2x[i].RXDName);
		pFiler->writeDouble(DLB2x[i].BLC);
		pFiler->writeDouble(DLB2x[i].ELC);
		pFiler->writeDouble(DLB2x[i].TYLC);
		const ACHAR *chr=NULL;
		chr=_tcsdup(DLB2x[i].BGH);  pFiler->writeItem(chr );
		chr=_tcsdup(DLB2x[i].EGH);  pFiler->writeItem(chr );

		//		pFiler->writeString(DLB2x[i].BGH);
		//		pFiler->writeString(DLB2x[i].EGH);
	}
	pFiler->writeItem(m_XLRelate.CurXLNme);
	pFiler->writeItem(m_XLRelate.InXLName);
	pFiler->writeItem(m_XLRelate.SXLDCnum);
	pFiler->writeItem(m_XLRelate.OutXLName);
	pFiler->writeItem(m_XLRelate.EXLDCnum);
	pFiler->writeItem(m_XLRelate.InterXLSum);
	for (int i=0;i<m_XLRelate.InterXLSum;i++)
	{
		pFiler->writeItem(m_XLRelate.InterXLNameArray[i].GCmin);
		pFiler->writeItem(m_XLRelate.InterXLNameArray[i].JJlowLimt);
		pFiler->writeItem(m_XLRelate.InterXLNameArray[i].InterXLName);
	}

	int nBPD=m_BPDArray.GetSize();
	const ACHAR *chr=NULL;
	pFiler->writeItem(nBPD);
	for (int j=0;j<nBPD;j++)
	{

		chr=_tcsdup(m_BPDArray[j].CKml );
		pFiler->writeItem(chr);
		pFiler->writeItem(m_BPDArray[j].cml);
		pFiler->writeItem(m_BPDArray[j].degree);
		pFiler->writeItem(m_BPDArray[j].GH);
		pFiler->writeItem(m_BPDArray[j].length);
		pFiler->writeItem(m_BPDArray[j].Level);
		pFiler->writeItem(m_BPDArray[j].ml);
		pFiler->writeItem(m_BPDArray[j].Notes);
		pFiler->writeItem(m_BPDArray[j].RLen);
		pFiler->writeItem(m_BPDArray[j].Rshu);
	}

	return pFiler->filerStatus();
}
// Files data in from a DWG file.
//
Acad::ErrorStatus GTZX2_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
	//���ܴ��̣���Ϊm_pm1Ϊָ�벻�ܱ���
{
	assertWriteEnabled();
	Acad::ErrorStatus es;
	if((es=AcDbEntity::dwgInFields(pFiler))!=Acad::eOk)
	{
		return es;
	}
	ACHAR *temp;
	//void *tpointer;
	//pFiler->readAddress(&tpointer);
	//m_pm1=(GTZX*)tpointer;

	m_pm1=pzLinepTemp;

	pFiler->readItem(&NJD);
	pFiler->readItem(&NDL);
	pFiler->readItem(&NLC);
	pFiler->readItem(&NH);
	pFiler->readItem(&Chinese_Engbz_type );
	pFiler->readItem(&draw_zybz); 
	pFiler->readItem(&JDTDBZ );
	pFiler->readItem(&qxbbz);
	pFiler->readItem(&JD0); 	
	pFiler->readItem(&texth);

	int i;
	for( i=0;i<NJD;i++)
	{
		pFiler->readItem(&JD2[i].JDXZ);
		pFiler->readItem(&JD2[i].jdno);
		pFiler->readDouble(&JD2[i].x);
		pFiler->readDouble(&JD2[i].y);
		pFiler->readDouble(&JD2[i].Lo1);
		pFiler->readDouble(&JD2[i].R);
		pFiler->readDouble(&JD2[i].Lo2);
		pFiler->readItem(&JD2[i].jdno1);
		pFiler->readDouble(&JD2[i].xjj);
		pFiler->readDouble(&JD2[i].qxjk);
		pFiler->readDouble(&JD2[i].NYDL);

		pFiler->readItem(&temp);
		_tcscpy(JD2[i].ZHLC,temp);		

		pFiler->readBool(&JD2[i].ISXJJFirst);
	}
	for(i=0;i<NDL;i++)
	{
		pFiler->readItem(&DLB2x[i].no);

		pFiler->readString(&temp);
		DLB2x[i].RXDName.Format(L"%s",temp);	

		pFiler->readDouble(&DLB2x[i].BLC);
		pFiler->readDouble(&DLB2x[i].ELC);
		pFiler->readDouble(&DLB2x[i].TYLC);

		pFiler->readString(&temp);
		DLB2x[i].BGH.Format(L"%s",temp);	

		pFiler->readString(&temp);
		DLB2x[i].EGH.Format(L"%s",temp);	
	}
	pFiler->readItem(&temp);
	_tcscpy(m_XLRelate.CurXLNme,temp);
	pFiler->readItem(&temp);
	_tcscpy(m_XLRelate.InXLName,temp);
	pFiler->readItem(&m_XLRelate.SXLDCnum);
	pFiler->readItem(&temp);
	_tcscpy(m_XLRelate.OutXLName,temp);
	pFiler->readItem(&m_XLRelate.EXLDCnum);
	pFiler->readItem(&m_XLRelate.InterXLSum);
	for (int i=0;i<m_XLRelate.InterXLSum;i++)
	{
		pFiler->readItem(&m_XLRelate.InterXLNameArray[i].GCmin);
		pFiler->readItem(&m_XLRelate.InterXLNameArray[i].JJlowLimt);
		pFiler->readItem(&temp);
		_tcscpy(m_XLRelate.InterXLNameArray[i].InterXLName,temp);
	}
	temp=NULL;
	int nBPD;
	//  const ACHAR *chr=NULL;
	pFiler->readItem(&nBPD);
	m_BPDArray.RemoveAll();
	m_BPDArray.SetSize(nBPD);
	for (int j=0;j<nBPD;j++)
	{
		pFiler->readItem(&temp);
		_tcscpy(m_BPDArray[j].CKml,temp);
		pFiler->readItem(&m_BPDArray[j].cml);
		pFiler->readItem(&m_BPDArray[j].degree);
		pFiler->readItem(&temp);
		m_BPDArray[j].GH.Format(L"%s",temp);
		pFiler->readItem(&m_BPDArray[j].length);
		pFiler->readItem(&m_BPDArray[j].Level);
		pFiler->readItem(&m_BPDArray[j].ml);
		pFiler->readItem(&temp);
		m_BPDArray[j].Notes.Format(L"%s",temp);
		pFiler->readItem(&m_BPDArray[j].RLen);
		pFiler->readItem(&m_BPDArray[j].Rshu);
	}
	return pFiler->filerStatus();
}

void GTZX2_ROAD::Create2XDLB()//���ɶ����������жεĶ�����
{
	//�������ʼ��
	for(int i=0;i<300;i++)
	{
		DLB2x[i].no=-1;
		DLB2x[i].TYLC=0.0;
		DLB2x[i].BGH="";
		DLB2x[i].EGH="";
		DLB2x[i].BLC=0.0;
		DLB2x[i].ELC=0.0;		
	}
	NDL=0;
	int RXDnum=GetRXDNum();//���ж���Ŀ
	for(int iRXD=0;iRXD<RXDnum;iRXD++)
		Create2XDLB(iRXD);
}

void GTZX2_ROAD::Create2XDLB(int iRXD,ACHAR EGH[])//���ɶ���ָ�����жεĶ�����
{
	int firstJDXH,endJDXH;//��β�������
	//��ȡָ�����ж���ʼ�������
	GetJDXHofiRXD(iRXD,firstJDXH,endJDXH);
	//	acutPrintf(L"firstJD=%d,endJD=%d\n",firstJDXH,endJDXH);

	//ָ�����жε�һ��ֱ���㣬���һ����ֱ���ͳһ���
	double StartZhTylc,EndHzTylc;

	StartZhTylc=QXB[firstJDXH][0];
	EndHzTylc  =QXB[endJDXH][3];
	//	acutPrintf(L"StartZhTylc=%lf,EndHzTylc=%lf\n",StartZhTylc,EndHzTylc);

	//ָ�����жε�һ��ֱ���㣬���һ����ֱ�������	
	double x1,y1,x2,y2;
	xlpoint sPt,ePt;

	sPt.lc=StartZhTylc;
	xlpoint_pz(array,tarray,&sPt);
	x1=sPt.x;y1=sPt.y;

	ePt.lc=EndHzTylc;
	xlpoint_pz(array,tarray,&ePt);
	x2=ePt.x;y2=ePt.y;
	//	acutPrintf(L"x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",x1,y1,x2,y2);

	//ָ�����жε�һ��ֱ����ģ����һ����ֱ�����һ���ϵ�ͶӰͳһ��̣�
	double stylc1x0,etylc1x0;
	//��������һ���ϵ�ͶӰͳһ��̣�
	double stylc1x,etylc1x;
	//�������յ��ͳһ��̣������ϡ�
	double stylc2x,etylc2x;
	//���ж����յ��Ӧ��һ�ߵĽ����
	int RXSJDIndex1x,RXEJDIndex1x;

	double xjj,resPt[2];
	ACHAR BGH1[20],EGH2[20];
	double BLC1,ELC2;
	EXDLB *ExDlb=new EXDLB[2];

	if (firstJDXH==0)//��һ������������
	{
		stylc2x = QXB[0][0];
		ExDlb[0].no=iRXD;
		ExDlb[0].TYLC=QXB[0][0];
		JudgePTLorR(JD2[0].x, JD2[0].y, xjj, m_pm1->array, m_pm1->tarray);
		if(EGH==NULL)//plh04.11.25
		{
			if(xjj>0)ExDlb[0].EGH.Format(L"Y%dK",iRXD+1);
			else ExDlb[0].EGH.Format(L"Z%dK",iRXD+1);
		}
		else
		{
			ExDlb[0].EGH.Format(L"%s",EGH);
		}
		ExDlb[0].ELC=QXB[0][0];

		stylc1x=PROJ_ML(m_pm1->array,m_pm1->tarray,x1,y1,x1,y1);
		if (stylc1x<0.0)//��������޷�ͶӰ��������
		{
			ExDlb[0].BGH=ExDlb[0].EGH;
			ExDlb[0].BLC=ExDlb[0].ELC;
		}
		else//����ͶӰ�������϶�ǰȡ�������
		{
			BLC1=XLC1(stylc1x,BGH1,m_pm1->NDL,m_pm1->DLB);//һ�����ֳ����
			ExDlb[0].BGH=BGH1;//���ɶ�����,����ǰ�Ϻ������ͬ
			ExDlb[0].BLC=BLC1;
		}
		ExDlb[0].RXDName.Format(L"%.1lf",ExDlb[0].BLC/1000.0);
	}

	else
	{
		//�״�ͶӰ��һ�ߵ�ͳһ���
		stylc1x0=PROJ_ML(m_pm1->array,m_pm1->tarray,x1,y1,x1,y1);
		//////////////////////////////////////////////////////////////////////////
		//�����ӳ�����������������.����Ѱ��ƽ�бߵķ���ȷ�����ж����յ��Ӧ��һ�ߵĽ���� ��ΰ2009-06-10
		RXSJDIndex1x = m_pm1->DistToNearJD(stylc1x0,false);
		AcGePoint2d Spt1x,Ept1x,Spt2x,Ept2x;
		AcGeLine2d Line2x,Line1x;
		AcGeTol Tol;
		Tol.setEqualVector(0.001);
		bool IsParallel = false;
		Spt2x.x = JD2[firstJDXH].y;
		Spt2x.y = JD2[firstJDXH].x;
		Ept2x.x = JD2[firstJDXH-1].y;
		Ept2x.y = JD2[firstJDXH-1].x;
		Line2x.set(Spt2x,Ept2x);
		for(int iJD1x=RXSJDIndex1x; iJD1x>0; iJD1x--)
		{
			Spt1x.x = m_pm1->JDarray[iJD1x][2];
			Spt1x.y = m_pm1->JDarray[iJD1x][1];
			Ept1x.x = m_pm1->JDarray[iJD1x-1][2];
			Ept1x.y = m_pm1->JDarray[iJD1x-1][1];
			Line1x.set(Spt1x,Ept1x);
			IsParallel = Line1x.isParallelTo(Line2x,Tol);
			if (IsParallel)
			{
				RXSJDIndex1x = iJD1x;
				break;
			}
		}

		stylc1x0 = min(stylc1x0,m_pm1->QXB[RXSJDIndex1x][0]);
		//////////////////////////////////////////////////////////////////////////
		//�״�ͶӰ��һ�ߵ�ͳһ���,����������������������ֱ����
		stylc1x=LocateHundredMeterOnLine(stylc1x0,true);//��ǰ����//һ�������

		//����ͶӰ��������//plh05.06
		CalXjj(stylc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,stylc2x,resPt);//plh05.06

		BLC1=XLC1(stylc1x,BGH1,m_pm1->NDL,m_pm1->DLB);//һ�����ֳ����

		ExDlb[0].BGH=BGH1;//���ɶ�����,����ǰ�Ϻ������ͬ
		ExDlb[0].no=iRXD;
		ExDlb[0].BLC=BLC1;
		ExDlb[0].TYLC=stylc2x;
		ExDlb[0].RXDName.Format(L"%.1lf",ExDlb[0].BLC/1000.0);
		//	acutPrintf(L"ExDlb[0].BLC=%lf,name=%s,%lf\n",ExDlb[0].BLC,ExDlb[0].RXDName,ExDlb[0].BLC/1000.0);
		if(EGH==NULL)//plh04.11.25
		{
			if(xjj>0)ExDlb[0].EGH.Format(L"Y%dK",iRXD+1);
			else ExDlb[0].EGH.Format(L"Z%dK",iRXD+1);
		}
		else
		{
			ExDlb[0].EGH.Format(L"%s",EGH);
		}
		ExDlb[0].ELC=ExDlb[0].BLC;
	}


	if (endJDXH==NJD-1)//���һ������������
	{

		etylc1x=PROJ_ML(m_pm1->array,m_pm1->tarray,x2,y2,x2,y2);
		etylc2x = QXB[NJD-1][3];
		ExDlb[1].no=iRXD;
		ExDlb[1].BGH=ExDlb[0].EGH;//���ɶ�����
		ExDlb[1].BLC=ExDlb[0].ELC+(etylc2x-stylc2x);//�յ��ǰ���Ϊ���Ϻ���̣��öγ���
		ExDlb[1].TYLC=etylc2x;
		if (etylc1x<0.0)//�޷�ͶӰ��������
		{
			ExDlb[1].EGH=ExDlb[1].BGH;
			ExDlb[1].ELC=ExDlb[1].BLC;
		}
		else
		{
			ELC2=XLC1(etylc1x,EGH2,m_pm1->NDL,m_pm1->DLB);//һ�����ֳ����
			ExDlb[1].EGH=EGH2;
			ExDlb[1].ELC=ELC2;
		}
	}

	else
	{
		//�״�ͶӰ��һ�ߵ�ͳһ���
		etylc1x0=PROJ_ML(m_pm1->array,m_pm1->tarray,x2,y2,x2,y2);
		//////////////////////////////////////////////////////////////////////////
		//�����ӳ�����������������.����Ѱ��ƽ�бߵķ���ȷ�����ж����յ��Ӧ��һ�ߵĽ���� ��ΰ2009-06-10
		RXEJDIndex1x = m_pm1->DistToNearJD(etylc1x0,true);
		AcGePoint2d Spt1x,Ept1x,Spt2x,Ept2x;
		AcGeLine2d Line2x,Line1x;
		AcGeTol Tol;
		Tol.setEqualVector(0.001);

		Spt2x.x = JD2[endJDXH].y;
		Spt2x.y = JD2[endJDXH].x;
		Ept2x.x = JD2[endJDXH+1].y;
		Ept2x.y = JD2[endJDXH+1].x;
		Line2x.set(Spt2x,Ept2x);
		bool IsParallel = false;
		for (int iJD1x=RXEJDIndex1x; iJD1x<m_pm1->NJD-1; iJD1x++)
		{
			Spt1x.x = m_pm1->JDarray[iJD1x][2];
			Spt1x.y = m_pm1->JDarray[iJD1x][1];
			Ept1x.x = m_pm1->JDarray[iJD1x+1][2];
			Ept1x.y = m_pm1->JDarray[iJD1x+1][1];
			Line1x.set(Spt1x,Ept1x);
			IsParallel = Line1x.isParallelTo(Line2x,Tol);
			if (IsParallel)
			{
				RXEJDIndex1x = iJD1x;
				break;
			}
		}
		etylc1x0 = max(etylc1x0,m_pm1->QXB[RXEJDIndex1x][3]);
		//////////////////////////////////////////////////////////////////////////
		//�״�ͶӰ��һ�ߵ�ͳһ���,����������������������ֱ����
		etylc1x=LocateHundredMeterOnLine(etylc1x0,false);//�������

		//����ͶӰ��������//plh05.06
		CalXjj(etylc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,etylc2x,resPt);//plh05.06

		ELC2=XLC1(etylc1x,EGH2,m_pm1->NDL,m_pm1->DLB);//һ�����ֳ����
		ExDlb[1].no=iRXD;
		ExDlb[1].BGH=ExDlb[0].EGH;//���ɶ�����
		ExDlb[1].BLC=ExDlb[0].ELC+(etylc2x-stylc2x);//�յ��ǰ���Ϊ���Ϻ���̣��öγ���
		ExDlb[1].TYLC=etylc2x;
		ExDlb[1].EGH=EGH2;
		ExDlb[1].ELC=ELC2;

	}

	//	acutPrintf(L"NDL=%d\n",NDL);
	Insert2XDLB(iRXD,ExDlb,2);
	//�����ж����յ������еĽ������ʸ�Ϊ����
	//1��S�����ս��㣬���в�ƽ�н��㡡�������ʸ�Ϊ����
	//2�����н��㣬�������ʸ�Ϊ���У��߼��Ϊǰһ������߼��(�Ӻ���ǰ��)
	ModifyJDXZ(iRXD);
	/*
	for(iRXD=0;iRXD<2;iRXD++)
	acutPrintf(L"start->no=%d,BGH=%s,BLC=%lf,EGH=%s,ELC=%lf\n"
	,ExDlb[iRXD].no,ExDlb[iRXD].BGH,ExDlb[iRXD].BLC,ExDlb[iRXD].EGH,ExDlb[iRXD].ELC);	
	*/
	if(ExDlb != NULL)
	{
		delete []ExDlb;
		ExDlb=NULL;
	}
}
void GTZX2_ROAD::ModifyJDXZ(int iRXD)
{
	//���ҳ������жε����ս���
	int firstJDXH,endJDXH;
	GetJDXHofiRXD(iRXD,firstJDXH,endJDXH);
	//���ҳ������жε����ն���
	int sdl,edl;
	GetiRXDDLinfo(iRXD,sdl,edl);
	//�������յ㴦��2��ͳһ���
	double stylc2x,etylc2x;
	stylc2x=DLB2x[sdl].TYLC;
	etylc2x=DLB2x[edl].TYLC;
	//��ǰ����λ�ڸö��ڵķ����н���
	int i;
	for(i=firstJDXH;i>=0;i--)
	{
		if(QXB[i][0]>stylc2x)
		{
			//�ҵ�һ������λ�ڸ����ж���

			if(JD2[i].JDXZ==B_X)//���н���
			{
				JD2[i].JDXZ=R_X;
				if(i>0)JD2[i].xjj=JD2[i-1].xjj;
			}
			else//S�佻�㣬���в�ƽ�н���
			{
				JD2[i].JDXZ=R_X;
			}
		}
	}
	//��������λ�ڸö��ڵķ����н���
	for(i=endJDXH;i<NJD;i++)
	{
		if(QXB[i][0]<etylc2x)
		{
			//�ҵ�һ������λ�ڸ����ж���
			if(JD2[i].JDXZ==B_X)//���н���
			{
				JD2[i].JDXZ=R_X;
				if(i<NJD-1)JD2[i].jdno1+=1;
			}
			else//S�佻�㣬���в�ƽ�н���
			{
				JD2[i].JDXZ=R_X;
			}
		}
	}
}

void GTZX2_ROAD::GetJDXHofiRXD(int iRXD, int &firstJDXH, int &endJDXH)//�õ�ָ�����жε����ս��㽻���
{
	int RXDnum=0;//���ж���Ŀ
	for(int i=0;i<NJD;i++)
	{
		if(JD2[i].JDXZ==R_X)
		{
			firstJDXH=i;
			while(i<NJD && JD2[i].JDXZ==R_X)
			{
				endJDXH=i;
				i++;
			}
			RXDnum++;
			if(iRXD==RXDnum-1)return;
		}
	}				
}

int GTZX2_ROAD::GetRXDNum()//�����ж���Ŀ
{
	int RXDnum=0;//���ж���Ŀ
	for(int i=0;i<NJD;i++)
	{
		if(JD2[i].JDXZ==R_X)
		{
			while(JD2[i].JDXZ==R_X)i++;
			RXDnum++;
		}
	}
	return RXDnum;
}

void GTZX2_ROAD::DeleteiRXDDLB(int iRXD)//ɾ��ĳ�����жζ�����
{
	//	assertWriteEnabled();
	int sDLno,eDLn0;
	GetiRXDDLinfo(iRXD,sDLno,eDLn0);
	//	acutPrintf(L"DeleteiRXDDLB(%d),sDLno=%d,eDLn0=%d\n",iRXD,sDLno,eDLn0);
	if(sDLno<0)return;
	int delDL=eDLn0-sDLno+1;//Ҫɾ���Ķ�����
	for(int i=sDLno;i<NDL-delDL;i++)
	{
		DLB2x[i]=DLB2x[i+delDL];
		DLB2x[i].no--;
	}
	NDL-=delDL;
}

void GTZX2_ROAD::GetiRXDDLinfo(int iRXD, int &sDLno, int &eDLno)
{
	sDLno=-1;
	eDLno=-1;
	for(int i=0;i<NDL;i++)
	{
		if(DLB2x[i].no==iRXD)
		{
			sDLno=i;
			while(i<NDL && DLB2x[i].no==iRXD)//plh05.26
			{
				eDLno=i;			
				i++;
			}
			break;
		}
	}
}

double GTZX2_ROAD::LocateHundredMeterOnLine(double tylc1x,bool IsForwardTo)
	//����ͳһ��̣��Ƿ���ǰѰ�Ҹ����������ױ꣬Ҫ����ֱ����,
{
	double gettylc=tylc1x;
	double lc0;//��׮
	int iXY=0;//����������Ԫ
	int XYClass;//��Ԫ����
	ACHAR GH[20];//��̹ں�
	double xlc;//һ����ͶӰ�ֳ����
	for(int i=m_pm1->tarray;i>0;i--)
	{
		if(tylc1x>=m_pm1->array[i][6])
		{
			iXY=i;
			break;
		}
	}
	XYClass=m_pm1->array[iXY][0];//ȡ����Ԫ����
	if(IsForwardTo)//�����ǰ����
	{
		if(XYClass==4)//�󻺣���ǰ�ƶ�һ����Ԫ
		{
			iXY--;
			XYClass=m_pm1->array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==2)//Բ����ǰ�ƶ�һ����Ԫ
		{
			iXY--;
			XYClass=m_pm1->array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==3)//ǰ������ǰ�ƶ�һ����Ԫ
		{
			iXY--;
		}
		//��ʱ��Ԫ��ֱ��
		gettylc=m_pm1->array[iXY+1][6];//һ��ֱ�������
		if(tylc1x<gettylc)gettylc=tylc1x;//�״ζ�λ��ֱ���ϣ�
		xlc=XLC1(gettylc,GH,m_pm1->NDL,m_pm1->DLB);
		lc0=xlc-100*floor(xlc/100+0.00001);
		if(lc0<m_pm1->array[iXY+1][6]-m_pm1->array[iXY][6]&&m_pm1->DistToNearDL(gettylc,true)>lc0)//�������Ը���Ԫ,�ҵ���ǰ��λ��֮��û�ж���
		{
			gettylc-=lc0;
		}
		else//ȡ��ʮ��
		{
			lc0=xlc-10*floor(xlc/10+0.00001);
			gettylc-=lc0;
		}
	}
	else
	{
		if(XYClass==3)//ǰ��������ƶ�һ����Ԫ
		{
			iXY++;
			XYClass=m_pm1->array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==2)//Բ������ƶ�һ����Ԫ
		{
			iXY++;
			XYClass=m_pm1->array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==4)//ǰ��������ƶ�һ����Ԫ
		{
			iXY++;
		}
		//��ʱ��Ԫ��ֱ��
		gettylc=m_pm1->array[iXY][6];//ֱ�����
		if(tylc1x>gettylc)gettylc=tylc1x;//�״ζ�λ��ֱ���ϣ�		
		xlc=XLC1(gettylc,GH,m_pm1->NDL,m_pm1->DLB);
		lc0=xlc-100*floor(xlc/100+0.00001);
		lc0=100.0-lc0;
		if(lc0<m_pm1->array[iXY+1][6]-m_pm1->array[iXY][6]&&fabs(m_pm1->DistToNearDL(gettylc,false))>lc0)//�������Ը���Ԫ
		{
			gettylc+=lc0;
		}
		else//ȡ��ʮ��
		{
			lc0=xlc-10*floor(xlc/10+0.00001);
			lc0=10.0-lc0;
			gettylc+=lc0;
		}		
	}
	return gettylc;
}

//��ע�߼��
void GTZX2_ROAD::BZXJJ(AcGiWorldDraw* pWd)
{
	int i;
	AcGePoint3d Pt1,Pt2,Pt0;
	double zhlc,hzlc,lc2x,lc1x,Tjiao;
	xlpoint PZ;
	ACHAR XJJ[20];

	//��עS���߼��
	for(i=0;i<NJD-1;i++)//���һ�����㲻��ע
	{
		if( (JD2[i].JDXZ==R_X&&JD2[i+1].JDXZ==R_X)
			|| (JD2[i].JDXZ==B_XNOTP_X&&JD2[i+1].JDXZ==B_XNOTP_X)
			|| (JD2[i].JDXZ==S_StartPoint&&JD2[i+1].JDXZ==S_EndPoint) )continue;
		hzlc=QXB[i][3];
		zhlc=QXB[i+1][0];
		lc2x=0.5*(zhlc+hzlc);
		if(JD2[i+1].JDXZ==S_StartPoint)lc2x=zhlc-10*texth;
		else if(JD2[i].JDXZ==S_EndPoint)lc2x=hzlc+10*texth;
		PZ.lc=lc2x;
		xlpoint_pz(array,tarray,&PZ);
		Tjiao=3.0*PI-PZ.a;
		Pt1.x=PZ.y;
		Pt1.y=PZ.x;
		//		if(i==0)acutPrintf(L"---------------------------------\n");
		lc1x=PROJ_ML(m_pm1->array,m_pm1->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		PZ.lc=lc1x;
		xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);
		Pt2.x=PZ.y;
		Pt2.y=PZ.x;

		if(JD2[i].JDXZ==R_X)_stprintf(XJJ,L"%.2lf",fabs(JD2[i+1].xjj));//plh05.06
		else _stprintf(XJJ,L"%.2lf",fabs(JD2[i].xjj));//plh05.06

		if(_wtof(XJJ)<0.01)continue;

		PZ.lc=lc2x-0.5*texth;
		xlpoint_pz(array,tarray,&PZ);

		Pt0.x=PZ.y+_tcslen(XJJ)*10.0*texth*sin(PZ.a+0.5*PI);
		Pt0.y=PZ.x+_tcslen(XJJ)*10.0*texth*cos(PZ.a+0.5*PI);
		G_maketext(pWd,Pt0,XJJ,Tjiao,10.0*texth,4,1);
		makeXJJFlag(pWd,Pt1,Pt2,80.0*texth);
	}
}

void GTZX2_ROAD::makeXJJFlag(AcGiWorldDraw* pWd,AcGePoint3d Pt1,AcGePoint3d Pt2,double len)
	//��ע�߼�ࣺ1 ����һ�㣬2 ����һ�㣬��ע�߳���=20mm*SCALE����������
{

	double jiao=fwj(Pt1.x,Pt1.y,Pt2.x,Pt2.y);
	AcGePoint3d Pt3,Pt4,Pt5,Pt6;

	getLineEndPt(Pt1,len,jiao+pi,Pt3);
	getLineEndPt(Pt1,0.3*len,jiao-0.1*pi+pi,Pt5);

	getLineEndPt(Pt2,len,jiao,Pt4);
	getLineEndPt(Pt2,0.3*len,jiao-0.1*pi,Pt6);

	G_makeline(pWd,Pt1,Pt3,2);	
	G_makeline(pWd,Pt1,Pt5,2);
	G_makeline(pWd,Pt2,Pt4,2);
	G_makeline(pWd,Pt2,Pt6,2);
}

void GTZX2_ROAD::BZRXD(AcGiWorldDraw* pWd)
{
	int RxdNum=GetRXDNum();//���ж���Ŀ
	//acutPrintf(L"����%d�����ж�!\n",RxdNum);
	for(int i=0;i<RxdNum;i++)
	{
		BZiRXDLC(pWd,i);
	}
}

void GTZX2_ROAD::BZiRXDLC(AcGiWorldDraw* pWd,int iRXD)//�����ע���ж�
{
	int sjd=0,ejd=0,njd=0,i=0,j=0,sdl=0,edl=0,ndl=0;
	EXDLB *iRxdDlb;
	GetiRXDDLinfo(iRXD,sdl,edl);//��ȡ���ն�����
	ndl=edl-sdl+1;
	//	acutPrintf(L"iRXD=%d,sdl=%d,edl=%d,ndl=%d\n",iRXD,sdl,edl,ndl);
	//	AfxMessageBox(L"BZiRXDLC1");
	iRxdDlb=new EXDLB[ndl];

	//	ACHAR   BGH[8];   �����Ķ�ǰ��̹ںźͶϺ���̹ں�	

	for(i=sdl;i<=edl;i++)
	{
		iRxdDlb[i-sdl]=DLB2x[i];
	}
	//	AfxMessageBox(L"BZiRXDLC2");
	xlpoint PZ1,PZ2;	//�������յ�������	
	PZ1.lc=iRxdDlb[0].TYLC;
	PZ2.lc=iRxdDlb[ndl-1].TYLC;
	xlpoint_pz(array,tarray,&PZ1);//������
	xlpoint_pz(array,tarray,&PZ2);
	GetJDXHofiRXD(iRXD,sjd,ejd);//��ȡ���ս����
	//	acutPrintf(L"sjd=%d,ejd=%d\n",sjd,ejd);

	njd=ejd-sjd+1;//������
	//�����������յ�
	njd += 2;
	if (sjd==0)
	{
		njd -= 1;
	}
	if (ejd==NJD-1)
	{
		njd -= 1;
	}
	double (*jday)[6];//��̬���ٽ�������
	jday=new double[njd][6];
	if (sjd!=0 && ejd!=NJD-1)
	{
		for(i=1;i<njd-1;i++)//1->njd-1
		{
			jday[i][0]=1.0*i;
			for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i-1][j];//sjd+i-1,��sjd��ʼ
		}
		//���ж������Ϊ��һ������
		i=0;
		jday[i][0]=i;
		jday[i][1]=PZ1.x;
		jday[i][2]=PZ1.y;
		jday[i][3]=0.0;
		jday[i][4]=0.0;
		jday[i][5]=0.0;
		//���ж��յ���Ϊ���һ������
		i=njd-1;
		jday[i][0]=i;
		jday[i][1]=PZ2.x;
		jday[i][2]=PZ2.y;
		jday[i][3]=0.0;
		jday[i][4]=0.0;
		jday[i][5]=0.0;
	}

	else if (sjd==0 && ejd!=NJD-1)
	{
		for(i=0;i<njd-1;i++)//1->njd-1
		{
			jday[i][0]=1.0*i;
			for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i][j];//sjd+i-1,��sjd��ʼ
		}
		jday[0][3] = 0.0;
		//���ж��յ���Ϊ���һ������
		i=njd-1;
		jday[i][0]=i;
		jday[i][1]=PZ2.x;
		jday[i][2]=PZ2.y;
		jday[i][3]=0.0;
		jday[i][4]=0.0;
		jday[i][5]=0.0;
	}

	else if (sjd!=0 && ejd==NJD-1)
	{
		for(i=1;i<njd;i++)//1->njd-1
		{
			jday[i][0]=1.0*i;
			for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i-1][j];//sjd+i-1,��sjd��ʼ
		}
		//���ж������Ϊ��һ������
		i=0;
		jday[i][0]=i;
		jday[i][1]=PZ1.x;
		jday[i][2]=PZ1.y;
		jday[i][3]=0.0;
		jday[i][4]=0.0;
		jday[i][5]=0.0;
	}

	else
	{
		for(i=0;i<njd;i++)//1->njd-1
		{
			jday[i][0]=1.0*i;
			for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i][j];//sjd+i-1,��sjd��ʼ
		}
		jday[0][3] = 0.0;
	}

	////��ע���
	double dlc;
	dlc=100.0;//����ױ�
	double (*qxb)[9];//��̬�������߱�
	qxb=new double[njd][9];
	double (*xyarray)[10];//��̬������Ԫ����
	xyarray=new double[4*njd][10];
	int tary=0;
	CalXYArrayFromJdArray(jday,njd,qxb,xyarray,tary);
	BZLC(pWd,dlc,-1,2,xyarray,tary,iRxdDlb,ndl);
	BZTZD(pWd,draw_zybz,xyarray,tary,false);//��ע���������
	//free
	delete []jday;	jday=NULL;
	delete []iRxdDlb;	iRxdDlb=NULL;
	delete []qxb;	qxb=NULL;
	delete []xyarray;	xyarray=NULL;
}

void GTZX2_ROAD::BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type,double (*xyarray)[10],int &tary,EXDLB *iExDlb,int ndl)  
	//  ��ע���������̱�  //    zybz=1,-1  �����ұߣ���� //���߶�����//������
{
	if (ndl<1)
	{
		return;
	}
	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,BZlc,lc,b,DDlength,DL;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[20],chr[50],ch[50];
	xlpoint  PZtmp;
	AcDbObjectId eId;
	_tcscpy(GH,iExDlb[0].EGH);
	WB=1.5*texth;
	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// ������ͳһ���
	DDlength=0.0;//iExDlb[0].TYLC;
	// ��������α�ע
	for (iDL=0;iDL<ndl-1;iDL++)//���һ������Ϊ�յ�
	{
		DL=iExDlb[iDL].ELC-iExDlb[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���
		_tcscpy(GH,iExDlb[iDL].EGH);
		_tcscpy(GH1,iExDlb[iDL].BGH);
		// ������� ��ںŸı��
		if (iDL>0 && iDL< ndl-1)  // ��Ϊ�� �յ�
		{ // ��� iDL��
			lc=DDlength;
			PZtmp.lc=lc;  // ͳһ���
			k=xlpoint_pz(xyarray,tary,&PZtmp);  
			if (k>0&& k<=6)  //�н�
			{
				B=PZtmp.a;
				pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
				pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
				ptb[X]=PZtmp.y;
				ptb[Y]=PZtmp.x; 
				G_makeline(pWd,ptb,pt,2);

				PZtmp.lc=lc-1.2*texth;
				k=xlpoint_pz(xyarray,tary,&PZtmp);  B=PZtmp.a;
				p1[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
				p1[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

				PZtmp.lc=lc+2.0*texth;
				k=xlpoint_pz(xyarray,tary,&PZtmp);  B=PZtmp.a;
				pte[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
				pte[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

				b=B+pi*0.5;
				b=pi*0.5-b;
				if(zybz==-1)b=b+pi;
				if(b<0.0)b=2*pi+b;
				if(b>2*pi) b=b-2*pi;		         
				if(type<=1)b=b-pi*0.5; 

				_tcscpy(ch,L"\0");
				_tcscpy(ch,LCchr(GH1,iExDlb[iDL].BLC,NLC,1));  // ��ǰ���
				_tcscpy(chr,L"  ");
				_tcscat(chr,ch);   
				G_maketext(pWd,p1,chr,b,texth,4,1);  

				_tcscpy(ch,L"\0");
				_tcscpy(ch,LCchr(GH,iExDlb[iDL].ELC,NLC,1));     // �Ϻ����
				_tcscpy(chr,L" =");
				_tcscat(chr,ch);   // e.f chr="ZH K10+123.45"   

				if (fabs( DL)>0.001)
				{ 
					if(DL>0.0)
					{
						_tcscat(chr, L" ���� "); 
						ads_rtos( DL,2,NLC,ch);
						_tcscat(chr, ch); 
						_tcscat(chr, L" m");
					}
					else if( DL<0.0)
					{
						_tcscat(chr, L" ���� "); 
						ads_rtos(fabs(DL),2,NLC,ch);
						_tcscat(chr, ch); 
						_tcscat(chr, L" m");
					}
					else
					{/*����ֵ����Ϊ0*/}
				}
				G_maketext(pWd,pte,chr,b,texth,4,1);		   
			};		   
		};//if (iDL>0 && iDL< ndl-1)  // ��Ϊ�� �յ�

		//���濪ʼ��ע���
		zlc=(long int)(iExDlb[iDL].ELC/dlc);
		BZlc=zlc*dlc;  // ʵ����� 
		if((BZlc-iExDlb[iDL].ELC)<0.0001)BZlc=BZlc+dlc;
		_tcscpy(GH,iExDlb[iDL].EGH);  //�ں�
		do   		
		{
			// ͳһ���
			lc=DDlength+BZlc-iExDlb[iDL].ELC; 
			PZtmp.lc=lc;  // ͳһ���

			k=xlpoint_pz(xyarray,tary,&PZtmp);   

			BZW=k;    //  K<0  �޽�
			if (k>0&& k<=6)  //�н�//��Ԫ����
			{
				// ads_printf(L"\n i=%d %10.3f %10.3f  %10.3f %10.3f ",iDL,PZtmp.lc, BZlc,LCB,iExDlb[iDL+1].BLC );  
				B=PZtmp.a;
				pt[X]=PZtmp.y+WB*sin(B+pi*0.5*zybz);
				pt[Y]=PZtmp.x+WB*cos(B+pi*0.5*zybz);
				pte[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
				pte[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
				G_makeline(pWd,ptb,pt,2);
				// makeline(ptb,pt,5);

				pt=pte;
				b=B+pi*0.5;
				b=pi*0.5-b; if (zybz==-1) b=b+pi;
				if (b<0.0) b=2*pi+b;
				if (b>2*pi) b=b-2*pi;

				// ads_printf(L"\n %10.3f %10.3f %10.3f %10.4f",PZtmp.lc,PZtmp.x,PZtmp.y,dms_rad(b));

				ads_rtos(BZlc,2,0,chr);  
				// _tcscpy(ch,GH); _tcscat(ch,chr);
				// ��̰��ױ�
				ii=_tcslen(chr);if(ii>20) ii=19;
				if(dlc>=100.0&&ii>=2)
				{
					chr[ii-1]='\0';chr[ii-2]='\0';
					_tcscpy(ch,chr);
					if (ii>3&&chr[ii-3]=='0') // �����
					{
						chr[ii-3]='\0';
						_tcscpy(ch,GH);
						_tcscat(ch,chr);
					}//pLCB[NLCB].lcflag=1000;
					else if(ii>3 && chr[ii-3]!='0')    // ���ױ�
					{ 
						ch[0]=chr[ii-3];
						ch[1]='\0';
					}		    
				}
				else
				{
					_tcscpy(ch,GH);
					if ( ii<=2)
					{
						_tcscpy(ch,L"+");
						ch[1]='0';
						ch[2]=chr[ii-2];
						ch[3]=chr[ii-1];
						ch[4]='\0';
					}
					else if ( ii>=3)
					{
						_tcscpy(chrtmp,L"+");
						chrtmp[1]=chr[ii-3];
						chrtmp[2]=chr[ii-2];
						chrtmp[3]=chr[ii-1];
						chrtmp[4]='\0';
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // �����
						{
							chr[ii-3]='\0';
							_tcscpy(ch,GH);
							_tcscat(ch,chr);
							_tcscat(ch,chrtmp); //pLCB[NLCB].lcflag=1000;
						}
						else if(ii>3&&chr[ii-3]!='0')
						{
							_tcscpy(ch,chrtmp);
						}
					}
				}
				if(type<=1)b=b-pi*0.5; 

				pte[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz)+0.5*texth*sin(B);
				pte[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz)+0.5*texth*cos(B);

				G_maketext(pWd,pte,ch,b,texth,4,1);
			};          
			BZlc=BZlc+dlc;
			if ( BZlc>iExDlb[iDL+1].BLC) BZW=-99;		 
		}while (BZW>0);
		DDlength=  DDlength+iExDlb[iDL+1].BLC-iExDlb[iDL].ELC;//������ͳһ��̡�
	}//for (iDL=0;iDL<ndl-1;iDL++)//���һ������Ϊ�յ� 

	//��ע���ж����
	int RxdNo=0;
	iDL=0;
	RxdNo=iExDlb[iDL].no;
	DL=iExDlb[iDL].ELC-iExDlb[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���
	lc=0.0;  // �ö�ͳһ���

	PZtmp.lc=lc;
	xlpoint_pz(xyarray,tary,&PZtmp);  
	B=PZtmp.a;
	pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
	pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
	ptb[X]=PZtmp.y;
	ptb[Y]=PZtmp.x; 
	G_makeline(pWd,ptb,pt,2);

	PZtmp.lc=lc;
	k=xlpoint_pz(xyarray,tary,&PZtmp);
	B=PZtmp.a;
	p1[X]=PZtmp.y-0.8*texth*sin(B);
	p1[Y]=PZtmp.x-0.8*texth*cos(B);

	pte[X]=p1[X]+2.4*texth*sin(B);
	pte[Y]=p1[Y]+2.4*texth*cos(B);

	b=B+pi*0.5;
	b=pi*0.5-b;
	if(zybz==-1)b=b+pi;
	if(b<0.0)b=2*pi+b;
	if(b>2*pi) b=b-2*pi;		         
	if(type<=1)b=b-pi*0.5; 

	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH1,iExDlb[0].BLC,NLC,1));  // ��ǰ���
	_tcscpy(chr,L"  ");
	_tcscat(chr,ch);
	//	_tcscat(chr,L"  ���ж����");
	G_maketext(pWd,p1,chr,b,texth,4,1);  

	ACHAR RXFlag[80];
	//plh04.11.26
	if(LEFT_OR_RIGHT==RIGHTLINE)_stprintf(RXFlag,L"%s ����������� ",iExDlb[0].RXDName);
	else _stprintf(RXFlag,L"%s ����������� ",iExDlb[0].RXDName);

	/*
	if(iExDlb[0].EGH.GetAt(0)=='Y')_stprintf(RXFlag,L"%s ����������� ",iExDlb[0].RXDName);
	else _stprintf(RXFlag,L"%s ����������� ",iExDlb[0].RXDName);
	*/
	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH,iExDlb[iDL].ELC,NLC,1));     // �Ϻ����
	_tcscpy(chr,L" = ");
	_tcscat(chr,RXFlag);
	_tcscat(chr,ch);   // e.f chr="ZH K10+123.45"   

	if (fabs( DL)>0.001)
	{ 
		if(DL>0.0)
		{
			_tcscat(chr, L" ���� "); 
			ads_rtos( DL,2,NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else if( DL<0.0)
		{
			_tcscat(chr, L" ���� "); 
			ads_rtos(fabs(DL),2,NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else
		{/*����ֵ����Ϊ0*/}
	}
	G_maketext(pWd,pte,chr,b,texth,4,1);
	//��ע���ж��յ�
	iDL=ndl-1;
	RxdNo=iExDlb[iDL].no;
	DL=iExDlb[iDL].ELC-iExDlb[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���
	lc=iExDlb[iDL].TYLC-iExDlb[0].TYLC;  // ͳһ���

	PZtmp.lc=lc;
	xlpoint_pz(xyarray,tary,&PZtmp);  
	B=PZtmp.a;
	pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
	pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
	ptb[X]=PZtmp.y;
	ptb[Y]=PZtmp.x; 
	G_makeline(pWd,ptb,pt,2);

	PZtmp.lc=lc;
	k=xlpoint_pz(xyarray,tary,&PZtmp);
	B=PZtmp.a;
	p1[X]=PZtmp.y-0.8*texth*sin(B);
	p1[Y]=PZtmp.x-0.8*texth*cos(B);

	pte[X]=p1[X]+2.4*texth*sin(B);
	pte[Y]=p1[Y]+2.4*texth*cos(B);

	b=B+pi*0.5;
	b=pi*0.5-b;
	if(zybz==-1)b=b+pi;
	if(b<0.0)b=2*pi+b;
	if(b>2*pi) b=b-2*pi;		         
	if(type<=1)b=b-pi*0.5; 

	//plh04.11.26
	if(LEFT_OR_RIGHT==RIGHTLINE)_stprintf(RXFlag,L"%s ���������յ� ",iExDlb[0].RXDName);
	else _stprintf(RXFlag,L"%s ���������յ� ",iExDlb[0].RXDName);

	/*
	if(iExDlb[0].EGH.GetAt(0)=='Y')_stprintf(RXFlag,L"%s ���������յ� ",iExDlb[0].RXDName);
	else _stprintf(RXFlag,L"%s ���������յ� ",iExDlb[0].RXDName);
	*/
	_tcscpy(ch,RXFlag);
	_tcscat(chr,L" ");
	_tcscat(ch,LCchr(GH,iExDlb[iDL].BLC,NLC,1));  // ��ǰ���
	_tcscpy(chr,L"  ");
	_tcscat(chr,ch);  
	G_maketext(pWd,p1,chr,b,texth,4,1);  

	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH1,iExDlb[iDL].ELC,NLC,1));     // �Ϻ����
	_tcscpy(chr,L" = ");
	_tcscat(chr,ch);// e.f chr="ZH K10+123.45"   

	if (fabs( DL)>0.001)
	{ 
		if(DL>0.0)
		{
			_tcscat(chr, L" ���� "); 
			ads_rtos( DL,2,NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else if( DL<0.0)
		{
			_tcscat(chr, L" ���� "); 
			ads_rtos(fabs(DL),2,NLC,ch);
			_tcscat(chr, ch); 
			_tcscat(chr, L" m");
		}
		else
		{/*����ֵ����Ϊ0*/}
	}
	G_maketext(pWd,pte,chr,b,texth,4,1);
}


// zybz==0   ������ƫ���ע   zybz=1,-1  �����ұߣ����
//xyxyarray->��Ԫ���飬nxy->��Ԫ��,�Ƿ����ע���к�S�佻��������
void GTZX2_ROAD::BZTZD(AcGiWorldDraw* pWd,int draw_zybz,double (*xyarray)[10],int nxy,bool IsOnlyB_S)//��ע�������������
{
	Chinese_Engbz_type=2;
	AcGePoint3d  ptb,pt,pte;
	double WB,lc, b,l,INDATA[5],L0;
	int i, j,k , KK=7,zybz=draw_zybz;
	ACHAR chr[50],ch[40],tch[6], GH[8],DGH[8];
	xlpoint  P0tmp,PZtmp;
	ptb[Z]=0.0;
	pt[Z]=0.0;
	pte[Z]=0.0;

	WB=4*texth;
	WB=WB+10*texth;
	P0tmp.lc=xyarray[0][4];
	P0tmp.x=xyarray[0][1];
	P0tmp.y=xyarray[0][2];
	P0tmp.a=rad(xyarray[0][3]);
	P0tmp.r=1e40;
	//��һ�������Ӧ���ߵ��ۼӳ��ȣ���������˵������Ϊ0��
	double BZFlag=true;//�Ƿ��ע
	int iJD;//��Ӧ�������
	double tylc0=0.0;
	bool ISShow=true;
	//������У�Ҫ���ϱ�������ڶ����ϵ�ͳһ���
	if(!IsOnlyB_S)tylc0=PROJ_ML(array,tarray,xyarray[0][1],xyarray[0][2],xyarray[0][1],xyarray[0][2]);
	P0tmp.lc+=tylc0;//��һ����Ԫ�����ȫ�ߵ�ͳһ���
	for (i=1;i<=nxy-1;i++)
	{
		ISShow=true;
		if(!IsOnlyB_S)BZFlag=true;//��������У���һ�����ϵͳ��ע����
		else//����ǲ��л�S�䣬����ע���в����Ӧ��������(�����жΰ��Լ������ϵͳ��)
		{
			if(xyarray[i][0]==1||xyarray[i][0]==4)iJD=DistToNearJD(xyarray[i][6],true);//ǰһ�����HZ/YH��
			else iJD=DistToNearJD(xyarray[i][6],false);
			if(!JD2[iJD].ISShow)ISShow=false;
			if(JD2[iJD].JDXZ!=R_X)BZFlag=true;//���л�S�佻��
			else BZFlag=false;//���н���
		}

		zybz=draw_zybz;
		for (k=0;k<5;k++)INDATA[k]=xyarray[i][k];
		j=(int)(xyarray[i][0]+0.01);
		l=0.0;
		if(j>=3)
		{
			// _tcscpy(chr,L"A=");
			_tcscpy(chr,L"Lo=");
			_tcscpy(tch,L".S."); 
			if(Chinese_Engbz_type==2)_tcscpy(tch,L"H");
		}
		if (j==1)
		{
			l=INDATA[1];
			_tcscpy(chr,L"L=");
			_tcscpy(tch,L".T.");
			if (Chinese_Engbz_type==2)_tcscpy(tch,L"Z");
		}
		if (j==2)
		{
			l=INDATA[2];
			_tcscpy(chr,L"R=");
			_tcscpy(tch,L".C.");
			if (Chinese_Engbz_type==2)_tcscpy(tch,L"Y");
		}		
		//  ��A=��
		if (j==3)l=INDATA[1]*INDATA[1]/INDATA[2];
		if (j==4)l=INDATA[1]*INDATA[1]/INDATA[2];
		if (j==5)l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
		if (j==6)l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);

		// ��L0=��
		L0=l; if (j==2) L0=INDATA[1];

		ptb[X]=P0tmp.y;
		ptb[Y]=P0tmp.x;
		if (zybz==1||zybz==-1)
		{
			lc=l*0.5-3.721*texth;
			if(lc<0.0) lc=l*0.5;
			xlzbxy(&P0tmp,&PZtmp,lc,INDATA,0);
			b=PZtmp.a+pi*0.5*zybz;
			pt[X]=PZtmp.y+2.0*texth*sin(b);
			pt[Y]=PZtmp.x+2.0*texth*cos(b);
			pt[Z]=0.0;
			b=PZtmp.a; 
			b=pi*0.5-b;
			if(b<0.0)b=2*pi+b;
			if(b>2*pi)b=b-2*pi;
			if (fabs(L0-(long int)(L0+0.001))<0.001)ads_rtos(L0,2,0,ch);
			else ads_rtos(L0,2,NLC,ch);
			_tcscat(chr,ch);  
			if (INDATA[0]>1&&INDATA[0]<3) //   ԰��
			{
				ads_rtos(INDATA[2],2,NLC,ch);
				_tcscat(chr,L" L=");
				_tcscat(chr,ch);
			}
			if(BZFlag&&ISShow)
				if(L0>0.001)G_maketext(pWd,pt,chr,b,texth,7,1);  // �� ֱ�ߡ�԰���ߡ��������߳���
		}		
		xlzbxy(&P0tmp,&PZtmp,l,INDATA,0); // ĳ��Ԫ�յ�
		// ���Ʒ���		

		if (zybz==0) 
		{
			j=(int)(xyarray[i][0]+0.01);  // ���� 
			if (j>1&&j<=6)  zybz=(int)(xyarray[i][4]+0.01);
			if(j==1) 		zybz=(int)(xyarray[i-1][4]+0.01);	 
			if (zybz==0||zybz>1||zybz<-1) zybz=-1;
		}
		//
		b=P0tmp.a+pi*0.5*zybz;
		pte[X]=ptb[X]+WB*sin(b); 
		pte[Y]=ptb[Y]+WB*cos(b);
		if(BZFlag&&ISShow)
			if ( L0>0.001&&i>1&&i<=nxy-1) G_makeline(pWd,ptb,pte,3);//���յ������ﲻ��0->��㣬1->�յ�

		if(i>1 && i<=nxy-1)
		{
			j=(int)(xyarray[(i-1)][0]+0.01);
			if (j>=3)
			{
				if (Chinese_Engbz_type==2)_tcscpy(ch,L" H"); 
				else _tcscpy(ch,L" S"); 
			}
			if (j==1)
			{
				if (Chinese_Engbz_type==2)_tcscpy(ch,L" Z");
				else _tcscpy(ch,L" T");				
			}
			if (j==2) 
			{
				if (Chinese_Engbz_type==2)_tcscpy(ch,L" Y"); 
				else _tcscpy(ch,L" C"); 				
			}
			_tcscpy(chr,ch);
			_tcscat(chr,tch);

			if (_tcsncmp(chr,L" YY",3)==0)_tcscpy(chr,L" GQ");
			if (_tcsncmp(chr,L" C.C.",5)==0)_tcscpy(chr,L" P.C.C.");
			if (_tcsncmp(chr,L" S.S.",5)==0&&(j==3||j==4) )_tcscpy(chr,L" PRC");
			if (_tcsncmp(chr,L" HH",3)==0&&(j==3||j==4))_tcscpy(chr,L" FGD");
			_tcscpy(DGH,chr);			
			if (KK==7)
			{
				lc=XCLC(P0tmp.lc,GH,DLB2x,NDL);

				_tcscpy(ch,L"\0"); 
				_tcscpy(ch,LCchr(GH,lc,NLC,1));
				_tcscat(chr,L" ");
				_tcscat(chr,ch);
			}
			b=P0tmp.a;

			pt[X]=ptb[X]-0.25*texth*sin(b);
			pt[Y]=ptb[Y]-0.25 *texth*cos(b);
			b=P0tmp.a-pi*0.5 ;
			if(zybz==1)
			{
				pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
				pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b);
			}			
			b=pi*0.5-b;
			if (b<0.0) b=2*pi+b;
			if (b>2*pi) b=b-2*pi; 
			if(BZFlag&&ISShow)
				G_maketext(pWd,pt,chr,b, texth,7,1);


		}//else if(i>1 && i<=nxy-1)
		P0tmp=PZtmp;
	}
}

void GTZX2_ROAD::CalXYArrayFromJdArray(double (*jdarray)[6], int njd,double (*qxb)[9],double (*xyarray)[10],int &tary)
	//�ɽ������飬������������߱�,������Ԫ���飬...
{
	double fb,fe,db,de,aa,c,lcb,lce,T0=0.0, interal=18;
	ACHAR zy[2];
	int i=0,j=0,k,color=2,iq=2;
	double BPMTL[10],T1,T2,L;
	double LCB,DDlength;
	int iDL=0;
	double (*arraypxy)[4][5];
	arraypxy=new double[njd][4][5];
	for(i=0;i<4*njd;i++)for(j=0;j<10;j++)xyarray[i][j]=0.0;//init
	for (i=0;i<10;i++) BPMTL[i]=0.0;
	for( i=0;i<njd;i++) { for (k=0;k<4;k++) {for (j=0;j<5;j++)arraypxy[i][k][j]=0.0;}; };
	db=xyddaa(jdarray[0][1],jdarray[0][2],jdarray[1][1],jdarray[1][2],&fb);
	jdarray[0][6]=fb;  
	lcb=jdarray[0][3];
	lce=lcb;
	fb=dms_rad(fb);
	arraypxy[0][0][0]=10.0;
	// N(X)=ACAD_Y
	arraypxy[0][0][1]=jdarray[0][1];
	//E(Y)=ACAD_X
	arraypxy[0][0][2]=jdarray[0][2];
	// FA
	arraypxy[0][0][3]=fb;
	// LC
	arraypxy[0][0][4]=jdarray[0][3];
	//  DLB[0].BLC=jdarray[0][3];   DLB[0].DL=0.0;  NDL=1;   // ��� ��Ϊһ���������

	qxb[0][0]=jdarray[0][3];  // ������
	qxb[0][1]=qxb[0][0];
	qxb[0][2]=qxb[0][0];
	qxb[0][3]=qxb[0][0];
	qxb[0][4]= qxb[0][5]= qxb[0][6]= qxb[0][7]= qxb[0][8]=0.0;
	T0=0.0;
	if (njd==2) // ������������,��һ��ֱ��ʱ
	{  arraypxy[1][0][0]=1.0;
	arraypxy[1][0][1]=db;
	};
	LCB=qxb[0][0]; // ������
	DDlength= LCB;
	iDL=1;

	for (i=1;i<njd-1;i++)
	{  
		if (jdarray[i][4]<=0) break;

		de=xyddaa(jdarray[i][1],jdarray[i][2],jdarray[i+1][1],jdarray[i+1][2],&fe);
		jdarray[i][6]=fe;  
		fe=dms_rad(fe);
		aa=rad(fe)-rad(fb);
		if (aa<-pi) aa=aa+2*pi;
		if  (aa>pi) aa=aa-2*pi;
		if (aa>0) 
		{  c=1; _tcscpy(zy,L"y"); }  //��ƫ
		else
		{ c=-1;_tcscpy(zy,L"z"); }; //��ƫ
		aa=fabs(aa);
		aa=dms_rad(aa);
		if (jdarray[i][4]<=0) break;
		getBPMTL(aa,jdarray[i][4],jdarray[i][3],jdarray[i][5],&BPMTL[0]);
		T1=BPMTL[6];
		T2=BPMTL[7];
		L=BPMTL[8];
		if ( L<(jdarray[i][3]+jdarray[i][5]))
		{
			ads_printf(L"\n %d������ ǰ�󻺺������ص�������L1+L2=%10.3f>�������߳�L=%10.3f ",i,jdarray[i][3]+jdarray[i][5],L);
		}
		else if((L-jdarray[i][3]-jdarray[i][5])<20.0)
		{ 
			ads_printf(L"\n %d�����ߵ�(R=%8.1f)Բ���߳�L=%10.3f ����20m ",i,jdarray[i][4],L-jdarray[i][3]-jdarray[i][5]);
		};

		//����Ҫ�� �Ƕ�Ϊ��.����
		qxb[i][4]=aa;
		qxb[i][5]=T1;
		qxb[i][6]=T2;
		qxb[i][7]=L;
		qxb[i][8]=c;
		//  line
		arraypxy[i][0][0]=1.0;
		arraypxy[i][0][1]=db-T1-T0;

		//ZH (zy)
		qxb[i][0]=qxb[i-1][3]+(db-T1-T0);   
		if( (db-T1-T0)<20.0) 
		{ 
			ads_printf(L"\n %d���������ZH/ZY��ǰһ����HZ/ZY����=%-10.3f ",i+JD0-1,db-T1-T0);
			if( (db-T1-T0)<0) ads_printf(L"  ����ǰһ����HZ/YZ���Ϸ� ");
		}		
		aa=sqrt(jdarray[i][3]*jdarray[i][4]);
		if ( aa>0.000000001)
		{
			arraypxy[i][1][0]=3.0; 
			arraypxy[i][1][1]=aa;
			//arraypxy[i][1][2]=QXLRL[i][1];
			arraypxy[i][1][2]=jdarray[i][4];
			arraypxy[i][1][3]=0.0;
			arraypxy[i][1][4]=c;
		}
		// HY
		qxb[i][1]=qxb[i][0]+jdarray[i][3];
		// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",arraypxy[i][1][0],arraypxy[i][1][1],arraypxy[i][1][2],arraypxy[i][1][4]);
		//  circle 
		aa=L-jdarray[i][3]-jdarray[i][5];
		if ( aa>0.000000001)
		{
			arraypxy[i][2][0]=2.0; 
			arraypxy[i][2][1]=jdarray[i][4];
			arraypxy[i][2][2]=aa;
			arraypxy[i][2][3]=0.0;
			arraypxy[i][2][4]=c;
		}
		// YH
		qxb[i][2]=qxb[i][1]+aa;
		//2-spiral
		aa=sqrt(jdarray[i][5]*jdarray[i][4]);
		if ( aa>0.000000001)
		{
			arraypxy[i][3][0]=4.0; 
			arraypxy[i][3][1]=aa;
			arraypxy[i][3][2]=jdarray[i][4];
			arraypxy[i][3][3]=0.0;
			arraypxy[i][3][4]=c;
		} 
		// HZ
		qxb[i][3]=qxb[i][2]+jdarray[i][5];
		db=de;
		lcb=lce;
		fb=fe;
		T0=T2;
	};
	//  line
	db=db-T0 ;
	if ( db>0.0000000001) 
	{
		arraypxy[njd-1][0][0]=1.0;	 
		arraypxy[njd-1][0][1]=db;
	};

	// �յ����
	qxb[njd-1][0]=qxb[njd-2][3]+ db;  
	xl_length=qxb[njd-1][0]-qxb[0][0];
	// if ((iDL<=NDL-1)&& (qxb[njd-1][0]>DLB[iDL].BLC) ) { qxb[njd-1][0]=qxb[njd-1][0]+DLB[iDL].DL;iDL=iDL+1; };
	qxb[njd-1][1]=qxb[njd-1][0];
	qxb[njd-1][2]=qxb[njd-1][0];
	qxb[njd-1][3]=qxb[njd-1][0];
	qxb[njd-1][4]=qxb[njd-1][5]=qxb[njd-1][6]=qxb[njd-1][7]=qxb[njd-1][8]=0.0;


	//  ���
	double INDATA[5],l,L0;
	struct xlpoint PB0,PEZ;
	for (j=0;j<5;j++)xyarray[0][j]=arraypxy[0][0][j]; 
	tary = 1;
	for( i=1;i<njd-1;i++) 
	{ 
		for (k=0;k<4;k++) 
		{			
			for (j=0;j<5;j++) INDATA[j]=arraypxy[i][k][j];
			j=(int)(INDATA[0]+0.01);
			l=0.0;
			if (j==1) l=INDATA[1];
			if (j==2) l=INDATA[2];
			if (j==3||j==4)   l=INDATA[1]*INDATA[1]/INDATA[2];
			if (j==5) l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
			if (j==6) l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);
			// ��L0=��
			L0=l;
			if (j==2) L0=INDATA[1];
			//		   ads_printf(L"l=%lf\n",l);
			if (L0>0.001)
			{
				for (j=0;j<5;j++)xyarray[tary][j]=arraypxy[i][k][j]; 
				// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",xyarray[trow][0],xyarray[trow][1],xyarray[trow][2],xyarray[trow][4]);
				tary=tary+1;  
			};  		   
		};
	};
	//  �յ�
	for (j=0;j<5;j++)xyarray[tary][j]=arraypxy[njd-1][0][j]; 	   
	tary=tary+1;
	i = 0;
	PB0.lc=xyarray[0][4];
	PB0.x =xyarray[0][1];
	PB0.y =xyarray[0][2];
	PB0.a =rad(xyarray[0][3]);
	PB0.r =1e40;
	xyarray[1][5] = PB0.a ;
	xyarray[1][6] = PB0.lc;
	xyarray[1][7] = PB0.r ;
	xyarray[1][8] = PB0.x ;
	xyarray[1][9] = PB0.y ;

	for (i = 1;i<tary;i++)//1->tary
	{ 
		// �� i ��Ԫ
		for (k=0;k<5;k++) INDATA[k]=xyarray[i][k];
		l=Get_INDATA_L(INDATA,&j);
		xlzbxy(&PB0,&PEZ,l,INDATA,0); 
		xyarray[i+1][5] = PEZ.a ;
		xyarray[i+1][6] = PEZ.lc;
		xyarray[i+1][7] = PEZ.r ;
		xyarray[i+1][8] = PEZ.x ;
		xyarray[i+1][9] = PEZ.y ;
		PB0=PEZ;
	};	
	delete []arraypxy;
	arraypxy=NULL;
}

//ͶӰ���ת�ֳ����
double GTZX2_ROAD::XCLC(double TYLC,ACHAR *GH, EXDLB *ExDlb, int ndl)
{
	double XCLC;//LCB ������ͳһ���
	int iDL=0;
	//������
	XCLC=TYLC;
	for(int i=NDL-1;i>=0;i--)
	{
		iDL=i;
		if(TYLC>=ExDlb[i].TYLC)break;
	}

	//�������жΣ������ж����д��ڣ��������ж���(����ǰ����������һ��������ͬ)
	//	acutPrintf(L"rxdnum=%d,iDL=%d,no1=%d,no=%d\n",GetRXDNum(),iDL,ExDlb[iDL].no,ExDlb[iDL+1].no);
	//plh04.21
	int RXDSum = GetRXDNum();
	if(TYLC>=ExDlb[0].TYLC&&RXDSum>0&&iDL>=0&&iDL<NDL-1&&ExDlb[iDL].no==ExDlb[iDL+1].no)
	{
		_tcscpy(GH,ExDlb[iDL].EGH);
		return TYLC-ExDlb[iDL].TYLC+ExDlb[iDL].ELC;//��ǰ�������ۼӳ��ȣ�ǰ��������ۼӳ��ȣ�ǰ������Ϻ����
	}
	else//ȫ�������ж� �������е��������ж���
	{
		//ͶӰ��һ���ϵ����//��ֱһ��
		xlpoint Pt;
		Pt.lc=TYLC;
		xlpoint_pz(array,tarray,&Pt);
		double tylc1x=m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,Pt.x,Pt.y,Pt.x,Pt.y);
		return m_pm1->XLC(tylc1x,GH,m_pm1->NDL);
	}	
}

//void GTZX2::AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd)//������������
//{
//	assertReadEnabled();
//	njd=0;
//	
//	if(GetiRXDFromNE(PT1.y,PT1.x)>=0)
//	{
//		AfxMessageBox(L"�˴������ж�!");
//		return;
//	}
//	
//	njd=-1;
//	AcGePoint3d PT2;
//	double x1,y1,x2,y2,L1,L2,R;
//	if(acedGetPoint(asDblArray(PT1),L"ȷ�������յ�:\n",asDblArray(PT2)))
//	{
//		int JDi=GetPreJD(PT1.y,PT1.x);
//
//
//		if(JDi>=NJD-1)return;
//		NJD=getJD_RL(0,&R,&L1,&L2,&y1,&x1);
//		
//		if(JDi>=0&&JDi<NJD-1)
//		{
//			NJD=getJD_RL(JDi,&R,&L1,&L2,&y1,&x1);			
//			NJD=getJD_RL(JDi+1,&R,&L1,&L2,&y2,&x2);	
////			acutPrintf(L"JDi=%d,x1=%lf,y1=%lf\n",JDi,x1,y1);
////			acutPrintf(L"JDi=%d,x2=%lf,y2=%lf\n",JDi+1,x2,y2);
//		}
//		//		acutPrintf(L"x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",m_xk1,m_yk1,m_xk2,m_yk2);
//		
//		if(JDi>=0&&JDi<NJD-1)
//		{
//			AcGePoint3d interPt;
//			AcDbLine L1(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));
//			AcDbLine L2(PT1,PT2);
//			//plh06.01
//			if(INTERS(&interPt.x,&interPt.y,x1,y1,x2,y2,PT1.x,PT1.y,PT2.x,PT2.y)>=0)
//			{
//				//acutPrintf(L"x1=%lf,y1=%lf,x0=%lf,y0=%lf,x2=%lf,y2=%lf\n",x1,y1,interPt[0].x,interPt[0].y,PT2.x,PT2.y);
//				double T2=QXB[JDi][6];//���г�
//				if(T2<0.00001)T2=50.0;//������Ϊ0
//				double fwj1=fwj(x1,y1,x2,y2);
//				AcGePoint3d tempPt(x1,y1,0.0);
//
//				getLineEndPt(tempPt,T2,fwj1,tempPt);
//				
//				
//				double rad=GetRFromT1T2(tempPt.x,tempPt.y,interPt.x,interPt.y,PT2.x,PT2.y,true,DV,0,1);
//				if(rad<0.0)return;
//				
//				njd=3;
//				int i=0;
//				jdarray[i][0]=0.0;
////				double jiao;
////				xyddaa(interPt.x,interPt.y,tempPt.x,tempPt.y,&jiao);
//				jdarray[i][1]=tempPt.y;//+0.1*sin(jiao);//��ǰ����һ��
//				jdarray[i][2]=tempPt.x;//+0.1*cos(jiao);
//				jdarray[i][3]=0.0;
//				jdarray[i][4]=0.0;
//				jdarray[i][5]=0.0;
//				
//				i=1;
//				jdarray[i][0]=0.0;
//				jdarray[i][1]=interPt.y;
//				jdarray[i][2]=interPt.x;
//				jdarray[i][3]=get_l0(rad,DV);
//				jdarray[i][4]=rad;
//				jdarray[i][5]=get_l0(rad,DV);
//				
//				i=2;
//				jdarray[i][0]=0.0;
//				jdarray[i][1]=PT2.y;
//				jdarray[i][2]=PT2.x;
//				jdarray[i][3]=0.0;
//				jdarray[i][4]=0.0;
//				jdarray[i][5]=0.0;
//			}
//			else
//			{
//				acutPrintf(L"AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd)....if(Acad::eOk==L1.intersectWith(&L2,AcDb::kExtendBoth,interPt,0,0)&&interPt.length()>0) \n");
//			}
//		}	
//		ads_printf(L"njd=%d\n",njd);
//	}
//}

//////////////////////////////////////////////////////////////////////////
//ԭ�������ж����������ǽ�ʰȡ����Ϊ�����Ľ��㡣��������ʵ���ӳ����������Ĺ��ܡ�
//���ڸĳɺͻ���������ʽ��ͬ�����㶨������ʰȡ���󽻣��ý���Ϊ��������
//��ΰ 2009-06-10
void GTZX2_ROAD::AddRXD(AcGePoint3d OutPt,double (*jdarray)[6],int &njd)//������������
{
	assertReadEnabled();
	njd=0;

	if(GetiRXDFromNE(OutPt.y,OutPt.x)>=0)
	{
		AfxMessageBox(L"�˴������ж�!");
		return;
	}

	njd=-1;
	AcGePoint3d PT2;
	AcGePoint3d PT1;
	double x1,y1,x2,y2,L1,L2,R;
	//if(acedGetPoint(asDblArray(OutPt),L"ȷ�������յ�:\n",asDblArray(PT2)))
	if(acedGetPoint(NULL,L"ȷ�����������:\n",asDblArray(PT1))==RTNORM
		&&acedGetPoint(asDblArray(PT1),L"ȷ���������յ�:\n",asDblArray(PT2))==RTNORM)
	{
		int JDi=GetPreJD(OutPt.y,OutPt.x);


		if(JDi>=NJD-1)return;
		NJD=getJD_RL(0,&R,&L1,&L2,&y1,&x1);

		if(JDi>=0&&JDi<NJD-1)
		{
			NJD=getJD_RL(JDi,&R,&L1,&L2,&y1,&x1);			
			NJD=getJD_RL(JDi+1,&R,&L1,&L2,&y2,&x2);	
		}

		if(JDi>=0&&JDi<NJD-1)
		{
			AcGePoint3d interPt;
			AcDbLine L1(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));
			//AcDbLine L2(OutPt,PT2);
			AcDbLine L2(PT1,PT2);
			//plh06.01
			if(INTERS(&interPt.x,&interPt.y,x1,y1,x2,y2,PT1.x,PT1.y,PT2.x,PT2.y)>=0)
			{
				//acutPrintf(L"x1=%lf,y1=%lf,x0=%lf,y0=%lf,x2=%lf,y2=%lf\n",x1,y1,interPt[0].x,interPt[0].y,PT2.x,PT2.y);
				double T2=QXB[JDi][6];//���г�
				if(T2<0.00001)T2=50.0;//������Ϊ0
				double fwj1=fwj(x1,y1,x2,y2);
				AcGePoint3d tempPt(x1,y1,0.0);

				getLineEndPt(tempPt,T2,fwj1,tempPt);


				double rad=GetRFromT1T2(tempPt.x,tempPt.y,interPt.x,interPt.y,PT2.x,PT2.y,mGuiFanCS,true,DV,0,1);
				if(rad<0.0)return;

				njd=3;
				int i=0;
				jdarray[i][0]=0.0;
				//				double jiao;
				//				xyddaa(interPt.x,interPt.y,tempPt.x,tempPt.y,&jiao);
				jdarray[i][1]=tempPt.y;//+0.1*sin(jiao);//��ǰ����һ��
				jdarray[i][2]=tempPt.x;//+0.1*cos(jiao);
				jdarray[i][3]=0.0;
				jdarray[i][4]=0.0;
				jdarray[i][5]=0.0;

				i=1;
				jdarray[i][0]=0.0;
				jdarray[i][1]=interPt.y;
				jdarray[i][2]=interPt.x;
				jdarray[i][3]=get_l0(rad,DV,mGuiFanCS);
				jdarray[i][4]=rad;
				jdarray[i][5]=get_l0(rad,DV,mGuiFanCS);

				i=2;
				jdarray[i][0]=0.0;
				jdarray[i][1]=PT2.y;
				jdarray[i][2]=PT2.x;
				jdarray[i][3]=0.0;
				jdarray[i][4]=0.0;
				jdarray[i][5]=0.0;
			}
			else
			{
				acutPrintf(L"AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd)....if(Acad::eOk==L1.intersectWith(&L2,AcDb::kExtendBoth,interPt,0,0)&&interPt.length()>0) \n");
			}
		}	
		ads_printf(L"njd=%d\n",njd);
	}
}


int GTZX2_ROAD::getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE )
{   
	assertReadEnabled();	
	if (JDi>=0 && JDi<=NJD-1)  //  ��~�յ�
	{  *NN=JDarray[JDi][1];
	*EE=JDarray[JDi][2];
	*L1i=JDarray[JDi][3];
	*L2i=JDarray[JDi][5];
	*Ri=JDarray[JDi][4];
	};	 
	return NJD;
}

//bool GTZX2::AddRXD(GTZX *newGtzx,double N=0,double E=0)
bool GTZX2_ROAD::AddRXD(double (*jdarray)[6],double (*qxb)[9],int njd,double N,double E,ACHAR EGH[])//plh04.11.25
	//һ��GTZXʵ����Ϊ���ж�����ߺϲ�����ʱ��β����ϴ���
{
	assertWriteEnabled();
	if(GetiRXDFromNE(N,E)>=0)
	{
		AfxMessageBox(L"�˴������ж�!");
		return false;
	}

	int i=0;
	double x1,y1,x2,y2,x3,y3;//��Ļ����:
	//1->newGtzx �����ڶ������� ��2->����ʵ����x0,y0ǰһ������,3->����ʵ���Ϻ�һ������
	double xe,ye;//newGtzx���һ���������Ļ����
	x1=jdarray[njd-2][2];
	y1=jdarray[njd-2][1];
	xe=jdarray[njd-1][2];
	ye=jdarray[njd-1][1];
	//	acutPrintf(L"xe=%lf,ye=%lf\n",xe,ye);

	if(fabs(N)<0.0001&&fabs(E)<0.0001)
	{
		N=ye;	E=xe;
	}
	int JDi=GetPreJD(N,E);
	if(JDi>=0&&JDi<NJD-1)
	{

		x2=JDarray[JDi][2];//��array��λ��Ԫ�׳���,����������JDarray��λ��������
		y2=JDarray[JDi][1];
		x3=JDarray[JDi+1][2];
		y3=JDarray[JDi+1][1];
		//		acutPrintf(L"x2=%lf,y2=%lf,x3=%lf,y3=%lf\n",x2,y2,x3,y3);
		AcDbLine L1(AcGePoint3d(x1,y1,0.0),AcGePoint3d(xe,ye,0.0));
		AcDbLine L2(AcGePoint3d(x2,y2,0.0),AcGePoint3d(x3,y3,0.0));
		AcGePoint3d interPt;//GTZX ����ߵĽ���,
		//plh06.01
		if(INTERS(&interPt.x,&interPt.y,x1,y1,xe,ye,x2,y2,x3,y3)>=0)
		{
			AcGePoint3d tPt1(x1,y1,0.0);
			double T2=qxb[njd-2][6];//���г�//plhmodify04.13
			double fwj1=fwj(tPt1.x,tPt1.y,interPt.x,interPt.y);
			getLineEndPt(tPt1,T2,fwj1,tPt1);

			AcGePoint3d tPt2(x3,y3,0.0);
			double T1=QXB[JDi+1][5];//ǰ�г�
			double fwj2=fwj(x3,y3,x2,y2);
			getLineEndPt(tPt2,T1,fwj2,tPt2);//plh05.11

			//			acutPrintf(L"interPt[0].x=%lf,interPt[0].y=%lf\n",interPt[0].x,interPt[0].y);
			double rad=GetRFromT1T2(tPt1.x,tPt1.y,interPt.x,interPt.y,tPt2.x,tPt2.y,mGuiFanCS,true,DV,0,12);
			if(rad>0)
			{
				///����һ�����жν����
				DoubleLineData *OneRXDJDB=new DoubleLineData[njd+1];
				for(i=0;i<njd;i++)
				{
					OneRXDJDB[i].JDXZ=R_X;
					OneRXDJDB[i].ISShow=true;
					OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
					OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
					OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������
					OneRXDJDB[i].x=jdarray[i][1];
					OneRXDJDB[i].y=jdarray[i][2];
					OneRXDJDB[i].Lo1=jdarray[i][3];
					OneRXDJDB[i].R=jdarray[i][4];
					OneRXDJDB[i].Lo2=jdarray[i][5];
					OneRXDJDB[i].NYDL =0.0;	
					OneRXDJDB[i].qxjk=0.0;
					OneRXDJDB[i].ISXJJFirst=false;
				}
				i=njd-1;
				OneRXDJDB[i].ISShow=true;
				OneRXDJDB[i].JDXZ=R_X;
				OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
				OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
				OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������
				OneRXDJDB[i].x=interPt.y;
				OneRXDJDB[i].y=interPt.x;
				OneRXDJDB[i].Lo1=get_l0(rad,DV,mGuiFanCS);
				OneRXDJDB[i].R=rad;
				OneRXDJDB[i].Lo2=get_l0(rad,DV,mGuiFanCS);
				OneRXDJDB[i].NYDL =0.0;	
				OneRXDJDB[i].qxjk=0.0;
				OneRXDJDB[i].ISXJJFirst=false;

				i=njd;
				OneRXDJDB[i].ISShow=true;
				OneRXDJDB[i].JDXZ=R_X;
				OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
				OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
				OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������
				OneRXDJDB[i].x=tPt2.y;
				OneRXDJDB[i].y=tPt2.x;
				OneRXDJDB[i].Lo1=0.0;
				OneRXDJDB[i].R=0.0;
				OneRXDJDB[i].Lo2=0.0;
				OneRXDJDB[i].NYDL =0.0;	
				OneRXDJDB[i].qxjk=0.0;
				OneRXDJDB[i].ISXJJFirst=false;

				InsertOneRXD(OneRXDJDB,njd+1,EGH);
				if(OneRXDJDB)
				{
					delete []OneRXDJDB;
					OneRXDJDB=NULL;
				}
				return true;
			}
		}
		else
		{
			acutPrintf(L"void GTZX2::AddRXD(GTZX *newGtzx)-...if(Acad::eOk==L1.intersectWith(&L2,AcDb::kExtendBoth,interPt,0,0)&&interPt.length()>0)\n");
		}
	}
	return false;
}

void GTZX2_ROAD::InsertOneRXD(DoubleLineData *OneRXDJDB,int njd,ACHAR EGH[])//plh04.11.25
{
	double tylc1,tylc2;//��β���ӵ��ͳһ���
	tylc1=PROJ_ML(array,tarray,OneRXDJDB[0].x,OneRXDJDB[0].y,OneRXDJDB[0].x,OneRXDJDB[0].y);
	tylc2=PROJ_ML(array,tarray,OneRXDJDB[njd-1].x,OneRXDJDB[njd-1].y,OneRXDJDB[njd-1].x,OneRXDJDB[njd-1].y);
	int JDNO1=-1,JDNO2=-1;//���ж�ǰ�����ڵĽ����
	JDNO1=DistToNearJD(tylc1,true);
	JDNO2=DistToNearJD(tylc2,false);
	double RxdStartXjj;//�����ж�����߼��
	RxdStartXjj=JD2[JDNO1].xjj;
	double RxdEndXjj;//�����ж��յ��߼��
	RxdEndXjj=JD2[JDNO2-1].xjj;
	if(JDNO1<0)JDNO1=0;
	//	acutPrintf(L"JDNO1=%d,JDNO2=%d,tylc1=%lf,tylc2=%lf %d %d\n",JDNO1,JDNO2,tylc1,tylc2,JD2[JDNO1].JDXZ,JD2[JDNO2].JDXZ);
	if(JD2[JDNO2].JDXZ!=B_X||JD2[JDNO1].JDXZ!=B_X)
	{
		return;
	}
	if(JDNO1>=0&&JDNO2>=JDNO1&&JDNO2<NJD)
	{	
		int dJD=njd-2-(JDNO2-JDNO1-1);//����ƶ�λ��//ȥ����β��������
		NJD+=dJD;
		int i;
		for(i=NJD-1;i>=JDNO2;i--)
		{				
			JD2[i]=JD2[i-dJD];
		}
		for(i=JDNO1+1;i<JDNO2+dJD;i++)
		{
			JD2[i]=OneRXDJDB[i-JDNO1];//��1�Ž��㼴�ڶ�������
			if(i==JDNO1+1)
			{
				JD2[i].jdno1=JD2[i-1].jdno1+1;
				JD2[i].xjj=RxdStartXjj;
			}
			if(i==JDNO2+dJD-1)
			{
				JD2[i].jdno1=JD2[i+1].jdno1;
				JD2[i].xjj=RxdEndXjj;
			}
		}
		//		acutPrintf(L"dJD=%d,JD=%d,JDNO1=%d,JDNO2=%d,tylc1=%lf,tylc2=%lf\n",dJD,NJD,JDNO1,JDNO2,tylc1,tylc2);
	}
	else
	{
		return;
	}
	//���ɶ�����Ԫ����
	getpxy_DATA();

	Create2XDLB(FindiRXD(JDNO1+1),EGH);//plh04.11.25
}

void GTZX2_ROAD::DeleteOneRXDByXLPoint(double N,double E)
	//ָ����·��һ��ɾ����������ж�
{
	assertWriteEnabled();
	int iJD;//ָ����ͶӰ����·��һλ�õģ�ǰһ��JD
	int FirstJD,EndJD;//ĳһ���ж���ʼ�����
	FirstJD=EndJD=iJD=-1;

	iJD=GetPreJD(N,E);
	if(iJD>=0&&iJD<=NJD-1)
	{
		if(JD2[iJD].JDXZ==R_X)
		{
			int RxdNum=GetRXDNum();
			for(int i=0;i<RxdNum;i++)
			{
				GetJDXHofiRXD(i,FirstJD,EndJD);
				if(iJD>=FirstJD&&iJD<=EndJD)
				{
					DeleteOneRXD(i);
					break;
				}
			}
		}
		else
		{
			acutPrintf(L"��ѡ��Ϊ���ж�!\n");
		}
	}
	else
	{
		acutPrintf(L"iJD=GetPreJD(N,E);\n");
	}
}

void GTZX2_ROAD::DeleteOneRXD(int iRXD)
	//��ָ�����жθĲ��У���ɾ����Ӧ����
{
	//	assertWriteEnabled();
	//��ɾ����Ӧ���жεĽ��㣬��������Ӧ�Ĳ��н���
	int FirstJD,EndJD;//ĳһ���ж���ʼ�����
	int PreJD,NextJD;//ĳһ���ж�ǰ�󽻵��
	int PreJD1x,NextJD1x;//ĳһ���ж�ǰ�󽻵��Ӧ��һ�߽������
	int DelJD,AddJD;//Ҫɾ���Ľ��㣬Ҫ���ӵĽ���
	int dJD;//���潻����ǰ�ƶ�λ��
	GetJDXHofiRXD(iRXD,FirstJD,EndJD);//��ȡiRXD����β�������
	double EndXjj;//���ж����һ��������߼��
	EndXjj=JD2[EndJD].xjj;
	PreJD=FirstJD-1;
	NextJD=EndJD+1;
	PreJD1x=JD2[PreJD].jdno1;
	NextJD1x=JD2[NextJD].jdno1;
	DelJD=NextJD-PreJD-1;
	AddJD=NextJD1x-PreJD1x-1;
	dJD=DelJD-AddJD;

	int i;
	for(i=FirstJD;i<NJD-DelJD;i++)//ɾ��RXD���㣬��ǰ��λ
	{
		JD2[i]=JD2[i+DelJD];
	}
	NJD-=DelJD;
	//ɾ����ȷ��Ҫ������
	NJD+=AddJD;
	for(i=NJD-1;i>=FirstJD+AddJD;i--)//���Ӳ��жν���洢λ�ã������λ
	{
		JD2[i]=JD2[i-AddJD];
	}
	double lo1,lo,D;
	for(i=FirstJD;i<FirstJD+AddJD; i++)
	{
		if(JD2[PreJD].JDXZ==B_X)JD2[i].jdno1=(JD2[PreJD].jdno1+1)+(i-FirstJD);
		else JD2[i].jdno1=(JD2[PreJD].jdno1)+(FirstJD-i);
		JD2[i].JDXZ=B_X;//���жν���
		JDarray[i][0] = JD0 + i;
		int jdn1 =i-FirstJD +1 + JD2[PreJD].jdno1;//��Ӧһ�߽������
		JD2[i].xjj=JD2[i-1].xjj;
		if(i==FirstJD+AddJD-1&&i<NJD-1)JD2[i].xjj = EndXjj;//���һ������߼�����ǰһ�����
		double xjj = JD2[i].xjj;
		double EXJD[2];
		CalEXJD(jdn1,i,EXJD);
		JD2[i].x = EXJD[0];
		JD2[i].y = EXJD[1];//��������߽���
		double R = m_pm1->JDarray[jdn1][4];
		double JK;
		JK = get_XJJK(m_pm1->DV,R,mGuiFanCS);//�߼��ӿ�
		JD2[i].qxjk = JK;
		JD2[i].Lo1 = m_pm1->JDarray[jdn1][3];
		JD2[i].R = R;
		JD2[i].Lo2 = m_pm1->JDarray[jdn1][5];           
		lo1 = m_pm1->JDarray[jdn1][3];//һ�߻���
		D = fabs(xjj);
		//ads_printf(L"21:%d %d %d R=%lf lo=%lf D=%lf\n",i,jdn1,JD2[i].jdno1,JD2[i].R,lo1,D);
		//���߰뾶
		if(i>0&&i<NJD-1)
		{
			double xjj2;
			xjj2 = JD2[i-1].xjj;//��һ�����߼��
			//   ads_printf(L"i=%d xjj=%lf xjj2=%lf\n",i,xjj,xjj2);
			if(fabs(xjj-xjj2)<0.01)//ǰ���߼�����
			{
				//xjj�������߼ӿ�............		   
				if(xjj>0)//һ��Ϊ����
				{
					if(m_pm1->QXB[jdn1][8]<0)//��ת,II������࣬���ٻ���
					{
						//���仺��
						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//���ȱ� 2003.12.4�ṩ
						JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
						D  = -1.0*D;
					}
					else
					{
						//���仺��
						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
						JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
					}
					//					   ads_printf(L"21:%d lo=%lf \n",i,lo);
				}
				else//һ��Ϊ����
				{
					if(m_pm1->QXB[jdn1][8]<0)//��ת,II�����ڲ�
					{
						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
						JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
					}
					else
					{   //���仺��
						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//�� 2003.12.4�ṩ
						JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
						D  = -1.0*D;
					}
				}
				//��뾶
				double B = R - D + (lo1*lo1)/24/R;
				//			ads_printf(L"lo1=%lf lo=%lf D=%lf JK=%lf B=%lf\n",lo1,lo,D,JK,B);
				JD2[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//���߰뾶				
			}
			//	   ads_printf(L"2:%d %lf %lf \n",i,JD2[i].R,JD2[i].Lo1);
		}   
		JD2[i].jdno = JD0 + i;
	}
	//////////////////////////////////////////////////////////////////////////
	DeleteiRXDDLB(iRXD);
	assertWriteEnabled();
	if(JD2[0].xjj>0)//����Ϊ����
		draw_zybz=1;//����Ҫ�ر������
	else
		draw_zybz=-1;//����Ҫ�ر����Ҳ�
	ModifyOriExJDXY();//���ɶ��߽�������
	CalJDdata1();
	SetJDdata();
	getpxy_DATA();//���ɶ�����Ԫ����
	//	Create2XDLB();
	CalNYDL();	//������ҵ����1
	//	UpdateData(false);
}

int GTZX2_ROAD::FindiRXD(int iJD)
	//����ָ���������������ж�
{
	SetJDdata();	
	getpxy_DATA();
	int iRXD=-1;//�ڼ������ж�
	for(int i=0;i<NJD;i++)
	{
		if(JD2[i].JDXZ==R_X)
		{
			iRXD++;			
			while(JD2[i].JDXZ==R_X)i++;
			if(iJD<=i)return iRXD;
		}
	}		
	return iRXD;
}

//  ����һ����������
Adesk::Boolean  GTZX2_ROAD::addDLM(double tylc,double BLC,double ELC, ACHAR *EGH )
	//���ж����ţ���ǰ��̣�����ֵ,tylc���Խ��ƣ�ֻ��Ϊ�ж�λ���ĸ�������
{   
	assertWriteEnabled();
	int iDL=0;//���������λ��,����IDLǰ��
	int i;
	for(i=NDL-1;i>=0;i--)//�������������λ��
	{
		if(tylc>DLB2x[i].TYLC)
		{
			iDL=i+1;
			break;
		}
	}
	if(iDL>NDL)return false;
	if(DLB2x[iDL-1].no!=DLB2x[iDL].no)return false;

	if (iDL>0 && iDL< NDL && NDL<300)  // NDL����СֵΪ2  ���ֵΪ300
	{
		if(	BLC>DLB2x[iDL-1].ELC && BLC< DLB2x[iDL].BLC )
		{
			//			assertWriteEnabled();
			for (i=iDL;i<=NDL-1;i++)DLB2x[NDL+iDL-i]=DLB2x[NDL-1+iDL-i];// ����һ��������
			NDL=NDL+1;
			DLB2x[iDL].EGH=EGH;
			DLB2x[iDL+1].BGH=EGH;
			DLB2x[iDL].no=DLB2x[iDL-1].no;
			//			DLB2x[iDL].TYLC=tylc;
			DLB2x[iDL].BLC=BLC;
			DLB2x[iDL].ELC=ELC;
			double DL=ELC-BLC;

			DLB2x[iDL+1].BLC += DL;
			//���¶���
			//			Create2XDLB();	
			//			GetDLTYLC();

			return 1;
		}
		else return 0;
	}
	else return 0;
}


//  ɾ��ĳͳһ��̸�����һ����������
Adesk::Boolean  GTZX2_ROAD::DelDLM(double tylc)
{ 
	assertWriteEnabled();

	int iDL;//Ҫɾ���Ķ���
	double dist,distmin=10000000;//������ľ���
	int i;
	double DL;

	for(i=0;i<NDL;i++)//����������λ��
	{
		dist=fabs(tylc-DLB2x[i].TYLC);
		if(dist<distmin)
		{
			distmin=dist;
			iDL=i;
		}
	}
	if (iDL>0 && iDL< NDL && NDL<300)  // NDL����СֵΪ2  ���ֵΪ300
	{
		DL=DLB2x[iDL].ELC-DLB2x[iDL].BLC;
		//		assertWriteEnabled();
		for (i=iDL;i<=NDL-1;i++) DLB2x[iDL+i-iDL]=DLB2x[iDL+1+i-iDL] ; // ǰ��һ��������
		NDL=NDL-1;
		DLB2x[iDL].BGH=DLB2x[iDL-1].EGH;	
		DLB2x[iDL].BLC= DLB2x[iDL].BLC-DL;
		DLB2x[iDL].ELC = DLB2x[iDL].ELC-DL;
		return 1;   	
	}
	else return 0;
}

Acad::ErrorStatus GTZX2_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											   AcDbIntArray& osnapModes,
											   AcDbIntArray& geomIds)const
{
	assertReadEnabled();	
	int i,j,k;
	AcGePoint3d Pt;
	xlpoint  PB,PE,PZ;
	double l=0.0,INDATA[5];

	for (i = 0; i < NJD; i++) //������Ϊ�е�
	{
		Pt.set(JD2[i].y,JD2[i].x,0.0);
		gripPoints.append(Pt);
	}

	// ���������Ӧ��Բ���е����е㼯����ţ�NJD-1��+��NJD-1��+1������NJD-1)+��NJD-1��+NJD-2
	//  ԰�����յ�
	PB.x=array_pxy[0][0][1]; //N
	PB.y=array_pxy[0][0][2]; // E
	PB.a=rad(array_pxy[0][0][3]);// FA
	PB.lc=array_pxy[0][0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for (i =1; i < NJD-1; i++) 
	{
		for (k =0; k <4; k++) 	
		{
			for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
			l=Get_INDATA_L(INDATA,&j);
			xlzbxy(&PB,&PE,l,INDATA,0);// ĳ��Ԫ�յ�
			xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
			Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
			if (j==2) // ԰�� �е�
			{
				gripPoints.append(Pt);
			}
			PB=PE;  
		}
	}
	/*
	for ( i = 0; i < NJD-1; i++) 
	{ 
	X1=(JDarray[i][2]+JDarray[i+1][2])*0.5;
	Y1=(JDarray[i][1]+JDarray[i+1][1])*0.5;
	Pt.set(X1,Y1,0.0);
	gripPoints.append(Pt);
	};	*/

	return Acad::eOk;
}

Acad::ErrorStatus GTZX2_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset)
{
	assertWriteEnabled();
	int k,j;
	double dx,dy,dd,R;
	dx=offset[X];
	dy=offset[Y];
	for (j=0;j<indices.length();j++)//����
	{	
		k=indices[j];

		// ������� 0~NJD-1
		if (k<NJD&&k>0) 
		{   // ��ѡ��ļе�Ϊ ĳһ���㣨���������յ㣩
			if( (JD2[k].JDXZ==R_X||JD2[k].JDXZ==B_XNOTP_X) && (JD2[k-1].JDXZ==R_X||JD2[k-1].JDXZ==B_XNOTP_X) && (JD2[k+1].JDXZ==R_X||JD2[k+1].JDXZ==B_XNOTP_X) )
			{
				JD2[k].y+=dx;
				JD2[k].x+=dy;
			}
			else if(JD2[k].JDXZ==S_StartPoint || (JD2[k].JDXZ==R_X && JD2[k-1].JDXZ!=R_X)
				|| (JD2[k].JDXZ==B_XNOTP_X && JD2[k-1].JDXZ!=B_XNOTP_X) )
			{
				double x1,y1,x2,y2;
				x1=JD2[k-1].y;
				y1=JD2[k-1].x;
				x2=JD2[k].y;
				y2=JD2[k].x;
				double f1=fwj(x1,y1,x2,y2);
				double f2=fwj(x1,y1,x2+dx,y2+dy);
				JD2[k].y+=dx;
				JD2[k].x+=dy;
				double dd=sqrt((JD2[k].x-JD2[k-1].x)*(JD2[k].x-JD2[k-1].x)
					+ (JD2[k].y-JD2[k-1].y)*(JD2[k].y-JD2[k-1].y) );
				dd*=cos(f2-f1);

				JD2[k].y=JD2[k-1].y+dd*sin(f1);
				JD2[k].x=JD2[k-1].x+dd*cos(f1);
			}
			else if(JD2[k].JDXZ==S_EndPoint
				|| (JD2[k].JDXZ==R_X && JD2[k+1].JDXZ!=R_X)
				|| (JD2[k].JDXZ==B_XNOTP_X && JD2[k+1].JDXZ!=B_XNOTP_X) )
			{
				double x1,y1,x2,y2;
				x1=JD2[k+1].y;	y1=JD2[k+1].x;
				x2=JD2[k].y;	y2=JD2[k].x;
				double f1=fwj(x1,y1,x2,y2);
				double f2=fwj(x1,y1,x2+dx,y2+dy);
				double f=fwj(0,0,dx,dy);
				JD2[k].y+=dx;
				JD2[k].x+=dy;
				double dd=sqrt((JD2[k].x-JD2[k+1].x)*(JD2[k].x-JD2[k+1].x)
					+ (JD2[k].y-JD2[k+1].y)*(JD2[k].y-JD2[k+1].y) );
				double df=fabs(f-f1);
				if(df>=2.0*PI)df-=2.0*PI;
				if(df<0.25*PI|| (df>0.75*PI&&df<1.25*PI) || df>1.75*PI)//���������߻���
				{
					dd*=cos(f2-f1);					
					JD2[k].y=JD2[k+1].y+dd*sin(f1);
					JD2[k].x=JD2[k+1].x+dd*cos(f1);				
				}
				else//�����߼��
				{
					double tylc=PROJ_ML(m_pm1->array,m_pm1->tarray,JD2[k].x,JD2[k].y,JD2[k].x,JD2[k].y);
					xlpoint Pt;//��һ������ͶӰ�������ϵĵ�
					Pt.lc=tylc;
					xlpoint_pz(m_pm1->array,m_pm1->tarray,&Pt);

					double xjj2=JD2[k+1].xjj/fabs(JD2[k+1].xjj)*sqrt((JD2[k].x-Pt.x)*(JD2[k].x-Pt.x)+(JD2[k].y-Pt.y)*(JD2[k].y-Pt.y));
					JD2[k].xjj=xjj2;//S��ڶ�������
					CalOneBXJDdata(k+1);//������ڲ��н���					
				}
			}
			if(JD2[k].JDXZ==R_X)//��Ӱ�쵽����
			{
				//�ý����Ӧ��һ�����
				int RxdNum=GetRXDNum();
				int rxdno,FirstDL,EndDL,sjd,ejd;
				rxdno = -1;
				double lc2x=QXB[k][2];//Բ�������
				for(int i=0;i<RxdNum;i++)
				{
					GetiRXDDLinfo(i,FirstDL,EndDL);
					if( lc2x>=DLB2x[FirstDL].TYLC
						&& lc2x<=DLB2x[EndDL].TYLC )
					{
						rxdno=i;
						break;
					}
				}
				GetJDXHofiRXD(rxdno,sjd,ejd);
				if(DLB2x[EndDL].TYLC<QXB[ejd][3])//���ж��յ����
				{//������°���	//plh05.19
					AfxMessageBox(L"���жζ�����");	
				}
			}
		}
		else if(k>=NJD&&k<=2*NJD-3)//Բ���е�
		{
			k=indices[j]-(NJD-1); // ��԰����Ӧ�Ľ����
			if(JD2[k].JDXZ==R_X&&JD2[k-1].JDXZ==R_X&&JD2[k+1].JDXZ==R_X)
			{
				dd=sqrt(offset[X]*offset[X]+offset[Y]*offset[Y]);
				if(dd>0.001)
				{
					double xt,yt;
					int track,type ;
					track = 1;
					type = 5;
					struct resbuf *result ;
					result = ads_buildlist(RTSTR,L"",0);
					ads_grread(track,&type,result );
					xt = result->resval.rpoint[Y];
					yt = result->resval.rpoint[X];
					ads_relrb(result) ;
					double tmp;
					calptan(JD2[k-1].x,JD2[k-1].y,JD2[k].x,JD2[k].y,JD2[k+1].x,JD2[k+1].y,xt,yt,&tmp,&R,&tmp,&tmp,&tmp,&tmp,&tmp);
					R = get_R(R,DV,mGuiFanCS);
					JD2[k].R = R;
					JD2[k].Lo1 = get_l0(R,DV,mGuiFanCS);
					JD2[k].Lo2 = get_l0(R,DV,mGuiFanCS);
				}
			}
		}
	}
	//	ModifyOriExJDXY();
	//	CalJDdata();//2005.2.1
	//	CalJDdata1();
	//
	TagEXJDIsShow();//2005-11-20��worlddraw()���Ƶ��˴�
	SetJDdata();
	getpxy_DATA();
	GetDLTYLC();
	CalNYDL();

	return Acad::eOk;
}

bool GTZX2_ROAD::AddS(double N1, double E1, double N2, double E2,double R1,double Lo1,double R2,double Lo2)
	//����һ���ԣ�N1��E1������N2��E2��Ϊ�����S��
{
	assertWriteEnabled();
	int i;
	int PreJD=GetPreJD(N1,E1);
	if(JD2[PreJD].JDXZ==R_X||JD2[PreJD+1].JDXZ==R_X)return false;//ֻ�ڽ��жμ�S��
	double tylc=PROJ_ML(m_pm1->array,m_pm1->tarray,N2,E2,N2,E2);
	xlpoint Pt;//��һ������ͶӰ�������ϵĵ�
	Pt.lc=tylc;
	xlpoint_pz(m_pm1->array,m_pm1->tarray,&Pt);
	NJD+=2;
	for(i=NJD-1;i>PreJD+2;i--)//�����2λ���ճ�����λ��
	{
		JD2[i]=JD2[i-2];
		JD2[i].jdno+=2;
	}

	i=PreJD+1;
	JD2[i].jdno=i;
	JD2[i].jdno1=JD2[PreJD+3].jdno1;
	JD2[i].JDXZ=S_StartPoint;
	JD2[i].xjj=JD2[i-1].xjj;
	JD2[i].x=N1;
	JD2[i].y=E1;
	JD2[i].Lo1=Lo1;
	JD2[i].R=R1;
	JD2[i].Lo2=Lo1;
	JD2[i].NYDL=0.0;	
	JD2[i].qxjk=0.0;
	JD2[i].ISXJJFirst=true;	

	/*
	//S���һ��������һ���ϵ�ͶӰͳһ���
	double tylc1=PROJ_ML(m_pm1->array,m_pm1->tarray,N1,E1,N1,E1);

	ACHAR GH[20];
	//S���һ��������һ���ϵ�ͶӰ�ֳ����
	double lc1x1jd=m_pm1->XLC(tylc1,GH,m_pm1->NDL);
	_tcscpy(JD2[i].ZHLC,GH);
	ACHAR temp[80];
	_stprintf(temp,L"%lf",lc);
	str*/



	i=PreJD+2;
	JD2[i].jdno=i;
	JD2[i].jdno1=JD2[PreJD+3].jdno1;
	JD2[i].JDXZ=S_EndPoint;	
	JD2[i].x=N2;
	JD2[i].y=E2;
	JD2[i].Lo1=Lo2;
	JD2[i].R=R2;
	JD2[i].Lo2=Lo2;
	JD2[i].NYDL=0.0;	
	JD2[i].qxjk=0.0;
	JD2[i].ISXJJFirst=true;

	double xjj2=JD2[i+1].xjj/fabs(JD2[i+1].xjj)*sqrt((N2-Pt.x)*(N2-Pt.x)+(E2-Pt.y)*(E2-Pt.y));
	JD2[i].xjj=xjj2;//S��ڶ�������

	CalOneBXJDdata(i+1);//������ڲ��н���

	//	SetJDdata();
	/*
	getpxy_DATA();
	CalNYDL(); 
	GetDLTYLC();//���¼��������ͳһ���*/

	UpdateData(false);
	return true;
}

void GTZX2_ROAD::CalOneBXJDdata(int iJD)
	//����ĳ���н���
{
	//	assertWriteEnabled();

	double lo,lo1,D;
	int i=iJD;
	if(JD2[i].JDXZ==B_X)//���жν���
	{
		//	if(JD2[i-1].JDXZ==S_EndPoint)JD2[i].jdno1=JD2[i-3].jdno1+1;//��һ�����н����Ӧ��һ�߽����
		//	   acutPrintf(L"B_X:%d\n",i);		   
		JDarray[i][0] = JD0 + i;
		int jdn1 = JD2[i].jdno1;//��Ӧһ�߽������
		if(i==NJD-1)
			JD2[i].xjj = JD2[i-1].xjj;//���һ������߼�����ǰһ�����
		double xjj = JD2[i].xjj;
		double EXJD[2];
		CalEXJD(jdn1,i,EXJD);
		JD2[i].x = EXJD[0];
		JD2[i].y = EXJD[1];//��������߽���
		double R = m_pm1->JDarray[jdn1][4];
		double JK;
		JK = get_XJJK(m_pm1->DV,R,mGuiFanCS);//�߼��ӿ�
		JD2[i].qxjk = JK;
		JD2[i].Lo1 = m_pm1->JDarray[jdn1][3];
		JD2[i].R = R;
		JD2[i].Lo2 = m_pm1->JDarray[jdn1][5];           
		lo1 = m_pm1->JDarray[jdn1][3];//һ�߻���
		D = fabs(xjj);
		//���߰뾶
		if(i>0&&i<NJD-1)
		{
			double xjj2;
			xjj2 = JD2[i-1].xjj;//��һ�����߼��
			//   ads_printf(L"i=%d xjj=%lf xjj2=%lf\n",i,xjj,xjj2);
			if(fabs(xjj-xjj2)<0.01)//ǰ���߼�����
			{
				//xjj�������߼ӿ�............		   
				if(xjj>0)//һ��Ϊ����
				{
					if(m_pm1->QXB[jdn1][8]<0)//��ת,II������࣬���ٻ���
					{
						//���仺��
						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//���ȱ� 2003.12.4�ṩ
						JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
						D  = -1.0*D;
					}
					else
					{
						//���仺��
						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
						JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
					}
				}
				else//һ��Ϊ����
				{
					if(m_pm1->QXB[jdn1][8]<0)//��ת,II�����ڲ�
					{
						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
						JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
					}
					else
					{   //���仺��
						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//�� 2003.12.4�ṩ
						JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
						D  = -1.0*D;
					}
				}
				//��뾶
				double B = R - D + (lo1*lo1)/24/R;
				JD2[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//���߰뾶				
			}
		}   
		JD2[i].jdno = JD0 + i;
	}//����
}

void GTZX2_ROAD::ModifyOneSWJDdata(int iJD,double N,double E,double XJJ=0.0,double JIAO=0.0)
	//�޸�S���һ�����������
{
	//	AfxMessageBox(L"ModifyOneSWJDdatain");
	assertWriteEnabled();
	//	acutPrintf(L"++++++++++++%d,%d\n",iJD,JD2[iJD].JDXZ);
	if(JD2[iJD].JDXZ==S_EndPoint)//
	{
		//		AfxMessageBox(L"ModifyOneSWJDdata1");
		int N1,E1,N2,E2,N3,E3,iJD1,iJD2;//��Ӧ��һ�߱�
		N3 = 0;
		E3 = 0;
		iJD1=JD2[iJD-2].jdno1;
		iJD2=JD2[iJD+1].jdno1;
		N1=m_pm1->JDarray[iJD1][1];
		E1=m_pm1->JDarray[iJD1][2];
		N2=m_pm1->JDarray[iJD2][1];
		E2=m_pm1->JDarray[iJD2][2];
		if(GetSide(N1,E1,N2,E2,JD2[iJD].x,JD2[iJD].y)*GetSide(N1,E1,N2,E2,N,E)<0)return;
		if(GetSide(N3,E3,N2,E2,JD2[iJD].x,JD2[iJD].y)*GetSide(N3,E3,N2,E2,N,E)<0)return;
		double tylc=PROJ_ML(m_pm1->array,m_pm1->tarray,N,E,N,E);
		xlpoint Pt;//��һ������ͶӰ�������ϵĵ�
		Pt.lc=tylc;
		xlpoint_pz(m_pm1->array,m_pm1->tarray,&Pt);
		if(fabs(XJJ)<=0.0001)//û�������߼��ʱ��������
		{
			JD2[iJD].x=N;
			JD2[iJD].y=E;
			double xjj2=JD2[iJD+1].xjj/fabs(JD2[iJD+1].xjj)*sqrt((N-Pt.x)*(N-Pt.x)+(E-Pt.y)*(E-Pt.y));
			JD2[iJD].xjj=xjj2;//S��ڶ�������
		}
		else//�����߼��ʱ�����߼����㽻��
		{
			double jiao=fwj(Pt.x,Pt.y,N,E);
			N=Pt.x+XJJ*sin(jiao);
			E=Pt.y+XJJ*cos(jiao);
			JD2[iJD].x=N;
			JD2[iJD].y=E;
			JD2[iJD].xjj=XJJ;//S��ڶ�������			
		}
		CalOneBXJDdata(iJD+1);//������ڲ��н���
		if(fabs(JIAO)>0.00001)//S���һ���������һ�����ڲ��н������߲����Ӧ��ƽ��,��������
		{
			if(iJD<NJD-2)//S����һ���н��㲻Ϊ��·�յ�
			{
				double Ne,Ee;//�������յ�
				double bianjiao;//�ñߵĽǶ�
				bianjiao=fwj(N,E,JD2[iJD+1].x,JD2[iJD+1].y)+JIAO;
				Ne=N+1000*sin(bianjiao);
				Ee=E+1000*cos(bianjiao);
				AcDbLine L1(AcGePoint3d(JD2[iJD+1].x,JD2[iJD+1].y,0.0),AcGePoint3d(JD2[iJD+2].x,JD2[iJD+2].y,0.0));
				AcDbLine L2(AcGePoint3d(N,E,0.0),AcGePoint3d(Ne,Ee,0.0));
				AcGePoint3dArray interPt;
				if(L1.intersectWith(&L2,AcDb::kExtendBoth,interPt)==Acad::eOk&&interPt.length()>0)
				{
					JD2[iJD+1].x=interPt[0].x;
					JD2[iJD+1].y=interPt[0].y;
				}
			}
		}
		UpdateData(false);

		/*
		SetJDdata();
		getpxy_DATA();
		CalNYDL();  
		GetDLTYLC();//���¼��������ͳһ���
		*/
	}
}

double GTZX2_ROAD::DistToJD(double N, double E, int iJD)
{
	assertReadEnabled();
	return sqrt((N-JD2[iJD].x)*(N-JD2[iJD].x)+(E-JD2[iJD].y)*(E-JD2[iJD].y));
}

void GTZX2_ROAD::DelS(double N, double E)
	//ɾ��ָ���㴦��S��
{
	assertWriteEnabled();
	int S_JD1,S_JD2;//S��������������
	int iJD=GetPreJD(N,E);
	if(JD2[iJD].JDXZ==S_StartPoint)
	{
		S_JD1=iJD;
		S_JD2=iJD+1;
	}
	else if(JD2[iJD].JDXZ==S_EndPoint)
	{
		S_JD1=iJD-1;
		S_JD2=iJD;
	}
	else
	{
		acutPrintf(L"��ѡ��ΪS�䣬������ѡ��!\n");
		return;
	}
	for(int i=S_JD1;i<NJD-2;i++)
	{
		JD2[i]=JD2[i+2];
		JD2[i].jdno=i;
	}
	CalOneBXJDdata(S_JD1);//������ڲ��н���
	NJD-=2;
	/*
	SetJDdata();
	getpxy_DATA();
	CalNYDL(); 
	GetDLTYLC();//���¼��������ͳһ���
	*/
}

void GTZX2_ROAD::SetDLB(EXDLB *Dlb, int ndl)
{
	assertWriteEnabled();
	NDL=ndl;
	for(int i=0;i<ndl;i++)DLB2x[i]=Dlb[i];
	GetDLTYLC();	
	Create2XGTDLB();
}
void GTZX2_ROAD::SetDLB(BAS_DRAW_FUN::EXDLB *Dlb, int ndl)
{
	EXDLB TempDlb;
	assertWriteEnabled();
	NDL=ndl;
	for(int i=0;i<ndl;i++)
	{
		TempDlb.BGH=Dlb[i].BGH;
		TempDlb.BLC=Dlb[i].BLC;
		TempDlb.EGH=Dlb[i].EGH;
		TempDlb.ELC=Dlb[i].ELC;
		TempDlb.no=Dlb[i].no;
		TempDlb.RXDName=Dlb[i].RXDName;
		TempDlb.TYLC=Dlb[i].TYLC;

		//TempDlb.XLName=Dlb[i].XLName;

		DLB2x[i]=TempDlb;
	}
	GetDLTYLC();	
	Create2XGTDLB();
}

void GTZX2_ROAD::GetDLTYLC()
	//����ÿ����������ͳһ��̣����������ϵͳ����
{
	ACHAR xlc[80],ch[80];
	double tylc1x,tylc2x;
	for(int i=0;i<NDL;i++)
	{
		if(i==0||(i>0&&DLB2x[i].no!=DLB2x[i-1].no))//������жε�һ������
		{

			_tcscpy(xlc,DLB2x[i].BGH);
			_stprintf(ch,L"%lf",DLB2x[i].BLC);
			_tcscat(xlc,ch);
			//			if(!m_pm1)AfxMessageBox(L"1");
			tylc1x=TYLC1(xlc,m_pm1->NDL,m_pm1->DLB);//������ж�����Ӧ��һ��ͳһ���
			double resPt[2],xjj;
			CalXjj(tylc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,DLB2x[i].TYLC,resPt);
			if (DLB2x[i].TYLC<-0.001)
			{
				DLB2x[i].TYLC=DLB2x[i].ELC;
			}
			//acutPrintf(L"tylc1x=%lf,tylc2x=%lf,%lf,%lf,xjj=%lf\n",tylc1x,DLB2x[i].TYLC,resPt[1],resPt[0],xjj);
			do 
			{
				i++;
				DLB2x[i].TYLC=DLB2x[i-1].TYLC+DLB2x[i].BLC-DLB2x[i-1].ELC;//�������жε�һ������֮��Ķ�����ͳһ���
			}while(i<NDL-1&&DLB2x[i+1].no==DLB2x[i].no);//���һ�������������ж����һ������
			_tcscpy(xlc,DLB2x[i].EGH);
			_stprintf(ch,L"%lf",DLB2x[i].ELC);
			_tcscat(xlc,ch);
			tylc1x=TYLC1(xlc,m_pm1->NDL,m_pm1->DLB);//������ж��յ��Ӧ��һ��ͳһ���
			CalXjj(tylc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,tylc2x,resPt);

			if (tylc2x>0.001)
			{
				DLB2x[i].TYLC=tylc2x;//plh05.14
				//			acutPrintf(L"xlc=%s,tylc1x=%lf,tylc2x=%lf\n",xlc,tylc1x,tylc2x);
				DLB2x[i].BLC=DLB2x[i].TYLC-DLB2x[i-1].TYLC+DLB2x[i-1].ELC;
			}
			else
			{
				DLB2x[i].TYLC=QXB[NJD-1][3];
			}
			//			dlc=tylc2x-DLB2x[i].TYLC;
			//			DLB2x[i].TYLC+=dlc;
			//			DLB2x[i].BLC+=dlc;

		}
	}
}

//void GTZX2::Insert2XDLB(int iRXD,EXDLB *ExDlb, int DlNum)
////����iRXD�Ķ��������������еĶ�������
//{
//	if(iRXD<0)return;
//	if(iRXD==GetRXDNum()-1)//��β��������ж�
//	{
//		for(int i=NDL;i<NDL+DlNum;i++)//�滻
//		{
//			DLB2x[i]=ExDlb[i-NDL];
//		}
//	}
//	else//���м�������ж�
//	{
//		int FirstDlno,EndDlno;
//		GetiRXDDLinfo(iRXD,FirstDlno,EndDlno);	
//
//		int i;
////		acutPrintf(L"NDL+DlNum-1=%d,FirstDlno+DlNum=%d\n",NDL+DlNum-1,FirstDlno+DlNum);
//		for(i=NDL+DlNum-1;i>=FirstDlno+DlNum;i--)//����ƶ�DlNumλ
//		{
//			DLB2x[i]=DLB2x[i-DlNum];
//			DLB2x[i].no+=1;
//		}
////		acutPrintf(L"FirstDlno=%d,irstDlno+DlNum=%d\n",FirstDlno,FirstDlno+DlNum);
//		for(i=FirstDlno;i<FirstDlno+DlNum;i++)//�滻
//		{
//			DLB2x[i]=ExDlb[i-FirstDlno];
//			DLB2x[i].no=iRXD;
//		}
//	}
//	NDL+=DlNum;	
//}

void GTZX2_ROAD::Insert2XDLB(int iRXD,EXDLB *ExDlb, int DlNum)
	//����iRXD�Ķ��������������еĶ�������
{

	if(iRXD<0)return;
	if(iRXD==GetRXDNum()-1)//��β��������ж�
	{
		for(int i=NDL;i<NDL+DlNum;i++)//�滻
		{
			DLB2x[i]=ExDlb[i-NDL];	
		}
	}
	else//���м�������ж�
	{
		int FirstDlno,EndDlno;
		GetiRXDDLinfo(iRXD,FirstDlno,EndDlno);	

		//�������� 
		if(FirstDlno==-1 && EndDlno==-1/* && NDL == 0 */)
		{
			for(int i=NDL;i<NDL+DlNum;i++)//�滻
			{
				DLB2x[i]=ExDlb[i-NDL];
			}
		}
		else
		{
			int i;
			//		acutPrintf(L"NDL+DlNum-1=%d,FirstDlno+DlNum=%d\n",NDL+DlNum-1,FirstDlno+DlNum);
			for(i=NDL+DlNum-1;i>=FirstDlno+DlNum;i--)//����ƶ�DlNumλ
			{	
				if(i-DlNum>=0)
				{
					DLB2x[i]=DLB2x[i-DlNum];
					DLB2x[i].no+=1;
				}
			}

			for(i=FirstDlno;i<FirstDlno+DlNum;i++)//�滻
			{
				DLB2x[i]=ExDlb[i-FirstDlno];
				DLB2x[i].no=iRXD;
			}


			/*	for(int i=0;i<DlNum;i++)
			{
			DLB2x[i]=ExDlb[i];	
			}*/
		}
	}
	NDL+=DlNum;

}

int GTZX2_ROAD::GetiRxdNoFromTYLC1x(double lc1x)
{//plh05.19
	int FirstDL,EndDL;//ĳһ���ж���ʼ�����
	FirstDL=EndDL=-1;
	//	GetDLTYLC();
	double lc2x,xjj,resPt[2];
	CalXjj(lc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,lc2x,resPt);
	int RxdNum=GetRXDNum();
	for(int i=0;i<RxdNum;i++)
	{
		GetiRXDDLinfo(i,FirstDL,EndDL);
		if( lc2x>=DLB2x[FirstDL].TYLC
			&& lc2x<=DLB2x[EndDL].TYLC )
		{
			return i;
		}
	}
	return -1;
}

int GTZX2_ROAD::GetiRXDFromNE(double N, double E)
	//�ɾ��ࡢγ�࣬���������ж���ţ�0��ʼ��
{
	int iJD;//ָ����ͶӰ����·��һλ�õģ�ǰһ��JD
	int FirstJD,EndJD;//ĳһ���ж���ʼ�����
	FirstJD=EndJD=iJD=-1;

	iJD=GetPreJD(N,E);
	if(iJD>=0&&iJD<=NJD-1)
	{
		if(JD2[iJD].JDXZ==R_X)
		{
			int RxdNum=GetRXDNum();
			for(int i=0;i<RxdNum;i++)
			{
				GetJDXHofiRXD(i,FirstJD,EndJD);
				if(iJD>=FirstJD&&iJD<=EndJD)
				{
					return i;
				}
			}
		}
		else
		{
			acutPrintf(L"��ѡ��Ϊ���ж�!\n");
			return -1;
		}
	}
	else
	{
		acutPrintf(L"iJD=%d %lf %lf\n",iJD,N,E);
	}
	return -1;
}

void GTZX2_ROAD::AddDoubleLineJD(DoubleLineData ExJD)//plh
{
	NJD+=1;
	JD2[NJD-1]=ExJD;
}

void GTZX2_ROAD::ModifyDoubleLineJD(int iJD,DoubleLineData ExJD)//plh
{
	JD2[iJD]=ExJD;
}

//
void GTZX2_ROAD::GetiRXDJDArray(int iRXD, double jday[][6], int &njd)//plh
{
	int sjd=0,ejd=0,i=0,j=0,sdl=0,edl=0,ndl=0;
	njd=0;
	GetiRXDDLinfo(iRXD,sdl,edl);//��ȡ���ն�����

	xlpoint PZ1,PZ2;	//�������յ�������	
	PZ1.lc=DLB2x[sdl].TYLC;
	PZ2.lc=DLB2x[edl].TYLC;

	xlpoint_pz(array,tarray,&PZ1);//������
	xlpoint_pz(array,tarray,&PZ2);

	GetJDXHofiRXD(iRXD,sjd,ejd);//��ȡ���ս����
	//	acutPrintf(L"sjd=%d,ejd=%d\n",sjd,ejd);

	njd=ejd-sjd+1;//������
	//�����������յ�
	njd += 2;
	if (sjd==0)
	{
		njd -= 1;
	}
	if (ejd==NJD-1)
	{
		njd -= 1;
	}
	// 	double (*jday)[6];//��̬���ٽ�������
	// 	jday=new double[njd][6];
	if (sjd!=0 && ejd!=NJD-1)
	{
		for(i=1;i<njd-1;i++)//1->njd-1
		{
			jday[i][0]=1.0*i;
			for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i-1][j];//sjd+i-1,��sjd��ʼ
		}
		//���ж������Ϊ��һ������
		i=0;
		jday[i][0]=i;
		jday[i][1]=PZ1.x;
		jday[i][2]=PZ1.y;
		jday[i][3]=0.0;
		jday[i][4]=0.0;
		jday[i][5]=0.0;
		//���ж��յ���Ϊ���һ������
		i=njd-1;
		jday[i][0]=i;
		jday[i][1]=PZ2.x;
		jday[i][2]=PZ2.y;
		jday[i][3]=0.0;
		jday[i][4]=0.0;
		jday[i][5]=0.0;
	}

	else if (sjd==0 && ejd!=NJD-1)
	{
		for(i=0;i<njd-1;i++)//1->njd-1
		{
			jday[i][0]=1.0*i;
			for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i][j];//sjd+i-1,��sjd��ʼ
		}
		jday[0][3] = 0.0;
		//���ж��յ���Ϊ���һ������
		i=njd-1;
		jday[i][0]=i;
		jday[i][1]=PZ2.x;
		jday[i][2]=PZ2.y;
		jday[i][3]=0.0;
		jday[i][4]=0.0;
		jday[i][5]=0.0;
	}

	else if (sjd!=0 && ejd==NJD-1)
	{
		for(i=1;i<njd;i++)//1->njd-1
		{
			jday[i][0]=1.0*i;
			for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i-1][j];//sjd+i-1,��sjd��ʼ
		}
		//���ж������Ϊ��һ������
		i=0;
		jday[i][0]=i;
		jday[i][1]=PZ1.x;
		jday[i][2]=PZ1.y;
		jday[i][3]=0.0;
		jday[i][4]=0.0;
		jday[i][5]=0.0;
	}

	else
	{
		for(i=0;i<njd;i++)//1->njd-1
		{
			jday[i][0]=1.0*i;
			for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i][j];//sjd+i-1,��sjd��ʼ
		}
		jday[0][3] = 0.0;
	}

	jday[0][3]=DLB2x[sdl].ELC;
}

//��Ƕ��߽����Ƿ�λ�ڶ�����(�Ƿ���ʾ����Ļ��)
void GTZX2_ROAD::TagEXJDIsShow()
{
	//һ�߶�Ӧ��������뷽����
	//���У������Ӧһ�߽����
	//�������յ㣬�����Ӧһ�߱ߵĺ�һ�������
	//S�����յ㣬�����Ӧһ�߱ߵĺ�һ������ţ���ǰΪǰһ������ţ�Ҫ�ģ���������������������
	//���в�ƽ�����յ㣬�����Ӧһ�߱ߵĺ�һ������ţ�
	int iDuanLuo=0;
	DoubleLineData *CurJD;
	DoubleLineData tempJD;	
	//��ǰ���㵽��Ӧ�ߵľ���,��0.5m��ʾ������,���㲻����ʱ������Ϊ��99
	//����һ�����Ӧ��ͶӰ��ͶӰ�㵽���������С����
	bool DuanLuoStart=true;//һ���������
	tempJD.R=0.0;
	tempJD.Lo1=0.0;
	tempJD.Lo2=0.0;
	tempJD.xjj=0.0;

	if(NJD<1)return;

	//��ÿ����������б�
	//��㶨Ϊ���߻����ߵ����
	int i=1;

	for(i=0;i<NJD;i++)
	{
		CurJD=&JD2[i];
		if(CurJD->JDXZ==B_X && fabs(CurJD->xjj)<0.0001)
			CurJD->ISShow=false;// ����ʾ
		else 
		{
			CurJD->ISShow = true;
			break;//����ǰ�治��ʾ�Ľ����ǳ���
		}
	}
	for(;i<NJD;i++)
	{
		CurJD=&JD2[i];//��ǰ���߽���
		CurJD->ISShow=true;
		//���Ҷ����յ�

		bool ifend=false;//�Ƿ�����յ�

		if(i==NJD-1) 
		{
			ifend = true;//�������,���һ����
		}
		else if(CurJD->JDXZ==B_X && fabs(CurJD->xjj)<0.0001)//���н��㣬���߼��Ϊ0
		{
			ifend = true;//�������,���߻���
		}
		else if(CurJD->JDXZ==S_EndPoint && fabs(CurJD->xjj)<0.0001)//S���յ㣬���߼��Ϊ0
		{
			ifend = true;//�������,ֱ�߻���
		}
		else if( (CurJD->JDXZ==R_X ||CurJD->JDXZ==B_XNOTP_X)
			&& CurJD->jdno1>0 && fabs(CurJD->xjj)<0.0001 )//���ж����һ�����㣬��Ӧһ�߽����������Ƕ�Ӧһ�߱ߵĺ�һ����
		{
			ifend=true;
			if(i>1&&JD2[i-1].ISStartEnd)ifend=false;
		}
		if(ifend)//�������
		{
			//������������Ϊ���в����߼��Ϊ0
			for(i++;i<NJD;i++)
			{
				CurJD=&JD2[i];
				if( CurJD->JDXZ==B_X && fabs(CurJD->xjj)<0.0001 )
					CurJD->ISShow=false;// ����ʾ
				else
				{
					CurJD->ISShow=true;
					break;
				}
			}
		}
	}
	for(i=0;i<NJD;i++)
	{
		if(!JD2[i].ISShow)
		{
			JD2[i].R=m_pm1->JDarray[JD2[i].jdno1][4];
			JD2[i].Lo1=m_pm1->JDarray[JD2[i].jdno1][3];
			JD2[i].Lo2=m_pm1->JDarray[JD2[i].jdno1][5];
		}
	}
	//	out();
}

void GTZX2_ROAD::out()
{
	FILE *fp;
	if(LEFT_OR_RIGHT==LEFTLINE)fp=_wfopen(L"C:\\Documents and Settings\\All Users\\����\\���߽���.txt",L"w");
	else fp=_wfopen(L"C:\\Documents and Settings\\All Users\\����\\���߽���.txt",L"w");
	fwprintf(fp,L"���       E       ����N           R         Lo1     ��ʾ��  �߼�� һ�߽����\n");
	int i;
	for( i=0;i<NJD;i++)
	{
		ACHAR xs[2];
		if(JD2[i].ISShow)_tcscpy(xs,L"��");
		else _tcscpy(xs,L"��");
		fwprintf(fp,L"%3d %14.3lf,%14.3lf %10.3lf %10.3lf %s %10.2lf %3d\n",i,JD2[i].y,
			JD2[i].x,JD2[i].R,JD2[i].Lo1,xs,JD2[i].xjj,JD2[i].jdno1);
	}
	fclose(fp);

	if(LEFT_OR_RIGHT==LEFTLINE)fp=_wfopen(L"C:\\Documents and Settings\\All Users\\����\\���߶���.txt",L"w");
	else fp=_wfopen(L"C:\\Documents and Settings\\All Users\\����\\���߶���.txt",L"w");
	fwprintf(fp,L"���       ��ǰ���       �����Ϻ����\n");
	fwprintf(fp,L"����%d������!\n",NDL);

	for(i=0;i<NDL;i++)
	{
		ACHAR xs[2];
		if(JD2[i].ISShow)_tcscpy(xs,L"��");
		else _tcscpy(xs,L"��");
		fwprintf(fp,L"%3d   %s %14.3lf  %s %14.3lf\n",i,DLB2x[i].BGH,
			DLB2x[i].BLC,DLB2x[i].EGH,DLB2x[i].ELC);
	}
	fclose(fp);	
}



//�����������꣬���������п��ܲ����򲻹���ȷ������������������������
//һ���������
//1������Ϊ���У�
//	��1��Ϊȫ����㣨��Ӧһ�߽������Ϊ0�������߼����
//	��2����Ϊȫ����㣨��Ӧһ�߽������>0),���߼���󣬶�Ӧһ�߽����ǰ�����ƽ����
//2������ΪS����㣨������S�����յ㣩
//	��1�������꣬�߼�ࣨ����߼��Ϊ0������һ�߶�Ӧ����ͶӰ������ƫ�����
//	��2����������̺��߼�ࣨ����߼��Ϊ0����
//3������Ϊ����
//	��1�������꣬�߼�ࣨ�߼��Ϊ0������һ�߶�Ӧ����ͶӰ������ƫ�����
//	��2����������̺��߼�ࣨ�߼��Ϊ0����
//4������Ϊ���в�ƽ��
//	��1��Ϊȫ����㣨��Ӧһ�߽������Ϊ0�������߼��������Ϊ����(!!!!!!�ݲ�����)
//	��1����Ϊȫ����㣨��Ӧһ�߽������>0��
//		�١������꣬�߼�ࣨ�߼��Ϊ0������һ�߶�Ӧ����ͶӰ������ƫ�����
//		�ڡ���������̺��߼�ࣨ�߼��Ϊ0����
//���������յ�
//1������Ϊ���У�
//	��1��Ϊȫ���յ㣨��Ӧһ�߽������ΪNJD��1�������߼��������Ϊ����
//	��2����Ϊȫ�յ�,����Ӧһ�߽�����ţ�NJD��1�����߼���󣬶�Ӧһ�߽����ǰ�����ƽ����
//2������ΪS���յ㣨������S�����յ㣩
//	��1�������꣬�߼�ࣨ�յ��߼��Ϊ0������һ�߶�Ӧ����ͶӰ������ƫ�����
//	��2����������̺��߼�ࣨ�յ��߼��Ϊ0����
//3������Ϊ����
//	��1�������꣬�߼�ࣨ�߼��Ϊ0������һ�߶�Ӧ����ͶӰ������ƫ�����
//	��2����������̺��߼�ࣨ�߼��Ϊ0����
//4������Ϊ���в�ƽ��
//	��1��Ϊȫ���յ㣨��Ӧһ�߽������ΪNJD��1�����꣬��̶�Ϊ�գ������߼����(!!!!!!�ݲ�����)
//	��2����Ϊȫ���յ㣨��
//		�١������꣬�߼�ࣨ�߼��Ϊ0������һ�߶�Ӧ����ͶӰ������ƫ�����
//		�ڡ���������̺��߼�ࣨ�߼��Ϊ0����
void GTZX2_ROAD::ModifyOriExJDXY()
{
	int i=0;
	double fwj;
	DoubleLineData *CurJD;
	for(i=0;i<NJD;i++)
	{
		CurJD=&JD2[i];
		if( CurJD->JDXZ==B_X )
		{//�����������Ϊ����(ȫ�����յ�)				
			if(CurJD->jdno1==0)
			{//��ȫ�����
				xyddaa(m_pm1->JDarray[0][1],m_pm1->JDarray[0][2],
					m_pm1->JDarray[1][1],m_pm1->JDarray[1][2],&fwj);
				double xjj;
				xjj = CurJD->xjj;
				CurJD->x  =  m_pm1->JDarray[0][1] + xjj * cos(fwj+0.5*pi);
				CurJD->y  =  m_pm1->JDarray[0][2] + xjj * sin(fwj+0.5*pi);//��������߽���
			}
			else if(CurJD->jdno1==m_pm1->NJD-1)
			{//ȫ���յ�
				int jd1x=m_pm1->NJD-2;
				xyddaa(m_pm1->JDarray[jd1x][1],m_pm1->JDarray[jd1x][2],
					m_pm1->JDarray[jd1x+1][1],m_pm1->JDarray[jd1x+1][2],&fwj);
				double xjj;
				xjj = CurJD->xjj;
				CurJD->x  =  m_pm1->JDarray[jd1x+1][1] + xjj * cos(fwj+0.5*pi);
				CurJD->y  =  m_pm1->JDarray[jd1x+1][2] + xjj * sin(fwj+0.5*pi);//��������߽���
			}
			else
			{
				//���ݽ���ǰ�������ߵ��߼��ƽ��һ����Ӧ���߼��õ�
				double xjj1;
				if(i==0)
					xjj1=0;
				else
					xjj1=JD2[i-1].xjj;
				CalExJDByParrel(CurJD->jdno1,xjj1,CurJD->xjj,CurJD);
			}
		}//if(CurJD->JDXZ==B_X)
		else
		{
			//���л��в�ƽ�жε��м佻�㲻����//plh2006.04.17
			if(i > 0 && JD2[i-1].JDXZ == JD2[i].JDXZ == R_X)
			{
				if(JD2[i].x < 0.001 || JD2[i].y < 0.001)
				{
					CString str;
					str.Format(L"��%d�����㾭γ����Ч!", i+1);
					AfxMessageBox(str);
				}
				continue;
			}
			if(i > 0 && JD2[i-1].JDXZ == JD2[i].JDXZ == B_XNOTP_X)
			{
				if(JD2[i].x < 0.001 || JD2[i].y < 0.001)
				{
					CString str;
					str.Format(L"��%d�����㾭γ����Ч!", i+1);
					AfxMessageBox(str);
				}
				continue;
			}
			if(i < NJD-1 && JD2[i+1].JDXZ == JD2[i].JDXZ == R_X)
			{
				if(JD2[i].x < 0.001 || JD2[i].y < 0.001)
				{
					CString str;
					str.Format(L"��%d�����㾭γ����Ч!", i+1);
					AfxMessageBox(str);
				}
				continue;
			}
			if(i < NJD-1 && JD2[i+1].JDXZ == JD2[i].JDXZ == B_XNOTP_X)
			{
				if(JD2[i].x < 0.001 || JD2[i].y < 0.001)
				{
					CString str;
					str.Format(L"��%d�����㾭γ����Ч!", i+1);
					AfxMessageBox(str);
				}
				continue;
			}
			//���л��в�ƽ�жε��м佻�㲻����//plh2006.04.17

			if(CurJD->jdno1>0)
			{
				if(fabs(CurJD->x)>0.1&&fabs(CurJD->y)>0.1)
				{//�����꣬���������ƫ�࣬�������������ȷλ��(��Ӧһ�߱���)
					//ModifyNEbyXJJ(CurJD->x,CurJD->y,CurJD->xjj);
					ModifyNEOnBian(CurJD);
				}
				else if(FindACHAR(CurJD->JDLC,'+')>=0)
				{//�����,���ݽ����Ӧ����̣���ƫ��������
					ModifyNEbyXJJ(CurJD->JDLC,CurJD->xjj,CurJD->x,CurJD->y);
				}
			}
		}
	}//for(i=0;i<OriNJD-1;i++)
	//���ݸ�EXJD
}

//�ɽ���ǰ��ߵ��߼�������߽�������,�����ڶ��߽��н���
void GTZX2_ROAD::CalExJDByParrel(int jdn1, double xjj1, double xjj2, DoubleLineData *CurJD)
{
	if(jdn1<1||jdn1>m_pm1->NJD-2)return;
	double fwj;
	xyddaa(m_pm1->JDarray[jdn1-1][1],m_pm1->JDarray[jdn1-1][2],m_pm1->JDarray[jdn1][1],m_pm1->JDarray[jdn1][2],&fwj);
	double pt1[2],pt2[2];
	pt1[0] = m_pm1->JDarray[jdn1-1][1] + xjj1 * cos(fwj+0.5*pi);
	pt1[1] = m_pm1->JDarray[jdn1-1][2] + xjj1 * sin(fwj+0.5*pi);
	pt2[0] = m_pm1->JDarray[jdn1][1] + xjj1 * cos(fwj+0.5*pi);
	pt2[1] = m_pm1->JDarray[jdn1][2] + xjj1 * sin(fwj+0.5*pi);
	double pt3[2],pt4[2];
	xyddaa(m_pm1->JDarray[jdn1][1],m_pm1->JDarray[jdn1][2],m_pm1->JDarray[jdn1+1][1],m_pm1->JDarray[jdn1+1][2],&fwj);
	pt3[0] = m_pm1->JDarray[jdn1][1] + xjj2 * cos(fwj+0.5*pi);
	pt3[1] = m_pm1->JDarray[jdn1][2] + xjj2 * sin(fwj+0.5*pi);
	pt4[0] = m_pm1->JDarray[jdn1+1][1] + xjj2 * cos(fwj+0.5*pi);
	pt4[1] = m_pm1->JDarray[jdn1+1][2] + xjj2 * sin(fwj+0.5*pi);
	//	  acutPrintf(L"%lf,%lf,%lf,%lf  %lf,%lf,%lf,%lf\n",pt1[1],pt1[0],pt2[1],pt2[0],pt3[1],pt3[0],pt4[1],pt4[0]);
	//�ҽ���

	INTERS(&CurJD->x,&CurJD->y,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);
}

//��STR����At��������Ϊ-1;
int GTZX2_ROAD::FindACHAR(ACHAR *str,ACHAR At)
{
	int num=_tcslen(str);
	ACHAR ch;
	int i;
	for( i=0;i<num;i++)
	{
		ch=str[i];
		if(_tcscmp(&ch,&At)==0)break;
	}
	if(i>=num)i=-1;
	return i;
}

//���뽻������(����ȷ)��ƫ�࣬������������
void GTZX2_ROAD::ModifyNEbyXJJ(double &N, double &E, double xjj)
{
	xlpoint PT;
	PT.lc=PROJ_ML(m_pm1->array,m_pm1->tarray,N,E,N,E);	
	xlpoint_pz(m_pm1->array,m_pm1->tarray,&PT);
	N=PT.x+xjj*cos(PT.a+0.5*PI);
	E=PT.y+xjj*sin(PT.a+0.5*PI);
}

//����һ����̣�ƫ�࣬���ض�Ӧ������
void GTZX2_ROAD::ModifyNEbyXJJ(ACHAR JDLC[],double xjj,double &N,double &E)
{
	xlpoint PT;
	PT.lc=m_pm1->TYLC(JDLC,m_pm1->NDL);	
	xlpoint_pz(m_pm1->array,m_pm1->tarray,&PT);
	N=PT.x+xjj*cos(PT.a+0.5*PI);
	E=PT.y+xjj*sin(PT.a+0.5*PI);	
}

//�ѽ���ͶӰ����Ӧһ�߱��ϣ������¼�Ľ����Ϊ��Ӧһ�߱ߵĺ�һ������
void GTZX2_ROAD::ModifyNEOnBian(DoubleLineData *CurJD)
{
	double inX1,inY1,inX2,inY2,distTo2PointOnLine;

	//��Ӧ�ߺ�һ�������
	int jd2=CurJD->jdno1;
	if(jd2<1||jd2>m_pm1->NJD-1)return;

	//��Ӧ��ǰһ�������
	int jd1=jd2-1;

	//��Ӧ�ߵ�ǰһ�����������
	inX1=m_pm1->JDarray[jd1][1];
	inY1=m_pm1->JDarray[jd1][2];

	//��Ӧ�ߵĺ�һ�����������
	inX2=m_pm1->JDarray[jd2][1];
	inY2=m_pm1->JDarray[jd2][2];

	DistToLine(inX1,inY1,inX2,inY2,CurJD->x,CurJD->y,distTo2PointOnLine);
	double fwj;
	xyddaa(inX1,inY1,inX2,inY2,&fwj);
	CurJD->x+=CurJD->xjj*cos(fwj+0.5*PI);
	CurJD->y+=CurJD->xjj*sin(fwj+0.5*PI);	
}


void GTZX2_ROAD::ModifyRXDName(double N, double E)
{
	assertWriteEnabled();
	int iRXD=GetiRXDFromNE(N,E);
	if(iRXD<0||iRXD>GetRXDNum()-1)return;
	int sdl,edl;
	GetiRXDDLinfo(iRXD,sdl,edl);
	ACHAR ori[256],result[256];
	_stprintf(ori,L"\n���������жε�����(%s):",DLB2x[sdl].RXDName);
	if(acedGetString(0,ori,result))
	{
		for(int i=sdl;i<=edl;i++)
		{
			DLB2x[i].RXDName=result;
		}
	}
}

//�ֳ����תͶӰ���
double GTZX2_ROAD::TYLC2x(CString XCLC2x,int iRXD)//��iRXD��0��ָ�����жβ���
{
	double XLC;
	double TLC=-99999999.0;//ͳһ���
	int sdl,edl;
	ACHAR GH[10],STRXLC[80];
	_tcscpy(STRXLC,XCLC2x);
	split_ckml(STRXLC,&XLC,GH);
	int bz=-99;
	if(iRXD>=0)
	{
		GetiRXDDLinfo(iRXD,sdl,edl);

		for(int i=sdl;i<edl;i++)
		{
			if( XLC>=DLB2x[i].ELC && XLC<=DLB2x[i+1].BLC )  // ���ڸö�����
			{   
				if(bz<0)
				{
					TLC= DLB2x[i].TYLC+(XLC-DLB2x[i].ELC);
					bz=1;
				}// ��һ����
				else 
				{
					bz=2;
					if(_tcscmp(GH,DLB2x[i].BGH)==0)
						TLC= DLB2x[i].TYLC+(XLC-DLB2x[i].ELC);
				}
			}
		}
		if(TLC>-99999999.0)return TLC;
	}
	else if (iRXD==-2)
	{
		ACHAR tmp[256];
		_tcscpy(tmp,XCLC2x);
		double tylc1x=m_pm1->TYLC1(tmp,m_pm1->NDL,m_pm1->DLB);
		double tylc2x,xjj,ResPt[2];
		m_pm1->CalXjj(tylc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,tylc2x,ResPt);
		return tylc2x;
	}
	else//��ָ�����жλ�ָ������
	{
		int NRxd=GetRXDNum();
		for(iRXD=0;iRXD<NRxd;iRXD++)
		{
			GetiRXDDLinfo(iRXD,sdl,edl);

			for(int i=sdl;i<edl;i++)
			{
				if( XLC>=DLB2x[i].ELC && XLC<=DLB2x[i+1].BLC )  // ���ڸö�����
				{   
					if(bz<0)
					{
						TLC= DLB2x[i].TYLC+(XLC-DLB2x[i].ELC);
						bz=1;
					}// ��һ����
					else 
					{
						bz=2;
						if(_tcscmp(GH,DLB2x[i].BGH)==0)
							TLC= DLB2x[i].TYLC+(XLC-DLB2x[i].ELC);
					}
				}
			}
			if(TLC>-99999999.0)return TLC;			
		}
	}
	//һ�������
	ACHAR tmp[256];
	_tcscpy(tmp,XCLC2x);
	double tylc1x=m_pm1->TYLC1(tmp,m_pm1->NDL,m_pm1->DLB);
	double tylc2x,xjj,ResPt[2];
	m_pm1->CalXjj(tylc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,tylc2x,ResPt);
	return tylc2x;
}

bool GTZX2_ROAD::TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml;
	double cml = PROJ_ML(array, tarray, N, E, N, E);
	dml = XCLC(cml, GH, DLB2x, NDL);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
	return true;
}

bool GTZX2_ROAD::TrsNEtoProjml(double N, double E, double& Projml)//�����������
{
	Projml = PROJ_ML(array, tarray, N, E, N, E);

	return true;
}

bool GTZX2_ROAD::TrsNEtoCml(double N, double E, double& Cml)
{
	ACHAR GH[8],Ckml[20];
	double dml;
	double cml = PROJ_ML(array, tarray, N, E, N, E);
	dml = XCLC(cml, GH, DLB2x, NDL);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
	Cml = TYLC1(Ckml, m_2XGTDLSum, DLB);

	return true;
}

bool GTZX2_ROAD::TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ)
{
	double Projml = TYLC2x(Ckml);
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, &PZ);

	return true;
}
bool GTZX2_ROAD::TrsCkmltoProjml(ACHAR Ckml[], double& Projml)
{
	Projml = TYLC2x(Ckml);

	return true;
}


bool GTZX2_ROAD::TrsCkmltoCml(ACHAR Ckml[], double& Cml)
{
	Cml = TYLC1(Ckml, m_2XGTDLSum, DLB);

	return true;
}

bool GTZX2_ROAD::TrsCmltoNE(double Cml, xlpoint& PZ)
{
	ACHAR Ckml[20];
	TrsCmltoCkml(Cml, Ckml);
	TrsCkmltoNE(Ckml, PZ);

	return true;

}
bool GTZX2_ROAD::TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml = 0.0;
	dml = XLC1(Cml, GH, m_2XGTDLSum, DLB);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));

	return true;

}
bool GTZX2_ROAD::TrsCmltoProjml(double Cml, double& Projml)
{
	xlpoint PZ;
	TrsCmltoNE(Cml, PZ);
	Projml = PROJ_ML(array, tarray, PZ.x, PZ.y, PZ.x, PZ.y);

	return true;
}
bool GTZX2_ROAD::TrsProjmltoNE(double Projml, xlpoint& PZ)
{
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, &PZ);

	return true;
}
bool GTZX2_ROAD::TrsProjmltoCml(double Projml, double& Cml)
{
	xlpoint PZ;
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, &PZ);
	TrsNEtoCml(PZ.x, PZ.y, Cml);

	return true;

}
bool GTZX2_ROAD::TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC)
{
	xlpoint PZ;
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, &PZ);
	TrsNEtoCkml(PZ.x, PZ.y, Ckml, NLC);

	return true;

}

void GTZX2_ROAD::SetiRXDBPDArray(int iRXD, int NPD, BAS_DRAW_FUN::PDTAB* BPDArr, bool IsAdd)
{
	PDTAB TempPDB;
	if (IsAdd)
	{
		for (int i=0; i<NPD; i++)
		{
			_tcscpy(TempPDB.CKml,BPDArr[i].CKml);
			TempPDB.GH=BPDArr[i].GH;
			TempPDB.cml=BPDArr[i].cml;
			TempPDB.degree=BPDArr[i].degree;
			TempPDB.length=BPDArr[i].length;
			TempPDB.Level=BPDArr[i].Level;
			TempPDB.ml=BPDArr[i].ml;
			TempPDB.Notes=BPDArr[i].Notes;
			TempPDB.RLen=BPDArr[i].RLen;
			TempPDB.Rshu=BPDArr[i].Rshu;
			AllRXDBPDArray[iRXD].Add(TempPDB);
		}

	}
	else
	{
		AllRXDBPDArray[iRXD].RemoveAll();
		AllRXDBPDArray[iRXD].SetSize(NPD);
		for (int i=0; i<NPD; i++)
		{
			_tcscpy(TempPDB.CKml,BPDArr[i].CKml);
			TempPDB.GH=BPDArr[i].GH;
			TempPDB.cml=BPDArr[i].cml;
			TempPDB.degree=BPDArr[i].degree;
			TempPDB.length=BPDArr[i].length;
			TempPDB.Level=BPDArr[i].Level;
			TempPDB.ml=BPDArr[i].ml;

			TempPDB.Notes=BPDArr[i].Notes;
			TempPDB.RLen=BPDArr[i].RLen;
			TempPDB.Rshu=BPDArr[i].Rshu;
			AllRXDBPDArray[iRXD][i] = TempPDB;
		}
	}
}

void GTZX2_ROAD::SetiRXDBPDArray(int iRXD, int NPD, PDTAB* BPDArr, bool IsAdd)
{
	if (IsAdd)
	{
		for (int i=0; i<NPD; i++)
			AllRXDBPDArray[iRXD].Add(BPDArr[i]);
	}
	else
	{
		AllRXDBPDArray[iRXD].RemoveAll();
		AllRXDBPDArray[iRXD].SetSize(NPD);
		for (int i=0; i<NPD; i++)
		{
			AllRXDBPDArray[iRXD][i] = BPDArr[i];
		}
	}
}
void GTZX2_ROAD::Create2XGTBPDArray()
{
	int RXDSum = AllRXDBPDArray.GetSize();
	PDTAB TempPDB;
	xlpoint PZ;
	double Projml2X = 0.0;
	ACHAR GH[8],Ckml[20];
	m_BPDArray.RemoveAll();
	if (RXDSum>0)
	{
		int BPD1xSum = m_pm1->m_BPDArray.GetSize();
		int iBPD1x=0;
		int SDLNo,EDLNo;
		SDLNo = EDLNo = -1;
		int iRXD = 0;
		GetiRXDDLinfo(iRXD, SDLNo, EDLNo);

		while (iRXD<RXDSum && iBPD1x<BPD1xSum)
		{
			m_pm1->TrsCkmltoNE(m_pm1->m_BPDArray[iBPD1x].CKml, PZ);
			TrsNEtoProjml(PZ.x, PZ.y, Projml2X);
			if (Projml2X<0.0)
				continue;

			if (Projml2X < DLB2x[SDLNo].TYLC-0.01)
			{
				TempPDB = m_pm1->m_BPDArray[iBPD1x];
				TrsCkmltoCml(TempPDB.CKml, TempPDB.cml);
				m_BPDArray.Add(TempPDB);
			}
			if (Projml2X > DLB2x[EDLNo].TYLC+0.01 || (Projml2X < DLB2x[EDLNo].TYLC-0.01 && iBPD1x==BPD1xSum-1))
			{
				int RXDBPDSum = AllRXDBPDArray[iRXD].GetSize();
				if (RXDBPDSum > 0)
				{
					for (int iRXDBPD=0; iRXDBPD<RXDBPDSum; iRXDBPD++)
					{
						_tcscpy(GH, AllRXDBPDArray[iRXD][iRXDBPD].GH);
						_tcscpy(Ckml, LCchr(GH, AllRXDBPDArray[iRXD][iRXDBPD].ml, 3, 1));
						_tcscpy(AllRXDBPDArray[iRXD][iRXDBPD].CKml, Ckml);
						TrsCkmltoCml(AllRXDBPDArray[iRXD][iRXDBPD].CKml, AllRXDBPDArray[iRXD][iRXDBPD].cml);
						m_BPDArray.Add(AllRXDBPDArray[iRXD][iRXDBPD]);
					}
				}

				iRXD++;
				if (iRXD==RXDSum)//���һ�����еĺ�Ļ��߶���
				{
					for (int iBXBPD=iBPD1x; iBXBPD<BPD1xSum; iBXBPD++)
					{
						TempPDB = m_pm1->m_BPDArray[iBXBPD];
						TrsCkmltoCml(TempPDB.CKml, TempPDB.cml);
						m_BPDArray.Add(TempPDB);
					}
					break;
				}
				else
					GetiRXDDLinfo(iRXD, SDLNo, EDLNo);
			}
			iBPD1x++;
		}
	}

	else
	{
		m_BPDArray.Append(m_pm1->m_BPDArray);
		for (int i=0; i<m_BPDArray.GetSize(); i++)
		{
			_tcscpy(GH, m_BPDArray[i].GH);
			_tcscpy(Ckml, LCchr(GH, m_BPDArray[i].ml, 3, 1));
			_tcscpy(m_BPDArray[i].CKml, Ckml);
			TrsCkmltoCml(m_BPDArray[i].CKml, m_BPDArray[i].cml);
		}
	}


	int NBPD = m_BPDArray.GetSize();
	if (NBPD<1)
	{
		m_NCM = 0;
		return;
	}
	double BPD_array[MAXBPDNUM][5];
	for (int i=0; i<NBPD; i++)
	{
		BPD_array[i][0] = m_BPDArray[i].cml;//�������
		BPD_array[i][1] = m_BPDArray[i].Level;	//�����¶�			
		BPD_array[i][2] = m_BPDArray[i].Rshu;//�����߰뾶
		BPD_array[i][3] = 0;
		BPD_array[i][4] = 0;
	}

	double LC1,H1,LC2,H2,R;
	//CM = new double[(NBPD+1)*5];
	LC1= BPD_array[0][0];
	H1=  BPD_array[0][1];  
	m_CM[0] =LC1,m_CM[1]=H1,m_CM[2]=0.0, m_CM[3] = 0.0, m_CM[4]=0.0;

	for(int i=1; i<NBPD; i++)
	{
		LC2= BPD_array[i][0];
		H2=BPD_array[i][1];    
		R =BPD_array[i][2];
		m_CM[i*5]=LC2,m_CM[i*5+1]=H2;
		m_CM[i*5+2]=(H2-H1)/(LC2-LC1);
		m_CM[i*5+3]=LC2-LC1;
		m_CM[i*5+4]=R;
		H1=H2,LC1=LC2;
	}
	m_CM[NBPD*5] =m_CM[NBPD*5+1]=m_CM[NBPD*5+2]=m_CM[NBPD*5+3]=m_CM[NBPD*5+4]=0.0;
	m_NCM = NBPD+1;

}

GTZX_ROAD* GTZX2_ROAD::GetiRXDGTZX(int irxd)
{
	GTZX_ROAD* pGTZXRXD = NULL;
	double arrayd[MAXJDNUM][6];
	int njd;
	int i,sDLno,eDLno;//���ն������
	GetiRXDJDArray(irxd,arrayd,njd);//

	pGTZXRXD = new GTZX_ROAD(njd,arrayd,L"AK",L"JD");//���ߵ�i�����ж�
	GetiRXDDLinfo(irxd,sDLno,eDLno);
	for(i=sDLno; i<=eDLno; i++)
	{
		ACHAR bgh[30],egh[30];
		_tcscpy(bgh,DLB2x[i].BGH);
		_tcscpy(egh,DLB2x[i].EGH);
		pGTZXRXD->setDLM(i-sDLno,DLB2x[i].BLC,DLB2x[i].ELC-DLB2x[i].BLC,bgh,egh);						
	}
	pGTZXRXD->m_BPDArray.RemoveAll();
	if (AllRXDBPDArray[irxd].GetSize()>0)
	{
		pGTZXRXD->m_BPDArray.Append(AllRXDBPDArray[irxd]);
		pGTZXRXD->TrsBPDArraytoCM();
	}
	return pGTZXRXD;

}

bool GTZX2_ROAD::BZCM(AcGiWorldDraw* pWd)
{
	xlpoint pt;
	AcGePoint3d cenPT,BefPt,AftPt,InterPt,InterPt2,BefSlopePt,AftSlopePt,MLPt;
	ACHAR Ckml[20];
	ACHAR BefLen[10];
	ACHAR AftLen[10];
	ACHAR BefSlope[10];
	ACHAR AftSlope[10];
	ACHAR Lever[10];
	int LorR = 1;
	double YL = 50;
	double XL = 50;
	int Color = 0;
	double CkmlAng;
	double LeverAng;
	double BefSlopeAng;
	double AftSlopeAng;
	bool isBefSlopeUp=false;
	bool isAftSlopeUp=false;


	for (int i=0; i< m_NCM-1;i++)
	{

		TrsCmltoNE(m_CM[i*5], pt);
		TrsCmltoCkml(m_CM[i*5], Ckml, m_Xsw);
		_stprintf(Lever,L"%0.3lf",m_CM[i*5+1]);
		_stprintf(BefLen,L"%0.3lf",m_CM[i*5+3]);		
		_stprintf(AftLen,L"%0.3lf",m_CM[(i+1)*5+3]);
		_stprintf(BefSlope,L"%0.2lf��",1000*m_CM[i*5+2]);
		_stprintf(AftSlope,L"%0.2lf��",1000*m_CM[(i+1)*5+2]);
		if (fabs(m_CM[i*5+1]-(long int)(m_CM[i*5+1]))<0.001)
			_stprintf(Lever,L"%.0lf",m_CM[i*5+1]);
		else
			_stprintf(Lever,L"%.2lf",m_CM[i*5+1]);
		if (fabs(m_CM[i*5+3]-(long int)(m_CM[i*5+3]))<0.001)
			_stprintf(BefLen,L"%.0lf",m_CM[i*5+3]);
		else
			_stprintf(BefLen,L"%.2lf",m_CM[i*5+3]);
		if (fabs(m_CM[(i+1)*5+3]-(long int)(m_CM[(i+1)*5+3]))<0.001)
			_stprintf(AftLen,L"%.0lf",m_CM[(i+1)*5+3]);
		else
			_stprintf(AftLen,L"%.2lf",m_CM[(i+1)*5+3]);
		if (fabs(m_CM[i*5+2]-(long int)(m_CM[i*5+2]))<0.001)
			_stprintf(BefSlope,L"%.0lf��",1000*m_CM[i*5+2]);
		else
			_stprintf(BefSlope,L"%.1lf��",1000*m_CM[i*5+2]);
		if (fabs(m_CM[(i+1)*5+2]-(long int)(m_CM[(i+1)*5+2]))<0.001)
			_stprintf(AftSlope,L"%.0lf��",1000*m_CM[(i+1)*5+2]);
		else
			_stprintf(AftSlope,L"%.1lf��",1000*m_CM[(i+1)*5+2]);
		if (m_CM[i*5+2]>0.001)
		{
			isBefSlopeUp=true;
		}
		if (m_CM[(i+1)*5+2]>0.001)
		{
			isAftSlopeUp=true;
		}
		cenPT[X]=pt.y;
		cenPT[Y]=pt.x;
		InterPt[X] = cenPT[X] + 0.5*YL * sin(pt.a + LorR*0.5*PI);
		InterPt[Y] = cenPT[Y] + 0.5*YL * cos(pt.a + LorR*0.5*PI);
		InterPt2[X]=cenPT[X] + YL * sin(pt.a + LorR*0.5*PI);
		InterPt2[Y]=cenPT[Y] + YL * cos(pt.a + LorR*0.5*PI);
		BefPt[X] = InterPt[X] - XL * sin(pt.a);
		BefPt[Y] = InterPt[Y] - XL * cos(pt.a);
		AftPt[X] = InterPt[X] + XL * sin(pt.a);
		AftPt[Y] = InterPt[Y] + XL * cos(pt.a);
		if (isBefSlopeUp)
		{
			BefSlopePt[X] = BefPt[X] + 0.5*YL * sin(pt.a - LorR*0.5*PI);
			BefSlopePt[Y] = BefPt[Y] + 0.5*YL * cos(pt.a - LorR*0.5*PI);
		}
		else
		{
			BefSlopePt[X] = BefPt[X] - 0.5*YL * sin(pt.a - LorR*0.5*PI);
			BefSlopePt[Y] = BefPt[Y] - 0.5*YL * cos(pt.a - LorR*0.5*PI);
		}
		if (isAftSlopeUp)
		{
			AftSlopePt[X] = AftPt[X] - 0.5*YL * sin(pt.a - LorR*0.5*PI);
			AftSlopePt[Y] = AftPt[Y] - 0.5*YL * cos(pt.a - LorR*0.5*PI);
		} 
		else
		{
			AftSlopePt[X] = AftPt[X] + 0.5*YL * sin(pt.a - LorR*0.5*PI);
			AftSlopePt[Y] = AftPt[Y] + 0.5*YL * cos(pt.a - LorR*0.5*PI);
		}
		MLPt[X] = cenPT[X] + 5 * sin(pt.a + LorR*0.5*PI);
		MLPt[Y] = cenPT[Y] + 5 * cos(pt.a + LorR*0.5*PI);


		G_makeline(pWd, cenPT, InterPt2, Color);
		G_makeline(pWd, InterPt, BefPt, Color);
		G_makeline(pWd, InterPt, AftPt, Color);
		if (i>0)
			G_makeline(pWd, InterPt, BefSlopePt, Color);
		if (i<m_NCM-2)
			G_makeline(pWd, InterPt, AftSlopePt, Color);


		ads_point Spt,Ept;

		if (LorR==-1)
		{
			Spt[0] = BefSlopePt[X];
			Spt[1] = BefSlopePt[Y];
			Ept[0] = InterPt[X];
			Ept[1] = InterPt[Y];
			BefSlopeAng = ads_angle(Spt,Ept);
			Spt[0] = InterPt[X];
			Spt[1] = InterPt[Y];
			Ept[0] = AftSlopePt[X];
			Ept[1] = AftSlopePt[Y];
			AftSlopeAng = ads_angle(Spt,Ept);

			CkmlAng = FwjtoAngX(pt.a -0.5*PI);
			G_maketext(pWd, MLPt, Ckml, CkmlAng, texth, Color, 1);

			LeverAng = FwjtoAngX(pt.a);
			G_maketext(pWd, InterPt2, Lever, LeverAng, texth, Color, 0);

			if (i>0)
			{
				G_maketext(pWd, BefSlopePt, BefSlope, BefSlopeAng, texth, Color, 1);
				G_maketext(pWd, BefSlopePt, BefLen, BefSlopeAng, texth, Color, 3);
			}
			if (i<m_NCM-2)           
			{
				G_maketext(pWd, AftSlopePt, AftSlope, AftSlopeAng, texth, Color, 2);
				G_maketext(pWd, AftSlopePt, AftLen, AftSlopeAng, texth, Color, 3);
			}
		}
		else
		{
			Spt[0] = BefSlopePt[X];
			Spt[1] = BefSlopePt[Y];
			Ept[0] = InterPt[X];
			Ept[1] = InterPt[Y];
			BefSlopeAng = ads_angle(Ept,Spt);
			Spt[0] = InterPt[X];
			Spt[1] = InterPt[Y];
			Ept[0] = AftSlopePt[X];
			Ept[1] = AftSlopePt[Y];
			AftSlopeAng = ads_angle(Ept,Spt);

			CkmlAng = FwjtoAngX(pt.a -0.5*PI);
			G_maketext(pWd, MLPt, Ckml, CkmlAng, texth, Color, 2);

			LeverAng = FwjtoAngX(pt.a + PI);
			G_maketext(pWd, InterPt2, Lever, LeverAng, texth, Color, 0);

			if (i>0)
			{
				G_maketext(pWd, BefSlopePt, BefSlope, BefSlopeAng, texth, Color, 2);
				G_maketext(pWd, BefSlopePt, BefLen, BefSlopeAng, texth, Color, 3);
			}
			if (i<m_NCM-2)
			{
				G_maketext(pWd, AftSlopePt, AftSlope, AftSlopeAng, texth, Color, 1);
				G_maketext(pWd, AftSlopePt, AftLen, AftSlopeAng, texth, Color, 3);
			}

		}
		BZSHUQX(pWd,i,LorR);
	}
	return true;

}

//���ɶ��߹�ͨ������
void GTZX2_ROAD::Create2XGTDLB()
{
	m_2XGTDLSum = 0;
	CArray<DLBZ,DLBZ> DLBArray;
	DLBZ TempDLB;
	int NDL1X = m_pm1->NDL;
	ACHAR ckml[20];
	double Projml1X,Projml2X;
	int RXDSum = GetRXDNum();
	xlpoint PZ;
	int iRXD = 0;
	int iDL = 0;
	int SDLNo = -1;
	int EDLNo = -1;

	if (RXDSum>0)
	{
		GetiRXDDLinfo(0, SDLNo, EDLNo);

		while (iRXD<RXDSum && iDL<NDL1X)
		{
			_tcscpy(ckml, LCchr(m_pm1->DLB[iDL].BGH, m_pm1->DLB[iDL].BLC, 3, 1));
			m_pm1->TrsCkmltoNE(ckml, PZ);
			TrsNEtoProjml(PZ.x, PZ.y, Projml2X);


			if (Projml2X < DLB2x[SDLNo].TYLC-0.01)
			{
				DLBArray.Add(m_pm1->DLB[iDL]);
			}
			if (Projml2X > DLB2x[EDLNo].TYLC+0.01)
			{
				for (int iRXDDL=SDLNo; iRXDDL<EDLNo+1; iRXDDL++)
				{
					_tcscpy(TempDLB.BGH, DLB2x[iRXDDL].BGH);
					_tcscpy(TempDLB.EGH, DLB2x[iRXDDL].EGH);
					TempDLB.BLC = DLB2x[iRXDDL].BLC;
					TempDLB.DL = DLB2x[iRXDDL].ELC - DLB2x[iRXDDL].BLC;
					DLBArray.Add(TempDLB);
				}

				iRXD++;
				if (iRXD==RXDSum)//���һ�����еĺ�Ļ��߶���
				{
					for (int i=iDL; i<NDL1X; i++)
					{
						DLBArray.Add(m_pm1->DLB[i]);
					}
					break;
				}
				else
					GetiRXDDLinfo(iRXD, SDLNo, EDLNo);
			}
			iDL++;
		}

		m_2XGTDLSum = DLBArray.GetSize();
		for (iDL=0; iDL<m_2XGTDLSum; iDL++)
		{
			DLB[iDL] = DLBArray[iDL];
		}
	}

	else
	{
		m_2XGTDLSum = m_pm1->NDL;
		for (iDL=0; iDL<m_pm1->NDL; iDL++)
		{
			DLB[iDL] = m_pm1->DLB[iDL];
		}
	}

}

void GTZX2_ROAD::CheckAllXLConstraint()
{
	//m_ConstraintErrArray.RemoveAll();
	//CString CurXLNme = m_XLRelate.CurXLNme;
	//if (CurXLNme.GetLength()<5)
	//	return;
	//CString ProStr1 = CurXLNme.Mid(0, CurXLNme.GetLength()-8);
	//CString ProStr2 = GetProByMdbName(mdbname);
	////if (ProStr1 != ProStr2)
	////{
	////	AfxMessageBox(mdbname + "�����/������������·�Ĺ�ϵ��������");
	////	return;
	////}

	//int InsertXLSum = m_XLRelate.InterXLSum;
	//CString InsertXLMdbName;
	//CString JXorEX;
	//CString ProStr;
	//double MinAng = 0.0;
	//double MinH = 0.0;
	//for (int iXL=0; iXL<InsertXLSum; iXL++)
	//{
	//	InsertXLMdbName = m_XLRelate.InterXLNameArray[iXL].InterXLName;
	//	MinAng = m_XLRelate.InterXLNameArray[iXL].JJlowLimt;
	//	MinH = m_XLRelate.InterXLNameArray[iXL].GCmin;
	//	JXorEX = InsertXLMdbName.Mid(InsertXLMdbName.GetLength()-8, 8);
	//	ProStr = InsertXLMdbName.Mid(0, InsertXLMdbName.GetLength()-8);
	//	InsertXLMdbName = Cworkdir + "\\DATA\\" + ProStr + ".mdb";
	//	CArray<ConstraintErr,ConstraintErr> OneXLErrorArray;
	//	if (JXorEX=="���л���")
	//	{
	//		GTZX_JYX* pXLPM = PmPointer::GetJYJX(InsertXLMdbName);
	//		if (pXLPM)
	//		{
	//			PmPointer::CheckOneXLConstraint(this, pXLPM, MinAng, MinH, OneXLErrorArray);
	//		}
	//		else
	//			continue;
	//	}
	//	else if (JXorEX == "��������")
	//	{
	//		GTZX2_JYX* pXLPM = PmPointer::GetJYEX(InsertXLMdbName,-1);
	//		if (pXLPM)
	//		{
	//			PmPointer::CheckOneXLConstraint(this, pXLPM, MinAng, MinH, OneXLErrorArray);
	//		}
	//		else
	//			continue;
	//	}
	//	else if (JXorEX == "��������")
	//	{
	//		GTZX2_JYX* pXLPM = PmPointer::GetJYEX(InsertXLMdbName,1);
	//		if (pXLPM)
	//		{
	//			PmPointer::CheckOneXLConstraint(this, pXLPM, MinAng, MinH, OneXLErrorArray);
	//		}
	//		else
	//			continue;
	//	}
	//	else if (JXorEX == "��ƻ���")
	//	{
	//		GTZX* pXLPM = PmPointer::GetSJJX(InsertXLMdbName);
	//		if (pXLPM)
	//		{
	//			PmPointer::CheckOneXLConstraint(this, pXLPM, MinAng, MinH, OneXLErrorArray);
	//		}
	//		else
	//			continue;
	//	}
	//	else if (JXorEX == "�������")
	//	{
	//		GTZX2* pXLPM = PmPointer::GetSJEX(InsertXLMdbName,-1);
	//		if (pXLPM)
	//		{
	//			PmPointer::CheckOneXLConstraint(this, pXLPM, MinAng, MinH, OneXLErrorArray);
	//		}
	//		else
	//			continue;
	//	}
	//	else if (JXorEX == "�������")
	//	{
	//		GTZX2* pXLPM = PmPointer::GetSJEX(InsertXLMdbName,1);
	//		if (pXLPM)
	//		{
	//			PmPointer::CheckOneXLConstraint(this, pXLPM, MinAng, MinH, OneXLErrorArray);
	//		}
	//		else
	//			continue;
	//	}
	//	else
	//		continue;

	//	//ads_printf(L"jiaodianshu%d\n",OneXLErrorArray.GetSize());
	//	int munum=OneXLErrorArray.GetSize();
	//	if (OneXLErrorArray.GetSize()>0)
	//	{
	//		m_ConstraintErrArray.Append(OneXLErrorArray);
	//	}
	//}
}