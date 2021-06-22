// BAS_DRAW_FUN1.cpp: implementation of the BAS_DRAW_FUN_JYX class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "aced.h"
#include "dbapserv.h"
#include "BAS_DRAW_FUN_JYX.h"
#include "migrtion.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016


//#include "XLDataBase_JYX.h"
#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS" 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//GuiFanCanShu mJYGuiFanCS;

//extern QuXianZheJiang m_ZJ;

extern CString Cworkdir,Pro;
extern ACHAR SysPath[1024];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BAS_DRAW_FUN_JYX::BAS_DRAW_FUN_JYX()
{
	m_dHalfPI = 0.5 * pi;
}

BAS_DRAW_FUN_JYX::~BAS_DRAW_FUN_JYX()
{

}

void BAS_DRAW_FUN_JYX::GetJdnoFromJdstr(CString JDstr, Adesk::Int16 &Jdno)
{
	//	int sno,eno;//交点串中尾部数字部分的起始位与终止位。
	int i,n;//交点串中尾部数字部分中数字的个数。
	n=JDstr.GetLength();
	CString JDNOSTR,temp;
	for(i=0;i<n;i++)
	{
		temp=JDstr.GetAt(i);
		if(temp=='-')
		{
			Jdno=-1;
			return;
		}
	}
	for(i=n-1;i>=0;i--)//从尾到头遍历找数字
	{
		temp=JDstr.GetAt(i);
		if(temp<=_T("9") && temp>=_T("0"))
		{
			JDNOSTR+=temp;
		}
		else break;
	}
	JDNOSTR.MakeReverse();
	Jdno=int(_wtof(JDNOSTR)+0.1);
}

//获取对应I线交点序号
bool BAS_DRAW_FUN_JYX::GetJdno1FromJdstr(CString JDstr, CArray<JDarrayStruct,JDarrayStruct>& JDarray,Adesk::Int16 &Jdno,int CountAllNum)
{
	int i,njd;
	CString str;
	njd = JDarray.GetSize();

	int CurCountNum = 0;
	for(i=0;i<njd;i++)
	{ 
		str = JDarray[i].JDNum;
		if(str == JDstr)
		{
			CurCountNum ++;
			if(CountAllNum==CurCountNum)
			{
				Jdno = i;
				return true;
			}
		}

	}
	return false;

}

// 向AUTOCAD 数据库中添加实体对象的通用函数
AcDbObjectId BAS_DRAW_FUN_JYX::AddEntityToDbs(AcDbEntity *pEntity)
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


//AcDbObjectId   BAS_DRAW_FUN::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType)
//{
//	AcDbLine *pLine = new AcDbLine(startPt, endPt);
//	if(icolor>=0)pLine->setColorIndex(icolor);
//	pLine->setLineWeight(lw);
//
//	AcDbLayerTableRecord *pLayerTableRecord;
//	AcDbObjectId LayerTabRecordId;
//	AcDbLayerTable *pLayerTable;
//	AcDbObjectId ltypeObjId;
//	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
//	if(LayerName==NULL)LayerName="0";
//	if(pLayerTable->getAt(LayerName,LayerTabRecordId) != Acad::eOk)
//	{
//		pLayerTableRecord=new AcDbLayerTableRecord;
//		pLayerTableRecord->setName(LayerName);
//		//		acutPrintf(L"%s\n",LayerName);
//		pLayerTable->getAt(LayerName,LayerTabRecordId);
//		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
//		pLayerTableRecord->close();
//	}
//	pLine->setLayer(LayerTabRecordId);	
//	pLayerTable->close();
//	if(LineType!=NULL)
//	{
//		acdbHostApplicationServices()->workingDatabase()->
//			loadLineTypeFile(LineType,L"acadiso.lin");//
//		pLine->setLinetype(LineType);
//	}
//	AcDbObjectId  LId;
//	LId = AddEntityToDbs(pLine) ;
//	return LId;     
//}


// 由两点绘直线
AcDbObjectId   BAS_DRAW_FUN_JYX::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt)
{   
	AcDbLine *pLine	= new AcDbLine(startPt,endPt);
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	return LId ;
}

//
// 向AUTOCAD 数据库中添加实体对象的通用函数
AcDbObjectId BAS_DRAW_FUN_JYX::AddEntityToDbs(AcDbEntity *pEntity,int mode,bool ispapespace)
{ 	

	//	Acad::ErrorStatus     es = Acad::eOk;
	AcDbObjectId objId ;
	AcDbBlockTable *pBlockTable;
	// CAD R2000
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable,AcDb::kForRead);
	// CAD R14
	// acdbCurDwg()->getBlockTable(pBlockTable,AcDb::kForRead);
	AcDbBlockTableRecord  *pSpaceRecord;
	if(ispapespace)pBlockTable->getAt(ACDB_PAPER_SPACE, pSpaceRecord,AcDb::kForWrite);
	else pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord,AcDb::kForWrite);
	pSpaceRecord->appendAcDbEntity(objId, pEntity);
	pSpaceRecord->close();

	pBlockTable->close();
	if(mode==0)pEntity->close();
	return objId;

}



AcDbObjectId   BAS_DRAW_FUN_JYX::makeline2(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType)
{
	AcDbLine *pLine = new AcDbLine(startPt, endPt);
	if(icolor>=0)pLine->setColorIndex(icolor);
	pLine->setLineWeight(lw);

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
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pLine->setLayer(LayerTabRecordId);	
	pLayerTable->close();
	if(LineType!=NULL)
	{
		acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType,L"acadiso.lin");//
		pLine->setLinetype(LineType);
	}
	AcDbObjectId  LId;
	LId = AddEntityToDbs(pLine) ;
	return LId;     
}


AcDbObjectId   BAS_DRAW_FUN_JYX::makeline2(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType)
{
	AcGePoint2dArray PtAy;
	PtAy.append(AcGePoint2d(startPt.x,startPt.y));
	PtAy.append(AcGePoint2d(endPt.x,endPt.y));
	return makepolyline(PtAy,icolor,lw,LayerName,LineType);	    
}

/*
AcDbObjectId   BAS_DRAW_FUN::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor)
{
AcDbLine *pLine = new AcDbLine(startPt, endPt) ;
pLine->setColorIndex(icolor) ;
AcDbObjectId  LId ;
LId=AddEntityToDbs(pLine) ;
return LId;     
}*/


void BAS_DRAW_FUN_JYX::getMinMaxXY(ads_point p1,ads_point p2,ads_point p3,ads_point p4,double &xmin,double &xmax,double &ymin,double &ymax)
{
	xmin=p1[X];
	xmax=p1[X];
	if(xmin>p2[X])xmin=p2[X];
	if(xmin>p3[X])xmin=p3[X];
	if(xmin>p4[X])xmin=p4[X];

	if(xmax<p2[X])xmax=p2[X];
	if(xmax<p3[X])xmax=p3[X];
	if(xmax<p4[X])xmax=p4[X];

	ymin=p1[Y];
	ymax=p1[Y];
	if(ymin>p2[Y])ymin=p2[Y];
	if(ymin>p3[Y])ymin=p3[Y];
	if(ymin>p4[Y])ymin=p4[Y];

	if(ymax<p2[Y])ymax=p2[Y];
	if(ymax<p3[Y])ymax=p3[Y];
	if(ymax<p4[Y])ymax=p4[Y];

}


void BAS_DRAW_FUN_JYX::mZOOM(double x1,double y1,double x2,double y2)
{
	ads_point P1,P2;
	P1[X]=x1;P1[Y]=y1;
	P2[X]=x2;P2[Y]=y2;
	double xmin,xmax,ymin,ymax;
	getMinMaxXY(P1,P2,P1,P2,xmin,xmax,ymin,ymax);
	xmin-=50;
	ymin-=50;
	xmax+=50;
	ymax+=50;
	P1[X]=xmin;P1[Y]=ymin;
	P2[X]=xmax;P2[Y]=ymax;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, P1, RTPOINT, P2, 0);
#else
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, P1, RTPOINT, P2, 0);
#endif // VERSION_CAD2010


	
}


void BAS_DRAW_FUN_JYX::dTodms(double dd, int Nw, ACHAR *dms)
	//度.分秒　　－＞度%%d，分\'，秒\"
{
	double degree=dd;
	ACHAR d[20],m[20],s[20];
	int IntDeg,IntMin,IntSec;
	IntDeg=(int)(degree);
	IntMin=(degree-int(degree))*100.0;//minute
	//	IntSec=(int)(0.5+(degree*100-int(degree*100))*100.0);//second四舍五入
	//if(IntSec>=60)
	//{
	//	IntSec-=60;
	//	IntMin++;
	//}
	//if(IntMin>=60)
	//{
	//	IntMin-=60;
	//	IntDeg++;
	//}

	double DoubleSec = ((fabs(degree)-IntDeg)*100.0-IntMin)*100.0;
	if(DoubleSec>=60.0)
	{
		DoubleSec-=60.0;
		IntMin++;
	}
	if(IntMin>=60)
	{
		IntMin-=60;
		IntDeg++;
	}

	acdbRToS(IntDeg,2,0,d);
	acdbRToS(IntMin+100,2,0,m);
	acdbRToS(DoubleSec+100,2,Nw,s);
	_tcscpy(dms,d);
	ACHAR ch[20];
	ch[0]='%';
	ch[1]='%';
	ch[2]='d';
	ch[3]=m[1];
	ch[4]=m[2];
	ch[5]='\0';
	_tcscat(dms,ch);
	//ch[0]='\'';
	//ch[1]=s[1];
	//ch[2]=s[2];
	//ch[3]='\0';
	//_tcscat(dms,ch);
	s[0] = '\'';
	_tcscat(dms,s);
	_tcscat(dms,L"\"");
}

AcDbObjectId BAS_DRAW_FUN_JYX::CreateGroup(ACHAR *GroupName, AcDbObjectIdArray IdAr)
{
	AcDbGroup       *pGroup = new AcDbGroup;
	AcDbObjectId     groupObjectId;
	AcDbDictionary  *pGroupDict = NULL;

	acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
	pGroupDict->setAt(GroupName, pGroup, groupObjectId);
	pGroupDict->close();	
	int len=IdAr.length();
	for(int i=0;i<len;i++)pGroup->append(IdAr[i]);   // entities which will be in the group.
	pGroup->close();
	return groupObjectId;	
}

void BAS_DRAW_FUN_JYX::AdjustAngle(double &j)
{
	if(j<0.0)j+=2.0*PI;
	if(j>2.0*PI)j-=2.0*PI;
}


//标注线间距：1 线上一点，2 线上一点，标注线长度=20mm*SCALE，。。。。
void BAS_DRAW_FUN_JYX::makeXJJFlag(AcGePoint3d Pt1,AcGePoint3d Pt2,double len,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType)
{

	double jiao=fwj(Pt1,Pt2);
	AcGePoint3d Pt3,Pt4,Pt5,Pt6;

	getLineEndPt(Pt1,len,jiao+pi,Pt3);
	getLineEndPt(Pt1,0.3*len,jiao-0.1*pi+pi,Pt5);

	getLineEndPt(Pt2,len,jiao,Pt4);
	getLineEndPt(Pt2,0.3*len,jiao-0.1*pi,Pt6);

	AcGePoint2dArray Pt2dAy;
	Pt2dAy.append(AcGePoint2d(Pt1.x,Pt1.y));
	Pt2dAy.append(AcGePoint2d(Pt3.x,Pt3.y));
	makepolyline(Pt2dAy,-1,lw,LayerName);

	Pt2dAy.removeSubArray(0,1);
	Pt2dAy.append(AcGePoint2d(Pt1.x,Pt1.y));
	Pt2dAy.append(AcGePoint2d(Pt5.x,Pt5.y));
	makepolyline(Pt2dAy,-1,lw,LayerName);

	Pt2dAy.removeSubArray(0,1);
	Pt2dAy.append(AcGePoint2d(Pt2.x,Pt2.y));
	Pt2dAy.append(AcGePoint2d(Pt4.x,Pt4.y));
	makepolyline(Pt2dAy,-1,lw,LayerName);

	Pt2dAy.removeSubArray(0,1);
	Pt2dAy.append(AcGePoint2d(Pt2.x,Pt2.y));
	Pt2dAy.append(AcGePoint2d(Pt6.x,Pt6.y));
	makepolyline(Pt2dAy,-1,lw,LayerName);

	//makeline2(Pt1,Pt3,icolor,lw,LayerName,LineType);
	//makeline2(Pt1,Pt5,icolor,lw,LayerName,LineType);
	//makeline2(Pt2,Pt4,icolor,lw,LayerName,LineType);
	//makeline2(Pt2,Pt6,icolor,lw,LayerName,LineType);
}


// 绘文本
AcDbObjectId BAS_DRAW_FUN_JYX::maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR * textStyle,ACHAR *LayerName)
{
	AcDbObjectId  TId;
	AcDbText *pText = new AcDbText;
	AcDbObjectId pTextStyleTblId;
	if (mode==1) // 左对齐
	{
		pText->setHorizontalMode(AcDb::kTextLeft);
	}
	else if (mode==2) // // 中对齐
	{
		pText->setHorizontalMode(AcDb::kTextCenter);
	}
	else if (mode==3) // // 中对齐
	{
		pText->setHorizontalMode(AcDb::kTextMid);
	}		
	else // // 右对齐
	{
		pText->setHorizontalMode(AcDb::kTextRight);
	};
	pText->setColorIndex(icolor);
	if (mode==1)
		pText->setPosition(pos);
	else 
		pText->setAlignmentPoint(pos);
	pTextStyleTblId=LoadTextStyle();
	pText->setTextStyle(pTextStyleTblId);
	pText->setRotation(ang);
	pText->setWidthFactor(0.7);
	pText->setHeight(texth);
	pText->setTextString(ctext);
	//layer///////////////////////////////////////////////////////////////////
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
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pText->setLayer(LayerTabRecordId);	
	pLayerTable->close();
	//////////////////////////////////////////////////////////////////////////
	TId=AddEntityToDbs(pText);
	return TId;  
}


AcDbObjectId BAS_DRAW_FUN_JYX::LoadTextStyle()
{
	//	AcDbTextStyleTable *pTextStyleTbl;
	//	AcDbObjectId pTextStyleTblId;
	//	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite);
	//	if(pTextStyleTbl->getAt(L"PMBZ",pTextStyleTblId)!= Acad::eOk)
	//	{
	//		AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
	//		
	//		//////////////4:
	//		pTextStyleTblRcd->setName(L"PMBZ");
	//		
	//		ACHAR SysPath[256];
	//		ACHAR TextStyleFile[256];
	//		GetPrivateProfileString("RLDVS",L"RLDVS",NULL,SysPath,256,L"win.ini");			
	//		_stprintf(TextStyleFile,L"%s\\lib\\Rs9.shx",SysPath);
	//		
	//		pTextStyleTblRcd->setFileName(TextStyleFile);
	//		_stprintf(TextStyleFile,L"%s\\lib\\HZTXT.shx",SysPath);
	//		pTextStyleTblRcd->setBigFontFileName(TextStyleFile);
	//		
	//		pTextStyleTblRcd->setFont(L"rs9.shx",0,0,134,2);
	//		pTextStyleTblRcd->setXScale(0.7);
	//
	//		pTextStyleTbl->add(pTextStyleTblRcd);
	//		pTextStyleTblRcd->close();
	//		pTextStyleTbl->getAt(L"PMBZ",pTextStyleTblId);
	//	}	
	//	pTextStyleTbl->close();	
	//  return pTextStyleTblId;     	
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite);
	if(pTextStyleTbl->getAt(L"PMBZ",pTextStyleTblId)!= Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
		pTextStyleTblRcd->setName(L"PMBZ");			
		pTextStyleTblRcd->setFont(L"宋体",0,0,134,2);
		pTextStyleTblRcd->setXScale(0.7);

		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		pTextStyleTbl->getAt(L"PMBZ",pTextStyleTblId);
	}	
	pTextStyleTbl->close();	
	return pTextStyleTblId; 
}



// 绘圆弧
AcDbObjectId   BAS_DRAW_FUN_JYX::makearc(AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx)
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
int BAS_DRAW_FUN_JYX::getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT)
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
int BAS_DRAW_FUN_JYX::getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT,AcGePoint3d& pT1,AcGePoint3d& pT2)
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
#else
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


int  BAS_DRAW_FUN_JYX::getPoint(ACHAR* pMessage, AcGePoint3d& pT)
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


void  BAS_DRAW_FUN_JYX::makeGroup(AcDbObjectId groupId)
{
	ads_name sset;
	int err = ads_ssget(NULL, NULL, NULL, NULL, sset);
	if (err != RTNORM) {
		return;
	}

	AcDbGroup *pGroup;
	if(acdbOpenObject(pGroup, groupId, AcDb::kForWrite)==Acad::eOk)
	{

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
}

void BAS_DRAW_FUN_JYX::removeAllButLines(AcDbObjectId groupId)
{
	AcDbGroup *pGroup;
	if(acdbOpenObject(pGroup, groupId, AcDb::kForWrite)==Acad::eOk)
	{
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
			} else
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
}

void BAS_DRAW_FUN_JYX::groups()
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


AcDbEntity*  BAS_DRAW_FUN_JYX::selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
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
	//	ads_printf(L"\nAcDbEntity name=%s",entObj->isA()->name());
	return entObj;
}


// jj--表示线元性质   
//计算线元长度
double  BAS_DRAW_FUN_JYX::Get_INDATA_L(double INDATA[5],int *jj)
{     
	int j;
	double l=0.0;
	j=(int)(INDATA[0]+0.01);
	if (j==1) l=INDATA[1];  //线元长度
	if (j==2) l=INDATA[2];  //圆线元的长度
	if (j==3) 
	{
		if(fabs(INDATA[2])>0.00001)
			l=INDATA[1]*INDATA[1]/INDATA[2];
		else
			l=0.0;
	}

	if (j==4)
	{
		if(fabs(INDATA[2])>0.00001)
			l=INDATA[1]*INDATA[1]/INDATA[2];
		else
			l=0.0;
	}


	//   R1----> R2  (BIG---->SMALL)    of spiral
	//   A=INDATA[1];R1=INDATA[2];R2=INDATA[3];L1=A*A/R1;L2=A*A/R2;L=L2-L1;
	if (j==5) 
	{
		if(fabs(INDATA[2]*INDATA[3])>0.00001)
			l=fabs(INDATA[1]*INDATA[1]*(INDATA[2]-INDATA[3])/(INDATA[2]*INDATA[3]));
		else
			l=0.0;
	}

	//   R2----> R1  (SMALL---->big)             of spiral
	//    A=INDATA[1];R2=INDATA[2];R1=INDATA[3];L=A*A*(R2-R1)/R1/R2;L1=A*A/R1;L2=L1+L;ll=L1+l;
	if (j==6) 
	{
		if(fabs(INDATA[2]*INDATA[3])>0.00001)
			l=fabs(INDATA[1]*INDATA[1]*(INDATA[3]-INDATA[2])/(INDATA[2]*INDATA[3]));
		else
			l=0.0;
	}

	*jj=j;
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


//求距离线元 起点长度为L的坐标   //* 反传-》 struct xlpoint *PZ,
int BAS_DRAW_FUN_JYX::xlzbxy(struct xlpoint *P0,struct xlpoint *PZ,double l,double INDATA[5],int display)
{  
	double az,a0,aj,R,L,A,R1,R2,zy,L1,L2,ll,xm,ym,t1,t2,x,y;
	int type,re=1;
	double ar1,ar2,ar,xr,yr,xr1,yr1,xr2,yr2;
	a0=P0->a;
	type=(int)(INDATA[0]+0.0001);
	zy=INDATA[4];  // 左右偏
	PZ->lc=P0->lc+l;   //  待求点的里程 
	switch (type)
	{  case 1:    //   line
	PZ->x=P0->x+l*cos(a0);
	PZ->y=P0->y+l*sin(a0);
	PZ->a=P0->a;PZ->r=1e40;
	//	ads_printf(L"???  PZ->x=%lf,P0->x=%lf,l=%lf,cos(a0)=%lf\n",PZ->x,P0->x,l,cos(a0));
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
		PZ->x=P0->x+(x-xr)*cos(az+pi)+(y-yr)*cos(az + pi - zy * .5*pi);
		PZ->y=P0->y+(x-xr)*sin(az+pi)+(y-yr)*sin(az + pi - zy * .5*pi);
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

// extern	CArray<BZLCStruct,BZLCStruct>m_BZLC;
int  BAS_DRAW_FUN_JYX::xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ) 
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
	if(jslc<PB0.lc+0.0001)  //里程在起点里程之前
	{
		PZ->x = PB0.x;
		PZ->y =PB0.y;
		PZ->a = PB0.a;
		return 1;
	}
	if(jslc>array2[trow2][6]-0.0001)  //里程在终点里程之后
	{
		PZ->a = array2[trow2][5];//线元起点方位角
		PZ->x = array2[trow2][8];  //x
		PZ->y =array2[trow2][9];  //y

		return 1;
	}
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

			////圆+线路长度为0
			//if( fabs(INDATA2[0]-2)<0.00001 && l<0.00000001 ) PZ->a = INDATA2[3];
			//if(l<0.00000001)
			//	return 1;

			PBI->lc = array2[i][6];
			PBI->r  = array2[i][7];
			PBI->x  = array2[i][8];
			PBI->y  = array2[i][9];
			// l 为该线元的长度 m

			//	CCurvePokeCaluate a;
			//	PZ->lc= a.CalcmlTonewcml(PBI->lc,m_BZLC);


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
	{  	return (-9); // 无解
	};
}

//  在某层上画回旋线
AcDbObjectId   BAS_DRAW_FUN_JYX::drawspi(double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ,int bz,double ND)
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

_stprintf(buf,L"%-5.0f %21.11f %20.8f %20.8f %20.8f %15.8f %12.8f %20.8f %20.8f %20.8f %20.8f\0",
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
pRb = NULL;
pTemp = NULL;
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

AcDbObjectId  BAS_DRAW_FUN_JYX::GUXWXY(double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,int bz)
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

}
else if (j<=2)
{
	xlzbxy(P0,PZ,l*0.5,INDATA,0); // 某线元中点
	pt[X]=PZ->y;pt[Y]=PZ->x;
	xlzbxy(P0,PZ,l,INDATA,0); // 某线元终点
	pte[X]=PZ->y;pte[Y]=PZ->x;  //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
	if (j==1) 
	{
		//       acedCommandS(RTSTR,L"LINE", RTPOINT,ptb, RTPOINT, pte, RTSTR,L"",0);
		entid= makeline(ptb,pte,3);
	}
	else if (j==2) 
	{
		//   acedCommandS(RTSTR,L"ARC", RTPOINT,ptb,  RTPOINT, pt, RTPOINT, pte, 0);
		entid= makearc(ptb,pte,INDATA[1],(int)(INDATA[4]+0.001));        
	};
};  //J
}
else  if ( bz==0) {  xlzbxy(P0,PZ,l,INDATA,0); } ;  // 某线元终点
// P0=PZ;
return ( entid);
}

int BAS_DRAW_FUN_JYX::getmodel(double x1,double y1,double x2,double y2,double x3,double y3,double L1,double L2,double R)
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
struct resbuf* BAS_DRAW_FUN_JYX::entdxf( struct resbuf *entdata,int gcode)
{
	while ((entdata!=NULL) && (entdata->restype!=gcode)) entdata=entdata->rbnext;
	return entdata;
}

//求方位角
double BAS_DRAW_FUN_JYX:: xyddaa(double xb,double yb,double xe,double ye,double *f)
{	
	double dx,dy,dd,aa;

	dx=xe-xb;dy=ye-yb;dd=sqrt(dx*dx+dy*dy);
	if (fabs(dx)<0.000000001 ) {
		if (dy>=0.0) aa=pi*0.5;
		else if (dy<0.0) aa=pi*1.5;
		*f=aa;
		return dd;
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


double BAS_DRAW_FUN_JYX::spiral_x(double A,double l)
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

	return sum;
}

double BAS_DRAW_FUN_JYX::spiral_y(double A,double l)
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

	return sum;
}

long BAS_DRAW_FUN_JYX::factorialfun(int n)
{
	long result=1;
	for(int i=1;i<n+1;i++)result*=i;
	return result;
}

double BAS_DRAW_FUN_JYX::rad(double d_ms)
{	double d;
d=deg(d_ms);
return (d*pi/180.0);
}
double BAS_DRAW_FUN_JYX::dms_rad(double rad)
{	double d_ms,ddd;
ddd=rad*180.0/pi;
d_ms=dms(ddd);
return (d_ms);
}
double BAS_DRAW_FUN_JYX::dms(double ddd)
{	int d,m; double s;
d=int( ddd);
m=int ((ddd-d)*60.0);
s=double (ddd-d)*3600.0-m*60.0;
return (d+m/100.0+s/10000.0);
}

int BAS_DRAW_FUN_JYX::getBPMTL(double a,double R,double L1,double L2,double *BPMTL)
{ // BPTL[9]=B1,B2,P1,P2,M1,M2,T1,T2,L
	double A1,A2,aa,B1,B2,P1,P2,M1,M2,T1,T2,LS;
	A1=sqrt(R*L1);A2=sqrt(R*L2);
	aa=rad(a);

	if(R>1.0e-6)
	{
		B1=L1*0.5/R;
		B2=L2*0.5/R;
	}
	else
	{
		B1=0;
		B2=0;
	}


	if (L1<0.0001) { P1=0.0;M1=0.0; }
	else {     P1=spiral_p(A1,L1);M1=spiral_xm(A1,L1);   };
	if (L2<0.0001) { P2=0.0;M2=0.0; }
	else {     P2=spiral_p(A2,L2); M2=spiral_xm(A2,L2);  };
	// P1=spiral_p(A1,L1);P2=spiral_p(A2,L2);//P1,P2
	// M1=spiral_xm(A1,L1);M2=spiral_xm(A2,L2);// M1,M2

	if(fabs(aa)>1.0e-6)
	{
		T1=(R+P1)*tan(aa*0.5)+M1-(P1-P2)/sin(aa);
		T2=(R+P2)*tan(aa*0.5)+M2+(P1-P2)/sin(aa);
	}
	else
	{
		T1=0;
		T2=0;
	}

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

double BAS_DRAW_FUN_JYX::deg(double d_ms)
{	int d,m;
double s;
d=int(d_ms+0.000000001);m=int((d_ms-d)*100.0+0.00000000001);
s=d_ms*10000.0-d*10000.0-m*100.0;
return (d+m/60.0+s/3600.0);
}
double BAS_DRAW_FUN_JYX::spiral_xm(double A,double L)
{ double R,t,XM;
R=A*A/L;t=A*A/(2.0*R*R);
XM=spiral_x(A,L)-R*sin(t);
return(XM);
}

double BAS_DRAW_FUN_JYX::spiral_p(double A,double L)
{ double R,t,p;
R=A*A/L;t=A*A/(2.0*R*R);
p=spiral_y(A,L)+R*cos(t)-R;
return(p);
}
void BAS_DRAW_FUN_JYX::printList(struct resbuf* pBuf)
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
			// acedGetString(0,L"Press <ENTER> to continue...", buf);
			//  CAD R14
			ads_getstring(0,L"Press <ENTER> to continue...", buf);

		}
	}
	return;
}
void  BAS_DRAW_FUN_JYX::iterate(AcDbObjectId plineId)
{
	AcDb2dPolyline *pPline;
	if(acdbOpenObject(pPline, plineId, AcDb::kForRead)==Acad::eOk)
	{
		AcDbObjectIterator *pVertIter= pPline->vertexIterator();
		pPline->close();  // Finished with the pline header.

		AcDb2dVertex *pVertex;
		AcGePoint3d location;
		AcDbObjectId vertexObjId;
		for (int vertexNumber = 0; !pVertIter->done();
			vertexNumber++, pVertIter->step())
		{
			vertexObjId = pVertIter->objectId();
			if(acdbOpenObject(pVertex, vertexObjId,
				AcDb::kForRead)==Acad::eOk)
			{
				location = pVertex->position();
				pVertex->close();

				ads_printf(L"\nVertex #%d's location is"
					L" : %0.3f, %0.3f, %0.3f", vertexNumber,
					location[X], location[Y], location[Z]);
			}
		}
		delete pVertIter;
	}
}
AcDbObjectId  BAS_DRAW_FUN_JYX::createPl_xlpm(int trow,double arrayd[MAXJDNUM][6])
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
ACHAR* BAS_DRAW_FUN_JYX::LCchr(ACHAR  *GH,double lc,int NLC,int mode=1)
{	
	long int zlc,gl;
	double bm;
	ACHAR  ch1[100],ch[100];
	bm=lc;
	//	zlc=(long int)(lc/1000.0); 
	zlc=(long int)((lc+0.0001)/1000.0); 
	bm=bm-zlc*1000.0;
	if (fabs(bm-1000.0)<=pow(0.1,NLC)*0.5)
	{
		zlc+=1;
	}
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
void BAS_DRAW_FUN_JYX::calptan(double x1, double y1, double x2, double y2, double x3, double y3, double xt, double yt,double *tanl, double *R, double *xc,double *yc,double *ang,double *startfw,double *endfw,double *pj)
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
	//	acutPrintf(L"**>>>>** R=%lf\n",*R);

	double tmp;
	tmp = (*tanl)/cos(alpha);
	*xc = x2 + tmp*cos(alphac);
	*yc = y2 + tmp*sin(alphac);
	*ang = alpha;

	if ( inf  == 1 )
	{
		*startfw = efw + m_dHalfPI ;
		*endfw   = sfw + m_dHalfPI ;
	}
	else
	{
		*startfw = sfw - m_dHalfPI ;
		*endfw   = efw - m_dHalfPI ;
	};

	xyddaa(x1,y1,x2,y2,startfw);
	xyddaa(x2,y2,x3,y3,endfw);
	*pj = ZXJ(*startfw,*endfw);

}

