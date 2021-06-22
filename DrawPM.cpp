// DrawPM.cpp: implementation of the CDrawPM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawPM.h"
#include "OutTAB.h"
#include "GTZDM.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDrawPM::CDrawPM()
{
	pm = NULL;	
	ifsgjk = true;
}

CDrawPM::CDrawPM(JD_CENTER *pz,double sdml,double edml)
{
	pm = pz;
	Sdml=sdml;
	Edml=edml;
	texth = 3.0;
	Scml=pm->TYLC(Sdml);
	if(Scml < pm->DLArray[0].TYLC)
		Scml = pm->DLArray[0].TYLC;
	Ecml=pm->TYLC(Edml);
	if(Ecml > pm->DLArray[pm->DLNum-1].TYLC)
		Ecml = pm->DLArray[pm->DLNum-1].TYLC;
	LcSpacer =100;
	NCRS = 0;
	hdmdes = NULL;
	NZLMK=NYLMK=0;
	ZLMK=YLMK=NULL;
	//读横断面路面宽分段
	BxNum = 0;
	ReadLMK("左路面宽分段表");
	ReadLMK("右路面宽分段表");
	ZGLR.glxl="";
	YGLR.glxl="";
	HoleNum = 0;
	ZxWid = 0;
	YdxWid = 1.0;
	Yzdm =Zzdm=zdm=NULL;
	m_ZDKLineColor = GREEN;
	m_IsGutter = false;
}

CDrawPM::~CDrawPM()
{

}

void CDrawPM::CalOffsetByXLZX(double cml,int NLMK,LMKdata lmk[],double& OffsetByXLZX)
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


double CDrawPM::GetLMK(double cml,LMKdata lmk[],int nlmk)
{
	int iwid;
	double wid;
	double half_fgdk,lydk,cxdk,yljk,tljk;
	double lmhp,yljhp,tljhp;
	int bkmode,sjbgmode;

	CalLMKNoJK(cml,nlmk,lmk,half_fgdk,lydk,cxdk,yljk,tljk,
		lmhp,yljhp,tljhp,bkmode,sjbgmode);
	//   iwid = FindIlmk(cml,lmk,nlmk);
	wid = half_fgdk+lydk+cxdk+yljk+tljk;
	//ads_printf(L"cml=%lf half_fgdk=%lf lydk=%lf cxdk=%lf yljk=%lf tlj=%lf wid=%lf\n",cml, half_fgdk,lydk,cxdk,yljk,tljk,wid);
	return wid;
}

// void CDrawPM::CalLMKNoJK(double cml,int NLMK,LMKdata lmk[],double &half_fgdk,
// 					double &lydk,double &cxdk,double &yljk,double &tljk,
// 					double &lmhp,double &yljhp,double &tljhp,
// 					int &bkmode,int &sjbgmode)
// {
// 	double k;
// 	double addwid;
// 
//     for(int i=0;i<NLMK;i++)
// 	 {
// 	    if(cml<lmk[i].cml+0.1)
// 			break;
// 	 }
// 
// 	if(i>0 && i<NLMK && fabs(lmk[i].cml-lmk[i-1].cml)>0.0001)
// 	   k = (cml - lmk[i-1].cml)/(lmk[i].cml-lmk[i-1].cml);
// 	else 
//        k = 0;
// 
// 	 if(i==0)
// 	    i = 1;
// 	 else if(i==NLMK)
// 		i = NLMK-1;
// 
// 	 if(lmk[i].bkmode!=0)
// 		 k = 4*k*k*k - 3*k*k*k*k;
//      bkmode = lmk[i].bkmode;
// 	 sjbgmode = lmk[i].sjbgmode;
// 
//      half_fgdk = lmk[i-1].fgdk + k*(lmk[i].fgdk- lmk[i-1].fgdk);
// 	 lydk = lmk[i-1].lydk + k*(lmk[i].lydk- lmk[i-1].lydk);
// 	 cxdk = lmk[i-1].cxdk + k*(lmk[i].cxdk- lmk[i-1].cxdk) + addwid;
// 
// 	 yljk = lmk[i-1].yljk + k*(lmk[i].yljk- lmk[i-1].yljk);
// 	 tljk = lmk[i-1].tljk + k*(lmk[i].tljk- lmk[i-1].tljk);
// 	 lmhp = lmk[i].lmhp;
// 	 yljhp = lmk[i].yljhp;
// 	 if(yljhp>0.1)//非爬坡车道
// 		 yljhp = lmhp;
// 	 else
// 	 {
// 		 if(fabs(lmhp)<=4.0)
// 	           yljhp = lmhp;
// 		 else
// 			   yljhp = sgn(lmhp)*4.0;
// 			   
// 	 }
// 	 tljhp = lmk[i].tljhp;
// 

// 
// }

//////////////////////////////////////////////////////////////////////////
//当路面宽度数组只有一个时，出现数组越界。addwid未赋值
//修正后的函数
//2008-02-29
void CDrawPM::CalLMKNoJK(double cml,int NLMK,LMKdata lmk[],double &half_fgdk,
						 double &lydk,double &cxdk,double &yljk,double &tljk,
						 double &lmhp,double &yljhp,double &tljhp,
						 int &bkmode,int &sjbgmode)
{
	double k;
	double addwid = 0.0;

	if (NLMK==1)
	{
		bkmode = lmk[0].bkmode;
		sjbgmode = lmk[0].sjbgmode;

		half_fgdk = lmk[0].fgdk;
		lydk = lmk[0].lydk;
		cxdk = lmk[0].cxdk;
		yljk = lmk[0].yljk;
		tljk = lmk[0].tljk;
		lmhp = lmk[0].lmhp;
		yljhp = lmk[0].yljhp;
		if(yljhp>0.1)//非爬坡车道
			yljhp = lmhp;
		else
		{
			if(fabs(lmhp)<=4.0)
				yljhp = lmhp;
			else
				yljhp = sgn(lmhp)*4.0;

		}
		tljhp = lmk[0].tljhp;
		return;
	}

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

	if(i==NLMK)
		i = NLMK-1;

	if(i==0)
	{
		bkmode = lmk[0].bkmode;
		sjbgmode = lmk[0].sjbgmode;

		half_fgdk = lmk[0].fgdk;
		lydk = lmk[0].lydk;
		cxdk = lmk[0].cxdk + addwid;

		yljk = lmk[0].yljk;
		tljk = lmk[0].tljk;
		lmhp = lmk[i].lmhp;
		yljhp = lmk[i].yljhp;
	}

	else
	{
		if(lmk[i].bkmode!=0)
			k = 4*k*k*k - 3*k*k*k*k;
		bkmode = lmk[i].bkmode;
		sjbgmode = lmk[i].sjbgmode;

		half_fgdk = lmk[i-1].fgdk + k*(lmk[i].fgdk- lmk[i-1].fgdk);
		lydk = lmk[i-1].lydk + k*(lmk[i].lydk- lmk[i-1].lydk);
		cxdk = lmk[i-1].cxdk + k*(lmk[i].cxdk- lmk[i-1].cxdk) + addwid;

		yljk = lmk[i-1].yljk + k*(lmk[i].yljk- lmk[i-1].yljk);
		tljk = lmk[i-1].tljk + k*(lmk[i].tljk- lmk[i-1].tljk);
		lmhp = lmk[i].lmhp;
		yljhp = lmk[i].yljhp;
	}


	if(yljhp>0.1)//非爬坡车道
		yljhp = lmhp;
	else
	{
		if(fabs(lmhp)<=4.0)
			yljhp = lmhp;
		else
			yljhp = sgn(lmhp)*4.0;

	}
	tljhp = lmk[i].tljhp;
}
//////////////////////////////////////////////////////////////////////////


int CDrawPM::FindIlmk(double cml,LMKdata lmk[],int nlmk)
{
	int i;
	double ml;

	for(i=0; i<nlmk; i++)
	{
		ml = lmk[i].xlc;//分段终现场里程
		ml = pm->TYLC(ml);
		if(cml<ml || fabs(ml-cml)<0.001)
			//所给里程处于第I个分段内
				return i;
	}

	return i-1;
}
void CDrawPM::ReadLMK(CString LMKTABname)
{
	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(pm->mdbname,LMKTABname,pm->RoadName);

	//   ads_printf(L"lmk=%d\n",xlmdb.NLMK);
	FILE *fpr;
	CString name=pm->mdbname;
	name.MakeUpper();
	int pos,len,k;
	pos = name.Find(L"DATA");
	len = name.GetLength();
	name=name.Right(len-pos-5);
	CString fname;
	ReadWorkdir();
	fname =Cworkdir + "\\DATA\\" + "BXLMKD." + name + pm->RoadName;
	if((fpr=_wfopen(fname,L"r"))!=NULL)
	{
		BXLMKD bxtmp;
		fwscanf(fpr,L"%d",&BxNum);
		pBXArr = new BXLMKD[BxNum];			
		for(k=0;k<BxNum;k++)
		{
			fwscanf(fpr,L"%lf%lf%d%d%d",&pBXArr[k].bxsml,&pBXArr[k].bxeml,&pBXArr[k].LorR,&pBXArr[k].bxinf,&pBXArr[k].ptnum);
			pBXArr[k].ptarr = new double[pBXArr[k].ptnum*3];
			for(int j=0;j<pBXArr[k].ptnum;j++)
				fwscanf(fpr,L"%lf%lf%lf",&pBXArr[k].ptarr[j*3],&pBXArr[k].ptarr[j*3+1],&pBXArr[k].ptarr[j*3+2]);
		}
		fclose(fpr); 
	}	

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

// 绘交点连线	20190706修改	ljx
void CDrawPM::DrawJDLine()
{
	//设置参数
	double aa;
	int DrawJdSn, DrawJdEn, c, i, n;
	AcGePoint3d Pt,PtText;

	//检测交点数量
	if(pm->JdCenNum<=0)
		return;
	DrawJdSn = 0; DrawJdEn= pm->JdCenNum-1;	//交点起点下标，终点下标初始赋值

	//寻找指定【绘图起点里程】的交点下标
	for (i = 1; i < pm->XYNum; i++)
	{
		if (pm->XYArray[i][6]>Scml - 0.01)
		{
			DrawJdSn = pm->XYJDNO[i] - 1;
			break;
		}
	}
	//寻找指定【绘图终点里程】的交点下标
	for (i = 1; i <= pm->XYNum; i++)
	{

		if (pm->XYArray[i][6] > Ecml - 0.01)
		{
			DrawJdEn = pm->XYJDNO[i];
			break;
		}
	}

	//开始绘图
	AcGePoint2dArray Pt2dAy;
	//AcGePoint3d Spt, Ept;
	//ads_point p1, pt1, pt2, p0;
	//double ang;
	//double rr = BaseSettting::JDCirRadius*DFACTOR;
	double INDATA[5];
	AcGePoint3d tempAcGePt;
	int Text_Color = 1;	//交点号的颜色
	ACHAR Layer_Name[256], tempJDACHAR[256], LayerNameJD[256];
	_tcscpy(Layer_Name, L"平面图-交点连线");
	_tcscpy(LayerNameJD, L"平面图-交点信息");
	for (i = DrawJdSn; i <= DrawJdEn; i++)
	{
		//原先是断开绘大圆，现在改为整体polyline
		//p0[0] = pm->JdCenArray[i].JDE;
		//p0[1] = pm->JdCenArray[i].JDN;
		//p1[0] = pm->JdCenArray[i + 1].JDE;
		//p1[1] = pm->JdCenArray[i + 1].JDN;
		//ang = ads_angle(p0, p1);
		//ads_polar(p0, ang, rr, pt1);
		//ads_polar(p1, ang, -1.0*rr, pt2);
		//Spt.x = pt1[0]; Spt.y = pt1[1];
		//Ept.x = pt2[0]; Ept.y = pt2[1];
		//makeline(Spt, Ept, BaseSettting::JDLineColor, 0.0, L"交点连线");
		Pt2dAy.append(AcGePoint2d(pm->JdCenArray[i].JDE, pm->JdCenArray[i].JDN));

		//20190707	绘制交点号
		double Projml = pm->PROJ_ML(pm->JdCenArray[i].JDN, pm->JdCenArray[i].JDE);	//计算交点所在线路上投影里程
		int Geo_Index = pm->GetXYNoFromTYLC(Projml);	//获取交点所处的线元编号
		for (int k = 0; k < 5; k++)
			INDATA[k] = pm->XYArray[Geo_Index][k];

		double Length = pm->XYArray[Geo_Index][2];
		xlpoint Rail_Pt_Start, Rail_Pt_Mid;
		Rail_Pt_Start.lc = pm->XYArray[Geo_Index][6];
		Rail_Pt_Start.x = pm->XYArray[Geo_Index][8];
		Rail_Pt_Start.y = pm->XYArray[Geo_Index][9];
		Rail_Pt_Start.a = pm->XYArray[Geo_Index][5];
		Rail_Pt_Start.r = pm->XYArray[Geo_Index][7];
		xlzbxy(&Rail_Pt_Start, &Rail_Pt_Mid, 0.5 * Length, INDATA, 0);	//计算线元中点坐标
		double Angle = Rail_Pt_Mid.a;
		Angle = FwjtoAngX(Angle);
		int LorR = (int)(pm->XYArray[Geo_Index][4] + 0.01);		//左或者右
		if (LorR == 0 || LorR > 1 || LorR < -1)
			LorR = -1;
		//tempAcGePt.x = pm->JdCenArray[i].JDE + 1.25 * texth * cos(Angle + LorR * 0.5 * pi);
		//tempAcGePt.y = pm->JdCenArray[i].JDN + 1.25 * texth * sin(Angle + LorR * 0.5 * pi);
		tempAcGePt.x = pm->JdCenArray[i].JDE + 1.25 * m_DrawPara.TextHPara.IntersectionInforTextH * cos(Angle + LorR * 0.5 * pi);
		tempAcGePt.y = pm->JdCenArray[i].JDN + 1.25 * m_DrawPara.TextHPara.IntersectionInforTextH * sin(Angle + LorR * 0.5 * pi);
		tempAcGePt.z = 0;
		_stprintf(tempJDACHAR, L"%s%.0lf", pm->JDGH, pm->JdCenArray[i].JdNo);
		maketext(tempAcGePt, tempJDACHAR, Angle, m_DrawPara.TextHPara.IntersectionInforTextH, Text_Color, 3, L"宋体", LayerNameJD);
	}
	//makepolyline(Pt2dAy, BaseSettting::JDLineColor, 0.0, L"交点连线");
	makepolyline(Pt2dAy, 3, 0.0, L"交点连线");	//BaseSettting::JDLineColor	这个参数现在无法设置，没有意义，先按给定改路模板颜色（绿色）绘制	20190706	ljx

	// 绘交点处圆	目前暂时废弃	20190706
	/*	
	double b, dd, de, th;
	th = texth;
	for (i = DrawJdSn; i <= DrawJdEn; i++)
	{

	Pt[X] = pm->JdCenArray[i].JDE; Pt[Y] = pm->JdCenArray[i].JDN;

	ACHAR JDSTR[20];
	if (i > 0 && i < pm->JdCenNum - 1)
	{
	double tempaa = pm->JdCenArray[i].PreFwj + PI;
	if (tempaa > 2.0*PI)tempaa -= 2.0*PI;

	aa = pm->JdCenArray[i].AftFwj - tempaa;
	if (aa < -pi) aa = aa + 2 * pi;
	if (aa > pi) aa = aa - 2 * pi;

	if (aa > 0) { c = 1; dd = th; }  //右偏
	else { c = -1; dd = th + th; }; //左偏
	de = tempaa + aa*0.5;

	_stprintf(JDSTR, L"%s%.0lf", pm->JDGH, pm->JdCenArray[i].JdNo);
	n = _tcslen(JDSTR);
	PtText.x = Pt[X] - n*0.5*texth*sin(de) + dd*sin(de - c*pi*0.5);
	PtText.y = Pt[Y] - n*0.5*texth*cos(de) + dd*cos(de - c*pi*0.5);

	b = 0.5*pi - de;
	}
	makecircle(Pt, rr, BaseSettting::JDLineColor);
	if (i > 0 && i < pm->JdCenNum - 1)
	maketext(PtText, JDSTR, b, th, BaseSettting::JDLineColor, 1);
	}*/
}

void CDrawPM::DrawXLZX()
{
	xlpoint PB,PE;
	double INDATA[5];
	int j;

	// 绘线位
	PB.x=pm->XYArray[0][1]; //N
	PB.y=pm->XYArray[0][2]; // E
	PB.a=pm->XYArray[0][3];// FA
	PB.lc=pm->XYArray[0][4];
	PB.r=1e40;
	PE.x=PE.y =PE.lc=PE.a=0.0;
	int DrawXySn,DrawXyEn,i;
	AcGePoint3d Pt,PtText;
	double cml;

	//寻找指定起始里程线元下标
	DrawXySn = 1; DrawXyEn= pm->XYNum;
	for (i = 1; i < pm->XYNum; i++)
	{
		if (pm->XYArray[i][6]>Scml - 0.01)
		{
			DrawXySn = i;
			break;
		}
	}
	//寻找指定终止里程线元下标
	for (i = 1; i <= pm->XYNum; i++)
	{
		if (pm->XYArray[i][6] > Ecml - 0.01)
		{
			DrawXyEn = i - 1;
			break;
		}
	}
	xlpoint  PZtmp;
	AcGePoint2dArray Pt2dAy, Pt2dAy1;
	AcDbObjectId qID, hID;
	if (DrawXySn > 1)
	{
		cml = Scml;
		PZtmp.lc = cml;
		pm->xlpoint_pz(&PZtmp);
		Pt2dAy.append(AcGePoint2d(PZtmp.y, PZtmp.x));
		if (pm->XYArray[DrawXySn - 1][0] < 1.1)//直线
		{
			Pt2dAy.append(AcGePoint2d(pm->XYArray[DrawXySn][9], pm->XYArray[DrawXySn][8]));
			qID = makepolyline(Pt2dAy, 7, ZxWid, L"中线");
		}
		else if (pm->XYArray[DrawXySn - 1][0] < 2.1)//圆
		{
			qID = makearc(AcGePoint3d(PZtmp.y,PZtmp.x,0.0),AcGePoint3d(pm->XYArray[DrawXySn][9],pm->XYArray[DrawXySn][8],0.0),pm->XYArray[DrawXySn-1][1],pm->XYArray[DrawXySn-1][4]);
			if(ZxWid>0.01)
			{
				ads_point ptt;
				ptt[X] = pm->XYArray[DrawXySn][9], ptt[Y] = pm->XYArray[DrawXySn][8];
#ifdef VERSION_CAD2016
				acedCommandS(RTSTR, L"Pedit", RTPOINT, ptt, RTSTR, L"Y", RTSTR, L"W", RTREAL, ZxWid, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
				ads_command(RTSTR, L"Pedit", RTPOINT, ptt, RTSTR, L"Y", RTSTR, L"W", RTREAL, ZxWid, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010		
			}
		}
		else//缓和曲线
		{
			cml += 5;
			while (cml < pm->XYArray[DrawXySn][6])
			{
				PZtmp.lc = cml;
				pm->xlpoint_pz(&PZtmp);
				Pt2dAy.append(AcGePoint2d(PZtmp.y, PZtmp.x));
				cml += 5;
			}
			PZtmp.lc = pm->XYArray[DrawXySn][6];
			pm->xlpoint_pz(&PZtmp);
			Pt2dAy.append(AcGePoint2d(PZtmp.y, PZtmp.x));
			qID = makepolyline(Pt2dAy, 7, ZxWid, L"中线");
		}
		SetXLXdata(qID, pm->mdbname, pm->RoadName);
	}

	for (i = DrawXySn; i < DrawXyEn; i++)
	{
		for (j = 0; j < 5; j++)INDATA[j] = pm->XYArray[i][j];
		PB.a = pm->XYArray[i][5];
		PB.x = pm->XYArray[i][8];
		PB.y = pm->XYArray[i][9];
		hID = GUXWXY(INDATA, &PB, &PE, 1, 2, ZxWid, L"中线");//绘线路中线
		//SetXLXdata(hID,pm->mdbname,pm->RoadName); 
	}

	if (DrawXyEn < pm->XYNum)
	{
		cml = pm->XYArray[DrawXyEn][6];
		Pt2dAy1.append(AcGePoint2d(pm->XYArray[DrawXyEn][9], pm->XYArray[DrawXyEn][8]));
		if (pm->XYArray[DrawXyEn][0] < 1.1)//直线
		{
			cml = Ecml;
			PZtmp.lc = cml;
			pm->xlpoint_pz(&PZtmp);
			Pt2dAy1.append(AcGePoint2d(PZtmp.y, PZtmp.x));
			hID = makepolyline(Pt2dAy1, 7, ZxWid, L"中线");
		}
		else if (pm->XYArray[DrawXyEn][0] < 2.1)//圆
		{
			PZtmp.lc = Ecml;
			pm->xlpoint_pz(&PZtmp);
			hID = makearc(AcGePoint3d(pm->XYArray[DrawXyEn][9],pm->XYArray[DrawXyEn][8],0.0),AcGePoint3d(PZtmp.y,PZtmp.x,0.0),pm->XYArray[DrawXyEn][1],pm->XYArray[DrawXyEn][4]);
			if(ZxWid>0.01)
			{
				ads_point ptt;
				ptt[X] = PZtmp.y, ptt[Y] = PZtmp.x;
#ifdef VERSION_CAD2016
				acedCommandS(RTSTR, L"Pedit", RTPOINT, ptt, RTSTR, L"Y", RTSTR, L"W", RTREAL, ZxWid, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
				ads_command(RTSTR, L"Pedit", RTPOINT, ptt, RTSTR, L"Y", RTSTR, L"W", RTREAL, ZxWid, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010	

			}
		}
		else
		{
			cml += 5;
			while (cml < Ecml)
			{
				PZtmp.lc = cml;
				pm->xlpoint_pz(&PZtmp);
				Pt2dAy1.append(AcGePoint2d(PZtmp.y, PZtmp.x));
				cml += 5;
			}
			PZtmp.lc = Ecml;
			pm->xlpoint_pz(&PZtmp);
			Pt2dAy1.append(AcGePoint2d(PZtmp.y, PZtmp.x));
			hID = makepolyline(Pt2dAy1, 7, ZxWid, L"中线");
		}

		SetXLXdata(hID, pm->mdbname, pm->RoadName);
		//		SetXYXdata(qID,hID,pm->XYArray[i-1]);
		//		SetXYXdata(hID,0,pm->XYArray[i]);
	}


}

void CDrawPM::Draw_Center_Line()	//20190707添加	绘制线路中心线
{
	xlpoint PB, PE;
	double INDATA[5];
	int j;

	// 绘线位
	PB.x = pm->XYArray[0][1]; //N
	PB.y = pm->XYArray[0][2]; // E
	PB.a = pm->XYArray[0][3];// FA
	PB.lc = pm->XYArray[0][4];
	PB.r = 1e40;
	PE.x = PE.y = PE.lc = PE.a = 0.0;
	int DrawXySn, DrawXyEn, i;
	AcGePoint3d Pt, PtText;
	double cml;

	//寻找指定起始里程线元下标
	DrawXySn = 1; DrawXyEn = pm->XYNum;
	for (i = 1; i < pm->XYNum; i++)
	{
		if (pm->XYArray[i][6]>Scml - 0.01)
		{
			DrawXySn = i;
			break;
		}
	}
	//寻找指定终止里程线元下标
	for (i = 1; i <= pm->XYNum; i++)
	{
		if (pm->XYArray[i][6] > Ecml - 0.01)
		{
			DrawXyEn = i - 1;
			break;
		}
	}
	xlpoint  PZtmp;
	AcGePoint2dArray Pt2dAy;
	AcGePoint3dArray Pt3dArray;
	AcDbObjectId qID, hID;
	if (DrawXySn > 1)
	{
		cml = Scml;
		PZtmp.lc = cml;
		pm->xlpoint_pz(&PZtmp);
		Pt3dArray.append(AcGePoint3d(PZtmp.y, PZtmp.x, 0));
		if (pm->XYArray[DrawXySn - 1][0] < 1.1)//直线
		{
			Pt3dArray.append(AcGePoint3d(pm->XYArray[DrawXySn][9], pm->XYArray[DrawXySn][8], 0));
		}
		else if (pm->XYArray[DrawXySn - 1][0] < 2.1)//圆
		{
			int NUM = Pt3dArray.length();
			Pt3dArray[NUM - 1].z = -tan((pm->XYArray[DrawXySn - 1][2] - Scml + pm->XYArray[DrawXySn - 1][6]) / pm->XYArray[DrawXySn - 1][1] / 4)*pm->XYArray[DrawXySn - 1][4];
			Pt3dArray.append(AcGePoint3d(pm->XYArray[DrawXySn][9], pm->XYArray[DrawXySn][8], 0.0));
		}
		else//缓和曲线
		{
			cml += 2;
			while (cml < pm->XYArray[DrawXySn][6])
			{
				PZtmp.lc = cml;
				pm->xlpoint_pz(&PZtmp);
				Pt3dArray.append(AcGePoint3d(PZtmp.y, PZtmp.x, 0));
				cml += 2;
			}
			PZtmp.lc = pm->XYArray[DrawXySn][6];
			pm->xlpoint_pz(&PZtmp);
			Pt3dArray.append(AcGePoint3d(PZtmp.y, PZtmp.x, 0));
		}
		//SetXLXdata(qID, pm->mdbname, pm->RoadName);	扩展数据这块暂时还不知道怎么处理
	}

	for (i = DrawXySn; i < DrawXyEn; i++)
	{
		//加入每个线元的起点
		Pt3dArray.append(AcGePoint3d(pm->XYArray[i][9], pm->XYArray[i][8], 0));
		cml = pm->XYArray[i][6];
		if (pm->XYArray[i][0] > 1.1 && pm->XYArray[i][0] < 2.1)	//如果线元是圆曲线，需要修改曲率
		{
			int NUM = Pt3dArray.length();
			Pt3dArray[NUM - 1].z = -tan(pm->XYArray[i][2] / pm->XYArray[i][1] / 4) * pm->XYArray[i][4];
		}
		else if (pm->XYArray[i][0] > 2.1)	//如果线元是缓和曲线，需要按等间距加入点
		{
			cml += 2;
			while (cml < pm->XYArray[i + 1][6])
			{
				PZtmp.lc = cml;
				pm->xlpoint_pz(&PZtmp);
				Pt3dArray.append(AcGePoint3d(PZtmp.y, PZtmp.x, 0));
				cml += 5;
			}
		}
		//SetXLXdata(hID,pm->mdbname,pm->RoadName); 
	}

	if (DrawXyEn < pm->XYNum)
	{
		cml = pm->XYArray[DrawXyEn][6];
		Pt3dArray.append(AcGePoint3d(pm->XYArray[DrawXyEn][9], pm->XYArray[DrawXyEn][8], 0));
		if (pm->XYArray[DrawXyEn][0] < 1.1)//直线
		{
			cml = Ecml;
			PZtmp.lc = cml;
			pm->xlpoint_pz(&PZtmp);
			Pt3dArray.append(AcGePoint3d(PZtmp.y, PZtmp.x, 0));
		}
		else if (pm->XYArray[DrawXyEn][0] < 2.1)//圆
		{
			//修改前一个点的曲率
			int NUM = Pt3dArray.length();
			Pt3dArray[NUM - 1].z = -tan((Ecml - cml) / pm->XYArray[DrawXyEn][1] / 4)*pm->XYArray[DrawXyEn][4];
			//加上最后一个点
			PZtmp.lc = Ecml;
			pm->xlpoint_pz(&PZtmp);
			Pt3dArray.append(AcGePoint3d(PZtmp.y, PZtmp.x, 0));
		}
		else
		{
			cml += 5;
			while (cml < Ecml)
			{
				PZtmp.lc = cml;
				pm->xlpoint_pz(&PZtmp);
				Pt3dArray.append(AcGePoint3d(PZtmp.y, PZtmp.x, 0));
				cml += 5;
			}
			PZtmp.lc = Ecml;
			pm->xlpoint_pz(&PZtmp);
			Pt3dArray.append(AcGePoint3d(PZtmp.y, PZtmp.x, 0));
		}

		//SetXLXdata(hID, pm->mdbname, pm->RoadName);
	}
	makepolylineTD(Pt3dArray, 5, ZxWid, L"平面图-中线", L"CONTINUOUS");
	//qID = makepolyline(Pt3dArray, 5, ZxWid, L"中线");
}

void CDrawPM::BZLC(int inf)//0线位图 1 平面图
{
	//	pm->BZLC(Scml,Ecml,LcSpacer,-1,1,1,texth,XSW,DFACTOR,inf);
	//pm->BZLC(Scml,Ecml,LcSpacer,-1,1,1,texth,XSW,DFACTOR,inf);
	pm->BZLC(Scml, Ecml, LcSpacer, -1, 1, 1, m_DrawPara.TextHPara.MileTextH, XSW, DFACTOR, inf);
}

void CDrawPM::DrawSeML()//标注起终点
{
	pm->DrawSEML(-1,1,texth,XSW);
}

void CDrawPM::BZTZD(int inf)//0线位图 1 平面图
{
	pm->BzRAL = BzRAL;
	//	pm->BZTZD(Scml,Ecml,inf,1,texth,XSW);
	//pm->BZTZD(Scml,Ecml,inf,1,texth,XSW);
	if(BzRAL)
		pm->BZTZD(Scml, Ecml, inf, 1, m_DrawPara.TextHPara.PileTextH, XSW);
}

void CDrawPM::Draw_PI_Infor()
{
	//pm->Draw_PI_Infor(Scml, Ecml, texth, XSW, BzRAL, m_IsGutter);
	pm->Draw_PI_Infor(Scml, Ecml, m_DrawPara.TextHPara.IntersectionInforTextH, XSW, BzRAL, m_IsGutter);
}

//读hdmresult文件
void CDrawPM::ReadHdmResultFile()
{
	CString filename;
	FILE *fpr;

	filename =Cworkdir + "\\data\\" + "hdmresult." + Pro + "." + pm->RoadName;
	if((fpr=_wfopen(filename ,L"r"))==NULL)
	{
		//ads_alert(L"无法打开hdmresult文件!");	//暂时屏蔽
		return ;
	}
	fwscanf(fpr,L"%d",&NCRS);
	NCRS = fabs(1.0*NCRS);
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

	DBS.Read_XLDbs(pm->mdbname,L"大中桥",pm->RoadName);	
	for(int i=0;i<DBS.BriNum;i++)
	{
		DBS.Bri[i].Scml = pm->TYLC(DBS.Bri[i].StDml);//起点里程
		DBS.Bri[i].Ecml = pm->TYLC(DBS.Bri[i].EdDml);//起点里程
	}
	return;
}

double CDrawPM::FindBriAng(double cml)
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

/*
void CDrawPM::ReadACrossResultData(FILE *fpr,int jcross)
{
char tmp[256];
AcGePoint3d pt;
int itmp;
double dtmp;

fwscanf(fpr,L"%lf%lf%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
&(hdmdes[jcross].cml),
&(hdmdes[jcross].dml),
&(hdmdes[jcross].DuanLuoInf),
&(hdmdes[jcross].DorS),
&(hdmdes[jcross].LZDK),
&(hdmdes[jcross].RZDK),
&(hdmdes[jcross].Ldeltah),
&(hdmdes[jcross].Rdeltah),
&(hdmdes[jcross].Y_Ldeltah),
&(hdmdes[jcross].Y_Rdeltah),
&(hdmdes[jcross].newLYd),
&(hdmdes[jcross].newRYd),
&(hdmdes[jcross].xjj),
&(hdmdes[jcross].FLDcml),
tmp
);
hdmdes[jcross].DorS=1;
hdmdes[jcross].FLDname = tmp;
CString GH;
double dml = pm->XLC(hdmdes[jcross].cml,GH);
_tcscpy(hdmdes[jcross].ckml,pm->LCchr(GH,dml,3));
//1.写左线左侧
fwscanf(fpr,L"%d",&hdmdes[jcross].ZDMXZ);
for(int j=0;j<3;j++)
fwscanf(fpr,L"%lf",&hdmdes[jcross].zlmhd[j]);

//写路面点
for(j=0;j<5;j++)
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].RHFL[2*j],&hdmdes[jcross].RHFL[2*j+1]);
//左设计点
fwscanf(fpr,L"%lf%lf%d",&hdmdes[jcross].Zt,&hdmdes[jcross].ZHc,&hdmdes[jcross].Lnpt);//换行
xlpoint PZtmp;
PZtmp.lc=hdmdes[jcross].cml;  // 统一里程
pm->xlpoint_pz(&PZtmp);  
for(j=0; j<hdmdes[jcross].Lnpt; j++)
{
double x,y;
fwscanf(fpr,L"%lf%lf",&x,&y);
pt.x = x, pt.y =y; pt.z =0;
hdmdes[jcross].RPTL.append(pt);
if(hdmdes[jcross].DorS==1)//单路面
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRPTL.append(pt);
}
else//双路面
{
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRPTL.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + fabs(x+hdmdes[jcross].xjj) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x+hdmdes[jcross].xjj) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRPTL.append(pt);
}
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
if(hdmdes[jcross].DorS==1)//单路面
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTLgou.append(pt);
}
else//双路面
{
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTLgou.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + fabs(x+hdmdes[jcross].xjj) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x+hdmdes[jcross].xjj) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTLgou.append(pt);
}
}
}
//左截水沟
for(j=0;j<hdmdes[jcross].LJSGouPtNum;j++)
{
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].LJSgou[j*2],&hdmdes[jcross].LJSgou[j*2+1]);
double x,y;
x = hdmdes[jcross].LJSgou[j*2];
y = hdmdes[jcross].LJSgou[j*2+1];
if(hdmdes[jcross].DorS==1)//单路面
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTLJSgou.append(pt);
}
else//双路面
{
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTLJSgou.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + fabs(x+hdmdes[jcross].xjj) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x+hdmdes[jcross].xjj) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTLJSgou.append(pt);
}
}
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
pt.x = x, pt.y =y; pt.z =0;
hdmdes[jcross].RPTR.append(pt);
if(hdmdes[jcross].DorS==1)//单路面
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRPTR.append(pt);
}
else//双路面
{
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRPTR.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + (fabs(hdmdes[jcross].xjj)-x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + (fabs(hdmdes[jcross].xjj)-x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRPTR.append(pt);
}
}
}	//右水沟点
fwscanf(fpr,L"%d%lf%d%d%d",&itmp,&dtmp,&hdmdes[jcross].RGouProperty,&hdmdes[jcross].RGouPtNum,&hdmdes[jcross].RJSGouPtNum);
for(j=0; j<hdmdes[jcross].RGouPtNum; j++)
{
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Rgou[j*2],&hdmdes[jcross].Rgou[j*2+1]);
double x,y;
x = hdmdes[jcross].Rgou[j*2];
y = hdmdes[jcross].Rgou[j*2+1];
if(hdmdes[jcross].DorS==1)//单路面
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRgou.append(pt);
}
else//双路面
{
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRgou.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + (fabs(hdmdes[jcross].xjj)-x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + (fabs(hdmdes[jcross].xjj)-x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRgou.append(pt);
}
}
}
//右截水沟
for(j=0;j<hdmdes[jcross].RJSGouPtNum;j++)
{
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].RJSgou[j*2],&hdmdes[jcross].RJSgou[j*2+1]);
double x,y;
x = hdmdes[jcross].RJSgou[j*2];
y = hdmdes[jcross].RJSgou[j*2+1];
if(hdmdes[jcross].DorS==1)//单路面
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRJSgou.append(pt);
}
else//双路面
{
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newLYd + y;
hdmdes[jcross].PTRJSgou.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + (fabs(hdmdes[jcross].xjj)-x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + (fabs(hdmdes[jcross].xjj)-x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newLYd+ y;
hdmdes[jcross].PTRJSgou.append(pt);
}
}
}

if(hdmdes[jcross].DorS==2)//双路面
{
fwscanf(fpr,L"%d",&hdmdes[jcross].Y_ZDMXZ);
for(j=0;j<3;j++)
fwscanf(fpr,L"%lf",&hdmdes[jcross].Y_zlmhd[j]);

//3.写右线左侧
//写路面点
for(j=0;j<5;j++)
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Y_RHFL[2*j],&hdmdes[jcross].Y_RHFL[2*j+1]);
fwscanf(fpr,L""); //一行空行
//右左设计点
fwscanf(fpr,L"%lf%lf%d",&hdmdes[jcross].Y_Zt,&hdmdes[jcross].Y_ZHc,&hdmdes[jcross].Y_Lnpt);
for(j=0;j<hdmdes[jcross].Y_Lnpt;j++)
{
double x,y;
fwscanf(fpr,L"%lf%lf",&x,&y);
pt.x = x, pt.y =y; pt.z =0;
hdmdes[jcross].Y_RPTL.append(pt);
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + (hdmdes[jcross].xjj+x) * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + (hdmdes[jcross].xjj+x) * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTRPTL.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTRPTL.append(pt);
}			
}	//右水沟点

//		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Y_RPTL[j].x,&hdmdes[jcross].Y_RPTL[j].y);
//右左水沟点
fwscanf(fpr,L"%d%lf%d%d%d",&itmp,&dtmp,&hdmdes[jcross].Y_LGouProperty,&hdmdes[jcross].Y_LGouPtNum,&hdmdes[jcross].Y_LJSGouPtNum);
for(j=0; j<hdmdes[jcross].Y_LGouPtNum; j++)
{
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Y_Lgou[j*2],&hdmdes[jcross].Y_Lgou[j*2+1]);
double x,y;
x = hdmdes[jcross].Y_Lgou[j*2];
y = hdmdes[jcross].Y_Lgou[j*2+1];
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + (hdmdes[jcross].xjj+x) * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + (hdmdes[jcross].xjj+x) * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTLgou.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTLgou.append(pt);
}			
}
//右左截水沟
for(j=0;j<hdmdes[jcross].Y_LJSGouPtNum;j++)
{
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Y_LJSgou[j*2],&hdmdes[jcross].Y_LJSgou[j*2+1]);
double x,y;
x = hdmdes[jcross].Y_LJSgou[j*2];
y = hdmdes[jcross].Y_LJSgou[j*2+1];
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + (hdmdes[jcross].xjj+x) * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + (hdmdes[jcross].xjj+x) * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTLJSgou.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTLJSgou.append(pt);
}			
}

//3.写右线右侧
fwscanf(fpr,L"%d",&hdmdes[jcross].Y_YDMXZ);
for(j=0;j<3;j++)
fwscanf(fpr,L"%lf",&hdmdes[jcross].Y_ylmhd[j]);
//写路面点
for(j=0;j<5;j++)
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Y_RHFR[2*j],&hdmdes[jcross].Y_RHFR[2*j+1]);

//右右设计点
fwscanf(fpr,L"%lf%lf%d",&hdmdes[jcross].Y_Yt,&hdmdes[jcross].Y_YHc,&hdmdes[jcross].Y_Rnpt);
for(j=0;j<hdmdes[jcross].Y_Rnpt;j++)
{
double x,y;
fwscanf(fpr,L"%lf%lf",&x,&y);
pt.x = x, pt.y =y; pt.z =0;
hdmdes[jcross].Y_RPTR.append(pt);
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + (hdmdes[jcross].xjj+x) * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + (hdmdes[jcross].xjj+x) * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTRPTR.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTRPTR.append(pt);
}			
}	//右水沟点

//右右水沟点
fwscanf(fpr,L"%d%lf%d%d%d",&itmp,&dtmp,&hdmdes[jcross].Y_RGouProperty,&hdmdes[jcross].Y_RGouPtNum,&hdmdes[jcross].Y_RJSGouPtNum);
for(j=0; j<hdmdes[jcross].Y_RGouPtNum; j++)
{
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Y_Rgou[j*2],&hdmdes[jcross].Y_Rgou[j*2+1]);
double x,y;
x = hdmdes[jcross].Y_Rgou[j*2];
y = hdmdes[jcross].Y_Rgou[j*2+1];
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + (hdmdes[jcross].xjj+x) * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + (hdmdes[jcross].xjj+x) * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTRgou.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTRgou.append(pt);
}			
}
//右右截水沟
for(j=0;j<hdmdes[jcross].Y_RJSGouPtNum;j++)
{
fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Y_RJSgou[j*2],&hdmdes[jcross].Y_RJSgou[j*2+1]);
double x,y;
x = hdmdes[jcross].Y_RJSgou[j*2];
y = hdmdes[jcross].Y_RJSgou[j*2+1];
if(hdmdes[jcross].xjj>=0)//当前pm为左线
{
pt.y = PZtmp.x + (hdmdes[jcross].xjj+x) * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + (hdmdes[jcross].xjj+x) * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTRJSgou.append(pt);
}
else//当前pm为右线
{
pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
pt.z = hdmdes[jcross].newRYd + y;
hdmdes[jcross].Y_PTRJSgou.append(pt);
}			
}
}
}*/

//////////////////////////////////////////////////////////////////////////
//已考虑路基中线与线路中线不重合问题
//////////////////////////////////////////////////////////////////////////
void CDrawPM::ReadACrossResultData(FILE *fpr,int jcross)
{
	ACHAR tmp[256];
	AcGePoint3d pt;
	int itmp;
	double dtmp,FillArea,CutArea;
	double x,y;
	double Z_EarthHOnLJCen,Y_EarthHOnLJCen;//路基中心标高
	double OffsetByXLZX = 0.0;//路基中线相对于线路中线的偏移量

	fwscanf(fpr,L"%lf%lf%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
		&(hdmdes[jcross].cml),
		&(hdmdes[jcross].dml),
		&(hdmdes[jcross].IsInsertHdm),
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
		&dtmp,&dtmp,
		&(hdmdes[jcross].FLDcml),
		tmp
		);
	OffsetByXLZX = hdmdes[jcross].OffsetByXLZX;
	hdmdes[jcross].newRYd = hdmdes[jcross].newLYd;
	Z_EarthHOnLJCen = Y_EarthHOnLJCen = hdmdes[jcross].newLYd;
	if(jcross>0&&fabs(hdmdes[jcross].cml-hdmdes[jcross-1].cml)<0.01)
		hdmdes[jcross].cml+=0.12;//?????????
	hdmdes[jcross].IfHasZlink=0;
	hdmdes[jcross].IfHasYlink=0;
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
	xlpoint PZtmp,ZPZtmp,YPZtmp;
	PZtmp.lc=hdmdes[jcross].cml;  // 统一里程
	pm->xlpoint_pz(&PZtmp);  
	for(j=0; j<hdmdes[jcross].Lnpt; j++)
	{
		fwscanf(fpr,L"%lf%lf",&x,&y);
		//////////////////////////////////////////////////////////////////////////蝶形沟不再计入设计点，无需在修正路堑设计点
		// 		if((fabs(hdmdes[jcross].Zt-1.0)<0.1) && hdmdes[jcross].ZCG_GDR>0.1 && hdmdes[jcross].Lnpt>16)//挖方且碟形沟
		// 			{
		// 				if(j>5  && j<16)
		// 	                continue;
		// 			}
		if (j==0)
		{
			Z_EarthHOnLJCen = hdmdes[jcross].RHFL[1] - y;
		}
		//挖方修正
		// 		if(ifsgjk && fabs(hdmdes[jcross].Zt-1.0)<0.1)
		// 		{
		// 			if(j==7 && fabs(x-hdmdes[jcross].RPTL[4].x)<1.0)
		// 				x = hdmdes[jcross].RPTL[4].x - 1.0;			
		// 		}
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
	//////////////////////////////////////////////////////////////////////////蝶形沟不再计入设计点，无需在修正路堑设计点
	// 	if((fabs(hdmdes[jcross].Zt-1.0)<0.1) && hdmdes[jcross].ZCG_GDR>0.1 && hdmdes[jcross].Lnpt>16)//挖方且碟形沟
	//          hdmdes[jcross].Lnpt = hdmdes[jcross].Lnpt-10;
	//左水沟点
	fwscanf(fpr,L"%d%lf%d%d%d",&itmp,&dtmp,&hdmdes[jcross].LGouProperty,&hdmdes[jcross].LGouPtNum,&hdmdes[jcross].LJSGouPtNum);
	for(j=0; j<hdmdes[jcross].LGouPtNum; j++)
	{
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Lgou[j*2],&hdmdes[jcross].Lgou[j*2+1]);
		x = hdmdes[jcross].Lgou[j*2];
		y = hdmdes[jcross].Lgou[j*2+1];	

		//		if(j==4 && fabs(x-hdmdes[jcross].Lgou[2])<1.0)
		//			x = hdmdes[jcross].Lgou[2] - 1.0;			

		// 		pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
		// 		pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
		// 		pt.z = hdmdes[jcross].newLYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Z_EarthHOnLJCen;
		hdmdes[jcross].PTLgou.append(pt);
		hdmdes[jcross].fwj=PZtmp.a;
		//修正坡脚点
		//	if(j==0)
		//	{
		//	    hdmdes[jcross].PTRPTL[hdmdes[jcross].PTRPTL.length()-1].x=pt.x;
		//		hdmdes[jcross].PTRPTL[hdmdes[jcross].PTRPTL.length()-1].y=pt.y;
		//	}
	}
	//左截水沟
	for(j=0;j<hdmdes[jcross].LJSGouPtNum;j++)
	{
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].LJSgou[j*2],&hdmdes[jcross].LJSgou[j*2+1]);
		x = hdmdes[jcross].LJSgou[j*2];
		y = hdmdes[jcross].LJSgou[j*2+1];	
		// 		pt.y = PZtmp.x + fabs(x) * cos(PZtmp.a - 0.5*PI);
		// 		pt.x = PZtmp.y + fabs(x) * sin(PZtmp.a - 0.5*PI);
		// 		pt.z = hdmdes[jcross].newLYd + y;
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
		fwscanf(fpr,L"%lf%lf",&x,&y);
		//////////////////////////////////////////////////////////////////////////蝶形沟不再计入设计点，无需在修正路堑设计点
		// 		if(fabs(hdmdes[jcross].Yt-1.0)<0.1 && hdmdes[jcross].YCG_GDR>0.1 && hdmdes[jcross].Rnpt>16)//挖方且碟形沟
		// 				{
		// 					if(j>5  && j<16)
		// 		                continue;
		// 				}

		if (j==0)
		{
			Y_EarthHOnLJCen = hdmdes[jcross].RHFR[1] - y;
		}

		// 		if(ifsgjk && fabs(hdmdes[jcross].Yt-1.0)<0.1)
		// 		{
		// 			if(j==7 && fabs(x-hdmdes[jcross].RPTR[4].x)<1.0)
		// 				x = hdmdes[jcross].RPTR[4].x + 1.0;			
		// 		}
		pt.x = x, pt.y =y; pt.z =0;
		hdmdes[jcross].RPTR.append(pt);

		// 		pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
		// 		pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
		// 		pt.z = hdmdes[jcross].newRYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Y_EarthHOnLJCen;
		hdmdes[jcross].PTRPTR.append(pt);
		hdmdes[jcross].fwj=PZtmp.a;

	}	

	// 	if((fabs(hdmdes[jcross].Yt-1.0)<0.1) && hdmdes[jcross].YCG_GDR>0.1 && hdmdes[jcross].Rnpt>16)//挖方且碟形沟
	// 	{
	// 		hdmdes[jcross].Rnpt = hdmdes[jcross].Rnpt-10;
	// 	}

	//右水沟点
	fwscanf(fpr,L"%d%lf%d%d%d",&itmp,&dtmp,&hdmdes[jcross].RGouProperty,&hdmdes[jcross].RGouPtNum,&hdmdes[jcross].RJSGouPtNum);
	for(j=0; j<hdmdes[jcross].RGouPtNum; j++)
	{
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].Rgou[j*2],&hdmdes[jcross].Rgou[j*2+1]);
		x = hdmdes[jcross].Rgou[j*2];
		y = hdmdes[jcross].Rgou[j*2+1];
		//		if(j==4 && fabs(x-hdmdes[jcross].Rgou[2])<1.0)
		//			x = hdmdes[jcross].Rgou[2] + 1.0;			

		// 		pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
		// 		pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
		// 		pt.z = hdmdes[jcross].newRYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Y_EarthHOnLJCen;
		hdmdes[jcross].PTRgou.append(pt);
		//修正坡脚
		//	if(j==0)
		//	{
		//	    hdmdes[jcross].PTRPTR[hdmdes[jcross].PTRPTR.length()-1].x=pt.x;
		//		hdmdes[jcross].PTRPTR[hdmdes[jcross].PTRPTR.length()-1].y=pt.y;
		//	}
	}
	//右截水沟
	for(j=0;j<hdmdes[jcross].RJSGouPtNum;j++)
	{
		fwscanf(fpr,L"%lf%lf",&hdmdes[jcross].RJSgou[j*2],&hdmdes[jcross].RJSgou[j*2+1]);
		x = hdmdes[jcross].RJSgou[j*2];
		y = hdmdes[jcross].RJSgou[j*2+1];

		// 		pt.y = PZtmp.x + x * cos(PZtmp.a + 0.5*PI);
		// 		pt.x = PZtmp.y + x * sin(PZtmp.a + 0.5*PI);
		// 		pt.z = hdmdes[jcross].newRYd + y;
		pt.y = PZtmp.x + (x + OffsetByXLZX) * cos(PZtmp.a + 0.5*PI);
		pt.x = PZtmp.y + (x + OffsetByXLZX) * sin(PZtmp.a + 0.5*PI);
		pt.z = y + Y_EarthHOnLJCen;
		hdmdes[jcross].PTRJSgou.append(pt);
	}

	if(hdmdes[jcross].IfHasZlink )//左有关联道路
	{
		hdmdes[jcross].zlink = new CRSDAT;
		fwscanf(fpr ,L"%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
			&hdmdes[jcross].zlink->cml,
			&hdmdes[jcross].zlink->dml,
			&hdmdes[jcross].zlink->newLYd,
			&hdmdes[jcross].zlink->newRYd,
			&hdmdes[jcross].zlink->xjj,
			&FillArea,
			&CutArea,
			&hdmdes[jcross].zlink->Ldeltah,
			&hdmdes[jcross].zlink->Rdeltah,			
			tmp			
			);        
		hdmdes[jcross].zlink->FLDname = tmp;
		for(j=0;j<hdm.ZRoadSum;j++)//确定左关联道路序号,取平面
		{
			if(hdm.Zroad[j].roadname==hdmdes[jcross].zlink->FLDname)
				break;
		}
		if(j<hdm.ZRoadSum)
		{
			ZPZtmp.lc=hdmdes[jcross].zlink->cml;  // 统一里程
			hdm.Zroad[j].pm->xlpoint_pz(&ZPZtmp);
			hdmdes[jcross].zlink->CenPt[0]=ZPZtmp.x;
			hdmdes[jcross].zlink->CenPt[1]=ZPZtmp.y;
			hdmdes[jcross].zlink->fwj = ZPZtmp.a; 			
		}
		else
		{
			hdmdes[jcross].zlink->CenPt[0]=PZtmp.x;
			hdmdes[jcross].zlink->CenPt[1]=PZtmp.y;
			hdmdes[jcross].zlink->fwj = PZtmp.a; 
		}
		if(fabs(hdmdes[jcross].ZDorS-2)<0.01)//双路面且同坡 取主线方位角
			hdmdes[jcross].zlink->fwj = PZtmp.a;
		//1.写左线左侧
		fwscanf(fpr,L"%d",&hdmdes[jcross].zlink->ZDMXZ);
		//写路面点
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&hdmdes[jcross].zlink->zlmhd[j]);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].zlink->RHFL[2*j],&hdmdes[jcross].zlink->RHFL[2*j+1]);
		//左设计点
		fwscanf(fpr,L"%lf%lf%d ",&hdmdes[jcross].zlink->Zt,&hdmdes[jcross].zlink->ZHc,&hdmdes[jcross].zlink->Lnpt);//换行
		for(j=0; j<hdmdes[jcross].zlink->Lnpt; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&x,&y);
			pt.x = x, pt.y =y; pt.z =0;
			hdmdes[jcross].zlink->RPTL.append(pt);

			pt.y = hdmdes[jcross].zlink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].zlink->fwj - 0.5*PI);
			pt.x = hdmdes[jcross].zlink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].zlink->fwj - 0.5*PI);
			pt.z = hdmdes[jcross].zlink->newLYd + y;
			hdmdes[jcross].zlink->PTRPTL.append(pt);				
		}
		//左水沟点
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,hdmdes[jcross].zlink->LGouProperty,hdmdes[jcross].zlink->LGouPtNum,&hdmdes[jcross].zlink->LJSGouPtNum);
		for(j=0; j<hdmdes[jcross].zlink->LGouPtNum; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].zlink->Lgou[j*2],&hdmdes[jcross].zlink->Lgou[j*2+1]);

			x = hdmdes[jcross].zlink->Lgou[j*2];
			y = hdmdes[jcross].zlink->Lgou[j*2+1];		
			pt.y = hdmdes[jcross].zlink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].zlink->fwj - 0.5*PI);
			pt.x = hdmdes[jcross].zlink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].zlink->fwj - 0.5*PI);
			pt.z = hdmdes[jcross].zlink->newLYd + y;
			hdmdes[jcross].zlink->PTLgou.append(pt);
		}
		//左截水沟
		for(j=0;j<hdmdes[jcross].zlink->LJSGouPtNum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].zlink->LJSgou[j*2],&hdmdes[jcross].zlink->LJSgou[j*2+1]);

			x = hdmdes[jcross].zlink->LJSgou[j*2];
			y = hdmdes[jcross].zlink->LJSgou[j*2+1];	
			pt.y = hdmdes[jcross].zlink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].zlink->fwj - 0.5*PI);
			pt.x = hdmdes[jcross].zlink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].zlink->fwj - 0.5*PI);
			pt.z = hdmdes[jcross].zlink->newLYd + y;
			hdmdes[jcross].zlink->PTLJSgou.append(pt);
		}
		//2.写左线右侧
		//写路面点
		fwscanf(fpr,L"%d",&hdmdes[jcross].zlink->YDMXZ);
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&hdmdes[jcross].zlink->ylmhd[j]);

		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].zlink->RHFR[2*j],&hdmdes[jcross].zlink->RHFR[2*j+1]);
		//右设计点
		fwscanf(fpr,L"%lf%lf%d ",&hdmdes[jcross].zlink->Yt,&hdmdes[jcross].zlink->YHc,&hdmdes[jcross].zlink->Rnpt);
		for(j=0; j<hdmdes[jcross].zlink->Rnpt; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&x,&y);
			pt.x = x, pt.y =y; pt.z =0;
			hdmdes[jcross].zlink->RPTR.append(pt);

			pt.y = hdmdes[jcross].zlink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].zlink->fwj + 0.5*PI);
			pt.x = hdmdes[jcross].zlink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].zlink->fwj + 0.5*PI);
			pt.z = hdmdes[jcross].zlink->newRYd + y;
			hdmdes[jcross].zlink->PTRPTR.append(pt);
		}
		//右水沟点
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,hdmdes[jcross].zlink->RGouProperty,hdmdes[jcross].zlink->RGouPtNum,&hdmdes[jcross].zlink->RJSGouPtNum);
		for(j=0; j<hdmdes[jcross].zlink->RGouPtNum; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].zlink->Rgou[j*2],&hdmdes[jcross].zlink->Rgou[j*2+1]);

			x = hdmdes[jcross].zlink->Rgou[j*2];
			y = hdmdes[jcross].zlink->Rgou[j*2+1];		
			pt.y = hdmdes[jcross].zlink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].zlink->fwj + 0.5*PI);
			pt.x = hdmdes[jcross].zlink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].zlink->fwj + 0.5*PI);
			pt.z = hdmdes[jcross].zlink->newRYd + y;
			hdmdes[jcross].zlink->PTRgou.append(pt);			
		}
		//截水沟
		for(j=0;j<hdmdes[jcross].zlink->RJSGouPtNum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].zlink->RJSgou[j*2],&hdmdes[jcross].zlink->RJSgou[j*2+1]);

			x = hdmdes[jcross].zlink->RJSgou[j*2];
			y = hdmdes[jcross].zlink->RJSgou[j*2+1];	
			pt.y = hdmdes[jcross].zlink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].zlink->fwj + 0.5*PI);
			pt.x = hdmdes[jcross].zlink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].zlink->fwj + 0.5*PI);
			pt.z = hdmdes[jcross].zlink->newRYd + y;
			hdmdes[jcross].zlink->PTRJSgou.append(pt);
		}		
	}

	if(hdmdes[jcross].IfHasYlink )//右有关联道路
	{
		hdmdes[jcross].ylink = new CRSDAT;
		fwscanf(fpr ,L"%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
			&hdmdes[jcross].ylink->cml,
			&hdmdes[jcross].ylink->dml,
			&hdmdes[jcross].ylink->newLYd,
			&hdmdes[jcross].ylink->newRYd,
			&hdmdes[jcross].ylink->xjj,
			&FillArea,
			&CutArea,
			&hdmdes[jcross].ylink->Ldeltah,
			&hdmdes[jcross].ylink->Rdeltah,			
			tmp			
			);
		hdmdes[jcross].ylink->FLDname = tmp;
		for(j=0;j<hdm.YRoadSum;j++)//确定左关联道路序号,取平面
		{
			if(hdm.Yroad[j].roadname==hdmdes[jcross].ylink->FLDname)
				break;
		}
		if(j<hdm.YRoadSum)
		{
			YPZtmp.lc=hdmdes[jcross].ylink->cml;  // 统一里程
			hdm.Yroad[j].pm->xlpoint_pz(&YPZtmp);
			hdmdes[jcross].ylink->CenPt[0]=YPZtmp.x;
			hdmdes[jcross].ylink->CenPt[1]=YPZtmp.y;
			hdmdes[jcross].ylink->fwj = YPZtmp.a; 			
		}
		else
		{
			hdmdes[jcross].ylink->CenPt[0]=PZtmp.x;
			hdmdes[jcross].ylink->CenPt[1]=PZtmp.y;
			hdmdes[jcross].ylink->fwj = PZtmp.a; 
		}
		if(fabs(hdmdes[jcross].YDorS-2)<0.01)//双路面且同坡 取主线方位角
			hdmdes[jcross].ylink->fwj = PZtmp.a;
		//1.写左线左侧
		fwscanf(fpr,L"%d",&hdmdes[jcross].ylink->ZDMXZ);
		//写路面点
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&hdmdes[jcross].ylink->zlmhd[j]);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].ylink->RHFL[2*j],&hdmdes[jcross].ylink->RHFL[2*j+1]);
		//左设计点
		fwscanf(fpr,L"%lf%lf%d ",&hdmdes[jcross].ylink->Zt,&hdmdes[jcross].ylink->ZHc,&hdmdes[jcross].ylink->Lnpt);//换行
		for(j=0; j<hdmdes[jcross].ylink->Lnpt; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&x,&y);
			pt.x = x, pt.y =y; pt.z =0;
			hdmdes[jcross].ylink->RPTL.append(pt);

			pt.y = hdmdes[jcross].ylink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].ylink->fwj - 0.5*PI);
			pt.x = hdmdes[jcross].ylink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].ylink->fwj - 0.5*PI);
			pt.z = hdmdes[jcross].ylink->newLYd + y;
			hdmdes[jcross].ylink->PTRPTL.append(pt);				
		}
		//左水沟点
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,hdmdes[jcross].ylink->LGouProperty,hdmdes[jcross].ylink->LGouPtNum,&hdmdes[jcross].ylink->LJSGouPtNum);
		for(j=0; j<hdmdes[jcross].ylink->LGouPtNum; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].ylink->Lgou[j*2],&hdmdes[jcross].ylink->Lgou[j*2+1]);

			x = hdmdes[jcross].ylink->Lgou[j*2];
			y = hdmdes[jcross].ylink->Lgou[j*2+1];		
			pt.y = hdmdes[jcross].ylink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].ylink->fwj - 0.5*PI);
			pt.x = hdmdes[jcross].ylink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].ylink->fwj - 0.5*PI);
			pt.z = hdmdes[jcross].ylink->newLYd + y;
			hdmdes[jcross].ylink->PTLgou.append(pt);
		}
		//左截水沟
		for(j=0;j<hdmdes[jcross].ylink->LJSGouPtNum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].ylink->LJSgou[j*2],&hdmdes[jcross].ylink->LJSgou[j*2+1]);

			x = hdmdes[jcross].ylink->LJSgou[j*2];
			y = hdmdes[jcross].ylink->LJSgou[j*2+1];	
			pt.y = hdmdes[jcross].ylink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].ylink->fwj - 0.5*PI);
			pt.x = hdmdes[jcross].ylink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].ylink->fwj - 0.5*PI);
			pt.z = hdmdes[jcross].ylink->newLYd + y;
			hdmdes[jcross].ylink->PTLJSgou.append(pt);
		}
		//2.写左线右侧
		//写路面点
		fwscanf(fpr,L"%d",&hdmdes[jcross].ylink->YDMXZ);
		for(j=0;j<3;j++)
			fwscanf(fpr,L"%lf ",&hdmdes[jcross].ylink->ylmhd[j]);

		for(j=0;j<5;j++)
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].ylink->RHFR[2*j],&hdmdes[jcross].ylink->RHFR[2*j+1]);
		//右设计点
		fwscanf(fpr,L"%lf%lf%d ",&hdmdes[jcross].ylink->Yt,&hdmdes[jcross].ylink->YHc,&hdmdes[jcross].ylink->Rnpt);
		for(j=0; j<hdmdes[jcross].ylink->Rnpt; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&x,&y);
			pt.x = x, pt.y =y; pt.z =0;
			hdmdes[jcross].ylink->RPTR.append(pt);

			pt.y = hdmdes[jcross].ylink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].ylink->fwj + 0.5*PI);
			pt.x = hdmdes[jcross].ylink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].ylink->fwj + 0.5*PI);
			pt.z = hdmdes[jcross].ylink->newRYd + y;
			hdmdes[jcross].ylink->PTRPTR.append(pt);
		}
		//右水沟点
		fwscanf(fpr,L"%d%lf%d%d%d ",&itmp,&dtmp,hdmdes[jcross].ylink->RGouProperty,hdmdes[jcross].ylink->RGouPtNum,&hdmdes[jcross].ylink->RJSGouPtNum);
		for(j=0; j<hdmdes[jcross].ylink->RGouPtNum; j++)
		{
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].ylink->Rgou[j*2],&hdmdes[jcross].ylink->Rgou[j*2+1]);

			x = hdmdes[jcross].ylink->Rgou[j*2];
			y = hdmdes[jcross].ylink->Rgou[j*2+1];		
			pt.y = hdmdes[jcross].ylink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].ylink->fwj + 0.5*PI);
			pt.x = hdmdes[jcross].ylink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].ylink->fwj + 0.5*PI);
			pt.z = hdmdes[jcross].ylink->newRYd + y;
			hdmdes[jcross].ylink->PTRgou.append(pt);			
		}
		//右截水沟
		for(j=0;j<hdmdes[jcross].ylink->RJSGouPtNum;j++)
		{
			fwscanf(fpr,L"%lf%lf ",&hdmdes[jcross].ylink->RJSgou[j*2],&hdmdes[jcross].ylink->RJSgou[j*2+1]);

			x = hdmdes[jcross].ylink->RJSgou[j*2];
			y = hdmdes[jcross].ylink->RJSgou[j*2+1];	
			pt.y = hdmdes[jcross].ylink->CenPt[0] + fabs(x) * cos(hdmdes[jcross].ylink->fwj + 0.5*PI);
			pt.x = hdmdes[jcross].ylink->CenPt[1] + fabs(x) * sin(hdmdes[jcross].ylink->fwj + 0.5*PI);
			pt.z = hdmdes[jcross].ylink->newRYd + y;
			hdmdes[jcross].ylink->PTRJSgou.append(pt);
		}		
	}
}


