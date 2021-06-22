// BAS_DRAW_FUN1.cpp: implementation of the BAS_DRAW_FUN class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "aced.h"
#include "dbapserv.h"
#include "BAS_DRAW_FUN_RAIL.h"
#include "migrtion.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016



#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS" 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
struct QuXianZheJiang
{
	float ZhJiangVal[8][8];
};
//GuiFanCanShu mGuiFanCS;
QuXianZheJiang m_ZJ;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BAS_DRAW_FUN_RAIL::BAS_DRAW_FUN_RAIL()
{

}

BAS_DRAW_FUN_RAIL::~BAS_DRAW_FUN_RAIL()
{

}

// 向AUTOCAD 数据库中添加实体对象的通用函数
AcDbObjectId BAS_DRAW_FUN_RAIL::AddEntityToDbs(AcDbEntity *pEntity)
{ 	

	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//	Acad::ErrorStatus     es = Acad::eOk;
	AcDbObjectId objId ;
	AcDbBlockTable *pBlockTable;
	// CAD R2000
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable,AcDb::kForRead);
	// CAD R14
	// acdbCurDwg()->getBlockTable(pBlockTable,AcDb::kForRead);
	AcDbBlockTableRecord  *pSpaceRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord,AcDb::kForWrite);
	pSpaceRecord->appendAcDbEntity(objId, pEntity);
	pSpaceRecord->close();
	pBlockTable->close();
	pEntity->close();
	acDocManager->unlockDocument(acDocManager->curDocument());
	return objId;
	//plh modify
	/*
	AcDbBlockTable *pBlockTable;
	//?	acdbCurDwg()->getBlockTable(pBlockTable,AcDb::kForWrite);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,AcDb::kForWrite);
	pBlockTable->close();

	AcDbObjectId arcId;
	pBlockTableRecord->appendAcDbEntity(arcId,pArc);
	pBlockTableRecord->close();
	pArc->close();
	return arcId;*/

}

// 由两点绘直线
AcDbObjectId   BAS_DRAW_FUN_RAIL::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt)
{   
	AcDbLine *pLine	= new AcDbLine(startPt,endPt);
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	return LId ;
}

AcDbObjectId   BAS_DRAW_FUN_RAIL::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor)
{
	AcDbLine *pLine = new AcDbLine(startPt, endPt) ;
	pLine->setColorIndex(icolor) ;
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	return LId;     
}
// 绘文本
AcDbObjectId   BAS_DRAW_FUN_RAIL::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType)
{
	AcGePoint2dArray PtAy;
	PtAy.append(AcGePoint2d(startPt.x,startPt.y));
	PtAy.append(AcGePoint2d(endPt.x,endPt.y));
	return makepolyline(PtAy,icolor,lw,LayerName,LineType);	    
}

AcDbObjectId BAS_DRAW_FUN_RAIL::makepolyline(AcGePoint2dArray PtAy,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType,double LTScale)
{
	int NUM=PtAy.length();
	AcDbPolyline *polyline=new AcDbPolyline(NUM);
	for(int i =0;i<NUM;i++)
	{
		polyline->addVertexAt(i,PtAy[i]);
		polyline->setWidthsAt(i,lw,lw);
	}
	if(icolor>=0)polyline->setColorIndex(icolor);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	if(LayerName==NULL)LayerName=L"0";
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord=new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);

		//acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	polyline->setLayer(LayerTabRecordId);	
	pLayerTable->close();
	if(LineType!=NULL)
	{
		if(Acad::eUndefinedLineType==acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType,L"acadiso.lin"))
		{
			ACHAR lineFname[256];
			_stprintf(lineFname,L"%s\\lib\\rail.lin",SysPath);
			acdbHostApplicationServices()->workingDatabase()->
				loadLineTypeFile(LineType,lineFname);//
		}

		polyline->setLinetype(LineType);		
	}
	if(LTScale>0.0)
		polyline->setLinetypeScale(LTScale);
	AcDbObjectId  LId;
	LId = AddEntityToDbs(polyline);
	return LId;     
}




AcDbObjectId BAS_DRAW_FUN_RAIL::maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR *layername,ACHAR* textStyle)
{
	AcDbObjectId  TId;
	AcDbText *pText = new AcDbText;
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite);
	if(pTextStyleTbl->getAt(textStyle,pTextStyleTblId)!= Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
		pTextStyleTblRcd->setName(textStyle);
		pTextStyleTblRcd->setFileName(textStyle);
		pTextStyleTblRcd->setFont(textStyle,0,0,134,2);
		pTextStyleTblRcd->setXScale(0.8);
		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		pTextStyleTbl->getAt(textStyle,pTextStyleTblId);
	}	
	pTextStyleTbl->close();	
	if (mode==1) // 左对齐
	{      pText->setHorizontalMode(AcDb::kTextLeft); }
	else if (mode==2) // // 中对齐
	{      pText->setHorizontalMode(AcDb::kTextCenter); }
	else // // 右对齐
	{      pText->setHorizontalMode(AcDb::kTextRight); };
	pText->setColorIndex(icolor);
	if (mode==1) {  pText->setPosition(pos);	}
	else {   pText->setAlignmentPoint(pos);     };
	//  ads_printf(L"\n   m        Pt=%10.3f   %10.3f   %10.4f %s",pos[X],pos[Y],dms_rad(ang),ctext);  
	pText->setTextStyle(pTextStyleTblId);
	pText->setRotation(ang);
	pText->setHeight(texth);
	pText->setTextString(ctext);
	if(layername)
	{
		AcDbObjectId LayerId;
		LayerId = CreateLayer(layername);
		pText->setLayer(LayerId);
	}
	TId=AddEntityToDbs(pText);
	return TId;     
}
// 绘圆弧
AcDbObjectId   BAS_DRAW_FUN_RAIL::makearc(AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx)
{
	AcDbArc *pArc = new AcDbArc;
	AcDbObjectId ArcId;

	//	acdbOpenObject(pArc,ArcId,AcDb::kForWrite,Adesk::kFalse);	
	AcGePoint3d centerPt;
	double f1,sf,ef,dd,AA;
	pArc->setRadius(R);
	dd=xyddaa(startPt[1],startPt[0],endPt[1],endPt[0],&f1);
	AA=acos(dd*0.5/R);
	centerPt[Y]=startPt[Y]+R*cos(f1+npx*AA);
	centerPt[X]=startPt[X]+R*sin(f1+npx*AA);
	centerPt[Z]=startPt[Z];
	//f2=f1+pi; if (f1>pi) f2=f1-pi;
	// ads_printf(L"\n 直线长 distane=%16.7f 方位角 (d.ms) f=%12.7f (%12.7f) ",dd,dms_rad(f1),dms_rad(f2));
	ads_printf(L"\n R%7.2f 偏向=%d ",R,npx);
	pArc->setCenter(centerPt);
	// 大地方位角
	dd=xyddaa(centerPt[1],centerPt [0],startPt[1],startPt[0],&f1);
	// 笛卡角
	sf=pi/2.0-f1; if (sf<0) sf=sf+pi*2; 
	//	 ads_printf(L"\n 直线长 distane=%16.7f 方位 (d.ms) sf=%12.7f  ",dd,dms_rad(sf));

	dd=xyddaa(centerPt[1],centerPt[0], endPt[1],endPt[0],&f1);
	// 笛卡角
	ef=pi/2.0-f1; if (ef<0) ef=ef+pi*2; 
	// ads_printf(L"  直线长 distane=%16.7f 方位 (d.ms) ef=%12.7f ",dd,dms_rad(ef));

	if (npx==1) // 右偏
	{   pArc->setStartAngle(ef);
	pArc->setEndAngle(sf);
	}else  // 左偏
	{   pArc->setStartAngle(sf);
	pArc->setEndAngle(ef);
	};

	pArc->setRadius(dd);
	pArc->setColorIndex(2);
	//	pArc->close();
	ArcId=AddEntityToDbs(pArc);         
	return ArcId ;
}

// getPoint - acquire WCS point from user:
int BAS_DRAW_FUN_RAIL::getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT)
{
	int rc;
	ads_point pFrom = {pF.x, pF.y, pF.z},  pTo;

	if (	(rc = ads_getpoint(pFrom, pMessage, pTo)) == RTNORM)
		// (rc = sa_w2u(pFrom, pFrom)) == RTNORM &&
			// CAD R2000
				// (rc = acedGetPoint(pFrom, pMessage, pTo)) == RTNORM) 
					// CAD R14
	{
		// rc = sa_u2w(pTo, pTo);
		pT.x = pTo[X];
		pT.y = pTo[Y];
		pT.z = pTo[Z];
	}
	return rc;
}

// getPoint - acquire WCS point from user:
int BAS_DRAW_FUN_RAIL::getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT,AcGePoint3d& pT1,AcGePoint3d& pT2)
{
	ads_point pFrom = {pF.x, pF.y, pF.z},
		ppT= { pT.x,  pT.y, pT.z},
		ppT1= { pT1.x,  pT1.y,  pT1.z},
		ppT2= { pT2.x,  pT2.y,  pT2.z} ;

	int track=1,type;
	struct resbuf entdata;// 鼠标拖动时的动态坐标
	ads_point  tmppt;
	while (track>0) {  //鼠标拖动循环

		ads_grread (track, &type, &entdata); 
		tmppt[0]=entdata.resval.rpoint[0];
		tmppt[1]=entdata.resval.rpoint[1];
		tmppt[2]=0.0;
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"REDRAW", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"REDRAW", RTNONE);
#endif // VERSION_CAD2010
		
		ads_grdraw (ppT1 , tmppt, 3, 1);
		ads_grdraw (ppT2 , tmppt, 1, 1);
		ads_grdraw (ppT , tmppt, 2, 1);

		if (type==3)  //  单击鼠标左键输入一点
		{      
			pT.x = tmppt[X];
			pT.y = tmppt[Y];
			pT.z = tmppt[Z];
			track=0;
		};
	}
	return 1;
}


int  BAS_DRAW_FUN_RAIL::getPoint(ACHAR* pMessage, AcGePoint3d& pT)
{
	int rc;
	ads_point pTo;
	// CAD R2000
	//  if ((rc = acedGetPoint(NULL, pMessage, pTo)) == RTNORM) {
	// CAD R14 
	if ((rc =ads_getpoint(NULL, pMessage, pTo)) == RTNORM) {

		// rc = sa_u2w(pTo, pTo);
		pT[X] = pTo[X];
		pT[Y] = pTo[Y];
		pT[Z] = pTo[Z];
	}
	return rc;
}