double BAS_DRAW_FUN_JYX::get_l0(double SR,double Design_V,BOOL isJYGuiFanCS,GuiFanCanShu mGuiFanCS)
{
	//if(isJYGuiFanCS)
	//{
	//	mGuiFanCS = mJYGuiFanCS;
	//}
	//else
	//{
	//	mGuiFanCS = mGJGuiFanCS ;
	//}

	//用户没有设置参数
	//if(!mGuiFanCS.isSetGuiFan)
	//{
	//	return 10;
	//}

	if(Design_V<1) return 10;


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

	double l0 = 0.0 ;
	int kk = 0 ;
	if(i>=1)l0 = mGuiFanCS.L[i-1]+ (SR-mGuiFanCS.R[i-1])*(mGuiFanCS.L[i]-mGuiFanCS.L[i-1])/(mGuiFanCS.R[i]-mGuiFanCS.R[i-1]);
	kk = int(l0/10.0 + 0.99);
	l0 = kk * 10.0;

	return  l0;	
}

double BAS_DRAW_FUN_JYX::get_R(double SR,double Design_V, BOOL isJYGuiFanCS, GuiFanCanShu mGuiFanCS)//按照半径系列取半径值，寻找最接近半径
{

	//用户没有设置参数
	//if(!mGuiFanCS.isSetGuiFan)
	//{
	//	return SR;
	//}

	if(Design_V<10) return SR;

	int i=mGuiFanCS.RXiangShu-1;



	while(i>0 && mGuiFanCS.R[i]<SR-1)
	{
		i--;
	}

	if(i==mGuiFanCS.RXiangShu-1)
	{
		return mGuiFanCS.R[i];
	}
	else if(i==0)
	{
		if(SR>mGuiFanCS.R[0])
			return mGuiFanCS.R[0];
		else
			i++;
	}

	return  fabs(mGuiFanCS.R[i]-SR)>fabs(mGuiFanCS.R[i-1]-SR) ? mGuiFanCS.R[i-1] : mGuiFanCS.R[i];
}


//把实体集移动：1：实体ID集合，2：移动的方位角，3：移动距离
void BAS_DRAW_FUN_JYX::MoveEntities(AcDbObjectIdArray *EntityIds,double fwj,double dist)
{
	AcDbEntity *Ent;
	AcDbObjectId eId;
	AcGeVector3d norm(0,0,1.0);
	AcGeMatrix3d xformT;

	for(int i=0;i<EntityIds->length();i++)
	{
		eId=EntityIds->at(i);
		acdbOpenObject(Ent,eId,AcDb::kForWrite);

		AcGeVector3d moveBy(dist*sin(fwj),dist*cos(fwj),0);
		xformT.setToTranslation(moveBy);
		Ent->transformBy(xformT);		
		Ent->close();
	}
}

AcDbObjectId BAS_DRAW_FUN_JYX::CreateLayer(CString LayerName, ACHAR *LineType,AcCmColor color,AcDb::LineWeight weight)
{
	ACHAR laynam[256];
	_stprintf(laynam,L"%s",LayerName);
	return CreateLayer(laynam,LineType,color,weight);
}

AcDbObjectId BAS_DRAW_FUN_JYX::CreateLayer(ACHAR *LayerName, ACHAR *LineType,AcCmColor color,AcDb::LineWeight weight)
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
	pLayerTableRecord->setLineWeight(weight);
	AcDbLinetypeTable *pLinetypeTble;
	acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTble,AcDb::kForRead);
	AcDbObjectId ltypeObjId;
	pLinetypeTble->getAt(LineType,ltypeObjId,Adesk::kTrue);
	//	acutPrintf(L"\nId=%d",ltypeObjId);
	pLayerTableRecord->setColor(color);
	pLayerTableRecord->setLinetypeObjectId(ltypeObjId);
	pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	pLinetypeTble->close();
	return LayerTabRecordId;
}

/*
AcDbObjectId   BAS_DRAW_FUN::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType)
{
AcGePoint2dArray PtAy;
PtAy.append(AcGePoint2d(startPt.x,startPt.y));
PtAy.append(AcGePoint2d(endPt.x,endPt.y));
return makepolyline(PtAy,icolor,lw,LayerName,LineType);	    
}

*/

double BAS_DRAW_FUN_JYX::angleX(AcGePoint3d sPtL,AcGePoint3d ePtL)//Line(sPtl,ePtl)
{
	ads_point spt,ept;
	spt[X]=sPtL.x;
	spt[Y]=sPtL.y;
	ept[X]=ePtL.x;
	ept[Y]=ePtL.y;
	double ang=ads_angle(spt,ept);//与X轴夹角
	return ang;
}


double BAS_DRAW_FUN_JYX::angleX(AcDbLine *pL)
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


//////////////////////////////////////////////////////////////////////////

bool BAS_DRAW_FUN_JYX::CalFrame(double d12,double d23,double d34,double d41,AcGePoint3d P1,AcGePoint3d P2,AcGePoint3d P3,AcGePoint3d P4,ads_point &pt1,ads_point &pt2,ads_point &pt3,ads_point &pt4)
{
	AcGePoint3d Pt1,Pt2,Pt3,Pt4;
	if(CalFrame(d12,d23,d34,d41,P1,P2,P3,P4,Pt1,Pt2,Pt3,Pt4))
	{
		pt1[X]=Pt1.x;	pt1[Y]=Pt1.y;	pt1[Z]=Pt1.z;
		pt2[X]=Pt2.x;	pt2[Y]=Pt2.y;	pt2[Z]=Pt2.z;
		pt3[X]=Pt3.x;	pt3[Y]=Pt3.y;	pt3[Z]=Pt3.z;
		pt4[X]=Pt4.x;	pt4[Y]=Pt4.y;	pt4[Z]=Pt4.z;
		return true;
	}
	else return false;
}


bool BAS_DRAW_FUN_JYX::CalFrame(double d12,double d23,double d34,double d41,AcGePoint3d P1,AcGePoint3d P2,AcGePoint3d P3,AcGePoint3d P4,AcGePoint3d &Pt1,AcGePoint3d &Pt2,AcGePoint3d &Pt3,AcGePoint3d &Pt4)
{
	AcDbLine *L1=new AcDbLine(P1,P3);
	AcDbLine *L2=new AcDbLine(P2,P4);//两对角线
	AcGePoint3dArray O;//两对角线交点
	AcGePoint3d O121,O122,O232,O233,O343,O344,O414,O411;
	//三角形OP1P2缩放后的顶点为O、O121、O122
	//三角形OP2P3缩放后的顶点为O、O232、O233
	O121=P1;O122=P2;
	O232=P2;O233=P3;
	O343=P3;O344=P4;
	O414=P4;O411=P1;
	if(L1->intersectWith(L2,AcDb::kOnBothOperands,O)==Acad::eOk)
	{
		ZoomTriangleByPoint_O(d12,O[0],O121,O122);
		ZoomTriangleByPoint_O(d23,O[0],O232,O233);
		ZoomTriangleByPoint_O(d34,O[0],O343,O344);
		ZoomTriangleByPoint_O(d41,O[0],O414,O411);
		AcDbLine *L12=new AcDbLine(O121,O122);
		AcDbLine *L23=new AcDbLine(O232,O233);
		AcDbLine *L34=new AcDbLine(O343,O344);
		AcDbLine *L41=new AcDbLine(O414,O411);
		L41->intersectWith(L12,AcDb::kExtendBoth,O);
		L12->intersectWith(L23,AcDb::kExtendBoth,O);
		L23->intersectWith(L34,AcDb::kExtendBoth,O);
		L34->intersectWith(L41,AcDb::kExtendBoth,O);
		Pt1.set(O[1].x,O[1].y,0.0);
		Pt2.set(O[2].x,O[2].y,0.0);
		Pt3.set(O[3].x,O[3].y,0.0);
		Pt4.set(O[4].x,O[4].y,0.0);
		O.removeSubArray(0,O.length()-1);
		/*
		AddEntityToDbs(L1);
		AddEntityToDbs(L2);
		AddEntityToDbs(L12);
		AddEntityToDbs(L23);
		AddEntityToDbs(L34);
		AddEntityToDbs(L41);
		*/
		delete L12;	L12=NULL;
		delete L23;	L23=NULL;
		delete L34;	L34=NULL;
		delete L41;	L41=NULL;
		delete L1;	L1=NULL;
		delete L2;	L2=NULL;
		return true;
	}
	else 
	{
		delete L1;	L1=NULL;
		delete L2;	L2=NULL;

		return false;
	}	
}		



void BAS_DRAW_FUN_JYX::ZoomTriangleByPoint_O(double dist,AcGePoint3d O,AcGePoint3d &P1,AcGePoint3d &P2)
{
	double H=0.0;//三角形的高，即O到P1P2边的距离
	double l1=0.0;//O到P1的距离
	double l2=0.0;//O到P2的距离
	double jiao1=0.0;//边OP1的角度
	double jiao2=0.0;//边OP2的角度

	H=distptol(O,P1,P2);
	l1=O.distanceTo(P1);
	l2=O.distanceTo(P2);
	jiao1=angleX(O,P1);
	jiao2=angleX(O,P2);
	l1*=(H+dist)/H;//缩放后的边长
	l2*=(H+dist)/H;//缩放后的边长
	P1.x=O.x+l1*cos(jiao1);
	P1.y=O.y+l1*sin(jiao1);
	P2.x=O.x+l2*cos(jiao2);
	P2.y=O.y+l2*sin(jiao2);
}


double BAS_DRAW_FUN_JYX::distptol(AcGePoint3d pt, AcDbLine *Line)
{
	AcGePoint3d sPt=Line->startPoint();
	AcGePoint3d ePt=Line->endPoint();
	AcGePoint2d pk(pt.x,pt.y),sPt2d(sPt.x,sPt.y),ePt2d(ePt.x,ePt.y);
	AcGeLine2d pL(sPt2d,ePt2d);
	AcGeLine2d perpLine;
	pL.getPerpLine(pk,perpLine);//做垂线
	AcGePoint2d interPt0;//垂足在已知线上
	AcGeTol tol;
	pL.intersectWith(perpLine,interPt0,tol); //求垂足
	double dist0p=interPt0.distanceTo(pk);//垂足到拾取点之距
	return dist0p;
}

double BAS_DRAW_FUN_JYX::distptol(AcGePoint3d O, AcGePoint3d P1,AcGePoint3d P2)
{
	AcDbLine *Line=new AcDbLine(P1,P2);
	double dH;//三角形的高
	dH=distptol(O,Line);
	delete Line;	Line=NULL;
	return dH;
}



AcDbObjectId BAS_DRAW_FUN_JYX::CreateBlockFromDwgFile(ACHAR *Path,ACHAR *Name)
	//传入路径，图名，从图中读取所有实体做为一个块，块名为图名NAME，返回块的ID号
{
	AcDbObjectId pBlockId=0;
	AcDbDatabase *pDb=new AcDbDatabase(Adesk::kFalse);
	ACHAR PathName[256];
	_tcscpy(PathName,Path);
	_tcscat(PathName,Name);
	//	acutPrintf(Path);
	//	acutPrintf(Name);
	if(pDb->readDwgFile(PathName,_SH_DENYNO)==Acad::eOk)
	{
		acdbHostApplicationServices()->workingDatabase()->insert(pBlockId,Name,pDb);
	}
	delete pDb;
	return pBlockId;	
}


void BAS_DRAW_FUN_JYX::RToS(double lc,int NLC,ACHAR ch[80])
{
	if(NLC==0)_stprintf(ch,L"%.0lf",lc);
	else if(NLC==1)_stprintf(ch,L"%.1lf",lc);
	else if(NLC==2)_stprintf(ch,L"%.2lf",lc);
	else if(NLC==3)_stprintf(ch,L"%.3lf",lc);
	else if(NLC==4)_stprintf(ch,L"%.4lf",lc);
	else if(NLC==5)_stprintf(ch,L"%.5lf",lc);
	else if(NLC==6)_stprintf(ch,L"%.6lf",lc);
	else _stprintf(ch,L"%lf",lc);
}


AcDbObjectId BAS_DRAW_FUN_JYX::makepolyline(AcGePoint3dArray PtArray,bool isClosed,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType)
{
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	Adesk::Boolean closed = Adesk::kFalse;
	if(isClosed)closed = Adesk::kTrue;
	AcDb3dPolyline *pLine=new AcDb3dPolyline(AcDb::k3dSimplePoly,PtArray,closed);
	if(icolor>=0)pLine->setColorIndex(icolor);
	pLine->setLineWeight(lw);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	//	AcDbLinetypeTable *pLinetypeTble;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	if(LayerName==NULL)LayerName=L"0";
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord=new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pLine->setLayer(LayerTabRecordId);	
	pLayerTable->close();
	if(LineType!=NULL)
	{
		acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType,L"acadiso.lin");//
		pLine->setLinetype(LineType);
	}
	AcDbObjectId eId=AddEntityToDbs(pLine);
	acDocManager->unlockDocument(acDocManager->curDocument());
	return eId;	
}



AcDbObjectId BAS_DRAW_FUN_JYX::makepolyline(AcGePoint2dArray PtAy,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType)
{
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	int NUM=PtAy.length();
	AcDbPolyline *polyline=new AcDbPolyline(NUM);
	for(int i =0;i<NUM;i++)
	{
		polyline->addVertexAt(i,PtAy[i]);
	}
	if(icolor>=0)polyline->setColorIndex(icolor);
	polyline->setLineWeight(lw);

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
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	polyline->setLayer(LayerTabRecordId);	
	pLayerTable->close();
	if(LineType!=NULL)
	{
		acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType,L"acadiso.lin");//
		polyline->setLinetype(LineType);
	}
	AcDbObjectId  LId;
	LId = AddEntityToDbs(polyline) ;

	acDocManager->unlockDocument(acDocManager->curDocument());
	return LId;     
}

AcDbObjectId BAS_DRAW_FUN_JYX::makepolyline(AcGePoint2dArray PtAy,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType,double LTScale)
{
	int NUM = PtAy.length();

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
			/*
			acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType,L"mylinetype.lin");//
			*/
			//	ACHAR SysPath[256];
			ACHAR lineFname[256];
			//	GetPrivateProfileString("RLDVS",L"RLDVS",NULL,SysPath,256,L"win.ini");			
			_stprintf(lineFname,L"%s\\lib\\rail.lin",SysPath);

			acdbHostApplicationServices()->workingDatabase()->
				loadLineTypeFile(LineType,lineFname);//
		}

		polyline->setLinetype(LineType);
		polyline->setLinetypeScale(LTScale);
	}
	AcDbObjectId  LId;
	LId = AddEntityToDbs(polyline);
	return LId;     
}

void BAS_DRAW_FUN_JYX::makeDX(AcGePoint2dArray PtAy,double R,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType,bool dxflag)//导线
{
	//dxflag=true 为导线，起终两点也有圆
	AcGePoint3dArray pts;
	int num=PtAy.length();	
	//	AcGePolyline2d poly2d(PtAy);
	AcGeVector3d normal(0,0,1);
	if(dxflag)
	{
		AcGePoint3d cen(PtAy[0].x,PtAy[0].y,0.0);
		AcGePoint3d ptb(PtAy[0].x,PtAy[0].y,0.0);
		AcGePoint3d pte(PtAy[1].x,PtAy[1].y,0.0);
		AcDbCircle Cir(cen,normal,R);
		AcDbLine L1(ptb,cen);
		AcDbLine L2(cen,pte);
		Cir.intersectWith(&L1,AcDb::kOnBothOperands,pts);
		Cir.intersectWith(&L2,AcDb::kOnBothOperands,pts);
	}
	else pts.append(AcGePoint3d(PtAy[0].x,PtAy[0].y,0.0));
	int i;
	for( i=1;i<num-1;i++)//1--(n-1)头尾两点非交点
	{
		AcGePoint3d cen(PtAy[i].x,PtAy[i].y,0.0);
		AcGePoint3d ptb(PtAy[i-1].x,PtAy[i-1].y,0.0);
		AcGePoint3d pte(PtAy[i+1].x,PtAy[i+1].y,0.0);
		AcDbCircle Cir(cen,normal,R);
		AcDbLine L1(ptb,cen);
		AcDbLine L2(cen,pte);
		Cir.intersectWith(&L1,AcDb::kOnBothOperands,pts);
		Cir.intersectWith(&L2,AcDb::kOnBothOperands,pts);
	}
	pts.append(AcGePoint3d(PtAy[i].x,PtAy[i].y,0.0));
	//if(dxflag)
	//{
	//	AcGePoint3d cen(PtAy[i].x,PtAy[i].y,0.0);
	//	AcGePoint3d ptb(PtAy[i-1].x,PtAy[i-1].y,0.0);
	//	AcGePoint3d pte(PtAy[i+1].x,PtAy[i+1].y,0.0);
	//	AcDbCircle Cir(cen,normal,R);
	//	AcDbLine L1(ptb,cen);
	//	AcDbLine L2(cen,pte);
	//	Cir.intersectWith(&L1,AcDb::kOnBothOperands,pts);
	//	Cir.intersectWith(&L2,AcDb::kOnBothOperands,pts);
	//}
	//else pts.append(AcGePoint3d(PtAy[i].x,PtAy[i].y,0.0));
	//	int pts_len=(num-2)*2+2;
	int pts_len = pts.logicalLength();
	for(i=1;i<pts_len;i=i+2)
	{
		makeline2(pts[i-1],pts[i],icolor,lw,LayerName,LineType);
	}
}

AcDbObjectId BAS_DRAW_FUN_JYX::CreateLayer(ACHAR *LayerName, ACHAR *LineType)
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

AcDbObjectId BAS_DRAW_FUN_JYX::CreateLayer(ACHAR *LayerName)
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

void BAS_DRAW_FUN_JYX::caljd(double x1, double y1, double x2, double y2, double x_1, double y_1, double x_2, double y_2, double *xjd, double *yjd)
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
void BAS_DRAW_FUN_JYX::calzxjd(double x1, double y1, double x2, double y2, double a2, double b2, double c2, double *xjd, double *yjd)
{
	double a1,b1,c1,abc;
	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = y1*(x2-x1)-x1*(y2-y1);
	abc = a1*b2 -a2*b1;
	*yjd = (c1*a2 -a1*c2)/abc;
	*xjd =-(c1*b2 -b1*c2)/abc; 
}


AcDbObjectId  BAS_DRAW_FUN_JYX::makecircle(AcGePoint3d& startPt,double R,int icolor)
{   AcGeVector3d  normal(0.0,0.0,1.0);
AcDbCircle *pC = new AcDbCircle(startPt,normal, R);
pC->setColorIndex(icolor);

AcDbObjectId  LId;
LId=AddEntityToDbs(pC);
return LId;     
}

//double BAS_DRAW_FUN::XLC1( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[30])
//{   double LCB=0, XCLC ;  // LCB 断链的统一里程
//    int iDL=0,bz;
//      //起点里程
//    XCLC=TYLC;
//    _tcscpy(GH,DLBTMP[0].BGH);
//
//	do {
//	  bz=99;
//	  //该断链点统一里程
//	  if  (iDL==0) { LCB=DLBTMP[iDL].BLC+DLBTMP[iDL].DL;  }
//	  else {  LCB=LCB+DLBTMP[iDL].BLC-(DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL); };  
//      _tcscpy(GH,DLBTMP[iDL].EGH);
//	  
//	  if( (TYLC>LCB) &&(iDL<NDL))
//	  {   XCLC=(TYLC-LCB)+DLBTMP[iDL].BLC+DLBTMP[iDL].DL; 
//	      _tcscpy(GH,DLBTMP[iDL].EGH);
//	      iDL=iDL+1;
//		  bz=-99;
//	  }
//	  else if (TYLC<=LCB)
//	  {   XCLC=(TYLC-LCB)+DLBTMP[iDL].BLC; 
//	      _tcscpy(GH,DLBTMP[iDL].BGH);
//		  if(iDL==0)
//              _tcscpy(GH,DLBTMP[iDL].EGH);
//	      bz=99;	            
//	  };
//	} while (bz<0);
//    return XCLC;
//}

double BAS_DRAW_FUN_JYX::XLC1( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[MAXDLNUM])
{
	double LCB=0,XCLC ;  // LCB 断链的统一里程
	int iDL=0;
	CString strs1,strs2;

	for (iDL=0;iDL<NDL;iDL++)
	{
		//该断链点统一里程
		if(iDL > 0)
			LCB=LCB+DLBTMP[iDL].BLC-(DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL);
		else
			LCB=DLBTMP[iDL].BLC+DLBTMP[iDL].DL;

		if (iDL == 0)
		{
			if (TYLC < LCB)
			{
				strs1 = DLBTMP[0].BGH;
				strs1.MakeUpper();
				_tcscpy(GH,strs1);
				XCLC = DLBTMP[0].BLC - LCB + TYLC;
				break;
			}
			else if (fabs(TYLC-LCB)<0.00005)
			{
				if (fabs(DLBTMP[iDL].BLC-(int)DLBTMP[iDL].BLC)<0.00005)
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
			else if (fabs(TYLC-LCB)<0.00005)
			{
				if (fabs(DLBTMP[iDL].BLC-(int)DLBTMP[iDL].BLC)<0.00005)
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
double BAS_DRAW_FUN_JYX:: TYLC1( double XLC, int NDL,DLBZ DLBTMP[30] )
{   
	if (NDL <3)
	{
		return XLC;
	}
	double TYLCB = 0,LCB, LCE,TLC;  // LCB 断链的统一里程
	int iDL=0,bz;
	//起点里程
	TLC = XLC;
	bz = -99;
	for(iDL=0;iDL<NDL-1;iDL++)
	{
		//该断链点统一里程
		if  (iDL==0) 
		{ 
			TYLCB = DLBTMP[iDL].BLC + DLBTMP[iDL].DL;  
		}  // 起点的后链里程
		else 
		{  
			TYLCB = TYLCB + DLBTMP[iDL].BLC - (DLBTMP[iDL-1].BLC + DLBTMP[iDL-1].DL); 
		}

		//该断链后链里程
		LCB = DLBTMP[iDL].BLC + DLBTMP[iDL].DL;  
		//下一断链的前链里程
		LCE = DLBTMP[iDL+1].BLC; 
		if( (XLC >= LCB) &&( XLC <= LCE))  // 落在该段落内
		{   
			if( bz<0) 
			{  
				TLC= TYLCB + XLC - LCB;  
				bz=1; 
			}  // 第一次求到
			else 
			{ 
				bz=2; 
			}
		}
	}
	return TLC;
}



//  由断链数据表 求某现场里程的统一里程
double BAS_DRAW_FUN_JYX::TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] )
{   
	//  由断链数据表 求某现场里程的统一里程
	double TLC = 0.0;
	double TLCtmp = 0.0;
	int iDL=0;
	//起点里程
	double XLC;
	ACHAR GH[20];
	split_ckml(ckml,&XLC,GH);
	TLC=XLC;
	if (NDL <3)
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
	for (iDL = 1;iDL < NDL - 1; iDL++)//检查是否在长链内
	{
		continueLC = continueLC + DLBTMP[iDL].BLC - DLBTMP[iDL-1].DL - DLBTMP[iDL-1].BLC;

		if ((XLC - DLBTMP[iDL].BLC < 0.001) && (DLBTMP[iDL].BLC + DLBTMP[iDL].DL - XLC < 0.001))//
		{
			strstr2 = DLBTMP[iDL].BGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLCtmp = continueLC - DLBTMP[iDL].BLC + XLC;
				if (fabs(TLC - TLCtmp) > 0.0009)
				{
					numb++;
					TLC = TLCtmp;
				}
			}
			else
			{
				strstr2 = DLBTMP[iDL].EGH;
				strstr2.TrimLeft();
				strstr2.TrimRight();
				strstr2.MakeLower();
				if (strstr1 == strstr2)
				{
					TLCtmp = continueLC - DLBTMP[iDL].BLC - DLBTMP[iDL].DL + XLC;
					if (fabs(TLC - TLCtmp) > 0.0009)
					{
						numb++;
						TLC = TLCtmp;
					}
				}
			}
		}
	}

	if (numb == 0)//不在长链内
	{
		continueLC = DLBTMP[0].BLC + DLBTMP[0].DL;
		for (iDL = 0; iDL < NDL; iDL++)
		{
			if (iDL > 0)
			{
				if ((DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL-XLC < 0.001) && (XLC-DLBTMP[iDL].BLC < 0.001))
				{
					strstr2 = DLBTMP[iDL-1].EGH;
					strstr2.TrimLeft();
					strstr2.TrimRight();
					strstr2.MakeLower();
					if(strstr1 == strstr2)
					{
						TLC = continueLC + XLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL;
						numb++;
						break;
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
							break;
						}
					}
				}
				continueLC =continueLC + DLBTMP[iDL].BLC - DLBTMP[iDL-1].DL - DLBTMP[iDL-1].BLC;
			}
			else if (iDL == 0)
			{
				if (XLC - DLBTMP[0].BLC < 0.00005)
				{
					strstr2 = DLBTMP[0].BGH;
					strstr2.TrimLeft();
					strstr2.TrimRight();
					strstr2.MakeLower();
					if (strstr1 == strstr2)
					{
						TLC = continueLC - DLBTMP[0].BLC + XLC;
						numb++;
						break;
					}
				}
			}
		}
		if (iDL == NDL && XLC > DLBTMP[iDL-1].BLC + DLBTMP[iDL-1].DL - 0.001 )
		{
			TLC = continueLC + XLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL;
			numb++;
		}
	}
	strstr1 = ckml;
	CString err;
	if (numb == 1)
		TLC = ((int)(TLC * 1000.0 + 0.5))/1000.0;
	else if (numb > 1)
		ads_printf(L"系统发现%s在线路上有多个位置，可能是断链表数据有问题\n",strstr1 );
	else if (numb == 0)
	{
		//	ads_printf(L"系统发现%s在线路上没有其位置，可能是短链或断链表数据有问题\n",strstr1 );
		TLC = DmlToCml(TLC,DLBTMP,NDL);
	}
	return TLC;
}

bool BAS_DRAW_FUN_JYX::TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] , double& TLC)
{   
	//  由断链数据表 求某现场里程的统一里程
	TLC = 0.0;
	double TLCtmp = 0.0;
	int iDL=0;
	//起点里程
	double XLC;
	ACHAR GH[20];
	split_ckml(ckml,&XLC,GH);
	TLC=XLC;
	if (NDL <3)
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
	for (iDL = 1;iDL < NDL - 1; iDL++)//检查是否在长链内
	{
		continueLC = continueLC + DLBTMP[iDL].BLC - DLBTMP[iDL-1].DL - DLBTMP[iDL-1].BLC;

		if ((XLC - DLBTMP[iDL].BLC < 0.001) &&
			(DLBTMP[iDL].BLC + DLBTMP[iDL].DL - XLC < 0.001))//
		{
			strstr2 = DLBTMP[iDL].BGH;
			strstr2.TrimLeft();
			strstr2.TrimRight();
			strstr2.MakeLower();
			if (strstr1 == strstr2)
			{
				TLCtmp = continueLC - DLBTMP[iDL].BLC + XLC;
				if (fabs(TLC - TLCtmp) > 0.0009)
				{
					numb++;
					TLC = TLCtmp;
				}
			}
			else
			{
				strstr2 = DLBTMP[iDL].EGH;
				strstr2.TrimLeft();
				strstr2.TrimRight();
				strstr2.MakeLower();
				if (strstr1 == strstr2)
				{
					TLCtmp = continueLC - DLBTMP[iDL].BLC - DLBTMP[iDL].DL + XLC;
					if (fabs(TLC - TLCtmp) > 0.0009)
					{
						numb++;
						TLC = TLCtmp;
					}
				}
			}
		}
	}

	if (numb == 0)//不在长链内
	{
		continueLC = DLBTMP[0].BLC + DLBTMP[0].DL;
		for (iDL = 0; iDL < NDL; iDL++)
		{
			if (iDL > 0)
			{
				if ((DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL-XLC < 0.001) && (XLC-DLBTMP[iDL].BLC < 0.001))
				{
					strstr2 = DLBTMP[iDL-1].EGH;
					strstr2.TrimLeft();
					strstr2.TrimRight();
					strstr2.MakeLower();
					if(strstr1 == strstr2)
					{
						TLC = continueLC + XLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL;
						numb++;
						break;
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
							break;
						}
					}
				}
				continueLC =continueLC + DLBTMP[iDL].BLC - DLBTMP[iDL-1].DL - DLBTMP[iDL-1].BLC;
			}
			else if (iDL == 0)
			{
				if (fabs(XLC - DLBTMP[0].BLC) < 0.00005)
				{
					strstr2 = DLBTMP[0].BGH;
					strstr2.TrimLeft();
					strstr2.TrimRight();
					strstr2.MakeLower();
					if (strstr1 == strstr2)
					{
						TLC = continueLC - DLBTMP[0].BLC + XLC;
						numb++;
						break;
					}
				}
			}
		}
		if (iDL == NDL && fabs(XLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL) < 0.001 )
		{
			TLC = continueLC + XLC - DLBTMP[iDL-1].BLC - DLBTMP[iDL-1].DL;
			numb++;
		}
	}
	strstr1 = ckml;
	CString err;
	if (numb == 1)
	{
		TLC = ((int)(TLC * 1000.0 + 0.5))/1000.0;
		return true;
	}

	if (numb > 1)
	{
		//		ads_printf(L"系统发现%s在线路上有多个位置，可能是断链表数据有问题\n",strstr1 );
		return false;
	}
	if (numb == 0)
	{
		//		ads_printf(L"系统发现%s在线路上没有其位置，可能是短链或断链表数据有问题\n",strstr1 );
		TLC = DmlToCml(TLC,DLBTMP,NDL);
		return false;
	}

	return true;
}

