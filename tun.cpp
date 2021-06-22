// Tun.cpp: implementation of the Tun class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "stdarx.h"
#include "resource.h"
#include "Tun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
TUN_ROAD *GlobalTun;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TUN_ROAD::TUN_ROAD(int nTun,double array[][2],GTZDM_ROAD *zm,int IfAutoName,int zdmno)//array[0]--lc,array[1]--level
{
	NTun = nTun;
	zmLine = zm;
	Zdmno = zdmno;

	if(NTun<1)
		return;
	for(int i=0; i<NTun; i++)
	{
		double sml,eml,cml;
		sml = Tun[i].sml = array[i][0];
		eml = Tun[i].eml = array[i][1];
		cml = Tun[i].cml = 0.5*(Tun[i].sml+Tun[i].eml);
		Tun[i].sdml = pzLinep->XLC1(sml);
		Tun[i].cdml = pzLinep->XLC1(cml);
		Tun[i].edml = pzLinep->XLC1(eml);
		if(zm->Property=="")
			Tun[i].XZ = "全幅";
		else
			Tun[i].XZ = zm->Property;
		Tun[i].ZZDK = 0;
		Tun[i].YZDK = 0;
		CString  GH;
		double xlc = pzLinep->XLC(Tun[i].sml,GH);
		_tcscpy(Tun[i].SKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Tun[i].eml,GH);
		_tcscpy(Tun[i].EKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Tun[i].cml,GH);
		_tcscpy(Tun[i].CKml,pzLinep->LCchr(GH,xlc,3));
	}
	//create layer
	IfChangeTunName = IfAutoName;//自动标隧道名
	TunLayerId = CreateLayer(L"Tun",L"CONTINUOUS");	
}

TUN_ROAD::TUN_ROAD(int nTun,TUNTAB tun[],GTZDM_ROAD *zm,int IfAutoName,int zdmno)//array[0]--lc,array[1]--level
{
	NTun = nTun;
	zmLine = zm;
	Zdmno = zdmno;

	if(NTun<1)
		return;
	for(int i=0; i<NTun; i++)
	{
		double sml,eml,cml;
		sml = Tun[i].sml = pzLinep->TYLC(tun[i].StDml);
		eml = Tun[i].eml = pzLinep->TYLC(tun[i].EdDml);
		cml = Tun[i].cml = 0.5*(sml + eml);
		Tun[i].sdml = tun[i].StDml;
		Tun[i].edml = tun[i].EdDml;
		Tun[i].XZ = tun[i].XZ;
		Tun[i].ZZDK = tun[i].ZZDK;
		Tun[i].YZDK = tun[i].YZDK;
		CString  GH;
		double xlc = pzLinep->XLC(Tun[i].cml,GH);
		_tcscpy(Tun[i].CKml,pzLinep->LCchr(GH,xlc,3));
		_tcscpy(Tun[i].SKml,tun[i].SKml);
		_tcscpy(Tun[i].EKml,tun[i].EKml);
		Tun[i].TunName =  tun[i].TunName;		
	}
	//create layer
	IfChangeTunName = IfAutoName;
	TunLayerId = CreateLayer(L"Tun",L"CONTINUOUS");	
}

TUN_ROAD::TUN_ROAD()
{

}

