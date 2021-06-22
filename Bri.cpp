// BRI.cpp: implementation of the BRI class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "stdarx.h"
#include "resource.h"
#include "bri.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//extern triDTM *ptriDTM;
BRI_ROAD::BRI_ROAD(int nbri,double array[][2],GTZDM_ROAD *zm,int IfAutoName,int zdmno)//array[0]--lc,array[1]--level
{
	NBRI = nbri;
	zmLine = zm;
	Zdmno = zdmno;
	if(NBRI<1)
		return;
	for(int i=0; i<NBRI; i++)
	{
		double sml,eml,cml;
		sml = Bri[i].sml = array[i][0];
		eml = Bri[i].eml = array[i][1];
		cml = Bri[i].cml = 0.5*(Bri[i].sml+Bri[i].eml);
		Bri[i].sdml = pzLinep->XLC1(sml);
		Bri[i].cdml = pzLinep->XLC1(cml);
		Bri[i].edml = pzLinep->XLC1(eml);
		Bri[i].HeadAng=90;
		Bri[i].TailAng=90;
		Bri[i].ZZDK = 1;
		Bri[i].YZDK = 1;
		if(zm->Property=="")
			Bri[i].XZ = "全幅";
		else
			Bri[i].XZ = zm->Property;
		CString  GH;
		double xlc = pzLinep->XLC(Bri[i].sml,GH);
		_tcscpy(Bri[i].SKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Bri[i].eml,GH);
		_tcscpy(Bri[i].EKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Bri[i].cml,GH);
		_tcscpy(Bri[i].CKml,pzLinep->LCchr(GH,xlc,3));
		Bri[i].SWHt = "";		
	}
	//create layer
	IfChangeBriName = IfAutoName;//自动标桥名
	BriLayerId = CreateLayer(L"BRI",L"CONTINUOUS");	
}

BRI_ROAD::BRI_ROAD(int nbri,BRITAB bri[],GTZDM_ROAD *zm,int IfAutoName,int zdmno)//array[0]--lc,array[1]--level
{
	NBRI = nbri;
	zmLine = zm;
	Zdmno = zdmno;

	if(NBRI<1)
		return;
	for(int i=0; i<NBRI; i++)
	{
		double sml,eml,cml;
		sml = Bri[i].sml = pzLinep->TYLC(bri[i].StDml);
		eml = Bri[i].eml = pzLinep->TYLC(bri[i].EdDml);
		cml = Bri[i].cml = pzLinep->TYLC(bri[i].CenDml);
		Bri[i].sdml = bri[i].StDml;
		Bri[i].cdml = bri[i].CenDml;
		Bri[i].edml = bri[i].EdDml;
		Bri[i].XZ = bri[i].XZ;
		Bri[i].HeadAng = bri[i].HeadAng;
		Bri[i].TailAng = bri[i].TailAng;
		Bri[i].ZZDK = bri[i].ZZDK;
		Bri[i].YZDK = bri[i].YZDK;
		_tcscpy(Bri[i].SKml,bri[i].SKml);
		_tcscpy(Bri[i].EKml,bri[i].EKml);
		_tcscpy(Bri[i].CKml,bri[i].CKml);

		Bri[i].SWHt = bri[i].SWHt;		
		Bri[i].KKSY = bri[i].KKSY;		
		Bri[i].BriName =  bri[i].BriName;		
	}
	//create layer
	IfChangeBriName = IfAutoName;//自动标桥名
	BriLayerId = CreateLayer(L"BRI",L"CONTINUOUS");	
}

BRI_ROAD::BRI_ROAD()
{

}

