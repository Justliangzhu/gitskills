#include "stdafx.h"
#include "pmpointer.h"
#include "XLDataBase_JYX.h"
#include "XLDataBase.h"
#include "BAS_DRAW_FUN.h"
#include "CWorkDir.h"

extern GTZX_JYX_ROAD *JiXianpGtzx ;
extern GTZX2_JYX_ROAD *ZuoXianpGtzx , *YouXianpGtzx ;
extern GTZX_JYX_ROAD* pGTZXArray[MAXGJD];

CArray<GTZXPoint,GTZXPoint> g_GTZXPtArray;
CArray<GTZX2Point,GTZX2Point> g_GTZX2PtArray;
CArray<GTZX_JYXPoint,GTZX_JYXPoint> g_GTZX_JYXPtArray;
CArray<GTZX2_JYXPoint,GTZX2_JYXPoint> g_GTZX2_JYXPtArray;

PmPointer::PmPointer(void)
{
}

PmPointer::~PmPointer(void)
{
}

GTZX_ROAD* PmPointer::AddSJJXToCADDBS(CString MdbName)
{
	GTZX_ROAD* pzLinep = NULL;
	//pzLinep = PmPointer::GetSJJX(MdbName);
	//if (pzLinep==NULL)
	//	return pzLinep;
	//if (pzLinep->isErased())
	//{
	//	int iXL = PmPointer::FindSJJX(MdbName);
	//	if (iXL>-1)
	//	{
	//		pzLinep = PmPointer::CreateSJJX(MdbName);
	//		if (pzLinep == NULL)
	//		{
	//			return  NULL;
	//		}
	//		g_GTZXPtArray[iXL] = pzLinep;
	//	}
	//}
	//AcDbObjectId  LId;
	//LId=AddEntityToDbs(pzLinep);
	return pzLinep;
}
GTZX_ROAD* PmPointer::AddSJJXToCADDBS1(CString MdbName,	bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD)
{
	GTZX_ROAD* pzLinep = NULL;
	//pzLinep = PmPointer::GetSJJX(MdbName);
	//if (pzLinep == NULL)
	//{
	//	return  NULL;
	//}
	//pzLinep->IsdrawTZD =  IsdrawTZD;
	//pzLinep->IsdrawLC =  IsdrawLC;
	//pzLinep->IsdrawXLM =  IsdrawXLM;
	//pzLinep->IsdrawBPD =  IsdrawBPD;
	//pzLinep->IsdrawJDLX =  IsdrawJDLX;
	//pzLinep->IsdrawJD =  IsdrawJD;
	//if (pzLinep==NULL)
	//	return pzLinep;
	//if (pzLinep->isErased())
	//{
	//	int iXL = PmPointer::FindSJJX(MdbName);
	//	if (iXL>-1)
	//	{
	//		pzLinep = PmPointer::CreateSJJX(MdbName);
	//		if (pzLinep == NULL)
	//		{
	//			return  NULL;
	//		}

	//		pzLinep->IsdrawTZD =  IsdrawTZD;
	//		pzLinep->IsdrawLC =  IsdrawLC;
	//		pzLinep->IsdrawXLM =  IsdrawXLM;
	//		pzLinep->IsdrawBPD =  IsdrawBPD;
	//		pzLinep->IsdrawJDLX =  IsdrawJDLX;
	//		pzLinep->IsdrawJD =  IsdrawJD;

	//		g_GTZXPtArray[iXL] = pzLinep;
	//	}
	//}
	//AcDbObjectId  LId;
	//LId=AddEntityToDbs(pzLinep);
	return pzLinep;
}


GTZX2_ROAD* PmPointer::AddSJEXToCADDBS1(CString MdbName, int zory,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD)
{
	GTZX2_ROAD* pzLinep = NULL;
	//pzLinep = PmPointer::GetSJEX(MdbName, zory);
	//if (pzLinep == NULL)
	//{
	//	return  NULL;
	//}
	//pzLinep->IsdrawTZD =  IsdrawTZD;
	//pzLinep->IsdrawLC =  IsdrawLC;
	//pzLinep->IsdrawXLM =  IsdrawXLM;
	//pzLinep->IsdrawBPD =  IsdrawBPD;
	//pzLinep->IsdrawJDLX =  IsdrawJDLX;
	//pzLinep->IsdrawJD =  IsdrawJD;
	//if (pzLinep==NULL)
	//	return pzLinep;
	//if (pzLinep->isErased())
	//{
	//	int iXL = PmPointer::FindSJEX(MdbName);
	//	if (iXL>-1)
	//	{
	//		pzLinep = PmPointer::CreateSJEX(MdbName, zory);
	//		if (pzLinep == NULL)
	//		{
	//			return NULL;
	//		}

	//		pzLinep->IsdrawTZD =  IsdrawTZD;
	//		pzLinep->IsdrawLC =  IsdrawLC;
	//		pzLinep->IsdrawXLM =  IsdrawXLM;
	//		pzLinep->IsdrawBPD =  IsdrawBPD;
	//		pzLinep->IsdrawJDLX =  IsdrawJDLX; 
	//		pzLinep->IsdrawJD =  IsdrawJD;
	//		g_GTZX2PtArray[iXL] = pzLinep;
	//	}
	//}
	//AcDbObjectId  LId;
	//LId=AddEntityToDbs(pzLinep);
	return pzLinep;
}

GTZX2_ROAD* PmPointer::AddSJEXToCADDBS(CString MdbName, int zory)
{
	GTZX2_ROAD* pzLinep = NULL;
	//pzLinep = PmPointer::GetSJEX(MdbName, zory);
	//if (pzLinep==NULL)
	//	return pzLinep;
	//if (pzLinep->isErased())
	//{
	//	int iXL = PmPointer::FindSJEX(MdbName);
	//	if (iXL>-1)
	//	{
	//		pzLinep = PmPointer::CreateSJEX(MdbName, zory);   
	//		if (pzLinep == NULL)
	//		{
	//			return  NULL;
	//		}
	//		g_GTZX2PtArray[iXL] = pzLinep;
	//	}
	//}
	//AcDbObjectId  LId;
	//LId=AddEntityToDbs(pzLinep);
	return pzLinep;
}
GTZX_JYX_ROAD* PmPointer::AddJYJXToCADDBS1(CString MdbName,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD)
{
	GTZX_JYX_ROAD* pzLinep = NULL;
	//pzLinep = PmPointer::GetJYJX(MdbName);
	//pzLinep->IsdrawTZD =  IsdrawTZD;
	//pzLinep->IsdrawLC =  IsdrawLC;
	//pzLinep->IsdrawXLM =  IsdrawXLM;
	//pzLinep->IsdrawBPD =  IsdrawBPD;
	//pzLinep->IsdrawJDLX =  IsdrawJDLX;
	//pzLinep->IsdrawJD =  IsdrawJD;
	//if (pzLinep==NULL)
	//	return pzLinep;
	//if (pzLinep->isErased())
	//{
	//	int iXL = PmPointer::FindJYJX(MdbName);
	//	if (iXL>-1)
	//	{
	//		pzLinep = PmPointer::CreateJYJX(MdbName);
	//		if (pzLinep == NULL)
	//		{
	//			return  NULL;
	//		}
	//		pzLinep->IsdrawTZD =  IsdrawTZD;
	//		pzLinep->IsdrawLC =  IsdrawLC;
	//		pzLinep->IsdrawXLM =  IsdrawXLM;
	//		pzLinep->IsdrawBPD =  IsdrawBPD;
	//		pzLinep->IsdrawJDLX =  IsdrawJDLX;
	//		pzLinep->IsdrawJD =  IsdrawJD;
	//		g_GTZX_JYXPtArray[iXL] = pzLinep;
	//	}
	//}
	//AcDbObjectId  LId;
	//LId=AddEntityToDbs(pzLinep);
	return pzLinep;
}

GTZX2_JYX_ROAD* PmPointer::AddJYEXToCADDBS1(CString MdbName, int zory,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD)
{
	GTZX2_JYX_ROAD* pzLinep = NULL;
	pzLinep = PmPointer::GetJYEX(MdbName,zory);
	pzLinep->IsdrawTZD =  IsdrawTZD;
	pzLinep->IsdrawLC =  IsdrawLC;
	pzLinep->IsdrawXLM =  IsdrawXLM;
	pzLinep->IsdrawBPD =  IsdrawBPD;
	pzLinep->IsdrawJDLX =  IsdrawJDLX;
	pzLinep->IsdrawJD =  IsdrawJD;
	if (pzLinep==NULL)
		return pzLinep;
	if (pzLinep->isErased())
	{
		int iXL = PmPointer::FindJYEX(MdbName);
		if (iXL>-1)
		{
			pzLinep = PmPointer::CreateJYEX(MdbName, zory);
			if (pzLinep == NULL)
			{
				return NULL;
			}
			pzLinep->IsdrawTZD =  IsdrawTZD;
			pzLinep->IsdrawLC =  IsdrawLC;
			pzLinep->IsdrawXLM =  IsdrawXLM;
			pzLinep->IsdrawBPD =  IsdrawBPD;
			pzLinep->IsdrawJDLX =  IsdrawJDLX;
			pzLinep->IsdrawJD =  IsdrawJD;
			g_GTZX2_JYXPtArray[iXL] = pzLinep;

		}
	}
	AcDbObjectId  LId;
	LId=AddEntityToDbs(pzLinep);
	return pzLinep;
}

GTZX_JYX_ROAD* PmPointer::AddJYJXToCADDBS(CString MdbName)
{
	GTZX_JYX_ROAD* pzLinep = NULL;
	pzLinep = PmPointer::GetJYJX(MdbName);
	if (pzLinep==NULL)
		return pzLinep;
	if (pzLinep->isErased())
	{
		int iXL = PmPointer::FindJYJX(MdbName);
		if (iXL>-1)
		{
			pzLinep = PmPointer::CreateJYJX(MdbName);
			if (pzLinep == NULL)
			{
				return  NULL;
			}
			g_GTZX_JYXPtArray[iXL] = pzLinep;
		}
	}
	AcDbObjectId  LId;
	LId=AddEntityToDbs(pzLinep);
	return pzLinep;
}

GTZX2_JYX_ROAD* PmPointer::AddJYEXToCADDBS(CString MdbName, int zory)
{
	GTZX2_JYX_ROAD* pzLinep = NULL;
	pzLinep = PmPointer::GetJYEX(MdbName,zory);
	if (pzLinep==NULL)
		return pzLinep;
	if (pzLinep->isErased())
	{
		int iXL = PmPointer::FindJYEX(MdbName);
		if (iXL>-1)
		{
			pzLinep = PmPointer::CreateJYEX(MdbName, zory);
			if (pzLinep == NULL)
			{
				return  NULL;
			}
			g_GTZX2_JYXPtArray[iXL] = pzLinep;
		}
	}
	AcDbObjectId  LId;
	LId=AddEntityToDbs(pzLinep);
	return pzLinep;
}
//��ȫ�����������������л���
int PmPointer::FindJYJX(CString mdbname)
{
	mdbname.MakeUpper();
	int i=0;
	for (i=0; i<g_GTZX_JYXPtArray.GetSize(); i++)
	{
		if (g_GTZX_JYXPtArray[i]->mdbname == mdbname)
		{
			return i;
		}
	}
	if (i == g_GTZX_JYXPtArray.GetSize())
	{
		return -1;
	}
}

//��ȫ�����������������ж���
int PmPointer::FindJYEX(CString mdbname)
{
	mdbname.MakeUpper();
	int i=0;
	for (i=0; i<g_GTZX2_JYXPtArray.GetSize(); i++)
	{
		if (g_GTZX2_JYXPtArray[i]->mdbname == mdbname)
		{
			return i;
		}
	}
	if (i == g_GTZX2_JYXPtArray.GetSize())
	{
		return -1;
	}
}

//��ȫ����������������ƻ���
int PmPointer::FindSJJX(CString mdbname)
{
	mdbname.MakeUpper();
	int i=0;
	for (i=0; i<g_GTZXPtArray.GetSize(); i++)
	{
		//ads_printf(L"%s  %s\n",g_GTZXPtArray[i]->mdbname,mdbname);
		if (g_GTZXPtArray[i]->mdbname == mdbname)
		{
			return i;
		}
	}
	if (i == g_GTZXPtArray.GetSize())
	{
		return -1;
	}
}

//��ȫ����������������ƶ���
int PmPointer::FindSJEX(CString mdbname)
{
	mdbname.MakeUpper();
	int i=0;
	for (i=0; i<g_GTZX2PtArray.GetSize(); i++)
	{
		if (g_GTZX2PtArray[i]->mdbname == mdbname)
		{
			return i;
		}
	}
	if (i == g_GTZX2PtArray.GetSize())
	{
		return -1;
	}
}

//�����л��߼���ȫ������
void PmPointer::AddJYJX(GTZX_JYX_ROAD* pJYJX)
{
	pJYJX->mdbname.MakeUpper();
	int i=0;
	for (i=0; i<g_GTZX_JYXPtArray.GetSize(); i++)
	{
		if (g_GTZX_JYXPtArray[i]->mdbname == pJYJX->mdbname)
		{
			return;
		}
	}
	if (i == g_GTZX_JYXPtArray.GetSize())
	{
		g_GTZX_JYXPtArray.Add(pJYJX);
	}
}

//�����ж��߼���ȫ������
void PmPointer::AddJYEX(GTZX2_JYX_ROAD* pJYEX)
{
	pJYEX->mdbname.MakeUpper();
	int i=0;
	for (i=0; i<g_GTZX2_JYXPtArray.GetSize(); i++)
	{
		if (g_GTZX2_JYXPtArray[i]->mdbname == pJYEX->mdbname)
		{
			return;
		}
	}
	if (i == g_GTZX2_JYXPtArray.GetSize())
	{
		g_GTZX2_JYXPtArray.Add(pJYEX);
	}
}

//����ƻ��߼���ȫ������
void PmPointer::AddSJJX(GTZX_ROAD* pSJJX)
{
	pSJJX->mdbname.MakeUpper();
	int i=0;
	for (i=0; i<g_GTZXPtArray.GetSize(); i++)
	{
		if (g_GTZXPtArray[i]->mdbname == pSJJX->mdbname)
		{
			return;
		}
	}
	if (i == g_GTZXPtArray.GetSize())
	{
		g_GTZXPtArray.Add(pSJJX);
	}
}

//����ƶ��߼���ȫ������
void PmPointer::AddSJEX(GTZX2_ROAD* pSJEX)
{
	pSJEX->mdbname.MakeUpper();
	int i=0;
	for (i=0; i<g_GTZX2PtArray.GetSize(); i++)
	{
		if (g_GTZX2PtArray[i]->mdbname == pSJEX->mdbname)
		{
			return;
		}
	}
	if (i == g_GTZX2PtArray.GetSize())
	{
		g_GTZX2PtArray.Add(pSJEX);
	}
}

bool PmPointer::CheckOneXLConstraint(GTZX_ROAD* pCurXLPM, GTZX_ROAD* pRelXLPM, double MinAng, double MinDetaH,
									 CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray)
{
	//ConstraintErrArray.RemoveAll();
	//AcGePoint2dArray InterPtArray;
	////pCurXLPM->CalXLjd(pCurXLPM->array, pCurXLPM->tarray, pRelXLPM->array, pRelXLPM->tarray, &InterPtArray);
	//pCurXLPM->CheckIfXLintersect(pCurXLPM->array,pCurXLPM->JDarray,pCurXLPM->NJD,pRelXLPM->array,pRelXLPM->JDarray,pRelXLPM->NJD,&InterPtArray);
	//int InterPtSum = InterPtArray.length();
	//BAS_DRAW_FUN_RAIL::xlpoint PZ;
	//BAS_DRAW_FUN_RAIL::ConstraintErr TempErr;
	//double Curcml,RelCml;
	//double CurSlope,RelSlope;
	//CString ErrorStr;
	//for (int i=0; i<InterPtSum; i++)
	//{
	//	bool IsMinAng = false;
	//	bool IsMinH =false;

	//	AcGePoint2d Pt = InterPtArray.at(i);
	//	pCurXLPM->TrsNEtoCml(Pt.x, Pt.y, Curcml);
	//	pCurXLPM->TrsCmltoCkml(Curcml, TempErr.CurCkml);

	//	pRelXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
	//	pRelXLPM->TrsCmltoCkml(RelCml, TempErr.InterCkml);

	//	TempErr.JiaJiao = 0.0;
	//	TempErr.DetaH = 0.0;
	//	TempErr.InterN = Pt.x;
	//	TempErr.interE = Pt.y;
	//	TempErr.MinAng = MinAng;
	//	TempErr.MinDetaH = MinDetaH;

	//	PZ.lc = Curcml;
	//	pCurXLPM->xlpoint_pz(pCurXLPM->array, pCurXLPM->tarray, &PZ);
	//	TempErr.CurFwj = PZ.a;
	//	PZ.lc = RelCml;
	//	pRelXLPM->xlpoint_pz(pRelXLPM->array, pRelXLPM->tarray, &PZ);
	//	TempErr.InterFwj = PZ.a;
	//	TempErr.JiaJiao = pCurXLPM->CalJiaJiao(TempErr.CurFwj, TempErr.InterFwj);
	//	TempErr.JiaJiao = dms_rad(TempErr.JiaJiao);

	//	TempErr.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
	//	TempErr.InterDesH = pRelXLPM->rdes_level(RelCml, RelSlope);
	//	TempErr.DetaH = TempErr.CurDesH - TempErr.InterDesH;

	//	ConstraintErrArray.Add(TempErr);
	//}
	//if (ConstraintErrArray.GetSize()==0)
	//	return true;
	//else
	return false;

}
bool PmPointer::CheckOneXLConstraint(GTZX_ROAD* pCurXLPM, GTZX2_ROAD* pRelXLPM, double MinAng, double MinDetaH,
									 CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray)
{
	//ConstraintErrArray.RemoveAll();
	//AcGePoint2dArray InterPtArray;
	////pCurXLPM->CalXLjd(pCurXLPM->array, pCurXLPM->tarray, pRelXLPM->array, pRelXLPM->tarray, &InterPtArray);
	//pCurXLPM->CheckIfXLintersect(pCurXLPM->array,pCurXLPM->JDarray,pCurXLPM->NJD,pRelXLPM->array,pRelXLPM->JDarray,pRelXLPM->NJD,&InterPtArray);
	//int InterPtSum = InterPtArray.length();
	//BAS_DRAW_FUN_RAIL::xlpoint PZ;
	//BAS_DRAW_FUN_RAIL::ConstraintErr TempErr;
	//double Curcml,RelCml;
	//double CurSlope,RelSlope;
	//CString ErrorStr;
	//for (int i=0; i<InterPtSum; i++)
	//{
	//	bool IsMinAng = false;
	//	bool IsMinH =false;

	//	AcGePoint2d Pt = InterPtArray.at(i);
	//	pCurXLPM->TrsNEtoCml(Pt.x, Pt.y, Curcml);
	//	pCurXLPM->TrsCmltoCkml(Curcml, TempErr.CurCkml);

	//	pRelXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
	//	pRelXLPM->TrsCmltoCkml(RelCml, TempErr.InterCkml);

	//	TempErr.JiaJiao = 0.0;
	//	TempErr.DetaH = 0.0;
	//	TempErr.InterN = Pt.x;
	//	TempErr.interE = Pt.y;
	//	TempErr.MinAng = MinAng;
	//	TempErr.MinDetaH = MinDetaH;

	//	PZ.lc = Curcml;
	//	pCurXLPM->xlpoint_pz(pCurXLPM->array, pCurXLPM->tarray, &PZ);
	//	TempErr.CurFwj = PZ.a;
	//	//PZ.lc = RelCml;
	//    pRelXLPM->TrsNEtoProjml(Pt.x, Pt.y,PZ.lc);
	//	pRelXLPM->xlpoint_pz(pRelXLPM->array, pRelXLPM->tarray, &PZ);
	//	TempErr.InterFwj = PZ.a;
	//	TempErr.JiaJiao = pCurXLPM->CalJiaJiao(TempErr.CurFwj, TempErr.InterFwj);
	//	TempErr.JiaJiao = dms_rad(TempErr.JiaJiao);

	//	TempErr.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
	//	TempErr.InterDesH = pRelXLPM->rdes_level(RelCml, RelSlope);
	//	TempErr.DetaH = TempErr.CurDesH - TempErr.InterDesH;

	//	ConstraintErrArray.Add(TempErr);
	//}
	//if (ConstraintErrArray.GetSize()==0)
	//	return true;
	//else
	return false;
}