void CDrawPM::DrawZTT()
{
	//如果当前道路为主线	
	CString RoadName = pm->RoadName;
	if(RoadName=="主线")
	{
		hdm.ReadBDFile();
		CString xl1;
		xl1.Format(L"%s,%s",pm->mdbname,pm->RoadName);		
		hdm.FindAndReadLinkRoad_for3d(xl1);
	}
	ReadHdmResultFile();

	DRAW_ALLGOU();

	draw_fill_cut();

}

void CDrawPM::FormHole()
{
	ifsgjk = false;
	ReadHdmResultFile();

	int i,j;
	AcGePoint3d spt,ept;
	NBriBj=0;
	xlpoint PZtmp;
	double OffsetByXLZX = 0.0;

	for(i=0;i<NCRS-1;i++)
	{
		if(hdmdes[i].ZDMXZ==3&&hdmdes[i+1].ZDMXZ==3&&(hdmdes[i+1].cml-hdmdes[i].cml)>0.01)//前后左端面为桥中
		{
			PZtmp.lc=hdmdes[i].cml;  // 统一里程
			pm->xlpoint_pz(&PZtmp);
			OffsetByXLZX = hdmdes[i].OffsetByXLZX;

			// 			 Abjx.qpt[0] = PZtmp.x + hdmdes[i].RHFL[8]*cos(PZtmp.a-0.5*PI);
			// 			 Abjx.qpt[1] = PZtmp.y + hdmdes[i].RHFL[8]*sin(PZtmp.a-0.5*PI);
			Abjx.qpt[0] = PZtmp.x + (-hdmdes[i].RHFL[8]+OffsetByXLZX)*cos(PZtmp.a+0.5*PI);
			Abjx.qpt[1] = PZtmp.y + (-hdmdes[i].RHFL[8]+OffsetByXLZX)*sin(PZtmp.a+0.5*PI);
			Abjx.qpt[2] = hdmdes[i].RHFL[9];
			PZtmp.lc=hdmdes[i+1].cml;  // 统一里程
			pm->xlpoint_pz(&PZtmp); 
			OffsetByXLZX = hdmdes[i+1].OffsetByXLZX;
			// 			 Abjx.hpt[0] = PZtmp.x + hdmdes[i+1].RHFL[8]*cos(PZtmp.a-0.5*PI);
			// 			 Abjx.hpt[1] = PZtmp.y + hdmdes[i+1].RHFL[8]*sin(PZtmp.a-0.5*PI);
			Abjx.hpt[0] = PZtmp.x + (-hdmdes[i+1].RHFL[8]+OffsetByXLZX)*cos(PZtmp.a+0.5*PI);
			Abjx.hpt[1] = PZtmp.y + (-hdmdes[i+1].RHFL[8]+OffsetByXLZX)*sin(PZtmp.a+0.5*PI);
			Abjx.hpt[2] = hdmdes[i+1].RHFL[9];
			spt.x = Abjx.qpt[1], spt.y = Abjx.qpt[0];
			ept.x = Abjx.hpt[1], ept.y = Abjx.hpt[0];
			//             makeline(spt,ept,1,0);
			BriBjArr.Add(Abjx);
			NBriBj++;
		}
	}
	for(i=0;i<NCRS-1;i++)
	{
		if(hdmdes[i].YDMXZ==3&&hdmdes[i+1].YDMXZ==3&&(hdmdes[i+1].cml-hdmdes[i].cml)>0.01)//前后右端面为桥中
		{
			PZtmp.lc=hdmdes[i].cml;  // 统一里程
			pm->xlpoint_pz(&PZtmp); 
			OffsetByXLZX = hdmdes[i].OffsetByXLZX;

			// 			Abjx.qpt[0] = PZtmp.x + hdmdes[i].RHFR[8]*cos(PZtmp.a+0.5*PI);
			// 			Abjx.qpt[1] = PZtmp.y + hdmdes[i].RHFR[8]*sin(PZtmp.a+0.5*PI);
			Abjx.qpt[0] = PZtmp.x + (hdmdes[i].RHFR[8]+OffsetByXLZX)*cos(PZtmp.a+0.5*PI);
			Abjx.qpt[1] = PZtmp.y + (hdmdes[i].RHFR[8]+OffsetByXLZX)*sin(PZtmp.a+0.5*PI);
			Abjx.qpt[2] = hdmdes[i].RHFR[9];
			PZtmp.lc=hdmdes[i+1].cml;  // 统一里程
			pm->xlpoint_pz(&PZtmp); 
			OffsetByXLZX = hdmdes[i+1].OffsetByXLZX;
			// 			Abjx.hpt[0] = PZtmp.x + hdmdes[i+1].RHFR[8]*cos(PZtmp.a+0.5*PI);
			// 			Abjx.hpt[1] = PZtmp.y + hdmdes[i+1].RHFR[8]*sin(PZtmp.a+0.5*PI);
			Abjx.hpt[0] = PZtmp.x + (hdmdes[i+1].RHFR[8]+OffsetByXLZX)*cos(PZtmp.a+0.5*PI);
			Abjx.hpt[1] = PZtmp.y + (hdmdes[i+1].RHFR[8]+OffsetByXLZX)*sin(PZtmp.a+0.5*PI);
			Abjx.hpt[2] = hdmdes[i+1].RHFR[9];
			spt.x = Abjx.qpt[1], spt.y = Abjx.qpt[0];
			ept.x = Abjx.hpt[1], ept.y = Abjx.hpt[0];
			//			makeline(spt,ept,1,0);
			BriBjArr.Add(Abjx);
			NBriBj++;
		}
	}

	HoleNum = 0;
	Ahole.nLpt=0;
	Ahole.nRpt=0;
	Ahole.qInpt[0]=-99;
	Ahole.qInpt[1]=-99;
	Count = 0;

	int sno,eno;
	//	if(_tcscmp(pm->RoadName,L"主线")==0)
	//	{
	sno=1,eno=NCRS;
	//	}
	//	else
	//	{
	//		sno=2,eno=NCRS-1;
	//	}

	//	for(i=1;i<NCRS;i++)
	for(i=sno;i<eno;i++)
	{		
		//绘左侧填方示坡线   
		//ads_printf(L"i=%d %s eno=%d\n",i,pm->RoadName,eno);
		if((hdmdes[i].cml - hdmdes[i-1].cml)>0.01)
		{
			formZorYHole(i,-1,hdmdes[i-1].Ldeltah,hdmdes[i].Ldeltah,hdmdes[i-1].Zt,hdmdes[i].Zt,hdmdes[i-1].ZDMXZ,
				hdmdes[i].ZDMXZ,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Lnpt,hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL,hdmdes[i-1].RHFL[1]);	
			//绘右侧挖方示坡线
			formZorYHole(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,
				hdmdes[i].YDMXZ,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Rnpt,hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,hdmdes[i-1].RHFR[1]);		   
		}
	}
	//计算洞的起终里程>0.001才做洞
	double zlen,ylen,zprjml1,zprjml2,yprjml1,yprjml2;
	zlen=ylen=0.0;
	if(Ahole.nLpt>0||Ahole.nRpt>0)
	{
		//去除一个断面的洞
		if(Ahole.nLpt>0)
		{
			zprjml1 = pm->PROJ_ML(Ahole.LptArr[0].y,Ahole.LptArr[0].x);
			zprjml2 = pm->PROJ_ML(Ahole.LptArr[Ahole.nLpt-1].y,Ahole.LptArr[Ahole.nLpt-1].x);
			zlen = zprjml2 - zprjml1;
		}

		if(Ahole.nRpt>0)
		{
			yprjml1 = pm->PROJ_ML(Ahole.RptArr[0].y,Ahole.RptArr[0].x);
			yprjml2 = pm->PROJ_ML(Ahole.RptArr[Ahole.nRpt-1].y,Ahole.RptArr[Ahole.nRpt-1].x);
			ylen = yprjml2 - yprjml1;

		}

		if(zlen>=0.001 && ylen>=0.001)
		{
			if((hdmdes[NCRS-1].ZDMXZ!=1&&hdmdes[NCRS-1].ZDMXZ!=2)||(hdmdes[NCRS-1].YDMXZ!=1&&hdmdes[NCRS-1].YDMXZ!=2))
			{
				HoleArray.Add(Ahole);
				HoleNum++;
			}
		}

		Ahole.nLpt=0;
		Ahole.nRpt=0;
		if(Ahole.LptArr.length() >0)
			Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
		if(Ahole.RptArr.length() >0)
			Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);
	}

	/*
	for(i=1;i<NCRS;i++)
	{		
	//绘左侧填方示坡线      
	if(fabs(hdmdes[i-1].cml-hdmdes[i].cml)>0.001)
	{
	formZPHole(i,-1,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Lnpt,hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL);	
	//绘右侧挖方示坡线
	formZPHole(i,1,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Rnpt,hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR);		   
	}
	}*/


	FILE *pmHolefpw;
	ACHAR PmHoleFname[256];
	_stprintf(PmHoleFname,L"%s\\data\\Hole.%s.%s", Cworkdir,pm->RoadName,Pro);

	pmHolefpw=_wfopen(PmHoleFname,L"w");

	if(pmHolefpw==NULL)
	{
		ads_printf(L"无法打开HOLE文件!");
		return;
	}
	fwprintf(pmHolefpw,L"%d\n",HoleNum);//洞总数

	AcGePoint3d pt;
	int ii;

	for(i=0;i<HoleNum;i++)
	{
		AcGePoint3dArray PtArr;

		for(ii=0;ii<HoleArray[i].nLpt;ii++)
		{
			pt.x = HoleArray[i].LptArr[ii].y;
			pt.y = HoleArray[i].LptArr[ii].x;
			pt.z = HoleArray[i].LptArr[ii].z;
			PtArr.append(pt);
		}
		for(ii=HoleArray[i].nRpt-1;ii>=0;ii--)
		{
			pt.x = HoleArray[i].RptArr[ii].y;
			pt.y = HoleArray[i].RptArr[ii].x;
			pt.z = HoleArray[i].RptArr[ii].z;
			PtArr.append(pt);
		}
		pt.x = HoleArray[i].LptArr[0].y; 
		pt.y = HoleArray[i].LptArr[0].x;
		pt.z = HoleArray[i].LptArr[0].z;
		PtArr.append(pt);


		if(HoleArray[i].nLpt>0&&HoleArray[i].nRpt>0)
		{
			spt.x = HoleArray[i].LptArr[0].x;
			spt.y = HoleArray[i].LptArr[0].y;
			ept.x = HoleArray[i].RptArr[0].x;
			ept.y = HoleArray[i].RptArr[0].y;
			makeline(spt,ept,1,0);
		}
		for(int j =1;j<HoleArray[i].nLpt;j++)
		{
			spt.x = HoleArray[i].LptArr[j-1].x;
			spt.y = HoleArray[i].LptArr[j-1].y;
			ept.x = HoleArray[i].LptArr[j].x;
			ept.y = HoleArray[i].LptArr[j].y;
			makeline(spt,ept,1,0);
		}	
		for(j =1 ;j<HoleArray[i].nRpt;j++)
		{
			spt.x = HoleArray[i].RptArr[j-1].x;
			spt.y = HoleArray[i].RptArr[j-1].y;
			ept.x = HoleArray[i].RptArr[j].x;
			ept.y = HoleArray[i].RptArr[j].y;
			makeline(spt,ept,1,0);
		}
		if(HoleArray[i].nLpt>0&&HoleArray[i].nRpt>0)
		{
			spt.x = HoleArray[i].LptArr[HoleArray[i].nLpt-1].x;
			spt.y = HoleArray[i].LptArr[HoleArray[i].nLpt-1].y;
			ept.x = HoleArray[i].RptArr[HoleArray[i].nRpt-1].x;
			ept.y = HoleArray[i].RptArr[HoleArray[i].nRpt-1].y;
			makeline(spt,ept,1,0);	
		}


		if(HoleArray[i].Inpt[0] <= 0.01)
		{
			HoleArray[i].Inpt[0] = 0;
			HoleArray[i].Inpt[1] = 0;
			for(j =0 ;j<HoleArray[i].nLpt;j++)
			{
				HoleArray[i].Inpt[0] += HoleArray[i].LptArr[j].y;
				HoleArray[i].Inpt[1] += HoleArray[i].LptArr[j].x;
			}
			for(j =0 ;j<HoleArray[i].nRpt;j++)
			{
				HoleArray[i].Inpt[0] += HoleArray[i].RptArr[j].y;
				HoleArray[i].Inpt[1] += HoleArray[i].RptArr[j].x;
			}
			HoleArray[i].Inpt[0]=HoleArray[i].Inpt[0]/(HoleArray[i].nLpt+HoleArray[i].nRpt);
			HoleArray[i].Inpt[1]=HoleArray[i].Inpt[1]/(HoleArray[i].nLpt+HoleArray[i].nRpt);
			//			ads_printf(L"mm=%lf %lf\n",HoleArray[i].Inpt[0],HoleArray[i].Inpt[1]);
		}

		int NUM=PtArr.length();
		AcGePoint2d TempPt;
		fwprintf(pmHolefpw,L"%d\n",NUM);
		HoleArray[i].pline=new AcDbPolyline(NUM);
		for(ii =0;ii<NUM;ii++)
		{
			TempPt.x = PtArr[ii].x;
			TempPt.y = PtArr[ii].y;
			HoleArray[i].pline->addVertexAt(ii,TempPt);
			HoleArray[i].pline->setWidthsAt(ii,0.0,0.0);
			//fwprintf(pmHolefpw,L"%lf,%lf,%lf\n",PtArr[ii].x,PtArr[ii].y,PtArr[ii].z);
			fwprintf(pmHolefpw, L"%lf %lf %lf\n", PtArr[ii].x, PtArr[ii].y, PtArr[ii].z);
		}
		PtArr.removeSubArray(0,PtArr.length()-1);
	}	
	fclose(pmHolefpw);
	ifsgjk = true;
}
//计算锥坡脚点
int CDrawPM::Cal_ZPJD(int ZorY,int HorT,AcGePoint3d Cenpt,AcGePoint3d Spt,AcGePoint3d Ept,AcGePoint3dArray &ptarr)//计算锥坡脚点
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
	/*
	p1.x = Ept.x;
	p1.y = Ept.y;
	p2dArr.append(p1);*/

	startPoint.x =Spt.x;
	startPoint.y =Spt.y;
	pnt.x = arcmid[X];
	pnt.y = arcmid[Y];
	endPoint.x = Ept.x;
	endPoint.y = Ept.y;

	AcGeCircArc2d parc(startPoint,pnt,endPoint);
	int intn;

	double sang,eang,R,ang;
	int intp;
	/*
	sang = parc.startAng();
	eang = parc.endAng();*/

	Cpt.x=parc.center().x;
	Cpt.y=parc.center().y;
	R = parc.radius();
	//    ads_printf(L"%lf %lf\n",sang,eang) ;
	/*
	p2.x = Cenpt.x;
	p2.y = Cenpt.y;
	len = Slen+Elen;
	class AcGeTol tol;
	tol.setEqualPoint(0.01);
	tol.setEqualVector(0.01);*/

	xyddaa(Cpt.x,Cpt.y,Ept.x,Ept.y,&sang);
	xyddaa(Cpt.x,Cpt.y,Spt.x,Spt.y,&eang);
	double angc;
	/*
	if(eang<sang)
	{
	tmp = eang;
	eang = sang;
	sang = tmp;
	}*/
	angc = eang-sang;
	if(angc<0)
		angc = 2*PI+angc;
	if(angc>PI)
		angc = 2*PI-angc;

	for(j=1; j<6; j++)//从横断面方向到线路前进方向(0.5*PI)范围内划分六个方向七个点
	{
		/*
		du= fwj + ZorY*(0.5*PI - j*PI/12.0);		
		pnt.x = Cenpt.x +  len*sin(du);
		pnt.y = Cenpt.y +  len*cos(du);
		AcGeLine2d line(p2,pnt);
		intn=0;
		parc.intersectWith(line,intn,p1,p2,tol);*/
		if(ZorY*HorT==1)
			ang = sang - j*angc/6.0;
		else
			ang = sang + j*angc/6.0;
		pt.x =  Cpt.x + R*cos(ang);
		pt.y =  Cpt.y + R*sin(ang);
		pt.z = Cenpt.z -20;

		/*
		p1.x = pt.x;
		p1.y = pt.y;*/

		ptarr.append(pt);
		//			p2dArr.append(p1);

	}

	ptarr.append(Spt);
	/*
	p1.x = Spt.x;
	p1.y = Spt.y;
	p2dArr.append(p1);
	AcDbObjectId id;
	makepolyline(p2dArr,id,1);*/

	return RTNORM;
}

