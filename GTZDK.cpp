// GTZDK.cpp: implementation of the GTZDK class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdarx.h"
#include "GTZDK.h"
#include "BAS_DRAW_FUN.H"
#include "XLdatabase.H"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016

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
#include "STDIO.h"
#include "malloc.h"
#include "time.h"
#define  streq(s,t) (_tcscmp((s),(t))==0)
#define  AppName  /*MSG1*/L"GUADS"
JD_CENTER*  TempPlinep;

Acad::ErrorStatus  GTZDK_ROAD::subTranformBy(const AcGeMatrix3d &)
{	return Acad::eOk;   }


GTZDK_ROAD::~GTZDK_ROAD()
{

}
GTZDK_ROAD::GTZDK_ROAD()  {

}

Adesk::Boolean  GTZDK_ROAD::G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx)
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

Adesk::Boolean  GTZDK_ROAD::G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor)
{     
	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	
	pWd->geometry().polyline(2,pArr);
	return  0;
}


Adesk::Boolean  GTZDK_ROAD::G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode=1)
{   	
	//AcGeVector3d normal(0.0,0.0,1.0), direction(cos(ang), sin(ang), 0.0);
	//// ads_printf(L"\n             %10.3f %10.3f %10.4f ",Pt.x,Pt.y,dms_rad(ang));
	//pWd->subEntityTraits().setColor(icolor);
	//AcGiTextStyle pTextStyle;
	////pTextStyle.setFont(L"宋体",TRUE,FALSE,0,0);
	//pTextStyle.setFont(L"宋体",0,0,134,2);
	//pTextStyle.setTextSize(texth);
	//pWd->geometry().text(Pt,normal,direction,ctext,_tcslen(ctext),Adesk::kTrue,pTextStyle);
	//return 1;

	AcGeVector3d normal(0.0, 0.0, 1.0), direction(cos(ang), sin(ang), 0.0);
	pWd->subEntityTraits().setColor(icolor);
	AcGiTextStyle pTextStyle;
	pTextStyle.setFont(L"宋体", 0, 0, 134, 2);
	pTextStyle.setTextSize(texth);
	AcGePoint3d pt1;
	double len = _tcslen(ctext) * texth * 0.7;
	if (mode == 0)//输入点为字中下点时
	{
		Pt.x = Pt.x - 0.5 * len * cos(ang);
		Pt.y = Pt.y - 0.5 * len * sin(ang);
	}
	else if (mode == 1)//输入点为字左下点时
	{
		Pt.x = Pt.x;
		Pt.y = Pt.y;
	}
	else if (mode == 2)//输入点为字右下点时
	{
		Pt.x = Pt.x - len * cos(ang);
		Pt.y = Pt.y - len * sin(ang);
	}
	else if (mode == 3)//输入点为字左上角点
	{
		Pt.x = Pt.x + texth * cos(ang - 0.5 * PI);
		Pt.y = Pt.y + texth * sin(ang - 0.5 * PI);
	}
	else if (mode == 4)//输入点为字右上角点
	{
		Pt.x = Pt.x + texth * cos(ang - 0.5 * PI) - len * cos(ang);
		Pt.y = Pt.y + texth * sin(ang - 0.5 * PI) - len * sin(ang);
	}
	else if (mode == 5)//输入点为字中上点时
	{
		Pt.x = Pt.x - 0.5 * len * cos(ang) + texth * cos(ang - 0.5 * PI);
		Pt.y = Pt.y - 0.5 * len * sin(ang) + texth * sin(ang - 0.5 * PI);
	}
	pWd->geometry().text(Pt, normal, direction, ctext, _tcslen(ctext), Adesk::kTrue, pTextStyle);
	return 1;
}