bool PmPointer::CheckOneXLConstraint(GTZX_ROAD* pCurXLPM, GTZX_JYX_ROAD* pRelXLPM, double MinAng, double MinDetaH,
									 CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray)
{
	//double JDarray2[MAXJDNUM][6];
	//for (int i=0;i<pRelXLPM->NJD;i++)
	//{
	//	JDarray2[i][0]=i;
	//	JDarray2[i][1]=pRelXLPM->JDarray[i].N;
	//	JDarray2[i][2]=pRelXLPM->JDarray[i].E;
	//}
	//ConstraintErrArray.RemoveAll();
	//AcGePoint2dArray InterPtArray;
	////pCurXLPM->CalXLjd(pCurXLPM->array, pCurXLPM->tarray, pRelXLPM->array, pRelXLPM->tarray, &InterPtArray);
	//pCurXLPM->CheckIfXLintersect(pCurXLPM->array,pCurXLPM->JDarray,pCurXLPM->NJD,pRelXLPM->array,JDarray2,pRelXLPM->NJD,&InterPtArray);
	//int InterPtSum = InterPtArray.length();
	//BAS_DRAW_FUN_RAIL::xlpoint PZ;
	//BAS_DRAW_FUN_JYX::xlpoint PZ_JYX;
	//BAS_DRAW_FUN_RAIL::ConstraintErr TempErr;
	//double Curcml,RelCml;
	//double CurSlope,RelSlope;
	//CString ErrorStr;
	//for (int i=0; i<InterPtSum; i++)
	//{
	//	bool IsMinAng = false;
	//	bool IsMinH =false;

	//	AcGePoint2d Pt = InterPtArray.at(i);
	//	pCurXLPM->TrsNEtoCml(Pt.x, Pt.y, Curcml);
	//	pCurXLPM->TrsCmltoCkml(Curcml, TempErr.CurCkml);

	//	pRelXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
	//	pRelXLPM->TrsCmltoCkml(RelCml, TempErr.InterCkml);

	//	TempErr.JiaJiao = 0.0;
	//	TempErr.DetaH = 0.0;
	//	TempErr.InterN = Pt.x;
	//	TempErr.interE = Pt.y;
	//	TempErr.MinAng = MinAng;
	//	TempErr.MinDetaH = MinDetaH;

	//	PZ.lc = Curcml;
	//	pCurXLPM->xlpoint_pz(pCurXLPM->array, pCurXLPM->tarray, &PZ);
	//	TempErr.CurFwj = PZ.a;
	//	PZ_JYX.lc = RelCml;
	//	pRelXLPM->xlpoint_pz(pRelXLPM->array, pRelXLPM->tarray, &PZ_JYX);
	//	TempErr.InterFwj = PZ_JYX.a;
	//	TempErr.JiaJiao = pCurXLPM->CalJiaJiao(TempErr.CurFwj, TempErr.InterFwj);
	//	TempErr.JiaJiao = dms_rad(TempErr.JiaJiao);

	//	TempErr.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
	//	TempErr.InterDesH = pRelXLPM->rdes_level(RelCml, RelSlope);
	//	TempErr.DetaH = TempErr.CurDesH - TempErr.InterDesH;

	//	ConstraintErrArray.Add(TempErr);
	//}
	//if (ConstraintErrArray.GetSize()==0)
	//	return true;
	//else
	return false;
}

bool PmPointer::CheckOneXLConstraint(GTZX_ROAD* pCurXLPM, GTZX2_JYX_ROAD* pRelXLPM, double MinAng, double MinDetaH,
									 CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray)
{
	//double JDarray2[MAXJDNUM][6];
	//for (int i=0;i<pRelXLPM->NJD;i++)
	//{
	//	JDarray2[i][0]=i;
	//	JDarray2[i][1]=pRelXLPM->JDarray[i].N;
	//	JDarray2[i][2]=pRelXLPM->JDarray[i].E;
	//}
	//ConstraintErrArray.RemoveAll();
	//AcGePoint2dArray InterPtArray;
	////pCurXLPM->CalXLjd(pCurXLPM->array, pCurXLPM->tarray, pRelXLPM->array, pRelXLPM->tarray, &InterPtArray);
	//pCurXLPM->CheckIfXLintersect(pCurXLPM->array,pCurXLPM->JDarray,pCurXLPM->NJD,pRelXLPM->array,JDarray2,pRelXLPM->NJD,&InterPtArray);
	//   int InterPtSum = InterPtArray.length();
	//BAS_DRAW_FUN_RAIL::xlpoint PZ;
	//BAS_DRAW_FUN_JYX::xlpoint PZ_JYX;
	//BAS_DRAW_FUN_RAIL::ConstraintErr TempErr;
	//double Curcml,RelCml;
	//double CurSlope,RelSlope;
	//CString ErrorStr;
	//for (int i=0; i<InterPtSum; i++)
	//{
	//	bool IsMinAng = false;
	//	bool IsMinH =false;

	//	AcGePoint2d Pt = InterPtArray.at(i);
	//	pCurXLPM->TrsNEtoCml(Pt.x, Pt.y, Curcml);
	//	pCurXLPM->TrsCmltoCkml(Curcml, TempErr.CurCkml);

	//	pRelXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
	//	pRelXLPM->TrsCmltoCkml(RelCml, TempErr.InterCkml);

	//	TempErr.JiaJiao = 0.0;
	//	TempErr.DetaH = 0.0;
	//	TempErr.InterN = Pt.x;
	//	TempErr.interE = Pt.y;
	//	TempErr.MinAng = MinAng;
	//	TempErr.MinDetaH = MinDetaH;

	//	PZ.lc = Curcml;
	//	pCurXLPM->xlpoint_pz(pCurXLPM->array, pCurXLPM->tarray, &PZ);
	//	TempErr.CurFwj = PZ.a;
	//	//PZ_JYX.lc = RelCml;
	//	pRelXLPM->TrsNEtoProjml(Pt.x, Pt.y,PZ_JYX.lc);
	//	pRelXLPM->xlpoint_pz(pRelXLPM->array, pRelXLPM->tarray, &PZ_JYX);
	//	TempErr.InterFwj = PZ_JYX.a;
	//	TempErr.JiaJiao = pCurXLPM->CalJiaJiao(TempErr.CurFwj, TempErr.InterFwj);
	//	TempErr.JiaJiao = dms_rad(TempErr.JiaJiao);

	//	TempErr.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
	//	TempErr.InterDesH = pRelXLPM->rdes_level(RelCml, RelSlope);
	//	TempErr.DetaH = TempErr.CurDesH - TempErr.InterDesH;

	//	ConstraintErrArray.Add(TempErr);
	//}
	//if (ConstraintErrArray.GetSize()==0)
	//	return true;
	//else
	return false;
}

bool PmPointer::CheckOneXLConstraint(GTZX2_ROAD* pCurXLPM, GTZX_ROAD* pRelXLPM, double MinAng, double MinDetaH,
									 CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray)
{
	//ConstraintErrArray.RemoveAll();
	//AcGePoint2dArray InterPtArray;
	////pCurXLPM->CalXLjd(pCurXLPM->array, pCurXLPM->tarray, pRelXLPM->array, pRelXLPM->tarray, &InterPtArray);
	//pCurXLPM->CheckIfXLintersect(pCurXLPM->array,pCurXLPM->JDarray,pCurXLPM->NJD,pRelXLPM->array,pRelXLPM->JDarray,pRelXLPM->NJD,&InterPtArray);
	//int InterPtSum = InterPtArray.length();
	//BAS_DRAW_FUN_RAIL::xlpoint PZ;
	//BAS_DRAW_FUN_RAIL::ConstraintErr TempErr;
	//double Curcml,RelCml;
	//double CurSlope,RelSlope;
	//CString ErrorStr;
	//for (int i=0; i<InterPtSum; i++)
	//{
	//	bool IsMinAng = false;
	//	bool IsMinH =false;

	//	AcGePoint2d Pt = InterPtArray.at(i);
	//	pCurXLPM->TrsNEtoCml(Pt.x, Pt.y, Curcml);
	//	pCurXLPM->TrsCmltoCkml(Curcml, TempErr.CurCkml);

	//	pRelXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
	//	pRelXLPM->TrsCmltoCkml(RelCml, TempErr.InterCkml);

	//	TempErr.JiaJiao = 0.0;
	//	TempErr.DetaH = 0.0;
	//	TempErr.InterN = Pt.x;
	//	TempErr.interE = Pt.y;
	//	TempErr.MinAng = MinAng;
	//	TempErr.MinDetaH = MinDetaH;

	//	//PZ.lc = Curcml;
	//	pCurXLPM->TrsNEtoProjml(Pt.x, Pt.y,PZ.lc);
	//	pCurXLPM->xlpoint_pz(pCurXLPM->array, pCurXLPM->tarray, &PZ);
	//	TempErr.CurFwj = PZ.a;
	//	PZ.lc = RelCml;
	//	pRelXLPM->xlpoint_pz(pRelXLPM->array, pRelXLPM->tarray, &PZ);
	//	TempErr.InterFwj = PZ.a;
	//	TempErr.JiaJiao = pCurXLPM->CalJiaJiao(TempErr.CurFwj, TempErr.InterFwj);
	//	TempErr.JiaJiao = dms_rad(TempErr.JiaJiao);

	//	TempErr.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
	//	TempErr.InterDesH = pRelXLPM->rdes_level(RelCml, RelSlope);
	//	TempErr.DetaH = TempErr.CurDesH - TempErr.InterDesH;

	//	ConstraintErrArray.Add(TempErr);
	//}
	//if (ConstraintErrArray.GetSize()==0)
	//	return true;
	//else
	return false;

}
bool PmPointer::CheckOneXLConstraint(GTZX2_ROAD* pCurXLPM, GTZX2_ROAD* pRelXLPM, double MinAng, double MinDetaH,
									 CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray)
{
	//ConstraintErrArray.RemoveAll();
	//AcGePoint2dArray InterPtArray;
	////pCurXLPM->CalXLjd(pCurXLPM->array, pCurXLPM->tarray, pRelXLPM->array, pRelXLPM->tarray, &InterPtArray);
	//pCurXLPM->CheckIfXLintersect(pCurXLPM->array,pCurXLPM->JDarray,pCurXLPM->NJD,pRelXLPM->array,pRelXLPM->JDarray,pRelXLPM->NJD,&InterPtArray);
	//int InterPtSum = InterPtArray.length();
	//BAS_DRAW_FUN_RAIL::xlpoint PZ;
	//BAS_DRAW_FUN_RAIL::ConstraintErr TempErr;
	//double Curcml,RelCml;
	//double CurSlope,RelSlope;
	//CString ErrorStr;
	//for (int i=0; i<InterPtSum; i++)
	//{
	//	bool IsMinAng = false;
	//	bool IsMinH =false;

	//	AcGePoint2d Pt = InterPtArray.at(i);
	//	pCurXLPM->TrsNEtoCml(Pt.x, Pt.y, Curcml);
	//	pCurXLPM->TrsCmltoCkml(Curcml, TempErr.CurCkml);

	//	pRelXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
	//	pRelXLPM->TrsCmltoCkml(RelCml, TempErr.InterCkml);

	//	TempErr.JiaJiao = 0.0;
	//	TempErr.DetaH = 0.0;
	//	TempErr.InterN = Pt.x;
	//	TempErr.interE = Pt.y;
	//	TempErr.MinAng = MinAng;
	//	TempErr.MinDetaH = MinDetaH;

	//	//PZ.lc = Curcml;
	//	pCurXLPM->TrsNEtoProjml(Pt.x, Pt.y,PZ.lc);
	//	pCurXLPM->xlpoint_pz(pCurXLPM->array, pCurXLPM->tarray, &PZ);
	//	TempErr.CurFwj = PZ.a;
	//	//PZ.lc = RelCml;
	//	pRelXLPM->TrsNEtoProjml(Pt.x, Pt.y,PZ.lc);
	//	pRelXLPM->xlpoint_pz(pRelXLPM->array, pRelXLPM->tarray, &PZ);
	//	TempErr.InterFwj = PZ.a;
	//	TempErr.JiaJiao = pCurXLPM->CalJiaJiao(TempErr.CurFwj, TempErr.InterFwj);
	//	TempErr.JiaJiao = dms_rad(TempErr.JiaJiao);

	//	TempErr.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
	//	TempErr.InterDesH = pRelXLPM->rdes_level(RelCml, RelSlope);
	//	TempErr.DetaH = TempErr.CurDesH - TempErr.InterDesH;

	//	ConstraintErrArray.Add(TempErr);
	//}
	//if (ConstraintErrArray.GetSize()==0)
	//	return true;
	//else
	return false;
}
bool PmPointer::CheckOneXLConstraint(GTZX2_ROAD* pCurXLPM, GTZX_JYX_ROAD* pRelXLPM, double MinAng, double MinDetaH,
									 CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray)
{
	//double JDarray2[MAXJDNUM][6];
	//for (int i=0;i<pRelXLPM->NJD;i++)
	//{
	//	JDarray2[i][0]=i;
	//	JDarray2[i][1]=pRelXLPM->JDarray[i].N;
	//	JDarray2[i][2]=pRelXLPM->JDarray[i].E;
	//}
	//ConstraintErrArray.RemoveAll();
	//AcGePoint2dArray InterPtArray;
	////pCurXLPM->CalXLjd(pCurXLPM->array, pCurXLPM->tarray, pRelXLPM->array, pRelXLPM->tarray, &InterPtArray);
	//pCurXLPM->CheckIfXLintersect(pCurXLPM->array,pCurXLPM->JDarray,pCurXLPM->NJD,pRelXLPM->array,JDarray2,pRelXLPM->NJD,&InterPtArray);
	//int InterPtSum = InterPtArray.length();
	//BAS_DRAW_FUN_RAIL::xlpoint PZ;
	//BAS_DRAW_FUN_JYX::xlpoint PZ_JYX;
	//BAS_DRAW_FUN_RAIL::ConstraintErr TempErr;
	//double Curcml,RelCml;
	//double CurSlope,RelSlope;
	//CString ErrorStr;
	//for (int i=0; i<InterPtSum; i++)
	//{
	//	bool IsMinAng = false;
	//	bool IsMinH =false;

	//	AcGePoint2d Pt = InterPtArray.at(i);
	//	pCurXLPM->TrsNEtoCml(Pt.x, Pt.y, Curcml);
	//	pCurXLPM->TrsCmltoCkml(Curcml, TempErr.CurCkml);

	//	pRelXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
	//	pRelXLPM->TrsCmltoCkml(RelCml, TempErr.InterCkml);

	//	TempErr.JiaJiao = 0.0;
	//	TempErr.DetaH = 0.0;
	//	TempErr.InterN = Pt.x;
	//	TempErr.interE = Pt.y;
	//	TempErr.MinAng = MinAng;
	//	TempErr.MinDetaH = MinDetaH;

	//	//PZ.lc = Curcml;
	//	pCurXLPM->TrsNEtoProjml(Pt.x, Pt.y,PZ.lc);
	//	pCurXLPM->xlpoint_pz(pCurXLPM->array, pCurXLPM->tarray, &PZ);
	//	TempErr.CurFwj = PZ.a;
	//	PZ_JYX.lc = RelCml;
	//	pRelXLPM->xlpoint_pz(pRelXLPM->array, pRelXLPM->tarray, &PZ_JYX);
	//	TempErr.InterFwj = PZ_JYX.a;
	//	TempErr.JiaJiao = pCurXLPM->CalJiaJiao(TempErr.CurFwj, TempErr.InterFwj);
	//	TempErr.JiaJiao = dms_rad(TempErr.JiaJiao);

	//	TempErr.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
	//	TempErr.InterDesH = pRelXLPM->rdes_level(RelCml, RelSlope);
	//	TempErr.DetaH = TempErr.CurDesH - TempErr.InterDesH;

	//	ConstraintErrArray.Add(TempErr);
	//}
	//if (ConstraintErrArray.GetSize()==0)
	//	return true;
	//else
	return false;
}

bool PmPointer::CheckOneXLConstraint(GTZX2_ROAD* pCurXLPM, GTZX2_JYX_ROAD* pRelXLPM, double MinAng, double MinDetaH,
									 CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray)
{
	//double JDarray2[MAXJDNUM][6];
	//for (int i=0;i<pRelXLPM->NJD;i++)
	//{
	//	JDarray2[i][0]=i;
	//	JDarray2[i][1]=pRelXLPM->JDarray[i].N;
	//	JDarray2[i][2]=pRelXLPM->JDarray[i].E;
	//}
	//ConstraintErrArray.RemoveAll();
	//AcGePoint2dArray InterPtArray;
	////pCurXLPM->CalXLjd(pCurXLPM->array, pCurXLPM->tarray, pRelXLPM->array, pRelXLPM->tarray, &InterPtArray);
	//pCurXLPM->CheckIfXLintersect(pCurXLPM->array,pCurXLPM->JDarray,pCurXLPM->NJD,pRelXLPM->array,JDarray2,pRelXLPM->NJD,&InterPtArray);
	//int InterPtSum = InterPtArray.length();
	//BAS_DRAW_FUN_RAIL::xlpoint PZ;
	//BAS_DRAW_FUN_JYX::xlpoint PZ_JYX;
	//BAS_DRAW_FUN_RAIL::ConstraintErr TempErr;
	//double Curcml,RelCml;
	//double CurSlope,RelSlope;
	//CString ErrorStr;
	//for (int i=0; i<InterPtSum; i++)
	//{
	//	bool IsMinAng = false;
	//	bool IsMinH =false;

	//	AcGePoint2d Pt = InterPtArray.at(i);
	//	pCurXLPM->TrsNEtoCml(Pt.x, Pt.y, Curcml);
	//	pCurXLPM->TrsCmltoCkml(Curcml, TempErr.CurCkml);

	//	pRelXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
	//	pRelXLPM->TrsCmltoCkml(RelCml, TempErr.InterCkml);

	//	TempErr.JiaJiao = 0.0;
	//	TempErr.DetaH = 0.0;
	//	TempErr.InterN = Pt.x;
	//	TempErr.interE = Pt.y;
	//	TempErr.MinAng = MinAng;
	//	TempErr.MinDetaH = MinDetaH;

	//	//PZ.lc = Curcml;
	//	pCurXLPM->TrsNEtoProjml(Pt.x, Pt.y,PZ.lc);
	//	pCurXLPM->xlpoint_pz(pCurXLPM->array, pCurXLPM->tarray, &PZ);
	//	TempErr.CurFwj = PZ.a;
	//	//PZ_JYX.lc = RelCml;
	//	pRelXLPM->TrsNEtoProjml(Pt.x, Pt.y,PZ_JYX.lc);
	//	pRelXLPM->xlpoint_pz(pRelXLPM->array, pRelXLPM->tarray, &PZ_JYX);
	//	TempErr.InterFwj = PZ_JYX.a;
	//	TempErr.JiaJiao = pCurXLPM->CalJiaJiao(TempErr.CurFwj, TempErr.InterFwj);
	//	TempErr.JiaJiao = dms_rad(TempErr.JiaJiao);

	//	TempErr.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
	//	TempErr.InterDesH = pRelXLPM->rdes_level(RelCml, RelSlope);
	//	TempErr.DetaH = TempErr.CurDesH - TempErr.InterDesH;

	//	ConstraintErrArray.Add(TempErr);
	//}
	//if (ConstraintErrArray.GetSize()==0)
	//	return true;
	//else
	return false;
}

//��ȡ���л��ߡ����ڴ��д����ڼ��л���ʱֱ�ӻ�ȡ��������ݿⴴ��
GTZX_JYX_ROAD* PmPointer::GetJYJX(CString mdbname)
{
	int i;
	mdbname.MakeUpper();
	for (i=0; i<g_GTZX_JYXPtArray.GetSize(); i++)
	{
		if (g_GTZX2_JYXPtArray[i]->mdbname == mdbname&&g_GTZX2_JYXPtArray[i]->XLShuXing=="���л���")
		{
			return g_GTZX_JYXPtArray[i];
		}
	}
	if (i == g_GTZX_JYXPtArray.GetSize())
	{
		GTZX_JYX_ROAD* pGTZXJYX = NULL;
		pGTZXJYX = CreateJYJX(mdbname);
		if (pGTZXJYX)
			g_GTZX_JYXPtArray.Add(pGTZXJYX);
		return pGTZXJYX;
	}
}