void CDrawPM::formZPHole(int i,int ZorY,int qdmxz,int hdmxz,int qoppxz,int hoppxz,int qnpt,int hnpt,AcGePoint3dArray &qPtarr, AcGePoint3dArray &hPtarr)
{
	xlpoint  PZtmp; 
	int j;
	AcGePoint3d Pt;
	AcGePoint3d Cenpt;
	AcGePoint3dArray ptArr;	

	if(qdmxz==-1&&qdmxz==qoppxz)//前一断面为桥头 且左右断面性质一样 
	{
		if(i>1)//第一个断面不是桥头或隧道头
		{	
			if(ZorY==-1)//左侧
			{
				PZtmp.lc = hdmdes[i-1].cml;				
				pm->xlpoint_pz(&PZtmp);
				if(ptArr.length()>0)
					ptArr.removeSubArray(0,ptArr.length()-1);

				CalBriSlopPt(i-1,-1,ptArr);							
				for(j=0;j<7;j++)
				{
					Pt = ptArr[j];
					Ahole.LptArr.append(Pt);
					Ahole.nLpt++;
				}			
			}
			else//右侧
			{	
				PZtmp.lc = hdmdes[i-1].cml;
				pm->xlpoint_pz(&PZtmp);	
				if(ptArr.length()>0)
					ptArr.removeSubArray(0,ptArr.length()-1);
				CalBriSlopPt(i-1,1,ptArr);	
				for(j=0;j<7;j++)
				{
					Pt = ptArr[j];							
					Ahole.RptArr.append(Pt);
					Ahole.nRpt++;
				}
				HoleArray.Add(Ahole);
				HoleNum++;
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);					
			}		
		}
	}
	else if(qdmxz==-1&&fabs(1.0*qoppxz)>0)//前一断面为桥头 且对侧断面性质为桥中或隧中 一个洞的终止
	{
		if(i>1)//第一个断面不是桥头或隧道头
		{	
			if(ZorY==-1)//左侧
			{
				PZtmp.lc = hdmdes[i-1].cml;
				pm->xlpoint_pz(&PZtmp);
				if(ptArr.length()>0)
					ptArr.removeSubArray(0,ptArr.length()-1);
				CalBriSlopPt(i-1,-1,ptArr);		
				for(j=0;j<7;j++)
				{
					Pt = ptArr[j];
					Ahole.LptArr.append(Pt);
					Ahole.nLpt++;
				}			
				Ahole.RptArr.append(qPtarr[0]);							   
				Ahole.nRpt++;							
				HoleArray.Add(Ahole);
				HoleNum++;
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);
			}
			else//右侧
			{	
				PZtmp.lc = hdmdes[i-1].cml;
				pm->xlpoint_pz(&PZtmp);	
				if(ptArr.length()>0)
					ptArr.removeSubArray(0,ptArr.length()-1);
				CalBriSlopPt(i-1,1,ptArr);


				for(j=0;j<7;j++)
				{
					Pt = ptArr[j];							
					Ahole.RptArr.append(Pt);
					Ahole.nRpt++;
				}
				Ahole.LptArr.append(qPtarr[0]);							   
				Ahole.nLpt++;
				HoleArray.Add(Ahole);
				HoleNum++;
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);
			}
		}
	} 
	else if(qdmxz==-1&&qoppxz==0)//前一断面为桥头 且对侧断面性质为一般路基 
	{
		if(i>1)//第一个断面不是桥头或隧道头
		{	
			if(ZorY==-1)//左侧
			{
				PZtmp.lc = hdmdes[i-1].cml;
				pm->xlpoint_pz(&PZtmp);
				if(ptArr.length()>0)
					ptArr.removeSubArray(0,ptArr.length()-1);
				CalBriSlopPt(i-1,-1,ptArr);

				for(j=0;j<7;j++)
				{
					Pt = ptArr[j];
					Ahole.LptArr.append(Pt);
					Ahole.nLpt++;
				}
				Ahole.RptArr.append(qPtarr[0]);							   
				Ahole.nRpt++;							
				HoleArray.Add(Ahole);
				HoleNum++;
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);				
			}
			else//右侧
			{	

				PZtmp.lc = hdmdes[i-1].cml;
				pm->xlpoint_pz(&PZtmp);	
				if(ptArr.length()>0)
					ptArr.removeSubArray(0,ptArr.length()-1);
				CalBriSlopPt(i-1,1,ptArr);
				for(j=0;j<7;j++)
				{
					Pt = ptArr[j];							
					Ahole.RptArr.append(Pt);
					Ahole.nRpt++;
				}
				Ahole.LptArr.append(qPtarr[0]);							   
				Ahole.nLpt++;
				HoleArray.Add(Ahole);
				HoleNum++;
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);				
			}
		}
	}       	      			
	else if(hdmxz==1&&hdmxz==hoppxz)//前一断面为桥尾 且左右断面性质一样
	{
		if(i>1)//第一个断面不是桥尾或隧道尾
		{
			if(ZorY==-1)//左侧
			{
				PZtmp.lc = hdmdes[i].cml;
				pm->xlpoint_pz(&PZtmp);
				if(ptArr.length()>0)
					ptArr.removeSubArray(0,ptArr.length()-1);

				CalBriSlopPt(i,-1,ptArr);	
				for(j=6;j>=0;j--)
				{
					Pt = ptArr[j];														
					Ahole.LptArr.append(Pt);
					Ahole.nLpt++;
				}		
			}
			else//右侧
			{
				PZtmp.lc = hdmdes[i].cml;
				pm->xlpoint_pz(&PZtmp);
				if(ptArr.length()>0)
					ptArr.removeSubArray(0,ptArr.length()-1);

				CalBriSlopPt(i,1,ptArr);

				for(j=6;j>=0;j--)
				{
					Pt = ptArr[j];							
					Ahole.RptArr.append(Pt);
					Ahole.nRpt++;
				}
				HoleArray.Add(Ahole);
				HoleNum++;
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);	
			}
		}
	}			
}
void CDrawPM::formZorYHole(int i,int ZorY,double qdeltah,double hdeltah,double qtw,double htw,int qdmxz,int hdmxz,int qoppxz,int hoppxz,int qnpt,int hnpt,AcGePoint3dArray &qPtarr, AcGePoint3dArray &hPtarr,double Ht)
{
	xlpoint  PZtmp; 
	int j;
	AcGePoint3d Pt;
	AcGePoint3d Cenpt;
	double SlopDegree=1;
	AcGePoint3dArray ptArr;
	double zlen,ylen,zprjml1,zprjml2,yprjml1,yprjml2;


	double dx,dy,len;
	if(((qtw<0.001&&htw<0.001)||(qtw>0.001&&htw>0.001)) &&qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)
		//（前后断面都为填方或都为挖方）且前断面为一般断面
	{
		Count++;
		if(qnpt>5)
		{
			if(qdmxz!=-1&&qdmxz!=-2&&qdmxz!=1&&qdmxz!=2)
			{
				Ahole.Inpt[0]=qPtarr[3].y;
				Ahole.Inpt[1]=qPtarr[3].x;
			}
			if(Ahole.qInpt[0]<0.0&&qdmxz!=1&&qdmxz!=2&&Count>2)
			{
				Ahole.qInpt[0]=qPtarr[3].y;
				Ahole.qInpt[1]=qPtarr[3].x;
			}
		}
		if(i==1)//第一断面
		{
			if(ZorY==-1)//左侧
			{
				for(j=4;j<qnpt;j++)
				{
					if(j==4||j==qnpt-1)
					{
						Ahole.LptArr.append(qPtarr[j]);
						Ahole.nLpt++;
					}
					else if(Ahole.nLpt>0)
					{
						dx = qPtarr[j].x-Ahole.LptArr[Ahole.nLpt-1].x; 
						dy = qPtarr[j].y-Ahole.LptArr[Ahole.nLpt-1].y; 
						len = sqrt(dx*dx+dy*dy);
						if(len > 3)
						{
							Ahole.LptArr.append(qPtarr[j]);
							Ahole.nLpt++;
						}
					}
				}
			}
			else//右侧
			{
				for(j=4;j<qnpt;j++)
				{
					if(j==4||j==qnpt-1)
					{
						Ahole.RptArr.append(qPtarr[j]);
						Ahole.nRpt++;
					}
					else if(Ahole.nRpt>0)
					{
						dx = qPtarr[j].x-Ahole.RptArr[Ahole.nRpt-1].x; 
						dy = qPtarr[j].y-Ahole.RptArr[Ahole.nRpt-1].y; 
						len = sqrt(dx*dx+dy*dy);
						if(len > 3)
						{
							Ahole.RptArr.append(qPtarr[j]);
							Ahole.nRpt++;
						}
					}
				}
			}
		}
		else if(i==NCRS-1)//最后断面
		{
			if(ZorY==-1)//左侧
			{
				Ahole.LptArr.append(qPtarr[qnpt-1]);
				Ahole.nLpt++;
				for(j=hnpt-1;j>=4;j--)
				{
					if(j==hnpt-1||j==4)
					{
						Ahole.LptArr.append(hPtarr[j]);
						Ahole.nLpt++;
					}
					else if(Ahole.nLpt>0)
					{
						dx = hPtarr[j].x-Ahole.LptArr[Ahole.nLpt-1].x; 
						dy = hPtarr[j].y-Ahole.LptArr[Ahole.nLpt-1].y; 
						len = sqrt(dx*dx+dy*dy);
						if(len > 3)
						{
							Ahole.LptArr.append(hPtarr[j]);
							Ahole.nLpt++;
						}
					}
				}
			}
			else//右侧
			{
				Ahole.RptArr.append(qPtarr[qnpt-1]);
				Ahole.nRpt++;
				for(j=hnpt-1;j>=4;j--)
				{
					if(j==hnpt-1||j==4)
					{
						Ahole.RptArr.append(hPtarr[j]);
						Ahole.nRpt++;
					}
					else if(Ahole.nRpt>0)
					{
						dx = hPtarr[j].x-Ahole.RptArr[Ahole.nRpt-1].x; 
						dy = hPtarr[j].y-Ahole.RptArr[Ahole.nRpt-1].y; 
						len = sqrt(dx*dx+dy*dy);
						if(len > 3)
						{
							Ahole.RptArr.append(hPtarr[j]);
							Ahole.nRpt++;
						}
					}
				}
			}
		}
		else//中间
		{
			if(ZorY==-1)//左侧
			{
				Ahole.LptArr.append(qPtarr[qnpt-1]);
				Ahole.nLpt++;
			}
			else//右侧
			{
				Ahole.RptArr.append(qPtarr[qnpt-1]);
				Ahole.nRpt++;
			}
		}
	}
	else if(((qtw>0.001&&htw<0.001)||(qtw<0.001 && htw>0.001))&&qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)//挖-填或填-挖
	{
		Count++;
		if(qnpt>5)
		{
			if(qdmxz!=-1&&qdmxz!=-2&&qdmxz!=1&&qdmxz!=2)
			{
				Ahole.Inpt[0]=qPtarr[4].y;
				Ahole.Inpt[1]=qPtarr[4].x;
			}
			if(Ahole.qInpt[0]<0.0&&qdmxz!=1&&qdmxz!=2&&Count>2)
			{
				Ahole.qInpt[0]=qPtarr[4].y;
				Ahole.qInpt[1]=qPtarr[4].x;
			}
		}
		//求填挖零点				
		double MidMl;
		AcGePoint2d MidLJPt,MidPJPt;
		double k;
		CalTWZeroPt(hdmdes[i-1].cml,hdmdes[i].cml,qdeltah,hdeltah,k,MidMl);
		//			if(MidArr.length() >0)
		//				MidArr.removeSubArray(0, MidArr.length()-1);
		Pt.x = (hPtarr[4].x + k*qPtarr[4].x)/(1+k);
		Pt.y = (hPtarr[4].y + k*qPtarr[4].y)/(1+k);			
		Pt.z = (hPtarr[4].z + k*qPtarr[4].z)/(1+k);
		if(i==1)//第一断面
		{
			if(ZorY==-1)//左侧
			{
				for(j=4;j<qnpt;j++)
				{
					if(j==4||j==qnpt-1)
					{
						Ahole.LptArr.append(qPtarr[j]);
						Ahole.nLpt++;
					}
					else if(Ahole.nLpt>0)
					{
						dx = qPtarr[j].x-Ahole.LptArr[Ahole.nLpt-1].x; 
						dy = qPtarr[j].y-Ahole.LptArr[Ahole.nLpt-1].y; 
						len = sqrt(dx*dx+dy*dy);
						if(len > 3)
						{
							Ahole.LptArr.append(qPtarr[j]);
							Ahole.nLpt++;
						}
					}
				}
				Ahole.LptArr.append(Pt);
				Ahole.nLpt++;
			}
			else//右侧
			{
				for(j=4;j<qnpt;j++)
				{
					if(j==4||j==qnpt-1)
					{
						Ahole.RptArr.append(qPtarr[j]);
						Ahole.nRpt++;
					}
					else if(Ahole.nRpt>0)
					{
						dx = qPtarr[j].x-Ahole.RptArr[Ahole.nRpt-1].x; 
						dy = qPtarr[j].y-Ahole.RptArr[Ahole.nRpt-1].y; 
						len = sqrt(dx*dx+dy*dy);
						if(len > 3)
						{
							Ahole.RptArr.append(qPtarr[j]);
							Ahole.nRpt++;
						}
					}
				}
				Ahole.RptArr.append(Pt);
				Ahole.nRpt++;
			}
		}
		else if(i==NCRS-1 )//最后断面
		{
			if(ZorY==-1)//左侧
			{
				Ahole.LptArr.append(qPtarr[qnpt-1]);
				Ahole.nLpt++;
				Ahole.LptArr.append(Pt);
				Ahole.nLpt++;					
				for(j=hnpt-1;j>=4;j--)
				{
					if(j==hnpt-1||j==4)
					{
						Ahole.LptArr.append(hPtarr[j]);
						Ahole.nLpt++;
					}
					else
					{
						dx = hPtarr[j].x-Ahole.LptArr[Ahole.nLpt-1].x; 
						dy = hPtarr[j].y-Ahole.LptArr[Ahole.nLpt-1].y; 
						len = sqrt(dx*dx+dy*dy);
						if(len > 3)
						{
							Ahole.LptArr.append(hPtarr[j]);
							Ahole.nLpt++;
						}
					}
				}
			}
			else//右侧
			{
				Ahole.RptArr.append(qPtarr[qnpt-1]);
				Ahole.nRpt++;
				Ahole.RptArr.append(Pt);
				Ahole.nRpt++;
				for(j=hnpt-1;j>=4;j--)
				{
					if(j==hnpt-1||j==4)
					{
						Ahole.RptArr.append(hPtarr[j]);
						Ahole.nRpt++;
					}
					else
					{
						dx = hPtarr[j].x-Ahole.RptArr[Ahole.nRpt-1].x; 
						dy = hPtarr[j].y-Ahole.RptArr[Ahole.nRpt-1].y; 
						len = sqrt(dx*dx+dy*dy);
						if(len > 3)
						{
							Ahole.RptArr.append(hPtarr[j]);
							Ahole.nRpt++;
						}							
					}
				}
			}
		}
		else//中间
		{
			if(ZorY==-1)//左侧
			{
				Ahole.LptArr.append(qPtarr[qnpt-1]);
				Ahole.nLpt++;
				Ahole.LptArr.append(Pt);
				Ahole.nLpt++;			
			}
			else//右侧
			{
				Ahole.RptArr.append(qPtarr[qnpt-1]);
				Ahole.nRpt++;
				Ahole.RptArr.append(Pt);
				Ahole.nRpt++;
			}
		}
	}

	else if((qdmxz==-1||qdmxz==-2)&&(qdmxz==qoppxz))//前一断面为桥头或隧道头 且左右断面性质一样 一个洞的终止
	{
		if(i>1)//第一个断面不是桥头或隧道头
		{	
			if(ZorY==-1)//左侧
			{
				if(qdmxz==-2)//前一断面为隧道头
				{
					Ahole.LptArr.append(qPtarr[qnpt-1]);							   
					Ahole.nLpt++;
					// 					   if(hdmdes[i-1].Zt>0.1)//左挖方
					// 					   {
					// 						   PZtmp.lc = hdmdes[i-1].cml;				
					// 						   pm->xlpoint_pz(&PZtmp);
					// 						   double wid;
					// 						   wid = sqrt((qPtarr[qnpt-1].x-qPtarr[4].x)*(qPtarr[qnpt-1].x-qPtarr[4].x)
					// 							         +(qPtarr[qnpt-1].y-qPtarr[4].y)*(qPtarr[qnpt-1].y-qPtarr[4].y))-0.5;						   
					// 						   Pt.y = qPtarr[4].y+ wid*cos(PZtmp.a-0.5*PI);
					// 						   Pt.x = qPtarr[4].x+ wid*sin(PZtmp.a-0.5*PI);
					// 						   Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					// 					   }
					// 					   else
					{
						Pt.x = qPtarr[4].x;
						Pt.y = qPtarr[4].y;
						Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					Ahole.LptArr.append(Pt);						   
					Ahole.nLpt++;

				}
				else//前一断面为桥头
				{
					PZtmp.lc = hdmdes[i-1].cml;				
					pm->xlpoint_pz(&PZtmp);

					for(j=qnpt-1;j>=4;j--)
					{	
						if(j==4)
						{
							Pt = qPtarr[j];
							Pt.z -=20;
							Ahole.LptArr.append(Pt);
						}
						else
							Ahole.LptArr.append(qPtarr[j]);							   
						Ahole.nLpt++;
					}
				}
			}
			else//右侧
			{	
				if(qdmxz==-2)//前一断面为隧道头
				{
					Ahole.RptArr.append(qPtarr[qnpt-1]);							   
					Ahole.nRpt++;
					// 						if(hdmdes[i-1].Yt>0.1)//左挖方
					// 						{
					// 							PZtmp.lc = hdmdes[i-1].cml;				
					// 							pm->xlpoint_pz(&PZtmp);
					// 							double wid;
					// 							wid = sqrt((qPtarr[qnpt-1].x-qPtarr[4].x)*(qPtarr[qnpt-1].x-qPtarr[4].x)
					// 								+(qPtarr[qnpt-1].y-qPtarr[4].y)*(qPtarr[qnpt-1].y-qPtarr[4].y))-0.5;						   
					// 							Pt.y = qPtarr[4].y+ wid*cos(PZtmp.a+0.5*PI);
					// 							Pt.x = qPtarr[4].x+ wid*sin(PZtmp.a+0.5*PI);
					// 							Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					// 						}
					// 						else
					{
						Pt.x = qPtarr[4].x;
						Pt.y = qPtarr[4].y;
						Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					Ahole.RptArr.append(Pt);						   
					Ahole.nRpt++;
				}
				else//前一断面为桥头
				{
					PZtmp.lc = hdmdes[i-1].cml;
					pm->xlpoint_pz(&PZtmp);	

					for(j=qnpt-1;j>=4;j--)
					{
						if(j==4)
						{
							Pt = qPtarr[j];
							Pt.z -=20;
							Ahole.RptArr.append(Pt);
						}
						else
							Ahole.RptArr.append(qPtarr[j]);							   

						Ahole.nRpt++;
					}
				}
				if(i>=2&&(hdmdes[i-2].ZDMXZ==0||hdmdes[i-2].YDMXZ==0||fabs(hdmdes[i-2].cml-hdmdes[i-1].cml)>0.1))//当前为头 前一个不为尾					
				{
					zlen=ylen=0.0;
					if(Ahole.nLpt>0)
					{
						zprjml1 = pm->PROJ_ML(Ahole.LptArr[0].y,Ahole.LptArr[0].x);
						zprjml2 = pm->PROJ_ML(Ahole.LptArr[Ahole.nLpt-1].y,Ahole.LptArr[Ahole.nLpt-1].x);
						zlen = zprjml2 - zprjml1;
					}

					if(Ahole.nRpt>0)
					{
						yprjml1 = pm->PROJ_ML(Ahole.RptArr[0].y,Ahole.RptArr[0].x);
						yprjml2 = pm->PROJ_ML(Ahole.RptArr[Ahole.nRpt-1].y,Ahole.RptArr[Ahole.nRpt-1].x);
						ylen = yprjml2 - yprjml1;

					}
					if(zlen>=0.001 && ylen>=0.001)
					{
						HoleArray.Add(Ahole);
						HoleNum++;

					}
				}
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);
			}
		}
	}
	else if((qdmxz==-1||qdmxz==-2)&&(qoppxz==3||qoppxz==4))//前一断面为桥头或隧道头 且对侧断面性质为桥中或隧中 一个洞的终止
	{
		if(i>1)//第一个断面不是桥头或隧道头
		{	
			if(ZorY==-1)//左侧
			{
				if(qdmxz==-2)//前一断面为隧道头
				{
					Ahole.LptArr.append(qPtarr[qnpt-1]);							   
					Ahole.nLpt++;
					// 					   if(hdmdes[i-1].Zt>0.1)//左挖方
					// 					   {
					// 						   PZtmp.lc = hdmdes[i-1].cml;				
					// 						   pm->xlpoint_pz(&PZtmp);
					// 						   double wid;
					// 						   wid = sqrt((qPtarr[qnpt-1][0]-qPtarr[4][0])*(qPtarr[qnpt-1][0]-qPtarr[4][0])
					// 							   +(qPtarr[qnpt-1][1]-qPtarr[4][1])*(qPtarr[qnpt-1][1]-qPtarr[4][1]))-0.1;						   
					// 						   Pt.y = qPtarr[4].y+ wid*cos(PZtmp.a-0.5*PI);
					// 						   Pt.x = qPtarr[4].x+ wid*sin(PZtmp.a-0.5*PI);
					// 						   Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					// 					   }
					// 					   else
					{
						Pt.x = qPtarr[4][0];
						Pt.y = qPtarr[4][1];
						Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					Ahole.LptArr.append(Pt);						   
					Ahole.nLpt++;

					Pt.x = qPtarr[0][0];
					Pt.y = qPtarr[0][1];
					Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6; 
					Ahole.LptArr.append(Pt);						   
					Ahole.nLpt++;
				}
				else//前一断面为桥头
				{
					PZtmp.lc = hdmdes[i-1].cml;
					pm->xlpoint_pz(&PZtmp);

					for(j=qnpt-1;j>=4;j--)
					{	
						if(j==4)
						{
							Pt = qPtarr[j];
							Pt.z -=20;
							Ahole.LptArr.append(Pt);
						}
						else							
							Ahole.LptArr.append(qPtarr[j]);							   
						Ahole.nLpt++;
					}
					Pt.x = qPtarr[0][0];
					Pt.y = qPtarr[0][1];
					Pt.z = qPtarr[0].z-20; 
					Ahole.LptArr.append(Pt);						   
					Ahole.nLpt++;
				}

				if(i>=2&&(hdmdes[i-2].ZDMXZ==0||hdmdes[i-2].YDMXZ==0||fabs(hdmdes[i-2].cml-hdmdes[i-1].cml)>0.1))//当前为头 前一个不为尾
				{
					zlen=ylen=0.0;
					if(Ahole.nLpt>0)
					{
						zprjml1 = pm->PROJ_ML(Ahole.LptArr[0].y,Ahole.LptArr[0].x);
						zprjml2 = pm->PROJ_ML(Ahole.LptArr[Ahole.nLpt-1].y,Ahole.LptArr[Ahole.nLpt-1].x);
						zlen = zprjml2 - zprjml1;
					}

					if(Ahole.nRpt>0)
					{
						yprjml1 = pm->PROJ_ML(Ahole.RptArr[0].y,Ahole.RptArr[0].x);
						yprjml2 = pm->PROJ_ML(Ahole.RptArr[Ahole.nRpt-1].y,Ahole.RptArr[Ahole.nRpt-1].x);
						ylen = yprjml2 - yprjml1;

					}
					if(zlen>=0.001 && ylen>=0.001)
					{
						HoleArray.Add(Ahole);
						HoleNum++;
					}
				}
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);
			}
			else//右侧
			{	
				if(qdmxz==-2)//前一断面为隧道头
				{
					Ahole.RptArr.append(qPtarr[qnpt-1]);							   
					Ahole.nRpt++;
					// 						if(hdmdes[i-1].Yt>0.1)//右挖方
					// 						{
					// 							PZtmp.lc = hdmdes[i-1].cml;				
					// 							pm->xlpoint_pz(&PZtmp);
					// 							double wid;
					// 							wid = sqrt((qPtarr[qnpt-1][0]-qPtarr[4][0])*(qPtarr[qnpt-1][0]-qPtarr[4][0])
					// 								+(qPtarr[qnpt-1][1]-qPtarr[4][1])*(qPtarr[qnpt-1][1]-qPtarr[4][1]))-0.1;						   
					// 							Pt.y = qPtarr[4].y+ wid*cos(PZtmp.a+0.5*PI);
					// 							Pt.x = qPtarr[4].x+ wid*sin(PZtmp.a+0.5*PI);
					// 							Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					// 						}
					// 						else
					{
						Pt.x = qPtarr[4][0];
						Pt.y = qPtarr[4][1];
						Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					Ahole.RptArr.append(Pt);						   
					Ahole.nRpt++;

					Pt.x = qPtarr[0][0];
					Pt.y = qPtarr[0][1];
					Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6; 
					Ahole.RptArr.append(Pt);						   
					Ahole.nRpt++;
				}
				else//前一断面为桥头
				{
					PZtmp.lc = hdmdes[i-1].cml;
					pm->xlpoint_pz(&PZtmp);	

					for(j=qnpt-1;j>=4;j--)
					{	
						if(j==4)
						{
							Pt = qPtarr[j];
							Pt.z -=20;
							Ahole.RptArr.append(Pt);
						}
						else
							Ahole.RptArr.append(qPtarr[j]);							   
						Ahole.nRpt++;
					}
					Pt.x = qPtarr[0][0];
					Pt.y = qPtarr[0][1];
					Pt.z = qPtarr[0].z-20; 
					Ahole.RptArr.append(Pt);						   
					Ahole.nRpt++;
				}
				if(i>=2&&(hdmdes[i-2].ZDMXZ==0||hdmdes[i-2].YDMXZ==0/*||fabs(hdmdes[i-2].cml-hdmdes[i-1].cml)>0.5*/))//当前为头 前一个不为尾
				{
					zlen=ylen=0.0;
					if(Ahole.nLpt>0)
					{
						zprjml1 = pm->PROJ_ML(Ahole.LptArr[0].y,Ahole.LptArr[0].x);
						zprjml2 = pm->PROJ_ML(Ahole.LptArr[Ahole.nLpt-1].y,Ahole.LptArr[Ahole.nLpt-1].x);
						zlen = zprjml2 - zprjml1;
					}

					if(Ahole.nRpt>0)
					{
						yprjml1 = pm->PROJ_ML(Ahole.RptArr[0].y,Ahole.RptArr[0].x);
						yprjml2 = pm->PROJ_ML(Ahole.RptArr[Ahole.nRpt-1].y,Ahole.RptArr[Ahole.nRpt-1].x);
						ylen = yprjml2 - yprjml1;

					}
					if(zlen>=0.001 && ylen>=0.001)
					{
						HoleArray.Add(Ahole);
						HoleNum++;
					}
				}
				Ahole.nLpt=0;
				Ahole.nRpt=0;
				Ahole.Inpt[0]=-99;
				Ahole.Inpt[1]=-99;
				Ahole.qInpt[0]=-99;
				Ahole.qInpt[1]=-99;
				if(Ahole.LptArr.length() >0)
					Ahole.LptArr.removeSubArray(0, Ahole.LptArr.length()-1);
				if(Ahole.RptArr.length() >0)
					Ahole.RptArr.removeSubArray(0, Ahole.RptArr.length()-1);
			}
		}
	} 
	else if((qdmxz==-1||qdmxz==-2)&&(qoppxz==0||qoppxz==1||qoppxz==2))//前一断面为桥头或隧道头 且对侧断面性质为一般路基 或桥尾隧道尾
	{
		if(i>1)//第一个断面不是桥头或隧道头
		{	
			if(ZorY==-1)//左侧
			{
				if(qdmxz==-2)//前一断面为隧道头
				{
					Ahole.LptArr.append(qPtarr[qnpt-1]);							   
					Ahole.nLpt++;
					if(hdmdes[i-1].Zt>0.1)//左挖方
					{
						PZtmp.lc = hdmdes[i-1].cml;				
						pm->xlpoint_pz(&PZtmp);
						double wid;
						wid = sqrt((qPtarr[qnpt-1][0]-qPtarr[4][0])*(qPtarr[qnpt-1][0]-qPtarr[4][0])
							+(qPtarr[qnpt-1][1]-qPtarr[4][1])*(qPtarr[qnpt-1][1]-qPtarr[4][1]))-0.1;						   
						Pt.y = qPtarr[4].y+ wid*cos(PZtmp.a-0.5*PI);
						Pt.x = qPtarr[4].x+ wid*sin(PZtmp.a-0.5*PI);
						Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					else
					{
						Pt.x = qPtarr[4][0];
						Pt.y = qPtarr[4][1];
						Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					Ahole.LptArr.append(Pt);						   
					Ahole.nLpt++;

					Pt.x = qPtarr[0][0];
					Pt.y = qPtarr[0][1];
					Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					Ahole.LptArr.append(Pt);						   
					Ahole.nLpt++;
				}
				else//前一断面为桥头
				{
					PZtmp.lc = hdmdes[i-1].cml;
					pm->xlpoint_pz(&PZtmp);

					for(j=qnpt-1;j>=4;j--)
					{	
						if(j==4)
						{
							Pt = qPtarr[j];
							Pt.z -=20;
							Ahole.LptArr.append(Pt);
						}
						else
							Ahole.LptArr.append(qPtarr[j]);							   
						Ahole.nLpt++;
					}
					Pt.x = qPtarr[0][0];
					Pt.y = qPtarr[0][1];
					Pt.z = qPtarr[0].z-20;
					Ahole.LptArr.append(Pt);						   
					Ahole.nLpt++;
				}
			}
			else//右侧
			{	
				if(qdmxz==-2)//前一断面为隧道头
				{
					Ahole.RptArr.append(qPtarr[qnpt-1]);							   
					Ahole.nRpt++;
					if(hdmdes[i-1].Yt>0.1)//右挖方
					{
						PZtmp.lc = hdmdes[i-1].cml;				
						pm->xlpoint_pz(&PZtmp);
						double wid;
						wid = sqrt((qPtarr[qnpt-1][0]-qPtarr[4][0])*(qPtarr[qnpt-1][0]-qPtarr[4][0])
							+(qPtarr[qnpt-1][1]-qPtarr[4][1])*(qPtarr[qnpt-1][1]-qPtarr[4][1]))-0.1;						   
						Pt.y = qPtarr[4].y+ wid*cos(PZtmp.a+0.5*PI);
						Pt.x = qPtarr[4].x+ wid*sin(PZtmp.a+0.5*PI);
						Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					else
					{
						Pt.x = qPtarr[4][0];
						Pt.y = qPtarr[4][1];
						Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					Ahole.RptArr.append(Pt);						   
					Ahole.nRpt++;

					Pt.x = qPtarr[0][0];
					Pt.y = qPtarr[0][1];
					Pt.z = qPtarr[0].z+5.4+2.2+1.5+0.6;
					Ahole.RptArr.append(Pt);						   
					Ahole.nRpt++;
				}
				else//前一断面为桥头
				{
					PZtmp.lc = hdmdes[i-1].cml;
					pm->xlpoint_pz(&PZtmp);	

					for(j=qnpt-1;j>=4;j--)
					{	
						if(j==4)
						{
							Pt = qPtarr[j];
							Pt.z -=20;
							Ahole.RptArr.append(Pt);
						}
						else							
							Ahole.RptArr.append(qPtarr[j]);							   
						Ahole.nRpt++;
					}
					Pt.x = qPtarr[0][0];
					Pt.y = qPtarr[0][1];
					Pt.z = qPtarr[0].z-20;
					Ahole.RptArr.append(Pt);						   
					Ahole.nRpt++;
				}				
			}
		}
	}       	      			
	else if((hdmxz==1||hdmxz==2)/*&&(fabs(hoppxz)>0*/)//后一断面为桥尾或隧道尾 一个洞的开始
	{
		if(i>1 && i<NCRS-1)//第一个断面不是桥尾或隧道尾
		{				
			if(ZorY==-1)//左侧
			{
				if(hdmxz==2)//后一断面为隧道尾
				{
					if(qdmxz==2)//前断面为隧道尾
						return;
					if(hoppxz!=2)//对侧不为隧道尾
					{
						Pt = hPtarr[0];	
						Pt.z = Pt.z+5.4+2.2+1.5+0.6;
						Ahole.LptArr.append(Pt);						   
						Ahole.nLpt++;						   
					}
					if(hdmdes[i].Zt>0.1)//左挖方
					{
						PZtmp.lc = hdmdes[i].cml;				
						pm->xlpoint_pz(&PZtmp);
						double wid;
						wid = sqrt((hPtarr[hnpt-1][0]-hPtarr[4][0])*(hPtarr[hnpt-1][0]-hPtarr[4][0])
							+(hPtarr[hnpt-1][1]-hPtarr[4][1])*(hPtarr[hnpt-1][1]-hPtarr[4][1]))-0.1;						   
						Pt.y = hPtarr[4].y+ wid*cos(PZtmp.a-0.5*PI);
						Pt.x = hPtarr[4].x+ wid*sin(PZtmp.a-0.5*PI);
						Pt.z = hPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					else
					{
						Pt.x = hPtarr[4][0];
						Pt.y = hPtarr[4][1];
						Pt.z = hPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					Ahole.LptArr.append(Pt);						   
					Ahole.nLpt++;

					Ahole.LptArr.append(hPtarr[hnpt-1]);							   
					Ahole.nLpt++;
				}
				else//后一断面为桥尾
				{
					if(qdmxz==1)//前断面为桥尾
						return;
					if(hoppxz!=1)//对侧不为桥尾
					{
						Pt = hPtarr[0];	
						Pt.z = Pt.z-20;
						Ahole.LptArr.append(Pt);						   
						Ahole.nLpt++;						   
					}
					for(j=4;j<hnpt;j++)
					{	
						if(j==4)
						{
							Pt = hPtarr[j];
							Pt.z -=20;
							Ahole.LptArr.append(Pt);
						}
						else							
							Ahole.LptArr.append(hPtarr[j]);							   
						Ahole.nLpt++;
					}
				}
			}
			else//右侧
			{
				if(hdmxz==2)//后一断面为隧道尾
				{
					if(qdmxz==2)//前断面为隧道尾
						return;
					if(hoppxz!=2)//对侧不为隧道尾
					{
						Pt = hPtarr[0];	
						Pt.z = Pt.z+5.4+2.2+1.5+0.6;
						Ahole.RptArr.append(Pt);						   
						Ahole.nRpt++;						   
					}
					if(hdmdes[i].Yt>0.1)//左挖方
					{
						PZtmp.lc = hdmdes[i].cml;				
						pm->xlpoint_pz(&PZtmp);
						double wid;
						wid = sqrt((hPtarr[hnpt-1][0]-hPtarr[4][0])*(hPtarr[hnpt-1][0]-hPtarr[4][0])
							+(hPtarr[hnpt-1][1]-hPtarr[4][1])*(hPtarr[hnpt-1][1]-hPtarr[4][1]))-0.1;						   
						Pt.y = hPtarr[4].y+ wid*cos(PZtmp.a+0.5*PI);
						Pt.x = hPtarr[4].x+ wid*sin(PZtmp.a+0.5*PI);
						Pt.z = hPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					else
					{
						Pt.x = hPtarr[4][0];
						Pt.y = hPtarr[4][1];
						Pt.z = hPtarr[0].z+5.4+2.2+1.5+0.6;
					}
					Ahole.RptArr.append(Pt);						   
					Ahole.nRpt++;

					Ahole.RptArr.append(hPtarr[hnpt-1]);							   
					Ahole.nRpt++;
				}
				else//后一断面为桥尾
				{
					if(qdmxz==1)//前断面为桥尾
						return;
					if(hoppxz!=1)//对侧不为桥尾
					{
						Pt = hPtarr[0];	
						Pt.z = Pt.z-20;
						Ahole.RptArr.append(Pt);						   
						Ahole.nRpt++;						   
					}
					for(j=4;j<hnpt;j++)
					{	
						if(j==4)
						{
							Pt = hPtarr[j];
							Pt.z -=20;
							Ahole.RptArr.append(Pt);
						}
						else							
							Ahole.RptArr.append(hPtarr[j]);							   
						Ahole.nRpt++;
					}
				}
			}
		}
	}		

	else if(qnpt==0&&qoppxz==0)//前一断面为为半幅桥或者半幅隧道
	{
		PZtmp.lc = hdmdes[i-1].cml;
		pm->xlpoint_pz(&PZtmp);
		double OffsetByXLZX = hdmdes[i-1].OffsetByXLZX;
		Pt.y = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
		Pt.x = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
		if(ZorY==-1)//左侧
		{		
			if(hdmdes[i-1].ZDMXZ==3)
				Pt.z =  Ht;	
			else
				Pt.z =  Ht+5.4+2.2+1.5+0.6;	
			Ahole.LptArr.append(Pt);
			Ahole.nLpt++;
		}
		else//右侧
		{
			if(hdmdes[i-1].YDMXZ==3)
				Pt.z =  Ht;	
			else
				Pt.z =  Ht+5.4+2.2+1.5+0.6;	
			Ahole.RptArr.append(Pt);
			Ahole.nRpt++;
		}             
	}
	return ;
}

void CDrawPM::DrawDZQ()//绘制大中桥
{
	double BriSml,BriEml,lfwj,rfwj,fwj,angle,cml,Zlmk=4,Ylmk=4,wlen,wlen1,width,cita;
	xlpoint  PZtmp; 
	xlpoint PZLJZX;//路基中线点
	AcGePoint3d spt,ept;
	ads_point bpt,SLpt,Lpt1,SRpt,Rpt1,ELpt,ERpt,pt1,Lpt,pt;
	ACHAR str[256],ckml[80];
	CString Kml;
	double ptt[2],prjml;
	double OffsetByXLZX = 0.0;

	ads_printf(L"正在绘大中桥.......\n");

	DBS.Read_XLDbs(pm->mdbname,L"大中桥",pm->RoadName);	
	for(int i=0;i<DBS.BriNum;i++)
	{
		BriSml = pm->TYLC(DBS.Bri[i].StDml);//起点里程
		BriEml = pm->TYLC(DBS.Bri[i].EdDml);//起点里程
		if(BriSml < Scml-0.0001 || BriEml > Ecml+0.0001)
		{
			continue;
		}
		ads_printf(L"绘%s......\n",DBS.Bri[i].BriName);
		_tcscpy(str,DBS.Bri[i].KKSY);
		// wlen = _tcslen(str)*2.0;		 
		wlen = _tcslen(str)*texth*0.8;	


		Zlmk = GetLMK(BriSml,ZLMK,NZLMK);
		if(DBS.Bri[i].XZ=="右幅")
			Zlmk = 0.0;
		Kml = pm->XLC(0.5*(BriSml+BriEml));
		//		 _tcscpy(ckml,Kml);
		_stprintf(ckml,L"%s%s",Kml,DBS.Bri[i].BriName);
		wlen1 = _tcslen(ckml)*texth*0.8;	
		wlen = wlen1 > wlen ? wlen1 : wlen;
		PZtmp.lc = 0.5*(BriSml+BriEml);
		pm->xlpoint_pz(&PZtmp);
		CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
		PZLJZX.x = PZtmp.x + OffsetByXLZX*cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX*sin(PZtmp.a+0.5*pi);
		cita = PZtmp.a;
		fwj = PZtmp.a-PI/2.0;
		if(fwj < 0.0)
			fwj = fwj + 2.0*PI;
		//       pt[X]=PZtmp.x; pt[Y]=PZtmp.y;
		pt[X]=PZLJZX.x; pt[Y]=PZLJZX.y;
		Lpt[X] = pt[X] + Zlmk*cos(fwj);
		Lpt[Y] = pt[Y] + Zlmk*sin(fwj);
		width = caldistance(pt,Lpt)+40;		 
		pt1[X] = pt[X] + width*cos(fwj);
		pt1[Y] = pt[Y] + width*sin(fwj);
		Lpt[X] = pt[X] + (width+2+0.5*texth)*cos(fwj);
		Lpt[Y] = pt[Y] + (width+2+0.5*texth)*sin(fwj);
		Lpt[X] = Lpt[X] + wlen*cos(cita)/2.0;
		Lpt[Y] = Lpt[Y] + wlen*sin(cita)/2.0;
		Lpt1[X] = pt[X] +(width+4+1.5*texth)*cos(fwj);
		Lpt1[Y] = pt[Y] +(width+4+1.5*texth)*sin(fwj);
		Lpt1[X] = Lpt1[X] +wlen*cos(cita)/2.0;
		Lpt1[Y] = Lpt1[Y] + wlen*sin(cita)/2.0;
		bpt[X] = pt[Y];
		bpt[Y] = pt[X];
		pt[X] = pt1[X] + wlen*cos(cita);
		pt[Y] = pt1[Y] + wlen*sin(cita);
		lfwj = pt1[Y];
		pt1[Y] = pt1[X];
		pt1[X] = lfwj;
		lfwj = pt[Y];
		pt[Y] = pt[X];
		pt[X] = lfwj;
		line0(bpt[X],bpt[Y],pt1[X],pt1[Y],4,L"桥梁");
		line0(pt1[X],pt1[Y],pt[X],pt[Y],4,L"桥梁");
		lfwj = Lpt[Y];
		Lpt[Y] = Lpt[X];
		Lpt[X] = lfwj;
		lfwj = Lpt1[Y];
		Lpt1[Y] = Lpt1[X];
		Lpt1[X] = lfwj;
		angle=ads_angle(pt1 , pt);
		//		 angle = angle*180.0/PI;
		spt.x = Lpt1[X],spt.y=Lpt1[Y];
		//		 maketext(spt,ckml,angle,texth,4,3,L"HZ",L"桥梁");
		maketext(spt,ckml,angle,texth,4,3,L"HZ",L"桥梁");
		spt.x = Lpt[X],spt.y=Lpt[Y];
		//		 maketext(spt,str,angle,texth,4,3,L"HZ",L"桥梁");
		maketext(spt,str,angle,texth,4,3,L"HZ",L"桥梁");
		//		 my_texts ("m", Lpt1[X], Lpt1[Y], 2.0, angle, kml);
		//		 my_texts ("m", Lpt[X], Lpt[Y], 2.0, angle, str);
		PZtmp.lc = BriSml;
		pm->xlpoint_pz(&PZtmp);
		CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
		PZLJZX.x = PZtmp.x + OffsetByXLZX*cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX*sin(PZtmp.a+0.5*pi);

		//		 CAL_XY(cml-dlen/2.0,&pt[X],&pt[Y],&cita);
		rfwj = PZtmp.a + DBS.Bri[i].HeadAng*PI/180;
		if(rfwj > 2.0*PI)
			rfwj = rfwj - 2.0*PI;
		lfwj = rfwj - PI;
		if(lfwj < 0)
			lfwj = lfwj + 2.0*PI;	
		//		 bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
		bpt[X] = PZLJZX.y; bpt[Y] = PZLJZX.x;
		SLpt[X] = bpt[X] + Zlmk*sin(lfwj);
		SLpt[Y] = bpt[Y] + Zlmk*cos(lfwj);
		if(fabs(DBS.Bri[i].HeadAng-90) >0.1)
		{
			pm->CalPZXPT(BriSml,DBS.Bri[i].HeadAng+180,Zlmk-OffsetByXLZX,ptt,prjml);
			SLpt[X] = ptt[1];
			SLpt[Y] = ptt[0];
		}

		angle = ads_angle(bpt,SLpt);
		ads_polar(SLpt,angle+PI/4.0,3.0,Lpt1);
		//		 line0(SLpt[X],SLpt[Y],Lpt1[X],Lpt1[Y]);
		spt.x = SLpt[X];spt.y = SLpt[Y];
		ept.x = Lpt1[X];ept.y = Lpt1[Y];
		makeline(spt,ept,4,0,L"桥梁");
		Ylmk = GetLMK(BriSml,YLMK,NYLMK);
		if(DBS.Bri[i].XZ=="左幅")
			Ylmk = 0.0;
		SRpt[X] = bpt[X] + Ylmk*sin(rfwj);
		SRpt[Y] = bpt[Y] + Ylmk*cos(rfwj);

		if(fabs(DBS.Bri[i].HeadAng-90) >0.1)
		{
			pm->CalPZXPT(BriSml,DBS.Bri[i].HeadAng,Ylmk+OffsetByXLZX,ptt,prjml);
			SRpt[X] = ptt[1];
			SRpt[Y] = ptt[0];
		}
		angle = ads_angle(bpt,SRpt);
		ads_polar(SRpt,angle-PI/4.0,3.0,Rpt1);
		spt.x = SRpt[X];spt.y = SRpt[Y];
		ept.x = Rpt1[X];ept.y = Rpt1[Y];
		makeline(spt,ept,4,0,L"桥梁");
		spt.x = SLpt[X],spt.y = SLpt[Y];
		ept.x = SRpt[X],ept.y = SRpt[Y];
		makeline(spt,ept,4,0,L"桥梁");

		Zlmk = GetLMK(BriEml,ZLMK,NZLMK);
		if(DBS.Bri[i].XZ=="右幅")
			Zlmk = 0.0;
		PZtmp.lc = BriEml;
		pm->xlpoint_pz(&PZtmp);
		CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
		rfwj = PZtmp.a + DBS.Bri[i].TailAng*PI/180;
		if(rfwj > 2.0*PI)
			rfwj = rfwj - 2.0*PI;
		lfwj = rfwj - PI;
		if(lfwj < 0)
			lfwj = lfwj + 2.0*PI;

		//		 bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
		bpt[X] = PZLJZX.y; bpt[Y] = PZLJZX.x;
		ELpt[X] = bpt[X] + Zlmk*sin(lfwj);
		ELpt[Y] = bpt[Y] + Zlmk*cos(lfwj);
		if(fabs(DBS.Bri[i].TailAng-90) >0.1)
		{
			pm->CalPZXPT(BriEml,DBS.Bri[i].TailAng+180,Zlmk - OffsetByXLZX,ptt,prjml);
			ELpt[X] = ptt[1];
			ELpt[Y] = ptt[0];
		}

		angle = ads_angle(bpt,ELpt);
		ads_polar(ELpt,angle-PI/4.0,3.0,Lpt1);

		spt.x = ELpt[X];spt.y = ELpt[Y];
		ept.x = Lpt1[X];ept.y = Lpt1[Y];
		makeline(spt,ept,4,0,L"桥梁");
		Ylmk = GetLMK(BriEml,YLMK,NYLMK);
		if(DBS.Bri[i].XZ=="左幅")
			Ylmk = 0.0;
		ERpt[X] = bpt[X] + Ylmk*sin(rfwj);
		ERpt[Y] = bpt[Y] + Ylmk*cos(rfwj);
		if(fabs(DBS.Bri[i].TailAng-90) >0.1)
		{
			pm->CalPZXPT(BriEml,DBS.Bri[i].TailAng,Ylmk + OffsetByXLZX,ptt,prjml);
			ERpt[X] = ptt[1];
			ERpt[Y] = ptt[0];
		}

		angle = ads_angle(bpt,ERpt);
		ads_polar(ERpt,angle+PI/4.0,3.0,Rpt1);
		spt.x = ERpt[X];spt.y = ERpt[Y];
		ept.x = Rpt1[X];ept.y = Rpt1[Y];
		makeline(spt,ept,4,0,L"桥梁");
		spt.x = ELpt[X],spt.y = ELpt[Y];
		ept.x = ERpt[X],ept.y = ERpt[Y];
		makeline(spt,ept,4,0,L"桥梁");		 

		AcGePoint2dArray Pt2dAy,Pt2dAy1;
		Pt2dAy.append(AcGePoint2d(SLpt[X],SLpt[Y]));
		cml  = BriSml+10; 
		while(cml<BriEml-Zlmk/tan(DBS.Bri[i].TailAng*PI/180))
		{
			PZtmp.lc = cml;
			pm->xlpoint_pz(&PZtmp);
			CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
			PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
			PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
			Zlmk = GetLMK(cml,ZLMK,NZLMK);
			if(DBS.Bri[i].XZ=="右幅")
				Zlmk = 0.0;
			lfwj = PZtmp.a - PI/2.0;
			if(lfwj < 0.0)
				lfwj = lfwj + 2.0*PI;
			//			 bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
			bpt[X] = PZLJZX.y; bpt[Y] = PZLJZX.x;
			SLpt[X] = bpt[X] + Zlmk*sin(lfwj);
			SLpt[Y] = bpt[Y] + Zlmk*cos(lfwj);
			Pt2dAy.append(AcGePoint2d(SLpt[X],SLpt[Y]));			 
			cml += 10.0;
		}		 
		Pt2dAy.append(AcGePoint2d(ELpt[X],ELpt[Y]));
		makepolyline(Pt2dAy,4,0.0,L"桥梁");	
		//		 acedCommandS(RTPOINT,ELpt,RTSTR,L"",RTNONE);

		//		 acedCommandS(RTSTR,L"Pline",RTPOINT,SRpt,RTNONE);
		Pt2dAy1.append(AcGePoint2d(SRpt[X],SRpt[Y]));
		cml  = BriSml+10; 
		while(cml<BriEml-Ylmk/tan(DBS.Bri[i].TailAng*PI/180))
		{
			PZtmp.lc = cml;
			pm->xlpoint_pz(&PZtmp);
			CalOffsetByXLZX(PZtmp.lc,NYLMK,YLMK,OffsetByXLZX);
			PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
			PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
			Ylmk = GetLMK(cml,YLMK,NYLMK);
			if(DBS.Bri[i].XZ=="左幅")
				Ylmk = 0.0;
			//			 CAL_XY(cml,&pt[X],&pt[Y],&cita);
			rfwj = PZtmp.a + PI/2.0;
			if(rfwj > 2.0*PI)
				rfwj = rfwj - 2.0*PI;
			//			 bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
			bpt[X] = PZLJZX.y; bpt[Y] = PZLJZX.x;
			SRpt[X] = bpt[X] + Ylmk*sin(rfwj);
			SRpt[Y] = bpt[Y] + Ylmk*cos(rfwj);
			Pt2dAy1.append(AcGePoint2d(SRpt[X],SRpt[Y]));
			cml += 10.0;
		}
		Pt2dAy1.append(AcGePoint2d(ERpt[X],ERpt[Y]));
		makepolyline(Pt2dAy1,4,0.0,L"桥梁");	
	}
	ads_printf(L"OK\n");
}

void CDrawPM::DrawTun()//绘制隧道
{
	double TunSml,TunEml,lfwj,rfwj,fwj,angle,cml,Zlmk=4,Ylmk=4,wlen,width,cita;
	xlpoint  PZtmp; 
	AcGePoint3d spt,ept;
	ads_point bpt,SLpt,Lpt1,SRpt,Rpt1,ELpt,ERpt,pt1,Lpt,pt;
	ACHAR str[256],ckml[80];
	CString Kml;
	xlpoint PZLJZX;
	double OffsetByXLZX = 0.0;
	ads_printf(L"正在绘隧道.......\n");
	DBS.Read_XLDbs(pm->mdbname ,L"隧道",pm->RoadName);	
	for(int i=0;i<DBS.TunNum;i++)
	{
		TunSml = pm->TYLC(DBS.Tun[i].StDml);//起点里程
		TunEml = pm->TYLC(DBS.Tun[i].EdDml);//起点里程
		if(TunSml < Scml-0.0001 || TunEml > Ecml+0.0001)
			continue;
		ads_printf(L"绘%s......\n",DBS.Tun[i].TunName);
		_tcscpy(str,DBS.Tun[i].TunName);
		// wlen = _tcslen(str)*2.0;		 
		wlen = _tcslen(str)*texth*0.8;		 
		Zlmk = GetLMK(TunSml,ZLMK,NZLMK);
		if(DBS.Tun[i].XZ=="右幅")
			Zlmk = 0.0;
		Kml = pm->XLC(0.5*(TunSml+TunEml));
		_tcscpy(ckml,Kml);
		PZtmp.lc = 0.5*(TunSml+TunEml);
		pm->xlpoint_pz(&PZtmp);
		CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
		PZLJZX = PZtmp;
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a + 0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a + 0.5*pi);
		cita = PZtmp.a;
		fwj = PZtmp.a-PI/2.0;
		if(fwj < 0.0)
			fwj = fwj + 2.0*PI;
		//       pt[X]=PZtmp.x; pt[Y]=PZtmp.y;
		pt[X] = PZLJZX.x; pt[Y] = PZLJZX.y;
		Lpt[X] = pt[X] + Zlmk*cos(fwj);
		Lpt[Y] = pt[Y] + Zlmk*sin(fwj);		 
		width = caldistance(pt,Lpt)+40;
		pt1[X] = pt[X] + width*cos(fwj);
		pt1[Y] = pt[Y] + width*sin(fwj);
		Lpt[X] = pt[X] + (width+2+0.5*texth)*cos(fwj);
		Lpt[Y] = pt[Y] + (width+2+0.5*texth)*sin(fwj);
		Lpt[X] = Lpt[X] + wlen*cos(cita)/2.0;
		Lpt[Y] = Lpt[Y] + wlen*sin(cita)/2.0;
		Lpt1[X] = pt[X] +(width+4+1.5*texth)*cos(fwj);
		Lpt1[Y] = pt[Y] +(width+4+1.5*texth)*sin(fwj);
		Lpt1[X] = Lpt1[X] +wlen*cos(cita)/2.0;
		Lpt1[Y] = Lpt1[Y] + wlen*sin(cita)/2.0;
		bpt[X] = pt[Y];
		bpt[Y] = pt[X];
		pt[X] = pt1[X] + wlen*cos(cita);
		pt[Y] = pt1[Y] + wlen*sin(cita);
		lfwj = pt1[Y];
		pt1[Y] = pt1[X];
		pt1[X] = lfwj;
		lfwj = pt[Y];
		pt[Y] = pt[X];
		pt[X] = lfwj;
		line0(bpt[X],bpt[Y],pt1[X],pt1[Y],4,L"隧道");
		line0(pt1[X],pt1[Y],pt[X],pt[Y],4,L"隧道");

		lfwj = Lpt[Y];
		Lpt[Y] = Lpt[X];
		Lpt[X] = lfwj;
		lfwj = Lpt1[Y];
		Lpt1[Y] = Lpt1[X];
		Lpt1[X] = lfwj;
		angle=ads_angle(pt1 , pt);
		//		 angle = angle*180.0/PI;
		spt.x = Lpt1[X],spt.y=Lpt1[Y];
		//		 maketext(spt,ckml,angle,texth,4,3,L"HZ",L"隧道");
		maketext(spt,ckml,angle,texth,4,3,L"HZ",L"隧道");
		spt.x = Lpt[X],spt.y=Lpt[Y];
		//		 maketext(spt,str,angle,texth,4,3,L"HZ",L"隧道");
		maketext(spt,str,angle,texth,4,3,L"HZ",L"隧道");
		//		 my_texts ("m", Lpt1[X], Lpt1[Y], 2.0, angle, kml);
		//		 my_texts ("m", Lpt[X], Lpt[Y], 2.0, angle, str);
		PZtmp.lc = TunSml;
		pm->xlpoint_pz(&PZtmp);
		CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
		PZLJZX = PZtmp;
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
		//		 CAL_XY(cml-dlen/2.0,&pt[X],&pt[Y],&cita);
		rfwj = PZtmp.a + 0.5*PI;
		if(rfwj > 2.0*PI)
			rfwj = rfwj - 2.0*PI;
		lfwj = rfwj - PI;
		if(lfwj < 0)
			lfwj = lfwj + 2.0*PI;	
		//		 bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
		bpt[X] = PZLJZX.y; bpt[Y] = PZLJZX.x;
		SLpt[X] = bpt[X] + Zlmk*sin(lfwj);
		SLpt[Y] = bpt[Y] + Zlmk*cos(lfwj);

		angle = ads_angle(bpt,SLpt);
		ads_polar(SLpt,angle+PI/4.0,3.0,Lpt1);
		spt.x = SLpt[X];spt.y = SLpt[Y];
		ept.x = Lpt1[X];ept.y = Lpt1[Y];
		//       makeline(spt,ept,4,0,L"隧道");
		Ylmk = GetLMK(TunSml,YLMK,NYLMK);
		if(DBS.Tun[i].XZ=="左幅")
			Ylmk = 0.0;		 
		SRpt[X] = bpt[X] + Ylmk*sin(rfwj);
		SRpt[Y] = bpt[Y] + Ylmk*cos(rfwj);
		angle = ads_angle(bpt,SRpt);
		ads_polar(SRpt,angle-PI/4.0,3.0,Rpt1);
		spt.x = SRpt[X];spt.y = SRpt[Y];
		ept.x = Rpt1[X];ept.y = Rpt1[Y];
		//		 makeline(spt,ept,4,0,L"隧道");
		spt.x = SLpt[X],spt.y = SLpt[Y];
		ept.x = SRpt[X],ept.y = SRpt[Y];
		//		 makeline(spt,ept,4,0,L"隧道");

		Zlmk = GetLMK(TunEml,ZLMK,NZLMK);
		if(DBS.Tun[i].XZ=="右幅")
			Zlmk = 0.0;
		PZtmp.lc = TunEml;
		pm->xlpoint_pz(&PZtmp);
		CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
		PZLJZX = PZtmp;
		PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
		PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
		rfwj = PZtmp.a + 0.5*PI;
		if(rfwj > 2.0*PI)
			rfwj = rfwj - 2.0*PI;
		lfwj = rfwj - PI;
		if(lfwj < 0)
			lfwj = lfwj + 2.0*PI;
		//		 bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
		bpt[X] = PZLJZX.y; bpt[Y] = PZLJZX.x;
		ELpt[X] = bpt[X] + Zlmk*sin(lfwj);
		ELpt[Y] = bpt[Y] + Zlmk*cos(lfwj);

		angle = ads_angle(bpt,ELpt);
		ads_polar(ELpt,angle-PI/4.0,3.0,Lpt1);
		spt.x = ELpt[X];spt.y = ELpt[Y];
		ept.x = Lpt1[X];ept.y = Lpt1[Y];
		//		 makeline(spt,ept,4,0,L"隧道");
		Ylmk = GetLMK(TunEml,YLMK,NYLMK);
		if(DBS.Tun[i].XZ=="左幅")
			Ylmk = 0.0;			 
		ERpt[X] = bpt[X] + Ylmk*sin(rfwj);
		ERpt[Y] = bpt[Y] + Ylmk*cos(rfwj);
		angle = ads_angle(bpt,ERpt);
		ads_polar(ERpt,angle+PI/4.0,3.0,Rpt1);
		spt.x = ERpt[X];spt.y = ERpt[Y];
		ept.x = Rpt1[X];ept.y = Rpt1[Y];
		//		 makeline(spt,ept,4,0,L"隧道");
		spt.x = ELpt[X],spt.y = ELpt[Y];
		ept.x = ERpt[X],ept.y = ERpt[Y];
		//		 makeline(spt,ept,4,0,L"隧道");		 

		AcGePoint2dArray Pt2dAy,Pt2dAy1;
		Pt2dAy.append(AcGePoint2d(SLpt[X],SLpt[Y]));
		cml  = TunSml+10; 
		while(cml<TunEml)
		{
			PZtmp.lc = cml;
			pm->xlpoint_pz(&PZtmp);
			CalOffsetByXLZX(PZtmp.lc,NZLMK,ZLMK,OffsetByXLZX);
			PZLJZX = PZtmp;
			PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
			PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
			Zlmk = GetLMK(cml,ZLMK,NZLMK);
			if(DBS.Tun[i].XZ=="右幅")
				Zlmk = 0.0;
			lfwj = PZtmp.a - PI/2.0;
			if(lfwj < 0.0)
				lfwj = lfwj + 2.0*PI;
			//			 bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
			bpt[X] = PZLJZX.y; bpt[Y] = PZLJZX.x;
			SLpt[X] = bpt[X] + Zlmk*sin(lfwj);
			SLpt[Y] = bpt[Y] + Zlmk*cos(lfwj);
			Pt2dAy.append(AcGePoint2d(SLpt[X],SLpt[Y]));			 
			cml += 10.0;
		}		 
		Pt2dAy.append(AcGePoint2d(ELpt[X],ELpt[Y]));
		makepolyline(Pt2dAy,7,0.0,L"隧道",L"HIDDEN2");	
		//		 acedCommandS(RTPOINT,ELpt,RTSTR,L"",RTNONE);

		//		 acedCommandS(RTSTR,L"Pline",RTPOINT,SRpt,RTNONE);
		Pt2dAy1.append(AcGePoint2d(SRpt[X],SRpt[Y]));
		cml  = TunSml+10; 
		while(cml<TunEml)
		{
			PZtmp.lc = cml;
			pm->xlpoint_pz(&PZtmp);
			CalOffsetByXLZX(PZtmp.lc,NYLMK,YLMK,OffsetByXLZX);
			PZLJZX = PZtmp;
			PZLJZX.x = PZtmp.x + OffsetByXLZX * cos(PZtmp.a+0.5*pi);
			PZLJZX.y = PZtmp.y + OffsetByXLZX * sin(PZtmp.a+0.5*pi);
			Ylmk = GetLMK(cml,YLMK,NYLMK);
			if(DBS.Tun[i].XZ=="左幅")
				Ylmk = 0.0;	
			//			 CAL_XY(cml,&pt[X],&pt[Y],&cita);
			rfwj = PZtmp.a + PI/2.0;
			if(rfwj > 2.0*PI)
				rfwj = rfwj - 2.0*PI;
			//			 bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
			bpt[X] = PZLJZX.y; bpt[Y] = PZLJZX.x;
			SRpt[X] = bpt[X] + Ylmk*sin(rfwj);
			SRpt[Y] = bpt[Y] + Ylmk*cos(rfwj);
			Pt2dAy1.append(AcGePoint2d(SRpt[X],SRpt[Y]));
			cml += 10.0;
		}
		Pt2dAy1.append(AcGePoint2d(ERpt[X],ERpt[Y]));
		makepolyline(Pt2dAy1,7,0.0,L"隧道",L"HIDDEN2");	
	}
	ads_printf(L"OK!\n");
}

//绘两相邻里程的挖方示坡线
void CDrawPM::DrawPartFillBetweenTwoMl(double sml,double eml, int ZorY,int qPtNum,int hPtNum,
									   AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr,bool &flag)
{
	ads_point pt1,pt2,tmpt,pjd1,pjd2,cpt1,cpt2;

	double ml = floor(sml/2)*2 + SPXJJ;
	double angle;
	int npt,i;
	AcGePoint3d spt,ept;

	if(qPtNum < hPtNum)
		npt = qPtNum;
	else
		npt = hPtNum;
	while(ml<=eml)
	{
		for(i=4;i<npt-1;i++)
		{
			cpt1[X]=qPtarr[i].x;
			cpt1[Y]=qPtarr[i].y;
			cpt2[X]=hPtarr[i].x;
			cpt2[Y]=hPtarr[i].y;
			if(fmod(i,2.0)==0)
			{
				angle=ads_angle(cpt1, cpt2);
				ads_polar(cpt1 , angle , ml-sml , pt1);
				ads_polar(pt1 , angle-ZorY*0.5*pi , 1000 , pt2);
				pjd1[X] = qPtarr[i+1].x;
				pjd1[Y] = qPtarr[i+1].y;
				pjd2[X] = hPtarr[i+1].x;
				pjd2[Y] = hPtarr[i+1].y;
				pt1[Z]=pt2[Z]=pjd1[Z]=pjd2[Z]=0.0;
				if(ads_inters(pt1,pt2,pjd1,pjd2,0,tmpt)==RTNORM)
				{
					pt2[X]=tmpt[X];
					pt2[Y]=tmpt[Y];
					if(flag)//fmod(k,2)==0 
					{
						pt2[X] = 0.5*(pt1[X] + pt2[X]);
						pt2[Y] = 0.5*(pt1[Y] + pt2[Y]);
					}
				}

				pt1[Z]=10000.0;
				pt2[Z]=10000.0;
				spt.set(pt1[X],pt1[Y],pt1[Z]);
				ept.set(pt2[X],pt2[Y],pt2[Z]);
				makeline(spt,ept,2,0,L"平面图-填方示坡线"); 
			}
		}
		if(qPtNum < hPtNum)
		{
			cpt1[X]=qPtarr[qPtNum-1].x;
			cpt1[Y]=qPtarr[qPtNum-1].y;
			pjd1[X] = qPtarr[qPtNum-1].x;
			pjd1[Y] = qPtarr[qPtNum-1].y;
			for(i=qPtNum-1;i<hPtNum-1;i++)
			{
				cpt2[X]=hPtarr[i].x;
				cpt2[Y]=hPtarr[i].y;
				if(fmod(i,2.0)==0)
				{
					angle=ads_angle(cpt1, cpt2);
					ads_polar(cpt1 , angle , ml-sml , pt1);
					ads_polar(pt1 , angle-ZorY*0.5*pi , 1000 , pt2);
					pjd2[X] = hPtarr[i+1].x;
					pjd2[Y] = hPtarr[i+1].y;
					pt1[Z]=pt2[Z]=pjd1[Z]=pjd2[Z]=0.0;
					if(ads_inters(pt1,pt2,pjd1,pjd2,0,tmpt)==RTNORM)
					{
						pt2[X]=tmpt[X],pt2[Y]=tmpt[Y];
						if(flag)//fmod(k,2)==0 
						{
							pt2[X] = 0.5*(pt2[X] + pt1[X]);
							pt2[Y] = 0.5*(pt2[Y] + pt1[Y]);
						}
						pt1[Z]=10000.0;
						pt2[Z]=10000.0;				
						spt.set(pt1[X],pt1[Y],pt1[Z]);
						ept.set(pt2[X],pt2[Y],pt2[Z]);
						makeline(spt,ept,2,0,L"平面图-填方示坡线"); 
					}
				}
			}
		}
		else
		{
			cpt2[X]=hPtarr[hPtNum-1].x;
			cpt2[Y]=hPtarr[hPtNum-1].y;
			pjd2[X] = hPtarr[hPtNum-1].x;
			pjd2[Y] = hPtarr[hPtNum-1].y;
			for(i=hPtNum-1;i<qPtNum-1;i++)
			{
				cpt1[X]=qPtarr[i].x;
				cpt1[Y]=qPtarr[i].y;
				if(fmod(i,2.0)==0)
				{
					angle=ads_angle(cpt1, cpt2);
					ads_polar(cpt1 , angle , ml-sml , pt1);
					ads_polar(pt1 , angle-ZorY*0.5*pi , 1000 , pt2);
					pjd1[X] = qPtarr[i+1].x;
					pjd1[Y] = qPtarr[i+1].y;
					pt1[Z]=pt2[Z]=pjd1[Z]=pjd2[Z]=0.0;
					if(ads_inters(pt1,pt2,pjd1,pjd2,0,tmpt)==RTNORM)
					{
						pt2[X]=tmpt[X],pt2[Y]=tmpt[Y];
						if(flag)//fmod(k,2)==0 
						{
							pt2[X] = 0.5*(pt2[X] + pt1[X]);
							pt2[Y] = 0.5*(pt2[Y] + pt1[Y]);
						}
						pt1[Z]=10000.0;
						pt2[Z]=10000.0;
						spt.set(pt1[X],pt1[Y],pt1[Z]);
						ept.set(pt2[X],pt2[Y],pt2[Z]);
						makeline(spt,ept,2,0,L"平面图-填方示坡线"); 
					}
				}
			}
		}
		flag = !flag;
		ml=ml+SPXJJ;
	}
	for(i=4;i<npt-1;i++)
	{
		spt=qPtarr[i];
		ept=hPtarr[i];
		spt.z = 10000.0;
		ept.z = 10000.0;
		if(i==4)
			makeline(spt,ept,1,1,L"平面图-路肩线");//绘路肩线
		else
			makeline(spt,ept,7,0,L"平面图-平台线");
	}
	if(qPtNum < hPtNum)
	{
		spt = qPtarr[qPtNum-1];
		spt.z = 10000.0;
		for(i=qPtNum-1;i<hPtNum;i++)
		{
			ept = hPtarr[i];
			ept.z = 10000.0;
			makeline(spt,ept,7,0,L"平面图-平台线");
		}
	}
	else
	{
		ept = hPtarr[hPtNum-1];
		ept.z = 10000.0;
		for(i=hPtNum-1;i<qPtNum;i++)
		{
			spt = qPtarr[i];
			spt.z = 10000.0;
			makeline(spt,ept,7,0,L"平面图-平台线");
		}
	}
	//绘坡脚线
	spt = qPtarr[qPtNum-1];
	ept = hPtarr[hPtNum-1];
	spt.z = 10000.0;
	ept.z = 10000.0;
	makeline(spt,ept,2,0.0,L"平面图-坡脚线");

}

//绘两相邻里程的挖方示坡线
void CDrawPM::DrawCutPartBetweenTwoMl(double sml,double eml,int ZorY,int qPtNum,int hPtNum,
									  AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr,bool &flag)
{
	ads_point pt1,pt2,tmpt,pjd1,pjd2,cpt1,cpt2,qGouPt,hGouPt,GouPt;
	double ml = floor(sml/2)*2 + SPXJJ;
	double angle;
	int npt,i;
	AcGePoint3d spt,ept;
	int QorH;
	QorH=1;	

	//绘边沟箭头
	if(qPtNum>6&&hPtNum>6)
	{
		qGouPt[X] = 0.5*(qPtarr[4].x+qPtarr[7].x);
		qGouPt[Y] = 0.5*(qPtarr[4].y+qPtarr[7].y);

		hGouPt[X] = 0.5*(hPtarr[4].x+hPtarr[7].x);
		hGouPt[Y] = 0.5*(hPtarr[4].y+hPtarr[7].y);
		if(qPtarr[4].z < hPtarr[4].z)//后一标高大于前一标高
		{			
			angle = ads_angle(hGouPt,qGouPt);
			QorH = -1;
		}
		else
		{
			angle = ads_angle(qGouPt,hGouPt);
			QorH = 1;
		}	
		GouPt[X]=0.5*(qGouPt[X]+hGouPt[X]);
		GouPt[Y]=0.5*(qGouPt[Y]+hGouPt[Y]);
		double cml = 0.5*(sml+eml);
		if(cml-BGarowcml > 15)
		{
			drawGouArrow(GouPt,angle,QorH*ZorY);
			BGarowcml = cml;
		}
	}

	if(qPtNum < hPtNum)
		npt = qPtNum;
	else
		npt = hPtNum;

	while(ml<=eml)
	{
		for(i=8;i<npt-1;i++)
		{
			cpt1[X]=qPtarr[i].x;
			cpt1[Y]=qPtarr[i].y;
			cpt2[X]=hPtarr[i].x;
			cpt2[Y]=hPtarr[i].y;
			if(fmod(i,2.0)==0)
			{
				angle=ads_angle(cpt1, cpt2);
				ads_polar(cpt1 , angle , ml-sml , pt1);
				ads_polar(pt1 , angle-ZorY*0.5*pi , 1000 , pt2);
				pjd1[X] = qPtarr[i+1].x;
				pjd1[Y] = qPtarr[i+1].y;
				pjd2[X] = hPtarr[i+1].x;
				pjd2[Y] = hPtarr[i+1].y;
				pt1[Z]=pt2[Z]=pjd1[Z]=pjd2[Z]=0.0;
				if(ads_inters(pt1,pt2,pjd1,pjd2,0,tmpt)==RTNORM)
				{
					pt2[X]=tmpt[X],pt2[Y]=tmpt[Y];
					if(flag)//fmod(k,2)==0 
					{
						pt1[X] = 0.5*(pt2[X] + pt1[X]);
						pt1[Y] = 0.5*(pt2[Y] + pt1[Y]);
					}
					pt1[Z]=10000.0;
					pt2[Z]=10000.0;
					spt.set(pt2[X],pt2[Y],pt2[Z]);
					ept.set(pt1[X],pt1[Y],pt1[Z]);
					makeline(spt,ept,3,0,L"平面图-挖方示坡线");
				}
			}
		}	        
		if(qPtNum < hPtNum)
		{
			cpt1[X]=qPtarr[qPtNum-1].x;
			cpt1[Y]=qPtarr[qPtNum-1].y;
			pjd1[X] = qPtarr[qPtNum-1].x;
			pjd1[Y] = qPtarr[qPtNum-1].y;
			for(i=qPtNum-1;i<hPtNum-1;i++)
			{
				if(i>7)
				{
					cpt2[X]=hPtarr[i].x;
					cpt2[Y]=hPtarr[i].y;
					if(fmod(i,2.0)==0)
					{
						angle=ads_angle(cpt1, cpt2);
						ads_polar(cpt1 , angle , ml-sml , pt1);
						ads_polar(pt1 , angle-ZorY*0.5*pi , 1000 , pt2);
						pjd2[X] = hPtarr[i+1].x;
						pjd2[Y] = hPtarr[i+1].y;
						pt1[Z]=pt2[Z]=pjd1[Z]=pjd2[Z]=0.0;
						if(ads_inters(pt1,pt2,pjd1,pjd2,0,tmpt)==RTNORM)
						{
							pt2[X]=tmpt[X],pt2[Y]=tmpt[Y];
							if(flag)//fmod(k,2)==0 
							{
								pt1[X] = 0.5*(pt2[X] + pt1[X]);
								pt1[Y] = 0.5*(pt2[Y] + pt1[Y]);
							}
							pt1[Z]=10000.0;
							pt2[Z]=10000.0;
							spt.set(pt2[X],pt2[Y],pt2[Z]);
							ept.set(pt1[X],pt1[Y],pt1[Z]);
							makeline(spt,ept,3,0,L"平面图-挖方示坡线");
						}
					}
				}
			}
		}
		else
		{
			cpt2[X]=hPtarr[hPtNum-1].x;
			cpt2[Y]=hPtarr[hPtNum-1].y;
			pjd2[X] = hPtarr[hPtNum-1].x;
			pjd2[Y] = hPtarr[hPtNum-1].y;
			for(i=hPtNum-1;i<qPtNum-1;i++)
			{
				if(i>7)
				{
					cpt1[X]=qPtarr[i].x;
					cpt1[Y]=qPtarr[i].y;
					if(fmod(i,2.0)==0)
					{
						angle=ads_angle(cpt1, cpt2);
						ads_polar(cpt1 , angle , ml-sml , pt1);
						ads_polar(pt1 , angle-ZorY*0.5*pi , 1000 , pt2);
						pjd1[X] = qPtarr[i+1].x;
						pjd1[Y] = qPtarr[i+1].y;
						pt1[Z]=pt2[Z]=pjd1[Z]=pjd2[Z]=0.0;
						if(ads_inters(pt1,pt2,pjd1,pjd2,0,tmpt)==RTNORM)
						{
							pt2[X]=tmpt[X],pt2[Y]=tmpt[Y];
							if(flag)//fmod(k,2)==0 
							{
								pt1[X] = 0.5*(pt2[X] + pt1[X]);
								pt1[Y] = 0.5*(pt2[Y] + pt1[Y]);
							}
							pt1[Z]=10000.0;
							pt2[Z]=10000.0;
							spt.set(pt2[X],pt2[Y],pt2[Z]);
							ept.set(pt1[X],pt1[Y],pt1[Z]);
							makeline(spt,ept,3,0,L"平面图-挖方示坡线");
						}
					}
				}
			}
		}
		flag = !flag;
		ml=ml+SPXJJ;
	}

	for(i=7;i<npt-1;i++)
	{
		spt=qPtarr[i];
		ept=hPtarr[i];
		spt.z = 10000.0;
		ept.z = 10000.0;
		if(i==7)
			makeline(spt,ept,2,0,L"平面图-边沟线");//绘路肩线
		else
			makeline(spt,ept,7,0,L"平面图-平台线");
	}
	if(qPtNum < hPtNum)
	{
		spt = qPtarr[qPtNum-1];
		spt.z = 10000.0;
		for(i=qPtNum-1;i<hPtNum;i++)
		{
			ept = hPtarr[i];
			ept.z = 10000.0;
			if(i>7)
				makeline(spt,ept,7,0,L"平面图-平台线");
			else if(i==7)
				makeline(spt,ept,2,0,L"平面图-边沟线");
		}
	}
	else
	{
		ept = hPtarr[hPtNum-1];
		ept.z = 10000.0;
		for(i=hPtNum-1;i<qPtNum;i++)
		{
			spt = qPtarr[i];
			spt.z = 10000.0;
			if(i>7)
				makeline(spt,ept,7,0,L"平面图-平台线");
			else if(i==7)
				makeline(spt,ept,2,0,L"平面图-边沟线");
		}
	}

	spt=qPtarr[4];
	ept=hPtarr[4];
	spt.z = 10000.0;
	ept.z = 10000.0;
	makeline(spt,ept,1,1,L"平面图-路肩线");//绘路肩线
	//绘坡脚线
	spt = qPtarr[qPtNum-1];
	ept = hPtarr[hPtNum-1];
	spt.z = 10000.0;
	ept.z = 10000.0;
	makeline(spt,ept,2,0.0,L"平面图-坡脚线");


}

void CDrawPM::drawGouArrow(ads_point pt, double angle,int inf)
{
	ads_point ept,mpt;

	ads_polar(pt,angle,3.0,ept);

	ads_polar(pt,angle-inf*0.5*pi,1.0,mpt);
	//  ads_polar(pt,angle-inf*0.5*pi,1.0,mpt);

	AcGePoint3d fpt,tpt,cpt;
	fpt.x = pt[X] ,  fpt.y = pt[Y] , fpt.z = 0.0;
	tpt.x = ept[X] , tpt.y = ept[Y] , tpt.z = 0.0;
	cpt.x = mpt[X] , cpt.y = mpt[Y] , cpt.z = 0.0;
	makesolid(fpt,tpt,cpt,6,L"平面图-水沟箭头");

	ads_polar(pt,angle+pi,4.25,ept);
	fpt.x = pt[X] ,  fpt.y = pt[Y] , fpt.z = 0.0;
	tpt.x = ept[X] , tpt.y = ept[Y] , tpt.z = 0.0;
	makeline(fpt,tpt,6,0,L"平面图-水沟箭头");  
}
//求填挖零点断面
void CDrawPM::CalTWZeroPt(double bml,double eml,double Bdeltah,double Edeltah,double &k,double &MidMl)
{
	if(Bdeltah*Edeltah<0.0)
		k = fabs(Edeltah)/fabs(Bdeltah);
	else
		k = 1;
	//找出路肩填挖零点
	MidMl = (eml + k*bml)/(1+k);
}

void CDrawPM::DrawAllSlop(int i)
{
	ads_point pt1,pt2,pt3,Ljpt;
	double k,ang,len,dx,dy,factor;
	xlpoint ptmp;
	double OffsetByXLZX = 0.0;
	factor = 2;
	xlpoint PZLJZX;//路基中心点
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
				ptmp.lc = hdmdes[i].cml +5.0;

			CalOffsetByXLZX(ptmp.lc,NZLMK,ZLMK,OffsetByXLZX);			
			pm->xlpoint_pz(&ptmp);
			PZLJZX = ptmp;
			PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
			PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
			// 			Ljpt[X] = ptmp.y + fabs(hdmdes[i+1].RHFL[8])*sin(ptmp.a-0.5*PI);
			// 			Ljpt[Y] = ptmp.x + fabs(hdmdes[i+1].RHFL[8])*cos(ptmp.a-0.5*PI);
			// 			Ljpt[X] = ptmp.y + (-hdmdes[i+1].RHFL[8] + OffsetByXLZX)*sin(ptmp.a+0.5*PI);
			// 			Ljpt[Y] = ptmp.x + (-hdmdes[i+1].RHFL[8] + OffsetByXLZX)*cos(ptmp.a+0.5*PI);
			Ljpt[X] = PZLJZX.y + fabs(hdmdes[i+1].RHFL[8])*sin(ptmp.a-0.5*PI);
			Ljpt[Y] = PZLJZX.x + fabs(hdmdes[i+1].RHFL[8])*cos(ptmp.a-0.5*PI);
			pt2[X] = hdmdes[i].PTRPTL[4].x + k*(Ljpt[X]-hdmdes[i].PTRPTL[4].x);
			pt2[Y] = hdmdes[i].PTRPTL[4].y + k*(Ljpt[Y]-hdmdes[i].PTRPTL[4].y);

			pt1[X] = hdmdes[i].PTRPTL[4].x;
			pt1[Y] = hdmdes[i].PTRPTL[4].y;
			pt3[X] = hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].x;
			pt3[Y] = hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].y;
		}
		else
		{
			ptmp.lc = hdmdes[i].cml;
			pm->xlpoint_pz(&ptmp);
			CalOffsetByXLZX(ptmp.lc,NZLMK,ZLMK,OffsetByXLZX);
			PZLJZX = ptmp;
			PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
			PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
			/*				len =  fabs(hdmdes[i].RHFL[6])/sin(ang*PI/180);
			if(len<0.1)
			len=1.0;
			pt1[X] = ptmp.y + len*sin(ptmp.a-(PI-ang*PI/180));
			pt1[Y] = ptmp.x + len*cos(ptmp.a-(PI-ang*PI/180));*/

			double pt[2],prjml;
			len =  fabs(hdmdes[i].RHFL[8]);
			if(len<0.001)
				len=0.001;
			pm->CalPZXPT(hdmdes[i].cml,ang+180,len-OffsetByXLZX,pt,prjml);
			pt1[X] = pt[1];
			pt1[Y] = pt[0];
			INTERS(&pt3[X],&pt3[Y],hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].x,hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].y,
				hdmdes[i-1].PTRPTL[hdmdes[i-1].Lnpt-1].x,hdmdes[i-1].PTRPTL[hdmdes[i-1].Lnpt-1].y,
				PZLJZX.y,PZLJZX.x,pt1[X],pt1[Y]);
			dx=pt3[X]-pt1[X];
			dy=pt3[Y]-pt1[Y];	
			len = 2*sqrt(dx*dx+dy*dy)/3;
			if(len<0.01)
				len = 5.0;
			pt2[X] = pt1[X] + len*sin(ptmp.a);
			pt2[Y] = pt1[Y] + len*cos(ptmp.a);

			AcGePoint3d p1,p2,p3,p4;
			p1.x = pt1[X];p1.y = pt1[Y];
			p2.x = pt3[X]+ len*factor*sin(ptmp.a-(PI-ang*PI/180));
			p2.y = pt3[Y]+ len*factor*cos(ptmp.a-(PI-ang*PI/180));
			p3.x = p2.x + len*factor*sin(ptmp.a);
			p3.y = p2.y + len*factor*cos(ptmp.a);
			p4.x = pt1[X] + len*factor*sin(ptmp.a);
			p4.y = pt1[Y] + len*factor*cos(ptmp.a);
			ads_point bkpt1,bkpt2,bkpt3,bkpt4;
			bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
			bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
			bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
			bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
			int result;
			result = 1234;
			result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);			   
			for(int k=2;k<4;k++)
			{
				//								flag_k=k;//是否显示进度条。<2显示
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);  
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
				if(result < 0)
					break;
			}
			k+=2;
			BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
					   			
		}
		dx = pt1[X]-pt3[X];
		dy = pt1[Y]-pt3[Y];
		len = sqrt(dx*dx+dy*dy);
		if(len < 0.01)
		{
			pt3[X] = pt1[X] + 5.0*sin(ptmp.a-0.5*PI);
			pt3[Y] = pt1[Y] + 5.0*cos(ptmp.a-0.5*PI);
		}
		Draw_BridgeSlop(pt1,pt2,pt3);			
	}
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
				ptmp.lc = hdmdes[i].cml +5.0;
			CalOffsetByXLZX(ptmp.lc,NYLMK,YLMK,OffsetByXLZX);
			pm->xlpoint_pz(&ptmp);
			PZLJZX = ptmp;
			PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
			PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
			// 			Ljpt[X] = ptmp.y + (hdmdes[i+1].RHFR[8]+OffsetByXLZX)*sin(ptmp.a+0.5*PI);
			// 			Ljpt[Y] = ptmp.x + (hdmdes[i+1].RHFR[8]+OffsetByXLZX)*cos(ptmp.a+0.5*PI);
			Ljpt[X] = PZLJZX.y + (hdmdes[i+1].RHFR[8])*sin(ptmp.a+0.5*PI);
			Ljpt[Y] = PZLJZX.x + (hdmdes[i+1].RHFR[8])*cos(ptmp.a+0.5*PI);
			pt2[X] = hdmdes[i].PTRPTR[4].x + k*(Ljpt[X]-hdmdes[i].PTRPTR[4].x);
			pt2[Y] = hdmdes[i].PTRPTR[4].y + k*(Ljpt[Y]-hdmdes[i].PTRPTR[4].y);
			pt1[X] = hdmdes[i].PTRPTR[4].x;
			pt1[Y] = hdmdes[i].PTRPTR[4].y;
			pt3[X] = hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].x;
			pt3[Y] = hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].y;
		}
		else
		{
			ptmp.lc = hdmdes[i].cml;
			pm->xlpoint_pz(&ptmp);
			CalOffsetByXLZX(ptmp.lc,NYLMK,YLMK,OffsetByXLZX);
			PZLJZX = ptmp;
			PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
			PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
			/*			len =  fabs(hdmdes[i].RHFR[6])/sin(ang*PI/180);
			if(len<0.1)
			len=1.0;
			pt1[X] = ptmp.y + len*sin(ptmp.a+ang*PI/180);
			pt1[Y] = ptmp.x + len*cos(ptmp.a+ang*PI/180);*/
			double pt[2],prjml;
			len =  fabs(hdmdes[i].RHFR[8]);
			if(len<0.001)
				len=0.001;
			pm->CalPZXPT(hdmdes[i].cml,ang,len+OffsetByXLZX,pt,prjml);
			pt1[X] = pt[1];
			pt1[Y] = pt[0];
			INTERS(&pt3[X],&pt3[Y],hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].x,hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].y,
				hdmdes[i-1].PTRPTR[hdmdes[i-1].Rnpt-1].x,hdmdes[i-1].PTRPTR[hdmdes[i-1].Rnpt-1].y,
				PZLJZX.y,PZLJZX.x,pt1[X],pt1[Y]);
			dx=pt3[X]-pt1[X];
			dy=pt3[Y]-pt1[Y];
			len = 2*sqrt(dx*dx+dy*dy)/3;
			if(len<0.01)
				len = 5.0;
			pt2[X] = pt1[X] + len*sin(ptmp.a);
			pt2[Y] = pt1[Y] + len*cos(ptmp.a);

			AcGePoint3d p1,p2,p3,p4;
			p1.x = pt1[X];p1.y = pt1[Y];
			p2.x = pt3[X]+ len*factor*sin(ptmp.a+ang*PI/180);
			p2.y = pt3[Y]+ len*factor*cos(ptmp.a+ang*PI/180);
			p3.x = p2.x + len*factor*sin(ptmp.a);
			p3.y = p2.y + len*factor*cos(ptmp.a);
			p4.x = pt1[X] + len*factor*sin(ptmp.a);
			p4.y = pt1[Y] + len*factor*cos(ptmp.a);
			ads_point bkpt1,bkpt2,bkpt3,bkpt4;
			bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
			bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
			bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
			bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
			int result;
			result = 1234;
			result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);			   
			for(int k=2;k<4;k++)
			{
				//								flag_k=k;//是否显示进度条。<2显示
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);  
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
				if(result < 0)
					break;
			}
			k+=2;
			BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR,L"erase",RTSTR,L"WP",RTPOINT,bkpt1,RTPOINT,bkpt2,RTPOINT,bkpt3,RTPOINT,bkpt4,RTSTR,L"",RTSTR,L"",0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
						   			
		}
		dx = pt1[X]-pt3[X];
		dy = pt1[Y]-pt3[Y];
		len = sqrt(dx*dx+dy*dy);
		if(len < 0.01)
		{
			pt3[X] = pt1[X] + 5.0*sin(ptmp.a+0.5*PI);
			pt3[Y] = pt1[Y] + 5.0*cos(ptmp.a+0.5*PI);
		}
		Draw_BridgeSlop(pt1,pt2,pt3);			
	}
	if(hdmdes[i].ZDMXZ==1)//左侧桥尾锥坡
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
				ptmp.lc = hdmdes[i].cml -5.0;
			CalOffsetByXLZX(ptmp.lc,NZLMK,ZLMK,OffsetByXLZX);
			pm->xlpoint_pz(&ptmp);
			PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
			PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
			// 			Ljpt[X] = ptmp.y + (-hdmdes[i-1].RHFL[8]+OffsetByXLZX)*sin(ptmp.a+0.5*PI);
			// 			Ljpt[Y] = ptmp.x + (-hdmdes[i-1].RHFL[8]+OffsetByXLZX)*cos(ptmp.a+0.5*PI);
			Ljpt[X] = ptmp.y + fabs(hdmdes[i-1].RHFL[8])*sin(ptmp.a-0.5*PI);
			Ljpt[Y] = ptmp.x + fabs(hdmdes[i-1].RHFL[8])*cos(ptmp.a-0.5*PI);
			pt2[X] = hdmdes[i].PTRPTL[4].x - k*(hdmdes[i].PTRPTL[4].x-Ljpt[X]);
			pt2[Y] = hdmdes[i].PTRPTL[4].y - k*(hdmdes[i].PTRPTL[4].y-Ljpt[Y]);
			pt1[X] = hdmdes[i].PTRPTL[4].x;
			pt1[Y] = hdmdes[i].PTRPTL[4].y;
			pt3[X] = hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].x;
			pt3[Y] = hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].y;
		}
		else
		{
			ptmp.lc = hdmdes[i].cml;
			pm->xlpoint_pz(&ptmp);
			CalOffsetByXLZX(ptmp.lc,NZLMK,ZLMK,OffsetByXLZX);
			PZLJZX = ptmp;
			PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a + 0.5*pi);
			PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a + 0.5*pi);
			/*			len =  fabs(hdmdes[i].RHFL[6])/sin(ang*PI/180);
			if(len<0.1)
			len=1.0;
			pt1[X] = ptmp.y + len*sin(ptmp.a-(PI-ang*PI/180));
			pt1[Y] = ptmp.x + len*cos(ptmp.a-(PI-ang*PI/180));	*/
			double pt[2],prjml;
			len =  fabs(hdmdes[i].RHFL[8]);
			if(len<0.001)
				len=0.001;
			pm->CalPZXPT(hdmdes[i].cml,ang+180,len-OffsetByXLZX,pt,prjml);
			pt1[X] = pt[1];
			pt1[Y] = pt[0];
			INTERS(&pt3[X],&pt3[Y],hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].x,hdmdes[i].PTRPTL[hdmdes[i].Lnpt-1].y,
				hdmdes[i+1].PTRPTL[hdmdes[i+1].Lnpt-1].x,hdmdes[i+1].PTRPTL[hdmdes[i+1].Lnpt-1].y,
				PZLJZX.y,PZLJZX.x,pt1[X],pt1[Y]);
			dx=pt3[X]-pt1[X];
			dy=pt3[Y]-pt1[Y];
			len = 2*sqrt(dx*dx+dy*dy)/3;
			if(len<0.01)
				len = 5.0;
			pt2[X] = pt1[X] + len*sin(ptmp.a+PI);
			pt2[Y] = pt1[Y] + len*cos(ptmp.a+PI);

			AcGePoint3d p1,p2,p3,p4;
			p1.x = pt1[X];p1.y = pt1[Y];
			p2.x = pt3[X]+ len*factor*sin(ptmp.a-(PI-ang*PI/180));
			p2.y = pt3[Y]+ len*factor*cos(ptmp.a-(PI-ang*PI/180));
			p3.x = p2.x + len*factor*sin(ptmp.a+PI);
			p3.y = p2.y + len*factor*cos(ptmp.a+PI);
			p4.x = pt1[X] + len*factor*sin(ptmp.a+PI);
			p4.y = pt1[Y] + len*factor*cos(ptmp.a+PI);
			ads_point bkpt1,bkpt2,bkpt3,bkpt4;
			bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
			bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
			bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
			bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
			int result;
			result = 1234;
			result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);			   
			for(int k=2;k<4;k++)
			{
				//								flag_k=k;//是否显示进度条。<2显示
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);  
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
				if(result < 0)
					break;
			}
			k+=2;
			BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
				   			
		}
		dx = pt1[X]-pt3[X];
		dy = pt1[Y]-pt3[Y];
		len = sqrt(dx*dx+dy*dy);
		if(len < 0.01)
		{
			pt3[X] = pt1[X] + 5.0*sin(ptmp.a-0.5*PI);
			pt3[Y] = pt1[Y] + 5.0*cos(ptmp.a-0.5*PI);
		}
		Draw_BridgeSlop(pt1,pt2,pt3);
	}
	if(hdmdes[i].YDMXZ==1)//右侧桥尾锥坡
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
				ptmp.lc = hdmdes[i].cml -5.0;
			CalOffsetByXLZX(ptmp.lc,NYLMK,YLMK,OffsetByXLZX);
			pm->xlpoint_pz(&ptmp);
			PZLJZX = ptmp;
			PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
			PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
			//			Ljpt[X] = ptmp.y + (hdmdes[i-1].RHFR[8]+OffsetByXLZX)*sin(ptmp.a+0.5*PI);
			//			Ljpt[Y] = ptmp.x + (hdmdes[i-1].RHFR[8]+OffsetByXLZX)*cos(ptmp.a+0.5*PI);		
			Ljpt[X] = ptmp.y + (hdmdes[i-1].RHFR[8])*sin(ptmp.a+0.5*PI);
			Ljpt[Y] = ptmp.x + (hdmdes[i-1].RHFR[8])*cos(ptmp.a+0.5*PI);	
			pt2[X] = hdmdes[i].PTRPTR[4].x - k*(hdmdes[i].PTRPTR[4].x-Ljpt[X]);
			pt2[Y] = hdmdes[i].PTRPTR[4].y - k*(hdmdes[i].PTRPTR[4].y-Ljpt[Y]);
			pt1[X] = hdmdes[i].PTRPTR[4].x;
			pt1[Y] = hdmdes[i].PTRPTR[4].y;
			pt3[X] = hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].x;
			pt3[Y] = hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].y;
		}
		else
		{
			ptmp.lc = hdmdes[i].cml;
			pm->xlpoint_pz(&ptmp);
			CalOffsetByXLZX(ptmp.lc,NYLMK,YLMK,OffsetByXLZX);
			PZLJZX = ptmp;
			PZLJZX.x = ptmp.x + OffsetByXLZX * cos(ptmp.a+0.5*pi);
			PZLJZX.y = ptmp.y + OffsetByXLZX * sin(ptmp.a+0.5*pi);
			/*			len =  fabs(hdmdes[i].RHFR[6])/sin(ang*PI/180);
			if(len<0.1)
			len=1.0;
			pt1[X] = ptmp.y + len*sin(ptmp.a+ang*PI/180);
			pt1[Y] = ptmp.x + len*cos(ptmp.a+ang*PI/180);	*/
			double pt[2],prjml;
			len =  fabs(hdmdes[i].RHFR[8]);
			if(len<0.001)
				len=0.001;
			pm->CalPZXPT(hdmdes[i].cml,ang,len+OffsetByXLZX,pt,prjml);
			pt1[X] = pt[1];
			pt1[Y] = pt[0];
			INTERS(&pt3[X],&pt3[Y],hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].x,hdmdes[i].PTRPTR[hdmdes[i].Rnpt-1].y,
				hdmdes[i+1].PTRPTR[hdmdes[i+1].Rnpt-1].x,hdmdes[i+1].PTRPTR[hdmdes[i+1].Rnpt-1].y,
				PZLJZX.y,PZLJZX.x,pt1[X],pt1[Y]);
			dx=pt3[X]-pt1[X];
			dy=pt3[Y]-pt1[Y];
			len = 2*sqrt(dx*dx+dy*dy)/3;
			if(len<0.01)
				len = 5.0;
			pt2[X] = pt1[X] + len*sin(ptmp.a+PI);
			pt2[Y] = pt1[Y] + len*cos(ptmp.a+PI);

			AcGePoint3d p1,p2,p3,p4;
			p1.x = pt1[X];p1.y = pt1[Y];
			p2.x = pt3[X]+ len*factor*sin(ptmp.a+ang*PI/180);
			p2.y = pt3[Y]+ len*factor*cos(ptmp.a+ang*PI/180);
			p3.x = p2.x + len*factor*sin(ptmp.a+PI);
			p3.y = p2.y + len*factor*cos(ptmp.a+PI);
			p4.x = pt1[X] + len*factor*sin(ptmp.a+PI);
			p4.y = pt1[Y] + len*factor*cos(ptmp.a+PI);
			ads_point bkpt1,bkpt2,bkpt3,bkpt4;
			bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
			bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
			bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
			bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
			int result;
			result = 1234;
			result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);			   
			for(int k=2;k<4;k++)
			{
				//								flag_k=k;//是否显示进度条。<2显示
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);  
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
				if(result < 0)
					break;
			}
			k+=2;
			BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
				   				
		}
		dx = pt1[X]-pt3[X];
		dy = pt1[Y]-pt3[Y];
		len = sqrt(dx*dx+dy*dy);
		if(len < 0.01)
		{
			pt3[X] = pt1[X] + 5.0*sin(ptmp.a+0.5*PI);
			pt3[Y] = pt1[Y] + 5.0*cos(ptmp.a+0.5*PI);
		}
		Draw_BridgeSlop(pt1,pt2,pt3);
	}
	//------------------

	if(hdmdes[i].DorS==2)//双路面
	{
		if(hdmdes[i].Y_ZDMXZ==-1)//左侧桥头锥坡
		{
			if(hdmdes[i].Y_Lnpt==0)
				return;
			if(2*fabs(hdmdes[i].Y_RPTL[hdmdes[i].Y_Lnpt-1].x)/3 > 20)//2/3坡脚点至路肩点距离
				k = 1;
			else
				k = fabs(hdmdes[i].Y_RPTL[hdmdes[i].Y_Lnpt-1].x)/30; 


			pt2[X] = hdmdes[i].Y_PTRPTL[4].x + k*(hdmdes[i+1].Y_PTRPTL[4].x-hdmdes[i].Y_PTRPTL[4].x);
			pt2[Y] = hdmdes[i].Y_PTRPTL[4].y + k*(hdmdes[i+1].Y_PTRPTL[4].y-hdmdes[i].Y_PTRPTL[4].y);
			pt1[X] = hdmdes[i].Y_PTRPTL[4].x;
			pt1[Y] = hdmdes[i].Y_PTRPTL[4].y;
			pt3[X] = hdmdes[i].Y_PTRPTL[hdmdes[i].Y_Lnpt-1].x;
			pt3[Y] = hdmdes[i].Y_PTRPTL[hdmdes[i].Y_Lnpt-1].y;
			Draw_BridgeSlop(pt1,pt2,pt3);			
		}
		if(hdmdes[i].Y_YDMXZ==-1)//右侧桥头锥坡
		{
			if(hdmdes[i].Y_Rnpt==0)
				return;
			if(2*fabs(hdmdes[i].Y_RPTR[hdmdes[i].Y_Rnpt-1].x)/3 > 20)//2/3坡脚点至路肩点距离
				k = 1;
			else
				k = fabs(hdmdes[i].Y_RPTR[hdmdes[i].Y_Rnpt-1].x)/30; 

			pt2[X] = hdmdes[i].Y_PTRPTR[4].x + k*(hdmdes[i+1].Y_PTRPTR[4].x-hdmdes[i].Y_PTRPTR[4].x);
			pt2[Y] = hdmdes[i].Y_PTRPTR[4].y + k*(hdmdes[i+1].Y_PTRPTR[4].y-hdmdes[i].Y_PTRPTR[4].y);
			pt1[X] = hdmdes[i].Y_PTRPTR[4].x;
			pt1[Y] = hdmdes[i].Y_PTRPTR[4].y;
			pt3[X] = hdmdes[i].Y_PTRPTR[hdmdes[i].Y_Rnpt-1].x;
			pt3[Y] = hdmdes[i].Y_PTRPTR[hdmdes[i].Y_Rnpt-1].y;
			Draw_BridgeSlop(pt1,pt2,pt3);			
		}
		if(hdmdes[i].Y_ZDMXZ==1)//左侧桥尾锥坡
		{
			if(hdmdes[i].Y_Lnpt==0)
				return;
			if(2*fabs(hdmdes[i].Y_RPTL[hdmdes[i].Y_Lnpt-1].x)/3 > 20)//2/3坡脚点至路肩点距离
				k = 1;
			else
				k =  fabs(hdmdes[i].Y_RPTL[hdmdes[i].Y_Lnpt-1].x)/30; 
			pt2[X] = hdmdes[i].Y_PTRPTL[4].x - k*(hdmdes[i].Y_PTRPTL[4].x-hdmdes[i-1].Y_PTRPTL[4].x);
			pt2[Y] = hdmdes[i].Y_PTRPTL[4].y - k*(hdmdes[i].Y_PTRPTL[4].y-hdmdes[i-1].Y_PTRPTL[4].y);
			pt1[X] = hdmdes[i].Y_PTRPTL[4].x;
			pt1[Y] = hdmdes[i].Y_PTRPTL[4].y;
			pt3[X] = hdmdes[i].Y_PTRPTL[hdmdes[i].Y_Lnpt-1].x;
			pt3[Y] = hdmdes[i].Y_PTRPTL[hdmdes[i].Y_Lnpt-1].y;
			Draw_BridgeSlop(pt1,pt2,pt3);
		}
		if(hdmdes[i].Y_YDMXZ==1)//右侧桥尾锥坡
		{
			if(hdmdes[i].Y_Rnpt==0)
				return;
			if(2*fabs(hdmdes[i].Y_RPTR[hdmdes[i].Y_Rnpt-1].x)/3 > 20)//2/3坡脚点至路肩点距离
				k = 1;
			else
				k =  fabs(hdmdes[i].Y_RPTR[hdmdes[i].Y_Rnpt-1].x)/30; 
			pt2[X] = hdmdes[i].Y_PTRPTR[4].x - k*(hdmdes[i].Y_PTRPTR[4].x-hdmdes[i-1].Y_PTRPTR[4].x);
			pt2[Y] = hdmdes[i].Y_PTRPTR[4].y - k*(hdmdes[i].Y_PTRPTR[4].y-hdmdes[i-1].Y_PTRPTR[4].y);
			pt1[X] = hdmdes[i].Y_PTRPTR[4].x;
			pt1[Y] = hdmdes[i].Y_PTRPTR[4].y;
			pt3[X] = hdmdes[i].Y_PTRPTR[hdmdes[i].Y_Rnpt-1].x;
			pt3[Y] = hdmdes[i].Y_PTRPTR[hdmdes[i].Y_Rnpt-1].y;
			Draw_BridgeSlop(pt1,pt2,pt3);
		}
	}
}


