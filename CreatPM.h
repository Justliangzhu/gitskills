#pragma once
#include "RHA.h"
#include "PMSJ.h"
#include "DocManage.h"

class CCreatPM 
{
public:
	CCreatPM(void);
	~CCreatPM(void);

	//创建线路平面自定义实体（全局或局部）
	static RHA* CreatRHA(CString mdbName, CString xlShuXing, CString xlName, bool isAddtoDbs=true);

	//创建全局的线路自定义实体，包括：既有基线、既有二线、设计基线、设计二线
	static RHA* CreatGlobalRHA(CString mdbName, CString xlName, bool isAddtoDbs=true);

	//建局部的线路平面实体指针，包括：临时线、施工便线
	static RHA* CreatPartRHA(CString mdbName, CString xlShuXing, CString xlName, bool isAddtoDbs=true);

	//根据线路属性和线路名修改线路，eg.将既有基线改为设计基线
	static RHA* ChgXLByXLAttrName(RHA *pRHA, CString xlShuXing, CString xlName);

	//直线转线路实体
	static void PLinetoRHA(CString XLNameToIn , AcDbObjectId eIdToIn , CString xlShuXing, CString mdbName=L"");

	//线路名按照既有线、设计线排序
	static void SortXLNames(CStringArray &xlNameAry);

	//全局线路数组中是否包含此线路名，如果包含返回在数组中的下标,否则返回-1
	static int IsIncluded(int iDoc, CString mdbName, CString xlName);

	//全局线路数组中是否包含此线路名，如果包含返回在窗口数组和线路数组中的下标
	static bool IsIncluded(AcApDocument *doc, CString mdbName, CString xlName, int &iDoc, int &iXL);

	//全局线路数组中是否包含此线路名，如果包含返回在窗口数组和线路数组中的下标
	static bool IsIncluded(CString mdbName, CString xlName , int &iDoc, int &iXL);

	//全局线路数组中是否包含此线路名，如果没有就生成该线路。如果包含或重新生成了该线路返回true, 反之返回false，输出ix为该线路在全局数组中的下标
	static bool IsGlobalXLHaveAndCreat( CString mdbName, CString xlName, int &ix, bool isAddtoDbs=false);
	
	//全局线路数组中是否包含此线路名,如果不包含重新生成该线路指针，返回该指针
	static RHA* IsGlobalXLHaveAndCreat(CString mdbName, CString xlName, bool isAddtoDbs=false);

	//获取当前窗口,输出窗口在g_DocManageAry中的下标
	static bool CurDocIndx(int &iDoc);

	//如果线路在全局数组中已经存在,就删除图形，给线路赋空
	static bool NullRHA(int iDoc, int iXL) ;

	//如果线路在全局数组中已经存在,就删除图形，删除线路
	static bool DelRHA(int iDoc, int iXL);

	//获取在数据库中xlName之前的所有线路名
	static void GetAllXLNames(CString mdbName, CString xlName, CStringArray &allXLNameAry);

	//比较两个线路名的优先级
	static bool CmpXLPriority(CString xlName1, CString xlName2);

	//创建所有的既有线和设计线的实体
	static void CreatAllJYXandSJX(CString mdbName, int iDoc=-1);

	//释放平面线路指针（全局）
	static void CCreatPM::FreeRHA(RHA* pRHA);

	//从当前窗口中 获取在系统的全局数组中 某数据库 的全部线路信息
	static void CCreatPM::GetSysXLInfbyMdbName(CString mdbName, CArray<XLInforTableStruct, XLInforTableStruct> &xlInfAry);

	



};
