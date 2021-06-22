#include "stdafx.h"
#include "resource.h"
#include "GTZX.h"
#include "triDTM_road.H"
//#include "LDZDM.H"
#include "dmx.h"
//#include "CVIEWZDM.h"
//#include "CXTYPEHDM.h"
//#include "DockTypeHdmBar.h"
#include "XLdataBase.h"
//#include "ERRMESDlg.h"
//#include "MyDockControlBar.h"
#include "PmPointer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define  sign(y, x)  { if ((x) == 0)        \
	y =0;             \
	else if ((x) > 0)    \
	y =1;             \
	else                 \
	y = -1;           \
}


#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS"
//extern triDTM *ptriDTM;
//extern CMyDockControlBar *dockBar;	
//LDZDM lddm;
//ERRMESDlg *ERRdlg;//Υ���涨�Ի���
//bool IfViewDmx=false;
//bool IfViewTypeHdm=false;//ƽ�������ʱ�Ƿ�鿴���ͺ����
//CVIEWZDM ViewZdmdlg;//һ�廯����в鿴������
//ads_point HdmPt;//�鿴���Ͷ����λ��
//extern AcDbObjectId ZpId;//��¼��ǰ����
//extern DockTypeHdmBar *TypeHdmBar;

extern CArray<GTZXPoint,GTZXPoint> g_GTZXPtArray;
extern CArray<GTZX2Point,GTZX2Point> g_GTZX2PtArray;
extern CArray<GTZX_JYXPoint,GTZX_JYXPoint> g_GTZX_JYXPtArray;
extern CArray<GTZX2_JYXPoint,GTZX2_JYXPoint> g_GTZX2_JYXPtArray;

GTZX_ROAD::~GTZX_ROAD()
{
	m_ConstraintErrArray.RemoveAll();
	//	lddm.DestroyWindow ();
}

Acad::ErrorStatus  GTZX_ROAD::subTranformBy(const AcGeMatrix3d &)
{
	return Acad::eOk;
}

int GTZX_ROAD::INIT()
{
	isReFresh=true;

	IsdrawTZD = true;
	IsdrawLC = true;
	IsdrawXLM = true;
	IsdrawBPD = true;
	IsdrawJDLX = true;
	IsdrawJD = true;


	station_num = 0;
	int i=0,j;
	for (i=0;i<MAXJDNUM;i++)
	{   
		for ( j=0;j<6;j++) JDarray[i][j]=0.0;
		for (  j=0;j<9;j++)   QXB[i][j]=0.0;
		//  for (  j=0;j<3;j++)   QXLRL[i][j]=0.0;
	};
	NJD=0; 
	for (i=0;i<30;i++) {   DLB[i].DL=0.0; DLB[i].BLC=0.0;
	_tcscpy(DLB[i].BGH,L"\0"); _tcscpy(DLB[i].EGH,L"\0");		
	};
	_tcscpy( m_XLRelate.CurXLNme,L"\0");	
	m_XLRelate.EXLDCnum=0;
	m_XLRelate.InterXLSum=0;
	//m_XLRelate.InXLName="";
	_tcscpy( m_XLRelate.InXLName,L"\0");	
	//m_XLRelate.OutXLName="";
	_tcscpy( m_XLRelate.OutXLName,L"\0");	
	m_XLRelate.SXLDCnum=0;
	NDL=2;
	NH=3;
	NLC=2;
	Chinese_Engbz_type=1;
	JDTDBZ=5;
	qxbbz=1;
	draw_zybz=-1;
	JD0=0;
	pro = -1;
	Zx[0]=Zx[1]=-1;
	nout=0;
	nin=0;
	mode_display=0;//plh07.19a

	DLB[0].DL=0.0; DLB[i].BLC=0.0;
	_tcscpy(DLB[0].BGH,L"AK"); _tcscpy(DLB[0].EGH,L"AK");

	DLB[1].DL=0.0; DLB[1].BLC=9000000.0;
	_tcscpy(DLB[1].BGH,L"AK"); _tcscpy(DLB[1].EGH,L"AK");

	NDL=2;

	_tcscpy(JDGH,L"JD");
	texth=4;    
	/*
	XLDataBase xlmdb;
	CString prj_fname;
	prj_fname.Format(L"%s\\data\\%s.mdb",Cworkdir,Pro);
	xlmdb.Read_XLDbs(prj_fname,L"��Ҫ������׼");
	DV = xlmdb.JSBZ.SDMB;
	RtjMin = xlmdb.JSBZ.RtjMin;
	RtjMax = xlmdb.JSBZ.RtjMax;
	TLDJ = xlmdb.JSBZ.TLDJ;
	XJJ =  xlmdb.JSBZ.XJJ;
	ZXSM = xlmdb.JSBZ.ZXSM;
	Rmin1 = xlmdb.JSBZ.Rmin1;
	Rmin2 = xlmdb.JSBZ.Rmin2;*/
	CString ProName = GetProByMdbName(mdbname);
	// ReadGuifanFile(ProName,mGuiFanCS);
	DV = mGuiFanCS.SuDuMuBiao;
	RtjMin = mGuiFanCS.TJianR1;//�Ƽ��뾶С
	RtjMax = mGuiFanCS.TJianR2;//�Ƽ��뾶��
	TLDJ = mGuiFanCS.TieLuDengJi;//��·�ȼ�
	XJJ = mGuiFanCS.XianJianJu;
	if(mGuiFanCS.ZhXianShu==1)		
		ZXSM ="����";
	else if(mGuiFanCS.ZhXianShu==2)
		ZXSM ="˫��";
	else
		ZXSM ="Ԥ������";
	Rmin1=mGuiFanCS.MinR1;
	Rmin2=mGuiFanCS.MinR2;

	m_LJorGM = 0;
	m_Xsw = 1;
	m_DesHXSW = 2;
	m_ConstraintErrArray.RemoveAll();
	m_BPDArray.RemoveAll();
	m_NCM = 0;
	m_NeedInterXLNum = 0;


	return 1;
}

GTZX_ROAD::GTZX_ROAD()
{ 
	INIT();
}

GTZX_ROAD::GTZX_ROAD(int iNJD,double iJDarray[MAXJDNUM][6],ACHAR *GH,ACHAR *JDchr)
{
	int i=0,j;
	ACHAR EGH[8];
	INIT();

	_tcscpy(JDGH,JDchr);
	for (i=0;i<iNJD;i++)
		for ( j=0;j<6;j++) JDarray[i][j]=iJDarray[i][j];	  

	JD0 = JDarray[0][0];
	NJD=iNJD; 
	NDL=1;  
	setDLM(0,JDarray[0][3],0.0,GH,GH); 

	NDL=2;
	_tcscpy(EGH,GH);//plh04.11.25
	setDLM(1,950,250,GH,GH );//plh04.11.25 	
	//  ����NJD����������(����㡢�յ�)JDarray[][]������R L1��L2 ������Ԫarray_pxy[][][]�����߱�QXB[][]
	getpxy_DATA();   
	NDL=2; 
	setDLM(1,QXB[NJD-1][0],0.0,EGH,EGH ); //plh04.11.25	
}

void GTZX_ROAD::Set(int iNJD,double iJDarray[MAXJDNUM][6],ACHAR *GH,ACHAR *JDchr)
{
	int i=0,j;
	ACHAR EGH[8];
	INIT();

	_tcscpy(JDGH,JDchr);
	for (i=0;i<iNJD;i++)
		for ( j=0;j<6;j++) JDarray[i][j]=iJDarray[i][j];	  

	JD0 = JDarray[0][0];
	NJD=iNJD; 
	NDL=1;  
	setDLM(0,JDarray[0][3],0.0,GH,GH); 

	NDL=2;
	_tcscpy(EGH,GH);//plh04.11.25
	setDLM(1,950,250,GH,GH );//plh04.11.25 	
	//  ����NJD����������(����㡢�յ�)JDarray[][]������R L1��L2 ������Ԫarray_pxy[][][]�����߱�QXB[][]
	getpxy_DATA();   
	NDL=2; 
	setDLM(1,QXB[NJD-1][0],0.0,EGH,EGH ); //plh04.11.25	
}

// Adesk::Boolean  GTZX::getpxy_DATA( double arrayd[100][7],int trow)
//  ����NJD����������(����㡢�յ�)JDarray[][]������R L1��L2 ������Ԫarray_pxy[][][]�����߱�QXB[][]
Adesk::Boolean  GTZX_ROAD::getpxy_DATA()
{     // ���߷��ɵ��������ļ�����λ L.SC.S.L.S.C.S.L...  �õ���Ԫ�ļ�
	double fb,fe,db,de,aa,c,lcb,lce,T0=0.0, interal=18;
	ACHAR zy[2],GH[20];
	int i=0,j=0,k,color=2,iq=2;
	double BPMTL[10],T1,T2,L;
	double LCB,DDlength;
	int iDL=0;

	for (i=0;i<10;i++)
		BPMTL[i]=0.0;
	for(i=0;i<MAXJDNUM;i++)
	{
		for (k=0;k<4;k++)
		{
			for (j=0;j<5;j++)
				array_pxy[i][k][j]=0.0;
		}
	}
	//���յ�뾶ӦΪ0    
	JDarray[0][4]=JDarray[NJD-1][4]=JDarray[0][5]=JDarray[NJD-1][5]=0.0;

	db=xyddaa(JDarray[0][1],JDarray[0][2],JDarray[1][1],JDarray[1][2],&fb);
	//   JDarray[0][6]=fb;  
	lcb=JDarray[0][3]; //�����ʼ���
	lce=lcb;
	fb=dms_rad(fb);//��㷽λ��
	array_pxy[0][0][0]=10.0;
	// N(X)=ACAD_Y
	array_pxy[0][0][1]=JDarray[0][1];//N����
	//E(Y)=ACAD_X
	array_pxy[0][0][2]=JDarray[0][2];//E����
	// FA
	array_pxy[0][0][3]=fb;
	// LC
	array_pxy[0][0][4]=JDarray[0][3];
	//  DLB[0].BLC=JDarray[0][3];   DLB[0].DL=0.0;  NDL=1;   // ��� ��Ϊһ���������

	QXB[0][0]=JDarray[0][3];  // ������
	QXB[0][1]=QXB[0][0];
	QXB[0][2]=QXB[0][0];
	QXB[0][3]=QXB[0][0];
	QXB[0][4]= QXB[0][5]= QXB[0][6]= QXB[0][7]= QXB[0][8]=0.0;
	T0=0.0;
	if (NJD==2) // ������������,��һ��ֱ��ʱ
	{
		array_pxy[1][0][0]=1.0;
		array_pxy[1][0][1]=db;
	};

	LCB=QXB[0][0]; // ������
	DDlength= LCB;
	iDL=1;

	for (i=1;i<NJD-1;i++)
	{  
		de=xyddaa(JDarray[i][1],JDarray[i][2],JDarray[i+1][1],JDarray[i+1][2],&fe);
		fe=dms_rad(fe);
		aa=rad(fe)-rad(fb);
		if (aa<-pi)
			aa=aa+2*pi;
		if  (aa>pi)
			aa=aa-2*pi;
		if (aa>0) 
		{
			c=1;
			_tcscpy(zy,L"y");
		}  //��ƫ
		else
		{
			c=-1;
			_tcscpy(zy,L"z");
		}//��ƫ
		aa=fabs(aa);
		aa=dms_rad(aa);
		//if (JDarray[i][4]<=0)	//����ᵼ����Ԫ���������Ϊ����������0
		//	break;
		getBPMTL(aa,JDarray[i][4],JDarray[i][3],JDarray[i][5],&BPMTL[0]);
		T1=BPMTL[6];
		T2=BPMTL[7];
		L=BPMTL[8];//���߳�=ǰ��+Բ����+��

		if ( L<(JDarray[i][3]+JDarray[i][5]))
		{
			ads_printf(L"\n%d������ ǰ�󻺺������ص�������L1+L2=%10.3f>�������߳�L=%10.3f ",i,JDarray[i][3]+JDarray[i][5],L);
		}
		else if((L-JDarray[i][3]-JDarray[i][5])<20.0)
		{ 
			ads_printf(L"\n %d�����ߵ�(R=%8.1f)Բ���߳�L=%10.3f ����20m ",i,JDarray[i][4],L-JDarray[i][3]-JDarray[i][5]);
		}

		//����Ҫ�� �Ƕ�Ϊ��.����
		QXB[i][4]=aa;
		QXB[i][5]=T1;
		QXB[i][6]=T2;
		QXB[i][7]=L;
		QXB[i][8]=c;//����ƫ
		//  line
		array_pxy[i][0][0]=1.0;
		array_pxy[i][0][1]=db-T1-T0;//��ֱ�߳���

		//ZH (zy)
		QXB[i][0]=QXB[i-1][3]+(db-T1-T0);   
		if( (db-T1-T0)<20.0) 
		{
			ads_printf(L"\n %d���������ZH/ZY��ǰһ����HZ/ZY����=%-10.3f ",i+JD0-1,db-T1-T0);
			if( (db-T1-T0)<0)
				ads_printf(L"  ����ǰһ����HZ/YZ���Ϸ� ");
		}
		aa=sqrt(JDarray[i][3]*JDarray[i][4]);
		if ( aa>0.000000001)
		{
			array_pxy[i][1][0]=3.0; 
			array_pxy[i][1][1]=aa;
			//array_pxy[i][1][2]=QXLRL[i][1];
			array_pxy[i][1][2]=JDarray[i][4];
			array_pxy[i][1][3]=0.0;
			array_pxy[i][1][4]=c;
		}
		// HY
		QXB[i][1]=QXB[i][0]+JDarray[i][3];
		//  circle 
		aa=L-JDarray[i][3]-JDarray[i][5];
		if ( aa>0.000000001)
		{
			array_pxy[i][2][0]=2.0; 
			array_pxy[i][2][1]=JDarray[i][4];
			array_pxy[i][2][2]=aa;
			array_pxy[i][2][3]=0.0;
			array_pxy[i][2][4]=c;
		}
		// YH
		QXB[i][2]=QXB[i][1]+aa;
		//2-spiral
		aa=sqrt(JDarray[i][5]*JDarray[i][4]);
		if ( aa>0.000000001)
		{
			array_pxy[i][3][0]=4.0; 
			array_pxy[i][3][1]=aa;
			array_pxy[i][3][2]=JDarray[i][4];
			array_pxy[i][3][3]=0.0;
			array_pxy[i][3][4]=c;
		} 
		// HZ
		QXB[i][3]=QXB[i][2]+JDarray[i][5];
		db=de;
		lcb=lce;
		fb=fe;
		T0=T2;
	}
	//  line
	db=db-T0 ;
	if ( db>0.0000000001) 
	{
		array_pxy[NJD-1][0][0]=1.0;	 
		array_pxy[NJD-1][0][1]=db;
	}

	// �յ����
	QXB[NJD-1][0]=QXB[NJD-2][3]+ db;  
	xl_length=QXB[NJD-1][0]-QXB[0][0];
	QXB[NJD-1][1]=QXB[NJD-1][0];
	QXB[NJD-1][2]=QXB[NJD-1][0];
	QXB[NJD-1][3]=QXB[NJD-1][0];
	QXB[NJD-1][4]=QXB[NJD-1][5]=QXB[NJD-1][6]=QXB[NJD-1][7]=QXB[NJD-1][8]=0.0;
	// �޸��յ������������� (�յ� ��Ϊһ���������)
	ACHAR Sgh[10],Egh[10];
	_tcscpy(Sgh,DLB[NDL-1].BGH);
	_tcscpy(Egh,DLB[NDL-1].EGH);//�޸��յ�����ں�  2004.7.16
	lcb = XLC(QXB[NJD-1][0],DLB[NDL-1].BGH,NDL-1);//2005-12-16
	if (_tcscmp(DLB[NDL-1].BGH,DLB[NDL-1].EGH) != 0 || fabs(DLB[NDL-1].DL) > 0.00001)
		lce = DLB[NDL-1].BLC + DLB[NDL-1].DL;//2005-12-16
	else
		lce = lcb;		
	setDLM(NDL-1,lcb,lce-lcb,Sgh,Egh); 	 
	//  ���
	double INDATA[5],l,L0;
	struct xlpoint PB0,PEZ;
	for (j=0;j<5;j++)
		array[0][j]=array_pxy[0][0][j]; 
	tarray = 1;
	for(i=1;i<NJD-1;i++) 
	{ 
		for (k=0;k<4;k++) 
		{		   
			for (j=0;j<5;j++)
				INDATA[j]=array_pxy[i][k][j];
			j=(int)(INDATA[0]+0.01);
			l=0.0;
			if (j==1)
				l=INDATA[1];
			else if (j==2)
				l=INDATA[2];
			else if (j==3||j==4)   
			{
				if(fabs(INDATA[2])>0.000001)
					l=INDATA[1]*INDATA[1]/INDATA[2];
				else
					l=0.0;
			}
			else if (j==5)
			{
				if(fabs(INDATA[2]*INDATA[3])>0.000001)
					l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
				else
					l=0.0;
			}
			else if (j==6)
			{
				if(fabs(INDATA[2]*INDATA[3])>0.000001)
					l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);
				else
					l=0.0;
			}
			// ��L0=��
			L0=l;
			if (j==2)
				L0=INDATA[1];
			//		   ads_printf(L"l=%lf\n",l);
			if (L0>0.001)
			{
				for (int kk=0;kk<5;kk++)
					array[tarray][kk]=array_pxy[i][k][kk]; 
				if(j==1)//ֱ��
				{
					db=xyddaa(JDarray[i-1][1],JDarray[i-1][2],JDarray[i][1],JDarray[i][2],&fb);
					array[tarray][5]=fb;
					array[tarray][6]=QXB[i-1][3];
					array[tarray][7]=1e40;
					array[tarray][8]=JDarray[i-1][1]+QXB[i-1][6]*cos(fb);
					array[tarray][9]=JDarray[i-1][2]+QXB[i-1][6]*sin(fb);
				}
				// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",array[trow][0],array[trow][1],array[trow][2],array[trow][4]);
				tarray=tarray+1;  
			};  		   
		};
	};
	//  �յ�
	for (j=0;j<5;j++)array[tarray][j]=array_pxy[NJD-1][0][j]; 	
	if(NJD>2)
	{
		i=NJD-1;
		db=xyddaa(JDarray[i-1][1],JDarray[i-1][2],JDarray[i][1],JDarray[i][2],&fb);
		array[tarray][5]=fb;
		array[tarray][6]=QXB[i-1][3];
		array[tarray][7]=1e40;
		array[tarray][8]=JDarray[i-1][1]+QXB[i-1][6]*cos(fb);
		array[tarray][9]=JDarray[i-1][2]+QXB[i-1][6]*sin(fb);
	}	

	tarray=tarray+1;
	i = 0;
	PB0.lc=array[0][4];
	PB0.x =array[0][1];
	PB0.y =array[0][2];
	PB0.a =rad(array[0][3]);
	PB0.r =1e40;
	array[1][5] = PB0.a ;
	array[1][6] = PB0.lc;
	array[1][7] = PB0.r ;
	array[1][8] = PB0.x ;
	array[1][9] = PB0.y ;
	for (i = 1;i<tarray;i++)
	{ 
		// �� i ��Ԫ
		for (k=0;k<5;k++)
			INDATA[k]=array[i][k];

		j=(int)(array[i+1][0]+0.0001);
		if(j==1)//��һ��Ԫ��ֱ�� 
		{
			PB0.a=array[i+1][5];
			PB0.lc=array[i+1][6];
			PB0.r=array[i+1][7];
			PB0.x=array[i+1][8];
			PB0.y=array[i+1][9];
		}
		else
		{
			l=Get_INDATA_L(INDATA,&j);
			xlzbxy(&PB0,&PEZ,l,INDATA,0); 
			array[i+1][5] = PEZ.a ;
			array[i+1][6] = PEZ.lc;
			array[i+1][7] = PEZ.r ;//��Ԫ���İ뾶,ֱԲ\ԲֱΪ�����
			array[i+1][8] = PEZ.x ;
			array[i+1][9] = PEZ.y ;
			PB0=PEZ;
		}
	}	 
	xlpoint PZ1;
	PZ1.lc=QXB[NJD-1][0];
	xlpoint_pz(array,tarray,&PZ1);
	StartCml = QXB[0][0];
	EndCml = QXB[NJD-1][0];
	double xlc=XLC(StartCml,GH,NDL);
	_tcscpy(StartCKml,L"\0");
	_tcscpy(StartCKml,LCchr(GH,xlc,NLC,1));
	xlc=XLC(EndCml,GH,NDL);
	_tcscpy(EndCKml,L"\0");
	_tcscpy(EndCKml,LCchr(GH,xlc,NLC,1));
	acutPrintf(L"\n����:");//plh07.19a

	/*��ʱ�������Ԫ����

	FILE *pfl = _wfopen(L"F:\\����һ�廯DLL\\qxbxy.xls",L"w");
	for(j=0; j < tarray; j++)
	{
	fwprintf(pfl,L"%lf %lf %lf %lf %lf %lf %lf %lf\n",array[j][0],array[j][4],array[j][6],
	array[j][1],array[j][2],array[j][5],array[j][8],array[j][9]);
	}
	fclose(pfl);
	*/
	return 1;
}




//  �ɶ������ݱ� ��ĳͳһ��̵��ֳ���̺͹ں�
double GTZX_ROAD::XLC(double TYLC,ACHAR *GH, int NDL)//�����������޸�
{   
	double LCB=0;
	double XCLC=0.0;// LCB ������ͳһ���
	int iDL=0;
	CString strs1,strs2;
	for (iDL=0;iDL<NDL;iDL++)
	{
		//�ö�����ͳһ���
		if(iDL > 0)
			LCB=LCB+DLB[iDL].BLC-(DLB[iDL-1].BLC+DLB[iDL-1].DL);
		else
			LCB=DLB[iDL].BLC+DLB[iDL].DL;

		if (iDL == 0)
		{
			if (TYLC < LCB)
			{
				strs1 = DLB[0].BGH;
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				XCLC = DLB[0].BLC - LCB + TYLC;
				break;
			}
			else if (fabs(TYLC-LCB)<0.00005)//TYLC-LCB
			{
				//				if (fabs(DLB[0].BLC-floor(DLB[0].BLC))<0.00005)//��ǰ������
				//				{
				//					strs1 = DLB[0].BGH;
				//					XCLC = DLB[0].BLC;
				//				}
				//				else
				//				{
				strs1 = DLB[0].EGH;
				XCLC = DLB[0].BLC + DLB[0].DL;
				//				}
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				break;
			}
		}
		else if (iDL < NDL)
		{
			if (TYLC < LCB)
			{
				XCLC = DLB[iDL].BLC - LCB + TYLC;
				strs1 = DLB[iDL-1].EGH;
				strs2 = DLB[iDL].BGH;
				strs1.TrimLeft();
				strs1.TrimRight();
				strs1.MakeUpper();
				strs2.TrimLeft();
				strs2.TrimRight();
				strs2.MakeUpper();
				if (strs1 == strs2)
					_tcscpy(GH,strs1);
				else
				{
					if (DLB[iDL-1].DL < 0.00001 && XCLC < DLB[iDL-1].BLC)//in ǰ����
						_tcscpy(GH,strs1);							
					else if  (DLB[iDL].DL < 0.00001 && XCLC < DLB[iDL].BLC)//in ����
						_tcscpy(GH,strs2);
					else//ǰ�󶼲��ǳ���
					{
						if (2.0*(LCB-TYLC) > DLB[iDL].BLC - DLB[iDL-1].BLC - DLB[iDL-1].DL)
							_tcscpy(GH,strs1);
						else
							_tcscpy(GH,strs2);
					}
				}
				break;
			}
			else if (fabs(TYLC - LCB) < 0.00005)//==
			{
				if (fabs(DLB[iDL].BLC-floor(DLB[iDL].BLC))<0.00005)
				{
					strs1 = DLB[iDL].BGH;
					XCLC = DLB[iDL].BLC;
				}
				else
				{
					strs1 = DLB[iDL].EGH;
					XCLC = DLB[iDL].BLC + DLB[iDL].DL;
				}
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				break;
			}
		}
	}
	if (iDL == NDL)
	{
		XCLC = DLB[iDL-1].BLC + DLB[iDL-1].DL + TYLC - LCB;
		_tcscpy(GH,DLB[iDL-1].EGH);
	}
	return XCLC;
}

//  �ɶ������ݱ� ��ĳͳһ��̵��ֳ���̺͹ں�
double GTZX_ROAD::XLC( double TYLC,ACHAR *GH, int NDL,int &iDL)
{
	double LCB=0;
	double XCLC=0.0;// LCB ������ͳһ���
	CString strs1,strs2;

	for (iDL=0;iDL<NDL;iDL++)
	{
		//�ö�����ͳһ���
		if(iDL > 0)
			LCB=LCB+DLB[iDL].BLC-(DLB[iDL-1].BLC+DLB[iDL-1].DL);
		else
			LCB=DLB[iDL].BLC+DLB[iDL].DL;

		if (iDL == 0)
		{
			if (TYLC < LCB)
			{
				strs1 = DLB[0].BGH;
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				XCLC = DLB[0].BLC - LCB + TYLC;
				break;
			}
			else if (fabs(TYLC-LCB)<0.00005)//==
			{
				if (fabs(DLB[0].BLC-floor(DLB[0].BLC))<0.00005)
				{
					strs1 = DLB[0].BGH;
					XCLC = DLB[0].BLC;
				}
				else
				{
					strs1 = DLB[0].EGH;
					XCLC = DLB[0].BLC + DLB[0].DL;
				}
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				break;
			}
		}
		else if (iDL < NDL)
		{
			if (TYLC < LCB)
			{
				XCLC = DLB[iDL].BLC - LCB + TYLC;
				strs1 = DLB[iDL-1].EGH;
				strs2 = DLB[iDL].BGH;
				strs1.TrimLeft();
				strs1.TrimRight();
				strs1.MakeUpper();
				strs2.TrimLeft();
				strs2.TrimRight();
				strs2.MakeUpper();
				if (strs1 == strs2)
					_tcscpy(GH,strs1);
				else
				{
					if (DLB[iDL-1].DL < 0.00001 && XCLC < DLB[iDL-1].BLC)//in ǰ����
						_tcscpy(GH,strs1);							
					else if  (DLB[iDL].DL < 0.00001 && XCLC < DLB[iDL].BLC)//in ����
						_tcscpy(GH,strs2);
					else//ǰ�󶼲��ǳ���
					{
						if (2.0*(LCB-TYLC) > DLB[iDL].BLC - DLB[iDL-1].BLC - DLB[iDL-1].DL)
							_tcscpy(GH,strs1);
						else
							_tcscpy(GH,strs2);
					}
				}
				break;
			}
			else if (fabs(TYLC - LCB) < 0.00005)//==
			{
				if (fabs(DLB[iDL].BLC-floor(DLB[iDL].BLC))<0.00005)
				{
					strs1 = DLB[iDL].BGH;
					XCLC = DLB[iDL].BLC;
				}
				else
				{
					strs1 = DLB[iDL].EGH;
					XCLC = DLB[iDL].BLC + DLB[iDL].DL;
				}
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				break;
			}
		}
	}
	if (iDL == NDL)
	{
		XCLC = DLB[iDL-1].BLC + DLB[iDL-1].DL + TYLC - LCB;
		_tcscpy(GH,DLB[iDL-1].EGH);
	}
	return XCLC;
}