TUN_ROAD::~TUN_ROAD()
{
}
Adesk::Boolean  TUN_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{   

	JD_CENTER *pz;
	pz = GlobalZDMTK.ZDMTK[Zdmno].pm;
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"Tun:无法找到平面方案!\n");
	zmLine = GlobalZDMTK.ZDMTK[Zdmno].zdm;

	if(NTun < 1) return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);

	if (pWd!= NULL) 
	{
		//	CheckTun();
		for (int i = 0; i < NTun; i++) 
		{

			double desh = zmLine->rdes_level(Tun[i].cml);
			AcGePoint2d realpt(Tun[i].cml,desh);
			Tun[i].cpt[0] = zmLine->Data_realtodraw(realpt).x;
			Tun[i].cpt[1] = zmLine->Data_realtodraw(realpt).y+8;

			desh = zmLine->rdes_level(Tun[i].sml);
			realpt.set(Tun[i].sml,desh);
			Tun[i].spt[0] = zmLine->Data_realtodraw(realpt).x;
			Tun[i].spt[1] = zmLine->Data_realtodraw(realpt).y;

			desh = zmLine->rdes_level(Tun[i].eml);
			realpt.set(Tun[i].eml,desh);
			Tun[i].ept[0] = zmLine->Data_realtodraw(realpt).x;
			Tun[i].ept[1] = zmLine->Data_realtodraw(realpt).y;

			DrawHLine(pWd,Tun[i].sml,Tun[i].eml,Tun[i].spt,Tun[i].ept,0);
			DrawHLine(pWd,Tun[i].sml,Tun[i].eml,Tun[i].spt,Tun[i].ept,2);
			BZTun(pWd,i,pORI_SCALE.PDTextH);
			Tun[i].sdml = zmLine->XLC1(Tun[i].sml);
			Tun[i].cdml = zmLine->XLC1(Tun[i].cml);
			Tun[i].edml = zmLine->XLC1(Tun[i].eml);
		}

	} 
	else
	{
		assert(Adesk::kFalse);//?	
	} 
	GlobalTun = this;//全局隧道，给坡度折减用
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
} 
void TUN_ROAD::DrawVLine(AcGiWorldDraw* pWd,double pt[2],double len)
{
	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(pt[0], pt[1], 0);
	pArr[1].set(pt[0], pt[1]+len, 0);

	pWd->subEntityTraits().setColor(5);	
	pWd->subEntityTraits().setLayer(TunLayerId);//先设层
	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt000);
	pWd->geometry().polyline(2,pArr);    
}

void TUN_ROAD::DrawHLine(AcGiWorldDraw* pWd,double sml,double eml,double Fpt[2],double Tpt[2],double hd)
{

	AcGePoint3d* pArr=new AcGePoint3d[100];
	int Num=0;
	pArr[0].set(Fpt[0], Fpt[1]+8-hd, 0);
	Num++;
	zmLine->ScreenToReal();//取出桥梁范围的变坡点
	for(int i=0; i<zmLine->NBPD; i++)
	{
		if(zmLine->PDarray[i][0] > sml && zmLine->PDarray[i][0]<eml)
		{
			pArr[Num].set(zmLine->BPD_array[i][0], zmLine->BPD_array[i][1]+8-hd, 0);
			Num++;
		}
		else if(zmLine->PDarray[i][0]>eml)
			break;
	}
	pArr[Num].set(Tpt[0], Tpt[1]+8-hd, 0);
	Num++;
	pWd->subEntityTraits().setColor(1);	
	pWd->subEntityTraits().setLayer(TunLayerId);//先设层
	pWd->geometry().polyline(Num,pArr);    
	if(pArr) delete[]pArr;
}

Acad::ErrorStatus TUN_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
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
	pFiler->readItem( &Zdmno);
	pFiler->readItem( &NTun);
	pFiler->readItem( &IfChangeTunName);

	for (int i=0;i<(int)(NTun);i++)
	{  
		pFiler->readItem( &Tun[i].sml);
		pFiler->readItem( &Tun[i].eml);
		pFiler->readItem( &Tun[i].cml);
		//		acutPrintf(L"\n%0.3lf,%0.3lf",Tunys[i].lc,Tunys[i].level);
	}
	//	acedAlert(L"dwgin2");
	//////////////////////////////
	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus TUN_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());
	/////////////////add///////////////
	pFiler->writeItem( Zdmno);
	pFiler->writeItem( NTun);
	pFiler->writeItem( IfChangeTunName);

	//	acutPrintf(L"\nwrite.dwgout NUM=%d",NUM);
	for (int i=0;i<NTun;i++)
	{
		pFiler->writeItem( Tun[i].sml);
		pFiler->writeItem( Tun[i].eml);
		pFiler->writeItem( Tun[i].cml);
	}
	///////////////////////////////
	return pFiler->filerStatus();
}