BRI_ROAD::~BRI_ROAD()
{
}
Adesk::Boolean  BRI_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{   

	ads_printf(L"Zdmno=%d\n",Zdmno);
	if(NBRI < 1) return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);

	JD_CENTER *pz;
	pz = GlobalZDMTK.ZDMTK[Zdmno].pm;
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"Tun:无法找到平面方案!\n");
	zmLine = GlobalZDMTK.ZDMTK[Zdmno].zdm;

	AcGePoint3d PT;
	if (pWd!= NULL && zmLine) 
	{
		//		CheckBri();
		for (int i = 0; i < NBRI; i++) 
		{
			double desh = zmLine->rdes_level(Bri[i].cml);
			AcGePoint2d realpt(Bri[i].cml,desh);
			Bri[i].cpt[0] = zmLine->Data_realtodraw(realpt).x;
			Bri[i].cpt[1] = zmLine->Data_realtodraw(realpt).y;

			desh = zmLine->rdes_level(Bri[i].sml);
			realpt.set(Bri[i].sml,desh);
			Bri[i].spt[0] = zmLine->Data_realtodraw(realpt).x;
			Bri[i].spt[1] = zmLine->Data_realtodraw(realpt).y;

			desh = zmLine->rdes_level(Bri[i].eml);
			realpt.set(Bri[i].eml,desh);
			Bri[i].ept[0] = zmLine->Data_realtodraw(realpt).x;
			Bri[i].ept[1] = zmLine->Data_realtodraw(realpt).y;
			//ads_printf(L"i=%d %lf %lf %lf %lf\n",i,Bri[i].spt[0],Bri[i].spt[1],Bri[i].ept[0],Bri[i].ept[1]);
			DrawVLine(pWd,Bri[i].spt,-10);
			DrawVLine(pWd,Bri[i].ept,-10);
			DrawHLine(pWd,Bri[i].sml,Bri[i].eml,Bri[i].spt,Bri[i].ept,2);
			BZBRI(pWd,i,pORI_SCALE.PDTextH);
			if(Bri[i].SWHt!="")//标水位
				DRAWSW(pWd,i);


			CString GH;
			Bri[i].sdml = pzLinep->XLC(Bri[i].sml,GH);
			Bri[i].cdml = pzLinep->XLC(Bri[i].cml,GH);
			Bri[i].edml = pzLinep->XLC(Bri[i].eml,GH);
		}

		PT.set(Bri[0].cpt[0],Bri[0].cpt[1],0.0);
		JD_CENTER *pz;
		pz = GlobalZDMTK.ZDMTK[Zdmno].pm;
		if(pz)
			pzLinep = pz;
		else
			ads_printf(L"bri:无法找到平面方案!\n");
	} 
	//else
	//{
	//	assert(Adesk::kFalse);//?	
	//} 

	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
} 
void BRI_ROAD::DrawVLine(AcGiWorldDraw* pWd,double pt[2],double len)
{
	AcGePoint3d* pArr=new AcGePoint3d[2];
	pArr[0].set(pt[0], pt[1], 0);
	pArr[1].set(pt[0], pt[1]+len, 0);

	pWd->subEntityTraits().setColor(1);	
	pWd->subEntityTraits().setLayer(BriLayerId);//先设层
	pWd->geometry().polyline(2,pArr);    
}
void BRI_ROAD::DrawHLine(AcGiWorldDraw* pWd,double sml,double eml,double Fpt[2],double Tpt[2],double hd)
{

	AcGePoint3d* pArr=new AcGePoint3d[100];
	int Num=0;
	pArr[0].set(Fpt[0], Fpt[1]-hd, 0);
	Num++;
	zmLine->ScreenToReal();//取出桥梁范围的变坡点
	for(int i=0; i<zmLine->NBPD; i++)
	{
		if(zmLine->PDarray[i][0] > sml && zmLine->PDarray[i][0]<eml)
		{
			pArr[Num].set(zmLine->BPD_array[i][0], zmLine->BPD_array[i][1]-hd, 0);
			Num++;
		}
		else if(zmLine->PDarray[i][0]>eml)
			break;
	}
	pArr[Num].set(Tpt[0], Tpt[1]-hd, 0);
	Num++;
	pWd->subEntityTraits().setColor(1);	
	pWd->subEntityTraits().setLayer(BriLayerId);//先设层
	pWd->geometry().polyline(Num,pArr);    
	if(pArr) delete[]pArr;
}