GTZX_JYX_ROAD* PmPointer::CreateJYJX(CString mdbname )
{

	mdbname.MakeUpper();
	int NJD,i;
	ACHAR GH[8],JDGH[8];
	CArray<GTZX_JYX_ROAD::JDarrayStruct,GTZX_JYX_ROAD::JDarrayStruct>array;
	array.RemoveAll();

	CString Temp1;

	Temp1 = "���л���";

	Temp1.Trim();
	Temp1=_T("��·��=\"") + Temp1 +_T("\"");
	XLDataBase_JYX xlmdb;
	if(xlmdb.Read_XLDbs(mdbname,L"���л��߽������ݱ�",Temp1))//������
	{  
		NJD = xlmdb.NJD;
		array.SetSize(NJD);
		for(i=0;i<NJD;i++)
		{
			_tcscpy(array[i].JDNum ,xlmdb.JDarray[i].JDNum);
			array[i].N = xlmdb.JDarray[i].N;
			array[i].E = xlmdb.JDarray[i].E;
			array[i].l1 = xlmdb.JDarray[i].l1;
			array[i].R = xlmdb.JDarray[i].R;
			array[i].l2 = xlmdb.JDarray[i].l2;
		}	
	}
	else
	{
		//ads_printf(L"\n���������ݱ�ʧ��!");
		return NULL;
	}
	if (NJD< 2) return NULL;
	_tcscpy(GH,L"AK"); 
	_tcscpy(JDGH,L"JD");
	GTZX_JYX_ROAD *pzx= new GTZX_JYX_ROAD(NJD,array,&GH[0],&JDGH[0]);
	pzx->setINIT(3,3,1,5);
	pzx->mode_display=0;
	pzx->mdbname = mdbname;
	pzx->ReadXLGuifan();

	if(xlmdb.Read_XLDbs(mdbname,L"���л��߶�����",Temp1))//������
	{
		pzx->NDL = xlmdb.NDL;
		for(i=0;i<pzx->NDL;i++)
		{
			_tcscpy(pzx->DLB[i].BGH,xlmdb.DLB[i].BGH);
			_tcscpy(pzx->DLB[i].EGH,xlmdb.DLB[i].EGH);
			pzx->DLB[i].BLC = xlmdb.DLB[i].BLC;
			pzx->DLB[i].DL = xlmdb.DLB[i].DL;

			pzx->DLB[i].N = xlmdb.DLB[i].N;
			pzx->DLB[i].E = xlmdb.DLB[i].E;
		}
		pzx->getpxy_DATA();
	}
	else
	{
		ads_printf(L"����GTZXʱ,û�ж�������!\n");
		return NULL;
	}

	if(xlmdb.Read_XLDbs(mdbname,L"���л��߱�־��̱�",Temp1))//����־���
	{
		pzx->NBZLC = xlmdb.BZLCNum;	
		pzx->BZLCarray.RemoveAll();
		pzx->BZLCarray.SetSize(pzx->NBZLC);
		for(int i=0;i<pzx->NBZLC;i++)
		{
			_tcscpy(pzx->BZLCarray[i].kml,xlmdb.BZLCTable[i].kml);
			pzx->BZLCarray[i].X = xlmdb.BZLCTable[i].X;
			pzx->BZLCarray[i].Y = xlmdb.BZLCTable[i].Y;
		}
	}
	pzx->SetBZLCVal();

	//////////////////////////////////////////////////////////////////////////
	CString Condition;
	Condition = Condition = "��·�� = \"���л���\"";
	if (xlmdb.Read_XLDbs(mdbname,L"�����¶ȱ�",Condition))
	{
		pzx->SetBPDArray(xlmdb.NPD, xlmdb.PDB, false);
		pzx->TrsBPDArraytoCM();
	}
	//////////////////////////////////////////////////////////////////////////
	pzx->IsReSetColor = FALSE ;
	pzx->setColorLineW();

	return pzx;
}

//��ȡ���Т��ߡ����ڴ��д����ڼ��Т���ʱֱ�ӻ�ȡ��������ݿⴴ��
GTZX2_JYX_ROAD* PmPointer::GetJYEX(CString mdbname, int zory)
{
	int i;
	for (i=0; i<g_GTZX2_JYXPtArray.GetSize(); i++)
	{
		if (g_GTZX2_JYXPtArray[i]->mdbname == mdbname&&_tcscmp(g_GTZX2_JYXPtArray[i]->xlname,L"���Т���")==0)
		{
			return g_GTZX2_JYXPtArray[i];
		}
	}
	if (i == g_GTZX2_JYXPtArray.GetSize())
	{
		GTZX2_JYX_ROAD* pGTZX2_JYX = NULL;
		pGTZX2_JYX = CreateJYEX(mdbname,zory);
		if (pGTZX2_JYX)
			g_GTZX2_JYXPtArray.Add(pGTZX2_JYX);
		return pGTZX2_JYX;
	}
}

GTZX2_JYX_ROAD *PmPointer::CreateJYEX(CString mdbname,int zory)
{
	GTZX2_JYX_ROAD *pzLinep2=NULL;
	mdbname.MakeUpper();
	int NJD,i;
	ACHAR GH[8],JDGH[8];
	CArray<GTZX_JYX_ROAD::JDarrayStruct,GTZX_JYX_ROAD::JDarrayStruct>array;
	array.RemoveAll();

	CString Temp1;
	Temp1 = "���л���";
	Temp1.Trim();
	Temp1=_T("��·��=\"") + Temp1 + "\"";
	XLDataBase_JYX xlmdb;
	if(xlmdb.Read_XLDbs(mdbname,L"�������ݱ�",Temp1))//������
	{  
		NJD = xlmdb.NJD;
		array.SetSize(NJD);
		for(i=0;i<NJD;i++)
		{
			_tcscpy(array[i].JDNum ,xlmdb.JDarray[i].JDNum);
			array[i].N = xlmdb.JDarray[i].N;
			array[i].E = xlmdb.JDarray[i].E;
			array[i].l1 = xlmdb.JDarray[i].l1;
			array[i].R = xlmdb.JDarray[i].R;
			array[i].l2 = xlmdb.JDarray[i].l2;
		}	
	}
	else
	{
		//ads_printf(L"\n���������ݱ�ʧ��!");
		return NULL;
	}
	if (NJD< 2) return NULL;
	_tcscpy(GH,L"AK"); 


	if(zory==-1) _tcscpy(JDGH,L"ZJD");
	else _tcscpy(JDGH,L"YJD");

	GTZX_JYX_ROAD *pzx= new GTZX_JYX_ROAD(NJD,array,&GH[0],&JDGH[0]);

	if(pzx==NULL)return NULL;

	pzx->setINIT(3,3,1,5);
	pzx->mode_display=0;
	pzx->mdbname = mdbname;
	pzx->ReadXLGuifan();

	if(xlmdb.Read_XLDbs(mdbname,L"������",Temp1))//������
	{
		pzx->NDL = xlmdb.NDL;
		for(i=0;i<pzx->NDL;i++)
		{
			_tcscpy(pzx->DLB[i].BGH,xlmdb.DLB[i].BGH);
			_tcscpy(pzx->DLB[i].EGH,xlmdb.DLB[i].EGH);
			pzx->DLB[i].BLC = xlmdb.DLB[i].BLC;
			pzx->DLB[i].DL = xlmdb.DLB[i].DL;

			pzx->DLB[i].N = xlmdb.DLB[i].N;
			pzx->DLB[i].E = xlmdb.DLB[i].E;
		}
	}
	pzx->getpxy_DATA();

	if(xlmdb.Read_XLDbs(mdbname,L"��־��̱�",Temp1))//����־���
	{
		pzx->NBZLC = xlmdb.BZLCNum;	
		pzx->BZLCarray.RemoveAll();
		pzx->BZLCarray.SetSize(pzx->NBZLC);
		for(int i=0;i<pzx->NBZLC;i++)
		{
			_tcscpy(pzx->BZLCarray[i].kml,xlmdb.BZLCTable[i].kml);
			pzx->BZLCarray[i].X = xlmdb.BZLCTable[i].X;
			pzx->BZLCarray[i].Y = xlmdb.BZLCTable[i].Y;
		}
	}
	pzx->SetBZLCVal();

	//////////////////////////////////////////////////////////////////////////
	if (xlmdb.Read_XLDbs(mdbname,L"�¶ȱ�",Temp1))
	{
		pzx->SetBPDArray(xlmdb.NPD, xlmdb.PDB, false);
		pzx->TrsBPDArraytoCM();
	}
	//////////////////////////////////////////////////////////////////////////

	if(zory == -1)
		Temp1 = "��������";
	else
		Temp1 = "��������";

	CString Temp2;
	Temp1.Trim();
	Temp2 = Temp1;
	Temp1=L"��·��=\"" + Temp1 + "\"";
	if(xlmdb.Read_XLDbs(mdbname,L"�������߱�",Temp1))//������
	{	
		pzLinep2= new GTZX2_JYX_ROAD(pzx,xlmdb.NQX2x,xlmdb.QXB2x,pzx->JD0,L"JD");

		if(pzLinep2==NULL)return NULL;
		pzLinep2->mdbname=mdbname;
		pzLinep2->ReadXLGuifan();

		_tcscpy( pzLinep2->xlname ,Temp2);
		if(xlmdb.Read_XLDbs(mdbname,L"���߶�����",Temp1))
		{
			pzLinep2->NDL=xlmdb.NDL2x;
			pzLinep2->SetDLB(xlmdb.DLB2x,xlmdb.NDL2x);	
		}
		else
			pzLinep2->Create2XGTDLB();

		CString XLName , Temp ;
		int RXDSum = pzLinep2->GetRXDNum();
		for(int i=0; i<RXDSum; i++)
		{
			XLName = pzLinep2->GetRXDXLNameByRXDIndex( i );

			Temp=L"��·��=\""+ XLName +"\"";
			//��־��̱�
			if(xlmdb.Read_XLDbs(mdbname,L"��־��̱�",Temp))//����־���
			{
				pzLinep2->SetBZLCArray(  xlmdb.BZLCTable , xlmdb.BZLCNum ) ;
			}

			if(xlmdb.BZLCNum<2)
			{
				CArray<BAS_DRAW_FUN_JYX::BZLCStructTable ,BAS_DRAW_FUN_JYX::BZLCStructTable>  BZLCarrayTmp ;
				pzLinep2->SetDefaultiRxdBZLC( i , BZLCarrayTmp );

				pzLinep2->SetBZLCArray(  BZLCarrayTmp ) ;
			}

			//////////////////////////////////////////////////////////////////////////
			CString Condition = L"��·�� = \"" + XLName + "\"";
			if (xlmdb.Read_XLDbs(mdbname,L"�¶ȱ�",Condition))
				pzLinep2->SetiRXDBPDArray(i, xlmdb.NPD, xlmdb.PDB, false);
			//////////////////////////////////////////////////////////////////////////

		}
		//�����־���
		pzLinep2->SetBZLCVal();
		pzLinep2->Create2XGTBPDArray();

		pzLinep2->IsReSetColor = FALSE ;
		pzLinep2->setColorLineW();

		pzLinep2->calsflag=false;		
	}
	else return NULL;

	return pzLinep2;
}

//��ȡ��ƻ��ߡ��������ƻ����Ѿ����ڴ�����ֱ��ȡ������������ݿⴴ��
GTZX_ROAD* PmPointer::GetSJJX(CString mdbname)
{
	mdbname.MakeUpper();
	int i;

	for (i=0; i<g_GTZXPtArray.GetSize(); i++)
	{
		if (g_GTZXPtArray[i]->mdbname == mdbname)
		{
			return g_GTZXPtArray[i];
		}
	}
	if (i == g_GTZXPtArray.GetSize())
	{
		GTZX_ROAD* pGTZX = NULL;
		pGTZX = CreateSJJX(mdbname);
		if (pGTZX)
			g_GTZXPtArray.Add(pGTZX);
		return pGTZX;
	}
}

GTZX_ROAD* PmPointer::CreateSJJX(CString MdbName)
{
	MdbName.MakeUpper();
	GTZX_ROAD* pzLinep = NULL;
	XLDataBase xlmdb;
	double arrayd[MAXJDNUM][6];
	int i,j;
	int NJD = 0;

	for (i=0;i<MAXJDNUM;i++)
	{
		for (j=0;j<6;j++)
			arrayd[i][j]=0.0;
	}

	if(xlmdb.Read_XLDbs_Railway(MdbName,L"���߱�"))//������
	{
		NJD = xlmdb.NQX;
		for(i=0;i<NJD;i++)
		{
			arrayd[i][0] = xlmdb.QXB[i].jdno;
			arrayd[i][1] = xlmdb.QXB[i].x;
			arrayd[i][2] = xlmdb.QXB[i].y;
			arrayd[i][3] = xlmdb.QXB[i].l1;
			arrayd[i][4] = xlmdb.QXB[i].R;
			arrayd[i][5] = xlmdb.QXB[i].l2;
		}
	}
	else
	{
		ads_printf(L"\n�����߱�ʧ��1!");
		return NULL;
	}

	ACHAR GH[8],JDGH[8];
	_tcscpy(JDGH,L"JD");

	if (NJD< 2) 
		return NULL;

	int BZW=3;
	int NLC=3; 
	int NH=3;

	int Texth=5;//plh

	_tcscpy(GH,L"AK"); 
	pzLinep= new GTZX_ROAD(NJD,arrayd,&GH[0],&JDGH[0]);

	////������
	pzLinep->mdbname=MdbName;
	if(xlmdb.Read_XLDbs_Railway(MdbName,L"������"))//������
	{
		pzLinep->assertWriteEnabled();
		pzLinep->NDL = xlmdb.NDL;
		for(i=0;i<pzLinep->NDL;i++)
			pzLinep->setDLM(i,xlmdb.DLArray[i].BLC,xlmdb.DLArray[i].DL,xlmdb.DLArray[i].BGH,xlmdb.DLArray[i].EGH);
	}

	pzLinep->setINIT(NLC,NH,1,Texth);
	pzLinep->mode_display=0;

	////////////////////////////////////////////////////////////////////////////
	////�ݶ����¶�
	if (xlmdb.Read_XLDbs_Railway(MdbName,L"�¶ȱ�"))
	{
		pzLinep->SetBPDArray(xlmdb.NPD, xlmdb.PDB, false);
		pzLinep->TrsBPDArraytoCM();
		if (xlmdb.NPD>0)
		{
			pzLinep->m_LJorGM = xlmdb.PDB[0].RLen;
			pzLinep->m_Xsw = _wtoi(xlmdb.PDB[0].Notes);
			pzLinep->m_DesHXSW = xlmdb.PDB[0].Rshu;
		}
	}

	return pzLinep;
}


//��ȡ��Ƣ��ߡ����ڴ��д�������Ƣ���ʱֱ�ӻ�ȡ��������ݿⴴ��
GTZX2_ROAD* PmPointer::GetSJEX(CString mdbname, int zory)
{
	mdbname.MakeUpper();
	int i;
	for (i=0; i<g_GTZX2PtArray.GetSize(); i++)
	{
		if (g_GTZX2PtArray[i]->mdbname == mdbname)
		{
			return g_GTZX2PtArray[i];
		}
	}
	if (i == g_GTZX2PtArray.GetSize())
	{
		GTZX2_ROAD* pGTZX2 = NULL;
		pGTZX2 = CreateSJEX(mdbname,zory);
		if (pGTZX2)
			g_GTZX2PtArray.Add(pGTZX2);
		return pGTZX2;
	}
}

//�õ���ƶ���ָ��
GTZX2_ROAD* PmPointer::CreateSJEX(CString MdbName, int zory)
{
	MdbName.MakeUpper();
	int NJD,j,i;
	ACHAR GH[8],JDGH[8];
	double arrayd[200][6];

	//��ȡ��ƻ���
	XLDataBase xlmdb;
	if(xlmdb.Read_XLDbs_Railway(MdbName,L"���߱�"))//������
	{
		NJD = xlmdb.NQX;
		for(i=0;i<NJD;i++)
		{
			arrayd[i][0] = xlmdb.QXB[i].jdno;
			arrayd[i][1] = xlmdb.QXB[i].x;
			arrayd[i][2] = xlmdb.QXB[i].y;
			arrayd[i][3] = xlmdb.QXB[i].l1;
			arrayd[i][4] = xlmdb.QXB[i].R;
			arrayd[i][5] = xlmdb.QXB[i].l2;	
		}
	}
	else
	{
		AfxMessageBox(L"\n�����߱�ʧ��!");
		return NULL;
	}

	_tcscpy(JDGH,L"JD");
	_tcscpy(GH,L"AK"); 
	GTZX_ROAD *pzx;//һ��		
	pzx= new GTZX_ROAD(NJD,arrayd,&GH[0],&JDGH[0]);
	pzx->setINIT(3,3,1,5);
	pzx->mode_display=0;
	pzx->mdbname = MdbName;


	//������
	if(xlmdb.Read_XLDbs_Railway(MdbName,L"������"))//������
	{
		pzx->NDL = xlmdb.NDL;
		for(int i = 0; i < pzx->NDL;i++)
			pzx->setDLM(i,xlmdb.DLArray[i].BLC,xlmdb.DLArray[i].DL,xlmdb.DLArray[i].BGH,xlmdb.DLArray[i].EGH);
	}
	else
	{
		ads_printf(L"\n��������ʧ��!");
		return NULL;
	}
	pzx->getpxy_DATA();

	//////////////////////////////////////////////////////////////////////////
	//�ݶ����¶�
	if (xlmdb.Read_XLDbs_Railway(MdbName,L"�¶ȱ�"))
	{
		if (xlmdb.NPD>0)
		{
			pzx->m_LJorGM = xlmdb.PDB[0].RLen;
			pzx->m_Xsw = _wtoi(xlmdb.PDB[0].Notes);
			pzx->m_DesHXSW = xlmdb.PDB[0].Rshu;
		}
		pzx->SetBPDArray(xlmdb.NPD,xlmdb.PDB,false);
		pzx->TrsBPDArraytoCM();
	}
	//////////////////////////////////////////////////////////////////////////
	GTZX2_ROAD *pzLinep2 = NULL;//����	
	if (zory == -1)
	{
		if(xlmdb.Read_XLDbs_Railway(MdbName,L"�������߱�"))//������
		{	
			pzLinep2= new GTZX2_ROAD(pzx,xlmdb.NQX2x,xlmdb.QXB2x,pzx->JD0,L"ZJD");
			pzLinep2->mdbname=MdbName;
			pzLinep2->NDL=xlmdb.NDL2x;
			if(xlmdb.Read_XLDbs_Railway(MdbName,L"���߶�����"))
				pzLinep2->SetDLB(xlmdb.DLB2x,xlmdb.NDL2x);
			else
				pzLinep2->Create2XGTDLB();
			//////////////////////////////////////////////////////////////////////////
			int RXDSum = pzLinep2->GetRXDNum();
			if (RXDSum>0)
				pzLinep2->AllRXDBPDArray.SetSize(RXDSum);
			for (int iRXD=0; iRXD<RXDSum; iRXD++)
			{
				xlmdb.NPD = 0;
				if (xlmdb.Read_EX_XLDbs(MdbName, L"�������ж��¶ȱ�", iRXD))
				{
					if (xlmdb.NPD>0)
					{
						pzLinep2->m_LJorGM = xlmdb.PDB[0].RLen;
						pzLinep2->m_Xsw = _wtoi(xlmdb.PDB[0].Notes);
						pzLinep2->m_DesHXSW = xlmdb.PDB[0].Rshu;
					}
					pzLinep2->SetiRXDBPDArray(iRXD, xlmdb.NPD, xlmdb.PDB, false);
				}
			}
			pzLinep2->Create2XGTBPDArray();
		}
		else
		{
			ads_printf(L"\n���������߱�ʧ��!");
			return NULL;
		}
	}

	else
	{
		if(xlmdb.Read_XLDbs_Railway(MdbName,L"�������߱�"))//������
		{
			pzLinep2= new GTZX2_ROAD(pzx,xlmdb.NQX2x,xlmdb.QXB2x,pzx->JD0,L"YJD");
			pzLinep2->mdbname=MdbName;
			pzLinep2->NDL=xlmdb.NDL2x;
			if(xlmdb.Read_XLDbs_Railway(MdbName,L"���߶�����"))
				pzLinep2->SetDLB(xlmdb.DLB2x,xlmdb.NDL2x);
			else
				pzLinep2->Create2XGTDLB();
			//////////////////////////////////////////////////////////////////////////
			int RXDSum = pzLinep2->GetRXDNum();
			if (RXDSum>0)
				pzLinep2->AllRXDBPDArray.SetSize(RXDSum);
			for (int iRXD=0; iRXD<RXDSum; iRXD++)
			{
				xlmdb.NPD = 0;
				if (xlmdb.Read_EX_XLDbs(MdbName, L"�������ж��¶ȱ�", iRXD))
				{
					if (xlmdb.NPD>0)
					{
						pzLinep2->m_LJorGM = xlmdb.PDB[0].RLen;
						pzLinep2->m_Xsw = _wtoi(xlmdb.PDB[0].Notes);
						pzLinep2->m_DesHXSW = xlmdb.PDB[0].Rshu;
					}
					pzLinep2->SetiRXDBPDArray(iRXD, xlmdb.NPD, xlmdb.PDB, false);
				}
			}
			pzLinep2->Create2XGTBPDArray();
		}
		else
		{
			ads_printf(L"\n���������߱�ʧ��!");
			return NULL;
		}
	}
	return pzLinep2;
}