//	标注占地宽
Adesk::Boolean  GTZDK_ROAD::BZ_ZDK(AcGiWorldDraw* pWd)
{       
	if (pLine == NULL)return -1;

	int i;
	xlpoint PZ;
	double Len = 30, btinf, texth = 3.5, b, dml, BoxL = 1.0;
	AcGePoint3d Pt1, Pt2, Pt;
	ACHAR chrh[20], gh[10], ckml[80];
	CString GH;
	AcGePoint3d ZDPtAy[5];
	int draw_zybz = -1;

	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt000);
	for (i = 0; i < NZDK; i++)
	{
		PZ.lc = pZDK[i].cml;
		pLine->xlpoint_pz(&PZ);

		Pt1.x = pZDK[i].Lpt[1], Pt1.y = pZDK[i].Lpt[0], Pt1.z = 0.0;
		Pt2.x = PZ.y + (pZDK[i].LZDK + Len) * sin(PZ.a - 0.5 * pi);
		Pt2.y = PZ.x + (pZDK[i].LZDK + Len) * cos(PZ.a - 0.5 * pi);
		if (pZDK[i].LZDK > 0.01)
		{
			G_makeline(pWd, Pt1, Pt2, 0);

			if (draw_zybz == 1)
			{
				Pt.x = Pt2.x + 0.5 * sin(PZ.a);
				Pt.y = Pt2.y + 0.5 * cos(PZ.a);
			}
			else
			{
				Pt.x = Pt2.x + (0.5 + texth) * sin(PZ.a);
				Pt.y = Pt2.y + (0.5 + texth) * cos(PZ.a);
			}
			b = -PZ.a;
			if (draw_zybz == -1) b = b + pi;
			if (b < 0.0) b = 2 * pi + b;
			if (b > 2 * pi) b = b - 2 * pi;

			_stprintf(chrh, L"%0.2lf", pZDK[i].LZDK);
			G_maketext(pWd, Pt, chrh, b, texth, 4, 2);

			Pt.x = Pt2.x - 0.5 * sin(PZ.a);
			Pt.y = Pt2.y - 0.5 * sin(PZ.a);
			GH = pLine->XLC(pZDK[i].cml);
			_tcscpy(ckml, GH);

			G_maketext(pWd, Pt, ckml, b, texth, 4, 2);
		}

		Pt1.x = pZDK[i].Rpt[1], Pt1.y = pZDK[i].Rpt[0], Pt1.z = 0.0;
		Pt2.x = PZ.y + (pZDK[i].RZDK + Len) * sin(PZ.a + 0.5 * pi);
		Pt2.y = PZ.x + (pZDK[i].RZDK + Len) * cos(PZ.a + 0.5 * pi);
		if (pZDK[i].RZDK > 0.01)
		{
			G_makeline(pWd, Pt1, Pt2, 0);

			if (draw_zybz == 1)
			{
				Pt.x = Pt2.x + 0.5 * sin(PZ.a);
				Pt.y = Pt2.y + 0.5 * cos(PZ.a);
			}
			else
			{
				Pt.x = Pt2.x + (0.5 + texth) * sin(PZ.a);
				Pt.y = Pt2.y + (0.5 + texth) * cos(PZ.a);
			}
			b = -PZ.a;
			if (draw_zybz == -1) b = b + pi;
			if (b < 0.0) b = 2 * pi + b;
			if (b > 2 * pi) b = b - 2 * pi;
			_stprintf(chrh, L"%0.2lf", pZDK[i].RZDK);
			G_maketext(pWd, Pt, chrh, b, texth, 4, 1);
			Pt.x = Pt.x - (texth + 1.0) * sin(PZ.a);
			Pt.y = Pt.y - (texth + 1.0) * cos(PZ.a);
			GH = pLine->XLC(pZDK[i].cml);
			_tcscpy(ckml, GH);
			G_maketext(pWd, Pt, ckml, b, texth, 4, 1);
		}
	}
	return 1;


}

void GTZDK_ROAD::CalZDKXY()//由占地宽计算占地界坐标
{
	int i;
	xlpoint PZ;
	double fwj;
	//	assertWriteEnabled();

	for(i=0;i<NZDK;i++)
	{
		PZ.lc = pZDK[i].cml;
		pLine->xlpoint_pz(&PZ);
		fwj = PZ.a;
		pZDK[i].Lpt[0]=PZ.x+pZDK[i].LZDK*cos(fwj-0.5*pi);
		pZDK[i].Lpt[1]=PZ.y+pZDK[i].LZDK*sin(fwj-0.5*pi);
		pZDK[i].Rpt[0]=PZ.x+pZDK[i].RZDK*cos(fwj+0.5*pi);
		pZDK[i].Rpt[1]=PZ.y+pZDK[i].RZDK*sin(fwj+0.5*pi);
	}
}