//  由断链数据表 求某现场里程的统一里程
//double BAS_DRAW_FUN::TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] )
//{   
//	double TYLCB=0,LCB, LCE,TLC,XLC, tylc;  // LCB 断链的统一里程
//    int iDL=0,bz,kk=0;
//      //起点里程
//	ACHAR GH[10];
//	split_ckml(ckml,&XLC,GH);
//    TLC=XLC;
//    bz=-99;
//    for (iDL=0;iDL<NDL-1;iDL++)   // 
//	{
// 	  //该断链点统一里程
//	  if  (iDL==0) { TYLCB=DLBTMP[iDL].BLC+DLBTMP[iDL].DL;  }  // 起点的后链里程
//	  else {  TYLCB=TYLCB+DLBTMP[iDL].BLC-(DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL); };  
//	  //该断链后链里程
//	  LCB=DLBTMP[iDL].BLC+DLBTMP[iDL].DL;  
//      //下一断链的前链里程
//	  LCE=DLBTMP[iDL+1].BLC; 
//	  if( (XLC>=LCB) &&( XLC<=LCE+0.01))  // 落在该段落内
//	  {   
//		  if( bz<0) {  TLC= TYLCB+XLC-LCB;  bz=1;kk=iDL;tylc=TYLCB;}  // 第一次求到
//		  else 
//		  { bz=2; /* ads_printf(L"\n 输入的里程有两个里程点：1:=%12.2f 2:=%12.2f  ",
//			                                                  TLC,TYLCB+XLC-LCB);  */
//		  		if(_tcscmp(GH,DLBTMP[iDL].EGH)==0)
//				   TLC=TYLCB+XLC-LCB;
//		  }
//	  }
//	 }
//	if(bz==1&&fabs(XLC-(DLBTMP[kk+1].BLC+DLBTMP[kk+1].DL))<0.001&&_tcscmp(GH,DLBTMP[kk+1].EGH)==0)//只求到一次,里程与断后里程一致
//		TLC=tylc+DLBTMP[kk+1].BLC-(DLBTMP[kk].BLC+DLBTMP[kk].DL);
//	 return TLC;     
//}
///*
//  由断链数据表 求某现场里程的统一里程
//double BAS_DRAW_FUN::TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] )
//{   
//	double TYLCB=0,LCB, LCE,TLC,XLC, tylc;  // LCB 断链的统一里程
//    int iDL=0,bz,kk=0;
//      //起点里程
//	ACHAR GH[10];
//	struct TYLCdata
//	{
//		double cml;
//		int idl;//断链区间 ckml > DLB[idl].Eml,ckml<DLB[idl+1].Bml
//	} tylcdata[10];
//	int ntylc=0;
//		
//	split_ckml(ckml,&XLC,GH);
//    TLC=XLC;
//    bz=-99;
//    for (iDL=0;iDL<NDL-1;iDL++)   // 
//	{
// 	  //该断链点统一里程
//	  if  (iDL==0) { TYLCB=DLBTMP[iDL].BLC+DLBTMP[iDL].DL;  }  // 起点的后链里程
//	  else {  TYLCB=TYLCB+DLBTMP[iDL].BLC-(DLBTMP[iDL-1].BLC+DLBTMP[iDL-1].DL); };  
//	  //该断链后链里程
//	  LCB=DLBTMP[iDL].BLC+DLBTMP[iDL].DL;  
//      //下一断链的前链里程
//	  LCE=DLBTMP[iDL+1].BLC; 
//	  if( (XLC>=LCB) &&( XLC<=LCE))  // 落在该段落内
//	  {   
//		  if( bz<0) 
//		  { 
//			  TLC= TYLCB+XLC-LCB;  bz=1;kk=iDL;tylc=TYLCB;
//              tylcdata[ntylc].cml=TLC;
//			  tylcdata[ntylc].idl=iDL;
//			  ntylc++;
//		  }  // 第一次求到
//		  else 
//		  { bz=2; / * ads_printf(L"\n 输入的里程有两个里程点：1:=%12.2f 2:=%12.2f  ",
//			                                                  TLC,TYLCB+XLC-LCB);  * /
//		  	//	if(_tcscmp(GH,DLBTMP[iDL].EGH)==0)
//		      TLC=TYLCB+XLC-LCB;
//			  tylcdata[ntylc].cml=TLC;
//			  tylcdata[ntylc].idl=iDL;
//			  ntylc++;
//		  }
//	  }
//	 }
//	if(ntylc==1)//只有一个里程
//	{
//		if(bz==1&&fabs(XLC-(DLBTMP[kk+1].BLC+DLBTMP[kk+1].DL))<0.001&&_tcscmp(GH,DLBTMP[kk+1].EGH)==0)//只求到一次,里程与断后里程一致
//			TLC=tylc+DLBTMP[kk+1].BLC-(DLBTMP[kk].BLC+DLBTMP[kk].DL);
//		
//	}
//	else if(ntylc>1)//不只一个里程,应选取一个里程
//	{
//		//打开文件
//		int dlno = FindWichDLFromFile(GH,XLC);
//		if(dlno<0)//没找到
//		{
//			//对话框用户指定
//		}
//		
//	}
//	 return TLC;     
//}


//plh05.13
double  BAS_DRAW_FUN_JYX::PROJ_ML(double array2[4*MAXJDNUM][10],int trow2,
								  double xout, double yout, double xon, double yon)
{
	double x1=0.0,y1=0.0,x2=0.0,y2=0.0,x3=0.0,y3=0.0,x4=0.0,y4=0.0,s1=0.0,s2=0.0,s=0.0,ZJ=0.0,Alf1=0.0,Alf2=0.0,
		Alf = 0.0,Xo = 0.0,Yo = 0.0 ,q = 0.0,x=0.0,y=0.0,ZJ1=0.0,ZJ2=0.0,R=0.0;
	int i,kk[100] ,ii,xynum=0;
	double LorR,X1[100],Y1[100],X2[100],Y2[100];      


	double lc=array2[0][4];
	//	PB0.x=array2[0][1];
	//	PB0.y=array2[0][2];
	//	PB0.a=rad(array2[0][3]);
	//	PB0.r=1e40;
	//起点向前延长1000m
	/*	for(i=0; i<=trow2;i++)
	{
	ads_printf(L"i=%d 0=%lf 8=%lf 9=%lf\n",i,array2[i][0],array2[i][8],array2[i][9]);
	}*/


	x2 = array2[0][1] + 1000*cos(rad(array2[0][3])+pi);
	y2 = array2[0][2] + 1000*sin(rad(array2[0][3])+pi);

	for(i=1; i<=trow2;i++)
	{
		x1 = x2;
		y1 = y2;
		x2 = array2[i][8];
		y2 = array2[i][9];

		q = array2[i][0];
		//  ads_printf(L"##x1=%lf,y1=%lf ,x2=%lf,y2=%lf,q=%lf\n",x1,y1,x2,y2,q);
		LorR = 1.0;
		if(q > 1.1)
			//		ads_printf(L"%lf\n",array2[i][4]);
				LorR = array2[i][4];
		if(i>1)
			Alf1 = array2[i-1][5] ;
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



		if(s1*s2 < 0.0001 || fabs(s1)<0.0001 || fabs(s2) <0.0001)
		{

			// ads_alert(L"指定点无法投影到指定线段上, 请重新指定线段!");
			kk[xynum] = i;
			X1[xynum]=x1;
			Y1[xynum]=y1;
			X2[xynum]=x2;
			Y2[xynum]=y2;
			//			acutPrintf(L"**********i=%d\n",i);
			//		    goto L10;
			xynum++;//搜索出所有可能线元


			//		   return  -1.0;
		}

	}

	if(xynum==0)
	{
		//		ads_printf(L"指定点%lf %lf不在线路上(1)!\n",xout,yout);
		return -1.0;
	}
L10:
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


		if(kk[i]>1)
			lc = array2[kk[i]-1][6];//当前线元起始里程
		else lc = array2[0][4];

		//	ads_printf(L">>??  lc=%lf\n",lc);

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

			Xo = array2[kk[i]-1][8] + R*cos(array2[kk[i]-1][5]+LorR * m_dHalfPI);
			Yo = array2[kk[i]-1][9] + R*sin(array2[kk[i]-1][5]+LorR * m_dHalfPI);


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

			//	   s = XY_to_ML(x , y,array2,trow2);
			//	   ads_printf(L"1##s=%lf\n",s);

			cml[i] = Circle_XY_to_ML(x,y,array2,kk[i]-1);
			//	ads_printf(L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 22222   i=%d,cml[i]=%lf\n",i,cml[i]);

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
			if(fabs(ZJ1) > m_dHalfPI)
				ii = 1;
			else
				ii = -1;

			s1 = lc;
			s2 = array2[(kk[i])][6];

			//		int CountAddNum = 0;
L20:
			s = 0.5*(s1+s2);
			cml[i] = s;
			struct xlpoint PZ;
			PZ.lc = s;

			xlpoint_pz(array2,trow2,&PZ); 

			//	CAL_XY(s, &x3,&y3,&z3);
			xyddaa( xout,yout,PZ.x,PZ.y,&Alf);		
			//	Alf = FAngle(x3-xout , y3-yout);
			ZJ = ZXJ(Alf , PZ.a);

			if((ii==1 && fabs(ZJ)>m_dHalfPI)|| (ii==-1 && fabs(ZJ)<m_dHalfPI))
				s1 = s;
			else
				s2 = s;
			//		CountAddNum ++;

			//		if(CountAddNum>1000)continue;
			if(fabs(s2-s1) > 0.00001)
				goto L20;
			//	ads_printf(L"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 3333333   i=%d,cml[i]=%lf\n",i,cml[i]);

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
		/*	if(fabs(yout-575.886493)<1.0 && fabs(yout-856.660367)>1.0 )
		acutPrintf(L"########   i=%d,xynum=%d,lc=%lf,dist=%lf,MinDis=%lf\n",i,xynum,cml[i],dist,MinDis);*/
		if(MinDis>dist)
		{
			s = cml[i];
			MinDis = dist;
		}
	}

	/*	if(fabs(yout-575.886493)<1.0 && fabs(yout-856.660367)>1.0 )
	{
	if(fabs(s-1292)<1.0)
	{
	for(int k=0;k<=trow2;k++)
	{
	for(int j=0;j<10;j++)
	ads_printf(L"= %lf  ",array2[k][j]);

	ads_printf(L"\n");
	}
	}
	}*/

	return s;
}
//---------------------------------------------------------------------
double  BAS_DRAW_FUN_JYX::GetSide(double Xo,double Yo,double x1,double y1,double x,double y)
	//左侧-，右+
{
	double s;

	s = (x-Xo)*(y1-Yo)-(x1-Xo)*(y-Yo);
	return s;
}

//---------------------------------------------------------------------
double   BAS_DRAW_FUN_JYX::ZXJ(double s2,double s1)
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
double  BAS_DRAW_FUN_JYX::XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int trow2)
{
	double x1,y1,x2,y2,s1,s2,s,ss1,ss2;
	int i,kk;
	//   FILE *e;

	//  fpw = _wfopen(L"c:\\ph\\data" ,L"w");

	double lc=array2[0][4];
	x2 = array2[0][1] , y2 = array2[0][2];

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
	ss1 = sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
	ss2 = sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
	s = (s1*ss2+s2*ss1)/(ss1+ss2);

	return s;	
}

double  BAS_DRAW_FUN_JYX::Circle_XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int kk)
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
bool BAS_DRAW_FUN_JYX::split_ckml(ACHAR str[], double *ml, ACHAR  *ck)
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
	if(len==0)
		return false;


	if (lplus == 0)  {
		for (i=len,j=0; i<str_len; i++,j++)
			cml[j]=str[i];
		cml[j]='\0';
		if(len==str_len)
			return false;
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

	return true;
}

void BAS_DRAW_FUN_JYX::split_ckml(ACHAR ckml[80],double &XLC,ACHAR GH[10])
{
	CString strLCtmp = ckml;
	strLCtmp.TrimLeft();
	strLCtmp.TrimRight();
	if (strLCtmp.IsEmpty())
	{
		XLC = 0.0;
		_tcscpy(GH,L"");
		return;
	}
	int iBitofK = -1;//K
	int iBitofk = -1;//k
	int iBitofJ = -1;//+
	int itmp = -1;
	while (TRUE)
	{
		itmp = strLCtmp.Find('K',iBitofK + 1);
		if (itmp >= 0)
			iBitofK = itmp;
		else
			break;
	}
	while (TRUE)
	{
		itmp = strLCtmp.Find('k',iBitofk + 1);
		if (itmp >= 0)
			iBitofk = itmp;
		else
			break;
	}
	iBitofK = (iBitofK > iBitofk ? iBitofK : iBitofk);
	if (iBitofK < 0)
	{
		XLC = 0.0;
		_tcscpy(GH,L"");
		return;
	}
	iBitofJ = strLCtmp.Find('+',iBitofK + 1);
	_tcscpy(GH,strLCtmp.Left(iBitofK + 1));

	if (iBitofJ > iBitofK)//有+号
	{
		int k = _wtoi(strLCtmp.Mid(iBitofK + 1, iBitofJ - iBitofK - 1));
		XLC = _wtof(strLCtmp.Right(strLCtmp.GetLength() - iBitofJ - 1));
		XLC = k*1000 + XLC;
	}
	else
	{
		XLC = _wtof(strLCtmp.Right(strLCtmp.GetLength() - iBitofK - 1));
	}
}

