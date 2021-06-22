#pragma once

#include "GTZX.h"
#include "GTZX2.h"
#include "GTZX_JYX.h"
#include "GTZX2_JYX.h"
#include "BAS_DRAW_FUN_RAIL.h"

typedef GTZX_ROAD* GTZXPoint;
typedef GTZX2_ROAD* GTZX2Point;
typedef GTZX_JYX_ROAD* GTZX_JYXPoint;
typedef GTZX2_JYX_ROAD* GTZX2_JYXPoint;

class PmPointer : public BAS_DRAW_FUN_JYX
{
public:
	PmPointer(void);
	~PmPointer(void);

	//////////////////////////////////////////////////////////////////////////
	//��·����·������
	static bool CalIntersectPt(JD_CENTER* pCurXLPM, GTZX_ROAD* pInterXLPM);
	//��·����·������
	static bool CalIntersectPt(JD_CENTER* pCurXLPM, GTZX2_ROAD* pInterXLPM);

	//��·����·������
	static bool CalIntersectPt(JD_CENTER* pCurXLPM, GTZX_JYX_ROAD* pInterXLPM);
	//��·����·������
	static bool CalIntersectPt(JD_CENTER* pCurXLPM, GTZX2_JYX_ROAD* pInterXLPM);


	static bool CheckOneXLConstraint(GTZX_ROAD* pCurXLPM, GTZX_ROAD* pRelXLPM, double MinAng, double MinDetaH, 
		CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray);
	static bool CheckOneXLConstraint(GTZX_ROAD* pCurXLPM, GTZX2_ROAD* pXLPM, double MinAng, double MinDetaH, 
		CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray);
	static bool CheckOneXLConstraint(GTZX_ROAD* pCurXLPM, GTZX_JYX_ROAD* pXLPM, double MinAng, double MinDetaH, 
		CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray);
	static bool CheckOneXLConstraint(GTZX_ROAD* pCurXLPM, GTZX2_JYX_ROAD* pXLPM, double MinAng, double MinDetaH, 
		CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray);

	static bool CheckOneXLConstraint(GTZX2_ROAD* pCurXLPM, GTZX_ROAD* pRelXLPM, double MinAng, double MinDetaH, 
		CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray);
	static bool CheckOneXLConstraint(GTZX2_ROAD* pCurXLPM, GTZX2_ROAD* pXLPM, double MinAng, double MinDetaH, 
		CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray);
	static bool CheckOneXLConstraint(GTZX2_ROAD* pCurXLPM, GTZX_JYX_ROAD* pXLPM, double MinAng, double MinDetaH, 
		CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray);
	static bool CheckOneXLConstraint(GTZX2_ROAD* pCurXLPM, GTZX2_JYX_ROAD* pXLPM, double MinAng, double MinDetaH, 
		CArray<BAS_DRAW_FUN_RAIL::ConstraintErr,BAS_DRAW_FUN_RAIL::ConstraintErr>& ConstraintErrArray);
	//////////////////////////////////////////////////////////////////////////
	//�����ݿ��е����ݵ��뵽CADͼ�����ݿ���
	static GTZX_ROAD* PmPointer::AddSJJXToCADDBS(CString MdbName);	
	static GTZX2_ROAD* PmPointer::AddSJEXToCADDBS(CString MdbName, int zory);

	static GTZX_ROAD* PmPointer::AddSJJXToCADDBS1(CString MdbName,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD);
	static GTZX2_ROAD* PmPointer::AddSJEXToCADDBS1(CString MdbName, int zory,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD);

	static GTZX_JYX_ROAD* PmPointer::AddJYJXToCADDBS(CString MdbName);
	static GTZX2_JYX_ROAD* PmPointer::AddJYEXToCADDBS(CString MdbName, int zory);

	static GTZX_JYX_ROAD* PmPointer::AddJYJXToCADDBS1(CString MdbName,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD);
	static GTZX2_JYX_ROAD* PmPointer::AddJYEXToCADDBS1(CString MdbName, int zory,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD);


	//��ȫ��ָ��������������·
	static int FindJYJX(CString mdbname);
	static int FindJYEX(CString mdbname);
	static int FindSJJX(CString mdbname);
	static int FindSJEX(CString mdbname);

	
	//�����л��ߡ����߼���ȫ������
	static void AddJYJX(GTZX_JYX_ROAD* pJYJX);
	static void AddJYEX(GTZX2_JYX_ROAD* pJYEX);
	//����ƻ��ߡ����߼���ȫ������
	static void AddSJJX(GTZX_ROAD* pSJJX);
	static void AddSJEX(GTZX2_ROAD* pSJEX);