bool GTZDK_ROAD::ReadZDKFile(FILE* fpr)
{
	//   assertWriteEnabled();
	ACHAR ckml[80];	
	double dml,cml,Lzdk,Rzdk;
	int DorS,num;
	NZDK=0;
	fwscanf(fpr, L"%d",&num);	
	for(int i=0;i<num;i++)
	{
		fwscanf(fpr , L"%lf%lf%d%lf%lf",&cml,&dml,&DorS,&Lzdk,&Rzdk);
		if(cml>=Scml-0.01&&cml<=Ecml+0.01 && DorS==0)//非增加断面
		{
			pZDK[NZDK].cml = cml;
			pZDK[NZDK].LZDK = Lzdk;
			pZDK[NZDK].RZDK = Rzdk;

			NZDK++;
		}
	}

	return true;
}

void GTZDK_ROAD::FastSort(struct ZDKDAT R[],int s1,int t1)
{
	int i;
	if(s1<t1)
	{
		i = PARTITION(R,s1,t1);
		FastSort(R,s1,i-1);
		FastSort(R,i+1,t1);
	}
}
int GTZDK_ROAD::PARTITION(struct ZDKDAT R[],int l,int h)
{
	int i,j;
	struct ZDKDAT temp;

	i = l;
	j = h;

	temp = R[i];
	do{
		while((R[j].cml>=temp.cml) && (i<j))
			j--;
		if(i<j) 
			R[i++] = R[j];
		while((R[i].cml<=temp.cml) && (i<j))
			i++;
		if(i<j) 
			R[j--] = R[i];
	}while(i!=j);
	R[i] = temp;
	return i;
}

void  GTZDK_ROAD::DelEntInYD()
{
	AcGePoint3d p1,p2,p3,p4;
	ads_point bkpt1,bkpt2,bkpt3,bkpt4,pt,pt1,wpt;
	int result,i;
	double dist,ang;
	CalZDKXY();   

	for(i=0;i<NZDK-1;i++)
	{
		p1.x = pZDK[i].Lpt[1];p1.y = pZDK[i].Lpt[0];
		p2.x = pZDK[i+1].Lpt[1];p2.y = pZDK[i+1].Lpt[0];
		p3.x = pZDK[i+1].Rpt[1];p3.y = pZDK[i+1].Rpt[0];
		p4.x = pZDK[i].Rpt[1];p4.y = pZDK[i].Rpt[0];

		bkpt1[X]=p1.x;	bkpt1[Y]=p1.y;
		bkpt2[X]=p2.x;	bkpt2[Y]=p2.y;
		bkpt3[X]=p3.x;	bkpt3[Y]=p3.y;
		bkpt4[X]=p4.x;	bkpt4[Y]=p4.y;			   	   
		result = 12;
		result = BAS_DRAW_FUN::ZDKAutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);			   
		for(int k=2;k<5;k++)
		{
			//								flag_k=k;//是否显示进度条。<2显示
			BAS_DRAW_FUN::CalFrame(-(k-1)*0.01,p1,p2,p3,p4,bkpt1,bkpt2,bkpt3,bkpt4);
			result = BAS_DRAW_FUN::ZDKAutoEntBK(bkpt1,bkpt2,bkpt3,bkpt4,result);
			if(result < 0)
				break;
		}
	}
	bkpt1[X]=pZDK[0].Lpt[1],bkpt1[Y]=pZDK[0].Lpt[0];
	bkpt2[X]=pZDK[0].Rpt[1],bkpt2[Y]=pZDK[0].Rpt[0];
	BAS_DRAW_FUN::AutoEntBK1(bkpt1,bkpt2);
	bkpt1[X]=pZDK[NZDK-1].Lpt[1],bkpt1[Y]=pZDK[NZDK-1].Lpt[0];
	bkpt2[X]=pZDK[NZDK-1].Rpt[1],bkpt2[Y]=pZDK[NZDK-1].Rpt[0];
	BAS_DRAW_FUN::AutoEntBK1(bkpt1,bkpt2);

	pt[X] = pZDK[0].Lpt[1], pt[Y] =  pZDK[0].Lpt[0] ;
	pt1[X] = pZDK[0].Rpt[1],pt1[Y] = pZDK[0].Rpt[0];
	ang = ads_angle(pt1 , pt);
	dist = sqrt((pt[X]-pt1[X])*(pt[X]-pt1[X]) + (pt[Y]-pt1[Y])*(pt[Y]-pt1[Y]));
	ads_polar(pt1 , ang , dist+0.5*DFACTOR , wpt);
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, wpt, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, wpt, RTNONE);
#endif // VERSION_CAD2010	
	
	for(i=0;i<NZDK;i++)
	{
		pt[X] = pZDK[i].Lpt[1] ,pt[Y] = pZDK[i].Lpt[0];
		pt1[X] = pZDK[i].Rpt[1],pt1[Y] = pZDK[i].Rpt[0];
		ang = ads_angle(pt1 , pt);
		dist = sqrt((pt[X]-pt1[X])*(pt[X]-pt1[X]) + (pt[Y]-pt1[Y])*(pt[Y]-pt1[Y]));
		ads_polar(pt1 , ang , dist+0.5*DFACTOR , wpt);
#ifdef VERSION_CAD2016
		acedCommandS(RTPOINT, wpt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTPOINT, wpt, 0);
#endif // VERSION_CAD2010		
	}
	//-----右侧
	for(i=NZDK-1;i>=0;i--)
	{
		pt[X] = pZDK[i].Lpt[1] ,pt[Y] = pZDK[i].Lpt[0] ;
		pt1[X] = pZDK[i].Rpt[1] ,pt1[Y] = pZDK[i].Rpt[0] ;
		ang = ads_angle(pt , pt1);
		dist = sqrt((pt[X]-pt1[X])*(pt[X]-pt1[X]) + (pt[Y]-pt1[Y])*(pt[Y]-pt1[Y]));
		ads_polar(pt , ang , dist+0.5*DFACTOR , wpt);
#ifdef VERSION_CAD2016
		acedCommandS(RTPOINT, wpt, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTPOINT, wpt, RTNONE);
#endif // VERSION_CAD2010		
		
	}

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"", RTSTR, L"", 0);
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"", RTSTR, L"", 0);
	ads_command(RTNONE);
