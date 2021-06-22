// GTZDM.cpp: implementation of the GTZDM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdarx.h"
#include "GTZDM.h"
#include "tun.h"
#include "BAS_DRAW_FUN.H"
#include "XLdatabase.H"
//szf #include "ERRMESDLG.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__ ;
#define new DEBUG_NEW
#endif
#define  sign(y, x)  { if ((x) == 0)        \
	y =0;             \
					   else if ((x) > 0)    \
					   y =1;             \
					   else                 \
					   y = -1;           \
}


#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <aced.h>
#include <dbsymtb.h>
//  #include <dbapserv.h>
#include <adslib.h>
#include <dbxrecrd.h>

#include <string.h>
#include <dbents.h>
#include <geassign.h>
#include <dbgroup.h>
#include <gearc2d.h >
#include <dbpl.h> 


#include <dbjig.h>
#include <acgi.h>

#include <assert.h>
//#include <iostream.h>
#include "rxdefs.h"
#include "ol_errno.h"
//#include "STDIO.h"
#include "malloc.h"
#include "time.h"
#include "ORI_SCALE.h"
#include "docktypehdmbar.h"
#include "RoadDesignWorkingDatabase.h"
#define  streq(s,t) (_tcscmp((s),(t))==0)
#define  AppName  /*MSG1*/L"GUADS"
ads_point HdmPt;//查看典型断面的位置

DockTypeHdmBar *TypeHdmBar; 
bool IfViewTypeHdm;//平纵面设计时是否查看典型横断面
JD_CENTER * pzLinep = NULL;
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库

GTZDM_ROAD::~GTZDM_ROAD()
{

}
GTZDM_ROAD::GTZDM_ROAD()  { INIT(); }


Acad::ErrorStatus  GTZDM_ROAD::subTranformBy(const AcGeMatrix3d &)
{	
	return Acad::eOk;   
}


Acad::ErrorStatus  GTZDM_ROAD::subGetTransformedCopy(
	const AcGeMatrix3d& mat,
	AcDbEntity*& ent) const
{
	assertReadEnabled();
	Acad::ErrorStatus es = Acad::eOk;

	return es;
}

void GTZDM_ROAD::INIT()
{ 
	int i=0,j;
	h_plan =   15.0;
	h_ml   =   27.5;
	h_addml = 320.0;
	h_addh =  332.5;
	h_prof =   35.0;
	h_geolo =  75.0;
	h_earth =  100.0;
	Property = "";
	for (i=0;i<MAXBPDNUM;i++)
	{   
		for ( j=0;j<4;j++)
			BPD_array[i][j]=0.0;
		BPD_array[i][3]=-1000.0;//坡度初值

	};
	NBPD=0; 
	/*
	for (i=0;i<MAXDLNUM;i++) 
	{   DLB[i].ELC=0.0; DLB[i].BLC=0.0;
	DLB[i].BGH=""; DLB[i].EGH="";
	ZDMDLBZ[i][0]=0; ZDMDLBZ[i][1]=100;
	};*/

	NH=3;
	NLC=3;
	GLbz=1;  //绘公路 =1 铁路=2
	VerRadius = 0;
	m_Is_JP = 0; //是否考虑接坡
	//texth=4.0;	
	texth = pORI_SCALE.PDTextH;

	/*
	VSCALE=10 ; //     高程比例 
	HSCALE=100 ; // 水平比例
	HBVSCALE=10; //  水平与高程比例之比   
	//   现场[K0,H0]===纵面图上(X0,Y0) 
	K0=10000;   H0=100;     X0=100  ; Y0=100; 
	*/
	//设置绘图参数
	VSCALE = pORI_SCALE.VSCALE; //     高程比例 
	HSCALE = pORI_SCALE.HSCALE; // 水平比例
	HBVSCALE = pORI_SCALE.HBVSCALE; //  水平与高程比例之比   
	TDFS = pORI_SCALE.TDFS;
	//   现场[K0,H0]===纵面图上(X0,Y0) 
	K0 = pORI_SCALE.STARTLC;
	H0 = pORI_SCALE.ZMIN;
	X0 = pORI_SCALE.X_ORI;
	Y0 = pORI_SCALE.Y_ORI;

	Xsw = pORI_SCALE.XSW;
	DesHXSW = pORI_SCALE.DesHXSW;

	// 起点里程 标线起点高程 起点图上X坐标  起点图上Y坐标
	EK = 100000;  //  终点里程 
	PMY0 = 10; BMLCY0 = 20; DMLCY0 = 50;  DMBGY0 = 60; PDY0 = 80; SJBGY0 = 200;
	// 起点   平面  百米标  地面里程 地面高程 坡度  坡度高程   

	Lmin=30;         
	// 竖曲线最短长度m 
	//GH

	//读取一些基本参数
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(pzLinep->mdbname,L"线路属性");//读取线路数据库中的数据
	DV = xlmdb.RoadXZ.DV;
	CString	m_A_JXRshuMin;
	CString	m_A_YBRshu;
	CString	m_MAXPD;
	CString	m_MINPC;
	CString	m_T_JXRShu;
	CString	m_T_YBRshuMin;
	CString	m_YBLShuMin;
	CString	m_YBPCMin;
	CString	m_JXPCMin;
	CString	m_JXLRShuMin;
	if(DV>0)
	{
		BAS_DRAW_FUN ob;
		ob.SetZdmMinPC(DV,m_MAXPD,m_YBPCMin,m_JXPCMin,
			m_T_YBRshuMin,m_T_JXRShu,
			m_A_YBRshu,m_A_JXRshuMin,
			m_YBLShuMin,m_JXLRShuMin);
	}
	xlmdb.Read_XLDbs(pzLinep->mdbname,L"纵面设计标准");//读取线路数据库中的数据
	if(xlmdb.ZDMBZ.MAXPD!="")
		MAXPD=_wtof(xlmdb.ZDMBZ.MAXPD);
	else
		MAXPD=_wtof(m_MAXPD);
	if(xlmdb.ZDMBZ.TRshu1!="")
		T_YBRshuMin=_wtof(xlmdb.ZDMBZ.TRshu1);
	else
		T_YBRshuMin=_wtof(m_T_YBRshuMin);
	if(xlmdb.ZDMBZ.TRshu2!="")
		T_JXRShu=_wtof(xlmdb.ZDMBZ.TRshu2);
	else
		T_JXRShu=_wtof(m_T_JXRShu);
	if(xlmdb.ZDMBZ.ARshu1!="")
		A_YBRshu=_wtof(xlmdb.ZDMBZ.ARshu1);
	else
		A_YBRshu=_wtof(m_A_YBRshu);
	if(xlmdb.ZDMBZ.ARshu2!="")
		A_JXRshuMin=_wtof(xlmdb.ZDMBZ.ARshu2);
	else
		A_JXRshuMin=_wtof(m_A_JXRshuMin);
	if(xlmdb.ZDMBZ.PCMin1!="")
		YBPCMin=_wtof(xlmdb.ZDMBZ.PCMin1);
	else
		YBPCMin=_wtof(m_YBPCMin);
	if(xlmdb.ZDMBZ.PCMin2!="")
		JXPCMin=_wtof(xlmdb.ZDMBZ.PCMin2);
	else
		JXPCMin=_wtof(m_JXPCMin);
	if(xlmdb.ZDMBZ.Lshu1!="")
		YBLShuMin=_wtof(xlmdb.ZDMBZ.Lshu1);
	else
		YBLShuMin=_wtof(m_YBLShuMin);
	if(xlmdb.ZDMBZ.Lshu2!="")
		JXLRShuMin=_wtof(xlmdb.ZDMBZ.Lshu2);
	else
		JXLRShuMin=_wtof(m_JXLRShuMin);

}

GTZDM_ROAD::GTZDM_ROAD(int iNBPD,double iBPD_array[MAXBPDNUM][4],ACHAR *GH ,int type,int deshxsw)
{  
	int i=0 ;
	ACHAR EGH[8];
	INIT();

	XLDataBase DBS;
	if(pzLinep)
	{  
		if (pzLinep->DLArray.GetSize())
		{
			DLNum = pzLinep->DLArray.GetSize();
			DLArray.SetSize(DLNum);
			for (int i = 0; i < pzLinep->DLArray.GetSize(); i++)
				setDLM(i, pzLinep->DLArray[i].BLC, pzLinep->DLArray[i].ELC, pzLinep->DLArray[i].BGH, pzLinep->DLArray[i].EGH);
		}

		//DBS.Read_XLDbs(pzLinep->mdbname,L"纵断链表",pzLinep->RoadName);
		//if(DBS.NZDL>0)
		//{
		//	DLNum=DBS.NZDL;
		//	DLArray.SetSize(DLNum);
		//	for(i=0;i<DLNum;i++)
		//		setDLM(i,DBS.ZDLArray[i].BLC,DBS.ZDLArray[i].ELC,DBS.ZDLArray[i].BGH,DBS.ZDLArray[i].EGH);
		//	//			K0=DLArray[0].ELC; 
		//}	
	}    
	for (i=0;i<iNBPD;i++)
	{  
		BPD_array[i][2]=iBPD_array[i][2];	  // R
		if (type == 0)
		{
			BPD_array[i][0] = Trs_Coor_System(iBPD_array[i][0], MILE, TO_DRAW);
			BPD_array[i][1] = Trs_Coor_System(iBPD_array[i][1], LEVEL, TO_DRAW);
			//BPD_array[i][0] = (iBPD_array[i][0] - K0) / HBVSCALE + X0;//LLC
			//BPD_array[i][1] = iBPD_array[i][1] - H0 + Y0;//HH
		}
		else
		{
			BPD_array[i][0] = iBPD_array[i][0];//LLC==xx
			BPD_array[i][1] = iBPD_array[i][1];//HH==yy
		}
	};
	NBPD=iNBPD; 

	// 终点  作为一特殊断链点
	acdbHostApplicationServices()->workingDatabase()->
		loadLineTypeFile(L"CONTINUOUS",L"acadiso.lin");//
	ZPId = CreateLayer(L"ZP",L"CONTINUOUS");	

	DesHXSW = deshxsw;
	BpdToCM();
}

Adesk::Boolean  GTZDM_ROAD::G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx)
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

Adesk::Boolean  GTZDM_ROAD::G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor)
{     
	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	
	pWd->geometry().polyline(2,pArr);
	return  0;
}

/*

Adesk::Boolean  GTZDM::G_makeDL(AcGiWorldDraw* pWd)
{   

AcGePoint3d centerPt,midPt;


AcGePoint3d* pArr=new AcGePoint3d[2];
pArr[0].set(startPt.x, startPt.y, startPt.z);
pArr[1].set(endPt.x, endPt.y, endPt.z);

pWd->subEntityTraits().setColor(icolor);	
pWd->geometry().polyline(2,pArr);
return  0;
return 1;
} 
*/

//	标注变坡点
Adesk::Boolean  GTZDM_ROAD::BZBPD(AcGiWorldDraw* pWd)
{      
	if (pzLinep == NULL)
		return -1;

	AcGePoint3d Pt1, Pt2;
	ACHAR chrh[40], chrlc[40];
	int i ;
	double HH, LC, LL, II, B, WB, yy, xlc, BLC;
	CString  GH;
	B = pi * 0.5;

	pWd->subEntityTraits().setLayer(ZPId);
	textStyle();

	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt000);

	WB = 4 * texth;
	WB = WB + 10 * texth;
	Pt2.z = 0.0;
	Pt2.z = 0.0;

	LC = Trs_Coor_System(BPD_array[0][0], MILE, TO_SLOPE);
	HH = Trs_Coor_System(BPD_array[0][1], LEVEL, TO_SLOPE);

	Pt1.x = BPD_array[0][0];
	Pt1.y = BPD_array[0][1];
	Pt2.x = Pt1.x;
	Pt2.y = Pt1.y + WB;
	G_makeline(pWd, Pt1, Pt2, 3);
	
	// 设计 高程	起点
	ads_rtos(HH, 2, NLC, chrh);
	Pt2.x = Pt1.x - texth * 0.5;
	Pt2.y = Pt1.y + 3 * texth;
	G_maketext(pWd, Pt2, chrh, B, texth, 4, 2);

	// 设计 里程	起点
	xlc = XLC(LC, GH);
	_tcscpy(chrlc, LCchr(GH, xlc, NLC));
	Pt2.x = Pt1.x + texth * 1.5;
	Pt2.y = Pt1.y + 3 * texth;
	G_maketext(pWd, Pt2, chrlc, B, texth, 4, 2);

	int TextColor;
	int TemTexth = texth;
	for (i = 1; i < NBPD; i++)
	{
		TextColor = 4;	//青色
		texth = TemTexth;
		LL = (BPD_array[i][0] - BPD_array[i - 1][0]) * HSCALE;	//坡长
		if (fabs(LL) < 0.001)
			LL = 100.0;
		double unit;
		unit = 1000.0; 
		if (GLbz == 1) 
			unit = 100.0;
		II = BPD_array[i][3];//已按给定的小数位取整
		BLC = LC;//前一变坡点里程
		LC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
		HH = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
		yy = BPD_array[i][1] - BPD_array[i - 1][1]; // 高差

		//绘制里程、高程原点
		Pt1.x = BPD_array[i][0];
		Pt1.y = BPD_array[i][1];
		Pt2.x = Pt1.x;
		Pt2.y = Pt1.y + WB;
		G_makeline(pWd, Pt1, Pt2, 3);

		// 设计 高程
		ads_rtos(HH, 2, DesHXSW, chrh);
		Pt2.x = Pt1.x - texth * 0.5;
		Pt2.y = Pt1.y + 3 * texth;
		G_maketext(pWd, Pt2, chrh, B, texth, TextColor, 2);
		// 设计 里程
		xlc = XLC(LC, GH);
		_tcscpy(chrlc, LCchr(GH, xlc, NLC));
		Pt2.x = Pt1.x + texth * 1.5;
		Pt2.y = Pt1.y + 3 * texth;
		G_maketext(pWd, Pt2, chrlc, B, texth, TextColor, 2);

		//依据坡长调整字高
		double l = _tcslen(chrh) * texth * HSCALE;
		double Oritexth;
		Oritexth = texth;

		while (LL < l && LL > 0.001)//需要调整字高
		{
			texth = texth - 0.5;
			l = _tcslen(chrh) * texth * HSCALE;
		}

		double NoteXZPD;

		//考虑坡度折减
		/*
		if(DV<249)
		{
		//1.先作隧道折减
		beta = pdzj_Tun(BLC,LC);
		MaxPD = beta*MaxPD;
		//2.作曲线折减
		pdzj(BLC,LC,&MAXPD1,&MAXPD2);
		}*/

		NoteXZPD = MAXPD;
		if (Xsw == 1) 
			NoteXZPD = (int)(NoteXZPD*10.0 + 0.00001) / 10.0;
		else if (Xsw == 2) 
			NoteXZPD = (int)(NoteXZPD*100.0 + 0.00001) / 100.0;
		else if (Xsw == 3) 
			NoteXZPD = (int)(NoteXZPD*1000.0 + 0.00001) / 1000.0;
		else 
			NoteXZPD = (int)(NoteXZPD + 0.00001);
		if (BPD_array[i][3] > NoteXZPD)
			TextColor = 1;

		


		//（左侧）坡度 坡长
		double fb, de;
		de = xyddaa(BPD_array[i - 1][1], BPD_array[i - 1][0], BPD_array[i][1], BPD_array[i][0], &fb);
		fb = 0.5 * pi - fb;
		// 坡度  
		ACHAR chrXZPD[20];
		if (Xsw == 1)
		{
			ads_rtos(II, 2, 1, chrh);
			ads_rtos(NoteXZPD, 2, 1, chrXZPD);
		}
		else if (Xsw == 2)
		{
			ads_rtos(II, 2, 2, chrh);
			ads_rtos(NoteXZPD, 2, 2, chrXZPD);
		}
		else if (Xsw == 3)
		{
			ads_rtos(II, 2, 3, chrh);
			ads_rtos(NoteXZPD, 2, 3, chrXZPD);
		}
		else
		{
			ads_rtos(II, 2, 0, chrh);
			ads_rtos(NoteXZPD, 2, 0, chrXZPD);
		}

		if (GLbz == 2)  
			_tcscat(chrh, L"‰");  //铁路
		else   
			_tcscat(chrh, L"％"); 	//公路

		if (TextColor == 4) 
			_stprintf(chrh, L"%s(限坡%s％)", chrh, chrXZPD);
		else 
			_stprintf(chrh, L"%s(超限%s％)", chrh, chrXZPD);

		//坡度画在连线上面
		Pt2.x = (BPD_array[i][0] + BPD_array[i - 1][0]) / 2 + 1 * texth * cos(fb + 0.5 * pi);
		Pt2.y = (BPD_array[i][1] + BPD_array[i - 1][1]) / 2 + 1 * texth * sin(fb + 0.5 * pi);
		G_maketext(pWd, Pt2, chrh, fb, texth, TextColor, 1);

		//  坡长
		if (fabs(LL - (int)(LL))<0.001)
			ads_rtos(LL, 2, 0, chrlc); 
		else 
			ads_rtos(LL, 2, 3, chrlc);
		
		//坡长画在连线下面
		Pt2.x = (BPD_array[i][0] + BPD_array[i - 1][0]) / 2 - 1 * texth * cos(fb + 0.5 * pi);
		Pt2.y = (BPD_array[i][1] + BPD_array[i - 1][1]) / 2 - 1 * texth * sin(fb + 0.5 * pi);
		G_maketext(pWd, Pt2, chrlc, fb, texth, TextColor, 1);

		if (i == NBPD - 1)
		{
			CString pszLabel;
			pszLabel.Format(L"%s坡长%s", chrh, chrlc);
			acedGrText(-1, pszLabel, 1);
		}
		texth = Oritexth;
	}
	return 1;
}

