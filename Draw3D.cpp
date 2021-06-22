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
	//    char Attribute;//���ԣ������������Ƿ���Ч,'Y'��Ч;'N'��Ч
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
	int len,j,k;  //BrgArray 0��Ӧ��β�� KNum-1��Ӧ����ͷ��

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

//�ڲ� ��Ƹ� ����·���
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

//ang �������� һ��Ϊ���� 90
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
		CalDeshLmk(BrgLArr[j].sml,zmh,ymh,Zwd,Ywd); //������Ƹ�	��·��� ��·��� 
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
		if(XZ=="���")
			Ywd = 0.0;
		else if(XZ=="�ҷ�")
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

		if(Zwd + Ywd < 20.0 || XZ!="ȫ��")//����˫��
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
//��hdmresult�ļ�
void CDraw3D::ReadHdmResultFile()
{
	CString filename;
	FILE *fpr;

	filename =Cworkdir + "\\data\\" + "hdmresult." + Pro + "." + pm->RoadName;
	if((fpr=_wfopen(filename , L"r"))==NULL)
	{
		ads_alert(L"�޷���hdmresult�ļ�!");
		return ;
	}

	fwscanf(fpr,L"%d",&NCRS);
	NCRS = fabs(1.0*NCRS);
	//	ads_printf(L"Ncrs=%d\n",NCRS);
	//���ռ�
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

	if(_tcscmp(pm->RoadName,L"����")!=0)//�����
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
		if(hdmdes[jcross].ZDMXZ==3)//��������
		{
			CreateBriPt(jcross,3,hdmdes[jcross].PTRPTL,-1);
			hdmdes[jcross].Lnpt = hdmdes[jcross].PTRPTL.length();
		}
		if(hdmdes[jcross].YDMXZ==3)
		{
			CreateBriPt(jcross,3,hdmdes[jcross].PTRPTR,1);
			hdmdes[jcross].Rnpt = hdmdes[jcross].PTRPTR.length();			
		}
		if(hdmdes[jcross].ZDMXZ==4)//�������
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

	DBS.Read_XLDbs(pm->mdbname,L"������",pm->RoadName);	

	for(int i=0;i<DBS.BriNum;i++)
	{
		DBS.Bri[i].Scml = pm->TYLC(DBS.Bri[i].StDml);//������
		DBS.Bri[i].Ecml = pm->TYLC(DBS.Bri[i].EdDml);//������
	}

	return;
}

//��������7��  ������ ����·�� 
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