void CDrawPM::CalBriSlopPt(int i,int ZorY,AcGePoint3dArray &ptarr)
{
	ads_point pt1,pt2,pt3,Ljpt;
	AcGePoint3d CenPt,Spt,Ept;
	double k,ang,len,dx,dy,factor;
	xlpoint ptmp;

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

				double pt[2],prjml;
				len =  fabs(hdmdes[i].RHFL[8]);
				if(len<0.001)
					len=0.001;
				pm->CalPZXPT(hdmdes[i].cml,ang+180,len,pt,prjml);
				CenPt.x = pt[1];
				CenPt.y = pt[0];
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
				double pt[2],prjml;
				len =  fabs(hdmdes[i].RHFL[8]);
				if(len<0.001)
					len=0.001;
				pm->CalPZXPT(hdmdes[i].cml,ang+180,len,pt,prjml);
				CenPt.x = pt[1];
				CenPt.y = pt[0];
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
				double pt[2],prjml;
				len =  fabs(hdmdes[i].RHFR[8]);
				if(len<0.001)
					len=0.001;
				pm->CalPZXPT(hdmdes[i].cml,ang,len,pt,prjml);
				CenPt.x = pt[1];
				CenPt.y = pt[0];
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

				double pt[2],prjml;
				len =  fabs(hdmdes[i].RHFR[8]);
				if(len<0.001)
					len=0.001;
				pm->CalPZXPT(hdmdes[i].cml,ang,len,pt,prjml);
				CenPt.x = pt[1];
				CenPt.y = pt[0];
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
/*------------------------------------------*/
void CDrawPM::DrawLsxLine(ads_point bpt, double cita, double lsxang)
{
	ads_point fpt,tpt,mpt;
	double fwj,x,y,lenth=5.0,mw=0.5,w=0.0,len=1.0;
	AcGePoint3d spt,ept;

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"color", RTSHORT, 2, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"color", RTSHORT, 2, RTNONE);
#endif // VERSION_CAD2010	
	
	fwj = cita + lsxang;
	if(fwj > 2*PI)
		fwj = fwj - 2*PI;
	x = bpt[X] + lenth*cos(fwj);
	y = bpt[Y] + lenth*sin(fwj);
	tpt[Y] = x , tpt[X] = y;

	fwj = fwj + PI;
	if(fwj > 2*PI)
		fwj = fwj - 2*PI;
	x = bpt[X] + lenth*cos(fwj);
	y = bpt[Y] + lenth*sin(fwj);
	fpt[Y] = x , fpt[X] = y;

	mpt[X] = fpt[X] + 4.0*(tpt[X]-fpt[X])/5.0;
	mpt[Y] = fpt[Y] + 4.0*(tpt[Y]-fpt[Y])/5.0;
	spt.x = fpt[X],spt.y = fpt[Y];
	ept.x = tpt[X],ept.y = tpt[Y];
	makeline(spt,ept,2,0,L"小桥涵");
	//	acedCommandS(RTSTR,L"PLINE", RTPOINT, fpt, RTSTR,L"w", RTREAL, w, RTREAL, w,
	//		RTPOINT, tpt, RTSTR,L"", RTNONE);
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"PLINE", RTPOINT, mpt, RTSTR, L"w", RTREAL, mw, RTREAL, w,
		RTPOINT, tpt, RTSTR, L"", RTNONE);
	acedCommandS(RTSTR, L"color", RTSHORT, 4, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"PLINE", RTPOINT, mpt, RTSTR, L"w", RTREAL, mw, RTREAL, w,
		RTPOINT, tpt, RTSTR, L"", RTNONE);
	ads_command(RTSTR, L"color", RTSHORT, 4, RTNONE);