// Files data in from a DXF file.
//
Acad::ErrorStatus TUN_ROAD::dxfInFields(AcDbDxfFiler* pFiler)//open
{
	assertWriteEnabled();
	Acad::ErrorStatus es;
	struct resbuf inbuf;//结果缓冲区
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }

	// Check if we're at the right subclass getData marker.

	if (!pFiler->atSubclassData(L"TUN")) {
		ads_printf(L"\n error 10010 类型有错 \n");
		return Acad::eBadDxfSequence;
	}
	/////////////////add///////////////
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32 )
		{	Zdmno= inbuf.resval.rint;	}
		else		{		}
	}

	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32 )
		{	NTun= inbuf.resval.rint;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32 )
		{	IfChangeTunName= inbuf.resval.rint;	}
		else		{		}
	}

	//	acutPrintf(L"\ndxfin NUM=%d",NUM);
	for (int i=0;i<NTun;i++)
	{  
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal)
			{
				Tun[i].sml= inbuf.resval.rreal;
			}
			else		{		}
		}
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1)
			{
				Tun[i].eml= inbuf.resval.rreal;
			}
			else		{		}
		}
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+2)
			{
				Tun[i].cml= inbuf.resval.rreal;
			}
			else		{		}
		}

	} 
	return pFiler->filerStatus();
}

// Files data out to a DXF file.
Acad::ErrorStatus TUN_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass, "TUN");///////////"classname  Tun"//add///////////
	pFiler->writeItem(AcDb::kDxfInt32,Zdmno);
	pFiler->writeItem(AcDb::kDxfInt32,NTun);
	pFiler->writeItem(AcDb::kDxfInt32,IfChangeTunName);

	for (int i=0;i<NTun;i++)
	{
		pFiler->writeItem(AcDb::kDxfReal,Tun[i].sml);
		pFiler->writeItem(AcDb::kDxfReal+1,Tun[i].eml);
		pFiler->writeItem(AcDb::kDxfReal+2,Tun[i].cml);
	}
	return pFiler->filerStatus();
}

//地面线实际坐标转化为图坐标
Acad::ErrorStatus TUN_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset)
{
	double dlc,xlc;
	CString  GH;
	int j,k;
	AcGePoint2d realpt;

	GTZDM_ROAD *pz;
	pz = GlobalZDMTK.ZDMTK[Zdmno].zdm;
	if(pz)
		zmLine = pz;
	else
		ads_printf(L"zm:无法找到纵面方案!\n");

	assertWriteEnabled();
	for ( j=0;j<indices.length();j++)
	{   // ads_printf(L"\n indices.length()=%d,j=%d  indices[j]=%d",indices.length(),j,indices[j]);
		// 变坡点序号 0~NBPD-1

		if (indices[j]<NTun)//修改进洞
		{
			k=indices[j];
			dlc= offset[X]*zmLine->HBVSCALE;
			Tun[k].sml += dlc;
			Tun[k].cml = 0.5*(Tun[k].sml+Tun[k].eml);

			Tun[k].sdml = pzLinep->XLC(Tun[k].sml,GH);
			_tcscpy(Tun[k].SKml,pzLinep->LCchr(GH,Tun[k].sdml,3));
			Tun[k].cdml = pzLinep->XLC(Tun[k].cml,GH);
			_tcscpy(Tun[k].CKml,pzLinep->LCchr(GH,Tun[k].cdml,3));
		}
		else  if(indices[j]<2*NTun)//修改出洞
		{
			k=indices[j] - NTun;
			dlc= offset[X]*zmLine->HBVSCALE;
			Tun[k].eml += dlc;
			Tun[k].cml = 0.5*(Tun[k].sml+Tun[k].eml);

			Tun[k].edml = pzLinep->XLC(Tun[k].eml,GH);
			_tcscpy(Tun[k].EKml,pzLinep->LCchr(GH,Tun[k].edml,3));
			Tun[k].cdml = pzLinep->XLC(Tun[k].cml,GH);
			_tcscpy(Tun[k].CKml,pzLinep->LCchr(GH,Tun[k].cdml,3));
		}
		else//移动隧道
		{
			k=indices[j] - 2*NTun;
			dlc= offset[X]*zmLine->HBVSCALE;

			Tun[k].sml += dlc;
			Tun[k].eml += dlc;
			Tun[k].cml = 0.5*(Tun[k].sml+Tun[k].eml);

			Tun[k].sdml = pzLinep->XLC(Tun[k].sml,GH);
			_tcscpy(Tun[k].SKml,pzLinep->LCchr(GH,Tun[k].sdml,3));
			Tun[k].edml = pzLinep->XLC(Tun[k].eml,GH);
			_tcscpy(Tun[k].EKml,pzLinep->LCchr(GH,Tun[k].edml,3));
			Tun[k].cdml = pzLinep->XLC(Tun[k].cml,GH);
			_tcscpy(Tun[k].CKml,pzLinep->LCchr(GH,Tun[k].cdml,3));
			//	ads_printf(L"%s\n",Tun[k].CKml);

		}
	}
	return Acad::eOk;
}


