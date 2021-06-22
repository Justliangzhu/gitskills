// DeshBz.cpp: implementation of the DeshBz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeshBz.h"
#include "ORI_SCALE.h"
#include "DMX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DeshBz_ROAD::DeshBz_ROAD()
{
	set_HScale_ori();
	texth = 3.0;

	acdbHostApplicationServices()->workingDatabase()->
		loadLineTypeFile(L"CONTINUOUS",L"acadiso.lin");//
	DeshId = CreateLayer(L"DeshBz",L"CONTINUOUS");
}

DeshBz_ROAD::~DeshBz_ROAD()
{

}

AcDbObjectId DeshBz_ROAD::CreateLayer(ACHAR *LayerName, ACHAR *LineType)
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
Adesk::Boolean  DeshBz_ROAD::G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode=1)//在头文件中不能写成mode=1,应为mode
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

Adesk::Boolean DeshBz_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{   
	AcGePoint3d Pt,tpt;
	JD_CENTER *pz;
	ACHAR str[500];

	Pt.set(x_ori,y_ori,0);
	/*
	CString mes;
	mes.Format(L"%0.3lf %0.3lf",Pt.x,Pt.y);
	ads_alert(mes);*/
	if (m_Frame_Index == -1)
	{
		m_Frame_Index = GlobalZDMTK.FindWichZdm(Pt);
		ads_printf(L"错误：地面线标注寻找图框号\n");	//20190712	图框号改为外部传递，如果有遗漏的命令，这里提示
	}

	pz = GlobalZDMTK.ZDMTK[m_Frame_Index].pm;
	if(pz)
		pzLinep = pz;
	else
		ads_printf(L"DeshBz:无法找到平面方案!\n");

	tpt.x = x_ori - 29;
	tpt.y = pORI_SCALE.Y_ORI-0.5*High;;
	_tcscpy(str,L"设计高程");
	G_maketext(pWd,tpt,str,0,texth+1.5,4,0);

	//int izdm = GlobalZDMTK.FindWichZdm(Pt);
	if (m_Frame_Index >= 0)
	{
		AcGePoint3d *verts=new AcGePoint3d[2];
		verts[0].x=x_ori;
		verts[0].y=pORI_SCALE.Y_ORI-High;
		verts[0].z=0.0;
		verts[1].x=x_ori+pzLinep->LenOfLine/m_HScale;	//20190709	修改采用双轴比例尺
		verts[1].y=verts[0].y;
		verts[1].z=0.0;
		pWd->subEntityTraits().setLayer(DeshId);//先设层
		pWd->subEntityTraits().setColor(6);	
		pWd->geometry().polyline(2,verts);
		if(verts) delete [] verts;   
		pzdm = GlobalZDMTK.ZDMTK[m_Frame_Index].zdm;
		DMX_ROAD *dmx = GlobalZDMTK.ZDMTK[m_Frame_Index].dmx;
		double desh,qcml;
		Pt.y = y_ori;
		if(dmx && pzdm)
		{
			desh = pzdm->rdes_level(dmx->DMXys[0].lc);
			Pt.x = cmltosx(dmx->DMXys[0].lc);
			_stprintf(str,L"%0.2lf",desh);		
			G_maketext(pWd,Pt,str,0.5*PI,texth,0,1);
			qcml = dmx->DMXys[0].lc;			
			for(int i=1; i<dmx->NUM-1;i++)
			{
				//if(dmx->DMXys[i].lc-qcml>=50)
				//{
					desh = pzdm->rdes_level(dmx->DMXys[i].lc);
					Pt.x = cmltosx(dmx->DMXys[i].lc);
					_stprintf(str,L"%0.2lf",desh);
					G_maketext(pWd,Pt,str,0.5*PI,texth,0,1);
					qcml = dmx->DMXys[i].lc;
				//}
			}
			desh = pzdm->rdes_level(dmx->DMXys[dmx->NUM-1].lc);
			Pt.x = cmltosx(dmx->DMXys[dmx->NUM-1].lc);
			_stprintf(str,L"%0.2lf",desh);
			G_maketext(pWd,Pt,str,0.5*PI,texth,0,1);
		}
	}

	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}