//	标注变坡点
/*
Adesk::Boolean  GTZDM::BZBPD0ld(AcGiWorldDraw* pWd)
{   
if(pzLinep==NULL)return -1;

AcGePoint3d Pt1,Pt2; 
char chrh[40],chrlc[40];
int i ;
double HH,LC,LL,II,B,WB,yy,xlc,BLC;
char GH[8];

pWd->subEntityTraits().setLayer(ZPId);
textStyle();

pWd->subEntityTraits().setLineWeight(AcDb::kLnWt000);

//   _tcscpy(GH,L"CK");
WB=4*texth;
WB=WB+10*texth;
Pt2.z=0.0;  Pt2.z=0.0;

LC=(BPD_array[0][0]-X0)*HBVSCALE+K0;
HH=BPD_array[0][1]-Y0+H0;    // 设计高程

Pt1.x=BPD_array[0][0];
Pt1.y=BPD_array[0][1];
Pt2.x=Pt1.x;
Pt2.y=Pt1.y+WB;
G_makeline(pWd,Pt1,Pt2,3);
B=pi*0.5;
// 设计 高程
ads_rtos(HH,2,NLC,chrh);
Pt2.x=Pt1.x-texth*0.5;
Pt2.y=Pt1.y+3*texth;
G_maketext(pWd,Pt2,chrh,B ,texth ,4,2);     
// maketext( p1,ch1,de,texth ,4,1);   
// 设计 里程
xlc=pzLinep->XLC(LC,GH,pzLinep->NDL);
//	  acutPrintf(L"GH=%s,xlc=%lf\n",GH,xlc);

_tcscpy(chrlc,LCchr(GH,xlc,NLC,2));
Pt2.x=Pt1.x+texth*1.5;
Pt2.y=Pt1.y+3*texth;
G_maketext(pWd,Pt2,chrlc,B ,texth ,4,2);     
// maketext( p1,ch1,de,texth ,4,1); 

int TextColor;
for (i=1;i<NBPD;i++)
{ 
TextColor = 4;
LL=(BPD_array[i][0]-BPD_array[i-1][0])*HBVSCALE;
if (fabs(LL)<0.001) LL=1000.0;
double unit;
unit=1000.0; if (GLbz==1) unit=100.0;
//		II=(BPD_array[i][1]-BPD_array[i-1][1])*unit/LL;
II = BPD_array[i][3];//已按给定的小数位取整
BLC = LC;//前一变坡点里程
LC=(BPD_array[i][0]-X0)*HBVSCALE+K0;
//		if(i==NBPD-1)acutPrintf(L"lc=%lf\n",LC);
HH=BPD_array[i][1]-Y0 +H0;    // 设计高程
yy=BPD_array[i][1]-BPD_array[i-1][1]; // 高差

Pt1.x=BPD_array[i][0];
Pt1.y=BPD_array[i][1];
Pt2.x=Pt1.x;
Pt2.y=Pt1.y+WB;

double beta,MaxPD,NoteXZPD;
double MAXPD1,MAXPD2;//考虑曲线折简后的上下坡设计坡度折减值

if(BPD_array[i][3]>=0.0)
MaxPD = XZPD;
else
MaxPD = XZPD1;
MAXPD1=MAXPD2=0.0;
//考虑坡度折减
if(mGuiFanCS.ZheJian)
{
//1.先作隧道折减
beta = pdzj_Tun(BLC,LC);
MaxPD = beta*MaxPD;
//2.作曲线折减
pdzj(BLC,LC,&MAXPD1,&MAXPD2);
}

if(BPD_array[i][3]>=0.0)//上坡
{

NoteXZPD = MaxPD-MAXPD1;
if(Xsw==1) NoteXZPD = (int)(NoteXZPD*10.0+0.00001)/10.0;
else if(Xsw==2) NoteXZPD = (int)(NoteXZPD*100.0+0.00001)/100.0;
else if(Xsw==3) NoteXZPD = (int)(NoteXZPD*1000.0+0.00001)/1000.0;
else NoteXZPD = (int)(NoteXZPD+0.00001);
if(BPD_array[i][3]>NoteXZPD)
TextColor = 1; 
}
else//下坡
{
NoteXZPD = MaxPD-fabs(MAXPD2);
if(Xsw==1) NoteXZPD = (int)(NoteXZPD*10.0+0.00001)/10.0;
else if(Xsw==2) NoteXZPD = (int)(NoteXZPD*100.0+0.00001)/100.0;
else if(Xsw==3) NoteXZPD = (int)(NoteXZPD*1000.0+0.00001)/1000.0;
else NoteXZPD = (int)(NoteXZPD+0.00001);			
if(fabs(BPD_array[i][3])>NoteXZPD)
TextColor = 1; 
}

G_makeline(pWd,Pt1,Pt2,3);
B=pi*0.5;
// 设计 高程
ads_rtos(HH,2,NLC,chrh);
Pt2.x=Pt1.x-texth*0.5;
Pt2.y=Pt1.y+3*texth;
G_maketext(pWd,Pt2,chrh,B ,texth ,TextColor,2);     
// maketext( p1,ch1,de,texth ,4,1);   
// 设计 里程
xlc=pzLinep->XLC(LC,GH,pzLinep->NDL);
_tcscpy(chrlc,LCchr(GH,xlc,NLC,2));
//		acutPrintf(L"GH=%s,str=%s\n",GH,chrlc);
Pt2.x=Pt1.x+texth*1.5;
Pt2.y=Pt1.y+3*texth;
G_maketext(pWd,Pt2,chrlc,B ,texth ,TextColor,2);     
// maketext( p1,ch1,de,texth ,4,1); 
//（左侧）坡度 坡长
double fb, de ;
de=xyddaa(BPD_array[i-1][1],BPD_array[i-1][0],BPD_array[i][1],BPD_array[i][0],&fb);
fb= 0.5*pi-fb;
// 坡度  
//		ads_printf(L"%d %lf\n",i,II);
char chrXZPD[20];
if(Xsw==1)
{
ads_rtos(II,2,1,chrh);
ads_rtos(NoteXZPD,2,1,chrXZPD);
}
else if(Xsw==2)
{
ads_rtos(II,2,2,chrh);
ads_rtos(NoteXZPD,2,2,chrXZPD);
}
else if(Xsw==3)
{
ads_rtos(II,2,3,chrh);
ads_rtos(NoteXZPD,2,3,chrXZPD);
}
else
{
ads_rtos(II,2,0,chrh);
ads_rtos(NoteXZPD,2,0,chrXZPD);
}

/ *
if (fabs(II-(int)(II))<0.001)  {  ads_rtos(II,2,0,chrh);}
else  if (fabs(II*10.0-(int)(II*10.0))<0.001)  {  ads_rtos(II,2,1,chrh);}
else  if (fabs(II*100.0-(int)(II*100.0))<0.001)  {  ads_rtos(II,2,2,chrh);}
else   ads_rtos( II,2,3,chrh);
* /

if ( GLbz==2)  {  _tcscat(chrh,L"(o/oo)"); 	 }  //铁路
else   _tcscat(chrh,L"(o/o)"); 	//公路

_stprintf(chrh,L"%s(%s)",chrh,chrXZPD);
Pt2.x=Pt1.x- LL*0.5/HBVSCALE-texth;
Pt2.y=Pt1.y+0.5*texth-yy*0.5;
G_maketext(pWd,Pt2,chrh,fb,texth ,TextColor,2);     
// maketext(Pt2,chrh,fb,texth ,4,1); 
//  坡长
if (fabs(LL-(int)(LL))<0.001)  {  ads_rtos(LL,2,0,chrlc);}
else ads_rtos(LL,2,3,chrlc);
//	_tcscat(chrh,L"(o/oo)"); 	    
Pt2.x=Pt1.x- LL*0.5/HBVSCALE-3*texth;
Pt2.y=Pt1.y-1.6*texth-yy*0.5;
G_maketext(pWd,Pt2,chrlc,fb,texth ,TextColor,2);     
//   maketext(Pt2,chrlc,fb,texth ,4,1); 
} 
return 1;
}
*/
//标注变坡点
Adesk::Boolean GTZDM_ROAD::BZBPD1(double  BPD_array[MAXBPDNUM][4],int NBPD,double CSB[6], int NLC,int GLbz)
{   
	AcGePoint3d Pt1,Pt2; 
	ACHAR chrh[40],chrlc[40];
	int i ;
	double HH,LC,LL,II,B,WB,yy;
	double K0,H0,X0,Y0,HBVSCALE,texth;
	ACHAR GH[8];
	K0=CSB[0];H0=CSB[1];X0=CSB[2];Y0=CSB[3];HBVSCALE=CSB[4];texth=CSB[5];

	WB=4*texth;
	WB=WB+10*texth;
	Pt2.z=0.0;  Pt2.z=0.0;

	LC=(BPD_array[0][0]-X0)*HBVSCALE+K0;
	HH=BPD_array[0][1]-Y0+H0;    // 设计高程

	Pt1.x=BPD_array[0][0];
	Pt1.y=BPD_array[0][1];
	Pt2.x=Pt1.x;
	Pt2.y=Pt1.y+WB;
	//G_makeline(pWd,Pt1,Pt2,3);
	makeline( Pt1,Pt2,3);
	B=pi*0.5;
	// 设计 高程
	ads_rtos(HH,2,NLC,chrh);
	Pt2.x=Pt1.x-texth*0.5;
	Pt2.y=Pt1.y+3*texth;
	// G_maketext(pWd,Pt2,chrh,B ,texth ,4,2);     
	maketext( Pt2,chrh,B ,texth ,4,1);   

	// 设计 里程
	_tcscpy(chrlc,LCchr(GH,LC,NLC,2));
	Pt2.x=Pt1.x+texth*1.5;
	Pt2.y=Pt1.y+3*texth;
	maketext(Pt2,chrlc,B ,texth ,4,2);  


	for (i=1;i<NBPD;i++)
	{ 
		LL=(BPD_array[i][0]-BPD_array[i-1][0])*HBVSCALE;
		if (fabs(LL)<0.001) LL=1000.0;
		double unit;
		unit=1000.0;
		if (GLbz==1) unit=100.0;
		II=(BPD_array[i][1]-BPD_array[i-1][1])*unit/LL;
		// II=(BPD_array[i][1]-BPD_array[i-1][1])*1000.0/LL;
		LC=(BPD_array[i][0]-X0)*HBVSCALE+K0;
		HH=BPD_array[i][1]-Y0 +H0;    // 设计高程
		yy=BPD_array[i][1]-BPD_array[i-1][1]; // 高差

		Pt1.x=BPD_array[i][0];
		Pt1.y=BPD_array[i][1];
		Pt2.x=Pt1.x;
		Pt2.y=Pt1.y+WB;
		// G_makeline(pWd,Pt1,Pt2,3);
		makeline( Pt1,Pt2,3);
		B=pi*0.5;
		// 设计 高程
		ads_rtos(HH,2,NLC,chrh);
		Pt2.x=Pt1.x-texth*0.5;
		Pt2.y=Pt1.y+3*texth;
		//G_maketext(pWd,Pt2,chrh,B ,texth ,4,2);     
		maketext( Pt2,chrh,B ,texth ,4,1); 
		// 设计 里程
		_tcscpy(chrlc,LCchr(GH,LC,NLC,2));
		Pt2.x=Pt1.x+texth*1.5;
		Pt2.y=Pt1.y+3*texth;
		// G_maketext(pWd,Pt2,chrlc,B ,texth ,4,2);     
		maketext( Pt2,chrlc,B ,texth ,4,1); 
		//（左侧）坡度 坡长
		double fb, de ;
		de=xyddaa(BPD_array[i-1][1],BPD_array[i-1][0],BPD_array[i][1],BPD_array[i][0],&fb);
		fb= 0.5*pi-fb;
		// 坡度  
		if (fabs(II-(int)(II))<0.001)  {  ads_rtos(II,2,0,chrh);}
		else  if (fabs(II*10.0-(int)(II*10.0))<0.001)  {  ads_rtos(II,2,1,chrh);}
		else  if (fabs(II*100.0-(int)(II*100.0))<0.001)  {  ads_rtos(II,2,2,chrh);}
		else   ads_rtos( II,2,3,chrh);
		if (GLbz==2) { _tcscat(chrh,L"(o/oo)"); 	    }
		else  _tcscat(chrh,L"(o/o)"); 	 
		Pt2.x=Pt1.x- LL*0.5/HBVSCALE-texth;
		Pt2.y=Pt1.y+0.5*texth-yy*0.5;
		// G_maketext(pWd,Pt2,chrh,fb,texth ,4,2);     
		maketext(Pt2,chrh,fb,texth ,4,1); 
		//  坡长
		if (fabs(LL-(int)(LL))<0.001)  {  ads_rtos(LL,2,0,chrlc);}
		else ads_rtos(LL,2,3,chrlc);
		_tcscat(chrh,L"(o/oo)"); 	    
		Pt2.x=Pt1.x- LL*0.5/HBVSCALE-3*texth;
		Pt2.y=Pt1.y-1.6*texth-yy*0.5;
		//G_maketext(pWd,Pt2,chrlc,fb,texth ,4,2);     
		maketext(Pt2,chrlc,fb,texth ,4,1); 
	};  
	return 1;
}