GTZX2_JYX_ROAD *PmPointer::CreateJiXianShejiGTZX2Pointer(CString mdbname , GTZX_JYX_ROAD * & pGTZXLinp )
{

	//���л���ָ��
	pGTZXLinp = CreateJYJX(mdbname);
	//	if(!pGTZXLinp)return NULL;

	if(pGTZXLinp==NULL)return NULL ;

	CString Temp1 = "��ƻ���(���л���)";
	XLDataBase_JYX xlmdb;
	GTZX2_JYX_ROAD *pzLinep2;

	CString Temp2;
	Temp1.Trim();
	Temp2 = Temp1;
	Temp1=L"��·��=\"" + Temp1 + "\"";

	//������·��,��ȡ������ƻ�������,ָ��
	if(xlmdb.Read_XLDbs(mdbname,L"�������߱�",Temp1))//������
	{			
		pzLinep2= new GTZX2_JYX_ROAD(pGTZXLinp,xlmdb.NQX2x,xlmdb.QXB2x,pGTZXLinp->JD0,L"JD");

		if(pzLinep2==NULL)return NULL ;

		pzLinep2->mdbname=mdbname;

		_tcscpy(pzLinep2->xlname , Temp2);
		if(xlmdb.Read_XLDbs(mdbname,L"���߶�����",Temp1))
		{
			pzLinep2->NDL=xlmdb.NDL2x;
			pzLinep2->SetDLB(xlmdb.DLB2x,xlmdb.NDL2x);
		}

		CString XLName , Temp ;
		for(int i=0;i<pzLinep2->GetRXDNum();i++)
		{
			XLName = pzLinep2->GetRXDXLNameByRXDIndex( i );

			Temp=L"��·��=\""+ XLName +"\"";
			//��־��̱�
			if(xlmdb.Read_XLDbs(mdbname,L"��־��̱�",Temp))//����־���
			{
				pzLinep2->SetBZLCArray(  xlmdb.BZLCTable , xlmdb.BZLCNum ) ;
			}

			if(xlmdb.BZLCNum<2)
			{
				CArray<BAS_DRAW_FUN_JYX::BZLCStructTable ,BAS_DRAW_FUN_JYX::BZLCStructTable>  BZLCarrayTmp ;
				pzLinep2->SetDefaultiRxdBZLC( i , BZLCarrayTmp );

				pzLinep2->SetBZLCArray(  BZLCarrayTmp ) ;
			}

		}

		pzLinep2->calsflag=false;		
	}
	else
		return NULL;

	return pzLinep2;
}


GTZX2_JYX_ROAD *PmPointer::CreateErXianShejiGTZX2Pointer(CString mdbname , int ZorY , GTZX_JYX_ROAD * & pGTZXLinp ,GTZX2_JYX_ROAD *& pGTZXLinp2ToJYX )
{

	//pGTZXLinp2ToJYX --����II��ָ��
	GTZX2_JYX_ROAD *pGTZXLinp2 = NULL ;
	pGTZXLinp = NULL ;

	//�õ�������(��)��ָ��
	pGTZXLinp2ToJYX = CreateJYEX(mdbname,ZorY);


	if(!pGTZXLinp2ToJYX)
	{
		return NULL;
	}

	pGTZXLinp2 = pGTZXLinp2ToJYX;
	//������(��)�� ת�� �ɻ���
	pGTZXLinp= pGTZXLinp2->CreateGtzx();

	if(pGTZXLinp==NULL)return NULL ;


	CString Temp,Temp2;
	if(ZorY<0)Temp = "��ƻ���(��������)";
	else Temp = "��ƻ���(��������)";

	XLDataBase_JYX xlmdb;
	GTZX2_JYX_ROAD *pzLinep2;

	Temp.Trim();
	Temp2 = Temp;
	Temp=L"��·��=\"" + Temp + "\"";

	//������·��,��ȡ������(��)�ߵ���ƻ��ߡ�����,�õ�ָ��
	if(xlmdb.Read_XLDbs(mdbname,L"�������߱�",Temp))//������
	{		

		pzLinep2= new GTZX2_JYX_ROAD(pGTZXLinp,xlmdb.NQX2x,xlmdb.QXB2x,pGTZXLinp->JD0,L"JD");

		if( pzLinep2 == NULL )return NULL ;

		pzLinep2->mdbname=mdbname;

		_tcscpy( pzLinep2->xlname ,Temp2);
		if(xlmdb.Read_XLDbs(mdbname,L"���߶�����",Temp))
		{
			pzLinep2->NDL=xlmdb.NDL2x;
			pzLinep2->SetDLB(xlmdb.DLB2x,xlmdb.NDL2x);	

		}

		CString XLName , Temp ;
		for(int i=0;i<pzLinep2->GetRXDNum();i++)
		{
			XLName = pzLinep2->GetRXDXLNameByRXDIndex( i );

			Temp=L"��·��=\""+ XLName +"\"";
			//��־��̱�
			if(xlmdb.Read_XLDbs(mdbname,L"��־��̱�",Temp))//����־���
			{
				pzLinep2->SetBZLCArray(  xlmdb.BZLCTable , xlmdb.BZLCNum ) ;
			}

			if(xlmdb.BZLCNum<2)
			{
				CArray<BAS_DRAW_FUN_JYX::BZLCStructTable ,BAS_DRAW_FUN_JYX::BZLCStructTable>  BZLCarrayTmp ;
				pzLinep2->SetDefaultiRxdBZLC( i , BZLCarrayTmp );

				pzLinep2->SetBZLCArray(  BZLCarrayTmp ) ;
			}

		}

		pzLinep2->calsflag=false;		
	}
	else
		return NULL;

	return pzLinep2;
}



//���ɻ��߻��߹�ͨ��ƻ��ߵ�ƽ��ָ��GTZX
GTZX_JYX_ROAD *PmPointer::CreateJYJX(CString mdbname,CString XLName , BOOL isLLX )
{
	CString curpm = mdbname;  	

	int NJD,i;
	ACHAR GH[8],JDGH[8];
	CArray<GTZX_JYX_ROAD::JDarrayStruct,GTZX_JYX_ROAD::JDarrayStruct>array;
	array.RemoveAll();


	CString Temp1;
	Temp1 = XLName;
	Temp1.Trim();
	Temp1=L"��·��=\"" + Temp1 + "\"";
	XLDataBase_JYX xlmdb;
	if(xlmdb.Read_XLDbs(curpm,L"�������ݱ�",Temp1))//������
	{  
		NJD = xlmdb.NJD;
		array.SetSize(NJD);
		for(i=0;i<NJD;i++)
		{
			_tcscpy(array[i].JDNum ,xlmdb.JDarray[i].JDNum);
			array[i].N = xlmdb.JDarray[i].N;
			array[i].E = xlmdb.JDarray[i].E;
			array[i].l1 = xlmdb.JDarray[i].l1;
			array[i].R = xlmdb.JDarray[i].R;
			array[i].l2 = xlmdb.JDarray[i].l2;
		}	
	}
	else
	{
		//ads_printf(L"\n���������ݱ�ʧ��!");
		return NULL;
	}
	if (NJD< 2) return NULL;
	_tcscpy(GH,L"AK"); 
	_tcscpy(JDGH,L"JD");

	GTZX_JYX_ROAD *pzx = NULL ;
	if(isLLX) 
	{
		pzx= new GTZX_JYX_ROAD(NJD,array,&GH[0],&JDGH[0],isLLX);
		_tcscpy(pzx->JDGH,L"LLJD");
	}
	else 
	{
		pzx = new GTZX_JYX_ROAD(NJD,array,&GH[0],&JDGH[0]);
	}


	if(pzx==NULL)return NULL ;

	pzx->XLShuXing = XLName ;

	pzx->setINIT(3,3,1,5);
	pzx->mode_display=0;
	pzx->mdbname = curpm;

	if(xlmdb.Read_XLDbs(curpm,L"������",Temp1))//������
	{
		pzx->NDL = xlmdb.NDL;
		for(i=0;i<pzx->NDL;i++)
		{
			_tcscpy(pzx->DLB[i].BGH,xlmdb.DLB[i].BGH);
			_tcscpy(pzx->DLB[i].EGH,xlmdb.DLB[i].EGH);
			pzx->DLB[i].BLC = xlmdb.DLB[i].BLC;
			pzx->DLB[i].DL = xlmdb.DLB[i].DL;

			pzx->DLB[i].N = xlmdb.DLB[i].N;
			pzx->DLB[i].E = xlmdb.DLB[i].E;
		}
	}

	pzx->getpxy_DATA();

	if(xlmdb.Read_XLDbs(mdbname,L"��־��̱�",Temp1))//����־���
	{
		pzx->NBZLC = xlmdb.BZLCNum;	
		pzx->BZLCarray.RemoveAll();
		pzx->BZLCarray.SetSize(pzx->NBZLC);
		for(int i=0;i<pzx->NBZLC;i++)
		{
			_tcscpy(pzx->BZLCarray[i].kml,xlmdb.BZLCTable[i].kml);
			pzx->BZLCarray[i].X = xlmdb.BZLCTable[i].X;
			pzx->BZLCarray[i].Y = xlmdb.BZLCTable[i].Y;
		}
	}
	pzx->SetBZLCVal();

	return pzx;

}

//���ɼ�����\���߻��߹�ͨ��Ƣ��ߵ�ƽ��ָ��GTZX2 (XLName=��ͨ��Ƣ���,������\����)
GTZX2_JYX_ROAD *PmPointer::CreateJYEX(CString mdbname,CString XLName)
{
	GTZX2_JYX_ROAD *pzLinep2=NULL;

	CString curpm = mdbname;  	

	int NJD,i;
	ACHAR GH[8],JDGH[8];
	CArray<GTZX_JYX_ROAD::JDarrayStruct,GTZX_JYX_ROAD::JDarrayStruct>array;
	array.RemoveAll();

	CString Temp1;
	if (XLName=="��ͨ��Ƣ���")
	{
		Temp1 = "��ͨ��ƻ���";
	}
	else
	{
		Temp1 = "���л���";
	}
	Temp1.Trim();
	Temp1=L"��·��=\"" + Temp1 + "\"";
	XLDataBase_JYX xlmdb;
	if(xlmdb.Read_XLDbs(curpm,L"�������ݱ�",Temp1))//������
	{  
		NJD = xlmdb.NJD;
		array.SetSize(NJD);
		for(i=0;i<NJD;i++)
		{
			_tcscpy(array[i].JDNum ,xlmdb.JDarray[i].JDNum);
			array[i].N = xlmdb.JDarray[i].N;
			array[i].E = xlmdb.JDarray[i].E;
			array[i].l1 = xlmdb.JDarray[i].l1;
			array[i].R = xlmdb.JDarray[i].R;
			array[i].l2 = xlmdb.JDarray[i].l2;
		}	
	}
	else
	{
		//ads_printf(L"\n���������ݱ�ʧ��!");
		return NULL;
	}
	if (NJD< 2) return NULL;
	_tcscpy(GH,L"AK"); 
	_tcscpy(JDGH,L"JD");
	GTZX_JYX_ROAD *pzx= new GTZX_JYX_ROAD(NJD,array,&GH[0],&JDGH[0]);

	if(pzx==NULL)return NULL ;

	pzx->setINIT(3,3,1,5);
	pzx->mode_display=0;
	pzx->mdbname = curpm;

	if(xlmdb.Read_XLDbs(curpm,L"������",Temp1))//������
	{
		pzx->NDL = xlmdb.NDL;
		for(i=0;i<pzx->NDL;i++)
		{
			_tcscpy(pzx->DLB[i].BGH,xlmdb.DLB[i].BGH);
			_tcscpy(pzx->DLB[i].EGH,xlmdb.DLB[i].EGH);
			pzx->DLB[i].BLC = xlmdb.DLB[i].BLC;
			pzx->DLB[i].DL = xlmdb.DLB[i].DL;

			pzx->DLB[i].N = xlmdb.DLB[i].N;
			pzx->DLB[i].E = xlmdb.DLB[i].E;
		}
	}
	pzx->getpxy_DATA();

	Temp1=XLName;
	Temp1.Trim();
	Temp1=L"��·��=\"" + Temp1 + "\"";
	if(xlmdb.Read_XLDbs(curpm,L"�������߱�",Temp1))//������
	{			

		pzLinep2= new GTZX2_JYX_ROAD(pzx,xlmdb.NQX2x,xlmdb.QXB2x,pzx->JD0,L"JD");

		if(pzLinep2==NULL)return NULL ;

		pzLinep2->mdbname=curpm;

		if(xlmdb.Read_XLDbs(curpm,L"���߶�����",Temp1))
		{
			pzLinep2->NDL=xlmdb.NDL2x;
			pzLinep2->SetDLB(xlmdb.DLB2x,xlmdb.NDL2x);	

			CString XLName , Temp ;
			for(int i=0;i<xlmdb.NDL2x;i++)
			{
				XLName = xlmdb.DLB2x[i].RXDName;

				Temp=L"��·��=\""+ XLName +"\"";
				//��־��̱�
				if(xlmdb.Read_XLDbs(mdbname,L"��־��̱�",Temp))//����־���
				{
					pzLinep2->SetBZLCArray(  xlmdb.BZLCTable , xlmdb.BZLCNum ) ;

				}
			}
		}
		pzLinep2->calsflag=false;		
	}

	return pzLinep2;
}



//��ȡ��ͨ��ƻ��ߡ����ڴ��д�����ʱֱ�ӻ�ȡ��������ݿⴴ��
GTZX_JYX_ROAD* PmPointer::GetGTSJJX(CString mdbname)
{
	int i;
	mdbname.MakeUpper();
	for (i=0; i<g_GTZX_JYXPtArray.GetSize(); i++)
	{
		if (g_GTZX_JYXPtArray[i]->mdbname == mdbname&&g_GTZX_JYXPtArray[i]->XLShuXing=="��ͨ��ƻ���")
		{
			return g_GTZX_JYXPtArray[i];
		}
	}
	if (i == g_GTZX_JYXPtArray.GetSize())
	{
		GTZX_JYX_ROAD* pGTZXJYX = NULL;
		pGTZXJYX = CreateGTSJJXPointer(mdbname);
		if (pGTZXJYX)
			g_GTZX_JYXPtArray.Add(pGTZXJYX);
		//ads_printf(L"qd2-%f %f\n",pGTZXJYX->JDarray[0].N,pGTZXJYX->JDarray[0].E);
		return pGTZXJYX;
	}
}

