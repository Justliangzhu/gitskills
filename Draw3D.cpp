// Draw3D.cpp: implementation of the CDraw3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Draw3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern struct Triangle{
	int vertex[3];
	//    char Attribute;//属性：代表三角形是否有效,'Y'有效;'N'无效
}  *TriList; 
extern struct PT{
	float pt[3];
} *ptdata;

extern double LeftLowPt[2];
extern int PTSUM,TriSum;

CDraw3D::CDraw3D()
{
	NZLMK = 0;
	NYLMK = 0;
	ZLMK = NULL;
	YLMK = NULL;
	hdmdes = NULL;
	pm=NULL;
}

CDraw3D::~CDraw3D()
{
	NZLMK = 0;
	NYLMK = 0;
	ZLMK = NULL;
	YLMK = NULL;
	hdmdes = NULL;
	pm=NULL;
}

double CDraw3D::Create3DModel()
{
	int i;

	ReadHdmResultFile();

	draw_fill_cut();


	for(i=0; i<DBS.BriNum; i++)
	{
		GetCmlKKSY(i);
		draw_bridge_base(DBS.Bri[i].HeadAng,DBS.Bri[i].XZ); 
	}

	draw_BrigeSlop();

	return 1;
}

void CDraw3D::GetCmlKKSY(int ibri)
{
	int len,j,k;  //BrgArray 0对应桥尾梁 KNum-1对应桥桥头梁

	ACHAR str1[256],brg[180],brg1[90];
	CString ks,kc;

	_tcscpy(str1,DBS.Bri[ibri].KKSY);
	KNum = 0;
	int pos=DBS.Bri[ibri].KKSY.FindOneOf(L"-");

	if(pos>0)
	{
		ks = DBS.Bri[ibri].KKSY.Left(pos);
		len = DBS.Bri[ibri].KKSY.GetLength();
		kc =  DBS.Bri[ibri].KKSY.Right(len-pos-1);
		BrgArray[KNum].klen = _wtof(kc);
		BrgArray[KNum].ks = _wtoi(ks);
		KNum++;
	}
	else
	{
		while(len=F_num(str1,'+'))
		{
			for(j=len,k=0; j<_tcslen(str1);k++, j++)
				brg[k] = str1[j];  
			str1[len-1] ='\0' ;
			brg[k] = '\0';

			len = F_num(brg,'x');
			for(k=0;k<len;k++)
				brg1[k] = brg[k];
			brg1[k]='\0';
			BrgArray[KNum].ks = _wtoi(brg1);
			for(j=len,k=0; j<_tcslen(brg);k++, j++)
				brg1[k] = brg[j];  	
			brg1[k]='\0';
			BrgArray[KNum].klen = _wtof(brg1);
			KNum++;
		}

		len = F_num(str1,'x');
		for(k=0;k<len;k++)
			brg1[k] = str1[k];
		brg1[k]='\0';
		BrgArray[KNum].ks = _wtoi(brg1);
		for(j=len,k=0; j<_tcslen(str1);k++, j++)
			brg1[k] = str1[j];  	
		brg1[k]='\0';
		BrgArray[KNum].klen = _wtof(brg1);
		KNum++;
	}

	nBriL = 0; 
	double sml;
	sml = DBS.Bri[ibri].Scml;

	for(j=KNum-1;j>=0;j--)
	{
		for(k=0;k<BrgArray[j].ks;k++)
		{
			BrgLArr[nBriL].sml = sml;
			sml = sml + BrgArray[j].klen;
			nBriL++;
		}
	}
}

//内插 设计高 左右路面宽
void CDraw3D::CalDeshLmk(double cml, double &zmh, double &ymh,double &Zwd, double &Ywd)
{
	int i;

	zmh=ymh=Zwd=Ywd=0.0;
	for(i=0; i<NCRS; i++)
		if(hdmdes[i].cml > cml)
			break;
	if(i>0&& i<NCRS)
	{
		zmh = hdmdes[i-1].RHFL[1]+ (cml-hdmdes[i-1].cml)*(hdmdes[i].RHFL[1]-hdmdes[i-1].RHFL[1])/(hdmdes[i].cml-hdmdes[i-1].cml);
		ymh = hdmdes[i-1].RHFR[1]+ (cml-hdmdes[i-1].cml)*(hdmdes[i].RHFR[1]-hdmdes[i-1].RHFR[1])/(hdmdes[i].cml-hdmdes[i-1].cml);
		Zwd = hdmdes[i-1].RHFL[8]+ (cml-hdmdes[i-1].cml)*(hdmdes[i].RHFL[8]-hdmdes[i-1].RHFL[8])/(hdmdes[i].cml-hdmdes[i-1].cml);
		Ywd = hdmdes[i-1].RHFR[8]+ (cml-hdmdes[i-1].cml)*(hdmdes[i].RHFR[8]-hdmdes[i-1].RHFR[8])/(hdmdes[i].cml-hdmdes[i-1].cml);
	}
}

//ang 桥梁交角 一般为正交 90
void CDraw3D::draw_bridge_base(double ang,CString XZ)
{
	int j;
	double mh,zmh,ymh,Zwd,Ywd,D,H,zfwj,yfwj;
	AcGePoint3d Lpt,Rpt,BriMP,CenPt;

	xlpoint PZtmp;
	xlpoint PZLJZX;
	double OffsetByXLZX = 0.0;
	for(j=1; j< nBriL; j++)
	{
		CalDeshLmk(BrgLArr[j].sml,zmh,ymh,Zwd,Ywd); //中心设计高	左路面宽 右路面宽 
		PZtmp.lc = BrgLArr[j].sml;
		pm->xlpoint_pz(&PZtmp);
		CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
		PZLJZX = PZtmp;
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.x+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.x+0.5*pi);
		// 	   PZtmp.x = PZtmp.x - LeftLowPt[0];
		// 	   PZtmp.y = PZtmp.y - LeftLowPt[1];	  
		PZtmp.x = PZLJZX.x - LeftLowPt[0];
		PZtmp.y = PZLJZX.y - LeftLowPt[1];
		Zwd =  Zwd/sin(ang*PI/180);
		Ywd =  Ywd/sin(ang*PI/180);
		zfwj = PZtmp.a - (180-ang)*PI/180.0;	
		yfwj = PZtmp.a + ang*PI/180.0;	
		mh = zmh;
		if(XZ=="左幅")
			Ywd = 0.0;
		else if(XZ=="右幅")
		{
			Zwd = 0.0;
			mh = ymh;
		}
		Lpt.x = PZtmp.x + Zwd * cos(zfwj);
		Lpt.y = PZtmp.y + Zwd * sin(zfwj);
		Lpt.z = mh;	

		Rpt.x = 	PZtmp.x + Ywd * cos(yfwj);
		Rpt.y = 	PZtmp.y + Ywd * sin(yfwj);	   	
		Rpt.z = mh;

		if(Zwd + Ywd < 20.0 || XZ!="全幅")//单幅双墩
		{
			BriMP.x = Lpt.x + 0.5*(Zwd+Ywd) * cos(yfwj);
			BriMP.y = Lpt.y + 0.5*(Zwd+Ywd) * sin(yfwj);

			CenPt.y = BriMP.x + (3.0+0.75)*cos(zfwj);
			CenPt.x = BriMP.y + (3.0+0.75)*sin(zfwj);
			CenPt.z = mh - 2.7;
			D = 1.5;
			H = 100.0;
			draw_circleBase(CenPt,D,H);           
			CenPt.y = BriMP.x + (3.0+0.75)*cos(yfwj);
			CenPt.x = BriMP.y + (3.0+0.75)*sin(yfwj);
			draw_circleBase(CenPt,D,H);           				
		}
		else
		{
			BriMP.x = PZtmp.x + 0.5*Zwd * cos(zfwj);
			BriMP.y = PZtmp.y + 0.5*Zwd * sin(zfwj);

			CenPt.y = BriMP.x + (3.0+0.75)*cos(zfwj);
			CenPt.x = BriMP.y + (3.0+0.75)*sin(zfwj);
			CenPt.z = mh - 2.7;
			D = 1.5;
			H = 100.0;
			draw_circleBase(CenPt,D,H);           
			CenPt.y = BriMP.x + (3.0+0.75)*cos(yfwj);
			CenPt.x = BriMP.y + (3.0+0.75)*sin(yfwj);
			draw_circleBase(CenPt,D,H);           

			BriMP.x = PZtmp.x + 0.5*Ywd * cos(yfwj);
			BriMP.y = PZtmp.y + 0.5*Ywd * sin(yfwj);

			CenPt.y = BriMP.x + (3.0+0.75)*cos(zfwj);
			CenPt.x = BriMP.y + (3.0+0.75)*sin(zfwj);
			CenPt.z = mh - 2.7;
			D = 1.5;
			H = 100.0;
			draw_circleBase(CenPt,D,H);           
			CenPt.y = BriMP.x + (3.0+0.75)*cos(yfwj);
			CenPt.x = BriMP.y + (3.0+0.75)*sin(yfwj);
			draw_circleBase(CenPt,D,H);                           
		}
		draw_BaseCape(Lpt,Rpt,PZtmp.a);           			
	}
}
//读hdmresult文件
void CDraw3D::ReadHdmResultFile()
{
	CString filename;
	FILE *fpr;

	filename =Cworkdir + "\\data\\" + "hdmresult." + Pro + "." + pm->RoadName;
	if((fpr=_wfopen(filename , L"r"))==NULL)
	{
		ads_alert(L"无法打开hdmresult文件!");
		return ;
	}

	fwscanf(fpr,L"%d",&NCRS);
	NCRS = fabs(1.0*NCRS);
	//	ads_printf(L"Ncrs=%d\n",NCRS);
	//开空间
	if(NCRS>0)
	{
		if(hdmdes) delete []hdmdes;
		hdmdes = NULL;
		hdmdes = new CRSDAT[NCRS];
	}
	else
	{
		fclose(fpr);
		return;
	}

	if(_tcscmp(pm->RoadName,L"主线")!=0)//分离段
	{
		fwscanf(fpr,L"%lf%lf%lf%lf%lf%lf",
			&SmlOnZx,&BPFLSmlOnZx,&BPFLSmlOnFLD,
			&EmlOnZx,&BPFLEmlOnZx,&BPFLEmlOnFLD);
	}
	for(int j=0; j<NCRS; j++)
		ReadACrossResultData(fpr,j);	
	fclose(fpr);

	int jcross;
	for(jcross=0; jcross<NCRS; jcross++)
	{
		if(hdmdes[jcross].ZDMXZ==3)//桥梁断面
		{
			CreateBriPt(jcross,3,hdmdes[jcross].PTRPTL,-1);
			hdmdes[jcross].Lnpt = hdmdes[jcross].PTRPTL.length();
		}
		if(hdmdes[jcross].YDMXZ==3)
		{
			CreateBriPt(jcross,3,hdmdes[jcross].PTRPTR,1);
			hdmdes[jcross].Rnpt = hdmdes[jcross].PTRPTR.length();			
		}
		if(hdmdes[jcross].ZDMXZ==4)//隧道断面
		{			
			CreateTunPt(jcross,hdmdes[jcross].PTRPTL,-1);
			hdmdes[jcross].Lnpt = hdmdes[jcross].PTRPTL.length();
		}
		if(hdmdes[jcross].YDMXZ==4)
		{
			CreateTunPt(jcross,hdmdes[jcross].PTRPTR,1);
			hdmdes[jcross].Rnpt = hdmdes[jcross].PTRPTR.length();
		}
	}

	DBS.Read_XLDbs(pm->mdbname,L"大中桥",pm->RoadName);	

	for(int i=0;i<DBS.BriNum;i++)
	{
		DBS.Bri[i].Scml = pm->TYLC(DBS.Bri[i].StDml);//起点里程
		DBS.Bri[i].Ecml = pm->TYLC(DBS.Bri[i].EdDml);//起点里程
	}

	return;
}

//增加梁身7点  单侧桥 单侧路基 
void CDraw3D::AddSideBriBodyPt(AcGePoint3dArray &Ptarr,int LorR,xlpoint &PZtmp)
{
	AcGePoint3d pt;
	double dx,dy,x;

	dx = Ptarr[4].x - Ptarr[0].x;
	dy = Ptarr[4].y - Ptarr[0].y;
	x = sqrt(dx*dx+dy*dy);
	if(LorR==-1)
	{
		pt.x = Ptarr[4].x;
		pt.y = Ptarr[4].y;
		pt.z = Ptarr[4].z-0.2;
		Ptarr.append(pt);
		pt.y = PZtmp.x + (x-1) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + (x-1) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z - 0.5;
		Ptarr.append(pt);
		pt.x = Ptarr[6].x;
		pt.y = Ptarr[6].y;
		pt.z = Ptarr[6].z-2.0;
		Ptarr.append(pt);
		pt.y = PZtmp.x + 1.0 * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + 1.0 * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[7].z;
		Ptarr.append(pt);
		pt.x = Ptarr[8].x;
		pt.y = Ptarr[8].y;
		pt.z = Ptarr[8].z+2.0;
		Ptarr.append(pt);
		pt.x = Ptarr[0].x;
		pt.y = Ptarr[0].y;
		pt.z = Ptarr[9].z+0.5;
		Ptarr.append(pt);
		pt.x = Ptarr[0].x;
		pt.y = Ptarr[0].y;
		pt.z = Ptarr[0].z;
		Ptarr.append(pt);		
	}
	else
	{
		pt.x = Ptarr[4].x;
		pt.y = Ptarr[4].y;
		pt.z = Ptarr[4].z-0.2;
		Ptarr.append(pt);
		pt.y = PZtmp.x + (x-1) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x-1) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z - 0.5;
		Ptarr.append(pt);
		pt.x = Ptarr[6].x;
		pt.y = Ptarr[6].y;
		pt.z = Ptarr[6].z-2.0;
		Ptarr.append(pt);
		pt.y = PZtmp.x + 1.0 * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + 1.0 * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[7].z;
		Ptarr.append(pt);
		pt.x = Ptarr[8].x;
		pt.y = Ptarr[8].y;
		pt.z = Ptarr[8].z+2.0;
		Ptarr.append(pt);
		pt.x = Ptarr[0].x;
		pt.y = Ptarr[0].y;
		pt.z = Ptarr[9].z+0.5;
		Ptarr.append(pt);
		pt.x = Ptarr[0].x;
		pt.y = Ptarr[0].y;
		pt.z = Ptarr[0].z;
		Ptarr.append(pt);		
	}
}

