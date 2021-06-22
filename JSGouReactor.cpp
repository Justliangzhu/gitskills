#include "stdafx.h"
#include "JSGouReactor.h"
#include "resource.h"
#include "LTGou.h"
#include "HdmDes.h"
#include "JSGOU.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern double Sfactor;
extern HdmDes HDM;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JSGouReactor::JSGouReactor(AcDbObjectId LEarthId, AcDbObjectId REarthId, DesignLine_ROAD* designLine, AcDbObjectId LTGouId, AcDbObjectId NoteId)
{
	m_LEarthObjId = LEarthId;
	m_REarthObjId = REarthId;
	//	m_DesignlineId = designlineId;
	m_LTgouId = LTGouId;
	m_NoteId = NoteId;
	designline = designLine;
}

JSGouReactor::~JSGouReactor()
{
}

void JSGouReactor::modified(const AcDbObject* pObj)
{
	LTGOU_ROAD* LTGou;
	JSGOU_ROAD* JSGou;
	EarthLine_ROAD* l_earthline;
	EarthLine_ROAD* r_earthline;
	HdmText_ROAD* Note;

	AcDbEntity* pEnt;
	AcGeVector3d transVec;
	AcGeMatrix3d mat;
	Acad::ErrorStatus es;
	ACHAR strError[80];
	//double FillORCut,DesPtList[50],gou[14],ZDK;
	//int DesPtSum;

	JSGou = JSGOU_ROAD::cast(pObj);
	if (!JSGou || JSGou->isErased())
	{
		return;
	}

	es = acdbOpenObject(pEnt, m_LEarthObjId, AcDb::kForWrite, true);//打开关联的地面线实体读取地面点数据
	if (es != Acad::eOk)
	{
		if (es == 83)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if (es == 85)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}
		//ads_printf(L"\nacdbOpenObject in LEarthObjId failed with error %d%s.\n", es, strError);	//20190928	暂时关闭提示
		//	pEnt->close();
		return;
	}

	l_earthline = EarthLine_ROAD::cast(pEnt);
	if (!l_earthline)
	{
		const ACHAR* pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
		pEnt->close();
		return;
	}
	es = acdbOpenObject(pEnt, m_REarthObjId, AcDb::kForWrite, true);//打开关联的地面线实体读取地面点数据
	if (es != Acad::eOk)
	{
		if (es == 83)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if (es == 85)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}

		//ads_printf(L"\nacdbOpenObject in REarthObjId failed with error %d%s.\n", es, strError);	//20190928	暂时关闭提示
		l_earthline->close();
		//pEnt->close();

		return;
	}

	r_earthline = EarthLine_ROAD::cast(pEnt);
	if (!r_earthline)
	{
		const ACHAR* pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
		pEnt->close();
		l_earthline->close();

		return;
	}

	//	es = acdbOpenObject(pEnt, m_DesignlineId, AcDb::kForRead,true);//打开关联的设计线实体读取地面点数据
	//	if(es != Acad::eOk)
	//	{
	//		designline=NULL;
	//		if(es == 83)
	//		{
	//			_tcscpy(strError, L" (open for write)");
	//		}
	//		else if(es == 85)
	//		{
	//			_tcscpy(strError, L" (open for notify)");
	//		}
	//		else
	//		{
	//			_tcscpy(strError, L"");
	//		}
	//		ads_printf(L"\nacdbOpenObject in m_DesignlineId failed with error %d%s.\n",
	//			es, strError);
	//	//	pEnt->close();
	//		return;
	//	}
	//	
	//	designline = DesignLine::cast(pEnt);
	//	if (!designline) {
	//		const char *pCstr = pEnt->isA()->name();
	//		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
	//		ads_printf(L"It should only be attached to 设计线实体. Sorry.\n");
	//		pEnt->close();
	//		return;
	//	}
	//	

	es = acdbOpenObject(pEnt, m_LTgouId, AcDb::kForWrite, true);
	if (es != Acad::eOk)
	{
		if (es == 83)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if (es == 85)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}
		//ads_printf(L"\nacdbOpenObject in m_JSgouId failed with error %d%s.\n", es, strError);		//20190928	暂时关闭提示
		l_earthline->close();
		r_earthline->close();
		//pEnt->close();
		return;
	}

	LTGou = LTGOU_ROAD::cast(pEnt);
	if (!LTGou)
	{
		const ACHAR* pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to 天沟实体. Sorry.\n");
		pEnt->close();
		l_earthline->close();
		r_earthline->close();

		return;
	}
	es = acdbOpenObject(pEnt, m_NoteId, AcDb::kForWrite, true);
	if (es != Acad::eOk)
	{
		if (es == 83)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if (es == 85)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}
		//ads_printf(L"\nacdbOpenObject in m_NoteId failed with error %d%s.\n", es, strError);	//20190928	暂时关闭提示
		l_earthline->close();
		r_earthline->close();
		LTGou->close();
		//pEnt->close();
		return;
	}
	Note = HdmText_ROAD::cast(pEnt);
	if (!Note)
	{
		const ACHAR* pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
		pEnt->close();
		l_earthline->close();
		r_earthline->close();
		LTGou->close();

		return;
	}

	l_earthline->assertWriteEnabled();
	r_earthline->assertWriteEnabled();
	//	designline->assertReadEnabled();	
	LTGou->assertWriteEnabled();
	Note->assertWriteEnabled();

	//ads_printf(L"%s , 设计点个数: %d\n",earthline->m_ckml,designline->m_DesignPointNum);
	HDM.SDM_AutoDesign(designline, l_earthline, LTGou, JSGou, r_earthline, Note);

	l_earthline->close();
	r_earthline->close();
	//	designline->close();
	LTGou->close();
	Note->close();
}