//  �ɶ������ݱ� ��ĳ�ֳ���̵�ͳһ���
double  GTZX_ROAD::TYLC( ACHAR ckml[80], int NDL )
{
	double TLC = 0.0;
	double TLCtmp = 0.0;
	int iDL=0;
	//������
	double XLC = 0.0;
	ACHAR GH[10];
	split_ckml(ckml,&XLC,GH);
	TLC=XLC;

	CString strstr1,strstr2;
	short int numb = 0;//ͳ�Ƽ����������̵ĸ���2005-08-26
	strstr1 = GH;
	strstr1.TrimLeft();
	strstr1.TrimRight();
	strstr1.MakeLower();
	double continueLC = DLB[0].BLC + DLB[0].DL;//������ͳһ���
	for (iDL = 1; iDL < NDL; iDL++)
	{
		if (DLB[iDL-1].BLC+DLB[iDL-1].DL-0.00005<XLC && XLC<DLB[iDL].BLC+0.00005)
		{
			strstr2 = DLB[iDL-1].EGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLC = continueLC + XLC - DLB[iDL-1].BLC - DLB[iDL-1].DL;
				numb++;
			}
			else
			{
				strstr2 = DLB[iDL].BGH;
				strstr2.TrimLeft();
				strstr2.TrimRight();
				strstr2.MakeLower();
				if(strstr1 == strstr2)
				{
					TLC = continueLC + XLC - DLB[iDL-1].BLC - DLB[iDL-1].DL;
					numb++;
					strstr2.Format(L"��%d�������ĶϺ���̹ں�%s���%d�������Ķ�ǰ��̹ں�%s��һ��,\n\n��������%s�ļ��㲻�ɿ�",
						iDL-1, DLB[iDL-1].EGH, iDL, DLB[iDL].BGH, ckml);
					AfxMessageBox(strstr2);
				}
			}
		}
		continueLC =continueLC + DLB[iDL].BLC - DLB[iDL-1].DL - DLB[iDL-1].BLC;
	}

	if (numb == 0)//����Ƿ������֮��
	{
		if (XLC<DLB[0].BLC+0.00005)//�����֮��
		{
			strstr2 = DLB[0].BGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLC = DLB[0].DL + XLC;
				numb++;
			}
		}
		else if (XLC>DLB[iDL-1].BLC-0.005 && XLC<DLB[iDL-1].BLC+0.005)
		{
			TLC = continueLC;
			return TLC;
		}
		else if (XLC > DLB[iDL-1].BLC + DLB[iDL-1].DL-0.00005)//���յ�֮��
		{
			strstr2 = DLB[iDL-1].EGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLC = continueLC + XLC - DLB[iDL-1].BLC - DLB[iDL-1].DL;
				numb++;
			}
		}
	}
	strstr1 = ckml;
	if (numb > 1)
		//AfxMessageBox(L"ϵͳ����" + strstr1 + "����·���ж��λ�ã������Ƕ���������������");
			ads_printf(L"ϵͳ���� %s ����·���ж��λ�ã������Ƕ���������������", ckml);
	else if (numb == 0)
		//AfxMessageBox(L"ϵͳ����" + strstr1 + "����·��û����λ�ã������Ƕ��������������������");
		ads_printf(L"ϵͳ���� %s ����·��û����λ�ã������Ƕ��������������������", ckml);
	return TLC;
}

//  
void  GTZX_ROAD::BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type)  
	//  ��ע���������̱�  
	//    zybz=1,-1  �����ұߣ����
{
	zybz=-1;

	AcGePoint3d  ptb,pt,pte,p1;
	double  B,WB,BZlc,lc,b,LCB,DDlength,DL;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[20],chr[50],ch[50];
	xlpoint  PZtmp; 

	AcDbObjectId eId;
	//	   NLCB=0;

	_tcscpy(GH,DLB[0].EGH);

	WB=1.5*texth;
	LCB=DLB[0].BLC+DLB[0].DL;

	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;pt[Z]=0.0;
	// ������ͳһ���
	DDlength=LCB;
	// ��������α�ע
	for (iDL=0;iDL<NDL;iDL++)
	{ 
		DL=DLB[iDL].DL;
		LCB= DLB[iDL].BLC+DL;  //���������� 
		_tcscpy(GH,DLB[iDL].EGH);
		_tcscpy(GH1,DLB[iDL].BGH);
		// ads_printf(L"\n iDL=%d %s %10.3f  %s %10.3f",iDL,DLB[iDL].BGH,DLB[iDL].BLC,DLB[iDL].EGH,DLB[iDL].BLC+DL);

		// ������� ��ںŸı��
		if (iDL>0 && iDL< NDL-1)  // ��Ϊ�� �յ�
		{ // ��� iDL��
			lc=DDlength;
			PZtmp.lc=lc;  // ͳһ���
			k=xlpoint_pz(array,tarray,&PZtmp);
			if (k>0&& k<=6)  //�н�
			{
				B=PZtmp.a;

				pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
				pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
				G_makeline(pWd,ptb,pt,2);
				// makeline(ptb,pt,5);

				PZtmp.lc=lc-1.2*texth;
				k=xlpoint_pz(array,tarray,&PZtmp); 

				B=PZtmp.a;
				p1[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
				p1[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

				PZtmp.lc=lc+2.0*texth;
				k=xlpoint_pz(array,tarray,&PZtmp); 
				B=PZtmp.a;
				pte[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
				pte[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

				b=B+pi*0.5;
				b=pi*0.5-b; if (zybz==-1) b=b+pi;
				if (b<0.0) b=2*pi+b;		  if (b>2*pi) b=b-2*pi;		         
				if(type<=1)  b=b-pi*0.5; 

				_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH1,DLB[iDL].BLC,NLC,1));  // ��ǰ���
				_tcscpy(chr,L"  ");_tcscat(chr,ch);   
				G_maketext(pWd,p1,chr,b,texth,4,1);  

				_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,LCB,NLC,1));     // �Ϻ����
				_tcscpy(chr,L" =");  _tcscat(chr,ch);   // e.f chr="ZH K10+123.45"   

				if (fabs( DL)>0.001)
				{  if ( DL>0.0)   { 
					_tcscat(chr, L" ���� "); 
					ads_rtos( DL,2,NLC,ch); _tcscat(chr, ch); 
					_tcscat(chr, L" m");  };
				if ( DL<0.0)   { 
					_tcscat(chr, L" ���� "); 
					ads_rtos(fabs(DL),2,NLC,ch); _tcscat(chr, ch); 
					_tcscat(chr, L" m");  };
				};

				G_maketext(pWd,pte,chr,b,texth,4,1);     

			};

		};

		zlc=(long int)(LCB/dlc);
		BZlc=zlc*dlc;  // ʵ����� 
		if ((BZlc-LCB)<0.0001)  { BZlc=BZlc+dlc;  }  
		_tcscpy(GH,DLB[iDL].EGH);  //�ں�


		do {
			// ͳһ���
			//pLCB[NLCB].lcflag=100;
			lc=DDlength+BZlc-LCB; 
			PZtmp.lc=lc;  // ͳһ���
			//pLCB[NLCB].lc=lc;			 
			k=xlpoint_pz(array,tarray,&PZtmp);   
			//pLCB[NLCB].x=PZtmp.y;
			//pLCB[NLCB].y=PZtmp.x;

			BZW=k;    //  K<0  �޽�
			if (k>0&& k<=6)  //�н�
			{
				// ads_printf(L"\n i=%d %10.3f %10.3f  %10.3f %10.3f ",iDL,PZtmp.lc, BZlc,LCB,DLB[iDL+1].BLC );  
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
				if (dlc>=100.0&&ii>=2)
				{  chr[ii-1]='\0';chr[ii-2]='\0';
				_tcscpy(ch,chr);
				if (ii>3&&chr[ii-3]=='0') // �����
				{ chr[ii-3]='\0';  _tcscpy(ch,GH); _tcscat(ch,chr); }//pLCB[NLCB].lcflag=1000;}
				else if(ii>3 && chr[ii-3]!='0')    // ���ױ�
				{ ch[0]=chr[ii-3];ch[1]='\0';};		    
				}
				else {
					_tcscpy(ch,GH);
					if ( ii<=2)
					{ _tcscpy(ch,L"+"); ch[1]='0';
					ch[2]=chr[ii-2]; ch[3]=chr[ii-1];ch[4]='\0';
					}
					else if ( ii>=3)
					{
						_tcscpy(chrtmp,L"+"); chrtmp[1]=chr[ii-3];
						chrtmp[2]=chr[ii-2]; chrtmp[3]=chr[ii-1];chrtmp[4]='\0';
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // �����
						{ chr[ii-3]='\0';
						_tcscpy(ch,GH);
						_tcscat(ch,chr); _tcscat(ch,chrtmp); //pLCB[NLCB].lcflag=1000;
						}
						else if (ii>3&&chr[ii-3]!='0')
						{   _tcscpy(ch,chrtmp);	    };
					};
					//  _tcscat(ch,chr);
				};
				// ads_printf(L"ch=%s chr=%s\n",ch,chr);
				//   ads_printf(L"\n %10.3f %10.3f  ",PZtmp.lc, BZlc);       
				if(type<=1)  b=b-pi*0.5; 

				pte[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz)+0.5*texth*sin(B);
				pte[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz)+0.5*texth*cos(B);

				G_maketext(pWd,pte,ch,b,texth,4,1);     
				/*
				_tcscpy(//pLCB[NLCB].bz,ch);
				NLCB++;
				*/
			};          
			BZlc=BZlc+dlc;
			if ( BZlc>DLB[iDL+1].BLC) BZW=-99;		 		 

		}while (BZW>0);

		DDlength=  DDlength+DLB[iDL+1].BLC-LCB;//������ͳһ��̡�
	} // IDL
	//	 for(int kk=0;kk<NLCB;kk++)acutPrintf(L"i=%d,lc=%0.3lf,x=%0.3lf,y=%0.3lf,bz=%s\n",kk,//pLCB[kk].lc,//pLCB[kk].x,//pLCB[kk].y,//pLCB[kk].bz);
}

//	��ע����� �����߲���
void GTZX_ROAD::BZJD(AcGiWorldDraw* pWd,double arrayd[MAXJDNUM][6],double qxb[MAXJDNUM][9],int njd,int JD0,ACHAR *JDchr)
{  double fb,fe,de,da,aa,dd,dt ;
AcGePoint3d  p1; 
ACHAR ch[60],ch1[60],chrtmp[80];
int i,c,n;
de=xyddaa(arrayd[0][1],arrayd[0][2],arrayd[1][1],arrayd[1][2],&fb);
for (i=1;i<njd-1;i++)
{  
	if (arrayd[i][4]<=0) break;
	de=xyddaa(arrayd[i][1],arrayd[i][2],arrayd[i+1][1],arrayd[i+1][2],&fe);
	aa= fe - fb ;
	if (aa<-pi) aa=aa+2*pi;
	if  (aa>pi) aa=aa-2*pi;
	if (aa>0) {  c=1; dd=texth;  }  //��ƫ
	else { c=-1; dd=texth+texth; }; //��ƫ
	de=fb+aa*0.5;

	ads_rtos(JD0+i,2,0,ch);
	_tcscpy(ch1,JDchr);
	_tcscat(ch1,ch); 
	n=_tcslen(ch1);
	p1[Z]=0.0;

	p1[X]=arrayd[i][2]-n*0.5*texth*sin(de)+dd*sin(de-c*pi*0.5) ; 
	p1[Y]=arrayd[i][1]-n*0.5*texth*cos(de)+dd*cos(de-c*pi*0.5);

	da= 0.5*pi-de;
	G_maketext(pWd,p1,ch1,da,texth ,4,1);    // �����  
	// maketext( p1,ch1,de,texth ,4,1); 
	draw_zybz=0;
	if( qxbbz==1)
	{
		// �����߲��� begin
		double R,T,T1,T2,L1,L2,LL;
		R=arrayd[i][4]; L1= arrayd[i][3];  L2=arrayd[i][5]; 
		T1=qxb[i][5]; T2=qxb[i][6];  LL=qxb[i][7];
		T=(T1+T2)*0.5-(L1+L2)*0.25;

		dd=sqrt(R*R+T*T)-R;
		// ads_printf(L"\n dd=%8.2f",dd);
		if (draw_zybz==0) 
		{  dd=dd+16*texth; dt=c*texth*1.5;
		if (c==-1) dd=dd+8*texth;
		}
		else {   dd=dd+draw_zybz*c*16*texth; dt=c*texth*1.5; 
		if (draw_zybz==-1) dd=dd-8*c*texth;
		}
		// ads_printf(L"\ dd=%8.2f",dd);
		p1[X]=arrayd[i][2]-dd*sin(de-c*pi*0.5) ; 
		p1[Y]=arrayd[i][1]-dd*cos(de-c*pi*0.5);
		_tcscpy(chrtmp,L"  "); _tcscat(chrtmp,ch1);
		G_maketext(pWd,p1,chrtmp,da,texth ,4,1);   // �����

		ads_rtos(qxb[i][4],2,4,ch);
		if (c==1) { _tcscpy(ch1,L"ay--"); }
		else _tcscpy(ch1,L"az--"); 
		_tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		G_maketext(pWd,p1,ch1,da,texth ,4,1);   //  ƫ��

		if (fabs(R-(long int)(R))<0.001)  { ads_rtos(R,2,0,ch); }
		else { ads_rtos(R,2,NLC,ch); };
		_tcscpy(ch1,L"R--"); _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		G_maketext(pWd,p1,ch1,da,texth ,4,1);   //   �뾶

		if (L1>0||L2>0)  //  ������Ϊ��
		{
			if (fabs(L1-(long int)(L1))<0.001)  { ads_rtos(L1,2,0,ch); }
			else { ads_rtos(L1,2,NLC,ch); };
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if (fabs(L1-L2)<0.001) 
			{ _tcscpy(ch1,L"Lo--");  _tcscat(ch1,ch);
			G_maketext(pWd,p1,ch1,da,texth ,4,1);   //   ���� ���
			} else
			{ _tcscpy(ch1,L"L1--");  _tcscat(ch1,ch);
			if (fabs(L2-(long int)(L2))<0.001)  { ads_rtos(L2,2,0,ch); }
			else { ads_rtos(L2,2,NLC,ch); };
			_tcscat(ch1,L"  L2--"); _tcscat(ch1,ch);
			G_maketext(pWd,p1,ch1,da,texth ,4,1);   //   ���Ȼ���
			};
		};

		if (fabs(T1-(long int)(T1))<0.001)  { ads_rtos(T1,2,0,ch); }
		else { ads_rtos(T1,2,NLC,ch); };
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if (fabs(L1-L2)<0.001) 
		{   _tcscpy(ch1,L"T--");  _tcscat(ch1,ch);
		G_maketext(pWd,p1,ch1,da,texth ,4,1);   //   �г� ���
		} else
		{   _tcscpy(ch1,L"T1--");  _tcscat(ch1,ch);
		ads_rtos(T2,2,NLC,ch);
		_tcscat(ch1,L"  T2--"); _tcscat(ch1,ch);
		G_maketext(pWd,p1,ch1,da,texth ,4,1);   //   �����г�
		};

		ads_rtos(LL,2,NLC,ch);
		_tcscpy(ch1,L"L--");  _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		G_maketext(pWd,p1,ch1,da,texth ,4,1);   //  ���߳�
		// �����߲��� end
	};
	fb=fe; 
};  
}

//  �����������
void GTZX_ROAD::BZTZD(AcGiWorldDraw* pWd, double dlc, int bz_zybz,int Chinese_Engbz_type)  //  ��ע���
	// bz_zybz==0   ������ƫ���ע   bz_zybz=1,-1  �����ұߣ����
{
	AcGePoint3d  ptb,pt,pte;
	double WB,lc, b,l,INDATA[5],LCB,L0;
	int i, j,k , KK=7,zybz=1;

	ACHAR chr[50],ch[40],tch[6], GH[8],DGH[8],lcchr[20] ;
	xlpoint  P0tmp,PZtmp;

	zybz=bz_zybz;
	if( zybz<-1|| zybz>1)
		zybz=-1;

	_tcscpy(GH,DLB[0].EGH);
	LCB=DLB[0].BLC+DLB[0].DL;
	// _tcscpy(GH,L"CK");
	// texth=2.0;

	WB=4*texth;
	WB=WB+10*texth;

	// ads_initget(1+2+4,NULL);
	// ads_getint(L"\n Enter ��������� type=1 (in English) / type=2 (in Chinese)):",&Chinese_Engbz_type);
	//  if (Chinese_Engbz_type<1||Chinese_Engbz_type>2) Chinese_Engbz_type=1;
	P0tmp.lc=array[0][4]; 
	P0tmp.x=array[0][1];P0tmp.y=array[0][2];
	P0tmp.a=rad(array[0][3]);P0tmp.r=1e40;

	for (i=1;i<tarray;i++)
	{
		zybz=1;

		for (k=0;k<5;k++) INDATA[k]=array[i][k];j=(int)(array[i][0]+0.01);
		l=0.0;
		if (j>=3)
		{
			_tcscpy(chr,L"Lo=");
			_tcscpy(tch,L".S."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"H");
		}
		if (j==1) { l=INDATA[1];  _tcscpy(chr,L"L="); _tcscpy(tch,L".T."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"Z"); };
		if (j==2) { l=INDATA[2];  _tcscpy(chr,L"R="); _tcscpy(tch,L".C."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"Y"); };

		//  ��A=��
		if (j==3)   l=INDATA[1]*INDATA[1]/INDATA[2];
		if (j==4)   l=INDATA[1]*INDATA[1]/INDATA[2];
		if (j==5) l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
		if (j==6) l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);

		// ��L0=��
		L0=l; if (j==2) L0=INDATA[1];

		ptb[X]=P0tmp.y;ptb[Y]=P0tmp.x; ptb[Z]=0.0;pt[Z]=0.0;pte[Z]=0.0;
		if (zybz==1||zybz==-1)
		{
			lc=l*0.5-3.721*texth; if (lc<0.0) lc=l*0.5;
			xlzbxy(&P0tmp,&PZtmp,lc,INDATA,0);b=PZtmp.a+pi*0.5*zybz;
			pt[X]=PZtmp.y+2.0*texth*sin(b);pt[Y]=PZtmp.x+2.0*texth*cos(b);pt[Z]=0.0;
			b=PZtmp.a; b=pi*0.5-b;if (b<0.0) b=2*pi+b;
			if (b>2*pi) b=b-2*pi;
			// ads_rtos(INDATA[1],2,NLC,ch);_tcscat(chr,ch);
			if (fabs(L0-(long int)(L0+0.001))<0.001)  { ads_rtos(L0,2,0,ch); }
			else { ads_rtos(L0,2,NLC,ch); };
			_tcscat(chr,ch);  
			if (INDATA[0]>1&&INDATA[0]<3) //   ԰��
			{ads_rtos(INDATA[2],2,NLC,ch); _tcscat(chr,L" L="); _tcscat(chr,ch);  };

			if ( L0>0.001) 
				G_maketext(pWd,pt,chr,b,texth,4,1);  // �� ֱ�ߡ�԰���ߡ��������߳���

		};

		xlzbxy(&P0tmp,&PZtmp,l,INDATA,0); // ĳ��Ԫ�յ�
		// ���Ʒ���

		zybz=bz_zybz;
		if (bz_zybz==0) 
		{ j=(int)(array[i][0]+0.01);  // ���� 
		if (j>1&&j<=6)  zybz=(int)(array[i][4]+0.01);
		if(j==1) 		zybz=(int)(array[i-1][4]+0.01);	 
		if (zybz==0||zybz>1||zybz<-1) zybz=-1;
		// ads_printf(L"\n j=%d array[i][4]=%2.0f zybz=%d",j,array[i][4],zybz);
		}
		//
		b=P0tmp.a+pi*0.5*zybz;
		pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
		if ( L0>0.001) G_makeline(pWd,ptb,pte,3);

		if (i==1)//plh04.11.25
		{ 
			if(_tcscmp(DLB[0].BGH,DLB[0].EGH)!=0 || fabs(DLB[0].DL)>0.001)
			{
				b=PZtmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				//G_maketext(pWd,pt,chr,b,texth,4,1);
				b=PZtmp.a+pi*0.5*zybz;
				pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
				G_makeline(pWd,ptb,pte,3);

				ACHAR BlcStr[80],ElcStr[80];
				_tcscpy(BlcStr,L"  ");
				_tcscat(BlcStr,LCchr(DLB[0].BGH,DLB[0].BLC,3,1));//����ǰ���
				_tcscpy(ElcStr,L" =QD ");
				_tcscat(ElcStr,LCchr(DLB[0].EGH,DLB[0].BLC+DLB[0].DL,3,1));//���Ϻ����

				b=PZtmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(BlcStr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(BlcStr)+0.25)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,BlcStr,b,texth,4,1);	

				b=PZtmp.a;
				pt[X]=ptb[X]+1.25*texth*sin(b); pt[Y]=ptb[Y]+1.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(ElcStr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(ElcStr)+0.25)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,ElcStr,b,texth,4,1);	

			}
			else
			{
				_tcscpy(chr,L" B.P"); if(Chinese_Engbz_type==2)_tcscpy(chr,L" QD");
				if (KK==7) 
				{
					lc=P0tmp.lc;     lc=XLC(lc,GH,NDL);
					_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,lc,NLC,1));
					_tcscat(chr,L" ");_tcscat(chr,ch);   // e.f chr="ZH K10+123.45"
					_tcscpy(lcchr,ch);                 // e.f lcchr="K10+123.45"
				};
				b=P0tmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=P0tmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b);
				}			 
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,chr,b,texth,4,1);			 

				b=PZtmp.a+pi*0.5*zybz;
				pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
				G_makeline(pWd,ptb,pte,3);
			}
		}
		else if (i>1 && i<=tarray-1)
		{
			j=(int)(array[i-1][0]+0.01);
			if (j>=3) { _tcscpy(ch,L" S"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" H"); };
			if (j==1) { _tcscpy(ch,L" T"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" Z"); };
			if (j==2) { _tcscpy(ch,L" C"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" Y"); };
			_tcscpy(chr,ch);_tcscat(chr,tch);

			if (_tcsncmp(chr,L" YY",3)==0)   {  _tcscpy(chr,L" GQ");    };
			if (_tcsncmp(chr,L" C.C.",5)==0) {  _tcscpy(chr,L" P.C.C.");};
			if (_tcsncmp(chr,L" S.S.",5)==0&&(j==3||j==4) ) {  _tcscpy(chr,L" PRC");};
			if (_tcsncmp(chr,L" HH",3)==0&&(j==3||j==4)) {  _tcscpy(chr,L" FGD");};
			_tcscpy(DGH,chr);

			if (KK==7)
			{
				lc=P0tmp.lc;
				lc=XLC(lc,GH,NDL);
				_tcscpy(ch,L"\0");
				_tcscpy(ch,LCchr(GH,lc,NLC,1));
				_tcscat(chr,L" ");
				_tcscat(chr,ch);
			};
			b=P0tmp.a;
			// pt[X]=ptb[X]+0.25*zybz*texth*sin(b); pt[Y]=ptb[Y]+0.25*zybz*texth*cos(b);
			// b=P0tmp.a+pi*0.5*zybz;
			pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
			b=P0tmp.a-pi*0.5 ;
			if(zybz==1) {	pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
			pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b); }

			b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi; 
			G_maketext(pWd,pt,chr,b, texth,4,1);
			// ads_printf(L"\n i=%d,L0= %10.4f lc=%12.3f chr=%s",i,L0,lc,chr);

			if (i==tarray-1 ) //plh04.11.25
			{
				int nn=NDL-1;
				if(_tcscmp(DLB[nn].BGH,DLB[nn].EGH)!=0 || fabs(DLB[nn].DL)>0.00001)
				{
					ptb[X]=PZtmp.y; ptb[Y]=PZtmp.x; ptb[Z]=0.0;
					b=PZtmp.a;
					pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
					b=PZtmp.a-pi*0.5;
					if(zybz==1)
					{
						pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
						pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b);
					}
					b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
					//G_maketext(pWd,pt,chr,b,texth,4,1);
					b=PZtmp.a+pi*0.5*zybz;
					pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
					G_makeline(pWd,ptb,pte,3);

					ACHAR BlcStr[80],ElcStr[80];
					_tcscpy(BlcStr,L" ZD ");
					_tcscat(BlcStr,LCchr(DLB[nn].BGH,DLB[nn].BLC,3,1));//����ǰ���
					_tcscpy(ElcStr,L" = ");
					_tcscat(ElcStr,LCchr(DLB[nn].EGH,DLB[nn].BLC+DLB[nn].DL,3,1));//���Ϻ����

					b=PZtmp.a;
					pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
					b=PZtmp.a-pi*0.5 ;
					if(zybz==1)
					{
						pt[X]=pt[X]-(_tcslen(BlcStr)-2)*texth*sin(b);
						pt[Y]=pt[Y]-(_tcslen(BlcStr)-2)*texth*cos(b);
					}
					b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
					G_maketext(pWd,pt,BlcStr,b,texth,4,1);	

					b=PZtmp.a;
					pt[X]=ptb[X]+1.25*texth*sin(b); pt[Y]=ptb[Y]+1.25 *texth*cos(b);
					b=PZtmp.a-pi*0.5 ;
					if(zybz==1)
					{
						pt[X]=pt[X]-(_tcslen(ElcStr)-2)*texth*sin(b);
						pt[Y]=pt[Y]-(_tcslen(ElcStr)-2)*texth*cos(b);
					}
					b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
					G_maketext(pWd,pt,ElcStr,b,texth,4,1);	
				}
				else
				{
					ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; ptb[Z]=0.0;
					_tcscpy(chr,L" EP");  if (Chinese_Engbz_type==2)_tcscpy(chr,L" ZD");
					if (KK==7) {
						lc=PZtmp.lc;   lc=XLC(lc,GH,NDL);
						_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,lc,NLC,1));
						_tcscat(chr,L" ");   _tcscat(chr,ch);
					};

					b=PZtmp.a;
					//pt[X]=ptb[X]+0.25*zybz*texth*sin(b); pt[Y]=ptb[Y]+0.25*zybz*texth*cos(b);
					//b=PZtmp.a+pi*0.5*zybz;
					pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
					b=PZtmp.a-pi*0.5 ;
					if(zybz==1) {	pt[X]=pt[X]-(_tcslen(chr)-2)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(chr)-2)*texth*cos(b); }

					b=pi*0.5-b;if (b<0.0) b=2*pi+b;  if (b>2*pi) b=b-2*pi;
					G_maketext(pWd,pt,chr,b,texth,4,1);
					b=PZtmp.a+pi*0.5*zybz;
					pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
					G_makeline(pWd,ptb,pte,3);
					// ads_printf(L"\n i=%d,lc=%12.3f chr=%s",trow-1,lc, chr);
				}
			}
		}
		P0tmp=PZtmp;
	}
}