#endif // VERSION_CAD2010		


}

Adesk::Boolean  GTZDK_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{ 

	textStyle();
	//   ZX.worldDraw(pWd);

	if(pLine==NULL) return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);

	CalZDKXY();	
	//   FastSort(pZDK,0,NLZDK-1);
	//   FastSort(pRZDK,0,NRZDK-1);
	AcGePoint3d Pt1,Pt2;
	double btinf1,btinf2;
	AcGeVector3d  normal(0.0,0.0,1.0);


	for (int i = 0; i < NZDK-1; i++) 
	{
		Pt1.x=pZDK[i].Lpt[1];Pt1.y=pZDK[i].Lpt[0];Pt1.z=0.0;
		Pt2.x=pZDK[i+1].Lpt[1];Pt2.y=pZDK[i+1].Lpt[0];Pt2.z=0.0;
		if(pZDK[i].LZDK>0.01||pZDK[i+1].LZDK>0.01)
			G_makeline(pWd,Pt1,Pt2,6);		

		Pt1.x=pZDK[i].Rpt[1];Pt1.y=pZDK[i].Rpt[0];Pt1.z=0.0;
		Pt2.x=pZDK[i+1].Rpt[1];Pt2.y=pZDK[i+1].Rpt[0];Pt2.z=0.0;
		if(pZDK[i].RZDK>0.01||pZDK[i+1].RZDK>0.01)
			G_makeline(pWd,Pt1,Pt2,6);
	}
	BZ_ZDK(pWd);
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}

Acad::ErrorStatus GTZDK_ROAD::subExplode(AcDbVoidPtrArray& entitySet) const 
{   AcDbVoidPtrArray entset;
AcDbObjectId entId;

return Acad::eOk;
}