Acad::ErrorStatus TUN_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											 AcDbIntArray& osnapModes,
											 AcDbIntArray& geomIds)const
{   
	AcGePoint3d Pt;
	int i;
	assertReadEnabled();
	// 将地面点加入夹点集
	// ads_printf(L"\n NJD=%d",NJD);
	for (i = 0; i < NTun; i++) 
	{  Pt.set(Tun[i].spt[0],Tun[i].spt[1]+8,0.0);
	gripPoints.append(Pt);
	}
	for (i = 0; i < NTun; i++) 
	{  Pt.set(Tun[i].ept[0],Tun[i].ept[1]+8,0.0);
	gripPoints.append(Pt);
	}
	for (i = 0; i < NTun; i++) 
	{  Pt.set(Tun[i].cpt[0],Tun[i].cpt[1],0.0);
	gripPoints.append(Pt);
	}

	return Acad::eOk;
}
//	标注变坡点
Adesk::Boolean  TUN_ROAD::BZTun(AcGiWorldDraw* pWd,int iTun,double texth)
{   
	AcGePoint3d Pt1,Pt2; 
	ACHAR chrh[80];
	double B,WB;

	//标进洞里程
	pWd->subEntityTraits().setLayer(TunLayerId);
	textStyle();
	WB=8*texth;
	Pt2.z=0.0;  Pt2.z=0.0;

	Pt1.x=Tun[iTun].spt[0];
	Pt1.y=Tun[iTun].spt[1]+6;
	Pt2.x=Tun[iTun].spt[0];
	Pt2.y=Tun[iTun].spt[1]+6+WB;
	DrawVLine(pWd,Tun[iTun].spt,WB);	//标注线
	B=pi*0.5;

	double len = Tun[iTun].eml-Tun[iTun].sml;
	ACHAR ck[50]=L"";
	double ml=0;
	split_ckml(Tun[iTun].SKml,&ml,ck);
	double sml = ml - (int)(ml/1000+0.001)*1000;
	_stprintf(chrh,L"+%0.1lf",sml);
	Pt2.x=Pt1.x-texth*0.5;
	Pt2.y=Pt1.y+2*texth;
	G_maketext(pWd,Pt2,chrh,B ,texth ,0,2);     

	double dmh,desh,twh;
	DMX_ROAD *dmx;
	dmx = GlobalZDMTK.ZDMTK[Zdmno].dmx;
	if(dmx)
	{
		dmh = dmx->CalDMGC(Tun[iTun].sml);
		if(dmh>-99)//有效高程
		{
			desh = zmLine->rdes_level(Tun[iTun].sml);

			twh = desh - dmh;
			_stprintf(chrh,L"%0.2lfm",twh);

			Pt2.x=Pt1.x+texth*1.5;
			Pt2.y=Pt1.y+2*texth;
			G_maketext(pWd,Pt2,chrh,B,texth,0,2);     
		}
	}
	else
		ads_printf(L"bri:无法找到地面线,标填挖高!\n");


	//标出洞里程
	Pt2.z=0.0;  Pt2.z=0.0;
	Pt1.x=Tun[iTun].ept[0];
	Pt1.y=Tun[iTun].ept[1]+6;
	Pt2.x=Tun[iTun].ept[0];
	Pt2.y=Tun[iTun].ept[1]+6+WB;
	DrawVLine(pWd,Tun[iTun].ept,WB);	//标注线
	B=pi*0.5;

	split_ckml(Tun[iTun].EKml,&ml,ck);
	double eml = ml - (int)(ml/1000+0.001)*1000;
	_stprintf(chrh,L"+%0.1lf",eml);
	Pt2.x=Pt1.x-texth*0.5;
	Pt2.y=Pt1.y+2*texth;
	G_maketext(pWd,Pt2,chrh,B ,texth ,0,2);     

	if(dmx)
	{
		dmh = dmx->CalDMGC(Tun[iTun].eml);
		if(dmh>-99)//有效高程
		{
			desh = zmLine->rdes_level(Tun[iTun].eml);

			twh = desh - dmh;
			_stprintf(chrh,L"%0.2lfm",twh);

			Pt2.x=Pt1.x+texth*1.5;
			Pt2.y=Pt1.y+2*texth;
			G_maketext(pWd,Pt2,chrh,B,texth,0,2);     
		}
	}

	//中心标注隧道名称
	WB=14*texth;
	Pt2.z=0.0;  Pt2.z=0.0;  	
	Pt1.x=Tun[iTun].cpt[0];
	Pt1.y=Tun[iTun].cpt[1]+6;
	Pt2.x=Tun[iTun].cpt[0];
	Pt2.y=Tun[iTun].cpt[1]+6+WB;
	DrawVLine(pWd,Tun[iTun].cpt,WB);	//标注线
	B=pi*0.5;

	Pt2.x=Pt1.x-texth*0.5;
	Pt2.y=Pt1.y+texth;
	if(IfChangeTunName)
		Tun[iTun].TunName.Format(L"隧道%d",iTun+1);
	_stprintf(chrh,L"%s %0.2lfm",Tun[iTun].TunName,len);
	G_maketext(pWd,Pt2,chrh,B,texth,0,2);     

	return 1;
}
//  删除一个交点数据
Adesk::Boolean  TUN_ROAD::DelTun(AcGePoint2d PT)
{   
	int i; 
	AcGePoint2d pt;
	pt = zmLine->Data_drawtoreal(PT);
	double cml = pt.x;
	int iTun=0,k=-1;
	while(iTun<NTun)
	{
		if(cml>Tun[iTun].sml-0.01 && cml<Tun[iTun].eml+0.01)
		{
			k = iTun;
			break;
		}
		iTun++;
	}
	if (k>=0)
	{  	 
		assertWriteEnabled();
		for (i=k;i<NTun-1;i++) // 前移一个 
		{
			Tun[i]= Tun[i+1];
			/*
			Tun[i].sml = Tun[i+1].sml;
			Tun[i].eml = Tun[i+1].eml;
			Tun[i].cml = Tun[i+1].cml;
			_tcscpy(Tun[i].SKml,Tun[i+1].SKml);
			_tcscpy(Tun[i].EKml,Tun[i+1].EKml);
			_tcscpy(Tun[i].CKml,Tun[i+1].CKml);*/

		}
		NTun--;	  
		return 1;  
	} else return 0;
}

