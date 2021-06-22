// EarthLine.cpp: implementation of the EarthLine class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "EarthLine.h"
#include "string.h"


ACRX_DXF_DEFINE_MEMBERS(EarthLine_ROAD, AcDbEntity,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						0,  EarthLine_ROAD ,RailHdm);
extern double Sfactor;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EarthLine_ROAD::EarthLine_ROAD()
{
	m_EarthPointNum = 0;
	m_BenchPointNum = 0;
	m_BenchPoints = NULL;
	m_EarthPoints = NULL;
	HTHD = 0.0;
}

EarthLine_ROAD::~EarthLine_ROAD()
{
}
/*
Acad::ErrorStatus EarthLine::dwgInFields(AcDbDwgFiler* pFiler)
{
assertWriteEnabled();
AcDbObject::dwgInFields(pFiler);

char *ckml = NULL;

pFiler->readString(&ckml);
_tcscpy(m_ckml, ckml);
pFiler->readItem(&m_cml);
pFiler->readItem(&m_TWH);
pFiler->readItem(&m_FillArea);
pFiler->readItem(&m_CutArea);
pFiler->readItem(&m_LeftZdk);
pFiler->readItem(&m_RightZdk);
pFiler->readItem(&m_OriPoint);

return pFiler->filerStatus();
}

Acad::ErrorStatus EarthLine::dwgOutFields(AcDbDwgFiler* pFiler) const
{
assertReadEnabled();
AcDbObject::dwgOutFields(pFiler);

pFiler->writeItem(m_ckml);
pFiler->writeItem(getEarthLineCml());
pFiler->writeItem(getEarthLineTWH());
pFiler->writeItem(getEarthLineFillArea());
pFiler->writeItem(getEarthLineCutArea());
pFiler->writeItem(getEarthLineLeftZdk());
pFiler->writeItem(getEarthLineRightZdk());
pFiler->writeItem(getEarthLineOriPoint());

return pFiler->filerStatus();
}


// Files data in from a DXF file
//
Acad::ErrorStatus EarthLine::dxfInFields(AcDbDxfFiler* pFiler)
{
struct resbuf rb;
AcGePoint3d llpt;

assertWriteEnabled();

Acad::ErrorStatus es;
if ((es = AcDbObject::dxfInFields(pFiler))
!= Acad::eOk)
{
ads_printf(L"\nError in DXFIN ");
return es;
}

// Check if we're at the right subclass getData marker
//
if (!pFiler->atSubclassData(L"EarthLine"))
{
ads_printf(L"\nBad DXFIN subclass marker ");
return Acad::eBadDxfSequence;
}

pFiler->readItem(&rb);
if(rb.restype == AcDb::kDxfXTextString)
{
_tcscpy(m_ckml, rb.resval.rstring);
}
else
{
pFiler->pushBackItem();

pFiler->setError(Acad::eInvalidDxfCode,
"\nError: expected group code %d",
AcDb::kDxfXTextString);

return pFiler->filerStatus();
}

pFiler->readItem(&rb);
if(rb.restype == AcDb::kDxfReal)
{
setEarthLineCml(rb.resval.rreal);
}
else
{
pFiler->pushBackItem();

pFiler->setError(Acad::eInvalidDxfCode,
"\nError: expected group code %d",
AcDb::kDxfReal);

return pFiler->filerStatus();
}

pFiler->readItem(&rb);
if(rb.restype == AcDb::kDxfReal)
{
setEarthLineTWH(rb.resval.rreal);
}
else
{
pFiler->pushBackItem();

pFiler->setError(Acad::eInvalidDxfCode,
"\nError: expected group code %d",
AcDb::kDxfReal);

return pFiler->filerStatus();
}

pFiler->readItem(&rb);
if(rb.restype == AcDb::kDxfReal)
{
setEarthLineFillArea(rb.resval.rreal);
}
else
{
pFiler->pushBackItem();

pFiler->setError(Acad::eInvalidDxfCode,
"\nError: expected group code %d",
AcDb::kDxfReal);

return pFiler->filerStatus();
}

pFiler->readItem(&rb);
if(rb.restype == AcDb::kDxfReal)
{
setEarthLineCutArea(rb.resval.rreal);
}
else
{
pFiler->pushBackItem();

pFiler->setError(Acad::eInvalidDxfCode,
"\nError: expected group code %d",
AcDb::kDxfReal);

return pFiler->filerStatus();
}

pFiler->readItem(&rb);
if(rb.restype == AcDb::kDxfReal)
{
setEarthLineLeftZdk(rb.resval.rreal);
}
else
{
pFiler->pushBackItem();

pFiler->setError(Acad::eInvalidDxfCode,
"\nError: expected group code %d",
AcDb::kDxfReal);

return pFiler->filerStatus();
}

pFiler->readItem(&rb);
if(rb.restype == AcDb::kDxfReal)
{
setEarthLineRightZdk(rb.resval.rreal);
}
else
{
pFiler->pushBackItem();

pFiler->setError(Acad::eInvalidDxfCode,
"\nError: expected group code %d",
AcDb::kDxfReal);

return pFiler->filerStatus();
}

pFiler->readItem(&rb);
if(rb.restype == AcDb::kDxfXCoord)
{
llpt.x = rb.resval.rpoint[X];
llpt.y = rb.resval.rpoint[Y];
setEarthLineOriPoint(llpt);
}
else
{
pFiler->pushBackItem();

pFiler->setError(Acad::eInvalidDxfCode,
"\nError: expected group code %d",
AcDb::kDxfXCoord);

return pFiler->filerStatus();
}


return pFiler->filerStatus();
}

// Files data out to a DXF file
//
Acad::ErrorStatus EarthLine::dxfOutFields(AcDbDxfFiler* pFiler) const
{
assertReadEnabled();

AcDbObject::dxfOutFields(pFiler);

pFiler->writeItem(AcDb::kDxfSubclass, "EarthLine");
pFiler->writeItem(AcDb::kDxfXTextString, m_ckml);
pFiler->writeItem(AcDb::kDxfReal, getEarthLineCml());
pFiler->writeItem(AcDb::kDxfReal, getEarthLineTWH());
pFiler->writeItem(AcDb::kDxfReal, getEarthLineFillArea());
pFiler->writeItem(AcDb::kDxfReal, getEarthLineCutArea());
pFiler->writeItem(AcDb::kDxfReal, getEarthLineLeftZdk());
pFiler->writeItem(AcDb::kDxfReal, getEarthLineRightZdk());
pFiler->writeItem(AcDb::kDxfXCoord, getEarthLineOriPoint());

return pFiler->filerStatus();
}*/