//增加隧道身  两侧性质都为隧道 x =隧面宽度的一半 PZTtmp 隧道中心位置
void CDraw3D::AddSinTunBodyPt(AcGePoint3dArray &Ptarr,int LorR, xlpoint &PZtmp)
{
	AcGePoint3d pt;
	double 	tunwide= 5.40;//隧道半径

	if(LorR==-1)
	{
		pt.y = PZtmp.x + 4.11 * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + 4.11 * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[0].z;
		Ptarr.append(pt);
		pt.x = Ptarr[5].x;
		pt.y = Ptarr[5].y;
		pt.z = Ptarr[5].z+0.25;
		Ptarr.append(pt);
		pt.y = PZtmp.x + 5.2 * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + 5.2 * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[6].z;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + 5.4 * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + 5.4 * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.5;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(PI/10) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(PI/10) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.5+tunwide*sin(PI/10);	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(PI/5) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(PI/5) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.5+tunwide*sin(PI/5);	
		Ptarr.append(pt);		
		pt.y = PZtmp.x + tunwide*cos(3*PI/10) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(3*PI/10) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.5+tunwide*sin(3*PI/10);	
		Ptarr.append(pt);	
		pt.y = PZtmp.x + tunwide*cos(2*PI/5) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(2*PI/5) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.5+tunwide*sin(2*PI/5);	
		Ptarr.append(pt);	
		pt.y = PZtmp.x;
		pt.x = PZtmp.y;
		pt.z = Ptarr[5].z+1.5+tunwide;	
		Ptarr.append(pt);	
	}
	else
	{
		pt.y = PZtmp.x + 4.11 * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + 4.11 * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[0].z;
		Ptarr.append(pt);
		pt.x = Ptarr[5].x;
		pt.y = Ptarr[5].y;
		pt.z = Ptarr[5].z+0.25;
		Ptarr.append(pt);
		pt.y = PZtmp.x + 5.2 * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + 5.2 * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[6].z;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + 5.4 * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + 5.4 * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.5;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(PI/10) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(PI/10) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.5+tunwide*sin(PI/10);	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(PI/5) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(PI/5) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.5+tunwide*sin(PI/5);	
		Ptarr.append(pt);		
		pt.y = PZtmp.x + tunwide*cos(3*PI/10) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(3*PI/10) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.5+tunwide*sin(3*PI/10);	
		Ptarr.append(pt);	
		pt.y = PZtmp.x + tunwide*cos(2*PI/5) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(2*PI/5) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.5+tunwide*sin(2*PI/5);	
		Ptarr.append(pt);	
		pt.y = PZtmp.x;
		pt.x = PZtmp.y;
		pt.z = Ptarr[5].z+1.5+tunwide;	
		Ptarr.append(pt);	
	}
}

//增加隧道身  两侧性质都为隧道 x =隧面宽度的一半 PZTtmp  隧道单洞中心位置  双拱
void CDraw3D::AddDouTunBodyPt(AcGePoint3dArray &Ptarr,int LorR, xlpoint &PZtmp)
{
	AcGePoint3d pt;
	double 	tunwide= 5.44;//隧道半径

	if(LorR==-1)
	{
		pt.y = PZtmp.x + tunwide*cos(12.6*PI/180-1.164) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(12.6*PI/180-1.164) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[4].z;
		Ptarr.append(pt);
		pt.x = Ptarr[5].x;
		pt.y = Ptarr[5].y;
		pt.z = Ptarr[5].z+0.25;
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(12.6*PI/180) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(12.6*PI/180) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[6].z;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + 5.44 * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + 5.44 * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.415;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(PI/10) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(PI/10) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*sin(PI/10);	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(PI/5) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(PI/5) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*sin(PI/5);	
		Ptarr.append(pt);		
		pt.y = PZtmp.x + tunwide*cos(3*PI/10) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(3*PI/10) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*sin(3*PI/10);	
		Ptarr.append(pt);	
		pt.y = PZtmp.x + tunwide*cos(2*PI/5) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(2*PI/5) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*sin(2*PI/5);	
		Ptarr.append(pt);	
		pt.y = PZtmp.x;
		pt.x = PZtmp.y;
		pt.z = Ptarr[5].z+1.415+tunwide;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*sin(21.1*PI/180) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*sin(21.1*PI/180) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*cos(21.1*PI/180);
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*sin(42.2*PI/180) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*sin(42.2*PI/180) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*cos(42.2*PI/180);	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*sin(63.3*PI/180) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*sin(63.3*PI/180) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*cos(63.3*PI/180);				
		Ptarr.append(pt);
		pt.x = Ptarr[16].x;
		pt.y = Ptarr[16].y;
		pt.z = Ptarr[5].z+0.25;				
		Ptarr.append(pt);
		pt.y = PZtmp.x + (tunwide*sin(63.3*PI/180)-0.5) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (tunwide*sin(63.3*PI/180)-0.5) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+0.25;				
		Ptarr.append(pt);
		pt.x = Ptarr[18].x;
		pt.y = Ptarr[18].y;
		pt.z = Ptarr[5].z;				
		Ptarr.append(pt);				
	}
	else
	{
		pt.y = PZtmp.x + tunwide*cos(12.6*PI/180-1.164) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(12.6*PI/180-1.164) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[4].z;
		Ptarr.append(pt);
		pt.x = Ptarr[5].x;
		pt.y = Ptarr[5].y;
		pt.z = Ptarr[5].z+0.25;
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(12.6*PI/180) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(12.6*PI/180) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[6].z;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + 5.44 * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + 5.44 * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.415;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(PI/10) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(PI/10) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*sin(PI/10);	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*cos(PI/5) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(PI/5) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*sin(PI/5);	
		Ptarr.append(pt);		
		pt.y = PZtmp.x + tunwide*cos(3*PI/10) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(3*PI/10) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*sin(3*PI/10);	
		Ptarr.append(pt);	
		pt.y = PZtmp.x + tunwide*cos(2*PI/5) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + tunwide*cos(2*PI/5) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*sin(2*PI/5);	
		Ptarr.append(pt);	
		pt.y = PZtmp.x;
		pt.x = PZtmp.y;
		pt.z = Ptarr[5].z+1.415+tunwide;	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*sin(21.1*PI/180) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*sin(21.1*PI/180) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*cos(21.1*PI/180);
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*sin(42.2*PI/180) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*sin(42.2*PI/180) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*cos(42.2*PI/180);	
		Ptarr.append(pt);
		pt.y = PZtmp.x + tunwide*sin(63.3*PI/180) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + tunwide*sin(63.3*PI/180) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+1.415+tunwide*cos(63.3*PI/180);				
		Ptarr.append(pt);
		pt.x = Ptarr[16].x;
		pt.y = Ptarr[16].y;
		pt.z = Ptarr[5].z+0.25;				
		Ptarr.append(pt);
		pt.y = PZtmp.x + (tunwide*sin(63.3*PI/180)-0.5) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + (tunwide*sin(63.3*PI/180)-0.5) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z+0.25;				
		Ptarr.append(pt);
		pt.x = Ptarr[18].x;
		pt.y = Ptarr[18].y;
		pt.z = Ptarr[5].z;				
		Ptarr.append(pt);			
	}
}
//增加梁身四点  两侧性质都为桥 x =桥面宽度的一半 PZTtmp 桥梁中心位置
void CDraw3D::AddBriBodyPt(AcGePoint3dArray &Ptarr,int LorR,double x, xlpoint &PZtmp)
{
	AcGePoint3d pt;

	if(LorR==-1)
	{
		pt.x = Ptarr[4].x;
		pt.y = Ptarr[4].y;
		pt.z = Ptarr[4].z-0.2;
		Ptarr.append(pt);
		pt.y = PZtmp.x + (x-1) * cos(PZtmp.a - 0.5*PI);
		pt.x = PZtmp.y + (x-1) * sin(PZtmp.a - 0.5*PI);
		pt.z = Ptarr[5].z - 0.5;
		Ptarr.append(pt);
		pt.x = Ptarr[6].x;
		pt.y = Ptarr[6].y;
		pt.z = Ptarr[6].z-2.0;
		Ptarr.append(pt);
		pt.x = PZtmp.y;
		pt.y = PZtmp.x;
		pt.z = Ptarr[7].z;
		Ptarr.append(pt);
	}
	else
	{
		pt.x = Ptarr[4].x;
		pt.y = Ptarr[4].y;
		pt.z = Ptarr[4].z-0.2;
		Ptarr.append(pt);
		pt.y = PZtmp.x + (x-1) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x-1) * sin(PZtmp.a + 0.5*PI);
		pt.z = Ptarr[5].z - 0.5;
		Ptarr.append(pt);
		pt.x = Ptarr[6].x;
		pt.y = Ptarr[6].y;
		pt.z = Ptarr[6].z-2.0;
		Ptarr.append(pt);
		pt.x = PZtmp.y;
		pt.y = PZtmp.x;
		pt.z = Ptarr[7].z;
		Ptarr.append(pt);
	}
}
void CDraw3D::ReadACrossResultData(FILE *fpr,int jcross)
{
	ACHAR tmp[256];
	AcGePoint3d pt;
	int itmp;
	double dtmp;
	double Z_EarthHOnLJCen,Y_EarthHOnLJCen;
	double OffsetByXLZX = 0.0;

	fwscanf(fpr, L"%lf%lf%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
		&(hdmdes[jcross].cml),
		&(hdmdes[jcross].dml),
		&(itmp),
		&(hdmdes[jcross].IfHasYlink),
		&(hdmdes[jcross].LZDK),
		&(hdmdes[jcross].RZDK),
		&(hdmdes[jcross].Ldeltah),
		&(hdmdes[jcross].Rdeltah),
		&(hdmdes[jcross].ZCG_GDR),
		&(hdmdes[jcross].YCG_GDR),
		&(hdmdes[jcross].newLYd),
		&(hdmdes[jcross].OffsetByXLZX),
		&(hdmdes[jcross].xjj),
		&dtmp,&dtmp,//清表厚度
		&(hdmdes[jcross].FLDcml),
		tmp
		);
	hdmdes[jcross].newRYd = hdmdes[jcross].newLYd;
	Z_EarthHOnLJCen = Y_EarthHOnLJCen = hdmdes[jcross].newLYd;
	OffsetByXLZX = hdmdes[jcross].OffsetByXLZX;
	hdmdes[jcross].IfHasZlink=hdmdes[jcross].IfHasYlink=0;
	hdmdes[jcross].DorS=1;
	hdmdes[jcross].FLDname = tmp;
	CString GH;
	double dml = pm->XLC(hdmdes[jcross].cml,GH);

	_tcscpy(hdmdes[jcross].ckml,pm->LCchr(GH,dml,3));
	//1.写左线左侧
	fwscanf(fpr,L"%d",&hdmdes[jcross].ZDMXZ);
	int j;
	for(j=0;j<3;j++)
		fwscanf(fpr,L"%lf",&hdmdes[jcross].zlmhd[j]);

	//写路面点
	for(j=0;j<5;j++)
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].RHFL[2*j],&hdmdes[jcross].RHFL[2*j+1]);
	//左设计点
	fwscanf(fpr,L"%lf%lf%d",&hdmdes[jcross].Zt,&hdmdes[jcross].ZHc,&hdmdes[jcross].Lnpt);//换行
	xlpoint PZtmp;
	PZtmp.lc=hdmdes[jcross].cml;  // 统一里程
	pm->xlpoint_pz(&PZtmp);
	PZtmp.x = PZtmp.x - LeftLowPt[0];
	PZtmp.y = PZtmp.y - LeftLowPt[1];
	double x,y;
	for(j=0; j<hdmdes[jcross].Lnpt; j++)
	{
		fwscanf(fpr,L"%lf%lf",&x,&y);

		//////////////////////////////////////////////////////////////////////////不再需要
		// 		if((fabs(hdmdes[jcross].Zt-1.0)<0.1) && hdmdes[jcross].ZCG_GDR>0.1 && hdmdes[jcross].Lnpt>16)//挖方且碟形沟
		// 		{
		// 			if(j>5  && j<16)
		// 				continue;
		// 		}
		if (j==0)
		{
			Z_EarthHOnLJCen = hdmdes[jcross].RHFL[1] - y;
		}
		pt.x = x, pt.y =y; pt.z =0;
		hdmdes[jcross].RPTL.append(pt);

		//		pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
		//		pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
		//		pt.z = hdmdes[jcross].newLYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Z_EarthHOnLJCen;

		hdmdes[jcross].PTRPTL.append(pt);	
	}
	// 	if((fabs(hdmdes[jcross].Zt-1.0)<0.1)&& hdmdes[jcross].ZCG_GDR>0.1 && hdmdes[jcross].Lnpt>16)//挖方且碟形沟
	// 		hdmdes[jcross].Lnpt = hdmdes[jcross].Lnpt-10;

	if(hdmdes[jcross].Lnpt==0)
	{
		hdmdes[jcross].Lnpt = 5;
		for(j=0;j<5;j++)//路面点
		{
			x = fabs(hdmdes[jcross].RHFL[2*j]);
			// 			pt.y = PZtmp.x + x * cos(PZtmp.a - 0.5*PI);
			// 			pt.x = PZtmp.y + x * sin(PZtmp.a - 0.5*PI);
			// 			pt.z = hdmdes[jcross].RHFL[2*j+1];
			pt.y = PZtmp.x + (-x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
			pt.x = PZtmp.y + (-x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
			pt.z = hdmdes[jcross].RHFL[2*j+1];
			hdmdes[jcross].PTRPTL.append(pt);
		}
	}	
	//左水沟点
	fwscanf(fpr,L"%d%lf%d%d%d",&itmp,&dtmp,&hdmdes[jcross].LGouProperty,&hdmdes[jcross].LGouPtNum,&hdmdes[jcross].LJSGouPtNum);
	for(j=0; j<hdmdes[jcross].LGouPtNum; j++)
	{
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Lgou[j*2],&hdmdes[jcross].Lgou[j*2+1]);
		double x,y;
		x = hdmdes[jcross].Lgou[j*2];
		y = hdmdes[jcross].Lgou[j*2+1];

		// 			pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
		// 			pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
		// 			pt.z = hdmdes[jcross].newLYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Z_EarthHOnLJCen;
		hdmdes[jcross].PTLgou.append(pt);

	}
	//左截水沟
	for(j=0;j<hdmdes[jcross].LJSGouPtNum;j++)
	{
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].LJSgou[j*2],&hdmdes[jcross].LJSgou[j*2+1]);
		double x,y;
		x = hdmdes[jcross].LJSgou[j*2];
		y = hdmdes[jcross].LJSgou[j*2+1];

		// 			pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
		// 			pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
		// 			pt.z = hdmdes[jcross].newLYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Z_EarthHOnLJCen;
		hdmdes[jcross].PTLJSgou.append(pt);
	}
	//2.写左线右侧
	//写路面点
	fwscanf(fpr,L"%d",&hdmdes[jcross].YDMXZ);
	for(j=0;j<3;j++)
		fwscanf(fpr,L"%lf",&hdmdes[jcross].ylmhd[j]);

	for(j=0;j<5;j++)
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].RHFR[2*j],&hdmdes[jcross].RHFR[2*j+1]);
	//右设计点
	fwscanf(fpr,L"%lf%lf%d ",&hdmdes[jcross].Yt,&hdmdes[jcross].YHc,&hdmdes[jcross].Rnpt);
	for(j=0; j<hdmdes[jcross].Rnpt; j++)
	{
		double x,y;
		fwscanf(fpr,L"%lf%lf",&x,&y);

		// 		if((fabs(hdmdes[jcross].Yt-1.0)<0.1) && hdmdes[jcross].YCG_GDR>0.1 && hdmdes[jcross].Rnpt>16)//挖方且碟形沟
		// 		{
		// 			if(j>5  && j<16)
		// 				continue;
		// 		}
		if (j==0)
		{
			Y_EarthHOnLJCen = hdmdes[jcross].RHFR[1] - y;
		}

		pt.x = x, pt.y =y; pt.z =0;
		hdmdes[jcross].RPTR.append(pt);

		// 			pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
		// 			pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
		// 			pt.z = hdmdes[jcross].newLYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Y_EarthHOnLJCen;
		hdmdes[jcross].PTRPTR.append(pt);

	}	//右水沟点
	// 	if((fabs(hdmdes[jcross].Yt-1.0)<0.1) && hdmdes[jcross].YCG_GDR>0.1 && hdmdes[jcross].Rnpt>16)//挖方且碟形沟
	// 	{
	// 		hdmdes[jcross].Rnpt = hdmdes[jcross].Rnpt-10;
	// 	}


	if(hdmdes[jcross].Rnpt==0)
	{
		hdmdes[jcross].Rnpt = 5;
		for(j=0;j<hdmdes[jcross].Rnpt;j++)
		{
			x = hdmdes[jcross].RHFR[2*j];
			// 			pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a + 0.5*PI);
			// 			pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a + 0.5*PI);
			// 			pt.z = hdmdes[jcross].RHFR[2*j+1];
			pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
			pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
			pt.z = hdmdes[jcross].RHFR[2*j+1];
			hdmdes[jcross].PTRPTR.append(pt);
		}
	}
	fwscanf(fpr,L"%d%lf%d%d%d",&itmp,&dtmp,&hdmdes[jcross].RGouProperty,&hdmdes[jcross].RGouPtNum,&hdmdes[jcross].RJSGouPtNum);
	for(j=0; j<hdmdes[jcross].RGouPtNum; j++)
	{
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Rgou[j*2],&hdmdes[jcross].Rgou[j*2+1]);
		double x,y;
		x = hdmdes[jcross].Rgou[j*2];
		y = hdmdes[jcross].Rgou[j*2+1];

		// 			pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
		// 			pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
		// 			pt.z = hdmdes[jcross].newLYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Y_EarthHOnLJCen;
		hdmdes[jcross].PTRgou.append(pt);	
	}
	//右截水沟
	for(j=0;j<hdmdes[jcross].RJSGouPtNum;j++)
	{
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].RJSgou[j*2],&hdmdes[jcross].RJSgou[j*2+1]);
		double x,y;
		x = hdmdes[jcross].RJSgou[j*2];
		y = hdmdes[jcross].RJSgou[j*2+1];

		// 			pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
		// 			pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
		// 			pt.z = hdmdes[jcross].newLYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Y_EarthHOnLJCen;
		hdmdes[jcross].PTRJSgou.append(pt);
	}

	if(hdmdes[jcross].IfHasZlink )//左有关联道路
	{
		//	hdmdes[jcross].zlink = new CRSDAT;
		double dtmp;
		int itmp,jsgnum,num;

		fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
			&dtmp,
			&dtmp,
			&dtmp,
			&dtmp,
			&dtmp,
			&dtmp,
			&dtmp,
			tmp			
			);
		//hdmdes[jcross].zlink->glRoadName = tmp;
		//1.写左线左侧
		fwscanf(fpr,L"%d",&itmp);
		//写路面点
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&dtmp);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//左设计点
		fwscanf(fpr,L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//换行
		for(j=0; j<itmp; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//左水沟点
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}
		//左截水沟
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}
		//2.写左线右侧
		//写路面点
		fwscanf(fpr,L"%d",&itmp);
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&dtmp);

		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//右设计点
		fwscanf(fpr,L"%lf%lf%d ",&dtmp,&dtmp,&num);
		for(j=0; j<num; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);

		//右水沟点
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//右截水沟
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}		
	}

	if(hdmdes[jcross].IfHasYlink )//右有关联道路
	{
		double dtmp;
		int itmp,jsgnum,num;

		fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
			&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,
			tmp			
			);
		//hdmdes[jcross].zlink->glRoadName = tmp;
		//1.写左线左侧
		fwscanf(fpr,L"%d",&itmp);
		//写路面点
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&dtmp);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//左设计点
		fwscanf(fpr,L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//换行
		for(j=0; j<itmp; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//左水沟点
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}
		//左截水沟
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}
		//2.写左线右侧
		//写路面点
		fwscanf(fpr,L"%d",&itmp);
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&dtmp);

		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//右设计点
		fwscanf(fpr,L"%lf%lf%d ",&dtmp,&dtmp,&num);
		for(j=0; j<num; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);

		//右水沟点
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//右截水沟
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}		
	}
}