Acad::ErrorStatus BRI_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
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
	pFiler->readItem( &NBRI);
	pFiler->readItem( &IfChangeBriName);

	for (int i=0;i<NBRI;i++)
	{  
		pFiler->readItem( &Bri[i].sml);
		pFiler->readItem( &Bri[i].eml);
		pFiler->readItem( &Bri[i].cml);

		//		acutPrintf(L"\n%0.3lf,%0.3lf",BRIys[i].lc,BRIys[i].level);
	}
	//	acedAlert(L"dwgin2");
	//////////////////////////////
	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus BRI_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());
	/////////////////add///////////////
	pFiler->writeItem( Zdmno);
	pFiler->writeItem( NBRI);
	pFiler->writeItem( IfChangeBriName);


	//	acutPrintf(L"\nwrite.dwgout NUM=%d",NUM);
	for (int i=0;i<NBRI;i++)
	{
		pFiler->writeItem( Bri[i].sml);
		pFiler->writeItem( Bri[i].eml);
		pFiler->writeItem( Bri[i].cml);
	}
	///////////////////////////////
	return pFiler->filerStatus();
}


// Files data in from a DXF file.
//
Acad::ErrorStatus BRI_ROAD::dxfInFields(AcDbDxfFiler* pFiler)//open
{
	//    acedAlert(L"dxfin1");
	assertWriteEnabled();
	Acad::ErrorStatus es;
	struct resbuf inbuf;//结果缓冲区
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }

	// Check if we're at the right subclass getData marker.

	if (!pFiler->atSubclassData(L"BRI")) {
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
		{	NBRI= inbuf.resval.rint;	}
		else		{		}
	}
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32 )
		{	IfChangeBriName= inbuf.resval.rint;	}
		else		{		}
	}

	//	acutPrintf(L"\ndxfin NUM=%d",NUM);
	for (int i=0;i<NBRI;i++)
	{  
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal)
			{
				Bri[i].sml= inbuf.resval.rreal;
			}
			else		{		}
		}
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+1)
			{
				Bri[i].eml= inbuf.resval.rreal;
			}
			else		{		}
		}
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if ( inbuf.restype == AcDb:: kDxfReal+2)
			{
				Bri[i].cml= inbuf.resval.rreal;
			}
			else		{		}
		}

	} 
	return pFiler->filerStatus();
}

