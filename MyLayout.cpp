// MyLayout.cpp: implementation of the CMyLayout class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MyLayout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyLayout::CMyLayout()
{

}

CMyLayout::~CMyLayout()
{

}

Acad::ErrorStatus CMyLayout::CreateLayout(ACHAR LayoutName[])
{
	AcDbObjectId layoutId,blockTableRecId;

	AcDbLayoutManager *pLayoutMan=acdbHostApplicationServices()->layoutManager();
	Acad::ErrorStatus es=pLayoutMan->createLayout(LayoutName,layoutId,blockTableRecId);
	pLayoutMan->setCurrentLayout(LayoutName);

	return es;
}

AcDbObjectId CMyLayout::CreateViewInLayout(ACHAR LayoutName[],AcGePoint3dArray Pt3dAy,double lw,bool ShowEdge,bool Lock)
{	
	AcDbLayoutManager *pLayoutMan=acdbHostApplicationServices()->layoutManager();
	pLayoutMan->setCurrentLayout(LayoutName);

	AcDbViewport *pViewport=new AcDbViewport;

	AcDb2dPolyline *Poly=new AcDb2dPolyline(AcDb::k2dSimplePoly,Pt3dAy,0,Adesk::kTrue,lw,lw);
	AcDbObjectId ClipPolyId=BAS_DRAW_FUN::AddEntityToDbs(Poly,0,1);
	//	pViewport->setNonRectClipOn();
	if(Acad::eOk==pViewport->setNonRectClipEntityId(ClipPolyId))
	{
		AcGePoint3d Center(410.0/2.0,297.0/2.0,0);
		//pViewport->setCenterPoint(Center);
		//pViewport->updateDisplay();
		//if(ShowEdge);
		//if(Lock)pViewport->setLocked();
		return BAS_DRAW_FUN::AddEntityToDbs(pViewport,0,1);
	}
	else delete pViewport;		
}