Adesk::Boolean  GTZDM_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{ 
	AcGePoint3d PT;
	PT.x = BPD_array[0][0], PT.y = BPD_array[0][1], PT.z = 0.0;
	JD_CENTER *pz = NULL;

	//pz = GlobalZDMTK.GetPM(PT);
	//pz = GlobalZDMTK.ZDMTK[m_Frame_Index].pm;
	//20191023	修改	平面数据从活动数据库中获取，不从图框中获取，因为图框中的平面数据可能过时
	if(m_Frame_Index < GlobalZDMTK.NZDM)
		pz = g_RoadWorkingDatabase.CreateHorizontalRoad(GlobalZDMTK.ZDMTK[m_Frame_Index].mdbname, GlobalZDMTK.ZDMTK[m_Frame_Index].RXDname);
	
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"zm:无法找到平面方案!\n");

	//修正限制坡度
	if (pzLinep == NULL || NBPD <= 1)
		return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);

	AcGePoint3d Pt, *verts = new AcGePoint3d[NBPD];
	AcGeVector3d  normal(0.0,0.0,1.0);
	int i,j=0,k=0;
	double LL,LC,ii,xlc1;
	double unit;
	unit=1000.0;
	if (GLbz == 1)
		unit=100.0;

	//设置里程取整与小数位数
	QZZ = pORI_SCALE.QZZ;
	Xsw = (int)(pORI_SCALE.XSW + 0.001);

	//检查起点是否超出线路长度范围 不检查 考虑接坡
	LC = Trs_Coor_System(BPD_array[0][0], MILE, TO_SLOPE);
	if(LC < K0)
		BPD_array[0][0] = X0;
	//检查终点是否超出线路长度范围
	LC = Trs_Coor_System(BPD_array[NBPD - 1][0], MILE, TO_SLOPE);
	if (LC > DLArray[DLNum - 1].TYLC + 0.001)
	{
		LC = DLArray[DLNum - 1].TYLC;
		BPD_array[NBPD - 1][0] = Trs_Coor_System(LC, MILE, TO_DRAW);
	}

	double deltaI, Rshu;//坡度代数差
	double Bii;//前一坡度
	ii = 0;
	BPD_array[0][3] = 0.0;//坡度为0
	BPD_array[0][2] = BPD_array[NBPD - 1][2] = 0.0;//起终点竖曲线半径为0;
	for (i = 1; i < NBPD; i++)//坡度取整
	{
		Bii = ii;
		LL = (BPD_array[i][0] - BPD_array[i - 1][0]) * HBVSCALE;	//这里是借助CAD坐标里程取整计算坡度，所以不需要修改为双轴
		if (fabs(LL) < 0.001) 
			LL = 100.0;
		int inf=1;

		if (BPD_array[i][1] - BPD_array[i - 1][1] < 0)
			inf = -1;

		if(Xsw==1)//小数位取一位
			ii=inf*((int)(fabs(BPD_array[i][1]-BPD_array[i-1][1])*unit*10/LL+0.5))/10.0;//坡度
		else if(Xsw==2)//小数位取两位
			ii=inf*((int)(fabs(BPD_array[i][1]-BPD_array[i-1][1])*unit*100/LL+0.5))/100.0;//坡度
		else if(Xsw==3)//小数位取三位
			ii=inf*((int)(fabs(BPD_array[i][1]-BPD_array[i-1][1])*unit*1000/LL+0.5))/1000.0;//坡度
		else//小数位取零位
			ii=inf*(int)(fabs(BPD_array[i][1]-BPD_array[i-1][1])*unit/LL+0.5);//坡度*/

		BPD_array[i][3]=ii;//坡度

		//计算坡度代数差
		if(i>1)
		{
			deltaI = ii-Bii;
			//			double Rshu = Get_Rshu(DV,deltaI);//按规范取值Rshu
			if(deltaI > 0)//凹形坡
				Rshu = A_YBRshu;
			else//凸形坡
				Rshu = T_YBRshuMin;
			//if(BPD_array[i-1][2] < 1.0 && Rshu>1.0)//尚未付值
			//	BPD_array[i-1][2] = Rshu;
			//	ads_printf(L"Rshu=%lf\n",BPD_array[i-1][2]);
		}
	}

	for (i = 0; i < NBPD; i++)//里程取整
	{
		LC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
		//LC = (BPD_array[i][0] - X0)*HBVSCALE + K0;
		
		if (LC > K0 + PDLmin && LC < DLArray[DLNum - 1].TYLC - PDLmin)
		{
			CString  GH;
			double xlc = XLC(LC, GH);//现场里程
			if (QZZ > 1.0 && LC < DLArray[DLNum - 1].TYLC - 0.001)
			{
				int fh;
				sign(fh, xlc);
				xlc1 = fh * int(fabs(xlc) / QZZ + 0.5) * QZZ;
				//				dmltockml(GH,xlc1,ckml);
				LC = TYLC(xlc1);//重新计算连续里程
			}
			if (LC > DLArray[DLNum - 1].TYLC + 0.001)
			{
				LC = DLArray[DLNum - 1].TYLC;
				NBPD = i + 1;//终止
			}
			BPD_array[i][0] = Trs_Coor_System(LC, MILE, TO_DRAW);
			//BPD_array[i][0]=(LC-K0)/HBVSCALE + X0;
		}		
	} 
	if(QZZ>1.0)
	{
		for(i=1;i<NBPD;i++)//计算标高
		{
			LL = (BPD_array[i][0] - BPD_array[i - 1][0])*HBVSCALE;
			BPD_array[i][1] = BPD_array[i - 1][1] + BPD_array[i][3] * LL / unit;
		}
	}

	//限制接坡起终坡度
	if (m_JPInforArray_slope.size() > 0 && m_JPInforArray_slope[0].Is_JP == 1
		&&(m_JPInforArray_slope[0].Is_JP_Bef == 1||m_JPInforArray_slope[0].Is_JP_Aft == 1))
		LimitJPSlope_StEd();

	BpdToCM();

	//计算BPDYcor数组
	double cml;
	for (i = 0; i < NBPD; i++) 
	{ 
		verts[i].x=BPD_array[i][0]; 
		verts[i].y=BPD_array[i][1];
		verts[i].z=0.0;
		cml = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
		BPDYcor[i] = Trs_Coor_System(rdes_level(cml), LEVEL, TO_DRAW);
	}

	AcGePoint3d spt, ept, cpt;
	if (pWd != NULL) 
	{
		pWd->subEntityTraits().setSelectionMarker(1);
		pWd->subEntityTraits().setLayer(ZPId);//先设层
		pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
		for (i = 1; i < NBPD; i++)
		{
			spt[X] = BPD_array[i - 1][0], spt[Y] = BPD_array[i - 1][1];
			ept[X] = BPD_array[i][0], ept[Y] = BPD_array[i][1];

			double NoteXZPD;
			//考虑坡度折减
			/*
			if(mGuiFanCS.ZheJian)
			{
			//1.先作隧道折减
			beta = pdzj_Tun(BLC,LC);
			MaxPD = beta*MaxPD;
			//2.作曲线折减
			pdzj(BLC,LC,&MAXPD1,&MAXPD2);
			}*/				
			//检测坡度是否大于最大坡度
			NoteXZPD = MAXPD;
			if(Xsw == 1) 
				NoteXZPD = (int)(NoteXZPD * 10.0 + 0.00001) / 10.0;
			else if(Xsw == 2) 
				NoteXZPD = (int)(NoteXZPD * 100.0 + 0.00001) / 100.0;
			else if(Xsw == 3) 
				NoteXZPD = (int)(NoteXZPD * 1000.0 + 0.00001) / 1000.0;
			else 
				NoteXZPD = (int)(NoteXZPD + 0.00001);

			PDDSC1 = YBPCMin;	//坡度代数差
			if (fabs(BPD_array[i][3]) > NoteXZPD || fabs(BPD_array[i][3] - BPD_array[i - 1][3]) > PDDSC1)//2005.3.9
				G_makeline(pWd,spt,ept,1);
			else
				G_makeline(pWd,spt,ept,7);
		}
		double i1, i2, di, T, E, yb, csml, ceml, R = 0;
		ACHAR mes[80];
		int Color=2;
		for (i = 1; i < NBPD - 1; i++)//绘制竖曲线
		{
			//1.计算竖曲线切线
			i1 = BPD_array[i][3];
			i2 = BPD_array[i + 1][3];
			di = fabs(i1 - i2);
			if (BPD_array[i][2] < 0.1)//竖曲线为0
				continue;
			T = BPD_array[i][2] * di / (2 * 100.0);	//R*△i/2
			//2.计算竖曲线范围，竖曲线颜色
			csml = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE) - T;	//竖曲线起始里程
			ceml = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE) + T;	//竖曲线终止里程
			Color = 2;
			R = BPD_array[i][2];
			Color = CheckVerR(i, R);
			if(2 * T < YBLShuMin)
				Color = 1;
			if (ceml > DLArray[DLNum - 1].TYLC + 0.001)
				ceml = DLArray[DLNum-1].TYLC;
			if(csml < K0)
				csml = K0;
			//3.绘制竖曲线
			DrawVerCurLine(csml, ceml, Color, pWd);
			//4.绘制竖曲线范围的横向
			yb = BPDYcor[i] - 6.0;
			spt[X] = BPD_array[i][0] - T / HSCALE, spt[Y] = yb;
			ept[X] = BPD_array[i][0] + T / HSCALE, ept[Y] = yb;
			G_makeline(pWd, spt, ept, 3);
			//5.绘制圆曲线半径文字
			cpt[X] = (spt[X] + ept[X]) / 2.0;
			cpt[Y] = BPDYcor[i] + 3.0;
			_stprintf(mes, L"R=%0.2lf", BPD_array[i][2]);
			double l = _tcslen(mes)*texth;
			double Oritexth;
			Oritexth = texth;
			while (T < l && texth>0.2)//需要调整字高
			{
				texth = texth - 0.5;
				l = _tcslen(mes)*texth;
			}
			G_maketext(pWd, cpt, mes, 0, 0.75*texth, 0, 1);
			//6.绘制竖曲线纵距文字
			E = T * T / 2 / BPD_array[i][2];
			cpt[Y] = yb - 3.0;
			_stprintf(mes, L"E=%0.2lf", E);
			G_maketext(pWd, cpt, mes, 0, 0.75*texth, 0, 1);
			//7.绘制竖曲线长度文字
			cpt[Y] = yb + 3.0;
			_stprintf(mes, L"c=%0.0lf", 2 * T);
			G_maketext(pWd, cpt, mes, 0, 0.75*texth, 0);
			//8.绘制竖曲线范围横向两端的两条小竖线
			spt[Y] = yb - 3.0, cpt[X] = spt[X], cpt[Y] = yb + 3.0;
			G_makeline(pWd, spt, cpt, 3);
			ept[Y] = yb - 3.0, cpt[X] = ept[X], cpt[Y] = yb + 3.0;
			G_makeline(pWd, ept, cpt, 3);

			texth = Oritexth;
		}
	} 
	else 
	{       
		assert(Adesk::kFalse);          
	}

	BZBPD(pWd); 

	DrawLevelCtrlPt(pWd);	//绘高程控制点

	delete []verts;
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}
//绘sml到eml的坡度线 考虑竖曲线，
void GTZDM_ROAD::DrawVerCurLine(double sml,double eml,int Color,AcGiWorldDraw* pWd)
{
	double cml;
	double Len=5;
	AcGePoint3d spt,ept;

	
	//spt[X] = (sml-K0)/HBVSCALE+X0;
	//spt[Y] = rdes_level(sml)-H0+Y0;
	spt[X] = Trs_Coor_System(sml, MILE, TO_DRAW);
	spt[Y] = Trs_Coor_System(rdes_level(sml), LEVEL, TO_DRAW);
	spt[Z] = ept[Z]= 0;
	cml = sml+Len;
	while(cml<eml)
	{ 
		
		//ept[X] = (cml-K0)/HBVSCALE+X0;
		//ept[Y] = rdes_level(cml)-H0+Y0;
		ept[X] = Trs_Coor_System(cml, MILE, TO_DRAW);
		ept[Y] = Trs_Coor_System(rdes_level(cml), LEVEL, TO_DRAW);
		G_makeline(pWd, spt, ept, Color);
		spt[X] = ept[X], spt[Y] = ept[Y];
		cml += Len;
	}	
	cml = eml;
	ept[X] = Trs_Coor_System(cml, MILE, TO_DRAW);
	ept[Y] = Trs_Coor_System(rdes_level(cml), LEVEL, TO_DRAW);
	G_makeline(pWd,spt,ept,Color);

}

Acad::ErrorStatus GTZDM_ROAD::subExplode(AcDbVoidPtrArray& entitySet) const 
{  
	AcDbVoidPtrArray entset;
	AcDbObjectId entId;
	
	AcGePoint3dArray  pArr;
	int i=0,j=0,k=0,trow=0;  
	double CSB[6],array[MAXBPDNUM][4];
	pArr.setLogicalLength(NBPD);
	for (i = 0; i < NBPD; i++) 
	{ 
		pArr[i].set(BPD_array[i][0],BPD_array[i][1],0.0);
	};
	
	AcDb2dPolyline *pPline = new AcDb2dPolyline(AcDb::k2dSimplePoly,pArr,0.0, Adesk::kFalse);
	pPline->setColorIndex(6);	
	entset.append(pPline);
	CSB[0]=K0;CSB[1]=H0;CSB[2]=X0;CSB[3]=Y0;CSB[4]=HBVSCALE;CSB[5]=texth;
	for (i = 0; i < NBPD; i++) 
	{   
		array[i][0]=BPD_array[i][0]; array[i][1]=BPD_array[i][1];
		array[i][2]=BPD_array[i][2]; array[i][3]=BPD_array[i][3];
	};
	
	//    BZBPD1( array,NBPD,CSB,NLC,GLbz);战士关闭
	
	entitySet=entset;
	return Acad::eOk;

}

// Files data in from a DWG file.
Acad::ErrorStatus GTZDM_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();
	int j,k;
	AcDbObject::dwgInFields(pFiler);
	// For wblock filing we wrote out our owner as a hard
	// pointer ID so now we need to read it in to keep things
	// in sync.
	if (pFiler->filerType() == AcDb::kWblockCloneFiler) {
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}
	pFiler->readItem(&NBPD);
	pFiler->readItem(&NLC);
	pFiler->readItem(&NH);
	pFiler->readItem(&GLbz );

	pFiler->readItem(&texth);
	pFiler->readItem(&VSCALE );
	pFiler->readItem(&HSCALE);
	pFiler->readItem(&HBVSCALE);
	pFiler->readItem(&K0 );
	pFiler->readItem(&X0);
	pFiler->readItem(&PMY0);
	pFiler->readItem(&BMLCY0);
	pFiler->readItem(&DMLCY0 );
	pFiler->readItem(&DMBGY0 );
	pFiler->readItem(&PDY0 );
	pFiler->readItem(&SJBGY0 );
	pFiler->readItem(&H0);
	pFiler->readItem(&Y0);
	pFiler->readItem(&EK );
	pFiler->readItem(&Lmin);
	pFiler->readItem(&m_Is_JP);
	pFiler->readItem(&m_Frame_Index);
	for (j = 0; j < NBPD; j++)
	{
		for (k = 0; k < 4; k++)
			pFiler->readItem(&BPD_array[j][k]);
	};

	for (j=0;j<NBPD;j++)
		pFiler->readItem(&BPDYcor[j]);

	pFiler->readItem(&DLNum);
	DLArray.RemoveAll();
	ACHAR *ch=NULL;
	BAS_DRAW_FUN::DLBZ DlObj;
	for(int i=0;i<DLNum;i++)
	{
		pFiler->readItem(&ch);
		DlObj.BGH.Format(L"%s",ch);

		pFiler->readItem(&ch);
		//DlObj.EGH=ch;
		DlObj.EGH.Format(L"%s",ch);

		pFiler->readItem(&DlObj.BLC);

		pFiler->readItem(&DlObj.ELC);

		DLArray.Add(DlObj);
	}
	GetDLTylc();

	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus GTZDM_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	int j,k;
	AcDbObject::dwgOutFields(pFiler);
	// Since objects of this class will be in the Named
	// Objects Dictionary tree and may be hard referenced
	// by some other object, to support wblock we need to
	// file out our owner as a hard pointer ID so that it
	// will be added to the list of objects to be wblocked.
	//
	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());
	pFiler->writeItem(NBPD);
	pFiler->writeItem(NLC);
	pFiler->writeItem(NH);
	pFiler->writeItem(GLbz );

	pFiler->writeItem( texth);
	pFiler->writeItem( VSCALE );
	pFiler->writeItem( HSCALE);
	pFiler->writeItem( HBVSCALE);
	pFiler->writeItem( K0 );
	pFiler->writeItem( X0);
	pFiler->writeItem( PMY0);
	pFiler->writeItem( BMLCY0);
	pFiler->writeItem( DMLCY0 );
	pFiler->writeItem( DMBGY0 );
	pFiler->writeItem( PDY0 );
	pFiler->writeItem( SJBGY0 );
	pFiler->writeItem( H0);
	pFiler->writeItem( Y0);
	pFiler->writeItem( EK );
	pFiler->writeItem( Lmin);
	pFiler->writeItem(m_Is_JP);
	pFiler->writeItem( m_Frame_Index);
	for (j=0;j<NBPD;j++)
	{   for (k=0;k<4;k++)  
	pFiler->writeItem( BPD_array[j][k]);
	};
	for (j=0;j<NBPD;j++)
		pFiler->writeItem( BPDYcor[j]);

	pFiler->writeItem(DLNum);
	const ACHAR *ch=NULL;
	for(int i=0;i<DLNum;i++)
	{
		BAS_DRAW_FUN::DLBZ DlObj=DLArray[i];		
		ch=_tcsdup(DlObj.BGH);
		pFiler->writeItem(ch);		
		ch=_tcsdup(DlObj.EGH);
		pFiler->writeItem(ch);		
		pFiler->writeItem(DlObj.BLC);		
		pFiler->writeItem(DlObj.ELC);
	}

	return pFiler->filerStatus();
}

// Files data in from a DXF file.
//
Acad::ErrorStatus GTZDM_ROAD::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();

	Acad::ErrorStatus es;
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }

	// Check if we're at the right subclass getData marker.
	//
	if (!pFiler->atSubclassData(L"GTZDM")) {
		ads_printf(L"\n error 10010 类型有错 \n");
		return Acad::eBadDxfSequence;
	}

	struct resbuf inbuf;
	ACHAR *m_modal;
	int k,j,kj;
	double dinit[15];
	k= 0 ;
	j = 0;
	kj=1  ;
	m_modal=NULL;
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfText)
		{
			_tcscpy(m_modal,inbuf.resval.rstring);
		}
		else
		{
		}
	}

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16 )
		{
			NBPD = inbuf.resval.rint;
		}
		else
		{
		}
	};

	es = pFiler->readItem(&inbuf);   
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+2 )
		{
			NLC = inbuf.resval.rint;
		}
		else
		{
		}
	};

	es = pFiler->readItem(&inbuf); 
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+3 )
		{
			NH = inbuf.resval.rint;
		}
		else
		{
		}
	};

	es = pFiler->readItem(&inbuf); 
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt16+4)
		{
			GLbz = inbuf.resval.rint;
		}
		else
		{
		}
	};

	es = pFiler->readItem(&inbuf); 
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal )
		{
			texth = inbuf.resval.rreal;
		}
		else
		{
		}
	};

	for (  j=0;j<15;j++)
	{  
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{
				dinit[j]= inbuf.resval.rreal;
			}
			else		   {		   }
		};
	};
	VSCALE=dinit[0] ;
	HSCALE=dinit[1];
	HBVSCALE=dinit[2];
	K0=dinit[3] ;
	X0=dinit[4] ;
	PMY0=dinit[5] ;
	BMLCY0=dinit[6];
	DMLCY0=dinit[7];
	DMBGY0=dinit[8];
	PDY0=dinit[9];
	SJBGY0=dinit[10];
	H0=dinit[11];
	Y0=dinit[12];
	EK=dinit[13];
	Lmin=dinit[14];

	for (  j=0;j<NBPD;j++)
	{  for (  k=0;k<4;k++)  
	{

		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1 )
			{
				BPD_array[j][k]= inbuf.resval.rreal;
			}
			else
			{
			}
		}
	};
	};

	return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus GTZDM_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();

	int j,k=0;
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass, "GTZDM");
	pFiler->writeItem(AcDb::kDxfInt16,NBPD);
	pFiler->writeItem(AcDb::kDxfInt16+2,NLC);
	pFiler->writeItem(AcDb::kDxfInt16+3,NH);
	pFiler->writeItem(AcDb::kDxfInt16+4,GLbz);

	pFiler->writeItem(AcDb::kDxfReal,texth);

	pFiler->writeItem(AcDb::kDxfReal+1, VSCALE );
	pFiler->writeItem(AcDb::kDxfReal+1,  HSCALE);
	pFiler->writeItem(AcDb::kDxfReal+1,  HBVSCALE);
	pFiler->writeItem(AcDb::kDxfReal+1,  K0 );
	pFiler->writeItem(AcDb::kDxfReal+1,  X0);
	pFiler->writeItem(AcDb::kDxfReal+1,  PMY0);
	pFiler->writeItem(AcDb::kDxfReal+1,  BMLCY0);
	pFiler->writeItem(AcDb::kDxfReal+1,  DMLCY0 );
	pFiler->writeItem(AcDb::kDxfReal+1,  DMBGY0 );
	pFiler->writeItem(AcDb::kDxfReal+1,  PDY0 );
	pFiler->writeItem(AcDb::kDxfReal+1,  SJBGY0 );
	pFiler->writeItem(AcDb::kDxfReal+1,  H0);
	pFiler->writeItem(AcDb::kDxfReal+1,  Y0);
	pFiler->writeItem(AcDb::kDxfReal+1,  EK );
	pFiler->writeItem(AcDb::kDxfReal+1,  Lmin);

	for (j=0;j<NBPD;j++)
	{   for (k=0;k<4;k++)  
	pFiler->writeItem(AcDb::kDxfReal+1,BPD_array[j][k]);
	ads_printf(L"k=%d j=%d %d %lf\n",j,k,k+4*j+1,BPD_array[j][k]);
	};

	return pFiler->filerStatus();
}

Acad::ErrorStatus GTZDM_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											   AcDbIntArray& osnapModes,
											   AcDbIntArray& geomIds) const
{
	AcGePoint3d Pt; 
	assertReadEnabled();
	// 交点
	// ads_printf(L"\n NBPD=%d",NBPD);
	int i;
	for ( i = 0; i < NBPD; i++) 
	{  
		Pt.set(BPD_array[i][0],BPD_array[i][1],0.0);
		gripPoints.append(Pt);
	};

	for ( i = 0; i < NBPD-1; i++) 
	{
		double  X1=(BPD_array[i][0]+BPD_array[i+1][0])*0.5;
		double  Y1=(BPD_array[i][1]+BPD_array[i+1][1])*0.5;
		Pt.set(X1,Y1,0.0);
		gripPoints.append(Pt);
	};
	double X1,Y1;
	for ( i = 1; i < NBPD-1; i++) 
	{
		X1=BPD_array[i][0];
		Y1=BPDYcor[i];
		Pt.set(X1,Y1,0.0);
		gripPoints.append(Pt);
	};

	return Acad::eOk;
}