#endif // VERSION_CAD2010	

}

int CDrawPM::CalBjPt(double cml,double ang,int mark, ads_point pt1, ads_point pt2, double &x, double &y)
{
	double tx,ty;
	int i,icur=0,bcur,counter=0;

	for(i=0;i<NCRS;i++)
	{
		if(hdmdes[i].cml > cml)
		{
			icur = i;
			break;
		}
	}
	if(icur==0)
		return -1;

	bcur = icur;
	if(mark ==-1)//mark = 1 算左侧坡角线交点 
	{
		if(hdmdes[icur].Lnpt==0||hdmdes[icur-1].Lnpt==0)
			return -1;
		while(INTERS(&tx,&ty,hdmdes[icur].PTRPTL[hdmdes[icur].Lnpt-1].x,hdmdes[icur].PTRPTL[hdmdes[icur].Lnpt-1].y,
			hdmdes[icur-1].PTRPTL[hdmdes[icur-1].Lnpt-1].x,hdmdes[icur-1].PTRPTL[hdmdes[icur-1].Lnpt-1].y,
			pt1[X],pt1[Y],pt2[X],pt2[Y]))
		{

			if(ang > PI/2.0) //往前走
			{
				icur--;
				if(hdmdes[icur].Lnpt==0||hdmdes[icur-1].Lnpt==0)
					counter++;
			}
			else//往后走
			{
				icur++;
				if(hdmdes[icur].Lnpt==0||hdmdes[icur-1].Lnpt==0)
					counter++;
			}
			if(icur==1 || icur== NCRS-1)
				counter++;
			if(counter == 1)
			{
				counter++;
				icur = bcur;
				if(ang > PI/2.0)
					ang = PI/4.0;
				else //往前走
					ang = PI;
			}
			if(counter == 3)
				return -1;
		}
	}
	else //mark = 1 算右侧坡角线交点
	{
		if(hdmdes[icur].Rnpt==0)
			return -1;
		while(INTERS(&tx,&ty,hdmdes[icur].PTRPTR[hdmdes[icur].Rnpt-1].x,hdmdes[icur].PTRPTR[hdmdes[icur].Rnpt-1].y,
			hdmdes[icur-1].PTRPTR[hdmdes[icur-1].Rnpt-1].x,hdmdes[icur-1].PTRPTR[hdmdes[icur-1].Rnpt-1].y,
			pt1[X],pt1[Y],pt2[X],pt2[Y]))
		{
			if(ang > PI/2.0) //往前走
			{
				icur--;
				if(hdmdes[icur].Rnpt==0||hdmdes[icur-1].Rnpt==0)
					counter++;
			}
			else//往后走
			{
				icur++;
				if(hdmdes[icur].Rnpt==0||hdmdes[icur-1].Rnpt==0)
					counter++;
			}
			if(icur==1 || icur== NCRS-1)
				counter++;
			if(counter == 1)
			{
				counter++;
				icur = bcur;
				if(ang > PI/2.0)
					ang = PI/4.0;
				else //往前走
					ang = PI;
			}
			if(counter == 3)
				return -1;
		}
	}
	x = tx;
	y = ty;
	return 1;
}

