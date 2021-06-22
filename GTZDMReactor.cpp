// GTZDMReactor.cpp: implementation of the GTZDMReactor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdarx.h"
#include "resource.h"
#include "GTZDM.h"
#include "GTZDMreactor.h"
#include "acdocman.h"
#include "BRI.h"
#include "TUN.h"
#include "DeshBz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GTZDMReactor::GTZDMReactor(AcDbObjectId ObjId,ACHAR BorT)
{
	m_ObjId=ObjId;	
	m_BorT = BorT;
}

GTZDMReactor::~GTZDMReactor()
{

}
void GTZDMReactor::openedForModify(const AcDbObject* pObj)
{
}
void GTZDMReactor::modified(const AcDbObject* pObj)
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
	if(m_BorT=='B')
	{
		BRI_ROAD *pBri;
		pBri = BRI_ROAD::cast(pEnt);
		if (!pBri) 
		{
			const ACHAR *pCstr = pEnt->isA()->name();
			ads_printf(L"This reactor is attached to a %s.\n", pCstr);
			ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
			pEnt->close();
			return;
		}
		pBri->assertWriteEnabled(Adesk::kTrue,Adesk::kTrue);	
		pBri->close();//?worlddraw() don't be load.????????
	}
	else if(m_BorT=='T')
	{
		TUN_ROAD *pTun;
		pTun = TUN_ROAD::cast(pEnt);
		if (!pTun) 
		{
			const ACHAR *pCstr = pEnt->isA()->name();
			ads_printf(L"This reactor is attached to a %s.\n", pCstr);
			ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
			pEnt->close();
			return;
		}
		pTun->assertWriteEnabled(Adesk::kTrue,Adesk::kTrue);	
		pTun->close();//?worlddraw() don't be load.????????
	}
	else
	{
		DeshBz_ROAD *pDesh;
		pDesh = DeshBz_ROAD::cast(pEnt);
		if (!pDesh) 
		{
			const ACHAR *pCstr = pEnt->isA()->name();
			ads_printf(L"This reactor is attached to a %s.\n", pCstr);
			ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
			pEnt->close();
			return;
		}
		pDesh->assertWriteEnabled(Adesk::kTrue,Adesk::kTrue);	     
		pDesh->close();//?worlddraw() don't be load.????????
	}
	es=acDocManager->unlockDocument(acDocManager->curDocument());
}

void GTZDMReactor::modifyUndone(const AcDbObject* pObj)
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
	if(m_BorT=='B')
	{
		BRI_ROAD *pBri;
		pBri = BRI_ROAD::cast(pEnt);
		if (!pBri) 
		{
			const ACHAR *pCstr = pEnt->isA()->name();
			ads_printf(L"This reactor is attached to a %s.\n", pCstr);
			ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
			pEnt->close();
			return;
		}
		pBri->close();
	}
	else if(m_BorT=='T')
	{
		TUN_ROAD *pTun;
		pTun = TUN_ROAD::cast(pEnt);
		if (!pTun) 
		{
			const ACHAR *pCstr = pEnt->isA()->name();
			ads_printf(L"This reactor is attached to a %s.\n", pCstr);
			ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
			pEnt->close();
			return;
		}
		pTun->close();
	}
	else
	{
		DeshBz_ROAD *pDesh;
		pDesh = DeshBz_ROAD::cast(pEnt);
		if (!pDesh) 
		{
			const ACHAR *pCstr = pEnt->isA()->name();
			ads_printf(L"This reactor is attached to a %s.\n", pCstr);
			ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
			pEnt->close();
			return;
		}
		//			ads_alert(L"0");
		pDesh->close();//?worlddraw() don't be load.????????*/
		//		    acedCommandS(RTSTR,"REGEN",RTSTR,"",RTNONE);
	}
	es=acDocManager->unlockDocument(acDocManager->curDocument());
}