GTZX_JYX_ROAD *PmPointer::CreateGTSJJXPointer(CString mdbname )
{
	////�õ�������ָ��
	// 	CPMSJ pmsj;
	// 	pmsj.FileName = mdbname;

	if (JiXianpGtzx==NULL)
		JiXianpGtzx = GetJYJX(mdbname);
	if (ZuoXianpGtzx==NULL)
		ZuoXianpGtzx = GetJYEX(mdbname,-1);
	if (YouXianpGtzx==NULL)
		YouXianpGtzx = GetJYEX(mdbname,1);

	//CString XLName ="��ͨ��ƻ���";
	CString XLName = "��ƻ���";

	int NJD,NH,NLC;
	double  Texth;
	int BZW;
	XLDataBase_JYX xlmdb;

	CString Temp; 

	Temp = XLName;
	Temp=L"��·��=\""+ Temp +"\"";

	CArray<GTZX_JYX_ROAD::JDarrayStruct,GTZX_JYX_ROAD::JDarrayStruct>array;
	if(xlmdb.Read_XLDbs(mdbname,L"��ƻ��߽������ݱ�",Temp))//������
	{  
		NJD = xlmdb.NJD;
		array.SetSize(NJD);

		for(int i=0;i<NJD;i++)
		{
			_tcscpy(array[i].JDNum ,xlmdb.JDarray[i].JDNum);
			array[i].N = xlmdb.JDarray[i].N;
			array[i].E = xlmdb.JDarray[i].E;
			array[i].l1 = xlmdb.JDarray[i].l1;
			array[i].R = xlmdb.JDarray[i].R;
			array[i].l2 = xlmdb.JDarray[i].l2;
		}	
	}
	else
	{
		//ads_printf(L"\n���������ݱ�ʧ��!");	//20190712	��ʱ�������⣬�Ȳ���ʾ
		return 0;
	}

	if (NJD< 2) return 0;
	BZW=3;
	NLC=3; NH=3;

	Texth=5;

	ACHAR GH[8];
	ACHAR JDGH[8];
	_tcscpy(GH,L"AK"); 
	_tcscpy(JDGH,L"JD");

	BOOL isConnectGTZX;

	if(XLName=="���л���")
		isConnectGTZX = FALSE;
	//else if(XLName=="��ͨ��ƻ���")
	else if (XLName == "��ƻ���")
		isConnectGTZX = FALSE;
	else if(XLName=="������")
		isConnectGTZX = FALSE;
	else 
		isConnectGTZX = TRUE;

	GTZX_JYX_ROAD *pzLinep= new GTZX_JYX_ROAD(NJD,array,&GH[0],&JDGH[0],isConnectGTZX);

	if(pzLinep==NULL)return NULL ;
	// ��ȡ ������  
	//������
	pzLinep->mdbname=mdbname;
	pzLinep->XLShuXing =  XLName;

	if(xlmdb.Read_XLDbs(mdbname,L"��ƻ��߶�����",Temp))//������
	{
		pzLinep->assertWriteEnabled();

		pzLinep->NDL = xlmdb.NDL;

		for(int i=0;i<pzLinep->NDL;i++)
		{
			pzLinep->setDLM(i,xlmdb.DLB[i].BLC,xlmdb.DLB[i].DL,xlmdb.DLB[i].BGH,xlmdb.DLB[i].EGH);
			//pzLinep->DLB[i].N = jxpm1->DLB[i].N;
			//pzLinep->DLB[i].E = jxpm1->DLB[i].E;
			pzLinep->DLB[i].N = xlmdb.DLB[i].N;
			pzLinep->DLB[i].E = xlmdb.DLB[i].E;
		}
	}

	//��־��̱�
	//if(xlmdb.Read_XLDbs(mdbname,L"��־��̱�",Temp))//����־���
	//{
	//	pzLinep->NBZLC = xlmdb.BZLCNum;	
	//	pzLinep->BZLCarray.RemoveAll();
	//	pzLinep->BZLCarray.SetSize(pzLinep->NBZLC);
	//	for(int i=0;i<pzLinep->NBZLC;i++)
	//	{
	//		_tcscpy(pzLinep->BZLCarray[i].kml,xlmdb.BZLCTable[i].kml);
	//		pzLinep->BZLCarray[i].X = xlmdb.BZLCTable[i].X;
	//		pzLinep->BZLCarray[i].Y = xlmdb.BZLCTable[i].Y;
	//	}
	//}

	pzLinep->SetBZLCVal();
	//if(XLName=="��ͨ��ƻ���")
	if (XLName == "��ƻ���")
	{
		//��ȡ�Ľ��λ��ܱ�
		if(xlmdb.Read_XLDbs(mdbname,L"�Ľ��λ��ܱ�",Temp))//���Ľ�������
		{  
			pzLinep->GJDInforArray.SetSize(xlmdb.GJDAllInforNum);
			pzLinep->GJDInfTab.SetSize(xlmdb.GJDAllInforNum);

			pzLinep->NGJD = xlmdb.GJDAllInforNum;
			for(int i=0;i<xlmdb.GJDAllInforNum;i++)
			{
				pzLinep->GJDInforArray[i].spt[0] = xlmdb.GJDAllInforTable[i].BpointToN ;
				pzLinep->GJDInforArray[i].spt[1] = xlmdb.GJDAllInforTable[i].BpointToE ;
				pzLinep->GJDInforArray[i].ept[0] = xlmdb.GJDAllInforTable[i].EpointToN ;
				pzLinep->GJDInforArray[i].ept[1] = xlmdb.GJDAllInforTable[i].EpointToE ;

				_tcscpy( pzLinep->GJDInforArray[i].BXLName , xlmdb.GJDAllInforTable[i].BXLName ) ;
				_tcscpy( pzLinep->GJDInforArray[i].EXLName , xlmdb.GJDAllInforTable[i].EXLName ) ;
				pzLinep->GJDInforArray[i].GJDType = xlmdb.GJDAllInforTable[i].GJDType ;

				_tcscpy( pzLinep->GJDInforArray[i].gjdname , xlmdb.GJDAllInforTable[i].ToInSGJJDNum ) ;
				//���/�յ��ֳ����
				//���
				_tcscpy( pzLinep->GJDInforArray[i].GJDBLC , xlmdb.GJDAllInforTable[i].GJDBLC ) ;
				//�յ����
				_tcscpy( pzLinep->GJDInforArray[i].GJDELC , xlmdb.GJDAllInforTable[i].GJDELC ) ;


				pzLinep->GJDInfTab[i].spt[0] = xlmdb.GJDAllInforTable[i].BpointToN ;
				pzLinep->GJDInfTab[i].spt[1] = xlmdb.GJDAllInforTable[i].BpointToE ;
				pzLinep->GJDInfTab[i].ept[0] = xlmdb.GJDAllInforTable[i].EpointToN ;
				pzLinep->GJDInfTab[i].ept[1] = xlmdb.GJDAllInforTable[i].EpointToE ;

				_tcscpy( pzLinep->GJDInfTab[i].BXLName , xlmdb.GJDAllInforTable[i].BXLName ) ;
				_tcscpy( pzLinep->GJDInfTab[i].EXLName , xlmdb.GJDAllInforTable[i].EXLName ) ;
				pzLinep->GJDInfTab[i].GJDType = xlmdb.GJDAllInforTable[i].GJDType ;

				_tcscpy( pzLinep->GJDInfTab[i].gjdname , xlmdb.GJDAllInforTable[i].ToInSGJJDNum ) ;

				//���/�յ��ֳ����
				//���
				_tcscpy( pzLinep->GJDInfTab[i].GJDBLC , xlmdb.GJDAllInforTable[i].GJDBLC ) ;
				//�յ����
				_tcscpy( pzLinep->GJDInfTab[i].GJDELC , xlmdb.GJDAllInforTable[i].GJDELC ) ;
			}
		}
		else
		{
			//ads_printf(L"\n���Ľ��λ��ܱ�ʧ��!");
			//return ;
		}
		//////////////////////////////////////////////////////////////////////////
		//�ݶ����¶�	20191113�޸�
		APDTAB pLeftSlope/*,pRightSlope*/;
		//GetSlopeData(mdbname, pzLinep, pLeftSlope, pRightSlope);
		GetSlopeDataSJJX(mdbname, pzLinep, pLeftSlope);
		int NBPD;
		PDTAB* BPD_array;

		NBPD=pLeftSlope.GetSize();
		pzLinep->m_NCM = pLeftSlope.GetSize()+1;
		BPD_array=new PDTAB[NBPD];
		if (NBPD>0)
		{
			for (int i=0 ; i<NBPD; i++)
			{
				BPD_array[i].ml = pLeftSlope[i].ml;//ͳһ���
				BPD_array[i].Level = pLeftSlope[i].Level;//�߳�
				BPD_array[i].Rshu = pLeftSlope[i].Rshu;//�����߰뾶	
				//BPD_array[i].CKml=pLeftSlope[i].CKml;
				_tcscpy(BPD_array[i].CKml,pLeftSlope[i].CKml);
				BPD_array[i].cml=pLeftSlope[i].cml;
				BPD_array[i].degree=pLeftSlope[i].degree;
				BPD_array[i].GH=pLeftSlope[i].GH;
				BPD_array[i].length=pLeftSlope[i].length;
				BPD_array[i].NameOfWay=pLeftSlope[i].NameOfWay;
				BPD_array[i].Notes=pLeftSlope[i].Notes;
				BPD_array[i].RLen=pLeftSlope[i].RLen;
				BPD_array[i].Rshu=pLeftSlope[i].Rshu;
			}
			//				DesHXSW = (int)(pLeftSlope->m_SlopeValue[0].Rshu + 0.1);//��Ʊ��С��λ��
			//				LJorGM = (int)(pLeftSlope->m_SlopeValue[0].RLen + 0.1);//��Ʊ����·�绹�ǹ���
			BPD_array[0].Rshu=BPD_array[NBPD-1].Rshu=0.0;//���ձ��µ������߰뾶Ϊ0
			pzLinep->SetBPDArray(NBPD,BPD_array,false);
			pzLinep->TrsBPDArraytoCM();//����ת��(���µ����� -> CM����)
		}


		pzLinep->FormSectionInfor();
	}

	pzLinep->setINIT(NLC,NH,1,Texth);
	pzLinep->mode_display=0;

	return pzLinep;
}

//��ȡ��ͨ��Ƣ��ߡ����ڴ��д���ʱֱ�ӻ�ȡ��������ݿⴴ��
GTZX2_JYX_ROAD* PmPointer::GetGTSJEX(CString mdbname)
{
	int i;
	for (i=0; i<g_GTZX2_JYXPtArray.GetSize(); i++)
	{
		if (g_GTZX2_JYXPtArray[i]->mdbname == mdbname&&_tcscmp(g_GTZX2_JYXPtArray[i]->xlname,L"��ͨ��Ƣ���")==0)
		{
			return g_GTZX2_JYXPtArray[i];
		}
	}
	if (i == g_GTZX2_JYXPtArray.GetSize())
	{
		GTZX2_JYX_ROAD* pGTZX2_JYX = NULL;
		pGTZX2_JYX = CreateGTSJEXPointer(mdbname);
		if (pGTZX2_JYX)
			g_GTZX2_JYXPtArray.Add(pGTZX2_JYX);
		return pGTZX2_JYX;
	}
}

//û�й��ָ��,���ȴ�������ٴ�����2ָ��
GTZX2_JYX_ROAD *PmPointer::CreateGTSJEXPointer(CString mdbname)
{
	GTZX_JYX_ROAD * pGTJXpLinp = CreateGTSJJXPointer(mdbname);

	if(pGTJXpLinp==NULL)
		return NULL;

	GTZX2_JYX_ROAD *pGTSJEXzLinep = CreateGTSJEXPointer(pGTJXpLinp,mdbname);

	if(pGTSJEXzLinep==NULL)
		return NULL ;

	return pGTSJEXzLinep;
}

//�Ѿ������˹��ָ��,��ֱ�Ӵ�����2ָ��
GTZX2_JYX_ROAD *PmPointer::CreateGTSJEXPointer2(GTZX_JYX_ROAD * pGTJXpLinp,CString mdbname)
{
	GTZX2_JYX_ROAD *pGTSJEXzLinep = CreateGTSJEXPointer(pGTJXpLinp,mdbname);

	if(pGTSJEXzLinep==NULL)
		return NULL;
	//////////////////////////////////////////////////////////////////////////
	//�ݶ����¶�
	XLDataBase_JYX xlmdb;
	APDTAB pLeftSlope,pRightSlope;
	GetSlopeData(mdbname, pGTJXpLinp, pLeftSlope, pRightSlope);
	int NBPD;
	PDTAB* BPD_array;
	NBPD = pRightSlope.GetSize();
	pGTSJEXzLinep->m_NCM=pRightSlope.GetSize()+1;
	if (NBPD>0)
	{
		for (int i=0 ; i<NBPD; i++)
		{
			BPD_array[i].ml = pRightSlope[i].ml;//ͳһ���
			BPD_array[i].Level = pRightSlope[i].Level;//�߳�
			BPD_array[i].Rshu = pRightSlope[i].Rshu;//�����߰뾶	
			//BPD_array[i].CKml=pLeftSlope[i].CKml;
			_tcscpy(BPD_array[i].CKml,pRightSlope[i].CKml);
			BPD_array[i].cml=pRightSlope[i].cml;
			BPD_array[i].degree=pRightSlope[i].degree;
			BPD_array[i].GH=pRightSlope[i].GH;
			BPD_array[i].length=pRightSlope[i].length;
			BPD_array[i].NameOfWay=pRightSlope[i].NameOfWay;
			BPD_array[i].Notes=pRightSlope[i].Notes;
			BPD_array[i].RLen=pRightSlope[i].RLen;
			BPD_array[i].Rshu=pRightSlope[i].Rshu;
		}
		//				DesHXSW = (int)(pRightSlope->m_SlopeValue[0].Rshu + 0.1);//��Ʊ��С��λ��
		//				LJorGM = (int)(pRightSlope->m_SlopeValue[0].RLen + 0.1);//��Ʊ����·�绹�ǹ���


		//////////////////////////////////////////////////////////////////////////
		int RXDSum = pGTSJEXzLinep->GetRXDNum();
		if (RXDSum>0)
			pGTSJEXzLinep->AllRXDBPDArray.SetSize(RXDSum);
		for (int iRXD=0; iRXD<RXDSum; iRXD++)
		{
			xlmdb.NPD = 0;
			CString  wlcstring;
			CString RXDNAME=pGTSJEXzLinep->GetRXDXLNameByRXDIndex(iRXD);
			wlcstring.Format(L"��·�� = '%s'",RXDNAME);
			if (xlmdb.Read_XLDbs(mdbname, L"�¶ȱ�", wlcstring))
			{
				if (xlmdb.NPD>0)
				{
					pGTSJEXzLinep->m_LJorGM = xlmdb.PDB[0].RLen;
					pGTSJEXzLinep->m_Xsw = _wtoi(xlmdb.PDB[0].Notes);
					pGTSJEXzLinep->m_DesHXSW = xlmdb.PDB[0].Rshu;
				}
				pGTSJEXzLinep->SetiRXDBPDArray(iRXD, xlmdb.NPD, xlmdb.PDB, false);
			}
		}
		pGTSJEXzLinep->Create2XGTBPDArray(pRightSlope);


		BPD_array[0].Rshu=BPD_array[NBPD-1].Rshu=0.0;//���ձ��µ������߰뾶Ϊ0
		/////�������жα��µ�

		pGTSJEXzLinep->SetBPDArray(NBPD,BPD_array,false);
		pGTSJEXzLinep->TrsBPDArraytoCM();//����ת��(���µ����� -> CM����)
	}

	return pGTSJEXzLinep;
}




GTZX2_JYX_ROAD *PmPointer::CreateGTSJEXPointer(GTZX_JYX_ROAD *pGTJXpLinpToIn , CString mdbname)
{


	XLDataBase_JYX xlmdb;
	CString Temp1;

	GTZX2_JYX_ROAD *pzLinep2 = NULL ;//һ��	
	Temp1 = "��ƶ���";
	Temp1.Trim();
	Temp1=L"��·��=\"" + Temp1 + "\"";

	if(xlmdb.Read_XLDbs(mdbname,L"������ƶ��߽������ݱ�",Temp1))//������
	{
		pzLinep2= new GTZX2_JYX_ROAD(pGTJXpLinpToIn,xlmdb.NQX2x,xlmdb.QXB2x,pGTJXpLinpToIn->JD0,L"ZJD");

		if(pzLinep2==NULL)return NULL ;
		pzLinep2->mdbname=mdbname;

		if(xlmdb.Read_XLDbs(mdbname,L"���߶�����",Temp1))
		{
			pzLinep2->NDL=xlmdb.NDL2x;
			pzLinep2->SetDLB(xlmdb.DLB2x,xlmdb.NDL2x);
		}
		else
			pzLinep2->Create2XGTDLB();


		CString XLName , Temp ;
		for(int i=0;i<pzLinep2->GetRXDNum();i++)
		{
			XLName = pzLinep2->GetRXDXLNameByRXDIndex( i );

			Temp=L"��·��=\""+ XLName +"\"";
			//��־��̱�
			if(xlmdb.Read_XLDbs(mdbname,L"��־��̱�",Temp))//����־���
			{
				pzLinep2->SetBZLCArray(xlmdb.BZLCTable,xlmdb.BZLCNum);
			}

			if(xlmdb.BZLCNum<2)
			{
				CArray<BAS_DRAW_FUN_JYX::BZLCStructTable ,BAS_DRAW_FUN_JYX::BZLCStructTable>  BZLCarrayTmp ;
				pzLinep2->SetDefaultiRxdBZLC( i , BZLCarrayTmp );

				pzLinep2->SetBZLCArray(  BZLCarrayTmp ) ;
			}

		}

		//��ȡ�Ľ��λ��ܱ�
		if(xlmdb.Read_XLDbs(mdbname,L"�Ľ��λ��ܱ�",Temp1))//���Ľ�������
		{  
			pzLinep2->GJDInforArray.SetSize(xlmdb.GJDAllInforNum);
			for(int i=0;i<xlmdb.GJDAllInforNum;i++)
			{
				pzLinep2->GJDInforArray[i].spt[0] = xlmdb.GJDAllInforTable[i].BpointToN ;
				pzLinep2->GJDInforArray[i].spt[1] = xlmdb.GJDAllInforTable[i].BpointToE ;
				pzLinep2->GJDInforArray[i].ept[0] = xlmdb.GJDAllInforTable[i].EpointToN ;
				pzLinep2->GJDInforArray[i].ept[1] = xlmdb.GJDAllInforTable[i].EpointToE ;

				_tcscpy( pzLinep2->GJDInforArray[i].BXLName , xlmdb.GJDAllInforTable[i].BXLName ) ;
				_tcscpy( pzLinep2->GJDInforArray[i].EXLName , xlmdb.GJDAllInforTable[i].EXLName ) ;
				pzLinep2->GJDInforArray[i].GJDType = xlmdb.GJDAllInforTable[i].GJDType ;

				_tcscpy( pzLinep2->GJDInforArray[i].gjdname , xlmdb.GJDAllInforTable[i].ToInSGJJDNum ) ;


				//���/�յ��ֳ����
				//���
				_tcscpy( pzLinep2->GJDInforArray[i].GJDBLC , xlmdb.GJDAllInforTable[i].GJDBLC ) ;

				//�յ����

				_tcscpy( pzLinep2->GJDInforArray[i].GJDELC , xlmdb.GJDAllInforTable[i].GJDELC ) ;

			}

			pzLinep2->assertWriteEnabled();
			//2�����÷ֶ���Ϣ
			pzLinep2->FormSectionInfor();


			//�Ӽ����ߵı�־��������л�ȡ��ͨ��ƶ��ߵ��������öεı�־���--------------------------------------------------
			xlpoint TmpSPz,TmpEPz,TmpBZLCPz;
			GJDAllInforStruct TmpGJDInfor;
			GTZX_JYX_ROAD *pJYJX = CreateJYJX(mdbname);

			int irxdNo = 0;
			BZLCStructTable bzlc;
			int iBZLCCount = pJYJX->BZLCarray.GetSize();
			int iGJDCount = pzLinep2->BasicSectionInfor.GetSize();
			for (int j = 0; j < iBZLCCount; j++)
			{
				bzlc =  pJYJX->BZLCarray[j];
				irxdNo = pzLinep2->GetiRXDFromNE(bzlc.X,bzlc.Y);
				if (irxdNo != -1)
				{
					for (int i=0 ; i < iGJDCount; i++)
					{
						TmpGJDInfor = pzLinep2->BasicSectionInfor[i];
						TmpSPz.x = TmpGJDInfor.BpointToN;
						TmpSPz.y = TmpGJDInfor.BpointToE;
						TmpEPz.x = TmpGJDInfor.EpointToN;
						TmpEPz.y = TmpGJDInfor.EpointToE;
						if (TmpGJDInfor.GJDType == -1 && _tcscmp(TmpGJDInfor.BXLName, L"���л���") == 0)
						{
							TmpSPz.lc = pJYJX->PROJ_ML(pJYJX->array, pJYJX->tarray, TmpSPz.x, TmpSPz.y, TmpSPz.x, TmpSPz.y);
							TmpEPz.lc = pJYJX->PROJ_ML(pJYJX->array, pJYJX->tarray, TmpEPz.x, TmpEPz.y, TmpEPz.x, TmpEPz.y);

							TmpBZLCPz.lc =  pJYJX->PROJ_ML(pJYJX->array, pJYJX->tarray, bzlc.X,bzlc.Y, bzlc.X,bzlc.Y);


							//��ĳ���öη�Χ��
							if ((TmpBZLCPz.lc - TmpSPz.lc)>0.001 && (TmpBZLCPz.lc - TmpEPz.lc) < 0.001)
							{
								_tcscpy(bzlc.GJDNum,pzLinep2->GetRXDXLNameByRXDIndex(irxdNo));

								//��֤������Ƿ��Ѿ����˸ñ�־���
								bool bHasAlready = false;
								for (int k = 0; k < pzLinep2->BZLCarray.GetSize();k++)
								{
									if (_tcscmp(pzLinep2->BZLCarray[k].kml,bzlc.kml) == 0)
									{
										bHasAlready = true;
										break;
									}

								}
								if (!bHasAlready)
								{
									pzLinep2->BZLCarray.Add(bzlc);
								}
							}


						}
					}

				}
			}
			if(pJYJX)
			{
				delete pJYJX;
				pJYJX = NULL;
			}

			pzLinep2->GetDLTYLC();
			//----------------------------------------------------------------------------------------------------


		}
		else
		{
			ads_printf(L"\n���Ľ��λ��ܱ�ʧ��!");
			//return ;
		}

		pzLinep2->calsflag=false;
		_tcscpy( pzLinep2->xlname , L"��ͨ��Ƣ���" ); 
		pzLinep2->mdbname = mdbname ;
		pzLinep2->CalNYDL();

		//////////////////////////////////////////////////////////////////////////
		//���������������
		pzLinep2->CalGT2xDLValue();


		//////////////////////////////////////////////////////////////////////////
		//�ݶ����¶�
		XLDataBase_JYX xlmdb;
		APDTAB pLeftSlope,pRightSlope;
		GetSlopeData(mdbname, pGTJXpLinpToIn, pLeftSlope, pRightSlope);
		int NBPD;
		PDTAB* BPD_array;
		NBPD = pRightSlope.GetSize();
		pzLinep2->m_NCM=pRightSlope.GetSize()+1;
		//if (NBPD>0)
		//{
		//for (int i=0 ; i<NBPD; i++)
		//{
		//	BPD_array[i].ml = pRightSlope[i].ml;//ͳһ���
		//	BPD_array[i].Level = pRightSlope[i].Level;//�߳�
		//	BPD_array[i].Rshu = pRightSlope[i].Rshu;//�����߰뾶	
		//	//BPD_array[i].CKml=pLeftSlope[i].CKml;
		//	_tcscpy(BPD_array[i].CKml,pRightSlope[i].CKml);
		//	BPD_array[i].cml=pRightSlope[i].cml;
		//	BPD_array[i].degree=pRightSlope[i].degree;
		//	BPD_array[i].GH=pRightSlope[i].GH;
		//	BPD_array[i].length=pRightSlope[i].length;
		//	BPD_array[i].NameOfWay=pRightSlope[i].NameOfWay;
		//	BPD_array[i].Notes=pRightSlope[i].Notes;
		//	BPD_array[i].RLen=pRightSlope[i].RLen;
		//	BPD_array[i].Rshu=pRightSlope[i].Rshu;
		//}
		//				DesHXSW = (int)(pRightSlope->m_SlopeValue[0].Rshu + 0.1);//��Ʊ��С��λ��
		//				LJorGM = (int)(pRightSlope->m_SlopeValue[0].RLen + 0.1);//��Ʊ����·�绹�ǹ���


		//////////////////////////////////////////////////////////////////////////
		int RXDSum = pzLinep2->GetRXDNum();
		if (RXDSum>0)
			pzLinep2->AllRXDBPDArray.SetSize(RXDSum);
		for (int iRXD=0; iRXD<RXDSum; iRXD++)
		{
			xlmdb.NPD = 0;
			CString  wlcstring;
			CString RXDNAME=pzLinep2->GetRXDXLNameByRXDIndex(iRXD);
			wlcstring.Format(L"��·�� = '%s'",RXDNAME);
			if (xlmdb.Read_XLDbs(mdbname, L"�¶ȱ�", wlcstring))
			{
				if (xlmdb.NPD>0)
				{
					pzLinep2->m_LJorGM = xlmdb.PDB[0].RLen;
					pzLinep2->m_Xsw = _wtoi(xlmdb.PDB[0].Notes);
					pzLinep2->m_DesHXSW = xlmdb.PDB[0].Rshu;
				}
				pzLinep2->SetiRXDBPDArray(iRXD, xlmdb.NPD, xlmdb.PDB, false);
			}
		}
		pzLinep2->Create2XGTBPDArray(pRightSlope);


		//	BPD_array[0].Rshu=BPD_array[NBPD-1].Rshu=0.0;//���ձ��µ������߰뾶Ϊ0
		/////�������жα��µ�

		//pzLinep2->SetBPDArray(NBPD,BPD_array,false);
		pzLinep2->TrsBPDArraytoCM();//����ת��(���µ����� -> CM����)
		//}


	}
	return pzLinep2 ;

}


