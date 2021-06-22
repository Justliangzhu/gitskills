// DMX_LCB.cpp: implementation of the DMX_LCB class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "DMX_LCB.h"
#include "JD_CENTER.h"
#include "string.h"
#include "drawable.h"
#include "stdafx.h"
#include "BAS_DRAW_FUN.H"
#include "dmx.h"
#include "ORI_SCALE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DMX_LCB_ROAD::DMX_LCB_ROAD()
{
	Modify_DATA();
}
Acad::ErrorStatus  DMX_LCB_ROAD::subTranformBy(const AcGeMatrix3d &)
{
	return Acad::eOk;
}

void DMX_LCB_ROAD::Modify_DATA()
{	
	set_ori_scale();
	NDL = pzLinep->DLNum;
	int i;
	for (i = 0; i < NDL; i++)
	{
		DLB[i].BGH = pzLinep->DLArray[i].BGH;
		DLB[i].EGH = pzLinep->DLArray[i].EGH;
		DLB[i].BLC = pzLinep->DLArray[i].BLC;
		DLB[i].ELC = pzLinep->DLArray[i].ELC;
	}
	DLB[NDL].BLC = DLB[NDL-1].BLC;
	startlc = pzLinep->DLArray[0].ELC;
	length = pzLinep->LenOfLine;
	tarray = pzLinep->XYNum;
	for (i = 0; i <= tarray; i++)
		for (int j = 0; j < 10; j++)
			array[i][j] = pzLinep->XYArray[i][j];
	acdbHostApplicationServices()->workingDatabase()->
		loadLineTypeFile(L"CONTINUOUS",L"acadiso.lin");//
	LcbId = CreateLayer(L"LCB", L"CONTINUOUS");
}

DMX_LCB_ROAD::~DMX_LCB_ROAD()
{

}
void DMX_LCB_ROAD::set_ori_scale()
{
	texth = pORI_SCALE.LCBTEXTH;//font high;
	Boxh = pORI_SCALE.LCBHigh;//box high;
	x_ori = pORI_SCALE.X_ORI;
	y_ori = pORI_SCALE.Y_ORI + pORI_SCALE.ZMTH + 2;
	NLC = pzLinep->NLC;//里程保留位数
	HBVScale = pORI_SCALE.HBVSCALE;
	//20190709	修改采用双轴比例尺
	m_HScale = pORI_SCALE.HSCALE;
	m_VScale = pORI_SCALE.VSCALE;
}