Adesk::Boolean  GTZX_ROAD::G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double LO,struct xlpoint *P0,struct xlpoint *PZ, double ND)
{   int k,nk;
double dlc;

nk=(int)(LO/ND)+1;
AcGePoint3d* pArr=new AcGePoint3d[nk+1];
pArr[0].set(P0->y,P0->x,0.0);
for (k=1;k<=nk;k++)
{     dlc=k*ND*1.0;
if (dlc>LO) dlc=LO;
xlzbxy(P0,PZ,dlc,INDATA,0);
// ads_printf(L"\n % %d %5.1f %10.3f %10.3f %10.4f",k,dlc,PZ->y,PZ->x,dms_rad(PZ->a));
pArr[k].set(PZ->y,PZ->x,0.0);
};

pWd->subEntityTraits().setColor(5);	
pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
pWd->geometry().polyline(nk+1,pArr);
// ads_printf(L"\n AA ����=%d %d %10.3f %10.3f %5.1f %10.2f",(int)INDATA[0],nk,INDATA[1],INDATA[2],ND,LO);
return  0;
}

Adesk::Boolean  GTZX_ROAD::G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx)
{     
	AcGePoint3d centerPt,midPt;

	double f1,dd,AA ;

	dd=xyddaa(startPt[1],startPt[0],endPt[1],endPt[0],&f1);
	AA=acos(dd*0.5/R);
	centerPt[Y]=startPt[Y]+R*cos(f1+npx*AA);
	centerPt[X]=startPt[X]+R*sin(f1+npx*AA);
	centerPt[Z]=startPt[Z];

	AA=asin(dd*0.5/R);
	midPt[Y]=centerPt[Y]+R*cos(f1-npx*pi*0.5);
	midPt[X]=centerPt[X]+R*sin(f1-npx*pi*0.5);
	midPt[Z]=centerPt[Z];

	pWd->subEntityTraits().setColor(1);	
	pWd->geometry().circularArc(startPt,midPt,endPt);
	//	ads_printf(L"\n R ����=2" );	ads_printf(L"\n R ����=2" ); 
	return 0 ;
}

Adesk::Boolean  GTZX_ROAD::G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor)
{     
	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	
	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt000);	
	pWd->geometry().polyline(2,pArr);
	return  0;
}

Adesk::Boolean  GTZX_ROAD::G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw)
{     
	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	
	pWd->subEntityTraits().setLineWeight(lw);	
	pWd->geometry().polyline(2,pArr);
	return  0;
}



Adesk::Boolean  GTZX_ROAD::G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode=1)
{   

	AcGeVector3d normal(0.0,0.0,1.0), direction(cos(ang), sin(ang), 0.0);
	// ads_printf(L"\n             %10.3f %10.3f %10.4f ",Pt.x,Pt.y,dms_rad(ang));
	pWd->subEntityTraits().setColor(icolor);	
	//    pWd->geometry().text(Pt , normal, direction, texth,1.0, 0.0, ctext);
	AcGiTextStyle pTextStyle;
	//pTextStyle.setFont(L"����",TRUE,FALSE,0,0);
	pTextStyle.setFont(L"����",0,0,134,2);
	pTextStyle.setTextSize(texth);
	AcGePoint3d pt1;
	double len=_tcslen(ctext)*texth*0.7;
	if (mode == 0)//�����Ϊ�����µ�ʱ
	{
		Pt.x = Pt.x - 0.5*len*cos(ang);
		Pt.y = Pt.y - 0.5*len*sin(ang);
	}
	else if (mode == 1)//�����Ϊ�����µ�ʱ
	{
		Pt.x = Pt.x ;
		Pt.y = Pt.y ;
	}
	else if (mode == 2)//�����Ϊ�����µ�ʱ
	{
		Pt.x = Pt.x - len*cos(ang);
		Pt.y = Pt.y - len*sin(ang);
	}
	else if (mode == 3)//�����Ϊ�����Ͻǵ�
	{
		Pt.x = Pt.x + texth * cos(ang-0.5*PI);
		Pt.y = Pt.y + texth * sin(ang-0.5*PI);
	}
	pWd->geometry().text(Pt,normal,direction,ctext,_tcslen(ctext),Adesk::kTrue,pTextStyle);
	return 1;
}

Adesk::Boolean  GTZX_ROAD::G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ)
{     int j;
double l=0.0;
AcGePoint3d ptb,pt,pte;
if(INDATA[1]<0.00000001) return 1;
l=Get_INDATA_L(INDATA,&j);
// ads_printf(L"\n  ����=%d l=%10.3f" ,j,l );
ptb[X]=P0->y;ptb[Y]=P0->x; ptb[Z]=0.0;
// ads_printf(L"\n PO=%10.3f %10.3f %10.3f %10.3f",P0->y,P0->x,dms_rad(P0->a),P0->lc) ;
if ((j<=3 &&j>=6 && INDATA[1]<=0.000000000000001)|| (l<=0.00000000001)) return 1;

if (j>2 && j<=6 )
{ //( ������)
	G_drawspi(pWd,INDATA,l,P0,PZ, 2.0);
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
	if(Zx[0]==-1&&Zx[1]==-1)
		pWd->subEntityTraits().setColor(3);	
	else
		pWd->subEntityTraits().setColor(4);	
	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
	pWd->geometry().polyline(2,pcArr);
	// ads_printf(L"\n L ����= 1"  );
	}
	else if (j==2) // ԰�� 
	{ 	pWd->subEntityTraits().setColor(1);	
	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
	pWd->geometry().circularArc(ptb,pt,pte);
	// ads_printf(L"\n r ����= 2"  );
	};
};  //J
// ads_printf(L"\n PZ=%10.3f %10.3f %10.3f %10.3f",PZ->y,PZ->x,dms_rad(PZ->a),PZ->lc) ;
return 0;
}

void GTZX_ROAD::INSERT_STATION(ACHAR CKml[80],ACHAR StaName[80],ACHAR TKSta[80],AcGiWorldDraw* pWd)
{
	/////////////////////////////////////////////////////////////
	//�ҳ�վͼ���λ��
	//acutPrintf(L"%s\n",TKSta);
	/*
	char SysPath[256],dwgPath[256],Name[50];
	GetPrivateProfileString("RLDVS",L"RLDVS",NULL,SysPath,256,L"win.ini");			
	_stprintf(dwgPath,L"%s\\DWG\\",SysPath);
	_stprintf(Name,L"%s.dwg",TKSta);
	*/

	/*
	AcDbObjectId pBlockId;
	pBlockId=CreateBlockFromDwgFile(dwgPath,Name);
	if(pBlockId==0)
	{
	acutPrintf(L"%s%s������!\n",dwgPath,Name);
	return;
	}
	*/
	//////////////////////////////////////////////////////////
	//AcDbObjectIdArray IdAr;
	double lc0;//����ͳһ���
	//lc0 = TYLC(CKml, NDL);
	lc0 = TYLC1(CKml, NDL,DLB);
	////////////////////////////////////////////////////////////
	double a0;//��·��λ��
	xlpoint Ptmp;

	if (lc0 < 0.001)
		lc0 = 0.001;
	Ptmp.lc=lc0;
	xlpoint_pz(array,tarray,&Ptmp);

	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//���������
	a0=Ptmp.a;
	//	AcDbBlockReference *pBkRr=new AcDbBlockReference(Pt0,pBlockId);
	double rotation = 2.5*pi-a0;
	//	if(draw_zybz==1)
	//		rotation+=pi;
	//	pBkRr->setRotation(rotation);
	//	pBkRr->setScaleFactors(AcGeScale3d(SCALE,SCALE,1));
	//	pBkRr->setLayer(layname);
	//	AddEntityToDbs(pBkRr);

	AcGePoint3d BZPt;//ע�ֵ�0;

	BZPt[X]=Ptmp.y+(2.0*texth)*sin(a0-pi*0.5)+0.8*texth*sin(a0+pi);
	BZPt[Y]=Ptmp.x+(2.0*texth)*cos(a0-pi*0.5)+0.8*texth*cos(a0+pi);

	G_maketext(pWd,BZPt,CKml,rotation+0.5*pi,2.0*texth,2);

	double WB=150.0;//��վ��־�ĸ߶�
	//	if(TKSta[3]=='0'&&TKSta[4]=='1')WB=41;
	//	else if(TKSta[3]=='0'&&TKSta[4]=='4')WB=35;
	//	else if(TKSta[3]=='0'&&TKSta[4]=='6')WB=33;
	//	else if(TKSta[3]=='0'&&TKSta[4]=='7')WB=39.1924;
	//	else if(TKSta[3]=='0'&&TKSta[4]=='8')WB=35;
	//	else if(TKSta[3]=='0'&&TKSta[4]=='9')WB=41;
	//	else if(TKSta[3]=='1'&&TKSta[4]=='0')WB=41;
	//	else if(TKSta[3]=='1'&&TKSta[4]=='5')WB=36;
	//	else if(TKSta[3]=='1'&&TKSta[4]=='6')WB=39.1924;
	//	else if(TKSta[3]=='1'&&TKSta[4]=='7')WB=35;
	//	else if(TKSta[3]=='1'&&TKSta[4]=='8')WB=35;
	//	else WB=41;
	//	WB=WB*SCALE;
	ACHAR str[100];
	_tcscpy(str, StaName);
	_tcscat(str,L" վ");
	BZPt[X]=Pt0.x+(WB+2.5*texth)*cos(2.5*pi-a0+0.5*pi);
	BZPt[Y]=Pt0.y+(WB+2.5*texth)*sin(2.5*pi-a0+0.5*pi);	
	BZPt[X]=BZPt.x-(0.5*0.7*4.0*_tcslen(str)*texth)*cos(2.5*pi-a0);
	BZPt[Y]=BZPt.y-(0.5*0.7*4.0*_tcslen(str)*texth)*sin(2.5*pi-a0);
	rotation = 2.5*pi - a0;
	G_maketext(pWd,BZPt,str,rotation,4.0*texth,2);

	AcGePoint3d ptb,pte;

	ptb[X]=Ptmp.y;
	ptb[Y]=Ptmp.x;
	pte[X]=ptb.x+WB*sin(a0-pi*0.5);
	pte[Y]=ptb.y+WB*cos(a0-pi*0.5);

	G_makeline(pWd,ptb,pte,2);

	ptb[X]=pte.x-50.0*cos(2.5*pi-a0);
	ptb[Y]=pte.y-50.0*sin(2.5*pi-a0);
	pte[X]=pte.x+50.0*cos(2.5*pi-a0);
	pte[Y]=pte.y+50.0*sin(2.5*pi-a0);
	G_makeline(pWd,ptb,pte,2);
}


Adesk::Boolean  GTZX_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{
	AcGePoint3d Pt,*verts=new AcGePoint3d[NJD];
	AcGeVector3d  normal(0.0,0.0,1.0);
	int i=0,j=0;
	int k=0;
	double INDATA[5],LCB;
	xlpoint  PB,PE; 
	// �潻������
	if ( mode_display == 0)
	{
		if (IsdrawJDLX)
		{
			for (i = 0; i < NJD; i++) 
			{
				verts[i].x=JDarray[i][2]; verts[i].y=JDarray[i][1];verts[i].z=0.0;
			}
			if (pWd!= NULL) 
			{
				pWd->subEntityTraits().setSelectionMarker(1);
				pWd->subEntityTraits().setColor(6);
				pWd->geometry().polyline(NJD,verts);
			}
			else
			{
				assert(Adesk::kFalse);//?
			}
			// �潻�㴦Բ
			for (i = 1; i < NJD-1; i++) 
			{
				Pt[X]=verts[i].x; Pt[Y]=verts[i].y;verts[i].z=0.0;
				pWd->subEntityTraits().setColor(6);	
				pWd->geometry().circle(Pt,3.6,normal);			
			}
		}

		// ������������
		for (i=0;i<NDL;i++)  
		{
			DLBTMP[i].BLC=DLB[i].BLC; 
			_tcscpy(DLBTMP[i].BGH,DLB[i].BGH); 
			DLBTMP[i].DL=DLB[i].DL;
			_tcscpy(DLBTMP[i].EGH,DLB[i].EGH); 
		}
		// ������������
		if(getpxy_DATA()<1)
			return 0;
		// �潻��ں�
		//		acutPrintf(L"JD0=%d,JDGH=%s\n",JD0,JDGH);
		xlpoint  PZtmp; 
		AcGePoint3d  ptb,pte;
		AcGePoint3d MidPt,StartPt,EndPt,APt1,APt2;//��·����
		double B,b;
		ACHAR str[80];
		PZtmp.lc=array_pxy[0][0][4]+xl_length/2.0;  // ͳһ���
		xlpoint_pz(array,tarray,&PZtmp);  
		B=PZtmp.a;
		pte[X]=PZtmp.y+xl_length*sin(B+pi*0.5)/4.0;
		pte[Y]=PZtmp.x+xl_length*cos(B+pi*0.5)/4.0;
		ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
		//////////////////////////////////////////////////////////////////////////
		if (IsdrawXLM)
		{
			//��·����
			MidPt.x=0.5*(ptb.x+pte.x);
			MidPt.y=0.5*(ptb.y+pte.y);
			MidPt.z=0.0;

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

			G_makeline(pWd,StartPt,EndPt,1);
			G_makeline(pWd,APt1,EndPt,1);
			G_makeline(pWd,APt2,EndPt,1);		
			//////////////////////////////////////////////////////////////////////////

			G_makeline(pWd,ptb,pte,1);
			Pt[Y]=pte[Y]+xl_length*cos(B)/20.0;
			Pt[X]=pte[X]+xl_length*sin(B)/20.0;
			ptb[Y]=pte[Y]+xl_length*cos(B+pi)/20.0;
			ptb[X]=pte[X]+xl_length*sin(B+pi)/20.0;
			G_makeline(pWd,Pt,ptb,1);
			pte[Y]=Pt[Y]+xl_length*cos(B+pi*0.5)/20.0;
			pte[X]=Pt[X]+xl_length*sin(B+pi*0.5)/20.0;
			G_makeline(pWd,Pt,pte,1);
			Pt[Y]=ptb[Y]+xl_length*cos(B+pi*0.5)/20.0;
			Pt[X]=ptb[X]+xl_length*sin(B+pi*0.5)/20.0;
			G_makeline(pWd,ptb,Pt,1);
			pte[Y]=Pt[Y]+xl_length*cos(B)/10.0;
			pte[X]=Pt[X]+xl_length*sin(B)/10.0;
			G_makeline(pWd,Pt,pte,1);
			CString name,Dir;
			name=mdbname;
			ReadWorkdir();
			Dir=Cworkdir;
			Dir.MakeUpper();
			Cworkdir=Dir;
			name.Replace(Cworkdir,NULL);
			name.Replace(L"\\DATA\\",NULL);
			name.Replace(L".MDB",L"����");
			_tcscpy(str,name);
			Pt[Y]=Pt[Y]+xl_length*cos(B-pi*0.5)/42.0;
			Pt[X]=Pt[X]+xl_length*sin(B-pi*0.5)/42.0;
			b=B+pi*0.5;
			b=pi*0.5-b; 
			if (b<0.0) b=2*pi+b;
			if (b>2*pi) b=b-2*pi;		         
			G_maketext(pWd,Pt,str,b+pi*0.5,xl_length/80.0,4,1);
		}


		//BZJD(pWd,JDarray,QXB,NJD,JD0,JDGH );	
		if (IsdrawJD)
		{
			BZJD(pWd,JDarray,QXB,NJD,JD0,JDGH );//����ŵ�������	
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
				G_WXY(pWd,INDATA,&PB,&PE ); //����·����
				PB=PE;
			}
		}
		LCB=array_pxy[0][0][4];  // ������

		//BZLC(pWd,100,draw_zybz,2 );    // ��ע ���ױ� ���
		//BZTZD(pWd, 100,0,2);  //  ��ע���������
		if (IsdrawLC)
		{
			BZLC(pWd,100,draw_zybz,2 );    // ��ע ���ױ� ���
		}
		if (IsdrawTZD)
		{
			BZTZD(pWd, 100,0,2);  //  ��ע���������
		}

		if (verts != NULL)
		{
			delete [] verts;
			verts = NULL;
		}
		//if(dockBar!=NULL && dockBar->flag==2)
		//{
		//	//	dockBar->SetFocus();
		//	//   PostMessage(dockBar->m_edjddlg->m_hWnd,CBN_SELCHANGE,1,1);
		//	dockBar->m_edjddlg->Invalidate();
		//}
		if(station_num > 0)
		{
			for(i = 0; i < station_num; i++)
			{
				INSERT_STATION(Station[i].CKml,Station[i].StaName,Station[i].TKSta,pWd);
			}
		}
		//���µ����� ת CM ����
		TrsBPDArraytoCM();
		if (IsdrawBPD)
		{
			BZCM(pWd);//��ע���µ�
		}

		CheckAllXLConstraint();
		DrawXLConstraint(pWd);//����·Լ��  (�ཻ��)

		if (isReFresh)
		{
			RefreshRelateXL();
		}
		isReFresh=true;
	}

	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}