Adesk::Boolean  TUN_ROAD::AddTun(AcGePoint2d SPT,AcGePoint2d EPT)
{   
	int i; 
	AcGePoint2d pt;
	CString  GH;
	pt = zmLine->Data_drawtoreal(SPT);
	double sml = pt.x;
	pt = zmLine->Data_drawtoreal(EPT);
	double eml = pt.x,xlc;
	int iTun=0,k=-1;

	while(iTun<NTun)
	{
		if(sml<Tun[iTun].sml-0.01)
		{
			k = iTun;
			break;
		}

		iTun++;
	}
	//	ads_printf(L"k=%d\n",k);
	if (k>=0)
	{  	 
		assertWriteEnabled();
		for (i=NTun;i>k;i--) // 前移一个 
		{
			/*
			Tun[i].sml = Tun[i-1].sml;
			Tun[i].eml = Tun[i-1].eml;
			Tun[i].cml = Tun[i-1].cml;
			_tcscpy(Tun[i].SKml,Tun[i-1].SKml);
			_tcscpy(Tun[i].EKml,Tun[i-1].EKml);
			_tcscpy(Tun[i].CKml,Tun[i-1].CKml);*/
			Tun[i]=Tun[i-1];
		}
		Tun[k].sml = sml;
		Tun[k].eml = eml;
		Tun[k].cml = 0.5*(sml+eml);
		Tun[k].sdml = pzLinep->XLC1(Tun[k].sml);
		Tun[k].edml = pzLinep->XLC1(Tun[k].eml);
		Tun[k].cdml = pzLinep->XLC1(Tun[k].cml);
		xlc = pzLinep->XLC(Tun[k].sml,GH);
		_tcscpy(Tun[k].SKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Tun[k].eml,GH);
		_tcscpy(Tun[k].EKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Tun[k].cml,GH);
		_tcscpy(Tun[k].CKml,pzLinep->LCchr(GH,xlc,3));

		NTun++;	       	 
	} 
	else//终点加桥
	{
		assertWriteEnabled();
		k = NTun;
		Tun[k].sml = sml;
		Tun[k].eml = eml;
		Tun[k].cml = 0.5*(sml+eml);
		Tun[k].sdml = pzLinep->XLC1(Tun[k].sml);
		Tun[k].edml = pzLinep->XLC1(Tun[k].eml);
		Tun[k].cdml = pzLinep->XLC1(Tun[k].cml);
		xlc = pzLinep->XLC(Tun[k].sml,GH);
		_tcscpy(Tun[k].SKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Tun[k].eml,GH);
		_tcscpy(Tun[k].EKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Tun[k].cml,GH);
		_tcscpy(Tun[k].CKml,pzLinep->LCchr(GH,xlc,3));
		NTun++;	       	 
	}
	return 1;
}
void TUN_ROAD::CheckTun()//检查隧道范围有无交插，合并处理
{
	double xlc;
	CString  GH;

	for(int i=0; i<NTun-1; i++)
	{
		if(Tun[i].eml>Tun[i+1].sml)//前一做桥的终点里程深入后一做桥,合并
		{
			Tun[i].sml = Tun[i].sml < Tun[i+1].sml ? Tun[i].sml : Tun[i+1].sml;
			Tun[i].eml = Tun[i].eml > Tun[i+1].eml ? Tun[i].eml : Tun[i+1].eml;
			Tun[i].cml = 0.5*(Tun[i].sml+Tun[i].eml);
			xlc = pzLinep->XLC(Tun[i].sml,GH);
			_tcscpy(Tun[i].SKml,pzLinep->LCchr(GH,xlc,3));
			xlc = pzLinep->XLC(Tun[i].eml,GH);
			_tcscpy(Tun[i].EKml,pzLinep->LCchr(GH,xlc,3));
			xlc = pzLinep->XLC(Tun[i].cml,GH);
			_tcscpy(Tun[i].CKml,pzLinep->LCchr(GH,xlc,3));
			for (int j=i+1;j<NTun-1;j++) // 前移一个 
			{
				Tun[j].sml = Tun[j+1].sml;
				Tun[j].eml = Tun[j+1].eml;
				Tun[j].cml = Tun[j+1].cml;
				_tcscpy(Tun[j].SKml,Tun[j+1].SKml);
				_tcscpy(Tun[j].EKml,Tun[j+1].EKml);
				_tcscpy(Tun[j].CKml,Tun[j+1].CKml);			   			   
			}
			NTun--;	  
		}
	}
}