// Files data out to a DXF file.
Acad::ErrorStatus BRI_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass, "BRI");///////////"classname  BRI"//add///////////
	pFiler->writeItem(AcDb::kDxfInt32,Zdmno);
	pFiler->writeItem(AcDb::kDxfInt32,NBRI);
	pFiler->writeItem(AcDb::kDxfInt32,IfChangeBriName);

	for (int i=0;i<NBRI;i++)
	{
		pFiler->writeItem(AcDb::kDxfReal,Bri[i].sml);
		pFiler->writeItem(AcDb::kDxfReal+1,Bri[i].eml);
		pFiler->writeItem(AcDb::kDxfReal+2,Bri[i].cml);
	}
	return pFiler->filerStatus();
}
//地面线实际坐标转化为图坐标
Acad::ErrorStatus BRI_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset)
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

		if (indices[j]<NBRI)//修改桥头
		{
			k=indices[j];
			dlc= offset[X]*zmLine->HBVSCALE;
			Bri[k].sml += dlc;
			Bri[k].cml = 0.5*(Bri[k].sml+Bri[k].eml);

			Bri[k].sdml = pzLinep->XLC(Bri[k].sml,GH);
			_tcscpy(Bri[k].SKml,pzLinep->LCchr(GH,Bri[k].sdml,3));
			Bri[k].cdml = pzLinep->XLC(Bri[k].cml,GH);
			_tcscpy(Bri[k].CKml,pzLinep->LCchr(GH,Bri[k].cdml,3));
		}
		else  if(indices[j]<2*NBRI)//修改桥尾巴
		{
			k=indices[j] - NBRI;
			dlc= offset[X]*zmLine->HBVSCALE;
			Bri[k].eml += dlc;
			Bri[k].cml = 0.5*(Bri[k].sml+Bri[k].eml);

			Bri[k].edml = pzLinep->XLC(Bri[k].eml,GH);
			_tcscpy(Bri[k].EKml,pzLinep->LCchr(GH, Bri[k].edml,3));
			Bri[k].cdml = pzLinep->XLC(Bri[k].cml,GH);
			_tcscpy(Bri[k].CKml,pzLinep->LCchr(GH,Bri[k].cdml,3));
		}
		else//移动桥梁
		{
			k=indices[j] - 2*NBRI;
			dlc= offset[X]*zmLine->HBVSCALE;

			Bri[k].cml += dlc;
			Bri[k].sml += dlc;
			Bri[k].eml += dlc;

			Bri[k].sdml = pzLinep->XLC(Bri[k].sml,GH);
			_tcscpy(Bri[k].SKml,pzLinep->LCchr(GH,Bri[k].sdml,3));
			Bri[k].edml = pzLinep->XLC(Bri[k].eml,GH);
			_tcscpy(Bri[k].EKml,pzLinep->LCchr(GH,Bri[k].edml,3));
			Bri[k].cdml = pzLinep->XLC(Bri[k].cml,GH);
			_tcscpy(Bri[k].CKml,pzLinep->LCchr(GH,Bri[k].cdml,3));
		}
	}

	return Acad::eOk;
}