void  BAS_DRAW_FUN_RAIL::makeGroup(AcDbObjectId groupId)
{
	ads_name sset;
	int err = ads_ssget(NULL, NULL, NULL, NULL, sset);
	if (err != RTNORM) {
		return;
	}

	AcDbGroup *pGroup;
	AcDbObject *pObj;

	/*acdbOpenObject(pObj, groupId, AcDb::kForWrite);*/
	if(acdbOpenObject(pObj,groupId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}

	pGroup = AcDbGroup::cast(pObj);
	// Traverse the selection set exchanging each ads_name
	// for an objectId, then adding the object to the group.
	//
	long i, length;
	ads_name ename;
	AcDbObjectId entId;
	ads_sslength(sset, &length);
	for (i = 0; i < length; i++) {
		ads_ssname(sset, i, ename);
		acdbGetObjectId(entId, ename);
		pGroup->append(entId);
	}
	pGroup->close();
	ads_ssfree(sset);
}

void BAS_DRAW_FUN_RAIL::removeAllButLines(AcDbObjectId groupId)
{
	AcDbGroup *pGroup;
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, groupId, AcDb::kForWrite);*/
	if(acdbOpenObject(pObj,groupId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pGroup = AcDbGroup::cast(pObj);
	AcDbGroupIterator *pIter = pGroup->newIterator();

	for (; !pIter->done(); pIter->next()) 
	{
		pIter->getObject(pObj, AcDb::kForRead);

		// If it's not a line or descended from a line,
		// close it and remove it from the group.  Otherwise
		// just close it.
		//
		if (!pObj->isKindOf(AcDbLine::desc()))
		{
			// AcDbGroup::remove() requires that the object
			// to be removed be closed, so close it now.
			pObj->close();
			pGroup->remove(pIter->objectId());
		} else
		{
			pObj->close();
		}
	}
	if (pIter != NULL)
	{
		delete pIter;
		pIter = NULL;
	}

	// Now change the color of all the entities in the group
	// to red (AutoCAD color index number 1).
	//
	pGroup->setColorIndex(1);

	pGroup->close();
}

void BAS_DRAW_FUN_RAIL::groups()
{
	AcDbGroup *pGroup = new AcDbGroup(L"grouptest");

	AcDbDictionary *pGroupDict;
	// CAD R2000
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict,AcDb::kForWrite);
	// CAD R14
	//    acdbCurDwg()->getGroupDictionary(pGroupDict,AcDb::kForWrite);


	AcDbObjectId groupId;
	pGroupDict->setAt(L"ASDK_GROUPTEST", pGroup, groupId);
	pGroupDict->close();
	pGroup->close();

	makeGroup(groupId);
	removeAllButLines(groupId);
}


AcDbEntity*  BAS_DRAW_FUN_RAIL::selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
{
	ads_name en;
	ads_point pt;
	int rc = ads_entsel(L"\nSelect an entity: ", en, pt);
	if (rc != RTNORM)
	{
		ads_printf(L"Nothing selected.\n", rc);
		return NULL;
	}

	Acad::ErrorStatus es = acdbGetObjectId(eId, en);
	if (es != Acad::eOk) 
	{
		ads_printf(L"Either acdbGetObjectId or ads_entsel failed: "
			L"Entity name <%lx,%lx>, error %d.\n", en[0], en[1], es);
		return NULL;
	}
	AcDbEntity* entObj;

	es = acdbOpenObject(entObj, eId, openMode);
	if (es != Acad::eOk) 
	{
		ads_printf(L"acdbOpenObject failed with error %d.\n", es);
		return NULL;
	}
	ads_printf(L"\nAcDbEntity name=%s",entObj->isA()->name());
	return entObj;
}

double  BAS_DRAW_FUN_RAIL::Get_INDATA_L(double INDATA[5],int *jj)
{     
	int j;
	double l = 0.0;
	j = (int)(INDATA[0] + 0.01);
	if (j == 1) 
		l = INDATA[1];
	if (j == 2) 
		l = INDATA[2];
	if (j == 3)
	{
		if (fabs(INDATA[2]) > 0.00001)
			l = INDATA[1] * INDATA[1] / INDATA[2];
		else
			l = 0.0;
	}

	if (j == 4)
	{
		if (fabs(INDATA[2]) > 0.00001)
			l = INDATA[1] * INDATA[1] / INDATA[2];
		else
			l = 0.0;
	}


	//   R1----> R2  (BIG---->SMALL)    of spiral
	//   A=INDATA[1];R1=INDATA[2];R2=INDATA[3];L1=A*A/R1;L2=A*A/R2;L=L2-L1;
	if (j == 5)
	{
		if (fabs(INDATA[2] * INDATA[3]) > 0.00001)
			l = INDATA[1] * INDATA[1] * (INDATA[2] - INDATA[3]) / (INDATA[2] * INDATA[3]);
		else
			l = 0.0;
	}

	//   R2----> R1  (SMALL---->big)             of spiral
	//    A=INDATA[1];R2=INDATA[2];R1=INDATA[3];L=A*A*(R2-R1)/R1/R2;L1=A*A/R1;L2=L1+L;ll=L1+l;
	if (j == 6)
	{
		if (fabs(INDATA[2] * INDATA[3]) > 0.00001)
			l = INDATA[1] * INDATA[1] * (INDATA[3] - INDATA[2]) / (INDATA[2] * INDATA[3]);
		else
			l = 0.0;
	}

	*jj = j;
	return l;
}

/* INDATA[0]=线元类型 1/2/3/4/5/6/7
INDATA[1]=直L/圆R/回A
INDATA[2]=直0/圆L/回R
INDATA[3]=直0/圆0/回R1 2
INDATA[4]=偏向(左1 右-1)"
INDATA[0]-> TYPE 类型 INDATA[1]->(直线L/园R/缓A) [2]->R1(R2)
[3]->R2(R1) [4]->ZY 偏向 (左Z=-1/右Y=1)
*/

int BAS_DRAW_FUN_RAIL::xlzbxy(struct xlpoint *P0,struct xlpoint *PZ,double l,double INDATA[5],int display)
{   double az,a0,aj,R,L,A,R1,R2,zy,L1,L2,ll,xm,ym,t1,t2,x,y;
int type,re=1;
double ar1,ar2,ar,xr,yr,xr1,yr1,xr2,yr2;
a0=P0->a;
type=(int)(INDATA[0]+0.0001);
zy=INDATA[4];
PZ->lc=P0->lc+l;
switch (type)
{  case 1:    //   line
PZ->x=P0->x+l*cos(a0);
PZ->y=P0->y+l*sin(a0);
PZ->a=P0->a;PZ->r=1e40;
break;
case 2:   //    part of circle
	R=INDATA[1];aj=l/R;az=a0+zy*aj;t1=R*tan(aj*.5);t2=t1;
	xm=P0->x+t1*cos(a0);ym=P0->y+t1*sin(a0);
	PZ->x=xm+t2*cos(az);PZ->y=ym+t2*sin(az);PZ->a=az;PZ->r=R;
	break;
case 3:   //    0=0--Ri-->R  l<=L&& Ri>R     of  spiral
	A=INDATA[1];R=INDATA[2];L=A*A/R;aj=l*l*.5/R/L;
	x=spiral_x(A,l);y=spiral_y(A,l);
	t1=x-y/tan(aj);t2=y/sin(aj);az=a0+aj*zy;
	xm=P0->x+t1*cos(a0);ym=P0->y+t1*sin(a0);
	PZ->x=xm+t2*cos(az);PZ->y=ym+t2*sin(az);PZ->a=az;
	if (l>0) { PZ->r=A*A/l;}
	else {  PZ->r=1e40;};
	break;
case 4:   //   R----> 0-0            of spiral
	A=INDATA[1];R=INDATA[2];L=A*A/R;ll=L-l;
	xr=spiral_x(A,L);yr=spiral_y(A,L);
	aj=ll*ll*.5/R/L;
	x=spiral_x(A,ll);y=spiral_y(A,ll);
	az=P0->a+zy*L*.5/R;PZ->a=az-zy*aj;
	if (ll>0) { PZ->r=A*A/ll;}
	else {  PZ->r=1e40;};
	PZ->x=P0->x+(x-xr)*cos(az+pi)+(y-yr)*cos(az+pi-zy*.5*pi);
	PZ->y=P0->y+(x-xr)*sin(az+pi)+(y-yr)*sin(az+pi-zy*.5*pi);
	break;
case 5:   //   R1----> R2  (BIG---->SMALL)    of spiral
	A=INDATA[1];R1=INDATA[2];R2=INDATA[3];L1=A*A/R1;L2=A*A/R2;
	L=L2-L1;ll=L1+l;
	if (ll<=L2+0.01) {
		ar1=L1*.5/R1;ar2=L2*.5/R2;ar=ll*ll*.5/R2/L2;aj=ar-ar1;
		xr1=spiral_x(A,L1);yr1=spiral_y(A,L1);
		x=spiral_x(A,ll);y=spiral_y(A,ll);
		a0=P0->a-zy*ar1;PZ->a=a0+zy*ar;
		if (ll>0) { PZ->r=A*A/ll;}
		else {  PZ->r=1e40;};
		PZ->x=P0->x+(x-xr1)*cos(a0)+(y-yr1)*cos(a0+zy*.5*pi);
		PZ->y=P0->y+(x-xr1)*sin(a0)+(y-yr1)*sin(a0+zy*.5*pi);
		re=1;   }
	else { re=0;};
	break;
case 6:   //   R2----> R1  (SMALL---->big)             of spiral
	A=INDATA[1];R2=INDATA[2];R1=INDATA[3];L=A*A*(R1-R2)/R1/R2;
	L1=A*A/R1;L2=L1+L;ll=L2-l;
	if (ll<=L2) {
		ar1=L1*.5/R1;ar2=L2*.5/R2;ar=ll*ll*.5/R2/L2;aj=ar-ar1;
		xr2=spiral_x(A,L2);yr2=spiral_y(A,L2);
		x=spiral_x(A,ll);y=spiral_y(A,ll);
		az=P0->a+zy*ar2;PZ->a=az-zy*ar;
		if (ll>0) { PZ->r=A*A/ll;}
		else {  PZ->r=1e40;};
		PZ->x=P0->x+(x-xr2)*cos(az+pi)+(y-yr2)*cos(az+pi-zy*.5*pi);
		PZ->y=P0->y+(x-xr2)*sin(az+pi)+(y-yr2)*sin(az+pi-zy*.5*pi);
		re=1;      }
	else { re=0; };
	break;
};
if (PZ->a<0.0) PZ->a=PZ->a+2*pi;
if (PZ->a>2*pi) PZ->a=PZ->a-2*pi;
return(re);
}

int  BAS_DRAW_FUN_RAIL::xlpoint_pz(double array2[][10],int trow2,BAS_DRAW_FUN:: xlpoint *PZ1) 
	// 求某一统一里程的坐标,并返回线元类型
{ 
	double l,dlc,lc;
	int i,j,k,bz=-99;
	double INDATA2[5],jslc;
	struct xlpoint PB0,PEZ,*PBI,*PEI;
	xlpoint *PZ;
	PZ=new xlpoint;
	PZ->a=PZ1->a;
	PZ->lc=PZ1->lc;
	PZ->r=PZ1->r;
	PZ->x=PZ1->x;
	PZ->y=PZ1->y;
	PB0.lc=array2[0][4];
	PB0.x=array2[0][1];
	PB0.y=array2[0][2];
	PB0.a=rad(array2[0][3]);
	PB0.r=1e40;
	dlc=array2[0][0];
	jslc=PZ->lc;
	PBI=&PB0;PEI=&PEZ; 
	i=0; bz=99;
	do
	{ 
		i=i+1; 
		if (i>=trow2) bz=-99;
		if (bz>0)
		{
			// 第 i 线元
			for (k=0;k<5;k++) INDATA2[k]=array2[i][k];
			l=Get_INDATA_L(INDATA2,&j);
			if(i>=trow2-1)
				l += 0.1;//最后一个线元加一个毫米
			PBI->a  = array2[i][5];
			PBI->lc = array2[i][6];
			PBI->r  = array2[i][7];
			PBI->x  = array2[i][8];
			PBI->y  = array2[i][9];
			// l 为该线元的长度 m

			if( ( jslc <= PBI->lc+l ) && (jslc>=PBI->lc) ) 
				// 计算点在 i 线元上
			{  lc=jslc-PBI->lc;
			if (lc<=0.0) lc=0.000000000001;
			xlzbxy(PBI,PZ,lc,INDATA2,0);
			k=999;
			bz=-99;
			};
		}  // bz>0
	}while (bz>0);

	PZ1->a=PZ->a;
	PZ1->lc=PZ->lc;
	PZ1->r=PZ->r;
	PZ1->x=PZ->x;
	PZ1->y=PZ->y;
	if (k==999) return j; //有解 j 为 线元类型
	else
	{  
		//ads_printf(L"xlpoint_pz求坐标无解\n");
		return (-9); // 无解
	};
}

int  BAS_DRAW_FUN_RAIL::xlpoint_pz(double array2[][10],int trow2,struct xlpoint *PZ) 
	// 求某一统一里程的坐标,并返回线元类型
{ 
	double l,dlc,lc;
	int i,j,k,bz=-99;
	double INDATA2[5],jslc;
	struct xlpoint PB0,PEZ,*PBI,*PEI;
	PB0.lc=array2[0][4];
	PB0.x=array2[0][1];
	PB0.y=array2[0][2];
	PB0.a=rad(array2[0][3]);
	PB0.r=1e40;
	dlc=array2[0][0];
	jslc=PZ->lc;
	PBI=&PB0;PEI=&PEZ; 
	i=0; bz=99;
	do
	{ 
		i=i+1; 
		if (i>=trow2) bz=-99;
		if (bz>0)
		{
			// 第 i 线元
			for (k=0;k<5;k++) INDATA2[k]=array2[i][k];
			l=Get_INDATA_L(INDATA2,&j);
			if(i>=trow2-1)
				l += 0.1;//最后一个线元加一个毫米
			PBI->a  = array2[i][5];
			PBI->lc = array2[i][6];
			PBI->r  = array2[i][7];
			PBI->x  = array2[i][8];
			PBI->y  = array2[i][9];
			// l 为该线元的长度 m

			if( ( jslc <= PBI->lc+l ) && (jslc>=PBI->lc) ) 
				// 计算点在 i 线元上
			{  lc=jslc-PBI->lc;
			if (lc<=0.0) lc=0.000000000001;
			xlzbxy(PBI,PZ,lc,INDATA2,0);
			k=999;
			bz=-99;
			};
		}  // bz>0
	}while (bz>0);

	if (k==999) return j; //有解 j 为 线元类型
	else
	{  
		//ads_printf(L"xlpoint_pz求坐标无解\n");
		return (-9); // 无解
	};
}

//  在某层上画回旋线
AcDbObjectId   BAS_DRAW_FUN_RAIL::drawspi(double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ,int bz,double ND)
{   int j,k;
double lc,b=0;
ACHAR buf[220];
AcGePoint3dArray  pArr;
AcGePoint3d ptb,pt,pte,pb,pe;

//  if(INDATA[1]<0.00000001 || INDATA[2]<0.00000001) return 1;
if (bz==1||bz==-1)
{
	xlzbxy(P0,PZ,l*0.5-6*1,INDATA,0);b=PZ->a+pi*0.5*bz;
	pt[X]=PZ->y+5*sin(b);pt[Y]=PZ->x+5*cos(b);pt[Z]=0.0;
	b=PZ->a; b=pi*0.5-b;if (b<0.0) b=2*pi+b;
	if (b>2*pi) b=b-2*pi;
	// ads_rtos(INDATA[1]+0.00000005,2,7,ch);_tcscpy(chr,L"A=");_tcscat(chr,ch);
	// G_maketext(pt,chr,b,4,4);
};
pt[X]=P0->y;pt[Y]=P0->x;
pt[Z]=0.0;
pb[X]=pt[X];pb[Y]=pt[Y];
pb[Z]=0.0;
xlzbxy(P0,PZ,l,INDATA,0);
pe[X]=PZ->y;pe[Y]=PZ->x;
pe[Z]=0.0;

_stprintf(buf, L"%-5.0f %21.11f %20.8f %20.8f %20.8f %15.8f %12.8f %20.8f %20.8f %20.8f %20.8f\0",
		  INDATA[0],INDATA[1],INDATA[2],INDATA[3],INDATA[4],dms_rad(P0->a),dms_rad(PZ->a),pb[1],pb[0],pe[1],pe[0]);

k=(int)(l/ND+1);
pArr.setLogicalLength(k+1);
pArr[0].set(P0->y,P0->x,0.0);
for (k=1;k<=l/ND+1;k++)
{     lc=k*ND;
if (lc>l) lc=l;
xlzbxy(P0,PZ,lc,INDATA,0);
pt[X]=PZ->y;pt[Y]=PZ->x;pt[Z]=0.0;
pArr[k].set(PZ->y,PZ->x,0.0);
//  ads_printf(L"%12.5f %12.5f\n",pt[0],pt[1]);
};
AcDb2dPolyline *pPline = new AcDb2dPolyline(AcDb::k2dSimplePoly,pArr,0.0, Adesk::kFalse);
AcDbObjectId plId;
pPline->setColorIndex(5);	
plId=AddEntityToDbs(pPline);

// 加 xdata 数据 开始
struct  resbuf  *pRb, *pTemp;
ACHAR appName[20];    
_tcscpy(appName,L"GUADS");   
AcDbObject* pObj;
if ((j = acdbOpenObject(pObj, plId, AcDb::kForRead))!= Acad::eOk)
{
	ads_printf(L"acdbOpenEntity failed");
	return plId ;
};

// Add user specified string to the Xdata.
// CAD R2000 
acdbRegApp(appName);  
// pRb = acutNewRb(AcDb::kDxfRegAppName);
// CAD R14
//	ads_regapp(appName);
//    pRb = ads_newrb(AcDb::kDxfRegAppName);
pTemp = pRb;
pTemp->resval.rstring= (ACHAR*) malloc(_tcslen(appName) + 1);
_tcscpy(pTemp->resval.rstring, appName);

// Add user-specified string to the xdata.
// CAD R2000
pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
// CAD R14
//  pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
pTemp = pTemp->rbnext;
pTemp->resval.rstring = (ACHAR*) malloc(_tcslen(buf) + 1);
_tcscpy(pTemp->resval.rstring, buf);
// Following code shows the use of upgradeOpen
// to change the entity from read to write.

pObj->upgradeOpen();	
pObj->setXData(pRb);
pObj->close();
// CAD R2000
//  acutRelRb(pRb);   
// CAD R14
//     ads_relrb(pRb);   
// 加 xdata 数据 结束
return  plId ;
} 

AcDbObjectId  BAS_DRAW_FUN_RAIL::GUXWXY(double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,int bz)
{     int j,k;
double l=0.0;
AcGePoint3d ptb,pt,pte;
AcDbObjectId entid;
// if(INDATA[1]<0.00000001 || INDATA[2]<0.00000001) return  ;
l=Get_INDATA_L(INDATA,&j);
ptb[X]=P0->y;ptb[Y]=P0->x; ptb[Z]=0.0;pt[Z]=0.0;pte[Z]=0.0;
// if ((j<=3 &&j>=6 && INDATA[1]<=0.000000000000000001)|| (l<=0.00000000001)) return -99;
if (bz==1||bz==9)
{  k=bz; if(bz==9) k=0;
if (j>2 && j<=6 )
{ //( 回旋线)
	entid=drawspi(INDATA,l,P0,PZ,k,2.0);
	xlzbxy(P0,PZ,l,INDATA,0);
	ads_printf(L"4\n");
}
else if (j<=2)
{
	xlzbxy(P0,PZ,l*0.5,INDATA,0); // 某线元中点
	pt[X]=PZ->y;pt[Y]=PZ->x;
	xlzbxy(P0,PZ,l,INDATA,0); // 某线元终点
	pte[X]=PZ->y;pte[Y]=PZ->x;  //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
	if (j==1) 
	{
		//       acedCommandS(RTSTR, L"LINE", RTPOINT,ptb, RTPOINT, pte, RTSTR,L"",0);
		entid= makeline(ptb,pte,3);
	}
	else if (j==2) 
	{
		//   acedCommandS(RTSTR, L"ARC", RTPOINT,ptb,  RTPOINT, pt, RTPOINT, pte, 0);
		entid= makearc(ptb,pte,INDATA[1],(int)(INDATA[4]+0.001));        
	};
};  //J
}
else  if ( bz==0) {  xlzbxy(P0,PZ,l,INDATA,0); } ;  // 某线元终点
// P0=PZ;
return ( entid);
}

int BAS_DRAW_FUN_RAIL::getmodel(double x1,double y1,double x2,double y2,double x3,double y3,double L1,double L2,double R)
{      double fb,fe,db,de,a,aa,c,T0=0.0,T1,T2,L;
double BPMTL[10];
double array[4][5],INDATA[5];
xlpoint P00,PZZ;
int i,j,sbz=-1;
for(i=0;i<4;i++) { for (j=0;j<5;j++) array[i][j]=0.0;};
for (j=0;j<5;j++) INDATA[j]=0.0;
db=xyddaa(x1,y1,x2,y2,&fb);
de=xyddaa(x2,y2,x3,y3,&fe);
aa=fe-fb;

if (aa<-pi) aa=aa+2*pi;
if  (aa>pi) aa=aa-2*pi;
if (aa>0) {  c=1;   }  //右偏
else      {  c=-1;  };
a=fabs(aa);
if(fabs(a)<0.0000001 || fabs(pi-a)<0.0000001)
{ 	 ads_printf(L"\n该S/C型 不能求解 偏角太小(nothing to do )!!!");
ads_printf(L"\n a1(rad)=%12.f\n",aa);
sbz=-1;
return RTNORM;
};
a=fabs(aa);a=dms_rad(a);
if ( db>0.01&& de>0.01&&a>0.01)
{ sbz=1;//成功
for (i=0;i<10;i++) BPMTL[i]=0.0;
getBPMTL(a,R,L1,L2,&BPMTL[0]);
ads_printf(L"\n"); for (i=6;i<10;i++)  ads_printf(L" %10.3f",BPMTL[i]);
ads_printf(L"\n");
T1=BPMTL[6];T2=BPMTL[7];L=BPMTL[8];
array[0][0]=10.0;array[0][1]=x1+(db-T1)*cos(fb);array[0][2]=y1+(db-T1)*sin(fb);array[0][3]=fb;
array[1][0]=3.01;array[1][1]=sqrt(L1*R);array[1][2]=R;array[1][4]=c;//spiral
aa=L-L1-L2;
array[2][0]=2.01;array[2][1]=R;array[2][2]=aa;array[2][4]=c;//CIRCLE
array[3][0]=4.01;array[3][1]=sqrt(L2*R);array[3][2]=R;array[3][4]=c;//spiral
if( (L-L1-L2)<0.0)  // 圆曲线长<0
{ sbz=-1;
ads_printf(L"\n   该S/C型有解 但缓和曲线重叠!!!");
ads_printf(L"\n   圆曲线长arc_LL1=%-10.3f ",(L-L1-L2));
}
else {
	sbz=1;

	P00.x=array[0][1];P00.y=array[0][2];P00.a=array[0][3];P00.lc=0.0;
	if (L1>0&& L2>0)
	{
		for(i=1;i<=3;i++)
		{
			for(j=0;j<5;j++) INDATA[j]=array[i][j];
			ads_printf(L"\n indata=%3.1f %10.3f %10.3f %10.3f",INDATA[0],INDATA[1],INDATA[2],INDATA[4]);
			GUXWXY(INDATA,&P00,&PZZ,1);

			P00=PZZ;
		};
	};
};
};
return sbz;
}


// 在某实体的数据结果缓表中查给定的类型码的组
struct resbuf* BAS_DRAW_FUN_RAIL::entdxf( struct resbuf *entdata,int gcode)
{
	while ((entdata!=NULL) && (entdata->restype!=gcode)) entdata=entdata->rbnext;
	return entdata;
}


double BAS_DRAW_FUN_RAIL:: xyddaa(double xb,double yb,double xe,double ye,double *f)
{	double dx,dy,dd,aa;

dx=xe-xb;dy=ye-yb;dd=sqrt(dx*dx+dy*dy);
if (fabs(dx)<0.000000001 ) {
	if (dy>=0.0) aa=pi*0.5;
	else if (dy<0.0) aa=pi*1.5;
	*f=aa;
	return(dd);
}
else {  aa=atan(dy/dx);};

if (dy>=0 && dx<0) { aa=aa+pi;}
else if (dy<0 && dx<0) { aa=aa+pi;}
else if (dy<0 && dx>0) { aa=aa+2*pi; };
if (aa<0) { aa=aa+2*pi;}
else if (aa>2*pi) { aa=aa-2*pi;};
*f=aa;
return(dd);
}
double BAS_DRAW_FUN_RAIL::spiral_x(double A,double l)
{
	//	SpiralX ob;
	//	acutPrintf(L"X:%lf ",ob.SpiralGetX(A,l));

	if(A<=0)return 0.0;
	double t=l*l/2/A/A;
	double sum=0.0,dy=0.0;
	int n=0,max=10;
	double c=2*A*A,dx=0.0;
	for(n=0;n<max;n++)
	{
		dx=pow(-1.0,n+2)*pow(l,4*n+1)/(4*n+1)/pow(c,2*n)/factorialfun(2*n);//最后为阶乘
		sum+=dx;
	}
	//	acutPrintf(L"  %lf\n",sum);
	//	AfxMessageBox(L"0");
	return sum;
}

double BAS_DRAW_FUN_RAIL::spiral_y(double A,double l)
{
	//	SpiralY ob;
	//	acutPrintf(L"Y:%lf ",ob.SpiralGetY(A,l));

	if(A<=0)return 0.0;
	double t=l*l/2/A/A;
	int n=0,max=10;
	double sum=0.0,dy=0.0;
	for(n=1;n<=max;n++)
	{
		dy=pow(-1.0,n+1)*pow(t,0.5*(4*n-1))/(0.5*(4*n-1))/factorialfun(2*n-1);//最后为阶乘
		sum+=dy;
	}
	sum=sum*A/sqrt(2.0);
	//	acutPrintf(L"  %lf\n",sum);
	//	AfxMessageBox(L"1");
	return sum;
}

long BAS_DRAW_FUN_RAIL::factorialfun(int n)
{
	long result=1;
	for(int i=1;i<n+1;i++)result*=i;
	return result;
}

double BAS_DRAW_FUN_RAIL::rad(double d_ms)
{	double d;
d=deg(d_ms);
return (d*pi/180.0);
}
double BAS_DRAW_FUN_RAIL::dms_rad(double rad)
{	double d_ms,ddd;
ddd=rad*180.0/pi;
d_ms=dms(ddd);
return (d_ms);
}
double BAS_DRAW_FUN_RAIL::dms(double ddd)
{	int d,m; double s;
d=int( ddd);
m=int ((ddd-d)*60.0);
s=double (ddd-d)*3600.0-m*60.0;
return (d+m/100.0+s/10000.0);
}

double BAS_DRAW_FUN_RAIL::dms(double ddd, int Nw, ACHAR* DMS_Str)
{	
	int d,m; 
	double s;
	d=int( ddd);
	m=int ((ddd-d)*60.0);
	s=double (ddd-d)*3600.0-m*60.0;
	if (Nw==0)
		_stprintf(DMS_Str, L"%d°%d′%.0lf″", d, m, s);
	else if (Nw==1)
		_stprintf(DMS_Str, L"%d°%d′%.1lf″", d, m, s);
	else if (Nw==2)
		_stprintf(DMS_Str, L"%d°%d′%.2lf″", d, m, s);
	else if (Nw==3)
		_stprintf(DMS_Str, L"%d°%d′%.3lf″", d, m, s);
	else if (Nw==4)
		_stprintf(DMS_Str, L"%d°%d′%.4lf″", d, m, s);

	return (d+m/100.0+s/10000.0);
}
//a-偏角、R-半径、L1-前缓、L2-后缓
int BAS_DRAW_FUN_RAIL::getBPMTL(double a,double R,double L1,double L2,double *BPMTL)
{ // BPTL[9]=B1,B2,P1,P2,M1,M2,T1,T2,L
	double A1,A2,aa,B1,B2,P1,P2,M1,M2,T1,T2,LS;
	A1=sqrt(R*L1);A2=sqrt(R*L2);
	aa=rad(a);

	B1=L1*0.5/R;
	B2=L2*0.5/R;

	if (L1<0.0001) { P1=0.0;M1=0.0; }
	else {     P1=spiral_p(A1,L1);M1=spiral_xm(A1,L1);   };
	if (L2<0.0001) { P2=0.0;M2=0.0; }
	else {     P2=spiral_p(A2,L2); M2=spiral_xm(A2,L2);  };
	// P1=spiral_p(A1,L1);P2=spiral_p(A2,L2);//P1,P2
	// M1=spiral_xm(A1,L1);M2=spiral_xm(A2,L2);// M1,M2
	T1=(R+P1)*tan(aa*0.5)+M1-(P1-P2)/sin(aa);
	T2=(R+P2)*tan(aa*0.5)+M2+(P1-P2)/sin(aa);
	LS=(aa-B1-B2)*R+L1+L2;
	*(BPMTL+0)=B1;
	*(BPMTL+1)=B2;
	*(BPMTL+2)=P1;
	*(BPMTL+3)=P2;
	*(BPMTL+4)=M1;
	*(BPMTL+5)=M2;
	*(BPMTL+6)=T1;
	*(BPMTL+7)=T2;
	*(BPMTL+8)=LS;
	return(1);
}

double BAS_DRAW_FUN_RAIL::deg(double d_ms)
{	int d,m;
double s;
d=int(d_ms+0.000000001);m=int((d_ms-d)*100.0+0.00000000001);
s=d_ms*10000.0-d*10000.0-m*100.0;
return (d+m/60.0+s/3600.0);
}
double BAS_DRAW_FUN_RAIL::spiral_xm(double A,double L)
{ double R,t,XM;
R=A*A/L;t=A*A/(2.0*R*R);
XM=spiral_x(A,L)-R*sin(t);
return(XM);
}

double BAS_DRAW_FUN_RAIL::spiral_p(double A,double L)
{ double R,t,p;
R=A*A/L;t=A*A/(2.0*R*R);
p=spiral_y(A,L)+R*cos(t)-R;
return(p);
}
void BAS_DRAW_FUN_RAIL::printList(struct resbuf* pBuf)
{
	int rt, i;
	ACHAR buf[133];  

	for (i = 0;pBuf != NULL;i++, pBuf = pBuf->rbnext) {
		if (pBuf->restype < 0)
			// Entity name (or other sentinel)
				rt = pBuf->restype;
		else if (pBuf->restype < 10)
			rt = RTSTR;
		else if (pBuf->restype < 38)
			rt = RT3DPOINT;
		else if (pBuf->restype < 60)
			rt = RTREAL;
		else if (pBuf->restype < 80)
			rt = RTSHORT;
		else if (pBuf->restype < 100)
			rt = RTLONG;
		else if (pBuf->restype < 106)
			rt = RTSTR;
		else if (pBuf->restype < 148)
			rt = RTREAL;
		else if (pBuf->restype < 290)
			rt = RTSHORT;
		else if (pBuf->restype < 320)
			rt = RTSTR;
		else if (pBuf->restype < 370)
			rt = RTENAME;
		else if (pBuf->restype < 999)
			rt = RT3DPOINT;
		else // pBuf->restype is already RTSHORT, RTSTR,
			rt = pBuf->restype; // etc. or it is unknown.

		switch (rt) {
		case RTSHORT:
			if (pBuf->restype == RTSHORT)
				ads_printf(
				L"RTSHORT : %d\n", pBuf->resval.rint);
			else
				ads_printf(L"(%d . %d)\n", pBuf->restype,
				pBuf->resval.rint);
			break;
		case RTREAL:
			if (pBuf->restype == RTREAL)
				ads_printf(
				L"RTREAL : %0.3f\n", pBuf->resval.rreal);
			else
				ads_printf(L"(%d . %0.3f)\n",
				pBuf->restype, pBuf->resval.rreal);
			break;
		case RTSTR:
			if (pBuf->restype == RTSTR)
				ads_printf(
				L"RTSTR : %s\n", pBuf->resval.rstring);
			else
				ads_printf(L"(%d . \"%s\")\n",
				pBuf->restype, pBuf->resval.rstring);
			break;
		case RT3DPOINT:
			if (pBuf->restype == RT3DPOINT)
				ads_printf(
				L"RT3DPOINT : %0.3f, %0.3f, %0.3f\n",
				pBuf->resval.rpoint[X],
				pBuf->resval.rpoint[Y],
				pBuf->resval.rpoint[Z]);
			else
				ads_printf(L"(%d %0.3f %0.3f %0.3f)\n",
				pBuf->restype,
				pBuf->resval.rpoint[X],
				pBuf->resval.rpoint[Y],
				pBuf->resval.rpoint[Z]);
			break;
		case RTLONG:
			ads_printf(L"RTLONG : %dl\n", pBuf->resval.rlong);
			break;
		case -1:
		case RTENAME: // First block entity
			ads_printf(L"(%d . <Entity name: %8lx>)\n",
				pBuf->restype, pBuf->resval.rlname[0]);
			break;
		case -3: // marks start of xdata
			ads_printf(L"(-3)\n");
		}

		if ((i == 23) && (pBuf->rbnext != NULL)) {
			i = 0;
			//  CAD R2000
			// acedGetString(0, L"Press <ENTER> to continue...", buf);
			//  CAD R14
			ads_getstring(0, L"Press <ENTER> to continue...", buf);

		}
	}
	return;
}
void  BAS_DRAW_FUN_RAIL::iterate(AcDbObjectId plineId)
{
	AcDb2dPolyline *pPline;
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, plineId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, plineId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pPline = AcDb2dPolyline::cast(pObj);
	AcDbObjectIterator *pVertIter= pPline->vertexIterator();
	pPline->close();  // Finished with the pline header.

	AcDb2dVertex *pVertex;
	AcGePoint3d location;
	AcDbObjectId vertexObjId;
	for (int vertexNumber = 0; !pVertIter->done();
		vertexNumber++, pVertIter->step())
	{
		vertexObjId = pVertIter->objectId();
		/*acdbOpenObject(pObj, vertexObjId,
		AcDb::kForRead);*/
		if(acdbOpenObject(pObj, vertexObjId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pVertex = AcDb2dVertex::cast(pObj);
		location = pVertex->position();
		pVertex->close();

		ads_printf(L"\nVertex #%d's location is"
			L" : %0.3f, %0.3f, %0.3f", vertexNumber,
			location[X], location[Y], location[Z]);
	}
	if (pVertIter != NULL)
	{
		delete pVertIter;
		pVertIter=NULL;
	}
}
AcDbObjectId  BAS_DRAW_FUN_RAIL::createPl_xlpm(int trow,double arrayd[MAXJDNUM][6])
{
	// Set trow vertex locations for the pline
	//

	AcGePoint3dArray ptArr;
	ptArr.setLogicalLength(trow);
	ads_printf(L"trow=%d \n",trow);
	for (int i = 0; i < trow; i++) ptArr[i].set(arrayd[i][2],arrayd[i][1], 0.0);

	//
	AcDb2dPolyline *pNewPline = new AcDb2dPolyline(AcDb::k2dSimplePoly, ptArr, 0.0, Adesk::kFalse);

	pNewPline->setColorIndex(1);
	pNewPline->setLayer(L"0");
	// Get a pointer to a BlockTable Object   
	AcDbObjectId  plId;
	plId=AddEntityToDbs( pNewPline);

	return plId;
}
ACHAR* BAS_DRAW_FUN_RAIL::LCchr(ACHAR  *GH,double lc,int NLC,int mode=1)
{
	long int zlc,gl;
	double bm;
	ACHAR  ch1[20],ch[40];
	bm=lc;
	zlc=(long int)((lc+0.0001)/1000.0); 
	bm=bm-zlc*1000.0;
	gl=zlc;
	ads_rtos(gl,2,0,ch1);
	_tcscpy(ch,GH);
	_tcscat(ch,ch1);
	// _tcscat(ch,L"+");
	bm=bm+1000.0;
	ads_rtos(bm,2,NLC,ch1);
	ch1[0]='+';
	_tcscat(ch,ch1);
	return &ch[0];
}
void BAS_DRAW_FUN_RAIL::calptan(double x1, double y1, double x2, double y2, double x3, double y3, double xt, double yt,double *tanl, double *R, double *xc,double *yc,double *ang,double *startfw,double *endfw)
{
	double sfw,efw,tfw,lt,alpha,bta,gma,alphac;
	int inf;
	ads_point pt[4];
	Spoint(pt[0],x1,y1,0);
	Spoint(pt[1],x2,y2,0);
	Spoint(pt[2],x3,y3,0);
	Spoint(pt[3],xt,yt,0);
	sfw =ads_angle(pt[0],pt[1]);
	efw =ads_angle(pt[1],pt[2]);
	tfw =ads_angle(pt[1],pt[3]);
	lt = ads_distance(pt[1],pt[3]);
	if ( sfw <= pi  )
	{
		if ( efw > sfw && efw  <= sfw+ pi  ) { alpha = efw - sfw ; inf = -1;}
		else  { alpha = sfw - efw ; inf = 1;};
		if ( alpha < 0.0 ) alpha = alpha + 2.0*pi ;
	}
	else if ( sfw > pi  )
	{
		if ( efw > sfw - pi  && efw  < sfw ) { alpha = sfw - efw ; inf = 1; }
		else  { alpha = efw - sfw ; inf = -1;};
		if ( alpha < 0.0 ) alpha = alpha + 2.0*pi ;
	};
	alpha = pi  - alpha;
	alpha = 0.5*alpha; 
	alphac= sfw +pi  + alpha*inf;

	if ( sfw <= pi  )
	{
		if ( tfw > sfw && tfw  <= sfw+ pi  ) { bta = tfw - sfw ; inf = -1;}
		else  { bta = sfw - tfw ; inf = 1;};
		if ( bta < 0.0 ) bta = bta + 2.0*pi ;
	}
	else if ( sfw > pi  )
	{
		if ( tfw > sfw - pi  && tfw  < sfw ) { bta = sfw - tfw ; inf = 1; }
		else  { bta = tfw - sfw ; inf = -1;};
		if ( bta < 0.0 ) bta = bta + 2.0*pi ;
	};
	bta = pi  - bta;
	gma = alpha - bta ;
	double b,c;
	b = -2.0*cos(gma)*lt/cos(alpha);
	c = lt*lt;

	*tanl =0.5*( -b+sqrt(b*b-4.0*c) );
	*R =(*tanl) * tan(alpha);
	acutPrintf(L"R=%lf\n",*R);

	double tmp;
	tmp = (*tanl)/cos(alpha);
	*xc = x2 + tmp*cos(alphac);
	*yc = y2 + tmp*sin(alphac);
	*ang = alpha;

	if ( inf  == 1 )
	{
		*startfw = efw + 0.5*pi ;
		*endfw   = sfw + 0.5*pi ;
	}
	else
	{
		*startfw = sfw - 0.5*pi ;
		*endfw   = efw - 0.5*pi ;
	};
}

double BAS_DRAW_FUN_RAIL::get_l0(double SR,double Design_V, GuiFanCanShu mGuiFanCS)
{
	/*
	double l0 =0.0;
	//   double Design_V = 80; 
	int KNLB = 0;

	if (Design_V > 299)
	{ 
	if ( KNLB == 0 )
	{
	if      (fabs(SR-5500 )<0.1) l0 = 700;
	else if (fabs(SR-6000 )<0.1) l0 = 700;
	else if (fabs(SR-7000 )<0.1) l0 = 670;
	else if (fabs(SR-8000 )<0.1) l0 = 570;
	else if (fabs(SR-9000 )<0.1 ) l0 =  490;
	else if (fabs(SR-10000 )<0.1 ) l0 =  430;
	else if (fabs(SR-11000 )<0.1 ) l0 =  370;
	else if (fabs(SR-12000 )<0.1) l0 =  330;
	else if (fabs(SR-14000 )<0.1 ) l0 =  280;		  
	}
	else if ( KNLB == 1 )
	{
	if      (fabs(SR-5500 )<0.1) l0 = 610;
	else if (fabs(SR-6000 )<0.1) l0 = 570;
	else if (fabs(SR-7000 )<0.1) l0 = 540;
	else if (fabs(SR-8000 )<0.1) l0 = 470;
	else if (fabs(SR-9000 )<0.1 ) l0 =  400;
	else if (fabs(SR-10000 )<0.1 ) l0 =  350;
	else if (fabs(SR-11000 )<0.1 ) l0 =  300;
	else if (fabs(SR-12000 )<0.1) l0 =  270;
	else if (fabs(SR-14000 )<0.1 ) l0 =  250;		  
	}
	else
	{
	if      (fabs(SR-5500 )<0.1) l0 = 490;
	else if (fabs(SR-6000 )<0.1) l0 = 460;
	else if (fabs(SR-7000 )<0.1) l0 = 430;
	else if (fabs(SR-8000 )<0.1) l0 = 380;
	else if (fabs(SR-9000 )<0.1 ) l0 =  320;
	else if (fabs(SR-10000 )<0.1 ) l0 =  270;
	else if (fabs(SR-11000 )<0.1 ) l0 =  220;
	else if (fabs(SR-12000 )<0.1) l0 =  190;
	else if (fabs(SR-14000 )<0.1 ) l0 =  150;		  
	}
	}
	else if (fabs(Design_V- 250 )<0.1)
	{ 
	if ( KNLB == 0 )
	{
	if      (fabs(SR-3500 )<0.1) l0 = 420 ;
	else if (fabs(SR-4000 )<0.1) l0 = 390;
	else if (fabs(SR-4500 )<0.1) l0 = 360;
	else if (fabs(SR-5000 )<0.1) l0 = 330;
	else if (fabs(SR-5500 )<0.1 ) l0 =  300;
	else if (fabs(SR-6000 )<0.1 ) l0 =  270;
	else if (fabs(SR-6500 )<0.1 ) l0 =  240;
	else if (fabs(SR-7000 )<0.1) l0 =  210;
	else if (fabs(SR-8000 )<0.1 ) l0 =  190;
	else if (fabs(SR-9000 )<0.1) l0 =  170;
	else if (fabs(SR-10000 )<0.1 ) l0 = 150;
	else if (fabs(SR-11000 )<0.1 ) l0 = 130;
	else if (fabs(SR-12000 )<0.1) l0 =  110;
	}
	else if ( KNLB == 1 )
	{
	if      (fabs(SR-3500 )<0.1) l0 = 340 ;
	else if (fabs(SR-4000 )<0.1) l0 = 320;
	else if (fabs(SR-4500 )<0.1) l0 = 300;
	else if (fabs(SR-5000 )<0.1) l0 = 280;
	else if (fabs(SR-5500 )<0.1 ) l0 =  260;
	else if (fabs(SR-6000 )<0.1 ) l0 =  230;
	else if (fabs(SR-6500 )<0.1 ) l0 =  200;
	else if (fabs(SR-7000 )<0.1) l0 =  180;
	else if (fabs(SR-8000 )<0.1 ) l0 =  160;
	else if (fabs(SR-9000 )<0.1) l0 =  140;
	else if (fabs(SR-10000 )<0.1 ) l0 = 120;
	else if (fabs(SR-11000 )<0.1 ) l0 = 100;
	else if (fabs(SR-12000 )<0.1) l0 =  90;
	}
	else
	{
	if      (fabs(SR-3500 )<0.1) l0 = 300;
	else if (fabs(SR-4000 )<0.1) l0 = 280;
	else if (fabs(SR-4500 )<0.1) l0 = 260;
	else if (fabs(SR-5000 )<0.1) l0 = 240;
	else if (fabs(SR-5500 )<0.1 ) l0 =  220;
	else if (fabs(SR-6000 )<0.1 ) l0 =  200;
	else if (fabs(SR-6500 )<0.1 ) l0 =  180;
	else if (fabs(SR-7000 )<0.1) l0 =  160;
	else if (fabs(SR-8000 )<0.1 ) l0 =  140;
	else if (fabs(SR-9000 )<0.1) l0 =  120;
	else if (fabs(SR-10000 )<0.1 ) l0 = 100;
	else if (fabs(SR-11000 )<0.1 ) l0 = 90;
	else if (fabs(SR-12000 )<0.1) l0 =  80;
	}
	}
	else if (fabs(Design_V- 200 )<0.1)
	{ 
	if ( KNLB == 0 )
	{
	if      (fabs(SR-2200 )<0.1) l0 = 340 ;
	else if (fabs(SR-2500 )<0.1) l0 = 320;
	else if (fabs(SR-2800 )<0.1) l0 = 300;
	else if (fabs(SR-3000 )<0.1) l0 = 280;
	else if (fabs(SR-3500 )<0.1 ) l0 =  260;
	else if (fabs(SR-4000 )<0.1 ) l0 =  240;
	else if (fabs(SR-4500 )<0.1 ) l0 =  210;
	else if (fabs(SR-5000 )<0.1) l0 =  180;
	else if (fabs(SR-5500 )<0.1 ) l0 =  160;
	else if (fabs(SR-6000 )<0.1) l0 =  140;
	else if (fabs(SR-7000 )<0.1) l0 =  120;
	else if (fabs(SR-8000 )<0.1) l0 =  100;
	else if (fabs(SR-9000 )<0.1) l0 =  90;
	else if (fabs(SR-10000 )<0.1 ) l0 =  80;
	else if (fabs(SR-11000 )<0.1 ) l0 =  70;
	else if (fabs(SR-12000 )<0.1) l0 =  60;
	}
	else if ( KNLB == 1 )
	{
	if      (fabs(SR-2200 )<0.1) l0 = 270 ;
	else if (fabs(SR-2500 )<0.1) l0 = 250;
	else if (fabs(SR-2800 )<0.1) l0 = 240;
	else if (fabs(SR-3000 )<0.1) l0 = 230;
	else if (fabs(SR-3500 )<0.1 ) l0 =  210;
	else if (fabs(SR-4000 )<0.1 ) l0 =  190;
	else if (fabs(SR-4500 )<0.1 ) l0 =  170;
	else if (fabs(SR-5000 )<0.1) l0 =  150;
	else if (fabs(SR-5500 )<0.1 ) l0 =  130;
	else if (fabs(SR-6000 )<0.1) l0 =  110;
	else if (fabs(SR-7000 )<0.1) l0 =  100;
	else if (fabs(SR-8000 )<0.1) l0 =  90;
	else if (fabs(SR-9000 )<0.1) l0 =  80;
	else if (fabs(SR-10000 )<0.1 ) l0 =  70;
	else if (fabs(SR-11000 )<0.1 ) l0 =  60;
	else if (fabs(SR-12000 )<0.1) l0 =  50;
	}
	else
	{
	if      (fabs(SR-2200 )<0.1) l0 = 240 ;
	else if (fabs(SR-2500 )<0.1) l0 = 230;
	else if (fabs(SR-2800 )<0.1) l0 = 220;
	else if (fabs(SR-3000 )<0.1) l0 = 210;
	else if (fabs(SR-3500 )<0.1 ) l0 =  190;
	else if (fabs(SR-4000 )<0.1 ) l0 =  170;
	else if (fabs(SR-4500 )<0.1 ) l0 =  150;
	else if (fabs(SR-5000 )<0.1) l0 =  130;
	else if (fabs(SR-5500 )<0.1 ) l0 =  110;
	else if (fabs(SR-6000 )<0.1) l0 =  100;
	else if (fabs(SR-7000 )<0.1) l0 =  90;
	else if (fabs(SR-8000 )<0.1) l0 =  80;
	else if (fabs(SR-9000 )<0.1) l0 =  70;
	else if (fabs(SR-10000 )<0.1 ) l0 =  60;
	else if (fabs(SR-11000 )<0.1 ) l0 =  50;
	else if (fabs(SR-12000 )<0.1) l0 =  40;
	}
	}
	else if (fabs(Design_V- 160 )<0.1)
	{ 
	if      (fabs(SR-1600 )<0.1) l0 = 190 ;
	else if (fabs(SR-2000 )<0.1) l0 = 150;
	else if (fabs(SR-2500 )<0.1) l0 = 120;
	else if (fabs(SR-3000 )<0.1) l0 = 100;
	else if (fabs(SR-3500 )<0.1 ) l0 =  80;
	else if (fabs(SR-4000 )<0.1 ) l0 =  80;
	else if (fabs(SR-4500 )<0.1 ) l0 =  70;
	else if (fabs(SR-5000 )<0.1) l0 =  60;
	else if (fabs(SR-6000 )<0.1 ) l0 =  60;
	else if (fabs(SR-8000 )<0.1) l0 =  40;
	else if (fabs(SR-10000 )<0.1 ) l0 =  40;
	else if (fabs(SR-12000 )<0.1) l0 =  40;

	}
	else if (fabs(Design_V- 140 )<0.1)
	{ 
	if ( KNLB == 0 )
	{
	if      (fabs( SR - 1200 )<0.1) l0 = 150 ;
	else if ( fabs( SR - 1400 )<0.1 ) l0 = 130;
	else if ( fabs( SR - 1600 )<0.1 ) l0 = 110;
	else if (fabs( SR - 1800 )<0.1 ) l0 = 100;
	else if ( fabs( SR - 2000 )<0.1 ) l0 =  90;
	else if ( fabs( SR - 2500 )<0.1) l0 =  80;
	else if ( fabs( SR - 3000 )<0.1 ) l0 =  70;
	else if ( fabs( SR - 4000 )<0.1 ) l0 =  60;
	else if ( fabs( SR - 5000 )<0.1) l0 =  60;
	else if ( fabs( SR - 6000 )<0.1 ) l0 =  50;
	else if ( fabs( SR - 8000 )<0.1 ) l0 =  40;
	else if ( fabs( SR - 10000 )<0.1 ) l0 =  30;
	}
	else
	{	
	if      ( fabs( SR - 1200 )<0.1 ) l0 = 130 ;
	else if ( fabs( SR - 1400 )<0.1 ) l0 = 110;
	else if ( fabs( SR - 1600 )<0.1 ) l0 = 100;
	else if ( fabs( SR - 1800 )<0.1) l0 =  80;
	else if ( fabs( SR - 2000 )<0.1 ) l0 =  80;
	else if ( fabs( SR - 2500 )<0.1 ) l0 =  70;
	else if ( fabs( SR - 3000 )<0.1) l0 =  50;
	else if ( fabs( SR - 4000 )<0.1 ) l0 =  40;
	else if ( fabs( SR - 5000 )<0.1 ) l0 =  40;
	else if ( fabs( SR - 6000 )<0.1 ) l0 =  30;
	else if ( fabs( SR - 8000 )<0.1 ) l0 =  20;
	else if ( fabs( SR - 10000 )<0.1 ) l0 =  20;
	};
	}
	else if ( fabs(Design_V - 120)<0.1)
	{ 
	if ( KNLB == 0 )
	{
	if      ( fabs(SR-800)<0.1) l0 = 150 ;
	else if ( fabs(SR-1000)<0.1) l0 = 120;
	else if ( fabs(SR-1200)<0.1 ) l0 =  90;
	else if ( fabs(SR-1400)<0.1 ) l0 =  80;
	else if ( fabs(SR-1600)<0.1 ) l0 =  70;
	else if ( fabs(SR-1800)<0.1 ) l0 =  70;
	else if ( fabs(SR-2000)<0.1 ) l0 =  60;
	else if ( fabs(SR-2500)<0.1 ) l0 =  60;
	else if ( fabs(SR-3000)<0.1 ) l0 =  50;
	else if ( fabs(SR-4000)<0.1 ) l0 =  50;
	else if ( fabs(SR-5000)<0.1 ) l0 =  40;
	else if ( fabs(SR-6000)<0.1 ) l0 =  30;
	else if ( fabs(SR-8000)<0.1 ) l0 =  30;
	else if ( fabs(SR-1000)<0.1 ) l0 =  20;
	}
	else
	{	
	if      ( fabs(SR-800)<0.1 ) l0 = 130;
	else if ( fabs(SR-1000)<0.1 ) l0 = 100;
	else if ( fabs(SR-1200)<0.1 ) l0 =  80;
	else if ( fabs(SR-1400)<0.1 ) l0 =  70;
	else if ( fabs(SR-1600)<0.1 ) l0 =  60;
	else if ( fabs(SR-1800)<0.1 ) l0 =  60;
	else if ( fabs(SR-2000)<0.1 ) l0 =  50;
	else if ( fabs(SR-2500)<0.1 ) l0 =  40;
	else if ( fabs(SR-3000)<0.1 ) l0 =  40;
	else if ( fabs(SR-4000)<0.1 ) l0 =  30;
	else if ( fabs(SR-5000)<0.1 ) l0 =  30;
	else if ( fabs(SR-6000)<0.1 ) l0 =  20;
	else if ( fabs(SR-8000)<0.1 ) l0 =  20;
	else if ( fabs(SR-10000)<0.1 ) l0 =  20;
	};
	}
	else if (fabs(Design_V - 100 )<0.1)
	{
	if ( KNLB == 0 )
	{
	if      ( fabs(SR-550)<0.1) l0 = 130 ;
	else if ( fabs(SR-600)<0.1 ) l0 = 120;
	else if ( fabs(SR-700)<0.1 ) l0 = 100;
	else if ( fabs(SR-800)<0.1 ) l0 =  80;
	else if ( fabs(SR-1000)<0.1 ) l0 =  70;
	else if ( fabs(SR-1200)<0.1 ) l0 =  60;
	else if ( fabs(SR-1400)<0.1 ) l0 =  60;
	else if ( fabs(SR-1600)<0.1) l0 =  50;
	else if ( fabs(SR-1800)<0.1) l0 =  50;
	else if ( fabs(SR-2000)<0.1 ) l0 =  50;
	else if ( fabs(SR-2500)<0.1 ) l0 =  40;
	else if ( fabs(SR-3000)<0.1 ) l0 =  40;
	else if ( fabs(SR-4000)<0.1 ) l0 =  30;
	else if ( fabs(SR-5000)<0.1 ) l0 =  20;
	else if ( fabs(SR-6000)<0.1 ) l0 =  20;
	else if ( fabs(SR-8000)<0.1 ) l0 =  20;
	else if ( fabs(SR-10000)<0.1 ) l0 =  20;
	}
	else
	{
	if      ( fabs(SR-550)<0.1) l0 = 110 ;
	else if (  fabs(SR-600)<0.1 ) l0 = 100;
	else if ( fabs(SR-700)<0.1  ) l0 =  90;
	else if ( fabs(SR-800)<0.1 ) l0 =  70;
	else if ( fabs(SR-1000)<0.1 ) l0 =  60;
	else if ( fabs(SR-1200)<0.1 ) l0 =  50;
	else if ( fabs(SR-1400)<0.1 ) l0 =  40;
	else if ( fabs(SR-1600)<0.1 ) l0 =  40;
	else if ( fabs(SR-1800)<0.1 ) l0 =  40;
	else if ( fabs(SR-2000)<0.1 ) l0 =  40;
	else if ( fabs(SR-2500)<0.1 ) l0 =  30;
	else if ( fabs(SR-3000)<0.1 ) l0 =  20;
	else if ( fabs(SR-4000)<0.1 ) l0 =  20;
	else if ( fabs(SR-5000)<0.1 ) l0 =  20;
	else if ( fabs(SR-6000)<0.1 ) l0 =  20;
	else if ( fabs(SR-8000)<0.1 ) l0 =  20;
	else if ( fabs(SR-10000)<0.1 ) l0 =  20;
	};
	}
	else if (fabs(Design_V - 80)<0.1)
	{
	if ( KNLB == 0 )
	{   if ( SR < 300 ) l0 = 0.0;         
	else if ( SR <=  351 ) l0 = 100 ;
	else if ( SR <=  401 ) l0 =  90;
	else if ( SR <=  451 ) l0 =  80;
	else if ( SR <=  501 ) l0 =  60;
	else if ( SR <=  551 ) l0 =  60;
	else if ( SR <=  601 ) l0 =  60;
	else if ( SR <=  701 ) l0 =  50;
	else if ( SR <=  801 ) l0 =  50;
	else if ( SR <= 1001 ) l0 =  40;
	else if ( SR <= 1201 ) l0 =  40;
	else if ( SR <= 1401 ) l0 =  40;
	else if ( SR <= 1601 ) l0 =  40;
	else if ( SR <= 1801 ) l0 =  30;
	else if ( SR <= 2001 ) l0 =  30;
	else if ( SR <= 2501 ) l0 =  30;
	else if ( SR <= 3001 ) l0 =  20;
	else if ( SR <= 4001 ) l0 =  20;
	else if ( SR <= 5001 ) l0 =  20;
	else if ( SR <= 6001 ) l0 =  20;
	else if ( SR <= 8001 ) l0 =  20;
	else if ( SR <=10001 ) l0 =  20;
	}
	else
	{	
	if      ( SR <=  351 ) l0 =  90 ;
	else if ( SR <=  401 ) l0 =  80;
	else if ( SR <=  451 ) l0 =  70;
	else if ( SR <=  501 ) l0 =  60;
	else if ( SR <=  551 ) l0 =  50;
	else if ( SR <=  601 ) l0 =  50;
	else if ( SR <=  701 ) l0 =  40;
	else if ( SR <=  801 ) l0 =  40;
	else if ( SR <= 1001 ) l0 =  30;
	else if ( SR <= 1201 ) l0 =  30;
	else if ( SR <= 1401 ) l0 =  20;
	else if ( SR <= 1601 ) l0 =  20;
	else if ( SR <= 1801 ) l0 =  20;
	else if ( SR <= 2001 ) l0 =  20;
	else if ( SR <= 2501 ) l0 =  20;
	else if ( SR <= 3001 ) l0 =  20;
	else if ( SR <= 4001 ) l0 =  20;
	else if ( SR <= 5001 ) l0 =  20;
	else if ( SR <= 6001 ) l0 =  20;
	else if ( SR <= 8001 ) l0 =  20;
	else if ( SR <=10001 ) l0 =  20;
	};
	}
	else if (fabs(Design_V - 70)<0.1)
	{
	if(SR<301) l0 =  70;
	else if(SR<351) l0 = 60;
	else if(SR<401) l0 = 50;
	else if(SR<451) l0 = 50;
	else if(SR<501) l0 = 40;
	else if(SR<551) l0 = 40;
	else if(SR<601) l0 = 40;
	else if(SR<701) l0 = 30;
	else if(SR<801) l0 = 30;
	else l0 = 20;
	}
	else if (fabs(Design_V - 60)<0.1)
	{
	if(SR<301) l0 =  50;
	else if(SR<351) l0 = 40;
	else if(SR<401) l0 = 40;
	else if(SR<451) l0 = 40;
	else if(SR<501) l0 = 30;
	else if(SR<551) l0 = 30;
	else if(SR<601) l0 = 30;
	else if(SR<2501) l0 = 20;
	else  l0 = 0;
	}
	else if (fabs(Design_V - 55)<0.1)
	{
	if(SR<251) l0 =  50;
	else if(SR<301) l0 = 40;
	else if(SR<351) l0 = 40;
	else if(SR<551) l0 = 30;
	else if(SR<2001) l0 = 20;
	else  l0 = 0;
	}
	else if (fabs(Design_V - 45)<0.1)
	{
	if(SR<251) l0 =  40;
	else if(SR<351) l0 = 30;
	else if(SR<1501) l0 = 20;
	else  l0 = 0;
	}
	else if(fabs(Design_V - 40)<0.1)
	{
	if(SR<151) l0 =  50;
	else if(SR<201) l0 = 40;
	else if(SR<301) l0 = 30;
	else if(SR<1201) l0 = 20;
	else  l0 = 0;
	}
	else if(fabs(Design_V - 30)<0.1)
	{
	if(SR<151) l0 =  30;
	else if(SR<601) l0 = 20;
	else  l0 = 0;
	}

	return l0;*/

	int i=mGuiFanCS.RXiangShu-1;
	while(i>=0 && mGuiFanCS.R[i]<SR-1)
		i--;

	if(i==mGuiFanCS.RXiangShu-1)
		return mGuiFanCS.L[i];
	else if(i==0)
	{
		if(SR>mGuiFanCS.R[0])
			return mGuiFanCS.L[0];
		else
			i++;
	}  

	double l0;
	int kk;
	l0 = mGuiFanCS.L[i-1]+ (SR-mGuiFanCS.R[i-1])*(mGuiFanCS.L[i]-mGuiFanCS.L[i-1])/(mGuiFanCS.R[i]-mGuiFanCS.R[i-1]);
	kk = int(l0/10.0 + 0.99);
	l0 = kk * 10.0;
	return  l0;		
}

double BAS_DRAW_FUN_RAIL::get_R(double SR,double Design_V, GuiFanCanShu mGuiFanCS)//按照半径系列取半径值，寻找最接近半径
{
	int i=mGuiFanCS.RXiangShu-1;
	while(i>0 && mGuiFanCS.R[i]<SR-1)
		i--;

	if(i==mGuiFanCS.RXiangShu-1)
		return mGuiFanCS.R[i];
	else if(i==0)
	{
		if(SR>mGuiFanCS.R[0])
			return mGuiFanCS.R[0];
		else
			i++;
	}  
	return  fabs(mGuiFanCS.R[i]-SR)>fabs(mGuiFanCS.R[i-1]-SR) ? mGuiFanCS.R[i-1] : mGuiFanCS.R[i];		

}
AcDbObjectId BAS_DRAW_FUN_RAIL::CreateLayer(ACHAR *LayerName, ACHAR *LineType)
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

AcDbObjectId BAS_DRAW_FUN_RAIL::CreateLayer(ACHAR *LayerName)
{
	AcDbLayerTable *pLayerTable;

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
	//	acutPrintf(L"\nId=%d",ltypeObjId);
	pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	return LayerTabRecordId;

}

void BAS_DRAW_FUN_RAIL::caljd(double x1, double y1, double x2, double y2, double x_1, double y_1, double x_2, double y_2, double *xjd, double *yjd)
{
	double a1,b1,c1,a2,b2,c2,abc;
	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = y1*(x2-x1)-x1*(y2-y1);
	a2 = y_2 - y_1;
	b2 = x_1 - x_2;
	c2 = y_1*(x_2-x_1)-x_1*(y_2-y_1);
	abc = a1*b2 -a2*b1;
	*yjd = (c1*a2 -a1*c2)/abc;
	*xjd = -(c1*b2 -b1*c2)/abc; 
}
void BAS_DRAW_FUN_RAIL::calzxjd(double x1, double y1, double x2, double y2, double a2, double b2, double c2, double *xjd, double *yjd)
{
	double a1,b1,c1,abc;
	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = y1*(x2-x1)-x1*(y2-y1);
	abc = a1*b2 -a2*b1;
	*yjd = (c1*a2 -a1*c2)/abc;
	*xjd =-(c1*b2 -b1*c2)/abc; 
}


AcDbObjectId  BAS_DRAW_FUN_RAIL::makecircle(AcGePoint3d& startPt,double R,int icolor)
{
	AcGeVector3d  normal(0.0,0.0,1.0);
	AcDbCircle *pC = new AcDbCircle(startPt,normal, R);
	pC->setColorIndex(icolor);

	AcDbObjectId  LId;
	LId=AddEntityToDbs(pC);
	return LId;     
}

double BAS_DRAW_FUN_RAIL::XLC1byt2y( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[30])
{   double LCB=0, XCLC ;  // LCB 断链的统一里程
int iDL=0,bz;
//起点里程
XCLC=TYLC;
_tcscpy(GH,DLBTMP[0].BGH);

do {
	bz=99;
	//该断链点统一里程
	if  (iDL==0) { LCB=DLBTMP[iDL].BLC+DLBTMP[iDL].DL;  }
	else {  LCB=LCB+DLBTMP[iDL].BLC-(DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL); };  
	_tcscpy(GH,DLBTMP[iDL].EGH);

	if( (TYLC>LCB) &&(iDL<NDL))
	{   XCLC=(TYLC-LCB)+DLBTMP[iDL].BLC+DLBTMP[iDL].DL; 
	_tcscpy(GH,DLBTMP[iDL].EGH);
	iDL=iDL+1;
	bz=-99;
	}
	else if (TYLC<=LCB)
	{   XCLC=(TYLC-LCB)+DLBTMP[iDL].BLC; 
	_tcscpy(GH,DLBTMP[iDL].BGH);
	if(iDL==0)
		_tcscpy(GH,DLBTMP[iDL].EGH);
	bz=99;	            
	};
} while (bz<0);
return XCLC;
}
//根据彭工建议修改
double BAS_DRAW_FUN_RAIL:: XLC1( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[30])
{   
	double LCB=0;
	double XCLC=0.0;// LCB 断链的统一里程
	int iDL=0;
	CString strs1,strs2;
	for (iDL=0;iDL<NDL;iDL++)
	{
		//该断链点统一里程
		if(iDL > 0)
			LCB=LCB+DLBTMP[iDL].BLC-(DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL);//LCB表示以0号断链的断后里程为起始的，到该断链处的几何长度
		else
			LCB=DLBTMP[iDL].BLC+DLBTMP[iDL].DL;//iDL=0时LCB第一断链的断后里程

		if (iDL == 0)//TYLC在0号断链范围
		{
			if (TYLC < LCB)
			{
				strs1 = DLBTMP[0].BGH;
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				XCLC = DLBTMP[0].BLC - LCB + TYLC;
				break;
			}
			else if (fabs(TYLC-LCB)<0.00005)//==
			{
				if (fabs(DLBTMP[0].BLC-floor(DLBTMP[0].BLC))<0.00005)
				{
					strs1 = DLBTMP[0].BGH;
					XCLC = DLBTMP[0].BLC;
				}
				else
				{
					strs1 = DLBTMP[0].EGH;
					XCLC = DLBTMP[0].BLC + DLBTMP[0].DL;
				}
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				break;
			}
		}
		else if (iDL < NDL)
		{
			if (TYLC < LCB)
			{
				XCLC = DLBTMP[iDL].BLC - LCB + TYLC;
				strs1 = DLBTMP[iDL-1].EGH;
				strs2 = DLBTMP[iDL].BGH;
				strs1.TrimLeft();
				strs1.TrimRight();
				strs1.MakeUpper();
				strs2.TrimLeft();
				strs2.TrimRight();
				strs2.MakeUpper();
				if (strs1 == strs2)
					_tcscpy(GH,strs1);
				else
				{
					if (DLBTMP[iDL-1].DL < 0.00001 && XCLC < DLBTMP[iDL-1].BLC)//in 前长链
						_tcscpy(GH,strs1);							
					else if  (DLBTMP[iDL].DL < 0.00001 && XCLC < DLBTMP[iDL].BLC)//in 后长链
						_tcscpy(GH,strs2);
					else//前后都不是长链
					{
						if (2.0*(LCB-TYLC) > DLBTMP[iDL].BLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL)
							_tcscpy(GH,strs1);
						else
							_tcscpy(GH,strs2);
					}
				}
				break;
			}
			else if (fabs(TYLC - LCB) < 0.00005)//==
			{
				if (fabs(DLBTMP[iDL].BLC-floor(DLBTMP[iDL].BLC))<0.00005)
				{
					strs1 = DLBTMP[iDL].BGH;
					XCLC = DLBTMP[iDL].BLC;
				}
				else
				{
					strs1 = DLBTMP[iDL].EGH;
					XCLC = DLBTMP[iDL].BLC + DLBTMP[iDL].DL;
				}
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				break;
			}
		}
	}
	if (iDL == NDL)
	{
		XCLC = DLBTMP[iDL-1].BLC + DLBTMP[iDL-1].DL + TYLC - LCB;
		_tcscpy(GH,DLBTMP[iDL-1].EGH);
	}
	return XCLC;
}
//  由断链数据表 求某现场里程的统一里程
double BAS_DRAW_FUN_RAIL::TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] )
{   
	//  由断链数据表 求某现场里程的统一里程
	double TLC = 0.0;
	double TLCtmp = 0.0;
	int iDL=0;
	//起点里程
	double XLC;
	ACHAR GH[10];
	split_ckml(ckml,&XLC,GH);
	TLC=XLC;
	if (NDL<3)
	{
		return TLC;
	}
	CString strstr1,strstr2;
	short int numb = 0;//统计计算的连续里程的个数2005-08-26
	strstr1 = GH;
	strstr1.TrimLeft();
	strstr1.TrimRight();
	strstr1.MakeLower();
	double continueLC = DLBTMP[0].BLC + DLBTMP[0].DL;//断链的统一里程
	for (iDL = 1; iDL < NDL; iDL++)
	{
		if ((DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL-0.00005 < XLC) && (XLC < DLBTMP[iDL].BLC+0.00005))
		{
			strstr2 = DLBTMP[iDL-1].EGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if(strstr1 == strstr2)
			{
				TLC = continueLC + XLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL;
				numb++;
			}
			else
			{
				strstr2 = DLBTMP[iDL].BGH;
				strstr2.TrimLeft();
				strstr2.TrimRight();
				strstr2.MakeLower();
				if(strstr1 == strstr2)
				{
					TLC = continueLC + XLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL;
					numb++;
					strstr2.Format(L"第%d个断链的断后里程冠号%s与第%d个断链的断前里程冠号%s不一致,\n\n他将导致%s的计算不可靠",
						iDL-1, DLBTMP[iDL-1].EGH, iDL, DLBTMP[iDL].BGH, ckml);
					AfxMessageBox(strstr2);
				}
			}
		}
		continueLC =continueLC + DLBTMP[iDL].BLC - DLBTMP[iDL-1].DL - DLBTMP[iDL-1].BLC;
	}
	if (numb == 0)
	{
		if (XLC<DLBTMP[0].BLC + 0.00005)//在起点之外
		{
			strstr2 = DLBTMP[0].BGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLC = DLBTMP[0].DL + XLC;
				numb++;
			}
		}
		else if (XLC>DLBTMP[iDL-1].BLC-0.005 && XLC<DLBTMP[iDL-1].BLC+0.005)
		{
			TLC = continueLC;
			return TLC;
		}
		else if (XLC > DLBTMP[iDL-1].BLC + DLBTMP[iDL-1].DL-0.00005)
		{
			strstr2 = DLBTMP[iDL-1].EGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLC = continueLC + XLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL;
				numb++;
			}
		}
	}
	strstr1 = ckml;
	if (numb > 1)
		//AfxMessageBox(L"系统发现" + strstr1 + "在线路上有多个位置，可能是断链表数据有问题");
			ads_printf(L"系统发现 %s 在线路上有多个位置，可能是断链表数据有问题", strstr1);
	else if (numb == 0)
		//AfxMessageBox(L"系统发现" + strstr1 + "在线路上没有其位置，可能是短链或断链表数据有问题");
		ads_printf(L"系统发现 %s 在线路上没有其位置，可能是短链或断链表数据有问题", strstr1);
	return TLC;
}