GTZX_JYX_ROAD* PmPointer::GetRXDPMFromRXDName(CString WayOfMDB,CString NameOfWay,CString NameOfRXD)
{
	//1.�������ж����õ����жα�źͶ���ָ��
	int RXDNum;
	GTZX2_JYX_ROAD *pGTZX2;
	if (NameOfWay=="��ͨ��Ƣ���")
	{
		pGTZX2=CreateGTSJEXPointer(WayOfMDB);
	}
	else
	{
		pGTZX2=CreateJYEX(WayOfMDB,NameOfWay);
	}
	if (NULL!=pGTZX2)
	{
		RXDNum = pGTZX2->GetiRxdNoFromRXDName(NameOfRXD);
	}
	else
	{
		return NULL;
	}

	if (RXDNum==-1)
	{
		return NULL;        
	}

	//2.����GetiRXDJDArray(int iRXD, double jday[][6], int &njd)���õ����жν���
	int njd,SJDno,EJDno;
	double jday[MAXJDNUM][6];
	pGTZX2->GetiRXDJDArray(RXDNum, jday, njd,SJDno,EJDno);
	CArray<GTZX_JYX_ROAD::JDarrayStruct,GTZX_JYX_ROAD::JDarrayStruct>array;
	array.SetSize(njd);
	int i;
	for( i=0;i<njd;i++)
	{
		_stprintf(array[i].JDNum ,L"%lf",jday[i][0]);
		array[i].N = jday[i][1];
		array[i].E = jday[i][2];
		array[i].l1 = jday[i][3];
		array[i].R = jday[i][4];
		array[i].l2 = jday[i][5];
	}

	//3.�õ����жζ���
	CString Temp1;
	Temp1=NameOfRXD;
	Temp1.Trim();
	Temp1=L"���ж���=\"" + Temp1 + "\"";
	Temp1 += " AND " ;
	CString Temp2;
	Temp2=NameOfWay;
	Temp2=L"��·��=\"" + Temp2 + "\"";
	Temp1 += Temp2;
	XLDataBase_JYX xlmdb;
	xlmdb.Read_XLDbs(WayOfMDB,L"���߶�����",Temp1);

	//4.�������жε�GTZX
	ACHAR GH[8],JDGH[8];
	if (njd< 2) return NULL;
	_tcscpy(GH,L"AK"); 
	_tcscpy(JDGH,L"JD");
	GTZX_JYX_ROAD *pzx= new GTZX_JYX_ROAD(njd,array,&GH[0],&JDGH[0]);

	if(pzx==NULL)return NULL ;
	pzx->setINIT(3,3,1,5);
	pzx->mode_display=0;
	pzx->mdbname = WayOfMDB;

	pzx->NDL = xlmdb.NDL2x;
	//ads_printf(L"\nNameOfRXD=%s",NameOfRXD);
	for(i=0;i<pzx->NDL;i++)
	{
		_tcscpy(pzx->DLB[i].BGH,xlmdb.DLB2x[i].BGH);
		_tcscpy(pzx->DLB[i].EGH,xlmdb.DLB2x[i].EGH);
		pzx->DLB[i].BLC = xlmdb.DLB2x[i].BLC;
		pzx->DLB[i].ELC = xlmdb.DLB2x[i].ELC;
		pzx->DLB[i].DL = xlmdb.DLB2x[i].ELC-xlmdb.DLB2x[i].BLC;
		// 		ads_printf(L"\npzx->DLB[%d].BLC=%lf",i,pzx->DLB[i].BLC);
		// 		ads_printf(L"\npzx->DLB[%d].ELC=%lf",i,pzx->DLB[i].ELC);
	}
	pzx->getpxy_DATA();

	if (NameOfWay=="��ͨ��Ƣ���")
	{
		//��pGTZX->DLB��ֵ
		for(i=0;i<pzx->NDL;i++)
		{
			_tcscpy(pzx->DLB[i].BGH,xlmdb.DLB2x[i].BGH);
			_tcscpy(pzx->DLB[i].EGH,xlmdb.DLB2x[i].EGH);
			pzx->DLB[i].BLC = xlmdb.DLB2x[i].BLC;
			pzx->DLB[i].ELC = xlmdb.DLB2x[i].ELC;
			pzx->DLB[i].DL = xlmdb.DLB2x[i].ELC-xlmdb.DLB2x[i].BLC;
		}

		//5. ��pGTZX->QXB��ֵ
		//5.1 ������������߽����
		Temp1=NameOfWay;
		Temp1.Trim();
		Temp1=L"��·��=\"" + Temp1 + "\"";
		CArray<QXBStruct,QXBStruct>QXB;

		QXB.RemoveAll();
		if(xlmdb.Read_XLDbs(WayOfMDB,L"������߽����",Temp1))//
		{	
			QXB.SetSize(xlmdb.NSJQXB);
			for(int i=0;i<xlmdb.NSJQXB;i++)
			{	
				_tcscpy(QXB[i].JDNum , xlmdb.SJQXB[i].JDNum);
				_tcscpy(QXB[i].CKZH , xlmdb.SJQXB[i].CKZH);
				_tcscpy(QXB[i].CKHY , xlmdb.SJQXB[i].CKHY);
				_tcscpy(QXB[i].CKYH , xlmdb.SJQXB[i].CKYH);
				_tcscpy(QXB[i].CKHZ , xlmdb.SJQXB[i].CKHZ);
				QXB[i].a = xlmdb.SJQXB[i].a;
				QXB[i].l1 = xlmdb.SJQXB[i].l1;
				QXB[i].R = xlmdb.SJQXB[i].R;
				QXB[i].l2 = xlmdb.SJQXB[i].l2;
			}
		}
		else
		{
			return NULL;
		}

		//5.2ȡ�����жη�Χ�ڵĽ����Ӧ�����߱�
		pzx->QXB.SetSize(njd);   
		for(i=1;i<njd-1;i++)//1->njd-1
		{
			_tcscpy(pzx->QXB[i].JDNum , QXB[SJDno+i-1].JDNum);
			_tcscpy(pzx->QXB[i].CKZH , QXB[SJDno+i-1].CKZH);
			_tcscpy(pzx->QXB[i].CKHY , QXB[SJDno+i-1].CKHY);
			_tcscpy(pzx->QXB[i].CKYH , QXB[SJDno+i-1].CKYH);
			_tcscpy(pzx->QXB[i].CKHZ , QXB[SJDno+i-1].CKHZ);
			pzx->QXB[i].a =QXB[SJDno+i-1].a;
			pzx->QXB[i].l1 = QXB[SJDno+i-1].l1;
			pzx->QXB[i].R = QXB[SJDno+i-1].R;
			pzx->QXB[i].l2 = QXB[SJDno+i-1].l2;
		}

		//��һ������
		i=0;
		_stprintf(pzx->QXB[i].JDNum ,L"%d", i);
		_stprintf(pzx->QXB[i].CKZH ,L"%s%lf",pzx->DLB[i].EGH,pzx->DLB[i].ELC);
		_tcscpy(pzx->QXB[i].CKHY , pzx->QXB[i].CKZH);
		_tcscpy(pzx->QXB[i].CKYH , pzx->QXB[i].CKZH);
		_tcscpy(pzx->QXB[i].CKHZ , pzx->QXB[i].CKZH);
		pzx->QXB[i].a =0;
		pzx->QXB[i].l1 = 0;
		pzx->QXB[i].R = 0;
		pzx->QXB[i].l2 = 0;
		//���һ������
		i=njd-1;
		_stprintf(pzx->QXB[i].JDNum ,L"%d", i);
		_stprintf(pzx->QXB[i].CKZH ,L"%s%lf",pzx->DLB[pzx->NDL-1].BGH,pzx->DLB[pzx->NDL-1].BLC);
		_tcscpy(pzx->QXB[i].CKHY , pzx->QXB[i].CKZH);
		_tcscpy(pzx->QXB[i].CKYH , pzx->QXB[i].CKZH);
		_tcscpy(pzx->QXB[i].CKHZ , pzx->QXB[i].CKZH);
		pzx->QXB[i].a =0;
		pzx->QXB[i].l1 = 0;
		pzx->QXB[i].R = 0;
		pzx->QXB[i].l2 = 0;

		for (int i=0;i<pzx->QXB.GetSize();i++)
		{
			pzx->QXB[i].KZH=pzx->TYLC1(pzx->QXB[i].CKZH,pzx->NDL,pzx->DLB);
			pzx->QXB[i].KHY=pzx->TYLC1(pzx->QXB[i].CKHY,pzx->NDL,pzx->DLB);
			pzx->QXB[i].KYH=pzx->TYLC1(pzx->QXB[i].CKYH,pzx->NDL,pzx->DLB);
			pzx->QXB[i].KHZ=pzx->TYLC1(pzx->QXB[i].CKHZ,pzx->NDL,pzx->DLB);
		}

	}


	delete pGTZX2;
	pGTZX2=NULL;

	return pzx;  
}




//������·��,���ݿ���,���ж���,�õ�ƽ��ָ��
GTZX_JYX_ROAD* PmPointer::GetpGtzxForXLName(CString WayOfMDB,CString NameOfWay,CString NameOfRXD)
{
	//1.�������ж����õ����жα�źͶ���ָ��
	int RXDNum;
	GTZX_JYX_ROAD *pGtzx = NULL ;
	GTZX2_JYX_ROAD *pGtzx2 = NULL ;
	if (NameOfWay=="��ͨ��ƻ���" || NameOfWay=="���л���" || NameOfWay=="ʩ������")
	{
		//�õ����л���ָ��
		if(NameOfWay=="���л���") 
		{
			pGtzx = CreateJYJX( WayOfMDB );
			//	_tcscpy(pGtzx->JDGH,L"JD");
		}
		else if (NameOfWay=="��ͨ��ƻ���")
		{
			pGtzx = CreateGTSJJXPointer( WayOfMDB );//��ͨ��ƻ��� ָ��
			//	_tcscpy(pGtzx->JDGH,L"GTJJD");
		}
		else//ʩ������
		{
			pGtzx = CreateJYJX(WayOfMDB, NameOfRXD, false );
		}
		if(pGtzx==NULL)return NULL ;
	}
	else if(NameOfWay=="��������" || NameOfWay=="��������" || NameOfWay=="��ͨ��Ƣ���" )
	{
		if(NameOfWay=="��ͨ��Ƣ���")
		{
			pGtzx2=CreateGTSJEXPointer(WayOfMDB);
		}
		else 
		{
			pGtzx2=CreateJYEX(WayOfMDB,NameOfWay);  //����2��
		}

		if (NULL!=pGtzx2) RXDNum = pGtzx2->GetiRxdNoFromRXDName(NameOfRXD);
		else return NULL;
		if (RXDNum==-1)return NULL;        

		//�������жα�ŵõ����ж�ƽ��ָ��
		pGtzx = pGtzx2->GetiRxdtoGTZXplinp(RXDNum);

		if(pGtzx==NULL)return NULL ;
	}
	else pGtzx = NULL ;

	return pGtzx ;
}


//����������ָ��
GTZX_JYX_ROAD* PmPointer::CreateLLXpGtzx( CString mdbname , CString LianLuoXianName )
{

	XLDataBase_JYX xlmdb ;
	GTZX_JYX_ROAD *pGtzxLinpTemp = NULL ;
	CString ToInXLNameString;
	CString XLNameTempToIn =L"��������·��=\"" + LianLuoXianName + "\"";
	if(xlmdb.Read_XLDbs( mdbname , L"������Ϣ��" , XLNameTempToIn ))
	{  
		if(xlmdb.ConnectLineInforNum>0)
		{
			//�����߶�����·��
			ToInXLNameString = xlmdb.ConnectLineTable[0].ToInSGJJDNum ;
			pGtzxLinpTemp = PmPointer::CreateJYJX( mdbname , ToInXLNameString , TRUE );

			if(pGtzxLinpTemp==NULL)return NULL ;

			_tcscpy(pGtzxLinpTemp->JDGH,L"LLJD");
			pGtzxLinpTemp->XLShuXing = LianLuoXianName ;
			pGtzxLinpTemp->YCXLName = xlmdb.ConnectLineTable[0].BXLName ;
			pGtzxLinpTemp->JRXLName = xlmdb.ConnectLineTable[0].EXLName ;
			return pGtzxLinpTemp;
		}
	}

	return pGtzxLinpTemp;
}

GTZX_JYX_ROAD* PmPointer::CreateSGBXpGtzx( CString mdbname , CString SGBXName )
{

	XLDataBase_JYX xlmdb ;
	GTZX_JYX_ROAD *pGtzxLinpTemp = NULL ;
	CString XLNameTemp =L"��·��=\"" + SGBXName + "\"";

	if(xlmdb.Read_XLDbs(mdbname,L"ʩ��������Ϣ��",XLNameTemp))
	{
		if(xlmdb.SGBXInforNum>0)
		{
			SGBXName = xlmdb.SGBXTable[0].SGBXName ;
			XLNameTemp =L"��·��=\"" + SGBXName + "\"";
			pGtzxLinpTemp = PmPointer::CreateJYJX( mdbname , SGBXName , FALSE );
			if(pGtzxLinpTemp==NULL)return NULL ;
			_tcscpy(pGtzxLinpTemp->JDGH,L"LLJD");

			pGtzxLinpTemp->YCXLName = xlmdb.SGBXTable[0].BXLName ;
			pGtzxLinpTemp->JRXLName = xlmdb.SGBXTable[0].EXLName ;
			pGtzxLinpTemp->XLShuXing = SGBXName ;
			return pGtzxLinpTemp;
		}
	}

	return NULL;
}

void PmPointer::GetXLDefaName( CString mdbName , CString XLNameToIn , CString & XLNameToOut )
{
	long MaxIndex = -1;

	int CurNumTmp = 0 ; 
	XLDataBase_JYX xlmdbTmp;
	CString strTmp ;
	CString SelStrTmp ;
	if( XLNameToIn == "ʩ������")
	{
		if(xlmdbTmp.Read_XLDbs(mdbName,L"ʩ��������Ϣ��",L""))
			if(xlmdbTmp.SGBXInforNum>0)
			{
				for(int i=0;i<xlmdbTmp.SGBXInforNum;i++ )
				{
					strTmp = xlmdbTmp.SGBXTable[i].SGBXName ;
					strTmp.Trim();
					SelStrTmp = strTmp.Right(strTmp.GetLength()-8);
					SelStrTmp.Trim();
					CurNumTmp = _wtoi(SelStrTmp);
					if(CurNumTmp > MaxIndex)
					{
						MaxIndex = CurNumTmp ;
					}
				}
			}
			XLNameToOut.Format(L"%s%d",XLNameToIn,MaxIndex+1);
	}
	if( XLNameToIn == "�ֲ�����")
	{

		if(xlmdbTmp.Read_XLDbs(mdbName,L"�ֲ�������Ϣ��",L""))
			if(xlmdbTmp.JBJXInforNum>0)
			{
				for(int i=0;i<xlmdbTmp.JBJXInforNum;i++ )
				{
					strTmp = xlmdbTmp.JBJXTable[i].JBJXName ;

					SelStrTmp = strTmp.Right(strTmp.GetLength()-4);

					SelStrTmp = strTmp.Right(strTmp.GetLength()-4);
					CurNumTmp = _wtoi(SelStrTmp);
					if(CurNumTmp > MaxIndex)
					{
						MaxIndex = CurNumTmp ;
					}
				}
			}
			XLNameToOut.Format(L"%s%d",XLNameToIn,MaxIndex+1);
	}
}

GTZX_JYX_ROAD* PmPointer::CreateJBJXpGtzx( CString mdbname , CString JBJXName )
{
	XLDataBase_JYX xlmdb ;
	GTZX_JYX_ROAD *pGtzxLinpTemp = NULL ;
	CString XLNameTemp =L"��·��=\"" + JBJXName + "\"";

	if(xlmdb.Read_XLDbs(mdbname,L"�ֲ�������Ϣ��",XLNameTemp))
	{
		if(xlmdb.JBJXInforNum>0)
		{
			JBJXName = xlmdb.JBJXTable[0].JBJXName ;
			XLNameTemp =L"��·��=\"" + JBJXName + "\"";
			pGtzxLinpTemp = PmPointer::CreateJYJX( mdbname , JBJXName , FALSE );
			if(pGtzxLinpTemp==NULL)return NULL ;
			_tcscpy(pGtzxLinpTemp->JDGH,L"LLJD");
			pGtzxLinpTemp->XLShuXing = JBJXName ;
			return pGtzxLinpTemp;
		}
	}

	return NULL;
}