Adesk::Boolean  DMX_LCB_ROAD::BZLC(AcGiWorldDraw* pWd)  
	//  标注公里百米里程标  
	//    zybz=1,-1  标在右边，左边
{
	double DLTexth=0.75*texth;
	pWd->subEntityTraits().setLayer(LcbId);//先设层
	AcGePoint3d  ptb,pt,pte,p1;
	double WB,BZlc,lc,LCB,DDlength,DL,angle;
	int ii,k ,BZW,iDL;
	long int zlc;
	ACHAR GH[8],GH1[8],chrtmp[20],chr[50],ch[50];
	xlpoint  PZtmp; 
	AcDbObjectId eId;
	double dlc = 100.0;

	pte.x = x_ori-29;
	pte.y = y_ori+2;
	_tcscpy(ch,L"里程桩号");
	G_maketext(pWd,pte,ch,0,texth+1.5,4,1);

	_tcscpy(GH,DLB[0].EGH);	   
	WB=0.1*texth;//=1.5*texth;
	LCB=DLB[0].ELC;

	ptb[Z]=0.0;pte[Z]=0.0;pt[Z]=0.0;p1[Z]=0.0;
	// 断链处统一里程
	DDlength=LCB;
	// 逐个断链段标注   	   
	for (iDL=0;iDL<NDL;iDL++)
	{ 
		DL = DLB[iDL].ELC-DLB[iDL].BLC;
		LCB=DLB[iDL].ELC;  //本段起点里程 
		_tcscpy(GH,DLB[iDL].EGH);
		_tcscpy(GH1,DLB[iDL].BGH);

		// 绘断链点 或冠号改变点
		if (iDL>0 && iDL< NDL-1)  // 不为起 终点
		{ // 绘出 iDL点
			lc=DDlength;
			PZtmp.lc=lc;  // 统一里程
			k=xlpoint_pz(array,tarray,&PZtmp);  
			if (k>0&& k<=6)  //有解
			{
				//断链里程标 直线起终点
				ptb[X] = x_ori + (PZtmp.lc - startlc) / m_HScale;	//20190709	修改采用双轴比例尺
				ptb[Y] = y_ori;
				pt[X] = ptb[X];
				pt[Y] = y_ori + 15 * DLTexth * 0.8;

				G_makeline(pWd, ptb, pt, 2);
				// makeline(ptb,pt,5);

				//断链里程标 断前断后里程标注字点
				p1[X]=ptb[X]-0.4*DLTexth;//断前里程
				p1[Y]=ptb[Y]+2*DLTexth;
				pte[X]=ptb[X]+1.2*DLTexth;//断后里程
				pte[Y]=ptb[Y]+2*DLTexth;

				_tcscpy(ch,L"\0");
				_tcscpy(ch,LCchr(GH1,DLB[iDL].BLC,NLC,1));  // 断前里程
				_tcscpy(chr,L"  ");
				_tcscat(chr,ch);   
				pzLinep->G_maketext(pWd,p1,chr,0.5*pi,DLTexth,1,1);  

				_tcscpy(ch,L"\0");
				_tcscpy(ch,LCchr(GH,LCB,NLC,1));     // 断后里程
				_tcscpy(chr,L" ="); 
				_tcscat(chr,ch);   // e.f chr="ZH K10+123.45"  

				if (fabs( DL)>0.001)
				{
					if ( DL>0.0)
					{ 
						_tcscat(chr,L" 短链 "); 
						ads_rtos( DL,2,NLC,ch); _tcscat(chr, ch); 
						_tcscat(chr,L" m");  
					}
					if ( DL<0.0)
					{
						_tcscat(chr,L" 长链 "); 
						ads_rtos(fabs(DL),2,NLC,ch); _tcscat(chr, ch); 
						_tcscat(chr,L" m");  
					}
				}
				G_maketext(pWd,pte,chr,0.5*pi,DLTexth,1,1);
			}
		}

		zlc=(long int)(LCB/dlc);
		BZlc=zlc*dlc;  // 实际里程 
		if ((BZlc-LCB)<0.0001)  { BZlc=BZlc+dlc;  }  
		_tcscpy(GH,DLB[iDL].EGH);  //冠号

		do {
			// 统一里程
			lc=DDlength+BZlc-LCB; 
			PZtmp.lc=lc;  // 统一里程
			k=xlpoint_pz(array,tarray,&PZtmp);   
			BZW=k;    //  K<0  无解
			if (k>0&& k<=6)  //有解
			{
				//里程标 直线起终点,线长度2mm WB字与线间距
				pt[X] = x_ori + (PZtmp.lc - startlc) / m_HScale;	//20190709	修改采用双轴比例尺
				pt[Y] = y_ori + 2;//line
				pte[X] = pt[X] - 0.35 * texth;//左移半个字宽
				pte[Y] = y_ori + (WB + texth);//char
				ptb[X] = pt[X];
				ptb[Y] = y_ori; //line
				G_makeline(pWd, ptb, pt, 2);
				// makeline(ptb,pt,5);

				pt=pte;
				ads_rtos(BZlc,2,0,chr);  

				// 里程百米标
				angle=0;
				ii=_tcslen(chr);
				if(ii>20) ii=19;
				if (dlc>=100.0&&ii>=2)
				{ 
					chr[ii-1]='\0';chr[ii-2]='\0';
					_tcscpy(ch,chr);
					if (ii>3&&chr[ii-3]=='0') // 公里标
					{
						chr[ii-3]='\0';  _tcscpy(ch,GH); _tcscat(ch,chr); 
						angle=0.5*pi;pte[X]+=0.8*texth;
					}
					else if(ii>3 && chr[ii-3]!='0')    // 百米标
					{
						ch[0]=chr[ii-3];ch[1]='\0';
					}		
				}
				else
				{
					_tcscpy(ch,GH);
					if ( ii<=2)
					{ 
						_tcscpy(ch,L"+"); ch[1]='0';
						ch[2]=chr[ii-2]; ch[3]=chr[ii-1];ch[4]='\0';
					}
					else if ( ii>=3)
					{
						_tcscpy(chrtmp,L"+"); chrtmp[1]=chr[ii-3];
						chrtmp[2]=chr[ii-2]; chrtmp[3]=chr[ii-1];chrtmp[4]='\0';
						if (ii>3&&chr[ii-3]=='0' && chr[ii-2]=='0'&&chr[ii-1]=='0') // 公里标
						{
							chr[ii-3]='\0';
							_tcscpy(ch,GH);
							_tcscat(ch,chr); _tcscat(ch,chrtmp);
							angle=0.5*pi;
							pte[X]+=0.8*texth;
						}
						else if (ii>3&&chr[ii-3]!='0'){_tcscpy(ch,chrtmp);}
					}
				}
				G_maketext(pWd,pte,ch,angle,texth,4,1);
			}                    
			BZlc=BZlc+dlc;
			if ( BZlc>DLB[iDL+1].BLC) BZW=-99;	
			//		   ads_printf(L"%lf %d %lf\n",BZlc,BZW,iDL,DLB[iDL+1].BLC);
		}while (BZW>0);
		DDlength=  DDlength+DLB[iDL+1].BLC-LCB;//断链处统一里程。

		if (iDL==0)   // 为起 终点
		{
			lc=startlc;
			ptb[X] = x_ori + (lc - startlc) / m_HScale;		//20190709	修改采用双轴比例尺
			ptb[Y]=y_ori; 
			pt[X]=ptb[X];
			pt[Y]=y_ori+25;		  
			G_makeline(pWd,ptb,pt,2);
			// makeline(ptb,pt,5);

			p1[X]=ptb[X]-0.5*texth;
			p1[Y]=ptb[Y]+6.5*DLTexth;		  
			_tcscpy(ch,L"\0");
			_tcscpy(ch,LCchr(GH1,DLB[iDL].BLC,NLC,1));  
			_tcscpy(chr,L"  ");
			_tcscat(chr,ch);   
			pzLinep->G_maketext(pWd,p1,chr,0.5*pi,0.8*texth,4,1);  		  
		}
		if(iDL==NDL-1)
		{			
			ptb[X] = x_ori + length / m_HScale;	//20190709	修改采用双轴比例尺
			ptb[Y]=y_ori; 
			pt[X]=ptb[X];
			pt[Y]=y_ori+25;		  
			G_makeline(pWd,ptb,pt,2);
			// makeline(ptb,pt,5);

			p1[X]=ptb[X]-0.5*texth;
			p1[Y]=ptb[Y]+6.5*DLTexth;		  
			_tcscpy(ch,L"\0");
			_tcscpy(ch,LCchr(GH1,DLB[iDL].BLC,NLC,1)); 
			_tcscpy(chr,L"  ");
			_tcscat(chr,ch);   
			pzLinep->G_maketext(pWd,p1,chr,0.5*pi,0.8*texth,4,1);  		  
		}	  
	} // IDL
	return 1;
}
/*




Adesk::Boolean  DMX_LCB::BZLC(AcGiWorldDraw* pWd)  
//  标注公里百米里程标  
//    zybz=1,-1  标在右边，左边

{
pWd->subEntityTraits().setLayer(LcbId);//先设层

AcGePoint3d  ptb,pt,pte,p1;
double  WB;
int kk ;
char gh[8],GH[8],ch[50],str[80];
AcDbObjectId eId;
double xlc;

_tcscpy(GH,DLB[0].EGH);

WB=0.5*texth;
//----------------------------------------------
pzLinep->FormLCB(100.0,pzLinep->StartCml,pzLinep->EndCml);  
//----------------------------------------------
for(int i=0; i<pzLinep->NLCB; i++)
{
ptb.x = x_ori+(pzLinep->pLCB[i].lc-startlc)/HBVScale; 
ptb.y = y_ori;
pte.x =ptb.x , pte.y= ptb.y+Boxh/5;
makeline(ptb,pte,0);

pte[Y]=pte.y+Boxh/5.0;
pte.x -= 0.3*texth;
if(pzLinep->pLCB[i].lcflag==-1)
{
xlc=pzLinep->XLC(pzLinep->pLCB[i].lc,gh,pzLinep->NDL);
_tcscpy(ch,L"\0");   _tcscpy(ch,LCchr(gh,xlc,2,1));
kk = F_num(ch,'+');
for (int j=0; j<kk; j++)
str[j]=ch[j];
str[kk-1]='\0';
G_maketext(pWd,pte,str,0,texth,4,1);     
}
else
{
if(pzLinep->pLCB[i].lcflag)
{
_stprintf(ch,L"%d",pzLinep->pLCB[i].lcflag);
G_maketext(pWd,pte,ch,0,texth,4,1);     
}
}
}
return 1;
}*/