Acad::ErrorStatus GTZX_ROAD::subExplode(AcDbVoidPtrArray& entitySet)  
{   // AcDbVoidPtrArray entset;
	AcDbObjectId entId;

	AcGePoint3dArray  pArr;
	int i=0,j=0,k=0,trow=0;
	double INDATA[5],LCB,arrayd[MAXJDNUM][6],qxb[MAXJDNUM][9];
	xlpoint  PB,PE; 
	ACHAR GCH[10];

	for (i=0;i<NDL;i++)  
	{  DLBTMP[i].BLC=DLB[i].BLC;   _tcscpy(DLBTMP[i].BGH,DLB[i].BGH); 
	DLBTMP[i].DL=DLB[i].DL;     _tcscpy(DLBTMP[i].EGH,DLB[i].EGH); };

	pArr.setLogicalLength(NJD);
	for (i = 0; i < NJD; i++) pArr[i].set(JDarray[i][2],JDarray[i][1],0.0);

	AcDb2dPolyline *pPline = new AcDb2dPolyline(AcDb::k2dSimplePoly,pArr,0.0, Adesk::kFalse);
	pPline->setColorIndex(6);
	entId=AddEntityToDbs(pPline);
	// entset.append(pPline);

	PB.x=array_pxy[0][0][1]; //N
	PB.y=array_pxy[0][0][2]; // E
	PB.a=rad(array_pxy[0][0][3]);// FA
	PB.lc=array_pxy[0][0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for (i =1; i < NJD; i++) 
	{    for (k =0; k <4; k++) 	
	{
		for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
		//G_WXY(pWd,INDATA,&PB,&PE ); 
		entId=GUXWXY(INDATA,&PB,&PE,1); 

		PB=PE;  };
	};
	LCB=array_pxy[0][0][4];  // ������

	/*    //  ���
	for (j=0;j<5;j++)array[0][j]=array_pxy[0][0][j]; 
	trow=1;
	for( i=1;i<NJD-1;i++) 
	{ for (k=0;k<4;k++) 	   { 
	for (j=0;j<5;j++)array[trow][j]=array_pxy[i][k][j]; 
	// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",array[trow][0],array[trow][1],array[trow][2],array[trow][4]);
	trow=trow+1;  	};  		   
	};
	//  �յ�
	for (j=0;j<5;j++)array[trow][j]=array_pxy[NJD-1][0][j]; 	   
	trow=trow+1;
	*/
	_tcscpy(GCH,DLB[0].EGH);
	//    BZLC1( array[400][10],tarray,GCH,NDL,NLC,texth,100.0, LCB,-1,2) ;
	//	BZTZD1(array[400][10],tarray,GCH, NDL,texth, LCB, draw_zybz ,2,NLC); 
	for (i =0; i < NJD; i++) 
	{    for (k =0; k <7; k++) arrayd[i][k]=JDarray[i][k]; 	};
	for (i =0; i < NJD; i++) 
	{    for (k =0; k <9; k++) qxb[i][k]=QXB[i][k]; 	};
	//     _tcscpy(JDGHTMP,JDGH);
	BZJD1(arrayd,qxb,NJD,0,texth,draw_zybz,qxbbz,NLC);

	// entitySet=entset;
	return Acad::eOk;

}


Acad::ErrorStatus GTZX_ROAD::subGetOsnapPoints(
	AcDb::OsnapMode   osnapMode,
	int               gsSelectionMark,
	const AcGePoint3d&    pickPoint,
	const AcGePoint3d&    lastPoint,
	const AcGeMatrix3d&   viewXform,
	AcGePoint3dArray& snapPoints,

	AcDbIntArray&     geomIds) const 
{    assertReadEnabled();
Acad::ErrorStatus es=Acad::eOk;
if (gsSelectionMark==0) return Acad::eOk;
if (osnapMode!=AcDb::kOsModeEnd &&osnapMode!=AcDb::kOsModeNear
	&& osnapMode != AcDb::kOsModeMid
	&&osnapMode!=AcDb::kOsModePerp &&osnapMode!=AcDb::kOsModeCen
	&&osnapMode!=AcDb::kOsModeIns  ) { return Acad::eOk;  };

AcGePoint3d Pt;

int i,j,k;
double l=0.0,INDATA[5] ;
xlpoint  PB,PE,PZ;   

switch (osnapMode) {
case AcDb::kOsModeEnd :
	// ����
	// ads_printf(L"\n NJD=%d",NJD);
	for ( i = 0; i < NJD; i++) 
	{  Pt.set(JDarray[i][2],JDarray[i][1],0.0);
	snapPoints.append(Pt);
	};

	//  ԰�����յ�
	PB.x=array_pxy[0][0][1]; //N
	PB.y=array_pxy[0][0][2]; // E
	PB.a=  rad(array_pxy[0][0][3]);// FA
	PB.lc=array_pxy[0][0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for (i =1; i < NJD; i++) 
	{    for (k =0; k <4; k++) 	
	{
		for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
		l=  Get_INDATA_L(INDATA,&j);
		xlzbxy(&PB,&PE,l,INDATA,0);// ĳ��Ԫ�յ�
		Pt[X]=PE.y;Pt[Y]=PE.x; Pt[Z]=0.0;
		snapPoints.append(Pt);
		/*
		xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
		Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
		if (j==2) // ԰�� �е�
		{ snapPoints.append(Pt);	};
		*/
		PB=PE;  
	};
	};
	//  ������
	for (i=1;i<NDL-1;i++)
	{	};
	break;

case  AcDb::kOsModeMid:

	PB.x=array_pxy[0][0][1]; //N
	PB.y=array_pxy[0][0][2]; // E
	PB.a=  rad(array_pxy[0][0][3]);// FA
	PB.lc=array_pxy[0][0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for (i =1; i < NJD; i++) 
	{    for (k =0; k <4; k++) 	
	{
		for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
		l=  Get_INDATA_L(INDATA,&j);
		xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
		Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
		if (j==2) // ԰�� �е�
		{ snapPoints.append(Pt);	};
		xlzbxy(&PB,&PE,l,INDATA,0);// ĳ��Ԫ�յ�
		PB=PE;  
	};
	};
	break;

default :       return Acad::eOk;

}

return Acad::eOk;
}


// Transform points and create a polyline out of them.
//
Acad::ErrorStatus  GTZX_ROAD::subGetTransformedCopy(
	const AcGeMatrix3d& mat,
	AcDbEntity*& ent) const
{
	assertReadEnabled();
	Acad::ErrorStatus es = Acad::eOk;

	/*
	AcGePoint3dArray vertexArray;
	if ((es = getVertices3d(vertexArray))
	!= Acad::eOk)
	{
	return es;
	}

	for (int i = 0; i < vertexArray.length(); i++) {
	vertexArray[i].transformBy(mat);
	}

	AcDbSpline *pSpline = NULL;
	if ((es = rx_makeSpline(vertexArray, pSpline))
	!= Acad::eOk)
	{
	return es;
	}
	assert(pSpline != NULL);
	pSpline->setPropertiesFrom(this);

	ent = pSpline;
	*/
	return es;
}

// Files data in from a DWG file.
//
Acad::ErrorStatus GTZX_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();
	int j,k;
	double dd;
	ACHAR  *chr=NULL;
	ACHAR *temp=NULL;
	AcDbObject::dwgInFields(pFiler);
	// For wblock filing we wrote out our owner as a hard
	// pointer ID so now we need to read it in to keep things
	// in sync.
	//

	if (pFiler->filerType() == AcDb::kWblockCloneFiler) {
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}

	pFiler->readItem(&NJD);
	pFiler->readItem(&NDL);
	pFiler->readItem(&NLC);
	pFiler->readItem(&NH);

	pFiler->readItem(&Chinese_Engbz_type );
	pFiler->readItem(&draw_zybz); 
	pFiler->readItem(&JDTDBZ );
	pFiler->readItem(&qxbbz);
	pFiler->readItem(&JD0); 
	pFiler->readItem(&pro); 
	pFiler->readItem(&Zx[0]); 
	pFiler->readItem(&Zx[1]); 
	pFiler->readItem(&texth);

	for (j=0;j<NJD;j++)
	{
		for (k=0;k<=6;k++)  
			pFiler->readItem( &JDarray[j][k]);
	}

	for (j=0;j<NDL;j++)
	{    
		pFiler->readItem(&dd);
		DLB[j].BLC=dd;
		pFiler->readItem(&dd );
		DLB[j].DL=dd;   
	}

	for (j=0;j<NDL;j++)
	{   
		pFiler->readItem(&chr);
		_tcscpy(DLB[j].BGH,chr);  
		pFiler->readItem(&chr);
		_tcscpy(DLB[j].EGH,chr);
	}
	pFiler->readItem(&chr); 
	mdbname = chr ;
	pFiler->readItem(&chr);
	_tcscpy(m_XLRelate.CurXLNme,chr);
	pFiler->readItem(&chr);
	_tcscpy(m_XLRelate.InXLName,chr);
	pFiler->readItem(&m_XLRelate.SXLDCnum);
	pFiler->readItem(&chr);
	_tcscpy(m_XLRelate.OutXLName,chr);
	pFiler->readItem(&m_XLRelate.EXLDCnum);
	pFiler->readItem(&m_XLRelate.InterXLSum);
	for (int i=0;i<m_XLRelate.InterXLSum;i++)
	{
		pFiler->readItem(&m_XLRelate.InterXLNameArray[i].GCmin);
		pFiler->readItem(&m_XLRelate.InterXLNameArray[i].JJlowLimt);
		pFiler->readItem(&chr);
		_tcscpy(m_XLRelate.InterXLNameArray[i].InterXLName,chr);
	}

	pFiler->readItem(&m_NCM);//��CM����鵵
	for (  j=0;j<m_NCM;j++)
	{
		pFiler->readItem( &m_CM[j*5]);
		pFiler->readItem( &m_CM[j*5+1]);
		pFiler->readItem( &m_CM[j*5+2]);
		pFiler->readItem( &m_CM[j*5+3]);
		pFiler->readItem( &m_CM[j*5+4]);
	}
	pFiler->readItem(&IsdrawTZD);
	pFiler->readItem(&IsdrawLC);
	pFiler->readItem(&IsdrawXLM);
	pFiler->readItem(&IsdrawBPD);
	pFiler->readItem(&IsdrawJDLX);
	pFiler->readItem(&IsdrawJD);
	//pFiler->readItem(&m_NeedInterXLNum);
	//for (int i=0; i<m_NeedInterXLNum; i++)
	//{
	//	pFiler->readItem(&chr);
	//	_tcscpy(m_NeedInterXLNameArray[i], chr);
	//}

	int nBPD;
	pFiler->readItem(&nBPD);
	m_BPDArray.RemoveAll();
	m_BPDArray.SetSize(nBPD);
	for (j=0;j<nBPD;j++)
	{
		pFiler->readItem(&chr);
		_tcscpy(m_BPDArray[j].CKml,chr);
		pFiler->readItem(&m_BPDArray[j].cml);
		pFiler->readItem(&m_BPDArray[j].degree);
		pFiler->readItem(&chr);
		m_BPDArray[j].GH.Format(L"%s",chr);
		pFiler->readItem(&m_BPDArray[j].length);
		pFiler->readItem(&m_BPDArray[j].Level);
		pFiler->readItem(&m_BPDArray[j].ml);
		pFiler->readItem(&chr);
		m_BPDArray[j].Notes.Format(L"%s",chr);
		pFiler->readItem(&m_BPDArray[j].RLen);
		pFiler->readItem(&m_BPDArray[j].Rshu);
	}

	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus GTZX_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	int j,k;
	double dd;
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());

	pFiler->writeItem(NJD);
	pFiler->writeItem(NDL);
	pFiler->writeItem(NLC);
	pFiler->writeItem(NH);
	pFiler->writeItem(Chinese_Engbz_type );
	pFiler->writeItem(draw_zybz);
	pFiler->writeItem(JDTDBZ); 
	pFiler->writeItem(qxbbz); 
	pFiler->writeItem(JD0);
	pFiler->writeItem(pro);
	pFiler->writeItem(Zx[0]);
	pFiler->writeItem(Zx[1]);
	pFiler->writeItem(texth);

	for (j=0;j<NJD;j++)
	{
		for (k=0;k<=6;k++)  
			pFiler->writeItem( JDarray[j][k]);
	}

	//ads_printf(L"dwg WW NJD=%d,NDL=%d NLC=%d NH=%d Chinese_Engbz_type=%d",NJD,NDL,NLC,NH,Chinese_Engbz_type); 

	for (j=0;j<NDL;j++)
	{     
		dd=DLB[j].BLC;
		pFiler->writeItem( dd);
		dd=DLB[j].DL;
		pFiler->writeItem( dd);  
	}

	const ACHAR *chr=NULL;
	for (j=0;j<NDL;j++)
	{   
		chr=_tcsdup(DLB[j].BGH );
		pFiler->writeItem(chr );
		chr=_tcsdup(DLB[j].EGH );
		pFiler->writeItem(chr );	 
	}
	pFiler->writeItem(mdbname);
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
	//pFiler->writeItem(m_NeedInterXLNum);
	//for (int i=0;i<m_NeedInterXLNum;i++)
	//{
	//	pFiler->writeItem(m_NeedInterXLNameArray[i]);
	//}

	pFiler->writeItem(m_NCM);//��CM����鵵
	for (  j=0;j<m_NCM;j++)
	{
		pFiler->writeItem( m_CM[j*5]);
		pFiler->writeItem( m_CM[j*5+1]);
		pFiler->writeItem( m_CM[j*5+2]);
		pFiler->writeItem( m_CM[j*5+3]);
		pFiler->writeItem( m_CM[j*5+4]);
	}
	pFiler->writeItem(IsdrawTZD);
	pFiler->writeItem(IsdrawLC);
	pFiler->writeItem(IsdrawXLM);
	pFiler->writeItem(IsdrawBPD);
	pFiler->writeItem(IsdrawJDLX);
	pFiler->writeItem(IsdrawJD);

	int nBPD=m_BPDArray.GetSize();
	pFiler->writeItem(nBPD);
	for (j=0;j<nBPD;j++)
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



// Files data in from a DXF file.
//

Acad::ErrorStatus GTZX_ROAD::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	const ACHAR *chr=NULL;
	Acad::ErrorStatus es;
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }

	// Check if we're at the right subclass getData marker.

	if (!pFiler->atSubclassData(L"GTZX")) {
		ads_printf(L"\n error 10010 �����д� \n");
		return Acad::eBadDxfSequence;
	}
	struct resbuf inbuf;
	ACHAR *m_modal=NULL;	

	int k,j,kj;
	k= 0 ;
	j = 0;
	kj=1  ;

	/*
	m_modal=NULL;
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
	if ( inbuf.restype == AcDb:: kDxfText)
	{
	_tcscpy(m_modal,inbuf.resval.rstring);
	}
	else		{		}
	}
	*/

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16 )
		{	NJD = inbuf.resval.rint;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+1 )
		{	NDL = inbuf.resval.rint;		}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+2 )
		{	NLC = inbuf.resval.rint;		}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+3 )
		{
			NH = inbuf.resval.rint;
		}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+4 )
		{
			Chinese_Engbz_type = inbuf.resval.rint;
		}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+5)
		{
			draw_zybz  = inbuf.resval.rint;
		}
		else		{		}
	} 
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+6)
		{
			JDTDBZ  = inbuf.resval.rint;
		}
		else		{		}
	} 
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+7)
		{
			qxbbz  = inbuf.resval.rint;
		}
		else		{		}
	} 

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+8)
		{
			JD0  = inbuf.resval.rint;
		}
		else		{		}
	} 

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+9)
		{
			pro  = inbuf.resval.rint;
		}
		else		{		}
	} 

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+10)
		{
			Zx[0]  = inbuf.resval.rint;
		}
		else		{		}
	} 

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+11)
		{
			Zx[1]  = inbuf.resval.rint;
		}
		else		{		}
	} 

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal )
		{
			texth = inbuf.resval.rreal;
		}
		else		{		}
	}

	for (j=0;j<NJD;j++)
	{  
		for (k=0;k<=6;k++)  
		{
			es = pFiler->readItem(&inbuf);
			if  ( es == Acad::eOk)
			{
				if ( inbuf.restype == AcDb:: kDxfReal+1 )
				{
					JDarray[j][k] = inbuf.resval.rreal;
				}
				else		   {		   }
			}
		};
	};
	// acutPrintf(L"in %d,%d,%d,%d,%d,%d,%d,%d,%d\n",NJD,NDL,NLC,NH,Chinese_Engbz_type,draw_zybz,JDTDBZ,qxbbz,JD0);
	//	ads_printf(L"DXF RR NJD=%d,NDL=%d NLC=%d NH=%d Chinese_Engbz_type=%d",NJD,NDL,NLC,NH,Chinese_Engbz_type); 

	for (j=0;j<NDL;j++)
	{    
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{   DLB[j].BLC  = inbuf.resval.rreal;	}
			else		   {		   } 
		};
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{   DLB[j].DL  = inbuf.resval.rreal;	}
			else		   {		   } 
		};
		//			 ads_printf(L"j=%d   BLC=%10.4f ELC=%10.4f DL=%10.4f\n",j, DLB[j].BLC, DLB[j].BLC+DLB[j].DL,DLB[j].DL); 


	};

	for (j=0;j<NDL;j++)
	{    
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype ==AcDb::kDxfText )
			{   _tcscpy(DLB[j].BGH,inbuf.resval.rstring);}
			else		   {		   } 
		};

		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype ==AcDb::kDxfText )
			{ _tcscpy(DLB[j].EGH,inbuf.resval.rstring);}
			else		   {		   } 
		}; 
		//			 ads_printf(L"j=%d   BLC=%10.4f ELC=%10.4f DL=%10.4f\n",j, DLB[j].BLC, DLB[j].BLC+DLB[j].DL,DLB[j].DL); 

	};

	return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus GTZX_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	double dd;
	int j,k=0;
	const ACHAR* chr=NULL;
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass, L"GTZX");
	//	pFiler->writeItem(AcDb:: kDxfText,m_modal);
	pFiler->writeItem(AcDb::kDxfInt16,NJD);
	pFiler->writeItem(AcDb::kDxfInt16+1,NDL);
	pFiler->writeItem(AcDb::kDxfInt16+2,NLC);
	pFiler->writeItem(AcDb::kDxfInt16+3,NH);
	pFiler->writeItem(AcDb::kDxfInt16+4,Chinese_Engbz_type );
	pFiler->writeItem(AcDb::kDxfInt16+5,draw_zybz);
	pFiler->writeItem(AcDb::kDxfInt16+6,JDTDBZ);
	pFiler->writeItem(AcDb::kDxfInt16+7,qxbbz);
	pFiler->writeItem(AcDb::kDxfInt16+8,JD0);
	pFiler->writeItem(AcDb::kDxfInt16+9,pro);
	pFiler->writeItem(AcDb::kDxfInt16+10,Zx[0]);
	pFiler->writeItem(AcDb::kDxfInt16+11,Zx[1]);
	//	acutPrintf(L"out %d,%d,%d,%d,%d,%d,%d,%d,%d\n",NJD,NDL,NLC,NH,Chinese_Engbz_type,draw_zybz,JDTDBZ,qxbbz,JD0);

	pFiler->writeItem(AcDb::kDxfReal,texth);

	for (j=0;j<NJD;j++)
	{
		for (k=0;k<=6;k++) 	
		{
			pFiler->writeItem(AcDb::kDxfReal+1,JDarray[j][k]);
			//	    ads_printf(L"k=%d j=%d %d %lf\n",j,k,k+5*j+1,JDarray[j][k]);	
		};
	};

	ads_printf(L"dxf WW NJD=%d,NDL=%d NLC=%d NH=%d Chinese_Engbz_type=%d",NJD,NDL,NLC,NH,Chinese_Engbz_type); 

	for (j=0;j<NDL;j++)
	{    
		dd=DLB[j].BLC;	pFiler->writeItem(AcDb::kDxfReal+1,dd);
		dd=DLB[j].DL;   pFiler->writeItem(AcDb::kDxfReal+1,dd);   
		//		ads_printf(L"j=%d   BLC=%10.4f ELC=%10.4f DL=%10.4f\n",j, DLB[j].BLC, DLB[j].BLC+DLB[j].DL,DLB[j].DL); 
	};

	for (j=0;j<NDL;j++)
	{      
		chr= _tcsdup(DLB[j].BGH );pFiler->writeItem(AcDb::kDxfText, chr);
		chr= _tcsdup(DLB[j].EGH );  pFiler->writeItem(AcDb::kDxfText, chr);	 
		//		 ads_printf(L"j=%d   BGH=%s Egh=%s\n",j, DLB[j].BGH, DLB[j].EGH); 
	};

	return pFiler->filerStatus();
}


Acad::ErrorStatus GTZX_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											  AcDbIntArray& osnapModes,
											  AcDbIntArray& geomIds)const
{
	return Acad::eOk;	//20191108	���Ե�����GTZXʵ���϶�ʱ�ᵼ��������Ŀǰ��ʱ�رոù���

	AcGePoint3d Pt;
	int i,j,k;
	double l=0.0,INDATA[5],X1,Y1;
	xlpoint  PB,PE,PZ;   

	assertReadEnabled();
	// ���������е㼯�����0����NJD-1
	// ads_printf(L"\n NJD=%d",NJD);
	for (i = 0; i < NJD; i++) 
	{  Pt.set(JDarray[i][2],JDarray[i][1],0.0);
	gripPoints.append(Pt);
	};
	// �����߱ߵ��е����е㼯����ţ�NJD-1��+1������NJD-1��+NJD-1
	// ads_printf(L"\n NJD=%d",NJD);
	for ( i = 0; i < NJD-1; i++) 
	{  X1=(JDarray[i][2]+JDarray[i+1][2])*0.5;
	Y1=(JDarray[i][1]+JDarray[i+1][1])*0.5;
	Pt.set(X1,Y1,0.0);
	gripPoints.append(Pt);
	};
	// ���������Ӧ��Բ���е����е㼯����ţ�NJD-1��+��NJD-1��+1������NJD-1)+��NJD-1��+NJD-2
	//  ԰�����յ�
	PB.x=array_pxy[0][0][1]; //N
	PB.y=array_pxy[0][0][2]; // E
	PB.a=rad(array_pxy[0][0][3]);// FA
	PB.lc=array_pxy[0][0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for (i =1; i < NJD; i++) 
	{    for (k =0; k <4; k++) 	
	{
		for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
		l=Get_INDATA_L(INDATA,&j);
		xlzbxy(&PB,&PE,l,INDATA,0);// ĳ��Ԫ�յ�
		xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
		Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
		if (j==2) // ԰�� �е�
		{ gripPoints.append(Pt);	};
		PB=PE;  
	};
	};

	/*
	// ���������Ӧ�Ļ��������е����е㼯��
	// ���
	// ԰�����յ�
	PB.x=array_pxy[0][0][1]; //N
	PB.y=array_pxy[0][0][2]; // E
	PB.a=rad(array_pxy[0][0][3]);// FA
	PB.lc=array_pxy[0][0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	for (i =1; i < NJD; i++) 
	{    for (k =0; k <4; k++) 	
	{
	for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
	l=Get_INDATA_L(INDATA,&j);
	xlzbxy(&PB,&PE,l,INDATA,0);// ĳ��Ԫ�յ�
	xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // ĳ��Ԫ�е�
	Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
	if (j==3||j==4) // �������� �е�
	{ gripPoints.append(Pt);	};
	PB=PE;  
	};
	};
	//  ������
	for (i=1;i<NDL-1;i++)
	{	};
	*/
	return Acad::eOk;
}

Acad::ErrorStatus GTZX_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset)
{
	AcGePoint3d startPt,endPt,centerPt;
	assertWriteEnabled();
	int  j,k,bz ;
	double R, dd,DD01,DD02,DD1,DD2,fb,d1,d2,fb1,fb2, dx,dy;
	//	JDTDBZ=4;

	int RN=1;
	if( (RN<1)||(RN>2)) RN=1;

	for ( j=0;j<indices.length();j++)
	{
		// ������� 0~NJD-1
		if  (indices[j]<NJD) 
		{   // ��ѡ��ļе�Ϊ ĳһ���㣨���������յ㣩
			bz=1;
			k=indices[j];  //�������
			dx=offset[X];dy=offset[Y];

			if (JDTDBZ==1)  //�������϶�
			{
				if (k<=NJD-2)
				{ 
					DD02 =xyddaa(JDarray[k][1],JDarray[k][2], JDarray[k+1][1],JDarray[k+1][2],&fb2);
					DD2=xyddaa(JDarray[k][1]+dy,JDarray[k][2]+dx, JDarray[k+1][1],JDarray[k+1][2],&fb);
					//  ��ֱ�߶�(�������)[(XB,YB),(XE,YE)]�ĳ��Ⱥͷ�λ��  ����:���룬 *��λ�� 
					d2=DD02-DD2;
				} 
				else {  d2=0.0; fb2=0.0; }
				if (k>0)
				{
					DD01 =xyddaa(JDarray[k-1][1],JDarray[k-1][2], JDarray[k ][1],JDarray[k][2],&fb1);
					DD1=xyddaa(JDarray[k-1][1],JDarray[k-1][2],JDarray[k][1]+dy,JDarray[k][2]+dx,&fb);
					//  ��ֱ�߶�(�������)[(XB,YB),(XE,YE)]�ĳ��Ⱥͷ�λ��  ����:���룬 *��λ�� 
					d1=DD1-DD01;
				}else {  d1=0.0; fb1=0.0; }
				dd=d1; fb=fb1;
				if (fabs(d1)<fabs(d2)) { dd=d2; fb=fb2;};
				//E
				JDarray[k][2]=JDarray[k][2]+dd*sin(fb);
				//N
				JDarray[k][1]=JDarray[k][1]+dd*cos(fb);
				//	ads_printf(L"  DT=%-10.4f",dd);
			}
			else if(JDTDBZ==2 || JDTDBZ==3 || JDTDBZ==4)
			{
				double OutN,OutE;
				OutN=JDarray[k][1]+offset[Y];
				OutE=JDarray[k][2]+offset[X];

				if (JDTDBZ==2 || JDTDBZ==4)  // ��ǰԲ
				{
					if(k>1)//�������
					{
						double Lo1,Lo2,R1,p1,tR1,A2;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

						Lo1=JDarray[k-1][3];
						Lo2=JDarray[k-1][5];
						R1=JDarray[k-1][4];
						if(Lo2>0.0)
						{
							A2=sqrt(R1*Lo2);
							double x=spiral_y(A2,Lo2);
							double t=Lo2/(2.0*R1);
							double dr=x+R1*sin(t+0.5*PI);			
							p1=dr-R1;
						}
						else p1=0.0;					
						//�뾶+������					
						tR1=R1+p1;
						//��ǰԲԲ��
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						N1=JDarray[k-2][1];
						E1=JDarray[k-2][2];
						N0=JDarray[k-1][1];
						E0=JDarray[k-1][2];
						N2=JDarray[k][1];
						E2=JDarray[k][2];
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R1,Lo1,Lo2,CenN,CenE);
						double TN,TE;//�е�
						//���е�
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR1,OutN,OutE,LRFlag,TN,TE);
						BAS_DRAW_FUN_RAIL::INTERS(&JDarray[k-1][1],&JDarray[k-1][2],N1,E1,N0,E0,TN,TE,OutN,OutE);
					}				
				}
				if (JDTDBZ==3 || JDTDBZ==4)  // �к�Բ
				{
					if(k<NJD-2)//�����յ�
					{
						double Lo1,Lo2,R2,p2,tR2,A1;//ǰ����󻺣�ǰԲ�뾶,������,ǰԲ���������Ĵ�Բ�뾶

						Lo1=JDarray[k+1][3];
						Lo2=JDarray[k+1][5];
						R2=JDarray[k+1][4];
						if(Lo1>0.0)
						{
							A1=sqrt(R2*Lo1);
							double x=spiral_y(A1,Lo1);
							double t=Lo1/(2.0*R2);
							double dr=x+R2*sin(t+0.5*PI);			
							p2=dr-R2;
						}
						else p2=0.0;					
						//�뾶+������					
						tR2=R2+p2;
						//��ǰԲԲ��
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						N1=JDarray[k][1];
						E1=JDarray[k][2];
						N0=JDarray[k+1][1];
						E0=JDarray[k+1][2];
						N2=JDarray[k+2][1];
						E2=JDarray[k+2][2];
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R2,Lo1,Lo2,CenN,CenE);
						//acutPrintf(L"CenE:%lf,%lf\n",CenE,CenN);
						double TN,TE;//�е�
						//���е�
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR2,OutN,OutE,-1*LRFlag,TN,TE);
						BAS_DRAW_FUN_RAIL::INTERS(&JDarray[k+1][1],&JDarray[k+1][2],N2,E2,N0,E0,TN,TE,OutN,OutE);
					}				
				}
				//(JDTDBZ==4)// ��ǰ������Բ
				//��ǰ������λ��
				JDarray[k][2]+=offset[X];//x					
				JDarray[k][1]+=offset[Y];//y
			}
			else   //�����϶�
			{  //x
				JDarray[k][2]=JDarray[k][2]+offset[X];
				//y
				JDarray[k][1]=JDarray[k][1]+offset[Y];
			}
		}
		else if ((indices[j]>=NJD)&&(indices[j]<2*NJD-2))  // ��ѡ��ļе�Ϊ  ĳһ���ߵ��е�
		{   // (NJD-1)+��NJD-2��+1--(NJD-1)+��NJD-2��+NJD-1
			bz=2;
			k=indices[j]-NJD+1; // �����߱߶�Ӧ�Ľ����(��㣩
			dd=sqrt(offset[X]*offset[X]+offset[Y]*offset[Y]);					   

			if(k<=NJD-2||k>0)
			{
				DD01 =xyddaa(JDarray[k-1][1],JDarray[k-1][2], JDarray[k ][1],JDarray[k][2],&fb1);

				DD02 =xyddaa(JDarray[k][1],JDarray[k][2], JDarray[k+1][1],JDarray[k+1][2],&fb2);
				fb=fb2;	
				JDarray[k][2]=JDarray[k][2]+dd*sin(fb);
				//N
				JDarray[k][1]=JDarray[k][1]+dd*cos(fb);

				JDarray[k-1][2]=JDarray[k-1][2]-dd*sin(fb1);
				//N
				JDarray[k-1][1]=JDarray[k-1][1]-dd*cos(fb1);	    
				//s_printf("  DT=%-10.4f",dd);
			};
			//	ads_printf(L"\n ����=%d, X=%10.2f Y=%10.2f  dd=%10.2f",k,offset[X],offset[Y],dd);
		}
		else if ((indices[j]>=2*NJD-1)&&(indices[j]<=3*NJD-4))  // ��ѡ��ļе�Ϊ  ĳһ԰�����е�
		{   // (NJD-1)+1--(NJD-1)+NJD-1
			bz=2;
			k=indices[j]-2*NJD+2; // ��԰����Ӧ�Ľ����
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
				calptan(JDarray[k-1][1],JDarray[k-1][2],JDarray[k][1],JDarray[k][2],JDarray[k+1][1],JDarray[k+1][2],xt,yt,&tmp,&R,&tmp,&tmp,&tmp,&tmp,&tmp);
				R = get_R(R,DV,mGuiFanCS);
				JDarray[k][4] = R;
				JDarray[k][3] = get_l0(R,DV,mGuiFanCS);
				JDarray[k][5] = get_l0(R,DV,mGuiFanCS);

				double fb,fe;
				double ddb=xyddaa(JDarray[k-1][1],JDarray[k-1][2],JDarray[k][1],JDarray[k][2],&fb);
				double dde=xyddaa(JDarray[k][1],JDarray[k][2],JDarray[k+1][1],JDarray[k+1][2],&fe);
				double aa=fe-fb;
				if (aa<-pi) aa=aa+2*pi;
				if  (aa>pi) aa=aa-2*pi;
				aa=fabs(aa);
				aa=dms_rad(aa);
				double BPMTL[10];
				getBPMTL(aa,JDarray[k][4],JDarray[k][3],JDarray[k][5],&BPMTL[0]);
				double T1=BPMTL[6];
				double T2=BPMTL[7];//L=BPMTL[8];

				double Lmin=mGuiFanCS.MinZhi;
				if(k==1)
				{
					if(T1>ddb)
					{
						JDarray[k-1][1]=JDarray[k][1]-(T1+Lmin)*cos(fb);
						JDarray[k-1][2]=JDarray[k][2]-(T1+Lmin)*sin(fb);
					}
				}
				if(k==NJD-2)
				{
					if(T2>dde)
					{
						JDarray[k+1][1]=JDarray[k][1]+(T2+Lmin)*cos(fe);
						JDarray[k+1][2]=JDarray[k][2]+(T2+Lmin)*sin(fe);
					}
				}
			}
		}
		//�յ�
		if(getpxy_DATA()<1)
			return Acad::eOk;
		// 		if(IfViewDmx)
		// 		{
		// 			if(k<1) k=1;
		// 			if(k>NJD-2) k=NJD-2;
		// 			if(k>0)
		// 			{
		// 				VIEW_PM_ZDM(k);
		// 		        VIEW_PM_PQX(k);
		// 			}
		// 			
		// 		}
		// 
		// 		if(IfViewTypeHdm)
		// 			VIEW_PM_TypeHdm();
	};
	return Acad::eOk;
}

void GTZX_ROAD::subList() const 
{ int i,j;
double dd;
assertReadEnabled();
ads_printf(L"\n �ࣺ\t%s",isA()->name());


for ( i = 0; i < NJD; i++) 
{ 	ads_printf(L"\n N%10.4f E%10.4f L1=%10.2f R=%10.2f L2=%10.2f", JDarray[i][1],JDarray[i][2],JDarray[i][3],JDarray[i][4],JDarray[i][5]);  };
// ���߱�
ads_printf(L"\n  ������QD=%-s%10.4f",DLB[0].EGH,QXB[0][0]);
for ( i = 1; i < NJD-1; i++) 
{   j=(int) (QXB[i][4]+0.00001)	;
ads_printf(L"\n  ZH=%-s%10.4f  HY=%10.4f YH=%10.4f HZ=%10.4f", DLB[0].EGH,QXB[i][0],QXB[i][1],QXB[i][2],QXB[i][3]);  
};
ads_printf(L"\n  �յ����ZD=%-s%10.4f",DLB[0].BGH,QXB[NJD-1][0]);
//������(�����յ�)
for ( i =0; i < NDL ; i++) 
{  dd=DLB[i].BLC+DLB[i].DL;
ads_printf(L"\n  I=%d ��ǰ���BLC=%-s%10.4f �Ϻ����ELC=%-s%10.4f ����=%10.4f ", 
		   i,DLB[i].BGH,DLB[i].BLC,DLB[i].EGH,dd,DLB[i].DL);  
};
}