//���������  �������ʶ�Ϊ��� x =�����ȵ�һ�� PZTtmp �������λ��
void CDraw3D::AddSinTunBodyPt(AcGePoint3dArray &Ptarr,int LorR, xlpoint &PZtmp)
{
	AcGePoint3d pt;
	double 	tunwide= 5.40;//����뾶

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

//���������  �������ʶ�Ϊ��� x =�����ȵ�һ�� PZTtmp  �����������λ��  ˫��
void CDraw3D::AddDouTunBodyPt(AcGePoint3dArray &Ptarr,int LorR, xlpoint &PZtmp)
{
	AcGePoint3d pt;
	double 	tunwide= 5.44;//����뾶

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
//���������ĵ�  �������ʶ�Ϊ�� x =�����ȵ�һ�� PZTtmp ��������λ��
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
		&dtmp,&dtmp,//�����
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
	//1.д�������
	fwscanf(fpr,L"%d",&hdmdes[jcross].ZDMXZ);
	int j;
	for(j=0;j<3;j++)
		fwscanf(fpr,L"%lf",&hdmdes[jcross].zlmhd[j]);

	//д·���
	for(j=0;j<5;j++)
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].RHFL[2*j],&hdmdes[jcross].RHFL[2*j+1]);
	//����Ƶ�
	fwscanf(fpr,L"%lf%lf%d",&hdmdes[jcross].Zt,&hdmdes[jcross].ZHc,&hdmdes[jcross].Lnpt);//����
	xlpoint PZtmp;
	PZtmp.lc=hdmdes[jcross].cml;  // ͳһ���
	pm->xlpoint_pz(&PZtmp);
	PZtmp.x = PZtmp.x - LeftLowPt[0];
	PZtmp.y = PZtmp.y - LeftLowPt[1];
	double x,y;
	for(j=0; j<hdmdes[jcross].Lnpt; j++)
	{
		fwscanf(fpr,L"%lf%lf",&x,&y);

		//////////////////////////////////////////////////////////////////////////������Ҫ
		// 		if((fabs(hdmdes[jcross].Zt-1.0)<0.1) && hdmdes[jcross].ZCG_GDR>0.1 && hdmdes[jcross].Lnpt>16)//�ڷ��ҵ��ι�
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
	// 	if((fabs(hdmdes[jcross].Zt-1.0)<0.1)&& hdmdes[jcross].ZCG_GDR>0.1 && hdmdes[jcross].Lnpt>16)//�ڷ��ҵ��ι�
	// 		hdmdes[jcross].Lnpt = hdmdes[jcross].Lnpt-10;

	if(hdmdes[jcross].Lnpt==0)
	{
		hdmdes[jcross].Lnpt = 5;
		for(j=0;j<5;j++)//·���
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
	//��ˮ����
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
	//���ˮ��
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
	//2.д�����Ҳ�
	//д·���
	fwscanf(fpr,L"%d",&hdmdes[jcross].YDMXZ);
	for(j=0;j<3;j++)
		fwscanf(fpr,L"%lf",&hdmdes[jcross].ylmhd[j]);

	for(j=0;j<5;j++)
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].RHFR[2*j],&hdmdes[jcross].RHFR[2*j+1]);
	//����Ƶ�
	fwscanf(fpr,L"%lf%lf%d ",&hdmdes[jcross].Yt,&hdmdes[jcross].YHc,&hdmdes[jcross].Rnpt);
	for(j=0; j<hdmdes[jcross].Rnpt; j++)
	{
		double x,y;
		fwscanf(fpr,L"%lf%lf",&x,&y);

		// 		if((fabs(hdmdes[jcross].Yt-1.0)<0.1) && hdmdes[jcross].YCG_GDR>0.1 && hdmdes[jcross].Rnpt>16)//�ڷ��ҵ��ι�
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

	}	//��ˮ����
	// 	if((fabs(hdmdes[jcross].Yt-1.0)<0.1) && hdmdes[jcross].YCG_GDR>0.1 && hdmdes[jcross].Rnpt>16)//�ڷ��ҵ��ι�
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
	//�ҽ�ˮ��
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

	if(hdmdes[jcross].IfHasZlink )//���й�����·
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
		//1.д�������
		fwscanf(fpr,L"%d",&itmp);
		//д·���
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&dtmp);
		//д·���
		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//����Ƶ�
		fwscanf(fpr,L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//����
		for(j=0; j<itmp; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//��ˮ����
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}
		//���ˮ��
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}
		//2.д�����Ҳ�
		//д·���
		fwscanf(fpr,L"%d",&itmp);
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&dtmp);

		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//����Ƶ�
		fwscanf(fpr,L"%lf%lf%d ",&dtmp,&dtmp,&num);
		for(j=0; j<num; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);

		//��ˮ����
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//�ҽ�ˮ��
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}		
	}

	if(hdmdes[jcross].IfHasYlink )//���й�����·
	{
		double dtmp;
		int itmp,jsgnum,num;

		fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
			&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,
			tmp			
			);
		//hdmdes[jcross].zlink->glRoadName = tmp;
		//1.д�������
		fwscanf(fpr,L"%d",&itmp);
		//д·���
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&dtmp);
		//д·���
		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//����Ƶ�
		fwscanf(fpr,L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//����
		for(j=0; j<itmp; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//��ˮ����
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}
		//���ˮ��
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		}
		//2.д�����Ҳ�
		//д·���
		fwscanf(fpr,L"%d",&itmp);
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&dtmp);

		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//����Ƶ�
		fwscanf(fpr,L"%lf%lf%d ",&dtmp,&dtmp,&num);
		for(j=0; j<num; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);

		//��ˮ����
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
			fwscanf(fpr,L"%lf%lf ",&dtmp,&dtmp);
		//�ҽ�ˮ��
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
	_stprintf(pszLabel,L"���ڻ��Ƶ�·ģ��,���Ժ�");
	acedSetStatusBarProgressMeter(pszLabel,0,100);

	for(i=1;i<NCRS;i++)
	{
		pos=(int)i*100.0/NCRS;
		acedSetStatusBarProgressMeterPos(pos);
		/*
		if(hdmdes[i].cml<Scml || hdmdes[i].cml>Ecml+10)
		continue;*/
		//(hdmdes[i-1].DorS==1&&hdmdes[i].DorS==1)
		//������ʾ����
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
		//���Ҳ��ڷ�ʾ����
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
	DrawAllSlop(i);//������׶��
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
//������������
void CDraw3D::CalTWZeroPt(double bml,double eml,double Bdeltah,double Edeltah,double &k,double &MidMl)
{
	if(Bdeltah*Edeltah<0.0)
		k = fabs(Edeltah)/fabs(Bdeltah);
	else
		k = 1;
	//�ҳ�·���������
	MidMl = (eml + k*bml)/(1+k);
}

//���������Ptarr
void CDraw3D::CreateTunPt(int jcross,AcGePoint3dArray &Ptarr,int ZorY)
{
	xlpoint PZtmp,PZ;
	double x,y,HarfWid;
	double OffsetByXLZX = 0.0;
	xlpoint PZLJZX;//·�����ĵ�
	PZtmp.lc=hdmdes[jcross].cml;  // ͳһ���
	pm->xlpoint_pz(&PZtmp);
	OffsetByXLZX = hdmdes[jcross].OffsetByXLZX;
	PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
	PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
	// 	PZtmp.x = PZtmp.x - LeftLowPt[0];
	// 	PZtmp.y = PZtmp.y - LeftLowPt[1];
	PZtmp.x = PZLJZX.x - LeftLowPt[0];
	PZtmp.y = PZLJZX.y - LeftLowPt[1];
	if(hdmdes[jcross].RHFL[8]+hdmdes[jcross].RHFR[8]<20.0)//�������
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
		if(ZorY==-1)//��ප
		{			
			PZ.x = PZtmp.x + 0.5*hdmdes[jcross].RHFL[8]*cos(PZtmp.a-0.5*PI);
			PZ.y = PZtmp.y + 0.5*hdmdes[jcross].RHFL[8]*sin(PZtmp.a-0.5*PI);
			PZ.a = PZtmp.a;
			AddDouTunBodyPt(Ptarr,ZorY,PZ);
		}
		else//�Ҳප
		{
			PZ.x = PZtmp.x + 0.5*hdmdes[jcross].RHFR[8]*cos(PZtmp.a+0.5*PI);
			PZ.y = PZtmp.y + 0.5*hdmdes[jcross].RHFR[8]*sin(PZtmp.a+0.5*PI);
			PZ.a = PZtmp.a;
			AddDouTunBodyPt(Ptarr,ZorY,PZ);
		}
	}
}
//���������湹��Ptarr brixzΪ��������
void CDraw3D::CreateBriPt(int jcross,int brixz,AcGePoint3dArray &Ptarr,int ZorY)
{
	xlpoint PZtmp,PZ;
	double x,y,HarfWid;
	xlpoint PZLJZX;
	double OffsetByXLZX = 0.0;

	if(hdmdes[jcross].ZDMXZ==brixz&&hdmdes[jcross].YDMXZ==brixz)//���Ҳ���������������ͬ
	{
		PZtmp.lc=hdmdes[jcross].cml;  // ͳһ���
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
	else if(hdmdes[jcross].ZDMXZ==brixz&&hdmdes[jcross].YDMXZ!=brixz)//���Ϊ���� �Ҳ������
	{
		PZtmp.lc=hdmdes[jcross].cml;  // ͳһ���
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
	else if(hdmdes[jcross].ZDMXZ!=brixz&&hdmdes[jcross].YDMXZ==brixz)//�������� �Ҳ�Ϊ����
	{
		PZtmp.lc=hdmdes[jcross].cml;  // ͳһ���
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
		//ǰ����涼Ϊ���ǰ����Ϊһ�����
			DrawPartFillBetweenTwoMl(ZorY,qnpt,hnpt,qPtarr,hPtarr);				

	else if(qtw>0.001 && htw<0.001&&qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)//��-��
	{
		//���������				
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
		//���ڷ�hdmdes[i-1].cml --  MidMl;
		DrawCutPartBetweenTwoMl(ZorY,qnpt,MidNum,qPtarr,MidArr);
		//���MidMl-hdmdes[i].cml;
		DrawPartFillBetweenTwoMl(ZorY,MidNum,hnpt,MidArr,hPtarr);								
	}
	else if(fabs(1.0*qdmxz)>0 || fabs(1.0*hdmxz)>0)//�Ż����
	{
		if(qdmxz==-1&&hdmxz==1)//��ͷ��β
		{
			AcGePoint3dArray BHArr,BTArr;      				
			for(j=0;j<5;j++)
				BHArr.append(qPtarr[j]);
			CreateBriPt(i-1,-1,BHArr,ZorY);	//	��ͷ					
			for(j=0;j<5;j++)
				BTArr.append(hPtarr[j]);
			CreateBriPt(i,1,BTArr,ZorY);  // ��β
			DrawBriBody(ZorY,BHArr,BTArr);	
		}
		else if(qdmxz==-1&&hdmxz==3)//��ͷ����
		{
			AcGePoint3dArray BHArr;	
			for(j=0;j<5;j++)
				BHArr.append(qPtarr[j]);
			CreateBriPt(i-1,-1,BHArr,ZorY);	//	��ͷ				
			DrawBriBody(ZorY,BHArr,hPtarr);
		}
		else if(qdmxz==3&&hdmxz==1)//������β
		{
			AcGePoint3dArray BTArr;		
			for(j=0;j<5;j++)
				BTArr.append(hPtarr[j]);
			CreateBriPt(i,1,BTArr,ZorY);  // ��β
			DrawBriBody(ZorY,qPtarr,BTArr);
		}
		else if(qdmxz==3&&hdmxz==3)//��������
			DrawBriBody(ZorY,qPtarr,hPtarr);
		else if(qdmxz==4&&hdmxz==4)//��������
			DrawTunBody(ZorY,qPtarr,hPtarr);
		else if(qdmxz==-2&&hdmxz==4)//��ͷ����
			DrawTunDoor(ZorY,i-1,i);
		else if(qdmxz==4&&hdmxz==2)//������β
			DrawTunDoor(ZorY,i,i-1);
		else if(qdmxz==2&&hdmxz==-1)//��β��ͷ
			DrawLM(ZorY,qPtarr,hPtarr);
		else if(qdmxz==1&&hdmxz==-2)//��β��ͷ
			DrawLM(ZorY,qPtarr,hPtarr);
	}       
	//���ڷ�ʾ����
	if(qtw>0.001 &&htw>0.001 && qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)//ǰ����涼Ϊ�ڷ���ǰ����Ϊһ�����
		DrawCutPartBetweenTwoMl(ZorY,qnpt,hnpt,qPtarr,hPtarr);			
	else if(qtw<0.001 && htw>0.001&& qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)//��-��
	{
		//���������
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

		//���hdmdes[i-1].cml --  MidMl;
		DrawPartFillBetweenTwoMl(ZorY,qnpt,MidNum,qPtarr,MidArr);				
		//���ڷ�MidMl-hdmdes[i].cml;
		DrawCutPartBetweenTwoMl(ZorY,MidNum,hnpt,MidArr,hPtarr);
	}
	return 1;
}

// �涴�� scrossΪ��ͷ��β����� ncrossΪ��һ�����ж����
void CDraw3D::DrawTunDoor(int ZorY,int scross, int ncross)
{
	AcGePoint3dArray InTunArr;//���ڴ���ncross�����Ӧ��
	AcGePoint3dArray OutTunArr;//������Ȧ�� ��InTunArr����
	AcGePoint3d pt;
	xlpoint PZtmp,PZ;
	double x,y,HarfWid,tunwide;
	int i;

	PZtmp.lc=hdmdes[scross].cml;  // ͳһ���
	pm->xlpoint_pz(&PZtmp);
	PZtmp.x = PZtmp.x - LeftLowPt[0];
	PZtmp.y = PZtmp.y - LeftLowPt[1];
	if(ZorY==-1)//���
		for(i=0;i<5;i++)
			InTunArr.append(hdmdes[scross].PTRPTL[i]);
	else
		for(i=0;i<5;i++)
			InTunArr.append(hdmdes[scross].PTRPTR[i]);
	if(hdmdes[scross].RHFL[8]+hdmdes[scross].RHFR[8]<20.0)//�������
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
		pt.z = InTunArr[7].z;//���������Ȧ�㣬��Ӧ7����Ȧ��
		OutTunArr.append(pt);
		pt.y = PZ.x + (5.4+0.6)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (5.4+0.6)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5;//���������Ȧ�㣬��Ӧ8����Ȧ��
		OutTunArr.append(pt);
		pt.y = PZ.x + (tunwide+0.6)*cos(PI/10)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (tunwide+0.6)*cos(PI/10)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6)*sin(PI/10);//���������Ȧ�㣬��Ӧ9����Ȧ��
		OutTunArr.append(pt);
		pt.y = PZ.x + (tunwide+0.6)*cos(PI/5)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (tunwide+0.6)*cos(PI/5)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6)*sin(PI/5);//���������Ȧ�㣬��Ӧ10����Ȧ��
		OutTunArr.append(pt);
		pt.y = PZ.x + (tunwide+0.6)*cos(3*PI/10)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (tunwide+0.6)*cos(3*PI/10)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6)*sin(3*PI/10);//���������Ȧ�㣬��Ӧ11����Ȧ��
		OutTunArr.append(pt);
		pt.y = PZ.x + (tunwide+0.6)*cos(2*PI/5)*cos(PZtmp.a+ZorY*0.5*PI);
		pt.x = PZ.y + (tunwide+0.6)*cos(2*PI/5)*sin(PZtmp.a+ZorY*0.5*PI);
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6)*sin(2*PI/5);//���������Ȧ�㣬��Ӧ12����Ȧ��
		OutTunArr.append(pt);
		pt.y = PZ.x;
		pt.x = PZ.y;
		pt.z = InTunArr[0].z+1.5+(tunwide+0.6);//���������Ȧ�㣬��Ӧ13����Ȧ��
		OutTunArr.append(pt);
		//����Ź�Ȧ
		for(i=0;i<OutTunArr.length()-1;i++)
			CreateAFace(OutTunArr[i],OutTunArr[i+1],InTunArr[i+8],InTunArr[i+7],1,L"�����Ȧ");
		Draw_TunBZ(ZorY,scross,OutTunArr);
	}	
	else//˫�����
	{
		tunwide = 5.44;
		if(ZorY==-1)//��ප
		{			
			PZ.x = PZtmp.x + 0.5*hdmdes[scross].RHFL[8]*cos(PZtmp.a-0.5*PI);
			PZ.y = PZtmp.y + 0.5*hdmdes[scross].RHFL[8]*sin(PZtmp.a-0.5*PI);
			PZ.a = PZtmp.a;
			AddDouTunBodyPt(InTunArr,ZorY,PZ);
			DrawTunBody(ZorY,InTunArr,hdmdes[ncross].PTRPTL);
			pt.y = PZ.x + (tunwide+0.6)*cos(12.6*PI/180)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(12.6*PI/180)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[7].z;//���������Ȧ�㣬��Ӧ7����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415;//���������Ȧ�㣬��Ӧ8����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PI/10)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(PI/10)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(PI/10);//���������Ȧ�㣬��Ӧ9����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PI/5)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(PI/5)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(PI/5);//���������Ȧ�㣬��Ӧ10����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(3*PI/10)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(3*PI/10)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(3*PI/10);//���������Ȧ�㣬��Ӧ11����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(2*PI/5)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(2*PI/5)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(2*PI/5);//���������Ȧ�㣬��Ӧ12����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x;
			pt.x = PZ.y;
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6);//���������Ȧ�㣬��Ӧ13����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*sin(63.3*PI/180/3)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(63.3*PI/180/3)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(63.3*PI/180/3);//���������Ȧ�㣬��Ӧ14����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*sin(2*63.3*PI/180/3)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(2*63.3*PI/180/3)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(2*63.3*PI/180/3);//���������Ȧ�㣬��Ӧ15����Ȧ��
			OutTunArr.append(pt);	
			pt.y = PZ.x + (tunwide+0.6)*sin(63.3*PI/180)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(63.3*PI/180)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(63.3*PI/180);//���������Ȧ�㣬��Ӧ16����Ȧ��
			OutTunArr.append(pt);
			pt.z = InTunArr[0].z;//���������Ȧ�㣬��Ӧ17����Ȧ��
			OutTunArr.append(pt);
			//����Ź�Ȧ
			for(i=0;i<OutTunArr.length()-1;i++)
				CreateAFace(OutTunArr[i],OutTunArr[i+1],InTunArr[i+8],InTunArr[i+7],1,L"�����Ȧ");
			AcGePoint3d p1,p2;
			p1.x = PZtmp.y; p1.y = PZtmp.x; p1.z = InTunArr[0].z;
			p2.x = PZtmp.y; p2.y = PZtmp.x; p2.z = OutTunArr[9].z;
			CreateAFace(OutTunArr[9],OutTunArr[10],p1,p2,8,L"�������");
		}
		else//�Ҳප
		{
			PZ.x = PZtmp.x + 0.5*hdmdes[scross].RHFR[8]*cos(PZtmp.a+0.5*PI);
			PZ.y = PZtmp.y + 0.5*hdmdes[scross].RHFR[8]*sin(PZtmp.a+0.5*PI);
			PZ.a = PZtmp.a;
			AddDouTunBodyPt(InTunArr,ZorY,PZ);
			DrawTunBody(ZorY,InTunArr,hdmdes[ncross].PTRPTR);

			pt.y = PZ.x + (tunwide+0.6)*cos(12.6*PI/180)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(12.6*PI/180)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[7].z;//���������Ȧ�㣬��Ӧ7����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415;//���������Ȧ�㣬��Ӧ8����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PI/10)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(PI/10)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(PI/10);//���������Ȧ�㣬��Ӧ9����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(PI/5)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(PI/5)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(PI/5);//���������Ȧ�㣬��Ӧ10����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(3*PI/10)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(3*PI/10)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(3*PI/10);//���������Ȧ�㣬��Ӧ11����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*cos(2*PI/5)*cos(PZ.a+0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*cos(2*PI/5)*sin(PZ.a+0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*sin(2*PI/5);//���������Ȧ�㣬��Ӧ12����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x;
			pt.x = PZ.y;
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6);//���������Ȧ�㣬��Ӧ13����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*sin(63.3*PI/180/3)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(63.3*PI/180/3)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(63.3*PI/180/3);//���������Ȧ�㣬��Ӧ14����Ȧ��
			OutTunArr.append(pt);
			pt.y = PZ.x + (tunwide+0.6)*sin(2*63.3*PI/180/3)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(2*63.3*PI/180/3)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(2*63.3*PI/180/3);//���������Ȧ�㣬��Ӧ15����Ȧ��
			OutTunArr.append(pt);	
			pt.y = PZ.x + (tunwide+0.6)*sin(63.3*PI/180)*cos(PZ.a-0.5*PI);
			pt.x = PZ.y + (tunwide+0.6)*sin(63.3*PI/180)*sin(PZ.a-0.5*PI);
			pt.z = InTunArr[0].z+1.415+(tunwide+0.6)*cos(63.3*PI/180);//���������Ȧ�㣬��Ӧ16����Ȧ��
			OutTunArr.append(pt);
			pt.z = InTunArr[0].z;//���������Ȧ�㣬��Ӧ17����Ȧ��
			OutTunArr.append(pt);			
			//����Ź�Ȧ
			for(i=0;i<OutTunArr.length()-1;i++)
				CreateAFace(OutTunArr[i],OutTunArr[i+1],InTunArr[i+8],InTunArr[i+7],1,L"�����Ȧ");
			AcGePoint3d p1,p2;
			p1.x = PZtmp.y; p1.y = PZtmp.x; p1.z = InTunArr[0].z;
			p2.x = PZtmp.y; p2.y = PZtmp.x; p2.z = OutTunArr[9].z;
			CreateAFace(OutTunArr[9],OutTunArr[10],p1,p2,8,L"�������");			
		}
		Draw_DouTunBZ(ZorY,scross,OutTunArr);
	}
}
//��������� scross ���ڴ������ OutTunArr ��Ȧ���㼯
void CDraw3D::Draw_TunBZ(int ZorY,int scross,AcGePoint3dArray &OutTunArr)
{
	double topht,wid,tunwide,dx,dy;
	char mes[80];
	AcGePoint3d p0,p1,p2,p3;
	int ln;

	tunwide = 5.4;
	if(ZorY==-1)//���
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
		//ˮƽ����
		CreateAFace(p0,p1,p2,p3,8,L"�������");
		//���
		p0.z = OutTunArr[6].z; p1.z = OutTunArr[5].z;		
		CreateAFace(p0,p1,OutTunArr[5],OutTunArr[6],8,L"�������");

		p0.z = OutTunArr[5].z, p1.z = OutTunArr[4].z; 
		CreateAFace(p0,p1,OutTunArr[4],OutTunArr[5],8,L"�������");

		p0.z = OutTunArr[4].z, p1.z = OutTunArr[3].z; 
		CreateAFace(p0,p1,OutTunArr[3],OutTunArr[4],8,L"�������");

		p0.z = OutTunArr[3].z, p1.z = OutTunArr[2].z; 
		CreateAFace(p0,p1,OutTunArr[2],OutTunArr[3],8,L"�������");

		p0.z = OutTunArr[2].z, p1.z = OutTunArr[1].z; 
		CreateAFace(p0,p1,OutTunArr[1],OutTunArr[2],8,L"�������");

		p0.z = OutTunArr[1].z, p1.z = OutTunArr[0].z; 
		CreateAFace(p0,p1,OutTunArr[0],OutTunArr[1],8,L"�������");

		p0.z = OutTunArr[0].z;		p1.z=hdmdes[scross].PTRPTL[4].z;
		p2 = OutTunArr[0], p2.z = OutTunArr[0].z-0.3;
		CreateAFace(p0,p1,p2,OutTunArr[0],8,L"�������");
		if(hdmdes[scross].Zt>0.001)//����ڷ�
		{
			ln = hdmdes[scross].Lnpt-1;
			dx = hdmdes[scross].PTRPTL[ln].x-hdmdes[scross].PTRPTL[4].x;
			dy = hdmdes[scross].PTRPTL[ln].y-hdmdes[scross].PTRPTL[4].y;
			wid = sqrt(dx*dx+dy*dy)+0.1;			
			p0.z = topht;
			xlpoint PZtmp;
			PZtmp.lc=hdmdes[scross].cml;  // ͳһ���
			pm->xlpoint_pz(&PZtmp);
			PZtmp.x = PZtmp.x - LeftLowPt[0];
			PZtmp.y = PZtmp.y - LeftLowPt[1];
			p1.x = hdmdes[scross].PTRPTL[4].x + wid*sin(PZtmp.a-0.5*PI);
			p1.y = hdmdes[scross].PTRPTL[4].y + wid*cos(PZtmp.a-0.5*PI);
			p1.z = topht;
			p2.x = p1.x; p2.y = p1.y; p2.z = hdmdes[scross].PTRPTL[4].z;           						
			CreateAFace(p0,p1,p2,hdmdes[scross].PTRPTL[4],8,L"�������");
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
		//ˮƽ����
		CreateAFace(p0,p1,p2,p3,8,L"�������");
		//���
		p0.z = OutTunArr[6].z; p1.z = OutTunArr[5].z;		
		CreateAFace(p0,p1,OutTunArr[5],OutTunArr[6],8,L"�������");

		p0.z = OutTunArr[5].z, p1.z = OutTunArr[4].z; 
		CreateAFace(p0,p1,OutTunArr[4],OutTunArr[5],8,L"�������");

		p0.z = OutTunArr[4].z, p1.z = OutTunArr[3].z; 
		CreateAFace(p0,p1,OutTunArr[3],OutTunArr[4],8,L"�������");

		p0.z = OutTunArr[3].z, p1.z = OutTunArr[2].z; 
		CreateAFace(p0,p1,OutTunArr[2],OutTunArr[3],8,L"�������");

		p0.z = OutTunArr[2].z, p1.z = OutTunArr[1].z; 
		CreateAFace(p0,p1,OutTunArr[1],OutTunArr[2],8,L"�������");

		p0.z = OutTunArr[1].z, p1.z = OutTunArr[0].z; 
		CreateAFace(p0,p1,OutTunArr[0],OutTunArr[1],8,L"�������");

		p0.z = OutTunArr[0].z;		p1.z=hdmdes[scross].PTRPTR[4].z;
		p2 = OutTunArr[0], p2.z = OutTunArr[0].z-0.3;
		CreateAFace(p0,p1,p2,OutTunArr[0],8,L"�������");
		if(hdmdes[scross].Zt>0.001)//����ڷ�
		{
			ln = hdmdes[scross].Rnpt-1;
			dx = hdmdes[scross].PTRPTR[ln].x-hdmdes[scross].PTRPTR[4].x;
			dy = hdmdes[scross].PTRPTR[ln].y-hdmdes[scross].PTRPTR[4].y;
			wid = sqrt(dx*dx+dy*dy)+0.1;
			if(wid>40.0)
				wid = 40;
			p0.z = topht;
			xlpoint PZtmp;
			PZtmp.lc=hdmdes[scross].cml;  // ͳһ���
			pm->xlpoint_pz(&PZtmp);
			PZtmp.x = PZtmp.x - LeftLowPt[0];
			PZtmp.y = PZtmp.y - LeftLowPt[1];			
			p1.x = hdmdes[scross].PTRPTR[4].x + wid*sin(PZtmp.a+0.5*PI);
			p1.y = hdmdes[scross].PTRPTR[4].y + wid*cos(PZtmp.a+0.5*PI);
			p1.z = topht;
			p2.x = p1.x; p2.y = p1.y; p2.z = hdmdes[scross].PTRPTR[4].z;           						
			CreateAFace(p0,p1,p2,hdmdes[scross].PTRPTR[4],8,L"�������");
		}		
	}
}

