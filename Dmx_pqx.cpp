// DMX_PQX.cpp: implementation of the DMX_PQX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdarx.h"
#include "resource.h"
#include "DMX_PQX.h"
#include "DMX.h"
#include "ORI_SCALE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define AppName  /*MSG1*/L"GUADS" //?????????????

AcGiWorldDraw* pWd_pqx;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AcDbObjectId DMX_PQX_ROAD::CreateLayer(ACHAR *LayerName, ACHAR *LineType)
{
	AcDbLayerTable *pLayerTable;
	//	AcDbObjectId LayerTabRecordId;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForRead);
	AcDbObjectId LayerTabRecordId;
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) == Acad::eOk)
	{
		pLayerTable->close();
		return LayerTabRecordId;
	}
	pLayerTable->close();

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	AcDbLayerTableRecord *pLayerTableRecord=new AcDbLayerTableRecord;
	pLayerTableRecord->setName(LayerName);
	AcDbLinetypeTable *pLinetypeTble;
	acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTble,AcDb::kForRead);
	AcDbObjectId ltypeObjId;
	pLinetypeTble->getAt(LineType,ltypeObjId,Adesk::kTrue);
	//	acutPrintf(L"\nId=%d",ltypeObjId);
	pLayerTableRecord->setLinetypeObjectId(ltypeObjId);
	pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	pLinetypeTble->close();
	return LayerTabRecordId;
}
Adesk::Boolean  DMX_PQX_ROAD::G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode=1)//在头文件中不能写成mode=1,应为mode
{   

	AcGeVector3d normal(0.0,0.0,1.0), direction(cos(ang), sin(ang), 0.0);
	// ads_printf(L"\n             %10.3f %10.3f %10.4f ",Pt.x,Pt.y,dms_rad(ang));
	pWd->subEntityTraits().setColor(icolor);	
	//    pWd->geometry().text(Pt , normal, direction, texth,1.0, 0.0, ctext);
	AcGiTextStyle pTextStyle;
	//pTextStyle.setFont(L"宋体",TRUE,FALSE,0,0);
	pTextStyle.setFont(L"宋体",0,0,134,2);
	pTextStyle.setTextSize(texth);
	AcGePoint3d Center;
	int ntext=_tcslen(ctext);
	if(mode==1)//调整为中心对齐
	{
		Center.x=Pt.x-0.5*texth*cos(ang+0.5*PI)-0.4*ntext*texth*cos(ang);
		Center.y=Pt.y-0.5*texth*sin(ang+0.5*PI)-0.4*ntext*texth*sin(ang);
	}
	else
	{
		Center=Pt;
	}
	pWd->geometry().text(Center,normal,direction,ctext,_tcslen(ctext),Adesk::kTrue,pTextStyle);
	return 1;
}


DMX_PQX_ROAD::~DMX_PQX_ROAD()
{	
}
DMX_PQX_ROAD::DMX_PQX_ROAD()//不可少
{

	set_HScale_ori();	
	//	Modify_DATA(num,array);

	acdbHostApplicationServices()->workingDatabase()->
		loadLineTypeFile(L"CONTINUOUS",L"acadiso.lin");//
	PqxId = CreateLayer(L"PQX",L"CONTINUOUS");
}

Adesk::Boolean  DMX_PQX_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{   
	AcGePoint3d Pt;
	JD_CENTER *pz;
	Pt.set(x_ori,y_ori,0);
	if (m_Frame_Index == -1)
	{
		m_Frame_Index = GlobalZDMTK.FindWichZdm(Pt);
		ads_printf(L"错误：地面线标注寻找图框号\n");	//20190712	图框号改为外部传递，如果有遗漏的命令，这里提示
	}

	pz = GlobalZDMTK.ZDMTK[m_Frame_Index].pm;
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"zm:无法找到平面方案!\n");

	draw_plan(pWd);//绘平面      
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}