void  GTZX_ROAD::setINIT(int nlc,int nh,int Chinese_Engbz,double Texth)
{
	assertWriteEnabled();
	NLC=nlc;
	NH=nh;
	texth=Texth;
	Chinese_Engbz_type=Chinese_Engbz;

}

void  GTZX_ROAD::setTD(short int JDTDBZ0, short int draw_zybz0, short int qxbbz0 ) 
	// JDTDBZ �����϶���־ ��JDTDBZ==1Ϊ�������϶�  =2 Ϊ��Բ =��������Ϊ�����϶�
{
	assertWriteEnabled();
	ads_printf(L"\n JDTDBZ=%d draw_zybz=%d qxbbz=%d",JDTDBZ,draw_zybz,qxbbz);
	JDTDBZ=JDTDBZ0;
	draw_zybz= draw_zybz0;
	qxbbz=qxbbz0;
	ads_printf(L"\n JDTDBZ=%d draw_zybz=%d qxbbz=%d",JDTDBZ,draw_zybz,qxbbz); 
}
//  ���ö�������
Adesk::Boolean  GTZX_ROAD::setDLM(int iDL,double BLC,double DL, ACHAR *BGH,ACHAR *EGH, int mode )
{   
	if (mode !=1 )
		assertWriteEnabled();
	if (iDL>=0&& iDL<NDL)
	{ 	
		_tcscpy(DLB[iDL].BGH,BGH);
		_tcscpy(DLB[iDL].EGH,EGH);
		DLB[iDL].BLC=BLC;
		DLB[iDL].DL= DL;
		//���
		if (iDL==0)
			JDarray[0][3]=BLC+DL;
		return 1;
	}
	else
		return 0;
}

//  ���ö�������
Adesk::Boolean  GTZX_ROAD::setDLM(int iDL,double BLC,double DL, CString BGH, CString EGH, int mode )
{
	ACHAR BGHStr[8],EGHStr[8];
	_tcscpy(BGHStr, BGH);
	_tcscpy(EGHStr, EGH);
	return setDLM(iDL, BLC, DL, BGHStr, EGHStr, mode );
}

//  ���һ����������
Adesk::Boolean  GTZX_ROAD::getDLM(int iDL,double *BLC,double *DL, ACHAR *BGH ,ACHAR *EGH )
{   

	if (iDL>=0 && iDL< NDL && NDL<30)  // NDL����СֵΪ2  ���ֵΪ30
	{  	       
		_tcscpy(EGH,DLB[iDL].BGH );
		_tcscpy(EGH,DLB[iDL].EGH );
		*BLC=DLB[iDL].BLC;
		*DL=DLB[iDL].DL;

	}   
	return NDL;
}
//  ����һ����������
Adesk::Boolean  GTZX_ROAD::addDLM(int iDL,double BLC,double DL, ACHAR *EGH  )
	//							 ���ж����ţ���ǰ��̣�����ֵ
{   
	int i;
	if (iDL>0 && iDL< NDL && NDL<30)  // NDL����СֵΪ2  ���ֵΪ30
	{  	
		if( (BLC>( DLB[iDL-1].BLC+DLB[iDL-1].DL)) && (BLC< DLB[iDL].BLC)) 
		{
			assertWriteEnabled();
			for (i=iDL;i<=NDL-1;i++) DLB[NDL+iDL-i]=DLB[NDL-1+iDL-i] ; // ����һ��������
			NDL=NDL+1;

			_tcscpy(DLB[iDL].EGH,EGH);   
			_tcscpy(DLB[iDL+1].BGH,EGH);
			DLB[iDL].BLC=BLC;        	
			DLB[iDL].DL= DL;

			DLB[iDL+1].BLC += DL;
			DLB[iDL+1].DL  -= DL;
			//ads_printf(L"%lf %lf\n",DLB[iDL+1].BLC , DLB[iDL+1].DL);

			for(i=1; i<NDL-1; i++)//�Գ������⴦��  DK10+105.43=D1K9+000  D1K10+105.43=DK10+105.43 
			{		                               //��ΪD1K10+105.44=DK10+105.44
				if(_tcscmp(DLB[i].BGH,DLB[i-1].EGH)!=0)//��һ����BGHӦ��ǰһ����EGHһ��
					_tcscpy(DLB[i].BGH,DLB[i-1].EGH);
				if(fabs(DLB[i].DL)<0.0001)//��ǰ����Ϊ0
				{
					if(DLB[i-1].DL < -0.01  //ǰһ����Ϊ����
						&&fabs(DLB[i-1].BLC-DLB[i].BLC)<0.001)
						DLB[i].BLC +=0.01;
					if(DLB[i+1].DL < -0.01  //��һ����Ϊ����
						&&fabs(DLB[i].BLC-(DLB[i+1].BLC+DLB[i+1].DL))<0.001)
						DLB[i].BLC -=0.01;
				}
			}


			//�޸��յ㣨δ���������
			//		   DLB[NDL-1].BLC= DLB[NDL-1].BLC+DL;

			return 1;  	}
		else return 0;
	} else return 0;
}


//  ɾ��һ����������
Adesk::Boolean  GTZX_ROAD::DelDLM(int iDL)
{   
	int i;
	double DL;
	if (iDL>0 && iDL< NDL && NDL<30)  // NDL����СֵΪ2  ���ֵΪ30
	{      DL=DLB[iDL].DL;
	assertWriteEnabled();
	for (i=iDL;i<=NDL-1;i++) DLB[iDL+i-iDL]=DLB[iDL+1+i-iDL] ; // ǰ��һ��������
	NDL=NDL-1;
	_tcscpy(DLB[iDL].BGH,DLB[iDL-1].EGH);

	// DLB[iDL].DL= DLB[iDL].DL+DL ;
	DLB[iDL].BLC= DLB[iDL].BLC-DL;
	DLB[iDL].DL = DLB[iDL].DL+DL;
	//�޸��յ㣨δ���������
	// DLB[NDL-1].BLC= DLB[NDL-1].BLC-DL;
	return 1;   

	} else return 0;
}

int GTZX_ROAD::savePM(ACHAR *fname)//2003.10.18�� ����Ͷ�����һ
{   
	assertReadEnabled();////
	int k,trow=0;
	FILE *fpc;
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"w"))!=NULL)
	{
		//save jd
		fwprintf(fpc,L"%d %s\n",NJD,JDGH);
		fwprintf(fpc,L"%d ",(int)(JDarray[trow][0]+0.001));  
		for (k=1;k<=5;k++) fwprintf(fpc,L" %lf", JDarray[trow][k]);
		fwprintf(fpc,L"\n");
		trow=trow+1;
		do
		{
			fwprintf(fpc,L"%d  ",(int)(JDarray[trow][0]+0.001));  
			for (k=1;k<=5;k++)  fwprintf(fpc,L"  %15.6f",JDarray[trow][k]);
			fwprintf(fpc,L"\n");
			trow++;

		} while(trow<NJD);
		//save DL
		fwprintf(fpc,L"%d\n",NDL);
		for(int i=0;i<NDL;i++)
		{
			//			double BLC,DL;  //  �����Ķ�ǰ��̺Ͷ���ֵ(+ֵΪ����;-Ϊ����)
			//			char   BGH[8],EGH[8]; //  �����Ķ�ǰ��̹ںźͶϺ���̹ں�

			fwprintf(fpc,L"%8s %8s %15.6lf %15.6lf\n",DLB[i].BGH,DLB[i].EGH,DLB[i].BLC,DLB[i].DL);			
		}
		fclose(fpc);
	};
	return 1;
}

int GTZX_ROAD::saveDL(ACHAR *fname)
{   
	assertReadEnabled();////
	int k,trow=0;
	FILE *fpc;
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"w"))!=NULL)
	{
		//save jd
		fwprintf(fpc,L"%d %s\n",NJD,JDGH);
		fwprintf(fpc,L"%d ",(int)(JDarray[trow][0]+0.001));  
		for (k=1;k<=5;k++) fwprintf(fpc,L" %lf", JDarray[trow][k]);
		fwprintf(fpc,L"\n");
		trow=trow+1;
		do
		{
			fwprintf(fpc,L"%d  ",(int)(JDarray[trow][0]+0.001));  
			for (k=1;k<=5;k++)  fwprintf(fpc,L"  %15.6f",JDarray[trow][k]);
			fwprintf(fpc,L"\n");
			trow++;

		} while(trow<NJD);
		//save DL
		fwprintf(fpc,L"%d\n",NDL);
		for(int i=0;i<NDL;i++)
		{
			//			double BLC,DL;  //  �����Ķ�ǰ��̺Ͷ���ֵ(+ֵΪ����;-Ϊ����)
			//			char   BGH[8],EGH[8]; //  �����Ķ�ǰ��̹ںźͶϺ���̹ں�

			fwprintf(fpc,L"%8s %8s %15.6lf %15.6lf\n",DLB[i].BGH,DLB[i].EGH,DLB[i].BLC,DLB[i].DL);			
		}
		fclose(fpc);
	};
	return 1;
}		

int GTZX_ROAD::savePXY(ACHAR *fname)
{   
	assertReadEnabled();
	int k,trow=0,i,j;
	FILE *fpc;
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"w"))!=NULL)
	{
		//  ���
		for (k=1;k<=5;k++) fwprintf(fpc,L"  %lf", array_pxy[0][0][k] );
		trow=1;
		for( i=1;i<NJD-1;i++) 
		{ for (k=0;k<4;k++) 	   { 
			for (j=0;j<5;j++)fwprintf(fpc,L" %lf", array_pxy[i][k][j] );
			trow=trow+1;  	}; 		   
		};
		//  �յ�
		for (j=0;j<5;j++) fwprintf(fpc,L" %lf",array_pxy[NJD-1][0][j]);   
		trow=trow+1;

		fclose(fpc);
	};
	return 1;
}


void GTZX_ROAD::setJD_RL(int JDi ,double Ri,double L1i,double L2i )
{
	assertWriteEnabled();
	if (JDi>0 && JDi<NJD-1)  // ��Ϊ���յ�
	{  JDarray[JDi][3]=L1i;
	JDarray[JDi][5]=L2i;
	JDarray[JDi][4]=Ri;
	} ;	 
} 

int GTZX_ROAD::getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE )
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

int GTZX_ROAD::get_pxy(int id,double *d0,double *d1,double *d2,double *d3,double *d4)
{    
	assertReadEnabled();
	int trow=0,i,k;
	trow=(NJD-2)*4+2;
	if(id==0)
	{  *d0=array_pxy[0][0][0];
	*d1=array_pxy[0][0][1];
	*d2=array_pxy[0][0][2];
	*d3=array_pxy[0][0][3];
	*d4=array_pxy[0][0][4];
	} 
	else 
	{   i=(int)((id-1)/4+1);
	k=(id-1)-(i-1)*4;
	*d0=array_pxy[i][k][0];
	*d1=array_pxy[i][k][1];
	*d2=array_pxy[i][k][2];
	*d3=array_pxy[i][k][3];
	*d4=array_pxy[i][k][4];
	};

	return trow;
}

Adesk::Boolean  GTZX_ROAD::addXL(int JDi,double R,double L1,double L2 ,double NN ,double EE,double Nend ,double Eend )
{   
	assertWriteEnabled();
	JDarray[NJD-1][0]=NJD-1;
	JDarray[NJD-1][1]=NN;
	JDarray[NJD-1][2]=EE;
	JDarray[NJD-1][3]=L1;
	JDarray[NJD-1][5]=L2;
	JDarray[NJD-1][4]=R;
	JDarray[NJD  ][0]=NJD;
	JDarray[NJD  ][1]=Nend;
	JDarray[NJD  ][2]=Eend;
	JDarray[NJD  ][3]=0.0;
	JDarray[NJD  ][5]=0.0;
	JDarray[NJD  ][4]=0.0;
	NJD =NJD +1;	  
	return 1;  
} 
//  ����һ����������
Adesk::Boolean  GTZX_ROAD::addJD(int JDi,double R,double L1,double L2 ,double NN ,double EE )
{   
	int i,j;
	if (JDi>0 && JDi<=NJD-1 && NJD<MAXJDNUM)  // ��Ϊ���յ�  NJD����СֵΪ2  ���ֵΪ100
	{  	    
		assertWriteEnabled();
		for (i=NJD-1;i>=JDi;i--) // ����һ�� ����
			for ( j=0;j<6;j++) JDarray[i+1][j]=JDarray[i][j];

		NJD =NJD +1;	
		JDarray[JDi][0]=NJD ;
		JDarray[JDi][1]=NN;
		JDarray[JDi][2]=EE;
		JDarray[JDi][3]=L1;
		JDarray[JDi][5]=L2;
		JDarray[JDi][4]=R;
		return 1;  
	} else return 0;
}


//  ɾ��һ����������
Adesk::Boolean  GTZX_ROAD:: DelJD(int JDi )
{   
	int i,j;
	//plh07.19a
	if (JDi>=0 && JDi< NJD && NJD<MAXJDNUM)  // ��Ϊ���յ�  NJD����СֵΪ2  ���ֵΪ100
	{  	 
		assertWriteEnabled();
		for (i=JDi;i<NJD-1;i++) // ǰ��һ�� ����
		{
			for ( j=0;j<6;j++) JDarray[JDi+(i-JDi)][j]=JDarray[JDi+1+(i-JDi)][j];		   
		}
		if(JDi==0)JDarray[JDi][3]=0.0;
		NJD =NJD -1;	  
		return 1; 
	} else return 0;
}

//	��ע����ź����߲���
int GTZX_ROAD::BZJD1(double arrayd[MAXJDNUM][6],double qxb[MAXJDNUM][9],int trow,int JD0, double texth,int draw_zybz,int qxbbz,int NLC)
{  double fb,fe,de,da,aa,dd,dt ;
AcGePoint3d  p1; 
ACHAR ch[10],ch1[60],chrtmp[80],ch2[10];
int i,c,n ;

ACHAR JDchr[20];
_tcscpy(JDchr,JDGH);

de=xyddaa(arrayd[0][1],arrayd[0][2],arrayd[1][1],arrayd[1][2],&fb);
for (i=1;i<trow-1;i++)
{  
	if (arrayd[i][4]<=0) break;
	de=xyddaa(arrayd[i][1],arrayd[i][2],arrayd[i+1][1],arrayd[i+1][2],&fe);
	aa= fe - fb ;
	if (aa<-pi) aa=aa+2*pi;
	if  (aa>pi) aa=aa-2*pi;
	if (aa>0) {  c=1; dd=texth;  }  //��ƫ
	else { c=-1; dd=texth+texth; }; //��ƫ
	de=fb+aa*0.5;

	ads_rtos(JD0+i,2,0,ch);
	_tcscpy(ch1,JDchr); _tcscat(ch1,ch); 
	n=_tcslen(ch1); p1[Z]=0.0;

	p1[X]=arrayd[i][2]-n*0.5*texth*sin(de)+dd*sin(de-c*pi*0.5) ; 
	p1[Y]=arrayd[i][1]-n*0.5*texth*cos(de)+dd*cos(de-c*pi*0.5);

	da= 0.5*pi-de;
	// G_maketext(pWd,p1,ch1,de,texth ,4,1);   
	maketext( p1,ch1,da,texth ,4,1);  // �����

	p1[X]=arrayd[i][2] ;  p1[Y]=arrayd[i][1] ;
	makecircle(p1,3.6,6);



	if( qxbbz==1)
	{
		// �����߲��� begin
		double R,T,T1,T2,L1,L2,LL,DFM;
		R=arrayd[i][4]; L1= arrayd[i][3];  L2=arrayd[i][5]; 
		T1=qxb[i][5]; T2=qxb[i][6];  LL=qxb[i][7];
		T=(T1+T2)*0.5-(L1+L2)*0.25;

		dd=sqrt(R*R+T*T)-R;
		// ads_printf(L"\n dd=%8.2f",dd);
		if (draw_zybz==0) 
		{  dd=dd+16*texth; dt=c*texth*1.5;
		if (c==-1) dd=dd+8*texth;
		}
		else {   dd=dd+draw_zybz*c*16*texth; dt=c*texth*1.5; 
		if (draw_zybz==-1) dd=dd-8*c*texth;
		}
		//ads_printf(L"\ dd=%8.2f",dd);
		p1[X]=arrayd[i][2]-dd*sin(de-c*pi*0.5) ; 
		p1[Y]=arrayd[i][1]-dd*cos(de-c*pi*0.5);
		_tcscpy(chrtmp,L"  "); _tcscat(chrtmp,ch1);
		maketext(p1,chrtmp,da,texth ,4,1);   // �����

		ads_rtos(qxb[i][4],2,0,ch );// ��

		DFM=qxb[i][4]-((int)qxb[i][4]) ;
		ads_rtos(DFM,2,4,chrtmp); _tcscpy(ch2,L"'");

		_tcscat(ch,L"%%d");
		ch1[0]=chrtmp[2]; ch1[1]=chrtmp[3]; ch1[2]=ch2[0];
		ch1[3]=chrtmp[4] ;ch1[4]=chrtmp[5] ; ch1[5]='\0';
		_tcscat(ch,ch1);_tcscat(ch,L"%%034"); 

		if (c==1) { _tcscpy(ch1,L"ay--"); }
		else _tcscpy(ch1,L"az--"); 
		_tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);

		maketext(p1,ch1,da,texth ,4,1);   //  ƫ��

		if (fabs(R-(long int)(R))<0.001)  { ads_rtos(R,2,0,ch); }
		else { ads_rtos(R,2,NLC,ch); };
		_tcscpy(ch1,L"R--"); _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		maketext(p1,ch1,da,texth ,4,1);   //   �뾶

		if (L1>0||L2>0)  //  ������Ϊ��
		{
			if (fabs(L1-(long int)(L1))<0.001)  { ads_rtos(L1,2,0,ch); }
			else { ads_rtos(L1,2,NLC,ch); };
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if (fabs(L1-L2)<0.001) 
			{ _tcscpy(ch1,L"Lo--");  _tcscat(ch1,ch);
			maketext(p1,ch1,da,texth ,4,1);   //   ���� ���
			} else
			{ _tcscpy(ch1,L"L1--");  _tcscat(ch1,ch);
			if (fabs(L2-(long int)(L2))<0.001)  { ads_rtos(L2,2,0,ch); }
			else { ads_rtos(L2,2,NLC,ch); };
			_tcscat(ch1,L"  L2--"); _tcscat(ch1,ch);
			maketext(p1,ch1,da,texth ,4,1);   //   ���Ȼ���
			};
		};

		if (fabs(T1-(long int)(T1))<0.001)  { ads_rtos(T1,2,0,ch); }
		else { ads_rtos(T1,2,NLC,ch); };
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if (fabs(L1-L2)<0.001) 
		{   _tcscpy(ch1,L"T--");  _tcscat(ch1,ch);
		maketext(p1,ch1,da,texth ,4,1);   //   �г� ���
		} else
		{   _tcscpy(ch1,L"T1--");  _tcscat(ch1,ch);
		ads_rtos(T2,2,NLC,ch);
		_tcscat(ch1,L"  T2--"); _tcscat(ch1,ch);
		maketext(p1,ch1,da,texth ,4,1);   //   �����г�
		};

		ads_rtos(LL,2,NLC,ch);
		_tcscpy(ch1,L"L--");  _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		maketext(p1,ch1,da,texth ,4,1);   //  ���߳�
		// �����߲��� end
	};

	fb=fe; 

};  
return 1;
}