//plh05.13
double BAS_DRAW_FUN_RAIL::PROJ_ML(double array2[][10],int trow2,
								  double xout, double yout, double xon, double yon)
{
	double x1,y1,x2,y2,x3,y3,x4,y4,s1,s2,s,ZJ,Alf1,Alf2,Alf,Xo,Yo,q,x,y,ZJ1,ZJ2,R;
	int i,kk[100] ,ii,xynum=0;
	double LorR,X1[100],Y1[100],X2[100],Y2[100];      

	double lc=array2[0][4];
	s = -1.0;
	//起点向前延长1000m
	x2 = array2[0][1] + 1000*cos(rad(array2[0][3])+pi);
	y2 = array2[0][2] + 1000*sin(rad(array2[0][3])+pi);
	for(i=1; i<=trow2;i++)
	{
		x1 = x2;
		y1 = y2;
		x2 = array2[i][8];
		y2 = array2[i][9];

		q = array2[i][0];
		// 	ads_printf(L"##x1=%lf,y1=%lf ,x2=%lf,y2=%lf,q=%lf\n",x1,y1,x2,y2,q);
		LorR = 1.0;
		if(q > 1.1 && q < 7.0)
			LorR = array2[i][4];
		//		ads_printf(L"%lf\n",array2[i][4]);
		if(i>1)
			Alf1 = array2[i-1][5];
		else
			Alf1 = rad(array2[i-1][3]);

		Alf2 =  array2[i][5] ;
		if(i==trow2)//终点向后延长1000m
		{
			//	ads_printf(L"Alf1=%lf Alf2=%lf\n",Alf1,Alf2);
			x2 = x2 + 1000*cos(Alf1);
			y2 = y2 + 1000*sin(Alf1);
		}
		x3 = x1 + 1000.0*cos(0.5*LorR*pi+Alf1);
		y3 = y1 + 1000.0*sin(0.5*LorR*pi+Alf1);

		x4 = x2 + 1000.0*cos(0.5*LorR*pi+Alf2);
		y4 = y2 + 1000.0*sin(0.5*LorR*pi+Alf2);

		s1 = GetSide(x3,y3,x1,y1,xon,yon);
		s2 = GetSide(x4,y4,x2,y2,xon,yon);

		if(s1*s2 < 0.0001 || fabs(s1)<0.00001 || fabs(s2) <0.00001)
		{
			kk[xynum] = i;
			X1[xynum]=x1;
			Y1[xynum]=y1;
			X2[xynum]=x2;
			Y2[xynum]=y2;
			xynum++;//搜索出所有可能线元
		}
	}
	if(xynum==0)
	{
		ads_printf(L"指定点不在线路上(1)!\n");
		return -1.0;
	}
	double cml[100];
	for(i=0; i<xynum; i++)
	{
		x1=X1[i];
		y1=Y1[i];
		x2=X2[i];
		y2=Y2[i];

		//搜索范围限制在kk-1-------kk段现元上
		if(kk[i]>0)
		{
			q =array2[kk[i]-1][0];
			LorR =  array2[kk[i]-1][4];//?左右转
		}
		else
		{
			q = array2[0][0];
			LorR = 0.0;//?左右转
		}
		//ads_printf(L"kk=%d,lc=%lf q=%lf %lf \n",kk,array2[kk][6],q,array2[kk-1][6]);
		//	LorR = 1;
		//	if(fabs(q) > 0.1)

		if(fabs(LorR)<0.001)
			LorR = 1.0;

		if(kk[i]>1)
			Alf1 = array2[kk[i]-1][5];
		else
			Alf1 = rad(array2[kk[i]-1][3]);

		Alf2 = array2[kk[i]][5];

		if(kk[i]>1)
			lc = array2[kk[i]-1][6];//当前线元起始里程

		if(q < 1.1)//直线
		{
			s1 = sqrt((xout-x1)*(xout-x1) + (yout-y1)*(yout-y1));
			xyddaa(x1,y1,xout,yout,&Alf2);
			ZJ = ZXJ(Alf2 , Alf1);
			// 	ads_printf(L"ZJ=%lf Alf2=%lf Ald1=%lf\n",ZJ,Alf2,Alf1);
			cml[i] = lc + s1*cos(ZJ);
		}
		else if(q<2.1)//圆
		{
			R = array2[kk[i]-1][1];

			Xo = array2[kk[i]-1][8] + R*cos(array2[kk[i]-1][5]+LorR*0.5*pi) ;
			Yo = array2[kk[i]-1][9] + R*sin(array2[kk[i]-1][5]+LorR*0.5*pi);


			s = 0.5*(array2[kk[i]][6] - lc);
			x = x1 + s*cos(Alf1);
			y = y1 + s*sin(Alf1);
			xyddaa( Xo,Yo,xout,yout,&Alf);

			s1 = GetSide(x1,y1,Xo,Yo,x,y);
			s2 = GetSide(x1,y1,Xo,Yo,xout,yout);
			if(s1*s2 < 0.001|| fabs(s1)<0.00001 || fabs(s2)<0.00001) 
				Alf = Alf + pi;
			x = Xo + R*cos(Alf);
			y = Yo + R*sin(Alf);

			cml[i] = Circle_XY_to_ML(x,y,array2,kk[i]-1);
		}
		else//缓和曲线
		{
			xyddaa( xout,yout,x1,y1,&Alf1);
			xyddaa( xout,yout,x2,y2,&Alf2);

			if(kk[i]>1)
				ZJ1 = ZXJ(Alf1 , array2[(kk[i])-1][5]);
			else
				ZJ1 = ZXJ(Alf1 , rad(array2[(kk[i])-1][3]));

			ZJ2 = ZXJ(Alf2 , array2[(kk[i])][5]);
			if(fabs(ZJ1) > 0.5*pi)
				ii = 1;
			else
				ii = -1;

			s1 = lc;
			s2 = array2[(kk[i])][6];
L20:
			s = 0.5*(s1+s2);
			cml[i] = s;
			struct xlpoint PZ;
			PZ.lc = s;
			xlpoint_pz(array2,trow2,&PZ);
			xyddaa(xout,yout,PZ.x,PZ.y,&Alf);
			ZJ = ZXJ(Alf , PZ.a);
			if((ii==1 && fabs(ZJ)>0.5*pi )|| (ii==-1 && fabs(ZJ)<0.5*pi))
				s1 = s;
			else
				s2 = s;
			if(fabs(s2-s1) > 0.00001)
				goto L20;
		}
	}
	//取出距离xon,yon的点
	double MinDis=100000000000000.0,dist;
	for(i=0; i<xynum; i++)
	{
		struct xlpoint PZ;
		PZ.lc = cml[i];
		xlpoint_pz(array2,trow2,&PZ);
		dist=sqrt((PZ.x-xon)*(PZ.x-xon)+(PZ.y-yon)*(PZ.y-yon));
		if(MinDis>dist)
		{
			s = cml[i];
			MinDis = dist;
		}
	}
	return s;
}
//---------------------------------------------------------------------
double  BAS_DRAW_FUN_RAIL::GetSide(double Xo,double Yo,double x1,double y1,double x,double y)
	//左侧-，右+
{
	double s;
	s = (x-Xo)*(y1-Yo)-(x1-Xo)*(y-Yo);
	return s;
}

//---------------------------------------------------------------------
double   BAS_DRAW_FUN_RAIL::ZXJ(double s2,double s1)
{
	double s;
	s=s2-s1;
	if(fabs(s)>pi)
	{
		if(s<0.0) //偏角小于-PI
		{
			s=2.0*pi+s;
		}
		else      //偏角大于PI
		{
			s=s-2.0*pi;
		}
	}
	return(s);
}

//两条直线方位角分别为Fwj1，Fwj2。计算其夹角
double BAS_DRAW_FUN_RAIL::CalJiaJiao(double Fwj1, double Fwj2)
{
	double JiaJiao = fabs(Fwj1-Fwj2);
	if (JiaJiao>pi/2&&JiaJiao<=pi)
	{JiaJiao=pi-JiaJiao;}
	else if (JiaJiao>pi&&JiaJiao<=3*pi/2)
	{
		JiaJiao=JiaJiao-pi;
	}
	else if (JiaJiao>3*pi/2&&JiaJiao<=2*pi)
	{
		JiaJiao=2*pi-JiaJiao;
	}

	return JiaJiao;
}
double  BAS_DRAW_FUN_RAIL::XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int trow2)
{
	double x1,y1,x2,y2,s1,s2,s,ss1,ss2;
	int i,kk;

	double lc=array2[0][4];
	x2 = array2[0][1];
	y2 = array2[0][2];
	for(i=1; i<=trow2;i++)
	{
		x1 = x2;
		y1 = y2;
		x2 = array2[i][8] ;
		y2 = array2[i][9] ;
		s1 = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);

		s = (x-x1)*(x-x1)+(y-y1)*(y-y1);
		if((x-x1)*(x2-x)>=0.0 && (y-y1)*(y2-y)>=0.0 || 
			s1>=s)
		{
			kk = i;
			goto L10;
		}
	}
	ads_printf(L"指定点不在线路上(2)!\n");
	return -1.0;
L10:
	if(kk>1)
		lc = array2[kk-1][6];

	s1 = lc;
	s2 =array2[kk][6];

	ss1 = sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
	ss2 = sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
	s = (s1*ss2+s2*ss1)/(ss1+ss2);

	return s;	
}

double  BAS_DRAW_FUN_RAIL::Circle_XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int kk)
	//x,y为给定圆上一点，kk为圆弧线元(不考虑回头)
{
	double x1,y1,x2,y2,s1,s2,s,ss1;

	s1 = array2[kk][6];
	s2 = array2[kk+1][6];
	double R = array2[kk][1];
	x1 = array2[kk][8] , y1 = array2[kk][9] ;
	x2 = array2[kk+1][8] , y2 = array2[kk+1][9] ;
	ss1 = sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
	s = s1 + R*2*asin(0.5*ss1/R);
	// ss2 = sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
	// s = (s1*ss2+s2*ss1)/(ss1+ss2);
	return s;	
}

/*----------------------------------------------------------*
split the string mileage SML[] into two parts, e.g.
CK 153 +550.00, after calling function split_ckml, you
can get ML=153550.0, ck=CK
*----------------------------------------------------------*/
void BAS_DRAW_FUN_RAIL::split_ckml(ACHAR str[], double *ml, ACHAR  *ck)
{
	int i,j, len, str_len, lplus;
	ACHAR  cml[17];
	double tml;
	*ml = 0.0;
	_tcscpy(ck,L"K");

	/*                CIK   153 + 342.53
	.       .
	len     lplus                               */
	ck[0]='\0';
	len = F_num(str, 'K');
	lplus = F_num(str, '+');
	str_len=_tcslen(str);
	for (i=0; i<len; i++)
		ck[i]=str[i];
	ck[len]='\0';


	if (lplus == 0)  {
		for (i=len,j=0; i<str_len; i++,j++)
			cml[j]=str[i];
		cml[j]='\0';
	}
	else {
		j=0;
		for (i=len; i<lplus-1; i++)
			if (str[i] != ' ')
				cml[j++]=str[i];

		for (i=lplus; i<str_len; i++)
			if (str[i] != ' ')
				cml[j++]=str[i];
		cml[j]='\0';
	};
	tml =  _wtof(cml);
	tml *= 1000.0;
	tml =  floor(tml);
	*ml =  tml/1000.0;
}
/*----------------------------------------------------------*
* Name:  determain the location of ACHARacter C in string
variable STR[], if found, return subscript of C in STR[],
otherwise, return 0
*----------------------------------------------------------*/
int BAS_DRAW_FUN_RAIL::F_num(ACHAR *str, ACHAR c)
{
	int i, len, num=0, smark, c1;

	smark = toupper(c);//将小写字母转化成大写
	len = _tcslen(str);
	for(i=len-1; i>=0; i--)  
	{
		c1 = toupper(str[i]);
		if (c1 == smark)
		{num=i+1; break;}
	}
	return(num);
}
/*----------------------------------------------------------*
put "+" to mileage (e.g. CK153500.0 --> CK153+500.0)
put the sign IN_CH,
*----------------------------------------------------------*/
void BAS_DRAW_FUN_RAIL::putplus(ACHAR str[], ACHAR in_ch, ACHAR s_ch, int place)
{
	int slen, splace, i, gone=1;

	slen = _tcslen(str);
	for (i=0; i<slen; i++)
		if (toupper(str[i]) == toupper(in_ch)) gone=0;

	if (gone) {
		for(i=0; i<slen; i++)
			if (str[i]==s_ch) { splace=i+1; break;}
			for (i=slen; i>=splace+place; i--)
				str[i]=str[i-1];
			str[i]=in_ch;
			str[slen+1]='\0';
	};
}
AcDbObjectId  BAS_DRAW_FUN_RAIL::make2dline(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,ACHAR LayerName[])
{
	//if ((startWid-999)<0.1)//X,Y转换，线路求交时用
	//{
	//	for (int i=0;i<ptarray.logicalLength();i++)
	//	{
	//		double t=ptarray[i].x;
	//		ptarray[i].x=ptarray[i].y;
	//		ptarray[i].y=t;
	//	}
	//	startWid=0.1;
	//}
	AcDb2dPolyline *pLine	= new AcDb2dPolyline(AcDb::k2dSimplePoly,ptarray,0,Adesk::kFalse,startWid,EndWid);
	pLine->setColorIndex(icolor) ;
	AcDbObjectId  LId ,LayerId;
	if(LayerName)
	{
		LayerId = CreateLayer(LayerName);
		pLine->setLayer(LayerId);
	}
	LId=AddEntityToDbs(pLine) ;
	return LId ;
}

void BAS_DRAW_FUN_RAIL::atodmss(double alpha, int &d, ACHAR mm[4], ACHAR ss[4])
{
	int m, s;
	d = floor(fabs(alpha));
	m = floor((fabs(alpha)-d)*100.0+0.0001);
	s = floor(((fabs(alpha)-d)*100.0-m)*100.0+0.0001);

	if (m <10)
		_stprintf (mm, L"0%d'\0", m);
	else
		_stprintf (mm, L"%d'\0", m);
	if (s <10)
		_stprintf (ss, L"0%d\"\0", s);
	else
		_stprintf (ss, L"%d\"\0", s);
}
void BAS_DRAW_FUN_RAIL::NoteBG(AcGePoint3d Opt,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,ACHAR layername[])
{
	DrawBGTri(Opt,sfactor,iColor,layername);

	AcGePoint3d origin;
	origin.x = Opt.x;
	origin.y = Opt.y + 0.25*sfactor;
	origin.z = Opt.z;
	maketext(origin,Str,0,texth,itextColor,mode,layername);	
}

void BAS_DRAW_FUN_RAIL::DrawBGTri(AcGePoint3d Opt,double sfactor,int iColor,ACHAR LayerName[])
{
	AcGePoint3d pt[4];

	pt[0] = Opt;
	pt[1].x = Opt.x - 0.1*sfactor;
	pt[1].y = Opt.y + 0.15*sfactor;
	pt[2].x = Opt.x + 0.1*sfactor;
	pt[2].y = Opt.y + 0.15*sfactor;
	pt[3] = pt[0];

	AcDbSolid *triangle = new AcDbSolid(pt[0],pt[1],pt[2],pt[3]);
	AcDbObjectId LayerId;
	if(LayerName)
	{
		LayerId = CreateLayer(LayerName);
		triangle->setLayer(LayerId);
	}
	triangle->setColorIndex(iColor);
	AddEntityToDbs(triangle) ;
}
void BAS_DRAW_FUN_RAIL::dlayer(ACHAR lname[])//删除层
{
	struct resbuf eb;
	ads_name sname;
	ACHAR sbuf[80];
	_tcscpy(sbuf,lname);

	eb.restype = 8;   /* Layer name  */
	eb.resval.rstring=sbuf;
	eb.rbnext = NULL;   /* No other properties  */
	/* Retrieve all entities on layer LNAME */
	if ((acedSSGet(L"X", NULL, NULL, &eb, sname)) == RTNORM)
	{
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"Erase", RTPICKS, sname, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"Erase", RTPICKS, sname, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010
	}
		
	else
	{
		//acutPrintf(L"\nfail to run ads_ssget function.");
		return ;
	}

	if (ads_ssfree(sname) != RTNORM)
	{
		//acutPrintf(L"\nsname free error");
		return ;
	}

}
double BAS_DRAW_FUN_RAIL::get_l0(double Design_V,int KNDD,double SR, GuiFanCanShu mGuiFanCS)
{
	/*
	double l0 =0.0; 
	if ( Design_V == 140 )
	{ 
	if ( KNDD == 0 )
	{	  if      ( SR >=10000 ) l0 =  30;
	else if ( SR >= 8000 ) l0 =  40;
	else if ( SR >= 6000 ) l0 =  50;
	else if ( SR >= 5000 ) l0 =  60;
	else if ( SR >= 4000 ) l0 =  60;
	else if ( SR >= 3000 ) l0 =  70;
	else if ( SR >= 2500 ) l0 =  80;
	else if ( SR >= 2000 ) l0 =  90;
	else if ( SR >= 1800 ) l0 = 100;
	else if ( SR >= 1600 ) l0 = 110;
	else if ( SR >= 1400 ) l0 = 130;
	else if ( SR >= 1200 ) l0 = 150 ;
	}
	else
	{
	if      ( SR >=10000 ) l0 =  20;
	else if ( SR >= 8000 ) l0 =  20;
	else if ( SR >= 6000 ) l0 =  30;
	else if ( SR >= 5000 ) l0 =  40;
	else if ( SR >= 4000 ) l0 =  40;
	else if ( SR >= 3000 ) l0 =  50;
	else if ( SR >= 2500 ) l0 =  70;
	else if ( SR >= 2000 ) l0 =  80;
	else if ( SR >= 1800 ) l0 =  80;
	else if ( SR >= 1600 ) l0 = 100;
	else if ( SR >= 1400 ) l0 = 110;		  
	else if ( SR >= 1200 ) l0 = 130 ;
	};
	}
	else if ( Design_V ==120)
	{ 
	if ( KNDD == 0 )
	{
	if      ( SR >=10000 ) l0 =  20;
	else if ( SR >= 8000 ) l0 =  30;		  
	else if ( SR >= 6000 ) l0 =  30;		  
	else if ( SR >= 5000 ) l0 =  40;		  
	else if ( SR >= 4000 ) l0 =  50;		  
	else if ( SR >= 3000 ) l0 =  50;		  
	else if ( SR >= 2500 ) l0 =  60;		  
	else if ( SR >= 2000 ) l0 =  60;		  
	else if ( SR >= 1800 ) l0 =  70;		  
	else if ( SR >= 1600 ) l0 =  70;		  
	else if ( SR >= 1400 ) l0 =  80;		  
	else if ( SR >= 1200 ) l0 =  90;		  
	else if ( SR >= 1000 ) l0 = 120;		  
	if      ( SR >=  800 ) l0 = 150 ;		  
	}
	else
	{	
	if      ( SR >=10000 ) l0 =  20;
	else if ( SR >= 8000 ) l0 =  20;
	else if ( SR >= 6000 ) l0 =  20;
	else if ( SR >= 5000 ) l0 =  30;
	else if ( SR >= 4000 ) l0 =  30;
	else if ( SR >= 3000 ) l0 =  40;
	else if ( SR >= 2500 ) l0 =  40;
	else if ( SR >= 2000 ) l0 =  50;
	else if ( SR >= 1800 ) l0 =  60;
	else if ( SR >= 1600 ) l0 =  60;
	else if ( SR >= 1400 ) l0 =  70;
	else if ( SR >= 1200 ) l0 =  80;
	else if ( SR >= 1000 ) l0 = 100;
	else if ( SR >=  800 ) l0 = 130;
	};
	}
	else if (Design_V == 100 )
	{
	if ( KNDD == 0 )
	{
	if ( SR >=10000 ) l0 =  20;
	else if ( SR >= 8000 ) l0 =  20;		  
	else if ( SR >= 6000 ) l0 =  20;		  
	else if ( SR >= 5000 ) l0 =  20;		  
	else if ( SR >= 4000 ) l0 =  30;		  
	else if ( SR >= 3000 ) l0 =  40;		  
	else if ( SR >= 2500 ) l0 =  40;		  
	else if ( SR >= 2000 ) l0 =  50;		  
	else if ( SR >= 1800 ) l0 =  50;		  
	else if ( SR >= 1600 ) l0 =  50;		  
	else if ( SR >= 1400 ) l0 =  60;		  
	else if ( SR >= 1200 ) l0 =  60;	
	else if ( SR >= 1000 ) l0 =  70;		  
	else if ( SR >=  800 ) l0 =  80;
	else if ( SR >=  700 ) l0 = 100;
	else if ( SR >=  600 ) l0 = 120;
	else if ( SR >=  550 ) l0 = 130 ;
	}
	else
	{
	if      ( SR >=10000 ) l0 =  20;
	else if ( SR >= 8000 ) l0 =  20;
	else if ( SR >= 6000 ) l0 =  20;
	else if ( SR >= 5000 ) l0 =  20;
	else if ( SR >= 4000 ) l0 =  20;
	else if ( SR >= 3000 ) l0 =  20;
	else if ( SR >= 2500 ) l0 =  30;		  
	else if ( SR >= 2000 ) l0 =  40;		  
	else if ( SR >= 1800 ) l0 =  40;		  
	else if ( SR >= 1600 ) l0 =  40;		  
	else if ( SR >= 1400 ) l0 =  40;		  
	else if ( SR >= 1200 ) l0 =  50;		  
	else if ( SR >= 1000 ) l0 =  60;		  
	else if ( SR >=  800 ) l0 =  70;		  
	else if ( SR >=  700 ) l0 =  90;		  
	else if ( SR >=  600 ) l0 = 100;		  
	else if      ( SR >=  550 ) l0 = 110 ;
	};
	}
	else if (Design_V == 80)
	{
	if ( KNDD == 0 )
	{
	if      ( SR >=10000 ) l0 =  20;
	else if ( SR >= 8000 ) l0 =  20;
	else if ( SR >= 6000 ) l0 =  20;
	else if ( SR >= 5000 ) l0 =  20;
	else if ( SR >= 4000 ) l0 =  20;
	else if ( SR >= 3000 ) l0 =  20;
	else if ( SR >= 2500 ) l0 =  30;
	else if ( SR >= 2000 ) l0 =  30;
	else if ( SR >= 1800 ) l0 =  30;
	else if ( SR >= 1600 ) l0 =  40;
	else if ( SR >= 1400 ) l0 =  40;
	else if ( SR >= 1200 ) l0 =  40;
	else if ( SR >= 1000 ) l0 =  40;
	else if ( SR >=  800 ) l0 =  50;
	else if ( SR >=  700 ) l0 =  50;
	else if ( SR >=  600 ) l0 =  60;
	else if ( SR >=  550 ) l0 =  60;
	else if ( SR >=  500 ) l0 =  60;		  
	else if ( SR >=  450 ) l0 =  80;		  
	else if ( SR >=  400 ) l0 =  90;		  
	else if ( SR >=  350 ) l0 = 100 ;		  
	else  l0 = 0.0;         
	}
	else
	{	
	if      ( SR >=10000 ) l0 =  20;
	else if ( SR >= 8000 ) l0 =  20;
	else if ( SR >= 6000 ) l0 =  20;
	else if ( SR >= 5000 ) l0 =  20;
	else if ( SR >= 4000 ) l0 =  20;
	else if ( SR >= 3000 ) l0 =  20;
	else if ( SR >= 2500 ) l0 =  20;
	else if ( SR >= 2000 ) l0 =  20;
	else if ( SR >= 1800 ) l0 =  20;
	else if ( SR >= 1600 ) l0 =  20;
	else if ( SR >= 1400 ) l0 =  20;
	else if ( SR >= 1200 ) l0 =  30;
	else if ( SR >= 1000 ) l0 =  30;
	else if ( SR >=  800 ) l0 =  40;
	else if ( SR >=  700 ) l0 =  40;		  
	else if ( SR >=  600 ) l0 =  50;		  
	else if ( SR >=  550 ) l0 =  50;		  
	else if ( SR >=  500 ) l0 =  60;		  
	else if ( SR >=  450 ) l0 =  70;		  
	else if ( SR >=  400 ) l0 =  80;		  
	else if      ( SR >=  350 ) l0 =  90 ;
	};
	};
	return l0;
	*/
	int i=mGuiFanCS.RXiangShu-1;
	while(i>=0 && mGuiFanCS.R[i]<SR-1)
		i--;

	if(i==mGuiFanCS.RXiangShu-1)
		return mGuiFanCS.L[i];
	else if(i==0)
	{
		if(SR>mGuiFanCS.R[0])
			return mGuiFanCS.L[0];
		else
			i++;
	}  

	double l0;
	int kk;
	l0 = mGuiFanCS.L[i-1]+ (SR-mGuiFanCS.R[i-1])*(mGuiFanCS.L[i]-mGuiFanCS.L[i-1])/(mGuiFanCS.R[i]-mGuiFanCS.R[i-1]);
	kk = int(l0/10.0 + 0.99);
	l0 = kk * 10.0;
	return  l0;		

}