// Files data in from a DWG file.
Acad::ErrorStatus GTZDK_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();
	AcDbObject::dwgInFields(pFiler);
	if (pFiler->filerType() == AcDb::kWblockCloneFiler) 
	{
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}

	//	ZX.dwgInFields(pFiler);
	int j,k;

	pFiler->readItem(&NZDK);
	//	 pFiler->readItem(&NRZDK);
	//	pLZDK = new CZDKDATA::ZDKDAT[NLZDK];    
	//	pRZDK = new CZDKDATA::ZDKDAT[NRZDK];	

	for (j=0;j<NZDK;j++)
	{  
		pFiler->readItem(&(pZDK[j].cml));
		pFiler->readItem(&(pZDK[j].LZDK));
		pFiler->readItem(&(pZDK[j].Lpt[0]));
		pFiler->readItem(&(pZDK[j].Lpt[1]));
		pFiler->readItem(&(pZDK[j].RZDK));	
		pFiler->readItem(&(pZDK[j].Rpt[0]));
		pFiler->readItem(&(pZDK[j].Rpt[1]));
	};

	void *pVoid;
	//pFiler->readAddress(&pVoid);
	pLine = TempPlinep;


	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus GTZDK_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);
	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());

	//	ZX.dwgOutFields(pFiler);
	int j;
	pFiler->writeItem(NZDK);
	//    pFiler->writeItem(NRZDK);

	for (j=0;j<NZDK;j++)
	{  
		pFiler->writeItem(pZDK[j].cml);
		pFiler->writeItem(pZDK[j].LZDK);	
		pFiler->writeItem(pZDK[j].Lpt[0]);
		pFiler->writeItem(pZDK[j].Lpt[1]);
		pFiler->writeItem(pZDK[j].RZDK);
		pFiler->writeItem(pZDK[j].Rpt[0]);
		pFiler->writeItem(pZDK[j].Rpt[1]);
	};

	// pFiler->writeAddress(pLine);	
	TempPlinep=pLine;
	return pFiler->filerStatus();
}
// Files data in from a DXF file.
//
Acad::ErrorStatus GTZDK_ROAD::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	/*
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
	char *m_modal;
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
	if ( inbuf.restype == AcDb:: kDxfInt16+1 )
	{
	NDL = inbuf.resval.rint;
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
	};*/


	return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus GTZDK_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();

	/*
	int j,k=0;
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass, "GTZDM");
	pFiler->writeItem(AcDb::kDxfInt16,NBPD);
	pFiler->writeItem(AcDb::kDxfInt16+1,NDL);
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
	};*/


	return pFiler->filerStatus();
}

Acad::ErrorStatus GTZDK_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											   AcDbIntArray& osnapModes,
											   AcDbIntArray& geomIds) const
{
	double btinf;
	AcGePoint3d Pt; 
	assertReadEnabled();
	//    ZX.getGripPoints(gripPoints,osnapModes,geomIds);
	int i;
	for (i = 0; i < NZDK; i++) 
	{  
		Pt.set(pZDK[i].Lpt[1],pZDK[i].Lpt[0],0.0);
		gripPoints.append(Pt);
	};

	for (i = 0; i < NZDK; i++) 
	{  
		Pt.set(pZDK[i].Rpt[1],pZDK[i].Rpt[0],0.0);
		gripPoints.append(Pt);
	};	
	return Acad::eOk;
}

Acad::ErrorStatus GTZDK_ROAD::subGetOsnapPoints(
	AcDb::OsnapMode   osnapMode,
	int               gsSelectionMark,
	const AcGePoint3d&    pickPoint,
	const AcGePoint3d&    lastPoint,
	const AcGeMatrix3d&   viewXform,
	AcGePoint3dArray& snapPoints,

	AcDbIntArray&     geomIds) const 
{   
	assertReadEnabled();

	//  ZX.getOsnapPoints(osnapMode,gsSelectionMark,pickPoint,lastPoint,viewXform,snapPoints,geomIds);
	AcGePoint3d Pt;
	double btinf;
	int i;

	switch (osnapMode) 
	{
	case AcDb::kOsModeEnd :
		//变坡点	
		for ( i = 0; i <NZDK; i++) 
		{   
			Pt.set(pZDK[i].Lpt[1],pZDK[i].Lpt[0],0.0);
			snapPoints.append(Pt);
		};
		for ( i = 0; i <NZDK; i++) 
		{   
			Pt.set(pZDK[i].Rpt[1],pZDK[i].Rpt[0],0.0);
			snapPoints.append(Pt);
		};
		break;
	default :       return Acad::eOk;	
	}

	return Acad::eOk;
}