int CDrawPM::DrawCulvet()
{
	ACHAR str[180],ckml[80],str1[80],str2[80],brg[80],mes[80],pszLabel[80];
	int i,k,len,j,pos;
	long n=0;
	double cml,ntype,dlen,lenth=100,ang,cita,lfwj,rfwj,dist=2.0,angle,fwj,width,wlen;
	ads_point pt,bpt,ptt,Lpt,Lpt1,Lpt2,Lpt3,Lpt4,Rpt,Rpt1,Rpt2,Rpt3,Rpt4,pt1,tpt;
	double lsxang;
	AcGePoint3d spt,ept;
	xlpoint  PZtmp;
	CString Kml;
	double dist1,dist2;

	pzLinep = pm;
	DBS.Read_XLDbs(pm->mdbname,L"小桥涵",pm->RoadName);
	_stprintf(pszLabel,L"正在绘小桥涵");
	acedSetStatusBarProgressMeter(pszLabel,0,100);

	for(i=0;i<DBS.NCU;i++)
	{
		pos=(int)i*100.0/DBS.NCU;
		acedSetStatusBarProgressMeterPos(pos);
		if(DBS.CUL[i].cml>=Scml&&DBS.CUL[i].cml<=Ecml)
		{			
			Kml = pm->XLC(DBS.CUL[i].cml);
			cml = DBS.CUL[i].cml;
			_stprintf(ckml,L"%s",Kml);
			_stprintf(str,L"%s%s",DBS.CUL[i].KKMS, DBS.CUL[i].QHMS);
			ads_printf(L"正在绘 %s %s......\n",ckml,str);
			ang = DBS.CUL[i].ang; 
			_tcscpy(str1,DBS.CUL[i].KKMS);
			_tcscpy(str2,DBS.CUL[i].QHMS);
			//湖南院标准 角度为线路前进方向顺时针旋转到流水线
			if(ang>360 || ang<0.0)
			{
				_stprintf(mes,L"第%d个桥涵角度有误!",i);
				ads_alert(mes);
				continue;
			}
			lsxang = ang;
			if(ang<0.1)
				ang = 0.0;
			else if(ang < 180)//右侧
				ang = 180 - ang;
			else
				ang = 360 - ang;
			lsxang = lsxang*PI/180.0;
			ang = ang*PI/180.0;			   
			//  wlen = _tcslen(str)*2.0;
			wlen = _tcslen(str)*texth*0.8;
			PZtmp.lc = cml;
			pm->xlpoint_pz(&PZtmp);
			cita = PZtmp.a;
			pt[X] = PZtmp.x;
			pt[Y] = PZtmp.y;
			lfwj = cita - ang;
			if(lfwj < 0.0)
				lfwj = lfwj + 2.0*PI;
			rfwj = lfwj + PI;
			if(rfwj > 2.0*PI)
				rfwj = rfwj - 2.0*PI;

			if(DBS.CUL[i].QHType=="盖板涵"||DBS.CUL[i].QHType=="箱涵")
			{
				len=F_num(str1,'-');
				for(k=0; k<len;k++)
					brg[k] = str1[k]; 
				brg[k]='\0';
				ntype = _wtof(brg);    
				for(j=len,k=0; j<_tcslen(str1);k++, j++)
					brg[k] = str1[j];  
				dlen = _wtof(brg);			   
				DrawLsxLine(pt, cita, lsxang);
				PZtmp.lc = cml-ntype*dlen/2.0;
				pm->xlpoint_pz(&PZtmp);	
				bpt[X] =  PZtmp.y;
				bpt[Y] =  PZtmp.x;
				ptt[X] = PZtmp.x + lenth*cos(lfwj);
				ptt[Y] = PZtmp.y + lenth*sin(lfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];

				if(CalBjPt(cml-ntype*dlen/2.0,ang,-1,bpt,pt1, Lpt[X], Lpt[Y])==-1)
					continue;

				Lpt1[X] = Lpt[X] + dist*sin(rfwj);
				Lpt1[Y] = Lpt[Y] + dist*cos(rfwj);
				angle=ads_angle(Lpt1 , Lpt);
				ads_polar(Lpt1 , angle+PI/8.0 , lenth , pt1);

				if(CalBjPt(cml-ntype*dlen/2.0,PI,-1,Lpt1,pt1, Lpt2[X], Lpt2[Y])==-1)
					continue;


				PZtmp.lc = cml+ntype*dlen/2.0;
				pm->xlpoint_pz(&PZtmp);	
				bpt[X] = PZtmp.y;
				bpt[Y] = PZtmp.x;
				ptt[X] = PZtmp.x + lenth*cos(lfwj);
				ptt[Y] = PZtmp.y + lenth*sin(lfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];

				if(CalBjPt(cml+ntype*dlen/2.0,ang,-1,bpt,pt1, Lpt4[X], Lpt4[Y])==-1)
					continue;

				Lpt[X] = Lpt4[X] + dist*sin(rfwj);
				Lpt[Y] = Lpt4[Y] + dist*cos(rfwj);
				angle=ads_angle(Lpt , Lpt4);
				ads_polar(Lpt , angle-PI/8.0 , lenth , pt1);//mark = -1 算左侧坡角线交点 

				if(CalBjPt(cml+ntype*dlen/2.0,0,-1,Lpt,pt1, Lpt3[X], Lpt3[Y])==-1)
					continue;

				ads_polar(Lpt3 , angle-PI/8.0 , 0.1 , pt1);
				ads_polar(Lpt3 , angle-PI/8.0+PI , 0.1 , tpt);

				AcGePoint3d p1,p2,p3,p4;
				p1.x = pt1[X];p1.y = pt1[Y];
				p2.x = Lpt[X];p2.y = Lpt[Y];
				p3.x = Lpt1[X];p3.y = Lpt1[Y];
				p4.x = Lpt2[X];p4.y = Lpt2[Y];
				ads_point bkpt1,bkpt2,bkpt3,bkpt4;
				bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
				bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
				bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
				bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
				int result;
				result = 1234;

				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);			   

				//				   for(int k=2;k<5;k++)
				//				   {
				//					   //								flag_k=k;//是否显示进度条。<2显示
				//					   BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);  
				//					   result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
				//					   if(result < 0)
				//						   break;
				//				   }
				k = 5;
				k+=2;
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
				dist1 = caldistance(bkpt1,bkpt2);
				dist2 = caldistance(bkpt2,bkpt3);
				if (dist1 > 0.01 && dist2 > 0.01)
				{
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR,L"erase",RTSTR,L"WP",RTPOINT,bkpt1,RTPOINT,bkpt2,RTPOINT,bkpt3,RTPOINT,bkpt4,RTSTR,L"",RTSTR,L"",0);		
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
				}
						   

				PZtmp.lc = cml-ntype*dlen/2.0;
				pm->xlpoint_pz(&PZtmp);			
				bpt[X] = PZtmp.y;
				bpt[Y] = PZtmp.x;
				ptt[X] = PZtmp.x + lenth*cos(rfwj);
				ptt[Y] = PZtmp.y + lenth*sin(rfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];
				if(CalBjPt(cml-ntype*dlen/2.0,PI-ang,1,bpt,pt1, Rpt[X], Rpt[Y])==-1)
					continue;
				Rpt1[X] = Rpt[X] + dist*sin(lfwj);
				Rpt1[Y] = Rpt[Y] + dist*cos(lfwj);
				angle=ads_angle(Rpt1 , Rpt); //mark = 1 算右侧坡角线交点
				ads_polar(Rpt1 , angle-PI/8.0 , lenth , pt1);
				if(CalBjPt(cml-ntype*dlen/2.0,PI,1,Rpt1,pt1, Rpt2[X], Rpt2[Y])==-1)
					continue;

				PZtmp.lc = cml+ntype*dlen/2.0;
				pm->xlpoint_pz(&PZtmp);	
				bpt[X] = PZtmp.y;
				bpt[Y] = PZtmp.x;
				ptt[X] = PZtmp.x + lenth*cos(rfwj);
				ptt[Y] = PZtmp.y + lenth*sin(rfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];
				if(CalBjPt(cml+ntype*dlen/2.0,PI-ang,1,bpt,pt1, Rpt4[X], Rpt4[Y])==-1)
					continue;
				Rpt[X] = Rpt4[X] + dist*sin(lfwj);
				Rpt[Y] = Rpt4[Y] + dist*cos(lfwj);
				angle=ads_angle(Rpt , Rpt4);
				ads_polar(Rpt , angle+PI/8.0 , lenth , pt1);
				if(CalBjPt(cml+ntype*dlen/2.0,0,1,Rpt,pt1, Rpt3[X], Rpt3[Y])==-1)
					continue;

				ads_polar(Rpt3 , angle+PI/8.0 , 0.1 , pt1);
				ads_polar(Rpt3 , angle+PI/8.0+PI , 0.1 , tpt);


				p1.x = pt1[X];p1.y = pt1[Y];
				p2.x = Rpt[X];p2.y = Rpt[Y];
				p3.x = Rpt1[X];p3.y = Rpt1[Y];
				p4.x = Rpt2[X];p4.y = Rpt2[Y];
				bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
				bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
				bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
				bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4);			   
				result = 1234;
				//			   for(k=2;k<5;k++)
				//			   {
				//				   //								flag_k=k;//是否显示进度条。<2显示
				//				   BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
				//				   result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
				//				   if(result < 0)
				//					   break;
				//			   }
				k = 5;
				k+=2;
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
				dist1 = caldistance(bkpt1,bkpt2);
				dist2 = caldistance(bkpt2,bkpt3);
				if (dist1 > 0.01 && dist2 > 0.01)
				{
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
				}
							 

				line0(Lpt1[X],Lpt1[Y],Lpt2[X],Lpt2[Y],4,L"小桥涵");
				line0(Lpt[X],Lpt[Y],Lpt3[X],Lpt3[Y],4,L"小桥涵");
				line0(Rpt1[X],Rpt1[Y],Rpt2[X],Rpt2[Y],4,L"小桥涵");
				line0(Rpt[X],Rpt[Y],Rpt3[X],Rpt3[Y],4,L"小桥涵");
				line0(Lpt1[X],Lpt1[Y],Lpt[X],Lpt[Y],4,L"小桥涵");
				line0(Rpt1[X],Rpt1[Y],Rpt[X],Rpt[Y],4,L"小桥涵");
				spt.x = Lpt1[X],spt.y = Lpt1[Y];
				ept.x = Rpt1[X],ept.y = Rpt1[Y];
				makeline(spt,ept,4,0,L"小桥涵",L"HIDDEN2");
				spt.x = Lpt[X],spt.y = Lpt[Y];
				ept.x = Rpt[X],ept.y = Rpt[Y];
				makeline(spt,ept,4,0,L"小桥涵",L"HIDDEN2");
				//			   linewd (Lpt1[X],Lpt1[Y], Rpt1[X], Rpt1[Y], 0.0);
				//			   linewd (Lpt[X],Lpt[Y], Rpt[X], Rpt[Y], 0.0);

				fwj = cita-PI/2.0;
				if(fwj < 0.0)
					fwj = fwj + 2.0*PI;			   
				Lpt[X] = Lpt1[Y];
				Lpt[Y] = Lpt1[X];
				width = caldistance(pt,Lpt)+20;			   
				pt1[X] = pt[X] + width*cos(fwj);
				pt1[Y] = pt[Y] + width*sin(fwj);
				// Lpt[X] = pt[X] + (width+4)*cos(fwj);
				// Lpt[Y] = pt[Y] + (width+4)*sin(fwj);
				Lpt[X] = pt[X] + (width+2+0.5*texth)*cos(fwj);
				Lpt[Y] = pt[Y] + (width+2+0.5*texth)*sin(fwj);
				Lpt[X] = Lpt[X] + wlen*cos(cita)/2.0;
				Lpt[Y] = Lpt[Y] + wlen*sin(cita)/2.0;
				//Lpt1[X] = pt[X] +(width+8)*cos(fwj);
				//Lpt1[Y] = pt[Y] +(width+8)*sin(fwj);
				Lpt1[X] = pt[X] +(width+4+1.5*texth)*cos(fwj);
				Lpt1[Y] = pt[Y] +(width+4+1.5*texth)*sin(fwj);
				Lpt1[X] = Lpt1[X] + wlen*cos(cita)/2.0;
				Lpt1[Y] = Lpt1[Y] + wlen*sin(cita)/2.0;
				bpt[X] = pt[Y];
				bpt[Y] = pt[X];
				pt[X] = pt1[X] + wlen*cos(cita);
				pt[Y] = pt1[Y] + wlen*sin(cita);
				lfwj = pt1[Y];
				pt1[Y] = pt1[X];
				pt1[X] = lfwj;
				lfwj = pt[Y];
				pt[Y] = pt[X];
				pt[X] = lfwj;
				line0(bpt[X],bpt[Y],pt1[X],pt1[Y],4,L"小桥涵");
				line0(pt1[X],pt1[Y],pt[X],pt[Y],4,L"小桥涵");
				lfwj = Lpt[Y];
				Lpt[Y] = Lpt[X];
				Lpt[X] = lfwj;
				lfwj = Lpt1[Y];
				Lpt1[Y] = Lpt1[X];
				Lpt1[X] = lfwj;
				angle=ads_angle(pt1 , pt);
				//			   angle = angle*180.0/PI;
				spt.x = Lpt1[X],spt.y=Lpt1[Y];

				//  maketext(spt,ckml,angle,2,4,2,L"HZ",L"小桥涵");
				//			   maketext(spt,ckml,angle,texth,4,2,L"HZ",L"小桥涵");
				maketext(spt,ckml,angle,texth,4,2,L"HZ",L"小桥涵");
				spt.x = Lpt[X],spt.y=Lpt[Y];
				// maketext(spt,str,angle,2,4,2,L"HZ",L"小桥涵");
				//			   maketext(spt,str,angle,texth,4,2,L"HZ",L"小桥涵");
				maketext(spt,str,angle,texth,4,2,L"HZ",L"小桥涵");
			}
			else if(DBS.CUL[i].QHType=="立体交叉"||DBS.CUL[i].QHType=="人行天桥")
			{
				PZtmp.lc = cml-2.0;
				pm->xlpoint_pz(&PZtmp);			   
				bpt[X] = PZtmp.y;
				bpt[Y] = PZtmp.x;
				ptt[X] = PZtmp.x + lenth*cos(lfwj);
				ptt[Y] = PZtmp.y + lenth*sin(lfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];
				if(CalBjPt(cml-2.0,ang,-1,bpt,pt1, Lpt[X], Lpt[Y])==-1)
					continue;
				angle=ads_angle(bpt , Lpt);
				ads_polar(Lpt , angle+PI/8.0 , 1.5 , Lpt1);
				PZtmp.lc = cml+2.0;
				pm->xlpoint_pz(&PZtmp);			   
				bpt[X] = PZtmp.y;
				bpt[Y] = PZtmp.x;
				ptt[X] = PZtmp.x + lenth*cos(lfwj);
				ptt[Y] = PZtmp.y + lenth*sin(lfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];
				if(CalBjPt(cml+2.0,ang,-1,bpt,pt1, Lpt2[X], Lpt2[Y])==-1)
					continue;
				angle=ads_angle(bpt , Lpt2);		    
				ads_polar(Lpt2 , angle-PI/8.0 , 1.5 , Lpt3);
				ads_polar(Lpt2 , angle-PI/8.0-PI,0.1, Lpt4);			   

				PZtmp.lc = cml-2.0;
				pm->xlpoint_pz(&PZtmp);			   
				bpt[X] = PZtmp.y;
				bpt[Y] = PZtmp.x;
				ptt[X] = PZtmp.x + lenth*cos(rfwj);
				ptt[Y] = PZtmp.y + lenth*sin(rfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];
				if(CalBjPt(cml-2.0,PI-ang,1,bpt,pt1, Rpt[X], Rpt[Y])==-1)
					continue;
				angle=ads_angle(bpt , Rpt);
				ads_polar(Rpt , angle-PI/8.0 , 1.5 , Rpt1);

				PZtmp.lc = cml+2.0;
				pm->xlpoint_pz(&PZtmp);
				bpt[X] = PZtmp.y;
				bpt[Y] = PZtmp.x;
				ptt[X] = PZtmp.x + lenth*cos(rfwj);
				ptt[Y] = PZtmp.y + lenth*sin(rfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];
				if(CalBjPt(cml+2.0,PI-ang,1,bpt,pt1, Rpt2[X], Rpt2[Y])==-1)
					continue;
				angle=ads_angle(bpt , Rpt2);
				ads_polar(Rpt2 , angle+PI/8.0 , 1.5 , Rpt3);
				ads_polar(Rpt2 , angle+PI/8.0-PI,0.1, Rpt4);

				AcGePoint3d p1,p2,p3,p4;
				p1.x = Lpt1[X];p1.y = Lpt1[Y];
				p2.x = Rpt1[X];p2.y = Rpt1[Y];
				p3.x = Rpt3[X];p3.y = Rpt3[Y];
				p4.x = Lpt3[X];p4.y = Lpt3[Y];
				ads_point bkpt1,bkpt2,bkpt3,bkpt4;
				bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
				bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
				bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
				bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
				int result;
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,1234);			   
				result = 1234;
				//			   for(int k=2;k<5;k++)
				//			   {
				//				   //								flag_k=k;//是否显示进度条。<2显示
				//				   BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
				//				   result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
				//				   if(result < 0)
				//					   break;
				//			   }
				k = 5;
				k+=2;
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
				dist1 = caldistance(bkpt1,bkpt2);
				dist2 = caldistance(bkpt2,bkpt3);
				if (dist1 > 0.01 && dist2 > 0.01)
				{
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
				}
					
				//			   acedCommandS(RTSTR,L"erase",RTSTR,L"WP",RTPOINT,Lpt1,RTPOINT,Rpt1,RTPOINT,Rpt3,RTPOINT,Lpt3,RTSTR,L"",RTSTR,L"",0);
				line0(Lpt1[X],Lpt1[Y],Lpt[X],Lpt[Y],4,L"小桥涵");
				line0(Lpt2[X],Lpt2[Y],Lpt3[X],Lpt3[Y],4,L"小桥涵");
				line0(Rpt1[X],Rpt1[Y],Rpt[X],Rpt[Y],4,L"小桥涵");
				line0(Rpt2[X],Rpt2[Y],Rpt3[X],Rpt3[Y],4,L"小桥涵");
				line0(Lpt[X],Lpt[Y],Rpt[X],Rpt[Y],4,L"小桥涵");
				line0(Lpt2[X],Lpt2[Y],Rpt2[X],Rpt2[Y],4,L"小桥涵");

				fwj = cita-PI/2.0;
				if(fwj < 0.0)
					fwj = fwj + 2.0*PI;

				Lpt[X] = Lpt1[Y];
				Lpt[Y] = Lpt1[X];
				width = caldistance(pt,Lpt)+20;

				pt1[X] = pt[X] + width*cos(fwj);
				pt1[Y] = pt[Y] + width*sin(fwj);
				Lpt[X] = pt[X] + (width+2+0.5*texth)*cos(fwj);
				Lpt[Y] = pt[Y] + (width+2+0.5*texth)*sin(fwj);
				Lpt[X] = Lpt[X] + wlen*cos(cita)/2.0;
				Lpt[Y] = Lpt[Y] + wlen*sin(cita)/2.0;
				Lpt1[X] = pt[X] +(width+4+1.5*texth)*cos(fwj);
				Lpt1[Y] = pt[Y] +(width+4+1.5*texth)*sin(fwj);
				Lpt1[X] = Lpt1[X] +wlen*cos(cita)/2.0;
				Lpt1[Y] = Lpt1[Y] + wlen*sin(cita)/2.0;
				bpt[X] = pt[Y];
				bpt[Y] = pt[X];
				pt[X] = pt1[X] + wlen*cos(cita);
				pt[Y] = pt1[Y] + wlen*sin(cita);
				lfwj = pt1[Y];
				pt1[Y] = pt1[X];
				pt1[X] = lfwj;
				lfwj = pt[Y];
				pt[Y] = pt[X];
				pt[X] = lfwj;
				line0(bpt[X],bpt[Y],pt1[X],pt1[Y],4,L"小桥涵");
				line0(pt1[X],pt1[Y],pt[X],pt[Y],4,L"小桥涵");
				lfwj = Lpt[Y];
				Lpt[Y] = Lpt[X];
				Lpt[X] = lfwj;
				lfwj = Lpt1[Y];
				Lpt1[Y] = Lpt1[X];
				Lpt1[X] = lfwj;
				angle=ads_angle(pt1 , pt);
				spt.x = Lpt1[X],spt.y=Lpt1[Y];
				//  maketext(spt,ckml,angle,2,4,3,L"HZ",L"小桥涵");
				//			   maketext(spt,ckml,angle,texth,4,3,L"HZ",L"小桥涵");
				maketext(spt,ckml,angle,texth,4,3,L"HZ",L"小桥涵");
				spt.x = Lpt[X],spt.y=Lpt[Y];
				//			   maketext(spt,str,angle,2,4,3,L"HZ",L"小桥涵");	  
				//			   maketext(spt,str,angle,texth,4,3,L"HZ",L"小桥涵");	  
				maketext(spt,str,angle,texth,4,3,L"HZ",L"小桥涵");	  

			}
			else if(DBS.CUL[i].QHType=="圆管涵"||DBS.CUL[i].QHType=="拱涵")
			{
				len=F_num(str1,'-');
				for(k=0; k<len;k++)
					brg[k] = str1[k]; 
				brg[k]='\0';
				ntype = _wtof(brg);    
				for(j=len,k=0; j<_tcslen(str1);k++, j++)
					brg[k] = str1[j];  
				dlen = _wtof(brg);

				DrawLsxLine(pt, cita, lsxang);			   

				bpt[X] = pt[Y];
				bpt[Y] = pt[X];
				ptt[X] = pt[X] + lenth*cos(lfwj);
				ptt[Y] = pt[Y] + lenth*sin(lfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];
				if(CalBjPt(cml,ang,-1,bpt,pt1, Lpt[X], Lpt[Y])==-1)
					continue;

				ptt[X] = pt[X] + lenth*cos(rfwj);
				ptt[Y] = pt[Y] + lenth*sin(rfwj);
				pt1[X] = ptt[Y];
				pt1[Y] = ptt[X];
				if(CalBjPt(cml,PI-ang,1,bpt,pt1, Rpt[X], Rpt[Y])==-1)
					continue;

				Lpt1[X] = Lpt[X] + dist*sin(rfwj);
				Lpt1[Y] = Lpt[Y] + dist*cos(rfwj);
				angle=ads_angle(Lpt1 , Lpt);
				ads_polar(Lpt1 , angle+PI/8.0 , lenth , pt1);
				if(CalBjPt(cml,PI,-1,Lpt1,pt1, Lpt2[X], Lpt2[Y])==-1)
					continue;

				ads_polar(Lpt1 , angle-PI/8.0 , lenth , pt1);
				if(CalBjPt(cml,0,-1,Lpt1,pt1, Lpt3[X], Lpt3[Y])==-1)
					continue;

				Rpt1[X] = Rpt[X] + dist*sin(lfwj);
				Rpt1[Y] = Rpt[Y] + dist*cos(lfwj);
				angle=ads_angle(Rpt1 , Rpt);
				ads_polar(Rpt1 , angle+PI/8.0 , lenth , pt1);
				if(CalBjPt(cml,0,1,Rpt1,pt1, Rpt2[X], Rpt2[Y])==-1)
					continue;

				ads_polar(Rpt1 , angle-PI/8.0 , lenth , pt1);
				if(CalBjPt(cml,PI,1,Rpt1,pt1, Rpt3[X], Rpt3[Y])==-1)
					continue;

				angle=ads_angle(Lpt1 , Lpt);
				ads_polar(Lpt2 , angle+PI/8.0 , 0.1 , pt);
				ads_polar(Lpt2 , angle+PI/8.0+PI , 0.1 , tpt);			   
				ads_polar(Lpt3 , angle-PI/8.0 , 1.0 , pt1);			   

				AcGePoint3d p1,p2,p3,p4;
				p1.x = pt[X];p1.y = pt[Y];
				p2.x = pt1[X];p2.y = pt1[Y];
				p3.x = Lpt1[X];p3.y = Lpt1[Y];
				p4.x = Lpt1[X];p4.y = Lpt1[Y];
				ads_point bkpt1,bkpt2,bkpt3,bkpt4;
				bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
				bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
				bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
				bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
				int result;
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4);			   
				result = 1234;
				//			   for(int k=2;k<5;k++)
				//			   {
				//				   //								flag_k=k;//是否显示进度条。<2显示
				//				   BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
				//				   result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
				//				   if(result < 0)
				//					   break;
				//			   }
				k=5;
				k+=2;
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);

				dist1 = caldistance(bkpt1,bkpt2);
				dist2 = caldistance(bkpt2,bkpt3);
				if (dist1 > 0.01 && dist2 > 0.01)
				{
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
				}
					

				angle=ads_angle(Rpt1 , Rpt);
				ads_polar(Rpt2 , angle+PI/8.0 , 0.1 , pt);
				ads_polar(Rpt2 , angle+PI/8.0+PI , 0.1 , tpt);			   
				ads_polar(Rpt3 , angle-PI/8.0 , 1.0 , pt1);            

				p1.x = pt[X];p1.y = pt[Y];
				p2.x = pt1[X];p2.y = pt1[Y];
				p3.x = Rpt1[X];p3.y = Rpt1[Y];
				p4.x = Rpt1[X];p4.y = Rpt1[Y];			 
				bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
				bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
				bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
				bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4);			   
				result = 1234;

				for(k=2;k<5;k++)
				{
					//								flag_k=k;//是否显示进度条。<2显示
					BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
					result = BAS_DRAW_FUN::AutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
					if(result < 0)
						break;
				}
				k+=2;
				BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);

				dist1 = caldistance(bkpt1,bkpt2);
				dist2 = caldistance(bkpt2,bkpt3);
				if (dist1 > 0.01 && dist2 > 0.01)
				{
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, bkpt1, RTPOINT, bkpt2, RTPOINT, bkpt3, RTPOINT, bkpt4, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
				}
					


				line0(Lpt1[X],Lpt1[Y],Lpt2[X],Lpt2[Y],4,L"小桥涵");
				line0(Lpt1[X],Lpt1[Y],Lpt3[X],Lpt3[Y],4,L"小桥涵");
				line0(Rpt1[X],Rpt1[Y],Rpt2[X],Rpt2[Y],4,L"小桥涵");
				line0(Rpt1[X],Rpt1[Y],Rpt3[X],Rpt3[Y],4,L"小桥涵");
				spt.x = Lpt1[X],spt.y = Lpt1[Y];
				ept.x = Rpt1[X],ept.y = Rpt1[Y];
				makeline(spt,ept,4,0,L"小桥涵",L"HIDDEN2");		

				fwj = cita-PI/2.0;
				if(fwj < 0.0)
					fwj = fwj + 2.0*PI;			   
				Lpt[X] = Lpt1[Y];
				Lpt[Y] = Lpt1[X];
				PZtmp.lc = cml;
				pm->xlpoint_pz(&PZtmp);
				pt[X] = PZtmp.x,pt[Y]=PZtmp.y;
				width = caldistance(pt,Lpt)+20;
				pt1[X] = pt[X] + width*cos(fwj);
				pt1[Y] = pt[Y] + width*sin(fwj);
				Lpt[X] = pt[X] + (width+2+0.5*texth)*cos(fwj);
				Lpt[Y] = pt[Y] + (width+2+0.5*texth)*sin(fwj);
				Lpt[X] = Lpt[X] + wlen*cos(cita)/2.0;
				Lpt[Y] = Lpt[Y] + wlen*sin(cita)/2.0;
				Lpt1[X] = pt[X] +(width+4+1.5*texth)*cos(fwj);
				Lpt1[Y] = pt[Y] +(width+4+1.5*texth)*sin(fwj);
				Lpt1[X] = Lpt1[X] +wlen*cos(cita)/2.0;
				Lpt1[Y] = Lpt1[Y] + wlen*sin(cita)/2.0;
				bpt[X] = pt[Y];
				bpt[Y] = pt[X];
				pt[X] = pt1[X] + wlen*cos(cita);
				pt[Y] = pt1[Y] + wlen*sin(cita);
				lfwj = pt1[Y];
				pt1[Y] = pt1[X];
				pt1[X] = lfwj;
				lfwj = pt[Y];
				pt[Y] = pt[X];
				pt[X] = lfwj;
				line0(bpt[X],bpt[Y],pt1[X],pt1[Y],4,L"小桥涵");
				line0(pt1[X],pt1[Y],pt[X],pt[Y],4,L"小桥涵");
				lfwj = Lpt[Y];
				Lpt[Y] = Lpt[X];
				Lpt[X] = lfwj;
				lfwj = Lpt1[Y];
				Lpt1[Y] = Lpt1[X];
				Lpt1[X] = lfwj;
				angle=ads_angle(pt1 , pt);
				spt.x = Lpt1[X],spt.y=Lpt1[Y];

				// maketext(spt,ckml,angle,2,4,3,L"HZ",L"小桥涵");
				//               maketext(spt,ckml,angle,texth,4,3,L"HZ",L"小桥涵");
				maketext(spt,ckml,angle,texth,4,3,L"HZ",L"小桥涵");
				spt.x = Lpt[X],spt.y=Lpt[Y];
				//			   maketext(spt,str,angle,texth,4,3,L"HZ",L"小桥涵");
				maketext(spt,str,angle,texth,4,3,L"HZ",L"小桥涵");
			}
		}
	}
	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
	return 1;
}