Adesk::Boolean EarthLine_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{
	assertReadEnabled();
	AcGePoint3d pt[4];
	AcGePoint3d origin;
	//绘地面线
	pWd->subEntityTraits().setColor(1);
	pWd->geometry().polyline(m_EarthPointNum,m_EarthPoints);
	pWd->subEntityTraits().setColor(2);
	AcGePoint3d* BenchPtArray;
	for (int Lever=0; Lever<m_BenchPointNum/3; Lever++)
	{
		BenchPtArray = new AcGePoint3d[3];
		BenchPtArray[0] = m_BenchPoints[Lever*3+0];
		BenchPtArray[1] = m_BenchPoints[Lever*3+1];
		BenchPtArray[2] = m_BenchPoints[Lever*3+2];
		pWd->geometry().polyline(3,BenchPtArray);
		delete BenchPtArray;
	}

	if(_tcscmp(m_ZorY,L"左侧")==0)//只标一次 
	{
		ACHAR Str[50];
		pt[0] = bzpoint;
		pt[1].x = pt[0].x - 0.1*Sfactor;
		pt[1].y = pt[0].y + 0.15*Sfactor;
		pt[2].x = pt[0].x + 0.1*Sfactor;
		pt[2].y = pt[0].y + 0.15*Sfactor;
		pt[3] = pt[0];
		pWd->subEntityTraits().setColor(6);
		pWd->subEntityTraits().setFillType(kAcGiFillAlways);
		pWd->geometry().polygon(4,pt);

		origin.x = pt[0].x;
		origin.y = pt[0].y + 0.25*Sfactor;
		origin.z = pt[0].z;
		AcGeVector3d direction(1.0,0.0,0.0);
		AcGeVector3d normal(0.0,0.0,1.0);
		AcGiTextStyle pTextStyle;
		pTextStyle.setTextSize(0.5);//设置字宽
		_stprintf(Str,L"%0.3lf",EarH);//路肩设计高 
		pWd->subEntityTraits().setColor(7);
		pWd->geometry().text(origin,normal,direction,Str,_tcslen(Str),Adesk::kTrue,pTextStyle);//字高、字宽、旋转角度
	}
	return Adesk::kTrue;
}