Acad::ErrorStatus GTZDM_ROAD::subGetOsnapPoints(
	AcDb::OsnapMode   osnapMode,
	int               gsSelectionMark,
	const AcGePoint3d&    pickPoint,
	const AcGePoint3d&    lastPoint,
	const AcGeMatrix3d&   viewXform,
	AcGePoint3dArray& snapPoints,

	AcDbIntArray&     geomIds) const 
{    assertReadEnabled();
Acad::ErrorStatus es=Acad::eOk;
//	if (gsSelectionMark==0){acedAlert(L"2"); return Acad::eOk;}
if (osnapMode!=AcDb::kOsModeEnd &&osnapMode!=AcDb::kOsModeNear
	&& osnapMode != AcDb::kOsModeMid
	&&osnapMode!=AcDb::kOsModePerp &&osnapMode!=AcDb::kOsModeCen
	&&osnapMode!=AcDb::kOsModeIns  ) 
{ return Acad::eOk;  };

AcGePoint3d Pt;

int i;

switch (osnapMode) 
{
case AcDb::kOsModeEnd :
	//变坡点

	for ( i = 0; i <NBPD; i++) 
	{   
		Pt.set(BPD_array[i][0],BPD_array[i][1],0.0);
		snapPoints.append(Pt);
		//			acutPrintf(L"i=%d,x=%lf,h=%lf\n",i,BPD_array[i][0],BPD_array[i][1]);
	};
	break;
default :       return Acad::eOk;

}
return Acad::eOk;
}

Acad::ErrorStatus GTZDM_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,
												  const AcGeVector3d& offset)
{  // AcDbIntArray moveNO;
	int j,bz,k,i;
	double dlc,LL,II,LC,yy,HH,II1=0,II2=0;
	AcGePoint3d startPt,endPt,centerPt;
	assertWriteEnabled();
	for ( j=0;j<indices.length();j++)
	{   // ads_printf(L"\n indices.length()=%d,j=%d  indices[j]=%d",indices.length(),j,indices[j]);
		// 变坡点序号 0~NBPD-1

		if (indices[j]<NBPD) {
			bz=1;
			k=indices[j];
			// 整单位里程移动
			dlc = offset[X];

			BPD_array[k][0] = BPD_array[k][0] + dlc;
			if(TDFS==1)//任意拖动
				BPD_array[k][1] = BPD_array[k][1] + offset[Y];
			else if(TDFS==2)//前坡度不变拖动
			{
				if(k > 0)
					BPD_array[k][1]=BPD_array[k-1][1]+(BPD_array[k][0]-BPD_array[k-1][0])*HBVSCALE*BPD_array[k][3]/100.0;
				else
					BPD_array[k][1]=BPD_array[k][1]+offset[Y];
			}
			else//后坡度不变拖动
			{
				if(k==NBPD-1) BPD_array[k][1]=BPD_array[k][1]+offset[Y];
				else
					BPD_array[k][1]=BPD_array[k+1][1]-(BPD_array[k+1][0]-BPD_array[k][0])*HBVSCALE*BPD_array[k+1][3]/100.0;
			}

			if (k>0)
			{ 
				i = k;
				LL = (BPD_array[i][0] - BPD_array[i - 1][0])*HSCALE;
				if (fabs(LL) < 0.001)
					LL = 100.0;
				II = (BPD_array[i][1] - BPD_array[i - 1][1]) * VSCALE * 100.0 / LL;

				int inf = 1, unit = 1000;
				if (GLbz == 1) unit = 100.0;
				if (BPD_array[i][1] - BPD_array[i - 1][1] < 0)
					inf = -1;

				if (Xsw == 1)//小数位取一位
					II = inf*((int)(fabs(II) * 10 + 0.5)) / 10.0;//坡度
				else if (Xsw == 2)//小数位取两位
					II = inf*((int)(fabs(II) * 100 + 0.5)) / 100.0;//坡度
				else if (Xsw == 3)//小数位取三位
					II = inf*((int)(fabs(II) * 1000 + 0.5)) / 1000.0;//坡度
				else//小数位取零位
					II = inf*(int)(fabs(II) + 0.5);//坡度

				//LC=(BPD_array[i][0]-X0)*HBVSCALE+K0;
				//HH=BPD_array[i][1]-Y0 +H0;    // 设计高程
				LC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
				HH = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
				yy = BPD_array[i][1] - BPD_array[i - 1][1]; // 高差
				// 整单位坡度移动 
				double tempLevel = Trs_Coor_System(BPD_array[i - 1][1], LEVEL, TO_SLOPE) + LL / 100.0 * II;
				//BPD_array[i][1] = BPD_array[i - 1][1] + LL / 100.0 * II;
				BPD_array[i][1] = Trs_Coor_System(tempLevel, LEVEL, TO_DRAW);
				HH = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
				//HH=BPD_array[i][1]-Y0 +H0;    // 设计高程
				ads_printf(L"\n 前坡段 LL=%-7.2f I=%-7.1f HH=%-7.3f ", LL, II, HH);
			}
			if (k < NBPD - 1)
			{  
				i = k;
				//LL = (BPD_array[i + 1][0] - BPD_array[i][0])*HBVSCALE;
				LL = (BPD_array[i + 1][0] - BPD_array[i][0])*HSCALE;
				if (fabs(LL) < 0.001) 
					LL = 100.0;
				II = (BPD_array[i + 1][1] - BPD_array[i][1]) * VSCALE * 100.0 / LL;
				//LC = (BPD_array[i + 1][0] - X0)*HBVSCALE + K0;
				LC = Trs_Coor_System(BPD_array[i + 1][0], MILE, TO_SLOPE);
				//HH = BPD_array[i + 1][1] - Y0 + H0;    // 设计高程
				HH = Trs_Coor_System(BPD_array[i + 1][1], LEVEL, TO_SLOPE);
				yy = BPD_array[i + 1][1] - BPD_array[i][1]; // 高差
				ads_printf(L"后坡段 LL=%-7.2f I=%-7.2f ", LL, II);
			};  

		}
		else if ((indices[j]>=NBPD)&&(indices[j]<2*NBPD-1))//为变坡点中点
		{
			k=indices[j]-NBPD+1; // 该切线边对应的变坡点号
			if (k<=NBPD-2 && k>1)
			{ 	 
				AcGePoint3d spt,ept;
				spt.set(BPD_array[k-1][0],BPD_array[k-1][1],0.0);
				ept.set(BPD_array[k][0],BPD_array[k][1],0.0);
				AcGeLine3d pdx(spt,ept),Newpdx(spt,ept);
				AcGeMatrix3d mt;

				//ads_printf(L"dlc=%lf\n",dlc);
				/*
				Newoffset.x = dlc;
				if(offset.x>0.001)
				Newoffset.y = offset.y*fabs(dlc/offset.x);
				else
				Newoffset.y = offset.y;
				Newoffset.z = 0.0; 
				Newoffset.set(dlc,Newoffset.y,0.0);
				*/

				mt.setToTranslation(offset);
				pdx.transformBy(mt);//新的坡度线

				spt.set(BPD_array[k-2][0],BPD_array[k-2][1],0.0);
				ept.set(BPD_array[k-1][0],BPD_array[k-1][1],0.0);
				AcGeLine3d Spdx(spt,ept);//前坡度线
				AcGePoint3d intPt;
				pdx.intersectWith(Spdx,intPt);
				//调整为整数桩
				//				DWL = 10.0;
				double dx = intPt.x-BPD_array[k-1][0];
				double dy = intPt.y-BPD_array[k-1][1];
				//			dlc=(long int )(dx*HBVSCALE/DWL);
				//			    dlc=dlc/HBVSCALE*DWL;
				dlc = dx;
				if(fabs(dx)>0.001)
				{
					BPD_array[k-1][0]=BPD_array[k-1][0]+dlc;
					BPD_array[k-1][1]=BPD_array[k-1][1]+dy*fabs(dlc/dx);
					AcGeVector3d Newoffset;
					Newoffset.set(dlc,dy*fabs(dlc/dx),0.0);
					mt.setToTranslation(Newoffset);
					Newpdx.transformBy(mt);//新的坡度线
					spt.set(BPD_array[k][0],BPD_array[k][1],0.0);
					ept.set(BPD_array[k+1][0],BPD_array[k+1][1],0.0);
					AcGeLine3d Epdx(spt,ept);//后坡度线
					Newpdx.intersectWith(Epdx,intPt);
					BPD_array[k][0]=intPt.x;
					BPD_array[k][1]=intPt.y;//交点即为新变坡点
				}
			};			
		}
		else if((indices[j]>=2*NBPD-1)&&(indices[j]<3*NBPD-3))//为竖曲线中点
		{
			k=indices[j]-2*NBPD+2; // 该交点的变坡点号
			double curx,cury,prex,prey,aftx,afty,XonArc,YonArc,R;
			if (k<=NBPD-2 && k>=1)
			{			
				//prex = (BPD_array[k-1][0]-X0)*HBVSCALE+K0;
				prex = Trs_Coor_System(BPD_array[k - 1][0], MILE, TO_SLOPE);
				//prey = BPD_array[k-1][1]-Y0 +H0;
				prey = Trs_Coor_System(BPD_array[k - 1][1], LEVEL, TO_SLOPE);
				//curx = (BPD_array[k][0]-X0)*HBVSCALE+K0;
				curx = Trs_Coor_System(BPD_array[k][0], MILE, TO_SLOPE);
				//cury = BPD_array[k][1]-Y0 +H0;
				cury = Trs_Coor_System(BPD_array[k][1], LEVEL, TO_SLOPE);
				//aftx = (BPD_array[k+1][0]-X0)*HBVSCALE+K0;
				aftx = Trs_Coor_System(BPD_array[k + 1][0], MILE, TO_SLOPE);
				//afty = BPD_array[k+1][1]-Y0 +H0;
				afty = Trs_Coor_System(BPD_array[k + 1][1], LEVEL, TO_SLOPE);
				//XonArc = (BPD_array[k][0]-X0)*HBVSCALE+K0;
				XonArc = Trs_Coor_System(BPD_array[k][0], MILE, TO_SLOPE);
				//YonArc = BPDYcor[k]-Y0 +H0;
				YonArc = BPDYcor[k];
				XonArc = XonArc + offset[X] * HSCALE;
				YonArc += offset[Y] * VSCALE;
				//AcDbLine L1(AcGePoint3d(prex,prey,0.0),AcGePoint3d(curx,cury,0.0));
				//AcDbLine L2(AcGePoint3d(curx,cury,0.0),AcGePoint3d(aftx,afty,0.0));
				//R=radiusfree(&L1,&L2,AcGePoint3d(XonArc,YonArc,0.0));

				//CheckVerR(k,R);
				//if(R > 0.1)
				//{
				//	if(VerRadius==0)
				//		BPD_array[k][2]=R;
				//	else if(VerRadius ==1)
				//		BPD_array[k][2] = int(R+0.5);
				//	else if(VerRadius==2)
				//		BPD_array[k][2] = int(R/10+0.5)*10;
				//	else
				//		BPD_array[k][2] = int(R/100+0.5)*100;
				//}
			}
		}
	};

	return Acad::eOk;
}

int GTZDM_ROAD::CheckVerR(int ii,double& R)
{
	double i1,i2,di,T;
	double csml,ceml,qsml,qeml,hsml,heml,qT,hT,R1,R2;
	int Color =2;

	i1=BPD_array[ii][3];
	i2=BPD_array[ii+1][3];
	di=fabs(i1-i2);
	T = R*di/200.0;
	csml = Trs_Coor_System(BPD_array[ii][0], MILE, TO_SLOPE) - T;
	ceml = Trs_Coor_System(BPD_array[ii][0], MILE, TO_SLOPE) + T;

	R1=R2=10000000000;
	if(ii>1&&ii<NBPD-2)
	{
		qT = BPD_array[ii - 1][2] * fabs(BPD_array[ii - 1][3] - BPD_array[ii][3]) / 200;
		qsml = Trs_Coor_System(BPD_array[ii - 1][0], MILE, TO_SLOPE) - qT;
		qeml = Trs_Coor_System(BPD_array[ii - 1][0], MILE, TO_SLOPE) + qT;
		hT = BPD_array[ii + 1][2] * fabs(BPD_array[ii + 1][3] - BPD_array[ii + 2][3]) / 200;
		hsml = Trs_Coor_System(BPD_array[ii + 1][0], MILE, TO_SLOPE) - hT;
		heml = Trs_Coor_System(BPD_array[ii + 1][0], MILE, TO_SLOPE) + hT;
		if(csml<qeml||ceml>hsml)
		{
			if(csml<qeml)//与前面穿袖
				R1 = ((BPD_array[ii][0] - BPD_array[ii - 1][0]) * HBVSCALE - qT) * 200 / di;
			if(ceml>hsml)//与后面穿袖
				R2 = ((BPD_array[ii + 1][0] - BPD_array[ii][0]) * HBVSCALE - hT) * 200 / di;
			R = R1 < R2 ? R1 : R2;
			Color =1;
		}
	}
	else if (ii == 1 && NBPD>2)
	{
		hT = BPD_array[ii + 1][2] * fabs(BPD_array[ii + 1][3] - BPD_array[ii + 2][3]) / 200;
		hsml = Trs_Coor_System(BPD_array[ii + 1][0], MILE, TO_SLOPE) - hT;
		heml = Trs_Coor_System(BPD_array[ii + 1][0], MILE, TO_SLOPE) + hT;
		if(csml<K0||ceml>hsml)
		{
			if (csml < Trs_Coor_System(BPD_array[0][0], MILE, TO_SLOPE))//与前面穿袖
				R1 = ((BPD_array[ii][0] - BPD_array[ii - 1][0])*HBVSCALE) * 200 / di;
			if(ceml>hsml)//与后面穿袖
				R2 = ((BPD_array[ii + 1][0] - BPD_array[ii][0])*HBVSCALE - hT) * 200 / di;
			R = R1<R2?R1:R2;
			Color = 1;
		}
	}
	else if(ii==NBPD-2)
	{
		qT = BPD_array[ii-1][2]*fabs(BPD_array[ii-1][3]-BPD_array[ii][3])/200;
		qsml = Trs_Coor_System(BPD_array[ii - 1][0], MILE, TO_SLOPE) - qT;
		qeml = Trs_Coor_System(BPD_array[ii - 1][0], MILE, TO_SLOPE) + qT;
		if(csml<qeml||ceml>DLArray[DLNum-1].TYLC)
		{
			if(csml<qeml)//与前面穿袖
				R1 = ((BPD_array[ii][0]-BPD_array[ii-1][0])*HBVSCALE-qT)*200/di;		
			if (ceml>Trs_Coor_System(BPD_array[NBPD - 1][0], MILE, TO_SLOPE))
				R2 = ((BPD_array[ii+1][0]-BPD_array[ii][0])*HBVSCALE)*200/di;
			R = R1<R2?R1:R2;
			Color = 1;
		}
	}
	return Color;
}

void GTZDM_ROAD::subList() const 
{ int i;
assertReadEnabled();
ads_printf(L"\n 类：\t%s",isA()->name());
double LC,HH,LL,II,yy;
HH = Trs_Coor_System(BPD_array[0][1], LEVEL, TO_SLOPE);
LC = Trs_Coor_System(BPD_array[0][0], MILE, TO_SLOPE);
//HH=BPD_array[0][1]-Y0 +H0;    // 起点设计高程
//LC=(BPD_array[0][0]-X0)*HBVSCALE+K0;// 起点里程
ads_printf(L"\n 1   %-10.3f %-10.3f %-10.3f  ",LC,HH,BPD_array[0][2]);
for ( i=1;i<NBPD;i++)
{
	LL = (BPD_array[i][0] - BPD_array[i - 1][0]) * HSCALE;
	if (fabs(LL) < 0.001)
		LL=1000.0;
	II=(BPD_array[i][1]-BPD_array[i-1][1]) * VSCALE * 1000.0 / LL;
	LC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
	HH = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
	//LC = (BPD_array[i][0] - X0)*HBVSCALE + K0;
	//HH = BPD_array[i][1] - Y0 + H0;    // 设计高程
	yy = BPD_array[i][1] - BPD_array[i - 1][1]; // 高差 
	ads_printf(L"\n %2d %-10.3f %-9.3f %-9.3f %-7.3f%-10.3f ",i+1,LC,HH,LL,II,BPD_array[i][2]);
};
}

void  GTZDM_ROAD::setINIT(int nlc,int nh,int Chinese_Engbz,double Texth,double HBSCALE,double XX0,double YY0, double KK0,double HH0)
{
	assertWriteEnabled();
	NLC=nlc;
	NH=nh;
	texth=Texth;
	/*
	HBVSCALE=HBSCALE;
	X0=pORI_SCALE.X_ORI  ;
	Y0=pORI_SCALE.Y_ORI; 
	K0=KK0;
	H0=HH0;*/

	VSCALE=pORI_SCALE.VSCALE; //     高程比例 
	HSCALE=pORI_SCALE.HSCALE; // 水平比例
	HBVSCALE=pORI_SCALE.HBVSCALE; //  水平与高程比例之比   
	//   现场[K0,H0]===纵面图上(X0,Y0) 
	H0=pORI_SCALE.ZMIN;     X0=pORI_SCALE.X_ORI  ; Y0=pORI_SCALE.Y_ORI; 
	//	K0=pORI_SCALE.STARTLC; 
	/*
	if(DLNum>0)
	K0=DLArray[0].ELC;
	else*/
	K0=pORI_SCALE.STARTLC; 
	Xsw = pORI_SCALE.XSW;
	DesHXSW = pORI_SCALE.DesHXSW;
}

void GTZDM_ROAD::setDLM(int iDL,double BLC,double ELC,CString BGH,CString EGH)
	//void JD_CENTER::setDLM(int iDL,double BLC,double ELC,char *BGH,char *EGH)
{  
	DLNum=DLArray.GetSize();
	if (iDL>=0&& iDL<DLNum)
	{
		DLArray[iDL].BGH=BGH;
		DLArray[iDL].EGH=EGH;
		DLArray[iDL].BLC=BLC;
		DLArray[iDL].ELC=ELC;
	}
	GetDLTylc();
}