int CDrawPM::drawfillandcut(int i,int ZorY,double qdeltah,double hdeltah,double qtw,double htw,int qdmxz,int hdmxz,int qnpt,int hnpt,AcGePoint3dArray &qPtarr, AcGePoint3dArray &hPtarr,bool& tflag,bool& wflag)
{
	//	CRSDAT MidCrs;
	AcGePoint3d Pt,spt,ept;
	xlpoint  PZtmp; 
	AcGePoint3dArray MidArr;
	int MidNum,j;
	double fwj,lmk;
	ads_point bpt;	
	/*
	if(ZorY <0)
	{
	qdeltah = hdmdes[i-1].Ldeltah;
	hdeltah = hdmdes[i].Ldeltah;
	}
	else
	{
	qdeltah = hdmdes[i-1].Rdeltah;
	hdeltah = hdmdes[i].Rdeltah;
	}*/
	//ads_printf(L"cml=%lf\n",hdmdes[i].cml);
	double ang,cml,len,dx,dy;
	double OffsetByXLZX = 0.0;
	if(qtw<0.001 && htw<0.001 &&qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)
		//前后断面都为填方且前断面为一般断面
	{
		if(hdmxz==-1)//桥头
		{
			OffsetByXLZX = hdmdes[i].OffsetByXLZX;
			ang  = FindBriAng(hdmdes[i].cml);		   
			if(fabs(ang-90)<0.1)
				DrawPartFillBetweenTwoMl(hdmdes[i-1].cml,hdmdes[i].cml,ZorY,qnpt,hnpt,qPtarr,hPtarr,tflag);
			else
			{
				PZtmp.lc = hdmdes[i].cml;
				pm->xlpoint_pz(&PZtmp);
				if(ZorY==-1)
				{
					lmk = GetLMK(hdmdes[i].cml,ZLMK,NZLMK)/sin(ang*PI/180);
					//				   fwj = PZtmp.a - (180-ang)*PI/180;
					fwj = PZtmp.a + ang*PI/180;	
				}
				else
				{
					lmk = GetLMK(hdmdes[i].cml,YLMK,NYLMK)/sin(ang*PI/180);
					fwj = PZtmp.a + ang*PI/180;		   
				}
				// 			   bpt[X] = PZtmp.x + lmk*cos(fwj);
				// 			   bpt[Y] = PZtmp.y + lmk*sin(fwj);
				bpt[X] = PZtmp.x + (ZorY * lmk + OffsetByXLZX) * cos(fwj);
				bpt[Y] = PZtmp.y + (ZorY * lmk + OffsetByXLZX) * sin(fwj);
				cml = pm->PROJ_ML(bpt[X],bpt[Y]);
				/*
				dx = hPtarr[hnpt-1].x-hPtarr[3].x;
				dy = hPtarr[hnpt-1].y-hPtarr[3].y;
				len = sqrt(dx*dx+dy*dy);
				bpt[X] = PZtmp.x + (lmk+len)*cos(fwj);
				bpt[Y] = PZtmp.y + (lmk+len)*sin(fwj);
				cml2 = pm->PROJ_ML(bpt[X],bpt[Y]);
				cml = cml1<cml2 ? cml1 : cml2;*/
				if(MidArr.length() >0)
					MidArr.removeSubArray(0, MidArr.length()-1);

				PZtmp.lc = cml;
				if (ZorY<0)
					CalOffsetByXLZX(cml,NZLMK,ZLMK,OffsetByXLZX);
				else
					CalOffsetByXLZX(cml,NYLMK,YLMK,OffsetByXLZX);
				pm->xlpoint_pz(&PZtmp);
				for(j=0;j<hnpt;j++)
				{
					dx = hPtarr[j].x-hPtarr[0].x;
					dy = hPtarr[j].y-hPtarr[0].y;
					len = sqrt(dx*dx+dy*dy);

					// 					   Pt.x =  PZtmp.y + len*sin(PZtmp.a+ZorY*PI/2);
					// 					   Pt.y =  PZtmp.x + len*cos(PZtmp.a+ZorY*PI/2);
					Pt.x =  PZtmp.y + (ZorY * len + OffsetByXLZX) * sin(PZtmp.a+PI/2);
					Pt.y =  PZtmp.x + (ZorY * len + OffsetByXLZX) * cos(PZtmp.a+PI/2);			
					Pt.z = hPtarr[j].z;
					MidArr.append(Pt);
				}
				if(cml > hdmdes[i-1].cml)
					DrawPartFillBetweenTwoMl(hdmdes[i-1].cml,cml,ZorY,qnpt,hnpt,qPtarr,MidArr,tflag);
			}
		}
		else if(qdmxz==1)//桥尾
		{
			OffsetByXLZX = hdmdes[i].OffsetByXLZX;
			ang  = FindBriAng(hdmdes[i-1].cml);
			if(fabs(ang-90)<0.1)
				DrawPartFillBetweenTwoMl(hdmdes[i-1].cml,hdmdes[i].cml,ZorY,qnpt,hnpt,qPtarr,hPtarr,tflag);
			else
			{
				PZtmp.lc = hdmdes[i-1].cml;
				pm->xlpoint_pz(&PZtmp);
				if(ZorY==-1)
				{
					lmk = GetLMK(hdmdes[i-1].cml,ZLMK,NZLMK)/sin(ang*PI/180);
					//				   fwj = PZtmp.a - (180-ang)*PI/180;
					fwj = PZtmp.a + ang*PI/180;	
				}
				else
				{
					lmk = GetLMK(hdmdes[i-1].cml,YLMK,NYLMK)/sin(ang*PI/180);
					fwj = PZtmp.a + ang*PI/180;		   
				}
				// 			   bpt[X] = PZtmp.x + lmk*cos(fwj);
				// 			   bpt[Y] = PZtmp.y + lmk*sin(fwj);
				bpt[X] = PZtmp.x + (ZorY * lmk + OffsetByXLZX) * cos(fwj);
				bpt[Y] = PZtmp.y + (ZorY * lmk + OffsetByXLZX) * sin(fwj);

				cml = pm->PROJ_ML(bpt[X],bpt[Y]);
				/*
				dx = qPtarr[hnpt-1].x-qPtarr[3].x;
				dy = qPtarr[hnpt-1].y-qPtarr[3].y;
				len = sqrt(dx*dx+dy*dy);
				bpt[X] = PZtmp.x + (lmk+len)*cos(fwj);
				bpt[Y] = PZtmp.y + (lmk+len)*sin(fwj);
				cml2 = pm->PROJ_ML(bpt[X],bpt[Y]);
				cml = cml1>cml2 ? cml1 : cml2;*/
				PZtmp.lc = cml;
				if (ZorY<0)
					CalOffsetByXLZX(cml,NZLMK,ZLMK,OffsetByXLZX);
				else
					CalOffsetByXLZX(cml,NYLMK,YLMK,OffsetByXLZX);
				pm->xlpoint_pz(&PZtmp);

				if(MidArr.length() >0)
					MidArr.removeSubArray(0, MidArr.length()-1);
				for(j=0;j<qnpt;j++)
				{
					dx = qPtarr[j].x-qPtarr[0].x;
					dy = qPtarr[j].y-qPtarr[0].y;
					len = sqrt(dx*dx+dy*dy);

					// 					   Pt.x =  PZtmp.y + len*sin(PZtmp.a+ZorY*PI/2);
					// 					   Pt.y =  PZtmp.x + len*cos(PZtmp.a+ZorY*PI/2);
					Pt.x =  PZtmp.y + (ZorY * len + OffsetByXLZX) * sin(PZtmp.a+PI/2);
					Pt.y =  PZtmp.x + (ZorY * len + OffsetByXLZX) * cos(PZtmp.a+PI/2);
					Pt.z = qPtarr[j].z;
					MidArr.append(Pt);
				}
				if(cml < hdmdes[i].cml)
					DrawPartFillBetweenTwoMl(cml,hdmdes[i].cml,ZorY,qnpt,hnpt,MidArr,hPtarr,tflag);
			}
		}
		else
		{
			DrawPartFillBetweenTwoMl(hdmdes[i-1].cml,hdmdes[i].cml,ZorY,qnpt,hnpt,qPtarr,hPtarr,tflag);				
		}
	}
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
		Pt.x = (hPtarr[4].x + k*qPtarr[4].x)/(1+k);
		Pt.y = (hPtarr[4].y + k*qPtarr[4].y)/(1+k);			
		Pt.z = (hPtarr[4].z + k*qPtarr[4].z)/(1+k);
		for(j=0;j<MidNum;j++)
			MidArr.append(Pt);
		//绘挖方hdmdes[i-1].cml --  MidMl;
		DrawCutPartBetweenTwoMl(hdmdes[i-1].cml,MidMl,ZorY,qnpt,MidNum,qPtarr,MidArr,wflag);
		//绘填方MidMl-hdmdes[i].cml;
		DrawPartFillBetweenTwoMl(MidMl,hdmdes[i].cml,ZorY,MidNum,hnpt,MidArr,hPtarr,tflag);								
	}
	else if(fabs(1.0*qdmxz)>0 || fabs(1.0*hdmxz)>0)
	{

		OffsetByXLZX = hdmdes[i-1].OffsetByXLZX;
		PZtmp.lc = hdmdes[i-1].cml;
		pm->xlpoint_pz(&PZtmp);
		if(ZorY==-1)//左侧
		{
			lmk = GetLMK(hdmdes[i-1].cml,ZLMK,NZLMK);
			// 			fwj = PZtmp.a - PI/2.0;
			// 			if(fwj < 0.0)
			// 				fwj = fwj + 2.0*PI;
			fwj = PZtmp.a + PI/2.0;
			if(fwj > 2.0*PI)
				fwj = fwj - 2.0*PI;
		}
		else
		{
			lmk = GetLMK(hdmdes[i-1].cml,YLMK,NYLMK);
			fwj = PZtmp.a + PI/2.0;
			if(fwj > 2.0*PI)
				fwj = fwj - 2.0*PI;
		}

		bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
		// 		spt.x = bpt[X] + lmk*sin(fwj);
		// 		spt.y = bpt[Y] + lmk*cos(fwj);
		spt.x = bpt[X] + (ZorY * lmk + OffsetByXLZX) * sin(fwj);
		spt.y = bpt[Y] + (ZorY * lmk + OffsetByXLZX) * cos(fwj);

		PZtmp.lc = hdmdes[i].cml;
		OffsetByXLZX = hdmdes[i].OffsetByXLZX;
		pm->xlpoint_pz(&PZtmp);
		if(ZorY==-1)
		{
			lmk = GetLMK(hdmdes[i].cml,ZLMK,NZLMK);
			fwj = PZtmp.a + PI/2.0;
			if(fwj > 2.0*PI)
				fwj = fwj - 2.0*PI;
			// 		   fwj = PZtmp.a - PI/2.0;
			// 		   if(fwj < 0.0)
			// 			   fwj = fwj + 2.0*PI;
		}
		else
		{
			lmk = GetLMK(hdmdes[i].cml,YLMK,NYLMK);
			fwj = PZtmp.a + PI/2.0;
			if(fwj > 2.0*PI)
				fwj = fwj - 2.0*PI;
		}

		bpt[X] = PZtmp.y; bpt[Y] = PZtmp.x;
		// 		ept.x = bpt[X] + lmk*sin(fwj);
		// 		ept.y = bpt[Y] + lmk*cos(fwj);
		ept.x = bpt[X] + (ZorY * lmk + OffsetByXLZX) * sin(fwj);
		ept.y = bpt[Y] + (ZorY * lmk + OffsetByXLZX) * cos(fwj);
		spt.z = 10000.0;
		ept.z = 10000.0;

		makeline(spt,ept,1,1,L"平面图-路肩线");//绘路肩线
	}       
	//绘挖方示坡线
	if(qtw>0.001 &&htw>0.001 && qdmxz*hdmxz==0&&qnpt>4&&hnpt>4)//前后断面都为挖方且前断面为一般断面
	{
		DrawCutPartBetweenTwoMl(hdmdes[i-1].cml,hdmdes[i].cml,ZorY,qnpt,hnpt,qPtarr,hPtarr,wflag);			
	}
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
		Pt.x = (hPtarr[4].x + k*qPtarr[4].x)/(1+k);
		Pt.y = (hPtarr[4].y + k*qPtarr[4].y)/(1+k);			
		Pt.z = (hPtarr[4].z + k*qPtarr[4].z)/(1+k);
		for(j=0;j<MidNum;j++)
			MidArr.append(Pt);		
		//绘填方hdmdes[i-1].cml --  MidMl;
		DrawPartFillBetweenTwoMl(hdmdes[i-1].cml,MidMl,ZorY,qnpt,MidNum,qPtarr,MidArr,tflag);				
		//绘挖方MidMl-hdmdes[i].cml;
		DrawCutPartBetweenTwoMl(MidMl,hdmdes[i].cml,ZorY,MidNum,hnpt,MidArr,hPtarr,wflag);
	}

	return 1;
}

int CDrawPM::draw_fill_cut()
{
	int i;	
	ACHAR pszLabel[256];	
	int pos;
	bool ztflag,ytflag, zwflag,ywflag;
	CRSDAT MidCrs;
	AcGePoint3d Pt,spt,ept;
	double OffsetByXLZX = 0.0;
	ztflag=ytflag=zwflag=ywflag=true;
	_stprintf(pszLabel,L"正在绘填挖示坡线");
	acedSetStatusBarProgressMeter(pszLabel,0,100);
	SPXJJ = 2.0;
	if (!hdmdes)
		return 0;
	BGarowcml = 0.5*(hdmdes[0].cml+hdmdes[1].cml);
	for(i=1;i<NCRS;i++)
	{
		pos=(int)i*100.0/NCRS;
		acedSetStatusBarProgressMeterPos(pos);
		if(hdmdes[i].cml<Scml || hdmdes[i].cml>Ecml+10)
			continue;

		if(hdmdes[i-1].DorS==1&&hdmdes[i].DorS==2)//前为单路面，后为双路面
		{            
			//绘左侧填方示坡线
			if(fabs(hdmdes[i-1].RHFR[8])<0.1 || fabs(hdmdes[i-1].RHFL[8])<0.1)//前断面无右侧路幅  或无左侧路幅
			{
				drawfillandcut(i,-1,hdmdes[i-1].Ldeltah,hdmdes[i].Ldeltah,hdmdes[i-1].Zt,hdmdes[i].Zt,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Lnpt,
					hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL,ztflag,zwflag);	
				//绘右侧挖方示坡线
				//		     drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
				//			  hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);
			}
			else //前断面为双路福
			{
				drawfillandcut(i,-1,hdmdes[i-1].Ldeltah,hdmdes[i].Ldeltah,hdmdes[i-1].Zt,hdmdes[i].Zt,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Lnpt,
					hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL,ztflag,zwflag);	
				//绘右侧挖方示坡线
				//			  drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Y_Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Y_Yt,hdmdes[i-1].YDMXZ,hdmdes[i].Y_YDMXZ,hdmdes[i-1].Rnpt,
				//				  hdmdes[i].Y_Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].Y_PTRPTR,ytflag,ywflag);			  
			}
		}
		else if(hdmdes[i-1].DorS==2&&hdmdes[i].DorS==1)//前为双路面，后为单路面 
		{
			//绘左侧填方示坡线
			if(fabs(hdmdes[i].RHFR[8])<0.1 || fabs(hdmdes[i].RHFL[8])<0.1)//后断面无右侧路幅  或无左侧路幅
			{
				drawfillandcut(i,-1,hdmdes[i-1].Ldeltah,hdmdes[i].Ldeltah,hdmdes[i-1].Zt,hdmdes[i].Zt,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Lnpt,
					hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL,ztflag,zwflag);	
				//绘右侧挖方示坡线
				//			  drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
				//				  hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);
			}
			else//后断面为双路福
			{
				drawfillandcut(i,-1,hdmdes[i-1].Ldeltah,hdmdes[i].Ldeltah,hdmdes[i-1].Zt,hdmdes[i].Zt,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Lnpt,
					hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL,ztflag,zwflag);
				//			  drawfillandcut(i,1,hdmdes[i-1].Y_Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Y_Yt,hdmdes[i].Yt,hdmdes[i-1].Y_YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Y_Rnpt,
				//				  hdmdes[i].Rnpt,hdmdes[i-1].Y_PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);			  
			}
		}
		else	if(hdmdes[i-1].DorS==2&&hdmdes[i].DorS==2)//前后都是双路面
		{
			//绘左侧填方示坡线
			drawfillandcut(i,-1,hdmdes[i-1].Ldeltah,hdmdes[i].Ldeltah,hdmdes[i-1].Zt,hdmdes[i].Zt,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Lnpt,
				hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL,ztflag,zwflag);	
			//绘右侧挖方示坡线
			//		   drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
			//			   hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);	
			//绘左侧填方示坡线
			drawfillandcut(i,-1,hdmdes[i-1].Y_Ldeltah,hdmdes[i].Y_Ldeltah,hdmdes[i-1].Y_Zt,hdmdes[i].Y_Zt,hdmdes[i-1].Y_ZDMXZ,hdmdes[i].Y_ZDMXZ,hdmdes[i-1].Y_Lnpt,
				hdmdes[i].Y_Lnpt,hdmdes[i-1].Y_PTRPTL, hdmdes[i].Y_PTRPTL,ztflag,zwflag);				
			// *------------------------right---------------------------------------	
			//			drawfillandcut(i,-1,hdmdes[i-1].Y_Rdeltah,hdmdes[i].Y_Rdeltah,hdmdes[i-1].Y_Yt,hdmdes[i].Y_Yt,hdmdes[i-1].Y_YDMXZ,hdmdes[i].Y_YDMXZ,hdmdes[i-1].Y_Rnpt,
			//				hdmdes[i].Y_Rnpt,hdmdes[i-1].Y_PTRPTR, hdmdes[i].Y_PTRPTR,ytflag,ywflag);				
			//绘右侧填方示坡线、路肩线、水沟线		
		}//前后都是双路面
		else //if(hdmdes[i-1].DorS==1&&hdmdes[i].DorS==1)
		{
			//绘左侧填方示坡线
			double qlmk,hlmk;
			qlmk = fabs(hdmdes[i-1].RHFL[4]-hdmdes[i-1].RHFL[2]);
			hlmk = fabs(hdmdes[i].RHFL[4]-hdmdes[i].RHFL[2]);

			if(fabs(qlmk-hlmk)<5.0)
				drawfillandcut(i,-1,hdmdes[i-1].Ldeltah,hdmdes[i].Ldeltah,hdmdes[i-1].Zt,hdmdes[i].Zt,hdmdes[i-1].ZDMXZ,hdmdes[i].ZDMXZ,hdmdes[i-1].Lnpt,
				hdmdes[i].Lnpt,hdmdes[i-1].PTRPTL, hdmdes[i].PTRPTL,ztflag,zwflag);	
			//绘右侧挖方示坡线
			//		   drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
			//			   hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);		   
		}
	}
	acedSetStatusBarProgressMeterPos(100);

	BGarowcml = 0.5*(hdmdes[0].cml+hdmdes[1].cml);	 
	for(i=1;i<NCRS;i++)
	{
		pos=(int)i*100.0/NCRS;
		acedSetStatusBarProgressMeterPos(pos);
		if(hdmdes[i].cml<Scml || hdmdes[i].cml>Ecml+10)
			continue;

		if(hdmdes[i-1].DorS==1&&hdmdes[i].DorS==2)//前为单路面，后为双路面
		{            
			//绘左侧填方示坡线
			if(fabs(hdmdes[i-1].RHFR[8])<0.1 || fabs(hdmdes[i-1].RHFL[8])<0.1)//前断面无右侧路幅  或无左侧路幅
			{
				//绘右侧挖方示坡线
				drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
					hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);
			}
			else //前断面为双路福
			{
				//绘右侧挖方示坡线
				drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Y_Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Y_Yt,hdmdes[i-1].YDMXZ,hdmdes[i].Y_YDMXZ,hdmdes[i-1].Rnpt,
					hdmdes[i].Y_Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].Y_PTRPTR,ytflag,ywflag);			  
			}
		}
		else if(hdmdes[i-1].DorS==2&&hdmdes[i].DorS==1)//前为双路面，后为单路面 
		{
			//绘左侧填方示坡线
			if(fabs(hdmdes[i].RHFR[8])<0.1 || fabs(hdmdes[i].RHFL[8])<0.1)//后断面无右侧路幅  或无左侧路幅
			{	
				//绘右侧挖方示坡线
				drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
					hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);
			}
			else//后断面为双路福
			{
				drawfillandcut(i,1,hdmdes[i-1].Y_Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Y_Yt,hdmdes[i].Yt,hdmdes[i-1].Y_YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Y_Rnpt,
					hdmdes[i].Rnpt,hdmdes[i-1].Y_PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);			  
			}
		}
		else	if(hdmdes[i-1].DorS==2&&hdmdes[i].DorS==2)//前后都是双路面
		{
			//绘右侧挖方示坡线
			drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
				hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);	
			//绘左侧填方示坡线	
			// *------------------------right---------------------------------------	
			drawfillandcut(i,-1,hdmdes[i-1].Y_Rdeltah,hdmdes[i].Y_Rdeltah,hdmdes[i-1].Y_Yt,hdmdes[i].Y_Yt,hdmdes[i-1].Y_YDMXZ,hdmdes[i].Y_YDMXZ,hdmdes[i-1].Y_Rnpt,
				hdmdes[i].Y_Rnpt,hdmdes[i-1].Y_PTRPTR, hdmdes[i].Y_PTRPTR,ytflag,ywflag);				
			//绘右侧填方示坡线、路肩线、水沟线		
		}//前后都是双路面
		else //if(hdmdes[i-1].DorS==1&&hdmdes[i].DorS==1)
		{
			//绘右侧挖方示坡线
			double qlmk,hlmk;
			qlmk = fabs(hdmdes[i-1].RHFR[4]-hdmdes[i-1].RHFR[2]);
			hlmk = fabs(hdmdes[i].RHFR[4]-hdmdes[i].RHFR[2]);

			if(fabs(qlmk-hlmk)<5)
				drawfillandcut(i,1,hdmdes[i-1].Rdeltah,hdmdes[i].Rdeltah,hdmdes[i-1].Yt,hdmdes[i].Yt,hdmdes[i-1].YDMXZ,hdmdes[i].YDMXZ,hdmdes[i-1].Rnpt,
				hdmdes[i].Rnpt,hdmdes[i-1].PTRPTR, hdmdes[i].PTRPTR,ytflag,ywflag);		   
		}
	}
	acedSetStatusBarProgressMeterPos(99);


	for(i=1;i<NCRS;i++)
	{
		pos=(int)i*100.0/NCRS;
		acedSetStatusBarProgressMeterPos(pos);
		if(hdmdes[i].cml<Scml || hdmdes[i].cml>Ecml+10)
			continue;
		if (fabs(hdmdes[i].cml-Scml)>0.1 && fabs(hdmdes[i].cml-Ecml)>0.1)
			DrawAllSlop(i);//绘桥面锥坡
		//绘分隔带线
		xlpoint PZtmp;
		AcGePoint3d Qfgdpt,Hfgdpt;

		PZtmp.lc = hdmdes[i-1].cml;
		OffsetByXLZX = hdmdes[i-1].OffsetByXLZX;
		pm->xlpoint_pz(&PZtmp);
		Qfgdpt.x = PZtmp.y + (-hdmdes[i-1].RHFL[2]+OffsetByXLZX)*sin(PZtmp.a+0.5*pi);
		Qfgdpt.y = PZtmp.x + (-hdmdes[i-1].RHFL[2]+OffsetByXLZX)*cos(PZtmp.a+0.5*pi);
		Qfgdpt.z = 10000.0; 

		PZtmp.lc = hdmdes[i].cml;
		OffsetByXLZX = hdmdes[i].OffsetByXLZX;
		pm->xlpoint_pz(&PZtmp);
		Hfgdpt.x = PZtmp.y + (-hdmdes[i].RHFL[2]+OffsetByXLZX)*sin(PZtmp.a+0.5*pi);
		Hfgdpt.y = PZtmp.x + (-hdmdes[i].RHFL[2]+OffsetByXLZX)*cos(PZtmp.a+0.5*pi);
		Hfgdpt.z = 10000.0;

		//ads_printf(L"cml=%lf %lf x=%lf y=%lf %lf %lf\n",	PZtmp.lc,hdmdes[i-1].RHFL[1],Qfgdpt.x,Qfgdpt.y,Hfgdpt.x,Hfgdpt.y);
		makeline(Qfgdpt,Hfgdpt,4,0,L"平面图-分隔带线");


		PZtmp.lc = hdmdes[i-1].cml;
		OffsetByXLZX = hdmdes[i-1].OffsetByXLZX;
		pm->xlpoint_pz(&PZtmp);
		Qfgdpt.x = PZtmp.y + (hdmdes[i-1].RHFR[2]+OffsetByXLZX)*sin(PZtmp.a+0.5*pi);
		Qfgdpt.y = PZtmp.x + (hdmdes[i-1].RHFR[2]+OffsetByXLZX)*cos(PZtmp.a+0.5*pi);
		Qfgdpt.z = 10000.0; 

		PZtmp.lc = hdmdes[i].cml;
		OffsetByXLZX = hdmdes[i].OffsetByXLZX;
		pm->xlpoint_pz(&PZtmp);
		Hfgdpt.x = PZtmp.y + (hdmdes[i].RHFR[2]+OffsetByXLZX)*sin(PZtmp.a+0.5*pi);
		Hfgdpt.y = PZtmp.x + (hdmdes[i].RHFR[2]+OffsetByXLZX)*cos(PZtmp.a+0.5*pi);
		Hfgdpt.z = 10000.0;

		makeline(Qfgdpt,Hfgdpt,4,0,L"平面图-分隔带线");
		//  makeline(hdmdes[i-1].PTRPTL[1],hdmdes[i].PTRPTL[1],4,0,L"分隔带线");
	}

	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
	return RTNORM;
}

void CDrawPM::DRAW_ALLGOU() 
{
	xlpoint PZ;
	AcGePoint2d Lpt,Rpt,Y_Lpt,Y_Rpt,Z_Lpt,Z_Rpt;//路肩线
	AcGePoint3d spt,ept;//
	ACHAR pszLabel[256];
	int pos;
	//绘水沟箭头
	int QorH,ZorY,i,iq,ih;

	QorH=ZorY=1;
	_stprintf(pszLabel,L"正在绘水沟线");
	acedSetStatusBarProgressMeter(pszLabel,0,100);
	iq = 0;

	for(i=1; i<NCRS; i++)
	{
		pos=(int)i*100.0/NCRS;
		acedSetStatusBarProgressMeterPos(pos);
		if(hdmdes[i].cml<Scml || hdmdes[i].cml>Ecml+10)
			continue;
		ih = i;
		AcGePoint3d qPJpt,hPJpt;
		double qfwj,hfwj;
		if(fabs(hdmdes[ih].cml-hdmdes[iq].cml) > 15)
		{
			//绘左排水沟线
			if(hdmdes[iq].PTRPTL.length()>0 && hdmdes[ih].PTRPTL.length()>0)
			{
				qPJpt = hdmdes[iq].PTRPTL[hdmdes[iq].PTRPTL.length()-1];
				hPJpt = hdmdes[ih].PTRPTL[hdmdes[ih].PTRPTL.length()-1];
				qfwj = hdmdes[iq].fwj;
				hfwj = hdmdes[ih].fwj;
				DRAW_GOU(qPJpt,hPJpt,qfwj,hfwj,hdmdes[iq].LGouPtNum,hdmdes[ih].LGouPtNum,hdmdes[iq].PTLgou, hdmdes[ih].PTLgou,-1);

				//绘右排水沟线
				//    DRAW_GOU(qPJpt,hPJpt,qfwj,hfwj,hdmdes[iq].RGouPtNum,hdmdes[ih].RGouPtNum,hdmdes[iq].PTRgou, hdmdes[ih].PTRgou,1);		  			
			}
			if(hdmdes[iq].PTRPTR.length()>0 && hdmdes[ih].PTRPTR.length()>0)
			{
				qPJpt = hdmdes[iq].PTRPTR[hdmdes[iq].PTRPTR.length()-1];
				hPJpt = hdmdes[ih].PTRPTR[hdmdes[ih].PTRPTR.length()-1];
				qfwj = hdmdes[iq].fwj;
				hfwj = hdmdes[ih].fwj;
				//绘右排水沟线
				DRAW_GOU(qPJpt,hPJpt,qfwj,hfwj,hdmdes[iq].RGouPtNum,hdmdes[ih].RGouPtNum,hdmdes[iq].PTRgou, hdmdes[ih].PTRgou,1);		  			
			}



			//	  DRAW_GOU(hdmdes[iq].LGouPtNum,hdmdes[ih].LGouPtNum,hdmdes[iq].PTLgou, hdmdes[ih].PTLgou,-1);
			//绘右排水沟线
			//	  DRAW_GOU(hdmdes[iq].RGouPtNum,hdmdes[ih].RGouPtNum,hdmdes[iq].PTRgou, hdmdes[ih].PTRgou,1);		  			

			//绘左截水沟线
			DRAW_GOU(hdmdes[iq].LJSGouPtNum,hdmdes[ih].LJSGouPtNum,hdmdes[iq].PTLJSgou,hdmdes[ih].PTLJSgou,-1);
			//绘右截水沟线
			DRAW_GOU(hdmdes[iq].RJSGouPtNum,hdmdes[ih].RJSGouPtNum,hdmdes[iq].PTRJSgou,hdmdes[ih].PTRJSgou,1);
			if(hdmdes[iq].DorS==2&&hdmdes[ih].DorS==2)//两个断面都是双路面
			{
				//绘左排水沟线
				DRAW_GOU(hdmdes[iq].Y_LGouPtNum,hdmdes[ih].Y_LGouPtNum,hdmdes[iq].Y_PTLgou, hdmdes[ih].Y_PTLgou,-1);
				//绘右排水沟线
				DRAW_GOU(hdmdes[iq].Y_RGouPtNum,hdmdes[ih].Y_RGouPtNum,hdmdes[iq].Y_PTRgou, hdmdes[ih].Y_PTRgou,1);		  			
				//绘左截水沟线
				DRAW_GOU(hdmdes[iq].Y_LJSGouPtNum,hdmdes[ih].Y_LJSGouPtNum,hdmdes[iq].Y_PTLJSgou,hdmdes[ih].Y_PTLJSgou,-1);
				//绘右截水沟线
				DRAW_GOU(hdmdes[iq].Y_RJSGouPtNum,hdmdes[ih].Y_RJSGouPtNum,hdmdes[iq].Y_PTRJSgou,hdmdes[ih].Y_PTRJSgou,1);
			}
			DRAW_TW_GOU(hdmdes[iq],hdmdes[ih]);//绘填挖交界处水沟
			iq = ih;
		}

	}	  
	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
}

void CDrawPM::DRAW_GOU(int qPtNum,int hPtNum,AcGePoint3dArray &qPtarr, AcGePoint3dArray &hPtarr,int ZorY)
{
	ads_point qGoupt,hGoupt,GouPt;
	//绘水沟箭头
	double angle;
	int QorH;
	AcGePoint3d spt,ept;//	

	if(qPtNum>4&&hPtNum>4)
	{
		spt = qPtarr[1];
		ept = hPtarr[1];
		makeline(spt,ept,4,0,L"水沟线");
		spt = qPtarr[4];
		ept = hPtarr[4];
		//spt = qPtarr[qPtNum-1];
		//ept = hPtarr[hPtNum-1];

		makeline(spt,ept,4,0,L"水沟线");	 

		qGoupt[X]=0.5*(qPtarr[2].x+qPtarr[3].x);
		qGoupt[Y]=0.5*(qPtarr[2].y+qPtarr[3].y);
		hGoupt[X]=0.5*(hPtarr[2].x+hPtarr[3].x);
		hGoupt[Y]=0.5*(hPtarr[2].y+hPtarr[3].y);
		if(qPtarr[3].z < hPtarr[3].z)//后一标高大于前一标高
		{
			angle = ads_angle(hGoupt,qGoupt);
			QorH = -1;
		}
		else
		{
			angle = ads_angle(qGoupt,hGoupt);
			QorH = 1;
		}
		GouPt[X]=0.5*(qGoupt[X]+hGoupt[X]);
		GouPt[Y]=0.5*(qGoupt[Y]+hGoupt[Y]);
		drawGouArrow(GouPt,angle,QorH*ZorY);			  
	}
}

void CDrawPM::DRAW_GOU(AcGePoint3d qpt,AcGePoint3d hpt,double qfwj,double hfwj,int qPtNum,int hPtNum,AcGePoint3dArray &qPtarr, AcGePoint3dArray &hPtarr,int ZorY)
{
	ads_point qGoupt,hGoupt,GouPt;
	//绘水沟箭头
	double angle;
	int QorH;
	AcGePoint3d spt,ept;//	
	qpt.z=hpt.z=0.0;
	if(qPtNum>4 && hPtNum>4)
	{
		spt = qPtarr[1];
		ept = hPtarr[1];
		makeline(spt,ept,4,0,L"平面图-水沟线");
		spt = qPtarr[4];
		ept = hPtarr[4];
		// spt.x = qpt.x + 2.0*sin(qfwj+ZorY*0.5*pi);
		// spt.y = qpt.y + 2.0*cos(qfwj+ZorY*0.5*pi);

		// ept.x = hpt.x + 2.0*sin(hfwj+ZorY*0.5*pi);
		// ept.y = hpt.y + 2.0*cos(hfwj+ZorY*0.5*pi);
		spt.z=ept.z=0.0;
		//ept = hPtarr[hPtNum-1];

		makeline(spt,ept,4,0,L"平面图-水沟线");	 

		//		  qGoupt[X]=0.5*(qPtarr[2].x+qPtarr[3].x);
		//		  qGoupt[Y]=0.5*(qPtarr[2].y+qPtarr[3].y);
		//		  hGoupt[X]=0.5*(hPtarr[2].x+hPtarr[3].x);
		//		  hGoupt[Y]=0.5*(hPtarr[2].y+hPtarr[3].y);
		qGoupt[X]=0.5*(qpt.x+spt.x);
		qGoupt[Y]=0.5*(qpt.y+spt.y);
		hGoupt[X]=0.5*(hpt.x+ept.x);
		hGoupt[Y]=0.5*(hpt.y+ept.y);
		//ads_printf(L"%lf %lf %lf %lf fwj=%lf %lf %lf %lf %lf %lf\n",qGoupt[X],qGoupt[Y],hGoupt[X],hGoupt[Y],qfwj,hfwj,qpt.x,qpt.y,hpt.x,hpt.y);
		if(qPtarr[3].z < hPtarr[3].z)//后一标高大于前一标高
		{
			angle = ads_angle(hGoupt,qGoupt);
			QorH = -1;
		}
		else
		{
			angle = ads_angle(qGoupt,hGoupt);
			QorH = 1;
		}
		GouPt[X]=0.5*(qGoupt[X]+hGoupt[X]);
		GouPt[Y]=0.5*(qGoupt[Y]+hGoupt[Y]);
		drawGouArrow(GouPt,angle,QorH*ZorY);			  
	}
}

void CDrawPM::Draw_BridgeSlop(ads_point bspt, ads_point rpt, ads_point spt)
{
	double angle,dist;
	ads_point midpt,arcmid;

	midpt[X] = (rpt[X]+spt[X])/2.0;
	midpt[Y] = (rpt[Y]+spt[Y])/2.0; //坡脚点与路肩点的中点	
	angle = ads_angle(bspt,midpt);
	dist = (caldistance(bspt,rpt)+caldistance(bspt,spt))/2.0;

	ads_polar(bspt,angle,dist,arcmid); //弧的中点
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"arc", RTPOINT, rpt, RTPOINT, arcmid, RTPOINT, spt, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"arc", RTPOINT, rpt, RTPOINT, arcmid, RTPOINT, spt, RTNONE);
#endif // VERSION_CAD2010	
	
	line0(bspt[X],bspt[Y],rpt[X],rpt[Y],2,L"锥坡");
	line0(bspt[X],bspt[Y],spt[X],spt[Y],2,L"锥坡");

	ads_polar(bspt,angle,dist*3/4.0,arcmid);
	line0(bspt[X],bspt[Y],arcmid[X],arcmid[Y],2,L"锥坡");
	arcmid[X] = (rpt[X]+midpt[X])/2.0;
	arcmid[Y] = (rpt[Y]+midpt[Y])/2.0;
	angle = ads_angle(bspt,arcmid);
	dist = caldistance(bspt,midpt)*2/3.0;
	ads_polar(bspt,angle,dist,arcmid);
	line0(bspt[X],bspt[Y],arcmid[X],arcmid[Y],2,L"锥坡");

	arcmid[X] = (spt[X]+midpt[X])/2.0;
	arcmid[Y] = (spt[Y]+midpt[Y])/2.0;
	angle = ads_angle(bspt,arcmid);
	ads_polar(bspt,angle,dist,arcmid);
	line0(bspt[X],bspt[Y],arcmid[X],arcmid[Y],2,L"锥坡");
}

void CDrawPM::line0(double x1, double y1, double x2, double y2,int color,ACHAR *layer)
{
	AcGePoint3d spt,ept;

	spt.x = x1;   spt.y = y1;  spt.z =11000.0;
	ept.x = x2;   ept.y = y2;  ept.z =11000.0;
	makeline(spt,ept,color,0,layer);
}

void CDrawPM::CheckZdkInf(int FldSum,double SEml[50][3],ZDKDAT &ZDK)
{
	int i;

	for(i=0;i<FldSum;i++)
	{
		if(ZDK.cml>SEml[i][0]-0.001&&ZDK.cml<SEml[i][1]+0.001)
		{
			if(SEml[i][2]<0.0)
			{
				ZDK.LZDK = 0.0;
			}
			else
			{
				ZDK.RZDK = 0.0;
			}
		}
	}
}

void CDrawPM::CalZdkFld(int &FldSum,double SEml[50][3])
{
	FldSum = 0;
	if(drawfldzdk)
		return;

	CString filename;
	FILE *fpr;
	int NCRS;
	double dtmp;

	CString roadname= pm->RoadName;
	if(roadname!="主线")
	{
		SEml[0][0]=pm->DLArray[0].ELC;
		SEml[0][1]=pm->XLLength;
		filename =Cworkdir + "\\data\\" + "hdmresult." + Pro + "." + roadname;
		if((fpr=_wfopen(filename ,L"r"))==NULL)
		{
			SEml[0][2]=-1;
		}
		fwscanf(fpr,L"%d",&NCRS);
		fclose(fpr);
		if(NCRS>0)
			SEml[0][2]=-1;
		else
			SEml[0][2]=1;


		FldSum = 1;
	}
	else
	{
		DBS.ReadAllRoadName(pm->mdbname);


		for(int i=1;i<DBS.m_RoadSum;i++)
		{						

			filename =Cworkdir + "\\data\\" + "hdmresult." + Pro + "." + DBS.Road[i].RoadName;
			if((fpr=_wfopen(filename ,L"r"))==NULL)
				continue;

			fwscanf(fpr,L"%d",&NCRS);
			//开空间
			fwscanf(fpr,L"%lf%lf%lf%lf%lf%lf",
				&SEml[FldSum][0],&dtmp,&dtmp,
				&SEml[FldSum][1],&dtmp,&dtmp);
			if(NCRS>0)
				SEml[FldSum][2]=1;
			else
				SEml[FldSum][2]=-1;

			FldSum++;
			fclose(fpr);
		}
	}
}

void CDrawPM::DrawZDK()
{
	//设置参数
	AcGePoint2d spt,ept;
	AcGePoint2d pt;
	xlpoint ptmp;
	double fwj,dml;
	int FldSum;
	double SEml[50][3];
	AcGePoint2dArray m_PtArray_L, m_PtArray_R;
	m_PtArray_L.removeAll();
	m_PtArray_R.removeAll();
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"平面图-占地边界线");

	//读取占地宽数据
	FILE *fpr;
	CString ZDKfilename;
	int DorS,i;
	ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + Pro + "." + pm->RoadName;
	if((fpr=_wfopen(ZDKfilename ,L"r"))==NULL)
	{
		ads_printf(L"无法打开HdmZDK文件!");
		return ;
	}
	fwscanf(fpr, L"%d", &NZDK);
	pZDK = new ZDKDAT[NZDK];
	for (i = 0; i < NZDK; i++)
		fwscanf(fpr, L"%lf%lf%d%lf%lf", &pZDK[i].cml, &dml, &pZDK[i].IsInsertHdm, &pZDK[i].LZDK, &pZDK[i].RZDK);
	fclose(fpr);

	//计算占地宽分离段数据
	CalZdkFld(FldSum,SEml);

	//检测占地宽信息
	for (i = 0; i < NZDK-1; i++) 
	{        
		if(pZDK[i].cml>=Scml&&pZDK[i].cml<=Ecml)
			CheckZdkInf(FldSum,SEml,pZDK[i]);		//分离段占地宽设置为0
	}

	//画占地线
	for (i = 0; i < NZDK - 1; i++)
	{        
		if (pZDK[i].cml >= Scml && pZDK[i].cml <= Ecml && pZDK[i].IsInsertHdm < 1 && pZDK[i + 1].IsInsertHdm < 1)
		{
			if (pZDK[i].LZDK > 0.01 || pZDK[i + 1].LZDK > 0.01)
			{
				ptmp.lc = pZDK[i].cml;
				pm->xlpoint_pz(&ptmp);
				spt.x = ptmp.y + pZDK[i].LZDK * sin(ptmp.a - 0.5 * PI);
				spt.y = ptmp.x + pZDK[i].LZDK * cos(ptmp.a - 0.5 * PI);
				ptmp.lc = pZDK[i+1].cml;
				pm->xlpoint_pz(&ptmp);
				ept.x = ptmp.y + pZDK[i + 1].LZDK * sin(ptmp.a - 0.5 * PI);
				ept.y = ptmp.x + pZDK[i + 1].LZDK * cos(ptmp.a - 0.5 * PI); 

				m_PtArray_L.append(spt);
				m_PtArray_L.append(ept);
				//makeline(spt,ept,0,YdxWid,L"占地边界线");	
			}
			else if (m_PtArray_L.length())
			{
				makepolyline(m_PtArray_L, m_ZDKLineColor, YdxWid, LayerName);
				m_PtArray_L.removeAll();
			}
				

			if (pZDK[i].RZDK > 0.01 || pZDK[i + 1].RZDK > 0.01)
			{
				ptmp.lc = pZDK[i].cml;
				pm->xlpoint_pz(&ptmp);
				spt.x = ptmp.y + pZDK[i].RZDK * sin(ptmp.a + 0.5 * PI);
				spt.y = ptmp.x + pZDK[i].RZDK * cos(ptmp.a + 0.5 * PI);
				ptmp.lc = pZDK[i + 1].cml;
				pm->xlpoint_pz(&ptmp);
				ept.x = ptmp.y + pZDK[i + 1].RZDK * sin(ptmp.a + 0.5 * PI);
				ept.y = ptmp.x + pZDK[i + 1].RZDK * cos(ptmp.a + 0.5 * PI);

				m_PtArray_R.append(spt);
				m_PtArray_R.append(ept);
				//makeline(spt,ept,0,YdxWid,L"占地边界线");
			}
			else if (m_PtArray_R.length())
			{
				makepolyline(m_PtArray_R, m_ZDKLineColor, YdxWid, LayerName);
				m_PtArray_R.removeAll();
			}

		}
		else
		{
			if (m_PtArray_L.length())
			{
				makepolyline(m_PtArray_L, m_ZDKLineColor, YdxWid, LayerName);
				m_PtArray_L.removeAll();
			}
			if (m_PtArray_R.length())
			{
				makepolyline(m_PtArray_R, m_ZDKLineColor, YdxWid, LayerName);
				m_PtArray_R.removeAll();
			}
		}
	}

	if (m_PtArray_L.length())
	{
		makepolyline(m_PtArray_L, m_ZDKLineColor, YdxWid, LayerName);
		m_PtArray_L.removeAll();
	}
	if (m_PtArray_R.length())
	{
		makepolyline(m_PtArray_R, m_ZDKLineColor, YdxWid, LayerName);
		m_PtArray_R.removeAll();
	}

	if(ZdkMode < 3)
		BZ_ZDK();

	if(pZDK)
	{
		delete pZDK;
		pZDK = NULL;
	}
	return ;
}

