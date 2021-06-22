// GTZX2.cpp: implementation of the GTZX2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTZX2_JYX.h"
#include "PmPointer.h"
#include "UltraCArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//extern GuiFanCanShu mJYGuiFanCS;
//extern GuiFanCanShu mGJGuiFanCS;
//#include "Cmymonitor.h"

GTZX_JYX_ROAD *pGtzxTemp = NULL ;  //��ʱһ��ָ��,��Ҫɾ��
extern GTZX_JYX_ROAD* pGTEXRXGJD;
//ACRX_DXF_DEFINE_MEMBERS(GTZX, AcDbEntity,  0,  GTZX,L"GUADS");
int MAKE_ACDBOPENOBJECT_FUNCTION(GTZX2_JYX_ROAD);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//���ڼ����ͨ��ƻ��ߵļ�����ָ��
extern GTZX_JYX_ROAD  * JiXianpGtzx ;
GTZX2_JYX_ROAD *ZuoXianpGtzx , * YouXianpGtzx;


//��֪��������
GTZX2_JYX_ROAD::GTZX2_JYX_ROAD(GTZX_JYX_ROAD *pm1,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[] , BOOL IsGaiJian )
	//һ�����ݣ����߽����������߽���ںţ���ʼ������
{
	AllColor = -1;
	if (pm1->XLShuXing == "��ͨ��ƻ���")
	{
		_tcscpy(xlname, L"��ͨ��Ƣ���");
	}
	m_IsNeedCalGJDInf = false;
	isJYGuiFanCS = !IsGaiJian;
	//if(isJYGuiFanCS) mGuiFanCS = mJYGuiFanCS;
	//else mGuiFanCS = mGJGuiFanCS;
	m_GuiFanCS = pm1->m_GuiFanCS;

	//��ʼ��
	calsflag =true;

	GJDInfTab.RemoveAll();

	JDarray.RemoveAll();
	JDarray.SetSize(pm1->JDarray.GetSize());


	QXB.RemoveAll();
	int i=0;
	for (i=0;i<100;i++)
	{   
		//	for ( j=0;j<6;j++) JDarray[i][j]=0.0;
		//	for (  j=0;j<9;j++)   QXB[i][j]=0.0;
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
	JD0 = jd2[0].jdno;
	NLC = 2;
	_tcscpy(JDGH,jdgh);
	for(i=0; i<NJD; i++)
	{
		JD2[i].JDXZ = jd2[i].JDXZ;//����......
		JD2[i].jdno1 = jd2[i].jdno1/*-pm1->JD0*/;//��Ӧһ�߽������(���ǽ����)

		//_tcscpy( JD2[i].JDNum , jd2[i].JDNum ) ;// ���߽����
		//JD2[i].jdno = jd2[i].jdno;// ���߽����
		JD2[i].jdno = jd2[i].jdno;// ���߽����

		JD2[i].xjj =  jd2[i].xjj;	
		JD2[i].x =  jd2[i].x;	
		JD2[i].y =  jd2[i].y;	
		JD2[i].R =  jd2[i].R;	
		JD2[i].Lo1 =  jd2[i].Lo1;	
		JD2[i].Lo2 =  jd2[i].Lo2;	
		JD2[i].NYDL =  jd2[i].NYDL;	
		JD2[i].qxjk =  jd2[i].qxjk;

		_tcscpy( JD2[i].JDNum , jd2[i].JDNum ) ;

		//_tcscpy( JD2[i].JDNum1x, jd2[i].JDNum1x);
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

	NDL=0;

	for(i=0; i<NJD; i++)
		_tcscpy(JD2[i].ZHLC,m_pm1->XLC(QXB[i].KZH));

	//���ǹ�ͨ���ߣ�����������ҵ����������ȸĽ�����Ϣ�����ɺ����
	if (_tcscmp(xlname, L"��ͨ��Ƣ���")!=0)
		CalNYDL();


	m_pm2 = NULL;

	IsReSetColor = FALSE ;

	RTolCheckUp( IsGaiJian );


	xlpoint PZtmp;
	//	PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+4.0 * xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(���л���)")==0)
		PZtmp.lc=array_pxy[0][0][4]+ 2.0 * xl_length/5.0;  // ͳһ���

	xlpoint_pz(array,tarray,&PZtmp);
	m_XLNameNotePt[X]=PZtmp.y+xl_length*sin(PZtmp.a+pi*0.5)/8.0;
	m_XLNameNotePt[Y]=PZtmp.x+xl_length*cos(PZtmp.a+pi*0.5)/8.0;

	m_BPDArray.RemoveAll();
	m_NCM = 0;
	//	NGJDFDInfTab = 0; //�Ľ��ηֶ���

}

/////////////////////////////////////////////////////////
GTZX2_JYX_ROAD::GTZX2_JYX_ROAD(GTZX2_JYX_ROAD *pm2,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[] , BOOL IsGaiJian )
	//һ�����ݣ����߽����������߽���ںţ���ʼ������
{
	GTZX_JYX_ROAD *pm1 = NULL ;
	pm1 = pm2->CreateGtzx();
	AllColor = -1;
	if (_tcscmp(pm2->xlname, L"��ͨ��ƻ���")==0)
	{
		_tcscpy(xlname, L"��ͨ��Ƣ���");
	}

	m_IsNeedCalGJDInf = false;
	isJYGuiFanCS = !IsGaiJian;
	//if(isJYGuiFanCS) mGuiFanCS = mJYGuiFanCS;
	//else mGuiFanCS = mGJGuiFanCS;
	m_GuiFanCS = pm2->m_GuiFanCS;

	//��ʼ��
	calsflag =true;

	GJDInfTab.RemoveAll();

	JDarray.RemoveAll();
	JDarray.SetSize(pm1->JDarray.GetSize());


	QXB.RemoveAll();
	int i=0;
	for (i=0;i<100;i++)
	{   
		//	for ( j=0;j<6;j++) JDarray[i][j]=0.0;
		//	for (  j=0;j<9;j++)   QXB[i][j]=0.0;
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
	for (i=0;i<30;i++) 
	{   
		DLB[i].DL=0.0; 
		DLB[i].BLC=0.0;

		_tcscpy(DLB[i].BGH,L"\0"); 
		_tcscpy(DLB[i].EGH,L"\0");		
	};

	//��ֵ	
	m_pm1=pm1;//һ��ƽ��
	texth=m_pm1->texth;
	GetT0min_Lrmin();

	NJD = njd2;
	JD0 = jd2[0].jdno;
	NLC = 2;
	_tcscpy(JDGH,jdgh);

	for(i=0; i<NJD; i++)
	{
		JD2[i].JDXZ = jd2[i].JDXZ;//����......
		JD2[i].jdno1 = jd2[i].jdno1/*-pm1->JD0*/;//��Ӧһ�߽������(���ǽ����)
		JD2[i].jdno = jd2[i].jdno;// ���߽����
		JD2[i].xjj =  jd2[i].xjj;	
		JD2[i].x =  jd2[i].x;	
		JD2[i].y =  jd2[i].y;	
		JD2[i].R =  jd2[i].R;	
		JD2[i].Lo1 =  jd2[i].Lo1;	
		JD2[i].Lo2 =  jd2[i].Lo2;	
		JD2[i].NYDL =  jd2[i].NYDL;	
		JD2[i].qxjk =  jd2[i].qxjk;	
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
	NDL=0;
	for(i=0; i<NJD; i++)
		_tcscpy(JD2[i].ZHLC,m_pm1->XLC(QXB[i].KZH));
	CalNYDL();	

	m_pm2 = pm2;

	IsReSetColor = FALSE ;

	RTolCheckUp( IsGaiJian );

	xlpoint PZtmp;
	//	PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+4.0 * xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(���л���)")==0)
		PZtmp.lc=array_pxy[0][0][4]+ 2.0 * xl_length/5.0;  // ͳһ���

	xlpoint_pz(array,tarray,&PZtmp);
	m_XLNameNotePt[X]=PZtmp.y+xl_length*sin(PZtmp.a+pi*0.5)/8.0;
	m_XLNameNotePt[Y]=PZtmp.x+xl_length*cos(PZtmp.a+pi*0.5)/8.0;
	//	NGJDFDInfTab = 0;//�Ľ��ηֶ���
	m_BPDArray.RemoveAll();
	m_NCM = 0;
}



/////////////////////////////////////////////////////////
GTZX2_JYX_ROAD::GTZX2_JYX_ROAD()
{	
	AllColor = -1;
	calsflag =true;	
	m_IsNeedCalGJDInf = false;
	//	m_pm1=NULL;
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

	isGJD = FALSE;
	/*	
	GetT0min_Lrmin();
	CalJDdata();
	SetJDdata();
	getpxy_DATA();
	CalNYDL();  
	GetDLTYLC();*/

	m_pm2= NULL;
	IsReSetColor = FALSE ;
	//	NGJDFDInfTab = 0;//�Ľ��ηֶ���
	m_BPDArray.RemoveAll();
	m_NCM = 0;
}

GTZX2_JYX_ROAD::~GTZX2_JYX_ROAD()
{

}

GTZX2_JYX_ROAD::GTZX2_JYX_ROAD(AcDbObjectId Id1x, double xjj ,BOOL IsGaiJian )//plh04.30
	//xjj=-1,��������,xjj=1,��������
{
	AllColor = -1;
	isJYGuiFanCS = !IsGaiJian;
	//if(isJYGuiFanCS) mGuiFanCS = mJYGuiFanCS;
	//else mGuiFanCS = mGJGuiFanCS;

	m_IsNeedCalGJDInf = false;
	calsflag =true;
	/*acdbOpenObject(m_pm1,Id1x,AcDb::kForRead);*/
	if(acdbOpenObject(m_pm1, Id1x, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	m_pm1->close();
	m_GuiFanCS = m_pm1->m_GuiFanCS;
	NJD=m_pm1->NJD;
	JD0=m_pm1->JD0;
	GetT0min_Lrmin();

	ACHAR JDGH2x[8];
	if( fabs( xjj)< 0.001 )
	{
		_tcscpy( JDGH2x , m_pm1->JDGH );
	}
	else if( xjj > 0.001 )
	{
		_tcscpy( JDGH2x , L"YJD" );
	}
	else 
	{
		_tcscpy( JDGH2x , L"ZJD" );
	}

	CString sTmp;
	for(int i=0;i<NJD;i++)
	{
		JD2[i].JDXZ=B_X;
		JD2[i].jdno=i;
		JD2[i].jdno1=i;//��Ӧһ�߽������;
		JD2[i].xjj=xjj;
		sTmp = m_pm1->JDarray[i].JDNum;
		sTmp.Replace(m_pm1->JDGH,L"");
		_stprintf( JD2[i].JDNum , L"%s%s" ,JDGH2x ,sTmp);

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
	NDL=0;
	Create2XDLB();//plh05.10
	m_pm2 = NULL;

	IsReSetColor = FALSE ;


	RTolCheckUp( IsGaiJian );
	xlpoint PZtmp;
	//	PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+4.0 * xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(���л���)")==0)
		PZtmp.lc=array_pxy[0][0][4]+ 2.0 * xl_length/5.0;  // ͳһ���

	xlpoint_pz(array,tarray,&PZtmp);
	m_XLNameNotePt[X]=PZtmp.y+xl_length*sin(PZtmp.a+pi*0.5)/8.0;
	m_XLNameNotePt[Y]=PZtmp.x+xl_length*cos(PZtmp.a+pi*0.5)/8.0;
	//	NGJDFDInfTab = 0;//�Ľ��ηֶ���
	m_BPDArray.RemoveAll();
	m_NCM = 0;
}

GTZX2_JYX_ROAD::GTZX2_JYX_ROAD(GTZX2_JYX_ROAD *gtzx2, double xjj , BOOL IsGaiJian )//plh04.30
	//xjj=-1,��������,xjj=1,��������
{

	AllColor = -1;
	if (_tcscmp(gtzx2->xlname, L"��ͨ��ƻ���")==0)
	{
		_tcscpy(xlname, L"��ͨ��Ƣ���");
	}
	GTZX_JYX_ROAD *gtzx;
	gtzx = gtzx2->CreateGtzx();

	isJYGuiFanCS = !IsGaiJian;
	//if(isJYGuiFanCS) mGuiFanCS = mJYGuiFanCS;
	//else mGuiFanCS = mGJGuiFanCS;
	m_GuiFanCS = gtzx2->m_GuiFanCS;

	m_IsNeedCalGJDInf = false;

	ACHAR JDGH2x[8];
	if( fabs( xjj)< 0.001 )
	{
		_tcscpy( JDGH2x , L"JD" );
	}
	else if( xjj > 0.001 )
	{
		_tcscpy( JDGH2x , L"YJD" );
	}
	else 
	{
		_tcscpy( JDGH2x , L"ZJD" );
	}


	zory = 0;
	_tcscpy(xlname,L"");
	calsflag =true;
	m_pm1=gtzx;

	NJD=m_pm1->NJD;
	JD0=m_pm1->JD0;
	JDarray.RemoveAll();
	JDarray.SetSize(m_pm1->JDarray.GetSize());
	QXB.RemoveAll();
	int i;
	for(i=0;i<NJD;i++)
	{
		JD2[i].JDXZ=B_X;
		JD2[i].jdno=i;
		JD2[i].jdno1=i;//��Ӧһ�߽������;
		JD2[i].xjj=xjj;
		_stprintf( JD2[i].JDNum ,L"%s%d",JDGH2x,i+JD0);//������
	}
	//���ս�����һ���ϵ�ͶӰ���//plh05.06
	ACHAR *lcstr=LCchr(m_pm1->DLB[0].EGH,
		m_pm1->DLB[0].BLC +	m_pm1->DLB[0].DL,3,1);
	_tcscpy(JD2[0].ZHLC,lcstr);
	lcstr=LCchr(m_pm1->DLB[m_pm1->NDL-1].EGH,
		m_pm1->DLB[m_pm1->NDL-1].BLC + m_pm1->DLB[m_pm1->NDL-1].DL,3,1);
	_tcscpy(JD2[NJD-1].ZHLC,lcstr);
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
	GetT0min_Lrmin();

	CalJDdata();

	SetJDdata();

	getpxy_DATA();

	NDL=0;

	CalNYDL();
	m_pm2 = gtzx2;

	IsReSetColor = FALSE ;

	RTolCheckUp( IsGaiJian );
	xlpoint PZtmp;
	//	PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+4.0 * xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(���л���)")==0)
		PZtmp.lc=array_pxy[0][0][4]+ 2.0 * xl_length/5.0;  // ͳһ���

	xlpoint_pz(array,tarray,&PZtmp);
	m_XLNameNotePt[X]=PZtmp.y+xl_length*sin(PZtmp.a+pi*0.5)/8.0;
	m_XLNameNotePt[Y]=PZtmp.x+xl_length*cos(PZtmp.a+pi*0.5)/8.0;
	//	NGJDFDInfTab = 0;//�Ľ��ηֶ���
	m_BPDArray.RemoveAll();
	m_NCM = 0;
}

//plh
//double GTZX2::T0min=GTZX2::mGuiFanCS.MinZhi; //    mGuiFanCS.MinZhi;
//double GTZX2::Lrmin=GTZX2::mGuiFanCS.MinYuan;//��СԲ���߳�

void GTZX2_JYX_ROAD::GetT0min_Lrmin()//plh
{
	T0min=m_GuiFanCS.MinZhi; //    mGuiFanCS.MinZhi;
	Lrmin=m_GuiFanCS.MinYuan;//��СԲ���߳�

	m_IsNeedCalGJDInf = false;
	// 	acutPrintf(L"DV=%lf\n",m_pm1->DV);
	if(m_pm1==NULL){T0min=40.0;Lrmin=20.0;}
	else
	{	
		BAS_DRAW_FUN_JYX::GetT0min_Lrmin(m_pm1->DV,T0min,Lrmin,m_pm1->m_GuiFanCS);
	}
}

void GTZX2_JYX_ROAD::INIT()
{

}
void GTZX2_JYX_ROAD::UpdateData(bool IsCreateFirst)
{
	assertWriteEnabled();
	if(JD2[0].xjj>0)//����Ϊ����
		draw_zybz=1;//����Ҫ�ر������
	else
		draw_zybz=-1;//����Ҫ�ر����Ҳ�	

	if (!IsCreateFirst)
	{
		ModifyOriExJDXY();//���ɶ��߽�������
		CalJDdata1();
	}
	else
	{
		CalJDdata();
	}

	SetJDdata();

	//���ɶ�����Ԫ����
	getpxy_DATA();

	//������ҵ����1
	Create2XDLB();

	CalNYDL();

}

//�ֲ�����II�����ж�
void GTZX2_JYX_ROAD::UpdateDataRXD(int iRXD)
{
	assertWriteEnabled();
	if(JD2[0].xjj>0)//����Ϊ����
		draw_zybz=1;//����Ҫ�ر������
	else
		draw_zybz=-1;//����Ҫ�ر����Ҳ�	

	ModifyOriExJDXY();//���ɶ��߽�������
	CalJDdata1();

	SetJDdata();

	//���ɶ�����Ԫ����
	getpxy_DATA();

	//�����´���ָ�����жεĶ�����
	Create2XDLB(iRXD);

	CalNYDL();


	//Zhujiang------��ƻ��ߵľֲ����²���Ҫͳ�ƸĽ�����Ϣ
	if (_tcscmp(xlname , L"��ͨ��Ƣ���") == 0)
	{
		int SDL,EDL;
		double RXDSml,RXDEml;
		GetiRXDDLinfo(iRXD,SDL,EDL);
		RXDSml = DLB2x[SDL].TYLC;
		RXDEml = DLB2x[EDL].TYLC;

		RefreshExFDArray(RXDSml,RXDEml,iRXD); 
	}



}



void GTZX2_JYX_ROAD::UpdateDataSeveralRXD(CArray<int,int> &p_arrChangedRXD,UltraCArray_T<RXDInfo,RXDInfo> &p_arrNewRXD)
{

	assertWriteEnabled();
	if(JD2[0].xjj>0)//����Ϊ����
		draw_zybz=1;//����Ҫ�ر������
	else
		draw_zybz=-1;//����Ҫ�ر����Ҳ�	

	ModifyOriExJDXY();//���ɶ��߽�������
	CalJDdata1();

	SetJDdata();

	//���ɶ�����Ԫ����
	getpxy_DATA();

	int i = 0;

	//-----------------------------------------------����ʵ��Ķ�����
	CArray<EXDLB,EXDLB> arrDLBNew;
	arrDLBNew.RemoveAll();
	for (i = 0; i < p_arrNewRXD.GetSize(); i++)
	{
		if (p_arrNewRXD[i].m_arrDLB.GetSize() == 0)
		{
			Generate2XDLB(i,arrDLBNew);
		}
		else
		{
			for (int j = 0; j < p_arrNewRXD[i].m_arrDLB.GetSize();j++)
			{
				//���¸��������������жα��(���жθ��������仯,��ֵҲ����ű仯)
				p_arrNewRXD[i].m_arrDLB[j].no = i;      
				arrDLBNew.Add(p_arrNewRXD[i].m_arrDLB[j]);
			}

		}
	}

	NDL = arrDLBNew.GetSize();
	for (i = 0; i <NDL;i++)
		DLB2x[i] = arrDLBNew[i];
	//--------------------------------------------------------------------

	CalNYDL();


	//Zhujiang------��ƻ��ߵľֲ����²���Ҫͳ�ƸĽ�����Ϣ
	if (_tcscmp(xlname , L"��ͨ��Ƣ���") == 0)
	{
		int SDL,EDL;
		double RXDSml,RXDEml;


		for (i = 0; i < p_arrChangedRXD.GetSize(); i++)
		{
			GetiRXDDLinfo(p_arrChangedRXD[i],SDL,EDL);
			RXDSml = DLB2x[SDL].TYLC;
			RXDEml = DLB2x[EDL].TYLC;

			RefreshExFDArray(RXDSml,RXDEml,p_arrChangedRXD[i]); 
		}
	}
	arrDLBNew.RemoveAll();
}

void GTZX2_JYX_ROAD::InsertDateRXD(int iRXD)
{
	assertWriteEnabled();
	if(JD2[0].xjj>0)//����Ϊ����
		draw_zybz=1;//����Ҫ�ر������
	else
		draw_zybz=-1;//����Ҫ�ر����Ҳ�	

	ModifyOriExJDXY();//���ɶ��߽�������
	CalJDdata1();

	SetJDdata();

	//���ɶ�����Ԫ����
	getpxy_DATA();

	Create2XDLB(iRXD,NULL,false);


	CalNYDL();

	//Zhujiang------��ƻ��ߵľֲ����²���Ҫͳ�ƸĽ�����Ϣ
	if (xlname == L"��ͨ��Ƣ���")
	{
		int SDL,EDL;
		double RXDSml,RXDEml;
		GetiRXDDLinfo(iRXD,SDL,EDL);
		RXDSml = DLB2x[SDL].TYLC;
		RXDEml = DLB2x[EDL].TYLC;
		RefreshExFDArray(RXDSml,RXDEml,iRXD);
	}

}

double GTZX2_JYX_ROAD::Lh_Rtop(double Lh,double R)
{
	double AA=sqrt(R*Lh);
	double y=spiral_x(AA,Lh);
	double x=spiral_y(AA,Lh);
	double t=Lh/(2*R);
	double dr=x+R*sin(t+0.5*pi);	
	return dr-R;
}

//�ɰ뾶R������Lh,��ֱ�߳���ֱ�߱�࣬�����߳�T,Afa
double GTZX2_JYX_ROAD::GetAfaFromR_Lh_T0_dBJ(double R,double Lh,double T0,double dBJ,double &T)
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
double GTZX2_JYX_ROAD::CalTFromR_Lr_Lh(double R,double Lr,double Lh)
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
double GTZX2_JYX_ROAD::CalT0andLjiaFromR_Lr_Lh_d(double R,double Lr,double Lh,double dbj,double &T0)
{
	double bata0,m,p,afa;
	bata0=0.5*Lh/R;
	afa=Lr/R+2.0*bata0;
	m=0.5*Lh-Lh*Lh*Lh/(240*R*R);
	p=Lh*Lh/(24*R)-Lh*Lh*Lh*Lh/(2688*R*R*R);
	T0=m+(R+p)*tan(0.5*afa);	
	return dbj/cos(afa)-2.0*T0;
}

Adesk::Boolean  GTZX2_JYX_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{ 
	//	setColorLineW();

	int i=0;
	AcGePoint3d Pt;

	AcGeVector3d  normal(0.0,0.0,1.0);
	int j=0,k=0;
	double INDATA[5],LCB;	

	//������ҵ����

	ModifyOriExJDXY();

	//	CalJDdata();//2005.2.1

	CalJDdata1();

	TagEXJDIsShow();

	SetJDdata();


	int ndl;

	ndl = NDL;
	getpxy_DATA();


	if(ndl<1)
		NDL=ndl;

	//	SetBZLCVal();
	GetDLTYLC();

	CalNYDL();

	xlpoint PB,PE; 

	if (IsdrawLC)
	{
		BZRXD(pWd);
		BZXJJ(pWd);	
	}


	AcGePoint3d *verts=new AcGePoint3d[NJD];
	// �潻������
	if (IsdrawJDLX)
	{
		for (i = 0; i < NJD; i++) 
		{
			verts[i].x=JDarray[i].E; 
			verts[i].y=JDarray[i].N;
			verts[i].z=0.0;
		}
		if (pWd!= NULL) 
		{
			pWd->subEntityTraits().setSelectionMarker(1);
			pWd->subEntityTraits().setColor(GTZXJDToJDColor);	
			pWd->geometry().polyline(NJD,verts);
		} 
		else
		{
			assert(Adesk::kFalse);
		}
		// �潻�㴦Բ
		for (i = 1; i < NJD-1; i++) //plh05.07
		{
			if(!JD2[i].ISShow)continue;
			Pt[X]=verts[i].x; Pt[Y]=verts[i].y;verts[i].z=0.0;
			pWd->subEntityTraits().setColor(4);	
			pWd->geometry().circle(Pt,3.6,normal);			
		}
	}



	DrawXL(pWd , xlname );
	if (IsdrawJD)
	{
		BZJD(pWd,JDarray,QXB,JDGH);
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


			//	 GUXWXY(INDATA,&PB,&PE,1); 
			PB=PE;
		}
	}

	LCB=array_pxy[0][0][4];  // ������

	//	BZTZD(pWd,draw_zybz,array,tarray,true);//��ע���������


	//����
	if (IsdrawTZD)
	{
		BZTZD2( pWd, 100,0,2);  //  ��ע��������� 

		////???
		//DrawBZLC(pWd);

		BZGJD(pWd);//��ע�Ľ���
	}


	//Ӱ�췶Χ
	DrawBandEPointsForGTZX2(pWd);

	delete [] verts;

	Create2XGTDLB();
	Create2XGTBPDArray();
	if (IsdrawBPD)
	{
		BZCM(pWd);
	}

	//	acutPrintf(L"��С��ֱ�߳�Ϊ��%.2lf\n",T0min);
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}


void GTZX2_JYX_ROAD::DrawBandEPointsForGTZX2(AcGiWorldDraw* pWd )
{
	AcGePoint3d startPt,endPt,MidPt,p1;

	ACHAR textACHAR[50];

	int zybz = -1 ; 
	double  WB;

	double BZLineLength = 30.0;
	WB=1.5*texth;

	MidPointArray.RemoveAll();
	GJDInforArrayNum = GJDInforArray.GetSize();

	for(int i=0; i<GJDInforArrayNum ;i++)
	{
		CString SKML,EKML;
		//���
		startPt.x = GJDInforArray[i].spt[1]; //��Ļ
		startPt.y = GJDInforArray[i].spt[0];
		startPt.z = 0.0;

		//�������ķ���
		PZtmp.lc = PROJ_ML(array,tarray,startPt.y,startPt.x,startPt.y,startPt.x);

		BAS_DRAW_FUN_JYX::xlpoint_pz(array,tarray,&PZtmp); 

		double angle;
		angle = PZtmp.a+pi*0.5*zybz;
		if(angle<0.0) angle = angle + 2*pi;
		if(angle>2*pi) angle = angle - 2*pi;


		// SKML = GetGTX2Xclc(GJDInforArray[i].spt[0],GJDInforArray[i].spt[1]);
		// _tcscpy( GJDInforArray[i].GJDBLC , SKML );
		//SKML = GJDInforArray[i].GJDBLC ;
		//ACHAR ckml[80];
		//double dml;
		//ACHAR GH[10];
		//_tcscpy(ckml,SKML);
		//split_ckml(ckml,dml,GH);
		////ע��
		//_stprintf(textACHAR , L"%.1lf%s���",dml/1000.0,GJDInforArray[i].gjdname );
		//_tcscat( textACHAR , SKML);

		SKML = GJDInforArray[i].GJDBLC;
		_stprintf(textACHAR , L"%s���%s",GJDInforArray[i].gjdname,SKML );

		BZLineLength = 5*_tcslen(textACHAR);
		endPt.x = PZtmp.y +(BZLineLength)*sin(angle);
		endPt.y = PZtmp.x +(BZLineLength)*cos(angle);
		endPt.z = 0.0; 

		double WB=1.5*texth;
		p1.x=PZtmp.y+1.5*texth*sin(PZtmp.a)+(WB+3*texth)*sin(PZtmp.a+pi*0.5*zybz) ;
		p1.y=PZtmp.x+1.5*texth*cos(PZtmp.a)+(WB+3*texth)*cos(PZtmp.a+pi*0.5*zybz) ; 
		G_makelineW(pWd , startPt ,endPt ,QXYSColor, LineWeightValue);

		double b=PZtmp.a +pi*0.5;
		b=pi*0.5-b; if (zybz==-1) b=b+pi;
		if (b<0.0) b=2*pi+b;		  if (b>2*pi) b=b-2*pi;		         
		//f(type<=1)  b=b-pi*0.5; 

		G_maketext(pWd,p1,textACHAR,b,texth,QXYSColor,1);   

		//�м�ֱ��
		MidPt.x = ( startPt.x + endPt.x )/2.0;
		MidPt.y = ( startPt.y + endPt.y )/2.0;
		MidPt.z = 0.0 ;

		endPt.x = MidPt.x +(5)*sin(PZtmp.a);
		endPt.y = MidPt.y +(5)*cos(PZtmp.a);
		endPt.z = 0.0; 

		MidPointArray.Add(MidPt);
		//	G_makelineW(pWd , MidPt ,endPt ,1, LineWeightValue);


		//�յ�	
		startPt.x = GJDInforArray[i].ept[1]; //��Ļ
		startPt.y = GJDInforArray[i].ept[0];
		startPt.z = 0.0;

		//	EKML = GetGTX2Xclc(GJDInforArray[i].ept[0],GJDInforArray[i].ept[1]);
		//	_tcscpy( GJDInforArray[i].GJDELC , EKML );
		EKML = GJDInforArray[i].GJDELC ;

		//�������ķ���
		PZtmp.lc = PROJ_ML(array,tarray,startPt.y,startPt.x,startPt.y,startPt.x);

		BAS_DRAW_FUN_JYX::xlpoint_pz(array,tarray,&PZtmp); 


		angle = PZtmp.a+pi*0.5*zybz;
		if(angle<0.0) angle = angle + 2*pi;
		if(angle>2*pi) angle = angle - 2*pi;


		//_stprintf(textACHAR , L"%.1lf%s�յ�",dml/1000.0,GJDInforArray[i].gjdname );
		//_tcscat( textACHAR , EKML);
		EKML = GJDInforArray[i].GJDELC;
		_stprintf(textACHAR , L"%s�յ�",GJDInforArray[i].gjdname,EKML );


		BZLineLength = 5*_tcslen(textACHAR);
		endPt.x = PZtmp.y +(BZLineLength)*sin(angle);
		endPt.y = PZtmp.x +(BZLineLength)*cos(angle);
		endPt.z = 0.0; 
		G_makelineW(pWd , startPt ,endPt ,QXYSColor, LineWeightValue);


		b=PZtmp.a +pi*0.5;
		b=pi*0.5-b; if (zybz==-1) b=b+pi;
		if (b<0.0) b=2*pi+b;		  if (b>2*pi) b=b-2*pi;		         
		//f(type<=1)  b=b-pi*0.5; 


		p1.x=PZtmp.y+0.5*texth*sin(PZtmp.a+pi)+(WB+3*texth)*sin(PZtmp.a+pi*0.5*zybz) ;
		p1.y=PZtmp.x+0.5*texth*cos(PZtmp.a+pi)+(WB+3*texth)*cos(PZtmp.a+pi*0.5*zybz) ; 

		G_maketext(pWd,p1,textACHAR,b , texth, QXYSColor,1);   


		//�м�ֱ��
		MidPt.x = ( startPt.x + endPt.x )/2.0;
		MidPt.y = ( startPt.y + endPt.y )/2.0;
		MidPt.z = 0.0 ;

		endPt.x = MidPt.x +(5)*sin(PZtmp.a + pi);
		endPt.y = MidPt.y +(5)*cos(PZtmp.a + pi);
		endPt.z = 0.0; 

		MidPointArray.Add(MidPt);
		//	G_makelineW(pWd , MidPt ,endPt ,5, LineWeightValue);

	}

	MidPointArrayNum = MidPointArray.GetSize();

}


////

void GTZX2_JYX_ROAD::DrawXL(AcGiWorldDraw* pWd , ACHAR CurXLName[256])
{

	///////////
	// �潻��ں�
	//	acutPrintf(L"JD0=%d,JDGH=%s\n",JD0,JDGH);
	AcGePoint3d Pt;
	xlpoint  PZtmp; 
	//	xlpoint  PB,PE;
	//	double INDATA[5],double LCB;
	AcGePoint3d  ptb,pte;
	AcGePoint3d MidPt,StartPt,EndPt,APt1,APt2;//��·����
	double B,b;
	ACHAR str[280];
	//	 ads_	printf(">>>>>   xl_length=%lf\n",xl_length);

	//	if(_tcscmp(CurXLName,L"��ƻ���(��������)")==0)
	//		PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	//	else if(_tcscmp(CurXLName,L"��ƻ���(��������)")==0)
	//		PZtmp.lc=array_pxy[0][0][4]+4.0 * xl_length/5.0;  // ͳһ���
	//	else if(_tcscmp(CurXLName,L"��ƻ���(���л���)")==0)
	//		PZtmp.lc=array_pxy[0][0][4]+ 2.0 * xl_length/5.0;  // ͳһ���

	PZtmp.lc = PROJ_ML(array,tarray,m_XLNameNotePt.y,m_XLNameNotePt.x,m_XLNameNotePt.y,m_XLNameNotePt.x);
	xlpoint_pz(array,tarray,&PZtmp);
	B=PZtmp.a;
	double Fwj = 0.0;
	xyddaa(PZtmp.x,PZtmp.y,m_XLNameNotePt.y,m_XLNameNotePt.x,&Fwj);
	//pte[X]=PZtmp.y+xl_length*sin(B+pi*0.5)/4.0;
	//pte[Y]=PZtmp.x+xl_length*cos(B+pi*0.5)/4.0;
	pte[X]=PZtmp.y+xl_length*sin(Fwj)/4.0;
	pte[Y]=PZtmp.x+xl_length*cos(Fwj)/4.0;

	ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
	//////////////////////////////////////////////////////////////////////////
	//��·����

	//	MidPt.x=0.5*(ptb.x+pte.x);
	//	MidPt.y=0.5*(ptb.y+pte.y);
	//	MidPt.z=0.0;

	MidPt = m_XLNameNotePt;

	StartPt.x=MidPt.x-xl_length*sin(B)/6.0;
	StartPt.y=MidPt.y-xl_length*cos(B)/6.0;
	StartPt.z=0.0;

	EndPt.x=MidPt.x+xl_length*sin(B)/6.0;
	EndPt.y=MidPt.y+xl_length*cos(B)/6.0;
	EndPt.z=0.0;

	APt1.x=EndPt.x+xl_length*sin(B+PI+0.1*PI)/15.0;
	APt1.y=EndPt.y+xl_length*cos(B+PI+0.1*PI)/15.0;
	APt1.z=0.0;

	APt2.x=EndPt.x+xl_length*sin(B+PI-0.1*PI)/15.0;
	APt2.y=EndPt.y+xl_length*cos(B+PI-0.1*PI)/15.0;
	APt2.z=0.0;

	G_makelineW(pWd,StartPt,EndPt,mdbnameBoxColor,LineWeightValue);
	G_makelineW(pWd,APt1,EndPt,mdbnameBoxColor,LineWeightValue);
	G_makelineW(pWd,APt2,EndPt,mdbnameBoxColor,LineWeightValue);
	//////////////////////////////////////////////////////////////////////////

	G_makelineW(pWd,ptb,pte,mdbnameBoxColor,LineWeightValue);
	Pt[Y]=pte[Y]+xl_length*cos(B)/20.0;
	Pt[X]=pte[X]+xl_length*sin(B)/20.0;
	ptb[Y]=pte[Y]+xl_length*cos(B+pi)/20.0;
	ptb[X]=pte[X]+xl_length*sin(B+pi)/20.0;
	G_makelineW(pWd,Pt,ptb,mdbnameBoxColor,LineWeightValue);
	//pte[Y]=Pt[Y]+xl_length*cos(B+pi*0.5)/20.0;
	//pte[X]=Pt[X]+xl_length*sin(B+pi*0.5)/20.0;
	pte[Y]=Pt[Y]+xl_length*cos(Fwj)/20.0;
	pte[X]=Pt[X]+xl_length*sin(Fwj)/20.0;
	G_makelineW(pWd,Pt,pte,mdbnameBoxColor,LineWeightValue);
	//Pt[Y]=ptb[Y]+xl_length*cos(B+pi*0.5)/20.0;
	//Pt[X]=ptb[X]+xl_length*sin(B+pi*0.5)/20.0;
	Pt[Y]=ptb[Y]+xl_length*cos(Fwj)/20.0;
	Pt[X]=ptb[X]+xl_length*sin(Fwj)/20.0;
	G_makelineW(pWd,ptb,Pt,mdbnameBoxColor,LineWeightValue);
	pte[Y]=Pt[Y]+xl_length*cos(B)/10.0;
	pte[X]=Pt[X]+xl_length*sin(B)/10.0;
	G_makelineW(pWd,Pt,pte,mdbnameBoxColor,LineWeightValue);
	CString name,Dir;

	name=mdbname+ " --" + xlname;
	// name= xlname ;
	name.Trim();


	_tcscpy(str,name);
	//Pt[Y]=Pt[Y]+xl_length*cos(B-pi*0.5)/42.0;
	//Pt[X]=Pt[X]+xl_length*sin(B-pi*0.5)/42.0;
	Pt[Y]=Pt[Y]+xl_length*cos(Fwj+PI)/42.0;
	Pt[X]=Pt[X]+xl_length*sin(Fwj+PI)/42.0;
	b=B+pi*0.5;
	b=pi*0.5-b; 
	if (b<0.0) b=2*pi+b;	  if (b>2*pi) b=b-2*pi;		         
	G_maketext(pWd,Pt,str,b+pi*0.5,xl_length/80.0,mdbnameTextColor,1);

}


void GTZX2_JYX_ROAD::CalJDdata()
{
	double lo,lo1,D;

	for(int i=0; i<NJD; i++)
	{	
		if(JD2[i].JDXZ==B_X)//���жν���
		{
			//		_stprintf(JDarray[i].JDNum,L"%d",JD0 + i);
			//   JDarray[i].JDNum = JD0 + i;			
			int jdn1 = JD2[i].jdno1;//��Ӧһ�߽������
			double xjj = JD2[i].xjj;
			double EXJD[2];
			CalEXJD(jdn1,i,EXJD);
			JD2[i].x = EXJD[0];
			JD2[i].y = EXJD[1];//��������߽���

			double R = m_pm1->JDarray[jdn1].R;
			double JK;
			if(JD2[i].JDXZ==B_X  )  // JD2[i].JDXZ== B_X_Des
			{		
				JK = get_XJJK(m_pm1->DV,R,isJYGuiFanCS,m_pm1->m_GuiFanCS);//�߼��ӿ�
				JD2[i].qxjk = JK;
				JD2[i].Lo1 = m_pm1->JDarray[jdn1].l1;
				JD2[i].R = R;

				JD2[i].Lo2 = m_pm1->JDarray[jdn1].l2;           
				lo1 = m_pm1->JDarray[jdn1].l1;//һ�߻���
				D = fabs(xjj);
			}
			//���߰뾶

			if( ( JD2[i].JDXZ==B_X  )&&i>0&&i<NJD-1)  //  JD2[i].JDXZ== B_X_Des
			{
				double xjj2;
				xjj2 = JD2[i-1].xjj;//��һ�����߼��
				if(fabs(xjj-xjj2)<0.01)//ǰ���߼�����
				{
					//////////////////////////////////////////////////////////////////////////
					////xjj�������߼ӿ�............		   
					//if(xjj>0)//һ��Ϊ����
					//{
					//	if(m_pm1->QXB[jdn1].LorR<0)//��ת,II������࣬���ٻ���
					//	{
					//		//���仺��

					//		lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//���ȱ� 2003.12.4�ṩ

					//		JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
					//		D = -1.0*D;
					//	}
					//	else
					//	{
					//		//���仺��
					//		lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
					//		JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
					//	}
					//}
					//else//һ��Ϊ����
					//{
					//	if(m_pm1->QXB[jdn1].LorR<0)//��ת,II�����ڲ�
					//	{
					//		lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
					//		JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
					//	}
					//	else
					//	{   //���仺��
					//		lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//�� 2003.12.4�ṩ
					//		JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
					//		D  = -1.0*D;
					//	}
					//}
					////��뾶
					//double B ;
					//if( fabs( xjj) > 1.0e-5 && fabs(R) >1.0e-5 ) //�����߼��Ϊ0(��ƻ���)
					//{
					//	B = R - D + (lo1*lo1)/24/R;

					//	JD2[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//���߰뾶	
					//}
					//else 
					//{
					//	JD2[i].Lo1 = m_pm1->JDarray[jdn1].l1 ;
					//	JD2[i].Lo2 = m_pm1->JDarray[jdn1].l2 ;
					//}
					//////////////////////////////////////////////////////////////////////////

					//////////////////////////////////////////////////////////////////////////
					CalTXY(m_pm1->QXB[jdn1].LorR * JD2[i].xjj,JD2[i].qxjk,R,lo1,JD2[i].R,lo);
					JD2[i].Lo2 = JD2[i].Lo1 = lo;
					//////////////////////////////////////////////////////////////////////////
				}

			}   
			JD2[i].jdno = JD0 + i;
		}//����
		else if(JD2[i].JDXZ==S_StartPoint)//ֱ�߱�����
		{
			if(calsflag)
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

	CheckJD2IsAvailable();
}

//����ͬ��Բ
bool GTZX2_JYX_ROAD::CalTXY(double XianJianJu,double JiaKuan,double BanJing1,double HuanChang1,
							double& BanJing2,double& HuanChang2)
{
	double B = 1.1;
	if (BanJing1 > 0.0)
	{
		B = BanJing1 - XianJianJu + HuanChang1 * HuanChang1 / 24.0 / BanJing1;
		double jianju = 0.0;

		if (JiaKuan == 0.0)//����ӿ�ͬ����
		{
			HuanChang2 = HuanChang1;
			jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
			BanJing2 = (B + jianju) / 2.0;
			jianju = fabs(BanJing1 - BanJing2);
			if (jianju < fabs(XianJianJu))
			{
				ads_printf(L"�����߼��С����С�߼��\n");
				BanJing2 = BanJing1;
				HuanChang2 = HuanChang1;
				return false;
			}
		}
		else//�ӿ�
		{
			if(XianJianJu > 0.0)//�������ڲ�
			{
				jianju=sqrt((BanJing1 - XianJianJu - JiaKuan) * (HuanChang1 * HuanChang1 / BanJing1 + 24.0 * JiaKuan));//�����ڲ໺��
				HuanChang2 = (int)(jianju/10.0+1.0)*10;//ȡ����10��
				jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
				BanJing2 = (B + jianju)/2.0;
				jianju=BanJing1 - BanJing2;
			}
			else if (XianJianJu < 0.0)//���������
			{
				if (HuanChang1 > 0.0)//�����˻��߻��������̶��߻���
				{
					//��С���߻���
					jianju = sqrt((BanJing1 - XianJianJu + JiaKuan) * (HuanChang1 * HuanChang1 / BanJing1 - 24.0 * JiaKuan));
					HuanChang2 = (int)(jianju / 10.0) * 10;

					//ji suan you xian ban jing
					B=BanJing1 - XianJianJu + HuanChang1 * HuanChang1 / 24.0 / BanJing1;
					jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
					BanJing2 = (B + jianju) / 2.0;
					jianju=BanJing1 - BanJing2;
				}
				else if (HuanChang2 > 0.0)//���߻���0����Ҫ�ɶ��߻����ӳ����߻���
				{
					jianju=sqrt(BanJing1 * (HuanChang2 * HuanChang2 / (BanJing1 - XianJianJu + JiaKuan) + 24.0 * JiaKuan));//�����ڲ໺��
					HuanChang1 = (int)(jianju / 10.0 + 1.0) * 10.0;//ȡ����10��

					//ji suan you xian ban jing
					B=BanJing1 - XianJianJu + HuanChang1 * HuanChang1 / 24.0 / BanJing1;
					jianju=sqrt(B * B - HuanChang2 * HuanChang2 / 6.0);
					BanJing2 = (B + jianju) / 2.0;
					jianju=BanJing1 - BanJing2;
				}
				else
				{
					AfxMessageBox(L"���ߺͶ��ߵĻ�������0�����ܼ���\n");
					BanJing2 = BanJing1;
					HuanChang2 = HuanChang1;
					return false;
				}
			}
			else
			{
				BanJing2 = BanJing1;
				HuanChang2 = HuanChang1;
				return true;
			}
		}
	}
	else
	{
		ads_printf(L"��ƻ��ߵ����߰뾶���󣬲��ܼ���\n");
		BanJing2 = BanJing1;
		HuanChang2 = HuanChang1;
		return false;
	}
	return true;
}

double GTZX2_JYX_ROAD::CalS(double R,double Lh,double T0,double xjj1,double xjj2,ACHAR ZHLC[80],double &x1,double &y1,double &x2,double &y2)
	//�뾶����������ֱ�߳����߼��1���߼��2��JD1xy��JD2xy,����Բ���߳�
{
	if(T0<0)T0 = 80.0;
	if(m_pm1==NULL)return -9999;
	double T;//�г�
	double afa=GetAfaFromR_Lh_T0_dBJ(R,Lh,T0,fabs(xjj2-xjj1),T);

	double xzh,yzh;//������x,y,������߷�λ��
	xlpoint Pt;
	//	Pt.lc=TYLC1(ZHLC,m_pm1->NDL,m_pm1->DLB);
	//	xlpoint_pz(m_pm1->array,m_pm1->tarray,&Pt);
	if (m_pm2)
		m_pm2->m_pm1->GetPZFromXCLC1X(ZHLC,Pt);
	else
		m_pm1->GetPZFromXCLC1X(ZHLC,Pt);
	xzh=Pt.x+xjj1*cos(Pt.a+m_dHalfPI);
	yzh=Pt.y+xjj1*sin(Pt.a+m_dHalfPI);
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


bool GTZX2_JYX_ROAD::CalS(double R1,double L01,double R2,double L02,double xjj1,double xjj2,double JZXLen,double ZH_N,double ZH_E,double Fwj,double& afa,double &x1,double &y1,double &x2,double &y2)
	//�뾶����������ֱ�߳����߼��1���߼��2��JD1xy��JD2xy,����Բ���߳�
{
	afa = 0.0;
	double T1,T2;
	T1 = T2 = 0.0;
	if (!Get_S_Afa(R1,L01,R2,L02,fabs(xjj2-xjj1),JZXLen,afa,T1,T2))
	{
		return false;
	}
	if (xjj1<-1.0e-6)
		afa *= -1.0;
	double xzh,yzh;//������x,y,������߷�λ��
	//xlpoint Pt;//ZH��
	//	Pt.lc=TYLC1(ZHLC,m_pm1->NDL,m_pm1->DLB);
	//	xlpoint_pz(m_pm1->array,m_pm1->tarray,&Pt);
	//Pt.lc = PROJ_ML(array,tarray,ZH_N,ZH_E,ZH_N,ZH_E);
	//xlpoint_pz(array,tarray,&Pt);
	//xzh=Pt.x+xjj1*cos(Pt.a+0.5*pi);
	//yzh=Pt.y+xjj1*sin(Pt.a+0.5*pi);
	//x1=xzh+T1*cos(Pt.a);
	//y1=yzh+T1*sin(Pt.a);
	//double flag=1.0;
	//if(xjj2<xjj1)flag=-1.0;
	//double dd;
	//dd=(T1+T2+JZXLen);
	//x2=x1+dd*cos(Pt.a+flag*afa);
	//y2=y1+dd*sin(Pt.a+flag*afa);

	xzh=ZH_N+xjj1*cos(Fwj+m_dHalfPI);
	yzh=ZH_E+xjj1*sin(Fwj+m_dHalfPI);
	x1=xzh+T1*cos(Fwj);
	y1=yzh+T1*sin(Fwj);
	double flag=1.0;
	if(xjj2<xjj1)flag=-1.0;
	double dd;
	dd=(T1+T2+JZXLen);
	x2=x1+dd*cos(Fwj+flag*afa);
	y2=y1+dd*sin(Fwj+flag*afa);

	return true;
}

//////////////////////////////////////////////////////////////////////////
//����S���ת��
//R1,L01��һ������İ뾶����
//R2,L02�ڶ�������İ뾶����
//DetaXJJǰ���߼���ֵ
//JZXLen��ֱ�߳�
//Alfaת��(���ȵ�λ)
bool GTZX2_JYX_ROAD::Get_S_Afa(double R1,double L1,double R2,double L2,double DetaXJJ,double JZXLen,double& Alfa,double& T1,double& T2)
{
	Alfa = 0.0;
	T1 = T2 = 0.0;
	double A1,A2,B1,B2,P1,P2,M1,M2;
	A1=sqrt(R1*L1);A2=sqrt(R2*L2);

	if(R1>1.0e-6 && R2>1.0e-6)
	{
		B1=L1*0.5/R1;
		B2=L2*0.5/R2;
	}
	else
	{
		AfxMessageBox(L"�뾶̫С");
		return false;
	}


	if (L1<0.0001) { P1=0.0;M1=0.0; }
	else {     P1=spiral_p(A1,L1);M1=spiral_xm(A1,L1);   };
	if (L2<0.0001) { P2=0.0;M2=0.0; }
	else {     P2=spiral_p(A2,L2); M2=spiral_xm(A2,L2);  };


	//////////////////////////////////////////////////////////////////////////
	//�������¹�ϵ����ת��Alfa
	//DetaXJJ / ((T1+T2)+JZXLen) = sin(Alfa)
	//����DetaXJJ / ((R1+P1)*tan(Alfa/2.0) + M1 + (R2+P2)*tan(Alfa/2.0) + M2 + JZXLen) = sin(Alfa)
	//��tan(Alfa/2.0) = t�����ܹ�ʽ��sin(Alfa) = 2*t / (1+t*t)
	//���÷�������(2*(R1+P1+R2+P2)-DetaXJJ) * t*t + 2*(M1+M2+JZXLen)*t + (-DetaXJJ) = 0
	double a,b,c,t;
	a = 2*(R1+P1+R2+P2)-DetaXJJ;
	b = 2*(M1+M2+JZXLen);
	c = -DetaXJJ;
	double Deta = b*b-4.0*a*c;
	if (Deta<0.0001)
	{
		AfxMessageBox(L"�޷��������������S��ת��");
		return false;
	}

	double t1 = (-1.0*b + sqrt(Deta)) / (2.0*a);
	double t2 = (-1.0*b - sqrt(Deta)) / (2.0*a);

	double Alf1 = 2 * atan(t1);
	double Alf2 = 2 * atan(t2);

	bool IsRuiJiao1,IsRuiJiao2;//�Ƿ����

	IsRuiJiao1 = IsRuiJiao2 = false;
	if (Alf1>1.0e-6 && Alf1<PI/2.0)
	{
		IsRuiJiao1 = true;
	}
	if (Alf2>1.0e-6 && Alf2<PI/2.0)
	{
		IsRuiJiao2 = true;
	}
	if (IsRuiJiao1 && IsRuiJiao2)
	{
		Alfa = min(Alf1,Alf2);
	}
	else if (IsRuiJiao1)
	{
		Alfa = Alf1;
	}
	else if (IsRuiJiao2)
	{
		Alfa = Alf2;
	}
	else
	{
		AfxMessageBox(L"S��ת��Ϊ�۽ǣ����������Ƿ�����");
		return false;
	}
	T1 = (R1+P1)*tan(Alfa/2.0) + M1;
	T2 = (R2+P2)*tan(Alfa/2.0) + M2;
	return true;
}

void GTZX2_JYX_ROAD::CalJDdata1()
{
	for(int i=0; i<NJD; i++)
	{
		if(JD2[i].JDXZ == B_X)//���жν���
		{
			JD2[i].jdno = JD0 + i;

			int jdn1 = JD2[i].jdno1;//��Ӧһ�߽������
			if(JD2[i].ISXJJFirst)
			{
				if(i == NJD-1)
					JD2[i].xjj = JD2[i-1].xjj;//���һ������߼�����ǰһ�����
				double xjj = JD2[i].xjj;				
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

Adesk::Boolean  GTZX2_JYX_ROAD::G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,bool ISShow)
{     
	int j;
	double l=0.0;
	AcGePoint3d ptb,pt,pte;
	//	if(INDATA[1]<0.00000001) return 1;
	l=Get_INDATA_L(INDATA,&j);

	//////////////////////////////////////////////////////////
	PZ->x = P0->x;
	PZ->y = P0->y; 
	PZ->a= P0->a;

	//Բ+��·����Ϊ0
	if( fabs(INDATA[0]-2)<0.00001 && l<0.00000001 ) PZ->a = INDATA[3];

	PZ->lc = P0->lc;
	PZ->r = P0->r;
	if(l<0.00000001)
		return 1;

	ptb[X]=P0->y;ptb[Y]=P0->x; ptb[Z]=0.0;

	if ((j<=3 &&j>=6 && INDATA[1]<=0.000000000000001)|| (l<=0.00000000001)) return 1;

	if (j>2 && j<=6 )
	{ //( ������)
		if(ISShow)G_drawspi(pWd,INDATA,l,P0,PZ, 2.0);
		xlzbxy(P0,PZ,l,INDATA,0);// ĳ��Ԫ�յ�
	}
	else if (j<=2)  // R L
	{//( ԰�� ֱ��)
		xlzbxy(P0,PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
		pt[X]=PZ->y;pt[Y]=PZ->x; pt[Z]=0.0;
		xlzbxy(P0,PZ,l,INDATA,0); // ĳ��Ԫ�յ�
		pte[X]=PZ->y;pte[Y]=PZ->x; pte[Z]=0.0; //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
		if (j==1) //ֱ��
		{	 AcGePoint3d pcArr[2];
		pcArr[0]=ptb; pcArr[1]=pte;
		if( sqrt( (P0->x-PZ->x)*(P0->x-PZ->x) + (P0->y-PZ->y)*(P0->y-PZ->y) ) >= T0min )
		{
			pWd->subEntityTraits().setColor(JiaZhiXianColorZuo);	
		}
		else
		{
			pWd->subEntityTraits().setColor(JiaZhiXianColorYou);	
		}
		if(ISShow)pWd->geometry().polyline(2,pcArr);
		// ads_printf(L"\n L ����= 1"  );
		}
		else if (j==2) // ԰�� 
		{ 	pWd->subEntityTraits().setColor(XWArcColor);	
		if(ISShow)pWd->geometry().circularArc(ptb,pt,pte);
		// ads_printf(L"\n r ����= 2"  );
		};
	};  //J
	// ads_printf(L"\n PZ=%10.3f %10.3f %10.3f %10.3f",PZ->y,PZ->x,dms_rad(PZ->a),PZ->lc) ;
	return 0;
}


//	��ע����� �����߲���
//void GTZX2::BZJD(AcGiWorldDraw* pWd,double arrayd[100][6],double qxb[100][9],int njd,int JD0,ACHAR *JDchr)
void GTZX2_JYX_ROAD::BZJD(AcGiWorldDraw* pWd,CArray<JDarrayStruct,JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb,ACHAR *JDchr)
{
	int njd = arrayd.GetSize();
	double fb,fe,de,da,aa,dd,dt ;
	AcGePoint3d  p1; 
	ACHAR ch[80],ch1[60],chrtmp[80];
	int i,c,n;
	de=xyddaa(arrayd[0].N,arrayd[0].E,arrayd[1].N,arrayd[1].E,&fb);
	for (i=1;i<njd-1;i++)
	{  
		if (arrayd[i].R<0) break;
		de=xyddaa(arrayd[i].N,arrayd[i].E,arrayd[i+1].N,arrayd[i+1].E,&fe);


		aa= fe - fb ;
		if (aa<-pi) aa=aa+2*pi;
		if  (aa>pi) aa=aa-2*pi;
		if (aa>0) {  c=1; dd=2*texth;  }  //��ƫ
		else { c=-1; dd= 2*texth; }; //��ƫ
		de=fb+aa*0.5;

		//	ads_rtos(JD0+i,2,0,ch);
		//	_tcscpy(ch1,JDchr);
		//	_tcscat(ch1,ch);
		_tcscpy(ch1,arrayd[i].JDNum);

		n=_tcslen(ch1);
		p1[Z]=0.0;

		p1[X]=arrayd[i].E-n*0.5*texth*sin(de)+dd*sin(de+c*pi*0.5);
		p1[Y]=arrayd[i].N-n*0.5*texth*cos(de)+dd*cos(de+c*pi*0.5);

		da= m_dHalfPI-de;
		if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);    // �����  
		// maketext( p1,ch1,de,texth ,4,1);  
		//	 if(JD2[i].jdno1==0||JD2[i].jdno1==m_pm1->NJD-1)continue;
		if( qxbbz==1)
		{
			// �����߲��� begin
			double R,T,T1,T2,L1,L2,LL,NYDL;
			R=arrayd[i].R; L1= arrayd[i].l1;  L2=arrayd[i].l2; 
			T1=qxb[i].T1; T2=qxb[i].T2;  LL=qxb[i].L;
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
			p1[X]=arrayd[i].E-dd*sin(de-c*pi*0.5) ; 
			p1[Y]=arrayd[i].N-dd*cos(de-c*pi*0.5);
			_tcscpy(chrtmp,L"  "); 
			_tcscat(chrtmp,ch1);
			if(JD2[i].ISShow)G_maketext(pWd,p1,chrtmp,da,texth ,QXYSColor,1);   // �����

			ads_rtos(qxb[i].a ,2,4,ch);
			if (c==1) { _tcscpy(ch1,L"ay--"); }
			else _tcscpy(ch1,L"az--"); 
			_tcscat(ch1,ch);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //  ƫ��

			if (fabs(R-(long int)(R))<0.001)  { ads_rtos(R,2,0,ch); }
			else { ads_rtos(R,2,NLC,ch); };
			_tcscpy(ch1,L"R--"); _tcscat(ch1,ch);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   �뾶

			if (L1>0||L2>0)  //  ������Ϊ��
			{
				if (fabs(L1-(long int)(L1))<0.001)  { ads_rtos(L1,2,NLC,ch); }
				else { ads_rtos(L1,2,NLC,ch); };
				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if (fabs(L1-L2)<0.001) 
				{ _tcscpy(ch1,L"Lo--");  _tcscat(ch1,ch);
				if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   ���� ���
				} else
				{ _tcscpy(ch1,L"L1--");  _tcscat(ch1,ch);
				if (fabs(L2-(long int)(L2))<0.001)  { ads_rtos(L2,2,NLC,ch); }
				else { ads_rtos(L2,2,NLC,ch); };
				_tcscat(ch1,L"  L2--"); _tcscat(ch1,ch);
				if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   ���Ȼ���
				};
			};

			if (fabs(T1-(long int)(T1))<0.001)  { ads_rtos(T1,2,0,ch); }
			else { ads_rtos(T1,2,NLC,ch); };
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if (fabs(L1-L2)<0.001) 
			{   _tcscpy(ch1,L"T--");  _tcscat(ch1,ch);
			if(JD2[i].ISShow) G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   �г� ���
			} else
			{   _tcscpy(ch1,L"T1--");  _tcscat(ch1,ch);
			ads_rtos(T2,2,NLC,ch);
			_tcscat(ch1,L"  T2--"); _tcscat(ch1,ch);
			if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   �����г�
			};

			ads_rtos(LL,2,NLC,ch);
			_tcscpy(ch1,L"L--");  _tcscat(ch1,ch);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //  ���߳�
			//��ע��ҵ����
			if(JD2[i].JDXZ!=R_X && JD2[i].JDXZ!=R_X_OneR && JD2[i].JDXZ!=R_XStart && JD2[i].JDXZ!=R_XEnd)//���н���/S��
			{
				NYDL = JD2[i].NYDL;
				ads_rtos(NYDL,2,NLC,ch1);
				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if(JD2[i].ISShow)G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   // ��ҵ����
			}
			// �����߲��� end
		};
		fb=fe; 
	};   


}

//����һ�߽���ţ����߶�Ӧ����ţ����ز��жζ��߽�������
void GTZX2_JYX_ROAD::CalEXJD(int jdn1,int njd2,double EXJD[2])
{

	double xjj1,xjj2,fwj;

	if(jdn1<m_pm1->NJD-1)
		xyddaa(m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,m_pm1->JDarray[jdn1+1].N,m_pm1->JDarray[jdn1+1].E,&fwj);
	else
		xyddaa(m_pm1->JDarray[jdn1-1].N,m_pm1->JDarray[jdn1-1].E,m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,&fwj);

	if(njd2==0|| njd2==NJD-1)//��ʼ����//plh05.06
	{
		//�ɸõ���һ���ϵ�ͶӰ��̺��߼�����
		//Ҫ��GTZX2�����յ��ZHLCҪ��

		if(jdn1==0|| jdn1==m_pm1->NJD-1)
		{
			double xjj;
			xjj = JD2[njd2].xjj;
			EXJD[0]  =  m_pm1->JDarray[jdn1].N + xjj * cos(fwj+m_dHalfPI);
			EXJD[1]  =  m_pm1->JDarray[jdn1].E + xjj * sin(fwj+m_dHalfPI);//��������߽���
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

		xyddaa(m_pm1->JDarray[jdn1-1].N,m_pm1->JDarray[jdn1-1].E,m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,&fwj);
		double pt1[2],pt2[2];
		pt1[0] = m_pm1->JDarray[jdn1-1].N + xjj1 * cos(fwj+m_dHalfPI);
		pt1[1] = m_pm1->JDarray[jdn1-1].E + xjj1 * sin(fwj+m_dHalfPI);
		pt2[0] = m_pm1->JDarray[jdn1].N + xjj1 * cos(fwj+m_dHalfPI);
		pt2[1] = m_pm1->JDarray[jdn1].E + xjj1 * sin(fwj+m_dHalfPI);
		double pt3[2],pt4[2];
		xyddaa(m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,m_pm1->JDarray[jdn1+1].N,m_pm1->JDarray[jdn1+1].E,&fwj);
		pt3[0] = m_pm1->JDarray[jdn1].N + xjj2 * cos(fwj+m_dHalfPI);
		pt3[1] = m_pm1->JDarray[jdn1].E + xjj2 * sin(fwj+m_dHalfPI);
		pt4[0] = m_pm1->JDarray[jdn1+1].N + xjj2 * cos(fwj+m_dHalfPI);
		pt4[1] = m_pm1->JDarray[jdn1+1].E + xjj2 * sin(fwj+m_dHalfPI);
		//�ҽ���

		double dist;
		dist = sqrt((pt2[0]-pt3[0])*(pt2[0]-pt3[0])+(pt2[1]-pt3[1])*(pt2[1]-pt3[1]));
		if(dist<0.001)
		{
			EXJD[0] = pt2[0];
			EXJD[1] = pt2[1];
		}
		else
			INTERS(&EXJD[0],&EXJD[1],pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

	}
}
int GTZX2_JYX_ROAD::INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4)
{
	double A11,A12,A21,A22,B1,B2,D,D1,D2;
	int INF;
	double X1min,Y1min,X2min,Y2min,X1max,Y1max,X2max,Y2max;
	double dist;

	dist = sqrt((X2-X3)*(X2-X3)+(Y2-Y3)*(Y2-Y3));

	if(dist<0.001)
	{
		*x = X2;
		*y = Y2;
		INF = 0;
		return INF; 
	}

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
void  GTZX2_JYX_ROAD::CalNYDL()//plh05.14
{
	for(int i=0; i<NJD; i++)
	{
		if(JD2[i].JDXZ==B_X)//���жν���
		{
			////1.ȷ�����㷶Χ
			//double ZHml2,HZml2;//�����ϵ�ֱ�����
			//ZHml2 = QXB[i].KZH , HZml2 = QXB[i].KHZ;
			//double ZHpt2[2],HZpt2[2];
			//xlpoint PZ;
			//PZ.lc = ZHml2;
			//xlpoint_pz(array,tarray,&PZ);
			//ZHpt2[0] = PZ.x ,ZHpt2[1] = PZ.y;
			//PZ.lc = HZml2;
			//xlpoint_pz(array,tarray,&PZ);
			//HZpt2[0] = PZ.x ,HZpt2[1] = PZ.y;
			//double ZHml2XIn1X,HZml2XIn1X;
			//ZHml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,ZHpt2[0],ZHpt2[1],ZHpt2[0],ZHpt2[1]);
			//HZml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,HZpt2[0],HZpt2[1],HZpt2[0],HZpt2[1]);
			//double ZHml1,HZml1;//1��ZH,HZ���
			//ZHml1 = m_pm1->QXB[JD2[i].jdno1].KZH;
			//HZml1 = m_pm1->QXB[JD2[i].jdno1].KHZ;
			//double sml,eml;
			//sml = ZHml2XIn1X < ZHml1 ?  ZHml2XIn1X : ZHml1;
			//eml = HZml2XIn1X > HZml1 ?  HZml2XIn1X : HZml1;
			////2.������һ�߳���
			//double len1 = eml - sml;
			////3.�����ڶ��߳���
			//double len2;
			//double spt1[2],ept1[2];//1��sml,eml����
			//double sml2,eml2,spt2[2],ept2[2];
			//int LorR;

			//PZ.lc = sml;
			//xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);
			//spt1[0] = PZ.x ,spt1[1] = PZ.y;//һ������
			//LorR = Judge2XLorR(PZ,array,tarray);
			//if(LorR==0)//������һ���غ�518
			//{
			//	sml2 = PROJ_ML(array,tarray,spt1[0],spt1[1],spt1[0],spt1[1]);//ͶӰ��2��
			//	spt2[0]=spt1[0];
			//	spt2[1]=spt1[1];
			//}
			//else
			//{
			//	sml2 = Search_EXPt(array,tarray,spt1,PZ.a+LorR*0.5*pi,spt2);//���߶�Ӧ���
			//}


			//PZ.lc = eml;
			//xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);
			//ept1[0] = PZ.x ,ept1[1] = PZ.y;//һ������
			//LorR = Judge2XLorR(PZ,array,tarray);
			//if(LorR==0)//������һ���غ�518
			//{
			//	eml2 = PROJ_ML(array,tarray,ept1[0],ept1[1],ept1[0],ept1[1]);//ͶӰ��2��
			//	ept2[0]=ept1[0];
			//	ept2[1]=ept1[1];
			//}
			//else
			//{
			//	eml2 = Search_EXPt(array,tarray,ept1,PZ.a+LorR*0.5*pi,ept2);//���߶�Ӧ���
			//}

			//len2 = eml2 - sml2;

			////		   acutPrintf(L"jd=%d,sml=%lf,eml=%lf,sml2=%lf,eml2=%lf,ept2x=%lf,ept2y=%lf\n",i,sml,eml,sml2,eml2,ept2[0],ept2[1]);
			////4.��ҵ����
			//if(i>0&&i<NJD-1&&fabs(len2-len1)>0.01)//��
			//	JD2[i].NYDL = len2 - len1 + 100.0;
			//else
			//	JD2[i].NYDL = 0.0;


			double ZHml2,HZml2;//�����ϵ�ֱ�����
			ZHml2 = QXB[i].KZH , HZml2 = QXB[i].KHZ;
			double ZHpt2[2],HZpt2[2];
			xlpoint PZ;
			PZ.lc = ZHml2;
			xlpoint_pz(array,tarray,&PZ);
			ZHpt2[0] = PZ.x ,ZHpt2[1] = PZ.y;
			PZ.lc = HZml2;
			xlpoint_pz(array,tarray,&PZ);
			HZpt2[0] = PZ.x ,HZpt2[1] = PZ.y;

			//double ZHml2XIn1X,HZml2XIn1X;
			//ZHml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,ZHpt2[0],ZHpt2[1],ZHpt2[0],ZHpt2[1]);
			//HZml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,HZpt2[0],HZpt2[1],HZpt2[0],HZpt2[1]);
			//JD2[i].NYDL=(HZml2-ZHml2)-(HZml2XIn1X-ZHml2XIn1X);

			//////////////////////////////////////////////////////////////////////////
			double dmlZH,dmlHZ;
			ACHAR ckml[80],GH[20];
			CString xclc;

			if( _tcscmp( xlname , L"��ͨ��Ƣ���" ) == 0 )
			{
				xclc = GetGTX2Xclc(ZHpt2[0],ZHpt2[1]);
				_tcscpy(ckml,xclc);
				split_ckml(ckml,dmlZH,GH);
				xclc = GetGTX2Xclc(HZpt2[0],HZpt2[1]);
				_tcscpy(ckml,xclc);
				split_ckml(ckml,dmlHZ,GH);
			}
			else
			{
				if (NDL>0)
				{
					dmlZH = XCLC(ZHml2 ,GH , DLB2x, NDL );
					dmlHZ = XCLC(HZml2 ,GH , DLB2x, NDL );
				}
				else
				{
					dmlZH = ZHml2;
					dmlHZ = HZml2;
				}
			}
			JD2[i].NYDL=(HZml2-ZHml2)-(dmlHZ-dmlZH);
			//////////////////////////////////////////////////////////////////////////

			if(fabs(JD2[i].NYDL)<0.01)JD2[i].NYDL=0.0;
			else JD2[i].NYDL+=100.0;


			//�Ӿ��޸�08.9.25������ֱ�߶���ҵ����
			if (i==NJD-1)
			{
				JD2[i].ZXNYDL=0;
				continue;
			}
			double sml,eml;
			sml = QXB[i].KHZ , eml = QXB[i+1].KZH;
			JD2[i].ZXNYDL=CalZXNYDL(sml,eml);



		}//����
		else if(JD2[i].JDXZ==S_StartPoint||JD2[i].JDXZ==S_EndPoint||JD2[i].JDXZ==B_XNOTP_X)//S��
		{
			double ZHml2,HZml2;//�����ϵ�ֱ�����
			ZHml2 = QXB[i].KZH , HZml2 = QXB[i].KHZ;
			double ZHpt2[2],HZpt2[2];
			xlpoint PZ;
			PZ.lc = ZHml2;
			xlpoint_pz(array,tarray,&PZ);
			ZHpt2[0] = PZ.x ,ZHpt2[1] = PZ.y;
			PZ.lc = HZml2;
			xlpoint_pz(array,tarray,&PZ);
			HZpt2[0] = PZ.x ,HZpt2[1] = PZ.y;

			//double ZHml2XIn1X,HZml2XIn1X;
			//ZHml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,ZHpt2[0],ZHpt2[1],ZHpt2[0],ZHpt2[1]);
			//HZml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,HZpt2[0],HZpt2[1],HZpt2[0],HZpt2[1]);
			//JD2[i].NYDL=(HZml2-ZHml2)-(HZml2XIn1X-ZHml2XIn1X);

			//////////////////////////////////////////////////////////////////////////
			double dmlZH,dmlHZ;
			ACHAR ckml[80],GH[20];
			CString xclc;

			if( _tcscmp( xlname , L"��ͨ��Ƣ���" ) == 0 )
			{
				xclc = GetGTX2Xclc(ZHpt2[0],ZHpt2[1]);
				_tcscpy(ckml,xclc);
				split_ckml(ckml,dmlZH,GH);
				xclc = GetGTX2Xclc(HZpt2[0],HZpt2[1]);
				_tcscpy(ckml,xclc);
				split_ckml(ckml,dmlHZ,GH);
			}
			else
			{
				if (NDL>0)
				{
					dmlZH = XCLC(ZHml2 ,GH , DLB2x, NDL );
					dmlHZ = XCLC(HZml2 ,GH , DLB2x, NDL );
				}
				else
				{
					dmlZH = ZHml2;
					dmlHZ = HZml2;
				}
			}
			JD2[i].NYDL=(HZml2-ZHml2)-(dmlHZ-dmlZH);
			//////////////////////////////////////////////////////////////////////////

			if(fabs(JD2[i].NYDL)<0.01)JD2[i].NYDL=0.0;
			else JD2[i].NYDL+=100.0;

			//�Ӿ��޸�08.9.25������ֱ�߶���ҵ����
			if (i==NJD-1)
			{
				JD2[i].ZXNYDL=0;
				continue;
			}
			double sml,eml;
			sml = QXB[i].KHZ , eml = QXB[i+1].KZH;
			JD2[i].ZXNYDL=CalZXNYDL(sml,eml);

		}
		else	JD2[i].NYDL=0.0;
	}

}


double GTZX2_JYX_ROAD::CalZXNYDL(double sml,double eml)//�������յ���̼���ֱ�߶���ҵ����
{
	//double sml,HZml2;//�����ϵ�ֱ�����
	//sml = QXB[i].KZH , HZml2 = QXB[i].KHZ;
	double ZHpt2[2],HZpt2[2];
	xlpoint PZ;
	PZ.lc = sml;
	xlpoint_pz(array,tarray,&PZ);
	ZHpt2[0] = PZ.x ,ZHpt2[1] = PZ.y;
	PZ.lc = eml;
	xlpoint_pz(array,tarray,&PZ);
	HZpt2[0] = PZ.x ,HZpt2[1] = PZ.y;
	double ZHml2XIn1X,HZml2XIn1X;
	ZHml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,ZHpt2[0],ZHpt2[1],ZHpt2[0],ZHpt2[1]);
	HZml2XIn1X = PROJ_ML(m_pm1->array,m_pm1->tarray,HZpt2[0],HZpt2[1],HZpt2[0],HZpt2[1]);
	double ZXNYDL=(eml-sml)-(HZml2XIn1X-ZHml2XIn1X);
	if(fabs(ZXNYDL)<0.0000000001)ZXNYDL=0.0;
	else ZXNYDL+=100.0;

	return ZXNYDL;
}

void GTZX2_JYX_ROAD::CheckJD2IsAvailable()
{
	double aa = 0.0 ,fb = 0.0 ,fe = 0.0 , a1 = 0.0 ,a2 = 0.0 ;
	if(NJD<2)return ;
	xyddaa(JD2[0].x,JD2[0].y,JD2[1].x,JD2[1].y,&fb);

	for(int i=1;i<NJD-1;i++)
	{
		xyddaa(JD2[i].x,JD2[i].y,JD2[i+1].x,JD2[i+1].y,&fe);
		aa = ZXJ(fe,fb);
		aa=fabs(aa);

		if(JD2[i].R<1.0e-5)
		{
			fb = fe ;
			continue ;
		}

		a1 = JD2[i].Lo1 / ( 2.0 * JD2[i].R );
		a2 = JD2[i].Lo2 / ( 2.0 * JD2[i].R );

		if((aa-a1-a2)<0.0)
		{
			JD2[i].Lo1 = m_pm1->JDarray[i].l1 ;
			JD2[i].Lo2 = m_pm1->JDarray[i].l2 ;
			//ȡһ������
			a1 = JD2[i].Lo1 / ( 2.0 * JD2[i].R );
			a2 = JD2[i].Lo2 / ( 2.0 * JD2[i].R );

			if( (aa-a1-a2)<0.0 )
			{
				JD2[i].Lo1 = 0 ;
				JD2[i].Lo2 = 0 ;
			}
		}
		fb = fe ;
	}
}

void GTZX2_JYX_ROAD::SetJDdata()//���ɶ���JDarray
{
	TagEXJDIsShow();
	JDarray.RemoveAll();
	JDarray.SetSize(NJD);

	for(int i=0; i<NJD; i++)
	{
		if( fabs(JD2[i].xjj)>0.5 )
		{
			if(JD2[i].xjj>0)LEFT_OR_RIGHT=RIGHTLINE;//�߼�����0����
			else LEFT_OR_RIGHT=LEFTLINE;//�߼��С��0����			
		}

		_tcscpy(JDarray[i].JDNum,JD2[i].JDNum );
		//	_stprintf(JDarray[i].JDNum,L"%d",JD0 + i);
		//	JDarray[i][0] = JD0 + i;//�����
		JDarray[i].N = JD2[i].x; //x->N,
		JDarray[i].E = JD2[i].y;//y->E
		JDarray[i].l1 = JD2[i].Lo1;
		JDarray[i].R = JD2[i].R;	
		JDarray[i].l2 = JD2[i].Lo2;
	}

}

// Files data out to a DWG file.
Acad::ErrorStatus GTZX2_JYX_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{ 
	assertReadEnabled();
	Acad::ErrorStatus es;
	if((es=AcDbEntity::dwgOutFields(pFiler))!=Acad::eOk)
	{
		return es;
	}

	//	pFiler->writeAddress(m_pm1);
	pGtzxTemp = NULL ;
	pGtzxTemp = m_pm1;

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


	pFiler->writeItem(GTZXJDToJDColor);
	pFiler->writeItem(XWArcColor);
	pFiler->writeItem(HuanHequxianColor);
	pFiler->writeItem(JiaZhiXianColorZuo);
	pFiler->writeItem(JiaZhiXianColorYou);
	pFiler->writeItem(mdbnameBoxColor);
	pFiler->writeItem(mdbnameTextColor);
	pFiler->writeItem(QXYSColor);

	pFiler->writeItem((int )LineWeightValue);

	int i;
	for( i=0;i<NJD;i++)
	{
		pFiler->writeItem(JD2[i].XLName);
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
		chr=_tcsdup(JD2[i].ZHLC);  pFiler->writeItem(chr);
		//pFiler->writeString(JD2[i].ZHLC);
		pFiler->writeBool(JD2[i].ISXJJFirst);
		pFiler->writeItem(JD2[i].JDNum);
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
	}	

	pFiler->writeItem(xlname);
	pFiler->writeItem(zory);	
	pFiler->writeItem(NGJD);
	for(i=0;i<NGJD;i++)
	{
		pFiler->writeDouble(GJDInfTab[i].spt[0]);
		pFiler->writeDouble(GJDInfTab[i].spt[1]);
		pFiler->writeDouble(GJDInfTab[i].ept[0]);
		pFiler->writeDouble(GJDInfTab[i].ept[1]);
		pFiler->writeItem(GJDInfTab[i].ID);
		const ACHAR *chr=NULL;
		chr=_tcsdup(GJDInfTab[i].gjdname); 
		pFiler->writeItem(chr);		
	}

	pFiler->writeItem(GJDInforArrayNum);

	int j;
	for (j=0;j<GJDInforArrayNum;j++)
	{   
		pFiler->writeItem( GJDInforArray[j].spt[0]);
		pFiler->writeItem( GJDInforArray[j].spt[1]);
		pFiler->writeItem( GJDInforArray[j].ept[0]);
		pFiler->writeItem( GJDInforArray[j].ept[1]);
		pFiler->writeItem( GJDInforArray[j].GJDType);


		pFiler->writeItem( GJDInforArray[j].BXLName);
		pFiler->writeItem( GJDInforArray[j].EXLName);
		pFiler->writeItem( GJDInforArray[j].gjdname);

		pFiler->writeItem( GJDInforArray[j].GJDBLC);
		pFiler->writeItem( GJDInforArray[j].GJDELC);

	}

	pFiler->writeItem(MidPointArrayNum);

	for (j=0;j<MidPointArrayNum;j++)
	{   
		pFiler->writeItem( MidPointArray[j].x );
		pFiler->writeItem( MidPointArray[j].y );
		pFiler->writeItem( MidPointArray[j].z );
	}

	pFiler->writeItem(NBZLC);

	for (j=0;j<NBZLC;j++)
	{   
		pFiler->writeString(BZLCarray[j].kml);
		pFiler->writeItem(BZLCarray[j].X);
		pFiler->writeItem(BZLCarray[j].Y);
	};

	pFiler->writeItem(m_XLNameNotePt);

	return pFiler->filerStatus();
}
// Files data in from a DWG file.


Acad::ErrorStatus GTZX2_JYX_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;
	if((es=AcDbEntity::dwgInFields(pFiler))!=Acad::eOk)
	{
		return es;
	}
	ACHAR *temp=NULL;
	//	void *tpointer;	
	//	pFiler->readAddress(&tpointer);
	//	m_pm1=(GTZX*)tpointer;


	m_pm1 = pGtzxTemp ;

	pGtzxTemp = NULL ;

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


	//��ɫ����
	pFiler->readItem(&GTZXJDToJDColor);
	pFiler->readItem(&XWArcColor);
	pFiler->readItem(&HuanHequxianColor);
	pFiler->readItem(&JiaZhiXianColorZuo);
	pFiler->readItem(&JiaZhiXianColorYou);
	pFiler->readItem(&mdbnameBoxColor);
	pFiler->readItem(&mdbnameTextColor);
	pFiler->readItem(&QXYSColor);

	pFiler->readItem((int *)&LineWeightValue);

	int i;
	for(i=0;i<NJD;i++)
	{
		pFiler->readItem(&temp);
		_tcscpy(JD2[i].XLName,temp);	

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
		pFiler->readItem(&temp);
		_tcscpy( JD2[i].JDNum,temp);
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
	_tcscpy(xlname,temp);

	pFiler->readItem(&zory);
	GJDInfTab.RemoveAll();
	pFiler->readItem(&NGJD);
	GJDInfTab.SetSize(NGJD);
	for(i=0;i<NGJD;i++)
	{
		pFiler->readDouble(&GJDInfTab[i].spt[0]);
		pFiler->readDouble(&GJDInfTab[i].spt[1]);
		pFiler->readDouble(&GJDInfTab[i].ept[0]);
		pFiler->readDouble(&GJDInfTab[i].ept[1]);
		pFiler->readItem(&GJDInfTab[i].ID);
		pFiler->readItem(&temp);
		_tcscpy(GJDInfTab[i].gjdname,temp);		
	}


	pFiler->readItem(&GJDInforArrayNum);
	GJDInforArray.SetSize(GJDInforArrayNum);
	int j;
	for (j=0;j<GJDInforArrayNum;j++)
	{   
		pFiler->readItem( &GJDInforArray[j].spt[0]);
		pFiler->readItem( &GJDInforArray[j].spt[1]);
		pFiler->readItem( &GJDInforArray[j].ept[0]);
		pFiler->readItem( &GJDInforArray[j].ept[1]);
		pFiler->readItem( &GJDInforArray[j].GJDType);

		pFiler->readItem(&temp); 
		_tcscpy(GJDInforArray[j].BXLName , temp); 

		pFiler->readItem(&temp); 
		_tcscpy(GJDInforArray[j].EXLName , temp); 

		pFiler->readItem(&temp); 
		_tcscpy(GJDInforArray[j].gjdname , temp); 

		//������
		pFiler->readItem(&temp); 
		_tcscpy(GJDInforArray[j].GJDBLC , temp); 

		//�յ����
		pFiler->readItem(&temp); 
		_tcscpy(GJDInforArray[j].GJDELC , temp); 

	}


	pFiler->readItem(&MidPointArrayNum);
	MidPointArray.SetSize(MidPointArrayNum);
	for (j=0;j<MidPointArrayNum;j++)
	{   
		pFiler->readItem( &MidPointArray[j].x );
		pFiler->readItem( &MidPointArray[j].y );
		pFiler->readItem( &MidPointArray[j].z );
	}

	pFiler->readItem(&NBZLC);

	BZLCarray.RemoveAll();
	BZLCarray.SetSize(NBZLC);

	ACHAR* chr=NULL;

	for (j=0;j<NBZLC;j++)
	{
		pFiler->readString(&chr);
		_tcscpy(BZLCarray[j].kml,chr);
		pFiler->readItem(&BZLCarray[j].X);
		pFiler->readItem(&BZLCarray[j].Y);
	}


	pFiler->readItem(&m_XLNameNotePt);

	return pFiler->filerStatus();
}


void GTZX2_JYX_ROAD::Create2XDLB()//���ɶ����������жεĶ�����
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
	int RXDnum = GetRXDNum();//���ж���Ŀ
	for(int iRXD = 0;iRXD < RXDnum; iRXD++)
		Create2XDLB(iRXD);	
}

//1.�ж��Ƿ���ӳ���������
// TRUE ��·�ϡ���FALSE  �ӳ�����
// JDNum -- ���߽����(����)
BOOL GTZX2_JYX_ROAD::JudgeCurJDIsOnXL( int JDNum , BOOL IsBefore )
{
	//�����߼��
	double xjj = JD2[JDNum].xjj ;
	////���߶�Ӧ��ZH���ֳ����
	//double ZHkml2x = QXB[JDNum].KZH ;

	AcGePoint3d PT1,PT2; //ƽ�ƶ��󽻵�
	double cml = 0.0 ;
	xlpoint PZTemp ; 
	int JDNum1x = 0 ;
	if(IsBefore)//��ǰ��
	{
		//ǰһ��HZ��(����)2��
		cml = QXB[JDNum].KHZ ;

		PZTemp.lc = cml ;
		xlpoint_pz(array,tarray,&PZTemp);

		//��Ӧһ�߽����
		JDNum1x = JD2[JDNum].jdno1 ;

		if(JDNum1x>=m_pm1->JDarray.GetSize()-2)return TRUE ;
		//�����ƶ�һ����


		PT1.set(m_pm1->JDarray[JDNum1x+1].N,m_pm1->JDarray[JDNum1x+1].E,0.0);
		PT2.set(m_pm1->JDarray[JDNum1x+2].N,m_pm1->JDarray[JDNum1x+2].E,0.0);

	}
	else // ����� 
	{

		//ǰһ��ZH��(����)2��
		cml = QXB[JDNum].KZH ;

		PZTemp.lc = cml ;
		xlpoint_pz(array,tarray,&PZTemp);

		//��Ӧһ�߽����±�
		JDNum1x = JD2[JDNum].jdno1 ;

		if(JDNum1x<2)return TRUE ;
		//�����ƶ�һ����
		PT1.set(m_pm1->JDarray[JDNum1x-2].N,m_pm1->JDarray[JDNum1x-2].E,0.0);
		PT2.set(m_pm1->JDarray[JDNum1x-1].N,m_pm1->JDarray[JDNum1x-1].E,0.0);
	}


	double PtoPfwj = fwj(PT1,PT2);


	if(xjj>=0)
	{
		PT1.x = PT1.x + xjj * cos( PtoPfwj + pi /2.0 ) ;
		PT1.y = PT1.y + xjj * sin( PtoPfwj + pi /2.0 ) ;
	}
	else
	{
		PT1.x = PT1.x - xjj * cos( PtoPfwj - pi /2.0 ) ;
		PT1.y = PT1.y - xjj * sin( PtoPfwj - pi /2.0 ) ;
	}

	AcDbLine *LineL1=new AcDbLine(PT1,PT2);


	if(IsBefore)//��ǰ��
	{
		PT1.set(JD2[JDNum].x,JD2[JDNum].y,0.0);
		PT2.set(JD2[JDNum+1].x,JD2[JDNum+1].y,0.0);
	}
	else 
	{
		PT1.set(JD2[JDNum].x,JD2[JDNum].y,0.0);
		PT2.set(JD2[JDNum-1].x,JD2[JDNum-1].y,0.0);
	}

	AcDbLine *LineL2=new AcDbLine(PT1,PT2);

	AcGePoint3dArray interPt;
	if( LineL2->intersectWith(LineL1,AcDb::kExtendBoth,interPt,0,0) == Acad::eOk && interPt.logicalLength()>0)
	{
		//2����Ƚϴ�С
		double dist1 = 0.0 ,dist2 = 0.0 ;

		double dx = 0.0 ,dy = 0.0 ;
		dx = PZTemp.x - interPt[0].x ;
		dy = PZTemp.y - interPt[0].y ;
		dist1 = pow( dx , 2.0 ) + pow( dy , 2.0 );

		dist1 = sqrt(dist1);

		if(IsBefore)//��ǰ��
		{
			dx = PZTemp.x - JD2[JDNum+1].x ;
			dy = PZTemp.y - JD2[JDNum+1].y ;
		}
		else
		{
			dx = PZTemp.x - JD2[JDNum-1].x ;
			dy = PZTemp.y - JD2[JDNum-1].y ;
		}
		dist2 = pow( dx , 2.0 ) + pow( dy , 2.0 );

		dist2 = sqrt(dist2);

		if(dist1>dist2 - 1.0 )return TRUE ; //��·��
		else return FALSE ; //�ӳ�����

	}
	else return TRUE ;

}

//���ɶ���ָ�����жεĶ�����
void GTZX2_JYX_ROAD::Create2XDLB(int p_iRXD,ACHAR EGH[],bool IsUpdate)
{

	int firstJDXH,endJDXH;//��β�������
	//��ȡָ�����ж���ʼ�������
	GetJDXHofiRXD(p_iRXD,firstJDXH,endJDXH);

	if(firstJDXH == -1 || endJDXH == -1 )
		return ;
	//ָ�����жε�һ��ֱ���㣬���һ����ֱ���ͳһ���
	double StartZhTylc,EndHzTylc;


	//-------------------------------------------------
	//1.��ȡָ��II�����жε�һ��ֱ���㣬���һ����ֱ�������
	double x1,y1,x2,y2;
	xlpoint sPt,ePt; // ,temtPt;

	StartZhTylc = QXB[firstJDXH].KZH;
	EndHzTylc   = QXB[endJDXH].KHZ;

	sPt.lc=StartZhTylc;
	xlpoint_pz(array,tarray,&sPt);
	x1=sPt.x;
	y1=sPt.y;

	ePt.lc=EndHzTylc;
	xlpoint_pz(array,tarray,&ePt);
	x2=ePt.x;
	y2=ePt.y;
	//-------------------------------------------------



	GTZX_JYX_ROAD *pGtzxTmp = NULL;

	if(m_pm2 == NULL)
		pGtzxTmp = m_pm1;
	else 
		pGtzxTmp = m_pm2->m_pm1;

	//ָ�����жε�һ��ֱ�����,���һ����ֱ�����һ���ϵ�ͶӰͳһ��̣�
	double sTYLC1x0,eTYLC1x0;
	//��������һ���ϵ�ͶӰͳһ��̣�
	double sTYLC1x,eTYLC1x;
	//�������յ��ͳһ��̣������ϡ�
	double stylc2x,etylc2x;

	//�״�ͶӰ��һ�ߵ�ͳһ���
	sTYLC1x0 = PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,x1,y1,x1,y1);
	eTYLC1x0 = PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,x2,y2,x2,y2);

	//////////////////////////////////////////////////////////////////////////
	//�ж�ͶӰ���Ƿ���ֱ�߱���
	//double QXSml,QXEml;
	//for (int iJD=0; iJD<pGtzxTmp->NJD-1; iJD++)
	//{
	//	QXSml = pGtzxTmp->QXB[iJD].KZH;
	//	QXEml = pGtzxTmp->QXB[iJD].KHZ;
	//	if (stylc1x0>QXSml && stylc1x0<QXEml)//�����������
	//	{
	//		stylc1x0 = pGtzxTmp->QXB[iJD].KZH;
	//	}
	//	if (etylc1x0>QXSml && etylc1x0<QXEml)//�յ���������
	//	{
	//		etylc1x0 = pGtzxTmp->QXB[iJD].KHZ;
	//		break;
	//	}
	//}

	int RXSJDIndex1x,RXEJDIndex1x;//���ж����յ��Ӧ��һ�ߵĽ����
	RXSJDIndex1x = pGtzxTmp->DistToNearJD(sTYLC1x0,false);
	RXEJDIndex1x = pGtzxTmp->DistToNearJD(eTYLC1x0,true);

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
		Spt1x.x = pGtzxTmp->JDarray[iJD1x].E;
		Spt1x.y = pGtzxTmp->JDarray[iJD1x].N;
		Ept1x.x = pGtzxTmp->JDarray[iJD1x-1].E;
		Ept1x.y = pGtzxTmp->JDarray[iJD1x-1].N;
		Line1x.set(Spt1x,Ept1x);
		IsParallel = Line1x.isParallelTo(Line2x,Tol);
		if (IsParallel)
		{
			RXSJDIndex1x = iJD1x;
			break;
		}
	}

	Spt2x.x = JD2[endJDXH].y;
	Spt2x.y = JD2[endJDXH].x;
	Ept2x.x = JD2[endJDXH+1].y;
	Ept2x.y = JD2[endJDXH+1].x;
	Line2x.set(Spt2x,Ept2x);
	IsParallel = false;
	for (int iJD1x=RXEJDIndex1x; iJD1x<pGtzxTmp->NJD-1; iJD1x++)
	{
		Spt1x.x = pGtzxTmp->JDarray[iJD1x].E;
		Spt1x.y = pGtzxTmp->JDarray[iJD1x].N;
		Ept1x.x = pGtzxTmp->JDarray[iJD1x+1].E;
		Ept1x.y = pGtzxTmp->JDarray[iJD1x+1].N;
		Line1x.set(Spt1x,Ept1x);
		IsParallel = Line1x.isParallelTo(Line2x,Tol);
		if (IsParallel)
		{
			RXEJDIndex1x = iJD1x;
			break;
		}
	}

	sTYLC1x0 = min(sTYLC1x0,pGtzxTmp->QXB[RXSJDIndex1x].KZH);
	eTYLC1x0 = max(eTYLC1x0,pGtzxTmp->QXB[RXEJDIndex1x].KHZ);
	//////////////////////////////////////////////////////////////////////////
	//1.�ж��Ƿ���ӳ���������
	//BOOL SPIsOnXL = JudgeCurJDIsOnXL( firstJDXH -1 , TRUE ) ;

	//if(!SPIsOnXL && firstJDXH>0 && (JD2[firstJDXH].JDXZ==R_XStart || JD2[firstJDXH].JDXZ==R_X || JD2[firstJDXH].JDXZ==R_X_OneR) ) //����������
	//{
	//	//�õ�2��firstJDXH-1
	//	int Cur1xJDnum = JD2[firstJDXH-1].jdno1 + 1 ;

	//	stylc1x0 = min(stylc1x0,pGtzxTmp->QXB[Cur1xJDnum].KZH) ;
	//}

	//BOOL EPIsOnXL = JudgeCurJDIsOnXL( endJDXH + 1 , FALSE ) ;
	//if(!EPIsOnXL && endJDXH < NJD-1 && (JD2[endJDXH].JDXZ==R_XEnd || JD2[endJDXH].JDXZ==R_X ||  JD2[endJDXH].JDXZ==R_X_OneR) ) //�����յ����
	//{
	//	//�õ�2��firstJDXH-1
	//	int Cur1xJDnum = JD2[endJDXH+1].jdno1 - 1 ;

	//	etylc1x0 = max(etylc1x0,pGtzxTmp->QXB[Cur1xJDnum].KHZ) ;
	//}
	//////////////////////////////////////////////////////////////////////////


	//�״�ͶӰ��һ�ߵ�ͳһ���,����������������������ֱ����
	sTYLC1x = pGtzxTmp->LocateHundredMeterOnLine(sTYLC1x0,true);//��ǰ����//һ�������
	eTYLC1x = pGtzxTmp->LocateHundredMeterOnLine(eTYLC1x0,false);//�������

	double xjj,resPt[2];  // projml,

	//����ͶӰ��������//plh05.06
	CalXjj(sTYLC1x,pGtzxTmp->array,pGtzxTmp->tarray,array,tarray,xjj,stylc2x,resPt);//plh05.06
	CalXjj(eTYLC1x,pGtzxTmp->array,pGtzxTmp->tarray,array,tarray,xjj,etylc2x,resPt);//plh05.06

	ACHAR BGH1[20],EGH2[20];
	double BLC1,ELC2;

	AcGePoint2d SptTmp,EptTmp;
	xlpoint PZ;


	//һ�����ֳ����
	BLC1=pGtzxTmp->XLC1(pGtzxTmp->CalnewcmlTocml(sTYLC1x),BGH1,pGtzxTmp->NDL,pGtzxTmp->DLB);
	ELC2=pGtzxTmp->XLC1(pGtzxTmp->CalnewcmlTocml(eTYLC1x),EGH2,pGtzxTmp->NDL,pGtzxTmp->DLB);

	PZ.lc = sTYLC1x ;
	pGtzxTmp->xlpoint_pz(pGtzxTmp->array,pGtzxTmp->tarray,&PZ);

	SptTmp.x = PZ.y ;
	SptTmp.y = PZ.x ;

	PZ.lc = eTYLC1x ;
	pGtzxTmp->xlpoint_pz(pGtzxTmp->array,pGtzxTmp->tarray,&PZ);

	EptTmp.x = PZ.y ;
	EptTmp.y = PZ.x ;

	if (pGtzxTmp->XLShuXing == "��ͨ��ƻ���")
	{
		ACHAR XLCGTX[80];
		_tcscpy(XLCGTX ,pGtzxTmp->GetGTXXclc(SptTmp.y,SptTmp.x));
		split_ckml(XLCGTX,BLC1,BGH1);
		_tcscpy(XLCGTX ,pGtzxTmp->GetGTXXclc(EptTmp.y,EptTmp.x));
		split_ckml(XLCGTX,ELC2,EGH2);
	}


	EXDLB *ExDlb = new EXDLB[2];

	CArray<BAS_DRAW_FUN_JYX::BZLCStructTable,BAS_DRAW_FUN_JYX::BZLCStructTable> RXDBZLCArray ;
	RXDBZLCArray.SetSize(2);

	ExDlb[0].BGH = BGH1;//���ɶ�����,����ǰ�Ϻ������ͬ
	ExDlb[0].no = p_iRXD;

	ExDlb[0].ELC = ExDlb[0].BLC = BLC1;
	ExDlb[0].TYLC = stylc2x;


	if(EGH == NULL)//plh04.11.25
	{
		//if(xjj>0)ExDlb[0].EGH.Format(L"Y%dK",iRXD+1);
		//else ExDlb[0].EGH.Format(L"Z%dK",iRXD+1);
		if(_tcscmp(m_GuiFanCS.SheJiJieDuan,L"����")==0) 
			ExDlb[0].EGH.Format(L"%s",L"DK");
		else 
			ExDlb[0].EGH.Format(L"%s",L"CK");
	}
	else
	{
		ExDlb[0].EGH.Format(L"%s",EGH);
	}



	RXDBZLCArray[0].X = SptTmp.y ;
	RXDBZLCArray[0].Y = SptTmp.x ;

	ACHAR GHTmp[8] ;
	_tcscpy(GHTmp ,ExDlb[0].EGH);
	_tcscpy(RXDBZLCArray[0].kml ,LCchr(GHTmp,ExDlb[0].ELC ,3,1));

	CString StrXLName ;
	StrXLName = xlname ;

	if(	StrXLName.Find(L"��ƻ���") >= 0 )
		_stprintf(RXDBZLCArray[0].GJDNum ,L"���иĽ�%0.1lf",ExDlb[0].ELC / 1000.0);
	else
		_stprintf(RXDBZLCArray[0].GJDNum ,L"���ж�%0.1lf",ExDlb[0].ELC / 1000.0);

	ExDlb[0].RXDName = RXDBZLCArray[0].GJDNum ;

	ExDlb[1].no = p_iRXD;
	ExDlb[1].BGH = ExDlb[0].EGH;
	ExDlb[1].BLC = ExDlb[0].ELC+(etylc2x-stylc2x);//�յ��ǰ���Ϊ���Ϻ���̣��öγ���

	ExDlb[1].TYLC = etylc2x;	
	ExDlb[1].EGH = EGH2;
	ExDlb[1].ELC = ELC2;

	RXDBZLCArray[1].X = EptTmp.y;
	RXDBZLCArray[1].Y = EptTmp.x;

	_tcscpy(GHTmp,ExDlb[1].BGH );

	_tcscpy(RXDBZLCArray[1].kml ,LCchr(GHTmp,ExDlb[1].BLC ,3,1)); 
	_stprintf(RXDBZLCArray[1].GJDNum,RXDBZLCArray[0].GJDNum);

	ExDlb[1].RXDName = ExDlb[0].RXDName;

	//ɾ��ĳ�����жζ�����
	if(IsUpdate)
		DeleteiRXDDLB(p_iRXD);

	Insert2XDLB(p_iRXD,ExDlb,2);

	Insert2xBZLC(p_iRXD, RXDBZLCArray);
	//�����ж����յ������еĽ������ʸ�Ϊ����
	//1��S�����ս��㣬���в�ƽ�н��㡡�������ʸ�Ϊ����
	//2�����н��㣬�������ʸ�Ϊ���У��߼��Ϊǰһ������߼��(�Ӻ���ǰ��)
	//	ModifyJDXZ(iRXD);
	/*
	for(iRXD=0;iRXD<2;iRXD++)
	acutPrintf(L"start->no=%d,BGH=%s,BLC=%lf,EGH=%s,ELC=%lf\n"
	,ExDlb[iRXD].no,ExDlb[iRXD].BGH,ExDlb[iRXD].BLC,ExDlb[iRXD].EGH,ExDlb[iRXD].ELC);	
	*/
	if(ExDlb)
	{
		delete []ExDlb;
		ExDlb=NULL;
	}
}



void GTZX2_JYX_ROAD::Generate2XDLB(int p_iRXD,CArray<EXDLB,EXDLB> &p_arrDLBNew)
{
	int firstJDXH,endJDXH;//��β�������
	//��ȡָ�����ж���ʼ�������
	GetJDXHofiRXD(p_iRXD,firstJDXH,endJDXH);

	if(firstJDXH == -1 || endJDXH == -1 )
		return ;
	//ָ�����жε�һ��ֱ���㣬���һ����ֱ���ͳһ���
	double StartZhTylc,EndHzTylc;


	//-------------------------------------------------
	//1.��ȡָ��II�����жε�һ��ֱ���㣬���һ����ֱ�������
	double x1,y1,x2,y2;
	xlpoint sPt,ePt; // ,temtPt;

	StartZhTylc = QXB[firstJDXH].KZH;
	EndHzTylc   = QXB[endJDXH].KHZ;

	sPt.lc=StartZhTylc;
	xlpoint_pz(array,tarray,&sPt);
	x1=sPt.x;
	y1=sPt.y;

	ePt.lc=EndHzTylc;
	xlpoint_pz(array,tarray,&ePt);
	x2=ePt.x;
	y2=ePt.y;
	//-------------------------------------------------



	GTZX_JYX_ROAD *pGtzxTmp = NULL;

	if(m_pm2 == NULL)
		pGtzxTmp = m_pm1;
	else 
		pGtzxTmp = m_pm2->m_pm1;

	//ָ�����жε�һ��ֱ�����,���һ����ֱ�����һ���ϵ�ͶӰͳһ��̣�
	double sTYLC1x0,eTYLC1x0;
	//��������һ���ϵ�ͶӰͳһ��̣�
	double sTYLC1x,eTYLC1x;
	//�������յ��ͳһ��̣������ϡ�
	double stylc2x,etylc2x;

	//�״�ͶӰ��һ�ߵ�ͳһ���
	sTYLC1x0 = PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,x1,y1,x1,y1);
	eTYLC1x0 = PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,x2,y2,x2,y2);

	//////////////////////////////////////////////////////////////////////////

	int RXSJDIndex1x,RXEJDIndex1x;//���ж����յ��Ӧ��һ�ߵĽ����
	RXSJDIndex1x = pGtzxTmp->DistToNearJD(sTYLC1x0,false);
	RXEJDIndex1x = pGtzxTmp->DistToNearJD(eTYLC1x0,true);

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
		Spt1x.x = pGtzxTmp->JDarray[iJD1x].E;
		Spt1x.y = pGtzxTmp->JDarray[iJD1x].N;
		Ept1x.x = pGtzxTmp->JDarray[iJD1x-1].E;
		Ept1x.y = pGtzxTmp->JDarray[iJD1x-1].N;
		Line1x.set(Spt1x,Ept1x);
		IsParallel = Line1x.isParallelTo(Line2x,Tol);
		if (IsParallel)
		{
			RXSJDIndex1x = iJD1x;
			break;
		}
	}

	Spt2x.x = JD2[endJDXH].y;
	Spt2x.y = JD2[endJDXH].x;
	Ept2x.x = JD2[endJDXH+1].y;
	Ept2x.y = JD2[endJDXH+1].x;
	Line2x.set(Spt2x,Ept2x);
	IsParallel = false;
	for (int iJD1x=RXEJDIndex1x; iJD1x<pGtzxTmp->NJD-1; iJD1x++)
	{
		Spt1x.x = pGtzxTmp->JDarray[iJD1x].E;
		Spt1x.y = pGtzxTmp->JDarray[iJD1x].N;
		Ept1x.x = pGtzxTmp->JDarray[iJD1x+1].E;
		Ept1x.y = pGtzxTmp->JDarray[iJD1x+1].N;
		Line1x.set(Spt1x,Ept1x);
		IsParallel = Line1x.isParallelTo(Line2x,Tol);
		if (IsParallel)
		{
			RXEJDIndex1x = iJD1x;
			break;
		}
	}

	sTYLC1x0 = min(sTYLC1x0,pGtzxTmp->QXB[RXSJDIndex1x].KZH);
	eTYLC1x0 = max(eTYLC1x0,pGtzxTmp->QXB[RXEJDIndex1x].KHZ);


	//�״�ͶӰ��һ�ߵ�ͳһ���,����������������������ֱ����
	sTYLC1x = pGtzxTmp->LocateHundredMeterOnLine(sTYLC1x0,true);//��ǰ����//һ�������
	eTYLC1x = pGtzxTmp->LocateHundredMeterOnLine(eTYLC1x0,false);//�������

	double xjj,resPt[2];  // projml,

	//����ͶӰ��������//plh05.06
	CalXjj(sTYLC1x,pGtzxTmp->array,pGtzxTmp->tarray,array,tarray,xjj,stylc2x,resPt);//plh05.06
	CalXjj(eTYLC1x,pGtzxTmp->array,pGtzxTmp->tarray,array,tarray,xjj,etylc2x,resPt);//plh05.06

	ACHAR BGH1[20],EGH2[20];
	double BLC1,ELC2;

	AcGePoint2d SptTmp,EptTmp;
	xlpoint PZ;


	//һ�����ֳ����
	BLC1=pGtzxTmp->XLC1(pGtzxTmp->CalnewcmlTocml(sTYLC1x),BGH1,pGtzxTmp->NDL,pGtzxTmp->DLB);
	ELC2=pGtzxTmp->XLC1(pGtzxTmp->CalnewcmlTocml(eTYLC1x),EGH2,pGtzxTmp->NDL,pGtzxTmp->DLB);

	PZ.lc = sTYLC1x ;
	pGtzxTmp->xlpoint_pz(pGtzxTmp->array,pGtzxTmp->tarray,&PZ);

	SptTmp.x = PZ.y ;
	SptTmp.y = PZ.x ;

	PZ.lc = eTYLC1x ;
	pGtzxTmp->xlpoint_pz(pGtzxTmp->array,pGtzxTmp->tarray,&PZ);

	EptTmp.x = PZ.y ;
	EptTmp.y = PZ.x ;

	if (pGtzxTmp->XLShuXing == "��ͨ��ƻ���")
	{
		ACHAR XLCGTX[80];
		_tcscpy(XLCGTX ,pGtzxTmp->GetGTXXclc(SptTmp.y,SptTmp.x));
		split_ckml(XLCGTX,BLC1,BGH1);
		_tcscpy(XLCGTX ,pGtzxTmp->GetGTXXclc(EptTmp.y,EptTmp.x));
		split_ckml(XLCGTX,ELC2,EGH2);
	}


	EXDLB *ExDlb = new EXDLB[2];

	CArray<BAS_DRAW_FUN_JYX::BZLCStructTable,BAS_DRAW_FUN_JYX::BZLCStructTable> RXDBZLCArray ;
	RXDBZLCArray.SetSize(2);

	ExDlb[0].BGH = BGH1;//���ɶ�����,����ǰ�Ϻ������ͬ
	ExDlb[0].no = p_iRXD;

	ExDlb[0].ELC = ExDlb[0].BLC = BLC1;
	ExDlb[0].TYLC = stylc2x;


	//if(EGH == NULL)//plh04.11.25
	//{
	if(_tcscmp(m_GuiFanCS.SheJiJieDuan,L"����")==0) 
		ExDlb[0].EGH.Format(L"%s",L"DK");
	else 
		ExDlb[0].EGH.Format(L"%s",L"CK");
	//}
	//else
	//{
	//	ExDlb[0].EGH.Format(L"%s",EGH);
	//}



	RXDBZLCArray[0].X = SptTmp.y ;
	RXDBZLCArray[0].Y = SptTmp.x ;

	ACHAR GHTmp[8] ;
	_tcscpy(GHTmp ,ExDlb[0].EGH);
	_tcscpy(RXDBZLCArray[0].kml ,LCchr(GHTmp,ExDlb[0].ELC ,3,1));

	CString StrXLName ;
	StrXLName = xlname ;

	if(	StrXLName.Find(L"��ƻ���") >= 0 )
		_stprintf(RXDBZLCArray[0].GJDNum ,L"���иĽ�%0.1lf",ExDlb[0].ELC / 1000.0);
	else
		_stprintf(RXDBZLCArray[0].GJDNum ,L"���ж�%0.1lf",ExDlb[0].ELC / 1000.0);

	ExDlb[0].RXDName = RXDBZLCArray[0].GJDNum ;

	ExDlb[1].no = p_iRXD;
	ExDlb[1].BGH = ExDlb[0].EGH;
	ExDlb[1].BLC = ExDlb[0].ELC+(etylc2x-stylc2x);//�յ��ǰ���Ϊ���Ϻ���̣��öγ���

	ExDlb[1].TYLC = etylc2x;	
	ExDlb[1].EGH = EGH2;
	ExDlb[1].ELC = ELC2;

	RXDBZLCArray[1].X = EptTmp.y;
	RXDBZLCArray[1].Y = EptTmp.x;

	_tcscpy(GHTmp,ExDlb[1].BGH );

	_tcscpy(RXDBZLCArray[1].kml ,LCchr(GHTmp,ExDlb[1].BLC ,3,1)); 
	_stprintf(RXDBZLCArray[1].GJDNum,RXDBZLCArray[0].GJDNum);

	ExDlb[1].RXDName = ExDlb[0].RXDName;

	ExDlb[0].no = p_iRXD;
	ExDlb[1].no = p_iRXD;

	p_arrDLBNew.Add(ExDlb[0]);
	p_arrDLBNew.Add(ExDlb[1]);

	if(ExDlb)
	{
		delete []ExDlb;
		ExDlb=NULL;
	}



}


void GTZX2_JYX_ROAD::addBZLCToGTZX2(ads_point PT ,ACHAR CurAddBZLC[30],CString iRxdXLName )
{
	assertWriteEnabled();

	NBZLC = BZLCarray.GetSize();

	BZLCStructTable BZLCTemp;
	_tcscpy( BZLCTemp.GJDNum , iRxdXLName ) ;
	BZLCTemp.X = PT[Y];
	BZLCTemp.Y = PT[X];
	_tcscpy(BZLCTemp.kml , CurAddBZLC);

	int CurRXDNum ;
	CurRXDNum = GetiRXDFromNE( PT[Y],PT[X] );

	if( CurRXDNum < 0 )return ;

	// GTZX *pGtzxTmp = GetiRxdtoGTZXplinp( CurRXDNum );

	BZLCarray.Add(BZLCTemp);

	/*
	int CurSBZLCNum , CurEBZLCNum ;

	GetiRXDBZLCinfo( CurRXDNum , CurSBZLCNum , CurEBZLCNum );


	double CmlTmp = 0.0 , CurPTcml = 0.0 ;
	int FindiBZLCNum = 0 ;
	CurPTcml = pGtzxTmp->PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,PT[Y],PT[X],PT[Y],PT[X]);

	for (int i = CurSBZLCNum ;i <= CurEBZLCNum ; i++ )
	{
	CmlTmp = pGtzxTmp->PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,BZLCarray[i].X,BZLCarray[i].Y,BZLCarray[i].X,BZLCarray[i].Y);

	if( CmlTmp > CurPTcml)
	{
	FindiBZLCNum = i  ;
	break ;
	}
	}

	BZLCarray.InsertAt( FindiBZLCNum , BZLCTemp );
	*/


	NBZLC = BZLCarray.GetSize();

}



void GTZX2_JYX_ROAD::SetDefaultiRxdBZLC(int iRXD , CArray<BAS_DRAW_FUN_JYX::BZLCStructTable ,BAS_DRAW_FUN_JYX::BZLCStructTable> & BZLCarrayToOut )
{
	BZLCarrayToOut.RemoveAll();

	BAS_DRAW_FUN_JYX::BZLCStructTable OneBZLCTmp ;

	double jday[MAXJDNUM][6];
	int njd;
	int SDLNum, EDLNum;
	//��ȡjdarray
	GetiRXDJDArray(iRXD,jday,njd);
	GetiRXDDLinfo(iRXD,SDLNum,EDLNum);

	CString iRxdNameTmp = GetRXDXLNameByRXDIndex(iRXD);
	//���
	_tcscpy( OneBZLCTmp.GJDNum ,  iRxdNameTmp ) ;
	OneBZLCTmp.X = jday[0][1] ;
	OneBZLCTmp.Y = jday[0][2] ;
	_tcscpy( OneBZLCTmp.kml ,LCchr(DLB[SDLNum].EGH,DLB[SDLNum].BLC + DLB[SDLNum].DL , 4, 1) );

	BZLCarrayToOut.Add(OneBZLCTmp);

	//β
	_tcscpy( OneBZLCTmp.GJDNum , iRxdNameTmp );
	OneBZLCTmp.X = jday[njd-1][1] ;
	OneBZLCTmp.Y = jday[njd-1][2] ;
	_tcscpy( OneBZLCTmp.kml ,LCchr(DLB[EDLNum].BGH,DLB[EDLNum].BLC ,4, 1) );

	BZLCarrayToOut.Add(OneBZLCTmp);

}


void GTZX2_JYX_ROAD::ModifyJDXZ(int iRXD)
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
	for( i=firstJDXH;i>=0;i--)
	{
		if(QXB[i].KZH>stylc2x)
		{
			//�ҵ�һ������λ�ڸ����ж���

			if(JD2[i].JDXZ==B_X )//���н���
			{
				JD2[i].JDXZ=R_X; //?????*****//
				if(i>0)JD2[i].xjj=JD2[i-1].xjj;
			}
			else if(JD2[i].JDXZ==R_X_OneR)
			{
				JD2[i].JDXZ=R_X_OneR; //?????*****//
			}
			else//S�佻�㣬���в�ƽ�н���
			{
				JD2[i].JDXZ=R_X; //?????*****//
			}
		}
	}
	//��������λ�ڸö��ڵķ����н���
	for(i=endJDXH;i<NJD;i++)
	{
		if(QXB[i].KZH<etylc2x)
		{
			//�ҵ�һ������λ�ڸ����ж���
			if(JD2[i].JDXZ==B_X  )//���н���
			{
				JD2[i].JDXZ=R_X;  //?????*****//
				if(i<NJD-1)JD2[i].jdno1+=1;
			}
			else if(JD2[i].JDXZ==R_X_OneR) //S�佻�㣬���в�ƽ�н���
			{
				JD2[i].JDXZ=R_X_OneR;   //?????*****//
			}
			else //S�佻�㣬���в�ƽ�н���
			{
				JD2[i].JDXZ=R_X;   //?????*****//
			}
		}
	}
}
//�õ�ָ�����жε����ս��㽻���??????????
void GTZX2_JYX_ROAD::GetJDXHofiRXD(int iRXD, int &firstJDXH, int &endJDXH)
{
	firstJDXH = -1;
	endJDXH = -1;
	int RXDnum=0;//���ж���Ŀ
	for(int i=0;i<NJD;i++)
	{
		if(JD2[i].JDXZ==R_XStart)//����ʼ
		{
			firstJDXH=i;
			if (i<NJD-1)
				i++;
			while(i<NJD && (JD2[i].JDXZ==R_X || JD2[i].JDXZ==R_XEnd ))
			{
				endJDXH=i;
				i++;
			}
			i--;
			RXDnum++;
			if(iRXD==RXDnum-1)return;
		}
		if( JD2[i].JDXZ==R_X_OneR)//���е�
		{
			firstJDXH = i;
			endJDXH = i; 
			RXDnum++;
			if(iRXD==RXDnum-1)return;
		}
	}				
}

int GTZX2_JYX_ROAD::GetRXDNum()//�����ж���Ŀ??????????
{
	int RXDnum=0;//���ж���Ŀ
	for(int i=0;i<NJD;i++)
	{

		if(JD2[i].JDXZ == R_XStart)
		{
			if (i<NJD-1)
				i++;
			while( JD2[i].JDXZ==R_X)i++;
			if (JD2[i].JDXZ==R_XEnd)
			{
				RXDnum++;
			}
			else
			{
				ads_printf(L"777  i=%d,JDXZ==%d\n",i,JD2[i].JDXZ);
				CString Mes;
				Mes.Format(L"�������С�����Ҳ���������ֹ���㣡��˲�����");
				AfxMessageBox(Mes);
				return -2;
			}
		}

		if( JD2[i].JDXZ==R_X_OneR )RXDnum++;
	}
	return RXDnum;
}

void GTZX2_JYX_ROAD::DeleteiRXDBZLC(int iRXD)
{
	int SBZLCNumTmp , EBZLCNumTmp ;


	GetiRXDBZLCinfo( iRXD,SBZLCNumTmp,EBZLCNumTmp);

	GTZX_JYX_ROAD * pGtzxLinpTmp = GetiRxdtoGTZXplinp( iRXD );


	if(SBZLCNumTmp>=0 && EBZLCNumTmp>SBZLCNumTmp )BZLCarray.RemoveAt(SBZLCNumTmp , EBZLCNumTmp - SBZLCNumTmp + 1 );

	if(SBZLCNumTmp==-1 && EBZLCNumTmp==-1 )
	{
		double cmlTmp = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,pGtzxLinpTmp->JDarray[1].N,pGtzxLinpTmp->JDarray[1].E,pGtzxLinpTmp->JDarray[1].N,pGtzxLinpTmp->JDarray[1].E);

		double cmlTmp1 = cmlTmp ;
		double cmlTmp2 = cmlTmp ;
		double NTmp ,ETmp ;

		for(int i= 0 ;i< BZLCarray.GetSize()-1;i++)
		{
			NTmp = BZLCarray[i].X ;
			ETmp = BZLCarray[i].Y ;

			if( GetiRXDFromNE(NTmp, ETmp) == -1 )
			{
				cmlTmp1 = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,NTmp,ETmp,NTmp,ETmp);
			}

			NTmp = BZLCarray[i+1].X ;
			ETmp = BZLCarray[i+1].Y ;

			if( GetiRXDFromNE(NTmp, ETmp) == -1 )
			{
				cmlTmp2 = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,NTmp,ETmp,NTmp,ETmp);
			}

			if( cmlTmp >cmlTmp1-0.01 &&   cmlTmp < cmlTmp2 + 0.01 )
			{
				SBZLCNumTmp = i;
				break ;
			}

		}

		if(SBZLCNumTmp>=0)
		{
			BZLCarray.RemoveAt(SBZLCNumTmp , 2 );
		}
	}

	if(pGtzxLinpTmp)
	{
		delete pGtzxLinpTmp;
		pGtzxLinpTmp = NULL ;
	}

}


void GTZX2_JYX_ROAD::DeleteiRXDDLB(int iRXD)//ɾ��ĳ�����жζ�����
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

void GTZX2_JYX_ROAD::GetiRXDDLinfo(int iRXD, int &sDLno, int &eDLno)
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

void GTZX2_JYX_ROAD::GetiRXDBZLCinfo(int iRXD, int &SBLZCNum, int &EBLZCNum)
{
	SBLZCNum=-1;
	EBLZCNum=-1;

	GTZX_JYX_ROAD * pGtzxTmp = NULL ;

	pGtzxTmp = GetiRxdtoGTZXplinp( iRXD );

	double cmlTmp = 0.0 ;
	int CountNumTmp = 0 ;
	for(int i=0;i<BZLCarray.GetSize();i++)
	{
		cmlTmp = pGtzxTmp->PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,BZLCarray[i].Y,
			BZLCarray[i].X,BZLCarray[i].Y,BZLCarray[i].X);
		cmlTmp = pGtzxTmp->CalcmlTonewcml( cmlTmp );

		if( cmlTmp >-0.001 )
		{
			if( CountNumTmp == 0 )SBLZCNum = CountNumTmp ;
			CountNumTmp ++ ;
			EBLZCNum = i ;
		}
	}

	if (pGtzxTmp)
	{
		delete pGtzxTmp;
		pGtzxTmp = NULL;
	}

}

//II�߸�ʽת��I�߸�ʽʵ��(����ƻ���ת��ͨ��ƻ���)
GTZX_JYX_ROAD *GTZX2_JYX_ROAD::CreateGtzx()
{
	//��������
	GTZX_JYX_ROAD * pGtzx = NULL ;
	CString GHJDTemp ;

	int iCount = JDarray.GetSize();
	for(int i=0;i < iCount; i++)
	{	
		GHJDTemp = JDarray[i].JDNum ;
		GHJDTemp.Trim();


		//Zhujiang ���ɹ�ͨ��ƻ���ʱ��������ƻ�����ͬ�Ľ����  20090306
		//GHJDTemp = m_pm1->JDGH + GHJDTemp ;

		_tcscpy(JDarray[i].JDNum ,L"\0");
		_tcscpy(JDarray[i].JDNum ,GHJDTemp);
	}

	pGtzx = new GTZX_JYX_ROAD(JDarray.GetSize(),JDarray,L"AK",&m_pm1->JDGH[0]);

	pGtzx->assertReadEnabled();


	pGtzx->XLShuXing = xlname ;
	pGtzx->mode_display=0;

	_tcscpy(pGtzx->JDGH,JDGH);

	pGtzx->NDL = CreateGtzxDLB();

	iCount = pGtzx->NDL;
	for(int i=0;i < iCount;i++)
	{
		pGtzx->setDLM(i,DLB[i].BLC,DLB[i].DL,DLB[i].BGH,DLB[i].EGH);

		pGtzx->DLB[i].N = DLB[i].N ;
		pGtzx->DLB[i].E = DLB[i].E ;
	}

	iCount = pGtzx->NJD;
	for(int i=0;i< iCount;i++)
	{
		if(JD2[i].JDXZ==6)
			pGtzx->JDarray[i].isGJ = TRUE ; //���������иĽ�
		else pGtzx->JDarray[i].isGJ = FALSE ; //�����㲢�иĽ�
	}

	return pGtzx;
}

int  GTZX2_JYX_ROAD::CreateGtzxDLB()
{
	int i,j,njd;
	double sml,eml;
	CArray<int,int&>DLno;

	int dlnum = 0;

	if (m_pm1->XLShuXing!="��ͨ��ƻ���")
	{
		xlpoint PZforDL;
		//�������������
		for( i=0;i<m_pm1->NDL;i++)
		{

			PZforDL.lc = m_pm1->TYLC(m_pm1->DLB[i].BLC);

			m_pm1->xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZforDL);

			m_pm1->DLB[i].N = PZforDL.x ;

			m_pm1->DLB[i].E = PZforDL.y ;
		}
	}

	sml = m_pm1->DLB[0].BLC+m_pm1->DLB[0].DL;
	int RxdNum=GetRXDNum();
	double jday[MAXJDNUM][6];

	xlpoint PZ;
	ACHAR ckml[20];


	for(i=0;i< RxdNum;i++)
	{	   	  
		GetiRXDJDArray(i,jday,njd);
		eml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,jday[0][1],jday[0][2],jday[0][1],jday[0][2]);
		m_pm1->GetDLBno(sml,eml,DLno);

		for(j=0;j<DLno.GetSize();j++)
		{
			DLB[dlnum]=m_pm1->DLB[DLno[j]];

			_tcscpy(ckml,m_pm1->LCchr(DLB[dlnum].BGH,DLB[dlnum].BLC,3,1));
			PZ.lc = m_pm1->TYLC1(ckml , m_pm1->NDL,m_pm1->DLB);
			m_pm1->xlpoint_pz(m_pm1->array,m_pm1->tarray,& PZ);

			DLB[dlnum].N = PZ.x ;
			DLB[dlnum].E = PZ.y ;

			dlnum++;
		} 
		DLno.RemoveAll();
		int sdlno,edlno;
		GetiRXDDLinfo(i,sdlno,edlno);
		GTZX_JYX_ROAD *rxdpm = NULL;

		rxdpm = GetiRxdtoGTZXplinp(i);

		for(j=sdlno;j<=edlno;j++)
		{
			//	double BLC,ELC ; // ,TYLC;  //  �����Ķ�ǰ��̡��Ϻ���̡�������ͳһ���
			CString   BGH,EGH; //  �����Ķ�ǰ��̹ںźͶϺ���̹ں�	
			DLB[dlnum].BLC = DLB2x[j].BLC;
			_tcscpy(DLB[dlnum].BGH, DLB2x[j].BGH);
			_tcscpy(DLB[dlnum].EGH, DLB2x[j].EGH);
			DLB[dlnum].DL = DLB2x[j].ELC- DLB2x[j].BLC;

			if(rxdpm)
			{
				_tcscpy(ckml,rxdpm->LCchr(DLB[dlnum].BGH,DLB[dlnum].BLC,3,1));
				PZ.lc = rxdpm->TYLC1(ckml , rxdpm->NDL,rxdpm->DLB);
				rxdpm->xlpoint_pz(rxdpm->array,rxdpm->tarray,& PZ);
				DLB[dlnum].N = PZ.x ;
				DLB[dlnum].E = PZ.y ;
			}
			else
			{

			}

			dlnum++;
		}	  
		sml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,jday[njd-1][1],jday[njd-1][2],jday[njd-1][1],jday[njd-1][2]);
		if (rxdpm)
		{
			delete rxdpm;
			rxdpm = NULL;
		}
	}

	eml = m_pm1->DLB[0].BLC+m_pm1->DLB[0].DL + m_pm1->xl_length;


	m_pm1->GetDLBno(sml,eml,DLno);
	for(j=0;j<DLno.GetSize();j++)
	{
		DLB[dlnum]=m_pm1->DLB[DLno[j]];

		_tcscpy(ckml,m_pm1->LCchr(DLB[dlnum].BGH,DLB[dlnum].BLC,3,1));
		PZ.lc = m_pm1->TYLC1(ckml , m_pm1->NDL,m_pm1->DLB);
		m_pm1->xlpoint_pz(m_pm1->array,m_pm1->tarray,& PZ);

		DLB[dlnum].N = PZ.x ;
		DLB[dlnum].E = PZ.y ;

		dlnum++;
	}
	DLno.RemoveAll();
	return dlnum;
}

double GTZX2_JYX_ROAD::LocateHundredMeterOnLine(double tylc1x,bool IsForwardTo)
	//����ͳһ��̣��Ƿ���ǰѰ�Ҹ����������ױ꣬Ҫ����ֱ����,
{
	double gettylc=tylc1x;
	double lc0;//��׮
	int iXY=0;//����������Ԫ
	int XYClass;//��Ԫ����
	ACHAR GH[20];//��̹ں�
	ACHAR XCLCStr[80];//�ֳ�����ַ���
	double xlc;//һ����ͶӰ�ֳ����
	GTZX_JYX_ROAD *pGtzxLinp = NULL ;
	xlpoint PZ;
	if(m_pm2==NULL)
	{
		pGtzxLinp = m_pm1;
	}
	else pGtzxLinp = m_pm2->m_pm1 ;

	//////////////////////////////////////////////////////////////////////////
	//�Ѿ��ڰ��ױ꣬�����ٶ�λ
	if (pGtzxLinp->XLShuXing=="��ͨ��ƻ���")
	{
		PZ.lc = tylc1x;
		xlpoint_pz(pGtzxLinp->array,pGtzxLinp->tarray,&PZ);
		_tcscpy(XCLCStr , pGtzxLinp->GetGTXXclc(PZ.x,PZ.y));
		split_ckml(XCLCStr,xlc,GH);
	}
	else
	{
		xlc = XLC1(pGtzxLinp->CalnewcmlTocml(tylc1x),GH,pGtzxLinp->NDL,pGtzxLinp->DLB);
	}

	if (fmod(xlc,100.0)<0.001)
		return tylc1x;

	for(int i=pGtzxLinp->tarray;i>0;i--)
	{
		if(tylc1x>=pGtzxLinp->array[i][6])
		{
			iXY=i;
			break;
		}
	}

	XYClass=pGtzxLinp->array[iXY][0];//ȡ����Ԫ����
	if(IsForwardTo)//�����ǰ����
	{
		if(XYClass==4)//�󻺣���ǰ�ƶ�һ����Ԫ
		{
			iXY--;
			XYClass=pGtzxLinp->array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==2)//Բ����ǰ�ƶ�һ����Ԫ
		{
			iXY--;
			XYClass=pGtzxLinp->array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==3)//ǰ������ǰ�ƶ�һ����Ԫ
		{
			iXY--;
		}
		//��ʱ��Ԫ��ֱ��
		gettylc=pGtzxLinp->array[iXY+1][6];//һ��ֱ�������
		if(tylc1x<gettylc)gettylc=tylc1x;//�״ζ�λ��ֱ���ϣ�

		double K = 1.0 ;
		if(pGtzxLinp->XLShuXing!="��ͨ��ƻ���")
		{		
			gettylc = pGtzxLinp->CalnewcmlTocml(gettylc ,K );
			xlc=XLC1(gettylc,GH,pGtzxLinp->NDL,pGtzxLinp->DLB); //ֱ�����ֳ����			
		}
		else
		{
			PZ.lc = gettylc;
			xlpoint_pz(pGtzxLinp->array,pGtzxLinp->tarray,&PZ);
			_tcscpy(XCLCStr , pGtzxLinp->GetGTXXclc(PZ.x,PZ.y));
			split_ckml(XCLCStr,xlc,GH);
		}

		double newxclc = xlc ;
		newxclc = 100*(int)(xlc/100+0.00001); //ZHǰһ���ױ��ֳ����

		if (newxclc<pGtzxLinp->DLB[0].BLC+pGtzxLinp->DLB[0].DL)//�����λ�İ��ױ�����·��ʼ���֮ǰ������λ���ױ�
			return tylc1x;

		double cml = 0.0;

		if (pGtzxLinp->XLShuXing=="��ͨ��ƻ���")
		{
			ACHAR XLCGTX[80];//��ͨ���ֳ����
			_tcscpy(XLCGTX , LCchr(GH,newxclc,3,1));
			pGtzxLinp->GetPZFromXCLC1X(XLCGTX,PZ);
			cml = PROJ_ML(pGtzxLinp->array,pGtzxLinp->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		}
		else
		{
			_tcscpy(XCLCStr,LCchr(GH,newxclc,3,1));
			cml = pGtzxLinp->TYLC1(XCLCStr,pGtzxLinp->NDL,pGtzxLinp->DLB);
			//cml = pGtzxLinp->TYLC1(newxclc,pGtzxLinp->NDL,pGtzxLinp->DLB);
			cml = pGtzxLinp->CalcmlTonewcml(cml);
		}
		lc0 = fabs( cml - gettylc ) ;

		if(lc0<pGtzxLinp->array[iXY+1][6]-pGtzxLinp->array[iXY][6] && pGtzxLinp->DistToNearDL(CalnewcmlTocml(gettylc),true)>lc0)//�������Ը���Ԫ,�ҵ���ǰ��λ��֮��û�ж���
		{
			return cml ;//����һ�߰��ױ���������(���Ǳ�־���)
		}
		else//ȡ��ʮ��
		{
			//���¼���,
			newxclc = 10*(int)(xlc/10+0.00001); //��ZHǰʮ�ױ��ֳ����
			cml = 0.0;
			if (pGtzxLinp->XLShuXing=="��ͨ��ƻ���")
			{
				ACHAR XLCGTX[80];//��ͨ���ֳ����
				_tcscpy(XLCGTX , LCchr(GH,newxclc,3,1));
				pGtzxLinp->GetPZFromXCLC1X(XLCGTX,PZ);
				cml = PROJ_ML(pGtzxLinp->array,pGtzxLinp->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
			}
			else
			{
				_tcscpy(XCLCStr,LCchr(GH,newxclc,3,1));
				cml = pGtzxLinp->TYLC1(XCLCStr,pGtzxLinp->NDL,pGtzxLinp->DLB);
				//cml = pGtzxLinp->TYLC1(newxclc,pGtzxLinp->NDL,pGtzxLinp->DLB);
				cml = pGtzxLinp->CalcmlTonewcml(cml);
			}
			return cml ;
		}
	}
	else
	{
		if(XYClass==3)//ǰ��������ƶ�һ����Ԫ
		{
			iXY++;
			XYClass=pGtzxLinp->array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==2)//Բ������ƶ�һ����Ԫ
		{
			iXY++;
			XYClass=pGtzxLinp->array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==4)//ǰ��������ƶ�һ����Ԫ
		{
			iXY++;
		}
		//��ʱ��Ԫ��ֱ��
		gettylc=pGtzxLinp->array[iXY][6];//��ֱ���
		if(tylc1x>gettylc)gettylc=tylc1x;//�״ζ�λ��ֱ���ϣ�

		double K = 1.0 ;
		if(pGtzxLinp->XLShuXing!="��ͨ��ƻ���")
		{
			double Tmptylc = pGtzxLinp->CalnewcmlTocml(gettylc ,K );
			xlc=XLC1(Tmptylc,GH,pGtzxLinp->NDL,pGtzxLinp->DLB);
		}
		else
		{
			PZ.lc = gettylc;
			xlpoint_pz(pGtzxLinp->array,pGtzxLinp->tarray,&PZ);
			_tcscpy(XCLCStr , pGtzxLinp->GetGTXXclc(PZ.x,PZ.y));
			split_ckml(XCLCStr,xlc,GH);
		}

		double newxclc;
		newxclc = 100*((int)(xlc/100+0.00001)+1); //HZ��һ���ױ��ֳ����
		double cml=0.0;
		if (newxclc>pGtzxLinp->DLB[pGtzxLinp->NDL-1].ELC)//�����λ�İ��ױ�����·��ʼ���֮ǰ������λ���ױ�
			return tylc1x;

		if (pGtzxLinp->XLShuXing!="��ͨ��ƻ���")
		{
			_tcscpy(XCLCStr,LCchr(GH,newxclc,3,1));
			cml = pGtzxLinp->TYLC1(XCLCStr,pGtzxLinp->NDL,pGtzxLinp->DLB);
			//cml = TYLC1(newxclc,pGtzxLinp->NDL,pGtzxLinp->DLB);
			cml = pGtzxLinp->CalcmlTonewcml(cml);
		}

		else
		{
			ACHAR XLCGTX[80];//��ͨ���ֳ����
			_tcscpy(XLCGTX , LCchr(GH,newxclc,3,1));
			pGtzxLinp->GetPZFromXCLC1X(XLCGTX,PZ);
			cml = PROJ_ML(pGtzxLinp->array,pGtzxLinp->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		}

		lc0 = fabs( cml - gettylc ) ;

		if(lc0<pGtzxLinp->array[iXY+1][6]-pGtzxLinp->array[iXY][6]&&fabs(pGtzxLinp->DistToNearDL(gettylc,false))>lc0)//�������Ը���Ԫ
		{
			return cml;
		}
		else//ȡ��ʮ��
		{

			newxclc = 10*((int)(xlc/10+0.00001)+1); //��HZ��ʮ�ױ��ֳ����
			if (XLShuXing=="��ͨ��ƻ���")
			{
				ACHAR XLCGTX[80];//��ͨ���ֳ����
				_tcscpy(XLCGTX, LCchr(GH,newxclc,3,1));
				pGtzxLinp->GetPZFromXCLC1X(XLCGTX,PZ);
				cml = PROJ_ML(pGtzxLinp->array,pGtzxLinp->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
			}
			else
			{
				_tcscpy(XCLCStr,LCchr(GH,newxclc,3,1));
				cml = pGtzxLinp->TYLC1(XCLCStr,pGtzxLinp->NDL,pGtzxLinp->DLB);
				//cml = TYLC1(newxclc,pGtzxLinp->NDL,pGtzxLinp->DLB);
				cml = CalcmlTonewcml(cml);
			}

			return cml ;
		}		
	}
	return gettylc;
}

//��ע�߼��
void GTZX2_JYX_ROAD::BZXJJ(AcGiWorldDraw* pWd)
{
	int i;
	AcGePoint3d Pt1,Pt2,Pt0;
	double zhlc,hzlc,lc2x,lc1x,Tjiao;
	xlpoint PZ;
	ACHAR XJJ[20];

	//��עS���߼��
	for(i=0;i<NJD-1;i++)//���һ�����㲻��ע
	{
		if( ((JD2[i].JDXZ==R_X||JD2[i].JDXZ==R_XStart) && (JD2[i+1].JDXZ==R_X||JD2[i+1].JDXZ==R_XEnd)) 
			|| (JD2[i].JDXZ==B_XNOTP_X&&JD2[i+1].JDXZ==B_XNOTP_X)
			|| (JD2[i].JDXZ==S_StartPoint&&JD2[i+1].JDXZ==S_EndPoint) || JD2[i].JDXZ==R_X_OneR )continue;
		hzlc=QXB[i].KHZ;
		zhlc=QXB[i+1].KZH;
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

		if(JD2[i].JDXZ==R_X )_stprintf(XJJ,L"%.2lf",fabs(JD2[i+1].xjj));//plh05.06
		else _stprintf(XJJ,L"%.2lf",fabs(JD2[i].xjj));//plh05.06

		if(_wtof(XJJ)<0.01)continue;

		PZ.lc=lc2x-0.5*texth;
		xlpoint_pz(array,tarray,&PZ);

		Pt0.x=PZ.y+_tcslen(XJJ)*10.0*texth*sin(PZ.a+m_dHalfPI);
		Pt0.y=PZ.x+_tcslen(XJJ)*10.0*texth*cos(PZ.a+m_dHalfPI);
		G_maketext(pWd,Pt0,XJJ,Tjiao,10.0*texth,QXYSColor,1);
		makeXJJFlag(pWd,Pt1,Pt2,80.0*texth);
	}
}

void GTZX2_JYX_ROAD::makeXJJFlag(AcGiWorldDraw* pWd,AcGePoint3d Pt1,AcGePoint3d Pt2,double len)
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


void GTZX2_JYX_ROAD::BZRXD(AcGiWorldDraw* pWd)
{
	int RxdNum=GetRXDNum();//���ж���Ŀ

	for(int i=0;i<RxdNum;i++)
	{
		BZiRXDLC(pWd,i);
	}

}

void GTZX2_JYX_ROAD::BZiRXDLC(AcGiWorldDraw* pWd,int iRXD)//�����ע���ж�
{
	int sjd=0,ejd=0,njd=0,i=0,j=0,sdl=0,edl=0,ndl=0;
	EXDLB *iRxdDlb;
	GetiRXDDLinfo(iRXD,sdl,edl);//��ȡ���ն�����

	if( sdl==-1 || edl==-1 ) return ;

	ndl=edl-sdl+1;
	//	acutPrintf(L"iRXD=%d,sdl=%d,edl=%d,ndl=%d\n",iRXD,sdl,edl,ndl);
	//	AfxMessageBox(L"BZiRXDLC1");
	iRxdDlb=new EXDLB[ndl];

	//	int no;//��ʾ�ڼ������ж�
	//	double BLC,ELC,TYLC;  //  �����Ķ�ǰ��̡��Ϻ���̡�������ͳһ���
	//	ACHAR   BGH[8],EGH[8]; //  �����Ķ�ǰ��̹ںźͶϺ���̹ں�	

	for(i=sdl;i<=edl;i++)
	{
		iRxdDlb[i-sdl]=DLB2x[i];
	}

	//xlpoint PZ1,PZ2;	//�������յ�������	
	//PZ1.lc=iRxdDlb[0].TYLC;
	//PZ2.lc=iRxdDlb[ndl-1].TYLC;
	//xlpoint_pz(array,tarray,&PZ1);//������
	//xlpoint_pz(array,tarray,&PZ2);
	//GetJDXHofiRXD(iRXD,sjd,ejd);//��ȡ���ս����

	//njd=ejd-sjd+1;//������
	//njd+=2;//�����������յ�
	////	double (*jday)[6];//��̬���ٽ�������
	////	jday=new double[njd][6];

	//CArray<JDarrayStruct,JDarrayStruct>JDarrayTemp;
	//JDarrayTemp.SetSize(njd);
	//BAS_DRAW_FUN::JDarrayStruct OneJDTemp;

	//for(i=1;i<njd-1;i++)//1->njd-1
	//{
	//	_tcscpy( JDarrayTemp[i].JDNum , JDarray[sjd+i-1].JDNum ) ;
	//	//	for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i-1][j];//sjd+i-1,��sjd��ʼ
	//	JDarrayTemp[i].N=JDarray[sjd+i-1].N;
	//	JDarrayTemp[i].E=JDarray[sjd+i-1].E;
	//	JDarrayTemp[i].l1=JDarray[sjd+i-1].l1;
	//	JDarrayTemp[i].R=JDarray[sjd+i-1].R;
	//	JDarrayTemp[i].l2=JDarray[sjd+i-1].l2;
	//}
	////���ж������Ϊ��һ������
	//i=0;
	//_tcscpy(JDarrayTemp[i].JDNum , L"QDTemp" );
	//JDarrayTemp[i].N=PZ1.x;
	//JDarrayTemp[i].E=PZ1.y;

	//JDarrayTemp[i].l1=iRxdDlb[i].ELC ;	//iRxdDlb[i].ELC
	//JDarrayTemp[i].R=0.0;
	//JDarrayTemp[i].l2=0.0;
	////���ж��յ���Ϊ���һ������
	//i=njd-1;
	//_tcscpy(JDarrayTemp[i].JDNum , L"ZDTemp" );
	//JDarrayTemp[i].N=PZ2.x;
	//JDarrayTemp[i].E=PZ2.y;
	//JDarrayTemp[i].l1=0.0;
	//JDarrayTemp[i].R=0.0;
	//JDarrayTemp[i].l2=0.0;

	//////��ע���
	//double dlc;
	//dlc=100.0;//����ױ�
	////	double (*qxb)[9];//��̬�������߱�
	////qxb=new double[njd][9];
	//double (*xyarray)[10];//��̬������Ԫ����
	//xyarray=new double[4*njd][10];
	//int tary=0;	

	//ACHAR JDGHTemp[8],GHTemp[8];

	//_tcscpy(JDGHTemp,L"JD");
	//_tcscpy(GHTemp,L"AK"); 

	//GTZX *pzLinepTemp= new GTZX(JDarrayTemp.GetSize(),JDarrayTemp,&GHTemp[0],&JDGHTemp[0]);

	//pzLinepTemp->NDL = ndl ;
	//ACHAR BghTmp[8] , EghTmp[8];
	//for(i=0;i<ndl;i++)
	//{
	//	_tcscpy( BghTmp ,iRxdDlb[i].BGH );
	//	_tcscpy( EghTmp ,iRxdDlb[i].EGH );
	//	pzLinepTemp->setDLM(i,iRxdDlb[i].BLC,iRxdDlb[i].ELC - iRxdDlb[i].BLC ,BghTmp ,EghTmp);
	//}
	//  pzLinepTemp->getpxy_DATA();
	//  pzLinepTemp->DLB[pzLinepTemp->NDL-1].BLC = iRxdDlb[ndl-1].BLC;
	//  pzLinepTemp->DLB[pzLinepTemp->NDL-1].DL = iRxdDlb[ndl-1].ELC - iRxdDlb[ndl-1].BLC;

	//int SBLZCNumTmp, EBLZCNumTmp ;

	//pzLinepTemp->assertWriteEnabled();
	//pzLinepTemp->BZLCarray.RemoveAll();

	//CArray<BAS_DRAW_FUN::BZLCStructTable , BAS_DRAW_FUN::BZLCStructTable > iRxdBZLCArray ;

	//GetiRxdBZLCArray( iRXD , iRxdBZLCArray );

	//pzLinepTemp->BZLCarray.Append(iRxdBZLCArray);

	//pzLinepTemp->SortBZLCArray();

	//pzLinepTemp->OnReCalBZLC(pzLinepTemp->BZLCarray);
	//	pzLinepTemp->BZLC2(pWd,100,draw_zybz,2);



	//tary = pzLinepTemp->tarray ;
	//xyarray = pzLinepTemp->array ;
	GTZX_JYX_ROAD* pzLinepTemp = NULL;
	if (_tcscmp(xlname,L"��ͨ��Ƣ���")==0)
	{
		pzLinepTemp = GetiRxdtoGTZXplinp_GTX(iRXD);
	}
	else
		pzLinepTemp = GetiRxdtoGTZXplinp(iRXD);

	BZLC2( pWd , pzLinepTemp , -1 , 2) ;
	int xysum = pzLinepTemp->tarray;
	//???
	BZLC(pWd,100.0,-1,2,pzLinepTemp->array,xysum,iRxdDlb,ndl , pzLinepTemp );
	//for(i=sdl;i<=edl;i++)
	//{
	//    DLB2x[i] = iRxdDlb[i-sdl];
	//}

	//  �����
	//BZRxdZD( pWd,100,-1,2 , pzLinepTemp ) ; //  ��ע���
	// bz_zybz==0   ������ƫ���ע   bz_zybz=1,-1  �����ұߣ����

	//	BZTZD(pWd,draw_zybz,xyarray,tary,false);//��ע���������
	//free
	//	delete pzLinepTemp ;
	// delete []jday;	jday=NULL;

	if(iRxdDlb)
	{
		delete []iRxdDlb;
		iRxdDlb=NULL;
	}
	if (pzLinepTemp)
	{
		delete pzLinepTemp;
		pzLinepTemp = NULL;
	}

}

void GTZX2_JYX_ROAD::DrawOneBZLC( AcGiWorldDraw* pWd ,GTZX_JYX_ROAD * pSubGtzxToIn ,int CurNum )
{
	int zybz = -1 , type = 2;
	AcGePoint3d Pt;

	AcGePoint3d  pte,ptb;
	int k;
	double B;
	xlpoint  PZtmp; 
	ACHAR str[80];
	double  WB;

	WB=1.5*texth;

	PZtmp.lc=BZLCarray[CurNum].xyTocml;  // ͳһ���	
	k=pSubGtzxToIn->xlpoint_pz(pSubGtzxToIn->array,pSubGtzxToIn->tarray,&PZtmp);   

	B=PZtmp.a;
	//�����
	Pt[Y]=PZtmp.x; 
	Pt[X]=PZtmp.y;
	Pt[Z]=0.0;

	//�����
	ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
	ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);
	double  b=B+pi*0.5;
	b=pi*0.5-b; if (zybz==-1) b=b+pi;
	if (b<0.0) b=2*pi+b;		 
	if (b>2*pi) b=b-2*pi;		         
	if(type<=1)  b=b-pi*0.5; 
	_tcscpy(str,BZLCarray[CurNum].kml);
	_tcscat(str,L" ��־���");
	AcGePoint3d BZPt;
	BZPt.y=ptb[Y]-(0.25*texth)*cos(B);
	BZPt.x=ptb[X]-(0.25*texth)*sin(B);

	G_maketext(pWd,BZPt,str,b,1.5*texth,QXYSColor,1);
	//���յ�
	pte[X]=PZtmp.y+(_tcslen(str)*1.2*texth)*sin(B+pi*0.5*zybz);
	pte[Y]=PZtmp.x+(_tcslen(str)*1.2*texth)*cos(B+pi*0.5*zybz);
	G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
}

void GTZX2_JYX_ROAD::DrawBZLC(AcGiWorldDraw* pWd)  
{
	int CurRXDNum = 0;
	GTZX_JYX_ROAD *GtzxLinpTmp = NULL ;
	for(int i=0;i<BZLCarray.GetSize();i++)
	{
		CurRXDNum = GetiRXDFromNE(BZLCarray[i].Y,BZLCarray[i].X);
		if(CurRXDNum==-1)continue ;
		else
		{
			GtzxLinpTmp = GetiRxdtoGTZXplinp( CurRXDNum ) ;
			DrawOneBZLC( pWd , GtzxLinpTmp , i ) ;
			if (GtzxLinpTmp)
			{
				delete GtzxLinpTmp;
				GtzxLinpTmp = NULL;
			}
		}

	}

}


void GTZX2_JYX_ROAD::BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type,double (*xyarray)[10],int &tary,EXDLB *iExDlb,int ndl , GTZX_JYX_ROAD *pSubGtzxToIn )  
	//  ��ע���������̱�  //    zybz=1,-1  �����ұߣ���� //���߶�����//������
{
	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,BZlc,lc,b,DDlength,DL;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[100],chr[100],ch[100];
	xlpoint  PZtmp;
	AcDbObjectId eId;
	_tcscpy(GH,iExDlb[0].EGH);
	WB=1.5*texth;
	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// ������ͳһ���
	DDlength=iExDlb[0].ELC;

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

			//
			PZtmp.lc=lc;  // ͳһ���
			if (pSubGtzxToIn->XLShuXing=="��ͨ��ƻ���")
				k = pSubGtzxToIn->xlpoint_pz_GTX(xyarray,tary,&PZtmp);
			else
			{	
				PZtmp.lc= pSubGtzxToIn->CalcmlTonewcml(PZtmp.lc);
				k=xlpoint_pz(xyarray,tary,&PZtmp); 
			}

			if (k>0&& k<=6)  //�н�
			{
				B=PZtmp.a;
				pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
				pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
				ptb[X]=PZtmp.y;
				ptb[Y]=PZtmp.x; 
				G_makeline(pWd,ptb,pt,2);

				//PZtmp.lc=lc-1.2*texth;
				//if (pSubGtzxToIn->XLShuXing=="��ͨ��ƻ���")
				//	k = pSubGtzxToIn->xlpoint_pz_GTX(xyarray,tary,&PZtmp);
				//else
				//	k=xlpoint_pz(xyarray,tary,&PZtmp); 
				//p1[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
				//p1[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

				//PZtmp.lc=lc+2.0*texth;
				//if (pSubGtzxToIn->XLShuXing=="��ͨ��ƻ���")
				//	k = pSubGtzxToIn->xlpoint_pz_GTX(xyarray,tary,&PZtmp);
				//else
				//	k=xlpoint_pz(xyarray,tary,&PZtmp); 
				//pte[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
				//pte[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

				p1[Y] = PZtmp.x - 1.2*texth*cos(B);
				p1[X] = PZtmp.y - 1.2*texth*sin(B);
				p1[X]=p1[X]+(WB+2*texth)*sin(B+pi*0.5*zybz);
				p1[Y]=p1[Y]+(WB+2*texth)*cos(B+pi*0.5*zybz);

				pte[Y] = PZtmp.x + 1.2*texth*cos(B);
				pte[X] = PZtmp.y + 1.2*texth*sin(B);
				pte[X]=pte[X]+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
				pte[Y]=pte[Y]+(WB+2*texth)*cos(B+pi*0.5*zybz) ;



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
				G_maketext(pWd,p1,chr,b,texth,QXYSColor,1);  

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
				G_maketext(pWd,pte,chr,b,texth,QXYSColor,1);		   
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
			//lc= pSubGtzxToIn->CalcmlTonewcml(lc);
			PZtmp.lc=lc;  // ͳһ���

			if (pSubGtzxToIn->XLShuXing=="��ͨ��ƻ���")
				k = pSubGtzxToIn->xlpoint_pz_GTX(xyarray,tary,&PZtmp);
			else
			{
				PZtmp.lc= pSubGtzxToIn->CalcmlTonewcml(PZtmp.lc);
				k=xlpoint_pz(xyarray,tary,&PZtmp); 
			}

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

				G_maketext(pWd,pte,ch,b,texth,QXYSColor,1);
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

	/*
	DL=iExDlb[iDL].ELC-iExDlb[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���

	lc=iExDlb[iDL].ELC;  // �ö�ͳһ���

	PZtmp.lc=m_pm1->TYLC1(lc,m_pm1->NDL,m_pm1->DLB);

	PZtmp.lc = m_pm1->CalcmlTonewcml(PZtmp.lc);


	xlpoint_pz(xyarray,tary,&PZtmp);  
	*/	

	PZtmp.x = pSubGtzxToIn->JDarray[0].N ;
	PZtmp.y = pSubGtzxToIn->JDarray[0].E ;


	B = pSubGtzxToIn->array[1][5];
	pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
	pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
	ptb[X]=PZtmp.y;
	ptb[Y]=PZtmp.x; 

	G_makeline(pWd,ptb,pt,2);


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

	_tcscpy(GH,iExDlb[0].EGH);
	_tcscpy(GH1,iExDlb[0].BGH);

	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH1,iExDlb[0].BLC,NLC,1));  // ��ǰ���
	_tcscpy(chr,L"  ");
	_tcscat(chr,ch);
	G_maketext(pWd,p1,chr,b,texth,QXYSColor,1);  

	ACHAR RXFlag[80];
	//plh04.11.26
	_stprintf(RXFlag,L"%s��� ",iExDlb[0].RXDName);


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
	G_maketext(pWd,pte,chr,b,texth,QXYSColor,1);




	//��ע���ж��յ�
	iDL=ndl-1;
	RxdNo=iExDlb[iDL].no;
	DL=iExDlb[iDL].ELC-iExDlb[iDL].BLC;//����ֵ=�Ϻ����-��ǰ���
	_tcscpy(GH,iExDlb[iDL].EGH);
	_tcscpy(GH1,iExDlb[iDL].BGH);
	/*
	lc = iExDlb[iDL].TYLC ;  // ͳһ���

	lc= pSubGtzxToIn->CalcmlTonewcml(lc);
	PZtmp.lc=lc;
	xlpoint_pz(xyarray,tary,&PZtmp);  

	B=PZtmp.a;

	*/

	ACHAR ctmp[80];
	_tcscpy(ctmp,LCchr(GH,iExDlb[iDL].ELC,3,1));
	GetPZFromXCLC2X(ctmp,PZtmp);

	PZtmp.x = pSubGtzxToIn->JDarray[pSubGtzxToIn->JDarray.GetSize()-1].N ;
	PZtmp.y = pSubGtzxToIn->JDarray[pSubGtzxToIn->JDarray.GetSize()-1].E ;



	B = pSubGtzxToIn->array[pSubGtzxToIn->tarray-1][5];


	pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
	pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
	ptb[X]=PZtmp.y;
	ptb[Y]=PZtmp.x; 
	G_makeline(pWd,ptb,pt,2);


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

	_stprintf(RXFlag,L"%s�յ� ",iExDlb[0].RXDName);


	_tcscpy(ch,RXFlag);
	_tcscat(chr,L" ");
	_tcscat(ch,LCchr(GH1,iExDlb[iDL].BLC,NLC,1));  // ��ǰ���
	_tcscpy(chr,L"  ");
	_tcscat(chr,ch);  
	G_maketext(pWd,p1,chr,b,texth,QXYSColor,1);  

	_tcscpy(ch,L"\0");
	_tcscpy(ch,LCchr(GH,iExDlb[iDL].ELC,NLC,1));     // �Ϻ����
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


	G_maketext(pWd,pte,chr,b,texth,QXYSColor,1);
}


//  �겢�жΡ�������
Adesk::Boolean GTZX2_JYX_ROAD::BZTZD2(AcGiWorldDraw* pWd,double dlc,int zybz,int type)  //  ��ע���
	// bz_zybz==0   ������ƫ���ע   bz_zybz=1,-1  �����ұߣ����
{    
	//  bz_zybz=1  �����ұ�
	//	zybz = -1;  // ���

	AcGePoint3d Pt;
	AcGePoint3d  pte,ptb;
	int k;
	double B;
	xlpoint  PZtmp; 
	ACHAR str[80],str2[80];
	double  WB;
	//��־���
	WB=1.5*texth;

	double cml=0.0,lc=0.0;
	ACHAR kml[80];
	double xlc;
	ACHAR GH[8];

	for(int i=0;i<NJD;i++)
	{
		if(QXB[i].LorR> 0)
		{
			zybz =1;
		}
		else
		{
			zybz =-1;
		}

		//		if( JD2[i].JDXZ==1 || JD2[i].JDXZ==R_X_OneR || JD2[i].JDXZ==R_XStart || JD2[i].JDXZ==R_XEnd)continue ;

		if(i>0 && i< NJD-1 )
		{
			for(int j=0;j<4;j++)
			{
				if(j==0)
					cml=QXB[i].KZH;
				if(j==1)
					cml=QXB[i].KHY;
				if(j==2)
					cml=QXB[i].KYH;
				if(j==3)
					cml=QXB[i].KHZ;

				PZtmp.lc = cml;

				k=xlpoint_pz(array,tarray,&PZtmp); 

				CString ckml ;
				if( _tcscmp( xlname , L"��ͨ��Ƣ���" ) == 0 )
				{
					ckml = GetGTX2Xclc(PZtmp.x,PZtmp.y);
					_tcscpy(kml,ckml);
				}
				else
				{
					xlc = XCLC(cml ,GH , DLB2x, NDL );
					_tcscpy(kml,L"\0");
					_tcscpy(kml,LCchr(GH,xlc,NLC,1));
				}

				//	lc= CalcmlTonewcml(cml);

				B=PZtmp.a;
				//�����
				Pt[Y]=PZtmp.x; 
				Pt[X]=PZtmp.y;
				Pt[Z]=0.0;

				//�����
				ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
				ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);

				ptb[Y] = ptb[Y] - 0.25*texth*cos(B);
				ptb[X] = ptb[X] - 0.25*texth*sin(B);

				if( JDarray[i].R > 1.0e-5 )
				{
					if( JDarray[i].l1 > 1.0e-5 )
					{
						if(j==0) _stprintf(str2,L"ZH %s",kml);
						if(j==1) _stprintf(str2,L"HY %s",kml);
					}
					else
					{
						if(j==0) continue ;
						if(j==1) _stprintf(str2,L"ZY %s",kml);
					}

					if( JDarray[i].l2 > 1.0e-5 )
					{
						if(j==2) _stprintf(str2,L"YH %s",kml);
						if(j==3) _stprintf(str2,L"HZ %s",kml);
					}
					else
					{
						if(j==2) _stprintf(str2,L"YZ %s",kml);
						if(j==3) continue ;
					}

				}
				else continue ;


				double b=0.0;
				if(zybz>0)
				{
					b =pi/2.0-( B +pi*0.5*zybz) + pi ;
					if (b<0.0) b=2*pi+b;		 
					if (b>2*pi) b=b-2*pi;
				}
				else 
				{
					b =pi/2.0-( B +pi*0.5*zybz) ;
					if (b<0.0) b=2*pi+b;		 
					if (b>2*pi) b=b-2*pi;
				}
				AcGePoint3d BZPt;
				BZPt.y=ptb[Y]-(0.25*texth)*cos(B);
				BZPt.x=ptb[X]-(0.25*texth)*sin(B);

				//���յ�
				pte[X]=PZtmp.y+(_tcslen(str2)*1.2*texth)*sin(B+pi*0.5*zybz);
				pte[Y]=PZtmp.x+(_tcslen(str2)*1.2*texth)*cos(B+pi*0.5*zybz);

				G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
				if(zybz>0)
					G_maketext(pWd,pte,str2,b,1.5*texth,QXYSColor,1);
				else G_maketext(pWd,ptb,str2,b,1.5*texth,QXYSColor,1);

			}

		}

	}

	return TRUE;
}



// zybz==0   ������ƫ���ע   zybz=1,-1  �����ұߣ����
//xyxyarray->��Ԫ���飬nxy->��Ԫ��,�Ƿ����ע���к�S�佻��������
void GTZX2_JYX_ROAD::BZTZD(AcGiWorldDraw* pWd,int draw_zybz,double (*xyarray)[10],int nxy,bool IsOnlyB_S)//��ע�������������
{
	Chinese_Engbz_type=2;
	AcGePoint3d  ptb,pt,pte;
	double WB,lc, b,l,INDATA[5],L0;
	int i, j,k , KK=7,zybz=draw_zybz;
	ACHAR chr[50],ch[40],tch[6], GH[8],DGH[8] ; // ,lcchr[20];
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
			if(JD2[iJD].JDXZ!=R_X && JD2[iJD].JDXZ!=R_X_OneR )BZFlag=true;//���л�S�佻��
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

			//???
			//	if( fabs(INDATA[0]-2)<0.00001 && l<0.00000001 ) P0tmpa = INDATA[3];

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
				if(L0>0.001)G_maketext(pWd,pt,chr,b,texth,QXYSColor,1);  // �� ֱ�ߡ�԰���ߡ��������߳���
		}		

		xlzbxy(&P0tmp,&PZtmp,l,INDATA,0); // ĳ��Ԫ�յ�
		//	if( fabs(INDATA[0]-2)<0.00001 && l<0.00000001 ) P0tmpa = INDATA[3];

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

				if(_tcscmp( xlname , L"��ͨ��Ƣ���")==0)
				{
					CString GTZBkml; //��ͨ��ע���ֳ����

					GTZBkml = GetGTX2XclcFromcml(P0tmp.lc);
					_tcscpy(ch , GTZBkml) ;
					_tcscat(chr,L" ");
					_tcscat(chr,ch);
				}
				else
				{
					lc=XCLC(P0tmp.lc,GH,DLB2x,NDL);

					_tcscpy(ch,L"\0"); 
					_tcscpy(ch,LCchr(GH,lc,NLC,1));

					_tcscat(chr,L" ");
					_tcscat(chr,ch);

				}



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
				G_maketext(pWd,pt,chr,b, texth,QXYSColor,1);


		}//else if(i>1 && i<=nxy-1)
		P0tmp=PZtmp;
	}

}

void GTZX2_JYX_ROAD::CalXYArrayFromJdArray(double (*jdarray)[6], int njd,double (*qxb)[9],double (*xyarray)[10],int &tary)
	//�ɽ������飬������������߱�,������Ԫ���飬...
{
	double fb,fe,db,de,aa,c,lcb,lce,T0=0.0, interal=18;
	ACHAR zy[2];  // ,GH[20];
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
			if( (db-T1-T0)<0) ads_printf(L"  ����ǰһ����HZ/YZ���Ϸ�\n");
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

double GTZX2_JYX_ROAD::XCLC(double TYLC,ACHAR *GH, EXDLB *ExDlb, int ndl)
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
	if(GetRXDNum()>0&& TYLC>=ExDlb[0].TYLC&&iDL>=0&&iDL<NDL-1&&ExDlb[iDL].no==ExDlb[iDL+1].no)
	{
		_tcscpy(GH,ExDlb[iDL].EGH);
		TYLC = CalnewcmlTocml(TYLC);
		return TYLC-ExDlb[iDL].TYLC+ExDlb[iDL].ELC;//��ǰ�������ۼӳ��ȣ�ǰ��������ۼӳ��ȣ�ǰ������Ϻ����
	}
	else//ȫ�������ж� �������е��������ж���
	{
		//ͶӰ��һ���ϵ����//��ֱһ��
		xlpoint Pt;
		Pt.lc=TYLC;

		xlpoint_pz(array,tarray,&Pt);
		double tylc1x = TYLC ;
		if(m_pm2)
		{
			double tylc2x=m_pm2->PROJ_ML(m_pm2->array,m_pm2->tarray,Pt.x,Pt.y,Pt.x,Pt.y);
			return m_pm2->XCLC(tylc2x,GH,m_pm2->DLB2x,m_pm2->NDL);
		}
		else
		{
			tylc1x=m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,Pt.x,Pt.y,Pt.x,Pt.y);
			tylc1x = m_pm1->CalnewcmlTocml(tylc1x);
			return m_pm1->XLC(tylc1x,GH,m_pm1->NDL);
		}
	}	
}

void GTZX2_JYX_ROAD::AddRXD(AcGePoint3d PT1,	AcGePoint3d OutPt,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd,BOOL isPreMove )//������������
{
	assertReadEnabled();
	njd=0;

	if(GetiRXDFromNE(PT1.y,PT1.x)>=0)
	{
		AfxMessageBox(L"�˴������ж�!");
		return;
	}

	njd=-1;
	AcGePoint3d PT2;
	double x1,y1,x2,y2,L1,L2,R;
	if(acedGetPoint(asDblArray(PT1),L"ȷ���������յ�:\n",asDblArray(PT2)))
	{
		int JDi=GetPreJD(OutPt.y,OutPt.x);

		if(JDi>=NJD-1)return;
		NJD=getJD_RL(0,&R,&L1,&L2,&y1,&x1);

		if(JDi>=0&&JDi<NJD-1)
		{
			NJD=getJD_RL(JDi,&R,&L1,&L2,&y1,&x1);			
			NJD=getJD_RL(JDi+1,&R,&L1,&L2,&y2,&x2);	
			//			acutPrintf(L"JDi=%d,x1=%lf,y1=%lf\n",JDi,x1,y1);
			//			acutPrintf(L"JDi=%d,x2=%lf,y2=%lf\n",JDi+1,x2,y2);
		}
		//		acutPrintf(L"x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",m_xk1,m_yk1,m_xk2,m_yk2);

		if(JDi>=0&&JDi<NJD-1)
		{
			AcGePoint3d interPt;
			AcDbLine L1(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));
			AcDbLine L2(PT1,PT2);
			//plh06.01
			if(INTERS(&interPt.x,&interPt.y,x1,y1,x2,y2,PT1.x,PT1.y,PT2.x,PT2.y)>=0)
			{
				double T2=QXB[JDi].T2;//���г�
				if(T2<0.00001)T2=50.0;//������Ϊ0
				double fwj1=fwj(x1,y1,x2,y2);
				AcGePoint3d tempPt(x1,y1,0.0);

				getLineEndPt(tempPt,T2,fwj1,tempPt);

				//		double BAS_DRAW_FUN::GetRFromT1T2(double &x1, double &y1, double &x0,double &y0,double &x2,double &y2, BOOL isJYGuiFanCS , bool IsStandard , double DV ,int INTRadius , int JUDGET012)
				double L0 = 0.0;
				double rad = GetRFromT1T2(L0,tempPt.x,tempPt.y,interPt.x,interPt.y,PT2.x,PT2.y,!isGJD,m_GuiFanCS,true,DV,0,1 , isPreMove );
				if(rad<0.0)return;

				njd=3;
				int i=0;
				_tcscpy(jdarray[i].JDNum,L"0");
				//				double jiao;
				//				xyddaa(interPt.x,interPt.y,tempPt.x,tempPt.y,&jiao);
				jdarray[i].N=tempPt.y;//+0.1*sin(jiao);//��ǰ����һ��
				jdarray[i].E=tempPt.x;//+0.1*cos(jiao);
				jdarray[i].l1=0.0;
				jdarray[i].R=0.0;
				jdarray[i].l2=0.0;

				i=1;
				_tcscpy(jdarray[i].JDNum , L"1");
				jdarray[i].N=interPt.y;
				jdarray[i].E=interPt.x;
				//				jdarray[i].l1=get_l0(rad,DV,!isGJD);
				jdarray[i].l1=L0;
				jdarray[i].R=rad;
				//				jdarray[i].l2=get_l0(rad,DV,!isGJD);
				jdarray[i].l2=L0;

				i=2;
				_tcscpy(jdarray[i].JDNum , L"2");
				jdarray[i].N=PT2.y;
				jdarray[i].E=PT2.x;
				jdarray[i].l1=0.0;
				jdarray[i].R=0.0;
				jdarray[i].l2=0.0;
			}
			else
			{
				acutPrintf(L"AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd)....if(Acad::eOk==L1.intersectWith(&L2,AcDb::kExtendBoth,interPt,0,0)&&interPt.length()>0) \n");
			}
		}	

	}
}


void GTZX2_JYX_ROAD::AddRXD(AcGePoint3d PT1,	AcGePoint3d PT2,AcGePoint3d OutPt,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd,BOOL isPreMove )//������������
{
	assertReadEnabled();
	njd=0;

	if(GetiRXDFromNE(PT1.y,PT1.x)>=0)
	{
		AfxMessageBox(L"�˴������ж�!");
		return;
	}

	njd=-1;
	double x1,y1,x2,y2,L1,L2,R;

	int JDi=GetPreJD(OutPt.y,OutPt.x);

	if(JDi>=NJD-1)return;
	NJD=getJD_RL(0,&R,&L1,&L2,&y1,&x1);

	if(JDi>=0&&JDi<NJD-1)
	{
		NJD=getJD_RL(JDi,&R,&L1,&L2,&y1,&x1);			
		NJD=getJD_RL(JDi+1,&R,&L1,&L2,&y2,&x2);	
		//			acutPrintf(L"JDi=%d,x1=%lf,y1=%lf\n",JDi,x1,y1);
		//			acutPrintf(L"JDi=%d,x2=%lf,y2=%lf\n",JDi+1,x2,y2);
	}
	//		acutPrintf(L"x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",m_xk1,m_yk1,m_xk2,m_yk2);

	if(JDi>=0&&JDi<NJD-1)
	{
		AcGePoint3d interPt;
		AcDbLine L1(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));
		AcDbLine L2(PT1,PT2);
		//plh06.01
		if(INTERS(&interPt.x,&interPt.y,x1,y1,x2,y2,PT1.x,PT1.y,PT2.x,PT2.y)>=0)
		{
			double T2=QXB[JDi].T2;//���г�
			if(T2<0.00001)T2=50.0;//������Ϊ0
			double fwj1=fwj(x1,y1,x2,y2);
			AcGePoint3d tempPt(x1,y1,0.0);

			getLineEndPt(tempPt,T2,fwj1,tempPt);

			//		double BAS_DRAW_FUN::GetRFromT1T2(double &x1, double &y1, double &x0,double &y0,double &x2,double &y2, BOOL isJYGuiFanCS , bool IsStandard , double DV ,int INTRadius , int JUDGET012)
			double L0 = 0.0;
			double rad = GetRFromT1T2(L0,tempPt.x,tempPt.y,interPt.x,interPt.y,PT2.x,PT2.y,!isGJD,m_GuiFanCS,true,DV,0,1 , isPreMove );
			if(rad<0.0)return;

			njd=3;
			int i=0;
			_tcscpy(jdarray[i].JDNum,L"0");
			//				double jiao;
			//				xyddaa(interPt.x,interPt.y,tempPt.x,tempPt.y,&jiao);
			jdarray[i].N=tempPt.y;//+0.1*sin(jiao);//��ǰ����һ��
			jdarray[i].E=tempPt.x;//+0.1*cos(jiao);
			jdarray[i].l1=0.0;
			jdarray[i].R=0.0;
			jdarray[i].l2=0.0;

			i=1;
			_tcscpy(jdarray[i].JDNum , L"1");
			jdarray[i].N=interPt.y;
			jdarray[i].E=interPt.x;
			//				jdarray[i].l1=get_l0(rad,DV,!isGJD);
			jdarray[i].l1=L0;
			jdarray[i].R=rad;
			//				jdarray[i].l2=get_l0(rad,DV,!isGJD);
			jdarray[i].l2=L0;

			i=2;
			_tcscpy(jdarray[i].JDNum , L"2");
			jdarray[i].N=PT2.y;
			jdarray[i].E=PT2.x;
			jdarray[i].l1=0.0;
			jdarray[i].R=0.0;
			jdarray[i].l2=0.0;
		}
		else
		{
			acutPrintf(L"AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd)....if(Acad::eOk==L1.intersectWith(&L2,AcDb::kExtendBoth,interPt,0,0)&&interPt.length()>0) \n");
		}
	}	
}

int GTZX2_JYX_ROAD::getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE )
{   
	assertReadEnabled();	
	if (JDi>=0 && JDi<=NJD-1)  //  ��~�յ�
	{  *NN=JDarray[JDi].N;
	*EE=JDarray[JDi].E;
	*L1i=JDarray[JDi].l1;
	*L2i=JDarray[JDi].l2;
	*Ri=JDarray[JDi].R;
	};	 
	return NJD;
}

bool GTZX2_JYX_ROAD::AddRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray,CArray<QXBStruct,QXBStruct>& qxb,int njd,double N,double E,ACHAR EGH[],ACHAR JDGH[])//plh04.11.25
	//һ��GTZXʵ����Ϊ���ж�����ߺϲ�����ʱ��β����ϴ���
{

	assertWriteEnabled();
	ACHAR rxdname[256];
	if(GetiRXDFromNE(N,E)>=0)
	{
		AfxMessageBox(L"�˴������ж�!");
		return false;
	}

	GTZX_JYX_ROAD *pGtzxTmp = NULL ;

	if(m_pm2==NULL)
	{
		pGtzxTmp = m_pm1 ;
	}
	else pGtzxTmp = m_pm2->m_pm1 ;


	int i=0;
	double x1,y1,x2,y2,x3,y3;//��Ļ����:
	//1->newGtzx �����ڶ������� ��2->����ʵ����x0,y0ǰһ������,3->����ʵ���Ϻ�һ������
	double xe,ye;//newGtzx���һ���������Ļ����
	x1=jdarray[njd-2].E;
	y1=jdarray[njd-2].N;
	xe=jdarray[njd-1].E;
	ye=jdarray[njd-1].N;

	if(fabs(N)<0.0001&&fabs(E)<0.0001)
	{
		N=ye;	E=xe;
	}
	int JDi=GetPreJD(N,E);



	//	BOOL StartPointIsOnXL,EndPointIsOnXL;//���/�յ��Ƿ����ӳ�����


	//����jdarray��һ���㶨λ,�õ�ǰ�����±�,�������
	//	int StartPointToJDi=GetPreJD(jdarray[0].N,jdarray[0].E);

	//	StartPointIsOnXL = JudgePtIsOnXL( jdarray[1].N,jdarray[1].E ,JDarray[StartPointToJDi].N, JDarray[StartPointToJDi].E , JDarray[StartPointToJDi+1].N, JDarray[StartPointToJDi+1].E );


	//������->�ֳ����
	//���߶�Ӧ��HZ��������̼����λ����
	//double HZkmlToOutXL = QXB[JDi].KHZ ;

	//xlpoint PZTmp ;
	//PZTmp.lc = HZkmlToOutXL ;
	//xlpoint_pz(array,tarray,&PZTmp);

	//double tylc1x=PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,PZTmp.x,PZTmp.y,PZTmp.x,PZTmp.y);

	//tylc1x = pGtzxTmp->LocateHundredMeterOnLine(tylc1x,false);//��HZ����������
	//ACHAR GHHZ1x[8];

	int CurRXDNo = GetRXDNum()+1;//��ǰ�ǵڼ������ж�
	if(JDi>=0&&JDi<NJD-1)
	{
		x2=JDarray[JDi].E;//��array��λ��Ԫ�׳���,����������JDarray��λ��������
		y2=JDarray[JDi].N;
		x3=JDarray[JDi+1].E;
		y3=JDarray[JDi+1].N;

		AcDbLine L1(AcGePoint3d(x1,y1,0.0),AcGePoint3d(xe,ye,0.0));
		AcDbLine L2(AcGePoint3d(x2,y2,0.0),AcGePoint3d(x3,y3,0.0));
		AcGePoint3d interPt;//GTZX ����ߵĽ���,
		//plh06.01
		if(INTERS(&interPt.x,&interPt.y,x1,y1,xe,ye,x2,y2,x3,y3)>=0)
		{
			/*distFromJDToJD2 =  pow( ( interPt.x - x3 ) , 2.0 ) + pow( ( interPt.y - y3 ) , 2.0 ) ;
			distFromJDToJD2 = sqrt(distFromJDToJD2);

			if(distFromJDToJD>distFromJDToJD2)EndPointIsOnXL = TRUE ;
			else EndPointIsOnXL = FALSE ;*/

			AcGePoint3d tPt1(x1,y1,0.0);
			double T2=qxb[njd-2].T2;//���г�//plhmodify04.13
			double fwj1=fwj(tPt1.x,tPt1.y,interPt.x,interPt.y);
			getLineEndPt(tPt1,T2,fwj1,tPt1);

			AcGePoint3d tPt2(x3,y3,0.0);
			double T1=QXB[JDi+1].T1;//ǰ�г�
			double fwj2=fwj(x3,y3,x2,y2);
			getLineEndPt(tPt2,T1,fwj2,tPt2);//plh05.11

			double L0 = 0.0;
			double rad=GetRFromT1T2(L0,tPt1.x,tPt1.y,interPt.x,interPt.y,tPt2.x,tPt2.y,!isGJD,m_GuiFanCS,true,DV,0,120);

			if(rad>0)
			{

				DoubleLineData *OneRXDJDB;

				///����һ�����жν����
				OneRXDJDB=new DoubleLineData[njd+1];
				for(i=0;i<njd;i++)
				{
					OneRXDJDB[i].JDXZ=R_X;   //?????*****//
					OneRXDJDB[i].ISShow=true;
					OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
					OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
					OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������
					OneRXDJDB[i].x=jdarray[i].N;
					OneRXDJDB[i].y=jdarray[i].E;
					OneRXDJDB[i].Lo1=jdarray[i].l1;
					OneRXDJDB[i].R=jdarray[i].R;
					OneRXDJDB[i].Lo2=jdarray[i].l2;
					OneRXDJDB[i].NYDL =0.0;	
					OneRXDJDB[i].qxjk=0.0;
					OneRXDJDB[i].ISXJJFirst=false;

					_stprintf( OneRXDJDB[i].JDNum , L"RX%d%s%d" ,CurRXDNo,JDGH, i );
				}

				i=njd-1;
				OneRXDJDB[i].ISShow=true;
				OneRXDJDB[i].JDXZ=R_X;  //?????*****//
				OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
				OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
				OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������
				OneRXDJDB[i].x=interPt.y;
				OneRXDJDB[i].y=interPt.x;
				//				OneRXDJDB[i].Lo1=get_l0(rad,DV,!isGJD);
				OneRXDJDB[i].Lo1=L0;
				OneRXDJDB[i].R=rad;
				//				OneRXDJDB[i].Lo2=get_l0(rad,DV,!isGJD);
				OneRXDJDB[i].Lo2=L0;
				OneRXDJDB[i].NYDL =0.0;	
				OneRXDJDB[i].qxjk=0.0;
				OneRXDJDB[i].ISXJJFirst=false;

				_stprintf( OneRXDJDB[i].JDNum , L"RX%d%s%d" ,CurRXDNo,JDGH, njd-1);

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

				_stprintf( OneRXDJDB[i].JDNum , L"RX%d%s%d" ,CurRXDNo,JDGH, njd);

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

void GTZX2_JYX_ROAD::InsertOneRXD(DoubleLineData *OneRXDJDB,int njd,ACHAR EGH[])//plh04.11.25
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

	if( JD2[JDNO2].JDXZ==R_X  || JD2[JDNO1].JDXZ==R_X )  //?????*****//
	{ 
		//	ads_alert(L"1");		
		//	return;
	}

	DoubleLineData JDtemp;

	if(JDNO1>=0&&JDNO2>=JDNO1&&JDNO2<NJD)
	{	
		int dJD=njd-2-(JDNO2-JDNO1-1);//����ƶ�λ��//ȥ����β��������


		JDtemp = JD2[JDNO2];
		int newNJD = NJD + dJD ;

		DoubleLineData NewJD2[MAXJDNUM2x];

		int i;
		for(i=NJD-1;i>=JDNO2;i--)
		{		
			NewJD2[newNJD-1]=JD2[i];
			newNJD -- ;
		}

		newNJD = NJD + dJD ;

		for( i=NJD-1;i>=JDNO2;i--)
		{		
			JD2[newNJD-1]=NewJD2[newNJD-1];
			newNJD -- ;
		}

		NJD+=dJD;
		for(i=JDNO1+1;i<JDNO2+dJD+1;i++)
		{
			if(i<JDNO2+dJD)JD2[i]=OneRXDJDB[i-JDNO1];//��1�Ž��㼴�ڶ�������
			else JD2[i]=JDtemp;

			if(i==JDNO1+1)
			{
				JD2[i].jdno1=JD2[i-1].jdno1+1;
				JD2[i].xjj=RxdStartXjj;
				if (njd>3)
					JD2[i].JDXZ = R_XStart;
			}
			if(i==JDNO2+dJD-1)
			{
				JD2[i].jdno1=JD2[i+1].jdno1;
				JD2[i].xjj=RxdEndXjj;
				if (njd>3)
					JD2[i].JDXZ = R_XEnd;
			}
		}

	}
	else
	{
		AfxMessageBox(L"insertOneRXD");
		return;
	}


	CalJDdata();
	//	CalJDdata1();
	SetJDdata();

	//���ɶ�����Ԫ����
	getpxy_DATA();

	Create2XDLB(FindiRXD(JDNO1+1),EGH);//plh04.11.25

}




void GTZX2_JYX_ROAD::DeleteOneRXDByXLPoint(double N,double E)
	//ָ����·��һ��ɾ����������ж�
{
	assertWriteEnabled();

	int RxdNum=GetRXDNum();
	for(int i=0;i<RxdNum;i++)
	{
		//��Ѱĳ���жε����ն������
		int sDLno = 0 ;
		int eDLno = 0 ;
		GetiRXDDLinfo(i , sDLno , eDLno);

		//if(RxdNum==2)
		//{
		//	sDLno ++ ;
		//	eDLno ++ ;
		//}

		double cml = PROJ_ML(array,tarray,N,E,N,E);

		ACHAR GH[8];
		double kml = XLC(cml,GH,NDL);

		CString Skml ;
		ACHAR SKmlACHAR[80];
		//��ǰ���
		//_tcscpy( SKmlACHAR , L"\0" ) ;
		//_tcscpy( GH , L"\0" );
		//_tcscpy( GH , DLB2x[sDLno].EGH );
		//_tcscpy( SKmlACHAR , LCchr( GH,DLB2x[sDLno].ELC,3,1) );
		//Skml = SKmlACHAR ;
		//double Bcml = TYLC2x( Skml );
		double Bcml = DLB2x[sDLno].TYLC;

		//�Ϻ����
		//_tcscpy( SKmlACHAR , L"\0" ) ;
		//_tcscpy( GH , L"\0" );
		//_tcscpy( GH , DLB2x[eDLno].BGH );

		//_tcscpy(SKmlACHAR , LCchr(GH,DLB2x[eDLno].BLC,3,1) );
		//Skml = SKmlACHAR ;
		//double Ecml = TYLC2x( Skml );
		double Ecml = DLB2x[eDLno].TYLC;


		if( kml > Bcml - 0.0001  && kml < Ecml + 0.0001 )
		{
			DeleteiRXDBZLC(i);
			DeleteOneRXD(i);
			return;
		}
	}
	ads_printf(L"ʰȡ�������ж�\n");
}



//void GTZX2::DeleteOneRXD(int iRXD)
////��ָ�����жθĲ��У���ɾ����Ӧ����
//{
//	//	assertWriteEnabled();
//	//��ɾ����Ӧ���жεĽ��㣬��������Ӧ�Ĳ��н���
//	int FirstJD,EndJD;//ĳһ���ж���ʼ�����
//	int PreJD,NextJD;//ĳһ���ж�ǰ�󽻵��
//	int PreJD1x,NextJD1x;//ĳһ���ж�ǰ�󽻵��Ӧ��һ�߽������
//	int DelJD,AddJD;//Ҫɾ���Ľ��㣬Ҫ���ӵĽ���
//	int dJD;//���潻����ǰ�ƶ�λ��
//	GetJDXHofiRXD(iRXD,FirstJD,EndJD);//��ȡiRXD����β�������
//	double EndXjj;//���ж����һ��������߼��
//	EndXjj=JD2[EndJD].xjj;
//	PreJD=FirstJD-1;
//	NextJD=EndJD+1;
//	PreJD1x=JD2[PreJD].jdno1;
//	NextJD1x=JD2[NextJD].jdno1;
//	DelJD=NextJD-PreJD-1;
//	AddJD=NextJD1x-PreJD1x-1;
//	dJD=DelJD-AddJD;
//
//	for(int i=FirstJD;i<NJD-DelJD;i++)//ɾ��RXD���㣬��ǰ��λ
//	{
//		JD2[i]=JD2[i+DelJD];
//	}
//	NJD-=DelJD;
//	//ɾ����ȷ��Ҫ������
//	NJD+=AddJD;
//	for(i=NJD-1;i>=FirstJD+AddJD;i--)//���Ӳ��жν���洢λ�ã������λ
//	{
//		JD2[i]=JD2[i-AddJD];
//	}
//	double lo1,lo,D;
//	for(i=FirstJD;i<FirstJD+AddJD; i++)
//	{
//		if(JD2[PreJD].JDXZ==B_X )JD2[i].jdno1=(JD2[PreJD].jdno1+1)+(i-FirstJD);
//		else JD2[i].jdno1=(JD2[PreJD].jdno1)+(FirstJD-i);
//		JD2[i].JDXZ=B_X;//���жν���
//
//		//	_stprintf(JDarray[i].JDNum,L"%d",JD0 + i);
//		//   JDarray[i][0] = JD0 + i;
//		int jdn1 =i-FirstJD +1 + JD2[PreJD].jdno1;//��Ӧһ�߽������
//		JD2[i].xjj=JD2[i-1].xjj;
//		if(i==FirstJD+AddJD-1&&i<NJD-1)JD2[i].xjj = EndXjj;//���һ������߼�����ǰһ�����
//		double xjj = JD2[i].xjj;
//		double EXJD[2];
//		CalEXJD(jdn1,i,EXJD);
//		JD2[i].x = EXJD[0];
//		JD2[i].y = EXJD[1];//��������߽���
//		double R = m_pm1->JDarray[jdn1].R;
//		double JK;
//		JK = get_XJJK(m_pm1->DV,R,isJYGuiFanCS);//�߼��ӿ�
//		JD2[i].qxjk = JK;
//		JD2[i].Lo1 = m_pm1->JDarray[jdn1].l1;
//		JD2[i].R = R;
//		JD2[i].Lo2 = m_pm1->JDarray[jdn1].l2;           
//		lo1 = m_pm1->JDarray[jdn1].l1;//һ�߻���
//		D = fabs(xjj);
//		//ads_printf(L"21:%d %d %d R=%lf lo=%lf D=%lf\n",i,jdn1,JD2[i].jdno1,JD2[i].R,lo1,D);
//		//���߰뾶
//		if(i>0&&i<NJD-1)
//		{
//			double xjj2;
//			xjj2 = JD2[i-1].xjj;//��һ�����߼��
//			//   ads_printf(L"i=%d xjj=%lf xjj2=%lf\n",i,xjj,xjj2);
//			if(fabs(xjj-xjj2)<0.01)//ǰ���߼�����
//			{
//				//xjj�������߼ӿ�............		   
//				if(xjj>0)//һ��Ϊ����
//				{
//					if(m_pm1->QXB[jdn1].LorR<0)//��ת,II������࣬���ٻ���
//					{
//						//���仺��
//						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//���ȱ� 2003.12.4�ṩ
//						JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
//						D  = -1.0*D;
//					}
//					else
//					{
//						//���仺��
//						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
//						JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
//					}
//					//					   ads_printf(L"21:%d lo=%lf \n",i,lo);
//				}
//				else//һ��Ϊ����
//				{
//					if(m_pm1->QXB[jdn1].LorR<0)//��ת,II�����ڲ�
//					{
//						lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
//						JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
//					}
//					else
//					{   //���仺��
//						lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//�� 2003.12.4�ṩ
//						JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
//						D  = -1.0*D;
//					}
//				}
//				//��뾶
//				double B = R - D + (lo1*lo1)/24/R;
//				//			ads_printf(L"lo1=%lf lo=%lf D=%lf JK=%lf B=%lf\n",lo1,lo,D,JK,B);
//				JD2[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//���߰뾶				
//			}
//			//	   ads_printf(L"2:%d %lf %lf \n",i,JD2[i].R,JD2[i].Lo1);
//		}   
//		JD2[i].jdno = JD0 + i;
//	}
//	//////////////////////////////////////////////////////////////////////////
//
//	DeleteiRXDDLB(iRXD);
//}


void GTZX2_JYX_ROAD::DeleteOneRXD(int iRXD)
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
	int SDL,EDL;
	double RXDSml1x,RXDEml1x;
	xlpoint PZ;
	GetiRXDDLinfo(iRXD,SDL,EDL);
	if (m_pm2)
	{
		PZ.lc = DLB2x[SDL].TYLC;
		xlpoint_pz(array,tarray,&PZ);
		RXDSml1x = m_pm2->PROJ_ML(m_pm2->array,m_pm1->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		PreJD1x = m_pm2->DistToNearJD(RXDSml1x,true);
		PZ.lc = DLB2x[EDL].TYLC;
		xlpoint_pz(array,tarray,&PZ);
		RXDEml1x = m_pm2->PROJ_ML(m_pm2->array,m_pm2->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		NextJD1x = m_pm2->DistToNearJD(RXDEml1x,false);
	}
	else
	{
		PZ.lc = DLB2x[SDL].TYLC;
		xlpoint_pz(array,tarray,&PZ);
		RXDSml1x = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		PreJD1x = m_pm1->DistToNearJD(RXDSml1x,true);
		PZ.lc = DLB2x[EDL].TYLC;
		xlpoint_pz(array,tarray,&PZ);
		RXDEml1x = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		NextJD1x = m_pm1->DistToNearJD(RXDEml1x,false);
	}
	//	PreJD1x=JD2[PreJD].jdno1;
	//	NextJD1x=JD2[NextJD].jdno1;
	DelJD=NextJD-PreJD-1;
	AddJD=NextJD1x-PreJD1x-1;
	dJD=DelJD-AddJD;

	int i;
	for( i=FirstJD;i<NJD-DelJD;i++)//ɾ��RXD���㣬��ǰ��λ
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
		//		if(JD2[PreJD].JDXZ==B_X )JD2[i].jdno1=(PreJD1x+1)+(i-FirstJD);
		//		else JD2[i].jdno1=(PreJD1x)+(FirstJD-i);
		JD2[i].jdno1=(PreJD1x+1)+(i-FirstJD);
		JD2[i].JDXZ=B_X;//���жν���

		//	_stprintf(JDarray[i].JDNum,L"%d",JD0 + i);
		//   JDarray[i][0] = JD0 + i;
		int jdn1 =i-FirstJD +1 + PreJD1x;//��Ӧһ�߽������
		JD2[i].xjj=JD2[i-1].xjj;
		if(i==FirstJD+AddJD-1&&i<NJD-1)JD2[i].xjj = EndXjj;//���һ������߼�����ǰһ�����
		double xjj = JD2[i].xjj;
		double EXJD[2];
		CalEXJD(jdn1,i,EXJD);
		JD2[i].x = EXJD[0];
		JD2[i].y = EXJD[1];//��������߽���
		double R = m_pm1->JDarray[jdn1].R;
		double JK;
		JK = get_XJJK(m_pm1->DV,R,isJYGuiFanCS,m_pm1->m_GuiFanCS);//�߼��ӿ�
		JD2[i].qxjk = JK;
		JD2[i].Lo1 = m_pm1->JDarray[jdn1].l1;
		JD2[i].R = R;
		JD2[i].Lo2 = m_pm1->JDarray[jdn1].l2;           
		lo1 = m_pm1->JDarray[jdn1].l1;//һ�߻���
		D = fabs(xjj);
		//ads_printf(L"21:%d %d %d R=%lf lo=%lf D=%lf\n",i,jdn1,JD2[i].jdno1,JD2[i].R,lo1,D);
		//���߰뾶
		if(i>0&&i<NJD-1)
		{
			double xjj2;
			xjj2 = JD2[i-1].xjj;//��һ�����߼��
			//   ads_printf(L"i=%d xjj=%lf xjj2=%lf\n",i,xjj,xjj2);
			if(fabs(xjj-xjj2)<0.01 && fabs(xjj)>0.01)//ǰ���߼�����
			{
				//xjj�������߼ӿ�............		   
				if(xjj>0)//һ��Ϊ����
				{
					if(m_pm1->QXB[jdn1].LorR<0)//��ת,II������࣬���ٻ���
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
					if(m_pm1->QXB[jdn1].LorR<0)//��ת,II�����ڲ�
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
		_stprintf(JD2[i].JDNum,L"%s%d",L"JD",JD2[i].jdno);
	}
	//////////////////////////////////////////////////////////////////////////

	DeleteiRXDDLB(iRXD);
}


int GTZX2_JYX_ROAD::FindiRXD(int iJD)
	//����ָ���������������ж�
{
	//SetJDdata();	
	//getpxy_DATA();
	int iRXD=-1;//�ڼ������ж�
	for(int i=0;i<NJD;i++)
	{
		if( JD2[i].JDXZ == R_XStart )
		{
			iRXD++;
			if (i<NJD-1)
			{
				i++;
			}
			while( JD2[i].JDXZ == R_X || JD2[i].JDXZ == R_XEnd)i++;
			i--;

			if(iJD<=i)
			{
				return iRXD;
			}
		}
		if( JD2[i].JDXZ==R_X_OneR )
		{
			iRXD++;
			if(iJD==i)return iRXD;
		}
	}		
	return iRXD;
}

//  ����һ����������
Adesk::Boolean  GTZX2_JYX_ROAD::addDLM(double tylc,double BLC,double ELC, ACHAR *EGH ,BOOL IsSingleModifyR )
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
	if(!IsSingleModifyR)
	{

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
	else
	{

		for (i=iDL;i<=NDL-1;i++)DLB2x[NDL+iDL-i]=DLB2x[NDL-1+iDL-i];// ����һ��������
		NDL=NDL+1;
		DLB2x[iDL].EGH=EGH;
		DLB2x[iDL+1].BGH=EGH;
		DLB2x[iDL].no=DLB2x[iDL-1].no;

		DLB2x[iDL].BLC=BLC;
		DLB2x[iDL].ELC=ELC;
		double DL=ELC-BLC;

		DLB2x[iDL+1].BLC += DL;

		return 1;

	}
}


//  ɾ��ĳͳһ��̸�����һ����������
Adesk::Boolean  GTZX2_JYX_ROAD::DelDLM(double tylc)
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

Acad::ErrorStatus GTZX2_JYX_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
												   AcDbIntArray& osnapModes,
												   AcDbIntArray& geomIds)const
{
	assertReadEnabled();	
	int i,j,k;
	AcGePoint3d Pt;
	xlpoint  PB,PE,PZ;
	//	double X1,Y1;
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


	for(i=0;i<MidPointArray.GetSize();i++)
		gripPoints.append(MidPointArray[i]);

	gripPoints.append(m_XLNameNotePt);
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

Acad::ErrorStatus GTZX2_JYX_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices, 
													  const AcGeVector3d& offset)

{
	assertWriteEnabled();


	GetT0min_Lrmin();
	CalJDdata1();
	SetJDdata();
	getpxy_DATA();
	CalNYDL();  
	GetDLTYLC();

	int k,j;
	//	double fb,fb1,fb2;

	int bz ;
	double R, dd,DD01,DD02,DD1,DD2,fb,d1,d2,fb1,fb2, dx,dy;
	CString  TempString;
	dx=offset[X];dy=offset[Y];	
	for ( j=0;j<indices.length();j++)//����
	{	
		k=indices[j];       
		// ������� 0~NJD-1
		if (k<NJD&&k>0) 
		{   // ��ѡ��ļе�Ϊ ĳһ���㣨���������յ㣩

			if( (JD2[k].JDXZ==R_X||JD2[k].JDXZ==B_XNOTP_X) && (JD2[k-1].JDXZ==R_X ||JD2[k-1].JDXZ==R_XStart||JD2[k-1].JDXZ==B_XNOTP_X) && (JD2[k+1].JDXZ==R_X||JD2[k+1].JDXZ==R_XEnd||JD2[k+1].JDXZ==B_XNOTP_X) )
			{

				bz=1;
				k=indices[j];  //�������
				dx=offset[X];dy=offset[Y];

				if (JDTDBZ==1)  //�������϶�
				{
					if (k<=NJD-2)
					{ 
						DD02 =xyddaa(JD2[k].x,JD2[k].y, JD2[k+1].x,JD2[k+1].y,&fb2);
						DD2=xyddaa(JD2[k].x+dy,JD2[k].y+dx, JD2[k+1].x,JD2[k+1].y,&fb);
						//  ��ֱ�߶�(�������)[(XB,YB),(XE,YE)]�ĳ��Ⱥͷ�λ��  ����:���룬 *��λ�� 
						d2=DD02-DD2;
					} 
					else {  d2=0.0; fb2=0.0; }
					if (k>0)
					{
						DD01 =xyddaa(JD2[k-1].x,JD2[k-1].y, JD2[k].x,JD2[k].y,&fb1);
						DD1=xyddaa(JD2[k-1].x,JD2[k-1].y,JD2[k].x+dy,JD2[k].y+dx,&fb);
						//  ��ֱ�߶�(�������)[(XB,YB),(XE,YE)]�ĳ��Ⱥͷ�λ��  ����:���룬 *��λ�� 
						d1=DD1-DD01;
					}else {  d1=0.0; fb1=0.0; }
					dd=d1; fb=fb1;
					if (fabs(d1)<fabs(d2)) { dd=d2; fb=fb2;};
					//E
					JD2[k].y=JD2[k].y+dd*sin(fb);
					//N
					JD2[k].x=JD2[k].x+dd*cos(fb);
					//	ads_printf(L"  DT=%-10.4f",dd);
				}
				else if(JDTDBZ==2 || JDTDBZ==3 || JDTDBZ==4)
				{
					double OutN,OutE;
					OutN=JD2[k].x+offset[Y];
					OutE=JD2[k].y+offset[X];

					if (JDTDBZ==2 || JDTDBZ==4)  // ��ǰԲ
					{

						if(k>1)//�������
						{
							double Lo1,Lo2,R1,p1,tR1,A2;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

							Lo1=JD2[k-1].Lo1;
							Lo2=JD2[k-1].Lo2;
							R1=JD2[k-1].R;
							if(Lo2>0.0)
							{
								A2=sqrt(R1*Lo2);
								double x=spiral_y(A2,Lo2);
								double t=Lo2/(2.0*R1);
								double dr=x+R1*sin(t+m_dHalfPI);			
								p1=dr-R1;
							}
							else p1=0.0;					
							//�뾶+������					
							tR1=R1+p1;
							//��ǰԲԲ��
							double CenN,CenE;
							double N1,E1,N0,E0,N2,E2;
							N1=JD2[k-2].x;
							E1=JD2[k-2].y;
							N0=JD2[k-1].x;
							E0=JD2[k-1].y;
							N2=JD2[k].x;
							E2=JD2[k].y;
							GetCenter3JD(N1,E1,N0,E0,N2,E2,R1,Lo1,Lo2,CenN,CenE);
							double TN,TE;//�е�
							//���е�
							int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
							GetQD(CenN,CenE,tR1,OutN,OutE,LRFlag,TN,TE);
							BAS_DRAW_FUN_JYX::INTERS(&JD2[k-1].x,&JD2[k-1].y,N1,E1,N0,E0,TN,TE,OutN,OutE);
						}				
					}
					if (JDTDBZ==3 || JDTDBZ==4)  // �к�Բ
					{
						if(k<NJD-2)//�����յ�
						{
							double Lo1,Lo2,R2,p2,tR2,A1;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

							Lo1=JD2[k+1].Lo1;
							Lo2=JD2[k+1].Lo2;
							R2=JD2[k+1].R;
							if(Lo1>0.0)
							{
								A1=sqrt(R2*Lo1);
								double x=spiral_y(A1,Lo1);
								double t=Lo1/(2.0*R2);
								double dr=x+R2*sin(t+m_dHalfPI);			
								p2=dr-R2;
							}
							else p2=0.0;					
							//�뾶+������					
							tR2=R2+p2;
							//��ǰԲԲ��
							double CenN,CenE;
							double N1,E1,N0,E0,N2,E2;
							N1=JD2[k].x;
							E1=JD2[k].y;
							N0=JD2[k+1].x;
							E0=JD2[k+1].y;
							N2=JD2[k+2].x;
							E2=JD2[k+2].y;
							GetCenter3JD(N1,E1,N0,E0,N2,E2,R2,Lo1,Lo2,CenN,CenE);
							//acutPrintf(L"CenE:%lf,%lf\n",CenE,CenN);
							double TN,TE;//�е�
							//���е�
							int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
							GetQD(CenN,CenE,tR2,OutN,OutE,-1*LRFlag,TN,TE);
							BAS_DRAW_FUN_JYX::INTERS(&JD2[k+1].x,&JD2[k+1].y,N2,E2,N0,E0,TN,TE,OutN,OutE);
						}				
					}
					//(JDTDBZ==4)// ��ǰ������Բ
					//��ǰ������λ��
					JD2[k].y+=offset[X];//x					
					JD2[k].x+=offset[Y];//y
				}
				else   //�����϶�
				{  //x
					JD2[k].y=JD2[k].y+offset[X];
					//y
					JD2[k].x=JD2[k].x+offset[Y];


				}



			}
			else if(JD2[k].JDXZ==S_StartPoint 
				|| (JD2[k].JDXZ==R_XStart)  //?????*****//
				|| (JD2[k].JDXZ==B_XNOTP_X && JD2[k-1].JDXZ!=B_XNOTP_X) )
			{
				double OriN,OriE;
				double x1,y1,x2,y2;
				OriN = JD2[k].x;
				OriE = JD2[k].y;
				x1=JD2[k-1].y;	y1=JD2[k-1].x;
				x2=JD2[k].y;	y2=JD2[k].x;
				double f1=fwj(x1,y1,x2,y2);
				double f2=fwj(x1,y1,x2+dx,y2+dy);
				JD2[k].y+=dx;
				JD2[k].x+=dy;
				double dd=sqrt((JD2[k].x-JD2[k-1].x)*(JD2[k].x-JD2[k-1].x)
					+ (JD2[k].y-JD2[k-1].y)*(JD2[k].y-JD2[k-1].y) );
				ads_printf(L"\n");
				dd*=cos(f2-f1);

				JD2[k].y=JD2[k-1].y+dd*sin(f1);
				JD2[k].x=JD2[k-1].x+dd*cos(f1);

				//////////////////////////////////////////////////////////////////////////
				if (JDTDBZ==3 || JDTDBZ==4)  // �к�Բ
				{
					if(k<NJD-2)//�����յ�
					{
						double Lo1,Lo2,R2,p2,tR2,A1;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

						Lo1=JD2[k+1].Lo1;
						Lo2=JD2[k+1].Lo2;
						R2=JD2[k+1].R;
						if(Lo1>0.0)
						{
							A1=sqrt(R2*Lo1);
							double x=spiral_y(A1,Lo1);
							double t=Lo1/(2.0*R2);
							double dr=x+R2*sin(t+m_dHalfPI);			
							p2=dr-R2;
						}
						else p2=0.0;					
						//�뾶+������					
						tR2=R2+p2;
						//��ǰԲԲ��
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						double OutN,OutE;
						OutN=JD2[k].x;
						OutE=JD2[k].y;
						N1=OriN;
						E1=OriE;
						N0=JD2[k+1].x;
						E0=JD2[k+1].y;
						N2=JD2[k+2].x;
						E2=JD2[k+2].y;
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R2,Lo1,Lo2,CenN,CenE);
						//acutPrintf(L"CenE:%lf,%lf\n",CenE,CenN);
						double TN,TE;//�е�
						//���е�
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR2,OutN,OutE,-1*LRFlag,TN,TE);
						BAS_DRAW_FUN_JYX::INTERS(&JD2[k+1].x,&JD2[k+1].y,N2,E2,N0,E0,TN,TE,OutN,OutE);
					}				
				}
				//////////////////////////////////////////////////////////////////////////

			}
			else if(JD2[k].JDXZ==S_EndPoint
				|| (JD2[k].JDXZ==R_XEnd )       //?????*****//
				|| (JD2[k].JDXZ==B_XNOTP_X && JD2[k+1].JDXZ!=B_XNOTP_X) )
			{
				double OriN,OriE;
				OriN = JD2[k].x;
				OriE = JD2[k].y;
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


					//	double xjj2=JD2[k+1].xjj/fabs(JD2[k+1].xjj)*sqrt((JD2[k].x-Pt.x)*(JD2[k].x-Pt.x)+(JD2[k].y-Pt.y)*(JD2[k].y-Pt.y));


					double xjj2;
					int LorR;
					double dist;
					LorR = JudgePTLorR(JD2[k].x,JD2[k].y,dist,array,tarray);
					xjj2= LorR * dist;


					JD2[k].xjj=xjj2;//S��ڶ�������
					CalOneBXJDdata(k+1);//������ڲ��н���					
				}

				//////////////////////////////////////////////////////////////////////////
				if (JDTDBZ==2 || JDTDBZ==4)  // ��ǰԲ
				{

					if(k>1)//�������
					{
						double Lo1,Lo2,R1,p1,tR1,A2;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

						Lo1=JD2[k-1].Lo1;
						Lo2=JD2[k-1].Lo2;
						R1=JD2[k-1].R;
						if(Lo2>0.0)
						{
							A2=sqrt(R1*Lo2);
							double x=spiral_y(A2,Lo2);
							double t=Lo2/(2.0*R1);
							double dr=x+R1*sin(t+m_dHalfPI);			
							p1=dr-R1;
						}
						else p1=0.0;					
						//�뾶+������					
						tR1=R1+p1;
						//��ǰԲԲ��
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						double OutN,OutE;
						OutN=JD2[k].x;
						OutE=JD2[k].y;
						N1=JD2[k-2].x;
						E1=JD2[k-2].y;
						N0=JD2[k-1].x;
						E0=JD2[k-1].y;
						N2=OriN;
						E2=OriE;
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R1,Lo1,Lo2,CenN,CenE);
						double TN,TE;//�е�
						//���е�
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR1,OutN,OutE,LRFlag,TN,TE);
						BAS_DRAW_FUN_JYX::INTERS(&JD2[k-1].x,&JD2[k-1].y,N1,E1,N0,E0,TN,TE,OutN,OutE);
					}				
				}
				//////////////////////////////////////////////////////////////////////////
			}
			if(JD2[k].JDXZ==R_X || JD2[k].JDXZ==R_XStart || JD2[k].JDXZ==R_XEnd)//��Ӱ�쵽����
			{
				//�ý����Ӧ��һ�����
				int RxdNum=GetRXDNum();
				int rxdno,FirstDL,EndDL,sjd,ejd;
				double lc2x=QXB[k].KYH;//Բ�������

				for(int i=0;i<RxdNum;i++)
				{
					GetJDXHofiRXD(i,sjd,ejd);
					if (k>=sjd && k<=ejd)
					{
						rxdno=i;
						break;
					}
				}

				CalJDdata1();
				SetJDdata();
				getpxy_DATA();
				GetiRXDDLinfo(rxdno,FirstDL,EndDL);
				if(DLB2x[EndDL].TYLC<QXB[ejd].KHZ)//���ж��յ����
				{//������°���
					//plh05.19
					GTZX_JYX_ROAD *pGtzxTmp = NULL ;

					if(m_pm2==NULL)
					{
						pGtzxTmp = m_pm1 ;
					}
					else pGtzxTmp = m_pm2->m_pm1 ;
					xlpoint PZ;
					ACHAR GH[20],ckml[80];
					double resPt[2];
					double cml1,cml2,dml,cmlHundred1,cmlHundred2;
					double xjj=0.0;
					PZ.lc = QXB[ejd].KHZ;
					xlpoint_pz(array,tarray,&PZ);
					cml1 = PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					cmlHundred1 = pGtzxTmp->LocateHundredMeterOnLine(cml1,false);//��ǰ����//һ�������
					CalXjj(cmlHundred1,pGtzxTmp->array,pGtzxTmp->tarray,array,tarray,xjj,cmlHundred2,resPt);//plh05.06

					dml=pGtzxTmp->XLC1(pGtzxTmp->CalnewcmlTocml(cmlHundred1),GH,pGtzxTmp->NDL,pGtzxTmp->DLB);//һ�����ֳ����
					DLB2x[EndDL].BLC = DLB2x[EndDL].BLC+cmlHundred2-DLB2x[EndDL].TYLC;
					DLB2x[EndDL].ELC = dml;
					DLB2x[EndDL].EGH = GH;
					DLB2x[EndDL].TYLC = cmlHundred2;
				}

				if(DLB2x[EndDL].TYLC<QXB[ejd].KHZ)//���ж��յ����
				{//������°���
					//plh05.19
					GTZX_JYX_ROAD *pGtzxTmp = NULL ;

					if(m_pm2==NULL)
					{
						pGtzxTmp = m_pm1 ;
					}
					else pGtzxTmp = m_pm2->m_pm1 ;
					xlpoint PZ;
					ACHAR GH[20],ckml[80];
					double resPt[2];
					double cml1,cml2,dml,cmlHundred1,cmlHundred2;
					double xjj=0.0;
					PZ.lc = QXB[ejd].KHZ;
					xlpoint_pz(array,tarray,&PZ);
					cml1 = PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					cmlHundred1 = pGtzxTmp->LocateHundredMeterOnLine(cml1,false);//��ǰ����//һ�������
					CalXjj(cmlHundred1,pGtzxTmp->array,pGtzxTmp->tarray,array,tarray,xjj,cmlHundred2,resPt);//plh05.06

					dml=pGtzxTmp->XLC1(pGtzxTmp->CalnewcmlTocml(cmlHundred1),GH,pGtzxTmp->NDL,pGtzxTmp->DLB);//һ�����ֳ����
					DLB2x[EndDL].BLC = DLB2x[EndDL].BLC+cmlHundred2-DLB2x[EndDL].TYLC;
					DLB2x[EndDL].ELC = dml;
					DLB2x[EndDL].EGH = GH;
					DLB2x[EndDL].TYLC = cmlHundred2;
				}

				if(DLB2x[FirstDL].TYLC>QXB[sjd].KZH)//���ж�������
				{//������°���
					//plh05.19
					GTZX_JYX_ROAD *pGtzxTmp = NULL ;

					if(m_pm2==NULL)
					{
						pGtzxTmp = m_pm1 ;
					}
					else pGtzxTmp = m_pm2->m_pm1 ;
					xlpoint PZ;
					ACHAR GH[20],ckml[80];
					double resPt[2];
					double cml1,cml2,dml,cmlHundred1,cmlHundred2;
					double xjj=0.0;
					PZ.lc = QXB[sjd].KZH;
					xlpoint_pz(array,tarray,&PZ);
					cml1 = PROJ_ML(pGtzxTmp->array,pGtzxTmp->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					cmlHundred1 = pGtzxTmp->LocateHundredMeterOnLine(cml1,true);//��ǰ����//һ�������
					CalXjj(cmlHundred1,pGtzxTmp->array,pGtzxTmp->tarray,array,tarray,xjj,cmlHundred2,resPt);//plh05.06

					dml=pGtzxTmp->XLC1(pGtzxTmp->CalnewcmlTocml(cmlHundred1),GH,pGtzxTmp->NDL,pGtzxTmp->DLB);//һ�����ֳ����
					DLB2x[FirstDL].BLC = dml;
					DLB2x[FirstDL].BGH = GH;
					DLB2x[FirstDL].ELC = dml;
					DLB2x[FirstDL].TYLC = cmlHundred2;
					CString StrXLName ;
					StrXLName = xlname ;

					if(	StrXLName.Find(L"��ƻ���") >= 0 )
						DLB2x[FirstDL].RXDName.Format(L"���иĽ�%0.1lf" , DLB2x[FirstDL].ELC / 1000.0) ;
					else
						DLB2x[FirstDL].RXDName.Format(L"���ж�%0.1lf" , DLB2x[FirstDL].ELC / 1000.0) ;
				}
			}
		}
		else if(k>=NJD&&k<=2*NJD-3)//Բ���е�
		{
			k=indices[j]-(NJD-1); // ��԰����Ӧ�Ľ����
			if(JD2[k].JDXZ==R_X&&JD2[k-1].JDXZ==R_X&&JD2[k+1].JDXZ==R_X)  //?????*****//
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
					calptan(JD2[k-1].x,JD2[k-1].y,JD2[k].x,JD2[k].y,JD2[k+1].x,JD2[k+1].y,xt,yt,&tmp,&R,&tmp,&tmp,&tmp,&tmp,&tmp,&tmp);
					R = get_R(R,DV,!isGJD,m_GuiFanCS);
					JD2[k].R = R;
					JD2[k].Lo1 = get_l0(R,DV,!isGJD,m_GuiFanCS);
					JD2[k].Lo2 = get_l0(R,DV,!isGJD,m_GuiFanCS);
				}
			}

			/*
			//����Ի������,ˢ�¶Ի�����,
			if(pEditGJDInforDlg)
			{
			//	ads_printf(L"F2\n");
			pEditGJDInforDlg->GJDInforArray.RemoveAll();
			//���Ի������鸳ֵ
			for(int i=0;i<GJDInforArray.GetSize();i++)
			{
			pEditGJDInforDlg->GJDInforArray.Add(GJDInforArray[i]);
			}

			//ˢ�º�����
			//	pEditGJDInforDlg->WriteGridCtrl();

			if(kk==0)pEditGJDInforDlg->ReWriteNEPoint(kk,PZtmp.x,PZtmp.y,true);
			else pEditGJDInforDlg->ReWriteNEPoint(kk,PZtmp.x,PZtmp.y,false);

			}

			*/

			/*
			else if(k>=2*NJD-2&&k<=3*NJD-4)//���е�
			{
			k=indices[j]-(2*NJD-3); // �����߱߶�Ӧ�Ľ����(��㣩
			dd=sqrt(offset[X]*offset[X]+offset[Y]*offset[Y]);					   

			if (k<=NJD-2||k>0)
			{
			double DD01 =xyddaa(JDarray[k-1][1],JDarray[k-1][2], JDarray[k ][1],JDarray[k][2],&fb1);
			double DD02 =xyddaa(JDarray[k][1],JDarray[k][2], JDarray[k+1][1],JDarray[k+1][2],&fb2);
			fb=fb2;	
			JD2[k].y+=dd*sin(fb);
			//N
			JD2[k].x+=dd*cos(fb);

			JD2[k-1].y+=-dd*sin(fb1);
			//N
			JD2[k-1].x+=-dd*cos(fb1);	    
			}
			}*/

		}
		else if (indices[j]==2*NJD-2+MidPointArray.GetSize())
		{
			m_XLNameNotePt.x += offset.x;
			m_XLNameNotePt.y += offset.y;
		}
		else //�Ľ�����Ϣ����
		{
			k=indices[j]-2*NJD + 2 ;	

			MidPointArray[k].x += offset[X];//x
			MidPointArray[k].y += offset[Y];//y

			PZtmp.lc = PROJ_ML(array,tarray,MidPointArray[k].y,MidPointArray[k].x,MidPointArray[k].y,MidPointArray[k].x);
			xlpoint_pz(array,tarray,&PZtmp);
			int kk = 0;
			kk = int(k/2.0);

			BOOL isStartPt = FALSE ;
			if(fabs(2.0*kk-k)<0.001) isStartPt = TRUE;
			else isStartPt = FALSE ;

			if( isStartPt )
			{//���
				GJDInforArray[kk].spt[0] = PZtmp.x ;
				GJDInforArray[kk].spt[1] = PZtmp.y ;

			}
			else
			{//�յ�
				GJDInforArray[kk].ept[0] = PZtmp.x ;
				GJDInforArray[kk].ept[1] = PZtmp.y ;
			}


		}
	}
	return Acad::eOk;
}

bool GTZX2_JYX_ROAD::AddS(double N1, double E1, double N2, double E2,double R1,double Lo1,double R2,double Lo2)
	//����һ���ԣ�N1��E1������N2��E2��Ϊ�����S��
{
	assertWriteEnabled();
	int i;
	int PreJD=GetPreJD(N1,E1);

	if(JD2[PreJD].JDXZ==R_X||JD2[PreJD+1].JDXZ==R_X||
		JD2[PreJD].JDXZ==R_XStart||JD2[PreJD+1].JDXZ==R_XStart||
		JD2[PreJD].JDXZ==R_XEnd||JD2[PreJD+1].JDXZ==R_XEnd)
		return false;//ֻ�ڽ��жμ�S��    //?????*****//
	else if( JD2[PreJD+1].JDXZ==R_X_OneR ) return false;

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


	//S���һ��������һ���ϵ�ͶӰͳһ���
	double tylc1=PROJ_ML(m_pm1->array,m_pm1->tarray,N1,E1,N1,E1);

	ACHAR GH[20];
	//S���һ��������һ���ϵ�ͶӰ�ֳ����
	double lc1x1jd=m_pm1->XLC(tylc1,GH,m_pm1->NDL);
	_tcscpy(JD2[i].ZHLC,m_pm1->LCchr(GH,lc1x1jd,3,1));
	//_tcscpy(GH);
	//  ACHAR temp[80];
	//  _stprintf(temp,L"%lf",lc);
	//str*/
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

	double xjj2;
	int LorR;
	double dist;
	LorR = JudgePTLorR(N2,E2, dist,array,tarray);
	xjj2= LorR * dist;

	JD2[i].xjj=xjj2;//S��ڶ�������

	//	ads_printf(L"xjj=%lf \n",xjj2);

	CalOneBXJDdata(i+1);//������ڲ��н���


	/*	SetJDdata();
	getpxy_DATA();
	CalNYDL(); 
	GetDLTYLC();//���¼��������ͳһ���*/

	return true;
}

void GTZX2_JYX_ROAD::CalOneBXJDdata(int iJD)
	//����ĳ���н���
{
	//	assertWriteEnabled();
	double lo,lo1,D;
	int i=iJD;
	if(JD2[i].JDXZ==B_X)//���жν���
	{
		if(JD2[i-1].JDXZ==S_EndPoint)
			JD2[i].jdno1=JD2[i-3].jdno1+1;//��һ�����н����Ӧ��һ�߽����

		//	   acutPrintf(L"B_X:%d\n",i);

		_tcscpy(JDarray[i].JDNum,JD2[i].JDNum);
		//  JDarray[i][0] = JD0 + i;
		int jdn1 = JD2[i].jdno1;//��Ӧһ�߽������
		if(i==NJD-1)
			JD2[i].xjj = JD2[i-1].xjj;//���һ������߼�����ǰһ�����
		double xjj = JD2[i].xjj;
		double EXJD[2];

		CalEXJD(jdn1,i,EXJD);
		JD2[i].x = EXJD[0];
		JD2[i].y = EXJD[1];//��������߽���
		double R = m_pm1->JDarray[jdn1].R;
		double JK;
		JK = get_XJJK(m_pm1->DV,R,isJYGuiFanCS,m_pm1->m_GuiFanCS);//�߼��ӿ�
		JD2[i].qxjk = JK;
		JD2[i].Lo1 = m_pm1->JDarray[jdn1].l1;
		JD2[i].R = R;
		JD2[i].Lo2 = m_pm1->JDarray[jdn1].l2;           
		lo1 = m_pm1->JDarray[jdn1].l1;//һ�߻���
		D = fabs(xjj);
		//���߰뾶
		if(i>0&&i<NJD-1)
		{
			double xjj2;
			xjj2 = JD2[i-1].xjj;//��һ�����߼��
			//   ads_printf(L"i=%d xjj=%lf xjj2=%lf\n",i,xjj,xjj2);
			if(fabs(xjj-xjj2)<0.01)//ǰ���߼�����
			{
				////xjj�������߼ӿ�............		   
				//if(xjj>0)//һ��Ϊ����
				//{
				//	if(m_pm1->QXB[jdn1].LorR<0)//��ת,II������࣬���ٻ���
				//	{
				//		//���仺��
				//		lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//���ȱ� 2003.12.4�ṩ
				//		JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
				//		D  = -1.0*D;
				//	}
				//	else
				//	{
				//		//���仺��
				//		lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
				//		JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
				//	}
				//}
				//else//һ��Ϊ����
				//{
				//	if(m_pm1->QXB[jdn1].LorR<0)//��ת,II�����ڲ�
				//	{
				//		lo = ((int)(sqrt((R-D)*(lo1*lo1/R+24*JK))/10-0.000001)+1)*10;//�� 2003.12.4�ṩ
				//		JD2[i].Lo1 = JD2[i].Lo2 =  lo;						  					   
				//	}
				//	else
				//	{   //���仺��
				//		lo = ((int)(sqrt((R+D)*(lo1*lo1/R-24*JK))/10))*10;//�� 2003.12.4�ṩ
				//		JD2[i].Lo1 = JD2[i].Lo2 =  lo;	
				//		D  = -1.0*D;
				//	}
				//}
				////��뾶
				//double B = R - D + (lo1*lo1)/24/R;
				//JD2[i].R = 0.5*(B + sqrt(B*B - lo*lo/6));//���߰뾶

				CalTXY(m_pm1->QXB[jdn1].LorR * JD2[i].xjj,JD2[i].qxjk,R,lo1,JD2[i].R,lo);
				JD2[i].Lo2 = JD2[i].Lo1 = lo;
			}
		}   
		JD2[i].jdno = JD0 + i;
	}//����
}

void GTZX2_JYX_ROAD::ModifyOneSWJDdata(int iJD,double N,double E,double XJJ=0.0,double JIAO=0.0)
	//�޸�S���һ�����������
{
	//	AfxMessageBox(L"ModifyOneSWJDdatain");
	assertWriteEnabled();	


	if(JD2[iJD].JDXZ==S_EndPoint)//
	{
		//	AfxMessageBox(L"ModifyOneSWJDdata1");		
		int iJD1,iJD2;//��Ӧ��һ�߱�
		double N1,E1,N2,E2;

		iJD1=JD2[iJD-2].jdno1;
		iJD2=JD2[iJD+1].jdno1;

		N1=m_pm1->JDarray[iJD1].N;
		E1=m_pm1->JDarray[iJD1].E;
		N2=m_pm1->JDarray[iJD2].N;
		E2=m_pm1->JDarray[iJD2].E;		
		double tmp1,tmp2;
		tmp1 = GetSide(N1,E1,N2,E2,JD2[iJD].x,JD2[iJD].y);
		tmp2 = GetSide(N1,E1,N2,E2,N,E);		
		if(tmp1*tmp2<0)return;	 
		//		if(GetSide(N1,E1,N2,E2,JD2[iJD].x,JD2[iJD].y)*GetSide(N1,E1,N2,E2,N,E)<0)return;	
		//		if(GetSide(N3,E3,N2,E2,JD2[iJD].x,JD2[iJD].y)*GetSide(N3,E3,N2,E2,N,E)<0)return;		
		double tylc=PROJ_ML(m_pm1->array,m_pm1->tarray,N,E,N,E);

		xlpoint Pt;//��һ������ͶӰ�������ϵĵ�
		Pt.lc=tylc;
		xlpoint_pz(m_pm1->array,m_pm1->tarray,&Pt);
		if(fabs(XJJ)<=0.0001)//û�������߼��ʱ��������
		{
			JD2[iJD].x=N;
			JD2[iJD].y=E;

			double xjj2;
			//�б�N2,E2����·����Ҳ�
			int LorR;
			double dist;
			LorR = JudgePTLorR(N,E, dist,array,tarray);
			xjj2= LorR * dist;

			//	double xjj2=JD2[iJD+1].xjj/fabs(JD2[iJD+1].xjj)*sqrt((N-Pt.x)*(N-Pt.x)+(E-Pt.y)*(E-Pt.y));
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

		/*
		SetJDdata();
		getpxy_DATA();
		CalNYDL();  
		GetDLTYLC();//���¼��������ͳһ���
		*/
	}

}

double GTZX2_JYX_ROAD::DistToJD(double N, double E, int iJD)
{
	assertReadEnabled();
	return sqrt((N-JD2[iJD].x)*(N-JD2[iJD].x)+(E-JD2[iJD].y)*(E-JD2[iJD].y));
}

void GTZX2_JYX_ROAD::DelS(double N, double E)
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
	NJD-=2;
	CalOneBXJDdata(S_JD1);//������ڲ��н���

	/*
	SetJDdata();
	getpxy_DATA();
	CalNYDL(); 
	GetDLTYLC();//���¼��������ͳһ���
	*/
}

//////////////////////////////////////////////////////////////////////////
//ѡ��S��
bool GTZX2_JYX_ROAD::SelSModel(double N, double E,int& SJD,int& S_JD1,int& S_JD2)
	//ɾ��ָ���㴦��S��
{
	S_JD1 = 0;
	S_JD2 = 0;
	int iJD=GetPreJD(N,E);
	SJD = iJD-1;
	S_JD1 = iJD;
	S_JD2 = iJD+1;
	ads_printf(L"%lf,%lf\n",QXB[S_JD1].a,QXB[S_JD2].a);
	if (fabs(QXB[S_JD1].LorR*QXB[S_JD1].a + QXB[S_JD2].LorR*QXB[S_JD2].a)<0.00001)//ѡ����S��
	{
		return true;
	}
	else
	{
		AfxMessageBox(L"��ѡ��ΪS�䣬������ѡ��!\n");
		return false;
	}
}



void GTZX2_JYX_ROAD::SetDLB(EXDLB *Dlb, int ndl)
{
	assertWriteEnabled();
	NDL=ndl;

	for(int i=0;i<ndl;i++)DLB2x[i]=Dlb[i];
	//	Create2XDLB();
	GetDLTYLC();
	Create2XGTDLB();
}

void GTZX2_JYX_ROAD::SetBZLCArray( CArray<BAS_DRAW_FUN_JYX::BZLCStructTable , BAS_DRAW_FUN_JYX::BZLCStructTable> & BZLCArrayToIn )
{
	assertWriteEnabled();
	for(int i=0;i<BZLCArrayToIn.GetSize();i++)
		BZLCarray.Add(BZLCArrayToIn[i]);
	NBZLC = BZLCarray.GetSize();
}


void GTZX2_JYX_ROAD::SetBZLCArray( BZLCStructTable *BZLCTableToIn , int AddBZLCNumToIn )
{
	assertWriteEnabled();
	for(int i=0;i<AddBZLCNumToIn;i++)
		BZLCarray.Add(BZLCTableToIn[i]);
	NBZLC = BZLCarray.GetSize();
}

void GTZX2_JYX_ROAD::GetDLTYLC()
	//����ÿ����������ͳһ��̣����������ϵͳ����
{
	ACHAR xlc[80],ch[80];
	double tylc1x,tylc2x;  //,dlc;

	GTZX_JYX_ROAD *piRXDGTZX = NULL;

	for(int i=0;i<NDL;i++)
	{
		if(i==0||(i>0&&DLB2x[i].no!=DLB2x[i-1].no))
		{
			_tcscpy(xlc,DLB2x[i].BGH);
			_stprintf(ch,L"%lf",DLB2x[i].BLC);
			_tcscat(xlc,ch);

			double resPt[2],xjj;
			xlpoint PZ;
			//�������жε�1��������ͳһ���
			if(m_pm2==NULL)
			{
				if(m_pm1->XLShuXing!="��ͨ��ƻ���")
				{
					//����
					tylc1x=m_pm1->TYLC1(xlc,m_pm1->NDL,m_pm1->DLB);//������ж�����Ӧ��һ��ͳһ���
					tylc1x = m_pm1->CalcmlTonewcml(tylc1x);
					CalXjj(tylc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,DLB2x[i].TYLC,resPt);
				}
				else
				{
					//��ͨ��ƻ���
					tylc1x=m_pm1->TYLC1(xlc,m_pm1->NDL,m_pm1->DLB);//������ж�����Ӧ��һ��ͳһ���
					PZ.lc = tylc1x;
					m_pm1->xlpoint_pz_GTX(m_pm1->array,m_pm1->tarray,&PZ);
					DLB2x[i].TYLC = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
				}
			}
			else//���ݶ��߹������Ķ���
			{
				tylc1x=m_pm1->TYLC1(xlc,m_pm2->m_pm1->NDL,m_pm2->m_pm1->DLB);//������ж�����Ӧ��һ��ͳһ���
				tylc1x = m_pm2->m_pm1->CalcmlTonewcml(tylc1x);
				CalXjj(tylc1x,m_pm2->m_pm1->array,m_pm2->m_pm1->tarray,array,tarray,xjj,DLB2x[i].TYLC,resPt);
			}


			int iDL = i;
			do 
			{
				iDL++;
			}while(iDL<NDL-1&&DLB2x[iDL+1].no==DLB2x[iDL].no);

			_tcscpy(xlc,DLB2x[iDL].EGH);
			_stprintf(ch,L"%lf",DLB2x[iDL].ELC);
			_tcscat(xlc,ch);

			if(m_pm2==NULL)
			{
				if(m_pm1->XLShuXing!="��ͨ��ƻ���")
				{
					//����
					tylc1x=m_pm1->TYLC1(xlc,m_pm1->NDL,m_pm1->DLB);//������ж��յ��Ӧ��һ��ͳһ���
					tylc1x = m_pm1->CalcmlTonewcml(tylc1x);
					CalXjj(tylc1x,m_pm1->array,m_pm1->tarray,array,tarray,xjj,tylc2x,resPt);
				}
				else
				{
					//��ͨ��ƻ���
					tylc1x=m_pm1->TYLC1(xlc,m_pm1->NDL,m_pm1->DLB);//������ж�����Ӧ��һ��ͳһ���
					PZ.lc = tylc1x;
					m_pm1->xlpoint_pz_GTX(m_pm1->array,m_pm1->tarray,&PZ);
					tylc2x = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
				}

			}
			else//���ݶ��߹������Ķ���
			{
				tylc1x=TYLC1(xlc,m_pm2->m_pm1->NDL,m_pm2->m_pm1->DLB);//������ж�����Ӧ��һ��ͳһ���
				tylc1x = m_pm2->m_pm1->CalcmlTonewcml(tylc1x);
				CalXjj(tylc1x,m_pm2->m_pm1->array,m_pm2->m_pm1->tarray,array,tarray,xjj,tylc2x,resPt);
			}

			DLB2x[iDL].TYLC = tylc2x;//plh05.14

			if (piRXDGTZX)
			{
				delete piRXDGTZX;
				piRXDGTZX = NULL;
			}
			piRXDGTZX = GetiRxdtoGTZXplinp(DLB2x[i].no);

			double dTYLCDL = 0.0;
			do 
			{
				i++;
				if (i != iDL)
				{
					dTYLCDL = piRXDGTZX->CalnewcmlTocml(DLB2x[i - 1].TYLC);
					DLB2x[i].TYLC = dTYLCDL + DLB2x[i].BLC - DLB2x[i-1].ELC;


					DLB2x[i].TYLC = piRXDGTZX->CalcmlTonewcml(DLB2x[i].TYLC);
				}
			}while(i<NDL-1&&DLB2x[i+1].no==DLB2x[i].no);//���һ�����������ж����һ������

			DLB2x[i].BLC = DLB2x[i].TYLC - DLB2x[i-1].TYLC + DLB2x[i-1].ELC;
		}
	}
}

void GTZX2_JYX_ROAD::Insert2xBZLC(int iRXD,CArray<BAS_DRAW_FUN_JYX::BZLCStructTable , BAS_DRAW_FUN_JYX::BZLCStructTable> & BZLCArrayToIn )
{
	DeliRxdBZLCArray(iRXD);
	int BZLCToInNum = BZLCArrayToIn.GetSize();

	if(BZLCToInNum<0)return;

	BZLCarray.Append(BZLCArrayToIn);
}

void GTZX2_JYX_ROAD::DeliRxdBZLCArray(int iRXD)
{
	CString iRxdNameTmp = GetRXDXLNameByRXDIndex(iRXD);

	iRxdNameTmp.Trim();
	CString strTmp ;

	for(int i=BZLCarray.GetSize()-1;i>=0;i--)
	{
		strTmp = BZLCarray[i].GJDNum ;
		strTmp.Trim();
		if(strTmp==iRxdNameTmp)
		{
			BZLCarray.RemoveAt(i);
		}
	}
}


void GTZX2_JYX_ROAD::GetiRxdBZLCArray(int iRXD ,  CArray<BAS_DRAW_FUN_JYX::BZLCStructTable,BAS_DRAW_FUN_JYX::BZLCStructTable> & iRxdBZLCArrayToOut )
{
	CString iRxdNameTmp = GetRXDXLNameByRXDIndex(iRXD);

	iRxdNameTmp.Trim();
	CString strTmp ;
	iRxdBZLCArrayToOut.RemoveAll();

	for(int i=0;i<BZLCarray.GetSize();i++)
	{
		strTmp = BZLCarray[i].GJDNum ;
		strTmp.Trim();
		if(strTmp==iRxdNameTmp)
		{
			iRxdBZLCArrayToOut.Add( BZLCarray[i] );
		}
	}

	if(iRxdBZLCArrayToOut.GetSize()==0)
	{
		BAS_DRAW_FUN_JYX::BZLCStructTable OneBZLCTmp ;

		double jday[MAXJDNUM][6];
		int njd;
		int SDLNum, EDLNum;
		//��ȡjdarray
		GetiRXDJDArray(iRXD,jday,njd);
		GetiRXDDLinfo(iRXD,SDLNum,EDLNum);

		//���
		_tcscpy( OneBZLCTmp.GJDNum ,  iRxdNameTmp ) ;
		OneBZLCTmp.X = jday[0][1] ;
		OneBZLCTmp.Y = jday[0][2] ;
		_tcscpy( OneBZLCTmp.kml ,LCchr(DLB[SDLNum].EGH,DLB[SDLNum].BLC + DLB[SDLNum].DL , 4, 1) );

		iRxdBZLCArrayToOut.Add(OneBZLCTmp);

		//β
		_tcscpy( OneBZLCTmp.GJDNum , iRxdNameTmp );
		OneBZLCTmp.X = jday[njd-1][1] ;
		OneBZLCTmp.Y = jday[njd-1][2] ;
		_tcscpy( OneBZLCTmp.kml ,LCchr(DLB[EDLNum].BGH,DLB[EDLNum].BLC ,4, 1) );
		iRxdBZLCArrayToOut.Add(OneBZLCTmp);

	}

}

void GTZX2_JYX_ROAD::Update2XDLB(int iRXD,EXDLB *ExDlb, int DlNum)
	//����iRXD�Ķ��������������еĶ�������
{

	if(iRXD<0)return;
	//if(iRXD==GetRXDNum()-1)//��β��������ж�
	//{
	//	for(int i=NDL;i<NDL+DlNum;i++)//�滻
	//	{
	//		DLB2x[i]=ExDlb[i-NDL];	
	//	}
	//}
	//else//���м�������ж�
	//{
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
		//�������жζ�������
		int RXDDLSum = EndDlno-FirstDlno+1;
		for (int iDL=FirstDlno; iDL<NDL-RXDDLSum; iDL++)
			DLB2x[iDL] = DLB2x[iDL+RXDDLSum];
		NDL -= RXDDLSum;
		for(i=NDL+DlNum-1;i>=FirstDlno+DlNum;i--)//����ƶ�DlNumλ
		{	
			if(i-DlNum>=0)
			{
				DLB2x[i]=DLB2x[i-DlNum];
				//	DLB2x[i].no+=1;
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
	//}
	NDL+=DlNum;

}

void GTZX2_JYX_ROAD::Insert2XDLB(int iRXD,EXDLB *ExDlb, int DlNum)
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

int GTZX2_JYX_ROAD::GetiRxdNoFromTYLC1x(double lc1x)
{//plh05.19
	//	int iRXD;//Ҫɾ�������ж����
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

int GTZX2_JYX_ROAD::GetiRXDFromNE(double N, double E)
	//�ɾ��ࡢγ�࣬���������ж���ţ�0��ʼ��
{
	//	int iRXD;//Ҫɾ�������ж����
	//	int iJD;//ָ����ͶӰ����·��һλ�õģ�ǰһ��JD
	//	int FirstJD,EndJD;//ĳһ���ж���ʼ�����

	double cmlOn2X,smlOn2X,emlOn2X;

	int njd;
	double jday[MAXJDNUM][6];

	cmlOn2X = PROJ_ML(array,tarray,N,E,N,E);
	int RxdNum = GetRXDNum();
	for(int i=0;i<RxdNum;i++)
	{
		GetiRXDJDArray(i, jday, njd);

		if(njd<1)
			continue;

		smlOn2X = PROJ_ML(array,tarray,jday[0][1],jday[0][2],jday[0][1],jday[0][2]);
		emlOn2X = PROJ_ML(array,tarray,jday[njd-1][1],jday[njd-1][2],jday[njd-1][1],jday[njd-1][2]);


		if(cmlOn2X>smlOn2X-0.001 && cmlOn2X<emlOn2X+0.001)
			return i;

	}

	//FirstJD=EndJD=iJD=-1;


	//iJD=GetPreJD(N,E);
	//if(iJD>=0&&iJD<=NJD-1)
	//{
	//	if(JD2[iJD].JDXZ==R_X)
	//	{
	//		int RxdNum=GetRXDNum();
	//		for(int i=0;i<RxdNum;i++)
	//		{
	//			GetJDXHofiRXD(i,FirstJD,EndJD);


	//			if(iJD>=FirstJD&&iJD<=EndJD)
	//			{
	//				return i;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		//		acutPrintf(L"��ѡ��Ϊ���ж�!\n");
	//	}
	//}
	//else
	//{
	//	acutPrintf(L"iJD=%d %lf %lf\n",iJD,N,E);
	//}
	return -1;
}

void GTZX2_JYX_ROAD::AddDoubleLineJD(DoubleLineData ExJD)//plh
{
	NJD+=1;
	JD2[NJD-1]=ExJD;
}

void GTZX2_JYX_ROAD::ModifyDoubleLineJD(int iJD,DoubleLineData ExJD)//plh
{
	JD2[iJD]=ExJD;
}

//
void GTZX2_JYX_ROAD::GetiRXDJDArray(int iRXD, double jday[][6], int &njd)//plh
{
	int sjd=0,ejd=0,i=0,j=0,sdl=0,edl=0,ndl=0;
	njd=0;
	//	EXDLB *iRxdDlb;
	GetiRXDDLinfo(iRXD,sdl,edl);//��ȡ���ն�����

	xlpoint PZ1,PZ2;	//�������յ�������	
	PZ1.lc=DLB2x[sdl].TYLC;
	PZ2.lc=DLB2x[edl].TYLC;

	xlpoint_pz(array,tarray,&PZ1);//������
	xlpoint_pz(array,tarray,&PZ2);

	GetJDXHofiRXD(iRXD,sjd,ejd);//��ȡ���ս����
	//	acutPrintf(L"sjd=%d,ejd=%d\n",sjd,ejd);

	njd=ejd-sjd+1;//������
	njd+=2;//�����������յ�
	//	jday=new double[njd][6];

	for(i=1;i<njd-1;i++)//1->njd-1
	{
		jday[i][0]=1.0*i;
		//	for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i-1][j];//sjd+i-1,��sjd��ʼ
		jday[i][1]=JDarray[sjd+i-1].N;
		jday[i][2]=JDarray[sjd+i-1].E;
		jday[i][3]=JDarray[sjd+i-1].l1;
		jday[i][4]=JDarray[sjd+i-1].R;
		jday[i][5]=JDarray[sjd+i-1].l2;

	}
	//���ж������Ϊ��һ������
	i=0;
	jday[i][0]=i;
	jday[i][1]=PZ1.x;
	jday[i][2]=PZ1.y;
	jday[i][3]=DLB2x[sdl].ELC;//��ʼ���
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

//��Ƕ��߽����Ƿ�λ�ڶ�����(�Ƿ���ʾ����Ļ��)
void GTZX2_JYX_ROAD::TagEXJDIsShow()
{
	//һ�߶�Ӧ��������뷽����
	//���У������Ӧһ�߽����
	//�������յ㣬�����Ӧһ�߱ߵĺ�һ�������
	//S�����յ㣬�����Ӧһ�߱ߵĺ�һ������ţ���ǰΪǰһ������ţ�Ҫ�ģ���������������������
	//���в�ƽ�����յ㣬�����Ӧһ�߱ߵĺ�һ������ţ�
	//	double PrePt[2],AfterPt[2],resPt[2];
	int iDuanLuo=0;
	DoubleLineData *CurJD; //  *NextJD1;
	DoubleLineData tempJD;	
	//��ǰ���㵽��Ӧ�ߵľ���,��0.5m��ʾ������,���㲻����ʱ������Ϊ��99
	//	double offset;
	//����һ�����Ӧ��ͶӰ��ͶӰ�㵽���������С����
	//	double ProjDist;
	//	double TempX,TempY;
	bool DuanLuoStart=true;//һ���������
	tempJD.R=0.0;
	tempJD.Lo1=0.0;
	tempJD.Lo2=0.0;
	tempJD.xjj=0.0;

	if(NJD<1)return;

	//��ÿ����������б�
	//��㶨Ϊ���߻����ߵ����
	int i=1;
	//	int jd1x;

	for(i=0;i<NJD;i++)
	{
		CurJD=&JD2[i];
		//	if( (CurJD->JDXZ==B_X||CurJD->JDXZ==B_X_Des) && fabs(CurJD->xjj)<0.0001 )CurJD->ISShow=false;// ����ʾ
		//	else 
		//	{
		CurJD->ISShow = true;
		//		break;//����ǰ�治��ʾ�Ľ����ǳ���
		//	}
	}
	//for(;i<NJD;i++)
	//{
	//	CurJD=&JD2[i];//��ǰ���߽���
	//	CurJD->ISShow=true;
	//	//���Ҷ����յ�

	//	bool ifend=false;//�Ƿ�����յ�

	//	if(i==NJD-1) 
	//	{
	//		ifend = true;//�������,���һ����
	//	}
	//	else if((CurJD->JDXZ==B_X||CurJD->JDXZ==B_X_Des) && fabs(CurJD->xjj)<0.0001)//���н��㣬���߼��Ϊ0
	//	{
	//		ifend = true;//�������,���߻���
	//	}
	//	else if(CurJD->JDXZ==S_EndPoint && fabs(CurJD->xjj)<0.0001)//S���յ㣬���߼��Ϊ0
	//	{
	//		ifend = true;//�������,ֱ�߻���
	//	}
	//	else if( (CurJD->JDXZ==R_X||CurJD->JDXZ==B_XNOTP_X)
	//		&& CurJD->jdno1>0 && fabs(CurJD->xjj)<0.0001 )//���ж����һ�����㣬��Ӧһ�߽����������Ƕ�Ӧһ�߱ߵĺ�һ����
	//	{
	//		ifend=true;
	//		if(i>1&&JD2[i-1].ISStartEnd)ifend=false;
	//	}
	//	if(ifend)//�������
	//	{
	//		//������������Ϊ���в����߼��Ϊ0
	//		//			acutPrintf(L"end---------i=%d",i);
	//		for(i++;i<NJD;i++)
	//		{
	//			//				acutPrintf(L"---------i=%d",i);
	//			CurJD=&JD2[i];
	//			if( (CurJD->JDXZ==B_X||CurJD->JDXZ==B_X_Des) && fabs(CurJD->xjj)<0.0001 )CurJD->ISShow=false;// ����ʾ
	//			else
	//			{
	//				CurJD->ISShow=true;
	//				break;
	//			}
	//		}
	//	}
	//}
	//for(i=0;i<NJD;i++)
	//{
	//	if(!JD2[i].ISShow)
	//	{
	//		JD2[i].R=m_pm1->JDarray[JD2[i].jdno1].R;
	//		JD2[i].Lo1=m_pm1->JDarray[JD2[i].jdno1].l1;
	//		JD2[i].Lo2=m_pm1->JDarray[JD2[i].jdno1].l2;
	//	}
	//}
	//	out();
}

void GTZX2_JYX_ROAD::out()
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
void GTZX2_JYX_ROAD::ModifyOriExJDXY()
{
	int i=0;
	double fwj;
	DoubleLineData *CurJD;
	//ǰһ������Ϊ�������ֹ
	for(i=0;i<NJD;i++)
	{
		CurJD=&JD2[i];		

		//		ACHAR str[30];	
		if( CurJD->JDXZ==B_X )
		{//�����������Ϊ����(ȫ�����յ�)

			if(CurJD->jdno1==0)
			{//��ȫ�����							
				xyddaa(m_pm1->JDarray[0].N,m_pm1->JDarray[0].E,
					m_pm1->JDarray[1].N,m_pm1->JDarray[1].E,&fwj);

				double xjj;
				xjj = CurJD->xjj;							
				CurJD->x  =  m_pm1->JDarray[0].N + xjj * cos(fwj+m_dHalfPI);
				CurJD->y  =  m_pm1->JDarray[0].E + xjj * sin(fwj+m_dHalfPI);//��������߽���							
			}
			else if(CurJD->jdno1==m_pm1->NJD-1)
			{//ȫ���յ�
				int jd1x=m_pm1->NJD-2;
				xyddaa(m_pm1->JDarray[jd1x].N,m_pm1->JDarray[jd1x].E,
					m_pm1->JDarray[jd1x+1].N,m_pm1->JDarray[jd1x+1].E,&fwj);
				double xjj;
				xjj = CurJD->xjj;
				CurJD->x  =  m_pm1->JDarray[jd1x+1].N + xjj * cos(fwj+m_dHalfPI);
				CurJD->y  =  m_pm1->JDarray[jd1x+1].E + xjj * sin(fwj+m_dHalfPI);//��������߽���
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
		}
		else  
		{
			//if(CurJD->jdno1 > 0)
			//{
			//	if(fabs(CurJD->x) > 0.1 && fabs(CurJD->y) > 0.1)
			//	{//�����꣬���������ƫ�࣬�������������ȷλ��(��Ӧһ�߱���)
			//		//ModifyNEbyXJJ(CurJD->x,CurJD->y,CurJD->xjj);
			//		//	ModifyNEOnBian(CurJD);
			//	}
			//	else if(FindACHAR(CurJD->JDLC,'+')>=0)
			//	{//�����,���ݽ����Ӧ����̣���ƫ��������
			//		ModifyNEbyXJJ(CurJD->JDLC,CurJD->xjj,CurJD->x,CurJD->y);
			//	}
			//}
			//Zhujiang 20090313-----------------------------------------------------
			if(fabs(CurJD->x) > 0.1 && fabs(CurJD->y) > 0.1)
			{
				Modify_RXD_SE_Coords(JD2,i,CurJD);
			}
			else if(FindACHAR(CurJD->JDLC,'+') >= 0)
			{
				ModifyNEbyXJJ(CurJD->JDLC,CurJD->xjj,CurJD->x,CurJD->y);
			}
			//---------------------------------------------------------------------

		}
	}//for(i=0;i<OriNJD-1;i++)
	//���ݸ�EXJD

}

/*===================================================================

��������:�����������ж����ս��������	
------------------------------------------------------------
����:DoubleLineData p_JD2Arr ���߽�������
int p_iJDIndex          ��ǰ�����ڶ��߽��������е����        
------------------------------------------------------------
���:  DoubleLineData *p_JD 
----------------------------------------------------------------------
�㷨����: ������Ϊ����ʼ�Ľ���Ϊ��,���ж���ʼ�Ĺ��ֱ�߱�ƽ��һ�߼��
���������жεڶ���ֱ�߱��ཻ��
---------------------------------------------------------------------
��ע:	                                 Zhujiang 2009:3:12
=====================================================================*/
void GTZX2_JYX_ROAD::Modify_RXD_SE_Coords(DoubleLineData p_JD2Arr[],int p_iJDIndex,DoubleLineData *p_JD)
{
	if (p_iJDIndex == NJD - 1 || p_iJDIndex == 0)
		return;

	double xjj = 0.0;
	int jd1xNo1,jd1xNo2;
	double fwj;
	double pt1[2],pt2[2];
	double pt3[2],pt4[2];
	double xtmp,ytmp,tylc1x;
	if (p_JD->JDXZ == R_XStart)
	{
		//if (p_JD2Arr[p_iJDIndex - 1].JDXZ == R_XEnd || p_JD2Arr[p_iJDIndex - 1].JDXZ == R_X_OneR)
		//	return;

		xjj = p_JD2Arr[p_iJDIndex - 1].xjj;
		//�������ʼǰ���Ǹ�������߼��û����,����������
		if (fabs(xjj) < 0.1)
			return;

		//��ȡ�����ϵ����������,���ܻ�ȡ����ȷ
		jd1xNo1 = p_JD2Arr[p_iJDIndex - 1].jdno1;
		jd1xNo2 = jd1xNo1 + 1;

		xyddaa(m_pm1->JDarray[jd1xNo1].N,m_pm1->JDarray[jd1xNo1].E,m_pm1->JDarray[jd1xNo2].N,m_pm1->JDarray[jd1xNo2].E,&fwj);

		pt1[0] = m_pm1->JDarray[jd1xNo1].N + xjj * cos(fwj+m_dHalfPI);
		pt1[1] = m_pm1->JDarray[jd1xNo1].E + xjj * sin(fwj+m_dHalfPI);
		pt2[0] = m_pm1->JDarray[jd1xNo2].N + xjj * cos(fwj+m_dHalfPI);
		pt2[1] = m_pm1->JDarray[jd1xNo2].E + xjj * sin(fwj+m_dHalfPI);

		pt3[0] =  p_JD2Arr[p_iJDIndex].x;
		pt3[1] =  p_JD2Arr[p_iJDIndex].y;
		pt4[0] =  p_JD2Arr[p_iJDIndex + 1].x;
		pt4[1] =  p_JD2Arr[p_iJDIndex + 1].y;

		INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);


		//���ڻ����ϵ���������Ż�ȡ����ȷ����1�ַ����������ɹ������õ�2�ַ���
		if (fabs(p_JD->x - xtmp) > 10 || fabs(p_JD->y - ytmp) > 10)
		{
			xtmp = p_JD2Arr[p_iJDIndex].x;
			ytmp = p_JD2Arr[p_iJDIndex].y;

			tylc1x = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,xtmp,ytmp,xtmp,ytmp);

			jd1xNo1 = m_pm1->DistToNearJD(tylc1x,true);
			jd1xNo2 = m_pm1->DistToNearJD(tylc1x,false);

			xyddaa(m_pm1->JDarray[jd1xNo1].N,m_pm1->JDarray[jd1xNo1].E,m_pm1->JDarray[jd1xNo2].N,m_pm1->JDarray[jd1xNo2].E,&fwj);

			pt1[0] = m_pm1->JDarray[jd1xNo1].N + xjj * cos(fwj+m_dHalfPI);
			pt1[1] = m_pm1->JDarray[jd1xNo1].E + xjj * sin(fwj+m_dHalfPI);
			pt2[0] = m_pm1->JDarray[jd1xNo2].N + xjj * cos(fwj+m_dHalfPI);
			pt2[1] = m_pm1->JDarray[jd1xNo2].E + xjj * sin(fwj+m_dHalfPI);

			pt3[0] =  p_JD2Arr[p_iJDIndex].x;
			pt3[1] =  p_JD2Arr[p_iJDIndex].y;
			pt4[0] =  p_JD2Arr[p_iJDIndex + 1].x;
			pt4[1] =  p_JD2Arr[p_iJDIndex + 1].y;

			INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

			//���ַ������������ɹ�,���˳�
			if (fabs(p_JD->x - xtmp) > 10 || fabs(p_JD->y - ytmp) > 10)
				return;
			else
			{
				p_JD->x = xtmp;
				p_JD->y = ytmp;
			}
		}
		else
		{
			p_JD->x = xtmp;
			p_JD->y = ytmp;
		}
	}
	if (p_JD->JDXZ == R_XEnd)
	{
		//if (p_JD2Arr[p_iJDIndex + 1].JDXZ == R_XStart || p_JD2Arr[p_iJDIndex + 1].JDXZ == R_X_OneR)
		//	return;

		xjj = p_JD->xjj;

		//�������ֹ���߼��û����,����������
		if (fabs(xjj) < 0.1)
			return;

		jd1xNo2 = p_JD2Arr[p_iJDIndex + 1].jdno1;
		jd1xNo1 = jd1xNo2 - 1;


		xyddaa(m_pm1->JDarray[jd1xNo1].N,m_pm1->JDarray[jd1xNo1].E,m_pm1->JDarray[jd1xNo2].N,m_pm1->JDarray[jd1xNo2].E,&fwj);
		pt1[0] = m_pm1->JDarray[jd1xNo1].N + xjj * cos(fwj+m_dHalfPI);
		pt1[1] = m_pm1->JDarray[jd1xNo1].E + xjj * sin(fwj+m_dHalfPI);
		pt2[0] = m_pm1->JDarray[jd1xNo2].N + xjj * cos(fwj+m_dHalfPI);
		pt2[1] = m_pm1->JDarray[jd1xNo2].E + xjj * sin(fwj+m_dHalfPI);


		pt3[0] =  p_JD2Arr[p_iJDIndex - 1].x;
		pt3[1] =  p_JD2Arr[p_iJDIndex - 1].y;
		pt4[0] =  p_JD2Arr[p_iJDIndex].x;
		pt4[1] =  p_JD2Arr[p_iJDIndex].y;

		INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

		//���ڻ����ϵ���������Ż�ȡ����ȷ����1�ַ����������ɹ������õ�2�ַ���
		if (fabs(p_JD->x - xtmp) > 10 || fabs(p_JD->y - ytmp) > 10)
		{
			xtmp = p_JD2Arr[p_iJDIndex].x;
			ytmp = p_JD2Arr[p_iJDIndex].y;

			tylc1x = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,xtmp,ytmp,xtmp,ytmp);

			jd1xNo1 = m_pm1->DistToNearJD(tylc1x,true);
			jd1xNo2 = m_pm1->DistToNearJD(tylc1x,false);

			xyddaa(m_pm1->JDarray[jd1xNo1].N,m_pm1->JDarray[jd1xNo1].E,m_pm1->JDarray[jd1xNo2].N,m_pm1->JDarray[jd1xNo2].E,&fwj);

			pt1[0] = m_pm1->JDarray[jd1xNo1].N + xjj * cos(fwj+m_dHalfPI);
			pt1[1] = m_pm1->JDarray[jd1xNo1].E + xjj * sin(fwj+m_dHalfPI);
			pt2[0] = m_pm1->JDarray[jd1xNo2].N + xjj * cos(fwj+m_dHalfPI);
			pt2[1] = m_pm1->JDarray[jd1xNo2].E + xjj * sin(fwj+m_dHalfPI);

			pt3[0] =  p_JD2Arr[p_iJDIndex - 1].x;
			pt3[1] =  p_JD2Arr[p_iJDIndex - 1].y;
			pt4[0] =  p_JD2Arr[p_iJDIndex].x;
			pt4[1] =  p_JD2Arr[p_iJDIndex].y;

			INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

			//���ַ������������ɹ�,���˳�
			if (fabs(p_JD->x - xtmp) > 10 || fabs(p_JD->y - ytmp) > 10)
			{
				jd1xNo1 += 1;
				jd1xNo2 += 1;

				xyddaa(m_pm1->JDarray[jd1xNo1].N,m_pm1->JDarray[jd1xNo1].E,m_pm1->JDarray[jd1xNo2].N,m_pm1->JDarray[jd1xNo2].E,&fwj);

				pt1[0] = m_pm1->JDarray[jd1xNo1].N + xjj * cos(fwj+m_dHalfPI);
				pt1[1] = m_pm1->JDarray[jd1xNo1].E + xjj * sin(fwj+m_dHalfPI);
				pt2[0] = m_pm1->JDarray[jd1xNo2].N + xjj * cos(fwj+m_dHalfPI);
				pt2[1] = m_pm1->JDarray[jd1xNo2].E + xjj * sin(fwj+m_dHalfPI);

				pt3[0] =  p_JD2Arr[p_iJDIndex - 1].x;
				pt3[1] =  p_JD2Arr[p_iJDIndex - 1].y;
				pt4[0] =  p_JD2Arr[p_iJDIndex].x;
				pt4[1] =  p_JD2Arr[p_iJDIndex].y;

				INTERS(&xtmp,&ytmp,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);

				if (fabs(p_JD->x - xtmp) > 10 || fabs(p_JD->y - ytmp) > 10)
				{
					return;
				}
				else
				{
					p_JD->x = xtmp;
					p_JD->y = ytmp;
				}
			}
			else
			{
				p_JD->x = xtmp;
				p_JD->y = ytmp;
			}
		}
		else
		{
			p_JD->x = xtmp;
			p_JD->y = ytmp;
		}

	}


}










//�ɽ���ǰ��ߵ��߼�������߽�������,�����ڶ��߽��н���
void GTZX2_JYX_ROAD::CalExJDByParrel(int jdn1, double xjj1, double xjj2, DoubleLineData *CurJD)
{
	if(jdn1<1 || jdn1>m_pm1->NJD-2)
		return;
	double fwj;
	xyddaa(m_pm1->JDarray[jdn1-1].N,m_pm1->JDarray[jdn1-1].E,m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,&fwj);
	double pt1[2],pt2[2];
	pt1[0] = m_pm1->JDarray[jdn1-1].N + xjj1 * cos(fwj+m_dHalfPI);
	pt1[1] = m_pm1->JDarray[jdn1-1].E + xjj1 * sin(fwj+m_dHalfPI);
	pt2[0] = m_pm1->JDarray[jdn1].N + xjj1 * cos(fwj+m_dHalfPI);
	pt2[1] = m_pm1->JDarray[jdn1].E + xjj1 * sin(fwj+m_dHalfPI);
	double pt3[2],pt4[2];
	xyddaa(m_pm1->JDarray[jdn1].N,m_pm1->JDarray[jdn1].E,m_pm1->JDarray[jdn1+1].N,m_pm1->JDarray[jdn1+1].E,&fwj);
	pt3[0] = m_pm1->JDarray[jdn1].N + xjj2 * cos(fwj+m_dHalfPI);
	pt3[1] = m_pm1->JDarray[jdn1].E + xjj2 * sin(fwj+m_dHalfPI);
	pt4[0] = m_pm1->JDarray[jdn1+1].N + xjj2 * cos(fwj+m_dHalfPI);
	pt4[1] = m_pm1->JDarray[jdn1+1].E + xjj2 * sin(fwj+m_dHalfPI);
	//�ҽ���

	INTERS(&CurJD->x,&CurJD->y,pt1[0],pt1[1],pt2[0],pt2[1],pt3[0],pt3[1],pt4[0],pt4[1]);
}

//��STR����At��������Ϊ-1;
int GTZX2_JYX_ROAD::FindACHAR(ACHAR *str,ACHAR At)
{
	int num=_tcslen(str);
	ACHAR ch;
	int i;
	for(i=0;i<num;i++)
	{
		ch=str[i];
		if(_tcscmp(&ch,&At)==0)break;
	}
	if(i>=num)i=-1;
	return i;
}

//���뽻������(����ȷ)��ƫ�࣬������������
void GTZX2_JYX_ROAD::ModifyNEbyXJJ(double &N, double &E, double xjj)
{
	xlpoint PT;
	PT.lc=PROJ_ML(m_pm1->array,m_pm1->tarray,N,E,N,E);	
	xlpoint_pz(m_pm1->array,m_pm1->tarray,&PT);
	N=PT.x+xjj*cos(PT.a+m_dHalfPI);
	E=PT.y+xjj*sin(PT.a+m_dHalfPI);
}

//����һ����̣�ƫ�࣬���ض�Ӧ������
void GTZX2_JYX_ROAD::ModifyNEbyXJJ(ACHAR JDLC[],double xjj,double &N,double &E)
{
	xlpoint PT;
	PT.lc = m_pm1->TYLC(JDLC,m_pm1->NDL);	
	xlpoint_pz(m_pm1->array,m_pm1->tarray,&PT);
	N=PT.x + xjj*cos(PT.a + m_dHalfPI);
	E=PT.y + xjj*sin(PT.a + m_dHalfPI);	
}

//�ѽ���ͶӰ����Ӧһ�߱��ϣ������¼�Ľ����Ϊ��Ӧһ�߱ߵĺ�һ������
void GTZX2_JYX_ROAD::ModifyNEOnBian(DoubleLineData *CurJD)
{
	double inX1,inY1,inX2,inY2,distTo2PointOnLine;

	//��Ӧ�ߺ�һ�������
	int jd2=CurJD->jdno1;
	if(jd2<1||jd2>m_pm1->NJD-1)return;

	//��Ӧ��ǰһ�������
	int jd1=jd2-1;

	//��Ӧ�ߵ�ǰһ�����������
	inX1=m_pm1->JDarray[jd1].N;
	inY1=m_pm1->JDarray[jd1].E;

	//��Ӧ�ߵĺ�һ�����������
	inX2=m_pm1->JDarray[jd2].N;
	inY2=m_pm1->JDarray[jd2].E;


	double outx,outy;
	outx=CurJD->x;
	outy=CurJD->y;

	DistToLine(inX1,inY1,inX2,inY2,outx,outy,distTo2PointOnLine);

	double fwj;
	xyddaa(inX1,inY1,inX2,inY2,&fwj);
	//CurJD->x = outx + CurJD->xjj*cos(fwj+0.5*PI);
	//CurJD->y = outy + CurJD->xjj*sin(fwj+0.5*PI);	
	if(fabs(CurJD->xjj)<1e-6) CurJD->xjj=1e-6;
	double x,y;
	x = outx;// + CurJD->xjj*cos(fwj+0.5*PI);
	y = outy;// + CurJD->xjj*sin(fwj+0.5*PI);	
	CurJD->x = x;
	CurJD->y = y;	

}


void GTZX2_JYX_ROAD::ModifyRXDName(double N, double E)
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

double GTZX2_JYX_ROAD::TYLC2x(CString XCLC2x,int iRXD)//��iRXD��0��ָ�����жβ���
{
	double XLC;
	double TLC=-99999999.0;//ͳһ���
	int sdl,edl;
	ACHAR GH[10],STRXLC[80];
	_tcscpy(STRXLC,XCLC2x);
	split_ckml(STRXLC,&XLC,GH);
	int bz=-99;

	ACHAR ctmp[80];
	double dDLBLC = 0.0,dDLELC = 0.0;
	if(iRXD>=0)
	{
		GetiRXDDLinfo(iRXD,sdl,edl);

		for(int i=sdl;i<edl;i++)
		{
			_stprintf(ctmp,L"%.3lf",DLB2x[i+1].BLC);
			dDLBLC = _wtof(ctmp);

			_stprintf(ctmp,L"%.3lf",DLB2x[i].ELC);
			dDLELC = _wtof(ctmp);

			if(XLC >= dDLELC && XLC <= dDLBLC)//���ڸö�����
			{	
				double DLCml = CalnewcmlTocml(DLB2x[i].TYLC);
				if(bz<0)
				{
					TLC = DLCml + (XLC-DLB2x[i].ELC);
					TLC = CalcmlTonewcml(TLC);
					bz=1;
				}// ��һ����
				else 
				{
					bz=2;
					if(_tcscmp(GH,DLB2x[i].BGH)==0)
					{	
						TLC= DLCml+(XLC-DLB2x[i].ELC);
						TLC = CalcmlTonewcml(TLC);
					}
				}
			}
		}
		if(TLC>-99999999.0)return TLC;
	}
	else//��ָ�����жλ�ָ������
	{
		int NRxd=GetRXDNum();
		for(iRXD=0;iRXD<NRxd;iRXD++)
		{
			GetiRXDDLinfo(iRXD,sdl,edl);

			for(int i=sdl;i<edl;i++)
			{
				_stprintf(ctmp,L"%.3lf",DLB2x[i+1].BLC);
				dDLBLC = _wtof(ctmp);

				_stprintf(ctmp,L"%.3lf",DLB2x[i].ELC);
				dDLELC = _wtof(ctmp);
				if(XLC >= dDLELC && XLC <= dDLBLC)  // ���ڸö�����
				{   
					double DLCml = CalnewcmlTocml(DLB2x[i].TYLC);
					if(bz<0)
					{
						TLC = DLCml + (XLC-DLB2x[i].ELC);
						TLC = CalcmlTonewcml(TLC);
						bz=1;
					}// ��һ����
					else 
					{
						bz=2;
						if(_tcscmp(GH,DLB2x[i].BGH)==0)
						{
							TLC = DLCml + (XLC-DLB2x[i].ELC);
							TLC = CalcmlTonewcml(TLC);
						}
					}
				}
			}
			if(TLC>-99999999.0)return TLC;
		}
	}
	//һ�������
	ACHAR tmp[256];
	_tcscpy(tmp,XCLC2x);

	double tylc1x = 0.0 ;
	double tylc2x = 0.0 ;
	//	tylc1x=m_pm1->TYLC(tmp,m_pm1->NDL);
	tylc1x=TYLC1(tmp,m_pm1->NDL,m_pm1->DLB);
	tylc1x = m_pm1->CalcmlTonewcml(tylc1x);
	xlpoint PZ;
	PZ.lc = tylc1x;
	m_pm1->xlpoint_pz(m_pm1->array, m_pm1->tarray, &PZ);
	tylc2x = PROJ_ML(array, tarray, PZ.x, PZ.y, PZ.x, PZ.y);

	if(_tcscmp(xlname,L"��ͨ��Ƣ���")==0)
	{

		xlpoint PZtmp;
		PZtmp.lc = tylc1x ;

		m_pm1->xlpoint_pz_GTX(m_pm1->array,m_pm1->tarray,&PZtmp);

		tylc2x = PROJ_ML(array,tarray,PZtmp.x,PZtmp.y,PZtmp.x,PZtmp.y);
	}

	return tylc2x;
} 


//////////////////////////////////////////////////////////////////////////
//���ֳ���̻�ȡ��Ӧ�ĸĽ��Σ������λ�ڲ��ж�ʱ����-1
int GTZX2_JYX_ROAD::GetRXDIndexByXCLC2X(CString XCLC2x)
{
	double XLC;
	int sdl,edl;
	ACHAR GH[10],CKML[80];
	_tcscpy(CKML,XCLC2x);
	split_ckml(CKML,&XLC,GH);

	int NRxd=GetRXDNum();
	for(int iRXD=0;iRXD<NRxd;iRXD++)
	{
		GetiRXDDLinfo(iRXD,sdl,edl);

		for(int i=sdl;i<edl;i++)
		{
			if(XLC >= DLB2x[i].ELC && XLC <= DLB2x[i+1].BLC)// �������ж���ʼ�ֳ���̵Ķ�����
			{   
				//Zhujiang20090509 Ӧ��һ���жϸ��ֳ���̵Ĺں��Ƿ�����ж��ڵ���̹ں�һ��,����
				//���ж������յ�Ϊ����ʱ�����ܻ����ؽ����ڸ����ж��ϵ��ֳ�����ж�Ϊ�ڸ����ж��ϡ�
				if (_tcscmp(GH,DLB2x[i+1].BGH) == 0 && _tcscmp(GH,DLB2x[i].EGH) == 0)
				{
					return iRXD;
				}
			}
		}			
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
int GTZX2_JYX_ROAD::GetPZFromXCLC2X(CString XCLC2x,struct xlpoint& PZ)
{
	bool IsOnXL = true;
	double cml1,cml2;
	double dml;
	ACHAR ckml[80],GH[20];
	int RXDIndex;
	RXDIndex = GetRXDIndexByXCLC2X(XCLC2x);
	if (RXDIndex==-1)//���ж�
	{
		if (_tcscmp(xlname,L"��ͨ��Ƣ���")==0)//��ͨ��ƶ���,���ֳ����Ϊ��ͨ��ƻ����ϵ���̡����������ڹ�ͨ��ƻ����ϵ����꣬��ͶӰ����ͨ����
		{
			if (m_pm1->GetPZFromXCLC1X(XCLC2x,PZ))
			{
				cml2 = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
				PZ.lc = cml2;
				xlpoint_pz(array,tarray,&PZ);
				return 1;
			}
			else
				return 0;
		}
		else//һ�����
		{
			if (!m_pm2)//�ö��߲��ջ��߶���,���ֳ����Ϊ��Ӧ���ߵ��ֳ���̡����������ڶ�Ӧ�����ϵ����꣬��ͶӰ������
			{	
				if (m_pm1->GetPZFromXCLC1X(XCLC2x,PZ))
				{
					cml2 = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					PZ.lc = cml2;
					xlpoint_pz(array,tarray,&PZ);
					return 1;
				}
				else
					return 0;
			}
			else//�ö��߲��ռ��ж��߶��������ֳ����Ϊ���ж����ϵ��ֳ���̡��ݹ���õõ��ڼ��ж����ϵ����꣬��ͶӰ����ǰ����
			{
				if (m_pm2->GetPZFromXCLC2X(XCLC2x,PZ))
				{
					cml2 = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					PZ.lc = cml2;
					xlpoint_pz(array,tarray,&PZ);
					return 1;
				}
				else
					return 0;
			}
		}
	}
	else//���ж�
	{
		GTZX_JYX_ROAD* pSubGTZX = NULL;
		if (_tcscmp(xlname,L"��ͨ��Ƣ���")==0)//��ͨ��ƶ���.�������ж���ȡΪ�ӹ�ͨ��ƻ��ߣ����ֳ���̼������ꡣ
		{
			pSubGTZX = GetiRxdtoGTZXplinp_GTX(RXDIndex);
			if (pSubGTZX)
			{
				int k = pSubGTZX->GetPZFromXCLC1X(XCLC2x,PZ);
				PZ.lc = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
				delete pSubGTZX;
				return k;
			}
			else
				return 0;
		}
		else
		{
			pSubGTZX = GetiRxdtoGTZXplinp(RXDIndex);
			if (pSubGTZX)
			{
				int k = pSubGTZX->GetPZFromXCLC1X(XCLC2x,PZ);
				PZ.lc = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);

				delete pSubGTZX;
				return k;
			}
			else
				return 0;
		}
	}
	return 1;
}

//����ʰȡ�����꣬�����ڸĽ���or���ö��ϣ����ظĽ����±�
int GTZX2_JYX_ROAD::GetGJDNum(double N,double E)
{
	//���������ͶӰ���������ϵ����
	double prjml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,N,E,N,E);
	//�ԸĽ�������ѭ��
	double sml,eml;

	for(int i=0; i<GJDInfTab.GetSize(); i++)
	{
		sml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,GJDInfTab[i].spt[0],GJDInfTab[i].spt[1],GJDInfTab[i].spt[0],GJDInfTab[i].spt[1]);
		eml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,GJDInfTab[i].ept[0],GJDInfTab[i].ept[1],GJDInfTab[i].ept[0],GJDInfTab[i].ept[1]);
		if(prjml>sml-0.001 && prjml<eml+0.001)
			return i;//�ҵ����ڵ�i���Ľ���
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////
//����N��E�����ȡ�õ����ڸĽ��ε������ţ����öη���-1
int GTZX2_JYX_ROAD::GetGT2xGJDIndex(double N,double E)
{
	//���������ͶӰ���������ϵ����
	double prjml = PROJ_ML(array,tarray,N,E,N,E);
	//�ԸĽ�������ѭ��
	double sml,eml;

	for(int i=0; i<GJDInforArray.GetSize(); i++)
	{
		sml = PROJ_ML(array,tarray,GJDInforArray[i].spt[0],GJDInforArray[i].spt[1],GJDInforArray[i].spt[0],GJDInforArray[i].spt[1]);
		eml = PROJ_ML(array,tarray,GJDInforArray[i].ept[0],GJDInforArray[i].ept[1],GJDInforArray[i].ept[0],GJDInforArray[i].ept[1]);
		if(prjml>sml-0.001 && prjml<eml+0.001)
			return i;//�ҵ����ڵ�i���Ľ���
	}
	return -1;
}

void GTZX2_JYX_ROAD::SetDefaultGjdName(double N,double E,int GJDType,ACHAR GjdName[256])
{
	//double prjml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,N,E,N,E);
	//ACHAR GH[8];
	//double dml = m_pm1->XLC(prjml,GH,m_pm1->NDL);
	//dml = dml/1000.0;
	//if (m_pm2==NULL)
	//	_stprintf(GjdName,L"%0.1lf%s�Ľ���",dml,m_pm1->XLShuXing);
	//else
	//	_stprintf(GjdName,L"%0.1lf%s�Ľ���",dml,m_pm2->m_pm1->XLShuXing);
	//ACHAR zxoryx[8];
	//if(zory<0)
	//	_tcscpy(zxoryx,L"����");
	//else
	//	_tcscpy(zxoryx,L"����");

	////���������ݿ���-����-�Ľ���-�����(��)��
	//_stprintf(GjdName,L"%s-%0.1lf%s-�Ľ���(%s)",m_pm1->mdbname,dml,m_pm1->XLShuXing,zxoryx);

	double prjml = 0.0;
	ACHAR GH[8];
	double dml = 0.0;
	if (m_pm2==NULL)
	{
		prjml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,N,E,N,E);
		dml = m_pm1->XLC(prjml,GH,m_pm1->NDL);
		if (GJDType==0)
			_stprintf(GjdName,L"%0.1lf���иĽ�",dml/1000.0);
		if (GJDType==2)
			_stprintf(GjdName,L"%0.1lf���иĽ�",dml/1000.0);
	}
	if (m_pm2)
	{
		prjml = m_pm2->m_pm1->PROJ_ML(m_pm2->m_pm1->array,m_pm2->m_pm1->tarray,N,E,N,E);
		dml = m_pm2->m_pm1->XLC(prjml,GH,m_pm2->m_pm1->NDL);
		if (GJDType==0)
			_stprintf(GjdName,L"%0.1lf���иĽ�",dml/1000.0);
		if (GJDType==2)
			_stprintf(GjdName,L"%0.1lf���иĽ�",dml/1000.0);
	}
	//dml = dml/1000.0;
	//if (m_pm2==NULL)
	//	_stprintf(GjdName,L"%0.1lf%s�Ľ���",dml,m_pm1->XLShuXing);
	//else
	//	_stprintf(GjdName,L"%0.1lf%s�Ľ���",dml,m_pm2->m_pm1->XLShuXing);
}

bool GTZX2_JYX_ROAD::IsJdInGTZX(int iJD)
{
	bool isInRoad;//�����Ƿ�����·��

	double dist;
	if(JD2[iJD].JDXZ!=R_X && JD2[iJD].JDXZ!=R_X_OneR && JD2[iJD].JDXZ!=R_XStart && JD2[iJD].JDXZ!=R_XEnd)
	{
		if(fabs(JD2[iJD].xjj)>0.001)
			isInRoad = false;//������·��
		else 
			isInRoad = true;//����·��
	}
	else//���н���
	{
		dist = m_pm1->DistFromPtToGTZX(JD2[iJD].x,JD2[iJD].y);
		if(dist >0.01)
			isInRoad = false;//������·��
		else 
			isInRoad = true;//������·��
	}
	return isInRoad;
}
//�Ӷ��߽�����sjdn��ejdn seach ��һ���Ľ�����ʼ�����
int GTZX2_JYX_ROAD::SearchGJDSptIndex(int sjdn, int ejdn)
{
	bool qisInRoad, hisInRoad;//ǰ�󽻵��Ƿ�����·��	
	if(sjdn > 0)
		qisInRoad = IsJdInGTZX(sjdn-1);
	else
		qisInRoad = true;
	for(int i=sjdn; i<= ejdn ; i++)
	{
		hisInRoad = IsJdInGTZX(i);
		if(qisInRoad && !hisInRoad)
			if(i>0)
			{
				if(JD2[i].JDXZ!=S_EndPoint)
					return i;
				else  //S���յ�
					return i-1;

			}
			else
				return 0;

		else if(qisInRoad && hisInRoad)//�޸��˵�����
		{
			if(i > 0 && (JD2[i].JDXZ==B_X) && fabs(JD2[i].xjj)<0.001)//�������߼��Ϊ0
			{
				int jdno1;//һ�߽����±�
				jdno1 = JD2[i].jdno1;
				if(jdno1>0 && jdno1<m_pm1->NJD)
				{
					double qR,qLo1,qLo2,hR,hLo1,hLo2;
					qR = m_pm1->JDarray[jdno1].R;
					qLo1 = m_pm1->JDarray[jdno1].l1;
					qLo2 = m_pm1->JDarray[jdno1].l2;
					hR = JD2[i].R;
					hLo1 = JD2[i].Lo1;
					hLo2 = JD2[i].Lo2;
					if(fabs(qR-hR)>0.001 || fabs(qLo1-hLo1)>0.001 || fabs(qLo2-hLo2)>0.001)
						return i;
				}

			}
		}

		qisInRoad = hisInRoad;
	}
	return -1;
}

//�Ӷ��߽�����sjdn��ejdn seach ��һ���Ľ�����ֹ�����
int GTZX2_JYX_ROAD::SearchGJDEptIndex(int sjdn, int ejdn)
{
	bool qisInRoad, hisInRoad;//ǰ�󽻵��Ƿ�����·��	
	if(sjdn > 0)
		qisInRoad = IsJdInGTZX(sjdn-1);
	else
		qisInRoad = true;
	for(int i=sjdn; i<= ejdn ; i++)
	{
		hisInRoad = IsJdInGTZX(i);
		if(!qisInRoad && hisInRoad)
			return i;
		else if(qisInRoad && hisInRoad)//�޸��˵�����
		{
			if(i > 0 && (JD2[i].JDXZ==B_X) && fabs(JD2[i].xjj)<0.001)//�������߼��Ϊ0
			{
				int jdno1;//һ�߽����±�
				jdno1 = JD2[i].jdno1;
				if(jdno1>0 && jdno1<m_pm1->NJD)
				{
					double qR,qLo1,qLo2,hR,hLo1,hLo2;
					qR = m_pm1->JDarray[jdno1].R;
					qLo1 = m_pm1->JDarray[jdno1].l1;
					qLo2 = m_pm1->JDarray[jdno1].l2;
					hR = JD2[i].R;
					hLo1 = JD2[i].Lo1;
					hLo2 = JD2[i].Lo2;
					if(fabs(qR-hR)>0.001 || fabs(qLo1-hLo1)>0.001 || fabs(qLo2-hLo2)>0.001)
						return i;
				}

			}
		}
		if(i==NJD-1 && !qisInRoad)
			return i;
		qisInRoad = hisInRoad;
	}
	return -1;
}

//�����Ľ�����Ϣ��
int GTZX2_JYX_ROAD::CreatGJDInfTab()
{

	assertWriteEnabled();
	int sjd,ejd;
	int sno,eno;
	double spt[2],ept[2];

	sjd = 0;
	ejd =  NJD -1;

	GJDInfTab.RemoveAll();
	for(;;)
	{
		sno = SearchGJDSptIndex(sjd,ejd);
		if(sno >=0)
		{
			sjd = sno;
			eno = SearchGJDEptIndex(sjd,ejd);
			if(eno > 0)
			{
				sjd = eno ;
				getZhHzPt(sno,-1,spt);
				//����Ľ����յ�����ept			 
				getZhHzPt(eno,1,ept);
				GJDInf OneGjdInfo;
				OneGjdInfo.spt[0] = spt[0],OneGjdInfo.spt[1] = spt[1];
				OneGjdInfo.ept[0] = ept[0],OneGjdInfo.ept[1] = ept[1];
				OneGjdInfo.ID = GJDInfTab.GetSize()+1;
				//				SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
				SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.GJDType,OneGjdInfo.gjdname);
				GJDInfTab.Add(OneGjdInfo);
				if(sno == eno)
					sjd = sno + 1;
			}
		}
		else
			break;
	}

	NGJD = GJDInfTab.GetSize();
	ads_printf(L"nn = %d\n",GJDInfTab.GetSize());
	for(int i=0;i<GJDInfTab.GetSize();i++)
	{
		//	BZiGJDLC(pWd,i);//�����ע���ж�
		ads_printf(L"sx=%lf sy=%lf ex=%lf ey=%lf\n", GJDInfTab[i].spt[0],GJDInfTab[i].spt[1],
			GJDInfTab[i].ept[0],GJDInfTab[i].ept[1]);
	}
	return 1;
}

bool GTZX2_JYX_ROAD::IsIncludeByOtherGjd(double spt[2],double ept[2])
{
	//�����������
	double sml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,spt[0],spt[1],spt[0],spt[1]);
	double eml = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,ept[0],ept[1],ept[0],ept[1]);

	double cml1,cml2;
	bool IfInclude;

	IfInclude = false;

	for(int i=0; i<GJDInfTab.GetSize(); i++)
	{
		cml1 = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,GJDInfTab[i].spt[0],GJDInfTab[i].spt[1],GJDInfTab[i].spt[0],GJDInfTab[i].spt[1]);
		cml2 = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,GJDInfTab[i].ept[0],GJDInfTab[i].ept[1],GJDInfTab[i].ept[0],GJDInfTab[i].ept[1]);
		if(sml>cml1-0.001 && eml<cml2+0.001)
			IfInclude = true;
		if(cml1>sml-0.001 && cml2<eml+0.001)
			IfInclude = true;

	}

	return IfInclude;
}
//����Ľ������յ����
CString GTZX2_JYX_ROAD::CalGJDLC(double N,double E)//�����ע���ж�
{
	double CmlOn1X;//���յ���1���ϵ����
	CString ckml;
	CmlOn1X = m_pm1->PROJ_ML(m_pm1->array,m_pm1->tarray,N,E,N,E);
	ckml = m_pm1->XLC(CmlOn1X);

	return ckml;
}

//��ע�Ľ������յ����
void GTZX2_JYX_ROAD::BZiGJDLC(AcGiWorldDraw* pWd,int iGJD)//�����ע���ж�
{

	//	double SmlOn1X,EmlOn1X;//���յ���1���ϵ����
	double SmlOn2X,EmlOn2X;//���յ���2���ϵ����

	//1.��2���϶������յ�λ������λ��
	xlpoint PZ1,PZ2;	//�������յ�������	
	SmlOn2X = PROJ_ML(array,tarray,GJDInfTab[iGJD].spt[0],GJDInfTab[iGJD].spt[1],GJDInfTab[iGJD].spt[0],GJDInfTab[iGJD].spt[1]);
	PZ1.lc = SmlOn2X;
	EmlOn2X = PROJ_ML(array,tarray,GJDInfTab[iGJD].ept[0],GJDInfTab[iGJD].ept[1],GJDInfTab[iGJD].ept[0],GJDInfTab[iGJD].ept[1]);
	PZ2.lc = EmlOn2X;
	xlpoint_pz(array,tarray,&PZ1);//������
	xlpoint_pz(array,tarray,&PZ2);
	//2.���ɱ�ע��Ϣ
	CString str,mlstr;
	str.Format(L"%s ���",GJDInfTab[iGJD].gjdname);
	mlstr = CalGJDLC(GJDInfTab[iGJD].spt[0],GJDInfTab[iGJD].spt[1]);

	BZSTR(pWd,PZ1,str,mlstr);  

	str.Format(L"%s �յ�",GJDInfTab[iGJD].gjdname);
	mlstr = CalGJDLC(GJDInfTab[iGJD].ept[0],GJDInfTab[iGJD].ept[1]);
	BZSTR(pWd,PZ2,str,mlstr);  


}
//��ע�ִ�,�����������·��+���յ㣬��������
void GTZX2_JYX_ROAD::BZSTR(AcGiWorldDraw* pWd,xlpoint PZ,CString str1,CString str2)  
{
	AcGePoint3d cpt,spt,ept,mpt;

	double _tcslen1,_tcslen2,_tcslen;
	double WB;

	_tcslen1 = str1.GetLength();
	_tcslen2 = str2.GetLength();
	_tcslen = _tcslen1 > _tcslen2 ? _tcslen1 : _tcslen2;

	WB = 1.5*texth*_tcslen;

	spt.set(PZ.y,PZ.x,0.0);

	ept.x = PZ.y + WB*sin(PZ.a+pi*0.5*draw_zybz);
	ept.y = PZ.x + WB*cos(PZ.a+pi*0.5*draw_zybz);
	ept.z = 0.0;

	G_makeline(pWd,spt,ept,6);//���ע��

	AcGeVector3d vec;
	vec = (ept-spt);
	vec.normalize();
	vec = 0.25*texth*_tcslen*vec;
	cpt = spt + vec;

	/*
	cpt.x = 0.5*(spt.x+ept.x);
	cpt.y = 0.5*(spt.y+ept.y);
	cpt.z = 0.0;

	*/



	mpt.x = cpt.x + 1.2*texth*sin(PZ.a);
	mpt.y = cpt.y + 1.2*texth*cos(PZ.a);
	mpt.z = 0.0;

	double b;

	b=PZ.a+pi*0.5;
	b=pi*0.5-b;
	if(draw_zybz==-1)b=b+pi;
	if(b<0.0)b=2*pi+b;
	if(b>2*pi) b=b-2*pi;		         
	//if(type<=1)b=b-pi*0.5; 


	ACHAR str[256];
	_tcscpy(str,str1);
	G_maketext(pWd,mpt,str,b,texth ,QXYSColor,1);


	mpt.x = cpt.x + 0.7*texth*sin(PZ.a+pi);
	mpt.y = cpt.y + 0.7*texth*cos(PZ.a+pi);
	mpt.z = 0.0;

	_tcscpy(str,str2);
	G_maketext(pWd,mpt,str,b,texth ,QXYSColor,1);
}

void GTZX2_JYX_ROAD::BZGJD(AcGiWorldDraw* pWd)//��ע���иĽ���
{

	for(int i=0;i<GJDInfTab.GetSize();i++)
	{
		BZiGJDLC(pWd,i);//�����ע���ж�
	}

}
//�������zhhz��
Adesk::Boolean  GTZX2_JYX_ROAD::getZhHzPt(int iJD,int zhorhz,double pt[2])
{
	double cml1,cml2,cml;
	xlpoint PZ;
	int jdno1;//��Ӧһ�߽����
	double pt1[2]; // ,pt2[2];


	if(iJD>=0 && iJD <NJD)
	{

		jdno1 = JD2[iJD].jdno1;

		if(zhorhz == -1)
		{	
			cml2 = QXB[iJD].KZH;
			if( m_pm1->getZhHzPt(jdno1,-1,pt1))
				cml1 = PROJ_ML(array,tarray,pt1[0],pt1[1],pt1[0],pt1[1]);//�����ڶ���ͶӰ���
			else
				cml1 = cml2;    	  		

			cml = cml1 < cml2 ? cml1 : cml2;		  
		}
		else
		{
			cml2 = QXB[iJD].KHZ;
			if( m_pm1->getZhHzPt(jdno1,1,pt1))
				cml1 = PROJ_ML(array,tarray,pt1[0],pt1[1],pt1[0],pt1[1]);//�����ڶ���ͶӰ���
			else
				cml1 = cml2;    	  		

			cml = cml1 > cml2 ? cml1 : cml2;	
		}

		pt[0] = 0.0;
		pt[1] = 0.0;

		PZ.lc=cml;
		xlpoint_pz(array,tarray,&PZ);  //ʵ�����ת����
		pt[0] = PZ.x;
		pt[1] = PZ.y;
		return 1;
	}

	return 0;    

}


// ����IRXD�Ļ���ָ��
GTZX_JYX_ROAD * GTZX2_JYX_ROAD::GetiRxdtoGTZXplinp(int irxd )
{
	if(irxd<0)return NULL;
	GTZX_JYX_ROAD * RXDGTZX;
	double jday[MAXJDNUM][6];
	int njd;
	int SDLNum, EDLNum;
	xlpoint PZ;
	//��ȡjdarray
	GetiRXDJDArray(irxd,jday,njd);
	GetiRXDDLinfo(irxd,SDLNum,EDLNum);

	DLBZ DLBTemp[30];
	int NDLTemp=0;
	int i;
	for(i=SDLNum;i<=EDLNum ;i++)
	{
		_tcscpy( DLBTemp[NDLTemp].BGH , DLB2x[i].BGH ) ;
		_tcscpy( DLBTemp[NDLTemp].EGH , DLB2x[i].EGH ) ;
		DLBTemp[NDLTemp].BLC =  DLB2x[i].BLC ;
		DLBTemp[NDLTemp].DL =  DLB2x[i].ELC - DLB2x[i].BLC;
		PZ.lc = DLB2x[i].TYLC;
		xlpoint_pz(array,tarray,&PZ);
		DLBTemp[NDLTemp].N = PZ.x;
		DLBTemp[NDLTemp].E = PZ.y;
		NDLTemp ++;
	}

	CArray<JDarrayStruct,JDarrayStruct> JDarrayTemp;

	JDarrayTemp.SetSize(njd);

	//////////////////////////////////////////////////////////////////////////
	//�����Ӧ��Ϊ��Ӧ�ڶ��ߵ��ַ������޸ĺ�
	int sjd = 0;
	int ejd = 0;
	GetJDXHofiRXD(irxd,sjd,ejd);//��ȡ���ս����
	int IndexOnEx = 0;

	for(i=0;i<njd;i++)
	{
		IndexOnEx = static_cast<int>(sjd-1+jday[i][0]+0.001);
		_tcscpy(JDarrayTemp[i].JDNum ,JD2[IndexOnEx].JDNum);//ԭ����
		JDarrayTemp[i].N = jday[i][1];
		JDarrayTemp[i].E = jday[i][2];
		JDarrayTemp[i].l1 = jday[i][3];
		JDarrayTemp[i].R = jday[i][4];
		JDarrayTemp[i].l2 = jday[i][5];
	}
	_tcscpy(JDarrayTemp[0].JDNum , L"RXQD");
	_tcscpy(JDarrayTemp[njd-1].JDNum, L"RXZD");
	//////////////////////////////////////////////////////////////////////

	//for(i=0;i<njd;i++)
	//{
	//	_stprintf( JDarrayTemp[i].JDNum ,L"%lf",jday[i][0] );//ԭ����
	//	int IndexOnEx = 0;
	//	JDarrayTemp[i].N = jday[i][1];
	//	JDarrayTemp[i].E = jday[i][2];
	//	JDarrayTemp[i].l1 = jday[i][3];
	//	JDarrayTemp[i].R = jday[i][4];
	//	JDarrayTemp[i].l2 = jday[i][5];
	//}

	JDarrayTemp[0].l1 = DLBTemp[0].BLC + DLBTemp[0].DL ;

	ACHAR GH[8],JDGH[8];
	_tcscpy(GH,L"AK"); 
	_tcscpy(JDGH,L"JD");

	RXDGTZX = new GTZX_JYX_ROAD(JDarrayTemp.GetSize(),JDarrayTemp,&GH[0],&JDGH[0]);

	RXDGTZX->NDL = NDLTemp;

	for(i=0;i<NDLTemp;i++)
	{
		RXDGTZX->setDLM(i,DLBTemp[i].BLC,DLBTemp[i].DL,DLBTemp[i].BGH,DLBTemp[i].EGH);
		RXDGTZX->DLB[i].N = DLBTemp[i].N;
		RXDGTZX->DLB[i].E = DLBTemp[i].E;
	}


	CArray<BAS_DRAW_FUN_JYX::BZLCStructTable,BAS_DRAW_FUN_JYX::BZLCStructTable>  iRxdBZLCArrayTmp ;
	GetiRxdBZLCArray( irxd , iRxdBZLCArrayTmp );

	RXDGTZX->BZLCarray.RemoveAll();

	RXDGTZX->BZLCarray.Append(iRxdBZLCArrayTmp);

	RXDGTZX->NBZLC = RXDGTZX->BZLCarray.GetSize();

	RXDGTZX->SortBZLCArray();

	RXDGTZX->OnReCalBZLC(RXDGTZX->BZLCarray);

	RXDGTZX->getpxy_DATA();

	//����getpxy_DATA()�������յ����ֵ�������յ���ֳ������Ϊ�µĶ�ǰ��̣�
	//�����յ���λ�ڶ����ϵģ����ܻ���ɶ�ǰ���Ҳ������ɶϺ���̣�����Ҫ�Ѷ���ֵ��ֵ������һ������Ϻ���̻ᵼ�����ϵͳ�������Խ�������ֵ����
	for(i=0;i<NDLTemp;i++)
	{
		RXDGTZX->setDLM(i,DLBTemp[i].BLC,DLBTemp[i].DL,DLBTemp[i].BGH,DLBTemp[i].EGH);
		RXDGTZX->DLB[i].N = DLBTemp[i].N;
		RXDGTZX->DLB[i].E = DLBTemp[i].E;
	}

	JDarrayTemp.RemoveAll();

	return RXDGTZX;

}




GTZX2_JYX_ROAD::GTZX2_JYX_ROAD(GTZX_JYX_ROAD *gtzx, double xjj , BOOL IsGaiJian )//plh04.30
	//xjj=-1,��������,xjj=1,��������
{

	AllColor = -1;
	if (gtzx->XLShuXing == "��ͨ��ƻ���")
	{
		_tcscpy(xlname, L"��ͨ��Ƣ���");
	}
	else	
		_tcscpy(xlname,L"");

	isJYGuiFanCS = !IsGaiJian;
	//if(isJYGuiFanCS) mGuiFanCS = mJYGuiFanCS;
	//else mGuiFanCS = mGJGuiFanCS;
	m_GuiFanCS = gtzx->m_GuiFanCS;

	m_IsNeedCalGJDInf = false;
	ACHAR JDGH2x[8];
	if( fabs( xjj)< 0.001 )
	{
		_tcscpy( JDGH2x , gtzx->JDGH);
	}
	else if( xjj > 0.001 )
	{
		_tcscpy( JDGH2x , L"YJD" );
	}
	else 
	{
		_tcscpy( JDGH2x , L"ZJD" );
	}

	zory = 0;
	calsflag =true;
	m_pm1=gtzx;
	NJD=m_pm1->NJD;
	JD0=m_pm1->JD0;
	JDarray.RemoveAll();
	JDarray.SetSize(m_pm1->JDarray.GetSize());

	QXB.RemoveAll();
	CString sTmp;
	int i;
	for( i=0;i<NJD;i++)
	{
		JD2[i].JDXZ=B_X;
		JD2[i].jdno=i;
		JD2[i].jdno1=i;//��Ӧһ�߽������;
		JD2[i].xjj=xjj;
		sTmp = m_pm1->JDarray[i].JDNum;
		sTmp.Replace(m_pm1->JDGH,L"");
		_stprintf( JD2[i].JDNum , L"%s%s" ,JDGH2x ,sTmp);
	}
	//���ս�����һ���ϵ�ͶӰ���//plh05.06
	ACHAR *lcstr=LCchr(m_pm1->DLB[0].EGH,
		m_pm1->DLB[0].BLC +	m_pm1->DLB[0].DL,3,1);
	_tcscpy(JD2[0].ZHLC,lcstr);
	lcstr=LCchr(m_pm1->DLB[m_pm1->NDL-1].EGH,
		m_pm1->DLB[m_pm1->NDL-1].BLC + m_pm1->DLB[m_pm1->NDL-1].DL,3,1);
	_tcscpy(JD2[NJD-1].ZHLC,lcstr);

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
	GetT0min_Lrmin();

	CalJDdata();

	SetJDdata();

	getpxy_DATA();

	NDL=0;

	CalNYDL();

	m_pm2 = NULL;
	IsReSetColor = FALSE ;

	RTolCheckUp( IsGaiJian );
	xlpoint PZtmp;
	//	PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(��������)")==0)
		PZtmp.lc=array_pxy[0][0][4]+4.0 * xl_length/5.0;  // ͳһ���
	else if(_tcscmp(xlname,L"��ƻ���(���л���)")==0)
		PZtmp.lc=array_pxy[0][0][4]+ 2.0 * xl_length/5.0;  // ͳһ���

	xlpoint_pz(array,tarray,&PZtmp);
	m_XLNameNotePt[X]=PZtmp.y+xl_length*sin(PZtmp.a+pi*0.5)/8.0;
	m_XLNameNotePt[Y]=PZtmp.x+xl_length*cos(PZtmp.a+pi*0.5)/8.0;

	m_BPDArray.RemoveAll();
	m_NCM = 0;
}

/***********************************************************************
�������ܣ�������ͨ��ƶ��߸Ľ�����Ϣ��������������GJDInfTab��
����ֵ��1-��������  -1-����
���������
pGTZXJYJX	���л���ָ��
pGTZXJYYX	��������ָ��
pGTZXJYZX	��������ָ��
EXGJDFDInfTab	���߸Ľ��ηֶ���Ϣ��
************************************************************************/
int GTZX2_JYX_ROAD::CreatEXGJDInfTab(GTZX_JYX_ROAD* pGTZXJYJX, GTZX_JYX_ROAD* pGTZXJYYX, GTZX_JYX_ROAD* pGTZXJYZX, CArray<EXGJDFDInf,EXGJDFDInf&> &EXGJDFDInfTab)
{
	CArray<GJDInf, GJDInf> GJDInfJYJX;//��ͨ��ƶ�������ڼ��л��߸Ľ�����Ϣ��
	CArray<GJDInf, GJDInf> GJDInfJYYX;//��ͨ��ƶ�������ڼ������߸Ľ�����Ϣ��
	CArray<GJDInf, GJDInf> GJDInfJYZX;//��ͨ��ƶ�������ڼ������߸Ľ�����Ϣ��
	GTZX_JYX_ROAD *pGTSJEX = CreateGtzx();//��ͨ��ƶ���ת��ΪGTZX


	pGTSJEX->GJDInfTab.RemoveAll();
	pGTSJEX->CreatGJDInfTab(pGTZXJYJX);//������ͨ��ƶ�������ڼ��л��߸Ľ�����Ϣ��
	GJDInfJYJX.Append(pGTSJEX->GJDInfTab);


	if (pGTZXJYYX)
	{
		pGTSJEX->GJDInfTab.RemoveAll();
		pGTSJEX->CreatGJDInfTab(pGTZXJYYX);//������ͨ��ƶ�������ڼ��ж��߸Ľ�����Ϣ��
		GJDInfJYYX.Append(pGTSJEX->GJDInfTab);
	}

	if (pGTZXJYZX)
	{
		pGTSJEX->GJDInfTab.RemoveAll();
		pGTSJEX->CreatGJDInfTab(pGTZXJYZX);//������ͨ��ƶ�������ڼ������߸Ľ�����Ϣ��
		GJDInfJYZX.Append(pGTSJEX->GJDInfTab);
	}

	pGTSJEX->GJDInfTab.RemoveAll();

	int i;
	for ( i=0 ; i<EXGJDFDInfTab.GetSize() ; i++)
	{
		if(i==0)
		{
			EXGJDFDInfTab[i].spt[0] = pGTSJEX->JDarray[0].N;
			EXGJDFDInfTab[i].spt[1] = pGTSJEX->JDarray[0].E;
		}
		else
		{
			EXGJDFDInfTab[i].spt[0] = EXGJDFDInfTab[i-1].ept[0];
			EXGJDFDInfTab[i].spt[1] = EXGJDFDInfTab[i-1].ept[1];
		}


		if (_tcscmp(EXGJDFDInfTab[i].XLName, L"���л���")==0)
		{
			pGTSJEX->SearchGJD(EXGJDFDInfTab[i].spt, EXGJDFDInfTab[i].ept, GJDInfJYJX, pGTZXJYJX,L"���л���");
		}
		else if (_tcscmp(EXGJDFDInfTab[i].XLName, L"��������")==0)
		{
			pGTSJEX->SearchGJD(EXGJDFDInfTab[i].spt, EXGJDFDInfTab[i].ept, GJDInfJYYX, pGTZXJYYX,L"��������");
		}
		else if (_tcscmp(EXGJDFDInfTab[i].XLName, L"��������")==0)
		{
			pGTSJEX->SearchGJD(EXGJDFDInfTab[i].spt, EXGJDFDInfTab[i].ept, GJDInfJYZX, pGTZXJYZX,L"��������");
		}
		else
		{
			ads_printf(L"��%d�в��յļ�����·��%s����!\n",i+1,EXGJDFDInfTab[i].XLName);
		}
		//		return -1;//��������
	}

	GJDInforArray.RemoveAll();
	GJDInforArray.SetSize(pGTSJEX->GJDInfTab.GetSize());
	for(i=0; i<pGTSJEX->GJDInfTab.GetSize(); i++)
	{

		GJDInforArray[i].spt[0]  = pGTSJEX->GJDInfTab[i].spt[0];

		GJDInforArray[i].spt[1]  = pGTSJEX->GJDInfTab[i].spt[1];

		GJDInforArray[i].ept[0]  = pGTSJEX->GJDInfTab[i].ept[0];
		GJDInforArray[i].ept[1]  = pGTSJEX->GJDInfTab[i].ept[1];



		_tcscpy( GJDInforArray[i].BXLName , pGTSJEX->GJDInfTab[i].BXLName ) ;
		_tcscpy( GJDInforArray[i].EXLName , pGTSJEX->GJDInfTab[i].EXLName ) ;

		_tcscpy( GJDInforArray[i].gjdname , pGTSJEX->GJDInfTab[i].gjdname );

		GJDInforArray[i].GJDType = pGTSJEX->GJDInfTab[i].GJDType ;


	}


	FormSectionInfor();

	//���Ľ��߷ֶ���Ϣ��ֵ

	//NGJDFDInfTab = EXGJDFDInfTab.GetSize();
	//GJDFDInfTab.RemoveAll();
	//GJDFDInfTab.SetSize(NGJDFDInfTab);
	//for(i=0;i<EXGJDFDInfTab.GetSize();i++ )
	//{
	//       GJDFDInfTab[i] = EXGJDFDInfTab[i] ; 
	//}

	return 1;
}


/*
void GTZX2::FormSectionInforForGTZX2()//�γɹ�ͨ���2�߷ֶ���Ϣ����
{
//1.���Ľ��λ��ܱ�,ͨ��ͶӰ���������ö�(������)�ϵõ�ÿ�����յ���ֳ����

//�Ľ��������С
int SizeOfGJDB=GJDInforArray2.GetSize();

//2.�ڲ����ʼ���ö�,�õ������ֶ���Ϣ

//2.1���⴦��,�γɵ�һ�����ö�
BasicSectionInfor.RemoveAll();
BasicSectionInfor.SetSize(SizeOfGJDB*2+1);

//_stprintf(StCkMl,L"%s%lf",m_pRunThroughWay->DLB[0].BGH,m_pRunThroughWay->DLB[0].BLC);//��ͨ������CKML�����ö�����CKML��һ��
//_tcscpy(BasicSectionInfor[0].ToOutSGJJDNum ,GJDB[0].BXLName);

_tcscpy(BasicSectionInfor[0].BXLName ,GJDInforArray2[0].BXLName);
_tcscpy(BasicSectionInfor[0].EXLName ,GJDInforArray2[0].BXLName);

BasicSectionInfor[0].BpointToN = JDarray[0].N ;
BasicSectionInfor[0].BpointToE = JDarray[0].E ;

BasicSectionInfor[0].EpointToN = GJDInforArray2[0].spt[0];
BasicSectionInfor[0].EpointToE = GJDInforArray2[0].spt[1];

BasicSectionInfor[0].GJDType = -1; //���ö�
//

int Num;
//2.2.ѭ������,�γ��������öκ͸Ľ���,�õ������ֶ���ϢBasicSectionInfor
for(int i=0;i<SizeOfGJDB;i++)
{
//�Ľ��ηֶ���Ϣ
Num=i*2+1;
//	_tcscpy(BasicSectionInfor[Num].ToOutSGJJDNum ,GJDInforArray[i].gjdname);

_tcscpy(BasicSectionInfor[Num].BXLName ,GJDInforArray2[i].BXLName);
_tcscpy(BasicSectionInfor[Num].EXLName ,GJDInforArray2[i].EXLName);

BasicSectionInfor[Num].BpointToN = GJDInforArray2[i].spt[0];
BasicSectionInfor[Num].BpointToE = GJDInforArray2[i].spt[1];

BasicSectionInfor[Num].EpointToN = GJDInforArray2[i].ept[0];
BasicSectionInfor[Num].EpointToE = GJDInforArray2[i].ept[1];

BasicSectionInfor[Num].GJDType = GJDInforArray2[i].GJDType;

//���öηֶ���Ϣ
if (i<SizeOfGJDB-1)
{

_tcscpy(BasicSectionInfor[Num+1].BXLName ,GJDInforArray2[i].EXLName);
_tcscpy(BasicSectionInfor[Num+1].EXLName ,GJDInforArray2[i+1].BXLName);

BasicSectionInfor[Num+1].BpointToN = GJDInforArray2[i].ept[0];
BasicSectionInfor[Num+1].BpointToE = GJDInforArray2[i].ept[1];

BasicSectionInfor[Num+1].EpointToN = GJDInforArray2[i+1].spt[0];
BasicSectionInfor[Num+1].EpointToE = GJDInforArray2[i+1].spt[1];

BasicSectionInfor[Num+1].GJDType = -1;
}
}


//2.3���⴦��,�γ����һ�����ö�

_tcscpy(BasicSectionInfor[SizeOfGJDB*2].BXLName ,GJDInforArray2[SizeOfGJDB-1].EXLName);
_tcscpy(BasicSectionInfor[SizeOfGJDB*2].EXLName ,GJDInforArray2[SizeOfGJDB-1].EXLName);

BasicSectionInfor[SizeOfGJDB*2].BpointToN = GJDInforArray2[SizeOfGJDB-1].ept[0]; ;
BasicSectionInfor[SizeOfGJDB*2].BpointToE = GJDInforArray2[SizeOfGJDB-1].ept[1]; ;

BasicSectionInfor[SizeOfGJDB*2].EpointToN = JDarray[JDarray.GetSize()-1].N;
BasicSectionInfor[SizeOfGJDB*2].EpointToE = JDarray[JDarray.GetSize()-1].E;

BasicSectionInfor[SizeOfGJDB*2].GJDType = -1; //���ö�
}

*/


//�����ͨ��N,E���꣬����õ���ֳ����
CString GTZX2_JYX_ROAD::GetGTX2Xclc(double N,double E)
{

	CString CurCalKml;
	//�ж��Ƿ�Ϊ����/����
	int CurRxdNun = GetiRXDFromNE(N,E);

	double Curcml; 	//�������
	if(CurRxdNun>=0)//���ж�
	{
		//int ndl;
		//ndl = NDL;
		//NDL = CreateGtzxDLB();
		CurCalKml = GetRXDGTXXclc(N,E);
		//		NDL = ndl;
	}
	else //���ж�
	{
		//	ads_printf(L"���ж�\n");
		//2 ���У�
		if (GetGT2xGJDIndex(N,E)==-1)//2.1���У����ü�����
		{
			double cml,dml;
			ACHAR GH[20],ckml[80];
			cml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,N,E,N,E);
			cml = JiXianpGtzx->CalnewcmlTocml(cml);
			dml = JiXianpGtzx->XLC1(cml,GH,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
			_tcscpy(ckml,JiXianpGtzx->LCchr(GH,dml,3,1));
			CurCalKml = ckml;
		}
		else//2.2���У������ü�����
		{
			CurCalKml = m_pm1->GetGTXXclc(N,E);
		}
	}


	return CurCalKml;

}





/////
/*
//�����ͨ��N,E���꣬����õ���ֳ����
CString GTZX::GetGTXXclc(double N,double E)
{

//1������N,E�ڹ�ͨ�ߵ�ͶӰ���;
CString xclc;
double tylc;
double Bcml,Ecml;
ACHAR gh[10],ckml[30];
tylc = PROJ_ML(array,tarray,N,E,N,E);
//2.  �б�N,E�����ڵ����Σ����öλ�Ľ��Σ�
for(int i=0; i<BasicSectionInfor.GetSize(); i++)
{
Bcml = PROJ_ML(array,tarray,BasicSectionInfor[i].BpointToN ,BasicSectionInfor[i].BpointToE,BasicSectionInfor[i].BpointToN ,BasicSectionInfor[i].BpointToE);
Ecml = PROJ_ML(array,tarray,BasicSectionInfor[i].EpointToN ,BasicSectionInfor[i].EpointToE,BasicSectionInfor[i].EpointToN ,BasicSectionInfor[i].EpointToE);

if(i==0 && tylc<Bcml-0.001)
{
xclc = JiXianpGtzx->LCchr(JiXianpGtzx->DLB[0].EGH,JiXianpGtzx->DLB[0].BLC+JiXianpGtzx->DLB[0].DL,3,1);
return xclc;
}

if(i==BasicSectionInfor.GetSize()-1 && tylc>Ecml+0.001)
{
xclc = JiXianpGtzx->LCchr(JiXianpGtzx->DLB[JiXianpGtzx->NDL-1].BGH,JiXianpGtzx->DLB[JiXianpGtzx->NDL-1].BLC,3,1);
return xclc;
}


if(tylc>Bcml-0.001 && tylc<Ecml+0.001)
break;
}

double cml;
if(i>=0 && i<BasicSectionInfor.GetSize())//�ҵ���i�ֶ�
{
// 3.  ��Ϊ���öλ��иĽ��Σ����ݼ����߼���õ��ֳ����
if(BasicSectionInfor[i].GJDType!=2)
{

CString XCLC2x;
GTZX2 *pm2=NULL;
double Curcml;

//2.��ͬһ�ֳ���̶�Ӧ������������
if(_tcscmp( BasicSectionInfor[i].BXLName , L"���л���")==0) //����
{
Curcml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,N,E,N,E);
xclc = JiXianpGtzx->XLC(Curcml);
}
else  //��������
{
if(_tcscmp( BasicSectionInfor[i].BXLName,L"��������")==0)
pm2 = ZuoXianpGtzx;
else
pm2 = YouXianpGtzx;

//ͬһ���ֳ���̣��ڼ����������Ӧ����
if(pm2)
{
Curcml = pm2->PROJ_ML(pm2->array,pm2->tarray,N,E,N,E);;//��iRXD��0��ָ�����жβ���
double Curdml;

Curdml = pm2->XCLC(Curcml,gh, pm2->DLB2x, pm2->NDL);
_tcscpy(ckml,L"\0");
_tcscpy(ckml, LCchr(gh,Curdml,3,1));
xclc = ckml;
}
else
{
cml = PROJ_ML(array,tarray,N,E,N,E);
xclc = XLC(cml);
}

}

}
else// ������Ϊ���иĽ��Σ�ȡ�����иĽ���GTZXָ�����õ��ֳ����
{

GTZX * pm1=NULL;
pm1 = Get_GTX_RXGJD_GTZX(BasicSectionInfor[i]);


//�����ӣǣԣڣ��УΣ�����
if(pm1)
{
double cml;
cml = pm1->PROJ_ML(pm1->array,pm1->tarray,N,E,N,E);
xclc = pm1->XLC(cml);
}
else
{
cml = PROJ_ML(array,tarray,N,E,N,E);
xclc = XLC(cml);

}


}
}
else
{
cml = PROJ_ML(array,tarray,N,E,N,E);
xclc = XLC(cml);

}


return xclc;

}
*/

//�������߱���ֳ����
void GTZX2_JYX_ROAD::CalCKQXBToKml(CArray<QXBStruct,QXBStruct>& CKQXB)
{
	ModifyOriExJDXY();

	CalJDdata1();

	TagEXJDIsShow();

	SetJDdata();

	getpxy_DATA();


	//��QXBѭ��
	double qxbZDcml;
	CKQXB.RemoveAll();
	CString CalLCString;
	CKQXB.SetSize(QXB.GetSize());
	//�����ֳ����
	for(int i=0;i<QXB.GetSize();i++)
	{
		//�ȸ���ֵ
		CKQXB[i] = QXB[i];

		for(int j=0;j<4;j++)
		{
			if(j==0)qxbZDcml = CKQXB[i].KZH ;
			else if(j==1)qxbZDcml = CKQXB[i].KHY ;
			else if(j==2)qxbZDcml = CKQXB[i].KYH ;
			else if(j==3)qxbZDcml = CKQXB[i].KHZ ;

			CalLCString = GetGTX2XclcFromcml(qxbZDcml);

			if(j==0)_tcscpy(CKQXB[i].CKZH , CalLCString );
			else if(j==1)_tcscpy(CKQXB[i].CKHY , CalLCString );
			else if(j==2)_tcscpy(CKQXB[i].CKYH , CalLCString );
			else if(j==3)_tcscpy(CKQXB[i].CKHZ , CalLCString );
		}
	}
}


void GTZX2_JYX_ROAD::CalCKQXBToKml()
{
	//��QXBѭ��
	double qxbZDcml;
	CString CalLCString;
	//�����ֳ����
	for(int i=0;i<QXB.GetSize();i++)
	{
		for(int j=0;j<4;j++)
		{
			if(j==0)qxbZDcml = QXB[i].KZH ;
			else if(j==1)qxbZDcml = QXB[i].KHY ;
			else if(j==2)qxbZDcml = QXB[i].KYH ;
			else if(j==3)qxbZDcml = QXB[i].KHZ ;

			CalLCString = GetGTX2XclcFromcml(qxbZDcml);

			if(j==0)_tcscpy(QXB[i].CKZH , CalLCString );
			else if(j==1)_tcscpy(QXB[i].CKHY , CalLCString );
			else if(j==2)_tcscpy(QXB[i].CKYH , CalLCString );
			else if(j==3)_tcscpy(QXB[i].CKHZ , CalLCString );
		}
	}
}

CString GTZX2_JYX_ROAD::GetGTX2XclcFromcml(double cml)
{
	xlpoint PZ;
	CString CalKml;
	PZ.lc = cml ;
	//����Σ�����
	xlpoint_pz(array,tarray,&PZ);

	//�����ֳ����
	CalKml = GetGTX2Xclc(PZ.x,PZ.y) ;
	return CalKml;
}


/////////////////////////////////


////zj 07.7.4
//GTZX* GTZX2::GetiRxdtoGTZXplinp(int RXDno)
//{
//	//1.����GetiRXDJDArray(int iRXD, double jday[][6], int &njd)���õ����жν���
//	int njd;
//	double jday[MAXJDNUM][6];
//	GetiRXDJDArray(RXDno, jday, njd);
//	CArray<GTZX::JDarrayStruct,GTZX::JDarrayStruct>array;
//	array.SetSize(njd);
//	for(int i=0;i<njd;i++)
//	{
//		_tcscpy(array[i].JDNum ,L"i");
//		array[i].N = jday[i][1];
//		array[i].E = jday[i][2];
//		array[i].l1 = jday[i][3];
//		array[i].R = jday[i][4];
//		array[i].l2 = jday[i][5];
//	}
//
//	//2.2�õ����жζ���
//	int iSt,iEd;
//	GetiRXDDLinfo(RXDno,iSt,iEd);
//
//
//	//3.�������жε�GTZX
//	ACHAR GH[8],JDGH[8];
//	if (njd< 2) return NULL;
//	_tcscpy(GH,L"AK"); 
//	_tcscpy(JDGH,L"JD");
//	GTZX *pzx= new GTZX(njd,array,&GH[0],&JDGH[0]);
//	pzx->setINIT(3,3,1,5);
//	pzx->mode_display=0;
//	//pzx->mdbname = WayOfMDB;
//
//	pzx->NDL = iEd-iSt+1;
//	for(i=iSt;i<=iEd;i++)
//	{
//		pzx->setDLM(i,DLB[i].BLC,DLB[i].DL,DLB[i].BGH,DLB[i].EGH);
//	}
//
//	pzx->getpxy_DATA();
//
//	return pzx;
//
//}

//�������ж����õ����Զα�� zj 07.7.15
int GTZX2_JYX_ROAD::GetiRxdNoFromRXDName(CString RXDName)
{
	//	int iRXD;
	int FirstDL,EndDL;//ĳһ���ж���ʼ�����
	FirstDL=EndDL=-1;
	int RxdNum=GetRXDNum();
	for(int i=0;i<RxdNum;i++)
	{
		GetiRXDDLinfo(i,FirstDL,EndDL);
		if (FirstDL!=-1 && EndDL!=-1)
		{	
			if( _tcscmp(RXDName,DLB2x[FirstDL].RXDName)==0 )
			{
				return i;
			}		
		}
	}
	ads_printf(L"\n�Ҳ���%s�����жα��",RXDName);
	return -1;

}


int GTZX2_JYX_ROAD::GetiRxdNoFromTYLC2x(double lc2x)
{//zj07.7.6
	//	int iRXD;
	int FirstDL,EndDL;//ĳһ���ж���ʼ�����
	FirstDL=EndDL=-1;
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

void GTZX2_JYX_ROAD::GetiRXDJDArray(int iRXD, double jday[][6], int &njd,int &SJDNum,int &EJDNum)//zj
{
	int sjd=0,ejd=0,i=0,j=0,sdl=0,edl=0,ndl=0;
	njd=0;
	//	EXDLB *iRxdDlb;
	GetiRXDDLinfo(iRXD,sdl,edl);//��ȡ���ն�����

	xlpoint PZ1,PZ2;	//�������յ�������	
	PZ1.lc=DLB2x[sdl].TYLC;
	PZ2.lc=DLB2x[edl].TYLC;

	xlpoint_pz(array,tarray,&PZ1);//������
	xlpoint_pz(array,tarray,&PZ2);

	GetJDXHofiRXD(iRXD,sjd,ejd);//��ȡ���ս����
	//	acutPrintf(L"sjd=%d,ejd=%d\n",sjd,ejd);
	SJDNum=sjd;
	EJDNum=ejd;

	njd=ejd-sjd+1;//������
	njd+=2;//�����������յ�
	//	jday=new double[njd][6];

	for(i=1;i<njd-1;i++)//1->njd-1
	{
		jday[i][0]=1.0*i;
		//	for(j=1;j<6;j++)jday[i][j]=JDarray[sjd+i-1][j];//sjd+i-1,��sjd��ʼ
		jday[i][1]=JDarray[sjd+i-1].N;
		jday[i][2]=JDarray[sjd+i-1].E;
		jday[i][3]=JDarray[sjd+i-1].l1;
		jday[i][4]=JDarray[sjd+i-1].R;
		jday[i][5]=JDarray[sjd+i-1].l2;
	}
	//���ж������Ϊ��һ������
	i=0;
	jday[i][0]=i;
	jday[i][1]=PZ1.x;
	jday[i][2]=PZ1.y;
	jday[i][3]=DLB2x[sdl].ELC;//��ʼ���
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



//��ΰ2007-07-28
//�����ͨ��N,E���꣬����õ���ֳ����
CString GTZX2_JYX_ROAD::GetRXDGTXXclc(double N,double E)
{

	//1������N,E�ڹ�ͨ�ߵ�ͶӰ���;
	CString xclc;
	double tylc;
	double Bcml,Ecml;
	ACHAR gh[10],ckml[30];
	tylc = PROJ_ML(array,tarray,N,E,N,E);
	//2.  �б�N,E�����ڵ����Σ����öλ�Ľ��Σ�
	int i;
	for(i=0; i<BasicSectionInfor.GetSize(); i++)
	{
		Bcml = PROJ_ML(array,tarray,BasicSectionInfor[i].BpointToN ,BasicSectionInfor[i].BpointToE,BasicSectionInfor[i].BpointToN ,BasicSectionInfor[i].BpointToE);
		Ecml = PROJ_ML(array,tarray,BasicSectionInfor[i].EpointToN ,BasicSectionInfor[i].EpointToE,BasicSectionInfor[i].EpointToN ,BasicSectionInfor[i].EpointToE);

		//	ads_printf(L"->>>>>   i=%d,BpointToN=%lf,EpointToN=%lf,tylc=%lf\n",i,BasicSectionInfor[i].BpointToN,BasicSectionInfor[i].EpointToN,tylc);

		if(i==0 && tylc<Bcml-0.001)
		{
			xclc = JiXianpGtzx->LCchr(JiXianpGtzx->DLB[0].EGH,JiXianpGtzx->DLB[0].BLC+JiXianpGtzx->DLB[0].DL,3,1);
			return xclc;
		}

		if(i==BasicSectionInfor.GetSize()-1 && tylc>Ecml+0.001)
		{
			xclc = JiXianpGtzx->LCchr(JiXianpGtzx->DLB[JiXianpGtzx->NDL-1].BGH,JiXianpGtzx->DLB[JiXianpGtzx->NDL-1].BLC,3,1);
			return xclc;
		}


		if(tylc>Bcml-0.001 && tylc<Ecml+0.001)
			break;
	}

	double cml;
	if(i>=0 && i<BasicSectionInfor.GetSize())//�ҵ���i�ֶ�
	{
		// 3.  ��Ϊ���öλ��иĽ��Σ����ݼ����߼���õ��ֳ����
		GJDAllInforStruct mystruct;
		mystruct=BasicSectionInfor[i];
		if(BasicSectionInfor[i].GJDType!=2)
		{

			CString XCLC2x;
			GTZX2_JYX_ROAD *pm2=NULL;
			double Curcml;

			//2.��ͬһ�ֳ���̶�Ӧ������������
			if(_tcscmp( BasicSectionInfor[i].BXLName , L"���л���")==0) //����
			{
				Curcml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,N,E,N,E);
				Curcml = JiXianpGtzx->CalnewcmlTocml(Curcml);
				xclc = JiXianpGtzx->XLC(Curcml);
			}
			else  //��������
			{
				if(_tcscmp( BasicSectionInfor[i].BXLName,L"��������")==0)
					pm2 = ZuoXianpGtzx;
				else
					pm2 = YouXianpGtzx;

				//ͬһ���ֳ���̣��ڼ����������Ӧ����
				if(pm2)
				{
					Curcml = pm2->PROJ_ML(pm2->array,pm2->tarray,N,E,N,E);;//��iRXD��0��ָ�����жβ���
					double Curdml;

					Curdml = pm2->XCLC(Curcml,gh, pm2->DLB2x, pm2->NDL);
					_tcscpy(ckml,L"\0");
					_tcscpy(ckml, LCchr(gh,Curdml,3,1));
					xclc = ckml;
				}
				else
				{
					cml = PROJ_ML(array,tarray,N,E,N,E);
					xclc = XLC(cml);
				}

			}


			if (pm2)
			{
				delete pm2;
				pm2 = NULL;
			}

		}
		else// ������Ϊ���иĽ��Σ�ȡ�����иĽ���GTZXָ�����õ��ֳ����
		{

			GTZX_JYX_ROAD * pm1=NULL;
			//pm1 = Get_GTX_RXGJD_GTZX(BasicSectionInfor[i]);
			int irxd;
			irxd = GetiRXDFromNE(N,E);
			pm1 = GetiRxdtoGTZXplinp(irxd);

			//�����ӣǣԣڣ��УΣ�����
			if(pm1)
			{
				double cml;
				cml = pm1->PROJ_ML(pm1->array,pm1->tarray,N,E,N,E);
				cml = pm1->CalnewcmlTocml(cml);
				xclc = pm1->XLC(cml);
			}
			else
			{
				cml = PROJ_ML(array,tarray,N,E,N,E);
				xclc = XLC(cml);

			}

			if (pm1)
			{
				delete pm1;
				pm1 = NULL;
			}


		}
	}
	else
	{
		cml = PROJ_ML(array,tarray,N,E,N,E);
		xclc = XLC(cml);

	}

	return xclc;

}
//��ΰ2007-07-28

//�õ���ͨ���2�߸Ľ��ֶ�����
void GTZX2_JYX_ROAD::GetGTXSJ2gjdInforArray(GTZX_JYX_ROAD *pGt1x , CArray<GJDInforStruct,GJDInforStruct&>& GJDInforArrayForGTXSJ2)
{

	//��һ�� �õ���ͨ���2������
	//��ͨ���2��
	CArray<GJDInforStruct,GJDInforStruct&>GJDSJ2xInforArray;
	GJDSJ2xInforArray.RemoveAll();

	int GtzxGJDNum = pGt1x->GJDInforArray.GetSize() ;  
	GJDSJ2xInforArray.SetSize(GtzxGJDNum);

	double CurCml =0.0;
	double CurN,CurE;
	xlpoint PZ;

	for(int i=0;i<=GtzxGJDNum;i++ )
	{
		if(i<GtzxGJDNum)
		{
			CurN = pGt1x->GJDInforArray[i].spt[0];
			CurE = pGt1x->GJDInforArray[i].spt[1];
		}
		else
		{
			//�յ㵥������
			CurN = pGt1x->GJDInforArray[i].ept[0];
			CurE = pGt1x->GJDInforArray[i].ept[1];
		}


		//�����������
		CurCml = PROJ_ML(array,tarray,CurN,CurE,CurN,CurE);
		PZ.lc = CurCml;
		//�����ͨ2���ϵ�����
		xlpoint_pz_GTX(array,tarray,&PZ);

		//����ͨ���2�ߵ�����
		if(i<GtzxGJDNum)
		{
			//���
			GJDSJ2xInforArray[i].spt[0] = PZ.x;
			GJDSJ2xInforArray[i].spt[1] = PZ.y;
		}
		else
		{
			//�����յ�����
			for(int k=0;k<GtzxGJDNum-1;k++)
			{
				//�˴��յ�Ϊǰһ���
				GJDSJ2xInforArray[k].ept[0] = GJDSJ2xInforArray[k+1].spt[0];
				GJDSJ2xInforArray[k].ept[1] = GJDSJ2xInforArray[k+1].spt[1];
			}

			//�յ�
			i=GtzxGJDNum;
			GJDSJ2xInforArray[i].ept[0] = PZ.x;
			GJDSJ2xInforArray[i].ept[1] = PZ.y;
		}

		if(i<GtzxGJDNum)
		{
			//�Ľ������� 0-���� 1-S��   // -�� - ��ȫ����
			GJDSJ2xInforArray[i].GJDType = pGt1x->GJDInforArray[i].GJDType;

			//����Ľ�����ʼ/��ֹ��·��(��)
			_tcscpy( GJDSJ2xInforArray[i].BXLName , pGt1x->GJDInforArray[i].BXLName );
			_tcscpy( GJDSJ2xInforArray[i].EXLName , pGt1x->GJDInforArray[i].EXLName );
		}
	}

	//�ڶ��� ϸ����ͨ���2������
	CArray<GJDInf,GJDInf&> NewGJDInforArrayForGTXSJ2 ;

	GetGTXSJ2Allgjdinfor(GJDSJ2xInforArray , NewGJDInforArrayForGTXSJ2);

	////�ԸĽ���GJDFDInfTab�������¸�ֵ
	//GJDFDInfTab.RemoveAll();

	//GJDFDInfTab.SetSize(GtzxGJDNum);
	//for(int i=0;i<GtzxGJDNum;i++)
	//{
	//	_tcscpy( GJDFDInfTab[i].XLName , GJDSJ2xInforArray[i].EXLName );
	//	GJDFDInfTab[i].ept[0] = GJDSJ2xInforArray[i].ept[0];
	//	GJDFDInfTab[i].ept[1] = GJDSJ2xInforArray[i].ept[1];
	//}

}

//�õ�ϸ����ĸĽ���ָ��(���߸�ʽ)
GTZX_JYX_ROAD  *GTZX2_JYX_ROAD::GetOneGTXSJ2TopLinep(GJDInforStruct CurGJDInforToIn )
{
	//ת����
	GTZX_JYX_ROAD *CurGTLinp = NULL;
	CurGTLinp = CreateGtzx();

	CArray<JDarrayStruct,JDarrayStruct>SubNewJDarray;

	CArray<DLBZ,DLBZ>SubNewDLB;
	int SubNewNDL;


	double CurN,CurE;
	xlpoint PZ;
	double CurBCml , CurECml;


	CurN = CurGJDInforToIn.spt[0];
	CurE = CurGJDInforToIn.spt[1];

	//�����������
	CurBCml = PROJ_ML(array,tarray,CurN,CurE,CurN,CurE);

	CurN = CurGJDInforToIn.ept[0];
	CurE = CurGJDInforToIn.ept[1];
	CurECml = PROJ_ML(array,tarray,CurN,CurE,CurN,CurE);



	///

	//ǰ(��)�����±�
	int BJDNum, EJDNum ;

	//������㽻���±�


	BJDNum =CurGTLinp->GetPreOrAftJD(CurGJDInforToIn.spt[0],CurGJDInforToIn.spt[1],FALSE);

	//�յ���㽻���±�

	double Endcml;
	Endcml = CurGTLinp->DLB[0].BLC + CurGTLinp->DLB[0].DL + CurGTLinp->xl_length;

	if(fabs(CurECml-Endcml)<1)EJDNum=CurGTLinp->GetPreOrAftJD(CurGJDInforToIn.ept[0],CurGJDInforToIn.ept[1],FALSE);//���һ�����öΣ��յ�ȡ��·�յ�
	else EJDNum =CurGTLinp->GetPreOrAftJD(CurGJDInforToIn.ept[0],CurGJDInforToIn.ept[1],true);

	//4.����BJDNum,EJDNum��ϳɡ�LYDJDarray


	for(int j=BJDNum;j<=EJDNum;j++)
	{
		SubNewJDarray.Add(CurGTLinp->JDarray[j]);
	}

	//�õ����öζ�������
	SubNewNDL = 0;
	double cml;


	for(int i=0;i<CurGTLinp->NDL;i++)
	{
		cml = CurGTLinp->PROJ_ML(CurGTLinp->array,CurGTLinp->tarray,CurGTLinp->DLB[i].N,CurGTLinp->DLB[i].E,
			CurGTLinp->DLB[i].N,CurGTLinp->DLB[i].E);

		if(cml>CurBCml-0.001 && cml<CurECml+0.001)
		{
			_tcscpy(SubNewDLB[SubNewNDL].BGH , CurGTLinp->DLB[i].BGH);
			SubNewDLB[SubNewNDL].BLC = CurGTLinp->DLB[i].BLC;
			_tcscpy(SubNewDLB[SubNewNDL].EGH , CurGTLinp->DLB[i].EGH);
			SubNewDLB[SubNewNDL].DL = CurGTLinp->DLB[i].DL;

			SubNewNDL++;
		}
	}	

	//ͷβ��������
	DLBZ DLBTempForfirst , DLBTempForLast;

	ACHAR ckml[30],gh[8];
	double xlc;
	CString CKml = GetGTX2Xclc(CurGJDInforToIn.spt[0],CurGJDInforToIn.spt[1]);
	_tcscpy(ckml,CKml);
	split_ckml(ckml,&xlc,gh);
	_tcscpy( DLBTempForfirst.BGH , gh );
	_tcscpy( DLBTempForfirst.EGH , gh );
	DLBTempForfirst.BLC = xlc ;
	DLBTempForfirst.DL = 0 ;

	DLBTempForfirst.ELC = DLBTempForfirst.BLC + DLBTempForfirst.DL ;

	//�յ㴦��

	CKml = GetGTX2Xclc(CurGJDInforToIn.ept[0],CurGJDInforToIn.ept[1]);
	_tcscpy(ckml,CKml);
	split_ckml(ckml,&xlc,gh);
	_tcscpy( DLBTempForLast.BGH , gh );
	_tcscpy( DLBTempForLast.EGH , gh );
	DLBTempForLast.BLC = xlc ;
	DLBTempForLast.DL = 0 ;

	DLBTempForLast.ELC = DLBTempForLast.BLC + DLBTempForLast.DL ;

	//���ͷβ������������
	SubNewDLB.InsertAt(0,DLBTempForfirst,1);
	SubNewDLB.Add(DLBTempForLast);

	ACHAR GH[8],JDGH[8];

	_tcscpy(GH,L"AK"); 
	_tcscpy(JDGH,L"JD");
	GTZX_JYX_ROAD *SubNewGtzx= new GTZX_JYX_ROAD(NJD,SubNewJDarray,&GH[0],&JDGH[0]);

	for (int i=0; i<SubNewNDL; i++)
	{
		SubNewGtzx->setDLM(i,SubNewDLB[i].BLC,SubNewDLB[i].DL,SubNewDLB[i].BGH,SubNewDLB[i].EGH);
	}


	return SubNewGtzx; 

}

void GTZX2_JYX_ROAD::GetGTXSJ2Allgjdinfor(CArray<GJDInforStruct,GJDInforStruct&>& GJDInforArrayForGTXSJ2ToIn , CArray<GJDInf,GJDInf&>& GJDInforArrayForGTXSJ2ToOut)
{

	//GJDInforStruct  CurGJDInfor;
	//GJDInforArrayForGTXSJ2ToOut.RemoveAll();
	//GTZX_JYX *pSubSelectGtzx;
	////�ԸĽ����������ѭ��,
	//for (int i=0;i<GJDInforArrayForGTXSJ2ToIn.GetSize();i++)
	//{
	//	//����ÿ���Ľ���,����ָ��
	//	CurGJDInfor = GJDInforArrayForGTXSJ2ToIn.GetAt(i);

	//	pSubSelectGtzx = NULL;

	//	pSubSelectGtzx = GetOneGTXSJ2TopLinep(CurGJDInfor);
	//	//�ֱ��3��������ƥ��,�õ�ϸ����ĸĽ�������


	//	//�������ָ�붼Ϊ�գ����¶����ݿ⣬��������ָ�루���뷽����
	//	if(JiXianpGtzx==NULL && ZuoXianpGtzx==NULL && YouXianpGtzx==NULL)
	//	{

	//		UltraCArray_T<CConnectLineGTZX::ConnectLineGTZXStruct ,CConnectLineGTZX::ConnectLineGTZXStruct&>  m_AllGJDStructArray;

	//		//�õ��ļ���
	//		CPMSJ CurPMSJtemp;
	//		CurPMSJtemp.GetCurFileName();

	//		GTZX_JYX * ZuoXianpGtzxLinp =NULL , * JiXianpGtzxLinp =NULL, * YouXianpGtzxLinp=NULL;

	//		GTZX_JYX * ZuoXianpGtzxLinpJYX = NULL , * YouXianpGtzxLinpToJYX = NULL;

	//		GTZX2_JYX *  ZuoGTZX2pLinp = NULL , * YouGTZX2pLinp = NULL ;

	//		GTZX_JYX * JiXianpGtzxLinpToJYX ; 


	//		GTZX2_JYX *ZuoJYXGTZX2pLinp ,* YouJYXGTZX2pLinp ;
	//		GTZX2_JYX * JiXianGTZX2pLinp;


	//		//�õ������ָ��
	//		CurPMSJtemp.GetSheJiXianpGTZX(ZuoXianpGtzxLinp,JiXianpGtzxLinp,YouXianpGtzxLinp,
	//			ZuoXianpGtzxLinpJYX,JiXianpGtzxLinpToJYX,YouXianpGtzxLinpToJYX,ZuoGTZX2pLinp,YouGTZX2pLinp,JiXianGTZX2pLinp,ZuoJYXGTZX2pLinp,YouJYXGTZX2pLinp);

	//		JiXianpGtzx = JiXianpGtzxLinpToJYX ;
	//		ZuoXianpGtzx = ZuoJYXGTZX2pLinp ; 
	//		YouXianpGtzx = YouJYXGTZX2pLinp;
	//	}



	//	//���ڼ����ͨ��ƻ��ߵļ�����ָ��
	//	if(JiXianpGtzx!=NULL)
	//	{

	//		if(pSubSelectGtzx)
	//			JiXianpGtzx->CreatGJDInfTab(pSubSelectGtzx);
	//		else
	//		{
	//			ads_printf(L"����ƽ������Ϊ��!\n");
	//		}

	//		for(int i=0;i<JiXianpGtzx->GJDInfTab.GetSize();i++)
	//		{
	//			_tcscpy( JiXianpGtzx->GJDInfTab[i].BXLName , L"���л���");
	//			_tcscpy( JiXianpGtzx->GJDInfTab[i].EXLName , L"���л���");

	//			GJDInforArrayForGTXSJ2ToOut.Add(JiXianpGtzx->GJDInfTab[i]);
	//		}

	//	}


	//	if(ZuoXianpGtzx!=NULL)
	//	{
	//		GTZX_JYX *ZuoXianpGtzxToGtzx;
	//		ZuoXianpGtzxToGtzx = ZuoXianpGtzx->CreateGtzx();
	//		if(pSubSelectGtzx)
	//			ZuoXianpGtzxToGtzx->CreatGJDInfTab(pSubSelectGtzx);
	//		else
	//		{
	//			ads_printf(L"�������߹�ͨ��ƻ���ƽ������Ϊ��!\n");
	//		}

	//		for(int i=0;i<ZuoXianpGtzx->GJDInfTab.GetSize();i++)
	//		{
	//			_tcscpy( ZuoXianpGtzx->GJDInfTab[i].BXLName , L"��������");
	//			_tcscpy( ZuoXianpGtzx->GJDInfTab[i].EXLName , L"��������");

	//			GJDInforArrayForGTXSJ2ToOut.Add(ZuoXianpGtzx->GJDInfTab[i]);
	//		}
	//	}

	//	if(YouXianpGtzx!=NULL)
	//	{
	//		GTZX_JYX *YouXianpGtzxToGtzx;
	//		YouXianpGtzxToGtzx = YouXianpGtzx->CreateGtzx();

	//		if(pSubSelectGtzx)
	//			YouXianpGtzxToGtzx->CreatGJDInfTab(pSubSelectGtzx);
	//		else
	//		{
	//			ads_printf(L"�������߹�ͨ��ƻ���ƽ������Ϊ��!\n");
	//		}

	//		for(int i=0;i<YouXianpGtzx->GJDInfTab.GetSize();i++)
	//		{
	//			_tcscpy( YouXianpGtzx->GJDInfTab[i].BXLName , L"��������");
	//			_tcscpy( YouXianpGtzx->GJDInfTab[i].EXLName , L"��������");

	//			GJDInforArrayForGTXSJ2ToOut.Add(YouXianpGtzx->GJDInfTab[i]);
	//		}

	//	}

	//	/*
	//	//������ƻ�����������߱Ƚ�
	//	if(ZuoXianpGtzxLinpJYX!=NULL)
	//	{
	//	ZuoXianpGtzxLinpJYX->assertWriteEnabled();
	//	ZuoXianGJDInfTabArray.RemoveAll();

	//	if(ZuoXianpGtzxLinp)
	//	ZuoXianpGtzxLinpJYX->CreatGJDInfTab(ZuoXianpGtzxLinp);
	//	else
	//	{
	//	ads_printf(L"�������߹�ͨ��ƻ���ƽ������Ϊ��!\n");
	//	}

	//	for(int i=0;i<ZuoXianpGtzxLinpJYX->GJDInfTab.GetSize();i++)
	//	{
	//	_tcscpy( ZuoXianpGtzxLinpJYX->GJDInfTab[i].BXLName , L"��������");
	//	_tcscpy( ZuoXianpGtzxLinpJYX->GJDInfTab[i].EXLName , L"��������");

	//	ZuoXianGJDInfTabArray.Add(ZuoXianpGtzxLinpJYX->GJDInfTab[i]);
	//	}
	//	}

	//	// ������ƻ�������߱Ƚ�
	//	if(JiXianpGtzxLinpToJYX!=NULL)
	//	{
	//	JiXianpGtzxLinpToJYX->assertWriteEnabled();
	//	JiXianGJDInfTabArray.RemoveAll();
	//	if(JiXianpGtzxLinp)
	//	JiXianpGtzxLinpToJYX->CreatGJDInfTab(JiXianpGtzxLinp);
	//	else
	//	{
	//	ads_printf(L"���߹�ͨ��ƻ���ƽ������Ϊ��!\n");
	//	}


	//	for(int i=0;i<JiXianpGtzxLinpToJYX->GJDInfTab.GetSize();i++)
	//	{

	//	_tcscpy( JiXianpGtzxLinpToJYX->GJDInfTab[i].BXLName , L"���л���");
	//	_tcscpy( JiXianpGtzxLinpToJYX->GJDInfTab[i].EXLName , L"���л���");

	//	JiXianGJDInfTabArray.Add(JiXianpGtzxLinpToJYX->GJDInfTab[i]);
	//	}
	//	}

	//	//������ƻ�����������߱Ƚ�
	//	if(YouXianpGtzxLinpToJYX!=NULL)
	//	{
	//	YouXianpGtzxLinpToJYX->assertWriteEnabled();
	//	YouXianGJDInfTabArray.RemoveAll();

	//	if(YouXianpGtzxLinp)
	//	YouXianpGtzxLinpToJYX->CreatGJDInfTab(YouXianpGtzxLinp);
	//	else
	//	ads_printf(L"�������߹�ͨ��ƻ���ƽ������Ϊ��!\n");

	//	for(int i=0;i<YouXianpGtzxLinpToJYX->GJDInfTab.GetSize();i++)
	//	{
	//	_tcscpy( YouXianpGtzxLinpToJYX->GJDInfTab[i].BXLName , L"��������");
	//	_tcscpy( YouXianpGtzxLinpToJYX->GJDInfTab[i].EXLName , L"��������");

	//	YouXianGJDInfTabArray.Add(YouXianpGtzxLinpToJYX->GJDInfTab[i]);
	//	}
	//	}

	//	*/

	//}

}


/*
void GTZX2::AutoFindGTXSJ2gjdInforArray()
{
//1 �ɹ�ͨ��ƶ���ָ��ֱ�����л��ߣ��������ߣ��������ߡ��Ƚ�
//�ҵ����Ե����öηֶ�����1���͸Ľ��ηֶ�����2(���к�����)

//2 .��(���л��ߣ��������ߣ���������)���ö����� ����ͨ���2�ߵ������С�����������

//3 �ɸĽ���ͷβ��·����������̣�ȥ�Ľ��ηֶ�����2�в��ң����������������·�ϵģ���ɹ�ͨ���2��
//�Ľ��ηֶ����飬


}
*/

//�ж�Pt(N,E)���Ƿ��ڸĽ����ϣ��͸Ľ�������:-1-���� 0-���иĽ� 1-S��Ľ� 2-���иĽ�
//���Pt(N,E)�㲻����·�����������·�ϵ�ͶӰ������ж�
int GTZX2_JYX_ROAD::GetPtGJDType(double N, double E)
{
	if (GJDInforArray.GetSize()==0)
		return -1;
	double GJDSpt[2],GJDEpt[2];
	double GJDSml,GJDEml,ml;
	ml = PROJ_ML(array,tarray,N,E,N,E);
	for (int i=0 ; i<GJDInforArray.GetSize(); i++)
	{
		GJDSpt[0] = GJDInforArray[i].spt[0];
		GJDSpt[1] = GJDInforArray[i].spt[1];
		GJDEpt[0] = GJDInforArray[i].ept[0];
		GJDEpt[1] = GJDInforArray[i].ept[1];
		GJDSml = PROJ_ML(array,tarray,GJDSpt[0],GJDSpt[1],GJDSpt[0],GJDSpt[1]);
		GJDEml = PROJ_ML(array,tarray,GJDEpt[0],GJDEpt[1],GJDEpt[0],GJDEpt[1]);
		if (ml+0.0001>GJDSml && ml-0.0001<GJDEml)
		{
			return GJDInforArray[i].GJDType;
		}
	}
	return -1;
}

void  GTZX2_JYX_ROAD::GetiRxdStartEndMlByNum(int irxd,double &ProjSml,double &ProjEml)//520
{
	int FirstDL,EndDL;//ĳһ���ж���ʼ�����
	FirstDL=EndDL=-1;

	GetiRXDDLinfo(irxd,FirstDL,EndDL);
	ProjSml = DLB2x[FirstDL].TYLC;
	ProjEml = DLB2x[EndDL].TYLC;

}


/*************************************************************
��������: GetiRxdtoGTZXplinp_GTX
�������ܡ����ܵȵ�����: ��ȡ��ͨ��ƶ��������жΣ�������ת��Ϊ��ͨ��ƻ���ָ��
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��: 
irxd	���жα��
�����������˵��: ��
��������ֵ��˵��:  ת����Ĺ�ͨ��ƻ���ָ��
����˵��: 
***************************************************************/
GTZX_JYX_ROAD * GTZX2_JYX_ROAD::GetiRxdtoGTZXplinp_GTX(int irxd)
{
	double RxdSml,RxdEml;
	double GJDInfSml,GJDInfEml;
	double sml,eml;
	double NRxdStart,ERxdStrat;//���ж����N��E����
	double NRxdEnd,ERxdEnd;//���ж��յ�N��E����
	double cmlDL;//��������cml
	ACHAR XLCDL[80];//�������ֳ����
	xlpoint PZ;
	GTZX_JYX_ROAD* pZlinep=NULL;
	pZlinep = GetiRxdtoGTZXplinp(irxd);
	pZlinep->IsEXRXD = true;
	pZlinep->XLShuXing = "��ͨ��ƻ���";
	//for (int i=0 ; i<pZlinep->NDL ; i++)
	//{
	//	_tcscpy(XLCDL, LCchr(pZlinep->DLB[i].BGH,pZlinep->DLB[i].BLC,3,1));
	//	cmlDL = TYLC1(XLCDL,pZlinep->NDL,pZlinep->DLB);
	//	PZ.lc = cmlDL;
	//	xlpoint_pz(pZlinep->array,pZlinep->tarray,&PZ);
	//	pZlinep->DLB[i].N = PZ.x;
	//	pZlinep->DLB[i].E = PZ.y;
	//}
	//	RxdSml = pZlinep->QXB[0].KZH;

	ACHAR cXLC[40];


	_tcscpy(cXLC, LCchr(pZlinep->DLB[0].EGH,pZlinep->DLB[0].BLC + pZlinep->DLB[0].DL,4,1));
	RxdSml = TYLC1(cXLC,NDL,DLB);
	//	RxdEml = pZlinep->QXB[pZlinep->NJD-1].KHZ;
	_tcscpy(cXLC, LCchr(pZlinep->DLB[pZlinep->NDL-1].BGH,pZlinep->DLB[pZlinep->NDL-1].BLC,4,1));
	RxdEml = TYLC1(cXLC, NDL,DLB);
	NRxdStart = pZlinep->JDarray[0].N;
	ERxdStrat = pZlinep->JDarray[0].E;
	NRxdEnd = pZlinep->JDarray[pZlinep->NJD-1].N;
	ERxdEnd = pZlinep->JDarray[pZlinep->NJD-1].E;
	RxdSml = PROJ_ML(array, tarray, NRxdStart, ERxdStrat, NRxdStart, ERxdStrat);
	RxdEml = PROJ_ML(array, tarray, NRxdEnd, ERxdEnd, NRxdEnd, ERxdEnd);

	GJDAllInforStruct TempGJDInf;
	for (int i=0 ; i<BasicSectionInfor.GetSize(); i++)
	{
		TempGJDInf = BasicSectionInfor.GetAt(i);
		GJDInfSml = PROJ_ML(array, tarray, TempGJDInf.BpointToN, TempGJDInf.BpointToE, TempGJDInf.BpointToN, TempGJDInf.BpointToE);
		GJDInfEml = PROJ_ML(array, tarray, TempGJDInf.EpointToN, TempGJDInf.EpointToE, TempGJDInf.EpointToN, TempGJDInf.EpointToE);
		sml = max(RxdSml,GJDInfSml);
		eml = min(RxdEml,GJDInfEml);
		if (eml-sml>10.0)
		{
			if (fabs(sml-RxdSml)<0.001)
			{
				TempGJDInf.BpointToN = NRxdStart;
				TempGJDInf.BpointToE = ERxdStrat;
				_tcscpy(TempGJDInf.GJDBLC,pZlinep->LCchr(pZlinep->DLB[0].EGH,pZlinep->DLB[0].BLC+pZlinep->DLB[0].DL,3,1));
			}
			if (fabs(eml-RxdEml)<0.001)
			{
				TempGJDInf.EpointToN = NRxdEnd;
				TempGJDInf.EpointToE = ERxdEnd;
				_tcscpy(TempGJDInf.GJDELC,pZlinep->LCchr(pZlinep->DLB[pZlinep->NDL-1].BGH,pZlinep->DLB[pZlinep->NDL-1].BLC,3,1));
			}
			//if (fabs(GJDInfSml-sml)>0.001)
			//{
			//	PZ.lc = sml;
			//	xlpoint_pz(array,tarray,&PZ);
			//	TempGJDInf.BpointToN = PZ.x;
			//	TempGJDInf.BpointToE = PZ.y;
			//}
			//	if (fabs(GJDInfEml-eml)>0.001)
			//{			
			//	PZ.lc = eml;
			//	xlpoint_pz(array,tarray,&PZ);
			//	TempGJDInf.EpointToN = PZ.x;
			//	TempGJDInf.EpointToE = PZ.y;
			//}
			//			ads_printf(L"%d���ж�������=%s,�յ����=%s,�Ľ�����%d\n",irxd,TempGJDInf.GJDBLC,TempGJDInf.GJDELC,TempGJDInf.GJDType);
			//			AfxMessageBox(L"OK");

			pZlinep->BasicSectionInfor.Add(TempGJDInf);
		}
	}
	//if (pZlinep->BasicSectionInfor.GetSize()==1)
	//{
	//	pZlinep->BasicSectionInfor.RemoveAll();
	//}
	return pZlinep ;
};



/*************************************************************
��������: CalGTXXJJ
�������ܡ����ܵȵ�����: ��������ߵ��߼��
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��: 
cmlOnGT1X	���ڹ�ͨ��ƻ����ϵ�������̣��ɶ����ó���
pJYJX		���л���ָ��
pGTJX		��ͨ��ƻ���ָ��
pGTEX		��ͨ��ƶ���ָ��
�����������˵��: ��
��������ֵ��˵��:  �����߼��
����˵��: ��
***************************************************************/
double GTZX2_JYX_ROAD::CalGTXXJJ(double cmlOnGT1X, GTZX_JYX_ROAD *pJYJX, GTZX_JYX_ROAD* pGTJX, GTZX2_JYX_ROAD* pGTEX,double& XJJ1, double&XJJ2)
{
	xlpoint PZ;
	double LCJYJX;
	double TYLCGTJX;
	double TYLCGTEX;
	double resPt[2];
	double XJJ;
	PZ.lc=cmlOnGT1X;
	pGTJX->xlpoint_pz_GTX(pGTJX->array,pGTJX->tarray,&PZ);//�����ͨ��ƻ�����̴��������
	LCJYJX = pJYJX->PROJ_ML(pJYJX->array,pJYJX->tarray,PZ.x,PZ.y,PZ.x,PZ.y);//��lc2x��̵ĵ�����л���ͶӰ���

	CalXjj(LCJYJX, pJYJX->array, pJYJX->tarray, pGTJX->array, pGTJX->tarray, XJJ1, TYLCGTJX, resPt);//����lcjy1x��̴����л������ͨ��ƻ��ߵ��߼��
	CalXjj(LCJYJX, pJYJX->array, pJYJX->tarray, pGTEX->array, pGTEX->tarray, XJJ2, TYLCGTEX, resPt);//����lcjy1x��̴����л������ͨ��ƻ��ߵ��߼��

	XJJ = XJJ2 - XJJ1;
	return XJJ;
}



bool GTZX2_JYX_ROAD::AddMSRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray,CArray<QXBStruct,QXBStruct>& qxb,int njd,double N,double E,ACHAR EGH[])
{
	int CurRXDNum = 0;

	CurRXDNum =GetiRXDFromNE(jdarray[2].N,jdarray[2].E);

	if(CurRXDNum>=0)
	{
		//	ads_printf(L"û�ж�λ�����ж���!\n");
		return FALSE;
	}

	int firstJDXH = 0 , endJDXH = 0 ;
	GetJDXHofiRXD(CurRXDNum,firstJDXH, endJDXH);

	///����һ�����жν����
	DoubleLineData *OneRXDJDB=new DoubleLineData[njd];
	for(int i=0;i<njd;i++)
	{
		OneRXDJDB[i].JDXZ=R_X;        //?????*****//
		OneRXDJDB[i].ISShow=true;
		OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
		OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
		OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������
		OneRXDJDB[i].x=jdarray[i].N;
		OneRXDJDB[i].y=jdarray[i].E;
		OneRXDJDB[i].Lo1=jdarray[i].l1;
		OneRXDJDB[i].R=jdarray[i].R;
		OneRXDJDB[i].Lo2=jdarray[i].l2;
		OneRXDJDB[i].NYDL =0.0;	
		OneRXDJDB[i].qxjk=0.0;
		OneRXDJDB[i].ISXJJFirst=false;

		_stprintf( OneRXDJDB[i].JDNum , L"RX%s" , jdarray[i].JDNum );
	}

	InsertOneRXD(OneRXDJDB,njd,EGH);

	if(OneRXDJDB)
	{
		delete []OneRXDJDB;
		OneRXDJDB=NULL;
	}
	return true;
}




int GTZX2_JYX_ROAD::GetRXDIndexByRXDXLName(CString RXDXLName)
{
	int SumRXD=0;
	int SDLNo,EDLNo;
	CString rxdname;
	SumRXD = GetRXDNum();
	for (int iRXD=0 ; iRXD<SumRXD ; iRXD++)
	{
		GetiRXDDLinfo(iRXD, SDLNo, EDLNo);
		rxdname = DLB2x[SDLNo].RXDName;
		if (rxdname == RXDXLName)
			return iRXD;
	}
	return -1;
}

//�����жα�Ż�ȡ���ж���·��
CString GTZX2_JYX_ROAD::GetRXDXLNameByRXDIndex(int RXDIndex)
{
	int SumRXD=0;
	int SDLNo,EDLNo;
	CString rxdname;
	SumRXD = GetRXDNum();
	if (SumRXD==0 || RXDIndex+1>SumRXD)
		return "";
	GetiRXDDLinfo(RXDIndex, SDLNo, EDLNo);
	rxdname = DLB2x[SDLNo].RXDName;
	return rxdname;
}



void GTZX2_JYX_ROAD::GetiRXD1xTYLC(int iRXD,double &startLc,double &endLc)
{
	GetDLTYLC();
	//ָ�����жε����ն�����
	int sdl,edl;
	GetiRXDDLinfo(iRXD,sdl,edl);
	//ָ�����жε����յ�ͳһ���(����)
	double starttylc2x,endtylc2x;
	starttylc2x=DLB2x[sdl].TYLC;
	endtylc2x=DLB2x[edl].TYLC;

	xlpoint PZ;
	PZ.lc=starttylc2x;
	xlpoint_pz(array,tarray,&PZ);//�������
	//�����һ����ͶӰ
	startLc=PROJ_ML(m_pm1->array,m_pm1->tarray,PZ.x,PZ.y,PZ.x,PZ.y);

	PZ.lc=endtylc2x;
	xlpoint_pz(array,tarray,&PZ);//�յ�����
	//�յ���һ����ͶӰ
	endLc=PROJ_ML(m_pm1->array,m_pm1->tarray,PZ.x,PZ.y,PZ.x,PZ.y);
}


//�������ж��߰��ױ������
void GTZX2_JYX_ROAD::ModifyAllExNEForHundredMeter( )
{

	//�Էֶ�����ѭ��
	for (int i=0;i<GJDInforArray.GetSize();i++)
	{
		ModifyExNEForHundredMeter( GJDInforArray[i].spt[0], GJDInforArray[i].spt[1] , TRUE ) ;

		ModifyExNEForHundredMeter( GJDInforArray[i].ept[0], GJDInforArray[i].ept[1] , FALSE ) ;
	}	
}

int  GTZX2_JYX_ROAD::GetGTX2XJD(double N, double E ,BOOL IsForWard)
{

	double cml = 0.0 ;
	double tzdcml = 0.0 ;
	double dis = 0.0 ,mindis=999999999999999;
	int curjdnum = 0;

	cml = PROJ_ML(array,tarray,N,E,N,E);

	for(int i=0; i<NJD; i++)
	{
		if(IsForWard)
			tzdcml = QXB[i].KZH;
		else
			tzdcml = QXB[i].KHZ;

		dis = fabs(cml - tzdcml);

		if(dis<mindis)
		{
			mindis = dis ;
			curjdnum = i;
		}

	}

	return curjdnum ;
}

/*
//�������߰��ױ������
void GTZX2::ModifyExNEForHundredMeter( double & N, double & E , BOOL IsForWard )
{
//���������������/�յ�ĳ��� 

double S1 = 0.0 ,S2 = 0.0 ;
//���
S1 = pow( ( JDarray[0].N - N ), 2.0 ) + pow( ( JDarray[0].E - E ), 2.0 ) ;
S1 = sqrt(S1);
S2 = pow( ( JDarray[NJD-1].N - N ), 2.0 ) + pow( ( JDarray[NJD-1].E - E ), 2.0 ) ;
S2 = sqrt(S2);
if(S1<1.0 || S2< 1.0)return ;

//������㽻���±�
int CurJD = GetGTX2XJD( N, E ,IsForWard);

ACHAR GH[20];
double kml ;

double cml = 0.0 ;
if( IsForWard ) //��ǰ ȡZH
{
if(CurJD>NJD)return ;
//�������
cml = QXB[CurJD].KZH ;

//�ֳ����
//��������̼����ֳ����


GetGTX2XclcFromcml( cml ,kml , GH ); //��ͨ���2��
kml = int( kml/100.0 + 0.001 ) * 100.0 ;


}
else //��� ȡHZ
{
//�������
cml = QXB[CurJD].KHZ ;

//�ֳ����
//��������̼����ֳ����


GetGTX2XclcFromcml( cml,kml,GH ); //��ͨ���2��

kml = ( int( kml/100.0 + 0.001 ) +1 ) * 100.0 ;

}

ACHAR LCACHAR[80];
_tcscpy(LCACHAR,LCchr(GH,kml,3,1));

CString XLCtring;
XLCtring = LCACHAR ;
XLCtring.Trim();

//���ֳ���̼�������
double cml2x = TYLC2x(XLCtring);

xlpoint PZ;
PZ.lc = cml2x ;

xlpoint_pz(array,tarray,&PZ);

N = PZ.x ;
E = PZ.y ;

//	ads_printf(L"kml=%lf,XLCtring==%s,N=%lf,E=%lf,cml2x=%lf\n",kml,XLCtring,N,E,cml2x);

}
*/

//�������߰��ױ������
void GTZX2_JYX_ROAD::ModifyExNEForHundredMeter( double & N, double & E , BOOL IsForWard )
{
	double dist=1e6;
	dist = sqrt( (JD2[NJD-1].x-N) * (JD2[NJD-1].x-N) +  (JD2[NJD-1].y-E) * (JD2[NJD-1].y-E) );
	if (dist<0.001)
		return;
	dist = sqrt( (JD2[0].x-N) * (JD2[0].x-N) +  (JD2[0].y-E) * (JD2[0].y-E) );
	if (dist<0.001)
		return;
	double cml = 0.0,dml,newdml ;
	//1.���N.E�����Ӧ���ֳ����dml
	ACHAR lcstr[80],gh[8];
	_tcscpy(lcstr , GetGTX2Xclc(N,E));
	split_ckml(lcstr,&dml,gh);

	//2.����dml�����µ��ֳ����newdml
	if( IsForWard ) //��ǰ ȡZH
		newdml = (int)((dml+0.001)/100.0)*100.0;
	else
		newdml = ((int)((dml+0.001)/100.0)+1)*100.0;


	//3.����newdml�����µ�N,E
	_tcscpy( lcstr , LCchr(gh,newdml,3,1) );
	CString lc2xCString ;
	lc2xCString = lcstr ;
	cml = TYLC2x(lc2xCString);
	//��������
	xlpoint PZ ;
	PZ.lc = cml ;
	xlpoint_pz( array,tarray,&PZ );

	N = PZ.x ;
	E = PZ.y ;

}





void  GTZX2_JYX_ROAD::GetGTX2XclcFromcml(double cml,double &dml,ACHAR gh[8])
{
	xlpoint PZ;
	CString CalKml;
	PZ.lc = cml ;
	//����Σ�����
	xlpoint_pz(array,tarray,&PZ);

	//�����ֳ����
	CalKml = GetGTX2Xclc(PZ.x,PZ.y) ;

	ACHAR ckml[80];
	_tcscpy(ckml,CalKml);
	split_ckml(ckml,&dml,gh);

}


//������������
void GTZX2_JYX_ROAD::CalNEForKml2x()
{
	assertWriteEnabled();
	//�õ���ͨ���2�ߵ��ֳ���̣�����NE����

	CString kml ;
	for(int i=0;i<GJDInforArray.GetSize();i++)
	{
		//���
		kml = GetGTX2Xclc(GJDInforArray[i].spt[0] ,GJDInforArray[i].spt[1] );
		_stprintf( GJDInforArray[i].GJDBLC , kml );

		//�յ�
		kml = GetGTX2Xclc(GJDInforArray[i].ept[0] ,GJDInforArray[i].ept[1] );
		_stprintf( GJDInforArray[i].GJDELC , kml );
	}
}

//����̼�������
void GTZX2_JYX_ROAD::CalKmlForNE2x()
{
	assertWriteEnabled();

	xlpoint PZ;
	CString XCLC2x ;
	for(int i=0;i<GJDInforArray.GetSize();i++)
	{
		//���
		XCLC2x = GJDInforArray[i].GJDBLC ;
		PZ.lc = TYLC2x(XCLC2x);
		xlpoint_pz(array,tarray,&PZ);

		GJDInforArray[i].spt[0] = PZ.x ;
		GJDInforArray[i].spt[1] = PZ.y ;


		//�յ�
		XCLC2x = GJDInforArray[i].GJDELC ;
		PZ.lc = TYLC2x(XCLC2x);
		xlpoint_pz(array,tarray,&PZ);

		GJDInforArray[i].ept[0] = PZ.x ;
		GJDInforArray[i].ept[1] = PZ.y ;
	}

}

//��־iRxd��־���
Adesk::Boolean  GTZX2_JYX_ROAD::BZLC2(AcGiWorldDraw* pWd , GTZX_JYX_ROAD * pSubGtzx ,int zybz ,int type )  
	//  
	//    zybz=1,-1  �����ұߣ����
{//��־���
	zybz = -1;
	AcGePoint3d Pt;

	AcGePoint3d  pte,ptb;
	int k;
	double B;
	xlpoint  PZtmp; 
	ACHAR str[80];
	double  WB;
	//��־���
	WB=1.5*texth;
	//1  ~ -1  ������

	for (int i=1;i<pSubGtzx->BZLCarray.GetSize()-1;i++)
	{

		PZtmp.lc=pSubGtzx->BZLCarray[i].xyTocml;  // ͳһ���	
		k=pSubGtzx->xlpoint_pz(pSubGtzx->array,pSubGtzx->tarray,&PZtmp);   

		B=PZtmp.a;
		//�����
		Pt[Y]=PZtmp.x; 
		Pt[X]=PZtmp.y;
		Pt[Z]=0.0;

		//�����
		ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
		ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);
		double  b=B+pi*0.5;
		b=pi*0.5-b; if (zybz==-1) b=b+pi;
		if (b<0.0) b=2*pi+b;		 
		if (b>2*pi) b=b-2*pi;		         
		if(type<=1)  b=b-pi*0.5; 
		_tcscpy(str,pSubGtzx->BZLCarray[i].kml);
		_tcscat(str,L" ��־���");
		AcGePoint3d BZPt;
		BZPt.y=ptb[Y]-(0.25*texth)*cos(B);
		BZPt.x=ptb[X]-(0.25*texth)*sin(B);
		G_maketext(pWd,BZPt,str,b,1.5*texth,QXYSColor,1);
		//���յ�
		pte[X]=PZtmp.y+(_tcslen(str)*1.2*texth)*sin(B+pi*0.5*zybz);
		pte[Y]=PZtmp.x+(_tcslen(str)*1.2*texth)*cos(B+pi*0.5*zybz);
		G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);

	}

	return TRUE;
}


//  �����
Adesk::Boolean GTZX2_JYX_ROAD::BZRxdZD(AcGiWorldDraw* pWd,double dlc,int zybz,int type , GTZX_JYX_ROAD * pSubGtzx )  //  ��ע���
	// bz_zybz==0   ������ƫ���ע   bz_zybz=1,-1  �����ұߣ����
{    

	//  bz_zybz=1  �����ұ�
	//	zybz = -1;  // ���
	AcGePoint3d Pt;
	AcGePoint3d  pte,ptb;
	int k;
	double B;
	xlpoint  PZtmp; 
	ACHAR str[80],str2[80];
	double  WB;
	//��־���
	WB=1.5*texth;

	double cml=0.0,lc=0.0;
	ACHAR kml[80];
	double xlc;
	ACHAR GH[8];

	for(int i=0;i<pSubGtzx->NJD;i++)
	{
		if(pSubGtzx->QXB[i].LorR> 0)
		{
			zybz =1;
		}
		else
		{
			zybz =-1;
		}

		if(i>0 && i< pSubGtzx->NJD-1 )
		{

			for(int j=0;j<4;j++)
			{

				if(j==0)
					cml=pSubGtzx->QXB[i].KZH;
				if(j==1)
					cml=pSubGtzx->QXB[i].KHY;
				if(j==2)
					cml=pSubGtzx->QXB[i].KYH;
				if(j==3)
					cml=pSubGtzx->QXB[i].KHZ;

				if(_tcscmp(xlname,L"��ͨ��Ƣ���")!=0)
				{

					double tmpLC = pSubGtzx->CalnewcmlTocml( cml );

					xlc = pSubGtzx->XLC(tmpLC,GH,pSubGtzx->NDL);
					_tcscpy(kml,L"\0");   
					_tcscpy(kml,LCchr(GH,xlc,NLC,1));


					//xlc = XLC(cml,GH,pSubGtzx->NDL);
					//_tcscpy(kml,L"\0");   
					//_tcscpy(kml,LCchr(GH,xlc,NLC,1));
				}
				else
				{

					PZtmp.lc = cml ;
					k=pSubGtzx->xlpoint_pz(pSubGtzx->array,pSubGtzx->tarray,&PZtmp); 
					CString ckml;
					ckml = pSubGtzx->GetGTXXclc(PZtmp.x,PZtmp.y);
					_tcscpy(kml,ckml);

					////
					/*	lc = pSubGtzx->CalnewcmlTocml(cml);
					PZtmp.lc = lc ;
					k= pSubGtzx->xlpoint_pz(pSubGtzx->array,pSubGtzx->tarray,&PZtmp); 
					CString ckml;
					ckml = pSubGtzx->GetGTXXclc(PZtmp.x,PZtmp.y);
					_tcscpy(kml,ckml);*/
				}

				// lc= CalcmlTonewcml(cml);
				// lc= CalnewcmlTocml(cml);

				//	lc = pSubGtzx->CalcmlTonewcml(cml);
				PZtmp.lc = cml ;

				k=pSubGtzx->xlpoint_pz(pSubGtzx->array,pSubGtzx->tarray,&PZtmp); 


				B=PZtmp.a;
				//�����	
				Pt[Y]=PZtmp.x; 
				Pt[X]=PZtmp.y;
				Pt[Z]=0.0;

				//�����
				ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
				ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);

				ptb[Y] = ptb[Y] - 0.25*texth*cos(B);
				ptb[X] = ptb[X] - 0.25*texth*sin(B);

				if( JDarray[i].R > 1.0e-5 )
				{
					if( JDarray[i].l1 > 1.0e-5 )
					{
						if(j==0) _stprintf(str2,L"ZH %s",kml);
						if(j==1) _stprintf(str2,L"HY %s",kml);
					}
					else
					{
						if(j==0) continue ;
						if(j==1) _stprintf(str2,L"ZY %s",kml);
					}

					if( JDarray[i].l2 > 1.0e-5 )
					{
						if(j==2) _stprintf(str2,L"YH %s",kml);
						if(j==3) _stprintf(str2,L"HZ %s",kml);
					}
					else
					{
						if(j==2) _stprintf(str2,L"YZ %s",kml);
						if(j==3) continue ;
					}

				}
				else continue ;


				double b=0.0;
				if(zybz>0)
				{
					b =pi/2.0-( B +pi*0.5*zybz) + pi ;
					if (b<0.0) b=2*pi+b;		 
					if (b>2*pi) b=b-2*pi;
				}
				else 
				{
					b =pi/2.0-( B +pi*0.5*zybz) ;
					if (b<0.0) b=2*pi+b;		 
					if (b>2*pi) b=b-2*pi;
				}
				AcGePoint3d BZPt;
				BZPt.y=ptb[Y]-(0.25*texth)*cos(B);
				BZPt.x=ptb[X]-(0.25*texth)*sin(B);

				//���յ�
				pte[X]=PZtmp.y+(_tcslen(str2)*1.2*texth)*sin(B+pi*0.5*zybz);
				pte[Y]=PZtmp.x+(_tcslen(str2)*1.2*texth)*cos(B+pi*0.5*zybz);

				G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
				//if(zybz>0)
				//	G_maketext(pWd,pte,str2,b,1.5*texth,QXYSColor,1);
				//else 	G_maketext(pWd,ptb,str2,b,1.5*texth,QXYSColor,1);

			}

		}

	}


	return TRUE;
}



//  ɾ��һ����־���
void GTZX2_JYX_ROAD::DeleteBZLCToGTZX2(ads_point PT)
{   
	int DelBZLCNum = 0;

	double MinL = 99999999999;
	double deltaL = 0.0 ;

	for(int i=0; i< BZLCarray.GetSize() ; i++)
	{
		deltaL = sqrt( pow( (PT[Y] - BZLCarray[i].X ) ,2.0) + pow( (PT[X] - BZLCarray[i].Y ) ,2.0)) ;
		if(MinL <  deltaL)
			MinL =  MinL;
		else
		{
			MinL = deltaL; 
			DelBZLCNum = i;
		}
	}

	//ɾ��ָ������,������� BZLCarray ����
	if(DelBZLCNum>0)
		BZLCarray.RemoveAt(DelBZLCNum,1);

}

////////////////////////////////////

void GTZX2_JYX_ROAD::GetNEToGtzx2xkml( CString kml2x , double & NToOut , double & EToOut )
{
	ACHAR ckml[30],gh[8];
	double xlcTmp = 0.0 ;
	_tcscpy(ckml,kml2x);
	split_ckml(ckml,&xlcTmp,gh);

	double cmlTmp = m_pm1->TYLC(xlcTmp);

	cmlTmp = CalcmlTonewcml(cmlTmp);

	xlpoint PZ ;
	PZ.lc = cmlTmp ;
	m_pm1->xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);

	double cml2x = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);

	PZ.lc = cml2x ;
	xlpoint_pz(array,tarray,&PZ);

	NToOut = PZ.x ;

	EToOut = PZ.y ;
}

//bool GTZX2::AddRXD(GTZX *newGtzx,double N=0,double E=0)
bool GTZX2_JYX_ROAD::AddRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray , ACHAR EGH[], bool IsRX,bool ISHBQX)
	//һ��GTZXʵ����Ϊ���ж�����ߺϲ�����ʱ��β����ϴ���
{
	assertWriteEnabled();
	DoubleLineData *OneRXDJDB;
	int njd = jdarray.GetSize()-1 ;
	int RXDSum = GetRXDNum();
	///����һ�����жν����
	OneRXDJDB=new DoubleLineData[njd+1];

	int i=0;
	for(i=0;i<njd;i++)
	{
		if (IsRX)
		{
			if (ISHBQX)
				OneRXDJDB[i].JDXZ=R_X_OneR;
			else
				OneRXDJDB[i].JDXZ=R_X;
		}
		else
			OneRXDJDB[i].JDXZ=B_XNOTP_X;
		OneRXDJDB[i].ISShow=true;
		OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
		OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
		OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������
		OneRXDJDB[i].x=jdarray[i].N;
		OneRXDJDB[i].y=jdarray[i].E;
		OneRXDJDB[i].Lo1=jdarray[i].l1;
		OneRXDJDB[i].R=jdarray[i].R;
		OneRXDJDB[i].Lo2=jdarray[i].l2;
		OneRXDJDB[i].NYDL =0.0;	
		OneRXDJDB[i].qxjk=0.0;
		OneRXDJDB[i].ISXJJFirst=false;

		//_stprintf( OneRXDJDB[i].JDNum , L"RX%dJD%s" , RXDSum+1,jdarray[i].JDNum );

		_stprintf( OneRXDJDB[i].JDNum , L"%s-%d" ,jdarray[i].JDNum,i);//�Ӿ� 09.2.24
	}

	//i=njd-1;
	//OneRXDJDB[i].ISShow=true;
	//if (IsRX)
	//	OneRXDJDB[i].JDXZ=R_X;  //?????*****//
	//else
	//	OneRXDJDB[i].JDXZ=B_XNOTP_X;  //?????*****//
	//OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
	//OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
	//OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������
	//OneRXDJDB[i].x=jdarray[i].N;
	//OneRXDJDB[i].y=jdarray[i].E;
	//OneRXDJDB[i].Lo1=jdarray[i].l1;
	//OneRXDJDB[i].R=jdarray[i].R;
	//OneRXDJDB[i].Lo2=jdarray[i].l2;
	//OneRXDJDB[i].NYDL =0.0;	
	//OneRXDJDB[i].qxjk=0.0;
	//OneRXDJDB[i].ISXJJFirst=false;

	//_stprintf( OneRXDJDB[i].JDNum , L"R%dXJD%s" , RXDSum+1,jdarray[i].JDNum );

	i=njd;

	OneRXDJDB[i].ISShow=true;
	if (IsRX)
	{
		if (ISHBQX)
			OneRXDJDB[i].JDXZ=R_X_OneR;
		else
			OneRXDJDB[i].JDXZ=R_X;
	}
	else
		OneRXDJDB[i].JDXZ=B_XNOTP_X;
	OneRXDJDB[i].jdno=i;//�ȸ�һ����ʼֵ
	OneRXDJDB[i].jdno1=0;//�ȸ�һ����ʼֵ//����InsertOneRXD()�ٷ�����ֵ
	OneRXDJDB[i].xjj=0;//�ȸ�һ����ʼֵ,��������������ж���˵������


	OneRXDJDB[i].x=jdarray[i].N;
	OneRXDJDB[i].y=jdarray[i].E;

	OneRXDJDB[i].Lo1=0.0;
	OneRXDJDB[i].R=0.0;
	OneRXDJDB[i].Lo2=0.0;
	OneRXDJDB[i].NYDL =0.0;	
	OneRXDJDB[i].qxjk=0.0;
	OneRXDJDB[i].ISXJJFirst=false;

	//_stprintf( OneRXDJDB[i].JDNum , L"RX%dJD%s" , RXDSum+1,jdarray[i].JDNum );
	_stprintf( OneRXDJDB[i].JDNum , L"%s-%d" ,jdarray[i].JDNum,i);//�Ӿ� 09.2.24

	InsertOneRXD(OneRXDJDB,njd+1,EGH);

	if(OneRXDJDB)
	{
		delete []OneRXDJDB;
		OneRXDJDB=NULL;
	}

	return TRUE ;

}

//////////////////////////////////////////////////////////////////////////
//����Spt��Ept��Χ�ڵ����ö���Ϣ����LYDInfArray
//////////////////////////////////////////////////////////////////////////
void GTZX2_JYX_ROAD::CreateLYDArray(double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray)
{
	double SmlGJ,EmlGJ,SmlJY,EmlJY;
	int JYJDStart,JYJDEnd,GJJDStart,GJJDEnd;
	double Tmpcml;
	bool IsQJZXCD,IsHJZXCD;//ǰ���ֱ���Ƿ��ص�
	int iQGJJD,iQJYJD;//ǰ��ֱ���ص��ļ��к͸Ľ������±�
	int iHGJJD,iHJYJD;//���ֱ���ص��ļ��к͸Ľ������±�
	double TmpXJJ,TmpProjml;
	double TmpPt[2];
	GJDInf OneGjdInfo;

	SmlGJ = PROJ_ML(array,tarray,Spt[0],Spt[1],Spt[0],Spt[1]);
	EmlGJ = PROJ_ML(array,tarray,Ept[0],Ept[1],Ept[0],Ept[1]);

	if (EmlGJ-SmlGJ<0.001)
		return ;

	if (m_pm2==NULL)
	{
		CreateLYDArray_OnJYJX(m_pm1,Spt,Ept,LYDInfArray);
	}
	else
	{
		CreateLYDArray_OnJYEX(m_pm2,Spt,Ept,LYDInfArray);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
//���ɹ�ͨ��ƶ�������л������ö�
//////////////////////////////////////////////////////////////////////////
void GTZX2_JYX_ROAD::CreateLYDArray_OnJYJX(GTZX_JYX_ROAD* pGTZXJY,double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray)
{
	double SmlGJ,EmlGJ,SmlJY,EmlJY;
	int JYJDStart,JYJDEnd,GJJDStart,GJJDEnd;
	double Tmpcml;
	bool IsQJZXCD,IsHJZXCD;//ǰ���ֱ���Ƿ��ص�
	bool IsSameQXYS;//����Ҫ���Ƿ���ͬ
	int iQGJJD,iQJYJD;//ǰ��ֱ���ص��ļ��к͸Ľ������±�
	int iHGJJD,iHJYJD;//���ֱ���ص��ļ��к͸Ľ������±�
	double TmpXJJ,TmpProjml;
	double TmpPt[2];
	GJDInf OneGjdInfo;
	double LYDSml,LYDEml;
	double Dist = 1e9;
	double GJXLSpt[2],GJXLEpt[2];
	xlpoint PZ;

	IsQJZXCD = IsHJZXCD = IsSameQXYS = false;

	SmlGJ = PROJ_ML(array,tarray,Spt[0],Spt[1],Spt[0],Spt[1]);
	EmlGJ = PROJ_ML(array,tarray,Ept[0],Ept[1],Ept[0],Ept[1]);
	//GJXLSpt[0] = JDarray[0].N;
	//GJXLSpt[1] = JDarray[0].E;
	//GJXLEpt[0] = JDarray[NJD-1].N;
	//GJXLEpt[1] = JDarray[NJD-1].E;
	GJXLSpt[0] = Spt[0];
	GJXLSpt[1] = Spt[1];
	GJXLEpt[0] = Ept[0];
	GJXLEpt[1] = Ept[1];

	if (SmlGJ<QXB[0].KZH-0.01 || SmlGJ<-0.001)
	{
		SmlGJ = QXB[0].KZH;
		Spt[0] = JDarray[0].N;
		Spt[1] = JDarray[1].E;
	}

	if (EmlGJ>QXB[NJD-1].KHZ+0.01 || EmlGJ<-0.001)
	{
		EmlGJ = QXB[NJD-1].KHZ;
		Ept[0] = JDarray[NJD-1].N;
		Ept[1] = JDarray[NJD-1].E;
	}

	if (EmlGJ-SmlGJ<0.001)
		return ;

	GJJDStart = DistToNearJD(SmlGJ,false);
	GJJDEnd = DistToNearJD(EmlGJ,true);


	SmlJY = pGTZXJY->PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,Spt[0],Spt[1],Spt[0],Spt[1]);
	EmlJY = pGTZXJY->PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,Ept[0],Ept[1],Ept[0],Ept[1]);
	JYJDStart = pGTZXJY->DistToNearJD(SmlJY,false);
	JYJDEnd = pGTZXJY->DistToNearJD(EmlJY,true);

	if (JYJDStart>JYJDEnd)//�������иĽ��ι���һ��������ֱ�߱�
	{
		CalXjj(0.5*(SmlJY+EmlJY),array,tarray,pGTZXJY->array,pGTZXJY->tarray,TmpXJJ,TmpProjml,TmpPt);

		if (fabs(TmpXJJ)<0.1)//���ö�
		{
			OneGjdInfo.spt[0] = Spt[0];
			OneGjdInfo.spt[1] = Spt[1];
			OneGjdInfo.ept[0] = Ept[0];
			OneGjdInfo.ept[1] = Ept[1];
			Dist = sqrt((OneGjdInfo.spt[0]-GJXLSpt[0])*(OneGjdInfo.spt[0]-GJXLSpt[0]) + (OneGjdInfo.spt[1]-GJXLSpt[1])*(OneGjdInfo.spt[1]-GJXLSpt[1]));
			if (Dist>0.1)
			{
				LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.spt[0],OneGjdInfo.spt[1]);
				PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
				xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
				LYDSml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
				PZ.lc = LYDSml;
				xlpoint_pz(array,tarray,&PZ);
				OneGjdInfo.spt[0] = PZ.x;
				OneGjdInfo.spt[1] = PZ.y;
			}

			Dist = sqrt((OneGjdInfo.ept[0]-GJXLEpt[0])*(OneGjdInfo.ept[0]-GJXLEpt[0]) + (OneGjdInfo.ept[1]-GJXLEpt[1])*(OneGjdInfo.ept[1]-GJXLEpt[1]));
			if (Dist>0.1)
			{
				LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.ept[0],OneGjdInfo.ept[1],OneGjdInfo.ept[0],OneGjdInfo.ept[1]);
				PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
				xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
				LYDEml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
				PZ.lc = LYDEml;
				xlpoint_pz(array,tarray,&PZ);
				OneGjdInfo.ept[0] = PZ.x;
				OneGjdInfo.ept[1] = PZ.y;
			}

			_tcscpy(OneGjdInfo.BXLName , pGTZXJY->XLShuXing);
			_tcscpy(OneGjdInfo.EXLName , pGTZXJY->XLShuXing);
			_tcscpy(OneGjdInfo.gjdname,L"���ö�");
			OneGjdInfo.GJDType = -1;
			LYDInfArray.Add(OneGjdInfo);
			return ;
		}
	}

	else
	{
		iQGJJD = GJJDStart;
		iHGJJD = GJJDStart;
		for (int iGJJD=GJJDStart;iGJJD<=GJJDEnd;iGJJD++)
		{
			IsQJZXCD = SearchJZXCDJDIndex_OnJYJX(pGTZXJY,iGJJD,-1,JYJDStart,JYJDEnd,iQJYJD);//������iGJJDǰ��ֱ���ص��ļ����߽���
			IsHJZXCD = SearchJZXCDJDIndex_OnJYJX(pGTZXJY,iGJJD,+1,JYJDStart,JYJDEnd,iHJYJD);//������iGJJDǰ��ֱ���ص��ļ����߽���

			if (
				IsQJZXCD && IsHJZXCD && iQJYJD==iHJYJD
				&& fabs(pGTZXJY->JDarray[iQJYJD].R - JDarray[iGJJD].R)<0.001
				&& fabs(pGTZXJY->JDarray[iQJYJD].l1 - JDarray[iGJJD].l1)<0.001
				&& fabs(pGTZXJY->JDarray[iQJYJD].l2 - JDarray[iGJJD].l2)<0.001
				)
			{
				Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
					JZXArray[iGJJD].QJZXStartPt.x,JZXArray[iGJJD].QJZXStartPt.y,
					JZXArray[iGJJD].QJZXStartPt.x,JZXArray[iGJJD].QJZXStartPt.y);
				if (pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.lc > Tmpcml)
				{
					OneGjdInfo.spt[0] = pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.x;
					OneGjdInfo.spt[1] = pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.y;
				}
				else
				{
					OneGjdInfo.spt[0] = JZXArray[iGJJD].QJZXStartPt.x;
					OneGjdInfo.spt[1] = JZXArray[iGJJD].QJZXStartPt.y;
				}

				Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
					JZXArray[iGJJD].HJZXEndPt.x,JZXArray[iGJJD].HJZXEndPt.y,
					JZXArray[iGJJD].HJZXEndPt.x,JZXArray[iGJJD].HJZXEndPt.y);
				if (pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.lc < Tmpcml)
				{
					OneGjdInfo.ept[0] = pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.x;
					OneGjdInfo.ept[1] = pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.y;
				}
				else
				{
					OneGjdInfo.ept[0] = JZXArray[iGJJD].HJZXEndPt.x;
					OneGjdInfo.ept[1] = JZXArray[iGJJD].HJZXEndPt.y;
				}
				Dist = sqrt((OneGjdInfo.spt[0]-GJXLSpt[0])*(OneGjdInfo.spt[0]-GJXLSpt[0]) + (OneGjdInfo.spt[1]-GJXLSpt[1])*(OneGjdInfo.spt[1]-GJXLSpt[1]));
				if (Dist>0.1)
				{
					LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.spt[0],OneGjdInfo.spt[1]);
					PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
					xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
					LYDSml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					PZ.lc = LYDSml;
					xlpoint_pz(array,tarray,&PZ);
					OneGjdInfo.spt[0] = PZ.x;
					OneGjdInfo.spt[1] = PZ.y;
				}

				Dist = sqrt((OneGjdInfo.ept[0]-GJXLEpt[0])*(OneGjdInfo.ept[0]-GJXLEpt[0]) + (OneGjdInfo.ept[1]-GJXLEpt[1])*(OneGjdInfo.ept[1]-GJXLEpt[1]));
				if (Dist>0.1)
				{
					LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.ept[0],OneGjdInfo.ept[1],OneGjdInfo.ept[0],OneGjdInfo.ept[1]);
					PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
					xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
					LYDEml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					PZ.lc = LYDEml;
					xlpoint_pz(array,tarray,&PZ);
					OneGjdInfo.ept[0] = PZ.x;
					OneGjdInfo.ept[1] = PZ.y;
				}
				_tcscpy(OneGjdInfo.BXLName , pGTZXJY->XLShuXing);
				_tcscpy(OneGjdInfo.EXLName , pGTZXJY->XLShuXing);
				_tcscpy(OneGjdInfo.gjdname,L"���ö�");
				OneGjdInfo.GJDType = -1;
				LYDInfArray.Add(OneGjdInfo);

			}
			else
			{
				if (IsQJZXCD)//ǰ��ֱ���������ò���
				{

					Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
						JZXArray[iGJJD].QJZXStartPt.x,JZXArray[iGJJD].QJZXStartPt.y,
						JZXArray[iGJJD].QJZXStartPt.x,JZXArray[iGJJD].QJZXStartPt.y);
					if (pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.lc > Tmpcml)
					{
						OneGjdInfo.spt[0] = pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.x;
						OneGjdInfo.spt[1] = pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.y;
					}
					else
					{
						OneGjdInfo.spt[0] = JZXArray[iGJJD].QJZXStartPt.x;
						OneGjdInfo.spt[1] = JZXArray[iGJJD].QJZXStartPt.y;
					}

					Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
						JZXArray[iGJJD].QJZXEndPt.x,JZXArray[iGJJD].QJZXEndPt.y,
						JZXArray[iGJJD].QJZXEndPt.x,JZXArray[iGJJD].QJZXEndPt.y);	
					if (pGTZXJY->JZXArray[iQJYJD].QJZXEndPt.lc < Tmpcml)
					{
						OneGjdInfo.ept[0] = pGTZXJY->JZXArray[iQJYJD].QJZXEndPt.x;
						OneGjdInfo.ept[1] = pGTZXJY->JZXArray[iQJYJD].QJZXEndPt.y;
					}
					else
					{
						OneGjdInfo.ept[0] = JZXArray[iGJJD].QJZXEndPt.x;
						OneGjdInfo.ept[1] = JZXArray[iGJJD].QJZXEndPt.y;
					}

					Dist = sqrt((OneGjdInfo.spt[0]-GJXLSpt[0])*(OneGjdInfo.spt[0]-GJXLSpt[0]) + (OneGjdInfo.spt[1]-GJXLSpt[1])*(OneGjdInfo.spt[1]-GJXLSpt[1]));
					if (Dist>0.1)
					{
						LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.spt[0],OneGjdInfo.spt[1]);
						PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
						xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
						LYDSml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
						PZ.lc = LYDSml;
						xlpoint_pz(array,tarray,&PZ);
						OneGjdInfo.spt[0] = PZ.x;
						OneGjdInfo.spt[1] = PZ.y;
					}
					Dist = sqrt((OneGjdInfo.ept[0]-GJXLEpt[0])*(OneGjdInfo.ept[0]-GJXLEpt[0]) + (OneGjdInfo.ept[1]-GJXLEpt[1])*(OneGjdInfo.ept[1]-GJXLEpt[1]));
					if (Dist>0.1)
					{
						LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.ept[0],OneGjdInfo.ept[1],OneGjdInfo.ept[0],OneGjdInfo.ept[1]);
						PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
						xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
						LYDEml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
						PZ.lc = LYDEml;
						xlpoint_pz(array,tarray,&PZ);
						OneGjdInfo.ept[0] = PZ.x;
						OneGjdInfo.ept[1] = PZ.y;
					}
					_tcscpy(OneGjdInfo.BXLName , pGTZXJY->XLShuXing);
					_tcscpy(OneGjdInfo.EXLName , pGTZXJY->XLShuXing);
					OneGjdInfo.GJDType = -1;
					_tcscpy(OneGjdInfo.gjdname,L"���ö�");
					LYDInfArray.Add(OneGjdInfo);
				}
				if (IsHJZXCD)//���ֱ���������ò���
				{
					Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
						JZXArray[iGJJD].HJZXStartPt.x,JZXArray[iGJJD].HJZXStartPt.y,
						JZXArray[iGJJD].HJZXStartPt.x,JZXArray[iGJJD].HJZXStartPt.y);	
					if (pGTZXJY->JZXArray[iHJYJD].HJZXStartPt.lc > Tmpcml)
					{
						OneGjdInfo.spt[0] = pGTZXJY->JZXArray[iHJYJD].HJZXStartPt.x;
						OneGjdInfo.spt[1] = pGTZXJY->JZXArray[iHJYJD].HJZXStartPt.y;
					}
					else
					{
						OneGjdInfo.spt[0] = JZXArray[iGJJD].HJZXStartPt.x;
						OneGjdInfo.spt[1] = JZXArray[iGJJD].HJZXStartPt.y;
					}

					Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
						JZXArray[iGJJD].HJZXEndPt.x,JZXArray[iGJJD].HJZXEndPt.y,
						JZXArray[iGJJD].HJZXEndPt.x,JZXArray[iGJJD].HJZXEndPt.y);	
					if (pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.lc < Tmpcml)
					{
						OneGjdInfo.ept[0] = pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.x;
						OneGjdInfo.ept[1] = pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.y;
					}
					else
					{
						OneGjdInfo.ept[0] = JZXArray[iGJJD].HJZXEndPt.x;
						OneGjdInfo.ept[1] = JZXArray[iGJJD].HJZXEndPt.y;
					}

					Dist = sqrt((OneGjdInfo.spt[0]-GJXLSpt[0])*(OneGjdInfo.spt[0]-GJXLSpt[0]) + (OneGjdInfo.spt[1]-GJXLSpt[1])*(OneGjdInfo.spt[1]-GJXLSpt[1]));
					if (Dist>0.1)
					{
						LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.spt[0],OneGjdInfo.spt[1]);
						PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
						xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
						LYDSml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
						PZ.lc = LYDSml;
						xlpoint_pz(array,tarray,&PZ);
						OneGjdInfo.spt[0] = PZ.x;
						OneGjdInfo.spt[1] = PZ.y;
					}

					Dist = sqrt((OneGjdInfo.ept[0]-GJXLEpt[0])*(OneGjdInfo.ept[0]-GJXLEpt[0]) + (OneGjdInfo.ept[1]-GJXLEpt[1])*(OneGjdInfo.ept[1]-GJXLEpt[1]));
					if (Dist>0.1)
					{
						LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.ept[0],OneGjdInfo.ept[1],OneGjdInfo.ept[0],OneGjdInfo.ept[1]);
						PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
						xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
						LYDEml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
						PZ.lc = LYDEml;
						xlpoint_pz(array,tarray,&PZ);
						OneGjdInfo.ept[0] = PZ.x;
						OneGjdInfo.ept[1] = PZ.y;
					}
					_tcscpy(OneGjdInfo.BXLName , pGTZXJY->XLShuXing);
					_tcscpy(OneGjdInfo.EXLName , pGTZXJY->XLShuXing);
					OneGjdInfo.GJDType = -1;
					_tcscpy(OneGjdInfo.gjdname,L"���ö�");
					LYDInfArray.Add(OneGjdInfo);
				}
			}

		}
	}

	int LYDSum = LYDInfArray.GetSize();
	if (LYDSum>0)
	{
		Tmpcml = PROJ_ML(array,tarray,LYDInfArray[0].spt[0],LYDInfArray[0].spt[1],LYDInfArray[0].spt[0],LYDInfArray[0].spt[1]);
		if (Tmpcml<SmlGJ)
		{
			LYDInfArray[0].spt[0] = Spt[0];
			LYDInfArray[0].spt[1] = Spt[1];
		}

		Tmpcml = PROJ_ML(array,tarray,LYDInfArray[LYDSum-1].ept[0],LYDInfArray[LYDSum-1].ept[1],LYDInfArray[LYDSum-1].ept[0],LYDInfArray[LYDSum-1].ept[1]);
		if (Tmpcml>EmlGJ)
		{
			LYDInfArray[LYDSum-1].ept[0] = Ept[0];
			LYDInfArray[LYDSum-1].ept[1] = Ept[1];
		}
	}

	MergeGTZX2GJDArray(LYDInfArray);
	return;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
void GTZX2_JYX_ROAD::CreateLYDArray_OnJYEX(GTZX2_JYX_ROAD* pGTZXJY,double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray)
{
	double SmlGJ,EmlGJ,SmlJY,EmlJY;
	int JYJDStart,JYJDEnd,GJJDStart,GJJDEnd;
	double Tmpcml;
	bool IsQJZXCD,IsHJZXCD;//ǰ���ֱ���Ƿ��ص�
	int iQGJJD,iQJYJD;//ǰ��ֱ���ص��ļ��к͸Ľ������±�
	int iHGJJD,iHJYJD;//���ֱ���ص��ļ��к͸Ľ������±�
	double TmpXJJ,TmpProjml;
	double TmpPt[2];
	GJDInf OneGjdInfo;
	double LYDSml,LYDEml;
	double Dist = 1e9;
	double GJXLSpt[2],GJXLEpt[2];
	xlpoint PZ;

	SmlGJ = PROJ_ML(array,tarray,Spt[0],Spt[1],Spt[0],Spt[1]);
	EmlGJ = PROJ_ML(array,tarray,Ept[0],Ept[1],Ept[0],Ept[1]);
	//GJXLSpt[0] = JDarray[0].N;
	//GJXLSpt[1] = JDarray[0].E;
	//GJXLEpt[0] = JDarray[NJD-1].N;
	//GJXLEpt[1] = JDarray[NJD-1].E;
	GJXLSpt[0] = Spt[0];
	GJXLSpt[1] = Spt[1];
	GJXLEpt[0] = Ept[0];
	GJXLEpt[1] = Ept[1];
	if (EmlGJ-SmlGJ<0.1)
		return ;

	GJJDStart = DistToNearJD(SmlGJ,false);
	GJJDEnd = DistToNearJD(EmlGJ,true);


	SmlJY = pGTZXJY->PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,Spt[0],Spt[1],Spt[0],Spt[1]);
	EmlJY = pGTZXJY->PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,Ept[0],Ept[1],Ept[0],Ept[1]);
	JYJDStart = pGTZXJY->DistToNearJD(SmlJY,false);
	JYJDEnd = pGTZXJY->DistToNearJD(EmlJY,true);

	if (JYJDStart>JYJDEnd)//�������иĽ��ι���һ��������ֱ�߱�
	{
		CalXjj(0.5*(SmlJY+EmlJY),array,tarray,pGTZXJY->array,pGTZXJY->tarray,TmpXJJ,TmpProjml,TmpPt);

		if (fabs(TmpXJJ)<0.1)//���ö�
		{
			OneGjdInfo.spt[0] = Spt[0];
			OneGjdInfo.spt[1] = Spt[1];
			OneGjdInfo.ept[0] = Ept[0];
			OneGjdInfo.ept[1] = Ept[1];
			Dist = sqrt((OneGjdInfo.spt[0]-GJXLSpt[0])*(OneGjdInfo.spt[0]-GJXLSpt[0]) + (OneGjdInfo.spt[1]-GJXLSpt[1])*(OneGjdInfo.spt[1]-GJXLSpt[1]));
			if (Dist>0.1)
			{
				LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.spt[0],OneGjdInfo.spt[1]);
				PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
				xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
				LYDSml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
				PZ.lc = LYDSml;
				xlpoint_pz(array,tarray,&PZ);
				OneGjdInfo.spt[0] = PZ.x;
				OneGjdInfo.spt[1] = PZ.y;
			}

			Dist = sqrt((OneGjdInfo.ept[0]-GJXLEpt[0])*(OneGjdInfo.ept[0]-GJXLEpt[0]) + (OneGjdInfo.ept[1]-GJXLEpt[1])*(OneGjdInfo.ept[1]-GJXLEpt[1]));
			if (Dist>0.1)
			{
				LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.ept[0],OneGjdInfo.ept[1],OneGjdInfo.ept[0],OneGjdInfo.ept[1]);
				PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
				xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
				LYDEml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
				PZ.lc = LYDEml;
				xlpoint_pz(array,tarray,&PZ);
				OneGjdInfo.ept[0] = PZ.x;
				OneGjdInfo.ept[1] = PZ.y;
			}

			_tcscpy(OneGjdInfo.BXLName , pGTZXJY->XLShuXing);
			_tcscpy(OneGjdInfo.EXLName , pGTZXJY->XLShuXing);
			_tcscpy(OneGjdInfo.gjdname,L"���ö�");
			OneGjdInfo.GJDType = -1;
			LYDInfArray.Add(OneGjdInfo);
			return ;
		}
	}

	else
	{
		iQGJJD = GJJDStart;
		iHGJJD = GJJDStart;
		for (int iGJJD=GJJDStart;iGJJD<=GJJDEnd;iGJJD++)
		{
			IsQJZXCD = SearchJZXCDJDIndex_OnJYJX(pGTZXJY,iGJJD,-1,JYJDStart,JYJDEnd,iQJYJD);//������iGJJDǰ��ֱ���ص��ļ����߽���
			IsHJZXCD = SearchJZXCDJDIndex_OnJYJX(pGTZXJY,iGJJD,+1,JYJDStart,JYJDEnd,iHJYJD);//������iGJJDǰ��ֱ���ص��ļ����߽���
			if (IsQJZXCD && IsHJZXCD && iQJYJD==iHJYJD)//ǰ���ֱ�߾��ص�������Ҫ����ͬ
			{
				if (
					fabs(pGTZXJY->JDarray[iQJYJD].R - JDarray[iGJJD].R)<0.001
					&& fabs(pGTZXJY->JDarray[iQJYJD].l1 - JDarray[iGJJD].l1)<0.001
					&& fabs(pGTZXJY->JDarray[iQJYJD].l2 - JDarray[iGJJD].l2)<0.001
					)
				{
					Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
						JZXArray[iGJJD].QJZXStartPt.x,JZXArray[iGJJD].QJZXStartPt.y,
						JZXArray[iGJJD].QJZXStartPt.x,JZXArray[iGJJD].QJZXStartPt.y);
					if (pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.lc > Tmpcml)
					{
						OneGjdInfo.spt[0] = pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.x;
						OneGjdInfo.spt[1] = pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.y;
					}
					else
					{
						OneGjdInfo.spt[0] = JZXArray[iGJJD].QJZXStartPt.x;
						OneGjdInfo.spt[1] = JZXArray[iGJJD].QJZXStartPt.y;
					}

					Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
						JZXArray[iGJJD].HJZXEndPt.x,JZXArray[iGJJD].HJZXEndPt.y,
						JZXArray[iGJJD].HJZXEndPt.x,JZXArray[iGJJD].HJZXEndPt.y);
					if (pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.lc < Tmpcml)
					{
						OneGjdInfo.ept[0] = pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.x;
						OneGjdInfo.ept[1] = pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.y;
					}
					else
					{
						OneGjdInfo.ept[0] = JZXArray[iGJJD].HJZXEndPt.x;
						OneGjdInfo.ept[1] = JZXArray[iGJJD].HJZXEndPt.y;
					}
					Dist = sqrt((OneGjdInfo.spt[0]-GJXLSpt[0])*(OneGjdInfo.spt[0]-GJXLSpt[0]) + (OneGjdInfo.spt[1]-GJXLSpt[1])*(OneGjdInfo.spt[1]-GJXLSpt[1]));
					if (Dist>0.1)
					{
						LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.spt[0],OneGjdInfo.spt[1]);
						PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
						xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
						LYDSml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
						PZ.lc = LYDSml;
						xlpoint_pz(array,tarray,&PZ);
						OneGjdInfo.spt[0] = PZ.x;
						OneGjdInfo.spt[1] = PZ.y;
					}

					Dist = sqrt((OneGjdInfo.ept[0]-GJXLEpt[0])*(OneGjdInfo.ept[0]-GJXLEpt[0]) + (OneGjdInfo.ept[1]-GJXLEpt[1])*(OneGjdInfo.ept[1]-GJXLEpt[1]));
					if (Dist>0.1)
					{
						LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.ept[0],OneGjdInfo.ept[1],OneGjdInfo.ept[0],OneGjdInfo.ept[1]);
						PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
						xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
						LYDEml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
						PZ.lc = LYDEml;
						xlpoint_pz(array,tarray,&PZ);
						OneGjdInfo.ept[0] = PZ.x;
						OneGjdInfo.ept[1] = PZ.y;
					}
					_tcscpy(OneGjdInfo.BXLName , pGTZXJY->XLShuXing);
					_tcscpy(OneGjdInfo.EXLName , pGTZXJY->XLShuXing);
					_tcscpy(OneGjdInfo.gjdname,L"���ö�");
					OneGjdInfo.GJDType = -1;
					LYDInfArray.Add(OneGjdInfo);
				}

			}
			if (IsQJZXCD && !IsHJZXCD)//ǰ��ֱ���������ò���
			{

				Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
					JZXArray[iGJJD].QJZXStartPt.x,JZXArray[iGJJD].QJZXStartPt.y,
					JZXArray[iGJJD].QJZXStartPt.x,JZXArray[iGJJD].QJZXStartPt.y);
				if (pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.lc > Tmpcml)
				{
					OneGjdInfo.spt[0] = pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.x;
					OneGjdInfo.spt[1] = pGTZXJY->JZXArray[iQJYJD].QJZXStartPt.y;
				}
				else
				{
					OneGjdInfo.spt[0] = JZXArray[iGJJD].QJZXStartPt.x;
					OneGjdInfo.spt[1] = JZXArray[iGJJD].QJZXStartPt.y;
				}

				Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
					JZXArray[iGJJD].QJZXEndPt.x,JZXArray[iGJJD].QJZXEndPt.y,
					JZXArray[iGJJD].QJZXEndPt.x,JZXArray[iGJJD].QJZXEndPt.y);	
				if (pGTZXJY->JZXArray[iQJYJD].QJZXEndPt.lc < Tmpcml)
				{
					OneGjdInfo.ept[0] = pGTZXJY->JZXArray[iQJYJD].QJZXEndPt.x;
					OneGjdInfo.ept[1] = pGTZXJY->JZXArray[iQJYJD].QJZXEndPt.y;
				}
				else
				{
					OneGjdInfo.ept[0] = JZXArray[iGJJD].QJZXEndPt.x;
					OneGjdInfo.ept[1] = JZXArray[iGJJD].QJZXEndPt.y;
				}

				Dist = sqrt((OneGjdInfo.spt[0]-GJXLSpt[0])*(OneGjdInfo.spt[0]-GJXLSpt[0]) + (OneGjdInfo.spt[1]-GJXLSpt[1])*(OneGjdInfo.spt[1]-GJXLSpt[1]));
				if (Dist>0.1)
				{
					LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.spt[0],OneGjdInfo.spt[1]);
					PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
					xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
					LYDSml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					PZ.lc = LYDSml;
					xlpoint_pz(array,tarray,&PZ);
					OneGjdInfo.spt[0] = PZ.x;
					OneGjdInfo.spt[1] = PZ.y;
				}
				Dist = sqrt((OneGjdInfo.ept[0]-GJXLEpt[0])*(OneGjdInfo.ept[0]-GJXLEpt[0]) + (OneGjdInfo.ept[1]-GJXLEpt[1])*(OneGjdInfo.ept[1]-GJXLEpt[1]));
				if (Dist>0.1)
				{
					LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.ept[0],OneGjdInfo.ept[1],OneGjdInfo.ept[0],OneGjdInfo.ept[1]);
					PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
					xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
					LYDEml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					PZ.lc = LYDEml;
					xlpoint_pz(array,tarray,&PZ);
					OneGjdInfo.ept[0] = PZ.x;
					OneGjdInfo.ept[1] = PZ.y;
				}
				_tcscpy(OneGjdInfo.BXLName , pGTZXJY->XLShuXing);
				_tcscpy(OneGjdInfo.EXLName , pGTZXJY->XLShuXing);
				OneGjdInfo.GJDType = -1;
				_tcscpy(OneGjdInfo.gjdname,L"���ö�");
				LYDInfArray.Add(OneGjdInfo);
			}
			if (!IsQJZXCD && IsHJZXCD)//���ֱ���������ò���
			{
				Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
					JZXArray[iGJJD].HJZXStartPt.x,JZXArray[iGJJD].HJZXStartPt.y,
					JZXArray[iGJJD].HJZXStartPt.x,JZXArray[iGJJD].HJZXStartPt.y);	
				if (pGTZXJY->JZXArray[iHJYJD].HJZXStartPt.lc > Tmpcml)
				{
					OneGjdInfo.spt[0] = pGTZXJY->JZXArray[iHJYJD].HJZXStartPt.x;
					OneGjdInfo.spt[1] = pGTZXJY->JZXArray[iHJYJD].HJZXStartPt.y;
				}
				else
				{
					OneGjdInfo.spt[0] = JZXArray[iGJJD].HJZXStartPt.x;
					OneGjdInfo.spt[1] = JZXArray[iGJJD].HJZXStartPt.y;
				}

				Tmpcml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,
					JZXArray[iGJJD].HJZXEndPt.x,JZXArray[iGJJD].HJZXEndPt.y,
					JZXArray[iGJJD].HJZXEndPt.x,JZXArray[iGJJD].HJZXEndPt.y);	
				if (pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.lc < Tmpcml)
				{
					OneGjdInfo.ept[0] = pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.x;
					OneGjdInfo.ept[1] = pGTZXJY->JZXArray[iHJYJD].HJZXEndPt.y;
				}
				else
				{
					OneGjdInfo.ept[0] = JZXArray[iGJJD].HJZXEndPt.x;
					OneGjdInfo.ept[1] = JZXArray[iGJJD].HJZXEndPt.y;
				}

				Dist = sqrt((OneGjdInfo.spt[0]-GJXLSpt[0])*(OneGjdInfo.spt[0]-GJXLSpt[0]) + (OneGjdInfo.spt[1]-GJXLSpt[1])*(OneGjdInfo.spt[1]-GJXLSpt[1]));
				if (Dist>0.1)
				{
					LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.spt[0],OneGjdInfo.spt[1]);
					PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
					xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
					LYDSml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					PZ.lc = LYDSml;
					xlpoint_pz(array,tarray,&PZ);
					OneGjdInfo.spt[0] = PZ.x;
					OneGjdInfo.spt[1] = PZ.y;
				}

				Dist = sqrt((OneGjdInfo.ept[0]-GJXLEpt[0])*(OneGjdInfo.ept[0]-GJXLEpt[0]) + (OneGjdInfo.ept[1]-GJXLEpt[1])*(OneGjdInfo.ept[1]-GJXLEpt[1]));
				if (Dist>0.1)
				{
					LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,OneGjdInfo.ept[0],OneGjdInfo.ept[1],OneGjdInfo.ept[0],OneGjdInfo.ept[1]);
					PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
					xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
					LYDEml =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					PZ.lc = LYDEml;
					xlpoint_pz(array,tarray,&PZ);
					OneGjdInfo.ept[0] = PZ.x;
					OneGjdInfo.ept[1] = PZ.y;
				}
				_tcscpy(OneGjdInfo.BXLName , pGTZXJY->XLShuXing);
				_tcscpy(OneGjdInfo.EXLName , pGTZXJY->XLShuXing);
				OneGjdInfo.GJDType = -1;
				_tcscpy(OneGjdInfo.gjdname,L"���ö�");
				LYDInfArray.Add(OneGjdInfo);
			}
		}
	}

	int LYDSum = LYDInfArray.GetSize();
	if (LYDSum>0)
	{
		Tmpcml = PROJ_ML(array,tarray,LYDInfArray[0].spt[0],LYDInfArray[0].spt[1],LYDInfArray[0].spt[0],LYDInfArray[0].spt[1]);
		if (Tmpcml<SmlGJ)
		{
			LYDInfArray[0].spt[0] = Spt[0];
			LYDInfArray[0].spt[1] = Spt[1];
		}

		Tmpcml = PROJ_ML(array,tarray,LYDInfArray[LYDSum-1].ept[0],LYDInfArray[LYDSum-1].ept[1],LYDInfArray[LYDSum-1].ept[0],LYDInfArray[LYDSum-1].ept[1]);
		if (Tmpcml>EmlGJ)
		{
			LYDInfArray[LYDSum-1].ept[0] = Ept[0];
			LYDInfArray[LYDSum-1].ept[1] = Ept[1];
		}
	}

	MergeGTZX2GJDArray(LYDInfArray);

	return;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void GTZX2_JYX_ROAD::LocateHundredMeterOnLine_LYD(double Pt[2],bool IsBefore)
{
	double dist;
	dist = sqrt( (Pt[0]-JDarray[0].N) * (Pt[0]-JDarray[0].N) + (Pt[1]-JDarray[0].E) * (Pt[1]-JDarray[0].E));
	if (dist<0.001)
		return;
	dist = sqrt( (Pt[0]-JDarray[NJD-1].N) * (Pt[0]-JDarray[NJD-1].N) + (Pt[1]-JDarray[NJD-1].E) * (Pt[1]-JDarray[NJD-1].E));
	if (dist<0.001)
		return;

	double cml;
	xlpoint PZ;
	GTZX_JYX_ROAD* pGTZX;
	if (m_pm2==NULL)
		pGTZX = m_pm1;
	else
		pGTZX = m_pm2->m_pm1;

	cml = PROJ_ML(pGTZX->array,pGTZX->tarray,Pt[0],Pt[1],Pt[0],Pt[1]);
	PZ.lc = LocateHundredMeterOnLine(cml,IsBefore);
	xlpoint_pz(pGTZX->array,pGTZX->tarray,&PZ);
	cml = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
	PZ.lc = cml;
	xlpoint_pz(array,tarray,&PZ);
	Pt[0] = PZ.x;
	Pt[1] = PZ.y;

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�������߼����Ӧһ�ߵļ�ֱ������
void GTZX2_JYX_ROAD::Create2XZJXArray()
{
	CreatJZXArray();
	if (m_pm2==NULL)
	{
		m_pm1->CreatJZXArray();
	}
	else
	{
		m_pm2->CreatJZXArray();
	}

}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//������ֱ���ص��Ľ����±�
//bool GTZX2::SearchJZXCDJDIndex(int iJD2,int QorH,int iSJD1, int iEJD1,int &iJD1)
//{
//	bool isCD = false;//��ֱ���Ƿ��ص�
//	double SPt1X,SPt1Y,EPt1X,EPt1Y;//һ�����ϼ�ֱ�����յ�����
//	double SPt2X,SPt2Y,EPt2X,EPt2Y;//�������ϼ�ֱ�����յ�����
//	if (m_pm2==NULL)
//	{
//		if (QorH == -1)//�ж�ǰ��ֱ���Ƿ��ص�
//		{
//			if (iJD2==0)
//			{
//				double dist1 = m_pm1->DistFromPtToGTZX(JDarray[0].N,JDarray[0].E);
//				if (dist1<1e-6)
//				{
//					iJD1 = 0;
//					return true;
//				}
//				return false;
//			}
//			SPt2X = JZXArray[iJD2].QJZXStartPt.x;
//			SPt2Y = JZXArray[iJD2].QJZXStartPt.y;
//			EPt2X = JZXArray[iJD2].QJZXEndPt.x;
//			EPt2Y = JZXArray[iJD2].QJZXEndPt.y;
//
//			if (iSJD1<1)
//				iSJD1=1;
//			if (iEJD1>m_pm1->NJD-1)
//				iEJD1=m_pm1->NJD-1;
//
//			for (int i=iSJD1 ; i<=iEJD1 ; i++ )
//			{
//				SPt1X = m_pm1->JZXArray[i].QJZXStartPt.x;
//				SPt1Y = m_pm1->JZXArray[i].QJZXStartPt.y;
//				EPt1X = m_pm1->JZXArray[i].QJZXEndPt.x;
//				EPt1Y = m_pm1->JZXArray[i].QJZXEndPt.y;
//				isCD = IsZXCD(SPt1X,SPt1Y,EPt1X,EPt1Y,SPt2X,SPt2Y,EPt2X,EPt2Y);
//				if (isCD)
//				{
//					iJD1 = i;
//					isCD = true;
//					return true;
//				}
//			}
//		}
//
//		else
//		{
//			if (iJD2==NJD-1)
//			{
//				double dist2 = m_pm1->DistFromPtToGTZX(JDarray[NJD-1].N , JDarray[NJD-1].E);	
//				if (dist2<1e-6)
//				{
//					iJD1 = m_pm1->NJD-1;
//					return true;
//				}
//				return false;
//			}
//			SPt2X = JZXArray[iJD2].HJZXStartPt.x;
//			SPt2Y = JZXArray[iJD2].HJZXStartPt.y;
//			EPt2X = JZXArray[iJD2].HJZXEndPt.x;
//			EPt2Y = JZXArray[iJD2].HJZXEndPt.y;
//
//			if (iSJD1<0)
//				iSJD1=0;
//			if (iEJD1>m_pm1->NJD-2)
//				iEJD1=m_pm1->NJD-2;
//			for (int i=iSJD1 ; i<=iEJD1 ; i++)
//			{
//				SPt1X = m_pm1->JZXArray[i].HJZXStartPt.x;
//				SPt1Y = m_pm1->JZXArray[i].HJZXStartPt.y;
//				EPt1X = m_pm1->JZXArray[i].HJZXEndPt.x;
//				EPt1Y = m_pm1->JZXArray[i].HJZXEndPt.y;
//				isCD = IsZXCD(SPt1X,SPt1Y,EPt1X,EPt1Y,SPt2X,SPt2Y,EPt2X,EPt2Y);
//				if (isCD)
//				{
//					iJD1 = i;
//					isCD = true;
//					return true;
//				}
//			}
//		}
//		return false;
//	}
//
//	else
//	{
//		GTZX* m_pm2;
//		if (QorH == -1)//�ж�ǰ��ֱ���Ƿ��ص�
//		{
//			if (iJD2==0)
//			{
//				double dist1 = m_pm2->DistFromPtToGTZX(JDarray[0].N,JDarray[0].E);
//				if (dist1<1e-6)
//				{
//					iJD1 = 0;
//					return true;
//				}
//				return false;
//			}
//			SPt2X = JZXArray[iJD2].QJZXStartPt.x;
//			SPt2Y = JZXArray[iJD2].QJZXStartPt.y;
//			EPt2X = JZXArray[iJD2].QJZXEndPt.x;
//			EPt2Y = JZXArray[iJD2].QJZXEndPt.y;
//
//			if (iSJD1<1)
//				iSJD1=1;
//			if (iEJD1>m_pm2->NJD-1)
//				iEJD1=m_pm2->NJD-1;
//
//			for (int i=iSJD1 ; i<=iEJD1 ; i++ )
//			{
//				SPt1X = m_pm2->JZXArray[i].QJZXStartPt.x;
//				SPt1Y = m_pm2->JZXArray[i].QJZXStartPt.y;
//				EPt1X = m_pm2->JZXArray[i].QJZXEndPt.x;
//				EPt1Y = m_pm2->JZXArray[i].QJZXEndPt.y;
//				isCD = IsZXCD(SPt1X,SPt1Y,EPt1X,EPt1Y,SPt2X,SPt2Y,EPt2X,EPt2Y);
//				if (isCD)
//				{
//					iJD1 = i;
//					isCD = true;
//					return true;
//				}
//			}
//		}
//
//		else
//		{
//			if (iJD2==NJD-1)
//			{
//				double dist2 = m_pm2->DistFromPtToGTZX(JDarray[NJD-1].N , JDarray[NJD-1].E);	
//				if (dist2<1e-6)
//				{
//					iJD1 = m_pm2->NJD-1;
//					return true;
//				}
//				return false;
//			}
//			SPt2X = JZXArray[iJD2].HJZXStartPt.x;
//			SPt2Y = JZXArray[iJD2].HJZXStartPt.y;
//			EPt2X = JZXArray[iJD2].HJZXEndPt.x;
//			EPt2Y = JZXArray[iJD2].HJZXEndPt.y;
//
//			if (iSJD1<0)
//				iSJD1=0;
//			if (iEJD1>m_pm2->NJD-2)
//				iEJD1=m_pm2->NJD-2;
//			for (int i=iSJD1 ; i<=iEJD1 ; i++)
//			{
//				SPt1X = m_pm2->JZXArray[i].HJZXStartPt.x;
//				SPt1Y = m_pm2->JZXArray[i].HJZXStartPt.y;
//				EPt1X = m_pm2->JZXArray[i].HJZXEndPt.x;
//				EPt1Y = m_pm2->JZXArray[i].HJZXEndPt.y;
//				isCD = IsZXCD(SPt1X,SPt1Y,EPt1X,EPt1Y,SPt2X,SPt2Y,EPt2X,EPt2Y);
//				if (isCD)
//				{
//					iJD1 = i;
//					isCD = true;
//					return true;
//				}
//			}
//		}
//		return false;
//	}
//
//}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//������ֱ���ص��Ľ����±�
bool GTZX2_JYX_ROAD::SearchJZXCDJDIndex_OnJYJX(GTZX_JYX_ROAD* pJYJX,int iJD2,int QorH,int iSJD1, int iEJD1,int &iJD1)
{
	bool isCD = false;//��ֱ���Ƿ��ص�
	double SPt1X,SPt1Y,EPt1X,EPt1Y;//һ�����ϼ�ֱ�����յ�����
	double SPt2X,SPt2Y,EPt2X,EPt2Y;//�������ϼ�ֱ�����յ�����

	if (QorH == -1)//�ж�ǰ��ֱ���Ƿ��ص�
	{
		if (iJD2==0)
		{
			double dist1 = pJYJX->DistFromPtToGTZX(JDarray[0].N,JDarray[0].E);
			if (dist1<1e-6)
			{
				iJD1 = 0;
				return true;
			}
			return false;
		}
		SPt2X = JZXArray[iJD2].QJZXStartPt.x;
		SPt2Y = JZXArray[iJD2].QJZXStartPt.y;
		EPt2X = JZXArray[iJD2].QJZXEndPt.x;
		EPt2Y = JZXArray[iJD2].QJZXEndPt.y;

		if (iSJD1<1)
			iSJD1=1;
		if (iEJD1>pJYJX->NJD-1)
			iEJD1=pJYJX->NJD-1;

		for (int i=iSJD1 ; i<=iEJD1 ; i++ )
		{
			SPt1X = pJYJX->JZXArray[i].QJZXStartPt.x;
			SPt1Y = pJYJX->JZXArray[i].QJZXStartPt.y;
			EPt1X = pJYJX->JZXArray[i].QJZXEndPt.x;
			EPt1Y = pJYJX->JZXArray[i].QJZXEndPt.y;
			isCD = IsZXCD(SPt1X,SPt1Y,EPt1X,EPt1Y,SPt2X,SPt2Y,EPt2X,EPt2Y);
			if (isCD)
			{
				iJD1 = i;
				isCD = true;
				return true;
			}
		}
	}

	else
	{
		if (iJD2==NJD-1)
		{
			double dist2 = pJYJX->DistFromPtToGTZX(JDarray[NJD-1].N , JDarray[NJD-1].E);	
			if (dist2<1e-6)
			{
				iJD1 = pJYJX->NJD-1;
				return true;
			}
			return false;
		}
		SPt2X = JZXArray[iJD2].HJZXStartPt.x;
		SPt2Y = JZXArray[iJD2].HJZXStartPt.y;
		EPt2X = JZXArray[iJD2].HJZXEndPt.x;
		EPt2Y = JZXArray[iJD2].HJZXEndPt.y;

		if (iSJD1<0)
			iSJD1=0;
		if (iEJD1>pJYJX->NJD-2)
			iEJD1=pJYJX->NJD-2;
		for (int i=iSJD1 ; i<=iEJD1 ; i++)
		{
			SPt1X = pJYJX->JZXArray[i].HJZXStartPt.x;
			SPt1Y = pJYJX->JZXArray[i].HJZXStartPt.y;
			EPt1X = pJYJX->JZXArray[i].HJZXEndPt.x;
			EPt1Y = pJYJX->JZXArray[i].HJZXEndPt.y;
			isCD = IsZXCD(SPt1X,SPt1Y,EPt1X,EPt1Y,SPt2X,SPt2Y,EPt2X,EPt2Y);
			if (isCD)
			{
				iJD1 = i;
				isCD = true;
				return true;
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool GTZX2_JYX_ROAD::SearchJZXCDJDIndex_OnJYEX(GTZX2_JYX_ROAD* pJYEX,int iJD2,int QorH,int iSJD1, int iEJD1,int &iJD1)
{
	bool isCD = false;//��ֱ���Ƿ��ص�
	double SPt1X,SPt1Y,EPt1X,EPt1Y;//һ�����ϼ�ֱ�����յ�����
	double SPt2X,SPt2Y,EPt2X,EPt2Y;//�������ϼ�ֱ�����յ�����

	if (QorH == -1)//�ж�ǰ��ֱ���Ƿ��ص�
	{
		if (iJD2==0)
		{
			double dist1 = pJYEX->DistFromPtToGTZX(JDarray[0].N,JDarray[0].E);
			if (dist1<1e-6)
			{
				iJD1 = 0;
				return true;
			}
			return false;
		}
		SPt2X = JZXArray[iJD2].QJZXStartPt.x;
		SPt2Y = JZXArray[iJD2].QJZXStartPt.y;
		EPt2X = JZXArray[iJD2].QJZXEndPt.x;
		EPt2Y = JZXArray[iJD2].QJZXEndPt.y;

		if (iSJD1<1)
			iSJD1=1;
		if (iEJD1>pJYEX->NJD-1)
			iEJD1=pJYEX->NJD-1;

		for (int i=iSJD1 ; i<=iEJD1 ; i++ )
		{
			SPt1X = pJYEX->JZXArray[i].QJZXStartPt.x;
			SPt1Y = pJYEX->JZXArray[i].QJZXStartPt.y;
			EPt1X = pJYEX->JZXArray[i].QJZXEndPt.x;
			EPt1Y = pJYEX->JZXArray[i].QJZXEndPt.y;
			isCD = IsZXCD(SPt1X,SPt1Y,EPt1X,EPt1Y,SPt2X,SPt2Y,EPt2X,EPt2Y);
			if (isCD)
			{
				iJD1 = i;
				isCD = true;
				return true;
			}
		}
	}

	else
	{
		if (iJD2==NJD-1)
		{
			double dist2 = pJYEX->DistFromPtToGTZX(JDarray[NJD-1].N , JDarray[NJD-1].E);	
			if (dist2<1e-6)
			{
				iJD1 = pJYEX->NJD-1;
				return true;
			}
			return false;
		}
		SPt2X = JZXArray[iJD2].HJZXStartPt.x;
		SPt2Y = JZXArray[iJD2].HJZXStartPt.y;
		EPt2X = JZXArray[iJD2].HJZXEndPt.x;
		EPt2Y = JZXArray[iJD2].HJZXEndPt.y;

		if (iSJD1<0)
			iSJD1=0;
		if (iEJD1>pJYEX->NJD-2)
			iEJD1=pJYEX->NJD-2;
		for (int i=iSJD1 ; i<=iEJD1 ; i++)
		{
			SPt1X = pJYEX->JZXArray[i].HJZXStartPt.x;
			SPt1Y = pJYEX->JZXArray[i].HJZXStartPt.y;
			EPt1X = pJYEX->JZXArray[i].HJZXEndPt.x;
			EPt1Y = pJYEX->JZXArray[i].HJZXEndPt.y;
			isCD = IsZXCD(SPt1X,SPt1Y,EPt1X,EPt1Y,SPt2X,SPt2Y,EPt2X,EPt2Y);
			if (isCD)
			{
				iJD1 = i;
				isCD = true;
				return true;
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*********************************************************************************
�������ܣ��������иĽ�������
�������:
Spt[2]		�������иĽ��ε����
Ept[2]		�������иĽ��ε��յ�
RXGJDArray	���иĽ�������
*********************************************************************************/
void GTZX2_JYX_ROAD::CreateRXGJD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>& RXGJDArray)
{
	GJDInf OneGjdInfo;
	xlpoint PZ;
	double sml,eml;
	sml = PROJ_ML(array,tarray,Spt[0],Spt[1],Spt[0],Spt[1]);
	eml = PROJ_ML(array,tarray,Ept[0],Ept[1],Ept[0],Ept[1]);
	if (eml-sml<0.1)
		return;

	RXGJDArray.RemoveAll();
	int RxdNum=GetRXDNum();
	for(int i=0;i<RxdNum;i++)
	{
		//��Ѱĳ���жε����ն������
		int sDLno = 0 ;
		int eDLno = 0 ;
		GetiRXDDLinfo(i , sDLno , eDLno);
		if(max(DLB2x[sDLno].TYLC,sml) < min(DLB2x[eDLno].TYLC,eml))//���ж����ѯ��Χ�н���
		{
			PZ.lc = DLB2x[sDLno].TYLC > sml ? DLB2x[sDLno].TYLC : sml;
			xlpoint_pz(array,tarray,&PZ);
			OneGjdInfo.spt[0] = PZ.x;
			OneGjdInfo.spt[1] = PZ.y;

			PZ.lc = DLB2x[eDLno].TYLC < eml ? DLB2x[eDLno].TYLC : eml;
			xlpoint_pz(array,tarray,&PZ);
			OneGjdInfo.ept[0] = PZ.x;
			OneGjdInfo.ept[1] = PZ.y;

			OneGjdInfo.GJDType = 2;
			SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],2,OneGjdInfo.gjdname);
			//			SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
			//////////////////////////////////////////////////////////////////////////
			//�ݶ�Ϊ���л���
			if (_tcscmp(xlname,L"��ͨ��Ƣ���")==0)
			{
				_tcscpy(OneGjdInfo.BXLName,L"���л���");
				_tcscpy(OneGjdInfo.EXLName,L"���л���");
			}
			//////////////////////////////////////////////////////////////////////////
			else
			{
				if (m_pm2==NULL)
				{
					_tcscpy(OneGjdInfo.BXLName,m_pm1->XLShuXing);
					_tcscpy(OneGjdInfo.EXLName,m_pm1->XLShuXing);
				}
				else
				{
					_tcscpy(OneGjdInfo.BXLName,m_pm2->m_pm1->XLShuXing);
					_tcscpy(OneGjdInfo.EXLName,m_pm2->m_pm1->XLShuXing);
				}
			}

			RXGJDArray.Add(OneGjdInfo);
		}

	}

}


/////////////////////////////////////////////////////////////////////////
//��Թ�ͨ��ƶ��߲��жΣ��������յ�λ�����ö���Ϣ���飬����Ľ�����Ϣ����
/*********************************************************************************
�������ܣ�������ͨ��ƶ����ϲ��в��ֵĸĽ�����Ϣ����
���������
Spt[2]		�������
Ept[2]		�����յ�
LYDArray	���ö�����
���������
GJDArray	�Ľ�������
**********************************************************************************/
void GTZX2_JYX_ROAD::CreateGJDInGT2XBXD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray)
{
	xlpoint PZ;
	GJDArray.RemoveAll();
	//1.����sml,eml,�����ö����飬�γɸĽ������յ�λ����
	CArray<GJDInf,GJDInf>  GJDArrayByLYDArray,OneGJDArray;
	CreateGJDPt_By_LYDPt(Spt,Ept,LYDArray,GJDArrayByLYDArray);
	//2.���ݸĽ������յ�λ������ȡ��ͨ��ƻ��ߵĸĽ�����Ϣ
	for(int i=0; i<GJDArrayByLYDArray.GetSize(); i++)
	{
		double GJD_Spt[2],GJD_Ept[2];
		GJD_Spt[0] = GJDArrayByLYDArray[i].spt[0];
		GJD_Spt[1] = GJDArrayByLYDArray[i].spt[1];
		GJD_Ept[0] = GJDArrayByLYDArray[i].ept[0];
		GJD_Ept[1] = GJDArrayByLYDArray[i].ept[1];
		m_pm1->getGJDInfo(GJD_Spt,GJD_Ept,OneGJDArray);//������ͨ���ߵĲ��иĽ���Ϣ
		for (int j=0; j<OneGJDArray.GetSize(); j++)//�����ж����յ����귴ͶӰ����ͨ��ƶ�����
		{
			PZ.lc = PROJ_ML(array,tarray,OneGJDArray[j].spt[0],OneGJDArray[j].spt[1],OneGJDArray[j].spt[0],OneGJDArray[j].spt[1]);
			xlpoint_pz(array,tarray,&PZ);
			OneGJDArray[j].spt[0] = PZ.x;
			OneGJDArray[j].spt[1] = PZ.y;
			PZ.lc = PROJ_ML(array,tarray,OneGJDArray[j].ept[0],OneGJDArray[j].ept[1],OneGJDArray[j].ept[0],OneGJDArray[j].ept[1]);
			xlpoint_pz(array,tarray,&PZ);
			OneGJDArray[j].ept[0] = PZ.x;
			OneGJDArray[j].ept[1] = PZ.y;

			//////////////////////////////////////////////////////////////////////////
			//��ͨ��ƶ�������Ľ����յ㲻�ڹ�ͨ����ֱ�߶���Ҫ����
			if (_tcscpy(xlname, L"��ͨ��Ƣ���"))
			{
				double SmlOnGTJX,EmlOnGTJX;
				int k;
				int iXY,XYClass;
				SmlOnGTJX = PROJ_ML(m_pm1->array,m_pm1->tarray,OneGJDArray[j].spt[0],OneGJDArray[j].spt[1],OneGJDArray[j].spt[0],OneGJDArray[j].spt[1]);
				for(k=m_pm1->tarray; k>=0; k--)
				{
					if(SmlOnGTJX >= m_pm1->array[k][6])
					{
						iXY=k;
						XYClass = m_pm1->array[k][0];
						break;
					}
				}
				if (XYClass!=1 && XYClass!=10)
				{
					SmlOnGTJX = LocateHundredMeterOnLine(SmlOnGTJX,true);
					PZ.lc = SmlOnGTJX;
					xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);
					PZ.lc = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					xlpoint_pz(array,tarray,&PZ);
					OneGJDArray[j].spt[0] = PZ.x;
					OneGJDArray[j].spt[1] = PZ.y;
				}

				EmlOnGTJX = PROJ_ML(m_pm1->array,m_pm1->tarray,OneGJDArray[j].ept[0],OneGJDArray[j].ept[1],OneGJDArray[j].ept[0],OneGJDArray[j].ept[1]);
				for(k=m_pm1->tarray; k>=0; k--)
				{
					if(EmlOnGTJX >= m_pm1->array[k][6])
					{
						iXY=k;
						XYClass = m_pm1->array[k][0];
						break;
					}
				}
				if (XYClass!=1 && XYClass!=10)
				{
					EmlOnGTJX = LocateHundredMeterOnLine(EmlOnGTJX,false);
					PZ.lc = EmlOnGTJX;
					xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);
					PZ.lc = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					xlpoint_pz(array,tarray,&PZ);
					OneGJDArray[j].ept[0] = PZ.x;
					OneGJDArray[j].ept[1] = PZ.y;
				}
			}
		}
		GJDArray.Append(OneGJDArray);
	}
	return;
}

/////////////////////////////////////////////////////////////////////////
//��Թ�ͨ��ƶ��߲��жΣ��������յ�λ�����ö���Ϣ���飬����Ľ�����Ϣ����
void GTZX2_JYX_ROAD::CreateGJDInGT2XRXD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray)
{
	GJDArray.RemoveAll();
	//1.����sml,eml,�����ö����飬�γɸĽ������յ�λ����
	CArray<GJDInf,GJDInf>  GJDArrayByLYDArray,OneGJDArray;
	CreateGJDPt_By_LYDPt(Spt,Ept,LYDArray,GJDArrayByLYDArray);
	//2.���ݸĽ������յ�λ������ȡ��ͨ��ƻ��ߵĸĽ�����Ϣ
	for(int i=0; i<GJDArrayByLYDArray.GetSize(); i++)
	{
		double GJD_Spt[2],GJD_Ept[2];
		GJD_Spt[0] = GJDArrayByLYDArray[i].spt[0];
		GJD_Spt[1] = GJDArrayByLYDArray[i].spt[1];
		GJD_Ept[0] = GJDArrayByLYDArray[i].ept[0];
		GJD_Ept[1] = GJDArrayByLYDArray[i].ept[1];
		CreateRXGJD(GJD_Spt,GJD_Ept,OneGJDArray);//������ͨ���ߵĲ��иĽ���Ϣ
		GJDArray.Append(OneGJDArray);
	}
}

/********************************************************************************
�������ܣ������ö��������յ㣬�ڲ���Ľ������յ�
���������
Spt[2]		�������
Ept[2]		�����յ�
LYDArray	���ö�����
���������
GJDArray	�Ľ�������
*********************************************************************************/
void GTZX2_JYX_ROAD::CreateGJDPt_By_LYDPt(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray)
{
	GJDArray.RemoveAll();
	GJDInf OneGjdInfo;
	xlpoint PZ;
	double sml,eml;
	double lyd_sml,lyd_eml;
	sml = PROJ_ML(array,tarray,Spt[0],Spt[1],Spt[0],Spt[1]);
	eml = PROJ_ML(array,tarray,Ept[0],Ept[1],Ept[0],Ept[1]);
	if (eml-sml<0.1)
		return;

	double PtArr[1000][3];//z�����ʾ������0��ʾ���ö���ֹ��1-���ö���ʼ
	PtArr[0][0]=Spt[0],PtArr[0][1]=Spt[1],PtArr[0][2]=0.0;

	int LYDNum=LYDArray.GetSize();
	if (LYDNum<1)
	{
		GJDArray.SetSize(1);
		GJDArray[0].spt[0] = Spt[0];
		GJDArray[0].spt[1] = Spt[1];
		GJDArray[0].ept[0] = Ept[0];
		GJDArray[0].ept[1] = Ept[1];
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//�ж�Spt,Ept��Χ����û�����öΣ�û�еĻ�Spt,Ept����Ϊ�Ľ�
	int IsInRange = false;
	double TempSml,TempEml;
	for (int iLYD=0; iLYD<LYDArray.GetSize(); iLYD++)
	{
		lyd_sml = PROJ_ML(array,tarray,LYDArray[iLYD].spt[0],LYDArray[iLYD].spt[1],LYDArray[iLYD].spt[0],LYDArray[iLYD].spt[1]);
		lyd_eml = PROJ_ML(array,tarray,LYDArray[iLYD].ept[0],LYDArray[iLYD].ept[1],LYDArray[iLYD].ept[0],LYDArray[iLYD].ept[1]);
		TempSml = max(sml,lyd_sml);
		TempEml = min(eml,lyd_eml);
		if (TempEml-TempSml>10.0)
		{
			IsInRange = true;
			break;
		}
	}
	if (!IsInRange)
	{
		GJDArray.SetSize(1);
		GJDArray[0].spt[0] = Spt[0];
		GJDArray[0].spt[1] = Spt[1];
		GJDArray[0].ept[0] = Ept[0];
		GJDArray[0].ept[1] = Ept[1];
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//��㵽��һ�����ö�
	lyd_sml = PROJ_ML(array,tarray,LYDArray[0].spt[0],LYDArray[0].spt[1],LYDArray[0].spt[0],LYDArray[0].spt[1]);
	if (lyd_sml-sml>0.1 && eml-lyd_sml>0.1)
	{
		OneGjdInfo.spt[0] = Spt[0];
		OneGjdInfo.spt[1] = Spt[1];
		OneGjdInfo.ept[0] = LYDArray[0].spt[0];
		OneGjdInfo.ept[1] = LYDArray[0].spt[1];
		GJDArray.Add(OneGjdInfo);
	}
	for(int i=0;i<LYDNum-1;i++)
	{
		lyd_eml = PROJ_ML(array,tarray,LYDArray[i].ept[0],LYDArray[i].ept[1],LYDArray[i].ept[0],LYDArray[i].ept[1]);
		lyd_sml = PROJ_ML(array,tarray,LYDArray[i+1].spt[0],LYDArray[i+1].spt[1],LYDArray[i+1].spt[0],LYDArray[i+1].spt[1]);
		double GJSml,GJEml;
		GJSml = max(lyd_eml,sml);
		GJEml = min(lyd_sml,eml);
		if (GJEml-GJSml>0.1)
		{
			if (lyd_eml>sml)
			{
				OneGjdInfo.spt[0] = LYDArray[i].ept[0];
				OneGjdInfo.spt[1] = LYDArray[i].ept[1];
			}
			else
			{
				OneGjdInfo.spt[0] = Spt[0];
				OneGjdInfo.spt[1] = Spt[1];
			}
			if (lyd_sml<eml)
			{
				OneGjdInfo.ept[0] = LYDArray[i+1].spt[0];
				OneGjdInfo.ept[1] = LYDArray[i+1].spt[1];
			}
			else
			{
				OneGjdInfo.ept[0] = Ept[0];
				OneGjdInfo.ept[1] = Ept[1];
			}
			GJDArray.Add(OneGjdInfo);
		}
	}

	lyd_eml = PROJ_ML(array,tarray,LYDArray[LYDNum-1].ept[0],LYDArray[LYDNum-1].ept[1],LYDArray[LYDNum-1].ept[0],LYDArray[LYDNum-1].ept[1]);
	if (lyd_eml-sml>0.1 && eml-lyd_eml>0.1)
	{
		OneGjdInfo.spt[0] = LYDArray[LYDNum-1].ept[0];
		OneGjdInfo.spt[1] = LYDArray[LYDNum-1].ept[1];
		OneGjdInfo.ept[0] = Ept[0];
		OneGjdInfo.ept[1] = Ept[1];
		GJDArray.Add(OneGjdInfo);
	}
}

void GTZX2_JYX_ROAD::CreateAllGJD(CArray<GJDInf,GJDInf>& BXGJDArray,CArray<GJDInf,GJDInf>& RXGJDArray,CArray<GJDInf,GJDInf>& AllGJDArray)
{
	AllGJDArray.RemoveAll();
	AllGJDArray.Append(BXGJDArray);
	AllGJDArray.Append(RXGJDArray);
	SortGTZX2GJDArray(AllGJDArray);
	MergeGTZX2GJDArray(AllGJDArray);
}

void GTZX2_JYX_ROAD::RefreshGTZX2GJDArray(double Sml, double Eml, CArray<GJDInf,GJDInf>& OriGJDArray, CArray<GJDInf,GJDInf>& NewGJDArray, CArray<GJDInf,GJDInf>& RefreshGJDArray)
{
	double GJDSml,GJDEml;
	double GJDSpt[2],GJDEpt[2];
	double FDSml,FDEml;
	GJDInf TempGJD;
	xlpoint PZ;
	RefreshGJDArray.RemoveAll();

	for (int iGJD=0; iGJD<OriGJDArray.GetSize(); iGJD++)
	{
		GJDSpt[0] = OriGJDArray[iGJD].spt[0];
		GJDSpt[1] = OriGJDArray[iGJD].spt[1];
		GJDEpt[0] = OriGJDArray[iGJD].ept[0];
		GJDEpt[1] = OriGJDArray[iGJD].ept[1];
		GJDSml = PROJ_ML(array,tarray,GJDSpt[0],GJDSpt[1],GJDSpt[0],GJDSpt[1]);
		GJDEml = PROJ_ML(array,tarray,GJDEpt[0],GJDEpt[1],GJDEpt[0],GJDEpt[1]);

		FDSml = max(GJDSml,Sml);
		FDEml = min(GJDEml,Eml);
		if (FDEml-FDSml>1.0)//�н���,ȡ�����ĸĽ�����Ϣ
		{
			TempGJD = OriGJDArray[iGJD];
			if (FDSml<Sml-0.001)
			{	
				PZ.lc = FDSml;
				xlpoint_pz(array,tarray,&PZ);
				TempGJD.spt[0] = PZ.x;
				TempGJD.spt[1] = PZ.y;
			}
			if (FDEml>Eml+0.001)
			{
				PZ.lc = FDEml;
				xlpoint_pz(array,tarray,&PZ);
				TempGJD.ept[0] = PZ.x;
				TempGJD.ept[1] = PZ.y;
			}
			RefreshGJDArray.Add(TempGJD);
		}

		else//û�н���
			RefreshGJDArray.Add(OriGJDArray[iGJD]);
	}

	for (int iGJD=0; iGJD<NewGJDArray.GetSize(); iGJD++)
	{
		GJDSpt[0] = NewGJDArray[iGJD].spt[0];
		GJDSpt[1] = NewGJDArray[iGJD].spt[1];
		GJDEpt[0] = NewGJDArray[iGJD].ept[0];
		GJDEpt[1] = NewGJDArray[iGJD].ept[1];
		GJDSml = PROJ_ML(array,tarray,GJDSpt[0],GJDSpt[1],GJDSpt[0],GJDSpt[1]);
		GJDEml = PROJ_ML(array,tarray,GJDEpt[0],GJDEpt[1],GJDEpt[0],GJDEpt[1]);

		if (GJDSml>Eml+1.0)
			break;
		FDSml = max(Sml,GJDSml);
		FDEml = min(Eml,GJDEml);
		if (FDEml-FDSml>1.0)//�н���
		{
			TempGJD = NewGJDArray[iGJD];
			if (FDSml<Sml-0.001)
			{	
				PZ.lc = FDSml;
				xlpoint_pz(array,tarray,&PZ);
				TempGJD.spt[0] = PZ.x;
				TempGJD.spt[1] = PZ.y;
			}
			if (FDEml>Eml+0.001)
			{
				PZ.lc = FDEml;
				xlpoint_pz(array,tarray,&PZ);
				TempGJD.ept[0] = PZ.x;
				TempGJD.ept[1] = PZ.y;
			}
			RefreshGJDArray.Add(TempGJD);
		}
	}
}

void GTZX2_JYX_ROAD::RefreshGTZX2DLArray(double Sml, double Eml,int OriNDL, EXDLB OriEXDLB[], int NewNDL, EXDLB NewEXDLB[],int& RefreshNDL, EXDLB RefreshEXDLB[])
{
	RefreshNDL=0;
	double DLCml;
	EXDLB TempDLB;
	for (int iDL=0; iDL<OriNDL; iDL++)
	{
		DLCml = OriEXDLB[iDL].TYLC;
		if (!(DLCml>Sml-0.001 && DLCml<Eml+0.001))
		{
			RefreshEXDLB[RefreshNDL] = OriEXDLB[iDL];
			RefreshNDL++;
		}
	}
	for (int iDL=0; iDL<NewNDL; iDL++)
	{
		DLCml = NewEXDLB[iDL].TYLC;
		if (DLCml>Sml-0.001 && DLCml<Eml+0.001)
		{
			RefreshEXDLB[RefreshNDL] = NewEXDLB[iDL];
			RefreshNDL++;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//����
	for (int i=0; i<RefreshNDL-1; i++)
	{
		for (int j=i+1; j<RefreshNDL; j++)
		{
			if (RefreshEXDLB[i].TYLC>RefreshEXDLB[j].TYLC)
			{
				TempDLB = RefreshEXDLB[i];
				RefreshEXDLB[i] = RefreshEXDLB[j];
				RefreshEXDLB[j] = TempDLB;
			}
		}
	}
}

void GTZX2_JYX_ROAD::SortGTZX2GJDArray(CArray<GJDInf,GJDInf>& AllGJDArray)
{	
	int GJDSum = AllGJDArray.GetSize();
	GJDInf TmpGJD;
	double cml1,cml2;
	for (int i=0; i<GJDSum-1; i++)
	{
		for (int j=i+1; j<GJDSum; j++)
		{
			cml1 = PROJ_ML(array,tarray,AllGJDArray[i].spt[0],AllGJDArray[i].spt[1],AllGJDArray[i].spt[0],AllGJDArray[i].spt[1]);
			cml2 = PROJ_ML(array,tarray,AllGJDArray[j].spt[0],AllGJDArray[j].spt[1],AllGJDArray[j].spt[0],AllGJDArray[j].spt[1]);
			if (cml1>cml2)
			{
				TmpGJD = AllGJDArray[i];
				AllGJDArray[i] = AllGJDArray[j];
				AllGJDArray[j] = TmpGJD;
			}	
		}
	}
}


void GTZX2_JYX_ROAD::MergeGTZX2GJDArray(CArray<GJDInf,GJDInf>& AllGJDArray)
{	
	for (int iGJD=0; iGJD<AllGJDArray.GetSize()-1; iGJD++)
	{
		double sml,eml;
		eml = PROJ_ML(array,tarray,AllGJDArray[iGJD].ept[0],AllGJDArray[iGJD].ept[1],AllGJDArray[iGJD].ept[0],AllGJDArray[iGJD].ept[1]);
		sml = PROJ_ML(array,tarray,AllGJDArray[iGJD+1].spt[0],AllGJDArray[iGJD+1].spt[1],AllGJDArray[iGJD+1].spt[0],AllGJDArray[iGJD+1].spt[1]);

		GJDInf GJD1,GJD2;
		GJD1 = AllGJDArray[iGJD];
		GJD2 = AllGJDArray[iGJD+1];
		if (fabs(sml-eml)<0.1 || eml-sml>=0.1)//ǰ��Ľ��ηǳ��ӽ��򽻲�
		{
			if (eml-sml>0.1)//ǰ��Ľ��γ��ֽ���ϲ����Ľ��Ρ�
			{
				if (AllGJDArray[iGJD].GJDType != AllGJDArray[iGJD+1].GJDType)//���Ľ������Ͳ�ͬʱȡ���иĽ�
					AllGJDArray[iGJD].GJDType = 2;
				AllGJDArray[iGJD].ept[0] = AllGJDArray[iGJD+1].ept[0];
				AllGJDArray[iGJD].ept[1] = AllGJDArray[iGJD+1].ept[1];
				AllGJDArray.RemoveAt(iGJD+1);
				iGJD--;
			}
			else//ǰ�����Ľ��ηǳ��ӽ�
			{
				if (AllGJDArray[iGJD].GJDType == AllGJDArray[iGJD+1].GJDType)//�Ľ��ηǳ��ӽ��ҸĽ�������ͬʱ�����Ľ��κϲ�
				{
					AllGJDArray[iGJD].ept[0] = AllGJDArray[iGJD+1].ept[0];
					AllGJDArray[iGJD].ept[1] = AllGJDArray[iGJD+1].ept[1];
					AllGJDArray.RemoveAt(iGJD+1);
					iGJD--;
				}
			}
		}
	}
}

void GTZX2_JYX_ROAD::CreateLYDAndBXGJDArray(double spt[2],double ept[2],CArray<GJDInf,GJDInf>& LYDArray,CArray<GJDInf,GJDInf>& BXGJDArray)
{
	double sml,eml,cml;
	double LYDSml,LYDEml;
	GJDInf TmpGJD;
	xlpoint PZ;
	GTZX_JYX_ROAD* pGTZXJY = NULL;
	sml = PROJ_ML(array,tarray,spt[0],spt[1],spt[0],spt[1]);
	eml = PROJ_ML(array,tarray,ept[0],ept[1],ept[0],ept[1]);
	if (eml-sml<0.1)
		return;
	if (m_pm2==NULL)
		pGTZXJY = m_pm1;
	else
		pGTZXJY = m_pm2->m_pm1;
	sml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,spt[0],spt[1],spt[0],spt[1]);
	eml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,ept[0],ept[1],ept[0],ept[1]);
	//�������ж��е����ö�
	CreateLYDArray(spt,ept,LYDArray);
	int LYDSum = LYDArray.GetSize();
	if (LYDSum==0)
	{
		BXGJDArray.SetSize(1);
		BXGJDArray[0].spt[0] = spt[0];
		BXGJDArray[0].spt[1] = spt[1];
		BXGJDArray[0].ept[0] = ept[0];
		BXGJDArray[0].ept[1] = ept[1];
		BXGJDArray[0].GJDType = 0;
		SetDefaultGjdName(spt[0],spt[1],0,BXGJDArray[0].gjdname);
		//		SetDefaultGjdName(BXGJDArray[0].GJDType,BXGJDArray[0].gjdname);
		_tcscpy(BXGJDArray[0].BXLName,pGTZXJY->XLShuXing);
		_tcscpy(BXGJDArray[0].EXLName,pGTZXJY->XLShuXing);
		return;
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		//��㵽��һ�����ö����
		LYDSml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,LYDArray[0].spt[0],LYDArray[0].spt[1],LYDArray[0].spt[0],LYDArray[0].spt[1]);
		if (LYDSml-sml>0.1)
		{
			TmpGJD.spt[0] = spt[0];
			TmpGJD.spt[1] = spt[1];
			TmpGJD.ept[0] = LYDArray[0].spt[0];
			TmpGJD.ept[1] = LYDArray[0].spt[1];
			PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDSml,false);
			xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
			cml = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
			PZ.lc = cml;
			xlpoint_pz(array,tarray,&PZ);
			TmpGJD.ept[0] = PZ.x;
			TmpGJD.ept[1] = PZ.y;
			TmpGJD.GJDType = 0;
			SetDefaultGjdName(TmpGJD.spt[0],TmpGJD.spt[1],0,TmpGJD.gjdname);
			//			SetDefaultGjdName(TmpGJD.GJDType,TmpGJD.gjdname);
			_tcscpy(TmpGJD.BXLName,pGTZXJY->XLShuXing);
			_tcscpy(TmpGJD.EXLName,pGTZXJY->XLShuXing);
			BXGJDArray.Add(TmpGJD);
		}

		//////////////////////////////////////////////////////////////////////////
		//
		for (int iLYD=0; iLYD<LYDSum-1; iLYD++)
		{
			TmpGJD.spt[0] = LYDArray[iLYD].ept[0];
			TmpGJD.spt[1] = LYDArray[iLYD].ept[1];
			TmpGJD.ept[0] = LYDArray[iLYD+1].spt[0];
			TmpGJD.ept[1] = LYDArray[iLYD+1].spt[1];
			cml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,TmpGJD.spt[0],TmpGJD.spt[1],TmpGJD.spt[0],TmpGJD.spt[1]);
			PZ.lc = pGTZXJY->LocateHundredMeterOnLine(cml,true);
			xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
			cml = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
			PZ.lc = cml;
			xlpoint_pz(array,tarray,&PZ);
			TmpGJD.spt[0] = PZ.x;
			TmpGJD.spt[1] = PZ.y;

			cml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,TmpGJD.ept[0],TmpGJD.ept[1],TmpGJD.ept[0],TmpGJD.ept[1]);
			PZ.lc = pGTZXJY->LocateHundredMeterOnLine(cml,false);
			xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
			cml = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
			PZ.lc = cml;
			xlpoint_pz(array,tarray,&PZ);
			TmpGJD.ept[0] = PZ.x;
			TmpGJD.ept[1] = PZ.y;
			TmpGJD.GJDType = 0;
			SetDefaultGjdName(TmpGJD.spt[0],TmpGJD.spt[1],0,TmpGJD.gjdname);
			//			SetDefaultGjdName(TmpGJD.GJDType,TmpGJD.gjdname);
			_tcscpy(TmpGJD.BXLName,pGTZXJY->XLShuXing);
			_tcscpy(TmpGJD.EXLName,pGTZXJY->XLShuXing);
			BXGJDArray.Add(TmpGJD);
		}

		//////////////////////////////////////////////////////////////////////////
		LYDEml = PROJ_ML(pGTZXJY->array,pGTZXJY->tarray,LYDArray[LYDSum-1].ept[0],LYDArray[LYDSum-1].ept[1],LYDArray[LYDSum-1].ept[0],LYDArray[LYDSum-1].ept[1]);
		if (eml-LYDEml>0.1)
		{
			TmpGJD.ept[0] = ept[0];
			TmpGJD.ept[1] = ept[1];
			TmpGJD.spt[0] = LYDArray[0].spt[0];
			TmpGJD.spt[1] = LYDArray[0].spt[1];
			PZ.lc = pGTZXJY->LocateHundredMeterOnLine(LYDEml,true);
			xlpoint_pz(pGTZXJY->array,pGTZXJY->tarray,&PZ);
			cml = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
			PZ.lc = cml;
			xlpoint_pz(array,tarray,&PZ);
			TmpGJD.spt[0] = PZ.x;
			TmpGJD.spt[1] = PZ.y;
			TmpGJD.GJDType = 0;
			SetDefaultGjdName(TmpGJD.spt[0],TmpGJD.spt[1],0,TmpGJD.gjdname);
			//			SetDefaultGjdName(TmpGJD.GJDType,TmpGJD.gjdname);
			_tcscpy(TmpGJD.BXLName,pGTZXJY->XLShuXing);
			_tcscpy(TmpGJD.EXLName,pGTZXJY->XLShuXing);
			BXGJDArray.Add(TmpGJD);
		}
	}
}

/****************************************************************************************************
�������ܣ�������ͨ��ƻ��߸Ľ�������
���������
spt[2]		�����Ľ��ε��������
ept[2]		�����Ľ��ε��յ�����
AllGJDArray	��������Χ�ڵĸĽ�������
LYDArray	��������Χ�ڵ����ö�����
*******************************************************************************************************/
void GTZX2_JYX_ROAD::CreateGTJXGJDArray(double spt[2],double ept[2],CArray<GJDInf,GJDInf>& AllGJDArray,CArray<GJDInf,GJDInf>& LYDArray)
{
	CArray<GJDInf,GJDInf> BXGJDArray,RXGJDArray,TmpBXGJDArray,TmpLYDArray;
	double RXGJSml,RXGJEml;
	double sml,eml;
	double RXGJSpt[2],RXGJEpt[2];
	sml = PROJ_ML(array,tarray,spt[0],spt[1],spt[0],spt[1]);
	eml = PROJ_ML(array,tarray,ept[0],ept[1],ept[0],ept[1]);
	if (eml-sml<0.1)
		return;

	CreateRXGJD(spt,ept,RXGJDArray);
	int RXGJDSum = RXGJDArray.GetSize();
	if (RXGJDSum==0)
	{
		CreateLYDAndBXGJDArray(spt,ept,LYDArray,BXGJDArray);
	}
	else
	{
		RXGJSml = PROJ_ML(array,tarray,RXGJDArray[0].spt[0],RXGJDArray[0].spt[1],RXGJDArray[0].spt[0],RXGJDArray[0].spt[1]);
		if (RXGJSml-sml>0.1)
		{
			CreateLYDAndBXGJDArray(spt,RXGJDArray[0].spt,TmpLYDArray,TmpBXGJDArray);
			LYDArray.Append(TmpLYDArray);
			BXGJDArray.Append(TmpBXGJDArray);
			TmpLYDArray.RemoveAll();
			TmpBXGJDArray.RemoveAll();
		}

		//////////////////////////////////////////////////////////////////////////
		//
		for (int iRXGJD=0; iRXGJD<RXGJDSum-1; iRXGJD++)
		{
			CreateLYDAndBXGJDArray(RXGJDArray[iRXGJD].ept,RXGJDArray[iRXGJD+1].spt,TmpLYDArray,TmpBXGJDArray);
			LYDArray.Append(TmpLYDArray);
			BXGJDArray.Append(TmpBXGJDArray);
			TmpLYDArray.RemoveAll();
			TmpBXGJDArray.RemoveAll();
		}

		//////////////////////////////////////////////////////////////////////////
		RXGJEml = PROJ_ML(array,tarray,RXGJDArray[RXGJDSum-1].ept[0],RXGJDArray[RXGJDSum-1].ept[1],RXGJDArray[RXGJDSum-1].ept[0],RXGJDArray[RXGJDSum-1].ept[1]);
		if (eml-RXGJEml>0.1)
		{
			CreateLYDAndBXGJDArray(RXGJDArray[RXGJDSum-1].ept,ept,TmpLYDArray,TmpBXGJDArray);
			LYDArray.Append(TmpLYDArray);
			BXGJDArray.Append(TmpBXGJDArray);
			TmpLYDArray.RemoveAll();
			TmpBXGJDArray.RemoveAll();
		}
	}

	AllGJDArray.Append(RXGJDArray);
	AllGJDArray.Append(BXGJDArray);
	SortGTZX2GJDArray(AllGJDArray);
	MergeGTZX2GJDArray(AllGJDArray);
}

//���ݸĽ�����Ϣ���飬�޸�II�߶�����
void GTZX2_JYX_ROAD::ModifyDLB2xByGJDArray()
{
	int RXDSum = 0;
	int RXDJDSum = 0;
	int RXDSJDIndex = 0;
	int RXDEJDIndex = 0;
	CArray<EXDLB,EXDLB> InsertDLBArray;
	double RXDSml,RXDEml;
	double GJDSml,GJDEml;
	CString RXDName;
	double SectionSpt[2],SectionEpt[2];
	double RXDSpt[2],RXDEpt[2];
	double FDSpt[2],FDEPt[2];
	double SectionSml,SectionEml;
	double FDSml,FDEml;
	xlpoint PZ;
	bool IsModifySDL,IsModifyEDL;
	EXDLB RXSDL,RXEDL;
	IsModifySDL = IsModifyEDL = false;
	int SDL,EDL,RemoveDLSum;
	CArray<EXDLB,EXDLB> AllInsertEXDLBArray;//������Ҫ�����ͨ�������жεĶ�������
	AllInsertEXDLBArray.RemoveAll();

	RXDSum = GetRXDNum();
	//////////////////////////////////////////////////////////////////////////
	//�����ж��ڳ����յ���Ķ��߶���ȫ��ɾ��
	for (int iRXD=0; iRXD<RXDSum; iRXD++)
	{
		GetiRXDDLinfo(iRXD,SDL,EDL);
		RemoveDLSum = EDL - SDL - 1;
		if (RemoveDLSum>0)
		{
			for (int iEXDL=SDL+1; iEXDL+RemoveDLSum<NDL; iEXDL++)
			{
				DLB2x[iEXDL] = DLB2x[iEXDL+RemoveDLSum];
			}
			NDL -= RemoveDLSum;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	int iSecCount = BasicSectionInfor.GetSize();
	for (int iRXD=0; iRXD < RXDSum; iRXD++)
	{
		IsModifySDL = IsModifyEDL = false;

		GetiRXDDLinfo(iRXD,SDL,EDL);
		GetiRxdStartEndMlByNum(iRXD,RXDSml,RXDEml);

		//�õ����ж���ʼ������
		PZ.lc = RXDSml;
		xlpoint_pz(array,tarray,&PZ);
		RXDSpt[0] = PZ.x;
		RXDSpt[1] = PZ.y;

		PZ.lc = RXDEml;
		xlpoint_pz(array,tarray,&PZ);
		RXDEpt[0] = PZ.x;
		RXDEpt[1] = PZ.y;


		RXDName = GetRXDXLNameByRXDIndex(iRXD);
		for (int iSection=0; iSection < iSecCount; iSection++)
		{
			//////////////////////////////////////////////////////////////////////////
			//��ȡ�������жη�Χ�ڵķֶ���Ϣ
			SectionSpt[0] = BasicSectionInfor[iSection].BpointToN;
			SectionSpt[1] = BasicSectionInfor[iSection].BpointToE;
			SectionEpt[0] = BasicSectionInfor[iSection].EpointToN;
			SectionEpt[1] = BasicSectionInfor[iSection].EpointToE;
			SectionSml = PROJ_ML(array,tarray,SectionSpt[0],SectionSpt[1],SectionSpt[0],SectionSpt[1]);
			SectionEml = PROJ_ML(array,tarray,SectionEpt[0],SectionEpt[1],SectionEpt[0],SectionEpt[1]);

			FDSml = max(RXDSml,SectionSml);
			FDEml = min(RXDEml,SectionEml);


			if(FDEml - FDSml > 10.0)//iSection�ڸ����жη�Χ��
			{
				if (BasicSectionInfor[iSection].GJDType == 0 || BasicSectionInfor[iSection].GJDType == -1)//���л�����
				{
					PZ.lc = FDSml;
					xlpoint_pz(array,tarray,&PZ);
					FDSpt[0] = PZ.x;
					FDSpt[1] = PZ.y;
					PZ.lc = FDEml;
					xlpoint_pz(array,tarray,&PZ);
					FDEPt[0] = PZ.x;
					FDEPt[1] = PZ.y;

					int SDL,EDL;
					double cml,dml;
					ACHAR GH[20];
					GetiRXDDLinfo(iRXD,SDL,EDL);

					int InsertDLSum = 0;
					DLBZ InsertDLBArray[MAXDLNUM];
					EXDLB TempEXDLB;
					CArray<EXDLB,EXDLB> EXDLBArray;
					EXDLBArray.RemoveAll();

					if (_tcscmp(BasicSectionInfor[iSection].BXLName,L"���л���")==0)
					{
						JiXianpGtzx->GetJYDLArray(FDSml,FDEml,InsertDLSum,InsertDLBArray);
						if (InsertDLSum > 0)
						{
							for (int iInsertDL=0; iInsertDL<InsertDLSum; iInsertDL++)
							{
								TempEXDLB.BGH = InsertDLBArray[iInsertDL].BGH;
								TempEXDLB.EGH = InsertDLBArray[iInsertDL].EGH;
								TempEXDLB.BLC = InsertDLBArray[iInsertDL].BLC;
								TempEXDLB.ELC = InsertDLBArray[iInsertDL].BLC + InsertDLBArray[iInsertDL].DL;
								TempEXDLB.no = iRXD;
								TempEXDLB.RXDName = RXDName;
								TempEXDLB.TYLC = PROJ_ML(array,tarray,InsertDLBArray[iInsertDL].N,InsertDLBArray[iInsertDL].E,InsertDLBArray[iInsertDL].N,InsertDLBArray[iInsertDL].E);
								EXDLBArray.Add(TempEXDLB);
							}
						}
						//////////////////////////////////////////////////////////////////////////
						//Ϊ���иĽ������ö�������Ӷ���
						if ((iSection > 0 && BasicSectionInfor[iSection - 1].GJDType == 2) || iSection == 0 || fabs(FDSml - RXDSml) < 0.001)
						{
							//��ǰ����ǹ�II�ϵ����(ǰ�������иĽ���)
							dml = XCLC(FDSml,GH,DLB2x,NDL);
							TempEXDLB.BLC = dml;
							TempEXDLB.BGH = GH;


							//�Ϻ�����Ǽ��л����ϵ����
							cml = PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,FDSpt[0],FDSpt[1],FDSpt[0],FDSpt[1]);
							cml = JiXianpGtzx->CalnewcmlTocml(cml);
							dml = JiXianpGtzx->XLC1(cml,GH,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
							TempEXDLB.EGH = GH;
							TempEXDLB.ELC = dml;
							TempEXDLB.TYLC = FDSml;
							TempEXDLB.no = iRXD;
							TempEXDLB.RXDName = RXDName;

							/*��������������ж��в���һ����
							|--------------------------       ���ж�
							|---------------------       ���иĽ�(�����ö�)*/
							if (FDSml - RXDSml > 0.1)
							{
								EXDLBArray.InsertAt(0,TempEXDLB);
							}
							/*���������������
							(1)      |--------------------            ���ж�
							|--------------------            ���иĽ�(�����ö�)

							(2)      |--------------------            ���ж�
							|----------------------            ���иĽ�(�����ö�)
							��Ҫ�����ж����ĶϺ���̸�Ϊ�������ϵ����,��ǰ�������ݹ������*/
							else
							{
								IsModifySDL = true;
								RXSDL = DLB2x[SDL];
								RXSDL.EGH = TempEXDLB.EGH;
								RXSDL.ELC = TempEXDLB.ELC;
							}
						}

						if ((iSection < BasicSectionInfor.GetSize() - 1 && BasicSectionInfor[iSection + 1].GJDType == 2) || iSection == BasicSectionInfor.GetSize() - 1 || fabs(FDEml-RXDEml)<0.001)
						{
							//�Ϻ�����ǹ�II�ϵ����(���������иĽ���)
							dml = XCLC(FDEml,GH,DLB2x,NDL);
							TempEXDLB.ELC = dml;
							TempEXDLB.EGH = GH;


							//��ǰ����Ǽ��л����ϵ����
							cml = PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,FDEPt[0],FDEPt[1],FDEPt[0],FDEPt[1]);
							cml = JiXianpGtzx->CalnewcmlTocml(cml);
							dml = JiXianpGtzx->XLC1(cml,GH,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
							TempEXDLB.BGH = GH;
							TempEXDLB.BLC = dml;
							TempEXDLB.TYLC = FDEml;
							TempEXDLB.no = iRXD;
							TempEXDLB.RXDName = RXDName;


							//Zhujiang--��II���ж��ϵ����иĽ���㣬��ʼ״̬ʱ��ǰ�Ϻ���̸���һ��
							TempEXDLB.ELC = TempEXDLB.BLC;

							/*��������������ж��в���һ����
							--------------------------|       ���ж�
							------------------|               ���иĽ�(�����ö�)*/
							if (RXDEml - FDEml > 0.1)
							{
								EXDLBArray.Add(TempEXDLB);
							}
							/*���������������
							(1)   --------------------|            ���ж�
							---------------------------|     ���иĽ�(�����ö�)

							(2)   --------------------|            ���ж�
							--------------------|            ���иĽ�(�����ö�)
							��Ҫ�����ж��յ�Ķ�ǰ��̸�Ϊ�������ϵ����,�Ϻ��������ݹ������*/
							else
							{
								IsModifyEDL = true;
								RXEDL = DLB2x[EDL];
								RXEDL.BGH = TempEXDLB.BGH;
								RXEDL.BLC = TempEXDLB.BLC;
							}
						}

						//////////////////////////////////////////////////////////////////////////
						//�������ɵĶ�������뵽��ͨ�������ж���
						AllInsertEXDLBArray.Append(EXDLBArray);
					}
					else if (_tcscmp(BasicSectionInfor[iSection].BXLName,L"��������")==0)
					{
						cml = PROJ_ML(ZuoXianpGtzx->array,ZuoXianpGtzx->tarray,RXDEpt[0],RXDEpt[1],RXDEpt[0],RXDEpt[1]);
						cml = ZuoXianpGtzx->CalnewcmlTocml(cml);
						dml = ZuoXianpGtzx->XCLC(cml,GH,ZuoXianpGtzx->DLB2x,ZuoXianpGtzx->NDL);
					}
					else
					{
						cml = PROJ_ML(YouXianpGtzx->array,YouXianpGtzx->tarray,RXDEpt[0],RXDEpt[1],RXDEpt[0],RXDEpt[1]);
						cml = YouXianpGtzx->CalnewcmlTocml(cml);
						dml = YouXianpGtzx->XCLC(cml,GH,YouXianpGtzx->DLB2x,YouXianpGtzx->NDL);
					}
				}
			}
		}
		//�޸����ж����յ����
		if (IsModifySDL)
			DLB2x[SDL] = RXSDL;
		if (IsModifyEDL)
			DLB2x[EDL] = RXEDL;
	}


	////////////////////////////////////////////////////////////////////////////
	////����Ҫ����Ķ���������߶�����
	for (int iDL=0;iDL<AllInsertEXDLBArray.GetSize(); iDL++)
	{
		for (int iEXDL=0; iEXDL<NDL-1; iEXDL++)
		{
			if (AllInsertEXDLBArray[iDL].TYLC > DLB2x[iEXDL].TYLC+10.0 && AllInsertEXDLBArray[iDL].TYLC < DLB2x[iEXDL+1].TYLC-10.0)//�����������
			{
				for (int idl=NDL-1; idl>iEXDL; idl--)
				{
					DLB2x[idl+1] = DLB2x[idl];
				}
				DLB2x[iEXDL+1] = AllInsertEXDLBArray[iDL];
				NDL++;
				break;
			}
		}
	}


}


void GTZX2_JYX_ROAD::ModifyiRXDDLB2xByGJDArray(int iRXD)
{

	CArray<EXDLB,EXDLB> InsertDLBArray;
	double RXDSml,RXDEml;
	double GJDSml,GJDEml;
	CString RXDName;
	double SectionSpt[2],SectionEpt[2];
	double RXDSpt[2],RXDEpt[2];
	double FDSpt[2],FDEPt[2];
	double SectionSml,SectionEml;
	double FDSml,FDEml;
	xlpoint PZ;
	bool ModifySDL,ModifyEDL;//�Ƿ������������Ϻ������յ��ǰ
	EXDLB RXSDL,RXEDL;
	ModifySDL = ModifyEDL = false;
	CArray<EXDLB,EXDLB> AllInsertEXDLBArray;//������Ҫ�����ͨ�������жεĶ�������
	AllInsertEXDLBArray.RemoveAll();

	//////////////////////////////////////////////////////////////////////////
	//�����ж��ڳ����յ���Ķ��߶���ȫ��ɾ��
	int SDL,EDL,RemoveDLSum;
	GetiRXDDLinfo(iRXD,SDL,EDL);
	RemoveDLSum = EDL - SDL - 1;
	if (RemoveDLSum>0)
	{
		for (int iEXDL=SDL+1; iEXDL+RemoveDLSum<NDL; iEXDL++)
		{
			DLB2x[iEXDL] = DLB2x[iEXDL+RemoveDLSum];
		}
		NDL -= RemoveDLSum;
	}

	GetiRxdStartEndMlByNum(iRXD,RXDSml,RXDEml);
	PZ.lc = RXDSml;
	xlpoint_pz(array,tarray,&PZ);
	RXDSpt[0] = PZ.x;
	RXDSpt[1] = PZ.y;
	PZ.lc = RXDEml;
	xlpoint_pz(array,tarray,&PZ);
	RXDEpt[0] = PZ.x;
	RXDEpt[1] = PZ.y;
	RXDName = GetRXDXLNameByRXDIndex(iRXD);
	for (int iSection=0; iSection<BasicSectionInfor.GetSize(); iSection++)
	{
		//////////////////////////////////////////////////////////////////////////
		//��ȡ�������жη�Χ�ڵķֶ���Ϣ
		SectionSpt[0] = BasicSectionInfor[iSection].BpointToN;
		SectionSpt[1] = BasicSectionInfor[iSection].BpointToE;
		SectionEpt[0] = BasicSectionInfor[iSection].EpointToN;
		SectionEpt[1] = BasicSectionInfor[iSection].EpointToE;
		SectionSml = PROJ_ML(array,tarray,SectionSpt[0],SectionSpt[1],SectionSpt[0],SectionSpt[1]);
		SectionEml = PROJ_ML(array,tarray,SectionEpt[0],SectionEpt[1],SectionEpt[0],SectionEpt[1]);
		FDSml = max(RXDSml,SectionSml);
		FDEml = min(RXDEml,SectionEml);

		if (FDEml-FDSml>10.0)//iSection�ڸ����жη�Χ��
		{
			if (BasicSectionInfor[iSection].GJDType==0 || BasicSectionInfor[iSection].GJDType==-1)//���л�����
			{
				PZ.lc = FDSml;
				xlpoint_pz(array,tarray,&PZ);
				FDSpt[0] = PZ.x;
				FDSpt[1] = PZ.y;
				PZ.lc = FDEml;
				xlpoint_pz(array,tarray,&PZ);
				FDEPt[0] = PZ.x;
				FDEPt[1] = PZ.y;
				int SDL,EDL;
				double cml,dml;
				ACHAR GH[20];
				GetiRXDDLinfo(iRXD,SDL,EDL);

				int InsertDLSum = 0;
				DLBZ InsertDLBArray[MAXDLNUM];
				EXDLB TempEXDLB;
				CArray<EXDLB,EXDLB> EXDLBArray;
				EXDLBArray.RemoveAll();

				if (_tcscmp(BasicSectionInfor[iSection].BXLName,L"���л���")==0)
				{
					JiXianpGtzx->GetJYDLArray(FDSml,FDEml,InsertDLSum,InsertDLBArray);
					if (InsertDLSum>0)
					{
						for (int iInsertDL=0; iInsertDL<InsertDLSum; iInsertDL++)
						{
							TempEXDLB.BGH = InsertDLBArray[iInsertDL].BGH;
							TempEXDLB.EGH = InsertDLBArray[iInsertDL].EGH;
							TempEXDLB.BLC = InsertDLBArray[iInsertDL].BLC;
							TempEXDLB.ELC = InsertDLBArray[iInsertDL].BLC + InsertDLBArray[iInsertDL].DL;
							TempEXDLB.no = iRXD;
							TempEXDLB.RXDName = RXDName;
							TempEXDLB.TYLC = PROJ_ML(array,tarray,InsertDLBArray[iInsertDL].N,InsertDLBArray[iInsertDL].E,InsertDLBArray[iInsertDL].N,InsertDLBArray[iInsertDL].E);
							EXDLBArray.Add(TempEXDLB);
						}
					}
					//////////////////////////////////////////////////////////////////////////
					//Ϊ���иĽ������ö����յ����Ӷ���
					if ((iSection>0 && BasicSectionInfor[iSection-1].GJDType==2) || iSection==0 || fabs(FDSml-RXDSml)<0.001)
					{
						dml = XCLC(FDSml,GH,DLB2x,NDL);
						TempEXDLB.BLC = dml;
						TempEXDLB.BGH = GH;
						cml = PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,FDSpt[0],FDSpt[1],FDSpt[0],FDSpt[1]);
						cml = JiXianpGtzx->CalnewcmlTocml(cml);
						dml = JiXianpGtzx->XLC1(cml,GH,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
						TempEXDLB.EGH = GH;
						TempEXDLB.ELC = dml;
						TempEXDLB.TYLC = FDSml;
						TempEXDLB.no = iRXD;
						TempEXDLB.RXDName = RXDName;
						if (TempEXDLB.TYLC-RXDSml>0.1)//�������ж������
						{
							EXDLBArray.InsertAt(0,TempEXDLB);
						}
						else//�����ж�����ϣ��������ж��������Ϻ�
						{
							ModifySDL = true;
							RXSDL = DLB2x[SDL];
							RXSDL.EGH = TempEXDLB.EGH;
							RXEDL.ELC = TempEXDLB.ELC;
							//DLB2x[SDL].EGH = TempEXDLB.EGH;
							//DLB2x[SDL].ELC = TempEXDLB.ELC;
						}
					}


					if ((iSection<BasicSectionInfor.GetSize()-1 && BasicSectionInfor[iSection+1].GJDType==2) || iSection==BasicSectionInfor.GetSize()-1 || fabs(FDEml-RXDEml)<0.001)
					{
						dml = XCLC(FDEml,GH,DLB2x,NDL);
						TempEXDLB.ELC = dml;
						TempEXDLB.EGH = GH;
						cml = PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,FDEPt[0],FDEPt[1],FDEPt[0],FDEPt[1]);
						cml = JiXianpGtzx->CalnewcmlTocml(cml);
						dml = JiXianpGtzx->XLC1(cml,GH,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
						TempEXDLB.BGH = GH;
						TempEXDLB.BLC = dml;
						TempEXDLB.TYLC = FDEml;
						TempEXDLB.no = iRXD;
						TempEXDLB.RXDName = RXDName;
						if (RXDEml-TempEXDLB.TYLC>0.1)//�������ж��յ���
						{
							EXDLBArray.Add(TempEXDLB);
						}
						else//�����ж��յ��ϣ��������ж��յ������ǰ
						{
							RXEDL = DLB2x[EDL];
							RXEDL.BGH = TempEXDLB.BGH;
							RXEDL.BLC = TempEXDLB.BLC;
							//DLB2x[EDL].BGH = TempEXDLB.BGH;
							//DLB2x[EDL].BLC = TempEXDLB.BLC;
						}
					}

					//////////////////////////////////////////////////////////////////////////
					//�������ɵĶ�������뵽��ͨ�������ж���
					AllInsertEXDLBArray.Append(EXDLBArray);
				}
				else if (_tcscmp(BasicSectionInfor[iSection].BXLName,L"��������")==0)
				{
					cml = PROJ_ML(ZuoXianpGtzx->array,ZuoXianpGtzx->tarray,RXDEpt[0],RXDEpt[1],RXDEpt[0],RXDEpt[1]);
					cml = ZuoXianpGtzx->CalnewcmlTocml(cml);
					dml = ZuoXianpGtzx->XCLC(cml,GH,ZuoXianpGtzx->DLB2x,ZuoXianpGtzx->NDL);
				}
				else
				{
					cml = PROJ_ML(YouXianpGtzx->array,YouXianpGtzx->tarray,RXDEpt[0],RXDEpt[1],RXDEpt[0],RXDEpt[1]);
					cml = YouXianpGtzx->CalnewcmlTocml(cml);
					dml = YouXianpGtzx->XCLC(cml,GH,YouXianpGtzx->DLB2x,YouXianpGtzx->NDL);
				}
			}
		}
	}

	//�������ж����յ����
	if (ModifySDL)
		DLB2x[SDL] = RXSDL;
	if (ModifyEDL)
		DLB2x[EDL] = RXEDL;
	////////////////////////////////////////////////////////////////////////////
	////����Ҫ����Ķ���������߶�����
	for (int iDL=0;iDL<AllInsertEXDLBArray.GetSize(); iDL++)
	{
		//		ads_printf(L"��������ж�%s,%d,BGH=%s,BLC=%lf,EGH=%s,ELC=%lf,TYLC=%lf\n",AllInsertEXDLBArray[iDL].RXDName,InsertDLBArray[iDL].no,InsertDLBArray[iDL].BGH,InsertDLBArray[iDL].BLC,InsertDLBArray[iDL].EGH,InsertDLBArray[iDL].ELC,InsertDLBArray[iDL].TYLC);
		for (int iEXDL=0; iEXDL<NDL-1; iEXDL++)
		{
			if (AllInsertEXDLBArray[iDL].TYLC > DLB2x[iEXDL].TYLC+10.0 && AllInsertEXDLBArray[iDL].TYLC < DLB2x[iEXDL+1].TYLC-10.0)//�����������
			{
				for (int idl=NDL-1; idl>iEXDL; idl--)
				{
					DLB2x[idl+1] = DLB2x[idl];
				}
				DLB2x[iEXDL+1] = AllInsertEXDLBArray[iDL];
				NDL++;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//�����ͨ���߶�������
void GTZX2_JYX_ROAD::CalGT2xDLValue()
{
	xlpoint PZ;
	ACHAR GH[20],Ckml1[80],Ckml2[80];
	for (int iDL=1; iDL<NDL-1; iDL++)
	{
		if (DLB2x[iDL].no == DLB2x[iDL-1].no && DLB2x[iDL].no == DLB2x[iDL+1].no)
		{
			_tcscpy(GH,DLB2x[iDL].BGH);
			_tcscpy(Ckml1,LCchr(GH,DLB2x[iDL].BLC,3,1));
			_tcscpy(GH,DLB2x[iDL].EGH);
			_tcscpy(Ckml2,LCchr(GH,DLB2x[iDL].ELC,3,1));
			if (GetPZFromXCLC2X(Ckml1,PZ))
			{
				DLB2x[iDL].TYLC = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
			}
			else
			{
				GetPZFromXCLC2X(Ckml2,PZ);
				DLB2x[iDL].TYLC = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
			}
		}
	}
}


void GTZX2_JYX_ROAD::GetGT2XDLArray(double Sml,double Eml,int& DLSum, EXDLB DLBArray[MAXDLNUM])//��ȡͶӰ�����Sml-Eml��Χ�ڵĶ���,������Sml��Eml
{
	DLSum = 0;
	int iDL=0;
	double DLProjMl;//������ͶӰ���
	for(iDL=0;iDL<NDL;iDL++)
	{
		DLProjMl = DLB2x[iDL].TYLC;
		if(DLProjMl>Sml+0.1 && DLProjMl<Eml-0.1)
		{
			DLBArray[DLSum] = DLB2x[iDL];
			DLSum++;
		}
	}
}

int GTZX2_JYX_ROAD::JudgePtIsOnDL(double Cml)//���뼸��ͳһ��̣��жϸĵ��Ƿ��ڶ�����
{
	int iDL = -1;
	double DLProjMl;//������ͶӰ���

	for(iDL=0;iDL<NDL;iDL++)
	{
		DLProjMl = DLB2x[iDL].TYLC;
		if (DLProjMl-Cml>1.0)
			return -1;
		if(fabs(DLProjMl-Cml)<0.001)
		{
			return iDL;
		}
	}

	return -1;
}

int GTZX2_JYX_ROAD::JudgePtIsOnRXD(double N,double E)
{
	double cml = PROJ_ML(array,tarray,N,E,N,E);
	int RXDSum = GetRXDNum();
	if (RXDSum==0)
	{
		return -1;
	}

	int SDL,EDL;
	for (int iRXD=0; iRXD<RXDSum; iRXD++)
	{
		GetiRXDDLinfo(iRXD,SDL,EDL);
		if (cml<DLB2x[SDL].TYLC-1.0)
		{
			break;
		}
		if (cml>DLB2x[SDL].TYLC-0.001 && cml<DLB2x[EDL].TYLC+0.001)
		{
			return iRXD;
		}
	}
	return -1;
}


void GTZX2_JYX_ROAD::RefreshExFDArray(double RefreshSml,double RefreshEml,int iRXD)
{
	//�õ���ǰ�ļ���
	CString FileName = mdbname;

	//��ͨ���2�� ָ��
	GTZX2_JYX_ROAD *pOriGTSJEX = PmPointer::CreateGTSJEXPointer(FileName);
	xlpoint PZ;

	//����ԭ�й�ͨ���ߵĶ�����͸Ľ�����Ϣ
	CArray<GJDInf,GJDInf> OriGJDInfArray;
	EXDLB Ori2xDLB[MAXDLNUM2x];
	int OriNDL2x = 0;
	GJDInf TempGJD;
	if (RefreshEml-RefreshSml>100.0)
	{
		OriGJDInfArray.SetSize(pOriGTSJEX->GJDInforArray.GetSize());
		for (int iGJD=0; iGJD<pOriGTSJEX->GJDInforArray.GetSize(); iGJD++)
		{
			_tcscpy(OriGJDInfArray[iGJD].BXLName , pOriGTSJEX->GJDInforArray[iGJD].BXLName);
			_tcscpy(OriGJDInfArray[iGJD].EXLName , pOriGTSJEX->GJDInforArray[iGJD].EXLName);
			_tcscpy(OriGJDInfArray[iGJD].GJDBLC , pOriGTSJEX->GJDInforArray[iGJD].GJDBLC);
			_tcscpy(OriGJDInfArray[iGJD].GJDELC , pOriGTSJEX->GJDInforArray[iGJD].GJDELC);
			_tcscpy(OriGJDInfArray[iGJD].gjdname , pOriGTSJEX->GJDInforArray[iGJD].gjdname);
			OriGJDInfArray[iGJD].GJDType = pOriGTSJEX->GJDInforArray[iGJD].GJDType;
			OriGJDInfArray[iGJD].spt[0] = pOriGTSJEX->GJDInforArray[iGJD].spt[0];
			OriGJDInfArray[iGJD].spt[1] = pOriGTSJEX->GJDInforArray[iGJD].spt[1];
			OriGJDInfArray[iGJD].ept[0] = pOriGTSJEX->GJDInforArray[iGJD].ept[0];
			OriGJDInfArray[iGJD].ept[1] = pOriGTSJEX->GJDInforArray[iGJD].ept[1];
			TempGJD = OriGJDInfArray[iGJD];
		}

		//////////////////////////////////////////////////////////////////////////
		//��ԭ����ͨ���ߵ�ͳһ���ͶӰ���µĹ�ͨ������
		OriNDL2x = pOriGTSJEX->NDL;
		for (int iDL=0; iDL<OriNDL2x; iDL++)
		{
			Ori2xDLB[iDL] = pOriGTSJEX->DLB2x[iDL];
			PZ.lc = pOriGTSJEX->DLB2x[iDL].TYLC;
			xlpoint_pz(pOriGTSJEX->array,pOriGTSJEX->tarray,&PZ);
			Ori2xDLB[iDL].TYLC =  PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
		}
	}

	//��ͨ���2��(���߸�ʽ)
	Create2XZJXArray();//������ͨ��ƶ��߼�ֱ������


	double spt[2],ept[2];
	double RXDSpt[2],RXDEpt[2];
	double XLSpt[2],XLEpt[2];


	if (JiXianpGtzx==NULL)
		JiXianpGtzx = PmPointer::CreateJYJX(mdbname);
	if (ZuoXianpGtzx==NULL)
		ZuoXianpGtzx = PmPointer::CreateJYEX(mdbname,-1);
	if (YouXianpGtzx==NULL)
	{
		YouXianpGtzx = PmPointer::CreateJYEX(mdbname,1);
	}


	CArray<GJDInf,GJDInf> JYZXLYDArray,JYYXLYDArray,JYJXLYDArray,AllLYDArray,AllBXGJDArray,AllRXGJDArray,AllGJDArray;
	//3.   �� 2��ָ��(���߸�ʽ)���롡����3ָ��Ƚ�,
	//     �õ����ö�����
	//������ƻ�����������߱Ƚ�

	//�õ�(��������/����/����)���ö�
	GJDInfTab.RemoveAll();

	if(ZuoXianpGtzx!=NULL)
	{
		ZuoXianpGtzx->Create2XZJXArray();
		spt[0] = ZuoXianpGtzx->JDarray[0].N;
		spt[1] = ZuoXianpGtzx->JDarray[0].E;
		ept[0] = ZuoXianpGtzx->JDarray[ZuoXianpGtzx->NJD-1].N;
		ept[1] = ZuoXianpGtzx->JDarray[ZuoXianpGtzx->NJD-1].E;
		CreateLYDArray_OnJYEX(ZuoXianpGtzx,spt,ept,JYZXLYDArray);
		AllLYDArray.Append(JYZXLYDArray);
	}

	if(JiXianpGtzx!=NULL)
	{
		JiXianpGtzx->CreatJZXArray();
		spt[0] = JiXianpGtzx->JDarray[0].N;
		spt[1] = JiXianpGtzx->JDarray[0].E;
		ept[0] = JiXianpGtzx->JDarray[JiXianpGtzx->NJD-1].N;
		ept[1] = JiXianpGtzx->JDarray[JiXianpGtzx->NJD-1].E;
		CreateLYDArray_OnJYJX(JiXianpGtzx,spt,ept,JYJXLYDArray);
		AllLYDArray.Append(JYJXLYDArray);
	}

	if(YouXianpGtzx!=NULL)
	{
		YouXianpGtzx->Create2XZJXArray();
		spt[0] = YouXianpGtzx->JDarray[0].N;
		spt[1] = YouXianpGtzx->JDarray[0].E;
		ept[0] = YouXianpGtzx->JDarray[YouXianpGtzx->NJD-1].N;
		ept[1] = YouXianpGtzx->JDarray[YouXianpGtzx->NJD-1].E;
		CreateLYDArray_OnJYEX(YouXianpGtzx,spt,ept,JYYXLYDArray);
		AllLYDArray.Append(JYYXLYDArray);
	}

	XLSpt[0] = JDarray[0].N;
	XLSpt[1] = JDarray[0].E;
	XLEpt[0] = JDarray[NJD-1].N;
	XLEpt[1] = JDarray[NJD-1].E;

	int RXDSum = GetRXDNum();
	int SDLIndex,EDLIndex;
	double cml;
	CArray<GJDInf,GJDInf> TmpGJDArray;
	//////////////////////////////////////////////////////////////////////////
	//��·��㵽��һ�����ж����
	if (RXDSum>0)
	{
		for (int iRXD=0; iRXD<RXDSum; iRXD++)
		{
			//////////////////////////////////////////////////////////////////////////
			//���ж�ǰ�Ĳ��ж�
			GetiRXDDLinfo(iRXD,SDLIndex,EDLIndex);
			PZ.lc = DLB2x[SDLIndex].TYLC;
			xlpoint_pz(array,tarray,&PZ);
			RXDSpt[0] = PZ.x;
			RXDSpt[1] = PZ.y;

			if (iRXD==0)
			{
				CreateGJDInGT2XBXD(XLSpt,RXDSpt,AllLYDArray,TmpGJDArray);
				AllBXGJDArray.Append(TmpGJDArray);
				TmpGJDArray.RemoveAll();
			}
			else
			{
				CreateGJDInGT2XBXD(RXDEpt,RXDSpt,AllLYDArray,TmpGJDArray);
				AllBXGJDArray.Append(TmpGJDArray);
				TmpGJDArray.RemoveAll();
			}

			//���ж�
			PZ.lc = DLB2x[EDLIndex].TYLC;
			xlpoint_pz(array,tarray,&PZ);
			RXDEpt[0] = PZ.x;
			RXDEpt[1] = PZ.y;

			CreateGJDInGT2XRXD(RXDSpt,RXDEpt,AllLYDArray,TmpGJDArray);
			AllRXGJDArray.Append(TmpGJDArray);
			TmpGJDArray.RemoveAll();
		}
		//////////////////////////////////////////////////////////////////////////
		//���һ�����жε��յ�
		CreateGJDInGT2XBXD(RXDEpt,XLEpt,AllLYDArray,TmpGJDArray);
	}

	else//ȫ�߲���
		CreateGJDInGT2XBXD(XLSpt,XLEpt,AllLYDArray,TmpGJDArray);

	AllBXGJDArray.Append(TmpGJDArray);
	TmpGJDArray.RemoveAll();
	AllGJDArray.Append(AllRXGJDArray);
	AllGJDArray.Append(AllBXGJDArray);
	if (RefreshEml-RefreshSml>100.0)
	{
		TmpGJDArray.Append(AllGJDArray);
		RefreshGTZX2GJDArray(RefreshSml,RefreshEml,OriGJDInfArray,TmpGJDArray,AllGJDArray);
	}
	SortGTZX2GJDArray(AllGJDArray);
	MergeGTZX2GJDArray(AllGJDArray);

	CString kml;
	//��ָ�븳ֵ
	GTZX_JYX_ROAD::GJDInforStruct GJDInforTemp;
	GJDInforArray.RemoveAll();
	for(int k=0;k<AllGJDArray.GetSize();k++)
	{

		GJDInforTemp.spt[0] = AllGJDArray[k].spt[0] ;
		GJDInforTemp.spt[1] = AllGJDArray[k].spt[1] ;
		GJDInforTemp.ept[0] = AllGJDArray[k].ept[0] ;
		GJDInforTemp.ept[1] = AllGJDArray[k].ept[1] ;

		_tcscpy( GJDInforTemp.BXLName , AllGJDArray[k].BXLName );
		_tcscpy( GJDInforTemp.EXLName , AllGJDArray[k].EXLName );

		GJDInforTemp.GJDType = AllGJDArray[k].GJDType ;

		_tcscpy(GJDInforTemp.gjdname,AllGJDArray[k].gjdname);

		GJDInforArray.Add(GJDInforTemp);
	}

	for(int i=0;i<GJDInforArray.GetSize();i++)
	{
		CalGJDStartEndXCLC(GJDInforArray[i]);
	}

	FormSectionInfor();

	ModifyiRXDDLB2xByGJDArray(iRXD);

	//if (RefreshEml-RefreshSml>100.0)
	//{
	//	int RefreshNDLB2x=NDL;
	//	EXDLB RefreshDLB2x[MAXDLNUM2x];
	//	for (int iDL=0; iDL<NDL; iDL++)
	//	{
	//		RefreshDLB2x[iDL] = DLB2x[iDL];
	//		ACHAR BGH[20],EGH[20];
	//		_tcscpy(BGH,DLB2x[iDL].BGH);
	//		_tcscpy(EGH,DLB2x[iDL].EGH);
	//		ads_printf(L"####%d,%s%lf,%s%lf,%d\n",iDL,BGH,DLB2x[iDL].BLC,EGH,DLB2x[iDL].ELC,DLB2x[iDL].no);
	//	}
	//	RefreshGTZX2DLArray(RefreshSml,RefreshEml,OriNDL2x,Ori2xDLB,NDL,DLB2x,RefreshNDLB2x,RefreshDLB2x);
	//	NDL = RefreshNDLB2x;
	//	for (int iDL=0; iDL<RefreshNDLB2x; iDL++)
	//	{
	//		DLB2x[iDL] = RefreshDLB2x[iDL];
	//		ACHAR BGH[20],EGH[20];
	//		_tcscpy(BGH,DLB2x[iDL].BGH);
	//		_tcscpy(EGH,DLB2x[iDL].EGH);
	//		ads_printf(L"$$$$$%d,%s%lf,%s%lf,%d\n",iDL,BGH,DLB2x[iDL].BLC,EGH,DLB2x[iDL].ELC,DLB2x[iDL].no);
	//	}
	//}
	//////////////////////////////////////////////////////////////////////////

	return;
}

void GTZX2_JYX_ROAD::CalGJDStartEndXCLC(GTZX_JYX_ROAD::GJDInforStruct& GJD)
{
	double cml,dml;
	ACHAR GH[20];

	if (_tcscmp(GJD.BXLName,L"���л���")==0)
	{
		cml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,GJD.spt[0],GJD.spt[1],GJD.spt[0],GJD.spt[1]);
		cml = JiXianpGtzx->CalnewcmlTocml(cml);
		dml = JiXianpGtzx->XLC1(cml,GH,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
		_tcscpy(GJD.GJDBLC ,LCchr(GH,dml,3,1));
	}
	else if (_tcscmp(GJD.BXLName,L"��������")==0)
	{
		cml = ZuoXianpGtzx->PROJ_ML(ZuoXianpGtzx->array,ZuoXianpGtzx->tarray,GJD.spt[0],GJD.spt[1],GJD.spt[0],GJD.spt[1]);
		dml = ZuoXianpGtzx->XCLC(cml,GH,ZuoXianpGtzx->DLB2x,ZuoXianpGtzx->NDL);
		_tcscpy(GJD.GJDBLC ,LCchr(GH,dml,3,1));

	}
	else
	{
		cml = YouXianpGtzx->PROJ_ML(YouXianpGtzx->array,YouXianpGtzx->tarray,GJD.spt[0],GJD.spt[1],GJD.spt[0],GJD.spt[1]);
		dml = YouXianpGtzx->XCLC(cml,GH,YouXianpGtzx->DLB2x,YouXianpGtzx->NDL);
		_tcscpy(GJD.GJDBLC ,LCchr(GH,dml,3,1));
	}


	if (_tcscmp(GJD.EXLName,L"���л���")==0)
	{
		cml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,GJD.ept[0],GJD.ept[1],GJD.ept[0],GJD.ept[1]);
		cml = JiXianpGtzx->CalnewcmlTocml(cml);
		dml = JiXianpGtzx->XLC1(cml,GH,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
		_tcscpy(GJD.GJDELC ,LCchr(GH,dml,3,1));
	}
	else if (_tcscmp(GJD.EXLName,L"��������")==0)
	{
		cml = ZuoXianpGtzx->PROJ_ML(ZuoXianpGtzx->array,ZuoXianpGtzx->tarray,GJD.ept[0],GJD.ept[1],GJD.ept[0],GJD.ept[1]);
		dml = ZuoXianpGtzx->XCLC(cml,GH,ZuoXianpGtzx->DLB2x,ZuoXianpGtzx->NDL);
		_tcscpy(GJD.GJDELC ,LCchr(GH,dml,3,1));
	}
	else
	{
		cml = YouXianpGtzx->PROJ_ML(YouXianpGtzx->array,YouXianpGtzx->tarray,GJD.ept[0],GJD.ept[1],GJD.ept[0],GJD.ept[1]);
		dml = YouXianpGtzx->XCLC(cml,GH,YouXianpGtzx->DLB2x,YouXianpGtzx->NDL);
		_tcscpy(GJD.GJDELC ,LCchr(GH,dml,3,1));
	}
}

//������ɫ���߿�
void GTZX2_JYX_ROAD::setColorLineW()
{
	if(!IsReSetColor)
	{

		CArray<SetLineAttriStruct,SetLineAttriStruct> SetLineAttrArrayTmp ;
		ReadColorFile( mdbname ,SetLineAttrArrayTmp);


		BOOL IsHasSet = FALSE ;

		for(int i=0;i<SetLineAttrArrayTmp.GetSize();i++)
		{
			for(int i=0;i<SetLineAttrArrayTmp.GetSize();i++)
			{
				if( _tcscmp(xlname,SetLineAttrArrayTmp[i].XLName)==0 ) 
				{
					AllColor = SetLineAttrArrayTmp[i].ColorIndex;
					LineWeightValue =  TranLineWeightFromIntToEnum(SetLineAttrArrayTmp[i].LineW);
					IsHasSet = TRUE ;
				}
			}
		}



		if(!IsHasSet)
		{
			if (_tcscmp(xlname, L"��������")==0)
				AllColor = 1;
			else if (_tcscmp(xlname, L"��������")==0)
				AllColor = 2;
			else if (_tcscmp(xlname, L"��ͨ��Ƣ���")==0)
				AllColor = 6;
			else
				AllColor = 0;

			CString strtmp;
			strtmp.Format(_T("%s"),xlname);
			if (strtmp.Find(L"��ƻ���") >= 0)
			{
				AllColor = 2;//��ƻ���Ϊ��ɫ

			}
			LineWeightValue =  TranLineWeightFromIntToEnum(0);
		}


	}

	GTZXJDToJDColor = AllColor;
	XWArcColor = AllColor;
	HuanHequxianColor = AllColor;
	JiaZhiXianColorZuo= AllColor;
	JiaZhiXianColorYou =AllColor;
	mdbnameBoxColor = AllColor;
	mdbnameTextColor =AllColor;
	QXYSColor = AllColor;

}


/*===================================================================

��������:�ҳ����ڸĽ�����Ϣ���޸�Ӱ�쵽�����жε����	
------------------------------------------------------------
����:CArray<GJDAllInforStruct,GJDAllInforStruct> &p_arrOriBasicSectionInfo 
�޸�֮ǰ�ĸĽ�����Ϣ����
------------------------------------------------------------
���:CArray<int,int> &p_arrInfluRXD 
Ӱ�쵽�����жα������
-----------------------------------------------------
�㷨����: 
-----------------------------------------------------
��ע:	                                           Zhujiang 2009:3:20
=====================================================================*/
void GTZX2_JYX_ROAD::InfluenceOfGJDModifyToRXD(CArray<GJDAllInforStruct,GJDAllInforStruct> &p_arrOriBasicSectionInfo,
											   CArray<int,int> &p_arrInfluRXD)
{
	double RXDSml,RXDEml;
	double FDSml,FDEml;
	double GJDSpt[2],GJDEpt[2],GJDSml,GJDEml;
	int iRXDSum = GetRXDNum();



	int iOriGJDAll = p_arrOriBasicSectionInfo.GetSize();
	int iNewGJDAll = BasicSectionInfor.GetSize();

	m_arrOldGJDArrToRXD.RemoveAll();
	m_arrNewGJDArrToRXD.RemoveAll();

	m_arrOldGJDArrToRXD.SetSize(iRXDSum);
	m_arrNewGJDArrToRXD.SetSize(iRXDSum);

	GJDArr_RXD gjdArr,oldgjdArr,newgjdArr;
	for (int iRXD = 0; iRXD < iRXDSum; iRXD++)
	{
		gjdArr.GJDArr.RemoveAll();

		GetiRxdStartEndMlByNum(iRXD,RXDSml,RXDEml);

		for (int iGJD = 0; iGJD < iOriGJDAll; iGJD ++)
		{
			GJDSpt[0] = p_arrOriBasicSectionInfo[iGJD].BpointToN;
			GJDSpt[1] = p_arrOriBasicSectionInfo[iGJD].BpointToE;
			GJDEpt[0] = p_arrOriBasicSectionInfo[iGJD].EpointToN;
			GJDEpt[1] = p_arrOriBasicSectionInfo[iGJD].EpointToE;
			GJDSml = PROJ_ML(array,tarray,GJDSpt[0],GJDSpt[1],GJDSpt[0],GJDSpt[1]);
			GJDEml = PROJ_ML(array,tarray,GJDEpt[0],GJDEpt[1],GJDEpt[0],GJDEpt[1]);

			FDSml = max(RXDSml,GJDSml);
			FDEml = min(RXDEml,GJDEml);


			if(FDEml - FDSml > 10.0)//iSection�ڸ����жη�Χ��
				//if ((GJDSml > RXDSml &&  GJDSml < RXDEml) || (GJDEml > RXDSml &&  GJDEml < RXDEml) 
					//	|| fabs(GJDSml - RXDSml) < 0.0001|| fabs(GJDSml - RXDEml) < 0.0001
						//	|| fabs(GJDEml - RXDSml) < 0.0001|| fabs(GJDEml - RXDEml) < 0.0001)
							gjdArr.GJDArr.Add(p_arrOriBasicSectionInfo[iGJD]);
		}
		m_arrOldGJDArrToRXD[iRXD] = gjdArr;




		gjdArr.GJDArr.RemoveAll();
		for (int iGJD = 0; iGJD < iNewGJDAll; iGJD ++)
		{
			GJDSpt[0] = BasicSectionInfor[iGJD].BpointToN;
			GJDSpt[1] = BasicSectionInfor[iGJD].BpointToE;
			GJDEpt[0] = BasicSectionInfor[iGJD].EpointToN;
			GJDEpt[1] = BasicSectionInfor[iGJD].EpointToE;
			GJDSml = PROJ_ML(array,tarray,GJDSpt[0],GJDSpt[1],GJDSpt[0],GJDSpt[1]);
			GJDEml = PROJ_ML(array,tarray,GJDEpt[0],GJDEpt[1],GJDEpt[0],GJDEpt[1]);

			FDSml = max(RXDSml,GJDSml);
			FDEml = min(RXDEml,GJDEml);


			if(FDEml - FDSml > 10.0)//iSection�ڸ����жη�Χ��
				//if ((GJDSml > RXDSml &&  GJDSml < RXDEml) || (GJDEml > RXDSml &&  GJDEml < RXDEml) 
					//	|| fabs(GJDSml - RXDSml) < 0.0001|| fabs(GJDSml - RXDEml) < 0.0001
						//	|| fabs(GJDEml - RXDSml) < 0.0001|| fabs(GJDEml - RXDEml) < 0.0001)
							gjdArr.GJDArr.Add(BasicSectionInfor[iGJD]);
		}

		m_arrNewGJDArrToRXD[iRXD] = gjdArr;


		oldgjdArr = m_arrOldGJDArrToRXD[iRXD];
		newgjdArr = m_arrNewGJDArrToRXD[iRXD];

		int iGJDOld = oldgjdArr.GJDArr.GetSize();
		int iGJDNew = newgjdArr.GJDArr.GetSize();

		GJDAllInforStruct gjdOld,gjdNew;
		bool bFlag = false;
		//��Ӧ�ĸĽ��θ��������˱仯
		if (iGJDOld != iGJDNew)
		{
			bFlag = true;
		}
		else//����һ����ѭ���Ա�
		{
			for (int i = 0; i < iGJDNew; i ++)
			{

				//////////////////////////////////////////////////////////////////////////
				bFlag = false;
				gjdNew = newgjdArr.GJDArr[i];
				for (int j = 0; j < iGJDOld; j ++)
				{
					gjdOld = oldgjdArr.GJDArr[j];
					if (gjdOld.GJDType == gjdNew.GJDType)
						if (fabs(gjdOld.BpointToN - gjdNew.BpointToN) < 0.0001)
							if (fabs(gjdOld.BpointToN - gjdNew.BpointToN) < 0.0001)
								if (fabs(gjdOld.BpointToN - gjdNew.BpointToN) < 0.0001)
									if (fabs(gjdOld.BpointToN - gjdNew.BpointToN) < 0.0001)
										bFlag = true;

				}
				//////////////////////////////////////////////////////////////////////////
				//���newgjdArr��ĳ��Ԫ����oldgjdArr�ж��Ҳ�����ȫһ����,�����жζ�Ӧ�ĸĽ���
				//��Ϣ�϶����ı���
				if (bFlag==false)
					p_arrInfluRXD.Add(iRXD);


			}

		}


	}





}


/*===================================================================

��������:ͨ��ͼ������������꣬�ҳ������Ľ����±�����ͽ�������	
------------------------------------------------------------
����:ads_point p_sptDwg     ͼ�����������
ads_point p_eptDwg     ͼ�����յ�����
------------------------------------------------------------
���:CArray<int,int>& p_arrJDIndex              �����±�����
CArray<DoubleLineData,DoubleLineData>& p_arrJD  ��������(II�߸�ʽ)
-----------------------------------------------------
�㷨����: 
-----------------------------------------------------
��ע:	                                           Zhujiang 2009:3:26
=====================================================================*/
void GTZX2_JYX_ROAD::GetJDIndexArrayByPts(ads_point p_sptDwg,ads_point p_eptDwg,
										  CArray<int,int>& p_arrJDIndex,CArray<DoubleLineData,DoubleLineData>& p_arrJD)
{
	p_arrJD.RemoveAll();

	int iStartJD,iEndJD;
	double dSml,dEml;


	//�������϶�������յ���̺ͽ������
	dSml = PROJ_ML(array,tarray,p_sptDwg[Y],p_sptDwg[X],p_sptDwg[Y],p_sptDwg[X]);
	dEml = PROJ_ML(array,tarray,p_eptDwg[Y],p_eptDwg[X],p_eptDwg[Y],p_eptDwg[X]);

	iStartJD = DistToNearJD(dSml,false);
	iEndJD = DistToNearJD(dEml,true);

	for (int i = 0;i < NJD;i++)
	{
		if (i >= iStartJD && i <= iEndJD)
		{
			p_arrJDIndex.Add(i);
			p_arrJD.Add(JD2[i]);
		}
	}
}

//��StepΪ����������ͶӰ���sml��eml��׮������
void GTZX2_JYX_ROAD::FormLCB2X(double ProjSml,double ProjEml,double Step)
{
	if (pLCB)
		delete pLCB;
	pLCB = NULL;
	NLCB = 0;
	if (_tcscmp(xlname,L"��ͨ��Ƣ���")!=0 && _tcscmp(xlname,L"��������")!=0 && _tcscmp(xlname,L"��������")!=0)
		return;

	CArray<LCB,LCB> LCBArray;
	LCB TempLCB;
	xlpoint PZ;
	int RxdSum = GetRXDNum();
	double RXDProjSml,RXDProjEml;//���ж����յ�ͶӰ���
	double SecProjSml,SecProjEml;//�������յ�ͶӰ���
	double SecSml,SecEml;////�������յ��������(����)
	GTZX_JYX_ROAD* pGTZX = NULL;
	CString Ckml;
	ACHAR ckml[20];
	bool IsGTSJEX = true;//�Ƿ��ͨ��ƶ���
	bool IsCalLCBProjCml = false;//�Ƿ����׮�����е�ͶӰ���

	if (_tcscmp(xlname,L"��ͨ��Ƣ���")==0)
		IsGTSJEX = true;

	CArray<Sec,Sec> SectionArray;
	Sec TempSec;
	if (RxdSum==0)
	{
		TempSec.Sml = QXB[0].KZH;
		TempSec.Eml = QXB[NJD-1].KHZ;
		TempSec.RXDIndex = -1;
		SectionArray.Add(TempSec);
	}
	else
	{
		//ÿ�����жκ����ж�֮ǰ�Ĳ��ж�
		for (int iRXD=0; iRXD<RxdSum; iRXD++)
		{
			GetiRxdStartEndMlByNum(iRXD,RXDProjSml,RXDProjEml);

			if (iRXD==0)
				TempSec.Sml = QXB[0].KZH;
			else
				TempSec.Sml = TempSec.Eml;
			TempSec.Eml = RXDProjSml;
			TempSec.RXDIndex = -1;
			if (TempSec.Eml-TempSec.Sml>0.1)
				SectionArray.Add(TempSec);

			TempSec.Sml = RXDProjSml;
			TempSec.Eml = RXDProjEml;
			TempSec.RXDIndex = iRXD;
			if (TempSec.Eml-TempSec.Sml>0.1)
				SectionArray.Add(TempSec);

		}

		//���һ�����жε��յ�
		TempSec.Sml = TempSec.Eml;
		TempSec.Eml = QXB[NJD-1].KHZ;
		TempSec.RXDIndex = -1;
		if (TempSec.Eml-TempSec.Sml>0.1)
			SectionArray.Add(TempSec);


	}

	int SecSum = SectionArray.GetSize();
	for (int iSec=0; iSec<SecSum; iSec++)
	{
		SecProjSml = max(SectionArray[iSec].Sml,ProjSml);
		SecProjEml = min(SectionArray[iSec].Eml,ProjEml);
		if (SecProjEml-SecProjSml>0.1)
		{
			if (SectionArray[iSec].RXDIndex!=-1)
			{
				if (IsGTSJEX)
					pGTZX = GetiRxdtoGTZXplinp_GTX(SectionArray[iSec].RXDIndex);
				else
					pGTZX = GetiRxdtoGTZXplinp(SectionArray[iSec].RXDIndex);

				PZ.lc = SecProjSml;
				xlpoint_pz(array,tarray,&PZ);
				_tcscpy(ckml,GetGTX2Xclc(PZ.x,PZ.y));
				SecSml = pGTZX->TYLC1(ckml,pGTZX->NDL,pGTZX->DLB);

				PZ.lc = SecProjEml;
				xlpoint_pz(array,tarray,&PZ);
				_tcscpy(ckml,GetGTX2Xclc(PZ.x,PZ.y));
				SecEml = pGTZX->TYLC1(ckml,pGTZX->NDL,pGTZX->DLB);

				pGTZX->FormLCB(Step,SecSml,SecEml);
				for (int iLCB=0; iLCB<pGTZX->NLCB; iLCB++)
				{
					LCBArray.Add(pGTZX->pLCB[iLCB]);

					if (IsCalLCBProjCml)
					{
						PZ.lc = pGTZX->pLCB[iLCB].lc;

						if (IsGTSJEX)
						{
							pGTZX->xlpoint_pz_GTX(pGTZX->array,pGTZX->tarray,&PZ);
						}
						else
						{
							PZ.lc = pGTZX->CalcmlTonewcml(PZ.lc);
							pGTZX->xlpoint_pz(pGTZX->array,pGTZX->tarray,&PZ);
						}
						LCBArray[iLCB].lc = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);

					}
				}
			}

			else
			{
				if (IsGTSJEX)
				{
					PZ.lc = SecProjSml;
					xlpoint_pz(array,tarray,&PZ);
					_tcscpy(ckml,m_pm1->GetGTXXclc(PZ.x,PZ.y));
					SecSml = m_pm1->TYLC1(ckml,m_pm1->NDL,m_pm1->DLB);

					PZ.lc = SecProjEml;
					xlpoint_pz(array,tarray,&PZ);
					_tcscpy(ckml,m_pm1->GetGTXXclc(PZ.x,PZ.y));
					SecEml = m_pm1->TYLC1(ckml,m_pm1->NDL,m_pm1->DLB);
				}
				else
				{
					SecSml = m_pm1->CalnewcmlTocml(SecProjSml);
					SecEml = m_pm1->CalnewcmlTocml(SecProjEml);
				}

				m_pm1->FormLCB(Step,SecSml,SecEml);

				for (int iLCB=0; iLCB<m_pm1->NLCB; iLCB++)
				{
					LCBArray.Add(m_pm1->pLCB[iLCB]);

					if (IsCalLCBProjCml)
					{
						PZ.lc = m_pm1->pLCB[iLCB].lc;
						if (IsGTSJEX)
						{
							m_pm1->xlpoint_pz_GTX(m_pm1->array,m_pm1->tarray,&PZ);
						}
						else
						{
							PZ.lc = m_pm1->CalcmlTonewcml(PZ.lc);
							m_pm1->xlpoint_pz(m_pm1->array,m_pm1->tarray,&PZ);
						}
						LCBArray[iLCB].lc = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
					}

				}
			}
		}
	}

	NLCB = LCBArray.GetSize();
	pLCB = new LCB[NLCB];
	for (int i=0; i<NLCB; i++)
	{
		pLCB[i] = LCBArray[i];
	}
}

//bool GTZX2_JYX::TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC)
//{
//	ACHAR GH[8];
//	double ProjCml = PROJ_ML(array,tarray,N,E,N,E);
//	double dml = XCLC(ProjCml,GH,DLB2x,NDL);
//	_tcscpy(Ckml , LCchr(GH,dml,NLC,1));
//
//	return true;
//}
//
//bool GTZX2_JYX::TrsNEtoProjml(double N, double E, double& Projml)
//{
//	Projml = PROJ_ML(array, tarray, N, E, N, E);
//	return true;
//}
//
//bool GTZX2_JYX::TrsNEtoCml(double N, double E, double& Cml)
//{
//	ACHAR Ckml[20];
//	TrsNEtoCkml(N, E, Ckml);
//	TrsCkmltoCml(Ckml, Cml);
//
//	return true;
//}
//
//bool GTZX2_JYX::TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ)
//{
//	CString CkmlString = Ckml;
//	PZ.lc = TYLC2x(CkmlString);
//	xlpoint_pz(array, tarray, &PZ);
//
//	return true;
//}
//bool GTZX2_JYX::TrsCkmltoProjml(ACHAR Ckml[], double& Projml)
//{
//	CString CkmlString = Ckml;
//	Projml = TYLC2x(CkmlString);
//
//	return true;
//}
//
//
//bool GTZX2_JYX::TrsCkmltoCml(ACHAR Ckml[], double& Cml, int iRXD)
//{
//	Cml = TYLC1(Ckml, m_2XGTDLSum, DLB);
//
//	return true;
//}
//
//bool GTZX2_JYX::TrsCmltoNE(double Cml, xlpoint& PZ)
//{
//	ACHAR Ckml[20];
//	TrsCmltoCkml(Cml, Ckml, 3);
//	TrsCkmltoNE(Ckml, PZ);
//
//	return true;
//}
//
//bool GTZX2_JYX::TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC)
//{
//	ACHAR GH[8];
//	double dml = XLC1(Cml, GH, m_2XGTDLSum, DLB);
//	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
//
//	return true;
//}
//
//bool GTZX2_JYX::TrsCmltoProjml(double Cml, double& Projml)
//{
//	ACHAR Ckml[20];
//	TrsCmltoCkml(Cml, Ckml, 3);
//	TrsCkmltoProjml(Ckml, Projml);
//
//	return true;
//
//}
//
//bool GTZX2_JYX::TrsProjmltoNE(double Projml, xlpoint& PZ)
//{
//	PZ.lc = Projml;
//	xlpoint_pz(array, tarray, &PZ);
//
//	return true;
//}
//
//bool GTZX2_JYX::TrsProjmltoCml(double Projml, double& Cml)
//{
//	ACHAR Ckml[20];
//	TrsProjmltoCkml(Projml, Ckml);
//	Cml = TrsCkmltoCml(Ckml, Cml);
//
//	return true;
//}
//
//bool GTZX2_JYX::TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC)
//{
//	ACHAR GH[8];
//	double dml = XCLC(Projml, GH, DLB2x, NDL);
//	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
//
//	return true;
//}
bool GTZX2_JYX_ROAD::TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC)
{
	if (_tcscmp(xlname, L"��ͨ��Ƣ���") ==0 )
	{
		_tcscpy(Ckml, GetGTX2Xclc(N, E));
	}
	else
	{
		ACHAR GH[8];
		double ProjCml = PROJ_ML(array,tarray,N,E,N,E);
		double dml = XCLC(ProjCml,GH,DLB2x,NDL);
		_tcscpy(Ckml , LCchr(GH,dml,NLC,1));
	}


	return true;
}

bool GTZX2_JYX_ROAD::TrsNEtoProjml(double N, double E, double& Projml)
{
	Projml = PROJ_ML(array, tarray, N, E, N, E);
	return true;
}

bool GTZX2_JYX_ROAD::TrsNEtoCml(double N, double E, double& Cml)
{
	ACHAR Ckml[20];
	TrsNEtoCkml(N, E, Ckml);
	TrsCkmltoCml(Ckml, Cml);

	return true;
}

bool GTZX2_JYX_ROAD::TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ)
{
	if (_tcscmp(xlname, L"��ͨ��Ƣ���")==0 )
	{
		GetPZFromXCLC2X(Ckml, PZ);
	}
	else
	{
		CString CkmlString = Ckml;
		PZ.lc = TYLC2x(CkmlString);
		xlpoint_pz(array, tarray, &PZ);
	}

	return true;
}
bool GTZX2_JYX_ROAD::TrsCkmltoProjml(ACHAR Ckml[], double& Projml)
{
	if (_tcscmp(xlname, L"��ͨ��Ƣ���")==0 )
	{
		xlpoint PZ;
		GetPZFromXCLC2X(Ckml, PZ);
		Projml = PZ.lc;
	}
	else
	{
		CString CkmlString = Ckml;
		Projml = TYLC2x(CkmlString);
	}
	return true;
}


bool GTZX2_JYX_ROAD::TrsCkmltoCml(ACHAR Ckml[], double& Cml, int iRXD)
{
	Cml = TYLC1(Ckml, m_2XGTDLSum, DLB);

	return true;
}

bool GTZX2_JYX_ROAD::TrsCmltoNE(double Cml, xlpoint& PZ)
{
	ACHAR Ckml[20];
	TrsCmltoCkml(Cml, Ckml, 3);
	TrsCkmltoNE(Ckml, PZ);

	return true;
}

bool GTZX2_JYX_ROAD::TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml = XLC1(Cml, GH, m_2XGTDLSum, DLB);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));

	return true;
}

bool GTZX2_JYX_ROAD::TrsCmltoProjml(double Cml, double& Projml)
{
	ACHAR Ckml[20];
	TrsCmltoCkml(Cml, Ckml, 3);
	TrsCkmltoProjml(Ckml, Projml);

	return true;

}

bool GTZX2_JYX_ROAD::TrsProjmltoNE(double Projml, xlpoint& PZ)
{
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, &PZ);

	return true;
}

bool GTZX2_JYX_ROAD::TrsProjmltoCml(double Projml, double& Cml)
{

	ACHAR Ckml[20];
	TrsProjmltoCkml(Projml, Ckml);
	Cml = TrsCkmltoCml(Ckml, Cml);

	return true;
}

bool GTZX2_JYX_ROAD::TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml = XCLC(Projml, GH, DLB2x, NDL);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));

	return true;
}
void GTZX2_JYX_ROAD::SetiRXDBPDArray(int iRXD, int NPD, PDTAB* BPDArr, bool IsAdd)
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

void GTZX2_JYX_ROAD::Create2XGTBPDArray(APDTAB&BPDArray)//��ͨ��ƶ���
{
	int RXDSum = AllRXDBPDArray.GetSize();
	PDTAB TempPDB;
	xlpoint PZ;
	double Projml2X = 0.0;
	ACHAR GH[8],Ckml[20];
	m_BPDArray.RemoveAll();
	if (RXDSum>0)
	{
		int BPD1xSum = BPDArray.GetSize();
		int iBPD1x=0;
		int SDLNo,EDLNo;
		SDLNo = EDLNo = -1;
		int iRXD = 0;
		GetiRXDDLinfo(iRXD, SDLNo, EDLNo);

		while (iRXD<RXDSum && iBPD1x<BPD1xSum)
		{
			m_pm1->TrsCkmltoNE(BPDArray[iBPD1x].CKml, PZ);
			TrsNEtoProjml(PZ.x, PZ.y, Projml2X);

			if (Projml2X < DLB2x[SDLNo].TYLC-0.01)
			{
				TempPDB = BPDArray[iBPD1x];
				TrsCkmltoCml(TempPDB.CKml, TempPDB.cml);
				m_BPDArray.Add(TempPDB);
			}
			if (Projml2X > DLB2x[EDLNo].TYLC+0.01)
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
						TempPDB = BPDArray[iBXBPD];
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
		m_BPDArray.Append(BPDArray);
		for (int i=0; i<m_BPDArray.GetSize(); i++)
		{
			_tcscpy(GH, m_BPDArray[i].GH);
			_tcscpy(Ckml, LCchr(GH, m_BPDArray[i].ml, 3, 1));
			_tcscpy(m_BPDArray[i].CKml, Ckml);
			TrsCkmltoCml(m_BPDArray[i].CKml, m_BPDArray[i].cml);
		}
	}


	int NBPD = m_BPDArray.GetSize();
	if (NBPD<3)
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

void GTZX2_JYX_ROAD::Create2XGTBPDArray()//���ж���
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

			if (Projml2X < DLB2x[SDLNo].TYLC-0.01)
			{
				TempPDB = m_pm1->m_BPDArray[iBPD1x];
				TrsCkmltoCml(TempPDB.CKml, TempPDB.cml);
				m_BPDArray.Add(TempPDB);
			}
			if (Projml2X > DLB2x[EDLNo].TYLC+0.01)
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
	if (NBPD<3)
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

//���ɶ��߹�ͨ������
void GTZX2_JYX_ROAD::Create2XGTDLB()
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

bool GTZX2_JYX_ROAD::BZCM(AcGiWorldDraw* pWd)
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
		TrsCmltoCkml(m_CM[i*5], Ckml, 3);
		_stprintf(Lever,L"%0.3lf",m_CM[i*5+1]);
		_stprintf(BefLen,L"%0.3lf",m_CM[i*5+3]);		
		_stprintf(AftLen,L"%0.3lf",m_CM[(i+1)*5+3]);
		_stprintf(BefSlope,L"%0.2lf��",1000*m_CM[i*5+2]);
		_stprintf(AftSlope,L"%0.2lf��",1000*m_CM[(i+1)*5+2]);
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

//���ñ�־�������
void GTZX2_JYX_ROAD::SetBZLCVal()
{   
	//�������յ㴦�ı�־���
	if(NBZLC<1) return;
	if(NJD<1) return;

	NBZLC = BZLCarray.GetSize();

	//���
	ACHAR Ckml[20];
	_tcscpy(BZLCarray[0].kml,LCchr(DLB[0].EGH, DLB[0].BLC+DLB[0].DL, NLC, 1));
	BZLCarray[0].X=JD2[0].x;
	BZLCarray[0].Y=JD2[0].y;
	BZLCarray[0].kmlTocml = DLB[0].BLC+DLB[0].DL;
	BZLCarray[0].xyTocml = DLB[0].BLC+DLB[0].DL;
	//�յ�
	_tcscpy(BZLCarray[NBZLC-1].kml,LCchr(DLB[m_2XGTDLSum-1].BGH,DLB[m_2XGTDLSum-1].BLC,NLC,1));
	BZLCarray[NBZLC-1].X=JD2[NJD-1].x;
	BZLCarray[NBZLC-1].Y=JD2[NJD-1].y;
	TrsCkmltoCml(BZLCarray[NBZLC-1].kml, BZLCarray[NBZLC-1].kmlTocml);
	BZLCarray[NBZLC-1].xyTocml = DLB[0].BLC+DLB[0].DL + xl_length;


	//��������־����е����ݸ���־��������е����г�Ա���¸�ֵ
	int i;
	for(i=1;i<NBZLC-1;i++)
	{	
		TrsCkmltoCml(BZLCarray[i].kml, BZLCarray[i].kmlTocml);
		TrsNEtoProjml(BZLCarray[i].X,BZLCarray[i].Y, BZLCarray[i].xyTocml);
	}

	//3. K
	double a=0.0,b=0.0;
	BZLCarray[0].K = 0.0; 
	for(i=1;i<NBZLC;i++)
	{
		a = BZLCarray[i].kmlTocml - BZLCarray[i-1].kmlTocml;
		b = BZLCarray[i].xyTocml - BZLCarray[i-1].xyTocml;
		if(fabs(a)>1.0e-6)
		{
			BZLCarray[i].K = b/a;

			//	if(fabs(b)<0.001)BZLCarray[i].K = 1.0;
		}
		else BZLCarray[i].K = 0.0;
	}	
}
void GTZX2_JYX_ROAD::TrsBPDArraytoCM()
{
	int NBPD = m_BPDArray.GetSize();
	if (NBPD==0)
		return;
	ACHAR GH[10],Ckml[20];
	double BPD_array[MAXBPDNUM][5];
	for (int i=0; i<NBPD; i++)
	{
		_tcscpy(GH, m_BPDArray[i].GH);
		_tcscpy(Ckml, LCchr(GH, m_BPDArray[i].ml, 3, 1));
		_tcscpy(m_BPDArray[i].CKml, Ckml);
		TrsCkmltoCml(m_BPDArray[i].CKml, m_BPDArray[i].cml);
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

void GTZX2_JYX_ROAD::FormSectionInfor()//�γɷֶ���Ϣ����
{

	int SizeOfGJDB = GJDInforArray.GetSize();

	GJDAllInforStruct TmpGJDInfor;
	if(SizeOfGJDB<1)return;
	//2.�ڲ����ʼ���ö�,�õ������ֶ���Ϣ

	//2.1���⴦��,�γɵ�һ�����ö�-----------------------------------------------------------------
	BasicSectionInfor.RemoveAll();
	BasicSectionInfor.SetSize(SizeOfGJDB*2 + 1);

	_tcscpy(BasicSectionInfor[0].BXLName ,GJDInforArray[0].BXLName);
	_tcscpy(BasicSectionInfor[0].EXLName ,GJDInforArray[0].BXLName);

	CString strtmp = GJDInforArray[0].BXLName;

	BasicSectionInfor[0].BpointToN = JDarray[0].N ;
	BasicSectionInfor[0].BpointToE = JDarray[0].E ;
	if (_tcscmp(GJDInforArray[0].BXLName,L"���л���")==0)	
		_tcscpy(BasicSectionInfor[0].GJDBLC , LCchr(JiXianpGtzx->DLB[0].EGH,JiXianpGtzx->DLB[0].BLC + JiXianpGtzx->DLB[0].DL,3,1));
	else if (_tcscmp(GJDInforArray[0].BXLName,L"��������")==0)
		_tcscpy(BasicSectionInfor[0].GJDBLC , LCchr(ZuoXianpGtzx->m_pm1->DLB[0].EGH,ZuoXianpGtzx->m_pm1->DLB[0].BLC+ZuoXianpGtzx->m_pm1->DLB[0].DL,3,1));
	else
		_tcscpy(BasicSectionInfor[0].GJDBLC , LCchr(YouXianpGtzx->m_pm1->DLB[0].EGH,YouXianpGtzx->m_pm1->DLB[0].BLC+YouXianpGtzx->m_pm1->DLB[0].DL,3,1));

	BasicSectionInfor[0].EpointToN = GJDInforArray[0].spt[0];
	BasicSectionInfor[0].EpointToE = GJDInforArray[0].spt[1];
	_tcscpy(BasicSectionInfor[0].GJDELC , GJDInforArray[0].GJDBLC);


	BasicSectionInfor[0].GJDType = -1; //���ö�
	//-------------------------------------------------------------------------------------------------







	//2.2.ѭ������,�γ��������öκ͸Ľ���,�õ������ֶ���ϢBasicSectionInfor----------------------------
	int Num;
	for(int i=0;i<SizeOfGJDB;i++)
	{
		//�Ľ��ηֶ���Ϣ
		Num = i*2 + 1;

		_tcscpy(BasicSectionInfor[Num].BXLName ,GJDInforArray[i].BXLName);
		_tcscpy(BasicSectionInfor[Num].EXLName ,GJDInforArray[i].EXLName);

		BasicSectionInfor[Num].BpointToN = GJDInforArray[i].spt[0];
		BasicSectionInfor[Num].BpointToE = GJDInforArray[i].spt[1];
		_tcscpy(BasicSectionInfor[Num].GJDBLC , GJDInforArray[i].GJDBLC);

		BasicSectionInfor[Num].EpointToN = GJDInforArray[i].ept[0];
		BasicSectionInfor[Num].EpointToE = GJDInforArray[i].ept[1];
		_tcscpy(BasicSectionInfor[Num].GJDELC ,GJDInforArray[i].GJDELC);


		BasicSectionInfor[Num].GJDType = GJDInforArray[i].GJDType;


		//���öηֶ���Ϣ
		if (i<SizeOfGJDB-1)
		{

			_tcscpy(BasicSectionInfor[Num+1].BXLName ,GJDInforArray[i].EXLName);
			_tcscpy(BasicSectionInfor[Num+1].EXLName ,GJDInforArray[i+1].BXLName);

			BasicSectionInfor[Num+1].BpointToN = GJDInforArray[i].ept[0];
			BasicSectionInfor[Num+1].BpointToE = GJDInforArray[i].ept[1];
			_tcscpy(BasicSectionInfor[Num+1].GJDBLC , GJDInforArray[i].GJDELC);

			BasicSectionInfor[Num+1].EpointToN = GJDInforArray[i+1].spt[0];
			BasicSectionInfor[Num+1].EpointToE = GJDInforArray[i+1].spt[1];
			_tcscpy(BasicSectionInfor[Num+1].GJDELC , GJDInforArray[i+1].GJDBLC);

			BasicSectionInfor[Num+1].GJDType = -1;

		}
	}
	//---------------------------------------------------------------------------------------





	//2.3���⴦��,�γ����һ�����ö�-----------------------------------------------------------

	_tcscpy(BasicSectionInfor[SizeOfGJDB*2].BXLName ,GJDInforArray[SizeOfGJDB-1].EXLName);
	_tcscpy(BasicSectionInfor[SizeOfGJDB*2].EXLName ,GJDInforArray[SizeOfGJDB-1].EXLName);

	BasicSectionInfor[SizeOfGJDB*2].BpointToN = GJDInforArray[SizeOfGJDB-1].ept[0];
	BasicSectionInfor[SizeOfGJDB*2].BpointToE = GJDInforArray[SizeOfGJDB-1].ept[1];
	_tcscpy(BasicSectionInfor[SizeOfGJDB*2].GJDBLC , GJDInforArray[SizeOfGJDB-1].GJDELC);

	BasicSectionInfor[SizeOfGJDB*2].EpointToN = JDarray[JDarray.GetSize()-1].N;
	BasicSectionInfor[SizeOfGJDB*2].EpointToE = JDarray[JDarray.GetSize()-1].E;

	if (_tcscmp(GJDInforArray[SizeOfGJDB-1].EXLName,L"���л���")==0)	
		_tcscpy(BasicSectionInfor[SizeOfGJDB*2].GJDELC , LCchr(JiXianpGtzx->DLB[JiXianpGtzx->NDL-1].BGH,JiXianpGtzx->DLB[JiXianpGtzx->NDL-1].BLC,3,1));
	else if (_tcscmp(GJDInforArray[SizeOfGJDB-1].EXLName,L"��������")==0)
		_tcscpy(BasicSectionInfor[SizeOfGJDB*2].GJDELC , LCchr(ZuoXianpGtzx->m_pm1->DLB[ZuoXianpGtzx->m_pm1->NDL-1].BGH,ZuoXianpGtzx->m_pm1->DLB[ZuoXianpGtzx->m_pm1->NDL-1].BLC,3,1));
	else
		_tcscpy(BasicSectionInfor[SizeOfGJDB*2].GJDELC , LCchr(YouXianpGtzx->m_pm1->DLB[YouXianpGtzx->m_pm1->NDL-1].BGH,YouXianpGtzx->m_pm1->DLB[YouXianpGtzx->m_pm1->NDL-1].BLC,3,1));

	BasicSectionInfor[SizeOfGJDB*2].GJDType = -1; //���ö�






	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
}