/*Acad::ErrorStatus EarthLine::getGripPoints(AcGePoint3dArray& gripPoints, 
AcDbIntArray& osnapMasks,
AcDbIntArray& geomIds) const
{
/*AcGePoint3d ll, lr, ur, ul;
AcGeVector3d vec;

assertReadEnabled();

// Get the window length, height and start point
len = getWindowLength();
ht	= getWindowHeight();
ll	= getWindowStartPoint();

// Calculate the other 3 corner points
// from ll
vec.set(len, 0, 0);
lr = ll + vec;
vec.set(0, ht, 0);
ur = lr + vec;
ul = ll + vec;


gripPoints.append(ll);
gripPoints.append(lr);
gripPoints.append(ur);
gripPoints.append(ul);
gripPoints.append(ll + 0.5 * (ur - ll));

return Acad::eOk;
}

/*
Acad::ErrorStatus EarthLine::moveGripPointsAt(const AcDbIntArray& indices,
const AcGeVector3d& offset)
{
AcGePoint3d ll, lr, ur, ul, ctrlpt;

AcGeVector3d vec, ctrlvec;	
assertWriteEnabled();

// Get the window length, height and start point
/ *	len = getWindowLength();
ht	= getWindowHeight();
ll	= getWindowStartPoint();

ctrlvec.set(10.0, 10.0, 0.0);

// Calculate the other 3 corner points
// from ll
vec.set(len, 0, 0);
lr = ll + vec;
vec.set(0, ht, 0);
ur = lr + vec;
ul = ll + vec;

switch(indices[0])
{
case 0:
// Change the start point and length and height of the window
// lower left grip

ll += offset;
ctrlpt = ur - ctrlvec;

if(ll.x <= ctrlpt.x && ll.y <= ctrlpt.y)
{
setWindowStartPoint(ll);
setWindowLength(ur.x - ll.x);
setWindowHeight(ur.y - ll.y);
}

break;

case 1:
// Change the width of the window
// lower right grip

lr += offset;
ctrlpt = ll + ctrlvec;

if(lr.x >= ctrlpt.x)
{
setWindowLength(lr.x - ll.x);
}

break;

case 2:
// Change the width and height of the window
// upper right grip

ur += offset;
ctrlpt = ll + ctrlvec;

if(ur.x >= ctrlpt.x && ur.y >= ctrlpt.y)
{
setWindowLength(ur.x - ll.x);
setWindowHeight(ur.y - ll.y);
}

break;

case 3:
// Change the height of the window
// upper left grip

ul += offset;
ctrlpt = ll + ctrlvec;

if(ul.y >= ctrlpt.y)
{
setWindowHeight(ul.y - ll.y);
}

break;

case 4:
// Change the window locatin
// center grip

ll += offset;
setWindowStartPoint(ll);

break;


}// end switch

return Acad::eOk;* /
}*/