//     功能：求直线段与直线段的交点 （笛卡儿坐标）
// 变量说明：P1、P2 定义直线的两点；P3、P4 定义另一直线的两点；
//   返回值：相交 99，PP为交点坐标; 平行-99; 不相交-89；

int BAS_DRAW_FUN_RAIL::crossll(ads_point p1, ads_point p2, ads_point p3, ads_point p4, ads_point pp)
{  // 直线段与直线段  
	double a1,a2,b1,b2,c1,c2;
	int tmp=0,bz=99;
	a1=p1[Y]-p2[Y];  b1=p2[0]-p1[0];  c1=p1[1]*(0-b1)-p1[0]*a1;
	a2=p3[Y]-p4[Y];  b2=p4[0]-p3[0];  c2=p3[1]*(0-b2)-p3[0]*a2;
	if ((a1*b2-a2*b1)==0) //平行
	{ bz=-99; pp[0]=-1000; pp[1]=-1000; }
	else
	{
		pp[1]=(a1*c2-a2*c1)/(b1*a2-a1*b2);
		if (p1[1]==p2[1])
		{  pp[0]=(-1)*(b2*pp[1]+c2)/a2; }
		else { pp[0]=(-1)*(b1*pp[1]+c1)/a1; };
		if (p1[1]!=p2[1])
		{  if ((p1[1]-pp[1])*(pp[1]-p2[1])<0) tmp=1; };
		if (p1[0]!=p2[0])
		{  if ((p1[0]-pp[0])*(pp[0]-p2[0])<0) tmp=1; };
		if (p3[1]!=p4[1])
		{  if ((p3[1]-pp[1])*(pp[1]-p4[1])<0) tmp=1; };
		if (p3[0]!=p4[0])
		{  if ((p3[0]-pp[0])*(pp[0]-p4[0])<0) tmp=1; };

		if (tmp==1)
		{ bz=-89;  pp[0]=-1000; pp[1]=-1000; };
	};
	return bz;
}
void BAS_DRAW_FUN_RAIL::dmltockml(CString GH,double dml,ACHAR ckml[80])
{
	double ys=fmod(dml,1000.0);
	if(fabs(ys-1000.0)<0.001)
	{
		_stprintf(ckml,L"%s%d+000",GH,int(dml/1000+0.01));
		return;
	}
	//if(GH!="")
	//{
	if(ys>=100.0)
		_stprintf(ckml,L"%s%d+%0.3lf",GH,int(dml/1000),ys);
	else if(ys>=10.0)
		_stprintf(ckml,L"%s%d+0%0.3lf",GH,int(dml/1000),ys);			
	else if(ys>0.0)
		_stprintf(ckml,L"%s%d+00%0.3lf",GH,int(dml/1000),ys);			
	else
		_stprintf(ckml,L"%s%d+000",GH,int(dml/1000));
	//}
	//else
	//{
	//	if(ys>=100.0)
	//		_stprintf(ckml,L"%s%d+%0.0lf",GH,int(dml/1000),ys);
	//	else if(ys>=10.0)
	//		_stprintf(ckml,L"%s%d+0%0.0lf",GH,int(dml/1000),ys);			
	//	else if(ys>0.0)
	//		_stprintf(ckml,L"%s%d+00%0.0lf",GH,int(dml/1000),ys);			
	//	else
	//		_stprintf(ckml,L"%s%d+000",GH,int(dml/1000));
	//}
}

void BAS_DRAW_FUN_RAIL::NullInStrto$(CString& text)
{
	int len,i;

	len = text.GetLength();
	i = text.Find(L" ");
	while(i < len && i >=0)
	{
		text.SetAt(i,'$');
		i = text.Find(L" ");
	}
	if(len==0)
		text="$";	
}

void BAS_DRAW_FUN_RAIL::InStr$toNull()
{
	int len,i;

	len = Cworkdir.GetLength();
	i = Cworkdir.Find('$');
	while(i < len && i >= 0)
	{
		Cworkdir.SetAt(i,' ');
		i = Cworkdir.Find('$');
	}	
}

int BAS_DRAW_FUN_RAIL::ReadWorkdir()
{
	ACHAR fname[256];

	_stprintf(fname,L"%s\\workdir",SysPath);

	FILE *fpr=_wfopen(fname,L"r");
	if(!fpr)
	{
		//	AfxMessageBox(L"您还没有设定项目名称!");
		Cworkdir="";
		return 0;
	}

	fwscanf(fpr,L"%s",fname);
	fclose(fpr);
	Cworkdir=fname;
	InStr$toNull();	

	Cworkdir.MakeUpper();
	CString ProFname;
	ProFname=Cworkdir+"\\project";

	fpr = _wfopen(ProFname,L"r");
	if(!fpr)
	{
		// 	AfxMessageBox(L"您还没有指定设计方案名称!");
		return 0;
	}
	fwscanf(fpr,L"%s",fname);
	fclose(fpr);
	Pro = fname;

	return 1;
}
/*
int BAS_DRAW_FUN::ReadWorkdir()
{
ACHAR fname[256];
FILE *fpr=_wfopen(L"c:\\workdir",L"r");
if(!fpr)
{
AfxMessageBox(L"您还没有设定项目名称!");
Cworkdir="";
return 0;
}

fwscanf(fpr,L"%s",fname);
fclose(fpr);
Cworkdir=fname;
InStr$toNull();	

Cworkdir.MakeUpper();
CString ProFname;
ProFname=Cworkdir+"\\project";

fpr = _wfopen(ProFname,L"r");
if(!fpr)
{
AfxMessageBox(L"您还没有指定设计方案名称!");
return 0;
}
fwscanf(fpr,L"%s",fname);
fclose(fpr);
Pro = fname;

return 1;
}*/

double BAS_DRAW_FUN_RAIL::get_xjj(double Design_V, GuiFanCanShu mGuiFanCS)
{
	double xjj;
	/*
	if (Design_V < 155)
	xjj = 4.0;
	else if(Design_V < 161)
	xjj = 4.2;
	else if(Design_V < 201)
	xjj = 4.4;
	else if(Design_V < 251)
	xjj = 4.6;
	else
	xjj = 5.0;*/

	xjj = mGuiFanCS.XianJianJu;	
	return xjj;
}

double BAS_DRAW_FUN_RAIL::get_XJJK(double Design_V,double SR, GuiFanCanShu mGuiFanCS)//线间距加宽
{
	int i=mGuiFanCS.RXiangShu-1;
	while(i>=0 && mGuiFanCS.R[i]<SR-1)
		i--;

	if(i==mGuiFanCS.RXiangShu-1)
		return mGuiFanCS.JiaKuan[i]/1000.0;
	else if(i<=0)
	{
		if(SR>mGuiFanCS.R[0])
			return mGuiFanCS.JiaKuan[0]/1000.0;
		else
			i++;
	}
	double jk;
	int kk;
	jk = mGuiFanCS.JiaKuan[i-1]+ (SR-mGuiFanCS.R[i-1])*(mGuiFanCS.JiaKuan[i]-mGuiFanCS.JiaKuan[i-1])/(mGuiFanCS.R[i]-mGuiFanCS.R[i-1]);
	kk = int(jk/5.0 + 0.99);
	jk = kk * 5.0/1000.0;
	return  jk;		
}

//double  BAS_DRAW_FUN::Search_EXPt(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3],double jiao,double ResPt[2])
////二线线元参数，线元个数，给定线外一点与角度定出直线找到该直线与二线交点ResPt及里程projml
//{
//    int sk = Search_XY(array2,trow2,Pt1);
//    double Spt[3],Ept[3];
//	double x1,y1,x2,y2;
//	Spt[0] = Pt1[0] , Spt[1] = Pt1[1] ,Spt[2]=Pt1[2];
//	Ept[0] = Pt1[0] + 1000*cos(jiao);
//	Ept[1] = Pt1[1] + 1000*sin(jiao);
//	int ek = Search_XY(array2,trow2,Ept);
//	int Kmin,Kmax;
//	Kmin = sk < ek ? sk : ek;
//	Kmax = sk < ek ? ek : sk;
//	
//	if(Kmin>1)
//		Kmin--;
//    if(Kmax<trow2-1)
//		Kmax++;
//	
//	double ProjMl=-1.0;
//	int inf=1;
//	
//	if(Kmin>1)
//		x2 = array2[Kmin-1][8] , y2 = array2[Kmin-1][9];
//    else
//	{
//		//起始线元延长100m
//		double fwj = array2[1][5]+pi;
//		x2 = array2[0][1] + 100.0*cos(fwj);
//		y2 = array2[0][2] + 100.0*sin(fwj);
//		
//		//x2 =  , y2 = array2[0][2];
//	}
//	double xmin,ymin,distmin,dist,ProjMlmin;//距Spt最近的点
//	distmin=100000;
//	//	for(int i=Kmin ; i<=Kmax; i++)//遍历找交点
//	for(int i=0 ; i<=trow2; i++)//遍历找交点
//	{
//		x1 = x2,y1= y2;
//		if(i<trow2-1)
//			x2 = array2[i+1][8],y2= array2[i+1][9];		   
//		else 
//			x2 = array2[trow2][8],y2= array2[trow2][9];
//		int res = IfOnXY(Spt[0],Spt[1],Ept[0],Ept[1],x1,y1,x2,y2);
//	
//		if(res==1)//在线元上
//		{
//			inf = CalXYjd(Spt,Ept,array2,trow2,i,ResPt,ProjMl);
//			//		 acutPrintf(L"inf=%d,ResPt[0]=%lf,ResPt[1]=%lf,ProjML=%lf\n",inf,ResPt[0],ResPt[1],ProjMl);
//			if(inf==0)//找到
//			{
//				//计算投影里程
//				dist=sqrt((Spt[0]-ResPt[0])*(Spt[0]-ResPt[0])+(Spt[1]-ResPt[1])*(Spt[1]-ResPt[1]));
//				if(distmin>dist)
//				{
//					xmin=ResPt[0];
//					ymin=ResPt[1];
//					distmin=dist;
//					ProjMlmin=ProjMl;
//				}			 
//			}
//		}
//	}
//	if(distmin<100000)
//	{
//		ResPt[0]=xmin;
//		ResPt[1]=ymin;
//		return ProjMlmin;		
//	}
//	else return -1.0;//找不到交点
//}
double
	BAS_DRAW_FUN_RAIL::Search_EXPt(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3],double jiao,double ResPt[2])
	//二线线元参数，线元个数，给定线外一点与角度定出直线找到该直线与二线交点ResPt及里程projml
{
	int iXY = 0;
	double dDst1 = 0.0;//试算点到法线的距离1、2
	double dDst2 = 0.0;
	double dtmpL1 =0.0;//累计曲线长1、2
	double dtmpL2 = 0.0;
	double dtmpPt1[2];
	double dtmpPt2[2];
	double dtmpAlf,dtmpP0,dtmpP1;
	double dtmpXjj = 0.0;//临时线间距
	double dtmpProjMlmin = 0.0;//里程
	double dminXjj = 1000000000.0;

	for (iXY=1; iXY < trow2; iXY++)
	{
		dDst1 = (array2[iXY][9]-Pt1[1])*cos(jiao)-(array2[iXY][8]-Pt1[0])*sin(jiao);
		dDst2 = (array2[iXY+1][9]-Pt1[1])*cos(jiao)-(array2[iXY+1][8]-Pt1[0])*sin(jiao);
		if (fabs(dDst1) < 0.000001)//在起点
		{
			dtmpXjj = sqrt((array2[iXY][8]-Pt1[0])*(array2[iXY][8]-Pt1[0])
				+(array2[iXY][9]-Pt1[1])*(array2[iXY][9]-Pt1[1]));
			if (dminXjj > dtmpXjj)
			{
				dminXjj = dtmpXjj;
				dtmpProjMlmin = array2[iXY][6];
				ResPt[0] = array2[iXY][8];
				ResPt[1] = array2[iXY][9];
			}
		}
		else if (fabs(dDst2) < 0.000001)//on the end point of this element
		{
			if (iXY == trow2 - 1)
			{
				dtmpXjj = sqrt((array2[trow2][8]-Pt1[0])*(array2[trow2][8]-Pt1[0])
					+(array2[trow2][9]-Pt1[1])*(array2[trow2][9]-Pt1[1]));
				if (dminXjj > dtmpXjj)
				{
					dminXjj = dtmpXjj;
					dtmpProjMlmin = array2[trow2][6];
					ResPt[0] = array2[trow2][8];
					ResPt[1] = array2[trow2][9];
				}
			}
		}
		else if (dDst1*dDst2 < 0.000001)//in this element
		{
			if (array2[iXY][0] < 1.5)//in the line
			{
				dtmpXjj = (Pt1[1]-array2[iXY][9])*cos(array2[iXY][5])-(Pt1[0]-array2[iXY][8])*sin(array2[iXY][5]);
				dtmpXjj = dtmpXjj/fabs(sin(jiao - array2[iXY][5]));
				if (dminXjj > fabs(dtmpXjj))
				{
					dminXjj = fabs(dtmpXjj);
					if (dtmpXjj < 0.00001)
					{
						ResPt[0] = Pt1[0] + dminXjj*cos(jiao);
						ResPt[1] = Pt1[1] + dminXjj*sin(jiao);
					}
					else
					{
						ResPt[0] = Pt1[0] + dminXjj*cos(jiao+pi);
						ResPt[1] = Pt1[1] + dminXjj*sin(jiao+pi);
					}
					dtmpProjMlmin = array2[iXY][6]+sqrt((ResPt[0]-array2[iXY][8])*(ResPt[0]-array2[iXY][8])
						+(ResPt[1]-array2[iXY][9])*(ResPt[1]-array2[iXY][9]));
				}
			}
			else if (array2[iXY][0] < 2.5)// in the circle
			{
				dtmpL1 = fabs(dDst1);
				dtmpL2 = fabs(dDst2);
				dDst1 =0.0, dDst2 = 0.0;
				while (fabs(array2[iXY][2] - dtmpL1 - dtmpL2) > 0.000004)
				{
					dtmpL1 = dtmpL1 + fabs(dDst1);
					dtmpL2 = dtmpL2 + fabs(dDst2);
					dtmpAlf = dtmpL1/array2[iXY][1];
					dtmpP0 = array2[iXY][1]*sin(dtmpAlf);
					dtmpP1 = array2[iXY][4]*array2[iXY][1]*(1.0 - cos(dtmpAlf));
					dtmpPt1[0] = array2[iXY][8] + dtmpP0*cos(array2[iXY][5]) - dtmpP1*sin(array2[iXY][5]);
					dtmpPt1[1] = array2[iXY][9] + dtmpP0*sin(array2[iXY][5]) + dtmpP1*cos(array2[iXY][5]);
					dDst1 = (dtmpPt1[1]-Pt1[1])*cos(jiao)-(dtmpPt1[0]-Pt1[0])*sin(jiao);

					dtmpAlf = (array2[iXY][2]-dtmpL2)/array2[iXY][1];
					dtmpP0 = array2[iXY][1]*sin(dtmpAlf);
					dtmpP1 = array2[iXY][4]*array2[iXY][1]*(1.0 - cos(dtmpAlf));
					dtmpPt2[0] = array2[iXY][8] + dtmpP0*cos(array2[iXY][5]) - dtmpP1*sin(array2[iXY][5]);
					dtmpPt2[1] = array2[iXY][9] + dtmpP0*sin(array2[iXY][5]) + dtmpP1*cos(array2[iXY][5]);
					dDst2 = (dtmpPt2[1]-Pt1[1])*cos(jiao)-(dtmpPt2[0]-Pt1[0])*sin(jiao);
				}
				dtmpXjj = sqrt((dtmpPt1[0]-Pt1[0])*(dtmpPt1[0]-Pt1[0])+(dtmpPt1[1]-Pt1[1])*(dtmpPt1[1]-Pt1[1]));
				if (dminXjj > dtmpXjj)
				{
					dminXjj = dtmpXjj;
					if (fabs(dDst1) < fabs(dDst2))
					{
						dtmpProjMlmin = array2[iXY][6] + dtmpL1;
						ResPt[0] = dtmpPt1[0];
						ResPt[1] = dtmpPt1[1];
					}
					else
					{
						dtmpProjMlmin = array2[iXY+1][6] - dtmpL2;
						ResPt[0] = dtmpPt2[0];
						ResPt[1] = dtmpPt2[1];
					}
				}
			}
			else if (array2[iXY][0] < 3.5)//in the first huanchang
			{
				dtmpL1 = fabs(dDst1);
				dtmpL2 = fabs(dDst2);
				dDst1 =0.0, dDst2 = 0.0;
				while (fabs(array2[iXY+1][6]-array2[iXY][6] - (dtmpL1+dtmpL2)) > 0.000004)
				{
					dtmpL1 = dtmpL1 + fabs(dDst1);
					dtmpL2 = dtmpL2 + fabs(dDst2);
					dtmpP0 =dtmpL1- dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*
						dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1] * dtmpL1/40.0;
					dtmpP1 =dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*dtmpL1/6.0-
						dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*
						dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*dtmpL1/336.0;
					dtmpP1 = array2[iXY][4]*dtmpP1;
					dtmpPt1[0] = array2[iXY][8] + dtmpP0*cos(array2[iXY][5]) - dtmpP1*sin(array2[iXY][5]);
					dtmpPt1[1] = array2[iXY][9] + dtmpP0*sin(array2[iXY][5]) + dtmpP1*cos(array2[iXY][5]);
					dDst1 = (dtmpPt1[1]-Pt1[1])*cos(jiao)-(dtmpPt1[0]-Pt1[0])*sin(jiao);

					dtmpAlf = array2[iXY+1][6]-array2[iXY][6] - dtmpL2;
					dtmpP0 =dtmpAlf- dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*
						dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1] * dtmpAlf/40.0;
					dtmpP1 =dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*dtmpAlf/6.0-
						dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*
						dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*dtmpAlf/336.0;
					dtmpP1 = array2[iXY][4]*dtmpP1;
					dtmpPt2[0] = array2[iXY][8] + dtmpP0*cos(array2[iXY][5]) - dtmpP1*sin(array2[iXY][5]);
					dtmpPt2[1] = array2[iXY][9] + dtmpP0*sin(array2[iXY][5]) + dtmpP1*cos(array2[iXY][5]);
					dDst2 = (dtmpPt2[1]-Pt1[1])*cos(jiao)-(dtmpPt2[0]-Pt1[0])*sin(jiao);
				}
				dtmpXjj = sqrt((dtmpPt1[0]-Pt1[0])*(dtmpPt1[0]-Pt1[0])+(dtmpPt1[1]-Pt1[1])*(dtmpPt1[1]-Pt1[1]));
				if (dminXjj > dtmpXjj)
				{
					dminXjj = dtmpXjj;
					if (fabs(dDst1) < fabs(dDst2))
					{
						dtmpProjMlmin = array2[iXY][6] + dtmpL1;
						ResPt[0] = dtmpPt1[0];
						ResPt[1] = dtmpPt1[1];
					}
					else
					{
						dtmpProjMlmin = array2[iXY+1][6] - dtmpL2;
						ResPt[0] = dtmpPt2[0];
						ResPt[1] = dtmpPt2[1];
					}
				}
			}
			else//in the second huanchang
			{
				dtmpL1 = fabs(dDst2);
				dtmpL2 = fabs(dDst1);
				dDst1 = 0.0, dDst2 = 0.0;
				while (fabs(array2[iXY+1][6]-array2[iXY][6] - (dtmpL1+dtmpL2)) > 0.000004)
				{
					dtmpL1 = dtmpL1 + fabs(dDst1);
					dtmpL2 = dtmpL2 + fabs(dDst2);
					dtmpP0 =dtmpL1- dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*
						dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1] * dtmpL1/40.0;
					dtmpP1 =dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*dtmpL1/6.0-
						dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*
						dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*dtmpL1/array2[iXY][1]*dtmpL1/336.0;
					dtmpP1 = -array2[iXY][4]*dtmpP1;
					dtmpPt1[0] = array2[iXY+1][8] + dtmpP0*cos(array2[iXY+1][5]+pi) - dtmpP1*sin(array2[iXY+1][5]+pi);
					dtmpPt1[1] = array2[iXY+1][9] + dtmpP0*sin(array2[iXY+1][5]+pi) + dtmpP1*cos(array2[iXY+1][5]+pi);
					dDst1 = (dtmpPt1[1]-Pt1[1])*cos(jiao)-(dtmpPt1[0]-Pt1[0])*sin(jiao);

					dtmpAlf = array2[iXY+1][6]-array2[iXY][6] - dtmpL2;
					dtmpP0 =dtmpAlf-dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*
						dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1] * dtmpAlf/40.0;
					dtmpP1 =dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*dtmpAlf/6.0-
						dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*
						dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*dtmpAlf/array2[iXY][1]*dtmpAlf/336.0;
					dtmpP1 = -array2[iXY][4]*dtmpP1;
					dtmpPt2[0] = array2[iXY+1][8] + dtmpP0*cos(array2[iXY+1][5]+pi) - dtmpP1*sin(array2[iXY+1][5]+pi);
					dtmpPt2[1] = array2[iXY+1][9] + dtmpP0*sin(array2[iXY+1][5]+pi) + dtmpP1*cos(array2[iXY+1][5]+pi);
					dDst2 = (dtmpPt2[1]-Pt1[1])*cos(jiao)-(dtmpPt2[0]-Pt1[0])*sin(jiao);
				}
				dtmpXjj = sqrt((dtmpPt1[0]-Pt1[0])*(dtmpPt1[0]-Pt1[0])+(dtmpPt1[1]-Pt1[1])*(dtmpPt1[1]-Pt1[1]));
				if (dminXjj > dtmpXjj)
				{
					dminXjj = dtmpXjj;
					if (fabs(dDst1) <= fabs(dDst2))
					{
						dtmpProjMlmin = array2[iXY][6] + dtmpL1;
						ResPt[0] = dtmpPt1[0];
						ResPt[1] = dtmpPt1[1];
					}
					else
					{
						dtmpProjMlmin = array2[iXY+1][6] - dtmpL2;
						ResPt[0] = dtmpPt2[0];
						ResPt[1] = dtmpPt2[1];
					}
				}
			}
		}
	}

	if (dminXjj < 100000000.0)
		return dtmpProjMlmin;
	else
	{
		ResPt[0] = Pt1[0];
		ResPt[1] = Pt1[1];
		return -1.0;//找不到交点
	}
}

int BAS_DRAW_FUN_RAIL::INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4)
{
	double A11,A12,A21,A22,B1,B2,D,D1,D2;
	int INF;
	double X1min,Y1min,X2min,Y2min,X1max,Y1max,X2max,Y2max;


	A11 = Y2-Y1;
	A12 = X1-X2;
	A21 = Y4-Y3;
	A22 = X3-X4;
	B1 = X1*Y2 - X2*Y1;
	B2 = X3*Y4 - X4*Y3;
	D = A11*A22-A21*A12;
	if(fabs(D)<1.0E-6)
		INF = -1;
	else
	{
		D1 = B1*A22-B2*A12;
		D2 = B2*A11-B1*A21;

		*x  = D1/D;
		*y  = D2/D;
		X1min=min(X1,X2)-0.00001;
		X1max=max(X1,X2)+0.00001;
		Y1min=min(Y1,Y2)-0.00001;
		Y1max=max(Y1,Y2)+0.00001;
		X2min=min(X3,X4)-0.00001;
		X2max=max(X3,X4)+0.00001;
		Y2min=min(Y3,Y4)-0.00001;
		Y2max=max(Y3,Y4)+0.00001;

		if(X1min<=*x && *x<=X1max && Y1min<=*y && *y<=Y1max
			&& X2min<=*x && *x<=X2max
			&& Y2min<=*y && *y<=Y2max)
			INF = 0;
		else
			INF = 1;
	}

	return INF; 
}    
int BAS_DRAW_FUN_RAIL::Search_XY(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3])//定位pt1点所在线元
{
	int i;
	double cml;      

	// ads_printf(L"1: %0.2lf %0.2lf!\n",Pt1[0],Pt1[1]);
	cml=PROJ_ML(array2,trow2,Pt1[0],Pt1[1],Pt1[0],Pt1[1]);//2004.5.14

	for(i=1;i<trow2;i++)
		if(cml>array2[i][6]-0.001&&cml<array2[i+1][6])
			return i;
	if(cml<array2[trow2][6]+1)
		return trow2;
	else
		ads_printf(L"无法定位点cml=%0.2lf %0.2lf %0.2lf对应线元!\n",cml,Pt1[0],Pt1[1]);
	return -1;
}