int GTZX_ROAD::BZLC1(double array[4*MAXJDNUM][10],int trow,ACHAR *GCH,int NDL,int NLC,double texth,double dlc,double LCB,int zybz,int type)  //  ��ע���
{     AcGePoint3d  ptb,pt,pte,p1;
ACHAR chrtmp[20],chr[50],ch[50];
double  B,WB,lc, b,DDlength,DL,BZlc ;
int  ii, k ,BZW ,iDL;
long int zlc;
ACHAR GH[20],GH1[20];
xlpoint  PZtmp;

_tcscpy(GH,DLBTMP[0].EGH);

WB=1.5*texth;
LCB=DLBTMP[0].BLC+DLBTMP[0].DL;

ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;p1[Z]=0.0;
// ������ͳһ���
DDlength=LCB;
// ��������α�ע
for (iDL=0;iDL<NDL;iDL++)
{ 
	DL=DLBTMP[iDL].DL;
	LCB= DLBTMP[iDL].BLC+DL;  //���������� 
	_tcscpy(GH,DLBTMP[iDL].EGH);
	_tcscpy(GH1,DLBTMP[iDL].BGH);
	// ads_printf(L"\n iDL=%d %s %10.3f  %s %10.3f",iDL,DLBTMP[iDL].BGH,DLBTMP[iDL].BLC,DLBTMP[iDL].EGH,DLBTMP[iDL].BLC+DL);

	// ������� ��ںŸı��
	if (iDL>0 && iDL< NDL-1)  // ��Ϊ�� �յ�
	{ // ��� iDL��
		lc=DDlength;
		PZtmp.lc=lc;  // ͳһ���
		k=xlpoint_pz(array,tarray,&PZtmp);  
		if (k>0&& k<=6)  //�н�
		{
			B=PZtmp.a;
			pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
			pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
			ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
			makeline( ptb,pt,2);
			// makeline(ptb,pt,5);

			PZtmp.lc=lc-1.2*texth;
			k=xlpoint_pz(array,tarray,&PZtmp);  B=PZtmp.a;
			p1[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
			p1[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

			PZtmp.lc=lc+2.0*texth;
			k=xlpoint_pz(array,tarray,&PZtmp);  B=PZtmp.a;
			pte[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
			pte[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

			b=B+pi*0.5;
			b=pi*0.5-b; if (zybz==-1) b=b+pi;
			if (b<0.0) b=2*pi+b;		  if (b>2*pi) b=b-2*pi;		         
			if(type<=1)  b=b-pi*0.5; 

			_tcscpy(ch,L"\0");  
			_tcscpy(ch,LCchr(GH1,DLBTMP[iDL].BLC,NLC,1));  // ��ǰ���
			_tcscpy(chr,L"  ");_tcscat(chr,ch);   
			maketext( p1,chr,b,texth,4,1);  

			_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,LCB,NLC,1));     // �Ϻ����
			_tcscpy(chr,L" =");  _tcscat(chr,ch);   // e.f chr="ZH K10+123.45"   

			if (fabs( DL)>0.001)
			{  if ( DL>0.0)   { 
				_tcscat(chr, L" ���� "); 
				ads_rtos( DL,2,NLC,ch); _tcscat(chr, ch); 
				_tcscat(chr, L" m");  };
			if ( DL<0.0)   { 
				_tcscat(chr, L" ���� "); 
				ads_rtos(fabs(DL),2,NLC,ch); _tcscat(chr, ch); 
				_tcscat(chr, L" m");  };
			};

			maketext( pte,chr,b,texth,4,1);     

		};

	};

	zlc=(long int)(LCB/dlc);
	BZlc=zlc*dlc;  // ʵ����� 
	if ((BZlc-LCB)<0.0001)  { BZlc=BZlc+dlc;  }  
	_tcscpy(GH,DLBTMP[iDL].EGH);  //�ں�

	do {
		// ͳһ���
		lc=DDlength+BZlc-LCB; 
		PZtmp.lc=lc;  // ͳһ���
		k=xlpoint_pz(array,tarray,&PZtmp);   
		BZW=k;    //  K<0  �޽�
		if (k>0&& k<=6)  //�н�
		{
			// ads_printf(L"\n i=%d %10.3f %10.3f  %10.3f %10.3f ",iDL,PZtmp.lc, BZlc,LCB,DLBTMP[iDL+1].BLC );  
			B=PZtmp.a;
			pt[X]=PZtmp.y+WB*sin(B+pi*0.5*zybz);
			pt[Y]=PZtmp.x+WB*cos(B+pi*0.5*zybz);
			pte[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
			pte[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);
			ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
			makeline( ptb,pt,2);
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
			if (dlc>=100.0&&ii>=2)
			{  chr[ii-1]='\0';chr[ii-2]='\0';
			_tcscpy(ch,chr);
			if (ii>3&&chr[ii-3]=='0') // �����
			{ chr[ii-3]='\0';  _tcscpy(ch,GH); _tcscat(ch,chr); }
			else if(ii>3 && chr[ii-3]!='0')    // ���ױ�
			{ ch[0]=chr[ii-3];ch[1]='\0';};		    
			}
			else {
				_tcscpy(ch,GH);
				if ( ii<=2)
				{ _tcscpy(ch,L"+"); ch[1]='0';
				ch[2]=chr[ii-2]; ch[3]=chr[ii-1];ch[4]='\0';
				}
				else if ( ii>=3)
				{
					_tcscpy(chrtmp,L"+"); chrtmp[1]=chr[ii-3];
					chrtmp[2]=chr[ii-2]; chrtmp[3]=chr[ii-1];chrtmp[4]='\0';
					if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // �����
					{ chr[ii-3]='\0';
					_tcscpy(ch,GH);
					_tcscat(ch,chr); _tcscat(ch,chrtmp);
					}
					else if (ii>3&&chr[ii-3]!='0')
					{   _tcscpy(ch,chrtmp);	    };
				};
				//  _tcscat(ch,chr);
			};
			// ads_printf(L"ch=%s chr=%s\n",ch,chr);
			//   ads_printf(L"\n %10.3f %10.3f  ",PZtmp.lc, BZlc);       
			if(type<=1)  b=b-pi*0.5; 

			pte[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz)+0.5*texth*sin(B);
			pte[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz)+0.5*texth*cos(B);

			maketext( pte,ch,b,texth,4,1);     
		}; 
		BZlc=BZlc+dlc;
		if ( BZlc>DLBTMP[iDL+1].BLC) BZW=-99;		 
	}while (BZW>0);
	DDlength=  DDlength+DLBTMP[iDL+1].BLC-LCB;
} // IDL   
return 1 ;
}

int GTZX_ROAD::BZTZD1 (double array[4*MAXJDNUM][10],int trow, ACHAR *GCH, int NDL,double texth ,double LCB,int bz_zybz,int Chinese_Engbz_type ,int NLC )  //  ��ע���
{     AcGePoint3d  ptb,pt,pte;
double   WB,lc,b,l,INDATA[5],L0;
int i, j,k ,KK=7,zybz=-1;

ACHAR chr[50],ch[40],tch[6], GH[10],DGH[10],lcchr[20] ;
xlpoint  P0tmp,PZtmp;

_tcscpy(GH,DGH);


WB=4*texth;
WB=WB+10*texth;

// ads_initget(1+2+4,NULL);
// ads_getint(L"\n Enter ��������� type=1 (in English) / type=2 (in Chinese)):",&Chinese_Engbz_type);
//  if (Chinese_Engbz_type<1||Chinese_Engbz_type>2) Chinese_Engbz_type=1;
P0tmp.lc=array[0][4]; 
P0tmp.x=array[0][1];P0tmp.y=array[0][2];
P0tmp.a=rad(array[0][3]);P0tmp.r=1e40;
for (i=1;i<trow;i++)
{
	zybz=1;

	for (k=0;k<5;k++) INDATA[k]=array[i][k];j=(int)(array[i][0]+0.01);
	l=0.0;
	if (j>=3)  {
		// _tcscpy(chr,L"A=");
		_tcscpy(chr,L"L=");
		_tcscpy(tch,L".S."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"H"); };
	if (j==1) { l=INDATA[1];  _tcscpy(chr,L"L="); _tcscpy(tch,L".T."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"Z"); };
	if (j==2) { l=INDATA[2];  _tcscpy(chr,L"R="); _tcscpy(tch,L".C."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"Y"); };

	//  ��A=��
	if (j==3)   l=INDATA[1]*INDATA[1]/INDATA[2];
	if (j==4)   l=INDATA[1]*INDATA[1]/INDATA[2];
	if (j==5) l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
	if (j==6) l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);

	// ��L0=��
	L0=l; if (j==2) L0=INDATA[1];

	ptb[X]=P0tmp.y;ptb[Y]=P0tmp.x; ptb[Z]=0.0;pt[Z]=0.0;pte[Z]=0.0;

	// �� ֱ�ߡ�԰���ߡ��������߳���
	if (zybz==1||zybz==-1) {
		lc=l*0.5-3.721*texth; if (lc<0.0) lc=l*0.5;
		xlzbxy(&P0tmp,&PZtmp,lc,INDATA,0);b=PZtmp.a+pi*0.5*zybz;
		pt[X]=PZtmp.y+2.0*texth*sin(b);pt[Y]=PZtmp.x+2.0*texth*cos(b);pt[Z]=0.0;
		b=PZtmp.a; b=pi*0.5-b;if (b<0.0) b=2*pi+b;
		if (b>2*pi) b=b-2*pi;
		// ads_rtos(INDATA[1],2,NLC,ch);_tcscat(chr,ch);
		if (fabs(L0-(long int)(L0))<0.001)  { ads_rtos(L0,2,0,ch); }
		else { ads_rtos(L0,2,NLC,ch); };
		_tcscat(chr,ch);
		if ( L0>0.001) 
			maketext( pt,chr,b,texth,4,1);  // �� ֱ�ߡ�԰���ߡ��������߳���
	};

	xlzbxy(&P0tmp,&PZtmp,l,INDATA,0); // ĳ��Ԫ�յ�
	//ȷ�� ���Ʒ���
	zybz=bz_zybz;
	if (bz_zybz==0) 
	{ j=(int)(array[i][0]+0.01);  // ���� 
	if (j>1&&j<=6)  zybz=(int)(array[i][4]+0.01);
	if(j==1) 		zybz=(int)(array[i-1][4]+0.01);	 
	if (zybz==0) zybz=-1;
	// ads_printf(L"\n j=%d array[i][4]=%2.0f zybz=%d",j,array[i][4],zybz);
	}
	//
	b=P0tmp.a+pi*0.5*zybz;
	pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
	if ( L0>0.001)  makeline( ptb,pte,3);

	if (i==1)
	{  _tcscpy(chr,L" B.P"); if (Chinese_Engbz_type==2)_tcscpy(chr,L" QD");
	if (KK==7) {
		lc=P0tmp.lc;   
		lc=XLC1(lc,GH,NDL,DLBTMP);
		_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,lc,NLC,1));
		_tcscat(chr,L" ");_tcscat(chr,ch);   // e.f chr="ZH K10+123.45"
		_tcscpy(lcchr,ch);                 // e.f lcchr="K10+123.45"
	};
	b=P0tmp.a;
	// pt[X]=ptb[X]+0.25*zybz*texth*sin(b); pt[Y]=ptb[Y]+0.25*zybz*texth*cos(b);
	//  b=P0tmp.a+pi*0.5*zybz;
	pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
	b=P0tmp.a-pi*0.5 ;
	if(zybz==1) {	pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
	pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b); }

	b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
	maketext( pt,chr,b,texth,4,1);
	// maketext( pt,chr,b,texth,5,1);

	if (i==trow-1 ) {
		ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; ptb[Z]=0.0;
		_tcscpy(chr,L" EP");  if (Chinese_Engbz_type==2)_tcscpy(chr,L" ZD");
		if (KK==7) {
			lc=PZtmp.lc; lc=XLC1(lc,GH,NDL,DLBTMP);

			_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,lc,NLC,1));
			_tcscat(chr,L" ");_tcscat(chr,ch);
		};

		b=PZtmp.a;
		// pt[X]=ptb[X]+0.25*zybz*texth*sin(b); pt[Y]=ptb[Y]+0.25*zybz*texth*cos(b);
		// b=PZtmp.a+pi*0.5*zybz;
		pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
		b=PZtmp.a-pi*0.5 ;
		if(zybz==1) {	pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
		pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b); }

		b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
		maketext( pt,chr,b,texth,4,1);
		b=PZtmp.a+pi*0.5*zybz;
		pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
		makeline( ptb,pte,3);
		// ads_printf(L"\n i=%d,lc=%12.3f chr=%s",trow-1,lc, chr);
	};
	}
	else if (i>1 && i<=trow-1) 
	{
		j=(int)(array[i-1][0]+0.01);
		if (j>=3) { _tcscpy(ch,L" S"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" H"); };
		if (j==1) { _tcscpy(ch,L" T"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" Z"); };
		if (j==2) { _tcscpy(ch,L" C"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" Y"); };
		_tcscpy(chr,ch);_tcscat(chr,tch);

		if (_tcsncmp(chr,L" YY",3)==0)   {  _tcscpy(chr,L" GQ");    };
		if (_tcsncmp(chr,L" C.C.",5)==0) {  _tcscpy(chr,L" P.C.C.");};
		if (_tcsncmp(chr,L" S.S.",5)==0&&(j==3||j==4) ) {  _tcscpy(chr,L" PRC");};
		if (_tcsncmp(chr,L" HH",3)==0&&(j==3||j==4)) {  _tcscpy(chr,L" FGD");};
		_tcscpy(DGH,chr);

		if (KK==7)
		{ lc=P0tmp.lc;   lc=XLC1(lc,GH, NDL,DLBTMP);
		_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,lc,NLC,1));
		_tcscat(chr,L" ");_tcscat(chr,ch);
		};
		b=P0tmp.a;
		// pt[X]=ptb[X]+0.25*zybz*texth*sin(b); pt[Y]=ptb[Y]+0.25*zybz*texth*cos(b);
		// b=P0tmp.a+pi*0.5*zybz;
		pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
		b=P0tmp.a-pi*0.5 ;
		if(zybz==1) {	pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
		pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b); }

		b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi; 
		maketext( pt,chr,b, texth,4,1);
		// ads_printf(L"\n i=%d,L0= %10.4f lc=%12.3f chr=%s",i,L0,lc,chr);

		if (i==trow-1 ) {
			ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; ptb[Z]=0.0;
			_tcscpy(chr,L" EP");  if (Chinese_Engbz_type==2)_tcscpy(chr,L" ZD");
			if (KK==7) {
				lc=PZtmp.lc;   lc=XLC1(lc,GH, NDL,DLBTMP);
				_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,lc,NLC,1));
				_tcscat(chr,L" ");_tcscat(chr,ch);
			};
			b=PZtmp.a;
			//pt[X]=ptb[X]+0.25*zybz*texth*sin(b); pt[Y]=ptb[Y]+0.25*zybz*texth*cos(b);
			//b=PZtmp.a+pi*0.5*zybz;
			pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
			b=PZtmp.a-pi*0.5 ;
			if(zybz==1) {	pt[X]=pt[X]-(_tcslen(chr)+0.25)*texth*sin(b);
			pt[Y]=pt[Y]-(_tcslen(chr)+0.25)*texth*cos(b); }

			b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
			maketext( pt,chr,b,texth,4,1);
			b=PZtmp.a+pi*0.5*zybz;
			pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
			makeline( ptb,pte,3);
			// ads_printf(L"\n i=%d,lc=%12.3f chr=%s",trow-1,lc, chr);
		};
	};
	P0tmp=PZtmp;
};
return 1   ;
}
void GTZX_ROAD::draw_pm()
{
	double h_plan=10.0,st_ml=30;
	/*    for (i =1 ; i < NJD-1 ; i++ )
	{
	_tcscpy(str ,L"");
	_tcscpy(str1,L"");
	_tcscpy(str2,L"");
	dd=(int)QXB[i][4];
	mm=(int)( QXB[i][4]*100.0 +0.001 - dd*100.0 );
	ss= QXB[i][4]*10000.0-dd*10000.0 - mm*100.0;
	B = ( dd + mm / 60.0 + ss / 3600.0 ) * pi /180.0 ;
	rtostr(QXB[i][0]-QXB[i-1][3],str) ;
	Spoint(pt[0] , ( QXB[i-1][3]*0.5+QXB[i][0]*0.5 - K0 ) / scale_h + st_ml , h_plan+.8, 0 ) ;
	//     cetext(4,str, pt[0], 2.0, 2.0, 1 ,0.0, 0.9,L"plan") ;
	maketext(AcGePoint3d(pt[0][0],pt[0][1],pt[0][2]),str,0.0 ,texth ,4,1);
	if ( i == 1 )  { Spoint(pt[0] , ( QXB[i-1][3] - K0 ) / scale_h + st_ml  , h_plan, 0 ) ; }
	else { Spoint(pt[0] , ( QXB[i-1][3] - K0 ) / scale_h + st_ml , h_plan, 0 ) ; };
	Spoint(pt[1] , ( QXB[i][0] - K0 ) / scale_h + st_ml , h_plan , 0 ) ;
	Spoint(pt[2] , ( QXB[i][1] - K0 ) / scale_h + st_ml , h_plan + QXB[i][8]*5.0 , 0 ) ;
	Spoint(pt[3] , ( QXB[i][2] - K0 ) / scale_h + st_ml , h_plan + QXB[i][8]*5.0 , 0 ) ;
	Spoint(pt[4] , ( QXB[i][3] - K0 ) / scale_h + st_ml , h_plan , 0 ) ;

	pline(5,&pt[0], 0 ,  0.3,L"plan",4) ;
	Spoint(pt[2] , ( QXB[i][0] - K0 ) / scale_h + st_ml , h_plan + 8.0 , 0 ) ;
	line(2,&pt[1],0,L"plan",4);
	Spoint(pt[2] , ( QXB[i][0] - K0 ) / scale_h + st_ml - 0.4, h_plan + 1.5 , 0 ) ;
	hm = QXB[i][0] - (int)(QXB[i][0]/100.0+0.0001)*100.0 ;
	if (CSINF == 0 ) _stprintf(str,L"%02.0lf",hm);
	else if (CSINF == 1 ) _stprintf(str,L"%05.2lf",hm);
	maketext(AcGePoint3d(pt[0][0],pt[0][1],pt[0][2]),str,0.0 ,texth ,4,1);
	//     cetext(4,str, pt[2], 2.0, 2.0, 0 ,90.0, 0.8,L"plan");
	Spoint(pt[3] , ( QXB[i][3] - K0 ) / scale_h + st_ml , h_plan + 8.0 , 0 ) ;
	line(2,&pt[3],0,L"plan",4);

	hm = QXB[i][3] - (int)(QXB[i][3]/100.0+0.0001)*100.0 ;
	if (CSINF == 0 ) _stprintf(str,L"%02.0lf",hm);
	else if (CSINF == 1 ) _stprintf(str,L"%05.2lf",hm);
	Spoint(pt[3] , ( QXB[i][3] - K0 ) / scale_h + st_ml - 0.4, h_plan + 1.5 , 0 ) ;
	maketext(AcGePoint3d(pt[3][0],pt[3][1],pt[3][2]),str,0.0 ,texth ,4,1);
	//       cetext(4,str, pt[3], 2.0, 2.0, 0 ,90.0, 0.8,L"plan");
	Spoint(pt[0],( 0.5*(QXB[i][0]+QXB[i][3]) - K0 ) / scale_h + st_ml , h_plan +2.0-QXB[i][8]*5.0, 0 );
	if ( CSINF == 0 )
	{
	if ( QXB[i][8] > 0.0 ) _stprintf(str,L"ay-%3d%s%02d'",dd,L"%%d",mm);
	else  _stprintf(str,L"az-%3d%s%02d'",dd,L"%%d",mm);
	}
	else if ( CSINF == 1 )
	{
	if ( QXB[i][8] > 0.0 ) _stprintf(str,L"ay-%3d%s%02d'%02.0f\"",dd,L"%%d",mm,ss);
	else  _stprintf(str,L"az-%3d%s%02d'%02.0f\"",dd,L"%%d",mm,ss);
	};

	d_space_str (str , 2 , 5 ) ;
	maketext(AcGePoint3d(pt[0][0],pt[0][1],pt[0][2]),str,0.0 ,texth ,4,1); 
	//       cetext(4,str, pt[0], 2.3, 2.3, 1 , 0.0, 0.9,L"plan");
	Spoint(pt[0],( 0.5*(QXB[i][0]+QXB[i][3]) - K0 ) / scale_h + st_ml , h_plan -1.25-QXB[i][8]*5.0, 0 );
	_tcscpy(str,L"R-");
	rtostr3(JDarray[i][4],str1);
	_tcscat(str,str1);
	if ( JDarray[i][3] == JDarray[i][5] ) _stprintf(str1,L"l0-%4.0f",JDarray[i][3]);
	else _stprintf(str1,L"l1-%4.0f l2-%4.0f",JDarray[i][3],JDarray[i][5]);
	_tcscat(str,str1);
	maketext(AcGePoint3d(pt[0][0],pt[0][1],pt[0][2]),str,0.0 ,texth ,4,1);
	//       cetext(4,str, pt[0], 2.3, 2.3, 1 , 0.0, 0.8,L"plan");
	Spoint(pt[0],( 0.5*(QXB[i][0]+QXB[i][3]) - K0 ) / scale_h + st_ml , h_plan -4.5-QXB[i][8]*5.0, 0 );
	if ( fabs(QXB[i][5] - QXB[i][6] ) < 0.001 ) _stprintf(str,L"T-%8.3f",QXB[i][5]);
	else _stprintf(str,L"T1-%8.3f T2-%8.3f",QXB[i][5],QXB[i][6]);
	_tcscat(str,L" L-") ; rtostr(QXB[i][3]-QXB[i][0],str2) ;
	_tcscat(str,str2);
	maketext(AcGePoint3d(pt[0][0],pt[0][1],pt[0][2]),str,0.0 ,texth ,4,1);
	//     cetext(4,str, pt[0], 2.3, 2.3, 1 , 0.0, 0.8,L"plan");
	}  ;    // exit for
	_tcscpy(str ,L"");
	_tcscpy(str1,L"");
	_tcscpy(str2,L"");
	Spoint(pt[0],( QXB[i][3][NJD-2] - K0 ) / scale_h + st_ml , h_plan, 0 ) ;
	Spoint(pt[1],( END - K0 ) / scale_h + st_ml , h_plan, 0 ) ;
	pline(2,&pt[0], 0 ,  0.3,L"plan",4) ;
	*/
}
int GTZX_ROAD::PM_DMX(double *dmx)
{
	double sml=DLB[0].BLC+DLB[0].DL;
	double eml=sml + xl_length;

	FormLCB(ZDMJG,sml,eml);
	double startLC,endLC;
	startLC=pLCB[0].lc;
	endLC=pLCB[NLCB-1].lc;
	xlpoint PZ;
	double pp[2],tmp;
	int NUM=0;
	int i=0,j=0,k=0;
	double lc=startLC;
	PZ.lc=lc;
	xlpoint_pz(array,tarray,&PZ) ;
	pp[0] = PZ.x;
	pp[1] = PZ.y;

	tmp=ptriDTM.interpolate(pp);

	dmx[NUM*2]=PZ.lc;
	dmx[NUM*2+1]=tmp;
	NUM++;

	ACHAR pszLabel[80];
	_tcscpy(pszLabel,L"�ݶ���������ڲ�");
	acedSetStatusBarProgressMeter(pszLabel,0,100);
	int SUM = (int)((endLC-startLC)/ZDMJG+0.1);
	while (PZ.lc<endLC)//QXB���Ϊ�ֳ���̣�
	{	
		int Pos = (int)(NUM*100.0/SUM);
		acedSetStatusBarProgressMeterPos(Pos);
		//ads_pointf("%d %f\n",i,pLCB[i].lc);
		//ads_printf(L"%d lc%f qxb%f lc2%f\n",i,pLCB[i].lc,QXB[j][k],lc);
		if(pLCB[i].lc<QXB[j][k])
		{
			lc=pLCB[i].lc;		
			i++;
		}	
		else if(j<NJD)
		{
			lc=QXB[j][k];
			k++;
			if(k==4)
			{j++;k=0;}
		}
		PZ.lc=lc;
		xlpoint_pz(array,tarray,&PZ) ;
		pp[0] = PZ.x ;
		pp[1] = PZ.y ;
		tmp=ptriDTM.interpolate(pp);

		dmx[NUM*2]=PZ.lc;
		dmx[NUM*2+1]=tmp;
		//		acutPrintf(L"num=%d,NUM*2=%0.3lf,(NUM-1)*2=%0.3lf  ",NUM,dmx[NUM*2],dmx[(NUM-1)*2]);//fabs(dmx[NUM*2]-dmx[(NUM-1)*2]));
		if(fabs(dmx[NUM*2]-dmx[NUM*2-2])>0.001)NUM++;			
	}
	acedSetStatusBarProgressMeterPos(100);    
	acedRestoreStatusBar();

	acutPrintf(L"NUM=%d\n",NUM);
	return NUM;
}

void GTZX_ROAD::outdata(CString str)
{
	FILE *fp;
	/*
	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\����\\QXB.txt",L"w+");
	for(int i=0;i<100;i++)
	{
	fwprintf(fp,L"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",QXB[i][0],QXB[i][1],QXB[i][2],QXB[i][3],QXB[i][4],QXB[i][5],QXB[i][6],QXB[i][7],QXB[i][8]);
	}
	fclose(fp);*/

	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\����\\"+str+"array.txt",L"w+");
	for(int i=0;i<4*MAXJDNUM;i++)
	{
		fwprintf(fp,L"%lf %lf fwj=%lf R=%lf x=%lf y=%lf\n",array[i][0],array[i][1],array[i][5],array[i][7],array[i][8],array[i][9]);
	}
	fclose(fp);	
	/*
	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\����\\array_pxy.txt",L"w+");
	for(i=0;i<100;i++)
	{
	fwprintf(fp,L"\n");
	for(int j=0;j<4;j++)
	{
	fwprintf(fp,L"%lf,%lf,%lf,%lf,%lf\n",array_pxy[i][0],array_pxy[i][1],array_pxy[i][2],array_pxy[i][3],array_pxy[i][4]);
	}		
	}
	fclose(fp);*/

}

int GTZX_ROAD::PM_PQX(double *pqx)
{

	for(int i=0;i<NJD-2;i++)
	{
		//line
		pqx[4*i*6]=QXB[i][3];
		pqx[4*i*6+1]=QXB[i+1][0]-QXB[i][3];
		pqx[4*i*6+2]=0;//line
		pqx[4*i*6+3]=0;
		pqx[4*i*6+4]=0;
		pqx[4*i*6+5]=0;
		//spire
		pqx[(4*i+1)*6+0]=QXB[i+1][0];
		pqx[(4*i+1)*6+1]=QXB[i+1][1]-QXB[i+1][0];
		pqx[(4*i+1)*6+2]=1;//spire
		pqx[(4*i+1)*6+3]=QXB[i+1][8];//lift-1 right1
		pqx[(4*i+1)*6+4]=0;
		pqx[(4*i+1)*6+5]=0;
		//circle
		pqx[(4*i+2)*6+0]=QXB[i+1][1];
		pqx[(4*i+2)*6+1]=QXB[i+1][2]-QXB[i+1][1];
		pqx[(4*i+2)*6+2]=2;//circle
		pqx[(4*i+2)*6+3]=QXB[i+1][8];//lift-1 right1
		pqx[(4*i+2)*6+4]=QXB[i+1][4];
		pqx[(4*i+2)*6+5]=JDarray[i+1][4];
		//spire
		pqx[(4*i+3)*6+0]=QXB[i+1][2];
		pqx[(4*i+3)*6+1]=QXB[i+1][3]-QXB[i+1][2];
		pqx[(4*i+3)*6+2]=-1;//spire
		pqx[(4*i+3)*6+3]=QXB[i+1][8];//lift-1 right1
		pqx[(4*i+3)*6+4]=0;
		pqx[(4*i+3)*6+5]=0;
	}
	//line
	pqx[4*(NJD-2)*6+0]=QXB[(NJD-2)][3];
	pqx[4*(NJD-2)*6+1]=QXB[(NJD-2)+1][0]-QXB[(NJD-2)][3];
	pqx[4*(NJD-2)*6+2]=0;//line
	pqx[4*(NJD-2)*6+3]=0;
	pqx[4*(NJD-2)*6+4]=0;
	pqx[4*(NJD-2)*6+5]=0;
	/*
	FILE *fp;
	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\����\\pqx.txt",L"w+");
	for(i=0;i<(4*(NJD-2)+1);i++)
	{	
	fwprintf(fp,L"%lf,%lf,%lf,%lf,%lf,%lf\n",pqx[6*i],pqx[6*i+1],pqx[6*i+2],pqx[6*i+3],pqx[6*i+4],pqx[6*i+5]);
	}
	fclose(fp);*/

	return (4*(NJD-2)+1);
}

void GTZX_ROAD::VIEW_PM_ZDM(int k)
{
	// 	xlpoint PZ;
	//     ads_point pp;
	// 	int i1;
	// 	char GH[20];
	// 	double sml,eml;
	// 	char SKml[80],EKml[80],WinTitle[256];
	// 	
	// 	if(!ptriDTM->DTMHasRead)
	// 	{
	//       if (ptriDTM->Read_Dtm())
	// 	  	ads_printf(L"�ɹ�������ģ!\n");
	// 	  else
	// 	  {
	// 		 ads_printf(L"������ģʧ��!\n");
	//          return ;
	// 	  }
	// 	}
	// 
	// //	if(!ViewZdmdlg.IsWindowVisible()) ViewZdmdlg.Create(IDD_VIEWZDM);
	// 	ViewZdmdlg.ShowWindow(SW_SHOW);
	// //	acedAlert(L"view_pm_zdm");
	// 	ViewZdmdlg.Hmax = -999999999;   
	// 	ViewZdmdlg.Hmin = 99999999999;   
	// 	PZ.lc = QXB[k-1][0];//zh.lc
	// 	i1 = 0;
	// 	double z;
	// 	while ( PZ.lc  < QXB[k+1][3] )//hz.lc
	// 	{
	// 		xlpoint_pz(array,tarray,&PZ) ;
	// 		pp[0] = PZ.x ;
	// 		pp[1] = PZ.y ;
	// 		z=ptriDTM->interpolate(pp);
	// 
	// 	//	if(ViewZdmdlg.EarthPtNew) {delete []ViewZdmdlg.EarthPtNew;ViewZdmdlg.EarthPtNew=NULL;}
	// 	//	ViewZdmdlg.EarthPtNew = new double[(int)((QXB[k+1][3]-QXB[k-1][0])/310.0)];
	// 
	// 		ViewZdmdlg.EarthPtNew[i1*2] = PZ.lc;
	// 		ViewZdmdlg.EarthPtNew[i1*2+1] = z; 
	// 	//	acutPrintf(L"i=%d,lc=%lf,z=%lf\n",i1,PZ.lc,z);
	// 		ViewZdmdlg.Hmax =   ViewZdmdlg.Hmax > z ?   ViewZdmdlg.Hmax:z;
	// 		ViewZdmdlg.Hmin =   ViewZdmdlg.Hmin < z ?   ViewZdmdlg.Hmin:z;					  
	// 		i1++;
	// 
	// 		PZ.lc +=(QXB[k+1][3]-QXB[k-1][0])/300.0;
	// 	//	ads_printf(L"lc=%lf\n",PZ.lc);
	// 	}
	// //	acedAlert(L"VIEW_PM_ZDM");
	// 	ViewZdmdlg.Len=(QXB[k+1][3]-QXB[k-1][0]);
	// 	ViewZdmdlg.NPTNew = i1;
	// 	ViewZdmdlg.InvalidateRect(ViewZdmdlg.m_sht);////ppp	
	// 	sml=QXB[k-1][0] , eml=QXB[k+1][3];
	// 	sml = XLC(sml,GH,NDL-1);
	//     _tcscpy(SKml,LCchr(GH,sml,2,1));
	// 	eml = XLC(eml,GH,NDL-1);
	// 	_tcscpy(EKml,LCchr(GH,eml,2,1));
	//     _stprintf(WinTitle,L"%s - %s���ݶ���",SKml,EKml); 
	// 	ViewZdmdlg.SetWindowText(WinTitle);
}