void CDrawPM::BZ_ZDK()
{   
	AcGePoint2dArray ZDPtAy; 
	AcGePoint2d Zpt;
	int i;
	xlpoint PZ;
	double b;
	AcGePoint3d Pt1,Pt2,Pt; 
	ACHAR chrh[80],ckml[80];
	CString GH;
	ACHAR pszLabel[256];	
	int pos,draw_zybz;
	CString str,str1;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"平面图-占地线");

	_stprintf(pszLabel,L"正在绘占地线");
	acedSetStatusBarProgressMeter(pszLabel,0,100);
	draw_zybz = -1;
	for(i=0;i<NZDK;i++)
	{
		pos=(int)i*100.0/(NZDK+NZDK);
		acedSetStatusBarProgressMeterPos(pos);

		if(pZDK[i].cml>=Scml&&pZDK[i].cml<=Ecml&&pZDK[i].LZDK>0.01&&pZDK[i].IsInsertHdm<1)
		{
			PZ.lc = pZDK[i].cml;
			pm->xlpoint_pz(&PZ);

			Pt1.x = PZ.y+pZDK[i].LZDK*sin(PZ.a-0.5*pi);
			Pt1.y = PZ.x+pZDK[i].LZDK*cos(PZ.a-0.5*pi);
			Pt2.x= PZ.y+ZdkZlen*sin(PZ.a-0.5*pi);
			Pt2.y= PZ.x+ZdkZlen*cos(PZ.a-0.5*pi);
			makeline(Pt1,Pt2,m_ZDKLineColor,0,L"占地线");							    
			if(draw_zybz==1)
			{
				Pt.x=Pt2.x+0.5*sin(PZ.a);
				Pt.y=Pt2.y+0.5*cos(PZ.a);
			}
			else
			{
				//Pt.x=Pt2.x+(0.5+texth)*sin(PZ.a);
				//Pt.y=Pt2.y+(0.5+texth)*cos(PZ.a);
				Pt.x = Pt2.x + (0.5 + m_DrawPara.TextHPara.EarthLineTextH) * sin(PZ.a);
				Pt.y = Pt2.y + (0.5 + m_DrawPara.TextHPara.EarthLineTextH) * cos(PZ.a);
			}

			b = -PZ.a;
			if (draw_zybz==-1) b=b+pi;
			if (b<0.0) b=2*pi+b;
			if (b>2*pi) b=b-2*pi;
			if(ZdkMode==0)
			{
				ads_rtos(pZDK[i].LZDK,2,2,chrh);
				_stprintf(chrh,L"%sm",chrh);
			}
			else if(ZdkMode==1)
				_stprintf(chrh,L"X-%0.2lf",Pt1.y);
			else
			{
				ads_rtos(pZDK[i].LZDK,2,2,ckml);
				_stprintf(chrh,L"%sm X-%0.2lf",ckml,Pt1.y);
			}			
			//maketext(Pt, chrh,b,texth ,0,4,L"宋体",L"占地线");
			maketext(Pt, chrh,b, m_DrawPara.TextHPara.EarthLineTextH, m_ZDKLineColor,4,L"宋体", LayerName);

			//Pt.x=Pt.x+(texth+1.0*DFACTOR)*sin(PZ.a-pi);
			//Pt.y=Pt.y+(texth+1.0*DFACTOR)*cos(PZ.a-pi);			
			Pt.x=Pt.x+(m_DrawPara.TextHPara.EarthLineTextH +1.0)*sin(PZ.a-pi);
			Pt.y=Pt.y+(m_DrawPara.TextHPara.EarthLineTextH +1.0)*cos(PZ.a-pi);
			str = pm->XLC(pZDK[i].cml);
			if(ZdkMode==0)
				_tcscpy(ckml,str);
			else if(ZdkMode==1)
			{
				str1.Format(L"Y-%0.2lf",Pt1.x);
				_tcscpy(ckml,str1);
			}
			else
			{
				str1.Format(L"%s Y-%0.2lf",str,Pt1.x);
				_tcscpy(ckml,str1);
			}
			//maketext(Pt,ckml,b,texth ,0,4,L"宋体",L"占地线");
			maketext(Pt,ckml,b, m_DrawPara.TextHPara.EarthLineTextH, m_ZDKLineColor,4,L"宋体", LayerName);
		}
	}
	///--------------------------------------------------right
	for(i=0;i<NZDK;i++)
	{
		pos=(int)(i+NZDK)*100.0/(NZDK+NZDK);
		acedSetStatusBarProgressMeterPos(pos);

		if(pZDK[i].cml>=Scml&&pZDK[i].cml<=Ecml&&pZDK[i].RZDK>0.01&&pZDK[i].IsInsertHdm<1)
		{
			PZ.lc = pZDK[i].cml;
			pm->xlpoint_pz(&PZ);

			Pt1.x = PZ.y+pZDK[i].RZDK*sin(PZ.a+0.5*pi);
			Pt1.y = PZ.x+pZDK[i].RZDK*cos(PZ.a+0.5*pi);
			Pt2.x=PZ.y+ZdkYlen*sin(PZ.a+0.5*pi);
			Pt2.y=PZ.x+ZdkYlen*cos(PZ.a+0.5*pi);	
			makeline(Pt1,Pt2, m_ZDKLineColor,0, LayerName);
			if(draw_zybz==1)
			{
				// 				Pt.x=Pt2.x+0.5*DFACTOR*sin(PZ.a);
				// 				Pt.y=Pt2.y+0.5*DFACTOR*cos(PZ.a);
				Pt.x=Pt2.x+0.5*sin(PZ.a);
				Pt.y=Pt2.y+0.5*cos(PZ.a);
			}
			else
			{
				// 				Pt.x=Pt2.x+(0.5*DFACTOR+texth)*sin(PZ.a);
				// 				Pt.y=Pt2.y+(0.5*DFACTOR+texth)*cos(PZ.a);
				Pt.x=Pt2.x+(0.5+ m_DrawPara.TextHPara.EarthLineTextH)*sin(PZ.a);
				Pt.y=Pt2.y+(0.5+ m_DrawPara.TextHPara.EarthLineTextH)*cos(PZ.a);
			}
			b = -PZ.a;
			if (draw_zybz==-1) b=b+pi;
			if (b<0.0) b=2*pi+b;
			if (b>2*pi) b=b-2*pi;		    						
			if(ZdkMode==0)
			{
				ads_rtos(pZDK[i].RZDK,2,2,chrh);
				_stprintf(chrh,L"%sm",chrh);
			}
			else if(ZdkMode==1)
				_stprintf(chrh,L"X-%0.2lf",Pt1.y);
			else
			{
				ads_rtos(pZDK[i].RZDK,2,2,ckml);
				_stprintf(chrh,L"%sm X-%0.2lf",ckml,Pt1.y);		
			}
			//			maketext(Pt, chrh,b,texth ,0,1,L"宋体",L"占地线");
			maketext(Pt, chrh,b, m_DrawPara.TextHPara.EarthLineTextH, m_ZDKLineColor,1,L"宋体", LayerName);


			// 			Pt.x=Pt.x+(texth+1.0*DFACTOR)*sin(PZ.a-pi);
			// 			Pt.y=Pt.y+(texth+1.0*DFACTOR)*cos(PZ.a-pi);
			Pt.x=Pt.x+(m_DrawPara.TextHPara.EarthLineTextH +1.0)*sin(PZ.a-pi);
			Pt.y=Pt.y+(m_DrawPara.TextHPara.EarthLineTextH +1.0)*cos(PZ.a-pi);

			str = pm->XLC(pZDK[i].cml);
			if(ZdkMode==0)
				_tcscpy(ckml,str);
			else if(ZdkMode==1)
			{
				str1.Format(L"Y-%0.2lf",Pt1.x);
				_tcscpy(ckml,str1);
			}
			else
			{
				str1.Format(L"%s Y-%0.2lf",str,Pt1.x);
				_tcscpy(ckml,str1);
			}		
			//			maketext(Pt, ckml,b,texth ,0,1,L"宋体",L"占地线");
			maketext(Pt, ckml, b, m_DrawPara.TextHPara.EarthLineTextH, m_ZDKLineColor, 1, L"宋体", LayerName);
		}
	}

	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
	return ;
}

void CDrawPM::INSERT_BM(double XX,double YY,CString BianHao,double LEV)
{	
	//图层
	ACHAR layname[80];
	_stprintf(layname,L"%s",BM);
	//////////////////////////////////////////////////////////////////////////////////////
	ACHAR GaoCheng[20];
	if(fabs(LEV)>0)
		acdbRToS(LEV,2,3,GaoCheng);
	else 
		_tcscpy(GaoCheng,L"");

	AcGePoint3d InsertPt;//插入点
	/*
	cml = pm->TYLC(XLC);
	if(cml<Scml||cml>Ecml)
	return;*/
	//////////////////////////////////////////////////////////////////////////
	//提取图块
	ACHAR dwgPath[80],Name[50];
	_stprintf(dwgPath,L"%s\\dwg\\导线点.dwg",SysPath);
	_tcscpy(Name,L"导线点");
	AcDbObjectId pBlockId;
	pBlockId=CreateBlockFromDwgFile(dwgPath,Name);
	if(pBlockId==0)
	{
		acutPrintf(L"%s不存在!\n",dwgPath);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	////insertblock
	int k=0;
	double a0;//线路方位角
	/*
	if(X<0.1&&Y<0.1)//x=0,y=0
	{

	xlpoint Ptmp;
	Ptmp.lc=cml;
	pm->xlpoint_pz(&Ptmp);
	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//线路中线点坐标
	a0=Ptmp.a;

	InsertPt.x=Pt0.x+DD*sin(a0+pi*0.5);
	InsertPt.y=Pt0.y+DD*cos(a0+pi*0.5);
	}*/

	InsertPt.set(YY,XX,0);

	AcDbBlockReference *pBkRr=new AcDbBlockReference(InsertPt,pBlockId);

	pBkRr->setScaleFactors(AcGeScale3d(DFACTOR,DFACTOR,1));
	pBkRr->setLayer(layname);
	AddEntityToDbs(pBkRr);

	AcGePoint3d BZCen,BZCenL,BZCenR;//注字短线中心点，及左、右两点
	AcGePoint3d BZTop,BZBtom;//水准点编号中心、高程中心


	int istrbh=_tcslen(BianHao);//水准点编号字符串长
	int istrlev=_tcslen(GaoCheng);//水准点高程字符串长
	int iMax=istrbh>istrlev?istrbh:istrlev;//字符串最大长度

	BZCen.x=InsertPt.x+(0.4*iMax+0.5)*texth+2*DFACTOR;
	BZCen.y=InsertPt.y;
	BZCen.z=0.0;

	BZCenL.x=InsertPt.x+(0.5)*texth+2*DFACTOR;
	BZCenL.y=InsertPt.y;
	BZCenL.z=0.0;

	BZCenR.x=InsertPt.x+(0.8*iMax+0.5)*texth+2*DFACTOR;
	BZCenR.y=InsertPt.y;
	BZCenR.z=0.0;

	BZTop.x=BZCen.x;
	BZTop.y=BZCen.y+0.6*texth;
	BZTop.z=0.0;

	BZBtom.x=BZCen.x;
	BZBtom.y=BZCen.y-0.6*texth;
	BZBtom.z=0.0;

	ACHAR bz[20];
	_tcscpy(bz,BianHao);

	// 	maketext(BZTop,bz,0.0,texth,-1,3,L"宋体",layname);
	// 	maketext(BZBtom,GaoCheng,0.0,texth,-1,3,L"宋体",layname);
	maketext(BZTop,bz,0.0,texth,-1,3,L"宋体",layname);
	maketext(BZBtom,GaoCheng,0.0,texth,-1,3,L"宋体",layname);
	makeline(BZCenL,BZCenR,-1,0,layname);
}


void CDrawPM::INSERT_DX(double XX,double YY,CString BianHao,double LEV)
	//上导线－－纬，径，点号，里程，高程
{
	//图层
	ACHAR layname[80];
	_stprintf(layname,L"%s",L"导线点");
	//////////////////////////////////////////////////////////////////////////////////////
	ACHAR GaoCheng[20];
	if(fabs(LEV)>0.0001)
		acdbRToS(LEV,2,3,GaoCheng);
	else 
		_tcscpy(GaoCheng,L"");
	AcGePoint3d InsertPt;//插入点
	double cml;	
	/*
	cml = pm->TYLC(XLC);
	if(cml<Scml||cml>Ecml)
	return;*/

	//////////////////////////////////////////////////////////////////////////
	//提取图块
	ACHAR dwgPath[80],Name[50];
	_stprintf(dwgPath,L"%s\\dwg\\导线点.dwg",SysPath);
	_tcscpy(Name,L"导线点");
	AcDbObjectId pBlockId;
	pBlockId=CreateBlockFromDwgFile(dwgPath,Name);
	if(pBlockId==0)
	{
		acutPrintf(L"%s不存在!\n",dwgPath);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	////insertblock
	int k=0;
	/*
	if(X<0.1&&Y<0.1)//x=0,y=0
	{

	xlpoint Ptmp;
	Ptmp.lc=cml;
	pm->xlpoint_pz(&Ptmp);
	AcGePoint3d Pt0(Ptmp.y,Ptmp.x,0.0);//线路中线点坐标
	a0=Ptmp.a;

	InsertPt.x=Pt0.x+DD*sin(a0+pi*0.5);
	InsertPt.y=Pt0.y+DD*cos(a0+pi*0.5);
	}*/

	InsertPt.set(YY,XX,0);

	AcDbBlockReference *pBkRr=new AcDbBlockReference(InsertPt,pBlockId);

	pBkRr->setScaleFactors(AcGeScale3d(DFACTOR*0.5,DFACTOR*0.5,1));
	pBkRr->setLayer(layname);
	AddEntityToDbs(pBkRr);

	AcGePoint3d BZCen,BZCenL,BZCenR;//注字短线中心点，及左、右两点
	AcGePoint3d BZTop,BZBtom;//导线点编号中心、高程中心

	//   double bztexth=5.0*DFACTOR;
	double bztexth=texth;
	int istrbh=_tcslen(BianHao);//导线点编号字符串长
	int istrlev=_tcslen(GaoCheng);//导线点高程字符串长
	int iMax=istrbh>istrlev?istrbh:istrlev;//字符串最大长度

	BZCen.x=InsertPt.x+(0.4*iMax+0.5)*bztexth+2*DFACTOR;
	BZCen.y=InsertPt.y;
	BZCen.z=0.0;

	BZCenL.x=InsertPt.x+(0.5)*bztexth+2*DFACTOR;
	BZCenL.y=InsertPt.y;
	BZCenL.z=0.0;

	BZCenR.x=InsertPt.x+(0.8*iMax+0.5)*bztexth+2*DFACTOR;
	BZCenR.y=InsertPt.y;
	BZCenR.z=0.0;

	BZTop.x=BZCen.x;
	BZTop.y=BZCen.y+0.6*bztexth;
	BZTop.z=0.0;

	BZBtom.x=BZCen.x;
	BZBtom.y=BZCen.y-0.6*bztexth;
	BZBtom.z=0.0;

	ACHAR bz[20];
	_tcscpy(bz,BianHao);

	maketext(BZTop,bz,0.0,bztexth,-1,3,L"宋体",layname);
	maketext(BZBtom,GaoCheng,0.0,bztexth,-1,3,L"宋体",layname);
	makeline(BZCenL,BZCenR,-1,0,layname);
}

void CDrawPM::DrawDX()//绘制导线
{
	int i;

	DBS.Read_XLDbs(pm->mdbname ,L"导线表",pm->RoadName);	
	CString GH,BianHao;
	double XX,YY,XLC,LEV;
	for(i=0;i<DBS.DXNum;i++)
	{
		XX=DBS.DX[i].N;
		YY=DBS.DX[i].E;		
		BianHao=DBS.DX[i].BianHao;
		LEV=DBS.DX[i].GaoCheng;
		INSERT_DX(XX,YY,BianHao,LEV);
	}
}

void CDrawPM::DrawBM()
{
	// TODO: Implement the command
	DBS.Read_XLDbs(pm->mdbname ,L"水准点表",pm->RoadName);
	//////////////////////////////////////////////////////////////////////////
	int i=0;
	CString GH,BianHao;
	double XX,YY,LEV;
	for(i=0;i<DBS.BmNum;i++)
	{
		XX=DBS.BM[i].N;
		YY=DBS.BM[i].E;		
		BianHao=DBS.BM[i].BianHao;
		LEV=DBS.BM[i].GaoCheng;
		INSERT_BM(XX,YY,BianHao,LEV);
	}
	//////////////////////////////////////////////////////////////////////////	
}

void CDrawPM::DrawBGDATA()//绘制标高数据图
{
	// TODO: Implement the command
	OutTAB tab;
	Zzdm = tab.ReadZm(pm->mdbname,pm->RoadName,L"左幅",pm);
	zdm = tab.ReadZm(pm->mdbname,pm->RoadName,L"全幅",pm);
	Yzdm = tab.ReadZm(pm->mdbname,pm->RoadName,L"右幅",pm);
	if(zdm==NULL&&Zzdm==NULL&&Yzdm)
	{
		ads_alert(L"请先录入主线纵断面数据!\n");
		return;
	}    

	pm->ReadCG();
	if(pm->ZcgNm<1&&pm->YcgNm<1)
	{
		ads_alert(L"请先计算主线超高加宽!\n");
		return;
	}
	pm->ReadJK();

	//绘制    	
	//1 读鼻端文件,形成当前pm的左侧关联里程段 和右侧关联里程段
	hdm.ReadBDFile();

	ZGLR.glxl="";
	YGLR.glxl="";

	hdm.FindWichGLROAD(pm,Scml,Ecml,ZGLR,YGLR); 

	if(ZGLR.glxl!=""||YGLR.glxl!="")
		hdm.ReadRoaDFile(pm,ZGLR,YGLR);
}   

//传入一点 返回路面标高
void CDrawPM::CalLmht(double N,double E,double &prjml,double &ZJ,double &Desh,double &lmhp,double &Lmbg)
{
	double yljhp,XJJ,CmlOnFLD;

	ReadLMK("左路面宽分段表");
	ReadLMK("右路面宽分段表");

	double RHF[10],YN,YE;
	prjml = pm->PROJ_ML(N,E);
	xlpoint PZ;
	PZ.lc = prjml;
	pm->xlpoint_pz(&PZ);
	Scml = Ecml = prjml;

	//查找关联道路
	DrawBGDATA();
	if(ZGLR.glxl=="" && YGLR.glxl=="")//无关联道路
	{
		//按单路面计算
		int LorR;
		//判断该点在pm线路的边测
		LorR=JudgePTLorR(N,E,ZJ,pm->XYArray,pm->XYNum);
		ZJ = LorR*ZJ;
		Lmbg = 0;
		lmhp = 0;
		if(LorR<0)
		{
			Desh = hdm.CalDesH(prjml,Zzdm,zdm);
			hdm.CalLMPT(prjml,Desh,pm->ZcgNm,pm->Zcg,pm->ZjkNm,pm->Zjk,NZLMK,ZLMK,RHF,
				LorR,BxNum,pBXArr,pm);
			if(CalLMBG(ZJ,RHF,Lmbg,lmhp)==0)
				ads_alert(L"指定点不在路面范围内!");
		}
		else
		{
			Desh = hdm.CalDesH(prjml,Yzdm,zdm);
			hdm.CalLMPT(prjml,Desh,pm->YcgNm,pm->Ycg,pm->YjkNm,pm->Yjk,NYLMK,YLMK,RHF,
				LorR,BxNum,pBXArr,pm);
			if(CalLMBG(ZJ,RHF,Lmbg,lmhp)==0)
				ads_alert(L"指定点不在路面范围内!");
		}

	}
	else//有关联道路
	{
		int res=0;
		double RHFL[10],RHFR[10],ZX_RHFL[10],ZX_RHFR[10],YX_RHFL[10],YX_RHFR[10];
		double cmlonzd;
		//HdmDes::RoadHdmdata ZX,ZZD,YZD; 

		//if(ZGLR.glxl!="")//有左关联
		//{
		//	if(ZGLR.glxlxz==1)//关联道路为匝道，当前线路为主线
		//	{
		//		res = CalLMBGOnNose(N,E,hdm.road[0],hdm.road[1],Lmbg,lmhp,ZJ,Desh);
		//	}
		//	else//关联道路为主线，当前线路为匝道
		//	{
		//		res = CalLMBGOnNose(N,E,hdm.road[1],hdm.road[0],Lmbg,lmhp,ZJ,Desh);
		//	}
		//}

		//if(res==0 && YGLR.glxl!="")//有右关联
		//{
		//	if(YGLR.glxlxz==1)//关联道路为匝道，当前线路为主线
		//	{
		//		res = CalLMBGOnNose(N,E,hdm.road[0],hdm.road[2],Lmbg,lmhp,ZJ,Desh);
		//	}
		//	else//关联道路为主线，当前线路为匝道
		//	{
		//		res = CalLMBGOnNose(N,E,hdm.road[2],hdm.road[0],Lmbg,lmhp,ZJ,Desh);
		//	}
		//}
		//int LorR=JudgePTLorR(N,E,ZJ,pm->XYArray,pm->XYNum);
		//ZJ = LorR*ZJ;
		//if(res==0)
		//	ads_alert(L"指定点不在路面范围内!");
	}

}
//传入一点 该点在路面及硬路肩范围内 计算该点的里程 标高 
void CDrawPM::CalYLMHt(double N,double E,double &prjml,double &Ht,double &lmhp)
{
	int LorR;
	double dist,desh;
	double LMHP,YLJHP;
	double half_fgdk;
	double RHF[10];

	prjml = pm->PROJ_ML(N,E);
	lmhp = -2;
	//判断该点在pm线路的边测
	LorR=JudgePTLorR(N,E,dist,pm->XYArray,pm->XYNum);
	if(LorR<0)//点在左侧
	{
		if(ZGLR.glxl==""||ZGLR.torf==1)//无左关联道路 或同向坡
		{
			desh = hdm.CalDesH(prjml,Zzdm,zdm);			

			hdm.CalLMPT(prjml,desh,pm->ZcgNm,pm->Zcg,pm->ZjkNm,pm->Zjk,
				NZLMK,ZLMK,RHF,
				-1,BxNum,pBXArr,pm);

			hdm.CalLMHP(prjml,pm->ZcgNm,pm->Zcg,LMHP,YLJHP);
			lmhp = LMHP;
			Ht = RHF[3] + (dist-RHF[2])*LMHP*0.01;

			return;
		}
		else if(ZGLR.glxl!=""&&ZGLR.torf==-1)//左侧有关联道路 且反坡
		{
			Ht = hdm.CalZDFPDesH(N,E,hdm.road[1],0);
		}
	}
	else//点在右侧
	{
		if(YGLR.glxl==""||YGLR.torf==1)//无右关联道路 或同向坡
		{
			desh = hdm.CalDesH(prjml,Yzdm,zdm);
			//计算路面横坡
			hdm.CalLMPT(prjml,desh,pm->YcgNm,pm->Ycg,pm->YjkNm,pm->Yjk,
				NYLMK,YLMK,RHF,
				1,BxNum,pBXArr,pm);
			hdm.CalLMHP(prjml,pm->YcgNm,pm->Ycg,LMHP,YLJHP);
			lmhp = LMHP;
			Ht = RHF[3] +(dist-RHF[2])*LMHP*0.01;	

			return;
		}
		else if(YGLR.glxl!=""&&YGLR.torf==-1)//右侧有关联道路 且反坡
		{
			Ht = hdm.CalZDFPDesH(N,E,hdm.road[2],0);
		}
	}
	return ;
}

//传入两点 一点为硬路肩边缘点  一点在土路肩范围内 计算该点的里程 标高 
//两点在同一横线上
void CDrawPM::CalTLMHt(double NY,double EY,double NT,double ET,double &prjml,double &Ht,double &tljhp)
{
	double dist;

	double LMHP,prjmlY,HtY;
	double half_fgdk,lydk,cxdk,yljk,tljk;
	int bkmode,sjbgmode,LorR;

	CalYLMHt(NY,EY,prjmlY,HtY,LMHP);
	LorR=JudgePTLorR(NY,EY,dist,pm->XYArray,pm->XYNum);
	if(LorR<0)
		hdm.CalLMK_JK(prjmlY,NZLMK,ZLMK,pm->ZcgNm,pm->Zcg,pm->ZjkNm,pm->Zjk,half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
	else
		hdm.CalLMK_JK(prjmlY,NYLMK,YLMK,pm->YcgNm,pm->Ycg,pm->YjkNm,pm->Yjk,half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
	//	if(fabs(tljhp) <0.1)
	//		hdm.CalLMK_JK(prjmlY,NYLMK,YLMK,pm->YjkNm,pm->Yjk,half_fgdk,lydk,cxdk,yljk,tljk,tljhp,bkmode,sjbgmode);
	dist = sqrt((NT-NY)*(NT-NY)+(ET-EY)*(ET-EY));
	Ht = HtY - dist*tljhp*0.01;
	//	Ht = HtY - tljk*tljhp*0.01;
	tljhp = -tljhp;
}

void CDrawPM::CalLMKbyFile(double cml,int ZorY,double &half_fgdk,double &lydk,double &cxdk,double &yljk,double &tljk)
{
	int i,j;
	double ToZFD,ToLYD,ToCXD,ToYLJ,ToTlj;
	AcGePoint2d Apt;

	ToZFD = half_fgdk;
	ToLYD = half_fgdk+lydk;
	ToCXD = half_fgdk+lydk+cxdk;
	ToYLJ = ToCXD + yljk;
	ToTlj = ToYLJ + tljk;
	xlpoint PZ;
	PZ.lc = cml;
	pm->xlpoint_pz(&PZ);
	double x1,y1,x2,y2,dis;
	x1 = PZ.x;
	y1 = PZ.y;
	x2 = x1 + 100*cos(PZ.a+ZorY*PI*0.5);
	y2 = y1 + 100*sin(PZ.a+ZorY*PI*0.5);

	for(i=0;i<BxNum;i++)
	{
		if(cml>pBXArr[i].bxsml&&cml<pBXArr[i].bxeml&&ZorY==pBXArr[i].LorR)
		{
			AcDbPolyline *pPline=new AcDbPolyline(pBXArr[i].ptnum);
			for(j =0;j<pBXArr[i].ptnum;j++)
			{
				Apt.x = pBXArr[i].ptarr[j*3];
				Apt.y = pBXArr[i].ptarr[j*3+1];
				pPline->addVertexAt(j,Apt);
				pPline->setWidthsAt(j,0.0,0.0);
			}

			AcDbLine *Line=new AcDbLine(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));		
			AcGePoint3dArray interPt;
			Line->intersectWith(pPline,AcDb::kOnBothOperands,interPt,0,0);
			if(interPt.length()>0)
			{
				double dx,dy;
				dx = interPt[0].x-x1, dy = interPt[0].y-y1;
				dis = sqrt(dx*dx+dy*dy);
				if(pBXArr[i].bxinf==0)
					ToZFD = dis;
				else if(pBXArr[i].bxinf==1)
					ToLYD = dis;
				else if(pBXArr[i].bxinf==2)
					ToCXD = dis;
				else if(pBXArr[i].bxinf==3)
					ToYLJ = dis;
				else
					ToTlj = dis;
			}
		}
	}

	half_fgdk = ToZFD;
	lydk = ToLYD - ToZFD;
	cxdk = ToCXD - ToLYD;
	yljk = ToYLJ - ToCXD;
	tljk = ToTlj - ToYLJ;
}

//给定距中线距离，及路面5点，求路面标高
int  CDrawPM::CalLMBG(double ZJ,double RHF[10],double &Lmbg,double &lmhp)
{
	int i=0;
	int res=0;
	Lmbg = 0.0;
	lmhp= 0.0;

	while(i<5 && fabs(ZJ)>RHF[i*2]+0.001)
		i++;
	if(i>=5 || i==0)
	{
		Lmbg = 0.0;//未找到
		lmhp = 0.0;
	}
	else //找到
	{
		if(fabs(RHF[i*2]-RHF[(i-1)*2])>0.001)
		{
			Lmbg = RHF[(i-1)*2+1] + (fabs(ZJ)-RHF[(i-1)*2])*(RHF[i*2+1]-RHF[(i-1)*2+1])/(RHF[i*2]-RHF[(i-1)*2]);
			lmhp = (RHF[i*2+1]-RHF[(i-1)*2+1])*100.0/(RHF[i*2]-RHF[(i-1)*2]);
		}
		else
		{
			Lmbg = RHF[(i-1)*2+1];
			lmhp = 0.0;
		}
		res = 1;
	}
	return res;
}
//输入主线与匝道数据
int  CDrawPM::CalLMBGOnNose(double N,double E,HdmDes::RoadHdmdata &ZX,HdmDes::RoadHdmdata &ZD,double &Lmbg,double &lmhp,double &ZJ,double &Desh)
{
	int res = 0;
	double zxcml;
	Lmbg=lmhp=ZJ=0.0;
	zxcml = ZX.pm->PROJ_ML(N,E);

	//计算主线路面点
	double RHFL[10],RHFR[10],ZD_RHFL[10],ZD_RHFR[10];
	Desh = hdm.CalDesH(zxcml,ZX.ZZM,ZX.AllZM);

	hdm.CalLMPT(zxcml,Desh,ZX.ZcgNm,ZX.Zcg,ZX.ZjkNm,ZX.Zjk,ZX.NZLMK,ZX.ZLMK,RHFL,
		-1,ZX.BxNum,ZX.pBXArr,ZX.pm);

	hdm.CalLMPT(zxcml,Desh,ZX.YcgNm,ZX.Ycg,ZX.YjkNm,ZX.Yjk,ZX.NYLMK,ZX.YLMK,RHFR,
		1,ZX.BxNum,ZX.pBXArr,ZX.pm);

	double XJJ,CmlOnFLD;

	hdm.CalZXxjj(zxcml,ZX.pm,ZD.pm,XJJ,CmlOnFLD);

	double linkpt[2];
	hdm.CalBDZDDesH(ZX,zxcml,CmlOnFLD,XJJ,ZD,ZD_RHFL,ZD_RHFR,RHFL,RHFR);

	//判别给定点是否位于主线范围
	int LorR;
	LorR=JudgePTLorR(N,E,ZJ,ZX.pm->XYArray,ZX.pm->XYNum);
	ZJ = LorR*ZJ;
	Lmbg = 0;
	lmhp = 0;

	if(LorR<0.0)//位于主线左侧
	{
		if(fabs(ZJ)<RHFL[8]+0.001)//在主线范围内
		{
			CalLMBG(ZJ,RHFL,Lmbg,lmhp);
			res = 1;
		}
	}
	else
	{
		if(fabs(ZJ)<RHFR[8]+0.001)//在主线范围内
		{
			CalLMBG(ZJ,RHFR,Lmbg,lmhp);
			res = 1;
		}
	}

	if(res==0)//在主线未找到,在匝道上找
	{
		LorR=JudgePTLorR(N,E,ZJ,ZD.pm->XYArray,ZD.pm->XYNum);
		ZJ = LorR*ZJ;
		Lmbg = 0;
		lmhp = 0;
		if(LorR<0.0)//位于主线左侧
		{
			if(fabs(ZJ)<ZD_RHFL[8]+0.001)//在匝道范围内
			{
				CalLMBG(ZJ,ZD_RHFL,Lmbg,lmhp);
				res = 1;
			}
		}
		else
		{
			if(fabs(ZJ)<ZD_RHFR[8]+0.001)//在匝道范围内
			{
				CalLMBG(ZJ,ZD_RHFR,Lmbg,lmhp);
				res = 1;
			}
		}

		return res;
	}
	else//找到在主线范围内
		return res;

	return res;
}


//绘填-》挖或者挖到填处的水沟
void  CDrawPM::DRAW_TW_GOU(CRSDAT Qhdm,CRSDAT Hhdm)
{
	AcGePoint3d pt1[2],pt2[2];
	double qfwj,hfwj;

	qfwj = Qhdm.fwj;
	hfwj = Hhdm.fwj;
	if(Qhdm.Zt<0.9 && Hhdm.Zt>0.9 && Qhdm.LGouPtNum>4 && Hhdm.PTRPTL.length()>8 )//填转挖
	{
		//1.取前一断面填方水沟两点

		//pt1[0].x = Qhdm.PTRPTL[Qhdm.PTRPTL.length()-1].x;
		//pt1[0].y = Qhdm.PTRPTL[Qhdm.PTRPTL.length()-1].y;

		//pt1[1].x = pt1[0].x + 2.0*sin(qfwj-0.5*pi);
		//pt1[1].y = pt1[0].y+ 2.0*cos(qfwj-0.5*pi);

		pt1[0].x = Qhdm.PTLgou[1].x;
		pt1[0].y = Qhdm.PTLgou[1].y;

		pt1[1].x = Qhdm.PTLgou[4].x;
		pt1[1].y = Qhdm.PTLgou[4].y;

		//pt1[1].x = pt1[0].x + 2.0*sin(qfwj-0.5*pi);
		//pt1[1].y = pt1[0].y+ 2.0*cos(qfwj-0.5*pi);


		pt2[0] = Hhdm.PTRPTL[4];
		pt2[1] = Hhdm.PTRPTL[8];

		makeline(pt1[0],pt2[0],4,0,L"平面图-水沟线");
		makeline(pt1[1],pt2[1],4,0,L"平面图-水沟线");

	}
	if(Qhdm.Zt>0.9 && Hhdm.Zt<0.9 && Qhdm.PTRPTL.length()>8 && Hhdm.LGouPtNum>4 )//挖转填
	{
		pt1[0] = Qhdm.PTRPTL[4];
		pt1[1] = Qhdm.PTRPTL[8];

		pt2[0].x = Hhdm.PTLgou[1].x;
		pt2[0].y = Hhdm.PTLgou[1].y;

		pt2[1].x = Hhdm.PTLgou[4].x;
		pt2[1].y = Hhdm.PTLgou[4].y;

		makeline(pt1[0],pt2[0],4,0,L"平面图-水沟线");
		makeline(pt1[1],pt2[1],4,0,L"平面图-水沟线");
	}


	if(Qhdm.Yt<0.9 && Hhdm.Yt>0.9 && Qhdm.RGouPtNum>4 && Hhdm.PTRPTR.length()>8)//填转挖
	{
		//1.取前一断面填方水沟两点

		pt1[0].x = Qhdm.PTRgou[1].x;
		pt1[0].y = Qhdm.PTRgou[1].y;

		pt1[1].x = Qhdm.PTRgou[4].x;
		pt1[1].y = Qhdm.PTRgou[4].y;


		pt2[0] = Hhdm.PTRPTR[4];
		pt2[1] = Hhdm.PTRPTR[8];

		makeline(pt1[0],pt2[0],4,0,L"平面图-水沟线");
		makeline(pt1[1],pt2[1],4,0,L"平面图-水沟线");

	}
	if(Qhdm.Yt>0.9 && Hhdm.Yt<0.9 && Qhdm.PTRPTR.length()>8 && Hhdm.RGouPtNum>4)//挖转填
	{
		pt1[0] = Qhdm.PTRPTR[4];
		pt1[1] = Qhdm.PTRPTR[8];

		pt2[0].x = Hhdm.PTRgou[1].x;
		pt2[0].y = Hhdm.PTRgou[1].y;

		pt2[1].x = Hhdm.PTRgou[4].x;
		pt2[1].y = Hhdm.PTRgou[4].y;


		makeline(pt1[0],pt2[0],4,0,L"平面图-水沟线");
		makeline(pt1[1],pt2[1],4,0,L"平面图-水沟线");
	}
}























































