int BAS_DRAW_FUN_RAIL::CalXYjd(double Spt[3],double Ept[3],double array2[4*MAXJDNUM][10],int trow2,int kk, double ResPt[2],double &ProjMl)
{
	double q;
	double s1,lc,s;
	double XYspt[2],XYept[2];
	//第一段可能有问题
	q = array2[kk][0];
	if(kk>0)
		lc = array2[kk][6];//起始里程
	else
		lc = array2[0][4];

	if(kk>0)
	{
		XYspt[0] = array2[kk][8];
		XYspt[1] = array2[kk][9];
		XYept[0] = array2[kk+1][8];
		XYept[1] = array2[kk+1][9];
	}
	else
	{
		XYspt[0] = array2[kk][1];
		XYspt[1] = array2[kk][2];
		XYept[0] = array2[kk+1][8];
		XYept[1] = array2[kk+1][9];
	}


	if(q<1.1)//线元为直线
	{
		INTERS(&ResPt[0],&ResPt[1],Spt[0],Spt[1],Ept[0],Ept[1],XYspt[0],XYspt[1],XYept[0],XYept[1]);
		s1 = sqrt((ResPt[0]-XYspt[0])*(ResPt[0]-XYspt[0]) + (ResPt[1]-XYspt[1])*(ResPt[1]-XYspt[1]));
		ProjMl = lc + s1;
		//	ads_printf(L"直线: lc=%lf s1=%lf %lf %lf\n",lc,s1,ResPt[0],ResPt[1]);
		return 0;//找到
	}
	else if(q<2.1)//线元为圆
	{
		double R,Xo,Yo;
		double LorR =  array2[kk][4];//?左右转
		R = array2[kk][1];//2004518
		Xo = array2[kk][8] + R*cos(array2[kk][5]+LorR*0.5*pi) ;
		Yo = array2[kk][9] + R*sin(array2[kk][5]+LorR*0.5*pi);//圆心

		double x1,y1,x2,y2;
		if(fabs(Ept[0]-Spt[0])>0.1)
		{
			double K,b;
			K = (Ept[1]-Spt[1])/(Ept[0]-Spt[0]);
			b = Ept[1] - K*Ept[0];
			double A = 1+K*K;
			double B = 2*K*(b-Yo)-2*Xo;
			double C = Xo*Xo+(b-Yo)*(b-Yo)-R*R;


			x1 = -B/A/2.0 + sqrt((B/A)*(B/A)/4-C/A);
			y1 = K*x1+b;
			x2 = -B/A/2.0 - sqrt((B/A)*(B/A)/4-C/A);
			y2 = K*x2+b;
		}
		else//平行于Y轴
		{
			x1 = x2 = Spt[0];
			y1 = Yo+sqrt(R*R-(Spt[0]-Xo)*(Spt[0]-Xo));
			y2 = Yo-sqrt(R*R-(Spt[0]-Xo)*(Spt[0]-Xo));			
		}
		//ads_printf(L"1:x1=%lf y1=%lf\n",x1,y1);
		double ml1=PROJ_ML(array2,trow2,x1,y1,x1,y1);
		xlpoint PZ;
		PZ.lc = ml1;
		xlpoint_pz(array2,trow2,&PZ); 
		double dis1 = sqrt((x1-PZ.x)*(x1-PZ.x)+(y1-PZ.y)*(y1-PZ.y));
		//ads_printf(L"2:x2=%lf y2=%lf\n",x2,y2);	
		double ml2 = PROJ_ML(array2,trow2,x2,y2,x2,y2);
		PZ.lc = ml2;
		xlpoint_pz(array2,trow2,&PZ); 
		double dis2 = sqrt((x2-PZ.x)*(x2-PZ.x)+(y2-PZ.y)*(y2-PZ.y));
		//所求交点到线路距离,取距离线路最近的点
		//ads_printf(L"@@@@@@@@@dis1=%lf dis2=%lf ml1=%lf ml2=%lf\n",dis1,dis2,ml1,ml2);
		if(dis1<dis2)
		{
			ResPt[0] = x1 , 	ResPt[1] = y1;
			ProjMl = Circle_XY_to_ML(x1,y1,array2,kk);
			//	ads_printf(L"圆: x1=%lf y1=%lf ml=%lf\n",x1,y1,ProjMl);
			return 0;
		}
		else
		{
			ResPt[0] = x2 , 	ResPt[1] = y2;
			ProjMl = Circle_XY_to_ML(x2,y2,array2,kk);
			//		ads_printf(L"圆: x2=%lf y2=%lf ml=%lf %lf %lf\n",x2,y2,ProjMl,PZ.x,PZ.y);
			return 0;		
		}		
	}
	else //线元为缓和曲线
	{
		double s1,s2;
		struct xlpoint PZ;
		double pt2[2];
		double Alf1,Alf2;

		s1 = lc;
		s2 = array2[kk+1][6];
		Alf1 = Spt[2];
		while(fabs(s2-s1)>0.0001)
		{
			/*
			PZ.lc = s1;
			xlpoint_pz(array2,trow2,&PZ); 
			pt1[0] = PZ.x ,pt1[1] = PZ.y;*/


			s = 0.5*(s1+s2);		
			PZ.lc = s;
			xlpoint_pz(array2,trow2,&PZ); 
			pt2[0] = PZ.x ,pt2[1] = PZ.y;

			xyddaa(Spt[0],Spt[1],PZ.x,PZ.y,&Alf2);
			double zj = ZXJ(Alf1,Alf2);

			if(zj>0)//二线在右侧
			{
				if(zj>0.5*pi)
					s1 = s;
				else
					s2 = s;
			}
			else
			{
				if(fabs(zj)>0.5*pi)
					s1 = s;
				else
					s2 = s;
			}

			/*
			PZ.lc = s2;
			xlpoint_pz(array2,trow2,&PZ); 
			pt3[0] = PZ.x ,pt3[1] = PZ.y;*/


			/*

			if(INTERS(&pt[0],&pt[1],pt1[0],pt1[1],pt2[0],pt2[1],Spt[0],Spt[1],Ept[0],Ept[1])==0)//找到交点
			s2 = s;
			else if(INTERS(&pt[0],&pt[1],pt2[0],pt2[1],pt3[0],pt3[1],Spt[0],Spt[1],Ept[0],Ept[1])==0)
			s1 = s;
			else
			break;		  		  		*/

		}
		// if(fabs(s2-s1)<0.002)
		// {
		//  ads_printf(L"HH:s1=%lf s2=%lf\n",s1,s2);
		ProjMl = 0.5*(s1+s2);
		PZ.lc = ProjMl;
		xlpoint_pz(array2,trow2,&PZ); 
		ResPt[0] = PZ.x ,ResPt[1] = PZ.y;
		return 0;
		//  }
	}	
	return -1;
}
int BAS_DRAW_FUN_RAIL::IfOnXY(double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4)//是否与线元有交点
{
	double A11,A12,A21,A22,B1,B2,D,D1,D2;
	int INF;
	double X1min,Y1min,X2min,Y2min,X1max,Y1max,X2max,Y2max;
	double x,y;


	A11 = Y2-Y1;
	A12 = X1-X2;
	A21 = Y4-Y3;
	A22 = X3-X4;
	B1 = X1*Y2 - X2*Y1;
	B2 = X3*Y4 - X4*Y3;
	D = A11*A22-A21*A12;
	if(fabs(D)<1.0E-5)
		INF = 0;
	else
	{
		D1 = B1*A22-B2*A12;
		D2 = B2*A11-B1*A21;

		x  = D1/D;
		y  = D2/D;
		X1min=min(X1,X2)-0.001;
		X1max=max(X1,X2)+0.001;
		Y1min=min(Y1,Y2)-0.001;
		Y1max=max(Y1,Y2)+0.001;
		X2min=min(X3,X4)-0.001;
		X2max=max(X3,X4)+0.001;
		Y2min=min(Y3,Y4)-0.001;
		Y2max=max(Y3,Y4)+0.001;

		if( X2min<=x && x<=X2max && Y2min<=y && y<=Y2max)//在线元上
			INF = 1;//有交点
		else
			INF = 0;
	}
	/*
	acedCommandS(RTSTR,L"setvar",RTSTR,L"cmdecho",RTSHORT,0,RTNONE);
	ads_point pt1,pt2;
	pt1[1]=X1,pt1[0]=Y1;
	pt2[1]=X2,pt2[0]=Y2;
	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTPOINT,pt2,RTSTR,L"",RTNONE);
	acedCommandS(RTSTR,L"color",RTSHORT,4,RTNONE);
	pt1[1]=X3,pt1[0]=Y3;
	pt2[1]=X4,pt2[0]=Y4;
	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTPOINT,pt2,RTSTR,L"",RTNONE);

	ads_printf(L"X1=%lf Y1=%lf X2=%lf Y2=%lf X3=%lf Y3=%lf X4=%lf Y4=%lf\n",X1,Y1,X2,Y2,X3,Y3,X4,Y4);
	ads_printf(L"D=%lf x=%lf Xmin=%lf Xmax=%lf\n",D,x,X2min,X2max);
	ads_printf(L"y=%lf Ymin=%lf Ymax=%lf\n",y,Y2min,Y2max);*/

	return INF; 

}
//输入一线里程，一线线元参数，二线线元数据，求线间距,二线投影连续里程及投影点
int BAS_DRAW_FUN_RAIL::CalXjj(double ml1,double array1[4*MAXJDNUM][10],int tarray1,double array2[4*MAXJDNUM][10],int tarray2,double &xjj,double &projml,double resPt[2])
{
	//假设为右侧;
	xlpoint PZ;
	PZ.lc = ml1;
	xlpoint_pz(array1,tarray1,&PZ);
	double fwj = PZ.a;
	double Pt1[3];
	Pt1[0]=PZ.x,Pt1[1]=PZ.y;//一线上的点,Pt1[2]=PZ.a

	//判断二线位于左侧还是右侧
	int LorR;
	LorR = Judge2XLorR(PZ,array2,tarray2);
	if(LorR==0)//二线与一线重合518
	{
		xjj = 0.0;
		projml = PROJ_ML(array2,tarray2,Pt1[0],Pt1[1],Pt1[0],Pt1[1]);//投影到2线
		resPt[0]=Pt1[0];
		resPt[1]=Pt1[1];
	}
	else
	{
		projml = Search_EXPt(array2,tarray2,Pt1,fwj+0.5*pi,resPt);//点在二线的投影连续里程几投影点LorR*
		xjj = 0.0;
		if(projml<-0.001)
		{
			resPt[0]=Pt1[0];//2006-06-13
			resPt[1]=Pt1[1];//2006-06-13
			return 0;//没找到
		}
		xjj = (resPt[1]-Pt1[1])*cos(fwj) - (resPt[0]-Pt1[0])*sin(fwj);//2006-06-13
		//		xjj = LorR*sqrt((Pt1[0]-resPt[0])*(Pt1[0]-resPt[0])+(Pt1[1]-resPt[1])*(Pt1[1]-resPt[1]));
		if(fabs(xjj)<0.001)
			xjj = 0.0;
	}
	return 1;
}
//输入一线上一点PZ,二线线元数据
int BAS_DRAW_FUN_RAIL::Judge2XLorR(xlpoint PZ,double array2[4*MAXJDNUM][10],int tarray2)
{
	double Pt1[2],fwj;
	fwj = PZ.a;
	Pt1[0] = PZ.x,Pt1[1] = PZ.y;
	double ml2 = PROJ_ML(array2,tarray2,Pt1[0],Pt1[1],Pt1[0],Pt1[1]);//投影到2线
	PZ.lc = ml2;
	xlpoint_pz(array2,tarray2,&PZ);////

	double dist = sqrt((Pt1[0]-PZ.x)*(Pt1[0]-PZ.x)+(Pt1[1]-PZ.y)*(Pt1[1]-PZ.y));//两个投影点距离

	if(dist<0.001)//二线与一线重合
		return 0;

	double fwj1to2;
	xyddaa(Pt1[0],Pt1[1],PZ.x,PZ.y,&fwj1to2);
	int LorR;
	double zj = ZXJ(fwj1to2,fwj);
	if(zj>0.0)//
		LorR = 1;//二线在右侧
	else
		LorR = -1;//二线在左侧

	return LorR;
}
//----------------------------------------------------------------------------------
double BAS_DRAW_FUN_RAIL::Get_QXJK(CString TLDJ,double design_V,double SR, GuiFanCanShu mGuiFanCS)
{
	/*
	double qxjk;

	if(TLDJ=="地方I级")
	{
	if(SR<401)	     qxjk = 0.4;
	else if(SR<451)	 qxjk = 0.3;
	else if(SR<701)  qxjk = 0.2;
	else if(SR<3001) qxjk = 0.1;
	else qxjk = 0.0;
	}
	else if(TLDJ=="地方II级")
	{
	if(SR<401)	     qxjk = 0.3;
	else if(SR<451)	 qxjk = 0.2;
	else if(SR<1201)  qxjk = 0.1;
	else qxjk = 0.0;
	}
	else if(TLDJ=="地方III级")
	{
	if(SR<301)	     qxjk = 0.3;
	else if(SR<451)	 qxjk = 0.2;
	else if(SR<1201)  qxjk = 0.1;
	else qxjk = 0.0;
	}
	else if(TLDJ=="I级" || TLDJ=="II级")
	{
	if(SR<801)	     qxjk = 0.5;
	else if(SR<1001)	 qxjk = 0.4;
	else if(SR<1601)  qxjk = 0.3;
	else if(SR<6001)  qxjk = 0.2;
	else if(SR<10001)  qxjk = 0.1;
	else qxjk = 0.0;
	}
	else  if(TLDJ=="III级")
	{
	if(SR<601)	     qxjk = 0.5;
	else if(SR<801)	 qxjk = 0.4;
	else if(SR<1001)  qxjk = 0.3;
	else if(SR<2001)  qxjk = 0.2;
	else if(SR<5001)  qxjk = 0.1;
	else qxjk = 0.0;
	}
	else if(fabs(design_V-160)<0.5)
	{
	if(SR<2001)	     qxjk = 0.4;
	else if(SR<4001)	 qxjk = 0.3;
	else if(SR<6001)  qxjk = 0.2;
	else if(SR<8001)  qxjk = 0.1;
	else qxjk = 0.0;
	}
	else if(fabs(design_V-200)<0.5)
	{
	if(SR<3001)	     qxjk = 0.4;
	else if(SR<6001)  qxjk = 0.3;
	else qxjk = 0.2;
	}
	else if(fabs(design_V-250)<0.5)
	{
	if(SR<3001)	     qxjk = 0.4;
	else if(SR<6001)  qxjk = 0.3;
	else qxjk = 0.2;
	}
	else if(design_V>299)
	{
	if(SR<5501)	     qxjk = 0.4;
	else if(SR<7001)  qxjk = 0.5;
	else if(SR<11001)  qxjk = 0.4;
	else if(SR<14001)  qxjk = 0.3;
	}
	else
	qxjk = 0.0;
	return qxjk;*/

	int i=0;
	for(i=0;i<mGuiFanCS.QuXianFJi;i++)
	{
		if(SR < mGuiFanCS.QuXianRFJ[i])
			return mGuiFanCS.LuJiMJiaK[i];
	}
	return mGuiFanCS.LuJiMJiaK[i-1];
}
//竖曲线半径
double BAS_DRAW_FUN_RAIL::Get_Rshu(double design_V,double deltaI, GuiFanCanShu mGuiFanCS)
{
	double Rshu=0.0;
	if(deltaI<=mGuiFanCS.ShuTiaoJ)
		Rshu=0.0;
	else
		Rshu= mGuiFanCS.ShuR;
	return Rshu;
}


double BAS_DRAW_FUN_RAIL::PXJ(double fwj1, double fwj2)
{
	double jiao;	
	jiao=fwj2-fwj1;
	if(jiao<0)jiao+=2*pi;
	if(jiao>pi)jiao=2*pi-jiao;
	return fabs(jiao);
}

double BAS_DRAW_FUN_RAIL::fwj(double x0,double y0,double x1,double y1)
{

	ads_point spt,ept;
	spt[X]=x0;
	spt[Y]=y0;
	ept[X]=x1;
	ept[Y]=y1;
	double ang=ads_angle(spt,ept);//与X轴夹角
	ang=2.5*pi-ang;
	if(ang>2*pi)ang-=2*pi;
	return ang;
}

double BAS_DRAW_FUN_RAIL::AFA(double fwj1,double fwj2)
{
	double af;
	af=fwj2-fwj1;
	if(af<0)af+=2*pi;
	if(af>2*pi)af=2*pi-af;
	return af;
}

double BAS_DRAW_FUN_RAIL::radiusfree(AcDbLine *Line1, AcDbLine *Line2,AcGePoint3d pk)
{
	AcGePoint3d sPt1,ePt1,sPt2,ePt2;
	double rad=0.0,XdE,xa,xb,xc,dita,jbata;
	AcDbObjectId arcId;

	//	acdbOpenObject(Line1,arcId,AcDb::kForRead,Adesk::kFalse);
	//	acdbOpenObject(Line2,arcId,AcDb::kForRead,Adesk::kFalse);
	if(Line1==NULL || Line2==NULL)return 0.0;
	sPt1=Line1->startPoint();
	ePt1=Line1->endPoint();
	sPt2=Line2->startPoint();
	ePt2=Line2->endPoint();

	AcGePoint3dArray interPt;
	Line1->intersectWith(Line2,AcDb::kExtendBoth,interPt,0,0); 

	double fwj1=fwj(sPt1.x,sPt1.y,ePt1.x,ePt1.y);
	double fwj2=fwj(sPt2.x,sPt2.y,ePt2.x,ePt2.y);
	double af1=AFA(fwj1,fwj2);
	double fwjr=-af1/2+fwj2+PI/2;
	ads_point pt1,pt2;
	pt1[X]=interPt[0].x;
	pt1[Y]=interPt[0].y;
	pt2[X]=pk.x;
	pt2[Y]=pk.y;
	double fwje=2.5*PI-ads_angle(pt2,pt1);
	jbata=PI-PXJ(fwje,fwjr);
	//	pk.x=20;
	//	pk.y=20;
	//	pk.z=0;
	XdE=pk.distanceTo(interPt[0]);
	double af=PXJ(fwj1,fwj2);
	xa=1/pow(cos(af/2),2)-1;
	xb=-2*XdE*cos(jbata)/cos(af/2);
	xc=pow(XdE,2);	
	dita=pow(xb,2)-4*xa*xc;
	rad=(-xb+powf(dita,0.5))/(2*xa);
	Line1->close();
	Line2->close();
	return rad;
}