void GTZX_ROAD::VIEW_PM_PQX(int k)
{
	// 	int j=0;
	// 	for(int i=k-1;i<=k+1;i++)
	// 	{
	// 		//line
	// 		ViewZdmdlg.pPqx[j].slc=QXB[i-1][3];
	// 		ViewZdmdlg.pPqx[j].l=QXB[i][0]-QXB[i-1][3];
	// 		ViewZdmdlg.pPqx[j].zhyflag=0;//line
	// 		ViewZdmdlg.pPqx[j].lrflag=0;
	// 		ViewZdmdlg.pPqx[j].a=0;
	// 		ViewZdmdlg.pPqx[j].R=0;
	// 		ViewZdmdlg.pPqx[j].iJD=i+JD0;
	// 		j++;
	// 		//spire
	// 		ViewZdmdlg.pPqx[j].slc=QXB[i][0];
	// 		ViewZdmdlg.pPqx[j].l=QXB[i][1]-QXB[i][0];
	// 		ViewZdmdlg.pPqx[j].zhyflag=1;//spire
	// 		ViewZdmdlg.pPqx[j].lrflag=QXB[i][8];//lift-1 right1
	// 		ViewZdmdlg.pPqx[j].a=0;
	// 		ViewZdmdlg.pPqx[j].R=0;
	// 		ViewZdmdlg.pPqx[j].iJD=i+JD0;
	// 		j++;
	// 		//circle
	// 		ViewZdmdlg.pPqx[j].slc=QXB[i][1];
	// 		ViewZdmdlg.pPqx[j].l=QXB[i][2]-QXB[i][1];
	// 		ViewZdmdlg.pPqx[j].zhyflag=2;//circle
	// 		ViewZdmdlg.pPqx[j].lrflag=QXB[i][8];//lift-1 right1
	// 		ViewZdmdlg.pPqx[j].a=QXB[i][4];
	// 		ViewZdmdlg.pPqx[j].R=JDarray[i][4];
	// 		ViewZdmdlg.pPqx[j].iJD=i+JD0;
	// 		j++;
	// 		//spire
	// 		ViewZdmdlg.pPqx[j].slc=QXB[i][2];
	// 		ViewZdmdlg.pPqx[j].l=QXB[i][3]-QXB[i][2];
	// 		ViewZdmdlg.pPqx[j].zhyflag=-1;//spire
	// 		ViewZdmdlg.pPqx[j].lrflag=QXB[i][8];//lift-1 right1
	// 		ViewZdmdlg.pPqx[j].a=0;
	// 		ViewZdmdlg.pPqx[j].R=0;
	// 		ViewZdmdlg.pPqx[j].iJD=i+JD0;
	// 		j++;
	// 	}
	// 	//line
	// 	ViewZdmdlg.pPqx[j].slc=QXB[i-1][3];
	// 	ViewZdmdlg.pPqx[j].l=QXB[i][0]-QXB[i-1][3];
	// 	ViewZdmdlg.pPqx[j].zhyflag=0;//line
	// 	ViewZdmdlg.pPqx[j].lrflag=0;
	// 	ViewZdmdlg.pPqx[j].a=0;
	// 	ViewZdmdlg.pPqx[j].R=0;
	// 	ViewZdmdlg.pPqx[j].iJD=i-1+JD0;
}

void GTZX_ROAD::VIEW_PM_TypeHdm()
{
}

//void  GTZX::FormLCB(double dlc,double sml,double eml)  
////  ��ע���������̱�  
//{    
//	 double  BZlc,lc,LCB,DDlength,DL;
//	  int BZW,iDL;
//	   long int zlc;
//	   
//     NLCB=0;
//  	 LCB=DLB[0].BLC+DLB[0].DL;
//     DDlength = LCB;
//
//	 if(LCB<sml-0.01)
//	 {
//	 	  pLCB[NLCB].lc=sml;
//    	  pLCB[NLCB].lcflag=-1;//���
//	      NLCB++;		 
//	 }
//	 else if(LCB<eml+0.001)
//	 {
//	 	  pLCB[NLCB].lc=LCB;
//    	  pLCB[NLCB].lcflag=-1;//���
//	      NLCB++;		 
//	 }
//	 
//	 // ��������α�ע
//	 for (iDL=0;iDL<NDL;iDL++)
//	 { 
//	   DL=DLB[iDL].DL;
//	   LCB= DLB[iDL].BLC+DL;  //���������� 
//       // ������� ��ںŸı��
//	   if (iDL>0 && iDL< NDL-1)  // ��Ϊ�� �յ�
//	   { // ��� iDL��
//         lc=DDlength;
//		 if(lc>eml+0.001)
//		    break;
//		 else if(lc>sml-0.001)
//		 {
//		   pLCB[NLCB].lc=lc;
//		   if(fmod(DLB[iDL].BLC,1000)<0.01)//�������ֳ���������ﴦ
//			   pLCB[NLCB].lcflag=10;
//		   else
//		   {
//			   pLCB[NLCB].lcflag = -99;//��������׮
//			   if(fmod((DLB[iDL].BLC-floor(DLB[iDL].BLC/1000)*1000),100)<0.01) //�������ֳ���������״�
//           pLCB[NLCB].lcflag=floor((DLB[iDL].BLC-floor(DLB[iDL].BLC/1000)*1000)/100+0.01);
//		   }
//		   NLCB++;		
//		 }
//	   };	  
//       zlc=(long int)(LCB/dlc);
//	   BZlc=zlc*dlc;  // ʵ����� 
//	   if ((BZlc-LCB)<0.0001)  { BZlc=BZlc+dlc;  }  
//		BZW=99;
//	   while(BZlc<DLB[iDL+1].BLC)
//	    {
//		 // ͳһ���
//		 lc=DDlength+BZlc-LCB; 
//		 if(lc>eml+0.001)
//			 break;
//         else if(lc>sml-0.001)
//		 {
//		   pLCB[NLCB].lc=lc;
//		   if(fmod(BZlc,1000)<0.01)//�ֳ���������ﴦ
//			//   pLCB[NLCB].lcflag=-1;
//			   pLCB[NLCB].lcflag=10;//2003.12.26
//		   else
//			   pLCB[NLCB].lcflag=floor((BZlc-floor(BZlc/1000)*1000)/100+0.01);
//		   NLCB++;		
//		 }
//		 BZlc=BZlc+dlc;		 
//	   } 
//	   if(iDL<NDL-1)
//	   {
//	      DDlength=DDlength+DLB[iDL+1].BLC-LCB;//������ͳһ��̡�
//		  lc=DDlength;
//	   }
//
//	 } // IDL
//     if(lc>eml)
//	 {
//		 pLCB[NLCB].lc=eml;			 
//		 pLCB[NLCB].lcflag=-1;
//		 if(pLCB[NLCB].lc-pLCB[NLCB-1].lc > 0.001)
//		         NLCB++;		
//	 }
//	 else if(lc>sml-0.001) // �յ�
//	 {
//		   pLCB[NLCB].lc=lc;			 
//		   pLCB[NLCB].lcflag=-1;
//		   NLCB++;		
//	 }
////	 ads_printf(L"++++++++++++++ sml=%lf eml=%lf %d\n",sml,eml,NLCB);
////ͳһ���ת�ֳ����
//	 char gh[8],ch[80] ;
//	 double xlc;
//	 for(int kk=0;kk<NLCB;kk++)
//	 {
//		 xlc=XLC(pLCB[kk].lc,gh,NDL);
//		 _tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(gh,xlc,2,1));
//	 }
//	 
// }


void  GTZX_ROAD::FormLCB(double dlc,double sml,double eml)  
	//  ��ע���������̱�  
{    
	double  BZlc,lc,Tylc,DDlength,DL;
	int BZW,iDL;
	long int zlc;

	NLCB=0;
	Tylc=DLB[0].BLC+DLB[0].DL;
	DDlength = Tylc;
	LCB TempLCB;
	pLCB.RemoveAll();

	if(Tylc<sml-0.01)
	{
		TempLCB.lc = sml;
		TempLCB.lcflag = -1;
		pLCB.Add(TempLCB);
		NLCB++;		 
	}
	else if(Tylc<eml+0.001)
	{
		TempLCB.lc = Tylc;
		TempLCB.lcflag = -1;
		pLCB.Add(TempLCB);
		NLCB++;		 
	}

	// ��������α�ע
	for (iDL=0;iDL<NDL;iDL++)
	{ 
		DL=DLB[iDL].DL;
		Tylc= DLB[iDL].BLC+DL;  //���������� 
		// ������� ��ںŸı��
		if (iDL>0 && iDL< NDL-1)  // ��Ϊ�� �յ�
		{ // ��� iDL��
			lc=DDlength;
			if(lc>eml+0.001)
				break;
			else if(lc>sml-0.001)
			{
				TempLCB.lc=lc;
				if(fmod(DLB[iDL].BLC,1000)<0.01)//�������ֳ���������ﴦ
					TempLCB.lcflag=10;
				else
				{
					TempLCB.lcflag = -99;//��������׮
					if(fmod((DLB[iDL].BLC-floor(DLB[iDL].BLC/1000)*1000),100)<0.01) //�������ֳ���������״�
						TempLCB.lcflag=floor((DLB[iDL].BLC-floor(DLB[iDL].BLC/1000)*1000)/100+0.01);
				}
				pLCB.Add(TempLCB);
				NLCB++;		
			}
		};	  
		zlc=(long int)(Tylc/dlc);
		BZlc=zlc*dlc;  // ʵ����� 
		if ((BZlc-Tylc)<0.0001)  { BZlc=BZlc+dlc;  }  
		BZW=99;
		while(iDL<NDL-1 && BZlc<DLB[iDL+1].BLC)
		{
			// ͳһ���
			lc=DDlength+BZlc-Tylc; 
			if(lc>eml+0.001)
				break;
			else if(lc>sml-0.001)
			{
				TempLCB.lc = lc;
				if(fmod(BZlc,1000)<0.01)//�ֳ���������ﴦ
					//   pLCB[NLCB].lcflag=-1;
						TempLCB.lcflag=10;//2003.12.26
				else
					TempLCB.lcflag=floor((BZlc-floor(BZlc/1000)*1000)/100+0.01);
				pLCB.Add(TempLCB);
				NLCB++;		
			}
			BZlc=BZlc+dlc;		 
		} 
		if(iDL<NDL-1)
		{
			DDlength=DDlength+DLB[iDL+1].BLC-Tylc;//������ͳһ��̡�
			lc=DDlength;
		}

	} // IDL
	if(lc>eml)
	{
		TempLCB.lc = eml;
		TempLCB.lcflag = -1;
		if(pLCB[NLCB].lc-pLCB[NLCB-1].lc > 0.001)
		{
			pLCB.Add(TempLCB);
			NLCB++;
		}
	}
	else if(lc>sml-0.001) // �յ�
	{
		TempLCB.lc = lc;
		TempLCB.lcflag = -1;
		pLCB.Add(TempLCB);
		NLCB++;
	}
}

void  GTZX_ROAD::AddTZDtoLCB() //�����������������̱� 
{
	double startLC,endLC;
	double *LCB;

	LCB = new double[NLCB+5*NJD];

	startLC=pLCB[0].lc;
	endLC=pLCB[NLCB-1].lc;
	int NUM=0;
	int i=0,j=0,k=0;
	double lc=startLC;
	double lc1,lc2;
	NUM = 0;
	LCB[NUM]=pLCB[0].lc;
	NUM++;

	j = 1;
	k = 0;


	while(j<NJD&&QXB[j][k]<pLCB[0].lc)//��ʼ����j,k
	{
		k++;
		if(k==4)
		{j++;k=0;}
	}
	if(j==NJD-1)
		return;

	for(i=1; i<NLCB; i++)//�Ե�������ѭ��
	{
		lc1 = pLCB[i].lc;
		if(j<NJD-1)
			lc2 = QXB[j][k];
		else
			lc2 =1.0E12;

		if(lc1 < lc2)
			LCB[NUM]=lc1;
		else
		{
			LCB[NUM]=lc2;
			k++;
			if(k==4)
			{j++;k=0;}
			if(j<NJD)
				i--;
		}
		if(fabs(LCB[NUM]-LCB[NUM-1])>0.001) NUM++;		
	}

	NLCB = NUM;
	pLCB.RemoveAll();
	pLCB.SetSize(NLCB);
	for(i=0; i<NUM; i++)
	{
		pLCB[i].lc=LCB[i];
	}

	if(LCB != NULL)
	{
		delete []LCB;
		LCB = NULL;
	}

}

void GTZX_ROAD::AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT) //������վ������̱� 
{
	double startLC,endLC;
	LCB *Lcb;

	if(NBTS<1)
		return;
	Lcb = new LCB[NLCB+2*NBTS];

	startLC=pLCB[0].lc;
	endLC=pLCB[NLCB-1].lc;
	int NUM=0;
	int i=0,j=0,k=0;
	double lc=startLC;
	double lc1,lc2;
	NUM = 0;
	Lcb[NUM].lc=pLCB[0].lc;
	Lcb[NUM].lcflag=pLCB[0].lcflag;
	NUM++;

	j = 0;
	k = 0;

	while(j<NBTS && BTS[j][k]<pLCB[0].lc)//��ʼ����j,k
	{
		k++;
		if(k==2)
		{
			j++;
			k=0;
		}
	}
	if(j==NBTS)
		return;

	for(i=1; i<NLCB; i++)//�Ե�������ѭ��
	{
		lc1 = pLCB[i].lc;
		if(j<NBTS)
			lc2 = BTS[j][k];
		else
			lc2 =1.0E12;

		if(lc1 < lc2)
		{
			Lcb[NUM].lc=lc1;
			Lcb[NUM].lcflag=pLCB[i].lcflag;
		}
		else if (fabs(BTS[j][0] - BTS[j][1]) < 0.01)
		{
			Lcb[NUM].lc=lc1;
			Lcb[NUM].lcflag=pLCB[i].lcflag;
			j++;
			k = 0;
		}
		else
		{
			Lcb[NUM].lc=lc2;
			if(BorT=='B')//��
			{
				if(k==0)
					Lcb[NUM].lcflag = -3;
				else
					Lcb[NUM].lcflag = 3;
			}
			else if(BorT=='T')//��
			{
				if(k==0)
					Lcb[NUM].lcflag = -4;
				else
					Lcb[NUM].lcflag = 4;		
			}
			else if(BorT=='S')//վ
			{
				if(k==0)
					Lcb[NUM].lcflag =-9;
				else
					Lcb[NUM].lcflag = 9;
			}
			else//���жε��������
			{
				if(k==0)
					Lcb[NUM].lcflag =-10;
				else
					Lcb[NUM].lcflag = 10;
			}
			k++;
			if(k==2)
			{j++;k=0;}
			if(j<NBTS)
				i--;
		}
		if(fabs(Lcb[NUM].lc-Lcb[NUM-1].lc)>0.001)
			NUM++;		
		else
			Lcb[NUM-1].lcflag = Lcb[NUM].lcflag;
	}

	NLCB = NUM;
	pLCB.RemoveAll();
	pLCB.SetSize(NLCB);
	for(i=0; i<NUM; i++)
	{
		pLCB[i].lc=Lcb[i].lc;
		pLCB[i].lcflag=Lcb[i].lcflag;
	}

	if(Lcb != NULL)
	{
		delete []Lcb;
		Lcb = NULL;
	}
}


void GTZX_ROAD::checkpm()
{
	// 	double ljia;
	// 	CString ERR;
	// 	int NERR=0;
	// 	FILE *fpw;
	// 	char fname[256];
	// 
	//     FindZXLenMin();//�ҳ���С��ֱ�߳�
	//     if(!ReadWorkdir())
	// 	{
	// 		ads_alert(L"����ָ����ȷ�Ĺ���·��!");
	// 		return ;
	// 	}
	//     _stprintf(fname,L"%s\\data\\Err",Cworkdir);
	// 	fpw = _wfopen(fname,L"w");
	// 	if(fpw==NULL)
	// 	{
	// 		ads_printf(L"���ļ�%sʧ��!\n",fname);
	// 		return;
	// 	}
	// 	fwprintf(fpw,L"%10d\n",NERR);
	// 	for(int i=2;i<NJD-1;i++)
	// 	{
	// 		ljia = QXB[i][0] - QXB[i-1][3];//��ֱ��
	// 		if(ljia<ZXLmin1)
	// 		{
	// 			xlpoint PZ;
	// 			TrsProjmltoNE(QXB[i][0], PZ);
	// 			ERR.Format(L"JD%d-JD%d��ֱ�߳�%0.2lfС����С��ֱ�߳�(һ��%0.2lf,����%0.2lf))",i-1,i,ljia,ZXLmin1,ZXLmin2);
	// 			fwprintf(fpw,L"%lf %s %.3f %.3f\n",0.5*(QXB[i][0] + QXB[i-1][3]),ERR,PZ.x,PZ.y);
	// 		//	fwprintf(fpw,L"%s\n",ERR);
	// 			NERR++;
	// 		}
	// 	}
	// 	for(i=1;i<NJD-1;i++)
	// 	{
	// 		ljia = QXB[i][2] - QXB[i][1];//��Բ��
	// 		if(ljia<20.0)
	// 		{
	// 			xlpoint PZ;
	// 			TrsProjmltoNE(QXB[i][2], PZ);
	// 			ERR.Format(L"JD%d��Բ���߳�%0.2lfС��20m",i,ljia);
	// 			fwprintf(fpw,L"%lf %s %.3f %.3f\n",0.5*(QXB[i][2] + QXB[i][1]),ERR,PZ.x,PZ.y);
	// 			NERR++;
	// 		}
	// 	}
	// 	for(i=1;i<NJD-1;i++)//��С���߰뾶
	// 	{		
	// 		if(JDarray[i][4]<Rmin1)
	// 		{
	// 			ERR.Format(L"JD%d�����߰뾶%0.2lfС����С���߰뾶(һ��:%0.0lf,����:%0.2lf)",i,JDarray[i][4],Rmin1,Rmin2);
	// 			fwprintf(fpw,L"%lf %s %.3f %.3f\n",0.5*(QXB[i][2] + QXB[i][1]),ERR,JDarray[1],JDarray[2]);
	// 			NERR++;
	// 		}
	// 	}
	//    // //////////////////////////////////////////////////////////////�ཻ���
	// 	double temp = 0.0;
	//    for (int i =0;i <m_ConstraintErrArray.GetSize();i++ )
	//    {  
	// 	   //m_ConstraintErrArray[i].JiaJiao = false( m_ConstraintErrArray[i].CurFwj - m_ConstraintErrArray[i].InterFwj);
	// 	   if ( m_ConstraintErrArray[i].JiaJiao < m_ConstraintErrArray[i].MinAng)
	// 	   {
	// 		   xlpoint PZ;
	// 		   TrsCkmltoNE(m_ConstraintErrArray[i].CurCkml,  PZ);
	// 		   ERR.Format(L"%s����·����%0.3lfС����С�ཻ�Ƕ�(%0.3lf)",m_ConstraintErrArray[i].CurCkml,m_ConstraintErrArray[i].JiaJiao,m_ConstraintErrArray[i].MinAng);
	// 		   fwprintf(fpw,L"%lf %s %.3f %.3f\n",0.5*(temp),ERR,PZ.x,PZ.y);
	// 		   NERR++;
	// 	   }
	// 
	//    }
	//    for (int i =0;i <m_ConstraintErrArray.GetSize();i++)
	//    {
	// 	   m_ConstraintErrArray[i].DetaH = m_ConstraintErrArray[i].CurDesH - m_ConstraintErrArray[i].InterDesH;
	// 	   if ((m_ConstraintErrArray[i].MinDetaH>0.0 && m_ConstraintErrArray[i].DetaH < m_ConstraintErrArray[i].MinDetaH)
	// 		   || (m_ConstraintErrArray[i].MinDetaH<0.0 && m_ConstraintErrArray[i].DetaH > m_ConstraintErrArray[i].MinDetaH) )
	// 	   {
	// 		   xlpoint PZ;
	// 		   TrsCkmltoNE(m_ConstraintErrArray[i].CurCkml,  PZ);
	// 		   ERR.Format(L"%s����·�߲�%0.3lfС����С�߲�(%0.3lf)",m_ConstraintErrArray[i].CurCkml,m_ConstraintErrArray[i].DetaH,m_ConstraintErrArray[i].MinDetaH);
	// 		   fwprintf(fpw,L"%lf %s %.3f %.3f\n",0.5*(temp),ERR,PZ.x,PZ.y);
	// 		   NERR++;
	// 	   }
	//    }
	// 
	// 	////////////////////////////////////////////////////////////
	// 
	// 	rewind(fpw);
	// 	fwprintf(fpw,L"%10d\n",NERR);
	// 	fclose(fpw);
	// 	if(NERR<1)
	// 	{
	// 		ads_printf(L"ͨ����ֱ�ߣ���Բ�ߣ���С���߰뾶���ཻ���!\n");
	// 		return;
	// 	}
	// 	ERRdlg = new ERRMESDlg(NULL);
	// 	ERRdlg->Create(IDD_DIALOG_ERR);
	// 	ERRdlg->ShowWindow(SW_SHOW);

}

void GTZX_ROAD::FindZXLenMin()//�ҳ���С��ֱ�߳�
{
	//	if(DV<75)//�ط���·
	//	{
	//		if(TLDJ=="�ط�I��")
	//		{ ZXLmin1 = 50 ,ZXLmin1 = 25/*,Rmin1=600,Rmin2=350*/;}
	//		else if(TLDJ=="�ط�II��")
	//		{ ZXLmin1 = 45 ,ZXLmin1 = 20/*,Rmin1=350,Rmin2=300*/;}
	//		else
	//		{ ZXLmin1 = 40 ,ZXLmin1 = 20/*,Rmin1=250,Rmin2=200*/;}
	//	}
	//	else if(DV<145)
	//	{
	//		if(DV>139)
	//		{ ZXLmin1 = 110 ,ZXLmin1 = 70;}
	//		else if(DV>119)
	//		{ ZXLmin1 = 80 ,ZXLmin1 = 50;}
	//		else if(DV>99)
	//		{ ZXLmin1 = 60 ,ZXLmin1 = 40;}
	//		else
	//		{ ZXLmin1 = 50 ,ZXLmin1 = 30;}				
	//	}
	//	else if(DV<161)
	//		{ ZXLmin1 = 120 ,ZXLmin1 = 80/*,Rmin1=2000,Rmin2=1600*/;}
	//	else if(DV<201)
	//		{ ZXLmin1 = 140 ,ZXLmin1 = 100/*,Rmin1=2800,Rmin2=2200*/;}
	//	else if(DV<251)
	//		{ ZXLmin1 = 180 ,ZXLmin1 = 130/*,Rmin1=4500,Rmin2=3500*/;}
	//	else if(DV>299)
	//		{ ZXLmin1 = 280 ,ZXLmin1 = 210/*,Rmin1=7000,Rmin2=5500*/;}

	ZXLmin1 = mGuiFanCS.MinZhi;
}



///////////////////////////////////plh
void GTZX_ROAD::ADDJD(double N, double E)
{
	assertWriteEnabled();
	NJD++;//����һ������
	JDarray[NJD-1][0]=NJD-1;
	JDarray[NJD-1][1]=N;
	JDarray[NJD-1][2]=E;
	JDarray[NJD-1][3]=0.0;
	JDarray[NJD-1][4]=0.0;
	JDarray[NJD-1][5]=0.0;
}

void GTZX_ROAD::MODIFYJD(int JDNO,double N, double E, double L1, double R, double L2)
	//ָ��������ţ��޸������
{
	assertWriteEnabled();
	JDarray[JDNO][1]=N;
	JDarray[JDNO][2]=E;
	JDarray[JDNO][3]=L1;
	JDarray[JDNO][4]=R;
	JDarray[JDNO][5]=L2;	
}


int GTZX_ROAD::GetPreJD(double N, double E)
{
	double tylc=PROJ_ML(array,tarray,N,E,N,E);
	//	acutPrintf(L"tylc=%lf,N=%lf,E=%lf,NJD=%d\n",tylc,N,E,NJD);
	return DistToNearJD(tylc,true);
}

int GTZX_ROAD::DistToNearJD(double tylc, bool IsForwardTo)
	//����ͳһ��̣��Ƿ���ǰ�����������븽���Ľ����
{
	int iJD=-1;
	for(int i=0;i<NJD;i++)
	{
		if(tylc<=0.5*(QXB[i][0]+QXB[i][3])+0.000001)
		{			
			iJD=i;
			break;
		}
	}
	if(IsForwardTo)//��ǰ����
	{
		iJD--;
	}
	else//�������
	{
	}
	//	acutPrintf(L"tylc=%lf,iJD=%d %lf %lf\n",tylc,iJD,QXB[NJD-1][0],QXB[NJD-1][3]);
	return iJD;	
}