/*----------------------------------------------------------*
* Name:  determain the location of ACHARacter C in string
variable STR[], if found, return subscript of C in STR[],
otherwise, return 0
*----------------------------------------------------------*/
int BAS_DRAW_FUN_JYX::F_num(ACHAR *str, ACHAR c)
{
	int i, len, num=0, smark, c1;

	smark = toupper(c);
	len = _tcslen(str);
	for(i=len-1; i>=0; i--)  {
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
void BAS_DRAW_FUN_JYX::putplus(ACHAR str[], ACHAR in_ch, ACHAR s_ch, int place)
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


AcDbObjectId  BAS_DRAW_FUN_JYX::make2dline(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,ACHAR LayerName[])
{
	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	if(LayerName==NULL) LayerName=L"0";
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord=new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pLayerTable->close();
	int i;
	for(i=0; i<ptarray.logicalLength();i++)
		ptarray[i][0] = DrawZorY*ptarray[i][0];

	AcDb2dPolyline *pLine	= new AcDb2dPolyline(AcDb::k2dSimplePoly,ptarray,0,Adesk::kFalse,startWid,EndWid);
	pLine->setColorIndex(icolor) ;
	pLine->setLayer(LayerTabRecordId);
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	for(i=0; i<ptarray.logicalLength();i++)
		ptarray[i][0]=DrawZorY*ptarray[i][0];

	return LId ;
}

void BAS_DRAW_FUN_JYX::atodmss(double alpha, int &d, ACHAR mm[4], ACHAR ss[4])
{
	int m, s;
	d = floor(fabs(alpha));
	m = floor((fabs(alpha)-d)*100.0+0.0001);
	s = floor(((fabs(alpha)-d)*100.0-m)*100.0+0.0001);

	if (m <10)
		_stprintf (mm,L"0%d'\0", m);
	else
		_stprintf (mm,L"%d'\0", m);
	if (s <10)
		_stprintf (ss,L"0%d\"\0", s);
	else
		_stprintf (ss,L"%d\"\0", s);
}


/*===================================================================

函数功能:转化成度分秒，秒带小数位	
-----------------------------------------------------
输入:	Pres 小数位数
-----------------------------------------------------
输出:   
-----------------------------------------------------
算法描述:   
-----------------------------------------------------

备注:	                                 钟晶 2008:10:15
=====================================================================*/
void BAS_DRAW_FUN_JYX::atodmss1(double alpha, int &d, ACHAR mm[4], ACHAR ss[12],int Pres)
{
	int m;
	d = floor(fabs(alpha));
	m = floor((fabs(alpha)-d)*100.0+0.0001);
	double s = ((fabs(alpha)-d)*100.0-m)*100.0;

	if (m <10)
		_stprintf (mm,L"0%d'\0", m);
	else
		_stprintf (mm,L"%d'\0", m);
	if (s <10)
	{
		if (Pres==0)
		{
			_stprintf (ss,L"0%.0lf\"\0", s);
		}
		else if (Pres==1)
		{
			_stprintf (ss,L"0%.1lf\"\0", s);
		}
		else if (Pres==2)
		{
			_stprintf (ss,L"0%.2lf\"\0", s);
		}
		else if (Pres==3)
		{
			_stprintf (ss,L"0%.3lf\"\0", s);
		}
		else
		{
			_stprintf (ss,L"0%.4lf\"\0", s);
		}
	}
	else
	{
		if (Pres==0)
		{
			_stprintf (ss,L"%.0lf\"\0", s);
		}
		else if (Pres==1)
		{
			_stprintf (ss,L"%.1lf\"\0", s);
		}
		else if (Pres==2)
		{
			_stprintf (ss,L"%.2lf\"\0", s);
		}
		else if (Pres==3)
		{
			_stprintf (ss,L"%.3lf\"\0", s);
		}
		else
		{
			_stprintf (ss,L"%.4lf\"\0", s);
		}
	}
}

void BAS_DRAW_FUN_JYX::NoteBG(AcGePoint3d Opt,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,ACHAR layername[])
{
	DrawBGTri(Opt,sfactor,iColor,layername);

	AcGePoint3d origin;
	origin.x = Opt.x;
	origin.y = Opt.y + 0.25*sfactor;
	origin.z = Opt.z;
	maketext(origin,Str,0,texth,itextColor,mode,layername);	
}

void BAS_DRAW_FUN_JYX::DrawBGTri(AcGePoint3d Opt,double sfactor,int iColor,ACHAR LayerName[])
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

void BAS_DRAW_FUN_JYX::dlayer(CString layername)//删除层
{
	ACHAR ln[256];
	_stprintf(ln,L"%s",layername);
	dlayer(ln);
}
//把实体集旋转 ：1：实体ID集合，2：旋转中心，3：旋转角
void BAS_DRAW_FUN_JYX::RotateEntities(AcDbObjectIdArray *EntityIds, AcGePoint3d Pos, double jiao)
{
	AcDbEntity *Ent;
	AcDbObjectId eId;
	AcGeVector3d norm(0,0,1.0);
	AcGeMatrix3d xformR;

	for(int i=0;i<EntityIds->length();i++)
	{
		eId=EntityIds->at(i);
		/*acdbOpenObject(Ent,eId,AcDb::kForWrite);*/
		if(acdbOpenObject(Ent,eId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}

		xformR.setToRotation(jiao,norm,Pos);
		Ent->transformBy(xformR);

		Ent->close();
	}
}

void BAS_DRAW_FUN_JYX::LCChr(CString GH, CString LC,int NLC,CString &LCStr)
{
	LCStr=GH;
	double xlc=_wtof(LC);//不带冠号的现场里程
	long int kw=(long int)(xlc/1000.0+0.0001);//千位
	ACHAR ch[20];
	ads_rtos(kw,2,0,ch);
	LCStr+=ch;
	double yu=xlc-kw*1000.000;//千位以后
	ACHAR yw[20];
	yu+=1000.0;//if(000.2||003.5)
	acdbRToS(yu,2,NLC,yw);
	yw[0]='+';
	LCStr+=yw;
}

void BAS_DRAW_FUN_JYX::dlayer(ACHAR lname[])//删除层
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
		//		acutPrintf(L"\nfail to run ads_ssget function.");
		return ;
	}

	if (ads_ssfree(sname) != RTNORM)
	{
		acutPrintf(L"\nsname free error");
		return ;
	}

}
double BAS_DRAW_FUN_JYX::get_l0(double Design_V,int KNDD,double SR, GuiFanCanShu mGuiFanCS)
{

	//用户没有设置参数
	//if(!mGuiFanCS.isSetGuiFan)
	//{
	//	return 10;
	//}

	if(Design_V<1) return 10;

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

int BAS_DRAW_FUN_JYX::crossll(ads_point p1, ads_point p2, ads_point p3, ads_point p4, ads_point & pp)
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
void BAS_DRAW_FUN_JYX::dmltockml(CString GH,double dml,ACHAR ckml[80])
{
	//double ys=fmod(dml,1000.0);
	//if(fabs(ys-1000.0)<0.001)
	//{
	//	_stprintf(ckml,L"%s%d+000",GH,int(dml/1000+0.01));
	//	return;
	//}
	//if(GH!="")
	//{
	//	//if(ys>=100.0 || fabs(ys-100.0)<0.001)
	//	if(ys>=100.0)
	//		_stprintf(ckml,L"%s%d+%0.3lf",GH,int(dml/1000),ys);
	//	else if(ys>=10.0 || fabs(ys-10.0)<0.001)
	//		_stprintf(ckml,L"%s%d+0%0.3lf",GH,int(dml/1000),ys);			
	//	else if(ys>0.0)
	//		_stprintf(ckml,L"%s%d+00%0.3lf",GH,int(dml/1000),ys);			
	//	else
	//		_stprintf(ckml,L"%s%d+000",GH,int(dml/1000));
	//}
	//else
	//{
	//	//if(ys>=100.0 || fabs(ys-100.0)<0.001)
	//	if(ys>=100.0)
	//		_stprintf(ckml,L"%s%d+%0.0lf",GH,int(dml/1000),ys);
	//	else if(ys>=10.0 || fabs(ys-10.0)<0.001)
	//		_stprintf(ckml,L"%s%d+0%0.0lf",GH,int(dml/1000),ys);			
	//	else if(ys>0.0)
	//		_stprintf(ckml,L"%s%d+00%0.0lf",GH,int(dml/1000),ys);			
	//	else
	//		_stprintf(ckml,L"%s%d+000",GH,int(dml/1000));
	//}
	ACHAR GHStr[8];
	_tcscpy(GHStr , GH);
	_tcscpy(ckml,LCchr(GHStr,dml,3,1));

}

void BAS_DRAW_FUN_JYX::NullInStrto$(CString& text)
{
	int len,i;

	len = text.GetLength();
	i = text.Find(L" ");
	//	ACHAR mes[80];
	while(i < len && i >=0)
	{
		text.SetAt(i,'$');
		i = text.Find(L" ");
	}
	if(len==0)
		text="$";	
}

void BAS_DRAW_FUN_JYX::InStr$toNull()
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

int BAS_DRAW_FUN_JYX::ReadWorkdir()
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

double BAS_DRAW_FUN_JYX::get_xjj(double Design_V, GuiFanCanShu mGuiFanCS)
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

double BAS_DRAW_FUN_JYX::get_XJJK(double Design_V,double SR,bool IsJYGuifan, GuiFanCanShu mGuiFanCS)//线间距加宽
{
	/*
	double R[20],JK[20];
	int NR;
	if(Design_V>161)
	return 0.0;//不加宽
	if (fabs(Design_V- 160 )<0.1)
	{ 
	R[0]=1200,R[1]=1400,R[2]=1600,R[3]=1800,R[4]=2000,R[5]=2500,R[6]=3000,R[7]=4000,R[8]=5000,R[9]=6000,R[10]=8000,R[11]=10000;
	JK[0]=0.21,JK[1]=0.19,JK[2]=0.15,JK[3]=0.135,JK[4]=0.125,JK[5]=0.11,JK[6]=0.1,JK[7]=0.085,JK[8]=0.08,JK[9]=0.065,JK[10]=0.05,JK[11]=0.035;
	NR = 12;			  	  
	}
	else if (fabs(Design_V- 140 )<0.1)
	{ 
	R[0]=1200,R[1]=1400,R[2]=1600,R[3]=1800,R[4]=2000,R[5]=2500,R[6]=3000,R[7]=4000,R[8]=5000,R[9]=6000,R[10]=8000,R[11]=10000;
	JK[0]=0.21,JK[1]=0.19,JK[2]=0.15,JK[3]=0.135,JK[4]=0.125,JK[5]=0.11,JK[6]=0.1,JK[7]=0.085,JK[8]=0.08,JK[9]=0.065,JK[10]=0.05,JK[11]=0.035;
	NR = 12;			  	  		
	}
	else if ( fabs(Design_V - 120)<0.1)
	{ 
	R[0]=800,R[1]=1000,R[2]=1200,R[3]=1400,R[4]=1600,R[5]=1800,R[6]=2000,R[7]=2500,R[8]=3000,R[9]=4000,R[10]=5000,R[11]=6000,R[12]=8000,R[13]=10000;
	JK[0]=0.265,JK[1]=0.22,JK[2]=0.165,JK[3]=0.15,JK[4]=0.135,JK[5]=0.125,JK[6]=0.115,JK[7]=0.1,JK[8]=0.09,JK[9]=0.085,JK[10]=0.055,JK[11]=0.05,JK[12]=0.035,JK[13]=0.03;
	NR = 14;			  	  		
	}
	else if (fabs(Design_V - 100 )<0.1)
	{
	R[0]=550,R[1]=600,R[2]=700,R[3]=800,R[4]=1000,R[5]=1200,R[6]=1400,R[7]=1600,R[8]=1800,R[9]=2000,R[10]=2500,R[11]=3000,R[12]=4000,R[13]=5000,R[14]=6000,R[15]=8000,R[16]=10000;
	JK[0]=0.315,JK[1]=0.295,JK[2]=0.26,JK[3]=0.21,JK[4]=0.175,JK[5]=0.155,JK[6]=0.135,JK[7]=0.125,JK[8]=0.11,JK[9]=0.105,JK[10]=0.1,JK[11]=0.08,JK[12]=0.055,JK[13]=0.04,JK[14]=0.035,JK[15]=0.025,JK[16]=0.02;
	NR = 17;			  	  		
	}
	else if (fabs(Design_V - 80)<0.1)
	{
	R[0]=350,R[1]=400,R[2]=450,R[3]=500,R[4]=550,R[5]=600,R[6]=700,R[7]=800,R[8]=1000,R[9]=1200,R[10]=1400,R[11]=1600,R[12]=1800,R[13]=2000,R[14]=2500,R[15]=3000,R[16]=4000,R[17]=5000,R[18]=6000,R[19]=8000,R[20]=10000;
	JK[0]=0.41,JK[1]=0.365,JK[2]=0.335,JK[3]=0.28,JK[4]=0.255,JK[5]=0.235,JK[6]=0.21,JK[7]=0.19,JK[8]=0.155,JK[9]=0.135,JK[10]=0.125,JK[11]=0.115,JK[12]=0.1,JK[13]=0.095,JK[14]=0.07,JK[15]=0.065,JK[16]=0.04,JK[17]=0.035,JK[18]=0.025,JK[19]=0.015,JK[20]=0.015;
	NR = 21;			  	  		
	}
	else if (Design_V < 75)
	{
	R[0]=150,R[1]=180,R[2]=200,R[3]=250,R[4]=300,R[5]=350,R[6]=400,R[7]=450,R[8]=500,R[9]=550,R[10]=600,R[11]=700,R[12]=800,R[13]=1000,R[14]=1200,R[15]=1500,R[16]=2000,R[17]=2500,R[18]=3000,R[19]=4000;
	JK[0]=0.715,JK[1]=0.62,JK[2]=0.575,JK[3]=0.49,JK[4]=0.43,JK[5]=0.37,JK[6]=0.325,JK[7]=0.29,JK[8]=0.26,JK[9]=0.235,JK[10]=0.215,JK[11]=0.185,JK[12]=0.16,JK[13]=0.13,JK[14]=0.11,JK[15]=0.085,JK[16]=0.065,JK[17]=0.05,JK[18]=0.045,JK[19]=0.035;
	NR = 20;			  	  		
	}
	int i=0;
	while(i<NR && R[i]<SR-1)
	i++;
	if(i==0)
	return JK[0];
	else if(i==NR)
	{
	if(SR>R[NR-1])
	return 0.0;//不加宽
	else
	i--;
	}
	double jk = JK[i-1] + (JK[i]-JK[i-1])*(SR-R[i-1])/(R[i]-R[i-1]);
	return  jk;*/

	//if (IsJYGuifan)
	//	mGuiFanCS = mJYGuiFanCS;
	//else
	//	mGuiFanCS = mGJGuiFanCS;

	if(SR<0.001)return 0.0 ;
	int i=mGuiFanCS.RXiangShu-1;
	while(i>=0 && mGuiFanCS.R[i]<SR-1)
		i--;

	if(i==mGuiFanCS.RXiangShu-1)
		return mGuiFanCS.JiaKuan[i]/1000.0;
	else if(i==-1)
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

double  BAS_DRAW_FUN_JYX::Search_EXPt(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3],double jiao,double ResPt[2])
	//二线线元参数，线元个数，给定线外一点与角度定出直线找到该直线与二线交点ResPt及里程projml
{
	int sk = Search_XY(array2,trow2,Pt1);
	//ads_printf(L"\nsk=%d",sk);
	double Spt[3],Ept[3];
	double x1,y1,x2,y2;
	Spt[0] = Pt1[0] , Spt[1] = Pt1[1] ,Spt[2]=Pt1[2];
	Ept[0] = Pt1[0] + 1000*cos(jiao);
	Ept[1] = Pt1[1] + 1000*sin(jiao);

	int ek = Search_XY(array2,trow2,Ept);

	int Kmin,Kmax;
	Kmin = sk < ek ? sk : ek;
	Kmax = sk < ek ? ek : sk;

	if(Kmin>1)
		Kmin--;
	if(Kmax<trow2-1)
		Kmax++;

	double ProjMl=-1.0;
	int inf=1;

	//	ads_printf(L"Kmin=%d Kmax=%d\n",Kmin,Kmax);
	if(Kmin>1)
		x2 = array2[Kmin-1][8] , y2 = array2[Kmin-1][9];
	else
	{
		//起始线元延长100m
		double fwj = array2[1][5]+pi;
		x2 = array2[0][1] + 100.0*cos(fwj);
		y2 = array2[0][2] + 100.0*sin(fwj);

		//x2 =  , y2 = array2[0][2];
	}
	double xmin,ymin,distmin,dist,ProjMlmin;//距Spt最近的点
	distmin=100000;
	//	for(int i=Kmin ; i<=Kmax; i++)//遍历找交点
	for(int i=0 ; i<=trow2; i++)//遍历找交点
	{
		x1 = x2,y1= y2;
		if(i<trow2-1)
			x2 = array2[i+1][8],y2= array2[i+1][9];		   
		else 
			x2 = array2[trow2][8],y2= array2[trow2][9];


		//	 ads_printf(L"###i=%d lc=%lf x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",i,array2[i][6],x1,y1,x2,y2);
		//	 ads_printf(L"\n$$$$$%lf,%lf,%lf,%lf",Spt[0],Spt[1],Ept[0],Ept[1]);
		//i,Spt[0],Spt[1],Ept[0],Ept[1],x1,y1,x2,y2);


		int res = IfOnXY(Spt[0],Spt[1],Ept[0],Ept[1],x1,y1,x2,y2);

		if(res==1)//在线元上
		{
			inf = CalXYjd(Spt,Ept,array2,trow2,i,ResPt,ProjMl);
			//		 acutPrintf(L"inf=%d,ResPt[0]=%lf,ResPt[1]=%lf,ProjML=%lf\n",inf,ResPt[0],ResPt[1],ProjMl);
			if(inf==0)//找到
			{
				//计算投影里程
				dist=sqrt((Spt[0]-ResPt[0])*(Spt[0]-ResPt[0])+(Spt[1]-ResPt[1])*(Spt[1]-ResPt[1]));
				if(distmin>dist)
				{
					xmin=ResPt[0];
					ymin=ResPt[1];
					distmin=dist;
					ProjMlmin=ProjMl;
				}			 
			}
		}
	}
	if(distmin<100000)
	{
		ResPt[0]=xmin;
		ResPt[1]=ymin;
		return ProjMlmin;		
	}
	else return -1.0;//找不到交点

}
int BAS_DRAW_FUN_JYX::INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4)
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
int BAS_DRAW_FUN_JYX::Search_XY(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3])//定位pt1点所在线元
{
	//	double x1,y1,x2,y2,x3,y3,x4,y4,s1,s2,Alf1,Alf2,q;
	int i;
	// kk ;
	//	double LorR,
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

int BAS_DRAW_FUN_JYX::CalXYjd(double Spt[3],double Ept[3],double array2[4*MAXJDNUM][10],int trow2,int kk, double ResPt[2],double &ProjMl)
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
		Xo = array2[kk][8] + R*cos(array2[kk][5]+LorR*m_dHalfPI);
		Yo = array2[kk][9] + R*sin(array2[kk][5]+LorR*m_dHalfPI);//圆心

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
		//		double pt1[2],
		double pt2[2];
		// pt3[2],pt[2];
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
				if(zj>m_dHalfPI)
					s1 = s;
				else
					s2 = s;
			}
			else
			{
				if(fabs(zj)>m_dHalfPI)
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
int BAS_DRAW_FUN_JYX::IfOnXY(double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4)//是否与线元有交点
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
int BAS_DRAW_FUN_JYX::CalXjj(double ml1,double array1[4*MAXJDNUM][10],int tarray1,double array2[4*MAXJDNUM][10],int tarray2,double &xjj,double &projml,double resPt[2])
{
	//假设为右侧;
	xlpoint PZ;
	PZ.lc = ml1;
	xlpoint_pz(array1,tarray1,&PZ);
	//	acutPrintf(L"----------lc=%lf,x=%lf,y=%lf\n",PZ.lc,PZ.x,PZ.y);
	double fwj = PZ.a;
	double Pt1[3];
	Pt1[0]=PZ.x,Pt1[1]=PZ.y,Pt1[2]=PZ.a;//一线上的点

	//判断二线位于左侧还是右侧
	int LorR;


	LorR = Judge2XLorR(PZ,array2,tarray2);

	if(LorR==0)//二线与一线重合518
	{
		xjj = 0.0;

		projml = PROJ_ML(array2,tarray2,Pt1[0],Pt1[1],Pt1[0],Pt1[1]);//投影到2线

		resPt[0]=Pt1[0];resPt[1]=Pt1[1];

	}
	else
	{
		projml = Search_EXPt(array2,tarray2,Pt1,fwj + LorR * m_dHalfPI,resPt);//点在二线的投影连续里程几投影点

		xjj = 0.0;
		if(projml<-0.001)
		{
			projml = array2[tarray2][6];//如果没找到用终点里程
			return 0;//没找到
		}
		xjj = LorR*sqrt((Pt1[0]-resPt[0])*(Pt1[0]-resPt[0])+(Pt1[1]-resPt[1])*(Pt1[1]-resPt[1]));
		if(fabs(xjj)<0.001)
			xjj = 0.0;

	}
	return 1;
}

//----------------------------------------------------------------------------------
double BAS_DRAW_FUN_JYX::Get_QXJK(CString TLDJ,double design_V,double SR, GuiFanCanShu mGuiFanCS)
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
double BAS_DRAW_FUN_JYX::Get_Rshu(double design_V,double deltaI, GuiFanCanShu mGuiFanCS)
{

	double Rshu=0.0;

	/* 	if(fabs(design_V-70)<0.1 || fabs(design_V-55)<0.1)
	{
	if(deltaI>4.0)
	Rshu = 5000.0;
	}
	else if(fabs(design_V-40)<0.1)
	{
	if(deltaI>5.0)
	Rshu = 3000.0;
	}
	else if(fabs(design_V-140)<0.1||fabs(design_V-120)<0.1)
	{
	if(deltaI>3.0)
	Rshu = 10000.0;
	}
	else if(fabs(design_V-100)<0.1)
	{
	if(deltaI>4.0)
	Rshu = 5000.0;
	}
	else if(fabs(design_V-80)<0.1)
	{
	if(deltaI>3.0)
	Rshu = 10000.0;
	}
	else if(fabs(design_V-160)<0.1)
	{
	if(deltaI>0.99)
	Rshu = 15000.0;
	}
	else if(fabs(design_V-200)<0.1)
	{
	if(deltaI>0.99)
	Rshu = 15000.0;
	}
	else if(fabs(design_V-250)<0.1)
	{
	if(deltaI>0.99)
	Rshu = 20000.0;
	}
	else if(design_V>299)
	{
	if(deltaI>0.99)
	Rshu = 25000.0;
	}*/

	if(deltaI<=mGuiFanCS.ShuTiaoJ)
		Rshu=0.0;
	else
		Rshu= mGuiFanCS.ShuR;
	return Rshu;
}


double BAS_DRAW_FUN_JYX::PXJ(double fwj1, double fwj2)
{
	double jiao;	
	jiao=fwj2-fwj1;
	if(jiao<0)jiao+=2*pi;
	if(jiao>pi)jiao=2*pi-jiao;
	return fabs(jiao);
}


double BAS_DRAW_FUN_JYX::fwj(AcGePoint3d sPt,AcGePoint3d ePt)//与Y轴夹角
{
	double ang=angleX(sPt,ePt);//与X轴夹角
	ang=2.5*PI-ang;
	if(ang>2*PI)ang-=2*PI;
	return ang;
}



double BAS_DRAW_FUN_JYX::fwj(double x0,double y0,double x1,double y1)
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

double BAS_DRAW_FUN_JYX::AFA(double fwj1,double fwj2)
{
	double af;
	af=fwj2-fwj1;
	if(af<0)af+=2*pi;
	if(af>2*pi)af=2*pi-af;
	return af;
}

double BAS_DRAW_FUN_JYX::radiusfree(AcDbLine *Line1, AcDbLine *Line2,AcGePoint3d pk)
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
//plh08.20
double BAS_DRAW_FUN_JYX::GetRFromT1T2(double& L0,double &x1, double &y1, double &x0,double &y0,double &x2,double &y2, BOOL isJYGuiFanCS ,GuiFanCanShu& mGuiFanCS, bool IsStandard , double DV ,int INTRadius , int JUDGET012 , BOOL isPreMove )
	//给定一个交点，直始x1,y1,交点x0,y0,终止x2,y2,是否遵守半径系列，返回半径
	//int JUDGET012=0//不进行夹直线判断？int JUDGET012=1//与前面夹直线判断int JUDGET012=2//与后面夹直线判断int JUDGET012=12//与前后夹直线判断
{
	L0 =0.0;
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


	ads_point pt1,pt2; // pt,
	AcDbObjectId eId,eId1,arcId,LId1,LId2;
	//	AcDbEntity * pEnt;
	//	AcDbEntity * pEnt1;
	//	ACHAR mes[80];
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
	double l0 = 0.0;
	struct resbuf result;
	AcDbArc *pArc= new AcDbArc;//(center,rad,angle1,angle2);
	arcId=AddEntityToDbs(pArc);	
	/*acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
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


	AcDbLine *perLine3d1=new AcDbLine();
	perLine3d1->setColorIndex(3);
	AcDbObjectId perLId1=AddEntityToDbs(perLine3d1);

	AcDbLine *perLine3d2=new AcDbLine();
	perLine3d2->setColorIndex(3);
	AcDbObjectId perLId2=AddEntityToDbs(perLine3d2);
	bool ISOK=true;
	double tmpR=0.0;

	for(;;)
	{	

		if(ads_grread(1 , &type , &result)!=RTNORM)
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

			if(!isPreMove)
			{
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
			}
			else
			{

				if(GetCircleFromT_P_R(x0,y0,x2,y2,rad,cenX,cenY)!=true)continue;


				dx=cenX-center.x;
				dy=cenY-center.y;
				//			acutPrintf(L"dx=%lf,dy=%lf\n",dx,dy);
				x0+=dx;
				y0+=dy;
				//			x1+=dx;
				//			y1+=dy;
				x1+=dx;
				y1+=dy;
			}
			/*acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			/*acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
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

			// E=pk.distanceTo(interPt[0]);

			double tempr= radiusfree(pl1,pl2,pk);
			if(tempr>5.0)rad=tempr;

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
						l0=get_l0(rad,DV,isJYGuiFanCS,mGuiFanCS);
						mes.Format(L"\n请输入缓长<%0.2lf>",l0);	
						rc = ads_getreal(mes,&l0);
						if (rc==RTCAN)
						{
							L0 = l0;	
							return tempr;
						}
					}
				}

				else
				{
					tempr=get_R(rad,DV,isJYGuiFanCS,mGuiFanCS);
					if(tempr>5.0)rad=tempr;
					l0=get_l0(rad,DV,isJYGuiFanCS,mGuiFanCS);
				}


				double BPMTL[10];
				double f1=fwj(x1,y1,x0,y0);
				double f2=fwj(x0,y0,x2,y2);
				double aa=AFA(f1,f2);
				if (aa>pi)aa=2*pi-aa;
				getBPMTL(dms_rad(aa),rad,l0,l0,&BPMTL[0]);
				T1=BPMTL[6];
				T2=BPMTL[7];


				//acutPrintf(L"最小夹直线长为: T0min %.2lf,T1=%lf,Tmax=%lf\n",T0min,T1,Tmax);

				if(JUDGET012==0)
				{
					ISERROR=false;
				}
				else if(JUDGET012==120)
				{					
					//acutPrintf(L"当前夹直线长为: L1=%.2lf,L2=%.2lf(右键平移!)\n",Tmax1-T1+T0min,Tmax2-T2+T0min);
					//acutPrintf(L"Tmax1=%.2lf,Tmax2=%.2lf\n",Tmax1,Tmax2);
					if(T1>Tmax1||T2>Tmax2)ISERROR=true;
					else ISERROR=false;
				}
				else if(JUDGET012==12)
				{					
					acutPrintf(L"当前夹直线长为: L1=%.2lf,L2=%.2lf(右键平移!)\n",Tmax1-T1+T0min,Tmax2-T2+T0min);
					acutPrintf(L"Tmax1=%.2lf,Tmax2=%.2lf\n",Tmax1,Tmax2);
					if(T1>Tmax1||T2>Tmax2)ISERROR=true;
					else ISERROR=false;
				}
				else if(JUDGET012==1)
				{
					acutPrintf(L"当前夹直线长为: L1=%.2lf(右键平移!)\n",Tmax-T1+T0min);
					if(T1>Tmax)ISERROR=true;
					else ISERROR=false;
				}
				else if(JUDGET012==2)
				{
					acutPrintf(L"当前夹直线长为: L2=%.2lf(右键平移!)\n",Tmax-T2+T0min);
					if(T2>Tmax)ISERROR=true;
					else ISERROR=false;
				}

			}
			else if(INTRadius>0)rad=(int)(rad/INTRadius)*INTRadius;

			if(fabs(rad - tmpR)>0.001)
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

			/*acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
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
			/*acdbOpenObject(perLine3d1,perLId1,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(perLine3d1,perLId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			perLine3d1->setStartPoint(AcGePoint3d(perX,perY,0.0));
			perLine3d1->setEndPoint(center);
			perLine3d1->close();//切点与圆心连线				


			GetPerPoint(center.x,center.y,x2,y2,x0,y0,perX,perY);

			/*acdbOpenObject(perLine3d2,perLId2,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(perLine3d2,perLId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0.0;
			}
			perLine3d2->setStartPoint(AcGePoint3d(perX,perY,0.0));
			perLine3d2->setEndPoint(center);
			perLine3d2->close();//切点与圆心连线	
			if (type==2)
				break;
		}
	}

	//acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);
	if(acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}

	pArc->erase();
	pArc->close();
	/*	acdbOpenObject(pText,textId,AcDb::kForWrite,Adesk::kFalse);
	pText->erase();	
	pText->close();
	*/
	/*acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pl1,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	/*acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pl2,LId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}


	double jiao1,jiao2,dist1,dist2;
	jiao1=acutAngle(asDblArray(pl1->endPoint()),asDblArray(pl1->startPoint()));
	jiao2=acutAngle(asDblArray(pl2->startPoint()),asDblArray(pl2->endPoint()));
	dist1=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	dist2=sqrt((x2-x0)*(x2-x0)+(y2-y0)*(y2-y0));
	if(T1>dist1)
	{
		if(JUDGET012==120)
		{
			ads_printf(L"当前交点HZ/YZ超过后一交点ZH/ZY\n");
			rad = 100.0;
			l0 = 0.0;
		}
		else
		{
			dist1=1.2*T1;
			x1=x0+dist1*cos(jiao1);
			y1=y0+dist1*sin(jiao1);
		}
	}
	if(T2>dist2)
	{

		if(JUDGET012==120)
		{
			ads_printf(L"当前交点HZ/YZ超过后一交点ZH/ZY\n");
			rad = 100.0;
			l0 = 0.0;
		}
		else
		{
			dist2=1.2*T2;
			x2=x0+dist2*cos(jiao2);
			y2=y0+dist2*sin(jiao2);
		}
	}
	//	acutPrintf(L"jiao1=%lf,jiao2=%lf,x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",jiao1,jiao2,x1,y1,x2,y2);
	pl1->erase();	
	pl2->erase();	
	pl1->close();
	pl2->close();

	/*acdbOpenObject(perLine3d1,perLId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(perLine3d1,perLId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}

	perLine3d1->erase();
	perLine3d1->close();//切点与圆心连线				
	/*acdbOpenObject(perLine3d2,perLId2,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(perLine3d2,perLId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return 0.0;
	}
	perLine3d2->erase();
	perLine3d2->close();//切点与圆心连线				
	if(ISERROR && JUDGET012!=120)rad=0;		
	//	acutPrintf(L"返回半径:%.2lf\n",rad);
	//	if(ISERROR)return -1.0;
	acedGrText(-5,L"完成",0);    
	//	acutPrintf(L"\n命令:");
	resSHORTCUT.resval.rint=saveShortCutValue;
	acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);

	if(ISOK)
	{
		L0 = l0;
		return rad;
	}
	else return -1.0;
}

bool BAS_DRAW_FUN_JYX::GetCircleFromT_P_R(double x1,double y1,double x2,double y2,double R,double &cenX,double &cenY)
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

	//x1+=100.0*dx;
	//y1+=100.0*dy;
	//	ads_name en;//select line;
	//	ads_point pt;
	AcDbObjectId eId;
	//	AcDbEntity *pEnt;
	AcGePoint3d sPt0(x1,y1,0.0),ePt0(x2,y2,0.0);
	AcDbLine *pl1=new AcDbLine(sPt0,ePt0);
	AcGePoint2d sPt1(sPt0.x,sPt0.y),ePt1(ePt0.x,ePt0.y);
	AcGeLine2d pLine1(sPt1,ePt1);
	//constructe the circle.	
	AcDbCircle *pCircle1= new AcDbCircle();
	AcDbObjectId cirId1=AddEntityToDbs(pCircle1);
	AcDbCircle *pCircle2= new AcDbCircle();
	AcDbObjectId cirId2=AddEntityToDbs(pCircle2);
	/*acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pCircle1->setColorIndex(1);
	pCircle1->close();
	/*acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse);	*/
	if(acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
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

			/*acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return false;
			}
			pCircle1->erase();
			pCircle1->close();
			/*acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse);	*/
			if(acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return false;
			}
			pCircle2->erase();
			pCircle2->close();
			/*acdbOpenObject(perLine3d,perLId,AcDb::kForWrite,Adesk::kFalse);*/
			if(acdbOpenObject(perLine3d,perLId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return false;
			}
			perLine3d->erase();
			perLine3d->close();//切点与圆心连线		

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
					/*acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					pCircle1->setCenter(p31);
					pCircle1->setRadius(R);
					pCircle1->close();
					/*acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					pCircle2->setCenter(p31);
					pCircle2->setRadius(0);
					pCircle2->close();
					cenX=p31.x;
					cenY=p31.y;
				}
				else
				{
					/*acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
					if(acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					pCircle1->setCenter(p32);
					pCircle1->setRadius(0);
					pCircle1->close();
					/*	acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse);	*/
					if(acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					pCircle2->setCenter(p32);
					pCircle2->setRadius(R);
					pCircle2->close();
					cenX=p32.x;
					cenY=p32.y;
				}

				GetPerPoint(cenX,cenY,x1,y1,x2,y2,perX,perY);
				/*	acdbOpenObject(perLine3d,perLId,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(perLine3d,perLId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return false;
				}
				perLine3d->setStartPoint(AcGePoint3d(perX,perY,0.0));
				perLine3d->setEndPoint(AcGePoint3d(cenX,cenY,0.0));
				perLine3d->close();//切点与圆心连线	
			}
		}
	}//for(;;)


	/*acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(pCircle1,cirId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pCircle1->erase();
	pCircle1->close();
	/*acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse);	*/
	if(acdbOpenObject(pCircle2,cirId2,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pCircle2->erase();
	pCircle2->close();
	/*acdbOpenObject(perLine3d,perLId,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(perLine3d,perLId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	perLine3d->erase();
	perLine3d->close();//切点与圆心连线	
	return true;
}

void BAS_DRAW_FUN_JYX::GetPerPoint(double outX, double outY, double x1, double y1, double x2, double y2, double &perX, double &perY)
	//线外一点作垂线,1:线外一点  2:线起点 3:线终点 4:返回垂足
{
	AcDbLine *pLine=new AcDbLine(AcGePoint3d(x1,y1,0.0),AcGePoint3d(x2,y2,0.0));
	double fwj1=fwj(x1,y1,x2,y2);

	fwj1 += m_dHalfPI;
	AcDbLine *perpLine=new AcDbLine(AcGePoint3d(outX,outY,0.0),AcGePoint3d(outX+10*sin(fwj1),outY+10*cos(fwj1),0.0));


	AcGePoint3dArray interPt;
	pLine->intersectWith(perpLine,AcDb::kExtendBoth,interPt,0,0);


	//ads_printf(L"1N=%lf,1E=%lf,2N=%lf,2E=%lf,3N=%lf,3E=%lf,4N=%lf,4E=%lf\n",x1,y1,x2,y2,outX,outY,outX+10*sin(fwj1),outY+10*cos(fwj1));
	//ads_printf(L"x=%lf,y=%lf\n",interPt[0].x,interPt[0].y);


	perX=interPt[0].x;
	perY=interPt[0].y;


	//	delete perpLine ;
	//	delete pLine;//

}

//plh08.20
double BAS_DRAW_FUN_JYX::GetRFromT_P_P(double x1,double y1,double x2,double y2,
									   double &xs,double &ys,double &x0,double &y0,
									   double &xe,double &ye,double &cenX,double &cenY,
									   AcDbObjectIdArray &eraseId,bool isJYGuiFanCS ,GuiFanCanShu& mGuiFanCS, bool IsStandard, double DV)
									   //传入切线方向，返回半径,前一个切点，交点，后一个切点
{
	// TODO: Implement the command


	ads_printf(L"注意 1  ********************************  x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",x1,y1,x2,y2);

	struct resbuf resSHORTCUT;//设置右键功能
	resSHORTCUT.restype=RTSHORT;
	acedGetVar(L"SHORTCUTMENU",&resSHORTCUT);
	short saveShortCutValue=resSHORTCUT.resval.rint;
	resSHORTCUT.resval.rint=8;
	acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);

	double jiao12;
	double dist=xyddaa(x1,y1,x2,y2,&jiao12);

	ads_printf(L"dist=%lf,jiao12=%lf\n",dist,jiao12);

	//	dist*=1000;
	x2+=dist*cos(jiao12);
	y2+=dist*sin(jiao12);
	x1-=dist*cos(jiao12);
	y1-=dist*sin(jiao12);
	double rad=-1.0;//半径
	//	ACHAR mes[80];
	double pxj,fwj1,fwj2,fwjr,fwjt,jiao,afa;
	AcGePoint3d center;

	AcGePoint3d sPt1(x1,y1,0.0);//切线的起点
	AcGePoint3d ePt1(x2,y2,0.0);//切线的终点
	AcDbLine *pl1=new AcDbLine(sPt1,ePt1);

	double side1,side2;	
	double tmpR = 0.0;
	//select point.
	ads_point pk00;

	if(RTNORM!=acedGetPoint(NULL,L"请选择圆弧上第一个点:\n",pk00))
	{
		resSHORTCUT.resval.rint=saveShortCutValue;
		acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);
		return -1.0;
	}

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
			double cenX = 0.0 ,cenY = 0.0 ,dx = 0.0 ,dy = 0.0 ;	
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
				delete pLinet;///
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
				if(temprad<0.00001||temprad>1.2*get_R(temprad,DV,isJYGuiFanCS,mGuiFanCS))continue;
				rad=temprad;
				if(IsStandard)rad = get_R(rad,DV,isJYGuiFanCS,mGuiFanCS);

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

				/*acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(pArc,arcId,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return 0.0;
				}
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

				//	ads_printf(L"注意 3  ********************************  x1=%lf,y1=%lf,x2=%lf,y2=%lf\n",x1,y1,x2,y2);

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
	//	acutPrintf(L"\n命令:");

	resSHORTCUT.resval.rint=saveShortCutValue;
	acedSetVar(L"SHORTCUTMENU",&resSHORTCUT);

	if(ISOK)return rad;
	else return -1.0;
}

bool BAS_DRAW_FUN_JYX::GetTangent(double CenX, double CenY, double R, double &OutX, double &OutY, double &TX, double &TY)
	//传入圆心，半径，一条切线的，起点,终点//终点为切点
	//返回 另一第切线的起点，终点//终点为切点
{
	AcGePoint2d pk;//必经点
	int type;
	struct resbuf result;

	AcDbLine *Line1=new AcDbLine();
	AcDbObjectId LId1=AddEntityToDbs(Line1);
	AcGePoint3d Pt1,Pt2;
	//	double jiao;	//和方向
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
				/*acdbOpenObject(Line1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
				if(acdbOpenObject(Line1,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return false;
				}
				Line1->setStartPoint(AcGePoint3d(TX,TY,0.0));
				Line1->setEndPoint(AcGePoint3d(OutX,OutY,0.0));
				Line1->close();				
			}
		}
	}
	/*acdbOpenObject(Line1,LId1,AcDb::kForWrite,Adesk::kFalse);*/
	if(acdbOpenObject(Line1,LId1,AcDb::kForWrite,Adesk::kFalse)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	Line1->erase();
	Line1->close();
	return true;
}



void BAS_DRAW_FUN_JYX::getLineEndPt(AcGePoint3d sPt,double len,double fwj,AcGePoint3d &ePt)
{
	ePt.z=0;
	ePt.x=sPt.x+len*sin(fwj);
	ePt.y=sPt.y+len*cos(fwj);
}

void BAS_DRAW_FUN_JYX::GetT0min_Lrmin(double DV,double &T0min, double &Lrmin, GuiFanCanShu mGuiFanCS)
	//传入设计车速
{

	if(DV<=0){T0min=40.0;Lrmin=20.0;}
	else
	{
		T0min = mGuiFanCS.MinZhi;
		Lrmin = mGuiFanCS.MinYuan;
	}
	/*	else
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

	//   ads_printf(L"############%lf %lf\n",T0min,Lrmin);

}

double BAS_DRAW_FUN_JYX::DistToLine(double inX1, double inY1, double inX2, double inY2, double &outX,
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

	//	ads_printf(L"inSPt: F1  %lf %lf ept:%lf %lf outpt:%lf %lf perpPt:%lf %lf\n",inX1,inY1,inX2,inY2,outX,outY,perpPt.x,perpPt.y);

	//返回线上投影点
	outX=perpPt.x;
	outY=perpPt.y;

	//线外点到线段端点的投影距离的最小值
	distTo2PointOnLine=0.5*( perpPt.distanceTo(inSPt)+perpPt.distanceTo(inEPt)
		-inSPt.distanceTo(inEPt) );

	//	ads_printf(L"inSPt:  F2  %lf %lf ept:%lf %lf outpt:%lf %lf perpPt:%lf %lf\n",inX1,inY1,inX2,inY2,outX,outY,perpPt.x,perpPt.y);


	return outPt.distanceTo(perpPt);
}

//////////////////////////06.09///////////////
double BAS_DRAW_FUN_JYX::FwjtoAngX(double ang)
{
	ang=2.5*PI-ang;
	StdJiao(ang);
	return ang;
}

void BAS_DRAW_FUN_JYX::StdJiao(double &jiao)
{
	if(jiao<0.0)jiao+=2.0*PI;
	if(jiao>2.0*PI)jiao-=2.0*PI;
}

///////////////////////////////
//求三个交点的左右转，左-1，右+1
int BAS_DRAW_FUN_JYX::GetLeftOrRight3JD(double N1, double E1, double N0, double E0, double N2, double E2)
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

void BAS_DRAW_FUN_JYX::GetCenter3JD(double N1, double E1, double N0, double E0, double N2, double E2, double R, double Lo1, double Lo2, double &CenN, double &CenE)
{
	int c;
	double fb,fe;
	double BPMTL[10],T1; // ,T2,L
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
		double dr=x+R*sin(t + m_dHalfPI);			
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
	CenN=NHY+R*cos(fb+c*(t + m_dHalfPI));
	CenE=EHY+R*sin(fb+c*(t + m_dHalfPI));
}

//给定圆心半径，从圆到圆外一点的左右转信息，求出公切点
void BAS_DRAW_FUN_JYX::GetQD(double CenN, double CenE, double R,
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



bool BAS_DRAW_FUN_JYX::LoadArx(ACHAR ArxFname[256],ACHAR cmd[80])
{
	ACHAR cmdstr[80];

	acedGetAcadDwgView()->SetFocus();
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\%s",SysPath,ArxFname);

	acedArxLoad(fname);
	acDocManager->activateDocument(acDocManager->curDocument());
	_stprintf(cmdstr ,L"(command \"%s\")\n",cmd);
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	

	return 1;

}



//设置规范参数省缺值
int BAS_DRAW_FUN_JYX::SetDefultGuifan(GuiFanCanShu &guifan)
{
	//CString FilePath;
	//FilePath=SysPath;
	//FilePath=FilePath+"\\lib\\GuiFancs.dat";

	//int ThePosition = 0;
	//CFile guifanFile;
	////检查规范数据文件是否存在，否，则指定该文件位置。
	//if (guifanFile.Open(FilePath,CFile::modeRead,NULL)==0)
	//{//规范数据文件不在当前路径，打开文件失败，重指定
	//	FilePath="";
	//	CString WhatFile;
	//	DWORD dwFlags=OFN_FILEMUSTEXIST+OFN_HIDEREADONLY;
	//	CFileDialog cmFileopen(true,L"",L"",dwFlags,L"GuiFancs.dat|GuiFancs.dat",NULL);
	//	cmFileopen.DoModal();
	//	WhatFile=cmFileopen.GetPathName();
	//	if (WhatFile=="")//不指定则关闭“标准”对话框，返回主窗口
	//	{
	//		//	OnCancel();
	//	}
	//	else//指定了文件，设置工作路径和打开文件
	//	{
	//		FilePath=WhatFile;
	//		guifanFile.Open(FilePath,CFile::modeRead,NULL);
	//	}
	//}
	//if (FilePath!="")
	//{//检查文件内容正确性		  
	//	CString FileBiaoZhi;
	//	ACHAR lb[13];
	//	guifanFile.Read(lb,12);
	//	lb[12]=NULL;
	//	FileBiaoZhi=lb;
	//	if (FileBiaoZhi=="xl040301-PXB")
	//	{//文件检验通过，可以读数据
	//		//从数据库中读取速度目标加入速度列表
	//		unsigned int Lid=(guifanFile.GetLength()-12-sizeof(QuXianZheJiang))/sizeof(GuiFanCanShu);
	//		if (Lid > 0)
	//		{
	//			for (int i = Lid; i > 0; i--)
	//			{
	//				guifanFile.Seek(12+sizeof(QuXianZheJiang) + (i - 1) * sizeof(GuiFanCanShu),CFile::begin);
	//				guifanFile.Read(&guifan,sizeof(GuiFanCanShu));

	//				FileBiaoZhi.Format(L"%d",mGuiFanCS.SuDuMuBiao);

	//				if (mGuiFanCS.SuDuMuBiao == 120)
	//					ThePosition = i - 1;
	//			}

	//			guifanFile.m_hFile=guifanFile.m_hFile;

	//			guifanFile.Seek(12+sizeof(QuXianZheJiang) + ThePosition * sizeof(GuiFanCanShu),CFile::begin);
	//			guifanFile.Read(&guifan,sizeof(GuiFanCanShu));



	//		}
	//	}
	//}

	return 0;
}

int BAS_DRAW_FUN_JYX::FindWichDLFromFile(ACHAR GH[10],double XLC)
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

void BAS_DRAW_FUN_JYX::DrawLinePolyline(AcGePoint3dArray ptarr,int color,double wide)
{

	AcDb2dPolyline *pLine=new AcDb2dPolyline(AcDb::k2dSimplePoly,
		ptarr,0.0,Adesk::kFalse,wide,wide,
		NULL);
	pLine->setColorIndex(color);

	AcDbDatabase *pDataBase;    //声明指向数据库的指针
	AcDbBlockTable *pBlockTable; //声明指向块表的指针
	AcDbBlockTableRecord *pBlockTableRecord;//声明指向块表记录的指针

	pDataBase=acdbHostApplicationServices()->workingDatabase();
	pDataBase->getSymbolTable(pBlockTable,AcDb::kForRead);
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTableRecord->appendAcDbEntity(pLine); //将指向添加到块表记录

	pLine->close();	
	pBlockTableRecord->close(); //关闭块表记录
	pBlockTable->close(); //关闭块表


	//	 acdbHostApplicationServices()->workingDatabase()
	//		 ->setLineWeightDisplay(true);//打开线宽显示

	// pLine1->setLineWeight(AcDb::kLnWt040);//设置线宽为0.4mm

}

/*
//两端为不完整缓和曲线的切长计算，输入转角，圆曲线半径，第一缓和段起始半径，第一缓和段长，第二缓和段终止半径，第二缓和段长，求切长
void BAS_DRAW_FUN::get_T_Len(double zj,double R,double RB,double L1,double RE,double L2,double &T1,double &T2,double &L)
{ // BPTL[9]=B1,B2,P1,P2,M1,M2,T1,T2,L
//1计算出补齐后总转角
double Rh,A1,A2;
double lo1,lo2,hhqx_zj1,hhqx_zj2,ZjAll,Lall1,Lall2,Tall1,Tall2,ZongChang;
double hqzj1,hqzj2,arczj,arcL,Rmin,Rmax,lb,le;
zj = fabs(rad(zj));
if(RB<1e6 && L1>1e-6)
{
Rh = R*RB/fabs(RB-R);
A1 = sqrt(Rh*L1);
lb = A1*A1/RB;
le = A1*A1/R;
hqzj1 = fabs(0.5*lb/RB - 0.5*le/R);
}
else
{
A1 = sqrt(R*L1);
hhqx_zj1=0.0;
Lall1 = L1;
lo1=0.0;
hqzj1=0.5*L1/R;
}

if(RE<1e6 && L2>1e-6)
{
Rh = R*RE/fabs(RE-R);
A2 = sqrt(Rh*L2);
lb = A2*A2/RE;
le = A2*A2/R;
hqzj2 = fabs(0.5*lb/RE - 0.5*le/R);
}
else
{
A2 = sqrt(R*L2);
hhqx_zj2=0.0;
Lall2 = L2; 
lo2=0.0;
hqzj2=0.5*L2/R;
}


arczj = zj - hqzj1 - hqzj2;
arcL = R*arczj;

double t11,t12;
Get_HHQX_T1T2(A1,RB,R,t11,t12);
double t21,t22;
t21 = t22 = fabs(R*tan(0.5*arczj));
double t31,t32;
Get_HHQX_T1T2(A2,R,RE,t31,t32);
double T11,T12,T13,t;
T11=t11;
T12 = fabs((t12+t21)*sin(arczj)/sin(hqzj1+arczj));
t = fabs((t12+t21)*sin(hqzj1)/sin(hqzj1+arczj));
T13 = fabs((t+t22+t31)*sin(hqzj2)/sin(zj));
T1 = T11 + T12 + T13;


double T21,T22,T23;
T21=t32;
T22 = fabs((t31+t22)*sin(arczj)/sin(hqzj2+arczj));
t = fabs((t31+t22)*sin(hqzj2)/sin(hqzj2+arczj));
T23 = fabs((t+t21+t12)*sin(hqzj1)/sin(zj));
T2 = T21 + T22 + T23;
L = L1+L2+arcL;
}
*/

void BAS_DRAW_FUN_JYX::Get_HHQX_T1T2(double A,double RB,double RE,double &TB,double &TE)
{
	double LB,LE;
	if(A<1e-6)
	{
		TB=TE=0.0;
	}
	double Rmax,Rmin;
	Rmax = RB > RE ? RB : RE;
	Rmin = RB < RE ? RB : RE;
	LB = A*A/Rmax;
	LE = A*A/Rmin;
	double xb,xe,ab,ae,al,bl,cl;
	ab = 0.5*LB/Rmax;
	ae = 0.5*LE/Rmin;
	xb = spiral_x(A,LB) - spiral_y(A,LB)/tan(ab);
	xe = spiral_x(A,LE) - spiral_y(A,LE)/tan(ae);
	al = fabs(xe - xb);

	double Tmax,Tmin;
	bl =al*sin(ae)/sin(fabs(ae-ab));
	Tmax = fabs(bl - spiral_y(A,LB)/sin(ab));

	cl = al*sin(ab)/sin(fabs(ae-ab));
	Tmin = fabs(cl - spiral_y(A,LE)/sin(ae));
	if(fabs(RB-Rmax)<0.01)
	{
		TB = Tmax , TE=Tmin;
	}
	else
	{
		TB=Tmin,TE=Tmax;
	}
}

//两端为不完整缓和曲线的切长计算，输入转角，圆曲线半径，第一缓和段起始半径，第一缓和段长，第二缓和段终止半径，第二缓和段长，求切长
void BAS_DRAW_FUN_JYX::get_T_Len(double zj,double R,double RB,double L1,double RE,double L2,double &T1,double &T2,double &L)
{ // BPTL[9]=B1,B2,P1,P2,M1,M2,T1,T2,L
	//1计算出补齐后总转角
	double Rh,A1,A2;
	double lo1,lo2,hhqx_zj1,hhqx_zj2,Lall1,Lall2;

	//Tall1,ZongChang,ZjAll,Rmin,Tall2
	double hqzj1,hqzj2,arczj,arcL,lb,le;  // Rmax,
	zj = fabs(rad(zj));
	if(RB<1e6 && L1>1e-6)
	{
		Rh = R*RB/fabs(RB-R);
		A1 = sqrt(Rh*L1);
		lb = A1*A1/RB;
		le = A1*A1/R;
		hqzj1 = fabs(0.5*lb/RB - 0.5*le/R);
	}
	else
	{
		A1 = sqrt(R*L1);
		hhqx_zj1=0.0;
		Lall1 = L1;
		lo1=0.0;
		hqzj1=0.5*L1/R;
	}

	if(RE<1e6 && L2>1e-6)
	{
		Rh = R*RE/fabs(RE-R);
		A2 = sqrt(Rh*L2);
		lb = A2*A2/RE;
		le = A2*A2/R;
		hqzj2 = fabs(0.5*lb/RE - 0.5*le/R);
	}
	else
	{
		A2 = sqrt(R*L2);
		hhqx_zj2=0.0;
		Lall2 = L2; 
		lo2=0.0;
		hqzj2=0.5*L2/R;
	}


	arczj = zj - hqzj1 - hqzj2;
	arcL = R*arczj;

	double t11,t12;
	if(A1>0.0)
		Get_HHQX_T1T2(A1,RB,R,t11,t12);
	else
		t11=t12=0.0;
	double t21,t22;
	t21 = t22 = fabs(R*tan(0.5*arczj));
	double t31,t32;
	if(A2>0.0)
		Get_HHQX_T1T2(A2,R,RE,t31,t32);
	else
		t31=t32=0.0;
	double T11,T12,T13,t;
	T11=t11;
	T12 = fabs((t12+t21)*sin(arczj)/sin(hqzj1+arczj));
	t = fabs((t12+t21)*sin(hqzj1)/sin(hqzj1+arczj));
	T13 = fabs((t+t22+t31)*sin(hqzj2)/sin(zj));
	T1 = T11 + T12 + T13;


	double T21,T22,T23;
	T21=t32;
	T22 = fabs((t31+t22)*sin(arczj)/sin(hqzj2+arczj));
	t = fabs((t31+t22)*sin(hqzj2)/sin(hqzj2+arczj));
	T23 = fabs((t+t21+t12)*sin(hqzj1)/sin(zj));
	T2 = T21 + T22 + T23;
	L = L1+L2+arcL;
}

//aa-dms
double BAS_DRAW_FUN_JYX::Adjust_L_JDLuan(double aa,double R,double RB,double Lo1,double RE,double Lo2,double T,int qorh)
{
	double T1,T2,L;
	double l11,l12;

	int num=0;
	l11=0.01,l12=2*R;
	for(;;)
	{
		num++;
		if(fabs(l11-l12)<PRECISION)
			return 0;
		double l10=0.5*(l11+l12);
		////求圆心//////////////////////////////////////////////////////////////////////
		double tempT;
		if(qorh==-1)//调整前lo
			get_T_Len(aa,R,RB,l10,RE,Lo2,tempT,T2,L);
		else
			get_T_Len(aa,R,RB,Lo1,RE,l10,T1,tempT,L);

		if(fabs(tempT-T)<PRECISION)return l10;
		else
		{
			if(tempT<T)
				l11 = l10;
			else 
				l12 = l10;
		}
		if(num >1000)
			return 0;
	}
}

void BAS_DRAW_FUN_JYX::SortArray( CArray<double,double> &a)
{
	//排序,从小到大
	double Temp;
	for(int i=0;i<a.GetSize()-1;i++)
	{
		for(int j=a.GetSize()-1;j>=i+1;j--)
		{
			if(a[j]<a[j-1])
			{
				Temp = a[j];
				a[j] = a[j-1];
				a[j-1] = Temp;
			}
		}
	}
}



//----------------------纵断面-------------------------------

void BAS_DRAW_FUN_JYX::DrawRect(AcGePoint3d RightUp,AcGePoint3d LeftDownPt,int Color,double wide)//绘矩形
{
	AcGePoint3d RightDownPt(RightUp.x,LeftDownPt.y,0.0);
	AcGePoint3d LeftUpPt(LeftDownPt.x,RightUp.y,0.0);
	AcGePoint3dArray ptarr;
	ptarr.setLogicalLength(5);
	ptarr[0].set(LeftDownPt.x,LeftDownPt.y,0.0);
	ptarr[1].set(RightDownPt.x,RightDownPt.y,0.0);
	ptarr[2].set(RightUp.x,RightUp.y,0.0);
	ptarr[3].set(LeftUpPt.x,LeftUpPt.y,0.0);
	ptarr[4].set(LeftDownPt.x,LeftDownPt.y,0.0);


	AcDb2dPolyline *pLine=new AcDb2dPolyline(AcDb::k2dSimplePoly,
		ptarr,0.0,Adesk::kFalse,wide,wide,
		NULL);
	pLine->setColorIndex(Color);

	AcDbDatabase *pDataBase;    //声明指向数据库的指针
	AcDbBlockTable *pBlockTable; //声明指向块表的指针
	AcDbBlockTableRecord *pBlockTableRecord;//声明指向块表记录的指针

	pDataBase=acdbHostApplicationServices()->workingDatabase();
	pDataBase->getSymbolTable(pBlockTable,AcDb::kForRead);
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTableRecord->appendAcDbEntity(pLine); //将指向添加到块表记录

	pLine->close();	
	pBlockTableRecord->close(); //关闭块表记录
	pBlockTable->close(); //关闭块表


	//	 acdbHostApplicationServices()->workingDatabase()
	//		 ->setLineWeightDisplay(true);//打开线宽显示

	// pLine1->setLineWeight(AcDb::kLnWt040);//设置线宽为0.4mm


}
//void BAS_DRAW_FUN::DrawLinePolyline(AcGePoint3dArray ptarr,int color,double wide)
//{
//
//	AcDb2dPolyline *pLine=new AcDb2dPolyline(AcDb::k2dSimplePoly,
//		ptarr,0.0,Adesk::kFalse,wide,wide,
//		NULL);
//	pLine->setColorIndex(color);
//
//	AcDbDatabase *pDataBase;    //声明指向数据库的指针
//	AcDbBlockTable *pBlockTable; //声明指向块表的指针
//	AcDbBlockTableRecord *pBlockTableRecord;//声明指向块表记录的指针
//
//	pDataBase=acdbHostApplicationServices()->workingDatabase();
//	pDataBase->getSymbolTable(pBlockTable,AcDb::kForRead);
//	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
//	pBlockTableRecord->appendAcDbEntity(pLine); //将指向添加到块表记录
//
//	pLine->close();	
//	pBlockTableRecord->close(); //关闭块表记录
//	pBlockTable->close(); //关闭块表
//
//
//	//	 acdbHostApplicationServices()->workingDatabase()
//	//		 ->setLineWeightDisplay(true);//打开线宽显示
//
//	// pLine1->setLineWeight(AcDb::kLnWt040);//设置线宽为0.4mm
//
//}




////任意点坐标法计算拨量所用函数

/*************************************************************
函数名称: DisAndAngleByX   
函数功能、性能等的描述: 计算两点之间的距离dDis和两点构成的直线与x的夹角dAngleByX
被本函数调用的函数清单: 无
调用本函数的函数清单: CHhqx::set,SetByEnd,GetPoint,GetPoint
CYYLuanLink::Set,L0toCC,
CYYCSLink::Set
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dXstart    起点的x坐标；
dYstart	   起点的y坐标；
dXend	   终点的x坐标;	
dYend	   终点的y坐标;
对输出参数的说明:  dAngleByX 为该条直线与X轴夹角
函数返回值的说明:  返回该直线的长度      
其它说明:  以大地N坐标代替X，大地E坐标代替Y，dAngleByX是坐标方位角。  
***************************************************************/
double BAS_DRAW_FUN_JYX::DisAndAngleByX(double &dAngleByX,double dXstart,double dYstart,double dXend,double dYend)
{	
	double dx=0.0,dy=0.0,dis=0.0,aa=0.0;
	dx = dXend - dXstart;
	dy = dYend - dYstart;
	dis = sqrt(dx*dx+dy*dy);
	if (fabs(dx)<PRECISION) 
	{
		if (dy>=0.0) 
			aa=pi*0.5;
		else
			aa=pi*1.5;
	}
	else 
		aa=atan(dy/dx);

	if (dy>=0 && dx<-PRECISION) 
		aa=aa+pi;
	else if (dy<0 && dx<-PRECISION) 
		aa=aa+pi;
	else if (dy<0 && dx>PRECISION) 
		aa=aa+2*pi;
	if (aa<-PRECISION) 
		aa=aa+2*pi;
	else if (aa>2*pi) 
		aa=aa-2*pi;
	dAngleByX=aa;
	return(dis);
}
/*************************************************************
函数名称: FormFwj   
函数功能、性能等的描述: 把计算出来的角度转换为0-2pi范围内的方位角
被本函数调用的函数清单: 无
调用本函数的函数清单: CHhqx::set,SetByEnd；
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      dFWj为前面算出来的角度
对输出参数的说明:  dFwj为转化后的方位角
函数返回值的说明:      
其它说明:     
***************************************************************/
void BAS_DRAW_FUN_JYX::FormFwj(double& dFwj) 
{
	while(dFwj < 0.0)
	{
		dFwj += 2*pi;
	}
	if(dFwj>2*pi)
	{
		dFwj = fmod(dFwj, 2*pi);
	}
}

/*************************************************************
函数名称: ZxAngle   
函数功能、性能等的描述: 求出两有向直线dLineAngle1到dLineAngle2的转向角
被本函数调用的函数清单: 无
调用本函数的函数清单:	CHhqx::GetAfa
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      dLineAngle1  第一条有向直线的坐标方位角 
dLineAngle2  第二条有向直线的坐标方位角 
对输出参数的说明:  无
函数返回值的说明:  返回dLineAngle1到dLineAngle2的转向角  
其它说明:  转向角顺时针方向为正     
***************************************************************/
double BAS_DRAW_FUN_JYX::ZxAngle(double dLineAngle2,double dLineAngle1)
{
	double dAngle=0.0;
	dAngle = dLineAngle2 - dLineAngle1;
	if(fabs(dAngle)>pi)
	{
		if(dAngle<0.0) //偏角小于-PI
		{
			dAngle = 2.0*pi + dAngle;
		}
		else      //偏角大于PI
		{
			dAngle = dAngle - 2.0*pi;
		}
	}
	return dAngle;
}

/*************************************************************
函数名称: DistanceFromPttoLine   
函数功能、性能等的描述: 求点到直线距离；
被本函数调用的函数清单: 无
调用本函数的函数清单:	CHhqx::GetJd1ZxHhqx，GetJdZxHhqx，LtoDeta;
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      
dXout      直线外点的坐标；
dYout      直线外点的坐标；
dXstart    直线起点的X坐标；
dYstart	   直线起点的Y坐标；
dXend      直线终点的X坐标；
dYend      直线终点的Y坐标；
对输出参数的说明:  无
函数返回值的说明:  返回(dXout,dYout)到直线的距离；
其它说明:		   特别要注意的是该函数的返回值并不是点到直线的距离，
返回值有正负号以反映点在直线的不同侧，绝对值才是点到直线的距离；
如果(dXout,dYout)就在直线上也可以计算，返回值为一个很小的值   
***************************************************************/

double BAS_DRAW_FUN_JYX::DistanceFromPttoLine(double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend)
{
	double a=0.0,b=0.0,c=0.0,d=0.0,absq=0.0,sr=0.0;
	if (fabs(dXend-dXstart)<PRECISION)
		d=dXout-(dXstart+dXend)/2;

	else if(fabs(dYend-dYstart)<PRECISION)
		d=dYout-(dYstart+dYend)/2;

	else
	{
		a=(dYend-dYstart)/(dXend-dXstart);
		b=-1;
		c=dYstart-dXstart*(dYend-dYstart)/(dXend-dXstart);
		absq=sqrt(a*a+b*b);
		sr=a*dXout+b*dYout+c;
		d=sr/absq;
	}	
	return d;
}

/*************************************************************
函数名称: ChuizutoZx   
函数功能、性能等的描述: 从一点向直线作垂线求垂足；
被本函数调用的函数清单: 无
调用本函数的函数清单:	CHhqx::GoldenQj;
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      
dXout      直线外点的X坐标；
dYout      直线外点的Y坐标；
dXstart    直线起点的X坐标；
dYstart	   直线起点的Y坐标；
dXend      直线终点的X坐标；
dYend      直线终点的Y坐标；
对输出参数的说明:  
dXchuizu   垂足点的X坐标；
dYchuizu   垂足点的Y坐标；
函数返回值的说明:  无返回值
其它说明:  无
***************************************************************/

void BAS_DRAW_FUN_JYX::ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend)
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

/*************************************************************
函数名称: CCW2   
函数功能、性能等的描述: 检测给定的三个点的位置是否按逆时针顺序排列；
被本函数调用的函数清单: 无
调用本函数的函数清单:	CHhqx::GoldenQj;
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      
dpt1x      第一个点的X坐标；
dpt1y      第一个点的y坐标；
dpt2x	   第二个点的X坐标；
dpt2y	   第二个点的y坐标；
dpt3x      第三个点的X坐标；
dpt3y      第三个点的y坐标；
对输出参数的说明:  无
函数返回值的说明:  返回值为正时这三点为逆时针，为负时为顺时针
其它说明:  合理的给定这三个点该函数就可以用于判定一个点在一条直线的哪一侧，
GoldenQj调用该函数就是使用了它的这个功能
***************************************************************/

double BAS_DRAW_FUN_JYX::CCW2(double pt1[2],double pt2[2],double pt3[2]) 
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

double BAS_DRAW_FUN_JYX::CCW2(double dPt1X,double dPt1Y,double dPt2X,double dPt2Y,double dPt3X,double dPt3Y) 
{
	double x1,x2,x3,y1,y2,y3,bx,by;

	double Val = 0.0;
	bx = (dPt1X + dPt2X + dPt3X) /3.0;
	by = (dPt1Y + dPt2Y + dPt3Y) /3.0;
	x1 = dPt1X - bx;
	x2 = dPt2X - bx;
	x3 = dPt3X - bx;
	y1 = dPt1Y - by;
	y2 = dPt2Y - by;
	y3 = dPt3Y - by;
	Val = (x1*(y2-y3)-x2*(y1-y3)+x3*(y1-y2));
	if (Val<0.0)
		return -1.0;
	else
		return 1.0;
}


/*************************************************************
函数名称: CheckIfOnArc  
函数功能、性能等的描述: 检查点(dNout,dEout)是否在给定的圆弧上  
被本函数调用的函数清单: CBasicFun::DisAndAngleByX
调用本函数的函数清单:	CHhqx::GetJdArcHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dNcircle		    圆弧的圆心N坐标;
dEcircle			圆弧的圆心E坐标;
dAngleStart			圆弧起点的角度;
dAngleEnd			圆弧终点的角度;
dNout				待判断点的N坐标;
dEout				待判断点的E坐标;
对输出参数的说明:	无
函数返回值的说明: 该点在圆弧上则返回ture否则返回false;
其它说明:  
注意：该函数判断点是否在圆弧上是以该点与圆弧圆心的距离为半径的条件下进行的。
圆弧被认为是从起点开始按逆时针方向到终点；
圆弧起点和终点的角度是以圆心为坐标原点建立的XoY坐标系下起点、终点与圆心的连线的角度值,逆时针方向为正。
对于这些方向和起终点的定义是与CAD中的定义一致的
***************************************************************/
bool BAS_DRAW_FUN_JYX::CheckIfOnArc(double dNcircle,double dEcircle,double dAngleStart,double dAngleEnd,double dNout,double dEout)
{
	double Angle=0.0;
	DisAndAngleByX(Angle,dEcircle,dNcircle,dEout,dNout);
	if (dAngleStart<dAngleEnd)
	{
		if (Angle<dAngleEnd+1e-11 && Angle>dAngleStart-1e-11)
			return true;
		else
			return false;
	}
	else if (dAngleStart>dAngleEnd)
	{
		if (Angle>dAngleEnd-1e-11 && Angle<dAngleStart+1e-11)
			return false;
		else
			return true;
	}
	else//起点角度等于终点角度,该圆弧是一个圆
		return true;	
}

//输入点N，E,线路线元数据，返回点在线路左侧 -1 右侧 1
int BAS_DRAW_FUN_JYX::JudgePTLorR(double N,double E,double &dist,double array[4*MAXJDNUM][10],int tarray)
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


/*************************************************************
函数名称: Judge2XLorR
函数功能、性能等的描述: 判断二线在基线的左右侧
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明: 
PZ		基线上的点
array2  二线线元数组
tarray2 二线线元个数
对输出参数的说明: 无
函数返回值的说明:  -1-左侧  1-右侧 0-重叠
其它说明: 
***************************************************************/
int BAS_DRAW_FUN_JYX::Judge2XLorR(xlpoint PZ,double array2[4*MAXJDNUM][10],int tarray2)
{
	double Pt1[2],fwj;
	double N,E;

	fwj = PZ.a;
	Pt1[0] = PZ.x,Pt1[1] = PZ.y;

	double ml2 = PROJ_ML(array2,tarray2,Pt1[0],Pt1[1],Pt1[0],Pt1[1]);//投影到2线

	PZ.lc = ml2;
	xlpoint_pz(array2,tarray2,&PZ);

	double dist = sqrt((Pt1[0]-PZ.x)*(Pt1[0]-PZ.x)+(Pt1[1]-PZ.y)*(Pt1[1]-PZ.y));//两个投影点距离

	if(dist<0.001)//二线与一线重合
		return 0;
	N=Pt1[0],E=Pt1[1];

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
		LorR = -1;//点在右侧
	else
		LorR = 1;//点在左侧

	//	if(fabs(cml-189990.35)<0.1)
	//		ads_printf(L"%lf x1=%lf y1=%lf x2=%lf y2=%lf fwj1=%lf fwj2=%lf\n",dist,Pt1[0],Pt1[1],PZ.x,PZ.y,fwj,fwj1to2);
	return LorR;
}


////任意点坐标法计算拨量所用函数


CString BAS_DRAW_FUN_JYX::ReverseFind(CString InputCstring)
{
	int CurDataNum = 0 ,CurNum = 0;
	CString OutCsting;

	CurDataNum = InputCstring.ReverseFind('DATA');

	CurNum = InputCstring.ReverseFind('\\');

	if(abs(CurDataNum-CurNum)>1)AfxMessageBox(L"所选数据库不在指定文件夹下!");
	OutCsting = InputCstring.Right(InputCstring.GetAllocLength() - CurNum - 1);

	return OutCsting;
}

/*******************************************************************************
函数功能：两直线是否有重叠部分
返回值：true-重叠 false-不重叠
输入参数：
SPt1X，SPt1Y		第一条直线的起点X，Y坐标
EPt1X，EPt1X		第一条直线的终点X，Y坐标
SPt2X，SPt2Y		第二条直线的起点X，Y坐标
EPt2X，EPt2Y		第二条直线的终点X，Y坐标
********************************************************************************/
bool BAS_DRAW_FUN_JYX::IsZXCD(double SPt1X,double SPt1Y,double EPt1X,double EPt1Y,
							  double SPt2X,double SPt2Y,double EPt2X,double EPt2Y)
{
	bool IsCD = false;//初始化为不重叠
	double Dist1,Dist2;
	Dist1 = DistanceFromPttoLine(SPt2X,SPt2Y,SPt1X,SPt1Y,EPt1X,EPt1Y);//第一条直线起点到第二条直线的距离
	Dist2 = DistanceFromPttoLine(EPt2X,EPt2Y,SPt1X,SPt1Y,EPt1X,EPt1Y);//第一条直线终点到第二条直线的距离
	double Min1X,Min1Y,Max1X,Max1Y;
	double Min2X,Min2Y,Max2X,Max2Y;
	double MaxX,MaxY,MinX,MinY;
	Min1X = SPt1X < EPt1X ? SPt1X : EPt1X ;//第一条直线X坐标的最小值
	Max1X = SPt1X > EPt1X ? SPt1X : EPt1X ;//第一条直线X坐标的最大值
	Min1Y = SPt1Y < EPt1Y ? SPt1Y : EPt1Y ;//第一条直线Y坐标的最小值
	Max1Y = SPt1Y > EPt1Y ? SPt1Y : EPt1Y ;//第一条直线Y坐标的最大值

	Min2X = SPt2X < EPt2X ? SPt2X : EPt2X ;//第二条直线X坐标的最小值
	Max2X = SPt2X > EPt2X ? SPt2X : EPt2X ;//第二条直线X坐标的最大值
	Min2Y = SPt2Y < EPt2Y ? SPt2Y : EPt2Y ;//第二条直线Y坐标的最小值
	Max2Y = SPt2Y > EPt2Y ? SPt2Y : EPt2Y ;//第二条直线Y坐标的最大值

	MinX = Min1X > Min2X ? Min1X : Min2X ;//两直线交集X坐标的下限
	MaxX = Max1X < Max2X ? Max1X : Max2X ;//两直线交集X坐标的上限
	MinY = Min1Y > Min2Y ? Min1Y : Min2Y ;//两直线交集Y坐标的下限
	MaxY = Max1Y < Max2Y ? Max1Y : Max2Y ;//两直线交集Y坐标的上限

	//zhuyi//
	if (fabs(Dist1) < 0.2 && fabs(Dist2) < 0.2 )
	{
		if (MinX <  MaxX + 1e-3 && MinY <  MaxY + 1e-3)
			IsCD = true;
	}

	return IsCD;
}



int BAS_DRAW_FUN_JYX::GetBeforeNum(int i  , CArray<JDarrayStruct , JDarrayStruct >& JDNumArray)
{
	int GetBeforeNum = 0;
	ACHAR CurJDNum[20];
	_tcscpy( CurJDNum , JDNumArray[i].JDNum );
	for(int k=0;k<=i;k++)
	{
		if(_tcscmp(JDNumArray[k].JDNum , CurJDNum)==0)
		{
			GetBeforeNum++;
		}
	}

	return GetBeforeNum;
}

////////////////////////////////////////////////////////////////////////
//int BAS_DRAW_FUN::xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2, DLBZ DLBtoGT ,int NDLtoGT ,
//							 CArray<JDarrayStruct,JDarrayStruct>& JDarraytoGT ,
//                             struct xlpoint *PZ , GTZX *ZuoXianpGtzxToIn ,
//	                GTZX * JiXianpGtzxToIn , GTZX * YouXianpGtzxToIn , GTZX * GTXGtzxToIn ,
//	CArray<GJDAllInforStruct,GJDAllInforStruct>& BasicSectionInforToIn )

/*

int BAS_DRAW_FUN::xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ ,  GTZX *ZuoXianpGtzxToIn ,
GTZX * JiXianpGtzxToIn , GTZX * YouXianpGtzxToIn ,CArray<GJDAllInforStruct,GJDAllInforStruct>& BasicSectionInforToIn)				
{

/*
double sml , eml ;
double CurN , CurE;
int flag;
struct xlpoint CurPZ;
double Curcml; //投影里程
ACHAR Curkml[50]; //投影点现场里程

//对改建段数组循环
for(int i=0;i<BasicSectionInforToIn.GetSize();i++)
{
//段落起点
CurN = BasicSectionInforToIn[i].BpointToN ; 
CurE = BasicSectionInforToIn[i].BpointToE ; 
sml = BAS_DRAW_FUN::PROJ_ML(array2,trow2,CurN,CurE,CurN,CurE);

//段落终点
CurN = BasicSectionInforToIn[i].EpointToN ; 
CurE = BasicSectionInforToIn[i].EpointToE ; 
eml = BAS_DRAW_FUN::PROJ_ML(array2,trow2,CurN,CurE,CurN,CurE);

//起点/终点里程之外
if(PZ->lc < sml - 0.001)return 0;
if(PZ->lc > eml + 0.001)return 0;
if( PZ->lc>sml-0.001 && PZ->lc<eml+0.001)//在区段内，跳出循环，
{

if(BasicSectionInforToIn[i].GJDType==-1)//完全利用原有
{
CurPZ.lc = PZ->lc;
xlpoint_pz(array2, trow2, &CurPZ );  //实现里程转坐标
PZ->x = CurPZ.x;
PZ->y = CurPZ.y;

return 1;

}
else if(BasicSectionInforToIn[i].GJDType==0)//并行
{

double CalN,CalE;
//1.求现场里程
_tcscpy(Curkml , XLC(PZ->lc));

//2.求同一现场里程对应既有线上坐标
if(BasicSectionInforToIn[i].BXLName=="既有左线") //既有左线
{
//同一个现场里程，在既有线上求对应坐标
Curcml = ZuoXianpGtzxToIn->TYLC1(Curkml,ZuoXianpGtzxToIn->NDL , ZuoXianpGtzxToIn->DLB);

CurPZ.lc = Curcml ;
// 得到既有线上坐标
ZuoXianpGtzxToIn->xlpoint_pz(ZuoXianpGtzxToIn->array,ZuoXianpGtzxToIn->tarray,&CurPZ);

}
else if(BasicSectionInforToIn[i].BXLName=="既有基线") //基线
{
Curcml = JiXianpGtzxToIn->TYLC1(Curkml,JiXianpGtzxToIn->NDL , JiXianpGtzxToIn->DLB);

CurPZ.lc = Curcml ;
// 得到既有线上坐标
JiXianpGtzxToIn->xlpoint_pz(JiXianpGtzxToIn->array,JiXianpGtzxToIn->tarray,&CurPZ);


}
else if(BasicSectionInforToIn[i].BXLName=="既有右线") //既有右线
{
Curcml = YouXianpGtzxToIn->TYLC1(Curkml,YouXianpGtzxToIn->NDL , YouXianpGtzxToIn->DLB);

CurPZ.lc = Curcml ;
// 得到既有线上坐标
YouXianpGtzxToIn->xlpoint_pz(YouXianpGtzxToIn->array,YouXianpGtzxToIn->tarray,&CurPZ);
}

//再投影到贯通线

CalN = CurPZ.x ;
CalE = CurPZ.y ;

Curcml = PROJ_ML(array2,trow2,CalN,CalE,CalN,CalE);
PZ->lc = Curcml;
xlpoint_pz(array2,trow2,PZ);

return 1;

}
else  //绕行改建／S弯
{
//根据ＮＥ重新构造子ＧＴＺＸ
CArray<JDarrayStruct,JDarrayStruct> CurSubJDarray;
int SNum , ENum; //交点表，断链表 起点／终点下标
SNum = DistToNearJD(sml,true);
ENum = DistToNearJD(eml,false);
CurSubJDarray.RemoveAll();
for(int i=SNum;i<=ENum;i++)
{
CurSubJDarray.Add(JDarraytoGT[i]);
}

CArray<DLBZ,DLBZ> CurSubDLBarray;

double SDLcml,EDLcml;

//对断链区段进行循环
for(i=0;i<NDLtoGT-1;i++)
{
SDLcml = TYLC1(DLBtoGT[i].BLC ,NDLtoGT,DLBtoGT);
EDLcml = TYLC1(DLBtoGT[i+1].BLC ,NDLtoGT,DLBtoGT);

//断链区间起点下标
if(sml>SDLcml-0.0001 && sml<EDLcml+0.0001)SNum = i+1;
//断链区间终点下标
if(eml>SDLcml-0.0001 && eml<EDLcml+0.0001)ENum = i ;
}

CurSubDLBarray.RemoveAll();
//得到对应子断链表
for(i=SNum;i<=ENum ;i++)
{
CurSubDLBarray.Add(DLBtoGT[i]);
}

ACHAR JDGH[8],GH[8];

CString CurKmlString;

//现场里程
CurKmlString = XLC(PZ->lc);
_tcscpy(Curkml,CurKmlString);

_tcscpy(JDGH,L"JD");
_tcscpy(GH,L"AK"); 
GTZX *pSubLinep= new GTZX(CurSubJDarray.GetSize(),CurSubJDarray,&GH[0],&JDGH[0]);

for(i=0;i<CurSubDLBarray.GetSize();i++)
{
pSubLinep->setDLM(i,CurSubDLBarray[i].BLC,CurSubDLBarray[i].DL,
CurSubDLBarray[i].BGH,CurSubDLBarray[i].EGH);
}

//计算子ＧＴＺＸ中ＮＥ坐标

Curcml = pSubLinep->TYLC1(Curkml,pSubLinep->NDL,pSubLinep->DLB);

CurPZ.lc = Curcml;
pSubLinep->xlpoint_pz(pSubLinep->array,pSubLinep->tarray,&CurPZ);

PZ->x = CurPZ.x;
PZ->y = CurPZ.y;

}

}

}


return 1;
}
*/
//
//int BAS_DRAW_FUN::xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ ,  GTZX *ZuoXianpGtzxToIn , GTZX * JiXianpGtzxToIn , GTZX * YouXianpGtzxToIn , CArray<GJDInf,GJDInf>& GJDInfTabArrayToIn) 
//{
//
//	return 0;
//}

AcDbObjectId BAS_DRAW_FUN_JYX::H_maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR* textStyle,AcDbObjectId layerId)
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
	pText->setLayer(layerId);
	TId=AddEntityToDbs(pText);
	return TId;     
}


void BAS_DRAW_FUN_JYX::fit(double x[], double y[], int ndata, double sig[], int mwt, double& a,
						   double& b, double& siga, double& sigb, double& chi2, double& q)
{
	int i;
	double sigdat,t,sxoss,wt,ss,sx = 0.0;
	double sy = 0.0;
	double st2 = 0.0;
	b = 0.0;
	if (mwt != 0)
	{
		ss = 0.0;
		for (i = 1; i<=ndata; i++)
		{
			wt = 1.0 / (sig[i] * sig[i]);
			ss = ss + wt;
			sx = sx + x[i] * wt;
			sy = sy + y[i] * wt;
		}
	}
	else
	{
		for (i = 1; i<=ndata; i++)
		{
			sx = sx + x[i];
			sy = sy + y[i];
		}
		ss = ndata;
	}
	sxoss = sx / ss;
	if (mwt != 0)
	{
		for (i = 1; i<=ndata; i++)
		{
			t = (x[i] - sxoss) / sig[i];
			st2 = st2 + t * t;
			b = b + t * y[i] / sig[i];
		}
	}
	else
	{
		for (i = 1; i<=ndata; i++)
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
		for (i = 1; i<=ndata; i++)
		{
			chi2 = chi2 + pow((y[i] - a - b * x[i]) , 2);
		}
		q = 1.0;
		sigdat = sqrt(chi2 / (ndata - 2));
		siga = siga * sigdat;
		sigb = sigb * sigdat;
	}
	else
	{
		for (i = 1; i<=ndata; i++)
		{
			chi2 = chi2 + pow(((y[i] - a - b * x[i]) / sig[i]) , 2);
		}
		q = gammq(0.5 * (ndata - 2), 0.5 * chi2);
	}
}

double BAS_DRAW_FUN_JYX::gammq(double a, double x)
{
	double temp,gamser,gammcf,gln;
	if (x < 0.0 || a <= 0.0)
	{
		// cout<< "pause";
		exit(1);
	}
	if (x < a + 1.0)
	{
		gser(gamser, a, x, gln);
		temp= 1.0 - gamser;
	}
	else
	{
		gcf(gammcf, a, x, gln);
		temp = gammcf;
	}
	return temp;   
}

void BAS_DRAW_FUN_JYX::gser(double& gamser, double& a, double& x, double& gln)
{
	int itmax,n;
	double ap,sum,del,eps;
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
	for( n = 1;n<=itmax;n++)
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

void BAS_DRAW_FUN_JYX::gcf(double& gammcf, double& a, double& x, double& gln)
{
	int itmax,n;
	double eps,a0,a1,b0,b1,fac,an,ana,anf,gold,g;
	itmax = 100;
	eps = 0.0000003;
	gln = gammln(a);
	gold = 0.0;
	a0 = 1.0;
	a1 = x;
	b0 = 0.0;
	b1 = 1.0;
	fac = 1.0;
	for (n = 1;n<=itmax;n++)
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
			if (fabs((g - gold) / g) < eps )
			{
				gammcf = exp(-x + a * log(x) - gln) * g;
				break;
			}
			gold = g;
		}
	}
}

double BAS_DRAW_FUN_JYX::gammln(double xx)
{
	int j;
	double temp;
	double cof[6],stp,half,one,fpf,x,tmp,ser;
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
	for (j = 1;j<=6;j++)
	{
		x = x + one;
		ser = ser + cof[j] / x;
	}
	temp = tmp + log(stp * ser);
	return temp;
}

//标注距离
void BAS_DRAW_FUN_JYX::NoteDist(AcGePoint3d Spt,AcGePoint3d Ept,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,AcDbObjectId LayerId)
{
	//绘线
	//makeline(Spt,Ept,iColor);
	AcDbLine *pLine	= new AcDbLine(Spt,Ept);
	pLine->setColorIndex(iColor) ;
	pLine->setLayer(LayerId);
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	//绘箭头
	AcGePoint3dArray ptarray;
	AcGePoint3d pt;
	pt.x = Spt.x + (Ept.x-Spt.x)/15.0;
	pt.y = Spt.y + (Ept.y-Spt.y)/15.0;
	pt.z =0.0;
	ptarray.append(pt);
	ptarray.append(Spt);
	make2dline(ptarray,iColor,0.05*sfactor,0.0,LayerId);
	ptarray.removeSubArray(0,1);//删除
	pt.x = Ept.x - (Ept.x-Spt.x)/15.0;
	pt.y = Ept.y - (Ept.y-Spt.y)/15.0;
	pt.z =0.0;
	ptarray.append(pt);
	ptarray.append(Ept);
	make2dline(ptarray,iColor,0.05*sfactor,0.0,LayerId);
	//注字
	AcGePoint3d origin;
	origin.x = 0.5*(Spt.x+Ept.x);
	origin.y = 0.5*(Spt.y+Ept.y) + 0.25*sfactor;
	origin.z =0.0;
	H_maketext(origin,Str,0,texth,itextColor,mode,L"宋体",LayerId);	
}

AcDbObjectId  BAS_DRAW_FUN_JYX::make2dline(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,AcDbObjectId LayerId)
{
	AcDb2dPolyline *pLine	= new AcDb2dPolyline(AcDb::k2dSimplePoly,ptarray,0,Adesk::kFalse,startWid,EndWid);
	pLine->setColorIndex(icolor) ;
	pLine->setLayer(LayerId);
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	return LId ;
}

AcDbObjectId  BAS_DRAW_FUN_JYX::makesolid(AcGePoint3d Pt1,AcGePoint3d Pt2,AcGePoint3d Pt3,int icolor,ACHAR *LayerName)
{
	AcDbSolid *pSolid = new AcDbSolid(Pt1,Pt2,Pt3);
	if(icolor>=0)pSolid->setColorIndex(icolor);

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
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pSolid->setLayer(LayerTabRecordId);	
	pLayerTable->close();
	AcDbObjectId  LId;
	LId = AddEntityToDbs(pSolid) ;
	return LId;     
}
//------------------------------------------------------
//        enttype= "F" or "C"
//------------------------------------------------------
int BAS_DRAW_FUN_JYX::AutoEntBK(ads_point p0, ads_point p1)
{
	struct resbuf *e,*elst,*p,*hdptr,*tlptr;
	ads_name ssname,ename,last;
	struct ptt_node_JYX *pttab,*newptr;
	struct resbuf *pointlist;
	ads_point pt0,pt1,mpt;
	int i,fit_type;
	long n;
	ACHAR a[20];

	pointlist = ads_buildlist(RTPOINT,p0,RTPOINT,p1,0);
	//   acedCommandS(RTSTR,L"line",RTPOINT,p0,RTPOINT,p1,RTSTR,L"",0);
	ads_grdraw(p0,p1,-1,0);
	//   acedCommandS(RTSTR,L"line",RTPOINT,p0,RTPOINT,p1,RTSTR,L"",0);
	ads_ssget(L"F",pointlist,NULL,NULL,ssname);
	if(ads_sslength(ssname,&n)==RTNORM)
	{
		//     ads_printf(L"Number of entity is %ld\n",n);
		//     ads_alert(L"3");

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
			/*	   if(!elst)
			return RTERROR; */
			_tcscpy(a,elst->resval.rstring);
			//       ads_printf(L"a=%s\n",a);

			if(_tcscmp(a,L"LINE")==0)
			{
				getrbpoint(pt0,e,10);
				getrbpoint(pt1,e,11);
				//		   i = ads_inters(p0,p1,pt0,pt1,1,mpt);
				i = INTERS(&mpt[X],&mpt[Y],p0[X],p0[Y],p1[X],p1[Y],pt0[X],pt0[Y],pt1[X],pt1[Y]);		     
				mpt[Z] = pt0[Z];
				if (i == 0 && (fabs(mpt[X] - pt0[X]) > 0.001 || fabs(mpt[Y] - pt0[Y]) > 0.001 || fabs(mpt[Z] - pt0[Z]) > 0.001)
					&& (fabs(mpt[X] - pt1[X]) > 0.001 || fabs(mpt[Y] - pt1[Y]) > 0.001 || fabs(mpt[Z] - pt1[Z]) > 0.001))
				{
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTPOINT, mpt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTPOINT, mpt, 0);
#endif // VERSION_CAD2010		
				}
					


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

					/*			 p = hdptr;
					while(p != tlptr)
					{
					p = p->rbnext;
					ads_relrb(hdptr);
					hdptr = p;
					}
					ads_relrb(p); */
					//			 ads_alert(L"1");
				}

				pttab = get_brkpt(e,p0,p1);
				//		  ads_alert(L"2");
				newptr = pttab->next;
				if(newptr)
				{
					ads_point_set(newptr->pt,mpt);
					//			  ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTSTR, L"@", 0);
					acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTSTR, L"@", 0);
					ads_command(RTNONE);
#endif // VERSION_CAD2010	


					pttab->next = newptr->next;
					free(newptr);
					newptr = pttab;
				}
				else
				{
					if(pttab) free(pttab);
					//			  ads_alert(L"s3");
					ads_relrb(e);
					//			  ads_alert(L"s4");
					ads_ssfree(ename);  
					goto L60;
				}
				while(newptr->next != NULL)
				{
					newptr = newptr->next;
					ads_point_set(newptr->pt,mpt);
					ads_entlast(last);
					//			ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
					/*acedCommandS(RTSTR,L"break",RTENAME,last,RTPOINT,mpt,RTPOINT,mpt,0);*/
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, last, RTPOINT, mpt, RTPOINT, mpt, 0);
					acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, last, RTPOINT, mpt, RTPOINT, mpt, 0);
					ads_command(RTNONE);
#endif // VERSION_CAD2010	

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
					//			  ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTSTR, L"@", 0);
					acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTSTR, L"@", 0);
					ads_command(RTNONE);
#endif // VERSION_CAD2010	

					/*			  {
					tpt[X] = mpt[X] + 0.5; tpt[Y] = mpt[Y] + 0.5; tpt[Z] = mpt[Z];
					i=acedCommandS(RTSTR,L"break",RTENAME,ename,RTPOINT,mpt,RTPOINT,tpt,0);				  
					ads_printf(L"i=%d",i);
					} */
					pttab->next = newptr->next;
					free(newptr);
					newptr = pttab;
				}
				else
				{
					if(pttab) free(pttab);
					ads_relrb(e);
					//			  ads_alert(L"s4");
					ads_ssfree(ename);  
					goto L60;
				}

				while(newptr->next != NULL)
				{
					newptr = newptr->next;
					ads_point_set(newptr->pt,mpt);
					ads_entlast(last);
					//			ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, last, RTPOINT, mpt, RTPOINT, mpt, 0);
					acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, last, RTPOINT, mpt, RTPOINT, mpt, 0);
					ads_command(RTNONE);