//��˫��������� scross ���ڴ������ OutTunArr ��Ȧ���㼯
void CDraw3D::Draw_DouTunBZ(int ZorY,int scross,AcGePoint3dArray &OutTunArr)
{
	double topht,wid,tunwide,dx,dy;
	char mes[80];
	AcGePoint3d p0,p1,p2,p3;
	int ln;
	xlpoint PZtmp;


	PZtmp.lc=hdmdes[scross].cml;  // ͳһ���
	pm->xlpoint_pz(&PZtmp);//��·���ĵ�
	PZtmp.x = PZtmp.x - LeftLowPt[0];
	PZtmp.y = PZtmp.y - LeftLowPt[1];
	tunwide = 5.44;
	if(ZorY==-1)//���
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
		//ˮƽ����
		CreateAFace(p0,p1,p2,p3,8,L"�������");
		//���
		p0.z = OutTunArr[6].z; p1.z = OutTunArr[5].z;		
		CreateAFace(p0,p1,OutTunArr[5],OutTunArr[6],8,L"�������");		
		p0.z = OutTunArr[5].z, p1.z = OutTunArr[4].z; 
		CreateAFace(p0,p1,OutTunArr[4],OutTunArr[5],8,L"�������");		
		p0.z = OutTunArr[4].z, p1.z = OutTunArr[3].z; 
		CreateAFace(p0,p1,OutTunArr[3],OutTunArr[4],8,L"�������");		
		p0.z = OutTunArr[3].z, p1.z = OutTunArr[2].z; 
		CreateAFace(p0,p1,OutTunArr[2],OutTunArr[3],8,L"�������");		
		p0.z = OutTunArr[2].z, p1.z = OutTunArr[1].z; 
		CreateAFace(p0,p1,OutTunArr[1],OutTunArr[2],8,L"�������");		
		p0.z = OutTunArr[1].z, p1.z = OutTunArr[0].z; 
		CreateAFace(p0,p1,OutTunArr[0],OutTunArr[1],8,L"�������");
		p0.z = OutTunArr[0].z;		p1.z=hdmdes[scross].PTRPTL[4].z;
		p2 = OutTunArr[0], p2.z = OutTunArr[0].z-0.3;
		CreateAFace(p0,p1,p2,OutTunArr[0],8,L"�������");//��������

		p0.x = PZtmp.y, p0.y = PZtmp.x, p0.z=OutTunArr[6].z;
		p1.x = PZtmp.y, p1.y = PZtmp.x, p1.z=OutTunArr[7].z;
		CreateAFace(p0,OutTunArr[6],OutTunArr[7],p1,8,L"�������");
		p0.z = p1.z, p1.z = OutTunArr[8].z;
		CreateAFace(p0,OutTunArr[7],OutTunArr[8],p1,8,L"�������");
		p0.z = p1.z, p1.z = OutTunArr[9].z;
		CreateAFace(p0,OutTunArr[8],OutTunArr[9],p1,8,L"�������");//�����Ұ��
		if(hdmdes[scross].Zt>0.001)//����ڷ�
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
			CreateAFace(p0,p1,p2,hdmdes[scross].PTRPTL[4],8,L"�������");
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
		//ˮƽ����
		CreateAFace(p0,p1,p2,p3,8,L"�������");
		//���
		p0.z = OutTunArr[6].z; p1.z = OutTunArr[5].z;		
		CreateAFace(p0,p1,OutTunArr[5],OutTunArr[6],8,L"�������");		
		p0.z = OutTunArr[5].z, p1.z = OutTunArr[4].z; 
		CreateAFace(p0,p1,OutTunArr[4],OutTunArr[5],8,L"�������");		
		p0.z = OutTunArr[4].z, p1.z = OutTunArr[3].z; 
		CreateAFace(p0,p1,OutTunArr[3],OutTunArr[4],8,L"�������");		
		p0.z = OutTunArr[3].z, p1.z = OutTunArr[2].z; 
		CreateAFace(p0,p1,OutTunArr[2],OutTunArr[3],8,L"�������");		
		p0.z = OutTunArr[2].z, p1.z = OutTunArr[1].z; 
		CreateAFace(p0,p1,OutTunArr[1],OutTunArr[2],8,L"�������");		
		p0.z = OutTunArr[1].z, p1.z = OutTunArr[0].z; 
		CreateAFace(p0,p1,OutTunArr[0],OutTunArr[1],8,L"�������");
		p0.z = OutTunArr[0].z;		p1.z=hdmdes[scross].PTRPTR[4].z;
		p2 = OutTunArr[0], p2.z = OutTunArr[0].z-0.3;
		CreateAFace(p0,p1,p2,OutTunArr[0],8,L"�������");//��������

		p0.x = PZtmp.y, p0.y = PZtmp.x, p0.z=OutTunArr[6].z;
		p1.x = PZtmp.y, p1.y = PZtmp.x, p1.z=OutTunArr[7].z;
		CreateAFace(p0,OutTunArr[6],OutTunArr[7],p1,8,L"�������");
		p0.z = p1.z, p1.z = OutTunArr[8].z;
		CreateAFace(p0,OutTunArr[7],OutTunArr[8],p1,8,L"�������");
		p0.z = p1.z, p1.z = OutTunArr[9].z;
		CreateAFace(p0,OutTunArr[8],OutTunArr[9],p1,8,L"�������");//�����Ұ��
		if(hdmdes[scross].Yt>0.001)//�Ҳ��ڷ�
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
			CreateAFace(p0,p1,p2,hdmdes[scross].PTRPTR[4],8,L"�������");
		}
	}
}
//�������� ���Ÿ���
void CDraw3D::DrawBriBody(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr)
{
	AcGePoint3d p0,p1,p2,p3;
	int npt,i,color=1,js;
	AcGePoint3d spt,ept;
	ACHAR lname[180];

	//	npt = qPtarr.length();//�����⣿����
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
				_tcscpy(lname,L"��������зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"������೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"�������Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"���������·��");
				color = 4;
			}
			else
			{
				_tcscpy(lname,L"�����������");
				color = 8;		
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"�����Ҳ��зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"�����Ҳ೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"�����Ҳ�Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"�����Ҳ���·��");
				color = 4;
			}
			else
			{
				_tcscpy(lname,L"�����Ҳ�����");
				color = 8;
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}
}

