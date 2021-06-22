// BAS_DRAW_FUN.cpp: implementation of the BAS_DRAW_FUN class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "aced.h"
#include "dbapserv.h"
#include "BAS_DRAW_FUN.h"
#include "XLdatabase.h"
#include "migrtion.h"
#include "BlockComputerFun.h"
#include "Hhqx.h"
#include "TRIM.h"
#include "dbhatch.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016

#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS" 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


extern CString Cworkdir, Pro;
extern ACHAR SysPath[1024];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BAS_DRAW_FUN::BAS_DRAW_FUN()
{

}

BAS_DRAW_FUN::~BAS_DRAW_FUN()
{

}

int BAS_DRAW_FUN::GetCircle(ads_name e, ads_point vertex)
{
	//	ads_point vertex;
	struct resbuf *sube, *p;
	//	ads_name  e;

	//	ads_entsel(L"请选择圆:", e , pt);
	sube = ads_entget(e);
	p = sube;

	for (; p != NULL;)
	{
		if (p->restype == 10)
		{
			vertex[X] = p->resval.rpoint[0];
			vertex[Y] = p->resval.rpoint[1];
			vertex[Z] = p->resval.rpoint[2];
			break;
		}
		p = p->rbnext;
	}
	//ads_printf(L"\n circle");
	//ads_printf(L"\n %12.3lf %12.3lf %12.3lf", vertex[0][X],vertex[0][Y],vertex[0][Z]);

	return RTNORM;
}

// 向AUTOCAD 数据库中添加实体对象的通用函数
AcDbObjectId BAS_DRAW_FUN::AddEntityToDbs(AcDbEntity *pEntity, int mode, int ShiftSpace)
{

	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	//	Acad::ErrorStatus     es = Acad::eOk;
	AcDbObjectId objId;
	AcDbBlockTable *pBlockTable;
	// CAD R2000
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
	// CAD R14
	// acdbCurDwg()->getBlockTable(pBlockTable,AcDb::kForRead);

	AcDbBlockTableRecord  *pSpaceRecord;
	if (ShiftSpace == 0)pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite);
	else pBlockTable->getAt(ACDB_PAPER_SPACE, pSpaceRecord, AcDb::kForWrite);

	pSpaceRecord->appendAcDbEntity(objId, pEntity);
	pSpaceRecord->close();

	pBlockTable->close();
	if (mode == 0)pEntity->close();
	acDocManager->unlockDocument(acDocManager->curDocument());
	return objId;
}


// 由两点绘直线
AcDbObjectId   BAS_DRAW_FUN::makeline(AcGePoint3d& startPt, AcGePoint3d& endPt)
{
	AcDbLine *pLine = new AcDbLine(startPt, endPt);
	AcDbObjectId  LId;

	LId = AddEntityToDbs(pLine);
	return LId;
}

AcDbObjectId   BAS_DRAW_FUN::makeline1(AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor)
{
	AcDbLine *pLine = new AcDbLine(startPt, endPt);
	pLine->setColorIndex(icolor);
	AcDbObjectId  LId;
	LId = AddEntityToDbs(pLine);
	return LId;
}
// 绘文本

// 绘文本
AcDbObjectId BAS_DRAW_FUN::maketext(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, int icolor, int mode, ACHAR* textStyle, ACHAR *LayerName)
{
	AcDbObjectId  TId;
	AcDbText *pText = new AcDbText;
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl, AcDb::kForWrite);
	if (pTextStyleTbl->getAt(textStyle, pTextStyleTblId) != Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd = new AcDbTextStyleTableRecord;

		//////////////4:
		pTextStyleTblRcd->setName(textStyle);

		ACHAR TextStyleFile[256];
		_stprintf(TextStyleFile, L"%s\\lib\\Rs9.shx", SysPath);

		pTextStyleTblRcd->setFileName(TextStyleFile);
		_stprintf(TextStyleFile, L"%s\\lib\\HZTXT.shx", SysPath);
		pTextStyleTblRcd->setBigFontFileName(TextStyleFile);

		pTextStyleTblRcd->setFont(L"rs9.shx", 0, 0, 134, 2);
		pTextStyleTblRcd->setFileName(textStyle);
		pTextStyleTblRcd->setFont(textStyle, 0, 0, 134, 2);

		///////////////////////:4

		pTextStyleTblRcd->setXScale(0.8);
		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		//	textStyle="PMBZ";//指定样式
		pTextStyleTbl->getAt(textStyle, pTextStyleTblId);

	}
	pTextStyleTbl->close();

	if (mode == 1) // 左对齐
	{
		pText->setHorizontalMode(AcDb::kTextLeft);
	}
	else if (mode == 2) // // 中对齐
	{
		pText->setHorizontalMode(AcDb::kTextCenter);
	}
	else if (mode == 3) // // 中对齐
	{
		pText->setHorizontalMode(AcDb::kTextMid);
	}
	else // // 右对齐
	{
		pText->setHorizontalMode(AcDb::kTextRight);
	};
	pText->setColorIndex(icolor);
	if (mode == 1) { pText->setPosition(pos); }
	else { pText->setAlignmentPoint(pos); };
	//  ads_printf(L"\n   m        Pt=%10.3f   %10.3f   %10.4f %s",pos[X],pos[Y],dms_rad(ang),ctext);  
	pText->setTextStyle(pTextStyleTblId);
	pText->setRotation(ang);
	pText->setHeight(texth);
	pText->setWidthFactor(0.8);
	pText->setTextString(ctext);
	//layer///////////////////////////////////////////////////////////////////
	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pText->setLayer(LayerTabRecordId);
	pLayerTable->close();
	//////////////////////////////////////////////////////////////////////////

	TId = AddEntityToDbs(pText);
	return TId;
}

//绘多行文本
AcDbObjectId BAS_DRAW_FUN::MakeMText(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, double Width, int icolor, AcDbMText::AttachmentPoint mode, ACHAR* textStyle, ACHAR *LayerName, double LineSpacingFactor)
{
	AcDbObjectId  TId;
	AcDbMText *pText = new AcDbMText;
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl, AcDb::kForWrite);
	if (pTextStyleTbl->getAt(textStyle, pTextStyleTblId) != Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd = new AcDbTextStyleTableRecord;

		//////////////4:
		pTextStyleTblRcd->setName(textStyle);
		ACHAR TextStyleFile[256];
		_stprintf(TextStyleFile, L"%s\\lib\\Rs9.shx", SysPath);

		pTextStyleTblRcd->setFileName(TextStyleFile);
		_stprintf(TextStyleFile, L"%s\\lib\\HZTXT.shx", SysPath);
		pTextStyleTblRcd->setBigFontFileName(TextStyleFile);

		pTextStyleTblRcd->setFont(L"rs9.shx", 0, 0, 134, 2);
		pTextStyleTblRcd->setFileName(textStyle);
		pTextStyleTblRcd->setFont(textStyle, 0, 0, 134, 2);

		///////////////////////:4

		pTextStyleTblRcd->setXScale(0.8);
		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		//	textStyle="PMBZ";//指定样式
		pTextStyleTbl->getAt(textStyle, pTextStyleTblId);

	}
	pTextStyleTbl->close();

	pText->setLocation(pos);
	pText->setContents(ctext);
	pText->setRotation(ang);
	pText->setTextHeight(texth);
	pText->setWidth(Width);
	pText->setColorIndex(icolor);
	pText->setAttachment(mode);
	pText->setTextStyle(pTextStyleTblId);
	pText->setLineSpacingFactor(LineSpacingFactor);
	//layer///////////////////////////////////////////////////////////////////
	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pText->setLayer(LayerTabRecordId);
	pLayerTable->close();
	//////////////////////////////////////////////////////////////////////////

	TId = AddEntityToDbs(pText);
	return TId;
}

// 绘圆弧
AcDbObjectId   BAS_DRAW_FUN::makearc(AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx)
{
	AcDbArc *pArc = new AcDbArc;
	AcDbObjectId ArcId;

	//	acdbOpenObject(pArc,ArcId,AcDb::kForWrite,Adesk::kFalse);	
	AcGePoint3d centerPt;
	double f1, sf, ef, dd, AA;
	pArc->setRadius(R);
	dd = xyddaa(startPt[1], startPt[0], endPt[1], endPt[0], &f1);
	AA = acos(dd*0.5 / R);
	centerPt[Y] = startPt[Y] + R * cos(f1 + npx * AA);
	centerPt[X] = startPt[X] + R * sin(f1 + npx * AA);
	centerPt[Z] = startPt[Z];
	//f2=f1+pi; if (f1>pi) f2=f1-pi;
	// ads_printf(L"\n 直线长 distane=%16.7f 方位角 (d.ms) f=%12.7f (%12.7f) ",dd,dms_rad(f1),dms_rad(f2));
	//   ads_printf(L"\n R%7.2f 偏向=%d ",R,npx);
	pArc->setCenter(centerPt);
	// 大地方位角
	dd = xyddaa(centerPt[1], centerPt[0], startPt[1], startPt[0], &f1);
	// 笛卡角
	sf = pi / 2.0 - f1; if (sf < 0) sf = sf + pi * 2;
	//	 ads_printf(L"\n 直线长 distane=%16.7f 方位 (d.ms) sf=%12.7f  ",dd,dms_rad(sf));

	dd = xyddaa(centerPt[1], centerPt[0], endPt[1], endPt[0], &f1);
	// 笛卡角
	ef = pi / 2.0 - f1; if (ef < 0) ef = ef + pi * 2;
	// ads_printf(L"  直线长 distane=%16.7f 方位 (d.ms) ef=%12.7f ",dd,dms_rad(ef));

	if (npx == 1) // 右偏
	{
		pArc->setStartAngle(ef);
		pArc->setEndAngle(sf);
	}
	else  // 左偏
	{
		pArc->setStartAngle(sf);
		pArc->setEndAngle(ef);
	};

	pArc->setRadius(dd);
	pArc->setColorIndex(2);
	//	pArc->close();
	ArcId = AddEntityToDbs(pArc);
	return ArcId;
}

// 绘圆弧
AcDbObjectId   BAS_DRAW_FUN::makearc(AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx, int color, int fh, ACHAR *LayerName)
{
	AcDbArc *pArc = new AcDbArc;
	AcDbObjectId ArcId;

	//	acdbOpenObject(pArc,ArcId,AcDb::kForWrite,Adesk::kFalse);	
	AcGePoint3d centerPt;
	double f1, sf, ef, dd, AA;
	pArc->setRadius(R);
	dd = xyddaa(startPt[1], startPt[0], endPt[1], endPt[0], &f1);
	AA = fh * acos(dd*0.5 / R);
	centerPt[Y] = startPt[Y] + R * cos(f1 + npx * AA);
	centerPt[X] = startPt[X] + R * sin(f1 + npx * AA);
	centerPt[Z] = startPt[Z];
	//f2=f1+pi; if (f1>pi) f2=f1-pi;
	// ads_printf(L"\n 直线长 distane=%16.7f 方位角 (d.ms) f=%12.7f (%12.7f) ",dd,dms_rad(f1),dms_rad(f2));
	//   ads_printf(L"\n R%7.2f 偏向=%d ",R,npx);
	pArc->setCenter(centerPt);
	// 大地方位角
	dd = xyddaa(centerPt[1], centerPt[0], startPt[1], startPt[0], &f1);
	// 笛卡角
	sf = pi / 2.0 - f1; if (sf < 0) sf = sf + pi * 2;
	//	 ads_printf(L"\n 直线长 distane=%16.7f 方位 (d.ms) sf=%12.7f  ",dd,dms_rad(sf));

	dd = xyddaa(centerPt[1], centerPt[0], endPt[1], endPt[0], &f1);
	// 笛卡角
	ef = pi / 2.0 - f1; if (ef < 0) ef = ef + pi * 2;
	// ads_printf(L"  直线长 distane=%16.7f 方位 (d.ms) ef=%12.7f ",dd,dms_rad(ef));

	if (npx == 1) // 右偏
	{
		pArc->setStartAngle(ef);
		pArc->setEndAngle(sf);
	}
	else  // 左偏
	{
		pArc->setStartAngle(sf);
		pArc->setEndAngle(ef);
	};

	pArc->setRadius(dd);
	pArc->setColorIndex(color);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pArc->setLayer(LayerTabRecordId);
	pLayerTable->close();
	//	pArc->close();
	ArcId = AddEntityToDbs(pArc);
	return ArcId;
}
// getPoint - acquire WCS point from user:
int BAS_DRAW_FUN::getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT)
{
	int rc;
	ads_point pFrom = { pF.x, pF.y, pF.z }, pTo;

	if ((rc = ads_getpoint(pFrom, pMessage, pTo)) == RTNORM)
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
int BAS_DRAW_FUN::getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT, AcGePoint3d& pT1, AcGePoint3d& pT2)
{
	ads_point pFrom = { pF.x, pF.y, pF.z },
		ppT = { pT.x,  pT.y, pT.z },
		ppT1 = { pT1.x,  pT1.y,  pT1.z },
		ppT2 = { pT2.x,  pT2.y,  pT2.z };

	int track = 1, type;
	struct resbuf entdata;// 鼠标拖动时的动态坐标
	ads_point  tmppt;
	while (track > 0) {  //鼠标拖动循环

		ads_grread(track, &type, &entdata);
		tmppt[0] = entdata.resval.rpoint[0];
		tmppt[1] = entdata.resval.rpoint[1];
		tmppt[2] = 0.0;
		//	 acedCommandS(RTSTR, L"REDRAW",RTNONE);
		acedRedraw(NULL, 0);
		ads_grdraw(ppT1, tmppt, 3, 1);
		ads_grdraw(ppT2, tmppt, 1, 1);
		ads_grdraw(ppT, tmppt, 2, 1);

		if (type == 3)  //  单击鼠标左键输入一点
		{
			pT.x = tmppt[X];
			pT.y = tmppt[Y];
			pT.z = tmppt[Z];
			track = 0;
		};
	}
	return 1;
}


int  BAS_DRAW_FUN::getPoint(ACHAR* pMessage, AcGePoint3d& pT)
{
	int rc;
	ads_point pTo;
	// CAD R2000
	//  if ((rc = acedGetPoint(NULL, pMessage, pTo)) == RTNORM) {
	// CAD R14 
	if ((rc = ads_getpoint(NULL, pMessage, pTo)) == RTNORM) {

		// rc = sa_u2w(pTo, pTo);
		pT[X] = pTo[X];
		pT[Y] = pTo[Y];
		pT[Z] = pTo[Z];
	}
	return rc;
}


void  BAS_DRAW_FUN::makeGroup(AcDbObjectId groupId)
{
	ads_name sset;
	int err = ads_ssget(NULL, NULL, NULL, NULL, sset);
	if (err != RTNORM) {
		return;
	}

	AcDbGroup *pGroup;
	acdbOpenObject(pGroup, groupId, AcDb::kForWrite);

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

void BAS_DRAW_FUN::removeAllButLines(AcDbObjectId groupId)
{
	AcDbGroup *pGroup;
	acdbOpenObject(pGroup, groupId, AcDb::kForWrite);

	AcDbGroupIterator *pIter = pGroup->newIterator();
	AcDbObject *pObj;
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
		}
		else
		{
			pObj->close();
		}
	}
	delete pIter;

	// Now change the color of all the entities in the group
	// to red (AutoCAD color index number 1).
	//
	pGroup->setColorIndex(1);

	pGroup->close();
}

void BAS_DRAW_FUN::groups()
{
	AcDbGroup *pGroup = new AcDbGroup(L"grouptest");

	AcDbDictionary *pGroupDict;
	// CAD R2000
	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	// CAD R14
	//    acdbCurDwg()->getGroupDictionary(pGroupDict,AcDb::kForWrite);


	AcDbObjectId groupId;
	pGroupDict->setAt(L"ASDK_GROUPTEST", pGroup, groupId);
	pGroupDict->close();
	pGroup->close();

	makeGroup(groupId);
	removeAllButLines(groupId);
}


AcDbEntity*  BAS_DRAW_FUN::selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
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
	ads_printf(L"\nAcDbEntity name=%s", entObj->isA()->name());
	return entObj;
}

double  BAS_DRAW_FUN::Get_INDATA_L(double INDATA[5], int *jj)
{
	int j;
	double l = 0.0;
	j = (int)(INDATA[0] + 0.01);
	if (j == 1) l = INDATA[1];
	if (j == 2) l = INDATA[2];
	if (j == 3) l = INDATA[1] * INDATA[1] / INDATA[2];
	if (j == 4) l = INDATA[1] * INDATA[1] / INDATA[2];
	//   R1----> R2  (BIG---->SMALL)    of spiral
	//   A=INDATA[1];R1=INDATA[2];R2=INDATA[3];L1=A*A/R1;L2=A*A/R2;L=L2-L1;
	if (j == 5) l = INDATA[1] * INDATA[1] * (INDATA[2] - INDATA[3]) / (INDATA[2] * INDATA[3]);
	//   R2----> R1  (SMALL---->big)             of spiral
	//    A=INDATA[1];R2=INDATA[2];R1=INDATA[3];L=A*A*(R2-R1)/R1/R2;L1=A*A/R1;L2=L1+L;ll=L1+l;
	if (j == 6) l = INDATA[1] * INDATA[1] * (INDATA[3] - INDATA[2]) / (INDATA[2] * INDATA[3]);
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

int BAS_DRAW_FUN::xlzbxy(struct xlpoint *P0, struct xlpoint *PZ, double l, double INDATA[5], int display)
{
	double az, a0, aj, R, L, A, R1, R2, zy, L1, L2, ll, xm, ym, t1, t2, x, y;
	int type, re = 1;
	double ar1, ar2, ar, xr, yr, xr1, yr1, xr2, yr2;
	a0 = P0->a;
	type = (int)(INDATA[0] + 0.0001);
	zy = INDATA[4];
	PZ->lc = P0->lc + l;
	switch (type)
	{
	case 1:    //   line
		PZ->x = P0->x + l * cos(a0);
		PZ->y = P0->y + l * sin(a0);
		PZ->a = P0->a; PZ->r = 1e40;
		break;
	case 2:   //    part of circle
		R = INDATA[1]; aj = l / R; az = a0 + zy * aj; t1 = R * tan(aj * .5); t2 = t1;
		xm = P0->x + t1 * cos(a0); ym = P0->y + t1 * sin(a0);
		PZ->x = xm + t2 * cos(az); PZ->y = ym + t2 * sin(az); PZ->a = az; PZ->r = R;
		break;
	case 3:   //    0=0--Ri-->R  l<=L&& Ri>R     of  spiral
		A = INDATA[1]; R = INDATA[2]; L = A * A / R; aj = l * l * .5 / R / L;
		x = spiral_x(A, l); y = spiral_y(A, l);
		t1 = x - y / tan(aj); t2 = y / sin(aj); az = a0 + aj * zy;
		xm = P0->x + t1 * cos(a0); ym = P0->y + t1 * sin(a0);
		PZ->x = xm + t2 * cos(az); PZ->y = ym + t2 * sin(az); PZ->a = az;
		if (l > 0) { PZ->r = A * A / l; }
		else { PZ->r = 1e40; };
		break;
	case 4:   //   R----> 0-0            of spiral
		A = INDATA[1]; R = INDATA[2]; L = A * A / R; ll = L - l;
		xr = spiral_x(A, L); yr = spiral_y(A, L);
		aj = ll * ll * .5 / R / L;
		x = spiral_x(A, ll); y = spiral_y(A, ll);
		az = P0->a + zy * L * .5 / R; PZ->a = az - zy * aj;
		if (ll > 0) { PZ->r = A * A / ll; }
		else { PZ->r = 1e40; };
		PZ->x = P0->x + (x - xr) * cos(az + pi) + (y - yr) * cos(az + pi - zy * .5 * pi);
		PZ->y = P0->y + (x - xr) * sin(az + pi) + (y - yr) * sin(az + pi - zy * .5 * pi);
		break;
	case 5:   //   R1----> R2  (BIG---->SMALL)    of spiral
		A = INDATA[1]; R1 = INDATA[2]; R2 = INDATA[3]; L1 = A * A / R1; L2 = A * A / R2;
		L = L2 - L1; ll = L1 + l;
		if (ll <= L2 + 0.01) {
			ar1 = L1 * .5 / R1; ar2 = L2 * .5 / R2; ar = ll * ll * .5 / R2 / L2; aj = ar - ar1;
			xr1 = spiral_x(A, L1); yr1 = spiral_y(A, L1);
			x = spiral_x(A, ll); y = spiral_y(A, ll);
			a0 = P0->a - zy * ar1; PZ->a = a0 + zy * ar;
			if (ll > 0) { PZ->r = A * A / ll; }
			else { PZ->r = 1e40; };
			PZ->x = P0->x + (x - xr1) * cos(a0) + (y - yr1) * cos(a0 + zy * .5 * pi);
			PZ->y = P0->y + (x - xr1) * sin(a0) + (y - yr1) * sin(a0 + zy * .5 * pi);
			re = 1;
		}
		else { re = 0; };
		break;
	case 6:   //   R2----> R1  (SMALL---->big)             of spiral
		A = INDATA[1]; R2 = INDATA[2]; R1 = INDATA[3]; L = A * A * (R1 - R2) / R1 / R2;
		L1 = A * A / R1; L2 = L1 + L; ll = L2 - l;
		if (ll <= L2) {
			ar1 = L1 * .5 / R1; ar2 = L2 * .5 / R2; ar = ll * ll * .5 / R2 / L2; aj = ar - ar1;
			xr2 = spiral_x(A, L2); yr2 = spiral_y(A, L2);
			x = spiral_x(A, ll); y = spiral_y(A, ll);
			az = P0->a + zy * ar2; PZ->a = az - zy * ar;
			if (ll > 0) { PZ->r = A * A / ll; }
			else { PZ->r = 1e40; };
			PZ->x = P0->x + (x - xr2) * cos(az + pi) + (y - yr2) * cos(az + pi - zy * .5 * pi);
			PZ->y = P0->y + (x - xr2) * sin(az + pi) + (y - yr2) * sin(az + pi - zy * .5 * pi);
			re = 1;
		}
		else { re = 0; };
		break;
	};
	if (PZ->a < 0.0) PZ->a = PZ->a + 2 * pi;
	if (PZ->a > 2 * pi) PZ->a = PZ->a - 2 * pi;
	return(re);
}

int  BAS_DRAW_FUN::xlpoint_pz(double array2[][10], int trow2, struct xlpoint *PZ)
// 求某一统一里程的坐标,并返回线元类型
{
	double l, dlc, lc;
	int i=0, j=0, k=0, bz = -99;
	double INDATA2[5], jslc;
	struct xlpoint PB0, PEZ, *PBI, *PEI;
	PB0.lc = array2[0][4];
	PB0.x = array2[0][1];
	PB0.y = array2[0][2];
	PB0.a = rad(array2[0][3]);
	PB0.r = 1e40;
	PZ->x = PB0.x;
	PZ->y = PB0.y;
	PZ->a = PB0.a;
	PZ->r = PB0.r;
	dlc = array2[0][0];
	jslc = PZ->lc;
	if (jslc > array2[trow2][6])
		jslc = array2[trow2][6];
	PBI = &PB0; PEI = &PEZ;
	i = 0; bz = 99;
	do
	{
		i = i + 1;
		if (i >= trow2) bz = -99;
		if (bz > 0)
		{
			// 第 i 线元
			for (k = 0; k < 5; k++) INDATA2[k] = array2[i][k];
			l = Get_INDATA_L(INDATA2, &j);
			if (i >= trow2 - 1)
				l += 0.1;//最后一个线元加一个毫米
			PBI->a = array2[i][5];
			PBI->lc = array2[i][6];
			PBI->r = array2[i][7];
			PBI->x = array2[i][8];
			PBI->y = array2[i][9];
			// l 为该线元的长度 m

			if ((jslc <= PBI->lc + l) && (jslc >= PBI->lc))
				// 计算点在 i 线元上
			{
				lc = jslc - PBI->lc;
				if (lc <= 0.0) lc = 0.000000000001;
				xlzbxy(PBI, PZ, lc, INDATA2, 0);
				k = 999;
				bz = -99;
			};
		}  // bz>0
	} while (bz > 0);

	if (k == 999) return j; //有解 j 为 线元类型
	else
	{
		return (-9); // 无解
	};
}

//  在某层上画回旋线
AcDbObjectId   BAS_DRAW_FUN::drawspi(double INDATA[5], double l, struct xlpoint *P0, struct xlpoint *PZ, int bz, double ND)
{
	int j, k;
	double lc, b = 0;
	ACHAR buf[220];
	AcGePoint2dArray  pArr;
	AcGePoint2d ptb, pt, pte, pb, pe;

	//  if(INDATA[1]<0.00000001 || INDATA[2]<0.00000001) return 1;
	if (bz == 1 || bz == -1)
	{
		xlzbxy(P0, PZ, l*0.5 - 6 * 1, INDATA, 0); b = PZ->a + pi * 0.5*bz;
		pt[X] = PZ->y + 5 * sin(b); pt[Y] = PZ->x + 5 * cos(b); pt[Z] = 0.0;
		b = PZ->a; b = pi * 0.5 - b; if (b < 0.0) b = 2 * pi + b;
		if (b > 2 * pi) b = b - 2 * pi;
		// ads_rtos(INDATA[1]+0.00000005,2,7,ch);_tcscpy(chr,L"A=");_tcscat(chr,ch);
		// G_maketext(pt,chr,b,4,4);
	};
	pt[X] = P0->y; pt[Y] = P0->x;
	pb[X] = pt[X]; pb[Y] = pt[Y];
	xlzbxy(P0, PZ, l, INDATA, 0);
	pe[X] = PZ->y; pe[Y] = PZ->x;

	_stprintf(buf, L"%-5.0f %21.11f %20.8f %20.8f %20.8f %15.8f %12.8f %20.8f %20.8f %20.8f %20.8f\0",
		INDATA[0], INDATA[1], INDATA[2], INDATA[3], INDATA[4], dms_rad(P0->a), dms_rad(PZ->a), pb[1], pb[0], pe[1], pe[0]);

	k = (int)(l / ND + 1);
	pArr.setLogicalLength(k + 1);
	pArr[0].set(P0->y, P0->x);
	for (k = 1; k <= l / ND + 1; k++)
	{
		lc = k * ND;
		if (lc > l) lc = l;
		xlzbxy(P0, PZ, lc, INDATA, 0);
		pArr[k].set(PZ->y, PZ->x);
	};
	int NUM = pArr.length();
	AcDbPolyline *pPline = new AcDbPolyline(NUM);
	for (int i = 0; i < NUM; i++)
	{
		pPline->addVertexAt(i, pArr[i]);
		pPline->setWidthsAt(i, 0.0, 0.0);
	}

	AcDbObjectId plId;
	pPline->setColorIndex(5);


	plId = AddEntityToDbs(pPline);
	return  plId;//plh.0516

	// 加 xdata 数据 开始
	struct  resbuf  *pRb, *pTemp;
	ACHAR appName[20];
	_tcscpy(appName, L"GUADS");
	AcDbObject* pObj;
	if ((j = acdbOpenObject(pObj, plId, AcDb::kForRead)) != Acad::eOk)
	{
		ads_printf(L"acdbOpenEntity failed");
		return plId;
	};

	// Add user specified string to the Xdata.
	// CAD R2000 
	acdbRegApp(appName);
	// pRb = acutNewRb(AcDb::kDxfRegAppName);
	// CAD R14
	//	ads_regapp(appName);
	//    pRb = ads_newrb(AcDb::kDxfRegAppName);
	pTemp = pRb;
	pTemp->resval.rstring = (ACHAR*)malloc(_tcslen(appName) + 1);
	_tcscpy(pTemp->resval.rstring, appName);

	// Add user-specified string to the xdata.
	// CAD R2000
	pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
	// CAD R14
	//  pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
	pTemp = pTemp->rbnext;
	pTemp->resval.rstring = (ACHAR*)malloc(_tcslen(buf) + 1);
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
	return  plId;
}
//  在某层上画回旋线
AcDbObjectId   BAS_DRAW_FUN::drawspi(double INDATA[5], double l, struct xlpoint *P0, struct xlpoint *PZ, int bz, double ND, int color, double Lwid, ACHAR *LayerName)
{
	int k;
	double lc, b = 0;
	ACHAR buf[220];
	AcGePoint2dArray  pArr;
	AcGePoint3d ptb, pt, pte, pb, pe;

	//  if(INDATA[1]<0.00000001 || INDATA[2]<0.00000001) return 1;
	if (bz == 1 || bz == -1)
	{
		xlzbxy(P0, PZ, l*0.5 - 6 * 1, INDATA, 0); b = PZ->a + pi * 0.5*bz;
		pt[X] = PZ->y + 5 * sin(b); pt[Y] = PZ->x + 5 * cos(b); pt[Z] = 0.0;
		b = PZ->a; b = pi * 0.5 - b; if (b < 0.0) b = 2 * pi + b;
		if (b > 2 * pi) b = b - 2 * pi;
		// ads_rtos(INDATA[1]+0.00000005,2,7,ch);_tcscpy(chr,L"A=");_tcscat(chr,ch);
		// G_maketext(pt,chr,b,4,4);
	};
	pt[X] = P0->y; pt[Y] = P0->x;
	pb[X] = pt[X]; pb[Y] = pt[Y];
	xlzbxy(P0, PZ, l, INDATA, 0);
	pe[X] = PZ->y; pe[Y] = PZ->x;

	_stprintf(buf, L"%-5.0f %21.11f %20.8f %20.8f %20.8f %15.8f %12.8f %20.8f %20.8f %20.8f %20.8f\0",
		INDATA[0], INDATA[1], INDATA[2], INDATA[3], INDATA[4], dms_rad(P0->a), dms_rad(PZ->a), pb[1], pb[0], pe[1], pe[0]);

	k = (int)(l / ND + 1);
	pArr.setLogicalLength(k + 1);
	pArr[0].set(P0->y, P0->x);
	for (k = 1; k <= l / ND + 1; k++)
	{
		lc = k * ND;
		if (lc > l) lc = l;
		xlzbxy(P0, PZ, lc, INDATA, 0);
		pArr[k].set(PZ->y, PZ->x);
	};
	int NUM = pArr.length();
	AcDbPolyline *pPline = new AcDbPolyline(NUM);
	for (int i = 0; i < NUM; i++)
	{
		pPline->addVertexAt(i, pArr[i]);
		pPline->setWidthsAt(i, Lwid, Lwid);
	}

	AcDbObjectId plId;
	pPline->setColorIndex(color);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pPline->setLayer(LayerTabRecordId);
	pLayerTable->close();

	plId = AddEntityToDbs(pPline);
	return  plId;//plh.0516

	//    // 加 xdata 数据 开始
	//    struct  resbuf  *pRb, *pTemp;
	//	ACHAR appName[20];    
	//    _tcscpy(appName,L"GUADS");   
	//	AcDbObject* pObj;
	//    if ((j = acdbOpenObject(pObj, plId, AcDb::kForRead))!= Acad::eOk)
	//    {
	//        ads_printf(L"acdbOpenEntity failed");
	//        return plId ;
	//    };
	//    
	//    // Add user specified string to the Xdata.
	//    // CAD R2000 
	//    acdbRegApp(appName);  
	//    // pRb = acutNewRb(AcDb::kDxfRegAppName);
	//    // CAD R14
	////	ads_regapp(appName);
	////    pRb = ads_newrb(AcDb::kDxfRegAppName);
	//    pTemp = pRb;
	//    pTemp->resval.rstring= (ACHAR*) malloc(_tcslen(appName) + 1);
	//    _tcscpy(pTemp->resval.rstring, appName);
	//	
	//	// Add user-specified string to the xdata.
	//	// CAD R2000
	//    pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
	//	// CAD R14
	//   //  pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
	//    pTemp = pTemp->rbnext;
	//    pTemp->resval.rstring = (ACHAR*) malloc(_tcslen(buf) + 1);
	//    _tcscpy(pTemp->resval.rstring, buf);
	//	// Following code shows the use of upgradeOpen
	//    // to change the entity from read to write.
	//    
	//    pObj->upgradeOpen();	
	//    pObj->setXData(pRb);
	//    pObj->close();
	//	// CAD R2000
	//    //  acutRelRb(pRb);   
	//	// CAD R14
	////     ads_relrb(pRb);   
	//    // 加 xdata 数据 结束
	//    return  plId ;
}

AcDbObjectId  BAS_DRAW_FUN::GUXWXY(double INDATA[5], struct xlpoint *P0, struct xlpoint *PZ, int bz)
{
	int zybz;
	if (INDATA[4] < 0.0) zybz = -1;
	else zybz = 1;
	int j, k;
	double l = 0.0;
	AcGePoint3d ptb, pt, pte;
	AcDbObjectId entid;
	// if(INDATA[1]<0.00000001 || INDATA[2]<0.00000001) return  ;
	l = Get_INDATA_L(INDATA, &j);
	ptb[X] = P0->y; ptb[Y] = P0->x; ptb[Z] = 0.0; pt[Z] = 0.0; pte[Z] = 0.0;
	// if ((j<=3 &&j>=6 && INDATA[1]<=0.000000000000000001)|| (l<=0.00000000001)) return -99;
	if (bz == 1 || bz == 9)
	{
		k = bz; if (bz == 9) k = 0;
		if (j > 2 && j <= 6)
		{ //( 回旋线)
			entid = drawspi(INDATA, l, P0, PZ, k, 5.0);
			xlzbxy(P0, PZ, l, INDATA, 0);
			//ads_printf(L"4\n");
		}
		else if (j <= 2)
		{
			xlzbxy(P0, PZ, l*0.5, INDATA, 0); // 某线元中点
			pt[X] = PZ->y; pt[Y] = PZ->x;
			xlzbxy(P0, PZ, l, INDATA, 0); // 某线元终点
			pte[X] = PZ->y; pte[Y] = PZ->x;  //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
			if (j == 1)
			{
				//       acedCommandS(RTSTR, L"LINE", RTPOINT,ptb, RTPOINT, pte, RTSTR,L"",0);
				entid = makeline1(ptb, pte, 3);
			}
			else if (j == 2)
			{
				//   acedCommandS(RTSTR, L"ARC", RTPOINT,ptb,  RTPOINT, pt, RTPOINT, pte, 0);
				entid = makearc(ptb, pte, INDATA[1], zybz);
			};
		};  //J
	}
	else  if (bz == 0) { xlzbxy(P0, PZ, l, INDATA, 0); };  // 某线元终点
	// P0=PZ;
	return (entid);
}

AcDbObjectId  BAS_DRAW_FUN::GUXWXY(double INDATA[5], struct xlpoint *P0, struct xlpoint *PZ, int bz, int color, double Lwid, ACHAR *LayerName)
{
	int zybz;
	if (INDATA[4] < 0.0) zybz = -1;
	else zybz = 1;
	int j, k;
	double l = 0.0;
	AcGePoint3d ptb, pt, pte;
	AcDbObjectId entid;
	// if(INDATA[1]<0.00000001 || INDATA[2]<0.00000001) return  ;
	l = Get_INDATA_L(INDATA, &j);
	ptb[X] = P0->y; ptb[Y] = P0->x; ptb[Z] = 0.0; pt[Z] = 0.0; pte[Z] = 0.0;
	// if ((j<=3 &&j>=6 && INDATA[1]<=0.000000000000000001)|| (l<=0.00000000001)) return -99;
	if (bz == 1 || bz == 9)
	{
		k = bz; if (bz == 9) k = 0;
		if (j > 2 && j <= 6)
		{ //( 回旋线)
			entid = drawspi(INDATA, l, P0, PZ, k, 5.0, 3, Lwid, LayerName);
			xlzbxy(P0, PZ, l, INDATA, 0);
			//ads_printf(L"4\n");
		}
		else if (j <= 2)
		{
			xlzbxy(P0, PZ, l*0.5, INDATA, 0); // 某线元中点
			pt[X] = PZ->y; pt[Y] = PZ->x;
			xlzbxy(P0, PZ, l, INDATA, 0); // 某线元终点
			pte[X] = PZ->y; pte[Y] = PZ->x;  //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
			if (j == 1)
			{
				//       acedCommandS(RTSTR, L"LINE", RTPOINT,ptb, RTPOINT, pte, RTSTR,L"",0);
				entid = makeline(ptb, pte, 7, Lwid, LayerName);
			}
			else if (j == 2)
			{
				//			    acedCommandS(RTSTR, L"ARC", RTPOINT,ptb,  RTPOINT, pt, RTPOINT, pte, 0);
				if (INDATA[2] / INDATA[1] <= PI)
					entid = makearc(ptb, pte, INDATA[1], zybz, color, 1, LayerName);
				else
					entid = makearc(ptb, pte, INDATA[1], zybz, color, -1, LayerName);
				if (Lwid > 0.01)
				{
					ads_point ptt;
					ptt[X] = ptb.x, ptt[Y] = ptb.y;
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"Pedit", RTPOINT, ptt, RTSTR, L"Y", RTSTR, L"W", RTREAL, Lwid, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"Pedit", RTPOINT, ptt, RTSTR, L"Y", RTSTR, L"W", RTREAL, Lwid, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010

				}
			};
		};  //J
	}
	else  if (bz == 0) { xlzbxy(P0, PZ, l, INDATA, 0); };  // 某线元终点
	// P0=PZ;
	return (entid);
}

int BAS_DRAW_FUN::getmodel(double x1, double y1, double x2, double y2, double x3, double y3, double L1, double L2, double R)
{
	double fb, fe, db, de, a, aa, c, T0 = 0.0, T1, T2, L;
	double BPMTL[10];
	double array[4][5], INDATA[5];
	xlpoint P00, PZZ;
	int i, j, sbz = -1;
	for (i = 0; i < 4; i++) { for (j = 0; j < 5; j++) array[i][j] = 0.0; };
	for (j = 0; j < 5; j++) INDATA[j] = 0.0;
	db = xyddaa(x1, y1, x2, y2, &fb);
	de = xyddaa(x2, y2, x3, y3, &fe);
	aa = fe - fb;

	if (aa < -pi) aa = aa + 2 * pi;
	if (aa > pi) aa = aa - 2 * pi;
	if (aa > 0) { c = 1; }  //右偏
	else { c = -1; };
	a = fabs(aa);
	if (fabs(a) < 0.0000001 || fabs(pi - a) < 0.0000001)
	{
		ads_printf(L"\n该S/C型 不能求解 偏角太小(nothing to do )!!!");
		ads_printf(L"\n a1(rad)=%12.f\n", aa);
		sbz = -1;
		return RTNORM;
	};
	a = fabs(aa); a = dms_rad(a);
	if (db > 0.01&& de > 0.01&&a > 0.01)
	{
		sbz = 1;//成功
		for (i = 0; i < 10; i++) BPMTL[i] = 0.0;
		getBPMTL(a, R, L1, L2, &BPMTL[0]);
		ads_printf(L"\n"); for (i = 6; i < 10; i++)  ads_printf(L" %10.3f", BPMTL[i]);
		ads_printf(L"\n");
		T1 = BPMTL[6]; T2 = BPMTL[7]; L = BPMTL[8];
		array[0][0] = 10.0; array[0][1] = x1 + (db - T1)*cos(fb); array[0][2] = y1 + (db - T1)*sin(fb); array[0][3] = fb;
		array[1][0] = 3.01; array[1][1] = sqrt(L1*R); array[1][2] = R; array[1][4] = c;//spiral
		aa = L - L1 - L2;
		array[2][0] = 2.01; array[2][1] = R; array[2][2] = aa; array[2][4] = c;//CIRCLE
		array[3][0] = 4.01; array[3][1] = sqrt(L2*R); array[3][2] = R; array[3][4] = c;//spiral
		if ((L - L1 - L2) < 0.0)  // 圆曲线长<0
		{
			sbz = -1;
			ads_printf(L"\n   该S/C型有解 但缓和曲线重叠!!!");
			ads_printf(L"\n   圆曲线长arc_LL1=%-10.3f ", (L - L1 - L2));
		}
		else {
			sbz = 1;

			P00.x = array[0][1]; P00.y = array[0][2]; P00.a = array[0][3]; P00.lc = 0.0;
			if (L1 > 0 && L2 > 0)
			{
				for (i = 1; i <= 3; i++)
				{
					for (j = 0; j < 5; j++) INDATA[j] = array[i][j];
					ads_printf(L"\n indata=%3.1f %10.3f %10.3f %10.3f", INDATA[0], INDATA[1], INDATA[2], INDATA[4]);
					GUXWXY(INDATA, &P00, &PZZ, 1);

					P00 = PZZ;
				};
			};
		};
	};
	return sbz;
}


// 在某实体的数据结果缓表中查给定的类型码的组
struct resbuf* BAS_DRAW_FUN::entdxf(struct resbuf *entdata, int gcode)
{
	while ((entdata != NULL) && (entdata->restype != gcode)) entdata = entdata->rbnext;
	return entdata;
}


double BAS_DRAW_FUN::xyddaa(double xb, double yb, double xe, double ye, double *f)
{
	double dx, dy, dd, aa;

	dx = xe - xb; dy = ye - yb; dd = sqrt(dx*dx + dy * dy);
	if (fabs(dx) < 0.000000001) {
		if (dy >= 0.0)
			aa = pi * 0.5;
		else if (dy < 0.0)
			aa = pi * 1.5;
	}
	else { aa = atan(dy / dx); };

	if (dy >= 0 && dx < -0.000000001)
	{
		aa = aa + pi;
	}
	else if (dy < 0 && dx < -0.000000001)
	{
		aa = aa + pi;
	}
	else if (dy<0 && dx>0.000000001)
	{
		aa = aa + 2 * pi;
	};
	if (aa < -0.000000001)
	{
		aa = aa + 2 * pi;
	}
	else if (aa > 2 * pi)
	{
		aa = aa - 2 * pi;
	};
	*f = aa;
	return(dd);
}
/*
double BAS_DRAW_FUN::spiral_x(double A,double l)
{
if(A<=PRECISION)return 0.0;
double t=l*l/2/A/A;
double sum=0.0,dy=0.0;
int n=0,max=SPIRALPRECISION;
double c=2*A*A,dx=0.0;
for(n=0;n<max;n++)
{
dx=pow(-1.0,n+2)*pow(l,4*n+1)/(4*n+1)/pow(c,2*n)/factorialfun(2*n);//最后为阶乘
sum+=dx;
}
return sum;
}

double BAS_DRAW_FUN::spiral_y(double A,double l)
{
if(A<=PRECISION)return 0.0;
double t=l*l/2/A/A;
int n=0,max=SPIRALPRECISION;
double sum=0.0,dy=0.0;
for(n=1;n<=max;n++)
{
dy=pow(-1.0,n+1)*pow(t,0.5*(4*n-1))/(0.5*(4*n-1))/factorialfun(2*n-1);//最后为阶乘
sum+=dy;
}
sum=sum*A/sqrt(2.0);
return sum;
}
*/
double BAS_DRAW_FUN::spiral_x(double A, double l)
{
	if (A <= PRECISION)return 0.0;
	double t = l * l / 2 / A / A;
	double sum = 0.0, dy = 0.0;
	int n = 0, max = SPIRALPRECISION;
	double c = A * A, dx = 0.0;
	double v1, v2, v3;
	int k;
	for (n = 1; n <= max - 1; n++)
	{
		v1 = pow(-1.0, n + 1);
		v2 = l / (4.0*n - 3.0);
		v3 = pow(t, 2 * n - 2) / factorialfun(2 * n - 2);
		dx = v1 * v2*v3;
		sum += dx;
	}

	return sum;
}


double BAS_DRAW_FUN::spiral_y(double A, double l)
{
	if (A <= PRECISION)return 0.0;
	double t = l * l / 2 / A / A;
	int n = 0, max = SPIRALPRECISION;
	double sum = 0.0, dy = 0.0;
	double c = A * A;
	double v1, v2, v3;
	int k;
	for (n = 1; n <= max; n++)
	{
		v1 = pow(-1.0, n + 1);
		v2 = l / (4 * n - 1);
		v3 = pow(t, 2 * n - 1) / (factorialfun(2 * n - 1));
		dy = v1 * v2*v3;
		sum += dy;
	}

	return sum;
}


double BAS_DRAW_FUN::factorialfun(int n)
{
	double result = 1;
	for (int i = 1; i < n + 1; i++)result *= i;
	return result;
}
double BAS_DRAW_FUN::rad(double d_ms)
{
	double d;
	d = deg(d_ms);
	return (d*pi / 180.0);
}
double BAS_DRAW_FUN::dms_rad(double rad)
{
	double d_ms, ddd;
	ddd = rad * 180.0 / pi;
	d_ms = dms(ddd);
	return (d_ms);
}
double BAS_DRAW_FUN::dms(double ddd)
{
	int d, m; double s;
	d = int(ddd);
	m = int((ddd - d)*60.0);
	s = double(ddd - d)*3600.0 - m * 60.0;
	return (d + m / 100.0 + s / 10000.0);
}

double BAS_DRAW_FUN::dms(double ddd, int Nw, ACHAR* DMS_Str)
{
	int d, m;
	double s;
	d = int(ddd);
	m = int((ddd - d)*60.0);
	s = double(ddd - d)*3600.0 - m * 60.0;
	if (Nw == 0)
		_stprintf(DMS_Str, L"%d°%d′%.0lf″", d, m, s);
	else if (Nw == 1)
		_stprintf(DMS_Str, L"%d°%d′%.1lf″", d, m, s);
	else if (Nw == 2)
		_stprintf(DMS_Str, L"%d°%d′%.2lf″", d, m, s);
	else if (Nw == 3)
		_stprintf(DMS_Str, L"%d°%d′%.3lf″", d, m, s);
	else if (Nw == 4)
		_stprintf(DMS_Str, L"%d°%d′%.4lf″", d, m, s);

	return (d + m / 100.0 + s / 10000.0);
}
//两端为不完整缓和曲线的切长计算，输入转角，圆曲线半径，第一缓和段起始半径，第一缓和段长，第二缓和段终止半径，第二缓和段长，求切长
//void BAS_DRAW_FUN::get_T_Len(double zj,double R,double RB,double L1,double RE,double L2,double &T1,double &T2,double &L)
//{ // BPTL[9]=B1,B2,P1,P2,M1,M2,T1,T2,L
//	//1计算出补齐后总转角
//	double Rh,A1,A2;
//	double lo1,lo2,hhqx_zj1,hhqx_zj2,ZjAll,Lall1,Lall2,Tall1,Tall2,ZongChang;
//	double hqzj1,hqzj2,arczj,arcL,Rmin;
//	zj = fabs(rad(zj));
//	if(RB<1e6 && L1>1e-6)
//	{
//		Rh = R*RB/fabs(RB-R);
//		A1 = sqrt(Rh*L1);
//		lo1 = A1*A1/RB;
//		hhqx_zj1 = 0.5*lo1/RB;
//        Rmin = R < RB ? R : RB;
//		Lall1 = A1*A1/Rmin;
//		
//	}
//	else
//	{
//		A1 = sqrt(R*L1);
//		hhqx_zj1=0.0;
//		Lall1 = L1;
//		lo1=0.0;
//	}
//	
//	if(RE<1e6 && L2>1e-6)
//	{
//		Rh = R*RE/fabs(RE-R);
//		A2 = sqrt(Rh*L2);
//		lo2 = A2*A2/RE;
//		hhqx_zj2 = 0.5*lo2/RE;
//		Rmin = R < RE ? R : RE;
//		Lall2 = A2*A2/Rmin;
//	}
//	else
//	{
//		A2 = sqrt(R*L2);
//		hhqx_zj2=0.0;
//		Lall2 = L2; 
//		lo2=0.0;
//	}
//
//	hqzj1 = fabs(0.5*Lall1/R - hhqx_zj1);
//	hqzj2 = fabs(0.5*Lall2/R - hhqx_zj2);
//	arczj = zj - hqzj1 - hqzj2;
//	arcL = R*arczj;
////ads_printf(L"!!!! R=%lf arcL=%lf zj=%lf hqzj1=%lf hqzj2=%lf lall1=%lf zj1=%lf lall2=%lf zj2=%lf A1=%lf A2=%lf\n",
////		   R,arcL,zj,hqzj1,hqzj2,Lall1,hhqx_zj1,Lall2,hhqx_zj2,A1,A2);
//	ZjAll = (zj + hhqx_zj1 +hhqx_zj2);
//
//	//2计算出补齐后Tall1，Tall2
//    double BPMTL[9];
//	getBPMTL(dms_rad(ZjAll),R,Lall1,Lall2,BPMTL);
//
//	Tall1 = fabs(BPMTL[6]);
//	Tall2 = fabs(BPMTL[7]);
//	ZongChang = BPMTL[8];
////ads_printf(L"## %lf %lf lo1=%lf lo2=%lf lall2=%lf RB=%lf A2=%lf R=%lf\n",zj,ZjAll,lo1,lo2,Lall2,RB,A2,R);
//	//3计算出缓和曲线第一段切长th1,th2
//    ZjAll = fabs(pi-ZjAll);
//	double th1,th2,tb1,tb2;
//	if(lo1>1e-6)
//	{
//		th1 = fabs(spiral_x(A1,lo1) - spiral_y(A1,lo1) / tan(hhqx_zj1));
//		tb1 = fabs(spiral_y(A1,lo1) / sin(hhqx_zj1));
//	}
//	else
//	{
//		th1 = 0.0;
//		tb1 = 0.0;
//	}
//
//	if(lo2>1e-6)
//	{
//		th2 = fabs(spiral_x(A2,lo2) - spiral_y(A2,lo2) / tan(hhqx_zj2));
//		tb2 = fabs(spiral_y(A2,lo2) / sin(hhqx_zj2));
//	}
//	else
//	{
//		th2 = 0.0;
//		tb2 = 0.0;
//	}
//	//4解三角形，计算a1,a2
//	double ta1,ta2;
////	zj = pi - zj;
//	if(lo1>1e-6)
//		ta2= fabs((Tall1-th1)*sin(hhqx_zj1)/sin(zj+hhqx_zj2));
//	else
//	    ta2=0.0;
//
//	if(lo2>1e-6)
//		ta1= fabs((Tall2-th2)*sin(hhqx_zj2)/sin(zj+hhqx_zj1));
//	else
//	    ta1=0.0;
//
//	//5,计算T1,T2
//	if(ta1>1e-6 && ta2>1e-6)
//	{
//		T1 = fabs(fabs((Tall1-th1-ta1)*sin(ZjAll+hhqx_zj2)/sin(zj)) - tb1);
//	    T2 = fabs(fabs((Tall2-th2-ta2)*sin(ZjAll+hhqx_zj1)/sin(zj)) - tb2);
//	}
//	else if(ta1>1e-6 && ta2<1e-6)
//	{
//		T1 = Tall1 - ta1;
//	    T2 = fabs(fabs((Tall2-th2-ta2)*sin(ZjAll+hhqx_zj1)/sin(zj)) - tb2);
//	}
//	else if(ta1<1e-6 && ta2>1e-6)
//	{
//		T1 = fabs(fabs((Tall1-th1-ta1)*sin(ZjAll+hhqx_zj2)/sin(zj)) - tb1);
//	    T2 = Tall2 - ta2;
//	}
//	else
//	{
//		T1 = Tall1;
//	    T2 = Tall2 ;
//	}
//	
//
//	L = L1+L2+arcL;
//}
//两端为不完整缓和曲线的切长计算，输入转角，圆曲线半径，第一缓和段起始半径，第一缓和段长，第二缓和段终止半径，第二缓和段长，求切长
void BAS_DRAW_FUN::get_T_Len(double zj, double R, double RB, double L1, double RE, double L2, double &T1, double &T2, double &L)
{ // BPTL[9]=B1,B2,P1,P2,M1,M2,T1,T2,L
	//1计算出补齐后总转角
	double Rh, A1, A2;
	double lo1, lo2, hhqx_zj1, hhqx_zj2, ZjAll, Lall1, Lall2, Tall1, Tall2, ZongChang;
	double hqzj1, hqzj2, arczj, arcL, Rmin, Rmax, lb, le;
	zj = fabs(rad(zj));
	if (RB<1e6 && L1>1e-6)
	{
		Rh = R * RB / fabs(RB - R);
		A1 = sqrt(Rh*L1);
		lb = A1 * A1 / RB;
		le = A1 * A1 / R;
		hqzj1 = fabs(0.5*lb / RB - 0.5*le / R);
	}
	else
	{
		A1 = sqrt(R*L1);
		hhqx_zj1 = 0.0;
		Lall1 = L1;
		lo1 = 0.0;
		hqzj1 = 0.5*L1 / R;
	}

	if (RE<1e6 && L2>1e-6)
	{
		Rh = R * RE / fabs(RE - R);
		A2 = sqrt(Rh*L2);
		lb = A2 * A2 / RE;
		le = A2 * A2 / R;
		hqzj2 = fabs(0.5*lb / RE - 0.5*le / R);
	}
	else
	{
		A2 = sqrt(R*L2);
		hhqx_zj2 = 0.0;
		Lall2 = L2;
		lo2 = 0.0;
		hqzj2 = 0.5*L2 / R;
	}


	arczj = zj - hqzj1 - hqzj2;
	arcL = R * arczj;

	double t11, t12;
	if (A1 > 0.0)
		Get_HHQX_T1T2(A1, RB, R, t11, t12);
	else
		t11 = t12 = 0.0;
	double t21, t22;
	t21 = t22 = fabs(R*tan(0.5*arczj));
	double t31, t32;
	if (A2 > 0.0)
		Get_HHQX_T1T2(A2, R, RE, t31, t32);
	else
		t31 = t32 = 0.0;
	double T11, T12, T13, t;
	T11 = t11;
	T12 = fabs((t12 + t21)*sin(arczj) / sin(hqzj1 + arczj));
	t = fabs((t12 + t21)*sin(hqzj1) / sin(hqzj1 + arczj));
	T13 = fabs((t + t22 + t31)*sin(hqzj2) / sin(zj));
	T1 = T11 + T12 + T13;


	double T21, T22, T23;
	T21 = t32;
	T22 = fabs((t31 + t22)*sin(arczj) / sin(hqzj2 + arczj));
	t = fabs((t31 + t22)*sin(hqzj2) / sin(hqzj2 + arczj));
	T23 = fabs((t + t21 + t12)*sin(hqzj1) / sin(zj));
	T2 = T21 + T22 + T23;
	L = L1 + L2 + arcL;
}

void BAS_DRAW_FUN::Get_HHQX_T1T2(double A, double RB, double RE, double &TB, double &TE)
{
	double LB, LE;
	if (A < 1e-6)
	{
		TB = TE = 0.0;
	}
	double Rmax, Rmin;
	Rmax = RB > RE ? RB : RE;
	Rmin = RB < RE ? RB : RE;
	LB = A * A / Rmax;
	LE = A * A / Rmin;
	double xb, xe, ab, ae, al, bl, cl;
	ab = 0.5*LB / Rmax;
	ae = 0.5*LE / Rmin;
	xb = spiral_x(A, LB) - spiral_y(A, LB) / tan(ab);
	xe = spiral_x(A, LE) - spiral_y(A, LE) / tan(ae);
	al = fabs(xe - xb);

	double Tmax, Tmin;
	bl = al * sin(ae) / sin(fabs(ae - ab));
	Tmax = fabs(bl - spiral_y(A, LB) / sin(ab));

	cl = al * sin(ab) / sin(fabs(ae - ab));
	Tmin = fabs(cl - spiral_y(A, LE) / sin(ae));
	if (fabs(RB - Rmax) < 0.01)
	{
		TB = Tmax, TE = Tmin;
	}
	else
	{
		TB = Tmin, TE = Tmax;
	}
}


int BAS_DRAW_FUN::getBPMTL(double a, double R, double L1, double L2, double *BPMTL)
{ // BPTL[9]=B1,B2,P1,P2,M1,M2,T1,T2,L
	double A1, A2, aa, B1, B2, P1, P2, M1, M2, T1, T2, LS;
	A1 = sqrt(R * L1); A2 = sqrt(R * L2);
	aa = rad(a);

	if (R < 0.01)
	{
		*(BPMTL + 0) = 0;
		*(BPMTL + 1) = 0;
		*(BPMTL + 2) = 0;
		*(BPMTL + 3) = 0;
		*(BPMTL + 4) = 0;
		*(BPMTL + 5) = 0;
		*(BPMTL + 6) = 0;
		*(BPMTL + 7) = 0;
		*(BPMTL + 8) = 0;
		return 1;
	}

	B1 = L1 * 0.5 / R;
	B2 = L2 * 0.5 / R;

	if (L1 < 0.0001) { P1 = 0.0; M1 = 0.0; }
	else { P1 = spiral_p(A1, L1); M1 = spiral_xm(A1, L1); };
	if (L2 < 0.0001) { P2 = 0.0; M2 = 0.0; }
	else { P2 = spiral_p(A2, L2); M2 = spiral_xm(A2, L2); };
	// P1=spiral_p(A1,L1);P2=spiral_p(A2,L2);//P1,P2
	// M1=spiral_xm(A1,L1);M2=spiral_xm(A2,L2);// M1,M2
	T1 = (R + P1) * tan(aa * 0.5) + M1 - (P1 - P2) / sin(aa);
	T2 = (R + P2) * tan(aa * 0.5) + M2 + (P1 - P2) / sin(aa);
	LS = (aa - B1 - B2) * R + L1 + L2;

	*(BPMTL + 0) = B1;
	*(BPMTL + 1) = B2;
	*(BPMTL + 2) = P1;
	*(BPMTL + 3) = P2;
	*(BPMTL + 4) = M1;
	*(BPMTL + 5) = M2;
	*(BPMTL + 6) = T1;
	*(BPMTL + 7) = T2;
	*(BPMTL + 8) = LS;
	return(1);
}

double BAS_DRAW_FUN::deg(double d_ms)
{
	int d, m;
	double s;
	d = int(d_ms + 0.000000001); m = int((d_ms - d)*100.0 + 0.00000000001);
	s = d_ms * 10000.0 - d * 10000.0 - m * 100.0;
	return (d + m / 60.0 + s / 3600.0);
}

double BAS_DRAW_FUN::spiral_xm(double A, double L)
{
	double R, t, XM;
	R = A * A / L; t = A * A / (2.0*R*R);
	XM = spiral_x(A, L) - R * sin(t);
	return(XM);
}

double BAS_DRAW_FUN::spiral_p(double A, double L)
{
	double R, t, p;
	R = A * A / L; t = A * A / (2.0*R*R);
	p = spiral_y(A, L) + R * cos(t) - R;
	return(p);
}
void BAS_DRAW_FUN::printList(struct resbuf* pBuf)
{
	int rt, i;
	ACHAR buf[133];

	for (i = 0; pBuf != NULL; i++, pBuf = pBuf->rbnext) {
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
void  BAS_DRAW_FUN::iterate(AcDbObjectId plineId)
{
	AcDb2dPolyline *pPline;
	/* acdbOpenObject(pPline, plineId, AcDb::kForRead);*/
	if (acdbOpenObject(pPline, plineId, AcDb::kForRead) != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	AcDbObjectIterator *pVertIter = pPline->vertexIterator();
	pPline->close();  // Finished with the pline header.

	AcDb2dVertex *pVertex;
	AcGePoint3d location;
	AcDbObjectId vertexObjId;
	for (int vertexNumber = 0; !pVertIter->done();
		vertexNumber++, pVertIter->step())
	{
		vertexObjId = pVertIter->objectId();
		//acdbOpenObject(pVertex, vertexObjId,
		//    AcDb::kForRead);
		if (acdbOpenObject(pVertex, vertexObjId, AcDb::kForRead) != Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		location = pVertex->position();
		pVertex->close();

		ads_printf(L"\nVertex #%d's location is"
			L" : %0.3f, %0.3f, %0.3f", vertexNumber,
			location[X], location[Y], location[Z]);
	}
	delete pVertIter;
}
AcDbObjectId  BAS_DRAW_FUN::createPl_xlpm(int trow, double arrayd[100][6])
{
	// Set trow vertex locations for the pline
	//

	AcGePoint3dArray ptArr;
	ptArr.setLogicalLength(trow);
	ads_printf(L"trow=%d \n", trow);
	for (int i = 0; i < trow; i++) ptArr[i].set(arrayd[i][2], arrayd[i][1], 0.0);

	//
	AcDb2dPolyline *pNewPline = new AcDb2dPolyline(AcDb::k2dSimplePoly, ptArr, 0.0, Adesk::kFalse);

	pNewPline->setColorIndex(1);
	pNewPline->setLayer(L"0");
	// Get a pointer to a BlockTable Object   
	AcDbObjectId  plId;
	plId = AddEntityToDbs(pNewPline);

	return plId;
}

ACHAR* BAS_DRAW_FUN::LCchr(ACHAR *GH, double lc, int NLC, int mode = 1)
{
	long int zlc, gl;
	double bm;
	ACHAR  ch1[20], ch[40];
	bm = lc;
	//	zlc=(long int)(lc/1000.0); 
	zlc = (long int)((lc + 0.0001) / 1000.0);
	bm = bm - zlc * 1000.0;
	gl = zlc;
	ads_rtos(gl, 2, 0, ch1);
	_tcscpy(ch, GH);
	_tcscat(ch, ch1);
	// _tcscat(ch,L"+");
	bm = bm + 1000.0;
	ads_rtos(bm, 2, NLC, ch1);
	ch1[0] = '+';
	_tcscat(ch, ch1);
	return &ch[0];
}

CString BAS_DRAW_FUN::DoubleToStr(double xz)//标注时将数值转为串，同时去掉无效的零
{
	int head;
	CString str;

	head = floor(fabs(xz));
	if (fabs(xz) - head < 0.001)
		str.Format(L"%0.0lf", xz);
	else
	{
		head = floor(fabs((xz * 10)));
		if (fabs(xz * 10) - head < 0.01)
			str.Format(L"%0.1lf", xz);
		else
		{
			head = floor(fabs((xz * 100)));
			if (fabs(xz * 100) - head < 0.1)
				str.Format(L"%0.2lf", xz);
			else
				str.Format(L"%0.3lf", xz);
		}
	}
	return str;
}

CString BAS_DRAW_FUN::LCchr(CString GH, double lc, int NLC)
{
	long int zlc, gl;
	double bm;
	ACHAR  ch1[20], ch[40];
	lc = fabs(lc);
	bm = lc;
	//	zlc=(long int)(lc/1000.0); 
	zlc = (long int)((lc + 0.0001) / 1000.0);
	bm = bm - zlc * 1000.0;
	gl = zlc;
	ads_rtos(gl, 2, 0, ch1);
	_tcscpy(ch, GH);
	_tcscat(ch, ch1);
	// _tcscat(ch,L"+");
	bm = bm + 1000.0;
	RToS(bm, NLC, ch1);
	ch1[0] = '+';
	_tcscat(ch, ch1);
	return &ch[0];
}
void BAS_DRAW_FUN::calptan(double x1, double y1, double x2, double y2, double x3, double y3, double xt, double yt, double *tanl, double *R, double *xc, double *yc, double *ang, double *startfw, double *endfw)
{
	double sfw, efw, tfw, lt, alpha, bta, gma, alphac;
	int inf;
	ads_point pt[4];
	Spoint(pt[0], x1, y1, 0);
	Spoint(pt[1], x2, y2, 0);
	Spoint(pt[2], x3, y3, 0);
	Spoint(pt[3], xt, yt, 0);
	sfw = ads_angle(pt[0], pt[1]);
	efw = ads_angle(pt[1], pt[2]);
	tfw = ads_angle(pt[1], pt[3]);
	lt = ads_distance(pt[1], pt[3]);
	if (sfw <= pi)
	{
		if (efw > sfw && efw <= sfw + pi) { alpha = efw - sfw; inf = -1; }
		else { alpha = sfw - efw; inf = 1; };
		if (alpha < 0.0) alpha = alpha + 2.0*pi;
	}
	else if (sfw > pi)
	{
		if (efw > sfw - pi && efw < sfw) { alpha = sfw - efw; inf = 1; }
		else { alpha = efw - sfw; inf = -1; };
		if (alpha < 0.0) alpha = alpha + 2.0*pi;
	};
	alpha = pi - alpha;
	alpha = 0.5*alpha;
	alphac = sfw + pi + alpha * inf;

	if (sfw <= pi)
	{
		if (tfw > sfw && tfw <= sfw + pi) { bta = tfw - sfw; inf = -1; }
		else { bta = sfw - tfw; inf = 1; };
		if (bta < 0.0) bta = bta + 2.0*pi;
	}
	else if (sfw > pi)
	{
		if (tfw > sfw - pi && tfw < sfw) { bta = sfw - tfw; inf = 1; }
		else { bta = tfw - sfw; inf = -1; };
		if (bta < 0.0) bta = bta + 2.0*pi;
	};
	bta = pi - bta;
	gma = alpha - bta;
	double b, c;
	b = -2.0*cos(gma)*lt / cos(alpha);
	c = lt * lt;

	*tanl = 0.5*(-b + sqrt(b*b - 4.0*c));
	*R = (*tanl) * tan(alpha);
	acutPrintf(L"R=%lf\n", *R);

	double tmp;
	tmp = (*tanl) / cos(alpha);
	*xc = x2 + tmp * cos(alphac);
	*yc = y2 + tmp * sin(alphac);
	*ang = alpha;

	if (inf == 1)
	{
		*startfw = efw + 0.5*pi;
		*endfw = sfw + 0.5*pi;
	}
	else
	{
		*startfw = sfw - 0.5*pi;
		*endfw = efw - 0.5*pi;
	};
}

double BAS_DRAW_FUN::get_l0(double SR, double Design_V)
{
	double l0 = 0.0;
	//   double Design_V = 80; 
	int KNLB = 0;

	if (Design_V > 299)
	{
		if (KNLB == 0)
		{
			if (fabs(SR - 5500) < 0.1) l0 = 700;
			else if (fabs(SR - 6000) < 0.1) l0 = 700;
			else if (fabs(SR - 7000) < 0.1) l0 = 670;
			else if (fabs(SR - 8000) < 0.1) l0 = 570;
			else if (fabs(SR - 9000) < 0.1) l0 = 490;
			else if (fabs(SR - 10000) < 0.1) l0 = 430;
			else if (fabs(SR - 11000) < 0.1) l0 = 370;
			else if (fabs(SR - 12000) < 0.1) l0 = 330;
			else if (fabs(SR - 14000) < 0.1) l0 = 280;
		}
		else if (KNLB == 1)
		{
			if (fabs(SR - 5500) < 0.1) l0 = 610;
			else if (fabs(SR - 6000) < 0.1) l0 = 570;
			else if (fabs(SR - 7000) < 0.1) l0 = 540;
			else if (fabs(SR - 8000) < 0.1) l0 = 470;
			else if (fabs(SR - 9000) < 0.1) l0 = 400;
			else if (fabs(SR - 10000) < 0.1) l0 = 350;
			else if (fabs(SR - 11000) < 0.1) l0 = 300;
			else if (fabs(SR - 12000) < 0.1) l0 = 270;
			else if (fabs(SR - 14000) < 0.1) l0 = 250;
		}
		else
		{
			if (fabs(SR - 5500) < 0.1) l0 = 490;
			else if (fabs(SR - 6000) < 0.1) l0 = 460;
			else if (fabs(SR - 7000) < 0.1) l0 = 430;
			else if (fabs(SR - 8000) < 0.1) l0 = 380;
			else if (fabs(SR - 9000) < 0.1) l0 = 320;
			else if (fabs(SR - 10000) < 0.1) l0 = 270;
			else if (fabs(SR - 11000) < 0.1) l0 = 220;
			else if (fabs(SR - 12000) < 0.1) l0 = 190;
			else if (fabs(SR - 14000) < 0.1) l0 = 150;
		}
	}
	else if (fabs(Design_V - 250) < 0.1)
	{
		if (KNLB == 0)
		{
			if (fabs(SR - 3500) < 0.1) l0 = 420;
			else if (fabs(SR - 4000) < 0.1) l0 = 390;
			else if (fabs(SR - 4500) < 0.1) l0 = 360;
			else if (fabs(SR - 5000) < 0.1) l0 = 330;
			else if (fabs(SR - 5500) < 0.1) l0 = 300;
			else if (fabs(SR - 6000) < 0.1) l0 = 270;
			else if (fabs(SR - 6500) < 0.1) l0 = 240;
			else if (fabs(SR - 7000) < 0.1) l0 = 210;
			else if (fabs(SR - 8000) < 0.1) l0 = 190;
			else if (fabs(SR - 9000) < 0.1) l0 = 170;
			else if (fabs(SR - 10000) < 0.1) l0 = 150;
			else if (fabs(SR - 11000) < 0.1) l0 = 130;
			else if (fabs(SR - 12000) < 0.1) l0 = 110;
		}
		else if (KNLB == 1)
		{
			if (fabs(SR - 3500) < 0.1) l0 = 340;
			else if (fabs(SR - 4000) < 0.1) l0 = 320;
			else if (fabs(SR - 4500) < 0.1) l0 = 300;
			else if (fabs(SR - 5000) < 0.1) l0 = 280;
			else if (fabs(SR - 5500) < 0.1) l0 = 260;
			else if (fabs(SR - 6000) < 0.1) l0 = 230;
			else if (fabs(SR - 6500) < 0.1) l0 = 200;
			else if (fabs(SR - 7000) < 0.1) l0 = 180;
			else if (fabs(SR - 8000) < 0.1) l0 = 160;
			else if (fabs(SR - 9000) < 0.1) l0 = 140;
			else if (fabs(SR - 10000) < 0.1) l0 = 120;
			else if (fabs(SR - 11000) < 0.1) l0 = 100;
			else if (fabs(SR - 12000) < 0.1) l0 = 90;
		}
		else
		{
			if (fabs(SR - 3500) < 0.1) l0 = 300;
			else if (fabs(SR - 4000) < 0.1) l0 = 280;
			else if (fabs(SR - 4500) < 0.1) l0 = 260;
			else if (fabs(SR - 5000) < 0.1) l0 = 240;
			else if (fabs(SR - 5500) < 0.1) l0 = 220;
			else if (fabs(SR - 6000) < 0.1) l0 = 200;
			else if (fabs(SR - 6500) < 0.1) l0 = 180;
			else if (fabs(SR - 7000) < 0.1) l0 = 160;
			else if (fabs(SR - 8000) < 0.1) l0 = 140;
			else if (fabs(SR - 9000) < 0.1) l0 = 120;
			else if (fabs(SR - 10000) < 0.1) l0 = 100;
			else if (fabs(SR - 11000) < 0.1) l0 = 90;
			else if (fabs(SR - 12000) < 0.1) l0 = 80;
		}
	}
	else if (fabs(Design_V - 200) < 0.1)
	{
		if (KNLB == 0)
		{
			if (fabs(SR - 2200) < 0.1) l0 = 340;
			else if (fabs(SR - 2500) < 0.1) l0 = 320;
			else if (fabs(SR - 2800) < 0.1) l0 = 300;
			else if (fabs(SR - 3000) < 0.1) l0 = 280;
			else if (fabs(SR - 3500) < 0.1) l0 = 260;
			else if (fabs(SR - 4000) < 0.1) l0 = 240;
			else if (fabs(SR - 4500) < 0.1) l0 = 210;
			else if (fabs(SR - 5000) < 0.1) l0 = 180;
			else if (fabs(SR - 5500) < 0.1) l0 = 160;
			else if (fabs(SR - 6000) < 0.1) l0 = 140;
			else if (fabs(SR - 7000) < 0.1) l0 = 120;
			else if (fabs(SR - 8000) < 0.1) l0 = 100;
			else if (fabs(SR - 9000) < 0.1) l0 = 90;
			else if (fabs(SR - 10000) < 0.1) l0 = 80;
			else if (fabs(SR - 11000) < 0.1) l0 = 70;
			else if (fabs(SR - 12000) < 0.1) l0 = 60;
		}
		else if (KNLB == 1)
		{
			if (fabs(SR - 2200) < 0.1) l0 = 270;
			else if (fabs(SR - 2500) < 0.1) l0 = 250;
			else if (fabs(SR - 2800) < 0.1) l0 = 240;
			else if (fabs(SR - 3000) < 0.1) l0 = 230;
			else if (fabs(SR - 3500) < 0.1) l0 = 210;
			else if (fabs(SR - 4000) < 0.1) l0 = 190;
			else if (fabs(SR - 4500) < 0.1) l0 = 170;
			else if (fabs(SR - 5000) < 0.1) l0 = 150;
			else if (fabs(SR - 5500) < 0.1) l0 = 130;
			else if (fabs(SR - 6000) < 0.1) l0 = 110;
			else if (fabs(SR - 7000) < 0.1) l0 = 100;
			else if (fabs(SR - 8000) < 0.1) l0 = 90;
			else if (fabs(SR - 9000) < 0.1) l0 = 80;
			else if (fabs(SR - 10000) < 0.1) l0 = 70;
			else if (fabs(SR - 11000) < 0.1) l0 = 60;
			else if (fabs(SR - 12000) < 0.1) l0 = 50;
		}
		else
		{
			if (fabs(SR - 2200) < 0.1) l0 = 240;
			else if (fabs(SR - 2500) < 0.1) l0 = 230;
			else if (fabs(SR - 2800) < 0.1) l0 = 220;
			else if (fabs(SR - 3000) < 0.1) l0 = 210;
			else if (fabs(SR - 3500) < 0.1) l0 = 190;
			else if (fabs(SR - 4000) < 0.1) l0 = 170;
			else if (fabs(SR - 4500) < 0.1) l0 = 150;
			else if (fabs(SR - 5000) < 0.1) l0 = 130;
			else if (fabs(SR - 5500) < 0.1) l0 = 110;
			else if (fabs(SR - 6000) < 0.1) l0 = 100;
			else if (fabs(SR - 7000) < 0.1) l0 = 90;
			else if (fabs(SR - 8000) < 0.1) l0 = 80;
			else if (fabs(SR - 9000) < 0.1) l0 = 70;
			else if (fabs(SR - 10000) < 0.1) l0 = 60;
			else if (fabs(SR - 11000) < 0.1) l0 = 50;
			else if (fabs(SR - 12000) < 0.1) l0 = 40;
		}
	}
	else if (fabs(Design_V - 160) < 0.1)
	{
		if (fabs(SR - 1600) < 0.1) l0 = 190;
		else if (fabs(SR - 2000) < 0.1) l0 = 150;
		else if (fabs(SR - 2500) < 0.1) l0 = 120;
		else if (fabs(SR - 3000) < 0.1) l0 = 100;
		else if (fabs(SR - 3500) < 0.1) l0 = 80;
		else if (fabs(SR - 4000) < 0.1) l0 = 80;
		else if (fabs(SR - 4500) < 0.1) l0 = 70;
		else if (fabs(SR - 5000) < 0.1) l0 = 60;
		else if (fabs(SR - 6000) < 0.1) l0 = 60;
		else if (fabs(SR - 8000) < 0.1) l0 = 40;
		else if (fabs(SR - 10000) < 0.1) l0 = 40;
		else if (fabs(SR - 12000) < 0.1) l0 = 40;

	}
	else if (fabs(Design_V - 140) < 0.1)
	{
		if (KNLB == 0)
		{
			if (fabs(SR - 1200) < 0.1) l0 = 150;
			else if (fabs(SR - 1400) < 0.1) l0 = 130;
			else if (fabs(SR - 1600) < 0.1) l0 = 110;
			else if (fabs(SR - 1800) < 0.1) l0 = 100;
			else if (fabs(SR - 2000) < 0.1) l0 = 90;
			else if (fabs(SR - 2500) < 0.1) l0 = 80;
			else if (fabs(SR - 3000) < 0.1) l0 = 70;
			else if (fabs(SR - 4000) < 0.1) l0 = 60;
			else if (fabs(SR - 5000) < 0.1) l0 = 60;
			else if (fabs(SR - 6000) < 0.1) l0 = 50;
			else if (fabs(SR - 8000) < 0.1) l0 = 40;
			else if (fabs(SR - 10000) < 0.1) l0 = 30;
		}
		else
		{
			if (fabs(SR - 1200) < 0.1) l0 = 130;
			else if (fabs(SR - 1400) < 0.1) l0 = 110;
			else if (fabs(SR - 1600) < 0.1) l0 = 100;
			else if (fabs(SR - 1800) < 0.1) l0 = 80;
			else if (fabs(SR - 2000) < 0.1) l0 = 80;
			else if (fabs(SR - 2500) < 0.1) l0 = 70;
			else if (fabs(SR - 3000) < 0.1) l0 = 50;
			else if (fabs(SR - 4000) < 0.1) l0 = 40;
			else if (fabs(SR - 5000) < 0.1) l0 = 40;
			else if (fabs(SR - 6000) < 0.1) l0 = 30;
			else if (fabs(SR - 8000) < 0.1) l0 = 20;
			else if (fabs(SR - 10000) < 0.1) l0 = 20;
		};
	}
	else if (fabs(Design_V - 120) < 0.1)
	{
		if (KNLB == 0)
		{
			if (fabs(SR - 800) < 0.1) l0 = 150;
			else if (fabs(SR - 1000) < 0.1) l0 = 120;
			else if (fabs(SR - 1200) < 0.1) l0 = 90;
			else if (fabs(SR - 1400) < 0.1) l0 = 80;
			else if (fabs(SR - 1600) < 0.1) l0 = 70;
			else if (fabs(SR - 1800) < 0.1) l0 = 70;
			else if (fabs(SR - 2000) < 0.1) l0 = 60;
			else if (fabs(SR - 2500) < 0.1) l0 = 60;
			else if (fabs(SR - 3000) < 0.1) l0 = 50;
			else if (fabs(SR - 4000) < 0.1) l0 = 50;
			else if (fabs(SR - 5000) < 0.1) l0 = 40;
			else if (fabs(SR - 6000) < 0.1) l0 = 30;
			else if (fabs(SR - 8000) < 0.1) l0 = 30;
			else if (fabs(SR - 1000) < 0.1) l0 = 20;
		}
		else
		{
			if (fabs(SR - 800) < 0.1) l0 = 130;
			else if (fabs(SR - 1000) < 0.1) l0 = 100;
			else if (fabs(SR - 1200) < 0.1) l0 = 80;
			else if (fabs(SR - 1400) < 0.1) l0 = 70;
			else if (fabs(SR - 1600) < 0.1) l0 = 60;
			else if (fabs(SR - 1800) < 0.1) l0 = 60;
			else if (fabs(SR - 2000) < 0.1) l0 = 50;
			else if (fabs(SR - 2500) < 0.1) l0 = 40;
			else if (fabs(SR - 3000) < 0.1) l0 = 40;
			else if (fabs(SR - 4000) < 0.1) l0 = 30;
			else if (fabs(SR - 5000) < 0.1) l0 = 30;
			else if (fabs(SR - 6000) < 0.1) l0 = 20;
			else if (fabs(SR - 8000) < 0.1) l0 = 20;
			else if (fabs(SR - 10000) < 0.1) l0 = 20;
		};
	}
	else if (fabs(Design_V - 100) < 0.1)
	{
		if (KNLB == 0)
		{
			if (fabs(SR - 550) < 0.1) l0 = 130;
			else if (fabs(SR - 600) < 0.1) l0 = 120;
			else if (fabs(SR - 700) < 0.1) l0 = 100;
			else if (fabs(SR - 800) < 0.1) l0 = 80;
			else if (fabs(SR - 1000) < 0.1) l0 = 70;
			else if (fabs(SR - 1200) < 0.1) l0 = 60;
			else if (fabs(SR - 1400) < 0.1) l0 = 60;
			else if (fabs(SR - 1600) < 0.1) l0 = 50;
			else if (fabs(SR - 1800) < 0.1) l0 = 50;
			else if (fabs(SR - 2000) < 0.1) l0 = 50;
			else if (fabs(SR - 2500) < 0.1) l0 = 40;
			else if (fabs(SR - 3000) < 0.1) l0 = 40;
			else if (fabs(SR - 4000) < 0.1) l0 = 30;
			else if (fabs(SR - 5000) < 0.1) l0 = 20;
			else if (fabs(SR - 6000) < 0.1) l0 = 20;
			else if (fabs(SR - 8000) < 0.1) l0 = 20;
			else if (fabs(SR - 10000) < 0.1) l0 = 20;
		}
		else
		{
			if (fabs(SR - 550) < 0.1) l0 = 110;
			else if (fabs(SR - 600) < 0.1) l0 = 100;
			else if (fabs(SR - 700) < 0.1) l0 = 90;
			else if (fabs(SR - 800) < 0.1) l0 = 70;
			else if (fabs(SR - 1000) < 0.1) l0 = 60;
			else if (fabs(SR - 1200) < 0.1) l0 = 50;
			else if (fabs(SR - 1400) < 0.1) l0 = 40;
			else if (fabs(SR - 1600) < 0.1) l0 = 40;
			else if (fabs(SR - 1800) < 0.1) l0 = 40;
			else if (fabs(SR - 2000) < 0.1) l0 = 40;
			else if (fabs(SR - 2500) < 0.1) l0 = 30;
			else if (fabs(SR - 3000) < 0.1) l0 = 20;
			else if (fabs(SR - 4000) < 0.1) l0 = 20;
			else if (fabs(SR - 5000) < 0.1) l0 = 20;
			else if (fabs(SR - 6000) < 0.1) l0 = 20;
			else if (fabs(SR - 8000) < 0.1) l0 = 20;
			else if (fabs(SR - 10000) < 0.1) l0 = 20;
		};
	}
	else if (fabs(Design_V - 80) < 0.1)
	{
		if (KNLB == 0)
		{
			if (SR < 300) l0 = 0.0;
			else if (SR <= 351) l0 = 100;
			else if (SR <= 401) l0 = 90;
			else if (SR <= 451) l0 = 80;
			else if (SR <= 501) l0 = 60;
			else if (SR <= 551) l0 = 60;
			else if (SR <= 601) l0 = 60;
			else if (SR <= 701) l0 = 50;
			else if (SR <= 801) l0 = 50;
			else if (SR <= 1001) l0 = 40;
			else if (SR <= 1201) l0 = 40;
			else if (SR <= 1401) l0 = 40;
			else if (SR <= 1601) l0 = 40;
			else if (SR <= 1801) l0 = 30;
			else if (SR <= 2001) l0 = 30;
			else if (SR <= 2501) l0 = 30;
			else if (SR <= 3001) l0 = 20;
			else if (SR <= 4001) l0 = 20;
			else if (SR <= 5001) l0 = 20;
			else if (SR <= 6001) l0 = 20;
			else if (SR <= 8001) l0 = 20;
			else if (SR <= 10001) l0 = 20;
		}
		else
		{
			if (SR <= 351) l0 = 90;
			else if (SR <= 401) l0 = 80;
			else if (SR <= 451) l0 = 70;
			else if (SR <= 501) l0 = 60;
			else if (SR <= 551) l0 = 50;
			else if (SR <= 601) l0 = 50;
			else if (SR <= 701) l0 = 40;
			else if (SR <= 801) l0 = 40;
			else if (SR <= 1001) l0 = 30;
			else if (SR <= 1201) l0 = 30;
			else if (SR <= 1401) l0 = 20;
			else if (SR <= 1601) l0 = 20;
			else if (SR <= 1801) l0 = 20;
			else if (SR <= 2001) l0 = 20;
			else if (SR <= 2501) l0 = 20;
			else if (SR <= 3001) l0 = 20;
			else if (SR <= 4001) l0 = 20;
			else if (SR <= 5001) l0 = 20;
			else if (SR <= 6001) l0 = 20;
			else if (SR <= 8001) l0 = 20;
			else if (SR <= 10001) l0 = 20;
		};
	}
	else if (fabs(Design_V - 70) < 0.1)
	{
		if (SR < 301) l0 = 70;
		else if (SR < 351) l0 = 60;
		else if (SR < 401) l0 = 50;
		else if (SR < 451) l0 = 50;
		else if (SR < 501) l0 = 40;
		else if (SR < 551) l0 = 40;
		else if (SR < 601) l0 = 40;
		else if (SR < 701) l0 = 30;
		else if (SR < 801) l0 = 30;
		else l0 = 20;
	}
	else if (fabs(Design_V - 60) < 0.1)
	{
		if (SR < 301) l0 = 50;
		else if (SR < 351) l0 = 40;
		else if (SR < 401) l0 = 40;
		else if (SR < 451) l0 = 40;
		else if (SR < 501) l0 = 30;
		else if (SR < 551) l0 = 30;
		else if (SR < 601) l0 = 30;
		else if (SR < 2501) l0 = 20;
		else  l0 = 0;
	}
	else if (fabs(Design_V - 55) < 0.1)
	{
		if (SR < 251) l0 = 50;
		else if (SR < 301) l0 = 40;
		else if (SR < 351) l0 = 40;
		else if (SR < 551) l0 = 30;
		else if (SR < 2001) l0 = 20;
		else  l0 = 0;
	}
	else if (fabs(Design_V - 45) < 0.1)
	{
		if (SR < 251) l0 = 40;
		else if (SR < 351) l0 = 30;
		else if (SR < 1501) l0 = 20;
		else  l0 = 0;
	}
	else if (fabs(Design_V - 40) < 0.1)
	{
		if (SR < 151) l0 = 50;
		else if (SR < 201) l0 = 40;
		else if (SR < 301) l0 = 30;
		else if (SR < 1201) l0 = 20;
		else  l0 = 0;
	}
	else if (fabs(Design_V - 30) < 0.1)
	{
		if (SR < 151) l0 = 30;
		else if (SR < 601) l0 = 20;
		else  l0 = 0;
	}

	//	ads_printf(L"v =%9d\n",Design_V);
	//	ads_printf(L"KNLB = %9d\n",KNLB);
	//	ads_printf(L"l0=%10.3lf\n",l0);
	return l0;
}
double BAS_DRAW_FUN::get_R(double SR, double Design_V)//按照半径系列取半径值，寻找最接近半径
{
	double l0 = 0.0;
	//   double Design_V = 80; 
	int KNLB = 0;
	double R[30];
	int NR, i;

	if (Design_V > 299)
	{
		R[0] = 5500, R[1] = 6000, R[2] = 7000, R[3] = 8000, R[4] = 9000, R[5] = 10000, R[6] = 11000, R[7] = 12000, R[8] = 14000;
		NR = 9;
	}
	else if (fabs(Design_V - 250) < 0.1)
	{
		R[0] = 3500, R[1] = 4000, R[2] = 4500, R[3] = 5000, R[4] = 5500, R[5] = 6000, R[6] = 6500, R[7] = 7000, R[8] = 8000, R[9] = 9000, R[10] = 10000, R[11] = 11000, R[12] = 12000;
		NR = 13;
	}
	else if (fabs(Design_V - 200) < 0.1)
	{
		R[0] = 2200, R[1] = 2500, R[2] = 2800, R[3] = 3000, R[4] = 3500, R[5] = 4000, R[6] = 4500, R[7] = 5000, R[8] = 5500, R[9] = 6000, R[10] = 7000, R[11] = 8000, R[12] = 9000, R[13] = 10000, R[14] = 11000, R[15] = 12000;
		NR = 16;
	}
	else if (fabs(Design_V - 160) < 0.1)
	{
		R[0] = 1600, R[1] = 2000, R[2] = 2500, R[3] = 3000, R[4] = 3500, R[5] = 4000, R[6] = 4500, R[7] = 5000, R[8] = 6000, R[9] = 8000, R[10] = 10000, R[11] = 12000, R[12] = 12000;
		NR = 13;
	}
	else if (fabs(Design_V - 140) < 0.1)
	{
		R[0] = 1200, R[1] = 1400, R[2] = 1600, R[3] = 1800, R[4] = 2000, R[5] = 2500, R[6] = 3000, R[7] = 4000, R[8] = 5000, R[9] = 6000, R[10] = 8000, R[11] = 10000;
		NR = 12;
	}
	else if (fabs(Design_V - 120) < 0.1)
	{
		R[0] = 800, R[1] = 1000, R[2] = 1200, R[3] = 1400, R[4] = 1600, R[5] = 1800, R[6] = 2000, R[7] = 2500, R[8] = 3000, R[9] = 4000, R[10] = 5000, R[11] = 6000, R[12] = 8000, R[13] = 10000;
		NR = 14;
	}
	else if (fabs(Design_V - 100) < 0.1)
	{
		R[0] = 550, R[1] = 600, R[2] = 700, R[3] = 800, R[4] = 1000, R[5] = 1200, R[6] = 1400, R[7] = 1600, R[8] = 1800, R[9] = 2000, R[10] = 2500, R[11] = 3000, R[12] = 4000, R[13] = 5000, R[14] = 6000, R[15] = 8000, R[16] = 10000;
		NR = 17;
	}
	else if (fabs(Design_V - 80) < 0.1)
	{
		R[0] = 350, R[1] = 400, R[2] = 450, R[3] = 500, R[4] = 550, R[5] = 600, R[6] = 700, R[7] = 800, R[8] = 1000, R[9] = 1200, R[10] = 1400, R[11] = 1600, R[12] = 1800, R[13] = 2000, R[14] = 2500, R[15] = 3000, R[16] = 4000, R[17] = 5000, R[18] = 6000, R[19] = 8000, R[20] = 10000;
		NR = 21;
	}
	else if (fabs(Design_V - 70) < 0.1)
	{
		R[0] = 300, R[1] = 350, R[2] = 400, R[3] = 450, R[4] = 500, R[5] = 550, R[6] = 600, R[7] = 700, R[8] = 800, R[9] = 1000, R[10] = 1200, R[11] = 1500, R[12] = 2000, R[13] = 2500, R[14] = 3000, R[15] = 4000;
		NR = 16;
	}
	else if (fabs(Design_V - 60) < 0.1)
	{
		R[0] = 300, R[1] = 350, R[2] = 400, R[3] = 450, R[4] = 500, R[5] = 550, R[6] = 600, R[7] = 700, R[8] = 800, R[9] = 1000, R[10] = 1200, R[11] = 1500, R[12] = 2000, R[13] = 2500, R[14] = 3000, R[15] = 4000;
		NR = 16;
	}
	else if (fabs(Design_V - 55) < 0.1)
	{
		R[0] = 250, R[1] = 300, R[2] = 350, R[3] = 400, R[4] = 450, R[5] = 500, R[6] = 550, R[7] = 600, R[8] = 700, R[9] = 800, R[10] = 1000, R[11] = 1200, R[12] = 1500, R[13] = 2000, R[14] = 2500, R[15] = 3000, R[16] = 4000;
		NR = 17;
	}
	else if (fabs(Design_V - 45) < 0.1)
	{
		R[0] = 250, R[1] = 300, R[2] = 350, R[3] = 400, R[4] = 450, R[5] = 500, R[6] = 550, R[7] = 600, R[8] = 700, R[9] = 800, R[10] = 1000, R[11] = 1200, R[12] = 1500, R[13] = 2000, R[14] = 2500, R[15] = 3000, R[16] = 4000;
		NR = 17;
	}
	else if (fabs(Design_V - 40) < 0.1)
	{
		R[0] = 150, R[1] = 180, R[2] = 200, R[3] = 250, R[4] = 300, R[5] = 350, R[6] = 400, R[7] = 450, R[8] = 500, R[9] = 550, R[10] = 600, R[11] = 700, R[12] = 800, R[13] = 1000, R[14] = 1200, R[15] = 1500, R[16] = 2000, R[17] = 2500, R[18] = 3000, R[19] = 4000;
		NR = 20;
	}
	else// if(fabs(Design_V - 30)<0.1)
	{
		R[0] = 150, R[1] = 180, R[2] = 200, R[3] = 250, R[4] = 300, R[5] = 350, R[6] = 400, R[7] = 450, R[8] = 500, R[9] = 550, R[10] = 600, R[11] = 700, R[12] = 800, R[13] = 1000, R[14] = 1200, R[15] = 1500, R[16] = 2000, R[17] = 2500, R[18] = 3000, R[19] = 4000;
		NR = 20;
	}
	i = 0;
	while (i < NR && R[i] < SR - 1)
		i++;

	if (i == 0)
		return R[0];
	else if (i == NR)
	{
		if (SR > R[NR - 1])
			return R[NR - 1];
		else
			i--;
	}
	return  fabs(R[i] - SR) > fabs(R[i - 1] - SR) ? R[i - 1] : R[i];

}
AcDbObjectId BAS_DRAW_FUN::CreateLayer(ACHAR *LayerName, ACHAR *LineType)
{
	AcDbLayerTable *pLayerTable;
	//	AcDbObjectId LayerTabRecordId;


	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForRead);
	AcDbObjectId LayerTabRecordId;
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) == Acad::eOk)
	{
		pLayerTable->close();
		return LayerTabRecordId;
	}
	pLayerTable->close();

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;
	pLayerTableRecord->setName(LayerName);
	AcDbLinetypeTable *pLinetypeTble;
	acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTble, AcDb::kForRead);
	AcDbObjectId ltypeObjId;
	pLinetypeTble->getAt(LineType, ltypeObjId, Adesk::kTrue);
	//	acutPrintf(L"\nId=%d",ltypeObjId);
	pLayerTableRecord->setLinetypeObjectId(ltypeObjId);
	pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	pLinetypeTble->close();
	return LayerTabRecordId;
}

AcDbObjectId BAS_DRAW_FUN::CreateLayer(ACHAR *LayerName)
{
	AcDbLayerTable *pLayerTable;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForRead);
	AcDbObjectId LayerTabRecordId;
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) == Acad::eOk)
	{
		pLayerTable->close();
		return LayerTabRecordId;
	}
	pLayerTable->close();

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;
	pLayerTableRecord->setName(LayerName);
	//	acutPrintf(L"\nId=%d",ltypeObjId);
	pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	return LayerTabRecordId;

}


void BAS_DRAW_FUN::SetCurLayer(ACHAR *LayerName)
{
	AcDbLayerTable *pLayerTable;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForRead);
	AcDbObjectId LayerTabRecordId;
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) == Acad::eOk)
	{

		acdbHostApplicationServices()->workingDatabase()->setClayer(LayerTabRecordId);

	}
	pLayerTable->close();

}

//20190805	设置图层颜色
void BAS_DRAW_FUN::SetLayerColor(ACHAR* LayerName, int ColorIndex)
{
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	AcDbLayerTable* pLayerTable;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	AcDbLayerTableRecord* pLayerTableRecord;
	AcCmColor Color;
	Color.setColorIndex(ColorIndex);
	if (pLayerTable->getAt(LayerName, pLayerTableRecord, AcDb::kForWrite) == Acad::eOk)
	{
		pLayerTableRecord->setColor(Color);
		pLayerTableRecord->close();
	}
	pLayerTable->close();
	acDocManager->unlockDocument(acDocManager->curDocument());
}



void BAS_DRAW_FUN::caljd(double x1, double y1, double x2, double y2, double x_1, double y_1, double x_2, double y_2, double *xjd, double *yjd)
{
	double a1, b1, c1, a2, b2, c2, abc;
	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = y1 * (x2 - x1) - x1 * (y2 - y1);
	a2 = y_2 - y_1;
	b2 = x_1 - x_2;
	c2 = y_1 * (x_2 - x_1) - x_1 * (y_2 - y_1);
	abc = a1 * b2 - a2 * b1;
	*yjd = (c1*a2 - a1 * c2) / abc;
	*xjd = -(c1*b2 - b1 * c2) / abc;
}
void BAS_DRAW_FUN::calzxjd(double x1, double y1, double x2, double y2, double a2, double b2, double c2, double *xjd, double *yjd)
{
	double a1, b1, c1, abc;
	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = y1 * (x2 - x1) - x1 * (y2 - y1);
	abc = a1 * b2 - a2 * b1;
	*yjd = (c1*a2 - a1 * c2) / abc;
	*xjd = -(c1*b2 - b1 * c2) / abc;
}

AcDbObjectId BAS_DRAW_FUN::CreateHatch(AcGePoint3dArray Pts, ACHAR* patName, bool bAssociative,int ColorIndex,bool iscir)
{
	AcDbObjectId Id;
	if (iscir)
	{
		if (ColorIndex==1)
		{
			Id = makecircle(Pts.at(0), 150, ColorIndex, L"干线点");
		}
		else if (ColorIndex == 2)
		{
			Id = makecircle(Pts.at(0), 120, ColorIndex, L"配合站点");
		}
		else if (ColorIndex == 3)
		{
			Id = makecircle(Pts.at(0), 100, ColorIndex, L"项目部");
		}
		
	}
	else
	{
		Id = make2dline(Pts, ColorIndex, 0.1, 0.1, L"地形粗糙度");
	}
	
	AcDbObjectIdArray objIds;
	objIds.append(Id);
	Acad::ErrorStatus es;
	AcDbHatch *pHatch = new AcDbHatch();
	// 设置填充平面
	AcGeVector3d normal(0, 0, 1);
	pHatch->setNormal(normal);
	pHatch->setElevation(0);
	// 设置关联性
	pHatch->setAssociative(bAssociative);
	// 设置填充图案
	pHatch->setPattern(AcDbHatch::kPreDefined, patName);
	// 添加填充边界
	es = pHatch->appendLoop(AcDbHatch::kExternal, objIds);
	// 显示填充对象
	es = pHatch->evaluateHatch();
	// 显示填充对象
	es = pHatch->setColorIndex(ColorIndex);
	// 添加到模型空间
	AcDbObjectId hatchId;
	hatchId = AddEntityToDbs(pHatch);
	// 如果是关联性的填充，将填充对象与边界绑定，以便使其能获得边界对象修改的通知
	if (bAssociative)
	{
		AcDbEntity *pEnt;
		for (int i = 0; i < objIds.length(); i++)
		{
			es = acdbOpenAcDbEntity(pEnt, objIds[i], AcDb::kForWrite);
			if (es == Acad::eOk)
			{
				// 添加一个永久反应器
				pEnt->addPersistentReactor(hatchId);
				pEnt->close();
			}
		}
	}
	return hatchId;
}

AcDbObjectId  BAS_DRAW_FUN::makecircle(AcGePoint3d& startPt, double R, int icolor, ACHAR *LayerName)
{
	AcGeVector3d  normal(0.0, 0.0, 1.0);
	AcDbCircle *pC = new AcDbCircle(startPt, normal, R);
	pC->setColorIndex(icolor);
	if (LayerName == NULL)LayerName = L"0";
	pC->setLayer(LayerName);
	AcDbObjectId  LId;
	LId = AddEntityToDbs(pC);
	return LId;
}

//plh05.13
double  BAS_DRAW_FUN::PROJ_ML(double array2[4 * MAXJDNUM][10], int trow2,
	double xout, double yout, double xon, double yon)
{
	double x1, y1, x2, y2, x3, y3, x4, y4, s1, s2, s, ZJ, Alf1, Alf2, Alf, Xo, Yo, q, ZJ1, ZJ2, R;
	x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = s1 = s2 = s = ZJ = Alf1 = Alf2 = Alf = Xo = Yo = q = ZJ1 = ZJ2 = R = 0;
	int i, kk[100], ii, xynum = 0;
	double LorR, X1[100], Y1[100], X2[100], Y2[100];

	double lc = array2[0][4];
	//	PB0.x=array2[0][1];
	//	PB0.y=array2[0][2];
	//	PB0.a=rad(array2[0][3]);
	//	PB0.r=1e40;
	//起点向前延长1000m
	x2 = array2[0][1] + 1000 * cos(rad(array2[0][3]) + pi);
	y2 = array2[0][2] + 1000 * sin(rad(array2[0][3]) + pi);
	for (i = 1; i <= trow2; i++)
	{
		x1 = x2;
		y1 = y2;
		x2 = array2[i][8];
		y2 = array2[i][9];

		q = array2[i - 1][0];
		// 	ads_printf(L"##x1=%lf,y1=%lf ,x2=%lf,y2=%lf,q=%lf\n",x1,y1,x2,y2,q);
		LorR = 1.0;
		if (q > 1.1 && q < 7.0)
			//		ads_printf(L"%lf\n",array2[i][4]);
			LorR = array2[i - 1][4];
		if (i > 1)
			Alf1 = array2[i - 1][5];
		else
			Alf1 = rad(array2[i - 1][3]);

		Alf2 = array2[i][5];
		if (i == trow2)//终点向后延长1000m
		{
			//	ads_printf(L"Alf1=%lf Alf2=%lf\n",Alf1,Alf2);
			x2 = x2 + 1000 * cos(Alf1);
			y2 = y2 + 1000 * sin(Alf1);
		}
		x3 = x1 + 1000.0*cos(0.5*LorR*pi + Alf1);
		y3 = y1 + 1000.0*sin(0.5*LorR*pi + Alf1);

		x4 = x2 + 1000.0*cos(0.5*LorR*pi + Alf2);
		y4 = y2 + 1000.0*sin(0.5*LorR*pi + Alf2);

		s1 = GetSide(x3, y3, x1, y1, xon, yon);
		s2 = GetSide(x4, y4, x2, y2, xon, yon);

		if (fabs(q - 2.0) < 0.1)//圆
		{
			//计算圆心
			double Xo, Yo, R, sfwj, dis, sx, sy;
			R = array2[i - 1][1];
			sfwj = array2[i - 1][5];
			sx = array2[i - 1][8];
			sy = array2[i - 1][9];
			Xo = sx + R * cos(sfwj + LorR * 0.5*pi);
			Yo = sy + R * sin(sfwj + LorR * 0.5*pi);
			dis = sqrt((xon - Xo)*(xon - Xo) + (yon - Yo)*(yon - Yo));
			if (fabs(dis - R) < 1000.0)//
			{
				s1 = 1.0, s2 = -1.0;
			}

		}
		//		 if(fabs(Pt1[0]-)<0.1)

		if (s1*s2 < 0.0001 || fabs(s1) < 0.00001 || fabs(s2) < 0.00001)
		{
			//		   ads_alert(L"指定点无法投影到指定线段上, 请重新指定线段!");

			kk[xynum] = i - 1;
			X1[xynum] = x1;
			Y1[xynum] = y1;
			X2[xynum] = x2;
			Y2[xynum] = y2;
			if (i == 1)
			{
				X1[xynum] = array2[0][1];
				Y1[xynum] = array2[0][2];
			}
			if (i == trow2)
			{
				X2[xynum] = array2[i][8];
				Y2[xynum] = array2[i][9];
			}
			//		    goto L10;
			xynum++;//搜索出所有可能线元
			//		   return  -1.0;
		}
	}


	if (xynum == 0)
	{
		ads_printf(L"指定点%lf %lf不在线路上(1)!\n", xon, yon);
		return -1.0;
	}

	double cml[100];
	int ncml;
	ncml = 0;
	for (i = 0; i < xynum; i++)
	{
		x1 = X1[i];
		y1 = Y1[i];
		x2 = X2[i];
		y2 = Y2[i];

		//搜索范围限制在kk-1-------kk段现元上
		if (kk[i] > 0)
		{
			q = array2[kk[i]][0];
			LorR = array2[kk[i]][4];//?左右转
		}
		else
		{
			q = array2[0][0];
			LorR = 0.0;//?左右转
		}
		//ads_printf(L"kk=%d,lc=%lf q=%lf %lf \n",kk,array2[kk][6],q,array2[kk-1][6]);
		//	LorR = 1;
		//	if(fabs(q) > 0.1)

		if (fabs(LorR) < 0.001)
			LorR = 1.0;

		if (kk[i] > 0)
			Alf1 = array2[kk[i]][5];
		else
			Alf1 = array2[kk[i]][3];

		if (kk[i] < trow2)
			Alf2 = array2[kk[i] + 1][5];
		else
			Alf2 = array2[trow2][5];
		/*
		x3 = x1 + 1000.0*cos(0.5*LorR*pi+Alf1);
		y3 = y1 + 1000.0*sin(0.5*LorR*pi+Alf1);

		x4 = x2 + 1000.0*cos(0.5*LorR*pi+Alf2);
		y4 = y2 + 1000.0*sin(0.5*LorR*pi+Alf2);

		s1 = GetSide(x3,y3,x1,y1,xout,yout);
		s2 = GetSide(x4,y4,x2,y2,xout,yout);

		if(s1*s2 > 0.0)
		{
		ads_printf(L"指定点%0.2lf %0.2lf无法投影到指定线段上, 请重新指定线段!\n",xout,yout);
		return -1.0;
		}*/
		//		if(fabs(yon-484981.40)<0.1)
		//		acutPrintf(L"**********q=%lf\n",q);
		if (kk[i] > 0)
			lc = array2[kk[i]][6];//当前线元起始里程
		else
			lc = array2[kk[i]][4];//当前线元起始里程

		/*
		if(fabs(xout-128189.148)<0.01)
		ads_printf(L"\nq=%lf %d\n",q,ncml);*/


		if (q < 1.1)//直线
		{
			s1 = sqrt((xout - x1)*(xout - x1) + (yout - y1)*(yout - y1));
			xyddaa(x1, y1, xout, yout, &Alf2);
			ZJ = ZXJ(Alf2, Alf1);
			//ads_printf(L"%d ZJ=%lf Alf2=%lf Ald1=%lf s1=%lf\n",kk[i],ZJ,Alf2,Alf1,s1);
			cml[ncml] = lc + s1 * cos(ZJ);
			ncml++;
		}
		else if (q < 2.1)//圆
		{

			double x, y, zx, fwj, pt[3], arclen;
			R = array2[kk[i]][1];
			Xo = array2[kk[i]][8] + R * cos(array2[kk[i]][5] + LorR * 0.5*pi);
			Yo = array2[kk[i]][9] + R * sin(array2[kk[i]][5] + LorR * 0.5*pi);

			AcGePoint2d Spt, Ept;
			double xmin, ymin, xmax, ymax;
			Spt.x = xout, Spt.y = yout;
			Ept.x = Xo;
			Ept.y = Yo;
			AcGeLine2d line(Spt, Ept);
			xmin = Spt.x < Ept.x ? Spt.x : Ept.x;
			ymin = Spt.y < Ept.y ? Spt.y : Ept.y;
			xmax = Spt.x > Ept.x ? Spt.x : Ept.x;
			ymax = Spt.y > Ept.y ? Spt.y : Ept.y;

			x = array2[kk[i]][8], y = array2[kk[i]][9];
			fwj = array2[kk[i]][5];
			zx = array2[kk[i]][4];
			arclen = array2[kk[i]][2];
			AcGePoint2d Arc_Spt, Arc_Ept, Arc_Mpt, jd1, jd2;
			Arc_Spt.set(x, y);
			GetXYonSomeXY(array2[kk[i]], arclen, pt);
			Arc_Ept.set(pt[0], pt[1]);
			GetXYonSomeXY(array2[kk[i]], 0.5*arclen, pt);
			Arc_Mpt.set(pt[0], pt[1]);

			int JDNUM = 0;
			AcGeCircArc2d ARC(Arc_Spt, Arc_Mpt, Arc_Ept);
			ARC.intersectWith(line, JDNUM, jd1, jd2);
			int njd;
			njd = 0;
			if (JDNUM > 0)
			{

				cml[ncml] = Circle_XY_to_ML(jd1.x, jd1.y, array2, kk[i]);
				ncml++;
			}
			if (JDNUM > 1)
			{
				cml[ncml] = Circle_XY_to_ML(jd2.x, jd2.y, array2, kk[i]);
				ncml++;
			}

		}
		else//缓和曲线
		{
			xyddaa(xout, yout, x1, y1, &Alf1);
			xyddaa(xout, yout, x2, y2, &Alf2);

			if (kk[i] > 0)
				ZJ1 = ZXJ(Alf1, array2[kk[i]][5]);
			else
				ZJ1 = ZXJ(Alf1, rad(array2[kk[i]][3]));

			ZJ2 = ZXJ(Alf2, Alf1);
			if (fabs(ZJ1) > 0.5*pi)
				ii = 1;
			else
				ii = -1;

			s1 = lc;
			if (kk[i] < trow2)
				s2 = array2[kk[i] + 1][6];
			else
				s2 = array2[trow2][6];

			while (fabs(s2 - s1) > 0.00001)
			{
				s = 0.5*(s1 + s2);
				struct xlpoint PZ;
				PZ.lc = s;

				xlpoint_pz(array2, trow2, &PZ);

				//	CAL_XY(s, &x3,&y3,&z3);
				xyddaa(xout, yout, PZ.x, PZ.y, &Alf);
				//	Alf = FAngle(x3-xout , y3-yout);
				ZJ = ZXJ(Alf, PZ.a);

				if ((ii == 1 && fabs(ZJ) > 0.5*pi) || (ii == -1 && fabs(ZJ) < 0.5*pi))
					s1 = s;
				else
					s2 = s;
			}

			cml[ncml] = s;
			ncml++;
		}
	}
	//取出距离xon,yon的点
	double MinDis = 100000000000000.0, dist;
	for (i = 0; i < ncml; i++)
	{
		struct xlpoint PZ;
		PZ.lc = cml[i];
		xlpoint_pz(array2, trow2, &PZ);
		dist = sqrt((PZ.x - xon)*(PZ.x - xon) + (PZ.y - yon)*(PZ.y - yon));
		if (MinDis > dist)
		{
			s = cml[i];
			if (s > array2[trow2][6])
				s = array2[trow2][6];
			if (s < array2[0][4])
				s = array2[0][4];
			MinDis = dist;
		}
	}
	if (s < array2[0][4])
		s = array2[0][4];
	if (s > array2[trow2][6])
		s = array2[trow2][6];
	//	ads_printf(L"########## cml=%lf\n",array2[trow2][6]);

	return s;
}

//
//double  BAS_DRAW_FUN::PROJ_ML(double array2[800][10],int trow2,double xout, double yout, double xon, double yon)
//{
//   double x1,y1,x2,y2,x3,y3,x4,y4,s1,s2,s,ZJ,Alf1,Alf2,Alf,Xo,Yo,q,x,y,ZJ1,ZJ2,R;
//   int i,kk[100] ,ii,xynum;
//   double LorR,X1[100],Y1[100],X2[100],Y2[100];      
//   
//   double lc=array2[0][4];
////	PB0.x=array2[0][1];
////	PB0.y=array2[0][2];
////	PB0.a=rad(array2[0][3]);
////	PB0.r=1e40;
////起点向前延长1000m
//    xynum = 0;
//	double fwj = array2[1][5]+pi;
//	x2 = array2[1][8] + 1000.0*cos(fwj);
//	y2 = array2[1][9] + 1000.0*sin(fwj);
//	
//    for(i=1; i<=trow2;i++)
//	{
//		x1 = x2;
//		y1 = y2;
//		x2 = array2[i][8];
//		y2 = array2[i][9];
//		//acutPrintf(L"i:%d,%lf,%lf,%lf,%lf\n",i,y1,x1,y2,x2);
//    	q = array2[i][0];
//	    LorR = 1.0;
//	    if(q > 1.1)
//	        LorR = array2[i][4];
//	//	if(i>1)
//        Alf1 = array2[i-1][5] ;
//	//	else
//	//		Alf1 = rad(array2[i-1][3]);
//
//	    Alf2 =  array2[i][5] ;
//		if(i==trow2)//终点向后延长1000m
//		{
//		//	ads_printf(L"Alf1=%lf Alf2=%lf\n",Alf1,Alf2);
//			x2 = x2 + 1000*cos(Alf1);
//			y2 = y2 + 1000*sin(Alf1);
//		}
//	    x3 = x1 + 1000.0*cos(0.5*LorR*pi+Alf1);
//	    y3 = y1 + 1000.0*sin(0.5*LorR*pi+Alf1);
//
//	    x4 = x2 + 1000.0*cos(0.5*LorR*pi+Alf2);
//	    y4 = y2 + 1000.0*sin(0.5*LorR*pi+Alf2);
//
//	    s1 = GetSide(x3,y3,x1,y1,xon,yon);
//	    s2 = GetSide(x4,y4,x2,y2,xon,yon);
//		
//		//acutPrintf(L"i=%d,lc=%lf,s1=%lf,s2=%lf,xon=%lf,yon=%lf,xynum=%d\n",i,array2[i][6],s1,s2,xon,yon,xynum);
//	    if(s1*s2 < 0.1 || fabs(s1)<0.001 || fabs(s2)<0.001)
//		{
////		   ads_alert(L"指定点无法投影到指定线段上, 请重新指定线段!");
//			kk[xynum] = i;
//			X1[xynum]=x1;
//			Y1[xynum]=y1;
//			X2[xynum]=x2;
//			Y2[xynum]=y2;
//		//	acutPrintf(L"**********i=%d %lf %lf %lf %lf\n",i,x1,y1,x2,y2);
////		    goto L10;
//			xynum++;//搜索出所有可能线元
////		   return  -1.0;
//		}
//	}
//
//	if(xynum==0)
//	{
//		ads_printf(L"指定点 %0.2lf %0.2lf不在线路上(1)!\n",xon,yon);
//		return -1.0;
//	}
//
//	double cml[100];
//	for(i=0; i<xynum; i++)
//	{
//		x1=X1[i];
//		y1=Y1[i];
//		x2=X2[i];
//		y2=Y2[i];
//		
//		//搜索范围限制在kk-1-------kk段现元上
//		if(kk[i]>0)
//		{
//			q =array2[kk[i]-1][0];
//			LorR =  array2[kk[i]-1][4];//?左右转
//		}
//		else
//		{
//			q = array2[0][0];
//			LorR = 0.0;//?左右转
//		}
//		
//		//	LorR = 1;
//		//	if(fabs(q) > 0.1)
//		
//		if(fabs(LorR)<0.001)
//			LorR = 1.0;
//		
//		if(kk[i]>1)
//			Alf1 = array2[kk[i]-1][5];
//		else
//			Alf1 = rad(array2[kk[i]-1][3]);
//				
//		Alf2 = array2[kk[i]][5];
//		/*
//		x3 = x1 + 1000.0*cos(0.5*LorR*pi+Alf1);
//		y3 = y1 + 1000.0*sin(0.5*LorR*pi+Alf1);
//		
//		  x4 = x2 + 1000.0*cos(0.5*LorR*pi+Alf2);
//		  y4 = y2 + 1000.0*sin(0.5*LorR*pi+Alf2);
//		  
//			s1 = GetSide(x3,y3,x1,y1,xout,yout);
//			s2 = GetSide(x4,y4,x2,y2,xout,yout);
//			
//			  if(s1*s2 > 0.0)
//			  {
//			  ads_printf(L"指定点%0.2lf %0.2lf无法投影到指定线段上, 请重新指定线段!\n",xout,yout);
//			  return -1.0;
//	}*/
//		
//		
//		if(kk[i]>1)
//			lc = array2[kk[i]-1][6];//当前线元起始里程
////ads_printf(L"kk[i]=%d,lc=%lf q=%lf\n",kk[i],lc,q);
//		if(q < 1.1)//直线
//		{
//			s1 = sqrt((xout-x1)*(xout-x1) + (yout-y1)*(yout-y1));
//			xyddaa(x1,y1,xout,yout,&Alf2);
//			ZJ = ZXJ(Alf2 , Alf1);
//			// 	ads_printf(L"ZJ=%lf Alf2=%lf Ald1=%lf\n",ZJ,Alf2,Alf1);
//			cml[i] = lc + s1*cos(ZJ);
//		}
//		else if(q<2.1)//圆
//		{
//		//	R = array2[kk[i]-1][7];
//			R = array2[kk[i]-1][1];
//			
//			Xo = array2[kk[i]-1][8] + R*cos(array2[kk[i]-1][5]+LorR*0.5*pi) ;
//			Yo = array2[kk[i]-1][9] + R*sin(array2[kk[i]-1][5]+LorR*0.5*pi);
//			
//			
//			s = 0.5*(array2[kk[i]][6] - lc);
//			x = x1 + s*cos(Alf1);
//			y = y1 + s*sin(Alf1);
//			xyddaa( Xo,Yo,xout,yout,&Alf);
//			
//			s1 = GetSide(x1,y1,Xo,Yo,x,y);
//			s2 = GetSide(x1,y1,Xo,Yo,xout,yout);
//			if(s1*s2 < 0.00001 || fabs(s1)<0.01 || fabs(s2)<0.01)  
//				Alf = Alf + pi;
//			x = Xo + R*cos(Alf);
//			y = Yo + R*sin(Alf);
//			
//			//	   s = XY_to_ML(x , y,array2,trow2);
//			//	   ads_printf(L"1##s=%lf\n",s);
////	ads_printf(L"####i=%d kk=%d R=%lf Xo=%lf Yo=%lf x1=%lf y1=%lf\n",i,kk[i],R,Xo,Yo,x1,y1);
//			cml[i] = Circle_XY_to_ML(x,y,array2,kk[i]-1);
//			
//		}
//		else//缓和曲线
//		{
//			xyddaa( xout,yout,x1,y1,&Alf1);		
//			xyddaa( xout,yout,x2,y2,&Alf2);
//			
//			if(kk[i]>1)
//				ZJ1 = ZXJ(Alf1 , array2[(kk[i])-1][5]);
//			else
//				ZJ1 = ZXJ(Alf1 , rad(array2[(kk[i])-1][3]));
//			
//			ZJ2 = ZXJ(Alf2 , array2[(kk[i])][5]);
//			if(fabs(ZJ1) > 0.5*pi)
//				ii = 1;
//			else
//				ii = -1;
//			
//			s1 = lc;
//			s2 = array2[(kk[i])][6];
//L20:
//			s = 0.5*(s1+s2);
//			cml[i] = s;
//			struct xlpoint PZ;
//			PZ.lc = s;
//			
//			xlpoint_pz(array2,trow2,&PZ); 
//			
//			//	CAL_XY(s, &x3,&y3,&z3);
//			xyddaa( xout,yout,PZ.x,PZ.y,&Alf);		
//			//	Alf = FAngle(x3-xout , y3-yout);
//			ZJ = ZXJ(Alf , PZ.a);
//			
//			if((ii==1 && fabs(ZJ)>0.5*pi )|| (ii==-1 && fabs(ZJ)<0.5*pi))
//				s1 = s;
//			else
//				s2 = s;
//			if(fabs(s2-s1) > 0.00001)
//				goto L20;
//			
//		}
//	}
////取出距离xon,yon最近的点
//    double MinDis=100000000000000.0,dist;
//	//acutPrintf(L"xynum=%d\n",xynum);
//	for(i=0; i<xynum; i++)
//	{
//       	struct xlpoint PZ;
//		PZ.lc = cml[i];
//		xlpoint_pz(array2,trow2,&PZ); 
//		dist=sqrt((PZ.x-xon)*(PZ.x-xon)+(PZ.y-yon)*(PZ.y-yon));
//		//acutPrintf(L"i=%d,dist=%lf\n",i,dist);
//		
//		if(MinDis>dist)
//		{
//			s = cml[i];
//			MinDis = dist;
//		}
//	}
//    return s;
//}
//---------------------------------------------------------------------
double  BAS_DRAW_FUN::GetSide(double Xo, double Yo, double x1, double y1, double x, double y)
//左侧-，右+
{
	double s;

	s = (x - Xo)*(y1 - Yo) - (x1 - Xo)*(y - Yo);
	return s;
}

//---------------------------------------------------------------------
double   BAS_DRAW_FUN::ZXJ(double s2, double s1)
{
	double s;
	s = s2 - s1;
	if (fabs(s) > pi)
	{
		if (s < 0.0) //偏角小于-PI
		{
			s = 2.0*pi + s;
		}
		else      //偏角大于PI
		{
			s = s - 2.0*pi;
		}
	}
	return(s);
}
double  BAS_DRAW_FUN::XY_to_ML(double x, double y, double array2[400][10], int trow2)
{
	double x1, y1, x2, y2, s1, s2, s, ss1, ss2;
	int i, kk;
	//   FILE *e;

	//  fpw = _wfopen(L"c:\\ph\\data" , L"w");

	double lc = array2[0][4];
	x2 = array2[0][1], y2 = array2[0][2];

	for (i = 1; i <= trow2; i++)
	{
		x1 = x2;
		y1 = y2;
		x2 = array2[i][8];
		y2 = array2[i][9];
		s1 = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);

		s = (x - x1)*(x - x1) + (y - y1)*(y - y1);
		if ((x - x1)*(x2 - x) >= 0.0 && (y - y1)*(y2 - y) >= 0.0 ||
			s1 >= s)
		{
			kk = i;
			goto L10;
		}
	}
	ads_printf(L"指定点不在线路上(2)!\n");
	return -1.0;
L10:
	if (kk > 1)
		lc = array2[kk - 1][6];

	s1 = lc;
	s2 = array2[kk][6];

	/*L20:
	s = 0.5*(s1+s2);
	CAL_XY(s, &x3,&y3,&z3);
	ads_printf(L"x3=%12.3lf y3=%12.3lf\n" ,x3,y3);
	fwprintf(fpw,L"x3=%12.3lf y3=%12.3lf\n" ,x3,y3);

	if((x-x1)*(x3-x)>=0.0 && (y-y1)*(y3-y)>=0.0)
	{
	x2 = x3;
	y2 = y3;
	s2 = s;
	}
	else if((x-x3)*(x2-x) >= 0.0 && (y-y3)*(y2-y)>=0.0)
	{
	x1 = x3;
	y1 = y3;
	s1 = s;
	}
	else
	{
	ads_alert(L"计算点不在线路上!");
	return -1;
	}

	if(fabs(s1-s2) > 0.001)  goto L20;
	fclose(fpw);*/
	ss1 = sqrt((x - x1)*(x - x1) + (y - y1)*(y - y1));
	ss2 = sqrt((x - x2)*(x - x2) + (y - y2)*(y - y2));
	s = (s1*ss2 + s2 * ss1) / (ss1 + ss2);

	return s;
}

double  BAS_DRAW_FUN::Circle_XY_to_ML(double x, double y, double array2[400][10], int kk)
//x,y为给定圆上一点，kk为圆弧线元(不考虑回头)
{
	double x1, y1, x2, y2, s1, s2, s, ss1;

	s1 = array2[kk][6];
	s2 = array2[kk + 1][6];
	double R = array2[kk][1];
	x1 = array2[kk][8], y1 = array2[kk][9];
	x2 = array2[kk + 1][8], y2 = array2[kk + 1][9];
	ss1 = sqrt((x - x1)*(x - x1) + (y - y1)*(y - y1));
	s = s1 + R * 2 * asin(0.5*ss1 / R);

	double sfwj = array2[kk][5];
	double szx = array2[kk][4];

	double Xo, Yo, fwj1, fwj2, zxj;
	Xo = x1 + R * cos(sfwj + szx * 0.5*pi);
	Yo = y1 + R * sin(sfwj + szx * 0.5*pi);

	xyddaa(Xo, Yo, x1, y1, &fwj1);
	xyddaa(Xo, Yo, x, y, &fwj2);
	zxj = ZXJ(fwj2, fwj1);
	//ads_printf(L"s=%lf zxj=%lf zx=%lf %lf %lf %lf %lf\n",s,zxj,szx,Xo,Yo,x,y);
	if (zxj*szx < 0.0)
	{
		s = s1 + R * 2 * pi - R * 2 * asin(0.5*ss1 / R);
	}
	// ss2 = sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
	// s = (s1*ss2+s2*ss1)/(ss1+ss2);
	//	  ads_printf(L"***R=%lf %lf %lf\n",R,s1,s);
	return s;
}

//szf
double BAS_DRAW_FUN::XLC1(double TYLC, int NDL, DLBZ DLBTMP[30])
{
	double LCB = 0;  // LCB 断链的统一里程
	int iDL = 0;
	double dml = -100, tylc1;

	for (iDL = NDL - 1; iDL >= 0; iDL--)
	{
		if (TYLC >= DLBTMP[iDL].TYLC)
		{
			//			GH=DLBTMP[iDL].EGH;
			dml = TYLC - DLBTMP[iDL].TYLC + DLBTMP[iDL].ELC;
			tylc1 = TYLC1(dml, NDL, DLBTMP);
			if (fabs(tylc1 - TYLC) < 0.001)
				return dml;
			else
				return -dml;
		}
	}
}

double BAS_DRAW_FUN::XLC1(double TYLC, ACHAR *GH, int NDL, DLBZ DLBTMP[30])
{
	double LCB = 0;
	double XCLC = 0.0;// LCB 断链的统一里程
	int iDL = 0;
	CString strs1, strs2;
	for (iDL = 0; iDL < NDL; iDL++)
	{
		//该断链点统一里程
		if (iDL > 0)
			LCB = LCB + DLBTMP[iDL].BLC - (DLBTMP[iDL - 1].BLC + DLBTMP[iDL - 1].DL);
		else
			LCB = DLBTMP[iDL].BLC + DLBTMP[iDL].DL;

		if (iDL == 0)
		{
			if (TYLC < LCB)
			{
				strs1 = DLBTMP[0].BGH;
				strs1.MakeUpper();
				_tcscpy(GH, strs1);
				XCLC = DLBTMP[0].BLC - LCB + TYLC;
				break;
			}
			else if (fabs(TYLC - LCB) < 0.00005)//==
			{
				if (fabs(DLBTMP[0].BLC - floor(DLBTMP[0].BLC)) < 0.00005)
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
				_tcscpy(GH, strs1);
				break;
			}
		}
		else if (iDL < NDL)
		{
			if (TYLC < LCB)
			{
				XCLC = DLBTMP[iDL].BLC - LCB + TYLC;
				strs1 = DLBTMP[iDL - 1].EGH;
				strs2 = DLBTMP[iDL].BGH;
				strs1.TrimLeft();
				strs1.TrimRight();
				strs1.MakeUpper();
				strs2.TrimLeft();
				strs2.TrimRight();
				strs2.MakeUpper();
				if (strs1 == strs2)
					_tcscpy(GH, strs1);
				else
				{
					if (DLBTMP[iDL - 1].DL < 0.00001 && XCLC < DLBTMP[iDL - 1].BLC)//in 前长链
						_tcscpy(GH, strs1);
					else if (DLBTMP[iDL].DL < 0.00001 && XCLC < DLBTMP[iDL].BLC)//in 后长链
						_tcscpy(GH, strs2);
					else//前后都不是长链
					{
						if (2.0*(LCB - TYLC) > DLBTMP[iDL].BLC - DLBTMP[iDL - 1].BLC - DLBTMP[iDL - 1].DL)
							_tcscpy(GH, strs1);
						else
							_tcscpy(GH, strs2);
					}
				}
				break;
			}
			else if (fabs(TYLC - LCB) < 0.00005)//==
			{
				if (fabs(DLBTMP[iDL].BLC - floor(DLBTMP[iDL].BLC)) < 0.00005)
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
				_tcscpy(GH, strs1);
				break;
			}
		}
	}
	if (iDL == NDL)
	{
		XCLC = DLBTMP[iDL - 1].BLC + DLBTMP[iDL - 1].DL + TYLC - LCB;
		_tcscpy(GH, DLBTMP[iDL - 1].EGH);
	}
	return XCLC;
}

//szf  由断链数据表 求某现场里程的统一里程
//  由断链数据表 求某现场里程的统一里程
double BAS_DRAW_FUN::TYLC1(ACHAR ckml[], int NDL, DLBZ DLBTMP[30])
{
	//  由断链数据表 求某现场里程的统一里程
	double TLC = 0.0;
	double TLCtmp = 0.0;
	int iDL = 0;
	//起点里程
	double XLC;
	ACHAR GH[10];
	split_ckml(ckml, &XLC, GH);
	TLC = XLC;

	CString strstr1, strstr2;
	short int numb = 0;//统计计算的连续里程的个数2005-08-26
	strstr1 = GH;
	strstr1.TrimLeft();
	strstr1.TrimRight();
	strstr1.MakeLower();
	double continueLC = DLBTMP[0].BLC + DLBTMP[0].DL;//断链的统一里程
	for (iDL = 1; iDL < NDL; iDL++)
	{
		if ((DLBTMP[iDL - 1].BLC + DLBTMP[iDL - 1].DL - 0.00005 < XLC) && (XLC < DLBTMP[iDL].BLC + 0.00005))
		{
			strstr2 = DLBTMP[iDL - 1].EGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLC = continueLC + XLC - DLBTMP[iDL - 1].BLC - DLBTMP[iDL - 1].DL;
				numb++;
			}
			else
			{
				strstr2 = DLBTMP[iDL].BGH;
				strstr2.TrimLeft();
				strstr2.TrimRight();
				strstr2.MakeLower();
				if (strstr1 == strstr2)
				{
					TLC = continueLC + XLC - DLBTMP[iDL - 1].BLC - DLBTMP[iDL - 1].DL;
					numb++;
					strstr2.Format(L"第%d个断链的断后里程冠号%s与第%d个断链的断前里程冠号%s不一致,\n\n他将导致%s的计算不可靠",
						iDL - 1, DLBTMP[iDL - 1].EGH, iDL, DLBTMP[iDL].BGH, ckml);
					AfxMessageBox(strstr2);
				}
			}
		}
		continueLC = continueLC + DLBTMP[iDL].BLC - DLBTMP[iDL - 1].DL - DLBTMP[iDL - 1].BLC;
	}
	if (numb == 0)
	{
		if (XLC < DLBTMP[0].BLC + 0.00005)//在起点之外
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
		else if (XLC > DLBTMP[iDL - 1].BLC - 0.005 && XLC < DLBTMP[iDL - 1].BLC + 0.005)
		{
			TLC = continueLC;
			return TLC;
		}
		else if (XLC > DLBTMP[iDL - 1].BLC + DLBTMP[iDL - 1].DL - 0.00005)
		{
			strstr2 = DLBTMP[iDL - 1].EGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLC = continueLC + XLC - DLBTMP[iDL - 1].BLC - DLBTMP[iDL - 1].DL;
				numb++;
			}
		}
	}
	strstr1 = ckml;
	if (numb > 1)
		//		AfxMessageBox(L"系统发现" + strstr1 + "在线路上有多个位置，可能是断链表数据有问题");
		ads_printf(L"系统发现 %s 在线路上有多个位置，可能是断链表数据有问题\n", strstr1);
	else if (numb == 0)
		//		AfxMessageBox(L"系统发现" + strstr1 + "在线路上没有其位置，可能是短链或断链表数据有问题");
		ads_printf(L"系统发现 %s 在线路上没有其位置，可能是短链或断链表数据有问题\n", strstr1);
	return TLC;
}

//szf  由断链数据表 求不带冠号某现场里程的统一里程 
double BAS_DRAW_FUN::TYLC1(double dml, int DLNum, DLBZ DLArray[30])
{
	double TYLCB = 0, LCB, LCE, TLC, XLC;  // LCB 断链的统一里程
	int iDL = 0, bz;
	//起点里程
	CString GH;

	TLC = fabs(dml);
	XLC = TLC;
	bz = -99;
	for (iDL = 0; iDL < DLNum; iDL++)   // 
	{
		//该断链点统一里程
		TYLCB = DLArray[iDL].TYLC;

		//该断链后链里程
		LCB = DLArray[iDL].ELC;
		//下一断链的前链里程
		if (iDL < DLNum - 1)LCE = DLArray[iDL + 1].BLC;
		if ((iDL < DLNum - 1 && XLC >= LCB && XLC <= LCE) || (iDL == DLNum - 1 && XLC >= LCB))  // 落在该段落内
		{
			if (bz < 0) { TLC = TYLCB + XLC - LCB;  bz = 1; }  // 第一次求到
			else
			{
				bz = 2;
				//				ads_printf(L"\n 输入的里程有两个里程点：1:=%12.2f 2:=%12.2f  ",
				//					TLC,TYLCB+XLC-LCB);  
				if (dml < 0.0)TLC = TYLCB + XLC - LCB;
			}
		}
	}
	return TLC;
}
/*----------------------------------------------------------*
split the string mileage SML[] into two parts, e.g.
CK 153 +550.00, after calling function split_ckml, you
can get ML=153550.0, ck=CK
*----------------------------------------------------------*/
void BAS_DRAW_FUN::split_ckml(ACHAR str[], double *ml, ACHAR  *ck)
{
	int i, j, len, str_len, lplus;
	ACHAR  cml[17] = L"";
	double tml;

	/*                CIK   153 + 342.53
	.       .
	len     lplus                               */
	ck[0] = '\0';
	len = F_num(str, 'K');
	lplus = F_num(str, '+');
	str_len = _tcslen(str);
	for (i = 0; i < len; i++)
		ck[i] = str[i];
	ck[len] = '\0';


	if (lplus == 0) {
		for (i = len, j = 0; i < str_len; i++, j++)
			cml[j] = str[i];
		cml[j] = '\0';
	}
	else {
		j = 0;
		for (i = len; i < lplus - 1; i++)
			if (str[i] != ' ')
				cml[j++] = str[i];

		for (i = lplus; i < str_len; i++)
			if (str[i] != ' ')
				cml[j++] = str[i];
		cml[j] = '\0';
	};
	tml = _wtof(cml);
	tml *= 1000.0;
	tml = floor(tml);
	*ml = tml / 1000.0;
}
/*----------------------------------------------------------*
split the string mileage SML[] into two parts, e.g.
CK 153 +550.00, after calling function split_ckml, you
can get ML=153550.0, ck=CK
*----------------------------------------------------------*/
void BAS_DRAW_FUN::split_ckml(CString str, double *ml, CString& ck)
{
	int i, j, len, str_len, lplus;
	ACHAR  cml[17];
	double tml;

	/*                CIK   153 + 342.53
	.       .
	len     lplus                               */
	ck = "";
	ACHAR tmp[256], tmp1[256];
	_tcscpy(tmp, str);
	len = F_num(tmp, 'K');
	lplus = F_num(tmp, '+');
	str_len = _tcslen(tmp);
	for (i = 0; i < len; i++)
		tmp1[i] = tmp[i];
	tmp1[len] = '\0';


	if (lplus == 0) {
		for (i = len, j = 0; i < str_len; i++, j++)
			cml[j] = tmp[i];
		cml[j] = '\0';
	}
	else {
		j = 0;
		for (i = len; i < lplus - 1; i++)
			if (str[i] != ' ')
				cml[j++] = str[i];

		for (i = lplus; i < str_len; i++)
			if (str[i] != ' ')
				cml[j++] = str[i];
		cml[j] = '\0';
	};
	tml = _wtof(cml);
	tml *= 1000.0;
	tml = floor(tml);
	*ml = tml / 1000.0;
	ck = tmp1;
}
/*----------------------------------------------------------*
* Name:  determain the location of ACHARacter C in string
variable STR[], if found, return subscript of C in STR[],
otherwise, return 0
*----------------------------------------------------------*/
int BAS_DRAW_FUN::F_num(ACHAR *str, ACHAR c)
{
	int i, len, num = 0, smark, c1;

	smark = toupper(c);
	len = _tcslen(str);
	for (i = len - 1; i >= 0; i--)
	{
		c1 = toupper(str[i]);
		if (c1 == smark)
		{
			num = i + 1; break;
		}
	}
	return(num);
}
/*----------------------------------------------------------*
put "+" to mileage (e.g. CK153500.0 --> CK153+500.0)
put the sign IN_CH,
*----------------------------------------------------------*/
void BAS_DRAW_FUN::putplus(ACHAR str[], ACHAR in_ch, ACHAR s_ch, int place)
{
	int slen, splace, i, gone = 1;

	slen = _tcslen(str);
	for (i = 0; i < slen; i++)
		if (toupper(str[i]) == toupper(in_ch)) gone = 0;

	if (gone) {
		for (i = 0; i < slen; i++)
			if (str[i] == s_ch) { splace = i + 1; break; }
		for (i = slen; i >= splace + place; i--)
			str[i] = str[i - 1];
		str[i] = in_ch;
		str[slen + 1] = '\0';
	};
}
AcDbObjectId  BAS_DRAW_FUN::make2dline(AcGePoint3dArray ptarray, int icolor, double startWid, double EndWid, ACHAR* LayerName, double ConstWide)
{
	if (fabs(startWid - 999) < 0.1)//X,Y转换，线路求交时用
	{
		for (int i = 0; i < ptarray.logicalLength(); i++)
		{
			double t = ptarray[i].x;
			ptarray[i].x = ptarray[i].y;
			ptarray[i].y = t;
		}
		startWid = 0.1;
	}
	AcDb2dPolyline *pLine = new AcDb2dPolyline(AcDb::k2dSimplePoly, ptarray, 0, Adesk::kFalse, startWid, EndWid);
	pLine->setColorIndex(icolor);
	AcDbObjectId  LId, LayerId;
	if (LayerName)
	{
		LayerId = CreateLayer(LayerName);
		pLine->setLayer(LayerId);
	}
	//pLine->setConstantWidth();
	LId = AddEntityToDbs(pLine);
	return LId;
}

void BAS_DRAW_FUN::atodmss(double alpha, int &d, ACHAR mm[4], ACHAR ss[4])
{
	int m, s;
	d = floor(fabs(alpha));
	m = floor((fabs(alpha) - d)*100.0 + 0.0001);
	s = floor(((fabs(alpha) - d)*100.0 - m)*100.0 + 0.0001);

	if (m < 10)
		_stprintf(mm, L"0%d'\0", m);
	else
		_stprintf(mm, L"%d'\0", m);
	if (s < 10)
		_stprintf(ss, L"0%d\"\0", s);
	else
		_stprintf(ss, L"%d\"\0", s);
}
void BAS_DRAW_FUN::NoteBG(AcGePoint3d Opt, double texth, ACHAR Str[], int iColor, int itextColor, double sfactor, int mode, ACHAR* layername, int Dir)
{
	if (Dir >= 0)
		DrawBGTri(Opt, sfactor, iColor, layername, Dir);
	else
	{
		AcGePoint3d spt, ept;
		spt.x = Opt.x - 0.1*sfactor;  spt.y = Opt.y + 0.1*sfactor;
		ept.x = Opt.x + 0.1*sfactor;  ept.y = Opt.y - 0.1*sfactor;
		makeline(spt, ept, iColor, 0);
		spt.x = Opt.x - 0.1*sfactor;  spt.y = Opt.y - 0.1*sfactor;
		ept.x = Opt.x + 0.1*sfactor;  ept.y = Opt.y + 0.1*sfactor;
		makeline(spt, ept, iColor, 0);
	}

	AcGePoint3d origin;
	origin.x = Opt.x;
	origin.y = Opt.y + 0.25 * sfactor;
	origin.z = Opt.z;
	maketext(origin, Str, 0, texth, itextColor, mode, L"宋体", layername);
}

void BAS_DRAW_FUN::NoteBGWithLine(AcGePoint3d Opt, double texth, ACHAR Str[], int iColor, int itextColor, double sfactor, int mode, ACHAR* layername)
{
	//画三角形
	AcGePoint2d Pt1, Pt2, Pt3;
	AcGePoint2dArray ptArray;
	Pt1.set(Opt.x, Opt.y);
	Pt2.set(Opt.x + 0.2 * sfactor, Opt.y + 0.3 * sfactor);
	Pt3.set(Opt.x - 0.2 * sfactor, Opt.y + 0.3 * sfactor);
	ptArray.removeAll();
	ptArray.append(Pt1);
	ptArray.append(Pt2);
	ptArray.append(Pt3);
	ptArray.append(Pt1);

	makepolyline(ptArray, iColor, 0.0, layername);

	//画竖线
	AcGePoint3d spt(0.0, 0.0, 0.0), ept(0.0, 0.0, 0.0);
	spt.x = Opt.x;
	spt.y = Opt.y + 0.3 * sfactor;
	ept.x = Opt.x;
	ept.y = Opt.y + 1 * sfactor;
	makeline(spt, ept, iColor, 0.0, layername);

	//画文字
	AcGePoint3d origin;
	origin.x = Opt.x;
	origin.y = ept.y + 0.1 * sfactor;
	origin.z = Opt.z;
	maketext(origin, Str, 0, texth, itextColor, mode, L"宋体", layername);
}

void BAS_DRAW_FUN::DrawBGTri(AcGePoint3d Opt, double sfactor, int iColor, ACHAR LayerName[], int Dir)
{
	AcGePoint3d pt[4];

	if (Dir)//尖头向下
	{
		pt[0] = Opt;
		pt[1].x = Opt.x - 0.1*sfactor;
		pt[1].y = Opt.y + 0.15*sfactor;
		pt[2].x = Opt.x + 0.1*sfactor;
		pt[2].y = Opt.y + 0.15*sfactor;
		pt[3] = pt[0];
	}
	else//尖头向上
	{
		pt[0] = Opt;
		pt[1].x = Opt.x - 0.1*sfactor;
		pt[1].y = Opt.y - 0.15*sfactor;
		pt[2].x = Opt.x + 0.1*sfactor;
		pt[2].y = Opt.y - 0.15*sfactor;
		pt[3] = pt[0];
	}
	AcDbSolid *triangle = new AcDbSolid(pt[0], pt[1], pt[2], pt[3]);
	AcDbObjectId LayerId;
	if (LayerName)
	{
		LayerId = CreateLayer(LayerName);
		triangle->setLayer(LayerId);
	}
	triangle->setColorIndex(iColor);
	AddEntityToDbs(triangle);
}
void BAS_DRAW_FUN::dlayer(ACHAR lname[])//删除层
{
	struct resbuf eb;
	ads_name sname;
	ACHAR sbuf[80];
	_tcscpy(sbuf, lname);

	eb.restype = 8;   /* Layer name  */
	eb.resval.rstring = sbuf;
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
		return;
	}

	if (ads_ssfree(sname) != RTNORM)
	{
		//acutPrintf(L"\nsname free error");
		return;
	}

}
double BAS_DRAW_FUN::get_l0(double Design_V, int KNDD, double SR)
{
	double l0 = 0.0;
	if (Design_V == 140)
	{
		if (KNDD == 0)
		{
			if (SR >= 10000) l0 = 30;
			else if (SR >= 8000) l0 = 40;
			else if (SR >= 6000) l0 = 50;
			else if (SR >= 5000) l0 = 60;
			else if (SR >= 4000) l0 = 60;
			else if (SR >= 3000) l0 = 70;
			else if (SR >= 2500) l0 = 80;
			else if (SR >= 2000) l0 = 90;
			else if (SR >= 1800) l0 = 100;
			else if (SR >= 1600) l0 = 110;
			else if (SR >= 1400) l0 = 130;
			else if (SR >= 1200) l0 = 150;
		}
		else
		{
			if (SR >= 10000) l0 = 20;
			else if (SR >= 8000) l0 = 20;
			else if (SR >= 6000) l0 = 30;
			else if (SR >= 5000) l0 = 40;
			else if (SR >= 4000) l0 = 40;
			else if (SR >= 3000) l0 = 50;
			else if (SR >= 2500) l0 = 70;
			else if (SR >= 2000) l0 = 80;
			else if (SR >= 1800) l0 = 80;
			else if (SR >= 1600) l0 = 100;
			else if (SR >= 1400) l0 = 110;
			else if (SR >= 1200) l0 = 130;
		};
	}
	else if (Design_V == 120)
	{
		if (KNDD == 0)
		{
			if (SR >= 10000) l0 = 20;
			else if (SR >= 8000) l0 = 30;
			else if (SR >= 6000) l0 = 30;
			else if (SR >= 5000) l0 = 40;
			else if (SR >= 4000) l0 = 50;
			else if (SR >= 3000) l0 = 50;
			else if (SR >= 2500) l0 = 60;
			else if (SR >= 2000) l0 = 60;
			else if (SR >= 1800) l0 = 70;
			else if (SR >= 1600) l0 = 70;
			else if (SR >= 1400) l0 = 80;
			else if (SR >= 1200) l0 = 90;
			else if (SR >= 1000) l0 = 120;
			if (SR >= 800) l0 = 150;
		}
		else
		{
			if (SR >= 10000) l0 = 20;
			else if (SR >= 8000) l0 = 20;
			else if (SR >= 6000) l0 = 20;
			else if (SR >= 5000) l0 = 30;
			else if (SR >= 4000) l0 = 30;
			else if (SR >= 3000) l0 = 40;
			else if (SR >= 2500) l0 = 40;
			else if (SR >= 2000) l0 = 50;
			else if (SR >= 1800) l0 = 60;
			else if (SR >= 1600) l0 = 60;
			else if (SR >= 1400) l0 = 70;
			else if (SR >= 1200) l0 = 80;
			else if (SR >= 1000) l0 = 100;
			else if (SR >= 800) l0 = 130;
		};
	}
	else if (Design_V == 100)
	{
		if (KNDD == 0)
		{
			if (SR >= 10000) l0 = 20;
			else if (SR >= 8000) l0 = 20;
			else if (SR >= 6000) l0 = 20;
			else if (SR >= 5000) l0 = 20;
			else if (SR >= 4000) l0 = 30;
			else if (SR >= 3000) l0 = 40;
			else if (SR >= 2500) l0 = 40;
			else if (SR >= 2000) l0 = 50;
			else if (SR >= 1800) l0 = 50;
			else if (SR >= 1600) l0 = 50;
			else if (SR >= 1400) l0 = 60;
			else if (SR >= 1200) l0 = 60;
			else if (SR >= 1000) l0 = 70;
			else if (SR >= 800) l0 = 80;
			else if (SR >= 700) l0 = 100;
			else if (SR >= 600) l0 = 120;
			else if (SR >= 550) l0 = 130;
		}
		else
		{
			if (SR >= 10000) l0 = 20;
			else if (SR >= 8000) l0 = 20;
			else if (SR >= 6000) l0 = 20;
			else if (SR >= 5000) l0 = 20;
			else if (SR >= 4000) l0 = 20;
			else if (SR >= 3000) l0 = 20;
			else if (SR >= 2500) l0 = 30;
			else if (SR >= 2000) l0 = 40;
			else if (SR >= 1800) l0 = 40;
			else if (SR >= 1600) l0 = 40;
			else if (SR >= 1400) l0 = 40;
			else if (SR >= 1200) l0 = 50;
			else if (SR >= 1000) l0 = 60;
			else if (SR >= 800) l0 = 70;
			else if (SR >= 700) l0 = 90;
			else if (SR >= 600) l0 = 100;
			else if (SR >= 550) l0 = 110;
		};
	}
	else if (Design_V == 80)
	{
		if (KNDD == 0)
		{
			if (SR >= 10000) l0 = 20;
			else if (SR >= 8000) l0 = 20;
			else if (SR >= 6000) l0 = 20;
			else if (SR >= 5000) l0 = 20;
			else if (SR >= 4000) l0 = 20;
			else if (SR >= 3000) l0 = 20;
			else if (SR >= 2500) l0 = 30;
			else if (SR >= 2000) l0 = 30;
			else if (SR >= 1800) l0 = 30;
			else if (SR >= 1600) l0 = 40;
			else if (SR >= 1400) l0 = 40;
			else if (SR >= 1200) l0 = 40;
			else if (SR >= 1000) l0 = 40;
			else if (SR >= 800) l0 = 50;
			else if (SR >= 700) l0 = 50;
			else if (SR >= 600) l0 = 60;
			else if (SR >= 550) l0 = 60;
			else if (SR >= 500) l0 = 60;
			else if (SR >= 450) l0 = 80;
			else if (SR >= 400) l0 = 90;
			else if (SR >= 350) l0 = 100;
			else  l0 = 0.0;
		}
		else
		{
			if (SR >= 10000) l0 = 20;
			else if (SR >= 8000) l0 = 20;
			else if (SR >= 6000) l0 = 20;
			else if (SR >= 5000) l0 = 20;
			else if (SR >= 4000) l0 = 20;
			else if (SR >= 3000) l0 = 20;
			else if (SR >= 2500) l0 = 20;
			else if (SR >= 2000) l0 = 20;
			else if (SR >= 1800) l0 = 20;
			else if (SR >= 1600) l0 = 20;
			else if (SR >= 1400) l0 = 20;
			else if (SR >= 1200) l0 = 30;
			else if (SR >= 1000) l0 = 30;
			else if (SR >= 800) l0 = 40;
			else if (SR >= 700) l0 = 40;
			else if (SR >= 600) l0 = 50;
			else if (SR >= 550) l0 = 50;
			else if (SR >= 500) l0 = 60;
			else if (SR >= 450) l0 = 70;
			else if (SR >= 400) l0 = 80;
			else if (SR >= 350) l0 = 90;
		};
	};
	return l0;
}

//     功能：求直线段与直线段的交点 （笛卡儿坐标）
// 变量说明：P1、P2 定义直线的两点；P3、P4 定义另一直线的两点；
//   返回值：相交 99，PP为交点坐标; 平行-99; 不相交-89；

int BAS_DRAW_FUN::crossll(ads_point p1, ads_point p2, ads_point p3, ads_point p4, ads_point pp)
{  // 直线段与直线段  
	double a1, a2, b1, b2, c1, c2;
	int tmp = 0, bz = 99;
	a1 = p1[Y] - p2[Y];  b1 = p2[0] - p1[0];  c1 = p1[1] * (0 - b1) - p1[0] * a1;
	a2 = p3[Y] - p4[Y];  b2 = p4[0] - p3[0];  c2 = p3[1] * (0 - b2) - p3[0] * a2;
	if ((a1*b2 - a2 * b1) == 0) //平行
	{
		bz = -99; pp[0] = -1000; pp[1] = -1000;
	}
	else
	{
		pp[1] = (a1*c2 - a2 * c1) / (b1*a2 - a1 * b2);
		if (p1[1] == p2[1])
		{
			pp[0] = (-1)*(b2*pp[1] + c2) / a2;
		}
		else { pp[0] = (-1)*(b1*pp[1] + c1) / a1; };
		if (p1[1] != p2[1])
		{
			if ((p1[1] - pp[1])*(pp[1] - p2[1]) < 0) tmp = 1;
		};
		if (p1[0] != p2[0])
		{
			if ((p1[0] - pp[0])*(pp[0] - p2[0]) < 0) tmp = 1;
		};
		if (p3[1] != p4[1])
		{
			if ((p3[1] - pp[1])*(pp[1] - p4[1]) < 0) tmp = 1;
		};
		if (p3[0] != p4[0])
		{
			if ((p3[0] - pp[0])*(pp[0] - p4[0]) < 0) tmp = 1;
		};

		if (tmp == 1)
		{
			bz = -89;  pp[0] = -1000; pp[1] = -1000;
		};
	};
	return bz;
}
void BAS_DRAW_FUN::dmltockml(CString GH, double dml, ACHAR ckml[80])
{
	double ys = fmod(dml, 1000.0);
	if (fabs(ys - 1000.0) < 0.001)
	{
		_stprintf(ckml, L"%s%d+000", GH, int(dml / 1000 + 0.01));
		return;
	}
	if (GH != "")
	{
		if (ys >= 100.0)
			_stprintf(ckml, L"%s%d+%0.3lf", GH, int(dml / 1000), ys);
		else if (ys >= 10.0)
			_stprintf(ckml, L"%s%d+0%0.3lf", GH, int(dml / 1000), ys);
		else if (ys > 0.0)
			_stprintf(ckml, L"%s%d+00%0.3lf", GH, int(dml / 1000), ys);
		else
			_stprintf(ckml, L"%s%d+000", GH, int(dml / 1000));
	}
	else
	{
		if (ys >= 100.0)
			_stprintf(ckml, L"%s%d+%0.0lf", GH, int(dml / 1000), ys);
		else if (ys >= 10.0)
			_stprintf(ckml, L"%s%d+0%0.0lf", GH, int(dml / 1000), ys);
		else if (ys > 0.0)
			_stprintf(ckml, L"%s%d+00%0.0lf", GH, int(dml / 1000), ys);
		else
			_stprintf(ckml, L"%s%d+000", GH, int(dml / 1000));
	}
}

int BAS_DRAW_FUN::ReadRailWorkdir(ACHAR workdir[256], ACHAR pro[256])
{
	ACHAR fname[256];
	_tcscpy(workdir, L"");
	_tcscpy(pro, L"");
	//	GetCurrentDirectory(256,fname);


	/////////////////////////////////////

	/*ACHAR SysPath[256];
	_tcscpy(fname,acedGetAppName());
	CString fstr;
	fstr = fname;
	fstr.MakeUpper();
	fstr.Replace(L"\\ROADDESIGN.ARX",NULL);
	_tcscpy(SysPath,fstr);*/
	/////////////////////////////////////////(zdf改)




	_stprintf(fname, L"%s\\workdir", SysPath);//这边的syspath为空
	FILE *fpr = _wfopen(fname, L"r");
	if (!fpr)
	{
		AfxMessageBox(L"您还没有设定项目名称!");
		return 0;
	}

	fwscanf(fpr, L"%s", workdir);
	fclose(fpr);

	Cworkdir = workdir;
	InStr$toNull();
	_tcscpy(workdir, Cworkdir);

	CString ProFname;
	ProFname.Format(L"%s\\project", workdir);
	fpr = _wfopen(ProFname, L"r");
	if (!fpr)
	{
		AfxMessageBox(L"您还没有指定设计方案名称!");
		//   ads_printf(L"您还没有指定设计方案名称!\n");
		return 0;
	}
	fwscanf(fpr, L"%s", pro);
	fclose(fpr);

	return 1;
}

double BAS_DRAW_FUN::get_xjj(double Design_V)
{
	double xjj;
	if (Design_V < 155)
		xjj = 4.0;
	else if (Design_V < 161)
		xjj = 4.2;
	else if (Design_V < 201)
		xjj = 4.4;
	else if (Design_V < 251)
		xjj = 4.6;
	else
		xjj = 5.0;

	return xjj;
}

double BAS_DRAW_FUN::get_XJJK(double Design_V, double SR)//线间距加宽
{
	double R[20], JK[20];
	int NR;

	if (Design_V > 161)
		return 0.0;//不加宽
	if (fabs(Design_V - 160) < 0.1)
	{
		R[0] = 1200, R[1] = 1400, R[2] = 1600, R[3] = 1800, R[4] = 2000, R[5] = 2500, R[6] = 3000, R[7] = 4000, R[8] = 5000, R[9] = 6000, R[10] = 8000, R[11] = 10000;
		JK[0] = 0.21, JK[1] = 0.19, JK[2] = 0.15, JK[3] = 0.135, JK[4] = 0.125, JK[5] = 0.11, JK[6] = 0.1, JK[7] = 0.085, JK[8] = 0.08, JK[9] = 0.065, JK[10] = 0.05, JK[11] = 0.035;
		NR = 12;
	}
	else if (fabs(Design_V - 140) < 0.1)
	{
		R[0] = 1200, R[1] = 1400, R[2] = 1600, R[3] = 1800, R[4] = 2000, R[5] = 2500, R[6] = 3000, R[7] = 4000, R[8] = 5000, R[9] = 6000, R[10] = 8000, R[11] = 10000;
		JK[0] = 0.21, JK[1] = 0.19, JK[2] = 0.15, JK[3] = 0.135, JK[4] = 0.125, JK[5] = 0.11, JK[6] = 0.1, JK[7] = 0.085, JK[8] = 0.08, JK[9] = 0.065, JK[10] = 0.05, JK[11] = 0.035;
		NR = 12;
	}
	else if (fabs(Design_V - 120) < 0.1)
	{
		R[0] = 800, R[1] = 1000, R[2] = 1200, R[3] = 1400, R[4] = 1600, R[5] = 1800, R[6] = 2000, R[7] = 2500, R[8] = 3000, R[9] = 4000, R[10] = 5000, R[11] = 6000, R[12] = 8000, R[13] = 10000;
		JK[0] = 0.265, JK[1] = 0.22, JK[2] = 0.165, JK[3] = 0.15, JK[4] = 0.135, JK[5] = 0.125, JK[6] = 0.115, JK[7] = 0.1, JK[8] = 0.09, JK[9] = 0.085, JK[10] = 0.055, JK[11] = 0.05, JK[12] = 0.035, JK[13] = 0.03;
		NR = 14;
	}
	else if (fabs(Design_V - 100) < 0.1)
	{
		R[0] = 550, R[1] = 600, R[2] = 700, R[3] = 800, R[4] = 1000, R[5] = 1200, R[6] = 1400, R[7] = 1600, R[8] = 1800, R[9] = 2000, R[10] = 2500, R[11] = 3000, R[12] = 4000, R[13] = 5000, R[14] = 6000, R[15] = 8000, R[16] = 10000;
		JK[0] = 0.315, JK[1] = 0.295, JK[2] = 0.26, JK[3] = 0.21, JK[4] = 0.175, JK[5] = 0.155, JK[6] = 0.135, JK[7] = 0.125, JK[8] = 0.11, JK[9] = 0.105, JK[10] = 0.1, JK[11] = 0.08, JK[12] = 0.055, JK[13] = 0.04, JK[14] = 0.035, JK[15] = 0.025, JK[16] = 0.02;
		NR = 17;
	}
	else if (fabs(Design_V - 80) < 0.1)
	{
		R[0] = 350, R[1] = 400, R[2] = 450, R[3] = 500, R[4] = 550, R[5] = 600, R[6] = 700, R[7] = 800, R[8] = 1000, R[9] = 1200, R[10] = 1400, R[11] = 1600, R[12] = 1800, R[13] = 2000, R[14] = 2500, R[15] = 3000, R[16] = 4000, R[17] = 5000, R[18] = 6000, R[19] = 8000, R[20] = 10000;
		JK[0] = 0.41, JK[1] = 0.365, JK[2] = 0.335, JK[3] = 0.28, JK[4] = 0.255, JK[5] = 0.235, JK[6] = 0.21, JK[7] = 0.19, JK[8] = 0.155, JK[9] = 0.135, JK[10] = 0.125, JK[11] = 0.115, JK[12] = 0.1, JK[13] = 0.095, JK[14] = 0.07, JK[15] = 0.065, JK[16] = 0.04, JK[17] = 0.035, JK[18] = 0.025, JK[19] = 0.015, JK[20] = 0.015;
		NR = 21;
	}
	else if (Design_V < 75)
	{
		R[0] = 150, R[1] = 180, R[2] = 200, R[3] = 250, R[4] = 300, R[5] = 350, R[6] = 400, R[7] = 450, R[8] = 500, R[9] = 550, R[10] = 600, R[11] = 700, R[12] = 800, R[13] = 1000, R[14] = 1200, R[15] = 1500, R[16] = 2000, R[17] = 2500, R[18] = 3000, R[19] = 4000;
		JK[0] = 0.715, JK[1] = 0.62, JK[2] = 0.575, JK[3] = 0.49, JK[4] = 0.43, JK[5] = 0.37, JK[6] = 0.325, JK[7] = 0.29, JK[8] = 0.26, JK[9] = 0.235, JK[10] = 0.215, JK[11] = 0.185, JK[12] = 0.16, JK[13] = 0.13, JK[14] = 0.11, JK[15] = 0.085, JK[16] = 0.065, JK[17] = 0.05, JK[18] = 0.045, JK[19] = 0.035;
		NR = 20;
	}

	int i = 0;
	while (i < NR && R[i] < SR - 1)
		i++;

	if (i == 0)
		return JK[0];
	else if (i == NR)
	{
		if (SR > R[NR - 1])
			return 0.0;//不加宽
		else
			i--;
	}
	double jk = JK[i - 1] + (JK[i] - JK[i - 1])*(SR - R[i - 1]) / (R[i] - R[i - 1]);
	return  jk;

	//	ads_printf(L"v =%9d\n",Design_V);
	//	ads_printf(L"KNLB = %9d\n",KNLB);

}

double  BAS_DRAW_FUN::Search_EXPt(double array2[4 * MAXJDNUM][10], int trow2, double Pt1[3], double jiao, double ResPt[2])
//二线线元参数，线元个数，给定线外一点与角度定出直线找到该直线与二线交点ResPt及里程projml
{

	int sk = Search_XY(array2, trow2, Pt1);

	double Spt[3], Ept[3];
	double x1, y1, x2, y2;
	Spt[0] = Pt1[0], Spt[1] = Pt1[1], Spt[2] = Pt1[2];
	Ept[0] = Pt1[0] + 1000 * cos(jiao);
	Ept[1] = Pt1[1] + 1000 * sin(jiao);

	int ek = Search_XY(array2, trow2, Ept);

	int Kmin, Kmax;
	Kmin = sk < ek ? sk : ek;
	Kmax = sk < ek ? ek : sk;

	if (Kmin > 1)
		Kmin--;
	if (Kmax < trow2 - 1)
		Kmax++;

	double ProjMl = -1.0;
	int inf = 1;

	//	ads_printf(L"Kmin=%d Kmax=%d\n",Kmin,Kmax);
	if (Kmin > 1)
		x2 = array2[Kmin - 1][8], y2 = array2[Kmin - 1][9];
	else
	{
		//起始线元延长100m
		double fwj = array2[1][5] + pi;
		x2 = array2[0][1] + 100.0*cos(fwj);
		y2 = array2[0][2] + 100.0*sin(fwj);

		//x2 =  , y2 = array2[0][2];
	}
	double xmin, ymin, distmin, dist, ProjMlmin;//距Spt最近的点
	distmin = 100000;
	//	for(int i=Kmin ; i<=Kmax; i++)//遍历找交点
	for (int i = 0; i <= trow2; i++)//遍历找交点
	{
		x1 = x2, y1 = y2;
		if (i < trow2 - 1)
			x2 = array2[i + 1][8], y2 = array2[i + 1][9];
		else
			x2 = array2[trow2][8], y2 = array2[trow2][9];


		//	 ads_printf(L"###i=%d lc=%lf x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",i,array2[i][6],x1,y1,x2,y2);
		//	 ads_printf(L"\n$$$$$%lf,%lf,%lf,%lf",Spt[0],Spt[1],Ept[0],Ept[1]);
		//i,Spt[0],Spt[1],Ept[0],Ept[1],x1,y1,x2,y2);


		int res = IfOnXY(Spt[0], Spt[1], Ept[0], Ept[1], x1, y1, x2, y2);


		if (res == 1)//在线元上
		{
			inf = CalXYjd(Spt, Ept, array2, trow2, i, ResPt, ProjMl);

			//		 acutPrintf(L"inf=%d,ResPt[0]=%lf,ResPt[1]=%lf,ProjML=%lf\n",inf,ResPt[0],ResPt[1],ProjMl);

			if (inf == 0)//找到
			{
				//计算投影里程
				dist = sqrt((Spt[0] - ResPt[0])*(Spt[0] - ResPt[0]) + (Spt[1] - ResPt[1])*(Spt[1] - ResPt[1]));
				if (distmin > dist)
				{
					xmin = ResPt[0];
					ymin = ResPt[1];
					distmin = dist;
					ProjMlmin = ProjMl;
				}
			}

		}
	}



	if (distmin < 100000)
	{
		ResPt[0] = xmin;
		ResPt[1] = ymin;
		return ProjMlmin;
	}
	else return -1.0;//找不到交点

}

int BAS_DRAW_FUN::INTERS(double *x, double *y, double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)
{
	double A11, A12, A21, A22, B1, B2, D, D1, D2;
	int INF;
	double X1min, Y1min, X2min, Y2min, X1max, Y1max, X2max, Y2max;


	A11 = Y2 - Y1;
	A12 = X1 - X2;
	A21 = Y4 - Y3;
	A22 = X3 - X4;
	B1 = X1 * Y2 - X2 * Y1;
	B2 = X3 * Y4 - X4 * Y3;
	D = A11 * A22 - A21 * A12;
	if (fabs(D) < 1.0E-6)
		INF = -1;
	else
	{
		D1 = B1 * A22 - B2 * A12;
		D2 = B2 * A11 - B1 * A21;

		*x = D1 / D;
		*y = D2 / D;
		X1min = min(X1, X2) - 0.001;
		X1max = max(X1, X2) + 0.001;
		Y1min = min(Y1, Y2) - 0.001;
		Y1max = max(Y1, Y2) + 0.001;
		X2min = min(X3, X4) - 0.001;
		X2max = max(X3, X4) + 0.001;
		Y2min = min(Y3, Y4) - 0.001;
		Y2max = max(Y3, Y4) + 0.001;

		if (X1min <= *x && *x <= X1max && Y1min <= *y && *y <= Y1max
			&& X2min <= *x && *x <= X2max
			&& Y2min <= *y && *y <= Y2max)
			INF = 0;
		else
			INF = 1;
	}

	return INF;
}

int BAS_DRAW_FUN::Search_XY(double array2[4 * MAXJDNUM][10], int trow2, double Pt1[3])//定位pt1点所在线元
{
	int i;
	double cml;

	cml = PROJ_ML(array2, trow2, Pt1[0], Pt1[1], Pt1[0], Pt1[1]);//2004.5.14


	for (i = 1; i < trow2; i++)
		if (cml > array2[i][6] - 0.001&&cml < array2[i + 1][6])
			return i;
	// if(cml<array2[trow2][6]+1)
	return trow2;
	// else
	//     ads_printf(L"无法定位点cml=%0.2lf %0.2lf %0.2lf对应线元!\n",cml,Pt1[0],Pt1[1]);
	//	return trow2;
}

int BAS_DRAW_FUN::CalXYjd(double Spt[3], double Ept[3], double array2[4 * MAXJDNUM][10], int trow2, int kk, double ResPt[2], double &ProjMl)
{
	double q;
	double s1, lc, s;
	double XYspt[2], XYept[2], XYLen;
	//第一段可能有问题
	q = array2[kk][0];
	if (kk > 0)
		lc = array2[kk][6];//起始里程
	else
		lc = array2[0][4];

	if (kk > 0)
	{
		XYspt[0] = array2[kk][8];
		XYspt[1] = array2[kk][9];
		if (kk < trow2)
		{
			XYept[0] = array2[kk + 1][8];
			XYept[1] = array2[kk + 1][9];
			XYLen = array2[kk + 1][6] - lc;
		}
		else
		{
			XYept[0] = array2[kk][8];
			XYept[1] = array2[kk][9];
			XYLen = 0.0;
		}
	}
	else
	{
		XYspt[0] = array2[kk][1];
		XYspt[1] = array2[kk][2];
		XYept[0] = array2[kk + 1][8];
		XYept[1] = array2[kk + 1][9];
		XYLen = 0.0;
	}

	if (XYLen < 0.0001)
	{
		ProjMl = lc;
		ResPt[0] = XYspt[0], ResPt[1] = XYspt[1];
		return 0;
	}

	if (q < 1.1)//线元为直线
	{
		INTERS(&ResPt[0], &ResPt[1], Spt[0], Spt[1], Ept[0], Ept[1], XYspt[0], XYspt[1], XYept[0], XYept[1]);
		s1 = sqrt((ResPt[0] - XYspt[0])*(ResPt[0] - XYspt[0]) + (ResPt[1] - XYspt[1])*(ResPt[1] - XYspt[1]));
		ProjMl = lc + s1;
		return 0;//找到
	}
	else if (q < 2.1)//线元为圆
	{
		double R, Xo, Yo;
		double LorR = array2[kk][4];//?左右转
		R = array2[kk][1];//2004518
		Xo = array2[kk][8] + R * cos(array2[kk][5] + LorR * 0.5*pi);
		Yo = array2[kk][9] + R * sin(array2[kk][5] + LorR * 0.5*pi);//圆心

		double x1, y1, x2, y2;
		if (fabs(Ept[0] - Spt[0]) > 0.1)
		{
			double K, b;
			K = (Ept[1] - Spt[1]) / (Ept[0] - Spt[0]);
			b = Ept[1] - K * Ept[0];
			double A = 1 + K * K;
			double B = 2 * K*(b - Yo) - 2 * Xo;
			double C = Xo * Xo + (b - Yo)*(b - Yo) - R * R;


			x1 = -B / A / 2.0 + sqrt((B / A)*(B / A) / 4 - C / A);
			y1 = K * x1 + b;
			x2 = -B / A / 2.0 - sqrt((B / A)*(B / A) / 4 - C / A);
			y2 = K * x2 + b;
		}
		else//平行于Y轴
		{
			x1 = x2 = Spt[0];
			y1 = Yo + sqrt(R*R - (Spt[0] - Xo)*(Spt[0] - Xo));
			y2 = Yo - sqrt(R*R - (Spt[0] - Xo)*(Spt[0] - Xo));
		}
		//ads_printf(L"1:x1=%lf y1=%lf\n",x1,y1);
		double ml1 = PROJ_ML(array2, trow2, x1, y1, x1, y1);
		xlpoint PZ;
		PZ.lc = ml1;
		xlpoint_pz(array2, trow2, &PZ);
		double dis1 = sqrt((x1 - PZ.x)*(x1 - PZ.x) + (y1 - PZ.y)*(y1 - PZ.y));
		//ads_printf(L"2:x2=%lf y2=%lf\n",x2,y2);	
		double ml2 = PROJ_ML(array2, trow2, x2, y2, x2, y2);
		PZ.lc = ml2;
		xlpoint_pz(array2, trow2, &PZ);
		double dis2 = sqrt((x2 - PZ.x)*(x2 - PZ.x) + (y2 - PZ.y)*(y2 - PZ.y));
		//所求交点到线路距离,取距离线路最近的点
		//ads_printf(L"@@@@@@@@@dis1=%lf dis2=%lf ml1=%lf ml2=%lf\n",dis1,dis2,ml1,ml2);
		if (dis1 < dis2)
		{
			ResPt[0] = x1, ResPt[1] = y1;
			ProjMl = Circle_XY_to_ML(x1, y1, array2, kk);
			//	ads_printf(L"圆: x1=%lf y1=%lf ml=%lf\n",x1,y1,ProjMl);
			return 0;
		}
		else
		{
			ResPt[0] = x2, ResPt[1] = y2;
			ProjMl = Circle_XY_to_ML(x2, y2, array2, kk);
			//		ads_printf(L"圆: x2=%lf y2=%lf ml=%lf %lf %lf\n",x2,y2,ProjMl,PZ.x,PZ.y);
			return 0;
		}
	}
	else //线元为缓和曲线
	{
		double s1, s2;
		struct xlpoint PZ;
		double pt2[2];
		double Alf1, Alf2;

		s1 = lc;
		s2 = array2[kk + 1][6];
		Alf1 = Spt[2];
		while (fabs(s2 - s1) > 0.0001)
		{
			/*
			PZ.lc = s1;
			xlpoint_pz(array2,trow2,&PZ);
			pt1[0] = PZ.x ,pt1[1] = PZ.y;*/


			s = 0.5*(s1 + s2);
			PZ.lc = s;
			xlpoint_pz(array2, trow2, &PZ);
			pt2[0] = PZ.x, pt2[1] = PZ.y;

			xyddaa(Spt[0], Spt[1], PZ.x, PZ.y, &Alf2);
			double zj = ZXJ(Alf1, Alf2);

			if (zj > 0)//二线在右侧
			{
				if (zj > 0.5*pi)
					s1 = s;
				else
					s2 = s;
			}
			else
			{
				if (fabs(zj) > 0.5*pi)
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
		ProjMl = 0.5*(s1 + s2);
		PZ.lc = ProjMl;
		xlpoint_pz(array2, trow2, &PZ);
		ResPt[0] = PZ.x, ResPt[1] = PZ.y;
		return 0;
		//  }
	}
	return -1;
}
int BAS_DRAW_FUN::IfOnXY(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4)//是否与线元有交点
{
	double A11, A12, A21, A22, B1, B2, D, D1, D2;
	int INF;
	double X1min, Y1min, X2min, Y2min, X1max, Y1max, X2max, Y2max;
	double x, y;


	A11 = Y2 - Y1;
	A12 = X1 - X2;
	A21 = Y4 - Y3;
	A22 = X3 - X4;
	B1 = X1 * Y2 - X2 * Y1;
	B2 = X3 * Y4 - X4 * Y3;
	D = A11 * A22 - A21 * A12;
	if (fabs(D) < 1.0E-5)
		INF = 0;
	else
	{
		D1 = B1 * A22 - B2 * A12;
		D2 = B2 * A11 - B1 * A21;

		x = D1 / D;
		y = D2 / D;
		X1min = min(X1, X2) - 0.001;
		X1max = max(X1, X2) + 0.001;
		Y1min = min(Y1, Y2) - 0.001;
		Y1max = max(Y1, Y2) + 0.001;
		X2min = min(X3, X4) - 0.001;
		X2max = max(X3, X4) + 0.001;
		Y2min = min(Y3, Y4) - 0.001;
		Y2max = max(Y3, Y4) + 0.001;

		if (X2min <= x && x <= X2max && Y2min <= y && y <= Y2max)//在线元上
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
//线间距是指过一线上连续里程为ml1的点Spt做垂线，该垂线与二线相交于Ept，Spt,Ept之间的距离为线间距的绝对值。二线在右侧时线间距为正，左侧为负。重合为0
int BAS_DRAW_FUN::CalXjj(double ml1, double array1[4 * MAXJDNUM][10], int tarray1, double array2[4 * MAXJDNUM][10], int tarray2, double &xjj, double &projml, double resPt[2])
{
	//假设为右侧;
	xlpoint PZ;
	PZ.lc = ml1;
	xlpoint_pz(array1, tarray1, &PZ);
	//	acutPrintf(L"----------lc=%lf,x=%lf,y=%lf\n",PZ.lc,PZ.x,PZ.y);
	double fwj = PZ.a;
	double Pt1[3];
	Pt1[0] = PZ.x, Pt1[1] = PZ.y, Pt1[2] = PZ.a;//一线上的点

	//判断二线位于左侧还是右侧
	int LorR;

	LorR = Judge2XLorR(PZ, array2, tarray2);

	if (LorR == 0)//二线与一线重合518
	{
		xjj = 0.0;
		projml = PROJ_ML(array2, tarray2, Pt1[0], Pt1[1], Pt1[0], Pt1[1]);//投影到2线
		resPt[0] = Pt1[0]; resPt[1] = Pt1[1];
	}
	else
	{
		projml = Search_EXPt(array2, tarray2, Pt1, fwj + LorR * 0.5*pi, resPt);//点在二线的投影连续里程几投影点

		xjj = 0.0;
		if (projml < -0.001)
		{
			//	acutPrintf(L"ML1=%lf LorR=%d %lf %lf\n",ml1,LorR,Pt1[0],Pt1[1]);
			return 0;//没找到
		}
		xjj = LorR * sqrt((Pt1[0] - resPt[0])*(Pt1[0] - resPt[0]) + (Pt1[1] - resPt[1])*(Pt1[1] - resPt[1]));
		if (fabs(xjj) < 0.001)
			xjj = 0.0;

	}
	return 1;
}
//输入一线里程，一线线元参数，二线线元数据，求线间距,二线投影连续里程及投影点
//int BAS_DRAW_FUN::CalXjj(double ml1,double array1[400][10],int tarray1,double array2[400][10],int tarray2,double &xjj,double &projml,double resPt[2])
//{
//	//假设为右侧;
//    xlpoint PZ;
//	PZ.lc = ml1;
//	xlpoint_pz(array1,tarray1,&PZ);
//	double fwj = PZ.a;
//	double Pt1[3];
//	Pt1[0]=PZ.x,Pt1[1]=PZ.y,Pt1[2]=PZ.a;
//	//判断二线位于左侧还是右侧
//	int LorR;
//	LorR = Judge2XLorR(PZ,array2,tarray2);
//	
//	projml = Search_EXPt(array2,tarray2,Pt1,fwj+LorR*0.5*pi,resPt);
//	xjj = 0.0;
//    if(projml<-0.001)
//		return 0;//没找到
//	xjj = LorR*sqrt((Pt1[0]-resPt[0])*(Pt1[0]-resPt[0])+(Pt1[1]-resPt[1])*(Pt1[1]-resPt[1]));
//	return 1;
//}
//输入一线上一点PZ,二线线元数据
int BAS_DRAW_FUN::Judge2XLorR(xlpoint PZ, double array2[4 * MAXJDNUM][10], int tarray2)
{
	//	double dist;
	//	return -JudgePTLorR(PZ.x,PZ.y,dist,array2,tarray2);
	double Pt1[2], fwj, cml;
	fwj = PZ.a;
	cml = PZ.lc;
	Pt1[0] = PZ.x, Pt1[1] = PZ.y;
	double ml2 = PROJ_ML(array2, tarray2, Pt1[0], Pt1[1], Pt1[0], Pt1[1]);//投影到2线
	xlpoint PZ2;
	PZ2.lc = ml2;
	xlpoint_pz(array2, tarray2, &PZ2);////

	double dist = sqrt((Pt1[0] - PZ2.x)*(Pt1[0] - PZ2.x) + (Pt1[1] - PZ2.y)*(Pt1[1] - PZ2.y));//两个投影点距离

	if (dist < 0.001)//二线与一线重合
		return 0;

	double fwj1to2;
	xyddaa(Pt1[0], Pt1[1], PZ2.x, PZ2.y, &fwj1to2);
	int LorR;
	double zj = ZXJ(fwj1to2, fwj);
	if (zj > 0.0)//
		LorR = 1;//二线在右侧
	else
		LorR = -1;//二线在左侧

	//	if(fabs(cml-84203)<0.1)
	//		ads_printf(L"cml1=%lf ml2=%lf %lf x1=%lf y1=%lf x2=%lf y2=%lf fwj1=%lf fwj2=%lf zj=%lf %d\n",cml,ml2,dist,Pt1[0],Pt1[1],PZ2.x,PZ2.y,fwj,fwj1to2,zj,LorR);
	return LorR;
}
//输入一线上一点PZ,二线线元数据
//int BAS_DRAW_FUN::Judge2XLorR(xlpoint PZ,double array2[400][10],int tarray2)
//{
//	double Pt1[2],fwj;
//	fwj = PZ.a;
//	Pt1[0] = PZ.x,Pt1[1] = PZ.y;
//	double ml2 = PROJ_ML(array2,tarray2,Pt1[0],Pt1[1],Pt1[0],Pt1[1]);//投影到2线
//	PZ.lc = ml2;
//	xlpoint_pz(array2,tarray2,&PZ);
//	double fwj1to2;
//	xyddaa(Pt1[0],Pt1[1],PZ.x,PZ.y,&fwj1to2);
//	int LorR;
//	double zj = ZXJ(fwj1to2,fwj);
//	if(zj>0.0)//
//		LorR = 1;//二线在右侧
//	else
//		LorR = -1;//二线在左侧
//	
//	return LorR;
//}
//----------------------------------------------------------------------------------
double BAS_DRAW_FUN::Get_QXJK(CString TLDJ, double design_V, double SR)
{
	double qxjk;

	if (TLDJ == "地方I级")
	{
		if (SR < 401)	     qxjk = 0.4;
		else if (SR < 451)	 qxjk = 0.3;
		else if (SR < 701)  qxjk = 0.2;
		else if (SR < 3001) qxjk = 0.1;
		else qxjk = 0.0;
	}
	else if (TLDJ == "地方II级")
	{
		if (SR < 401)	     qxjk = 0.3;
		else if (SR < 451)	 qxjk = 0.2;
		else if (SR < 1201)  qxjk = 0.1;
		else qxjk = 0.0;
	}
	else if (TLDJ == "地方III级")
	{
		if (SR < 301)	     qxjk = 0.3;
		else if (SR < 451)	 qxjk = 0.2;
		else if (SR < 1201)  qxjk = 0.1;
		else qxjk = 0.0;
	}
	else if (TLDJ == "I级" || TLDJ == "II级")
	{
		if (SR < 801)	     qxjk = 0.5;
		else if (SR < 1001)	 qxjk = 0.4;
		else if (SR < 1601)  qxjk = 0.3;
		else if (SR < 6001)  qxjk = 0.2;
		else if (SR < 10001)  qxjk = 0.1;
		else qxjk = 0.0;
	}
	else  if (TLDJ == "III级")
	{
		if (SR < 601)	     qxjk = 0.5;
		else if (SR < 801)	 qxjk = 0.4;
		else if (SR < 1001)  qxjk = 0.3;
		else if (SR < 2001)  qxjk = 0.2;
		else if (SR < 5001)  qxjk = 0.1;
		else qxjk = 0.0;
	}
	else if (fabs(design_V - 160) < 0.5)
	{
		if (SR < 2001)	     qxjk = 0.4;
		else if (SR < 4001)	 qxjk = 0.3;
		else if (SR < 6001)  qxjk = 0.2;
		else if (SR < 8001)  qxjk = 0.1;
		else qxjk = 0.0;
	}
	else if (fabs(design_V - 200) < 0.5)
	{
		if (SR < 3001)	     qxjk = 0.4;
		else if (SR < 6001)  qxjk = 0.3;
		else qxjk = 0.2;
	}
	else if (fabs(design_V - 250) < 0.5)
	{
		if (SR < 3001)	     qxjk = 0.4;
		else if (SR < 6001)  qxjk = 0.3;
		else qxjk = 0.2;
	}
	else if (design_V > 299)
	{
		if (SR < 5501)	     qxjk = 0.4;
		else if (SR < 7001)  qxjk = 0.5;
		else if (SR < 11001)  qxjk = 0.4;
		else if (SR < 14001)  qxjk = 0.3;
	}
	else
		qxjk = 0.0;
	return qxjk;
}
//竖曲线半径
double BAS_DRAW_FUN::Get_Rshu(double design_V, double deltaI)
{
	double Rshu = 0.0;

	if (fabs(design_V - 70) < 0.1 || fabs(design_V - 55) < 0.1)
	{
		if (deltaI > 4.0)
			Rshu = 5000.0;
	}
	else if (fabs(design_V - 40) < 0.1)
	{
		if (deltaI > 5.0)
			Rshu = 3000.0;
	}
	else if (fabs(design_V - 140) < 0.1 || fabs(design_V - 120) < 0.1)
	{
		if (deltaI > 3.0)
			Rshu = 10000.0;
	}
	else if (fabs(design_V - 100) < 0.1)
	{
		if (deltaI > 4.0)
			Rshu = 5000.0;
	}
	else if (fabs(design_V - 80) < 0.1)
	{
		if (deltaI > 3.0)
			Rshu = 10000.0;
	}
	else if (fabs(design_V - 160) < 0.1)
	{
		if (deltaI > 0.99)
			Rshu = 15000.0;
	}
	else if (fabs(design_V - 200) < 0.1)
	{
		if (deltaI > 0.99)
			Rshu = 15000.0;
	}
	else if (fabs(design_V - 250) < 0.1)
	{
		if (deltaI > 0.99)
			Rshu = 20000.0;
	}
	else if (design_V > 299)
	{
		if (deltaI > 0.99)
			Rshu = 25000.0;
	}
	return Rshu;
}


double BAS_DRAW_FUN::PXJ(double fwj1, double fwj2)
{
	double jiao;
	jiao = fwj2 - fwj1;
	if (jiao < 0)jiao += 2 * pi;
	if (jiao > pi)jiao = 2 * pi - jiao;
	return fabs(jiao);
}

double BAS_DRAW_FUN::fwj(double x0, double y0, double x1, double y1)
{

	ads_point spt, ept;
	spt[X] = x0;
	spt[Y] = y0;
	ept[X] = x1;
	ept[Y] = y1;
	double ang = ads_angle(spt, ept);//与X轴夹角
	ang = 2.5*pi - ang;
	if (ang > 2 * pi)ang -= 2 * pi;
	return ang;
}

double BAS_DRAW_FUN::AFA(double fwj1, double fwj2)
{
	double af;
	af = fwj2 - fwj1;
	if (af < 0)af += 2 * pi;
	if (af > 2 * pi)af = 2 * pi - af;
	return af;
}

double BAS_DRAW_FUN::radiusfree(AcDbLine *Line1, AcDbLine *Line2, AcGePoint3d pk)
{
	AcGePoint3d sPt1, ePt1, sPt2, ePt2;
	double rad = 0.0, XdE, xa, xb, xc, dita, jbata;
	AcDbObjectId arcId;

	if (!Line1 || !Line2)
		return 0.0;


	//	if(acdbOpenObject(Line1,Line1->objectId(),AcDb::kForRead,Adesk::kFalse)!=Acad::eOk)
	//		return 0.0;

	//	if(acdbOpenObject(Line2,Line2->objectId(),AcDb::kForRead,Adesk::kFalse)!=Acad::eOk)
	//		return 0.0;

	sPt1 = Line1->startPoint();
	ePt1 = Line1->endPoint();
	sPt2 = Line2->startPoint();
	ePt2 = Line2->endPoint();

	AcGePoint3dArray interPt;
	if (Acad::eOk != Line1->intersectWith(Line2, AcDb::kExtendBoth, interPt, 0, 0))
	{
		return 0.0;
	}


	double fwj1 = fwj(sPt1.x, sPt1.y, ePt1.x, ePt1.y);
	double fwj2 = fwj(sPt2.x, sPt2.y, ePt2.x, ePt2.y);
	double af1 = AFA(fwj1, fwj2);
	double fwjr = -af1 / 2 + fwj2 + PI / 2;
	ads_point pt1, pt2;
	pt1[X] = interPt[0].x;
	pt1[Y] = interPt[0].y;
	pt2[X] = pk.x;
	pt2[Y] = pk.y;
	double fwje = 2.5*PI - ads_angle(pt2, pt1);
	jbata = PI - PXJ(fwje, fwjr);
	//	pk.x=20;
	//	pk.y=20;
	//	pk.z=0;
	XdE = pk.distanceTo(interPt[0]);
	double af = PXJ(fwj1, fwj2);
	xa = 1 / pow(cos(af / 2), 2) - 1;
	xb = -2 * XdE*cos(jbata) / cos(af / 2);
	xc = pow(XdE, 2);
	dita = pow(xb, 2) - 4 * xa*xc;
	rad = (-xb + powf(dita, 0.5)) / (2 * xa);
	//	Line1->close();
	//	Line2->close();
	return rad;
}

//plh07.25
double BAS_DRAW_FUN::GetRFromT1T2(double &x1, double &y1, double &x0, double &y0, double &x2, double &y2, bool IsStandard, double DV, int INTRadius, int JUDGET012)
//给定一个交点，直始x1,y1,交点x0,y0,终止x2,y2,是否遵守半径系列，返回半径
//int JUDGET012=0//不进行夹直线判断？int JUDGET012=1//与前面夹直线判断int JUDGET012=2//与后面夹直线判断int JUDGET012=12//与前后夹直线判断
{
	//	acutPrintf(L"x1=%.2lf,y1=%.2lf,x0=%.2lf,y0=%.2lf,x2=%.2lf,y2=%.2lf\n",x1,y1,x0,y0,x2,y2);

	bool ISERROR = false;
	double T0min, Lrmin;
	//	GetT0min_Lrmin(DV,T0min,Lrmin);
	T0min = 0.0;
	Lrmin = 0.0;
	double T1 = 99999, T2 = 99999;
	//由x1,y1,x0,y0,x2,y2确定最大切长以保证最小夹直线长
	double Tmax, Tmax1, Tmax2;
	if (JUDGET012 == 0)
	{
		Tmax = 99999999;
	}
	else if (JUDGET012 == 1)
	{
		Tmax = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)) - T0min;
	}
	else if (JUDGET012 == 2)
	{
		Tmax = sqrt((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)) - T0min;
	}
	else//12
	{
		Tmax1 = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)) - T0min;
		Tmax2 = sqrt((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)) - T0min;
		Tmax = min(Tmax1, Tmax2);
	}
	ads_point pt1, pt2;
	AcDbObjectId eId, eId1, arcId, LId1, LId2;
	AcGePoint3d center;//圆心

	AcGePoint3d sPt(x1, y1, 0.0), sPt1(x0, y0, 0.0);
	AcGePoint3d ePt(x0, y0, 0.0), ePt1(x2, y2, 0.0);
	AcGePoint2d spoint1(x1, y1), epoint1(x0, y0), spoint2(x0, y0), epoint2(x2, y2);
	AcGeLine2d Line1(spoint1, epoint1);
	AcGeLine2d Line2(spoint2, epoint2);
	AcDbLine *pl1 = new AcDbLine(sPt, ePt);
	AcDbLine *pl2 = new AcDbLine(sPt1, ePt1);
	/*
	pl1->setLineWeight(AcDb::kLnWt080);
	pl2->setLineWeight(AcDb::kLnWt080);
	*/
	LId1 = AddEntityToDbs(pl1);
	LId2 = AddEntityToDbs(pl2);

	double fwj1, fwj2;
	fwj1 = fwj(sPt.x, sPt.y, ePt.x, ePt.y);
	fwj2 = fwj(sPt1.x, sPt1.y, ePt1.x, ePt1.y);
	double pxj = PXJ(fwj1, fwj2);
	double afa = AFA(fwj1, fwj2);
	double fwjr = (pi - afa) / 2 + fwj2;
	double rad = -1.0, dT, jiao;
	double angle1, angle2;
	double ang1, ang2;
	AcGePoint3d pk(0, 0, 0);
	int type;
	struct resbuf result;
	AcDbArc *pArc = new AcDbArc;//(center,rad,angle1,angle2);
	arcId = AddEntityToDbs(pArc);
	/*acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
	if (acdbOpenObject(pArc, arcId, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
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
	AcDbLine *perLine3d1 = new AcDbLine();
	perLine3d1->setColorIndex(3);
	AcDbObjectId perLId1 = AddEntityToDbs(perLine3d1);

	AcDbLine *perLine3d2 = new AcDbLine();
	perLine3d2->setColorIndex(3);
	AcDbObjectId perLId2 = AddEntityToDbs(perLine3d2);
	bool ISOK = true;
	double tmpR = 0.0;
	for (;;)
	{
		if (ads_grread(1, &type, &result) != RTNORM)
		{
			ISOK = false;
			break;
		}
		if (type == 2)
		{
			short KEYCODE = result.resval.rint;
			//acutPrintf(L"%d",KEYCODE);
			if (KEYCODE == 32)
			{
			}
			else if (KEYCODE == 13)
			{
				break;
			}
			else {}
		}
		else if (type == 25)
		{
			if (rad < 0.00001)continue;
			double cenX, cenY, dx, dy;
			cenX = center.x;
			cenY = center.y;
			if (GetCircleFromT_P_R(x1, y1, x0, y0, rad, cenX, cenY) != true)continue;
			dx = cenX - center.x;
			dy = cenY - center.y;
			//			acutPrintf(L"dx=%lf,dy=%lf\n",dx,dy);
			x0 += dx;
			y0 += dy;
			//			x1+=dx;
			//			y1+=dy;
			x2 += dx;
			y2 += dy;
			/*acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
			if (acdbOpenObject(pl1, LId1, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			/*acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse);*/
			if (acdbOpenObject(pl2, LId2, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			pl1->setStartPoint(AcGePoint3d(x1, y1, 0.0));
			pl1->setEndPoint(AcGePoint3d(x0, y0, 0.0));
			pl2->setStartPoint(AcGePoint3d(x0, y0, 0.0));
			pl2->setEndPoint(AcGePoint3d(x2, y2, 0.0));
			pl1->close();
			pl2->close();
			if (JUDGET012 == 0)
			{
				Tmax = 99999999;
			}
			else if (JUDGET012 == 1)
			{
				Tmax = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)) - T0min;
			}
			else if (JUDGET012 == 2)
			{
				Tmax = sqrt((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)) - T0min;
			}
			else//12
			{
				Tmax1 = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)) - T0min;
				Tmax2 = sqrt((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0)) - T0min;
				Tmax = min(Tmax1, Tmax2);
			}
		}
		else if (type == 3)break;
		else if (type == 5)
		{
			pk.x = result.resval.rpoint[X];
			pk.y = result.resval.rpoint[Y];
			pk.z = result.resval.rpoint[Z];

			//			E=pk.distanceTo(interPt[0]);
			double tempr = radiusfree(pl1, pl2, pk);
			if (tempr > 5.0)rad = tempr;

			if (IsStandard)
			{
				//				tempr=get_R(rad,DV);
				if (tempr > 5.0)rad = tempr;
				double l0 = 0.0;//get_l0(rad,DV);
				double BPMTL[10];
				double f1 = fwj(x1, y1, x0, y0);
				double f2 = fwj(x0, y0, x2, y2);
				double aa = AFA(f1, f2);
				if (aa > pi)aa = 2 * pi - aa;
				getBPMTL(dms_rad(aa), rad, l0, l0, &BPMTL[0]);
				T1 = BPMTL[6];
				T2 = BPMTL[7];
				//				double L=BPMTL[8];
				//				acutPrintf(L"rad=%lf,DV=%lf,AFA=%lf,l0=%lf,T1=%lf,T2=%lf,Tmax=%lf\n",rad,DV,aa,l0,T1,T2,Tmax);
				//				acutPrintf(L"最小夹直线长为: %.2lf,L",T0min);

				if (JUDGET012 == 0)
				{
					ISERROR = false;
				}
				else if (JUDGET012 == 12)
				{
					//					acutPrintf(L"当前夹直线长为: L1=%.2lf,L2=%.2lf(右键平移!)\n",Tmax1-T1+T0min,Tmax2-T2+T0min);
					//					acutPrintf(L"Tmax1=%.2lf,Tmax2=%.2lf\n",Tmax1,Tmax2);
					//					acutPrintf(L"x1=%.2lf,y1=%.2lf,x2=%.2lf,y2=%.2lf\n",x1,y1,x2,y2);
					if (T1 > Tmax1 || T2 > Tmax2)ISERROR = true;
					else ISERROR = false;
				}
				else if (JUDGET012 == 1)
				{
					acutPrintf(L"当前夹直线长为: L1=%.2lf(右键平移!)\n", Tmax - T1 + T0min);
					if (T1 > Tmax)ISERROR = true;
					else ISERROR = false;
				}
				else if (JUDGET012 == 2)
				{
					acutPrintf(L"当前夹直线长为: L2=%.2lf(右键平移!)\n", Tmax - T2 + T0min);
					if (T2 > Tmax)ISERROR = true;
					else ISERROR = false;
				}
			}
			else if (INTRadius > 0)rad = (int)(rad / INTRadius)*INTRadius;

			if (fabs(rad - tmpR) > 0.001)
			{
				ACHAR pszLabel[256];
				if (ISERROR)_stprintf(pszLabel, L"当前半径为: %.2lf  不合适!", rad);
				else _stprintf(pszLabel, L"当前半径为: %.2lf", rad);
				acedGrText(-1, pszLabel, 1);
				tmpR = rad;
			}

			center.x = x0 + (rad / cos(pxj / 2))*sin(fwjr);
			center.y = y0 + (rad / cos(pxj / 2))*cos(fwjr);

			dT = rad * tan(0.5*pxj);
			pt1[X] = sPt.x;
			pt1[Y] = sPt.y;
			pt2[X] = ePt.x;
			pt2[Y] = ePt.y;
			angle1 = ads_angle(pt2, pt1);

			pt1[X] = sPt1.x;
			pt1[Y] = sPt1.y;
			pt2[X] = ePt1.x;
			pt2[Y] = ePt1.y;
			angle2 = ads_angle(pt1, pt2);

			ads_point qpt1, qpt2, cen;
			qpt1[X] = x0 + dT * cos(angle1);
			qpt1[Y] = y0 + dT * sin(angle1);

			qpt2[X] = x0 + dT * cos(angle2);
			qpt2[Y] = y0 + dT * sin(angle2);

			cen[X] = center.x;
			cen[Y] = center.y;

			ang1 = ads_angle(cen, qpt1);
			ang2 = ads_angle(cen, qpt2);


			/*acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
			if (acdbOpenObject(pArc, arcId, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			jiao = fwj2 - fwj1;
			if (jiao < 0)jiao += 2 * PI;
			pArc->setCenter(center);
			pArc->setRadius(rad);
			if (jiao > PI) {
				pArc->setStartAngle(ang1);
				pArc->setEndAngle(ang2);
			}
			else {
				pArc->setStartAngle(ang2);
				pArc->setEndAngle(ang1);
			}
			if (ISERROR)
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
			double perX, perY;
			GetPerPoint(center.x, center.y, x1, y1, x0, y0, perX, perY);
			/*acdbOpenObject(perLine3d1,perLId1,AcDb::kForWrite,Adesk::kFalse);*/
			if (acdbOpenObject(perLine3d1, perLId1, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			perLine3d1->setStartPoint(AcGePoint3d(perX, perY, 0.0));
			perLine3d1->setEndPoint(center);
			perLine3d1->close();//切点与圆心连线				

			GetPerPoint(center.x, center.y, x2, y2, x0, y0, perX, perY);
			/*acdbOpenObject(perLine3d2,perLId2,AcDb::kForWrite,Adesk::kFalse);*/
			if (acdbOpenObject(perLine3d2, perLId2, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			perLine3d2->setStartPoint(AcGePoint3d(perX, perY, 0.0));
			perLine3d2->setEndPoint(center);
			perLine3d2->close();//切点与圆心连线		
		}
	}
	if (pArc && !pArc->isErased())
	{
		if (acdbOpenObject(pArc, arcId, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
		{
			pArc->erase();
			pArc->close();
		}
	}

	/*	acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
	pText->erase();
	pText->close();
	*/
	/*acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
	if (acdbOpenObject(pl1, LId1, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	/*acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse);*/
	if (acdbOpenObject(pl2, LId2, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}

	double jiao1, jiao2, dist1, dist2;
	jiao1 = acutAngle(asDblArray(pl1->endPoint()), asDblArray(pl1->startPoint()));
	jiao2 = acutAngle(asDblArray(pl2->startPoint()), asDblArray(pl2->endPoint()));
	dist1 = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	dist2 = sqrt((x2 - x0)*(x2 - x0) + (y2 - y0)*(y2 - y0));
	if (T1 > dist1)
	{
		dist1 = 1.2*T1;
		x1 = x0 + dist1 * cos(jiao1);
		y1 = y0 + dist1 * sin(jiao1);
	}
	if (T2 > dist2)
	{
		dist2 = 1.2*T2;
		x2 = x0 + dist2 * cos(jiao2);
		y2 = y0 + dist2 * sin(jiao2);
	}
	//	acutPrintf(L"jiao1=%lf,jiao2=%lf,x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",jiao1,jiao2,x1,y1,x2,y2);
	if (pl1 && !pl1->isErased())
	{
		pl1->erase();
		pl1->close();
	}
	if (pl2 && !pl2->isErased())
	{
		pl2->erase();
		pl2->close();
	}

	if (acdbOpenObject(perLine3d1, perLId1, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
	{
		perLine3d1->erase();
		perLine3d1->close();//切点与圆心连线				
	}

	if (acdbOpenObject(perLine3d2, perLId2, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
	{
		perLine3d2->erase();
		perLine3d2->close();//切点与圆心连线				
	}

	if (ISERROR)rad = 0;
	//	acutPrintf(L"返回半径:%.2lf\n",rad);
	//	if(ISERROR)return -1.0;
	acedGrText(-5, L"完成", 0);
	acutPrintf(L"\n命令:");
	if (ISOK)return rad;
	else return -1.0;
}

bool BAS_DRAW_FUN::GetCircleFromT_P_R(double x1, double y1, double x2, double y2, double R, double &cenX, double &cenY)
//给定切线，半径，返回圆心
{
	if (R < 0.0001)return false;
	// TODO: Implement the command
	// input R;
	double side1, side2;
	side1 = GetSide(x1, y1, x2, y2, cenX, cenY);
	side1 /= fabs(side1);
	double dx = x1 - x2;
	double dy = y1 - y2;
	x1 += 100.0*dx;
	y1 += 100.0*dy;
	AcDbObjectId eId;
	AcGePoint3d sPt0(x1, y1, 0.0), ePt0(x2, y2, 0.0);
	AcDbLine *pl1 = new AcDbLine(sPt0, ePt0);
	AcGePoint2d sPt1(sPt0.x, sPt0.y), ePt1(ePt0.x, ePt0.y);
	AcGeLine2d pLine1(sPt1, ePt1);
	//constructe the circle.	
	AcDbCircle *pCircle1 = new AcDbCircle();
	AcDbObjectId cirId1 = AddEntityToDbs(pCircle1);
	AcDbCircle *pCircle2 = new AcDbCircle();
	AcDbObjectId cirId2 = AddEntityToDbs(pCircle2);
	/*acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
	if (acdbOpenObject(pCircle1, cirId1, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pCircle1->setColorIndex(1);
	pCircle1->close();
	//acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse);
	if (acdbOpenObject(pCircle2, cirId2, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pCircle2->setColorIndex(1);
	pCircle2->close();
	AcDbLine *perLine3d = new AcDbLine();
	perLine3d->setColorIndex(3);
	AcDbObjectId perLId = AddEntityToDbs(perLine3d);
	// get the point of the circle must be passed.
	AcGePoint2d pk;
	int type;
	int shift = 0;
	struct resbuf result;
	AcGeLine2d perpLine;
	double perX, perY;

	//int shift=0;//{shift=0 two circle;shift =1 one circle;shift=2 the other circle}
	for (;;) {//

		if (ads_grread(1, &type, &result) != RTNORM)
		{
			break;
		}
		if (type == 3)      break;
		if (type == 25)
		{
			if (cirId1 && acdbOpenObject(pCircle1, cirId1, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
			{
				pCircle1->erase();
				pCircle1->close();
			}

			if (cirId2 && acdbOpenObject(pCircle2, cirId2, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
			{
				pCircle2->erase();
				pCircle2->close();
			}

			if (perLId && acdbOpenObject(perLine3d, perLId, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
			{
				perLine3d->erase();
				perLine3d->close();//切点与圆心连线			
			}

			return false;
		}
		if ((type != 5) && (type != 12))
		{
			//ads_printf(L"\n必须用鼠标! ");		
			break;
		}
		if (type == 5)
		{
			pk.x = result.resval.rpoint[X];
			pk.y = result.resval.rpoint[Y];//get AcGePoint2d pk;
			side2 = GetSide(x1, y1, x2, y2, pk.x, pk.y);
			side2 /= fabs(side2);
			//	acutPrintf(L"%lf,%lf\n",side1,side2);
			if (side1*side2 <= 0)continue;
			AcGeLine2d pLine1(sPt1, ePt1);
			pLine1.getPerpLine(pk, perpLine);//做垂线
			AcGePoint2d interPt0;//垂足在已知线上
			AcGeTol tol;
			pLine1.intersectWith(perpLine, interPt0, tol); //求垂足
			double dist0p = interPt0.distanceTo(pk);//垂足到拾取点之距
			if (dist0p <= 2 * R&&dist0p > 0)
			{
				AcGeVector2d translateVec(R*(pk.x - interPt0.x) / dist0p, R*(pk.y - interPt0.y) / dist0p);
				AcGeMatrix2d matrix = matrix.translation(translateVec);
				pLine1.transformBy(matrix);			//得pLine1的平行线，它们之距为R
				AcGeCircArc2d pCirclefz(pk, R);
				int intn;
				AcGePoint2d p1, p2;
				pCirclefz.intersectWith(pLine1, intn, p1, p2, tol);//辅助圆与平行线的交点。	
				AcGePoint3d p31(p1.x, p1.y, 0), p32(p2.x, p2.y, 0);
				double distcen1 = p1.distanceTo(sPt1);//from the start point of line to center1
				double distcen2 = p2.distanceTo(sPt1);//from the start point of line to center2
				if (distcen1 < distcen2)
				{
					if (cirId1 && acdbOpenObject(pCircle1, cirId1, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
					{
						pCircle1->setCenter(p31);
						pCircle1->setRadius(R);
						pCircle1->close();
					}

					if (cirId2 && acdbOpenObject(pCircle2, cirId2, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
					{
						pCircle2->setCenter(p31);
						pCircle2->setRadius(0);
						pCircle2->close();

					}
					cenX = p31.x;
					cenY = p31.y;
				}
				else
				{
					if (cirId1 && acdbOpenObject(pCircle1, cirId1, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
					{
						pCircle1->setCenter(p32);
						pCircle1->setRadius(0);
						pCircle1->close();
					}

					if (cirId2 && acdbOpenObject(pCircle2, cirId2, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
					{
						pCircle2->setCenter(p32);
						pCircle2->setRadius(R);
						pCircle2->close();

					}
					cenX = p32.x;
					cenY = p32.y;
				}
				GetPerPoint(cenX, cenY, x1, y1, x2, y2, perX, perY);
				if (perLId && acdbOpenObject(perLine3d, perLId, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
				{
					perLine3d->setStartPoint(AcGePoint3d(perX, perY, 0.0));
					perLine3d->setEndPoint(AcGePoint3d(cenX, cenY, 0.0));
					perLine3d->close();//切点与圆心连线				
				}

			}
		}
	}//for(;;)
	if (cirId1 && acdbOpenObject(pCircle1, cirId1, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
	{
		pCircle1->erase();
		pCircle1->close();
	}

	if (cirId2 && acdbOpenObject(pCircle2, cirId2, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
	{
		pCircle2->erase();
		pCircle2->close();
	}

	if (perLId && acdbOpenObject(perLine3d, perLId, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
	{
		perLine3d->erase();
		perLine3d->close();//切点与圆心连线	
	}

	return true;
}

void BAS_DRAW_FUN::GetPerPoint(double outX, double outY, double x1, double y1, double x2, double y2, double &perX, double &perY)
//线外一点作垂线,1:线外一点  2:线起点 3:线终点 4:返回垂足
{
	AcDbLine *Line = new AcDbLine(AcGePoint3d(x1, y1, 0.0), AcGePoint3d(x2, y2, 0.0));
	double fwj1 = fwj(x1, y1, x2, y2);
	fwj1 += 0.5*pi;
	AcDbLine *perpLine = new AcDbLine(AcGePoint3d(outX, outY, 0.0), AcGePoint3d(outX + 10 * sin(fwj1), outY + 10 * cos(fwj1), 0.0));
	AcGePoint3dArray interPt;
	Line->intersectWith(perpLine, AcDb::kExtendBoth, interPt, 0, 0);
	perX = interPt[0].x;
	perY = interPt[0].y;
}

//plh07.19
double BAS_DRAW_FUN::GetRFromT_P_P(double x1, double y1, double x2, double y2,
	double &xs, double &ys, double &x0, double &y0,
	double &xe, double &ye, double &cenX, double &cenY,
	AcDbObjectIdArray &eraseId, bool IsStandard, double DV)
	//传入切线方向，返回半径,前一个切点，交点，后一个切点
{
	// TODO: Implement the command
	double jiao12;
	double dist = xyddaa(x1, y1, x2, y2, &jiao12);
	dist *= 1000;
	x2 += dist * cos(jiao12);
	y2 += dist * sin(jiao12);
	x1 -= dist * cos(jiao12);
	y1 -= dist * sin(jiao12);
	double rad = -1.0;//半径
	double pxj, fwj1, fwj2, fwjr, fwjt, jiao, afa;
	AcGePoint3d center;

	AcGePoint3d sPt1(x1, y1, 0.0);//切线的起点
	AcGePoint3d ePt1(x2, y2, 0.0);//切线的终点
	AcDbLine *pl1 = new AcDbLine(sPt1, ePt1);

	double side1, side2;
	double tmpR;
	//select point.
	ads_point pk00;
	//	AfxMessageBox(L"1");
	if (RTNORM != acedGetPoint(NULL, L"请选择圆弧上第一个点:\n", pk00))return -1.0;
	//	AfxMessageBox(L"2");
	AcGePoint3d Pk1(pk00[X], pk00[Y], pk00[Z]);
	ads_point pt1, pt2;
	AcGePoint3d pk;
	int type;
	struct resbuf result;
	double dT;
	double angle1;
	double ang1, ang2;
	//	AcDbArc *pArc= new AcDbArc();//弧
	AcDbCircle *pArc = new AcDbCircle();//圆
	AcDbObjectId arcId = AddEntityToDbs(pArc);
	/*	ACHAR Rchr[80];
	double texth=8;
	AcDbText *pText=new AcDbText(pk,Rchr);
	pText->setColorIndex(3);
	pText->setHeight(texth);
	AcDbObjectId textId=AddEntityToDbs(pText);
	*/
	bool ISOK = true;
	for (;;) {

		if (ads_grread(1, &type, &result) != RTNORM)
		{
			ISOK = false;
			break;
		}
		if (type == 3)break;
		else if (type == 25)
		{
			if (rad < 0.00001)continue;
			double cenX, cenY, dx, dy;
			cenX = center.x;//plh06.03
			cenY = center.y;
			if (GetCircleFromT_P_R(x1, y1, x2, y2, rad, cenX, cenY) != true)continue;
			dx = cenX - center.x;
			dy = cenY - center.y;
			//acutPrintf(L"dx=%lf,dy=%lf\n",dx,dy);
			center.x += dx;
			center.y += dy;
			Pk1.x += dx;
			Pk1.y += dy;
		}
		else if (type == 5)
		{
			pk.x = result.resval.rpoint[X];
			pk.y = result.resval.rpoint[Y];
			pk.z = result.resval.rpoint[Z];


			side1 = GetSide(x1, y1, x2, y2, pk.x, pk.y);
			side2 = GetSide(x1, y1, x2, y2, Pk1.x, Pk1.y);
			side1 /= fabs(side1);
			side2 /= fabs(side2);

			if (side1 == 0 || side2 == 0)continue;//plh05.27??????????????????
			else
			{
				if (side1*side2 <= 0)continue;
			}
			AcGePoint3d Pk2(pk.x, pk.y, pk.z);
			if (Pk1.distanceTo(Pk2) != 0)
			{
				AcDbLine *pLinet = new AcDbLine(Pk1, Pk2);

				AcGePoint3dArray interPt;
				pLinet->intersectWith(pl1, AcDb::kExtendBoth, interPt, 0, 0);
				delete pLinet;///
				AcGePoint3d Pt1(interPt[0].x, interPt[0].y, interPt[0].z);
				AcGePoint3d Pt0((Pk1.x + Pk2.x) / 2, (Pk1.y + Pk2.y) / 2, (Pk1.z + Pk2.z) / 2);
				AcGePoint3d Pt2((2 * Pt0.x - Pt1.x), (2 * Pt0.y - Pt1.y), (2 * Pt0.z - Pt1.z));
				fwjt = fwj(Pk1.x, Pk1.y, Pk2.x, Pk2.y);
				AcGePoint3d Ptx((Pt0.x + 100 * sin(fwjt + PI / 2)), (Pt0.y + 100 * cos(fwjt + PI / 2)), Pt0.z);

				AcDbLine *pLine3 = new AcDbLine(Pt0, Ptx);
				//				if(fwjr!=fwj1&&fwj1+fwjr!=PI){ 

				AcGePoint3dArray interPtd;
				pLine3->intersectWith(pl1, AcDb::kExtendBoth, interPtd, 0, 0);
				delete pLine3;
				AcDbLine *pl2 = new AcDbLine(interPtd[0], Pt2);
				AcDbLine *pl11 = new AcDbLine(Pt1, interPtd[0]);

				fwj1 = fwj(Pt1.x, Pt1.y, interPtd[0].x, interPtd[0].y);
				fwj2 = fwj(interPtd[0].x, interPtd[0].y, Pt2.x, Pt2.y);
				pxj = PXJ(fwj1, fwj2);
				afa = AFA(fwj1, fwj2);
				fwjr = (PI - afa) / 2 + fwj2;
				double temprad = radiusfree(pl11, pl2, Pk2);
				//				if(temprad<0.00001||temprad>1.2*get_R(temprad,DV))continue;
				rad = temprad;
				//				if(IsStandard)rad = get_R(rad,DV);

				if (fabs(rad - tmpR) > 0.001)
				{
					ACHAR pszLabel[256];
					_stprintf(pszLabel, L"当前半径为: %.2lf", rad);
					acedGrText(-1, pszLabel, 1);
					tmpR = rad;
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

				center.x = interPtd[0].x + (rad / cos(pxj / 2))*sin(fwjr);
				center.y = interPtd[0].y + (rad / cos(pxj / 2))*cos(fwjr);

				dT = rad * tan(0.5*pxj);
				pt1[X] = Pt1.x;
				pt1[Y] = Pt1.y;
				pt2[X] = interPtd[0].x;
				pt2[Y] = interPtd[0].y;
				angle1 = ads_angle(pt2, pt1);

				ads_point qpt1, qpt2, cen;
				qpt1[X] = interPtd[0].x + dT * cos(angle1);
				qpt1[Y] = interPtd[0].y + dT * sin(angle1);
				qpt2[X] = Pk2.x;
				qpt2[Y] = Pk2.y;
				cen[X] = center.x;
				cen[Y] = center.y;

				ang1 = ads_angle(cen, qpt1);
				ang2 = ads_angle(cen, qpt2);

				/*acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
				if (acdbOpenObject(pArc, arcId, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return 0.0;
				}
				jiao = fwj2 - fwj1;
				if (jiao < 0)jiao += 2 * PI;
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
	cenX = center.x;
	cenY = center.y;
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
	pl1 = NULL;
	//	acutPrintf(L"rad=%lf\n",rad);
	acedGrText(-5, L"完成", 0);
	acutPrintf(L"\n命令:");

	if (ISOK)return rad;
	else return -1.0;
}

bool BAS_DRAW_FUN::GetTangent(double CenX, double CenY, double R, double &OutX, double &OutY, double &TX, double &TY)
//传入圆心，半径，一条切线的，起点,终点//终点为切点
//返回 另一第切线的起点，终点//终点为切点
{
	AcGePoint2d pk;//必经点
	int type;
	struct resbuf result;

	AcDbLine *Line1 = new AcDbLine();
	AcDbObjectId LId1 = AddEntityToDbs(Line1);
	AcGePoint3d Pt1, Pt2;
	double fwj, OC, bta, side;
	double x1, y1, x2, y2;//线中起终点
	x1 = OutX;
	y1 = OutY;
	x2 = TX;
	y2 = TY;
	for (;;)
	{
		if (ads_grread(1, &type, &result) != RTNORM)
		{
			break;
		}
		if (type == 3)      break;
		if ((type != 5) && (type != 12))
		{
			ads_printf(L"必须用鼠标!\n");
			break;
		}
		if (type == 5)
		{
			OutX = result.resval.rpoint[X];
			OutY = result.resval.rpoint[Y];
			OC = xyddaa(CenX, CenY, OutX, OutY, &fwj);
			if (OC > R)
			{
				if (R / OC > 1) bta = 0;
				else if (R / OC < -1)bta = pi;
				else bta = acos(R / OC);

				side = GetSide(x1, y1, x2, y2, OutX, OutY);//判断左右侧左-右+///plh05.27??????????

				if (side > 0.0)//>0,圆心在线路左侧，直线与圆相切点在鼠标点与圆心连线的右边
				{
					TX = CenX + R * cos(fwj + bta);//X
					TY = CenY + R * sin(fwj + bta);
				}
				else
				{
					TX = CenX + R * cos(fwj - bta);//X
					TY = CenY + R * sin(fwj - bta);
				}
				/*acdbOpenObject(Line1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
				if (acdbOpenObject(Line1, LId1, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return false;
				}
				Line1->setStartPoint(AcGePoint3d(TX, TY, 0.0));
				Line1->setEndPoint(AcGePoint3d(OutX, OutY, 0.0));
				Line1->close();
			}
		}
	}
	if (LId1 && acdbOpenObject(Line1, LId1, AcDb::kForWrite, Adesk::kFalse) == Acad::eOk)
	{
		Line1->erase();
		Line1->close();
	}

	return true;
}



void BAS_DRAW_FUN::getLineEndPt(AcGePoint3d sPt, double len, double fwj, AcGePoint3d &ePt)
{
	ePt.z = 0;
	ePt.x = sPt.x + len * sin(fwj);
	ePt.y = sPt.y + len * cos(fwj);
}

void BAS_DRAW_FUN::GetT0min_Lrmin(double DV, double &T0min, double &Lrmin)
//传入设计车速
{
	if (DV <= 0) { T0min = -1000.0; Lrmin = -1000.0; }
	else
	{
		if (DV >= 350)T0min = Lrmin = 280;
		else if (DV >= 300)T0min = Lrmin = 280;
		else if (DV >= 250)T0min = Lrmin = 180;
		else if (DV >= 200)T0min = Lrmin = 140;
		else if (DV >= 140)T0min = Lrmin = 110;
		else if (DV >= 120)T0min = Lrmin = 80;
		else if (DV >= 100)T0min = Lrmin = 60;
		else if (DV >= 80)T0min = Lrmin = 50;
		else if (DV >= 70) { T0min = 50; Lrmin = 20.0; }
		else if (DV >= 55) { T0min = 45; Lrmin = 20.0; }
		else if (DV >= 40) { T0min = 40; Lrmin = 20.0; }
		else { T0min = -1000.0; Lrmin = -1000.0; }
	}
}

//计算不同车速下最小圆曲线半径	20190724
double BAS_DRAW_FUN::Cal_Road_MinR(double DesignSpeed)
{
	if (DesignSpeed > 119.0)
		return 1000.0;
	else if (DesignSpeed > 99.0)
		return 700.0;
	else if (DesignSpeed > 79.0)
		return 400.0;
	else if (DesignSpeed > 59.0)
		return 200.0;
	else if (DesignSpeed > 39.0)
		return 100.0;
	else if (DesignSpeed > 29.0)
		return 65.0;
	else
		return 30.0;
}


double BAS_DRAW_FUN::DistToLine(double inX1, double inY1, double inX2, double inY2, double &outX,
	double &outY, double &distTo2PointOnLine)
{
	AcGePoint2d inSPt, inEPt;//线段的起终点
	AcGePoint2d outPt;//线外一点
	AcGePoint2d perpPt;//垂足

	inSPt.set(inX1, inY1);
	inEPt.set(inX2, inY2);
	outPt.set(outX, outY);

	AcGeLine2d inLine(inSPt, inEPt);//线段
	AcGeLine2d perpLine;//过线外一点作线段的垂线	
	inLine.getPerpLine(outPt, perpLine);//作垂线
	inLine.intersectWith(perpLine, perpPt);//求垂足

	//返回线上投影点
	outX = perpPt.x;
	outY = perpPt.y;

	//线外点到线段端点的投影距离的最小值
	distTo2PointOnLine = 0.5*(perpPt.distanceTo(inSPt) + perpPt.distanceTo(inEPt)
		- inSPt.distanceTo(inEPt));
	return outPt.distanceTo(perpPt);
}

double BAS_DRAW_FUN::LineNearestPoint(AcGePoint3d Pt, AcDbLine *Line, AcGePoint3d &NearestPt)
//找距直线上最近的点，返回距离。　
{
	AcGePoint2d spt(Line->startPoint().x, Line->startPoint().y), ept(Line->endPoint().x, Line->endPoint().y), Pt0(Pt.x, Pt.y), perPt;
	AcGeLine2d pLine(spt, ept);
	AcGeLine2d perpLine;
	pLine.getPerpLine(Pt0, perpLine);//做垂线
	//垂足在已知线上
	AcGeTol tol;
	pLine.intersectWith(perpLine, perPt, tol); //求垂足
	NearestPt.x = perPt.x;
	NearestPt.y = perPt.y;
	NearestPt.z = 0;
	return NearestPt.distanceTo(Pt);
}

double BAS_DRAW_FUN::CircleNearestPoint(AcGePoint3d Pt, AcDbCircle *Circle, AcGePoint3d &NearestPt)
//找距圆上最近的点，返回距离。　
{
	AcGePoint3d Cen = Circle->center(), Pt1, Pt0;
	Cen.z = 0;
	Pt.z = 0;
	AcDbLine pLine(Pt, Cen);
	AcGePoint3dArray points;
	pLine.intersectWith(Circle, AcDb::kExtendThis, points);//点与圆心连线与圆的交点
	if (Circle->radius() < 0.000001&&points.length() < 1)
	{
		NearestPt = Circle->center();
		return NearestPt.distanceTo(Pt);
	}
	//	ASSERT(points.length()>1);
	//	acedAlert(L"asser");
	Pt0.x = points[0].x;	Pt0.y = points[0].y;	Pt0.z = 0;
	Pt1.x = points[1].x;	Pt1.y = points[1].y;	Pt1.z = 0;
	/*
	acutPrintf(L"x0=%lf,y0=%lf\n",points[0].x,points[0].y);
	acutPrintf(L"x1=%lf,y1=%lf\n",points[1].x,points[1].y);
	*/
	double d0, d1;
	d0 = Pt0.distanceTo(Pt); d1 = Pt1.distanceTo(Pt);
	if (d0 < d1)
	{
		NearestPt = Pt0;
		return d0;
	}
	else
	{
		NearestPt = Pt1;
		return d1;
	}
	/*
	double x0=Circle->center().x,y0=Circle->center().y;
	double x1=Pt.x,y1=Pt.y;
	double R=Circle->radius();
	double dist=sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
	NearestPt.x=x0+(x1-x0)*R/dist;
	NearestPt.y=y0+(y1-y0)*R/dist;
	NearestPt.z=0;
	return NearestPt.distanceTo(Pt);
	*/
}

double BAS_DRAW_FUN::ArcNearestPoint(AcGePoint3d Pt, AcDbArc *Arc, AcGePoint3d &NearestPt)
//找距圆上最近的点，返回距离。　
{
	AcDbCircle *Cir = new AcDbCircle(Arc->center(), AcGeVector3d(0, 0, 1), Arc->radius());
	return CircleNearestPoint(Pt, Cir, NearestPt);
}

double BAS_DRAW_FUN::fwj(AcDbLine *pL)//与Y轴夹角
{
	double ang = angleX(pL);//与X轴夹角
	ang = 2.5*PI - ang;
	if (ang > 2 * PI)ang -= 2 * PI;
	return ang;
}

double BAS_DRAW_FUN::fwj(AcGePoint3d sPt, AcGePoint3d ePt)//与Y轴夹角
{
	double ang = angleX(sPt, ePt);//与X轴夹角
	ang = 2.5*PI - ang;
	if (ang > 2 * PI)ang -= 2 * PI;
	return ang;
}


double BAS_DRAW_FUN::FwjtoAngX(double ang)
{
	ang = 2.5*PI - ang;
	if (ang > 2 * PI)ang -= 2 * PI;
	return ang;
}

double BAS_DRAW_FUN::distfwj(double xb, double yb, double xe, double ye, double *f)//与Y轴夹角f,返回两点之距
{
	AcGePoint3d sPt(xb, yb, 0), ePt(xe, ye, 0);
	AcDbLine *pL = new AcDbLine(sPt, ePt);
	double ang = angleX(pL);//与X轴夹角
	ang = 2.5*PI - ang;
	if (ang > 2 * PI)ang -= 2 * PI;
	*f = ang;
	return sqrt((xb - xe)*(xb - xe) + (yb - ye)*(yb - ye));
}

double BAS_DRAW_FUN::angleX(AcDbLine *pL)
{
	AcGePoint3d sPtL(pL->startPoint()), ePtL(pL->endPoint());
	ads_point spt, ept;
	spt[X] = sPtL.x;
	spt[Y] = sPtL.y;
	ept[X] = ePtL.x;
	ept[Y] = ePtL.y;
	double ang = ads_angle(spt, ept);//与X轴夹角
	return ang;
}

double BAS_DRAW_FUN::angleX(AcGePoint3d sPtL, AcGePoint3d ePtL)//Line(sPtl,ePtl)
{
	ads_point spt, ept;
	spt[X] = sPtL.x;
	spt[Y] = sPtL.y;
	ept[X] = ePtL.x;
	ept[Y] = ePtL.y;
	double ang = ads_angle(spt, ept);//与X轴夹角
	return ang;
}


double BAS_DRAW_FUN::distptol(AcGePoint3d pt, AcDbLine *Line)
{
	AcGePoint3d sPt = Line->startPoint();
	AcGePoint3d ePt = Line->endPoint();
	AcGePoint2d pk(pt.x, pt.y), sPt2d(sPt.x, sPt.y), ePt2d(ePt.x, ePt.y);
	AcGeLine2d pL(sPt2d, ePt2d);
	AcGeLine2d perpLine;
	pL.getPerpLine(pk, perpLine);//做垂线
	AcGePoint2d interPt0;//垂足在已知线上
	AcGeTol tol;
	pL.intersectWith(perpLine, interPt0, tol); //求垂足
	double dist0p = interPt0.distanceTo(pk);//垂足到拾取点之距
	return dist0p;
}

AcDbArc* BAS_DRAW_FUN::getArc(double x1, double y1, double x0, double y0, double x2, double y2)//平面//返回半径，INPUT：圆弧上三点
{
	AcGePoint3d ptb(x1, y1, 0);
	AcGePoint3d pt(x0, y0, 0);
	AcGePoint3d pte(x2, y2, 0);
	AcDbArc* pArc = getArcFrom3Pt(ptb, pt, pte);//平面
	return pArc;
}

double BAS_DRAW_FUN::getArc3Pt(AcDbArc *pArc, AcGePoint3d &ptb, AcGePoint3d &pt, AcGePoint3d &pte)//平面
{
	double jiao1 = pArc->startAngle(), jiao3 = pArc->endAngle(), jiao2 = 0, R = pArc->radius();
	jiao2 = 0.5*(jiao1 + jiao3);
	AcGePoint3d Cen = pArc->center();
	ptb.x = Cen.x + R * cos(jiao1); ptb.y = Cen.y + R * sin(jiao1); ptb.z = 0;
	pt.x = Cen.x + R * cos(jiao2); pt.y = Cen.y + R * sin(jiao2); pt.z = 0;
	pte.x = Cen.x + R * cos(jiao3); pte.y = Cen.y + R * sin(jiao3); pte.z = 0;
	return R;
}

double BAS_DRAW_FUN::getArc3Pt(AcDbArc *pArc, double &x1, double &y1, double &x0, double &y0, double &x2, double &y2)//平面
{
	double jiao1 = pArc->startAngle(), jiao3 = pArc->endAngle(), jiao2 = 0, R = pArc->radius();
	jiao2 = 0.5*(jiao1 + jiao3);
	AcGePoint3d Cen = pArc->center();
	x1 = Cen.x + R * cos(jiao1); y1 = Cen.y + R * sin(jiao1);
	x0 = Cen.x + R * cos(jiao2); y0 = Cen.y + R * sin(jiao2);
	x2 = Cen.x + R * cos(jiao3); y2 = Cen.y + R * sin(jiao3);
	return R;
}

AcDbArc* BAS_DRAW_FUN::getArcFrom3Pt(AcGePoint3d ptb, AcGePoint3d pt, AcGePoint3d pte)//平面
{
	AcDbLine *pL1 = new AcDbLine(ptb, pt);
	AcDbLine *pL2 = new AcDbLine(pte, pt);
	double fwj1 = angleX(pL1), fwj2 = angleX(pL2);
	delete pL1; delete pL2;
	double x1 = 0.5*(ptb.x + pt.x);
	double y1 = 0.5*(ptb.y + pt.y);
	double x2 = 0.5*(pt.x + pte.x);
	double y2 = 0.5*(pt.y + pte.y);
	AcGePoint3d p1(x1, y1, 0), p2(x1 + 100 * cos(fwj1 + 0.5*PI), y1 + 100 * sin(fwj1 + 0.5*PI), 0);
	AcGePoint3d p3(x2, y2, 0), p4(x2 + 100 * cos(fwj2 + 0.5*PI), y2 + 100 * sin(fwj2 + 0.5*PI), 0);
	AcGeLine3d L1(p1, p2);
	AcGeLine3d L2(p3, p4);
	AcGePoint3d cen;
	L1.intersectWith(L2, cen);
	double R = cen.distanceTo(pt);
	AcDbLine *L3 = new AcDbLine(cen, ptb);
	AcDbLine *L4 = new AcDbLine(cen, pte);
	double startAng = angleX(L3);
	double endAng = angleX(L4);
	delete L3; delete L4;
	int lrflag = getLRRot(ptb, pt, pte);
	if (lrflag > 0)
	{
		double temp;
		temp = startAng;
		startAng = endAng;
		endAng = temp;
	}	AcDbArc *Arc = new AcDbArc(cen, R, startAng, endAng);
	return Arc;
}

int BAS_DRAW_FUN::getLRRot(AcGePoint3d ptb, AcGePoint3d pt, AcGePoint3d pte)
//left -1;	right +1;
{
	int lrflag = 0;
	double jiao1 = angleX(ptb, pt);
	double jiao2 = angleX(pt, pte);
	double jiao = jiao1 - jiao2;
	if (jiao < 0)jiao += 2 * PI;
	if (jiao < PI)lrflag = 1;
	else lrflag = -1;
	return lrflag;
}

double BAS_DRAW_FUN::getArcFrom3Pt(double x1, double y1, double x0, double y0, double x2, double y2)//平面//返回半径，INPUT：圆弧上三点
{
	AcGePoint3d ptb(x1, y1, 0);
	AcGePoint3d pt(x0, y0, 0);
	AcGePoint3d pte(x2, y2, 0);
	AcDbArc* pArc = getArcFrom3Pt(ptb, pt, pte);//平面
	return pArc->radius();
}

void BAS_DRAW_FUN::getArcCenFrom3Pt(double x1, double y1, double x0, double y0, double x2, double y2, AcGePoint3d &cen)//平面//返回半径，INPUT：圆弧上三点
{
	AcGePoint3d ptb(x1, y1, 0);
	AcGePoint3d pt(x0, y0, 0);
	AcGePoint3d pte(x2, y2, 0);
	AcDbArc* pArc = getArcFrom3Pt(ptb, pt, pte);//平面
	cen = pArc->center();
}


double BAS_DRAW_FUN::GetArcLengthBy2PointOnCircleAndLR(double CenN, double CenE, double R,
	double N1, double E1, double N2, double E2, int lr)
{
	double j1, j2, dj;
	xyddaa(CenN, CenE, N1, E1, &j1);
	xyddaa(CenN, CenE, N2, E2, &j2);


	if (lr < 0.0)//left
	{
		dj = j1 - j2;
	}
	else//right
	{
		dj = j2 - j1;
	}
	if (dj < 0.0)dj += 2.0*PI;

	//	dj = ZXJ(j2,j1);
	//	if(lr*dj<0.0)
	//		dj =2*pi-dj;
	return dj * R;
}

double BAS_DRAW_FUN::func(double x)
{
	return 1;
}

double BAS_DRAW_FUN::golden(double ax, double bx, double cx, double tol, double& xmin)
{
	double x0, x1, x2, x3, f0, f1, f2, f3, r = 0.61803399;
	double c = 0.38196601;
	x0 = ax;
	x3 = cx;
	if (fabs(cx - bx) > fabs(bx - ax))
	{
		x1 = bx;
		x2 = bx + c * (cx - bx);
	}
	else
	{
		x2 = bx;
		x1 = bx - c * (bx - ax);
	}
	f1 = func(x1);
	f2 = func(x2);
	while (fabs(x3 - x0) > tol * (fabs(x1) + fabs(x2)))
	{
		if (f2 < f1)
		{
			x0 = x1;
			x1 = x2;
			x2 = r * x1 + c * x3;
			f0 = f1;
			f1 = f2;
			f2 = func(x2);
		}
		else
		{
			x3 = x2;
			x2 = x1;
			x1 = r * x2 + c * x0;
			f3 = f2;
			f2 = f1;
			f1 = func(x1);
		}
	}
	if (f1 < f2)
	{
		xmin = x1;
		return f1;
	}
	else
	{
		xmin = x2;
		return f2;
	}
}

void BAS_DRAW_FUN::mnbrak(double& ax, double& bx, double& cx, double& fa, double& fb, double& fc)
{
	double r, q, dum, gold = 1.618034;
	int glimit = 100;
	double u, ulim, fu, tiny = 1e-20;
	fa = func(ax);
	fb = func(bx);
	if (fb > fa)
	{
		dum = ax;
		ax = bx;
		bx = dum;
		dum = fb;
		fb = fa;
		fa = dum;
	}
	cx = bx + gold * (bx - ax);
	fc = func(cx);
	while (fb >= fc)
	{
		r = (bx - ax) * (fb - fc);
		q = (bx - cx) * (fb - fa);
		dum = q - r;
		if (fabs(dum) < tiny)
		{
			dum = tiny;
		}
		u = bx - ((bx - cx) * q - (bx - ax) * r) / (2 * dum);
		ulim = bx + glimit * (cx - bx);
		if ((bx - u) * (u - cx) > 0)
		{
			fu = func(u);
			if (fu < fc)
			{
				ax = bx;
				fa = fb;
				bx = u;
				fb = fu;
				return;
			}
			else
			{
				if (fu > fb)
				{
					cx = u;
					fc = fu;
					return;
				}
			}
			u = cx + gold * (cx - bx);
			fu = func(u);
		}
		else
		{
			if ((cx - u) * (u - ulim) > 0)
			{
				fu = func(u);
				if (fu < fc)
				{
					bx = cx;
					cx = u;
					u = cx + gold * (cx - bx);
					fb = fc;
					fc = fu;
					fu = func(u);
				}
			}
			else
			{
				if ((u - ulim) * (ulim - cx) >= 0)
				{
					u = ulim;
					fu = func(u);
				}
				else
				{
					u = cx + gold * (cx - bx);
					fu = func(u);
				}
			}
		}
		ax = bx;
		bx = cx;
		cx = u;
		fa = fb;
		fb = fc;
		fc = fu;
		//	acutPrintf(L"1:ax=%.2lf,bx=%.2lf,cx=%.2lf\n",ax,bx,cx);
	}
	//	acutPrintf(L"2:ax=%.2lf,bx=%.2lf,cx=%.2lf\n",ax,bx,cx);
}

void BAS_DRAW_FUN::RToS(double lc, int NLC, ACHAR ch[])
{
	ACHAR tmp[80];

	ads_rtos(lc, 2, 3, tmp);
	//    ads_printf(L"%lf %s\n",lc,tmp);
	if (lc < 10)
	{
		_stprintf(ch, L"00%s", tmp);
		/*
		if(NLC==0)_stprintf(ch,L"00%.0lf",lc);
		else if(NLC==1)_stprintf(ch,L"00%.1lf",lc);
		else if(NLC==2)_stprintf(ch,L"00%.2lf",lc);
		else if(NLC==3)_stprintf(ch,L"00%.3lf",lc);
		else if(NLC==4)_stprintf(ch,L"00%.4lf",lc);
		else if(NLC==5)_stprintf(ch,L"00%.5lf",lc);
		else if(NLC==6)_stprintf(ch,L"00%.6lf",lc);
		else _stprintf(ch,L"00%lf",lc);*/

	}
	else if (lc < 100)
	{
		_stprintf(ch, L"0%s", tmp);
		/*
		if(NLC==0)_stprintf(ch,L"0%.0lf",lc);
		else if(NLC==1)_stprintf(ch,L"0%.1lf",lc);
		else if(NLC==2)_stprintf(ch,L"0%.2lf",lc);
		else if(NLC==3)_stprintf(ch,L"0%.3lf",lc);
		else if(NLC==4)_stprintf(ch,L"0%.4lf",lc);
		else if(NLC==5)_stprintf(ch,L"0%.5lf",lc);
		else if(NLC==6)_stprintf(ch,L"0%.6lf",lc);
		else _stprintf(ch,L"0%lf",lc);*/

	}
	else
	{
		_stprintf(ch, L"%s", tmp);
		/*
		if(NLC==0)_stprintf(ch,L"%.0lf",lc);
		else if(NLC==1)_stprintf(ch,L"%.1lf",lc);
		else if(NLC==2)_stprintf(ch,L"%.2lf",lc);
		else if(NLC==3)_stprintf(ch,L"%.3lf",lc);
		else if(NLC==4)_stprintf(ch,L"%.4lf",lc);
		else if(NLC==5)_stprintf(ch,L"%.5lf",lc);
		else if(NLC==6)_stprintf(ch,L"%.6lf",lc);
		else _stprintf(ch,L"%lf",lc);*/
	}
}

AcDbObjectId   BAS_DRAW_FUN::makeline(AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor, double lw, ACHAR *LayerName, ACHAR *LineType)
{
	AcGePoint2dArray PtAy;
	PtAy.append(AcGePoint2d(startPt.x, startPt.y));
	PtAy.append(AcGePoint2d(endPt.x, endPt.y));
	return makepolyline(PtAy, icolor, lw, LayerName, LineType);
}

AcDbObjectId BAS_DRAW_FUN::makepolyline(AcGePoint2dArray PtAy, int icolor, AcDb::LineWeight lw, ACHAR *LayerName, ACHAR *LineType)
{
	int NUM = PtAy.length();
	AcDbPolyline *polyline = new AcDbPolyline(NUM);
	for (int i = 0; i < NUM; i++)
	{
		polyline->addVertexAt(i, PtAy[i]);
	}
	if (icolor >= 0)polyline->setColorIndex(icolor);
	polyline->setLineWeight(lw);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	polyline->setLayer(LayerTabRecordId);
	pLayerTable->close();
	if (LineType != NULL)
	{
		acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType, L"acadiso.lin");//
		polyline->setLinetype(LineType);
	}
	AcDbObjectId  LId;
	LId = AddEntityToDbs(polyline);
	return LId;
}
AcDbObjectId BAS_DRAW_FUN::makepolyline(AcGePoint2dArray PtAy, int icolor, double lw, ACHAR *LayerName, ACHAR *LineType, double LTScale)
{
	int NUM = PtAy.length();
	AcDbPolyline *polyline = new AcDbPolyline(NUM);
	for (int i = 0; i < NUM; i++)
	{
		polyline->addVertexAt(i, PtAy[i]);
		polyline->setWidthsAt(i, lw, lw);
	}
	if (icolor >= 0)polyline->setColorIndex(icolor);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);

		//acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	polyline->setLayer(LayerTabRecordId);
	pLayerTable->close();
	if (LineType != NULL)
	{
		if (Acad::eUndefinedLineType == acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType, L"acadiso.lin"))
		{
			ACHAR lineFname[256];
			_stprintf(lineFname, L"%s\\lib\\rail.lin", SysPath);
			acdbHostApplicationServices()->workingDatabase()->
				loadLineTypeFile(LineType, lineFname);//
		}

		polyline->setLinetype(LineType);
	}
	if (LTScale > 0.0)
		polyline->setLinetypeScale(LTScale);
	AcDbObjectId  LId;
	LId = AddEntityToDbs(polyline);
	return LId;
}

AcDbObjectId BAS_DRAW_FUN::CreateLayer(ACHAR *LayerName, ACHAR *LineType, AcCmColor color, AcDb::LineWeight weight)
{
	AcDbLayerTable *pLayerTable;
	//	AcDbObjectId LayerTabRecordId;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForRead);
	AcDbObjectId LayerTabRecordId;
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) == Acad::eOk)
	{
		pLayerTable->close();
		return LayerTabRecordId;
	}
	pLayerTable->close();

	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;
	pLayerTableRecord->setName(LayerName);
	pLayerTableRecord->setLineWeight(weight);
	AcDbLinetypeTable *pLinetypeTble;
	acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTble, AcDb::kForRead);
	AcDbObjectId ltypeObjId;
	pLinetypeTble->getAt(LineType, ltypeObjId, Adesk::kTrue);

	pLayerTableRecord->setColor(color);
	pLayerTableRecord->setLinetypeObjectId(ltypeObjId);
	pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	pLinetypeTble->close();

	acDocManager->unlockDocument(acDocManager->curDocument());

	return LayerTabRecordId;
}

Adesk::Boolean  BAS_DRAW_FUN::G_drawspi(AcGiWorldDraw* pWd, double INDATA[5], double LO, xlpoint *P0, xlpoint *PZ, double ND, int icolor)
{
	int k, nk;
	double dlc;

	nk = (int)(LO / ND) + 1;
	AcGePoint3d* pArr = new AcGePoint3d[nk + 1];
	pArr[0].set(P0->y, P0->x, 0.0);
	for (k = 1; k <= nk; k++)
	{
		dlc = k * ND*1.0;
		if (dlc > LO) dlc = LO;
		xlzbxy(P0, PZ, dlc, INDATA, 0);
		// ads_printf(L"\n % %d %5.1f %10.3f %10.3f %10.4f",k,dlc,PZ->y,PZ->x,dms_rad(PZ->a));
		pArr[k].set(PZ->y, PZ->x, 0.0);
	};

	if (icolor < 0)pWd->subEntityTraits().setColor(5);
	else pWd->subEntityTraits().setColor(icolor);
	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
	pWd->geometry().polyline(nk + 1, pArr);
	// ads_printf(L"\n AA 类型=%d %d %10.3f %10.3f %5.1f %10.2f",(int)INDATA[0],nk,INDATA[1],INDATA[2],ND,LO);
	return  0;
}

Adesk::Boolean  BAS_DRAW_FUN::G_makearc(AcGiWorldDraw* pWd, AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx)
{
	AcGePoint3d centerPt, midPt;

	double f1, dd, AA;

	dd = xyddaa(startPt[1], startPt[0], endPt[1], endPt[0], &f1);
	AA = acos(dd*0.5 / R);
	centerPt[Y] = startPt[Y] + R * cos(f1 + npx * AA);
	centerPt[X] = startPt[X] + R * sin(f1 + npx * AA);
	centerPt[Z] = startPt[Z];

	AA = asin(dd*0.5 / R);
	midPt[Y] = centerPt[Y] + R * cos(f1 - npx * pi*0.5);
	midPt[X] = centerPt[X] + R * sin(f1 - npx * pi*0.5);
	midPt[Z] = centerPt[Z];

	pWd->subEntityTraits().setColor(1);
	pWd->geometry().circularArc(startPt, midPt, endPt);
	//	ads_printf(L"\n R 类型=2" );	ads_printf(L"\n R 类型=2" ); 
	return 0;
}

Adesk::Boolean  BAS_DRAW_FUN::G_makeline(AcGiWorldDraw* pWd, AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor)
{
	AcGePoint3d centerPt, midPt;

	AcGePoint3d* pArr = new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);
	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt000);
	pWd->geometry().polyline(2, pArr);
	return  0;
}

Adesk::Boolean  BAS_DRAW_FUN::G_makelineW(AcGiWorldDraw* pWd, AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor, AcDb::LineWeight lw)
{
	AcGePoint3d centerPt, midPt;

	AcGePoint3d* pArr = new AcGePoint3d[2];
	pArr[0].set(startPt.x, startPt.y, startPt.z);
	pArr[1].set(endPt.x, endPt.y, endPt.z);

	pWd->subEntityTraits().setColor(icolor);
	pWd->subEntityTraits().setLineWeight(lw);
	pWd->geometry().polyline(2, pArr);
	return  0;
}



Adesk::Boolean  BAS_DRAW_FUN::G_maketext(AcGiWorldDraw* pWd, AcGePoint3d& Pt, ACHAR* ctext, double ang, double texth, int icolor, int mode)
{
	//mode==1:Center
	AcGeVector3d normal(0.0, 0.0, 1.0), direction(cos(ang), sin(ang), 0.0);
	// ads_printf(L"\n             %10.3f %10.3f %10.4f ",Pt.x,Pt.y,dms_rad(ang));
	pWd->subEntityTraits().setColor(icolor);
	//    pWd->geometry().text(Pt , normal, direction, texth,1.0, 0.0, ctext);
	AcGiTextStyle pTextStyle;
	//pTextStyle.setFont(L"宋体",TRUE,FALSE,0,0);
	pTextStyle.setFont(L"宋体", 0, 0, 134, 2);
	pTextStyle.setTextSize(texth);
	AcGePoint3d Center;
	int ntext = _tcslen(ctext);
	if (mode == 1)//调整为中心对齐
	{
		Center.x = Pt.x - 0.5 * texth * cos(ang + 0.5 * PI) - 0.5 * ntext * texth * cos(ang);
		Center.y = Pt.y - 0.5 * texth * sin(ang + 0.5 * PI) - 0.5 * ntext * texth * sin(ang);
	}
	else
	{
		Center = Pt;
	}
	pWd->geometry().text(Center, normal, direction, ctext, _tcslen(ctext), Adesk::kTrue, pTextStyle);
	return 1;
}

Adesk::Boolean  BAS_DRAW_FUN::G_WXY(AcGiWorldDraw* pWd, double INDATA[5], struct xlpoint *P0, struct xlpoint *PZ, int icolor)
{
	int j;
	double l = 0.0;
	AcGePoint3d ptb, pt, pte;
	if (INDATA[1] < 0.00000001) return 1;
	l = Get_INDATA_L(INDATA, &j);
	// ads_printf(L"\n  类型=%d l=%10.3f" ,j,l );
	ptb[X] = P0->y; ptb[Y] = P0->x; ptb[Z] = 0.0;
	// ads_printf(L"\n PO=%10.3f %10.3f %10.3f %10.3f",P0->y,P0->x,dms_rad(P0->a),P0->lc) ;
	if ((j <= 3 && j >= 6 && INDATA[1] <= 0.000000000000001) || (l <= 0.00000000001)) return 1;

	if (j > 2 && j <= 6)
	{ //( 回旋线)
		if (icolor < 0)G_drawspi(pWd, INDATA, l, P0, PZ, 2.0);
		else G_drawspi(pWd, INDATA, l, P0, PZ, 2.0, icolor);
		xlzbxy(P0, PZ, l, INDATA, 0);// 某线元终点
	}
	else if (j <= 2)  // R L
	{//( 园弧 直线)
		xlzbxy(P0, PZ, l*0.5, INDATA, 0); // 某线元中点
		pt[X] = PZ->y; pt[Y] = PZ->x; pt[Z] = 0.0;
		xlzbxy(P0, PZ, l, INDATA, 0); // 某线元终点
		pte[X] = PZ->y; pte[Y] = PZ->x; pte[Z] = 0.0; //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
		if (j == 1) //直线
		{
			AcGePoint3d pcArr[2];
			pcArr[0] = ptb; pcArr[1] = pte;
			if (icolor < 0)pWd->subEntityTraits().setColor(3);
			else pWd->subEntityTraits().setColor(icolor);
			pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
			pWd->geometry().polyline(2, pcArr);
			// ads_printf(L"\n L 类型= 1"  );
		}
		else if (j == 2) // 园弧 			 
		{
			if (icolor < 0)pWd->subEntityTraits().setColor(50);
			else pWd->subEntityTraits().setColor(icolor);
			pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
			pWd->geometry().circularArc(ptb, pt, pte);
			// ads_printf(L"\n r 类型= 2"  );
		};
	};  //J
	// ads_printf(L"\n PZ=%10.3f %10.3f %10.3f %10.3f",PZ->y,PZ->x,dms_rad(PZ->a),PZ->lc) ;
	return 0;
}


void BAS_DRAW_FUN::StdJiao(double &jiao)
{
	if (jiao < 0.0)jiao += 2.0*PI;
	if (jiao > 2.0*PI)jiao -= 2.0*PI;
}

int BAS_DRAW_FUN::SIGN(double a)
{
	if (a < 0.0)return -1;
	else if (a > 0.0)return 1;
	else return 0;
}


///////////////////////////////
//求三个交点的左右转，左-1，右+1
int BAS_DRAW_FUN::GetLeftOrRight3JD(double N1, double E1, double N0, double E0, double N2, double E2)
{
	double fb, fe;
	int c;
	xyddaa(N1, E1, N0, E0, &fb);
	xyddaa(N0, E0, N2, E2, &fe);
	double aa = fe - fb;
	if (aa < -pi)aa = aa + 2 * pi;
	if (aa > pi)aa = aa - 2 * pi;
	if (aa > 0)c = 1;//右偏
	else c = -1;//左偏
	return c;
}

void BAS_DRAW_FUN::GetCenter3JD(double N1, double E1, double N0, double E0, double N2, double E2, double R, double Lo1, double Lo2, double &CenN, double &CenE)
{
	int c;
	double fb, fe;
	double BPMTL[10], T1;
	xyddaa(N1, E1, N0, E0, &fb);
	xyddaa(N0, E0, N2, E2, &fe);
	double aa = fe - fb;
	if (aa < -pi)aa = aa + 2 * pi;
	if (aa > pi)aa = aa - 2 * pi;
	if (aa > 0)c = 1;//右偏
	else c = -1;//左偏
	aa = fabs(aa);
	aa = dms_rad(aa);
	if (R <= 0)return;

	double p1;//前内移量
	if (Lo1 > 0.0)
	{
		double A1 = sqrt(R*Lo1);
		double x = spiral_y(A1, Lo1);
		double t = Lo1 / (2.0*R);
		double dr = x + R * sin(t + 0.5*PI);
		p1 = dr - R;
	}
	else p1 = 0.0;

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
	double NZH, EZH;
	getBPMTL(aa, R, Lo1, Lo2, &BPMTL[0]);
	T1 = BPMTL[6];
	NZH = N0 - T1 * cos(fb);
	EZH = E0 - T1 * sin(fb);
	//缓圆点坐标
	double NHY, EHY;
	if (Lo1 > 0.0)
	{
		double x, y, t, A1;
		A1 = sqrt(R*Lo1);
		x = spiral_x(A1, Lo1);
		y = spiral_y(A1, Lo1);
		double dist = xyddaa(0.0, 0.0, x, y, &t);
		NHY = NZH + dist * cos(fb + c * t);
		EHY = EZH + dist * sin(fb + c * t);
	}
	else
	{
		NHY = NZH;
		EHY = EZH;
	}
	double t = 0.5*Lo1 / R;
	CenN = NHY + R * cos(fb + c * (t + 0.5*PI));
	CenE = EHY + R * sin(fb + c * (t + 0.5*PI));
}

//给定圆心半径，从圆到圆外一点的左右转信息，求出公切点
void BAS_DRAW_FUN::GetQD(double CenN, double CenE, double R,
	double OutN, double OutE, int LRFlag,
	double &QDN, double &QDE)
{
	double DistCenterToOutPt;//圆心到圆外点之距
	double FwjCenterToOutPt;//圆心到圆外点连线的方位角
	DistCenterToOutPt = xyddaa(CenN, CenE, OutN, OutE, &FwjCenterToOutPt);
	if (DistCenterToOutPt <= R)return;//点落到圆内
	double t;//公切点与圆心连线 与 圆心与圆外点连线 的夹角	
	t = acos(R / DistCenterToOutPt);
	double FwjCenterToQD;//圆心到切点连线的方位角
	int c;
	if (LRFlag < 0)c = -1;//左转
	else c = 1;//右转
	FwjCenterToQD = FwjCenterToOutPt - c * t;
	QDN = CenN + R * cos(FwjCenterToQD);
	QDE = CenE + R * sin(FwjCenterToQD);
	//acutPrintf(L"QD:%lf,%lf\n",QDE,QDN);
}

Acad::ErrorStatus BAS_DRAW_FUN::getAllEntity(AcDbDatabase *pDb, AcDbObjectIdArray& IdArr,
	const AcArray<CString>& layerNameArr)
{
	Acad::ErrorStatus es = Acad::eOk;
	ASSERT(pDb);
	if (pDb == NULL)
		return Acad::eInvalidInput;
	AcDbBlockTable *pBlkTable = NULL;
	if ((es = pDb->getBlockTable(pBlkTable, AcDb::kForRead)) != Acad::eOk)//打开块表
	{
		acedAlert(L"打开块表失败");
		return es;
	}
	AcDbBlockTableRecord *pBlkTableRecord = NULL;
	if ((es = pBlkTable->getAt(ACDB_MODEL_SPACE, pBlkTableRecord, AcDb::kForRead)) != Acad::eOk)//打开块表记录
	{
		acedAlert(L"打开块表记录失败");
		pBlkTable->close();
		return es;
	}
	pBlkTable->close();//关闭块表

	AcDbBlockTableRecordIterator *pIterator = NULL;                       //创建叠代器
	if ((es = pBlkTableRecord->newIterator(pIterator)) != Acad::eOk)
	{
		pBlkTableRecord->close();
		return es;
	}

	CAcModuleResourceOverride resoverride;
	//	progress.SetWindowText(L"正在检测图形中所有实体...");

	for (pIterator->start(); !pIterator->done(); pIterator->step())//遍历整个数据库
	{
		AcDbEntity *entity = NULL;
		es = pIterator->getEntity(entity, AcDb::kForRead);	//打开实体

		if (es == Acad::eLockViolation)
		{
			acedAlert(L"内存锁定");
		}
		else if (es == Acad::eWasOpenForWrite)
		{
			acedAlert(L"实体以写方式打开");
		}
		else if (es == Acad::eWasOpenForRead)
		{
			acedAlert(L"实体以读方式打开");
		}
		else
		{
			if (layerNameArr.contains(entity->layer()))
				IdArr.append(entity->objectId());
			entity->close();
		}
	}
	delete pIterator; pIterator = NULL;
	pBlkTableRecord->close();
	return es;
}
#include "JD_CENTER.h"
void BAS_DRAW_FUN::GetJD_CENTEREntity(AcDbObjectIdArray& IdArr)
{
	AcDbObjectIdArray IdArr0;
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	AcArray<CString> layerArray;
	layerArray.append("公路平面设计专用层");
	getAllEntity(acdbHostApplicationServices()->workingDatabase(), IdArr0, layerArray);
	int i, num = IdArr0.logicalLength();
	for (i = 0; i < num; i++)
	{
		AcDbEntity *pEnt;
		if (Acad::eOk == acdbOpenObject(pEnt, IdArr0[i], AcDb::kForRead))
		{
			if (pEnt->isKindOf(JD_CENTER::desc()))
			{
				IdArr.append(IdArr0[i]);
			}
		}
		pEnt->close();
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
}

//-------------------------------------------------------------------
double BAS_DRAW_FUN::chk(double pt1[3], double pt2[3], double pt3[3])
{
	double a, b;
	if (pt2[X] == pt1[X])
		pt1[X] = pt1[X] - 0.0001;
	a = (pt2[Y] - pt1[Y]) / (pt2[X] - pt1[X]);
	b = (pt1[Y] * pt2[X] - pt2[Y] * pt1[X]) / (pt2[X] - pt1[X]);
	return (pt3[Y] - a * pt3[X] - b);
}

//---------------------------------------------------------------------
//求方位角
double BAS_DRAW_FUN::FAngle(double s1, double s2)
{
	double c;
	if (fabs(s1) < 0.0000001)
	{
		if (s2 > 0.0)
		{
			c = 0.5*PI;
		}
		else
		{
			c = 1.5*PI;
		}
	}
	else
	{
		if (s1 < 0.0)
		{
			c = PI + atan(s2 / s1);
		}
		else
		{
			if (s2 < 0.0)
			{
				c = 2.0*PI + atan(s2 / s1);
			}
			else
			{
				c = atan(s2 / s1);
			}
		}
	}
	return(c);
}
/*-----------------------------------------------*/
/*-Get the height of edge pt1-pt2;pt3 is vertex--*/
/*------------------------------------------*/
double BAS_DRAW_FUN::cal_h(ads_point pt1, ads_point pt2, ads_point pt3)
{
	double angl, a, b, s, s1, s2;
	double watchA1 = FAngle(pt1[X] - pt3[X], pt1[Y] - pt3[Y]);
	double watchA2 = FAngle(pt2[X] - pt3[X], pt2[Y] - pt3[Y]);
	angl = FAngle(pt1[X] - pt3[X], pt1[Y] - pt3[Y]) - FAngle(pt2[X] - pt3[X], pt2[Y] - pt3[Y]);	//pt3与pt1、pt2之间的夹角
	if (angl < -0.00000001)
		angl = angl + 2.0 * PI;
	s1 = pt1[X] - pt3[X];
	s2 = pt1[Y] - pt3[Y];
	a = sqrt(s1*s1 + s2 * s2);
	s1 = pt2[X] - pt3[X];
	s2 = pt2[Y] - pt3[Y];
	b = sqrt(s1*s1 + s2 * s2);
	s1 = pt2[X] - pt1[X];
	s2 = pt2[Y] - pt1[Y];
	s = sqrt(s1*s1 + s2 * s2);
	if (chk(pt1, pt2, pt3) <= 0)
		return(-1.0 * fabs(a * b * sin(angl) / s));
	else
		return(fabs(a * b * sin(angl) / s));
}

void BAS_DRAW_FUN::getMinMaxXY(ads_point p1, ads_point p2, ads_point p3, ads_point p4, double &xmin, double &xmax, double &ymin, double &ymax)
{
	xmin = p1[X];
	xmax = p1[X];
	if (xmin > p2[X])xmin = p2[X];
	if (xmin > p3[X])xmin = p3[X];
	if (xmin > p4[X])xmin = p4[X];

	if (xmax < p2[X])xmax = p2[X];
	if (xmax < p3[X])xmax = p3[X];
	if (xmax < p4[X])xmax = p4[X];

	ymin = p1[Y];
	ymax = p1[Y];
	if (ymin > p2[Y])ymin = p2[Y];
	if (ymin > p3[Y])ymin = p3[Y];
	if (ymin > p4[Y])ymin = p4[Y];

	if (ymax < p2[Y])ymax = p2[Y];
	if (ymax < p3[Y])ymax = p3[Y];
	if (ymax < p4[Y])ymax = p4[Y];
}

void BAS_DRAW_FUN::fline(ads_point pt1, ads_point pt2, ads_point result, double delta)
{
	ads_real angle;

	angle = ads_angle(pt2, pt1);
	ads_polar(pt1, angle, delta, result);
}

void BAS_DRAW_FUN::CalFrame(double dist, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, ads_point &p1, ads_point &p2, ads_point &p3, ads_point &p4)
{
	ads_point Lfup, Lfbt, Rtup, Rtbt;

	Lfup[X] = P1.x; Lfup[Y] = P1.y;
	Rtup[X] = P2.x; Rtup[Y] = P2.y;
	Rtbt[X] = P3.x; Rtbt[Y] = P3.y;
	Lfbt[X] = P4.x; Lfbt[Y] = P4.y;

	fline(Lfup, Rtbt, p1, -dist);
	fline(Rtup, Lfbt, p2, -dist);
	fline(Rtbt, Lfup, p3, -dist);
	fline(Lfbt, Rtup, p4, -dist);


	/*
	AcGePoint3dArray vertices;
	AcGePoint3d Pt;
	Pt.x = p1[X];	Pt.y = p1[Y];
	vertices.append(Pt);
	Pt.x = p2[X];	Pt.y = p2[Y];
	vertices.append(Pt);
	Pt.x = p3[X];	Pt.y = p3[Y];
	vertices.append(Pt);
	Pt.x = p4[X];	Pt.y = p4[Y];
	vertices.append(Pt);
	AcDbObjectId eId,eId1;
	AcDb2dPolyline *Poly=new AcDb2dPolyline(AcDb::k2dSimplePoly,vertices,0.0,Adesk::kTrue);
	eId=AddEntityToDbs(Poly);*/


	/*	//用四点作一POLYLINE，OFFSET后得到另一条POLYLINE。
	AcGePoint3dArray vertices;
	vertices.append(P1);
	vertices.append(P2);
	vertices.append(P3);
	vertices.append(P4);
	AcDbVoidPtrArray offsetCurves;
	AcDbObjectId eId,eId1;
	AcDb2dPolyline *Poly=new AcDb2dPolyline(AcDb::k2dSimplePoly,vertices,0.0,Adesk::kTrue);
	eId=AddEntityToDbs(Poly);

	Acad::ErrorStatus es;
	es=acdbOpenObject(Poly,eId,AcDb::kForWrite);
	if(es!=Acad::eOk)return;
	Poly->getOffsetCurves(dist,offsetCurves);
	//	acutPrintf(L"dist=%lf\n",dist);
	Poly->erase();
	Poly->close();
	AcDb2dPolyline *offsetPoly=(AcDb2dPolyline*)offsetCurves[0];
	eId1=AddEntityToDbs(offsetPoly);
	AcDbObjectIterator *pVertIter=offsetPoly->vertexIterator();
	AcDb2dVertex *pVertex;
	AcGePoint3d location;
	AcDbObjectId VertexObjId;
	AcGePoint3dArray ver;
	for(int vertexNumber=0;!pVertIter->done();vertexNumber++,pVertIter->step())
	{
	VertexObjId=pVertIter->objectId();
	acdbOpenObject(pVertex,VertexObjId,AcDb::kForRead);
	location=pVertex->position();
	ver.append(location);
	pVertex->close();
	}
	delete pVertIter;
	p1[X]=ver[0].x;p1[Y]=ver[0].y;
	p2[X]=ver[1].x;p2[Y]=ver[1].y;
	p3[X]=ver[2].x;p3[Y]=ver[2].y;
	p4[X]=ver[3].x;p4[Y]=ver[3].y;
	ver.removeSubArray(0,ver.length()-1);
	acdbOpenObject(offsetPoly,eId1,AcDb::kForWrite);
	offsetPoly->erase();
	offsetPoly->close();
	vertices.removeSubArray(0,vertices.length()-1);

	AcGePoint3d Pt;
	Pt.x = p1[X];	Pt.y = p1[Y];
	vertices.append(Pt);
	Pt.x = p2[X];	Pt.y = p2[Y];
	vertices.append(Pt);
	Pt.x = p3[X];	Pt.y = p3[Y];
	vertices.append(Pt);
	Pt.x = p4[X];	Pt.y = p4[Y];
	vertices.append(Pt);
	AcDb2dPolyline *Poly1=new AcDb2dPolyline(AcDb::k2dSimplePoly,vertices,0.0,Adesk::kTrue);
	eId=AddEntityToDbs(Poly1);*/
}//-----------------------------------------------------------------------------

void BAS_DRAW_FUN::AutoEntTransform(AcGeMatrix3d xformr, AcGeMatrix3d xformt, ads_point p1, ads_point p2, ads_point p3, ads_point p4)
{
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	AcGeMatrix3d xform = xformt * xformr;
	ads_name ssname, ename;
	struct resbuf *pointlist;
	long n;
	AcDbObjectId eId;
	AcDbEntity *pEnt, *pEntclone;
	pointlist = ads_buildlist(RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, 0);
	acedSSGet(L"WP", pointlist, NULL, NULL, ssname);
	if (ads_sslength(ssname, &n) == RTNORM)
	{
		//ads_printf(L"Number of entity is %ld\n",n);
		int Max = n;
		double Cur = 0;
		int Pos = 0;
		while (n > 0)
		{
			Cur = (Max - n) * 100.0 / Max;
			Pos = (int)Cur;
			acedSetStatusBarProgressMeterPos(Pos);

			n = n - 1;
			ads_ssname(ssname, n, ename);
			acdbGetObjectId(eId, ename);
			if (Acad::eOk != acdbOpenObject(pEnt, eId, AcDb::kForWrite))
				continue;

			if (pEnt->getTransformedCopy(xform, pEntclone) == Acad::eOk)
			{
				double ConstWid = 0.0;
				if (pEnt->isKindOf(AcDbPolyline::desc()))
				{
					AcDbPolyline* p2dPolyline = AcDbPolyline::cast(pEnt);
					p2dPolyline->getConstantWidth(ConstWid);

					AcDbPolyline* ColneLine = AcDbPolyline::cast(pEntclone);
					ColneLine->setConstantWidth(ConstWid);

					AddEntityToDbs(ColneLine);
				}
				else
				{
					//20200322梁柱，判断非空，避免死机
					if (pEntclone)
						AddEntityToDbs(pEntclone);
					else
					{
						ads_printf(L"Can't obtain the object!!!");
						return;
					}
				}

			}
			pEnt->close();

		}
		ads_relrb(pointlist);
		ads_ssfree(ssname);
	}
	ads_point Pt;
	Pt[X] = 0.0; Pt[Y] = 0.0;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"PAN", RTPOINT, Pt, RTPOINT, Pt, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"PAN", RTPOINT, Pt, RTPOINT, Pt, RTNONE);
#endif // VERSION_CAD2010
	acDocManager->unlockDocument(acDocManager->curDocument());
}

int BAS_DRAW_FUN::ZDKAutoEntBK(ads_point qLpt, ads_point hLpt, ads_point hRpt, ads_point qRpt, int ITK)
{
	bool fail1, fail2;
	//     1       2       
	fail1 = fail2 = false;
	if (ITK > 0)
	{
		if (ITK == 1 || ITK == 12)
		{
			ads_grdraw(qLpt, hLpt, -1, 0);
			if (AutoEntBK1(qLpt, hLpt) < 0)
				fail1 = true;
		}

		if (ITK == 2 || ITK == 12)
		{
			ads_grdraw(qRpt, hRpt, -1, 0);
			if (AutoEntBK1(qRpt, hRpt) < 0)
				fail2 = true;
		}

		if (fail1&&fail2)
			return 12;
		else if (fail1)
			return 1;
		else if (fail2)
			return 2;
		else
			return -10;
	}
	return -10;
}

int BAS_DRAW_FUN::AutoEntBK(ads_point p0, ads_point p1, ads_point p2, ads_point p3, int ITK)
{
	bool fail1, fail2, fail3, fail4;
	//     1       2       3      4
	fail1 = fail2 = fail3 = fail4 = false;
	//	acedSetStatusBarProgressMeterPos(0);
	if (ITK > 0)
	{

		//		if(p0[X] < 1000||p1[X] < 1000||p2[X] < 1000||p3[X] < 1000)
		//		{
		//		  ads_printf(L"p0x=%lf p0y=%lf p1x=%lf p1y=%lf\n",p0[X],p0[Y],p1[X],p1[Y]);	
		//		  ads_printf(L"p2x=%lf p2y=%lf p3x=%lf p3y=%lf\n",p2[X],p2[Y],p3[X],p3[Y]);
		//		}

		if (ITK == 1 || ITK == 12 || ITK == 13 || ITK == 14 || ITK == 123 || ITK == 124 || ITK == 134 || ITK == 1234)
		{
			ads_grdraw(p0, p1, -1, 0);

			if (AutoEntBK1(p0, p1) < 0)
				fail1 = true;

		}
		//	  acedSetStatusBarProgressMeterPos(36);
		if (ITK == 2 || ITK == 12 || ITK == 23 || ITK == 24 || ITK == 123 || ITK == 124 || ITK == 234 || ITK == 1234)
		{
			ads_grdraw(p1, p2, -1, 0);

			if (AutoEntBK1(p1, p2) < 0)
				fail2 = true;

		}
		//	  acedSetStatusBarProgressMeterPos(50);
		if (ITK == 3 || ITK == 13 || ITK == 23 || ITK == 34 || ITK == 123 || ITK == 134 || ITK == 234 || ITK == 1234)
		{
			ads_grdraw(p2, p3, -1, 0);

			if (AutoEntBK1(p2, p3) < 0)
				fail3 = true;

		}
		//	  acedSetStatusBarProgressMeterPos(86);
		if (ITK == 4 || ITK == 14 || ITK == 24 || ITK == 34 || ITK == 124 || ITK == 134 || ITK == 234 || ITK == 1234)
		{
			ads_grdraw(p3, p0, -1, 0);

			if (AutoEntBK1(p3, p0) < 0)
				fail4 = true;

		}
#ifdef VERSION_CAD2016
		acedCommandS(0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(0);
#endif // VERSION_CAD2010


		if (fail1&&fail2&&fail3&&fail4)
			return 1234;
		else if (fail1&&fail2&&fail3)
			return 123;
		else if (fail1&&fail2&&fail4)
			return 124;
		else if (fail1&&fail3&&fail4)
			return 134;
		else if (fail2&&fail3&&fail4)
			return 234;
		else if (fail1&&fail2)
			return 12;
		else if (fail1&&fail3)
			return 13;
		else if (fail1&&fail4)
			return 14;
		else if (fail2&&fail3)
			return 23;
		else if (fail2&&fail4)
			return 24;
		else if (fail3&&fail4)
			return 34;
		else  if (fail1)
			return 1;
		else  if (fail2)
			return 2;
		else  if (fail3)
			return 3;
		else  if (fail4)
			return 4;
		else
			return -10;
		//	  acedSetStatusBarProgressMeterPos(100);
	}
	return -10;
}

//删除与 p0， p1, p2, p3相交的实体
void BAS_DRAW_FUN::DelBoxObject(ads_point p0, ads_point p1, ads_point p2, ads_point p3)
{
	AcGePoint3dArray points;
	AcDbVoidPtrArray curveSegments;
	ads_point breakPt;
	int flag = 1;//1:OK  -1:not finished,有线没剪断
	ads_name ssname, ename, last;
	struct resbuf *pointlist;
	long n;
	AcGePoint3d Pt0, Pt1;

	AcDbObjectId eId;
	AcDbEntity *pEnt;
	AcDbCurve *pCurve;
	Acad::ErrorStatus es;

	pointlist = ads_buildlist(RTPOINT, p0, RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, 0);
	//  ads_grdraw(p0,p1,-1,0);

	acedSSGet(L"F", pointlist, NULL, NULL, ssname);
	if (ads_sslength(ssname, &n) == RTNORM)
	{

		int Max = n;
		double Cur = 0;
		int Pos = 0;
		es = Acad::eOk;
		while (n > 0)
		{
			n = n - 1;
			ads_ssname(ssname, n, ename);
			acdbGetObjectId(eId, ename);
			if (Acad::eOk == acdbOpenObject(pEnt, eId, AcDb::kForWrite))
			{
				pEnt->erase(Adesk::kTrue);
				pEnt->close();
			}
		}
	}
}
//

int BAS_DRAW_FUN::AutoEntBK1(ads_point p0, ads_point p1)
{
	AcGePoint3dArray points;
	AcDbVoidPtrArray curveSegments;
	ads_point breakPt;
	int flag = 1;//1:OK  -1:not finished,有线没剪断
	ads_name ssname, ename, last;
	struct resbuf *pointlist;
	long n;
	AcGePoint3d Pt0, Pt1;
	AcGePoint3d tempP0, tempP1;
	Pt0.set(p0[X], p0[Y], 0);
	Pt1.set(p1[X], p1[Y], 0);
	tempP0.set(p0[X], p0[Y], 0);
	tempP1.set(p1[X], p1[Y], 0);

	AcDbLine *pLine = new AcDbLine(Pt0, Pt1);
	AcDb2dPolyline* Polyline2d;
	AcDb3dPolyline* Polyline3d;
	AcDb::Poly2dType Poly2dTyp;
	AcDb::Poly3dType Poly3dTyp;

	//	AcDbObjectId Lid=ob.AddEntityToDbs(pLine);
	//AcDbLine pLine1(Pt0,Pt1);

	AcDbObjectId eId, vertexObjId;
	AcDbEntity *pEnt;
	AcDbCurve *pCurve;
	Acad::ErrorStatus es;
	AcGePlane plane(AcGePoint3d(0, 0, 0), AcGeVector3d(0, 0, 1));
	//	AcGePlane plane();	
	pointlist = ads_buildlist(RTPOINT, p0, RTPOINT, p1, 0);

	acedSSGet(L"F", pointlist, NULL, NULL, ssname);

	int watch = ads_sslength(ssname, &n);
	if (watch == RTNORM)
	{
		int Max = n;
		double Cur = 0;
		int Pos = 0;
		es = Acad::eOk;

		//逐个实体循环
		while (n > 0)
		{
			n = n - 1;
			ads_ssname(ssname, n, ename);
			acdbGetObjectId(eId, ename);
			if (Acad::eOk != acdbOpenObject(pEnt, eId, AcDb::kForRead))
			{
				acutPrintf(L"eId=%ld,n=%d  ", eId, n);
				acutPrintf(L"open error\n");
				continue;
			}
			pEnt->close();
			//ads_printf(L"n=%d \n",n);
			if (pEnt->isKindOf(TRIM_ROAD::desc())
				|| pEnt->isKindOf(AcDbGroup::desc())
				|| pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				continue;
			}
			if (points.length() > 0)
				points.removeSubArray(0, points.length() - 1);


			es = pEnt->intersectWith(pLine, AcDb::kOnBothOperands, plane, points);
			//ads_printf(L"name=%s %d\n",pEnt->isA()->name(),points.length());
			if (pEnt->isKindOf(AcDbCurve::desc()) && Acad::eOk == es && points.length() > 0)
			{
				if (acdbOpenObject(pCurve, eId, AcDb::kForRead) != Acad::eOk)
					continue;
				pCurve->close();//      

				if (curveSegments.length() > 0)
					curveSegments.removeSubArray(0, curveSegments.length() - 1);

				//if (_tcscmp(pCurve->layer(), L"平面图-中线"))
				//	ads_printf(L"!");

				//20190905	针对2dpolyline可能存在的崩溃问题，先进行分解操作
				AcDbVoidPtrArray entitySet;
				if (pCurve->isKindOf(AcDb2dPolyline::desc()) || pCurve->isKindOf(AcDb3dPolyline::desc()))
				{
					pCurve->explode(entitySet);
					for (int i = 0; i < entitySet.length(); i++)
					{
						pCurve = (AcDbCurve*)entitySet[i];
						es = pCurve->intersectWith(pLine, AcDb::kOnBothOperands, plane, points);
						if (Acad::eOk == es)
						{
							pCurve->getSplitCurves(points, curveSegments);
							AcDbEntity* iEnt;
							for (int j = 0; j < curveSegments.length(); j++)
							{
								iEnt = (AcDbEntity*)curveSegments[j];
								AddEntityToDbs(iEnt, 0);
							}
						}

						AddEntityToDbs(pCurve);
					}
				}
				else
				{
					try
					{
						es = pCurve->getSplitCurves(points, curveSegments);
					}
					catch (...)
					{
						continue;
					}
				}

				if (es == Acad::eOk)
				{
					if (eId && acdbOpenObject(pCurve, eId, AcDb::kForWrite) == Acad::eOk)
					{
						pCurve->erase();
						pCurve->close();
						AcDbEntity* iEnt;
						for (int j = 0; j < curveSegments.length(); j++)
						{
							iEnt = (AcDbEntity*)curveSegments[j];
							AddEntityToDbs(iEnt, 0);
						}
					}
					else
						continue;
				}
			}
			if (es != Acad::eOk)
				flag = -1;

		}//while()
		ads_relrb(pointlist);
		ads_ssfree(ssname);
		//	acedRestoreStatusBar();		
	}
	if (points.length() > 0)
		points.removeSubArray(0, points.length() - 1);

	delete pLine;
	pLine = NULL;
	ads_point Pt;
	Pt[X] = 0.0; Pt[Y] = 0.0;

#ifdef VERSION_CAD2016
	acedCommandS(RTNONE);
	acedCommandS(RTSTR, L"PAN", RTPOINT, Pt, RTPOINT, Pt, RTNONE);//更新视图	
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTNONE);
	ads_command(RTSTR, L"PAN", RTPOINT, Pt, RTPOINT, Pt, RTNONE);//更新视图	
	ads_command(RTNONE);
#endif // VERSION_CAD2010

	return flag;
}
//------------------------------------------------------
//        enttype= "F" or "C"
//------------------------------------------------------
/*
int  BAS_DRAW_FUN::AutoEntBK1(ads_point p0, ads_point p1)
{
struct resbuf *e,*elst,*p,*hdptr,*tlptr;
ads_name ssname,ename,last;
struct ptt_node *pttab,*newptr;
struct resbuf *pointlist;
ads_point pt0,pt1,mpt,tpt;
int i,fit_type;
long n;
ACHAR a[20];
pointlist = ads_buildlist(RTPOINT,p0,RTPOINT,p1,0);
acedCommandS(RTSTR,L"line",RTPOINT,p0,RTPOINT,p1,RTSTR,L"",0);
ads_grdraw(p0,p1,-1,0);
acedCommandS(RTSTR,L"line",RTPOINT,p0,RTPOINT,p1,RTSTR,L"",0);
ads_ssget(L"F",pointlist,NULL,NULL,ssname);
if(ads_sslength(ssname,&n)==RTNORM)
{
ads_printf(L"Number of entity is %ld\n",n);

L60:
while(n > 0)
{
n = n - 1;

if(ads_ssname(ssname,n,ename)==RTNORM)
{
e = ads_entget(ename);
}
else
goto L60;
elst = get_resbuf(0,e);

_tcscpy(a,elst->resval.rstring);
//     ads_printf(L"a=%s\n",a);
if(_tcscmp(a,L"LINE")==0)
{
getrbpoint(pt0,e,10);
getrbpoint(pt1,e,11);
i = ads_inters(p0,p1,pt0,pt1,1,mpt);
i = INTERS(&mpt[X],&mpt[Y],p0[X],p0[Y],p1[X],p1[Y],pt0[X],pt0[Y],pt1[X],pt1[Y]);
mpt[Z] = pt0[Z];
if(i==0 && (fabs(mpt[X]-pt0[X])>0.001 || fabs(mpt[Y]-pt0[Y])>0.001 || fabs(mpt[Z]-pt0[Z])>0.001)
&& (fabs(mpt[X]-pt1[X])>0.001 || fabs(mpt[Y]-pt1[Y])>0.001 || fabs(mpt[Z]-pt1[Z])>0.001))
acedCommandS(RTSTR,L"break",RTENAME,ename,RTPOINT,mpt,RTPOINT,mpt,0);
acedCommandS(RTNONE);
}
if(_tcscmp(a,L"LWPOLYLINE")==0)
{
elst = get_resbuf(70,e);
fit_type = elst->resval.rint;
if(fit_type == 1 || fit_type == 129)
{
elst->resval.rint = 0;
elst = get_resbuf(10,e);
if(elst)
{
mpt[X] = elst->resval.rpoint[X];
mpt[Y] = elst->resval.rpoint[Y];
}
p = ads_newrb(RTPOINT);
p->restype = 10;
p->resval.rpoint[X] = mpt[X];
p->resval.rpoint[Y] = mpt[Y];
hdptr = p;
tlptr = p;
p = ads_newrb(RTREAL);
p->restype = 40;
p->resval.rreal = 0.0;
tlptr->rbnext = p;
tlptr = p;
p = ads_newrb(RTREAL);
p->restype = 41;
p->resval.rreal = 0.0;
tlptr->rbnext = p;
tlptr = p;
p = ads_newrb(RTREAL);
p->restype = 42;
p->resval.rreal = 0.0;
tlptr->rbnext = p;
tlptr = p;
p = e;
while(p->rbnext->restype != 210)
p = p->rbnext;
tlptr->rbnext = p->rbnext;
p->rbnext = hdptr;
ads_entmod(e);

}
pttab = get_brkpt(e,p0,p1);

newptr = pttab->next;
if(newptr)
{
ads_point_set(newptr->pt,mpt);
ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
acedCommandS(RTSTR,L"break",RTENAME,ename,RTPOINT,mpt,RTPOINT,mpt,0);
acedCommandS(RTNONE);

pttab->next = newptr->next;
free(newptr);
newptr = pttab;
}
else
{
if(pttab) free(pttab);
ads_relrb(e);
ads_ssfree(ename);
goto L60;
}
while(newptr->next != NULL)
{
newptr = newptr->next;
ads_point_set(newptr->pt,mpt);
ads_entlast(last);
ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
acedCommandS(RTSTR,L"break",RTENAME,last,RTPOINT,mpt,RTPOINT,mpt,0);
acedCommandS(RTNONE);
pttab->next = newptr->next;
free(newptr);
ads_ssfree(last);
newptr = pttab;
}
if(pttab) free(pttab);
}
if(_tcscmp(a,L"POLYLINE")==0)
{
pttab = getbrkpt(e,p0,p1);
newptr = pttab->next;
if(newptr)
{
ads_point_set(newptr->pt,mpt);
ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
acedCommandS(RTSTR,L"break",RTENAME,ename,RTPOINT,mpt,RTPOINT,mpt,0);
acedCommandS(RTNONE);

pttab->next = newptr->next;
free(newptr);
newptr = pttab;
}
else
{
if(pttab) free(pttab);
ads_relrb(e);

ads_ssfree(ename);
goto L60;
}

while(newptr->next != NULL)
{
newptr = newptr->next;
ads_point_set(newptr->pt,mpt);
ads_entlast(last);
ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
acedCommandS(RTSTR,L"break",RTENAME,last,RTPOINT,mpt,RTPOINT,mpt,0);
acedCommandS(RTNONE);
pttab->next = newptr->next;
free(newptr);
ads_ssfree(last);
newptr = pttab;
}
if(pttab) free(pttab);
}
ads_relrb(e);
ads_ssfree(ename);
}
}
ads_relrb(pointlist);
ads_ssfree(ssname);
return 1;
}
*/
AcDbObjectId BAS_DRAW_FUN::CreateBlockFromDwgFile(ACHAR *Path, ACHAR *Name)
//传入路径，图名，从图中读取所有实体做为一个块，块名为图名NAME，返回块的ID号
{
	AcDbObjectId pBlockId = 0;
	AcDbDatabase *pDb = new AcDbDatabase(Adesk::kFalse);

	if (pDb->readDwgFile(Path, _SH_DENYNO) == Acad::eOk)
	{
		acdbHostApplicationServices()->workingDatabase()->insert(pBlockId, Name, pDb);
	}
	delete pDb;
	return pBlockId;
}

int BAS_DRAW_FUN::ReadWorkdir()
{
	ACHAR fname[256];

	//////////////////////////////////////////
	//_tcscpy(fname,acedGetAppName());
	//CString fstr;
	//fstr = fname;
	//fstr.MakeUpper();
	//fstr.Replace(L"\\ROADDESIGN.ARX",NULL);
	//_tcscpy(SysPath,fstr);
	//////////////////////////////////////////(zdf改)

	_stprintf(fname, L"%s\\workdir", SysPath);
	FILE *fpr = _wfopen(fname, L"r");
	if (!fpr)
	{
		AfxMessageBox(L"您还没有设定项目名称!");
		Cworkdir = "";
		return 0;
	}

	fwscanf(fpr, L"%s", fname);
	fclose(fpr);
	Cworkdir = fname;
#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
	//Cworkdir += "\\DATA\\改移道路"; //lbw ???????????????????????????????????????????????????????????????????????????????????????改移道路的路径到底是啥
#endif

	InStr$toNull();

	Cworkdir.MakeUpper();
	CString ProFname;

	ProFname = Cworkdir + "\\project";
	fpr = _wfopen(ProFname, L"r");
	if (!fpr)
	{
		AfxMessageBox(L"您还没有指定设计方案名称!");
		//	ads_printf(L"您还没有指定设计方案名称!\n");
		return 0;
	}
	fwscanf(fpr, L"%s", fname);
	fclose(fpr);
	Pro = fname;

	//20191106	修改读取
	int len, i;
	len = Pro.GetLength();
	i = Pro.Find('$');
	while (i < len && i >= 0)
	{
		Pro.SetAt(i, ' ');
		i = Pro.Find('$');
	}

	//ProFname=Cworkdir+"\\Modify";
	//fpr = _wfopen(ProFname,L"r");
	//if(!fpr)
	//{
	//	AfxMessageBox(L"您还没有指定设计方案名称!");
	//	return 0;
	//}
	//fwscanf(fpr,L"%s",fname);
	//fclose(fpr);
	//Mdf = fname;

	return 1;
}
void BAS_DRAW_FUN::NullInStrto$(CString& text)
{
	int len, i;

	len = text.GetLength();
	i = text.Find(L" ");
	while (i < len && i >= 0)
	{
		text.SetAt(i, '$');
		i = text.Find(L" ");
	}
	if (len == 0)
		text = "$";
}

void BAS_DRAW_FUN::InStr$toNull()
{
	int len, i;

	len = Cworkdir.GetLength();
	i = Cworkdir.Find('$');
	while (i < len && i >= 0)
	{
		Cworkdir.SetAt(i, ' ');
		i = Cworkdir.Find('$');
	}
}
CString BAS_DRAW_FUN::InStr$toNull(CString text)
{
	int len, i;

	len = text.GetLength();
	i = text.Find('$');
	while (i < len && i >= 0)
	{
		text.SetAt(i, ' ');
		i = text.Find('$');
	}

	return text;
}
void BAS_DRAW_FUN::CheckFwj(double &fwj)
{
	if (fwj > 2 * PI) fwj -= 2 * PI;
	if (fwj < 0) fwj += 2 * PI;
}


void BAS_DRAW_FUN::SetZdmMinPC(int DV, CString &MAXPD, CString &YBPCMin, CString &JXPCMin, CString &T_YBRshuMin, CString &T_JXRShu,
	CString &A_YBRshuMin, CString &A_JXRShu, CString &YBLRshuMin, CString &JXLRshuMin)
{
	YBPCMin = "";		//一般坡长最小值
	JXPCMin = "";		//极限坡长最小值
	T_YBRshuMin = "";	//凸形竖曲线	一般最小值
	T_JXRShu = "";		//凸形竖曲线	极限最小值
	A_YBRshuMin = "";	//凹形竖曲线	一般最小值
	A_JXRShu = "";		//凹形竖曲线	极限最小值
	YBLRshuMin = "";	//一般竖曲线长度
	JXLRshuMin = "";	//极限竖曲线长度
	MAXPD = "";			//最大坡度

	if (DV == 120)
	{
		MAXPD = "3";
		YBPCMin = "400";
		JXPCMin = "300";
		T_YBRshuMin = "17000";
		T_JXRShu = "11000";
		A_YBRshuMin = "6000";
		A_JXRShu = "4000";
		YBLRshuMin = "250";
		JXLRshuMin = "100";
	}
	else if (DV == 100)
	{
		MAXPD = "4";
		YBPCMin = "350";
		JXPCMin = "250";
		T_YBRshuMin = "10000";
		T_JXRShu = "6500";
		A_YBRshuMin = "4500";
		A_JXRShu = "3000";
		YBLRshuMin = "210";
		JXLRshuMin = "85";
	}
	else if (DV == 80)
	{
		MAXPD = "5";
		YBPCMin = "250";
		JXPCMin = "200";
		T_YBRshuMin = "4500";
		T_JXRShu = "3000";
		A_YBRshuMin = "3000";
		A_JXRShu = "2000";
		YBLRshuMin = "170";
		JXLRshuMin = "70";
	}
	else if (DV == 60)
	{
		MAXPD = "6";
		YBPCMin = "200";
		JXPCMin = "150";
		T_YBRshuMin = "2000";
		T_JXRShu = "1400";
		A_YBRshuMin = "1500";
		A_JXRShu = "1000";
		YBLRshuMin = "120";
		JXLRshuMin = "50";
	}
	else if (DV == 40)
	{
		MAXPD = "7";
		YBPCMin = "160";
		JXPCMin = "120";
		T_YBRshuMin = "700";
		T_JXRShu = "450";
		A_YBRshuMin = "700";
		A_JXRShu = "450";
		YBLRshuMin = "90";
		JXLRshuMin = "35";
	}
	else if (DV == 30)
	{
		MAXPD = "8";
		YBPCMin = "130";
		JXPCMin = "100";
		T_YBRshuMin = "400";
		T_JXRShu = "250";
		A_YBRshuMin = "400";
		A_JXRShu = "250";
		YBLRshuMin = "60";
		JXLRshuMin = "25";
	}
	else if (DV == 20)
	{
		MAXPD = "9";
		YBPCMin = "80";
		JXPCMin = "60";
		T_YBRshuMin = "200";
		T_JXRShu = "100";
		A_YBRshuMin = "200";
		A_JXRShu = "100";
		YBLRshuMin = "50";
		JXLRshuMin = "20";
	}

}

void BAS_DRAW_FUN::ReadRoadDV(int &DV, CString &grade)
{
	CString mdbname;

	ReadWorkdir();
	mdbname = Cworkdir;
	mdbname += "\\data\\";
	mdbname += Pro;
	mdbname += ".mdb";

	DV = 0;
	XLDataBase xlmdb;
	if (xlmdb.Read_XLDbs(mdbname, L"线路属性"))//读取线路数据库中的数据
	{
		if (xlmdb.RoadXZ.DV > 0)
			DV = xlmdb.RoadXZ.DV;
		grade = xlmdb.RoadXZ.Grade;
	}

}

/*===================================================================
函数名:SetLMKByDV
------------------------------------------------------------
函数功能:根据速度设置路面宽
------------------------------------------------------------
输入:	CString Grade	道路等级
		int		DV		速度
------------------------------------------------------------
输出:	double &fgdk	分隔带宽
		double &lydk	路缘带宽
		double &cxdk	车行道宽
		double &lmhp	路面横坡
		double &yljk	硬路肩宽
		double &yljhp	硬路肩横坡
		double &tljk	土路肩宽
		double &tljhp	土路肩横坡
		int &bkmode		变宽方式
		int &sjbgmode	设计标高位置
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805编写注释
------------------------------------------------------------
备注：
=====================================================================*/
void BAS_DRAW_FUN::SetLMKByDV(CString Grade, int DV, double &fgdk, double &lydk, double &cxdk,
	double &lmhp, double &yljk, double &yljhp, double &tljk,
	double &tljhp, int &bkmode, int &sjbgmode)
{
	bkmode = 0;//线性变宽
	if (Grade == "高速" || Grade == "一级")
		sjbgmode = 0;
	else
		sjbgmode = 1;
	sjbgmode = 0;//分隔带边缘
	//20190805	增加根据道路等级计算默认的车道数（一侧）
	double RoadWayNum = 2.0;
	if (Grade == "高速" || Grade == "一级")
		RoadWayNum = 2.0;
	else if (Grade == "二级" || Grade == "三级")
		RoadWayNum = 1.0;
	else
		RoadWayNum = 0.5;	//四级默认是单行道
	if (DV == 120)
	{
		fgdk = 1.5;
		lydk = 0.75;
		cxdk = 3.75 * RoadWayNum;
		lmhp = yljhp = tljhp = 2.0;
		yljk = 3.0;	//20190805	更新为3.0
		tljk = 0.75;
	}
	else if (DV == 100)
	{
		fgdk = 1.0;
		lydk = 0.75;
		cxdk = 3.75 * RoadWayNum;
		lmhp = yljhp = tljhp = 2.0;
		yljk = 3.0;
		tljk = 0.75;
	}
	else if (DV == 80)
	{
		fgdk = 1.0;
		lydk = 0.5;
		cxdk = 3.75* RoadWayNum;
		lmhp = yljhp = tljhp = 2.0;
		yljk = 2.5;
		tljk = 0.75;
		if (Grade == "二级")
			yljk = 1.5;
	}
	else if (DV == 60)
	{
		fgdk = 1.0;
		lydk = 0.5;
		cxdk = 3.5* RoadWayNum;
		lmhp = yljhp = tljhp = 2.0;
		yljk = 2.5;
		tljk = 0.75;
		if (Grade == "二级")
			yljk = 0.75;
	}
	else if (DV == 40)
	{
		fgdk = 0.0;
		lydk = 0.0;
		cxdk = 3.5* RoadWayNum;
		lmhp = yljhp = tljhp = 2.0;
		yljk = 0;
		tljk = 0.75;
	}
	else if (DV == 30)
	{
		fgdk = 0.0;
		lydk = 0.0;
		if (Grade == "四级")
			cxdk = 3.5* RoadWayNum;
		else
			cxdk = 3.25 * RoadWayNum;
		lmhp = yljhp = tljhp = 2.0;
		yljk = 0;
		tljk = 0.5;
	}
	else
	{
		fgdk = 0.0;
		lydk = 0.0;
		if (Grade == "四级")
			cxdk = 3.5 * RoadWayNum;
		else
			cxdk = 3.0 * RoadWayNum;
		lmhp = yljhp = tljhp = 2.0;
		yljk = 0;
		tljk = 0.25;
	}

}

//依据加宽类别取出加宽值
//20190712	修改	采用JTGD20-2017公路路线设计规范
void BAS_DRAW_FUN::FindJK(CString JKLB, int &NJK, double JK[10][2])
{
	if (JKLB == "一类加宽")//1类加宽
	{
		NJK = 9;
		JK[0][0] = 250; JK[0][1] = 0.4;
		JK[1][0] = 200; JK[1][1] = 0.5;
		JK[2][0] = 150; JK[2][1] = 0.6;
		JK[3][0] = 100; JK[3][1] = 0.7;
		JK[4][0] = 70; JK[4][1] = 0.9;
		JK[5][0] = 50; JK[5][1] = 1.3;
		JK[6][0] = 30; JK[6][1] = 1.5;
		JK[7][0] = 25; JK[7][1] = 1.8;
		JK[8][0] = 20; JK[8][1] = 2.2;
	}
	else if (JKLB == "二类加宽")//2类加宽
	{
		NJK = 6;
		JK[0][0] = 250; JK[0][1] = 0.6;
		JK[1][0] = 200; JK[1][1] = 0.7;
		JK[2][0] = 150; JK[2][1] = 0.9;
		JK[3][0] = 100; JK[3][1] = 1.2;
		JK[4][0] = 70; JK[4][1] = 1.5;
		JK[5][0] = 50; JK[5][1] = 2.0;
	}
	else  if (JKLB == "三类加宽")//2类加宽
		//3类加宽
	{
		NJK = 5;
		JK[0][0] = 250; JK[0][1] = 0.8;
		JK[1][0] = 200; JK[1][1] = 1.0;
		JK[2][0] = 150; JK[2][1] = 1.5;
		JK[3][0] = 100; JK[3][1] = 2.0;
		JK[4][0] = 70; JK[4][1] = 2.7;
	}
	else  if (JKLB == "按单车道加宽")
		//3类加宽
	{
		NJK = 8;
		JK[0][0] = 72; JK[0][1] = 0.25;
		JK[1][0] = 58; JK[1][1] = 0.5;
		JK[2][0] = 48; JK[2][1] = 0.75;
		JK[3][0] = 42; JK[3][1] = 1.0;
		JK[4][0] = 36; JK[4][1] = 1.25;
		JK[5][0] = 32; JK[5][1] = 1.5;
		JK[6][0] = 29; JK[6][1] = 1.75;
		JK[7][0] = 27; JK[7][1] = 2.0;
	}
	else
	{
		NJK = 9;
		JK[0][0] = 47; JK[0][1] = 0.25;
		JK[1][0] = 43; JK[1][1] = 0.5;
		JK[2][0] = 39; JK[2][1] = 0.75;
		JK[3][0] = 36; JK[3][1] = 1.0;
		JK[4][0] = 33; JK[4][1] = 1.25;
		JK[5][0] = 31; JK[5][1] = 1.5;
		JK[6][0] = 29; JK[6][1] = 1.75;
		JK[7][0] = 27; JK[7][1] = 2.0;
		JK[8][0] = 26; JK[8][1] = 2.25;

	}
}

//依据车速取出超高值
void BAS_DRAW_FUN::FindSuphI(int DV, double MAXCG, double Ig, int &NSuphI, double SuphI[10][2])
{
	if (DV == 120)
	{
		if (fabs(MAXCG - 10) < 0.1)
		{
			NSuphI = 10;
			if (Ig > 2.1)
				SuphI[0][0] = 7550;
			else
				SuphI[0][0] = 5500;
			SuphI[0][1] = 2;

			SuphI[1][0] = 2950;	SuphI[1][1] = 3;

			SuphI[2][0] = 2080;	SuphI[2][1] = 4;

			SuphI[3][0] = 1590;	SuphI[3][1] = 5;

			SuphI[4][0] = 1280;	SuphI[4][1] = 6;

			SuphI[5][0] = 1070;	SuphI[5][1] = 7;

			SuphI[6][0] = 910;	SuphI[6][1] = 8;

			SuphI[7][0] = 790;	SuphI[7][1] = 9;

			SuphI[8][0] = 680;	SuphI[8][1] = 10;

			SuphI[9][0] = 570;	SuphI[9][1] = 10;
		}
		else if (fabs(MAXCG - 8) < 0.1)
		{
			NSuphI = 8;
			if (Ig > 2.1)
				SuphI[0][0] = 7550;
			else
				SuphI[0][0] = 5500;
			SuphI[0][1] = 2;

			SuphI[1][0] = 2860;	SuphI[1][1] = 3;

			SuphI[2][0] = 1990;	SuphI[2][1] = 4;

			SuphI[3][0] = 1500;	SuphI[3][1] = 5;

			SuphI[4][0] = 1190;	SuphI[4][1] = 6;

			SuphI[5][0] = 980;	SuphI[5][1] = 7;

			SuphI[6][0] = 790;	SuphI[6][1] = 8;

			SuphI[7][0] = 650;	SuphI[7][1] = 8;
		}
		else if (fabs(MAXCG - 6) < 0.1)
		{
			NSuphI = 6;
			if (Ig > 2.1)
				SuphI[0][0] = 7550;
			else
				SuphI[0][0] = 5500;
			SuphI[0][1] = 2;

			SuphI[1][0] = 2730;	SuphI[1][1] = 3;

			SuphI[2][0] = 1840;	SuphI[2][1] = 4;

			SuphI[3][0] = 1340;	SuphI[3][1] = 5;

			SuphI[4][0] = 970;	SuphI[4][1] = 6;

			SuphI[5][0] = 710;	SuphI[5][1] = 6;
		}

	}
	if (DV == 100)
	{
		if (fabs(MAXCG - 10) < 0.1)
		{
			NSuphI = 10;
			if (Ig > 2.1)
				SuphI[0][0] = 5250;
			else
				SuphI[0][0] = 4000;
			SuphI[0][1] = 2;
			SuphI[1][0] = 2180;	SuphI[1][1] = 3;
			SuphI[2][0] = 1520;	SuphI[2][1] = 4;
			SuphI[3][0] = 1160;	SuphI[3][1] = 5;
			SuphI[4][0] = 920;	SuphI[4][1] = 6;
			SuphI[5][0] = 760;	SuphI[5][1] = 7;
			SuphI[6][0] = 640;	SuphI[6][1] = 8;
			SuphI[7][0] = 540;	SuphI[7][1] = 9;
			SuphI[8][0] = 450;	SuphI[8][1] = 10;
			SuphI[9][0] = 340;	SuphI[9][1] = 10;
		}
		else if (fabs(MAXCG - 8) < 0.1)
		{
			NSuphI = 8;
			if (Ig > 2.1)
				SuphI[0][0] = 5250;
			else
				SuphI[0][0] = 4000;
			SuphI[0][1] = 2;

			SuphI[1][0] = 2150;	SuphI[1][1] = 3;
			SuphI[2][0] = 1480;	SuphI[2][1] = 4;
			SuphI[3][0] = 1100;	SuphI[3][1] = 5;
			SuphI[4][0] = 860;	SuphI[4][1] = 6;
			SuphI[5][0] = 690;	SuphI[5][1] = 7;
			SuphI[6][0] = 530;	SuphI[6][1] = 8;
			SuphI[7][0] = 400;	SuphI[7][1] = 8;
		}
		else if (fabs(MAXCG - 6) < 0.1)
		{
			NSuphI = 6;
			if (Ig > 2.1)
				SuphI[0][0] = 5250;
			else
				SuphI[0][0] = 4000;
			SuphI[0][1] = 2;

			SuphI[1][0] = 2000;	SuphI[1][1] = 3;
			SuphI[2][0] = 1320;	SuphI[2][1] = 4;
			SuphI[3][0] = 920;	SuphI[3][1] = 5;
			SuphI[4][0] = 630;	SuphI[4][1] = 6;
			SuphI[5][0] = 440;	SuphI[5][1] = 6;
		}
	}
	if (DV == 80)
	{
		if (fabs(MAXCG - 10) < 0.1)
		{
			NSuphI = 10;
			if (Ig > 2.1)
				SuphI[0][0] = 3350;
			else
				SuphI[0][0] = 2500;
			SuphI[0][1] = 2;
			SuphI[1][0] = 1460;	SuphI[1][1] = 3;
			SuphI[2][0] = 1020;	SuphI[2][1] = 4;
			SuphI[3][0] = 770;	SuphI[3][1] = 5;
			SuphI[4][0] = 610;	SuphI[4][1] = 6;
			SuphI[5][0] = 500;	SuphI[5][1] = 7;
			SuphI[6][0] = 410;	SuphI[6][1] = 8;
			SuphI[7][0] = 340;	SuphI[7][1] = 9;
			SuphI[8][0] = 280;	SuphI[8][1] = 10;
			SuphI[9][0] = 220;	SuphI[9][1] = 10;
		}
		else if (fabs(MAXCG - 8) < 0.1)
		{
			NSuphI = 8;
			if (Ig > 2.1)
				SuphI[0][0] = 3350;
			else
				SuphI[0][0] = 2500;
			SuphI[0][1] = 2;

			SuphI[1][0] = 1410;	SuphI[1][1] = 3;
			SuphI[2][0] = 960;	SuphI[2][1] = 4;
			SuphI[3][0] = 710;	SuphI[3][1] = 5;
			SuphI[4][0] = 550;	SuphI[4][1] = 6;
			SuphI[5][0] = 420;	SuphI[5][1] = 7;
			SuphI[6][0] = 320;	SuphI[6][1] = 8;
			SuphI[7][0] = 250;	SuphI[7][1] = 8;
		}
		else if (fabs(MAXCG - 6) < 0.1)
		{
			NSuphI = 6;
			if (Ig > 2.1)
				SuphI[0][0] = 3350;
			else
				SuphI[0][0] = 2500;
			SuphI[0][1] = 2;

			SuphI[1][0] = 1360;	SuphI[1][1] = 3;
			SuphI[2][0] = 890;	SuphI[2][1] = 4;
			SuphI[3][0] = 600;	SuphI[3][1] = 5;
			SuphI[4][0] = 400;	SuphI[4][1] = 6;
			SuphI[5][0] = 270;	SuphI[5][1] = 6;
		}
	}
	if (DV == 60)
	{
		if (fabs(MAXCG - 10) < 0.1)
		{
			NSuphI = 10;
			if (Ig > 2.1)
				SuphI[0][0] = 1900;
			else
				SuphI[0][0] = 1500;
			SuphI[0][1] = 2;
			SuphI[1][0] = 900;	SuphI[1][1] = 3;
			SuphI[2][0] = 620;	SuphI[2][1] = 4;
			SuphI[3][0] = 470;	SuphI[3][1] = 5;
			SuphI[4][0] = 360;	SuphI[4][1] = 6;
			SuphI[5][0] = 290;	SuphI[5][1] = 7;
			SuphI[6][0] = 240;	SuphI[6][1] = 8;
			SuphI[7][0] = 190;	SuphI[7][1] = 9;
			SuphI[8][0] = 150;	SuphI[8][1] = 10;
			SuphI[9][0] = 115;	SuphI[9][1] = 10;
		}
		else if (fabs(MAXCG - 8) < 0.1)
		{
			NSuphI = 8;
			if (Ig > 2.1)
				SuphI[0][0] = 1900;
			else
				SuphI[0][0] = 1500;
			SuphI[0][1] = 2;

			SuphI[1][0] = 870;	SuphI[1][1] = 3;
			SuphI[2][0] = 590;	SuphI[2][1] = 4;
			SuphI[3][0] = 430;	SuphI[3][1] = 5;
			SuphI[4][0] = 320;	SuphI[4][1] = 6;
			SuphI[5][0] = 240;	SuphI[5][1] = 7;
			SuphI[6][0] = 170;	SuphI[6][1] = 8;
			SuphI[7][0] = 125;	SuphI[7][1] = 8;
		}
		else if (fabs(MAXCG - 6) < 0.1)
		{
			NSuphI = 6;
			if (Ig > 2.1)
				SuphI[0][0] = 1900;
			else
				SuphI[0][0] = 1500;
			SuphI[0][1] = 2;

			SuphI[1][0] = 800;	SuphI[1][1] = 3;
			SuphI[2][0] = 500;	SuphI[2][1] = 4;
			SuphI[3][0] = 320;	SuphI[3][1] = 5;
			SuphI[4][0] = 200;	SuphI[4][1] = 6;
			SuphI[5][0] = 135;	SuphI[5][1] = 6;
		}
		else if (fabs(MAXCG - 4) < 0.1)
		{
			NSuphI = 4;
			if (Ig > 2.1)
				SuphI[0][0] = 1900;
			else
				SuphI[0][0] = 1500;
			SuphI[0][1] = 2;

			SuphI[1][0] = 610;	SuphI[1][1] = 3;
			SuphI[2][0] = 270;	SuphI[2][1] = 4;
			SuphI[3][0] = 150;	SuphI[3][1] = 4;
		}
	}
	if (DV == 40)
	{
		if (fabs(MAXCG - 8) < 0.1)
		{
			NSuphI = 8;
			if (Ig > 2.1)
				SuphI[0][0] = 800;
			else
				SuphI[0][0] = 600;
			SuphI[0][1] = 2;

			SuphI[1][0] = 470;	SuphI[1][1] = 3;
			SuphI[2][0] = 310;	SuphI[2][1] = 4;
			SuphI[3][0] = 220;	SuphI[3][1] = 5;
			SuphI[4][0] = 160;	SuphI[4][1] = 6;
			SuphI[5][0] = 120;	SuphI[5][1] = 7;
			SuphI[6][0] = 80;	SuphI[6][1] = 8;
			SuphI[7][0] = 55;	SuphI[7][1] = 8;
		}
		else if (fabs(MAXCG - 6) < 0.1)
		{
			NSuphI = 6;
			if (Ig > 2.1)
				SuphI[0][0] = 800;
			else
				SuphI[0][0] = 600;
			SuphI[0][1] = 2;

			SuphI[1][0] = 410;	SuphI[1][1] = 3;
			SuphI[2][0] = 250;	SuphI[2][1] = 4;
			SuphI[3][0] = 150;	SuphI[3][1] = 5;
			SuphI[4][0] = 90;	SuphI[4][1] = 6;
			SuphI[5][0] = 60;	SuphI[5][1] = 6;

		}
		else if (fabs(MAXCG - 4) < 0.1)
		{
			NSuphI = 4;
			if (Ig > 2.1)
				SuphI[0][0] = 800;
			else
				SuphI[0][0] = 600;
			SuphI[0][1] = 2;

			SuphI[1][0] = 330;	SuphI[1][1] = 3;
			SuphI[2][0] = 130;	SuphI[2][1] = 4;
			SuphI[3][0] = 70;	SuphI[3][1] = 4;
		}
		else if (fabs(MAXCG - 2) < 0.1)
		{
			NSuphI = 2;
			if (Ig > 2.1)
				SuphI[0][0] = 800;
			else
				SuphI[0][0] = 600;
			SuphI[0][1] = 2;

			SuphI[1][0] = 75;	SuphI[1][1] = 2;
		}
	}
	if (DV == 30)
	{
		if (fabs(MAXCG - 8) < 0.1)
		{
			NSuphI = 8;
			if (Ig > 2.1)
				SuphI[0][0] = 450;
			else
				SuphI[0][0] = 350;
			SuphI[0][1] = 2;

			SuphI[1][0] = 250;	SuphI[1][1] = 3;
			SuphI[2][0] = 170;	SuphI[2][1] = 4;
			SuphI[3][0] = 120;	SuphI[3][1] = 5;
			SuphI[4][0] = 90;	SuphI[4][1] = 6;
			SuphI[5][0] = 60;	SuphI[5][1] = 7;
			SuphI[6][0] = 40;	SuphI[6][1] = 8;
			SuphI[7][0] = 30;	SuphI[7][1] = 8;
		}
		else if (fabs(MAXCG - 6) < 0.1)
		{
			NSuphI = 6;
			if (Ig > 2.1)
				SuphI[0][0] = 450;
			else
				SuphI[0][0] = 350;
			SuphI[0][1] = 2;

			SuphI[1][0] = 230;	SuphI[1][1] = 3;
			SuphI[2][0] = 140;	SuphI[2][1] = 4;
			SuphI[3][0] = 80;	SuphI[3][1] = 5;
			SuphI[4][0] = 50;	SuphI[4][1] = 6;
			SuphI[5][0] = 35;	SuphI[5][1] = 6;

		}
		else if (fabs(MAXCG - 4) < 0.1)
		{
			NSuphI = 4;
			if (Ig > 2.1)
				SuphI[0][0] = 450;
			else
				SuphI[0][0] = 350;
			SuphI[0][1] = 2;

			SuphI[1][0] = 150;	SuphI[1][1] = 3;
			SuphI[2][0] = 60;	SuphI[2][1] = 4;
			SuphI[3][0] = 35;	SuphI[3][1] = 4;
		}
		else if (fabs(MAXCG - 2) < 0.1)
		{
			NSuphI = 2;
			if (Ig > 2.1)
				SuphI[0][0] = 450;
			else
				SuphI[0][0] = 350;
			SuphI[0][1] = 2;

			SuphI[1][0] = 40;	SuphI[1][1] = 2;
		}
	}
	if (DV == 20)
	{
		if (fabs(MAXCG - 8) < 0.1)
		{
			NSuphI = 8;
			if (Ig > 2.1)
				SuphI[0][0] = 200;
			else
				SuphI[0][0] = 150;
			SuphI[0][1] = 2;

			SuphI[1][0] = 140;	SuphI[1][1] = 3;
			SuphI[2][0] = 90;	SuphI[2][1] = 4;
			SuphI[3][0] = 70;	SuphI[3][1] = 5;
			SuphI[4][0] = 50;	SuphI[4][1] = 6;
			SuphI[5][0] = 40;	SuphI[5][1] = 7;
			SuphI[6][0] = 30;	SuphI[6][1] = 8;
			SuphI[7][0] = 15;	SuphI[7][1] = 8;
		}
		else if (fabs(MAXCG - 6) < 0.1)
		{
			NSuphI = 6;
			if (Ig > 2.1)
				SuphI[0][0] = 200;
			else
				SuphI[0][0] = 150;
			SuphI[0][1] = 2;

			SuphI[1][0] = 110;	SuphI[1][1] = 3;
			SuphI[2][0] = 70;	SuphI[2][1] = 4;
			SuphI[3][0] = 40;	SuphI[3][1] = 5;
			SuphI[4][0] = 30;	SuphI[4][1] = 6;
			SuphI[5][0] = 15;	SuphI[5][1] = 6;
		}
		else if (fabs(MAXCG - 4) < 0.1)
		{
			NSuphI = 4;
			if (Ig > 2.1)
				SuphI[0][0] = 200;
			else
				SuphI[0][0] = 150;
			SuphI[0][1] = 2;

			SuphI[1][0] = 70;	SuphI[1][1] = 3;
			SuphI[2][0] = 30;	SuphI[2][1] = 4;
			SuphI[3][0] = 15;	SuphI[3][1] = 4;

		}
		else if (fabs(MAXCG - 2) < 0.1)
		{
			NSuphI = 2;
			if (Ig > 2.1)
				SuphI[0][0] = 200;
			else
				SuphI[0][0] = 150;
			SuphI[0][1] = 2;

			SuphI[1][0] = 20;	SuphI[1][1] = 2;
		}
	}
}

//依据车速取出匝道超高值
void BAS_DRAW_FUN::FindZD_SuphI(int DV, int &NSuphI, double SuphI[10][2])
{
	if (DV == 80)
	{

		NSuphI = 9;

		SuphI[0][0] = 3500;	  SuphI[0][1] = 2;

		SuphI[1][0] = 1240;	  SuphI[1][1] = 3;

		SuphI[2][0] = 870;	SuphI[2][1] = 4;

		SuphI[3][0] = 670;	SuphI[3][1] = 5;

		SuphI[4][0] = 540;	SuphI[4][1] = 6;

		SuphI[5][0] = 450;	SuphI[5][1] = 7;

		SuphI[6][0] = 380;	SuphI[6][1] = 8;

		SuphI[7][0] = 330;	SuphI[7][1] = 9;

		SuphI[8][0] = 280;	SuphI[8][1] = 10;

	}
	if (DV == 70)
	{
		NSuphI = 9;

		SuphI[0][0] = 2600;	  SuphI[0][1] = 2;

		SuphI[1][0] = 1000;	  SuphI[1][1] = 3;

		SuphI[2][0] = 700;	SuphI[2][1] = 4;

		SuphI[3][0] = 550;	SuphI[3][1] = 5;

		SuphI[4][0] = 430;	SuphI[4][1] = 6;

		SuphI[5][0] = 350;	SuphI[5][1] = 7;

		SuphI[6][0] = 300;	SuphI[6][1] = 8;

		SuphI[7][0] = 250;	SuphI[7][1] = 9;

		SuphI[8][0] = 210;	SuphI[8][1] = 10;

	}
	if (DV == 60)
	{
		NSuphI = 9;

		SuphI[0][0] = 2000;	  SuphI[0][1] = 2;

		SuphI[1][0] = 800;	  SuphI[1][1] = 3;

		SuphI[2][0] = 560;	SuphI[2][1] = 4;

		SuphI[3][0] = 420;	SuphI[3][1] = 5;

		SuphI[4][0] = 330;	SuphI[4][1] = 6;

		SuphI[5][0] = 270;	SuphI[5][1] = 7;

		SuphI[6][0] = 220;	SuphI[6][1] = 8;

		SuphI[7][0] = 180;	SuphI[7][1] = 9;

		SuphI[8][0] = 140;	SuphI[8][1] = 10;
	}
	if (DV == 50)
	{
		NSuphI = 9;

		SuphI[0][0] = 1300;	  SuphI[0][1] = 2;

		SuphI[1][0] = 590;	  SuphI[1][1] = 3;

		SuphI[2][0] = 410;	SuphI[2][1] = 4;

		SuphI[3][0] = 310;	SuphI[3][1] = 5;

		SuphI[4][0] = 240;	SuphI[4][1] = 6;

		SuphI[5][0] = 200;	SuphI[5][1] = 7;

		SuphI[6][0] = 160;	SuphI[6][1] = 8;

		SuphI[7][0] = 120;	SuphI[7][1] = 9;

		SuphI[8][0] = 90;	SuphI[8][1] = 10;
	}
	if (DV == 40)
	{
		NSuphI = 9;

		SuphI[0][0] = 800;	  SuphI[0][1] = 2;

		SuphI[1][0] = 400;	  SuphI[1][1] = 3;

		SuphI[2][0] = 280;	SuphI[2][1] = 4;

		SuphI[3][0] = 210;	SuphI[3][1] = 5;

		SuphI[4][0] = 160;	SuphI[4][1] = 6;

		SuphI[5][0] = 130;	SuphI[5][1] = 7;

		SuphI[6][0] = 90;	SuphI[6][1] = 8;

		SuphI[7][0] = 70;	SuphI[7][1] = 9;

		SuphI[8][0] = 50;	SuphI[8][1] = 10;
	}
	if (DV == 35)
	{
		NSuphI = 9;

		SuphI[0][0] = 650;	  SuphI[0][1] = 2;

		SuphI[1][0] = 280;	  SuphI[1][1] = 3;

		SuphI[2][0] = 220;	SuphI[2][1] = 4;

		SuphI[3][0] = 140;	SuphI[3][1] = 5;

		SuphI[4][0] = 110;	SuphI[4][1] = 6;

		SuphI[5][0] = 90;	SuphI[5][1] = 7;

		SuphI[6][0] = 60;	SuphI[6][1] = 8;

		SuphI[7][0] = 50;	SuphI[7][1] = 9;

		SuphI[8][0] = 40;	SuphI[8][1] = 10;
	}
	if (DV == 30)
	{
		NSuphI = 7;

		SuphI[0][0] = 500;	  SuphI[0][1] = 2;

		SuphI[1][0] = 220;	  SuphI[1][1] = 3;

		SuphI[2][0] = 150;	SuphI[2][1] = 4;

		SuphI[3][0] = 110;	SuphI[3][1] = 5;

		SuphI[4][0] = 80;	SuphI[4][1] = 6;

		SuphI[5][0] = 60;	SuphI[5][1] = 7;

		SuphI[6][0] = 40;	SuphI[6][1] = 8;


	}
}

//直线超高渐变率
double  BAS_DRAW_FUN::FindZXCGJBL(int DV, int mode)
{
	double CGJBL;
	if (DV == 120)
	{
		if (mode == 0)//绕中线
			CGJBL = 250;
		else
			CGJBL = 220;
	}
	else if (DV == 100)
	{
		if (mode == 0)//绕中线
			CGJBL = 225;
		else
			CGJBL = 175;
	}
	else if (DV == 80)
	{
		if (mode == 0)//绕中线
			CGJBL = 200;
		else
			CGJBL = 150;
	}
	else if (DV == 60)
	{
		if (mode == 0)//绕中线
			CGJBL = 175;
		else
			CGJBL = 125;
	}
	else if (DV == 40)
	{
		if (mode == 0)//绕中线
			CGJBL = 150;
		else
			CGJBL = 100;
	}
	else if (DV == 30)
	{
		if (mode == 0)//绕中线
			CGJBL = 125;
		else
			CGJBL = 75;
	}
	else if (DV == 20)
	{
		if (mode == 0)//绕中线
			CGJBL = 100;
		else
			CGJBL = 50;
	}
	else
		CGJBL = 225;

	return CGJBL;
}
//-------------------------------------------------------------------------
//直线拟合
//--------------------------------------------------------------------------
void BAS_DRAW_FUN::fit(double x[], double y[], int ndata, double sig[], int mwt, double& a,
	double& b, double& siga, double& sigb, double& chi2, double& q)
{
	int i;
	double sigdat, t, sxoss, wt, ss, sx = 0.0;
	double sy = 0.0;
	double st2 = 0.0;
	b = 0.0;
	if (mwt != 0)
	{
		ss = 0.0;
		for (i = 1; i <= ndata; i++)
		{
			wt = 1.0 / (sig[i] * sig[i]);
			ss = ss + wt;
			sx = sx + x[i] * wt;
			sy = sy + y[i] * wt;
		}
	}
	else
	{
		for (i = 1; i <= ndata; i++)
		{
			sx = sx + x[i];
			sy = sy + y[i];
		}
		ss = ndata;
	}
	sxoss = sx / ss;
	if (mwt != 0)
	{
		for (i = 1; i <= ndata; i++)
		{
			t = (x[i] - sxoss) / sig[i];
			st2 = st2 + t * t;
			b = b + t * y[i] / sig[i];
		}
	}
	else
	{
		for (i = 1; i <= ndata; i++)
		{
			t = x[i] - sxoss;
			st2 = st2 + t * t;
			b = b + t * y[i];
		}
	}
	b = b / st2;
	a = (sy - sx * b) / ss;
	siga = sqrt((1.0 + sx * sx / (ss * st2)) / ss);
	sigb = sqrt(1.0 / st2);
	chi2 = 0.0;
	if (mwt == 0)
	{
		for (i = 1; i <= ndata; i++)
		{
			chi2 = chi2 + pow((y[i] - a - b * x[i]), 2);
		}
		q = 1.0;
		sigdat = sqrt(chi2 / (ndata - 2));
		siga = siga * sigdat;
		sigb = sigb * sigdat;
	}
	else
	{
		for (i = 1; i <= ndata; i++)
		{
			chi2 = chi2 + pow(((y[i] - a - b * x[i]) / sig[i]), 2);
		}
		q = gammq(0.5 * (ndata - 2), 0.5 * chi2);
	}
}
double BAS_DRAW_FUN::gammq(double a, double x)
{
	double temp, gamser, gammcf, gln;
	if (x < 0.0 || a <= 0.0)
	{
		// cout<< "pause";
		exit(1);
	}
	if (x < a + 1.0)
	{
		gser(gamser, a, x, gln);
		temp = 1.0 - gamser;
	}
	else
	{
		gcf(gammcf, a, x, gln);
		temp = gammcf;
	}
	return temp;
}

void BAS_DRAW_FUN::gser(double& gamser, double& a, double& x, double& gln)
{
	int itmax, n;
	double ap, sum, del, eps;
	itmax = 100;
	eps = 0.0000003;
	gln = gammln(a);
	if (x <= 0.0)
	{
		if (x < 0.0)
		{
			//  cout<<" pause";
			exit(1);
		}
		gamser = 0.0;
		exit(1);
	}
	ap = a;
	sum = 1.0 / a;
	del = sum;
	for (n = 1; n <= itmax; n++)
	{
		ap = ap + 1.0;
		del = del * x / ap;
		sum = sum + del;
		if (fabs(del) < (fabs(sum) * eps))
		{//	goto loop;
			gamser = sum * exp(-x + a * log(x) - gln);
			break;
		}
	}
}

void BAS_DRAW_FUN::gcf(double& gammcf, double& a, double& x, double& gln)
{
	int itmax, n;
	double eps, a0, a1, b0, b1, fac, an, ana, anf, gold, g;
	itmax = 100;
	eps = 0.0000003;
	gln = gammln(a);
	gold = 0.0;
	a0 = 1.0;
	a1 = x;
	b0 = 0.0;
	b1 = 1.0;
	fac = 1.0;
	for (n = 1; n <= itmax; n++)
	{
		an = n;
		ana = an - a;
		a0 = (a1 + a0 * ana) * fac;
		b0 = (b1 + b0 * ana) * fac;
		anf = an * fac;
		a1 = x * a0 + anf * a1;
		b1 = x * b0 + anf * b1;
		if (a1 != 0.0)
		{
			fac = 1.0 / a1;
			g = b1 * fac;
			if (fabs((g - gold) / g) < eps)
			{
				gammcf = exp(-x + a * log(x) - gln) * g;
				break;
			}
			gold = g;
		}
	}
}

double BAS_DRAW_FUN::gammln(double xx)
{
	int j;
	double temp;
	double cof[6], stp, half, one, fpf, x, tmp, ser;
	cof[1] = 76.18009173;
	cof[2] = -86.50532033;
	cof[3] = 24.01409822;
	cof[4] = -1.231739516;
	cof[5] = 0.00120858003;
	cof[6] = -0.00000536382;
	stp = 2.50662827465;
	half = 0.5;
	one = 1.0;
	fpf = 5.5;
	x = xx - one;
	tmp = x + fpf;
	tmp = (x + half) * log(tmp) - tmp;
	ser = one;
	for (j = 1; j <= 6; j++)
	{
		x = x + one;
		ser = ser + cof[j] / x;
	}
	temp = tmp + log(stp * ser);
	return temp;
}

void BAS_DRAW_FUN::medfit(double x[], double y[], int ndata, double& a, double& b, double& abdev)
{
	int j;
	double bb, del, chisq, f, sx = 0.0;
	double sigb, b1, b2, f1, f2, sy = 0.0;
	double sxy = 0.0;
	double sxx = 0.0;
	double abdevt;
	double aa;

	for (j = 1; j <= ndata; j++)
	{
		sx = sx + x[j];
		sy = sy + y[j];
		sxy = sxy + x[j] * y[j];
		sxx = sxx + x[j] * x[j];
	}
	//  ndatat = ndata;
	del = ndata * sxx - sx * sx;
	aa = (sxx * sy - sx * sxy) / del;
	bb = (ndata * sxy - sx * sy) / del;
	chisq = 0.0;
	for (j = 1; j <= ndata; j++)
	{
		chisq = chisq + pow((y[j] - (aa + bb * x[j])), 2);
	}
	sigb = sqrt(chisq / del);
	b1 = bb;

	f1 = rofunc(b1, x, y, ndata, abdevt);
	b2 = bb + fabs(3 * sigb) * sgn(f1);
	f2 = rofunc(b2, x, y, ndata, abdevt);
	while (f1 * f2 > 0.0)
	{
		bb = 2.0 * b2 - b1;
		b1 = b2;
		f1 = f2;
		b2 = bb;
		f2 = rofunc(b2, x, y, ndata, abdevt);
	}
	sigb = 0.01 * sigb;
	while (fabs(b2 - b1) > sigb)
	{
		bb = 0.5 * (b1 + b2);
		if (bb == b1 || bb == b2)
		{
			break;
		}
		f = rofunc(bb, x, y, ndata, abdevt);
		if (f * f1 >= 0.0)
		{
			f1 = f;
			b1 = bb;
		}
		else
		{
			f2 = f;
			b2 = bb;
		}
	}
	a = aa;
	b = bb;
	abdev = abdevt / ndata;
}
double BAS_DRAW_FUN::rofunc(double b, double xt[], double yt[], int ndatat, double &abdevt)
{
	int n1, j;
	double nml, nmh;
	double d, aa;
	double arr[1000];

	n1 = ndatat + 1;
	nml = (double)n1 / 2;
	nmh = n1 - nml;
	for (j = 1; j <= ndatat; j++)
	{
		arr[j] = yt[j] - b * xt[j];
	}
	sort(ndatat, arr);
	aa = 0.5 * (arr[(int)nml] + arr[(int)nmh]);
	double sum = 0.0;
	abdevt = 0.0;
	for (j = 1; j <= ndatat; j++)
	{
		d = yt[j] - (b * xt[j] + aa);
		abdevt = abdevt + fabs(d);
		sum = sum + xt[j] * sgn(d);
	}
	return sum;
}
void  BAS_DRAW_FUN::sort(int n, double ra[])
{
	int i, j, l, ir;
	double rra;
	l = int(n / 2) + 1;
	ir = n;
	do
	{
		if (l > 1)
		{
			l = l - 1;
			rra = ra[l];
		}
		else
		{
			rra = ra[ir];
			ra[ir] = ra[1];
			ir = ir - 1;
			if (ir == 1)
			{
				ra[1] = rra;
				return;
			}
		}
		i = l;
		j = l + l;
		while (j <= ir)
		{
			if (j < ir)
			{
				if (ra[j] < ra[j + 1])
				{
					j += 1;
				}
			}
			if (rra < ra[j])
			{
				ra[i] = ra[j];
				i = j;
				j += j;
			}
			else
			{
				j = ir + 1;
			}
		}
		ra[i] = rra;
	} while (1);
}

int BAS_DRAW_FUN::sgn(double number)
{
	if (fabs(number) < 0.0000001)
		return 0;
	else if (number > 0.0)
		return 1;
	else if (number < 0.0)
		return -1;
	return 0;
}


AcDbObjectId  BAS_DRAW_FUN::make2dline(AcGePoint3dArray ptarray, int icolor, double startWid, double EndWid, AcDbObjectId LayerId)
{
	AcDb2dPolyline *pLine = new AcDb2dPolyline(AcDb::k2dSimplePoly, ptarray, 0, Adesk::kFalse, startWid, EndWid);
	pLine->setColorIndex(icolor);
	pLine->setLayer(LayerId);
	AcDbObjectId  LId;
	LId = AddEntityToDbs(pLine);
	return LId;
}

AcDbObjectId BAS_DRAW_FUN::H_maketext(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, int icolor, int mode, ACHAR* textStyle, AcDbObjectId layerId)
{

	AcDbObjectId  TId;
	AcDbText *pText = new AcDbText;
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl, AcDb::kForWrite);
	if (pTextStyleTbl->getAt(textStyle, pTextStyleTblId) != Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd = new AcDbTextStyleTableRecord;
		pTextStyleTblRcd->setName(textStyle);
		pTextStyleTblRcd->setFileName(textStyle);
		pTextStyleTblRcd->setFont(textStyle, 0, 0, 134, 2);
		pTextStyleTblRcd->setXScale(0.8);
		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		pTextStyleTbl->getAt(textStyle, pTextStyleTblId);
	}
	pTextStyleTbl->close();
	if (mode == 1) // 左对齐
	{
		pText->setHorizontalMode(AcDb::kTextLeft);
	}
	else if (mode == 2) // // 中对齐
	{
		pText->setHorizontalMode(AcDb::kTextCenter);
	}
	else // // 右对齐
	{
		pText->setHorizontalMode(AcDb::kTextRight);
	};
	pText->setColorIndex(icolor);
	if (mode == 1) { pText->setPosition(pos); }
	else { pText->setAlignmentPoint(pos); };
	//  ads_printf(L"\n   m        Pt=%10.3f   %10.3f   %10.4f %s",pos[X],pos[Y],dms_rad(ang),ctext);  
	pText->setTextStyle(pTextStyleTblId);
	pText->setRotation(ang);
	pText->setHeight(texth);
	pText->setTextString(ctext);
	pText->setLayer(layerId);
	TId = AddEntityToDbs(pText);
	return TId;
}

void BAS_DRAW_FUN::maketable(struct Table& tab, int icolor, double startWid, double EndWid, ACHAR* LayerName)
{
	int i, j;
	AcGePoint3d startPt, endPt;
	double ht = 0, wd = 0;
	AcGePoint3dArray PtArr;

	PtArr.setLogicalLength(2);
	PtArr[0].set(tab.LDpt[X], tab.LDpt[Y], 0);
	for (i = 0; i < tab.ncol; i++)
		wd += tab.ColWid[i];
	PtArr[1].set(tab.LDpt[X] + wd, tab.LDpt[Y], 0);
	make2dline(PtArr, icolor, startWid, EndWid, LayerName);
	for (i = 0; i < tab.nrow; i++)//draw row line
	{
		ht += tab.RowH[i];
		PtArr[0].set(tab.LDpt[X], tab.LDpt[Y] + ht, 0);
		PtArr[1].set(tab.LDpt[X] + wd, tab.LDpt[Y] + ht, 0);
		make2dline(PtArr, icolor, startWid, EndWid, LayerName);
	}

	wd = 0.0;
	PtArr[0].set(tab.LDpt[X], tab.LDpt[Y], 0);
	PtArr[1].set(tab.LDpt[X], tab.LDpt[Y] + ht, 0);
	make2dline(PtArr, icolor, startWid, EndWid, LayerName);
	for (i = 0; i < tab.ncol; i++)//draw column line
	{
		wd += tab.ColWid[i];
		PtArr[0].set(tab.LDpt[X] + wd, tab.LDpt[Y], 0);
		PtArr[1].set(tab.LDpt[X] + wd, tab.LDpt[Y] + ht, 0);
		make2dline(PtArr, icolor, startWid, EndWid, LayerName);
	}
	AcGePoint3d pos, lpos, rpos;
	ACHAR str[80], str1[80], str2[80];
	for (i = 0; i < tab.Nstr; i++)
	{
		wd = ht = 0;
		for (j = 0; j < tab.Str[i].rownum; j++)
			ht += tab.RowH[j];
		pos[Y] = tab.LDpt[Y] + ht - tab.RowH[j - 1] / 2.0;
		for (j = 0; j < tab.Str[i].colnum; j++)
			wd += tab.ColWid[j];
		pos[X] = tab.LDpt[X] + wd - tab.ColWid[j - 1] / 2.0;
		pos[Z] = 0;
		if (tab.Str[i].ntext == 1)//只有一行文本
		{
			_tcscpy(str, tab.Str[i].text);
			if (tab.Str[i].fitinf)
			{
				lpos[X] = pos[X] - tab.ColWid[tab.Str[i].colnum - 1] / 2.0, lpos[Y] = pos[Y] - (tab.RowH[tab.Str[i].rownum - 1] - tab.Str[i].texth) / tab.Str[i].dv, lpos[Z] = 0;
				rpos[X] = pos[X] + tab.ColWid[tab.Str[i].colnum - 1] / 2.0, rpos[Y] = pos[Y] - (tab.RowH[tab.Str[i].rownum - 1] - tab.Str[i].texth) / tab.Str[i].dv, rpos[Z] = 0;
				maketext(lpos, rpos, str, 0, tab.Str[i].texth, 2, L"宋体", L"simplex.shx", L"hzdx.shx", LayerName);
			}
			else
				maketext(pos, str, 0, tab.Str[i].texth, 2, 3, L"宋体", LayerName);
		}
		else if (tab.Str[i].ntext == 2)//两行文本
		{
			_tcscpy(str, tab.Str[i].text);
			_tcscpy(str1, tab.Str[i].text1);
			lpos[X] = pos[X], lpos[Y] = pos[Y] + tab.RowH[tab.Str[i].rownum - 1] / 4, lpos[Z] = 0;
			rpos[X] = pos[X], rpos[Y] = pos[Y] - tab.RowH[tab.Str[i].rownum - 1] / 4, rpos[Z] = 0;
			maketext(lpos, str, 0, tab.Str[i].texth, 2, 3, L"宋体", LayerName);
			maketext(rpos, str1, 0, tab.Str[i].texth, 2, 3, L"宋体", LayerName);
		}
		else//三行文本
		{
			_tcscpy(str, tab.Str[i].text);
			_tcscpy(str1, tab.Str[i].text1);
			_tcscpy(str2, tab.Str[i].text2);
			lpos[X] = pos[X], lpos[Y] = pos[Y] + tab.RowH[tab.Str[i].rownum - 1] / 3, lpos[Z] = 0;
			rpos[X] = pos[X], rpos[Y] = pos[Y] - tab.RowH[tab.Str[i].rownum - 1] / 3, rpos[Z] = 0;
			maketext(pos, str, 0, tab.Str[i].texth, 2, 3, L"宋体", LayerName);
			maketext(lpos, str1, 0, tab.Str[i].texth, 2, 3, L"宋体", LayerName);
			maketext(rpos, str2, 0, tab.Str[i].texth, 2, 3, L"宋体", LayerName);
		}
	}
}

// 绘文本
AcDbObjectId BAS_DRAW_FUN::maketext(AcGePoint3d lpos, AcGePoint3d rpos, ACHAR* ctext, double ang, double texth, int icolor, ACHAR* textStyle, ACHAR* font, ACHAR*bigfont, ACHAR *LayerName)
{
	AcDbObjectId  TId;
	AcDbText *pText = new AcDbText;
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL) LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pLayerTable->close();

	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl, AcDb::kForWrite);
	if (pTextStyleTbl->getAt(textStyle, pTextStyleTblId) != Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd = new AcDbTextStyleTableRecord;
		pTextStyleTblRcd->setName(textStyle);
		pTextStyleTblRcd->setFileName(textStyle);
		pTextStyleTblRcd->setFont(font, 0, 0, 134, 2);
		pTextStyleTblRcd->setBigFontFileName(bigfont);
		pTextStyleTblRcd->setXScale(0.7);
		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		pTextStyleTbl->getAt(textStyle, pTextStyleTblId);
	}
	pTextStyleTbl->close();
	pText->setPosition(lpos);
	pText->setHorizontalMode(AcDb::kTextFit);
	pText->setAlignmentPoint(rpos);
	pText->setColorIndex(icolor);
	pText->setLayer(LayerTabRecordId);
	//   pText->set
	pText->setTextStyle(pTextStyleTblId);
	pText->setRotation(ang);
	pText->setHeight(texth);
	pText->setWidthFactor(0.7);
	pText->setTextString(ctext);
	TId = AddEntityToDbs(pText);
	return TId;
}

AcDbObjectId  BAS_DRAW_FUN::makesolid(AcGePoint3d Pt1, AcGePoint3d Pt2, AcGePoint3d Pt3, int icolor, ACHAR *LayerName)
{
	AcDbSolid *pSolid = new AcDbSolid(Pt1, Pt2, Pt3);
	if (icolor >= 0)pSolid->setColorIndex(icolor);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pSolid->setLayer(LayerTabRecordId);
	pLayerTable->close();
	AcDbObjectId  LId;
	LId = AddEntityToDbs(pSolid);
	return LId;
}

void BAS_DRAW_FUN::makeDX(AcGePoint2dArray PtAy, double R, int icolor, double lw, ACHAR *LayerName, ACHAR *LineType, bool dxflag)//导线
{
	//dxflag=true 为导线，起终两点也有圆
	AcGePoint3dArray pts;
	int num = PtAy.length();
	//	AcGePolyline2d poly2d(PtAy);
	AcGeVector3d normal(0, 0, 1);
	if (dxflag)
	{
		AcGePoint3d cen(PtAy[0].x, PtAy[0].y, 0.0);
		AcGePoint3d ptb(PtAy[0].x, PtAy[0].y, 0.0);
		AcGePoint3d pte(PtAy[1].x, PtAy[1].y, 0.0);
		AcDbCircle Cir(cen, normal, R);
		AcDbLine L1(ptb, cen);
		AcDbLine L2(cen, pte);
		Cir.intersectWith(&L1, AcDb::kOnBothOperands, pts);
		Cir.intersectWith(&L2, AcDb::kOnBothOperands, pts);
	}
	else pts.append(AcGePoint3d(PtAy[0].x, PtAy[0].y, 0.0));
	int i;
	for (i = 1; i < num - 1; i++)//1--(n-1)头尾两点非交点
	{
		AcGePoint3d cen(PtAy[i].x, PtAy[i].y, 0.0);
		AcGePoint3d ptb(PtAy[i - 1].x, PtAy[i - 1].y, 0.0);
		AcGePoint3d pte(PtAy[i + 1].x, PtAy[i + 1].y, 0.0);
		AcDbCircle Cir(cen, normal, R);
		AcDbLine L1(ptb, cen);
		AcDbLine L2(cen, pte);
		Cir.intersectWith(&L1, AcDb::kOnBothOperands, pts);
		Cir.intersectWith(&L2, AcDb::kOnBothOperands, pts);
	}
	if (dxflag)
	{
		AcGePoint3d cen(PtAy[i].x, PtAy[i].y, 0.0);
		AcGePoint3d ptb(PtAy[i - 1].x, PtAy[i - 1].y, 0.0);
		AcGePoint3d pte(PtAy[i + 1].x, PtAy[i + 1].y, 0.0);
		AcDbCircle Cir(cen, normal, R);
		AcDbLine L1(ptb, cen);
		AcDbLine L2(cen, pte);
		Cir.intersectWith(&L1, AcDb::kOnBothOperands, pts);
		Cir.intersectWith(&L2, AcDb::kOnBothOperands, pts);
	}
	else pts.append(AcGePoint3d(PtAy[i].x, PtAy[i].y, 0.0));
	int pts_len = (num - 2) * 2 + 2;
	for (i = 1; i < pts_len; i = i + 2)
	{
		makeline(pts[i - 1], pts[i], icolor, lw, LayerName, LineType);
	}
}

void BAS_DRAW_FUN::dTodms(double dd, int Nw, ACHAR *dms)
//度.分秒　　－＞度%%d，分\'，秒\"
{
	double degree = dd;
	ACHAR d[20], m[20], s[20];
	int IntDeg, IntMin, IntSec;
	IntDeg = (int)(degree);
	IntMin = (degree - int(degree))*100.0;//minute
	IntSec = (int)(0.5 + (degree * 100 - int(degree * 100))*100.0);//second四舍五入
	if (IntSec >= 60)
	{
		IntSec -= 60;
		IntMin++;
	}
	if (IntMin >= 60)
	{
		IntMin -= 60;
		IntDeg++;
	}
	acdbRToS(IntDeg, 2, 0, d);
	acdbRToS(IntMin + 100, 2, 0, m);
	acdbRToS(IntSec + 100, 2, 0, s);
	_tcscpy(dms, d);
	ACHAR ch[20];
	ch[0] = '%';
	ch[1] = '%';
	ch[2] = 'd';
	ch[3] = m[1];
	ch[4] = m[2];
	ch[5] = '\0';
	_tcscat(dms, ch);
	ch[0] = '\'';
	ch[1] = s[1];
	ch[2] = s[2];
	ch[3] = '\0';
	_tcscat(dms, ch);
	_tcscat(dms, L"\"");
}

//直线与直线
int BAS_DRAW_FUN::CalJD_ZX_ZX(double Line1_array[10], double Line2_array[10], double ResPt[2])
{
	int JDNUM;
	JDNUM = 0;
	ResPt[0] = 0.0;
	ResPt[1] = 0.0;
	double Spt1[2], Ept1[2], Spt2[2], Ept2[2];
	Spt1[0] = Line1_array[8], Spt1[1] = Line1_array[9];
	Ept1[0] = Spt1[0] + Line1_array[1] * cos(Line1_array[5]);
	Ept1[1] = Spt1[1] + Line1_array[1] * sin(Line1_array[5]);
	Spt2[0] = Line2_array[8], Spt2[1] = Line2_array[9];
	Ept2[0] = Spt2[0] + Line2_array[1] * cos(Line2_array[5]);
	Ept2[1] = Spt2[1] + Line2_array[1] * sin(Line2_array[5]);

	if (INTERS(&ResPt[0], &ResPt[1], Spt1[0], Spt1[1], Ept1[0], Ept1[1], Spt2[0], Spt2[1], Ept2[0], Ept2[1]) == 0)
		JDNUM = 1;//找到

	//	if(JDNUM>0)
	//	{
	//	  ResPt[3]=Line1_array[6] + sqrt((ResPt[0]-Spt1[0])*(ResPt[0]-Spt1[0])+(ResPt[1]-Spt1[1])*(ResPt[1]-Spt1[1]));
	//	  ResPt[4]=Line2_array[6] + sqrt((ResPt[0]-Spt2[0])*(ResPt[0]-Spt2[0])+(ResPt[1]-Spt2[1])*(ResPt[1]-Spt2[1]));
	//	}
	return JDNUM;//没找到
}
//直线与圆
int BAS_DRAW_FUN::CalJD_ZX_ARC(double Line_array[10], double ARC_array[10], double ResPt[2], double ResPt1[2])
{
	int JDSum;

	ResPt[0] = 0.0;
	ResPt[1] = 0.0;
	ResPt1[0] = 0.0;
	ResPt1[1] = 0.0;
	JDSum = 0;

	AcGePoint2d Spt, Ept;
	double xmin, ymin, xmax, ymax;
	Spt.x = Line_array[8], Spt.y = Line_array[9];
	Ept.x = Spt.x + Line_array[1] * cos(Line_array[5]);
	Ept.y = Spt.y + Line_array[1] * sin(Line_array[5]);
	AcGeLine2d line(Spt, Ept);
	xmin = Spt.x < Ept.x ? Spt.x : Ept.x;
	ymin = Spt.y < Ept.y ? Spt.y : Ept.y;
	xmax = Spt.x > Ept.x ? Spt.x : Ept.x;
	ymax = Spt.y > Ept.y ? Spt.y : Ept.y;

	double R, Xo, Yo, x, y, zx, fwj, pt[3];
	R = ARC_array[1];
	x = ARC_array[8], y = ARC_array[9];
	fwj = ARC_array[5];
	zx = ARC_array[4];
	Xo = x + R * cos(fwj + 0.5*pi*zx);
	Yo = y + R * sin(fwj + 0.5*pi*zx);
	AcGePoint2d Arc_Spt, Arc_Ept, Arc_Mpt, jd1, jd2;

	Arc_Spt.set(x, y);
	GetXYonSomeXY(ARC_array, ARC_array[2], pt);
	Arc_Ept.set(pt[0], pt[1]);
	GetXYonSomeXY(ARC_array, 0.5*ARC_array[2], pt);
	Arc_Mpt.set(pt[0], pt[1]);

	AcGeCircArc2d ARC(Arc_Spt, Arc_Mpt, Arc_Ept);

	ARC.intersectWith(line, JDSum, jd1, jd2);
	int njd;
	njd = 0;
	if (JDSum > 0)
	{
		if (jd1.x > xmin - 0.001 && jd1.x<xmax + 0.001
			&& jd1.y>ymin - 0.001 && jd1.y < ymax + 0.001)
		{
			njd++;
			ResPt[0] = jd1.x, ResPt[1] = jd1.y;
		}
		if (jd2.x > xmin - 0.001 && jd2.x<xmax + 0.001
			&& jd2.y>ymin - 0.001 && jd2.y < ymax + 0.001)
		{
			njd++;
			if (njd == 2)
				ResPt1[0] = jd2.x, ResPt1[1] = jd2.y;
			else
				ResPt[0] = jd2.x, ResPt[1] = jd2.y;
		}
	}

	return njd;
}
//直线与缓和曲线
int BAS_DRAW_FUN::CalJD_ZX_Spiral(double Line_array[10], double Spiral_array[10], double ResPt[3], double ResPt1[3])
{
	double Spiral_Len = 0;
	int JDNUM = 0;
	CHhqx Hhqx1;
	double Rstart = 10000000000000;
	double Rend = 0;
	double Spiral1_A, Spiral1_Spt[3], Spiral1_Len, Spiral1_ZorY;
	//1.定义直线
	double Spt[2], Ept[2], pt[3], ptcz[2];
	Spt[0] = Spt[1] = 0;
	Ept[0] = Ept[1] = 0;
	pt[0] = pt[1] = pt[2] = 0;
	ptcz[0] = ptcz[1] = 0;
	Spt[0] = Line_array[8], Spt[1] = Line_array[9];
	Ept[0] = Spt[0] + Line_array[1] * cos(Line_array[5]);
	Ept[1] = Spt[1] + Line_array[1] * sin(Line_array[5]);
	pt[0] = 0;
	pt[1] = 0;
	pt[2] = 0;
	double xmin, xmax, ymin, ymax;
	xmin = xmax = ymin = ymax = 0;
	xmin = Spt[0] < Ept[0] ? Spt[0] : Ept[0];
	ymin = Spt[1] < Ept[1] ? Spt[1] : Ept[1];
	xmax = Spt[0] > Ept[0] ? Spt[0] : Ept[0];
	ymax = Spt[1] > Ept[1] ? Spt[1] : Ept[1];

	//计算缓长
	Spiral1_Len = Spiral_array[1] * Spiral_array[1] / Spiral_array[2];
	Spiral1_ZorY = Spiral_array[4];
	if (Spiral_array[0] < 3.1)//前缓
	{
		//Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		Spiral1_Spt[0] = Spiral_array[8], Spiral1_Spt[1] = Spiral_array[9], Spiral1_Spt[2] = Spiral_array[5];
		//SpiralSL1=0.0;
		//SpiralEL1=Spiral1_Len;
		Rend = Spiral_array[2];
	}
	else if (Spiral_array[0] < 4.1)//后缓
	{
		//Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		GetXYonSomeXY(Spiral_array, Spiral1_Len, Spiral1_Spt);
		Spiral1_Spt[2] += pi;
		Spiral1_ZorY = -1 * Spiral1_ZorY;
		Rend = Spiral_array[2];
	}
	Hhqx1.Set(Spiral1_Spt[0], Spiral1_Spt[1], Spiral1_Spt[2], Spiral1_Len, Rstart, Rend, Spiral1_ZorY, 0);
	Hhqx1.GetJdZxHhqx(ResPt[0], ResPt[1], ResPt[2], ResPt1[0], ResPt1[1], ResPt1[2], JDNUM, Spt[0], Spt[1], Ept[0], Ept[1]);
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
//圆与圆
//int BAS_DRAW_FUN::CalJD_ARC_ARC(double Xo1,double Yo1,double R1,double Xo2,double Yo2,double R2,double ResPt[2])
//{
//	double CC;
//	ResPt[0]=ResPt[1]=0.0;
//	CC = sqrt((Xo1-Xo2)*(Xo1-Xo2)+(Yo1-Yo2)*(Yo1-Yo2));
//    if(CC > R1+R2 || CC<fabs(R1-R2))
//		return 1;
//	//1.解三角形，得到两个值
//	//2.判别合理解
//}
int BAS_DRAW_FUN::CalJD_Arc_Arc(double Arc1_array[10], double Arc2_array[10], double ResPt[3], double ResPt1[3])
{
	double pt[3];
	int JDNUM;
	AcGePoint2d Arc1_Spt, Arc1_Ept, Arc1_Mpt;

	Arc1_Spt.set(Arc1_array[9], Arc1_array[8]);
	GetXYonSomeXY(Arc1_array, Arc1_array[2], pt);
	Arc1_Ept.set(pt[1], pt[0]);
	GetXYonSomeXY(Arc1_array, 0.5*Arc1_array[2], pt);
	Arc1_Mpt.set(pt[1], pt[0]);

	AcGePoint2d Arc2_Spt, Arc2_Ept, Arc2_Mpt;
	Arc2_Spt.set(Arc2_array[9], Arc2_array[8]);
	GetXYonSomeXY(Arc2_array, Arc2_array[2], pt);
	Arc2_Ept.set(pt[1], pt[0]);
	GetXYonSomeXY(Arc2_array, 0.5*Arc2_array[2], pt);
	Arc2_Mpt.set(pt[1], pt[0]);

	AcGeCircArc2d ARC1(Arc1_Spt, Arc1_Mpt, Arc1_Ept);
	AcGeCircArc2d ARC2(Arc2_Spt, Arc2_Mpt, Arc2_Ept);
	AcGePoint2d jd1, jd2;

	ARC1.intersectWith(ARC2, JDNUM, jd1, jd2);
	if (JDNUM > 0)
	{
		ResPt[0] = jd1.y, ResPt[1] = jd1.x;
		ResPt1[0] = jd2.y, ResPt1[1] = jd2.x;
	}

	return JDNUM;
}
int BAS_DRAW_FUN::CalJD_ARC_Spiral(double Arc_array[10], double Spiral_array[10], double ResPt[3], double ResPt1[3])
{
	double Arc_Len;
	double Spiral_Len;
	int JDNUM;

	double  R, Xo, Yo, x, y, zx, fwj, fwj2, pt[3];
	CHhqx Hhqx1;
	double Rstart = 10000000000000;
	double Rend = 0;
	double Spiral1_A, Spiral1_Spt[3], Spiral1_Len, Spiral1_ZorY;
	R = Arc_array[1];
	Arc_Len = Arc_array[2];
	x = Arc_array[8], y = Arc_array[9];
	fwj = Arc_array[5];
	zx = Arc_array[4];
	Xo = x + R * cos(fwj + 0.5*pi*zx);
	Yo = y + R * sin(fwj + 0.5*pi*zx);
	fwj2 = fwj + (Arc_Len / R)*zx;
	if (zx > 0)
	{
		fwj = Arc_array[5] - pi / 2;
		fwj2 = fwj + (Arc_Len / R);
		fwj = pi * 2 - fwj + pi / 2;
		fwj2 = pi * 2 - fwj2 + pi / 2;
		while (fwj > pi * 2)
		{
			fwj -= pi * 2;
		}
		while (fwj2 > pi * 2)
		{
			fwj2 -= pi * 2;
		}
	}
	else
	{
		fwj = Arc_array[5] + pi / 2;
		fwj2 = fwj - (Arc_Len / R);
		fwj = pi * 2 - fwj + pi / 2;
		fwj2 = pi * 2 - fwj2 + pi / 2;
		while (fwj > pi * 2)
		{
			fwj -= pi * 2;
		}
		while (fwj2 > pi * 2)
		{
			fwj2 -= pi * 2;
		}
	}
	JDNUM = 0;
	ResPt[0] = ResPt[1] = ResPt[2] = 0.0;
	ResPt1[0] = ResPt1[1] = ResPt1[2] = 0.0;

	//if(Spiral_array[0]<4.1)//前缓
	//  Spiral_Len = Spiral_array[1]*Spiral_array[1]/Spiral_array[2];   
	//else if(Spiral_array[0]<6.1)//不完整缓(R大->R小)
	//  Spiral_Len = fabs(Spiral_array[1]*Spiral_array[1]/Spiral_array[3]-Spiral_array[1]*Spiral_array[1]/Spiral_array[2]);   	   

	BlockComputerFun ob;
	double dis = 100.0;
	double len = 0.0;
	JDNUM = 0;
	Spiral1_Len = Spiral_array[1] * Spiral_array[1] / Spiral_array[2];
	Spiral1_ZorY = Spiral_array[4];
	if (Spiral_array[0] < 3.1)//前缓
	{
		//Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		Spiral1_Spt[0] = Spiral_array[8], Spiral1_Spt[1] = Spiral_array[9], Spiral1_Spt[2] = Spiral_array[5];
		//SpiralSL1=0.0;
		//SpiralEL1=Spiral1_Len;
		Rend = Spiral_array[2];
	}
	else if (Spiral_array[0] < 4.1)//后缓
	{
		//Spiral1_Len = Spiral1_array[1]*Spiral1_array[1]/Spiral1_array[2];   
		GetXYonSomeXY(Spiral_array, Spiral1_Len, Spiral1_Spt);
		Spiral1_Spt[2] += pi;
		Spiral1_ZorY = -1 * Spiral1_ZorY;
		Rend = Spiral_array[2];
	}
	Hhqx1.Set(Spiral1_Spt[0], Spiral1_Spt[1], Spiral1_Spt[2], Spiral1_Len, Rstart, Rend, Spiral1_ZorY, 0);
	if (fwj < fwj2)
		Hhqx1.GetJdArcHhqx(ResPt[0], ResPt[1], ResPt1[0], ResPt1[1], JDNUM, Xo, Yo, R, fwj, fwj2);
	else
		Hhqx1.GetJdArcHhqx(ResPt[0], ResPt[1], ResPt1[0], ResPt1[1], JDNUM, Xo, Yo, R, fwj2, fwj);
	//   if(CheckIfOnArc(Arc_array,pt))
	//{
	//	 JDNUM++;
	//	 ResPt[0] = pt[0],ResPt[1] = pt[1];
	//  }
	//从起点开始
	//	while(len<Spiral_Len+0.001 && dis>0.0001)
	//	{
	//    	GetXYonSomeXY(Spiral_array,len,pt);
	//        dis = sqrt((pt[0]-Xo)*(pt[0]-Xo)+(pt[1]-Yo)*(pt[1]-Yo)) - R;
	//		dis = fabs(dis);
	//		len = len + dis;
	//	}
	//	if(dis<=0.0001)
	//	{
	//	//	ads_printf(L"\n&&&&&&&& %lf %lf\n",pt[0],pt[1]);
	//	  if(CheckIfOnArc(Arc_array,pt))
	//	  {
	//		 JDNUM++;
	//		 ResPt[0] = pt[0],ResPt[1] = pt[1];
	//	  }
	//	}
	////从终点开始
	//	len = Spiral_Len;
	//	dis = 100;
	//	while(len>=0.0 && dis>0.0001)
	//	{
	//    	GetXYonSomeXY(Spiral_array,len,pt);
	//        dis = sqrt((pt[0]-Xo)*(pt[0]-Xo)+(pt[1]-Yo)*(pt[1]-Yo)) - R;
	//		dis = fabs(dis);
	//		len = len - dis;
	//	}
	//	if(dis<=0.0001)
	//	{
	//		if(JDNUM>0)
	//		    dis = sqrt((pt[0]-ResPt[0])*(pt[0]-ResPt[0])+(pt[1]-ResPt[1])*(pt[1]-ResPt[1]));
	//		if(JDNUM==0 || (JDNUM>0&&dis>0.0001))
	//		{
	//		   if(CheckIfOnArc(Arc_array,pt))
	//		   {
	//			  JDNUM++;
	//			  if(JDNUM>1)
	//		         ResPt1[0] = pt[0],ResPt1[1] = pt[1];
	//			  else
	//				 ResPt[0] = pt[0],ResPt[1] = pt[1];
	//		   }		   
	//		}
	//	}

	return JDNUM;
}

int BAS_DRAW_FUN::CalJD_Spiral_Spiral(double Spiral1_array[10], double Spiral2_array[10], double ResPt[3], double ResPt1[3])
{
	//int Njd=0;
	////	double LtoBasePt1,LtoBasePt2;
	//double Np1,Ep1,Np2,Ep2;
	CHhqx Hhqx1, Hhqx2;
	//double Hhqx1_StartFwj,Hhqx1_L0,Hhqx1_Rstart,Hhqx1_Rend,Hhqx1_StartN,Hhqx1_StartE;
	//double Hhqx2_StartFwj,Hhqx2_L0,Hhqx2_Rstart,Hhqx2_Rend,Hhqx2_StartN,Hhqx2_StartE;
	//int Hhqx1_Mod,Hhqx1_LeftorRight,Hhqx2_Mod,Hhqx2_LeftorRight;
	//Hhqx1_StartFwj=
	double Rstart = 10000000000000;
	double Rend = 0;
	double Spiral1_A, Spiral1_Spt[3], Spiral1_Len, Spiral1_ZorY;
	double Spiral2_A, Spiral2_Spt[3], Spiral2_Len, Spiral2_ZorY;
	double l2_spiral1, l2_spiral2;
	double l1_spiral1, l1_spiral2;
	int inf = 0;
	int JDNUM;
	double SpiralSL1, SpiralEL1;
	double SpiralSL2, SpiralEL2;
	Spiral1_A = Spiral1_Len = Spiral1_ZorY = 0;
	Spiral2_A = Spiral2_Len = Spiral2_ZorY = 0;
	l2_spiral1 = l2_spiral2 = 0;
	l1_spiral1 = l1_spiral2 = 0;
	Spiral1_Spt[0] = Spiral1_Spt[1] = Spiral1_Spt[2] = 0;
	Spiral2_Spt[0] = Spiral2_Spt[1] = Spiral2_Spt[2] = 0;
	SpiralSL1 = SpiralEL1 = 0;
	SpiralSL2 = SpiralEL2 = 0;
	JDNUM = 0;
	ResPt[0] = ResPt[1] = ResPt[2] = 0.0;
	ResPt1[0] = ResPt1[1] = ResPt1[2] = 0.0;

	Spiral1_A = Spiral1_array[1];
	Spiral1_ZorY = Spiral1_array[4];
	if (Spiral1_array[0] < 3.1)//前缓
	{
		Spiral1_Len = Spiral1_array[1] * Spiral1_array[1] / Spiral1_array[2];
		Spiral1_Spt[0] = Spiral1_array[8], Spiral1_Spt[1] = Spiral1_array[9], Spiral1_Spt[2] = Spiral1_array[5];
		SpiralSL1 = 0.0;
		SpiralEL1 = Spiral1_Len;
		Rend = Spiral1_array[2];
	}
	else if (Spiral1_array[0] < 4.1)//后缓
	{
		Spiral1_Len = Spiral1_array[1] * Spiral1_array[1] / Spiral1_array[2];
		GetXYonSomeXY(Spiral1_array, Spiral1_Len, Spiral1_Spt);
		Spiral1_Spt[2] += pi;
		Spiral1_ZorY = -1 * Spiral1_ZorY;
		SpiralSL1 = 0.0;
		SpiralEL1 = Spiral1_Len;
		Rend = Spiral1_array[2];
	}
	else if (Spiral1_array[0] < 5.1)//不完整缓(R大->R小)
	{
		BlockComputerFun fun;
		fun.GetOrignCorOfSprial(Spiral1_array[8], Spiral1_array[9], Spiral1_array[2],
			Spiral1_array[5], Spiral1_array[1], Spiral1_array[4], LargeToSmall,
			Spiral1_Spt[0], Spiral1_Spt[1], Spiral1_Spt[2]);
		Spiral1_Len = Spiral1_array[1] * Spiral1_array[1] / Spiral1_array[3];
		SpiralSL1 = Spiral1_array[1] * Spiral1_array[1] / Spiral1_array[2];
		SpiralEL1 = Spiral1_Len;
		Rend = Spiral1_array[2];
	}
	else//不完整缓(R小->R大)
	{
		BlockComputerFun fun;
		fun.GetOrignCorOfSprial(Spiral1_array[8], Spiral1_array[9], Spiral1_array[3],
			Spiral1_array[5], Spiral1_array[1], Spiral1_array[4], SmallToLarge,
			Spiral1_Spt[0], Spiral1_Spt[1], Spiral1_Spt[2]);
		//Spiral1_Spt[2] += pi;
		Spiral1_ZorY = -1 * Spiral1_ZorY;
		Spiral1_Len = Spiral1_array[1] * Spiral1_array[1] / Spiral1_array[3];
		SpiralSL1 = Spiral1_array[1] * Spiral1_array[1] / Spiral1_array[2];
		SpiralEL1 = Spiral1_Len;
		Rend = Spiral1_array[2];
	}
	Hhqx1.Set(Spiral1_Spt[0], Spiral1_Spt[1], Spiral1_Spt[2], Spiral1_Len, Rstart, Rend, Spiral1_ZorY, 0);
	Rend = 0;
	Spiral2_A = Spiral2_array[1];
	Spiral2_ZorY = Spiral2_array[4];
	if (Spiral2_array[0] < 3.1)//前缓
	{
		Spiral2_Len = Spiral2_array[1] * Spiral2_array[1] / Spiral2_array[2];
		Spiral2_Spt[0] = Spiral2_array[8], Spiral2_Spt[1] = Spiral2_array[9], Spiral2_Spt[2] = Spiral2_array[5];
		SpiralSL2 = 0.0;
		SpiralEL2 = Spiral2_Len;
		Rend = Spiral2_array[2];
	}
	else if (Spiral2_array[0] < 4.1)//后缓
	{
		Spiral2_Len = Spiral2_array[1] * Spiral2_array[1] / Spiral2_array[2];
		GetXYonSomeXY(Spiral2_array, Spiral2_Len, Spiral2_Spt);
		Spiral2_Spt[2] += pi;
		Spiral2_ZorY = -1 * Spiral2_ZorY;
		SpiralSL2 = 0.0;
		SpiralEL2 = Spiral2_Len;
		Rend = Spiral2_array[2];
	}
	else if (Spiral2_array[0] < 5.1)//不完整缓(R大->R小)
	{
		BlockComputerFun fun;
		fun.GetOrignCorOfSprial(Spiral2_array[8], Spiral2_array[9], Spiral2_array[2],
			Spiral2_array[5], Spiral2_array[1], Spiral2_array[4], LargeToSmall,
			Spiral2_Spt[0], Spiral2_Spt[1], Spiral2_Spt[2]);
		Spiral2_Len = Spiral2_array[1] * Spiral2_array[1] / Spiral2_array[3];
		SpiralSL2 = Spiral2_array[1] * Spiral2_array[1] / Spiral2_array[2];
		SpiralEL2 = Spiral2_Len;
		Rend = Spiral2_array[2];
	}
	else//不完整缓(R小->R大)
	{
		BlockComputerFun fun;
		fun.GetOrignCorOfSprial(Spiral2_array[8], Spiral2_array[9], Spiral2_array[3],
			Spiral2_array[5], Spiral2_array[1], Spiral2_array[4], SmallToLarge,
			Spiral2_Spt[0], Spiral2_Spt[1], Spiral2_Spt[2]);
		Spiral2_Spt[2] += pi;
		Spiral2_ZorY = -1 * Spiral2_ZorY;
		Spiral2_Len = Spiral2_array[1] * Spiral2_array[1] / Spiral2_array[3];
		SpiralSL2 = Spiral2_array[1] * Spiral2_array[1] / Spiral2_array[2];
		SpiralEL2 = Spiral2_Len;
		Rend = Spiral2_array[2];
	}
	Hhqx2.Set(Spiral2_Spt[0], Spiral2_Spt[1], Spiral2_Spt[2], Spiral2_Len, Rstart, Rend, Spiral2_ZorY, 0);
	// CArray<CBasicFun::Jd,CBasicFun::Jd> HHJd;
	Hhqx1.GetJdHhqxHhqx(ResPt[0], ResPt[1], ResPt[2], ResPt1[0], ResPt1[1], ResPt1[2], JDNUM, Hhqx1, Hhqx2, 0, Spiral1_Len, 0, Spiral2_Len);
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

//
int BAS_DRAW_FUN::CalJD_ARC_Spiral_diedai(double &l2_arc, double &l2_spiral, double Arc_R, double Arc_Spt[3], double Arc_Len, double Arc_ZorY,
	double Spiral_A, double Spiral_Spt[3], double Spiral_Len, double Spiral_ZorY)
{
	double x1_arc, y1_arc;//arc
	double x1_spiral, y1_spiral;//spiral
	double l1_arc, l1_spiral;

	double Spiral_R;
	double x1, x2;

	//1.给出初始值l1
	//	l2_arc=l2_spiral=0.0;
	l1_arc = Arc_Len;
	l1_spiral = Spiral_Len;
	//2.计算x1,y1
	//2.1计算圆x1,y1
	// l 为该线元的长度 m
	while (l2_arc > -0.00001&&l2_arc<Arc_Len &&l2_spiral>-0.00001&& l2_spiral<Spiral_Len && fabs(l1_arc - l2_arc)>0.0001 && fabs(l1_spiral - l2_spiral) > 0.0001)
	{
		l1_arc = l2_arc;
		l1_spiral = l2_spiral;

		x1_arc = l1_arc - pow(l1_arc, 3) / 6.0 / pow(Arc_R, 3) + pow(l1_arc, 5) / 120.0 / pow(Arc_R, 4);
		y1_arc = pow(l1_arc, 2) / 2.0 / pow(Arc_R, 2) - pow(l1_arc, 4) / 24.0 / pow(Arc_R, 3) + pow(l1_arc, 6) / 720.0 / pow(Arc_R, 5);
		y1_arc = Arc_ZorY * y1_arc;
		//2.2计算缓和曲线x1,y1
		if (l1_spiral > 0.0001)
			Spiral_R = Spiral_A * Spiral_A / l1_spiral;
		else
			Spiral_R = 1.0E50;

		x1_spiral = l1_spiral - pow(l1_spiral, 2) / 40.0 / pow(Spiral_R, 2) + pow(l1_spiral, 4) / 3456.0 / pow(Spiral_R, 4);
		y1_spiral = pow(l1_spiral, 2) / 6.0 / Spiral_R - pow(l1_spiral, 4) / 336.0 / pow(Spiral_R, 3) + pow(l1_spiral, 6) / 42240.0 / pow(Spiral_R, 5);
		y1_spiral = Spiral_ZorY * y1_spiral;
		//3.计算新的x2,y2
		x1 = (Arc_Spt[0] - Spiral_Spt[0] + x1_arc * cos(Arc_Spt[2]) - y1_arc * sin(Arc_Spt[2]) + y1_spiral * sin(Spiral_Spt[2])) / cos(Spiral_Spt[2]);
		x2 = (Spiral_Spt[1] - Arc_Spt[1] + x1_spiral * sin(Spiral_Spt[2]) + y1_spiral * cos(Spiral_Spt[2]) - y1_arc * cos(Arc_Spt[2])) / sin(Arc_Spt[2]);
		//4.计算新的l2
		l2_arc = x2 + pow(l1_arc, 3) / 6.0 / pow(Arc_R, 3) + pow(l1_arc, 5) / 120.0 / pow(Arc_R, 4);
		l2_spiral = x1 + pow(l1_spiral, 2) / 40.0 / pow(Spiral_R, 2) + pow(l1_spiral, 4) / 3456.0 / pow(Spiral_R, 4);;
		ads_printf(L"2:x1=%lf x2=%lf l1_arc=%lf l1_s=%lf\n", x1, x2, l2_arc, l2_spiral);
	}
	//5.比较l1,l2
	if (l2_arc < Arc_Len && l2_spiral < Spiral_Len && fabs(l1_arc - l2_arc) < 0.0001 && fabs(l1_spiral - l2_spiral) < 0.0001)
		return 1;

	return 0;
}

int BAS_DRAW_FUN::GetXYonSomeXY(double array2[10], double jslen, double respt[3])
{
	double INDATA2[5];
	xlpoint *PBI, *PZ;

	PBI = PZ = new xlpoint;
	for (int k = 0; k < 5; k++)
		INDATA2[k] = array2[k];
	int j;
	double lc;
	double l = Get_INDATA_L(INDATA2, &j);
	PBI->a = array2[5];
	PBI->lc = array2[6];
	PBI->r = array2[7];
	PBI->x = array2[8];
	PBI->y = array2[9];
	// l 为该线元的长度 m

	double jslc;
	jslc = PBI->lc + jslen;
	// 计算点在i线元上
	if ((jslc <= PBI->lc + l) && (jslc >= PBI->lc))
	{
		lc = jslc - PBI->lc;
		if (lc <= 0.0) lc = 0.000000000001;
		xlzbxy(PBI, PZ, lc, INDATA2, 0);
		respt[0] = PZ->x, respt[1] = PZ->y, respt[2] = PZ->a;
		return 1;
	}
	else
		respt[0] = 0.0, respt[1] = 0.0, respt[2] = 0.0;
	return 0;
}

int BAS_DRAW_FUN::CalJD_line_Spiral_diedai(double &l2_line, double &l2_spiral, double line_Spt[3], double line_Len,
	double Spiral_A, double Spiral_Spt[3], double Spiral_Len, double Spiral_ZorY)
{
	double x1_line, y1_line;//line
	double x1_spiral, y1_spiral;//spiral
	double l1_line, l1_spiral;

	double Spiral_R;
	double x1, x2;

	//1.给出初始值l1
	//	l2_line=l2_spiral=0.0;
	l1_line = line_Len;
	l1_spiral = Spiral_Len;
	//2.计算x1,y1
	//2.1计算圆x1,y1
	// l 为该线元的长度 m
	//ads_printf(L"l2_line=%lf line_Len=%lf l2_spiral=%lf Spiral_Len=%lf\n",l2_line,line_Len,l2_spiral,Spiral_Len);
	while (l2_line > -0.00001&&l2_line<line_Len && l2_spiral>-0.00001&&l2_spiral<Spiral_Len && fabs(l1_line - l2_line)>0.0001 && fabs(l1_spiral - l2_spiral) > 0.0001)
	{
		l1_line = l2_line;
		l1_spiral = l2_spiral;

		ads_printf(L"1:%lf %lf\n", l1_line, l1_spiral);

		//	x1_line=l1_line-pow(l1_line,2)/2.0+pow(l1_line,4)/24.0;
		//	y1_line=l1_line-pow(l1_line,3)/6.0+pow(l1_line,5)/120.0;
		x1_line = l1_line;
		y1_line = l1_line;// - pow(l1_line,3)/6.0+pow(l1_line,5)/120.0;

		int n = 2;
		double Bxval, Byval, xval, yval;
		Bxval = Byval = 0.0;
		xval = yval = 1.0;
		//	while(fabs(Bxval-xval)>1.0E-10 || fabs(Byval-yval)>1.0E-10)
		//	{
		//	   Bxval = xval,Byval = yval;
		n = 50;
		CalLineExtension(n, l1_line, xval, yval);
		//	   n++;
		//	}
		x1_line += xval;
		y1_line += yval;

		ads_printf(L"n=%d %lf %lf\n", n, xval, yval);

		//2.2计算缓和曲线x1,y1
		if (l1_spiral > 0.0001)
			Spiral_R = Spiral_A * Spiral_A / l1_spiral;
		else
			Spiral_R = 1.0E50;

		x1_spiral = l1_spiral - pow(l1_spiral, 2) / 40.0 / pow(Spiral_R, 2) + pow(l1_spiral, 4) / 3456.0 / pow(Spiral_R, 4);
		y1_spiral = pow(l1_spiral, 2) / 6.0 / Spiral_R - pow(l1_spiral, 4) / 336.0 / pow(Spiral_R, 3) + pow(l1_spiral, 6) / 42240.0 / pow(Spiral_R, 5);
		y1_spiral = Spiral_ZorY * y1_spiral;
		//3.计算新的x2,y2
		x1 = (line_Spt[0] - Spiral_Spt[0] + x1_line + y1_spiral * sin(Spiral_Spt[2])) / cos(Spiral_Spt[2]);
		x2 = (Spiral_Spt[0] - line_Spt[0] + x1_spiral * cos(Spiral_Spt[2]) - y1_spiral * sin(Spiral_Spt[2]));
		//4.计算新的l2

		l2_line = x2 + xval;
		l2_spiral = x1 + pow(l1_spiral, 2) / 40.0 / pow(Spiral_R, 2) - pow(l1_spiral, 4) / 3456.0 / pow(Spiral_R, 4);;

		ads_printf(L"2:x1=%lf x2=%lf l1=%lf l2=%lf\n", x1, x2, l2_line, l2_spiral);
	}
	//5.比较l1,l2
	if (l2_line > -0.00001&&l2_line<line_Len && l2_spiral>-0.00001&&l2_spiral<Spiral_Len && fabs(l1_line - l2_line)>0.0001 && fabs(l1_spiral - l2_spiral) > 0.0001)
		return 1;

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int BAS_DRAW_FUN::CalJD_ARC_ARC_diedai(double &l2_arc1, double &l2_arc2, double Arc1_R, double Arc1_Spt[3], double Arc1_Len, double Arc1_ZorY,
	double Arc2_R, double Arc2_Spt[3], double Arc2_Len, double Arc2_ZorY)
{

	double x1_arc1, y1_arc1;//arc
	double x1_arc2, y1_arc2;//arc
	double l1_arc1, l1_arc2;
	double x1, x2;

	//1.给出初始值l1
	//	l2_arc=l2_spiral=0.0;
	l1_arc1 = Arc1_Len;
	l1_arc2 = Arc2_Len;
	//2.计算x1,y1
	//2.1计算圆x1,y1
	// l 为该线元的长度 m
	while (l2_arc1 > -0.00001&&l2_arc1<Arc1_Len + 0.0001 &&l2_arc2>-0.00001&& l2_arc2<Arc2_Len + 0.0001 && fabs(l1_arc1 - l2_arc1)>0.0001 && fabs(l1_arc2 - l2_arc2) > 0.0001)
	{
		l1_arc1 = l2_arc1;
		l1_arc2 = l2_arc2;

		x1_arc1 = l1_arc1 - pow(l1_arc1, 3) / 6.0 / pow(Arc1_R, 3) + pow(l1_arc1, 5) / 120.0 / pow(Arc1_R, 4);
		y1_arc1 = pow(l1_arc1, 2) / 2.0 / pow(Arc1_R, 2) - pow(l1_arc1, 4) / 24.0 / pow(Arc1_R, 3) + pow(l1_arc1, 6) / 720.0 / pow(Arc1_R, 5);
		y1_arc1 = Arc1_ZorY * y1_arc1;

		x1_arc2 = l1_arc2 - pow(l1_arc2, 3) / 6.0 / pow(Arc2_R, 3) + pow(l1_arc2, 5) / 120.0 / pow(Arc2_R, 4);
		y1_arc2 = pow(l1_arc2, 2) / 2.0 / pow(Arc2_R, 2) - pow(l1_arc2, 4) / 24.0 / pow(Arc2_R, 3) + pow(l1_arc2, 6) / 720.0 / pow(Arc2_R, 5);
		y1_arc2 = Arc2_ZorY * y1_arc2;
		//3.计算新的x2,y2
		x1 = (Arc1_Spt[0] - Arc2_Spt[0] + x1_arc1 * cos(Arc1_Spt[2]) - y1_arc1 * sin(Arc1_Spt[2]) + y1_arc2 * sin(Arc2_Spt[2])) / cos(Arc2_Spt[2]);
		x2 = (Arc2_Spt[1] - Arc1_Spt[1] + x1_arc2 * sin(Arc2_Spt[2]) + y1_arc2 * cos(Arc2_Spt[2]) - y1_arc1 * cos(Arc1_Spt[2])) / sin(Arc1_Spt[2]);
		//4.计算新的l2
		l2_arc1 = x2 + pow(l1_arc1, 3) / 6.0 / pow(Arc1_R, 3) + pow(l1_arc1, 5) / 120.0 / pow(Arc1_R, 4);
		l2_arc2 = x1 + pow(l1_arc2, 2) / 40.0 / pow(Arc2_R, 2) + pow(l1_arc2, 4) / 3456.0 / pow(Arc2_R, 4);
	}
	//5.比较l1,l2
	if (l2_arc1 < Arc1_Len + 0.0001 && l2_arc2 < Arc2_Len + 0.0001 && fabs(l1_arc1 - l2_arc1) < 0.0001 && fabs(l1_arc2 - l2_arc2) < 0.0001)
		return 1;

	return 0;
}

int BAS_DRAW_FUN::CalJD_Spiral_Spiral_diedai(double &l2_spiral1, double &l2_spiral2, double Spiral1_A, double Spiral1_Spt[3], double Spiral1_Len, double Spiral1_ZorY,
	double Spiral2_A, double Spiral2_Spt[3], double Spiral2_Len, double Spiral2_ZorY)
{
	double x1_spiral1, y1_spiral1;//spiral
	double x1_spiral2, y1_spiral2;//spiral
	double l1_spiral1, l1_spiral2;

	double Spiral1_R, Spiral2_R;
	double x1, x2;

	//1.给出初始值l1
	//	l2_arc=l2_spiral=0.0;
	//	l1_spiral1=Spiral1_Len;
	//	l1_spiral2=Spiral2_Len;
	if (l2_spiral1 > 0.001)
		l1_spiral1 = 0.0;
	else
		l1_spiral1 = Spiral1_Len;
	if (l1_spiral2 > 0.001)
		l1_spiral2 = 0.0;
	else
		l1_spiral2 = Spiral2_Len;
	//2.计算x1,y1
	//2.1计算圆x1,y1
	// l 为该线元的长度 m
	//while(l2_spiral1>-0.00001&& l2_spiral1<Spiral1_Len+0.0001&&l2_spiral2>-0.00001&& l2_spiral2<Spiral2_Len+0.0001 && fabs(l1_spiral1-l2_spiral1)>0.0001 && fabs(l1_spiral2-l2_spiral2)>0.0001)
	while (fabs(l1_spiral1 - l2_spiral1) > 0.0001 || fabs(l1_spiral2 - l2_spiral2) > 0.0001)
	{
		l1_spiral1 = l2_spiral1;
		l1_spiral2 = l2_spiral2;
		//ads_printf(L"1:%lf %lf\n",l1_spiral1,l1_spiral2);		
		if (l1_spiral1 > 0.0001)
			Spiral1_R = Spiral1_A * Spiral1_A / l1_spiral1;
		else
			Spiral1_R = 1.0E50;

		//2.2计算缓和曲线x1,y1
		if (l1_spiral2 > 0.0001)
			Spiral2_R = Spiral2_A * Spiral2_A / l1_spiral2;
		else
			Spiral2_R = 1.0E50;

		x1_spiral1 = l1_spiral1 - pow(l1_spiral1 / Spiral1_R, 2)*l1_spiral1 / 40.0 + pow(l1_spiral1 / Spiral1_R, 4)*l1_spiral1 / 3456.0 - pow(l1_spiral1 / Spiral1_R, 6)*l1_spiral1 / 599040.0;
		y1_spiral1 = pow(l1_spiral1, 2) / 6.0 / Spiral1_R - pow(l1_spiral1 / Spiral1_R, 3)*l1_spiral1 / 336.0 + pow(l1_spiral1 / Spiral1_R, 5)*l1_spiral1 / 42240.0;
		y1_spiral1 = Spiral1_ZorY * fabs(y1_spiral1);

		x1_spiral2 = l1_spiral2 - pow(l1_spiral2 / Spiral2_R, 2)*l1_spiral2 / 40.0 + pow(l1_spiral2 / Spiral2_R, 4)*l1_spiral2 / 3456.0 - pow(l1_spiral2 / Spiral2_R, 6)*l1_spiral2 / 599040.0;
		y1_spiral2 = pow(l1_spiral2, 2) / 6.0 / Spiral2_R - pow(l1_spiral2 / Spiral2_R, 3)*l1_spiral2 / 336.0 + pow(l1_spiral2 / Spiral2_R, 5)*l1_spiral2 / 42240.0;
		y1_spiral2 = Spiral2_ZorY * fabs(y1_spiral2);

		//3.计算新的x2,y2
		x1 = (Spiral2_Spt[0] - Spiral1_Spt[0] + x1_spiral2 * cos(Spiral2_Spt[2]) - y1_spiral2 * sin(Spiral2_Spt[2]) + y1_spiral1 * sin(Spiral1_Spt[2])) / cos(Spiral1_Spt[2]);
		//    x1 = (Spiral2_Spt[1] - Spiral1_Spt[1] + x1_spiral2*sin(Spiral2_Spt[2])+y1_spiral2*cos(Spiral2_Spt[2])-y1_spiral1*cos(Spiral1_Spt[2]))/sin(Spiral1_Spt[2]);
		x2 = (Spiral1_Spt[1] - Spiral2_Spt[1] + x1_spiral1 * sin(Spiral1_Spt[2]) + y1_spiral1 * cos(Spiral1_Spt[2]) - y1_spiral2 * cos(Spiral2_Spt[2])) / sin(Spiral2_Spt[2]);
		//ads_printf(L"spt2.x=%lf spt1.x=%lf x1=%lf x2=%lf y1=%lf y2=%lf fwj1=%lf fwj2=%lf x1=%lf x2=%lf cosa1=%lf cosa2=%lf %lf %lf %lf %lf\n",
		//		   Spiral1_Spt[0],Spiral2_Spt[0],
		//		   x1_spiral1,x1_spiral2,
		//		   y1_spiral1,y1_spiral2,
		//		   Spiral1_Spt[2],Spiral2_Spt[2],x1,x2,cos(Spiral1_Spt[2]),cos(Spiral2_Spt[2]),
		//		   Spiral2_Spt[0] - Spiral1_Spt[0], x1_spiral2*cos(Spiral2_Spt[2]),-y1_spiral2*sin(Spiral2_Spt[2]),y1_spiral1*sin(Spiral1_Spt[2]));		
		//
		//4.计算新的l2
		l2_spiral1 = fabs(x1) + pow(l1_spiral1 / Spiral1_R, 2)*l1_spiral1 / 40.0 - pow(l1_spiral1 / Spiral1_R, 4)*l1_spiral1 / 3456.0 + pow(l1_spiral1 / Spiral1_R, 6)*l1_spiral1 / 599040.0;
		l2_spiral2 = fabs(x2) + pow(l1_spiral2 / Spiral2_R, 2)*l1_spiral2 / 40.0 - pow(l1_spiral2 / Spiral2_R, 4)*l1_spiral2 / 3456.0 + pow(l1_spiral2 / Spiral2_R, 6)*l1_spiral2 / 599040.0;
	}

	//5.比较l1,l2
	//ads_printf(L"2:%lf %lf %lf %lf %lf %lf\n",l2_spiral1,l2_spiral2,Spiral1_Len,Spiral2_Len,fabs(l1_spiral1-l2_spiral1),fabs(l1_spiral2-l2_spiral2));
	if (l2_spiral1 > -0.001 && l2_spiral1<Spiral1_Len + 0.001 && l2_spiral2>-0.001 && l2_spiral2 < Spiral2_Len + 0.001
		&& fabs(l1_spiral1 - l2_spiral1) <= 0.001 &&
		fabs(l1_spiral2 - l2_spiral2) <= 0.001)
		return 1;

	return 0;
}

int BAS_DRAW_FUN::CalJD_XY1_XY2(double XY1[10], double XY2[10], double ResPt[3], double ResPt1[3])
{
	int JDNUM;

	JDNUM = 0;

	if (XY1[0] > 0.1 && XY2[0] > 0.1)
	{
		if (XY1[0] < 1.1)//直线
		{
			if (XY2[0] < 1.1)//直线
				JDNUM = CalJD_ZX_ZX(XY1, XY2, ResPt);
			else if (XY2[0] < 2.1)//圆
				JDNUM = CalJD_ZX_ARC(XY1, XY2, ResPt, ResPt1);
			else
			{
				//if (checkIfXJ(XY1,XY2,5))
				//{

				JDNUM = CalJD_ZX_Spiral(XY1, XY2, ResPt, ResPt1);
				//}	
				//else
				//	JDNUM=0;
			}
		}
		else if (XY1[0] < 2.1)//圆
		{
			if (XY2[0] < 1.1)//直线
				JDNUM = CalJD_ZX_ARC(XY2, XY1, ResPt, ResPt1);
			else if (XY2[0] < 2.1)//圆
				JDNUM = CalJD_Arc_Arc(XY1, XY2, ResPt, ResPt1);
			else
			{
				//if (checkIfXJ(XY1,XY2,5))
				//{
				JDNUM = CalJD_ARC_Spiral(XY1, XY2, ResPt, ResPt1);
				//}	
				//else
				//	JDNUM=0;
			}

		}
		else
		{
			if (XY2[0] < 1.1)//直线
			{
				//if (checkIfXJ(XY1,XY2,5))
				//{

				JDNUM = CalJD_ZX_Spiral(XY2, XY1, ResPt, ResPt1);
				/*}*/
				//else
				//	JDNUM=0;
			}

			else if (XY2[0] < 2.1)//圆
			{
				//if (checkIfXJ(XY1,XY2,5))
				//{
				JDNUM = CalJD_ARC_Spiral(XY2, XY1, ResPt, ResPt1);
				//}	
				//else
				//	JDNUM=0;
			}
			else
			{
				if (checkIfXJ(XY1, XY2, 1))
				{

					JDNUM = CalJD_Spiral_Spiral(XY1, XY2, ResPt, ResPt1);
				}
				else
					JDNUM = 0;
			}
		}
	}
	if (JDNUM > 1)
	{
		double dist = sqrt((ResPt[0] - ResPt1[0])*(ResPt[0] - ResPt1[0]) + (ResPt[1] - ResPt1[1])*(ResPt[1] - ResPt1[1]));
		if (dist < 0.001)
		{
			JDNUM = 1;
		}
	}
	return JDNUM;
}

void BAS_DRAW_FUN::CalLineExtension(int n, double l, double &xval, double &yval)
{
	double jc;

	xval = 0.0;
	jc = 1;
	int i;
	for (i = 1; i < n; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			jc *= j;
		}
		xval += pow(-1.0, i)*pow(l, 2 * i) / jc;
	}

	yval = 0.0;
	jc = 1;

	for (i = 1; i < n; i++)
	{
		for (int j = 1; j <= i; j++)
			jc *= j;
		yval += pow(-1.0, i)*pow(l, 2 * i + 1) / jc / (2 * i + 1);
	}
}

double  BAS_DRAW_FUN::Circle_XY_to_ML(double x, double y, double array2[10])
{
	double x1, y1, x2, y2, s1, s2, s, ss1, pt[3];
	s1 = array2[6];
	s2 = s1 + array2[2];
	double R = array2[1];
	x1 = array2[8], y1 = array2[9];
	GetXYonSomeXY(array2, array2[2], pt);
	x2 = pt[0], y2 = pt[1];
	ss1 = sqrt((x - x1)*(x - x1) + (y - y1)*(y - y1));
	s = s1 + R * 2 * asin(0.5*ss1 / R);

	double sfwj = array2[5];
	double szx = array2[4];

	double Xo, Yo, fwj1, fwj2, zxj;
	Xo = x1 + R * cos(sfwj + szx * 0.5*pi);
	Yo = y1 + R * sin(sfwj + szx * 0.5*pi);

	xyddaa(Xo, Yo, x1, y1, &fwj1);
	xyddaa(Xo, Yo, x, y, &fwj2);
	zxj = ZXJ(fwj2, fwj1);
	//ads_printf(L"s=%lf zxj=%lf zx=%lf %lf %lf %lf %lf\n",s,zxj,szx,Xo,Yo,x,y);
	if (zxj*szx < 0.0)
		s = s1 + R * 2 * pi - R * 2 * asin(0.5*ss1 / R);
	// ss2 = sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
	// s = (s1*ss2+s2*ss1)/(ss1+ss2);

	return s;
}

bool BAS_DRAW_FUN::CheckIfOnArc(double array[10], double pt[3])
{
	bool inf;
	inf = false;
	double fwj1, fwj2, fwj, fwjmin, fwjmax;

	double Arc_Len, R, Xo, Yo, x, y, zx, zj;
	R = array[1];
	Arc_Len = array[2];
	x = array[8], y = array[9];
	fwj = array[5];
	zx = array[4];
	Xo = x + R * cos(fwj + 0.5*pi*zx);
	Yo = y + R * sin(fwj + 0.5*pi*zx);

	xyddaa(x, y, Xo, Yo, &fwj1);
	StdJiao(fwj1);
	xyddaa(pt[0], pt[1], Xo, Yo, &fwj);
	StdJiao(fwj);
	zj = zx * Arc_Len / R;
	fwj2 = fwj1 + zj;
	StdJiao(fwj2);
	fwjmin = fwj1 < fwj2 ? fwj1 : fwj2;
	fwjmax = fwj1 > fwj2 ? fwj1 : fwj2;
	//ads_printf(L"fwj=%lf fwj1=%lf fwj2=%lf\n",fwj,fwjmin,fwjmax);
	if (fwj > fwjmin - 0.00001 && fwj < fwjmax + 0.00001)
		return true;
	return false;
}

double BAS_DRAW_FUN::caldistance(ads_point pt1, ads_point pt2)
{
	double len, dx, dy;

	dx = (pt1[X] - pt2[X]);
	dy = (pt1[Y] - pt2[Y]);
	len = sqrt(dx*dx + dy * dy);
	return len;
}

void BAS_DRAW_FUN::CreateAFace(double p0[3], double p1[3], double p2[3], Adesk::UInt16 color)
{
	AcGePoint3d pt0(p0[1], p0[0], p0[2]);
	AcGePoint3d pt1(p1[1], p1[0], p1[2]);
	AcGePoint3d pt2(p2[1], p2[0], p2[2]);

	AcDbFace  *AFace = new AcDbFace(pt0, pt1, pt2);
	AFace->setColorIndex(color);
	AcDbObjectId FaceId;
	FaceId = AddEntityToDbs(AFace);
}

void BAS_DRAW_FUN::CreateAFace(AcGePoint3d &p0, AcGePoint3d &p1, AcGePoint3d &p2, AcGePoint3d &p3, Adesk::UInt16 color, ACHAR *LayerName)
{

	AcDbFace  *AFace = new AcDbFace(p0, p1, p2, p3);
	AFace->setColorIndex(color);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	AFace->setLayer(LayerTabRecordId);
	pLayerTable->close();

	AcDbObjectId FaceId;
	FaceId = AddEntityToDbs(AFace);
}

void BAS_DRAW_FUN::CreateFace(AcGePoint3d &p0, AcGePoint3d &p1, AcGePoint3d &p2, Adesk::UInt16 color, ACHAR *LayerName)
{

	AcDbFace  *AFace = new AcDbFace(p0, p1, p2);
	AFace->setColorIndex(color);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	AFace->setLayer(LayerTabRecordId);
	pLayerTable->close();

	AcDbObjectId FaceId;
	FaceId = AddEntityToDbs(AFace);
}

void BAS_DRAW_FUN::CreateFace(double p0[3], double p1[3], double p2[3], Adesk::UInt16 color, ACHAR *LayerName)
{

	AcGePoint3d pt0, pt1, pt2;
	pt0.x = p0[1]; pt0.y = p0[0]; pt0.z = p0[2];
	pt1.x = p1[1]; pt1.y = p1[0]; pt1.z = p1[2];
	pt2.x = p2[1]; pt2.y = p2[0]; pt2.z = p2[2];

	AcDbFace  *AFace = new AcDbFace(pt0, pt1, pt2);
	AFace->setColorIndex(color);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	AFace->setLayer(LayerTabRecordId);
	pLayerTable->close();

	AcDbObjectId FaceId;
	FaceId = AddEntityToDbs(AFace);
}
//考虑加宽计算的路面宽
void BAS_DRAW_FUN::CalLMK_JK(double cml, int NLMK, LMKdata lmk[], int njk, CgSZ jk[], double &half_fgdk,
	double &lydk, double &cxdk, double &yljk, double &tljk, double &tljhp,
	int &bkmode, int &sjbgmode)
{
	double k;
	double addwid;
	int i;

	for (i = 0; i < NLMK; i++)
	{
		if (cml < lmk[i].cml + 0.1)
			break;
	}

	if (i == NLMK)
		i--;

	if (i > 0 && i<NLMK && fabs(lmk[i].cml - lmk[i - 1].cml)>0.0001)
		k = (cml - lmk[i - 1].cml) / (lmk[i].cml - lmk[i - 1].cml);
	else
		k = 0;

	if (lmk[i].bkmode != 0)
		k = 4 * k*k*k - 3 * k*k*k*k;
	bkmode = lmk[i].bkmode;
	sjbgmode = lmk[i].sjbgmode;

	double kk;
	addwid = 0.0;
	int j;
	for (j = 0; j < njk; j++)
	{
		if (cml < jk[j].cml + 0.1)
			break;
	}

	if (j > 0 && j < njk)
	{
		if (fabs(jk[j].cml - jk[j - 1].cml) > 0.0001)
		{
			kk = (cml - jk[j - 1].cml) / (jk[j].cml - jk[j - 1].cml);
			addwid = jk[j - 1].cg + kk * (jk[j].cg - jk[j - 1].cg);
		}
		else
			addwid = jk[j - 1].cg;
	}

	if (i > 0 && i < NLMK)
	{
		half_fgdk = lmk[i - 1].fgdk + k * (lmk[i].fgdk - lmk[i - 1].fgdk);
		lydk = lmk[i - 1].lydk + k * (lmk[i].lydk - lmk[i - 1].lydk);
		cxdk = lmk[i - 1].cxdk + k * (lmk[i].cxdk - lmk[i - 1].cxdk) + addwid;
		yljk = lmk[i - 1].yljk + k * (lmk[i].yljk - lmk[i - 1].yljk);
		tljk = lmk[i - 1].tljk + k * (lmk[i].tljk - lmk[i - 1].tljk);
		tljhp = lmk[i].tljhp;
	}
	else if (i == 0)
	{
		i = 1;
		half_fgdk = lmk[i - 1].fgdk;
		lydk = lmk[i - 1].lydk;
		cxdk = lmk[i - 1].cxdk;
		yljk = lmk[i - 1].yljk;
		tljk = lmk[i - 1].tljk;
		tljhp = lmk[i - 1].tljhp;
	}
	else
	{
		i = NLMK - 1;
		half_fgdk = lmk[i].fgdk;
		lydk = lmk[i].lydk;
		cxdk = lmk[i].cxdk;
		yljk = lmk[i].yljk;
		tljk = lmk[i].tljk;
		tljhp = lmk[i].tljhp;
	}
	//	 ads_printf(L"%lf %lf %lf %lf %lf\n", half_fgdk,lydk,cxdk,yljk,tljk,tljhp);	
}

//判断三角形trino内是否包含点PT 是 返回true 否 返回false 
bool BAS_DRAW_FUN::CheckPtInTri(AcGePoint3d PPT, AcGePoint3d ppt0, AcGePoint3d ppt1, AcGePoint3d ppt2)
{
	double pt0[2], pt1[2], pt2[2], pt[2], ZXpt[2];
	double snp01, snp12, snp20, znp01, znp12, znp20;
	double zmin, zmax;

	zmin = ppt0.z < ppt1.z ? ppt0.z : ppt1.z;
	zmin = ppt2.z < zmin ? ppt2.z : zmin;
	zmax = ppt0.z > ppt1.z ? ppt0.z : ppt1.z;
	zmax = ppt2.z > zmax ? ppt2.z : zmax;


	pt[0] = PPT.x; pt[1] = PPT.y;
	pt0[0] = ppt0.x, pt0[1] = ppt0.y;
	pt1[0] = ppt1.x, pt1[1] = ppt1.y;
	pt2[0] = ppt2.x, pt2[1] = ppt2.y;
	//重心
	ZXpt[0] = (ppt0.x + ppt1.x + ppt2.x) / 3;
	ZXpt[1] = (ppt0.y + ppt1.y + ppt2.y) / 3;

	snp01 = chkd(pt0, pt1, pt);
	znp01 = chkd(pt0, pt1, ZXpt);
	snp12 = chkd(pt1, pt2, pt);
	znp12 = chkd(pt1, pt2, ZXpt);
	snp20 = chkd(pt2, pt0, pt);
	znp20 = chkd(pt2, pt0, ZXpt);
	if (snp01*znp01 >= -0.00000001 && snp12*znp12 >= -0.00000001 && snp20*znp20 >= -0.00000001)
	{
		AcGePlane face(ppt0, ppt1, ppt2);
		if (face.distanceTo(PPT) < 0.000001 && PPT.z<zmax + 0.001 && PPT.z>zmin - 0.001)
			return true;
		else
			return false;
	}
	else
		return false;
}

double BAS_DRAW_FUN::chkd(double pt11[3], double pt22[3], double pt33[3])
{
	double c;
	double pt1[2], pt2[2], pt3[2], x, y;

	x = (pt11[0] + pt22[0]) / 2.0;
	y = (pt11[1] + pt22[1]) / 2.0;
	pt1[0] = pt11[0] - x;
	pt1[1] = pt11[1] - y;
	pt2[0] = pt22[0] - x;
	pt2[1] = pt22[1] - y;
	pt3[0] = pt33[0] - x;
	pt3[1] = pt33[1] - y;

	c = CCW2(pt1, pt2, pt3);

	return (c);
}

double BAS_DRAW_FUN::CCW2(double pt1[2], double pt2[2], double pt3[2])
{
	double x1, x2, x3, y1, y2, y3, bx, by;

	bx = (pt1[0] + pt2[0] + pt3[0]) / 3.0;
	by = (pt1[1] + pt2[1] + pt3[1]) / 3.0;
	x1 = pt1[0] - bx;
	x2 = pt2[0] - bx;
	x3 = pt3[0] - bx;
	y1 = pt1[1] - by;
	y2 = pt2[1] - by;
	y3 = pt3[1] - by;
	return (x1*(y2 - y3) - x2 * (y1 - y3) + x3 * (y1 - y2));
}


//初始化结果缓冲区
void BAS_DRAW_FUN::InitResBuf(struct  resbuf   *pTemp)
{
	if (pTemp == NULL)
		return;
	//	_tcscpy(pTemp->resval.rstring, L"");
		//memset(pTemp->resval.ihandle,0,8);
		//pTemp->resval.ihandle
		/*pTemp->resval.mnInt64=0;
		pTemp->resval.mnLongPtr=NULL;
		pTemp->resval.rbinary.buf=NULL;
		pTemp->resval.rbinary.clen=0;
		pTemp->resval.rint=0;
		pTemp->resval.rlname[0]=0L;
		pTemp->resval.rlname[1]=0L;
		pTemp->resval.rlong=0;
		pTemp->resval.rpoint[0]=0;
		pTemp->resval.rpoint[1]=0;
		pTemp->resval.rpoint[2]=0;
		pTemp->resval.rreal=0.0;*/

}

//对CurID 写入扩展数据 xydata[10] 和后接AftID  对AftID写入扩展数据前接CurID
void BAS_DRAW_FUN::SetXYXdata(AcDbObjectId CurID, AcDbObjectId AftID, double xydata[10])
{
	AcDbObject* pCurObj = NULL, *pAftObj = NULL;
	ACHAR appName[132];
	int i;
	_tcscpy(appName, L"xydata");
	//	ads_printf(L"@@@@@@@ ");
	//	for(i=0;i<10;i++)
	//	{
	//		ads_printf(L"%lf ",xydata[i]);		
	//	}
	//	ads_printf(L"\n");
//	if(acdbOpenObject(pCurObj, CurID, AcDb::kForRead)!= Acad::eOk)
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	if (acdbOpenObject(pCurObj, CurID, AcDb::kForWrite) != Acad::eOk)
	{
		;
		//		ads_printf(L"CurID=%ld\n",CurID);
		//		ads_printf(L"设置缓和曲线的扩展数据失败1!");
		return;
	}
	if ((acdbOpenObject(pAftObj, AftID, AcDb::kForWrite)) != Acad::eOk)
	{
		pAftObj = NULL;
		//		ads_printf(L"AftID=%ld\n",AftID);
	}
	// 加 xdata 数据 开始
	struct  resbuf  *pRb = NULL, *pTemp = NULL, *pTail = NULL;
	int Counter = 0;

	pRb = pCurObj->xData();
	AcDbHandle handle;
	ACHAR handleStr[256];

	if (pRb != NULL) //If xdata is 存在
	{
		for (pTemp = pRb; pTemp != NULL; pTemp = pTemp->rbnext)
		{
			if (pTemp->restype == 1005)
			{
				Counter++;
				if (Counter == 2)
				{
					if (pAftObj)
					{
						pAftObj->getAcDbHandle(handle);
						handle.getIntoAsciiBuffer(handleStr);
					}
					else
						_tcscpy(handleStr, L"");
					acutNewString(handleStr, pTemp->resval.rstring);
					Counter = 0;
				}
			}
			if (pTemp->restype == 1040)
			{
				if (Counter < 10)
					pTemp->resval.rreal = xydata[Counter];
				Counter++;
			}
		}
	}
	else // If xdata is not 存在, 
	{
		acdbRegApp(appName);
		pRb = acutNewRb(AcDb::kDxfRegAppName);
		pTemp = pRb;
		InitResBuf(pTemp);//初始化
	//	pTemp->resval.rstring
	//		= (ACHAR*) malloc(_tcslen(appName) + 1);
		acutNewString(appName, pTemp->resval.rstring);
		//_tcscpy(pTemp->resval.rstring, appName);
		pTemp->rbnext = NULL;

		pTemp = acutNewRb(1005);
		//		pAftObj->getAcDbHandle(handle);	
		//		handle.getIntoAsciiBuffer(handleStr);
		InitResBuf(pTemp);//初始化
		_tcscpy(handleStr, L"");
		acutNewString(handleStr, pTemp->resval.rstring);
		pTemp->rbnext = NULL;
		pRb->rbnext = pTemp;
		pTail = pTemp;

		pTemp = acutNewRb(1005);
		InitResBuf(pTemp);//初始化
		if (pAftObj)
		{
			pAftObj->getAcDbHandle(handle);
			handle.getIntoAsciiBuffer(handleStr);
		}
		else
			_tcscpy(handleStr, L"");
		acutNewString(handleStr, pTemp->resval.rstring);

		pTemp->rbnext = NULL;
		pTail->rbnext = pTemp;
		pTail = pTemp;
		for (i = 0; i < 10; i++)
		{
			pTemp = acutNewRb(1040);
			InitResBuf(pTemp);//初始化

			pTemp->resval.rreal = xydata[i];
			pTemp->rbnext = NULL;
			pTail->rbnext = pTemp;
			pTail = pTemp;
		}
	}
	pCurObj->upgradeOpen();
	pCurObj->setXData(pRb);
	pCurObj->close();
	if (pRb)
	{
		acutRelRb(pRb);
		pRb = NULL;
	}

	if (pAftObj == NULL)
		return;
	Counter = 0;
	pRb = pAftObj->xData();
	if (pRb != NULL) //If xdata is 存在
	{
		for (pTemp = pRb; pTemp != NULL; pTemp = pTemp->rbnext)
		{
			if (pTemp->restype == 1005)
				Counter++;
			if (Counter == 1)
			{
				pCurObj->getAcDbHandle(handle);
				handle.getIntoAsciiBuffer(handleStr);
				acutNewString(handleStr, pTemp->resval.rstring);
				break;
			}
		}
	}
	else // If xdata is not 存在, 
	{
		acdbRegApp(appName);
		pRb = acutNewRb(AcDb::kDxfRegAppName);
		pTemp = pRb;
		InitResBuf(pTemp);//初始化
	//	pTemp->resval.rstring
	//		= (ACHAR*) malloc(_tcslen(appName) + 1);
	//	_tcscpy(pTemp->resval.rstring, appName);	
		acutNewString(appName, pTemp->resval.rstring);

		pTemp->rbnext = NULL;
		pTail = pTemp;

		pTemp = acutNewRb(1005);
		InitResBuf(pTemp);//初始化

		pCurObj->getAcDbHandle(handle);
		handle.getIntoAsciiBuffer(handleStr);
		acutNewString(handleStr, pTemp->resval.rstring);

		pTemp->rbnext = NULL;
		pTail->rbnext = pTemp;
		pTail = pTemp;

		pTemp = acutNewRb(1005);
		InitResBuf(pTemp);//初始化
		_tcscpy(handleStr, L"");
		acutNewString(handleStr, pTemp->resval.rstring);
		pTemp->rbnext = NULL;
		pTail->rbnext = pTemp;
		pTail = pTemp;

		for (i = 0; i < 10; i++)
		{
			pTemp = acutNewRb(1040);
			InitResBuf(pTemp);//初始化

			pTemp->resval.rreal = xydata[i];
			pTemp->rbnext = NULL;
			pTail->rbnext = pTemp;
			pTail = pTemp;
		}
	}
	//	pAftObj->upgradeOpen();
	if (pRb)
		pAftObj->setXData(pRb);

	pAftObj->close();
	if (pRb)
	{
		acutRelRb(pRb);
		pRb = NULL;
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
}

//从CurID中得到线元数据 0 无扩展数据 1有扩展数据
int BAS_DRAW_FUN::GetXdataFromEntity(AcDbObjectId CurID, XYarray &xray)
{
	AcDbObject* pCurObj;
	Acad::ErrorStatus res;

	struct  resbuf  *pRb = NULL, *pTemp = NULL;
	int i;
	AcDbHandle handle;

	res = acdbOpenObject(pCurObj, CurID, AcDb::kForRead);
	ads_printf(L"\nAcDbEntity name=%s", pCurObj->isA()->name());
	pRb = pCurObj->xData();
	if (pRb == NULL)
	{
		pCurObj->close();
		return 0;
	}
	for (i = 0; i < 10; i++)
		xray.xyarr[i] = 0;
	if (res == Acad::eOk)
	{
		if (pCurObj->isKindOf(AcDbPolyline::desc())
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
					if (i == 10)
						break;
				}
			}
			else if (pCurObj->isKindOf(AcDbLine::desc())) //直线
			{
				pCurObj->close();
				AcDbLine *LINE = AcDbLine::cast(pCurObj);
				AcGePoint3d sPt, ePt;
				//			   acdbOpenObject(LINE,eId,AcDb::kForWrite);		
				sPt = LINE->startPoint();
				ePt = LINE->endPoint();

				xray.xyarr[0] = 1;
				xray.xyarr[1] = xyddaa(sPt.y, sPt.x, ePt.y, ePt.x, &xray.xyarr[5]);
				xray.xyarr[2] = xray.xyarr[3] = xray.xyarr[4] = 0;
				xray.xyarr[7] = 1e40;
				xray.xyarr[8] = sPt.y;
				xray.xyarr[9] = sPt.x;
			}
			else if (pCurObj->isKindOf(AcDbArc::desc()))//plh07.17圆曲线
			{
				pCurObj->close();
				AcDbArc *ARC = AcDbArc::cast(pCurObj);
				//			   acdbOpenObject(ARC,eId,AcDb::kForWrite);
				double j1, j2, x0, y0, R, yxj;
				j1 = ARC->startAngle();
				j2 = ARC->endAngle();
				x0 = ARC->center().y;
				y0 = ARC->center().x;
				R = ARC->radius();
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
					if (i == 10)
						break;
				}

				double fwj1, fwj2;
				fwj1 = FwjtoAngX(j1);
				fwj2 = FwjtoAngX(j2);
				xray.xyarr[0] = 2;
				xray.xyarr[1] = R;
				yxj = j2 - j1;
				if (yxj <= 0)
					yxj = yxj + 2 * PI;
				xray.xyarr[2] = R * yxj;
				xray.xyarr[3] = 0;
				xray.xyarr[7] = R;

				//求圆弧的起终点坐标
				if (xray.xyarr[4] < 0)
				{
					xray.xyarr[8] = x0 + R * cos(fwj1);
					xray.xyarr[9] = y0 + R * sin(fwj1);
					xray.xyarr[5] = fwj1 - 0.5*PI;
					if (xray.xyarr[5] < 0)
						xray.xyarr[5] += 2 * PI;
				}
				else
				{
					xray.xyarr[8] = x0 + R * cos(fwj2);
					xray.xyarr[9] = y0 + R * sin(fwj2);
					xray.xyarr[5] = fwj2 + 0.5*PI;
					if (xray.xyarr[5] > 2 * PI)
						xray.xyarr[5] -= 2 * PI;
				}
			}
			else if (pCurObj->isKindOf(AcDbCircle::desc()))//plh07.17
			{
				double R;
				pCurObj->close();
				AcDbCircle *Cir = AcDbCircle::cast(pCurObj);
				R = Cir->radius();
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
					if (i == 10)
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

void BAS_DRAW_FUN::FormXYarray(AcDbObjectId CurID, CArray<XYarray, XYarray>&pXYArr)
{
	AcDbObject* pCurObj;
	AcDbObjectId NextID, PreID;
	XYarray xray;
	Acad::ErrorStatus res;

	struct  resbuf  *pRb = NULL, *pTemp = NULL;
	int i;
	//	acdbGetObjectId(eId, en);
	AcDbHandle handle;

	pXYArr.RemoveAll();
	res = acdbOpenObject(pCurObj, CurID, AcDb::kForRead);
	//ads_printf(L"\nAcDbEntity name=%s",pCurObj->isA()->name());
	pRb = pCurObj->xData();
	if (pRb == NULL)
	{
		pCurObj->close();
		return;
	}
	//   pRb = pRb->
	if (pRb&&pRb->rbnext)
	{
		pTemp = pRb->rbnext;
		//	   ads_printf(L"type=%d",pTemp->restype);
		CString handleStr = pTemp->resval.rstring;
		handle = handleStr;
		acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(PreID, Adesk::kFalse, handle);
		//	   ads_printf(L"PreID=%ld %s\n",PreID,handleStr);
	}
	if (pRb&&pRb->rbnext&&pRb->rbnext->rbnext)
	{
		pTemp = pRb->rbnext->rbnext;
		//	   ads_printf(L"type=%d",pRb->rbnext->rbnext->restype);
		CString handleStr = pTemp->resval.rstring;
		handle = handleStr;
		acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(NextID, Adesk::kFalse, handle);
		//	   ads_printf(L"NextID=%ld %s\n",NextID,handleStr);
	}

	while (res == Acad::eOk)
	{
		ads_printf(L"\nAcDbEntity1 name=%s", pCurObj->isA()->name());
		if (pCurObj->isKindOf(AcDbPolyline::desc())
			|| pCurObj->isKindOf(AcDbLine::desc())
			|| pCurObj->isKindOf(AcDbArc::desc())
			|| pCurObj->isKindOf(AcDbCircle::desc())
			)
		{
			for (i = 0; i < 10; i++)
				xray.xyarr[i] = 0;
			xray.eID = CurID;

			i = 0;
			pTemp = pRb->rbnext->rbnext->rbnext;
			while (pTemp)
			{
				xray.xyarr[i] = pTemp->resval.rreal;
				pTemp = pTemp->rbnext;
				i++;
				if (i == 10)
					break;
			}
			if (pCurObj->isKindOf(AcDbPolyline::desc())) //缓和曲线
			{
				pCurObj->close();
				pXYArr.Add(xray);
			}
			else if (pCurObj->isKindOf(AcDbLine::desc())) //直线
			{
				pCurObj->close();
				AcDbLine *LINE = AcDbLine::cast(pCurObj);
				AcGePoint3d sPt, ePt;
				//			   acdbOpenObject(LINE,eId,AcDb::kForWrite);		
				sPt = LINE->startPoint();
				ePt = LINE->endPoint();

				xray.xyarr[0] = 1;
				xray.xyarr[1] = xyddaa(sPt.y, sPt.x, ePt.y, ePt.x, &xray.xyarr[5]);
				xray.xyarr[2] = xray.xyarr[3] = xray.xyarr[4] = 0;
				xray.xyarr[7] = 1e40;
				xray.xyarr[8] = sPt.y;
				xray.xyarr[9] = sPt.x;
				pXYArr.Add(xray);
			}
			else if (pCurObj->isKindOf(AcDbArc::desc()))//plh07.17圆曲线
			{
				pCurObj->close();
				AcDbArc *ARC = AcDbArc::cast(pCurObj);
				//			   acdbOpenObject(ARC,eId,AcDb::kForWrite);
				double j1, j2, x0, y0, R, yxj;
				j1 = ARC->startAngle();
				j2 = ARC->endAngle();
				x0 = ARC->center().y;
				y0 = ARC->center().x;
				R = ARC->radius();
				//			   ARC->close();			   
				double fwj1, fwj2;
				fwj1 = FwjtoAngX(j1);
				fwj2 = FwjtoAngX(j2);
				xray.xyarr[0] = 2;
				xray.xyarr[1] = R;
				yxj = j2 - j1;
				if (yxj <= 0)
					yxj = yxj + 2 * PI;
				xray.xyarr[2] = R * yxj;
				xray.xyarr[3] = 0;
				xray.xyarr[7] = R;

				//求圆弧的起终点坐标
				if (xray.xyarr[4] < 0)
				{
					xray.xyarr[8] = x0 + R * cos(fwj1);
					xray.xyarr[9] = y0 + R * sin(fwj1);
					xray.xyarr[5] = fwj1 - 0.5*PI;
					if (xray.xyarr[5] < 0)
						xray.xyarr[5] += 2 * PI;
				}
				else
				{
					xray.xyarr[8] = x0 + R * cos(fwj2);
					xray.xyarr[9] = y0 + R * sin(fwj2);
					xray.xyarr[5] = fwj2 + 0.5*PI;
					if (xray.xyarr[5] > 2 * PI)
						xray.xyarr[5] -= 2 * PI;
				}
				//			   	ads_printf(L"arc: %0.3lf %0.3lf \n",xray.xyarr[8],xray.xyarr[9]);
				//两条边的方位角
				pXYArr.Add(xray);
			}
			else if (pCurObj->isKindOf(AcDbCircle::desc()))//plh07.17
			{
				double R;
				pCurObj->close();
				AcDbCircle *Cir = AcDbCircle::cast(pCurObj);
				//			   acdbOpenObject(ARC,eId,AcDb::kForWrite);
				R = Cir->radius();
				//			   ARC->close();
				/*
				i = 0;
				xray.xyarr[4]=-1;
				pTemp = pRb->rbnext->rbnext->rbnext;
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

				xray.xyarr[0] = -1;
				xray.xyarr[1] = R;
				pXYArr.Add(xray);
			}
		}
		else
		{
			pCurObj->close();
			ads_printf(L"\nSelected entity is not a Polyline!");
			return;
		}

		acutRelRb(pRb);
		res = acdbOpenObject(pCurObj, NextID, AcDb::kForRead);
		if (res == Acad::eOk)
		{
			//	    ads_printf(L"NextID=%ld\n",NextID);	
			pRb = pCurObj->xData();
			if (pRb&&pRb->rbnext&&pRb->rbnext->rbnext)
			{
				pTemp = pRb->rbnext->rbnext;
				//			   ads_printf(L"type=%d",pRb->rbnext->rbnext->restype);
				CString handleStr = pTemp->resval.rstring;
				handle = handleStr;
				acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(NextID, Adesk::kFalse, handle);
			}
		}
		CurID = NextID;
	}


	res = acdbOpenObject(pCurObj, PreID, AcDb::kForRead);
	if (pCurObj)
		pRb = pCurObj->xData();
	else
		pRb = NULL;
	CString handleStr;
	if (pRb&&pRb->rbnext)
	{
		handleStr = pRb->rbnext->resval.rstring;
		handle = handleStr;
		acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(PreID, Adesk::kFalse, handle);
	}
	while (res == Acad::eOk)
	{
		for (i = 0; i < 10; i++)
			xray.xyarr[i] = 0;
		xray.eID = PreID;
		if (pCurObj->isKindOf(AcDbPolyline::desc())
			|| pCurObj->isKindOf(AcDbLine::desc())
			|| pCurObj->isKindOf(AcDbArc::desc())
			|| pCurObj->isKindOf(AcDbCircle::desc())
			)
		{
			i = 0;
			pTemp = pRb->rbnext->rbnext->rbnext;
			while (pTemp)
			{
				xray.xyarr[i] = pTemp->resval.rreal;
				pTemp = pTemp->rbnext;
				i++;
				if (i == 10)
					break;
			}
			if (pCurObj->isKindOf(AcDbPolyline::desc()))
			{
				pCurObj->close();

				pXYArr.InsertAt(0, xray);
			}
			else if (pCurObj->isKindOf(AcDbLine::desc()))
			{
				pCurObj->close();
				AcDbLine *LINE = AcDbLine::cast(pCurObj);
				AcGePoint3d sPt, ePt;
				//			   acdbOpenObject(LINE,eId,AcDb::kForWrite);		
				sPt = LINE->startPoint();
				ePt = LINE->endPoint();

				xray.xyarr[0] = 1;
				xray.xyarr[1] = xyddaa(sPt.y, sPt.x, ePt.y, ePt.x, &xray.xyarr[5]);
				xray.xyarr[2] = xray.xyarr[3] = xray.xyarr[4] = 0;
				xray.xyarr[7] = 1e40;
				xray.xyarr[8] = sPt.y;
				xray.xyarr[9] = sPt.x;
				pXYArr.InsertAt(0, xray);
			}
			else if (pCurObj->isKindOf(AcDbArc::desc()))//plh07.17
			{
				pCurObj->close();
				AcDbArc *ARC = AcDbArc::cast(pCurObj);
				//			   acdbOpenObject(ARC,eId,AcDb::kForWrite);
				double j1, j2, x0, y0, R, yxj;
				j1 = ARC->startAngle();
				j2 = ARC->endAngle();
				x0 = ARC->center().y;
				y0 = ARC->center().x;
				R = ARC->radius();
				//			   ARC->close();
				/*
				i = 0;
				xray.xyarr[4]=-1;
				pTemp = pRb->rbnext->rbnext->rbnext;
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

				double fwj1, fwj2;
				fwj1 = FwjtoAngX(j1);
				fwj2 = FwjtoAngX(j2);
				xray.xyarr[0] = 2;
				xray.xyarr[1] = R;
				yxj = j2 - j1;
				if (yxj <= 0)
					yxj = yxj + 2 * PI;
				xray.xyarr[2] = R * yxj;
				xray.xyarr[3] = 0;
				xray.xyarr[7] = R;

				//求圆弧的起终点坐标
				if (xray.xyarr[4] < 0)
				{
					xray.xyarr[8] = x0 + R * cos(fwj1);
					xray.xyarr[9] = y0 + R * sin(fwj1);
					xray.xyarr[5] = fwj1 - 0.5*PI;
					if (xray.xyarr[5] < 0)
						xray.xyarr[5] += 2 * PI;
				}
				else
				{
					xray.xyarr[8] = x0 + R * cos(fwj2);
					xray.xyarr[9] = y0 + R * sin(fwj2);
					xray.xyarr[5] = fwj2 + 0.5*PI;
					if (xray.xyarr[5] > 2 * PI)
						xray.xyarr[5] -= 2 * PI;
				}
				pXYArr.InsertAt(0, xray);
			}
			else if (pCurObj->isKindOf(AcDbCircle::desc()))//plh07.17
			{
				pCurObj->close();
				AcDbCircle *Cir = AcDbCircle::cast(pCurObj);
				//			   acdbOpenObject(ARC,eId,AcDb::kForWrite);
				double R;
				R = Cir->radius();
				//			   ARC->close();

				xray.xyarr[0] = -1;
				xray.xyarr[1] = R;
				pXYArr.InsertAt(0, xray);
			}
		}
		else
		{
			pCurObj->close();
			ads_printf(L"\nSelected entity is not a Polyline!");
			return;
		}

		//		  ads_printf(L"####PreID=%ld\n",PreID);
		acutRelRb(pRb);
		res = acdbOpenObject(pCurObj, PreID, AcDb::kForRead);
		if (res == Acad::eOk)
		{
			pRb = pCurObj->xData();
			if (pRb&&pRb->rbnext)
			{
				handleStr = pRb->rbnext->resval.rstring;
				handle = handleStr;
				acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(PreID, Adesk::kFalse, handle);
			}
		}
	}

	double INDATA[5], len, sfwj, efwj, zj, Cenx, Ceny;
	int k, j;
	//对其中线元性质为-1 即圆作特殊处理
	for (i = 0; i < pXYArr.GetSize(); i++)
	{
		if (pXYArr[i].xyarr[0] < -0.1)
		{
			pXYArr[i].xyarr[0] = 2.0;
			pXYArr[i].xyarr[3] = 0.0;
			pXYArr[i].xyarr[7] = pXYArr[i].xyarr[1];
			if (i == 0)
			{
				efwj = pXYArr[i + 1].xyarr[5];
				sfwj = efwj - pXYArr[i].xyarr[4] * 5 * pi / 6;
				if (sfwj < 0)
					sfwj += 2 * pi;
				if (sfwj > 2 * pi)
					sfwj -= 2 * pi;
				pXYArr[i].xyarr[5] = sfwj;
				pXYArr[i].xyarr[2] = 5 * pi*pXYArr[i].xyarr[1] / 6;
				Cenx = pXYArr[i + 1].xyarr[8] + pXYArr[i].xyarr[1] * cos(efwj + pXYArr[i].xyarr[4] * 0.5*pi);
				Ceny = pXYArr[i + 1].xyarr[9] + pXYArr[i].xyarr[1] * sin(efwj + pXYArr[i].xyarr[4] * 0.5*pi);
				pXYArr[i].xyarr[8] = Cenx + pXYArr[i].xyarr[1] * cos(sfwj - pXYArr[i].xyarr[4] * 0.5*pi);
				pXYArr[i].xyarr[9] = Ceny + pXYArr[i].xyarr[1] * sin(sfwj - pXYArr[i].xyarr[4] * 0.5*pi);
			}
			else if (i == pXYArr.GetSize() - 1)
			{
				for (j = 0; j < 5; j++) INDATA[j] = pXYArr[i - 1].xyarr[j];
				len = Get_INDATA_L(INDATA, &k);
				xlpoint PBI, PZ;
				PBI.a = pXYArr[i - 1].xyarr[5];
				PBI.lc = pXYArr[i - 1].xyarr[6];
				PBI.r = pXYArr[i - 1].xyarr[7];
				PBI.x = pXYArr[i - 1].xyarr[8];
				PBI.y = pXYArr[i - 1].xyarr[9];
				// len 为该线元的长度 m
				xlzbxy(&PBI, &PZ, len, INDATA, 0);
				pXYArr[i].xyarr[8] = PZ.x;
				pXYArr[i].xyarr[9] = PZ.y;
				pXYArr[i].xyarr[5] = PZ.a;
				pXYArr[i].xyarr[2] = 5 * pi*pXYArr[i].xyarr[1] / 6;
			}
			/*
			else if(i>0&&i<pXYArr.GetSize()-1)
			{
			//3.计算连接圆线元数据

			for (int j=0;j<5;j++) INDATA[j]=pXYArr[i-1].xyarr[j];
			int k;

			len=Get_INDATA_L(INDATA,&k);
			xlpoint PBI,PZ;

			PBI.a  = pXYArr[i-1].xyarr[5];
			PBI.lc = pXYArr[i-1].xyarr[6];
			PBI.x  = pXYArr[i-1].xyarr[8];
			PBI.y  = pXYArr[i-1].xyarr[9];
			// len 为该线元的长度 m
			xlzbxy(&PBI,&PZ,len,INDATA,0);
			pXYArr[i].xyarr[8] = PZ.x;
			pXYArr[i].xyarr[9] = PZ.y;
			pXYArr[i].xyarr[5] = PZ.a;
			sfwj = 	PZ.a;
			//计算圆心到起点的方位角
			efwj = pXYArr[i+1].xyarr[5];
			if(pXYArr[i].xyarr[4]<0.0)//左转
			{
			sfwj +=0.5*PI;
			if(sfwj > 2*PI)
			sfwj-=2*PI;
			efwj +=0.5*PI;
			if(efwj > 2*PI)
			efwj-=2*PI;
			zj = sfwj - efwj;
			if(zj<0.0)
			zj+=2*PI;
			}
			else
			{
			sfwj -=0.5*PI;
			if(sfwj < 0)
			sfwj+=2*PI;
			efwj -=0.5*PI;
			if(efwj < 0)
			efwj+=2*PI;
			zj = efwj - sfwj;
			if(zj<0.0)
			zj+=2*PI;
			}
			pXYArr[i].xyarr[2] =  pXYArr[i].xyarr[1] * fabs(zj);
			}*/

		}
		if (fabs(pXYArr[i].xyarr[0] - 2.0) < 0.01)
		{
			if (i > 0 && i < pXYArr.GetSize() - 1)
			{
				//3.计算连接圆线元数据				

				for (int j = 0; j < 5; j++) INDATA[j] = pXYArr[i - 1].xyarr[j];
				int k;

				len = Get_INDATA_L(INDATA, &k);
				xlpoint PBI, PZ;

				PBI.a = pXYArr[i - 1].xyarr[5];
				PBI.lc = pXYArr[i - 1].xyarr[6];
				PBI.x = pXYArr[i - 1].xyarr[8];
				PBI.y = pXYArr[i - 1].xyarr[9];
				// len 为该线元的长度 m
				xlzbxy(&PBI, &PZ, len, INDATA, 0);
				pXYArr[i].xyarr[8] = PZ.x;
				pXYArr[i].xyarr[9] = PZ.y;
				pXYArr[i].xyarr[5] = PZ.a;
				sfwj = PZ.a;
				//计算圆心到起点的方位角
				efwj = pXYArr[i + 1].xyarr[5];
				if (pXYArr[i].xyarr[4] < 0.0)//左转
				{
					sfwj += 0.5*PI;
					if (sfwj > 2 * PI)
						sfwj -= 2 * PI;
					efwj += 0.5*PI;
					if (efwj > 2 * PI)
						efwj -= 2 * PI;
					zj = sfwj - efwj;
					if (zj < 0.0)
						zj += 2 * PI;
				}
				else
				{
					sfwj -= 0.5*PI;
					if (sfwj < 0)
						sfwj += 2 * PI;
					efwj -= 0.5*PI;
					if (efwj < 0)
						efwj += 2 * PI;
					zj = efwj - sfwj;
					if (zj < 0.0)
						zj += 2 * PI;
				}
				pXYArr[i].xyarr[2] = pXYArr[i].xyarr[1] * fabs(zj);
			}
		}
	}

	//	ads_printf(L"\nnum=%d\n",pXYArr.GetSize());
	k = pXYArr.GetSize();
	for (i = 0; i < 10; i++)
		xray.xyarr[i] = 0;
	for (i = 0; i < 5; i++) INDATA[i] = pXYArr[k - 1].xyarr[i];
	len = Get_INDATA_L(INDATA, &j);
	xlpoint PBI, PZ;
	PBI.a = pXYArr[k - 1].xyarr[5];
	PBI.lc = pXYArr[k - 1].xyarr[6];
	PBI.r = pXYArr[k - 1].xyarr[7];
	PBI.x = pXYArr[k - 1].xyarr[8];
	PBI.y = pXYArr[k - 1].xyarr[9];
	// len 为该线元的长度 m
	xlzbxy(&PBI, &PZ, len, INDATA, 0);
	xray.xyarr[8] = PZ.x;
	xray.xyarr[9] = PZ.y;
	xray.xyarr[5] = PZ.a;
	pXYArr.Add(xray);

	xray.xyarr[0] = 0;
	xray.xyarr[1] = pXYArr[0].xyarr[8];
	xray.xyarr[2] = pXYArr[0].xyarr[9];
	xray.xyarr[3] = pXYArr[0].xyarr[5];
	xray.xyarr[4] = pXYArr[0].xyarr[6];
	xray.xyarr[6] = pXYArr[0].xyarr[6];
	for (i = 7; i < 10; i++)
		xray.xyarr[i] = 0;
	pXYArr.InsertAt(0, xray);

	double Nsx, Nsy, Cex, Cey, fwj;
	for (i = 1; i < pXYArr.GetSize() - 1; i++)
	{
		if (pXYArr[i].xyarr[0] < 1.1)
		{
			Nsx = pXYArr[i + 1].xyarr[8];
			Nsy = pXYArr[i + 1].xyarr[9];
			CalXYXY(pXYArr[i].xyarr, pXYArr[i].xyarr[1], Cex, Cey, fwj);
			if (sqrt((Cex - Nsx)*(Cex - Nsx) + (Cey - Nsy)*(Cey - Nsy)) > 0.001)
			{
				len = sqrt((pXYArr[i].xyarr[8] - Nsx)*(pXYArr[i].xyarr[8] - Nsx) + (pXYArr[i].xyarr[9] - Nsy)*(pXYArr[i].xyarr[9] - Nsy));
				CalXYXY(pXYArr[i].xyarr, len, pXYArr[i].xyarr[8], pXYArr[i].xyarr[9], fwj);
			}
		}
		else if (pXYArr[i].xyarr[0] < 2.1)//修正线元终点
		{

			Nsx = pXYArr[i + 1].xyarr[8];
			Nsy = pXYArr[i + 1].xyarr[9];
			CalXYXY(pXYArr[i].xyarr, pXYArr[i].xyarr[2], Cex, Cey, fwj);

			if (sqrt((Cex - Nsx)*(Cex - Nsx) + (Cey - Nsy)*(Cey - Nsy)) > 0.001)
			{
				sfwj = pXYArr[i].xyarr[5];
				efwj = pXYArr[i + 1].xyarr[5];
				if (pXYArr[i].xyarr[4] < 0.0)//左转
				{
					sfwj += 0.5*PI;
					if (sfwj > 2 * PI)
						sfwj -= 2 * PI;
					efwj += 0.5*PI;
					if (efwj > 2 * PI)
						efwj -= 2 * PI;
					zj = sfwj - efwj;
					if (zj < 0.0)
						zj += 2 * PI;
				}
				else
				{
					sfwj -= 0.5*PI;
					if (sfwj < 0)
						sfwj += 2 * PI;
					efwj -= 0.5*PI;
					if (efwj < 0)
						efwj += 2 * PI;
					zj = efwj - sfwj;
					if (zj < 0.0)
						zj += 2 * PI;
				}
				len = pXYArr[i].xyarr[1] * zj;
				pXYArr[i].xyarr[2] = len;
			}
			//			ads_printf(L"i=%d R=%lf len=%lf ex=%lf ey=%lf sx=%lf sy=%lf\n",i,pXYArr[i].xyarr[1],len,Cex,Cey,Nsx,Nsy);
		}
	}

	for (i = 1; i < pXYArr.GetSize(); i++)
	{
		len = 0;
		for (k = 0; k < 5; k++) INDATA[k] = pXYArr[i - 1].xyarr[k];
		len = Get_INDATA_L(INDATA, &j);
		pXYArr[i].xyarr[6] = pXYArr[i - 1].xyarr[6] + len;
	}
	/*
	for(i=0;i<pXYArr.GetSize()-1;i++)
	{
	ads_printf(L"i=%d ",i);
	for(j=0;j<10;j++)
	ads_printf(L"%0.3lf ",pXYArr[i].xyarr[j]);
	ads_printf(L"\n");
	}	*/
}

/////////////////////////////////////////////////////////////////
// Descrip: 两单元(基本)模式法解算
// Calls:   AutoModel()      依据两圆关系，确定圆圆连接方式
// Inputs:  AcDbObjectId Id1 第一个实体ID号
//          AcDbObjectId Id2 第二个实体ID号
//          int& iLinkModel  连接模式:为圆圆连接时,0--S型 1--C型,其它连接时自动赋值
//          int LeftOrRight  第一段缓和曲线的偏向:-1--左偏 1--右偏
//          double A[2]      初始缓和曲线A值, 只有一段缓和曲线时,取A[0]    
// Outputs: double XYArray[2][10]   缓和曲线线元数据
//          int& iXYNum      缓和曲线个数:0--解算不成功
// Return:  bool             true--解算成功,false:--解算不成功
// Author:  ldf
// Date:    2005.07.14
/////////////////////////////////////////////////////////////////
bool BAS_DRAW_FUN::Msf2Elem(AcDbObjectId Id1, AcDbObjectId Id2, int& iLinkModel, int LeftOrRight, double A[2],
	double XYArray[2][10], int& iXYNum)
{
	int EntityType1, EntityType2; // 传入的实体类型:0--直线, 1--圆或圆弧
	AcGePoint3d SPt, EPt;     // 直线起终点
	double      dFwj;             // 方位角
	AcGePoint3d Center1, Center2; // 圆心坐标
	double      R1, R2;           // 半径
	SPIRAL      *pSprial1 = NULL, *pSprial2 = NULL;

	//???
	A[0] = A[1] = 1.0;

	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if (acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite) != Acad::eOk)
	{
		ads_printf(L"CAD文档打开错误!\n");
		return false;
	}

	// 打开Id1实体
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity, Id1, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	// 判断实体类型,并获取相关数据
	if (_tcscmp(pEntity->isA()->name(), L"AcDbCircle") == 0)
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		EntityType1 = 1;
		Center1 = m_pCircle->center();
		R1 = m_pCircle->radius();
	}
	else if (_tcscmp(pEntity->isA()->name(), L"AcDbArc") == 0)
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		EntityType1 = 1;
		Center1 = m_pArc->center();
		R1 = m_pArc->radius();
	}
	else if (_tcscmp(pEntity->isA()->name(), L"AcDbLine") == 0)
	{
		AcDbLine *m_pLine = AcDbLine::cast(pEntity);
		EntityType1 = 0;
		SPt = m_pLine->startPoint();
		EPt = m_pLine->endPoint();
		dFwj = fwj(m_pLine);
	}
	else
	{
		acutPrintf(L"所选实体不是直线、圆或圆弧!\n");
		return false;
	}
	pEntity->close(); // 关闭所选实体

	// 打开Id2实体
	if (acdbOpenObject(pEntity, Id2, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	// 判断实体类型,并获取相关数据
	if (_tcscmp(pEntity->isA()->name(), L"AcDbCircle") == 0)
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		EntityType2 = 1;
		Center2 = m_pCircle->center();
		R2 = m_pCircle->radius();

	}
	else if (_tcscmp(pEntity->isA()->name(), L"AcDbArc") == 0)
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		EntityType2 = 1;
		Center2 = m_pArc->center();
		R2 = m_pArc->radius();
	}
	else if (_tcscmp(pEntity->isA()->name(), L"AcDbLine") == 0)
	{
		AcDbLine *m_pLine = AcDbLine::cast(pEntity);
		EntityType2 = 0;
		SPt = m_pLine->startPoint();
		EPt = m_pLine->endPoint();
		dFwj = fwj(m_pLine);
	}
	else
	{
		acutPrintf(L"所选实体不是直线、圆或圆弧!\n");
		return false;
	}
	pEntity->close(); // 关闭所选实体

	iXYNum = 0;

	// 根据实体类型解算连接单元
	if ((EntityType1 == 0 && EntityType2 == 1) || (EntityType1 == 1 && EntityType2 == 0)) // 直圆连接
	{
		iLinkModel = -1;
		if (EntityType1 == 0) // 直圆
		{
			ZYLink LineCirObj(SPt[Y], SPt[X], dFwj, Center2[Y], Center2[X], R2, LeftOrRight);
			if (!LineCirObj.ERRFlag)
			{
				pSprial1 = &(LineCirObj.pSPIRAL);

				XYArray[0][0] = 3;
				XYArray[0][1] = pSprial1->A;
				XYArray[0][2] = pSprial1->EndR;
				XYArray[0][3] = 0;
				XYArray[0][4] = pSprial1->LeftOrRight;
				XYArray[0][5] = pSprial1->StartFwj;
				XYArray[0][6] = 0.0;
				XYArray[0][7] = pSprial1->EndR;
				XYArray[0][8] = pSprial1->StartN;
				XYArray[0][9] = pSprial1->StartE;

				iXYNum = 1;
			}
			else
			{
				ads_printf(L"###: 直圆无法连接过渡,请调整相关参数!\n");
				return false;
			}
		}
		else // 圆直
		{
			ZYLink LineCirObj(SPt[Y], SPt[X], dFwj, Center1[Y], Center1[X], R1, -1 * LeftOrRight);
			if (!LineCirObj.ERRFlag)
			{
				pSprial1 = &(LineCirObj.pSPIRAL);

				XYArray[0][0] = 4;
				XYArray[0][1] = pSprial1->A;
				XYArray[0][2] = pSprial1->EndR;
				XYArray[0][3] = 0;
				XYArray[0][4] = -1 * pSprial1->LeftOrRight;
				double dFwjTmp = pSprial1->EndFwj + PI;
				XYArray[0][5] = FormFwj(dFwjTmp);;
				XYArray[0][6] = 0.0;
				XYArray[0][7] = pSprial1->EndR;
				XYArray[0][8] = pSprial1->EndN;
				XYArray[0][9] = pSprial1->EndE;

				iXYNum = 1;
			}
			else
			{
				ads_printf(L"###: 圆直无法连接过渡,请调整相关参数!\n");
				return false;
			}
		}
	}
	else if (EntityType1 == 1 && EntityType2 == 1) // 圆圆连接 
	{
		int iLinkModelTmp;
		iLinkModelTmp = AutoModel(Center1, Center2, R1, R2);
		if (iLinkModelTmp == -1)
		{
			return false;
		}
		if (iLinkModelTmp != 0) // 两圆分离时,根据给定的连接方式解算
		{
			iLinkModel = iLinkModelTmp;
		}

		if (iLinkModel == 0) // S_MODEL
		{
			YYSLink YYSobj(Center1[Y], Center1[X], R1, Center2[Y], Center2[X], R2, A[0], A[1], LeftOrRight);
			if (!YYSobj.ERRFlag)
			{
				pSprial1 = &(YYSobj.pSPIRAL1);
				pSprial2 = &(YYSobj.pSPIRAL2);

				XYArray[0][0] = 4;
				XYArray[0][1] = pSprial1->A;
				XYArray[0][2] = pSprial1->EndR;
				XYArray[0][3] = 0;
				XYArray[0][4] = -1.0*pSprial1->LeftOrRight;
				XYArray[0][5] = pSprial1->EndFwj + PI;
				FormFwj(XYArray[0][5]);
				XYArray[0][6] = 0.0;
				XYArray[0][7] = pSprial1->EndR;
				XYArray[0][8] = pSprial1->EndN;
				XYArray[0][9] = pSprial1->EndE;

				XYArray[1][0] = 3;
				XYArray[1][1] = pSprial2->A;
				XYArray[1][2] = pSprial2->EndR;
				XYArray[1][3] = 0;
				XYArray[1][4] = pSprial2->LeftOrRight;
				XYArray[1][5] = pSprial2->StartFwj;
				XYArray[1][6] = pSprial1->EndL - pSprial1->StartL;
				XYArray[1][7] = pSprial2->EndR;
				XYArray[1][8] = pSprial2->StartN;
				XYArray[1][9] = pSprial2->StartE;

				iXYNum = 2;
			}
			else
			{
				ads_printf(L"###: 圆圆无法通过S型过渡,请调整相关参数!\n");
				return false;
			}
			//			acutPrintf(L"ComputerDRampParameters22\n");
		} // S_MODEL
		else if (iLinkModel == 1) // C_MODEL
		{
			YYCLink YYCobj(Center1[Y], Center1[X], R1, Center2[Y], Center2[X], R2, A[0], A[1], LeftOrRight);
			if (!YYCobj.ERRFlag)
			{
				pSprial1 = &(YYCobj.pSPIRAL1);
				pSprial2 = &(YYCobj.pSPIRAL2);

				XYArray[0][0] = 4;
				XYArray[0][1] = pSprial1->A;
				XYArray[0][2] = pSprial1->EndR;
				XYArray[0][3] = 0;
				XYArray[0][4] = -1.0*pSprial1->LeftOrRight;
				XYArray[0][5] = pSprial1->EndFwj + PI;
				FormFwj(XYArray[0][5]);
				XYArray[0][6] = 0.0;
				XYArray[0][7] = pSprial1->EndR;
				XYArray[0][8] = pSprial1->EndN;
				XYArray[0][9] = pSprial1->EndE;

				XYArray[1][0] = 3;
				XYArray[1][1] = pSprial2->A;
				XYArray[1][2] = pSprial2->EndR;
				XYArray[1][3] = 0;
				XYArray[1][4] = pSprial2->LeftOrRight;
				XYArray[1][5] = pSprial2->StartFwj;
				XYArray[1][6] = pSprial1->EndL - pSprial1->StartL;
				XYArray[1][7] = pSprial2->EndR;
				XYArray[1][8] = pSprial2->StartN;
				XYArray[1][9] = pSprial2->StartE;

				iXYNum = 2;
			}
			else
			{
				ads_printf(L"圆圆无法通过S型过渡,请调整相关参数!\n");
				return false;
			}
		} // C_MODEL
		else if (iLinkModel == 2) // 卵型 LUAN_MODEL
		{
			YYLuanLink YYLuanobj(Center1[Y], Center1[X], R1, Center2[Y], Center2[X], R2, LeftOrRight);
			if (!YYLuanobj.ERRFlag)
			{
				pSprial1 = &(YYLuanobj.pSPIRAL);

				if (R1 > R2)
				{
					XYArray[0][0] = 5;
					XYArray[0][2] = pSprial1->StartR;
					XYArray[0][3] = pSprial1->EndR;
					XYArray[0][4] = pSprial1->LeftOrRight;
					XYArray[0][5] = pSprial1->StartFwj;
					XYArray[0][6] = 0.0;
					XYArray[0][7] = pSprial1->StartR;
					XYArray[0][8] = pSprial1->StartN;
					XYArray[0][9] = pSprial1->StartE;
				}
				else
				{
					XYArray[0][0] = 6;
					XYArray[0][2] = pSprial1->EndR;
					XYArray[0][3] = pSprial1->StartR;
					XYArray[0][4] = -1 * pSprial1->LeftOrRight;
					XYArray[0][5] = pSprial1->EndFwj + PI;
					FormFwj(XYArray[0][5]);
					XYArray[0][7] = pSprial1->EndR;
					XYArray[0][8] = pSprial1->EndN;
					XYArray[0][9] = pSprial1->EndE;
				}
				XYArray[0][1] = pSprial1->A;
				XYArray[0][6] = 0.0;

				iXYNum = 1;
			}
			else
			{
				ads_printf(L"圆圆无法通过卵型过渡,请调整相关参数!\n");
				return false;
			}
		}
		else
		{
			ads_printf(L"###: 两圆无法过渡,请调整相关参数!\n");
			return false;
		}
	}
	else
	{
		acutPrintf(L"两实体不能进行连接设计!\n");
		return false;
	}

	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());

	if (iXYNum == 0)
	{
		return false;
	}

	return true;
}

// 依据两圆关系，自动确定连接方式 2005.07.14 ldf
int BAS_DRAW_FUN::AutoModel(AcGePoint3d Center1, AcGePoint3d Center2, double R1, double R2)
{
	int    iLinkModel = 3;
	double dist;
	dist = Center1.distanceTo(Center2);

	if (dist < 0.0001)
	{
		AfxMessageBox(L"两圆同心!");
		iLinkModel = -1;
	}
	else if (dist > (R1 + R2))
	{
		iLinkModel = 0; // S: 两圆分离
	}
	else if (dist < fabs(R1 - R2))
	{
		iLinkModel = 2; // 卵: 大圆包小圆
	}
	else if (dist < (R1 + R2))
	{
		iLinkModel = 1; // C: 两圆相交
	}

	return iLinkModel;
}


// 格式化方位角至0到2PI之间 2005.07.08 ldf
double BAS_DRAW_FUN::FormFwj(double& dFwj)
{
	while (dFwj < 0.0)
	{
		dFwj += 2 * PI;
	}
	if (dFwj > 2 * PI)
	{
		dFwj = fmod(dFwj, 2 * PI);
	}

	return dFwj;
}
// 绘制单个线元 2005.07.15 ldf
bool BAS_DRAW_FUN::DrawXY(double XYItem[10], AcDbObjectId& XYId, int iColor)
{
	bool bState;
	AcGePoint3d startPt(XYItem[9], XYItem[8], 0.0);

	if (fabs(XYItem[0] - 1.0) < DataPrecision) // 直线
	{
		bState = makeline(startPt, XYItem[5], XYItem[1], XYId, iColor);
	}
	else if (fabs(XYItem[0] - 2.0) < DataPrecision) // 圆弧
	{
		bState = makearc(startPt, XYItem[5], XYItem[1], XYItem[2], XYItem[4], XYId, iColor);
	}
	else if ((XYItem[0] > 2.0) && (XYItem[0] < 7.0)) // 缓和曲线(3-6)
	{
		SPIRAL SprialTmp;
		if (fabs(XYItem[0] - 3.0) < DataPrecision) // 前缓
		{
			SprialTmp.Set(startPt[Y], startPt[X], XYItem[5], XYItem[1], 1e40, XYItem[2], XYItem[4], iColor);
		}
		else if (fabs(XYItem[0] - 4.0) < DataPrecision) // 后缓
		{
			SprialTmp.Set2(startPt[Y], startPt[X], (XYItem[5] + PI), XYItem[1], 1e40, XYItem[2], -1 * XYItem[4], iColor);
		}
		else if (fabs(XYItem[0] - 5.0) < DataPrecision) // R大-->R小
		{
			SprialTmp.Set(startPt[Y], startPt[X], XYItem[5], XYItem[1], XYItem[2], XYItem[3], XYItem[4], iColor);
		}
		else if (fabs(XYItem[0] - 6.0) < DataPrecision) // R小-->R大
		{
			// 缓和曲线类标准样式规定为R大(包括无穷大)-->R小,所以赋值时一定要注意 2005.07.21 ldf
			SprialTmp.Set2(startPt[Y], startPt[X], XYItem[5] + PI, XYItem[1], XYItem[3], XYItem[2], -1 * XYItem[4], iColor);
		}

		if (XYId)
		{
			SprialTmp.SprialID = XYId;
			SprialTmp.DRAW();
		}
		else
		{
			SprialTmp.DRAW();
			XYId = SprialTmp.SprialID;
		}
		bState = true;
	}
	else
	{
		acutPrintf(L"非线元!\n");
		return false;
	}

	return bState;
}
// 由两点绘直线(指定颜色和ID) 2005.07.14 ldf
bool BAS_DRAW_FUN::makeline(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId& lineId, int iColor)
{
	AcDbLine *pLine = NULL;

	if (lineId == NULL) // 直线不存在,则创建新直线
	{
		pLine = new AcDbLine(startPt, endPt); // 新建直线
		pLine->setColorIndex(iColor);   // 设置直线颜色
		lineId = AddEntityToDbs(pLine);      // 将直线添加到数据库中,并显示
	}
	else // 直线已存在,则打开实体修改属性
	{
		if (acdbOpenObject(pLine, lineId, AcDb::kForWrite) != Acad::eOk)
		{
			lineId = NULL;
			acutPrintf(L"makeline:实体打开失败,释放实体ID!\n");
			return false;
		}
		pLine->assertWriteEnabled(Adesk::kFalse, Adesk::kFalse);
		pLine->setStartPoint(startPt);
		pLine->setEndPoint(endPt);
		pLine->setColorIndex(iColor);
		pLine->close(); // 关闭并显示实体
	}

	return true;
}

// 由起点和长度绘直线(指定颜色和ID) 2005.07.15 ldf
bool BAS_DRAW_FUN::makeline(AcGePoint3d startPt, double dFwj, double Len, AcDbObjectId& lineId, int iColor)
{
	AcDbLine *pLine = NULL;
	ads_point sPt, ePt;

	sPt[X] = startPt[X];
	sPt[Y] = startPt[Y];

	acutPolar(sPt, 0.5*PI - dFwj, Len, ePt);
	AcGePoint3d endPt(ePt[X], ePt[Y], 0.0);

	return makeline(startPt, endPt, lineId, iColor);
}
// 绘圆弧(指定ID和颜色) 2005.07.14 ldf
bool BAS_DRAW_FUN::makearc(AcGePoint3d startPt, double dFwj, double R, double Len, int LorR, AcDbObjectId& arcId, int iColor)
{
	ads_point sPt, cPt;
	double    dStartAngle, dEndAngle; // 起终角度
	double    dAngleTmp;

	sPt[X] = startPt[X];
	sPt[Y] = startPt[Y];

	dAngleTmp = (0.5*PI - dFwj) - 0.5*LorR*PI; // 先化成辐角,再加减90度到圆心方向
	acutPolar(sPt, dAngleTmp, R, cPt);

	dStartAngle = ads_angle(cPt, sPt);
	dAngleTmp = Len / R;
	dEndAngle = dStartAngle - LorR * dAngleTmp;

	//	acutPrintf(L"dStartAngle:%lf,%lf\n",dStartAngle,dEndAngle);

	if (LorR == 1) // 如果圆弧是右偏,调换起终辐角
	{
		dAngleTmp = dStartAngle;
		dStartAngle = dEndAngle;
		dEndAngle = dAngleTmp;
	}

	AcDbArc *pArc = NULL;
	AcGePoint3d centerPt(cPt[X], cPt[Y], 0.0);

	if (arcId == NULL)
	{
		pArc = new AcDbArc(centerPt, R, dStartAngle, dEndAngle); // 按逆时钟绘制
		pArc->setColorIndex(iColor);
		arcId = AddEntityToDbs(pArc);
	}
	else
	{
		if (acdbOpenObject(pArc, arcId, AcDb::kForWrite) != Acad::eOk)
		{
			arcId = NULL;
			acutPrintf(L"makearc:实体打开失败,释放实体ID!\n");
			return false;
		}
		pArc->assertWriteEnabled(Adesk::kFalse, Adesk::kFalse);
		pArc->setCenter(centerPt);
		pArc->setRadius(R);
		pArc->setStartAngle(dStartAngle);
		pArc->setEndAngle(dEndAngle);
		pArc->setColorIndex(iColor);
		pArc->close();
	}

	return true;
}

void BAS_DRAW_FUN::CalArrowPt(AcGePoint3d pt, double fwj, double RePt[2][2])
{
	RePt[0][0] = pt.x + 15 * cos(fwj - 5 * pi / 6);
	RePt[0][1] = pt.y + 15 * sin(fwj - 5 * pi / 6);
	RePt[1][0] = pt.x + 15 * cos(fwj + 5 * pi / 6);
	RePt[1][1] = pt.y + 15 * sin(fwj + 5 * pi / 6);
}

// 绘制AcDbPolyline 2005.07.15 ldf
bool BAS_DRAW_FUN::makepolyline(AcGePoint2dArray& vertices, AcDbObjectId& polylineId, int iColor)
{
	AcDbPolyline *pPolyline = NULL;
	int ArraySize = vertices.length();

	if (polylineId == NULL)
	{
		pPolyline = new AcDbPolyline();
		for (int i = 0; i < ArraySize; i++) // 将顶点集的点添加为多义线的顶点
		{
			pPolyline->addVertexAt(i, vertices[i]);
		}
		pPolyline->setColorIndex(iColor);
		polylineId = AddEntityToDbs(pPolyline);
	}
	else
	{
		if (acdbOpenObject(pPolyline, polylineId, AcDb::kForWrite) != Acad::eOk)
		{
			polylineId = NULL;
			acutPrintf(L"makepolyline:实体打开失败,释放实体ID!\n");
			return false;
		}

		int VertCount = pPolyline->numVerts();
		int i;
		for (i = 0; i < VertCount; i++) //删除原有线所有的顶点数据
		{
			pPolyline->removeVertexAt(0);
		}
		for (i = 0; i < ArraySize; i++) // 将新数据加入到多义线中
		{
			pPolyline->addVertexAt(i, vertices[i]);
		}
		pPolyline->setColorIndex(iColor);
		pPolyline->removeVertexAt(ArraySize); // 删除线创建基点	
		pPolyline->close();
	}

	return true;
}

void BAS_DRAW_FUN::CalXYXY(double XY[10], double len, double &x, double &y, double &fwj)
{
	double INDATA[5];
	for (int j = 0; j < 5; j++) INDATA[j] = XY[j];
	xlpoint PBI, PZ;
	PBI.a = XY[5];
	PBI.x = XY[8];
	PBI.y = XY[9];
	// len 为该线元的长度 m
	xlzbxy(&PBI, &PZ, len, INDATA, 0);
	x = PZ.x;
	y = PZ.y;
	fwj = PZ.a;
}

void BAS_DRAW_FUN::ReverseXY(double XY[10])
{
	double x, y, fwj, len, Rtmp;

	if (fabs(XY[0] - 1) < 0.1)
	{
		CalXYXY(XY, XY[1], x, y, fwj);
		XY[5] += PI;
		if (XY[5] > 2 * PI)
			XY[5] -= 2 * PI;
		XY[8] = x;
		XY[9] = y;
	}
	else if (fabs(XY[0] - 2) < 0.1)
	{
		CalXYXY(XY, XY[2], x, y, fwj);
		XY[4] = -XY[4];
		XY[5] = fwj + PI;
		if (XY[5] > 2 * PI)
			XY[5] -= 2 * PI;
		XY[8] = x;
		XY[9] = y;
	}
	else if (fabs(XY[0] - 3) < 0.1)
	{
		len = XY[1] * XY[1] / XY[2];
		CalXYXY(XY, len, x, y, fwj);
		XY[0] = 4;
		XY[4] = -XY[4];
		XY[5] = fwj + PI;
		if (XY[5] > 2 * PI)
			XY[5] -= 2 * PI;
		XY[8] = x;
		XY[9] = y;
	}
	else if (fabs(XY[0] - 4) < 0.1)
	{
		len = XY[1] * XY[1] / XY[2];
		CalXYXY(XY, len, x, y, fwj);
		XY[0] = 3;
		XY[4] = -XY[4];
		XY[5] = fwj + PI;
		if (XY[5] > 2 * PI)
			XY[5] -= 2 * PI;
		XY[8] = x;
		XY[9] = y;
	}
	else if (fabs(XY[0] - 5) < 0.1)
	{
		len = fabs(XY[1] * XY[1] / XY[2] - XY[1] * XY[1] / XY[3]);
		CalXYXY(XY, len, x, y, fwj);
		XY[0] = 6;
		XY[4] = -XY[4];
		XY[5] = fwj + PI;
		if (XY[5] > 2 * PI)
			XY[5] -= 2 * PI;
		XY[8] = x;
		XY[9] = y;
		Rtmp = XY[2];
		XY[2] = XY[3];
		XY[3] = Rtmp;
	}
	else if (fabs(XY[0] - 6) < 0.1)
	{
		len = fabs(XY[1] * XY[1] / XY[2] - XY[1] * XY[1] / XY[3]);
		CalXYXY(XY, len, x, y, fwj);
		XY[0] = 5;
		XY[4] = -XY[4];
		XY[5] = fwj + PI;
		if (XY[5] > 2 * PI)
			XY[5] -= 2 * PI;
		XY[8] = x;
		XY[9] = y;
		Rtmp = XY[2];
		XY[2] = XY[3];
		XY[3] = Rtmp;
	}
}
/////////////////////////////////////////////////////////////////
// Descrip: 三单元(扩展)模式法解算
// Calls:   AutoModel()      依据两圆关系，确定圆圆连接方式
// Inputs:  AcDbObjectId sId 起始单元ID号
//          AcDbObjectId cId 连接单元ID号
//          AcDbObjectId eId 终止单元ID号
//          int& iQLinkModel 前两单元连接模式:为圆圆连接时,0--S型 1--C型,其它连接时自动赋值
//          int& iHLinkModel 后两单元连接模式:为圆圆连接时,0--S型 1--C型,其它连接时自动赋值
//          int LeftOrRight  第一段缓和曲线的偏向:-1--左偏 1--右偏
//          double A[4]      初始缓和曲线A值, 只有一段缓和曲线时,前面取A[0],后面取A[3]    
// Outputs: double XYArray[5][10]   缓和曲线线元数据
//          int& iXYNum      缓和曲线个数:0--解算不成功
// Return:  bool             true--解算成功,false:--解算不成功
// Author:  ldf
// Date:    2005.07.19
/////////////////////////////////////////////////////////////////
bool BAS_DRAW_FUN::Msf3Elem(AcDbObjectId sId, AcDbObjectId cId, AcDbObjectId eId, int& iQLinkModel, int& iHLinkModel, int LeftOrRight, double A[4],
	double XYArray[5][10], int& iXYNum)
{
	int SXYNum, EXYNum, n, j;
	double ATmp[2];
	double SXYArray[2][10], EXYArray[2][10], MXY[10], INDATA[5];

	iXYNum = 0;
	SXYNum = EXYNum = 0;

	if (sId == 0 && eId == 0)
	{
		acutPrintf(L"");
		return false;
	}

	// 1.计算起始控制单元与连接元线元数据
	if (sId != 0) // 有起始单元
	{
		ATmp[0] = A[0];
		ATmp[1] = A[1];

		if (!Msf2Elem(sId, cId, iQLinkModel, LeftOrRight, ATmp, SXYArray, SXYNum))
		{
			iXYNum = 0;
			return false;
		}
	}

	// 2.计算终止控制单元与连接元线元数据
	if (eId != 0) // 有终止单元
	{
		ATmp[0] = A[3];
		ATmp[1] = A[4];

		if (iQLinkModel == 0) // 当前面为S型连接时,后面起始缓和曲线偏向与前面相反
		{
			if (!Msf2Elem(cId, eId, iHLinkModel, -1 * LeftOrRight, ATmp, EXYArray, EXYNum))
			{
				iXYNum = 0;
				return false;
			}
		}
		else
		{
			if (!Msf2Elem(cId, eId, iHLinkModel, LeftOrRight, ATmp, EXYArray, EXYNum))
			{
				iXYNum = 0;
				return false;
			}
		}
	}

	for (n = 0; n < SXYNum; n++)
	{
		for (j = 0; j < 10; j++)
		{
			XYArray[n][j] = SXYArray[n][j];
		}
	}

	// 3.计算连接圆线元数据
	double len, sfwj, efwj, zxj; // 中间圆弧的长度、起终点方位角及转向角

	MXY[0] = 2.0;
	if (iQLinkModel == -1)
	{
		MXY[1] = SXYArray[0][2];
	}
	else if (iQLinkModel == 0 || iQLinkModel == 1)
	{
		MXY[1] = SXYArray[1][2];
	}
	else if (iQLinkModel == 2)
	{
		MXY[1] = SXYArray[0][3];
	}

	MXY[3] = 0.0;
	if (iQLinkModel == 0)
	{
		MXY[4] = -LeftOrRight;
	}
	else
	{
		MXY[4] = LeftOrRight;
	}
	int i;
	for (i = 0; i < 5; i++)
	{
		INDATA[i] = SXYArray[SXYNum - 1][i];
	}
	len = Get_INDATA_L(INDATA, &j);
	MXY[6] = SXYArray[SXYNum - 1][6] + len;
	MXY[7] = MXY[1];

	CalXYXY(SXYArray[SXYNum - 1], len, MXY[8], MXY[9], MXY[5]);

	sfwj = MXY[5];
	efwj = EXYArray[0][5];

	FormFwj(sfwj);
	FormFwj(efwj);
	zxj = efwj - sfwj;
	FormFwj(zxj);
	if (MXY[4] < 0.0) // 左转(逆时钟360度互补)
	{
		zxj = 2 * PI - zxj;
	}
	MXY[2] = MXY[1] * fabs(zxj);

	for (j = 0; j < 10; j++)
	{
		XYArray[n][j] = MXY[j];
	}

	// 4.生成整体线元数据
	for (i = 0; i < EXYNum; i++)
	{
		n++;
		for (j = 0; j < 10; j++)
		{
			XYArray[n][j] = EXYArray[i][j];
		}
	}
	iXYNum = n + 1;

	if (iXYNum <= 0)
	{
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////
// Descrip: 两单元(基本)模式法解算(固定起点及连接参数A)
// Calls:   AutoModel()      依据两圆关系，确定圆圆连接方式
// Inputs:  AcDbObjectId Id1 第一个实体ID号
//          AcDbObjectId Id2 第二个实体ID号,必须为圆或圆弧
//          AcGePoint3d BasePt 线形起点位置
//          double dFwj        起点方位角
//          int iLinkModel   连接模式:为圆圆连接时,0--S型 1--C型,其它连接时自动赋值
//          int LeftOrRight  第一段缓和曲线的偏向:-1--左偏 1--右偏
//          double A[2]      固定缓和曲线A值, 只有一段缓和曲线时,取A[1]    
// Outputs: double XYArray[2][10]   缓和曲线线元数据
//          int& iXYNum      缓和曲线个数:0--解算不成功
// Return:  bool             true--解算成功,false:--解算不成功
// Author:  ldf
// Date:    2005.07.24
/////////////////////////////////////////////////////////////////
bool BAS_DRAW_FUN::Msf2ElemFitA(AcDbObjectId Id1, AcDbObjectId Id2, AcGePoint3d BasePt, double dFwj, int iLinkModel, int LeftOrRight, double A[2],
	double XYArray[2][10], int& iXYNum)
{
	int EntityType1;              // 实体Id1的类型:0--直线, 1--圆或圆弧
	AcGePoint3d SPt, EPt;     // 直线起终点
	AcGePoint3d Center1, Center2; // 圆心坐标
	double      R1, R2;           // 半径
	SPIRAL      *pSprial1 = NULL, *pSprial2 = NULL;

	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if (acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite) != Acad::eOk)
	{
		ads_printf(L"CAD文档打开错误!\n");
		return false;
	}

	// 打开Id1实体
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity, Id1, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	// 判断实体类型,并获取相关数据
	if (_tcscmp(pEntity->isA()->name(), L"AcDbCircle") == 0)
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		EntityType1 = 1;
		Center1 = m_pCircle->center();
		R1 = m_pCircle->radius();
	}
	else if (_tcscmp(pEntity->isA()->name(), L"AcDbArc") == 0)
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		EntityType1 = 1;
		Center1 = m_pArc->center();
		R1 = m_pArc->radius();
	}
	else if (_tcscmp(pEntity->isA()->name(), L"AcDbLine") == 0)
	{
		AcDbLine *m_pLine = AcDbLine::cast(pEntity);
		EntityType1 = 0;
		SPt = m_pLine->startPoint();
		EPt = m_pLine->endPoint();
		dFwj = fwj(m_pLine);
	}
	else
	{
		acutPrintf(L"所选实体不是直线、圆或圆弧!\n");
		return false;
	}
	pEntity->close(); // 关闭所选实体

	// 打开Id2实体,必须确保为圆或圆弧
	if (acdbOpenObject(pEntity, Id2, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	// 判断实体类型,并获取相关数据
	if (_tcscmp(pEntity->isA()->name(), L"AcDbCircle") == 0)
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		Center2 = m_pCircle->center();
		R2 = m_pCircle->radius();
	}
	else if (_tcscmp(pEntity->isA()->name(), L"AcDbArc") == 0)
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		Center2 = m_pArc->center();
		R2 = m_pArc->radius();
	}
	else
	{
		acutPrintf(L"连接实体必须是圆或圆弧!\n");
		return false;
	}
	pEntity->close(); // 关闭所选实体

	iXYNum = 0;
	AcGePoint3d CenterTmp;

	// 根据实体类型解算连接单元
	if (EntityType1 == 0) // 直圆连接
	{
		SPIRAL SprialTmp(BasePt[Y], BasePt[X], dFwj, A[1], 1e40, R2, LeftOrRight);
		pSprial1 = &SprialTmp;

		XYArray[0][0] = 3;
		XYArray[0][1] = pSprial1->A;
		XYArray[0][2] = pSprial1->EndR;
		XYArray[0][3] = 0;
		XYArray[0][4] = pSprial1->LeftOrRight;
		XYArray[0][5] = pSprial1->StartFwj;
		XYArray[0][6] = 0.0;
		XYArray[0][7] = pSprial1->EndR;
		XYArray[0][8] = pSprial1->StartN;
		XYArray[0][9] = pSprial1->StartE;

		iXYNum = 1;

		// 计算连接圆的圆心坐标(缓和曲线终点处的圆心坐标)
		CenterTmp[X] = pSprial1->EndE + R2 * sin(pSprial1->EndFwj + LeftOrRight * PI / 2);
		CenterTmp[Y] = pSprial1->EndN + R2 * cos(pSprial1->EndFwj + LeftOrRight * PI / 2);
	}
	else if (EntityType1 == 1) // 圆圆连接 
	{
		int iLinkModelTmp;
		iLinkModelTmp = AutoModel(Center1, Center2, R1, R2);
		if (iLinkModelTmp == -1)
		{
			return false;
		}
		if (iLinkModelTmp != 0) // 两圆分离时,根据给定的连接方式解算
		{
			iLinkModel = iLinkModelTmp;
		}

		if (iLinkModel == 0 || iLinkModel == 1) // S_MODEL || C_MODEL
		{
			SPIRAL SprialTmp;
			SprialTmp.Set2(BasePt[Y], BasePt[X], dFwj + PI, A[0], 1e40, R1, -1 * LeftOrRight);
			pSprial1 = &SprialTmp;

			XYArray[0][0] = 4;
			XYArray[0][1] = pSprial1->A;
			XYArray[0][2] = pSprial1->EndR;
			XYArray[0][3] = 0;
			XYArray[0][4] = LeftOrRight;
			XYArray[0][5] = dFwj;
			XYArray[0][6] = 0.0;
			XYArray[0][7] = pSprial1->EndR;
			XYArray[0][8] = pSprial1->EndN;
			XYArray[0][9] = pSprial1->EndE;

			dFwj = pSprial1->StartFwj + PI;
			FormFwj(dFwj);
			if (iLinkModel == 0) // S型连接时后一个缓和曲线左右转相反
			{
				LeftOrRight = -1 * LeftOrRight;
			}
			SprialTmp.Set(pSprial1->StartN, pSprial1->StartE, dFwj, A[1], 1e40, R2, LeftOrRight);
			pSprial2 = &SprialTmp;

			XYArray[1][0] = 3;
			XYArray[1][1] = pSprial2->A;
			XYArray[1][2] = pSprial2->EndR;
			XYArray[1][3] = 0;
			XYArray[1][4] = pSprial2->LeftOrRight;
			XYArray[1][5] = pSprial2->StartFwj;
			XYArray[1][6] = pSprial1->EndL - pSprial1->StartL;
			XYArray[1][7] = pSprial2->EndR;
			XYArray[1][8] = pSprial2->StartN;
			XYArray[1][9] = pSprial2->StartE;

			iXYNum = 2;

			// 计算连接圆的圆心坐标(缓和曲线终点处的圆心坐标)
			CenterTmp[X] = pSprial2->EndE + R2 * sin(pSprial2->EndFwj + LeftOrRight * PI / 2);
			CenterTmp[Y] = pSprial2->EndN + R2 * cos(pSprial2->EndFwj + LeftOrRight * PI / 2);

		} // S_MODEL || C_MODEL
		else if (iLinkModel == 2) // 卵型 LUAN_MODEL
		{
			SPIRAL SprialTmp;
			if (R1 > R2)
			{
				SprialTmp.Set(BasePt[Y], BasePt[X], dFwj, A[1], R1, R2, LeftOrRight);
				pSprial1 = &SprialTmp;
				XYArray[0][0] = 5;
				XYArray[0][1] = pSprial1->A;
				XYArray[0][2] = pSprial1->StartR;
				XYArray[0][3] = pSprial1->EndR;
				XYArray[0][4] = pSprial1->LeftOrRight;
				XYArray[0][5] = pSprial1->StartFwj;
				XYArray[0][6] = 0.0;
				XYArray[0][7] = pSprial1->StartR;
				XYArray[0][8] = pSprial1->StartN;
				XYArray[0][9] = pSprial1->StartE;

				// 计算连接圆的圆心坐标(缓和曲线终点处的圆心坐标)
				CenterTmp[X] = pSprial1->EndE + R2 * sin(pSprial1->EndFwj + LeftOrRight * PI / 2);
				CenterTmp[Y] = pSprial1->EndN + R2 * cos(pSprial1->EndFwj + LeftOrRight * PI / 2);
			}
			else
			{
				SprialTmp.Set2(BasePt[Y], BasePt[X], dFwj + PI, A[1], R2, R1, -1 * LeftOrRight);
				pSprial1 = &SprialTmp;
				XYArray[0][0] = 6;
				XYArray[0][1] = pSprial1->A;
				XYArray[0][2] = pSprial1->EndR;
				XYArray[0][3] = pSprial1->StartR;
				XYArray[0][4] = -1 * pSprial1->LeftOrRight;
				XYArray[0][5] = pSprial1->EndFwj + PI;
				FormFwj(XYArray[0][5]);
				XYArray[0][6] = 0.0;
				XYArray[0][7] = pSprial1->EndR;
				XYArray[0][8] = pSprial1->EndN;
				XYArray[0][9] = pSprial1->EndE;

				// 计算连接圆的圆心坐标(缓和曲线终点处的圆心坐标)
				dFwj = pSprial1->StartFwj + PI;
				FormFwj(dFwj);
				CenterTmp[X] = pSprial1->StartE + R2 * sin(dFwj + LeftOrRight * PI / 2);
				CenterTmp[Y] = pSprial1->StartN + R2 * cos(dFwj + LeftOrRight * PI / 2);
			}
			iXYNum = 1;
		}
		else
		{
			ads_printf(L"###: 两圆无法过渡,请调整相关参数!\n");
			return false;
		}
	}
	else
	{
		acutPrintf(L"两实体不能进行连接设计!\n");
		return false;
	}

	// 修改连接圆的圆心坐标
	if (acdbOpenObject(pEntity, Id2, AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	// 判断实体类型,并获取相关数据
	if (_tcscmp(pEntity->isA()->name(), L"AcDbCircle") == 0)
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		m_pCircle->setCenter(CenterTmp);

	}
	else if (_tcscmp(pEntity->isA()->name(), L"AcDbArc") == 0)
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		m_pArc->setCenter(CenterTmp);
	}
	else
	{
		acutPrintf(L"连接实体必须是圆或圆弧!\n");
		return false;
	}
	pEntity->close(); // 关闭所选实体

	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());

	return true;
}

//////////////////////////////////////////////////////////////////////////
//                               绘圆
//////////////////////////////////////////////////////////////////////////

// 指定颜色和ID 2005.07.20 ldf
bool BAS_DRAW_FUN::makecircle(AcGePoint3d& centerPt, double R, AcDbObjectId& circleId, int iColor)
{
	AcDbCircle *pCircle = NULL;

	if (circleId == NULL) // 直线不存在,则创建新直线
	{
		AcGeVector3d normal(0.0, 0.0, 1.0);
		pCircle = new AcDbCircle(centerPt, normal, R);
		pCircle->setColorIndex(iColor);
		circleId = AddEntityToDbs(pCircle);
	}
	else // 圆已存在,则打开实体修改属性
	{
		if (acdbOpenObject(pCircle, circleId, AcDb::kForWrite) != Acad::eOk)
		{
			circleId = NULL;
			acutPrintf(L"makecircle:实体打开失败,释放实体ID!\n");
			return false;
		}
		pCircle->assertWriteEnabled(Adesk::kFalse, Adesk::kFalse);
		pCircle->setRadius(R);
		pCircle->setCenter(centerPt);
		pCircle->setColorIndex(iColor);
		pCircle->close(); // 关闭并显示实体
	}

	return true;
}

//输入点N，E,线路线元数据，返回点在线路左侧 -1 右侧 1
int BAS_DRAW_FUN::JudgePTLorR(double N, double E, double &dist, double array[4 * MAXJDNUM][10], int tarray)
{
	double fwj, cml;
	xlpoint PZ;

	cml = PROJ_ML(array, tarray, N, E, N, E);//投影到线路
	PZ.lc = cml;
	xlpoint_pz(array, tarray, &PZ);////
	fwj = PZ.a;
	dist = sqrt((N - PZ.x)*(N - PZ.x) + (E - PZ.y)*(E - PZ.y));//两个投影点距离

	if (dist < 0.001)//点在线路上
		return 0;

	double pt1[2], pt2[2], pt3[2];
	pt1[0] = PZ.x, pt1[1] = PZ.y;

	pt2[0] = PZ.x + 1.0*cos(PZ.a);
	pt2[1] = PZ.y + 1.0*sin(PZ.a);

	pt3[0] = N, pt3[1] = E;

	double  zy = CCW2(pt1, pt2, pt3);
	//	
	//	double fwjpttoxl;
	//	xyddaa(N,E,PZ.x,PZ.y,&fwjpttoxl);
	int LorR;
	//	double zj = ZXJ(fwjpttoxl,fwj);

	if (zy > 0.0)//
		LorR = 1;//点在右侧
	else
		LorR = -1;//点在左侧

	//	if(fabs(cml-189990.35)<0.1)
	//		ads_printf(L"%lf x1=%lf y1=%lf x2=%lf y2=%lf fwj1=%lf fwj2=%lf\n",dist,Pt1[0],Pt1[1],PZ.x,PZ.y,fwj,fwj1to2);
	return LorR;
}

//设置线路数据
void BAS_DRAW_FUN::SetXLXdata(AcDbObjectId CurID, CString mdbname, CString roadname)
{
	AcDbObject* pCurObj = NULL;
	ACHAR appName[132];
	_tcscpy(appName, L"xldata");

	if (acdbOpenObject(pCurObj, CurID, AcDb::kForRead) != Acad::eOk)
		return;

	// 加 xdata 数据 开始
	struct  resbuf  *pRb = NULL, *pTemp = NULL, *pTail = NULL;
	int Counter = 0;

	pRb = pCurObj->xData();
	AcDbHandle handle;
	acdbRegApp(appName);
	pRb = acutNewRb(AcDb::kDxfRegAppName);
	pTemp = pRb;
	pTemp->resval.rstring
		= (ACHAR*)malloc(_tcslen(appName) * 2 + 2);
	_tcscpy(pTemp->resval.rstring, appName);
	pTemp->rbnext = NULL;

	pTemp = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp->resval.rstring
		= (ACHAR*)malloc(2 * mdbname.GetLength() + 2);
	_tcscpy(pTemp->resval.rstring, mdbname);
	pTemp->rbnext = NULL;
	pRb->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp->resval.rstring
		= (ACHAR*)malloc(2 * roadname.GetLength() + 2);
	_tcscpy(pTemp->resval.rstring, roadname);
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pCurObj->upgradeOpen();
	pCurObj->setXData(pRb);
	pCurObj->close();
	acutRelRb(pRb);
}
bool BAS_DRAW_FUN::GetXLXdata(AcDbObjectId CurID, CString &mdbname, CString &roadname)
{
	AcDbObject* pCurObj;
	Acad::ErrorStatus res;
	struct  resbuf  *pRb = NULL, *pTemp = NULL;

	/*  res = acdbOpenObject(pCurObj, CurID, AcDb::kForRead);	*/
	if (res != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pRb = pCurObj->xData();
	if (pRb == NULL)
	{
		pCurObj->close();
		return false;
	}

	if (pRb&&pRb->rbnext)
	{
		pTemp = pRb->rbnext;
		mdbname = pTemp->resval.rstring;
		if (pTemp->rbnext)
		{
			pTemp = pTemp->rbnext;
			roadname = pTemp->resval.rstring;
		}
	}
	pCurObj->close();
	return true;
}
//设置端部横线数据
void BAS_DRAW_FUN::SetDBXdata(AcDbObjectId CurID, CString mdbname, CString roadname, double cml, CString HxBh, double curx, double cury, double prex, double prey, int xsw)
{
	AcDbObject* pCurObj = NULL;
	ACHAR appName[132];
	_tcscpy(appName, L"dbdata");

	if (acdbOpenObject(pCurObj, CurID, AcDb::kForRead) != Acad::eOk)
		return;

	// 加 xdata 数据 开始
	struct  resbuf  *pRb = NULL, *pTemp = NULL, *pTail = NULL;
	int Counter = 0;

	pRb = pCurObj->xData();
	AcDbHandle handle;
	acdbRegApp(appName);
	pRb = acutNewRb(AcDb::kDxfRegAppName);
	pTemp = pRb;
	pTemp->resval.rstring
		= (ACHAR*)malloc(_tcslen(appName) + 1);
	_tcscpy(pTemp->resval.rstring, appName);
	pTemp->rbnext = NULL;

	pTemp = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp->resval.rstring
		= (ACHAR*)malloc(mdbname.GetLength() + 1);
	_tcscpy(pTemp->resval.rstring, mdbname);
	pTemp->rbnext = NULL;
	pRb->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp->resval.rstring
		= (ACHAR*)malloc(roadname.GetLength() + 1);
	_tcscpy(pTemp->resval.rstring, roadname);
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(1040);
	pTemp->resval.rreal = cml;
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp->resval.rstring
		= (ACHAR*)malloc(HxBh.GetLength() + 1);
	_tcscpy(pTemp->resval.rstring, HxBh);    //横线标号     		
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(1040);
	pTemp->resval.rreal = curx;
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(1040);
	pTemp->resval.rreal = cury;
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(1040);
	pTemp->resval.rreal = prex;
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(1040);
	pTemp->resval.rreal = prey;
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pTemp = acutNewRb(1070);
	pTemp->resval.rint = xsw;
	pTemp->rbnext = NULL;
	pTail->rbnext = pTemp;
	pTail = pTemp;

	pCurObj->upgradeOpen();
	pCurObj->setXData(pRb);
	pCurObj->close();
	acutRelRb(pRb);
}
bool BAS_DRAW_FUN::GetDBXdata(AcDbObjectId CurID, CString &mdbname, CString &roadname, double &cml, CString &HxBh, double &curx, double &cury, double &prex, double &prey, int&xsw)
{
	AcDbObject* pCurObj;
	Acad::ErrorStatus res;
	struct  resbuf  *pRb = NULL, *pTemp = NULL;

	res = acdbOpenObject(pCurObj, CurID, AcDb::kForRead);
	pRb = pCurObj->xData();
	if (pRb == NULL)
	{
		pCurObj->close();
		return false;
	}

	if (pRb&&pRb->rbnext)
	{
		pTemp = pRb->rbnext;
		mdbname = pTemp->resval.rstring;
		if (pTemp->rbnext)
		{
			pTemp = pTemp->rbnext;
			roadname = pTemp->resval.rstring;
			if (pTemp->rbnext)
			{
				pTemp = pTemp->rbnext;
				cml = pTemp->resval.rreal;
				if (pTemp->rbnext)
				{
					pTemp = pTemp->rbnext;
					HxBh = pTemp->resval.rstring;
					if (pTemp->rbnext)
					{
						pTemp = pTemp->rbnext;
						curx = pTemp->resval.rreal;
						if (pTemp->rbnext)
						{
							pTemp = pTemp->rbnext;
							cury = pTemp->resval.rreal;
							if (pTemp->rbnext)
							{
								pTemp = pTemp->rbnext;
								prex = pTemp->resval.rreal;
								if (pTemp->rbnext)
								{
									pTemp = pTemp->rbnext;
									prey = pTemp->resval.rreal;
									if (pTemp->rbnext)
									{
										pTemp = pTemp->rbnext;
										xsw = pTemp->resval.rint;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	pCurObj->close();
	return true;
}

void BAS_DRAW_FUN::GetDBHXEntity(AcDbObjectIdArray& IdArr, AcArray<CString>& layerArray, CString mdbname, CString roadname, CString HxBh)
{
	AcDbObjectIdArray IdArr0;
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	getAllEntity(acdbHostApplicationServices()->workingDatabase(), IdArr0, layerArray);
	int i, xsw, num = IdArr0.logicalLength();
	double cml, tmp;
	CString mname, rname, xbh;
	for (i = 0; i < num; i++)
	{
		cml = -100;
		AcDbEntity *pEnt;
		rname = "";
		mname = "";
		if (Acad::eOk == acdbOpenObject(pEnt, IdArr0[i], AcDb::kForRead))
		{
			//			if(pEnt->isKindOf(AcDbPolyline::desc()))
			//			{
			GetDBXdata(IdArr0[i], mname, rname, cml, xbh, tmp, tmp, tmp, tmp, xsw);
			if (cml > -0.1&&mname == mdbname && rname == roadname && xbh == HxBh)
				IdArr.append(IdArr0[i]);
			//			}
		}
		pEnt->close();
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
}

void BAS_DRAW_FUN::MergeBX(BXLMKD &oldbx, BXLMKD &newbx, int inf, BXLMKD &resbx)
{
	int i;
	double minsml, maxeml;

	Point ptt;
	CArray<Point, Point> ptarr;
	minsml = oldbx.bxsml < newbx.bxsml ? oldbx.bxsml : newbx.bxsml;
	maxeml = oldbx.bxeml > newbx.bxeml ? oldbx.bxeml : newbx.bxeml;

	if (inf == 0)//以newbx为准 合并oldbx及newbx到resbx中
	{
		for (i = 0; i < newbx.ptnum; i++)
		{
			ptt.x = newbx.ptarr[i * 3];
			ptt.y = newbx.ptarr[i * 3 + 1];
			ptt.z = newbx.ptarr[i * 3 + 2];
			ptarr.Add(ptt);
		}
		for (i = oldbx.ptnum - 1; i >= 0; i--)
			if (oldbx.ptarr[i * 3 + 2] <= newbx.ptarr[2])
			{
				ptt.x = oldbx.ptarr[i * 3];
				ptt.y = oldbx.ptarr[i * 3 + 1];
				ptt.z = oldbx.ptarr[i * 3 + 2];
				ptarr.InsertAt(0, ptt);
			}
		for (i = 0; i < oldbx.ptnum; i++)
			if (oldbx.ptarr[i * 3 + 2] >= newbx.ptarr[(newbx.ptnum - 1) * 3 + 2])
			{
				ptt.x = oldbx.ptarr[i * 3];
				ptt.y = oldbx.ptarr[i * 3 + 1];
				ptt.z = oldbx.ptarr[i * 3 + 2];
				ptarr.Add(ptt);
			}
	}
	else//以oldbx为准 合并oldbx及newbx到resbx中
	{
		for (i = 0; i < oldbx.ptnum; i++)
		{
			ptt.x = oldbx.ptarr[i * 3];
			ptt.y = oldbx.ptarr[i * 3 + 1];
			ptt.z = oldbx.ptarr[i * 3 + 2];
			ptarr.Add(ptt);
		}
		for (i = newbx.ptnum - 1; i >= 0; i--)
			if (newbx.ptarr[i * 3 + 2] <= oldbx.ptarr[2])
			{
				ptt.x = newbx.ptarr[i * 3];
				ptt.y = newbx.ptarr[i * 3 + 1];
				ptt.z = newbx.ptarr[i * 3 + 2];
				ptarr.InsertAt(0, ptt);
			}
		for (i = 0; i < newbx.ptnum; i++)
			if (newbx.ptarr[i * 3 + 2] >= oldbx.ptarr[(oldbx.ptnum - 1) * 3 + 2])
			{
				ptt.x = newbx.ptarr[i * 3];
				ptt.y = newbx.ptarr[i * 3 + 1];
				ptt.z = newbx.ptarr[i * 3 + 2];
				ptarr.Add(ptt);
			}
	}
	resbx.bxsml = minsml;
	resbx.bxeml = maxeml;
	resbx.LorR = newbx.LorR;
	resbx.bxinf = newbx.bxinf;
	resbx.ptnum = ptarr.GetSize();
	resbx.ptarr = new double[resbx.ptnum * 3];
	for (i = 0; i < resbx.ptnum; i++)
	{
		resbx.ptarr[i * 3] = ptarr[i].x;
		resbx.ptarr[i * 3 + 1] = ptarr[i].y;
		resbx.ptarr[i * 3 + 2] = ptarr[i].z;
	}
}

void BAS_DRAW_FUN::GetLoMin(double DV, double &Lmin)
{
	//Lmin = 1.0E10;
	//if(fabs(DV-120)<0.1)
	//	Lmin = 100;
	//if(fabs(DV-100)<0.1)
	//	Lmin = 85;
	//if(fabs(DV-80)<0.1)
	//	Lmin = 70;
	//if(fabs(DV-60)<0.1)
	//	Lmin = 60;
	//if(fabs(DV-40)<0.1)
	//	Lmin = 40;
	//if(fabs(DV-30)<0.1)
	//	Lmin = 30;
	//if(fabs(DV-20)<0.1)
	//	Lmin = 20;
	//20190723	按最新规范计算各速度值下的最小缓和曲线
	Lmin = 1.0E10;
	if (fabs(DV - 120) < 0.1)
		Lmin = 100.0;
	if (fabs(DV - 100) < 0.1)
		Lmin = 85.0;
	if (fabs(DV - 80) < 0.1)
		Lmin = 70.0;
	if (fabs(DV - 60) < 0.1)
		Lmin = 50.0;
	if (fabs(DV - 40) < 0.1)
		Lmin = 35.0;
	if (fabs(DV - 30) < 0.1)
		Lmin = 25.0;
	if (fabs(DV - 20) < 0.1)
		Lmin = 20.0;

}


void BAS_DRAW_FUN::GetCirLenMin(double DV, double &Lmin)
{
	Lmin = 1.0E10;
	if (fabs(DV - 120) < 0.1)
		Lmin = 200;
	if (fabs(DV - 100) < 0.1)
		Lmin = 170;
	if (fabs(DV - 80) < 0.1)
		Lmin = 140;
	if (fabs(DV - 60) < 0.1)
		Lmin = 100;
	if (fabs(DV - 40) < 0.1)
		Lmin = 70;
	if (fabs(DV - 30) < 0.1)
		Lmin = 50;
	if (fabs(DV - 20) < 0.1)
		Lmin = 40;

}
bool BAS_DRAW_FUN::checkIfSamePm(double cml, double DV, double array2[][10], int trow2)
{
	bool res = true;
	double sml, eml;
	sml = cml - DV / 3.6*3.0;
	eml = cml + DV / 3.6*3.0;
	int ixy1, ixy2;
	ixy1 = FindiXY(sml, array2, trow2);
	ixy2 = FindiXY(eml, array2, trow2);
	if (ixy1 > 0 && ixy2 > 0)
	{
		if (ixy1 != ixy2)
			res = false;
	}
	return res;
}

int BAS_DRAW_FUN::FindiXY(double cml, double array2[][10], int trow2)
{
	int ixy;
	double l, dlc, lc;
	int i, j, k, bz = -99;
	double INDATA2[5], jslc;
	struct xlpoint PB0, PEZ, *PBI, *PEI, *PZ = NULL;
	PZ = new xlpoint;
	PB0.lc = array2[0][4];
	PB0.x = array2[0][1];
	PB0.y = array2[0][2];
	PB0.a = rad(array2[0][3]);
	PB0.r = 1e40;

	PZ->x = PB0.x;
	PZ->y = PB0.y;
	PZ->a = PB0.a;
	PZ->r = PB0.r;
	PZ->lc = cml;

	dlc = array2[0][0];
	jslc = PZ->lc;
	if (jslc > array2[trow2][6])
		jslc = array2[trow2][6];
	PBI = &PB0; PEI = &PEZ;
	i = 0; bz = 99;
	do
	{
		i = i + 1;
		if (i >= trow2) bz = -99;
		if (bz > 0)
		{
			// 第 i 线元
			for (k = 0; k < 5; k++) INDATA2[k] = array2[i][k];
			l = Get_INDATA_L(INDATA2, &j);
			if (i >= trow2 - 1)
				l += 0.1;//最后一个线元加一个毫米
			PBI->a = array2[i][5];
			PBI->lc = array2[i][6];
			PBI->r = array2[i][7];
			PBI->x = array2[i][8];
			PBI->y = array2[i][9];
			// l 为该线元的长度 m

			if ((jslc <= PBI->lc + l) && (jslc >= PBI->lc))
				// 计算点在 i 线元上
			{
				lc = jslc - PBI->lc;
				if (lc <= 0.0) lc = 0.000000000001;
				xlzbxy(PBI, PZ, lc, INDATA2, 0);
				k = 999;
				bz = -99;
			};
		}  // bz>0
	} while (bz > 0);
	if (PZ) delete PZ;
	if (k == 999)
	{
		ixy = i;
		return ixy;
	}
	else
	{
		return -1; // 无解
	}

}

bool BAS_DRAW_FUN::checkIfSameZm(double cml, double DV, double CM[], int NCM)
{
	bool res = true;
	double sml, eml;
	sml = cml - DV / 3.6*3.0;
	eml = cml + DV / 3.6*3.0;
	int ipd1, ipd2;
	ipd1 = FindiPD(sml, CM, NCM);
	ipd2 = FindiPD(eml, CM, NCM);
	if (ipd1 > 0 && ipd2 > 0)
	{
		if (ipd1 != ipd2)
			res = false;
	}
	return res;
}

int BAS_DRAW_FUN::FindiPD(double cml, double CM[], int NCM)
{
	for (int j = 1; j < NCM - 1; j++)
		if (CM[(j - 1) * 5 + 0] - 0.0001 <= cml && cml <= CM[j * 5 + 0] + 0.0001)
		{
			return j;
		}
	return -1;
}

void BAS_DRAW_FUN::GetArcPt(double spt[2], double ept[2], double R, int npt, double pt[])
{
	double wid;
	wid = sqrt((spt[0] - ept[0])*(spt[0] - ept[0]) + (spt[1] - ept[1])*(spt[1] - ept[1]));
	double a, b;
	a = sqrt(R*R - (0.5*wid)*(0.5*wid));
	double mid[2], cen[2];
	mid[0] = 0.5*(spt[0] + ept[0]);
	mid[1] = 0.5*(spt[1] + ept[1]);

	int i;
	for (i = 0; i < npt / 2; i++)
	{
		pt[i * 2] = spt[0] + i * wid / npt;
		b = sqrt(R*R - (mid[0] - pt[i * 2])*(mid[0] - pt[i * 2]));
		pt[i * 2 + 1] = spt[1] - (b - a);
	}
	int sno = npt / 2;
	for (i = 0; i < npt / 2; i++)
	{
		pt[(sno + i) * 2] = mid[0] + i * wid / npt;
		b = sqrt(R*R - (i*wid / npt)*(i*wid / npt));
		pt[(sno + i) * 2 + 1] = spt[1] - (b - a);
	}
}
/*------------------------------------------*/
struct resbuf *BAS_DRAW_FUN::get_resbuf(int code, struct resbuf *p)
{
	struct resbuf *rb;
	for (rb = p; (rb != NULL) && (rb->restype != code); rb = rb->rbnext);
	return(rb);
}

int BAS_DRAW_FUN::getrbpoint(ads_point pt, struct resbuf *ebuf, int code)
{
	struct resbuf *rb;

	if ((rb = get_resbuf(code, ebuf)) == NULL)
		return RTERROR;
	pt[X] = rb->resval.rpoint[X];
	pt[Y] = rb->resval.rpoint[Y];
	pt[Z] = rb->resval.rpoint[Z];
	return 1;
}

/*-------------------------------------------------------------------
;  Given starting point Ps, ending point PE of a piece of arc, bulge of arc,
;  find the ridius R and center coordinates.
;   return: (R (xo yo))
;-------------------------------------------------------------------*/
struct arc_lst *BAS_DRAW_FUN::findxyr(ads_point ps, ads_point pe, ads_real bulge)
{
	ads_real a, gama, cord, R, cita, angse, ang;
	ads_point cxy;
	struct arc_lst *newptr;

	a = atan(fabs(bulge));
	gama = 2.0 * a;
	cord = ads_distance(ps, pe);
	R = (0.5 * cord) / sin(gama);
	cita = 0.5 * PI - gama;
	angse = ads_angle(ps, pe);
	if (bulge > 0.0)
		ang = angse + cita;
	else
		ang = angse - cita;
	ads_polar(ps, ang, R, cxy);
	newptr = (struct arc_lst *)calloc(1, sizeof(struct arc_lst));
	newptr->R = R;
	newptr->xo = cxy[X];
	newptr->yo = cxy[Y];
	return(newptr);
}


/*-------------------------------------------------------------------
;  Given starting point Ps, ending point PE of a piece of arc, bulge of arc,
;  find the ridius R and center coordinates.
;   return: (R (xo yo))
;-------------------------------------------------------------------*/
struct point_lk *BAS_DRAW_FUN::findroot(ads_point ps, ads_point pe, struct arc_lst *pxyr, ads_real bulge,
	ads_point ps1, ads_point ps2)
{
	ads_real R, xo, yo, xs1, ys1, xs2, ys2, dx, dy, b, k, a1, b1, c1, del, x1, y1, x2, y2, pi2, aps, ape, alp1, alp2;
	bool OK;
	ads_point p1, p2, cxy;
	struct point_lk *newptr, *headptr, *tailptr;

	newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
	if (newptr)
	{
		newptr->next = NULL;
		headptr = newptr;
		tailptr = newptr;
	}

	R = pxyr->R;
	xo = pxyr->xo; yo = pxyr->yo;
	xs1 = ps1[X], ys1 = ps1[Y];
	xs2 = ps2[X], ys2 = ps2[Y];
	cxy[X] = xo; cxy[Y] = yo;

	dx = ps1[X] - ps2[X];
	OK = true;
	if (fabs(dx) > 0.00000001)
	{
		dy = ps1[Y] - ps2[Y];
		k = dy / dx;
		b = ps1[Y] - k * ps1[X];
		a1 = 1.0 + k * k;
		b1 = 2.0 * k * (b - yo) - 2.0 * xo;
		c1 = xo * xo + (b - yo) * (b - yo) - R * R;
		del = b1 * b1 - 4.0 * a1 * c1;
		if (del >= 0.0)
		{
			del = sqrt(del);
			x1 = (del - b1) / (2.0*a1);
			y1 = k * x1 + b;
			x2 = (del + b1) / (-2.0*a1);
			y2 = k * x2 + b;
		}
		else
			OK = false;
	}
	else
	{
		x1 = xs1; x2 = x1;
		del = R * R - (x1 - xo) * (x1 - xo);
		if (del >= 0.0)
		{
			del = sqrt(del);
			y1 = yo + del;
			y2 = yo - del;
		}
		else
			OK = false;
	}
	p1[X] = x1; p1[Y] = y1; p2[X] = x2; p2[Y] = y2; pi2 = 2.0 * PI;

	if (OK)
	{
		aps = ads_angle(cxy, ps);
		ape = ads_angle(cxy, pe);
		alp1 = ads_angle(cxy, p1);
		alp2 = ads_angle(cxy, p2);
		if (bulge > 0.0)
		{
			if (ape > aps)
			{
				if (alp1 >= aps && alp1 <= ape)
				{
					newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
					if (newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}
				}
				if (alp2 >= aps && alp2 <= ape)
				{
					newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
					if (newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p2[X]; newptr->pt[Y] = p2[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}
				}
			}
			else
			{
				if ((alp1 >= 0.0 && alp1 <= ape) || (alp1 >= aps && alp1 <= pi2))
				{
					newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
					if (newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}
				}
				if ((alp2 >= 0.0 && alp2 <= ape) || (alp2 >= aps && alp2 <= pi2))
				{
					newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
					if (newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p2[X]; newptr->pt[Y] = p2[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}
				}
			}
		}
		else
		{
			if (aps > ape)
			{
				if (alp1 >= ape && alp1 <= aps)
				{
					newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
					if (newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}
				}
				if (alp2 >= ape && alp2 <= aps)
				{
					newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
					if (newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p2[X]; newptr->pt[Y] = p2[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}
				}
			}
			else
			{
				if ((alp1 >= 0.0 && alp1 <= aps) || (alp1 >= ape && alp1 <= pi2))
				{
					newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
					if (newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}
				}
				if ((alp2 >= 0.0 && alp2 <= aps) || (alp2 >= ape && alp2 <= pi2))
				{
					newptr = (struct point_lk *)calloc(1, sizeof(struct point_lk));
					if (newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p2[X]; newptr->pt[Y] = p2[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}
				}
			}
		}
	}
	return headptr;
}
/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *BAS_DRAW_FUN::get_brkpt(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	struct ptt_node *Verlst;

	Verlst = get_Arcpt(Entlist, p0, p1);
	//   ads_alert(L"ok2");   
	if (Verlst->next == NULL)
		Verlst = get_plinept(Entlist, p0, p1);

	return Verlst;
}

/*------------------------------------------------------------------
;   get the intersection point xy between polyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *BAS_DRAW_FUN::getbrkpt(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	struct ptt_node *Verlst;

	Verlst = getArcpt(Entlist, p0, p1);
	if (Verlst->next == NULL)
		Verlst = getplinept(Entlist, p0, p1);

	return Verlst;
}

/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *BAS_DRAW_FUN::get_plinept(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	ads_point pt0, pt1, p00, mptn, mpto, PTS;
	struct resbuf *elst, *tlst, *plst;
	//  ads_name tname,result;
	struct ptt_node *newptr, *headptr, *tailptr;
	double Zelev;
	ACHAR Zo[20];
	int i;

	headptr = (struct ptt_node *)calloc(1, sizeof(struct ptt_node));
	headptr->next = NULL;
	tailptr = headptr;

	elst = Entlist;
	//  getrbpoint(pt0,elst,10);
	plst = get_resbuf(38, elst);
	Zelev = plst->resval.rreal;

	plst = get_resbuf(10, elst);
	//      ads_alert(L"1");
	if (plst)
	{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		pt0[Z] = Zelev;
	}
	else
		return headptr;
	ads_point_set(pt0, PTS);
	ads_point_set(pt0, mpto);
	/*   p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = pt0[Z];
	if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	{
	tlst = get_resbuf(-1,elst);
	ads_name_set(tname,tlst->resval.rlname);
	ads_entnext(tname,result);
	ads_entdel(result);
	elst = ads_entget(result);
	getrbpoint(pt0,elst,10);
	plst = plst->rbnext;
	plst = get_resbuf(10,plst);
	if(plst)
	{
	pt0[X] = plst->resval.rpoint[X];
	pt0[Y] = plst->resval.rpoint[Y];
	//      pt0[Z] = plst->resval.rpoint[Z];
	}
	}  */
	/*   tlst = get_resbuf(0,elst);
	_tcscpy(Zo,tlst->resval.rstring);*/
	//   ads_point_set(pt0,PTS);
	//   ads_point_set(pt0,mpto);

	p0[Z] = Zelev; p1[Z] = Zelev;

	//   ads_alert(L"ii");
	while (plst != NULL)
	{
		//     getrbpoint(pt1,elst,10);
		plst = plst->rbnext;
		plst = get_resbuf(10, plst);
		if (plst)
		{
			pt1[X] = plst->resval.rpoint[X];
			pt1[Y] = plst->resval.rpoint[Y];
			pt1[Z] = Zelev;

			i = ads_inters(p0, p1, pt0, pt1, 1, mptn);

			if (i == RTNORM)
			{
				mptn[Z] = Zelev;

				if (fabs(mptn[X] - mpto[X]) > 0.001 || fabs(mptn[Y] - mpto[Y]) > 0.001 || fabs(mptn[Z] - mpto[Z]) > 0.001)
				{
					newptr = (struct ptt_node *)calloc(1, sizeof(struct ptt_node));
					ads_point_set(pt0, newptr->pt0);
					ads_point_set(mptn, newptr->pt);
					//		   ads_printf(L"xx=%lf\n",newptr->pt[X]);
					ads_point_set(pt1, newptr->pt1);
					newptr->next = NULL;
					tailptr->next = newptr;
					tailptr = newptr;
				}
			}
			ads_point_set(pt1, pt0);
			ads_point_set(mptn, mpto);

			/*     tlst = get_resbuf(-1,elst);
			ads_name_set(tname,tlst->resval.rlname);
			ads_entnext(tname,result);
			elst = ads_entget(result);
			tlst = get_resbuf(0,elst);
			_tcscpy(Zo,tlst->resval.rstring);*/
		}
	}
	//   ads_alert(L"kk");
	if (headptr->next != NULL)
	{
		newptr = headptr->next;
		ads_point_set(newptr->pt, pt0);
		if (fabs(PTS[X] - pt0[X]) < 0.0001 && fabs(PTS[Y] - pt0[Y]) < 0.0001 && fabs(PTS[Z] - pt0[Z]) < 0.0001)
		{
			headptr->next = newptr->next;
			if (newptr) free(newptr);
		}
		//	 ads_alert(L"jj");
		if (fabs(mptn[X] - pt1[X]) < 0.0001 && fabs(mptn[Y] - pt1[Y]) < 0.0001 && fabs(mptn[Z] - pt1[Z]) < 0.0001)
		{
			//		 ads_alert(L"j");
			newptr = headptr;
			if (newptr == tailptr)
				return headptr;
			while (newptr->next != tailptr)
				newptr = newptr->next;
			newptr->next = NULL;
			if (tailptr) free(tailptr);
		}
	}

	return(headptr);
}

/*------------------------------------------------------------------
;   get the intersection point xy between polyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *BAS_DRAW_FUN::getplinept(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	ads_point pt0, pt1, p00, mptn, mpto, PTS;
	struct resbuf *elst, *tlst, *plst;
	ads_name tname, result;
	struct ptt_node *newptr, *headptr, *tailptr;
	double Zelev;
	ACHAR Zo[20];
	int i;

	headptr = (struct ptt_node *)calloc(1, sizeof(struct ptt_node));
	headptr->next = NULL;
	tailptr = headptr;

	elst = Entlist;
	getrbpoint(pt0, elst, 10);
	/*   plst = get_resbuf(38,elst);
	Zelev = plst->resval.rreal;
	ads_printf(L"k=%d,Ze=%lf\n",plst->restype,Zelev);
	plst = get_resbuf(10,elst);
	ads_alert(L"1");
	if(plst)
	{
	pt0[X] = plst->resval.rpoint[X];
	pt0[Y] = plst->resval.rpoint[Y];
	pt0[Z] = Zelev;
	}
	else
	return headptr; */

	p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = pt0[Z];
	if (fabs(pt0[X] - p00[X]) < 0.001 && fabs(pt0[Y] - p00[Y]) < 0.001 && fabs(pt0[Z] - p00[Z]) < 0.001)
	{
		tlst = get_resbuf(-1, elst);
		//     ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tlst->resval.rlname, result);

		elst = ads_entget(result);
		getrbpoint(pt0, elst, 10);
		Zelev = pt0[Z];
		/*	 plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		//      pt0[Z] = plst->resval.rpoint[Z];
		} */
	}
	tlst = get_resbuf(0, elst);
	_tcscpy(Zo, tlst->resval.rstring);
	ads_point_set(pt0, PTS);
	ads_point_set(pt0, mpto);

	p0[Z] = Zelev; p1[Z] = Zelev;

	//   ads_alert(L"ii");
	while (_tcscmp(Zo, L"SEQEND") != 0)
	{
		getrbpoint(pt1, elst, 10);
		/*	 plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
		pt1[X] = plst->resval.rpoint[X];
		pt1[Y] = plst->resval.rpoint[Y];*/
		pt1[Z] = Zelev;

		i = ads_inters(p0, p1, pt0, pt1, 1, mptn);

		if (i == RTNORM)
		{
			mptn[Z] = Zelev;
			//	     ads_printf(L"i=%d,tn[X]=%lf,tn[Y]=%lf,tn[Z]=%lf\n",i,mptn[X],mptn[Y],mptn[Z]);
			if (fabs(mptn[X] - mpto[X]) > 0.001 || fabs(mptn[Y] - mpto[Y]) > 0.001 || fabs(mptn[Z] - mpto[Z]) > 0.001)
			{
				newptr = (struct ptt_node *)calloc(1, sizeof(struct ptt_node));
				ads_point_set(pt0, newptr->pt0);
				ads_point_set(mptn, newptr->pt);
				//		   ads_printf(L"xx=%lf\n",newptr->pt[X]);
				ads_point_set(pt1, newptr->pt1);
				newptr->next = NULL;
				tailptr->next = newptr;
				tailptr = newptr;
			}
		}
		ads_point_set(pt1, pt0);
		ads_point_set(mptn, mpto);

		tlst = get_resbuf(-1, elst);
		//     ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tlst->resval.rlname, result);
		elst = ads_entget(result);
		tlst = get_resbuf(0, elst);
		_tcscpy(Zo, tlst->resval.rstring);
	}
	//   ads_alert(L"kk");
	if (headptr->next != NULL)
	{
		newptr = headptr->next;
		ads_point_set(newptr->pt, pt0);
		if (fabs(PTS[X] - pt0[X]) < 0.0001 && fabs(PTS[Y] - pt0[Y]) < 0.0001 && fabs(PTS[Z] - pt0[Z]) < 0.0001)
		{
			headptr->next = newptr->next;
			if (newptr) free(newptr);
		}
		//	 ads_alert(L"jj");
		if (fabs(mptn[X] - pt1[X]) < 0.0001 && fabs(mptn[Y] - pt1[Y]) < 0.0001 && fabs(mptn[Z] - pt1[Z]) < 0.0001)
		{
			//		 ads_alert(L"j");
			newptr = headptr;
			if (newptr == tailptr)
				return headptr;
			while (newptr->next != tailptr)
				newptr = newptr->next;
			newptr->next = NULL;
			if (tailptr) free(tailptr);
		}
	}

	return(headptr);
}


/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *BAS_DRAW_FUN::get_Arcpt(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	struct resbuf *elst, *tlst, *plst;
	struct arc_lst *xyr;
	struct ptt_node *headptr, *tailptr, *newptr;
	struct point_lk *xy, *txy;
	int fit_type;
	double Zelev, bulge;
	bool loop;
	ads_name tname, result;
	ads_point p00, pt0, pt1, pt;

	headptr = (struct ptt_node *)calloc(1, sizeof(struct ptt_node));
	headptr->next = NULL;
	tailptr = headptr;
	elst = Entlist;
	//   getrbpoint(pt0,elst,10);
	plst = get_resbuf(38, elst);
	Zelev = plst->resval.rreal;

	plst = get_resbuf(10, elst);

	if (plst)
	{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		pt0[Z] = Zelev;
	}
	tlst = get_resbuf(70, elst);
	fit_type = tlst->resval.rint;

	/*   p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = Zelev;

	if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	{
	tlst = get_resbuf(-1,elst);
	ads_entnext(tlst->resval.rlname,result);
	ads_printf(L"name=%ld,result=%ld\n",tlst->resval.rlname,result);
	ads_alert(L"1");
	plst = ads_entget(result);
	plst = plst->rbnext;
	plst = get_resbuf(10,plst);
	if(plst)
	{
	pt0[X] = plst->resval.rpoint[X];
	pt0[Y] = plst->resval.rpoint[Y];
	//      pt0[Z] = plst->resval.rpoint[Z];
	}
	//	 getrbpoint(pt0,elst,10);

	Zelev = pt0[Z];
	}

	p0[Z] = Zelev; p1[Z] = Zelev;
	//   ads_printf(L"fit_type=%d\n",fit_type);*/

	if (fit_type < 2)
	{
		/*     tlst = get_resbuf(0,elst);
		ads_printf(L"string=%s\n",tlst->resval.rstring);*/
		tlst = get_resbuf(42, tlst);
		/*	 if(!tlst)
		{
		ads_alert(L"q");
		return headptr;
		} */
		bulge = tlst->resval.rreal;
	}

	//   ads_printf(L"bulge=%lf\n",bulge);
	if (fit_type < 2 && fabs(bulge) > 0.00001)
	{
		/*     tlst = get_resbuf(-1,elst);
		ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tname,result);
		elst = ads_entget(result);
		getrbpoint(pt1,elst,10);  */
		plst = plst->rbnext;
		plst = get_resbuf(10, plst);
		if (plst)
		{
			pt1[X] = plst->resval.rpoint[X];
			pt1[Y] = plst->resval.rpoint[Y];
			pt1[Z] = Zelev;
		}

		//	 ads_alert(L"p1");
		xyr = findxyr(pt0, pt1, bulge);
		//     ads_alert(L"p2"); 
		xy = findroot(pt0, pt1, xyr, bulge, p0, p1);

		if (xyr) free(xyr);

		while (xy->next != NULL)
		{
			txy = xy->next;
			xy->next = txy->next;

			pt[X] = txy->pt[X]; pt[Y] = txy->pt[Y];

			if ((fabs(pt0[X] - pt[X]) < 0.001 && fabs(pt0[Y] - pt[Y]) < 0.001 && fabs(pt0[Z] - pt[Z]) < 0.001)
				|| (fabs(pt1[X] - pt[X]) < 0.001 && fabs(pt1[Y] - pt[Y]) < 0.001 && fabs(pt1[Z] - pt[Z]) < 0.001))
				loop = true;
			else
			{
				newptr = (struct ptt_node *)calloc(1, sizeof(struct ptt_node));
				newptr->next = NULL;
				ads_point_set(pt0, newptr->pt0);
				ads_point_set(pt, newptr->pt);
				ads_point_set(pt1, newptr->pt1);

				tailptr->next = newptr;
				tailptr = newptr;
			}

			if (txy) free(txy);
		}
	}

	return(headptr);
}

/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;--
-------------------------------------------------*/
struct ptt_node *BAS_DRAW_FUN::getArcpt(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	struct resbuf *elst, *tlst;
	struct arc_lst *xyr;
	struct ptt_node *headptr, *tailptr, *newptr;
	struct point_lk *xy, *txy;
	int fit_type;
	double Zelev, bulge;
	bool loop;
	ads_name tname, result;
	ads_point p00, pt0, pt1, pt;

	headptr = (struct ptt_node *)calloc(1, sizeof(struct ptt_node));
	headptr->next = NULL;
	tailptr = headptr;
	elst = Entlist;
	getrbpoint(pt0, elst, 10);

	tlst = get_resbuf(70, elst);
	fit_type = tlst->resval.rint;

	p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = Zelev;

	//  if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	//  {
	//    剔除POLYLINE第一层放的点,取出下一点的高程
	tlst = get_resbuf(-1, elst);

	ads_entnext(tlst->resval.rlname, result);
	elst = ads_entget(result);
	getrbpoint(pt0, elst, 10);

	Zelev = pt0[Z];
	//  }

	p0[Z] = Zelev; p1[Z] = Zelev;

	//拟合方式
	if (fit_type < 2)
	{
		tlst = get_resbuf(42, elst);
		//曲率
		bulge = tlst->resval.rreal;
	}

	if (fit_type < 2 && fabs(bulge) > 0.00001)//曲线
	{
		tlst = get_resbuf(-1, elst);
		ads_name_set(tname, tlst->resval.rlname);
		ads_entnext(tname, result);
		elst = ads_entget(result);
		getrbpoint(pt1, elst, 10);

		xyr = findxyr(pt0, pt1, bulge);
		xy = findroot(pt0, pt1, xyr, bulge, p0, p1);

		if (xyr) free(xyr);

		while (xy->next != NULL)
		{
			txy = xy->next;
			xy->next = txy->next;

			pt[X] = txy->pt[X]; pt[Y] = txy->pt[Y];

			if ((fabs(pt0[X] - pt[X]) < 0.001 && fabs(pt0[Y] - pt[Y]) < 0.001 && fabs(pt0[Z] - pt[Z]) < 0.001)
				|| (fabs(pt1[X] - pt[X]) < 0.001 && fabs(pt1[Y] - pt[Y]) < 0.001 && fabs(pt1[Z] - pt[Z]) < 0.001))
				loop = true;
			else
			{
				newptr = (struct ptt_node *)calloc(1, sizeof(struct ptt_node));
				newptr->next = NULL;
				ads_point_set(pt0, newptr->pt0);
				ads_point_set(pt, newptr->pt);
				ads_point_set(pt1, newptr->pt1);

				tailptr->next = newptr;
				tailptr = newptr;
			}

			if (txy) free(txy);
		}
	}

	return(headptr);
}

AcDbObjectId BAS_DRAW_FUN::makepolyline(AcGePoint3dArray PtArray, bool isClosed, int icolor, AcDb::LineWeight lw, ACHAR *LayerName, ACHAR *LineType)
{
	Adesk::Boolean closed = Adesk::kFalse;
	if (isClosed)closed = Adesk::kTrue;
	AcDb3dPolyline *pLine = new AcDb3dPolyline(AcDb::k3dSimplePoly, PtArray, closed);
	if (icolor >= 0)pLine->setColorIndex(icolor);
	pLine->setLineWeight(lw);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	//	AcDbLinetypeTable *pLinetypeTble;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		acutPrintf(L"%s\n", LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pLine->setLayer(LayerTabRecordId);
	pLayerTable->close();
	if (LineType != NULL)
	{
		acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType, L"acadiso.lin");//
		pLine->setLinetype(LineType);
	}
	AcDbObjectId eId = AddEntityToDbs(pLine);
	return eId;
}

CString BAS_DRAW_FUN::GetProByMdbName(CString mdbPath)
{
	CString ProStr;
	int myINT = mdbPath.ReverseFind('\\');
	if (myINT < 0)
		return Pro;
	int myINT2 = mdbPath.GetLength();
	myINT++;
	myINT2 = myINT2 - myINT - 4;
	if (myINT2 < 1)
		return Pro;
	ProStr = mdbPath.Mid(myINT, myINT2);
	return ProStr;
}

CString BAS_DRAW_FUN::GetCurProMdbPath()
{
	if (ReadWorkdir() == 0)
		return L"";

	CString MdbName = Cworkdir + "\\DATA\\" + Pro + ".mdb";

	return MdbName;
}


bool BAS_DRAW_FUN::CalFrame(double d12, double d23, double d34, double d41, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, ads_point &pt1, ads_point &pt2, ads_point &pt3, ads_point &pt4)
{
	AcGePoint3d Pt1, Pt2, Pt3, Pt4;
	if (CalFrame(d12, d23, d34, d41, P1, P2, P3, P4, Pt1, Pt2, Pt3, Pt4))
	{
		pt1[X] = Pt1.x;	pt1[Y] = Pt1.y;	pt1[Z] = Pt1.z;
		pt2[X] = Pt2.x;	pt2[Y] = Pt2.y;	pt2[Z] = Pt2.z;
		pt3[X] = Pt3.x;	pt3[Y] = Pt3.y;	pt3[Z] = Pt3.z;
		pt4[X] = Pt4.x;	pt4[Y] = Pt4.y;	pt4[Z] = Pt4.z;
		return true;
	}
	else return false;
}


bool BAS_DRAW_FUN::CalFrame(double d12, double d23, double d34, double d41, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, AcGePoint3d &Pt1, AcGePoint3d &Pt2, AcGePoint3d &Pt3, AcGePoint3d &Pt4)
{
	AcDbLine *L1 = new AcDbLine(P1, P3);
	AcDbLine *L2 = new AcDbLine(P2, P4);//两对角线
	AcGePoint3dArray O;//两对角线交点
	AcGePoint3d O121, O122, O232, O233, O343, O344, O414, O411;
	//三角形OP1P2缩放后的顶点为O、O121、O122
	//三角形OP2P3缩放后的顶点为O、O232、O233
	O121 = P1; O122 = P2;
	O232 = P2; O233 = P3;
	O343 = P3; O344 = P4;
	O414 = P4; O411 = P1;
	if (L1->intersectWith(L2, AcDb::kOnBothOperands, O) == Acad::eOk)
	{
		ZoomTriangleByPoint_O(d12, O[0], O121, O122);
		ZoomTriangleByPoint_O(d23, O[0], O232, O233);
		ZoomTriangleByPoint_O(d34, O[0], O343, O344);
		ZoomTriangleByPoint_O(d41, O[0], O414, O411);
		AcDbLine *L12 = new AcDbLine(O121, O122);
		AcDbLine *L23 = new AcDbLine(O232, O233);
		AcDbLine *L34 = new AcDbLine(O343, O344);
		AcDbLine *L41 = new AcDbLine(O414, O411);
		L41->intersectWith(L12, AcDb::kExtendBoth, O);
		L12->intersectWith(L23, AcDb::kExtendBoth, O);
		L23->intersectWith(L34, AcDb::kExtendBoth, O);
		L34->intersectWith(L41, AcDb::kExtendBoth, O);
		Pt1.set(O[1].x, O[1].y, 0.0);
		Pt2.set(O[2].x, O[2].y, 0.0);
		Pt3.set(O[3].x, O[3].y, 0.0);
		Pt4.set(O[4].x, O[4].y, 0.0);
		O.removeSubArray(0, O.length() - 1);
		/*
		AddEntityToDbs(L1);
		AddEntityToDbs(L2);
		AddEntityToDbs(L12);
		AddEntityToDbs(L23);
		AddEntityToDbs(L34);
		AddEntityToDbs(L41);
		*/
		delete L12;	L12 = NULL;
		delete L23;	L23 = NULL;
		delete L34;	L34 = NULL;
		delete L41;	L41 = NULL;
		delete L1;	L1 = NULL;
		delete L2;	L2 = NULL;
		return true;
	}
	else
	{
		delete L1;	L1 = NULL;
		delete L2;	L2 = NULL;

		return false;
	}
}

void BAS_DRAW_FUN::ZoomTriangleByPoint_O(double dist, AcGePoint3d O, AcGePoint3d &P1, AcGePoint3d &P2)
{
	double H = 0.0;//三角形的高，即O到P1P2边的距离
	double l1 = 0.0;//O到P1的距离
	double l2 = 0.0;//O到P2的距离
	double jiao1 = 0.0;//边OP1的角度
	double jiao2 = 0.0;//边OP2的角度

	H = distptol(O, P1, P2);
	l1 = O.distanceTo(P1);
	l2 = O.distanceTo(P2);
	jiao1 = angleX(O, P1);
	jiao2 = angleX(O, P2);
	l1 *= (H + dist) / H;//缩放后的边长
	l2 *= (H + dist) / H;//缩放后的边长
	P1.x = O.x + l1 * cos(jiao1);
	P1.y = O.y + l1 * sin(jiao1);
	P2.x = O.x + l2 * cos(jiao2);
	P2.y = O.y + l2 * sin(jiao2);
}

double BAS_DRAW_FUN::distptol(AcGePoint3d O, AcGePoint3d P1, AcGePoint3d P2)
{
	AcDbLine *Line = new AcDbLine(P1, P2);
	double dH;//三角形的高
	dH = distptol(O, Line);
	delete Line;	Line = NULL;
	return dH;
}


void BAS_DRAW_FUN::IniMdfRoadData(CString MdfRoadNmae, ModifyRoadTab& ModifyRoad)
{
	ModifyRoad.RailCkml.Empty();
	ModifyRoad.CutVol = 0.0;
	ModifyRoad.ECkml.Empty();
	ModifyRoad.FillVol = 0.0;
	ModifyRoad.InterAng = 0.0;
	ModifyRoad.KKSY.Empty();
	ModifyRoad.LandArea = 0.0;
	ModifyRoad.LjWide = 0.0;
	ModifyRoad.LmArea = 0.0;
	ModifyRoad.LmThick = 0.0;
	ModifyRoad.LmType.Empty();
	ModifyRoad.LmWide = 0.0;
	ModifyRoad.MasonryVol = 0.0;
	ModifyRoad.Note.Empty();
	ModifyRoad.RandType.Empty();
	ModifyRoad.RoadLen = 0.0;
	ModifyRoad.RoadName = MdfRoadNmae;
	ModifyRoad.SCkml.Empty();
	ModifyRoad.UpDown.Empty();
	ModifyRoad.DetaH = 0.0;
	ModifyRoad.RailName.Empty();
	ModifyRoad.LandAreaStr.Empty();
	ModifyRoad.RoadDml = 0.0;
}

void BAS_DRAW_FUN::CheckIfXLintersect(double array1[4 * MAXJDNUM][10], double JDarray1[MAXJDNUM][6], int NunJD1, double array2[4 * MAXJDNUM][10], double JDarray2[MAXJDNUM][6], int NunJD2, AcGePoint2dArray* JDpt)
{
	AcGePoint3dArray Triangle1pt;
	AcGePoint3dArray Triangle2pt;
	Triangle1pt.setLogicalLength(4);
	Triangle2pt.setLogicalLength(4);
	int Sxynum, Exynum, Sxynum2, Exynum2;
	for (int i = 1; i < NunJD1; i++)
	{
		Sxynum = Exynum = (int)JDarray1[i][4];
		if (fabs(array1[Sxynum - 1][0] - 3) < 0.01)
		{
			Sxynum--;
		}
		if (fabs(array1[Sxynum - 1][0] - 1) < 0.01)
		{
			Sxynum--;
		}
		if (i != NunJD1 - 1)
		{
			if (fabs(array1[Exynum + 1][0] - 4) < 0.01)
			{
				Exynum++;
			}
		}
		else
			Exynum--;
		Triangle1pt[0].set(JDarray1[i][1], JDarray1[i][2], 0.0);
		if (Sxynum == 0)
		{
			Triangle1pt[1].set(array1[Sxynum][1], array1[Sxynum][2], 0.0);
		}
		else
		{
			Triangle1pt[1].set(array1[Sxynum][8], array1[Sxynum][9], 0.0);
		}
		Triangle1pt[2].set(array1[Exynum + 1][8], array1[Exynum + 1][9], 0.0);
		Triangle1pt[3].set(JDarray1[i][1], JDarray1[i][2], 0.0);
		double a = JDarray1[i][1];
		double b = JDarray1[i][2];
		double c = array1[(i - 1) * 4][8];
		double d = array1[(i - 1) * 4][9];
		double f = array1[i * 4][8];
		double e = array1[i * 4][9];
		//make2dline(Triangle1pt,5,999,0.1);
		for (int j = 1; j < NunJD2; j++)
		{
			Sxynum2 = Exynum2 = (int)JDarray2[j][4];
			if (fabs(array2[Sxynum2 - 1][0] - 3) < 0.01)
			{
				Sxynum2--;
			}
			if (fabs(array2[Sxynum2 - 1][0] - 1) < 0.01)
			{
				Sxynum2--;
			}
			if (i != NunJD2 - 1)
			{
				if (fabs(array2[Exynum2 + 1][0] - 4) < 0.01)
				{
					Exynum2++;
				}
			}
			else
				Exynum2--;
			Triangle2pt[0].set(JDarray2[j][1], JDarray2[j][2], 0.0);
			if (Sxynum2 == 0)
			{
				Triangle2pt[1].set(array2[Sxynum2][1], array2[Sxynum2][2], 0.0);
			}
			else
			{
				Triangle2pt[1].set(array2[Sxynum2][8], array2[Sxynum2][9], 0.0);

			}
			Triangle2pt[2].set(array2[Exynum2 + 1][8], array2[Exynum2 + 1][9], 0.0);
			Triangle2pt[3].set(JDarray2[j][1], JDarray2[j][2], 0.0);
			a = JDarray2[j][1];
			b = JDarray2[j][2];
			c = array2[(j - 1) * 4][1];
			d = array2[(j - 1) * 4][2];
			e = array2[j * 4][8];
			f = array2[j * 4][9];
			if (fabs(i - 1.0) < 0.01)
			{

				//make2dline(Triangle2pt,5,999,0.1);

			}
			if (IfTriangleXJ(Triangle1pt, Triangle2pt))
			{
				CalXLjd(array1, Sxynum, Exynum, array2, Sxynum2, Exynum2, JDpt);
			}
		}
	}
}
bool BAS_DRAW_FUN::IfTriangleXJ(AcGePoint3dArray Triangle1pt, AcGePoint3dArray Triangle2pt)
{
	AcGePoint3dArray ptarry;
	AcDb2dPolyline *pLine1 = new AcDb2dPolyline(AcDb::k2dSimplePoly, Triangle1pt, 0.0, Adesk::kFalse);
	AcDb2dPolyline *pLine2 = new AcDb2dPolyline(AcDb::k2dSimplePoly, Triangle2pt, 0.0, Adesk::kFalse);
	pLine1->intersectWith(pLine2, AcDb::kOnBothOperands, ptarry);
	if (ptarry.length() > 0)
		return true;
	else
		return false;
}
void  BAS_DRAW_FUN::CalXLjd(double array1[4 * MAXJDNUM][10], int Sxynum, int Exynum, double array2[4 * MAXJDNUM][10], int Sxynum2, int Exynum2, AcGePoint2dArray* JDpt)
{
	int JDNUM;
	double ResPt[3], ResPt1[3];
	int i, j;
	AcGePoint2d respt;
	AcGePoint3d cyclepoint;
	//计算线元交点

	//if (NunJD1==1)
	//{
	//	i=(NunJD1-1)*4+1;
	//}
	//else
	//{

	//	i=(NunJD1-1)*4;
	//}
	for (i = Sxynum; i <= Exynum + 1; i++)
	{
		//if (NunJD2==1)
		//{
		//	j=(NunJD2-1)*4+1;
		//}
		//else
		//{

		//	j=(NunJD2-1)*4;
		//}

		for (j = Sxynum2; j <= Exynum2 + 1; j++)
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
			JDNUM = CalJD_XY1_XY2(array1[i], array2[j], ResPt, ResPt1);
			if (JDNUM > 0)
			{
				//ads_printf(L"i=%d  j=%d\n",i,j);
				//ads_printf(L"x=%f  y=%f\n",ResPt[0],ResPt[1]);
				//AfxMessageBox(L"1");
				respt.set(ResPt[0], ResPt[1]);
				JDpt->append(respt);
				cyclepoint.set(ResPt[1], ResPt[0], ResPt[2]);
				//makecircle(cyclepoint,3.6,10);
			}
			if (JDNUM == 2)
			{
				respt.set(ResPt1[0], ResPt1[1]);
				JDpt->append(respt);
				cyclepoint.set(ResPt1[1], ResPt1[0], ResPt1[2]);
				//makecircle(cyclepoint,3.6,10);
			}

		}
	}
}
bool BAS_DRAW_FUN::checkIfXJ(double array[10], double array2[10], double step)
{
	AcGePoint3dArray Triangle1pt;
	AcGePoint3dArray Triangle2pt;
	Triangle1pt.setLogicalLength(4);
	Triangle2pt.setLogicalLength(4);
	double lenth = 0;
	double qidian[2];
	qidian[0] = qidian[1] = 0.0;
	double qdfwj = 0.0;
	double  pt[3];
	pt[0] = pt[1] = pt[2] = 0.0;
	double  jiaodian[2];
	jiaodian[0] = jiaodian[1] = 0.0;
	AcGePoint3d  pt3d;
	double zdfwj = 0.0;
	double out1, out2;
	out1 = out2 = 0.0;
	pt3d.x = pt3d.y = pt3d.z = 0;
	//int ptnum;
	//缓和曲线2
	qidian[0] = array[8];
	qidian[1] = array[9];
	qdfwj = array[5];
	if ((array[0] - 2) < 0.001)
	{
		lenth = array[2];
	}
	else
	{
		lenth = array[1] * array[1] / array[2];
	}
	GetXYonSomeXY(array, lenth, pt);
	pt[2] += pi;
	double x2 = qidian[0] + 1000 * cos(qdfwj);
	double y2 = qidian[1] + 1000 * sin(qdfwj);
	double x_2 = pt[0] + 1000 * cos(pt[2]);
	double y_2 = pt[1] + 1000 * sin(pt[2]);
	caljd(qidian[0], qidian[1], x2, y2, pt[0], pt[1], x_2, y_2, &out1, &out2);
	jiaodian[0] = out1;
	jiaodian[1] = out2;
	Triangle1pt[0].set(qidian[0], qidian[1], 0.0);
	Triangle1pt[1].set(jiaodian[0], jiaodian[1], 0.0);
	Triangle1pt[2].set(pt[0], pt[1], 0.0);
	Triangle1pt[3].set(qidian[0], qidian[1], 0.0);
	//make2dline(Triangle1pt,5,999,0.1);
	//缓和曲线2
	qidian[0] = array2[8];
	qidian[1] = array2[9];
	qdfwj = array2[5];
	if ((array2[0] - 2) < 0.001)
	{
		lenth = array2[2];
	}
	else
	{
		lenth = array2[1] * array2[1] / array2[2];
	}
	GetXYonSomeXY(array2, lenth, pt);
	pt[2] += pi;
	x2 = qidian[0] + 1000 * cos(qdfwj);
	y2 = qidian[1] + 1000 * sin(qdfwj);
	x_2 = pt[0] + 1000 * cos(pt[2]);
	y_2 = pt[1] + 1000 * sin(pt[2]);
	caljd(qidian[0], qidian[1], x2, y2, pt[0], pt[1], x_2, y_2, &out1, &out2);
	jiaodian[0] = out1;
	jiaodian[1] = out2;
	Triangle2pt[0].set(qidian[0], qidian[1], 0.0);
	Triangle2pt[1].set(jiaodian[0], jiaodian[1], 0.0);
	Triangle2pt[2].set(pt[0], pt[1], 0.0);
	Triangle2pt[3].set(qidian[0], qidian[1], 0.0);
	//make2dline(Triangle2pt,5,999,0.1);
	if (IfTriangleXJ(Triangle1pt, Triangle2pt))
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

//两条直线方位角分别为Fwj1，Fwj2。计算其夹角
double BAS_DRAW_FUN::CalJiaJiao(double Fwj1, double Fwj2)
{
	double JiaJiao = fabs(Fwj1 - Fwj2);
	if (JiaJiao > pi / 2 && JiaJiao <= pi)
	{
		JiaJiao = pi - JiaJiao;
	}
	else if (JiaJiao > pi&&JiaJiao <= 3 * pi / 2)
	{
		JiaJiao = JiaJiao - pi;
	}
	else if (JiaJiao > 3 * pi / 2 && JiaJiao <= 2 * pi)
	{
		JiaJiao = 2 * pi - JiaJiao;
	}

	return JiaJiao;
}



bool BAS_DRAW_FUN::DrawFromDwgFile(ACHAR *PathName, AcGePoint3d& Pt, ACHAR* Layer, double tuscale, double rotation, bool IsModel)//pt 旋转基点（大地坐标） 
{
	acedSetStatusBarProgressMeterPos(0);
	AcDbObjectId pBlockId = 0;
	AcDbDatabase *pDb = new AcDbDatabase(Adesk::kFalse);
	if (pDb->readDwgFile(PathName, _SH_DENYNO) == Acad::eOk)
	{
		acdbHostApplicationServices()->workingDatabase()->insert(pBlockId, PathName, pDb);
	}
	delete pDb;
	if (pBlockId == NULL)
	{
		ads_printf(L"无法创建块\n");
		return false;
	}

	AcGeVector3d vect(0, 0, 1);
	AcGeMatrix3d tf;
	AcGeScale3d scal(tuscale, tuscale, tuscale);
	/*AcGeVector3d xyvector(-E_OFF,-N_OFF,0);*/
	acedSetStatusBarProgressMeterPos(20);
	AcDbBlockReference *pBkRr = new AcDbBlockReference(Pt, pBlockId);
	pBkRr->setBlockTableRecord(pBlockId);//指定所引用的图块表记录的对象ID
	tf.setToRotation(rotation, vect, Pt);//绕旋转点旋转
	pBkRr->transformBy(tf);
	pBkRr->setScaleFactors(scal);
	//tf.setToTranslation(xyvector);// 左右偏移
	/*pBkRr->transformBy(tf);*/
	pBkRr->setLayer(Layer);
	AddEntityToDbs(pBkRr, IsModel);
	acedSetStatusBarProgressMeterPos(60);
	/*acdbOpenObject(pBkRr, pBkRr->id(), AcDb::kForWrite);*/
	if (acdbOpenObject(pBkRr, pBkRr->id(), AcDb::kForWrite) != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pBkRr->assertWriteEnabled();
	AcDbVoidPtrArray EntSet;
	pBkRr->explode(EntSet);
	for (int i = 0; i < EntSet.length(); i++)
		AddEntityToDbs((AcDbEntity*)(EntSet[i]), IsModel);

	pBkRr->erase();
	pBkRr->close();
	acedSetStatusBarProgressMeterPos(100);
	return true;
}

int BAS_DRAW_FUN::CountLines(ACHAR *filename)
{
	ifstream ReadFile;

	int n = 0;
	string tmp;
	ReadFile.open(filename, ios::in);//ios::in 表示以只读的方式读取文件
	if (ReadFile.fail())//文件打开失败:返回0
	{
		return 0;
	}
	else//文件存在
	{
		while (getline(ReadFile, tmp, '\n'))
		{
			n++;
		}
		ReadFile.close();
		return n - 1;
	}

}

AcDbObjectId BAS_DRAW_FUN::CreateDLPolyLine(AcGePoint3dArray points, int ColorIndex, double SetoffX, double SetoffY, int ClassGrade)
{
	for (int i = 0; i < points.length(); i++)
	{
		points.at(i).x += SetoffX;
		points.at(i).y += SetoffY;
	}
	AcDb3dPolyline *p3dline = new AcDb3dPolyline(AcDb::k3dSimplePoly, points, Adesk::kFalse);
	p3dline->setColorIndex(ColorIndex);
	if (ClassGrade == 1)
		p3dline->setLineWeight(AcDb::kLnWt100);
	else if (ClassGrade == 2)
		p3dline->setLineWeight(AcDb::kLnWt053);
	else if (ClassGrade == 3)
		p3dline->setLineWeight(AcDb::kLnWt025);
	//基本于构建二维多段线方式一致，只是不需要设置“凸度”参数
	return AddEntityToDbs(p3dline);
}

AcDbObjectId BAS_DRAW_FUN::Create3dPolyLine(AcGePoint3dArray points, int ColorIndex,double SetoffX, double SetoffY, int ClassGrade)
{
	for (int i = 0; i < points.length(); i++)
	{
		points.at(i).x += SetoffX;
		points.at(i).y += SetoffY;
	}
	AcDb3dPolyline *p3dline = new AcDb3dPolyline(AcDb::k3dSimplePoly, points, Adesk::kFalse);
	p3dline->setColorIndex(ColorIndex);
	if(ClassGrade==1)
		p3dline->setLineWeight(AcDb::kLnWt100);
	else if(ClassGrade==2)
		p3dline->setLineWeight(AcDb::kLnWt053);
	else if (ClassGrade == 3)
		p3dline->setLineWeight(AcDb::kLnWt025);
	//基本于构建二维多段线方式一致，只是不需要设置“凸度”参数
	return AddEntityToDbs(p3dline);
}

AcGePoint2d BAS_DRAW_FUN::GetOrthPt(AcGePoint2d pt0, AcGePoint2d pt1, AcGePoint2d pt2)
{
	AcGePoint2d pt;
	//第一种情况,与X轴平行
	double dis;
	if (pt1.y > pt2.y)
		dis = pt1.y - pt2.y;
	else
		dis = pt2.y - pt1.y;
	if (dis < 0.01)
	{
		pt.x = pt0.x;
		pt.y = pt1.y;
		return pt;
	}
	//第二种情况,与Y轴平行
	if (pt1.x > pt2.x)
		dis = pt1.x - pt2.x;
	else
		dis = pt2.x - pt1.x;
	if (dis < 0.01)
	{
		pt.x = pt1.x;
		pt.y = pt0.y;
		return pt;
	}
	//第三种情况,与XY轴均不平行
	double k, x, y;
	k = (pt2.y - pt1.y) / (pt2.x - pt1.x);
	x = (k*k*pt1.x + k * (pt0.y - pt1.y) + pt0.x) / (k*k + 1);
	y = k * (x - pt1.x) + pt1.y;
	pt.x = x;
	pt.y = y;
	return pt;
}

AcDbObjectId BAS_DRAW_FUN::makepolylineTD(AcGePoint3dArray& PtAy, int icolor, double lw, ACHAR *LayerName, ACHAR *LineType, double LTScale, double Offset) const
{
	int NUM = PtAy.length();
	AcDbObjectIdArray  IdArray;
	AcDbPolyline *polyline = new AcDbPolyline(NUM);
	AcGePoint2d TempPT;
	bool IsRAILLEGEND = false;
	double TempTD;


	for (int i = 1; i < PtAy.length(); i++)
	{
		if (PtAy[i].z<0.000001 && PtAy[i].z>-0.000001
			&& (PtAy[i].x - PtAy[i - 1].x) < 0.001 && (PtAy[i].x - PtAy[i - 1].x) > -0.001
			&& (PtAy[i].y - PtAy[i - 1].y) < 0.001 && (PtAy[i].y - PtAy[i - 1].y) > -0.001)
		{
			PtAy.removeAt(i);
			i--;
		}
	}
	if (_tcscmp(LineType, L"RAILLEGEND") == 0)
		IsRAILLEGEND = true;

	for (int i = 0; i < NUM; i++)
	{
		TempPT.x = PtAy[i].x;
		TempPT.y = PtAy[i].y;
		TempTD = PtAy[i].z;
		polyline->addVertexAt(i, TempPT, TempTD);
		//polyline->setWidthsAt(i,lw,lw);
	}
	if (icolor >= 0)polyline->setColorIndex(icolor);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForWrite);
	if (LayerName == NULL)LayerName = L"0";
	if (pLayerTable->getAt(LayerName, LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord = new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName, LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId, pLayerTableRecord);
		pLayerTableRecord->close();
	}
	polyline->setLayer(LayerTabRecordId);
	pLayerTable->close();
	AcDbVoidPtrArray offsetCurves;
	if (LineType != NULL)
	{
		if (Acad::eUndefinedLineType == acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType, L"acadiso.lin"))
		{

			ACHAR lineFname[MAX_PATH];
			_stprintf(lineFname, L"%s\\lib\\rail.lin", SysPath);
			acdbHostApplicationServices()->workingDatabase()->
				loadLineTypeFile(LineType, lineFname);//
		}

		if (IsRAILLEGEND)
		{
			polyline->getOffsetCurves(-0.5*lw + Offset, offsetCurves);
			for (int Curve = 0; Curve < offsetCurves.length(); Curve++)
			{
				AcDbPolyline *OffsetPolyline = (AcDbPolyline*)offsetCurves[Curve];
				OffsetPolyline->setLinetype(L"Continuous");
				IdArray.append(AddEntityToDbs(OffsetPolyline));
			}
			if (offsetCurves.length() > 0)
				offsetCurves.removeSubArray(0, offsetCurves.length() - 1);

			polyline->getOffsetCurves(0.5*lw + Offset, offsetCurves);
			for (int Curve = 0; Curve < offsetCurves.length(); Curve++)
			{
				AcDbPolyline *OffsetPolyline = (AcDbPolyline*)offsetCurves[Curve];
				OffsetPolyline->setLinetype(L"Continuous");
				IdArray.append(AddEntityToDbs(OffsetPolyline));
			}
			if (offsetCurves.length() > 0)
				offsetCurves.removeSubArray(0, offsetCurves.length() - 1);
		}

		for (int i = 0; i < NUM; i++)
			polyline->setWidthsAt(i, lw, lw);
		polyline->setLinetype(LineType);
		polyline->setConstantWidth(lw);
		polyline->setLineWeight(TranLineWeightFromIntToEnum(lw * 100));
		polyline->setLinetypeScale(LTScale);
		polyline->setPlinegen(Adesk::kTrue);
	}

	AcDbObjectId  LId;
	if (Offset > -0.001 && Offset < 0.001)//线路中线不需要偏移
	{
		LId = AddEntityToDbs(polyline);
		if (IsRAILLEGEND)
			IdArray.append(LId);
	}
	else
	{
		polyline->getOffsetCurves(Offset, offsetCurves);
		for (int Curve = 0; Curve < offsetCurves.length(); Curve++)
		{
			AcDbPolyline *offsetPolyline = (AcDbPolyline*)offsetCurves[Curve];
			LId = AddEntityToDbs(offsetPolyline);
			if (IsRAILLEGEND)

				IdArray.append(LId);
		}
		if (offsetCurves.length() > 0)
			offsetCurves.removeSubArray(0, offsetCurves.length() - 1);
	}

	if (_tcscmp(LineType, L"RAILLEGEND") == 0)
	{
		ACHAR GroupName[32];
		_stprintf(GroupName, L"线路中线%x", LId);
		LId = CreateGroup(GroupName, IdArray);
		if (IdArray.length() > 0)
			IdArray.removeSubArray(0, IdArray.length() - 1);
	}

	return LId;
}

AcDb::LineWeight BAS_DRAW_FUN::TranLineWeightFromIntToEnum(int LineWeight) const
{
	AcDb::LineWeight LW;
	switch (LineWeight)
	{
	case -1:
		LW = AcDb::kLnWtByLayer;
		break;
	case -2:
		LW = AcDb::kLnWtByBlock;
		break;
	case -3:
		LW = AcDb::kLnWtByLwDefault;
		break;
	case 0:
		LW = AcDb::kLnWt000;
		break;
	case 5:
		LW = AcDb::kLnWt005;
		break;
	case 9:
		LW = AcDb::kLnWt009;
		break;
	case 13:
		LW = AcDb::kLnWt013;
		break;
	case 15:
		LW = AcDb::kLnWt015;
		break;
	case 18:
		LW = AcDb::kLnWt018;
		break;
	case 20:
		LW = AcDb::kLnWt020;
		break;
	case 25:
		LW = AcDb::kLnWt025;
		break;
	case 30:
		LW = AcDb::kLnWt030;
		break;
	case 35:
		LW = AcDb::kLnWt035;
		break;
	case 40:
		LW = AcDb::kLnWt040;
		break;
	case 50:
		LW = AcDb::kLnWt050;
		break;
	case 53:
		LW = AcDb::kLnWt053;
		break;
	case 60:
		LW = AcDb::kLnWt060;
		break;
	case 70:
		LW = AcDb::kLnWt070;
		break;
	case 80:
		LW = AcDb::kLnWt080;
		break;
	case 90:
		LW = AcDb::kLnWt090;
		break;
	case 100:
		LW = AcDb::kLnWt100;
		break;
	case 106:
		LW = AcDb::kLnWt106;
		break;
	case 120:
		LW = AcDb::kLnWt120;
		break;
	case 140:
		LW = AcDb::kLnWt140;
		break;
	case 158:
		LW = AcDb::kLnWt158;
		break;
	case 200:
		LW = AcDb::kLnWt200;
		break;
	case 211:
		LW = AcDb::kLnWt211;
		break;
	default:
		LW = AcDb::kLnWtByLayer;
		break;
	}
	return LW;
}

AcDbObjectId BAS_DRAW_FUN::CreateGroup(ACHAR *GroupName, AcDbObjectIdArray IdAr) const
{
	AcDbGroup       *pGroup = new AcDbGroup;
	AcDbObjectId     groupObjectId;
	AcDbDictionary  *pGroupDict = NULL;

	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	pGroupDict->setAt(GroupName, pGroup, groupObjectId);
	pGroupDict->close();
	int len = IdAr.length();
	for (int i = 0; i < len; i++)pGroup->append(IdAr[i]);   // entities which will be in the group.
	pGroup->close();

	return groupObjectId;
}

//20190713	新增ZOOM函数
void BAS_DRAW_FUN::Zoom(double Left, double Top, double Right, double Bottom)
{
	CString Point1, Point2;
	Point1.Format(L"%.3lf,%.3lf", Left, Top);
	Point2.Format(L"%.3lf,%.3lf", Right, Bottom);

	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"ZOOM\n", 0);
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"W\n", 0);
	acDocManager->sendStringToExecute(acDocManager->curDocument(), Point1, 0);
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"\n", 0);
	acDocManager->sendStringToExecute(acDocManager->curDocument(), Point2, 0);
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"\n", 0);
}
//由方位角求解向量
AcGeVector3d BAS_DRAW_FUN::calfwjvec(double fwj)
{
	//将方位角控制在0-z*pi
	if (fwj < -0.000001)
		fwj += 2 * pi;
	else if (fwj > 2 * pi - 0.00001)
		fwj -= 2 * pi;

	AcGeVector3d vec(1, 0, 0);
	if (fabs(fwj) < 0.0001 || fabs(fwj - 2 * pi) < 0.0001)
		vec.set(0, 1, 0);
	else if (fabs(fwj - pi / 2.0) < 0.0001)
		vec.set(1, 0, 0);
	else if (fabs(fwj - pi) < 0.0001)
		vec.set(0, -1, 0);
	else if (fabs(fwj - 3.0*pi / 2.0) < 0.0001)
		vec.set(-1, 0, 0);
	else if (fwj > 0 && fwj < pi / 2.0)
		vec.set(sin(fwj), cos(fwj), 0);
	else if (fwj<pi && fwj>pi / 2.0)
		vec.set(cos(fwj - pi / 2.0), -1.0*sin(fwj - pi / 2.0), 0);
	else if (fwj > pi && fwj < 3.0*pi / 2.0)
		vec.set(-1.0*sin(fwj - pi), -1.0*cos(fwj - pi), 0);
	else if (fwj<2 * pi && fwj>3.0*pi / 2.0)
		vec.set(-1.0*sin(2 * pi - fwj), cos(2 * pi - fwj), 0);

	vec.normalize();
	return vec;
}
//设置捕捉模式
void BAS_DRAW_FUN::Set_OSMODE(short Mode_Type)
{
	struct resbuf resOSMODE;
	resOSMODE.restype = RTSHORT;
	acedGetVar(L"OSMODE", &resOSMODE);
	short saveOsnapValue = resOSMODE.resval.rint;
	resOSMODE.resval.rint = Mode_Type;
	acedSetVar(L"OSMODE", &resOSMODE);
}

/*===================================================================
函数名:DeleteEntityRectangle
------------------------------------------------------------
函数功能:删除指定范围内的实体（矩形）
------------------------------------------------------------
输入:	ads_point PtLeftButtom		矩形左下角点
		ads_point PtRightUp			矩形右上角点
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190814
------------------------------------------------------------
备注：
=====================================================================*/
void BAS_DRAW_FUN::DeleteEntityRectangle(ads_point PtLeftButtom, ads_point PtRightUp)
{
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, PtLeftButtom, RTPOINT, PtRightUp, RTSTR, L"", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"erase", RTSTR, L"BOX", RTPOINT, PtLeftButtom, RTPOINT, PtRightUp, RTSTR, L"", RTNONE);
#endif // VERSION_CAD2010	
}

/*===================================================================
函数功能: //由于修正里程与高程操作的需要,重新计算坡度\坡长,
-----------------------------------------------------
输入:	ml level Rshu
-----------------------------------------------------
输出:   length degree Rlen
-----------------------------------------------------
备注:	无
=====================================================================*/
void BAS_DRAW_FUN::NewDegree(vector<PDTAB> &Slope)
{
	int slopeNum = Slope.size();
	if (slopeNum == 0)
		return;

	for (int k = 0; k < slopeNum - 1; k++)
	{
		Slope[k].length = (Slope[k + 1].ml - Slope[k].ml);
		Slope[k].degree = (Slope[k + 1].Level - Slope[k].Level) / Slope[k].length;
	}
	Slope[slopeNum - 1].degree = 0.0;
	Slope[slopeNum - 1].length = 0.0;
}

double BAS_DRAW_FUN::MLToH_Bas(double cml, int ncm, double cm[], double& degree)
{
	//初始化求解的值
	degree = 0.0;
	double H = 0.0;

	//检测特殊情况
	if (ncm == 0 || ncm == 1)		//变坡点小于等于1，返回默认值
		return H;
	else if (ncm == 2)	//只有一个变坡点
	{
		H = cm[1];
		degree = cm[2];
		return H;
	}
	else if (cml < cm[0] - 0.0001)	//如果给定的里程小于起点里程，那么无法计算
		return H;

	//第一段直线坡段
	double R1 = cm[1 * 5 + 4];
	double part_cml = R1 * tan(fabs(atan(cm[1 * 5 + 2]) - atan(cm[2 * 5 + 2])) / 2) * (cos(atan(cm[1 * 5 + 2])));	//第一个圆曲线起点里程
	if (cml < (cm[1 * 5 + 0] - part_cml))
	{
		H = cm[1 * 5 + 1] - cm[1 * 5 + 2] * (cm[1 * 5 + 0] - cml);
		degree = cm[1 * 5 + 2];
		return H;
	}

	for (int i = 1; i + 2 < ncm; i++)
	{
		//计算切长T=RW/2
		double R, R2;
		R = cm[i * 5 + 4];	//竖曲线半径
		R2 = cm[(i + 1) * 5 + 4];	//下一个变坡点的竖曲线半径
		double PreAlpha = atan(cm[i * 5 + 2]);
		double AftAlpha = atan(cm[(i + 1) * 5 + 2]);
		double Tsh = R * tan((fabs(PreAlpha - AftAlpha)) / 2);
		//计算圆弧起点坐标
		double distX_Begin = cm[i * 5 + 0] - Tsh * cos(PreAlpha);
		//计算圆弧终点坐标
		double distX_End = cm[i * 5 + 0] + Tsh * cos(AftAlpha);
		//夹直线范围判断
		double part_cml_1 = 0.0;
		if (i + 3 >= ncm)	//倒数第二个变坡点
			part_cml_1 = cm[(i + 1) * 5 + 0];	//最后一个变坡点的里程
		else
			part_cml_1 = cm[(i + 1) * 5 + 0] - R2 * tan(fabs(atan(cm[(i + 1) * 5 + 2]) - atan(cm[(i + 2) * 5 + 2])) / 2) * (cos(atan(cm[(i + 1) * 5 + 2])));


		if (cml > distX_Begin + 0.0001 && cml < distX_End - 0.0001) //20190609 修改范围
		{
			H = VerticleArc(i, cml, cm);

			//判断属于哪个坡段以计算竖曲线纵距和坡度
			if (cml < cm[i * 5 + 0])		//在当前变坡点的小里程一侧
			{
				degree = cm[i * 5 + 2];
			}
			else		//在当前变坡点的大里程一侧
			{
				degree = cm[(i + 1) * 5 + 2];
			}

			return H;
		}
		else if (cml < part_cml_1)
		{
			//直线坡段范围
			H = cm[i * 5 + 1] + cm[(i + 1) * 5 + 2] * (cml - cm[i * 5 + 0]);
			degree = cm[(i + 1) * 5 + 2];
			return H;
		}
	}

	//超出范围，无法计算
	return H;
}

double BAS_DRAW_FUN::VerticleArc(int iSlope, double cml, double cm[])
{
	//先进行赋值操作
	double Cur_Cml = cm[iSlope * 5 + 0];
	double Cur_Level = cm[iSlope * 5 + 1];
	double Cur_Degree = cm[(iSlope + 1) * 5 + 2];
	double Cur_R = cm[iSlope * 5 + 4];

	double Pre_Cml = cm[(iSlope - 1) * 5 + 0];
	double Pre_Level = cm[(iSlope - 1) * 5 + 1];
	double Pre_Degree = cm[iSlope * 5 + 2];
	double Pre_R = cm[(iSlope - 1) * 5 + 4];

	if (iSlope == 0)
		return 0.0;
	else if (Cur_R < 0.00000001)
		return (Pre_Level + (cml - Pre_Cml) * Pre_Degree);
	else if (Cur_R < 100)//抛物线
	{
		//1.计算坡差W
		double dGradeFall = Cur_Degree - Pre_Degree;
		//2.计算切长T=RW/2
		double R;
		R = 20000.0 / Cur_R;
		double Tsh = R * fabs(dGradeFall) / 2.0;
		//3.计算竖曲线起点里程OML=dsml[i]-T
		double distX = Cur_Cml - Tsh;
		double distY = Cur_Level - Pre_Degree * Tsh;
		//4.循环算点、画线 
		double n, e;
		e = distX;
		n = distY;
		double l = 0, h = 0;
		int flag = dGradeFall < 0.0 ? -1 : 1;
		l = cml - distX;
		h = l * l / 2.0 / R;
		e = distX + l;
		n = (Pre_Level + (e - Pre_Cml) * Pre_Degree + flag * h);
		return n;
	}
	else//圆曲线 20190609
	{
		//----------------计算绘圆弧的起终点---------------------------------
		//1.计算坡差W,判断左右偏
		int i = iSlope;
		double dGradeFall = Cur_Degree - Pre_Degree;		//大于0，凹型，小于0，凸型
		int npx = (dGradeFall > 0) ? 1 : -1;
		//2.计算切长T=RW/2
		double R;
		R = Cur_R;
		//20190610	采用更精确的方式计算T
		double PreAlpha = atan(Pre_Degree);
		double AftAlpha = atan(Cur_Degree);
		double Tsh = R * tan((fabs(PreAlpha - AftAlpha)) / 2);

		//计算圆弧起点坐标
		double distX_Begin = Cur_Cml - Tsh * cos(PreAlpha);
		double distY_Begin = Cur_Level - Tsh * sin(PreAlpha);
		//计算圆心坐标
		double StartPtFwj;
		double dd = xyddaa(distY_Begin, distX_Begin, Cur_Level, Cur_Cml, &StartPtFwj);
		double Q = (dGradeFall > 0) ? (StartPtFwj - pi / 2.0) : (StartPtFwj + pi / 2.0);
		double yx_X = distX_Begin + R * sin(Q);		//圆心e
		double yx_Y = distY_Begin + R * cos(Q);		//圆心n

		//根据圆公式计算出直线与圆的两个交点
		double Y_1 = yx_Y + sqrt(R * R - pow(cml - yx_X, 2));
		double Y_2 = yx_Y - sqrt(R * R - pow(cml - yx_X, 2));

		if (dGradeFall > 0)
			return Y_2;
		else
			return Y_1;
	}

	return 0.0;
}
AcGePoint3d  BAS_DRAW_FUN::resetjd_centerid(AcGePoint3d Point, AcDbObject* pEntity, AcDbObjectId& lineId)
{
	AcGePoint3d startPt;
	lineId = NULL;
	JD_CENTER* pJD_CENTER;
	pJD_CENTER = JD_CENTER::cast(pEntity);
	double EndProMiles = 0.00;
	EndProMiles = pJD_CENTER->PROJ_ML(Point[Y], Point[X]);
	xlpoint xlpointsq;
	xlpointsq.lc = EndProMiles;
	pJD_CENTER->xlpoint_pz(&xlpointsq);
	double R = xlpointsq.r;

	int ItemIndex = pJD_CENTER->GetXYNoFromTYLC(xlpointsq.lc); // 线元号
	double dFwj = pJD_CENTER->XYArray[ItemIndex][5];
	double ItemType = pJD_CENTER->XYArray[ItemIndex][0]; // 线元的类型
	double LeftOrRight = pJD_CENTER->XYArray[ItemIndex][4]; // 线元的偏向	
	if (fabs(ItemType - 1.0) < DataPrecision)
	{
		double Len = pJD_CENTER->XYArray[ItemIndex][1];
		startPt.x = pJD_CENTER->XYArray[ItemIndex][9];
		startPt.y = pJD_CENTER->XYArray[ItemIndex][8];
		startPt.z = 0.00;
		makeline(startPt, dFwj, Len, lineId, 1);
	}
	else if (ItemType > 1.0)
	{
		double Len = pJD_CENTER->XYArray[ItemIndex][2];
		startPt.x = pJD_CENTER->XYArray[ItemIndex][9];
		startPt.y = pJD_CENTER->XYArray[ItemIndex][8];
		startPt.z = 0.00;
		makearc(startPt, dFwj, pJD_CENTER->XYArray[ItemIndex][1], Len, LeftOrRight, lineId, 0);
	}
	return startPt;

}