int CDraw3D::draw_fill_cut()
{
	int i;	
	ACHAR pszLabel[256];	
	int pos;
	bool ztflag,ytflag, zwflag,ywflag;
	CRSDAT MidCrs;
	AcGePoint3d Pt,spt,ept;
	ztflag=ytflag=zwflag=ywflag=true;
	_stprintf(pszLabel,L"正在绘制道路模型,请稍候");
	acedSetStatusBarProgressMeter(pszLabel,0,100);

	for(i=1;i<NCRS;i++)
	{
		pos=(int)i*100.0/NCRS;
		acedSetStatusBarProgressMeterPos(pos);
		/*
		if(hdmdes[i].cml<Scml || hdmdes[i].cml>Ecml+10)
		continue;*/
		//(hdmdes[i-1].DorS==1&&hdmdes[i].DorS==1)
		//绘左侧填方示坡线
		double qlmk,hlmk;
		qlmk = fabs(hdmdes[i-1].RHFL[4]-hdmdes[i-1].RHFL[2]);
		hlmk = fabs(hdmdes[i].RHFL[4]-hdmdes[i].RHFL[2]);

		/*if(fabs(qlmk-hlmk)<5.0)*/
			drawfillandcut(i,-1,hdmdes[i-1].Ldeltah,hdmdes[i].Ldeltah,hdmdes[i-1].Zt,hdmdes[i].Zt,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Lnpt,
			hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL);	
	}		   
	acedSetStatusBarProgressMeterPos(100);


	for(i=1;i<NCRS;i++)
	{
		pos=(int)i*100.0/NCRS;
		acedSetStatusBarProgressMeterPos(pos);
		/*
		if(hdmdes[i].cml<Scml || hdmdes[i].cml>Ecml+10)
		continue;*/
		//(hdmdes[i-1].DorS==1&&hdmdes[i].DorS==1)
		//绘右侧挖方示坡线
		double qlmk,hlmk;
		qlmk = fabs(hdmdes[i-1].RHFR[4]-hdmdes[i-1].RHFR[2]);
		hlmk = fabs(hdmdes[i].RHFR[4]-hdmdes[i].RHFR[2]);
		if(fabs(qlmk-hlmk)<5)
			drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
			hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR);		   

	}
	acedSetStatusBarProgressMeterPos(99);

	/*
	for(i=1;i<NCRS;i++)
	{
	pos=(int)i*100.0/NCRS;
	acedSetStatusBarProgressMeterPos(pos);
	if(hdmdes[i].cml<Scml || hdmdes[i].cml>Ecml+10)
	continue;
	DrawAllSlop(i);//绘桥面锥坡
	}*/

	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
	return RTNORM;
}

double CDraw3D::FindBriAng(double cml)
{
	for(int i=0;i<DBS.BriNum;i++)
	{
		if(fabs(cml-DBS.Bri[i].Scml)<0.1)
			return DBS.Bri[i].HeadAng;
		else if(fabs(cml-DBS.Bri[i].Ecml)<0.1)
			return DBS.Bri[i].TailAng;
	}
	return 90;
}
//求填挖零点断面
void CDraw3D::CalTWZeroPt(double bml,double eml,double Bdeltah,double Edeltah,double &k,double &MidMl)
{
	if(Bdeltah*Edeltah<0.0)
		k = fabs(Edeltah)/fabs(Bdeltah);
	else
		k = 1;
	//找出路肩填挖零点
	MidMl = (eml + k*bml)/(1+k);
}

//对隧道构建Ptarr
void CDraw3D::CreateTunPt(int jcross,AcGePoint3dArray &Ptarr,int ZorY)
{
	xlpoint PZtmp,PZ;
	double x,y,HarfWid;
	double OffsetByXLZX = 0.0;
	xlpoint PZLJZX;//路基中心点
	PZtmp.lc=hdmdes[jcross].cml;  // 统一里程
	pm->xlpoint_pz(&PZtmp);
	OffsetByXLZX = hdmdes[jcross].OffsetByXLZX;
	PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
	PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
	// 	PZtmp.x = PZtmp.x - LeftLowPt[0];
	// 	PZtmp.y = PZtmp.y - LeftLowPt[1];
	PZtmp.x = PZLJZX.x - LeftLowPt[0];
	PZtmp.y = PZLJZX.y - LeftLowPt[1];
	if(hdmdes[jcross].RHFL[8]+hdmdes[jcross].RHFR[8]<20.0)//单拱隧道
	{
		x = PZtmp.x + hdmdes[jcross].RHFR[8]*cos(PZtmp.a+0.5*PI);
		y = PZtmp.y + hdmdes[jcross].RHFR[8]*sin(PZtmp.a+0.5*PI);
		HarfWid = 0.5*(hdmdes[jcross].RHFL[8]+hdmdes[jcross].RHFR[8]);
		PZ.x = x + HarfWid*cos(PZtmp.a-0.5*PI);
		PZ.y = y + HarfWid*sin(PZtmp.a-0.5*PI);
		PZ.a = PZtmp.a;
		AddSinTunBodyPt(Ptarr,ZorY,PZ);
	}
	else
	{
		if(ZorY==-1)//左侧洞
		{			
			PZ.x = PZtmp.x + 0.5*hdmdes[jcross].RHFL[8]*cos(PZtmp.a-0.5*PI);
			PZ.y = PZtmp.y + 0.5*hdmdes[jcross].RHFL[8]*sin(PZtmp.a-0.5*PI);
			PZ.a = PZtmp.a;
			AddDouTunBodyPt(Ptarr,ZorY,PZ);
		}
		else//右侧洞
		{
			PZ.x = PZtmp.x + 0.5*hdmdes[jcross].RHFR[8]*cos(PZtmp.a+0.5*PI);
			PZ.y = PZtmp.y + 0.5*hdmdes[jcross].RHFR[8]*sin(PZtmp.a+0.5*PI);
			PZ.a = PZtmp.a;
			AddDouTunBodyPt(Ptarr,ZorY,PZ);
		}
	}
}
//对桥梁断面构建Ptarr brixz为断面性质
void CDraw3D::CreateBriPt(int jcross,int brixz,AcGePoint3dArray &Ptarr,int ZorY)
{
	xlpoint PZtmp,PZ;
	double x,y,HarfWid;
	xlpoint PZLJZX;
	double OffsetByXLZX = 0.0;

	if(hdmdes[jcross].ZDMXZ==brixz&&hdmdes[jcross].YDMXZ==brixz)//左右侧桥梁断面性质相同
	{
		PZtmp.lc=hdmdes[jcross].cml;  // 统一里程
		pm->xlpoint_pz(&PZtmp);
		OffsetByXLZX = hdmdes[jcross].OffsetByXLZX;
		PZLJZX = PZtmp;
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
		// 		PZtmp.x = PZtmp.x - LeftLowPt[0];
		// 		PZtmp.y = PZtmp.y - LeftLowPt[1];
		PZtmp.x = PZLJZX.x - LeftLowPt[0];
		PZtmp.y = PZLJZX.y - LeftLowPt[1];
		x = PZtmp.x + hdmdes[jcross].RHFR[8]*cos(PZtmp.a+0.5*PI);
		y = PZtmp.y + hdmdes[jcross].RHFR[8]*sin(PZtmp.a+0.5*PI);
		HarfWid = 0.5*(hdmdes[jcross].RHFL[8]+hdmdes[jcross].RHFR[8]);
		PZ.x = x + HarfWid*cos(PZtmp.a-0.5*PI);
		PZ.y = y + HarfWid*sin(PZtmp.a-0.5*PI);
		PZ.a = PZtmp.a;
		AddBriBodyPt(Ptarr,ZorY,HarfWid,PZ);	
	}
	else if(hdmdes[jcross].ZDMXZ==brixz&&hdmdes[jcross].YDMXZ!=brixz)//左侧为桥中 右侧非桥中
	{
		PZtmp.lc=hdmdes[jcross].cml;  // 统一里程
		pm->xlpoint_pz(&PZtmp);
		OffsetByXLZX = hdmdes[jcross].OffsetByXLZX;
		PZLJZX = PZtmp;
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);	
		// 		PZtmp.x = PZtmp.x - LeftLowPt[0];
		// 		PZtmp.y = PZtmp.y - LeftLowPt[1];
		PZtmp.x = PZLJZX.x - LeftLowPt[0];
		PZtmp.y = PZLJZX.y - LeftLowPt[1];
		AddSideBriBodyPt(Ptarr,ZorY,PZtmp);		
	}
	else if(hdmdes[jcross].ZDMXZ!=brixz&&hdmdes[jcross].YDMXZ==brixz)//左侧非桥中 右侧为桥中
	{
		PZtmp.lc=hdmdes[jcross].cml;  // 统一里程
		pm->xlpoint_pz(&PZtmp);
		OffsetByXLZX = hdmdes[jcross].OffsetByXLZX;
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
		// 		PZtmp.x = PZtmp.x - LeftLowPt[0];
		// 		PZtmp.y = PZtmp.y - LeftLowPt[1];
		PZtmp.x = PZLJZX.x - LeftLowPt[0];
		PZtmp.y = PZLJZX.y - LeftLowPt[1];
		AddSideBriBodyPt(Ptarr,ZorY,PZtmp);			
	}
}