//����� 
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
				_tcscpy(lname,L"�������зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"�����೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"������Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"��������·��");
				color = 4;
			}
			else
			{
				_tcscpy(lname,L"�����ප��");
				color = 8;		
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"����Ҳ��зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"����Ҳ೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"����Ҳ�Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"����Ҳ���·��");
				color = 4;
			}
			else
			{
				_tcscpy(lname,L"����Ҳප��");
				color = 8;
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}
}

//������������·��
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
				_tcscpy(lname,L"����зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"��೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"���Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"�����·��");
				color = 4;
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"�Ҳ��зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"�Ҳ೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"�Ҳ�Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"�Ҳ���·��");
				color = 4;
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}
}
//����������̵��ʾ����
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
				_tcscpy(lname,L"����зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"��೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"���Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"�����·��");
				color = 4;
			}
			else
			{
				if(fmod(i,2.0)<0.01)
				{
					js = (i-4)*0.5+1;
					_stprintf(lname,L"���%d������",js);
					color = 2;
				}
				else
				{
					js = (i-3)*0.5;
					_stprintf(lname,L"���%d��ƽ̨",js);
					color = 8;
				}
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"�Ҳ��зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"�Ҳ೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"�Ҳ�Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"�Ҳ���·��");
				color = 4;
			}
			else
			{
				if(fmod(i,2.0)<0.01)
				{
					js = (i-4)*0.5+1;
					_stprintf(lname,L"�Ҳ�%d������",js);
					color = 2;
				}
				else
				{
					js = (i-3)*0.5;
					_stprintf(lname,L"�Ҳ�%d��ƽ̨",js);
					color = 8;
				}
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}

	if(ZorY==-1)
		_tcscpy(lname,L"������");
	else
		_tcscpy(lname,L"�Ҳ����");
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