/*Acad::ErrorStatus EarthLine::getStretchPoints(AcGePoint3dArray& stretchPoints) const
{
AcDbIntArray osnapModes;
AcDbIntArray geomIds;

assertReadEnabled();

//	getStretchPoints == getGripPoints
//
return getGripPoints(stretchPoints, osnapModes, geomIds);
}

/*Acad::ErrorStatus EarthLine::getOsnapPoints(AcDb::OsnapMode osnapMode,
int gsSelectionMark,
const AcGePoint3d& pickPoint,
const AcGePoint3d& lastPoint,
const AcGeMatrix3d& viewXform,
AcGePoint3dArray& snapPoints,
AcDbIntArray& geomIds) const
{
assertReadEnabled();

switch(osnapMode)
{

case AcDb::kOsModeEnd:
return osnapEnd(pickPoint, snapPoints);

case AcDb::kOsModeMid:
return osnapMid(pickPoint, snapPoints);

case AcDb::kOsModeCen:
return osnapCen(pickPoint, snapPoints);

case AcDb::kOsModeNear:
case AcDb::kOsModeQuad:
case AcDb::kOsModeNode:
case AcDb::kOsModeIns:
case AcDb::kOsModePerp:
case AcDb::kOsModeTan:
default:
break;
}

return Acad::eInvalidInput;
}

Acad::ErrorStatus EarthLine::osnapEnd(const AcGePoint3d& pickPoint,
AcGePoint3dArray& snapPoints) const
{
AcGePoint3d ll, lr, ur, ul;
AcGeVector3d vec;
double len, ht;

assertReadEnabled();

// Get the window length, height and start point
len = getWindowLength();
ht	= getWindowHeight();
ll	= getWindowStartPoint();

// Calculate the other 3 corner points
// from ll
vec.set(len, 0, 0);
lr = ll + vec;
vec.set(0, ht, 0);
ur = lr + vec;
ul = ll + vec;

snapPoints.append(ll);
snapPoints.append(lr);
snapPoints.append(ur);
snapPoints.append(ul);

return Acad::eOk;
}

Acad::ErrorStatus EarthLine::osnapMid(const AcGePoint3d& pickPoint,
AcGePoint3dArray& snapPoints) const
{
AcGePoint3d ll, lr, ur, ul, btm_mid, top_mid, left_mid, right_mid;
AcGeVector3d vec;
double len, ht;

assertReadEnabled();

// Get the window length, height and start point
len = getWindowLength();
ht	= getWindowHeight();
ll	= getWindowStartPoint();

// Calculate the other 3 corner points
// from ll
vec.set(len, 0, 0);
lr = ll + vec;
vec.set(0, ht, 0);
ur = lr + vec;
ul = ll + vec;

// Calculate the mid points
vec.set(0.5 * len, 0, 0);
btm_mid = ll + vec;
top_mid = ul + vec;
vec.set(0, 0.5 * ht, 0);
left_mid = ll + vec;
right_mid = lr + vec;

snapPoints.append(left_mid);
snapPoints.append(right_mid);
snapPoints.append(btm_mid);
snapPoints.append(top_mid);

return Acad::eOk;
}

Acad::ErrorStatus EarthLine::osnapCen(const AcGePoint3d& pickPoint,
AcGePoint3dArray& snapPoints) const
{
AcGePoint3d ll, ur, cen;
AcGeVector3d vec;
double len, ht;

assertReadEnabled();

// Get the window length, height and start point
len = getWindowLength();
ht	= getWindowHeight();
ll	= getWindowStartPoint();

// Calculate the other 3 corner points
// from ll
vec.set(len, ht, 0);
ur = ll + vec;

cen = ll + 0.5 * vec;

snapPoints.append(cen);

return Acad::eOk;
}



Acad::ErrorStatus EarthLine::moveStretchPointsAt(const AcDbIntArray& indices,
const AcGeVector3d& offset)
{
assertWriteEnabled();

//	moveStretchPointsAt == moveGripPointsAt
//
return moveGripPointsAt(indices, offset);
}*/