bool PmPointer::CalIntersectPt(JD_CENTER* pCurXLPM, GTZX_ROAD* pInterXLPM)
{
	//��ʼ������
	pCurXLPM->m_InterSectData.CurDml = 0.0;
	_tcscpy(pCurXLPM->m_InterSectData.InterCkml, L"\0");
	pCurXLPM->m_InterSectData.CurDesH = pCurXLPM->m_InterSectData.DetaH = pCurXLPM->m_InterSectData.InterDesH = 0.0;
	pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->m_InterSectData.CurFwj = pCurXLPM->m_InterSectData.InterFwj = 0.0;
	pCurXLPM->m_InterSectData.InterN = pCurXLPM->m_InterSectData.interE = 0.0;
	pCurXLPM->m_InterSectData.MinAng = pCurXLPM->m_InterSectData.MinDetaH = 0.0;

	//��ȡ��·�Ľ�����Ϣ
	AcGePoint2dArray InterPtArray;
	double JDArray[50][6];
	for (int i=0; i<50; i++)
		for (int j=0; j<6; j++)
			JDArray[i][j] = 0.0;
	for (int iJD=0; iJD<pCurXLPM->JdCenArray.GetSize(); iJD++)
	{
		JDArray[iJD][0] = iJD;
		JDArray[iJD][1] = pCurXLPM->JdCenArray[iJD].JDN;
		JDArray[iJD][2] = pCurXLPM->JdCenArray[iJD].JDE;
		JDArray[iJD][3] = pCurXLPM->JdCenArray[iJD].A1*pCurXLPM->JdCenArray[iJD].A1 / pCurXLPM->JdCenArray[iJD].R;
		JDArray[iJD][4] = pCurXLPM->JdCenArray[iJD].R;
		JDArray[iJD][5] = pCurXLPM->JdCenArray[iJD].A2*pCurXLPM->JdCenArray[iJD].A2 / pCurXLPM->JdCenArray[iJD].R;
	}

	//��ȡ��·�Ľ�����Ϣ
	double JDarray2[MAXJDNUM][6];
	for (int i=0;i<pInterXLPM->NJD;i++)
	{

		JDarray2[i][0]=i;
		JDarray2[i][1]=pInterXLPM->JDarray[i][1];
		JDarray2[i][2]=pInterXLPM->JDarray[i][2];
		JDarray2[i][3]=pInterXLPM->JDarray[i][3];
		JDarray2[i][4]=pInterXLPM->JDarray[i][4];
		JDarray2[i][5]=pInterXLPM->JDarray[i][5];
	}

	//��·Բ��Ԫ
	int Yxynum=0;
	JDArray[Yxynum][4] =0;
	for (int iXY=0;iXY<pCurXLPM->XYNum;iXY++)
	{
		if (fabs(pCurXLPM->XYArray[iXY][0]-2)<0.001)	//Բ����
		{
			Yxynum++;
			JDArray[Yxynum][4]=iXY;
		}
	}
	JDArray[pCurXLPM->JdCenNum-1][4]=pCurXLPM->XYNum-1;

	//��·Բ��Ԫ
	Yxynum=0;
	JDarray2[Yxynum][4] =0;
	for (int iXY=0;iXY<pInterXLPM->tarray;iXY++)
	{
		if (fabs(pInterXLPM->array[iXY][0]-2)<0.001)
		{
			Yxynum++;
			JDarray2[Yxynum][4]=iXY;
		}
	}
	JDarray2[pInterXLPM->NJD-1][4]=pInterXLPM->tarray-1;

	//�����·�Ƿ��ཻ
	pCurXLPM->CheckIfXLintersect(pCurXLPM->XYArray,JDArray,pCurXLPM->JdCenNum,pInterXLPM->array,JDarray2,pInterXLPM->NJD,&InterPtArray);

	int InterPtSum = InterPtArray.length();
	if (InterPtSum==0)
		return false;

	BAS_DRAW_FUN::xlpoint PZ;
	double Curcml,RelCml;
	double CurSlope,RelSlope;
	CString GH;

	if (InterPtSum>0)
	{
		AcGePoint2d Pt = InterPtArray.at(0);
		pCurXLPM->m_InterSectData.InterN = Pt.x;
		pCurXLPM->m_InterSectData.interE = Pt.y;
		Curcml = pCurXLPM->PROJ_ML(Pt.x, Pt.y);
		pCurXLPM->m_InterSectData.CurDml = pCurXLPM->XLC(Curcml, GH);

		pInterXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
		pInterXLPM->TrsCmltoCkml(RelCml, pCurXLPM->m_InterSectData.InterCkml);

		PZ.lc = Curcml;
		pCurXLPM->xlpoint_pz(&PZ);
		pCurXLPM->m_InterSectData.CurFwj = PZ.a;
		PZ.lc = RelCml;
		pInterXLPM->xlpoint_pz(pInterXLPM->array, pInterXLPM->tarray, &PZ);
		pCurXLPM->m_InterSectData.InterFwj = PZ.a;
		pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->CalJiaJiao(pCurXLPM->m_InterSectData.CurFwj, pCurXLPM->m_InterSectData.InterFwj);
		pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->dms_rad(pCurXLPM->m_InterSectData.JiaJiao);


		pCurXLPM->m_InterSectData.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
		pCurXLPM->m_InterSectData.InterDesH = pInterXLPM->rdes_level(RelCml, RelSlope);
		pCurXLPM->m_InterSectData.DetaH = pCurXLPM->m_InterSectData.CurDesH - pCurXLPM->m_InterSectData.InterDesH;

	}

	return true;
}


bool PmPointer::CalIntersectPt(JD_CENTER* pCurXLPM, GTZX2_ROAD* pInterXLPM)
{
	pCurXLPM->m_InterSectData.CurDml = 0.0;
	_tcscpy(pCurXLPM->m_InterSectData.InterCkml, L"\0");
	pCurXLPM->m_InterSectData.CurDesH = pCurXLPM->m_InterSectData.DetaH = pCurXLPM->m_InterSectData.InterDesH = 0.0;
	pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->m_InterSectData.CurFwj = pCurXLPM->m_InterSectData.InterFwj = 0.0;
	pCurXLPM->m_InterSectData.InterN = pCurXLPM->m_InterSectData.interE = 0.0;
	pCurXLPM->m_InterSectData.MinAng = pCurXLPM->m_InterSectData.MinDetaH = 0.0;
	AcGePoint2dArray InterPtArray;
	double JDArray[50][6];
	for (int iJD=0; iJD<pCurXLPM->JdCenNum; iJD++)
	{
		JDArray[iJD][0] = iJD;
		JDArray[iJD][1] = pCurXLPM->JdCenArray[iJD].JDN;
		JDArray[iJD][2] = pCurXLPM->JdCenArray[iJD].JDE;
		JDArray[iJD][3] = pCurXLPM->JdCenArray[iJD].A1*pCurXLPM->JdCenArray[iJD].A1 / pCurXLPM->JdCenArray[iJD].R;
		JDArray[iJD][4] = pCurXLPM->JdCenArray[iJD].R;
		JDArray[iJD][5] = pCurXLPM->JdCenArray[iJD].A2*pCurXLPM->JdCenArray[iJD].A2 / pCurXLPM->JdCenArray[iJD].R;
	}

	double JDarray2[MAXJDNUM][6];
	for (int i=0;i<pInterXLPM->NJD;i++)
	{

		JDarray2[i][0]=i;
		JDarray2[i][1]=pInterXLPM->JDarray[i][1];
		JDarray2[i][2]=pInterXLPM->JDarray[i][2];
		JDarray2[i][3]=pInterXLPM->JDarray[i][3];
		JDarray2[i][4]=pInterXLPM->JDarray[i][4];
		JDarray2[i][5]=pInterXLPM->JDarray[i][5];
	}
	int Yxynum=0;
	JDArray[Yxynum][4] =0;
	for (int iXY=0;iXY<pCurXLPM->XYNum;iXY++)
	{
		if (fabs(pCurXLPM->XYArray[iXY][0]-2)<0.001)
		{
			Yxynum++;
			JDArray[Yxynum][4]=iXY;
		}
	}
	JDArray[pCurXLPM->JdCenNum-1][4]=pCurXLPM->XYNum-1;

	Yxynum=0;
	JDarray2[Yxynum][4] =0;
	for (int iXY=0;iXY<pInterXLPM->tarray;iXY++)
	{
		if (fabs(pInterXLPM->array[iXY][0]-2)<0.001)
		{
			Yxynum++;
			JDarray2[Yxynum][4]=iXY;
		}
	}
	JDarray2[pInterXLPM->NJD-1][4]=pInterXLPM->tarray-1;
	pCurXLPM->CheckIfXLintersect(pCurXLPM->XYArray,JDArray,pCurXLPM->JdCenNum,pInterXLPM->array,JDarray2,pInterXLPM->NJD,&InterPtArray);
	int InterPtSum = InterPtArray.length();
	if (InterPtSum==0)
		return false;

	BAS_DRAW_FUN::xlpoint PZ;
	double Curcml,RelCml;
	double CurSlope,RelSlope;
	CString GH;

	if (InterPtSum>0)
	{
		AcGePoint2d Pt = InterPtArray.at(0);
		pCurXLPM->m_InterSectData.InterN = Pt.x;
		pCurXLPM->m_InterSectData.interE = Pt.y;
		Curcml = pCurXLPM->PROJ_ML(Pt.x, Pt.y);
		pCurXLPM->m_InterSectData.CurDml = pCurXLPM->XLC(Curcml, GH);

		pInterXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
		pInterXLPM->TrsCmltoCkml(RelCml, pCurXLPM->m_InterSectData.InterCkml);

		PZ.lc = Curcml;
		pCurXLPM->xlpoint_pz(&PZ);
		pCurXLPM->m_InterSectData.CurFwj = PZ.a;
		PZ.lc = RelCml;
		pInterXLPM->xlpoint_pz(pInterXLPM->array, pInterXLPM->tarray, &PZ);
		pCurXLPM->m_InterSectData.InterFwj = PZ.a;
		pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->CalJiaJiao(pCurXLPM->m_InterSectData.CurFwj, pCurXLPM->m_InterSectData.InterFwj);
		pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->dms_rad(pCurXLPM->m_InterSectData.JiaJiao);

		pCurXLPM->m_InterSectData.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
		pCurXLPM->m_InterSectData.InterDesH = pInterXLPM->rdes_level(RelCml, RelSlope);
		pCurXLPM->m_InterSectData.DetaH = pCurXLPM->m_InterSectData.CurDesH - pCurXLPM->m_InterSectData.InterDesH;

	}

	return true;
}

bool PmPointer::CalIntersectPt(JD_CENTER* pCurXLPM, GTZX_JYX_ROAD* pInterXLPM)
{
	if (!pCurXLPM || !pInterXLPM)
		return false;

	pCurXLPM->m_InterSectData.CurDml = 0.0;
	_tcscpy(pCurXLPM->m_InterSectData.InterCkml, L"\0");
	pCurXLPM->m_InterSectData.CurDesH = pCurXLPM->m_InterSectData.DetaH = pCurXLPM->m_InterSectData.InterDesH = 0.0;
	pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->m_InterSectData.CurFwj = pCurXLPM->m_InterSectData.InterFwj = 0.0;
	pCurXLPM->m_InterSectData.InterN = pCurXLPM->m_InterSectData.interE = 0.0;
	pCurXLPM->m_InterSectData.MinAng = pCurXLPM->m_InterSectData.MinDetaH = 0.0;

	AcGePoint2dArray InterPtArray;
	double JDArray[50][6];
	for (int i=0; i<50; i++)
		for (int j=0; j<6; j++)
			JDArray[i][j] = 0.0;
	for (int iJD=0; iJD<pCurXLPM->JdCenArray.GetSize(); iJD++)
	{
		JDArray[iJD][0] = iJD;
		JDArray[iJD][1] = pCurXLPM->JdCenArray[iJD].N;
		JDArray[iJD][2] = pCurXLPM->JdCenArray[iJD].E;
		JDArray[iJD][3] = pCurXLPM->JdCenArray[iJD].A1;
		JDArray[iJD][4] = pCurXLPM->JdCenArray[iJD].R;
		JDArray[iJD][5] = pCurXLPM->JdCenArray[iJD].A2;
	}

	double JDarray2[MAXJDNUM][6];
	for (int i=0;i<pInterXLPM->NJD;i++)
	{

		JDarray2[i][0]=i;
		JDarray2[i][1]=pInterXLPM->JDarray[i].N;
		JDarray2[i][2]=pInterXLPM->JDarray[i].E;
		JDarray2[i][3]=pInterXLPM->JDarray[i].l1;
		JDarray2[i][4]=pInterXLPM->JDarray[i].R;
		JDarray2[i][5]=pInterXLPM->JDarray[i].l2;
	}
	int Yxynum=0;
	JDArray[Yxynum][4] =0;
	for (int iXY=0;iXY<pCurXLPM->XYNum;iXY++)
	{
		if (fabs(pCurXLPM->XYArray[iXY][0]-2)<0.001)
		{
			Yxynum++;
			JDArray[Yxynum][4]=iXY;
		}
	}
	JDArray[pCurXLPM->JdCenNum-1][4]=pCurXLPM->XYNum-1;

	Yxynum=0;
	JDarray2[Yxynum][4] =0;
	for (int iXY=0;iXY<pInterXLPM->tarray;iXY++)
	{
		if (fabs(pInterXLPM->array[iXY][0]-2)<0.001)
		{
			Yxynum++;
			JDarray2[Yxynum][4]=iXY;
		}
	}
	JDarray2[pInterXLPM->NJD-1][4]=pInterXLPM->tarray-1;
	pCurXLPM->CheckIfXLintersect(pCurXLPM->XYArray,JDArray,pCurXLPM->JdCenNum,pInterXLPM->array,JDarray2,pInterXLPM->NJD,&InterPtArray);

	int InterPtSum = InterPtArray.length();
	if (InterPtSum==0)
		return false;

	BAS_DRAW_FUN::xlpoint PZ;
	BAS_DRAW_FUN_JYX::xlpoint PZ_JYX;
	double Curcml,RelCml;
	double CurSlope,RelSlope;
	CString GH;

	if (InterPtSum>0)
	{
		AcGePoint2d Pt = InterPtArray.at(0);
		pCurXLPM->m_InterSectData.InterN = Pt.x;
		pCurXLPM->m_InterSectData.interE = Pt.y;
		//ads_printf(L"Pt=%f %f\n",Pt.x,Pt.y);
		Curcml = pCurXLPM->PROJ_ML(Pt.x, Pt.y);
		pCurXLPM->m_InterSectData.CurDml = pCurXLPM->XLC(Curcml, GH);

		pInterXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
		pInterXLPM->TrsCmltoCkml(RelCml, pCurXLPM->m_InterSectData.InterCkml);

		PZ.lc = Curcml;
		pCurXLPM->xlpoint_pz(&PZ);
		pCurXLPM->m_InterSectData.CurFwj = PZ.a;
		PZ_JYX.lc = RelCml;
		pInterXLPM->xlpoint_pz(pInterXLPM->array, pInterXLPM->tarray, &PZ_JYX);
		pCurXLPM->m_InterSectData.InterFwj = PZ_JYX.a;
		pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->CalJiaJiao(pCurXLPM->m_InterSectData.CurFwj, pCurXLPM->m_InterSectData.InterFwj);
		pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->dms_rad(pCurXLPM->m_InterSectData.JiaJiao);


		pCurXLPM->m_InterSectData.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
		pCurXLPM->m_InterSectData.InterDesH = pInterXLPM->rdes_level(RelCml, RelSlope);
		pCurXLPM->m_InterSectData.DetaH = pCurXLPM->m_InterSectData.CurDesH - pCurXLPM->m_InterSectData.InterDesH;

	}

	return true;
}