int CDraw3D::drawfillandcut(int i,int ZorY,double qdeltah,double hdeltah,double qtw,double htw,int qdmxz,int hdmxz,int qnpt,int hnpt,AcGePoint3dArray &qPtarr, AcGePoint3dArray &hPtarr)
{
	//	CRSDAT MidCrs;
	AcGePoint3d Pt,spt,ept;
	xlpoint  PZtmp; 
	AcGePoint3dArray MidArr;
	int MidNum,j;
	double fwj,lmk,SlopDegree = 1;
	ads_point bpt;	

	double ang,cml,len,dx,dy;
	if(qtw<0.001 && htw<0.001 &&qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)
		//前后断面都为填方且前断面为一般断面
			DrawPartFillBetweenTwoMl(ZorY,qnpt,hnpt,qPtarr,hPtarr);				

	else if(qtw>0.001 && htw<0.001&&qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)//挖-填
	{
		//求填挖零点				
		double MidMl;
		AcGePoint2d MidLJPt,MidPJPt;
		double k;
		CalTWZeroPt(hdmdes[i-1].cml,hdmdes[i].cml,qdeltah,hdeltah,k,MidMl);
		if(MidArr.length() >0)
			MidArr.removeSubArray(0, MidArr.length()-1);
		MidNum = 6;	
		for(j=0;j<5;j++)
		{
			Pt.x = (hPtarr[j].x + k*qPtarr[j].x)/(1+k);
			Pt.y = (hPtarr[j].y + k*qPtarr[j].y)/(1+k);			
			Pt.z = (hPtarr[j].z + k*qPtarr[j].z)/(1+k);
			MidArr.append(Pt);
		}
		MidArr.append(Pt);
		//绘挖方hdmdes[i-1].cml --  MidMl;
		DrawCutPartBetweenTwoMl(ZorY,qnpt,MidNum,qPtarr,MidArr);
		//绘填方MidMl-hdmdes[i].cml;
		DrawPartFillBetweenTwoMl(ZorY,MidNum,hnpt,MidArr,hPtarr);								
	}
	else if(fabs(1.0*qdmxz)>0 || fabs(1.0*hdmxz)>0)//桥或隧道
	{
		if(qdmxz==-1&&hdmxz==1)//桥头桥尾
		{
			AcGePoint3dArray BHArr,BTArr;      				
			for(j=0;j<5;j++)
				BHArr.append(qPtarr[j]);
			CreateBriPt(i-1,-1,BHArr,ZorY);	//	桥头					
			for(j=0;j<5;j++)
				BTArr.append(hPtarr[j]);
			CreateBriPt(i,1,BTArr,ZorY);  // 桥尾
			DrawBriBody(ZorY,BHArr,BTArr);	
		}
		else if(qdmxz==-1&&hdmxz==3)//桥头桥中
		{
			AcGePoint3dArray BHArr;	
			for(j=0;j<5;j++)
				BHArr.append(qPtarr[j]);
			CreateBriPt(i-1,-1,BHArr,ZorY);	//	桥头				
			DrawBriBody(ZorY,BHArr,hPtarr);
		}
		else if(qdmxz==3&&hdmxz==1)//桥中桥尾
		{
			AcGePoint3dArray BTArr;		
			for(j=0;j<5;j++)
				BTArr.append(hPtarr[j]);
			CreateBriPt(i,1,BTArr,ZorY);  // 桥尾
			DrawBriBody(ZorY,qPtarr,BTArr);
		}
		else if(qdmxz==3&&hdmxz==3)//桥中桥中
			DrawBriBody(ZorY,qPtarr,hPtarr);
		else if(qdmxz==4&&hdmxz==4)//隧中隧中
			DrawTunBody(ZorY,qPtarr,hPtarr);
		else if(qdmxz==-2&&hdmxz==4)//隧头隧中
			DrawTunDoor(ZorY,i-1,i);
		else if(qdmxz==4&&hdmxz==2)//隧中隧尾
			DrawTunDoor(ZorY,i,i-1);
		else if(qdmxz==2&&hdmxz==-1)//隧尾桥头
			DrawLM(ZorY,qPtarr,hPtarr);
		else if(qdmxz==1&&hdmxz==-2)//桥尾隧头
			DrawLM(ZorY,qPtarr,hPtarr);
	}       
	//绘挖方示坡线
	if(qtw>0.001 &&htw>0.001 && qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)//前后断面都为挖方且前断面为一般断面
		DrawCutPartBetweenTwoMl(ZorY,qnpt,hnpt,qPtarr,hPtarr);			
	else if(qtw<0.001 && htw>0.001&& qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)//填-挖
	{
		//求填挖零点
		double MidMl;
		AcGePoint2d MidLJPt,MidPJPt;
		double k;
		CalTWZeroPt(hdmdes[i-1].cml,hdmdes[i].cml,qdeltah,hdeltah,k,MidMl);
		if(MidArr.length() >0)
			MidArr.removeSubArray(0, MidArr.length()-1);				
		MidNum = 6;
		for(j=0;j<5;j++)
		{
			Pt.x = (hPtarr[j].x + k*qPtarr[j].x)/(1+k);
			Pt.y = (hPtarr[j].y + k*qPtarr[j].y)/(1+k);			
			Pt.z = (hPtarr[j].z + k*qPtarr[j].z)/(1+k);	
			MidArr.append(Pt);
		}
		MidArr.append(Pt);

		//绘填方hdmdes[i-1].cml --  MidMl;
		DrawPartFillBetweenTwoMl(ZorY,qnpt,MidNum,qPtarr,MidArr);				
		//绘挖方MidMl-hdmdes[i].cml;
		DrawCutPartBetweenTwoMl(ZorY,MidNum,hnpt,MidArr,hPtarr);
	}
	return 1;
}

// 绘洞门 scross为洞头或洞尾断面号 ncross为下一个洞中断面号
void CDraw3D::DrawTunDoor(int ZorY,int scross, int ncross)
{
	AcGePoint3dArray InTunArr;//洞口处与ncross断面对应点
	AcGePoint3dArray OutTunArr;//洞口外圈点 与InTunArr连面
	AcGePoint3d pt;
	xlpoint PZtmp,PZ;
	double x,y,HarfWid,tunwide;
	int i;

	PZtmp.lc=hdmdes[scross].cml;  // 统一里程
	pm->xlpoint_pz(&PZtmp);
	PZtmp.x = PZtmp.x - LeftLowPt[0];
	PZtmp.y = PZtmp.y - LeftLowPt[1];
	if(ZorY==-1)//左侧
		for(i=0;i<5;i++)
			InTunArr.append(hdmdes[scross].PTRPTL[i]);
	else
		for(i=0;i<5;i++)
			InTunArr.append(hdmdes[scross].PTRPTR[i]);
	if(hdmdes[scross].RHFL[8]+hdmdes[scross].RHFR[8]<20.0)//单拱隧道
	{
		tunwide = 5.4;
		x = PZtmp.x + hdmdes[scross].RHFR[8]*cos(PZtmp.a+0.5*PI);
		y = PZtmp.y + hdmdes[scross].RHFR[8]*sin(PZtmp.a+0.5*PI);
		HarfWid = 0.5*(hdmdes[scross].RHFL[8]+hdmdes[scross].RHFR[8]);
		PZ.x = x + HarfWid*cos(PZtmp.a-0.5*PI);
		PZ.y = y + HarfWid*sin(PZtmp.a-0.5*PI);
		PZ.a = PZtmp.a;
		AddSinTunBodyPt(InTunArr,ZorY,PZ);
		if(ZorY==-1)
			DrawTunBody(ZorY,InTunArr,hdmdes[ncross].PTRPTL);
		else
			DrawTunBody(ZorY,InTunArr,hdmdes[ncross].PTRPTR);
		pt.y = PZ.x + (5.2+0.6)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (5.2+0.6)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[7].z;//隧道洞门外圈点，对应7号内圈点
		OutTunArr.append(pt);
		pt.y = PZ.x + (5.4+0.6)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (5.4+0.6)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5;//隧道洞门外圈点，对应8号内圈点
		OutTunArr.append(pt);
		pt.y = PZ.x + (tunwide+0.6)*cos(PI/10)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (tunwide+0.6)*cos(PI/10)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6)*sin(PI/10);//隧道洞门外圈点，对应9号内圈点
		OutTunArr.append(pt);
		pt.y = PZ.x + (tunwide+0.6)*cos(PI/5)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (tunwide+0.6)*cos(PI/5)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6)*sin(PI/5);//隧道洞门外圈点，对应10号内圈点
		OutTunArr.append(pt);
		pt.y = PZ.x + (tunwide+0.6)*cos(3*PI/10)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (tunwide+0.6)*cos(3*PI/10)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6)*sin(3*PI/10);//隧道洞门外圈点，对应11号内圈点
		OutTunArr.append(pt);
		pt.y = PZ.x + (tunwide+0.6)*cos(2*PI/5)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (tunwide+0.6)*cos(2*PI/5)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6)*sin(2*PI/5);//隧道洞门外圈点，对应12号内圈点
		OutTunArr.append(pt);
		pt.y = PZ.x;
		pt.x = PZ.y;
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6);//隧道洞门外圈点，对应13号内圈点
		OutTunArr.append(pt);
		//隧道门拱圈
		for(i=0;i<OutTunArr.length()-1;i++)
			CreateAFace(OutTunArr[i],OutTunArr[i+1],InTunArr[i+8],InTunArr[i+7],1,L"隧道拱圈");
		Draw_TunBZ(ZorY,scross,OutTunArr);
	}	
	else//双拱隧道
	{
		tunwide = 5.44;
		if(ZorY==-1)//左侧洞
		{			
			PZ.x = PZtmp.x + 0.5*hdmdes[scross].RHFL[8]*cos(PZtmp.a-0.5*PI);
			PZ.y = PZtmp.y + 0.5*hdmdes[scross].RHFL[8]*sin(PZtmp.a-0.5*PI);
			PZ.a = PZtmp.a;
			AddDouTunBodyPt(InTunArr,ZorY,PZ);
			DrawTunBody(ZorY,InTunArr,hdmdes[ncross].PTRPTL);
			pt.y = PZ.x + (tunwide+0.6)*cos(12.6*PI/180)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(12.6*PI/180)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[7].z;//隧道洞门外圈点，对应7号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415;//隧道洞门外圈点，对应8号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PI/10)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(PI/10)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(PI/10);//隧道洞门外圈点，对应9号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PI/5)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(PI/5)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(PI/5);//隧道洞门外圈点，对应10号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(3*PI/10)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(3*PI/10)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(3*PI/10);//隧道洞门外圈点，对应11号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(2*PI/5)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(2*PI/5)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(2*PI/5);//隧道洞门外圈点，对应12号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x;
			pt.x = PZ.y;
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6);//隧道洞门外圈点，对应13号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*sin(63.3*PI/180/3)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(63.3*PI/180/3)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(63.3*PI/180/3);//隧道洞门外圈点，对应14号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*sin(2*63.3*PI/180/3)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(2*63.3*PI/180/3)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(2*63.3*PI/180/3);//隧道洞门外圈点，对应15号内圈点
			OutTunArr.append(pt);	
			pt.y = PZ.x + (tunwide+0.6)*sin(63.3*PI/180)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(63.3*PI/180)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(63.3*PI/180);//隧道洞门外圈点，对应16号内圈点
			OutTunArr.append(pt);
			pt.z = InTunArr[0].z;//隧道洞门外圈点，对应17号内圈点
			OutTunArr.append(pt);
			//隧道门拱圈
			for(i=0;i<OutTunArr.length()-1;i++)
				CreateAFace(OutTunArr[i],OutTunArr[i+1],InTunArr[i+8],InTunArr[i+7],1,L"隧道拱圈");
			AcGePoint3d p1,p2;
			p1.x = PZtmp.y; p1.y = PZtmp.x; p1.z = InTunArr[0].z;
			p2.x = PZtmp.y; p2.y = PZtmp.x; p2.z = OutTunArr[9].z;
			CreateAFace(OutTunArr[9],OutTunArr[10],p1,p2,8,L"隧道洞门");
		}
		else//右侧洞
		{
			PZ.x = PZtmp.x + 0.5*hdmdes[scross].RHFR[8]*cos(PZtmp.a+0.5*PI);
			PZ.y = PZtmp.y + 0.5*hdmdes[scross].RHFR[8]*sin(PZtmp.a+0.5*PI);
			PZ.a = PZtmp.a;
			AddDouTunBodyPt(InTunArr,ZorY,PZ);
			DrawTunBody(ZorY,InTunArr,hdmdes[ncross].PTRPTR);

			pt.y = PZ.x + (tunwide+0.6)*cos(12.6*PI/180)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(12.6*PI/180)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[7].z;//隧道洞门外圈点，对应7号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415;//隧道洞门外圈点，对应8号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PI/10)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(PI/10)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(PI/10);//隧道洞门外圈点，对应9号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PI/5)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(PI/5)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(PI/5);//隧道洞门外圈点，对应10号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(3*PI/10)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(3*PI/10)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(3*PI/10);//隧道洞门外圈点，对应11号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(2*PI/5)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(2*PI/5)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(2*PI/5);//隧道洞门外圈点，对应12号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x;
			pt.x = PZ.y;
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6);//隧道洞门外圈点，对应13号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*sin(63.3*PI/180/3)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(63.3*PI/180/3)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(63.3*PI/180/3);//隧道洞门外圈点，对应14号内圈点
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*sin(2*63.3*PI/180/3)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(2*63.3*PI/180/3)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(2*63.3*PI/180/3);//隧道洞门外圈点，对应15号内圈点
			OutTunArr.append(pt);	
			pt.y = PZ.x + (tunwide+0.6)*sin(63.3*PI/180)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(63.3*PI/180)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(63.3*PI/180);//隧道洞门外圈点，对应16号内圈点
			OutTunArr.append(pt);
			pt.z = InTunArr[0].z;//隧道洞门外圈点，对应17号内圈点
			OutTunArr.append(pt);			
			//隧道门拱圈
			for(i=0;i<OutTunArr.length()-1;i++)
				CreateAFace(OutTunArr[i],OutTunArr[i+1],InTunArr[i+8],InTunArr[i+7],1,L"隧道拱圈");
			AcGePoint3d p1,p2;
			p1.x = PZtmp.y; p1.y = PZtmp.x; p1.z = InTunArr[0].z;
			p2.x = PZtmp.y; p2.y = PZtmp.x; p2.z = OutTunArr[9].z;
			CreateAFace(OutTunArr[9],OutTunArr[10],p1,p2,8,L"隧道洞门");			
		}
		Draw_DouTunBZ(ZorY,scross,OutTunArr);
	}
}
//绘隧道板子 scross 洞口处断面号 OutTunArr 拱圈外侧点集
void CDraw3D::Draw_TunBZ(int ZorY,int scross,AcGePoint3dArray &OutTunArr)
{
	double topht,wid,tunwide,dx,dy;
	char mes[80];
	AcGePoint3d p0,p1,p2,p3;
	int ln;

	tunwide = 5.4;
	if(ZorY==-1)//左侧
	{
		topht = hdmdes[scross].PTRPTL[0].z+tunwide+2.2+1.5+0.6;

		p0.x = hdmdes[scross].PTRPTL[4].x;
		p0.y = hdmdes[scross].PTRPTL[4].y;
		p0.z = topht;
		p1.x = p0.x;
		p1.y = p0.y;
		p1.z = OutTunArr[6].z;		
		p2.x = OutTunArr[6].x;
		p2.y = OutTunArr[6].y;
		p2.z = OutTunArr[6].z;
		p3.x = OutTunArr[6].x;
		p3.y = OutTunArr[6].y;
		p3.z = topht;	
		//水平方向
		CreateAFace(p0,p1,p2,p3,8,L"隧道洞门");
		//左侧
		p0.z = OutTunArr[6].z; p1.z = OutTunArr[5].z;		
		CreateAFace(p0,p1,OutTunArr[5],OutTunArr[6],8,L"隧道洞门");

		p0.z = OutTunArr[5].z, p1.z = OutTunArr[4].z; 
		CreateAFace(p0,p1,OutTunArr[4],OutTunArr[5],8,L"隧道洞门");

		p0.z = OutTunArr[4].z, p1.z = OutTunArr[3].z; 
		CreateAFace(p0,p1,OutTunArr[3],OutTunArr[4],8,L"隧道洞门");

		p0.z = OutTunArr[3].z, p1.z = OutTunArr[2].z; 
		CreateAFace(p0,p1,OutTunArr[2],OutTunArr[3],8,L"隧道洞门");

		p0.z = OutTunArr[2].z, p1.z = OutTunArr[1].z; 
		CreateAFace(p0,p1,OutTunArr[1],OutTunArr[2],8,L"隧道洞门");

		p0.z = OutTunArr[1].z, p1.z = OutTunArr[0].z; 
		CreateAFace(p0,p1,OutTunArr[0],OutTunArr[1],8,L"隧道洞门");

		p0.z = OutTunArr[0].z;		p1.z=hdmdes[scross].PTRPTL[4].z;
		p2 = OutTunArr[0], p2.z = OutTunArr[0].z-0.3;
		CreateAFace(p0,p1,p2,OutTunArr[0],8,L"隧道洞门");
		if(hdmdes[scross].Zt>0.001)//左侧挖方
		{
			ln = hdmdes[scross].Lnpt-1;
			dx = hdmdes[scross].PTRPTL[ln].x-hdmdes[scross].PTRPTL[4].x;
			dy = hdmdes[scross].PTRPTL[ln].y-hdmdes[scross].PTRPTL[4].y;
			wid = sqrt(dx*dx+dy*dy)+0.1;			
			p0.z = topht;
			xlpoint PZtmp;
			PZtmp.lc=hdmdes[scross].cml;  // 统一里程
			pm->xlpoint_pz(&PZtmp);
			PZtmp.x = PZtmp.x - LeftLowPt[0];
			PZtmp.y = PZtmp.y - LeftLowPt[1];
			p1.x = hdmdes[scross].PTRPTL[4].x + wid*sin(PZtmp.a-0.5*PI);
			p1.y = hdmdes[scross].PTRPTL[4].y + wid*cos(PZtmp.a-0.5*PI);
			p1.z = topht;
			p2.x = p1.x; p2.y = p1.y; p2.z = hdmdes[scross].PTRPTL[4].z;           						
			CreateAFace(p0,p1,p2,hdmdes[scross].PTRPTL[4],8,L"隧道洞门");
		}
	}
	else
	{
		topht = hdmdes[scross].PTRPTR[0].z+tunwide+2.2+1.5+0.6;

		p0.x = hdmdes[scross].PTRPTR[4].x;
		p0.y = hdmdes[scross].PTRPTR[4].y;
		p0.z = topht;
		p1.x = p0.x;
		p1.y = p0.y;
		p1.z = OutTunArr[6].z;		
		p2.x = OutTunArr[6].x;
		p2.y = OutTunArr[6].y;
		p2.z = OutTunArr[6].z;
		p3.x = OutTunArr[6].x;
		p3.y = OutTunArr[6].y;
		p3.z = topht;	
		//水平方向
		CreateAFace(p0,p1,p2,p3,8,L"隧道洞门");
		//左侧
		p0.z = OutTunArr[6].z; p1.z = OutTunArr[5].z;		
		CreateAFace(p0,p1,OutTunArr[5],OutTunArr[6],8,L"隧道洞门");

		p0.z = OutTunArr[5].z, p1.z = OutTunArr[4].z; 
		CreateAFace(p0,p1,OutTunArr[4],OutTunArr[5],8,L"隧道洞门");

		p0.z = OutTunArr[4].z, p1.z = OutTunArr[3].z; 
		CreateAFace(p0,p1,OutTunArr[3],OutTunArr[4],8,L"隧道洞门");

		p0.z = OutTunArr[3].z, p1.z = OutTunArr[2].z; 
		CreateAFace(p0,p1,OutTunArr[2],OutTunArr[3],8,L"隧道洞门");

		p0.z = OutTunArr[2].z, p1.z = OutTunArr[1].z; 
		CreateAFace(p0,p1,OutTunArr[1],OutTunArr[2],8,L"隧道洞门");

		p0.z = OutTunArr[1].z, p1.z = OutTunArr[0].z; 
		CreateAFace(p0,p1,OutTunArr[0],OutTunArr[1],8,L"隧道洞门");

		p0.z = OutTunArr[0].z;		p1.z=hdmdes[scross].PTRPTR[4].z;
		p2 = OutTunArr[0], p2.z = OutTunArr[0].z-0.3;
		CreateAFace(p0,p1,p2,OutTunArr[0],8,L"隧道洞门");
		if(hdmdes[scross].Zt>0.001)//左侧挖方
		{
			ln = hdmdes[scross].Rnpt-1;
			dx = hdmdes[scross].PTRPTR[ln].x-hdmdes[scross].PTRPTR[4].x;
			dy = hdmdes[scross].PTRPTR[ln].y-hdmdes[scross].PTRPTR[4].y;
			wid = sqrt(dx*dx+dy*dy)+0.1;
			if(wid>40.0)
				wid = 40;
			p0.z = topht;
			xlpoint PZtmp;
			PZtmp.lc=hdmdes[scross].cml;  // 统一里程
			pm->xlpoint_pz(&PZtmp);
			PZtmp.x = PZtmp.x - LeftLowPt[0];
			PZtmp.y = PZtmp.y - LeftLowPt[1];			
			p1.x = hdmdes[scross].PTRPTR[4].x + wid*sin(PZtmp.a+0.5*PI);
			p1.y = hdmdes[scross].PTRPTR[4].y + wid*cos(PZtmp.a+0.5*PI);
			p1.z = topht;
			p2.x = p1.x; p2.y = p1.y; p2.z = hdmdes[scross].PTRPTR[4].z;           						
			CreateAFace(p0,p1,p2,hdmdes[scross].PTRPTR[4],8,L"隧道洞门");
		}		
	}
}