//double BAS_DRAW_FUN::radiusfree(AcDbLine *Line1, AcDbLine *Line2,AcGePoint3d pk)
//{
//	AcGePoint3d sPt1,ePt1,sPt2,ePt2;
//	double rad=0.0,XdE,xa,xb,xc,dita,jbata;
//	AcDbObjectId arcId;
//	AcDbObject *pObj;
//	acdbOpenObject(pObj,arcId,AcDb::kForRead,Adesk::kFalse);
//	Line1 = AcDbLine::cast(pObj);
//	acdbOpenObject(pObj,arcId,AcDb::kForRead,Adesk::kFalse);
//	Line2 = AcDbLine::cast(pObj);
//	sPt1=Line1->startPoint();
//	ePt1=Line1->endPoint();
//	sPt2=Line2->startPoint();
//	ePt2=Line2->endPoint();
//	
//	AcGePoint3dArray interPt;
//	Line1->intersectWith(Line2,AcDb::kExtendBoth,interPt,0,0); 
//	
//	double fwj1=fwj(sPt1.x,sPt1.y,ePt1.x,ePt1.y);
//	double fwj2=fwj(sPt2.x,sPt2.y,ePt2.x,ePt2.y);
//	double af1=AFA(fwj1,fwj2);
//	double fwjr=-af1/2+fwj2+PI/2;
//	ads_point pt1,pt2;
//	pt1[X]=interPt[0].x;
//	pt1[Y]=interPt[0].y;
//	pt2[X]=pk.x;
//	pt2[Y]=pk.y;
//	double fwje=2.5*PI-ads_angle(pt2,pt1);
//	jbata=PI-PXJ(fwje,fwjr);
//	//	pk.x=20;
//	//	pk.y=20;
//	//	pk.z=0;
//	XdE=pk.distanceTo(interPt[0]);
//	double af=PXJ(fwj1,fwj2);
//	xa=1/pow(cos(af/2),2)-1;
//	xb=-2*XdE*cos(jbata)/cos(af/2);
//	xc=pow(XdE,2);	
//	dita=pow(xb,2)-4*xa*xc;
//	rad=(-xb+powf(dita,0.5))/(2.0*xa);
//	Line1->close();
//	Line2->close();
//	return rad;
//}
//plh08.20
double BAS_DRAW_FUN_RAIL::GetRFromT1T2(double &x1, double &y1, double &x0,double &y0,double &x2,double &y2, GuiFanCanShu& mGuiFanCS, bool IsStandard, double DV,int INTRadius,int JUDGET012)
	//给定一个交点，直始x1,y1,交点x0,y0,终止x2,y2,是否遵守半径系列，返回半径
	//int JUDGET012=0//不进行夹直线判断？int JUDGET012=1//与前面夹直线判断int JUDGET012=2//与后面夹直线判断int JUDGET012=12//与前后夹直线判断
{
	//	acutPrintf(L"x1=%.2lf,y1=%.2lf,x0=%.2lf,y0=%.2lf,x2=%.2lf,y2=%.2lf\n",x1,y1,x0,y0,x2,y2);
	struct resbuf resSHORTCUT;//设置右键功能
	resSHORTCUT.restype=RTSHORT;
	acedGetVar(L"SHORTCUTMENU",&resSHORTCUT);
	short saveShortCutValue=resSHORTCUT.resval.rint;
	resSHORTCUT.resval.rint=8;
	acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);


	bool ISERROR=false;
	double T0min,Lrmin;
	GetT0min_Lrmin(DV,T0min,Lrmin,mGuiFanCS);
	double T1=99999,T2=99999;
	//由x1,y1,x0,y0,x2,y2确定最大切长以保证最小夹直线长
	double Tmax,Tmax1,Tmax2;
	if(JUDGET012==0)
	{
		Tmax=99999999;
	}
	else if(JUDGET012==1)
	{
		Tmax=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))-T0min;
	}
	else if(JUDGET012==2)
	{
		Tmax=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))-T0min;
	}
	else//12
	{
		Tmax1=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))-T0min;
		Tmax2=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))-T0min;
		Tmax=min(Tmax1,Tmax2);
	}
	ads_point pt1,pt2;
	AcDbObjectId eId,eId1,arcId,LId1,LId2;
	AcGePoint3d center;//圆心

	AcGePoint3d sPt(x1,y1,0.0),sPt1(x0,y0,0.0);
	AcGePoint3d ePt(x0,y0,0.0),ePt1(x2,y2,0.0);
	AcGePoint2d spoint1(x1,y1),epoint1(x0,y0),spoint2(x0,y0),epoint2(x2,y2);
	AcGeLine2d Line1(spoint1,epoint1);
	AcGeLine2d Line2(spoint2,epoint2);
	AcDbLine *pl1=new AcDbLine(sPt,ePt);
	AcDbLine *pl2=new AcDbLine(sPt1,ePt1);
	/*
	pl1->setLineWeight(AcDb::kLnWt080);
	pl2->setLineWeight(AcDb::kLnWt080);
	*/
	LId1=AddEntityToDbs(pl1);
	LId2=AddEntityToDbs(pl2);

	double fwj1,fwj2;
	fwj1=fwj(sPt.x,sPt.y,ePt.x,ePt.y);
	fwj2=fwj(sPt1.x,sPt1.y,ePt1.x,ePt1.y);
	double pxj=PXJ(fwj1,fwj2);
	double afa=AFA(fwj1,fwj2);
	double fwjr=(pi-afa)/2+fwj2;
	double rad=-1.0,dT,jiao;
	double angle1,angle2;
	double ang1,ang2;
	AcGePoint3d pk(0,0,0);
	int type;
	struct resbuf result;
	AcDbObject *pObj;
	AcDbArc *pArc= new AcDbArc;//(center,rad,angle1,angle2);
	arcId=AddEntityToDbs(pArc);	
	/*acdbOpenObject(pObj,arcId,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	pArc = AcDbArc::cast(pObj);
	pArc->setColorIndex(3);
	pArc->close();
	/*	ACHAR Rchr[256];
	_tcscpy(Rchr,L"");
	double texth=8.0,textroate=0.0;
	texth=0.02*sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));
	AcDbText *pText=new AcDbText(pk,Rchr);
	pText->setColorIndex(3);
	pText->setHeight(texth);
	AcDbObjectId textId=AddEntityToDbs(pText);
	*/	
	AcDbLine *perLine3d1=new AcDbLine();
	perLine3d1->setColorIndex(3);
	AcDbObjectId perLId1=AddEntityToDbs(perLine3d1);

	AcDbLine *perLine3d2=new AcDbLine();
	perLine3d2->setColorIndex(3);
	AcDbObjectId perLId2=AddEntityToDbs(perLine3d2);
	bool ISOK=true;
	double tmpR=0.0;
	acedSSSetFirst(NULL,NULL);
	for(;;)
	{	 
		if(acedGrRead(1 , &type , &result)!=RTNORM)
		{ 
			ISOK=false;
			break;
		}
		//if(type==2)
		//{
		//	short KEYCODE=result.resval.rint;
		//	//acutPrintf(L"%d",KEYCODE);
		//	if(KEYCODE==32)
		//	{
		//	}
		//	else if(KEYCODE==13)
		//	{
		//		break;
		//	}
		//	else {}
		//}
		else if(type==25)
		{
			if(rad<0.00001)continue;			
			double cenX,cenY,dx,dy;
			cenX=center.x;
			cenY=center.y;
			if(GetCircleFromT_P_R(x1,y1,x0,y0,rad,cenX,cenY)!=true)continue;
			dx=cenX-center.x;
			dy=cenY-center.y;
			//			acutPrintf(L"dx=%lf,dy=%lf\n",dx,dy);
			x0+=dx;
			y0+=dy;
			//			x1+=dx;
			//			y1+=dy;
			x2+=dx;
			y2+=dy;
			/*acdbOpenObject(pObj,LId1,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pObj,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			pl1 = AcDbLine::cast(pObj);
			/*	acdbOpenObject(pObj,LId2,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pObj,LId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			pl2 = AcDbLine::cast(pObj);
			pl1->setStartPoint(AcGePoint3d(x1,y1,0.0));
			pl1->setEndPoint(AcGePoint3d(x0,y0,0.0));
			pl2->setStartPoint(AcGePoint3d(x0,y0,0.0));
			pl2->setEndPoint(AcGePoint3d(x2,y2,0.0));
			pl1->close();
			pl2->close();
			if(JUDGET012==0)
			{
				Tmax=99999999;
			}
			else if(JUDGET012==1)
			{
				Tmax=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))-T0min;
			}
			else if(JUDGET012==2)
			{
				Tmax=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))-T0min;
			}
			else//12
			{
				Tmax1=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0))-T0min;
				Tmax2=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0))-T0min;
				Tmax=min(Tmax1,Tmax2);
			}
		}
		else if(type == 3)break;
		else if(type == 5 || type==2)
		{
			pk.x=result.resval.rpoint[X];
			pk.y=result.resval.rpoint[Y];
			pk.z=result.resval.rpoint[Z];

			//			E=pk.distanceTo(interPt[0]);
			double tempr=radiusfree(pl1,pl2,pk);
			if(tempr>5.0)rad=tempr;
			double l0 = 0.0;
			if(IsStandard)
			{
				if (type==2)
				{
					if(acedUsrBrk())
					{
						return 100.0;			 
					}
					CString mes;
					mes.Format(L"\n请输入半径<%0.2lf>",rad);	
					int rc = ads_getreal(mes,&tempr);
					if (rc==RTCAN)
						return 100.0;
					else rad = tempr;
					if (rc==RTNORM || rc==RTNONE)
					{
						l0=get_l0(rad,DV,mGuiFanCS);
						//mes.Format(L"\n请输入缓长<%0.2lf>",l0);	
						//rc = ads_getreal(mes,&l0);
						//if (rc==RTCAN)
						//{
						//	return tempr;
						//}
					}
				}
				else
				{
					tempr=get_R(rad,DV,mGuiFanCS);
					if(tempr>5.0)rad=tempr;
					l0=get_l0(rad,DV,mGuiFanCS);
				}

				double BPMTL[10];
				double f1=fwj(x1,y1,x0,y0);
				double f2=fwj(x0,y0,x2,y2);
				double aa=AFA(f1,f2);
				if (aa>pi)aa=2*pi-aa;
				getBPMTL(dms_rad(aa),rad,l0,l0,&BPMTL[0]);
				T1=BPMTL[6];
				T2=BPMTL[7];

				//acutPrintf(L"最小夹直线长为: %.2lf,L",T0min);

				if(JUDGET012==0)
				{
					ISERROR=false;
				}
				else if(JUDGET012==12)
				{					
					//acutPrintf(L"当前夹直线长为: L1=%.2lf,L2=%.2lf(右键平移!)\n",Tmax1-T1+T0min,Tmax2-T2+T0min);
					//acutPrintf(L"Tmax1=%.2lf,Tmax2=%.2lf\n",Tmax1,Tmax2);
					if(T1>Tmax1||T2>Tmax2)ISERROR=true;
					else ISERROR=false;
				}
				else if(JUDGET012==1)
				{
					//acutPrintf(L"当前夹直线长为: L1=%.2lf(右键平移!)\n",Tmax-T1+T0min);
					if(T1>Tmax)ISERROR=true;
					else ISERROR=false;
				}
				else if(JUDGET012==2)
				{
					//acutPrintf(L"当前夹直线长为: L2=%.2lf(右键平移!)\n",Tmax-T2+T0min);
					if(T2>Tmax)ISERROR=true;
					else ISERROR=false;
				}
			}
			else if(INTRadius>0)rad=(int)(rad/INTRadius)*INTRadius;

			if(fabs(rad-tmpR)>0.001)
			{
				ACHAR pszLabel[256];
				if(ISERROR)_stprintf(pszLabel,L"当前半径为: %.2lf  不合适!",rad);
				else _stprintf(pszLabel,L"当前半径为: %.2lf",rad);				
				acedGrText(-1,pszLabel,1);    
				tmpR=rad;
			}			

			center.x=x0+(rad/cos(pxj/2))*sin(fwjr);
			center.y=y0+(rad/cos(pxj/2))*cos(fwjr);

			dT=rad*tan(0.5*pxj);
			pt1[X]=sPt.x;
			pt1[Y]=sPt.y;
			pt2[X]=ePt.x;
			pt2[Y]=ePt.y;
			angle1=ads_angle(pt2,pt1);

			pt1[X]=sPt1.x;
			pt1[Y]=sPt1.y;
			pt2[X]=ePt1.x;
			pt2[Y]=ePt1.y;
			angle2=ads_angle(pt1,pt2);

			ads_point qpt1,qpt2,cen;
			qpt1[X]=x0+dT*cos(angle1);
			qpt1[Y]=y0+dT*sin(angle1);

			qpt2[X]=x0+dT*cos(angle2);
			qpt2[Y]=y0+dT*sin(angle2);

			cen[X]=center.x;
			cen[Y]=center.y;

			ang1=ads_angle(cen,qpt1);
			ang2=ads_angle(cen,qpt2);


			/*			_stprintf(Rchr,L"%.2lf\n",rad);
			texth=0.1*rad;
			acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
			pText->setTextString(Rchr);
			pText->setHeight(texth);
			pText->setPosition(pk);
			if(ISERROR)pText->setColorIndex(1);
			else pText->setColorIndex(3);			
			pText->close();
			*/

			/*	acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			jiao=fwj2-fwj1;
			if(jiao<0)jiao+=2*PI;
			pArc->setCenter(center);
			pArc->setRadius(rad);
			if(jiao>PI){
				pArc->setStartAngle(ang1);
				pArc->setEndAngle(ang2);
			}
			else{
				pArc->setStartAngle(ang2);
				pArc->setEndAngle(ang1);
			}
			if(ISERROR)
			{
				pArc->setColorIndex(1);
				acutPrintf(L"夹直线长不够!\n");
			}
			else pArc->setColorIndex(3);			
			pArc->close();

			/*
			x1=center.x+rad*cos(ang1);
			y1=center.y+rad*sin(ang1);
			x2=center.x+rad*cos(ang2);
			y2=center.y+rad*sin(ang2);
			acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse);
			acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse);
			pl1->setStartPoint(AcGePoint3d(x1,y1,0.0));
			pl1->setEndPoint(AcGePoint3d(x0,y0,0.0));
			pl2->setStartPoint(AcGePoint3d(x0,y0,0.0));
			pl2->setEndPoint(AcGePoint3d(x2,y2,0.0));
			pl1->close();
			pl2->close();
			*/
			double perX,perY;
			GetPerPoint(center.x,center.y,x1,y1,x0,y0,perX,perY);
			/*acdbOpenObject(pObj,perLId1,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pObj,perLId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			perLine3d1=AcDbLine::cast(pObj);
			perLine3d1->setStartPoint(AcGePoint3d(perX,perY,0.0));
			perLine3d1->setEndPoint(center);
			perLine3d1->close();//切点与圆心连线				

			GetPerPoint(center.x,center.y,x2,y2,x0,y0,perX,perY);
			/*acdbOpenObject(pObj,perLId2,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pObj,perLId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			perLine3d2=AcDbLine::cast(pObj);
			perLine3d2->setStartPoint(AcGePoint3d(perX,perY,0.0));
			perLine3d2->setEndPoint(center);
			perLine3d2->close();//切点与圆心连线		
		}
		if (type==2)
			break;
	}
	/*acdbOpenObject(pObj,arcId,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	pArc = AcDbArc::cast(pObj);
	pArc->erase();
	pArc->close();
	/*	acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
	pText->erase();	
	pText->close();
	*/
	/*acdbOpenObject(pObj,LId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	pl1 = AcDbLine::cast(pObj);
	/*acdbOpenObject(pObj,LId2,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,LId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	pl2 = AcDbLine::cast(pObj);
	double jiao1,jiao2,dist1,dist2;
	jiao1=acutAngle(asDblArray(pl1->endPoint()),asDblArray(pl1->startPoint()));
	jiao2=acutAngle(asDblArray(pl2->startPoint()),asDblArray(pl2->endPoint()));
	dist1=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	dist2=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));
	if(T1>dist1)
	{
		dist1=1.2*T1;
		x1=x0+dist1*cos(jiao1);
		y1=y0+dist1*sin(jiao1);
	}
	if(T2>dist2)
	{
		dist2=1.2*T2;
		x2=x0+dist2*cos(jiao2);
		y2=y0+dist2*sin(jiao2);
	}
	//	acutPrintf(L"jiao1=%lf,jiao2=%lf,x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",jiao1,jiao2,x1,y1,x2,y2);
	pl1->erase();	
	pl2->erase();	
	pl1->close();
	pl2->close();


	/*acdbOpenObject(pObj,perLId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,perLId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	pObj->erase();
	pObj->close();//切点与圆心连线				
	/*acdbOpenObject(pObj,perLId2,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,perLId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	pObj->erase();
	pObj->close();//切点与圆心连线				
	if(ISERROR)rad=0;		
	//	acutPrintf(L"返回半径:%.2lf\n",rad);
	//	if(ISERROR)return -1.0;
	acedGrText(-5,L"完成",0);    
	acutPrintf(L"\n命令:");
	resSHORTCUT.resval.rint=saveShortCutValue;
	acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);
	if(ISOK)return rad;
	else return -1.0;
}

bool BAS_DRAW_FUN_RAIL::GetCircleFromT_P_R(double x1,double y1,double x2,double y2,double R,double &cenX,double &cenY)
	//给定切线，半径，返回圆心
{
	if(R<0.0001)return false;
	// TODO: Implement the command
	// input R;
	double side1,side2;
	side1=GetSide(x1,y1,x2,y2,cenX,cenY);
	side1/=fabs(side1);
	double dx=x1-x2;
	double dy=y1-y2;
	x1+=100.0*dx;
	y1+=100.0*dy;
	AcDbObjectId eId;
	AcGePoint3d sPt0(x1,y1,0.0),ePt0(x2,y2,0.0);
	AcDbLine *pl1=new AcDbLine(sPt0,ePt0);
	AcGePoint2d sPt1(sPt0.x,sPt0.y),ePt1(ePt0.x,ePt0.y);
	AcGeLine2d pLine1(sPt1,ePt1);
	//constructe the circle.	
	AcDbCircle *pCircle1= new AcDbCircle();
	AcDbObjectId cirId1=AddEntityToDbs(pCircle1);
	AcDbCircle *pCircle2= new AcDbCircle();
	AcDbObjectId cirId2=AddEntityToDbs(pCircle2);
	AcDbObject *pObj;
	//acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse);
	if(acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pCircle1 = AcDbCircle::cast(pObj);
	pCircle1->setColorIndex(1);
	pCircle1->close();
	/*acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse);	*/
	if(acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pCircle2 = AcDbCircle::cast(pObj);
	pCircle2->setColorIndex(1);
	pCircle2->close();
	AcDbLine *perLine3d=new AcDbLine();
	perLine3d->setColorIndex(3);
	AcDbObjectId perLId=AddEntityToDbs(perLine3d);

	// get the point of the circle must be passed.
	AcGePoint2d pk;
	int type;
	int shift=0;
	struct resbuf result;
	AcGeLine2d perpLine;
	double perX,perY;

	//int shift=0;//{shift=0 two circle;shift =1 one circle;shift=2 the other circle}
	for(;;){//

		if(ads_grread(1 , &type , &result)!=RTNORM)
		{ 
			break ;
		}
		if(type==2)
		{
			short KEYCODE=result.resval.rint;
			//acutPrintf(L"%d",KEYCODE);
			if(KEYCODE==32)
			{
			}
			else if(KEYCODE==13)
			{
				break;
			}
			else {}
		}
		if(type == 3)      break;
		if(type == 25)
		{
			/*	acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return false;
			}
			pObj->erase();
			pObj->close();
			/*acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return false;
			}
			pObj->erase();
			pObj->close();
			/*acdbOpenObject(pObj,perLId,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pObj,perLId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return false;
			}
			pObj->erase();
			pObj->close();//切点与圆心连线			
			return false;
		}
		if((type!=5) && (type!=12) )
		{ 
			//ads_printf(L"\n必须用鼠标! ");		
			break ;
		}
		if(type == 5)
		{
			pk.x=result.resval.rpoint[X];
			pk.y=result.resval.rpoint[Y];//get AcGePoint2d pk;
			side2=GetSide(x1,y1,x2,y2,pk.x,pk.y);
			side2/=fabs(side2);
			acutPrintf(L"%lf,%lf\n",side1,side2);
			if(side1*side2<=0)continue;
			AcGeLine2d pLine1(sPt1,ePt1);
			pLine1.getPerpLine(pk,perpLine);//做垂线
			AcGePoint2d interPt0;//垂足在已知线上
			AcGeTol tol;
			pLine1.intersectWith(perpLine,interPt0,tol); //求垂足
			double dist0p=interPt0.distanceTo(pk);//垂足到拾取点之距
			if(dist0p<=2*R&&dist0p>0)
			{
				AcGeVector2d translateVec(R*(pk.x-interPt0.x)/dist0p,R*(pk.y-interPt0.y)/dist0p);
				AcGeMatrix2d matrix=matrix.translation(translateVec);
				pLine1.transformBy(matrix);			//得pLine1的平行线，它们之距为R
				AcGeCircArc2d pCirclefz(pk,R);
				int intn;
				AcGePoint2d p1,p2;
				pCirclefz.intersectWith(pLine1,intn,p1,p2,tol);//辅助圆与平行线的交点。	
				AcGePoint3d p31(p1.x,p1.y,0),p32(p2.x,p2.y,0);
				double distcen1=p1.distanceTo(sPt1);//from the start point of line to center1
				double distcen2=p2.distanceTo(sPt1);//from the start point of line to center2
				if(distcen1<distcen2)
				{	
					/*acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					pCircle1 = AcDbCircle::cast(pObj);
					pCircle1->setCenter(p31);
					pCircle1->setRadius(R);
					pCircle1->close();
					/*acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					pCircle2 = AcDbCircle::cast(pObj);
					pCircle2->setCenter(p31);
					pCircle2->setRadius(0);
					pCircle2->close();
					cenX=p31.x;
					cenY=p31.y;
				}
				else
				{
					/*acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					pCircle1 = AcDbCircle::cast(pObj);
					pCircle1->setCenter(p32);
					pCircle1->setRadius(0);
					pCircle1->close();
					/*acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse);	*/
					if(acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					pCircle2 = AcDbCircle::cast(pObj);
					pCircle2->setCenter(p32);
					pCircle2->setRadius(R);
					pCircle2->close();
					cenX=p32.x;
					cenY=p32.y;
				}
				GetPerPoint(cenX,cenY,x1,y1,x2,y2,perX,perY);
				/*acdbOpenObject(pObj,perLId,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(pObj,perLId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return false;
				}
				perLine3d = AcDbLine::cast(pObj);
				perLine3d->setStartPoint(AcGePoint3d(perX,perY,0.0));
				perLine3d->setEndPoint(AcGePoint3d(cenX,cenY,0.0));
				perLine3d->close();//切点与圆心连线				
			}
		}
	}//for(;;)
	/*acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}

	pObj->erase();
	pObj->close();
	/*acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pObj->erase();
	pObj->close();
	/*acdbOpenObject(pObj,perLId,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,perLId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pObj->erase();
	pObj->close();//切点与圆心连线	
	return true;
}

void BAS_DRAW_FUN_RAIL::GetPerPoint(double outX, double outY, double x1, double y1, double x2, double y2, double &perX, double &perY)
	//线外一点作垂线,1:线外一点  2:线起点 3:线终点 4:返回垂足
{
	AcDbLine *Line=new AcDbLine(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));
	double fwj1=fwj(x1,y1,x2,y2);
	fwj1+=0.5*pi;
	AcDbLine *perpLine=new AcDbLine(AcGePoint3d(outX,outY,0.0),AcGePoint3d(outX+10*sin(fwj1),outY+10*cos(fwj1),0.0));
	AcGePoint3dArray interPt;
	Line->intersectWith(perpLine,AcDb::kExtendBoth,interPt,0,0);
	perX=interPt[0].x;
	perY=interPt[0].y;
}

//plh08.20
double BAS_DRAW_FUN_RAIL::GetRFromT_P_P(double x1,double y1,double x2,double y2,
										double &xs,double &ys,double &x0,double &y0,
										double &xe,double &ye,double &cenX,double &cenY,
										AcDbObjectIdArray &eraseId, GuiFanCanShu& mGuiFanCS, bool IsStandard, double DV)
										//传入切线方向，返回半径,前一个切点，交点，后一个切点
{
	// TODO: Implement the command
	struct resbuf resSHORTCUT;//设置右键功能
	resSHORTCUT.restype=RTSHORT;
	acedGetVar(L"SHORTCUTMENU",&resSHORTCUT);
	short saveShortCutValue=resSHORTCUT.resval.rint;
	resSHORTCUT.resval.rint=8;
	acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);

	double jiao12;
	double dist=xyddaa(x1,y1,x2,y2,&jiao12);
	dist*=1000;
	x2+=dist*cos(jiao12);
	y2+=dist*sin(jiao12);
	x1-=dist*cos(jiao12);
	y1-=dist*sin(jiao12);
	double rad=-1.0;//半径
	double pxj,fwj1,fwj2,fwjr,fwjt,jiao,afa;
	AcGePoint3d center;

	AcGePoint3d sPt1(x1,y1,0.0);//切线的起点
	AcGePoint3d ePt1(x2,y2,0.0);//切线的终点
	AcDbLine *pl1=new AcDbLine(sPt1,ePt1);

	double side1,side2;	
	double tmpR=0.0;
	//select point.
	ads_point pk00;
	//	AfxMessageBox(L"1");
	if(RTNORM!=acedGetPoint(NULL,L"请选择圆弧上第一个点:\n",pk00))
	{
		resSHORTCUT.resval.rint=saveShortCutValue;
		acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);
		return -1.0;
	}
	//	AfxMessageBox(L"2");
	AcGePoint3d Pk1(pk00[X],pk00[Y],pk00[Z]);
	ads_point pt1,pt2;
	AcGePoint3d pk;
	int type;
	struct resbuf result;
	double dT;
	double angle1;
	double ang1,ang2;
	//	AcDbArc *pArc= new AcDbArc();//弧
	AcDbCircle *pArc= new AcDbCircle();//圆
	AcDbObjectId arcId=AddEntityToDbs(pArc);
	/*	ACHAR Rchr[80];
	double texth=8;
	AcDbText *pText=new AcDbText(pk,Rchr);
	pText->setColorIndex(3);
	pText->setHeight(texth);
	AcDbObjectId textId=AddEntityToDbs(pText);
	*/
	bool ISOK=true;
	for(;;){

		if(ads_grread(1 , &type , &result)!=RTNORM)
		{ 
			ISOK=false;
			break;
		}
		if(type == 3)break;
		else if(type==25)
		{
			if(rad<0.00001)continue;			
			double cenX,cenY,dx,dy;	
			cenX=center.x;//plh06.03
			cenY=center.y;
			if(GetCircleFromT_P_R(x1,y1,x2,y2,rad,cenX,cenY)!=true)continue;
			dx=cenX-center.x;
			dy=cenY-center.y;
			//acutPrintf(L"dx=%lf,dy=%lf\n",dx,dy);
			center.x+=dx;
			center.y+=dy;
			Pk1.x+=dx;
			Pk1.y+=dy;
		}
		else if(type == 5)
		{
			pk.x=result.resval.rpoint[X];
			pk.y=result.resval.rpoint[Y];
			pk.z=result.resval.rpoint[Z];


			side1=GetSide(x1,y1,x2,y2,pk.x,pk.y);
			side2=GetSide(x1,y1,x2,y2,Pk1.x,Pk1.y);
			side1/=fabs(side1);
			side2/=fabs(side2);

			if(side1==0||side2==0)continue;//plh05.27??????????????????
			else
			{
				if(side1*side2<=0)continue;
			}
			AcGePoint3d Pk2(pk.x,pk.y,pk.z);
			if(Pk1.distanceTo(Pk2)!=0)
			{
				AcDbLine *pLinet=new AcDbLine(Pk1,Pk2);

				AcGePoint3dArray interPt;
				pLinet->intersectWith(pl1,AcDb::kExtendBoth,interPt,0,0);
				if (pLinet != NULL)
				{
					delete pLinet;///
					pLinet = NULL;
				}
				AcGePoint3d Pt1(interPt[0].x,interPt[0].y,interPt[0].z);
				AcGePoint3d Pt0((Pk1.x+Pk2.x)/2,(Pk1.y+Pk2.y)/2,(Pk1.z+Pk2.z)/2);
				AcGePoint3d Pt2((2*Pt0.x-Pt1.x),(2*Pt0.y-Pt1.y),(2*Pt0.z-Pt1.z));
				fwjt=fwj(Pk1.x,Pk1.y,Pk2.x,Pk2.y);
				AcGePoint3d Ptx((Pt0.x+100*sin(fwjt+PI/2)),(Pt0.y+100*cos(fwjt+PI/2)),Pt0.z);

				AcDbLine *pLine3=new AcDbLine(Pt0,Ptx);
				//				if(fwjr!=fwj1&&fwj1+fwjr!=PI){ 

				AcGePoint3dArray interPtd;
				pLine3->intersectWith(pl1,AcDb::kExtendBoth,interPtd,0,0);
				delete pLine3;
				AcDbLine *pl2=new AcDbLine(interPtd[0],Pt2);
				AcDbLine *pl11=new AcDbLine(Pt1,interPtd[0]);

				fwj1=fwj(Pt1.x,Pt1.y,interPtd[0].x,interPtd[0].y);
				fwj2=fwj(interPtd[0].x,interPtd[0].y,Pt2.x,Pt2.y);
				pxj=PXJ(fwj1,fwj2);
				afa=AFA(fwj1,fwj2);
				fwjr=(PI-afa)/2+fwj2;
				double temprad=radiusfree(pl11,pl2,Pk2);				
				if(temprad<0.00001||temprad>1.2*get_R(temprad,DV,mGuiFanCS))continue;
				rad=temprad;
				if(IsStandard)rad = get_R(rad,DV,mGuiFanCS);

				if(fabs(rad-tmpR)>0.001)
				{
					ACHAR pszLabel[256];
					_stprintf(pszLabel,L"当前半径为: %.2lf",rad);
					acedGrText(-1,pszLabel,1);    
					tmpR=rad;
				}

				/*				_stprintf(Rchr,L"%.2lf\n",rad);
				texth=0.1*rad;
				acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
				pText->setTextString(Rchr);
				pText->setHeight(texth);
				pText->setPosition(pk);
				pText->close();
				*/				
				delete pl11;
				delete pl2;

				center.x=interPtd[0].x+(rad/cos(pxj/2))*sin(fwjr);
				center.y=interPtd[0].y+(rad/cos(pxj/2))*cos(fwjr);

				dT=rad*tan(0.5*pxj);
				pt1[X]=Pt1.x;
				pt1[Y]=Pt1.y;
				pt2[X]=interPtd[0].x;
				pt2[Y]=interPtd[0].y;
				angle1=ads_angle(pt2,pt1);

				ads_point qpt1,qpt2,cen;
				qpt1[X]=interPtd[0].x+dT*cos(angle1);
				qpt1[Y]=interPtd[0].y+dT*sin(angle1);
				qpt2[X]=Pk2.x;
				qpt2[Y]=Pk2.y;
				cen[X]=center.x;
				cen[Y]=center.y;

				ang1=ads_angle(cen,qpt1);
				ang2=ads_angle(cen,qpt2);
				AcDbObject *pObj;
				/*acdbOpenObject(pObj,arcId,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(pObj,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return 0.0;
				}
				pArc = AcDbCircle::cast(pObj);
				jiao=fwj2-fwj1;
				if(jiao<0)jiao+=2*PI;
				pArc->setCenter(center);
				pArc->setRadius(rad);
				/*
				if(jiao>PI){
				pArc->setStartAngle(ang1);
				pArc->setEndAngle(ang2);
				}
				else{
				pArc->setStartAngle(ang2);
				pArc->setEndAngle(ang1);
				}
				*/
				pArc->close();
				//}
			}
		}
	}
	cenX=center.x;
	cenY=center.y;
	//	eraseId.append(textId);//顺序不能变
	eraseId.append(arcId);

	/*//delete 对象
	acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
	pText->erase();
	pText->close();
	acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);
	pArc->erase();
	pArc->close();
	*/

	//	double &xs,double &ys,double &x0,double &y0,double &xe,double &ye,
	delete pl1;
	pl1=NULL;
	//	acutPrintf(L"rad=%lf\n",rad);
	acedGrText(-5,L"完成",0);    
	acutPrintf(L"\n命令:");

	resSHORTCUT.resval.rint=saveShortCutValue;
	acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);

	if(ISOK)return rad;
	else return -1.0;
}

bool BAS_DRAW_FUN_RAIL::GetTangent(double CenX, double CenY, double R, double &OutX, double &OutY, double &TX, double &TY)
	//传入圆心，半径，一条切线的，起点,终点//终点为切点
	//返回 另一第切线的起点，终点//终点为切点
{
	AcGePoint2d pk;//必经点
	int type;
	struct resbuf result;

	AcDbLine *Line1=new AcDbLine();
	AcDbObjectId LId1=AddEntityToDbs(Line1);
	AcGePoint3d Pt1,Pt2;
	double fwj,OC,bta,side;	
	double x1,y1,x2,y2;//线中起终点
	x1=OutX;
	y1=OutY;
	x2=TX;
	y2=TY;	
	for(;;)
	{		
		if(ads_grread(1 , &type , &result)!=RTNORM)
		{ 
			break;
		}
		if(type == 3)      break;
		if( (type!=5) && (type!=12) )
		{
			ads_printf(L"必须用鼠标!\n");
			break ;
		}
		if(type == 5)
		{
			OutX=result.resval.rpoint[X];
			OutY=result.resval.rpoint[Y];			
			OC = xyddaa(CenX,CenY,OutX,OutY,&fwj);
			if(OC>R)
			{
				if(R/OC>1) bta=0;
				else if(R/OC<-1)bta=pi;
				else bta = acos(R/OC);			

				side=GetSide(x1,y1,x2,y2,OutX,OutY);//判断左右侧左-右+///plh05.27??????????

				if(side>0.0)//>0,圆心在线路左侧，直线与圆相切点在鼠标点与圆心连线的右边
				{
					TX = CenX + R*cos(fwj+bta);//X
					TY = CenY + R*sin(fwj+bta);
				}
				else
				{
					TX = CenX + R*cos(fwj-bta);//X
					TY = CenY + R*sin(fwj-bta);
				}
				AcDbObject *pObj;
				/*acdbOpenObject(pObj,LId1,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(pObj,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return false;
				}
				Line1 = AcDbLine::cast(pObj);
				Line1->setStartPoint(AcGePoint3d(TX,TY,0.0));
				Line1->setEndPoint(AcGePoint3d(OutX,OutY,0.0));
				Line1->close();				
			}
		}
	}
	AcDbObject *pObj;
	/*acdbOpenObject(pObj,LId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pObj,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pObj->erase();
	pObj->close();
	return true;
}



void BAS_DRAW_FUN_RAIL::getLineEndPt(AcGePoint3d sPt,double len,double fwj,AcGePoint3d &ePt)
{
	ePt.z=0;
	ePt.x=sPt.x+len*sin(fwj);
	ePt.y=sPt.y+len*cos(fwj);
}

void BAS_DRAW_FUN_RAIL::GetT0min_Lrmin(double DV,double &T0min, double &Lrmin, GuiFanCanShu mGuiFanCS)
	//传入设计车速
{
	/*
	if(DV<=0){T0min=-1000.0;Lrmin=-1000.0;}
	else
	{	
	if(DV>=350)T0min=Lrmin=280;
	else if(DV>=300)T0min=Lrmin=280;
	else if(DV>=250)T0min=Lrmin=180;
	else if(DV>=200)T0min=Lrmin=140;
	else if(DV>=140)T0min=Lrmin=110;
	else if(DV>=120)T0min=Lrmin=80;
	else if(DV>=100)T0min=Lrmin=60;
	else if(DV>=80)T0min=Lrmin=50;
	else if(DV>=70){T0min=50;Lrmin=20.0;}
	else if(DV>=55){T0min=45;Lrmin=20.0;}
	else if(DV>=40){T0min=40;Lrmin=20.0;}
	else {T0min=-1000.0;Lrmin=-1000.0;}				
	}*/
	T0min = mGuiFanCS.MinZhi;
	Lrmin = mGuiFanCS.MinYuan;
	//   ads_printf(L"############%lf %lf\n",T0min,Lrmin);

}

double BAS_DRAW_FUN_RAIL::DistToLine(double inX1, double inY1, double inX2, double inY2, double &outX,
									 double &outY, double &distTo2PointOnLine)
{
	AcGePoint2d inSPt,inEPt;//线段的起终点
	AcGePoint2d outPt;//线外一点
	AcGePoint2d perpPt;//垂足

	inSPt.set(inX1,inY1);
	inEPt.set(inX2,inY2);
	outPt.set(outX,outY);

	AcGeLine2d inLine(inSPt,inEPt);//线段
	AcGeLine2d perpLine;//过线外一点作线段的垂线	
	inLine.getPerpLine(outPt,perpLine);//作垂线
	inLine.intersectWith(perpLine,perpPt);//求垂足

	//返回线上投影点
	outX=perpPt.x;
	outY=perpPt.y;

	//线外点到线段端点的投影距离的最小值
	distTo2PointOnLine=0.5*( perpPt.distanceTo(inSPt)+perpPt.distanceTo(inEPt)
		-inSPt.distanceTo(inEPt) );
	return outPt.distanceTo(perpPt);
}

//////////////////////////06.09///////////////
double BAS_DRAW_FUN_RAIL::FwjtoAngX(double ang)
{
	ang=2.5*PI-ang;
	StdJiao(ang);
	return ang;
}

void BAS_DRAW_FUN_RAIL::StdJiao(double &jiao)
{
	if(jiao<0.0)jiao+=2.0*PI;
	if(jiao>2.0*PI)jiao-=2.0*PI;
}

///////////////////////////////
//求三个交点的左右转，左-1，右+1
int BAS_DRAW_FUN_RAIL::GetLeftOrRight3JD(double N1, double E1, double N0, double E0, double N2, double E2)
{
	double fb,fe;
	int c;
	xyddaa(N1,E1,N0,E0,&fb);
	xyddaa(N0,E0,N2,E2,&fe);
	double aa=fe-fb;
	if(aa<-pi)aa=aa+2*pi;
	if(aa>pi)aa=aa-2*pi;
	if(aa>0)c=1;//右偏
	else c=-1;//左偏
	return c;
}

void BAS_DRAW_FUN_RAIL::GetCenter3JD(double N1, double E1, double N0, double E0, double N2, double E2, double R, double Lo1, double Lo2, double &CenN, double &CenE)
{
	int c;
	double fb,fe;
	double BPMTL[10],T1;
	xyddaa(N1,E1,N0,E0,&fb);
	xyddaa(N0,E0,N2,E2,&fe);
	double aa=fe-fb;
	if(aa<-pi)aa=aa+2*pi;
	if(aa>pi)aa=aa-2*pi;
	if(aa>0)c=1;//右偏
	else c=-1;//左偏
	aa=fabs(aa);
	aa=dms_rad(aa);
	if(R<=0)return;

	double p1;//前内移量
	if(Lo1>0.0)
	{
		double A1=sqrt(R*Lo1);
		double x=spiral_y(A1,Lo1);
		double t=Lo1/(2.0*R);
		double dr=x+R*sin(t+0.5*PI);			
		p1=dr-R;
	}
	else p1=0.0;					

	/*
	//适用 于对称情况
	getBPMTL(aa,R+p1,0.0,0.0,&BPMTL[0]);
	T1=BPMTL[6];
	double NZY,EZY;
	NZY=N0-T1*cos(fb);
	EZY=E0-T1*sin(fb);
	CenN=NZY+(R+p1)*cos(fb+c*0.5*PI);
	CenE=EZY+(R+p1)*sin(fb+c*0.5*PI);*/


	//求直缓点坐标
	double NZH,EZH;
	getBPMTL(aa,R,Lo1,Lo2,&BPMTL[0]);
	T1=BPMTL[6];
	NZH=N0-T1*cos(fb);
	EZH=E0-T1*sin(fb);	
	//缓圆点坐标
	double NHY,EHY;
	if(Lo1>0.0)
	{
		double x,y,t,A1;
		A1=sqrt(R*Lo1);
		x=spiral_x(A1,Lo1);
		y=spiral_y(A1,Lo1);
		double dist=xyddaa(0.0,0.0,x,y,&t);
		NHY=NZH+dist*cos(fb+c*t);
		EHY=EZH+dist*sin(fb+c*t);
	}
	else
	{
		NHY=NZH;
		EHY=EZH;
	}
	double t=0.5*Lo1/R;
	CenN=NHY+R*cos(fb+c*(t+0.5*PI));
	CenE=EHY+R*sin(fb+c*(t+0.5*PI));
}

//给定圆心半径，从圆到圆外一点的左右转信息，求出公切点
void BAS_DRAW_FUN_RAIL::GetQD(double CenN, double CenE, double R,
							  double OutN, double OutE, int LRFlag,
							  double &QDN, double &QDE)
{
	double DistCenterToOutPt;//圆心到圆外点之距
	double FwjCenterToOutPt;//圆心到圆外点连线的方位角
	DistCenterToOutPt=xyddaa(CenN,CenE,OutN,OutE,&FwjCenterToOutPt);
	if(DistCenterToOutPt<=R)return;//点落到圆内
	double t;//公切点与圆心连线 与 圆心与圆外点连线 的夹角	
	t=acos(R/DistCenterToOutPt);
	double FwjCenterToQD;//圆心到切点连线的方位角
	int c;
	if(LRFlag<0)c=-1;//左转
	else c=1;//右转
	FwjCenterToQD=FwjCenterToOutPt-c*t;
	QDN=CenN+R*cos(FwjCenterToQD);
	QDE=CenE+R*sin(FwjCenterToQD);
	//acutPrintf(L"QD:%lf,%lf\n",QDE,QDN);
}

//int BAS_DRAW_FUN::ReadGuifanFile()
//{
//   CFile guifanFile;
//   CString FilePath;
//
//   ReadWorkdir();
//   FilePath=Cworkdir+"\\data\\jishucanshu." + Pro;
//   //FilePath=Cworkdir+"\\data\\jishucanshu.dat";
//   if(guifanFile.Open(FilePath,CFile::modeRead|CFile::shareDenyNone,NULL)!=NULL)
//   {
//	   ACHAR str01[12];
//	   guifanFile.Read(str01,12);
//	   guifanFile.Read(&m_ZJ,sizeof(QuXianZheJiang));
//	   guifanFile.Read(&mGuiFanCS,sizeof(GuiFanCanShu));
//	   guifanFile.Close();
//	   return 1;
//   }
//   else
//	   return 0;
//}

int BAS_DRAW_FUN_RAIL::ReadGuifanFile(CString ProName, GuiFanCanShu& mGuiFanCS)
{
	CFile guifanFile;
	CString FilePath;

	ReadWorkdir();
	FilePath=Cworkdir+"\\data\\jishucanshu." + ProName;
	//FilePath=Cworkdir+"\\data\\jishucanshu.dat";
	if(guifanFile.Open(FilePath,CFile::modeRead|CFile::shareDenyNone,NULL)!=NULL)
	{
		ACHAR str01[12];
		guifanFile.Read(str01,12);
		guifanFile.Read(&m_ZJ,sizeof(QuXianZheJiang));
		guifanFile.Read(&mGuiFanCS,sizeof(GuiFanCanShu));
		guifanFile.Close();
		return 1;
	}
	else
		return 0;
}

int BAS_DRAW_FUN_RAIL::FindWichDLFromFile(ACHAR GH[10],double XLC)
{
	CString fname;
	int NLC;
	int dlno;
	ACHAR gh[10];
	double xlc,idl;

	dlno = -1;

	fname = Cworkdir + "\\dada\\CLMLInf." + Pro;//特殊里程信息文件
	FILE *fpr;
	fpr = _wfopen(fname,L"r");
	if(fpr)
	{
		fwscanf(fpr,L"%d",&NLC);
		for(int i=0; i<NLC; i++)
		{
			fwscanf(fpr,L"%s%lf%d",gh,&xlc,&idl);
			if(_tcscmp(gh,GH)==0 && fabs(xlc-XLC)<0.01)//文件中有数据
			{
				dlno = idl;
				break;
			}
		}
		fclose(fpr);
	}
	return dlno;
}
int BAS_DRAW_FUN_RAIL::CalJD_XY1_XY2(double XY1[10],double XY2[10],double ResPt[3],double ResPt1[3])	
{
	int JDNUM;

	JDNUM=0;

	if(XY1[0]>0.1 && XY2[0]>0.1)
	{
		if(XY1[0]<1.1)//直线
		{
			if(XY2[0]<1.1)//直线
				JDNUM = CalJD_ZX_ZX(XY1,XY2,ResPt);
			else if(XY2[0]<2.1)//圆
				JDNUM = CalJD_ZX_ARC(XY1,XY2,ResPt,ResPt1);
			else
			{
				//if (checkIfXJ(XY1,XY2,5))
				//{
				JDNUM = CalJD_ZX_Spiral(XY1,XY2,ResPt,ResPt1);
				//}	
				//else
				//	JDNUM=0;
			}
		}
		else if(XY1[0]<2.1)//圆
		{
			if(XY2[0]<1.1)//直线
				JDNUM = CalJD_ZX_ARC(XY2,XY1,ResPt,ResPt1);
			else if(XY2[0]<2.1)//圆
				JDNUM = CalJD_Arc_Arc(XY1,XY2,ResPt,ResPt1);
			else
			{
				//if (checkIfXJ(XY1,XY2,5))
				//{
				JDNUM = CalJD_ARC_Spiral(XY1,XY2,ResPt,ResPt1);	
				//}	
				//else
				//	JDNUM=0;
			}

		}
		else
		{
			if(XY2[0]<1.1)//直线
			{
				//if (checkIfXJ(XY1,XY2,5))
				//{
				JDNUM = CalJD_ZX_Spiral(XY2,XY1,ResPt,ResPt1);
				/*}*/	
				//else
				//	JDNUM=0;
			}

			else if(XY2[0]<2.1)//圆
			{
				//if (checkIfXJ(XY1,XY2,5))
				//{
				JDNUM = CalJD_ARC_Spiral(XY2,XY1,ResPt,ResPt1);
				//}	
				//else
				//	JDNUM=0;
			}
			else
			{
				if (checkIfXJ(XY1,XY2,1))
				{
					JDNUM = CalJD_Spiral_Spiral(XY1,XY2,ResPt,ResPt1);
				}	
				else
					JDNUM=0;
			}		
		}
	}
	if (JDNUM>1)
	{
		double dist=sqrt((ResPt[0]-ResPt1[0])*(ResPt[0]-ResPt1[0])+(ResPt[1]-ResPt1[1])*(ResPt[1]-ResPt1[1]));
		if (dist<0.001)
		{
			JDNUM=1;
		}
	}
	return JDNUM;
}
int BAS_DRAW_FUN_RAIL::CalJD_ZX_ZX(double Line1_array[10],double Line2_array[10],double ResPt[2])
{
	int JDNUM;
	JDNUM = 0;
	ResPt[0]=ResPt[1]=0.0;
	double Spt1[2],Ept1[2],Spt2[2],Ept2[2];
	Spt1[0]=Line1_array[8],Spt1[1]=Line1_array[9];
	Ept1[0]=Spt1[0]+Line1_array[1]*cos(Line1_array[5]);
	Ept1[1]=Spt1[1]+Line1_array[1]*sin(Line1_array[5]);
	Spt2[0]=Line2_array[8],Spt2[1]=Line2_array[9];
	Ept2[0]=Spt2[0]+Line2_array[1]*cos(Line2_array[5]);
	Ept2[1]=Spt2[1]+Line2_array[1]*sin(Line2_array[5]);

	if(INTERS(&ResPt[0],&ResPt[1],Spt1[0],Spt1[1],Ept1[0],Ept1[1],Spt2[0],Spt2[1],Ept2[0],Ept2[1])==0)
		JDNUM = 1;//找到

	//	if(JDNUM>0)
	//	{
	//	  ResPt[3]=Line1_array[6] + sqrt((ResPt[0]-Spt1[0])*(ResPt[0]-Spt1[0])+(ResPt[1]-Spt1[1])*(ResPt[1]-Spt1[1]));
	//	  ResPt[4]=Line2_array[6] + sqrt((ResPt[0]-Spt2[0])*(ResPt[0]-Spt2[0])+(ResPt[1]-Spt2[1])*(ResPt[1]-Spt2[1]));
	//	}
	return JDNUM;//没找到
}
int BAS_DRAW_FUN_RAIL::CalJD_ZX_ARC(double Line_array[10],double ARC_array[10],double ResPt[2],double ResPt1[2])
{
	int JDSum=0;;

	ResPt[0]= 0.0;
	ResPt[1]= 0.0;
	ResPt1[0]= 0.0;
	ResPt1[1]=0.0;
	JDSum = 0;

	AcGePoint2d Spt,Ept;
	double xmin,ymin,xmax,ymax;
	Spt.x=Line_array[8],Spt.y=Line_array[9];
	Ept.x=Spt.x+Line_array[1]*cos(Line_array[5]);
	Ept.y=Spt.y+Line_array[1]*sin(Line_array[5]);
	AcGeLine2d line(Spt,Ept);
	xmin = Spt.x < Ept.x ? Spt.x : Ept.x;
	ymin = Spt.y < Ept.y ? Spt.y : Ept.y;
	xmax = Spt.x > Ept.x ? Spt.x : Ept.x;
	ymax = Spt.y > Ept.y ? Spt.y : Ept.y;

	double R,Xo,Yo,x,y,zx,fwj,pt[3];
	R = ARC_array[1];
	x = ARC_array[8] , y= ARC_array[9];
	fwj = ARC_array[5];
	zx =  ARC_array[4];
	Xo = x + R*cos(fwj+0.5*pi*zx);
	Yo = y + R*sin(fwj+0.5*pi*zx);
	AcGePoint2d Arc_Spt,Arc_Ept,Arc_Mpt,jd1,jd2;

	Arc_Spt.set(x,y);
	GetXYonSomeXY(ARC_array,ARC_array[2],pt);
	Arc_Ept.set(pt[0],pt[1]);
	GetXYonSomeXY(ARC_array,0.5*ARC_array[2],pt);
	Arc_Mpt.set(pt[0],pt[1]);

	AcGeCircArc2d ARC(Arc_Spt,Arc_Mpt,Arc_Ept);

	ARC.intersectWith(line,JDSum,jd1,jd2);
	int njd;
	njd=0;
	if(JDSum>0)
	{
		if(jd1.x>xmin-0.001 && jd1.x<xmax+0.001
			&& jd1.y>ymin-0.001 && jd1.y<ymax+0.001)
		{
			njd++;
			ResPt[0] = jd1.x,ResPt[1] = jd1.y;
		}
		if(jd2.x>xmin-0.001 && jd2.x<xmax+0.001
			&& jd2.y>ymin-0.001 && jd2.y<ymax+0.001)
		{
			njd++;
			if(njd==2)
				ResPt1[0] = jd2.x,ResPt1[1] = jd2.y;
			else
				ResPt[0] = jd2.x,ResPt[1] = jd2.y;
		}
	}

	return njd;
}
int BAS_DRAW_FUN_RAIL::CalJD_ZX_Spiral(double Line_array[10],double Spiral_array[10],double ResPt[3],double ResPt1[3])
{
	double Spiral_Len=0;
	int JDNUM=0;
	CHhqx Hhqx1;
	double Rstart=10000000000000;
	double Rend=0;
	double Spiral1_A,Spiral1_Spt[3],Spiral1_Len,Spiral1_ZorY;
	//1.定义直线
	double Spt[2],Ept[2],pt[3],ptcz[2];
	Spt[0]=Spt[1]=0;
	Ept[0]=Ept[1]=0;
	pt[0]=pt[1]=pt[2]=0;
	ptcz[0]=ptcz[1]=0;
	Spt[0]=Line_array[8],Spt[1]=Line_array[9];
	Ept[0]=Spt[0]+Line_array[1]*cos(Line_array[5]);
	Ept[1]=Spt[1]+Line_array[1]*sin(Line_array[5]);
	pt[0]=0;
	pt[1]=0;
	pt[2]=0;
	double xmin,xmax,ymin,ymax;
	xmin=xmax=ymin=ymax=0;
	xmin = Spt[0] < Ept[0] ? Spt[0] : Ept[0];
	ymin = Spt[1] < Ept[1] ? Spt[1] : Ept[1];
	xmax = Spt[0] > Ept[0] ? Spt[0] : Ept[0];
	ymax = Spt[1] > Ept[1] ? Spt[1] : Ept[1];

	//计算缓长
	Spiral1_Len = Spiral_array[1]*Spiral_array[1]/Spiral_array[2]; 
	Spiral1_ZorY = Spiral_array[4];
	if(Spiral_array[0]<3.1)//前缓
	{
		//Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		Spiral1_Spt[0] = Spiral_array[8],Spiral1_Spt[1] = Spiral_array[9],Spiral1_Spt[2]=Spiral_array[5];
		//SpiralSL1=0.0;
		//SpiralEL1=Spiral1_Len;
		Rend=Spiral_array[2];
	}
	else if(Spiral_array[0]<4.1)//后缓
	{
		//Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		GetXYonSomeXY(Spiral_array,Spiral1_Len,Spiral1_Spt);
		Spiral1_Spt[2] += pi;
		Spiral1_ZorY = -1*Spiral1_ZorY;
		Rend=Spiral_array[2];
	}
	Hhqx1.Set(Spiral1_Spt[0],Spiral1_Spt[1],Spiral1_Spt[2],Spiral1_Len,Rstart,Rend,Spiral1_ZorY,0);
	Hhqx1.GetJdZxHhqx(ResPt[0],ResPt[1],ResPt[2],ResPt1[0],ResPt1[1],ResPt1[2],JDNUM,Spt[0],Spt[1],Ept[0],Ept[1]);
	////	BlockComputerFun ob;
	//double dis=100.0;
	//double len=0.0;
	//JDNUM = 0;
	////从起点开始
	//
	//while(len<Spiral_Len+0.001 && dis>0.0001)
	//{
	//	GetXYonSomeXY(Spiral_array,len,pt);
	//	// DistToLine(Spt[0],Spt[1],Ept[0],Ept[1],pt[0],pt[1],dis);
	//	 ChuizuToZx(ptcz[0],ptcz[1],pt[0],pt[1],Spt[0],Spt[1],Ept[0],Ept[1]);
	//	 dis=sqrt((ptcz[0]-pt[0])*(ptcz[0]-pt[0])+(ptcz[1]-pt[1])*(ptcz[1]-pt[1]));
	//	dis = fabs(dis);
	//	len = len + dis;
	//}
	//if(dis<=0.0001)
	//{
	//	if(pt[0]>xmin-0.001&&pt[0]<xmax+0.001&&pt[1]>ymin-0.001&&pt[1]<ymax+0.001)
	//	{
	//		JDNUM++;
	//		ResPt[0] = pt[0],ResPt[1] = pt[1];
	//	}		
	//}
	////从终点开始
	//len = Spiral_Len;
	//dis = 100;
	//while(len>=0.0 && dis>0.0001)
	//{
	//	GetXYonSomeXY(Spiral_array,len,pt);
	//	//DistToLine(Spt[0],Spt[1],Ept[0],Ept[1],pt[0],pt[1],dis);
	//	ChuizuToZx(ptcz[0],ptcz[1],pt[0],pt[1],Spt[0],Spt[1],Ept[0],Ept[1]);
	//	dis=sqrt((ptcz[0]-pt[0])*(ptcz[0]-pt[0])+(ptcz[1]-pt[1])*(ptcz[1]-pt[1]));
	//	dis = fabs(dis);
	//	len = len - dis;
	//}
	//if(dis<=0.0001)
	//{
	//	if(JDNUM>0)
	//		dis = sqrt((pt[0]-ResPt[0])*(pt[0]-ResPt[0])+(pt[1]-ResPt[1])*(pt[1]-ResPt[1]));
	//	if(JDNUM==0 || (JDNUM>0&&dis>0.001))
	//	{
	//		if(pt[0]>xmin-0.001&&pt[0]<xmax+0.001&&pt[1]>ymin-0.001&&pt[1]<ymax+0.001)
	//		{
	//			JDNUM++;
	//			ResPt1[0] = pt[0],ResPt1[1] = pt[1];
	//		}
	//	}
	//}


	return JDNUM;
}
int BAS_DRAW_FUN_RAIL::CalJD_Arc_Arc(double Arc1_array[10],double Arc2_array[10],double ResPt[3],double ResPt1[3])
{
	double pt[3];
	int JDNUM;
	AcGePoint2d Arc1_Spt,Arc1_Ept,Arc1_Mpt;
	pt[0]=pt[1]=pt[2]=0;
	JDNUM=0;
	Arc1_Spt.x=Arc1_Spt.y=0;
	Arc1_Ept.x=Arc1_Ept.y=0;
	Arc1_Mpt.x=Arc1_Mpt.y=0;

	Arc1_Spt.set(Arc1_array[9],Arc1_array[8]);
	GetXYonSomeXY(Arc1_array,Arc1_array[2],pt);
	Arc1_Ept.set(pt[1],pt[0]);
	GetXYonSomeXY(Arc1_array,0.5*Arc1_array[2],pt);
	Arc1_Mpt.set(pt[1],pt[0]);

	AcGePoint2d Arc2_Spt,Arc2_Ept,Arc2_Mpt;
	Arc2_Spt.x=Arc2_Spt.y=0;
	Arc2_Ept.x=Arc2_Ept.y=0;
	Arc2_Mpt.x=Arc2_Mpt.y=0;
	Arc2_Spt.set(Arc2_array[9],Arc2_array[8]);
	GetXYonSomeXY(Arc2_array,Arc2_array[2],pt);
	Arc2_Ept.set(pt[1],pt[0]);
	GetXYonSomeXY(Arc2_array,0.5*Arc2_array[2],pt);
	Arc2_Mpt.set(pt[1],pt[0]);

	AcGeCircArc2d ARC1(Arc1_Spt,Arc1_Mpt,Arc1_Ept);
	AcGeCircArc2d ARC2(Arc2_Spt,Arc2_Mpt,Arc2_Ept);
	AcGePoint2d jd1,jd2;
	jd1.x=jd1.y=0;
	jd2.x=jd2.y=0;
	ARC1.intersectWith(ARC2,JDNUM,jd1,jd2);
	if(JDNUM>0)
	{
		ResPt[0] = jd1.y,ResPt[1] = jd1.x;
		ResPt1[0] = jd2.y,ResPt1[1] = jd2.x;
	}

	return JDNUM;	
}
int BAS_DRAW_FUN_RAIL::CalJD_ARC_Spiral(double Arc_array[10],double Spiral_array[10],double ResPt[3],double ResPt1[3])
{
	double Arc_Len;
	double Spiral_Len;
	int JDNUM;

	double  R,Xo,Yo,x,y,zx,fwj,fwj2,pt[3];
	CHhqx Hhqx1;
	double Rstart=10000000000000;
	double Rend=0;
	double Spiral1_A,Spiral1_Spt[3],Spiral1_Len,Spiral1_ZorY;
	R = Arc_array[1];
	Arc_Len = Arc_array[2];
	x = Arc_array[8] , y= Arc_array[9];
	fwj = Arc_array[5];
	zx =  Arc_array[4];
	Xo = x + R*cos(fwj+0.5*pi*zx);
	Yo = y + R*sin(fwj+0.5*pi*zx);
	fwj2= fwj+(Arc_Len/R)*zx;
	JDNUM = 0;
	ResPt[0]=ResPt[1]=ResPt[2]=0.0;
	ResPt1[0]=ResPt1[1]=ResPt1[2]=0.0;

	//if(Spiral_array[0]<4.1)//前缓
	//  Spiral_Len = Spiral_array[1]*Spiral_array[1]/Spiral_array[2];   
	//else if(Spiral_array[0]<6.1)//不完整缓(R大->R小)
	//  Spiral_Len = fabs(Spiral_array[1]*Spiral_array[1]/Spiral_array[3]-Spiral_array[1]*Spiral_array[1]/Spiral_array[2]);   	   

	BlockComputerFun ob;
	double dis=100.0;
	double len=0.0;
	JDNUM = 0;
	Spiral1_Len = Spiral_array[1]*Spiral_array[1]/Spiral_array[2]; 
	Spiral1_ZorY = Spiral_array[4];
	if(Spiral_array[0]<3.1)//前缓
	{
		//Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		Spiral1_Spt[0] = Spiral_array[8],Spiral1_Spt[1] = Spiral_array[9],Spiral1_Spt[2]=Spiral_array[5];
		//SpiralSL1=0.0;
		//SpiralEL1=Spiral1_Len;
		Rend=Spiral_array[2];
	}
	else if(Spiral_array[0]<4.1)//后缓
	{
		//Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		GetXYonSomeXY(Spiral_array,Spiral1_Len,Spiral1_Spt);
		Spiral1_Spt[2] += pi;
		Spiral1_ZorY = -1*Spiral1_ZorY;
		Rend=Spiral_array[2];
	}
	Hhqx1.Set(Spiral1_Spt[0],Spiral1_Spt[1],Spiral1_Spt[2],Spiral1_Len,Rstart,Rend,Spiral1_ZorY,0);
	Hhqx1.GetJdArcHhqx(ResPt[0],ResPt[1],ResPt1[0],ResPt1[1],JDNUM,Xo,Yo,R,fwj,fwj2);

	return JDNUM;	
}

int BAS_DRAW_FUN_RAIL::CalJD_Spiral_Spiral(double Spiral1_array[10],double Spiral2_array[10],double ResPt[3],double ResPt1[3])
{
	//int Njd=0;
	////	double LtoBasePt1,LtoBasePt2;
	//double Np1,Ep1,Np2,Ep2;
	CHhqx Hhqx1,Hhqx2;
	//double Hhqx1_StartFwj,Hhqx1_L0,Hhqx1_Rstart,Hhqx1_Rend,Hhqx1_StartN,Hhqx1_StartE;
	//double Hhqx2_StartFwj,Hhqx2_L0,Hhqx2_Rstart,Hhqx2_Rend,Hhqx2_StartN,Hhqx2_StartE;
	//int Hhqx1_Mod,Hhqx1_LeftorRight,Hhqx2_Mod,Hhqx2_LeftorRight;
	//Hhqx1_StartFwj=
	double Rstart=10000000000000;
	double Rend=0;
	double Spiral1_A,Spiral1_Spt[3],Spiral1_Len,Spiral1_ZorY;
	double Spiral2_A,Spiral2_Spt[3],Spiral2_Len,Spiral2_ZorY;
	double l2_spiral1,l2_spiral2;
	double l1_spiral1,l1_spiral2;
	int inf=0;
	int JDNUM;
	double SpiralSL1,SpiralEL1;
	double SpiralSL2,SpiralEL2;
	Spiral1_A=Spiral1_Len=Spiral1_ZorY=0;
	Spiral2_A=Spiral2_Len=Spiral2_ZorY=0;
	l2_spiral1=l2_spiral2=0;
	l1_spiral1=l1_spiral2=0;
	Spiral1_Spt[0]=Spiral1_Spt[1]=Spiral1_Spt[2]=0;
	Spiral2_Spt[0]=Spiral2_Spt[1]=Spiral2_Spt[2]=0;
	SpiralSL1=SpiralEL1=0;
	SpiralSL2=SpiralEL2=0;
	JDNUM = 0;
	ResPt[0]=ResPt[1]=ResPt[2]=0.0;
	ResPt1[0]=ResPt1[1]=ResPt1[2]=0.0;

	Spiral1_A = Spiral1_array[1];
	Spiral1_ZorY = Spiral1_array[4];
	if(Spiral1_array[0]<3.1)//前缓
	{
		Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		Spiral1_Spt[0] = Spiral1_array[8],Spiral1_Spt[1] = Spiral1_array[9],Spiral1_Spt[2]=Spiral1_array[5];
		SpiralSL1=0.0;
		SpiralEL1=Spiral1_Len;
		Rend=Spiral1_array[2];
	}
	else if(Spiral1_array[0]<4.1)//后缓
	{
		Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		GetXYonSomeXY(Spiral1_array,Spiral1_Len,Spiral1_Spt);
		Spiral1_Spt[2] += pi;
		Spiral1_ZorY = -1*Spiral1_ZorY;
		SpiralSL1=0.0;
		SpiralEL1=Spiral1_Len;
		Rend=Spiral1_array[2];
	}
	else if(Spiral1_array[0]<5.1)//不完整缓(R大->R小)
	{
		BlockComputerFun fun;
		fun.GetOrignCorOfSprial(Spiral1_array[8],Spiral1_array[9],Spiral1_array[2],
			Spiral1_array[5],Spiral1_array[1],Spiral1_array[4],LargeToSmall,
			Spiral1_Spt[0],Spiral1_Spt[1],Spiral1_Spt[2]);
		Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[3];   	   
		SpiralSL1=Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];
		SpiralEL1=Spiral1_Len;
		Rend=Spiral1_array[2];
	}
	else//不完整缓(R小->R大)
	{
		BlockComputerFun fun;
		fun.GetOrignCorOfSprial(Spiral1_array[8],Spiral1_array[9],Spiral1_array[3],
			Spiral1_array[5],Spiral1_array[1],Spiral1_array[4],SmallToLarge,
			Spiral1_Spt[0],Spiral1_Spt[1],Spiral1_Spt[2]);
		//Spiral1_Spt[2] += pi;
		Spiral1_ZorY = -1*Spiral1_ZorY; 	
		Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[3];   	   
		SpiralSL1=Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];
		SpiralEL1=Spiral1_Len;
		Rend=Spiral1_array[2];
	}
	Hhqx1.Set(Spiral1_Spt[0],Spiral1_Spt[1],Spiral1_Spt[2],Spiral1_Len,Rstart,Rend,Spiral1_ZorY,0);
	Rend=0;
	Spiral2_A = Spiral2_array[1];
	Spiral2_ZorY = Spiral2_array[4];
	if(Spiral2_array[0]<3.1)//前缓
	{
		Spiral2_Len = Spiral2_array[1]*Spiral2_array[1]/Spiral2_array[2];   
		Spiral2_Spt[0] = Spiral2_array[8],Spiral2_Spt[1] = Spiral2_array[9],Spiral2_Spt[2]=Spiral2_array[5];
		SpiralSL2=0.0;
		SpiralEL2=Spiral2_Len;
		Rend=Spiral2_array[2];
	}
	else if(Spiral2_array[0]<4.1)//后缓
	{
		Spiral2_Len = Spiral2_array[1]*Spiral2_array[1]/Spiral2_array[2];   
		GetXYonSomeXY(Spiral2_array,Spiral2_Len,Spiral2_Spt);
		Spiral2_Spt[2] += pi;
		Spiral2_ZorY = -1*Spiral2_ZorY;
		SpiralSL2=0.0;
		SpiralEL2=Spiral2_Len;
		Rend=Spiral2_array[2];
	}
	else if(Spiral2_array[0]<5.1)//不完整缓(R大->R小)
	{
		BlockComputerFun fun;
		fun.GetOrignCorOfSprial(Spiral2_array[8],Spiral2_array[9],Spiral2_array[2],
			Spiral2_array[5],Spiral2_array[1],Spiral2_array[4],LargeToSmall,
			Spiral2_Spt[0],Spiral2_Spt[1],Spiral2_Spt[2]);
		Spiral2_Len = Spiral2_array[1]*Spiral2_array[1]/Spiral2_array[3];   	   
		SpiralSL2=Spiral2_array[1]*Spiral2_array[1]/Spiral2_array[2];
		SpiralEL2=Spiral2_Len;
		Rend=Spiral2_array[2];
	}
	else//不完整缓(R小->R大)
	{
		BlockComputerFun fun;
		fun.GetOrignCorOfSprial(Spiral2_array[8],Spiral2_array[9],Spiral2_array[3],
			Spiral2_array[5],Spiral2_array[1],Spiral2_array[4],SmallToLarge,
			Spiral2_Spt[0],Spiral2_Spt[1],Spiral2_Spt[2]);
		Spiral2_Spt[2] += pi;
		Spiral2_ZorY = -1*Spiral2_ZorY; 	
		Spiral2_Len = Spiral2_array[1]*Spiral2_array[1]/Spiral2_array[3];   	   
		SpiralSL2=Spiral2_array[1]*Spiral2_array[1]/Spiral2_array[2];
		SpiralEL2=Spiral2_Len;
		Rend=Spiral2_array[2];
	}
	Hhqx2.Set(Spiral2_Spt[0],Spiral2_Spt[1],Spiral2_Spt[2],Spiral2_Len,Rstart,Rend,Spiral2_ZorY,0);
	// CArray<CBasicFun::Jd,CBasicFun::Jd> HHJd;
	Hhqx1.GetJdHhqxHhqx( ResPt[0],ResPt[1],ResPt[2],ResPt1[0],ResPt1[1],ResPt1[2],JDNUM,Hhqx1,Hhqx2,0,Spiral1_Len,0,Spiral2_Len);
	/* if(Hhqx1.GetJdHhqxHhqxHuitou(HHJd,Hhqx1,Hhqx2))
	{
	if(HHJd.GetSize( ResPt[0],)==1)
	{
	JDNUM=1;
	ResPt[0]=HHJd[0].dNJd;
	ResPt[1]=HHJd[0].dEJd;
	ResPt[2]=0;
	}
	else if (HHJd.GetSize()==2)
	{
	JDNUM=2;
	ResPt[0]=HHJd[0].dNJd;
	ResPt[1]=HHJd[0].dEJd;
	ResPt[2]=0;
	ResPt1[0]=HHJd[1].dNJd;
	ResPt1[1]=HHJd[1].dEJd;
	ResPt1[2]=0;
	}
	else
	{
	JDNUM=0;
	}
	}*/

	//l2_spiral1=Spiral1_Len;
	//l2_spiral2=Spiral2_Len;
	////	l2_spiral1=0.5*Spiral1_Len;
	////	l2_spiral2=0.5*Spiral2_Len;

	//inf = CalJD_Spiral_Spiral_diedai(l2_spiral1,l2_spiral2,Spiral1_A,Spiral1_Spt,Spiral1_Len,Spiral1_ZorY,
	//	Spiral2_A,Spiral2_Spt,Spiral2_Len,Spiral2_ZorY);
	//if(inf==0)
	//{
	//	l2_spiral1=0.0;
	//	l2_spiral2=0.0;
	//	inf = CalJD_Spiral_Spiral_diedai(l2_spiral1,l2_spiral2,Spiral1_A,Spiral1_Spt,Spiral1_Len,Spiral1_ZorY,
	//		Spiral2_A,Spiral2_Spt,Spiral2_Len,Spiral2_ZorY);	
	//}

	////ads_printf(L"inf=%d %lf %lf %lf\n",inf,l2_spiral1,SpiralSL1,SpiralEL1);
	//if(inf && l2_spiral1>SpiralSL1-0.0001 && l2_spiral1<SpiralEL1+0.0001 &&
	//	l2_spiral2>SpiralSL2-0.0001 && l2_spiral2<SpiralEL2+0.0001)//有交点,第一次迭代结束
	//{
	//	if (fabs(Spiral1_array[0]-3.0)<0.1||fabs(Spiral1_array[0]-5.0)<0.1)
	//	  inf = GetXYonSomeXY(Spiral1_array,l2_spiral1-SpiralSL1,ResPt);
	//	else
	//	  inf = GetXYonSomeXY(Spiral1_array,SpiralEL1-l2_spiral1,ResPt);
	//	
	//	if(inf)//找到一个交点,再找第二个
	//	{
	//		JDNUM = 1;
	//		l1_spiral1 = l2_spiral1;
	//		l1_spiral2 = l2_spiral2;

	//		inf = CalJD_Spiral_Spiral_diedai(l2_spiral1,l2_spiral2,Spiral1_A,Spiral1_Spt,Spiral1_Len,Spiral1_ZorY,
	//			Spiral2_A,Spiral2_Spt,Spiral2_Len,Spiral2_ZorY);
	//		if(inf && l2_spiral1>SpiralSL1-0.0001 && l2_spiral1<SpiralEL1+0.0001 &&
	//			l2_spiral2>SpiralSL2-0.0001 && l2_spiral2<SpiralEL2+0.0001)
	//		{
	//			if(fabs(l2_spiral1-l1_spiral1)<0.001 && fabs(l2_spiral2-l1_spiral2)<0.001)//同一交点
	//				return JDNUM;
	//			if (fabs(Spiral1_array[0]-3.0)<0.1||fabs(Spiral1_array[0]-5.0)<0.1)
	//				inf = GetXYonSomeXY(Spiral1_array,l2_spiral1-SpiralSL1,ResPt);
	//		    else
	//			    inf = GetXYonSomeXY(Spiral1_array,SpiralEL1-l2_spiral1,ResPt);
	//			if(inf)//找到一个交点,再找第二个
	//				JDNUM = 2;
	//		}
	//	}
	//}


	return JDNUM;	
}
int BAS_DRAW_FUN_RAIL::GetXYonSomeXY(double array2[10],double jslen,double respt[3])
{
	double INDATA2[5];
	xlpoint *PBI,*PZ;

	PBI=PZ=new xlpoint;
	for (int k=0;k<5;k++) 
		INDATA2[k]=array2[k];
	int j;
	double lc;
	double l=Get_INDATA_L(INDATA2,&j);
	PBI->a  = array2[5];
	PBI->lc = array2[6];
	PBI->r  = array2[7];
	PBI->x  = array2[8];
	PBI->y  = array2[9];
	// l 为该线元的长度 m

	double jslc;
	jslc = PBI->lc+jslen;
	// 计算点在i线元上
	if((jslc<=PBI->lc+l) && (jslc>=PBI->lc) ) 
	{  
		lc=jslc-PBI->lc;
		if (lc<=0.0) lc=0.000000000001;
		xlzbxy(PBI,PZ,lc,INDATA2,0);
		respt[0] = PZ->x,respt[1] = PZ->y,respt[2] = PZ->a;
		return 1;
	}
	else
		respt[0] = 0.0,respt[1] = 0.0,respt[2] = 0.0;
	return 0;
}
bool BAS_DRAW_FUN_RAIL::CheckIfOnArc(double array[10],double pt[3])
{
	bool inf;
	inf=false;
	double fwj1,fwj2,fwj,fwjmin,fwjmax;

	double Arc_Len,R,Xo,Yo,x,y,zx,zj;
	R = array[1];
	Arc_Len = array[2];
	x = array[8] , y= array[9];
	fwj = array[5];
	zx =  array[4];
	Xo = x + R*cos(fwj+0.5*pi*zx);
	Yo = y + R*sin(fwj+0.5*pi*zx);

	xyddaa(Xo,Yo,x,y,&fwj1);
	StdJiao(fwj1);
	xyddaa(Xo,Yo,pt[0],pt[1],&fwj);
	StdJiao(fwj);

	double angle;
	angle=ZXJ(fwj,fwj1);

	zj = zx*Arc_Len/R;
	//fwj2 = fwj1+zj;
	//StdJiao(fwj2);
	/*fwjmin = fwj1 < fwj2 ? fwj1 : fwj2;
	fwjmax = fwj1 > fwj2 ? fwj1 : fwj2;
	ads_printf(L"fwj=%lf fwj1=%lf fwj2=%lf fwjmax=%lf Arc_len=%lf R=%lf zj=%lf\n",fwj,fwj1,fwj2,fwjmax, Arc_Len, R,zj);
	if(fwj>fwjmin-0.00001 && fwj<fwjmax+0.00001)
	return true;*/
	if (fabs(angle)<fabs(zj)+0.0001&&angle*zj>-0.0001)
	{
		return true;
	}
	return false;
}
//void BAS_DRAW_FUN::GetOrignCorOfSprial(double SprX,double SprY,double SprRad,
//										   double SprOrient,double ValueA,int Deflect,int RadForward,
//										   double &OrignX,double &OrignY,double &OrignOrient)
//{
//	//如果当前缓曲线是LargeToSmall或BigToSmall;
//	if (RadForward==LargeToSmall || RadForward==BigToSmall) 
//	{
//		//如果当前给定点即为源点;
//		if (SprRad ==1E50)
//		{
//			OrignX = SprX;
//			OrignY = SprY;
//			OrignOrient = SprOrient;
//		}
//		else
//		{		
//			//定义曲长变量;
//			double CirLen = ValueA*ValueA/SprRad;
//			//获取曲线源定点的偏角;
//			double DivAngle = CirLen/(2*SprRad);
//			//获取源点的方位角;
//			OrignOrient = SprOrient - Deflect*DivAngle;
//			//获取源点的坐标转换角;
//			double Angle = ConvertOrientToAngle(OrignOrient);
//			//获得给定点的相对坐标;
//			double InterX,InterY;
//			InterX = GetAbsXCorOfSprialCir(ValueA,CirLen);
//			InterY = GetAbsYCorOfSprialCir(ValueA,CirLen);
//			//计算源点坐标;
//			OrignX = SprX - InterX*sin(Angle)+Deflect*InterY*cos(Angle);
//			OrignY = SprY - InterX*cos(Angle)-Deflect*InterY*sin(Angle);
//		}
//	}
//	//如果当前缓曲线是SmallToLarge或SmallToBig;
//	else 
//	{
//		if (SprRad ==1E50)
//		{
//			OrignX = SprX;
//			OrignY = SprY;
//			OrignOrient = SprOrient;
//		}
//		else
//		{		
//			//定义曲长变量;
//			double CirLen = ValueA*ValueA/SprRad;
//			//获取曲线源定点的偏角;
//			double DivAngle = CirLen/(2*SprRad);
//			//获取源点的方位角;
//			OrignOrient = SprOrient + Deflect*DivAngle;
//			//获取源点的坐标转换角;
//			double Angle = ConvertOrientToAngle(OrignOrient)- Deflect*PI;
//			//获得给定点的相对坐标;
//			double InterX,InterY;
//			InterX = GetAbsXCorOfSprialCir(ValueA,CirLen);
//			InterY = GetAbsYCorOfSprialCir(ValueA,CirLen);
//			//计算源点坐标;
//			OrignX = SprX - InterX*sin(Angle)-/*+*/Deflect*InterY*cos(Angle);
//			OrignY = SprY - InterX*cos(Angle)+/*-*/Deflect*InterY*sin(Angle);
//		}		
//	}
//
//	return ;
//}
int BAS_DRAW_FUN_RAIL::CalJD_Spiral_Spiral_diedai(double &l2_spiral1,double &l2_spiral2,double Spiral1_A,double Spiral1_Spt[3],double Spiral1_Len,double Spiral1_ZorY,
												  double Spiral2_A,double Spiral2_Spt[3],double Spiral2_Len,double Spiral2_ZorY)
{
	double x1_spiral1,y1_spiral1;//spiral
	double x1_spiral2,y1_spiral2;//spiral
	double l1_spiral1,l1_spiral2;

	double Spiral1_R,Spiral2_R;
	double x1,x2;
	x1_spiral1=0;
	y1_spiral1=0;
	x1_spiral2=0;
	y1_spiral2=0;
	l1_spiral1=0;
	l1_spiral2=0;
	Spiral1_R=0;
	Spiral2_R=0;
	x1=0;
	x2=0;
	//while((Spiral1_Spt[2]-pi/2.0)>0&&(Spiral2_Spt[2]-pi/2.0)>0)
	//{
	//	Spiral1_Spt[2]-=pi/2.0;
	//	Spiral2_Spt[2]-=pi/2.0;
	//}
	//1.给出初始值l1
	//	l2_arc=l2_spiral=0.0;
	//	l1_spiral1=Spiral1_Len;
	//	l1_spiral2=Spiral2_Len;
	if(l2_spiral1>0.001)
		l1_spiral1=0.0;
	else
		l1_spiral1=Spiral1_Len;
	if(l2_spiral2>0.001)
		l1_spiral2=0.0;
	else
		l1_spiral2=Spiral2_Len;
	//2.计算x1,y1
	//2.1计算圆x1,y1
	// l 为该线元的长度 m
	//while(l2_spiral1>-0.00001&& l2_spiral1<Spiral1_Len+0.0001&&l2_spiral2>-0.00001&& l2_spiral2<Spiral2_Len+0.0001 && fabs(l1_spiral1-l2_spiral1)>0.0001 && fabs(l1_spiral2-l2_spiral2)>0.0001)
	int xuhan=0;
	while(fabs(l1_spiral1-l2_spiral1)>0.0001 || fabs(l1_spiral2-l2_spiral2)>0.0001)
	{
		l1_spiral1 = l2_spiral1;
		l1_spiral2 = l2_spiral2;
		//ads_printf(L"1:%lf %lf\n",l1_spiral1,l1_spiral2);		
		if(l1_spiral1>0.0001)
			Spiral1_R = Spiral1_A*Spiral1_A/l1_spiral1;
		else
			Spiral1_R = 1.0E50;

		//2.2计算缓和曲线x1,y1
		if(l1_spiral2>0.0001)
			Spiral2_R = Spiral2_A*Spiral2_A/l1_spiral2;
		else
			Spiral2_R = 1.0E50;

		x1_spiral1=l1_spiral1-pow(l1_spiral1/Spiral1_R,2)*l1_spiral1/40.0+pow(l1_spiral1/Spiral1_R,4)*l1_spiral1/3456.0-pow(l1_spiral1/Spiral1_R,6)*l1_spiral1/599040.0;
		y1_spiral1=pow(l1_spiral1,2)/6.0/Spiral1_R-pow(l1_spiral1/Spiral1_R,3)*l1_spiral1/336.0+pow(l1_spiral1/Spiral1_R,5)*l1_spiral1/42240.0 ;
		y1_spiral1=Spiral1_ZorY*fabs(y1_spiral1);

		x1_spiral2=l1_spiral2-pow(l1_spiral2/Spiral2_R,2)*l1_spiral2/40.0+pow(l1_spiral2/Spiral2_R,4)*l1_spiral2/3456.0-pow(l1_spiral2/Spiral2_R,6)*l1_spiral2/599040.0;
		y1_spiral2=pow(l1_spiral2,2)/6.0/Spiral2_R-pow(l1_spiral2/Spiral2_R,3)*l1_spiral2/336.0+pow(l1_spiral2/Spiral2_R,5)*l1_spiral2/42240.0;
		y1_spiral2=Spiral2_ZorY*fabs(y1_spiral2);

		//3.计算新的x2,y2
		x1 = (Spiral2_Spt[0] - Spiral1_Spt[0] + x1_spiral2*cos(Spiral2_Spt[2])-y1_spiral2*sin(Spiral2_Spt[2])+y1_spiral1*sin(Spiral1_Spt[2]))/cos(Spiral1_Spt[2]);
		//    x1 = (Spiral2_Spt[1] - Spiral1_Spt[1] + x1_spiral2*sin(Spiral2_Spt[2])+y1_spiral2*cos(Spiral2_Spt[2])-y1_spiral1*cos(Spiral1_Spt[2]))/sin(Spiral1_Spt[2]);
		x2 = (Spiral1_Spt[1] - Spiral2_Spt[1] + x1_spiral1*sin(Spiral1_Spt[2])+y1_spiral1*cos(Spiral1_Spt[2])-y1_spiral2*cos(Spiral2_Spt[2]))/sin(Spiral2_Spt[2]);
		//ads_printf(L"spt2.x=%lf spt1.x=%lf x1=%lf x2=%lf y1=%lf y2=%lf fwj1=%lf fwj2=%lf x1=%lf x2=%lf cosa1=%lf cosa2=%lf %lf %lf %lf %lf\n",
		//		   Spiral1_Spt[0],Spiral2_Spt[0],
		//		   x1_spiral1,x1_spiral2,
		//		   y1_spiral1,y1_spiral2,
		//		   Spiral1_Spt[2],Spiral2_Spt[2],x1,x2,cos(Spiral1_Spt[2]),cos(Spiral2_Spt[2]),
		//		   Spiral2_Spt[0] - Spiral1_Spt[0], x1_spiral2*cos(Spiral2_Spt[2]),-y1_spiral2*sin(Spiral2_Spt[2]),y1_spiral1*sin(Spiral1_Spt[2]));		
		//
		//4.计算新的l2
		l2_spiral1=fabs(x1) + pow(l1_spiral1/Spiral1_R,2)*l1_spiral1/40.0-pow(l1_spiral1/Spiral1_R,4)*l1_spiral1/3456.0+pow(l1_spiral1/Spiral1_R,6)*l1_spiral1/599040.0;
		l2_spiral2=fabs(x2) + pow(l1_spiral2/Spiral2_R,2)*l1_spiral2/40.0-pow(l1_spiral2/Spiral2_R,4)*l1_spiral2/3456.0+pow(l1_spiral2/Spiral2_R,6)*l1_spiral2/599040.0;
		xuhan++;
		//if (xuhan>1000000)
		//{
		//	return 0;
		//}
	}

	//5.比较l1,l2

	if(l2_spiral1>-0.001 && l2_spiral1<Spiral1_Len+0.001 && l2_spiral2>-0.001 && l2_spiral2<Spiral2_Len+0.001 
		&& fabs(l1_spiral1-l2_spiral1)<=0.001 &&
		fabs(l1_spiral2-l2_spiral2)<=0.001)
	{
		ads_printf(L":%lf %lf %lf %lf %lf %lf\n",l2_spiral1,l2_spiral2,Spiral1_Len,Spiral2_Len,fabs(l1_spiral1-l2_spiral1),fabs(l1_spiral2-l2_spiral2));
		return 1;
	}

	return 0;
}

////将方位角转化成辐角;
//double BlockComputerFun::ConvertOrientToAngle(double ValueOrient)
//{
//	ValueOrient = ValueOrient*180/PI;
//	//方位角的范围是:0~360度;
//	//如果给定的方位角在0~90度间(相当于辐角的第一象限);
//	if (ValueOrient>0 && ValueOrient<=90) 
//		return (90-ValueOrient)*PI/180;
//	//如果给定的方位角在90~360度间(相当于辐角的第四,三,二象限);
//	else 
//		return (450-ValueOrient)*PI/180;
//}

double BAS_DRAW_FUN_RAIL::distfwj(double xb,double yb,double xe,double ye,double *f)//与Y轴夹角f,返回两点之距
{
	AcGePoint3d sPt(xb,yb,0),ePt(xe,ye,0);
	AcDbLine *pL=new AcDbLine(sPt,ePt);
	double ang=angleX(pL);//与X轴夹角
	ang=2.5*PI-ang;
	if(ang>2*PI)ang-=2*PI;
	*f=ang;
	return sqrt((xb-xe)*(xb-xe)+(yb-ye)*(yb-ye));
}

double BAS_DRAW_FUN_RAIL::angleX(AcDbLine *pL)
{
	AcGePoint3d sPtL(pL->startPoint()),ePtL(pL->endPoint());
	ads_point spt,ept;
	spt[X]=sPtL.x;
	spt[Y]=sPtL.y;
	ept[X]=ePtL.x;
	ept[Y]=ePtL.y;
	double ang=ads_angle(spt,ept);//与X轴夹角
	return ang;
}

double BAS_DRAW_FUN_RAIL::angleX(AcGePoint3d sPtL,AcGePoint3d ePtL)//Line(sPtl,ePtl)
{
	ads_point spt,ept;
	spt[X]=sPtL.x;
	spt[Y]=sPtL.y;
	ept[X]=ePtL.x;
	ept[Y]=ePtL.y;
	double ang=ads_angle(spt,ept);//与X轴夹角
	return ang;
}

//从CurID中得到线元数据 0 无扩展数据 1有扩展数据
int BAS_DRAW_FUN_RAIL::GetXdataFromEntity(AcDbObjectId CurID,XYarray &xray)
{
	AcDbObject* pCurObj;
	Acad::ErrorStatus res;

	struct  resbuf  *pRb=NULL, *pTemp=NULL;
	int i;
	AcDbHandle handle;

	res = acdbOpenObject(pCurObj, CurID, AcDb::kForRead);	
	ads_printf(L"\nAcDbEntity name=%s",pCurObj->isA()->name());
	pRb = pCurObj->xData();
	if(pRb==NULL)
	{
		pCurObj->close();
		return 0;
	}
	for(i=0;i<10;i++)
		xray.xyarr[i]=0;
	if(res==Acad::eOk)
	{
		if(pCurObj->isKindOf(AcDbPolyline::desc())
			|| pCurObj->isKindOf(AcDbLine::desc())
			|| pCurObj->isKindOf(AcDbArc::desc())
			|| pCurObj->isKindOf(AcDbCircle::desc()) 
			)
		{   
			xray.eID = CurID;
			if (pCurObj->isKindOf(AcDbPolyline::desc())) //缓和曲线
			{  
				pCurObj->close();

				i = 0;
				pTemp = pRb->rbnext->rbnext->rbnext;
				while (pTemp) 
				{
					xray.xyarr[i] = pTemp->resval.rreal;
					pTemp = pTemp->rbnext;
					i++;
					if(i==10)
						break;
				}			   
			}
			else if  (pCurObj->isKindOf(AcDbLine::desc())) //直线
			{     
				pCurObj->close();
				AcDbLine *LINE=AcDbLine::cast(pCurObj);
				AcGePoint3d sPt,ePt;
				//			   acdbOpenObject(LINE,eId,AcDb::kForWrite);		
				sPt=LINE->startPoint();
				ePt=LINE->endPoint();

				xray.xyarr[0] = 1;
				xray.xyarr[1] = xyddaa(sPt.y,sPt.x,ePt.y,ePt.x,&xray.xyarr[5]);
				xray.xyarr[2]=xray.xyarr[3]=xray.xyarr[4] = 0;
				xray.xyarr[7]=1e40;
				xray.xyarr[8]=sPt.y;
				xray.xyarr[9]=sPt.x;
			}
			else if(pCurObj->isKindOf(AcDbArc::desc()))//plh07.17圆曲线
			{
				pCurObj->close();			   
				AcDbArc *ARC=AcDbArc::cast(pCurObj);
				//			   acdbOpenObject(ARC,eId,AcDb::kForWrite);
				double j1,j2,x0,y0,R,yxj;
				j1=ARC->startAngle();
				j2=ARC->endAngle();
				x0=ARC->center().y;
				y0=ARC->center().x;
				R=ARC->radius();
				//			   ARC->close();
				i = 0;
				//			   	xray.xyarr[4]=-1;
				pTemp = pRb->rbnext->rbnext->rbnext;
				/*
				while (pTemp) 
				{
				pTemp = pTemp->rbnext;
				i++;
				if(i==4)
				{
				xray.xyarr[4] = pTemp->resval.rreal;
				ads_printf(L"ccc=%lf\n",xray.xyarr[4]);
				break;
				}
				}*/
				while (pTemp) 
				{
					xray.xyarr[i] = pTemp->resval.rreal;
					pTemp = pTemp->rbnext;
					i++;
					if(i==10)
						break;
				}			   

				double fwj1,fwj2;
				fwj1=FwjtoAngX(j1);
				fwj2=FwjtoAngX(j2);			   
				xray.xyarr[0] = 2;
				xray.xyarr[1] = R;
				yxj = j2 - j1;
				if(yxj<=0)
					yxj = yxj+2*PI;
				xray.xyarr[2]= R*yxj;
				xray.xyarr[3] = 0;
				xray.xyarr[7]=R;

				//求圆弧的起终点坐标
				if(xray.xyarr[4] < 0)
				{
					xray.xyarr[8]=x0+R*cos(fwj1);
					xray.xyarr[9]=y0+R*sin(fwj1);	
					xray.xyarr[5]=fwj1-0.5*PI;
					if(xray.xyarr[5]<0)
						xray.xyarr[5]+=2*PI;
				}
				else
				{
					xray.xyarr[8]=x0+R*cos(fwj2);
					xray.xyarr[9]=y0+R*sin(fwj2);	
					xray.xyarr[5]=fwj2+0.5*PI;
					if(xray.xyarr[5]>2*PI)
						xray.xyarr[5]-=2*PI;
				}
			}
			else if(pCurObj->isKindOf(AcDbCircle::desc()))//plh07.17
			{
				double R;
				pCurObj->close();			   
				AcDbCircle *Cir=AcDbCircle::cast(pCurObj);
				R=Cir->radius();
				i = 0;
				//			   xray.xyarr[4]=-1;
				pTemp = pRb->rbnext->rbnext->rbnext;
				/*
				while (pTemp) 
				{
				pTemp = pTemp->rbnext;
				i++;
				if(i==4)
				{
				xray.xyarr[4] = pTemp->resval.rreal;
				break;
				}
				}*/
				while (pTemp) 
				{
					xray.xyarr[i] = pTemp->resval.rreal;
					pTemp = pTemp->rbnext;
					i++;
					if(i==10)
						break;
				}			   
				xray.xyarr[0] = 2;
				xray.xyarr[1] = R;
			}
		}
		else
		{
			pCurObj->close();
			ads_printf(L"\nSelected entity is not a Polyline!");
			return 1;
		}
		acutRelRb(pRb);		
	}
	return 1;
}
void BAS_DRAW_FUN_RAIL::ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend)
{
	double k=0.0;//直线的斜率
	double b=0.0;//将直线化为y=kx+b后的b值
	if (fabs(dYstart-dYend)<PRECISION)
	{
		dXchuizu = dXout;
		dYchuizu = (dYstart+dYend)/2;
	}
	else if (fabs(dXstart-dXend)<PRECISION)
	{
		dYchuizu = dYout;
		dXchuizu = (dXstart+dXend)/2;
	}
	else 
	{	
		k = (dYend-dYstart)/(dXend-dXstart);
		b = dYstart-dXstart*k;
		dXchuizu = (dYout+dXout/k-b)/(k+1/k);
		dYchuizu = k*(dYout+dXout/k-b)/(k+1/k)+b;
	}
}
bool BAS_DRAW_FUN_RAIL:: checkIfXJ(double array[10],double array2[10],double step)
{
	AcGePoint3dArray Triangle1pt;
	AcGePoint3dArray Triangle2pt;
	Triangle1pt.setLogicalLength(4);
	Triangle2pt.setLogicalLength(4);
	double lenth=0;
	double qidian[2];
	qidian[0]=qidian[1]=0.0;
	double qdfwj=0.0;
	double  pt[3];
	pt[0]=pt[1]=pt[2]=0.0;
	double  jiaodian[2];
	jiaodian[0]=jiaodian[1]=0.0;
	AcGePoint3d  pt3d;
	double zdfwj=0.0;
	double out1,out2;
	out1=out2=0.0;
	pt3d.x=pt3d.y=pt3d.z=0;
	//int ptnum;
	//缓和曲线2
	qidian[0]=array[8];
	qidian[1]=array[9];
	qdfwj=array[5];
	if ((array[0]-2)<0.001)
	{
		lenth=array[2];
	}
	else
	{
		lenth=array[1]*array[1]/array[2];
	}
	GetXYonSomeXY(array,lenth,pt);
	pt[2]+=pi;
	double x2=qidian[0]+1000*cos(qdfwj);
	double y2=qidian[1]+1000*sin(qdfwj);
	double x_2=pt[0]+1000*cos(pt[2]);
	double y_2=pt[1]+1000*sin(pt[2]);
	caljd(qidian[0],qidian[1],x2,y2,pt[0],pt[1],x_2,y_2,&out1,&out2);
	jiaodian[0]=out1;
	jiaodian[1]=out2;
	Triangle1pt[0].set(qidian[0],qidian[1],0.0);
	Triangle1pt[1].set(jiaodian[0],jiaodian[1],0.0);
	Triangle1pt[2].set(pt[0],pt[1],0.0);
	Triangle1pt[3].set(qidian[0],qidian[1],0.0);
	//make2dline(Triangle1pt,5,999,0.1);
	//缓和曲线2
	qidian[0]=array2[8];
	qidian[1]=array2[9];
	qdfwj=array2[5];
	if ((array2[0]-2)<0.001)
	{
		lenth=array2[2];
	}
	else
	{
		lenth=array2[1]*array2[1]/array2[2];
	}
	GetXYonSomeXY(array2,lenth,pt);
	pt[2]+=pi;
	x2=qidian[0]+1000*cos(qdfwj);
	y2=qidian[1]+1000*sin(qdfwj);
	x_2=pt[0]+1000*cos(pt[2]);
	y_2=pt[1]+1000*sin(pt[2]);
	caljd(qidian[0],qidian[1],x2,y2,pt[0],pt[1],x_2,y_2,&out1,&out2);
	jiaodian[0]=out1;
	jiaodian[1]=out2;
	Triangle2pt[0].set(qidian[0],qidian[1],0.0);
	Triangle2pt[1].set(jiaodian[0],jiaodian[1],0.0);
	Triangle2pt[2].set(pt[0],pt[1],0.0);
	Triangle2pt[3].set(qidian[0],qidian[1],0.0);
	//make2dline(Triangle2pt,5,999,0.1);
	if (IfTriangleXJ(Triangle1pt,Triangle2pt))
	{
		return true;
	}
	else
	{
		return false;
	}
	//while (lenth<lenth1)
	//{
	//	GetXYonSomeXY(array,lenth,pt);
	//	pt3d.set(pt[0],pt[1],pt[2]);
	//	ptarry.append(pt3d);
	//	//ptnum++;
	//	lenth+=step;
	//}
	//GetXYonSomeXY(array,lenth1,pt);
	//pt3d.set(pt[0],pt[1],pt[2]);
	//ptarry.append(pt3d);
	//AcDb2dPolyline  *pLine1=new AcDb2dPolyline(AcDb::k2dSimplePoly,ptarry,0.0, Adesk::kFalse);
	//	ptarry.removeSubArray(0,ptarry.length()-1);
	//	if(array2[0]<1.1)//直线
	//	{
	//		lenth2=array2[1];
	//		step=array2[1];
	//	}
	//	else if(array2[0]<2.1)//圆
	//	{
	//		lenth2=array2[2];
	//	}
	//	else
	//	{
	//		lenth2=array2[1]*array2[1]/array2[2];
	//	}
	//	lenth=0;
	//	while (lenth<lenth2)
	//	{
	//		GetXYonSomeXY(array2,lenth,pt);
	//		pt3d.set(pt[0],pt[1],pt[2]);
	//		ptarry.append(pt3d);
	//		//ptnum++;
	//		lenth+=step;
	//	}
	//	GetXYonSomeXY(array2,lenth1,pt);
	//	pt3d.set(pt[0],pt[1],pt[2]);
	//	ptarry.append(pt3d);
	//	AcDb2dPolyline *pLine2=new AcDb2dPolyline(AcDb::k2dSimplePoly,ptarry,0.0, Adesk::kFalse);
	//		ptarry.removeSubArray(0,ptarry.length()-1);
	//	pLine1->intersectWith(pLine2,AcDb::kOnBothOperands,ptarry);
	//	if(ptarry.length()>0)
	//		return true;
	//	else
	//		return false;
	////ptnum++;

}


////////////////////////引出接入  道岔
double BAS_DRAW_FUN_RAIL::SwitchNumToAngle(int Num)//道岔号数转角度（弧度）
{
	double angle=0;
	angle = atan(1.0/Num);
	return angle;
}
/*===================================================================

函数功能:线元终点坐标计算	
-----------------------------------------------------
输入:单个线元SingleXY[10]	
-----------------------------------------------------
输出:线元终点   
-----------------------------------------------------
算法描述:   
-----------------------------------------------------

备注:CAD坐标 N-Y  E-X	                                 钟晶 2008:3:6
=====================================================================*/
BOOL BAS_DRAW_FUN_RAIL::GetSingleSwitchCoordinate(double SingleXY[10],AcGePoint3d &Center,AcGePoint3d &End1,AcGePoint3d &End2,AcGePoint3d &End3)
{
	int XLSX=(int)(SingleXY[0]+0.01);
	if(XLSX == 1)//直线
	{

		Center.x = SingleXY[9];
		Center.y = SingleXY[8];
		Center.z=0;
		//斜边终点
		End2.x = Center.x + SingleXY[2]*sin(SingleXY[5]+SingleXY[4]*SingleXY[3]);
		End2.y = Center.y + SingleXY[2]*cos(SingleXY[5]+SingleXY[4]*SingleXY[3]);
		End2.z = 0; 

		//计算直线边终点
		End1.x = SingleXY[9] +(SingleXY[1]+SingleXY[2])*sin(SingleXY[5]);//
		End1.y = SingleXY[8] +(SingleXY[1]+SingleXY[2])*cos(SingleXY[5]);
		End1.z = 0; 


		//计算直线边起点    
		End3.x = Center.x - SingleXY[1]*sin(SingleXY[5]+SingleXY[3]);
		End3.y = Center.y - SingleXY[1]*cos(SingleXY[5]+SingleXY[3]);
		End3.z = 0; 
		//ads_printf(L"\nEnd2.x=%lf,End2.y =%lf",End2.x,End2.y);

	}
	else if (XLSX>=2 && XLSX<=6)//原有线元 除直线
	{ 
		double INDATA[5],l;
		xlpoint P0,PZ;
		AcGePoint3d ptb,pt,pte;

		for (int j=0;j<5;j++)
		{
			INDATA[j]=SingleXY[j];
		}
		l=Get_INDATA_L(INDATA,&XLSX);
		P0.x = SingleXY[8];
		P0.y = SingleXY[9];
		P0.a = SingleXY[5];
		P0.lc = SingleXY[6];
		xlzbxy(&P0,&PZ,l,INDATA,0);
		Center.x=PZ.y;
		Center.y=PZ.x;
		Center.z=0;
	}

	else if (XLSX == 7)//单开道岔
	{

		//计算岔心  岔心为失拾取点
		Center.x = SingleXY[9] ;//从拾取点开始算
		Center.y = SingleXY[8] ;
		Center.z = 0; 

		//计算直线边终点
		End1.x = Center.x  +SingleXY[2]*sin(SingleXY[5]);//
		End1.y = Center.y  +SingleXY[2]*cos(SingleXY[5]);
		End1.z = 0; 

		//计算支线边终点	    
		End2.x = Center.x + SingleXY[2]*sin(SingleXY[5]+SingleXY[4]*SingleXY[3]);
		End2.y = Center.y + SingleXY[2]*cos(SingleXY[5]+SingleXY[4]*SingleXY[3]);
		End2.z = 0; 

		//计算直线边起点    
		End3.x = Center.x - (SingleXY[1])*sin(SingleXY[5]);
		End3.y = Center.y - (SingleXY[1])*cos(SingleXY[5]);
		End3.z = 0; 
		////计算岔心
		//Center.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]);//从拾取点开始算
		//Center.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]);
		//Center.z = 0; 

		////计算直线边终点
		//End1.x = SingleXY[9] +(SingleXY[1]+SingleXY[2])*sin(SingleXY[5]);//
		//End1.y = SingleXY[8] +(SingleXY[1]+SingleXY[2])*cos(SingleXY[5]);
		//End1.z = 0; 

		////计算支线边终点	    
		//End2.x = Center.x + SingleXY[2]*sin(SingleXY[5]+SingleXY[4]*SingleXY[3]);
		//End2.y = Center.y + SingleXY[2]*cos(SingleXY[5]+SingleXY[4]*SingleXY[3]);
		//End2.z = 0; 
	}
	else if (XLSX == 8)//对称双开道岔
	{
		//计算岔心
		Center.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]);
		Center.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]);
		Center.z = 0; 

		//计算左偏终点
		End1.x = Center.x + SingleXY[2]*sin(SingleXY[5]-SingleXY[3]/2.0);
		End1.y = Center.y + SingleXY[2]*cos(SingleXY[5]-SingleXY[3]/2.0);
		End1.z = 0; 

		//计算右偏终点	    
		End2.x = Center.x + SingleXY[2]*sin(SingleXY[5]+SingleXY[3]/2.0);
		End2.y = Center.y + SingleXY[2]*cos(SingleXY[5]+SingleXY[3]/2.0);
		End2.z = 0; 
	}
	else if (XLSX == 9)//对称三开道岔
	{
		//计算岔心
		Center.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]);
		Center.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]);
		Center.z = 0; 

		//计算直线边终点
		End1.x = SingleXY[9] +(SingleXY[1]+SingleXY[2])*sin(SingleXY[5]);
		End1.y = SingleXY[8] +(SingleXY[1]+SingleXY[2])*cos(SingleXY[5]);
		End1.z = 0; 

		//计算左偏终点
		End2.x = Center.x + SingleXY[2]*sin(SingleXY[5]-SingleXY[3]);
		End2.y = Center.y + SingleXY[2]*cos(SingleXY[5]-SingleXY[3]);
		End2.z = 0; 

		//计算右偏终点	    
		End3.x = Center.x + SingleXY[2]*sin(SingleXY[5]+SingleXY[3]);
		End3.y = Center.y + SingleXY[2]*cos(SingleXY[5]+SingleXY[3]);
		End3.z = 0; 
	}
	else if (XLSX == 10 || XLSX == 12)//交分道岔或菱形交叉
	{
		//计算顺向左偏终点
		Center.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]-SingleXY[3]/2.0);
		Center.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]-SingleXY[3]/2.0);
		Center.z = 0; 

		//计算顺向右偏终点
		End1.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]+SingleXY[3]/2.0);
		End1.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]+SingleXY[3]/2.0);
		End1.z = 0; 

		//计算逆向左偏终点
		End2.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]-SingleXY[3]/2.0+pi);
		End2.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]-SingleXY[3]/2.0+pi);
		End2.z = 0; 

		//计算逆向右偏终点	    
		End3.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]+SingleXY[3]/2.0+pi);
		End3.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]+SingleXY[3]/2.0+pi);
		End3.z = 0; 
	}
	else if (XLSX == 11)//直角交叉
	{
		//计算顺向左偏终点
		Center.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]);
		Center.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]);
		Center.z = 0; 

		//计算顺向右偏终点
		End1.x = SingleXY[9] +SingleXY[2]*sin(SingleXY[5]+pi/2.0);
		End1.y = SingleXY[8] +SingleXY[2]*cos(SingleXY[5]+pi/2.0);
		End1.z = 0; 

		//计算逆向左偏终点
		End2.x = SingleXY[9] +SingleXY[1]*sin(SingleXY[5]+pi);
		End2.y = SingleXY[8] +SingleXY[1]*cos(SingleXY[5]+pi);
		End2.z = 0; 

		//计算逆向右偏终点	    
		End3.x = SingleXY[9] +SingleXY[2]*sin(SingleXY[5]+1.5*pi);
		End3.y = SingleXY[8] +SingleXY[2]*cos(SingleXY[5]+1.5*pi);
		End3.z = 0; 
	}
	else if (13==XLSX)//车档
	{
		//计算左偏折点
		Center.x = SingleXY[9] +SingleXY[1]/2.0*sin(SingleXY[5]-pi/2.0);
		Center.y = SingleXY[8] +SingleXY[1]/2.0*cos(SingleXY[5]-pi/2.0);
		Center.z = 0; 

		//计算右偏折点
		End1.x = SingleXY[9] +SingleXY[1]/2.0*sin(SingleXY[5]+pi/2.0);
		End1.y = SingleXY[8] +SingleXY[1]/2.0*cos(SingleXY[5]+pi/2.0);
		End1.z = 0; 

		//计算左偏终点
		End2.x = Center.x +SingleXY[2]*sin(SingleXY[5]);
		End2.y = Center.y +SingleXY[2]*cos(SingleXY[5]);
		End2.z = 0; 

		//计算右偏终点	    
		End3.x = End1.x +SingleXY[2]*sin(SingleXY[5]);
		End3.y = End1.y +SingleXY[2]*cos(SingleXY[5]);
		End3.z = 0; 
	}
	else
	{
		return FALSE;
	}

	return TRUE;

}
int BAS_DRAW_FUN_RAIL::JudgePTLorR(double N,double E,double &dist,double array[4*MAXJDNUM][10],int tarray)
{
	double fwj,cml;
	xlpoint PZ;

	cml = PROJ_ML(array,tarray,N,E,N,E);//投影到线路
	PZ.lc = cml;
	xlpoint_pz(array,tarray,&PZ);////
	fwj = PZ.a;
	dist = sqrt((N-PZ.x)*(N-PZ.x)+(E-PZ.y)*(E-PZ.y));//两个投影点距离

	if(dist<0.001)//点在线路上
		return 0;

	double pt1[2],pt2[2],pt3[2];
	pt1[0] = PZ.x , pt1[1] = PZ.y;

	pt2[0] = PZ.x + 1.0*cos(PZ.a);
	pt2[1] = PZ.y + 1.0*sin(PZ.a);

	pt3[0] = N , pt3[1] = E;

	double  zy = CCW2(pt1,pt2,pt3); 
	//	
	//	double fwjpttoxl;
	//	xyddaa(N,E,PZ.x,PZ.y,&fwjpttoxl);
	int LorR;
	//	double zj = ZXJ(fwjpttoxl,fwj);

	if(zy>0.0)//
		LorR = 1;//点在右侧
	else
		LorR = -1;//点在左侧

	//	if(fabs(cml-189990.35)<0.1)
	//		ads_printf(L"%lf x1=%lf y1=%lf x2=%lf y2=%lf fwj1=%lf fwj2=%lf\n",dist,Pt1[0],Pt1[1],PZ.x,PZ.y,fwj,fwj1to2);
	return LorR;
}
double BAS_DRAW_FUN_RAIL::CCW2(double pt1[2],double pt2[2],double pt3[2]) 
{
	double x1,x2,x3,y1,y2,y3,bx,by;

	bx = (pt1[0]+pt2[0]+pt3[0]) /3.0;
	by = (pt1[1]+pt2[1]+pt3[1]) /3.0;
	x1 = pt1[0]-bx;
	x2 = pt2[0]-bx;
	x3 = pt3[0]-bx;
	y1 = pt1[1]-by;
	y2 = pt2[1]-by;
	y3 = pt3[1]-by;
	return (x1*(y2-y3)-x2*(y1-y3)+x3*(y1-y2));
}
int BAS_DRAW_FUN_RAIL::FitSwitch(double angle)
{
	int Num = -1;
	double testNum = fabs(1.0/tan(angle));
	if (fabs( 9.0-testNum )<0.001)
	{
		Num=9;		
	}
	else if (fabs( 6.0-testNum )<0.001)
	{
		Num=6;		
	}
	else if (fabs( 7.0-testNum )<0.001)
	{
		Num=7;		
	}
	else if (fabs( 8.0-testNum )<0.001)
	{
		Num=8;		
	}
	else if (fabs( 10.0-testNum )<0.001)
	{
		Num=10;	
	}
	else if (fabs( 11.0-testNum )<0.001)
	{
		Num=11;		
	}
	else if (fabs( 12.0-testNum )<0.001)
	{
		Num=12;		
	}
	else if (fabs( 18.0-testNum )<0.001)
	{
		Num=18;		
	}
	else if (fabs( 24.0-testNum )<0.001)
	{
		Num=24;		
	}
	else if (fabs( 30.0-testNum )<0.001)
	{
		Num=30;		
	}
	else
	{
		Num = int(testNum);
	}
	return Num;
}
void BAS_DRAW_FUN_RAIL::InitSwitchNum(CComboBox &Swith)
{


	Swith.AddString(L"6");//0
	Swith.AddString(L"7");//1
	Swith.AddString(L"8");//2
	Swith.AddString(L"9");//3
	Swith.AddString(L"10");//4
	Swith.AddString(L"11");//5
	Swith.AddString(L"12");//6
	Swith.AddString(L"18");//7
	Swith.AddString(L"24");//8
	Swith.AddString(L"30");//9

	Swith.SetCurSel(3);

}