//����������̵��ڷ�ʾ����
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
				_tcscpy(lname,L"����зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"��೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"���Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"�����·��");
				color = 4;
			}
			else if(i==4)
			{
				_tcscpy(lname,L"���ˮ�����");
				color = 8;
			}
			else if(i==5)
			{
				_tcscpy(lname,L"���ˮ����");
				color = 8;
			}
			else if(i==6)
			{
				_tcscpy(lname,L"���ˮ�����");
				color = 8;
			}
			else
			{
				if(fmod(i,2.0)<0.01)
				{
					js = (i-8)*0.5+1;
					_stprintf(lname,L"���%d������",js);
					color = 2;				
				}
				else
				{
					js = (i-5)*0.5;
					_stprintf(lname,L"���%d��ƽ̨",js);
					color = 8;
				}
			}
		}
		else
		{
			if(i==0)
			{
				_tcscpy(lname,L"�Ҳ��зִ�");
				color = 0;
			}
			else if(i==1)
			{
				_tcscpy(lname,L"�Ҳ೵�е�");
				color = 1;
			}
			else if(i==2)
			{
				_tcscpy(lname,L"�Ҳ�Ӳ·��");
				color = 5;
			}
			else if(i==3)
			{
				_tcscpy(lname,L"�Ҳ���·��");
				color = 4;
			}
			else if(i==4)
			{
				_tcscpy(lname,L"�Ҳ�ˮ�����");
				color = 8;
			}
			else if(i==5)
			{
				_tcscpy(lname,L"�Ҳ�ˮ����");
				color = 8;
			}
			else if(i==6)
			{
				_tcscpy(lname,L"�Ҳ�ˮ�����");
				color = 8;
			}
			else
			{
				if(fmod(i,2.0)<0.01)
				{
					js = (i-8)*0.5+1;
					_stprintf(lname,L"�Ҳ�%d������",js);
					color = 2;				
				}
				else
				{
					js = (i-5)*0.5;
					_stprintf(lname,L"�Ҳ�%d��ƽ̨",js);
					color = 8;
				}
			}
		}
		CreateAFace(p0,p1,p2,p3,color,lname);
	}

	if(ZorY==-1)
		_tcscpy(lname,L"������");
	else
		_tcscpy(lname,L"�Ҳ����");
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
		CreateAFace(TptArr[i],BptArr[i],BptArr[i+1],TptArr[i+1],8,L"�Ŷ�");

	AcGePoint3d BCenPt;
	BCenPt.x = CenPt.x,BCenPt.y = CenPt.y,BCenPt.z = CenPt.z-H;
	for(i=0; i<15; i++)
	{
		CreateAFace(TptArr[i],TptArr[i+1],CenPt,CenPt,8,L"�Ŷ�");
		CreateAFace(BptArr[i],BptArr[i+1],BCenPt,BCenPt,8,L"�Ŷ�");
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
		CreateAFace(TptArr[i],TptArr[i+1],BptArr[i+1],BptArr[i],8,L"��ñ");

	CreateAFace(TptArr[3],TptArr[0],BptArr[0],BptArr[3],8,L"��ñ");	
	CreateAFace(BptArr[0],BptArr[1],BptArr[2],BptArr[3],8,L"��ñ");			
}

