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
	//公路与铁路基线求交
	static bool CalIntersectPt(JD_CENTER* pCurXLPM, GTZX_ROAD* pInterXLPM);
	//公路与铁路二线求交
	static bool CalIntersectPt(JD_CENTER* pCurXLPM, GTZX2_ROAD* pInterXLPM);

	//公路与铁路基线求交
	static bool CalIntersectPt(JD_CENTER* pCurXLPM, GTZX_JYX_ROAD* pInterXLPM);
	//公路与铁路二线求交
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
	//将数据库中的数据导入到CAD图形数据库中
	static GTZX_ROAD* PmPointer::AddSJJXToCADDBS(CString MdbName);	
	static GTZX2_ROAD* PmPointer::AddSJEXToCADDBS(CString MdbName, int zory);

	static GTZX_ROAD* PmPointer::AddSJJXToCADDBS1(CString MdbName,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD);
	static GTZX2_ROAD* PmPointer::AddSJEXToCADDBS1(CString MdbName, int zory,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD);

	static GTZX_JYX_ROAD* PmPointer::AddJYJXToCADDBS(CString MdbName);
	static GTZX2_JYX_ROAD* PmPointer::AddJYEXToCADDBS(CString MdbName, int zory);

	static GTZX_JYX_ROAD* PmPointer::AddJYJXToCADDBS1(CString MdbName,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD);
	static GTZX2_JYX_ROAD* PmPointer::AddJYEXToCADDBS1(CString MdbName, int zory,bool IsdrawTZD,bool IsdrawLC,bool IsdrawXLM,bool IsdrawBPD,bool IsdrawJDLX,bool IsdrawJD);


	//在全局指针数组中搜索线路
	static int FindJYJX(CString mdbname);
	static int FindJYEX(CString mdbname);
	static int FindSJJX(CString mdbname);
	static int FindSJEX(CString mdbname);

	
	//将既有基线、二线加入全局数组
	static void AddJYJX(GTZX_JYX_ROAD* pJYJX);
	static void AddJYEX(GTZX2_JYX_ROAD* pJYEX);
	//将设计基线、二线加入全局数组
	static void AddSJJX(GTZX_ROAD* pSJJX);
	static void AddSJEX(GTZX2_ROAD* pSJEX);

	//得到既有基线指针   isJYYX --是否是既有基线
	static GTZX_JYX_ROAD* CreateJYJX(CString mdb);
	static GTZX_JYX_ROAD* GetJYJX(CString mdb);

	//得到既有左(右)线指针
	static GTZX2_JYX_ROAD *CreateJYEX(CString mdb,int zory);
	static GTZX2_JYX_ROAD *GetJYEX(CString mdb, int zory);

	//得到设计基线
	static GTZX_ROAD* CreateSJJX(CString mdb);
	static GTZX_ROAD* GetSJJX(CString mdb);
    
	//得到设计二线指针
	static GTZX2_ROAD* CreateSJEX(CString mdb, int zory);
	static GTZX2_ROAD* GetSJEX(CString mdb, int zory);

	//得到贯通设计基线
	static GTZX_JYX_ROAD* GetGTSJJX(CString mdb);
	static GTZX_JYX_ROAD *CreateGTSJJXPointer(CString mdbname );

	//得到贯通设计二线
	static GTZX2_JYX_ROAD* GetGTSJEX(CString mdb);
	static GTZX2_JYX_ROAD *CreateGTSJEXPointer(CString mdbname );


	//得到基线设计基线的指针
	static GTZX2_JYX_ROAD *CreateJiXianShejiGTZX2Pointer(CString mdbname , GTZX_JYX_ROAD * & pGTZXLinp );

	//得到既有左(右)线设计基线的指针
	static GTZX2_JYX_ROAD *CreateErXianShejiGTZX2Pointer(CString mdbname , int ZorY , GTZX_JYX_ROAD * & pGTZXLinp , GTZX2_JYX_ROAD *&pGTZXLinp2ToJYX);

	//贯通设计2线 指针
	static GTZX2_JYX_ROAD *CreateGTSJEXPointer2(GTZX_JYX_ROAD * pGTJXpLinp,CString mdbname);

	static GTZX2_JYX_ROAD *CreateGTSJEXPointer(GTZX_JYX_ROAD *pGTJXpLinpToIn , CString mdbname);

	//创建联络线指针
	static GTZX_JYX_ROAD* CreateLLXpGtzx(CString mdbname , CString LianLuoXianName );

	//创建局部基线指针
	static GTZX_JYX_ROAD* PmPointer::CreateJBJXpGtzx( CString mdbname , CString JBJXName );

	//传入线路名,数据库名,绕行段名,得到平面指针
	GTZX_JYX_ROAD* GetpGtzxForXLName(CString WayOfMDB,CString NameOfWay,CString NameOfRXD);

	//纵面创建  isLLX --是否联络线
	static GTZX_JYX_ROAD *CreateJYJX(CString mdb,CString XLName,BOOL isLLX=FALSE );
	static GTZX2_JYX_ROAD *CreateJYEX(CString mdb,CString XLName);

	GTZX_JYX_ROAD* GetRXDPMFromRXDName(CString WayOfMDB,CString NameOfWay,CString NameOfRXD);

	static GTZX_JYX_ROAD* CreateSGBXpGtzx( CString mdbname , CString SGBXName );

	static void GetXLDefaName( CString mdbName ,CString XLNameToIn , CString & XLNameToOut );

	static void  GetSlopeData(CString NameOfMDB,GTZX_JYX_ROAD *pGTZX,APDTAB &pLeftSlope,APDTAB &pRightSlope,/*CString  XLName="贯通设计基线"*/CString  XLName = "设计基线");
	static void  GetSlopeDataSJJX(CString NameOfMDB, GTZX_JYX_ROAD* pGTZX, APDTAB& pSlope);	//20191113新增	获取设计基线坡度数据
	static int FindSlopeNum(APDTAB &pSlope,double ml);
	static void Ordel(APDTAB&Slope);
	static void DeleteRepeatData(APDTAB&Slope);
	static bool SJXorJYX(CString MDBpath ,int i,bool ErrMes);

	//20191107	根据公路平面指针计算交叉点信息
	static bool CalIntersectPt(JD_CENTER* pCurXLPM);

	//20191108	根据方案及路径绘制铁路
	static void DrawRailPro();
};