Adesk::Boolean  DMX_LCB_ROAD::BZDL(AcGiWorldDraw* pWd)  
{
	AcGePoint3d  ptb,pt,pte,p1;
	double lc,LCB,DDlength,DL;
	int iDL;
	ACHAR GH[8],GH1[8],chr[50],ch[50];
	AcDbObjectId eId;

	DDlength = DLB[0].ELC;

	for (iDL=0;iDL<pzLinep->DLNum;iDL++)
	{ 
		DL=DLB[iDL].ELC-DLB[iDL].BLC;
		LCB=DLB[iDL].ELC;  //本段起点里程 
		_tcscpy(GH,DLB[iDL].EGH);
		_tcscpy(GH1,DLB[iDL].BGH);
		// ads_printf(L"\n iDL=%d %s %10.3f  %s %10.3f",iDL,DLB[iDL].BGH,DLB[iDL].BLC,DLB[iDL].EGH,DLB[iDL].BLC+DL);

		// 绘断链点 或冠号改变点
		if (iDL>0 && iDL< NDL-1)  // 不为起 终点
		{ // 绘出 iDL点
			lc=DDlength;
			ptb[X] = x_ori + (lc - startlc) / m_HScale;	//20190709	修改采用双轴比例尺
			ptb[Y]=y_ori + 10 + Boxh; 
			pt[X]=ptb[X];
			pt[Y]=ptb[Y]+50;
			G_makeline(pWd,ptb,pt,2);

			p1[X]=ptb[X]-0.2*texth;
			p1[Y]=ptb[Y];
			pte[X]=ptb[X]+0.6*texth;
			pte[Y]=ptb[Y];

			_tcscpy(ch,L"\0");
			_tcscpy(ch,LCchr(GH1,DLB[iDL].BLC,NLC,1));  // 断前里程
			_tcscpy(chr,L"  ");
			_tcscat(chr,ch);   
			pzLinep->G_maketext(pWd,p1,chr,0.5*pi,0.5*texth,1,1);  

			_tcscpy(ch,L"\0");
			_tcscpy(ch,LCchr(GH,LCB,NLC,1));     // 断后里程
			_tcscpy(chr,L" ="); 
			_tcscat(chr,ch);   // e.f chr="ZH K10+123.45"  

			if (fabs( DL)>0.001)
			{  if ( DL>0.0)   { 
				_tcscat(chr,L" 短链 "); 
				ads_rtos( DL,2,NLC,ch); _tcscat(chr, ch); 
				_tcscat(chr,L" m");  };
			if ( DL<0.0)   { 
				_tcscat(chr,L" 长链 "); 
				ads_rtos(fabs(DL),2,NLC,ch); _tcscat(chr, ch); 
				_tcscat(chr,L" m");  };
			};

			G_maketext(pWd,pte,chr,0.5*pi,0.5*texth,1,1);     
		}

		DDlength =  DDlength+DLB[iDL+1].BLC-LCB;//断链处统一里程。
	}
	return true;
}