CString BAS_DRAW_FUN_RAIL::GetProByMdbName(CString mdbPath)
{
	CString ProStr;
	int myINT=mdbPath.ReverseFind('\\');
	if (myINT<0)
		return Pro;
	int myINT2=mdbPath.GetLength();
	myINT++;
	myINT2=myINT2-myINT-4;
	if (myINT2<1)
		return Pro;
	ProStr=mdbPath.Mid(myINT,myINT2);
	return ProStr;
}


void  BAS_DRAW_FUN_RAIL:: CalXLjd(double array1[4*MAXJDNUM][10],int NunJD1,double array2[4*MAXJDNUM][10],int NunJD2,AcGePoint2dArray* JDpt)
{	
	int JDNUM;
	double ResPt[3],ResPt1[3];
	int i,j;
	AcGePoint2d respt;
	AcGePoint3d cyclepoint;
	//计算线元交点
	if(NunJD1*4>0)
	{
		if (NunJD1==1)
		{
			i=(NunJD1-1)*4+1;
		}
		else
		{

			i=(NunJD1-1)*4;
		}
		for( i; i<NunJD1*4; i++)
		{
			if (NunJD2==1)
			{
				j=(NunJD2-1)*4+1;
			}
			else
			{

				j=(NunJD2-1)*4;
			}

			for( j; j<NunJD2*4; j++)
			{
				//double E1,E2;
				//if(array1[i][0]==1)
				//{
				//	E1=0;
				//}
				//else if(array1[i][0]==2)
				//{
				//	E1=array1[i][1]*array1[i][4];
				//}
				//else
				//{
				//	double hx=array1[i][8]-array1[i+1][8];
				//	double hy=array1[i][9]-array1[i+1][9];
				//	double lenth=sqrt(hx*hx+hy*hy);
				//	E1=fabs(hy*0.5*lenth/hx)*array1[i][4];
				//}
				//if(array2[j][0]==1)
				//{
				//	E2=0;
				//}
				//else if(array2[j][0]==2)
				//{
				//	E2=array2[j][1]*array2[j][4];
				//}
				//else
				//{
				//	double hx=array2[j][8]-array2[j+1][8];
				//	double hy=array2[j][9]-array2[j+1][9];
				//	double lenth=sqrt(hx*hx+hy*hy);
				//	E2=fabs(hy*0.5*lenth/hx)*array2[j][4];
				//}
				//double minx1,maxx1,miny1,maxy1;
				//double minx2,maxx2,miny2,maxy2;
				//double x1,x2,y1,y2;
				//x1=array1[i][8];
				//x2=array1[i+1][8];
				//y1=array1[i][9];
				//y2=array1[i+1][9];
				//movepoint(x1,y1,x2,y2,E1);
				//getminandmax( x1,x2,array1[i][8],array1[i+1][8],minx1,maxx1);
				//getminandmax( y1,y2,array1[i][9],array1[i+1][9],miny1,maxy1);
				//x1=array2[j][8];
				//x2=array2[j+1][8];
				//y1=array2[j][9];
				//y2=array2[j+1][9];
				//movepoint(x1,y1,x2,y2,E2);
				//getminandmax( x1,x2,array2[j][8],array2[j+1][8],minx2,maxx2);
				//getminandmax( y1,y2,array2[j][9],array2[j+1][9],miny2,maxy2);
				//if(minx1>maxx2||maxx1<minx2)
				//{
				//	continue;
				//}
				//if(miny1>maxy2||maxy1<miny2)
				//{
				//	continue;
				//}
				JDNUM = CalJD_XY1_XY2(array1[i],array2[j],ResPt,ResPt1);	
				if(JDNUM>0)
				{
					//ads_printf(L"i=%d  j=%d\n",i,j);
					//ads_printf(L"x=%f  y=%f\n",ResPt[0],ResPt[1]);
					//AfxMessageBox(L"1");
					respt.set(ResPt[0],ResPt[1]);
					JDpt->append(respt);
					cyclepoint.set(ResPt[1],ResPt[0],ResPt[2]);
					//makecircle(cyclepoint,3.6,10);
				}
				if(JDNUM==2)
				{
					respt.set(ResPt1[0],ResPt1[1]);
					JDpt->append(respt);	
					cyclepoint.set(ResPt1[1],ResPt1[0],ResPt1[2]);
					//makecircle(cyclepoint,3.6,10);
				}

			}
		}
	}
}
void BAS_DRAW_FUN_RAIL::movepoint(double& x1,double& y1, double& x2, double& y2,double dis)
{
	double x,vecX;
	double y,vecY;
	x=x2-x1;
	y=y2-y1;
	vecX=-1;//考虑左右偏
	vecY=(-1+x*vecX)/y;
	double lenth=sqrt(vecX*vecX+vecY*vecY);
	x1=x1+vecX*dis/lenth;
	y1=y1+vecY*dis/lenth;
	x2=x2+vecX*dis/lenth;
	y2=y2+vecY*dis/lenth;

}
void BAS_DRAW_FUN_RAIL::getminandmax(double x1,double x2,double x3,double x4,double& min,double& max)
{
	double fmin,fmax;
	if(x1<x2)
	{
		fmin=x1;
		fmax=x2;
	}
	else
	{
		fmin=x2;
		fmax=x1;
	}
	if(fmin>x3)
	{
		fmin=x3;
	}
	if(fmax<x3)
	{
		fmax=x3;
	}
	if(fmin>x4)
	{
		fmin=x4;
	}
	if(fmax<x4)
	{
		fmax=x4;
	}
	min=fmin;
	max=fmax;

}
bool BAS_DRAW_FUN_RAIL::IfXJandCaljd(double x1, double y1, double x2, double y2, double x_1, double y_1, double x_2, double y_2, double *xjd, double *yjd)
{
	double a1,b1,c1,a2,b2,c2,abc;
	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = y1*(x2-x1)-x1*(y2-y1);
	a2 = y_2 - y_1;
	b2 = x_1 - x_2;
	c2 = y_1*(x_2-x_1)-x_1*(y_2-y_1);
	abc = a1*b2 -a2*b1;
	*yjd = (c1*a2 -a1*c2)/abc;
	*xjd = -(c1*b2 -b1*c2)/abc;
	if ((*yjd-y1)*(*yjd-y2)<=0)
	{
		if ((*xjd-x1)*(*xjd-x2)<=0)
		{
			return  true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool BAS_DRAW_FUN_RAIL:: IfTriangleXJ(AcGePoint3dArray Triangle1pt,  AcGePoint3dArray Triangle2pt)
{
	AcGePoint3dArray ptarry;
	AcDb2dPolyline *pLine1=new AcDb2dPolyline(AcDb::k2dSimplePoly,Triangle1pt,0.0, Adesk::kFalse);
	AcDb2dPolyline *pLine2=new AcDb2dPolyline(AcDb::k2dSimplePoly,Triangle2pt,0.0, Adesk::kFalse);
	ptarry.removeSubArray(0,ptarry.length()-1);
	pLine1->intersectWith(pLine2,AcDb::kOnBothOperands,ptarry);
	if(ptarry.length()>0)
		return true;
	else
		return false;
}
void BAS_DRAW_FUN_RAIL:: CheckIfXLintersect(double array1[4*MAXJDNUM][10],double JDarray1[MAXJDNUM][6],int NunJD1,double array2[4*MAXJDNUM][10],double JDarray2[MAXJDNUM][6],int NunJD2,AcGePoint2dArray* JDpt)
{
	AcGePoint3dArray Triangle1pt;
	AcGePoint3dArray Triangle2pt;
	Triangle1pt.setLogicalLength(4);
	Triangle2pt.setLogicalLength(4);
	for (int i=1;i<NunJD1;i++)
	{
		Triangle1pt[0].set(JDarray1[i][1],JDarray1[i][2],0.0);
		if (i==1)
		{
			Triangle1pt[1].set(array1[(i-1)*4][1],array1[(i-1)*4][2],0.0);
		}
		else
		{
			Triangle1pt[1].set(array1[(i-1)*4][8],array1[(i-1)*4][9],0.0);
		}
		Triangle1pt[2].set(array1[i*4][8],array1[i*4][9],0.0);
		Triangle1pt[3].set(JDarray1[i][1],JDarray1[i][2],0.0);
		double a=JDarray1[i][1];
		double b=JDarray1[i][2];
		double c=array1[(i-1)*4][8];
		double d=array1[(i-1)*4][9];
		double f=array1[i*4][8];
		double e=array1[i*4][9];
		//make2dline(Triangle1pt,5,999,0.1);
		for(int j=1;j<NunJD2;j++)
		{
			Triangle2pt[0].set(JDarray2[j][1],JDarray2[j][2],0.0);
			if (j==1)
			{
				Triangle2pt[1].set(array2[(j-1)*4][1],array2[(j-1)*4][2],0.0);
			}
			else
			{
				Triangle2pt[1].set(array2[(j-1)*4][8],array2[(j-1)*4][9],0.0);
			}
			Triangle2pt[2].set(array2[j*4][8],array2[j*4][9],0.0);
			Triangle2pt[3].set(JDarray2[j][1],JDarray2[j][2],0.0);
			if (i==1)
			{
				//make2dline(Triangle2pt,5,999,0.1);
			}
			if (IfTriangleXJ(Triangle1pt,Triangle2pt))
			{
				CalXLjd(array1,i,array2,j,JDpt);
			}
		}
	}
}
bool BAS_DRAW_FUN_RAIL::LoadArx(ACHAR ArxFname[256],ACHAR cmd[80])
{
	ACHAR cmdstr[80];

	acedGetAcadDwgView()->SetFocus();
	ACHAR fname[256];
	_stprintf(fname , L"%s\\%s",SysPath,ArxFname);

	acedArxLoad(fname);
	acDocManager->activateDocument(acDocManager->curDocument());
	_stprintf(cmdstr , L"(command \"%s\")\n",cmd);
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	

	return 1;

}
void BAS_DRAW_FUN_RAIL::RToS(double lc,int NLC,ACHAR ch[80])
{
	ACHAR fmat[16];
	_stprintf(fmat,L"%%0.%dlf",NLC);
	_stprintf(ch,fmat,lc);
}