Acad::ErrorStatus DMX_PQX_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
{
	assertWriteEnabled();
	AcDbObject::dwgInFields(pFiler);
	// For wblock filing we wrote out our owner as a hard
	// pointer ID so now we need to read it in to keep things
	// in sync.
	//
	if (pFiler->filerType() == AcDb::kWblockCloneFiler) {
		AcDbHardPointerId id;
		pFiler->readItem(&id);
	}	
	/////////////////add///////////////

	pFiler->readItem( &NUM);
	pFiler->readItem( &High);
	pFiler->readItem( &HBVScale);
	pFiler->readItem(&x_ori);
	pFiler->readItem(&y_ori);
	pFiler->readItem(&m_HScale);
	pFiler->readItem(&m_VScale);
	pFiler->readItem(&m_Frame_Index);
	//	acutPrintf(L"\n%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf\n",
	//		startLC,endLC,H_ori,HScale,VScale,ZMax,ZMin,x_ori,y_ori);

	//	acutPrintf(L"\ndwgin NUM=%d",NUM);
	//	acedAlert(L"dwgin2");
	//////////////////////////////
	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus DMX_PQX_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());
	/////////////////add///////////////
	pFiler->writeItem( NUM);
	pFiler->writeItem( High);
	pFiler->writeItem( HBVScale);
	pFiler->writeItem( x_ori);
	pFiler->writeItem( y_ori);
	pFiler->writeItem(m_HScale);
	pFiler->writeItem(m_VScale);
	pFiler->writeItem(m_Frame_Index);
	//	acutPrintf(L"\n%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf\n",
	//		startLC,endLC,H_ori,HScale,VScale,ZMax,ZMin,x_ori,y_ori);

	//	acutPrintf(L"\ndwgin NUM=%d",NUM);
	//	acutPrintf(L"\nwrite.dwgout NUM=%d",NUM);
	///////////////////////////////
	return pFiler->filerStatus();
}

// Files data in from a DXF file.

Acad::ErrorStatus DMX_PQX_ROAD::dxfInFields(AcDbDxfFiler* pFiler)
{
	//    acedAlert(L"dxfin1");
	assertWriteEnabled();
	Acad::ErrorStatus es;
	struct resbuf inbuf;//结果缓冲区
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }

	// Check if we're at the right subclass getData marker.

	if (!pFiler->atSubclassData(L"DMX_PQX")) {
		ads_printf(L"\n error 10010 类型有错 \n");
		return Acad::eBadDxfSequence;
	}
	/////////////////add///////////////

	es = pFiler->readItem(&inbuf);	
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32 )
		{	NUM= inbuf.resval.rint;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);	
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb::kDxfReal )
		{	High= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);	
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb::kDxfReal )
		{	HBVScale= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);	
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb::kDxfReal+1 )
		{	x_ori= inbuf.resval.rreal;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);	
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb::kDxfReal+2 )
		{	y_ori= inbuf.resval.rreal;	}
		else		{		}
	}

	//	acedAlert(L"dxfin2");
	return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus DMX_PQX_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass, "DMX_PQX");///////////"classname  DMX_PQX"//add///////////
	pFiler->writeItem(AcDb::kDxfInt32,NUM);
	pFiler->writeItem( AcDb::kDxfReal,High);
	pFiler->writeItem( AcDb::kDxfReal,HBVScale);
	pFiler->writeItem( AcDb::kDxfReal+1,x_ori);
	pFiler->writeItem( AcDb::kDxfReal+2,y_ori);
	//cutPrintf("\ndxfout NUM=%d",NUM);

	return pFiler->filerStatus();
}


void DMX_PQX_ROAD::set_HScale_ori()
{	
	High = pORI_SCALE.PQXHIGH;
	x_ori = pORI_SCALE.X_ORI;
	y_ori = pORI_SCALE.Y_ORI - pORI_SCALE.DESHIGH - pORI_SCALE.EARHIGH - High*0.5;
	HBVScale = pORI_SCALE.HBVSCALE;
	m_HScale = pORI_SCALE.HSCALE;
	m_VScale = pORI_SCALE.VSCALE;

	texth = pORI_SCALE.PQXTEXTH;
}

double DMX_PQX_ROAD::xyddaa(double xb,double yb,double xe,double ye,double *f)
{	
	double dx,dy,dd,aa;

	dx=xe-xb;dy=ye-yb;dd=sqrt(dx*dx+dy*dy);
	if (fabs(dx)<0.000000001 )
	{
		if (dy>=0.0) aa=pi*0.5;
		else if (dy<0.0) aa=pi*1.5;
	}
	else {  aa=atan(dy/dx);};

	if (dy>=0 && dx<-0.000000001) { aa=aa+pi;}
	else if (dy<0 && dx<-0.000000001) { aa=aa+pi;}
	else if (dy<0 && dx>0.000000001) { aa=aa+2*pi; };
	if (aa<-0.000000001) { aa=aa+2*pi;}
	else if (aa>2*pi) { aa=aa-2*pi;};
	*f=aa;
	return(dd);
}