Acad::ErrorStatus BRI_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											 AcDbIntArray& osnapModes,
											 AcDbIntArray& geomIds)const
{   AcGePoint3d Pt;
int i;
assertReadEnabled();
// 将地面点加入夹点集
// ads_printf(L"\n NJD=%d",NJD);
for (i = 0; i < NBRI; i++) 
{  Pt.set(Bri[i].spt[0],Bri[i].spt[1]-2,0.0);
gripPoints.append(Pt);
}
for (i = 0; i < NBRI; i++) 
{  Pt.set(Bri[i].ept[0],Bri[i].ept[1]-2,0.0);
gripPoints.append(Pt);
}
for (i = 0; i < NBRI; i++) 
{  Pt.set(Bri[i].cpt[0],Bri[i].cpt[1]-2,0.0);
gripPoints.append(Pt);
}

return Acad::eOk;
}
//	标注变坡点
Adesk::Boolean  BRI_ROAD::BZBRI(AcGiWorldDraw* pWd,int iBri,double texth)
{   
	AcGePoint3d Pt1,Pt2; 
	ACHAR chrh[80],chrlc[180];
	double B,WB;

	pWd->subEntityTraits().setLayer(BriLayerId);
	textStyle();

	WB=14*texth;
	Pt2.z=0.0;  Pt2.z=0.0;

	Pt1.x=Bri[iBri].cpt[0];
	Pt1.y=Bri[iBri].cpt[1];
	Pt2.x=Bri[iBri].cpt[0];
	Pt2.y=Bri[iBri].cpt[1]+WB;
	DrawVLine(pWd,Bri[iBri].cpt,WB);	//标注线
	B=pi*0.5;

	double len = Bri[iBri].eml-Bri[iBri].sml;
	if(IfChangeBriName>0)//自动标桥名
		Bri[iBri].KKSY.Format(L"桥长%0.2lf米",len);
	_tcscpy(chrh,Bri[iBri].KKSY);
	Pt2.x=Pt1.x-texth*0.5;
	Pt2.y=Pt1.y+3*texth;
	G_maketext(pWd,Pt2,chrh,B ,texth ,1,2);     
	Pt2.x=Pt1.x+texth*1.5;
	Pt2.y=Pt1.y+3*texth;

	if(IfChangeBriName>0)
	{
		if(len>499.0)
			Bri[iBri].BriName = "特大桥";
		else if(len>99.0)
			Bri[iBri].BriName = "大桥";
		else if(len>49.0)
			Bri[iBri].BriName = "中桥";
		else
			Bri[iBri].BriName = "小桥";
	}

	/*

	else if(len>99.0)
	_stprintf(chrlc, L"大桥%s",Bri[iBri].CKml);
	else if(len>49.0)
	_stprintf(chrlc, L"中桥%s",Bri[iBri].CKml);
	else
	_stprintf(chrlc, L"小桥%s",Bri[iBri].CKml);
	*/

	_stprintf(chrlc,L"%s %s",Bri[iBri].BriName,Bri[iBri].CKml);
	G_maketext(pWd,Pt2,chrlc,B ,texth ,1,2);     
	//    textStyle();

	WB=-14*texth;
	Pt2.z=0.0;  Pt2.z=0.0;
	//标桥头里程
	Pt1.x=Bri[iBri].spt[0];
	Pt1.y=Bri[iBri].spt[1]+6;
	Pt2.x=Bri[iBri].spt[0];
	Pt2.y=Bri[iBri].spt[1]+6+WB;
	DrawVLine(pWd,Bri[iBri].spt,WB);	//标注线
	B=pi*0.5;

	len = Bri[iBri].eml-Bri[iBri].sml;
	ACHAR ck[50]=L"";
	double ml=0;
	split_ckml(Bri[iBri].SKml,&ml,ck);
	double sml = ml - (int)(ml/1000+0.001)*1000;
	//_stprintf(chrh, L"+%0.1lf",sml);
	_tcscpy(chrh,Bri[iBri].SKml);
	Pt2.x=Pt1.x+texth*1.5;
	Pt2.y=Pt1.y-13*texth;
	G_maketext(pWd,Pt2,chrh,B ,texth ,0,2);     
	//标桥头填高
	double dmh,desh,twh;
	DMX_ROAD *dmx;
	dmx = GlobalZDMTK.ZDMTK[Zdmno].dmx;
	if(dmx)
	{
		dmh = dmx->CalDMGC(Bri[iBri].sml);
		if(dmh>-99)//有效高程
		{
			desh = zmLine->rdes_level(Bri[iBri].sml);

			twh = desh - dmh;
			_stprintf(chrh, L"%0.2lfm",twh);

			Pt2.x=Pt1.x-texth*0.5;
			Pt2.y=Pt1.y-10*texth;
			G_maketext(pWd,Pt2,chrh,B,texth,0,2);     
		}
	}
	else
		ads_printf(L"bri:无法找到地面线,标填挖高!\n");



	//标桥尾里程
	Pt2.z=0.0;  Pt2.z=0.0;
	Pt1.x=Bri[iBri].ept[0];
	Pt1.y=Bri[iBri].ept[1]+6;
	Pt2.x=Bri[iBri].ept[0];
	Pt2.y=Bri[iBri].ept[1]+6+WB;
	DrawVLine(pWd,Bri[iBri].ept,WB);	//标注线
	B=pi*0.5;

	split_ckml(Bri[iBri].EKml,&ml,ck);
	// double eml = ml - (int)(ml/1000+0.001)*1000;
	// _stprintf(chrh, L"+%0.1lf",eml);
	_tcscpy(chrh,Bri[iBri].EKml);
	Pt2.x=Pt1.x-texth*0.5;
	Pt2.y=Pt1.y-13*texth;
	G_maketext(pWd,Pt2,chrh,B ,texth ,0,2);    

	if(dmx)
	{
		dmh = dmx->CalDMGC(Bri[iBri].eml);
		if(dmh>-99)//有效高程
		{
			desh = zmLine->rdes_level(Bri[iBri].eml);

			twh = desh - dmh;
			_stprintf(chrh, L"%0.2lfm",twh);

			Pt2.x=Pt1.x+texth*1.5;
			Pt2.y=Pt1.y-10*texth;
			G_maketext(pWd,Pt2,chrh,B,texth,0,2);     
		}
	}
	else
		ads_printf(L"bri:无法找到地面线,标填挖高!\n");

	return 1;
}