//绘双拱隧道板子 scross 洞口处断面号 OutTunArr 拱圈外侧点集
void CDraw3D::Draw_DouTunBZ(int ZorY,int scross,AcGePoint3dArray &OutTunArr)
{
	double topht,wid,tunwide,dx,dy;
	char mes[80];
	AcGePoint3d p0,p1,p2,p3;
	int ln;
	xlpoint PZtmp;


	PZtmp.lc=hdmdes[scross].cml;  // 统一里程
	pm->xlpoint_pz(&PZtmp);//线路中心点
	PZtmp.x = PZtmp.x - LeftLowPt[0];
	PZtmp.y = PZtmp.y - LeftLowPt[1];
	tunwide = 5.44;
	if(ZorY==-1)//左侧
	{
		topht = hdmdes[scross].PTRPTL[0].z+tunwide+2.2+1.5+0.6;

		p0.x = hdmdes[scross].PTRPTL[4].x;
		p0.y = hdmdes[scross].PTRPTL[4].y;
		p0.z = topht;
		p1.x = p0.x;
		p1.y = p0.y;
		p1.z = OutTunArr[6].z;		
		p2.x = PZtmp.y;
		p2.y = PZtmp.x;
		p2.z = OutTunArr[6].z;
		p3.x = PZtmp.y;
		p3.y = PZtmp.x;
		p3.z = topht;	
		//水平方向
		CreateAFace(p0,p1,p2,p3,8,L"隧道洞门");
		//左侧
		p0.z = OutTunArr[6].z; p1.z = OutTunArr[5].z;		
		CreateAFace(p0,p1,OutTunArr[5],OutTunArr[6],8,L"隧道洞门");		
		p0.z = OutTunArr[5].z, p1.z = OutTunArr[4].z; 
		CreateAFace(p0,p1,OutTunArr[4],OutTunArr[5],8,L"隧道洞门");		
		p0.z = OutTunArr[4].z, p1.z = OutTunArr[3].z; 
		CreateAFace(p0,p1,OutTunArr[3],OutTunArr[4],8,L"隧道洞门");		
		p0.z = OutTunArr[3].z, p1.z = OutTunArr[2].z; 
		CreateAFace(p0,p1,OutTunArr[2],OutTunArr[3],8,L"隧道洞门");		
		p0.z = OutTunArr[2].z, p1.z = OutTunArr[1].z; 
		CreateAFace(p0,p1,OutTunArr[1],OutTunArr[2],8,L"隧道洞门");		
		p0.z = OutTunArr[1].z, p1.z = OutTunArr[0].z; 
		CreateAFace(p0,p1,OutTunArr[0],OutTunArr[1],8,L"隧道洞门");
		p0.z = OutTunArr[0].z;		p1.z=hdmdes[scross].PTRPTL[4].z;
		p2 = OutTunArr[0], p2.z = OutTunArr[0].z-0.3;
		CreateAFace(p0,p1,p2,OutTunArr[0],8,L"隧道洞门");//单洞左半边

		p0.x = PZtmp.y, p0.y = PZtmp.x, p0.z=OutTunArr[6].z;
		p1.x = PZtmp.y, p1.y = PZtmp.x, p1.z=OutTunArr[7].z;
		CreateAFace(p0,OutTunArr[6],OutTunArr[7],p1,8,L"隧道洞门");
		p0.z = p1.z, p1.z = OutTunArr[8].z;
		CreateAFace(p0,OutTunArr[7],OutTunArr[8],p1,8,L"隧道洞门");
		p0.z = p1.z, p1.z = OutTunArr[9].z;
		CreateAFace(p0,OutTunArr[8],OutTunArr[9],p1,8,L"隧道洞门");//单洞右半边
		if(hdmdes[scross].Zt>0.001)//左侧挖方
		{
			ln = hdmdes[scross].Lnpt-1;
			dx = hdmdes[scross].PTRPTL[ln].x-hdmdes[scross].PTRPTL[4].x;
			dy = hdmdes[scross].PTRPTL[ln].y-hdmdes[scross].PTRPTL[4].y;
			wid = sqrt(dx*dx+dy*dy)+0.1;
			p0.x = hdmdes[scross].PTRPTL[4].x;
			p0.y = hdmdes[scross].PTRPTL[4].y;			
			p0.z = topht;		

			p1.x = hdmdes[scross].PTRPTL[4].x + wid*sin(PZtmp.a-0.5*PI);
			p1.y = hdmdes[scross].PTRPTL[4].y + wid*cos(PZtmp.a-0.5*PI);
			p1.z = topht;
			p2.x = p1.x; p2.y = p1.y; p2.z = hdmdes[scross].PTRPTL[4].z;           						
			CreateAFace(p0,p1,p2,hdmdes[scross].PTRPTL[4],8,L"隧道洞门");
		}
	}
	else
	{
		topht = hdmdes[scross].PTRPTR[0].z+tunwide+2.2+1.5+0.6;

		p0.x = hdmdes[scross].PTRPTR[4].x;
		p0.y = hdmdes[scross].PTRPTR[4].y;
		p0.z = topht;
		p1.x = p0.x;
		p1.y = p0.y;
		p1.z = OutTunArr[6].z;		
		p2.x = PZtmp.y;
		p2.y = PZtmp.x;
		p2.z = OutTunArr[6].z;
		p3.x = PZtmp.y;
		p3.y = PZtmp.x;
		p3.z = topht;	
		//水平方向
		CreateAFace(p0,p1,p2,p3,8,L"隧道洞门");
		//左侧
		p0.z = OutTunArr[6].z; p1.z = OutTunArr[5].z;		
		CreateAFace(p0,p1,OutTunArr[5],OutTunArr[6],8,L"隧道洞门");		
		p0.z = OutTunArr[5].z, p1.z = OutTunArr[4].z; 
		CreateAFace(p0,p1,OutTunArr[4],OutTunArr[5],8,L"隧道洞门");		
		p0.z = OutTunArr[4].z, p1.z = OutTunArr[3].z; 
		CreateAFace(p0,p1,OutTunArr[3],OutTunArr[4],8,L"隧道洞门");		
		p0.z = OutTunArr[3].z, p1.z = OutTunArr[2].z; 
		CreateAFace(p0,p1,OutTunArr[2],OutTunArr[3],8,L"隧道洞门");		
		p0.z = OutTunArr[2].z, p1.z = OutTunArr[1].z; 
		CreateAFace(p0,p1,OutTunArr[1],OutTunArr[2],8,L"隧道洞门");		
		p0.z = OutTunArr[1].z, p1.z = OutTunArr[0].z; 
		CreateAFace(p0,p1,OutTunArr[0],OutTunArr[1],8,L"隧道洞门");
		p0.z = OutTunArr[0].z;		p1.z=hdmdes[scross].PTRPTR[4].z;
		p2 = OutTunArr[0], p2.z = OutTunArr[0].z-0.3;
		CreateAFace(p0,p1,p2,OutTunArr[0],8,L"隧道洞门");//单洞左半边

		p0.x = PZtmp.y, p0.y = PZtmp.x, p0.z=OutTunArr[6].z;
		p1.x = PZtmp.y, p1.y = PZtmp.x, p1.z=OutTunArr[7].z;
		CreateAFace(p0,OutTunArr[6],OutTunArr[7],p1,8,L"隧道洞门");
		p0.z = p1.z, p1.z = OutTunArr[8].z;
		CreateAFace(p0,OutTunArr[7],OutTunArr[8],p1,8,L"隧道洞门");
		p0.z = p1.z, p1.z = OutTunArr[9].z;
		CreateAFace(p0,OutTunArr[8],OutTunArr[9],p1,8,L"隧道洞门");//单洞右半边
		if(hdmdes[scross].Yt>0.001)//右侧挖方
		{
			ln = hdmdes[scross].Rnpt-1;
			dx = hdmdes[scross].PTRPTR[ln].x-hdmdes[scross].PTRPTR[4].x;
			dy = hdmdes[scross].PTRPTR[ln].y-hdmdes[scross].PTRPTR[4].y;
			wid = sqrt(dx*dx+dy*dy)+0.1;
			p0.x = hdmdes[scross].PTRPTR[4].x;
			p0.y = hdmdes[scross].PTRPTR[4].y;			
			p0.z = topht;		

			p1.x = hdmdes[scross].PTRPTR[4].x + wid*sin(PZtmp.a+0.5*PI);
			p1.y = hdmdes[scross].PTRPTR[4].y + wid*cos(PZtmp.a+0.5*PI);
			p1.z = topht;
			p2.x = p1.x; p2.y = p1.y; p2.z = hdmdes[scross].PTRPTR[4].z;           						
			CreateAFace(p0,p1,p2,hdmdes[scross].PTRPTR[4],8,L"隧道洞门");
		}
	}
}
//绘桥梁身 共九个点
void CDraw3D::DrawBriBody(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr)
{
	AcGePoint3d p0,p1,p2,p3;
	int npt,i,color=1,js;
	AcGePoint3d spt,ept;
	ACHAR lname[180];

	//	npt = qPtarr.length();//有问题？？？
	npt = min(qPtarr.length(),hPtarr.length());
	for(i=0;i<npt-1;i++)
	{
		p0 = qPtarr[i];
		p1 = qPtarr[i+1];
		p2 = hPtarr[i+1];
		p3 = hPtarr[i];
		if(p0.x<0.1||p1.x<0.1||p2.x<0.1||p3.x<0.1)
			continue;
		if(ZorY==-1)
		{
			if(i==0)
			{
				_tcscpy(lname,L"桥梁左侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"桥梁左侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"桥梁左侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"桥梁左侧土路肩");
				color = 4;
			}
			else
			{
				_tcscpy(lname,L"桥梁左侧梁身");
				color = 8;		
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"桥梁右侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"桥梁右侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"桥梁右侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"桥梁右侧土路肩");
				color = 4;
			}
			else
			{
				_tcscpy(lname,L"桥梁右侧梁身");
				color = 8;
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}
}

//绘隧道 
void CDraw3D::DrawTunBody(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr)
{
	AcGePoint3d p0,p1,p2,p3;
	int npt,i,color=1,js;
	AcGePoint3d spt,ept;
	ACHAR lname[180];

	npt = qPtarr.length();
	for(i=0;i<npt-1;i++)
	{
		p0 = qPtarr[i];
		p1 = qPtarr[i+1];
		p2 = hPtarr[i+1];
		p3 = hPtarr[i];
		if(ZorY==-1)
		{
			if(i==0)
			{
				_tcscpy(lname,L"隧道左侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"隧道左侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"隧道左侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"隧道左侧土路肩");
				color = 4;
			}
			else
			{
				_tcscpy(lname,L"隧道左侧洞身");
				color = 8;		
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"隧道右侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"隧道右侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"隧道右侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"隧道右侧土路肩");
				color = 4;
			}
			else
			{
				_tcscpy(lname,L"隧道右侧洞身");
				color = 8;
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}
}

//绘桥隧相连处路面
void CDraw3D::DrawLM(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr)
{
	AcGePoint3d p0,p1,p2,p3;
	int npt,i,color=1,js;
	AcGePoint3d spt,ept;
	ACHAR lname[180];

	npt = 5;

	for(i=0;i<npt-1;i++)
	{
		p0 = qPtarr[i];
		p1 = qPtarr[i+1];
		p2 = hPtarr[i+1];
		p3 = hPtarr[i];
		if(ZorY==-1)
		{
			if(i==0)
			{
				_tcscpy(lname,L"左侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"左侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"左侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"左侧土路肩");
				color = 4;
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"右侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"右侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"右侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"右侧土路肩");
				color = 4;
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}
}
//绘两相邻里程的填方示坡线
void CDraw3D::DrawPartFillBetweenTwoMl(int ZorY,int qPtNum,int hPtNum,
									   AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr)
{
	AcGePoint3d p0,p1,p2,p3;
	int npt,i,color=1,js;
	AcGePoint3d spt,ept;
	ACHAR lname[180];

	if(qPtNum < hPtNum)
		npt = qPtNum;
	else
		npt = hPtNum;

	for(i=0;i<npt-1;i++)
	{
		p0 = qPtarr[i];
		p1 = qPtarr[i+1];
		p2 = hPtarr[i+1];
		p3 = hPtarr[i];
		if(ZorY==-1)
		{
			if(i==0)
			{
				_tcscpy(lname,L"左侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"左侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"左侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"左侧土路肩");
				color = 4;
			}
			else
			{
				if(fmod(i,2.0)<0.01)
				{
					js = (i-4)*0.5+1;
					_stprintf(lname,L"左侧%d级边坡",js);
					color = 2;
				}
				else
				{
					js = (i-3)*0.5;
					_stprintf(lname,L"左侧%d级平台",js);
					color = 8;
				}
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"右侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"右侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"右侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"右侧土路肩");
				color = 4;
			}
			else
			{
				if(fmod(i,2.0)<0.01)
				{
					js = (i-4)*0.5+1;
					_stprintf(lname,L"右侧%d级边坡",js);
					color = 2;
				}
				else
				{
					js = (i-3)*0.5;
					_stprintf(lname,L"右侧%d级平台",js);
					color = 8;
				}
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}

	if(ZorY==-1)
		_tcscpy(lname,L"左侧边坡");
	else
		_tcscpy(lname,L"右侧边坡");
	color = 2;

	if(qPtNum < hPtNum)
	{
		p0 = qPtarr[qPtNum-1];
		p3 = qPtarr[qPtNum-1];
		for(i=qPtNum-1;i<hPtNum-1;i++)
		{
			p1 = hPtarr[i];
			p2 = hPtarr[i+1];
			CreateAFace(p0,p1,p2,p3,color,lname);
		}
	}
	else
	{
		p0=p3=hPtarr[hPtNum-1];
		for(i=hPtNum-1;i<qPtNum-1;i++)
		{
			p1 = qPtarr[i];
			p2 = qPtarr[i+1];
			CreateAFace(p0,p1,p2,p3,color,lname);
		}
	}
}

//绘两相邻里程的挖方示坡线
void CDraw3D::DrawCutPartBetweenTwoMl(int ZorY,int qPtNum,int hPtNum,
									  AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr)
{
	int npt,i,color=0,js;
	AcGePoint3d p0,p1,p2,p3;
	ACHAR lname[180];

	if(qPtNum < hPtNum)
		npt = qPtNum;
	else
		npt = hPtNum;

	for(i=0;i<npt-1;i++)
	{
		p0 = qPtarr[i];
		p1 = qPtarr[i+1];
		p2 = hPtarr[i+1];
		p3 = hPtarr[i];
		if(ZorY==-1)
		{
			if(i==0)
			{
				_tcscpy(lname,L"左侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"左侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"左侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"左侧土路肩");
				color = 4;
			}
			else if(i==4)
			{
				_tcscpy(lname,L"左侧水沟侧边");
				color = 8;
			}
			else if(i==5)
			{
				_tcscpy(lname,L"左侧水沟底");
				color = 8;
			}
			else if(i==6)
			{
				_tcscpy(lname,L"左侧水沟侧边");
				color = 8;
			}
			else
			{
				if(fmod(i,2.0)<0.01)
				{
					js = (i-8)*0.5+1;
					_stprintf(lname,L"左侧%d级边坡",js);
					color = 2;				
				}
				else
				{
					js = (i-5)*0.5;
					_stprintf(lname,L"左侧%d级平台",js);
					color = 8;
				}
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"右侧中分带");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"右侧车行道");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"右侧硬路肩");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"右侧土路肩");
				color = 4;
			}
			else if(i==4)
			{
				_tcscpy(lname,L"右侧水沟侧边");
				color = 8;
			}
			else if(i==5)
			{
				_tcscpy(lname,L"右侧水沟底");
				color = 8;
			}
			else if(i==6)
			{
				_tcscpy(lname,L"右侧水沟侧边");
				color = 8;
			}
			else
			{
				if(fmod(i,2.0)<0.01)
				{
					js = (i-8)*0.5+1;
					_stprintf(lname,L"右侧%d级边坡",js);
					color = 2;				
				}
				else
				{
					js = (i-5)*0.5;
					_stprintf(lname,L"右侧%d级平台",js);
					color = 8;
				}
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}

	if(ZorY==-1)
		_tcscpy(lname,L"左侧边坡");
	else
		_tcscpy(lname,L"右侧边坡");
	color =2;
	if(qPtNum < hPtNum)
	{
		p0 = p3 = qPtarr[qPtNum-1];
		for(i=qPtNum-1;i<hPtNum-1;i++)
		{
			p1 = hPtarr[i]; 
			p2 = hPtarr[i+1]; 
			CreateAFace(p0,p1,p2,p3,color,lname);
		}
	}
	else
	{
		p0 = p3 = hPtarr[hPtNum-1];
		for(i=hPtNum-1;i<qPtNum-1;i++)
		{
			p1 = qPtarr[i];
			p2 = qPtarr[i+1];
			CreateAFace(p0,p1,p2,p3,color,lname);
		}
	}	
}

void CDraw3D::draw_circleBase(AcGePoint3d CenPt,double D,double H)
{
	double Tpt[16][3],Bpt[16][3];
	AcGePoint3dArray TptArr,BptArr;
	AcGePoint3d pt;

	pt.x = CenPt.x + 0.5*D*cos(PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(PI/8.0);
	pt.z = CenPt.z;
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(PI/4.0);
	pt.y = CenPt.y + 0.5*D*sin(PI/4.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(3*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(3*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(PI/2.0);
	pt.y = CenPt.y + 0.5*D*sin(PI/2.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(5*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(5*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(3*PI/4.0);
	pt.y = CenPt.y + 0.5*D*sin(3*PI/4.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(7*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(7*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(PI);
	pt.y = CenPt.y + 0.5*D*sin(PI);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(9*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(9*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(10*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(10*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(11*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(11*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(12*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(12*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(13*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(13*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(14*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(14*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(15*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(15*PI/8.0);
	TptArr.append(pt);

	pt.x = CenPt.x + 0.5*D*cos(16*PI/8.0);
	pt.y = CenPt.y + 0.5*D*sin(16*PI/8.0);
	TptArr.append(pt);

	int i;
	for(i=0; i< 16; i++)
	{ 
		pt.x = TptArr[i].x;
		pt.y = TptArr[i].y;
		pt.z = CenPt.z-H;
		BptArr.append(pt);
	}

	for(i=0; i<15; i++)
		CreateAFace(TptArr[i],BptArr[i],BptArr[i+1],TptArr[i+1],8,L"桥墩");

	AcGePoint3d BCenPt;
	BCenPt.x = CenPt.x,BCenPt.y = CenPt.y,BCenPt.z = CenPt.z-H;
	for(i=0; i<15; i++)
	{
		CreateAFace(TptArr[i],TptArr[i+1],CenPt,CenPt,8,L"桥墩");
		CreateAFace(BptArr[i],BptArr[i+1],BCenPt,BCenPt,8,L"桥墩");
	}
}

void CDraw3D::draw_BaseCape(AcGePoint3d Lpt,AcGePoint3d Rpt,double fwj)           
{
	double wide,H;
	AcGePoint3dArray TptArr,BptArr;
	AcGePoint3d pt;

	wide = 2.5;
	H = 2.0;

	pt.y = Lpt.x + 0.5*wide*cos(fwj);
	pt.x = Lpt.y + 0.5*wide*sin(fwj);
	pt.z = Lpt.z - 2.2;
	TptArr.append(pt);

	pt.y = Rpt.x + 0.5*wide*cos(fwj);
	pt.x = Rpt.y + 0.5*wide*sin(fwj);
	TptArr.append(pt);

	pt.y = Rpt.x + 0.5*wide*cos(fwj+PI);
	pt.x = Rpt.y + 0.5*wide*sin(fwj+PI);
	TptArr.append(pt);

	pt.y = Lpt.x + 0.5*wide*cos(fwj+PI);
	pt.x = Lpt.y + 0.5*wide*sin(fwj+PI);
	TptArr.append(pt);

	int i;
	for(i=0;i<4;i++)
	{
		pt.x = TptArr[i].x;
		pt.y = TptArr[i].y;
		pt.z = Lpt.z - 4.2;
		BptArr.append(pt);
	}	

	for(i=0; i<3; i++)
		CreateAFace(TptArr[i],TptArr[i+1],BptArr[i+1],BptArr[i],8,L"桥帽");

	CreateAFace(TptArr[3],TptArr[0],BptArr[0],BptArr[3],8,L"桥帽");	
	CreateAFace(BptArr[0],BptArr[1],BptArr[2],BptArr[3],8,L"桥帽");			
}

/*
//计算锥坡脚点
int CDraw3D::Cal_ZPJD(int ZorY,AcGePoint3d Cenpt,double fwj,AcGePoint3dArray &ptarr,double SlopDegree)//计算锥坡脚点
{
int i,j,inf;
double du,jspt[2],len;
AcGePoint3d pt;

//	len = sqrt(20*20+(20*SlopDegree)*(20*SlopDegree));//锥坡高20m;
len = SlopDegree;
for(j=0; j<=6; j++)//从横断面方向到线路前进方向(0.5*PI)范围内划分六个方向七个点
{
du= fwj + ZorY*(0.5*PI - j*PI/12.0);

pt.x = Cenpt.x +  len*sin(du);
pt.y = Cenpt.y +  len*cos(du);
pt.z = Cenpt.z - 20;
ptarr.append(pt);
}
return RTNORM;
}*/
//计算锥坡脚点
int CDraw3D::Cal_ZPJD(int ZorY,int HorT,AcGePoint3d Cenpt,AcGePoint3d Spt,AcGePoint3d Ept,AcGePoint3dArray &ptarr)//计算锥坡脚点
{
	int i,j,inf;
	double du,jspt[2],Elen,Slen,Mlen,dx,dy;
	double angle,len;
	ads_point midpt,arcmid,bspt;
	AcGePoint3d pt;
	AcGePoint2d startPoint,pnt,endPoint,Cpt,p1,p2;	
	AcGePoint2dArray p2dArr;

	Spt.z = Cenpt.z-20;
	Ept.z = Cenpt.z-20;
	bspt[X] = Cenpt.x;
	bspt[Y] = Cenpt.y;
	midpt[X] = (Ept.x+Spt.x)/2.0;
	midpt[Y] = (Ept.y+Spt.y)/2.0; //坡脚点与路肩点的中点	
	angle = ads_angle(bspt,midpt);
	dx = Ept.x-Cenpt.x;
	dy = Ept.y-Cenpt.y;	
	Elen = sqrt(dx*dx+dy*dy);
	dx = Spt.x-Cenpt.x;
	dy = Spt.y-Cenpt.y;	
	Slen = sqrt(dx*dx+dy*dy);
	Mlen = 0.5*(Elen+Slen);
	ads_polar(bspt,angle,Mlen,arcmid); //弧的中点
	ptarr.append(Ept);

	startPoint.x =Spt.x;
	startPoint.y =Spt.y;
	pnt.x = arcmid[X];
	pnt.y = arcmid[Y];
	endPoint.x = Ept.x;
	endPoint.y = Ept.y;

	AcGeCircArc2d parc(startPoint,pnt,endPoint);
	int intn;

	double sang,eang,R,ang;

	Cpt.x=parc.center().x;
	Cpt.y=parc.center().y;
	R = parc.radius();

	xyddaa(Cpt.x,Cpt.y,Ept.x,Ept.y,&sang);
	xyddaa(Cpt.x,Cpt.y,Spt.x,Spt.y,&eang);
	double angc;

	angc = eang-sang;
	if(angc<0)
		angc = 2*PI+angc;
	if(angc>PI)
		angc = 2*PI-angc;

	for(j=1; j<6; j++)//从横断面方向到线路前进方向(0.5*PI)范围内划分六个方向七个点
	{
		if(ZorY*HorT==1)
			ang = sang - j*angc/6.0;
		else
			ang = sang + j*angc/6.0;
		pt.x =  Cpt.x + R*cos(ang);
		pt.y =  Cpt.y + R*sin(ang);
		pt.z = Cenpt.z -20;

		ptarr.append(pt);

	}
	ptarr.append(Spt);
	return RTNORM;
}

void CDraw3D::CalBriSlopPt(int i,int ZorY,AcGePoint3dArray &ptarr)
{
	ads_point pt1,pt2,pt3,Ljpt;
	AcGePoint3d CenPt,Spt,Ept;
	double k,ang,len,dx,dy,factor;
	xlpoint ptmp;
	xlpoint PZLJZX;
	double OffsetByXLZX = 0.0;

	factor = 2;
	if(ZorY==-1)
	{
		if(hdmdes[i].ZDMXZ==-1)//左侧桥头锥坡
		{
			if(hdmdes[i].Lnpt==0)
				return;
			ang = FindBriAng(hdmdes[i].cml);
			if(fabs(ang-90)<0.1)
			{
				if(2*fabs(hdmdes[i].RPTL[hdmdes[i].Lnpt-1].x)/3 > 20)//2/3坡脚点至路肩点距离
					k = 1;
				else
					k = fabs(hdmdes[i].RPTL[hdmdes[i].Lnpt-1].x)/30; 

				ptmp.lc = hdmdes[i+1].cml;
				if(fabs(hdmdes[i+1].cml-hdmdes[i].cml)<0.01)
					ptmp.lc = hdmdes[i].cml+5.0;

				pm->xlpoint_pz(&ptmp);
				CalOffsetByXLZX(ptmp.lc,NZLMK,ZLMK,OffsetByXLZX);
				PZLJZX = ptmp;
				PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
				PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
				// 				ptmp.x = ptmp.x - LeftLowPt[0];
				// 				ptmp.y = ptmp.y - LeftLowPt[1];
				ptmp.x = PZLJZX.x - LeftLowPt[0];
				ptmp.y = PZLJZX.y - LeftLowPt[1];
				Ljpt[X] = ptmp.y + fabs(hdmdes[i+1].RHFL[8])*sin(ptmp.a-0.5*PI);
				Ljpt[Y] = ptmp.x + fabs(hdmdes[i+1].RHFL[8])*cos(ptmp.a-0.5*PI);
				Spt.x = hdmdes[i].PTRPTL[4].x + k*(Ljpt[X]-hdmdes[i].PTRPTL[4].x);
				Spt.y = hdmdes[i].PTRPTL[4].y + k*(Ljpt[Y]-hdmdes[i].PTRPTL[4].y);

				CenPt.x = hdmdes[i].PTRPTL[4].x;
				CenPt.y = hdmdes[i].PTRPTL[4].y;
				Ept.x = hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].x;
				Ept.y = hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].y;
			}
			else
			{
				ptmp.lc = hdmdes[i].cml;
				pm->xlpoint_pz(&ptmp);
				OffsetByXLZX = hdmdes[i].OffsetByXLZX;
				PZLJZX = ptmp;
				PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
				PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
				// 				ptmp.x = ptmp.x - LeftLowPt[0];
				// 				ptmp.y = ptmp.y - LeftLowPt[1];
				ptmp.x = PZLJZX.x - LeftLowPt[0];
				ptmp.y = PZLJZX.y - LeftLowPt[1];
				double pt[2],prjml;
				len =  fabs(hdmdes[i].RHFL[8]);
				if(len<0.001)
					len=0.001;
				pm->CalPZXPT(hdmdes[i].cml,ang+180,len-OffsetByXLZX,pt,prjml);
				CenPt.x = pt[1]-LeftLowPt[1];
				CenPt.y = pt[0]-LeftLowPt[0];
				INTERS(&Ept.x,&Ept.y,hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].x,hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].y,
					hdmdes[i-1].PTRPTL[hdmdes[i-1].Lnpt-1].x,hdmdes[i-1].PTRPTL[hdmdes[i-1].Lnpt-1].y,
					ptmp.y,ptmp.x,CenPt.x,CenPt.y);
				dx=Ept.x-CenPt.x;
				dy=Ept.y-CenPt.y;

				len = 2*sqrt(dx*dx+dy*dy)/3;
				if(len<0.01)
					len = 5.0;
				Spt.x = CenPt.x + len*sin(ptmp.a);
				Spt.y = CenPt.y + len*cos(ptmp.a);							
			}
			CenPt.z = hdmdes[i].PTRPTL[0].z;
			dx = CenPt.x-Ept.x;
			dy = CenPt.y-Ept.y;
			len = sqrt(dx*dx+dy*dy);
			if(len < 0.01)
			{
				Ept.x = CenPt.x + 5.0*sin(ptmp.a-0.5*PI);
				Ept.y = CenPt.y + 5.0*cos(ptmp.a-0.5*PI);
			}
			Cal_ZPJD(-1,-1,CenPt,Spt,Ept,ptarr);//计算左侧桥头锥坡脚点
		}
		else if(hdmdes[i].ZDMXZ==1)//左侧桥尾锥坡
		{
			if(hdmdes[i].Lnpt==0)
				return;

			ang = FindBriAng(hdmdes[i].cml);
			if(fabs(ang-90)<0.1)
			{
				if(2*fabs(hdmdes[i].RPTL[hdmdes[i].Lnpt-1].x)/3 > 20)//2/3坡脚点至路肩点距离
					k = 1;
				else
					k =  fabs(hdmdes[i].RPTL[hdmdes[i].Lnpt-1].x)/30; 
				ptmp.lc = hdmdes[i-1].cml;
				if(fabs(hdmdes[i-1].cml-hdmdes[i].cml)<0.01)
					ptmp.lc = hdmdes[i].cml-5.0;
				pm->xlpoint_pz(&ptmp);
				CalOffsetByXLZX(ptmp.lc,NZLMK,ZLMK,OffsetByXLZX);
				PZLJZX = ptmp;
				PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
				PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
				// 				ptmp.x = ptmp.x - LeftLowPt[0];
				// 				ptmp.y = ptmp.y - LeftLowPt[1];
				ptmp.x = PZLJZX.x - LeftLowPt[0];
				ptmp.y = PZLJZX.y - LeftLowPt[1];
				Ljpt[X] = ptmp.y + fabs(hdmdes[i-1].RHFL[8])*sin(ptmp.a-0.5*PI);
				Ljpt[Y] = ptmp.x + fabs(hdmdes[i-1].RHFL[8])*cos(ptmp.a-0.5*PI);
				Spt.x = hdmdes[i].PTRPTL[4].x - k*(hdmdes[i].PTRPTL[4].x-Ljpt[X]);
				Spt.y = hdmdes[i].PTRPTL[4].y - k*(hdmdes[i].PTRPTL[4].y-Ljpt[Y]);
				CenPt.x = hdmdes[i].PTRPTL[4].x;
				CenPt.y = hdmdes[i].PTRPTL[4].y;
				Ept.x = hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].x;
				Ept.y = hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].y;
			}
			else
			{
				ptmp.lc = hdmdes[i].cml;
				pm->xlpoint_pz(&ptmp);
				OffsetByXLZX = hdmdes[i].OffsetByXLZX;
				PZLJZX = ptmp;
				PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
				PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
				// 				ptmp.x = ptmp.x - LeftLowPt[0];
				// 				ptmp.y = ptmp.y - LeftLowPt[1];
				ptmp.x = PZLJZX.x - LeftLowPt[0];
				ptmp.y = PZLJZX.y - LeftLowPt[1];
				double pt[2],prjml;
				len =  fabs(hdmdes[i].RHFL[8]);
				if(len<0.001)
					len=0.001;
				pm->CalPZXPT(hdmdes[i].cml,ang+180,len-OffsetByXLZX,pt,prjml);
				CenPt.x = pt[1] - LeftLowPt[1];
				CenPt.y = pt[0] - LeftLowPt[0] ;
				INTERS(&Ept.x,&Ept.y,hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].x,hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].y,
					hdmdes[i+1].PTRPTL[hdmdes[i+1].Lnpt-1].x,hdmdes[i+1].PTRPTL[hdmdes[i+1].Lnpt-1].y,
					ptmp.y,ptmp.x,CenPt.x,CenPt.y);
				dx=Ept.x-CenPt.x;
				dy=Ept.y-CenPt.y;
				len = 2*sqrt(dx*dx+dy*dy)/3;
				if(len<0.01)
					len = 0.01;
				Spt.x = CenPt.x + len*sin(ptmp.a+PI);
				Spt.y = CenPt.y + len*cos(ptmp.a+PI);				
			}
			CenPt.z = hdmdes[i].PTRPTL[0].z;
			dx = CenPt.x-Ept.x;
			dy = CenPt.y-Ept.y;
			len = sqrt(dx*dx+dy*dy);
			if(len < 0.01)
			{
				Ept.x = CenPt.x + 5.0*sin(ptmp.a-0.5*PI);
				Ept.y = CenPt.y + 5.0*cos(ptmp.a-0.5*PI);
			}
			Cal_ZPJD(-1,1,CenPt,Spt,Ept,ptarr);//计算左侧桥尾锥坡脚点
		}		
	}
	else//右侧
	{
		if(hdmdes[i].YDMXZ==-1)//右侧桥头锥坡
		{
			if(hdmdes[i].Rnpt==0)
				return;
			ang = FindBriAng(hdmdes[i].cml);
			if(fabs(ang-90)<0.1)
			{
				if(2*fabs(hdmdes[i].RPTR[hdmdes[i].Rnpt-1].x)/3 > 20)//2/3坡脚点至路肩点距离
					k = 1;
				else
					k = fabs(hdmdes[i].RPTR[hdmdes[i].Rnpt-1].x)/30; 

				ptmp.lc = hdmdes[i+1].cml;
				if(fabs(hdmdes[i+1].cml-hdmdes[i].cml)<0.01)
					ptmp.lc = hdmdes[i].cml+5.0;
				pm->xlpoint_pz(&ptmp);
				CalOffsetByXLZX(ptmp.lc,NYLMK,YLMK,OffsetByXLZX);
				PZLJZX = ptmp;
				PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
				PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
				// 				ptmp.x = ptmp.x - LeftLowPt[0];
				// 				ptmp.y = ptmp.y - LeftLowPt[1];
				ptmp.x = PZLJZX.x - LeftLowPt[0];
				ptmp.y = PZLJZX.y - LeftLowPt[1];
				Ljpt[X] = ptmp.y + fabs(hdmdes[i+1].RHFR[8])*sin(ptmp.a+0.5*PI);
				Ljpt[Y] = ptmp.x + fabs(hdmdes[i+1].RHFR[8])*cos(ptmp.a+0.5*PI);
				Spt.x = hdmdes[i].PTRPTR[4].x + k*(Ljpt[X]-hdmdes[i].PTRPTR[4].x);
				Spt.y = hdmdes[i].PTRPTR[4].y + k*(Ljpt[Y]-hdmdes[i].PTRPTR[4].y);
				CenPt.x = hdmdes[i].PTRPTR[4].x;
				CenPt.y = hdmdes[i].PTRPTR[4].y;
				Ept.x = hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].x;
				Ept.y = hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].y;
			}
			else
			{
				ptmp.lc = hdmdes[i].cml;
				pm->xlpoint_pz(&ptmp);
				OffsetByXLZX = hdmdes[i].OffsetByXLZX;
				PZLJZX = ptmp;
				PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
				PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
				// 				ptmp.x = ptmp.x - LeftLowPt[0];
				// 				ptmp.y = ptmp.y - LeftLowPt[1];
				ptmp.x = PZLJZX.x - LeftLowPt[0];
				ptmp.y = PZLJZX.y - LeftLowPt[1];
				double pt[2],prjml;
				len =  fabs(hdmdes[i].RHFR[8]);
				if(len<0.001)
					len=0.001;
				pm->CalPZXPT(hdmdes[i].cml,ang,len+OffsetByXLZX,pt,prjml);
				CenPt.x = pt[1] - LeftLowPt[1];
				CenPt.y = pt[0] - LeftLowPt[0];
				INTERS(&Ept.x,&Ept.y,hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].x,hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].y,
					hdmdes[i-1].PTRPTR[hdmdes[i-1].Rnpt-1].x,hdmdes[i-1].PTRPTR[hdmdes[i-1].Rnpt-1].y,
					ptmp.y,ptmp.x,CenPt.x,CenPt.y);
				dx=Ept.x-CenPt.x;
				dy=Ept.y-CenPt.y;
				len = 2*sqrt(dx*dx+dy*dy)/3;
				if(len<0.01)
					len = 5.0;
				Spt.x = CenPt.x + len*sin(ptmp.a);
				Spt.y = CenPt.y + len*cos(ptmp.a);	
			}
			CenPt.z = hdmdes[i].PTRPTR[0].z;
			dx = CenPt.x-Ept.x;
			dy = CenPt.y-Ept.y;
			len = sqrt(dx*dx+dy*dy);
			if(len < 0.01)
			{
				Ept.x = CenPt.x + 5.0*sin(ptmp.a+0.5*PI);
				Ept.y = CenPt.y + 5.0*cos(ptmp.a+0.5*PI);
			}
			Cal_ZPJD(1,-1,CenPt,Spt,Ept,ptarr);//计算右侧桥头锥坡脚点				
		}
		else if(hdmdes[i].YDMXZ==1)//右侧桥尾锥坡
		{
			if(hdmdes[i].Rnpt==0)
				return;
			ang = FindBriAng(hdmdes[i].cml);
			if(fabs(ang-90)<0.1)
			{
				if(2*fabs(hdmdes[i].RPTR[hdmdes[i].Rnpt-1].x)/3 > 20)//2/3坡脚点至路肩点距离
					k = 1;
				else
					k =  fabs(hdmdes[i].RPTR[hdmdes[i].Rnpt-1].x)/30;
				ptmp.lc = hdmdes[i-1].cml;
				if(fabs(hdmdes[i-1].cml-hdmdes[i].cml)<0.01)
					ptmp.lc = hdmdes[i].cml-5.0;
				pm->xlpoint_pz(&ptmp);
				CalOffsetByXLZX(ptmp.lc,NYLMK,YLMK,OffsetByXLZX);
				PZLJZX = ptmp;
				PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
				PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
				// 				ptmp.x = ptmp.x - LeftLowPt[0];
				// 				ptmp.y = ptmp.y - LeftLowPt[1];
				ptmp.x = PZLJZX.x - LeftLowPt[0];
				ptmp.y = PZLJZX.y - LeftLowPt[1];
				Ljpt[X] = ptmp.y + fabs(hdmdes[i-1].RHFR[8])*sin(ptmp.a+0.5*PI);
				Ljpt[Y] = ptmp.x + fabs(hdmdes[i-1].RHFR[8])*cos(ptmp.a+0.5*PI);		
				Spt.x = hdmdes[i].PTRPTR[4].x - k*(hdmdes[i].PTRPTR[4].x-Ljpt[X]);
				Spt.y = hdmdes[i].PTRPTR[4].y - k*(hdmdes[i].PTRPTR[4].y-Ljpt[Y]);
				CenPt.x = hdmdes[i].PTRPTR[4].x;
				CenPt.y = hdmdes[i].PTRPTR[4].y;
				Ept.x = hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].x;
				Ept.y = hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].y;
			}
			else
			{
				ptmp.lc = hdmdes[i].cml;
				pm->xlpoint_pz(&ptmp);
				OffsetByXLZX = hdmdes[i].OffsetByXLZX;
				PZLJZX = ptmp;
				PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
				PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);

				// 				ptmp.x = ptmp.x - LeftLowPt[0];
				// 				ptmp.y = ptmp.y - LeftLowPt[1];		
				ptmp.x = PZLJZX.x - LeftLowPt[0];
				ptmp.y = PZLJZX.y - LeftLowPt[1];	
				double pt[2],prjml;
				len =  fabs(hdmdes[i].RHFR[8]);
				if(len<0.001)
					len=0.001;
				pm->CalPZXPT(hdmdes[i].cml,ang,len+OffsetByXLZX,pt,prjml);
				CenPt.x = pt[1] - LeftLowPt[1];
				CenPt.y = pt[0] - LeftLowPt[0];
				INTERS(&Ept.x,&Ept.y,hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].x,hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].y,
					hdmdes[i+1].PTRPTR[hdmdes[i+1].Rnpt-1].x,hdmdes[i+1].PTRPTR[hdmdes[i+1].Rnpt-1].y,
					ptmp.y,ptmp.x,CenPt.x,CenPt.y);
				dx=Ept.x-CenPt.x;
				dy=Ept.y-CenPt.y;
				len = 2*sqrt(dx*dx+dy*dy)/3;
				if(len<0.01)
					len = 5.0;
				Spt.x = CenPt.x + len*sin(ptmp.a+PI);
				Spt.y = CenPt.y + len*cos(ptmp.a+PI);			
			}	
			CenPt.z = hdmdes[i].PTRPTR[0].z;
			dx = CenPt.x-Ept.x;
			dy = CenPt.y-Ept.y;
			len = sqrt(dx*dx+dy*dy);
			if(len < 0.01)
			{
				Ept.x = CenPt.x + 5.0*sin(ptmp.a+0.5*PI);
				Ept.y = CenPt.y + 5.0*cos(ptmp.a+0.5*PI);
			}			
			Cal_ZPJD(1,1,CenPt,Spt,Ept,ptarr);//计算右侧桥尾锥坡脚点				
		}
	}
}

//绘桥头锥坡
void CDraw3D::draw_BrigeSlop()
{
	AcGePoint3d Cenpt,lpt,rpt;
	double SlopDegree=1,dx,dy;
	AcGePoint3dArray ptArr;
	int i,j;
	xlpoint PZtmp;
	xlpoint PZLJZX;
	double OffsetByXLZX = 0.0;
	for(i=0; i<NCRS; i++)
	{
		PZtmp.lc = hdmdes[i].cml;
		pm->xlpoint_pz(&PZtmp);
		OffsetByXLZX = hdmdes[i].OffsetByXLZX;
		PZLJZX = PZtmp;
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
		// 		PZtmp.x = PZtmp.x - LeftLowPt[0];
		// 		PZtmp.y = PZtmp.y - LeftLowPt[1];
		PZtmp.x = PZLJZX.x - LeftLowPt[0];
		PZtmp.y = PZLJZX.y - LeftLowPt[1];
		if(ptArr.length()>0)
			ptArr.removeSubArray(0,ptArr.length()-1);
		if(hdmdes[i].ZDMXZ==-1 && hdmdes[i].YDMXZ==-1)//桥头
		{	
			CalBriSlopPt(i,-1,ptArr);
			//			Cal_ZPJD(-1,hdmdes[i].PTRPTL[4],PZtmp.a,ptArr,SlopDegree);//计算左侧桥头锥坡脚点
			for(j=0; j<6;j++)
				CreateFace(ptArr[j],hdmdes[i].PTRPTL[4],ptArr[j+1],2,L"桥梁锥坡");
			lpt = ptArr[6];//左侧向前坡脚点	

			if(ptArr.length()>0)
				ptArr.removeSubArray(0,ptArr.length()-1);
			CalBriSlopPt(i,1,ptArr);
			//			Cal_ZPJD(1,hdmdes[i].PTRPTR[4],PZtmp.a,ptArr,SlopDegree);//计算右侧桥头锥坡脚点			
			for(j=0; j<6; j++)
				CreateFace(hdmdes[i].PTRPTR[4],ptArr[j],ptArr[j+1],2,L"桥梁锥坡");
			rpt = ptArr[6];//右侧向前坡脚点
			CreateAFace(hdmdes[i].PTRPTR[4],rpt,lpt,hdmdes[i].PTRPTL[4],2,L"桥梁锥坡");//中间锥坡面
		}
		else if(hdmdes[i].ZDMXZ==-1)
		{
			CalBriSlopPt(i,-1,ptArr);
			//			Cal_ZPJD(-1,hdmdes[i].PTRPTL[4],PZtmp.a,ptArr,SlopDegree);//计算左侧桥头锥坡脚点
			for(j=0; j<6;j++)
				CreateFace(ptArr[j],hdmdes[i].PTRPTL[4],ptArr[j+1],2,L"桥梁锥坡");			
		}
		else if(hdmdes[i].YDMXZ==-1)
		{
			CalBriSlopPt(i,1,ptArr);
			//			Cal_ZPJD(1,hdmdes[i].PTRPTR[4],PZtmp.a,ptArr,SlopDegree);//计算右侧桥头锥坡脚点			
			for(j=0; j<6; j++)
				CreateFace(hdmdes[i].PTRPTR[4],ptArr[j],ptArr[j+1],2,L"桥梁锥坡");			
		}

		if(hdmdes[i].ZDMXZ==1 && hdmdes[i].YDMXZ==1)//桥尾
		{
			CalBriSlopPt(i,-1,ptArr);
			//			Cal_ZPJD(1,hdmdes[i].PTRPTL[4],PZtmp.a+PI,ptArr,SlopDegree);//计算左侧桥头锥坡脚点
			for(j=0; j<6;j++)
				CreateFace(ptArr[j],ptArr[j+1],hdmdes[i].PTRPTL[4],2,L"桥梁锥坡");
			lpt = ptArr[6];//左侧向前坡脚点

			if(ptArr.length()>0)
				ptArr.removeSubArray(0,ptArr.length()-1);
			CalBriSlopPt(i,1,ptArr);
			//			Cal_ZPJD(-1,hdmdes[i].PTRPTR[4],PZtmp.a+PI,ptArr,SlopDegree);//计算右侧桥头锥坡脚点			
			for(j=0; j<6; j++)
				CreateFace(hdmdes[i].PTRPTR[4],ptArr[j+1],ptArr[j],2,L"桥梁锥坡");
			rpt = ptArr[6];//右侧向前坡脚点
			CreateAFace(hdmdes[i].PTRPTL[4],lpt,rpt,hdmdes[i].PTRPTR[4],2,L"桥梁锥坡");//中间锥坡面
		}	
		else if(hdmdes[i].ZDMXZ==1)
		{
			CalBriSlopPt(i,-1,ptArr);
			//			Cal_ZPJD(1,hdmdes[i].PTRPTL[4],PZtmp.a+PI,ptArr,SlopDegree);//计算左侧桥头锥坡脚点
			for(j=0; j<6;j++)
				CreateFace(ptArr[j],ptArr[j+1],hdmdes[i].PTRPTL[4],2,L"桥梁锥坡");
		}
		else if(hdmdes[i].YDMXZ==1)
		{
			CalBriSlopPt(i,1,ptArr);
			//			Cal_ZPJD(-1,hdmdes[i].PTRPTR[4],PZtmp.a+PI,ptArr,SlopDegree);//计算右侧桥头锥坡脚点			
			for(j=0; j<6; j++)
				CreateFace(hdmdes[i].PTRPTR[4],ptArr[j+1],ptArr[j],2,L"桥梁锥坡");
		}
	}

	return ;
}

int CDraw3D::draw_Terrain()
{
	int i,v1,v2,v3;
	FILE *fpr;
	ACHAR fname[80];
	double step,pt1[3],pt2[3],pt3[3];	

	acedSetStatusBarProgressMeter(L"正在显示地形三维模型...",0,TriSum);	

	_stprintf(fname, L"%s\\3d\\NetForWatch.txt", Cworkdir);
	fpr = _wfopen(fname, L"w");
	fwprintf(fpr, L"%d\n", TriSum);

		

	

	step=TriSum/15;
	for(i = 0 ; i < TriSum ; i++)
	{		
		v1 = TriList[i].vertex[0]; v2=TriList[i].vertex[1]; v3=TriList[i].vertex[2];

		pt1[0] = ptdata[v1].pt[0],pt1[1] = ptdata[v1].pt[1],pt1[2] = ptdata[v1].pt[2];
		pt2[0] = ptdata[v2].pt[0],pt2[1] = ptdata[v2].pt[1],pt2[2] = ptdata[v2].pt[2];
		pt3[0] = ptdata[v3].pt[0],pt3[1] = ptdata[v3].pt[1],pt3[2] = ptdata[v3].pt[2];
		CreateFace(pt1,pt2,pt3,3,L"地面模型");

		fwprintf(fpr, L"Num:\t%d\n", i);
		fwprintf(fpr, L"%lf\t%lf\t%lf\n", i, pt1[0], pt1[1], pt1[2]);
		fwprintf(fpr, L"%lf\t%lf\t%lf\n", i, pt2[0], pt2[1], pt2[2]);
		fwprintf(fpr, L"%lf\t%lf\t%lf\n", i, pt3[0], pt3[1], pt3[2]);

		if(i>step)
		{
			acedSetStatusBarProgressMeterPos(i);
			step += TriSum/15;
		}     
	}
	acedSetStatusBarProgressMeterPos(TriSum);
	acedRestoreStatusBar();

	fclose(fpr);

	_stprintf(fname, L"%s\\3d\\PtForWatch.txt", Cworkdir);
	fpr = _wfopen(fname, L"w");
	fwprintf(fpr, L"%d\n", PTSUM);
	for (i = 0; i < PTSUM; i++)
	{
		fwprintf(fpr, L"Num:\t%d\t%lf\t%lf\t%lf\n", i, ptdata[i].pt[0], ptdata[i].pt[1], ptdata[i].pt[2]);
	}
	fclose(fpr);

	_stprintf(fname, L"%s\\3d\\TriForWatch.txt", Cworkdir);
	fpr = _wfopen(fname, L"w");
	fwprintf(fpr, L"%d\n", TriSum);
	for (i = 0; i < TriSum; i++)
	{
		fwprintf(fpr, L"Num:\t%d\t%d\t%d\t%d\n", i, TriList[i].vertex[0], TriList[i].vertex[1], TriList[i].vertex[2]);
	}
	fclose(fpr);

	return 1;
}


void CDraw3D::CalOffsetByXLZX(double cml,int NLMK,LMKdata lmk[],double& OffsetByXLZX)
{
	double k;
	double addwid;

	int i;
	for( i=0;i<NLMK;i++)
	{
		if(cml<lmk[i].cml+0.1)
			break;
	}

	if(i>0 && i<NLMK && fabs(lmk[i].cml-lmk[i-1].cml)>0.0001)
		k = (cml - lmk[i-1].cml)/(lmk[i].cml-lmk[i-1].cml);
	else 
		k = 0;

	if(i==0)
		i = 1;
	else if(i==NLMK)
		i = NLMK-1;

	OffsetByXLZX = lmk[i-1].OffsetByXLZX + k*(lmk[i].OffsetByXLZX- lmk[i-1].OffsetByXLZX);
}

void CDraw3D::ReadLMK(CString LMKTABname)
{
	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(pm->mdbname,LMKTABname,pm->RoadName);

	if(xlmdb.NLMK<1)
		return;

	if(LMKTABname == "左路面宽分段表")
	{
		NZLMK = xlmdb.NLMK;
		ZLMK = new LMKdata[xlmdb.NLMK];
		for(int i=0;i<xlmdb.NLMK;i++)
		{
			ZLMK[i] = xlmdb.lmk[i];	
			ZLMK[i].cml = pm->TYLC(ZLMK[i].xlc);
		}
	}
	else
	{
		NYLMK = xlmdb.NLMK;
		YLMK = new LMKdata[xlmdb.NLMK];
		for(int i=0;i<xlmdb.NLMK;i++)
		{
			YLMK[i] = xlmdb.lmk[i];	
			YLMK[i].cml = pm->TYLC(YLMK[i].xlc);
		}
	}

	if(xlmdb.lmk)
		delete []xlmdb.lmk;
	xlmdb.lmk = NULL;
	xlmdb.NLMK = 0;	
}