double GTZX_ROAD::LocateHundredMeterOnLine(double tylc1x,bool IsForwardTo)
	//����ͳһ��̣��Ƿ���ǰѰ�Ҹ����������ױ꣬Ҫ����ֱ����,
{
	double gettylc=tylc1x;
	double lc0;//��׮
	int iXY=0;//����������Ԫ
	int XYClass;//��Ԫ����
	ACHAR GH[20];//��̹ں�
	double xlc;//һ����ͶӰ�ֳ����
	int i;
	for(i=tarray;i>0;i--)
	{
		if(tylc1x>=array[i][6])
		{
			iXY=i;
			break;
		}
	}
	if(i<=0)return -1.0;//plh06.03
	XYClass=array[iXY][0];//ȡ����Ԫ����
	if(IsForwardTo)//�����ǰ����
	{
		if(XYClass==4)//�󻺣���ǰ�ƶ�һ����Ԫ
		{
			iXY--;
			XYClass=array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==2)//Բ����ǰ�ƶ�һ����Ԫ
		{
			iXY--;
			XYClass=array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==3)//ǰ������ǰ�ƶ�һ����Ԫ
		{
			iXY--;
		}
		//��ʱ��Ԫ��ֱ��
		gettylc=array[iXY+1][6];//һ��ֱ�������
		if(tylc1x<gettylc)gettylc=tylc1x;//�״ζ�λ��ֱ���ϣ�
		xlc=XLC1(gettylc,GH,NDL,DLB);
		lc0=xlc-100*floor(xlc/100+0.00001);
		if(lc0<array[iXY+1][6]-array[iXY][6]&&DistToNearDL(gettylc,true)>lc0)//�������Ը���Ԫ,�ҵ���ǰ��λ��֮��û�ж���
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
			XYClass=array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==2)//Բ������ƶ�һ����Ԫ
		{
			iXY++;
			XYClass=array[iXY][0];//ȡ����Ԫ����
		}
		if(XYClass==4)//ǰ��������ƶ�һ����Ԫ
		{
			iXY++;
		}
		//��ʱ��Ԫ��ֱ��
		gettylc=array[iXY][6];//ֱ�����
		if(tylc1x>gettylc)gettylc=tylc1x;//�״ζ�λ��ֱ���ϣ�		
		xlc=XLC1(gettylc,GH,NDL,DLB);
		lc0=xlc-100*floor(xlc/100+0.00001);
		lc0=100.0-lc0;
		if(lc0<array[iXY+1][6]-array[iXY][6]&&fabs(DistToNearDL(gettylc,false))>lc0)//�������Ը���Ԫ
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

double GTZX_ROAD::DistToNearDL(double tylc,bool IsForwardTo=true)//
	//����ͳһ��̣��Ƿ���ǰ�����������븽��������ͳһ��̵Ĳ�ֵ��tylc>������lc...+
{
	double LCB=0.0;//������ͳһ���
	int iDL=0;
	double iXLlength=0.0;//ĳ�γ���
	for(iDL=1;iDL<NDL;iDL++)
	{
		if(tylc<=LCB)break;
		iXLlength=DLB[iDL].BLC-(DLB[iDL-1].BLC+DLB[iDL-1].DL);//ĳ�γ���
		LCB+=iXLlength;//������ͳһ���
	}
	if(IsForwardTo)//��ǰ����
	{
		LCB-=iXLlength;
	}
	else//�������
	{
	}
	return tylc-LCB;	
}

void GTZX_ROAD::Invert()//plh07.02
{
	assertWriteEnabled();
	double a[6];
	int i,k;
	for(i=0;i<NJD/2;i++)
	{
		for(k=1;k<6;k++)
		{
			a[k]=JDarray[i][k];
			JDarray[i][k]=JDarray[NJD-1-i][k];
			JDarray[NJD-1-i][k]=a[k];
		}
	}

	double ENDCML;
	PDTAB myPDTAB;
	int sum=m_BPDArray.GetSize()-1;
	CArray<BAS_DRAW_FUN_RAIL::xlpoint,BAS_DRAW_FUN_RAIL::xlpoint> mypointarray;
	BAS_DRAW_FUN_RAIL::xlpoint mypt;
	for (i=0;i<m_BPDArray.GetSize();i++)
	{
		TrsCmltoNE(m_BPDArray[i].cml,mypt);
		mypointarray.Add(mypt);
		//m_BPDArray[i].cml=ENDCML-m_BPDArray[i].cml;
		//TrsCmltoCkml(m_BPDArray[i].cml,m_BPDArray[i].CKml,3);
		//char myGH[16];
		//split_ckml(m_BPDArray[i].CKml,&m_BPDArray[i].ml,myGH);
		//m_BPDArray[i].GH.Format(L"%s",myGH);
		//myPDTAB=m_BPDArray[sum-i];
		//m_BPDArray[sum-i]=m_BPDArray[i];
		//m_BPDArray[i]=myPDTAB;
	}
	getpxy_DATA();
	for (i=0;i<m_BPDArray.GetSize()/2;i++)
	{
		myPDTAB=m_BPDArray[sum-i];
		m_BPDArray[sum-i]=m_BPDArray[i];
		m_BPDArray[i]=myPDTAB;
	}
	for(i=0;i<m_BPDArray.GetSize();i++)
	{
		TrsNEtoCkml(mypointarray[sum-i].x,mypointarray[sum-i].y,m_BPDArray[i].CKml);
		CString CKML;
		CKML.Format(L"%s",m_BPDArray[i].CKml);
		double x=mypointarray[sum-i].x;
		double y=mypointarray[sum-i].y;
		//TrsCmltoCkml(m_BPDArray[i].cml,m_BPDArray[i].CKml,3);
		ACHAR myGH[16];
		split_ckml(m_BPDArray[i].CKml,&m_BPDArray[i].ml,myGH);
		m_BPDArray[i].GH.Format(L"%s",myGH);
	}
	for (i=0;i<m_BPDArray.GetSize();i++)
	{
		ads_printf(L"cml=%s\n",m_BPDArray[i].CKml);
	}



}

//plh07.17
void GTZX_ROAD::GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2)
{
	int i=GetPreJD(N,E);
	N1=JDarray[i][1];
	E1=JDarray[i][2];
	N2=JDarray[i+1][1];
	E2=JDarray[i+1][2];
}

//����cml���ĸ߳�,�¶�
double GTZX_ROAD::rdes_level(double cml,double &slope)
	/* ml; continue mileage  */
{
	slope = -100.0;
	if (m_NCM==0)
	{
		slope = 0.0;
		return -100.0;
	}
	ACHAR       str[80];
	int   j;
	double     z=-1.0,t1,A,B,C,delta_i,z2,x,sml,eml;
	//--------------------------------------------------------
	sml = m_CM[0];//��ʼ�������
	eml =  m_CM[(m_NCM-2)*5];

	str[0]='\0';
	if (cml<sml-0.001 || cml>eml+0.01) {
		//    ads_printf(L"%d %d ML=%0.2lf out of range(%0.3lf---%0.3lf)\n",NBPD,NCM,cml,sml,eml);
		//   ads_alert(str);
		return -100.0;
	}
	else if ( cml<m_CM[0]) 
	{
		z = m_CM[1]-m_CM[2]*(m_CM[0]-cml);
		slope = m_CM[2];
		//     z+= DLEVEL(0, cml,NCM,CM); 
	}
	else if (cml>m_CM[(m_NCM-2)*5+0] ) {
		z= m_CM[(m_NCM-2)*5+1]+m_CM[(m_NCM-1)*5+2]*(cml - m_CM[(m_NCM-2)*5+0]);
		slope = m_CM[(m_NCM-1)*5+2];
		//    z+=DLEVEL(NCM-1,cml,NCM,CM);
	}
	else{
		for(j=1; j<m_NCM-1; j++)
			if (m_CM[(j-1)*5+0]-0.0001<=cml&&cml<=m_CM[j*5+0]+0.0001)
			{
				slope = m_CM[j*5+2];
				if(m_CM[(j-1)*5+4]<0.0)//������
				{
					t1 = -1.0*m_CM[(j-1)*5+4];
					if(cml < m_CM[(j-1)*5]+t1)//���������߷�Χ��
					{
						z = m_CM[(j-1)*5+1]-m_CM[(j-1)*5+2]*t1;//���������߳�
						C = z;
						delta_i = m_CM[j*5+2]-m_CM[(j-1)*5+2];
						A = delta_i/4.0/t1;//�������¶ȱ仯��
						z2 = m_CM[(j-1)*5+1]+m_CM[j*5+2]*t1;//�������յ�߳�
						B = ((z2-z)-t1*delta_i)/2.0/t1;
						x = cml - m_CM[(j-1)*5] + t1;
						z = A*x*x + B*x + C;
						break;
					}
				}
				if(m_CM[j*5+4]<0.0)//������
				{
					t1 = -1.0*m_CM[j*5+4];
					if(cml > m_CM[j*5]-t1)//���������߷�Χ��
					{
						z = m_CM[j*5+1]-m_CM[j*5+2]*t1;//���������߳�
						C = z;
						delta_i = m_CM[(j+1)*5+2]-m_CM[j*5+2];
						A = delta_i/4.0/t1;//�������¶ȱ仯��
						z2 = m_CM[j*5+1]+m_CM[(j+1)*5+2]*t1;//�������յ�߳�
						B = ((z2-z)-t1*delta_i)/2.0/t1;
						x = cml - (m_CM[j*5] - t1);
						z = A*x*x + B*x + C;
						break;
					}
				}

				z = m_CM[(j-1)*5+1]+m_CM[j*5+2]*(cml-m_CM[(j-1)*5+0]);
				z+= DLEVEL(j,cml);
				break;

			}
	}
	return (z);
}

//��CM�������cml���ĸ߳�
double GTZX_ROAD::DLEVEL(int icm, double cml)
{
	if (m_NCM==0)
		return -100.0;

	double vr1, vr2, di1,di2, vt1,vt2, ds, ds1, dds,s1,s2;
	int kr;

	if (icm==0) 
	{
		vr1=1.0e8;
		di1=0.0;
		vr2 = m_CM[icm*5+4];
		di2 = (m_CM[(icm+1)*5+2] - m_CM[icm*5+2])*100.0;
		ds=m_CM[0]-cml;
	}
	else if (icm==m_NCM-1) {
		vr1 = m_CM[(icm-1)*5+4];
		di1 = (m_CM[icm*5+2] - m_CM[(icm-1)*5+2])*100.0;
		vr2=1.0e8;
		di2=0.0;
		ds=cml-m_CM[icm*5+0];
	}
	else 
	{
		vr1 = m_CM[(icm-1)*5+4];
		vr2 = m_CM[icm*5+4];
		di1 = (m_CM[icm*5+2] - m_CM[(icm-1)*5+2])*100.0;
		di2 = (m_CM[(icm+1)*5+2] - m_CM[icm*5+2])*100.0;
		ds = cml-m_CM[(icm-1)*5+0];
	}
	sign(s1, di1);
	sign(s2, di2);
	if(vr1>=0.0)
		vt1 = fabs(vr1*di1/200.0);
	else
		vt1 = fabs(vr1);

	if(vr2>=0.0)
		vt2 = fabs(vr2*di2/200.0);
	else
		vt2 = fabs(vr2);
	//      vt2 = fabs(vr2*di2/200.0);

	if (ds<vt1) {
		ds1 = vt1-ds;
		kr  = 1;
	}
	else if (ds>m_CM[icm*5+3]-vt2) {
		ds1 = ds-m_CM[icm*5+3]+vt2;
		kr  = 2;
	}
	else  {
		ds1 = 0.0;
		kr  = 0;
	}
	if (kr==1 && vr1>0.0)
		dds = s1*ds1*ds1/2.0/vr1;
	else if (kr==2 && vr2>0.0)
		dds = s2*ds1*ds1/2.0/vr2;
	else
		dds = 0.0;
	return (dds);
}


bool GTZX_ROAD::TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml;
	double cml = PROJ_ML(array, tarray, N, E, N, E);
	dml = XLC1(cml, GH, NDL, DLB);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
	return true;
}

bool GTZX_ROAD::TrsNEtoProjml(double N, double E, double& Projml)
{
	Projml = PROJ_ML(array, tarray, N, E, N, E);
	return true;
}

bool GTZX_ROAD::TrsNEtoCml(double N, double E, double& Cml)
{
	Cml = PROJ_ML(array, tarray, N, E, N, E);
	return true;
}

bool GTZX_ROAD::TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ)
{
	double cml = TYLC1(Ckml, NDL, DLB);
	PZ.lc = cml;
	xlpoint_pz(array, tarray, &PZ);

	return true;
}
bool GTZX_ROAD::TrsCkmltoProjml(ACHAR Ckml[], double& Projml)
{
	Projml = TYLC1(Ckml, NDL, DLB);
	return true;
}


bool GTZX_ROAD::TrsCkmltoCml(ACHAR Ckml[], double& Cml)
{
	Cml = TYLC1(Ckml, NDL, DLB);
	return true;
}



bool GTZX_ROAD::TrsCmltoNE(double Cml, xlpoint& PZ)
{
	PZ.lc = Cml;
	xlpoint_pz(array, tarray, &PZ);
	return true;

}
bool GTZX_ROAD::TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml = 0.0;
	dml = XLC1(Cml, GH, NDL, DLB);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));

	return true;

}
bool GTZX_ROAD::TrsCmltoProjml(double Cml, double& Projml)
{
	Projml = Cml;

	return true;
}
bool GTZX_ROAD::TrsProjmltoNE(double Projml, xlpoint& PZ)
{
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, &PZ);

	return true;
}
bool GTZX_ROAD::TrsProjmltoCml(double Projml, double& Cml)
{
	Cml = Projml;

	return true;

}
bool GTZX_ROAD::TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml = 0.0;
	double Cml = Projml;
	dml = XLC1(Cml, GH, NDL, DLB);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));

	return true;

}

void GTZX_ROAD::SetBPDArray(int NPD, BAS_DRAW_FUN::PDTAB* BPDArr, bool IsAdd)
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

			m_BPDArray.Add(TempPDB);
		}
	}
	else
	{
		m_BPDArray.RemoveAll();
		m_BPDArray.SetSize(NPD);
		for (int i=0; i<NPD; i++)
		{
			_tcscpy(m_BPDArray[i].CKml,BPDArr[i].CKml);
			m_BPDArray[i].GH=BPDArr[i].GH;
			m_BPDArray[i].cml=BPDArr[i].cml;
			m_BPDArray[i].degree=BPDArr[i].degree;
			m_BPDArray[i].length=BPDArr[i].length;
			m_BPDArray[i].Level=BPDArr[i].Level;
			m_BPDArray[i].ml=BPDArr[i].ml;
			m_BPDArray[i].Notes=BPDArr[i].Notes;
			m_BPDArray[i].RLen=BPDArr[i].RLen;
			m_BPDArray[i].Rshu=BPDArr[i].Rshu;
		}
	}
}

void GTZX_ROAD::SetBPDArray(int NPD, PDTAB* BPDArr, bool IsAdd)
{
	if (IsAdd)
	{
		for (int i=0; i<NPD; i++)
			m_BPDArray.Add(BPDArr[i]);
	}
	else
	{
		m_BPDArray.RemoveAll();
		m_BPDArray.SetSize(NPD);
		for (int i=0; i<NPD; i++)
		{
			m_BPDArray[i] = BPDArr[i];
		}
	}
}

void GTZX_ROAD::TrsBPDArraytoCM()
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


bool GTZX_ROAD::BZCM(AcGiWorldDraw* pWd)
{
	xlpoint pt;
	AcGePoint3d cenPT,BefPt,AftPt,InterPt,InterPt2,BefSlopePt,AftSlopePt,MLPt,BefSlopePt1;
	ACHAR Ckml[20];
	ACHAR BefLen[20];
	ACHAR AftLen[20];
	ACHAR BefSlope[20];
	ACHAR AftSlope[20];
	ACHAR Lever[20];
	int LorR = -1;
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
		isBefSlopeUp=false;
		isAftSlopeUp=false;
		TrsCmltoNE(m_CM[i*5], pt);
		TrsCmltoCkml(m_CM[i*5], Ckml, m_Xsw);
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

void GTZX_ROAD::BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR)
{
	xlpoint pt;
	double YL = 10;
	double XL = 5;
	double LC;
	int Color = 1;
	AcGePoint3d cenPT,BefPt,AftPt,InterPt;
	AcGePoint3d  TBefPt,TAftPt;
	if (m_BPDArray[num].RLen<0.001)
	{
		return;
	}
	LC=m_CM[num*5]-m_BPDArray[num].RLen*0.5;
	if ((num == 0) && (LC < StartCml))//���
	{
		LC=m_CM[num*5];

	}
	TrsCmltoNE(LC, pt);
	BefPt[X] = pt.y;
	BefPt[Y] = pt.x;
	InterPt[X] = BefPt[X] + YL * sin(pt.a + LorR*0.5*PI);
	InterPt[Y] = BefPt[Y] + YL * cos(pt.a + LorR*0.5*PI);
	TBefPt[X] = InterPt[X] - XL * sin(pt.a);
	TBefPt[Y] = InterPt[Y] - XL * cos(pt.a);
	TAftPt[X] = InterPt[X] + XL * sin(pt.a);
	TAftPt[Y] = InterPt[Y] + XL * cos(pt.a);
	G_makeline(pWd, BefPt, InterPt, Color);
	G_makeline(pWd, InterPt, TBefPt, Color);
	G_makeline(pWd, InterPt, TAftPt, Color);
	LC=m_CM[num*5]+m_BPDArray[num].RLen*0.5;
	if ( (num == m_NCM-2) && (LC> EndCml))
	{
		LC=m_CM[num*5];
	}
	TrsCmltoNE(LC, pt);
	AftPt[X]=pt.y;
	AftPt[Y]=pt.x;
	InterPt[X] = AftPt[X] + YL * sin(pt.a + LorR*0.5*PI);
	InterPt[Y] = AftPt[Y] + YL * cos(pt.a + LorR*0.5*PI);
	TBefPt[X] = InterPt[X] - XL * sin(pt.a);
	TBefPt[Y] = InterPt[Y] - XL * cos(pt.a);
	TAftPt[X] = InterPt[X] + XL * sin(pt.a);
	TAftPt[Y] = InterPt[Y] + XL * cos(pt.a);
	G_makeline(pWd, AftPt, InterPt, Color);
	G_makeline(pWd, InterPt, TBefPt, Color);
	G_makeline(pWd, InterPt, TAftPt, Color);
}
void GTZX_ROAD::SetXLRelate(XLRelate& Relate)
{
	m_XLRelate = Relate;
}

void GTZX_ROAD::CheckAllXLConstraint()
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
	// for (int iXL=0; iXL<InsertXLSum; iXL++)
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

	//	if (OneXLErrorArray.GetSize()>0)
	//	{
	//		m_ConstraintErrArray.Append(OneXLErrorArray);
	//	}
	//}
}

void GTZX_ROAD::DrawXLConstraint(AcGiWorldDraw* pWd)
{
	ConstraintErr CnstErr;
	double LY = 150;
	double LX = 50;
	int LorR = -1;
	if (_tcscmp(this->isA()->name(),L"GTZX") ==0 )
	{
		LorR = -1;
	}
	else
		LorR = 1;
	int Color = 0;
	AcGePoint3d CenPt,BefPt,AftPt,CrsPt,MLPt;

	//ads_printf(L"jiaodianshu2%d\n"m_ConstraintErrArray.GetSize());
	int mynum=m_ConstraintErrArray.GetSize();
	int k=0;
	for (k=0; k<m_ConstraintErrArray.GetSize(); k++)
	{
		Color = 0;
		CnstErr = m_ConstraintErrArray[k];
		ACHAR MLStr[200];
		ACHAR CnsStr[200];
		_tcscpy(MLStr, L"");
		_tcscpy(CnsStr, L"");
		bool IsAngErr = false;
		bool IsDetaHErr = false;
		double NoteAng;
		if (CnstErr.JiaJiao<CnstErr.MinAng)
		{
			//IsAngErr=true;
			Color=1;

		}
		if (CnstErr.MinDetaH<0)
		{
			if (CnstErr.DetaH>CnstErr.MinDetaH)
			{
				//IsDetaHErr=true;
				Color=1;
			}
		}
		if(CnstErr.MinDetaH<0)
		{
			if (CnstErr.DetaH<CnstErr.MinDetaH)
			{
				//IsDetaHErr=true;
				Color=1;
			}
		}
		if ( fabs(CnstErr.CurDesH+100)<0.01||fabs(CnstErr.InterDesH+100)<0.01)
		{
			Color=1;
		}

		CnstErr = m_ConstraintErrArray[k];
		NoteAng = FwjtoAngX(CnstErr.CurFwj - pi/2.0);

		CenPt[X] = CnstErr.interE;
		CenPt[Y] = CnstErr.InterN;

		CrsPt[X] = CenPt[X] + LY * sin(CnstErr.CurFwj + LorR*0.5*PI);
		CrsPt[Y] = CenPt[Y] + LY * cos(CnstErr.CurFwj + LorR*0.5*PI);

		BefPt[X] = CrsPt[X] - LX * sin(CnstErr.CurFwj);
		BefPt[Y] = CrsPt[Y] - LX * cos(CnstErr.CurFwj);

		AftPt[X] = CrsPt[X] + LX * sin(CnstErr.CurFwj);
		AftPt[Y] = CrsPt[Y] + LX * cos(CnstErr.CurFwj);

		G_makeline(pWd, CenPt, CrsPt, Color);
		G_makeline(pWd, BefPt, AftPt, Color);

		MLPt[X] = CenPt[X] + 5.0 * sin(CnstErr.CurFwj + LorR*0.5*PI);
		MLPt[Y] = CenPt[Y] + 5.0 * cos(CnstErr.CurFwj + LorR*0.5*PI);

		NoteAng = FwjtoAngX(CnstErr.CurFwj + LorR*0.5*PI);


		_stprintf(MLStr, L"%s  H=%.3lf", CnstErr.CurCkml, CnstErr.CurDesH);
		G_maketext(pWd, MLPt, MLStr, NoteAng, texth, Color);



		_stprintf(MLStr, L"%s  H=%.3lf", CnstErr.InterCkml, CnstErr.InterDesH);
		G_maketext(pWd, MLPt, MLStr, NoteAng, texth, Color, 3);
		ACHAR Ang[200];
		_tcscpy(Ang,L"");
		dms(CnstErr.JiaJiao, 0, Ang);
		if ( fabs(CnstErr.CurDesH+100)<0.01||fabs(CnstErr.InterDesH+100)<0.01)
		{
			_stprintf(CnsStr, L"δ���ñ��µ�, ��=%s",  Ang);
		}
		else
		{
			_stprintf(CnsStr, L"��H=%.3lf, ��=%s", CnstErr.DetaH, Ang);
		}
		//_stprintf(CnsStr, L"��H=%.3lf, ��=%s", CnstErr.DetaH, Ang);
		NoteAng = FwjtoAngX(CnstErr.CurFwj);
		G_maketext(pWd, CrsPt, CnsStr, NoteAng, texth, Color, 0);
	}
}

void GTZX_ROAD::ReadXLGuifan()
{
	CString ProName =  GetProByMdbName(mdbname);
	ReadGuifanFile(ProName, mGuiFanCS);
	DV = mGuiFanCS.SuDuMuBiao;
}

//��ȡ��������
//JPXLName	������·��
//Lever	���¸߳�
//Slope	�����¶�
void GTZX_ROAD::GetJPData(CString JPXLName, bool IsStart, double& Lever, double& Slope)
{
	Lever = -100.0;
	Slope = -100.0;

	CString JXorEX;
	CString ProStr;
	double Cml = 0.0;

	JXorEX = JPXLName.Mid(JPXLName.GetLength()-8, 8);
	ProStr = JPXLName.Mid(0, JPXLName.GetLength()-8);
	JPXLName = Cworkdir + "\\DATA\\" + ProStr + ".mdb";
	double N,E;
	if (IsStart)
	{
		N=JDarray[0][1];
		E=JDarray[0][2];

	}
	else
	{
		N=JDarray[NJD-1][1];
		E=JDarray[NJD-1][2];
	}

	if (JXorEX=="���л���")
	{
		GTZX_JYX_ROAD* pXLPM = PmPointer::GetJYJX(JPXLName);
		if (pXLPM)
		{
			pXLPM->TrsNEtoCml(N,E,Cml); 
			Lever = pXLPM->rdes_level(Cml, Slope);
		}
	}
	else if (JXorEX == "��������")
	{
		GTZX2_JYX_ROAD* pXLPM = PmPointer::GetJYEX(JPXLName,-1);
		if (pXLPM)
		{
			pXLPM->TrsNEtoCml(N,E,Cml); 
			Lever = pXLPM->rdes_level(Cml, Slope);
		}
	}
	else if (JXorEX == "��������")
	{
		GTZX2_JYX_ROAD* pXLPM = PmPointer::GetJYEX(JPXLName,1);
		if (pXLPM)
		{
			pXLPM->TrsNEtoCml(N,E,Cml); 
			Lever = pXLPM->rdes_level(Cml, Slope);
		}
	}
	else if (JXorEX == "��ƻ���")
	{
		GTZX_ROAD* pXLPM = PmPointer::GetSJJX(JPXLName);
		if (pXLPM)
		{
			pXLPM->TrsNEtoCml(N,E,Cml); 
			Lever = pXLPM->rdes_level(Cml, Slope);
		}
	}
	else if (JXorEX == "�������")
	{
		GTZX2_ROAD* pXLPM = PmPointer::GetSJEX(JPXLName,-1);
		if (pXLPM)
		{
			pXLPM->TrsNEtoCml(N,E,Cml); 
			Lever = pXLPM->rdes_level(Cml, Slope);
		}
	}
	else if (JXorEX == "�������")
	{
		GTZX2_ROAD* pXLPM = PmPointer::GetSJEX(JPXLName,1);
		if (pXLPM)
		{
			pXLPM->TrsNEtoCml(N,E,Cml); 
			Lever = pXLPM->rdes_level(Cml, Slope);
		}
	}
	else
	{
		Lever = -100.0;
		Slope = -100.0;
	}
}

//ˢ���뵱ǰ��·�ཻ�������·ʵ��
void GTZX_ROAD::RefreshRelateXL()
{
	int InsertXLSum = m_NeedInterXLNum;
	if (InsertXLSum<1)
	{
		return;
	}
	CString InsertXLMdbName;
	CString JXorEX;
	CString ProStr;
	double MinAng = 0.0;
	double MinH = 0.0;
	for (int iXL=0; iXL<InsertXLSum; iXL++)
	{
		InsertXLMdbName = m_NeedInterXLNameArray[iXL];
		JXorEX = InsertXLMdbName.Mid(InsertXLMdbName.GetLength()-8, 8);
		ProStr = InsertXLMdbName.Mid(0, InsertXLMdbName.GetLength()-8);
		InsertXLMdbName = Cworkdir + "\\DATA\\" + ProStr + ".mdb";
		CArray<ConstraintErr,ConstraintErr> OneXLErrorArray;
		if (JXorEX == "��ƻ���")
		{
			int k = PmPointer::FindSJJX(InsertXLMdbName);
			if (k>-1)
			{
				if (g_GTZXPtArray[k]->isErased())
				{
					continue;
				}
				AcDbObject *pObj = NULL;
				if (acdbOpenObject(pObj, g_GTZXPtArray[k]->id(), AcDb::kForWrite)== Acad::eOk)
				{
					//g_GTZXPtArray[k] = GTZX::cast(pObj);
					g_GTZXPtArray[k]->assertWriteEnabled();
					g_GTZXPtArray[k]->isReFresh=false;
					g_GTZXPtArray[k]->close();

				}


			}
			else
				continue;
		}
		if (JXorEX == "�������" || JXorEX == "�������")
		{
			int k = PmPointer::FindSJEX(InsertXLMdbName);
			if (k>-1)
			{
				if (g_GTZX2PtArray[k]->isErased())
				{
					continue;
				}
				AcDbObject *pObj = NULL;
				if (acdbOpenObject(pObj, g_GTZX2PtArray[k]->id(), AcDb::kForWrite) == Acad::eOk )
				{
					//g_GTZXPtArray[k] = GTZX2::cast(pObj);
					g_GTZX2PtArray[k]->assertWriteEnabled();
					g_GTZX2PtArray[k]->isReFresh=false;
					g_GTZX2PtArray[k]->close();

				}


			}
			else
				continue;
		}
		else
			continue;
	}
}