Acad::ErrorStatus GTZDK_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,
												  const AcGeVector3d& offset)
{  // AcDbIntArray moveNO;
	int j,k,i;   
	xlpoint PZ;
	double dx,dy,kk;
	assertWriteEnabled();
	for ( j=0;j<indices.length();j++)
	{   // ads_printf(L"\n indices.length()=%d,j=%d  indices[j]=%d",indices.length(),j,indices[j]);
		// 变坡点序号 0~NBPD-1

		if (indices[j]<NZDK)//左占地线
		{
			k=indices[j];
			if(pZDK[k].LZDK>0.01)
			{
				pZDK[k].Lpt[0]=pZDK[k].Lpt[0]+offset[Y];
				pZDK[k].Lpt[1]=pZDK[k].Lpt[1]+offset[X];
				//			PZ.lc = pLZDK[k].cml;
				//			xlpoint_pz(pLine->array,pLine->tarray,&PZ);            
				//            pLZDK[k].cml=pLine->PROJ_ML(pLZDK[k].pt[0],pLZDK[k].pt[1]);
				PZ.lc = pZDK[k].cml;
				pLine->xlpoint_pz(&PZ);
				dx=PZ.x-pZDK[k].Lpt[0], dy=PZ.y-pZDK[k].Lpt[1];
				kk=sqrt(dx*dx+dy*dy);
				pZDK[k].LZDK = kk;
			}
			//			ads_printf(L"%lf %lf %lf %lf %lf %lf\n",pLZDK[k].cml,pLZDK[k].pt[0],pLZDK[k].pt[1],offset[Y],offset[X],pLZDK[k].ZDK);
		}
		else if ((indices[j]>=NZDK)&&(indices[j]<NZDK+NZDK))//右占地线
		{
			k=indices[j]-NZDK; // 该切线边对应的变坡点号
			if(pZDK[k].RZDK>0.01)
			{
				pZDK[k].Rpt[0]=pZDK[k].Rpt[0]+offset[Y];
				pZDK[k].Rpt[1]=pZDK[k].Rpt[1]+offset[X];
				//			PZ.lc = pLZDK[k].cml;
				//			pLine->xlpoint_pz(pLine->array,pLine->tarray,&PZ);            
				//            pRZDK[k].cml=pLine->PROJ_ML(pRZDK[k].pt[0],pRZDK[k].pt[1]);
				PZ.lc = pZDK[k].cml;
				pLine->xlpoint_pz(&PZ);
				dx=PZ.x-pZDK[k].Rpt[0], dy=PZ.y-pZDK[k].Rpt[1];
				kk=sqrt(dx*dx+dy*dy);
				pZDK[k].RZDK = kk;
			}
		}
	};

	return Acad::eOk;
}

void GTZDK_ROAD::subList() const 
{
	int i;
	assertReadEnabled();
	ads_printf(L"\n 类：\t%s",isA()->name());

}


int GTZDK_ROAD::textStyle()
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