Acad::ErrorStatus DMX_LCB_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();

	char  *chr=NULL;
	AcDbObject::dwgInFields(pFiler);
	// For wblock filing we wrote out our owner as a hard
	// pointer ID so now we need to read it in to keep things
	// in sync.
	//
	if (pFiler->filerType() == AcDb::kWblockCloneFiler) {
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}

	pFiler->readItem(&NDL);
	pFiler->readItem(&tarray);
	pFiler->readItem(&NLC);
	pFiler->readItem(&startlc);
	pFiler->readItem(&length);
	pFiler->readItem(&x_ori); 
	pFiler->readItem(&y_ori);
	pFiler->readItem(&texth);
	pFiler->readItem(&HBVScale); 
	pFiler->readItem(&m_HScale);	//20190709	修改采用双轴比例尺
	pFiler->readItem(&m_VScale);
	for (int i=0;i<tarray;i++)
	{   for (int j=0;j<=6;j++)  
	pFiler->readItem( &array[i][j]);
	}
	/*
	for(i=0;i<NDL;i++)
	{
	pFiler->readItem(DLB[i].BGH);
	pFiler->readItem(DLB[i].EGH);
	pFiler->readItem(&DLB[i].BLC);
	pFiler->readItem(&DLB[i].DL);
	}*/


	//ads_printf(L"\n dwg RR NJD=%d,NDL=%d NLC=%d NH=%d Chinese_Engbz_type=%d",NJD,NDL,NLC,NH,Chinese_Engbz_type); 
	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus DMX_LCB_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());

	pFiler->writeItem(NDL);
	pFiler->writeItem(tarray);
	pFiler->writeItem(NLC);
	pFiler->writeItem(startlc);
	pFiler->writeItem(length);
	pFiler->writeItem(x_ori); 
	pFiler->writeItem(y_ori);
	pFiler->writeItem(texth);
	pFiler->writeItem(HBVScale); 
	pFiler->writeItem(m_HScale);	//20190709	修改采用双轴比例尺
	pFiler->writeItem(m_VScale);
	for (int i=0;i<tarray;i++)
	{   for (int j=0;j<=6;j++)  
	pFiler->writeItem(array[i][j]);
	}
	/*
	for(i=0;i<NDL;i++)
	{
	pFiler->writeItem(DLB[i].BGH);
	pFiler->writeItem(DLB[i].EGH);
	pFiler->writeItem(DLB[i].BLC);
	pFiler->writeItem(DLB[i].DL);
	}*/


	return pFiler->filerStatus();
}
// Files data in from a DXF file.
//
Acad::ErrorStatus DMX_LCB_ROAD::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	const char *chr=NULL;
	Acad::ErrorStatus es;
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }

	// Check if we're at the right subclass getData marker.

	if (!pFiler->atSubclassData(L"DMX_LCB")) {
		ads_printf(L"\n error 10010 类型有错 \n");
		return Acad::eBadDxfSequence;
	}
	struct resbuf inbuf;


	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
		if ( inbuf.restype == AcDb:: kDxfInt16)
			NDL = inbuf.resval.rint;
	}
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
		if ( inbuf.restype == AcDb:: kDxfInt16+1)
			NLC = inbuf.resval.rint;	
	}
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
		if ( inbuf.restype == AcDb:: kDxfReal)
			startlc = inbuf.resval.rreal;	
	}			
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
		if ( inbuf.restype == AcDb:: kDxfReal)
			length = inbuf.resval.rreal;	
	}			
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
		if ( inbuf.restype == AcDb:: kDxfReal)
			x_ori = inbuf.resval.rreal;	
	}			
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
		if ( inbuf.restype == AcDb:: kDxfReal)
			y_ori = inbuf.resval.rreal;	
	}		
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
		if ( inbuf.restype == AcDb:: kDxfReal)
			texth = inbuf.resval.rreal;	
	}			
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
		if ( inbuf.restype == AcDb:: kDxfReal)
			HBVScale = inbuf.resval.rreal;	
	}			
	//	acutPrintf(L"out %d,%d,%d,%d,%d,%d,%d,%d,%d\n",NJD,NDL,NLC,NH,Chinese_Engbz_type,draw_zybz,JDTDBZ,qxbbz,JD0);


	for (int i=0;i<tarray;i++)
	{   
		for (int j=0;j<=6;j++)  
		{
			es = pFiler->readItem(&inbuf);
			{if  ( es == Acad::eOk)
				if ( inbuf.restype == AcDb:: kDxfReal+1)
					array[i][j] = inbuf.resval.rreal;}
		}
	}			

	/*
	for(i=0;i<NDL;i++)
	{
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
	if ( inbuf.restype == AcDb::kDxfText)
	_tcscpy(DLB[i].BGH,inbuf.resval.rstring);
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
	if ( inbuf.restype == AcDb::kDxfText+1)
	_tcscpy(DLB[i].EGH,inbuf.resval.rstring);}	
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
	if ( inbuf.restype == AcDb:: kDxfReal+2)
	DLB[i].BLC = inbuf.resval.rreal;}
	es = pFiler->readItem(&inbuf);
	{if  ( es == Acad::eOk)
	if ( inbuf.restype == AcDb:: kDxfReal+3)
	DLB[i].DL = inbuf.resval.rreal;}
	}
	*/

	return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus DMX_LCB_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	const char* chr=NULL;
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass,L"DMX_LCB");
	pFiler->writeItem(AcDb::kDxfInt16,NDL);
	pFiler->writeItem(AcDb::kDxfInt16+1,tarray);
	pFiler->writeItem(AcDb::kDxfInt16+1,NLC);
	pFiler->writeItem(AcDb::kDxfReal,startlc);
	pFiler->writeItem(AcDb::kDxfReal,length );
	pFiler->writeItem(AcDb::kDxfReal,x_ori);
	pFiler->writeItem(AcDb::kDxfReal,y_ori);
	pFiler->writeItem(AcDb::kDxfReal,texth);
	pFiler->writeItem(AcDb::kDxfReal,HBVScale);
	//	acutPrintf(L"out %d,%d,%d,%d,%d,%d,%d,%d,%d\n",NJD,NDL,NLC,NH,Chinese_Engbz_type,draw_zybz,JDTDBZ,qxbbz,JD0);	
	for (int i=0;i<tarray;i++)
	{   for (int j=0;j<=6;j++)  
	pFiler->writeItem(AcDb::kDxfReal+1,array[i][j]);
	}
	/*
	for(i=0;i<NDL;i++)
	{
	chr= _tcsdup(DLB[i].BGH );pFiler->writeItem(AcDb::kDxfText, chr);
	chr= _tcsdup(DLB[i].EGH );pFiler->writeItem(AcDb::kDxfText+1, chr);

	pFiler->writeItem(AcDb::kDxfReal+2,DLB[i].BLC);
	pFiler->writeItem(AcDb::kDxfReal+3,DLB[i].DL);
	}*/


	return pFiler->filerStatus();
}

