// DMXReactor.cpp: implementation of the DMXReactor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DMXReactor.h"
#include "EarhBz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DMXReactor::DMXReactor(AcDbObjectId ObjId)
{
	m_ObjId = ObjId;
}

DMXReactor::~DMXReactor()
{

}

void DMXReactor::openedForModify(const AcDbObject* pObj)
{
}
void DMXReactor::modified(const AcDbObject* pObj)
{
	Acad::ErrorStatus es;
	ACHAR strError[80];
	AcDbEntity *pEnt;

	//open space;
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//修改Bri数据；
	es = acdbOpenObject(pEnt, m_ObjId, AcDb::kForWrite);//打开关联的实体BRIorTun修改数据
	if(es != Acad::eOk)
	{
		if(es == 83)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if(es == 85)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}
		return;

	}	

	EarhBz_ROAD *pEarh;
	pEarh = EarhBz_ROAD::cast(pEnt);
	if (!pEarh) 
	{
		const ACHAR *pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
		pEnt->close();
		return;
	}
	pEarh->assertWriteEnabled(Adesk::kTrue,Adesk::kTrue);	     
	pEarh->close();//?worlddraw() don't be load.????????

	es=acDocManager->unlockDocument(acDocManager->curDocument());
}

void DMXReactor::modifyUndone(const AcDbObject* pObj)
{
	Acad::ErrorStatus es;

	AcDbEntity *pEnt;
	ACHAR strError[80];
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//修改BRI TUN数据；
	es = acdbOpenObject(pEnt, m_ObjId, AcDb::kForWrite);//打开关联的实体DMX修改数据
	if(es != Acad::eOk)
	{
		if(es == 83)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if(es == 85)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}

		return;
	}	

	EarhBz_ROAD *pEarh;
	pEarh = EarhBz_ROAD::cast(pEnt);
	if (!pEarh) 
	{
		const ACHAR *pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
		pEnt->close();
		return;
	}
	//			ads_alert(L"0");
	pEarh->close();//?worlddraw() don't be load.????????*/
	//		    acedCommandS(RTSTR,"REGEN",RTSTR,"",RTNONE);
	es=acDocManager->unlockDocument(acDocManager->curDocument());
}

