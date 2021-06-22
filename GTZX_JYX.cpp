
#include "StdAfx.h"
//#include "XLDataBase_JYX.h"
//#include "BAS_DRAW_FUN_JYX.h"
#include "GTZX_JYX.h"
//#include "GTZX2_JYX.h"
//#include "XLJDFit.h"

//ACRX_DXF_DEFINE_MEMBERS(GTZX_JYX,AcDbEntity,AcDb::kDHL_CURRENT,AcDb::kMReleaseCurrent,0,GTZX_JYX,L"wlPingMian");
int MAKE_ACDBOPENOBJECT_FUNCTION(GTZX_JYX_ROAD);


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS"

#define  sign(y, x)  { if ((x) == 0)        \
	y =0;             \
	else if ((x) > 0)    \
	y =1;             \
	else                 \
	y = -1;           \
}

//用于计算贯通设计基线的既有线指针
GTZX_JYX_ROAD  *JiXianpGtzx = NULL ;
GTZX_JYX_ROAD* pGTZXArray[MAXGJD];
GTZX_JYX_ROAD* pGTEXRXGJD = NULL;
extern GTZX2_JYX_ROAD *ZuoXianpGtzx ;
extern GTZX2_JYX_ROAD *YouXianpGtzx ;
GTZX_JYX_ROAD::~GTZX_JYX_ROAD()
{
	//   lddm.DestroyWindow ();
	if (pLCB) delete pLCB; pLCB = NULL;
}

Acad::ErrorStatus  GTZX_JYX_ROAD::subTranformBy(const AcGeMatrix3d &)
{
	return Acad::eOk;
}

int GTZX_JYX_ROAD::INIT( BOOL IsGaiJian )
{  
	IsdrawTZD = true;
	IsdrawLC = true;
	IsdrawXLM = true;
	IsdrawBPD = true;
	IsdrawJDLX = true;
	IsdrawJD = true;

	AllColor = -1;
	IsEXRXD = false;
	int i=0,j;
	//	_tcscpy(m_AddJDGH,L"");
	JDarray.RemoveAll();
	QXB.RemoveAll();
	XLShuXing = "既有基线";

	NJD=0; 
	for (i=0;i<30;i++) { DLB[i].DL=0.0; DLB[i].BLC=0.0;
	_tcscpy(DLB[i].BGH,L"\0"); _tcscpy(DLB[i].EGH,L"\0");		
	};

	JD0 = 0 ;

	NDL=2;
	NH=3;
	NLC=3;
	Chinese_Engbz_type=1;
	JDTDBZ=5;
	qxbbz=1;
	draw_zybz=-1;

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

	CString ProName = BAS_DRAW_FUN_JYX::GetProByMdbName(mdbname);
	//	ReadGuifanFile(ProName, m_GuiFanCS);

	DV = m_GuiFanCS.SuDuMuBiao;

	IsReSetColor = FALSE ; 

	// 	XLDataBase xlmdb;
	// 	CString prj_fname;
	//    prj_fname.Format(L"%s\\data\\%s.mdb",Cworkdir,Pro);
	//	xlmdb.Read_XLDbs(prj_fname,L"主要技术标准",L"");
	// 	DV = xlmdb.JSBZ.SDMB;
	//     RtjMin = xlmdb.JSBZ.RtjMin;
	// 	RtjMax = xlmdb.JSBZ.RtjMax;
	// 	TLDJ = xlmdb.JSBZ.TLDJ;
	// 	XJJ =  xlmdb.JSBZ.XJJ;
	// 	ZXSM = xlmdb.JSBZ.ZXSM;
	// 	Rmin1 = xlmdb.JSBZ.Rmin1;
	// 	Rmin2 = xlmdb.JSBZ.Rmin2;
	//    ReadGuifanFile();
	// 	DV = mGuiFanCS.SuDuMuBiao;
	//     RtjMin = mGuiFanCS.TJianR1;
	// 	RtjMax = mGuiFanCS.TJianR2;
	// 	TLDJ = mGuiFanCS.TieLuDengJi;
	// 	XJJ = mGuiFanCS.XianJianJu;
	// 	if(mGuiFanCS.ZhXianShu==1)		
	// 	   ZXSM ="单线";
	// 	else if(mGuiFanCS.ZhXianShu==2)
	// 	   ZXSM ="双线";
	// 	else
	// 	   ZXSM ="预留二线";
	// 	Rmin1=mGuiFanCS.MinR1;
	// 	Rmin2=mGuiFanCS.MinR2;
	pLCB = NULL;
	NLCB = 0;
	m_BPDArray.RemoveAll();
	m_NCM = 0;
	return 1;

}

GTZX_JYX_ROAD::GTZX_JYX_ROAD()
{ 
	INIT(FALSE);
}


//ACHAR *GH,里程冠号 ， ACHAR *JDch交点冠号
GTZX_JYX_ROAD::GTZX_JYX_ROAD(int iNJD,CArray<JDarrayStruct,JDarrayStruct>& iJDarray,ACHAR *GH,ACHAR *JDchr,BOOL isConnectNewGTZXToIn , int isJXflag , BOOL IsGaiJian )
{   

	int i=0,j;
	ACHAR EGH[8]; 
	INIT(IsGaiJian);

	if(isJXflag==1)XLShuXing = "贯通设计基线";
	if(isJXflag==2)XLShuXing = "参照线";

	//是否为联络线
	isConnectNewGTZX = isConnectNewGTZXToIn;

	_tcscpy(JDGH,JDchr);

	NJD = iJDarray.GetSize();

	if(NJD<1)return;


	JDarray.RemoveAll();
	JDarray.SetSize(NJD);

	for (i=0;i<NJD;i++)
	{
		_tcscpy(JDarray[i].JDNum , iJDarray[i].JDNum);
		JDarray[i].N = iJDarray[i].N;
		JDarray[i].E = iJDarray[i].E;
		JDarray[i].l1 = iJDarray[i].l1;
		JDarray[i].R = iJDarray[i].R;
		JDarray[i].l2 = iJDarray[i].l2;
	}

	//半径与缓长的规范检测
	RTolCheckUp(IsGaiJian);

	NDL=1;  
	setDLM(0,JDarray[0].l1,0.0,GH,GH ); 

	NDL=2;
	_tcscpy(EGH,GH);//plh04.11.25
	setDLM(1,950,250,GH,GH );//plh04.11.25 	  //


	//  根据NJD个交点坐标(含起点、终点)JDarray[][]和曲线R L1、L2 计算线元array_pxy[][][]和曲线表QXB[][]
	getpxy_DATA(); //***********************  

	if(BZLCarray.GetSize()<2)
	{
		BZLCarray.SetSize(2);

		ACHAR GH[8];

		//起点
		_tcscpy(GH,DLB[0].BGH);

		_tcscpy(BZLCarray[0].kml,LCchr(GH,DLB[0].BLC+DLB[0].DL,3,1));

		BZLCarray[0].X = JDarray[0].N;

		BZLCarray[0].Y = JDarray[0].E;

		//终点
		_tcscpy(GH,DLB[NDL-1].EGH);

		_tcscpy(BZLCarray[1].kml,LCchr(GH,DLB[NDL-1].BLC,3,1));

		BZLCarray[1].X = JDarray[JDarray.GetSize()-1].N;

		BZLCarray[1].Y = JDarray[JDarray.GetSize()-1].E;

	}

	NDL=2; 
	setDLM(1,QXB[NJD-1].KZH,0.0,EGH,EGH ); //plh04.11.25	
	NBZLC = BZLCarray.GetSize();

	IsReSetColor = FALSE ;

	xlpoint PZtmp;
	PZtmp.lc=array_pxy[0][0][4]+xl_length/2.0;
	xlpoint_pz(array,tarray,&PZtmp);
	m_XLNameNotePt[X]=PZtmp.y+xl_length*sin(PZtmp.a+pi*0.5)/8.0;
	m_XLNameNotePt[Y]=PZtmp.x+xl_length*cos(PZtmp.a+pi*0.5)/8.0;
	texth=4; 
}

//传入交点下标号 计算出zh或hz点坐标 zhorhz=-1 zh; zhorhz=1 hz
Adesk::Boolean  GTZX_JYX_ROAD::getZhHzPt(int iJD,int zhorhz,double pt[2])
{
	double cml;
	xlpoint PZ;


	pt[0] = 0.0;
	pt[1] = 0.0;
	if(iJD>=0 && iJD <NJD)
	{
		if(zhorhz == -1)
			cml = QXB[iJD].KZH;		  		
		else
			cml = QXB[iJD].KHZ;		  

		PZ.lc=cml;
		xlpoint_pz(array,tarray,&PZ);  //实现里程转坐标
		pt[0] = PZ.x;
		pt[1] = PZ.y;
		return 1;
	}

	return 0;

}


// Adesk::Boolean  GTZX::getpxy_DATA( double arrayd[100][7],int trow)
Adesk::Boolean  GTZX_JYX_ROAD::getpxy_DATA()
{   // 导线法由导线数据文件求线位 L.SC.S.L.S.C.S.L...  得到线元文件


	double fb,fe,db,de,aa,c,lcb,lce,T0=0.0, interal=18;
	ACHAR zy[2],GH[20];
	int i=0,j=0,k,color=2,iq=2;
	double BPMTL[10],T1,T2,L;
	double LCB,DDlength;
	int iDL=0;

	for (i=0;i<10;i++) BPMTL[i]=0.0;

	//给线元数组array_pxy[i][k][j] 初始化
	for( i=0;i<MAXJDNUM;i++) 
	{
		for (k=0;k<4;k++)
		{
			for (j=0;j<5;j++)array_pxy[i][k][j]=0.0;
			for (j=0;j<10;j++)array[i*4+k][j]=0.0;
		}; 
	};

	//起终点半径应为0    
	//   JDarray[0][4]=JDarray[NJD-1][4]=JDarray[0][5]=JDarray[NJD-1][5]=0.0;
	db=xyddaa(JDarray[0].N,JDarray[0].E,JDarray[1].N,JDarray[1].E,&fb);
	//   JDarray[0][6]=fb;  
	lcb=JDarray[0].l1; //里程
	lce=lcb;
	//  fb=dms_rad(fb);
	array_pxy[0][0][0]=10.0;// 起点属性
	// N(X)=ACAD_Y
	array_pxy[0][0][1]=JDarray[0].N; //坐标
	//E(Y)=ACAD_X
	array_pxy[0][0][2]=JDarray[0].E; //坐标
	// FA
	array_pxy[0][0][3]=dms_rad(fb);  //方位角
	// LC
	array_pxy[0][0][4]=JDarray[0].l1; //里程
	//  DLB[0].BLC=JDarray[0][3];   DLB[0].DL=0.0;  NDL=1;   // 起点 作为一特殊断链点

	QXB.RemoveAll();
	QXB.SetSize(NJD);

	QXB[0].l1 = JDarray[0].l1;
	QXB[0].R = JDarray[0].R;
	QXB[0].l2 = JDarray[0].l2;
	_tcscpy(QXB[0].JDNum , JDarray[0].JDNum);
	QXB[0].isGJ = JDarray[0].isGJ;
	QXB[0].KZH=JDarray[0].l1;  // 起点里程
	QXB[0].KHY=QXB[0].KZH;  
	QXB[0].KYH=QXB[0].KZH;
	QXB[0].KHZ=QXB[0].KZH;
	QXB[0].a = QXB[0].T1 = QXB[0].T2 = QXB[0].L ;
	QXB[0].LorR = 0;


	T0=0.0;
	if (NJD==2) // 仅有两个交点,即一条直线时
	{  
		array_pxy[1][0][0]=1.0; //
		array_pxy[1][0][1]=db;  
	};

	LCB=QXB[0].KZH; // 起点里程
	DDlength= LCB;
	iDL=1;


	for (i=1;i<NJD-1;i++)
	{  
		de=xyddaa(JDarray[i].N,JDarray[i].E,JDarray[i+1].N,JDarray[i+1].E,&fe);
		aa = ZXJ(fe,fb);

		if (aa>0) 
		{  c=1; _tcscpy(zy,L"y"); }  //右偏
		else
		{ c=-1;_tcscpy(zy,L"z"); }; //左偏

		aa=fabs(aa);
		aa=dms_rad(aa);

		//    if (JDarray[i].R<=0) break;

		//计算切线长
		getBPMTL(aa,JDarray[i].R,JDarray[i].l1,JDarray[i].l2,&BPMTL[0]); 
		T1=BPMTL[6];
		T2=BPMTL[7];
		L=BPMTL[8];  //曲线长

		bool PreLinK,AftLink;
		PreLinK=FALSE,AftLink=FALSE;
		//if(fabs(JDarray[i][0]-JDarray[i-1][0])<0.1)
		// PreLinK=TRUE;
		//if(fabs(JDarray[i][0]-JDarray[i+1][0])<0.1)
		// AftLink=TRUE;

		if(_tcscmp(JDarray[i].JDNum,JDarray[i-1].JDNum)==0)
			PreLinK=TRUE;
		if(_tcscmp(JDarray[i].JDNum,JDarray[i+1].JDNum)==0)
			AftLink=TRUE;
		double RB,RE,R;
		RB=1e50,RE=1e50,R=0.0;

		double Lo1=JDarray[i].l1,Lo2=JDarray[i].l2;
		int qorh=0;

		double tt=0.0;

		if(PreLinK ||AftLink)
		{
			if( PreLinK && JDarray[i].l1>1e-6 )
			{
				RB = JDarray[i-1].R;
				//调整A1
				//  Lo1 = fabs((CurJdCen->A1*CurJdCen->A1)/RB - (CurJdCen->A1*CurJdCen->A1)/R);
				// Lo1=JDarray[i][3];
				qorh=-1;
				tt = db-T0;
			}
			else
				RB = 1e50;

			if(AftLink && JDarray[i].l2>1e-6)
			{
				RE = JDarray[i+1].R;
				//  Lo2 = fabs((CurJdCen->A2*CurJdCen->A2)/RE - (CurJdCen->A2*CurJdCen->A2)/R);
				// Lo2 = JDarray[i][5];
				qorh=1;

				double at1,at2;
				CalAJDT1T2(i+1,at1,at2);
				tt = de-at1; 

			}
			else
				RE = 1e50;
			R= JDarray[i].R;
			get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);

			double A1=0.0,A2=0.0;
			if(qorh==-1)
			{
				if(fabs(T1-tt)>PRECISION)//需要调整缓长
				{

					double lo=Adjust_L_JDLuan(aa,R,RB,Lo1,RE,Lo2,tt,qorh);

					if(lo>0.0)//有解
					{
						Lo1 = lo;

						get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);
						A1=sqrt(RB*R/fabs(RB-R)*Lo1);
						//	CurJdCen->A1=A1;
						//	CurJdCen->Ls1=Lo1;
						JDarray[i].l1 = Lo1;
						//	ads_printf(L"*********   lo1=%lf\n",Lo1);
					}
				}
			}
			if(qorh==1)
			{
				if(fabs(T2-tt)>PRECISION)//需要调整缓长
				{
					double lo=Adjust_L_JDLuan(aa,R,RB,Lo1,RE,Lo2,tt,qorh);

					if(lo>0.0)//有解
					{
						Lo2 = lo;
						get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);
						A2=sqrt(RE*R/fabs(RE-R)*Lo2);
						//	CurJdCen->A2=A2;
						//	CurJdCen->Ls2=Lo2;
						JDarray[i].l2 = Lo2;

					}
				}
			}
		}

		//考虑最后一段缓和曲线为不完整缓和曲线的情况
		if(i==NJD-2)
		{
			if(JDarray[i+1].R>0.001)
			{
				RE = JDarray[i+1].R;
				//  Lo2 = fabs((CurJdCen->A2*CurJdCen->A2)/RE - (CurJdCen->A2*CurJdCen->A2)/R);
				Lo2 = JDarray[i].l2;

				get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);
			}
		}

		if ( L<(JDarray[i].l1+JDarray[i].l2))
		{
			ads_printf(L"\n%d号曲线 前后缓和曲线重叠：缓长L1+L2=%10.3f>大于曲线长L=%10.3f ",i,JDarray[i].l1+JDarray[i].l2,L);
			//		  return 0;
		}
		else if((L-JDarray[i].l1-JDarray[i].l2)<20.0 && fabs(JDarray[i].R)>0.001)
		{ 
			ads_printf(L"\n %d号曲线的(R=%8.1f)圆曲线长L=%10.3f 不足20m ",i,JDarray[i].R,L-JDarray[i].l1-JDarray[i].l2);
			//	  return 0;
		};


		//曲线要数 角度为度.分秒
		QXB[i].a=aa;
		QXB[i].T1=T1;
		QXB[i].T2=T2;
		QXB[i].L=L;
		QXB[i].LorR = c;

		//  line
		array_pxy[i][0][0]=1.0;
		array_pxy[i][0][1]=db-T1-T0;

		//ZH (zy)
		QXB[i].KZH=QXB[i-1].KHZ+(db-T1-T0); 

		if( (db-T1-T0)<20.0) 
		{  
			//  ads_printf(L"\n %d号曲线起点ZH/ZY距前一曲线HZ/ZY距离=%-10.3f ",i+JD0-1,db-T1-T0);
			if( (db-T1-T0)<0) ads_printf(L"  超过前一曲线HZ/YZ不合法\n");
		};

		// if ((iDL<=NDL-1)&& (QXB[i][0]>DLB[iDL].BLC) ) { QXB[i][0]=QXB[i][0]+DLB[iDL].DL;iDL=iDL+1; };


		// ads_printf(L"\n%d \n %1.0f %10.3f ",i,array_pxy[i][0][0],array_pxy[i][0][1]);
		//1-spiral
		//aa=sqrt(QXLRL[i][0]*QXLRL[i][2]); 

		//A
		if(RB>1.0e6)
		{
			aa=sqrt(JDarray[i].l1*JDarray[i].R);
			if ( aa>0.000000001) 
			{ 
				array_pxy[i][1][0]=3.0; 
				array_pxy[i][1][1]=aa; //A
				//array_pxy[i][1][2]=QXLRL[i][1];
				array_pxy[i][1][2]=JDarray[i].R;  //R
				array_pxy[i][1][3]=0.0;
				array_pxy[i][1][4]=c;  //z y pian
			}

		}
		else 
		{

			double Re=0.0;
			if(fabs(JDarray[i].R-RB)>1.0e-6)
				Re = (JDarray[i].R*RB)/fabs(JDarray[i].R-RB);
			else Re=0.0;
			aa=sqrt(JDarray[i].l1*Re);
			if ( aa>0.000000001) 
			{
				if(JDarray[i].R>RB)
				{
					array_pxy[i][1][0]=6.0;
					array_pxy[i][1][2]=RB;  //R
					array_pxy[i][1][3]=JDarray[i].R; 
				}
				else 
				{
					array_pxy[i][1][0]=5.0;
					array_pxy[i][1][2]=JDarray[i].R;  //R
					array_pxy[i][1][3]=RB; 
				}  
				array_pxy[i][1][2]=RB;  //R
				array_pxy[i][1][3]=JDarray[i].R; 
				array_pxy[i][1][1]=aa; //A
				//array_pxy[i][1][2]=QXLRL[i][1];

				array_pxy[i][1][4]=c;  //z y pian
			};

		}
		// HY
		// QXB[i][1]=QXB[i][0]+QXLRL[i][0];//
		//HY里程
		QXB[i].KHY=QXB[i].KZH + JDarray[i].l1;
		// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",array_pxy[i][1][0],array_pxy[i][1][1],array_pxy[i][1][2],array_pxy[i][1][4]);

		// if ((iDL<=NDL-1)&& (QXB[i][1]>DLB[iDL].BLC) ) { QXB[i][1]=QXB[i][1]+DLB[iDL].DL;iDL=iDL+1; };

		//  circle

		//aa 园曲线长
		aa=L-JDarray[i].l1-JDarray[i].l2;

		//	  ads_printf(L">>>  aa=%lf\n",aa);
		if ( aa>0.000000001) 
		{ 
			array_pxy[i][2][0]=2.0;  //性质
			array_pxy[i][2][1]=JDarray[i].R; //R
			array_pxy[i][2][2]=aa; //yuan  chang 
			array_pxy[i][2][3]=0.0;
			array_pxy[i][2][4]=c;
		}
		else
		{
			array_pxy[i][2][0]=2.0;  //性质
			array_pxy[i][2][1]=JDarray[i].R; //R
			array_pxy[i][2][2]=aa; //yuan  chang 
			array_pxy[i][2][3]=fe ;

			array_pxy[i][2][4]=c;

			//三赋数值

		}
		// YH
		QXB[i].KYH = QXB[i].KHY + aa;
		// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",array_pxy[i][2][0],array_pxy[i][2][1],array_pxy[i][2][2],array_pxy[i][2][4]);

		// if ((iDL<=NDL-1)&& (QXB[i][2]>DLB[iDL].BLC) ) { QXB[i][2]=QXB[i][2]+DLB[iDL].DL;iDL=iDL+1; };

		//2-spiral

		if(RE>1.0e6)
		{
			aa=sqrt(JDarray[i].l2*JDarray[i].R);
			if ( aa>0.000000001)
			{
				//
				array_pxy[i][3][0]=4.0; 
				array_pxy[i][3][1]=aa;
				array_pxy[i][3][2]=JDarray[i].R;
				array_pxy[i][3][3]=0.0;
				array_pxy[i][3][4]=c;
			}; 

		}
		else
		{
			double Re=0.0;
			if(fabs(JDarray[i].R-RE)>1.0e-6)
				Re = (JDarray[i].R*RE)/fabs(JDarray[i].R-RE);
			else 
				Re=0.0;
			aa=sqrt(JDarray[i].l2*Re);
			if ( aa>0.000000001) 
			{
				if(JDarray[i].R>RE)
				{
					array_pxy[i][1][0]=5.0;
					array_pxy[i][1][2]=JDarray[i].R;  //R
					array_pxy[i][1][3]=RE;
				}
				else
				{
					array_pxy[i][1][0]=6.0;
					array_pxy[i][1][2]=RE;  //R
					array_pxy[i][1][3]=JDarray[i].R;
				}  
				array_pxy[i][1][1]=aa; //A
				//array_pxy[i][1][2]=QXLRL[i][1];
				array_pxy[i][1][2]=JDarray[i].R;  //R
				array_pxy[i][1][3]=RE;			 
				array_pxy[i][1][4]=c;  //z y pian
			};
		}

		// HZ
		QXB[i].KHZ = QXB[i].KYH + JDarray[i].l2;

		QXB[i].l1 = JDarray[i].l1;
		QXB[i].R = JDarray[i].R;
		QXB[i].l2 = JDarray[i].l2;
		_tcscpy(QXB[i].JDNum , JDarray[i].JDNum);
		QXB[i].isGJ = JDarray[i].isGJ;

		// if ((iDL<=NDL-1)&& (QXB[i][3]>DLB[iDL].BLC) ) { QXB[i][3]=QXB[i][3]+DLB[iDL].DL;iDL=iDL+1; };

		db=de;
		lcb=lce;
		fb=fe;
		T0=T2;

	};


	//  line
	db=db-T0 ;  //终点长度
	if ( db>0.0000000001) 
	{
		array_pxy[NJD-1][0][0]=1.0;	 
		array_pxy[NJD-1][0][1]=db;
	};

	// 终点里程

	QXB[NJD-1].l1 = JDarray[NJD-1].l1;
	QXB[NJD-1].R = JDarray[NJD-1].R;
	QXB[NJD-1].l2 = JDarray[NJD-1].l2;
	_tcscpy(QXB[NJD-1].JDNum, JDarray[NJD-1].JDNum);

	QXB[NJD-1].isGJ = JDarray[NJD-1].isGJ;

	QXB[NJD-1].KZH = QXB[NJD-2].KHZ + db;  
	xl_length = QXB[NJD-1].KZH - QXB[0].KZH;
	// if ((iDL<=NDL-1)&& (QXB[NJD-1][0]>DLB[iDL].BLC) ) { QXB[NJD-1][0]=QXB[NJD-1][0]+DLB[iDL].DL;iDL=iDL+1; };
	QXB[NJD-1].KHY=QXB[NJD-1].KZH;
	QXB[NJD-1].KYH=QXB[NJD-1].KZH;
	QXB[NJD-1].KHZ=QXB[NJD-1].KZH;
	QXB[NJD-1].a=QXB[NJD-1].T1 = QXB[NJD-1].T2 = QXB[NJD-1].L=QXB[NJD-1].LorR = 0.0;


	/*if(NDL<1)
	{
	DLB[0].DL=0.0; DLB[i].BLC=JDarray[0].l1;
	_tcscpy(DLB[0].BGH,L"AK"); _tcscpy(DLB[0].EGH,L"AK");

	DLB[1].DL=0.0; DLB[1].BLC=9000000.0;
	_tcscpy(DLB[1].BGH,L"AK"); _tcscpy(DLB[1].EGH,L"AK");

	NDL=2;

	}*/

	// 修改终点特殊断链点里程 (终点 作为一特殊断链点)

	ACHAR Sgh[10],Egh[10];

	_tcscpy(Sgh,DLB[NDL-1].BGH);

	_tcscpy(Egh,DLB[NDL-1].EGH);//修改终点断链冠号  2004.7.16

	double elc,blc,dl;

	blc = XLC(QXB[NJD-1].KZH,Sgh,NDL);

	if( fabs(DLB[NDL-1].DL) > 0.001)
	{
		elc = DLB[NDL-1].BLC + DLB[NDL-1].DL;
		dl =  elc-blc;
	}
	else
	{
		dl = 0.0;
	}


	if(XLShuXing!="贯通设计基线" )
		setDLM(NDL-1,blc,dl,Sgh,Egh); 	 

	//  起点
	double INDATA[5],l,L0;
	struct xlpoint PB0,PEZ;
	for (j=0;j<5;j++)array[0][j]=array_pxy[0][0][j]; 
	tarray = 1;


	//生成2维 线元数组 10
	for( i=1;i<NJD-1;i++) 
	{ 
		for (k=0;k<4;k++) 
		{		   
			for (j=0;j<5;j++) INDATA[j]=array_pxy[i][k][j];
			j=(int)(INDATA[0]+0.01);
			l=0.0;
			//各个线元长度
			if (j==1) l=INDATA[1];//直线
			if (j==2) l=INDATA[2]; //
			if (j==3||j==4)   
			{
				if(fabs(INDATA[2])>0.000001)  //R
					l=INDATA[1]*INDATA[1]/INDATA[2];  //l
				else
					l=0.0;

			}
			if (j==5)
			{
				if(fabs(INDATA[2]*INDATA[3])>0.000001)
					l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
				else
					l=0.0;

			}
			if (j==6)
			{
				if(fabs(INDATA[2]*INDATA[3])>0.000001)
					l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);
				else
					l=0.0;
			}
			//暂不要

			// 求L0=？
			L0=l;  //线元长度 
			if (j==2) L0=INDATA[1]; //R
			//		   ads_printf(L"l=%lf\n",l);


			if (L0>0.001)
			{
				for (int kk=0;kk<5;kk++) array[tarray][kk]=array_pxy[i][k][kk]; 
				if(j==1)//直线
				{
					db=xyddaa(JDarray[i-1].N,JDarray[i-1].E,JDarray[i].N,JDarray[i].E,&fb);
					array[tarray][5]=fb ;
					array[tarray][6]=QXB[i-1].KHZ; //HZ  xian yuan  qi dian licheng
					array[tarray][7]=1e40;  //R
					array[tarray][8]=JDarray[i-1].N+QXB[i-1].T2*cos(fb);  //起点坐标
					array[tarray][9]=JDarray[i-1].E+QXB[i-1].T2*sin(fb);
				}


				// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",array[trow][0],array[trow][1],array[trow][2],array[trow][4]);
				tarray=tarray+1;  
			}

		};
	};
	//  终点 线元

	for (j=0;j<5;j++)array[tarray][j]=array_pxy[NJD-1][0][j]; 	
	if(NJD>2)
	{
		i=NJD-1;  //终点线元  
		db=xyddaa(JDarray[i-1].N,JDarray[i-1].E,JDarray[i].N,JDarray[i].E,&fb);
		array[tarray][5]=fb;   
		array[tarray][6]=QXB[i-1].KHZ;  //zhong dian licheng  HZ
		array[tarray][7]=1e40;
		array[tarray][8]=JDarray[i-1].N+QXB[i-1].T2*cos(fb);
		array[tarray][9]=JDarray[i-1].E+QXB[i-1].T2*sin(fb);
	}	

	/*
	//0：线元性质 1-直线，2-圆，3-前缓，4-后缓, 5-不完整缓R大->R小, 6-不完整缓R小->R大

	//(R1>R2)/////////////////////////////////////////////////////////////////////////
	//1:			L		R    A         A              A                A        //
	//2:					L	 R         R              R1               R2       //
	//3:                                                  R2               R1       //
	//////////////////////////////////////////////////////////////////////////////////
	//4: 左右转
	//5: 起始切线方位角
	//6: 线元起点里程（=统一里程=线路起点的里程+参照点到线路起点的曲线距离）
	//7: 半径 R
	//8，9：线元起点坐标 N,E
	//最后一个线元为终点
	*/

	tarray=tarray+1;
	i = 0;
	PB0.lc=array[0][4];  // 起点
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
		// 第 i 线元
		for (k=0;k<5;k++) INDATA[k]=array[i][k];

		j=(int)(array[i+1][0]+0.0001);  //** i+1
		if(j==1)//下一单元是直线   
		{
			PB0.a=array[i+1][5];
			PB0.lc=array[i+1][6];
			PB0.r=array[i+1][7];
			PB0.x=array[i+1][8];
			PB0.y=array[i+1][9];
		}
		else
		{
			l=Get_INDATA_L(INDATA,&j);  // 计算线元长度
			xlzbxy(&PB0,&PEZ,l,INDATA,0);   // 计算坐标   //***********//

			array[i+1][5] = PEZ.a ;  //线元起点方位角

			//圆+线路长度为0
			//	if( fabs(array_pxy[i][k][0]-2)<0.00001 && l<0.00000001 ) array[i+1][5] = INDATA[3];



			array[i+1][6] = PEZ.lc;  // 求线元 终点坐标
			array[i+1][7] = PEZ.r ;//线元起点的半径,直圆\圆直为无穷大
			array[i+1][8] = PEZ.x ;
			array[i+1][9] = PEZ.y ;
			PB0=PEZ;   //坐标
		}
	};	 

	//xlpoint PZ1;
	//PZ1.lc=QXB[NJD-1].KZH;
	//xlpoint_pz(array,tarray,&PZ1);  //实现里程转坐标
	StartCml = QXB[0].KZH;
	EndCml = QXB[NJD-1].KZH;
	double xlc=XLC(StartCml,GH,NDL);
	_tcscpy(StartCKml,L"\0");   _tcscpy(StartCKml,LCchr(GH,xlc,NLC,1));
	xlc=XLC(EndCml,GH,NDL);
	_tcscpy(EndCKml,L"\0");   _tcscpy(EndCKml,LCchr(GH,xlc,NLC,1));

	return 1;
}


//  由断链数据表 求某统一里程的现场里程和冠号
double GTZX_JYX_ROAD::XLC( double TYLC,ACHAR *GH, int NDL)
{ 
	//2005.10.12改
	double LCB=0,XCLC ;  // LCB 断链的统一里程
	int iDL=0;
	CString strs1,strs2;

	XCLC = TYLC ;

	for (iDL=0;iDL<NDL;iDL++)
	{
		//该断链点统一里程
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
			else if (fabs(TYLC-LCB)<0.00005)
			{
				if (fabs(DLB[iDL].BLC-(int)DLB[iDL].BLC)<0.00005)
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
					if (DLB[iDL-1].DL < 0.00001 && XCLC < DLB[iDL-1].BLC)//in 前长链
						_tcscpy(GH,strs1);							
					else if  (DLB[iDL].DL < 0.00001 && XCLC < DLB[iDL].BLC)//in 后长链
						_tcscpy(GH,strs2);
					else//前后都不是长链
					{
						if (2.0*(LCB-TYLC) > DLB[iDL].BLC - DLB[iDL-1].BLC - DLB[iDL-1].DL)
							_tcscpy(GH,strs1);
						else
							_tcscpy(GH,strs2);
					}
				}
				break;
			}
			else if (fabs(TYLC-LCB)<0.00005)
			{
				if (fabs(DLB[iDL].BLC-(int)DLB[iDL].BLC)<0.00005)
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
	if (NDL >0 && iDL == NDL)
	{
		XCLC = DLB[iDL-1].BLC + DLB[iDL-1].DL + TYLC - LCB;
		_tcscpy(GH,DLB[iDL-1].EGH);
	}
	return XCLC;
}

//  由断链数据表 求某统一里程的现场里程和冠号
double   GTZX_JYX_ROAD::XLC( double TYLC,ACHAR *GH, int NDL,int &iDL)
{ 
	//2005.10.12改
	double LCB=0,XCLC ;  // LCB 断链的统一里程
	iDL=0;
	CString strs1,strs2;

	XCLC = TYLC;
	for (iDL=0;iDL<NDL;iDL++)
	{
		//该断链点统一里程
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
			else if (fabs(TYLC-LCB)<0.00005)
			{
				if (fabs(DLB[iDL].BLC-(int)DLB[iDL].BLC)<0.00005)
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
					if (DLB[iDL-1].DL < 0.00001 && XCLC < DLB[iDL-1].BLC)//in 前长链
						_tcscpy(GH,strs1);							
					else if  (DLB[iDL].DL < 0.00001 && XCLC < DLB[iDL].BLC)//in 后长链
						_tcscpy(GH,strs2);
					else//前后都不是长链
					{
						if (2.0*(LCB-TYLC) > DLB[iDL].BLC - DLB[iDL-1].BLC - DLB[iDL-1].DL)
							_tcscpy(GH,strs1);
						else
							_tcscpy(GH,strs2);
					}
				}
				break;
			}
			else if (fabs(TYLC-LCB)<0.00005)
			{
				if (fabs(DLB[iDL].BLC-(int)DLB[iDL].BLC)<0.00005)
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
	if (iDL == NDL && NDL>0)
	{
		XCLC = DLB[iDL-1].BLC + DLB[iDL-1].DL + TYLC - LCB;
		_tcscpy(GH,DLB[iDL-1].EGH);
	}
	return XCLC;


}


//  由断链数据表 求某现场里程的统一里程
double GTZX_JYX_ROAD::TYLC( ACHAR ckml[], int NDL )
{

	double TLC = 0.0;
	double TLCtmp = 0.0;
	int iDL=0;
	//起点里程
	double XLC = 0.0;
	ACHAR GH[10];
	split_ckml(ckml,&XLC,GH);
	TLC=XLC;

	CString strstr1,strstr2;
	short int numb = 0;//统计计算的连续里程的个数2005-08-26
	strstr1 = GH;
	strstr1.TrimLeft();
	strstr1.TrimRight();
	strstr1.MakeLower();
	double continueLC = DLB[0].BLC + DLB[0].DL;//断链的统一里程
	for (iDL = 1;iDL < NDL - 1; iDL++)//检查是否在长链内
	{
		continueLC = continueLC + DLB[iDL].BLC - DLB[iDL-1].DL - DLB[iDL-1].BLC;
		if (XLC-DLB[iDL].BLC<0.0005 || DLB[iDL].BLC + DLB[iDL].DL-XLC<0.0005)//
		{
			strstr2 = DLB[iDL].BGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLCtmp = continueLC - DLB[iDL].BLC + XLC; 
				if (fabs(TLC - TLCtmp)>0.0009)
				{
					numb++;
					TLC = TLCtmp;
				}
			}
			else
			{
				strstr2 = DLB[iDL].EGH;
				strstr2.TrimLeft();
				strstr2.TrimRight();
				strstr2.MakeLower();
				if (strstr1 == strstr2)
				{
					TLCtmp = continueLC - DLB[iDL].BLC - DLB[iDL].DL + XLC;
					if (fabs(TLC - TLCtmp)>0.0009)
					{
						TLC = TLCtmp;
						numb++;
					}
				}
			}
		}
	}

	if (numb == 0)//不在长链内
	{
		continueLC = DLB[0].BLC + DLB[0].DL;
		for (iDL = 0; iDL < NDL; iDL++)
		{
			if (iDL > 0)
			{
				if (DLB[iDL-1].BLC+DLB[iDL-1].DL-XLC<0.0005 || XLC-DLB[iDL].BLC<0.0005)
				{
					strstr2 = DLB[iDL-1].EGH;
					strstr2.TrimLeft();
					strstr2.TrimRight();
					strstr2.MakeLower();
					if (strstr1 == strstr2)
					{
						TLC = continueLC + XLC - DLB[iDL-1].BLC - DLB[iDL-1].DL;
						numb++;
						break;
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
							break;
						}
					}
				}
				continueLC =continueLC + DLB[iDL].BLC - DLB[iDL-1].DL - DLB[iDL-1].BLC;
			}
			else if (iDL == 0)
			{
				if (XLC - DLB[0].BLC<0.00005)//在起点之外
				{
					strstr2 = DLB[0].BGH;
					strstr2.TrimLeft();
					strstr2.TrimRight();
					strstr2.MakeLower();
					if (strstr1 == strstr2)
					{
						TLC = continueLC - DLB[0].BLC + XLC;
						numb++;
						break;
					}
				}
			}
		}
		if (iDL == NDL && XLC > DLB[iDL-1].BLC + DLB[iDL-1].DL)//在终点之外
		{
			TLC = continueLC + XLC - DLB[iDL-1].BLC - DLB[iDL-1].DL;
			numb++;
		}
	}
	strstr1 = ckml;
	CString ERR;
	if (numb == 1)
		TLC = ((int)(TLC * 1000.0 + 0.5))/1000.0;
	else if (numb > 1)
		ads_printf(L"\n系统发现" + strstr1 + "在线路上有多个位置，可能是断链表数据有问题");
	else if (numb == 0)
		//		ads_printf(L"\n系统发现" + strstr1 + "在线路上没有其位置，可能是短链或断链表数据有问题");
		return TLC;

	return 0;
}


Adesk::Boolean  GTZX_JYX_ROAD::BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type)  
	//  标注公里百米里程标  
	//    zybz=1,-1  标在右边，左边
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

	ptb[Z]=0.0;
	pte[Z]=0.0;
	pt[Z]=0.0;
	pt[Z]=0.0;
	// 断链处统一里程
	DDlength=LCB;

	for (iDL=0;iDL<NDL;iDL++)
	{   
		DL = DLB[iDL].DL;
		LCB= DLB[iDL].BLC+DL;  //本段起点里程 
		_tcscpy(GH,DLB[iDL].EGH);
		_tcscpy(GH1,DLB[iDL].BGH);

		// 绘断链点 或冠号改变点idl断链下标
		if (iDL>=0 && iDL< NDL)  
		{ 
			BOOL IfBZDL=true;//是否需要标注断链
			if(iDL==0 || iDL==NDL-1)
			{
				if(fabs(DLB[iDL].DL)<0.001)
					IfBZDL = false;                  
			}

			//	ads_printf(L"iDL=%d,DLB[iDL].DL=%lf,IfBZDL=%d\n",iDL,DLB[iDL].DL,IfBZDL);
			/*  标注iDL断链信息*/
			if(IfBZDL)
			{

				lc=DDlength;

				lc= CalcmlTonewcml(lc);
				PZtmp.lc=lc;  // 统一里程

				if(XLShuXing!="贯通设计基线")
					k=xlpoint_pz(array,tarray,&PZtmp);  
				else
					k=xlpoint_pz_GTX(array,tarray,&PZtmp);  

				if (k>0&& k<=6)  //有解
				{
					B=PZtmp.a;

					pt[X]=PZtmp.y+WB*18*sin(B+pi*0.5*zybz);
					pt[Y]=PZtmp.x+WB*18*cos(B+pi*0.5*zybz);
					ptb[X]=PZtmp.y;
					ptb[Y]=PZtmp.x;

					//	ads_printf(L">>>>  BLC=%lf,lc=%lf,X=%lf,Y=%lf\n",DLB[iDL].BLC,PZtmp.lc,PZtmp.x,PZtmp.y);
					G_makelineW(pWd,ptb,pt,QXYSColor ,LineWeightValue);
					// makeline(ptb,pt,5);
					/////
					PZtmp.lc=lc-1.2*texth;

					if(XLShuXing!="贯通设计基线")
						k=xlpoint_pz(array,tarray,&PZtmp);  
					else
						k=xlpoint_pz_GTX(array,tarray,&PZtmp); 

					B=PZtmp.a;
					p1[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
					p1[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

					PZtmp.lc=lc+2.0*texth;

					if(XLShuXing!="贯通设计基线")
						k=xlpoint_pz(array,tarray,&PZtmp);  
					else
						k=xlpoint_pz_GTX(array,tarray,&PZtmp);  

					B=PZtmp.a;
					pte[X]=PZtmp.y+(WB+2*texth)*sin(B+pi*0.5*zybz) ;
					pte[Y]=PZtmp.x+(WB+2*texth)*cos(B+pi*0.5*zybz) ;

					b=B+pi*0.5;
					b=pi*0.5-b; if (zybz==-1) b=b+pi;
					if (b<0.0) b=2*pi+b;		  if (b>2*pi) b=b-2*pi;		         
					if(type<=1)  b=b-pi*0.5; 

					_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH1,DLB[iDL].BLC,NLC,1));  // 断前里程
					_tcscpy(chr,L"  ");_tcscat(chr,ch);   
					G_maketext(pWd,p1,chr,b,texth,QXYSColor,1);  

					_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,LCB,NLC,1));     // 断后里程
					_tcscpy(chr,L" =");  _tcscat(chr,ch);   // e.f chr="ZH K10+123.45"   

					if (fabs( DL)>0.001)
					{  if ( DL>0.0)   { 
						_tcscat(chr, L" 短链 "); 
						ads_rtos( DL,2,NLC,ch); _tcscat(chr, ch); 
						_tcscat(chr, L" m");  };
					if ( DL<0.0)   { 
						_tcscat(chr, L" 长链 "); 
						ads_rtos(fabs(DL),2,NLC,ch); _tcscat(chr, ch); 
						_tcscat(chr, L" m");  };
					};

					G_maketext(pWd,pte,chr,b,texth,QXYSColor,1);     

				}

			};

		};
		//本段起点里程 
		zlc=(long int)(LCB/dlc);  //dlc-百米标
		BZlc=zlc*dlc;  // 本段起始　百米标实际里程 
		if ((BZlc-LCB)<0.0001)  
		{ 
			BZlc=BZlc+dlc; 
		}  
		_tcscpy(GH,DLB[iDL].EGH);  //冠号

		if( BZlc<DLB[iDL+1].BLC )
		{
			do {
				// 统一里程
				//pLCB[NLCB].lcflag=100;
				//lc ---百米标的统一里程
				lc=DDlength+BZlc-LCB;

				PZtmp.lc=lc;  // 统一里程
				//	pLCB[NLCB].lc=lc;

				PZtmp.lc=CalcmlTonewcml(lc);  //考虑标志里程 计算 新的连续里程

				if(XLShuXing!="贯通设计基线")
					k=xlpoint_pz(array,tarray,&PZtmp);  
				else
					k=xlpoint_pz_GTX(array,tarray,&PZtmp);  

				BZW=k;    //  K<0  无解
				if (k>0&& k<=6)  //有解
				{
					// ads_printf(L"\n i=%d %10.3f %10.3f  %10.3f %10.3f ",iDL,PZtmp.lc, BZlc,LCB,DLB[iDL+1].BLC );  

					//---------------------------------->
					//百米标的标注线
					B=PZtmp.a;
					pt[X]=PZtmp.y+WB*sin(B+pi*0.5*zybz);
					pt[Y]=PZtmp.x+WB*cos(B+pi*0.5*zybz);
					pte[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
					pte[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);
					ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; 
					G_makelineW(pWd,ptb,pt,QXYSColor,LineWeightValue);
					// makeline(ptb,pt,5);

					pt=pte;
					b=B+pi*0.5;
					b=pi*0.5-b; if (zybz==-1) b=b+pi;
					if (b<0.0) b=2*pi+b;
					if (b>2*pi) b=b-2*pi;

					// ads_printf(L"\n %10.3f %10.3f %10.3f %10.4f",PZtmp.lc,PZtmp.x,PZtmp.y,dms_rad(b));

					ads_rtos(BZlc,2,0,chr);  
					// _tcscpy(ch,GH); _tcscat(ch,chr);
					// 里程百米标
					ii=_tcslen(chr);
					if(ii>20) ii=19;
					if (dlc>=100.0&&ii>=2)
					{  chr[ii-1]='\0';chr[ii-2]='\0';
					_tcscpy(ch,chr);
					if (ii>3&&chr[ii-3]=='0') // 公里标
					{ chr[ii-3]='\0'; 
					_tcscpy(ch,GH); 
					_tcscat(ch,chr); }//pLCB[NLCB].lcflag=1000;}
					else if(ii>3 && chr[ii-3]!='0')    // 百米标
					{ 
						ch[0]=chr[ii-3];ch[1]='\0';
					};		    
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
							if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // 公里标
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

					G_maketext(pWd,pte,ch,b,texth,QXYSColor,1);     
					/*
					_tcscpy(//pLCB[NLCB].bz,ch);
					NLCB++;
					*/
				};          
				BZlc=BZlc+dlc;

				if ( BZlc>DLB[iDL+1].BLC) BZW=-99;		 		 

			}while (BZW>0);
		}
		DDlength=  DDlength+DLB[iDL+1].BLC-LCB;//断链处统一里程。
	} // IDL
	//	 for(int kk=0;kk<NLCB;kk++)acutPrintf(L"i=%d,lc=%0.3lf,x=%0.3lf,y=%0.3lf,bz=%s\n",kk,//pLCB[kk].lc,//pLCB[kk].x,//pLCB[kk].y,//pLCB[kk].bz);


	return TRUE;
}

//	标注交点号 和曲线参数
void GTZX_JYX_ROAD::BZJD(AcGiWorldDraw* pWd,CArray<JDarrayStruct,JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb,int njd)
{  
	double fb,fe,de,da,aa,dd,dt ;
	AcGePoint3d  p1; 
	ACHAR ch[50],ch1[60],chrtmp[80];
	ACHAR ch2[100];
	int i,c,n;
	de=xyddaa(arrayd[0].N,arrayd[0].E,arrayd[1].N,arrayd[1].E,&fb);

	for (i=1;i<njd-1;i++)
	{  
		if (arrayd[i].R<0) break;
		de=xyddaa(arrayd[i].N,arrayd[i].E,arrayd[i+1].N,arrayd[i+1].E,&fe);
		aa= fe - fb ;
		if (aa<-pi) aa=aa+2*pi;
		if  (aa>pi) aa=aa-2*pi;
		if (aa>0) 
		{ //右偏
			c=1;
			dd=texth;  
		}  
		else  //左偏
		{ 
			c=-1;
			dd=texth+texth;
		};
		de=fb+aa*0.5;

		//		_tcscpy(ch2,m_AddJDGH);
		_tcscpy(ch1,arrayd[i].JDNum);

		//if(_tcscmp(ch2,L"")==0)
		//{

		//}
		//else
		//{
		//	_tcscat(ch2,ch1);
		//	_tcscpy(ch1,ch2);
		//}


		n=_tcslen(ch1);
		p1[Z]=0.0;

		p1[X]=arrayd[i].E-n*0.5*texth*sin(de)+dd*sin(de-c*pi*0.5) ; 
		p1[Y]=arrayd[i].N-n*0.5*texth*cos(de)+dd*cos(de-c*pi*0.5);

		da= m_dHalfPI-de;
		if(arrayd[i].R>0.001) G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);    // 交点号  
		// maketext( p1,ch1,de,texth ,4,1); 
		draw_zybz=0;
		if( qxbbz==1)
		{
			// 标曲线参数 begin
			double R,T,T1,T2,L1,L2,LL;
			//	R=arrayd[i].R; L1= arrayd[i].l1;  L2=arrayd[i].l2; 

			R=qxb[i].R; L1= qxb[i].l1;  L2=qxb[i].l2; 
			T1=qxb[i].T1; T2=qxb[i].T2;  LL=qxb[i].L;
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
			p1[X]=arrayd[i].E-dd*sin(de-c*pi*0.5) ; 
			p1[Y]=arrayd[i].N-dd*cos(de-c*pi*0.5);
			_tcscpy(chrtmp,L"  "); _tcscat(chrtmp,ch1);
			G_maketext(pWd,p1,chrtmp,da,texth ,QXYSColor,1);   // 交点号

			ads_rtos(qxb[i].a,2,4,ch);
			if (c==1) { _tcscpy(ch1,L"ay--"); }
			else _tcscpy(ch1,L"az--"); 
			_tcscat(ch1,ch);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //  偏角

			if (fabs(R-(long int)(R))<0.001)  { ads_rtos(R,2,0,ch); }
			else { ads_rtos(R,2,NLC,ch); };
			_tcscpy(ch1,L"R--"); _tcscat(ch1,ch);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   半径

			if (L1>0||L2>0)  //  缓长不为零
			{
				if (fabs(L1-(long int)(L1))<0.001)  { ads_rtos(L1,2,0,ch); }
				else { ads_rtos(L1,2,NLC,ch); };
				p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
				p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
				if (fabs(L1-L2)<0.001) 
				{ 
					_tcscpy(ch1,L"Lo--");  _tcscat(ch1,ch);         
					G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   缓长 相等
				} 
				else
				{ 
					_tcscpy(ch1,L"L1--");  _tcscat(ch1,ch);
					if (fabs(L2-(long int)(L2))<0.001)  { ads_rtos(L2,2,0,ch); }
					else { ads_rtos(L2,2,NLC,ch); };
					_tcscat(ch1,L"  L2--"); _tcscat(ch1,ch);
					G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   不等缓长
				};
			};

			if (fabs(T1-(long int)(T1))<0.001) 
			{ 
				ads_rtos(T1,2,0,ch); 
			}
			else 
			{ 
				ads_rtos(T1,2,NLC,ch);
			};
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if (fabs(L1-L2)<0.001) 
			{   _tcscpy(ch1,L"T--");  _tcscat(ch1,ch);
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   切长 相等
			} else
			{   _tcscpy(ch1,L"T1--");  _tcscat(ch1,ch);
			ads_rtos(T2,2,NLC,ch);
			_tcscat(ch1,L"  T2--"); _tcscat(ch1,ch);
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //   不等切长
			};

			ads_rtos(LL,2,NLC,ch);
			_tcscpy(ch1,L"L--");  _tcscat(ch1,ch);
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			G_maketext(pWd,p1,ch1,da,texth ,QXYSColor,1);   //  曲线长
			// 标曲线参数 end
		};
		fb=fe; 
	};  
}

//  标特征点里程
void GTZX_JYX_ROAD::BZTZD(AcGiWorldDraw* pWd, double dlc, int bz_zybz,int Chinese_Engbz_type)  //  标注里程
	// bz_zybz==0   按曲线偏向标注   bz_zybz=1,-1  标在右边，左边
{      AcGePoint3d  ptb,pt,pte;
double WB,lc, b,l,INDATA[5],LCB,L0;
int i, j,k , KK=7,zybz=1;

ACHAR chr[50],ch[40],tch[6], GH[8],DGH[8],lcchr[20] ;
xlpoint  P0tmp,PZtmp;

zybz=bz_zybz;
if( zybz<-1|| zybz>1)  zybz=-1;

_tcscpy(GH,DLB[0].EGH);
LCB=DLB[0].BLC+DLB[0].DL;
// _tcscpy(GH,L"CK");
// texth=2.0;

WB=4*texth;
WB=WB+10*texth;

// ads_initget(1+2+4,NULL);
// ads_getint(L"\n Enter 特征点符号 type=1 (in English) / type=2 (in Chinese)):",&Chinese_Engbz_type);
//  if (Chinese_Engbz_type<1||Chinese_Engbz_type>2) Chinese_Engbz_type=1;
P0tmp.lc=array[0][4]; 
P0tmp.x=array[0][1];P0tmp.y=array[0][2];
P0tmp.a=rad(array[0][3]);P0tmp.r=1e40;

for (i=1;i<tarray;i++)
{
	zybz=1;

	for (k=0;k<5;k++) INDATA[k]=array[i][k];j=(int)(array[i][0]+0.01);
	l=0.0;
	if (j>=3)  {
		// _tcscpy(chr,L"A=");
		_tcscpy(chr,L"Lo=");
		_tcscpy(tch,L".S."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"H"); };
	if (j==1) { l=INDATA[1];  _tcscpy(chr,L"L="); _tcscpy(tch,L".T."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"Z"); };
	if (j==2) { l=INDATA[2];  _tcscpy(chr,L"R="); _tcscpy(tch,L".C."); if (Chinese_Engbz_type==2)_tcscpy(tch,L"Y"); };

	//  求A=？
	if (j==3)   l=INDATA[1]*INDATA[1]/INDATA[2];
	if (j==4)   l=INDATA[1]*INDATA[1]/INDATA[2];
	if (j==5) l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
	if (j==6) l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);

	// 求L0=？
	L0=l; if (j==2) L0=INDATA[1];

	ptb[X]=P0tmp.y;ptb[Y]=P0tmp.x; ptb[Z]=0.0;pt[Z]=0.0;pte[Z]=0.0;
	if (zybz==1||zybz==-1) {
		lc=l*0.5-3.721*texth; if (lc<0.0) lc=l*0.5;
		//计算线元坐标
		xlzbxy(&P0tmp,&PZtmp,lc,INDATA,0);b=PZtmp.a+pi*0.5*zybz;

		pt[X]=PZtmp.y+2.0*texth*sin(b);
		pt[Y]=PZtmp.x+2.0*texth*cos(b);
		pt[Z]=0.0;
		b=PZtmp.a; 
		b=pi*0.5-b;
		if (b<0.0) b=2*pi+b;
		if (b>2*pi) b=b-2*pi;
		// ads_rtos(INDATA[1],2,NLC,ch);_tcscat(chr,ch);
		if (fabs(L0-(long int)(L0+0.001))<0.001)  { ads_rtos(L0,2,0,ch); }
		else { ads_rtos(L0,2,NLC,ch); };
		_tcscat(chr,ch);  
		if (INDATA[0]>1&&INDATA[0]<3) //   园弧
		{ads_rtos(INDATA[2],2,NLC,ch); _tcscat(chr,L" L="); _tcscat(chr,ch);  };

		if ( L0>0.001) 
			G_maketext(pWd,pt,chr,b,texth,QXYSColor,1);  // 标 直线、园曲线、缓和曲线长度

	};

	xlzbxy(&P0tmp,&PZtmp,l,INDATA,0); // 某线元终点
	// 绘制方向

	zybz=bz_zybz;
	if (bz_zybz==0) 
	{ j=(int)(array[i][0]+0.01);  // 类型 
	if (j>1&&j<=6)  zybz=(int)(array[i][4]+0.01);
	if(j==1) 		zybz=(int)(array[i-1][4]+0.01);	 
	if (zybz==0||zybz>1||zybz<-1) zybz=-1;
	// ads_printf(L"\n j=%d array[i][4]=%2.0f zybz=%d",j,array[i][4],zybz);
	}
	//
	b=P0tmp.a+pi*0.5*zybz;
	pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
	if ( L0>0.001) G_makelineW(pWd,ptb,pte,QXYSColor,LineWeightValue);

	if (i==1)//plh04.11.25
	{ 
		if(_tcscmp(DLB[0].BGH,DLB[0].EGH)!=0 || fabs(DLB[0].DL)>0.01)
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
			G_makelineW(pWd,ptb,pte,QXYSColor,LineWeightValue);

			ACHAR BlcStr[80],ElcStr[80];
			_tcscpy(BlcStr,L"  ");	
			_tcscat(BlcStr,LCchr(DLB[0].BGH,DLB[0].BLC,3,1));//起点断前里程
			_tcscpy(ElcStr,L" =QD ");
			_tcscat(ElcStr,LCchr(DLB[0].EGH,DLB[0].BLC+DLB[0].DL,3,1));//起点断后里程

			b=PZtmp.a;
			pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
			b=PZtmp.a-pi*0.5 ;
			if(zybz==1)
			{
				pt[X]=pt[X]-(_tcslen(BlcStr)+0.25)*texth*sin(b);
				pt[Y]=pt[Y]-(_tcslen(BlcStr)+0.25)*texth*cos(b);
			}
			b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
			G_maketext(pWd,pt,BlcStr,b,texth,QXYSColor,1);	

			b=PZtmp.a;
			pt[X]=ptb[X]+1.25*texth*sin(b); pt[Y]=ptb[Y]+1.25 *texth*cos(b);
			b=PZtmp.a-pi*0.5 ;
			if(zybz==1)
			{
				pt[X]=pt[X]-(_tcslen(ElcStr)+0.25)*texth*sin(b);
				pt[Y]=pt[Y]-(_tcslen(ElcStr)+0.25)*texth*cos(b);
			}
			b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
			G_maketext(pWd,pt,ElcStr,b,texth,QXYSColor,1);	

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
			G_maketext(pWd,pt,chr,b,texth,QXYSColor,1);			 

			b=PZtmp.a+pi*0.5*zybz;
			pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
			G_makelineW(pWd,ptb,pte,QXYSColor,LineWeightValue);
		}
	}   
	else if (i>1 && i<=tarray-1) {
		j=(int)(array[i-1][0]+0.01);
		if (j>=3) { _tcscpy(ch,L" S"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" H"); };
		if (j==1) { _tcscpy(ch,L" T"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" Z"); };
		if (j==2) { _tcscpy(ch,L" C"); if (Chinese_Engbz_type==2)_tcscpy(ch,L" Y"); };
		_tcscpy(chr,ch);_tcscat(chr,tch);
		//chr
		if (_tcsncmp(chr,L" YY",3)==0)   {  _tcscpy(chr,L" GQ");    };
		if (_tcsncmp(chr,L" C.C.",5)==0) {  _tcscpy(chr,L" P.C.C.");};
		if (_tcsncmp(chr,L" S.S.",5)==0&&(j==3||j==4) ) {  _tcscpy(chr,L" PRC");};
		if (_tcsncmp(chr,L" HH",3)==0&&(j==3||j==4)) {  _tcscpy(chr,L" FGD");};
		_tcscpy(DGH,chr);

		if (KK==7)
		{
			lc=P0tmp.lc;   lc=XLC(lc,GH,NDL);
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
		G_maketext(pWd,pt,chr,b, texth,QXYSColor,1);
		// ads_printf(L"\n i=%d,L0= %10.4f lc=%12.3f chr=%s",i,L0,lc,chr);

		if (i==tarray-1 ) //plh04.11.25
		{
			int nn=NDL-1;
			if(_tcscmp(DLB[nn].BGH,DLB[nn].EGH)!=0 || fabs(DLB[nn].DL)>0.01)
			{
				ptb[X]=PZtmp.y;ptb[Y]=PZtmp.x; ptb[Z]=0.0;
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
				G_makelineW(pWd,ptb,pte,QXYSColor,LineWeightValue);

				ACHAR BlcStr[80],ElcStr[80];
				_tcscpy(BlcStr,L" ZD ");
				_tcscat(BlcStr,LCchr(DLB[nn].BGH,DLB[nn].BLC,3,1));//起点断前里程
				_tcscpy(ElcStr,L" = ");
				_tcscat(ElcStr,LCchr(DLB[nn].EGH,DLB[nn].BLC+DLB[nn].DL,3,1));//起点断后里程

				b=PZtmp.a;
				pt[X]=ptb[X]-0.25*texth*sin(b); pt[Y]=ptb[Y]-0.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(BlcStr)-2)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(BlcStr)-2)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,BlcStr,b,texth,QXYSColor,1);	

				b=PZtmp.a;
				pt[X]=ptb[X]+1.25*texth*sin(b); pt[Y]=ptb[Y]+1.25 *texth*cos(b);
				b=PZtmp.a-pi*0.5 ;
				if(zybz==1)
				{
					pt[X]=pt[X]-(_tcslen(ElcStr)-2)*texth*sin(b);
					pt[Y]=pt[Y]-(_tcslen(ElcStr)-2)*texth*cos(b);
				}
				b=pi*0.5-b;if (b<0.0) b=2*pi+b;if (b>2*pi) b=b-2*pi;
				G_maketext(pWd,pt,ElcStr,b,texth,QXYSColor,1);	

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
				G_maketext(pWd,pt,chr,b,texth,QXYSColor,1);
				b=PZtmp.a+pi*0.5*zybz;
				pte[X]=ptb[X]+WB*sin(b); pte[Y]=ptb[Y]+WB*cos(b);
				G_makelineW(pWd,ptb,pte,QXYSColor,LineWeightValue);
				// ads_printf(L"\n i=%d,lc=%12.3f chr=%s",trow-1,lc, chr);
			}
		}
	}
	P0tmp=PZtmp;
}
}

Adesk::Boolean  GTZX_JYX_ROAD::G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double LO,struct xlpoint *P0,struct xlpoint *PZ, double ND)
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

pWd->subEntityTraits().setColor(HuanHequxianColor);	
pWd->subEntityTraits().setLineWeight(LineWeightValue);
pWd->geometry().polyline(nk+1,pArr);
// ads_printf(L"\n AA 类型=%d %d %10.3f %10.3f %5.1f %10.2f",(int)INDATA[0],nk,INDATA[1],INDATA[2],ND,LO);
return  0;
}

Adesk::Boolean  GTZX_JYX_ROAD::G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx)
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
	//	ads_printf(L"\n R 类型=2" );	ads_printf(L"\n R 类型=2" ); 
	return 0 ;
}

Adesk::Boolean  GTZX_JYX_ROAD::G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor)
{     
	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	
	pWd->subEntityTraits().setLineWeight(LineWeightValue);	
	pWd->geometry().polyline(2,pArr);
	return  0;
}

Adesk::Boolean  GTZX_JYX_ROAD::G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw)
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



Adesk::Boolean  GTZX_JYX_ROAD::G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode=1)
{   

	AcGeVector3d normal(0.0,0.0,1.0), direction(cos(ang), sin(ang), 0.0);
	// ads_printf(L"\n             %10.3f %10.3f %10.4f ",Pt.x,Pt.y,dms_rad(ang));
	pWd->subEntityTraits().setColor(icolor);	
	//    pWd->geometry().text(Pt , normal, direction, texth,1.0, 0.0, ctext);
	AcGiTextStyle pTextStyle;
	//pTextStyle.setFont(L"宋体",TRUE,FALSE,0,0);
	pTextStyle.setFont(L"宋体",0,0,134,2);
	pTextStyle.setTextSize(texth);
	AcGePoint3d pt1;
	double len=_tcslen(ctext)*texth*0.7;
	if (mode == 0)//输入点为字中下点时
	{
		Pt.x = Pt.x - 0.5*len*cos(ang);
		Pt.y = Pt.y - 0.5*len*sin(ang);
	}
	else if (mode == 1)//输入点为字左下点时
	{
		Pt.x = Pt.x ;
		Pt.y = Pt.y ;
	}
	else if (mode == 2)//输入点为字右下点时
	{
		Pt.x = Pt.x - len*cos(ang);
		Pt.y = Pt.y - len*sin(ang);
	}
	else if (mode == 3)//输入点为字左上角点
	{
		Pt.x = Pt.x + texth * cos(ang-0.5*PI);
		Pt.y = Pt.y + texth * sin(ang-0.5*PI);
	}
	pWd->geometry().text(Pt,normal,direction,ctext,_tcslen(ctext),Adesk::kTrue,pTextStyle);
	return 1;
}

Adesk::Boolean  GTZX_JYX_ROAD::G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ)
{    
	int j;
	double l=0.0;
	AcGePoint3d ptb,pt,pte;
	//	  PZ = P0;

	l=Get_INDATA_L(INDATA,&j);
	PZ->x = P0->x;
	PZ->y = P0->y; 

	PZ->a= P0->a;

	//圆+线路长度为0
	if( fabs(INDATA[0]-2)<0.00001 && l<0.00000001 ) PZ->a = INDATA[3];

	PZ->lc = P0->lc;
	PZ->r = P0->r;
	if(l<0.00000001)
		return 1;
	//       ads_printf(L"\n  类型=%d l=%10.3f" ,j,l );
	ptb[X]=P0->y;ptb[Y]=P0->x; ptb[Z]=0.0;
	// ads_printf(L"\n PO=%10.3f %10.3f %10.3f %10.3f",P0->y,P0->x,dms_rad(P0->a),P0->lc) ;
	if ((j<=3 &&j>=6 && INDATA[1]<=0.000000000000001)|| (l<=0.00000000001)) return 1;

	if (j>2 && j<=6 )
	{ //( 回旋线)
		G_drawspi(pWd,INDATA,l,P0,PZ, 2.0);
		xlzbxy(P0,PZ,l,INDATA,0);// 某线元终点
	}
	else if (j<=2)  // R L
	{//( 园弧 直线)

		xlzbxy(P0,PZ,l*0.5,INDATA,0); // 某线元中点
		pt[X]=PZ->y;pt[Y]=PZ->x; pt[Z]=0.0;

		xlzbxy(P0,PZ,l,INDATA,0); // 某线元终点

		pte[X]=PZ->y;pte[Y]=PZ->x; pte[Z]=0.0; //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
		//  ads_printf(L">>>>???  pte[X]=%lf,pte[Y]=%lf\n",pte[X],pte[Y]);
		if (j==1) //直线
		{	 AcGePoint3d pcArr[2];
		pcArr[0]=ptb; pcArr[1]=pte;
		if(Zx[0]==-1&&Zx[1]==-1)
			pWd->subEntityTraits().setColor(JiaZhiXianColorZuo);	
		else
			pWd->subEntityTraits().setColor(JiaZhiXianColorYou);	
		pWd->subEntityTraits().setLineWeight(LineWeightValue);
		pWd->geometry().polyline(2,pcArr);
		// ads_printf(L"\n L 类型= 1"  );
		}
		else if (j==2) // 园弧 
		{ 	pWd->subEntityTraits().setColor(XWArcColor);	
		pWd->subEntityTraits().setLineWeight(LineWeightValue);
		pWd->geometry().circularArc(ptb,pt,pte);
		// ads_printf(L"\n r 类型= 2"  );
		};
	};  //J
	// ads_printf(L"\n PZ=%10.3f %10.3f %10.3f %10.3f",PZ->y,PZ->x,dms_rad(PZ->a),PZ->lc) ;
	return 0;
}

Adesk::Boolean  GTZX_JYX_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{ 
	//	setColorLineW();

	DrawGTZXPMXL(pWd);
	TrsBPDArraytoCM();
	if (IsdrawBPD)
	{
		BZCM(pWd);
	}


	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}

//半径与缓长的规范检测
void GTZX_JYX_ROAD::RTolCheckUp( BOOL IsGaiJian)
{
	double DesignL = 0.0 ; //根据半径设计查规范的缓长 
	double DV = 0.0 ;  //速度目标值
	for( int i=1 ; i< JDarray.GetSize()-1 ; i++ )
	{
		DV = m_GuiFanCS.SuDuMuBiao;
		DesignL = get_l0( JDarray[i].R ,DV,!IsGaiJian, m_GuiFanCS);

		//	if( fabs( DesignL-JDarray[i].l1) > 0.001 )ads_printf(L"[半径%0.2lfm]前缓长[%0.1lfm]不满足规范要求,规范规定为[%0.1lfm]\n",JDarray[i].R,JDarray[i].l1,DesignL);

		//	if( fabs( DesignL-JDarray[i].l2) > 0.001 )ads_printf(L"[半径%0.2lfm]前缓长[%0.1lfm]不满足规范要求,规范规定为[%0.1lfm]\n",JDarray[i].R,JDarray[i].l2,DesignL);
	}
}


//设置颜色和线宽
void GTZX_JYX_ROAD::setColorLineW()
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
				if( XLShuXing == SetLineAttrArrayTmp[i].XLName ) 
				{
					AllColor = SetLineAttrArrayTmp[i].ColorIndex ;
					LineWeightValue =  TranLineWeightFromIntToEnum(SetLineAttrArrayTmp[i].LineW) ;
					IsHasSet = TRUE ;
				}
			}
		}

		//如果用户没有设置
		if(!IsHasSet)
		{
			if (XLShuXing=="既有基线")
				AllColor = 0 ;
			else if (XLShuXing=="贯通设计基线")
				AllColor = 3;
			else if (XLShuXing=="施工便线")
				AllColor = 4;
			else
				AllColor = 0;
			LineWeightValue =  TranLineWeightFromIntToEnum(0) ;
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
AcDb::LineWeight GTZX_JYX_ROAD::TranLineWeightFromIntToEnum( int LineWeight)
{
	AcDb::LineWeight LW;
	switch(LineWeight)
	{
	case -1:
		LW = AcDb::kLnWtByLayer;
		break;
	case -2:
		LW = AcDb::kLnWtByBlock;
		break;
	case -3:
		LW = AcDb::kLnWtByLwDefault;
		break;
	case 0:
		LW = AcDb::kLnWt000;
		break;
	case 5:
		LW = AcDb::kLnWt005;
		break;
	case 9:
		LW = AcDb::kLnWt009;
		break;
	case 13:
		LW = AcDb::kLnWt013;
		break;
	case 15:
		LW = AcDb::kLnWt015;
		break;
	case 18:
		LW = AcDb::kLnWt018;
		break;
	case 20:
		LW = AcDb::kLnWt020;
		break;
	case 25:
		LW = AcDb::kLnWt025;
		break;
	case 30:
		LW = AcDb::kLnWt030;
		break;
	case 35:
		LW = AcDb::kLnWt035;
		break;
	case 40:
		LW = AcDb::kLnWt040;
		break;
	case 50:
		LW = AcDb::kLnWt050;
		break;
	case 53:
		LW = AcDb::kLnWt053;
		break;
	case 60:
		LW = AcDb::kLnWt060;
		break;
	case 70:
		LW = AcDb::kLnWt070;
		break;
	case 80:
		LW = AcDb::kLnWt080;
		break;
	case 90:
		LW = AcDb::kLnWt090;
		break;
	case 100:
		LW = AcDb::kLnWt100;
		break;
	case 106:
		LW = AcDb::kLnWt106;
		break;
	case 120:
		LW = AcDb::kLnWt120;
		break;
	case 140:
		LW = AcDb::kLnWt140;
		break;
	case 158:
		LW = AcDb::kLnWt158;
		break;
	case 200:
		LW = AcDb::kLnWt200;
		break;
	case 211:
		LW = AcDb::kLnWt211;
		break;
	default:
		LW = AcDb::kLnWtByLayer;
		break;
	}
	return LW;
}
Adesk::Boolean GTZX_JYX_ROAD::DrawGTZXPMXL(AcGiWorldDraw* pWd)
{
	NJD = JDarray.GetSize();

	int i=0,j=0,k=0;

	if(NJD<1) return FALSE;
	// 绘交点连线

	if ( mode_display == 0)
	{
		//绘制平面交点连线
		if (IsdrawJDLX)
		{
			DrawPMCSSZ(pWd);
		}



		// 交换断链数据
		if(getpxy_DATA()<1)
			return 0;

		//把平面计算的QXB + JYQXB 组合成  QXB
		//	MergeQXBandJYQXBToQXB();

		// 计算T长
		CalT1T2L();

		SortBZLCArray();
		//重新计算标志数组参数

		if(!BZLCarray.IsEmpty())
			OnReCalBZLC(BZLCarray) ;

		//绘制平面
		DrawPMXL(pWd);

		SetBZLCVal();
		if (IsdrawLC)
		{
			BZLC(pWd,100,draw_zybz,2 );    // 标注 百米标 里程

			BZLC2(pWd,100,draw_zybz,2);   //标志里程
		}

		//BZLC(pWd,100,draw_zybz,2 );    // 标注 百米标 里程

		//新线
		//BZTZD( pWd, 100,0,2);  //  标注特征点里程

		if (IsdrawTZD)
		{
			BZTZD(pWd, 100,0,2);  //  标注特征点里程
		}
		//既有
		//	BZTZD2( pWd, 100,0,2);  //  标注特征点里程



		//	if(XLShuXing=="")
		//影响范围
		DrawBandEPoints(pWd);

	};

	return TRUE;
}




void GTZX_JYX_ROAD::GetArrayCS(CArray<QXBStruct,QXBStruct>& array,ACHAR CurJD[20],int &firstNum,int &AllNum)
{
	AllNum = 0;
	firstNum = -1;
	for(int k=0;k<array.GetSize();k++)
	{
		if(_tcscmp(array[k].JDNum ,CurJD)==0)
		{
			if(AllNum==0)firstNum = k;
			AllNum++;
		}
	}

}



Acad::ErrorStatus GTZX_JYX_ROAD::explode(AcDbVoidPtrArray& entitySet)  
{   // AcDbVoidPtrArray entset;
	AcDbObjectId entId;

	AcGePoint3dArray  pArr;
	int i=0,j=0,k=0,trow=0;
	double INDATA[5],LCB;
	CArray<QXBStruct,QXBStruct> qxb; // arrayd[MAXJDNUM][6]

	QXBStruct Tempqxb;

	CArray<JDarrayStruct,JDarrayStruct> arrayd;
	xlpoint  PB,PE; 
	ACHAR GCH[10];

	for (i=0;i<NDL;i++)  
	{  
		DLBTMP[i].BLC=DLB[i].BLC;   
		_tcscpy(DLBTMP[i].BGH,DLB[i].BGH); 
		DLBTMP[i].DL=DLB[i].DL;     
		_tcscpy(DLBTMP[i].EGH,DLB[i].EGH); 
	};

	pArr.setLogicalLength(NJD);
	for (i = 0; i < NJD; i++) pArr[i].set(JDarray[i].E,JDarray[i].N,0.0);

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
	{   
		for (k =0; k <4; k++) 	
		{
			for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
			//G_WXY(pWd,INDATA,&PB,&PE ); 
			entId=GUXWXY(INDATA,&PB,&PE,1); 

			PB=PE;  };
	};

	LCB=array_pxy[0][0][4];  // 起点里程

	/*    //  起点
	for (j=0;j<5;j++)array[0][j]=array_pxy[0][0][j]; 
	trow=1;
	for( i=1;i<NJD-1;i++) 
	{ for (k=0;k<4;k++) 	   { 
	for (j=0;j<5;j++)array[trow][j]=array_pxy[i][k][j]; 
	// ads_printf(L"\n %1.0f %10.3f %10.3f %1.0f",array[trow][0],array[trow][1],array[trow][2],array[trow][4]);
	trow=trow+1;  	};  		   
	};
	//  终点
	for (j=0;j<5;j++)array[trow][j]=array_pxy[NJD-1][0][j]; 	   
	trow=trow+1;
	*/
	_tcscpy(GCH,DLB[0].EGH);
	//    BZLC1( array[400][10],tarray,GCH,NDL,NLC,texth,100.0, LCB,-1,2) ;
	//	BZTZD1(array[400][10],tarray,GCH, NDL,texth, LCB, draw_zybz ,2,NLC); 
	for (i =0; i < NJD; i++) 
	{   
		// for (k =0; k <7; k++) arrayd[i][k]=JDarray[i][k]; 	
		_tcscpy(arrayd[i].JDNum , JDarray[i].JDNum);
		arrayd[i].N=JDarray[i].N;
		arrayd[i].E=JDarray[i].E;
		arrayd[i].l1=JDarray[i].l1;
		arrayd[i].R=JDarray[i].R;
		arrayd[i].l2=JDarray[i].l2;

	}

	qxb.RemoveAll();
	for (i =0; i < NJD; i++) 
	{ 
		//  for (k =0; k <9; k++) qxb[i][k]=QXB[i][k];

		Tempqxb.KZH=QXB[i].KZH;
		Tempqxb.KHY=QXB[i].KHY;
		Tempqxb.KYH=QXB[i].KYH;
		Tempqxb.KHZ=QXB[i].KHZ;
		Tempqxb.a=QXB[i].a;
		Tempqxb.T1=QXB[i].T1;

		Tempqxb.T2=QXB[i].T2;
		Tempqxb.LorR=QXB[i].LorR;
		_tcscpy(Tempqxb.JDNum,QXB[i].JDNum);

		qxb.Add(Tempqxb);

	};
	//     _tcscpy(JDGHTMP,JDGH);
	BZJD1(arrayd,qxb,NJD,texth,draw_zybz,qxbbz,NLC);

	// entitySet=entset;
	return Acad::eOk;

}


Acad::ErrorStatus GTZX_JYX_ROAD::subGetOsnapPoints( AcDb::OsnapMode osnapMode,int gsSelectionMark,
												   const AcGePoint3d& pickPoint,
												   const AcGePoint3d& lastPoint,
												   const AcGeMatrix3d& viewXform,
												   AcGePoint3dArray& snapPoints,
												   AcDbIntArray& geomIds) const 
{    
	assertReadEnabled();
	Acad::ErrorStatus es=Acad::eOk;
	if (gsSelectionMark==0) return Acad::eOk;
	if (osnapMode!=AcDb::kOsModeEnd &&osnapMode!=AcDb::kOsModeNear
		&& osnapMode != AcDb::kOsModeMid
		&&osnapMode!=AcDb::kOsModePerp &&osnapMode!=AcDb::kOsModeCen
		&&osnapMode!=AcDb::kOsModeIns  ) 
	{ 
		return Acad::eOk;  
	};

	AcGePoint3d Pt;

	int i,j,k;
	double l=0.0,INDATA[5] ;
	xlpoint  PB,PE,PZ;   

	switch (osnapMode) 
	{
	case AcDb::kOsModeEnd :
		// 交点
		// ads_printf(L"\n NJD=%d",NJD);
		for ( i = 0; i < NJD; i++) 
		{  
			Pt.set(JDarray[i].E,JDarray[i].N,0.0);
			snapPoints.append(Pt);
		};

		//  园弧的终点
		PB.x=array_pxy[0][0][1]; //N
		PB.y=array_pxy[0][0][2]; // E
		PB.a=  rad(array_pxy[0][0][3]);// FA
		PB.lc=array_pxy[0][0][4];
		PB.r=1e40;
		PE.x=PE.y =PE.lc=PE.a=0.0;
		for (i =1; i < NJD; i++) 
		{    
			for (k =0; k <4; k++) 	
			{
				for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
				l=  Get_INDATA_L(INDATA,&j);
				xlzbxy(&PB,&PE,l,INDATA,0);// 某线元终点
				Pt[X]=PE.y;Pt[Y]=PE.x; Pt[Z]=0.0;
				snapPoints.append(Pt);
				/*
				xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // 某线元中点
				Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
				if (j==2) // 园弧 中点
				{ snapPoints.append(Pt);	};
				*/
				PB=PE;  
			};
		};
		//  断链点
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
		{    
			for (k =0; k <4; k++) 	
			{
				for (j=0; j <5; j++) INDATA[j]=array_pxy[i][k][j]; 
				l=  Get_INDATA_L(INDATA,&j);
				xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // 某线元中点
				Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
				if (j==2) // 园弧 中点
				{ 
					snapPoints.append(Pt);	
				};
				xlzbxy(&PB,&PE,l,INDATA,0);// 某线元终点
				PB=PE;  
			};
		};
		break;

	default :       
		return Acad::eOk;
	}

	return Acad::eOk;
}


// Transform points and create a polyline out of them.
//
Acad::ErrorStatus  GTZX_JYX_ROAD::subGetTransformedCopy(
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

Acad::ErrorStatus GTZX_JYX_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
{

	assertWriteEnabled();

	int j,k;
	double dd;
	ACHAR  *chr=NULL;
	JDarray.RemoveAll();

	AcDbObject::dwgInFields(pFiler);
	// For wblock filing we wrote out our owner as a hard
	// pointer ID so now we need to read it in to keep things
	// in sync.
	//

	if (pFiler->filerType() == AcDb::kWblockCloneFiler) {
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}

	ACHAR jdstr[20];


	pFiler->readItem(&isConnectNewGTZX);

	pFiler->readItem(&AllColor);
	pFiler->readItem(&GTZXJDToJDColor);
	pFiler->readItem(&XWArcColor);
	pFiler->readItem(&HuanHequxianColor);
	pFiler->readItem(&JiaZhiXianColorZuo);
	pFiler->readItem(&JiaZhiXianColorYou);
	pFiler->readItem(&mdbnameBoxColor);
	pFiler->readItem(&mdbnameTextColor);
	pFiler->readItem(&QXYSColor);

	pFiler->readItem((int *)&LineWeightValue);

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

	JDarray.SetSize(NJD);

	for (j=0;j<NJD;j++)
	{   
		pFiler->readItem(&chr);
		_tcscpy(JDarray[j].JDNum,chr);

		pFiler->readItem( &JDarray[j].N);
		pFiler->readItem( &JDarray[j].E);
		pFiler->readItem( &JDarray[j].l1);
		pFiler->readItem( &JDarray[j].R);
		pFiler->readItem( &JDarray[j].l2);
		pFiler->readItem( &JDarray[j].isGJ);
	};


	for (j=0;j<NDL;j++)
	{    
		pFiler->readItem(&chr); _tcscpy(DLB[j].BGH,chr);  
		pFiler->readItem(&dd); DLB[j].BLC=dd;
		pFiler->readItem(&chr  ); _tcscpy(DLB[j].EGH,chr );
		pFiler->readItem(&dd ); DLB[j].DL=dd;

	};

	pFiler->readItem(&NBZLC);

	BZLCarray.RemoveAll();
	BZLCarray.SetSize(NBZLC);

	for (j=0;j<NBZLC;j++)
	{
		pFiler->readString(&chr);
		_tcscpy(BZLCarray[j].kml,chr);
		pFiler->readItem(&BZLCarray[j].X);
		pFiler->readItem(&BZLCarray[j].Y);
	}

	ACHAR *tempACHAR;

	//pFiler->readString(&tempACHAR);
	//_tcscmp(m_AddJDGH , tempACHAR);


	pFiler->readItem(&GJDInforArrayNum);
	GJDInforArray.SetSize(GJDInforArrayNum);
	for (j=0;j<GJDInforArrayNum;j++)
	{   
		pFiler->readItem( &GJDInforArray[j].spt[0]);
		pFiler->readItem( &GJDInforArray[j].spt[1]);
		pFiler->readItem( &GJDInforArray[j].ept[0]);
		pFiler->readItem( &GJDInforArray[j].ept[1]);
		pFiler->readItem( &GJDInforArray[j].GJDType);

		pFiler->readItem(&chr); 
		_tcscpy(GJDInforArray[j].BXLName , chr); 

		pFiler->readItem(&chr); 
		_tcscpy(GJDInforArray[j].EXLName , chr); 

		pFiler->readItem(&chr); 
		_tcscpy(GJDInforArray[j].gjdname , chr); 

		//起点里程
		pFiler->readItem(&chr); 
		_tcscpy(GJDInforArray[j].GJDBLC , chr); 

		//终点里程
		pFiler->readItem(&chr); 
		_tcscpy(GJDInforArray[j].GJDELC , chr); 

	}

	pFiler->readItem(&MidPointArrayNum);
	MidPointArray.SetSize(MidPointArrayNum);
	for (j=0;j<MidPointArrayNum;j++)
	{   
		pFiler->readItem( &MidPointArray[j].x );
		pFiler->readItem( &MidPointArray[j].y );
		pFiler->readItem( &MidPointArray[j].z );
	}

	pFiler->readItem(&IsReSetColor);

	pFiler->readItem(&m_XLNameNotePt);


	pFiler->readItem(&chr); 
	YCXLName = chr ;
	pFiler->readItem(&chr); 
	JRXLName = chr ;

	pFiler->readItem(&IsEXRXD);

	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus GTZX_JYX_ROAD::dwgOutFields(AcDbDwgFiler* pFiler)  const
{
	assertReadEnabled();
	int j,k;
	double dd;
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());

	ACHAR JDCString[40];

	pFiler->writeItem(isConnectNewGTZX);

	pFiler->writeItem(AllColor);
	pFiler->writeItem(GTZXJDToJDColor);
	pFiler->writeItem(XWArcColor);
	pFiler->writeItem(HuanHequxianColor);
	pFiler->writeItem(JiaZhiXianColorZuo);
	pFiler->writeItem(JiaZhiXianColorYou);
	pFiler->writeItem(mdbnameBoxColor);
	pFiler->writeItem(mdbnameTextColor);
	pFiler->writeItem(QXYSColor);


	pFiler->writeItem((int )LineWeightValue);

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
		_tcscpy(JDCString,JDarray[j].JDNum);
		pFiler->writeItem(JDCString);
		pFiler->writeItem( JDarray[j].N);
		pFiler->writeItem( JDarray[j].E);
		pFiler->writeItem( JDarray[j].l1);
		pFiler->writeItem( JDarray[j].R);
		pFiler->writeItem( JDarray[j].l2);
		pFiler->writeItem( JDarray[j].isGJ);

	};


	const ACHAR *chr=NULL;
	for (j=0;j<NDL;j++)
	{     
		chr=_tcsdup(DLB[j].BGH );  pFiler->writeItem(chr );
		dd=DLB[j].BLC; pFiler->writeItem( dd);
		chr=_tcsdup(DLB[j].EGH );  pFiler->writeItem(chr );
		dd=DLB[j].DL;  pFiler->writeItem( dd);

	};


	pFiler->writeItem(NBZLC);

	for (j=0;j<NBZLC;j++)
	{   
		pFiler->writeString(BZLCarray[j].kml);
		pFiler->writeItem(BZLCarray[j].X);
		pFiler->writeItem(BZLCarray[j].Y);
	};

	//	pFiler->writeString(m_AddJDGH);

	pFiler->writeItem(GJDInforArrayNum);

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


	pFiler->writeItem(IsReSetColor);


	pFiler->writeItem(m_XLNameNotePt);

	pFiler->writeString((LPCTSTR)YCXLName);
	//	YCXLName = chr ;
	pFiler->writeString((LPCTSTR)JRXLName);
	//	JRXLName = chr ;

	pFiler->writeItem(IsEXRXD);

	return pFiler->filerStatus();
}



// Files data in from a DXF file.
//
Acad::ErrorStatus GTZX_JYX_ROAD::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	const ACHAR *chr=NULL;
	Acad::ErrorStatus es;
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }
	// Check if we're at the right subclass getData marker.

	if (!pFiler->atSubclassData(L"GTZX")) {
		ads_printf(L"\n error 10010 类型有错 \n");
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
	//if  ( es == Acad::eOk)
	//{
	//	if ( inbuf.restype == AcDb:: kDxfInt16+8)
	//	{
	//		JD0  = inbuf.resval.rint;
	//	}
	//	else		{		}
	//   } 

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

		//for (k=0;k<=6;k++)  
		//{
		//      es = pFiler->readItem(&inbuf);
		//   if  ( es == Acad::eOk)
		//{
		//  if ( inbuf.restype == AcDb:: kDxfReal+1 )
		//  {
		//JDarray[j][k] = inbuf.resval.rreal;
		//  }
		//  else		   {		   }
		//}
		//};


		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{
				CString TempCString;
				TempCString.Format(L"%lf",inbuf.resval.rreal);
				_tcscpy(JDarray[j].JDNum , TempCString);
			}
			else		   {		   }
		}


		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{
				JDarray[j].N = inbuf.resval.rreal;
			}
			else		   {		   }
		}

		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{
				JDarray[j].E = inbuf.resval.rreal;
			}
			else		   {		   }
		}

		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{
				JDarray[j].l1 = inbuf.resval.rreal;
			}
			else		   {		   }
		}

		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{
				JDarray[j].R = inbuf.resval.rreal;
			}
			else		   {		   }
		}

		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{
				JDarray[j].l2 = inbuf.resval.rreal;
			}
			else		   {		   }
		}

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
Acad::ErrorStatus GTZX_JYX_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
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
	//    pFiler->writeItem(AcDb::kDxfInt16+8,JD0);
	pFiler->writeItem(AcDb::kDxfInt16+9,pro);
	pFiler->writeItem(AcDb::kDxfInt16+10,Zx[0]);
	pFiler->writeItem(AcDb::kDxfInt16+11,Zx[1]);
	//	acutPrintf(L"out %d,%d,%d,%d,%d,%d,%d,%d,%d\n",NJD,NDL,NLC,NH,Chinese_Engbz_type,draw_zybz,JDTDBZ,qxbbz,JD0);

	pFiler->writeItem(AcDb::kDxfReal,texth);

	for (j=0;j<NJD;j++)
	{
		//  for (k=0;k<=6;k++) 	
		//  {
		pFiler->writeItem(AcDb::kDxfReal+1,JDarray[j].JDNum);
		pFiler->writeItem(AcDb::kDxfReal+1,JDarray[j].N);
		pFiler->writeItem(AcDb::kDxfReal+1,JDarray[j].E);
		pFiler->writeItem(AcDb::kDxfReal+1,JDarray[j].l1);
		pFiler->writeItem(AcDb::kDxfReal+1,JDarray[j].R);
		pFiler->writeItem(AcDb::kDxfReal+1,JDarray[j].l2);

		//	    ads_printf(L"k=%d j=%d %d %lf\n",j,k,k+5*j+1,JDarray[j][k]);	
		//  };
	};

	//	ads_printf(L"dxf WW NJD=%d,NDL=%d NLC=%d NH=%d Chinese_Engbz_type=%d",NJD,NDL,NLC,NH,Chinese_Engbz_type); 

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


Acad::ErrorStatus GTZX_JYX_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
												  AcDbIntArray& osnapModes,
												  AcDbIntArray& geomIds)const
{


	AcGePoint3d Pt;
	int i,j,k;
	double l=0.0,INDATA[5],X1,Y1;
	xlpoint  PB,PE,PZ;   

	assertReadEnabled();
	// 将交点加入夹点集，序号0——NJD-1
	// ads_printf(L"\n NJD=%d",NJD);
	for (i = 0; i < NJD; i++) 
	{  Pt.set(JDarray[i].E,JDarray[i].N,0.0);
	gripPoints.append(Pt);
	};
	// 将切线边的中点加入夹点集，序号（NJD-1）+1——（NJD-1）+NJD-1
	// ads_printf(L"\n NJD=%d",NJD);
	for ( i = 0; i < NJD-1; i++) 
	{  X1=(JDarray[i].E+JDarray[i+1].E)*0.5;
	Y1=(JDarray[i].N+JDarray[i+1].N)*0.5;
	Pt.set(X1,Y1,0.0);
	gripPoints.append(Pt);
	};
	// 将各交点对应的圆弧中点加入夹点集，序号（NJD-1）+（NJD-1）+1——（NJD-1)+（NJD-1）+NJD-2
	//  园弧的终点
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
		xlzbxy(&PB,&PE,l,INDATA,0);// 某线元终点
		xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // 某线元中点
		Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
		if (j==2) // 园弧 中点
		{ gripPoints.append(Pt);	};
		PB=PE;  
	};
	};


	for(i=0;i<MidPointArray.GetSize();i++)
		gripPoints.append(MidPointArray[i]);

	gripPoints.append(m_XLNameNotePt);

	/*
	// 将各交点对应的缓和曲线中点加入夹点集，
	// 序号
	// 园弧的终点
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
	xlzbxy(&PB,&PE,l,INDATA,0);// 某线元终点
	xlzbxy(&PB,&PZ,l*0.5,INDATA,0); // 某线元中点
	Pt[X]=PZ.y;Pt[Y]=PZ.x; Pt[Z]=0.0;
	if (j==3||j==4) // 缓和曲线 中点
	{ gripPoints.append(Pt);	};
	PB=PE;  
	};
	};
	//  断链点
	for (i=1;i<NDL-1;i++)
	{	};
	*/
	return Acad::eOk;
}


Acad::ErrorStatus GTZX_JYX_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset)
{
	AcGePoint3d startPt,endPt,centerPt;
	assertWriteEnabled();
	int  j,k,bz ;
	double R, dd,DD01,DD02,DD1,DD2,fb,d1,d2,fb1,fb2, dx,dy;
	CString  TempString;
	//	JDTDBZ=4;


	//   int RN=1;
	//	if( (RN<1)||(RN>2)) RN=1;

	for ( j=0;j<indices.length();j++)
	{
		// 交点序号 0~NJD-1
		if  (indices[j]<NJD) 
		{   // 所选择的夹点为 某一交点（包括起点和终点）
			bz=1;
			k=indices[j];  //交点序号
			dx=offset[X];dy=offset[Y];

			if (JDTDBZ==1)  //沿切线拖动
			{
				if (k<=NJD-2)
				{ 
					DD02 =xyddaa(JDarray[k].N,JDarray[k].E, JDarray[k+1].N,JDarray[k+1].E,&fb2);
					DD2=xyddaa(JDarray[k].N+dy,JDarray[k].E+dx, JDarray[k+1].N,JDarray[k+1].E,&fb);
					//  由直线段(大地坐标)[(XB,YB),(XE,YE)]的长度和方位角  返回:距离， *方位角 
					d2=DD02-DD2;
				} 
				else {  d2=0.0; fb2=0.0; }
				if (k>0)
				{
					DD01 =xyddaa(JDarray[k-1].N,JDarray[k-1].E, JDarray[k].N,JDarray[k].E,&fb1);
					DD1=xyddaa(JDarray[k-1].N,JDarray[k-1].E,JDarray[k].N+dy,JDarray[k].E+dx,&fb);
					//  由直线段(大地坐标)[(XB,YB),(XE,YE)]的长度和方位角  返回:距离， *方位角 
					d1=DD1-DD01;
				}else {  d1=0.0; fb1=0.0; }
				dd=d1; fb=fb1;
				if (fabs(d1)<fabs(d2)) { dd=d2; fb=fb2;};
				//E
				JDarray[k].E=JDarray[k].E+dd*sin(fb);
				//N
				JDarray[k].N=JDarray[k].N+dd*cos(fb);
				//	ads_printf(L"  DT=%-10.4f",dd);
			}
			else if(JDTDBZ==2 || JDTDBZ==3 || JDTDBZ==4)
			{
				double OutN,OutE;
				OutN=JDarray[k].N+offset[Y];
				OutE=JDarray[k].E+offset[X];

				if (JDTDBZ==2 || JDTDBZ==4)  // 切前圆
				{

					if(k>1)//不是起点
					{
						double Lo1,Lo2,R1,p1,tR1,A2;//前交点后缓，前圆半径,内移量,前圆加内移量的大圆半径

						Lo1=JDarray[k-1].l1;
						Lo2=JDarray[k-1].l2;
						R1=JDarray[k-1].R;
						if(Lo2>0.0)
						{
							A2=sqrt(R1*Lo2);
							double x=spiral_y(A2,Lo2);
							double t=Lo2/(2.0*R1);
							double dr=x+R1*sin(t+m_dHalfPI);			
							p1=dr-R1;
						}
						else p1=0.0;					
						//半径+内移量					
						tR1=R1+p1;
						//求前圆圆心
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						N1=JDarray[k-2].N;
						E1=JDarray[k-2].E;
						N0=JDarray[k-1].N;
						E0=JDarray[k-1].E;
						N2=JDarray[k].N;
						E2=JDarray[k].E;
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R1,Lo1,Lo2,CenN,CenE);
						double TN,TE;//切点
						//求切点
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR1,OutN,OutE,LRFlag,TN,TE);
						BAS_DRAW_FUN_JYX::INTERS(&JDarray[k-1].N,&JDarray[k-1].E,N1,E1,N0,E0,TN,TE,OutN,OutE);
					}				
				}
				if (JDTDBZ==3 || JDTDBZ==4)  // 切后圆
				{
					if(k<NJD-2)//不是终点
					{
						double Lo1,Lo2,R2,p2,tR2,A1;//前交点后缓，前圆半径,内移量,前圆加内移量的大圆半径

						Lo1=JDarray[k+1].l1;
						Lo2=JDarray[k+1].l2;
						R2=JDarray[k+1].R;
						if(Lo1>0.0)
						{
							A1=sqrt(R2*Lo1);
							double x=spiral_y(A1,Lo1);
							double t=Lo1/(2.0*R2);
							double dr=x+R2*sin(t+m_dHalfPI);			
							p2=dr-R2;
						}
						else p2=0.0;					
						//半径+内移量					
						tR2=R2+p2;
						//求前圆圆心
						double CenN,CenE;
						double N1,E1,N0,E0,N2,E2;
						N1=JDarray[k].N;
						E1=JDarray[k].E;
						N0=JDarray[k+1].N;
						E0=JDarray[k+1].E;
						N2=JDarray[k+2].N;
						E2=JDarray[k+2].E;
						GetCenter3JD(N1,E1,N0,E0,N2,E2,R2,Lo1,Lo2,CenN,CenE);
						//acutPrintf(L"CenE:%lf,%lf\n",CenE,CenN);
						double TN,TE;//切点
						//求切点
						int LRFlag=GetLeftOrRight3JD(N1,E1,N0,E0,N2,E2);
						GetQD(CenN,CenE,tR2,OutN,OutE,-1*LRFlag,TN,TE);
						BAS_DRAW_FUN_JYX::INTERS(&JDarray[k+1].N,&JDarray[k+1].E,N2,E2,N0,E0,TN,TE,OutN,OutE);
					}				
				}
				//(JDTDBZ==4)// 切前、后两圆
				//当前交点新位置
				JDarray[k].E+=offset[X];//x					
				JDarray[k].N+=offset[Y];//y
			}
			else   //任意拖动
			{  //x
				JDarray[k].E=JDarray[k].E+offset[X];
				//y
				JDarray[k].N=JDarray[k].N+offset[Y];
			}

		}
		else if ((indices[j]>=NJD)&&(indices[j]<2*NJD-2))  // 所选择的夹点为  某一切线的中点
		{   // (NJD-1)+（NJD-2）+1--(NJD-1)+（NJD-2）+NJD-1
			bz=2;
			k=indices[j]-NJD+1; // 该切线边对应的交点号(后点）
			dd=sqrt(offset[X]*offset[X]+offset[Y]*offset[Y]);					   

			if(k<=NJD-2||k>0)
			{
				DD01 =xyddaa(JDarray[k-1].N,JDarray[k-1].E, JDarray[k].N,JDarray[k].E,&fb1);

				DD02 =xyddaa(JDarray[k].N,JDarray[k].E, JDarray[k+1].N,JDarray[k+1].E,&fb2);
				fb=fb2;	
				JDarray[k].E=JDarray[k].E+dd*sin(fb);
				//N
				JDarray[k].N=JDarray[k].N+dd*cos(fb);

				JDarray[k-1].E=JDarray[k-1].E-dd*sin(fb1);
				//N
				JDarray[k-1].N=JDarray[k-1].N-dd*cos(fb1);	    
				//s_printf("  DT=%-10.4f",dd);
			};
			//	ads_printf(L"\n 交点=%d, X=%10.2f Y=%10.2f  dd=%10.2f",k,offset[X],offset[Y],dd);
		}
		else if ((indices[j]>=2*NJD-1)&&(indices[j]<=3*NJD-4))  // 所选择的夹点为  某一园弧的中点
		{   // (NJD-1)+1--(NJD-1)+NJD-1
			bz=2;
			k=indices[j]-2*NJD+2; // 该园弧对应的交点号
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
				double tmp,Pj,startfw,endfw;
				calptan(JDarray[k-1].N,JDarray[k-1].E,JDarray[k].N,JDarray[k].E,JDarray[k+1].N,JDarray[k+1].E,xt,yt,&tmp,&R,&tmp,&tmp,&tmp,&startfw,&endfw,&Pj);
				//*****

				//	Pj = Pj * 180.0/PI;
				//	ads_printf(L"*****  Pj=%lf\n",Pj);

				//    R = get_R(R,DV);
				JDarray[k].R = R;
				//  JDarray[k][3] = get_l0(R,DV);
				//	JDarray[k][5] = get_l0(R,DV);

				double fb,fe;
				double ddb=xyddaa(JDarray[k-1].N,JDarray[k-1].E,JDarray[k].N,JDarray[k].E,&fb);
				double dde=xyddaa(JDarray[k].N,JDarray[k].E,JDarray[k+1].N,JDarray[k+1].E,&fe);
				double aa=fe-fb;
				if (aa<-pi) aa=aa+2*pi;
				if  (aa>pi) aa=aa-2*pi;
				aa=fabs(aa);
				aa=dms_rad(aa);
				double BPMTL[10];
				getBPMTL(aa,JDarray[k].R,JDarray[k].l1,JDarray[k].l2,&BPMTL[0]);
				double T1=BPMTL[6];
				double T2=BPMTL[7];//L=BPMTL[8];

				double Lmin=m_GuiFanCS.MinZhi;
				if(k==1)
				{
					if(T1>ddb)
					{
						JDarray[k-1].N=JDarray[k].N-(T1+Lmin)*cos(fb);
						JDarray[k-1].E=JDarray[k].E-(T1+Lmin)*sin(fb);
					}
				}
				if(k==NJD-2)
				{
					if(T2>dde)
					{
						JDarray[k+1].N=JDarray[k].N+(T2+Lmin)*cos(fe);
						JDarray[k+1].E=JDarray[k].E+(T2+Lmin)*sin(fe);
					}
				}
			}

		}

		else if (indices[j]==3*NJD-3+MidPointArray.GetSize())
		{
			m_XLNameNotePt.x += offset.x;
			m_XLNameNotePt.y += offset.y;
		}

		else if (MidPointArray.GetSize()>0 && indices[j]>=3*NJD-3 && indices[j]<=3*NJD-3+MidPointArray.GetSize())
			//改建段信息数组
		{
			k=indices[j]-3*NJD+3;

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
			{//起点
				GJDInforArray[kk].spt[0] = PZtmp.x ;
				GJDInforArray[kk].spt[1] = PZtmp.y ;
			}
			else
			{//终点
				GJDInforArray[kk].ept[0] = PZtmp.x ;
				GJDInforArray[kk].ept[1] = PZtmp.y ;
			}
		}

		else
		{
			ads_printf(L"\n夹点序号错误！");
		}
		//终点
		if(getpxy_DATA()<1)
			return Acad::eOk;


		CString TempCString;

	};

	return Acad::eOk;
}

void GTZX_JYX_ROAD::subList() const 
{ 
	int i,j;
	double dd;
	assertReadEnabled();
	ads_printf(L"\n 类：\t%s",isA()->name());


	for ( i = 0; i < NJD; i++) 
	{
		ads_printf(L"\n N%10.4f E%10.4f L1=%10.2f R=%10.2f L2=%10.2f", JDarray[i].N,JDarray[i].E,JDarray[i].l1,JDarray[i].R,JDarray[i].l2);  };
	// 曲线表
	ads_printf(L"\n  起点里程QD=%-s%10.4f",DLB[0].EGH,QXB[0].KZH);
	for ( i = 1; i < NJD-1; i++) 
	{  
		j=(int) (QXB[i].a+0.00001);
		ads_printf(L"\n  ZH=%-s%10.4f  HY=%10.4f YH=%10.4f HZ=%10.4f", DLB[0].EGH,QXB[i].KZH,QXB[i].KHY,QXB[i].KYH,QXB[i].KHZ);  
	};

	ads_printf(L"\n  终点里程ZD=%-s%10.4f",DLB[0].BGH,QXB[NJD-1].KZH);
	//断链表(含起终点)
	for ( i =0; i < NDL ; i++) 
	{  
		dd=DLB[i].BLC+DLB[i].DL;
		ads_printf(L"\n  I=%d 断前里程BLC=%-s%10.4f 断后里程ELC=%-s%10.4f 断链=%10.4f ", 
			i,DLB[i].BGH,DLB[i].BLC,DLB[i].EGH,dd,DLB[i].DL);  
	};
}

void  GTZX_JYX_ROAD::setINIT(int nlc,int nh,int Chinese_Engbz,double Texth)
{
	assertWriteEnabled();
	NLC=nlc;
	NH=nh;
	texth=Texth;
	Chinese_Engbz_type=Chinese_Engbz;

}

void  GTZX_JYX_ROAD::setTD(short int JDTDBZ0, short int draw_zybz0, short int qxbbz0 ) 
	// JDTDBZ 交点拖动标志 ，JDTDBZ==1为沿切线拖动  =2 为切圆 =其他整数为任意拖动
{
	assertWriteEnabled();
	ads_printf(L"\n JDTDBZ=%d draw_zybz=%d qxbbz=%d",JDTDBZ,draw_zybz,qxbbz);
	JDTDBZ=JDTDBZ0;
	draw_zybz= draw_zybz0;
	qxbbz=qxbbz0;
	ads_printf(L"\n JDTDBZ=%d draw_zybz=%d qxbbz=%d",JDTDBZ,draw_zybz,qxbbz); 
}
//  设置断链数据
Adesk::Boolean  GTZX_JYX_ROAD::setDLM(int iDL,double BLC,double DL, ACHAR *BGH,ACHAR *EGH, int mode )
{   
	if (mode !=1 )
		assertWriteEnabled();
	if (iDL>=0&& iDL<NDL)
	{ 	
		_tcscpy(DLB[iDL].BGH,BGH);	
		_tcscpy(DLB[iDL].EGH,EGH);
		DLB[iDL].BLC=BLC;        	
		DLB[iDL].DL= DL;
		DLB[iDL].ELC = BLC + DL ;   
		//起点
		if (iDL==0) JDarray[0].l1=BLC+DL;
		return 1;
	} else return 0;
}


//  获得一个断链数据
Adesk::Boolean  GTZX_JYX_ROAD::getDLM(int iDL,double *BLC,double *DL, ACHAR *BGH ,ACHAR *EGH )
{   

	if (iDL>=0 && iDL< NDL && NDL<30)  // NDL的最小值为2  最大值为30
	{  	       
		_tcscpy(EGH,DLB[iDL].BGH );
		_tcscpy(EGH,DLB[iDL].EGH );
		*BLC=DLB[iDL].BLC;
		*DL=DLB[iDL].DL;

	}   
	return NDL;
}
//  增加一个断链数据
Adesk::Boolean  GTZX_JYX_ROAD::addDLM(int iDL,double BLC,double DL, ACHAR *EGH  )
	//							 增中断链号，断前里程，断链值
{   
	int i;

	if (iDL>0 && iDL< NDL && NDL<30)  // NDL的最小值为2  最大值为30
	{  	
		if( (BLC>( DLB[iDL-1].BLC+DLB[iDL-1].DL)) && (BLC< DLB[iDL].BLC)) 
		{
			assertWriteEnabled();
			for (i=iDL;i<=NDL-1;i++) DLB[NDL+iDL-i]=DLB[NDL-1+iDL-i] ; // 后移一个断链点
			NDL=NDL+1;

			_tcscpy(DLB[iDL].EGH,EGH);    _tcscpy(DLB[iDL+1].BGH,EGH);
			DLB[iDL].BLC=BLC;        	DLB[iDL].DL= DL;

			DLB[iDL+1].BLC += DL;
			DLB[iDL+1].DL  -= DL;

			for(i=1; i<NDL-1; i++)//对长链特殊处理  DK10+105.43=D1K9+000  D1K10+105.43=DK10+105.43 
			{		                               //改为D1K10+105.44=DK10+105.44
				if(_tcscmp(DLB[i].BGH,DLB[i-1].EGH)!=0)//下一断链BGH应与前一断链EGH一样
					_tcscpy(DLB[i].BGH,DLB[i-1].EGH);
				if(fabs(DLB[i].DL)<0.0001)//当前断炼为0
				{
					if(DLB[i-1].DL < -0.01  //前一断链为长链
						&&fabs(DLB[i-1].BLC-DLB[i].BLC)<0.001)
						DLB[i].BLC +=0.01;
					if(DLB[i+1].DL < -0.01  //后一断链为长链
						&&fabs(DLB[i].BLC-(DLB[i+1].BLC+DLB[i+1].DL))<0.001)
						DLB[i].BLC -=0.01;
				}
			}


			//修改终点（未断链）里程
			//		   DLB[NDL-1].BLC= DLB[NDL-1].BLC+DL;

			return 1;  	}
		else return 0;
	} else return 0;
}


//  删除一个断链数据
Adesk::Boolean  GTZX_JYX_ROAD::DelDLM(int iDL)
{   
	int i;
	double DL;
	if (iDL>0 && iDL< NDL && NDL<30)  // NDL的最小值为2  最大值为30
	{     
		DL=DLB[iDL].DL;
		assertWriteEnabled();

		for (i=iDL;i<=NDL-1;i++) 
			DLB[iDL+i-iDL]=DLB[iDL+1+i-iDL] ; // 前移一个断链点

		NDL=NDL-1;
		_tcscpy(DLB[iDL].BGH,DLB[iDL-1].EGH);

		// DLB[iDL].DL= DLB[iDL].DL+DL ;
		DLB[iDL].BLC= DLB[iDL].BLC-DL;
		DLB[iDL].DL = DLB[iDL].DL+DL;
		//修改终点（未断链）里程
		// DLB[NDL-1].BLC= DLB[NDL-1].BLC-DL;
		return 1;   

	} else return 0;
}

int GTZX_JYX_ROAD::savePM(ACHAR *fname)//2003.10.18改 交点和断链合一
{   
	assertReadEnabled();
	int k,trow=0;
	FILE *fpc;
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"w"))!=NULL)
	{
		//save jd
		fwprintf(fpc,L"%d %s\n",NJD,JDGH);
		//	fwprintf(fpc,L"%d ",(int)(JDarray[trow].JDNum+0.001));  
		//	for (k=1;k<=5;k++) fwprintf(fpc,L" %lf", JDarray[trow][k]);
		fwprintf(fpc,L"\n");
		trow=trow+1;
		do
		{
			//	fwprintf(fpc,L"%d  ",(int)(JDarray[trow][0]+0.001));  
			//	for (k=1;k<=5;k++)  fwprintf(fpc,L"  %15.6f",JDarray[trow][k]);
			//	fwprintf(fpc,L"\n");
			trow++;

		} while(trow<NJD);
		//save DL
		fwprintf(fpc,L"%d\n",NDL);
		for(int i=0;i<NDL;i++)
		{
			//			double BLC,DL;  //  断链的断前里程和断链值(+值为长链;-为断链)
			//			ACHAR   BGH[8],EGH[8]; //  断链的断前里程冠号和断后里程冠号

			fwprintf(fpc,L"%8s %8s %15.6lf %15.6lf\n",DLB[i].BGH,DLB[i].EGH,DLB[i].BLC,DLB[i].DL);			
		}
		fclose(fpc);
	};
	return 1;
}

int GTZX_JYX_ROAD::saveDL(ACHAR *fname)
{   
	assertReadEnabled();////
	int k,trow=0;
	FILE *fpc;
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"w"))!=NULL)
	{
		//save jd
		//	fwprintf(fpc,L"%d %s\n",NJD,JDGH);
		//	fwprintf(fpc,L"%d ",(int)(JDarray[trow][0]+0.001));  
		//	for (k=1;k<=5;k++) fwprintf(fpc,L" %lf", JDarray[trow][k]);
		//	fwprintf(fpc,L"\n");
		trow=trow+1;
		do
		{
			//	fwprintf(fpc,L"%d  ",(int)(JDarray[trow][0]+0.001));  
			//	for (k=1;k<=5;k++)  fwprintf(fpc,L"  %15.6f",JDarray[trow][k]);
			//	fwprintf(fpc,L"\n");
			trow++;

		} while(trow<NJD);
		//save DL
		fwprintf(fpc,L"%d\n",NDL);
		for(int i=0;i<NDL;i++)
		{
			//			double BLC,DL;  //  断链的断前里程和断链值(+值为长链;-为断链)
			//			ACHAR   BGH[8],EGH[8]; //  断链的断前里程冠号和断后里程冠号

			fwprintf(fpc,L"%8s %8s %15.6lf %15.6lf\n",DLB[i].BGH,DLB[i].EGH,DLB[i].BLC,DLB[i].DL);			
		}
		fclose(fpc);
	};
	return 1;
}		

int GTZX_JYX_ROAD::savePXY(ACHAR *fname)
{   
	assertReadEnabled();
	int k,trow=0,i,j;
	FILE *fpc;
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"w"))!=NULL)
	{
		//  起点
		for (k=1;k<=5;k++) fwprintf(fpc,L"  %lf", array_pxy[0][0][k] );
		trow=1;
		for( i=1;i<NJD-1;i++) 
		{ for (k=0;k<4;k++) 	   { 
			for (j=0;j<5;j++)fwprintf(fpc,L" %lf", array_pxy[i][k][j] );
			trow=trow+1;  	}; 		   
		};
		//  终点
		for (j=0;j<5;j++) fwprintf(fpc,L" %lf",array_pxy[NJD-1][0][j]);   
		trow=trow+1;

		fclose(fpc);
	};
	return 1;
}


void GTZX_JYX_ROAD::setJD_RL(int JDi ,double Ri,double L1i,double L2i )
{
	assertWriteEnabled();
	if (JDi>0 && JDi<NJD-1)  // 不为起、终点
	{  JDarray[JDi].l1=L1i;
	JDarray[JDi].l2=L2i;
	JDarray[JDi].R=Ri;
	} ;	 
} 

int GTZX_JYX_ROAD::getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE )
{   
	assertReadEnabled();

	if (JDi>=0 && JDi<=NJD-1)  //  起~终点
	{  
		*NN=JDarray[JDi].N;
		*EE=JDarray[JDi].E;
		*L1i=JDarray[JDi].l1;
		*L2i=JDarray[JDi].l2;
		*Ri=JDarray[JDi].R;
	};	 
	return NJD;
}

int GTZX_JYX_ROAD::get_pxy(int id,double *d0,double *d1,double *d2,double *d3,double *d4)
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

Adesk::Boolean  GTZX_JYX_ROAD::addXL(int JDi,double R,double L1,double L2 ,double NN ,double EE,double Nend ,double Eend )
{   
	assertWriteEnabled();
	//暂删除
	//	JDarray[NJD-1][0]=NJD-1;
	JDarray[NJD-1].N=NN;
	JDarray[NJD-1].E=EE;
	JDarray[NJD-1].l1=L1;
	JDarray[NJD-1].l2=L2;
	JDarray[NJD-1].R=R;
	//	JDarray[NJD  ][0]=NJD;
	JDarray[NJD  ].N=Nend;
	JDarray[NJD  ].E=Eend;
	JDarray[NJD  ].l1=0.0;
	JDarray[NJD  ].R=0.0;
	JDarray[NJD  ].l2=0.0;
	NJD =NJD +1;	  
	return 1;  
} 

//  增加一个交点数据
Adesk::Boolean  GTZX_JYX_ROAD::addJD(int JDi,double R,double L1,double L2 ,double NN ,double EE )
{   
	int i,j;
	if (JDi>0 && JDi<=NJD-1 && NJD<MAXJDNUM)  // 不为起、终点  NJD的最小值为2  最大值为100
	{  	    
		assertWriteEnabled();
		for (i=NJD-1;i>=JDi;i--) // 后移一个 交点
		{
			_tcscpy(JDarray[i+1].JDNum , JDarray[i].JDNum);
			JDarray[i+1].N=JDarray[i].N;
			JDarray[i+1].E=JDarray[i].E;
			JDarray[i+1].l1=JDarray[i].l1;
			JDarray[i+1].R=JDarray[i].R;
			JDarray[i+1].l2=JDarray[i].l2;
		}
		// for ( j=0;j<6;j++) JDarray[i+1][j]=JDarray[i][j];

		NJD =NJD +1;	
		//暂不要
		//   JDarray[JDi][0]=NJD ;
		JDarray[JDi].N=NN;
		JDarray[JDi].E=EE;
		JDarray[JDi].l1=L1;
		JDarray[JDi].l2=L2;
		JDarray[JDi].R=R;
		return 1;  
	} 
	else 
		return 0;
}


//  删除一个交点数据
Adesk::Boolean  GTZX_JYX_ROAD:: DelJD(int JDi )
{   
	int i,j;
	//plh07.19a
	if (JDi>=0 && JDi< NJD && NJD<MAXJDNUM)  // 不为起、终点  NJD的最小值为2  最大值为100
	{  	 
		assertWriteEnabled();
		for (i=JDi;i<NJD-1;i++) // 前移一个 交点
		{
			//  for ( j=0;j<6;j++) JDarray[JDi+(i-JDi)][j]=JDarray[JDi+1+(i-JDi)][j];
			_tcscpy( JDarray[JDi+(i-JDi)].JDNum , JDarray[JDi+1+(i-JDi)].JDNum);
			JDarray[JDi+(i-JDi)].N=JDarray[JDi+1+(i-JDi)].N;
			JDarray[JDi+(i-JDi)].E=JDarray[JDi+1+(i-JDi)].E;
			JDarray[JDi+(i-JDi)].l1=JDarray[JDi+1+(i-JDi)].l1;
			JDarray[JDi+(i-JDi)].R=JDarray[JDi+1+(i-JDi)].R;
			JDarray[JDi+(i-JDi)].l2=JDarray[JDi+1+(i-JDi)].l2;
		}
		if(JDi==0)JDarray[JDi].l1=0.0;
		NJD =NJD -1;	  
		return 1; 
	} else return 0;
}

//	标注交点号和曲线参数
int GTZX_JYX_ROAD::BZJD1(CArray<GTZX_JYX_ROAD::JDarrayStruct,GTZX_JYX_ROAD::JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb,int trow, double texth,int draw_zybz,int qxbbz,int NLC)
{  double fb,fe,de,da,aa,dd,dt ;
AcGePoint3d  p1; 
ACHAR ch[10],ch1[60],chrtmp[80],ch2[10];
int i,c,n ;

ACHAR JDchr[20];
_tcscpy(JDchr,JDGH);

de=xyddaa(arrayd[0].N,arrayd[0].E,arrayd[1].N,arrayd[1].E,&fb);
for (i=1;i<trow-1;i++)
{  
	if (arrayd[i].R<0) break;
	de=xyddaa(arrayd[i].N,arrayd[i].E,arrayd[i+1].N,arrayd[i+1].E,&fe);
	aa= fe - fb ;
	if (aa<-pi) aa=aa+2*pi;
	if  (aa>pi) aa=aa-2*pi;
	if (aa>0) {  c=1; dd=texth;  }  //右偏
	else { c=-1; dd=texth+texth; }; //左偏
	de=fb+aa*0.5;

	/*   ads_rtos(JD0+i,2,0,ch);
	_tcscpy(ch1,JDchr); _tcscat(ch1,ch); */

	_tcscpy(ch1,arrayd[i].JDNum);
	n=_tcslen(ch1); p1[Z]=0.0;

	p1[X]=arrayd[i].E-n*0.5*texth*sin(de)+dd*sin(de-c*pi*0.5) ; 
	p1[Y]=arrayd[i].N-n*0.5*texth*cos(de)+dd*cos(de-c*pi*0.5);

	da= m_dHalfPI-de;
	// G_maketext(pWd,p1,ch1,de,texth ,4,1);   
	maketext( p1,ch1,da,texth ,4,1);  // 交点号

	p1[X]=arrayd[i].E ;  p1[Y]=arrayd[i].N ;
	makecircle(p1,3.6,6);



	if( qxbbz==1)
	{
		// 标曲线参数 begin
		double R,T,T1,T2,L1,L2,LL,DFM;
		R=arrayd[i].R; 
		L1= arrayd[i].l1; 
		L2=arrayd[i].l2; 
		T1=qxb[i].T1;
		T2=qxb[i].T2; 
		LL=qxb[i].L;
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
		p1[X]=arrayd[i].E-dd*sin(de-c*pi*0.5) ; 
		p1[Y]=arrayd[i].N-dd*cos(de-c*pi*0.5);
		_tcscpy(chrtmp,L"  "); _tcscat(chrtmp,ch1);
		maketext(p1,chrtmp,da,texth ,4,1);   // 交点号

		ads_rtos(qxb[i].a,2,0,ch );// 度

		DFM=qxb[i].a-((int)qxb[i].a) ;
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

		maketext(p1,ch1,da,texth ,4,1);   //  偏角

		if (fabs(R-(long int)(R))<0.001)  { ads_rtos(R,2,0,ch); }
		else { ads_rtos(R,2,NLC,ch); };
		_tcscpy(ch1,L"R--"); _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		maketext(p1,ch1,da,texth ,4,1);   //   半径

		if (L1>0||L2>0)  //  缓长不为零
		{
			if (fabs(L1-(long int)(L1))<0.001)  { ads_rtos(L1,2,0,ch); }
			else { ads_rtos(L1,2,NLC,ch); };
			p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
			p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
			if (fabs(L1-L2)<0.001) 
			{ _tcscpy(ch1,L"Lo--");  _tcscat(ch1,ch);
			maketext(p1,ch1,da,texth ,4,1);   //   缓长 相等
			} else
			{ _tcscpy(ch1,L"L1--");  _tcscat(ch1,ch);
			if (fabs(L2-(long int)(L2))<0.001)  { ads_rtos(L2,2,0,ch); }
			else { ads_rtos(L2,2,NLC,ch); };
			_tcscat(ch1,L"  L2--"); _tcscat(ch1,ch);
			maketext(p1,ch1,da,texth ,4,1);   //   不等缓长
			};
		};

		if (fabs(T1-(long int)(T1))<0.001)  { ads_rtos(T1,2,0,ch); }
		else { ads_rtos(T1,2,NLC,ch); };
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		if (fabs(L1-L2)<0.001) 
		{   _tcscpy(ch1,L"T--");  _tcscat(ch1,ch);
		maketext(p1,ch1,da,texth ,4,1);   //   切长 相等
		} else
		{   _tcscpy(ch1,L"T1--");  _tcscat(ch1,ch);
		ads_rtos(T2,2,NLC,ch);
		_tcscat(ch1,L"  T2--"); _tcscat(ch1,ch);
		maketext(p1,ch1,da,texth ,4,1);   //   不等切长
		};

		ads_rtos(LL,2,NLC,ch);
		_tcscpy(ch1,L"L--");  _tcscat(ch1,ch);
		p1[X]=p1[X] -dt*sin(de-c*pi*0.5) ; 
		p1[Y]=p1[Y] -dt*cos(de-c*pi*0.5);
		maketext(p1,ch1,da,texth ,4,1);   //  曲线长
		// 标曲线参数 end
	};

	fb=fe; 

};  
return 1;
}

int GTZX_JYX_ROAD::BZLC1(double array[4*MAXJDNUM][10],int trow,ACHAR *GCH,int NDL,int NLC,double texth,double dlc,double LCB,int zybz,int type)  //  标注里程
{     
	AcGePoint3d  ptb,pt,pte,p1;
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
	// 断链处统一里程
	DDlength=LCB;
	// 逐个断链段标注
	for (iDL=0;iDL<NDL;iDL++)
	{ 
		DL=DLBTMP[iDL].DL;
		LCB= DLBTMP[iDL].BLC+DL;  //本段起点里程 
		_tcscpy(GH,DLBTMP[iDL].EGH);
		_tcscpy(GH1,DLBTMP[iDL].BGH);
		// ads_printf(L"\n iDL=%d %s %10.3f  %s %10.3f",iDL,DLBTMP[iDL].BGH,DLBTMP[iDL].BLC,DLBTMP[iDL].EGH,DLBTMP[iDL].BLC+DL);

		// 绘断链点 或冠号改变点
		if (iDL>0 && iDL< NDL-1)  // 不为起 终点
		{ // 绘出 iDL点
			lc=DDlength;
			PZtmp.lc=lc;  // 统一里程
			k=xlpoint_pz(array,tarray,&PZtmp);  
			if (k>0&& k<=6)  //有解
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
				_tcscpy(ch,LCchr(GH1,DLBTMP[iDL].BLC,NLC,1));  // 断前里程
				_tcscpy(chr,L"  ");_tcscat(chr,ch);   
				maketext( p1,chr,b,texth,4,1);  

				_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(GH,LCB,NLC,1));     // 断后里程
				_tcscpy(chr,L" =");  _tcscat(chr,ch);   // e.f chr="ZH K10+123.45"   

				if (fabs( DL)>0.001)
				{  if ( DL>0.0)   { 
					_tcscat(chr, L" 短链 "); 
					ads_rtos( DL,2,NLC,ch); _tcscat(chr, ch); 
					_tcscat(chr, L" m");  };
				if ( DL<0.0)   { 
					_tcscat(chr, L" 长链 "); 
					ads_rtos(fabs(DL),2,NLC,ch); _tcscat(chr, ch); 
					_tcscat(chr, L" m");  };
				};

				maketext( pte,chr,b,texth,4,1);     

			};

		};

		zlc=(long int)(LCB/dlc);
		BZlc=zlc*dlc;  // 实际里程 
		if ((BZlc-LCB)<0.0001)  { BZlc=BZlc+dlc;  }  
		_tcscpy(GH,DLBTMP[iDL].EGH);  //冠号

		do {
			// 统一里程
			lc=DDlength+BZlc-LCB; 
			PZtmp.lc=lc;  // 统一里程
			k=xlpoint_pz(array,tarray,&PZtmp);   
			BZW=k;    //  K<0  无解
			if (k>0&& k<=6)  //有解
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
				// 里程百米标
				ii=_tcslen(chr);if(ii>20) ii=19;
				if (dlc>=100.0&&ii>=2)
				{  chr[ii-1]='\0';chr[ii-2]='\0';
				_tcscpy(ch,chr);
				if (ii>3&&chr[ii-3]=='0') // 公里标
				{ chr[ii-3]='\0';  _tcscpy(ch,GH); _tcscat(ch,chr); }
				else if(ii>3 && chr[ii-3]!='0')    // 百米标
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
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // 公里标
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

int GTZX_JYX_ROAD::BZTZD1 (double array[4*MAXJDNUM][10],int trow, ACHAR *GCH, int NDL,double texth ,double LCB,int bz_zybz,int Chinese_Engbz_type ,int NLC )  //  标注里程
{     AcGePoint3d  ptb,pt,pte;
double   WB,lc,b,l,INDATA[5],L0;
int i, j,k ,KK=7,zybz=-1;

ACHAR chr[50],ch[40],tch[6], GH[10],DGH[10],lcchr[20] ;
xlpoint  P0tmp,PZtmp;

_tcscpy(GH,DGH);


WB=4*texth;
WB=WB+10*texth;

// ads_initget(1+2+4,NULL);
// ads_getint(L"\n Enter 特征点符号 type=1 (in English) / type=2 (in Chinese)):",&Chinese_Engbz_type);
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

	//  求A=？
	if (j==3)   l=INDATA[1]*INDATA[1]/INDATA[2];
	if (j==4)   l=INDATA[1]*INDATA[1]/INDATA[2];
	if (j==5) l=INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]);
	if (j==6) l=INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]);

	// 求L0=？
	L0=l; if (j==2) L0=INDATA[1];

	ptb[X]=P0tmp.y;ptb[Y]=P0tmp.x; ptb[Z]=0.0;pt[Z]=0.0;pte[Z]=0.0;

	// 标 直线、园曲线、缓和曲线长度
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
			maketext( pt,chr,b,texth,4,1);  // 标 直线、园曲线、缓和曲线长度
	};

	xlzbxy(&P0tmp,&PZtmp,l,INDATA,0); // 某线元终点
	//确定 绘制方向
	zybz=bz_zybz;
	if (bz_zybz==0) 
	{ j=(int)(array[i][0]+0.01);  // 类型 
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

		};
	};
	P0tmp=PZtmp;
};
return 1   ;
}
void GTZX_JYX_ROAD::draw_pm()
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

int GTZX_JYX_ROAD::PM_DMX(double *dmx,double zdmjg,double ZJ,double sml,double eml)
{	
	//double startLC,endLC;
	//startLC=pLCB[0].lc;
	//endLC=pLCB[NLCB-1].lc;
	//ads_printf(L"sml=%lf eml=%lf elc=%lf\n",sml,eml,endLC);
	//xlpoint PZ;
	//double pp[2],tmp;
	//int NUM=0;
	//int i,j,k=0;
	//double lc=startLC;
	//PZ.lc=lc;

	//if(XLShuXing!="贯通设计基线")
	//	xlpoint_pz(array,tarray,&PZ);  
	//else
	//	xlpoint_pz_GTX(array,tarray,&PZ);  

	//pp[0] = PZ.x+ZJ*cos(PZ.a+m_dHalfPI);
	//pp[1] = PZ.y+ZJ*sin(PZ.a+m_dHalfPI);

	//tmp=ptriDTM.interpolate(pp);
	//dmx[NUM*2]=pLCB[0].lc;
	//dmx[NUM*2+1]=tmp;
	//NUM++;

	//ACHAR pszLabel[80];
	//_tcscpy(pszLabel,L"纵断面地面线内插");
	//acedSetStatusBarProgressMeter(pszLabel,0,100);
	//int SUM = NLCB;
	//i = 0; j=1;
	////	while(XYArray[j][6]<=startLC)
	////		j++;
	//while (i<NLCB)//QXB里程为现场里程；
	//{	
	//	int Pos = (int)(NUM*100.0/SUM);
	//	acedSetStatusBarProgressMeterPos(Pos);
	//	/*		if(pLCB[i].lc<XYArray[j][6]+0.0001)
	//	{
	//	lc=pLCB[i].lc;		
	//	i++;
	//	}	
	//	else if(j<=XYNum)
	//	{
	//	lc=XYArray[j][6];
	//	j++;
	//	}
	//	else
	//	break;*/
	//	lc=pLCB[i].lc;
	//	i++;
	//	PZ.lc=lc;

	//	if(XLShuXing!="贯通设计基线")
	//		xlpoint_pz(array,tarray,&PZ);  
	//	else
	//		xlpoint_pz_GTX(array,tarray,&PZ);  

	//	pp[0] = PZ.x +ZJ*cos(PZ.a+m_dHalfPI);
	//	pp[1] = PZ.y +ZJ*sin(PZ.a+m_dHalfPI);

	//	tmp=ptriDTM.interpolate(pp);
	//	dmx[NUM*2]=lc;
	//	dmx[NUM*2+1]=tmp;
	//	//	acutPrintf(L"%lf %lf %lf\n",lc,pLCB[i].lc,tmp);//fabs(dmx[NUM*2]-dmx[(NUM-1)*2]));
	//	if(fabs(dmx[NUM*2]-dmx[NUM*2-2])>0.001)
	//		NUM++;
	//}
	//acedSetStatusBarProgressMeterPos(100);    
	//acedRestoreStatusBar();
	//acutPrintf(L"NUM=%d\n",NUM);
	//return NUM/*NUM*/;

	return 0;
}

int GTZX_JYX_ROAD::PM_DMX(double *dmx)
{

	double sml=DLB[0].BLC+DLB[0].DL;
	double eml=sml + xl_length;

	FormLCB(ZDMJG,sml,eml);
	double startLC,endLC;
	startLC=pLCB[0].lc;
	endLC=pLCB[NLCB-1].lc;
	xlpoint PZ;
	double pp[2],tmp=0.0;
	int NUM=0;
	int i=0,j=0,k=0;
	double TZDcml;
	double lc=startLC;
	PZ.lc=lc;
	if(XLShuXing!=L"贯通设计基线")
		xlpoint_pz(array,tarray,&PZ);  
	else
		xlpoint_pz_GTX(array,tarray,&PZ);  

	pp[0] = PZ.x ;
	pp[1] = PZ.y ;

	tmp=ptriDTM.interpolate(pp);

	dmx[NUM*2]=PZ.lc;//统一里程
	dmx[NUM*2+1]=tmp;
	NUM++;

	ACHAR pszLabel[80];
	_tcscpy(pszLabel,L"纵断面地面线内插");
	acedSetStatusBarProgressMeter(pszLabel,0,100);
	int Pos ;
	int SUM = (int)((endLC-startLC)/ZDMJG+0.1);

	TZDcml = QXB[0].KZH;//统一里程

	while (PZ.lc<endLC)//QXB里程为现场里程；
	{	
		int Pos = (int)(NUM*100.0/SUM);
		acedSetStatusBarProgressMeterPos(Pos);
		//现场里程　
		if(pLCB[i].lc < TZDcml)
		{
			lc=pLCB[i].lc;		
			i++;
		}	
		else if(j<NJD)
		{
			lc=TZDcml;
			k++;
			if(k==1)
				TZDcml = QXB[j].KHY;
			else if(k==2)
				TZDcml = QXB[j].KYH;
			else if(k==3)
				TZDcml = QXB[j].KHZ;

			if(k==4)
			{  
				j++;
				k=0;
				if(j<NJD) TZDcml = QXB[j].KZH;
			}
		}

		PZ.lc=lc;
		if(XLShuXing!=L"贯通设计基线")
			xlpoint_pz(array,tarray,&PZ);  
		else
			xlpoint_pz_GTX(array,tarray,&PZ);  

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
	/*
	acutPrintf(L"NUM=%d",NUM);
	FILE *fp;
	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\桌面\\QXB.txt",L"w+");
	for(i=0;i<NUM;i++)
	{
	fwprintf(fp,L"%d,%lf\n",i,dmx[i*2]);
	}
	fclose(fp);
	*/
	acutPrintf(L"NUM=%d\n",NUM);
	return NUM;
}


void GTZX_JYX_ROAD::outdata(CString str)
{
	FILE *fp;
	/*
	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\桌面\\QXB.txt",L"w+");
	for(int i=0;i<100;i++)
	{
	fwprintf(fp,L"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",QXB[i][0],QXB[i][1],QXB[i][2],QXB[i][3],QXB[i][4],QXB[i][5],QXB[i][6],QXB[i][7],QXB[i][8]);
	}
	fclose(fp);*/

	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\桌面\\"+str+"array.txt",L"w+");
	for(int i=0;i<4*JDarray.GetSize();i++)
	{
		for(int j=0;j<10;j++)
			fwprintf(fp,L"%lf ",array[i][j]);
		fwprintf(fp,L"\n");
		//	fwprintf(fp,L"%lf %lf fwj=%lf R=%lf x=%lf y=%lf\n",array[i][0],array[i][1],array[i][5],array[i][7],array[i][8],array[i][9]);
	}
	fclose(fp);	
	/*
	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\桌面\\array_pxy.txt",L"w+");
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

int GTZX_JYX_ROAD::PM_PQX(double *pqx)
{

	for(int i=0;i<NJD-2;i++)
	{
		//line
		pqx[4*i*6]=QXB[i].KHZ;
		pqx[4*i*6+1]=QXB[i+1].KZH-QXB[i].KHZ;
		pqx[4*i*6+2]=0;//line
		pqx[4*i*6+3]=0;
		pqx[4*i*6+4]=0;
		pqx[4*i*6+5]=0;
		//spire
		pqx[(4*i+1)*6+0]=QXB[i+1].KZH;
		pqx[(4*i+1)*6+1]=QXB[i+1].KHY - QXB[i+1].KZH;
		pqx[(4*i+1)*6+2]=1;//spire
		pqx[(4*i+1)*6+3]=QXB[i+1].LorR;//lift-1 right1
		pqx[(4*i+1)*6+4]=0;
		pqx[(4*i+1)*6+5]=0;
		//circle
		pqx[(4*i+2)*6+0]=QXB[i+1].KZH;
		pqx[(4*i+2)*6+1]=QXB[i+1].KYH - QXB[i+1].KHY;
		pqx[(4*i+2)*6+2]=2;//circle
		pqx[(4*i+2)*6+3]=QXB[i+1].LorR;//lift-1 right1
		pqx[(4*i+2)*6+4]=QXB[i+1].a;
		pqx[(4*i+2)*6+5]=JDarray[i+1].R;
		//spire
		pqx[(4*i+3)*6+0]=QXB[i+1].KYH;
		pqx[(4*i+3)*6+1]=QXB[i+1].KHZ - QXB[i+1].KYH;
		pqx[(4*i+3)*6+2]=-1;//spire
		pqx[(4*i+3)*6+3]=QXB[i+1].LorR;//lift-1 right1
		pqx[(4*i+3)*6+4]=0;
		pqx[(4*i+3)*6+5]=0;
	}
	//line
	pqx[4*(NJD-2)*6+0]=QXB[(NJD-2)].KHZ;
	pqx[4*(NJD-2)*6+1]=QXB[(NJD-2)+1].KZH - QXB[(NJD-2)].KHZ;
	pqx[4*(NJD-2)*6+2]=0;//line
	pqx[4*(NJD-2)*6+3]=0;
	pqx[4*(NJD-2)*6+4]=0;
	pqx[4*(NJD-2)*6+5]=0;
	/*
	FILE *fp;
	fp=_wfopen(L"C:\\Documents and Settings\\All Users\\桌面\\pqx.txt",L"w+");
	for(i=0;i<(4*(NJD-2)+1);i++)
	{	
	fwprintf(fp,L"%lf,%lf,%lf,%lf,%lf,%lf\n",pqx[6*i],pqx[6*i+1],pqx[6*i+2],pqx[6*i+3],pqx[6*i+4],pqx[6*i+5]);
	}
	fclose(fp);*/

	return (4*(NJD-2)+1);
}

void GTZX_JYX_ROAD::VIEW_PM_ZDM(int k)
{/*
 xlpoint PZ;
 ads_point pp;
 int i1;
 ACHAR GH[20];
 double sml,eml;
 ACHAR SKml[80],EKml[80],WinTitle[256];

 // 	if(!ptriDTM.DTMHasRead)
 // 	{
 // //      if (ptriDTM.Read_Dtm())
 // 	  	ads_printf(L"成功读入数模!\n");
 // 	  else
 // 	  {
 // 		 ads_printf(L"导入数模失败!\n");
 //          return ;
 // 	  }
 // 	}

 if(!ViewZdmdlg.IsWindowVisible()) ViewZdmdlg.Create(IDD_VIEWZDM);
 ViewZdmdlg.ShowWindow(SW_SHOW);
 //	acedAlert(L"view_pm_zdm");
 ViewZdmdlg.Hmax = -999999999;   
 ViewZdmdlg.Hmin = 99999999999;   
 PZ.lc = QXB[k-1][0];//zh.lc
 i1 = 0;
 double z;
 while ( PZ.lc  < QXB[k+1][3] )//hz.lc
 {
 xlpoint_pz(array,tarray,&PZ) ;
 pp[0] = PZ.x ;
 pp[1] = PZ.y ;
 z=ptriDTM.interpolate(pp);

 //	if(ViewZdmdlg.EarthPtNew) {delete []ViewZdmdlg.EarthPtNew;ViewZdmdlg.EarthPtNew=NULL;}
 //	ViewZdmdlg.EarthPtNew = new double[(int)((QXB[k+1][3]-QXB[k-1][0])/310.0)];

 ViewZdmdlg.EarthPtNew[i1*2] = PZ.lc;
 ViewZdmdlg.EarthPtNew[i1*2+1] = z; 
 //	acutPrintf(L"i=%d,lc=%lf,z=%lf\n",i1,PZ.lc,z);
 ViewZdmdlg.Hmax =   ViewZdmdlg.Hmax > z ?   ViewZdmdlg.Hmax:z;
 ViewZdmdlg.Hmin =   ViewZdmdlg.Hmin < z ?   ViewZdmdlg.Hmin:z;					  
 i1++;

 PZ.lc +=(QXB[k+1][3]-QXB[k-1][0])/300.0;
 //	ads_printf(L"lc=%lf\n",PZ.lc);
 }
 //	acedAlert(L"VIEW_PM_ZDM");
 ViewZdmdlg.Len=(QXB[k+1][3]-QXB[k-1][0]);
 ViewZdmdlg.NPTNew = i1;
 ViewZdmdlg.InvalidateRect(ViewZdmdlg.m_sht);////ppp	
 sml=QXB[k-1][0] , eml=QXB[k+1][3];
 sml = XLC(sml,GH,NDL-1);
 _tcscpy(SKml,LCchr(GH,sml,2,1));
 eml = XLC(eml,GH,NDL-1);
 _tcscpy(EKml,LCchr(GH,eml,2,1));
 _stprintf(WinTitle,L"%s - %s段纵断面",SKml,EKml); 
 ViewZdmdlg.SetWindowText(WinTitle);
 */
}

void GTZX_JYX_ROAD::VIEW_PM_PQX(int k)
{
	int j=0;
	for(int i=k-1;i<=k+1;i++)
	{
		//line
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
		//circle
		// 		ViewZdmdlg.pPqx[j].slc=QXB[i][1];
		// 		ViewZdmdlg.pPqx[j].l=QXB[i][2]-QXB[i][1];
		// 		ViewZdmdlg.pPqx[j].zhyflag=2;//circle
		// 		ViewZdmdlg.pPqx[j].lrflag=QXB[i][8];//lift-1 right1
		// 		ViewZdmdlg.pPqx[j].a=QXB[i][4];
		// 		ViewZdmdlg.pPqx[j].R=JDarray[i][4];
		// 		ViewZdmdlg.pPqx[j].iJD=i+JD0;
		// 		j++;
		//spire
		// 		ViewZdmdlg.pPqx[j].slc=QXB[i][2];
		// 		ViewZdmdlg.pPqx[j].l=QXB[i][3]-QXB[i][2];
		// 		ViewZdmdlg.pPqx[j].zhyflag=-1;//spire
		// 		ViewZdmdlg.pPqx[j].lrflag=QXB[i][8];//lift-1 right1
		// 		ViewZdmdlg.pPqx[j].a=0;
		// 		ViewZdmdlg.pPqx[j].R=0;
		// 		ViewZdmdlg.pPqx[j].iJD=i+JD0;
		// 		j++;
	}
	//line
	// 	ViewZdmdlg.pPqx[j].slc=QXB[i-1][3];
	// 	ViewZdmdlg.pPqx[j].l=QXB[i][0]-QXB[i-1][3];
	// 	ViewZdmdlg.pPqx[j].zhyflag=0;//line
	// 	ViewZdmdlg.pPqx[j].lrflag=0;
	// 	ViewZdmdlg.pPqx[j].a=0;
	// 	ViewZdmdlg.pPqx[j].R=0;
	// 	ViewZdmdlg.pPqx[j].iJD=i-1+JD0;
}

void GTZX_JYX_ROAD::VIEW_PM_TypeHdm()
{/*
 ACHAR GH[20];
 double cml;
 ACHAR ckml[80];

 if(!ptriDTM.DTMHasRead)
 {
 if (ptriDTM.Read_Dtm())
 ads_printf(L"成功读入数模!\n");
 else
 {
 ads_printf(L"导入数模失败!\n");
 return ;
 }
 }


 cml = PROJ_ML(array,tarray,HdmPt[Y],HdmPt[X],HdmPt[Y],HdmPt[X]);
 if(cml<StartCml || cml>EndCml)
 {
 ads_printf(L"无法找到对应线路里程,请重新指定典型断面位置!\n");
 return;
 }
 double xlc=XLC(cml,GH,NDL);
 _tcscpy(ckml,L"\0");   _tcscpy(ckml,LCchr(GH,xlc,NLC,1));
 GTZDM  *zmLine;
 if(acdbOpenObject(zmLine, ZpId, AcDb::kForRead)!=Acad::eOk)
 {
 ads_printf(L"请指定纵坡方案!\n");
 return;
 }
 zmLine->close();

 if(TypeHdmBar !=NULL)
 {
 TypeHdmBar->m_ckml = ckml;
 TypeHdmBar->m_pm = this;
 TypeHdmBar->m_zm = zmLine;
 TypeHdmBar->Invalidate();
 }
 */
}

//形成间距为dlc的里程表数组
//void  GTZX::FormLCB(double dlc,double sml,double eml)  
////  标注公里百米里程标  
//{    
//	double  BZlc,lc,LCB,DDlength,DL;
//	int BZW,iDL;
//	long int zlc;
//
//	NLCB=0;
//	LCB=DLB[0].BLC+DLB[0].DL;
//	DDlength = LCB;
//
//	if(LCB<sml-0.01)
//	{
//		pLCB[NLCB].lc=sml;
//		pLCB[NLCB].lcflag=-100;//起点
//		NLCB++;		 
//	}
//	else if(LCB<eml+0.001)
//	{
//		pLCB[NLCB].lc=LCB;
//		pLCB[NLCB].lcflag=-100;//起点
//		NLCB++;		 
//	}
//
//	// 逐个断链段标注
//	for (iDL=0;iDL<NDL;iDL++)
//	{ 
//		DL=DLB[iDL].DL;
//		LCB= DLB[iDL].BLC+DL;  //本段起点里程 
//		// 绘断链点 或冠号改变点
//		if (iDL>0 && iDL< NDL-1)  // 不为起 终点
//		{ // 绘出 iDL点
//			lc=DDlength;
//			if(lc>eml+0.001)
//				break;
//			else if(lc>sml-0.001)
//			{
//				pLCB[NLCB].lc=lc;
//				if(fmod(DLB[iDL].BLC,1000)<0.01)//断链在现场里程整公里处
//					pLCB[NLCB].lcflag=-1;
//				else
//				{
//					pLCB[NLCB].lcflag = -1;//李伟2008-01-23
//					//					if(fmod((DLB[iDL].BLC-floor(DLB[iDL].BLC/1000)*1000),100)<0.01) //断链在现场里程整百米处
//					//						pLCB[NLCB].lcflag=floor((DLB[iDL].BLC-floor(DLB[iDL].BLC/1000)*1000)/100+0.01);
//				}
//				NLCB++;		
//			}
//		};	  
//		//zlc=(long int)(LCB/dlc);
//		//BZlc=zlc*dlc;  // 实际里程  为什么要这么两句话？当采用30m间距的时候会出问题。
//		BZlc = LCB;
//		if ((BZlc-LCB)<0.0001)  { BZlc=BZlc+dlc;  }  
//		BZW=99;
//		while(BZlc<DLB[iDL+1].BLC)
//		{
//			// 统一里程
//			lc=DDlength+BZlc-LCB; 
//			if(lc>eml+0.001)
//				break;
//			else if(lc>sml-0.001)
//			{
//				pLCB[NLCB].lc=lc;
//				if(fmod(BZlc,1000)<0.01)//现场里程整公里处
//					//   pLCB[NLCB].lcflag=-1;
//					pLCB[NLCB].lcflag=0;//2003.12.26
//				else
//					//					pLCB[NLCB].lcflag=floor((BZlc-floor(BZlc/1000)*1000)/100+0.01);
//					pLCB[NLCB].lcflag=-99;//李伟2008-01-23
//				NLCB++;		
//			}
//			BZlc=BZlc+dlc;		 
//		} 
//		if(iDL<NDL-1)
//		{
//			DDlength=DDlength+DLB[iDL+1].BLC-LCB;//断链处统一里程。
//			lc=DDlength;
//		}
//
//	} // IDL
//	if(lc>eml)
//	{
//		pLCB[NLCB].lc=eml;			 
//		pLCB[NLCB].lcflag=100;
//		if(pLCB[NLCB].lc-pLCB[NLCB-1].lc > 0.001)
//			NLCB++;		
//	}
//	else if(lc>sml-0.001) // 终点
//	{
//		pLCB[NLCB].lc=lc;			 
//		pLCB[NLCB].lcflag=100;
//		NLCB++;		
//	}
//	//	 ads_printf(L"++++++++++++++ sml=%lf eml=%lf %d\n",sml,eml,NLCB);
//	//统一里程转现场里程
//	ACHAR gh[8],ch[80] ;
//	double xlc;
//	for(int kk=0;kk<NLCB;kk++)
//	{
//		//+++++
//		xlc=XLC(pLCB[kk].lc,gh,NDL);   
//		_tcscpy(ch,LCchr(gh,xlc,2,1));
//	}
//
//}


//形成间距为dlc的里程表数组
void  GTZX_JYX_ROAD::FormLCB(double dlc,double sml,double eml)  
	//  标注公里百米里程标  
{    
	double  BZlc,lc,Tylc,DDlength,DL;
	int BZW,iDL;
	long int zlc;

	NLCB=0;
	Tylc=DLB[0].BLC+DLB[0].DL;
	DDlength = Tylc;
	CArray<LCB,LCB> LCBArray;
	LCB TempLCB;

	if(Tylc<sml-0.01)
	{
		TempLCB.lc = sml;
		TempLCB.lcflag = -100;
		LCBArray.Add(TempLCB); 
	}
	else if(Tylc<eml+0.001)
	{
		TempLCB.lc=Tylc;
		TempLCB.lcflag=-100;//起点
		LCBArray.Add(TempLCB);
	}

	// 逐个断链段标注
	for (iDL=0;iDL<NDL;iDL++)
	{ 
		DL=DLB[iDL].DL;
		Tylc= DLB[iDL].BLC+DL;  //本段起点里程 
		// 绘断链点 或冠号改变点
		if (iDL>0 && iDL< NDL-1)  // 不为起 终点
		{ // 绘出 iDL点
			lc=DDlength;
			if(lc>eml+0.001)
				break;
			else if(lc>sml-0.001)
			{
				TempLCB.lc=lc;
				TempLCB.lcflag = -1;
				LCBArray.Add(TempLCB);
			}
		};	  
		//zlc=(long int)(Tylc/dlc);
		//BZlc=zlc*dlc;  // 实际里程  为什么要这么两句话？当采用30m间距的时候会出问题。
		BZlc = Tylc;
		if ((BZlc-Tylc)<0.0001)  { BZlc=BZlc+dlc;  }  
		BZW=99;
		while(BZlc<DLB[iDL+1].BLC)
		{
			// 统一里程
			lc=DDlength+BZlc-Tylc; 
			if(lc>eml+0.001)
				break;
			else if(lc>sml-0.001)
			{
				TempLCB.lc=lc;
				if(fmod(BZlc,1000)<0.01)//现场里程整公里处
					TempLCB.lcflag=0;//2003.12.26
				else
					//					pLCB[NLCB].lcflag=floor((BZlc-floor(BZlc/1000)*1000)/100+0.01);
					TempLCB.lcflag=-99;//李伟2008-01-23
				LCBArray.Add(TempLCB);		
			}
			BZlc=BZlc+dlc;		 
		} 
		if(iDL<NDL-1)
		{
			DDlength=DDlength+DLB[iDL+1].BLC-Tylc;//断链处统一里程。
			lc=DDlength;
		}

	} // IDL
	if(lc>eml)
	{
		TempLCB.lc=eml;			 
		TempLCB.lcflag=100;
		if(TempLCB.lc-LCBArray[LCBArray.GetSize()-1].lc > 0.001)
			LCBArray.Add(TempLCB);		
	}
	else if(lc>sml-0.001) // 终点
	{
		TempLCB.lc=lc;			 
		TempLCB.lcflag=100;
		LCBArray.Add(TempLCB);		
	}

	if (pLCB) delete []pLCB; pLCB = NULL;
	NLCB = LCBArray.GetSize();
	pLCB = new LCB[NLCB];

	ACHAR GH[8];
	double dml;
	for(int kk=0;kk<NLCB;kk++)
	{
		pLCB[kk] = LCBArray[kk];
		dml = XLC1(pLCB[kk].lc,GH,NDL,DLB);
		_tcscpy(pLCB[kk].ckml,LCchr(GH,dml,3,1));
	}
}


void  GTZX_JYX_ROAD::AddTZDtoLCB() //将曲线特征点加入里程表 
{
	double startLC,endLC;
	LCB *NewLCB = new LCB[NLCB+5*NJD];

	startLC=pLCB[0].lc;
	endLC=pLCB[NLCB-1].lc;
	int NUM=0;
	int i=0,j=0,k=0;


	double lc=startLC;
	double lc1,lc2;
	NUM = 0;
	NewLCB[NUM].lc=pLCB[0].lc;
	NUM++;

	j = 1;
	k = 0;


	bool IsJY = true;
	if (XLShuXing=="既有基线")
		IsJY = true;
	else
		IsJY = false;

	while(j<NJD && QXB[j].KZH < pLCB[0].lc)//起始曲线j,k
	{
		k++;
		if(k==4)
		{
			j++;
			k=0;
		}
	}
	if(j==NJD-1)
		return;

	for(i=1; i<NLCB; i++)//对点数进行循环
	{
		lc1 = pLCB[i].lc;
		if(j<NJD-1)
		{
			if (IsJY)
			{
				if(k==0)lc2 = QXB[j].KZH;
				if(k==1)lc2 = QXB[j].KHY;
				if(k==2)lc2 = QXB[j].KYH;
				if(k==3)lc2 = QXB[j].KHZ;
			}
			else
			{
				//////////////////////////////////////////////////////////////////////////
				//采用断链计算的连续里程，而不是几何连续里程
				if(k==0)lc2 = TYLC1(QXB[j].CKZH,NDL,DLB);
				if(k==1)lc2 = TYLC1(QXB[j].CKHY,NDL,DLB);
				if(k==2)lc2 = TYLC1(QXB[j].CKYH,NDL,DLB);
				if(k==3)lc2 = TYLC1(QXB[j].CKHZ,NDL,DLB);
			}
		}
		else
			lc2 =1.0E12;

		if(lc1 < lc2)
			NewLCB[NUM]=pLCB[i];
		else
		{
			NewLCB[NUM].lc=lc2;
			NewLCB[NUM].lcflag=k+5;

			k++;
			if(k==4)
			{j++;k=0;}
			if(j<NJD)
				i--;
		}
		if(fabs(NewLCB[NUM].lc-NewLCB[NUM-1].lc)>0.001) NUM++;		
	}
	if (pLCB) delete []pLCB; pLCB = NULL;
	NLCB = NUM;
	pLCB = new LCB[NLCB];
	for(i=0; i<NUM; i++)
	{
		pLCB[i]=NewLCB[i];
	}

	if(NewLCB) delete []NewLCB;

}
//  ERRMESDlg *ERRdlg;//违反规定对话框
void GTZX_JYX_ROAD::checkpm()
{
	/*
	double ljia;
	CString ERR;
	int NERR=0;
	FILE *fpw;
	ACHAR fname[256];

	FindZXLenMin();//找出最小夹直线长
	if(!ReadWorkdir())
	{
	ads_alert(L"请先指定正确的工作路径!");
	return ;
	}
	_stprintf(fname,L"%s\\data\\Err",Cworkdir);
	fpw = _wfopen(fname,L"w");
	if(fpw==NULL)
	{
	ads_printf(L"打开文件%s失败!\n",fname);
	return;
	}
	fwprintf(fpw,L"%10d\n",NERR);
	for(int i=2;i<NJD-1;i++)
	{
	ljia = QXB[i][0] - QXB[i-1][3];//夹直线
	if(ljia<ZXLmin1)
	{
	ERR.Format(L"JD%d-JD%d夹直线长%0.2lf小于最小夹直线长(一般%0.2lf,困难%0.2lf))",i-1,i,ljia,ZXLmin1,ZXLmin2);
	fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i][0] + QXB[i-1][3]),ERR);
	NERR++;
	}
	}
	for(i=1;i<NJD-1;i++)
	{
	ljia = QXB[i][2] - QXB[i][1];//夹圆线
	if(ljia<20.0)
	{
	ERR.Format(L"JD%d的圆曲线长%0.2lf小于20m",i,ljia);
	fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i][2] + QXB[i][1]),ERR);
	NERR++;
	}
	}
	for(i=1;i<NJD-1;i++)
	{		
	if(JDarray[i][4]<Rmin1)
	{
	ERR.Format(L"JD%d的曲线半径%0.2lf小于最小曲线半径(一般:%0.0lf,困难:%0.2lf)",i,JDarray[i][4],Rmin1,Rmin2);
	fwprintf(fpw,L"%lf %s\n",0.5*(QXB[i][2] + QXB[i][1]),ERR);
	NERR++;
	}
	}

	rewind(fpw);
	fwprintf(fpw,L"%10d\n",NERR);
	fclose(fpw);
	if(NERR<1)
	{
	ads_printf(L"平面线形通过夹直线，夹圆线及最小曲线半径检测!\n");
	return;
	}
	ERRdlg = new ERRMESDlg(NULL);
	ERRdlg->Create(IDD_DIALOG_ERR);
	ERRdlg->ShowWindow(SW_SHOW);
	*/
}

void GTZX_JYX_ROAD::FindZXLenMin()//找出最小夹直线长
{
	//	if(DV<75)//地方铁路
	//	{
	//		if(TLDJ=="地方I级")
	//		{ ZXLmin1 = 50 ,ZXLmin1 = 25/*,Rmin1=600,Rmin2=350*/;}
	//		else if(TLDJ=="地方II级")
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

	//	ZXLmin1 = mGuiFanCS.MinZhi;
}



///////////////////////////////////plh
void GTZX_JYX_ROAD::ADDJD(double N, double E)
{
	assertWriteEnabled();
	JDarrayStruct TempJDdata;

	NJD++;//增加一个交点
	//	JDarray[NJD-1][0]=NJD-1;
	ACHAR str[20];
	_stprintf(str,L"%d",NJD-1);
	_tcscpy(TempJDdata.JDNum,str);
	TempJDdata.N=N;
	TempJDdata.E=E;
	TempJDdata.l1=0.0;
	TempJDdata.R=0.0;
	TempJDdata.l2=0.0;
	JDarray.Add(TempJDdata);

	NJD = JDarray.GetSize();  //增加一个交点
}

void GTZX_JYX_ROAD::MODIFYJD(int JDNO,double N, double E, double L1, double R, double L2)
	//指定交点序号，修改其参数
{
	assertWriteEnabled();

	JDarray[JDNO].N=N;
	JDarray[JDNO].E=E;

	JDarray[JDNO].l1=L1;
	JDarray[JDNO].R=R;
	JDarray[JDNO].l2=L2;
}

int GTZX_JYX_ROAD::GetPreOrAftJD(double N, double E,bool IsForwardTo)
{//FALSE 向后　　
	double tylc=PROJ_ML(array,tarray,N,E,N,E);
	return DistToNearJD(tylc,IsForwardTo);
}


int GTZX_JYX_ROAD::GetPreJD(double N, double E)
{
	double tylc=PROJ_ML(array,tarray,N,E,N,E);
	return DistToNearJD(tylc,true);
}

int GTZX_JYX_ROAD::DistToNearJD(double tylc, bool IsForwardTo)
	//给定统一里程，是否向前搜索，返回与附近的交点号
{
	int iJD=0;
	for(int i=0;i<NJD;i++)
	{
		if(tylc<=0.5*(QXB[i].KZH + QXB[i].KHZ)+0.001)
		{		
			iJD=i;
			break;
		}
	}
	if(IsForwardTo)//向前搜索
	{
		iJD--;
		if(iJD<0)iJD=0;
	}

	return iJD;	
}

/*===================================================================

函数功能:通过图形上两点的坐标，找出两点间的交点下标数组和交点数组	
------------------------------------------------------------
输入:ads_point p_sptDwg     图形上起点坐标
ads_point p_eptDwg     图形上终点坐标
------------------------------------------------------------
输出:CArray<int,int>& p_arrJDIndex              交点下标数组
CArray<JDarrayStruct,JDarrayStruct>& p_arrJD    交点数组(基线格式)
-----------------------------------------------------
算法描述: 
-----------------------------------------------------
备注:	                                           Zhujiang 2009:3:26
=====================================================================*/
void GTZX_JYX_ROAD::GetJDIndexArrayByPts(ads_point p_sptDwg,ads_point p_eptDwg,
										 CArray<int,int>& p_arrJDIndex,CArray<JDarrayStruct,JDarrayStruct>& p_arrJD)
{
	p_arrJD.RemoveAll();

	int iStartJD,iEndJD;
	double dSml,dEml;


	//既有线上段落的起终点里程和交点序号
	dSml = PROJ_ML(array,tarray,p_sptDwg[Y],p_sptDwg[X],p_sptDwg[Y],p_sptDwg[X]);
	dEml = PROJ_ML(array,tarray,p_eptDwg[Y],p_eptDwg[X],p_eptDwg[Y],p_eptDwg[X]);

	iStartJD = DistToNearJD(dSml,false);
	iEndJD = DistToNearJD(dEml,true);

	for (int i = 0;i < NJD;i++)
	{
		if (i >= iStartJD && i <= iEndJD)
		{
			p_arrJDIndex.Add(i);
			p_arrJD.Add(JDarray[i]);
		}
	}






}




double GTZX_JYX_ROAD::LocateHundredMeterOnLine(double tylc1x,bool IsForwardTo)
	//给定统一里程，是否向前寻找附近的整百米标，要求在直线上,
{
	double gettylc=tylc1x;
	double lc0;//零桩
	int iXY=0;//搜索所处线元
	int XYClass;//线元类型
	ACHAR GH[20];//里程冠号
	double xlc;//一线上投影现场里程
	ACHAR XCLCStr[80];//现场里程字符串
	double OriXlc;//tylc1x对应的现场里程
	xlpoint TmpPZ;//
	//////////////////////////////////////////////////////////////////////////
	//已经在百米标，无需再定位
	if (XLShuXing=="贯通设计基线")
	{
		TmpPZ.lc = tylc1x;
		xlpoint_pz(array,tarray,&TmpPZ);
		_tcscpy(XCLCStr , GetGTXXclc(TmpPZ.x,TmpPZ.y));
		split_ckml(XCLCStr,xlc,GH);
	}
	else
		xlc = XLC1(CalnewcmlTocml(tylc1x),GH,NDL,DLB);
	OriXlc = xlc;
	if (fmod(xlc+0.0001,100.0)<0.001)
		return tylc1x;

	if (DistToNearDL(CalnewcmlTocml(tylc1x),true)<1.0 || DistToNearDL(CalnewcmlTocml(tylc1x),false)<1.0)
		return tylc1x;
	int i;
	for( i=tarray;i>0;i--)
	{
		if(tylc1x>=array[i][6])
		{
			iXY=i;
			break;
		}
	}
	if(i<=0)return -1.0;//plh06.03
	XYClass=array[iXY][0];//取到线元类型
	if(IsForwardTo)//如果向前搜索
	{
		if(XYClass==4)//后缓，向前移动一个线元
		{
			iXY--;
			XYClass=array[iXY][0];//取到线元类型
		}
		if(XYClass==2)//圆，向前移动一个线元
		{
			iXY--;
			XYClass=array[iXY][0];//取到线元类型
		}
		if(XYClass==3)//前缓，向前移动一个线元
		{
			iXY--;
		}
		//此时线元必直线
		gettylc=array[iXY+1][6];//一线直缓点里程
		if(tylc1x<gettylc)gettylc=tylc1x;//首次定位到直线上，

		double K = 1.0 ;
		if (XLShuXing=="贯通设计基线")
		{
			TmpPZ.lc = gettylc;
			xlpoint_pz(array,tarray,&TmpPZ);
			_tcscpy(XCLCStr , GetGTXXclc(TmpPZ.x,TmpPZ.y));
			split_ckml(XCLCStr,xlc,GH);
		}
		else
			xlc=XLC1(CalnewcmlTocml(gettylc,K),GH,NDL,DLB);//ZH处现场里程
		double newxclc = xlc ;
		newxclc = 100*(int)(xlc/100+0.00001); //ZH前一百米标现场里程
		lc0 = xlc - newxclc;//ZH到可能的前一百米标的距离

		if (gettylc-lc0<DLB[0].BLC+DLB[0].DL)//如果定位的百米标在线路起始里程之前，不定位百米标
			return tylc1x;

		double cml = 0.0;
		int NearDLIndex = 0;
		double disttoneardl = 1e6;
		if (XLShuXing=="贯通设计基线")
		{
			ACHAR XLCGTX[80];//贯通线现场里程
			disttoneardl = SearchNearDLOnGTX(gettylc,true,NearDLIndex);
			if (disttoneardl<lc0)//ZH到可能的前一百米标之间有断链，按断链来确定现场里程
			{
				if (fmod(DLB[NearDLIndex].BLC,100.0)<0.01
					||fmod(DLB[NearDLIndex].BLC+DLB[NearDLIndex].DL,100.0)<0.01)//断链是整百米
				{
					cml = PROJ_ML(array,tarray,DLB[NearDLIndex].N,DLB[NearDLIndex].E,DLB[NearDLIndex].N,DLB[NearDLIndex].E);
					return cml;
				}
				else
				{
					_tcscpy(GH,DLB[NearDLIndex].BGH);
					xlc = DLB[NearDLIndex].BLC;
					newxclc = 100*(int)(xlc/100+0.00001);
					_tcscpy(XLCGTX,LCchr(GH,newxclc,3,1));
					if (GetPZFromXCLC1X(XLCGTX,TmpPZ))
					{
						cml = PROJ_ML(array,tarray,TmpPZ.x,TmpPZ.y,TmpPZ.x,TmpPZ.y);
						return cml;
					}
					else
						return gettylc;

				}
			}
			else
			{
				_tcscpy(XLCGTX , LCchr(GH,newxclc,3,1));
				if (GetPZFromXCLC1X(XLCGTX,TmpPZ))
				{
					cml = PROJ_ML(array,tarray,TmpPZ.x,TmpPZ.y,TmpPZ.x,TmpPZ.y);
					return cml;
				}
				else
					return gettylc;
			}
		}
		else
		{
			_tcscpy(XCLCStr , LCchr(GH,newxclc,3,1));
			cml = TYLC1(XCLCStr,NDL,DLB);
			//cml = TYLC1(newxclc,NDL,DLB);
			cml = CalcmlTonewcml(cml);
		}

		lc0 = fabs( cml - gettylc ) ;

		if(lc0<array[iXY+1][6]-array[iXY][6] && DistToNearDL(CalnewcmlTocml(gettylc),true)>lc0+0.001)//调整后仍该线元,且调整前后位置之间没有断链
		{
			return cml ;

		}
		else//取整十米
		{
			//重新计算,

			newxclc = 10*(int)(xlc/10+0.00001); //求ZH前十米标现场里程
			double cml = 0.0;
			if (XLShuXing=="贯通设计基线")
			{
				ACHAR XLCGTX[80];//贯通线现场里程
				_tcscpy(XLCGTX , LCchr(GH,newxclc,3,1));
				GetPZFromXCLC1X(XLCGTX,TmpPZ);
				cml = PROJ_ML(array,tarray,TmpPZ.x,TmpPZ.y,TmpPZ.x,TmpPZ.y);
				//				cml = gettylc + newxclc - OriXlc;
			}
			else
			{
				_tcscpy(XCLCStr , LCchr(GH,newxclc,3,1));
				cml = TYLC1(XCLCStr,NDL,DLB);
				//cml = TYLC1(newxclc,NDL,DLB);
				cml = CalcmlTonewcml(cml);
			}
			return cml ;
			//	lc0=xlc-10*(int)(xlc/10+0.00001);
			//	gettylc = CalnewcmlTocml(gettylc) - lc0*K ;
		}
	}
	else
	{
		if(XYClass==3)//前缓，向后移动一个线元
		{
			iXY++;
			XYClass=array[iXY][0];//取到线元类型
		}
		if(XYClass==2)//圆，向后移动一个线元
		{
			iXY++;
			XYClass=array[iXY][0];//取到线元类型
		}
		if(XYClass==4)//前缓，向后移动一个线元
		{
			iXY++;
		}
		//此时线元必直线
		gettylc=array[iXY][6];//缓直里程
		if(tylc1x>gettylc)gettylc=tylc1x;//首次定位到直线上，

		double K = 1.0 ;
		if (XLShuXing=="贯通设计基线")
		{
			TmpPZ.lc = gettylc;
			xlpoint_pz(array,tarray,&TmpPZ);
			_tcscpy(XCLCStr , GetGTXXclc(TmpPZ.x,TmpPZ.y));
			split_ckml(XCLCStr,xlc,GH);
		}
		else
			xlc=XLC1(CalnewcmlTocml(gettylc,K),GH,NDL,DLB);//缓直的现场里程

		//////////////////////////////////////////////////////////////////////////
		double newxclc = xlc ;
		newxclc = 100*((int)(xlc/100+0.00001)+1); //HZ后一百米标现场里程
		lc0 = newxclc - xlc;

		if (newxclc>DLB[NDL-1].ELC)//如果定位的百米标在线路起始里程之前，不定位百米标
			return tylc1x;

		double cml = 0.0;
		double disttoneardl = 0.0;
		int NearDLIndex = 0;

		if (XLShuXing=="贯通设计基线")
		{
			ACHAR XLCGTX[80];//贯通线现场里程
			disttoneardl = SearchNearDLOnGTX(gettylc,false,NearDLIndex);
			if (disttoneardl<lc0)//ZH到可能的前一百米标之间有断链，按断链来确定现场里程
			{
				if (fmod(DLB[NearDLIndex].BLC,100.0)<0.01
					||fmod(DLB[NearDLIndex].BLC+DLB[NearDLIndex].DL,100.0)<0.01)//断链是整百米
				{
					cml = PROJ_ML(array,tarray,DLB[NearDLIndex].N,DLB[NearDLIndex].E,DLB[NearDLIndex].N,DLB[NearDLIndex].E);
					return cml;
				}
				else
				{
					_tcscpy(GH,DLB[NearDLIndex].EGH);
					xlc = DLB[NearDLIndex].BLC + DLB[NearDLIndex].DL;
					newxclc = 100*((int)(xlc/100+0.00001)+1);
					_tcscpy(XLCGTX,LCchr(GH,newxclc,3,1));
					if (GetPZFromXCLC1X(XLCGTX,TmpPZ))
					{
						cml = PROJ_ML(array,tarray,TmpPZ.x,TmpPZ.y,TmpPZ.x,TmpPZ.y);
						return cml;
					}
					else
						return gettylc;
				}
			}
			else
			{
				_tcscpy(XLCGTX , LCchr(GH,newxclc,3,1));
				if (GetPZFromXCLC1X(XLCGTX,TmpPZ))
				{
					cml = PROJ_ML(array,tarray,TmpPZ.x,TmpPZ.y,TmpPZ.x,TmpPZ.y);
					return cml;
				}
				else
					return gettylc;
			}
		}
		else
		{
			_tcscpy(XCLCStr , LCchr(GH,newxclc,3,1));
			cml = TYLC1(XCLCStr,NDL,DLB);
			//cml = TYLC1(newxclc,NDL,DLB);
			cml = CalcmlTonewcml(cml);
		}

		lc0 = fabs( cml - gettylc ) ;


		if(lc0<array[iXY+1][6]-array[iXY][6] && DistToNearDL(CalnewcmlTocml(gettylc),false)>lc0)//调整后仍该线元,且调整前后位置之间没有断链
		{
			return cml ;

		}
		else//取整十米
		{
			//重新计算,

			newxclc = 10*((int)(xlc/10+0.00001)+1); //求HZ后十米标现场里程
			double cml = 0.0;
			if (XLShuXing=="贯通设计基线")
			{
				ACHAR XLCGTX[80];//贯通线现场里程
				_tcscpy(XLCGTX, LCchr(GH,newxclc,3,1));
				GetPZFromXCLC1X(XLCGTX,TmpPZ);
				cml = PROJ_ML(array,tarray,TmpPZ.x,TmpPZ.y,TmpPZ.x,TmpPZ.y);
				//				cml = gettylc + newxclc - OriXlc;
			}
			else
			{
				_tcscpy(XCLCStr , LCchr(GH,newxclc,3,1));
				cml = TYLC1(XCLCStr,NDL,DLB);
				//cml = TYLC1(newxclc,NDL,DLB);
				cml = CalcmlTonewcml(cml);
			}

			return cml ;
			//	lc0=xlc-10*(int)(xlc/10+0.00001);
			//	gettylc = CalnewcmlTocml(gettylc) - lc0*K ;
		}

	}
	return gettylc;
}

double GTZX_JYX_ROAD::DistToNearDL(double tylc,bool IsForwardTo=true)//
	//给定统一里程，是否向前搜索，返加与附近断链处统一里程的差值，tylc>断链处lc...+
{
	double LCB=0.0;//断链处统一里程
	int iDL=0;
	double iXLlength=0.0;//某段长度
	for(iDL=1;iDL<NDL;iDL++)
	{
		if(tylc<=LCB)break;
		iXLlength=DLB[iDL].BLC-(DLB[iDL-1].BLC+DLB[iDL-1].DL);//某段长度
		LCB+=iXLlength;//断链处统一里程
	}
	if(IsForwardTo)//向前搜索
	{
		LCB-=iXLlength;
	}
	else//向后搜索
	{
	}
	return fabs(tylc-LCB);	
}

//void GTZX_JYX::Invert()//plh07.02
//{
//	assertWriteEnabled();
//	double a[6];
//	int i,k;
//
//
//	for(i=0;i<JDarray.GetSize()/2;i++)
//	{
//		a[1]=JDarray[i].N;
//		JDarray[i].N=JDarray[NJD-1-i].N;
//		JDarray[NJD-1-i].N=a[1];
//
//		a[2]=JDarray[i].E;
//		JDarray[i].E=JDarray[NJD-1-i].E;
//		JDarray[NJD-1-i].N=a[2];
//
//		a[3]=JDarray[i].l1;
//		JDarray[i].l1=JDarray[NJD-1-i].l1;
//		JDarray[NJD-1-i].N=a[3];
//
//		a[4]=JDarray[i].R;
//		JDarray[i].R=JDarray[NJD-1-i].R;
//		JDarray[NJD-1-i].R=a[4];
//
//		a[5]=JDarray[i].l2;
//		JDarray[i].l2=JDarray[NJD-1-i].l2;
//		JDarray[NJD-1-i].l2=a[5];
//
//	}
//	getpxy_DATA();
//}

//plh07.17
int GTZX_JYX_ROAD::GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2)
{
	int i=GetPreJD(N,E);
	N1=JDarray[i].N;
	E1=JDarray[i].E;
	N2=JDarray[i+1].N;
	E2=JDarray[i+1].E;
	return i;
}

void GTZX_JYX_ROAD::CalAJDT1T2(int ijd,double &T1,double &T2)
{
	double fb,fe,db,de,aa,c,lcb,lce,Lo1,Lo2;
	ACHAR zy[2],GH[20];
	int i=0,j=0,k,color=2,iq=2;
	double BPMTL[10],L;
	double LCB,DDlength;
	int iDL=0;

	if(ijd==0 || ijd==NJD-1)
	{
		T1=0.0;
		T2=0.0;
		return;
	}

	i=ijd;
	db=xyddaa(JDarray[i-1].N,JDarray[i-1].E,JDarray[i].N,JDarray[i].E,&fb);
	de=xyddaa(JDarray[i].N,JDarray[i].E,JDarray[i+1].N,JDarray[i+1].E,&fe);
	aa=fe-fb;
	//?? BAS_Fun
	if (aa<-pi) aa=aa+2*pi;
	if  (aa>pi) aa=aa-2*pi;
	aa=fabs(aa);
	aa=dms_rad(aa);
	//计算切线长
	Lo1 = JDarray[i].l1;
	Lo2 = JDarray[i].l2;
	getBPMTL(aa,JDarray[i].R,JDarray[i].l1,JDarray[i].l2,&BPMTL[0]); 
	T1=BPMTL[6];
	T2=BPMTL[7];

	//////////////
	bool PreLinK,AftLink;
	PreLinK=FALSE,AftLink=FALSE;
	if(_tcscmp(JDarray[i].JDNum,JDarray[i-1].JDNum)==0)
		PreLinK=TRUE;
	if(_tcscmp(JDarray[i].JDNum , JDarray[i+1].JDNum)==0)
		AftLink=TRUE;
	double RB,RE,R;
	RB=1e50,RE=1e50,R=0.0;

	if(PreLinK ||AftLink)
	{
		if( PreLinK && JDarray[i].l1>1e-6 )
			RB = JDarray[i-1].R;
		else
			RB = 1e50;

		if(AftLink && JDarray[i].l2>1e-6)
			RE = JDarray[i+1].R;
		else
			RE = 1e50;
		R= JDarray[i].R;
		get_T_Len(aa,R,RB,Lo1,RE,Lo2,T1,T2,L);
	};	 

}

//设置标志里程
/*
Adesk::Boolean  GTZX::setBZLC(int iBZ,double BLC,double DL, ACHAR *GH, int mode )
{  
/*
if (mode !=1 )
assertWriteEnabled();
if (iDL>=0&& iDL<NDL)
{ 	
_tcscpy(DLB[iDL].BGH,BGH);	
_tcscpy(DLB[iDL].EGH,EGH);
DLB[iDL].BLC=BLC;        
DLB[iDL].DL= DL;
//	BZLCarray[].
//起点
if (iDL==0) JDarray[0][3]=BLC+DL;
return 1;
} else return 0;
*/
/*
if (mode !=1 )
assertWriteEnabled();
if (iBZ>=0&& iBZ<NBZLC)
{ 	
_tcscpy(BZLCarray[iBZ].GH,GH);	
//????/
//	BZLCarray[iBZ].kml = XLC()
//    DLB[iBZ].BLC=BLC;        
//	DLB[iBZ].DL= DL;

//起点
//	if (iBZ==0) JDarray[0][3]=BLC+DL;
//   return 1;
} else return 0;

}
*/


void GTZX_JYX_ROAD::DrawBandEPoints(AcGiWorldDraw* pWd )
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
		//起点
		startPt.x = GJDInforArray[i].spt[1]; //屏幕
		startPt.y = GJDInforArray[i].spt[0];
		startPt.z = 0.0;

		//计算起点的法向
		PZtmp.lc = PROJ_ML(array,tarray,startPt.y,startPt.x,startPt.y,startPt.x);

		BAS_DRAW_FUN_JYX::xlpoint_pz(array,tarray,&PZtmp); 

		double angle;
		angle = PZtmp.a+pi*0.5*zybz;
		if(angle<0.0) angle = angle + 2*pi;
		if(angle>2*pi) angle = angle - 2*pi;


		//SKML = GJDInforArray[i].GJDBLC ;
		//ACHAR ckml[80];
		//double dml;
		//ACHAR GH[10];
		//_tcscpy(ckml,SKML);
		//split_ckml(ckml,dml,GH);
		////注字
		//_stprintf(textACHAR , L"%.1lf%s起点",dml/1000.0,GJDInforArray[i].gjdname );

		SKML = GJDInforArray[i].GJDBLC;
		_stprintf(textACHAR , L"%s起点%s",GJDInforArray[i].gjdname,SKML );

		BZLineLength = 5*_tcslen(textACHAR);
		endPt.x = PZtmp.y +(BZLineLength)*sin(angle);
		endPt.y = PZtmp.x +(BZLineLength)*cos(angle);
		endPt.z = 0.0; 

		double WB=1.5*texth;
		p1.x=PZtmp.y+3.5*texth*sin(PZtmp.a)+(WB+3*texth)*sin(PZtmp.a+pi*0.5*zybz) ;
		p1.y=PZtmp.x+3.5*texth*cos(PZtmp.a)+(WB+3*texth)*cos(PZtmp.a+pi*0.5*zybz) ; 
		G_makelineW(pWd , startPt ,endPt ,QXYSColor, LineWeightValue);

		double b=PZtmp.a +pi*0.5;
		b=pi*0.5-b; if (zybz==-1) b=b+pi;
		if (b<0.0) b=2*pi+b;		  if (b>2*pi) b=b-2*pi;		         
		//f(type<=1)  b=b-pi*0.5; 

		G_maketext(pWd,p1,textACHAR,b,texth,QXYSColor,1);   


		//中间直线
		MidPt.x = ( startPt.x + endPt.x )/2.0;
		MidPt.y = ( startPt.y + endPt.y )/2.0;
		MidPt.z = 0.0 ;

		endPt.x = MidPt.x +(5)*sin(PZtmp.a);
		endPt.y = MidPt.y +(5)*cos(PZtmp.a);
		endPt.z = 0.0; 

		MidPointArray.Add(MidPt);
		//	G_makelineW(pWd , MidPt ,endPt ,1, LineWeightValue);


		//终点	
		startPt.x = GJDInforArray[i].ept[1]; //屏幕
		startPt.y = GJDInforArray[i].ept[0];
		startPt.z = 0.0;

		//	EKML = GetGTXXclc(GJDInforArray[i].ept[0],GJDInforArray[i].ept[1]);
		//	_tcscpy( GJDInforArray[i].GJDELC , EKML );
		EKML = GJDInforArray[i].GJDELC ;

		//计算起点的法向
		PZtmp.lc = PROJ_ML(array,tarray,startPt.y,startPt.x,startPt.y,startPt.x);

		BAS_DRAW_FUN_JYX::xlpoint_pz(array,tarray,&PZtmp); 


		angle = PZtmp.a+pi*0.5*zybz;
		if(angle<0.0) angle = angle + 2*pi;
		if(angle>2*pi) angle = angle - 2*pi;


		_stprintf(textACHAR , L"%s终点%s",GJDInforArray[i].gjdname,EKML);


		BZLineLength = 5*_tcslen(textACHAR);
		endPt.x = PZtmp.y +(BZLineLength)*sin(angle);
		endPt.y = PZtmp.x +(BZLineLength)*cos(angle);
		endPt.z = 0.0; 
		G_makelineW(pWd , startPt ,endPt ,QXYSColor, LineWeightValue);


		b=PZtmp.a +pi*0.5;
		b=pi*0.5-b; if (zybz==-1) b=b+pi;
		if (b<0.0) b=2*pi+b;		  if (b>2*pi) b=b-2*pi;		         
		//f(type<=1)  b=b-pi*0.5; 


		p1.x=PZtmp.y+2.5*texth*sin(PZtmp.a+pi)+(WB+3*texth)*sin(PZtmp.a+pi*0.5*zybz) ;
		p1.y=PZtmp.x+2.5*texth*cos(PZtmp.a+pi)+(WB+3*texth)*cos(PZtmp.a+pi*0.5*zybz) ; 

		G_maketext(pWd,p1,textACHAR,b , texth, QXYSColor,1);   


		//中间直线
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

Adesk::Boolean  GTZX_JYX_ROAD::BZLC2(AcGiWorldDraw* pWd,double dlc,int zybz,int type)  
	//  
	//    zybz=1,-1  标在右边，左边
{//标志里程
	zybz = -1;
	AcGePoint3d Pt;

	AcGePoint3d  pte,ptb;
	int k;
	double B;
	xlpoint  PZtmp; 
	ACHAR str[256];
	double  WB;
	//标志里程
	WB=1.5*texth;
	//1  ~ -1  有问题

	for(int i=1;i<BZLCarray.GetSize()-1;i++)  //NBZLC
	{
		PZtmp.lc=BZLCarray[i].xyTocml;  // 统一里程	

		//ads_printf(L"%s\n",BZLCarray[i].kml);


		k=xlpoint_pz(array,tarray,&PZtmp);   

		B=PZtmp.a;
		//点起点
		Pt[Y]=PZtmp.x; 
		Pt[X]=PZtmp.y;
		Pt[Z]=0.0;

		//字起点
		ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
		ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);
		double  b=B+pi*0.5;
		b=pi*0.5-b; if (zybz==-1) b=b+pi;
		if (b<0.0) b=2*pi+b;		 
		if (b>2*pi) b=b-2*pi;		         
		if(type<=1)  b=b-pi*0.5; 
		_tcscpy(str,BZLCarray[i].kml);
		_tcscat(str,L" 标志里程");
		AcGePoint3d BZPt;
		//BZPt.y=ptb[Y]-(0.25*texth)*cos(B);
		//BZPt.x=ptb[X]-(0.25*texth)*sin(B);
		BZPt.y=ptb[Y]+(1.8*texth)*cos(B);
		BZPt.x=ptb[X]+(1.8*texth)*sin(B);
		G_maketext(pWd,BZPt,str,b,1.5*texth,QXYSColor,1);
		//点终点
		pte[X]=PZtmp.y+(_tcslen(str)*1.2*texth)*sin(B+pi*0.5*zybz);
		pte[Y]=PZtmp.x+(_tcslen(str)*1.2*texth)*cos(B+pi*0.5*zybz);
		G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
	}

	return TRUE;
}

double  GTZX_JYX_ROAD::CalcmlTonewcml(double kmlTocml)
{
	double newcml = 0.0 ;
	newcml=kmlTocml;
	double a=0.0,b=0.0;
	for(int i=1;i<BZLCarray.GetSize();i++)
	{
		if(kmlTocml>BZLCarray[i-1].kmlTocml-0.00001 && kmlTocml<BZLCarray[i].kmlTocml+0.00001)
		{
			newcml = BZLCarray[i-1].xyTocml + BZLCarray[i].K * (kmlTocml - BZLCarray[i-1].kmlTocml);
			return newcml;
		}
	}

	return newcml;
}

double  GTZX_JYX_ROAD::CalnewcmlTocml(double newcml)
{
	double cml = newcml;
	//	newcml=kmlTocml;
	double a=0.0,b=0.0;

	if (BZLCarray.GetSize()<1)
		return newcml;
	for(int i=1;i<BZLCarray.GetSize();i++)
	{
		if(newcml>BZLCarray[i-1].xyTocml-0.00001 && newcml<BZLCarray[i].xyTocml+0.00001)
		{
			//	newcml = BZLCarray[i-1].xyTocml + BZLCarray[i].K * (kmlTocml - BZLCarray[i-1].kmlTocml);

			if(fabs(BZLCarray[i].K)<1.0e-6)
			{
				return BZLCarray[i-1].kmlTocml;
			}
			else 
			{
				cml =( newcml - BZLCarray[i-1].xyTocml )/( BZLCarray[i].K ) +  BZLCarray[i-1].kmlTocml ;
				return cml;
			}
		}	
	}

	return cml;
}

double  GTZX_JYX_ROAD::CalnewcmlTocml(double newcml , double & K )
{
	double cml = newcml;
	//	newcml=kmlTocml;
	double a=0.0,b=0.0;

	if (BZLCarray.GetSize()<1)
	{
		K = 1;
		return newcml;
	}
	for(int i=1;i<BZLCarray.GetSize();i++)
	{
		if(newcml>BZLCarray[i-1].xyTocml-0.00001 && newcml<BZLCarray[i].xyTocml+0.00001)
		{
			//	newcml = BZLCarray[i-1].xyTocml + BZLCarray[i].K * (kmlTocml - BZLCarray[i-1].kmlTocml);

			if(fabs(BZLCarray[i].K)<1.0e-6)
			{
				K = 1 ;
				return BZLCarray[i-1].kmlTocml;
			}
			else 
			{
				cml =( newcml - BZLCarray[i-1].xyTocml )/( BZLCarray[i].K ) +  BZLCarray[i-1].kmlTocml ;
				K = BZLCarray[i].K ;
				return cml;
			}
		}	
	}

	K = 1 ;
	return cml;
}

//  标里程
Adesk::Boolean GTZX_JYX_ROAD::BZTZD2(AcGiWorldDraw* pWd,double dlc,int zybz,int type)  //  标注里程
	// bz_zybz==0   按曲线偏向标注   bz_zybz=1,-1  标在右边，左边
{    
	//  bz_zybz=1  标在右边
	//	zybz = -1;  // 左边
	AcGePoint3d Pt;
	AcGePoint3d  pte,ptb;
	int k;
	double B;
	xlpoint  PZtmp; 
	ACHAR str[80],str2[80];
	double  WB;
	//标志里程
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

				if(XLShuXing!="贯通设计基线")
				{
					double tmpLC = CalnewcmlTocml( cml );

					xlc = XLC(tmpLC,GH,NDL);
					_tcscpy(kml,L"\0");   
					_tcscpy(kml,LCchr(GH,xlc,NLC,1));
				}
				else
				{
					//	lc = CalnewcmlTocml(cml);
					PZtmp.lc = cml ;
					k=xlpoint_pz(array,tarray,&PZtmp); 
					CString ckml;
					ckml = GetGTXXclc(PZtmp.x,PZtmp.y);
					_tcscpy(kml,ckml);
				}

				//	lc= CalcmlTonewcml(cml);

				//				lc= CalnewcmlTocml(cml);
				PZtmp.lc = cml ;

				k=xlpoint_pz(array,tarray,&PZtmp); 


				B=PZtmp.a;
				//点起点
				Pt[Y]=PZtmp.x; 
				Pt[X]=PZtmp.y;
				Pt[Z]=0.0;

				//字起点
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

				//点终点
				pte[X]=PZtmp.y+(_tcslen(str2)*1.2*texth)*sin(B+pi*0.5*zybz);
				pte[Y]=PZtmp.x+(_tcslen(str2)*1.2*texth)*cos(B+pi*0.5*zybz);

				G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
				if(zybz>0)
					G_maketext(pWd,pte,str2,b,1.5*texth,QXYSColor,1);
				else 	G_maketext(pWd,ptb,str2,b,1.5*texth,QXYSColor,1);

			}

		}
		else 
		{
			if(i==0)
			{
				cml = DLB[0].BLC +  DLB[0].DL ;
				_tcscpy(kml,LCchr(DLB[0].EGH,cml,NLC,1));
			}
			else
			{
				cml = DLB[0].BLC +  DLB[0].DL + xl_length;
				_tcscpy(kml,LCchr(DLB[NDL-1].BGH,DLB[NDL-1].BLC,NLC,1));
			}


			//	lc= CalcmlTonewcml(cml);
			lc= cml;
			PZtmp.lc = lc;
			k=xlpoint_pz(array,tarray,&PZtmp); 


			B=PZtmp.a;
			//点起点
			Pt[Y]=PZtmp.x; 
			Pt[X]=PZtmp.y;
			Pt[Z]=0.0;

			//字起点
			ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
			ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);

			ptb[Y] = ptb[Y] - 0.25*texth*cos(B);
			ptb[X] = ptb[X] - 0.25*texth*sin(B);

			if(i==0)_stprintf(str2,L"QD %s",kml);
			else _stprintf(str2,L"ZD %s",kml);
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

			//点终点
			pte[X]=PZtmp.y+(_tcslen(str2)*1.2*texth)*sin(B+pi*0.5*zybz);
			pte[Y]=PZtmp.x+(_tcslen(str2)*1.2*texth)*cos(B+pi*0.5*zybz);

			G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
			if(zybz>0)
				G_maketext(pWd,pte,str2,b,1.5*texth,QXYSColor,1);
			else 	G_maketext(pWd,ptb,str2,b,1.5*texth,QXYSColor,1);

		}
	}


	return TRUE;
}


void GTZX_JYX_ROAD::DrawBZTZD(AcGiWorldDraw* pWd,int zybz,ACHAR str[])
{
	double B=0.0;
	AcGePoint3d  ptb,pt;
	AcGePoint3d Pt,pte;
	double WB;

	WB=4*texth;
	WB=WB+10*texth;
	B=PZtmp.a;
	//点起点
	Pt[Y]=PZtmp.x; 
	Pt[X]=PZtmp.y;
	Pt[Z]=0.0;


	//字起点
	ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
	ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);
	double  b=B+pi*0.5;
	b=pi*0.5-b; if (zybz==-1) b=b+pi;
	if (b<0.0) b=2*pi+b;		 
	if (b>2*pi) b=b-2*pi;		         
	//	if(type<=1)  b=b-pi*0.5; 

	//	_tcscpy(str,BZLCarray[i].kml);
	//	_tcscat(str,L" 标志里程");

	AcGePoint3d BZPt;
	BZPt.y=ptb[Y]-(0.25*texth)*cos(B);
	BZPt.x=ptb[X]-(0.25*texth)*sin(B);
	//		G_maketext(pWd,BZPt,str,b,1.5*texth,3,1);
	//点终点
	pte[X]=PZtmp.y+(_tcslen(str)*1.5*texth)*sin(B+pi*0.5*zybz);
	pte[Y]=PZtmp.x+(_tcslen(str)*1.5*texth)*cos(B+pi*0.5*zybz);
	G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);
}

double GTZX_JYX_ROAD::TYLC(double dml)
{   
	double TYLCB=0,LCB, LCE,TLC,XLC;  // LCB 断链的统一里程
	int iDL=0,bz;
	//起点里程
	CString GH;
	double DLTYLC[30];

	DLTYLC[0] = DLB[0].BLC + DLB[0].DL;//断链的统一里程
	for (iDL = 1;iDL < NDL; iDL++)//检查是否在长链内
	{
		DLTYLC[iDL] = DLTYLC[iDL-1]  + DLB[iDL].BLC - DLB[iDL-1].DL - DLB[iDL-1].BLC;
	}

	TLC=fabs(dml);
	XLC = TLC;
	bz=-99;
	for (iDL=0;iDL<NDL;iDL++)   // 
	{
		//该断链点统一里程
		TYLCB=DLTYLC[iDL];

		//该断链后链里程
		LCB=DLB[iDL].BLC+DLB[iDL].DL ;	
		//下一断链的前链里程
		if(iDL<NDL-1)LCE=DLB[iDL+1].BLC; 
		if( ( iDL<NDL-1 && XLC>=LCB && XLC<=LCE ) || (iDL==NDL-1 && XLC>=LCB) )  // 落在该段落内
		{   
			if( bz<0) {  TLC= TYLCB+XLC-LCB;  bz=1; }  // 第一次求到
			else 
			{ 
				bz=2;
				//				ads_printf(L"\n 输入的里程有两个里程点：1:=%12.2f 2:=%12.2f  ",
				//					TLC,TYLCB+XLC-LCB);  
				if(dml<0.0)TLC=TYLCB+XLC-LCB;
			}
		}
	}
	return TLC;     
}

//设置标志里程数组
void GTZX_JYX_ROAD::SetBZLCVal()
{   
	//修正起终点处的标志里程
	if(NBZLC<1) return;
	if(NJD<1) return;

	NBZLC = BZLCarray.GetSize();

	//起点
	ACHAR Ckml[20];
	_tcscpy(Ckml, LCchr(DLB[0].EGH,DLB[0].BLC+DLB[0].DL,NLC,1));
	_tcscpy(BZLCarray[0].kml, Ckml);
	BZLCarray[0].X=JDarray[0].N;
	BZLCarray[0].Y=JDarray[0].E;
	BZLCarray[0].kmlTocml = DLB[0].BLC+DLB[0].DL;
	BZLCarray[0].xyTocml = DLB[0].BLC+DLB[0].DL;
	//终点
	_tcscpy(Ckml, LCchr(DLB[NDL-1].BGH,DLB[NDL-1].BLC,NLC,1));
	_tcscpy(BZLCarray[NBZLC-1].kml,Ckml);
	BZLCarray[NBZLC-1].X=JDarray[NJD-1].N;
	BZLCarray[NBZLC-1].Y=JDarray[NJD-1].E;
	_tcscpy(Ckml, BZLCarray[NBZLC-1].kml);
	TrsCkmltoCml(Ckml, BZLCarray[NBZLC-1].kmlTocml);
	BZLCarray[NBZLC-1].xyTocml = DLB[0].BLC+DLB[0].DL + xl_length;


	//逐个计算标志里程中的数据给标志里程数组中的所有成员重新付值
	int i;
	for( i=1;i<NBZLC-1;i++)
	{	
		BZLCarray[i].kmlTocml = TYLC1(BZLCarray[i].kml,NDL,DLB);
		BZLCarray[i].xyTocml  = PROJ_ML(array,tarray,BZLCarray[i].X,BZLCarray[i].Y,BZLCarray[i].X,BZLCarray[i].Y);
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


//添加标志里程
void GTZX_JYX_ROAD::addBZLCToGTZX(ads_point PT ,ACHAR CurAddBZLC[30])
{
	NBZLC = BZLCarray.GetSize();


	for(int i=0 ; i<BZLCarray.GetSize() ; i++)
	{
		_tcscpy(BZLCarray[i].GJDNum , XLShuXing );
	}

	double TYLC,NewTYLC;
	int AddBZLCNum = 0;
	BZLCStructTable BZLCTemp;
	_tcscpy(BZLCTemp.GJDNum , XLShuXing );
	_tcscpy(BZLCTemp.kml , CurAddBZLC);
	BZLCTemp.X = PT[Y];
	BZLCTemp.Y = PT[X];
	//1.由拾取点坐标  计算  cml
	TYLC = PROJ_ML(array,tarray,PT[Y],PT[X],PT[Y],PT[X]);

	//  ads_printf(L"TYLC=%lf,PT[Y]=%lf,PT[X]=%lf,BZLCarray[0].xyTocml=%lf\n",TYLC,PT[Y],PT[X],BZLCarray[0].xyTocml);
	//获得满足条件的下标
	assertWriteEnabled();

	for(int i=1;i<NBZLC;i++)
	{

		if(TYLC < BZLCarray[0].xyTocml)
		{
			//	ads_printf(L"头\n");
			AddBZLCNum = 0;
			break;
		}
		if(TYLC > BZLCarray[i-1].xyTocml - 0.00001 && TYLC < BZLCarray[i].xyTocml + 0.00001)
		{
			//	ads_printf(L"[i-1].xyTocml=%lf,[i].xyTocml=%lf\n",BZLCarray[i-1].xyTocml,BZLCarray[i].xyTocml);
			//   ads_printf(L"中\n");
			AddBZLCNum = i;
			break;
		}   
		if(TYLC > BZLCarray[NBZLC-1].xyTocml)
		{
			//  ads_printf(L"尾\n,BZLCarray[NBZLC-1].xyTocml =%lf\n",BZLCarray[NBZLC-1].xyTocml);
			AddBZLCNum = NBZLC;
			break;
		}
	}

	//	ads_printf(L"AddBZLCNum=%d,NBZLC=%d\n",AddBZLCNum,NBZLC);

	////重新组合 BZLCarray 数组
	BZLCarray.InsertAt(AddBZLCNum,BZLCTemp,1);

	NBZLC = BZLCarray.GetSize();

}

//  删除一个标志里程
void GTZX_JYX_ROAD::DeleteBZLCToGTZX(ads_point PT)
{   
	double TYLC,NewTYLC;
	int DelBZLCNum = 0;
	BZLCStructTable BZLCTemp;
	BZLCTemp.X = PT[X];
	BZLCTemp.Y = PT[Y];
	//1.由拾取点坐标  计算  cml
	TYLC = PROJ_ML(array,tarray,PT[Y],PT[X],PT[Y],PT[X]);

	double MinL = 99999999999;
	double deltaL = 0.0 ;
	for(int i=1; i< BZLCarray.GetSize() -1; i++)
	{
		deltaL = fabs( TYLC - BZLCarray[i].xyTocml );
		//	double deltaL = fabs(Newdl_TYLC - NewTYLC);
		if(MinL <  deltaL)
			MinL =  MinL;
		else
		{
			MinL = deltaL; 
			DelBZLCNum = i;
		}
	}

	//删除指定数据,重新组合 BZLCarray 数组
	if(DelBZLCNum>0)
		BZLCarray.RemoveAt(DelBZLCNum,1);
}


void GTZX_JYX_ROAD::SortBZLCArray()
{
	//不考虑标志里程影响
	//	GTZX *pzLinepTmp= new GTZX(JDarray.GetSize(),JDarray,L"k",L"JD" );

	CArray<BAS_DRAW_FUN_JYX::BZLCStructTable , BAS_DRAW_FUN_JYX::BZLCStructTable> SortBZLCArrayTmp;
	double cmlTmp = 0.0 , cmlTmp2 = 0.0 , cmlTmp3 = 0.0 ;

	for (int i=0 ; i<BZLCarray.GetSize() ; i++)
	{
		BZLCarray[i].xyTocml = PROJ_ML(array,tarray,BZLCarray[i].X,BZLCarray[i].Y,BZLCarray[i].X,BZLCarray[i].Y);
	}

	BAS_DRAW_FUN_JYX::BZLCStructTable BZLCTemp ;

	for (int i=0 ; i<BZLCarray.GetSize()-1 ; i++)
	{
		for (int j=i+1; j<BZLCarray.GetSize() ; j++)
		{
			cmlTmp = BZLCarray[i].xyTocml;

			cmlTmp2 = BZLCarray[j].xyTocml;

			if ( cmlTmp > cmlTmp2 )
			{

				BZLCTemp = BZLCarray[i];
				BZLCarray[i] = BZLCarray[j];
				BZLCarray[j] = BZLCTemp ;
			}
		}
	}


}

void GTZX_JYX_ROAD::OnReCalBZLC(CArray<BZLCStructTable,BZLCStructTable> & m_BZLC) 
{

	//往实体内 赋"起点" 数据
	m_BZLC[0].xyTocml = DLB[0].BLC + DLB[0].DL;
	m_BZLC[0].kmlTocml = DLB[0].BLC + DLB[0].DL;

	int i;
	for(i=1;i<m_BZLC.GetSize()-1;i++)
	{
		//由坐标x,y -> 的连续里程
		m_BZLC[i].xyTocml = PROJ_ML(array,tarray,m_BZLC[i].X,m_BZLC[i].Y,m_BZLC[i].X,m_BZLC[i].Y);
		//现场里程 -> 的连续里程
		m_BZLC[i].kmlTocml = TYLC1(m_BZLC[i].kml,NDL,DLB);

	}
	m_BZLC[m_BZLC.GetSize()-1].xyTocml = DLB[0].BLC + DLB[0].DL + xl_length;
	m_BZLC[m_BZLC.GetSize()-1].kmlTocml = DLB[0].BLC + DLB[0].DL + xl_length;

	//3. K
	double a=0.0,b=0.0;
	m_BZLC[0].K = 0.0; 
	for(i=1;i<m_BZLC.GetSize();i++)
	{
		a = m_BZLC[i].kmlTocml - m_BZLC[i-1].kmlTocml;
		b = m_BZLC[i].xyTocml - m_BZLC[i-1].xyTocml;
		if(fabs(a)>1.0e-6)
			m_BZLC[i].K = b/a;
		else m_BZLC[i].K = 0.0;

	}		

}


void GTZX_JYX_ROAD::CalT1T2L()
{


	double T1,T2,L;
	double RB,RE,R,L1,L2,ZJ;

	for(int i=1;i<QXB.GetSize()-1;i++)
	{
		//不相等
		if((_tcscmp(QXB[i-1].JDNum,QXB[i].JDNum)!=0) && (_tcscmp(QXB[i].JDNum,QXB[i+1].JDNum)!=0))	
		{//单交点
			RB = 1e50;
			RE = 1e50;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//相等
		if((_tcscmp(QXB[i-1].JDNum,QXB[i].JDNum)==0) && (_tcscmp(QXB[i].JDNum,QXB[i+1].JDNum)==0))	
		{
			//复曲线
			RB = QXB[i-1].R;
			RE = QXB[i+1].R;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//一测相等
		if((_tcscmp(QXB[i-1].JDNum,QXB[i].JDNum)==0) && (_tcscmp(QXB[i].JDNum,QXB[i+1].JDNum)!=0))	
		{
			//复曲线
			RB = QXB[i-1].R;
			RE = 1e50;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}

		//一测相等
		if((_tcscmp(QXB[i-1].JDNum,QXB[i].JDNum)!=0) && (_tcscmp(QXB[i].JDNum,QXB[i+1].JDNum)==0))	
		{
			//复曲线
			RB = 1e50;
			RE = QXB[i+1].R;
			ZJ = QXB[i].a;
			R = QXB[i].R;
			L1 = QXB[i].l1;
			L2 = QXB[i].l2;
		}
		get_T_Len(ZJ,R,RB,L1,RE,L2,T1,T2,L);
		//	ads_printf(L"****  JDNum=%s,T1=%lf,T2=%lf,L=%lf\n",QXB[i].JDNum,T1,T2,L);
		QXB[i].T1 = T1;
		QXB[i].T2 = T2;
		QXB[i].L  = L;
	}
}

/*
//合并成新的曲线表
void GTZX::MergeQXBandJYQXBToQXB()
{
//临时存储交点下标数组
CArray<CString,CString>TempJDNumArray;
//曲线表中第一个下标，所有数数目
int QXBfirstNum,QXBAllNum;
//既有曲线表中第一个下标，所有数数目
int JYQXBfirstNum,JYQXBAllNum;

int JDAllNum;
////把平面计算的QXB + JYQXB 组合成  QXB
for(int i=0;i<NJD;i++)
{	
if(QXB[i].isGJ==TRUE)
continue;
//得到指定交点号在曲线表中的　第一个下标，所有满足条件的总数
GetArrayCS(QXB,QXB[i].JDNum,QXBfirstNum,QXBAllNum);
//得到指定交点号在既有曲线表中的　第一个下标，所有满足条件的总数
GetArrayCS(JYQXB,QXB[i].JDNum,JYQXBfirstNum,JYQXBAllNum);

if(JYQXBfirstNum<0)continue;
//求每个交点,在数组中相同个数
int k=0;

ACHAR GHACHAR[20],LCACHAR[80];//冠号
CString LCstring;//里程串
if(JYQXBAllNum==QXBAllNum)
{
for(int g=0;g<QXBAllNum;g++)
{//JYQXBfirstNum


_tcscpy(GHACHAR , JYQXB[JYQXBfirstNum+g].GH );
if(_tcscmp(GHACHAR,L"")==0 || _tcscmp(GHACHAR,L"NULL") ==0 )
{

QXB[QXBfirstNum+g].KZH = TYLC(JYQXB[JYQXBfirstNum+g].KZH);
QXB[QXBfirstNum+g].KHY = TYLC(JYQXB[JYQXBfirstNum+g].KHY);
QXB[QXBfirstNum+g].KYH = TYLC(JYQXB[JYQXBfirstNum+g].KYH);
QXB[QXBfirstNum+g].KHZ = TYLC(JYQXB[JYQXBfirstNum+g].KHZ);
}
else
{
_tcscpy(LCACHAR,LCchr(GHACHAR,JYQXB[JYQXBfirstNum+g].KZH,3,1));
QXB[QXBfirstNum+g].KZH = TYLC1(LCACHAR,NDL,DLB);

//同理
_tcscpy(LCACHAR,LCchr(GHACHAR,JYQXB[JYQXBfirstNum+g].KHY,3,1));
QXB[QXBfirstNum+g].KHY = TYLC1(LCACHAR,NDL,DLB);

//
_tcscpy(LCACHAR,LCchr(GHACHAR,JYQXB[JYQXBfirstNum+g].KYH,3,1));
QXB[QXBfirstNum+g].KYH = TYLC1(LCACHAR,NDL,DLB);

//
_tcscpy(LCACHAR,LCchr(GHACHAR,JYQXB[JYQXBfirstNum+g].KHZ,3,1));
QXB[QXBfirstNum+g].KHZ = TYLC1(LCACHAR,NDL,DLB);
}
//
//TYLC()
//	 split_ckml()
//	

QXB[QXBfirstNum+g].a = JYQXB[JYQXBfirstNum+g].a;
QXB[QXBfirstNum+g].l1 = JYQXB[JYQXBfirstNum+g].l1;
QXB[QXBfirstNum+g].R = JYQXB[JYQXBfirstNum+g].R;
QXB[QXBfirstNum+g].l2 = JYQXB[JYQXBfirstNum+g].l2;
QXB[QXBfirstNum+g].T1 = JYQXB[JYQXBfirstNum+g].T1;
QXB[QXBfirstNum+g].T2 = JYQXB[JYQXBfirstNum+g].T2;
QXB[QXBfirstNum+g].L = JYQXB[JYQXBfirstNum+g].L;
QXB[QXBfirstNum+g].LorR = JYQXB[JYQXBfirstNum+g].LorR;
}
}
}


/*
for(int i=0;i<QXB.GetSize();i++)
{
ads_printf(L"******  JDNum=%s,l1=%lf,R=%lf,l2=%lf\n",QXB[i].JDNum,QXB[i].l1,QXB[i].R,QXB[i].l2);
}
for(int i=0;i<JYQXB.GetSize();i++)
{
ads_printf(L"*?????  JDNum=%s,l1=%lf,R=%lf,l2=%lf\n",JYQXB[i].JDNum,JYQXB[i].l1,JYQXB[i].R,JYQXB[i].l2);
}

//临时存储交点下标数组
CArray<CString,CString>TempJDNumArray;
//曲线表中第一个下标，所有数数目
int QXBfirstNum,QXBAllNum;
//既有曲线表中第一个下标，所有数数目
int JYQXBfirstNum,JYQXBAllNum;

int JDAllNum;
////把平面计算的QXB + JYQXB 组合成  QXB
for(int i=0;i<NJD;i++)
{	
if(QXB[i].isGJ==TRUE)
continue;
//得到指定交点号在曲线表中的　第一个下标，所有满足条件的总数
GetArrayCS(QXB,QXB[i].JDNum,QXBfirstNum,QXBAllNum);
//得到指定交点号在既有曲线表中的　第一个下标，所有满足条件的总数
GetArrayCS(JYQXB,QXB[i].JDNum,JYQXBfirstNum,JYQXBAllNum);

if(JYQXBfirstNum<0)continue;
//求每个交点,在数组中相同个数
int k=0;
if(JYQXBAllNum==QXBAllNum)
{
ads_printf(L"QXB[i].JDNum=%s,QXBAllNum=%d\n",QXB[i].JDNum,QXBAllNum);
for(int g=0;g<QXBAllNum;g++)
{//JYQXBfirstNum
QXB[QXBfirstNum+g].KZH = TYLC(JYQXB[JYQXBfirstNum+g].KZH);
QXB[QXBfirstNum+g].KHY = TYLC(JYQXB[JYQXBfirstNum+g].KHY);
QXB[QXBfirstNum+g].KYH = TYLC(JYQXB[JYQXBfirstNum+g].KYH);
QXB[QXBfirstNum+g].KHZ = TYLC(JYQXB[JYQXBfirstNum+g].KHZ);

QXB[QXBfirstNum+g].a = JYQXB[JYQXBfirstNum+g].a;
QXB[QXBfirstNum+g].l1 = JYQXB[JYQXBfirstNum+g].l1;
QXB[QXBfirstNum+g].R = JYQXB[JYQXBfirstNum+g].R;
QXB[QXBfirstNum+g].l2 = JYQXB[JYQXBfirstNum+g].l2;
QXB[QXBfirstNum+g].T1 = JYQXB[JYQXBfirstNum+g].T1;
QXB[QXBfirstNum+g].T2 = JYQXB[JYQXBfirstNum+g].T2;
QXB[QXBfirstNum+g].L = JYQXB[JYQXBfirstNum+g].L;
QXB[QXBfirstNum+g].LorR = JYQXB[JYQXBfirstNum+g].LorR;
}
}
}


}*/

void GTZX_JYX_ROAD::DrawPMXL(AcGiWorldDraw* pWd)
{
	// 绘交点冠号
	//	acutPrintf(L"JD0=%d,JDGH=%s\n",JD0,JDGH);
	AcGePoint3d Pt;
	xlpoint PZtmp;
	xlpoint  PB,PE;
	double INDATA[5],LCB;
	AcGePoint3d  ptb,pte;
	AcGePoint3d MidPt,StartPt,EndPt,APt1,APt2;//线路走向
	double B,b;
	ACHAR str[280];
	double Fwj = 0.0;
	PZtmp.lc = PROJ_ML(array,tarray,m_XLNameNotePt.y,m_XLNameNotePt.x,m_XLNameNotePt.y,m_XLNameNotePt.x);
	BAS_DRAW_FUN_JYX::xlpoint_pz(array,tarray,&PZtmp);  
	B=PZtmp.a;
	xyddaa(PZtmp.x,PZtmp.y,m_XLNameNotePt.y,m_XLNameNotePt.x,&Fwj);
	pte[X] = PZtmp.y + xl_length*sin(Fwj)/4.0;
	pte[Y] = PZtmp.x + xl_length*cos(Fwj)/4.0;
	//pte[X]=PZtmp.y+xl_length*sin(B+PI+pi*0.5)/4.0;
	//pte[Y]=PZtmp.x+xl_length*cos(B+PI+pi*0.5)/4.0;
	ptb[X]=PZtmp.y;
	ptb[Y]=PZtmp.x;
	//////////////////////////////////////////////////////////////////////////
	if (IsdrawXLM)
	{
		//线路走向
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
		name=mdbname+ " --" + XLShuXing;

		name.Trim();

		//	ReadWorkdir();
		//		 Dir=Cworkdir;
		//	Dir.MakeUpper();
		//		 Cworkdir=Dir;
		//	     name.Replace(Cworkdir,NULL);
		//         name.Replace(L"\\DATA\\",NULL);
		//		 name.Replace(L".MDB",L"方案");
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


	if (IsdrawJD)
	{
		BZJD(pWd,JDarray,QXB,NJD);	//标注交点
	}
	//BZJD(pWd,JDarray,QXB,NJD);	//标注交点

	// 绘线位
	PB.x=array_pxy[0][0][1]; //N
	PB.y=array_pxy[0][0][2]; // E
	PB.a=rad(array_pxy[0][0][3]);// FA

	//从一个线元开始依次传递转角
	PB.lc=array_pxy[0][0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;

	for (int i =1; i < NJD; i++) 
	{

		for (int k =0; k <4; k++) 	
		{
			for (int j=0; j<5; j++)
			{
				INDATA[j]=array_pxy[i][k][j]; 
			}

			G_WXY(pWd,INDATA,&PB,&PE ); //绘线路中线
			//	ads_printf(L"i=%d,1x=%lf,1y=%lf,2x=%lf,2y=%lf\n",i,PB.x,PB.y,PE.x,PE.y);
			// GUXWXY(INDATA,&PB,&PE,1); 
			PB=PE;
		}
	}

	LCB=array_pxy[0][0][4];  // 起点里程
}

CString  GTZX_JYX_ROAD::XLC( double TYLC)
{
	CString ckml;
	double dml;
	ACHAR GH[8],kml[50];

	dml = XLC1(TYLC,GH,NDL,DLB);

	_tcscpy(kml,LCchr(GH,dml,3,1));
	ckml=kml;
	//	ads_printf(L"dml=%lf,ckml=%s\n",dml,ckml);
	return ckml;
}

Adesk::Boolean GTZX_JYX_ROAD::MakeGJDsToOne()
{
	return 0;
}

int GTZX_JYX_ROAD::GetCurJDno(double N,double E)
{
	int JDi;

	JDi=GetPreJD(N,E);

	if(JDi<NJD-1)	
	{
		double dist1,dist2;
		double f;
		dist1=xyddaa(JDarray[JDi].N,JDarray[JDi].E,N,E,&f);
		dist2=xyddaa(JDarray[JDi+1].N,JDarray[JDi+1].E,N,E,&f);

		if(dist2<dist1)JDi++;

	}
	return JDi;
}


void  GTZX_JYX_ROAD::DrawYXFW(AcGiWorldDraw* pWd,ACHAR GTZXLC[50],ACHAR GJDGTZXLC[50],int flag,ACHAR GJDName[255])
{

	double dlc = 100;
	int zybz, type=2;
	zybz = -1;	
	AcGePoint3d Pt;

	AcGePoint3d  pte,ptb;
	int k;
	double B;
	xlpoint  PZtmp; 
	ACHAR str[256];
	double  WB;
	//标志里程
	WB=1.5*texth;
	//1  ~ -1  有问题
	//1.kml ->cml
	PZtmp.lc = TYLC1(GJDGTZXLC,NDL,DLB);

	//	PZtmp.lc = CalnewcmlTocml(PZtmp.lc);


	PZtmp.lc = CalcmlTonewcml(PZtmp.lc);

	k=xlpoint_pz(array,tarray,&PZtmp);   

	B=PZtmp.a;
	//点起点
	Pt[Y]=PZtmp.x; 
	Pt[X]=PZtmp.y;
	Pt[Z]=0.0;

	//字起点
	ptb[X]=PZtmp.y+(WB+0.5*texth)*sin(B+pi*0.5*zybz);
	ptb[Y]=PZtmp.x+(WB+0.5*texth)*cos(B+pi*0.5*zybz);

	double  b=B+pi*0.5;
	b=pi*0.5-b; if (zybz==-1) b=b+pi;

	if (b<0.0) b=2*pi+b;

	if (b>2*pi) b=b-2*pi;	

	if(type<=1)  b=b-pi*0.5; 

	//起始里程
	//	_tcscpy(str,LC);
	_tcscpy(str,GJDName);


	if(flag==0)
	{
		_tcscat(str,L" 起点");
	}
	else
	{
		_tcscat(str,L" 终点");
	}

	AcGePoint3d BZPt;
	BZPt.y=ptb[Y]-(0.25*texth)*cos(B);
	BZPt.x=ptb[X]-(0.25*texth)*sin(B);
	G_maketext(pWd,BZPt,str,b,1.5*texth,QXYSColor,1);
	//点终点
	pte[X]=PZtmp.y+(_tcslen(str)*1.2*texth)*sin(B+pi*0.5*zybz);
	pte[Y]=PZtmp.x+(_tcslen(str)*1.2*texth)*cos(B+pi*0.5*zybz);
	G_makelineW(pWd,Pt,pte,QXYSColor,LineWeightValue);

	//标注里程
	_tcscpy(str ,GTZXLC); 
	BZPt.y=ptb[Y]+(2.0*texth)*cos(B);
	BZPt.x=ptb[X]+(2.0*texth)*sin(B);
	G_maketext(pWd,BZPt,str,b,1.5*texth,QXYSColor,1);
}

void GTZX_JYX_ROAD::GetDLBno(double sml,double eml,CArray<int,int&>&DLno)
{
	int i;
	double tylc;

	for(i=0;i<NDL;i++)
	{
		//	tylc = TYLC(DLB[i].BLC);

		tylc = PROJ_ML(array,tarray,DLB[i].N,DLB[i].E,DLB[i].N,DLB[i].E);

		if(tylc>sml-0.001&&tylc<eml+0.001)
			DLno.Add(i);
	}
}

/*
double  GTZX::DistFromPtToGTZX(double x, double y)
{
double cml =  PROJ_ML(array,tarray,x,y,x,y);
xlpoint PZ;
PZ.lc = cml;
xlpoint_pz(array,tarray,&PZ);
double dist,dx,dy;
dx = x-PZ.x;
dy = y=PZ.y;
dist = sqrt(dx*dx+dy*dy);
return dist;
}
*/


/////////////////////////////////////////////////////

double  GTZX_JYX_ROAD::DistFromPtToGTZX(double x, double y)
{
	double dist,dx,dy;
	/*计算点(x,y)到GTZX起点的距离，如果距离小于1e-6返回0.0*/
	dx = JDarray[0].N - x;
	dy = JDarray[0].E - y;
	dist = sqrt(dx*dx+dy*dy);
	if (dist<1e-3)
		return 1e-12;
	/*计算点(x,y)到GTZX终点的距离，如果距离小于1e-6返回0.0*/
	dx = JDarray[NJD-1].N - x;
	dy = JDarray[NJD-1].E - y;
	dist = sqrt(dx*dx+dy*dy);
	if (dist<1e-3)
		return 1e-12;
	double cml =  PROJ_ML(array,tarray,x,y,x,y);
	xlpoint PZ;
	PZ.lc = cml;
	xlpoint_pz(array,tarray,&PZ);
	dx = x-PZ.x;
	dy = y-PZ.y;
	dist = sqrt(dx*dx+dy*dy);
	return dist;
}

/************************************************************************
函数功能：创建夹直线数组
以交点为单位，将该交点前后夹直线的起终点，交点号作为一个夹直线结构存入夹直线数组中
*************************************************************************/
void GTZX_JYX_ROAD::CreatJZXArray()
{

	JZXArray.RemoveAll();
	JZXStruct tempJZX;

	tempJZX.QJZXStartPt.lc = QXB[0].KHZ;
	xlpoint_pz(array,tarray,&tempJZX.QJZXStartPt);
	tempJZX.QJZXEndPt.lc = QXB[0].KZH;
	xlpoint_pz(array,tarray,&tempJZX.QJZXEndPt);
	tempJZX.HJZXStartPt.lc = QXB[0].KHZ;
	xlpoint_pz(array,tarray,&tempJZX.HJZXStartPt);
	tempJZX.HJZXEndPt.lc = QXB[1].KZH;
	xlpoint_pz(array,tarray,&tempJZX.HJZXEndPt);
	tempJZX.JDNum = 0;
	JZXArray.Add(tempJZX);

	for (int i=1 ; i<NJD-1 ; i++)
	{
		tempJZX.QJZXStartPt.lc = QXB[i-1].KHZ;
		xlpoint_pz(array,tarray,&tempJZX.QJZXStartPt);
		tempJZX.QJZXEndPt.lc = QXB[i].KZH;
		xlpoint_pz(array,tarray,&tempJZX.QJZXEndPt);
		tempJZX.HJZXStartPt.lc = QXB[i].KHZ;
		xlpoint_pz(array,tarray,&tempJZX.HJZXStartPt);
		tempJZX.HJZXEndPt.lc = QXB[i+1].KZH;
		xlpoint_pz(array,tarray,&tempJZX.HJZXEndPt);
		tempJZX.JDNum = i;                                                       
		JZXArray.Add(tempJZX);
	}

	tempJZX.QJZXStartPt.lc = QXB[NJD-2].KHZ;
	xlpoint_pz(array,tarray,&tempJZX.QJZXStartPt);
	tempJZX.QJZXEndPt.lc = QXB[NJD-1].KZH;
	xlpoint_pz(array,tarray,&tempJZX.QJZXEndPt);
	tempJZX.HJZXStartPt.lc = QXB[NJD-1].KHZ;
	xlpoint_pz(array,tarray,&tempJZX.HJZXStartPt);
	tempJZX.HJZXEndPt.lc = QXB[NJD].KZH;
	xlpoint_pz(array,tarray,&tempJZX.HJZXEndPt);
	tempJZX.JDNum = NJD-1;
	JZXArray.Add(tempJZX);

}

/*************************************************************************
函数功能：判断二号线iJD2号交点下标的夹直线是否与一号线的某交点重叠
返回值：ture-有重叠的交点 false-无重叠的交点
输入参数：
iJD2		二号线的交点下标
pGTZX		二号线平面对象
QorH		-1-前夹直线是否重叠 其它-后夹直线是否重叠
输出参数
iJD1		重叠时，对应与一号线的交点下标
**************************************************************************/
bool GTZX_JYX_ROAD::IsJZXCD(int iJD2,GTZX_JYX_ROAD *pGTZX,int QorH,int iSJD1, int iEJD1,int &iJD1)
{
	bool isCD = false;//夹直线是否重叠
	double SPt1X,SPt1Y,EPt1X,EPt1Y;//一号线上夹直线起终点坐标
	double SPt2X,SPt2Y,EPt2X,EPt2Y;//二号线上夹直线起终点坐标
	if (QorH == -1)//判断前夹直线是否重叠
	{
		if (iJD2==0)
		{
			double dist1 = DistFromPtToGTZX(pGTZX->JDarray[0].N,pGTZX->JDarray[0].E);
			if (dist1<1e-6)
			{
				iJD1 = 0;
				return true;
			}
			return false;
		}
		SPt2X = pGTZX->JZXArray[iJD2].QJZXStartPt.x;
		SPt2Y = pGTZX->JZXArray[iJD2].QJZXStartPt.y;
		EPt2X = pGTZX->JZXArray[iJD2].QJZXEndPt.x;
		EPt2Y = pGTZX->JZXArray[iJD2].QJZXEndPt.y;

		if (iSJD1<1)
			iSJD1=1;
		if (iEJD1>NJD-1)
			iEJD1=NJD-1;

		for (int i=iSJD1 ; i<=iEJD1 ; i++ )
		{
			SPt1X = JZXArray[i].QJZXStartPt.x;
			SPt1Y = JZXArray[i].QJZXStartPt.y;
			EPt1X = JZXArray[i].QJZXEndPt.x;
			EPt1Y = JZXArray[i].QJZXEndPt.y;
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
		if (iJD2==pGTZX->NJD-1)
		{
			double dist2 = DistFromPtToGTZX(pGTZX->JDarray[pGTZX->NJD-1].N , pGTZX->JDarray[pGTZX->NJD-1].E);	
			if (dist2<1e-6)
			{
				iJD1 = NJD-1;
				return true;
			}
			return false;
		}
		SPt2X = pGTZX->JZXArray[iJD2].HJZXStartPt.x;
		SPt2Y = pGTZX->JZXArray[iJD2].HJZXStartPt.y;
		EPt2X = pGTZX->JZXArray[iJD2].HJZXEndPt.x;
		EPt2Y = pGTZX->JZXArray[iJD2].HJZXEndPt.y;

		if (iSJD1<0)
			iSJD1=0;
		if (iEJD1>NJD-2)
			iEJD1=NJD-2;
		for (int i=iSJD1 ; i<=iEJD1 ; i++)
		{
			SPt1X = JZXArray[i].HJZXStartPt.x;
			SPt1Y = JZXArray[i].HJZXStartPt.y;
			EPt1X = JZXArray[i].HJZXEndPt.x;
			EPt1Y = JZXArray[i].HJZXEndPt.y;
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

/**************************************************************************
函数功能：二号线中交点下标为sjdn到ejdn范围内搜索改建段起点在一号线的交点下标号
返回值：改建段起点在一号线上的交点下标。当为-1时表明在sjdn到ejdn范围内搜索不到改建段
输入参数：
sjdn		二号线上搜索的起始交点下标号
ejdn		二号线上搜索的终止交点下标号
pGTZX		二号线平面对象
***************************************************************************/
int GTZX_JYX_ROAD::SearchGJDSptIndex(int sjdn, int ejdn , GTZX_JYX_ROAD *pGTZX,int &iJD1)
{
	iJD1 = -1;
	int iJD2 = -10000;
	int iJD1Q = -1;
	int iJD1H = -1;
	int TmpiJD1 = -1;
	int TmpiJD2 = -1;
	bool qisInRoad = false, hisInRoad = false;//前后夹直线是否重叠

	if (pGTZX->JDarray[sjdn].isGJ)
	{
		double TmpXjj,TmpMl,TmpPt[2];
		CalXjj(pGTZX->QXB[sjdn].KZH,pGTZX->array,pGTZX->tarray,array,tarray,XJJ,TmpMl,TmpPt);
		iJD1 = DistToNearJD(TmpMl,false);
		iJD2 = sjdn;
		return iJD2;
	}

	qisInRoad = IsJZXCD(sjdn,pGTZX,-1,1,NJD-1,iJD1Q);

	if (sjdn>0 && pGTZX->JDarray[sjdn-1].isGJ)
	{
		double TmpXjj,TmpMl,TmpPt[2];
		CalXjj(pGTZX->QXB[sjdn-1].KHZ,pGTZX->array,pGTZX->tarray,array,tarray,XJJ,TmpMl,TmpPt);
		TmpiJD1 = DistToNearJD(TmpMl,false);
		TmpiJD1 = TmpiJD1-1 ;
		TmpiJD2 = sjdn-1 ;

		//		return iJD2;
	}

	for(int iJD2=sjdn; iJD2<= ejdn ; iJD2++)
	{
		hisInRoad = IsJZXCD(iJD2,pGTZX,1,0,NJD-2,iJD1H);
		if (qisInRoad && TmpiJD1!=-1 && iJD1Q>TmpiJD1+1 && iJD2>TmpiJD2+1)
		{
			iJD1 = -TmpiJD1;
			iJD2 = -TmpiJD2;
			return iJD2;
		}
		if (sjdn==0 && !qisInRoad)
		{
			iJD1 = 0;
			iJD2 = 0;
			return iJD2;
		}
		if(qisInRoad && !hisInRoad)
		{
			iJD1 = iJD1Q;
			return iJD2;
		}
		if(qisInRoad && hisInRoad)//修改了单曲线
		{
			iJD1 = iJD1Q;
			double R1,L011,L021;//一号线交点半径前后缓长
			double R2,L012,L022;//二号线交点半径前后缓长
			R1 = JDarray[iJD1].R;
			L011 = JDarray[iJD1].l1;
			L021 = JDarray[iJD1].l2;
			R2= pGTZX->JDarray[iJD2].R;
			L012 = pGTZX->JDarray[iJD2].l1;
			L022 = pGTZX->JDarray[iJD2].l2;
			if (fabs(R1-R2)>1e-6 || fabs(L011-L012)>1e-6 || fabs(L021-L022)>1e-6)
				return iJD2;
		}

		qisInRoad = hisInRoad;
		if (hisInRoad)
		{
			iJD1Q = iJD1H+1;
		}
	}

	return -10000;

	//iJD1 = -1;
	//int iJD2 = -1;
	//bool qisInRoad = false, hisInRoad = false;//前后夹直线是否重叠	
	//qisInRoad = IsJZXCD(sjdn,pGTZX,-1,iJD1);
	//for(int iJD2=sjdn; iJD2<= ejdn ; iJD2++)
	//{
	//	hisInRoad = IsJZXCD(iJD2,pGTZX,1,iJD1);

	//	if(qisInRoad && !hisInRoad)
	//		return iJD2;

	//	if(qisInRoad && hisInRoad)//修改了单曲线
	//	{
	//		double R1,L011,L021;//一号线交点半径前后缓长
	//		double R2,L012,L022;//二号线交点半径前后缓长
	//		R1 = JDarray[iJD1].R;
	//		L011 = JDarray[iJD1].l1;
	//		L021 = JDarray[iJD1].l2;
	//		R2= pGTZX->JDarray[iJD2].R;
	//		L012 = pGTZX->JDarray[iJD2].l1;
	//		L022 = pGTZX->JDarray[iJD2].l2;
	//		if (fabs(R1-R2)>1e-6 || fabs(L011-L012)>1e-6 || fabs(L021-L022)>1e-6)
	//			return iJD2;
	//	}

	//	qisInRoad = hisInRoad;
	//	if (hisInRoad)
	//	{
	//		iJD1++;
	//	}
	//}

	//return -1;
}

/**************************************************************************
函数功能：从二号线中交点下标为sjdn到ejdn范围内搜索改建段终点在一号线的交点下标号
返回值：改建段终点在一号线上的交点下标。当为-10000时表明在sjdn到ejdn范围内搜索不到改建段
输入参数：
sjdn		二号线上搜索的起始交点下标号
ejdn		二号线上搜索的终止交点下标号
pGTZX		二号线平面对象
***************************************************************************/
int GTZX_JYX_ROAD::SearchGJDEptIndex(int sjdn, int ejdn ,GTZX_JYX_ROAD *pGTZX,int &iJD1)
{
	bool qisInRoad, hisInRoad;//前后交点是否在线路上	
	iJD1 = -1;
	int iJD2 = -10000;
	int TmpiJD1 = -1;
	int TmpiJD2 = -1;
	qisInRoad = IsJZXCD(sjdn,pGTZX,-1,1,NJD-1,iJD1);
	hisInRoad = IsJZXCD(sjdn-1,pGTZX,1,0,NJD-2,iJD1);
	if (hisInRoad)
	{
		bool hisInRoad2 = IsJZXCD(sjdn-1,pGTZX,1,iJD1+1,NJD-2,iJD1);
		if (hisInRoad2 && qisInRoad)//截弯取直
			return sjdn-1;
	}

	//从sjdn开始搜索出下一个单曲线修改的交点号
	for(int iJD2=sjdn; iJD2<= ejdn ; iJD2++)
	{
		if (pGTZX->JDarray[iJD2].isGJ)//如果当前交点为单曲线绕行改建，当前交点即为一个单曲线绕行改建段
		{
			double TmpXjj,TmpMl,TmpPt[2];
			CalXjj(pGTZX->QXB[iJD2].KHZ,pGTZX->array,pGTZX->tarray,array,tarray,XJJ,TmpMl,TmpPt);
			iJD1 = DistToNearJD(TmpMl,true);
			return iJD2;
		}
		if (iJD2<ejdn && pGTZX->JDarray[iJD2+1].isGJ)//如果以后的交点为单曲线绕行改建，先记录下单曲线修改的交点号与后面根据夹直线重叠计算出的交点号进行比较
		{
			double TmpXjj,TmpMl,TmpPt[2];
			CalXjj(pGTZX->QXB[iJD2+1].KZH,pGTZX->array,pGTZX->tarray,array,tarray,XJJ,TmpMl,TmpPt);
			TmpiJD1 = DistToNearJD(TmpMl,true);
			TmpiJD2 = iJD2;
			TmpiJD1 = TmpiJD1+1;
			TmpiJD2 = TmpiJD2+1;
			break;
		}
	}


	for(int iJD2=sjdn; iJD2<= ejdn ; iJD2++)
	{
		hisInRoad = IsJZXCD(iJD2,pGTZX,1,0,NJD-2,iJD1);
		if (hisInRoad && TmpiJD2!=-1 && TmpiJD1!=-1 &&iJD2>=TmpiJD2 && iJD1>=TmpiJD1)//按夹直线重叠计算出的交点号在单曲线绕线改建交点的后面，改建段终点由单曲线绕行改建确定
		{
			iJD1 = -TmpiJD1;//交点号为负数，以便后面求算坐标时明确向前还是向后定位
			iJD2 = -TmpiJD2;
			return iJD2;
		}
		if (sjdn==pGTZX->NJD-1 && !hisInRoad)
		{
			iJD1 = NJD-1;
			iJD2 = pGTZX->NJD-1;
			return iJD2;
		}
		if(!qisInRoad && hisInRoad)
			return iJD2;
		if(qisInRoad && hisInRoad)//修改了单曲线
		{
			double R1,L011,L021;//一号线交点参数
			double R2,L012,L022;//二号线交点参数
			R1 = JDarray[iJD1].R;
			L011 = JDarray[iJD1].l1;
			L021 = JDarray[iJD1].l2;
			R2= pGTZX->JDarray[iJD2].R;
			L012 = pGTZX->JDarray[iJD2].l1;
			L022 = pGTZX->JDarray[iJD2].l2;
			if (fabs(R1-R2)>1 || fabs(L011-L012)>1 || fabs(L021-L022)>1 )  //1e-6
				return iJD2;
		}

		qisInRoad = hisInRoad;
		if (hisInRoad)
		{
			iJD1++;
		}
	}
	return -10000;
}


//创建改建段信息表
int GTZX_JYX_ROAD::CreatGJDInfTab(GTZX_JYX_ROAD *pGTZX)
{
	//	assertWriteEnabled();

	if(pGTZX==NULL)
	{
		return 0;
	}


	CreatJZXArray();


	pGTZX->CreatJZXArray();

	int sjd,ejd;//搜索的起终交点下标
	int sno2,eno2;//改建段起终点在二号线上的交点下标
	int sno1,eno1;//改建段起终点在一号线上的交点下标
	double spt[2],ept[2];//改建段起终点在一线上的坐标
	int GJDType;//改建段类型0-并行改建 1-S弯 2-绕行改建
	//bool isEXQDOnYX = true;//二线起点在一线上
	//bool isEXZDOnYX = true;//二线终点在一线上
	//bool isEnd = false;//
	GJDInf OneGjdInfo;//改建段信息

	//xlpoint PZ1,PZ2,PZ;
	//double ml1,ml2,ml;

	sjd = 0;
	ejd =  pGTZX->NJD -1;

	GJDInfTab.RemoveAll();

	//double dist1 = DistFromPtToGTZX(pGTZX->JDarray[0].N,pGTZX->JDarray[0].E);
	//double dist2 = DistFromPtToGTZX(pGTZX->JDarray[pGTZX->NJD-1].N , pGTZX->JDarray[pGTZX->NJD-1].E);
	//if (dist1>0.001)
	//	isEXQDOnYX = false;
	//if (dist2>0.001)
	//	isEXZDOnYX = false;

	////////////////////////////////////////////////////////////////////////
	for(;;)
	{
		sno2 = SearchGJDSptIndex(sjd,ejd,pGTZX,sno1);
		if(abs(sno2) >= sjd && sno2!=-10000)
		{
			sjd = sno2;
			CalGJDQZZB(sno1,sno2,-1,pGTZX,spt[0],spt[1]);

			eno2 = SearchGJDEptIndex(sjd,ejd,pGTZX,eno1);
			if(abs(eno2) >= sjd-1 && eno2!=-10000)//搜索到改建段终点
			{
				CalGJDQZZB(eno1,eno2,1,pGTZX,ept[0],ept[1]);
				if (eno2<0 && eno1<0)
				{
					eno1 = abs(eno1)-1;	
					eno2 = abs(eno2)-1;
				}
				GJDType = GetGJDType(sno1,sno2,eno1,eno2,pGTZX);
				if( GJDType==0 && sno2==eno2 && pGTZX->JDarray[sno2].isGJ)GJDType = 2 ;
				AddGJDInf(spt,ept,sno1,sno2,eno1,eno2,GJDType);

				if (eno2==sjd-1)
					sjd=sjd+1;
				else if(eno2 == sno2)
					sjd = sno2 + 1;
				else
					sjd = eno2;
			}
			else//未搜索到改建段终点
			{
				ept[0] = pGTZX->JDarray[pGTZX->NJD-1].N;
				ept[1] = pGTZX->JDarray[pGTZX->NJD-1].E;
				eno1 = NJD-1;
				eno2 = pGTZX->NJD-1;
				GJDType = GetGJDType(sno1,sno2,eno1,eno2,pGTZX);
				if( GJDType==0 && sno2==eno2 && pGTZX->JDarray[sno2].isGJ)GJDType = 2 ;
				AddGJDInf(spt,ept,sno1,sno2,eno1,eno2,GJDType);
				break ;
			}

		}
		else
			break;
	}

	NGJD = GJDInfTab.GetSize();
	return 1;
}

//////////////////////////////////////////////////////////////////////////
//创建改建段信息表
//输入参数：
//pGTZXGJ	设计基线，设计左右线转换为基线的指针
//JDShuxingArray	设计基线，设计左右线二线格式的交点属性
//调用方式：既有线对象的GTZX调用改函数就可生成改建段信息
//////////////////////////////////////////////////////////////////////////
int GTZX_JYX_ROAD::CreatGJDInfTab(GTZX_JYX_ROAD *pGTZXGJ,int JDShuxingArray[])
{
	//	assertWriteEnabled();

	return 1;

	//	if(pGTZXGJ==NULL)
	//	{
	//		return 0;
	//	}
	//
	//
	//	CreatJZXArray();
	//
	//
	//	pGTZXGJ->CreatJZXArray();
	//
	//	int sjd,ejd;//搜索的起终交点下标
	//	int sno2,eno2;//改建段起终点在二号线上的交点下标
	//	int sno1,eno1;//改建段起终点在一号线上的交点下标
	//	double spt[2],ept[2];//改建段起终点在一线上的坐标
	//	int GJDType;//改建段类型0-并行改建 1-S弯 2-绕行改建
	//	GJDInf OneGjdInfo;//改建段信息
	//	int GTZXGJJDSum;//改建线交点总数
	//
	//	//xlpoint PZ1,PZ2,PZ;
	//	//double ml1,ml2,ml;
	//
	//	sjd = 0;
	//	ejd =  pGTZXGJ->NJD -1;
	//	GJDInfTab.RemoveAll();
	//	//////////////////////////////////////////////////////////////////////////
	//	//生成绕行改建段
	//	GTZXGJJDSum = pGTZXGJ->NJD;
	//	bool IsFindRXGJStart = false;
	//	bool IsFindRXGJEnd = false;
	//	for (int nJD=0; nJD<GTZXGJJDSum; nJD++)
	//	{
	//		double TmpcmlGJ = 0.0;
	//		double TmpcmlJY = 0.0;
	//		double Tmpcml = 0.0;
	//		double TmpPt[2];
	//		double TmpXJJ = 0.0;
	//		xlpoint TmpPZ;
	//		if (JDShuxingArray[nJD]==R_XStart || JDShuxingArray[nJD]==R_X_OneR)
	//		{
	//			OneGjdInfo.GJDType = 2;
	//			OneGjdInfo.iSJD2 = nJD;
	//			TmpPZ.lc = pGTZXGJ->QXB[nJD].KZH;
	//			pGTZXGJ->xlpoint_pz(pGTZXGJ->array,pGTZXGJ->tarray,&TmpPZ);
	//			TmpcmlGJ = PROJ_ML(array,tarray,TmpPZ.x,TmpPZ.y,TmpPZ.x,TmpPZ.y);//改建线改建段起点在既有线上的投影里程
	//			OneGjdInfo.iSJD1 = DistToNearJD(TmpcmlGJ,false);
	//			TmpcmlJY = QXB[OneGjdInfo.iSJD1].KZH;//既有线改建段起点在既有线上的连续里程
	//			Tmpcml = TmpcmlGJ<TmpcmlJY ? TmpcmlGJ : TmpcmlJY;
	//			Tmpcml = LocateHundredMeterOnLine(Tmpcml,true);
	//			CalXjj(Tmpcml,array,tarray,pGTZXGJ->array,pGTZXGJ->tarray,TmpXJJ,TmpcmlGJ,TmpPt);
	//			OneGjdInfo.spt[0] = TmpPt[0];
	//			OneGjdInfo.spt[1] = TmpPt[1];
	//			IsFindRXGJStart = true;
	//		}
	//		if (JDShuxingArray[nJD]==R_XEnd || JDShuxingArray[nJD]==R_X_OneR)
	//		{
	//			OneGjdInfo.GJDType = 2;
	//			OneGjdInfo.iEJD2 = nJD;
	//			TmpPZ.lc = pGTZXGJ->QXB[nJD].KHZ;
	//			pGTZXGJ->xlpoint_pz(pGTZXGJ->array,pGTZXGJ->tarray,&TmpPZ);
	//			TmpcmlGJ = PROJ_ML(array,tarray,TmpPZ.x,TmpPZ.y,TmpPZ.x,TmpPZ.y);//改建线改建段起点在既有线上的投影里程
	//			OneGjdInfo.iEJD1 = DistToNearJD(TmpcmlGJ,true);
	//			TmpcmlJY = QXB[OneGjdInfo.iEJD1].KHZ;//既有线改建段起点在既有线上的连续里程
	//			Tmpcml = TmpcmlGJ>TmpcmlJY ? TmpcmlGJ : TmpcmlJY;
	//			Tmpcml = LocateHundredMeterOnLine(Tmpcml,false);
	//			CalXjj(Tmpcml,array,tarray,pGTZXGJ->array,pGTZXGJ->tarray,TmpXJJ,TmpcmlGJ,TmpPt);
	//			OneGjdInfo.ept[0] = TmpPt[0];
	//			OneGjdInfo.ept[1] = TmpPt[1];
	//			IsFindRXGJEnd = true;
	//		}
	//		if (IsFindRXGJStart && IsFindRXGJEnd)
	//		{
	//			SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.GJDType,OneGjdInfo.gjdname);
	////			SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
	//			OneGjdInfo.ID = GJDInfTab.GetSize()+1;
	//			GJDInfTab.Add(OneGjdInfo);
	//			IsFindRXGJStart = false;
	//			IsFindRXGJEnd = false;
	//		}
	//
	//	}
	//	//////////////////////////////////////////////////////////////////////////
	//
	//	//////////////////////////////////////////////////////////////////////////
	//	//搜索绕行段中的利用段
	//	int RXGJDSum = GJDInfTab.GetSize();
	//	CArray<GJDInf,GJDInf> RXGJDInfTab;//绕行改建段信息表
	//	RXGJDInfTab.Append(GJDInfTab);
	//	CArray<LYD,LYD>LYDArray;
	//	LYD TmpLYD;
	//	double sml,eml;
	//	double SearchSpt[2],SearchEpt[2];
	//	int GJJDStart,GJJDEnd,JYJDStart,JYJDEnd;
	//	//////////////////////////////////////////////////////////////////////////
	//	//计算起点与第一个绕行段之间的并行段
	//	if (RXGJDSum==0)
	//	{
	//		GJJDStart = 0;
	//		GJJDEnd = pGTZXGJ->NJD-1;
	//		JYJDStart = 0;
	//		JYJDEnd = NJD-1;
	//		SearchSpt[0] = pGTZXGJ->JDarray[0].N;
	//		SearchSpt[1] = pGTZXGJ->JDarray[0].E;
	//		SearchEpt[0] = pGTZXGJ->JDarray[pGTZXGJ->NJD-1].N;
	//		SearchEpt[1] = pGTZXGJ->JDarray[pGTZXGJ->NJD-1].E;
	//		CreatBXGJD(pGTZXGJ,GJJDStart,GJJDEnd,JYJDStart,JYJDEnd,SearchSpt,SearchEpt);
	//	}
	//
	//	else
	//	{
	//		GJJDStart = 0;
	//		GJJDEnd = RXGJDInfTab[0].iSJD2-1;
	//		JYJDStart = 0;
	//		JYJDEnd = RXGJDInfTab[0].iSJD1-1;
	//		SearchSpt[0] = pGTZXGJ->JDarray[0].N;
	//		SearchSpt[1] = pGTZXGJ->JDarray[0].E;
	//		SearchEpt[0] = RXGJDInfTab[0].spt[0];
	//		SearchEpt[1] = RXGJDInfTab[0].spt[1];
	//		CreatBXGJD(pGTZXGJ,GJJDStart,GJJDEnd,JYJDStart,JYJDEnd,SearchSpt,SearchEpt);
	//
	//		for (int iRXGJD=0; iRXGJD<RXGJDSum-1; iRXGJD++)
	//		{
	//			GJJDStart = RXGJDInfTab[iRXGJD].iEJD2+1;
	//			GJJDEnd = RXGJDInfTab[iRXGJD+1].iSJD2-1;
	//			JYJDStart = RXGJDInfTab[iRXGJD].iEJD1+1;
	//			JYJDEnd = RXGJDInfTab[iRXGJD+1].iSJD1-1;
	//			SearchSpt[0] = RXGJDInfTab[iRXGJD].ept[0];
	//			SearchSpt[1] = RXGJDInfTab[iRXGJD].ept[1];
	//			SearchEpt[0] = RXGJDInfTab[iRXGJD+1].spt[0];
	//			SearchEpt[1] = RXGJDInfTab[iRXGJD+1].spt[1];
	//			CreatBXGJD(pGTZXGJ,GJJDStart,GJJDEnd,JYJDStart,JYJDEnd,SearchSpt,SearchEpt);
	//		}
	//
	//		GJJDStart = RXGJDInfTab[RXGJDSum-1].iEJD2+1;
	//		GJJDEnd = pGTZXGJ->NJD-1;
	//		JYJDStart = RXGJDInfTab[RXGJDSum-1].iEJD1+1;
	//		JYJDEnd = NJD-1;
	//		SearchSpt[0] = RXGJDInfTab[RXGJDSum-1].ept[0];
	//		SearchSpt[1] = RXGJDInfTab[RXGJDSum-1].ept[1];
	//		SearchEpt[0] = pGTZXGJ->JDarray[pGTZXGJ->NJD-1].N;
	//		SearchEpt[1] = pGTZXGJ->JDarray[pGTZXGJ->NJD-1].E;
	//		CreatBXGJD(pGTZXGJ,GJJDStart,GJJDEnd,JYJDStart,JYJDEnd,SearchSpt,SearchEpt);
	//	}
	//
	//	//////////////////////////////////////////////////////////////////////////
	//	//合并改建段信息数组
	//	SortGJDArray();
	//	MergeGJDArray();
	//	for (int iGJD=0; iGJD<NGJD; iGJD++)
	//	{
	//		ads_printf(L"改建类型%d,起点N=%lf,E=%lf,终点N=%lf,E=%lf\n",GJDInfTab[iGJD].GJDType,
	//			GJDInfTab[iGJD].spt[0],GJDInfTab[iGJD].spt[1],GJDInfTab[iGJD].ept[0],GJDInfTab[iGJD].ept[1]);
	//	}
	//	return 1;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//改建段数组排序
void GTZX_JYX_ROAD::SortGJDArray()
{
	double sml1,sml2;
	GJDInf TmpGJD;
	int GJDSum = GJDInfTab.GetSize();
	for (int i=0; i<GJDSum-1; i++)
	{
		for (int j=i+1; j<GJDSum; j++)
		{
			sml1 = PROJ_ML(array,tarray,GJDInfTab[i].spt[0],GJDInfTab[i].spt[1],GJDInfTab[i].spt[0],GJDInfTab[i].spt[1]);
			sml2 = PROJ_ML(array,tarray,GJDInfTab[j].spt[0],GJDInfTab[j].spt[1],GJDInfTab[j].spt[0],GJDInfTab[j].spt[1]);
			if (sml1>sml2)
			{
				TmpGJD = GJDInfTab[i];
				GJDInfTab[i] = GJDInfTab[j];
				GJDInfTab[j] = TmpGJD;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//改建段合并
void GTZX_JYX_ROAD::MergeGJDArray()
{
	for (int iGJD=0; iGJD<GJDInfTab.GetSize()-1; iGJD++)
	{
		double sml,eml;
		eml = PROJ_ML(array,tarray,GJDInfTab[iGJD].ept[0],GJDInfTab[iGJD].ept[1],GJDInfTab[iGJD].ept[0],GJDInfTab[iGJD].ept[1]);
		sml = PROJ_ML(array,tarray,GJDInfTab[iGJD+1].spt[0],GJDInfTab[iGJD+1].spt[1],GJDInfTab[iGJD+1].spt[0],GJDInfTab[iGJD+1].spt[1]);
		if (fabs(sml-eml)<0.1)//改建段数组非常接近。需进行改建段数组的合并
		{
			if (GJDInfTab[iGJD].GJDType != GJDInfTab[iGJD+1].GJDType && eml-sml>0.1)//前后改建段交叉，且改建类型不同
				GJDInfTab[iGJD].GJDType = 2; 

			GJDInfTab[iGJD].ept[0] = GJDInfTab[iGJD+1].ept[0];
			GJDInfTab[iGJD].ept[1] = GJDInfTab[iGJD+1].ept[1];
			GJDInfTab.RemoveAt(iGJD+1);
			iGJD--;
		}
	}
	NGJD = GJDInfTab.GetSize();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//搜索两绕行段之间的并行改建段
//////////////////////////////////////////////////////////////////////////
int GTZX_JYX_ROAD::CreatBXGJD(GTZX_JYX_ROAD* pGTZXGJ,int GJJDStart,int GJJDEnd,int JYJDStart,int JYJDEnd,double spt[2],double ept[2])
{

	double sml,eml;
	double Tmpcml;
	bool IsQJZXCD,IsHJZXCD;//前后夹直线是否重叠
	int iQGJJD,iQJYJD;//前夹直线重叠的既有和改建交点下标
	int iHGJJD,iHJYJD;//后夹直线重叠的既有和改建交点下标
	double TmpXJJ,TmpProjml;
	double TmpPt[2];
	GJDInf OneGjdInfo;
	CArray<LYD,LYD>LYDArray;
	LYD TmpLYD;
	//sml = pGTZXGJ->PROJ_ML(pGTZXGJ->array,pGTZXGJ->tarray,spt[0],spt[1],spt[0],spt[1]);
	//eml = pGTZXGJ->PROJ_ML(pGTZXGJ->array,pGTZXGJ->tarray,ept[0],ept[1],ept[0],ept[1]);
	sml = PROJ_ML(array,tarray,spt[0],spt[1],spt[0],spt[1]);
	eml = PROJ_ML(array,tarray,ept[0],ept[1],ept[0],ept[1]);
	if (eml-sml<0.001)
		return 0;

	if (JYJDStart>JYJDEnd)//两个绕行改建段共用一条既有线直线边
	{
		CalXjj(0.5*(sml+eml),pGTZXGJ->array,pGTZXGJ->tarray,array,tarray,TmpXJJ,TmpProjml,TmpPt);
		if (fabs(TmpXJJ)>0.1)//并行改建
		{
			OneGjdInfo.GJDType = 0;
			OneGjdInfo.spt[0] = spt[0];
			OneGjdInfo.spt[1] = spt[1];
			OneGjdInfo.ept[0] = ept[0];
			OneGjdInfo.ept[1] = ept[1];
			OneGjdInfo.iSJD1 = JYJDStart;
			OneGjdInfo.iEJD1 = JYJDEnd;
			OneGjdInfo.iSJD2 = GJJDStart;
			OneGjdInfo.iEJD2 = GJJDEnd;
			OneGjdInfo.ID = GJDInfTab.GetSize()+1;
			SetDefaultGjdName(spt[0],spt[1],0,OneGjdInfo.gjdname);
			//			SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
			GJDInfTab.Add(OneGjdInfo);
			return 1;
		}
	}

	else
	{
		iQGJJD = GJJDStart;
		iHGJJD = GJJDStart;
		for (int iGJJD=GJJDStart;iGJJD<=GJJDEnd;iGJJD++)
		{
			IsQJZXCD = IsJZXCD(iGJJD,pGTZXGJ,-1,JYJDStart,JYJDEnd,iQJYJD);//搜索与iGJJD前夹直线重叠的既有线交点
			IsHJZXCD = IsJZXCD(iGJJD,pGTZXGJ,+1,JYJDStart,JYJDEnd,iHJYJD);//搜索与iGJJD前夹直线重叠的既有线交点
			if (IsQJZXCD && IsHJZXCD && iQJYJD==iHJYJD)//前后夹直线均重叠且曲线要素相同
			{
				if (
					fabs(JDarray[iQJYJD].R - pGTZXGJ->JDarray[iGJJD].R)<0.001
					&& fabs(JDarray[iQJYJD].l1 - pGTZXGJ->JDarray[iGJJD].l1)<0.001
					&& fabs(JDarray[iQJYJD].l2 - pGTZXGJ->JDarray[iGJJD].l2)<0.001
					)
				{
					//Tmpcml = pGTZXGJ->PROJ_ML(pGTZXGJ->array,pGTZXGJ->tarray,
					//	JZXArray[iQJYJD].QJZXStartPt.x,JZXArray[iQJYJD].QJZXStartPt.y,
					//	JZXArray[iQJYJD].QJZXStartPt.x,JZXArray[iQJYJD].QJZXStartPt.y);
					//TmpLYD.sml = max(pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.lc,Tmpcml);
					//Tmpcml = pGTZXGJ->PROJ_ML(pGTZXGJ->array,pGTZXGJ->tarray,
					//	JZXArray[iHJYJD].QJZXEndPt.x,JZXArray[iHJYJD].QJZXEndPt.y,
					//	JZXArray[iHJYJD].QJZXEndPt.x,JZXArray[iHJYJD].QJZXEndPt.y);
					//TmpLYD.eml = min(pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.lc,Tmpcml);
					//LYDArray.Add(TmpLYD);

					Tmpcml = PROJ_ML(array,tarray,
						pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.x,pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.y,
						pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.x,pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.y);
					TmpLYD.sml = max(JZXArray[iQJYJD].QJZXStartPt.lc,Tmpcml);
					Tmpcml = PROJ_ML(array,tarray,
						pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.x,pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.y,
						pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.x,pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.y);
					TmpLYD.eml = min(JZXArray[iHJYJD].HJZXEndPt.lc,Tmpcml);
					LYDArray.Add(TmpLYD);
				}

			}
			if (IsQJZXCD && !IsHJZXCD)//前夹直线上有利用部分
			{
				//Tmpcml = pGTZXGJ->PROJ_ML(pGTZXGJ->array,pGTZXGJ->tarray,
				//	JZXArray[iQJYJD].QJZXStartPt.x,JZXArray[iQJYJD].QJZXStartPt.y,
				//	JZXArray[iQJYJD].QJZXStartPt.x,JZXArray[iQJYJD].QJZXStartPt.y);				
				//TmpLYD.sml = max(pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.lc,Tmpcml);

				//Tmpcml = pGTZXGJ->PROJ_ML(pGTZXGJ->array,pGTZXGJ->tarray,
				//	JZXArray[iQJYJD].QJZXEndPt.x,JZXArray[iQJYJD].QJZXEndPt.y,
				//	JZXArray[iQJYJD].QJZXEndPt.x,JZXArray[iQJYJD].QJZXEndPt.y);
				//TmpLYD.eml = min(pGTZXGJ->JZXArray[iGJJD].QJZXEndPt.lc,Tmpcml);
				//LYDArray.Add(TmpLYD);

				Tmpcml = PROJ_ML(array,tarray,
					pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.x,pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.y,
					pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.x,pGTZXGJ->JZXArray[iGJJD].QJZXStartPt.y);				
				TmpLYD.sml = max(JZXArray[iQJYJD].QJZXStartPt.lc,Tmpcml);

				Tmpcml = PROJ_ML(array,tarray,
					pGTZXGJ->JZXArray[iGJJD].QJZXEndPt.x,pGTZXGJ->JZXArray[iGJJD].QJZXEndPt.y,
					pGTZXGJ->JZXArray[iGJJD].QJZXEndPt.x,pGTZXGJ->JZXArray[iGJJD].QJZXEndPt.y);	
				TmpLYD.eml = min(JZXArray[iQJYJD].QJZXEndPt.lc,Tmpcml);
				LYDArray.Add(TmpLYD);
			}
			if (!IsQJZXCD && IsHJZXCD)//后夹直线上有利用部分
			{
				//Tmpcml = pGTZXGJ->PROJ_ML(pGTZXGJ->array,pGTZXGJ->tarray,
				//	JZXArray[iHJYJD].HJZXStartPt.x,JZXArray[iHJYJD].HJZXStartPt.y,
				//	JZXArray[iHJYJD].HJZXStartPt.x,JZXArray[iHJYJD].HJZXStartPt.y);			
				//TmpLYD.sml = max(pGTZXGJ->JZXArray[iGJJD].HJZXStartPt.lc,Tmpcml);
				//Tmpcml = pGTZXGJ->PROJ_ML(pGTZXGJ->array,pGTZXGJ->tarray,
				//	JZXArray[iHJYJD].HJZXEndPt.x,JZXArray[iHJYJD].HJZXEndPt.y,
				//	JZXArray[iHJYJD].HJZXEndPt.x,JZXArray[iHJYJD].HJZXEndPt.y);			
				//TmpLYD.eml = min(pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.lc,Tmpcml);
				//LYDArray.Add(TmpLYD);

				Tmpcml = PROJ_ML(array,tarray,
					pGTZXGJ->JZXArray[iGJJD].HJZXStartPt.x,pGTZXGJ->JZXArray[iGJJD].HJZXStartPt.y,
					pGTZXGJ->JZXArray[iGJJD].HJZXStartPt.x,pGTZXGJ->JZXArray[iGJJD].HJZXStartPt.y);				
				TmpLYD.sml = max(JZXArray[iHJYJD].HJZXStartPt.lc,Tmpcml);

				Tmpcml = PROJ_ML(array,tarray,
					pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.x,pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.y,
					pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.x,pGTZXGJ->JZXArray[iGJJD].HJZXEndPt.y);	
				TmpLYD.eml = min(JZXArray[iHJYJD].HJZXEndPt.lc,Tmpcml);
				LYDArray.Add(TmpLYD);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//内插出并行改建段
		for (int iLYD=0; iLYD<LYDArray.GetSize()-1; iLYD++)
		{
			if (LYDArray[iLYD].eml>LYDArray[iLYD+1].sml)
			{
				LYDArray[iLYD].eml = LYDArray[iLYD+1].eml;
				LYDArray.RemoveAt(iLYD+1);
				iLYD--;
			}
		}
		int LYDSum = LYDArray.GetSize();
		if (LYDSum>0)
		{
			if (LYDArray[0].sml>sml+1.0)//在第一个利用段之前有并行改建段
			{
				OneGjdInfo.GJDType = 0;
				OneGjdInfo.spt[0] = spt[0];
				OneGjdInfo.spt[1] = spt[1];
				Tmpcml = LocateHundredMeterOnLine(LYDArray[0].sml,false);
				CalXjj(Tmpcml,array,tarray,pGTZXGJ->array,pGTZXGJ->tarray,TmpXJJ,TmpProjml,TmpPt);
				OneGjdInfo.ept[0] = TmpPt[0];
				OneGjdInfo.ept[1] = TmpPt[1];
				OneGjdInfo.ID = GJDInfTab.GetSize()+1;
				SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],0,OneGjdInfo.gjdname);
				//				SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
				GJDInfTab.Add(OneGjdInfo);
			}

			for (int iLYD=0; iLYD<LYDSum-1; iLYD++)
			{
				OneGjdInfo.GJDType = 0;
				Tmpcml = LocateHundredMeterOnLine(LYDArray[iLYD].eml,true);
				CalXjj(Tmpcml,array,tarray,pGTZXGJ->array,pGTZXGJ->tarray,TmpXJJ,TmpProjml,TmpPt);
				OneGjdInfo.spt[0] = TmpPt[0];
				OneGjdInfo.spt[1] = TmpPt[1];
				Tmpcml = LocateHundredMeterOnLine(LYDArray[iLYD+1].sml,false);
				CalXjj(Tmpcml,array,tarray,pGTZXGJ->array,pGTZXGJ->tarray,TmpXJJ,TmpProjml,TmpPt);
				OneGjdInfo.ept[0] = TmpPt[0];
				OneGjdInfo.ept[1] = TmpPt[1];
				OneGjdInfo.ID = GJDInfTab.GetSize()+1;
				SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],0,OneGjdInfo.gjdname);
				//				SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
				GJDInfTab.Add(OneGjdInfo);
			}

			if (LYDArray[LYDSum-1].eml<eml+1.0)//在第一个利用段之前有并行改建段
			{
				OneGjdInfo.GJDType = 0;
				Tmpcml = LocateHundredMeterOnLine(LYDArray[LYDSum-1].eml,true);
				CalXjj(Tmpcml,array,tarray,pGTZXGJ->array,pGTZXGJ->tarray,TmpXJJ,TmpProjml,TmpPt);
				OneGjdInfo.spt[0] = TmpPt[0];
				OneGjdInfo.spt[1] = TmpPt[1];
				OneGjdInfo.ept[0] = ept[0];
				OneGjdInfo.ept[1] = ept[1];
				OneGjdInfo.ID = GJDInfTab.GetSize()+1;
				SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],0,OneGjdInfo.gjdname);
				//				SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
				GJDInfTab.Add(OneGjdInfo);
			}
		}
		else//没有利用段
		{
			OneGjdInfo.GJDType = 0;
			OneGjdInfo.spt[0] = spt[0];
			OneGjdInfo.spt[1] = spt[1];
			OneGjdInfo.ept[0] = ept[0];
			OneGjdInfo.ept[1] = ept[1];
			OneGjdInfo.ID = GJDInfTab.GetSize()+1;
			SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],0,OneGjdInfo.gjdname);
			//			SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
			GJDInfTab.Add(OneGjdInfo);
		}
	}
	return GJDInfTab.GetSize();
}
//////////////////////////////////////////////////////////////////////////

void GTZX_JYX_ROAD::SetDefaultGjdName(double N,double E,int GJDType,ACHAR GjdName[256])
{
	double prjml = PROJ_ML(array,tarray,N,E,N,E);
	ACHAR GH[8];
	double dml = XLC(prjml,GH,NDL);
	dml = dml/1000.0;

	//既有线数据库名-属性-改建线-设计左(右)线
	//	_stprintf(GjdName,L"%s-%s-改建线%0.3lf",mdbname,XLShuXing,dml);
	//_stprintf(GjdName,L"%0.1lf%s改建线",dml,XLShuXing);
	if (GJDType==0)
		_stprintf(GjdName,L"%0.1lf并行改建",dml);
	if (GJDType==2)
		_stprintf(GjdName,L"%0.1lf绕行改建",dml);
}

//void GTZX::SetDefaultGjdName(int GJDType,ACHAR GJDName[256])
//{
//	_tcscpy(GJDName,L"\0");
//	if (GJDType==0)
//		_tcscpy(GJDName,L"并行改建");
//	if (GJDType==2)
//		_tcscpy(GJDName,L"绕行改建");
//}

void GTZX_JYX_ROAD::CalGJDQZZB(int iJD1,int iJD2,int QDorZD,GTZX_JYX_ROAD *pGTZX,double &PtX,double &PtY)
{
	double ml1,ml2,ml;
	xlpoint PZ2,PZ; 
	if (QDorZD == -1)//要计算改建段起点
	{
		if (iJD2>=0 && iJD1>=0)
		{
			ml1 = QXB[iJD1].KZH;//一号线上iJD1交点的ZH里程
			PZ2.lc = pGTZX->QXB[iJD2].KZH;//二号线上iJD2交点的ZH里程
		}
		else
		{
			ml1 = QXB[abs(iJD1)].KHZ;//一号线上iJD1交点的ZH里程
			PZ2.lc = pGTZX->QXB[abs(iJD2)].KHZ;//二号线上iJD2交点的ZH里程
		}
		//	PZ2.lc = LocateHundredMeterOnLine(PZ2.lc,true); //找ZH前面整百米
	}
	if (QDorZD == 1)//要计算改建段终点
	{
		if (iJD2>=0 && iJD1>=0)
		{
			ml1 = QXB[iJD1].KHZ;//一号线上iJD1交点的HZ里程
			PZ2.lc = pGTZX->QXB[iJD2].KHZ;//二号线上iJD2交点的HZ里程
		}
		else
		{
			ml1 = QXB[abs(iJD1)].KZH;//一号线上iJD1交点的HZ里程
			PZ2.lc = pGTZX->QXB[abs(iJD2)].KZH;//二号线上iJD2交点的HZ里程
		}

		//	PZ2.lc = LocateHundredMeterOnLine(PZ2.lc,false); //找HZ前面整百米
	}

	pGTZX->xlpoint_pz(pGTZX->array,pGTZX->tarray,&PZ2);//计算二号线上ZH或HZ点PZ2的大地坐标

	ml2 = PROJ_ML(array,tarray,PZ2.x,PZ2.y,PZ2.x,PZ2.y);//PZ2在一号线上的投影里程

	if (QDorZD == -1)
	{
		if (iJD2>=0 && iJD1>=0)
		{
			ml = ml1<ml2 ? ml1 : ml2 ;//取两条线路上ZH点在一号线上投影里程较小的里程
			ml = LocateHundredMeterOnLine(ml,true);
		}
		else
		{
			ml = ml1>ml2 ? ml1 : ml2 ;//取两条线路上ZH点在一号线上投影里程较小的里程
			ml = LocateHundredMeterOnLine(ml,true);
		}
	}
	if (QDorZD == 1)
	{
		if (iJD2>=0 && iJD1>=0)
		{
			ml = ml1>ml2 ? ml1 : ml2 ;//取两条线路上HZ点在一号线上投影里程较大的里程
			ml = LocateHundredMeterOnLine(ml,false);
		}
		else
		{
			ml = ml1<ml2 ? ml1 : ml2 ;//取两条线路上HZ点在一号线上投影里程较大的里程
			ml = LocateHundredMeterOnLine(ml,true);
		}
	}

	//由里程投影计算起终点的坐标
	double TmpXJJ,TmpMl,TmpPt[2];
	CalXjj(ml,array,tarray,pGTZX->array,pGTZX->tarray,TmpXJJ,TmpMl,TmpPt);
	PtX = TmpPt[0];
	PtY = TmpPt[1];

}

/*将改建段的信息存入改建段信息表*/
void GTZX_JYX_ROAD::AddGJDInf(double spt[2],double ept[2],int iSJD1,int iSJD2,int iEJD1,int iEJD2,int GJDType)//添加改建段信息
{
	GJDInf OneGjdInfo;
	OneGjdInfo.spt[0] = spt[0],OneGjdInfo.spt[1] = spt[1];
	OneGjdInfo.ept[0] = ept[0],OneGjdInfo.ept[1] = ept[1];
	OneGjdInfo.iSJD1 = iSJD1;
	OneGjdInfo.iSJD2 = iSJD2;
	OneGjdInfo.iEJD1 = iEJD1;
	OneGjdInfo.iEJD2 = iEJD2;
	OneGjdInfo.GJDType = GJDType;
	OneGjdInfo.ID = GJDInfTab.GetSize()+1;

	SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.GJDType,OneGjdInfo.gjdname);
	//	SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
	GJDInfTab.Add(OneGjdInfo);
}

void GTZX_JYX_ROAD::AddGJDInf(double spt[2],double ept[2],int iSJD1,int iSJD2,int iEJD1,int iEJD2,int GJDType,CString xln)//添加改建段信息
{
	GJDInf OneGjdInfo;
	OneGjdInfo.spt[0] = spt[0],OneGjdInfo.spt[1] = spt[1];
	OneGjdInfo.ept[0] = ept[0],OneGjdInfo.ept[1] = ept[1];
	OneGjdInfo.iSJD1 = iSJD1;
	OneGjdInfo.iSJD2 = iSJD2;
	OneGjdInfo.iEJD1 = iEJD1;
	OneGjdInfo.iEJD2 = iEJD2;
	OneGjdInfo.GJDType = GJDType;
	OneGjdInfo.ID = GJDInfTab.GetSize()+1;
	_tcscpy(OneGjdInfo.BXLName,xln);
	_tcscpy(OneGjdInfo.EXLName,xln);

	SetDefaultGjdName(OneGjdInfo.spt[0],OneGjdInfo.spt[1],OneGjdInfo.GJDType,OneGjdInfo.gjdname);
	//	SetDefaultGjdName(OneGjdInfo.GJDType,OneGjdInfo.gjdname);
	GJDInfTab.Add(OneGjdInfo);
}

/*获取下标为iNumGJD改建段的改建类型0-并行改建，1-S弯，2-绕行改建*/
int GTZX_JYX_ROAD::GetGJDType(int sno1,int sno2,int eno1,int eno2,GTZX_JYX_ROAD *pGTZX)
{
	int iSJD1,iSJD2;
	int iEJD1,iEJD2;
	iSJD1 = max(sno1-1 , 0);
	iSJD2 = max(sno2-1 , 0);
	iEJD1 = min(eno1 , NJD-2);
	iEJD2 = min(eno2 , pGTZX->NJD-2);

	double SPt1[2],SPt2[2];
	double EPt1[2],EPt2[2];
	CArray<int,int> BPXArray;//记录二号线不平行直线边起点交点号的数组
	for (int i=iSJD2 ; i<=iEJD2 ; i++)
	{
		bool isPX = false;
		SPt2[0] = pGTZX->JDarray[i].N;
		SPt2[1] = pGTZX->JDarray[i].E;
		EPt2[0] = pGTZX->JDarray[i+1].N;
		EPt2[1] = pGTZX->JDarray[i+1].E;
		int j;
		for (j=iSJD1 ; j<= iEJD1 ; j++)
		{
			SPt1[0] = JDarray[j].N;
			SPt1[1] = JDarray[j].E;
			EPt1[0] = JDarray[j+1].N;
			EPt1[1] = JDarray[j+1].E;
			isPX = isZXPX(SPt1,EPt1,SPt2,EPt2);
			if (isPX)
				break;
		}
		if (j==iEJD1+1)
			BPXArray.Add(i);
	}
	if (BPXArray.GetSize()==0)//二号线上没有与一号线不平行的直线
	{
		if (eno2-sno2 < eno1-sno1)
			return 2;//截弯取值
		else
			return 0;//并行改建
	}
	//		OneGjdInf.GJDType =  0;//并行改建
	int i;
	for ( i=0 ; i<BPXArray.GetSize() ; i++)
	{
		bool isPX=false;
		int k=BPXArray.GetAt(i);
		if (k>pGTZX->NJD-3 || k <1)//二号线的第一条和最后一条直线边与既有线不平行则认为第一改建段和最后一个改建段为绕行改建
			return 2;
		//			OneGjdInf.GJDType = 2 ;//绕行
		/*判断所有不平行边两头的直线边是否平行，如果平行则为S弯，否则为绕行改建*/
		SPt1[0] = pGTZX->JDarray[k-1].N;
		SPt1[1] = pGTZX->JDarray[k-1].E;
		EPt1[0] = pGTZX->JDarray[k].N;
		EPt1[1] = pGTZX->JDarray[k].E;
		SPt2[0] = pGTZX->JDarray[k+1].N;
		SPt2[1] = pGTZX->JDarray[k+1].E;
		EPt2[0] = pGTZX->JDarray[k+2].N;
		EPt2[1] = pGTZX->JDarray[k+2].E;

		isPX = isZXPX(SPt1,EPt1,SPt2,EPt2);
		if (!isPX)
			return 2;//绕行改建
		//			OneGjdInf.GJDType = 2 ;//绕行改建
	}
	if (i==BPXArray.GetSize())
		return 1;//S弯改建
	//		OneGjdInf.GJDType = 1;//S弯改建

	return 2;
}

/*判断两直线是否平行（包括共线的情况）*/
bool GTZX_JYX_ROAD::isZXPX(double SPt1[2],double EPt1[2],double SPt2[2],double EPt2[2])
{
	double dX1,dY1;
	double dX2,dY2;
	double dk1,dk2;
	dX1 = EPt1[0]-SPt1[0];
	dX2 = EPt2[0]-SPt2[0];
	dY1 = EPt1[1]-SPt1[1];
	dY2 = EPt2[1]-SPt2[1];
	if (fabs(dX1)<1e-3 && fabs(dX2)<1e-3)
	{
		return true;
	}
	else
	{
		dk1 = dY1 / dX1 ;
		dk2 = dY2 / dX2 ;
		if (fabs(dk1-dk2)<1e-3)
		{
			return true;
		}
	}
	return false;
}

void GTZX_JYX_ROAD::SetCKQXB(CArray<QXBStruct,QXBStruct>& CKQXBToIn)
{
	//对内部数组赋值
	//assertWriteEnabled();
	//CKQXB.RemoveAll();
	//CKQXB.Append(CKQXBToIn);

}

///////////////////////////////////////


/*两线路匹配，寻求最佳的平移向量和旋转角度*/
double GTZX_JYX_ROAD::GTZXFit(GTZX_JYX_ROAD *pGTZX,double &XMove,double &YMove,double &DegRotate)
{
	XMove = 0.0;
	YMove = 0.0;
	DegRotate = 0.0;
	CArray<AcGePoint3d,AcGePoint3d> JYJDArray,CLJDArray;
	if (NJD!=pGTZX->NJD)
	{
		AfxMessageBox(L"两条线路交点数不一致，无法匹配");
		return 1e6;
	}
	for (int k=0 ; k<NJD ; k++)
	{
		AcGePoint3d Pt;
		Pt.x = JDarray[k].E;
		Pt.y = JDarray[k].N;
		JYJDArray.Add(Pt);
		Pt.x = pGTZX->JDarray[k].E;
		Pt.y = pGTZX->JDarray[k].N;
		CLJDArray.Add(Pt);
	}

	int i,j,iter,ndim,np = 3;
	ndim = np;
	double fret,ftol = 1e-10;
	double p[4], xi[10];
	for (i = 1; i<=np; i++)
	{
		for (j = 1; j<=np; j++)
		{
			xi[(i-1)*np+j] = 0.0;
		}
	}
	xi[1] = 1.0;
	xi[1*np+2] = 1.0;
	xi[2*np+3] = 1.0;
	p[1] = 0.0; p[2] = 0.0; p[3] = 0.0;
	powell(p, xi, ndim, np, ftol, iter, fret ,JYJDArray, CLJDArray);
	XMove = p[1];
	YMove = p[2];
	DegRotate = p[3];
	return fret;
}

/*测量的线路平移旋转后得到修改后的线路，计算修改后的线路与既有的线路对应交点距离的平方和*/
double GTZX_JYX_ROAD::CalJDPC(CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray,double XMove,double YMove,double DegRotate)
{
	double Dist2;
	double dX,dY;
	double DistSum2=0.0;
	int nDate;
	CArray<AcGePoint3d,AcGePoint3d> XGJDArray;//经修改(平移旋转)后的交点坐标数组
	nDate = JYJDArray.GetSize();
	XGJDArray.SetSize(nDate);
	AcGeMatrix3d xformR,xformT;//平移旋转矩阵
	AcGePoint3d OriPt;//旋转基点
	OriPt.x = CLJDArray[0].x;
	OriPt.y = CLJDArray[0].y;
	AcGeVector3d norm(0,0,1);
	AcGeVector3d  moveBy(XMove,YMove,0);
	xformR.setToRotation(DegRotate,norm,OriPt);
	xformT.setToTranslation(moveBy);
	for (int i=0 ; i< nDate ; i++)
	{		
		XGJDArray[i].x = CLJDArray[i].x;
		XGJDArray[i].y = CLJDArray[i].y;
		XGJDArray[i].transformBy(xformT);
		XGJDArray[i].transformBy(xformR);
		dX = JYJDArray[i].x - XGJDArray[i].x;
		dY = JYJDArray[i].y - XGJDArray[i].y;
		Dist2 = dX * dX + dY * dY;
		DistSum2 += Dist2;
	}
	return DistSum2;
}

/*包威尔法求多元函数极小值*/
void GTZX_JYX_ROAD::powell(double p[], double xi[], int n, int np, double ftol, int& iter, double& fret,CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray)
{
	int i,j,ibig,itmax = 200;
	double pt[21], ptt[21], xit[21];
	double t,dum,fp,del,fptt;
	fret = func2(p, n,JYJDArray,CLJDArray);
	for (j = 1; j<=n; j++)
	{
		pt[j] = p[j];
	}
	iter = 0;
	do
	{
		do
		{
			do
			{
				iter = iter + 1;
				fp = fret;
				ibig = 0;
				del = 0.0;
				for (i = 1; i<=n; i++)
				{
					for (j = 1; j<=n; j++)
					{
						xit[j] = xi[(j-1)*np+i];
					}
					fptt = fret;
					linmin(p, xit, n, fret,JYJDArray,CLJDArray);
					ads_printf(L"交点距离平方和=%.9lf\n",fret);
					if (fabs(fptt - fret) > del)
					{
						del = fabs(fptt - fret);
						ibig = i;
					}
				}
				if (2.0 * fabs(fp - fret) <= ftol * (fabs(fp) + fabs(fret)))
				{
					eraseall(xit, ptt, pt);
					return;
				}
				if (iter == itmax)
				{
					AfxMessageBox(L"powell exceeding maximum iterations");
					return;
				}
				for (j = 1; j<=n; j++)
				{
					ptt[j] = 2.0 * p[j] - pt[j];
					xit[j] = p[j] - pt[j];
					pt[j] = p[j];
				}
				fptt = func2(ptt, n , JYJDArray , CLJDArray);
			}while (fptt >= fp);
			dum = fp - 2 * fret + fptt;
			t = 2.0 * dum * pow((fp - fret - del) , 2) - del * pow((fp - fptt) , 2);
		}while (t >= 0.0);
		linmin(p, xit, n, fret,JYJDArray,CLJDArray);
		ads_printf(L"交点距离平方和=%.9lf\n",fret);
		for (j = 1; j<=n; j++)
		{
			xi[(j-1)*np+ibig] = xit[j];
		}
	}while(1);
}

void GTZX_JYX_ROAD::eraseall(double pbar[], double prr[], double pr[])
{
	for (int i=0; i<=20; i++)
	{
		pbar[i]=0;
		prr[i]=0;
		pr[i]=0;
	}
}

void GTZX_JYX_ROAD::linmin(double p[], double xi[], int n, double& fret,CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray)
{
	int j;
	double tol = 0.0001;
	ncom = n;
	for (j = 1; j<=n; j++)
	{
		pcom[j] = p[j];
		xicom[j] = xi[j];
	}
	double fa,fx,fb,bx,ax = 0.0;
	double xmin,xx = 1.0;
	mnbrak(ax, xx, bx, fa, fx, fb,JYJDArray,CLJDArray);
	fret = brent(ax, xx, bx, tol, xmin,JYJDArray,CLJDArray);
	for (j = 1; j<=n; j++)
	{
		xi[j] = xmin * xi[j];
		p[j] = p[j] + xi[j];
	}
}

double GTZX_JYX_ROAD::brent(double ax, double bx, double cx, double tol, double& xmin,CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray)
{
	int done,iter,itmax = 100;
	double d,fu,r,q,p,xm,tol1,tol2,a,b,cgold = 0.381966;
	double u,etemp,dum,v,w,x,e,fx,fv1,fw,zeps = 0.0000000001;
	a = ax;
	if (cx < ax)
	{
		a = cx;
	}
	b = ax;
	if (cx > ax)
	{
		b = cx;
	}
	v = bx;
	w = v;
	x = v;
	e = 0.0;
	//	fx = func(x);
	fx = f1dim(x,JYJDArray,CLJDArray);
	fv1 = fx;
	fw = fx;
	for (iter = 1; iter<=itmax; iter++)
	{
		xm = 0.5 * (a + b);
		tol1 = tol * fabs(x) + zeps;
		tol2 = 2.0 * tol1;
		if (fabs(x - xm) <= tol2 - 0.5 * (b - a))
		{
			break;
		}
		done = -1;
		if (fabs(e) > tol1)
		{
			r = (x - w) * (fx - fv1);
			q = (x - v) * (fx - fw);
			p = (x - v) * q - (x - w) * r;
			q = 2.0 * (q - r);
			if (q > 0.0)
			{
				p = -p;
			}
			q = fabs(q);
			etemp = e;
			e = d;
			dum = fabs(0.5 * q * etemp);
			if (fabs(p) < dum && p > q * (a - x) && p < q * (b - x))
			{
				d = p / q;
				u = x + d;
				if (u - a < tol2 || b - u < tol2)
				{
					d = fabs(tol1) * sgn(xm - x);
				}
				done = 0;
			}
		}
		if (done)
		{
			if (x >= xm)
			{
				e = a - x;
			}
			else
			{
				e = b - x;
			}
			d = cgold * e;
		}
		if (fabs(d) >= tol1)
		{
			u = x + d;
		}
		else
		{
			u = x + fabs(tol1) * sgn(d);
		}
		fu = f1dim(u,JYJDArray,CLJDArray);
		if (fu <= fx)
		{
			if (u >= x)
			{
				a = x;
			}
			else
			{
				b = x;
			}
			v = w;
			fv1 = fw;
			w = x;
			fw = fx;
			x = u;
			fx = fu;
		}
		else
		{
			if (u < x)
			{
				a = u;
			}
			else
			{
				b = u;
			}
			if (fu <= fw || w == x)
			{
				v = w;
				fv1 = fw;
				w = u;
				fw = fu;
			}
			else
			{
				if (fu <= fv1 || v == x || v == w)
				{
					v = u;
					fv1 = fu;
				}
			}
		}
	}
	if (iter > itmax)
	{
		//	cout<<" brent exceed maximum iterations."<<endl;
		ads_printf(L"brent exceed maximum iterations.\n");
	}
	xmin = x;
	return fx;
}

void GTZX_JYX_ROAD::mnbrak(double& ax, double& bx, double& cx, double& fa, double& fb, double& fc, CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray)
{
	double r,q,dum,gold = 1.618034;
	int glimit = 100;
	double u,ulim,fu,tiny = 1e-20;
	fa = f1dim(ax,JYJDArray,CLJDArray);
	fb = f1dim(bx,JYJDArray,CLJDArray);
	if (fb > fa)
	{
		dum = ax;
		ax = bx;
		bx = dum;
		dum = fb;
		fb = fa;
		fa = dum;
	}
	cx = bx + gold * (bx - ax);
	fc = f1dim(cx,JYJDArray,CLJDArray);
	while (fb >= fc)
	{
		r = (bx - ax) * (fb - fc);
		q = (bx - cx) * (fb - fa);
		dum = q - r;
		if (fabs(dum) < tiny)
		{
			dum = tiny;
		}
		u = bx - ((bx - cx) * q - (bx - ax) * r) / (2 * dum);
		ulim = bx + glimit * (cx - bx);
		if ((bx - u) * (u - cx) > 0)
		{
			fu = f1dim(u,JYJDArray,CLJDArray);
			if (fu < fc)
			{
				ax = bx;
				fa = fb;
				bx = u;
				fb = fu;
				return;
			}
			else
			{
				if (fu > fb)
				{
					cx = u;
					fc = fu;
					return;
				}
			}
			u = cx + gold * (cx - bx);
			fu = f1dim(u,JYJDArray,CLJDArray);
		}
		else
		{
			if ((cx - u) * (u - ulim) > 0)
			{
				fu = f1dim(u,JYJDArray,CLJDArray);
				if (fu < fc)
				{
					bx = cx;
					cx = u;
					u = cx + gold * (cx - bx);
					fb = fc;
					fc = fu;
					fu = f1dim(u,JYJDArray,CLJDArray);
				}
			}
			else
			{
				if ((u - ulim) * (ulim - cx) >= 0)
				{
					u = ulim;
					fu = f1dim(u,JYJDArray,CLJDArray);
				}
				else
				{
					u = cx + gold * (cx - bx);
					fu = f1dim(u,JYJDArray,CLJDArray);
				}
			}
		}
		ax = bx;
		bx = cx;
		cx = u;
		fa = fb;
		fb = fc;
		fc = fu;
	}
}

double GTZX_JYX_ROAD::f1dim(double x , CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray)
{
	double xt[51];
	for (int j = 1; j<=ncom; j++)
	{
		xt[j] = pcom[j] + x * xicom[j];
	}
	return func2(xt, ncom , JYJDArray , CLJDArray);
	//erase xt
}

int GTZX_JYX_ROAD::sgn(double x)
{
	if (x>0)
	{
		return 1;
	}
	else
	{
		if(x<0)
		{
			return -1;
		}
	}
	return 0;
}

double GTZX_JYX_ROAD::func2(double x[], int n , CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray)
{
	return CalJDPC(JYJDArray,CLJDArray,x[1],x[2],x[3]);
}


//GTZX_JYX* GTZX_JYX::CreatJX(CArray<QXBStruct,QXBStruct> &QXBFromCL)
//{
//	CArray<JDarrayStruct,JDarrayStruct> XLJDArray;//拟合后的线路交点结构数组
//	double ml = 0.0;
//	int CZJDSum = NJD;//既有参照基线的交点总数
//	int CLJDSum = QXBFromCL.GetSize();//既有定测基线的交点总数
//
//	if (CZJDSum==0)//既有参照基线不存在
//	{
//		AfxMessageBox(L"参照基线不存在！请先设计参照基线");
//		return NULL;
//	}
//	if (CLJDSum==0)
//	{
//		AfxMessageBox(L"没有拨量成果数据！生成的既有线将与参照线完全一致");
//	}
//
//	XLJDArray.SetSize(CZJDSum);
//	CArray<QXBStruct,QXBStruct> QXBAfterFit;//拟合后的曲线表
//	QXBAfterFit.SetSize(CZJDSum);
//
//	CArray<int,int> NotCLJDIndexOnCZX;//参照线上未测量的交点下标数组
//	//////////////////////////////////////////////////////////////////////////
//	//形成合并曲线表：将参照线上的交点依次取出，判断该交点是否测量了.
//	//如果测量了则将测量数据存入合并曲线表，否则将参照线数据存入合并曲线表
//	for (int CZJDIndex=0; CZJDIndex<CZJDSum; CZJDIndex++)
//	{
//		CString CZJDNum = JDarray[CZJDIndex].JDNum;
//		int CLJDIndex;
//		for (CLJDIndex=0; CLJDIndex<CLJDSum; CLJDIndex++)
//		{
//			CString CLJDNum = QXBFromCL[CLJDIndex].JDNum;
//			if (_tcscmp(JDarray[CZJDIndex].JDNum,QXBFromCL[CLJDIndex].JDNum)==0)
//			{
//				if (fabs(QXBFromCL[CLJDIndex].a*QXBFromCL[CLJDIndex].LorR - QXB[CZJDIndex].a*QXB[CZJDIndex].LorR)>1.0)
//				{
//					CString ErrorStr;
//					ErrorStr.Format(L"参照线交点%s转角为%.6f,拨量计算交点%s转角为%lf。两者相差太大可能是拨量数据有误，是否继续计算？",
//						JDarray[CZJDIndex].JDNum,QXB[CZJDIndex].a*QXB[CZJDIndex].LorR,QXBFromCL[CLJDIndex].JDNum,QXBFromCL[CLJDIndex].a*QXBFromCL[CLJDIndex].LorR);
//					if (AfxMessageBox(ErrorStr,MB_ICONWARNING|MB_YESNO)==IDNO)
//						return NULL;
//				}
//				ml = QXBFromCL[CLJDIndex].KZH;
//				QXBAfterFit[CZJDIndex] = QXBFromCL[CLJDIndex];
//				XLJDArray[CZJDIndex].N = JDarray[CZJDIndex].N;
//				XLJDArray[CZJDIndex].E = JDarray[CZJDIndex].E;
//				break;
//			}
//		}
//		if (CLJDIndex==CLJDSum)//在测量交点数组中找不到该交点号，该交点未测量
//		{
//			ml = QXB[CZJDIndex].KZH;
//			NotCLJDIndexOnCZX.Add(CZJDIndex);
//			QXBAfterFit[CZJDIndex] = QXB[CZJDIndex];
//			XLJDArray[CZJDIndex].N = JDarray[CZJDIndex].N;
//			XLJDArray[CZJDIndex].E = JDarray[CZJDIndex].E;
//		}
//	}
//	//////////////////////////////////////////////////////////////////////////
//
//
//	//////////////////////////////////////////////////////////////////////////
//	//分段拟合
//	int NotCLJDIndexOnCZXSum = NotCLJDIndexOnCZX.GetSize();
//	if (CZJDSum > NotCLJDIndexOnCZXSum)//至少测量了一个交点
//	{
//		for (int i=0; i<NotCLJDIndexOnCZXSum-1; i++)
//		{
//			if (NotCLJDIndexOnCZX[i+1] - NotCLJDIndexOnCZX[i] > 1)
//			{
//				int StrtIndex = NotCLJDIndexOnCZX[i];
//				int EndIndex = NotCLJDIndexOnCZX[i+1];
//				int FitJDSum = EndIndex - StrtIndex -1 ;
//				CXLJDFit XLJDFit(FitJDSum);
//				for (int j=StrtIndex; j<=EndIndex; j++)
//				{
//					double N,E,ZJ;
//					N = XLJDFit.m_CZJDArray[j-StrtIndex].N = JDarray[j].N;
//					E = XLJDFit.m_CZJDArray[j-StrtIndex].E = JDarray[j].E;
//					ZJ = XLJDFit.m_FitJDArray[j-StrtIndex].ZJ = QXBAfterFit[j].a * QXBAfterFit[j].LorR;
//				}
//				XLJDFit.CalXLJDFit();
//
//				for (j=StrtIndex; j<=EndIndex; j++)
//				{
//					double N,E;
//					N = XLJDArray[j].N = XLJDFit.m_FitJDArray[j-StrtIndex].N;
//					E = XLJDArray[j].E = XLJDFit.m_FitJDArray[j-StrtIndex].E;
//				}
//			}
//		}
//		for (int i=0 ; i<CZJDSum ; i++)
//		{
//			_tcscpy(XLJDArray[i].JDNum, QXBAfterFit[i].JDNum);
//			XLJDArray[i].isGJ = false;
//			XLJDArray[i].R = QXBAfterFit[i].R;
//			XLJDArray[i].l1 = QXBAfterFit[i].l1;
//			XLJDArray[i].l2 = QXBAfterFit[i].l2;
//		}
//	}
//
//	else//所有交点均未测量，采用参照基线数据
//	{
//		for (int i=0 ; i<CZJDSum ; i++)
//		{
//			_tcscpy(XLJDArray[i].JDNum, JDarray[i].JDNum);
//			XLJDArray[i].N = JDarray[i].N;
//			XLJDArray[i].E = JDarray[i].E;
//			XLJDArray[i].isGJ = false;
//			XLJDArray[i].R = JDarray[i].R;
//			XLJDArray[i].l1 = JDarray[i].l1;
//			XLJDArray[i].l2 = JDarray[i].l2;
//		}
//	}
//
//	//创建拟合后的既有基线
//	GTZX_JYX* pGTZXFit = new GTZX_JYX(CZJDSum,XLJDArray,DLB[0].EGH,L"JD");
//	//	pGTZXFit->XLShuXing = "既有基线";
//
//	//////////////////////////////////////////////////////////////////////////
//	//添加标志里程
//	pGTZXFit->BZLCarray.RemoveAll();
//	pGTZXFit->BZLCarray.SetSize(2*(CZJDSum-2)+2);
//
//	ACHAR GH[10];
//	_tcscpy(GH,DLB[0].EGH);
//	_tcscpy(pGTZXFit->BZLCarray[0].kml,LCchr(GH,DLB[0].BLC+DLB[0].DL,3,1));
//	pGTZXFit->BZLCarray[0].X = JDarray[0].N;
//	pGTZXFit->BZLCarray[0].Y = JDarray[0].E;
//
//	//终点
//	_tcscpy(GH,DLB[NDL-1].BGH);
//	_tcscpy(pGTZXFit->BZLCarray[2*(CZJDSum-2)+1].kml,LCchr(GH,DLB[NDL-1].BLC,3,1));
//	pGTZXFit->BZLCarray[2*(CZJDSum-2)+1].X = JDarray[CZJDSum-1].N;
//	pGTZXFit->BZLCarray[2*(CZJDSum-2)+1].Y = JDarray[CZJDSum-1].E;
//
//	//////////////////////////////////////////////////////////////////////////
//	//将ZH，HZ点的标志里程加入生成后的既有线
////	for (int i=1; i<CZJDSum-1; i++)
////	{
////		xlpoint PZ;
////		double dml;
////		//ZH
////		PZ.lc = pGTZXFit->QXB[i].KZH;
////		pGTZXFit->xlpoint_pz(pGTZXFit->array,pGTZXFit->tarray,&PZ);
////		pGTZXFit->BZLCarray[2*i-1].X = PZ.x;
////		pGTZXFit->BZLCarray[2*i-1].Y = PZ.y;
//////		dml = XLC1(QXB[i].KZH,GH,NDL,DLB);
////		dml = QXBAfterFit[i].KZH;
////		dml = XLC1(QXBAfterFit[i].KZH,GH,NDL,DLB);
////		_tcscpy(pGTZXFit->BZLCarray[2*i-1].kml,LCchr(GH,dml,3,1));
////
////		//HY
////		//PZ.lc = pGTZXFit->QXB[i].KHY;
////		//pGTZXFit->xlpoint_pz(pGTZXFit->array,pGTZXFit->tarray,&PZ);
////		//pGTZXFit->BZLCarray[4*i-2].X = PZ.x;
////		//pGTZXFit->BZLCarray[4*i-2].Y = PZ.y;
////		//dml = XLC1(QXB[i].KHY,GH,NDL,DLB);
////		//_tcscpy(pGTZXFit->BZLCarray[4*i-2].kml,LCchr(GH,dml,3,1));
////
////		//YH
////		//PZ.lc = pGTZXFit->QXB[i].KYH;
////		//pGTZXFit->xlpoint_pz(pGTZXFit->array,pGTZXFit->tarray,&PZ);
////		//pGTZXFit->BZLCarray[4*i-1].X = PZ.x;
////		//pGTZXFit->BZLCarray[4*i-1].Y = PZ.y;
////		//dml = XLC1(QXB[i].KYH,GH,NDL,DLB);
////		//_tcscpy(pGTZXFit->BZLCarray[4*i-1].kml,LCchr(GH,dml,3,1));
////
////		//HZ
////		PZ.lc = pGTZXFit->QXB[i].KHZ;
////		pGTZXFit->xlpoint_pz(pGTZXFit->array,pGTZXFit->tarray,&PZ);
////		pGTZXFit->BZLCarray[2*i].X = PZ.x;
////		pGTZXFit->BZLCarray[2*i].Y = PZ.y;
//////		dml = XLC1(QXB[i].KHZ,GH,NDL,DLB);
////		dml = QXBAfterFit[i].KHZ;
////		dml = XLC1(QXBAfterFit[i].KHZ,GH,NDL,DLB);
////		_tcscpy(pGTZXFit->BZLCarray[2*i].kml,LCchr(GH,dml,3,1));
////	}
//	//////////////////////////////////////////////////////////////////////////
//	//	AddEntityToDbs(pGTZXFit);
//	return pGTZXFit;
//}

/*获取pGTZX线路上两拾取点PickSPt，PickEPt范围内的改建段信息表*/
void GTZX_JYX_ROAD::SearchGJD(double PickSPt[2], double PickEPt[2], CArray<GJDInf,GJDInf> &XGGJDInfArray, GTZX_JYX_ROAD *pGTZX, CString xlm)
{
	double GJDSml,GJDEml;//改建段起终点投影里程
	double PickSml,PickEml;//拾取点起终投影里程
	double GJDSpt[2],GJDEpt[2];//改建段起终点坐标
	PickSml = pGTZX->PROJ_ML(pGTZX->array, pGTZX->tarray, PickSPt[0], PickSPt[1], PickSPt[0], PickSPt[1]);
	PickEml = pGTZX->PROJ_ML(pGTZX->array, pGTZX->tarray, PickEPt[0], PickEPt[1], PickEPt[0], PickEPt[1]);

	for (int i=0 ; i<XGGJDInfArray.GetSize() ; i++)
	{
		GJDSml = pGTZX->PROJ_ML(pGTZX->array,pGTZX->tarray,XGGJDInfArray[i].spt[0],XGGJDInfArray[i].spt[1],XGGJDInfArray[i].spt[0],XGGJDInfArray[i].spt[1]);
		GJDEml = pGTZX->PROJ_ML(pGTZX->array,pGTZX->tarray,XGGJDInfArray[i].ept[0],XGGJDInfArray[i].ept[1],XGGJDInfArray[i].ept[0],XGGJDInfArray[i].ept[1]);

		if ((GJDSml>PickSml-1e-3 && GJDSml<PickEml+1e-3) || (GJDEml>PickSml-1e-3 && GJDEml<PickEml+1e-3) || (GJDSml<PickSml && GJDEml>PickEml))
		{
			if (GJDSml<PickSml)
			{
				GJDSpt[0] = PickSPt[0];
				GJDSpt[1] = PickSPt[1];
			}
			else
			{
				GJDSpt[0] = XGGJDInfArray[i].spt[0];
				GJDSpt[1] = XGGJDInfArray[i].spt[1];
			}
			if (GJDEml>PickEml)
			{
				GJDEpt[0] = PickEPt[0];
				GJDEpt[1] = PickEPt[1];
			}
			else
			{
				GJDEpt[0] = XGGJDInfArray[i].ept[0];
				GJDEpt[1] = XGGJDInfArray[i].ept[1];
			}
			AddGJDInf(GJDSpt, GJDEpt, XGGJDInfArray[i].iSJD1, XGGJDInfArray[i].iSJD2, XGGJDInfArray[i].iEJD1, XGGJDInfArray[i].iEJD2, XGGJDInfArray[i].GJDType,xlm);
		}

		if (GJDSml > PickEml)
			break;
	}
}

bool GTZX_JYX_ROAD::JudgeGTZX2GJDArrayChanged(CArray<GJDAllInforStruct,GJDAllInforStruct>& p_OriGJDArr,
											  CArray<GJDAllInforStruct,GJDAllInforStruct>& p_NewGJDArr,
											  CArray<int,int>& p_ChangedIndexArr)
{
	int iOriSize = p_OriGJDArr.GetSize();
	int iNewSize = p_NewGJDArr.GetSize();

	if (iOriSize != iNewSize)
		return false;






	return true;



}

//统计出全线的并行改建段、绕行改建段，并将结果存入BasicSectionInfor数组
void GTZX_JYX_ROAD::FormSectionInfor()//形成分段信息数组
{

	int SizeOfGJDB = GJDInforArray.GetSize();

	GJDAllInforStruct TmpGJDInfor;
	if(SizeOfGJDB<1)return;
	//2.内插出初始利用段,得到基本分段信息

	//2.1特殊处理,形成第一段利用段-----------------------------------------------------------------
	BasicSectionInfor.RemoveAll();
	BasicSectionInfor.SetSize(SizeOfGJDB*2 + 1);

	_tcscpy(BasicSectionInfor[0].BXLName ,GJDInforArray[0].BXLName);
	_tcscpy(BasicSectionInfor[0].EXLName ,GJDInforArray[0].BXLName);

	CString strtmp = GJDInforArray[0].BXLName;

	BasicSectionInfor[0].BpointToN = JDarray[0].N ;
	BasicSectionInfor[0].BpointToE = JDarray[0].E ;
	if (_tcscmp(GJDInforArray[0].BXLName,L"既有基线")==0)	
		_tcscpy(BasicSectionInfor[0].GJDBLC , LCchr(JiXianpGtzx->DLB[0].EGH,JiXianpGtzx->DLB[0].BLC + JiXianpGtzx->DLB[0].DL,3,1));
	else if (_tcscmp(GJDInforArray[0].BXLName,L"既有左线")==0)
		_tcscpy(BasicSectionInfor[0].GJDBLC , LCchr(ZuoXianpGtzx->m_pm1->DLB[0].EGH,ZuoXianpGtzx->m_pm1->DLB[0].BLC+ZuoXianpGtzx->m_pm1->DLB[0].DL,3,1));
	else
		_tcscpy(BasicSectionInfor[0].GJDBLC , LCchr(YouXianpGtzx->m_pm1->DLB[0].EGH,YouXianpGtzx->m_pm1->DLB[0].BLC+YouXianpGtzx->m_pm1->DLB[0].DL,3,1));

	BasicSectionInfor[0].EpointToN = GJDInforArray[0].spt[0];
	BasicSectionInfor[0].EpointToE = GJDInforArray[0].spt[1];
	_tcscpy(BasicSectionInfor[0].GJDELC , GJDInforArray[0].GJDBLC);


	BasicSectionInfor[0].GJDType = -1; //利用段
	//-------------------------------------------------------------------------------------------------







	//2.2.循环处理,形成其他利用段和改建段,得到基本分段信息BasicSectionInfor----------------------------
	int Num;
	for(int i=0;i<SizeOfGJDB;i++)
	{
		//改建段分段信息
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


		//利用段分段信息
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





	//2.3特殊处理,形成最后一段利用段-----------------------------------------------------------

	_tcscpy(BasicSectionInfor[SizeOfGJDB*2].BXLName ,GJDInforArray[SizeOfGJDB-1].EXLName);
	_tcscpy(BasicSectionInfor[SizeOfGJDB*2].EXLName ,GJDInforArray[SizeOfGJDB-1].EXLName);

	BasicSectionInfor[SizeOfGJDB*2].BpointToN = GJDInforArray[SizeOfGJDB-1].ept[0];
	BasicSectionInfor[SizeOfGJDB*2].BpointToE = GJDInforArray[SizeOfGJDB-1].ept[1];
	_tcscpy(BasicSectionInfor[SizeOfGJDB*2].GJDBLC , GJDInforArray[SizeOfGJDB-1].GJDELC);

	BasicSectionInfor[SizeOfGJDB*2].EpointToN = JDarray[JDarray.GetSize()-1].N;
	BasicSectionInfor[SizeOfGJDB*2].EpointToE = JDarray[JDarray.GetSize()-1].E;

	if (_tcscmp(GJDInforArray[SizeOfGJDB-1].EXLName,L"既有基线")==0)	
		_tcscpy(BasicSectionInfor[SizeOfGJDB*2].GJDELC , LCchr(JiXianpGtzx->DLB[JiXianpGtzx->NDL-1].BGH,JiXianpGtzx->DLB[JiXianpGtzx->NDL-1].BLC,3,1));
	else if (_tcscmp(GJDInforArray[SizeOfGJDB-1].EXLName,L"既有左线")==0)
		_tcscpy(BasicSectionInfor[SizeOfGJDB*2].GJDELC , LCchr(ZuoXianpGtzx->m_pm1->DLB[ZuoXianpGtzx->m_pm1->NDL-1].BGH,ZuoXianpGtzx->m_pm1->DLB[ZuoXianpGtzx->m_pm1->NDL-1].BLC,3,1));
	else
		_tcscpy(BasicSectionInfor[SizeOfGJDB*2].GJDELC , LCchr(YouXianpGtzx->m_pm1->DLB[YouXianpGtzx->m_pm1->NDL-1].BGH,YouXianpGtzx->m_pm1->DLB[YouXianpGtzx->m_pm1->NDL-1].BLC,3,1));

	BasicSectionInfor[SizeOfGJDB*2].GJDType = -1; //利用段






	//////////////////////////////////////////////////////////////////////////
	//if (_tcscmp(this->isA()->name(),L"GTZX_JYX") ==0 )//贯通设计二线时不进行以下计算
	//{
	for (int iSection=0; iSection<BasicSectionInfor.GetSize(); iSection++)
	{
		pGTZXArray[iSection] = NULL;
		if (BasicSectionInfor[iSection].GJDType == 2)//绕行改建段
			pGTZXArray[iSection] = Get_GTX_RXGJD_GTZX(BasicSectionInfor[iSection]);
		_tcscpy(BasicSectionInfor[iSection].GJDBLC, GetGTXXclc(BasicSectionInfor[iSection].BpointToN,BasicSectionInfor[iSection].BpointToE));
		_tcscpy(BasicSectionInfor[iSection].GJDELC, GetGTXXclc(BasicSectionInfor[iSection].EpointToN,BasicSectionInfor[iSection].EpointToE));
	}
	//}
	//////////////////////////////////////////////////////////////////////////
}




//贯通设计基线里程转坐标
int GTZX_JYX_ROAD::xlpoint_pz_GTX(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ) 
{

	double sml , eml ;
	double CurN , CurE;
	int flag;
	struct xlpoint CurPZ;
	double Curcml; //投影里程
	ACHAR Curkml[50]; //投影点现场里程
	ACHAR FDDXCLC[50];//分段点的现场里程

	//对改建段数组循环

	PZ->a = 0.0;
	PZ->x = 0.0;
	PZ->y = 0.0;
	PZ->r = 0.0;

	if(BasicSectionInfor.GetSize()==0)
	{
		BAS_DRAW_FUN_JYX::xlpoint_pz(array,tarray,PZ);
	}

	for(int i=0;i<BasicSectionInfor.GetSize();i++)
	{

		//段落起点
		CurN = BasicSectionInfor[i].BpointToN ; 
		CurE = BasicSectionInfor[i].BpointToE ;
		//		_tcscpy(FDDXCLC , GetGTXXclc(CurN,CurE));
		_tcscpy(FDDXCLC,BasicSectionInfor[i].GJDBLC);
		sml = TYLC1(FDDXCLC,NDL,DLB);
		//		sml = PROJ_ML(array,tarray,CurN,CurE,CurN,CurE);

		//段落终点
		CurN = BasicSectionInfor[i].EpointToN ; 
		CurE = BasicSectionInfor[i].EpointToE ; 
		//		_tcscpy(FDDXCLC , GetGTXXclc(CurN,CurE));
		_tcscpy(FDDXCLC , BasicSectionInfor[i].GJDELC);
		eml = TYLC1(FDDXCLC,NDL,DLB);
		//		eml = PROJ_ML(array,tarray,CurN,CurE,CurN,CurE);

		//起点/终点里程之外

		if(i==0 &&(PZ->lc < sml - 0.0015)) 	
			PZ->lc = sml;

		if(i==BasicSectionInfor.GetSize()-1 && PZ->lc > eml + 0.0015) 
			PZ->lc = eml;

		//Zhujiang 20090731 
		if ((eml-sml<0.001) &&  i != BasicSectionInfor.GetSize() - 1)
			continue;

		if( PZ->lc>sml-0.0015 && PZ->lc<eml+0.0015)//在区段内，跳出循环，
		{
			//1.求现场里程
			_tcscpy(Curkml , XLC(PZ->lc));

			if(BasicSectionInfor[i].GJDType != 2)//完全利用原有或并行改建段
			{ 
				//				_tcscpy(Curkml , ModifyXCLCOnDL(PZ->lc));
				double CalN,CalE;

				CString XCLC2x;
				//2.求同一现场里程对应既有线上坐标
				if( _tcscmp(BasicSectionInfor[i].BXLName , L"既有基线")==0 ) //基线
				{
					if (fabs(PZ->lc-sml)<0.001)
						Curcml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,
						BasicSectionInfor[i].BpointToN,BasicSectionInfor[i].BpointToE,BasicSectionInfor[i].BpointToN,BasicSectionInfor[i].BpointToE);
					else if (fabs(PZ->lc-eml)<0.001)
						Curcml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,
						BasicSectionInfor[i].EpointToN,BasicSectionInfor[i].EpointToE,BasicSectionInfor[i].EpointToN,BasicSectionInfor[i].EpointToE);
					else
					{
						Curcml = JiXianpGtzx->TYLC1(Curkml,JiXianpGtzx->NDL , JiXianpGtzx->DLB);
						Curcml = JiXianpGtzx->CalcmlTonewcml(Curcml);
					}

					CurPZ.lc = Curcml ;
					// 得到既有线上坐标
					JiXianpGtzx->xlpoint_pz(JiXianpGtzx->array,JiXianpGtzx->tarray,&CurPZ);

				}
				else  //既有II线
				{
					GTZX2_JYX_ROAD *pm2=NULL;

					if(_tcscmp( BasicSectionInfor[i].BXLName , L"既有左线" )==0)
					{
						pm2 = ZuoXianpGtzx;
					}
					else
					{

						pm2 = YouXianpGtzx;
					}

					//同一个现场里程，在既有线上求对应坐标
					if(pm2)
					{
						XCLC2x = Curkml;

						Curcml = pm2->TYLC2x(XCLC2x,-1);//若iRXD＞0在指定绕行段查找
						CurPZ.lc = Curcml ;
						// 得到既有线上坐标
						BAS_DRAW_FUN_JYX::xlpoint_pz(pm2->array,pm2->tarray,&CurPZ);

					}
					else
					{
						BAS_DRAW_FUN_JYX::xlpoint_pz(array,tarray,PZ);
						return 1;
					}
				}

				if(BasicSectionInfor[i].GJDType!=-1)//并行改建段，再投影到贯通线
				{
					CalN = CurPZ.x ;
					CalE = CurPZ.y ;

					Curcml = PROJ_ML(array,tarray,CalN,CalE,CalN,CalE);
					PZ->lc = Curcml;

					BAS_DRAW_FUN_JYX::xlpoint_pz(array,tarray,PZ);
				}
				else//完全利用段
				{
					*PZ = CurPZ;
				}
				return 1;
			}
			else  //绕行改建
			{
				//取得绕行改建段GTZX指针

				if (IsEXRXD)
				{
					GTZX_JYX_ROAD * pm1=NULL;
					pm1 = Get_GTX_RXGJD_GTZX(BasicSectionInfor[i]);

					//计算子ＧＴＺＸ中ＮＥ坐标
					if(pm1)
					{
						double RXDsmlOnGTX,DistToRXDQD;
						//	RXDsmlOnGTX = PROJ_ML(array,tarray,pm1->JDarray[0].N,pm1->JDarray[0].E,pm1->JDarray[0].N,pm1->JDarray[0].E);

						ACHAR cXLC[40];

						_tcscpy(cXLC , pGTZXArray[i]->LCchr(pm1->DLB[0].EGH,pm1->DLB[0].BLC+pm1->DLB[0].DL,3,1));
						RXDsmlOnGTX = TYLC1(cXLC,NDL,DLB);
						//////////////////////////////////////////////////////////////////////////

						DistToRXDQD = PZ->lc - RXDsmlOnGTX;
						Curcml = pm1->DLB[0].BLC+pm1->DLB[0].DL +DistToRXDQD;

						PZ->lc = Curcml;


						int k= BAS_DRAW_FUN_JYX::xlpoint_pz(pm1->array,pm1->tarray,PZ);

						if (k>0&& k<=6)  //有解
						{
							delete pm1;
							return 1 ;
						}

						delete pm1;
					}
					else
					{
						ads_printf(L"子GTZX_JYX为空1");
						return BAS_DRAW_FUN_JYX::xlpoint_pz(array2,trow2,PZ);
					}
				}


				//////////////////////////////////////////////////////////////////////////
				//计算子ＧＴＺＸ中ＮＥ坐标
				else
				{
					if(pGTZXArray[i])
					{
						double RXDsmlOnGTX,DistToRXDQD;
						ACHAR cXLC[40];

						_tcscpy(cXLC , pGTZXArray[i]->LCchr(pGTZXArray[i]->DLB[0].EGH,pGTZXArray[i]->DLB[0].BLC + pGTZXArray[i]->DLB[0].DL,3,1));

						//RXDsmlOnGTX = TYLC1(pGTZXArray[i]->DLB[0].BLC + pGTZXArray[i]->DLB[0].DL,NDL,DLB);
						RXDsmlOnGTX = TYLC1(cXLC,NDL,DLB);

						DistToRXDQD = PZ->lc - RXDsmlOnGTX;
						Curcml = pGTZXArray[i]->DLB[0].BLC+pGTZXArray[i]->DLB[0].DL +DistToRXDQD;
						PZ->lc = Curcml;
						int k= BAS_DRAW_FUN_JYX::xlpoint_pz(pGTZXArray[i]->array,pGTZXArray[i]->tarray,PZ);
						if (k>0&& k<=6)  //有解
						{
							return 1 ;
						}
					}
					else
					{
						ads_printf(L"子GTZX_JYX为空2");
						return BAS_DRAW_FUN_JYX::xlpoint_pz(array2,trow2,PZ);
					}
				}
				//////////////////////////////////////////////////////////////////////////

			}

		}

	}
	return 1;
}



//输入贯通线N,E坐标，计算该点的现场里程
CString GTZX_JYX_ROAD::GetGTXXclc(double N,double E)
{

	CString xclc;
	double tylc;
	double Bcml,Ecml;
	ACHAR gh[10],ckml[30];
	int i=0;
	tylc = PROJ_ML(array,tarray,N,E,N,E);
	//2.  判别N,E点所在的区段（利用段或改建段）
	for(i=0; i<BasicSectionInfor.GetSize(); i++)
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
	if(i>=0 && i<BasicSectionInfor.GetSize())//找到第i分段
	{
		// 3.  若为利用段或并行改建段，依据既有线计算该点现场里程
		if(BasicSectionInfor[i].GJDType!=2)
		{

			CString XCLC2x;
			GTZX2_JYX_ROAD *pm2=NULL;
			double Curcml;

			//2.求同一现场里程对应既有线上坐标
			if(_tcscmp( BasicSectionInfor[i].BXLName , L"既有基线")==0) //基线
			{
				Curcml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,N,E,N,E);
				Curcml = JiXianpGtzx->CalnewcmlTocml(Curcml);
				xclc = JiXianpGtzx->XLC(Curcml);
			}
			else  //既有2线
			{
				if(_tcscmp( BasicSectionInfor[i].BXLName,L"既有左线")==0)
					pm2 = ZuoXianpGtzx;
				else
					pm2 = YouXianpGtzx;

				//同一个现场里程，在既有线上求对应坐标
				if(pm2)
				{
					Curcml = pm2->PROJ_ML(pm2->array,pm2->tarray,N,E,N,E);//若iRXD＞0在指定绕行段查找
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
		else// ４．若为绕行改建段，取该绕行改建段GTZX指针计算该点现场里程
		{

			if (IsEXRXD)
			{
				GTZX_JYX_ROAD * pm1=NULL;
				pm1 = Get_GTX_RXGJD_GTZX(BasicSectionInfor[i]);

				//计算子ＧＴＺＸ中ＮＥ坐标
				if(pm1)
				{
					double cml;
					cml = pm1->PROJ_ML(pm1->array,pm1->tarray,N,E,N,E);
					JDarrayStruct JD;
					xclc = pm1->XLC(cml);
					delete pm1;
				}
				else
				{
					cml = PROJ_ML(array,tarray,N,E,N,E);
					xclc = XLC(cml);
				}
			}
			else
			{
				if(pGTZXArray[i])
				{
					double cml;
					cml = pGTZXArray[i]->PROJ_ML(pGTZXArray[i]->array,pGTZXArray[i]->tarray,N,E,N,E);
					JDarrayStruct JD;
					for (int iJD=0 ; iJD<pGTZXArray[i]->NJD ; iJD++)
					{
						JD = pGTZXArray[i]->JDarray[iJD];
					}
					xclc = pGTZXArray[i]->XLC(cml);
				}
				else
				{
					cml = PROJ_ML(array,tarray,N,E,N,E);
					xclc = XLC(cml);
				}
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

//得到子GTZX
GTZX_JYX_ROAD *GTZX_JYX_ROAD::Get_GTX_RXGJD_GTZX(GJDAllInforStruct RXDBasicSectionInfor)
{
	//如果不为绕行,返回null
	if( RXDBasicSectionInfor.GJDType!=2)return NULL ; 

	//根据ＮＥ重新构造子ＧＴＺＸ
	CArray<JDarrayStruct,JDarrayStruct> CurSubJDarray;
	int SNum , ENum; //交点表，断链表 起点／终点下标
	JDarrayStruct jddata;

	jddata.N = RXDBasicSectionInfor.BpointToN;
	jddata.E = RXDBasicSectionInfor.BpointToE;
	jddata.R = 0.0;
	jddata.l1 = 0.0;
	jddata.l2 = 0.0;
	_tcscpy(jddata.JDNum,L"QD");

	//zh ,hz

	double sml,eml;

	//起点
	sml = PROJ_ML(array,tarray,jddata.N ,jddata.E,jddata.N ,jddata.E  );
	eml = PROJ_ML(array,tarray,RXDBasicSectionInfor.EpointToN ,RXDBasicSectionInfor.EpointToE,RXDBasicSectionInfor.EpointToN ,RXDBasicSectionInfor.EpointToE);

	SNum = DistToNearJD(sml,false);
	ENum = DistToNearJD(eml,true);

	CurSubJDarray.RemoveAll();

	CurSubJDarray.Add(jddata);

	double dist;
	if(SNum>=0 )
	{
		dist = sqrt(pow( (jddata.N -JDarray[SNum].N) ,2.0) + pow((jddata.E -JDarray[SNum].E) ,2.0) );
		if(dist<0.001)SNum++ ; 
	}

	if(ENum>=0 )
	{
		dist = sqrt(pow( ( RXDBasicSectionInfor.EpointToN -JDarray[ENum].N) ,2.0) + pow(( RXDBasicSectionInfor.EpointToE -JDarray[ENum].E) ,2.0) );
		if(dist<0.001)ENum-- ; 
	}

	for(int i=SNum;i<=ENum;i++)
	{
		CurSubJDarray.Add(JDarray[i]);
	}


	jddata.N = RXDBasicSectionInfor.EpointToN;
	jddata.E = RXDBasicSectionInfor.EpointToE;
	jddata.R = 0.0;
	jddata.l1 = 0.0;
	jddata.l2 = 0.0;
	_tcscpy(jddata.JDNum,L"ZD");
	CurSubJDarray.Add(jddata);


	int CurSubNjd = CurSubJDarray.GetSize();
	sml = PROJ_ML(array,tarray,CurSubJDarray[0].N ,CurSubJDarray[0].E,CurSubJDarray[0].N ,CurSubJDarray[0].E);
	eml = PROJ_ML(array,tarray,CurSubJDarray[CurSubNjd-1].N ,CurSubJDarray[CurSubNjd-1].E,CurSubJDarray[CurSubNjd-1].N ,CurSubJDarray[CurSubNjd-1].E);

	ACHAR JDGH[8],GH[8];

	CString CurKmlString;


	_tcscpy(JDGH,L"JD");
	_tcscpy(GH,L"AK"); 


	GTZX_JYX_ROAD *pSubLinep= NULL;
	pSubLinep=new GTZX_JYX_ROAD(CurSubJDarray.GetSize(),CurSubJDarray,&GH[0],&JDGH[0]);

	if(!pSubLinep)
		return NULL;


	CArray<DLBZ,DLBZ> CurSubDLBarray;
	CurSubDLBarray.RemoveAll();

	//起点断链

	//输入贯通线N,E坐标，计算该点的现场里程
	CString ckml;
	double Curcml;
	GTZX2_JYX_ROAD *pm2=NULL;
	ACHAR gh[8];
	//2.求同一现场里程对应既有线上坐标
	double Curdml;
	int IsOnRXD=false;
	int irxd=-1;
	GTZX2_JYX_ROAD *expm=NULL;
	bool IsOnJYX = true;
	bool IsOnJYJX = false;
	bool IsOnJYZX = false;
	bool IsOnJYYX = false;

	JiXianpGtzx->JudgePTLorR(RXDBasicSectionInfor.BpointToN, RXDBasicSectionInfor.BpointToE, dist, JiXianpGtzx->array, JiXianpGtzx->tarray);
	if (fabs(dist)<0.001)
		IsOnJYJX = true;

	if(ZuoXianpGtzx)
	{
		ZuoXianpGtzx->JudgePTLorR(RXDBasicSectionInfor.BpointToN, RXDBasicSectionInfor.BpointToE, dist, ZuoXianpGtzx->array, ZuoXianpGtzx->tarray);
		if (fabs(dist)<0.001)
		{
			IsOnJYZX = true;	
			irxd = ZuoXianpGtzx->GetiRXDFromNE(RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE);
			if(irxd>=0)
				expm = ZuoXianpGtzx;
		}
	}

	if(YouXianpGtzx)
	{
		YouXianpGtzx->JudgePTLorR(RXDBasicSectionInfor.BpointToN, RXDBasicSectionInfor.BpointToE, dist, YouXianpGtzx->array, YouXianpGtzx->tarray);
		if (fabs(dist)<0.001)
		{
			IsOnJYYX = true;	
			irxd = YouXianpGtzx->GetiRXDFromNE(RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE);
			if(irxd>=0)
				expm = YouXianpGtzx;
		}
	}

	if (IsOnJYJX==false && IsOnJYZX==false && IsOnJYYX==false)
		IsOnJYX = false;

	if (IsOnJYX)
	{
		if(_tcscmp( RXDBasicSectionInfor.BXLName , L"既有基线")==0 && irxd<0) //基线
		{
			Curcml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE,RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE);
			Curcml = JiXianpGtzx->CalnewcmlTocml(Curcml);
			Curdml = JiXianpGtzx->XLC1(Curcml ,gh,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
		}
		else  //既有左线
		{
			if(_tcscmp( RXDBasicSectionInfor.BXLName , L"既有左线")==0) 
				pm2 = ZuoXianpGtzx;

			if(_tcscmp( RXDBasicSectionInfor.BXLName , L"既有右线")==0) 
				pm2 = YouXianpGtzx;

			if(_tcscmp( RXDBasicSectionInfor.BXLName , L"既有基线")==0 && irxd>=0)
				pm2 = expm;

			//同一个现场里程，在既有线上求对应坐标
			if(pm2)
			{
				Curcml = pm2->PROJ_ML(pm2->array,pm2->tarray,RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE,RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE);//若iRXD＞0在指定绕行段查找

				Curcml = pm2->CalnewcmlTocml(Curcml);
				Curdml = pm2->XCLC(Curcml,gh, pm2->DLB2x, pm2->NDL);
				//加起点断链
			}
			else//采用贯通设计基线计算
			{
				Curcml=PROJ_ML(array,tarray,RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE,RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE);//若iRXD＞0在指定绕行段查找

				//				Curdml = XLC1(Curcml,gh,NDL,DLB);//新XLC1
				Curdml = DmlGTX(Curcml,gh,NDL,DLB);
			}
		}
	}

	else
	{
		Curcml=PROJ_ML(array,tarray,RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE,RXDBasicSectionInfor.BpointToN,RXDBasicSectionInfor.BpointToE);//若iRXD＞0在指定绕行段查找
		//		Curdml = XLC1(Curcml,gh,NDL,DLB);//新XLC1
		Curdml = DmlGTX(Curcml,gh,NDL,DLB);//新XLC1
	}

	DLBZ CurOneDL;
	//	double SDLcml,EDLcml;
	double DLcml;
	//对断链区段进行循环
	for(int i=0;i<NDL;i++)
	{
		//		SDLcml = TYLC1(DLB[i].BLC,NDL,DLB);
		//		EDLcml = TYLC1(DLB[i+1].BLC,NDL,DLB);
		//		SDLcml = PROJ_ML(array,tarray,DLB[i].N ,DLB[i].E,DLB[i].N ,DLB[i].E  );
		//		EDLcml = PROJ_ML(array,tarray,DLB[i+1].N ,DLB[i+1].E,DLB[i+1].N ,DLB[i+1].E);
		DLcml = PROJ_ML(array,tarray,DLB[i].N ,DLB[i].E,DLB[i].N ,DLB[i].E);
		//	DLcml = TYLC1(DLB[i].BLC,NDL,DLB);
		//if(fabs(DLcml-sml)<1)
		//{
		//	CurSubDLBarray[0] = DLB[i];
		//	continue ;

		//}
		//|| fabs(DLcml-eml)<1)continue;

		if (DLcml>sml-10.0 && DLcml<eml+10.0)
		{
			CurSubDLBarray.Add(DLB[i]);
		}

		////断链区间起点下标
		//if(sml>SDLcml-0.0001 && sml<EDLcml+0.0001)SNum = i+1;
		////断链区间终点下标
		//if(eml>SDLcml-0.0001 && eml<EDLcml+0.0001)ENum = i ;
	}



	IsOnJYX = true;
	IsOnJYJX = false;
	IsOnJYZX = false;
	IsOnJYYX = false;

	JiXianpGtzx->JudgePTLorR(RXDBasicSectionInfor.EpointToN, RXDBasicSectionInfor.EpointToE, dist, JiXianpGtzx->array, JiXianpGtzx->tarray);
	if (fabs(dist)<0.001)
		IsOnJYJX = true;

	if(ZuoXianpGtzx)
	{
		ZuoXianpGtzx->JudgePTLorR(RXDBasicSectionInfor.EpointToN, RXDBasicSectionInfor.EpointToE, dist, ZuoXianpGtzx->array, ZuoXianpGtzx->tarray);
		if (fabs(dist)<0.001)
		{
			IsOnJYZX = true;
			irxd = ZuoXianpGtzx->GetiRXDFromNE(RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE);
			if(irxd>=0)
				expm = ZuoXianpGtzx;
		}
	}
	if(YouXianpGtzx)
	{
		YouXianpGtzx->JudgePTLorR(RXDBasicSectionInfor.EpointToN, RXDBasicSectionInfor.EpointToE, dist, YouXianpGtzx->array, YouXianpGtzx->tarray);
		if (fabs(dist)<0.001)
		{
			IsOnJYYX = true;
			irxd = YouXianpGtzx->GetiRXDFromNE(RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE);
			if(irxd>=0)
				expm = YouXianpGtzx;
		}

	}

	if (IsOnJYJX==false && IsOnJYZX==false && IsOnJYYX==false)
		IsOnJYX = false;

	if (IsOnJYX)
	{
		if(_tcscmp( RXDBasicSectionInfor.EXLName , L"既有基线")==0 && irxd<0) //基线
		{
			Curcml = JiXianpGtzx->PROJ_ML(JiXianpGtzx->array,JiXianpGtzx->tarray,RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE,RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE);
			Curdml = JiXianpGtzx->XLC1(Curcml ,gh,JiXianpGtzx->NDL,JiXianpGtzx->DLB);
		}

		else  //既有左线
		{
			if(_tcscmp( RXDBasicSectionInfor.EXLName , L"既有左线")==0)
				pm2 = ZuoXianpGtzx;

			if (_tcscmp( RXDBasicSectionInfor.EXLName , L"既有右线")==0)
				pm2 = YouXianpGtzx;

			if(_tcscmp( RXDBasicSectionInfor.EXLName , L"既有基线")==0 && irxd>=0)
				pm2 = expm;

			//同一个现场里程，在既有线上求对应坐标
			if(pm2)
			{
				Curcml = pm2->PROJ_ML(pm2->array,pm2->tarray,RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE,RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE);//若iRXD＞0在指定绕行段查找
				Curdml = pm2->XCLC(Curcml,gh, pm2->DLB2x, pm2->NDL);
			}
			else
			{
				Curcml = PROJ_ML(array,tarray,RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE,RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE);//若iRXD＞0在指定绕行段查找
				//				Curdml = XLC1(Curcml,gh,NDL,DLB);//新XLC1
				Curdml = DmlGTX(Curcml,gh,NDL,DLB);//新XLC1

			}
		}
	}

	else
	{
		Curcml = PROJ_ML(array,tarray,RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE,RXDBasicSectionInfor.EpointToN,RXDBasicSectionInfor.EpointToE);
		//		Curdml = XLC1(Curcml,gh,NDL,DLB);//新XLC1
		Curdml = DmlGTX(Curcml,gh,NDL,DLB);//新XLC1
	}


	pSubLinep->NDL = CurSubDLBarray.GetSize();
	for(int i=0;i<CurSubDLBarray.GetSize();i++)
	{
		pSubLinep->setDLM(i,CurSubDLBarray[i].BLC,CurSubDLBarray[i].DL,CurSubDLBarray[i].BGH,CurSubDLBarray[i].EGH);
	}

	if (CurSubDLBarray.GetSize()>0)
	{
		pSubLinep->JDarray[0].l1 = CurSubDLBarray[0].BLC + CurSubDLBarray[0].DL;
	}

	pSubLinep->XLShuXing = "贯通设计基线";
	pSubLinep->getpxy_DATA();

	CurSubDLBarray.RemoveAll();

	return pSubLinep;
}
//
////计算贯通设计基线曲线表中现场里程
void GTZX_JYX_ROAD::CalCKQXBToKmlForGTZX()
{

	//返回现场里程
	assertReadEnabled();
	getpxy_DATA();


	ACHAR GHTmp[8];
	double dml = 0.0 ;
	for( int i=0;i<QXB.GetSize();i++)
	{

		//// KZH
		////dml = XLC1(QXB[i].KZH,GHTmp,NDL,DLB);
		////dml = DmlGTX(QXB[i].KZH,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KZH,GHTmp);
		//_tcscpy(QXB[i].CKZH,LCchr(GHTmp,dml,3,1));

		//// KHY
		////dml = XLC1(QXB[i].KHY,GHTmp,NDL,DLB);
		////dml = DmlGTX(QXB[i].KHY,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KHY,GHTmp);
		//_tcscpy(QXB[i].CKHY,LCchr(GHTmp,dml,3,1));

		////KYH
		////dml = XLC1(QXB[i].KYH,GHTmp,NDL,DLB);
		////dml = DmlGTX(QXB[i].KYH,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KYH,GHTmp);
		//_tcscpy(QXB[i].CKYH,LCchr(GHTmp,dml,3,1));


		////KHZ
		////dml = XLC1(QXB[i].KHZ,GHTmp,NDL,DLB);
		////dml = DmlGTX(QXB[i].KHZ,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KHZ,GHTmp);
		//_tcscpy(QXB[i].CKHZ,LCchr(GHTmp,dml,3,1));


		CalXlcByProjml(QXB[i].KZH,QXB[i].CKZH);
		CalXlcByProjml(QXB[i].KHY,QXB[i].CKHY);
		CalXlcByProjml(QXB[i].KYH,QXB[i].CKYH);
		CalXlcByProjml(QXB[i].KHZ,QXB[i].CKHZ);

	}
}


//计算贯通设计基线曲线表中现场里程
void GTZX_JYX_ROAD::CalCKQXBToKmlForGTZX( CArray<QXBStruct,QXBStruct>& CKQXB )
{

	//返回现场里程
	CString LCforPoint;
	double cml;
	xlpoint PZ;

	assertReadEnabled();
	getpxy_DATA();

	CKQXB.RemoveAll();
	CKQXB.SetSize(QXB.GetSize());

	ACHAR GHTmp[8];
	double dml = 0.0 ;
	for( int i=0;i<QXB.GetSize();i++)
	{
		CKQXB[i] = QXB[i];

		_tcscpy( CKQXB[i].XLName , XLShuXing );

		// KZH
		//dml = XLC1(QXB[i].KZH,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KZH,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KZH,GHTmp);
		//_tcscpy(CKQXB[i].CKZH,LCchr(GHTmp,dml,3,1));

		//// KHY
		////dml = XLC1(QXB[i].KHY,GHTmp,NDL,DLB);
		////dml = DmlGTX(QXB[i].KHY,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KHY,GHTmp);
		//_tcscpy(CKQXB[i].CKHY,LCchr(GHTmp,dml,3,1));

		////KYH
		////dml = XLC1(QXB[i].KYH,GHTmp,NDL,DLB);
		////dml = DmlGTX(QXB[i].KYH,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KYH,GHTmp);
		//_tcscpy(CKQXB[i].CKYH,LCchr(GHTmp,dml,3,1));


		////KHZ
		////dml = XLC1(QXB[i].KHZ,GHTmp,NDL,DLB);
		////dml = DmlGTX(QXB[i].KHZ,GHTmp,NDL,DLB);
		//dml = DmlGTX(QXB[i].KHZ,GHTmp);
		//_tcscpy(CKQXB[i].CKHZ,LCchr(GHTmp,dml,3,1));

		CalXlcByProjml(QXB[i].KZH,CKQXB[i].CKZH);
		CalXlcByProjml(QXB[i].KHY,CKQXB[i].CKHY);
		CalXlcByProjml(QXB[i].KYH,CKQXB[i].CKYH);
		CalXlcByProjml(QXB[i].KHZ,CKQXB[i].CKHZ);
	}
}

int GTZX_JYX_ROAD::GetPtGJDType(double N, double E)
{
	if (GJDInforArray.GetSize()==0)//无改建段，
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
		if (ml+0.001>GJDSml && ml-0.001<GJDEml)
		{
			return GJDInforArray[i].GJDType;
		}
	}
	return -1;
}


void  GTZX_JYX_ROAD::AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT) //将桥隧站加入里程表 
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

	while(j<NBTS&&BTS[j][k]<pLCB[0].lc)//起始曲线j,k
	{
		k++;
		if(k==2)
		{j++;k=0;}
	}
	if(j==NBTS)
		return;

	for(i=1; i<NLCB; i++)//对点数进行循环
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
		else
		{
			Lcb[NUM].lc=lc2;
			if(BorT=='B')//桥
			{
				if(k==0)
					Lcb[NUM].lcflag = -3;
				else
					Lcb[NUM].lcflag = 3;
			}
			else if(BorT=='T')//隧
			{
				if(k==0)
					Lcb[NUM].lcflag = -4;
				else
					Lcb[NUM].lcflag = 4;		
			}
			else if(BorT=='S')//站
			{
				if(k==0)
					Lcb[NUM].lcflag =-9;
				else
					Lcb[NUM].lcflag = 9;
			}
			else//绕行段的起终里程
			{
				if(k==0)
				{
					Lcb[NUM].lcflag =-10;
					//ads_printf(L"\nlc=%lf",Lcb[NUM].lc);
				}
				else
					Lcb[NUM].lcflag = 10;
			}
			k++;
			if(k==2)
			{j++;k=0;}
			if(j<NBTS)
				i--;
		}
		if(fabs(Lcb[NUM].lc-Lcb[NUM-1].lc)>0.001) NUM++;		
		else
			Lcb[NUM-1].lcflag = Lcb[NUM].lcflag;
	}

	if (pLCB) delete []pLCB; pLCB = NULL;
	NLCB = NUM;
	pLCB = new LCB[NLCB];
	for(i=0; i<NUM; i++)
	{
		pLCB[i].lc=Lcb[i].lc;
		pLCB[i].lcflag=Lcb[i].lcflag;
	}

	if(Lcb) delete []Lcb;
	Lcb = NULL;
}

//输入设计点上的坐标(N,E)，获取该处的改建段类型，改建段起点和终点处的既有线线路名。
//成功获取返回true失败返回false
bool GTZX_JYX_ROAD::GetPtGJDInf(double dml, int &GJDType, CString &GJDStartJYXLName, CString &GJDEndJYXLName)
{
	if (BasicSectionInfor.GetSize()<=0)
	{
		AfxMessageBox(L"基本分段数组为空，请先创建改建段数组");
		return false;
	}
	double GJDSpt[2],GJDEpt[2];
	double GJDSml,GJDEml;
	for (int i=0 ; i<BasicSectionInfor.GetSize(); i++)
	{
		GJDSpt[0] = BasicSectionInfor[i].BpointToN;
		GJDSpt[1] = BasicSectionInfor[i].BpointToE;
		GJDEpt[0] = BasicSectionInfor[i].EpointToN;
		GJDEpt[1] = BasicSectionInfor[i].EpointToE;
		GJDSml = PROJ_ML(array,tarray,GJDSpt[0],GJDSpt[1],GJDSpt[0],GJDSpt[1]);
		GJDEml = PROJ_ML(array,tarray,GJDEpt[0],GJDEpt[1],GJDEpt[0],GJDEpt[1]);
		if (dml+0.0001>GJDSml && dml-0.0001<GJDEml)
		{
			GJDType = BasicSectionInfor[i].GJDType ;
			GJDStartJYXLName = BasicSectionInfor[i].BXLName ;
			GJDEndJYXLName = BasicSectionInfor[i].EXLName;
			return true ;
		}
	}
	AfxMessageBox(L"改建段基本分段数组中找不到该点的信息！");
	return false;
}


/*
//设置线路颜色
void GTZX::SetXLColor( CString XLNameToIn )
{
int  CurSelColorIndex = 0;

//设置线宽

for(int i=0;i<SetXLColorDlg.m_SetXLColorStructArray.GetSize();i++)
{
if( SetXLColorDlg.m_SetXLColorStructArray[i].m_CurXLName==XLNameToIn )
{
for(int j=0;j<SetXLColorDlg.m_SetXLColorStructArray[i].m_ColorIndexArray.GetSize();j++)
{

CurSelColorIndex = SetXLColorDlg.m_SetXLColorStructArray[i].m_ColorIndexArray[j];

if(j==0)
{
GTZXJDToJDColor = CurSelColorIndex ;

//给其他数据赋予初值->对应全线颜色

XWArcColor = CurSelColorIndex;
HuanHequxianColor = CurSelColorIndex;
JiaZhiXianColorZuo= CurSelColorIndex;
JiaZhiXianColorYou =CurSelColorIndex;
mdbnameBoxColor = CurSelColorIndex;
mdbnameTextColor =CurSelColorIndex;
QXYSColor = CurSelColorIndex;

LineWeightValue = SetXLKuanFormat(SetXLColorDlg.m_SetXLColorStructArray[i].m_XLKuan );
}
else if(j==1) XWArcColor = CurSelColorIndex;
else if(j==2)HuanHequxianColor = CurSelColorIndex;
else if(j==3)JiaZhiXianColorZuo= CurSelColorIndex;
else if(j==4)JiaZhiXianColorYou =CurSelColorIndex;
else if(j==5)mdbnameBoxColor = CurSelColorIndex;
else if(j==6)mdbnameTextColor =CurSelColorIndex;
else QXYSColor = CurSelColorIndex;

}

}

}
}
*/


//设置标准数据
void GTZX_JYX_ROAD::SetStandardXLKuanFormat( CArray< SetLineWeightStruct , SetLineWeightStruct &> & LineWeightArrayToOut )
{
	LineWeightArrayToOut.RemoveAll();
	SetLineWeightStruct OneLineWeightTemp;
	//设置标准数据
	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt000 ;
	OneLineWeightTemp.m_XLKuanInt =  0 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt005 ;
	OneLineWeightTemp.m_XLKuanInt =  5 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt009 ;
	OneLineWeightTemp.m_XLKuanInt =  9 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt013 ;
	OneLineWeightTemp.m_XLKuanInt =  13 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt015 ;
	OneLineWeightTemp.m_XLKuanInt =  15 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt018 ;
	OneLineWeightTemp.m_XLKuanInt =  18 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt020 ;
	OneLineWeightTemp.m_XLKuanInt =  20 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt025 ;
	OneLineWeightTemp.m_XLKuanInt =  25 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt030 ;
	OneLineWeightTemp.m_XLKuanInt =  30 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt035 ;
	OneLineWeightTemp.m_XLKuanInt =  35 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt040 ;
	OneLineWeightTemp.m_XLKuanInt =  40 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);



	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt050 ;
	OneLineWeightTemp.m_XLKuanInt =  50 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt053 ;
	OneLineWeightTemp.m_XLKuanInt =  53 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt060 ;
	OneLineWeightTemp.m_XLKuanInt =  60 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt070 ;
	OneLineWeightTemp.m_XLKuanInt =  70 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt080 ;
	OneLineWeightTemp.m_XLKuanInt =  80 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt090 ;
	OneLineWeightTemp.m_XLKuanInt =  90 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt100 ;
	OneLineWeightTemp.m_XLKuanInt =  100 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt106 ;
	OneLineWeightTemp.m_XLKuanInt =  106 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt120 ;
	OneLineWeightTemp.m_XLKuanInt =  120 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt140 ;
	OneLineWeightTemp.m_XLKuanInt =  140 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);


	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt158 ;
	OneLineWeightTemp.m_XLKuanInt =  158 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt200 ;
	OneLineWeightTemp.m_XLKuanInt =  200 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

	OneLineWeightTemp.m_LineWeight =  AcDb::kLnWt211 ;
	OneLineWeightTemp.m_XLKuanInt =  211 ;
	LineWeightArrayToOut.Add(OneLineWeightTemp);

}




//设置线路格式
AcDb::LineWeight GTZX_JYX_ROAD::SetXLKuanFormat( int XLKuanInt )
{

	CString str;

	CArray< SetLineWeightStruct , SetLineWeightStruct &> LineWeightArray;

	LineWeightArray.RemoveAll();
	//设置标准数据
	SetStandardXLKuanFormat( LineWeightArray );

	if(XLKuanInt<0)
	{
		if(XLKuanInt==-1)return AcDb::kLnWtByLayer;
		if(XLKuanInt==-2)return AcDb::kLnWtByBlock;
		if(XLKuanInt==-3)return AcDb::kLnWtByLwDefault;

		return  AcDb::kLnWt000;
	}
	else
	{
		double DXLKuan1= 0.0 , DXLKuan2 = 0.0 ;
		if(XLKuanInt<=LineWeightArray[0].m_XLKuanInt ) return AcDb::kLnWt000 ;

		if(XLKuanInt>LineWeightArray[LineWeightArray.GetSize()-1].m_XLKuanInt)return AcDb::kLnWt211 ;

		for(int i=1;i<LineWeightArray.GetSize();i++)
		{
			if( XLKuanInt > LineWeightArray[i-1].m_XLKuanInt && XLKuanInt < LineWeightArray[i].m_XLKuanInt)
			{
				DXLKuan1 = XLKuanInt - LineWeightArray[i-1].m_XLKuanInt ;
				DXLKuan2 = LineWeightArray[1].m_XLKuanInt - XLKuanInt ;

				if(DXLKuan1>=DXLKuan2)return LineWeightArray[i].m_LineWeight ;
				else return LineWeightArray[i-1].m_LineWeight ;
			}
		}
	}

	return  AcDb::kLnWt000;

}



//由坐标计算里程
void GTZX_JYX_ROAD::CalNEForKml()
{
	assertWriteEnabled();

	double N = 0.0 ,E = 0.0 , cml = 0.0 ;
	CString kml ;
	for(int i=0;i<GJDInforArray.GetSize();i++)
	{

		kml = GetGTXXclc(GJDInforArray[i].spt[0],GJDInforArray[i].spt[1]);
		_tcscpy( GJDInforArray[i].GJDBLC , kml );
		kml = GetGTXXclc(GJDInforArray[i].ept[0],GJDInforArray[i].ept[1]);
		_tcscpy( GJDInforArray[i].GJDELC , kml );

	}

}

//由里程计算坐标
void GTZX_JYX_ROAD::CalKmlForNE()
{
	assertWriteEnabled();

	ACHAR ckml[80] ;
	xlpoint PZ;
	for(int i=0;i<GJDInforArray.GetSize();i++)
	{
		_tcscpy( ckml , L"\0" ) ;
		_tcscpy( ckml , GJDInforArray[i].GJDBLC ) ;

		PZ.lc = TYLC1(ckml,NDL,DLB);

		xlpoint_pz_GTX(array,tarray,&PZ);

		GJDInforArray[i].spt[0] = PZ.x ; 
		GJDInforArray[i].spt[1] = PZ.y ;

		_tcscpy( ckml , L"\0" ) ;
		_tcscpy( ckml , GJDInforArray[i].GJDELC ) ;

		PZ.lc = TYLC1(ckml,NDL,DLB);

		xlpoint_pz_GTX(array,tarray,&PZ);
		GJDInforArray[i].ept[0] = PZ.x ;
		GJDInforArray[i].ept[1] = PZ.y ;

	}
}

//修正断链处点的现场里程
//传入的cml是由断链计算得出的连续里程
CString GTZX_JYX_ROAD::ModifyXCLCOnDL(double cml)
{
	CString ModifyXLC;//修正后的断链现场里程
	ACHAR TmpXLC[50];//断链处的现场里程
	double DLTYLC;//断链统一里程
	double GJDQDTYLC,GJDZDTYLC;//改建段起终点统一里程
	ModifyXLC = XLC(cml);
	for (int i=0 ; i<NDL ; i++)
	{
		_tcscpy(TmpXLC, LCchr(DLB[i].BGH,DLB[i].BLC,3,1));
		DLTYLC = TYLC1(TmpXLC,NDL,DLB);
		if (fabs(cml-DLTYLC)<0.001)//该连续里程在断链处
		{
			for (int j=0 ; j<GJDInforArray.GetSize() ; j++)
			{
				if (GJDInforArray[j].GJDType==2)
				{
					GJDQDTYLC = PROJ_ML(array,tarray,GJDInforArray[j].spt[0],GJDInforArray[j].spt[1],GJDInforArray[j].spt[0],GJDInforArray[j].spt[1]);
					GJDZDTYLC = PROJ_ML(array,tarray,GJDInforArray[j].ept[0],GJDInforArray[j].ept[1],GJDInforArray[j].ept[0],GJDInforArray[j].ept[1]);
					if (fabs(cml-GJDQDTYLC)<10.0)//cml为断链点，并且在改建段起点
					{
						_tcscpy(TmpXLC , LCchr(DLB[i].BGH,DLB[i].BLC,3,1));
						ModifyXLC = TmpXLC;
						break;
					}
					if (fabs(cml-GJDZDTYLC)<10.0)//cml为断链点，并且在改建段终点
					{
						_tcscpy(TmpXLC , LCchr(DLB[i].EGH,DLB[i].ELC,3,1));
						ModifyXLC = TmpXLC;
						break;
					}
				}
			}
			break ;
		}
	}
	return ModifyXLC;
}

//对于贯通设计基线传入由线元计算出的TYLC，返回现场里程
double GTZX_JYX_ROAD::DmlGTX( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[MAXDLNUM])
{
	double LCB=0,XCLC ;  // LCB 断链的统一里程
	int iDL=0;
	CString strs1,strs2;

	for (iDL=0;iDL<NDL;iDL++)
	{
		//该断链点统一里程
		LCB = PROJ_ML(array,tarray,DLBTMP[iDL].N,DLBTMP[iDL].E,DLBTMP[iDL].N,DLBTMP[iDL].E);

		if (iDL == 0)
		{
			if (TYLC < LCB)
			{
				strs1 = DLBTMP[0].BGH;
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				XCLC = DLBTMP[0].BLC - LCB + TYLC;
				break;
			}
			else if (fabs(TYLC-LCB)<0.00005)
			{
				if (fabs(DLBTMP[iDL].BLC-(int)DLBTMP[iDL].BLC)<0.00005)
				{
					strs1 = DLBTMP[iDL].BGH;
					XCLC = DLBTMP[iDL].BLC;
				}
				else
				{
					strs1 = DLBTMP[iDL].EGH;
					XCLC = DLBTMP[iDL].BLC + DLBTMP[iDL].DL;
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
				XCLC = DLBTMP[iDL].BLC - LCB + TYLC;
				strs1 = DLBTMP[iDL-1].EGH;
				strs2 = DLBTMP[iDL].BGH;
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
					if (DLBTMP[iDL-1].DL < 0.00001 && XCLC < DLBTMP[iDL-1].BLC)//in 前长链
						_tcscpy(GH,strs1);							
					else if  (DLBTMP[iDL].DL < 0.00001 && XCLC < DLBTMP[iDL].BLC)//in 后长链
						_tcscpy(GH,strs2);
					else//前后都不是长链
					{
						if (2.0*(LCB-TYLC) > DLBTMP[iDL].BLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL)
							_tcscpy(GH,strs1);
						else
							_tcscpy(GH,strs2);
					}
				}
				break;
			}
			else if (fabs(TYLC-LCB)<0.00005)
			{
				if (fabs(DLBTMP[iDL].BLC-(int)DLBTMP[iDL].BLC)<0.00005)
				{
					strs1 = DLBTMP[iDL].BGH;
					XCLC = DLBTMP[iDL].BLC;
				}
				else
				{
					strs1 = DLBTMP[iDL].EGH;
					XCLC = DLBTMP[iDL].BLC + DLBTMP[iDL].DL;
				}
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				break;
			}
		}
	}
	if (iDL == NDL)
	{
		XCLC = DLBTMP[iDL-1].BLC + DLBTMP[iDL-1].DL + TYLC - LCB;
		_tcscpy(GH,DLBTMP[iDL-1].EGH);
	}
	return XCLC;
}

double GTZX_JYX_ROAD::DmlGTX(double TYLC,ACHAR *GH)
{
	xlpoint PZtmp;
	PZtmp.lc = TYLC ;
	xlpoint_pz(array,tarray,&PZtmp); 
	CString ckml;
	ACHAR kml[30];
	double dml;
	ckml = GetGTXXclc(PZtmp.x,PZtmp.y);
	_tcscpy(kml,ckml);
	split_ckml(kml,dml,GH);
	return dml;
}


void GTZX_JYX_ROAD::DrawPMCSSZ(AcGiWorldDraw* pWd)
{
	AcGePoint3d Pt,*verts=new AcGePoint3d[NJD];

	AcGeVector3d  normal(0.0,0.0,1.0);
	int i;
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
		pWd->subEntityTraits().setLineWeight(LineWeightValue);
		pWd->geometry().polyline(NJD,verts);
		//	ads_printf(L">>>  NJD=%d\n",NJD);
	} 
	else
	{
		assert(Adesk::kFalse);//?
	}
	// 绘交点处圆
	for (i = 1; i < NJD-1; i++) 
	{
		Pt[X]=verts[i].x; Pt[Y]=verts[i].y;verts[i].z=0.0;
		pWd->subEntityTraits().setColor(GTZXJDToJDColor);	
		pWd->subEntityTraits().setLineWeight(LineWeightValue);
		pWd->geometry().circle(Pt,GTZXJDToJDColor,normal);
	}

	// 交换断链数据
	for (i=0;i<NDL;i++)  
	{
		DLBTMP[i].BLC=DLB[i].BLC; 
		_tcscpy(DLBTMP[i].BGH,DLB[i].BGH); 
		DLBTMP[i].DL=DLB[i].DL;
		_tcscpy(DLBTMP[i].EGH,DLB[i].EGH); 
		//			ads_printf(L"2:%lf %lf\n",DLBTMP[i].BLC,DLBTMP[i].DL);
	}

	delete [] verts;
}

/**********************************************************************************
函数功能：获取贯通设计基线上GJD_Spt到GJD_Ept的改建段信息数组
输入参数：
GJD_Spt[2]		搜索改建段的起点坐标
GJD_Ept[2]		搜索改建段的终点坐标
OneGJDArray		该范围内的改建段数组
**********************************************************************************/
void GTZX_JYX_ROAD::getGJDInfo(double GJD_Spt[2],double GJD_Ept[2],CArray<GJDInf,GJDInf>& OneGJDArray)
{
	double sml,eml;
	double GJDSml,GJDEml;
	xlpoint PZ;
	GJDInf TmpGJD;
	GJDInf TmpGJD2;
	OneGJDArray.RemoveAll();
	sml = PROJ_ML(array,tarray,GJD_Spt[0],GJD_Spt[1],GJD_Spt[0],GJD_Spt[1]);
	eml = PROJ_ML(array,tarray,GJD_Ept[0],GJD_Ept[1],GJD_Ept[0],GJD_Ept[1]);
	if (eml-sml<0.1)
		return;

	//////////////////////////////////////////////////////////////////////////
	//取出贯通设计基线上指定范围内的改建段信息
	for (int i=0; i<GJDInfTab.GetSize(); i++)
	{
		TmpGJD = GJDInfTab[i];
		GJDSml = PROJ_ML(array,tarray,TmpGJD.spt[0],TmpGJD.spt[1],TmpGJD.spt[0],TmpGJD.spt[1]);
		GJDEml = PROJ_ML(array,tarray,TmpGJD.ept[0],TmpGJD.ept[1],TmpGJD.ept[0],TmpGJD.ept[1]);
		if (min(GJDEml,eml) - max(GJDSml,sml) >0.1)
		{
			if (GJDSml<sml)
			{
				TmpGJD.spt[0] = GJD_Spt[0];
				TmpGJD.spt[1] = GJD_Spt[1];
			}
			if (GJDEml>eml)
			{
				TmpGJD.ept[0] = GJD_Ept[0];
				TmpGJD.ept[1] = GJD_Ept[1];
			}
			OneGJDArray.Add(TmpGJD);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//将贯通设计基线两两改建段之间的里程范围定为并行改建

	//没有改建段，整体并行改建
	if (OneGJDArray.GetSize()<1)
	{
		TmpGJD2.spt[0] = GJD_Spt[0];
		TmpGJD2.spt[1] = GJD_Spt[1];
		TmpGJD2.ept[0] = GJD_Ept[0];
		TmpGJD2.ept[1] = GJD_Ept[1];
		TmpGJD2.GJDType = 0;
		SetDefaultGjdName(TmpGJD2.spt[0],TmpGJD2.spt[1],TmpGJD2.GJDType,TmpGJD2.gjdname);
		//		SetDefaultGjdName(TmpGJD2.GJDType,TmpGJD2.gjdname);
		//////////////////////////////////////////////////////////////////////////
		_tcscpy(TmpGJD2.BXLName , L"既有基线");
		_tcscpy(TmpGJD2.EXLName , L"既有基线");
		//////////////////////////////////////////////////////////////////////////
		OneGJDArray.Add(TmpGJD2);

	}

	//有改建段，内插出并行改建段
	for (int j=0; j<OneGJDArray.GetSize(); j++)
	{
		TmpGJD = OneGJDArray[j];
		GJDSml = PROJ_ML(array,tarray,TmpGJD.spt[0],TmpGJD.spt[1],TmpGJD.spt[0],TmpGJD.spt[1]);
		if (GJDSml-sml>0.1)
		{
			TmpGJD2.spt[0] = GJD_Spt[0];
			TmpGJD2.spt[1] = GJD_Spt[1];
			TmpGJD2.ept[0] = TmpGJD.spt[0];
			TmpGJD2.ept[1] = TmpGJD.spt[1];
			TmpGJD2.GJDType = 0;
			SetDefaultGjdName(TmpGJD2.spt[0],TmpGJD2.spt[1],TmpGJD2.GJDType,TmpGJD2.gjdname);
			//			SetDefaultGjdName(TmpGJD2.GJDType,TmpGJD2.gjdname);

			//////////////////////////////////////////////////////////////////////////
			_tcscpy(TmpGJD2.BXLName , TmpGJD.BXLName);
			_tcscpy(TmpGJD2.EXLName , TmpGJD.BXLName);
			//////////////////////////////////////////////////////////////////////////
			OneGJDArray.InsertAt(j+1,TmpGJD2);
			j++;
		}
		sml = PROJ_ML(array,tarray,TmpGJD.ept[0],TmpGJD.ept[1],TmpGJD.ept[0],TmpGJD.ept[1]);
		GJD_Spt[0] = TmpGJD.ept[0];
		GJD_Spt[1] = TmpGJD.ept[1];
	}

	//改建段终点到搜索终点
	GJDSml = PROJ_ML(array,tarray,GJD_Spt[0],GJD_Spt[1],GJD_Spt[0],GJD_Spt[1]);
	if (eml-GJDSml>0.1)
	{
		TmpGJD2.spt[0] = GJD_Spt[0];
		TmpGJD2.spt[1] = GJD_Spt[1];
		TmpGJD2.ept[0] = GJD_Ept[0];
		TmpGJD2.ept[1] = GJD_Ept[1];
		TmpGJD2.GJDType = 0;
		SetDefaultGjdName(TmpGJD2.spt[0],TmpGJD2.spt[1],TmpGJD2.GJDType,TmpGJD2.gjdname);
		//		SetDefaultGjdName(TmpGJD2.GJDType,TmpGJD2.gjdname);
		//////////////////////////////////////////////////////////////////////////
		_tcscpy(TmpGJD2.BXLName , L"既有基线");
		_tcscpy(TmpGJD2.EXLName , L"既有基线");
		//////////////////////////////////////////////////////////////////////////
		OneGJDArray.Add(TmpGJD2);
	}

}

int GTZX_JYX_ROAD::GetPZFromXCLC1X(CString XCLC1x,struct xlpoint& PZ)
{
	bool IsOnXL = true;
	ACHAR ckml[80];
	double cml;
	_tcscpy(ckml,XCLC1x);
	IsOnXL = TYLC1(ckml,NDL,DLB,cml);
	if (!IsOnXL)
	{
		//CString Mes;
		//Mes.Format(L"当前线路上无%s该现场里程！",XCLC1x);
		//AfxMessageBox(Mes);
		return 0;
	}
	if (XLShuXing=="贯通设计基线")
	{
		PZ.lc = cml;
		xlpoint_pz_GTX(array,tarray,&PZ);
		PZ.lc = PROJ_ML(array,tarray,PZ.x,PZ.y,PZ.x,PZ.y);
	}
	else
	{
		PZ.lc = CalcmlTonewcml(cml);
		xlpoint_pz(array,tarray,&PZ);
	}

	return 1;
}

double GTZX_JYX_ROAD::SearchNearDLOnGTX(double ProjCml,bool IsForwardTo,int& DLIndex)
{
	double Dist = 0.0;
	double DLCml=0.0;//断链处统一里程
	int iDL=0;
	double iXLlength=0.0;//某段长度
	for(iDL=0;iDL<NDL;iDL++)
	{
		DLCml = PROJ_ML(array,tarray,DLB[iDL].N,DLB[iDL].E,DLB[iDL].N,DLB[iDL].E);
		if (fabs(ProjCml-DLCml)<0.01)
		{
			DLIndex = iDL;
			Dist = 0.0;
			return Dist;
		}
		if(ProjCml<=DLCml)break;
	}
	if (iDL==NDL)
	{
		DLIndex = -1;
		Dist = 1e6;
		return Dist;
	}
	if(IsForwardTo && iDL>0)//向前搜索
	{
		DLIndex = iDL-1;
		DLCml = PROJ_ML(array,tarray,DLB[iDL-1].N,DLB[iDL-1].E,DLB[iDL-1].N,DLB[iDL-1].E);
		Dist = ProjCml - DLCml;
	}
	else//向后搜索
	{
		DLIndex = iDL;
		Dist = DLCml - ProjCml;
	}
	return Dist;
}

//获取投影里程在Sml-Eml范围内的断链,不包括Sml，Eml
void GTZX_JYX_ROAD::GetJYDLArray(double Sml,double Eml,int& DLSum, DLBZ DLBArray[MAXDLNUM])
{
	DLSum = 0;
	double LCB=DLB[0].BLC + DLB[0].DL;//断链处统一里程
	int iDL=0;
	int StartDL,EndDL;
	double iXLlength=0.0;//某段长度
	xlpoint PZ;
	double DLProjMl;//断链的投影里程
	for(iDL=0;iDL<NDL;iDL++)
	{
		DLProjMl = CalcmlTonewcml(LCB);
		if(DLProjMl>Sml+0.1 && DLProjMl<Eml-0.1)
		{
			PZ.lc = DLProjMl;
			xlpoint_pz(array,tarray,&PZ);
			DLBArray[DLSum] = DLB[iDL];
			DLBArray[DLSum].N = PZ.x;
			DLBArray[DLSum].E = PZ.y;
			DLSum++;
		}
		iXLlength=DLB[iDL+1].BLC-(DLB[iDL].BLC+DLB[iDL].DL);//某段长度
		LCB+=iXLlength;//断链处统一里程
	}
}


void GTZX_JYX_ROAD::GetGTXDLArray(double Sml,double Eml,int& DLSum, DLBZ DLBArray[MAXDLNUM])//获取投影里程在Sml-Eml范围内的断链,不包括Sml，Eml
{
	DLSum = 0;
	int iDL=0;
	double DLProjMl;//断链的投影里程
	for(iDL=0;iDL<NDL;iDL++)
	{
		DLProjMl = PROJ_ML(array,tarray,DLB[iDL].N,DLB[iDL].E,DLB[iDL].N,DLB[iDL].E);
		if(DLProjMl>Sml+0.1 && DLProjMl<Eml-0.1)
		{
			DLBArray[DLSum] = DLB[iDL];
			DLSum++;
		}
	}
}

int GTZX_JYX_ROAD::GetGT1xGJDIndex(double N,double E)
{
	//计算给定点投影到既有线上的里程
	double prjml = PROJ_ML(array,tarray,N,E,N,E);
	//对改建段数组循环
	double sml,eml;

	for(int i=0; i<GJDInforArray.GetSize(); i++)
	{
		sml = PROJ_ML(array,tarray,GJDInforArray[i].spt[0],GJDInforArray[i].spt[1],GJDInforArray[i].spt[0],GJDInforArray[i].spt[1]);
		eml = PROJ_ML(array,tarray,GJDInforArray[i].ept[0],GJDInforArray[i].ept[1],GJDInforArray[i].ept[0],GJDInforArray[i].ept[1]);
		if(prjml>sml-0.001 && prjml<eml+0.001)
			return i;//找到，在第i个改建段
	}
	return -1;
}

int GTZX_JYX_ROAD::JudgePtIsOnDL(double Cml)//传入几何统一里程，判断改点是否在断链上
{
	int iDL = -1;
	double DLProjMl;//断链的投影里程
	if (XLShuXing=="贯通设计基线")
	{
		for(iDL=0;iDL<NDL;iDL++)
		{
			DLProjMl = PROJ_ML(array,tarray,DLB[iDL].N,DLB[iDL].E,DLB[iDL].N,DLB[iDL].E);
			if(DLProjMl-Cml>1.0)
				return -1;
			if(fabs(DLProjMl-Cml)<0.001)
			{
				return iDL;
			}
		}
	}

	else
	{
		double LCB=DLB[0].BLC + DLB[0].DL;//断链处统一里程
		double iXLlength=0.0;//某段长度
		for(iDL=0;iDL<NDL;iDL++)
		{
			DLProjMl = CalcmlTonewcml(LCB);
			if (DLProjMl-Cml>1.0)
				return -1;
			if(fabs(DLProjMl-Cml)<0.001)
			{
				return iDL;
			}
			iXLlength=DLB[iDL].BLC-(DLB[iDL-1].BLC+DLB[iDL-1].DL);//某段长度
			LCB+=iXLlength;//断链处统一里程
		}
	}

	return -1;
}

void GTZX_JYX_ROAD::SetAllColor(int ColorIndex)
{
	AllColor = ColorIndex;
	GTZXJDToJDColor = ColorIndex;
	XWArcColor = ColorIndex;
	HuanHequxianColor = ColorIndex;
	JiaZhiXianColorZuo= ColorIndex;
	JiaZhiXianColorYou =ColorIndex;
	mdbnameBoxColor = ColorIndex;
	mdbnameTextColor =ColorIndex;
	QXYSColor = ColorIndex;
}

void GTZX_JYX_ROAD::CalXlcByProjml(double cml, ACHAR kml[40])
{
	ACHAR GH[8];
	double k,xlc;
	if(XLShuXing!="贯通设计基线")
	{
		double tmpLC = CalnewcmlTocml( cml );

		xlc = XLC(tmpLC,GH,NDL);
		_tcscpy(kml,L"\0");   
		_tcscpy(kml,LCchr(GH,xlc,NLC,1));
	}
	else
	{
		//	lc = CalnewcmlTocml(cml);
		PZtmp.lc = cml ;
		k=xlpoint_pz(array,tarray,&PZtmp); 
		CString ckml;
		ckml = GetGTXXclc(PZtmp.x,PZtmp.y);
		_tcscpy(kml,ckml);
	}
}

//bool GTZX_JYX::TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC)
//{
//	ACHAR GH[8];
//	double dml;
//	if (XLShuXing == "既有基线")
//	{
//		double Projcml = PROJ_ML(array, tarray, N, E, N, E);
//		double cml = CalnewcmlTocml(Projcml);
//		dml = XLC1(cml, GH, NDL, DLB);
//		_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
//	}
//	else
//	{
//	 	CString CkmlStr = GetGTXXclc(N,E);
//		_tcscpy(Ckml, CkmlStr);
//	}
//
//	return true;
//}
//
//bool GTZX_JYX::TrsNEtoProjml(double N, double E, double& Projml)
//{
//	Projml = PROJ_ML(array, tarray, N, E, N, E);
//	return true;
//}
//
//bool GTZX_JYX::TrsNEtoCml(double N, double E, double& Cml)
//{
//	if (XLShuXing == "既有基线")
//	{
//		double ProjCml = PROJ_ML(array, tarray, N, E, N, E);
//		Cml = CalnewcmlTocml(ProjCml);
//	}
//	else
//	{
//		ACHAR Ckml[20];
//		_tcscpy(Ckml, GetGTXXclc(N,E));
//		Cml = TYLC1(Ckml,NDL,DLB);
//	}
//	return true;
//}
//
//bool GTZX_JYX::TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ)
//{
//	if (XLShuXing == "既有基线")
//	{
//		double cml = TYLC1(Ckml, NDL, DLB);
//		double ProjCml = CalcmlTonewcml(cml);
//		PZ.lc = ProjCml;
//		xlpoint_pz(array, tarray, &PZ);
//	}
//	else
//		GetPZFromXCLC1X(Ckml, PZ);
//
//
//	return true;
//}
//
//bool GTZX_JYX::TrsCkmltoProjml(ACHAR Ckml[], double& Projml)
//{
//	if (XLShuXing == "既有基线")
//	{
//		double Cml = TYLC1(Ckml, NDL, DLB);
//		Projml = CalcmlTonewcml(Cml);
//	}
//	else
//	{
//		xlpoint PZ;
//		GetPZFromXCLC1X(Ckml, PZ);
//		Projml = PROJ_ML(array, tarray, PZ.x, PZ.y, PZ.x, PZ.y);
//	}
//
//	return true;
//}
//
//
//bool GTZX_JYX::TrsCkmltoCml(ACHAR Ckml[], double& Cml)
//{
//	Cml = TYLC1(Ckml, NDL, DLB);
//	return true;
//}
//
//bool GTZX_JYX::TrsCmltoNE(double Cml, xlpoint& PZ)
//{
//	if (XLShuXing == "既有基线")
//	{
//		double Projml = CalcmlTonewcml(Cml);
//		PZ.lc = Projml;
//		xlpoint_pz(array, tarray, &PZ);
//	}
//	else
//	{
//		PZ.lc = Cml;
//		xlpoint_pz_GTX(array, tarray, &PZ);
//	}
//
//
//	return true;
//}
//
//bool GTZX_JYX::TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC)
//{
//	ACHAR GH[8];
//	double dml = 0.0;
//	dml = XLC1(Cml, GH, NDL, DLB);
//	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
//
//	return true;
//}
//
//bool GTZX_JYX::TrsCmltoProjml(double Cml, double& Projml)
//{
//	if (XLShuXing == "既有线")
//	{
//		Projml = CalcmlTonewcml(Cml);
//	}
//	else
//	{
//		xlpoint PZ;
//		PZ.lc = Cml;
//		xlpoint_pz_GTX(array, tarray, &PZ);
//		Projml = PROJ_ML(array, tarray, PZ.x, PZ.y, PZ.x, PZ.y);
//	}
//	
//
//	return true;
//}
//
//bool GTZX_JYX::TrsProjmltoNE(double Projml, xlpoint& PZ)
//{
//	PZ.lc = Projml;
//	xlpoint_pz(array, tarray, &PZ);
//
//	return true;
//}
//
//bool GTZX_JYX::TrsProjmltoCml(double Projml, double& Cml)
//{
//	if (XLShuXing == "既有基线")
//	{
//		Cml = CalnewcmlTocml(Projml);
//	}
//	else
//	{
//		xlpoint PZ;
//		ACHAR Ckml[20];
//		PZ.lc = Projml;
//		xlpoint_pz(array, tarray, &PZ);
//		_tcscpy(Ckml, GetGTXXclc(PZ.x, PZ.y));
//		Cml = TYLC1(Ckml, NDL, DLB);
//	}
//	
//
//	return true;
//}
//
//bool GTZX_JYX::TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC)
//{
//	if (XLShuXing == "既有基线")
//	{
//		ACHAR GH[8];
//		double dml = 0.0;
//		double Cml = CalnewcmlTocml(Projml);
//		dml = XLC1(Cml, GH, NDL, DLB);
//		_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
//	}
//	else
//	{
//		xlpoint PZ;
//		PZ.lc = Projml;
//		xlpoint_pz(array, tarray, &PZ);
//		_tcscpy(Ckml, GetGTXXclc(PZ.x, PZ.y));
//	}
//
//	return true;
//}

bool GTZX_JYX_ROAD::TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml;
	if (XLShuXing == "既有基线")
	{
		double Projcml = PROJ_ML(array, tarray, N, E, N, E);
		double cml = CalnewcmlTocml(Projcml);
		dml = XLC1(cml, GH, NDL, DLB);
		_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
	}
	else
	{
		CString CkmlStr = GetGTXXclc(N,E);
		_tcscpy(Ckml, CkmlStr);
	}

	return true;
}

bool GTZX_JYX_ROAD::TrsNEtoProjml(double N, double E, double& Projml)
{
	Projml = PROJ_ML(array, tarray, N, E, N, E);
	return true;
}

bool GTZX_JYX_ROAD::TrsNEtoCml(double N, double E, double& Cml)
{
	if (XLShuXing == "既有基线")
	{
		double ProjCml = PROJ_ML(array, tarray, N, E, N, E);
		Cml = CalnewcmlTocml(ProjCml);
	}
	else
	{
		ACHAR Ckml[20];
		_tcscpy(Ckml, GetGTXXclc(N,E));
		Cml = TYLC1(Ckml,NDL,DLB);
	}
	return true;
}

bool GTZX_JYX_ROAD::TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ)
{
	if (XLShuXing == "既有基线")
	{
		double cml = TYLC1(Ckml, NDL, DLB);
		double ProjCml = CalcmlTonewcml(cml);
		PZ.lc = ProjCml;
		xlpoint_pz(array, tarray, &PZ);
	}
	else
		GetPZFromXCLC1X(Ckml, PZ);


	return true;
}

bool GTZX_JYX_ROAD::TrsCkmltoProjml(ACHAR Ckml[], double& Projml)
{
	if (XLShuXing == "既有基线")
	{
		double Cml = TYLC1(Ckml, NDL, DLB);
		Projml = CalcmlTonewcml(Cml);
	}
	else
	{
		xlpoint PZ;
		GetPZFromXCLC1X(Ckml, PZ);
		Projml = PROJ_ML(array, tarray, PZ.x, PZ.y, PZ.x, PZ.y);
	}

	return true;
}


bool GTZX_JYX_ROAD::TrsCkmltoCml(ACHAR Ckml[], double& Cml)
{
	Cml = TYLC1(Ckml, NDL, DLB);
	return true;
}

bool GTZX_JYX_ROAD::TrsCmltoNE(double Cml, xlpoint& PZ)
{
	if (XLShuXing == "既有基线")
	{
		double Projml = CalcmlTonewcml(Cml);
		PZ.lc = Projml;
		xlpoint_pz(array, tarray, &PZ);
	}
	else
	{
		PZ.lc = Cml;
		xlpoint_pz_GTX(array, tarray, &PZ);
	}


	return true;
}

bool GTZX_JYX_ROAD::TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC)
{
	ACHAR GH[8];
	double dml = 0.0;
	dml = XLC1(Cml, GH, NDL, DLB);
	_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));

	return true;
}

bool GTZX_JYX_ROAD::TrsCmltoProjml(double Cml, double& Projml)
{
	if (XLShuXing == "既有线")
	{
		Projml = CalcmlTonewcml(Cml);
	}
	else
	{
		xlpoint PZ;
		PZ.lc = Cml;
		xlpoint_pz_GTX(array, tarray, &PZ);
		Projml = PROJ_ML(array, tarray, PZ.x, PZ.y, PZ.x, PZ.y);
	}


	return true;
}

bool GTZX_JYX_ROAD::TrsProjmltoNE(double Projml, xlpoint& PZ)
{
	PZ.lc = Projml;
	xlpoint_pz(array, tarray, &PZ);

	return true;
}

bool GTZX_JYX_ROAD::TrsProjmltoCml(double Projml, double& Cml)
{
	if (XLShuXing == "既有基线")
	{
		Cml = CalnewcmlTocml(Projml);
	}
	else
	{
		xlpoint PZ;
		ACHAR Ckml[20];
		PZ.lc = Projml;
		xlpoint_pz(array, tarray, &PZ);
		_tcscpy(Ckml, GetGTXXclc(PZ.x, PZ.y));
		Cml = TYLC1(Ckml, NDL, DLB);
	}


	return true;
}

bool GTZX_JYX_ROAD::TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC)
{
	if (XLShuXing == "既有基线")
	{
		ACHAR GH[8];
		double dml = 0.0;
		double Cml = CalnewcmlTocml(Projml);
		dml = XLC1(Cml, GH, NDL, DLB);
		_tcscpy(Ckml, LCchr(GH, dml, NLC, 1));
	}
	else
	{
		xlpoint PZ;
		PZ.lc = Projml;
		xlpoint_pz(array, tarray, &PZ);
		_tcscpy(Ckml, GetGTXXclc(PZ.x, PZ.y));
	}

	return true;
}

void GTZX_JYX_ROAD::SetBPDArray(int NPD, PDTAB* BPDArr, bool IsAdd)
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

void GTZX_JYX_ROAD::TrsBPDArraytoCM()
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
		BPD_array[i][0] = m_BPDArray[i].cml;//连续里程
		BPD_array[i][1] = m_BPDArray[i].Level;	//主线坡度			
		BPD_array[i][2] = m_BPDArray[i].Rshu;//竖曲线半径
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

//计算cml处的高程,坡度
double GTZX_JYX_ROAD::rdes_level(double cml,double &slope)
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
	sml = m_CM[0];//起始连续里程
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
				slope = m_CM[(j)*5+2];
				if(m_CM[(j-1)*5+4]<0.0)//抛物线
				{
					t1 = -1.0*m_CM[(j-1)*5+4];
					if(cml < m_CM[(j-1)*5]+t1)//处于竖曲线范围内
					{
						z = m_CM[(j-1)*5+1]-m_CM[(j-1)*5+2]*t1;//竖曲线起点高程
						C = z;
						delta_i = m_CM[j*5+2]-m_CM[(j-1)*5+2];
						A = delta_i/4.0/t1;//竖曲线坡度变化率
						z2 = m_CM[(j-1)*5+1]+m_CM[j*5+2]*t1;//竖曲线终点高程
						B = ((z2-z)-t1*delta_i)/2.0/t1;
						x = cml - m_CM[(j-1)*5] + t1;
						z = A*x*x + B*x + C;
						break;
					}
				}
				if(m_CM[j*5+4]<0.0)//抛物线
				{
					t1 = -1.0*m_CM[j*5+4];
					if(cml > m_CM[j*5]-t1)//处于竖曲线范围内
					{
						z = m_CM[j*5+1]-m_CM[j*5+2]*t1;//竖曲线起点高程
						C = z;
						delta_i = m_CM[(j+1)*5+2]-m_CM[j*5+2];
						A = delta_i/4.0/t1;//竖曲线坡度变化率
						z2 = m_CM[j*5+1]+m_CM[(j+1)*5+2]*t1;//竖曲线终点高程
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
//由CM数组计算cml处的高程
double GTZX_JYX_ROAD::DLEVEL(int icm, double cml)
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

bool GTZX_JYX_ROAD::BZCM(AcGiWorldDraw* pWd)
{
	xlpoint pt;
	AcGePoint3d cenPT,BefPt,AftPt,InterPt,InterPt2,BefSlopePt,AftSlopePt,MLPt;
	ACHAR Ckml[20];
	ACHAR BefLen[10];
	ACHAR AftLen[10];
	ACHAR BefSlope[10];
	ACHAR AftSlope[10];
	ACHAR Lever[10];
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

		TrsCmltoNE(m_CM[i*5], pt);
		TrsCmltoCkml(m_CM[i*5], Ckml, 3);
		_stprintf(Lever,L"%0.3lf",m_CM[i*5+1]);
		_stprintf(BefLen,L"%0.3lf",m_CM[i*5+3]);		
		_stprintf(AftLen,L"%0.3lf",m_CM[(i+1)*5+3]);
		_stprintf(BefSlope,L"%0.2lf‰",1000*m_CM[i*5+2]);
		_stprintf(AftSlope,L"%0.2lf‰",1000*m_CM[(i+1)*5+2]);
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
				G_maketext(pWd, BefSlopePt, BefLen, BefSlopeAng, texth, Color,3);
			}
			if (i<m_NCM-2)
			{
				G_maketext(pWd, AftSlopePt, AftSlope, AftSlopeAng, texth, Color, 1);
				G_maketext(pWd, AftSlopePt, AftLen, AftSlopeAng, texth, Color,3);
			}

		}
		BZSHUQX(pWd,i,LorR);
	}
	return true;

}
void GTZX_JYX_ROAD::BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR)
{
	xlpoint pt;
	double YL = 10;
	double XL = 5;
	double LC;
	int Color = 0;
	AcGePoint3d cenPT,BefPt,AftPt,InterPt;
	AcGePoint3d  TBefPt,TAftPt;
	if (m_BPDArray[num].RLen<0.001)
	{
		return;
	}
	LC=m_CM[num*5]-m_BPDArray[num].RLen*0.5;
	if ((num == 0) && (LC < StartCml))//起点
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

void GTZX_JYX_ROAD::Invert()
{

	CArray<BAS_DRAW_FUN_JYX::JDarrayStruct,BAS_DRAW_FUN_JYX::JDarrayStruct>JDarrayTemp;

	JDarrayTemp.Append(JDarray);

	double lTmp = JDarrayTemp[0].l1 ;

	JDarrayTemp[0].l1 = 0 ;
	JDarrayTemp[JDarrayTemp.GetSize()-1].l1 = lTmp ;

	JDarray.RemoveAll();

	int i;
	for( i=JDarrayTemp.GetSize()-1;i>= 0;i--)
	{
		JDarray.Add(JDarrayTemp[i]);
	}
	double ENDCML;
	PDTAB myPDTAB;
	int sum=m_BPDArray.GetSize()-1;
	//AcGePoint3dArray mypointarray;
	CArray<BAS_DRAW_FUN_JYX::xlpoint,BAS_DRAW_FUN_JYX::xlpoint> mypointarray;
	BAS_DRAW_FUN_JYX::xlpoint mypt;
	for (i=0;i<m_BPDArray.GetSize();i++)
	{
		TrsCmltoNE(m_BPDArray[i].cml,mypt);

		mypointarray.Add(mypt);
		//m_BPDArray[i].cml=ENDCML-m_BPDArray[i].cml;
		//TrsCmltoCkml(m_BPDArray[i].cml,m_BPDArray[i].CKml,3);
		//ACHAR myGH[16];
		//split_ckml(m_BPDArray[i].CKml,&m_BPDArray[i].ml,myGH);
		//m_BPDArray[i].GH.Format(L"%s",myGH);
		//myPDTAB=m_BPDArray[sum-i];
		//m_BPDArray[sum-i]=m_BPDArray[i];
		//m_BPDArray[i]=myPDTAB;
	}
	getpxy_DATA();
	for (i=0;i<m_BPDArray.GetSize();i++)
	{
		TrsNEtoCml(mypointarray[i].x,mypointarray[i].y,m_BPDArray[i].cml);
		TrsCmltoCkml(m_BPDArray[i].cml,m_BPDArray[i].CKml,3);
		ACHAR myGH[16];
		split_ckml(m_BPDArray[i].CKml,&m_BPDArray[i].ml,myGH);
		m_BPDArray[i].GH.Format(L"%s",myGH);
		myPDTAB=m_BPDArray[sum-i];
		m_BPDArray[sum-i]=m_BPDArray[i];
		m_BPDArray[i]=myPDTAB;
	}

	double cmlTmp =PROJ_ML(array,tarray,m_XLNameNotePt.y,m_XLNameNotePt.x,m_XLNameNotePt.y,m_XLNameNotePt.x);

	BAS_DRAW_FUN_JYX::xlpoint PZTmp ;
	PZTmp.lc = cmlTmp ;
	xlpoint_pz(array,tarray,&PZTmp);

	double dist = xl_length / 8.0 ;


	m_XLNameNotePt.x = PZTmp.y  + dist * sin(PZTmp.a+PI/2.0 ) ;

	m_XLNameNotePt.y = PZTmp.x + dist * cos(PZTmp.a +PI/2.0 ) ;
}

void GTZX_JYX_ROAD::ReadXLGuifan()
{
	CString ProName =  GetProByMdbName(mdbname);
	//	ReadGuifanFile(ProName, m_GuiFanCS);
	DV = m_GuiFanCS.SuDuMuBiao;
}