void   DMX_PQX_ROAD::draw_plan (AcGiWorldDraw* pWd)//绘平面      
{
	double  ym,  yl, yr;
	double  sspt[3],eept[3];
	double  len;
	double  ang,l0;
	ACHAR    str[80];
	int i;
	AcGePoint3dArray Ptarr;
	AcGePoint3d spt,ept,tpt;

	ym  = y_ori ;//平面栏中间高度
	yr  = ym+High/4.0;//平面栏上部圆曲线高度,曲线turn right
	yl  = ym-High/4.0;//平面栏下部圆曲线高度,曲线turn left

	tpt.x = x_ori - 29;
	tpt.y = ym;
	_tcscpy(str,L"线路平面");
	G_maketext(pWd,tpt,str,0,texth-0.5,4,0);

	for(i=1;i<pzLinep->XYNum;i++)
	{
		spt[X]=cmltosx(pzLinep->XYArray[i][6]);
		ept[X]=cmltosx(pzLinep->XYArray[i+1][6]);
		tpt[X] = 0.5*(spt[X]+ept[X]);	 
		len = pzLinep->XYArray[i+1][6] - pzLinep->XYArray[i][6];		    
		if(pzLinep->XYArray[i][0] < 1.2) //当前第i个线元为直线
		{
			spt[Y]=ept[Y]=ym;
			G_makeline(pWd,spt,ept,2);
			tpt[Y] = ym - 1.0 -texth;
			//		   _stprintf(str,L"%0.2lf",len);
			_tcscpy(str,DoubleToStr(len));
			G_maketext(pWd,tpt,str,0,texth,0,1);
		}
		else if(pzLinep->XYArray[i][0]<2.2)//当前第i个线元为园
		{
			if(pzLinep->XYArray[i][4]<0)//左转
			{
				spt[Y]=ept[Y]=yl;
				tpt[Y] = ym +texth;
			}
			else//右转
			{
				spt[Y]=ept[Y]=yr;
				tpt[Y] = ym - 1.0 -texth;
			}
			G_makeline(pWd,spt,ept,2);
			//			_stprintf(str,L"R-%0.2lf",pzLinep->XYArray[i][1]);
			_stprintf(str,L"R-%s",DoubleToStr(pzLinep->XYArray[i][1]));
			G_maketext(pWd,tpt,str,0,texth,0,1);
			if(fabs(pzLinep->XYArray[i-1][0]-1)<0.01 ||((fabs(pzLinep->XYArray[i-1][0]-4)<0.01||fabs(pzLinep->XYArray[i-1][0]-2)<0.01)&&pzLinep->XYArray[i-1][4]*pzLinep->XYArray[i][4]<-0.1))//前一线元为直线 或为后缓或圆并转向相反
			{
				if(pzLinep->XYArray[i][4]<0)//左转
				{
					spt[Y]=ym;
					tpt[X]=spt[X];
					tpt[Y] = yl;
				}
				else//右转
				{
					spt[Y]=ym;
					tpt[X]=spt[X];
					tpt[Y] = yr;
				}
				G_makeline(pWd,spt,tpt,2);
			}
			if(fabs(pzLinep->XYArray[i+1][0]-1) < 0.01||((fabs(pzLinep->XYArray[i+1][0]-3)<0.01||fabs(pzLinep->XYArray[i+1][0]-2)<0.01)&&pzLinep->XYArray[i][4]*pzLinep->XYArray[i+1][4]<-0.1))//后一线元为直线 或为前缓并转向相反
			{
				if(pzLinep->XYArray[i][4]<0)//左转
				{
					ept[Y]=ym;
					tpt[X]=ept[X];
					tpt[Y] = yl;
				}
				else//右转
				{
					ept[Y]=ym;
					tpt[X]=ept[X];
					tpt[Y] = yr;
				}
				G_makeline(pWd,ept,tpt,2);
			}
		}
		else if(pzLinep->XYArray[i][0]<3.2)//当前第i个线元为前缓
		{
			spt[Y]=ym;
			if(pzLinep->XYArray[i][4]<0)//左转
			{
				ept[Y]=yl;
				tpt[Y] = ym +texth;
			}
			else//右转
			{
				ept[Y]=yr;
				tpt[Y] = ym - 1.0 -texth;
			}			
			G_makeline(pWd,spt,ept,2);	

			sspt[0]=spt[X];
			sspt[1]=spt[Y];
			eept[0]=ept[X];
			eept[1]=ept[Y];
			ang=ads_angle(sspt,eept);
			if(pzLinep->XYArray[i][4]<0)//左转
				tpt[Y]= ym-2*texth;
			else
				tpt[Y]= ym;
			l0=pzLinep->XYArray[i][1]*pzLinep->XYArray[i][1]/pzLinep->XYArray[i+1][7];
			_stprintf(str,L"l-%s",DoubleToStr(l0));
			//			_stprintf(str,L"l-%0.2lf",pzLinep->XYArray[i][1]*pzLinep->XYArray[i][1]/pzLinep->XYArray[i+1][7]);	

			G_maketext(pWd,tpt,str,ang,texth,0,1);	
			tpt[Y] = tpt[Y]+2*texth;
			_stprintf(str,L"A-%s",DoubleToStr(pzLinep->XYArray[i][1]));
			//			_stprintf(str,L"A-%0.2lf",pzLinep->XYArray[i][1]);	
			G_maketext(pWd,tpt,str,ang,texth,0,1);				
		}
		else if(pzLinep->XYArray[i][0]<4.2)//当前第i个线元为后缓
		{
			ept[Y]=ym;
			if(pzLinep->XYArray[i][4]<0)//左转
			{
				spt[Y]=yl;
				tpt[Y] = ym +texth;
			}
			else//右转
			{
				spt[Y]=yr;
				tpt[Y] = ym - 1.0 -texth;
			}			
			G_makeline(pWd,spt,ept,2);

			sspt[0]=spt[X];
			sspt[1]=spt[Y];
			eept[0]=ept[X];
			eept[1]=ept[Y];
			ang=ads_angle(sspt,eept);
			if(pzLinep->XYArray[i][4]<0)//左转
				tpt[Y]= ym-2*texth;
			else
				tpt[Y]= ym;
			l0=pzLinep->XYArray[i][1]*pzLinep->XYArray[i][1]/pzLinep->XYArray[i][7];
			_stprintf(str,L"l-%s",DoubleToStr(l0));
			//			_stprintf(str,L"l-%0.2lf",pzLinep->XYArray[i][1]*pzLinep->XYArray[i][1]/pzLinep->XYArray[i][7]);	
			G_maketext(pWd,tpt,str,ang,texth,0,1);	
			tpt[Y] = tpt[Y]+2*texth;
			_stprintf(str,L"A-%s",DoubleToStr(pzLinep->XYArray[i][1]));
			//			_stprintf(str,L"A-%0.2lf",pzLinep->XYArray[i][1]);	
			G_maketext(pWd,tpt,str,ang,texth,0,1);	
		}
		else if(pzLinep->XYArray[i][0]<5.2)//当前第i个线元为不完整缓R大->R小
		{
			if(pzLinep->XYArray[i][4]<0)//左转
			{
				spt[Y]=ept[Y]=yl;
				tpt[Y] = ym +texth;
			}
			else//右转
			{
				spt[Y]=ept[Y]=yr;
				tpt[Y] = ym - 1.0 -texth;
			}
			G_makeline(pWd,spt,ept,1);			
		}
		else//当前第i个线元为不完整缓R小->R大
		{
			if(pzLinep->XYArray[i][4]<0)//左转
			{
				spt[Y]=ept[Y]=yl;
				tpt[Y] = ym +texth;
			}
			else//右转
			{
				spt[Y]=ept[Y]=yr;
				tpt[Y] = ym - 1.0 -texth;
			}
			G_makeline(pWd,spt,ept,1);	
		}
	}

	/*  xb = x_ori;
	xe = cmltosx(pzLinep->EndCml);   
	x[0][1] = x[1][1] = x[4][1] = ym;//一个平曲线在中间的三个点

	double XlSml,XlEml;
	XlSml = pzLinep->StartCml;
	XlEml = pzLinep->EndCml;
	Ptarr.setLogicalLength(5);
	bml = XlSml;
	for(i=0;i<pzLinep->NJD;i++)//QXB:zh里程,hy里程,yh里程,hz里程,a,T1,T2,L,左偏为-1(右1)
	{
	if(len > 0.0)//夹直线长度>0
	{
	spt[X] = cmltosx(bml+0.5*len);	 
	spt[Y] = ym - 1.0 -texth;
	_stprintf(str,L"%0.2lf",len);
	G_maketext(pWd,spt,str,0,texth,0,2);
	}
	if(pzLinep->JDarray[i][4]>0.5)//有半径
	{
	atodmss(pzLinep->QXB[i][4], d, mm, ss);
	spt[X] = (x[3][0]+x[2][0])/2.0;//圆曲线中间
	st = x[1][0] + 0.6 * (x[2][0]-x[1][0]);
	et = x[4][0] - 0.6 * (x[4][0]-x[3][0]);
	se = et -st;
	if ( (se > 24 && pzLinep->JDarray[i][4] > 100.0) || se >32 )//圆曲线长 及缓长>100 绘制两行
	{             
	if (pzLinep->QXB[i][8] < 0.0)//左转
	{				  
	spt[Y]= yl+6*High/20;
	tpt[X] = spt[X] - 10*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "az-%d°%s%s R-%4.0lf lo-%3.0lf", d, mm, ss, pzLinep->JDarray[i][4],pzLinep->JDarray[i][3]);
	G_maketext(pWd,tpt,str,0,texth,0,2);

	spt[Y]= yl+3*High/20;
	tpt[X] = spt[X] - 7*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "T-%6.2lf L-%6.2lf", pzLinep->QXB[i][5],pzLinep->QXB[i][7]);
	G_maketext(pWd,tpt,str,0,texth,0,2);
	};
	if (pzLinep->QXB[i][8] > 0.0)//右转
	{
	spt[Y]= yr-4*High/20;
	tpt[X] = spt[X] - 10*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "ay-%d°%s%s R-%4.0lf l-%3.0lf", d, mm, ss, pzLinep->JDarray[i][4],pzLinep->JDarray[i][3]);
	G_maketext(pWd,tpt,str,0,texth,0,2);
	spt[Y]= yr-7*High/20;
	tpt[X] = spt[X] - 7*texth;
	tpt[Y] = spt[Y];

	_stprintf(str, "T-%6.2lf L-%6.2lf", pzLinep->QXB[i][5],pzLinep->QXB[i][7]);
	G_maketext(pWd,tpt,str,0,texth,0,2);
	};
	}
	else if ( se > 17 || ( se > 12 && pzLinep->JDarray[i][4] > 80 )) //圆曲线长 及缓长>80 绘制三行
	{
	if (pzLinep->QXB[i][8] < 0.0)//左转
	{
	spt[Y]= yl+9.0*High/20;
	tpt[X] = spt[X] - 5*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "az-%d°%s%s R-%4.0lf", d, mm,ss,pzLinep->JDarray[i][4]);
	G_maketext(pWd,tpt,str,0,texth,0,2);
	spt[Y]= yl+5.0*High/20;
	tpt[X] = spt[X] - 2.5*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "l-%3.0lf T-%6.2lf", pzLinep->JDarray[i][3],pzLinep->QXB[i][5]);
	G_maketext(pWd,tpt,str,0,texth,0,2);

	spt[Y] = yl+1.0*High/20;
	tpt[X] = spt[X] - 1.25*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "L-%6.2lf",pzLinep->QXB[i][7]);
	G_maketext(pWd,tpt,str,0,texth,0,2);
	};
	if (pzLinep->QXB[i][8] > 0.0)//右转
	{
	spt[Y]= yr-3.0*High/20;
	tpt[X] = spt[X] - 5*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "ay-%d°%s%s\n", d,mm,ss);
	G_maketext(pWd,tpt,str,0,texth,0,2);
	spt[Y]= yr-7.0*High/20;
	tpt[X] = spt[X] - 2.5*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "R-%4.0lf l-%3.0lf",pzLinep->JDarray[i][3],pzLinep->JDarray[i][4]);
	G_maketext(pWd,tpt,str,0,texth,0,2);
	spt[Y]= yr-11.0*High/20;
	tpt[X] = spt[X] - 1.25*texth;
	tpt[Y] = spt[Y];
	_stprintf(str, "T-%6.2lf L-%6.2lf", pzLinep->QXB[i][5],pzLinep->QXB[i][7]);
	G_maketext(pWd,tpt,str,0,texth,0,2);
	};
	}
	else if ( (x[3][0]-x[2][0]) > 10 && pzLinep->JDarray[i][4] > 80 ) 
	{
	if (pzLinep->QXB[i][8] < 0.0)//左转
	{
	spt[X] =x[1][0]+0.5, spt[Y] =yl+9.0*High/20;
	ept[X] =x[4][0]-0.5; ept[Y] =spt[Y];
	_stprintf(str, "az-%d°%s%s R-%4.0lf",d, mm,ss,pzLinep->JDarray[i][3]);
	G_maketext(pWd,spt,str,0,texth,0,2);

	spt[X] =x[1][0]+1.2, spt[Y] =yl+5.0*High/20;
	ept[X] =x[4][0]-1.2, ept[Y] =spt[Y];
	_stprintf(str, "l-%3.0lf T-%6.2lf", pzLinep->JDarray[i][4],pzLinep->QXB[i][5]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	spt[X] =st+1.8,  spt[Y] =yl+1.0*High/20;
	ept[X] =et-1.8,  ept[Y] =spt[Y];
	_stprintf(str, "L-%6.2lf",pzLinep->QXB[i][7]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	};
	if (pzLinep->QXB[i][8] > 0.0)//右转
	{
	spt[X] =st,  spt[Y] =yr-3.0*High/20;;
	ept[0] =et,  ept[1] =spt[Y];
	_stprintf(str, "ay-%d°%s%s", d, mm,ss);
	G_maketext(pWd,spt,str,0,texth,0,2);
	spt[0] =x[1][0]+1.2, spt[1] =yr-7.0*High/20;
	ept[0] =x[4][0]-1.2; ept[1]=spt[1];
	_stprintf(str, "R-%4.0lf l-%3.0lf",pzLinep->JDarray[i][4],pzLinep->JDarray[i][3]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	spt[0] =x[1][0]+0.5,   spt[1] =yr-11.0*High/20;
	ept[0] =x[4][0]-0.5,   ept[1] =spt[1];
	_stprintf(str, "T-%6.2lf L-%6.2lf", pzLinep->QXB[i][5],pzLinep->QXB[i][7]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	};
	}
	else if((x[3][0]-x[2][0]) > 5)//圆曲线在图上长度>5
	{
	if (pzLinep->QXB[i][8] < 0.0)//左转
	{
	spt[0] =x[1][0]+0.2,  spt[1] =yl+9.0*High/20;
	ept[0] =x[4][0]-0.2,  ept[1] =spt[1];
	_stprintf(str, "az-%d°%s%s R-%4.0lf",d,mm,ss,pzLinep->JDarray[i][4]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	spt[0] =x[1][0]+0.6,  spt[1] =yl+5.0*High/20;
	ept[0] =x[4][0]-0.6,  ept[1] =spt[1];
	_stprintf(str, "l-%3.0lf T-%6.2lf", pzLinep->JDarray[i][3],pzLinep->QXB[i][5]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	spt[0] =st-1.0,   spt[1] =yl+1.0*High/20;
	ept[0] =et+1.0,   ept[1] =spt[1];
	_stprintf(str, "L-%6.2lf",pzLinep->QXB[i][7]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	};
	if (pzLinep->QXB[i][8] > 0.0)//右转
	{
	spt[0] =st-1.0,   spt[1] =yr-3.0*High/20;;
	ept[0] =et+1.0,   ept[1] =spt[1];
	_stprintf(str, "ay%d°%s%s", d, mm,ss);
	G_maketext(pWd,spt,str,0,texth,0,2);
	spt[0] =x[1][0]+0.6,  spt[1] =yr-7.0*High/20;
	ept[0] =x[4][0]-0.6,  ept[1] =spt[1];
	_stprintf(str, "R-%4.0lf l-%3.0lf",pzLinep->JDarray[i][4],pzLinep->JDarray[i][3]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	spt[0] =x[1][0]+0.2,  spt[1] =yr-11.0*High/20;
	ept[0] =x[4][0]-0.2,  ept[1] =spt[1];
	_stprintf(str, "T-%6.2lf L-%6.2lf", pzLinep->QXB[i][5],pzLinep->QXB[i][7]);
	G_maketext(pWd,spt,str,0,texth,0,2);
	}
	}
	}//end of if(pzLinep->JDarray[i][4]>0.5)//有半径


	bml = pzLinep->QXB[i][3];//下一个曲线段起点里程，本曲线段的hz里程
	xb  = x[4][0];//下一个曲线段起点图上x位置

	} / * end for * /   */


}
double DMX_PQX_ROAD::cmltosx (double cml)
{
	double sx;
	sx = x_ori + (cml - pzLinep->DLArray[0].ELC) / m_HScale;	//20190709	修改采用双轴比例尺
	return sx;
}
Adesk::Boolean  DMX_PQX_ROAD::G_make2dline(AcGiWorldDraw* pWd,AcGePoint3dArray ptarray, int icolor)
{     

	pWd->subEntityTraits().setColor(icolor);	

	//	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);	
	pWd->geometry().polyline(ptarray.length(),ptarray.asArrayPtr());
	return  0;
}