Acad::ErrorStatus DeshBz_ROAD::dwgInFields(AcDbDwgFiler* pFiler)
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

	pFiler->readItem( &High);
	pFiler->readItem( &HBVScale);
	pFiler->readItem( &x_ori);
	pFiler->readItem( &y_ori);
	pFiler->readItem(&m_HScale);
	pFiler->readItem(&m_VScale);
	pFiler->readItem(&m_Frame_Index);
	/*
	void *pVoid;
	pFiler->readAddress(&pVoid);
	pzdm = (GTZDM *)pVoid;*/

	//////////////////////////////
	return pFiler->filerStatus();
}

// Files data out to a DWG file.
//
Acad::ErrorStatus DeshBz_ROAD::dwgOutFields(AcDbDwgFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dwgOutFields(pFiler);

	if (pFiler->filerType() == AcDb::kWblockCloneFiler)
		pFiler->writeHardPointerId((AcDbHardPointerId)ownerId());
	/////////////////add///////////////
	pFiler->writeItem( High);
	pFiler->writeItem( HBVScale);
	pFiler->writeItem( x_ori);
	pFiler->writeItem( y_ori);
	pFiler->writeItem(m_HScale);
	pFiler->writeItem(m_VScale);
	pFiler->writeItem(m_Frame_Index);
	
	//    pFiler->writeAddress(pzdm);	
	//	acutPrintf(L"\n%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf\n",
	//		startLC,endLC,H_ori,HScale,VScale,ZMax,ZMin,x_ori,y_ori);

	//	acutPrintf(L"\ndwgin NUM=%d",NUM);
	//	acutPrintf(L"\nwrite.dwgout NUM=%d",NUM);
	///////////////////////////////
	return pFiler->filerStatus();
}

// Files data in from a DXF file.

Acad::ErrorStatus DeshBz_ROAD::dxfInFields(AcDbDxfFiler* pFiler)
{

	assertWriteEnabled();
	Acad::ErrorStatus es;
	struct resbuf inbuf;//结果缓冲区
	if ((es = AcDbObject::dxfInFields(pFiler))
		!= Acad::eOk)
	{        return es;    }

	// Check if we're at the right subclass getData marker.

	if (!pFiler->atSubclassData(L"DESH")) {
		ads_printf(L"\n error 10010 类型有错 \n");
		return Acad::eBadDxfSequence;
	}
	/////////////////add///////////////

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

	return pFiler->filerStatus();
}

// Files data out to a DXF file.
//
Acad::ErrorStatus DeshBz_ROAD::dxfOutFields(AcDbDxfFiler* pFiler) const
{
	assertReadEnabled();
	AcDbObject::dxfOutFields(pFiler);
	pFiler->writeItem(AcDb::kDxfSubclass, "DeshBz");///////////"classname  Desh"//add///////////
	pFiler->writeItem( AcDb::kDxfReal,High);
	pFiler->writeItem( AcDb::kDxfReal,HBVScale);
	pFiler->writeItem( AcDb::kDxfReal+1,x_ori);
	pFiler->writeItem( AcDb::kDxfReal+2,y_ori);
	//cutPrintf("\ndxfout NUM=%d",NUM);

	return pFiler->filerStatus();
}


void DeshBz_ROAD::set_HScale_ori()
{	
	High = pORI_SCALE.DESHIGH;
	x_ori = pORI_SCALE.X_ORI;
	y_ori = pORI_SCALE.Y_ORI - High * 0.5;
	HBVScale = pORI_SCALE.HBVSCALE;
	m_HScale = pORI_SCALE.HSCALE;	//20190709	修改采用双轴比例尺
	m_VScale = pORI_SCALE.VSCALE;
	texth = pORI_SCALE.PQXTEXTH;
}

double DeshBz_ROAD::cmltosx (double cml)
{
	double sx;
	sx = x_ori + (cml - pzLinep->DLArray[0].ELC) / m_HScale;	//20190709	修改采用双轴比例尺
	return sx;
}
Adesk::Boolean  DeshBz_ROAD::G_make2dline(AcGiWorldDraw* pWd,AcGePoint3dArray ptarray, int icolor)
{     

	pWd->subEntityTraits().setColor(icolor);	

	//	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);	
	pWd->geometry().polyline(ptarray.length(),ptarray.asArrayPtr());
	return  0;
}