void GTZDM_ROAD::GetDLTylc()
{//计算每个断链处的统一里程
	CString FLDName;

	double LCB=0;  // LCB 断链的统一里程
	int iDL=0;
	for(iDL=0;iDL<DLNum;iDL++)
	{
		//该断链点统一里程
		if(iDL==0)LCB=DLArray[iDL].ELC;
		else  LCB=LCB+DLArray[iDL].BLC-(DLArray[iDL-1].ELC);
		DLArray[iDL].TYLC=LCB;
	}
}
//  由断链数据表 求某现场里程的统一里程
double GTZDM_ROAD::TYLC(double dml)
{   
	double TYLCB=0,LCB, LCE,TLC,XLC;  // LCB 断链的统一里程
	int iDL=0,bz;
	//起点里程
	CString GH;

	TLC=fabs(dml);
	XLC = TLC;
	bz=-99;
	for (iDL=0;iDL<DLNum;iDL++)   // 
	{
		//该断链点统一里程
		TYLCB=DLArray[iDL].TYLC;

		//该断链后链里程
		LCB=DLArray[iDL].ELC;	
		//下一断链的前链里程
		if(iDL<DLNum-1)LCE=DLArray[iDL+1].BLC; 
		if( ( iDL<DLNum-1 && XLC>=LCB && XLC<=LCE ) || (iDL==DLNum-1 && XLC>=LCB) )  // 落在该段落内
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
//  增加一个断链数据
void GTZDM_ROAD::addDLM(int iDL,double BLC,double ELC,CString EGH  )
	//							 增中断链号，断前里程，断后里程
{
	if(fabs(ELC-BLC)<0.001)ELC=BLC;
	if (iDL>0 && iDL< DLNum )
	{
		if( (iDL<DLNum && BLC>DLArray[iDL-1].ELC && BLC< DLArray[iDL].BLC )
			|| iDL==DLNum )
		{
			assertWriteEnabled();
			BAS_DRAW_FUN::DLBZ newDL;
			DLArray.InsertAt(iDL,newDL);
			DLNum+=1;
			DLArray[iDL].BGH=DLArray[iDL-1].EGH;
			DLArray[iDL].EGH=EGH;
			/*if(iDL<DLNum-1)*/DLArray[iDL+1].BGH=EGH;
			DLArray[iDL].BLC=BLC;
			DLArray[iDL].ELC= ELC;
			double DL=ELC-BLC;
			/*if(iDL<DLNum-1)*/DLArray[iDL+1].BLC += DL;
		}
	}
	GetDLTylc();
}

//  删除一个断链数据
void GTZDM_ROAD::DelDLM(int iDL)
{
	double DL;
	if (iDL>0 && iDL< DLNum-1)
	{
		assertWriteEnabled();

		DL=DLArray[iDL].ELC-DLArray[iDL].BLC;
		DLArray.RemoveAt(iDL);
		DLNum-=1;
		if(iDL<DLNum)
		{
			DLArray[iDL].BGH=DLArray[iDL-1].EGH;
			DLArray[iDL].BLC-=DL;
		}
	}
	GetDLTylc();
}

//传入断链表
void GTZDM_ROAD::setDLB(DLBZ *dlb,int ndl)
{
	assertWriteEnabled();
	DLArray.RemoveAll();
	DLNum=ndl;
	for(int i=0;i<DLNum;i++) 
		DLArray.Add(dlb[i]);

	GetDLTylc();
	//	K0 = DLArray[0].ELC; 
}

double GTZDM_ROAD::XLC1(double cml)
{
	double LCB=0;  // LCB 断链的统一里程
	int iDL=0;
	double dml=0,tylc1;

	dml = cml;
	for(iDL=DLNum-1;iDL>=0;iDL--)
	{
		if(cml>=DLArray[iDL].TYLC)
		{
			//			GH=DLBTMP[iDL].EGH;
			dml=cml-DLArray[iDL].TYLC+DLArray[iDL].ELC;
			tylc1 = TYLC(dml);
			if(fabs(tylc1-cml)<0.001)
				return dml;
			else
				return -dml;
		}
	}
	return dml;
}
//  由断链数据表 求某统一里程的现场里程和冠号
double GTZDM_ROAD::XLC( double cml,CString& GH)
{
	double LCB=0,tylc1;  // LCB 断链的统一里程
	int iDL=0;

	double dml = cml;
	GH = DLArray[0].EGH;
	for(iDL=DLNum-1;iDL>=0;iDL--)
	{
		if(cml>=DLArray[iDL].TYLC)
		{
			GH=DLArray[iDL].EGH;
			dml = cml-DLArray[iDL].TYLC+DLArray[iDL].ELC;
			tylc1 = TYLC(dml);
			if(fabs(tylc1-cml)<0.001)
				return dml;
			else
				return -dml;
		}
	}
	return dml;
}

CString GTZDM_ROAD::XLC(double TYLC)
{
	CString GH;
	double xlc=fabs(XLC(TYLC,GH));
	return LCchr(GH,xlc,NLC);
}

int GTZDM_ROAD::saveBPD(ACHAR *fname)
{   
	assertReadEnabled();
	int i, trow=0;
	double LC,HH,LL,II,yy;
	FILE *fpc;
	ads_printf(L"%s",fname);
	if((fpc=_wfopen(fname,L"w"))!=NULL)
	{     
		LC = Trs_Coor_System(BPD_array[0][0], MILE, TO_SLOPE);
		HH = Trs_Coor_System(BPD_array[0][1], LEVEL, TO_SLOPE);
		//HH=BPD_array[0][1]-Y0 +H0;    // 起点设计高程
		//LC=(BPD_array[0][0]-X0)*HBVSCALE+K0;// 起点里程
		fwprintf(fpc,L"%d\n",NBPD);
		fwprintf( fpc,L"%-10.3f %-10.3f  0\n",LC,HH );
		ads_printf(L"%-10.3f %-10.3f %-10.3f \n",LC,HH,BPD_array[0][2]);
		for ( i=1;i<NBPD;i++)
		{
			LL = (BPD_array[i][0] - BPD_array[i - 1][0])*HSCALE;
			if (fabs(LL)<0.001) 
				LL = 1000.0;
			II = (BPD_array[i][1] - BPD_array[i - 1][1]) * VSCALE *1000.0 / LL;
			LC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
			HH = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
			//LC=(BPD_array[i][0]-X0)*HBVSCALE+K0;
			//HH=BPD_array[i][1]-Y0 +H0;    // 设计高程
			yy=BPD_array[i][1]-BPD_array[i-1][1]; // 高差 
			if (i<NBPD-1) 
			{  
				fwprintf(fpc, L"%-10.3f %-10.3f %-10.3f \n", LC, HH, BPD_array[i][2]);
			}
			else  
				fwprintf(fpc, L"%-10.3f %-10.3f  0\n", LC, HH);

			//          ads_printf(L" %-10.3f %-10.3f %-10.3f \n",LC,HH,BPD_array[i][2]);
		};
		fclose(fpc);
	};
	return 1;
}

void GTZDM_ROAD::setBPD(int BPDi ,double Ri,double XLC,double HY )
{
	assertWriteEnabled();
	if (BPDi>=0 && BPDi<=NBPD-1)  // 可为起、终点
	{  BPD_array[BPDi][0]=XLC;
	BPD_array[BPDi][1]=HY;
	BPD_array[BPDi][2]=Ri;
	} ;	 
} 

int GTZDM_ROAD::getBPD(int BPDi , double *Ri,double *XLCi,double *HYi)
{   
	assertReadEnabled();

	if (BPDi>=0 && BPDi<=NBPD-1)  //  起~终点
	{   
		*XLCi=BPD_array[BPDi][0];
		*HYi=BPD_array[BPDi][1];
		*Ri=BPD_array[BPDi][2];
	};	 
	return NBPD;
}


int GTZDM_ROAD::getK0H0(double *XX0,double *YY0,double *KK0,double *HH0,double *hscale)
{   assertReadEnabled();
*XX0=X0;	*YY0=Y0;
*KK0=K0;    *HH0=H0;
*hscale=HBVSCALE;
return NBPD;
}
int  GTZDM_ROAD::textStyle()
{

	AcDbTextStyleTable *pTextStyleTbl;

	AcDbObjectId pTextStyleTblId;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite);

	if(pTextStyleTbl->getAt(L"宋体",pTextStyleTblId) == Acad::eOk)
	{
		pTextStyleTbl->close();
		return 0;
	}

	AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
	pTextStyleTblRcd->setName(L"宋体");
	pTextStyleTblRcd->setFileName(L"宋体");
	pTextStyleTblRcd->setFont(L"宋体",0,0,134,2);
	pTextStyleTblRcd->setXScale(0.8);
	pTextStyleTbl->add(pTextStyleTblRcd);
	pTextStyleTblRcd->close();
	pTextStyleTbl->close();	
	return 0;
}

//绘制高程控制点
void GTZDM_ROAD::DrawLevelCtrlPt(AcGiWorldDraw* pWd)
{
	//设置参数
	AcGePoint2d OriPt;
	AcGePoint3d OriPt3D, TriRightTop, TriLeftTop, TempPt;
	AcGeVector3d VecRightTop, VecLeftTop;
	VecRightTop.normalize();
	VecLeftTop.normalize();
	ACHAR LevelText[256];
	double TriLen = 5.0;

	for (int i = 0; i < m_LevelCtrlPtArray.size(); i++)
	{
		//绘制上限点
		if (m_LevelCtrlPtArray[i].Style == 1 || m_LevelCtrlPtArray[i].Style == 3)
		{
			VecRightTop.set(cos(pi / 3), sin(pi / 3), 0.0);
			VecLeftTop.set(cos(pi * 2 / 3), sin(pi * 2 / 3), 0.0);
			VecRightTop.normalize();
			VecLeftTop.normalize();
			//计算绘图原点
			OriPt.set(m_LevelCtrlPtArray[i].Cml, m_LevelCtrlPtArray[i].DesHSX);
			OriPt = Data_realtodraw(OriPt);
			OriPt3D.set(OriPt.x, OriPt.y, 0.0);
			TriRightTop = OriPt3D + VecRightTop * TriLen;
			TriLeftTop = OriPt3D + VecLeftTop * TriLen;

			//绘制三角形
			G_makeline(pWd, OriPt3D, TriRightTop, RED);
			G_makeline(pWd, TriRightTop, TriLeftTop, RED);
			G_makeline(pWd, TriLeftTop, OriPt3D, RED);

			//绘制直线
			TempPt = OriPt3D;
			TempPt.x += 30.0;
			G_makeline(pWd, OriPt3D, TempPt, RED);

			//绘制文字
			_stprintf(LevelText, L"高程上限：%.3lf", m_LevelCtrlPtArray[i].DesHSX);
			TempPt.x = OriPt3D.x + 2.0;
			TempPt.y = OriPt3D.y + 1.0;
			G_maketext(pWd, TempPt, LevelText, 0.0, 2.0, RED, 2);
			TempPt.y += 5.0;
			_tcscpy(LevelText, m_LevelCtrlPtArray[i].Note);
			G_maketext(pWd, TempPt, LevelText, 0.0, 2.0, RED, 2);
		}

		//绘制下限点
		if (m_LevelCtrlPtArray[i].Style == 2 || m_LevelCtrlPtArray[i].Style == 3)
		{
			VecRightTop.set(cos(-pi / 3), sin(-pi / 3), 0.0);
			VecLeftTop.set(cos(-pi * 2 / 3), sin(-pi * 2 / 3), 0.0);
			VecRightTop.normalize();
			VecLeftTop.normalize();
			//计算绘图原点
			OriPt.set(m_LevelCtrlPtArray[i].Cml, m_LevelCtrlPtArray[i].DesHXX);
			OriPt = Data_realtodraw(OriPt);
			OriPt3D.set(OriPt.x, OriPt.y, 0.0);
			TriRightTop = OriPt3D + VecRightTop * TriLen;
			TriLeftTop = OriPt3D + VecLeftTop * TriLen;

			G_makeline(pWd, OriPt3D, TriRightTop, RED);
			G_makeline(pWd, TriRightTop, TriLeftTop, RED);
			G_makeline(pWd, TriLeftTop, OriPt3D, RED);

			//绘制直线
			TempPt = OriPt3D;
			TempPt.x += 30.0;
			G_makeline(pWd, OriPt3D, TempPt, RED);

			//绘制文字
			_stprintf(LevelText, L"高程下限：%.3lf", m_LevelCtrlPtArray[i].DesHXX);
			TempPt.x = OriPt3D.x + 2.0;
			TempPt.y = OriPt3D.y - 3.0;
			G_maketext(pWd, TempPt, LevelText, 0.0, 2.0, RED, 2);
			TempPt.y -= 5.0;
			_tcscpy(LevelText, m_LevelCtrlPtArray[i].Note);
			G_maketext(pWd, TempPt, LevelText, 0.0, 2.0, RED, 2);
		}
	}
}

//  增加一个点数据
Adesk::Boolean  GTZDM_ROAD::addBPD(int BPDi,double R,double XLC1,double HY)
{   
	int i,j;
	if (BPDi>0 && BPDi<=(NBPD-1 )&& NBPD<MAXBPDNUM)  // 不为起、终点  NJD的最小值为2  最大值为100
	{  	 
		assertWriteEnabled();

		for (i=NBPD;i>BPDi;i--) // 后移一个 交点
			for ( j=0;j<4;j++) BPD_array[i][j]=BPD_array[i-1][j];
		BPD_array[BPDi][0]=XLC1;
		BPD_array[BPDi][1]=HY;
		BPD_array[BPDi][2]=R;       

		NBPD =NBPD +1;	  
		return 1;  
	} else return 0;
}

//  删除一个 点数据
Adesk::Boolean  GTZDM_ROAD:: DelBPD(int BPDi)
{   
	int i,j;
	if (BPDi>=0 && BPDi<= NBPD-1 && NBPD<MAXBPDNUM)  // 不为起、终点  NJD的最小值为2  最大值为100
	{  	 
		assertWriteEnabled();
		for (i=BPDi;i<=NBPD-1;i++) // 前移一个 交点
			for ( j=0;j<4;j++) BPD_array[BPDi+(i-BPDi)][j]=BPD_array[BPDi+1+(i-BPDi)][j];		   
		NBPD =NBPD -1;	  
		return 1;  
	} else return 0;
}

void GTZDM_ROAD::AdjustBPD(int dlc)
{
	double LC,xlc,xlc1;
	CString GH,GH1;
	BPD_array[0][0]=X0;

	for (int i=1;i<NBPD-1;i++)
	{  
		LC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
		//LC=(BPD_array[i][0]-X0)*HBVSCALE+K0;
		xlc=fabs(XLC(LC,GH));
		xlc1=(int)(xlc/dlc)*dlc;//化为整桩
		BPD_array[i][0] = Trs_Coor_System(xlc1, MILE, TO_DRAW);
		//BPD_array[i][0]-=(xlc-xlc1)/HBVSCALE;
		//判断是否BPD进入前一个断链；
		LC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
		xlc=fabs(XLC(LC,GH1));
		if (!_tcscmp(GH1, GH) && (int)(xlc / dlc)*dlc != xlc)
			//BPD_array[i][0]+=dlc/HBVSCALE;
			BPD_array[i][0] += dlc / HSCALE;
	}
	BPD_array[NBPD - 1][0] = Trs_Coor_System(pORI_SCALE.ENDLC, MILE, TO_DRAW);
	//BPD_array[NBPD-1][0]=(pORI_SCALE.ENDLC-K0)/HBVSCALE+X0;	
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"regen\n",false,true);
}

