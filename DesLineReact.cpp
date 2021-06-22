// DesLineReact.cpp: implementation of the DesLineReact class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "DesLineReact.h"
#include "HdmText.h"
#include "HdmDes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern double Sfactor;
extern HdmDes HDM;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DesLineReact::DesLineReact(AcDbObjectId LEarthId,AcDbObjectId REarthId,AcDbObjectId LTgouId,AcDbObjectId JSgouId,AcDbObjectId NoteId)
{
	m_LEarthObjId = LEarthId;
	m_REarthObjId = REarthId;
	m_GouObjId = LTgouId;
	m_JSgouId = JSgouId;
	m_NoteId = NoteId;
}

DesLineReact::~DesLineReact()
{

}

void DesLineReact::modified(const AcDbObject* pObj)
{
	LTGOU_ROAD *LTGou;
	JSGOU_ROAD *JSGou;
	EarthLine_ROAD *l_earthline;
	EarthLine_ROAD *r_earthline;
	DesignLine_ROAD *designline;
	HdmText_ROAD *Note;

	AcDbEntity *pEnt;
	AcGeVector3d transVec;
	AcGeMatrix3d mat;
	Acad::ErrorStatus es;
	ACHAR strError[80];
	//double FillORCut,DesPtList[50],gou[14],ZDK;
	//int DesPtSum;
	designline = DesignLine_ROAD::cast(pObj);

	if(!designline || designline->isErased())
	{
		const ACHAR *cstr = pObj->isA()->name();
		//20190715	��ʱ�ȹص���Щ��ʾ��ɾ���ݶ����ʱ��᲻�ϵ������û���е�Ī������
		//ads_printf(L"This is a %s.\n", cstr);
		//ads_printf(L"I only work with �����ʵ��. Sorry.\n");
		return;
	}
	//�򿪹����ġ��������ʵ�塿��ȡ���������
	es = acdbOpenObject(pEnt, m_LEarthObjId, AcDb::kForRead,true);

	if(es != Acad::eOk)
	{
		if(es == Acad::eWasOpenForWrite)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if(es == Acad::eWasNotifying)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}
		ads_printf(L"\nacdbOpenObject in LEarthObjId failed with error %d%s.\n",
			es, strError);
		//	pEnt->close();
		return;
	}

	l_earthline = EarthLine_ROAD::cast(pEnt);
	if (!l_earthline) 
	{
		const ACHAR *pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
		pEnt->close();
		return;
	}

	//�򿪹����ġ��ҵ�����ʵ�塿��ȡ���������
	es = acdbOpenObject(pEnt, m_REarthObjId, AcDb::kForRead,true);
	if(es != Acad::eOk)
	{
		if(es == Acad::eWasOpenForWrite)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if(es == Acad::eWasNotifying)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}

		//ads_printf(L"\nacdbOpenObject in REarthObjId failed with error %d%s.\n", es, strError);	//20190928	��ʱ�ر���ʾ

		//pEnt->close();
		l_earthline->close();

		return;
	}

	r_earthline = EarthLine_ROAD::cast(pEnt);
	if (!r_earthline) 
	{
		const ACHAR *pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
		pEnt->close();
		l_earthline->close();

		return;
	}


	//�򿪹����ġ�·�̹�ʵ�塿��ȡ����
	es = acdbOpenObject(pEnt, m_GouObjId, AcDb::kForWrite,true);
	if(es != Acad::eOk)
	{
		LTGou=NULL;
		if(es == Acad::eWasOpenForWrite)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if(es == Acad::eWasNotifying)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}
		//ads_printf(L"\nacdbOpenObject in m_GouObjId failed with error %d%s.\n", es, strError);	//20190928	��ʱ�ر���ʾ
		//	pEnt->close();
		l_earthline->close();
		r_earthline->close();

		return;
	}

	LTGou = LTGOU_ROAD::cast(pEnt);
	if (!LTGou) 
	{
		const ACHAR *pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to ˮ��ʵ��. Sorry.\n");
		pEnt->close();
		l_earthline->close();
		r_earthline->close();

		return;
	}

	//�򿪹����ġ��칵ʵ�塿��ȡ����
	es = acdbOpenObject(pEnt,m_JSgouId,AcDb::kForWrite,true);
	if(es != Acad::eOk)
	{
		if(es == Acad::eWasOpenForWrite)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if(es == Acad::eWasNotifying)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}
		//ads_printf(L"\nacdbOpenObject in m_JSgouId failed with error %d%s.\n", es, strError);		//20190928	��ʱ�ر���ʾ
		l_earthline->close();
		r_earthline->close();
		LTGou->close();

		//pEnt->close();
		return;
	}

	JSGou=JSGOU_ROAD::cast(pEnt);
	if(!JSGou)
	{
		const ACHAR *pCstr=pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to �칵ʵ��. Sorry.\n");
		pEnt->close();
		l_earthline->close();
		r_earthline->close();
		LTGou->close();

		return;
	}

	//�򿪹����ġ���עʵ�塿��ȡ����
	es = acdbOpenObject(pEnt, m_NoteId, AcDb::kForWrite,true);
	if(es != Acad::eOk)
	{
		if(es == Acad::eWasOpenForWrite)
		{
			_tcscpy(strError, L" (open for write)");
		}
		else if(es == Acad::eWasNotifying)
		{
			_tcscpy(strError, L" (open for notify)");
		}
		else
		{
			_tcscpy(strError, L"");
		}
		//ads_printf(L"\nacdbOpenObject in m_NoteId failed with error %d%s.\n", es, strError);		//20190928	��ʱ�ر���ʾ
		//pEnt->close();
		l_earthline->close();
		r_earthline->close();
		LTGou->close();
		JSGou->close();

		return;
	}
	Note = HdmText_ROAD::cast(pEnt);
	if (!Note) 
	{
		const ACHAR *pCstr = pEnt->isA()->name();
		ads_printf(L"This reactor is attached to a %s.\n", pCstr);
		ads_printf(L"It should only be attached to RectWindow custom Entities. Sorry.\n");
		pEnt->close();
		l_earthline->close();
		r_earthline->close();
		LTGou->close();
		JSGou->close();

		return;
	}

	//������д
	l_earthline->assertReadEnabled();
	r_earthline->assertReadEnabled();
	LTGou->assertWriteEnabled();	
	JSGou->assertWriteEnabled();
	Note->assertWriteEnabled();

	//20190928����	
	JSGou->CheckNSetGutterStatus();
	if (designline->m_GutterType == 2)	//·��ˮ��
	{
		designline->m_GutterType = -1;
		LTGou->SetGutterPara(designline->m_LDG_T2W, designline->m_LDG_T1W, designline->m_LDG_T2H, designline->m_LDG_GH, designline->m_LDG_GW, designline->m_LDG_HH,
			designline->m_LDG_SP1, designline->m_LDG_SP2, designline->m_LDG_SP3, designline->m_LDG_JQHD, designline->m_LDG_GDR);
		//LTGou->GouProperty = 1;
		LTGou->GouProperty = 3;	//20200430	�����µ�״̬-�޸Ĺ���·��ˮ��
	}
	else if (designline->m_GutterType == 3)	//�칵
	{
		designline->m_GutterType = -1;
		JSGou->SetGutterPara(designline->m_TG_BJ, designline->m_TG_GW, designline->m_TG_GS, designline->m_TG_SP1, designline->m_TG_SP2, designline->m_TG_JQHD, designline->m_TG_GDR);

		LTGou->GouProperty = 4;	//20200430	�����µ�״̬-�޸Ĺ����칵
	}

	//�����������Զ����
	HDM.SDM_AutoDesign(designline,l_earthline,LTGou,JSGou,r_earthline,Note);

	l_earthline->close();
	r_earthline->close();
	JSGou->close();
	Note->close();	
	LTGou->close();	
}






