//  删除一个桥梁数据
Adesk::Boolean  BRI_ROAD::DelBri(AcGePoint2d PT)
{   
	int i; 
	AcGePoint2d pt;
	pt = zmLine->Data_drawtoreal(PT);
	double cml = pt.x;
	int iBri=0,k=-1;
	while(iBri<NBRI)
	{
		if(cml>Bri[iBri].sml-0.01 && cml<Bri[iBri].eml+0.01)
		{
			k = iBri;
			break;
		}
		iBri++;
	}
	if (k>=0)
	{  	 
		assertWriteEnabled();
		for (i=k;i<NBRI-1;i++) // 前移一个 
		{
			Bri[i]=Bri[i+1];
			/*
			Bri[i].sml = Bri[i+1].sml;
			Bri[i].eml = Bri[i+1].eml;
			Bri[i].cml = Bri[i+1].cml;
			_tcscpy(Bri[i].SKml,Bri[i+1].SKml);
			_tcscpy(Bri[i].EKml,Bri[i+1].EKml);
			_tcscpy(Bri[i].CKml,Bri[i+1].CKml);
			Bri[i].SWHt = Bri[i+1].SWHt;*/

		}
		NBRI--;	  
		return 1;  
	} else return 0;
}

Adesk::Boolean  BRI_ROAD::AddBri(AcGePoint2d SPT,AcGePoint2d EPT)
{   
	int i; 
	AcGePoint2d pt;
	CString  GH;
	pt = zmLine->Data_drawtoreal(SPT);
	double sml = pt.x;
	pt = zmLine->Data_drawtoreal(EPT);
	double eml = pt.x,xlc;
	int iBri=0,k=-1;

	while(iBri<NBRI)
	{
		if(sml<Bri[iBri].sml-0.01)
		{
			k = iBri;
			break;
		}

		iBri++;
	}
	//	ads_printf(L"k=%d\n",k);
	if (k>=0)
	{  	 
		assertWriteEnabled();
		for (i=NBRI;i>k;i--) // 前移一个 
		{

			Bri[i]=Bri[i-1];
			/*
			Bri[i].sml = Bri[i-1].sml;
			Bri[i].eml = Bri[i-1].eml;
			Bri[i].cml = Bri[i-1].cml;
			_tcscpy(Bri[i].SKml,Bri[i-1].SKml);
			_tcscpy(Bri[i].EKml,Bri[i-1].EKml);
			_tcscpy(Bri[i].CKml,Bri[i-1].CKml);
			Bri[i].SWHt = Bri[i-1].SWHt;*/

		}
		Bri[k].sml = sml;
		Bri[k].eml = eml;
		Bri[k].cml = 0.5*(sml+eml);
		Bri[k].sdml = pzLinep->XLC1(Bri[k].sml);
		Bri[k].edml = pzLinep->XLC1(Bri[k].eml);
		Bri[k].cdml = pzLinep->XLC1(Bri[k].cml);
		xlc = pzLinep->XLC(Bri[k].sml,GH);
		_tcscpy(Bri[k].SKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Bri[k].eml,GH);
		_tcscpy(Bri[k].EKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Bri[k].cml,GH);
		_tcscpy(Bri[k].CKml,pzLinep->LCchr(GH,xlc,3));
		Bri[k].SWHt = "";
		double len=eml-sml;
		Bri[iBri].KKSY.Format(L"桥长%0.2lf米",len);

		if(len>499.0)
			Bri[k].BriName = "特大桥";
		else if(len>99.0)
			Bri[k].BriName = "大桥";
		else if(len>49.0)
			Bri[k].BriName = "中桥";
		else
			Bri[k].BriName = "小桥";
		NBRI++;	       	 
	} 
	else//终点加桥
	{
		assertWriteEnabled();
		k = NBRI;
		Bri[k].sml = sml;
		Bri[k].eml = eml;
		Bri[k].cml = 0.5*(sml+eml);
		Bri[k].sdml = pzLinep->XLC1(Bri[k].sml);
		Bri[k].edml = pzLinep->XLC1(Bri[k].eml);
		Bri[k].cdml = pzLinep->XLC1(Bri[k].cml);
		xlc = pzLinep->XLC(Bri[k].sml,GH);
		_tcscpy(Bri[k].SKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Bri[k].eml,GH);
		_tcscpy(Bri[k].EKml,pzLinep->LCchr(GH,xlc,3));
		xlc = pzLinep->XLC(Bri[k].cml,GH);
		_tcscpy(Bri[k].CKml,pzLinep->LCchr(GH,xlc,3));
		Bri[k].SWHt = "";
		NBRI++;	       	 
	}
	return 1;
}
void BRI_ROAD::DRAWSW(AcGiWorldDraw* pWd,int i)
{
	int pos=Bri[i].SWHt.Find(L".");
	if(pos<=0)
		return;
	CString SWHt=Bri[i].SWHt.Left(pos+3);
	double desh=_wtof(SWHt);

	AcGePoint2d realpt(Bri[i].cml,desh);
	AcGePoint3d cpt,ept,spt;
	cpt.z=ept.z=spt.z=0.0;
	cpt.x = zmLine->Data_realtodraw(realpt).x;
	cpt.y = zmLine->Data_realtodraw(realpt).y;
	ept.x = cpt.x+27, ept.y=cpt.y;
	G_makeline(pWd,cpt,ept,6);

	spt.x=cpt.x, spt.y=cpt.y;
	ept.x=spt.x-1,ept.y=spt.y+2;
	cpt.x=spt.x+1,cpt.y=ept.y;
	//画小三角形
	G_makeline(pWd,spt,ept,6); 
	G_makeline(pWd,spt,cpt,6);
	G_makeline(pWd,ept,cpt,6);

	ACHAR mes[80];
	_stprintf(mes,L"%s",SWHt);
	spt.x=spt.x+2;
	G_maketext(pWd,spt,mes,0,2.5,1,2);     	 
}
void BRI_ROAD::CheckBri()//检查桥梁范围有无交插，合并处理
{
	double xlc;
	CString  GH;

	for(int i=0; i<NBRI-1; i++)
	{
		if(Bri[i].eml>Bri[i+1].sml)//前一做桥的终点里程深入后一做桥,合并
		{
			Bri[i].sml = Bri[i].sml < Bri[i+1].sml ? Bri[i].sml : Bri[i+1].sml;
			Bri[i].eml = Bri[i].eml > Bri[i+1].eml ? Bri[i].eml : Bri[i+1].eml;
			Bri[i].cml = 0.5*(Bri[i].sml+Bri[i].eml);
			xlc = pzLinep->XLC(Bri[i].sml,GH);
			_tcscpy(Bri[i].SKml,pzLinep->LCchr(GH,xlc,3));
			xlc = pzLinep->XLC(Bri[i].eml,GH);
			_tcscpy(Bri[i].EKml,pzLinep->LCchr(GH,xlc,3));
			xlc = pzLinep->XLC(Bri[i].cml,GH);
			_tcscpy(Bri[i].CKml,pzLinep->LCchr(GH,xlc,3));
			for (int j=i+1;j<NBRI-1;j++) // 前移一个 
			{
				Bri[j].sml = Bri[j+1].sml;
				Bri[j].eml = Bri[j+1].eml;
				Bri[j].cml = Bri[j+1].cml;
				_tcscpy(Bri[j].SKml,Bri[j+1].SKml);
				_tcscpy(Bri[j].EKml,Bri[j+1].EKml);
				_tcscpy(Bri[j].CKml,Bri[j+1].CKml);
				Bri[j].SWHt = Bri[j+1].SWHt;
			}
			NBRI--;	  
		}
	}
}