bool PmPointer::CalIntersectPt(JD_CENTER* pCurXLPM, GTZX2_JYX_ROAD* pInterXLPM)
{
	pCurXLPM->m_InterSectData.CurDml = 0.0;
	_tcscpy(pCurXLPM->m_InterSectData.InterCkml, L"\0");
	pCurXLPM->m_InterSectData.CurDesH = pCurXLPM->m_InterSectData.DetaH = pCurXLPM->m_InterSectData.InterDesH = 0.0;
	pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->m_InterSectData.CurFwj = pCurXLPM->m_InterSectData.InterFwj = 0.0;
	pCurXLPM->m_InterSectData.InterN = pCurXLPM->m_InterSectData.interE = 0.0;
	pCurXLPM->m_InterSectData.MinAng = pCurXLPM->m_InterSectData.MinDetaH = 0.0;
	AcGePoint2dArray InterPtArray;
	double JDArray[50][6];
	for (int iJD=0; iJD<pCurXLPM->JdCenNum; iJD++)
	{
		JDArray[iJD][0] = iJD;
		JDArray[iJD][1] = pCurXLPM->JdCenArray[iJD].JDN;
		JDArray[iJD][2] = pCurXLPM->JdCenArray[iJD].JDE;
		JDArray[iJD][3] = pCurXLPM->JdCenArray[iJD].A1*pCurXLPM->JdCenArray[iJD].A1 / pCurXLPM->JdCenArray[iJD].R;
		JDArray[iJD][4] = pCurXLPM->JdCenArray[iJD].R;
		JDArray[iJD][5] = pCurXLPM->JdCenArray[iJD].A2*pCurXLPM->JdCenArray[iJD].A2 / pCurXLPM->JdCenArray[iJD].R;
	}

	double JDarray2[MAXJDNUM][6];
	for (int i=0;i<pInterXLPM->NJD;i++)
	{

		JDarray2[i][0]=i;
		JDarray2[i][1]=pInterXLPM->JDarray[i].N;
		JDarray2[i][2]=pInterXLPM->JDarray[i].E;
		JDarray2[i][3]=pInterXLPM->JDarray[i].l1;
		JDarray2[i][4]=pInterXLPM->JDarray[i].R;
		JDarray2[i][5]=pInterXLPM->JDarray[i].l2;
	}
	int Yxynum=0;
	JDArray[Yxynum][4] =0;
	for (int iXY=0;iXY<pCurXLPM->XYNum;iXY++)
	{
		if (fabs(pCurXLPM->XYArray[iXY][0]-2)<0.001)
		{
			Yxynum++;
			JDArray[Yxynum][4]=iXY;
		}
	}
	JDArray[pCurXLPM->JdCenNum-1][4]=pCurXLPM->XYNum-1;
	Yxynum=0;
	JDarray2[Yxynum][4] =0;
	for (int iXY=0;iXY<pInterXLPM->tarray;iXY++)
	{
		if (fabs(pInterXLPM->array[iXY][0]-2)<0.001)
		{
			Yxynum++;
			JDarray2[Yxynum][4]=iXY;
		}
	}
	JDarray2[pInterXLPM->NJD-1][4]=pInterXLPM->tarray-1;
	pCurXLPM->CheckIfXLintersect(pCurXLPM->XYArray,JDArray,pCurXLPM->JdCenNum,pInterXLPM->array,JDarray2,pInterXLPM->NJD,&InterPtArray);
	int InterPtSum = InterPtArray.length();
	if (InterPtSum==0)
		return false;

	BAS_DRAW_FUN::xlpoint PZ;
	BAS_DRAW_FUN_JYX::xlpoint PZ_JYX;
	double Curcml,RelCml;
	double CurSlope,RelSlope;
	CString GH;

	if (InterPtSum>0)
	{
		AcGePoint2d Pt = InterPtArray.at(0);
		pCurXLPM->m_InterSectData.InterN = Pt.x;
		pCurXLPM->m_InterSectData.interE = Pt.y;
		Curcml = pCurXLPM->PROJ_ML(Pt.x, Pt.y);
		pCurXLPM->m_InterSectData.CurDml = pCurXLPM->XLC(Curcml, GH);

		pInterXLPM->TrsNEtoCml(Pt.x, Pt.y, RelCml);
		pInterXLPM->TrsCmltoCkml(RelCml, pCurXLPM->m_InterSectData.InterCkml);

		PZ.lc = Curcml;
		pCurXLPM->xlpoint_pz(&PZ);
		pCurXLPM->m_InterSectData.CurFwj = PZ.a;
		PZ_JYX.lc = RelCml;
		pInterXLPM->xlpoint_pz(pInterXLPM->array, pInterXLPM->tarray, &PZ_JYX);
		pCurXLPM->m_InterSectData.InterFwj = PZ_JYX.a;
		pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->CalJiaJiao(pCurXLPM->m_InterSectData.CurFwj, pCurXLPM->m_InterSectData.InterFwj);
		pCurXLPM->m_InterSectData.JiaJiao = pCurXLPM->dms_rad(pCurXLPM->m_InterSectData.JiaJiao);

		pCurXLPM->m_InterSectData.CurDesH = pCurXLPM->rdes_level(Curcml, CurSlope);
		pCurXLPM->m_InterSectData.InterDesH = pInterXLPM->rdes_level(RelCml, RelSlope);
		pCurXLPM->m_InterSectData.DetaH = pCurXLPM->m_InterSectData.CurDesH - pCurXLPM->m_InterSectData.InterDesH;

	}

	return true;
}
void PmPointer::GetSlopeData(CString NameOfMDB,GTZX_JYX_ROAD *pGTZX,APDTAB &pLeftSlope,APDTAB &pRightSlope,CString XLName)
{
	//1.����ͨ��ƻ��ߵ��¶ȣ�������ͨ��ƻ���ָ��
	XLDataBase_JYX xlmdb ;

	CString strNameOfWay1=L"��·��=\""+XLName+"\"";

	if(xlmdb.Read_XLDbs(NameOfMDB,L"�����¶ȱ�",strNameOfWay1))
	{
		PDTAB temp;
		ACHAR GH[8];
		for (int i=0;i<xlmdb.NPD;i++)
		{
			_tcscpy(GH,xlmdb.PDB[i].GH);
			_tcscpy(temp.CKml,LCchr(GH,xlmdb.PDB[i].ml,3,1));
			temp.ml=pGTZX->TYLC1(temp.CKml,pGTZX->NDL,pGTZX->DLB);
			temp.Level=xlmdb.PDB[i].Level;
			temp.degree=xlmdb.PDB[i].degree/1000.0;
			temp.length=xlmdb.PDB[i].length;
			temp.Rshu=xlmdb.PDB[i].Rshu;
			temp.RLen=xlmdb.PDB[i].RLen;
			temp.Notes=xlmdb.PDB[i].Notes;
			temp.GH.Format(L"%s",GH);
			pLeftSlope.Add(temp);
			pRightSlope.Add(temp);
		}
	}

	//XLName="��ͨ��ƻ���";
	XLName = "��ƻ���";
	//2.���Ľ��¶�
	//CString strNameOfDel="(�Ľ�����=\"";
	//strNameOfDel += "���еȸ߸���\"" ;

	//strNameOfDel += " OR " ;
	//CString strTemp="�Ľ�����=\"";
	//strTemp += "��ͨ��ƻ��߸���\"" ;
	//strNameOfDel += strTemp ;

	//strNameOfDel += " OR " ;
	//CString strTemp1="�Ľ�����=\"";
	//strTemp1 += "��ͨ��ƶ��߸���\")" ;
	//strNameOfDel += strTemp1 ;

	//strNameOfDel += " AND " ;
	CString  strNameOfWay=L"��·��=\""+ XLName +"\"";;
	//strNameOfDel += strNameOfWay ;

	CArray<CString,CString>strZPGJ;
	CString strXLName;
	//if(xlmdb.Read_XLDbs(NameOfMDB,L"�Ľ��λ��ܱ�", strNameOfWay))//���Ľ�������
	//{
	//	for(int i=0;i<xlmdb.GJDAllInforNum;i++)
	//	{
	//		strXLName.Format(L"%s",xlmdb.GJDAllInforTable[i].ToOutSGJJDNum);
	//		strZPGJ.Add(strXLName);
	//	}
	//}

	if(xlmdb.Read_XLDbs(NameOfMDB,L"�����ݶ�������", strNameOfWay))//���Ľ�������
	{
		for(int i=0;i<xlmdb.FZZDMNum;i++)
		{
			strXLName.Format(L"%s",xlmdb.FZZDMArray[i].DLName);
			strZPGJ.Add(strXLName);
		}
	}


	MPDTAB m_MPD;
	APDTAB m_APD;

	m_MPD.SetSize(strZPGJ.GetSize());
	for (int i=0;i<strZPGJ.GetSize();i++)
	{
		CString strNameOfWay = strZPGJ.GetAt(i); 
		strNameOfWay=L"������=\""+strNameOfWay+"\"";
		xlmdb.Read_XLDbs(NameOfMDB,L"�����ݶ����¶ȱ�",strNameOfWay);
		PDTAB tempPDTAB;

		ACHAR strTemp[80];
		ACHAR GH[8];
		for (int j=0;j<xlmdb.NPD;j++)
		{
			tempPDTAB.NameOfWay=xlmdb.PDB[j].NameOfWay;
			//_stprintf(strTemp,L"%s%lf",PDB[j].GH,PDB[j].ml);
			_tcscpy(GH,xlmdb.PDB[j].GH);
			_tcscpy(tempPDTAB.CKml,LCchr(GH,xlmdb.PDB[j].ml,3,1));
			tempPDTAB.ml=TYLC1(tempPDTAB.CKml,pGTZX->NDL,pGTZX->DLB);
			tempPDTAB.Level=xlmdb.PDB[j].Level;
			tempPDTAB.degree=xlmdb.PDB[j].degree/1000.0;
			tempPDTAB.length=xlmdb.PDB[j].length;
			tempPDTAB.Rshu=xlmdb.PDB[j].Rshu;
			tempPDTAB.RLen=xlmdb.PDB[j].RLen;
			tempPDTAB.Notes=xlmdb.PDB[j].Notes;
			tempPDTAB.GH.Format(L"%s",GH);
			m_APD.Add(tempPDTAB);
		}
		m_MPD.GetAt(i) = m_APD;
		m_APD.RemoveAll();
	}



	//3.�����������¶�
	for (int i=0;i<m_MPD.GetSize();i++)
	{
		m_APD.RemoveAll();
		m_APD = m_MPD.GetAt(i);


		//3.1�ҵ�Ҫ�滻����ʼ�¶Ⱥ�

		double dtmp1 = m_APD[0].ml;
		double dtmp2 = m_APD[m_APD.GetSize()-1].ml;

		int StNumLeft,EdNumLeft;
		int StNumRight,EdNumRight;

		int flag;
		if (m_APD[1].Notes=="���еȸ߸���")
		{
			flag=0;
			StNumLeft = FindSlopeNum(pLeftSlope,m_APD[0].ml);
			EdNumLeft = FindSlopeNum(pLeftSlope,m_APD[m_APD.GetSize()-1].ml);
			StNumRight = FindSlopeNum(pRightSlope,m_APD[0].ml);
			EdNumRight = FindSlopeNum(pRightSlope,m_APD[m_APD.GetSize()-1].ml);
		}
		else if (m_APD[1].Notes=="��ͨ��ƻ��߸���")
		{
			flag=1;
			StNumLeft = FindSlopeNum(pLeftSlope,m_APD[0].ml);
			EdNumLeft = FindSlopeNum(pLeftSlope,m_APD[m_APD.GetSize()-1].ml);
		}
		else 
		{
			flag=2;
			StNumRight = FindSlopeNum(pRightSlope,m_APD[0].ml);
			EdNumRight = FindSlopeNum(pRightSlope,m_APD[m_APD.GetSize()-1].ml);
		}


		//3.2.remove StNum<Num<EdNum+1 �ı��µ�aa
		int len=0;
		if (flag==0)
		{
			len = EdNumLeft - StNumLeft;
			pLeftSlope.RemoveAt(StNumLeft+1,len);
			len = EdNumRight - StNumRight;
			pRightSlope.RemoveAt(StNumRight+1,len);
		}
		else if (flag==1)
		{
			len = EdNumLeft - StNumLeft;
			pLeftSlope.RemoveAt(StNumLeft+1,len);
		}
		else
		{
			len = EdNumRight - StNumRight;
			pRightSlope.RemoveAt(StNumRight+1,len);
		}


		//3.3.add m_APD��0~Size-1�ı��µ�
		for (int k=0;k<m_APD.GetSize();k++)
		{
			if (flag==0)
			{
				pLeftSlope.Add(m_APD.GetAt(k));
				pRightSlope.Add(m_APD.GetAt(k));
			}
			else if (flag==1)
			{
				pLeftSlope.Add(m_APD.GetAt(k));
			}
			else
			{
				pRightSlope.Add(m_APD.GetAt(k));
			}
		}

		//3.4���������
		Ordel(pLeftSlope);
		Ordel(pRightSlope);


		//Zhujiang-�޳��ظ����¶����� 20090618
		DeleteRepeatData(pLeftSlope);
		DeleteRepeatData(pRightSlope);
	}


	for (int i=0;i<pLeftSlope.GetSize();i++)
	{
		pLeftSlope[i].NameOfWay = "��ͨ��ƻ���";
	}

	for (int i=0;i<pRightSlope.GetSize();i++)
	{
		pRightSlope[i].NameOfWay = "��ͨ��Ƣ���";
	}



}
void PmPointer::GetSlopeDataSJJX(CString NameOfMDB, GTZX_JYX_ROAD* pGTZX, APDTAB& pSlope)
{
	XLDataBase_JYX xlmdb;
	CString XLName = L"��ƻ���";
	CString strNameOfWay1 = L"��·��=\"" + XLName + "\"";

	if (xlmdb.Read_XLDbs(NameOfMDB, L"�����¶ȱ�", strNameOfWay1))
	{
		PDTAB temp;
		ACHAR GH[8];
		for (int i = 0; i < xlmdb.NPD; i++)
		{
			_tcscpy(GH, xlmdb.PDB[i].GH);
			_tcscpy(temp.CKml, LCchr(GH, xlmdb.PDB[i].ml, 3, 1));
			temp.ml = pGTZX->TYLC1(temp.CKml, pGTZX->NDL, pGTZX->DLB);
			temp.Level = xlmdb.PDB[i].Level;
			temp.degree = xlmdb.PDB[i].degree / 1000.0;
			temp.length = xlmdb.PDB[i].length;
			temp.Rshu = xlmdb.PDB[i].Rshu;
			temp.RLen = xlmdb.PDB[i].RLen;
			temp.Notes = xlmdb.PDB[i].Notes;
			temp.GH.Format(L"%s", GH);
			pSlope.Add(temp);
		}
	}

	Ordel(pSlope);
	DeleteRepeatData(pSlope);
}
int PmPointer::FindSlopeNum(APDTAB &pSlope,double ml)
{
	int i;
	int num=0;
	for(i=1;i<pSlope.GetSize();i++)
	{
		num=i-1;
		if (ml<pSlope[i].ml-0.001)
		{
			break;
		}
	}
	return num;

}

void PmPointer::Ordel(APDTAB&Slope)
{
	PDTAB temp;
	for (int i=0;i<Slope.GetSize();i++)
	{
		for (int j=i+1;j<Slope.GetSize();j++)
		{
			if (Slope[i].ml>Slope[j].ml)
			{
				temp = Slope[i];
				Slope[i] = Slope[j];
				Slope[j] = temp;
			}
		}
	}
}

void PmPointer::DeleteRepeatData(APDTAB&Slope)
{

	APDTAB PDTmp;
	PDTmp.RemoveAll();

	bool IsNeedAdd = true;
	for(int i=0;i<Slope.GetSize();i++)
	{
		IsNeedAdd = true;
		if(PDTmp.GetSize()==0)
			PDTmp.Add(Slope[i]);
		for(int j=0;j<PDTmp.GetSize();j++)
		{
			if(fabs(Slope[i].ml - PDTmp[j].ml) < 0.0001)
			{
				IsNeedAdd = false;
				break;
			} 
		}
		if (IsNeedAdd)
			PDTmp.Add(Slope[i]);
	}

	Slope.RemoveAll();

	Slope.Append(PDTmp);


}
bool PmPointer::SJXorJYX(CString MDBpath ,int i,bool ErrMes)
{
	XLDataBase myXLDataBase;
	XLDataBase_JYX myXLDataBase_JYX;
	int isEX=0;
	myXLDataBase.IsMdb2(MDBpath,isEX);
	CString Err;
	if (ErrMes)
	{
		if (i==0)
		{
			Err="����·������ƻ��ߣ�������ѡ����·��";
		}
		else if (i==1)
		{
			Err="����·����������ߣ�������ѡ����·��";
		}
		else if (i==2)
		{
			Err="����·����������ߣ�������ѡ����·��";
		}
		else if (i==3)
		{
			Err="����·���Ǽ��л��ߣ�������ѡ����·��";
		}
		else if (i==4)
		{
			Err="����·���Ǽ��������ߣ�������ѡ����·��";
		}
		else if (i==5)
		{
			Err="����·���Ǽ����ߣ�������ѡ����·��";
		}
		else if (i==6)
		{
			Err="����·��������ߣ�������ѡ����·��";
		}
		else if (i==7)
		{
			Err="����·������ƶ��ߣ�������ѡ����·��";
		}
		else if (i==8)
		{
			Err="����·û�й�ͨ����ߣ�������ѡ����·��";
		}
		else
		{
			Err="";
		}

	}
	///////////
	if (isEX==-1)//���ݿ�Ϊ�����߽�һ���ж��ǲ��Ƕ���
	{
		myXLDataBase_JYX.Read_XLDbs(MDBpath,L"�������߱�",L"");
		if (abs(myXLDataBase_JYX.NQX2x)>0.1)
		{
			isEX=4;//���ж���
		} 
		else
		{
			isEX=3;//���л���
		}

	}
	///////////
	if (i==8)//�жϹ�ͨ�����
	{
		CString Temp; 
		Temp ="��ͨ��ƻ���";
		Temp=L"��·��=\""+ Temp +"\"";

		if(myXLDataBase_JYX.Read_XLDbs(MDBpath,L"�������ݱ�",Temp))//������
		{  
			return true;
		}
		else
		{
			if (ErrMes)
			{
				AfxMessageBox(Err);
			}
			return false;
		}
	}
	//////////
	if (i==7)//�ж���ƶ���
	{
		if ((isEX==1)||(isEX==2))
		{
			return true;
		}
		else
		{
			if (ErrMes)
			{
				AfxMessageBox(Err);
			}
			return false;
		}
	}
	//////////
	if (i==6)//�ж������
	{
		if (isEX<=2)
		{
			return true;
		}
		else
		{
			if (ErrMes)
			{
				AfxMessageBox(Err);
			}
			return false;
		}
	}
	////////
	if (i==5)//�жϼ�����
	{
		if (isEX==3)
		{
			return true;
		}
		else if (isEX==4)
		{
			return true;
		}
		else
		{
			if (ErrMes)
			{
				AfxMessageBox(Err);
			}
			return false;
		}
	}
	////////
	if (i==isEX)
	{
		true;
	}
	else 
	{
		if (ErrMes)
		{
			AfxMessageBox(Err);
		}
		return false;
	}
}

/*===================================================================
������:CalIntersectPt
------------------------------------------------------------
��������:���ݹ�·ƽ��ָ����㽻�����Ϣ
------------------------------------------------------------
����:	JD_CENTER*		pCurXLPM		��·ƽ��ָ��
------------------------------------------------------------
���:	bool			����ֵ			�Ƿ�ɹ����㽻����Ϣ
		true�����ɹ�����
		false����δ�ܳɹ����㣬����δ�ܵõ���·ƽ��ָ���Լ���·���·�����޽�
------------------------------------------------------------
�㷨����:
------------------------------------------------------------
��д����:20191107
------------------------------------------------------------
��ע��
=====================================================================*/
bool PmPointer::CalIntersectPt(JD_CENTER* pCurXLPM)
{
	CString RailName = pCurXLPM->m_RelRailName;
	bool IsInterSect = false;
	if (!pCurXLPM->m_RelRailName.IsEmpty())
	{
		CString JXorEX, ProStr, ProName, MdfName, RailMdbName;
		BAS_DRAW_FUN BasFun;
		if (RailName.ReverseFind('\\') == -1)
		{
			JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);

			//��ȡ��·���ݿ�·��
			ProStr = RailName.Mid(0, RailName.GetLength() - 2);

			ProName = BasFun.GetProByMdbName(pCurXLPM->mdbname);
			MdfName = pCurXLPM->mdbname;
			MdfName.Replace(ProName + L".mdb", L"���Ƶ�·.mdf");
			RailMdbName = MdfName;
			RailMdbName.Replace(L"\\���Ƶ�·.mdf", L"\\" + ProStr + ".mdb");
		}
		else
		{
			RailName = BasFun.GetProByMdbName(RailName);
			JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);

			//��ȡ��·���ݿ�·��
			ProStr = RailName.Mid(0, RailName.GetLength() - 2);

			ProName = BasFun.GetProByMdbName(pCurXLPM->mdbname);
			RailMdbName = pCurXLPM->m_RelRailName;
			RailMdbName.Replace(RailName, ProStr);
		}

		//CString JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);

		////��ȡ��·���ݿ�·��
		//CString ProStr = RailName.Mid(0, RailName.GetLength() - 2);
		//
		//CString ProName = BasFun.GetProByMdbName(pCurXLPM->mdbname);
		//CString MdfName = pCurXLPM->mdbname;
		//MdfName.Replace(ProName + L".mdb", L"���Ƶ�·.mdf");
		//CString RailMdbName = MdfName;
		//RailMdbName.Replace(L"\\���Ƶ�·.mdf", L"\\" + ProStr + ".mdb");


		GTZX_ROAD* pSJJX = NULL;
		GTZX2_ROAD* pSJEX = NULL;
		GTZX_JYX_ROAD* pJYJX = NULL;
		GTZX2_JYX_ROAD* pJYEX = NULL;

		bool isSJX = PmPointer::SJXorJYX(RailMdbName, 6, false);
		pCurXLPM->m_IsJYX = !isSJX;
		if (JXorEX == L"����")
		{
			if (isSJX)
			{
				pSJJX = PmPointer::CreateSJJX(RailMdbName);
				if(pSJJX)
					IsInterSect = PmPointer::CalIntersectPt(pCurXLPM, pSJJX);

			}
			else
			{
				pJYJX = PmPointer::CreateGTSJJXPointer(RailMdbName);
				if(pJYJX)
					IsInterSect = PmPointer::CalIntersectPt(pCurXLPM, pJYJX);

			}
		}
		else if (JXorEX == L"����")
		{
			if (isSJX)
			{
				pSJEX = PmPointer::CreateSJEX(RailMdbName, -1);
				if(pSJEX)
					IsInterSect = PmPointer::CalIntersectPt(pCurXLPM, pSJEX);

			}

			else
			{
				pJYEX = PmPointer::CreateGTSJEXPointer(RailMdbName);
				if(pJYEX)
					IsInterSect = PmPointer::CalIntersectPt(pCurXLPM, pJYEX);

			}
		}
		else if (JXorEX == L"����")
		{
			if (isSJX)
			{
				pSJEX = PmPointer::CreateSJEX(RailMdbName, 1);
				if(pSJEX)
					IsInterSect = PmPointer::CalIntersectPt(pCurXLPM, pSJEX);

			}
			else
			{
				pJYEX = PmPointer::CreateGTSJEXPointer(RailMdbName);
				if(pJYEX)
					IsInterSect = PmPointer::CalIntersectPt(pCurXLPM, pJYEX);

			}
		}
		else
			AfxMessageBox(L"ProName��Ӧ��·��·����������");
	}
	return IsInterSect;
}

//20191108	���ݷ�����·��������·
void PmPointer::DrawRailPro()
{
	//���ò���
	XLDataBase DBS;
	CWorkDir WorkDir;

	GTZX_ROAD* pSJJX = NULL;
	GTZX2_ROAD* pSJEX = NULL;
	GTZX_JYX_ROAD* pJYJX = NULL;
	GTZX2_JYX_ROAD* pJYEX = NULL;

	//��ȡ��·��������
	CString MdfName = WorkDir.DataPath + L"���Ƶ�·.mdf";
	CString ProName = WorkDir.ProjName;
	DBS.ModifyRoadSum = 0;
	DBS.Read_XLDbs(MdfName, L"���Ƶ�·��", ProName);
	CString RailName;
	if (DBS.ModifyRoadSum <= 0)
		return;
	RailName = DBS.pModifyRoad[0].RailName;

	//��ȡ��·���ݿ�·��
	//CString JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);
	//CString ProStr = RailName.Mid(0, RailName.GetLength() - 2);
	//CString RailMdbName = MdfName;
	//RailMdbName.Replace(L"\\���Ƶ�·.mdf", L"\\" + ProStr + ".mdb");

	CString JXorEX, ProStr, RailMdbName;
	if (RailName.ReverseFind('\\') == -1)
	{
		JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);
		ProStr = RailName.Mid(0, RailName.GetLength() - 2);
		RailMdbName = MdfName;
		RailMdbName.Replace(L"\\���Ƶ�·.mdf", L"\\" + ProStr + ".mdb");
	}
	else
	{
		BAS_DRAW_FUN BasFun;
		RailName = BasFun.GetProByMdbName(RailName);
		JXorEX = RailName.Mid(RailName.GetLength() - 2, 2);
		ProStr = RailName.Mid(0, RailName.GetLength() - 2);
		RailMdbName = DBS.pModifyRoad[0].RailName;
		RailMdbName.Replace(RailName, ProStr);
	}

	//��ȡ��·ָ��
	bool isSJX = SJXorJYX(RailMdbName, 6, false);
	if (JXorEX == L"����")
	{
		if (isSJX)
		{
			pSJJX = CreateSJJX(RailMdbName);
			if(pSJJX)
				DBS.AddEntityToDbs(pSJJX);
		}
		else
		{
			pJYJX = CreateGTSJJXPointer(RailMdbName);
			if(pJYJX)
				DBS.AddEntityToDbs(pJYJX);
		}
	}
	else if (JXorEX == L"����")
	{
		if (isSJX)
		{
			pSJEX = CreateSJEX(RailMdbName, -1);
			if(pSJEX)
				DBS.AddEntityToDbs(pSJEX);
		}

		else
		{
			pJYEX = CreateGTSJEXPointer(RailMdbName);
			if (pJYEX)
				DBS.AddEntityToDbs(pJYEX);
		}
	}
	else if (JXorEX == L"����")
	{
		if (isSJX)
		{
			pSJEX = CreateSJEX(RailMdbName, 1);
			if (pSJEX)
				DBS.AddEntityToDbs(pSJEX);
		}
		else
		{
			pJYEX = CreateGTSJEXPointer(RailMdbName);
			if (pJYEX)
				DBS.AddEntityToDbs(pJYEX);
		}
	}
	else
		AfxMessageBox(L"ProName��Ӧ��·��·����������");
}