//  增加一个点数据
void GTZDK_ROAD::AddZDK()
{  	
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;

	rc = ads_entsel(L"\nSelect a  占地线上要添加的边界点 : ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;	
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*  acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDK_ROAD::desc())) 
	{    
		pObj->close(); 
		GTZDK_ROAD  *pzdk;
		/*acdbOpenObject(pzdk, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pzdk,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pzdk->assertWriteEnabled();
		pzdk->ZDKAdd(PT);
		pzdk->close();  // Finished with the pline header.                
	}
	else 
	{
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDK");
	}
	return  ;   
}

int GTZDK_ROAD::ZDKChange(AcGePoint3d PT)
{
	int i,j,rc;
	double dx,dy,dis,dml,ZDK,Bdml;
	ACHAR mes[140],ckml[80],gh[10];
	CString GH;

	for(i=0;i<NZDK;i++)
	{
		dx = pZDK[i].Lpt[0]-PT.y;
		dy = pZDK[i].Lpt[1]-PT.x;
		dis = sqrt(dx*dx+dy*dy);
		if(dis < 2.0)
			break;
	}
	if(i<NZDK)//改变左边第i个占地点
	{
		/*
		dml=pLine->XLC1(pZDK[i].cml);	
		ads_initget(2+4,NULL);
		_stprintf(mes, L"\n请输入里程 = <%0.2lf>    ",dml);
		rc=ads_getreal(mes,&Bdml);
		if(rc==RTCAN)
		return 1;
		if(rc!=RTNONE)	
		pLZDK[i].cml=pLine->TYLC(Bdml);*/

		ads_initget(2+4,NULL);		
		_stprintf(mes, L"\n请输入占地宽 = <%0.2lf>    ",pZDK[i].LZDK);
		rc = ads_getreal(mes,&ZDK);
		if(rc==RTCAN)
			return 1;
		if(rc!=RTNONE)
			pZDK[i].LZDK=ZDK;
		return 1;
	}
	else//对右边占地点循环
	{
		for(i=0;i<NZDK;i++)
		{
			dx = pZDK[i].Rpt[0]-PT.y;
			dy = pZDK[i].Rpt[1]-PT.x;
			dis = sqrt(dx*dx+dy*dy);
			if(dis < 2.0)
				break;
		}
		if(i<NZDK)//改变右边第i个占地点
		{
			/*
			dml=pLine->XLC1(pRZDK[i].cml);	
			ads_initget(2+4,NULL);
			_stprintf(mes, L"\n请输入里程 = <%0.2lf>    ",dml);
			rc=ads_getreal(mes,&Bdml);
			if(rc==RTCAN)
			return 1;
			if(rc!=RTNONE)	
			pRZDK[i].cml=pLine->TYLC(Bdml);*/

			ads_initget(2+4,NULL);		
			_stprintf(mes, L"\n请输入占地宽 = <%0.2lf>  ",pZDK[i].RZDK);
			rc = ads_getreal(mes,&ZDK);
			if(rc==RTCAN)
				return 1;
			if(rc!=RTNONE)
				pZDK[i].RZDK=ZDK;
			return 1;
		}
		else
			return 0; //未删除
	}
}
void GTZDK_ROAD::ChangeZDK()
{
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;

	rc = ads_entsel(L"\nSelect a  占地线上要修改的边界点 : ", en, asDblArray(PT));
	if(rc==RTCAN)
		return;	
	if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }

	AcDbObjectId eId;
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*  acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(GTZDK_ROAD::desc())) 
	{    
		pObj->close(); 
		GTZDK_ROAD  *pzdk;
		/*acdbOpenObject(pzdk, eId, AcDb::kForWrite);*/
		if(acdbOpenObject(pzdk,eId,AcDb::kForWrite)!=Acad::eOk)	
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pzdk->assertWriteEnabled();
		pzdk->ZDKChange(PT);
		pzdk->close();  // Finished with the pline header.                
	}
	else 
	{
		pObj->close();
		ads_printf(L"\nSelected entity is not a GTZDK");
	}
	return  ;   
}

int GTZDK_ROAD::ZDKAdd(AcGePoint3d PT)
{
	/*
	int i,j;
	double dx,dy,dis1,dis2,dis;

	for(i=0;i<NLZDK-1;i++)
	{
	dx = pLZDK[i].pt[0]-PT.y;
	dy = pLZDK[i].pt[1]-PT.x;
	dis1 = sqrt(dx*dx+dy*dy);
	dx = pLZDK[i+1].pt[0]-PT.y;
	dy = pLZDK[i+1].pt[1]-PT.x;
	dis2 = sqrt(dx*dx+dy*dy);
	dx = pLZDK[i+1].pt[0]-pLZDK[i].pt[0];
	dy = pLZDK[i+1].pt[1]-pLZDK[i].pt[1];
	dis = sqrt(dx*dx+dy*dy);
	if(fabs(dis-dis1-dis2) < 1.0)
	break;
	}
	if(i<NLZDK-1)//在左边第i个占地点到第i+1个占地点间添加个占地点
	{
	for(j=NLZDK-1;j>i;j--)
	{
	pLZDK[j+1].cml =pLZDK[j].cml;
	pLZDK[j+1].pt[0] =pLZDK[j].pt[0];
	pLZDK[j+1].pt[1] =pLZDK[j].pt[1];
	pLZDK[j+1].ZDK =pLZDK[j].ZDK;
	}
	pLZDK[i+1].cml = pLZDK[i].cml+dis1*(pLZDK[i+2].cml-pLZDK[i].cml)/dis;
	pLZDK[i+1].ZDK = (pLZDK[i].ZDK+pLZDK[i+2].ZDK)/2.0;
	NLZDK+=1;
	return 1;
	}
	else//对右边占地点循环
	{
	for(i=0;i<NRZDK-1;i++)
	{
	dx = pRZDK[i].pt[0]-PT.y;
	dy = pRZDK[i].pt[1]-PT.x;
	dis1 = sqrt(dx*dx+dy*dy);
	dx = pRZDK[i+1].pt[0]-PT.y;
	dy = pRZDK[i+1].pt[1]-PT.x;
	dis2 = sqrt(dx*dx+dy*dy);
	dx = pRZDK[i+1].pt[0]-pRZDK[i].pt[0];
	dy = pRZDK[i+1].pt[1]-pRZDK[i].pt[1];
	dis = sqrt(dx*dx+dy*dy);
	if(fabs(dis-dis1-dis2) < 1.0)
	break;
	}
	if(i<NRZDK-1)//在左边第i个占地点到第i+1个占地点间添加个占地点
	{
	for(j=NRZDK-1;j>i;j--)
	{
	pRZDK[j+1].cml =pRZDK[j].cml;
	pRZDK[j+1].pt[0] =pRZDK[j].pt[0];
	pRZDK[j+1].pt[1] =pRZDK[j].pt[1];
	pRZDK[j+1].ZDK =pRZDK[j].ZDK;
	}
	pRZDK[i+1].cml = pRZDK[i].cml+dis1*(pRZDK[i+2].cml-pRZDK[i].cml)/dis;
	pRZDK[i+1].ZDK = (pRZDK[i].ZDK+pRZDK[i+2].ZDK)/2.0;
	NRZDK+=1;
	return 1;
	}
	else
	return 0; //未能添加
	}*/
	return 0;
}