	//�õ����л���ָ��   isJYYX --�Ƿ��Ǽ��л���
	static GTZX_JYX_ROAD* CreateJYJX(CString mdb);
	static GTZX_JYX_ROAD* GetJYJX(CString mdb);

	//�õ�������(��)��ָ��
	static GTZX2_JYX_ROAD *CreateJYEX(CString mdb,int zory);
	static GTZX2_JYX_ROAD *GetJYEX(CString mdb, int zory);

	//�õ���ƻ���
	static GTZX_ROAD* CreateSJJX(CString mdb);
	static GTZX_ROAD* GetSJJX(CString mdb);
    
	//�õ���ƶ���ָ��
	static GTZX2_ROAD* CreateSJEX(CString mdb, int zory);
	static GTZX2_ROAD* GetSJEX(CString mdb, int zory);

	//�õ���ͨ��ƻ���
	static GTZX_JYX_ROAD* GetGTSJJX(CString mdb);
	static GTZX_JYX_ROAD *CreateGTSJJXPointer(CString mdbname );

	//�õ���ͨ��ƶ���
	static GTZX2_JYX_ROAD* GetGTSJEX(CString mdb);
	static GTZX2_JYX_ROAD *CreateGTSJEXPointer(CString mdbname );


	//�õ�������ƻ��ߵ�ָ��
	static GTZX2_JYX_ROAD *CreateJiXianShejiGTZX2Pointer(CString mdbname , GTZX_JYX_ROAD * & pGTZXLinp );

	//�õ�������(��)����ƻ��ߵ�ָ��
	static GTZX2_JYX_ROAD *CreateErXianShejiGTZX2Pointer(CString mdbname , int ZorY , GTZX_JYX_ROAD * & pGTZXLinp , GTZX2_JYX_ROAD *&pGTZXLinp2ToJYX);

	//��ͨ���2�� ָ��
	static GTZX2_JYX_ROAD *CreateGTSJEXPointer2(GTZX_JYX_ROAD * pGTJXpLinp,CString mdbname);

	static GTZX2_JYX_ROAD *CreateGTSJEXPointer(GTZX_JYX_ROAD *pGTJXpLinpToIn , CString mdbname);

	//����������ָ��
	static GTZX_JYX_ROAD* CreateLLXpGtzx(CString mdbname , CString LianLuoXianName );

	//�����ֲ�����ָ��
	static GTZX_JYX_ROAD* PmPointer::CreateJBJXpGtzx( CString mdbname , CString JBJXName );

	//������·��,���ݿ���,���ж���,�õ�ƽ��ָ��
	GTZX_JYX_ROAD* GetpGtzxForXLName(CString WayOfMDB,CString NameOfWay,CString NameOfRXD);

	//���洴��  isLLX --�Ƿ�������
	static GTZX_JYX_ROAD *CreateJYJX(CString mdb,CString XLName,BOOL isLLX=FALSE );
	static GTZX2_JYX_ROAD *CreateJYEX(CString mdb,CString XLName);

	GTZX_JYX_ROAD* GetRXDPMFromRXDName(CString WayOfMDB,CString NameOfWay,CString NameOfRXD);

	static GTZX_JYX_ROAD* CreateSGBXpGtzx( CString mdbname , CString SGBXName );

	static void GetXLDefaName( CString mdbName ,CString XLNameToIn , CString & XLNameToOut );

	static void  GetSlopeData(CString NameOfMDB,GTZX_JYX_ROAD *pGTZX,APDTAB &pLeftSlope,APDTAB &pRightSlope,/*CString  XLName="��ͨ��ƻ���"*/CString  XLName = "��ƻ���");
	static void  GetSlopeDataSJJX(CString NameOfMDB, GTZX_JYX_ROAD* pGTZX, APDTAB& pSlope);	//20191113����	��ȡ��ƻ����¶�����
	static int FindSlopeNum(APDTAB &pSlope,double ml);
	static void Ordel(APDTAB&Slope);
	static void DeleteRepeatData(APDTAB&Slope);
	static bool SJXorJYX(CString MDBpath ,int i,bool ErrMes);

	//20191107	���ݹ�·ƽ��ָ����㽻�����Ϣ
	static bool CalIntersectPt(JD_CENTER* pCurXLPM);

	//20191108	���ݷ�����·��������·
	static void DrawRailPro();
};