Adesk::Boolean  DMX_LCB_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{   

	BZLC(pWd);//标注里程

	//	BZDL(pWd);//标注断链
	AcGePoint3d Pt,*verts=new AcGePoint3d[2];
	AcGeVector3d  normal(0.0,0.0,1.0);
	//	if ( mode_display == 0)
	//	AcDbObjectId DmxId = CreateLayer(L"DMX",L"CONTINUOUS");
	verts[0].x=x_ori;
	verts[0].y=y_ori;
	verts[0].z=0.0;
	verts[1].x = x_ori + length / m_HScale;	//20190709	修改采用双轴比例尺
	verts[1].y=y_ori;
	verts[1].z=0.0;
	if (pWd!= NULL) 
	{
		//			pWd->subEntityTraits().setSelectionMarker(1);
		pWd->subEntityTraits().setLayer(LcbId);//先设层
		pWd->subEntityTraits().setColor(6);	
		pWd->geometry().polyline(2,verts);
	} 
	else
	{
		assert(Adesk::kFalse);//?	
	}
	if(verts) delete [] verts;
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}


Adesk::Boolean  DMX_LCB_ROAD::G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor)
{     
	AcGePoint3d centerPt,midPt;

	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);	

	//	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);	
	pWd->geometry().polyline(2,pArr);
	return  0;
}

Adesk::Boolean  DMX_LCB_ROAD::G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode=1)
{   

	AcGeVector3d normal(0.0,0.0,1.0), direction(cos(ang), sin(ang), 0.0);
	pWd->subEntityTraits().setColor(icolor);	
	AcGiTextStyle pTextStyle;
	pTextStyle.setFont(L"宋体",0,0,134,2);
	pTextStyle.setTextSize(texth);
	pWd->geometry().text(Pt,normal,direction,ctext,_tcslen(ctext),Adesk::kTrue,pTextStyle);
	return 1;
}