void GTZDM_ROAD::BpdToCM()
{
	double LC1,LC2,H2,H1,R;
	//------------------------------------------------------

	LC1 = Trs_Coor_System(BPD_array[0][0], MILE, TO_SLOPE);	//20190709	修改采用双轴比例尺
	H1 = Trs_Coor_System(BPD_array[0][1], LEVEL, TO_SLOPE);
	CM[0] =LC1,CM[1]=H1,CM[2]=0.0, CM[3] = 0.0, CM[4]=0.0;
	for (int i = 1; i < NBPD; i++)
	{
		LC2 = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);	//20190709	修改采用双轴比例尺
		H2 = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
		R = BPD_array[i][2];
		CM[i * 5] = LC2, CM[i * 5 + 1] = H2;
		if (LC2 > LC1 + 0.0001)
			CM[i * 5 + 2] = (H2 - H1) / (LC2 - LC1);
		else
			CM[i * 5 + 2] = 0.0;

		CM[i * 5 + 3] = LC2 - LC1;
		CM[i * 5 + 4] = R;
		H1 = H2, LC1 = LC2;
	}
	CM[NBPD * 5] = CM[NBPD * 5 + 1] = CM[NBPD * 5 + 2] = CM[NBPD * 5 + 3] = CM[NBPD * 5 + 4] = 0.0;
	NCM = NBPD+1;
}
/*---------------------------------------------------------*
*        calculate the design level at certain mileage    *
*---------------------------------------------------------*/
double GTZDM_ROAD::rdes_level (double cml)
	/* ml; continue mileage  */
{
	char       str[80];
	int   j;
	double     z=-1.0,sml,eml;

	//--------------------------------------------------------
	//  sml = pzLinep->DLArray[0].ELC;//起始连续里程
	//  eml = pzLinep->XLLength;
	sml = CM[0];//起始连续里程
	eml =  CM[(NCM-2)*5];
	// sml = pzLinep->DLArray[0].ELC;//起始连续里程
	// eml = pzLinep->XLLength;
	str[0]='\0';
	if (cml<sml-0.001 || cml>eml+0.01) 
	{
		//      ads_printf(L"\nML=%0.2lf out of range (%lf - %lf)",cml,sml,eml);

		//  return -999;

		if(cml<sml)
			return CM[1];
		else
			return CM[(NCM-2)*5+1];

	}
	else if ( cml<CM[0]) {
		z = CM[1]-CM[2]*(CM[0]-cml);
		z+= DLEVEL(0, cml); 
	}
	else if (cml>CM[(NCM-2)*5+0] ) {

		z= CM[(NCM-2)*5+1]+CM[(NCM-1)*5+2]*(cml - CM[(NCM-2)*5+0]);
		z+=DLEVEL(NCM-1,cml);
	}
	else{
		for(j=1; j<NCM-1; j++)
			if (CM[(j-1)*5+0]-0.0001<=cml&&cml<=CM[j*5+0]+0.0001)
			{
				double a;
				z = CM[(j-1)*5+1]+CM[j*5+2]*(cml-CM[(j-1)*5+0]);
				a =DLEVEL(j,cml);
				z= z+a;
				break;

			}
	}
	return (z);
}
/*---------------------------------------------------------*
*        calculate the design level at certain mileage    *
*---------------------------------------------------------*/
double GTZDM_ROAD::DLEVEL(int icm, double cml)
{
	double vr1, vr2, di1,di2, vt1,vt2, ds, ds1, dds,s1,s2;
	int kr;

	if (icm==0) {
		vr1=1.0e8;
		di1=0.0;
		vr2 = CM[icm*5+4];
		di2 = (CM[(icm+1)*5+2] - CM[icm*5+2])*100.0;
		ds=CM[0]-cml;
	}
	else if (icm==NCM-1) {
		vr1 = CM[(icm-1)*5+4];
		di1 = (CM[icm*5+2] - CM[(icm-1)*5+2])*100.0;
		vr2=1.0e8;
		di2=0.0;
		ds=cml-CM[icm*5+0];
	}
	else 
	{
		vr1 = CM[(icm-1)*5+4];
		vr2 = CM[icm*5+4];
		di1 = (CM[icm*5+2] - CM[(icm-1)*5+2])*100.0;
		di2 = (CM[(icm+1)*5+2] - CM[icm*5+2])*100.0;
		ds = cml-CM[(icm-1)*5+0];
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
	else if (ds>CM[icm*5+3]-vt2) {
		ds1 = ds-CM[icm*5+3]+vt2;
		kr  = 2;
	}
	else  {
		ds1 = 0.0;
		kr  = 0;
	}

	if (kr==1 && vr1>0.0001)
		dds = s1*ds1*ds1/2.0/vr1;
	else if (kr==2 && vr2>0.0001)
		dds = s2*ds1*ds1/2.0/vr2;
	else
		dds = 0.0;
	return (dds);
}

AcGePoint2d GTZDM_ROAD::Data_drawtoreal(AcGePoint2d pt)
{
	AcGePoint2d pk;
	pk.x = Trs_Coor_System(pt.x, MILE, TO_SLOPE);
	pk.y = Trs_Coor_System(pt.y, LEVEL, TO_SLOPE);
	//pk.x=(pt.x-X0)*HBVSCALE+K0;
	//pk.y=(pt.y-Y0)+H0;
	return pk;
}

AcGePoint2d GTZDM_ROAD::Data_realtodraw(AcGePoint2d pt)
{
	AcGePoint2d pk;
	pk.x = Trs_Coor_System(pt.x, MILE, TO_DRAW);
	pk.y = Trs_Coor_System(pt.y, LEVEL, TO_DRAW);
	//pk.x=(pt.x-K0)/HBVSCALE+X0;
	//pk.y=(pt.y-H0)+Y0;
	return pk;
}

void GTZDM_ROAD::ScreenToReal()
{
	double LC = Trs_Coor_System(BPD_array[0][0], MILE, TO_SLOPE);
	double HH = Trs_Coor_System(BPD_array[0][1], LEVEL, TO_SLOPE);
	//double HH=BPD_array[0][1]-Y0 +H0;    // 起点设计高程
	//double LC=(BPD_array[0][0]-X0)*HBVSCALE+K0;// 起点里程

	PDarray[0][0] = LC,PDarray[0][1] = HH,PDarray[0][2] = BPD_array[0][2];

	for (int i=1;i<NBPD;i++)
	{
		double LL = (BPD_array[i][0] - BPD_array[i - 1][0])*HSCALE;
		if (fabs(LL)<0.001) LL=1000.0;
		double II = (BPD_array[i][1] - BPD_array[i - 1][1]) * VSCALE * 1000.0 / LL;
		LC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
		HH = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
		//LC=(BPD_array[i][0]-X0)*HBVSCALE+K0;
		//HH=BPD_array[i][1]-Y0+H0;    // 设计高程	  
		if (i<NBPD-1) 
		{ 
			PDarray[i][0] = LC;
			PDarray[i][1] = HH;
			PDarray[i][2] = BPD_array[i][2];
		}
		else  
		{
			PDarray[i][0] = BPD_array[i][0];
			PDarray[i][1] = BPD_array[i][1];
			PDarray[i][2] = 0.0;
		}
		//          ads_printf(L" %-10.3f %-10.3f %-10.3f \n",LC,HH,BPD_array[i][2]);

	};
}
void GTZDM_ROAD::VIEW_ZM_TypeHdm()
{
	if(TypeHdmBar !=NULL)
	{
		AcGePoint2d ResPt,INpt;
		INpt.set(HdmPt[X],HdmPt[Y]);
		ResPt = Data_drawtoreal(INpt);
		double cml = ResPt.x; 
		ACHAR ckml[80];
		CString GH;
		double xlc=XLC(cml,GH);
		_tcscpy(ckml,L"\0");   _tcscpy(ckml,LCchr(GH,xlc,3));
		TypeHdmBar->m_ckml = ckml;
		TypeHdmBar->m_pm = pzLinep;
		TypeHdmBar->m_zm = this;
		TypeHdmBar->UpdateData(false);
		TypeHdmBar->Invalidate();
	}
}
//--------------------------------------------------------------------------
void GTZDM_ROAD::CalTWB(int NCM,double CM[],double cml,double &DMGC,double &SJGC,double &TWH,double &SJPD,double &LJKD,double &SQXZJ,double &LJSGG,double &ZXTJ)
{
	ACHAR       str[80];
	int   j;
	double     z=-1.0,sml,eml;

	SJGC = SJPD = TWH =  LJKD = ZXTJ = LJSGG = 0.0;//暂时
	sml = CM[0];//起始连续里程
	eml =  CM[(NCM-2)*5];
	str[0]='\0';
	if (cml<sml-0.001 || cml>eml+0.01) {
		//      ads_printf(L"%d %d ML=%0.2lf out of range(%0.3lf---%0.3lf)\n",NBPD,NCM,cml,sml,eml);
		ads_printf(L"无法计算%0.2lf里程的设计标高!\n",cml);
		//   ads_alert(str);
		return ;
	}
	else if ( cml<CM[0]-0.0001) {
		SJGC = CM[1]-CM[2]*(CM[0]-cml);
		//		TWH = SJGC - DMGC;
		SJPD = CM[2];
		SQXZJ = DLEVEL(0,cml);
		SJGC += SQXZJ;
	}
	else if (cml>CM[(NCM-2)*5+0]+0.0001 ) {
		SJGC= CM[(NCM-2)*5+1]+CM[(NCM-1)*5+2]*(cml - CM[(NCM-2)*5+0]);
		//    TWH = SJGC - DMGC;
		SJPD = CM[NCM-1];
		SQXZJ = DLEVEL(NCM-1,cml);        
		SJGC += SQXZJ;
	}
	else{
		for(j=1; j<NCM-1; j++)
			if (CM[(j-1)*5+0]-0.0001<=cml&&cml<=CM[j*5+0]+0.0001)
			{
				SJGC = CM[(j-1)*5+1]+CM[j*5+2]*(cml-CM[(j-1)*5+0]);//设计高程
				//			   TWH = SJGC - DMGC;
				SJPD = CM[j*5+2];
				SQXZJ = DLEVEL(j,cml);
				SJGC += SQXZJ;
				break;
			}
	} 

	int inf;
	double val;

	if(DesHXSW==1)
		val = 10.0;
	else if(DesHXSW==2)
		val = 100.0;
	else if(DesHXSW==3)
		val = 1000.0;
	else
		val = 1.0;

	TWH = SJGC - DMGC;  //填挖高3
	sign(inf,TWH);
	TWH=inf*(int(fabs(TWH)*val+0.500001))/val;

	sign(inf,SJGC);
	SJGC=inf*(int(fabs(SJGC)*val+0.500001))/val; 
	sign(inf,DMGC);
	DMGC=inf*(int(fabs(DMGC)*val+0.500001))/val;

	sign(inf,SJPD);
	SJPD=inf*(int(fabs(SJPD)*10000.0+0.500001))/10.0;

	sign(inf,SQXZJ);
	SQXZJ=inf*(int(fabs(SQXZJ)*val+0.500001))/val;

	LJKD = 0.0;
	ZXTJ = 0.0;
	LJSGG = SJGC + SQXZJ;
	_stprintf(str,L"%0.3lf",LJSGG);
	LJSGG = _wtof(str);  
}

//extern ERRMESDlg *ERRdlg;//违反规定对话框
void GTZDM_ROAD::checkzm()
{
	CString ERR;
	int NERR=0;

	//   FindZXLenMin();//找出最小夹直线长
	ScreenToReal();

	if(!ReadWorkdir())
	{
		ads_alert(L"请先指定正确的工作路径!");
		return ;
	}
	/*
	ReadGuifanFile();
	DV = mGuiFanCS.SuDuMuBiao;
	TLDJ = mGuiFanCS.TieLuDengJi;
	XZPD = mGuiFanCS.XianZhiPo1;//应分上下行分别选择
	XZPD1 = mGuiFanCS.XianZhiPo;//应分上下行分别选择   

	DFXYXC = (double)mGuiFanCS.DaoFaXian;
	if(mGuiFanCS.QianYin==1)
	QYZL = "电力";
	else if(mGuiFanCS.QianYin==2)
	QYZL = "内燃";
	else
	QYZL = "蒸汽";
	FindPDLenMin();////找出有关规范值

	_stprintf(fname,L"%s\\data\\Err",Cworkdir);
	fpw = _wfopen(fname,L"w");
	if(fpw==NULL)
	{
	ads_printf(L"打开文件%s失败!\n",fname);
	return;
	}
	fwprintf(fpw,L"%10d\n",NERR);

	char GH[8];
	double LC,BLC,xlc,Bxlc;
	for (i=1;i<NBPD;i++)
	{ 
	LL=(BPD_array[i][0]-BPD_array[i-1][0])*HBVSCALE;		
	PD[i]=BPD_array[i][3];//表示从i-1到i的坡度
	LC=(BPD_array[i][0]-X0)*HBVSCALE+K0;
	xlc = XLC(LC,GH);
	BLC=(BPD_array[i-1][0]-X0)*HBVSCALE+K0;
	Bxlc = XLC(BLC,GH);
	ads_printf(L"i=%d %lf\n",i,PD[i]);

	//考虑坡度折减
	double beta,MaxPD,NoteXZPD;
	double MAXPD1,MAXPD2;//考虑曲线折简后的上下坡设计坡度折减值
	//修正限制坡度
	int DXorRXD,itmp,rxdno;
	CString RXDname;
	if(mGuiFanCS.ZhXianShu!=1)//非双线
	{
	AcGePoint3d PT;
	PT.x=BPD_array[0][0],PT.y=BPD_array[0][1],PT.z=0.0;
	GlobalZDMTK.IsRXDOrDX(PT,DXorRXD,rxdno,RXDname);
	if(DXorRXD==1 || DXorRXD==2)//	并行或左绕,取下行限坡
	XZPD=XZPD1;
	else if(DXorRXD==3)//右绕,取上行限坡
	XZPD1=XZPD;
	}


	if(PD[i]>=0.0)
	MaxPD = XZPD;
	else
	MaxPD = XZPD1;
	MAXPD1=MAXPD2=0.0;
	//考虑坡度折减
	if(mGuiFanCS.ZheJian)
	{
	//1.先作隧道折减
	beta = pdzj_Tun(BLC,LC);
	MaxPD = beta*MaxPD;
	//2.作曲线折减
	pdzj(BLC,LC,&MAXPD1,&MAXPD2);
	}


	if(PD[i]>=0.0)//上坡
	{
	NoteXZPD = MaxPD-MAXPD1;
	if(Xsw==1) NoteXZPD = (int)(NoteXZPD*10.0+0.00001)/10.0;
	else if(Xsw==2) NoteXZPD = (int)(NoteXZPD*100.0+0.00001)/100.0;
	else if(Xsw==3) NoteXZPD = (int)(NoteXZPD*1000.0+0.00001)/1000.0;
	else NoteXZPD = (int)(NoteXZPD+0.00001);
	if(PD[i]>NoteXZPD)
	{
	ERR.Format(L"变坡点%0.2lf-变坡点%0.2lf坡度为%0.2lf‰大于限制坡度(%0.2lf‰))",Bxlc,xlc,PD[i],NoteXZPD);
	fwprintf(fpw,L"%lf %s\n",0.5*(PDarray[i-1][0] + PDarray[i][0]),ERR);
	NERR++;
	}		
	}
	else
	{
	NoteXZPD = MaxPD-fabs(MAXPD2);
	if(Xsw==1) NoteXZPD = (int)(NoteXZPD*10.0+0.00001)/10.0;
	else if(Xsw==2) NoteXZPD = (int)(NoteXZPD*100.0+0.00001)/100.0;
	else if(Xsw==3) NoteXZPD = (int)(NoteXZPD*1000.0+0.00001)/1000.0;
	else NoteXZPD = (int)(NoteXZPD+0.00001);			
	if(fabs(PD[i])>NoteXZPD)
	{
	ERR.Format(L"变坡点%0.2lf-变坡点%0.2lf坡度为%0.2lf‰大于限制坡度(%0.2lf‰))",Bxlc,xlc,PD[i],NoteXZPD);
	fwprintf(fpw,L"%lf %s\n",0.5*(PDarray[i-1][0] + PDarray[i][0]),ERR);
	NERR++;
	}			
	}

	/ *
	if(PD[i]>XZPD)
	{
	ERR.Format(L"变坡点%0.2lf-变坡点%0.2lf坡度为%0.2lf%%大于限制坡度(%0.2lf%%))",Bxlc,xlc,PD[i],XZPD);
	fwprintf(fpw,L"%lf %s\n",0.5*(PDarray[i-1][0] + PDarray[i][0]),ERR);
	NERR++;
	}
	* /
	if(LL<PDLmin)
	{
	ERR.Format(L"变坡点%0.2lf-变坡点%0.2lf坡段长为%0.2lf小于最小坡长(%0.2lf))",Bxlc,xlc,LL,PDLmin);
	fwprintf(fpw,L"%lf %s\n",0.5*(PDarray[i-1][0] + PDarray[i][0]),ERR);
	NERR++;		
	}
	if(i>1)
	{
	double di = fabs(PD[i]-PD[i-1]);//表示第i-1个变坡点的坡度代数差
	if(di>PDDSC1)
	{
	ERR.Format(L"变坡点%0.2lf处的坡度代数差%0.2lf大于限制值(一般:%0.2lf)",Bxlc,fabs(PD[i]-PD[i-1]),PDDSC1);
	fwprintf(fpw,L"%lf %s\n",0.5*(PDarray[i-1][0] + PDarray[i][0]),ERR);
	NERR++;		
	}

	double T = BPD_array[i-1][2]*di/2000.0;
	//		ads_printf(L"i=%d di=%lf R=%lf T=%lf\n",i,di,BPD_array[i][2],T);
	if(T<0.1)//无竖曲线
	continue;

	double sml = BLC-T;    //变坡点起始里程
	double eml = BLC+T;    

	for (int j=1;j<pzLinep->NJD-1;j++)
	{
	if((pzLinep->QXB[j][0]>sml && pzLinep->QXB[j][0]<eml)||
	(pzLinep->QXB[j][1]>sml && pzLinep->QXB[j][1]<eml)||
	(pzLinep->QXB[j][2]>sml && pzLinep->QXB[j][2]<eml)||
	(pzLinep->QXB[j][3]>sml && pzLinep->QXB[j][3]<eml)||
	(sml>pzLinep->QXB[j][0] && sml<pzLinep->QXB[j][1])||
	(eml>pzLinep->QXB[j][2] && eml<pzLinep->QXB[j][3])
	)//zh,hy,yh,hz里程有落在竖曲线范围或者竖曲线起始终止里程有落在zh-hy,yh-hz范围
	{
	ERR.Format(L"变坡点%0.2lf处的竖曲线与缓和曲线重叠",Bxlc);
	fwprintf(fpw,L"%lf %s\n",0.5*(PDarray[i-1][0] + PDarray[i][0]),ERR);
	NERR++;		
	}
	}   
	//

	double Rshu = Get_Rshu(DV,di);//按规范取值Rshu
	if(fabs(BPD_array[i-1][2]-Rshu)>0.1)//尚未付值
	{
	ERR.Format(L"变坡点%0.2lf处的竖曲线半径%0.2lf与规范(%0.2lf)不符",Bxlc,BPD_array[i-1][2],Rshu);
	fwprintf(fpw,L"%lf %s\n",0.5*(PDarray[i-1][0] + PDarray[i][0]),ERR);
	NERR++;		
	BPD_array[i-1][2] = Rshu;//按规范更正					
	ads_printf(L"变坡点%0.2lf处的竖曲线半径%0.2lf与规范(%0.2lf)不符, 已更正!\n",BPD_array[i-1][2]);
	}		     

	}		
	} 
	//检查坡度代数差
	rewind(fpw);
	fwprintf(fpw,L"%10d\n",NERR);
	fclose(fpw);
	if(NERR<1)
	{
	ads_printf(L"未发现错误，纵面线形通过坡段长，最大设计坡度及坡度代数差的检测!\n");
	return;
	}*/

	/*
	if(ERRdlg) ERRdlg->DestroyWindow();
	ERRdlg = new ERRMESDlg(NULL);
	ERRdlg->Create(IDD_DIALOG_ERR);
	ERRdlg->ShowWindow(SW_SHOW);*///szf

}

void GTZDM_ROAD::FindPDLenMin()//找出最小坡段长
{
	PDLmin=PDDSC1=PDDSC2=1000000.0;//不受限制
	/*
	if(DV<75)//地方铁路
	{
	if(DFXYXC>1049) PDLmin=500;
	else if(DFXYXC>849) PDLmin=400;
	else if(DFXYXC>749) PDLmin=350;
	else if(DFXYXC>649) PDLmin=300;
	else if(DFXYXC>549) PDLmin=250;
	else  PDLmin=200;

	if(TLDJ=="地方I级")
	{
	if(DFXYXC>1049) {PDDSC1=5,PDDSC2=6;}
	else if(DFXYXC>849) {PDDSC1=6,PDDSC2=8;}
	else if(DFXYXC>749) {PDDSC1=8,PDDSC2=10;}
	else if(DFXYXC>649) {PDDSC1=10,PDDSC2=12;}
	else if(DFXYXC>549) {PDDSC1=12,PDDSC2=14;}
	else  {PDDSC1=14,PDDSC2=16;}
	}
	else if(TLDJ=="地方II级")
	{
	if(DFXYXC>1049) {PDDSC1=6,PDDSC2=8;}
	else if(DFXYXC>849) {PDDSC1=8,PDDSC2=10;}
	else if(DFXYXC>749) {PDDSC1=10,PDDSC2=12;}
	else if(DFXYXC>649) {PDDSC1=12,PDDSC2=14;}
	else if(DFXYXC>549) {PDDSC1=14,PDDSC2=16;}
	else  {PDDSC1=16,PDDSC2=18;}
	}
	else
	{
	if(DFXYXC>749) {PDDSC1=18,PDDSC2=20;}
	else if(DFXYXC>649) {PDDSC1=20,PDDSC2=25;}
	else if(DFXYXC>549) {PDDSC1=25,PDDSC2=30;}
	else  {PDDSC1=25,PDDSC2=30;}

	}
	}
	else if(DV<145)
	{
	if(DFXYXC>1049) 	PDLmin=400;
	else if(DFXYXC>849) PDLmin=350;
	else if(DFXYXC>749) PDLmin=300;
	else if(DFXYXC>649) PDLmin=250;
	else if(DFXYXC>549) PDLmin=200;
	else  PDLmin=200;

	if(TLDJ=="I级" || TLDJ=="II级")
	{
	if(DFXYXC>1049) {PDDSC1=8,PDDSC2=10;}
	else if(DFXYXC>849) {PDDSC1=10,PDDSC2=12;}
	else if(DFXYXC>749) {PDDSC1=12,PDDSC2=15;}
	else {PDDSC1=15,PDDSC2=18;}
	}
	else 
	{
	if(DFXYXC>1049) {PDDSC1=10,PDDSC2=12;}
	else if(DFXYXC>849) {PDDSC1=12,PDDSC2=15;}
	else if(DFXYXC>749) {PDDSC1=15,PDDSC2=18;}
	else if(DFXYXC>649) {PDDSC1=18,PDDSC2=20;}
	else  {PDDSC1=20,PDDSC2=25;}
	}		
	}
	else if(DV<161)
	{ PDLmin = 400;}
	else if(DV<201)
	{ PDLmin = 400;}
	else if(DV<251)
	{ PDLmin = 400;}
	else if(DV>299)
	{ PDLmin = 900;}*/
	/*
	PDLmin = mGuiFanCS.MinPoChang[0];
	PDDSC1 = mGuiFanCS.MaxPoCha[0];
	PDDSC2 = mGuiFanCS.MaxPoCha[0];*/

}
//----------------------------------------------------------------------------------
void GTZDM_ROAD::PDdataToCM(int &NCM,double CM[])
{	   
	double LC1,LC2,H2,H1,R;

	LC1 = Trs_Coor_System(BPD_array[0][0], MILE, TO_SLOPE);
	H1 = Trs_Coor_System(BPD_array[0][1], LEVEL, TO_SLOPE);
	//LC1=(BPD_array[0][0]-X0)*HBVSCALE+K0;
	//H1=BPD_array[0][1]-Y0+H0;    
	CM[0] =LC1,CM[1]=H1,CM[2]=0.0, CM[3] = 0.0, CM[4]=0.0;
	for(int i=1; i<NBPD; i++)
	{
		LC2 = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
		H2 = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
		//LC2=(BPD_array[i][0]-X0)*HBVSCALE+K0;
		//H2=BPD_array[i][1]-Y0+H0;    
		R =BPD_array[i][2];

		CM[i*5]=LC2,CM[i*5+1]=H2;
		if(LC2>LC1+0.0001)
			CM[i*5+2]=(H2-H1)/(LC2-LC1);
		else
			CM[i*5+2]=0.0;

		CM[i*5+3]=LC2-LC1;
		CM[i*5+4]=R;
		H1=H2,LC1=LC2;
		//		ads_printf(L"NCM=%d i=%d ml=%lf\n",NBPD+1,i,CM[i*5]);
	}
	CM[NBPD*5] =CM[NBPD*5+1]=CM[NBPD*5+2]=CM[NBPD*5+3]=CM[NBPD*5+4]=0.0;
	NCM = NBPD+1;	
}

void GTZDM_ROAD::AdjustPD(int ipd,double pd,int mode)//调整坡度,mode=0顺延，1-不顺延
{
	double BGC,OriGC,HGC,GC,BLC,LC,HLC;

	if(ipd<0 || ipd>NBPD-1)//2005.3.9
		return;
	if(ipd==NBPD-1)
		ipd--;

	assertWriteEnabled();
	  
	//BLC=(BPD_array[ipd][0]-X0)*HBVSCALE+K0;
	//BGC = BPD_array[ipd][1] - Y0 + H0;
	BLC = Trs_Coor_System(BPD_array[ipd][0], MILE, TO_SLOPE);
	BGC = Trs_Coor_System(BPD_array[ipd][1], LEVEL, TO_SLOPE);

	//LC=(BPD_array[ipd+1][0]-X0)*HBVSCALE+K0;
	LC = Trs_Coor_System(BPD_array[ipd + 1][0], MILE, TO_SLOPE);
	GC = BGC + (LC - BLC) * pd / 100.0;
	
	//OriGC = BPD_array[ipd+1][1]-Y0+H0;  
	OriGC = Trs_Coor_System(BPD_array[ipd + 1][1], LEVEL, TO_SLOPE);
	  
	//BPD_array[ipd+1][1] = (GC-H0)+Y0;
	BPD_array[ipd + 1][1] = Trs_Coor_System(GC, LEVEL, TO_DRAW);
	if(mode==0)//高程顺延
	{
		for(int i=ipd+2; i<NBPD; i++)
		{
			//HLC = (BPD_array[i][0]-X0)*HBVSCALE+K0;
			//HGC = BPD_array[i][1] - Y0 + H0;
			HLC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
			HGC = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
			pd = (HGC-OriGC)/(HLC-LC)*100.0;//保持坡度不变
			GC = GC + (HLC-LC)*pd/100.0;
			OriGC = HGC;
			LC = HLC;
			BPD_array[i][1] = Trs_Coor_System(GC, LEVEL, TO_DRAW);
			//BPD_array[i][1] = (GC-H0)+Y0;				
		}
	}
	else if(mode ==2)///高程顺延,边坡点里程改变,前后坡度不变
	{
		double x1,y1,x2,y2,x3,y3,x4,y4,x,y,JDLC;
		x1=BLC,y1=BGC,x2=LC,y2=GC;
		x3 = LC, y3 = OriGC;
		//x4=(BPD_array[ipd+2][0]-X0)*HBVSCALE+K0;
		x4 = Trs_Coor_System(BPD_array[ipd + 2][0], MILE, TO_SLOPE);
		//y4 = BPD_array[ipd+2][1]-Y0+H0;
		y4 = Trs_Coor_System(BPD_array[ipd + 2][1], LEVEL, TO_SLOPE);
		INTERS(&x,&y,x1,y1,x2,y2,x3,y3,x4,y4);
		//		ads_printf(L"%lf %lf\n",x,y);
		if(x>=x1&&x<=x4)//x为交点里程
		{
			if(QZZ > 0.1)
				JDLC = int(x / QZZ + 0.5)*QZZ;//取整
			else
				JDLC = x;
			//BPD_array[ipd+1][0] = (JDLC-K0)/HBVSCALE + X0;
			BPD_array[ipd + 1][0] = Trs_Coor_System(JDLC, MILE, TO_DRAW);
			GC = BGC + (JDLC-BLC)*pd/100.0;			
			//BPD_array[ipd+1][1] = (GC-H0)+Y0;
			BPD_array[ipd + 1][1] = Trs_Coor_System(GC, LEVEL, TO_DRAW);

			for (int i = ipd + 2; i < NBPD; i++)
			{ 
				//HLC = (BPD_array[i][0]-X0)*HBVSCALE+K0;
				//HGC = BPD_array[i][1] - Y0 + H0;
				HLC = Trs_Coor_System(BPD_array[i][0], MILE, TO_SLOPE);
				HGC = Trs_Coor_System(BPD_array[i][1], LEVEL, TO_SLOPE);
				pd = (HGC - OriGC) / (HLC - LC)*100.0;//保持坡度不变
				if(i==ipd+2)
					GC = GC + (HLC-JDLC)*pd/100.0;
				else
					GC = GC + (HLC-LC)*pd/100.0;
				OriGC = HGC;
				LC = HLC;
				//BPD_array[i][1] = (GC-H0)+Y0;	
				BPD_array[i][1] = Trs_Coor_System(GC, LEVEL, TO_DRAW);
			}
		}
	}
	else//高程不顺延
	{

	}
	//	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"regen\n",false,true);
}

//计算坡度折减
//D1 坡度起点里程  D2坡度终点里程  *U1 下行坡度折减值 *U2上行坡度折减值
//若下行限坡BIP 上行限坡CIP(为负数）则当前坡度i可行域  CIP+ *U2  < i < BIP - *U1
void GTZDM_ROAD::pdzj( double D1,double D2 ,double *U1,double *U2)
{
	/*
	double DD1 , DD2 , RR , DDI , LCC ,SUM ,U , U3 , U4 , PDC ;
	int IRS ,BO1 ,BO2 , K1 ,K2 , KK1 , KK2 , i , j ;
	double ZH[5*MAXBPDNUM],HZ[5*MAXBPDNUM],LS1[5*MAXBPDNUM],LS2[5*MAXBPDNUM],RADIO[5*MAXBPDNUM];
	int NQXS;
	double BIP,CIP;//上，下行限坡

	if (!mGuiFanCS.ZheJian)
	{
	*U1=*U2=0.0;
	return;
	}
	LCC=DFXYXC - 50.0 ;
	BIP = XZPD;
	CIP = -1*XZPD;

	// 列车长=到发现有效长-50.0;
	BO1= 1 ;
	BO2= 1 ;
	K2=0 ;
	PDC = D2 - D1;
	SUM=0.0 ;
	U=0.0 ;
	U3=0.0 ;
	U4=0.0 ;


	NQXS = pzLinep->NJD;
	for(i=0; i<pzLinep->NJD;i++)
	{
	ZH[i] = pzLinep->QXB[i][0];
	HZ[i] = pzLinep->QXB[i][3];
	LS1[i] = pzLinep->QXB[i][1] - pzLinep->QXB[i][0];
	LS2[i] = pzLinep->QXB[i][3] - pzLinep->QXB[i][2];
	RADIO[i] = pzLinep->JDarray[i][4];
	//  ads_printf(L"i=%d %lf %lf ls1-%lf %lf R-%lf\n",i,ZH[i],HZ[i],LS1[i],LS2[i],RADIO[i]);
	}
	i = 1 ;
	//判断坡段起点在那个曲线上k1 曲线号 
	while ( D1 > ZH[i] + 0.5*LS1[i] )
	{ i = i + 1 ;
	if ( i == NQXS - 1 ) break;
	} ;

	if ( i == NQXS - 1 ) { K1 = NQXS -2 ; K2 = NQXS -2 ;}
	else K1 = i - 1 ;
	//  ads_printf(L"1:i=%d K1=%d\n",i,K1);
	//相对D1 如果在曲线范围内 BO1 = 0 else BO1 = 1  
	//相对D2 如果在曲线范围内 BO2 = 0 else BO2 = 1  
	if ( D1 < HZ[K1]-0.5*LS2[K1] ) { BO1 = 0; K1=K1-1;};
	for (i = K1+1; i < NQXS -1; i++ )
	{
	if ( D2 <= ZH[i]+0.5*LS1[i] )
	//坡度终点在本曲线前面
	{
	BO2 = 1 ;
	K2 = i - 1 ;
	goto l40;
	}
	else if (D2 > HZ[i]-0.5*LS2[i] )
	//坡度终点在本曲线后面 SUM 坡度范围内累计偏角
	{
	if ( D1 > ZH[i] + 0.5*LS1[i] ) SUM = SUM+(HZ[i]-0.5*LS2[i]-D1)/fabs(RADIO[i]) ;
	else                           SUM = SUM+(HZ[i]- ZH[i]-0.5*(LS1[i]+LS2[i]) )/fabs(RADIO[i]) ;
	}
	else
	//坡度终点在本曲线里面
	{
	if ( D1 > ZH[i] + 0.5*LS1[i] ) SUM = SUM + PDC/fabs(RADIO[i]);
	else                           SUM = SUM + ( D2 - ZH[i] - 0.5*LS1[i] )/fabs(RADIO[i]);
	BO2 =  0 ;
	K2 = i ;
	} ;
	if ( i == NQXS - 2 )
	{
	K2 = NQXS - 2 ;
	if ( HZ[K2]+0.5*LS2[K2] <= D2 )
	{
	BO2 = 1 ;
	goto l40;
	} ;
	};
	if ( HZ[i]-0.5*LS2[i] >= D2 ) goto l40 ;
	} ; / * exit for * /
	l40:  IRS = K2 - K1 ;
	//	  ads_printf(L"2:K2=%d K1=%d Bo1=%d Bo2=%d\n",K2,K1,BO1,BO2);
	//      本坡段所跨越的曲线数
	/ *THE FALLOWING IS IRS=0 (NO RADIS) * /
	/ *  IF IRS <> 0   DDI  当前曲线范围内的坡度长度* /
	if      ( BO1 == 1 && BO2 == 1 )  DDI = HZ[K2]-0.5*LS2[K2]-ZH[K1+1]-0.5*LS1[K1+1] ;
	else if ( BO1 == 1 && BO2 == 0 )  DDI = D2 - ZH[K1+1] - 0.5*LS1[K1+1] ;
	else if ( BO1 == 0 && BO2 == 1 )  DDI = HZ[K2] - 0.5*LS2[K2] - D1 ;
	else  DDI = D2 - D1 ;
	//ads_printf(L"k1=%d k2=%d bip=%lf cip=%lf ZH=%lf bo1=%d IRS=%d DDI=%lf LCC=%lf DFXYXC=%lf\n",K1,K2,BIP,CIP,ZH[K1],BO1,IRS,DDI,LCC,DFXYXC);

	/ *-----THE FALLOWING IS IRS=1 　坡度内只有一个曲线 * /
	// LCC 列车长＝到发线有效长－50
	if ( IRS == 1 )
	{
	//	  ads_printf(L"pdc=%lf SUM=%lf\n",PDC,SUM);
	if ( ( BO1 == 0 && BO2 == 0 ) || DDI > LCC )  U =600.0/fabs(RADIO[K2]) ;//坡段起终点都在曲线上，坡段长大于列车长
	else
	{
	PDC=CALMIN(PDC,LCC) ;
	U= 10.5*SUM*180/pi/PDC ;
	} ;
	RR = fabs(RADIO[K2]);
	// U3 U4 小半径折减值
	LRADCON(RR,BIP,CIP,&U3,&U4);
	*U1 = U + U3 ;
	*U2 = U + U4 ;
	//		  ads_printf(L"U=%lf U3=%lf %lf\n",U,U3,*U1);
	}
	else if ( IRS > 1 )
	{
	//小于列车长，取坡度长列车长最小值
	//	  ads_printf(L"pdc=%lf lcc=%lf\n",PDC,LCC);
	if ( DDI <= LCC )
	{
	RR = 4000.0 ;
	PDC=CALMIN(PDC,LCC);
	U=10.50*SUM*180/pi/PDC;
	for ( j=K1+1;j<=K2;j++) RR = CALMIN(fabs(RADIO[j]),RR );
	LRADCON(RR,BIP,CIP,&U3,&U4);
	*U1 = U + U3 ;
	*U2 = U + U4 ;
	}
	else
	/ *----THE FALLOWING IS (IRS>1 AND  DDI>SSL) * /
	//大于列车长，取进行最不利组合
	{	  KK1 = K1+1;
	if (BO1 == 1 )
	{
	DD1 = ZH[KK1]+0.5*LS1[KK1];
	}
	else
	{
	DD1 = D1;
	};

	*U1=0.0 ;
	*U2=0.0 ;
	DD2 = DD1 + LCC ;
	//  ads_printf(L"1\n");
	do
	{
	U= 0.0 ;
	if ( DD2 >= D2 ) break;
	RR=4000.0 ;			  //ads_printf(L"2\n");
	for ( j = KK1;j <= K2;j++ )
	{
	if ( HZ[j]-0.5*LS2[j] <=  DD2 )
	{
	if ( j == KK1 ) U=10.50*(HZ[j]-0.5*LS2[j] - DD1)/fabs(RADIO[j])*180/pi/LCC + U;
	else            U=10.50*(HZ[j]-ZH[j]-0.5*(LS1[j]+LS2[j]) )/fabs(RADIO[j])*180/pi/LCC + U;
	RR =CALMIN(fabs(RADIO[j]),RR);
	}
	else if (ZH[j]+0.5*LS1[j] <= DD2 )
	{
	if (j == KK1 && BO1 == 0 ) U=600.0/fabs(RADIO[KK1]);
	else U=10.50*(DD2-ZH[j]-0.5*LS1[j])/fabs(RADIO[j])*180/pi/LCC+U;
	RR=CALMIN(fabs(RADIO[j]),RR);
	}
	else goto l210;
	} ;
	l210:         LRADCON(RR , BIP, CIP,&U3,&U4);
	U3 = U + U3;
	U4 = U + U4;
	*U1 = CALMAX(*U1,U3);
	*U2 = CALMAX(*U2,U4);
	KK1= KK1 + 1 ;
	DD1 =ZH[KK1]+0.5*LS1[KK1];
	DD2 = DD1 + LCC ;
	} while ( DD2 < D2 );

	KK2 = K2;
	if ( BO2 == 1 ) DD2 = HZ[K2] - 0.5*LS2[K2];
	else DD2 = D2 ;
	DD1 = DD2 - LCC ;
	do
	{
	U=0.0 ;
	if ( DD1 < D1 ) break ;
	RR=4000.0 ;
	for (j = KK2 ; j >= K1 ; j = j-1 )
	{
	if (ZH[j]+0.5*LS1[j] > DD1 )
	{
	if ( j == K2 ) U=10.50*(DD2-ZH[j]-0.5*LS1[j])/fabs(RADIO[j])*180/pi/LCC+U;
	else           U=10.50*(HZ[j]-ZH[j]-0.5*(LS1[j]+LS2[j]) )  /fabs(RADIO[j])*180/pi/LCC+U;
	RR=CALMIN(fabs(RADIO[j]),RR);
	}
	else if (HZ[j]-0.5*LS2[j] >= DD1)
	{
	if ( BO2 == 0 && j == K2 ) U = 600.0/fabs(RADIO[j]);
	else   U=10.50*(HZ[j]-0.5*LS2[j]-DD1)/fabs(RADIO[j])*180/pi/LCC+U;
	RR=CALMIN(fabs(RADIO[j]),RR);
	}
	else goto l310;
	};
	l310:              LRADCON(RR,BIP,CIP,&U3,&U4);
	U3 = U+U3;
	U4 = U+U4;
	*U1=CALMAX(*U1,U3);
	*U2=CALMAX(*U2,U4);
	KK2 = KK2 - 1;
	DD2=HZ[KK2]-0.5*LS2[KK2];
	DD1 = DD2 - LCC ;
	} while ( DD1 > D1 );
	};/ * exit if ddi > ssl * /
	} / * EXIT IF IRS > 1 * /
	else if ( IRS == 0 )
	// 没有曲线折减值为 ０　
	{
	*U1 = 0.0 ;
	*U2 = 0.0 ;
	};*/

	//	  ads_printf(L"####%lf %lf\n",*U1,*U2);
}

//电力牵引的小曲线半径折减 BIP CIP 下、上行限制坡度 CIP为负值
void GTZDM_ROAD::LRADCON(double R,double BIP,double CIP,double *U3,double *U4)
{
	*U3 = 0.0 ;
	*U4=0.0;

	if(QYZL != "电力")
		return;//只有电力牵引才折减

	if ( int((R+0.1)/50.0)*50.0 == 350 )
	{
		if      ( BIP == 4.0 ) *U3 =0.50;
		else if ( BIP == 6.0 ) *U3 =0.70;
		else if ( BIP == 9.0 ) *U3 =1.00;
		else if ( BIP == 12.0 ) *U3 =1.25;
		else if ( BIP == 15.0 ) *U3 =1.50;
		else if ( BIP == 20.0 ) *U3 =2.00;
		else if ( BIP == 25.0 ) *U3 =2.45;
		else if ( BIP == 30.0 ) *U3 =2.90;

		if      (-CIP == 4.0 ) *U4 =0.50;
		else if (-CIP == 6.0 ) *U4 =0.70;
		else if (-CIP == 9.0 ) *U4 =1.00;
		else if (-CIP == 12.0 ) *U4 =1.25;
		else if (-CIP == 15.0 ) *U4 =1.50;
		else if (-CIP == 20.0 ) *U4 =2.00;
		else if (-CIP == 25.0 ) *U4 =2.45;
		else if (-CIP == 30.0 ) *U4 =2.90;
	}
	else if ( int((R+0.1)/50.0)*50.0 == 400 )
	{
		if      ( BIP == 4.0 ) *U3 =0.35;
		else if ( BIP == 6.0 ) *U3 =0.50;
		else if ( BIP == 9.0 ) *U3 =0.65;
		else if ( BIP == 12.0 ) *U3 =0.85;
		else if ( BIP == 15.0 ) *U3 =1.05;
		else if ( BIP == 20.0 ) *U3 =1.35;
		else if ( BIP == 25.0 ) *U3 =1.65;
		else if ( BIP == 30.0 ) *U3 =1.95;

		if      (-CIP == 4.0 ) *U4 =0.35;
		else if (-CIP == 6.0 ) *U4 =0.50;
		else if (-CIP == 9.0 ) *U4 =0.65;
		else if (-CIP == 12.0 ) *U4 =0.85;
		else if (-CIP == 15.0 ) *U4 =1.05;
		else if (-CIP == 20.0 ) *U4 =1.35;
		else if (-CIP == 25.0 ) *U4 =1.65;
		else if (-CIP == 30.0 ) *U4 =1.95;	  
	}
	else if ( int((R+0.1)/50.0)*50.0 == 450 )
	{
		if      ( BIP == 4.0 ) *U3 =0.20;
		else if ( BIP == 6.0 ) *U3 =0.25;
		else if ( BIP == 9.0 ) *U3 =0.35;
		else if ( BIP == 12.0 ) *U3 =0.45;
		else if ( BIP == 15.0 ) *U3 =0.55;
		else if ( BIP == 20.0 ) *U3 =0.70;
		else if ( BIP == 25.0 ) *U3 =0.90;
		else if ( BIP == 30.0 ) *U3 =1.05;

		if      (-CIP == 4.0 ) *U4 =0.20;
		else if (-CIP == 6.0 ) *U4 =0.25;
		else if (-CIP == 9.0 ) *U4 =0.35;
		else if (-CIP == 12.0 ) *U4 =0.45;
		else if (-CIP == 15.0 ) *U4 =0.55;
		else if (-CIP == 20.0 ) *U4 =0.70;
		else if (-CIP == 25.0 ) *U4 =0.90;
		else if (-CIP == 30.0 ) *U4 =1.05;
	};
}
double GTZDM_ROAD::CALMAX( double A,double B )
{
	if ( A < B ) A = B;
	return A;
}

double GTZDM_ROAD::CALMIN( double A,double B )
{
	if ( A > B ) A = B;
	return A;
}
//输入坡段的起始和终止里程,从隧道数组中驱除折减系数
//
double GTZDM_ROAD::pdzj_Tun(double BLC,double ELC)
{
	double beta,zjxs,tunLen;
	beta = 1.0;
	if(GlobalTun)
	{
		for(int i=0; i<GlobalTun->NTun; i++)
		{
			if((GlobalTun->Tun[i].sml>=BLC && GlobalTun->Tun[i].sml<=ELC)||
				(GlobalTun->Tun[i].eml>=BLC && GlobalTun->Tun[i].eml<=ELC))
			{
				tunLen = GlobalTun->Tun[i].eml - GlobalTun->Tun[i].sml;
				//	if(tunLen<400.1) continue;//只对超过400m的隧道折减
				zjxs = GetTunZJXS(tunLen,QYZL);//依据线路规范取隧道折减系数
				beta = beta < zjxs ? beta : zjxs;//当一个坡段含有多个隧道时取最小的折减系数
			}
		}
	}
	return beta;
}
//从线规中取折减系数
double GTZDM_ROAD::GetTunZJXS(double tunlen,CString qyzl)
{
	double zjxs=1.0;
	/*
	if(qyzl=="电力")
	{
	if(tunlen<1000.1 && tunlen>400.9)
	zjxs = 0.95;
	else if(tunlen<4000.1 && tunlen>1000.9)
	zjxs = 0.90;
	else if(tunlen>4000)
	zjxs = 0.85;		
	}
	else if(qyzl=="内燃")
	{
	if(tunlen<1000.1 && tunlen>400.9)
	zjxs = 0.90;
	else if(tunlen<4000.1 && tunlen>1000.9)
	zjxs = 0.80;
	else if(tunlen>4000)
	zjxs = 0.75;		
	}*/
	/*
	int i=0;
	if(mGuiFanCS.ZheJian==true)//要折减
	{
	while (i<4 && mGuiFanCS.SuiDaoFJ[i] > 1.0)
	{
	if(tunlen < mGuiFanCS.SuiDaoFJ[i])
	break;
	i++;
	}
	if(qyzl=="电力")
	zjxs = mGuiFanCS.SDXiShuD[i];
	else if(qyzl=="内燃")
	zjxs = mGuiFanCS.SDXiShuN[i];
	}*/
	return zjxs;
}
/*---------------------------------------------------------*
*        calculate the design level at certain mileage    *
*---------------------------------------------------------*/
double GTZDM_ROAD::CalT(int icm)
{
	double  vr2, di2, vt2;

	if (icm==0) {
		vr2 = CM[icm*5+4];
		di2 = (CM[(icm+1)*5+2] - CM[icm*5+2])*100.0;
	}
	else if (icm==NCM-1) {
		vr2=1.0e8;
		di2=0.0;
	}
	else {
		vr2 = CM[icm*5+4];
		di2 = (CM[(icm+1)*5+2] - CM[icm*5+2])*100.0;
	}

	if(vr2>=0.0)
		vt2 = fabs(vr2*di2/200.0);
	else
		vt2 = fabs(vr2);

	return vt2;
}
double GTZDM_ROAD::rdes_levelWithoutVerCurve (double cml)
	/* ml; continue mileage  */
{
	char       str[80];
	int   j;
	double     z=-1.0,sml,eml;
	//--------------------------------------------------------
	if(NCM<2)
		return -10000.0;

	sml = CM[0];//起始连续里程
	//eml = pzLinep->XLLength;
	eml = CM[(NCM-2)*5+0];
	str[0]='\0';
	if (cml<sml-0.001 || cml>eml+0.01) {
		//  ads_printf(L"\nML=%0.2lf out of range",cml);

		return -10000.0;
	}
	else if ( cml<CM[0]) {
		z = CM[1]-CM[2]*(CM[0]-cml);
		//  z+= DLEVEL(0, cml); 
	}
	else if (cml>CM[(NCM-2)*5+0] ) {

		z= CM[(NCM-2)*5+1]+CM[(NCM-1)*5+2]*(cml - CM[(NCM-2)*5+0]);
		//     z+=DLEVEL(NCM-1,cml);
	}
	else{
		for(j=1; j<NCM-1; j++)
			if (CM[(j-1)*5+0]-0.0001<=cml&&cml<=CM[j*5+0]+0.0001)
			{
				z = CM[(j-1)*5+1]+CM[j*5+2]*(cml-CM[(j-1)*5+0]);
				//	   a =DLEVEL(j,cml);
				//        z= z+a;
				break;

			}
	}
	return (z);
}

void GTZDM_ROAD::checkzm(CString mdbname,CString roadname,CString XZ,CStringArray &ErrMesArr)
{
	CString ErrMes;
	int NERR=0;
	//检查坡度是否超限
	for(int i=1; i<NCM-1; i++)
	{
		CString lc;
		double dml;
		CString GH;
		ACHAR ckml[80],ckml1[80];
		dml = XLC(CM[i*5],GH);
		dmltockml(GH,dml,ckml);

		dml = XLC(CM[(i-1)*5],GH);
		dmltockml(GH,dml,ckml1);

		if(CM[i*5+2]*100 > MAXPD)//最大坡度
		{
			ErrMes.Format(L"错误：%s-%s-%s纵坡 里程 %s-%s 处 坡度%0.3lf 大于最大坡度%0.3lf",mdbname,roadname,XZ,ckml1,ckml,CM[i*5+2]*100,MAXPD);
			ErrMesArr.Add(ErrMes);
		}

		if(CM[i*5+3] < JXPCMin || CM[i*5+3]<YBPCMin)//坡长
		{
			ErrMes.Format(L"错误：%s-%s-%s纵坡 里程 %s-%s 处 坡长%0.3lf 小于最小坡长(一般%0.3lf,极限%0.3lf)",mdbname,roadname,XZ,ckml1,ckml,CM[i*5+3],YBPCMin,JXPCMin);
			ErrMesArr.Add(ErrMes);
		}

		//判别凹、凸竖曲线
		if(CM[i*5+1] > CM[(i-1)*5+1] && CM[i*5+1] > CM[(i+1)*5+1])//凸形竖曲线半径
		{
			if(CM[i*5+4]>0.1 && (CM[i*5+4]<T_JXRShu || CM[i*5+4]<T_YBRshuMin))
			{
				ErrMes.Format(L"错误：%s-%s-%s纵坡 里程 %s 处 竖曲线半径%0.3lf 小于凸形最小竖曲线半径(一般%0.3lf,极限%0.3lf)",mdbname,roadname,XZ,ckml,CM[i*5+4],T_YBRshuMin,T_JXRShu);
				ErrMesArr.Add(ErrMes);
			}			
		}

		if(CM[i*5+1] < CM[(i-1)*5+1] && CM[i*5+1] < CM[(i+1)*5+1])//凹形竖曲线半径
		{
			if(CM[i*5+4]>0.1 && (CM[i*5+4]<A_JXRshuMin || CM[i*5+4]<A_YBRshu))
			{
				ErrMes.Format(L"错误：%s-%s-%s纵坡 里程 %s 处 竖曲线半径%0.3lf 小于凹形最小竖曲线半径(一般%0.3lf,极限%0.3lf)",mdbname,roadname,XZ,ckml,CM[i*5+4],A_YBRshu,A_JXRshuMin);
				ErrMesArr.Add(ErrMes);
			}			
		}
		//
		double i1,i2,di,RshuLen,T;
		i1=CM[i*5+2];
		i2=CM[(i+1)*5+2];
		di=fabs(i1-i2)*100.0;
		T = CM[i*5+4]*di/200.0;
		RshuLen = 2*T;
		if(RshuLen>0.1 && RshuLen < YBLShuMin)
		{
			ErrMes.Format(L"错误：%s-%s-%s纵坡 里程 %s 处 竖曲线长度%0.3lf 小于最小竖曲线长度(一般%0.3lf,极限%0.3lf)",mdbname,roadname,XZ,ckml,RshuLen,YBLShuMin,JXLRShuMin);
			ErrMesArr.Add(ErrMes);
		}

	}
}

/*===================================================================
函数名:Trs_Coor_System
------------------------------------------------------------
函数功能:CAD绘图坐标系坐标与里程高程坐标系转换函数
------------------------------------------------------------
输入:	double	Ori_Para		原始参数（CAD坐标X、Y或者变坡点里程、高程）
		int		Mile_Level_Flag	计算里程还是高程
								MILE		计算里程
								LEVEL		计算高程
		int		Trs_Flag		从变坡点参数转换成CAD坐标还是相反
								TO_DRAW		转换为CAD绘制坐标
								TO_SLOPE	转换为坡度参数
------------------------------------------------------------
输出:	double	返回值	计算出的CAD坐标X、Y或者变坡点里程、高程
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190709	
------------------------------------------------------------
备注：
=====================================================================*/
double GTZDM_ROAD::Trs_Coor_System(double Ori_Para, int Mile_Level_Flag, int Trs_Flag) const
{
	double Answer = 0.0;
	if (Mile_Level_Flag == MILE)
	{
		if (Trs_Flag == TO_DRAW)
			Answer = (Ori_Para - K0) / HSCALE + X0;
		else if (Trs_Flag == TO_SLOPE)
			Answer = (Ori_Para - X0) * HSCALE + K0;
		else
			ads_printf(L"传入参数错误，纵面里程参数转换失败！");
	}
	else if (Mile_Level_Flag == LEVEL)
	{
		if (Trs_Flag == TO_DRAW)
			Answer = (Ori_Para - H0) / VSCALE + Y0;
		else if (Trs_Flag == TO_SLOPE)
			Answer = (Ori_Para - Y0) * VSCALE + H0;
		else
			ads_printf(L"传入参数错误，纵面高程参数转换失败！");
	}
	else
		ads_printf(L"传入参数错误，纵面参数转换失败！");

	return Answer;
}

//限制接坡起终坡度
void GTZDM_ROAD::LimitJPSlope_StEd()
{
	if (m_JPInforArray_slope.size() < 1)
		return;

	double PD_array[MAXBPDNUM][4];
	for (int i = 0; i < MAXBPDNUM; i++)
		for (int j = 0; j < 4; j++)
			PDarray[i][j] = 0.0;

	PDarray[0][0] = Trs_Coor_System(m_JPInforArray_slope[0].Stcml, MILE, TO_DRAW); //前约束里程
	PDarray[0][1] = Trs_Coor_System(m_JPInforArray_slope[0].Bef_H, LEVEL, TO_DRAW); //前约束高程

	PDarray[1][0] = Trs_Coor_System(m_JPInforArray_slope[0].Edcml, MILE, TO_DRAW); //后约束里程
	PDarray[1][1] = Trs_Coor_System(m_JPInforArray_slope[0].Aft_H, LEVEL, TO_DRAW); //后约束高程;

	//[0][3]为0;[1][3]为前一坡段坡度 [NBPD - 1][3]为最后一坡段坡度
	if (NBPD > 2 && m_JPInforArray_slope[0].Is_JP_Bef == 1 && m_JPInforArray_slope[0].Is_JP_Aft == 1) //前后约束
	{
		//起点标高
		BPD_array[0][1] = PDarray[0][1];
		//起点坡度
		BPD_array[1][3] = m_JPInforArray_slope[0].Bef_PD;
		//第二点标高
		BPD_array[1][1] = BPD_array[0][1] + m_JPInforArray_slope[0].Bef_PD*0.1*(BPD_array[1][0] - BPD_array[0][0]);

		//终点高程
		BPD_array[NBPD - 1][1] = PDarray[1][1];
		//第二点高程
		BPD_array[NBPD - 2][1] = BPD_array[NBPD - 1][1] - (BPD_array[NBPD - 1][0] - BPD_array[NBPD - 2][0])*m_JPInforArray_slope[0].Aft_PD*0.1;
		//第二点坡度
		BPD_array[NBPD - 1][3] = m_JPInforArray_slope[0].Aft_PD;
	}
	else if (m_JPInforArray_slope[0].Is_JP_Bef == 1 && m_JPInforArray_slope[0].Is_JP_Aft == 0) //前约束后不约束
	{
		//前里程
		BPD_array[0][0] = PDarray[0][0];
		//起点标高
		BPD_array[0][1] = PDarray[0][1];
		//起点坡度
		BPD_array[1][3] = m_JPInforArray_slope[0].Bef_PD;
		//第二点标高
		BPD_array[1][1] = BPD_array[0][1] + m_JPInforArray_slope[0].Bef_PD*0.1*(BPD_array[1][0] - BPD_array[0][0]);
	}
	else if (m_JPInforArray_slope[0].Is_JP_Bef == 0 && m_JPInforArray_slope[0].Is_JP_Aft == 1) //前不约束后约束
	{
		//终里程
		BPD_array[NBPD - 1][0] = PDarray[1][0];
		//终点高程
		BPD_array[NBPD - 1][1] = PDarray[1][1];
		//第二点高程
		BPD_array[NBPD - 2][1] = BPD_array[NBPD - 1][1] - (BPD_array[NBPD - 1][0] - BPD_array[NBPD - 2][0])*m_JPInforArray_slope[0].Aft_PD*0.1;
		//第二点坡度
		BPD_array[NBPD - 1][3] = m_JPInforArray_slope[0].Aft_PD;
	}
}