int GTZDK_ROAD::ZDKDel(AcGePoint3d PT)
{
	/*
	int i,j;
	double dx,dy,dis;

	for(i=0;i<NLZDK;i++)
	{
	dx = pLZDK[i].pt[0]-PT.y;
	dy = pLZDK[i].pt[1]-PT.x;
	dis = sqrt(dx*dx+dy*dy);
	if(dis < 2.0)
	break;
	}
	if(i<NLZDK)//删除左边第i个占地点
	{
	for(j=i;j<NLZDK-1;j++)
	{
	pLZDK[j].cml =	pLZDK[j+1].cml;
	pLZDK[j].pt[0] =	pLZDK[j+1].pt[0];
	pLZDK[j].pt[1] =	pLZDK[j+1].pt[1];
	pLZDK[j].ZDK =	pLZDK[j+1].ZDK;
	}
	NLZDK-=1;
	return 1;
	}
	else//对右边占地点循环
	{
	for(i=0;i<NRZDK;i++)
	{
	dx = pRZDK[i].pt[0]-PT.y;
	dy = pRZDK[i].pt[1]-PT.x;
	dis = sqrt(dx*dx+dy*dy);
	if(dis < 2.0)
	break;
	}
	if(i<NRZDK)//删除右边第i个占地点
	{
	for(j=i;j<NRZDK-1;j++)
	{
	pRZDK[j].cml =	pRZDK[j+1].cml;
	pRZDK[j].pt[0] =	pRZDK[j+1].pt[0];
	pRZDK[j].pt[1] =	pRZDK[j+1].pt[1];
	pRZDK[j].ZDK =	pRZDK[j+1].ZDK;
	}
	NRZDK-=1;
	return 1;
	}
	else
	return 0; //未删除
	}*/
	return 0;
}

void GTZDK_ROAD::DelZDK()
{
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	int type,track=1;
	struct resbuf entdata;// 鼠标拖动时的动态坐标	
	while (track>0) 
	{  
		rc = ads_entsel(L"\nSelect a  占地线上要删除的边界点 : ", en, asDblArray(PT));
		if(rc==RTCAN)
			return;
		if (rc != RTNORM) {    ads_printf(L"\nError during object selection");  return;   }

		AcDbObjectId eId;
		acdbGetObjectId(eId, en);

		AcDbObject *pObj;
		/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

		if (pObj->isKindOf(GTZDK_ROAD::desc())) 
		{    
			pObj->close(); 
			GTZDK_ROAD  *pzdk;
			/* acdbOpenObject(pzdk, eId, AcDb::kForWrite);*/
			if(acdbOpenObject(pzdk,eId,AcDb::kForWrite)!=Acad::eOk)	
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pzdk->assertWriteEnabled();
			pzdk->ZDKDel(PT);
			pzdk->close();  // Finished with the pline header.                
		}
		else 
		{
			pObj->close();
			ads_printf(L"\nSelected entity is not a GTZDK");
		}

		if(ads_grread (track, &type, &entdata)!=RTNORM)
			break;
		if((type!=5) && (type!=12))
			return;
	}
	return  ;   
}




