/*Acad::ErrorStatus EarthLine::transformBy(const AcGeMatrix3d& xform)
{
assertWriteEnabled();

AcGePoint3d ll;

ll = getEarthLineOriPoint();

ll.transformBy(xform);

setEarthLineOriPoint(ll);

return Acad::eOk;
}*/

void EarthLine_ROAD::subList() const
{
	int i;
	assertReadEnabled();

	AcDbEntity::list();

	ads_printf(L"\n\t\t自定义横断面地面线实体");
	ads_printf(L"\n\t\t里程 = %s %s", m_ckml,m_ZorY);
	ads_printf(L"\n\t\t地面点数 = %d", m_EarthPointNum);
	for(i=0; i<m_EarthPointNum; i++)
		ads_printf(L"\n\t\tx=%0.3lf y=%0.3lf", m_EarthPoints[i].x,m_EarthPoints[i].y);


}

int EarthLine_ROAD::Cal_Yd(double x,double &Yd,int &ptnum)
{
	int i;

	i = 0;
	Yd = m_EarthPoints[0].y;
	ptnum = 0;

	if(_tcscmp(m_ZorY,L"左侧")==0)
	{
		while(x < m_EarthPoints[i].x - 0.0001&& i<m_EarthPointNum)
			i++;
	}
	else
	{
		while(x > m_EarthPoints[i].x + 0.0001&& i<m_EarthPointNum)
			i++;
	}
	if(i==m_EarthPointNum)
	{
		Yd = m_EarthPoints[i-1].y;
		ptnum = i-1;
		return 0;//找不到
	}
	if(i==0)
		return -1;		

	ptnum = i;
	Yd = m_EarthPoints[i-1].y + (x-m_EarthPoints[i-1].x)*(m_EarthPoints[i].y-m_EarthPoints[i-1].y)/(m_EarthPoints[i].x-m_EarthPoints[i-1].x);
	return 1;//找到
}
void EarthLine_ROAD::Draw(int iColor, double NoteTextH)
{
	AcGePoint3dArray EarthPtArray;
	AcDbPolyline *pline;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"横断面图-地面线");

	assertReadEnabled();
	//绘设计线
	if(m_EarthPointNum<1)
		return;
	for(int i=0; i<m_EarthPointNum; i++)
		EarthPtArray.append(m_EarthPoints[i]);
	make2dline(EarthPtArray, iColor, 0.0, 0.0, LayerName);//3

	if (m_BenchPointNum>0)
	{
		AcGePoint3dArray BenchPtArray;
		for(int Lever=0; Lever<m_BenchPointNum/3; Lever++)
		{
			BenchPtArray.append(m_BenchPoints[Lever*3+0]);
			BenchPtArray.append(m_BenchPoints[Lever*3+1]);
			BenchPtArray.append(m_BenchPoints[Lever*3+2]);
			make2dline(BenchPtArray, iColor, 0.0, 0.0, LayerName);
			BenchPtArray.removeSubArray(0,2);
		}
	}



	if(_tcscmp(m_ZorY,L"左侧")==0)//只标一次 
	{
		ACHAR Str[50];
		_stprintf(Str,L"%0.3lf",EarH);//地面高 
		//bzPoint.x=m_EarthPoints[0].x-offset*Sfactor;
		NoteBG(bzpoint, NoteTextH/*0.4 * m_sfactor*/, Str, 6, 2, Sfactor, 2, LayerName);
	}
	if(HTHD>0.0001)
	{
		AcGePoint2d pt;

		pline = new AcDbPolyline;
		pline->setLinetype(L"ACAD_ISO03W100");//画一线
		pline->setLayer(L"横断面图-清表土");
		for(int i=0; i<m_EarthPointNum; i++)
		{
			pt.x = m_EarthPoints[i].x;
			pt.y = m_EarthPoints[i].y - HTHD*Sfactor; 
			pline->addVertexAt(i,pt);
		}
		AddEntityToDbs(pline) ;
	}
}