#endif // VERSION_CAD2010	

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
/*------------------------------------------*/
struct resbuf *BAS_DRAW_FUN_JYX::get_resbuf(int code,struct resbuf *p)
{
	struct resbuf *rb;
	for(rb=p; (rb != NULL) && (rb->restype != code); rb = rb->rbnext);
	return(rb);
}

int BAS_DRAW_FUN_JYX::getrbpoint(ads_point pt,struct resbuf *ebuf,int code)
{
	struct resbuf *rb;

	if((rb = get_resbuf(code,ebuf)) == NULL)
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
struct arc_lst_JYX *BAS_DRAW_FUN_JYX::findxyr(ads_point ps,ads_point pe,ads_real bulge)
{
	ads_real a,gama,cord,R,cita,angse,ang;
	ads_point cxy;
	struct arc_lst_JYX *newptr;

	a = atan(fabs(bulge));
	gama = 2.0 * a;
	cord = ads_distance(ps,pe);
	R = (0.5 * cord) / sin(gama);
	cita = m_dHalfPI - gama;
	angse = ads_angle(ps,pe);
	if(bulge > 0.0)
		ang = angse + cita;
	else
		ang = angse - cita;
	ads_polar(ps,ang,R,cxy);
	newptr = (struct arc_lst_JYX *)calloc(1,sizeof(struct arc_lst_JYX));
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
struct point_lk_JYX *BAS_DRAW_FUN_JYX::findroot(ads_point ps, ads_point pe, struct arc_lst_JYX *pxyr,ads_real bulge,
	ads_point ps1, ads_point ps2)
{
	ads_real R,xo,yo,xs1,ys1,xs2,ys2,dx,dy,b,k,a1,b1,c1,del,x1,y1,x2,y2,pi2,aps,ape,alp1,alp2;
	bool OK;
	ads_point p1,p2,cxy;
	struct point_lk_JYX *newptr,*headptr,*tailptr;

	newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
	if(newptr)
	{
		newptr->next = NULL;
		headptr = newptr;
		tailptr = newptr;
	}

	R = pxyr->R;
	xo = pxyr->xo; yo = pxyr->yo;
	xs1 = ps1[X] , ys1=ps1[Y];
	xs2 = ps2[X] , ys2=ps2[Y];
	cxy[X] = xo; cxy[Y] = yo;

	dx = ps1[X] - ps2[X];
	OK = true;
	if(fabs(dx) > 0.00000001)
	{
		dy = ps1[Y] - ps2[Y];
		k = dy / dx;
		b = ps1[Y] - k * ps1[X];
		a1 = 1.0 + k * k;
		b1 = 2.0 * k * (b - yo) - 2.0 * xo;
		c1 = xo * xo + (b-yo) * (b-yo) - R * R;
		del = b1 * b1 - 4.0 * a1 * c1;
		if(del >= 0.0)
		{
			del = sqrt(del);
			x1 = (del - b1)/(2.0*a1);
			y1 = k * x1 + b;
			x2 = (del + b1)/(-2.0*a1);
			y2 = k * x2 + b;
		}
		else
			OK = false;
	}
	else
	{
		x1 = xs1; x2 = x1;
		del = R*R - (x1 - xo) * (x1 - xo);
		if(del >= 0.0)
		{
			del = sqrt(del);
			y1 = yo + del;
			y2 = yo - del;
		}
		else
			OK = false;
	}
	p1[X] = x1; p1[Y] = y1; p2[X] = x2; p2[Y] = y2; pi2 = 2.0 * pi;

	if(OK)
	{
		aps = ads_angle(cxy,ps);
		ape = ads_angle(cxy,pe);
		alp1 = ads_angle(cxy,p1);
		alp2 = ads_angle(cxy,p2);
		if(bulge > 0.0)
		{
			if(ape > aps)
			{
				if(alp1 >= aps && alp1 <= ape)
				{
					newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}				
				}
				if(alp2 >= aps && alp2 <= ape)
				{
					newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
					if(newptr)
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
				if((alp1>=0.0 && alp1<=ape) || (alp1>= aps && alp1 <= pi2))
				{
					newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								 
				}
				if((alp2 >=0.0 && alp2 <= ape) || (alp2 >= aps && alp2 <= pi2))
				{
					newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
					if(newptr)
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
			if(aps > ape)
			{
				if(alp1>=ape && alp1<=aps)
				{
					newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								  
				}
				if(alp2>=ape && alp2<=aps)
				{
					newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
					if(newptr)
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
				if((alp1>=0.0 && alp1<=aps) || (alp1>=ape && alp1 <= pi2))
				{
					newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								  
				}
				if((alp2>=0.0 && alp2<=aps) || (alp2>=ape && alp2 <= pi2))
				{
					newptr = (struct point_lk_JYX *)calloc(1,sizeof(struct point_lk_JYX));
					if(newptr)
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
struct ptt_node_JYX *BAS_DRAW_FUN_JYX::get_brkpt(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	struct ptt_node_JYX *Verlst;

	Verlst = get_Arcpt(Entlist,p0,p1);
	//   ads_alert(L"ok2");   
	if(Verlst->next==NULL)
		Verlst = get_plinept(Entlist,p0,p1);

	return Verlst;
}

/*------------------------------------------------------------------
;   get the intersection point xy between polyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node_JYX *BAS_DRAW_FUN_JYX::getbrkpt(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	struct ptt_node_JYX *Verlst;

	Verlst = getArcpt(Entlist,p0,p1);
	if(Verlst->next==NULL)
		Verlst = getplinept(Entlist,p0,p1);

	return Verlst;
}

/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node_JYX *BAS_DRAW_FUN_JYX::get_plinept(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	ads_point pt0,pt1,mptn,mpto,PTS;
	struct resbuf *elst,*plst;
	//  ads_name tname,result;
	struct ptt_node_JYX *newptr,*headptr,*tailptr;
	double Zelev;

	int i;

	headptr = (struct ptt_node_JYX *)calloc(1,sizeof(struct ptt_node_JYX));
	headptr->next = NULL;
	tailptr = headptr;

	elst = Entlist;
	//  getrbpoint(pt0,elst,10);
	plst = get_resbuf(38,elst);
	Zelev = plst->resval.rreal;

	plst = get_resbuf(10,elst);
	//      ads_alert(L"1");
	if(plst)
	{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		pt0[Z] = Zelev;
	}
	else
		return headptr;
	ads_point_set(pt0,PTS);
	ads_point_set(pt0,mpto); 
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
	while(plst != NULL)
	{
		//     getrbpoint(pt1,elst,10);
		plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
			pt1[X] = plst->resval.rpoint[X];
			pt1[Y] = plst->resval.rpoint[Y];
			pt1[Z] = Zelev;

			i = ads_inters(p0,p1,pt0,pt1,1,mptn);

			if(i==RTNORM)
			{
				mptn[Z] = Zelev;

				if(fabs(mptn[X]-mpto[X])>0.001 || fabs(mptn[Y]-mpto[Y])>0.001 || fabs(mptn[Z]-mpto[Z])>0.001)
				{
					newptr = (struct ptt_node_JYX *)calloc(1,sizeof(struct ptt_node_JYX));
					ads_point_set(pt0,newptr->pt0);
					ads_point_set(mptn,newptr->pt); 
					//		   ads_printf(L"xx=%lf\n",newptr->pt[X]);
					ads_point_set(pt1,newptr->pt1);
					newptr->next = NULL;
					tailptr->next = newptr;
					tailptr = newptr;
				}
			}
			ads_point_set(pt1,pt0);
			ads_point_set(mptn,mpto); 

			/*     tlst = get_resbuf(-1,elst);
			ads_name_set(tname,tlst->resval.rlname);
			ads_entnext(tname,result);
			elst = ads_entget(result);
			tlst = get_resbuf(0,elst);
			_tcscpy(Zo,tlst->resval.rstring);*/
		}
	}
	//   ads_alert(L"kk");
	if(headptr->next != NULL)
	{
		newptr = headptr->next;
		ads_point_set(newptr->pt,pt0);
		if(fabs(PTS[X]-pt0[X])<0.0001 && fabs(PTS[Y]-pt0[Y])<0.0001 && fabs(PTS[Z]-pt0[Z])<0.0001)
		{
			headptr->next = newptr->next;
			if(newptr) free(newptr);
		}
		//	 ads_alert(L"jj");
		if(fabs(mptn[X]-pt1[X])<0.0001 && fabs(mptn[Y]-pt1[Y])<0.0001 && fabs(mptn[Z]-pt1[Z])<0.0001)
		{
			//		 ads_alert(L"j");
			newptr = headptr;
			if(newptr==tailptr)
				return headptr;
			while(newptr->next != tailptr)
				newptr = newptr->next;
			newptr->next = NULL;
			if(tailptr) free(tailptr);
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
struct ptt_node_JYX *BAS_DRAW_FUN_JYX::getplinept(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	ads_point pt0,pt1,p00,mptn,mpto,PTS;
	struct resbuf *elst,*tlst;
	ads_name result;
	struct ptt_node_JYX *newptr,*headptr,*tailptr;
	double Zelev;
	ACHAR Zo[20];
	int i;

	headptr = (struct ptt_node_JYX *)calloc(1,sizeof(struct ptt_node_JYX));
	headptr->next = NULL;
	tailptr = headptr;

	elst = Entlist;
	getrbpoint(pt0,elst,10);
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
	if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	{
		tlst = get_resbuf(-1,elst);
		//     ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tlst->resval.rlname,result);

		elst = ads_entget(result); 
		getrbpoint(pt0,elst,10); 
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
	tlst = get_resbuf(0,elst);
	_tcscpy(Zo,tlst->resval.rstring);
	ads_point_set(pt0,PTS);
	ads_point_set(pt0,mpto);

	p0[Z] = Zelev; p1[Z] = Zelev;

	//   ads_alert(L"ii");
	while(_tcscmp(Zo,L"SEQEND") != 0)
	{
		getrbpoint(pt1,elst,10);
		/*	 plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
		pt1[X] = plst->resval.rpoint[X];
		pt1[Y] = plst->resval.rpoint[Y];*/
		pt1[Z] = Zelev;

		i = ads_inters(p0,p1,pt0,pt1,1,mptn);

		if(i==RTNORM)
		{
			mptn[Z] = Zelev;
			//	     ads_printf(L"i=%d,tn[X]=%lf,tn[Y]=%lf,tn[Z]=%lf\n",i,mptn[X],mptn[Y],mptn[Z]);
			if(fabs(mptn[X]-mpto[X])>0.001 || fabs(mptn[Y]-mpto[Y])>0.001 || fabs(mptn[Z]-mpto[Z])>0.001)
			{
				newptr = (struct ptt_node_JYX *)calloc(1,sizeof(struct ptt_node_JYX));
				ads_point_set(pt0,newptr->pt0);
				ads_point_set(mptn,newptr->pt); 
				//		   ads_printf(L"xx=%lf\n",newptr->pt[X]);
				ads_point_set(pt1,newptr->pt1);
				newptr->next = NULL;
				tailptr->next = newptr;
				tailptr = newptr;
			}
		}
		ads_point_set(pt1,pt0);
		ads_point_set(mptn,mpto); 

		tlst = get_resbuf(-1,elst);
		//     ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tlst->resval.rlname,result);
		elst = ads_entget(result);
		tlst = get_resbuf(0,elst);
		_tcscpy(Zo,tlst->resval.rstring);
	}
	//   ads_alert(L"kk");
	if(headptr->next != NULL)
	{
		newptr = headptr->next;
		ads_point_set(newptr->pt,pt0);
		if(fabs(PTS[X]-pt0[X])<0.0001 && fabs(PTS[Y]-pt0[Y])<0.0001 && fabs(PTS[Z]-pt0[Z])<0.0001)
		{
			headptr->next = newptr->next;
			if(newptr) free(newptr);
		}
		//	 ads_alert(L"jj");
		if(fabs(mptn[X]-pt1[X])<0.0001 && fabs(mptn[Y]-pt1[Y])<0.0001 && fabs(mptn[Z]-pt1[Z])<0.0001)
		{
			//		 ads_alert(L"j");
			newptr = headptr;
			if(newptr==tailptr)
				return headptr;
			while(newptr->next != tailptr)
				newptr = newptr->next;
			newptr->next = NULL;
			if(tailptr) free(tailptr);
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
struct ptt_node_JYX *BAS_DRAW_FUN_JYX::get_Arcpt(struct resbuf *Entlist,ads_point p0, ads_point p1)
{
	struct resbuf *elst,*tlst,*plst;
	struct arc_lst_JYX *xyr;
	struct ptt_node_JYX *headptr,*tailptr,*newptr;
	struct point_lk_JYX *xy,*txy;
	int fit_type;
	double Zelev,bulge;
	bool loop;

	ads_point p00,pt0,pt1,pt;
	for (int i=0; i<3; i++)
	{
		p00[i] = 0.0;
		pt0[i] = 0.0;
		pt1[i] = 0.0;
		pt[i] = 0.0;
	}

	headptr = (struct ptt_node_JYX *)calloc(1,sizeof(struct ptt_node_JYX));
	headptr->next = NULL;
	tailptr = headptr;
	elst = Entlist;
	//   getrbpoint(pt0,elst,10);
	plst = get_resbuf(38,elst);
	Zelev = plst->resval.rreal;

	plst = get_resbuf(10,elst);

	if(plst)
	{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		pt0[Z] = Zelev;
	}
	tlst = get_resbuf(70,elst);
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

	if(fit_type < 2) 
	{
		/*     tlst = get_resbuf(0,elst);
		ads_printf(L"string=%s\n",tlst->resval.rstring);*/
		tlst = get_resbuf(42,tlst);
		/*	 if(!tlst)
		{
		ads_alert(L"q");
		return headptr;
		} */
		bulge = tlst->resval.rreal;
	}

	//   ads_printf(L"bulge=%lf\n",bulge);
	if(fit_type < 2 && fabs(bulge) > 0.00001)
	{
		/*     tlst = get_resbuf(-1,elst);
		ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tname,result);
		elst = ads_entget(result);
		getrbpoint(pt1,elst,10);  */
		plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
			pt1[X] = plst->resval.rpoint[X];
			pt1[Y] = plst->resval.rpoint[Y];
			pt1[Z] = Zelev;
		}

		//	 ads_alert(L"p1");
		xyr = findxyr(pt0,pt1,bulge);
		//     ads_alert(L"p2"); 
		xy = findroot(pt0,pt1,xyr,bulge,p0,p1);

		if(xyr) free(xyr);

		while(xy->next != NULL)
		{
			txy = xy->next;
			xy->next = txy->next;

			pt[X] = txy->pt[X]; pt[Y] = txy->pt[Y];

			if((fabs(pt0[X]-pt[X])<0.001 && fabs(pt0[Y]-pt[Y])<0.001 && fabs(pt0[Z]-pt[Z])<0.001)
				|| (fabs(pt1[X]-pt[X])<0.001 && fabs(pt1[Y]-pt[Y])<0.001 && fabs(pt1[Z]-pt[Z])<0.001))        
				loop = true;
			else
			{
				newptr = (struct ptt_node_JYX *)calloc(1,sizeof(struct ptt_node_JYX));
				newptr->next = NULL;
				ads_point_set(pt0,newptr->pt0);
				ads_point_set(pt,newptr->pt);
				ads_point_set(pt1,newptr->pt1); 

				tailptr->next = newptr;
				tailptr = newptr;
			}

			if(txy) free(txy);
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
struct ptt_node_JYX *BAS_DRAW_FUN_JYX::getArcpt(struct resbuf *Entlist,ads_point p0, ads_point p1)
{
	struct resbuf *elst,*tlst;
	struct arc_lst_JYX *xyr;
	struct ptt_node_JYX *headptr,*tailptr,*newptr;
	struct point_lk_JYX *xy,*txy;
	int fit_type;
	double Zelev,bulge;
	bool loop;
	ads_name tname,result;
	ads_point p00,pt0,pt1,pt;
	for (int i=0; i<3; i++)
	{
		p00[i] = pt0[i] = pt1[i] = pt[i] = 0.0;
	}
	for (int i=0; i<2; i++)
	{
		tname[i] = result[i] = 0;
	}

	Zelev = 0.0;
	headptr = (struct ptt_node_JYX *)calloc(1,sizeof(struct ptt_node_JYX));
	headptr->next = NULL;
	tailptr = headptr;
	elst = Entlist;
	getrbpoint(pt0,elst,10);

	tlst = get_resbuf(70,elst);
	fit_type = tlst->resval.rint;

	p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = Zelev;

	//  if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	//  {
	//    剔除POLYLINE第一层放的点,取出下一点的高程
	tlst = get_resbuf(-1,elst);

	ads_entnext(tlst->resval.rlname,result);	 
	elst = ads_entget(result);  
	getrbpoint(pt0,elst,10);

	Zelev = pt0[Z];
	//  }

	p0[Z] = Zelev; p1[Z] = Zelev;

	//拟合方式
	if(fit_type < 2) 
	{
		tlst = get_resbuf(42,elst);
		//曲率
		bulge = tlst->resval.rreal;
	}

	if(fit_type < 2 && fabs(bulge) > 0.00001)//曲线
	{
		tlst = get_resbuf(-1,elst);
		ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tname,result);
		elst = ads_entget(result);
		getrbpoint(pt1,elst,10);  

		xyr = findxyr(pt0,pt1,bulge);
		xy = findroot(pt0,pt1,xyr,bulge,p0,p1);

		if(xyr) free(xyr);

		while(xy->next != NULL)
		{
			txy = xy->next;
			xy->next = txy->next;

			pt[X] = txy->pt[X]; pt[Y] = txy->pt[Y];

			if((fabs(pt0[X]-pt[X])<0.001 && fabs(pt0[Y]-pt[Y])<0.001 && fabs(pt0[Z]-pt[Z])<0.001)
				|| (fabs(pt1[X]-pt[X])<0.001 && fabs(pt1[Y]-pt[Y])<0.001 && fabs(pt1[Z]-pt[Z])<0.001))        
				loop = true;
			else
			{
				newptr = (struct ptt_node_JYX *)calloc(1,sizeof(struct ptt_node_JYX));
				newptr->next = NULL;
				ads_point_set(pt0,newptr->pt0);
				ads_point_set(pt,newptr->pt);
				ads_point_set(pt1,newptr->pt1); 

				tailptr->next = newptr;
				tailptr = newptr;
			}

			if(txy) free(txy);
		}
	}

	return(headptr);
}          


//读颜色文件
BOOL BAS_DRAW_FUN_JYX::ReadColorFile( CString SelFileName , 	 
									 CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArrayToOut )
{
	SetLineAttrArrayToOut.RemoveAll();

	SetLineAttriStruct SetLineAttriStructTemp;
	FILE *fpr = NULL ;
	CString Fname = Cworkdir+"\\SetLineAttriData";
	ACHAR str[80];
	int ColorIndex = 0 ; //线路颜色
	int LineW = 0; //线宽
	CString ColorToXLName ;
	int AllNum;


	if(( fpr = _wfopen(Fname,L"r"))!=NULL)
	{
		fwscanf(fpr,L"%s",str);

		ColorToXLName = Cworkdir + "\\data" + "\\" + str + "";

		if(ColorToXLName!=SelFileName)return FALSE;
		fwscanf(fpr,L"%d",&AllNum);
		for(int i=0;i<AllNum;i++)
		{
			fwscanf(fpr,L"%s",str);

			SetLineAttriStructTemp.XLName = str;

			//线路颜色
			fwscanf(fpr,L"%d",&ColorIndex);
			SetLineAttriStructTemp.ColorIndex = ColorIndex ;

			// 线宽
			fwscanf(fpr,L"%d",&LineW);

			SetLineAttriStructTemp.LineW = LineW ;

			SetLineAttrArrayToOut.Add(SetLineAttriStructTemp);
		}

		fclose(fpr);

		return TRUE;
	}
	else return FALSE;

}

//写颜色文件
void BAS_DRAW_FUN_JYX::WriteColorFile( CString SelFileName ,CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArray )
{

	FILE *fpr = NULL ;
	CString Fname = Cworkdir+"\\SetLineAttriData";
	fpr=_wfopen(Fname,L"w");

	//	CString FilePath;
	if( SetLineAttrArray.GetSize() >0 )
	{
		//串拦截

		int CurFindNum = SelFileName.ReverseFind('\\');

		SelFileName = SelFileName.Right(SelFileName	.GetLength() - CurFindNum -1);


		fwprintf(fpr,L"%s %d\n",SelFileName , SetLineAttrArray.GetSize() );
		for(int i=0;i<SetLineAttrArray.GetSize();i++)
		{
			fwprintf(fpr,L"%s %d %d\n",SetLineAttrArray[i].XLName , 
				SetLineAttrArray[i].ColorIndex , SetLineAttrArray[i].LineW );
		}
	}
	fclose(fpr);
}

//设置默认颜色参数
void BAS_DRAW_FUN_JYX::SetDefaultColor(CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArrayToOut )
{
	SetLineAttrArrayToOut.RemoveAll();

	SetLineAttriStruct SetLineAttr;

	SetLineAttr.XLName = "既有基线" ;
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);


	SetLineAttr.XLName = "既有左线";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);

	SetLineAttr.XLName = "既有右线";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);

	SetLineAttr.XLName = "贯通设计基线";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);

	SetLineAttr.XLName = "贯通设计Ⅱ线";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);

	SetLineAttr.XLName = "联络线";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);

	SetLineAttr.XLName = "施工便线";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);


	SetLineAttr.XLName = "设计基线(既有基线)";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);


	SetLineAttr.XLName = "设计基线(既有左线)";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);


	SetLineAttr.XLName = "设计基线(既有右线)";
	SetLineAttr.ColorIndex = 0 ;
	SetLineAttr.LineW = 0 ;
	SetLineAttrArrayToOut.Add(SetLineAttr);
}


// 由多点绘线
void BAS_DRAW_FUN_JYX::maketable(struct Table &tab,int icolor,double startWid,double EndWid)
{
	int i,j;
	AcGePoint3d startPt, endPt;
	double ht=0,wd=0;
	AcGePoint3dArray PtArr;

	PtArr.setLogicalLength(2);
	PtArr[0].set(tab.LDpt[X],tab.LDpt[Y],0);
	for(i=0;i<tab.ncol;i++)
		wd+=tab.ColWid[i];
	PtArr[1].set(tab.LDpt[X]+wd,tab.LDpt[Y],0);	
	make2dline1(PtArr,icolor,startWid,EndWid);
	for(i=0;i<tab.nrow;i++)//draw row line
	{
		ht+=tab.RowH[i];
		PtArr[0].set(tab.LDpt[X],tab.LDpt[Y]+ht,0);
		PtArr[1].set(tab.LDpt[X]+wd,tab.LDpt[Y]+ht,0);
		make2dline1(PtArr,icolor,startWid,EndWid);

		//ads_printf(L"\ni=%d,ht=%lf,str=%s",i,tab.RowH[i],tab.Str[i].text);
	}

	wd=0.0;
	PtArr[0].set(tab.LDpt[X],tab.LDpt[Y],0);
	PtArr[1].set(tab.LDpt[X],tab.LDpt[Y]+ht,0);
	make2dline1(PtArr,icolor,startWid,EndWid);
	for(i=0;i<tab.ncol;i++)//draw column line
	{
		wd+=tab.ColWid[i];  
		PtArr[0].set(tab.LDpt[X]+wd,tab.LDpt[Y],0);
		PtArr[1].set(tab.LDpt[X]+wd,tab.LDpt[Y]+ht,0);
		make2dline1(PtArr,icolor,startWid,EndWid);
	}
	AcGePoint3d pos,lpos,rpos;
	ACHAR str[80],str1[80],str2[80];
	for(i=0;i<tab.Nstr;i++)
	{
		wd=ht=0;
		for(j=0;j<tab.Str[i].rownum;j++)
			ht+=tab.RowH[j];		
		pos[Y]=tab.LDpt[Y]+ht-tab.RowH[j-1]/2.0;
		for(j=0;j<tab.Str[i].colnum;j++)
			wd+=tab.ColWid[j];		
		pos[X]=tab.LDpt[X]+wd-tab.ColWid[j-1]/2.0;
		pos[Z]=0;
		if(tab.Str[i].ntext==1)//只有一行文本
		{
			_tcscpy(str,tab.Str[i].text);
			if(tab.Str[i].fitinf)
			{         
				lpos[X]=pos[X]-tab.ColWid[tab.Str[i].colnum-1]/2.0,lpos[Y]=pos[Y]-(tab.RowH[tab.Str[i].rownum-1]-tab.Str[i].texth)/tab.Str[i].dv,lpos[Z]=0;
				rpos[X]=pos[X]+tab.ColWid[tab.Str[i].colnum-1]/2.0,rpos[Y]=pos[Y]-(tab.RowH[tab.Str[i].rownum-1]-tab.Str[i].texth)/tab.Str[i].dv,rpos[Z]=0;
				maketext1(lpos,rpos,str,0,tab.Str[i].texth);
			}
			else
				maketext1(pos,str,0,tab.Str[i].texth);
		}
		else if(tab.Str[i].ntext==2)//两行文本
		{
			_tcscpy(str,tab.Str[i].text);
			_tcscpy(str1,tab.Str[i].text1);
			lpos[X]=pos[X],lpos[Y]=pos[Y]+tab.RowH[tab.Str[i].rownum-1]/4,lpos[Z]=0;
			rpos[X]=pos[X],rpos[Y]=pos[Y]-tab.RowH[tab.Str[i].rownum-1]/4,rpos[Z]=0;
			maketext1(lpos,str,0,tab.Str[i].texth);
			maketext1(rpos,str1,0,tab.Str[i].texth);
		}
		else//三行文本
		{
			_tcscpy(str,tab.Str[i].text);
			_tcscpy(str1,tab.Str[i].text1);
			_tcscpy(str2,tab.Str[i].text2);
			lpos[X]=pos[X],lpos[Y]=pos[Y]+tab.RowH[tab.Str[i].rownum-1]/3,lpos[Z]=0;
			rpos[X]=pos[X],rpos[Y]=pos[Y]-tab.RowH[tab.Str[i].rownum-1]/3,rpos[Z]=0;
			maketext1(pos,str,0,tab.Str[i].texth);
			maketext1(lpos,str1,0,tab.Str[i].texth);
			maketext1(rpos,str2,0,tab.Str[i].texth);
		}
	}
}

void BAS_DRAW_FUN_JYX::createBlocks()
{
	AcGePoint3dArray parr;
	AcDbObjectId objId,blockId;
	AcDbDatabase *pDb;
	//ACHAR fname[256];

	ACHAR fname[1024];
	_tcscpy(fname,acedGetAppName());
	CString fstr;
	fstr = fname;
	fstr.MakeUpper();
	fstr.Replace(L"\\ERCAD.ARX",NULL);
	_tcscpy(SysPath,fstr);

	//  GetPrivateProfileString("RLDVS",L"RLDVS",NULL,SysPath,256,L"win.ini");
	Staflag[0].blockname ="乘降所01",Staflag[1].blockname ="乘降所02";//车站图块
	Staflag[2].blockname ="辅助所01",Staflag[3].blockname ="辅助所02";
	Staflag[4].blockname ="区段站01",Staflag[5].blockname ="区段站02";
	Staflag[6].blockname ="区段站03",Staflag[7].blockname ="区段站04";
	Staflag[8].blockname ="线路所01",Staflag[9].blockname ="线路所02";
	Staflag[10].blockname ="中间站01",Staflag[11].blockname ="中间站02";
	Staflag[12].blockname ="中间站03",Staflag[13].blockname ="中间站04";
	Staflag[14].blockname ="中间站05",Staflag[15].blockname ="中间站06";
	Staflag[16].blockname ="中间站07",Staflag[17].blockname ="中间站08";
	Staflag[18].blockname ="中间站09",Staflag[19].blockname ="中间站10";
	Staflag[20].blockname ="中间站11",Staflag[21].blockname ="中间站12";
	Staflag[22].blockname ="中间站13",Staflag[23].blockname ="中间站14";
	Staflag[24].blockname ="中间站15",Staflag[25].blockname ="中间站16";
	Staflag[26].blockname ="中间站17";
	/*

	Staflag[67].blockname ="201011Z",Staflag[68].blockname ="201012Z";
	Staflag[69].blockname ="201021Z",Staflag[70].blockname ="201022Z";
	Staflag[71].blockname ="201031Z",Staflag[72].blockname ="201032Z";
	Staflag[73].blockname ="201033Z",Staflag[74].blockname ="201034Z";
	Staflag[75].blockname ="201035Z",Staflag[76].blockname ="201041Z";
	Staflag[77].blockname ="201042Z",Staflag[78].blockname ="201043Z";
	Staflag[79].blockname ="201044Z",Staflag[80].blockname ="201045Z";
	Staflag[81].blockname ="201046Z",Staflag[82].blockname ="201051Z";
	Staflag[83].blockname ="201052Z",Staflag[84].blockname ="201061Z";
	Staflag[85].blockname ="201062Z",Staflag[86].blockname ="201063Z";
	Staflag[87].blockname ="201064Z",Staflag[88].blockname ="201065Z";
	Staflag[89].blockname ="201071Z",Staflag[90].blockname ="201072Z";
	Staflag[91].blockname ="201073Z",Staflag[92].blockname ="201074Z";
	Staflag[93].blockname ="201075Z",Staflag[94].blockname ="201076Z";
	Staflag[95].blockname ="201081Z",Staflag[96].blockname ="201082Z";
	Staflag[97].blockname ="201083Z",Staflag[98].blockname ="201084Z";
	Staflag[99].blockname ="201085Z",Staflag[100].blockname ="201086Z";
	Staflag[101].blockname ="201091Z",Staflag[102].blockname ="201092Z";
	Staflag[103].blockname ="201101Z",Staflag[104].blockname ="201102Z";
	Staflag[105].blockname ="201111Z",Staflag[106].blockname ="201112Z";
	Staflag[107].blockname ="201121Z",Staflag[108].blockname ="201122Z";
	Staflag[109].blockname ="201131Z",Staflag[110].blockname ="201141Z";
	Staflag[111].blockname ="201142Z",Staflag[112].blockname ="201143Z";
	Staflag[113].blockname ="201144Z",Staflag[114].blockname ="201151Z";
	Staflag[115].blockname ="201152Z",Staflag[116].blockname ="201153Z";
	Staflag[117].blockname ="201154Z",Staflag[118].blockname ="201161Z";
	Staflag[119].blockname ="201162Z",Staflag[120].blockname ="201163Z";
	Staflag[121].blockname ="201164Z",Staflag[122].blockname ="201171Z";
	Staflag[123].blockname ="201172Z",Staflag[124].blockname ="201173Z";
	Staflag[125].blockname ="201174Z",Staflag[126].blockname ="201175Z";
	Staflag[127].blockname ="201181Z",Staflag[128].blockname ="201182Z";
	Staflag[129].blockname ="201183Z",Staflag[130].blockname ="201184Z";
	Staflag[131].blockname ="201185Z",Staflag[132].blockname ="201191Z";
	Staflag[133].blockname ="201192Z";   
	*/
	//	SflgN = 134;
	SflgN = 27;//27种车站
	CFileFind dwgfile;
	int i;
	for(i=0; i<SflgN; i++)
	{
		_stprintf(fname,L"%s\\dwg\\%s.dwg",SysPath,Staflag[i].blockname);

		if(!dwgfile.FindFile(fname))//文件不存在
			continue;
		pDb = new AcDbDatabase(Adesk::kFalse);
		if(pDb->readDwgFile(fname)==Acad::eOk)
		{
			_stprintf(fname,L"%s",Staflag[i].blockname);
			acdbHostApplicationServices()->workingDatabase()->insert(objId,fname,pDb);	
			Staflag[i].blockId=objId;
		}
		delete pDb;
	}

	Pjdflag[0].blockname="普通", Pjdflag[1].blockname="上立交";
	Pjdflag[2].blockname="下立交", Pjdflag[3].blockname="无看守平交";
	Pjdflag[4].blockname="左看守平交", Pjdflag[5].blockname="右看守平交";
	Pjdflag[6].blockname="双看守平交", Pjdflag[7].blockname="自动化平交";
	PjdN=8;

	for(i=0; i<PjdN; i++)
	{
		pDb = new AcDbDatabase(Adesk::kFalse);
		_stprintf(fname,L"%s\\dwg\\%s.dwg",SysPath,Pjdflag[i].blockname);
		if(!dwgfile.FindFile(fname))//文件不存在
			continue;
		if(pDb->readDwgFile(fname)==Acad::eOk)
		{
			_stprintf(fname,L"%s",Pjdflag[i].blockname);
			acdbHostApplicationServices()->workingDatabase()->insert(objId,fname,pDb);	
			Pjdflag[i].blockId=objId;
		}
		delete pDb;
	}

	/*
	Ltjc[0].blockname = "321062";//立体交叉图块
	Ltjc[1].blockname = "321061";//立体交叉图块
	for(i=0; i<2; i++)
	{
	pDb = new AcDbDatabase(Adesk::kFalse);
	_stprintf(fname,L"%s\\dwg\\%s.dwg",SysPath,Ltjc[i].blockname);
	if(!dwgfile.FindFile(fname))//文件不存在
	continue;
	if(pDb->readDwgFile(fname)==Acad::eOk)
	{
	_stprintf(fname,L"%s",Ltjc[i].blockname);
	acdbHostApplicationServices()->workingDatabase()->insert(objId,fname,pDb);	
	Ltjc[i].blockId=objId;
	}
	delete pDb;
	}*/


	//    AcGePoint3d basept;
	//    AcDbBlockTable *pBlockTable;
	//    AcDbBlockTableRecord *pBlockRecord;
	//	AcDb2dPolyline *pLine;
	//    basept[X]=0.0,basept[Y]=0.0,basept[Z]=0.0;
	//	pBlockRecord = new AcDbBlockTableRecord;		
	//    pBlockRecord->setName(L"321062");
	//	pBlockRecord->setOrigin(basept);
	//    acdbHostApplicationServices()->workingDatabase()
	//		->getSymbolTable(pBlockTable,AcDb::kForWrite);	
	//    pBlockTable->add(blockId,pBlockRecord);
	//    parr.setLogicalLength(4);
	//	parr[0].set(-1,0,0),parr[1].set(-1,2,0);
	//	parr[2].set(1,2,0), parr[3].set(1,0,0);
	//	pLine = new AcDb2dPolyline(AcDb::k2dSimplePoly,parr,0,Adesk::kTrue,0,0);
	//	pBlockRecord->appendAcDbEntity(objId,pLine);
	//	pLine->close();
	//	parr.setLogicalLength(2);
	//	parr[0].set(0,2,0),parr[1].set(0,7,0);
	//    pLine = new AcDb2dPolyline(AcDb::k2dSimplePoly,parr,0,Adesk::kFalse,0,0);
	//	pBlockRecord->appendAcDbEntity(objId,pLine);
	//	pLine->close();
	//	parr.setLogicalLength(3);
	//	parr[0].set(0,5,0),parr[1].set(7,5,0),parr[2].set(5,6,0);
	//	pLine = new AcDb2dPolyline(AcDb::k2dSimplePoly,parr,0,Adesk::kFalse,0,0);
	//	pBlockRecord->appendAcDbEntity(objId,pLine);
	//	pLine->close();
	//	parr.setLogicalLength(2);
	//	parr[0].set(0,6.5,0),parr[1].set(5,6.5,0);
	//	pLine = new AcDb2dPolyline(AcDb::k2dSimplePoly,parr,0,Adesk::kFalse,1,1);
	//	pBlockRecord->appendAcDbEntity(objId,pLine);
	//	pLine->close();
	//	parr[0].set(5,6,0),parr[1].set(5,7,0);
	//	pLine = new AcDb2dPolyline(AcDb::k2dSimplePoly,parr,0,Adesk::kFalse,0,4);
	//	pBlockRecord->appendAcDbEntity(objId,pLine);
	//	pLine->close();
	//	pBlockRecord->close();
	//    pBlockTable->close();		
	//    Ltjc[0].blockId = blockId;	
}

AcDbObjectId BAS_DRAW_FUN_JYX::maketext1(AcGePoint3d lpos, AcGePoint3d rpos,ACHAR* ctext,double ang,double texth ,int icolor,ACHAR* textStyle,ACHAR* font,ACHAR*bigfont,ACHAR *LayerName,int i)
{
	AcDbObjectId  TId;
	AcDbText *pText = new AcDbText;
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;

	if(DrawZorY==-1)
	{
		double tmp;
		tmp = lpos[X];
		lpos[X]=DrawZorY*rpos[X];
		rpos[X]=DrawZorY*tmp;
	}

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	if(LayerName==NULL) LayerName=L"0";
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord=new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pLayerTable->close();

	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite);
	if(pTextStyleTbl->getAt(textStyle,pTextStyleTblId)!= Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
		pTextStyleTblRcd->setName(textStyle);
		pTextStyleTblRcd->setFileName(textStyle);
		pTextStyleTblRcd->setFont(font,0,0,134,2);
		pTextStyleTblRcd->setBigFontFileName(bigfont);
		pTextStyleTblRcd->setXScale(0.7);
		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		pTextStyleTbl->getAt(textStyle,pTextStyleTblId);
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
	TId=AddEntityToDbs(pText);
	return TId;     
}


// 由两点绘直线
AcDbObjectId   BAS_DRAW_FUN_JYX::makeline1(AcGePoint3d startPt,AcGePoint3d endPt)
{   
	startPt[X]=DrawZorY*startPt[X];
	endPt[X]=DrawZorY*endPt[X];
	AcDbLine *pLine	= new AcDbLine(startPt,endPt);
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	return LId ;
}

AcDbObjectId  BAS_DRAW_FUN_JYX::make2dline1(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,ACHAR *LayerName)
{
	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;

	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	if(LayerName==NULL) LayerName=L"0";
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord=new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pLayerTable->close();
	int i;
	for( i=0; i<ptarray.logicalLength();i++)
		ptarray[i][0] = DrawZorY*ptarray[i][0];

	AcDb2dPolyline *pLine	= new AcDb2dPolyline(AcDb::k2dSimplePoly,ptarray,0,Adesk::kFalse,startWid,EndWid);
	pLine->setColorIndex(icolor) ;
	pLine->setLayer(LayerTabRecordId);
	AcDbObjectId  LId ;
	LId=AddEntityToDbs(pLine) ;
	for(i=0; i<ptarray.logicalLength();i++)
		ptarray[i][0]=DrawZorY*ptarray[i][0];

	return LId ;
}

AcDbObjectId BAS_DRAW_FUN_JYX::makeline1(AcGePoint3d startPt,AcGePoint3d endPt,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType)
{
	startPt[X]=DrawZorY*startPt[X];
	endPt[X]=DrawZorY*endPt[X];

	AcDbLine *pLine = new AcDbLine(startPt, endPt);
	pLine->setColorIndex(icolor);
	pLine->setLineWeight(lw);

	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;
	AcDbObjectId ltypeObjId;
	//	AcDbLinetypeTable *pLinetypeTble;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	if(LayerName==NULL) LayerName=L"0";
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord=new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}

	pLine->setLayer(LayerTabRecordId);	
	pLayerTable->close();
	if(LineType!=NULL)
	{
		acdbHostApplicationServices()->workingDatabase()->
			loadLineTypeFile(LineType,L"acadiso.lin");//
		pLine->setLinetype(LineType);
	}
	AcDbObjectId  LId;
	LId = AddEntityToDbs(pLine) ;
	return LId;  
}

// 绘文本
AcDbObjectId BAS_DRAW_FUN_JYX::maketext1(AcGePoint3d pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR* textStyle,ACHAR *LayerName)
{

	AcDbObjectId  TId;
	AcDbText *pText = new AcDbText;
	AcDbTextStyleTable *pTextStyleTbl;
	AcDbObjectId pTextStyleTblId;
	AcDbLayerTableRecord *pLayerTableRecord;
	AcDbObjectId LayerTabRecordId;
	AcDbLayerTable *pLayerTable;

	if(DrawZorY==-1)
	{
		pos[X]=DrawZorY*pos[X];
		if(fabs(ang - m_dHalfPI)>0.1)
		{
			if(mode==1)
				mode = 3;
			else if(mode == 3)
				mode = 1;
		}
	}
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable,AcDb::kForWrite);
	if(LayerName==NULL) LayerName=L"0";
	if(pLayerTable->getAt(LayerName,LayerTabRecordId) != Acad::eOk)
	{
		pLayerTableRecord=new AcDbLayerTableRecord;
		pLayerTableRecord->setName(LayerName);
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pLayerTable->close();

	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl,AcDb::kForWrite);
	if(pTextStyleTbl->getAt(textStyle,pTextStyleTblId)!= Acad::eOk)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
		pTextStyleTblRcd->setName(textStyle);
		pTextStyleTblRcd->setFileName(textStyle);
		pTextStyleTblRcd->setFont(textStyle,0,0,134,2);
		pTextStyleTblRcd->setXScale(0.7);
		pTextStyleTbl->add(pTextStyleTblRcd);
		pTextStyleTblRcd->close();
		pTextStyleTbl->getAt(textStyle,pTextStyleTblId);
	}	
	pTextStyleTbl->close();	
	if (mode==1) // 左对齐
	{      pText->setHorizontalMode(AcDb::kTextLeft); }
	else if (mode==2) // // 中对齐
	{      pText->setHorizontalMode(AcDb::kTextCenter); }
	else if(mode==3)// // 右对齐
	{      pText->setHorizontalMode(AcDb::kTextRight); }
	else 
	{
		pText->setHorizontalMode(AcDb::kTextMid);
	}

	pText->setColorIndex(icolor);
	if (mode==1) {  pText->setPosition(pos);	}
	else {  
		pText->setAlignmentPoint(pos);     }
	//  ads_printf(L"\n   m        Pt=%10.3f   %10.3f   %10.4f %s",pos[X],pos[Y],dms_rad(ang),ctext);  
	pText->setTextStyle(pTextStyleTblId);
	pText->setRotation(ang);
	pText->setHeight(texth);
	pText->setTextString(ctext);
	pText->setWidthFactor(0.7);
	pText->setLayer(LayerTabRecordId);	
	TId=AddEntityToDbs(pText);
	return TId;     
}

///*===================================================================
//
//函数功能:为对话框初始化线路名	
//-----------------------------------------------------
//输入:	CComboBox m_Combo_WayName 控件变量
//        int JYorGJ 既有为1  改建为0 施工便线2
//		CString m_Edit_FName MDB路径
//-----------------------------------------------------
//输出:   
//-----------------------------------------------------
//算法描述:   
//-----------------------------------------------------
//
//备注:	                                 钟晶 2008:4:3
//=====================================================================*/
//void BAS_DRAW_FUN_JYX::InitNameOfWay(CComboBox *m_Combo_WayName,int JYorGJ,CString m_Edit_FName)//初始化线路名
//{
//	m_Combo_WayName->ResetContent();
//	XLDataBase_JYX xlmdb;
//	if (0==	JYorGJ)//改建平面
//	{
//		m_Combo_WayName->AddString("贯通设计基线");
//		m_Combo_WayName->SetCurSel(0);
//		//读出绕行段名
//		CString strTemp;
//		CString strNameOfWay = "贯通设计Ⅱ线"; 
//		strNameOfWay="线路名=\""+strNameOfWay+"\"";
//		if(xlmdb.Read_XLDbs(m_Edit_FName,L"二线断链表",strNameOfWay))//读改建段数据
//		{
//			for(int i=0;i<xlmdb.NDL2x;i++)
//			{
//				strTemp.Format(L"%s",xlmdb.DLB2x[i].RXDName);
//				if ((strTemp!="")&&(strTemp!="NULL"))
//				{
//					if (i>0)
//					{
//						if (_tcscmp(strTemp,xlmdb.DLB2x[i-1].RXDName)!=0)
//						{
//							m_Combo_WayName->AddString(strTemp);
//						}
//					}
//					else
//					{
//						m_Combo_WayName->AddString(strTemp);
//					}
//				}
//			}
//		}
//	}
//	else if (1==JYorGJ)	//既有平面
//	{
//		m_Combo_WayName->AddString("既有基线");
//		m_Combo_WayName->SetCurSel(0);
//		//读出既有左线绕行段名
//		CString strTemp;
//		CString strNameOfWay = "既有左线"; 
//		strNameOfWay="线路名=\""+strNameOfWay+"\"";
//		if(xlmdb.Read_XLDbs(m_Edit_FName,L"二线断链表",strNameOfWay))//读改建段数据
//		{
//			for(int i=0;i<xlmdb.NDL2x;i++)
//			{
//				strTemp.Format(L"%s",xlmdb.DLB2x[i].RXDName);
//				if ((strTemp!="")&&(strTemp!="NULL"))
//				{
//					if (i>0)
//					{
//						if (strTemp!=xlmdb.DLB2x[i-1].RXDName)
//						{
//							m_Combo_WayName->AddString(strTemp);
//						}
//					}
//					else
//					{
//						m_Combo_WayName->AddString(strTemp);
//					}
//				}
//			}
//		}
//
//		CString strTemp1;
//		CString strNameOfWay1 = "既有右线"; 
//		strNameOfWay1="线路名=\""+strNameOfWay1+"\"";
//		if(xlmdb.Read_XLDbs(m_Edit_FName,L"二线断链表",strNameOfWay1))//读改建段数据
//		{
//			for(int i=0;i<xlmdb.NDL2x;i++)
//			{
//				strTemp1.Format(L"%s",xlmdb.DLB2x[i].RXDName);
//				if ((strTemp1!="")&&(strTemp1!="NULL"))
//				{
//					if (i>0)
//					{
//						if (strTemp1!=xlmdb.DLB2x[i-1].RXDName)
//						{
//							m_Combo_WayName->AddString(strTemp1);
//						}
//					}
//					else
//					{
//						m_Combo_WayName->AddString(strTemp1);
//					}
//				}
//			}
//		}
//	}
//	else//施工便线
//	{
//		//读施工便线
//		if(xlmdb.Read_XLDbs(m_Edit_FName,L"施工便线信息表",L""))
//		{
//			if(xlmdb.SGBXInforNum>0)
//			{
//				for(int i=0;i<xlmdb.SGBXInforNum;i++ )
//				{
//					m_Combo_WayName->AddString(xlmdb.SGBXTable[i].SGBXName);
//					m_Combo_WayName->SetCurSel(0);
//				}
//			}
//		}
//	}
//
//}

double BAS_DRAW_FUN_JYX::DmlToCml(double dml,DLBZ DLBArray[MAXDLNUM],int DlSum)
{   
	double TYLCB=0,LCB, LCE,TLC,XLC;  // LCB 断链的统一里程
	int iDL=0,bz;
	//起点里程
	CString GH;
	double DLTYLC[MAXDLNUM];

	DLTYLC[0] = DLBArray[0].BLC + DLBArray[0].DL;//断链的统一里程
	for (iDL = 1;iDL < DlSum; iDL++)//检查是否在长链内
	{
		DLTYLC[iDL] = DLTYLC[iDL-1]  + DLBArray[iDL].BLC - DLBArray[iDL-1].DL - DLBArray[iDL-1].BLC;
	}

	TLC=fabs(dml);
	XLC = TLC;
	bz=-99;
	for (iDL=0;iDL<DlSum;iDL++)   // 
	{
		//该断链点统一里程
		TYLCB=DLTYLC[iDL];

		//该断链后链里程
		LCB=DLBArray[iDL].BLC+DLBArray[iDL].DL ;	
		//下一断链的前链里程
		if(iDL<DlSum-1)LCE=DLBArray[iDL+1].BLC; 
		if( ( iDL<DlSum-1 && XLC>=LCB && XLC<=LCE ) || (iDL==DlSum-1 && XLC>=LCB) )  // 落在该段落内
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

AcDbObjectId  BAS_DRAW_FUN_JYX::GUXWXY(double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,int bz,int color,double Lwid,ACHAR *LayerName)
{  
	int zybz;
	if(INDATA[4]<0.0) zybz=-1;
	else zybz=1;
	int j,k;
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
		entid=drawspi(INDATA,l,P0,PZ,k,5.0,color,Lwid,LayerName);
		xlzbxy(P0,PZ,l,INDATA,0);
		//ads_printf(L"4\n");
	}
	else if (j<=2)
	{
		xlzbxy(P0,PZ,l*0.5,INDATA,0); // 某线元中点
		pt[X]=PZ->y;pt[Y]=PZ->x;
		xlzbxy(P0,PZ,l,INDATA,0); // 某线元终点
		pte[X]=PZ->y;pte[Y]=PZ->x;  //  ads_printf(L"%12.5f %12.5f\n",pte[0],pte[1]);
		if (j==1) 
		{
			//       acedCommandS(RTSTR,L"LINE", RTPOINT,ptb, RTPOINT, pte, RTSTR,L"",0);
			entid= makeline(ptb,pte,color,Lwid,LayerName);
		}
		else if (j==2) 
		{
			//			    acedCommandS(RTSTR,L"ARC", RTPOINT,ptb,  RTPOINT, pt, RTPOINT, pte, 0);
			if(INDATA[2]/INDATA[1]<=PI)
				entid= makearc(ptb,pte,INDATA[1],zybz,color,1,LayerName);
			else
				entid= makearc(ptb,pte,INDATA[1],zybz,color,-1,LayerName);
			if(Lwid>0.01)
			{
				ads_point ptt;
				ptt[X] = pte.x,ptt[Y] = pte.y;
#ifdef VERSION_CAD2016
				acedCommandS(RTSTR, L"Pedit", RTPOINT, ptt, RTSTR, L"Y", RTSTR, L"W", RTREAL, Lwid, RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
				ads_command(RTSTR, L"Pedit", RTPOINT, ptt, RTSTR, L"Y", RTSTR, L"W", RTREAL, Lwid, RTSTR, L"", 0);
#endif // VERSION_CAD2010	
				
			}
		};
	};  //J
	}
	else  if ( bz==0) {  xlzbxy(P0,PZ,l,INDATA,0); } ;  // 某线元终点
	// P0=PZ;
	return ( entid);
}

AcDbObjectId BAS_DRAW_FUN_JYX::drawspi(double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ,int bz,double ND,int color,double Lwid,ACHAR *LayerName)
{   int k;
double lc,b=0;
ACHAR buf[220];
AcGePoint2dArray  pArr;
AcGePoint2d ptb,pt,pte,pb,pe;

//  if(INDATA[1]<0.00000001 || INDATA[2]<0.00000001) return 1;
if (bz==1||bz==-1)
{
	xlzbxy(P0,PZ,l*0.5-6*1,INDATA,0);b=PZ->a+pi*0.5*bz;
	pt[X]=PZ->y+5*sin(b);pt[Y]=PZ->x+5*cos(b);
	b=PZ->a; b=pi*0.5-b;if (b<0.0) b=2*pi+b;
	if (b>2*pi) b=b-2*pi;
	// ads_rtos(INDATA[1]+0.00000005,2,7,ch);_tcscpy(chr,L"A=");_tcscat(chr,ch);
	// G_maketext(pt,chr,b,4,4);
};
pt[X]=P0->y;pt[Y]=P0->x;
pb[X]=pt[X];pb[Y]=pt[Y];
xlzbxy(P0,PZ,l,INDATA,0);
pe[X]=PZ->y;pe[Y]=PZ->x;

_stprintf(buf,L"%-5.0f %21.11f %20.8f %20.8f %20.8f %15.8f %12.8f %20.8f %20.8f %20.8f %20.8f\0",
		  INDATA[0],INDATA[1],INDATA[2],INDATA[3],INDATA[4],dms_rad(P0->a),dms_rad(PZ->a),pb[1],pb[0],pe[1],pe[0]);

k=(int)(l/ND+1);
pArr.setLogicalLength(k+1);
pArr[0].set(P0->y,P0->x);
for (k=1;k<=l/ND+1;k++)
{
	lc=k*ND;
	if (lc>l) lc=l;
	xlzbxy(P0,PZ,lc,INDATA,0);
	pArr[k].set(PZ->y,PZ->x);
};
int NUM=pArr.length();
AcDbPolyline *pPline=new AcDbPolyline(NUM);
for(int i =0;i<NUM;i++)
{
	pPline->addVertexAt(i,pArr[i]);
	pPline->setWidthsAt(i,Lwid,Lwid);
}

AcDbObjectId plId;
pPline->setColorIndex(color);

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
	//		acutPrintf(L"%s\n",LayerName);
	pLayerTable->getAt(LayerName,LayerTabRecordId);
	pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
	pLayerTableRecord->close();
}
pPline->setLayer(LayerTabRecordId);	
pLayerTable->close();

plId=AddEntityToDbs(pPline);
return  plId ;//plh.0516

} 

AcDbObjectId BAS_DRAW_FUN_JYX::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType)
{
	AcGePoint2dArray PtAy;
	PtAy.append(AcGePoint2d(startPt.x,startPt.y));
	PtAy.append(AcGePoint2d(endPt.x,endPt.y));
	return makepolyline(PtAy,icolor,lw,LayerName,LineType);
}

AcDbObjectId BAS_DRAW_FUN_JYX::makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType)
{
	AcGePoint2dArray PtAy;
	PtAy.append(AcGePoint2d(startPt.x,startPt.y));
	PtAy.append(AcGePoint2d(endPt.x,endPt.y));
	return makepolyline(PtAy,icolor,lw,LayerName,LineType);
}

AcDbObjectId BAS_DRAW_FUN_JYX::makearc(AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx,int color,int fh,ACHAR *LayerName)
{
	AcDbArc *pArc = new AcDbArc;
	AcDbObjectId ArcId;

	//	acdbOpenObject(pArc,ArcId,AcDb::kForWrite,Adesk::kFalse);	
	AcGePoint3d centerPt;
	double f1,sf,ef,dd,AA;
	pArc->setRadius(R);
	dd=xyddaa(startPt[1],startPt[0],endPt[1],endPt[0],&f1);
	AA=fh*acos(dd*0.5/R);
	centerPt[Y]=startPt[Y]+R*cos(f1+npx*AA);
	centerPt[X]=startPt[X]+R*sin(f1+npx*AA);
	centerPt[Z]=startPt[Z];
	//f2=f1+pi; if (f1>pi) f2=f1-pi;
	// ads_printf(L"\n 直线长 distane=%16.7f 方位角 (d.ms) f=%12.7f (%12.7f) ",dd,dms_rad(f1),dms_rad(f2));
	//   ads_printf(L"\n R%7.2f 偏向=%d ",R,npx);
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
	pArc->setColorIndex(color);

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
		//		acutPrintf(L"%s\n",LayerName);
		pLayerTable->getAt(LayerName,LayerTabRecordId);
		pLayerTable->add(LayerTabRecordId,pLayerTableRecord);
		pLayerTableRecord->close();
	}
	pArc->setLayer(LayerTabRecordId);	
	pLayerTable->close();
	//	pArc->close();
	ArcId=AddEntityToDbs(pArc);         
	return ArcId ;
}