/*
//����׶�½ŵ�
int CDraw3D::Cal_ZPJD(int ZorY,AcGePoint3d Cenpt,double fwj,AcGePoint3dArray &ptarr,double SlopDegree)//����׶�½ŵ�
{
int i,j,inf;
double du,jspt[2],len;
AcGePoint3d pt;

//	len = sqrt(20*20+(20*SlopDegree)*(20*SlopDegree));//׶�¸�20m;
len = SlopDegree;
for(j=0; j<=6; j++)//�Ӻ���淽����·ǰ������(0.5*PI)��Χ�ڻ������������߸���
{
du= fwj + ZorY*(0.5*PI - j*PI/12.0);

pt.x = Cenpt.x +  len*sin(du);
pt.y = Cenpt.y +  len*cos(du);
pt.z = Cenpt.z - 20;
ptarr.append(pt);
}
return RTNORM;
}*/
//����׶�½ŵ�
int CDraw3D::Cal_ZPJD(int ZorY,int HorT,AcGePoint3d Cenpt,AcGePoint3d Spt,AcGePoint3d Ept,AcGePoint3dArray &ptarr)//����׶�½ŵ�
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
	midpt[Y] = (Ept.y+Spt.y)/2.0; //�½ŵ���·�����е�	
	angle = ads_angle(bspt,midpt);
	dx = Ept.x-Cenpt.x;
	dy = Ept.y-Cenpt.y;	
	Elen = sqrt(dx*dx+dy*dy);
	dx = Spt.x-Cenpt.x;
	dy = Spt.y-Cenpt.y;	
	Slen = sqrt(dx*dx+dy*dy);
	Mlen = 0.5*(Elen+Slen);
	ads_polar(bspt,angle,Mlen,arcmid); //�����е�
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

	for(j=1; j<6; j++)//�Ӻ���淽����·ǰ������(0.5*PI)��Χ�ڻ������������߸���
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
		if(hdmdes[i].ZDMXZ==-1)//�����ͷ׶��
		{
			if(hdmdes[i].Lnpt==0)
				return;
			ang = FindBriAng(hdmdes[i].cml);
			if(fabs(ang-90)<0.1)
			{
				if(2*fabs(hdmdes[i].RPTL[hdmdes[i].Lnpt-1].x)/3 > 20)//2/3�½ŵ���·������
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
			Cal_ZPJD(-1,-1,CenPt,Spt,Ept,ptarr);//���������ͷ׶�½ŵ�
		}
		else if(hdmdes[i].ZDMXZ==1)//�����β׶��
		{
			if(hdmdes[i].Lnpt==0)
				return;

			ang = FindBriAng(hdmdes[i].cml);
			if(fabs(ang-90)<0.1)
			{
				if(2*fabs(hdmdes[i].RPTL[hdmdes[i].Lnpt-1].x)/3 > 20)//2/3�½ŵ���·������
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
			Cal_ZPJD(-1,1,CenPt,Spt,Ept,ptarr);//���������β׶�½ŵ�
		}		
	}
	else//�Ҳ�
	{
		if(hdmdes[i].YDMXZ==-1)//�Ҳ���ͷ׶��
		{
			if(hdmdes[i].Rnpt==0)
				return;
			ang = FindBriAng(hdmdes[i].cml);
			if(fabs(ang-90)<0.1)
			{
				if(2*fabs(hdmdes[i].RPTR[hdmdes[i].Rnpt-1].x)/3 > 20)//2/3�½ŵ���·������
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
			Cal_ZPJD(1,-1,CenPt,Spt,Ept,ptarr);//�����Ҳ���ͷ׶�½ŵ�				
		}
		else if(hdmdes[i].YDMXZ==1)//�Ҳ���β׶��
		{
			if(hdmdes[i].Rnpt==0)
				return;
			ang = FindBriAng(hdmdes[i].cml);
			if(fabs(ang-90)<0.1)
			{
				if(2*fabs(hdmdes[i].RPTR[hdmdes[i].Rnpt-1].x)/3 > 20)//2/3�½ŵ���·������
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
			Cal_ZPJD(1,1,CenPt,Spt,Ept,ptarr);//�����Ҳ���β׶�½ŵ�				
		}
	}
}

//����ͷ׶��
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
		if(hdmdes[i].ZDMXZ==-1 && hdmdes[i].YDMXZ==-1)//��ͷ
		{	
			CalBriSlopPt(i,-1,ptArr);
			//			Cal_ZPJD(-1,hdmdes[i].PTRPTL[4],PZtmp.a,ptArr,SlopDegree);//���������ͷ׶�½ŵ�
			for(j=0; j<6;j++)
				CreateFace(ptArr[j],hdmdes[i].PTRPTL[4],ptArr[j+1],2,L"����׶��");
			lpt = ptArr[6];//�����ǰ�½ŵ�	

			if(ptArr.length()>0)
				ptArr.removeSubArray(0,ptArr.length()-1);
			CalBriSlopPt(i,1,ptArr);
			//			Cal_ZPJD(1,hdmdes[i].PTRPTR[4],PZtmp.a,ptArr,SlopDegree);//�����Ҳ���ͷ׶�½ŵ�			
			for(j=0; j<6; j++)
				CreateFace(hdmdes[i].PTRPTR[4],ptArr[j],ptArr[j+1],2,L"����׶��");
			rpt = ptArr[6];//�Ҳ���ǰ�½ŵ�
			CreateAFace(hdmdes[i].PTRPTR[4],rpt,lpt,hdmdes[i].PTRPTL[4],2,L"����׶��");//�м�׶����
		}
		else if(hdmdes[i].ZDMXZ==-1)
		{
			CalBriSlopPt(i,-1,ptArr);
			//			Cal_ZPJD(-1,hdmdes[i].PTRPTL[4],PZtmp.a,ptArr,SlopDegree);//���������ͷ׶�½ŵ�
			for(j=0; j<6;j++)
				CreateFace(ptArr[j],hdmdes[i].PTRPTL[4],ptArr[j+1],2,L"����׶��");			
		}
		else if(hdmdes[i].YDMXZ==-1)
		{
			CalBriSlopPt(i,1,ptArr);
			//			Cal_ZPJD(1,hdmdes[i].PTRPTR[4],PZtmp.a,ptArr,SlopDegree);//�����Ҳ���ͷ׶�½ŵ�			
			for(j=0; j<6; j++)
				CreateFace(hdmdes[i].PTRPTR[4],ptArr[j],ptArr[j+1],2,L"����׶��");			
		}

		if(hdmdes[i].ZDMXZ==1 && hdmdes[i].YDMXZ==1)//��β
		{
			CalBriSlopPt(i,-1,ptArr);
			//			Cal_ZPJD(1,hdmdes[i].PTRPTL[4],PZtmp.a+PI,ptArr,SlopDegree);//���������ͷ׶�½ŵ�
			for(j=0; j<6;j++)
				CreateFace(ptArr[j],ptArr[j+1],hdmdes[i].PTRPTL[4],2,L"����׶��");
			lpt = ptArr[6];//�����ǰ�½ŵ�

			if(ptArr.length()>0)
				ptArr.removeSubArray(0,ptArr.length()-1);
			CalBriSlopPt(i,1,ptArr);
			//			Cal_ZPJD(-1,hdmdes[i].PTRPTR[4],PZtmp.a+PI,ptArr,SlopDegree);//�����Ҳ���ͷ׶�½ŵ�			
			for(j=0; j<6; j++)
				CreateFace(hdmdes[i].PTRPTR[4],ptArr[j+1],ptArr[j],2,L"����׶��");
			rpt = ptArr[6];//�Ҳ���ǰ�½ŵ�
			CreateAFace(hdmdes[i].PTRPTL[4],lpt,rpt,hdmdes[i].PTRPTR[4],2,L"����׶��");//�м�׶����
		}	
		else if(hdmdes[i].ZDMXZ==1)
		{
			CalBriSlopPt(i,-1,ptArr);
			//			Cal_ZPJD(1,hdmdes[i].PTRPTL[4],PZtmp.a+PI,ptArr,SlopDegree);//���������ͷ׶�½ŵ�
			for(j=0; j<6;j++)
				CreateFace(ptArr[j],ptArr[j+1],hdmdes[i].PTRPTL[4],2,L"����׶��");
		}
		else if(hdmdes[i].YDMXZ==1)
		{
			CalBriSlopPt(i,1,ptArr);
			//			Cal_ZPJD(-1,hdmdes[i].PTRPTR[4],PZtmp.a+PI,ptArr,SlopDegree);//�����Ҳ���ͷ׶�½ŵ�			
			for(j=0; j<6; j++)
				CreateFace(hdmdes[i].PTRPTR[4],ptArr[j+1],ptArr[j],2,L"����׶��");
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

	acedSetStatusBarProgressMeter(L"������ʾ������άģ��...",0,TriSum);	

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
		CreateFace(pt1,pt2,pt3,3,L"����ģ��");

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

	if(LMKTABname == "��·���ֶα�")
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