//void BAS_DRAW_FUN_JYX::GetAllRXD(CString MDBName,CString XLName,CArray<CString,CString>&AllRXDName)
//{
//	//读出绕行段名
//	CString strTemp;
//	CString strNameOfWay = XLName; 
//	strNameOfWay="线路名=\""+strNameOfWay+"\"";
//	XLDataBase_JYX xlmdb;
//	if(xlmdb.Read_XLDbs(MDBName,L"二线断链表",strNameOfWay))
//	{
//		for(int i=0;i<xlmdb.NDL2x;i++)
//		{
//			strTemp.Format(L"%s",xlmdb.DLB2x[i].RXDName);
//			if ((strTemp!="")&&(strTemp!="NULL"))
//			{
//				if (i>0)
//				{
//					if (_tcscmp(strTemp,xlmdb.DLB2x[i-1].RXDName)!=0)
//					{
//						AllRXDName.Add(strTemp);
//					}
//				}
//				else
//				{
//					AllRXDName.Add(strTemp);
//				}
//			}
//		}
//	}
//}

//传入多边形的ID数组，用以合成面域，求多边形相交的面积(curveIds1为多个多边形，curveIds2为一个多边形)
bool BAS_DRAW_FUN_JYX::GetArea(const AcDbObjectIdArray& curveIds1,const AcDbObjectIdArray& curveIds2,double & ReArea)
{
	AcDbVoidPtrArray curves1,curves2; // 指向作为面域边界的曲线的指针的数组
	AcDbVoidPtrArray regions1,regions2; // 指向创建的面域对象的指针的数组
	AcDbEntity *pEnt1,*pEnt2; // 临时指针，用来关闭边界曲线
	AcDbRegion *pRegion; // 临时对象，用来将面域添加到模型空间

	// 用curveIds1初始化curves1
	for (int i = 0; i < curveIds1.length(); i++)
	{
		/*acdbOpenAcDbEntity(pEnt1, curveIds1.at(i), AcDb::kForRead);*/
		if(acdbOpenObject(pEnt1, curveIds1.at(i), AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return false;
		}
		if (pEnt1->isKindOf(AcDbCurve::desc()))
			curves1.append(static_cast<void*>(pEnt1));
	}
	Acad::ErrorStatus es1 = AcDbRegion::createFromCurves(curves1,regions1);

	// 用curveIds2初始化curves2
	int i;
	for (i = 0; i < curveIds2.length(); i++)
	{
		/*acdbOpenAcDbEntity(pEnt2, curveIds2.at(i), AcDb::kForRead);*/
		if(acdbOpenObject(pEnt2, curveIds2.at(i), AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return false;
		}
		if (pEnt2->isKindOf(AcDbCurve::desc()))
			curves2.append(static_cast<void*>(pEnt2));
	}
	Acad::ErrorStatus es2 = AcDbRegion::createFromCurves(curves2,regions2);

	// 关闭作为边界的对象
	for (i = 0; i < curves1.length(); i++)
	{
		pEnt1 = static_cast<AcDbEntity*>(curves1[i]);
		pEnt1->close();
	}

	for (i = 0; i < curves2.length(); i++)
	{
		pEnt2 = static_cast<AcDbEntity*>(curves2[i]);
		pEnt2->close();
	}

	//计算多个多边形的并集
	AcDbRegion *pRegion1=NULL,*pRegion2=NULL,*pRegion3=NULL,*pRegion4=NULL;
	if (regions1.length()>0)
		pRegion1 = static_cast<AcDbRegion*>(regions1[0]);
	else
	{
		AfxMessageBox(L"用地面积多段线不闭合，无法计算面积！");
		return false;
	}

	//将用地面积多边形面域求并集
	for (int j=1;j<regions1.length();j++)
	{
		pRegion2 = static_cast<AcDbRegion*>(regions1[j]);
		pRegion1->booleanOper(AcDb::kBoolUnite, pRegion2);
	}

	//将分段多边形面域求并集
	if (regions2.length()>0)
		pRegion3 = static_cast<AcDbRegion*>(regions2[0]);
	else
	{
		AfxMessageBox(L"分段多边形无法创建，无法计算面积！");
		return false;
	}

	for (int j=1;j<regions2.length();j++)
	{
		pRegion4 = static_cast<AcDbRegion*>(regions2[j]);
		pRegion3->booleanOper(AcDb::kBoolUnite, pRegion4);
	}

	//并集后再与分段多边形组成的面域求交集
	pRegion1->booleanOper(AcDb::kBoolIntersect, pRegion3);

	// 	pRegion1.setDatabaseDefaults();
	// 	AcDbObjectId regionId;
	// 	regionId = AddEntityToDbs(pRegion1);
	// 	regionIds.append(regionId);
	pRegion1->getArea(ReArea);
	return true;
	// 	ads_printf(L"reArea=%lf\n",reArea);
}

AcDbObjectId BAS_DRAW_FUN_JYX::FromePLineBySEMl(double Scml, double Ecml, double (*XYArray)[10], int XYNum)
{
	AcDbObjectId PlineId;
	int DrawXySn,DrawXyEn,i;
	AcGePoint2d Pt;
	double cml;
	double lenth = 300.0;

	DrawXySn = 1; DrawXyEn=  XYNum;
	for (i = 1; i <  XYNum; i++) 
	{
		if( XYArray[i][6]>Scml-0.01)
		{
			DrawXySn = i;
			break;
		}
	}
	if (i==XYNum)
	{
		DrawXySn = XYNum -1;
	}
	for (i = 1; i <=  XYNum; i++)
	{
		if( XYArray[i][6]>Ecml-0.01)
		{
			DrawXyEn = i-1;
			break;
		}
	}
	xlpoint  PZtmp; 
	AcGePoint2dArray LeftPt2dAy,RightPt2dAy;
	cml = Scml;
	PZtmp.lc = cml;
	xlpoint_pz(XYArray,XYNum,&PZtmp);
	Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
	Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
	LeftPt2dAy.append(Pt);

	Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
	Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
	RightPt2dAy.append(Pt);

	if(DrawXySn > 1)
	{
		if (DrawXyEn==DrawXySn-1)//绘图范围不足一个线元
		{
			xlpoint PZtmp2;
			PZtmp2.lc = Ecml;
			xlpoint_pz(XYArray,XYNum,&PZtmp2);
			if( XYArray[DrawXySn-1][0]<1.1)//直线
			{
				Pt.y = PZtmp2.x + lenth*cos(PZtmp2.a-0.5*Pi);
				Pt.x = PZtmp2.y + lenth*sin(PZtmp2.a-0.5*Pi);
				LeftPt2dAy.append(Pt);

				Pt.y = PZtmp2.x + lenth*cos(PZtmp2.a+0.5*Pi);
				Pt.x = PZtmp2.y + lenth*sin(PZtmp2.a+0.5*Pi);
				RightPt2dAy.append(Pt);

			}
			else//缓和曲线
			{
				cml+=20.0;
				while(cml <  Ecml)
				{
					PZtmp.lc = cml;
					xlpoint_pz(XYArray,XYNum,&PZtmp);
					Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
					Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
					LeftPt2dAy.append(Pt);

					Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
					Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
					RightPt2dAy.append(Pt);

					cml+=20.0;
				}
				PZtmp.lc =  Ecml;
				xlpoint_pz(XYArray,XYNum,&PZtmp);
				Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
				Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
				LeftPt2dAy.append(Pt);

				Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
				Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
				RightPt2dAy.append(Pt);			

			}

			for (int iPt=RightPt2dAy.length()-1; iPt>-1; iPt--)
			{
				LeftPt2dAy.append(RightPt2dAy.at(iPt));
			}
			LeftPt2dAy.append(LeftPt2dAy.at(0));
			AcDbPolyline *polyline=new AcDbPolyline(LeftPt2dAy.length());
			for(int i =0;i<LeftPt2dAy.length();i++)
			{
				polyline->addVertexAt(i,LeftPt2dAy.at(i));
			}
			//  			PlineId = polyline->objectId();
			PlineId = makepolyline(LeftPt2dAy,-1,0,NULL);

			return PlineId;
		}

		if( XYArray[DrawXySn-1][0]<1.1)//直线
		{
			Pt.x = XYArray[DrawXySn][9] + lenth*sin(XYArray[DrawXySn][5] - 0.5*pi);
			Pt.y = XYArray[DrawXySn][8] + lenth*cos(XYArray[DrawXySn][5] - 0.5*pi);
			LeftPt2dAy.append(Pt);

			Pt.x = XYArray[DrawXySn][9] + lenth*sin(XYArray[DrawXySn][5] + 0.5*pi);
			Pt.y = XYArray[DrawXySn][8] + lenth*cos(XYArray[DrawXySn][5] + 0.5*pi);
			RightPt2dAy.append(Pt);
		}
		else//缓和曲线
		{
			cml+=20;
			while(cml <  XYArray[DrawXySn][6])
			{
				PZtmp.lc = cml;
				xlpoint_pz(XYArray,XYNum,&PZtmp);
				Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
				Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
				LeftPt2dAy.append(Pt);

				Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
				Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
				RightPt2dAy.append(AcGePoint2d(Pt));

				cml+=20;
			}
			PZtmp.lc =  XYArray[DrawXySn][6];
			xlpoint_pz(XYArray,XYNum,&PZtmp);
			Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
			Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
			LeftPt2dAy.append(Pt);	

			Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
			Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
			RightPt2dAy.append(Pt);			
		}		
	}

	for(i=DrawXySn;i<DrawXyEn;i++)
	{

		if( XYArray[i][0]<1.1)//直线
		{
			Pt.x = XYArray[i+1][9] + lenth*sin(XYArray[i+1][5] - 0.5*pi);
			Pt.y = XYArray[i+1][8] + lenth*cos(XYArray[i+1][5] - 0.5*pi);
			LeftPt2dAy.append(Pt);

			Pt.x = XYArray[i+1][9] + lenth*sin(XYArray[i+1][5] + 0.5*pi);
			Pt.y = XYArray[i+1][8] + lenth*cos(XYArray[i+1][5] + 0.5*pi);
			RightPt2dAy.append(Pt);
		}
		else//缓和曲线
		{
			cml =  XYArray[i][6];
			cml+=20;
			while(cml <  XYArray[i+1][6])
			{
				PZtmp.lc = cml;
				xlpoint_pz(XYArray,XYNum,&PZtmp);
				Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
				Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
				LeftPt2dAy.append(Pt);

				Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
				Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
				RightPt2dAy.append(Pt);

				cml+=20;
			}
			PZtmp.lc =  XYArray[i+1][6];
			xlpoint_pz(XYArray,XYNum,&PZtmp);
			Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
			Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
			LeftPt2dAy.append(Pt);

			Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
			Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
			RightPt2dAy.append(Pt);			
		}		
	}

	if(DrawXyEn <  XYNum)
	{    
		cml =  XYArray[DrawXyEn][6];
		Pt.x = XYArray[DrawXyEn][9] + lenth*sin(XYArray[DrawXyEn][5] - 0.5*pi);
		Pt.y = XYArray[DrawXyEn][8] + lenth*cos(XYArray[DrawXyEn][5] - 0.5*pi);
		LeftPt2dAy.append(Pt);

		Pt.x = XYArray[DrawXyEn][9] + lenth*sin(XYArray[DrawXyEn][5] + 0.5*pi);
		Pt.y = XYArray[DrawXyEn][8] + lenth*cos(XYArray[DrawXyEn][5] + 0.5*pi);
		RightPt2dAy.append(Pt);

		if( XYArray[DrawXyEn][0]<1.1)//直线
		{
			cml = Ecml;
			PZtmp.lc = cml;
			xlpoint_pz(XYArray,XYNum,&PZtmp);
			Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
			Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
			LeftPt2dAy.append(Pt);

			Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
			Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
			RightPt2dAy.append(Pt);
		}
		else
		{
			cml+=20;
			while(cml < Ecml)
			{
				PZtmp.lc = cml;
				xlpoint_pz(XYArray,XYNum,&PZtmp);
				Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
				Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
				LeftPt2dAy.append(Pt);

				Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
				Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
				RightPt2dAy.append(Pt);

				cml+=20;
			}
			PZtmp.lc = Ecml;
			xlpoint_pz(XYArray,XYNum,&PZtmp);
			Pt.y = PZtmp.x + lenth*cos(PZtmp.a-0.5*Pi);
			Pt.x = PZtmp.y + lenth*sin(PZtmp.a-0.5*Pi);
			LeftPt2dAy.append(Pt);

			Pt.y = PZtmp.x + lenth*cos(PZtmp.a+0.5*Pi);
			Pt.x = PZtmp.y + lenth*sin(PZtmp.a+0.5*Pi);
			RightPt2dAy.append(Pt);
		}
	}

	for (int iPt=RightPt2dAy.length()-1; iPt>-1; iPt--)
	{
		LeftPt2dAy.append(RightPt2dAy.at(iPt));
	}
	LeftPt2dAy.append(LeftPt2dAy.at(0));
	AcDbPolyline *polyline=new AcDbPolyline(LeftPt2dAy.length());
	for(int i =0;i<LeftPt2dAy.length();i++)
	{
		polyline->addVertexAt(i,LeftPt2dAy.at(i));
	}
	//  	PlineId = polyline->objectId();
	PlineId = makepolyline(LeftPt2dAy,-1,0,NULL);

	return PlineId;
}
BOOL BAS_DRAW_FUN_JYX:: IsOnSameLine( AcGePoint3d PT1 , AcGePoint3d PT2 , AcGePoint3d PT3 , AcGePoint3d PT4 )
{

	AcGeLine3d pLine1(PT1,PT2);

	double Dist1 = pLine1.distanceTo(PT3);

	double Dist2 = pLine1.distanceTo(PT4);

	if( fabs(Dist1)<0.01 && fabs(Dist2)<0.01 )
		return TRUE ;

	return FALSE ;
} 

CString BAS_DRAW_FUN_JYX::GetProByMdbName(CString mdbPath)
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