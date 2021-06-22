#include "stdafx.h"
//#include "StdArx.h"
#include "RoadAutoDesign.h"
#include "RoadDesignWorkingDatabase.h"
#include "BAS_DRAW_FUN.h"
#include "calcg.h"
#include "HdmDes.h"
#include "OutTAB.h"
#include "GTZDK.h"
#include "DrawPM.h"
#include "MyLayout.h"
#include "HdmDes.h"
#include "SetDrawQXYS.h"
#include "CWorkDir.h"
#include "CProfileOpt.h"
//#include   <direct.h> 
//#include "ArcGQ.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"

#endif // VERSION_CAD2016


extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库
extern triDTM_road ptriDTM;		//数模
extern CString Cworkdir, Pro;
extern AcDbVoidPtrArray DesLineInfor;//设计线实体信息，ID
extern CArray<CZDMHT, CZDMHT>DrawZdm;
extern int NZdmT;
extern double Sfactor;
extern ACHAR SysPath[1024];

extern int isCT;//1表示只自动出图，不设计 0表示先自动设计，后出图

extern vector<AcDbObjectId> XianLuArray_ZDCTZY;//线路数组，自动出图专用

//构造函数
RoadAutoDesign::RoadAutoDesign(void)
{
	//设置最大横坡和最大超高
	m_MAXSuphI = _T("8");
	m_IG = _T("2");
	if_PL_CT = true;
	isAllinView = false;
	//设置超高计算参数
	m_HxCgJbl = _T("330");	//缓和段最小超高渐变率
	m_ZxCgJbl = _T("225");	//直线段超高渐变率
	m_WidenMode = 0;		//加宽方式
	m_SuphMode = 0;			//超高方式
	m_SupNWidLenMode = 0;	//取整方式

	//设置基本参数
	m_IsReadFromMdb = true;
	m_pRoad = NULL;

	//设置横断面桩号序列步长
	m_StationStep = 20.0;
	m_Sta_IsAddTZZ = false;		//20190819	暂时设置为不加
	m_Sta_IsAddBri = false;		//20190819	暂时设置为不加
	m_Sta_IsAddTun = false;		//20190819	暂时设置为不加

	//图框参数
	//m_Frame_Height = 554.0;
	//m_Frame_Width = 780.0;
	m_Frame_Height = 554.0;
	m_Frame_Width = 780.0;
	m_TKPos = 2;
	m_TrimFlag = 0;

	m_PageLen = 160.0;
	m_PageHeight = 32.0;

	//水沟参数
	m_GutterPath = L"";

	//页码参数
	m_IsreadTK = false;
	//图名参数
	m_DrawingNum.clear();

	//是否绘制横断面占地桩
	m_IsDrawCrossEarthPile = false;

	//改沟模式
	m_IsGutter = false;

	//是否绘制横断面网格
	m_IsDrawHengDuanMianWangGe = TRUE;
}

//析构函数
RoadAutoDesign::~RoadAutoDesign(void)
{

}


/*===================================================================
函数名:AutoDesign
------------------------------------------------------------
函数功能:改移道路自动设计主控函数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	一、平面部分
			1.计算道路宽度，存储数据库
			2.设置超高、加宽标准，存储数据库
			3.计算超高、加宽，存储数据库
			二、纵断面部分
			4.数模内插地面线，存储数据库
			5.自动拉坡设计，存储数据库
			三、横断面部分
			6.设置基本参数，存储数据库
			7.计算桩号序列
			8.提取地面线，存储数据库
			9.横断面自动设计
			四、图形输出
			10.出平面图
			11.出用地图
			12.出纵断面图
			13.出横断面图
			14.曲线要素表
			15.路基大样图
			16.工程数量表
------------------------------------------------------------
编写日期:20190805开始
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::AutoDesign()
{
	if (isCT == 0)
	{
		//获取平面指针
		m_pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
		if (!m_pRoad)
		{
			AfxMessageBox(L"无法获取平面设计数据，自动设计失败！");
			return;
		}

		GetNEraseLine();

		//获取方案名、项目名
		BAS_DRAW_FUN Bas_Fun;
		Bas_Fun.ReadWorkdir();

		//一、平面部分
		//1.计算道路宽度
		CalRoadWidth();
		//2.设置超高、加宽标准
		SetSuphStd();
		SetWidenStd();
		//3.计算超高、加宽
		SetSuphCalPara();
		CalSuphNWiden();

		//二、纵断面部分
		//4.数模内插地面线
		if (!GetEarthLine())
			return;
		//5.自动拉坡设计
		CalSlopeData();

		//三、横断面部分
		//6.设置横断面基本参数
		SetCrossParameter();
		//7.计算桩号序列
		CalStationSequence();

		g_RoadWorkingDatabase.SaveAllData(m_MdbName, m_RoadName);


		//8.提取地面线
		CalCrossEarthLine();
		//9.横断面自动设计
		CrossAutoDesgin();
	}

	
	
	//四、图形输出
	if (isCT == 1)//只出图时，删除图面上的线路  王许生
	{
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		AcDbObject *pObj = NULL;
		for (int i = 0;i < XianLuArray_ZDCTZY.size();i++)
		{
			if (acdbOpenObject(pObj,XianLuArray_ZDCTZY[i],AcDb::kForWrite) == Acad::eOk)
			{
				pObj->assertWriteEnabled();
				pObj->erase();
				pObj->close();
			}
		}
		acDocManager->unlockDocument(acDocManager->curDocument());
	}

	//设置出图方式
	if (m_OvalDrawPara.OutMode == OutputDrawingMode::Break)
		m_TrimFlag = 0;
	else
		m_TrimFlag = 1;

	//10.出平面图
	DrawHorizontalRoad();
	//11.出用地图
	DrawHEarth();
	//11-1.出平面整体图
	DrawOverallHoriRoad();

	//12.纵断面图
	DrawProfile();
	//13.出横断面图
	DrawCross();
	//14.曲线要素表
	DrawCurveInfor();
	//15.路基大样图
	DrawRoadbed();

	//16.标注页码及图名
	NotePage();
	NoteDrawingName();

	g_RoadWorkingDatabase.DeleteWorkingDatabase(m_MdbName, m_RoadName);
	acutPrintf(L"出图完成！");
}
void RoadAutoDesign::SetMdbName(CString MdbName)
{
	m_MdbName = MdbName;
}
void RoadAutoDesign::SetRoadName(CString RoadName)
{
	m_RoadName = RoadName;
}

void RoadAutoDesign::SetFramePath(CString FramePath)
{
	m_FramePath = FramePath;
}

void RoadAutoDesign::SetDrawOriCoor(double XCoor, double YCoor)
{
	m_OriX = XCoor;
	m_OriY = YCoor;
}

/*===================================================================
函数名:SetDataSource
------------------------------------------------------------
函数功能:设置数据来源
------------------------------------------------------------
输入:	bool	IsFromMdb	是否优先考虑从数据库读取数据
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190813
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetDataSource(bool IsFromMdb)
{
	m_IsReadFromMdb = IsFromMdb;
}

/*===================================================================
函数名:AutoDrawGutter
------------------------------------------------------------
函数功能:自动改沟出图
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：20190905编写注释
=====================================================================*/
void RoadAutoDesign::AutoDrawGutter()
{
	//四、图形输出
	if (isCT == 1)//只出图时，删除图面上的线路  王许生
	{
		acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
		AcDbObject* pObj = NULL;
		for (int i = 0; i < XianLuArray_ZDCTZY.size(); i++)
		{
			if (acdbOpenObject(pObj, XianLuArray_ZDCTZY[i], AcDb::kForWrite) == Acad::eOk)
			{
				pObj->assertWriteEnabled();
				pObj->erase();
				pObj->close();
			}
		}
		acDocManager->unlockDocument(acDocManager->curDocument());
	}
	m_IsGutter = true;
	//1.出平面图
	DrawHorizontalRoad();
	//2.纵断面图
	DrawProfile_Gutter();
	//3.出横断面图
	DrawCross_Gutter();
	//4.标注页码和图名
	NotePage();
	NoteDrawingName();
}

void RoadAutoDesign::SetGutterPath(CString GutterPath)
{
	m_GutterPath = GutterPath;
}

void RoadAutoDesign::SetGutterThick(double GutterThick)
{
	m_GutterThick = GutterThick;
}


void RoadAutoDesign::SetTK(bool IsReadTK)
{
	m_IsreadTK = IsReadTK;
}

//设置总体绘图参数
void RoadAutoDesign::SetOverallDrawPara(DrawOverallPara OvalDrawPara)
{
	m_OvalDrawPara = OvalDrawPara;
}


//设置平面绘图参数
void RoadAutoDesign::SetHorizontalDrawPara(DrawHorizontalPara HDrawPara)
{
	m_HDrawPara = HDrawPara;

	//20191231新增	根据比例尺调整字体大小
	double Scale = _wtof(HDrawPara.SCALE) / 1000;
	m_HDrawPara.TextHPara.EarthLineTextH *= Scale;
	m_HDrawPara.TextHPara.IntersectionInforTextH *= Scale;
	m_HDrawPara.TextHPara.MileTextH *= Scale;
	m_HDrawPara.TextHPara.PileTextH *= Scale;
}

//设置纵断面绘图参数
void RoadAutoDesign::SetProfileDrawPara(DrawProfilePara VDrawPara)
{
	m_VDrawPara = VDrawPara;
	m_VDrawPara.Hscale /= 2.0;
	m_VDrawPara.Zscale /= 2.0;
}

//设置横断面绘图参数
void RoadAutoDesign::SetCrossDrawPara(DrawCrossPara CDrawPara)
{
	m_CDrawPara = CDrawPara;
	m_CDrawPara.Scale /= 2.0;
}

void RoadAutoDesign::SetCrossEarthPile(bool IsDrawCrossEarthPile)
{
	m_IsDrawCrossEarthPile = IsDrawCrossEarthPile;
}

//设置指定桩号
void RoadAutoDesign::SetCrossAssignedPileAttri(std::vector<CrossType>& CrossTypeArray)
{
	m_CrossTypeArray = CrossTypeArray;
}

void RoadAutoDesign::GetNEraseLine()
{
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable* pBlockTable;
	pDb->getBlockTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForRead);
	pBlockTable->close();
	AcDbBlockTableRecordIterator* pIter;
	pBlockTableRecord->newIterator(pIter); // Create an iterator
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		AcDbEntity* pEntity;
		if (pIter->getEntity(pEntity, AcDb::kForWrite) != Acad::eOk)   
			continue;

		if (pEntity->isKindOf(JD_CENTER::desc()))
		{
			JD_CENTER* pJC = NULL;
			pJC = JD_CENTER::cast(pEntity);
			_wcsupr_s(pJC->mdbname, wcslen(pJC->mdbname) + 1);
			_wcsupr_s(m_pRoad->mdbname, wcslen(m_pRoad->mdbname) + 1);
			if (_tcscmp(pJC->mdbname, m_pRoad->mdbname) == 0)
				pJC->erase();
			pJC->close();
		}
		else
			pEntity->close();
	}
	pBlockTableRecord -> close();
	acDocManager->unlockDocument(acDocManager->curDocument());
}

/*===================================================================
函数名:CalRoadWidth
------------------------------------------------------------
函数功能:计算道路宽度
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从活动数据库中读取
			2.读取失败则从规范中创建
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::CalRoadWidth()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetRoadWidthArray(m_MdbName, LEFT, m_RoadWidthArray_L, m_RoadName))
	{
		;	//如果选择优先从数据库读取，且读取成功，则不计算宽度
	}
	else
	{
		CalRoadWidthByCode(LEFT, m_RoadWidthArray_L);
		g_RoadWorkingDatabase.SetRoadWidthArray(m_MdbName, m_RoadName, LEFT, m_RoadWidthArray_L);
	}

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetRoadWidthArray(m_MdbName, RIGHT, m_RoadWidthArray_L, m_RoadName))
	{
		;	//如果选择优先从数据库读取，且读取成功，则不计算宽度
	}
	else
	{
		CalRoadWidthByCode(RIGHT, m_RoadWidthArray_R);
		g_RoadWorkingDatabase.SetRoadWidthArray(m_MdbName, m_RoadName, RIGHT, m_RoadWidthArray_R);
	}
}

/*===================================================================
函数名:CalRoadWidthByCode
------------------------------------------------------------
函数功能:根据规范计算道路宽度
------------------------------------------------------------
输入:	int			LorR			左侧或右侧
------------------------------------------------------------
输出:	std::vector<LMKdata>& RoadWidthArray	道路宽数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：通过该方式产生的道路宽数组里只有一个数据
=====================================================================*/
void RoadAutoDesign::CalRoadWidthByCode(int LorR, std::vector<LMKdata>& RoadWidthArray)
{
	//清空数据
	RoadWidthArray.clear();

	//获取道路平面指针用以获取速度和等级信息
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
	{
		AfxMessageBox(L"计算加宽值时错误！");
		return;
	}
		
	//从规范中计算
	LMKdata tempRoadWitdh;
	tempRoadWitdh.xlc = pRoad->DLArray[pRoad->DLNum - 1].BLC;
	tempRoadWitdh.fgdhp = 0.0;
	tempRoadWitdh.OffsetByXLZX = 0.0;
	BAS_DRAW_FUN::SetLMKByDV(pRoad->Grade, pRoad->DV, tempRoadWitdh.fgdk, tempRoadWitdh.lydk, tempRoadWitdh.cxdk,
		tempRoadWitdh.lmhp, tempRoadWitdh.yljk, tempRoadWitdh.yljhp, tempRoadWitdh.tljk,
		tempRoadWitdh.tljhp, tempRoadWitdh.bkmode, tempRoadWitdh.sjbgmode);
	RoadWidthArray.push_back(tempRoadWitdh);
}

/*===================================================================
函数名:SetSuphStd
------------------------------------------------------------
函数功能:设置超高标准
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetSuphStd()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetRoadSuphStd(m_MdbName, m_RoadName, m_SurpArray, m_IG, m_MAXSuphI))
	{
		return;
	}
	else
	{
		CalRoadSuphStdByCode();
		g_RoadWorkingDatabase.SetRoadSuphStd(m_MdbName, m_RoadName, m_SurpArray, m_IG, m_MAXSuphI);
	}
}

/*===================================================================
函数名:CalRoadSuphStdByCode
------------------------------------------------------------
函数功能:通过规范计算超高
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190805
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::CalRoadSuphStdByCode()
{
	//获取平面指针
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
	{
		AfxMessageBox(L"计算超高标准时错误！");
		return;
	}

	//计算超高
	BAS_DRAW_FUN BasFun;
	int NSUPH = 0;
	double SuphData[10][2];
	if (pRoad->Grade != "匝道")
		BasFun.FindSuphI(pRoad->DV, _wtof(m_MAXSuphI), _wtof(m_IG), NSUPH, SuphData);
	else
		BasFun.FindZD_SuphI(pRoad->DV, NSUPH, SuphData);

	//转换
	m_SurpArray.clear();
	std::vector<double> tempVec;
	tempVec.resize(2);
	for (int i = 0; i < NSUPH; i++)
	{
		tempVec[0] = SuphData[i][0];
		tempVec[1] = SuphData[i][1];
		m_SurpArray.push_back(tempVec);
	}
}

/*===================================================================
函数名:SetWidenStd
------------------------------------------------------------
函数功能:设置加宽标准
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetWidenStd()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetRoadWidenStd(m_MdbName, m_RoadName, m_WidenArray, m_WidenType))
	{
		return;
	}
	else
	{
		CalRoadWidenStdByCode();
		g_RoadWorkingDatabase.SetRoadWidenStd(m_MdbName, m_RoadName, m_WidenArray, m_WidenType);
	}
}

/*===================================================================
函数名:CalRoadWidenStdByCode
------------------------------------------------------------
函数功能:通过规范计算加宽
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::CalRoadWidenStdByCode()
{
	//获取道路技术标准
	CString Grade = L"";
	int DesignVelocity = 0;
	if (!g_RoadWorkingDatabase.GetGradeAndVelocity(m_MdbName, m_RoadName, Grade, DesignVelocity))
		return;

	//获取数据
	int NJK = 0;
	double JKData[10][2];
	BAS_DRAW_FUN BasFun;
	if (Grade != "匝道")
	{
		if (Grade == "四级" || (Grade == "三级" && DesignVelocity < 31))
			m_WidenType = "一类加宽";
		else
			m_WidenType = "三类加宽";
		BasFun.FindJK(m_WidenType, NJK, JKData);
	}
	else
	{
		m_WidenType = "按单车道加宽";
		BasFun.FindJK(m_WidenType, NJK, JKData);
	}

	//转换
	m_WidenArray.clear();
	std::vector<double> tempVec;
	tempVec.resize(2);
	for (int i = 0; i < NJK; i++)
	{
		tempVec[0] = JKData[i][0];
		tempVec[1] = JKData[i][1];
		m_WidenArray.push_back(tempVec);
	}
}

/*===================================================================
函数名:SetSuphCalPara
------------------------------------------------------------
函数功能:获取超高标准数据
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	尝试从活动数据库中读取数据，如果数据库中没有，
			那么采取默认值（在构造函数中已赋值）
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetSuphCalPara()
{
	CString tempStr1 = L"", tempStr2 = L"";
	int Mode1 = 0, Mode2 = 0, Mode3 = 0;
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetSuphAndWidenPara(m_MdbName, m_RoadName, tempStr1, tempStr2, Mode1, Mode2, Mode3))
	{
		m_HxCgJbl = tempStr1;
		m_ZxCgJbl = tempStr2;
		m_WidenMode = Mode1;
		m_SuphMode = Mode2;
		m_SupNWidLenMode = Mode3;
	}
	else
	{
		g_RoadWorkingDatabase.SetSuphAndWidenPara(m_MdbName, m_RoadName, m_HxCgJbl, m_ZxCgJbl, m_WidenMode, m_SuphMode, m_SupNWidLenMode);
	}
}

/*===================================================================
函数名:CalSuphNWiden
------------------------------------------------------------
函数功能:计算道路的超高和加宽
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::CalSuphNWiden()
{
	if (m_IsReadFromMdb 
		&& g_RoadWorkingDatabase.GetSuphResult(m_MdbName, m_RoadName, LEFT, m_SuphResultArray_L)
		&& g_RoadWorkingDatabase.GetSuphResult(m_MdbName, m_RoadName, RIGHT, m_SuphResultArray_R)
		&& g_RoadWorkingDatabase.GetWidenResult(m_MdbName, m_RoadName, LEFT, m_WidenResultArray_L)
		&& g_RoadWorkingDatabase.GetWidenResult(m_MdbName, m_RoadName, RIGHT, m_WidenResultArray_R))
		return;

	//创建平面指针
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
	{
		AfxMessageBox(L"计算超高时创建平面指针失败！");
		return;
	}

	//创建超高加宽计算类，并传递数据
	calcg ccalcg(pRoad, _wtof(m_ZxCgJbl), _wtof(m_HxCgJbl), m_SupNWidLenMode, m_SuphMode);
	//超高标准数据
	ccalcg.NSUPH = m_SurpArray.size();
	for (int i = 0; i < m_SurpArray.size(); i++)
	{
		ccalcg.SuphData[i][0] = m_SurpArray[i][0];
		ccalcg.SuphData[i][1] = m_SurpArray[i][1];
	}
	//加宽标准数据
	ccalcg.NJK = m_WidenArray.size();
	for (int i = 0; i < m_WidenArray.size(); i++)
	{
		ccalcg.JKData[i][0] = m_WidenArray[i][0];
		ccalcg.JKData[i][1] = m_WidenArray[i][1];
	}
	//左路面宽数据
	ccalcg.NZLMK = m_RoadWidthArray_L.size();
	ccalcg.ZLMK = new LMKdata[ccalcg.NZLMK];
	for(int i=0;i<m_RoadWidthArray_L.size();i++)
		ccalcg.ZLMK[i] = m_RoadWidthArray_L[i];
	//右路面宽数据
	ccalcg.NYLMK = m_RoadWidthArray_R.size();
	ccalcg.YLMK = new LMKdata[ccalcg.NYLMK];
	for (int i = 0; i < m_RoadWidthArray_R.size(); i++)
		ccalcg.YLMK[i] = m_RoadWidthArray_R[i];

	//计算超高加宽
	ccalcg.Cal_Suph();

	//将数据存入成员变量中
	//超高数据
	m_SuphResultArray_L.clear();
	for (int i = 0; i < ccalcg.ZcgNm; i++)
	{
		ccalcg.Zcg[i].xlc = pRoad->XLC1(ccalcg.Zcg[i].cml);
		m_SuphResultArray_L.push_back(ccalcg.Zcg[i]);
	}
	m_SuphResultArray_R.clear();
	for (int i = 0; i < ccalcg.YcgNm; i++)
	{
		ccalcg.Ycg[i].xlc = pRoad->XLC1(ccalcg.Ycg[i].cml);
		m_SuphResultArray_R.push_back(ccalcg.Ycg[i]);
	}
	//加宽数据
	m_WidenResultArray_L.clear();
	for (int i = 0; i < ccalcg.ZjkNm; i++)
	{
		ccalcg.Zjk[i].xlc = pRoad->XLC1(ccalcg.Zjk[i].cml);
		m_WidenResultArray_L.push_back(ccalcg.Zjk[i]);
	}
	m_WidenResultArray_R.clear();
	for (int i = 0; i < ccalcg.YjkNm; i++)
	{
		ccalcg.Yjk[i].xlc = pRoad->XLC1(ccalcg.Yjk[i].cml);
		m_WidenResultArray_R.push_back(ccalcg.Yjk[i]);
	}

	//传入到活动数据库中
	g_RoadWorkingDatabase.SetSuphResult(m_MdbName, m_RoadName, LEFT, m_SuphResultArray_L);
	g_RoadWorkingDatabase.SetSuphResult(m_MdbName, m_RoadName, RIGHT, m_SuphResultArray_R);
	g_RoadWorkingDatabase.SetWidenResult(m_MdbName, m_RoadName, LEFT, m_WidenResultArray_L);
	g_RoadWorkingDatabase.SetWidenResult(m_MdbName, m_RoadName, RIGHT, m_WidenResultArray_R);
}

/*===================================================================
函数名:GetEarthLine
------------------------------------------------------------
函数功能:获取地面线数据
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:	bool	返回值		是否成功获取地面线	true-是	false-否
------------------------------------------------------------
算法描述:	1.尝试从数据库读取地面线数据
			2.从地面线读取数据失败后，采取数模内插地面线
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadAutoDesign::GetEarthLine()
{
	//尝试从数据库读取地面线数据
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetEarthLine(m_MdbName, m_RoadName, m_EarthLine))
		return true;

	//尝试从数模内插地面线
	if (CalEarthLine())
	{
		g_RoadWorkingDatabase.SetEarthLine(m_MdbName, m_RoadName, m_EarthLine);
		return true;
	}
		
	return false;
}

/*===================================================================
函数名:CalEarthLine
------------------------------------------------------------
函数功能:数模内插地面线
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:	bool	返回值		是否成功创建地面线	true-是	false-否
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190806
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadAutoDesign::CalEarthLine()
{
	//创建平面指针
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
		return false;

	//读取数模
	if (!ptriDTM.Read_Dtm())
	{
		AfxMessageBox(L"读取数模失败！无法完成自动设计");
		return false;
	}

	//计算步长和地面线点数量，创建临时地面线数组
	double Step = 10.0;
	int EarthNum = (int)(pRoad->LenOfLine / (Step - 1)) * 2;
	if (EarthNum < 1)
		return false;
	double* dmx = new double[EarthNum * 2];

	//在平面中生成地面线点数组
	pRoad->ZDMJG = Step;
	EarthNum = pRoad->PM_DMX(dmx, 0, pRoad->DLArray[0].ELC, pRoad->DLArray[0].ELC + pRoad->LenOfLine);	//这个函数用来生成地面线
	
	//转换参数
	m_EarthLine.clear();
	CString gh;
	double xlc;
	DMXYS tempEarth;
	for (int i = 0; i < EarthNum; i++)
	{
		tempEarth.lc = dmx[2 * i];
		tempEarth.level = dmx[2 * i + 1];

		xlc = pRoad->XLC(tempEarth.lc, gh);
		_tcscpy(tempEarth.ckml, L"\0");
		_tcscpy(tempEarth.ckml, pRoad->LCchr(gh, xlc, 2));

		m_EarthLine.push_back(tempEarth);
	}

	if (dmx)
		delete[]dmx;

	return true;
}

/*===================================================================
函数名:CalSlopeData
------------------------------------------------------------
函数功能:计算坡度数组
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.根据地面线点计算变坡点里程和限制坡度
				1.1	计算地面线三阶插商
				1.2	计算一阶均差
				1.3	获取变坡点
			2.以地面线拟合的期望来生成各变坡点设计标高
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::CalSlopeData()
{
	//首先尝试从数据库读取，如果数据库存在数据，那么
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetSlopeData(m_MdbName, m_RoadName, m_SlopeArray))
		return;

	//设置参数
	std::vector<DMXYS> SmoothEarthLine;	//平顺地面线
	std::vector<DMXYS> ThreeDiffQuotientPt, OneDiffQuotientPt;
	std::vector<DMXYS> ChangePt;	//变坡点数组
	m_BPDArray.clear();
	m_LastPDArray.clear();
	m_JPInforArray.clear();
	m_LevelPtArray.clear();
	int NBPD; //变坡点个数
	Is_NeedInitPro = true;//默认需要，当道路总长过短时直接求交上变坡点

	//获取平面指针
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
		return;

	//计算纵断面设计标准
	BAS_DRAW_FUN BasFun;
	BasFun.SetZdmMinPC(pRoad->DV, m_PrfDesData.MAXPD, m_PrfDesData.YBPCMin, m_PrfDesData.JXPCMin, m_PrfDesData.T_YBRshuMin,
		m_PrfDesData.T_JXRShu, m_PrfDesData.A_YBRshuMin, m_PrfDesData.A_JXRShu, m_PrfDesData.YBLRshuMin, m_PrfDesData.JXLRshuMin);

	g_RoadWorkingDatabase.GetLevelCtrlData(m_MdbName, m_RoadName, m_LevelPtArray);	//获取控制高程数组
	g_RoadWorkingDatabase.GetJPData(m_MdbName, m_RoadName, m_JPInforArray); //获取接坡数组

	m_UserSetGCKZDArray_SX.clear();
	m_UserSetGCKZDArray_XX.clear();
	GCKZPt tempGCKZD;
	//考虑交叉点
	for (int i = 0; i < m_LevelPtArray.size(); i++)
	{
		if (m_LevelPtArray[i].Note.Find(L"交叉") >= 0)
		{
			if (m_LevelPtArray[i].Style == 1)
			{
				tempGCKZD.Cml = m_LevelPtArray[i].Cml;
				tempGCKZD.DesHSX = m_LevelPtArray[i].DesHSX;
				m_UserSetGCKZDArray_SX.push_back(tempGCKZD);
			}
			else if (m_LevelPtArray[i].Style == 2)
			{
				tempGCKZD.Cml = m_LevelPtArray[i].Cml;
				tempGCKZD.DesHXX = m_LevelPtArray[i].DesHXX;
				m_UserSetGCKZDArray_XX.push_back(tempGCKZD);
			}
		}
	}

	//根据是否有接坡信息获取起终变坡点信息
	GetBPDStAndEdfromJP(NBPD, pRoad);

	//设置优化起终点里程、高程
	GetStAndEdLC(m_StartLC, m_EndLC, m_StartH, m_EndH,pRoad);

	//获得接坡约束范围内的地面线
	std::vector<DMXYS> LSDMX; //临时地面线表
	LSDMX.clear();
	LSDMX = m_EarthLine;
	m_EarthLine.clear();
	DMXYS tempDMXYS;
	for (int i = 0; i < LSDMX.size(); i++)
	{
		if (LSDMX[i].lc >= m_StartLC && LSDMX[i].lc <= m_EndLC)
		{
			tempDMXYS.lc = LSDMX[i].lc;
			tempDMXYS.level = LSDMX[i].level;
			m_EarthLine.push_back(tempDMXYS);
		}
	}

	//-------------------获取初始方案--------------
	if (Is_NeedInitPro)
	{
		//三阶均差计算
		GetThreeDiffQuotientPt(m_EarthLine, SmoothEarthLine, ThreeDiffQuotientPt);

		//一阶均差计算
		GetOneDiffQuotientPt(ThreeDiffQuotientPt, OneDiffQuotientPt);

		//确定地面线突变点，作为纵坡的初值
		GetChangePt(pRoad, OneDiffQuotientPt, ChangePt);

		//初始化坡度数组
		IniSlopeArrray(ChangePt);

		//调整初始方案的分段
		JudgeSection();

		//以地面线拟合的期望来生成各变坡点设计标高
		CreatPlanMiu();

		//形成初始纵断面方案
		MergeSlope();
	}
	else
		m_SlopeArray = m_BPDArray;
	//-----------------优化--------------
	for (int i = 0; i < m_SlopeArray.size(); i++)
		m_SlopeArray[i].ml = m_SlopeArray[i].cml;//优化使用ml,需转换
	//优化参数设置
	CProfileOpt* profileOpt = new CProfileOpt();
	profileOpt->m_pPlain = pRoad; //平面指针
	profileOpt->m_DMXLine = m_EarthLine; //地面线数组
	if (m_JPInforArray.size() > 0)
		profileOpt->m_JPInforArray = m_JPInforArray; //接坡信息
	profileOpt->m_SlopeArray = m_SlopeArray; //初始方案
	profileOpt->m_BPDArray = m_BPDArray; //接坡点信息用于判断起始里程
	profileOpt->StartOptimization(m_LastPDArray,m_YHGetBriArray,m_YHGetTunArray);

	//优化形成最终方案
	if (m_JPInforArray.size() > 0)
	{
		//考虑接坡约束时，优化里程从接坡点开始，接坡信息需要与最终方案合并
		if (m_JPInforArray[0].Is_JP_Bef == 1 || m_JPInforArray[0].Is_JP_Aft == 1)
		{
			//当道路满足最小坡长时,考虑接坡约束，否则直接上交点信息
			if (Is_NeedInitPro)
			{
				for (int i = 0; i < m_LastPDArray.size(); i++)
					m_BPDArray.push_back(m_LastPDArray[i]);
			}
		}
		else
		{
			//不需要接坡约束,按起终里程优化直接赋值
			m_BPDArray.clear();
			m_BPDArray = m_LastPDArray;
		}
	}
	else
	{
		//没有接坡信息，则可直接赋值
		m_BPDArray.clear();
		m_BPDArray = m_LastPDArray;
	}

	//对优化后变坡点数组按里程排序
	SortData(m_BPDArray);

	for (int i = 0; i < m_BPDArray.size(); i++)
	{
		if (i > 0 && i + 1 < m_BPDArray.size())
			m_BPDArray[i].Rshu = _wtof(m_PrfDesData.A_YBRshuMin);
		else
			m_BPDArray[i].Rshu = 0.0;
	}

	m_SlopeArray.clear();
	m_SlopeArray = m_BPDArray;

	//存入活动数据库
	if (!m_SlopeArray.size())
		return;
	//转换成现场里程（不带冠号）形式，以存储数据库
	CString str;
	for (int i = 0; i < m_SlopeArray.size(); i++)
		m_SlopeArray[i].cml = m_SlopeArray[i].ml;//优化使用ml,需转换

	for (int i = 0; i < m_SlopeArray.size(); i++)
	{
		double xlc = pRoad->XLC1(m_SlopeArray[i].cml);
		str.Format(L"%0.3lf", xlc);
		m_SlopeArray[i].ml = _wtof(str);
		if (i + 1 < m_SlopeArray.size())
			m_SlopeArray[i].degree = (m_SlopeArray[i + 1].Level - m_SlopeArray[i].Level) / (m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml);
	}

	//设置首行
	m_SlopeArray[0].Rshu = 3.0;		//设置里程小数位数
	m_SlopeArray[0].RLen = 3.0;		//设置高程小数位数
	g_RoadWorkingDatabase.SetSlopeData(m_MdbName, m_RoadName, m_SlopeArray); //保存变坡点数据

	if(m_JPInforArray.size() > 0)
		g_RoadWorkingDatabase.SetJPData(m_MdbName, m_RoadName, m_JPInforArray); //保存接坡数据
	if(m_YHGetBriArray.size()> 0)
		g_RoadWorkingDatabase.SetBriData(m_MdbName, m_RoadName, m_YHGetBriArray);//保存优化获得桥梁数据
	if(m_YHGetTunArray.size() > 0)
		g_RoadWorkingDatabase.SetTunData(m_MdbName, m_RoadName, m_YHGetTunArray);//保存优化获得隧道数据
}

/*===================================================================
函数名:GetThreeDiffQuotientPt
------------------------------------------------------------
函数功能:计算平顺地面线的三阶差商
------------------------------------------------------------
输入:	std::vector<DMXYS>&		EarthLine	地面线点
------------------------------------------------------------
输出:	std::vector<DMXYS>& SmoothedEarthLine	平顺后的点
		std::vector<DMXYS>& ThreeDiffQuotientPt	三阶差商点
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：从Automatic.cpp中移植并修改适应本项目数据结构
=====================================================================*/
void RoadAutoDesign::GetThreeDiffQuotientPt(std::vector<DMXYS>& EarthLine, std::vector<DMXYS>& SmoothedEarthLine, std::vector<DMXYS>& ThreeDiffQuotientPt)
{
	SmoothEarthline(_wtof(m_PrfDesData.YBPCMin), 5, EarthLine, SmoothedEarthLine);
	ThreeDiffQuotientPt.clear();
	DMXYS temp;
	for (int i = 0; i < SmoothedEarthLine.size(); i++)
	{
		if (i == 0)
		{
			temp.level = (SmoothedEarthLine[i + 1].level - SmoothedEarthLine[i].level) / (SmoothedEarthLine[i + 1].lc - SmoothedEarthLine[i].lc);
			temp.lc = SmoothedEarthLine[i].lc;
		}
		else if (i == SmoothedEarthLine.size() - 1)
		{
			temp.level = (SmoothedEarthLine[i].level - SmoothedEarthLine[i - 1].level) / (SmoothedEarthLine[i].lc - SmoothedEarthLine[i - 1].lc);
			temp.lc = SmoothedEarthLine[i].lc;
		}
		else
		{
			double temp1 = SmoothedEarthLine[i].level - SmoothedEarthLine[i - 1].level;
			double temp2 = SmoothedEarthLine[i + 1].lc - SmoothedEarthLine[i].lc;
			double temp3 = SmoothedEarthLine[i + 1].level - SmoothedEarthLine[i].level;
			double temp4 = SmoothedEarthLine[i].lc - SmoothedEarthLine[i - 1].lc;
			double temp5 = SmoothedEarthLine[i + 1].lc - SmoothedEarthLine[i - 1].lc;
			temp.level = (temp1 * temp2 / temp4 + temp3 * temp4 / temp2) / temp5;
			temp.lc = SmoothedEarthLine[i].lc;
		}
		ThreeDiffQuotientPt.push_back(temp);
	}
}

/*===================================================================
函数名:SmoothEarthline
------------------------------------------------------------
函数功能:平顺地面线
------------------------------------------------------------
输入:	double	MinSlopeLen		最小坡长
		int		SmoothNum		平顺次数
		std::vector<DMXYS>& EarthLine	原地面线点数组
------------------------------------------------------------
输出:	std::vector<DMXYS>& SmoothedEarthLine	平顺后的地面线点数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：从Automatic.cpp中PsDmx()移植并修改适应本项目数据结构
=====================================================================*/
void RoadAutoDesign::SmoothEarthline(double MinSlopeLen, int SmoothNum, std::vector<DMXYS>& EarthLine, std::vector<DMXYS>& SmoothedEarthLine)
{
	if (SmoothNum == 0)
	{
		SmoothedEarthLine = EarthLine;
		return;
	}

	int i, j;
	DMXYS tmpPt;
	tmpPt.lc = 0.00; tmpPt.level = 0.00;

	SmoothedEarthLine.clear();

	for (i = 0; i < EarthLine.size(); i++)    //第一次平顺
	{
		tmpPt.lc = EarthLine[i].lc;
		tmpPt.level = GetSmoothedHigh(i, MinSlopeLen, EarthLine);
		SmoothedEarthLine.push_back(tmpPt);
	}

	for (i = 1; i < SmoothNum; i++)                 //如果平顺次数大于1，继续平顺
	{
		for (j = 0; j < SmoothedEarthLine.size(); j++)
		{
			tmpPt.lc = SmoothedEarthLine[j].lc;
			//tmpPt.level = GetSmoothedHigh(j, MinSlopeLen, 1, PSdmxPt);
			tmpPt.level = GetSmoothedHigh(j, MinSlopeLen, SmoothedEarthLine);
			SmoothedEarthLine[j] = tmpPt;
		}
	}
}


/*===================================================================
函数名:GetSmoothedHigh
------------------------------------------------------------
函数功能:获取平顺后的地面线高程
------------------------------------------------------------
输入:	int i	索引号
		double MinSlopeLen	最小坡长
		std::vector<DMXYS>& EarthLine	地面线点数组
------------------------------------------------------------
输出:	double 返回值	平顺后的地面线高程
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		GetPsHigh(int i,double R,UltraCArray_T<gridPoint,gridPoint> &ZMdmxPt)
		移植并修改适应本项目数据结构
=====================================================================*/
double RoadAutoDesign::GetSmoothedHigh(int i, double MinSlopeLen, std::vector<DMXYS>& EarthLine)
{
	double totalL(0.0), totalR(0.0), dltX(0.0);
	double FLeft(0.0), FRight(0.0);
	double high;
	int j;
	//计算左侧
	j = 1;

	if (i - j > 0)
	{
		dltX = fabs(EarthLine[i - j].lc - EarthLine[i].lc);
	}

	while (i - j > 0 && dltX <= MinSlopeLen)
	{
		totalL += EarthLine[i - j].level * (MinSlopeLen - dltX) / MinSlopeLen;
		FLeft += (MinSlopeLen - dltX) / MinSlopeLen;
		j++;
		if (i - j > 0)
		{
			dltX = fabs(EarthLine[i - j].lc - EarthLine[i].lc);
		}
	}

	dltX = 0.0;
	//计算右侧
	j = 1;
	if (i + j <= EarthLine.size() - 1)
	{
		dltX = fabs(EarthLine[i + j].lc - EarthLine[i].lc);
	}

	while (i + j <= EarthLine.size() - 1 && dltX <= MinSlopeLen)
	{
		totalR += EarthLine[i + j].level * (MinSlopeLen - dltX) / MinSlopeLen;
		FRight += (MinSlopeLen - dltX) / MinSlopeLen;
		j++;
		if (i + j <= EarthLine.size() - 1)
		{
			dltX = fabs(EarthLine[i + j].lc - EarthLine[i].lc);
		}
	}

	//求得高程
	high = (totalL + totalR + EarthLine[i].level) / (FLeft + FRight + 1.0);

	return high;
}

/*===================================================================
函数名:GetSmoothedHigh
------------------------------------------------------------
函数功能:获取平顺后的地面线高程
------------------------------------------------------------
输入:	int i	索引号
		double MinSlopeLen	最小坡长
		int tp
		std::vector<DMXYS>& EarthLine	地面线点数组
------------------------------------------------------------
输出:	double 返回值	平顺后的地面线高程
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		GetPsHigh(int i,double R,UltraCArray_T<gridPoint,gridPoint> &ZMdmxPt)
		移植并修改适应本项目数据结构
=====================================================================*/
double RoadAutoDesign::GetSmoothedHigh(int i, double MinSlopeLen, int tp, std::vector<DMXYS>& EarthLine)
{
	return 0.0;
}

/*===================================================================
函数名:GetOneDiffQuotientPt
------------------------------------------------------------
函数功能:计算一阶差商
------------------------------------------------------------
输入:	std::vector<DMXYS>& ThreeDiffQuotientPt	三阶差商数组
------------------------------------------------------------
输出:	std::vector<DMXYS>& OneDiffQuotientPt	一阶插商数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		GetOneDiffQuotientPt(UltraCArray_T<gridPoint,gridPoint> &ThreeDiffQuotientPt,UltraCArray_T<gridPoint,gridPoint> &OneDiffQuotientPt);
		移植并修改适应本项目数据结构
=====================================================================*/
void RoadAutoDesign::GetOneDiffQuotientPt(std::vector<DMXYS>& ThreeDiffQuotientPt, std::vector<DMXYS>& OneDiffQuotientPt)
{
	OneDiffQuotientPt.clear();
	DMXYS temp;
	for (int i = 0; i < ThreeDiffQuotientPt.size(); i++)
	{
		if (i == 0)
		{
			temp.level = (ThreeDiffQuotientPt[i + 1].level - ThreeDiffQuotientPt[i].level) / (ThreeDiffQuotientPt[i + 1].lc - ThreeDiffQuotientPt[i].lc);
			temp.lc = ThreeDiffQuotientPt[i].lc;
		}
		else if (i == ThreeDiffQuotientPt.size() - 1)
		{
			temp.level = (ThreeDiffQuotientPt[i].level - ThreeDiffQuotientPt[i - 1].level) / (ThreeDiffQuotientPt[i].lc - ThreeDiffQuotientPt[i - 1].lc);
			temp.lc = ThreeDiffQuotientPt[i].lc;
		}
		else
		{
			double temp1 = ThreeDiffQuotientPt[i + 1].level - ThreeDiffQuotientPt[i - 1].level;
			double temp2 = ThreeDiffQuotientPt[i + 1].lc - ThreeDiffQuotientPt[i - 1].lc;
			temp.level = temp1 / temp2;
			temp.lc = ThreeDiffQuotientPt[i].lc;
		}
		OneDiffQuotientPt.push_back(temp);
	}
}

/*===================================================================
函数名:GetChangePt
------------------------------------------------------------
函数功能:获取变坡点
------------------------------------------------------------
输入:	JD_CENTER* pRoad	道路平面指针
		std::vector<DMXYS>& OneDiffQuotientPt	一阶差商点
------------------------------------------------------------
输出:	std::vector<DMXYS>& ChangePt	变坡点数组
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void GetChangePt(PlaneCAL* pGTZX,UltraCArray_T<gridPoint,gridPoint> &OneDiffQuotientPt,UltraCArray_T<gridPoint,gridPoint> &ChangePt);
		移植并修改适应本项目数据结构
=====================================================================*/
void RoadAutoDesign::GetChangePt(JD_CENTER* pRoad, std::vector<DMXYS>& OneDiffQuotientPt, std::vector<DMXYS>& ChangePt)
{
	ChangePt.clear();
	DMXYS temppt;

	/////////////////////////////////加入起点////////////////////////////
	temppt.lc = 0.0;
	temppt.level = m_StartH;
	ChangePt.push_back(temppt);

	for (int Crsi = 0; Crsi < OneDiffQuotientPt.size() - 1; Crsi++)
	{
		if (OneDiffQuotientPt[Crsi].level * 1.0e10 * OneDiffQuotientPt[Crsi + 1].level * 1.0e10 < 0.000001)
		{
			ChangePt.push_back(OneDiffQuotientPt[Crsi]);
		}
	}

	temppt.lc = pRoad->LenOfLine;
	temppt.level = m_EndH;
	ChangePt.push_back(temppt);
	////////////////////////////////加入终点///////////////////////////////
}

/*===================================================================
函数名:IniSlopeArrray
------------------------------------------------------------
函数功能:初始化坡度数组
------------------------------------------------------------
输入:	std::vector<DMXYS>& OneDiffQuotientPt	变坡点数组
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::IniSlopeArrray(std::vector<DMXYS>& ChangePt)
{
	//设置参数
	PDTAB_ROAD tempSlope;
	
	//初始化坡度数组
	m_SlopeArray.clear();
	for (int i = 0; i < ChangePt.size(); i++)
	{
		tempSlope.cml = ChangePt[i].lc;
		tempSlope.degree = 0.0;
		tempSlope.Level = 0.0;
		tempSlope.length = _wtof(m_PrfDesData.YBPCMin);
		tempSlope.RLen = 0.0;

		if (i > 0 && i + 1 < ChangePt.size())
			tempSlope.Rshu = _wtof(m_PrfDesData.A_YBRshuMin);
		else
			tempSlope.Rshu = 0.0;

		m_SlopeArray.push_back(tempSlope);
	}
}

/*===================================================================
函数名:JudgeSection
------------------------------------------------------------
函数功能:调整初始方案的分段
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void JudgeSection(PlanePlan &oneplane);
		移植并修改适应本项目数据结构
=====================================================================*/
void RoadAutoDesign::JudgeSection()
{
	int TempSlopeNum = 0;
	double TempFrontLenth = 0.0;
	double TempNowLenth = 0.0;
	double TempBackLenth = 0.0;
	double MinLongitudinalSlopeL = _wtof(m_PrfDesData.YBPCMin);

	if ((int)m_SlopeArray.size() >= 3)
	{
		for (int i = 0; i + 1 < m_SlopeArray.size(); i++)
		{
			if (i == 0)
			{
				TempNowLenth = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml;
				TempBackLenth = m_SlopeArray[i + 2].cml - m_SlopeArray[i + 1].cml;
				if (TempNowLenth < MinLongitudinalSlopeL)
				{
					double tempLenth = MinLongitudinalSlopeL - TempNowLenth;
					if (TempBackLenth - tempLenth >= MinLongitudinalSlopeL)
					{
						m_SlopeArray[i + 1].cml = m_SlopeArray[i + 1].cml + tempLenth;
					}
					else
					{
						//oneplane.m_ProfilePlan.SlopePtArray.RemoveAt(i + 1);
						m_SlopeArray.erase(m_SlopeArray.begin() + i + 1);
						i--;
					}
				}
			}
			else if (i == m_SlopeArray.size() - 2)
			{
				TempNowLenth = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml;
				TempFrontLenth = m_SlopeArray[i].cml - m_SlopeArray[i - 1].cml;

				if (TempNowLenth < MinLongitudinalSlopeL)
				{
					double tempLenth = MinLongitudinalSlopeL - TempNowLenth;
					if (TempFrontLenth - tempLenth >= MinLongitudinalSlopeL)
					{
						m_SlopeArray[i].cml = m_SlopeArray[i].cml - tempLenth;
					}
					else
					{
						m_SlopeArray.erase(m_SlopeArray.begin() + i);
						i--;
					}
				}
			}
			else
			{
				TempFrontLenth = m_SlopeArray[i].cml - m_SlopeArray[i - 1].cml;
				TempNowLenth = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml;
				TempBackLenth = m_SlopeArray[i + 2].cml - m_SlopeArray[i + 1].cml;
				double temp = m_SlopeArray[i].cml;
				if (TempNowLenth < MinLongitudinalSlopeL)
				{
					double tempLenth = MinLongitudinalSlopeL - TempNowLenth;
					double maxLenth = max(TempFrontLenth, TempBackLenth);
					if (maxLenth - tempLenth >= MinLongitudinalSlopeL)//可以调整
					{
						if (TempFrontLenth > TempBackLenth)//前一坡段比后坡段长，则当前坡段往前移动
						{
							m_SlopeArray[i].cml = m_SlopeArray[i].cml - tempLenth;
						}
						else//当前坡段往后移动
						{
							m_SlopeArray[i + 1].cml = m_SlopeArray[i + 1].cml + tempLenth;
						}
					}
					else
					{
						if (TempFrontLenth > TempBackLenth)//前一坡段比后坡段长，合并到后一坡段
						{
							m_SlopeArray.erase(m_SlopeArray.begin() + i + 1);
						}
						else
						{
							m_SlopeArray.erase(m_SlopeArray.begin() + i);
						}
						i--;
					}
				}
			}
			int num = (int)m_SlopeArray.size();
			if (num <= 2)
				return;
		}
	}

}

/*===================================================================
函数名:CreatPlanMiu
------------------------------------------------------------
函数功能:生成各变坡点标高
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool CreatPlanMiu(PlanePlan &oneplane, bool IsMinTW,bool YorN);
		移植并修改适应本项目数据结构
=====================================================================*/
bool RoadAutoDesign::CreatPlanMiu()
{
	for (int SlopePtIndex = 0; SlopePtIndex < m_SlopeArray.size(); SlopePtIndex++)
	{
		if (SlopePtIndex == 0)//起点
			m_SlopeArray[SlopePtIndex].Level = m_StartH;
		else if (SlopePtIndex == m_SlopeArray.size() - 1)//终点
			m_SlopeArray[SlopePtIndex].Level = m_EndH;
		else
		{
			bool tempD;
			tempD = CreateSloptPtDesH(SlopePtIndex);

			if (tempD == false)
				return false;
		}

	}
	return true;
}

/*===================================================================
函数名:CreateSloptPtDesH
------------------------------------------------------------
函数功能:生成第索引为SlopePtIndex的变坡点设计高程
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool CreateSloptPtDesH(PlanePlan &oneplane,int SlopePtIndex, bool IsMinTW,bool YorN );
		移植并修改适应本项目数据结构
=====================================================================*/
bool RoadAutoDesign::CreateSloptPtDesH(int SlopePtIndex)
{
	double dmxnumber;
	double minH, maxH;
	double temp;
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	FrontMileage = m_SlopeArray[SlopePtIndex - 1].cml;
	Fronthigh = m_SlopeArray[SlopePtIndex - 1].Level;
	NowMileage = m_SlopeArray[SlopePtIndex].cml;

	double MiuDesH = 0.0;

	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);

	AdjustGCKZD(m_SlopeArray[SlopePtIndex - 1].cml,
		m_SlopeArray[SlopePtIndex - 1].Level,
		pRoad->LenOfLine, m_EndH);

	Env_GetMiu(SlopePtIndex, MiuDesH);

	bool tempDR = CalSloptPtDesHRange(SlopePtIndex, minH, maxH);

	/************************************************************************/
	/* 依据坡度代数差确定当前变坡点的最小最大高程 */
	/************************************************************************/
	double miu = 0.0;//期望
	bool cheekKZD = true;//用于后面判断变坡点在高程必经点以前否
	bool cheek = true;//高程控制点在该坡段内时为false
	double cml = 0.0;
	double Gckzd = 0.0;
	double tempPD = 0.0;
	if (maxH < minH - 0.01)
		return false;
	if (maxH < minH)
	{
		double templever = minH;
		minH = maxH;
		maxH = templever;
	}
	//////////////////////////////////////////////////////////////////////////
	//double scale = pow(10.0, 3);
	double scale = 1.0;
	int MaxRoundGrade = floor((maxH - Fronthigh) / (NowMileage - FrontMileage) * scale * 100.0 + 0.001);
	int MinRoundGrade = ceil((minH - Fronthigh) / (NowMileage - FrontMileage) * scale * 100.0 - 0.001);
	if ((MaxRoundGrade - MinRoundGrade) < 2)
		tempPD = MinRoundGrade / scale / 100.0;
	else
	{
		//miu = floor((MiuDesH - Fronthigh) / (NowMileage - FrontMileage) * scale * 100.0 + 0.5);
		//double sigma = (MaxRoundGrade - MinRoundGrade) / 1.5;//方差 
		double sigma = min(maxH - MiuDesH, MiuDesH - minH) / 3.0;//方差
		double DesH = NormalRandom(MiuDesH, sigma, Fronthigh + (NowMileage - FrontMileage)* MinRoundGrade / 100, Fronthigh + (NowMileage - FrontMileage)*MaxRoundGrade / 100);
		miu = (DesH - Fronthigh) / (NowMileage - FrontMileage) * scale * 100.0;

		if (miu < MinRoundGrade)
		{
			tempPD = MinRoundGrade / scale / 100.0;
		}
		else if (miu > MaxRoundGrade)
		{
			tempPD = MaxRoundGrade / scale / 100.0;
		}
		else
		{
			//tempPD = NormalRandom(miu, sigma, MinRoundGrade, MaxRoundGrade);
			tempPD = miu;
			//tempPD = floor(tempPD + 0.5);

			tempPD = tempPD / scale / 100.0;

		}
	}
	temp = Fronthigh + (NowMileage - FrontMileage) * tempPD;

	///////////////////////////////调整坡度，计算最终设计高程///////////////////////////////////
	double TempH = temp;
	if (cheek == false)
	{
		TempH = (NowMileage - FrontMileage) * (Gckzd - Fronthigh) / (cml - FrontMileage) + Fronthigh;
		tempPD = (TempH - Fronthigh) / (NowMileage - FrontMileage) * 100.0;
		tempPD = floor(tempPD * scale + 0.5) / scale;
		TempH = Fronthigh + (NowMileage - FrontMileage) * tempPD / 100.0;
		m_SlopeArray[SlopePtIndex].Level = TempH;
	}
	else
	{
		m_SlopeArray[SlopePtIndex].Level = TempH;
	}
	if (m_SlopeArray[SlopePtIndex].Level > maxH)
	{
		m_SlopeArray[SlopePtIndex].Level = maxH;
		m_SlopeArray[SlopePtIndex].degree = (maxH - Fronthigh) / (NowMileage - FrontMileage);
	}
	if (m_SlopeArray[SlopePtIndex].Level < minH)
	{
		m_SlopeArray[SlopePtIndex].Level = minH;
		m_SlopeArray[SlopePtIndex].degree = (minH - Fronthigh) / (NowMileage - FrontMileage);
	}

	return true;
}

/*===================================================================
函数名:AdjustGCKZD
------------------------------------------------------------
函数功能:调整高程控制点
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void AdjustGCKZD(PlanePlan &oneplane, double StartCml, double StartH, double EndCml, double EndH, bool IsAdjustDMX=true);
		移植并修改适应本项目数据结构
=====================================================================*/
void RoadAutoDesign::AdjustGCKZD(double StartCml, double StartH, double EndCml, double EndH)
{
	std::vector<GCKZPt> SX_GCKZDArry, XX_GCKZDArry;	//上下限高程控制点数组
	GCKZPt TempGCKZD;
	std::vector<GCKZPt> Temp_SX_GCKZDArry, Temp_XX_GCKZDArry;//临时高程控制点数组
	double MinH, MaxH;

	//起点
	TempGCKZD.Cml = StartCml;
	TempGCKZD.DesHSX = StartH;
	TempGCKZD.DesHXX = StartH;

	TempGCKZD.Style = 1;
	SX_GCKZDArry.push_back(TempGCKZD);

	TempGCKZD.Style = 2;
	XX_GCKZDArry.push_back(TempGCKZD);

	//终点
	TempGCKZD.Cml = EndCml;
	TempGCKZD.DesHSX = EndH;
	TempGCKZD.DesHXX = EndH;

	TempGCKZD.Style = 1;
	SX_GCKZDArry.push_back(TempGCKZD);

	TempGCKZD.Style = 2;
	XX_GCKZDArry.push_back(TempGCKZD);

	//起终点形成信封区域
	GetEnvelop(SX_GCKZDArry, XX_GCKZDArry);

	//整理出用户设置的上下限控制点,先考虑下限，再考虑上限
	InsertGCKZDArray(SX_GCKZDArry, XX_GCKZDArry, m_UserSetGCKZDArray_SX, true);
	InsertGCKZDArray(SX_GCKZDArry, XX_GCKZDArry, m_UserSetGCKZDArray_XX, false);

	GetEnvelop(SX_GCKZDArry, XX_GCKZDArry);
}

/*===================================================================
函数名:GetEnvelop
------------------------------------------------------------
函数功能:调整高程控制点
------------------------------------------------------------
输入:	std::vector<GCKZPt>& SX_GCKZDArry	上行控制点数组
		std::vector<GCKZPt>& XX_GCKZDArry	下行控制点数组
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void GetEnvelop(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry);
		移植并修改适应本项目数据结构
=====================================================================*/
void RoadAutoDesign::GetEnvelop(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry)
{
	GCKZPt TempGCKZD;
	//对上限高程控制点进行排序，剔除相同里程处的控制点
	int KZDSum = SX_GCKZDArry.size();
	for (int i = 0; i < KZDSum - 1; i++)
	{
		for (int j = i + 1; j < KZDSum; j++)
		{
			if (SX_GCKZDArry[i].Cml > SX_GCKZDArry[j].Cml)
			{
				TempGCKZD = SX_GCKZDArry[i];
				SX_GCKZDArry[i] = SX_GCKZDArry[j];
				SX_GCKZDArry[j] = TempGCKZD;
			}
		}
	}
	for (int i = 0; i < SX_GCKZDArry.size() - 1; i++)
	{
		if (fabs(SX_GCKZDArry[i].Cml - SX_GCKZDArry[i + 1].Cml) < 0.01)
		{
			if (SX_GCKZDArry[i].DesHSX > SX_GCKZDArry[i + 1].DesHSX)
				SX_GCKZDArry.erase(SX_GCKZDArry.begin() + i);
			else
				SX_GCKZDArry.erase(SX_GCKZDArry.begin() + i + 1);
			i--;
		}
	}

	//对下限高程控制点进行排序，剔除相同里程处的控制点
	KZDSum = XX_GCKZDArry.size();
	for (int i = 0; i < KZDSum - 1; i++)
	{
		for (int j = i + 1; j < KZDSum; j++)
		{
			if (XX_GCKZDArry[i].Cml > XX_GCKZDArry[j].Cml)
			{
				TempGCKZD = XX_GCKZDArry[i];
				XX_GCKZDArry[i] = XX_GCKZDArry[j];
				XX_GCKZDArry[j] = TempGCKZD;
			}
		}
	}
	for (int i = 0; i < XX_GCKZDArry.size() - 1; i++)
	{
		if (fabs(XX_GCKZDArry[i].Cml - XX_GCKZDArry[i + 1].Cml) < 0.01)
		{
			if (XX_GCKZDArry[i].DesHSX < XX_GCKZDArry[i + 1].DesHSX)
				XX_GCKZDArry.erase(XX_GCKZDArry.begin() + i);
			else
				XX_GCKZDArry.erase(XX_GCKZDArry.begin() + i + 1);
			i--;
		}
	}

	//形成信封区域
	MaxEnvPtArray.clear();
	MinEnvPtArray.clear();
	EnvPt TempEnvPt;
	//循环将各个高程控制点加入，每加入一个高程控制点同时，需要将当前控制点与前一个高程控制点之间的点加入
	//两直线求交计算两控制点中间的点
	double BefLineEnd_Mileage, BefLineEnd_Lever;//前一控制点沿着最大最小坡往后走Lengh之后的里程和高程
	double AftLine_Start_Mileage, AftLine_Start_Mileage_Lever;//当前控制点沿着最大最小坡往前走Lengh之后的里程和高程
	int Res = 0;
	double MaxSlope_SX = _wtof(m_PrfDesData.MAXPD);
	double MaxSlope_XX = _wtof(m_PrfDesData.MAXPD);
	double MAX_Cml, MAX_H, MIN_Cml, MIN_H;
	//计算上限点
	//将起点加入
	TempEnvPt.Mileage = SX_GCKZDArry[0].Cml;
	TempEnvPt.Lever = SX_GCKZDArry[0].DesHSX;
	MaxEnvPtArray.push_back(TempEnvPt);
	for (int iKZD = 1; iKZD < SX_GCKZDArry.size(); iKZD++)
	{
		GetJD(SX_GCKZDArry[iKZD - 1].Cml, SX_GCKZDArry[iKZD - 1].DesHSX, SX_GCKZDArry[iKZD].Cml, SX_GCKZDArry[iKZD].DesHSX, MAX_Cml, MAX_H, MIN_Cml, MIN_H);
		TempEnvPt.Mileage = MAX_Cml;
		TempEnvPt.Lever = MAX_H;
		MaxEnvPtArray.push_back(TempEnvPt);

		TempEnvPt.Mileage = SX_GCKZDArry[iKZD].Cml;
		TempEnvPt.Lever = SX_GCKZDArry[iKZD].DesHSX;
		MaxEnvPtArray.push_back(TempEnvPt);
	}
	//将起点加入下限点数组
	TempEnvPt.Mileage = XX_GCKZDArry[0].Cml;
	TempEnvPt.Lever = XX_GCKZDArry[0].DesHXX;
	MinEnvPtArray.push_back(TempEnvPt);
	//处理下限点
	for (int iKZD = 1; iKZD < XX_GCKZDArry.size(); iKZD++)
	{
		GetJD(XX_GCKZDArry[iKZD - 1].Cml, XX_GCKZDArry[iKZD - 1].DesHXX, XX_GCKZDArry[iKZD].Cml, XX_GCKZDArry[iKZD].DesHXX, MAX_Cml, MAX_H, MIN_Cml, MIN_H);
		TempEnvPt.Mileage = MIN_Cml;
		TempEnvPt.Lever = MIN_H;
		MinEnvPtArray.push_back(TempEnvPt);

		TempEnvPt.Mileage = XX_GCKZDArry[iKZD].Cml;
		TempEnvPt.Lever = XX_GCKZDArry[iKZD].DesHXX;
		MinEnvPtArray.push_back(TempEnvPt);
	}
}

/*===================================================================
函数名:GetJD
------------------------------------------------------------
函数功能:获取交点
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void GetEnvelop(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry);
		移植并修改适应本项目数据结构
=====================================================================*/
int RoadAutoDesign::GetJD(double Scml, double SLever, double Ecml, double ELever,
	double& MAX_cml, double& MAX_H, double& MIN_cml, double& MIN_H)
{
	///////////////////////////////////////得到信封区域的交点//////////////////////////////////

	double QZminH = 0.0;//起终点确定的最小高程
	double QZmaxH = 0.0;//起终点确定的最大高程
	double x1 = Scml;
	double y1 = SLever;

	double maxx2 = Ecml;
	double maxy2 = y1 + (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	double x3 = Ecml;
	double y3 = ELever;

	double maxx4 = x1;
	double maxy4 = y3 + (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	BAS_DRAW_FUN::INTERS(&MAX_cml, &MAX_H, x1, y1, maxx2, maxy2, x3, y3, maxx4, maxy4);//计算信封区域的最高点
	if (MAX_cml < Scml)
	{
		MAX_cml = Scml;
		MAX_H = SLever;
		return -1;
	}
	if (MAX_cml > Ecml)
	{
		MAX_cml = Ecml;
		MAX_H = ELever;
		return 1;
	}


	double minx2 = Ecml;
	double miny2 = y1 - (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	double minx4 = x1;
	double miny4 = y3 - (Ecml - Scml) * _wtof(m_PrfDesData.MAXPD);

	double MinY;
	BAS_DRAW_FUN::INTERS(&MIN_cml, &MIN_H, x1, y1, minx2, miny2, x3, y3, minx4, miny4);//计算信封区域的最低点
	if (MIN_cml < Scml)
	{
		MIN_cml = Scml;
		MIN_H = SLever;
		return -1;
	}
	if (MIN_cml > Ecml)
	{
		MIN_cml = Ecml;
		MIN_H = ELever;
		return 1;
	}

	return 0;
}

/*===================================================================
函数名:InsertGCKZDArray
------------------------------------------------------------
函数功能:插入控制点数组
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		void InsertGCKZDArray(UltraCArray_T<GCKZPt,GCKZPt>& SX_GCKZDArry, UltraCArray_T<GCKZPt,GCKZPt>& XX_GCKZDArry,
		UltraCArray_T<GCKZPt,GCKZPt>& GCKZDArray, bool IsSX);
		移植并修改适应本项目数据结构
=====================================================================*/
void RoadAutoDesign::InsertGCKZDArray(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry, std::vector<GCKZPt>& GCKZDArray, bool IsSX)
{
	GCKZPt TempGCKZD;
	double MinH, MaxH;
	for (int i = 0; i < GCKZDArray.size(); i++)
	{
		TempGCKZD = GCKZDArray[i];
		GetEnvelopRange(TempGCKZD.Cml, MinH, MaxH);
		if (IsSX)
		{
			if (TempGCKZD.DesHSX<MinH - 00.1 || TempGCKZD.DesHSX>MaxH + 0.01)
				continue;
			else
			{
				InsertGCKZD(SX_GCKZDArry, TempGCKZD);
				GetEnvelop(SX_GCKZDArry, XX_GCKZDArry);
			}
		}
		else
		{
			if (TempGCKZD.DesHXX<MinH - 00.1 || TempGCKZD.DesHXX>MaxH + 0.01)
				continue;
			else
			{
				InsertGCKZD(XX_GCKZDArry, TempGCKZD);
				GetEnvelop(SX_GCKZDArry, XX_GCKZDArry);
			}
		}
	}
}

/*===================================================================
函数名:GetEnvelopRange
------------------------------------------------------------
函数功能:得到信封区域所确定的里程范围
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool GetEnvelopRange(double Mileage,double& MinH, double& MaxH);
		移植并修改适应本项目数据结构
=====================================================================*/
bool RoadAutoDesign::GetEnvelopRange(double Mileage, double& MinH, double& MaxH)
{
	MinH = 1e99;
	MaxH = -1e99;
	double a, b, c, d;
	int MaxEnvPtSum = MaxEnvPtArray.size();
	int MinEnvPtSum = MinEnvPtArray.size();
	if (MaxEnvPtSum == 0 || MinEnvPtSum == 0)
		return false;

	int i = 0;
	int j = 0;

	if (Mileage <= MaxEnvPtArray[0].Mileage || Mileage <= MinEnvPtArray[0].Mileage)
	{
		MaxH = m_StartH;	
		MinH = m_StartH;

		return false;
	}

	if (Mileage >= MaxEnvPtArray[MaxEnvPtSum - 1].Mileage + 0.001 || Mileage >= MinEnvPtArray[MinEnvPtSum - 1].Mileage + 0.001)
	{
		MaxH = m_EndH;
		MinH = m_EndH;

		return false;
	}

	for (i = 1; i < MaxEnvPtSum; i++)
	{
		if (Mileage < MaxEnvPtArray[i].Mileage)//找到当前坡段的后一个信封区域点
		{
			if (fabs(MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage) < 0.1)
			{
				MaxH = MaxEnvPtArray[i].Lever;
			}
			else
			{
				a = MaxEnvPtArray[i - 1].Mileage;
				b = MaxEnvPtArray[i - 1].Lever;
				c = MaxEnvPtArray[i].Mileage;
				d = MaxEnvPtArray[i].Lever;
				MaxH = MaxEnvPtArray[i - 1].Lever +
					(Mileage - MaxEnvPtArray[i - 1].Mileage) * (MaxEnvPtArray[i].Lever - MaxEnvPtArray[i - 1].Lever) / (MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage);
			}
			break;
		}
	}
	for (i = 1; i < MinEnvPtSum; i++)
	{
		if (Mileage < MinEnvPtArray[i].Mileage)//找到第一个大于当前里程的信封区域点
		{
			if (fabs(MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage) < 0.1)
			{
				MinH = MinEnvPtArray[i].Lever;
			}
			else
			{
				a = MinEnvPtArray[i - 1].Mileage;
				b = MinEnvPtArray[i - 1].Lever;
				c = MinEnvPtArray[i].Mileage;
				d = MinEnvPtArray[i].Lever;
				MinH = MinEnvPtArray[i - 1].Lever +
					(Mileage - MinEnvPtArray[i - 1].Mileage) * (MinEnvPtArray[i].Lever - MinEnvPtArray[i - 1].Lever) / (MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage);
			}

			break;
		}
	}
	return true;
}

/*===================================================================
函数名:GetEnvelopRange
------------------------------------------------------------
函数功能:得到信封区域所确定的里程范围
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
			bool GetEnvelopRange(double PreDesH, double PreMileage, double Mileage,double& MinH, double& MaxH);
		移植并修改适应本项目数据结构
=====================================================================*/
bool RoadAutoDesign::GetEnvelopRange(double PreDesH, double PreMileage, double Mileage, double& MinH, double& MaxH)
{
	MinH = 1e99;
	MaxH = -1e99;
	double a, b, c, d;
	int MaxEnvPtSum = MaxEnvPtArray.size();
	int MinEnvPtSum = MinEnvPtArray.size();
	if (MaxEnvPtSum == 0 || MinEnvPtSum == 0)
		return false;

	int i = 0;
	int j = 0;

	if (Mileage < MaxEnvPtArray[0].Mileage || Mileage < MinEnvPtArray[0].Mileage)
	{
		MaxH = m_StartH;
		MinH = m_StartH;
		return false;
	}

	if (Mileage > MaxEnvPtArray[MaxEnvPtSum - 1].Mileage + 0.001 || Mileage > MinEnvPtArray[MinEnvPtSum - 1].Mileage + 0.001)
	{
		MaxH = m_EndH;
		MinH = m_EndH;
		return false;
	}

	for (i = 1; i < MaxEnvPtSum; i++)
	{
		if (Mileage < MaxEnvPtArray[i].Mileage)//找到当前坡段的后一个信封区域点
		{
			//for (j = i-1; j>=0; j--)
			//{
			//	if (MaxEnvPtArray[j].Mileage < PreMileage+0.01)//找到当前坡段的前一个信封区域点
			//		break;
			//}

			//if (j>=0 && (i-j)>1)//该坡度内有信封区域点j+1到i，所有点取最小数
			//{
			//	double Slope, MinSlope = 100000.0;
			//	for (int k=j+1; k<=i; k++)
			//	{
			//		Slope = (MaxEnvPtArray[k].Lever - PreDesH) / (MaxEnvPtArray[k].Mileage-PreMileage);
			//		if (Slope < MinSlope)
			//			MinSlope = Slope;
			//	}
			//	MaxH = PreDesH + (Mileage-PreMileage) * MinSlope;
			//}

			//else
			{
				if (fabs(MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage) < 0.1)
				{
					MaxH = MaxEnvPtArray[i].Lever;
				}
				else
				{
					a = MaxEnvPtArray[i - 1].Mileage;
					b = MaxEnvPtArray[i - 1].Lever;
					c = MaxEnvPtArray[i].Mileage;
					d = MaxEnvPtArray[i].Lever;

					MaxH = MaxEnvPtArray[i - 1].Lever +
						(Mileage - MaxEnvPtArray[i - 1].Mileage) * (MaxEnvPtArray[i].Lever - MaxEnvPtArray[i - 1].Lever) / (MaxEnvPtArray[i].Mileage - MaxEnvPtArray[i - 1].Mileage);
				}
			}

			break;
		}
	}
	for (i = 1; i < MinEnvPtSum; i++)
	{
		if (Mileage < MinEnvPtArray[i].Mileage)//找到第一个大于当前里程的信封区域点
		{
			//for (j = i-1; j>=0; j--)
			//{
			//	if (MinEnvPtArray[j].Mileage < PreMileage+0.01)//找到当前坡段的前一个信封区域点
			//		break;
			//}

			//if ((j>=0) && (i-j>1))//该坡度内有信封区域点j+1到i，所有点取最小数
			//{
			//	double Slope, MaxSlope = -100000.0;
			//	for (int k=j+1; k<=i; k++)
			//	{
			//		Slope = (MinEnvPtArray[k].Lever - PreDesH) / (MinEnvPtArray[k].Mileage-PreMileage);
			//		if (Slope > MaxSlope)
			//			MaxSlope = Slope;
			//	}
			//	MinH = PreDesH + (Mileage-PreMileage) * MaxSlope;
			//}

			//else
			{
				if (fabs(MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage) < 0.1)
				{
					MinH = MinEnvPtArray[i].Lever;
				}
				else
				{
					a = MaxEnvPtArray[i - 1].Mileage;
					b = MaxEnvPtArray[i - 1].Lever;
					c = MaxEnvPtArray[i].Mileage;
					d = MaxEnvPtArray[i].Lever;
					MinH = MinEnvPtArray[i - 1].Lever +
						(Mileage - MinEnvPtArray[i - 1].Mileage) * (MinEnvPtArray[i].Lever - MinEnvPtArray[i - 1].Lever) / (MinEnvPtArray[i].Mileage - MinEnvPtArray[i - 1].Mileage);
				}
			}

			break;
		}
	}
	return true;
}


/*===================================================================
函数名:InsertGCKZD
------------------------------------------------------------
函数功能:查找高程控制点在数组中的位置
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190807
------------------------------------------------------------
备注：	1.从Automatic.cpp中
		void InsertGCKZD(UltraCArray_T<GCKZPt,GCKZPt>& GCKZDArry, GCKZPt KZD);
		移植并修改适应本项目数据结构
		2.INTERS函数暂时无效，这是直接从BAS_DRAW_FUN中找了一个函数
=====================================================================*/
void RoadAutoDesign::InsertGCKZD(std::vector<GCKZPt>& GCKZDArry, GCKZPt KZD)
{
	int Index = 0;
	GCKZPt PreKZD, NextKZD;
	double InterCml, InterH;
	double x1, y1, x2, y2, x3, y3, x4, y4;
	double Slope = _wtof(m_PrfDesData.MAXPD);
	double SlopeLen = 0.0;//坡长
	bool IsInsertStart = false, IsInsertEnd = false;
	for (int i = 0; i < GCKZDArry.size() - 1; i++)
	{
		if ((KZD.Cml > GCKZDArry[i].Cml + 0.1) && (KZD.Cml < GCKZDArry[i + 1].Cml + 0.1))//找到控制点KZD的插入位置
		{
			PreKZD = GCKZDArry[i];
			NextKZD = GCKZDArry[i + 1];
			//检查该控制点、前/后一个控制点是否有多余的
			if (KZD.Style == 1)//上限控制点
			{
				SlopeLen = (KZD.Cml - PreKZD.Cml);
				x1 = KZD.Cml;
				y1 = PreKZD.DesHSX - SlopeLen * Slope;
				x2 = PreKZD.Cml;
				y2 = KZD.DesHSX - SlopeLen * Slope;

				SlopeLen = (NextKZD.Cml - KZD.Cml);
				x3 = KZD.Cml;
				y3 = NextKZD.DesHSX - SlopeLen * Slope;
				x4 = NextKZD.Cml;
				y4 = KZD.DesHSX - SlopeLen * Slope;

				//if (0 == INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHSX, x1, y1, x2, y2, KZD.Cml, KZD.DesHSX))//有实交点，插入该控制点
				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHSX, x1, y1, x2, y2, KZD.Cml, KZD.DesHSX))//有实交点，插入该控制点
					IsInsertStart = true;
				else
					IsInsertStart = false;
				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, KZD.Cml, KZD.DesHSX, x4, y4, x3, y3, NextKZD.Cml, NextKZD.DesHSX))
					IsInsertEnd = true;
				else
					IsInsertEnd = false;

				if (IsInsertStart && IsInsertEnd)
				{
					//GCKZDArry.InsertAt(i + 1, KZD);
					GCKZDArry.insert(GCKZDArry.begin() + i + 1, KZD);
					return;
				}

				if (!IsInsertStart)//考虑前一个控制点不能插入
				{
					if (KZD.DesHSX < PreKZD.DesHSX)//新加入的小，覆盖掉前面那个，否则保持不变
						GCKZDArry[i] = KZD;
				}

				if (!IsInsertEnd)//考虑后一个控制点不能插入
				{
					if (KZD.DesHSX < NextKZD.DesHSX)//新加入的小，覆盖掉前面那个，否则保持不变
						GCKZDArry[i + 1] = KZD;
				}
			}

			if (KZD.Style == 2)//下限控制点
			{
				SlopeLen = (KZD.Cml - PreKZD.Cml);
				x1 = KZD.Cml;
				y1 = PreKZD.DesHXX + SlopeLen * Slope;
				x2 = PreKZD.Cml;
				y2 = KZD.DesHXX + SlopeLen * Slope;

				SlopeLen = (NextKZD.Cml - KZD.Cml);
				x3 = KZD.Cml;
				y3 = NextKZD.DesHXX + SlopeLen * Slope;
				x4 = NextKZD.Cml;
				y4 = KZD.DesHXX + SlopeLen * Slope;

				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, PreKZD.Cml, PreKZD.DesHXX, x1, y1, x2, y2, KZD.Cml, KZD.DesHXX))//有实交点，插入该控制点
					IsInsertStart = true;
				else
					IsInsertStart = false;
				if (0 == BAS_DRAW_FUN::INTERS(&InterCml, &InterH, KZD.Cml, KZD.DesHXX, x4, y4, x3, y3, NextKZD.Cml, NextKZD.DesHXX))
					IsInsertEnd = true;
				else
					IsInsertEnd = false;

				if (IsInsertStart && IsInsertEnd)
				{
					//GCKZDArry.InsertAt(i + 1, KZD);
					GCKZDArry.insert(GCKZDArry.begin() + i + 1, KZD);
					return;
				}

				if (!IsInsertStart)//考虑前一个控制点不能插入
				{
					if (KZD.DesHXX > PreKZD.DesHXX)//新加入的小，覆盖掉前面那个，否则保持不变
						GCKZDArry[i] = KZD;
				}

				if (!IsInsertEnd)//考虑后一个控制点不能插入
				{
					if (KZD.DesHXX > NextKZD.DesHXX)//新加入的小，覆盖掉前面那个，否则保持不变
						GCKZDArry[i + 1] = KZD;
				}
			}
		}
	}
}

/*===================================================================
函数名:CalSloptPtDesHRange
------------------------------------------------------------
函数功能:计算边坡点高程范围
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool CalSloptPtDesHRange(bool YorN,PlanePlan &oneplane,int SlopePtIndex,double& minH,double& maxH);
		移植并修改适应本项目数据结构
=====================================================================*/
bool RoadAutoDesign::CalSloptPtDesHRange(int SlopePtIndex, double& minH, double& maxH)
{
	//在这里添加代码
	double FrontfrontMileage = 0.0;//前两个变坡点的里程
	double Frontfronthigh = 0.0;//前两个变坡点的高程
	double FrontMileage = 0.0;//前一个变坡点的里程
	double Fronthigh = 0.0;//前一个变坡点的高程
	double Nowhigh = 0.0;//当前变坡点的高程
	double NowMileage = 0.0;//当前变坡点的里程
	double FrontPD = 0.0;//前一个坡度值
	double tempPD = 0.0;
	double dmxnumber = 0.0;
	double aftermaxH = 0.0;
	double afterminH = 0.0;
	double endMileage = 0.0;
	double tempmaxH = 0.0;
	double tempminH = 0.0;
	FrontMileage = m_SlopeArray[SlopePtIndex - 1].cml;
	Fronthigh = m_SlopeArray[SlopePtIndex - 1].Level;
	NowMileage = m_SlopeArray[SlopePtIndex].cml;

	double temppd = 0.0;
	//temppd = oneplane.m_ProfilePlan.SlopePtArray[SlopePtIndex].LimitGrade;
	temppd = _wtof(m_PrfDesData.MAXPD) / 100.0;
	double MaxSlopeDifference = _wtof(m_PrfDesData.MAXPD) * 2 / 100.0;	//20190808	
	if (SlopePtIndex == 1)
	{
		minH = Fronthigh - (NowMileage - FrontMileage) * temppd;//最小高程
		maxH = Fronthigh + (NowMileage - FrontMileage) * temppd;//最大高程
	}
	else
	{

		FrontfrontMileage = m_SlopeArray[SlopePtIndex - 2].cml;
		Frontfronthigh = m_SlopeArray[SlopePtIndex - 2].Level;
		FrontPD = (Fronthigh - Frontfronthigh) / (FrontMileage - FrontfrontMileage);
		tempPD = min(temppd, (FrontPD + MaxSlopeDifference));//考虑了最大坡度代数差
		maxH = Fronthigh + (NowMileage - FrontMileage) * tempPD;//最大高程(可能有问题)
		//maxH = Fronthigh+(NowMileage-FrontMileage)*temppd;

		tempPD = max(-temppd, (FrontPD - MaxSlopeDifference));//考虑了最大坡度代数差
		minH = Fronthigh + (NowMileage - FrontMileage) * tempPD;//最小高程
		//minH = Fronthigh-(NowMileage-FrontMileage)*temppd;//最小高程
	}

	////////////////////////////考虑信封区域//////////////////////////////////////////
	double TempMinH;//临时最小高程
	double TempMaxH;//临时最大高程

	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);

	GetEnvelopRange(Fronthigh, FrontMileage, NowMileage, TempMinH, TempMaxH);
	double MaxPD = _wtof(m_PrfDesData.MAXPD) / 100.0;	//20190808
	double StaMaxH = m_EndH + MaxPD * (pRoad->LenOfLine - NowMileage);
	double StaMinH = m_EndH - MaxPD * (pRoad->LenOfLine - NowMileage);
	TempMinH = max(TempMinH, StaMinH);
	TempMaxH = min(TempMaxH, StaMaxH);
	/*double maxH1 = maxH;
	double minH1 = minH;*/
	//ads_printf("$$$$$$$$$4里程%lf,信封下限%lf,信封上限%lf\n",NowMileage,TempMinH,TempMaxH);
	minH = max(minH, TempMinH);
	maxH = min(TempMaxH, maxH);
	if (maxH < minH - 0.01)//信封区域顶点处，无法同时考虑坡度代数差与信封区域，只考虑信封区域
	{
		//改为只考虑坡度代数差试一试
		maxH = TempMaxH;
		minH = TempMinH;
	}
	return true;
}

/*===================================================================
函数名:NormalRandom
------------------------------------------------------------
函数功能:传入期望、方差、取值范围，得到按正态分布的随机数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		double NormalRandom(double miu, double sigma, double min, double max);
		移植
=====================================================================*/
double RoadAutoDesign::NormalRandom(double miu, double sigma, double min, double max)
{
	srand(1);
	double x;
	double dScope;
	double y;
	do
	{
		x = AverageRandom(min, max);
		y = Normal(x, miu, sigma);
		double temp = Normal(miu, miu, sigma);
		dScope = AverageRandom(0.0, Normal(miu, miu, sigma));
	} while (dScope > y);
	return x;
}

/*===================================================================
函数名:AverageRandom
------------------------------------------------------------
函数功能:得到均匀分布随机数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		double AverageRandom(double min, double max);
		移植
=====================================================================*/
double RoadAutoDesign::AverageRandom(double min, double max)
{
	long minInteger = (long)(min * 100);
	long maxInteger = (long)(max * 100);
	if (maxInteger == minInteger)
		return (min + max) / 2.0;

	long randInteger = rand() * rand();

	long diffInteger = maxInteger - minInteger;

	long resultInteger = randInteger % diffInteger + minInteger;

	return resultInteger / 100.0;
}

/*===================================================================
函数名:Normal
------------------------------------------------------------
函数功能:计算x的概率密度函数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		double Normal(double x, double miu, double sigma);
		移植
=====================================================================*/
double RoadAutoDesign::Normal(double x, double miu, double sigma)
{
	return 1.0 / sqrt(2 * PI * sigma) * exp(-1 * (x - miu) * (x - miu) / (2 * sigma * sigma));
}

/*===================================================================
函数名:Env_GetMiu
------------------------------------------------------------
函数功能:根据地面线计算填挖方最小高程
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		void Env_GetMiu(PlanePlan &oneplane,int PDNum,double &Miu);
		移植
=====================================================================*/
void RoadAutoDesign::Env_GetMiu(int PDNum, double& Miu)
{
	double FrontMileage;//前一个变坡点的里程
	double Fronthigh;//前一个变坡点的高程
	double NowMileage;//当前变坡点的里程
	FrontMileage = m_SlopeArray[PDNum - 1].cml;
	Fronthigh = m_SlopeArray[PDNum - 1].Level;
	NowMileage = m_SlopeArray[PDNum].cml;
	GetDMX1(FrontMileage, NowMileage);
	double sum1 = 0.0;
	double sum2 = 0.0;
	for (int i = 0; i < m_CRS_PT.size(); i++)
	{
		sum1 = sum1 + (Fronthigh - m_CRS_PT[i].Earh) * (m_CRS_PT[i].Cml - FrontMileage);
		sum2 = sum2 + (m_CRS_PT[i].Cml - FrontMileage) * (m_CRS_PT[i].Cml - FrontMileage);
	}

	double K = 0.0;
	if (sum2 <= 1.0)
	{
		K = 0.0;
	}
	else
	{
		K = -sum1 / sum2;//斜率
	}
	Miu = K * (m_SlopeArray[PDNum].cml - FrontMileage) + Fronthigh;
}

/*===================================================================
函数名:GetDMX1
------------------------------------------------------------
函数功能:获取地面线
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		void GetDMX1(PlanePlan &oneplane,double StratM, double EndM);
		移植
=====================================================================*/
void RoadAutoDesign::GetDMX1(double StratM, double EndM)
{
	/////////////////////////////////////提取有效地面线/////////////////////////////
	m_CRS_PT.clear();
	int EndIndex = m_EarthLine.size() - 1;
	CRSPT crs_pt;
	for (int Index = 0; Index <= EndIndex; Index++)
	{
		if (m_EarthLine[Index].lc > StratM - 0.01 && m_EarthLine[Index].lc < EndM + 0.01)
		{
			crs_pt.Style = 0;
			crs_pt.Cml = m_EarthLine[Index].lc;
			crs_pt.Earh = m_EarthLine[Index].level;
			m_CRS_PT.push_back(crs_pt);
		}
	}
}

/*===================================================================
函数名:FormFinalSlope
------------------------------------------------------------
函数功能:形成最终纵断面方案
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool FormFinalSlope(PlanePlan& plan);
		移植
=====================================================================*/
void RoadAutoDesign::FormFinalSlope()
{
	MergeSlope();
	RoundSlope();
}

/*===================================================================
函数名:RoundSlope
------------------------------------------------------------
函数功能:坡度取整
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		void RoundSlope(ProfilePlan& profile);
		移植
=====================================================================*/
void RoadAutoDesign::RoundSlope()
{
	int SlopePtNum = m_SlopeArray.size();
	double Grade = 0.0;
	double SlopeLen = 0.0;
	double scale = pow(10.0, 3);
	for (int i = 0; i < SlopePtNum - 1; i++)
	{
		SlopeLen = m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml;
		Grade = 100.0 * (m_SlopeArray[i + 1].Level - m_SlopeArray[i].Level) / SlopeLen;
		Grade = floor(Grade * scale + 0.5) / scale / 100.0;
		m_SlopeArray[i].degree = Grade;
		m_SlopeArray[i + 1].Level = m_SlopeArray[i].Level + Grade * SlopeLen;
	}
}

/*===================================================================
函数名:MergeSlope
------------------------------------------------------------
函数功能:合并坡度
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		void MergeSlope(PlanePlan& Profile);
		移植
=====================================================================*/
void RoadAutoDesign::MergeSlope()
{
	double BefGrade = 0.0;
	double AftGrade = 0.0;
	for (int i = 1; i < m_SlopeArray.size() - 2; i++)
	{
		AftGrade = (m_SlopeArray[i + 1].Level - m_SlopeArray[i].Level)
			/ (m_SlopeArray[i + 1].cml - m_SlopeArray[i].cml);
		BefGrade = (m_SlopeArray[i].Level - m_SlopeArray[i - 1].Level)
			/ (m_SlopeArray[i].cml - m_SlopeArray[i - 1].cml);
		if (fabs(AftGrade - BefGrade) < 0.02)		//暂时按0.02（2%）坡度处理
		{
			if (SlopePtCanDel(i, i))
			{
				m_SlopeArray.erase(m_SlopeArray.begin() + i);
				i--;
			}
		}
	}
}

/*===================================================================
函数名:SlopePtCanDel
------------------------------------------------------------
函数功能:判断Profile方案的SlopetIndex变坡点能否被删除
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190808
------------------------------------------------------------
备注：	从Automatic.cpp中
		bool SlopePtCanDel(PlanePlan& Profile, int StartIndex, int EndIndex);
		移植
=====================================================================*/
bool RoadAutoDesign::SlopePtCanDel(int StartIndex, int EndIndex)
{
	int SlopePtSum = m_SlopeArray.size();
	if (StartIndex < 1 || EndIndex > SlopePtSum - 2)
	{
		return false;
	}

	double MAXSlope_SX, MAXSlope_XX;
	double Slope, BefSlope, AftSlope;

	MAXSlope_SX = _wtof(m_PrfDesData.MAXPD) / 100.0;
	MAXSlope_XX = _wtof(m_PrfDesData.MAXPD) / 100.0;

	Slope = (m_SlopeArray[EndIndex + 1].Level - m_SlopeArray[StartIndex - 1].Level) /
		(m_SlopeArray[EndIndex + 1].cml - m_SlopeArray[StartIndex - 1].cml);
	if (Slope > 0.0)
	{
		if (Slope > MAXSlope_SX + 0.0000001)
			return false;
	}
	else
	{
		if (-Slope > MAXSlope_XX + 0.0000001)
			return false;
	}

	if (StartIndex > 1)
	{
		BefSlope = (m_SlopeArray[StartIndex - 1].Level - m_SlopeArray[StartIndex - 2].Level) /
			(m_SlopeArray[StartIndex - 1].cml - m_SlopeArray[StartIndex - 2].cml);

		if (fabs(Slope - BefSlope) > _wtof(m_PrfDesData.MAXPD) / 100.0 * 2.0 + 0.0000001)	//最大坡度代数差
			return false;
	}

	if (EndIndex < SlopePtSum - 2)
	{
		AftSlope = (m_SlopeArray[EndIndex + 2].Level - m_SlopeArray[EndIndex + 1].Level) /
			(m_SlopeArray[EndIndex + 2].cml - m_SlopeArray[EndIndex + 1].cml);

		if (fabs(AftSlope - Slope) > _wtof(m_PrfDesData.MAXPD) / 100.0 * 2.0 + 0.0000001)	//最大坡度代数差
			return false;
	}

	return true;
}

/*===================================================================
函数名:SetCrossParameter
------------------------------------------------------------
函数功能:自动设置横断面基本参数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	逐个设置每个表的基本参数
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetCrossParameter()
{
	//设置松方压实系数
	SetYSYXPara();
	//设置填方分类
	SetFillTypePara();
	//设置用地面积汇总分段表
	SetEarthSectionPara();
	//设置用地类型表
	SetEarthTypePara();
	//设置挖方分类
	SetCutTypePara();
	//设置关联断面路肩拉直
	SetGLDMLZPara();
	//设置关联断面路堑边坡推平
	SetGLDMLQBPTPPara();
	//设置关联断面线间沟
	SetGLDMXJGPara();
	//设置清表土厚度
	SetHTHDPara(LEFT);
	SetHTHDPara(RIGHT);
	//设置沟底坡
	SetGouPDPara(LEFT);
	SetGouPDPara(RIGHT);
	//设置超填宽度表
	SetCTKDPara(LEFT);
	SetCTKDPara(RIGHT);
	//设置弧形边坡表
	SetARCBPPara(LEFT);
	SetARCBPPara(RIGHT);
	//设置挡墙
	SetDQPara(LEFT);
	SetDQPara(RIGHT);
	//设置低填路堤自动填平
	SetLDTPPara(LEFT);
	SetLDTPPara(RIGHT);
	//设置低挖路堑自动推平
	SetLQTPPara(LEFT);
	SetLQTPPara(RIGHT);
	//设置低填路堤自动加宽土路肩
	SetTLJJKPara(LEFT);
	SetTLJJKPara(RIGHT);
	//设置路面厚度
	SetLMHDPara(LEFT);
	SetLMHDPara(RIGHT);
	//设置地面线内插宽
	SetCRSWIDPara(LEFT);
	SetCRSWIDPara(RIGHT);
	//设置横断面限制宽度
	SetDMPara(LEFT);
	SetDMPara(RIGHT);
	//设置用地加宽
	SetPWDPara(LEFT);
	SetPWDPara(RIGHT);
	//设置路堤边坡
	SetLDBPPara(LEFT);
	SetLDBPPara(RIGHT);
	//设置路堑边坡
	SetLQBPPara(LEFT);
	SetLQBPPara(RIGHT);
	//设置排水沟
	SetLDSGPara(LEFT);
	SetLDSGPara(RIGHT);
	//设置侧沟
	SetLQSGPara(LEFT);
	SetLQSGPara(RIGHT);
	//设置天沟
	SetTGPara(LEFT);
	SetTGPara(RIGHT);
}

/*===================================================================
函数名:SetYSYXPara
------------------------------------------------------------
函数功能:设置松方压实系数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetYSYXPara()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetSFYSXS(m_MdbName, m_RoadName, m_YSXS))
		return;

	m_YSXS[0] = 1.23;
	m_YSXS[1] = 1.16;
	m_YSXS[2] = 1.09;
	m_YSXS[3] = 0.92;
	m_YSXS[4] = 0.92;
	m_YSXS[5] = 0.92;

	g_RoadWorkingDatabase.SetSFYSXS(m_MdbName, m_RoadName, m_YSXS);
}

/*===================================================================
函数名:SetFillTypePara
------------------------------------------------------------
函数功能:设置填方分类
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetFillTypePara()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetFillType(m_MdbName, m_RoadName, m_FillType))
		return;

	m_FillType.clear();
	LDTLdata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.type = 0;
	m_FillType.push_back(DefultPara);

	g_RoadWorkingDatabase.SetFillType(m_MdbName, m_RoadName, m_FillType);
}

/*===================================================================
函数名:SetEarthSectionPara
------------------------------------------------------------
函数功能:设置用地面积汇总分段表
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetEarthSectionPara()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetYongDiFDB(m_MdbName, m_RoadName, m_EarthSection))
		return;

	m_EarthSection.clear();
	YongDiFDB DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.BZ = L"";
	m_EarthSection.push_back(DefultPara);

	g_RoadWorkingDatabase.SetYongDiFDB(m_MdbName, m_RoadName, m_EarthSection);
}

/*===================================================================
函数名:SetEarthTypePara
------------------------------------------------------------
函数功能:设置用地类型表
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetEarthTypePara()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetEarthType(m_MdbName, m_RoadName, m_EarthType))
		return;

	m_EarthType.clear();
	YongDiLXB DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.YDLX = L"水稻田";
	m_EarthType.push_back(DefultPara);

	g_RoadWorkingDatabase.SetEarthType(m_MdbName, m_RoadName, m_EarthType);
}

/*===================================================================
函数名:SetCutTypePara
------------------------------------------------------------
函数功能:设置挖方分类
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetCutTypePara()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetCutType(m_MdbName, m_RoadName, m_CutType))
		return;

	m_CutType.clear();
	WFFLdata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	for (int i = 0; i < 6; i++)
	{
		DefultPara.sj[i] = 0.0;
	}
	DefultPara.sj[2] = 100.0;
	m_CutType.push_back(DefultPara);

	g_RoadWorkingDatabase.SetCutType(m_MdbName, m_RoadName, m_CutType);
}

/*===================================================================
函数名:SetGLDMLZPara
------------------------------------------------------------
函数功能:设置关联断面路肩拉直
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetGLDMLZPara()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetGLDMLZ(m_MdbName, m_RoadName, m_GLDMLZ))
		return;

	m_GLDMLZ.clear();
	GLDMLZdata DefultPara;
	DefultPara.Sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.Edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.dX = 5.0;
	DefultPara.dH = 1.0;
	m_GLDMLZ.push_back(DefultPara);

	g_RoadWorkingDatabase.SetGLDMLZ(m_MdbName, m_RoadName, m_GLDMLZ);
}

/*===================================================================
函数名:SetGLDMLQBPTPPara
------------------------------------------------------------
函数功能:设置关联断面路堑边坡推平
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetGLDMLQBPTPPara()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetGLDMLQBPTP(m_MdbName, m_RoadName, m_GLDMLQBPTP))
		return;

	m_GLDMLQBPTP.clear();
	GLDMLQBPTPdata DefultPara;
	DefultPara.Sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.Edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.dX = 5.0;
	m_GLDMLQBPTP.push_back(DefultPara);

	g_RoadWorkingDatabase.SetGLDMLQBPTP(m_MdbName, m_RoadName, m_GLDMLQBPTP);
}

/*===================================================================
函数名:SetGLDMXJGPara
------------------------------------------------------------
函数功能:设置关联断面线间沟
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetGLDMXJGPara()
{
	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetGLDMXJG(m_MdbName, m_RoadName, m_GLDMXJG))
		return;

	m_GLDMXJG.clear();
	GLDMXJGdata DefultPara;
	DefultPara.Sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.Edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.GouWide = 1.0;
	DefultPara.GouHigh = 0.5;
	DefultPara.NBP = 0.0;
	DefultPara.WBP = 0.0;
	DefultPara.JQHD = 0.2;
	DefultPara.GDR = 0.0;
	m_GLDMXJG.push_back(DefultPara);

	g_RoadWorkingDatabase.SetGLDMXJG(m_MdbName, m_RoadName, m_GLDMXJG);
}

/*===================================================================
函数名:SetHTHDPara
------------------------------------------------------------
函数功能:设置清表土厚度
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetHTHDPara(int LorR)
{
	std::vector<hthddata>& HTHD = (LorR == LEFT) ? m_HTHD_L : m_HTHD_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetHTHD(m_MdbName, m_RoadName, LorR, HTHD))
		return;

	HTHD.clear();
	hthddata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.hthd = 0.0;
	DefultPara.xz = 0;

	HTHD.push_back(DefultPara);

	g_RoadWorkingDatabase.SetHTHD(m_MdbName, m_RoadName, LorR, HTHD);
}

/*===================================================================
函数名:SetGouPDPara
------------------------------------------------------------
函数功能:设置沟底坡
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetGouPDPara(int LorR)
{
	return;

	std::vector<GouPDdata>& GouPD = (LorR == LEFT) ? m_GouPD_L : m_GouPD_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetGouPD(m_MdbName, m_RoadName, LorR, GouPD))
		return;

	GouPD.clear();
	GouPDdata DefultPara;
	DefultPara.Sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.Edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.SH = 0.0;
	DefultPara.EH = 0.0;

	GouPD.push_back(DefultPara);

	g_RoadWorkingDatabase.SetGouPD(m_MdbName, m_RoadName, LorR, GouPD);
}

/*===================================================================
函数名:SetCTKDPara
------------------------------------------------------------
函数功能:设置超填宽度表
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetCTKDPara(int LorR)
{
	return;

	std::vector<ctkddata>& CTKD = (LorR == LEFT) ? m_CTKD_L : m_CTKD_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetCTKD(m_MdbName, m_RoadName, LorR, CTKD))
		return;

	CTKD.clear();
	ctkddata DefultPara;
	DefultPara.Sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.Edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.Sctkd = 0.0;
	DefultPara.Ectkd = 0.0;

	CTKD.push_back(DefultPara);

	g_RoadWorkingDatabase.SetCTKD(m_MdbName, m_RoadName, LorR, CTKD);
}

/*===================================================================
函数名:SetARCBPPara
------------------------------------------------------------
函数功能:设置弧形边坡表
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetARCBPPara(int LorR)
{
	return;

	std::vector<arcbpata>& ARCBP = (LorR == LEFT) ? m_ARCBP_L : m_ARCBP_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetARCBP(m_MdbName, m_RoadName, LorR, ARCBP))
		return;

	ARCBP.clear();
	arcbpata DefultPara;
	DefultPara.Sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.Edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.SR = 0.0;
	DefultPara.ER = 0.0;
	DefultPara.ArcLocation = 0.0;

	ARCBP.push_back(DefultPara);

	g_RoadWorkingDatabase.SetARCBP(m_MdbName, m_RoadName, LorR, ARCBP);
}

/*===================================================================
函数名:SetDQPara
------------------------------------------------------------
函数功能:设置挡墙
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetDQPara(int LorR)
{
	return;

	std::vector<dqdata>& DQ = (LorR == LEFT) ? m_DQ_L : m_DQ_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetDQ(m_MdbName, m_RoadName, LorR, DQ))
		return;

	DQ.clear();
	dqdata DefultPara;
	DefultPara.sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.szj = 100.0;
	DefultPara.ezj = 100.0;

	DQ.push_back(DefultPara);

	g_RoadWorkingDatabase.SetDQ(m_MdbName, m_RoadName, LorR, DQ);
}

/*===================================================================
函数名:SetLDTPPara
------------------------------------------------------------
函数功能:设置低填路堤自动填平
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetLDTPPara(int LorR)
{
	return;

	std::vector<LDTPData>& LDTP = (LorR == LEFT) ? m_LDTP_L : m_LDTP_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetLDTP(m_MdbName, m_RoadName, LorR, LDTP))
		return;

	LDTP.clear();
	LDTPData DefultPara;
	DefultPara.sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况

	LDTP.push_back(DefultPara);

	g_RoadWorkingDatabase.SetLDTP(m_MdbName, m_RoadName, LorR, LDTP);
}

/*===================================================================
函数名:SetLQTPPara
------------------------------------------------------------
函数功能:设置低挖路堑自动推平
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetLQTPPara(int LorR)
{
	return;

	std::vector<LQTPData>& LQTP = (LorR == LEFT) ? m_LQTP_L : m_LQTP_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetLQTP(m_MdbName, m_RoadName, LorR, LQTP))
		return;

	LQTP.clear();
	LQTPData DefultPara;
	DefultPara.sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况

	LQTP.push_back(DefultPara);

	g_RoadWorkingDatabase.SetLQTP(m_MdbName, m_RoadName, LorR, LQTP);
}

/*===================================================================
函数名:SetTLJJKPara
------------------------------------------------------------
函数功能:设置低填路堤自动加宽土路肩
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetTLJJKPara(int LorR)
{
	return;

	std::vector<TLJJKData>& TLJJK = (LorR == LEFT) ? m_TLJJK_L : m_TLJJK_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetTLJJK(m_MdbName, m_RoadName, LorR, TLJJK))
		return;

	TLJJK.clear();
	TLJJKData DefultPara;
	DefultPara.sdml = m_pRoad->DLArray[0].BLC;
	DefultPara.edml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况

	TLJJK.push_back(DefultPara);

	g_RoadWorkingDatabase.SetTLJJK(m_MdbName, m_RoadName, LorR, TLJJK);
}

/*===================================================================
函数名:SetLMHDPara
------------------------------------------------------------
函数功能:设置路面厚度
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetLMHDPara(int LorR)
{
	std::vector<LMHdata>& LMHD = (LorR == LEFT) ? m_LMHD_L : m_LMHD_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetLMHD(m_MdbName, m_RoadName, LorR, LMHD))
		return;

	LMHD.clear();
	LMHdata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.cxdh = 0.5;
	DefultPara.yljh = 0.25;
	DefultPara.tljh = 0.1;

	LMHD.push_back(DefultPara);

	g_RoadWorkingDatabase.SetLMHD(m_MdbName, m_RoadName, LorR, LMHD);
}

/*===================================================================
函数名:SetCRSWIDPara
------------------------------------------------------------
函数功能:设置地面线内插宽
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetCRSWIDPara(int LorR)
{
	std::vector<crswiddata>& CRSWID = (LorR == LEFT) ? m_CRSWID_L : m_CRSWID_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetCRSWID(m_MdbName, m_RoadName, LorR, CRSWID))
		return;

	CRSWID.clear();
	crswiddata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.wid = 100.0;

	CRSWID.push_back(DefultPara);

	g_RoadWorkingDatabase.SetCRSWID(m_MdbName, m_RoadName, LorR, CRSWID);
}

/*===================================================================
函数名:SetDMPara
------------------------------------------------------------
函数功能:设置横断面限制宽度
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetDMPara(int LorR)
{
	std::vector<dmdata>& DM = (LorR == LEFT) ? m_DM_L : m_DM_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetDM(m_MdbName, m_RoadName, LorR, DM))
		return;

	DM.clear();
	dmdata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.xk = 100.0;

	DM.push_back(DefultPara);

	g_RoadWorkingDatabase.SetDM(m_MdbName, m_RoadName, LorR, DM);
}

/*===================================================================
函数名:SetPWDPara
------------------------------------------------------------
函数功能:设置用地加宽
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetPWDPara(int LorR)
{
	std::vector<pwddata>& PWD = (LorR == LEFT) ? m_PWD_L : m_PWD_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetPWD(m_MdbName, m_RoadName, LorR, PWD))
		return;

	PWD.clear();
	pwddata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.ltjk = 1.0;
	DefultPara.lqjk = 1.0;

	PWD.push_back(DefultPara);

	g_RoadWorkingDatabase.SetPWD(m_MdbName, m_RoadName, LorR, PWD);
}

/*===================================================================
函数名:SetLDBPPara
------------------------------------------------------------
函数功能:设置路堤边坡
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetLDBPPara(int LorR)
{
	std::vector<LDBPdata>& LDBP = (LorR == LEFT) ? m_LDBP_L : m_LDBP_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetLDBP(m_MdbName, m_RoadName, LorR, LDBP))
		return;

	LDBP.clear();
	LDBPdata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况

	DefultPara.BPJS = 3;
	DefultPara.bpdata[0] = 1.5;
	DefultPara.bpdata[1] = 8;
	DefultPara.bpdata[2] = 1.0;
	DefultPara.bpdata[3] = 0.0;

	DefultPara.bpdata[4] = 1.75;
	DefultPara.bpdata[5] = 10;
	DefultPara.bpdata[6] = 1.0;
	DefultPara.bpdata[7] = 0.0;

	DefultPara.bpdata[8] = 2.0;
	DefultPara.bpdata[9] = 100;
	DefultPara.bpdata[10] = 1.0;
	DefultPara.bpdata[11] = 0.0;

	LDBP.push_back(DefultPara);

	g_RoadWorkingDatabase.SetLDBP(m_MdbName, m_RoadName, LorR, LDBP);
}

/*===================================================================
函数名:SetLQBPPara
------------------------------------------------------------
函数功能:设置路堑边坡
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetLQBPPara(int LorR)
{
	std::vector<LDBPdata>& LQBP = (LorR == LEFT) ? m_LQBP_L : m_LQBP_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetLQBP(m_MdbName, m_RoadName, LorR, LQBP))
		return;

	LQBP.clear();
	LDBPdata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况

	DefultPara.BPJS = 3;
	DefultPara.bpdata[0] = 1.5;
	DefultPara.bpdata[1] = 8;
	DefultPara.bpdata[2] = 1.0;
	DefultPara.bpdata[3] = 0.0;

	DefultPara.bpdata[4] = 1.25;
	DefultPara.bpdata[5] = 10;
	DefultPara.bpdata[6] = 1.0;
	DefultPara.bpdata[7] = 0.0;

	DefultPara.bpdata[8] = 1.5;
	DefultPara.bpdata[9] = 100;
	DefultPara.bpdata[10] = 1.0;
	DefultPara.bpdata[11] = 0.0;

	LQBP.push_back(DefultPara);

	g_RoadWorkingDatabase.SetLQBP(m_MdbName, m_RoadName, LorR, LQBP);
}

/*===================================================================
函数名:SetLDSGPara
------------------------------------------------------------
函数功能:设置排水沟
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetLDSGPara(int LorR)
{
	return;

	std::vector<LDSGdata>& LDSG = (LorR == LEFT) ? m_LDSG_L : m_LDSG_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetLDSG(m_MdbName, m_RoadName, LorR, LDSG))
		return;

	LDSG.clear();
	LDSGdata DefultPara;
	/*
	if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
	ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//动态声请一个空间
	ZLDSG[0].sdml = Sxlc;
	ZLDSG[0].edml = Exlc;

	ZLDSG[0].WallorGou  = 1;//默认的是水沟
	ZLDSG[0].S_HPDK=ZLDSG[0].E_HPDK=2;//护坡道宽
	ZLDSG[0].S_HPDH=ZLDSG[0].E_HPDH=0.5;//护坡道宽
	ZLDSG[0].S_DSYK=ZLDSG[0].E_DSYK=1;//护坡道宽
	ZLDSG[0].S_DSYH=ZLDSG[0].E_DSYH=0.5;//护坡道宽
	ZLDSG[0].S_GK=ZLDSG[0].E_GK=0.4;//横坡
	ZLDSG[0].S_GS=ZLDSG[0].E_GS=0.6;//横坡
	ZLDSG[0].nbp  = 1;//内侧边坡
	ZLDSG[0].wbp  = 1;//外侧边坡
	ZLDSG[0].DSYP  = 1;//外侧边坡
	ZLDSG[0].jh   = 0.2;//浆砌厚度
	*/

	LDSG.push_back(DefultPara);

	g_RoadWorkingDatabase.SetLDSG(m_MdbName, m_RoadName, LorR, LDSG);
}

/*===================================================================
函数名:SetLQSGPara
------------------------------------------------------------
函数功能:设置侧沟
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetLQSGPara(int LorR)
{
	std::vector<LQSGdata>& LQSG = (LorR == LEFT) ? m_LQSG_L : m_LQSG_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetLQSG(m_MdbName, m_RoadName, LorR, LQSG))
		return;

	LQSG.clear();
	LQSGdata DefultPara;
	DefultPara.dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].ELC;	//这里不考虑断链为空的情况
	DefultPara.gw = 0.4;
	DefultPara.gs = 0.6;
	//DefultPara.nbp = 1;
	//DefultPara.wbp = 1;
	DefultPara.nbp = 0.0;
	DefultPara.wbp = 0.0;
	DefultPara.Jh = 0.3;////浆砌厚度
	DefultPara.GDR = 0.0;

	LQSG.push_back(DefultPara);

	g_RoadWorkingDatabase.SetLQSG(m_MdbName, m_RoadName, LorR, LQSG);
}

/*===================================================================
函数名:SetTGPara
------------------------------------------------------------
函数功能:设置天沟
------------------------------------------------------------
输入:	int		LorR		左侧或右侧	-1左侧	1右侧
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.尝试从数据库读取数据
			2.设置默认值
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：	HdmDataBase里对于该数据处理是没有从数据库中读取到则不默认赋值
		从程序中设定横断面基本参数存储后该表内容也为空
		故在此暂时不读取该数据
=====================================================================*/
void RoadAutoDesign::SetTGPara(int LorR)
{
	return;

	std::vector<TGdata>& TG = (LorR == LEFT) ? m_TG_L : m_TG_R;

	if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetTG(m_MdbName, m_RoadName, LorR, TG))
		return;

	TG.clear();
	TGdata DefultPara;
	/*
	if(ZTG) {delete []ZTG;ZTG=NULL;}
	ZTG=(TGdata* )new TGdata[NZTG];
	ZTG[0].sdml = Sxlc;
	ZTG[0].edml = Exlc;

	ZTG[0].sbj=2;//起始边距（坡顶离水沟内侧距离）
	ZTG[0].sgw=0.4;//沟宽
	ZTG[0].sgs=0.6;//沟深
	ZTG[0].ebj=2;//终止边距
	ZTG[0].egw=0.4;//沟宽
	ZTG[0].egs=0.6;//沟深
	ZTG[0].nbp=1;//内侧边坡
	ZTG[0].wbp=1;//外侧边坡
	ZTG[0].jqhd=0.3;//浆切厚度
	*/

	TG.push_back(DefultPara);

	g_RoadWorkingDatabase.SetTG(m_MdbName, m_RoadName, LorR, TG);
}

/*===================================================================
函数名:CalStationSequence
------------------------------------------------------------
函数功能:计算桩号序列
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	逐个设置每个表的基本参数
------------------------------------------------------------
编写日期:20190810
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::CalStationSequence()
{
	//尝试从数据库读取数据	20191009	这里先暂时关闭
	//if (m_IsReadFromMdb && g_RoadWorkingDatabase.GetSationSequence(m_MdbName, m_RoadName, m_StationSequence))
	//	return;

	//自动计算
	//设置桥隧数据
	g_RoadWorkingDatabase.GetBriData(m_MdbName, m_RoadName, m_BriArray);
	g_RoadWorkingDatabase.GetTunData(m_MdbName, m_RoadName, m_TunArray);

	m_pRoad->FormLCB(m_StationStep, m_pRoad->DLArray[0].ELC, m_pRoad->LenOfLine);

	if (m_Sta_IsAddTZZ)
		m_pRoad->AddTZDtoLCB();

	if (m_Sta_IsAddBri)
	{
		int NBRI = m_BriArray.size();
		double BriAry[200][3];

		for (int j = 0; j < m_BriArray.size(); j++)
		{
			BriAry[j][0] = m_pRoad->TYLC(m_BriArray[j].StDml);
			BriAry[j][1] = m_pRoad->TYLC(m_BriArray[j].EdDml);
			if (m_BriArray[j].XZ == "左幅")
				BriAry[j][2] = 1.0;
			else if (m_BriArray[j].XZ == "右幅")
				BriAry[j][2] = 2.0;
			else
				BriAry[j][2] = 0.0;
		}

		m_pRoad->AddBritoLCB(NBRI, BriAry);
	}

	if (m_Sta_IsAddTun)
	{
		int NTUN = m_TunArray.size();
		double TunAry[200][3];

		for (int j = 0; j < m_TunArray.size(); j++)
		{
			TunAry[j][0] = m_pRoad->TYLC(m_TunArray[j].StDml);
			TunAry[j][1] = m_pRoad->TYLC(m_TunArray[j].EdDml);
			if (m_TunArray[j].XZ == "左幅")
				TunAry[j][2] = 1.0;
			else if (m_TunArray[j].XZ == "右幅")
				TunAry[j][2] = 2.0;
			else
				TunAry[j][2] = 0.0;
		}

		m_pRoad->AddTuntoLCB(NTUN, TunAry);
	}

	//获取数据
	m_StationSequence.clear();
	for (int i = 0; i < m_pRoad->pLCB.GetSize(); i++)
		m_StationSequence.push_back(m_pRoad->pLCB[i]);
	g_RoadWorkingDatabase.SetSationSequence(m_MdbName, m_RoadName, m_StationSequence);
}

/*===================================================================
函数名:CalCrossEarthLine
------------------------------------------------------------
函数功能:计算横断面地面线
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：从HdmDmx::CalDmx()中移植过来，去除了对话框部分，默认选择数模内插
=====================================================================*/
void RoadAutoDesign::CalCrossEarthLine()
{
	if (!ptriDTM.DTMHasRead)
	{
		if (!ptriDTM.Read_Dtm())
			return;
	}

	EdgeNode* CrsDmxHead, * Tmp;
	int LPnum, RPnum, j;
	double LLd, RRd;
	double xl[200], yl[200], zl[200];
	double xr[200], yr[200], zr[200];

	ACHAR pszLabel[256];
	_stprintf(pszLabel, L"%s横断面地面线内插", m_RoadName);
	acedSetStatusBarProgressMeter(pszLabel, 0, 100);

	m_CrossEarthLine.clear();
	m_CrossEarthLine.resize(m_StationSequence.size());
	for (j = 0; j < m_StationSequence.size(); j++)
	{
		CString GH;
		m_CrossEarthLine[j].lc = m_pRoad->XLC(m_StationSequence[j].lc, GH);

		double cml;
		int k;
		cml = m_StationSequence[j].lc;//统一里程

		//计算左右地面点
		LLd = CalCrossRoadWidth(m_CrossEarthLine[j].lc, m_CRSWID_L);
		RRd = CalCrossRoadWidth(m_CrossEarthLine[j].lc, m_CRSWID_R);

		BAS_DRAW_FUN::xlpoint PZ;
		PZ.lc = cml;
		m_pRoad->xlpoint_pz(&PZ);
		double pt1[3], pt2[3], cita, x, y, yyt, Yd;
		Yd = 0.0;//初始值
		pt1[0] = PZ.x; pt1[1] = PZ.y; pt1[2] = 0.0;
		cita = PZ.a;
		x = PZ.x;
		y = PZ.y;
		yyt = cita - 0.5 * pi;
		pt2[0] = x + LLd * cos(yyt);
		pt2[1] = y + LLd * sin(yyt);
		pt2[2] = 0.0;

		if (ptriDTM.DTMHasRead)
			CrsDmxHead = ptriDTM.FindInterPt(pt1, pt2, LPnum);
		else
			LPnum = 0;

		if (LPnum > 0)//左侧地面线有点
		{
			k = 0;
			while (CrsDmxHead)
			{
				xl[k] = CrsDmxHead->pt[0];
				yl[k] = CrsDmxHead->pt[1];
				zl[k] = CrsDmxHead->pt[2];

				Tmp = CrsDmxHead;
				CrsDmxHead = CrsDmxHead->next;
				free(Tmp);
				k++;
			}

			if (k > 0)
			{
				Yd = zl[0];
				if (Yd < 0.1 && k>1)
				{
					Yd = zl[1];
				}
			}

			for (k = 1; k < LPnum; k++)
			{
				xl[k] = sqrt((xl[k] - xl[0]) * (xl[k] - xl[0]) + (yl[k] - yl[0]) * (yl[k] - yl[0]));

				if (zl[k] < -100.0)//没数模 按中桩
					zl[k] = Yd;
				yl[k] = zl[k] - Yd;

			}
			xl[0] = 0.0;
			yl[0] = 0.0;
		}//至少有一个点
		else//一个点也没有
		{
			LPnum = 2;
			Yd = 0.0;
			xl[0] = 0.0;
			yl[0] = 0.0;

			xl[1] = LLd;
			yl[1] = 0.0;
		}

		yyt = cita + 0.5 * pi;
		pt2[0] = x + RRd * cos(yyt);
		pt2[1] = y + RRd * sin(yyt);
		pt2[2] = 0.0;
		CrsDmxHead = ptriDTM.FindInterPt(pt1, pt2, RPnum);

		//计算横断面右侧地面点坐标
		if (RPnum > 0)
		{
			k = 0;
			while (CrsDmxHead)
			{
				xr[k] = CrsDmxHead->pt[0];
				yr[k] = CrsDmxHead->pt[1];
				zr[k] = CrsDmxHead->pt[2];
				Tmp = CrsDmxHead;
				CrsDmxHead = CrsDmxHead->next;
				free(Tmp);
				k++;
			}
			for (k = 1; k < RPnum; k++)
			{
				xr[k] = sqrt((xr[k] - xr[0]) * (xr[k] - xr[0]) + (yr[k] - yr[0]) * (yr[k] - yr[0]));

				if (zr[k] < -100.0)//没数模 按中桩
					zr[k] = Yd;
				yr[k] = zr[k] - Yd;
			}
			xr[0] = 0.0;
			yr[0] = 0.0;
		}
		else
		{
			RPnum = 2;
			Yd = 0.0;
			xr[0] = 0.0;
			yr[0] = 0.0;

			xr[1] = RRd;
			yr[1] = 0.0;
		}

		//地面线赋值
		AcGePoint3d pt;
		m_CrossEarthLine[j].EarH = Yd;
		m_CrossEarthLine[j].ZEarPtNum = LPnum;
		m_CrossEarthLine[j].ZEarPtAry.clear();
		for (k = 0; k < LPnum; k++)
		{
			pt.x = xl[k], pt.y = yl[k], pt.z = 0.0;
			m_CrossEarthLine[j].ZEarPtAry.push_back(pt);
		}
		m_CrossEarthLine[j].YEarPtNum = RPnum;
		m_CrossEarthLine[j].YEarPtAry.clear();
		for (k = 0; k < RPnum; k++)
		{
			pt.x = xr[k], pt.y = yr[k], pt.z = 0.0;
			m_CrossEarthLine[j].YEarPtAry.push_back(pt);
		}
		int Pos = (int)(j * 100.0 / (m_StationSequence.size() + 1));
		acedSetStatusBarProgressMeterPos(Pos);
	}

	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();

	if (ptriDTM.DTMHasRead)
		ptriDTM.ReleaseDtm();

	WriteCrossEarthFile();
	//写hdkzdk文件		
	CString pathstr1 = Cworkdir + L"\\data\\" + m_RoadName;
	if (!PathIsDirectory(pathstr1))
		::_wmkdir(pathstr1);
	WriteZdkHdkFile_AutoDesign();

}

/*===================================================================
函数名:CalCrossRoadWidth
------------------------------------------------------------
函数功能:计算地面线内插宽
------------------------------------------------------------
输入:	double dml		现场里程（无冠号）
		std::vector<crswiddata> CRSWID	地面线内插宽分组
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190811
------------------------------------------------------------
备注：
=====================================================================*/
double RoadAutoDesign::CalCrossRoadWidth(double dml, std::vector<crswiddata> CRSWID)
{
	int i;
	double cml, ml;

	cml = m_pRoad->TYLC(dml);
	for (i = 0; i < CRSWID.size(); i++)
	{
		ml = m_pRoad->TYLC(CRSWID[i].dml);//分段终里程
		if (cml < ml || fabs(ml - cml) < 0.001)
			//所给里程处于第I个分段内
			return CRSWID[i].wid;
	}

	return CRSWID[i - 1].wid;
}

/*===================================================================
函数名:WriteCrossEarthFile
------------------------------------------------------------
函数功能:写入横断面地面线文件
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：	从void HdmDmx::WriteHdmDmxFile(int i)移植
=====================================================================*/
void RoadAutoDesign::WriteCrossEarthFile()
{
	FILE* fpw;
	CString DmxFname;

	DmxFname = Cworkdir + "\\data\\" + "hdmdmx." + Pro + "." + m_RoadName;
	fpw = _wfopen(DmxFname, L"w");
	if (fpw)
	{
		fwprintf(fpw, L"%d\n", m_StationSequence.size());
		int j;
		for (j = 0; j < m_StationSequence.size(); j++)
		{
			fwprintf(fpw, L"%0.3lf %0.3lf %d %d\n", m_CrossEarthLine[j].lc, m_CrossEarthLine[j].EarH, m_CrossEarthLine[j].ZEarPtNum, m_CrossEarthLine[j].YEarPtNum);
			int k;
			for (k = 0; k < m_CrossEarthLine[j].ZEarPtAry.size(); k++)
				fwprintf(fpw, L"%0.3lf %0.3lf ", m_CrossEarthLine[j].ZEarPtAry[k].x, m_CrossEarthLine[j].ZEarPtAry[k].y);
			fwprintf(fpw, L"\n");
			for (k = 0; k < m_CrossEarthLine[j].YEarPtAry.size(); k++)
				fwprintf(fpw, L"%0.3lf %0.3lf ", m_CrossEarthLine[j].YEarPtAry[k].x, m_CrossEarthLine[j].YEarPtAry[k].y);
			fwprintf(fpw, L"\n");
		}
		fclose(fpw);
		char mes[256];
		ads_printf(L"%s横断面地面线已保存在%s中!\n", m_RoadName, DmxFname);
	}
}

/*===================================================================
函数名:WriteZdkHdkFile_AutoDesign
------------------------------------------------------------
函数功能:写入占地宽文件
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::WriteZdkHdkFile_AutoDesign()
{
	FILE* fpw, * fpwhdk;
	CString GH, fname, fnamehdk, mes;
	double dml;
	int iKM1, iKM2, crsnum, itmp, k;

	if (m_StationSequence.size() > 0)
	{
		dml = m_CrossEarthLine[0].lc;
		iKM1 = (int)((dml + 0.001) / 1000.0);


		fname.Format(L"%s\\data\\%s\\ZDK%d.TXT", Cworkdir, m_RoadName, iKM1);
		if ((fpw = _wfopen(fname, L"w")) == NULL)
		{
			mes.Format(L"无法打开%s文件!", fname);
			AfxMessageBox(mes);
			return;
		}
		fnamehdk.Format(L"%s\\data\\%s\\HDK%d.TXT", Cworkdir, m_RoadName, iKM1);
		if ((fpwhdk = _wfopen(fnamehdk, L"w")) == NULL)
		{
			mes.Format(L"无法打开%s文件!", fnamehdk);
			AfxMessageBox(mes);
			return;
		}

		crsnum = 0;
		itmp = 1000;

		fwprintf(fpw, L"\"%s%d\" %3d\n", GH, iKM1, itmp);
		fwprintf(fpwhdk, L"\"%s%d\" %3d\n", GH, iKM1, itmp);

		for (int j = 0; j < m_StationSequence.size(); j++)
		{
			//  dml = road[i].pm->XLC(road[i].hdmdmx[j].lc,GH);
			dml = m_CrossEarthLine[j].lc;

			iKM2 = (int)((dml + 0.001) / 1000.0);
			if (iKM2 - iKM1 < 0.5)
			{
				fwprintf(fpw, L"%12.3lf %12.3lf\n", dml, m_CrossEarthLine[j].EarH);
				fwprintf(fpwhdk, L"%0.3lf 0 %d\n", dml, m_CrossEarthLine[j].ZEarPtNum + m_CrossEarthLine[j].YEarPtNum - 1);
				for (k = m_CrossEarthLine[j].ZEarPtNum - 1; k > 0; k--)
					fwprintf(fpwhdk, L"%6.3lf %6.3lf ", -m_CrossEarthLine[j].ZEarPtAry[k].x, m_CrossEarthLine[j].ZEarPtAry[k].y);

				fwprintf(fpwhdk, L"\n");
				fwprintf(fpwhdk, L"0 0 ");
				for (k = 1; k < m_CrossEarthLine[j].YEarPtNum; k++)
					fwprintf(fpwhdk, L"%6.3lf %6.3lf ", m_CrossEarthLine[j].YEarPtAry[k].x, m_CrossEarthLine[j].YEarPtAry[k].y);

				fwprintf(fpwhdk, L"\n");
				crsnum++;
			}
			else
			{
				fwprintf(fpw, L"%12.3lf %12.3lf\n", dml, m_CrossEarthLine[j].EarH);

				fwprintf(fpwhdk, L"%0.3lf 0 %d\n", dml, m_CrossEarthLine[j].ZEarPtNum + m_CrossEarthLine[j].YEarPtNum - 1);
				for (k = m_CrossEarthLine[j].ZEarPtNum - 1; k > 0; k--)
					fwprintf(fpwhdk, L"%6.3lf %6.3lf ", -m_CrossEarthLine[j].ZEarPtAry[k].x, m_CrossEarthLine[j].ZEarPtAry[k].y);
				fwprintf(fpwhdk, L"\n");
				fwprintf(fpwhdk, L"0 0 ");
				for (k = 1; k < m_CrossEarthLine[j].YEarPtNum; k++)
					fwprintf(fpwhdk, L"%6.3lf %6.3lf ", m_CrossEarthLine[j].YEarPtAry[k].x, m_CrossEarthLine[j].YEarPtAry[k].y);
				fwprintf(fpwhdk, L"\n");

				crsnum++;
				rewind(fpw);
				fwprintf(fpw, L"\"%s%d\" %3d\n", GH, iKM1, crsnum);
				rewind(fpwhdk);
				fwprintf(fpwhdk, L"\"%s%d\" %3d\n", GH, iKM1, crsnum);
				fclose(fpw);
				fclose(fpwhdk);
				crsnum = 0;
				iKM1 = iKM2;
				fname.Format(L"%s\\data\\%s\\ZDK%d.TXT", Cworkdir, m_RoadName, iKM1);
				if ((fpw = _wfopen(fname, L"w")) == NULL)
				{
					mes.Format(L"无法打开%s文件!", fname);
					AfxMessageBox(mes);
					return;
				}
				fwprintf(fpw, L"\"%s%d\" %3d\n", GH, iKM1, itmp);
				fwprintf(fpw, L"%12.3lf %12.3lf\n", dml, m_CrossEarthLine[j].EarH);

				fnamehdk.Format(L"%s\\data\\%s\\HDK%d.TXT", Cworkdir, m_RoadName, iKM1);
				if ((fpwhdk = _wfopen(fnamehdk, L"w")) == NULL)
				{
					mes.Format(L"无法打开%s文件!", fnamehdk);
					AfxMessageBox(mes);
					return;
				}
				fwprintf(fpwhdk, L"\"%s%d\" %3d\n", GH, iKM1, itmp);
				fwprintf(fpwhdk, L"%0.3lf 0 %d\n", dml, m_CrossEarthLine[j].ZEarPtNum + m_CrossEarthLine[j].YEarPtNum - 1);
				for (k = m_CrossEarthLine[j].ZEarPtNum - 1; k > 0; k--)
					fwprintf(fpwhdk, L"%6.3lf %6.3lf ", -m_CrossEarthLine[j].ZEarPtAry[k].x, m_CrossEarthLine[j].ZEarPtAry[k].y);
				fwprintf(fpwhdk, L"\n");
				fwprintf(fpwhdk, L"0 0 ");
				for (k = 1; k < m_CrossEarthLine[j].YEarPtNum; k++)
					fwprintf(fpwhdk, L"%6.3lf %6.3lf ", m_CrossEarthLine[j].YEarPtAry[k].x, m_CrossEarthLine[j].YEarPtAry[k].y);
				fwprintf(fpwhdk, L"\n");
				crsnum++;
			}
		}
	}
	rewind(fpw);
	fwprintf(fpw, L"\"%s%d\" %3d\n", GH, iKM1, crsnum);
	fclose(fpw);

	rewind(fpwhdk);
	fwprintf(fpwhdk, L"\"%s%d\" %3d\n", GH, iKM1, crsnum);
	fclose(fpwhdk);
}

/*===================================================================
函数名:CrossAutoDesgin
------------------------------------------------------------
函数功能:横断面自动设计
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::CrossAutoDesgin()
{
	HdmDes HDM;
	DesLineInfor.removeAll();
	HDM.DesignHdm_AutoDesign();
	RefreshTSFAndYDArea();
}

/*===================================================================
函数名:RefreshTSFAndYDArea
------------------------------------------------------------
函数功能:刷新土石方和用地数据
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::RefreshTSFAndYDArea()
{
	OutTAB outTab;
	CString MdbName = Cworkdir + "\\DATA\\" + Pro + ".mdb";
	XLDataBase DBS;
	double FillVolSum, CutVolSum;

	outTab.CalAllTSF(MdbName, L"主线", m_pRoad->DLArray[0].ELC, m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].BLC, FillVolSum, CutVolSum);

	DBS.YDLXSum = 0;
	DBS.Read_XLDbs(MdbName, L"用地类型表", L"主线");
	if (DBS.YDLXSum == 0)
	{
		DBS.YDLXSum = 1;
		DBS.YDLXB = new YongDiLXB[1];
		DBS.YDLXB[0].dml = m_pRoad->DLArray[m_pRoad->DLArray.GetSize() - 1].BLC;
		DBS.YDLXB[0].YDLX = "耕地";
	}

	double szydk, syydk, ezydk, eyydk, sml, eml;
	int i, sno = -1, eno = -1, iroad, ii;
	double zxsml = m_pRoad->DLArray[0].ELC;
	double zxeml = m_pRoad->XLLength;

	CString ZDKfilename = Cworkdir + "\\data\\" + "hdmZDK." + Pro + "." + "主线";
	FILE* fpr = NULL;
	int NZDK;
	ZDKDAT* pZDK = NULL;
	double dml;
	if ((fpr = _wfopen(ZDKfilename, L"r")) != NULL)
	{
		fwscanf(fpr, L"%d", &NZDK);
		if (NZDK > 0)
		{
			pZDK = new ZDKDAT[NZDK];
			for (int j = 0; j < NZDK; j++)
				fwscanf(fpr, L"%lf%lf%d%lf%lf", &pZDK[j].cml, &dml, &pZDK[j].IsInsertHdm, &pZDK[j].LZDK, &pZDK[j].RZDK);
		}
		else
		{
			fclose(fpr);
			return;
		}
		fclose(fpr);
	}
	else
	{
		AfxMessageBox(L"无法打开占地宽文件!请先进行横断面或用地设计!");
		return;
	}

	CString LandAreaStr;//用地面积
	double YdAll = 0.0;
	zxsml = m_pRoad->DLArray[0].ELC;
	for (int iYD = 0; iYD < DBS.YDLXSum; iYD++)
	{
		zxeml = m_pRoad->TYLC(DBS.YDLXB[iYD].dml);

		//if(zxsml< pZDK[0].cml-0.01||zxeml> pZDK[ NZDK-1].cml+0.01)
		//{
		//	ads_alert(L"里程出界!");
		//	return;
		//}
		if (zxsml < pZDK[0].cml - 0.01)
		{
			ads_printf(L"统计用地的起点里程小于用地桩起点里程\n");
			zxsml = pZDK[0].cml;
		}
		if (zxeml > pZDK[NZDK - 1].cml + 0.01)
		{
			ads_printf(L"统计用地的终点里程大于用地桩终点里程\n");
			zxeml = pZDK[NZDK - 1].cml;
		}
		double SmlZx, EmlZx;
		SmlZx = zxsml, EmlZx = zxeml;

		double zarea = 0.0, yarea = 0.0;

		sml = zxsml;
		eml = zxeml;
		sno = -1, eno = -1;
		for (i = 0; i < NZDK; i++)
		{
			if (pZDK[i].cml >= sml - 0.01)
			{
				sno = i;
				if (i < NZDK && i>0)
				{
					szydk = pZDK[i - 1].LZDK + (sml - pZDK[i - 1].cml) *
						(pZDK[i].LZDK - pZDK[i - 1].LZDK) /
						(pZDK[i].cml - pZDK[i - 1].cml);
					syydk = pZDK[i - 1].RZDK + (sml - pZDK[i - 1].cml) *
						(pZDK[i].RZDK - pZDK[i - 1].RZDK) /
						(pZDK[i].cml - pZDK[i - 1].cml);
				}
				else if (i == 0)
				{
					szydk = pZDK[0].LZDK;
					syydk = pZDK[0].RZDK;
				}
				break;
			}
		}

		for (i = 0; i < NZDK; i++)
		{
			if (pZDK[i].cml >= eml - 0.01)
			{
				eno = i;
				if (i < NZDK && i>0)
				{
					ezydk = pZDK[i - 1].LZDK + (eml - pZDK[i - 1].cml) *
						(pZDK[i].LZDK - pZDK[i - 1].LZDK) /
						(pZDK[i].cml - pZDK[i - 1].cml);
					eyydk = pZDK[i - 1].RZDK + (eml - pZDK[i - 1].cml) *
						(pZDK[i].RZDK - pZDK[i - 1].RZDK) /
						(pZDK[i].cml - pZDK[i - 1].cml);
				}
				break;
			}
		}

		if (sno >= 0 && eno >= 0)
		{
			for (i = sno; i < eno - 1; i++)
			{
				zarea += 0.5 * (pZDK[i].LZDK + pZDK[i + 1].LZDK) *
					(pZDK[i + 1].cml - pZDK[i].cml);
				yarea += 0.5 * (pZDK[i].RZDK + pZDK[i + 1].RZDK) *
					(pZDK[i + 1].cml - pZDK[i].cml);
			}
			if (eno > sno)
			{
				zarea += 0.5 * (szydk + pZDK[sno].LZDK) * (pZDK[sno].cml - sml);
				zarea += 0.5 * (ezydk + pZDK[eno - 1].LZDK) * (eml - pZDK[eno - 1].cml);
				yarea += 0.5 * (syydk + pZDK[sno].RZDK) * (pZDK[sno].cml - sml);
				yarea += 0.5 * (eyydk + pZDK[eno - 1].RZDK) * (eml - pZDK[eno - 1].cml);
			}
			else
			{
				zarea += 0.5 * (szydk + ezydk) * (eml - sml);
				yarea += 0.5 * (syydk + eyydk) * (eml - sml);
			}

		}

		if (zarea + yarea > 1.0)
		{
			CString Temp;
			Temp.Format(L"%.1lf", (zarea + yarea) * 3.0 / 2000.0);
			LandAreaStr += (Temp + "(" + DBS.YDLXB[iYD].YDLX + ")");
		}

		YdAll += (zarea + yarea) * 3.0 / 2000.0;

		zxsml = zxeml;

	}

#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
	//////////////////////////////////////////////////////////////////////////
	//将线路属性写入改移道路表
	CString MdfRoadNmae = Pro;
	CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
	DBS.Read_XLDbs(MdfName, L"改移道路表", MdfRoadNmae);
	if (DBS.ModifyRoadSum < 1)
	{
		DBS.ModifyRoadSum = 1;
		DBS.pModifyRoad = new BAS_DRAW_FUN::ModifyRoadTab[DBS.ModifyRoadSum];
		DBS.IniMdfRoadData(MdfRoadNmae, DBS.pModifyRoad[0]);
	}
	else if (DBS.ModifyRoadSum > 1)
	{
		AfxMessageBox(Pro + "在改移道路表中有多条记录!");
		return;
	}

	DBS.pModifyRoad[0].FillVol = FillVolSum;
	DBS.pModifyRoad[0].CutVol = CutVolSum;
	DBS.pModifyRoad[0].LandArea = YdAll;
	DBS.pModifyRoad[0].LandAreaStr = LandAreaStr;
	DBS.Write_XLDbs(MdfName, L"改移道路表", MdfRoadNmae);
#endif
}

/*===================================================================
函数名:DrawHorizontalRoad
------------------------------------------------------------
函数功能:绘制线路平面图
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawHorizontalRoad()
{
	//获取平面指针
	if (isCT == 1)
		m_pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);

	//初始化页码数组
	m_DrawingNum.clear();
	m_DrawingNum.resize(7);
	for (int i = 0; i < m_DrawingNum.size(); i++)
		m_DrawingNum[i] = 0;
	CalHoriDrawingNum();

	m_CurX = m_OriX + m_DrawingNum[0] * 1000;
	m_CurY = m_OriY;
	ReadDrawHorizontalPara();
	
	//检测是否绘制平面
	if (!m_OvalDrawPara.IsDrawHoriRoad)
		return;

	m_HDrawPara.DrawMode = 0;
	DrawCenterLine();
	//DrawHorizontalOutput();
	DrawHorizontalOutput_Trimflag_D();
}

/*===================================================================
函数名:DrawCenterLine
------------------------------------------------------------
函数功能:绘制线路中线
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawCenterLine()
{
	// TODO: Implement the command
	CDrawPM* drawpm;
	//m_HDrawPara.ZD = 200;
	//添加起终点
	/*if (!if_PL_CT)
	{
		m_HDrawPara.QD = CTsml;
		m_HDrawPara.ZD = CTeml;
	}*/
	//读取线路信息	
	drawpm = new CDrawPM(m_pRoad, m_HDrawPara.QD, m_HDrawPara.ZD);
	drawpm->m_DrawPara = m_HDrawPara;

	//设置绘图参数
	drawpm->DFACTOR = _wtof(m_HDrawPara.SCALE) / 1000;
	drawpm->texth = m_HDrawPara.WHT;
	//drawpm->DFACTOR = drawpm->DFACTOR * (drawpm->texth / 3.0);
	drawpm->drawfldzdk = m_HDrawPara.drawfldzdk;
	if (m_HDrawPara.XSW == "一位")
		drawpm->XSW = 1;
	else if (m_HDrawPara.XSW == "两位")
		drawpm->XSW = 2;
	else
		drawpm->XSW = 3;

	if (m_HDrawPara.LCBZ == "二十米标")
		drawpm->LcSpacer = 20;
	else if (m_HDrawPara.LCBZ == "百米标")
		drawpm->LcSpacer = 100;
	else if (m_HDrawPara.LCBZ == "五百米标")
		drawpm->LcSpacer = 500;
	else
		drawpm->LcSpacer = 1000;
	drawpm->ZdkMode = m_HDrawPara.ZDKMode;
	drawpm->ZdkZlen = m_HDrawPara.ZcLen;
	drawpm->ZdkYlen = m_HDrawPara.YcLen;
	drawpm->BzRAL = m_HDrawPara.BZRAL;
	drawpm->ZxWid = _wtof(m_HDrawPara.ZxWid)*_wtof(m_HDrawPara.SCALE)/1000;
	drawpm->YdxWid = _wtof(m_HDrawPara.YDXWid)*_wtof(m_HDrawPara.SCALE) / 1000;
	drawpm->m_IsGutter = m_IsGutter;

	//绘制线路中线
	drawpm->Draw_Center_Line();

	if (m_HDrawPara.DrawMode == 0)//线位
	{
		drawpm->DrawJDLine();	//绘制交点连线
		drawpm->BZLC(0);			//标注里程
		drawpm->BZTZD(0);		//标注特征桩
		drawpm->Draw_PI_Infor();	//绘制交点信息	20190706
		if (m_HDrawPara.DrawDZQ)	//绘制大中桥
			drawpm->DrawDZQ();

		if (m_HDrawPara.DrawTun)	//绘制隧道
			drawpm->DrawTun();

		if (m_HDrawPara.DrawBM)		//绘制水准点
			drawpm->DrawBM();

		if (m_HDrawPara.DrawDX)		//绘制导线
			drawpm->DrawDX();

		if (m_HDrawPara.DrawCulvet)	//绘制小桥涵
		{
			drawpm->ReadHdmResultFile();
			drawpm->DrawCulvet();
		}

	}
	else if (m_HDrawPara.DrawMode == 1)	//总体图
	{
		drawpm->BZLC(1);
		//drawpm->BZTZD(1);

		drawpm->DrawZTT();

		drawpm->ZdkMode = 4;
		drawpm->YdxWid = 0.0;
		drawpm->DrawZDK();//绘用地线

		if (m_HDrawPara.DrawBM)
			drawpm->DrawBM();
		if (m_HDrawPara.DrawDX)
			drawpm->DrawDX();

		if (m_HDrawPara.DrawCulvet)
		{
			drawpm->ReadHdmResultFile();
			drawpm->DrawCulvet();
		}
		if (m_HDrawPara.DrawDZQ)
			drawpm->DrawDZQ();
		if (m_HDrawPara.DrawTun)
			drawpm->DrawTun();
	}
	else//用地
	{
		drawpm->BZLC(1);
		drawpm->BZTZD(1);
		drawpm->DrawZDK();
		if (m_HDrawPara.DrawDZQ)
			drawpm->DrawDZQ();
		if (m_HDrawPara.DrawTun)
			drawpm->DrawTun();
		if (m_HDrawPara.DrawBM)
			drawpm->DrawBM();
		if (m_HDrawPara.DrawDX)
			drawpm->DrawDX();
		if (m_HDrawPara.DrawCulvet)
			drawpm->DrawCulvet();
	}

	if (drawpm)
	{
		delete drawpm;
		drawpm = NULL;
	}
}

/*===================================================================
函数名:DrawHorizontalOutput
------------------------------------------------------------
函数功能:绘制线路输出图（带图框，地形）
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawHorizontalOutput()
{
	// TODO: Implement the command
	//关闭捕捉对象
	BAS_DRAW_FUN::Set_OSMODE(0);


	//检测平面
	if (pzLinep == NULL)
	{
		ads_alert(L"请先绘制平面图，再切图!");
		return;
	}

	//读取平面模板
	CString m_XLName, StartLc, EndLc, m_JDGH;
	int Scale, StartNo;
	double m_Texth, LWide, RWide;
	ACHAR ch[256];
	m_Texth = m_HDrawPara.WHT; //字高
	Scale = _wtoi(m_HDrawPara.SCALE); //比例
	double K = Scale / 1000.0; 
	pzLinep = m_pRoad; //平面指针
	StartLc.Format(L"%.3lf", m_HDrawPara.QD); //起点里程
	EndLc.Format(L"%.3lf", m_HDrawPara.ZD); //终点里程
	StartNo = 0;	//暂时设置为0

	double sml, eml;
	sml = pzLinep->TYLC(StartLc);	//起始里程
	eml = pzLinep->TYLC(EndLc);	//终止里程

	//设置图框长宽参数	20190902
	double HFrameLen = 0.0, HFrameHeight = 0.0;
	HFrameLen = m_HDrawPara.FrameLength - 20.0;
	if (m_HDrawPara.DrawMode == 0 && m_HDrawPara.BottomSpace != 0)
	{
		HFrameHeight = m_HDrawPara.FrameHeight - 10.0;	//这是为了留一点空隙，如果到时候不需要刻意删除
		HFrameHeight = HFrameHeight - m_HDrawPara.BottomSpace - m_HDrawPara.TopSpace;	//计算留白
	}
	else
		HFrameHeight = m_HDrawPara.FrameHeight - 20.0;	//这是为了留一点空隙，如果到时候不需要刻意删除

	//计算每幅图的图框中心
	int TukuangNum;
	TukuangNum = int(fabs(eml - sml) / m_HDrawPara.OneMapRoadLen + 50);
	STuKuang* TK = new STuKuang[TukuangNum];
	int TkNum;
	pzLinep->CalTK(_wtof(StartLc), _wtof(EndLc), m_HDrawPara.OneMapRoadLen, HFrameHeight, HFrameLen, TK, TkNum, m_HDrawPara.MoveCenterX, m_HDrawPara.MoveCenterY);
	if (m_HDrawPara.CenterMode == ByCoordinate && TkNum > 0)
		pzLinep->SetTK(TK[0], HFrameHeight, HFrameLen, m_HDrawPara.CenterX, m_HDrawPara.CenterY);

	if (m_TrimFlag == 0)//打断方式
	{
		ads_point OriPt, Lfup, Lfbt, Rtup, Rtbt;
		AcGePoint3d spt, ept;

		//计算绘图中心
		OriPt[X] = m_CurX;
		OriPt[Y] = m_CurY;
		double DrawCenterX = OriPt[X] + HFrameLen / 2.0 + 20 / 2.0;
		double DrawCenterY = 0.0;
		if (m_HDrawPara.DrawMode == 0)
			DrawCenterY = OriPt[Y] + HFrameHeight / 2.0 + m_HDrawPara.BottomSpace;
		else
			DrawCenterY = OriPt[Y] + HFrameHeight / 2.0 + 20 / 2.0;

		pzLinep->TransFormTK(TK, TkNum, DrawCenterX, DrawCenterY, K);

		ACHAR dwgPath[256], bkname[256], filename[256];
		_tcscpy(dwgPath, m_FramePath);
		AcDbObjectId pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, m_HDrawPara.FrameBlockName);

		CString pszLabel = "正在分幅平面图";
		acedSetStatusBarProgressMeter(pszLabel, 0, 100);

		for (int iTK = 0; iTK < TkNum; iTK++)
		{
			int pos = (int)iTK * 100.0 / TkNum;
			acedSetStatusBarProgressMeterPos(pos);
			STuKuang* CurTK = &TK[iTK];

			//计算绘图中心
			OriPt[X] = m_CurX;
			OriPt[Y] = m_CurY;
			double DrawCenterX = OriPt[X] + HFrameLen / 2.0 + 20 / 2.0;
			double DrawCenterY = 0.0;
			if (m_HDrawPara.DrawMode == 0)
				DrawCenterY = OriPt[Y] + HFrameHeight / 2.0 + m_HDrawPara.BottomSpace;
			else
				DrawCenterY = OriPt[Y] + HFrameHeight / 2.0 + 20 / 2.0;

			double dX = 1000.0 * iTK;
			CurTK->newCenX += dX;

			AcGeMatrix3d xformT;
			xformT.setToTranslation(AcGeVector3d(dX, 0.0, 0.0));

			AcGePoint3d p1, p2, p3, p4;
			p1[X] = CurTK->LtUpX;
			p1[Y] = CurTK->LtUpY;

			p2[X] = CurTK->RtUpX;
			p2[Y] = CurTK->RtUpY;

			p3[X] = CurTK->RtBmX;
			p3[Y] = CurTK->RtBmY;

			p4[X] = CurTK->LtBmX;
			p4[Y] = CurTK->LtBmY;

			ads_point pt1, pt2, pt3, pt4;
			ads_point bkpt1, bkpt2, bkpt3, bkpt4;
			bkpt1[X] = p1.x;	bkpt1[Y] = p1.y;
			bkpt2[X] = p2.x;	bkpt2[Y] = p2.y;
			bkpt3[X] = p3.x;	bkpt3[Y] = p3.y;
			bkpt4[X] = p4.x;	bkpt4[Y] = p4.y;

			int result;
			BAS_DRAW_FUN::CalFrame(-2.0 * m_Texth, p1, p2, p3, p4, pt1, pt2, pt3, pt4);

			result = BAS_DRAW_FUN::AutoEntBK(bkpt1, bkpt2, bkpt3, bkpt4);
			ads_printf(L"第%d副图切图完成.\n", iTK + 1);

			if (result > 0)
			{
				BAS_DRAW_FUN::CalFrame(0.01, p1, p2, p3, p4, bkpt1, bkpt2, bkpt3, bkpt4);
				result = BAS_DRAW_FUN::AutoEntBK(bkpt1, bkpt2, bkpt3, bkpt4, result);
			}

			//把框内的所有实体复制转移到指定
			BAS_DRAW_FUN::AutoEntTransform(CurTK->xform, xformT, pt1, pt2, pt3, pt4);

			//20190813新增	切除页码处的实体
			pt2[X] = m_CurX + m_HDrawPara.FrameLength;
			pt2[Y] = m_CurY + m_HDrawPara.FrameHeight;
			pt1[X] = pt2[X] - m_PageLen;
			pt1[Y] = pt2[Y];
			pt3[Y] = pt2[Y] - m_PageHeight;
			pt3[X] = pt2[X];
			pt4[X] = pt2[X] - m_PageLen;
			pt4[Y] = pt2[Y] - m_PageHeight;
			BAS_DRAW_FUN::AutoEntBK(pt1, pt2, pt3, pt4);
			BAS_DRAW_FUN::DeleteEntityRectangle(pt4, pt2);

			//修改底图颜色与图层
			DrawCenterX = m_CurX + HFrameLen / 2.0 + 20 / 2.0;
			ads_point LeftBottom, RightTop;
			LeftBottom[X] = DrawCenterX - HFrameLen / 2.0 - 10.0;
			LeftBottom[Y] = DrawCenterY - HFrameHeight / 2.0 - 10.0;
			LeftBottom[Z] = 0.0;
			RightTop[X] = DrawCenterX + HFrameLen / 2.0 + 10.0;
			RightTop[Y] = DrawCenterY + HFrameHeight / 2.0 + 10.0;
			RightTop[Z] = 0.0;
			ModifyBaseMap(LeftBottom, RightTop);


			if (m_TKPos == 1)//顶
			{
				CurTK->inspt[X] = CurTK->newCenX;
				CurTK->inspt[Y] = CurTK->newCenY - (0.5 * 554 - 15);
			}
			else if (m_TKPos == 2)//中
			{
				CurTK->inspt[X] = m_CurX + m_Frame_Width / 2.0;
				CurTK->inspt[Y] = m_CurY + m_Frame_Height / 2.0;
			}
			else //底
			{
				CurTK->inspt[X] = CurTK->newCenX;
				CurTK->inspt[Y] = CurTK->newCenY + (0.5 * 554 - 15);
			}


			//插入图框
			AcGePoint3d InsertPt(CurTK->inspt[X], CurTK->inspt[Y], 0.0);
			if (pBlockId > 0)
			{
				AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
				pBkRr->setColorIndex(7);
				pBkRr->setLayer(L"图框");
				//pBkRr->setScaleFactors(AcGeScale3d(K,K,1));
				BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
			}

			//如果平面图顶部和底部留白，那么画线
			if (m_HDrawPara.DrawMode == 0 && m_HDrawPara.BottomSpace > 0)
			{
				AcGePoint3d PtStart(m_CurX, m_CurY + m_HDrawPara.BottomSpace, 0.0);
				AcGePoint3d PtEnd(PtStart.x + m_HDrawPara.FrameLength, PtStart.y, 0.0);
				m_pRoad->makeline(PtStart, PtEnd, 0, 1.0, L"图框");
			}
			if (m_HDrawPara.DrawMode == 0 && m_HDrawPara.TopSpace > 0)
			{
				AcGePoint3d PtStart(m_CurX, m_CurY + m_HDrawPara.FrameHeight - m_HDrawPara.TopSpace - 10.0, 0.0);
				AcGePoint3d PtEnd(PtStart.x + m_HDrawPara.FrameLength, PtStart.y, 0.0);
				m_pRoad->makeline(PtStart, PtEnd, 0, 1.0, L"图框");
			}

			StartNo++;
			m_CurX += 1000.0;	//20190811
		}
		acedSetStatusBarProgressMeterPos(100);
	}
	else
	{

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"zoom", RTSTR, L"e", RTNONE);	//自动缩放以保证能够切到完整的图
#endif
#ifdef VERSION_CAD2010 
	ads_command(RTSTR, L"zoom", RTSTR, L"e", RTNONE);//更新视图	
#endif	
		pzLinep->TransFormTK(TK, TkNum, 411.48 / 2.0, 284.90 / 2.0, 1);//K->1

		for (int iTK = 0; iTK < TkNum; iTK++)
		{
			STuKuang* CurTK = &TK[iTK];

			ACHAR LayoutName[256];
			if (m_HDrawPara.DrawMode == 0)
				_stprintf(LayoutName, L"PM%s～PM%s", CurTK->StartXCLC, CurTK->EndXCLC);
			else if(m_HDrawPara.DrawMode == 1)
				_stprintf(LayoutName, L"PMZT%s～PMZT%s", CurTK->StartXCLC, CurTK->EndXCLC);
			else if (m_HDrawPara.DrawMode == 2)
				_stprintf(LayoutName, L"YD%s～YD%s", CurTK->StartXCLC, CurTK->EndXCLC);

			CMyLayout::CreateLayout(LayoutName); 

#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
			acedCommandS(RTSTR, L"-OSNAP", RTSTR, L"OFF");
#endif
#ifdef VERSION_CAD2010 
			ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
			ads_command(RTSTR, L"-OSNAP", RTSTR, L"OFF");
#endif	


#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE); 
			acedCommandS(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE); //设置坐标系
			ads_command(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE); //清除布局中所有实体
#endif // VERSION_CAD2010

			ads_point p1, p2, p3, p4, z1, z2, ori, orix, oriy;
			double xmin, xmax, ymin, ymax;
			p1[X] = CurTK->newLtUpX;
			p1[Y] = CurTK->newLtUpY;

			p2[X] = CurTK->newRtUpX;
			p2[Y] = CurTK->newRtUpY;

			p3[X] = CurTK->newRtBmX;
			p3[Y] = CurTK->newRtBmY;

			p4[X] = CurTK->newLtBmX;
			p4[Y] = CurTK->newLtBmY;

			double jiao;
			BAS_DRAW_FUN::xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);
			jiao += 0.5 * PI;
			ori[X] = CurTK->LtBmX;
			ori[Y] = CurTK->LtBmY;

			orix[X] = CurTK->RtBmX;
			orix[Y] = CurTK->RtBmY;

			oriy[X] = ori[X] + 10 * cos(jiao);
			oriy[Y] = ori[Y] + 10 * sin(jiao);

			xmin = 0.0;
			xmax = (p3[X] - p4[X]);//*K;
			ymin = 0.0;
			ymax = (p2[Y] - p3[Y]);//*K;

			if (p1[X] < p4[X])xmin -= (p4[X] - p1[X]);//*K;
			if (p2[X] > p3[X])xmax += (p2[X] - p3[X]);//*K;					

			z1[X] = xmin;
			z1[Y] = ymin;
			z2[X] = xmax;
			z2[Y] = ymax;
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
			acedCommandS(RTSTR, L"MSPACE", RTNONE);
			acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);

			acedCommandS(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, ori, RTPOINT, orix, RTPOINT, oriy, RTNONE);
			acedCommandS(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);

			acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE);
			acedCommandS(RTSTR, L"PSPACE", RTNONE);
			acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
			acedCommandS(RTSTR, L"-VPORTS", RTSTR, L"LOCK", RTSTR, L"ON", RTSTR, L"ALL", RTSTR, L"", RTNONE);
			acedCommandS(RTSTR, L"ZOOM", RTSTR, L"E", RTNONE);
			acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
			ads_command(RTSTR, L"MSPACE", RTNONE);
			ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);

			ads_command(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, ori, RTPOINT, orix, RTPOINT, oriy, RTNONE);//设置坐标系
			ads_command(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);//更新视图	

			ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE); //缩放
			ads_command(RTSTR, L"PSPACE", RTNONE);
			ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
			ads_command(RTSTR, L"-VPORTS", RTSTR, L"LOCK", RTSTR, L"ON", RTSTR, L"ALL", RTSTR, L"", RTNONE);
			ads_command(RTSTR, L"ZOOM", RTSTR, L"E", RTNONE);
			ads_command(RTNONE);
#endif // VERSION_CAD2010
			ACHAR dwgPath[256];
			_tcscpy(dwgPath, m_FramePath);
			AcDbObjectId pBlockId;
			pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"");
			if (pBlockId > 0)
			{
				AcGePoint3d InsertPt(411.48 / 2.0, 284.90 / 2.0, 0.0);
				AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
				pBkRr->setScaleFactors(AcGeScale3d(K, K, 1));
				BAS_DRAW_FUN::AddEntityToDbs(pBkRr, 0, 1);
			}
#ifdef VERSION_CAD2016
			acedCommandS(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
				RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
				RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
			acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
			ads_command(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
				RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
				RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
			ads_command(RTNONE);
#endif // VERSION_CAD2010
		}
	}
	if (TK)delete[]TK;
}

/*===================================================================
函数名:DrawHorizontalOutput_Trimflag_D
------------------------------------------------------------
函数功能:绘制线路输出图（打断先出，再截取）
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawHorizontalOutput_Trimflag_D()
{
	// TODO: Implement the command
	//关闭捕捉对象
	BAS_DRAW_FUN::Set_OSMODE(0);


	//检测平面
	if (pzLinep == NULL)
	{
		ads_alert(L"请先绘制平面图，再切图!");
		return;
	}

	//读取平面模板
	CString m_XLName, StartLc, EndLc, m_JDGH;
	int Scale, StartNo;
	double m_Texth, LWide, RWide;
	ACHAR ch[256];
	m_Texth = m_HDrawPara.WHT; //字高
	Scale = _wtoi(m_HDrawPara.SCALE); //比例
	double K = Scale / 1000.0;
	pzLinep = m_pRoad; //平面指针
	//起终点标记
	StartLc.Format(L"%.3lf", m_HDrawPara.QD); //起点里程
	EndLc.Format(L"%.3lf", m_HDrawPara.ZD); //终点里程
	StartNo = 0;	//暂时设置为0

	double sml, eml;
	sml = pzLinep->TYLC(StartLc);	//起始里程
	eml = pzLinep->TYLC(EndLc);	//终止里程

	//设置图框长宽参数	20190902
	double HFrameLen = 0.0, HFrameHeight = 0.0;
	HFrameLen = m_HDrawPara.FrameLength - 20.0;
	if (m_HDrawPara.DrawMode == 0 && m_HDrawPara.BottomSpace != 0)
	{
		HFrameHeight = m_HDrawPara.FrameHeight - 10.0;	//这是为了留一点空隙，如果到时候不需要刻意删除
		HFrameHeight = HFrameHeight - m_HDrawPara.BottomSpace - m_HDrawPara.TopSpace;	//计算留白
	}
	else
		HFrameHeight = m_HDrawPara.FrameHeight - 20.0;	//这是为了留一点空隙，如果到时候不需要刻意删除

	//计算每幅图的图框中心
	int TukuangNum;
	TukuangNum = int(fabs(eml - sml) / m_HDrawPara.OneMapRoadLen + 50);
	STuKuang* TK = new STuKuang[TukuangNum];
	int TkNum;
	pzLinep->CalTK(_wtof(StartLc), _wtof(EndLc), m_HDrawPara.OneMapRoadLen, HFrameHeight, HFrameLen, TK, TkNum, m_HDrawPara.MoveCenterX, m_HDrawPara.MoveCenterY);
	if (m_HDrawPara.CenterMode == ByCoordinate && TkNum > 0)
		pzLinep->SetTK(TK[0], HFrameHeight, HFrameLen, m_HDrawPara.CenterX, m_HDrawPara.CenterY);

	//打断出图
	ads_point OriPt, Lfup, Lfbt, Rtup, Rtbt;
	AcGePoint3d spt, ept;
	double m_CT_CurX, m_CT_CurY;
	m_CT_CurY = m_CurX;
	m_CT_CurY = m_CurY;
	//计算绘图中心
	OriPt[X] = m_CurX;
	OriPt[Y] = m_CurY;
	double DrawCenterX = OriPt[X] + HFrameLen / 2.0 + 20 / 2.0;
	double DrawCenterY = 0.0;
	if (m_HDrawPara.DrawMode == 0)
		DrawCenterY = OriPt[Y] + HFrameHeight / 2.0 + m_HDrawPara.BottomSpace;
	else
		DrawCenterY = OriPt[Y] + HFrameHeight / 2.0 + 20 / 2.0;

	pzLinep->TransFormTK(TK, TkNum, DrawCenterX, DrawCenterY, K);

	ACHAR dwgPath[256], bkname[256], filename[256];
	_tcscpy(dwgPath, m_FramePath);
	AcDbObjectId pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, m_HDrawPara.FrameBlockName);

	CString pszLabel = "正在分幅平面图";
	acedSetStatusBarProgressMeter(pszLabel, 0, 100);

	for (int iTK = 0; iTK < TkNum; iTK++)
	{
		int pos = (int)iTK * 100.0 / TkNum;
		acedSetStatusBarProgressMeterPos(pos);
		STuKuang* CurTK = &TK[iTK];

		//计算绘图中心
		OriPt[X] = m_CurX;
		OriPt[Y] = m_CurY;
		double DrawCenterX = OriPt[X] + HFrameLen / 2.0 + 20 / 2.0;
		double DrawCenterY = 0.0;
		if (m_HDrawPara.DrawMode == 0)
			DrawCenterY = OriPt[Y] + HFrameHeight / 2.0 + m_HDrawPara.BottomSpace;
		else
			DrawCenterY = OriPt[Y] + HFrameHeight / 2.0 + 20 / 2.0;

		double dX = 1000.0 * iTK;
		CurTK->newCenX += dX;

		AcGeMatrix3d xformT;
		xformT.setToTranslation(AcGeVector3d(dX, 0.0, 0.0));

		AcGePoint3d p1, p2, p3, p4;
		p1[X] = CurTK->LtUpX;
		p1[Y] = CurTK->LtUpY;

		p2[X] = CurTK->RtUpX;
		p2[Y] = CurTK->RtUpY;

		p3[X] = CurTK->RtBmX;
		p3[Y] = CurTK->RtBmY;

		p4[X] = CurTK->LtBmX;
		p4[Y] = CurTK->LtBmY;

		ads_point pt1, pt2, pt3, pt4;
		ads_point bkpt1, bkpt2, bkpt3, bkpt4;
		bkpt1[X] = p1.x;	bkpt1[Y] = p1.y;
		bkpt2[X] = p2.x;	bkpt2[Y] = p2.y;
		bkpt3[X] = p3.x;	bkpt3[Y] = p3.y;
		bkpt4[X] = p4.x;	bkpt4[Y] = p4.y;
		bkpt1[Z] = 0.0;
		bkpt2[Z] = 0.0;
		bkpt3[Z] = 0.0;
		bkpt4[Z] = 0.0;

		int result;
		BAS_DRAW_FUN::CalFrame(-2.0 * m_Texth, p1, p2, p3, p4, pt1, pt2, pt3, pt4);

		result = BAS_DRAW_FUN::AutoEntBK(bkpt1, bkpt2, bkpt3, bkpt4);
		result = BAS_DRAW_FUN::AutoEntBK(bkpt1, bkpt2, bkpt3, bkpt4);	//20191204	第一遍的时候主线有时切不断
		ads_printf(L"第%d副图切图完成.\n", iTK + 1);

		if (result > 0)
		{
			BAS_DRAW_FUN::CalFrame(0.01, p1, p2, p3, p4, bkpt1, bkpt2, bkpt3, bkpt4);
			result = BAS_DRAW_FUN::AutoEntBK(bkpt1, bkpt2, bkpt3, bkpt4, result);
		}

		//把框内的所有实体复制转移到指定
		BAS_DRAW_FUN::AutoEntTransform(CurTK->xform, xformT, pt1, pt2, pt3, pt4);

		//20190813新增	切除页码处的实体
		pt2[X] = m_CurX + m_HDrawPara.FrameLength;
		pt2[Y] = m_CurY + m_HDrawPara.FrameHeight;
		pt1[X] = pt2[X] - m_PageLen;
		pt1[Y] = pt2[Y];
		pt3[Y] = pt2[Y] - m_PageHeight;
		pt3[X] = pt2[X];
		pt4[X] = pt2[X] - m_PageLen;
		pt4[Y] = pt2[Y] - m_PageHeight;
		BAS_DRAW_FUN::AutoEntBK(pt1, pt2, pt3, pt4);
		BAS_DRAW_FUN::DeleteEntityRectangle(pt4, pt2);

		//修改底图颜色与图层
		DrawCenterX = m_CurX + HFrameLen / 2.0 + 20 / 2.0;
		ads_point LeftBottom, RightTop;
		LeftBottom[X] = DrawCenterX - HFrameLen / 2.0 - 10.0;
		LeftBottom[Y] = DrawCenterY - HFrameHeight / 2.0 - 10.0;
		LeftBottom[Z] = 0.0;
		RightTop[X] = DrawCenterX + HFrameLen / 2.0 + 10.0;
		RightTop[Y] = DrawCenterY + HFrameHeight / 2.0 + 10.0;
		RightTop[Z] = 0.0;
		ModifyBaseMap(LeftBottom, RightTop);


		if (m_TKPos == 1)//顶
		{
			CurTK->inspt[X] = CurTK->newCenX;
			CurTK->inspt[Y] = CurTK->newCenY - (0.5 * 554 - 15);
		}
		else if (m_TKPos == 2)//中
		{
			CurTK->inspt[X] = m_CurX + m_Frame_Width / 2.0;
			CurTK->inspt[Y] = m_CurY + m_Frame_Height / 2.0;
		}
		else //底
		{
			CurTK->inspt[X] = CurTK->newCenX;
			CurTK->inspt[Y] = CurTK->newCenY + (0.5 * 554 - 15);
		}


		//插入图框
		AcGePoint3d InsertPt(CurTK->inspt[X], CurTK->inspt[Y], 0.0);
		if (pBlockId > 0)
		{
			AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
			pBkRr->setColorIndex(7);
			pBkRr->setLayer(L"图框");
			//pBkRr->setScaleFactors(AcGeScale3d(K,K,1));
			BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
		}

		//如果平面图顶部和底部留白，那么画线
		if (m_HDrawPara.DrawMode == 0 && m_HDrawPara.BottomSpace > 0)
		{
			AcGePoint3d PtStart(m_CurX, m_CurY + m_HDrawPara.BottomSpace, 0.0);
			AcGePoint3d PtEnd(PtStart.x + m_HDrawPara.FrameLength, PtStart.y, 0.0);
			m_pRoad->makeline(PtStart, PtEnd, 0, 1.0, L"图框");
		}
		if (m_HDrawPara.DrawMode == 0 && m_HDrawPara.TopSpace > 0)
		{
			AcGePoint3d PtStart(m_CurX, m_CurY + m_HDrawPara.FrameHeight - m_HDrawPara.TopSpace - 10.0, 0.0);
			AcGePoint3d PtEnd(PtStart.x + m_HDrawPara.FrameLength, PtStart.y, 0.0);
			m_pRoad->makeline(PtStart, PtEnd, 0, 1.0, L"图框"); 
		}

		if(m_TrimFlag == 1)
			DrawHorizontalOutput_Trimflag_B(CurTK, 0);
		StartNo++;
		m_CurX += 1000.0;	//20190811
	}

	acedSetStatusBarProgressMeterPos(100);
	if (TK)
		delete[]TK;
}

/*===================================================================
函数名:DrawHorizontalOutput_Trimflag_B
------------------------------------------------------------
函数功能:绘制线路输出图（布局）
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawHorizontalOutput_Trimflag_B(STuKuang *OriTK,int TKnum)
{
//#ifdef VERSION_CAD2016
//	acedCommandS(RTSTR, L"zoom", RTSTR, L"e", RTNONE);	//自动缩放以保证能够切到完整的图
//#endif
//#ifdef VERSION_CAD2010 
//	ads_command(RTSTR, L"zoom", RTSTR, L"e", RTNONE);//更新视图	
//#endif	

	//关闭捕捉对象
	BAS_DRAW_FUN::Set_OSMODE(0);
#ifdef VERSION_CAD2016
	acedCommandS(RTNONE);
	acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	acedCommandS(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);
	acedCommandS(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTNONE);
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);	//命令行提示关
	ads_command(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);	//十字架模式关
	ads_command(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);	//设置坐标系
#endif // VERSION_CAD2010
	AcGePoint3d pt;
	pt.x = OriTK[TKnum].newCenX;
	pt.y = OriTK[TKnum].newCenY;

	//只有中心点正确
	STuKuang* Cu_TK = NULL;
	Cu_TK = new STuKuang[1];

	Cu_TK->CenX = pt.x;
	Cu_TK->CenY = pt.y;
	if (m_HDrawPara.DrawMode = 0) //保证线位图重叠
	{
		Cu_TK->LtUpX = Cu_TK->CenX - m_Frame_Width / 2.0 - 50.0;
		Cu_TK->LtUpY = Cu_TK->CenY + m_Frame_Height / 2.0 + 20.0;

		Cu_TK->LtBmX = Cu_TK->CenX - m_Frame_Width / 2.0 - 50.0;
		Cu_TK->LtBmY = Cu_TK->CenY - m_Frame_Height / 2.0 - 20.0;

		Cu_TK->RtUpX = Cu_TK->CenX + m_Frame_Width / 2.0 + 10.0;
		Cu_TK->RtUpY = Cu_TK->CenY + m_Frame_Height / 2.0 + 20.0;

		Cu_TK->RtBmX = Cu_TK->CenX + m_Frame_Width / 2.0 + 10.0;
		Cu_TK->RtBmY = Cu_TK->CenY - m_Frame_Height / 2.0 - 20.0;
	}
	else
	{
		Cu_TK->LtUpX = Cu_TK->CenX - m_Frame_Width / 2.0 - 50.0;
		Cu_TK->LtUpY = Cu_TK->CenY + m_Frame_Height / 2.0 + 10.0;

		Cu_TK->LtBmX = Cu_TK->CenX - m_Frame_Width / 2.0 - 50.0;
		Cu_TK->LtBmY = Cu_TK->CenY - m_Frame_Height / 2.0 - 30.0;

		Cu_TK->RtUpX = Cu_TK->CenX + m_Frame_Width / 2.0 + 10.0;
		Cu_TK->RtUpY = Cu_TK->CenY + m_Frame_Height / 2.0 + 10.0;

		Cu_TK->RtBmX = Cu_TK->CenX + m_Frame_Width / 2.0 + 10.0;
		Cu_TK->RtBmY = Cu_TK->CenY - m_Frame_Height / 2.0 - 30.0;
	}


	TransFormTK_PM(Cu_TK,411.48 / 2.0, 284.90 / 2.0, 1);//K->1

	STuKuang* CurTK = &Cu_TK[0];

	ACHAR LayoutName[256];
	if (m_HDrawPara.DrawMode == 0)
		_stprintf(LayoutName, L"PM%s～PM%s", OriTK[TKnum].StartXCLC, OriTK[TKnum].EndXCLC);
	else if (m_HDrawPara.DrawMode == 1)
		_stprintf(LayoutName, L"PMZT%s～PMZT%s", OriTK[TKnum].StartXCLC, OriTK[TKnum].EndXCLC);
	else if (m_HDrawPara.DrawMode == 2)
		_stprintf(LayoutName, L"YD%s～YD%s", OriTK[TKnum].StartXCLC, OriTK[TKnum].EndXCLC);

	CMyLayout::CreateLayout(LayoutName);

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	acedCommandS(RTSTR, L"-OSNAP", RTSTR, L"OFF");
#endif
#ifdef VERSION_CAD2010 
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	ads_command(RTSTR, L"-OSNAP", RTSTR, L"OFF");
#endif	


#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	acedCommandS(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE); //设置坐标系
	ads_command(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE); //清除布局中所有实体
#endif // VERSION_CAD2010

	ads_point p1, p2, p3, p4, z1, z2, ori, orix, oriy;
	double xmin, xmax, ymin, ymax;
	p1[X] = CurTK->newLtUpX;
	p1[Y] = CurTK->newLtUpY;

	p2[X] = CurTK->newRtUpX;
	p2[Y] = CurTK->newRtUpY;

	p3[X] = CurTK->newRtBmX;
	p3[Y] = CurTK->newRtBmY;

	p4[X] = CurTK->newLtBmX;
	p4[Y] = CurTK->newLtBmY;

	double jiao;
	BAS_DRAW_FUN::xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);
	jiao += 0.5 * PI;
	ori[X] = CurTK->LtBmX;
	ori[Y] = CurTK->LtBmY;

	orix[X] = CurTK->RtBmX;
	orix[Y] = CurTK->RtBmY;

	oriy[X] = ori[X] + 10 * cos(jiao);
	oriy[Y] = ori[Y] + 10 * sin(jiao);

	xmin = 0.0;
	xmax = (p3[X] - p4[X]);//*K;
	ymin = 0.0;
	ymax = (p2[Y] - p3[Y]);//*K;

	if (p1[X] < p4[X])xmin -= (p4[X] - p1[X]);//*K;
	if (p2[X] > p3[X])xmax += (p2[X] - p3[X]);//*K;					

	z1[X] = xmin;
	z1[Y] = ymin;
	z2[X] = xmax;
	z2[Y] = ymax;
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
	acedCommandS(RTSTR, L"MSPACE", RTNONE);
	acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);

	acedCommandS(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, ori, RTPOINT, orix, RTPOINT, oriy, RTNONE);
	acedCommandS(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);

	acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE);
	acedCommandS(RTSTR, L"PSPACE", RTNONE);
	acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	acedCommandS(RTSTR, L"-VPORTS", RTSTR, L"LOCK", RTSTR, L"ON", RTSTR, L"ALL", RTSTR, L"", RTNONE);
	acedCommandS(RTSTR, L"ZOOM", RTSTR, L"E", RTNONE);
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
	ads_command(RTSTR, L"MSPACE", RTNONE);
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);

	ads_command(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, ori, RTPOINT, orix, RTPOINT, oriy, RTNONE);//设置坐标系
	ads_command(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);//更新视图	

	ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE); //缩放
	ads_command(RTSTR, L"PSPACE", RTNONE);
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	ads_command(RTSTR, L"-VPORTS", RTSTR, L"LOCK", RTSTR, L"ON", RTSTR, L"ALL", RTSTR, L"", RTNONE);
	ads_command(RTSTR, L"ZOOM", RTSTR, L"E", RTNONE);
	ads_command(RTNONE);
#endif // VERSION_CAD2010
	ACHAR dwgPath[256];
	_tcscpy(dwgPath, m_FramePath);
	AcDbObjectId pBlockId;
	pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"");
	if (pBlockId > 0)
	{
		AcGePoint3d InsertPt(411.48 / 2.0, 284.90 / 2.0, 0.0);
		AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setScaleFactors(AcGeScale3d(1, 1, 1));//K
		BAS_DRAW_FUN::AddEntityToDbs(pBkRr, 0, 1);
	}
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
	ads_command(RTNONE);
#endif // VERSION_CAD2010


	if (Cu_TK)delete[]Cu_TK;

}

void RoadAutoDesign:: TransFormTK_PM(STuKuang *TK, double CenX, double CenY, double scale)
{
	AcGeVector3d norm(0, 0, 1);
	norm.normalize();

	STuKuang *CurTK = &TK[0];

	CurTK->newCenX = CenX;
	CurTK->newCenY = CenY;

	AcGeMatrix3d xformR, xformT, xformS;
	double jiao;
	BAS_DRAW_FUN::xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);

	AcGeVector3d  moveBy(CurTK->newCenX - CurTK->CenX, CurTK->newCenY - CurTK->CenY, 0.0);
	AcGePoint3d PP(CurTK->CenX, CurTK->CenY, 0.0);
	xformR.setToRotation(-1.0*jiao, norm, PP);
	xformT.setToTranslation(moveBy);
	CurTK->xform = xformT * xformR;

	AcGePoint3d StPt(CurTK->StX, CurTK->StY, 0.0);
	StPt.transformBy(CurTK->xform);
	CurTK->newStX = StPt.x;
	CurTK->newStY = StPt.y;

	AcGePoint3d EdPt(CurTK->EdX, CurTK->EdY, 0.0);
	EdPt.transformBy(CurTK->xform);
	CurTK->newEdX = EdPt.x;
	CurTK->newEdY = EdPt.y;

	AcGePoint3d LtUpPt(CurTK->LtUpX, CurTK->LtUpY, 0.0);
	LtUpPt.transformBy(CurTK->xform);
	CurTK->newLtUpX = LtUpPt.x;
	CurTK->newLtUpY = LtUpPt.y;

	AcGePoint3d LtBmPt(CurTK->LtBmX, CurTK->LtBmY, 0.0);
	LtBmPt.transformBy(CurTK->xform);
	CurTK->newLtBmX = LtBmPt.x;
	CurTK->newLtBmY = LtBmPt.y;

	AcGePoint3d RtUpPt(CurTK->RtUpX, CurTK->RtUpY, 0.0);
	RtUpPt.transformBy(CurTK->xform);
	CurTK->newRtUpX = RtUpPt.x;
	CurTK->newRtUpY = RtUpPt.y;

	AcGePoint3d RtBmPt(CurTK->RtBmX, CurTK->RtBmY, 0.0);
	RtBmPt.transformBy(CurTK->xform);
	CurTK->newRtBmX = RtBmPt.x;
	CurTK->newRtBmY = RtBmPt.y;
}

/*===================================================================
函数名:ReadDrawHorizontalPara
------------------------------------------------------------
函数功能:读取平面绘图参数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::ReadDrawHorizontalPara()
{

	CString FName = m_MdbName;
	FName.Replace(L".mdb", L".pmcs");

	//现在改从外部传递参数数据	20190902
	//if (!ReadCSFile(FName))
	//{
	//	CString tempCStr = SysPath;
	//	FName = tempCStr + L"\\lib\\模板.pmcs";
	//	if (!ReadCSFile(FName))
	//	{
	//		AfxMessageBox(FName + L"不存在!");
	//		return;
	//	}
	//	//修改一些默认值
	//	m_HDrawPara.ZxWid = L"1.4";
	//	//m_HDrawPara.LCBZ = L"不标注";
	//}
	//m_HDrawPara.LCBZ = L"百米标";	//20190821	因为目前应用是均要标百米标，这里默认给百米标

	ACHAR str[256];
	ads_rtos(m_pRoad->DLArray[0].ELC, 2, 3, str);
	m_HDrawPara.QD = _wtof(str);
	ads_rtos(m_pRoad->DLArray[m_pRoad->DLNum - 1].BLC, 2, 3, str);
	m_HDrawPara.ZD = _wtof(str);
	_stprintf(m_HDrawPara.FrameBlockName, L"图框-%s", Pro);	//20190910新增	修改图框名称
	WriteCSFile(FName);

	FName = m_MdbName;
	FName.Replace(L".mdb", L".PmDrawSet");
	WriteDrawSetFile(FName);
}

/*===================================================================
函数名:ReadCSFile
------------------------------------------------------------
函数功能:读取平面参数文件
------------------------------------------------------------
输入:	CString		FilePath		参数文件的路径
------------------------------------------------------------
输出:	bool		返回值			true-读取成功	false-读取失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadAutoDesign::ReadCSFile(CString FilePath)
{
	FILE* fp = NULL;
	fp = _wfopen(FilePath, L"r");
	ACHAR TempChr[20];
	if (fp)
	{
		fwscanf(fp, L"%d\n", &m_HDrawPara.DrawMode);//绘图方式 0  线位图 1 平面总体图 2 用地图
		fwscanf(fp, L"%d\n", &m_HDrawPara.DrawAll);//是否绘所有线路0-是 1-非
		fwscanf(fp, L"%d\n", &m_HDrawPara.ZDKMode);//绘制占地图方式
		fwscanf(fp, L"%s\n", TempChr);//道路名
		m_HDrawPara.RoadName = TempChr;
		fwscanf(fp, L"%lf\n", &m_HDrawPara.QD);//起点里程
		fwscanf(fp, L"%lf\n", &m_HDrawPara.ZD);//终点里程
		fwscanf(fp, L"%s\n", TempChr);//比例
		m_HDrawPara.SCALE = TempChr;
		fwscanf(fp, L"%s\n", TempChr);//小数位数
		m_HDrawPara.XSW = TempChr;
		fwscanf(fp, L"%lf\n", &m_HDrawPara.WHT);//字高
		fwscanf(fp, L"%s\n", TempChr);//里程标注样式
		m_HDrawPara.LCBZ = TempChr;
		fwscanf(fp, L"%lf\n", &m_HDrawPara.ZcLen);//左侧用地标注线长
		fwscanf(fp, L"%lf\n", &m_HDrawPara.YcLen);//右侧用地标注线长
		fwscanf(fp, L"%s\n", TempChr);//线路中线宽度
		m_HDrawPara.ZxWid = TempChr;
		fwscanf(fp, L"%s\n", TempChr);//用地桩线宽度
		m_HDrawPara.YDXWid = TempChr;
		fwscanf(fp, L"%d\n", &m_HDrawPara.DrawDZQ);
		fwscanf(fp, L"%d\n", &m_HDrawPara.DrawTun);
		fwscanf(fp, L"%d\n", &m_HDrawPara.DrawCulvet);
		fwscanf(fp, L"%d\n", &m_HDrawPara.DrawBM);
		fwscanf(fp, L"%d\n", &m_HDrawPara.DrawDX);
		fwscanf(fp, L"%d\n", &m_HDrawPara.DelEntInf);
		fwscanf(fp, L"%d\n", &m_HDrawPara.BZRAL);
		fwscanf(fp, L"%d\n", &m_HDrawPara.DrawJD);
		fwscanf(fp, L"%d\n", &m_HDrawPara.drawfldzdk);
		fclose(fp);
		return true;
	}
	else
		return false;
}

/*===================================================================
函数名:WriteCSFile
------------------------------------------------------------
函数功能:写入平面参数文件
------------------------------------------------------------
输入:	CString		FilePath		参数文件的路径
------------------------------------------------------------
输出:	bool		返回值			true-写入成功	false-写入失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadAutoDesign::WriteCSFile(CString FilePath)
{
	FILE* fp = NULL;
	fp = _wfopen(FilePath, L"w");
	if (fp)
	{
		fwprintf(fp, L"%d\n", m_HDrawPara.DrawMode);//绘图方式 0  线位图 1 平面总体图 2 用地图
		fwprintf(fp, L"%d\n", m_HDrawPara.DrawAll);//是否绘所有线路0-是 1-非
		fwprintf(fp, L"%d\n", m_HDrawPara.ZDKMode);//绘制占地图方式
		fwprintf(fp, L"%s\n", m_HDrawPara.RoadName);//道路名
		fwprintf(fp, L"%lf\n", m_HDrawPara.QD);//起点里程
		fwprintf(fp, L"%lf\n", m_HDrawPara.ZD);//终点里程
		fwprintf(fp, L"%s\n", m_HDrawPara.SCALE);//比例
		fwprintf(fp, L"%s\n", m_HDrawPara.XSW);//小数位数
		fwprintf(fp, L"%lf\n", m_HDrawPara.WHT);//字高
		fwprintf(fp, L"%s\n", m_HDrawPara.LCBZ);//里程标注样式
		fwprintf(fp, L"%lf\n", m_HDrawPara.ZcLen);//左侧用地标注线长
		fwprintf(fp, L"%lf\n", m_HDrawPara.YcLen);//右侧用地标注线长
		fwprintf(fp, L"%s\n", m_HDrawPara.ZxWid);//线路中线宽度
		fwprintf(fp, L"%s\n", m_HDrawPara.YDXWid);//用地桩线宽度
		fwprintf(fp, L"%d\n", m_HDrawPara.DrawDZQ);//绘大中桥
		fwprintf(fp, L"%d\n", m_HDrawPara.DrawTun);//绘隧道
		fwprintf(fp, L"%d\n", m_HDrawPara.DrawCulvet);//绘小桥涵
		fwprintf(fp, L"%d\n", m_HDrawPara.DrawBM);//绘水准点
		fwprintf(fp, L"%d\n", m_HDrawPara.DrawDX);//绘导线
		fwprintf(fp, L"%d\n", m_HDrawPara.DelEntInf);//删除界内实体
		fwprintf(fp, L"%d\n", m_HDrawPara.BZRAL);//标注半径缓长
		fwprintf(fp, L"%d\n", m_HDrawPara.DrawJD);//绘交点
		fwprintf(fp, L"%d\n", m_HDrawPara.drawfldzdk);//绘分离段占地宽
		fclose(fp);
		return true;
	}
	else
		return false;
}

bool RoadAutoDesign::ReadDrawSetFile(CString FilePath)
{
	return false;
}

/*===================================================================
函数名:WriteDrawSetFile
------------------------------------------------------------
函数功能:写入分幅输出参数
------------------------------------------------------------
输入:	CString		FilePath		参数文件的路径
------------------------------------------------------------
输出:	bool		返回值			true-写入成功	false-写入失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
bool RoadAutoDesign::WriteDrawSetFile(CString FilePath)
{
	FILE* fp = NULL;
	fp = _wfopen(FilePath, L"w");
	if (fp)
	{
		fwprintf(fp, L"%s\n", m_HDrawPara.RoadName);
		fwprintf(fp, L"%0.3lf\n", m_HDrawPara.QD);
		fwprintf(fp, L"%0.3lf\n", m_HDrawPara.ZD);
		fwprintf(fp, L"%d\n", _wtoi(m_HDrawPara.SCALE));
		if (m_HDrawPara.XSW == "一位")
			fwprintf(fp, L"%d\n", 1);
		else if (m_HDrawPara.XSW == "两位")
			fwprintf(fp, L"%d\n", 2);
		else fwprintf(fp, L"%d\n", 3);
		fwprintf(fp, L"%0.1lf\n", m_HDrawPara.WHT);
		fclose(fp);

		return true;
	}
	else
		return false;
}

//修改底图颜色与图层	20190918
void RoadAutoDesign::ModifyBaseMap(ads_point& LeftBottom, ads_point& RightTop)
{
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	ads_point LeftTop, RightBottom;
	LeftTop[X] = LeftBottom[X];
	LeftTop[Y] = RightTop[Y];
	LeftTop[Z] = 0.0;
	RightBottom[X] = RightTop[X];
	RightBottom[Y] = LeftBottom[Y];
	RightBottom[Z] = 0.0;

	std::vector<CString> FixLayerArray;
	FixLayerArray.push_back(L"平面图-中线");
	FixLayerArray.push_back(L"平面图-交点连线");
	FixLayerArray.push_back(L"平面图-交点信息");
	FixLayerArray.push_back(L"平面图-里程");
	FixLayerArray.push_back(L"平面图-特征桩");
	FixLayerArray.push_back(L"平面图-占地边界线");
	FixLayerArray.push_back(L"平面图-占地线");
	FixLayerArray.push_back(L"平面图-路肩线");
	FixLayerArray.push_back(L"平面图-平台线");
	FixLayerArray.push_back(L"平面图-坡脚线");
	FixLayerArray.push_back(L"平面图-水沟箭头");
	FixLayerArray.push_back(L"平面图-填方示坡线");
	FixLayerArray.push_back(L"平面图-挖方示坡线");
	FixLayerArray.push_back(L"平面图-水沟线");
	FixLayerArray.push_back(L"平面图-边沟线");
	FixLayerArray.push_back(L"平面图-边沟线");
	FixLayerArray.push_back(L"平面图-分隔带线");

	BAS_DRAW_FUN::CreateLayer(L"平面图-底图");

	ads_name ssname, ename;
	struct resbuf* pointlist;
	long n;
	AcDbObjectId eId;
	AcDbEntity* pEnt, * pEntclone;
	pointlist = ads_buildlist(RTPOINT, LeftBottom, RTPOINT, LeftTop, RTPOINT, RightTop, RTPOINT, RightBottom, 0);
	acedSSGet(L"WP", pointlist, NULL, NULL, ssname);
	if (ads_sslength(ssname, &n) == RTNORM)
	{
		int Max = n;
		double Cur = 0;
		int Pos = 0;
		while (n > 0)
		{
			Cur = (Max - n) * 100.0 / Max;
			Pos = (int)Cur;
			acedSetStatusBarProgressMeterPos(Pos);

			n = n - 1;
			ads_ssname(ssname, n, ename);
			acdbGetObjectId(eId, ename);
			if (Acad::eOk != acdbOpenObject(pEnt, eId, AcDb::kForWrite))
				continue;

			pEnt->assertWriteEnabled();

			ACHAR LayerName[256];
			_tcscpy(LayerName, pEnt->layer());
			int CurIndex = 999;
			for (int i = 0; i < FixLayerArray.size(); i++)
			{
				if (_tcscmp(LayerName, FixLayerArray[i]) == 0)
				{
					pEnt->close();
					CurIndex = i;
					break;
				}
			}
			if(CurIndex < FixLayerArray.size())
				continue;

			pEnt->setLayer(L"平面图-底图");
			if (m_HDrawPara.IsBaseMapGray)
				pEnt->setColorIndex(8);

			pEnt->close();

		}
		ads_relrb(pointlist);
		ads_ssfree(ssname);
	}
	acDocManager->unlockDocument(acDocManager->curDocument());

}
//20200323梁柱，视口缩放函数
void GetCurrentView(AcDbViewTableRecord &view)
{
	//AcDbViewTableRecord view;
	struct resbuf rb;
	struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系统标记
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	// 获得当前视口的“查看”模式
	acedGetVar(L"VIEWMODE", &rb);
	view.setPerspectiveEnabled(rb.resval.rint & 1);//透视
	view.setFrontClipEnabled(rb.resval.rint & 2);//剪切
	view.setBackClipEnabled(rb.resval.rint & 4);//回剪切
	view.setFrontClipAtEye(!(rb.resval.rint & 16));//前剪切

												   // 当前视口中视图的中心点（UCS坐标）
	acedGetVar(L"VIEWCTR", &rb);
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	view.setCenterPoint(AcGePoint2d(rb.resval.rpoint[X], rb.resval.rpoint[Y]));

	// 当前视口透视图中的镜头焦距长度（单位为毫米）
	acedGetVar(L"LENSLENGTH", &rb);
	view.setLensLength(rb.resval.rreal);

	// 当前视口中目标点的位置（以 UCS 坐标表示）
	acedGetVar(L"TARGET", &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	view.setTarget(AcGePoint3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的观察方向（UCS）
	acedGetVar(L"VIEWDIR", &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	view.setViewDirection(AcGeVector3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的视图高度（图形单位）
	acedGetVar(L"VIEWSIZE", &rb);
	view.setHeight(rb.resval.rreal);
	double height = rb.resval.rreal;

	// 以像素为单位的当前视口的大小（X 和 Y 值）
	acedGetVar(L"SCREENSIZE", &rb);
	view.setWidth(rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height);

	// 当前视口的视图扭转角
	acedGetVar(L"VIEWTWIST", &rb);
	view.setViewTwist(rb.resval.rreal);

	// 将模型选项卡或最后一个布局选项卡置为当前
	acedGetVar(L"TILEMODE", &rb);
	int tileMode = rb.resval.rint;

	// 设置当前视口的标识码（modelSpace视图还是paperSpace视图）
	acedGetVar(L"CVPORT", &rb);
	int cvport = rb.resval.rint;

	// 是否是模型空间的视图
	bool paperspace = ((tileMode == 0) && (cvport == 1)) ? true : false;
	view.setIsPaperspaceView(paperspace);
	if (!paperspace)
	{
		// 当前视口中前向剪裁平面到目标平面的偏移量
		acedGetVar(L"FRONTZ", &rb);
		view.setFrontClipDistance(rb.resval.rreal);

		// 获得当前视口后向剪裁平面到目标平面的偏移值
		acedGetVar(L"BACKZ", &rb);
		view.setBackClipDistance(rb.resval.rreal);
	}
	else
	{
		view.setFrontClipDistance(0.0);
		view.setBackClipDistance(0.0);
	}
	return;
}
void ZoomWindow(AcGePoint3d PT1, AcGePoint3d PT2)
{
	// 提示用户选择定义缩放窗口的两个角点
	ads_point pt1, pt2;
	
	pt1[X] = PT1.x;
	pt1[Y] = PT1.y;
	pt1[Z] = PT1.z;
	pt2[X] = PT2.x;
	pt2[Y] = PT2.y;
	pt2[Z] = PT2.z;
	//pt1 = asDblArray(PT1);
	// 转换坐标时使用的坐标系统标记 
	struct resbuf wcs, ucs, dcs;
	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	acedTrans(pt1, &ucs, &dcs, 0, pt1);
	acedTrans(pt2, &ucs, &dcs, 0, pt2);
	AcDbViewTableRecord view;
	GetCurrentView(view);

	// 设置视图的中心点
	view.setCenterPoint(AcGePoint2d((pt1[X] + pt2[X]) / 2, (pt1[Y] + pt2[Y]) / 2));

	// 设置视图的高度和宽度
	view.setHeight(fabs(pt1[Y] - pt2[Y]));
	view.setWidth(fabs(pt1[X] - pt2[X]));

	// 将视图对象设置为当前视图
	acedSetCurrentView(&view, NULL);
}
void ZoomEachWindow(STuKuang* TK)
{
	AcGePoint3d FnameLtUp, FnameRtBm, CliLtUp, CliRtBm;
	FnameLtUp.set(TK->LtUpX, TK->LtUpY,0);
	FnameRtBm.set(TK->RtBmX, TK->RtBmY,0);
	
	CRect rc;
	CView* pCurView = acedGetAcadDwgView();
	pCurView->GetClientRect(&rc);
	CPoint ptTopLeft = rc.TopLeft();
	CPoint ptBottomRight = rc.BottomRight();
	acedDwgPoint pt1, pt2;
	acedCoordFromPixelToWorld(ptTopLeft, pt1);
	acedCoordFromPixelToWorld(ptBottomRight, pt2);
	CliLtUp.set(pt1[X], pt1[Y], pt1[Z]);
	CliRtBm.set(pt2[X], pt2[Y], pt2[Z]);
	if ((CliLtUp.x<FnameLtUp.x)&& (CliLtUp.y > FnameLtUp.y)&&(CliRtBm.x>FnameRtBm.x)&& (CliRtBm.y < FnameRtBm.y))
	{
		ads_printf(L"\n窗口区域正常，继续出图！");
	}
	else
	{
		/*if (AfxMessageBox(L"当前出图方案超出视口范围，可能导致切图失败，是否缩放进行处理?")==IDOK)
		{*/
			ZoomWindow(FnameLtUp,FnameRtBm);
			ads_printf(L"\n当前出图方案超出视口范围，可能导致切图失败，自动进行缩放处理！");
		//}
	}

}
//计算平面图幅
void RoadAutoDesign::CalHoriDrawingNum()
{
	CString StartLc, EndLc;
	StartLc.Format(L"%.3lf", m_HDrawPara.QD); //起点里程
	EndLc.Format(L"%.3lf", m_HDrawPara.ZD); //终点里程
	double sml, eml;
	sml = m_pRoad->TYLC(StartLc);	//起始里程
	eml = m_pRoad->TYLC(EndLc);	//终止里程

	//设置图框长宽参数	20190902
	double HFrameLen = 0.0, HFrameHeight = 0.0;
	HFrameLen = m_HDrawPara.FrameLength - 20.0;
	if (m_HDrawPara.DrawMode == 0 && m_HDrawPara.BottomSpace != 0)
	{
		HFrameHeight = m_HDrawPara.FrameHeight - 10.0;	//这是为了留一点空隙，如果到时候不需要刻意删除
		HFrameHeight = HFrameHeight - m_HDrawPara.BottomSpace - m_HDrawPara.TopSpace;	//计算留白
	}
	else
		HFrameHeight = m_HDrawPara.FrameHeight - 20.0;	//这是为了留一点空隙，如果到时候不需要刻意删除

	//计算每幅图的图框中心
	int TukuangNum;
	TukuangNum = int(fabs(eml - sml) / m_HDrawPara.OneMapRoadLen + 50);
	STuKuang* TK = new STuKuang[TukuangNum];
	int TkNum;
	m_pRoad->CalTK(_wtof(StartLc), _wtof(EndLc), m_HDrawPara.OneMapRoadLen, HFrameHeight, HFrameLen, TK, TkNum, m_HDrawPara.MoveCenterX, m_HDrawPara.MoveCenterY);
	//20200323梁柱，判断方案书否处于当前视口，进行缩放//范围问题
	if (!isAllinView)
	{
		ZoomEachWindow(TK);
	}
	//ZoomEachWindow(TK);
	

	if (m_OvalDrawPara.IsDrawHoriOvalDrawing)
		m_DrawingNum[0] = TkNum;
	if (m_OvalDrawPara.IsDrawHoriRoad)
		m_DrawingNum[1] = TkNum;
	if (m_OvalDrawPara.IsDrawHoriEarth)
		m_DrawingNum[2] = TkNum;

	if (TK)
		delete[]TK;
}

/*===================================================================
函数名:DrawHEarth
------------------------------------------------------------
函数功能:绘平面用地图
------------------------------------------------------------
输入:	CString		FilePath		参数文件的路径
------------------------------------------------------------
输出:	bool		返回值			true-写入成功	false-写入失败
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190812
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawHEarth()
{
	if (!m_OvalDrawPara.IsDrawHoriEarth)
		return;

	m_HDrawPara.DrawMode = 2;
	_stprintf(m_HDrawPara.FrameBlockName, L"图框-%s", Pro);
	DrawCenterLine();
	//DrawHorizontalOutput();
	DrawHorizontalOutput_Trimflag_D();
}

/*===================================================================
函数名:DrawProfile
------------------------------------------------------------
函数功能:绘纵断面图
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190813
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawProfile()
{
	//计算起始位置
	m_CurX = m_OriX + (m_DrawingNum[0] + m_DrawingNum[1] + m_DrawingNum[2]) * 1000;

	//检测是否绘制
	if (!m_OvalDrawPara.IsDrawProfile)
		return;

	CString XZ;
	double Scml, Ecml, xscale, yscale, xinori, yinori, cml;
	int i, drawmode;

	CZDMHT zdmdraw;
	if (DrawZdm.GetSize() > 0)
		DrawZdm.RemoveAll();

	NZdmT = 0;
	int NZCG, NYCG, NZJK, NYJK, NZLMK, NYLMK, NBXDMX = 0;
	CgSZ* pZcg = NULL, * pYcg = NULL, * pZjk = NULL, * pYjk = NULL;
	LMKdata* pZlmk = NULL, * pYlmk = NULL;
	XLDataBase* pdbs = NULL, * pzdbs = NULL, * pydbs = NULL;
	XLDataBase* pBxDBS[10];//最多10个边线地面线
	pdbs = new  XLDataBase;
	pzLinep = NULL;
	pzLinep = m_pRoad;
	if (pzLinep == NULL)
	{
		return;
	}

	pdbs->Read_XLDbs(pzLinep->mdbname, L"纵断链表", pzLinep->RoadName);

	//读取坡度表数据
	if (m_VDrawPara.ZPInf == 1)//全幅
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"全幅");
	else if (m_VDrawPara.ZPInf == 2)//左幅
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"左幅");
	else if (m_VDrawPara.ZPInf == 3)//右幅
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"右幅");
	else//全部
	{
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"全幅");
		pdbs->ReadAllXZ(pzLinep->mdbname, pzLinep->RoadName);
		if (pdbs->RoadZ == "左幅")
		{
			pzdbs = new  XLDataBase;
			pzdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"左幅");
		}
		if (pdbs->RoadY == "右幅")
		{
			pydbs = new  XLDataBase;
			pydbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"右幅");
		}
	}
	BAS_DRAW_FUN ob;
	if (pdbs)
	{
		for (i = 0; i < pdbs->NPD; i++)
			pdbs->PDB[i].cml = ob.TYLC1(pdbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}
	if (pzdbs)
	{
		for (i = 0; i < pzdbs->NPD; i++)
			pzdbs->PDB[i].cml = ob.TYLC1(pzdbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}
	if (pydbs)
	{
		for (i = 0; i < pydbs->NPD; i++)
			pydbs->PDB[i].cml = ob.TYLC1(pydbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}

	//读取地面线数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"纵地面线表", pzLinep->RoadName);

	//读取桥隧数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"大中桥", pzLinep->RoadName/*,XZ*/);
	pdbs->Read_XLDbs(pzLinep->mdbname, L"隧道", pzLinep->RoadName/*,XZ*/);
	pdbs->Read_XLDbs(pzLinep->mdbname, L"小桥涵", pzLinep->RoadName);

	//读取超高数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"左超高分段表", pzLinep->RoadName);
	NZCG = pdbs->NCG;
	pZcg = new CgSZ[NZCG];
	for (i = 0; i < NZCG; i++)
	{
		pZcg[i] = pdbs->CG[i];
		pZcg[i].cml = pzLinep->TYLC(pZcg[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname, L"右超高分段表", pzLinep->RoadName);
	NYCG = pdbs->NCG;
	pYcg = new CgSZ[NYCG];
	for (i = 0; i < NYCG; i++)
	{
		pYcg[i] = pdbs->CG[i];
		pYcg[i].cml = pzLinep->TYLC(pYcg[i].xlc);
	}

	//读取超高数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"左加宽分段表", pzLinep->RoadName);
	NZJK = pdbs->NCG;
	pZjk = new CgSZ[NZJK];
	for (i = 0; i < NZJK; i++)
	{
		pZjk[i] = pdbs->CG[i];
		pZjk[i].cml = pzLinep->TYLC(pZjk[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname, L"右加宽分段表", pzLinep->RoadName);
	NYJK = pdbs->NCG;
	pYjk = new CgSZ[NYJK];
	for (i = 0; i < NYJK; i++)
	{
		pYjk[i] = pdbs->CG[i];
		pYjk[i].cml = pzLinep->TYLC(pYjk[i].xlc);
	}

	//读取路面宽数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"左路面宽分段表", pzLinep->RoadName);
	NZLMK = pdbs->NLMK;
	pZlmk = new LMKdata[NZLMK];
	for (i = 0; i < NZLMK; i++)
	{
		pZlmk[i] = pdbs->lmk[i];
		pZlmk[i].cml = pzLinep->TYLC(pZlmk[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname, L"右路面宽分段表", pzLinep->RoadName);
	NYLMK = pdbs->NLMK;
	pYlmk = new LMKdata[NYLMK];
	for (i = 0; i < NYLMK; i++)
	{
		pYlmk[i] = pdbs->lmk[i];
		pYlmk[i].cml = pzLinep->TYLC(pYlmk[i].xlc);
	}

	if (pdbs->NPD == 0)
		return;

	//设置绘图参数
	xscale = m_VDrawPara.Hscale / 1000;		//x轴比例尺
	yscale = m_VDrawPara.Zscale / 1000;		//y轴比例尺
	xinori = m_CurX;				//绘图原点X
	yinori = m_CurY;				//绘图原点Y
	drawmode = m_VDrawPara.DrawMode;				//绘图模式
	Scml = m_pRoad->TYLC(m_VDrawPara.StDml);	//起点里程
	Ecml = m_pRoad->TYLC(m_VDrawPara.EdDml);	//终点里程
	//添加起终点
	/*if (!if_PL_CT)
	{
		Scml = CTsml;
		Ecml = CTeml;
	}*/
	pzLinep->FormLCB(100, Scml, Ecml);	//按100m间距生成里程表
	i = 0;
	int CurTno, j;
	CurTno = m_VDrawPara.SPage;
	_stprintf(m_VDrawPara.FrameBlockName, L"图框-%s", Pro);

	while (Scml < Ecml - 0.01)
	{
		//传递绘图数据
		zdmdraw.m_pm = m_pRoad; //平面指针
		zdmdraw.CurTNo = CurTno + i;//页码
		zdmdraw.m_TKName = m_FramePath; //图框文件路径
		zdmdraw.m_dzgkname = m_VDrawPara.dzgkname;//地质概况文件路径
		zdmdraw.m_bzym = m_VDrawPara.bzym;//是否标注页码
		zdmdraw.pDBS = pdbs;
		zdmdraw.pZDBS = pzdbs;
		zdmdraw.pYDBS = pydbs;
		zdmdraw.NZCG = NZCG;
		zdmdraw.NYCG = NYCG;
		zdmdraw.ZCG = pZcg;
		zdmdraw.YCG = pYcg;
		zdmdraw.NZJK = NZJK;
		zdmdraw.NYJK = NYJK;
		zdmdraw.ZJK = pZjk;
		zdmdraw.YJK = pYjk;
		zdmdraw.NZLMK = NZLMK;
		zdmdraw.NYLMK = NYLMK;
		zdmdraw.ZLMK = pZlmk;
		zdmdraw.YLMK = pYlmk;
		zdmdraw.X_scale = xscale;
		zdmdraw.Y_scale = yscale;
		zdmdraw.X_inori = xinori + i * 1000; //内框左下角
		zdmdraw.Y_inori = yinori;
		zdmdraw.DrawMode = drawmode;//0 初步 1施工
		zdmdraw.infwgx = m_VDrawPara.infwgx;//是否绘制网格线
		zdmdraw.isdrawctrh = m_VDrawPara.ifDrawCtrH;//是否绘制网格线
		zdmdraw.m_Scml = Scml; //该幅纵面图起终连续里程
		zdmdraw.NBxDmx = NBXDMX;

		m_CurX += 1000.0;
		//传递边界地面线数据
		for (j = 0; j < NBXDMX; j++)
			zdmdraw.BxDBS[j] = pBxDBS[j];
		//根据图幅设定传递相应参数
		if (drawmode)//施工
		{
			if (i == 0)//第一幅
			{
				if (m_VDrawPara.FForZF == 0)//分幅输出
				{
					//  cml = Scml+xscale*345;
					cml = Scml + xscale * 700;
					if (cml > Ecml)
						cml = Ecml;
				}
				else//整幅输出
				{
					cml = Ecml;
					//zdmdraw.NTKHt = (pdbs->Hmax-pdbs->Hmin)/yscale;
					//zdmdraw.WTKHt = zdmdraw.NTKHt+20;
					//ads_printf(L"yscale=%lf %lf %lf hmax=%lf hmin=%lf\n",yscale,zdmdraw.NTKHt,zdmdraw.WTKHt,pdbs->Hmax,pdbs->Hmin);
					//
				}
				zdmdraw.m_Ecml = cml;
				zdmdraw.IsFirst = true;
			}
			else
			{
				//  cml = Scml+xscale*370;
				cml = Scml + xscale * 700;
				if (cml > Ecml)
					cml = Ecml;
				zdmdraw.m_Ecml = cml;
				zdmdraw.IsFirst = false;
			}
		}
		else//初步设计
		{
			if (m_VDrawPara.FForZF == 0)//分幅输出
			{
				cml = Scml + xscale * 700;
				if (cml > Ecml)
					cml = Ecml;
			}
			else//整幅输出
			{
				cml = Ecml;
				//zdmdraw.NTKHt = (pdbs->Hmax-pdbs->Hmin)/yscale;
				//zdmdraw.WTKHt = zdmdraw.NTKHt+20;

			}

			zdmdraw.m_Ecml = cml;
		}

		SetWxpPara(zdmdraw);
		
		zdmdraw.NTKLen = m_Frame_Width;
		zdmdraw.NTKHt = m_Frame_Height;
		m_VDrawPara.Is_buju_ZDM = m_TrimFlag;
		zdmdraw.m_VDrawPara = m_VDrawPara;

		zdmdraw.DrawZDM(m_VDrawPara.YbreakTextH, m_VDrawPara.DefTexhH);
		 
		Scml = zdmdraw.m_Ecml;
		DrawZdm.Add(zdmdraw);
		i++;
	}

	NZdmT = i;

	if (pZcg) delete[]pZcg;
	if (pYcg) delete[]pYcg;

	int Sum = 0;
	for (int i = 0; i < m_DrawingNum.size(); i++)
		Sum += m_DrawingNum[i];
	int DrawingNum = (int)((m_CurX - m_OriX) / 1000.0 + 0.1) - Sum;
	m_DrawingNum[3] = DrawingNum;
}

/*===================================================================
函数名SetProfileDrawPara
------------------------------------------------------------
函数功能:设置纵断面绘图参数
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190813
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetProfileDrawPara()
{
	m_VDrawPara.DrawMode = 1;
	m_VDrawPara.FForZF = 0;
	m_VDrawPara.ZPInf = 0;
	m_VDrawPara.Hscale = 1000.0;
	m_VDrawPara.Zscale = 100.0;
	m_VDrawPara.StDml = m_pRoad->DLArray[0].ELC;
	m_VDrawPara.EdDml = m_pRoad->DLArray[m_pRoad->DLNum - 1].BLC;
	m_VDrawPara.SPage = 0;
	m_VDrawPara.bzym = false;
	m_VDrawPara.infwgx = true;
	m_VDrawPara.ifDrawCtrH = true;
	m_VDrawPara.dzgkname = L"";	//暂时先不考虑地质描述文件
}

/*===================================================================
函数名:SetWxpPara
------------------------------------------------------------
函数功能:设置五线谱参数
------------------------------------------------------------
输入:	
------------------------------------------------------------
输出:	CZDMHT&		zdmdraw		纵断面绘图类
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190813
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::SetWxpPara(CZDMHT& zdmdraw)
{
	zdmdraw.WayorRamp = 0;	//公路	暂时不考虑匝道
	zdmdraw.InfLmPt[0] = false;
	zdmdraw.InfLmPt[1] = false;
	zdmdraw.InfLmPt[2] = false;
	zdmdraw.InfLmPt[3] = false;
	zdmdraw.InfLmPt[4] = false;
	zdmdraw.InfLmPt[5] = false;
	zdmdraw.InfLmPt[6] = false;
	zdmdraw.InfLmPt[7] = false;
	zdmdraw.LmPtName[0] = _T("P1");
	zdmdraw.LmPtName[1] = _T("P2");
	zdmdraw.LmPtName[2] = _T("P3");
	zdmdraw.LmPtName[3] = _T("P4");
	zdmdraw.LmPtName[4] = _T("P5");
	zdmdraw.LmPtName[5] = _T("P6");
	zdmdraw.LmPtName[6] = _T("P7");
	zdmdraw.LmPtName[7] = _T("P8");

	//传递五线谱参数（倒序传入）
	zdmdraw.WXPnum = m_VDrawPara.WXPArray.size();
	if (!m_VDrawPara.WXPArray.size())
		return;

	int Count = 0;
	for (int i = m_VDrawPara.WXPArray.size() - 1; i >= 0; i--)
	{
		zdmdraw.wxp[Count] = m_VDrawPara.WXPArray[i];
		Count++;
	}
}

/*===================================================================
函数名:DrawCross
------------------------------------------------------------
函数功能:绘横断面图
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190813
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawCross()
{
	if (!m_OvalDrawPara.IsDrawCross)
		return;

	HdmDes Cross;

	Cross.CreateLayer(L"清表土");
	//读文件		
	if (!Cross.ReadFile(false))//横断面标准
		return;

	Cross.ReadBDFile();

	//从鼻端文件中读取左右关联道路，若道路总数>0按互通处理，否则按分离式处理
	CString xl1;
	xl1.Format(L"%s,%s", m_MdbName, m_RoadName);

	//	SetHdmLm();//求路面

	Cross.FindAndReadLinkRoad_for3d(xl1);

	ads_printf(L"读数据库OK!\n");

	//横断面成果
	for (int i = 0; i < Cross.RoadSum; i++)
	{

		if (!Cross.ReadHdmResultFile(i))
			return;

		if (!Cross.ReadHdmAreaFile(i))
			return;
	}

	int iroad;

	CString CKSml = Cross.road[0].hdmdes[0].ckml;
	CString CKEml = Cross.road[0].hdmdes[Cross.road[0].NCRS - 1].ckml;

	iroad = Cross.GetIRoadByName(m_RoadName);
	double sml, eml;
	sml = Cross.road[iroad].pm->TYLC(CKSml);
	eml = Cross.road[iroad].pm->TYLC(CKEml);
	//添加起终点
	/*if (!if_PL_CT)
	{
		sml = CTsml;
		eml = CTeml;
	}*/
	int lnum;//列数
	lnum = 2;	//暂时设置为2列
	double zk, yk;
	zk = m_CDrawPara.EarthWidL;
	yk = m_CDrawPara.EarthWidR;
	Cross.StartX = m_CurX;
	Cross.StartY = m_CurY;   // 获取绘制起点
	CString TKname = m_FramePath;
	Cross.CurRoad = iroad;
	int spage;
	spage = 0;
	Cross.Ifpage = false;
	Cross.m_Isbuju_HDm = m_TrimFlag;//布局
	Sfactor = 1000.0 / m_CDrawPara.Scale;
	Cross.m_FrameHeight = m_Frame_Height;
	Cross.m_FrameWid = m_Frame_Width;
	Cross.m_IsDrawCrossEarthPile = m_CDrawPara.IsDrawEarthPile;
	Cross.m_IsDrawHengDuanMianWangGe = m_CDrawPara.IsDrawGrid;
	_stprintf(Cross.m_FrameBlockName, L"图框-%s", Pro);
	_stprintf(Cross.m_FrameBlockName, L"水沟-%s", Pro);

	Cross.DrawHdmMap(iroad, sml, eml, zk, yk, TKname, lnum, spage, Cross.Ifpage, false, m_CDrawPara.TabTextH, m_CDrawPara.BZTextH);

	m_CurX += Cross.MAPSUM * 1000.0;

	BAS_DRAW_FUN::SetLayerColor(L"图框", 7);	//20190805

	int Sum = 0;
	for (int i = 0; i < m_DrawingNum.size(); i++)
		Sum += m_DrawingNum[i];
	int DrawingNum = (int)((m_CurX - m_OriX) / 1000.0 + 0.1) - Sum;
	m_DrawingNum[4] = DrawingNum;
}

/*===================================================================
函数名:DrawCurveInfor
------------------------------------------------------------
函数功能:绘曲线要素表
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190813
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawCurveInfor()
{
	if (!m_OvalDrawPara.IsDrawCurveInfo)
		return;

	//插入图框
	ACHAR dwgPath[256], FrameBlockName[256];
	_tcscpy(dwgPath, m_FramePath);
	_stprintf(FrameBlockName, L"曲线要素表-%s", Pro);
	AcDbObjectId pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, FrameBlockName);
	AcGePoint3d InsertPt(m_CurX + m_Frame_Width / 2.0, m_CurY + m_Frame_Height / 2.0, 0.0);
	if (pBlockId > 0)
	{
		AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setColorIndex(7);
		BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
	}

	//绘制曲线要素
	double d1, d2, d3, h1, h2;
	struct resbuf result;
	int type;
	ads_point pt, ptold, tmpt, LDpt, LUpt, RDpt, RUpt, LDptold, LUptold, RDptold, RUptold;
	double Wtab = 0.0, Htab = 0.0;

	double DFACTOR = 2000.0 / 1000;
	int SIP = m_pRoad->JdCenArray[0].JdNo;
	int EIP = m_pRoad->JdCenArray[m_pRoad->JdCenArray.GetSize() - 1].JdNo;
	int JDn = EIP - SIP + 1;

	d1 = 20 * DFACTOR;
	d2 = 15 * DFACTOR;
	d3 = 10 * DFACTOR;
	h1 = 8 * DFACTOR, h2 = 5 * DFACTOR;
	int DrawMode = 1;
	if (DrawMode == 0)
	{
		Wtab = d3 + 3 * d1 + 7 * d2;
		Htab = 2 * h1 + JDn * h2;
	}
	else
	{
		Wtab = d3 + 10 * d1 + 7 * d2;
		Htab = 2 * h1 + JDn * h2;
	}

	ads_point Ppt;
	double INPTX = m_CurX + 75.0;
	double INPTY = m_CurY + 400.0;
	Ppt[X] = INPTX, Ppt[Y] = INPTY, Ppt[Z] = 0.0;
	double ALPHA = 0.0;
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	del_ent(Wtab, Htab, ALPHA, Ppt, DFACTOR);
	//生成直曲转角表	
	m_pRoad->CreateZQZJB();

	//再标曲线表;
	if (DrawMode == 0)
		wr_cmf_c(ALPHA, Ppt, DFACTOR, Wtab, Htab, JDn, SIP);
	else
		wr_cmf_s(ALPHA, Ppt, DFACTOR, Wtab, Htab, JDn, SIP);

	acDocManager->unlockDocument(acDocManager->curDocument());

	if (m_pRoad->ZQZJB)
	{
		delete[]m_pRoad->ZQZJB;
		m_pRoad->ZQZJB = NULL;
	}

	//布局出图
	double CenX = m_CurX + m_Frame_Width / 2.0;
	double CentY = m_CurY + m_Frame_Height / 2.0;
	CString StartLC, EndLC;
	StartLC = pzLinep->XLC(m_HDrawPara.QD);
	EndLC = pzLinep->XLC(m_HDrawPara.ZD);
	if (m_TrimFlag == 1)
		DrawCurInforOutput(CenX,CentY,StartLC,EndLC,m_FramePath);

	m_CurX += 1000.0;

	int Sum = 0;
	for (int i = 0; i < m_DrawingNum.size(); i++)
		Sum += m_DrawingNum[i];
	int DrawingNum = (int)((m_CurX - m_OriX) / 1000.0 + 0.1) - Sum;
	m_DrawingNum[5] = DrawingNum;
}

int RoadAutoDesign::del_ent(double Wtab, double Htab, double ALPHA, ads_point Ppt, double DFACTOR)
{
	double ang, h1;
	ads_point tmpt, pr1, pr2, pl1, pl2, pl3, LDpt, LUpt, RDpt, RUpt;
	h1 = Wtab;
	ang = ALPHA * PI / 180;
	pl1[X] = Ppt[X], pl1[Y] = Ppt[Y], pl1[Z] = Ppt[Z];

	ptAddx(pl1, ang, Wtab, pr1);
	ptAddy(pr1, ang, -Htab, pr2);
	ptAddy(pl1, ang, -Htab, pl2);
	ptAddy(pl1, ang, -h1, pl3);
	//acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pl1,RTREAL,1.0,RTREAL,0.0,RTSTR,L"pl1",RTNONE);
	//pl1,pl2,pr1,pr2四点
	//切断
	ads_polar(pl1, ang, -3.0 * DFACTOR, tmpt);
	ads_polar(tmpt, ang + 0.5 * PI, 9.0 * DFACTOR, LUpt);
	ptAddx(LUpt, ang, Wtab + 6.0 * DFACTOR, RUpt);
	ptAddy(RUpt, ang, -Htab - 10.0 * DFACTOR, RDpt);
	ptAddy(LUpt, ang, -Htab - 10.0 * DFACTOR, LDpt);

	BAS_DRAW_FUN::AutoEntBK(LUpt, RUpt, RDpt, LDpt);
	/*
	AutoEntBK1(LUpt,RUpt);
	AutoEntBK1(RUpt,RDpt);
	AutoEntBK1(RDpt,LDpt);
	AutoEntBK1(LDpt,LUpt);*/

	//筐选
	ads_polar(pl1, ang, -4.0 * DFACTOR, tmpt);
	ads_polar(tmpt, ang + 0.5 * PI, 10.0 * DFACTOR, LUpt);
	ptAddx(LUpt, ang, Wtab + 8.0 * DFACTOR, RUpt);
	ptAddy(RUpt, ang, -Htab - 12.0 * DFACTOR, RDpt);
	ptAddy(LUpt, ang, -Htab - 12.0 * DFACTOR, LDpt);

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, LDpt, RTPOINT, LUpt, RTPOINT, RUpt, RTPOINT, RDpt, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, LDpt, RTPOINT, LUpt, RTPOINT, RUpt, RTPOINT, RDpt, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	

	return RTNORM;
}
void RoadAutoDesign::ptAddx(ads_point p1, double angx, double x, ads_point pt)
{
	ads_polar(p1, angx, x, pt);
}
void RoadAutoDesign::ptAddy(ads_point p1, double angx, double y, ads_point pt)
{
	ads_polar(p1, angx + 0.5 * PI, y, pt);
}
int RoadAutoDesign::wr_cmf_c(double ALPHA, ads_point Ppt, double DFACTOR, double Wtab, double Htab, int JDn, int SIP)
{
	double ang, d1, d2, d3, h1, h2, lw, d, ht, di, angd, hh, wk;
	ads_point p1, p2, tmpt, pr1, pr2, pl1, pl2, pl3, pt, pj, p0, Mpt;
	int i, Dip, du;
	ACHAR str[20], ZjStr[30];
	ACHAR mm[4], ss[4];
	ads_point pt1, pt2;

	ang = ALPHA * PI / 180;
	pl1[X] = Ppt[X], pl1[Y] = Ppt[Y], pl1[Z] = Ppt[Z];
	d1 = 20 * DFACTOR;
	d2 = 15 * DFACTOR;
	d3 = 10 * DFACTOR;
	h1 = 8 * DFACTOR, h2 = 5 * DFACTOR;
	Wtab = d3 + 3 * d1 + 7 * d2;
	Htab = 2 * h1 + JDn * h2;
	lw = 0.5 * DFACTOR;
	ptAddx(pl1, ang, Wtab, pr1);
	ptAddy(pr1, ang, -Htab, pr2);
	ptAddy(pl1, ang, -Htab, pl2);
	ptAddy(pl1, ang, -h1, pl3);

	AcGePoint2dArray Pt2dAy;

	Pt2dAy.append(AcGePoint2d(pl1[X], pl1[Y]));
	Pt2dAy.append(AcGePoint2d(pr1[X], pr1[Y]));
	Pt2dAy.append(AcGePoint2d(pr2[X], pr2[Y]));
	Pt2dAy.append(AcGePoint2d(pl2[X], pl2[Y]));
	Pt2dAy.append(AcGePoint2d(pl1[X], pl1[Y]));
	BAS_DRAW_FUN::makepolyline(Pt2dAy, 2, lw, L"曲线要素");
	//   acedCommandS(RTSTR,L"Pline",RTPOINT,pl1,RTSTR,L"W",RTREAL,lw,RTREAL,lw,RTPOINT,pr1,RTPOINT,pr2,RTPOINT,pl2,RTSTR,L"C",RTNONE);

	lw = 0.0;
	ptAddx(pl1, ang, d3 + d1, p1);
	ptAddx(pl1, ang, d3 + 3 * d1, p2);

	LineUD(p2, p1, ang, -h1, 0);
	LineUD(pl1, pr1, ang, -2.0 * h1, 0);

	LineLR(pl1, pl2, ang, d3, 0);
	LineLR(pl1, pl2, ang, d3 + d1, 0);
	LineLR(pl1, pl2, ang, d3 + 3 * d1, 0);
	LineLR(pl3, pl2, ang, d3 + 2 * d1, 0);

	i = 1;
	d = d3 + 3 * d1;
	while (i < 8)
	{
		di = d + i * d2;
		LineLR(pl1, pl2, ang, di, 0);
		i++;
	}

	i = 1;
	d = 2 * h1;
	while (i < JDn)
	{
		di = d + i * h2;
		LineUD(pl1, pr1, ang, -di, 0);
		i++;
	}

	angd = ang;
	ht = 4.0 * DFACTOR;
	wk = 0.5 * (h2 - ht);
	ptAddx(pl1, ang, 0.5 * Wtab, Mpt);
	ptAddy(Mpt, ang, 8 * DFACTOR, Mpt);
	wr_text(pj, ang, 0.0, wk, ht, str);
	wr_text(Mpt, angd, 0.0, wk, ht, L"平  曲  线  要  素  表");
	//    acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,Mpt,RTREAL,ht,RTREAL,angd,RTSTR,L"平  曲  线  要  素  表",RTNONE);
	//	acedCommandS(RTSTR,L"Color",RTSHORT,7,RTNONE);
	ptAddx(pl1, ang, 0.5 * Wtab, Mpt);
	ptAddy(Mpt, ang, 2 * DFACTOR, Mpt);
	ptAddx(Mpt, ang, -47.5 * 0.5 * DFACTOR, pt1);
	ptAddx(Mpt, ang, 47.5 * 0.5 * DFACTOR, pt2);
	LineLR(pt1, pt2, 0, 0, 0.6, 7);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,0.6,RTREAL,0.6,RTPOINT,pt2,RTSTR,L"",RTNONE);
	ptAddy(pt1, ang, 1.5 * DFACTOR, pt1);
	ptAddy(pt2, ang, 1.5 * DFACTOR, pt2);
	LineLR(pt1, pt2, 0, 0, 0.0, 7);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,0.0,RTREAL,0.0,RTPOINT,pt2,RTSTR,L"",RTNONE);
	//	acedCommandS(RTSTR,L"Color",RTSHORT,2,RTNONE);

	MidPoint(pl1, d3, -2.0 * h1, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"JD");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"JD",RTNONE);

	ptAddx(pl1, ang, d3, tmpt);
	MidPoint(tmpt, d1, -2.0 * h1, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"角桩桩号");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"角桩桩号",RTNONE);

	ptAddx(pl1, ang, d3 + d1, tmpt);
	MidPoint(tmpt, 2.0 * d1, -h1, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"转   角");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"转   角",RTNONE);

	ptAddx(pl3, ang, d3 + d1, tmpt);
	MidPoint(tmpt, d1, -h1, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"左");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"左",RTNONE);

	ptAddx(pl3, ang, d3 + 2.0 * d1, tmpt);
	MidPoint(tmpt, d1, -h1, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"右");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"右",RTNONE);


	ptAddx(pl1, ang, d3 + 3 * d1, p1);
	hh = h1 + h1;
	MidPoint(p1, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"R");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"R",RTNONE);

	ptAddx(p1, ang, d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"Ls1");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Ls1",RTNONE);

	ptAddx(p1, ang, 2 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"Ls2");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Ls2",RTNONE);

	ptAddx(p1, ang, 3 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"T1");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"T1",RTNONE);

	ptAddx(p1, ang, 4 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"T2");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"T2",RTNONE);

	ptAddx(p1, ang, 5 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"L");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"L",RTNONE);


	ptAddx(p1, ang, 6 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, 0, ht, L"E");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"E",RTNONE);
	//--------------------------------------------------------------------------------------------
	ptAddx(p1, ang, 7 * d2, p1);
	i = 0;
	ht = 2.0 * DFACTOR;
	wk = 0.5 * (h2 - ht);

	BAS_DRAW_FUN Bas_Fun;
	ptAddy(pl1, ang, -2.0 * h1, pl3);
	Dip = SIP - m_pRoad->JD0 + 1;
	while (i < JDn)
	{
		ptAddy(pl3, ang, -1.0 * i * h2, p0);
		MidPoint(p0, d3, -h2, ang, pj);
		//		_stprintf(str,L"%d",CmfTab[i+Dip].JdNo);
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].JDNO);
		wr_text(pj, ang, 0.0, wk, ht, str);

		//		wr_text(pj, ang,0.5*(d3+d1),wk,ht,CmfTab[i+Dip].jdml);
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].JDZH);
		wr_text(pj, ang, 0.5 * (d3 + d1), wk, ht, str);

		Bas_Fun.atodmss(Bas_Fun.dms_rad(fabs(m_pRoad->ZQZJB[i + Dip].AFA)), du, mm, ss);
		//125612
		_stprintf(ZjStr, L"%d°%s%s", du, mm, ss);
		if (m_pRoad->ZQZJB[i + Dip].AFA < 0.0)
			wr_text(pj, ang, 0.5 * d3 + 1.5 * d1, wk, ht, ZjStr);
		else
			wr_text(pj, ang, 0.5 * d3 + 2.5 * d1, wk, ht, ZjStr);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].R2, 2, 3, str);

		wr_text(pj, ang, 0.5 * d3 + 3.0 * d1 + 0.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].Ls1, 2, 3, str);

		wr_text(pj, ang, 0.5 * d3 + 3.0 * d1 + 1.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].Ls2, 2, 3, str);

		wr_text(pj, ang, 0.5 * d3 + 3.0 * d1 + 2.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].T1, 2, 3, str);

		wr_text(pj, ang, 0.5 * d3 + 3.0 * d1 + 3.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].T2, 2, 3, str);

		wr_text(pj, ang, 0.5 * d3 + 3.0 * d1 + 4.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].L, 2, 3, str);

		wr_text(pj, ang, 0.5 * d3 + 3.0 * d1 + 5.5 * d2, wk, ht, str);

		if (m_pRoad->ZQZJB[i + Dip].OutDist > 0.0)
		{
			ads_rtos(m_pRoad->ZQZJB[i + Dip].OutDist, 2, 3, str);
			wr_text(pj, ang, 0.5 * d3 + 3.0 * d1 + 6.5 * d2, wk, ht, str);
		}

		i++;
	}

	return RTNORM;
}
int RoadAutoDesign::wr_cmf_s(double ALPHA, ads_point Ppt, double DFACTOR, double Wtab, double Htab, int JDn, int SIP)
{
	double ang, d1, d2, d3, h1, h2, lw, d, ht, di, angd, hh, wk;
	ads_point p1, p2, tmpt, pr1, pr2, pl1, pl2, pl3, pt, pj, p0, Mpt;
	int i, Dip, du;
	ACHAR str[256], ZjStr[256];
	ACHAR mm[4], ss[4];
	ads_point pt1, pt2;

	//	acedCommandS(RTSTR,L"Color",RTSHORT,7,RTNONE);
	ang = ALPHA * PI / 180;
	pl1[X] = Ppt[X], pl1[Y] = Ppt[Y], pl1[Z] = Ppt[Z];
	d1 = 20 * DFACTOR;
	d2 = 15 * DFACTOR;
	d3 = 10 * DFACTOR;
	h1 = 8 * DFACTOR, h2 = 5 * DFACTOR;
	Wtab = d3 + 10 * d1 + 7 * d2;
	Htab = 2 * h1 + JDn * h2;
	lw = 0.5 * DFACTOR;
	ptAddx(pl1, ang, Wtab, pr1);
	ptAddy(pr1, ang, -Htab, pr2);
	ptAddy(pl1, ang, -Htab, pl2);
	ptAddy(pl1, ang, -h1, pl3);
	//	acedCommandS(RTSTR,L"color",RTSTR,L"Yellow",RTNONE);
	AcGePoint2dArray Pt2dAy;

	Pt2dAy.append(AcGePoint2d(pl1[X], pl1[Y]));
	Pt2dAy.append(AcGePoint2d(pr1[X], pr1[Y]));
	Pt2dAy.append(AcGePoint2d(pr2[X], pr2[Y]));
	Pt2dAy.append(AcGePoint2d(pl2[X], pl2[Y]));
	Pt2dAy.append(AcGePoint2d(pl1[X], pl1[Y]));
	BAS_DRAW_FUN::makepolyline(Pt2dAy, 2, lw, L"曲线要素");
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pl1,RTSTR,L"W",RTREAL,lw,RTREAL,lw,RTPOINT,pr1,RTPOINT,pr2,RTPOINT,pl2,RTSTR,L"C",RTNONE);

	lw = 0.0;
	ptAddx(pl1, ang, d3 + 5 * d1, p1);
	ptAddx(pl1, ang, d3 + 3 * d1, p2);
	LineUD(p2, p1, ang, -h1, 0);
	LineUD(pl1, pr1, ang, -2.0 * h1, 0);

	LineLR(pl1, pl2, ang, d3, 0);
	LineLR(pl1, pl2, ang, d3 + d1, 0);
	LineLR(pl1, pl2, ang, d3 + 2 * d1, 0);
	LineLR(pl1, pl2, ang, d3 + 3 * d1, 0);
	LineLR(pl3, pl2, ang, d3 + 4 * d1, 0);
	LineLR(pl1, pl2, ang, d3 + 5 * d1, 0);

	i = 1;
	d = d3 + 5 * d1;
	while (i < 8)
	{
		di = d + i * d2;
		LineLR(pl1, pl2, ang, di, 0);
		i++;
	}

	i = 1;
	d = d3 + 5 * d1 + 7 * d2;
	while (i < 5)
	{
		di = d + i * d1;
		LineLR(pl1, pl2, ang, di, 0);
		i++;
	}

	i = 1;
	d = 2 * h1;
	while (i < JDn)
	{
		di = d + i * h2;
		LineUD(pl1, pr1, ang, -di, 0);
		i++;
	}


	angd = ang;
	ht = 4.0 * DFACTOR;
	//    acedCommandS(RTSTR,L"-Style",RTSTR,L"hz",RTSTR,L"宋体",RTREAL,0.0,RTREAL,0.75,RTREAL,0.0,RTSTR,L"N",RTSTR,L"N",RTNONE);

	ptAddx(pl1, ang, 0.5 * Wtab, Mpt);
	ptAddy(Mpt, ang, 8 * DFACTOR, Mpt);

	wk = 0.0;	//20190716	暂时没用上
	wr_text(Mpt, angd, 0.0, wk, ht, L"平  曲  线  要  素  表");
	//    acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,Mpt,RTREAL,ht,RTREAL,angd,RTSTR,L"平  曲  线  要  素  表",RTNONE);
	//	acedCommandS(RTSTR,L"Color",RTSHORT,7,RTNONE);
	ptAddx(pl1, ang, 0.5 * Wtab, Mpt);
	ptAddy(Mpt, ang, 2 * DFACTOR, Mpt);
	ptAddx(Mpt, ang, -47.5 * 0.5 * DFACTOR, pt1);
	ptAddx(Mpt, ang, 47.5 * 0.5 * DFACTOR, pt2);
	LineLR(pt1, pt2, 0, 0, 0.6, 7);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,0.6,RTREAL,0.6,RTPOINT,pt2,RTSTR,L"",RTNONE);
	ptAddy(pt1, ang, 1.5 * DFACTOR, pt1);
	ptAddy(pt2, ang, 1.5 * DFACTOR, pt2);
	LineLR(pt1, pt2, 0, 0, 0.0, 7);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,0.0,RTREAL,0.0,RTPOINT,pt2,RTSTR,L"",RTNONE);
	//	acedCommandS(RTSTR,L"Color",RTSHORT,2,RTNONE);

	MidPoint(pl1, d3, -2.0 * h1, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"JD");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"JD",RTNONE);

	ptAddx(pl1, ang, d3, tmpt);
	MidPoint(tmpt, d1, -2.0 * h1, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"角桩桩号");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"角桩桩号",RTNONE);

	ptAddx(pl1, ang, d3 + d1, tmpt);
	MidPoint(tmpt, d1, -2.0 * h1, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"E(X)");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"X",RTNONE);

	ptAddx(pl1, ang, d3 + 2.0 * d1, tmpt);
	MidPoint(tmpt, d1, -2.0 * h1, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"N(Y)");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Y",RTNONE);

	ptAddx(pl1, ang, d3 + 3.0 * d1, tmpt);
	MidPoint(tmpt, 2.0 * d1, -h1, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"转   角");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"转   角",RTNONE);

	ptAddx(pl3, ang, d3 + 3.0 * d1, tmpt);
	MidPoint(tmpt, d1, -h1, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"左");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"左",RTNONE);

	ptAddx(pl3, ang, d3 + 4.0 * d1, tmpt);
	MidPoint(tmpt, d1, -h1, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"右");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"右",RTNONE);


	ptAddx(pl1, ang, d3 + 5 * d1, p1);
	hh = h1 + h1;
	MidPoint(p1, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"半径");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"R",RTNONE);

	ptAddx(p1, ang, d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"前缓长");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Ls1",RTNONE);

	ptAddx(p1, ang, 2 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"后缓长");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Ls2",RTNONE);

	ptAddx(p1, ang, 3 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	pt[Y] += (3) * DFACTOR;
	wr_text(pt, angd, 0.0, wk, ht, L"前切");
	pt[Y] -= (6) * DFACTOR;
	wr_text(pt, angd, 0.0, wk, ht, L"线长");
	//wr_text(pt, angd, 0.0, wk, ht, L"前切线长");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"T1",RTNONE);

	ptAddx(p1, ang, 4 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	pt[Y] += (3) * DFACTOR;
	wr_text(pt, angd, 0.0, wk, ht, L"后切");
	pt[Y] -= (6) * DFACTOR;
	wr_text(pt, angd, 0.0, wk, ht, L"线长");
	//wr_text(pt, angd, 0.0, wk, ht, L"后切线长");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"T2",RTNONE);

	ptAddx(p1, ang, 5 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"曲线长");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"L",RTNONE);

	ptAddx(p1, ang, 6 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"外矢距");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"E",RTNONE);
	//--------------------------------------------------------------------------------------------
	ptAddx(p1, ang, 7 * d2, tmpt);
	p1[X] = tmpt[X], p1[Y] = tmpt[Y];
	MidPoint(p1, d1, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"ZH");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"ZH",RTNONE);

	ptAddx(p1, ang, d1, tmpt);
	MidPoint(tmpt, d1, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"HY(ZY)");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"HY(ZY)",RTNONE);

	ptAddx(p1, ang, 2.0 * d1, tmpt);
	MidPoint(tmpt, d1, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"QZ");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"QZ",RTNONE);

	ptAddx(p1, ang, 3.0 * d1, tmpt);
	MidPoint(tmpt, d1, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"YH(YZ)");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"YH(YZ)",RTNONE);

	ptAddx(p1, ang, 4.0 * d1, tmpt);
	MidPoint(tmpt, d1, -hh, ang, pt);
	wr_text(pt, angd, 0.0, wk, ht, L"HZ");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR, "HZ",RTNONE);

	BAS_DRAW_FUN Bas_Fun;
	i = 0;
	ht = 2.0 * DFACTOR;
	wk = 0.5 * (h2 - ht);
	ptAddy(pl1, ang, -2.0 * h1, pl3);
	Dip = SIP - m_pRoad->JD0 + 1;
	while (i < JDn)
	{
		ptAddy(pl3, ang, -1.0 * i * h2, p0);
		MidPoint(p0, d3, -h2, ang, pj);
		//		_stprintf(str,L"%d",CmfTab[i+Dip].JdNo);
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].JDNO);
		wr_text(pj, ang, 0.0, wk, ht, str);
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].JDZH);
		wr_text(pj, ang, 0.5 * (d3 + d1), wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].JDN, 2, 3, str);
		wr_text(pj, ang, 0.5 * d3 + 1.5 * d1, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].JDE, 2, 3, str);
		wr_text(pj, ang, 0.5 * d3 + 2.5 * d1, wk, ht, str);


		Bas_Fun.atodmss(Bas_Fun.dms_rad(fabs(m_pRoad->ZQZJB[i + Dip].AFA)), du, mm, ss);
		//125612
		_stprintf(ZjStr, L"%d°%s%s", du, mm, ss);
		//	_stprintf(ZjStr,L"%d.%s%s",du,mm,ss);
		if (m_pRoad->ZQZJB[i + Dip].AFA < 0.0)
			wr_text(pj, ang, 0.5 * d3 + 3.5 * d1, wk, ht, ZjStr);
		else
			wr_text(pj, ang, 0.5 * d3 + 4.5 * d1, wk, ht, ZjStr);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].R2, 2, 3, str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].R);
		wr_text(pj, ang, 0.5 * d3 + 5.0 * d1 + 0.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].Ls1, 2, 3, str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].Lo1);
		wr_text(pj, ang, 0.5 * d3 + 5.0 * d1 + 1.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].Ls2, 2, 3, str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].Lo2);
		wr_text(pj, ang, 0.5 * d3 + 5.0 * d1 + 2.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].T1, 2, 3, str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].T1);
		wr_text(pj, ang, 0.5 * d3 + 5.0 * d1 + 3.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].T2, 2, 3, str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].T2);
		wr_text(pj, ang, 0.5 * d3 + 5.0 * d1 + 4.5 * d2, wk, ht, str);

		ads_rtos(m_pRoad->ZQZJB[i + Dip].L, 2, 3, str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].K);
		wr_text(pj, ang, 0.5 * d3 + 5.0 * d1 + 5.5 * d2, wk, ht, str);

		if (m_pRoad->ZQZJB[i + Dip].OutDist > 0.0)
		{
			ads_rtos(m_pRoad->ZQZJB[i + Dip].OutDist, 2, 3, str);
			//          _stprintf(str,L"%0.3lf",CmfTab[i+Dip].E);
			wr_text(pj, ang, 0.5 * d3 + 5.0 * d1 + 6.5 * d2, wk, ht, str);
		}
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].ZHZH);
		wr_text(pj, ang, 0.5 * d3 + 5.5 * d1 + 7.0 * d2, wk, ht, str);
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].HYZH);
		wr_text(pj, ang, 0.5 * d3 + 6.5 * d1 + 7.0 * d2, wk, ht, str);
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].YoZH);
		wr_text(pj, ang, 0.5 * d3 + 7.5 * d1 + 7.0 * d2, wk, ht, str);
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].YHZH);
		wr_text(pj, ang, 0.5 * d3 + 8.5 * d1 + 7.0 * d2, wk, ht, str);
		_stprintf(str, L"%s", m_pRoad->ZQZJB[i + Dip].HZZH);
		wr_text(pj, ang, 0.5 * d3 + 9.5 * d1 + 7.0 * d2, wk, ht, str);
		i++;
	}
	return RTNORM;
}
void RoadAutoDesign::LineUD(ads_point p1, ads_point p2, double angx, double dist, double lw)
{
	ads_point pt1, pt2;

	ptAddy(p1, angx, dist, pt1);
	ptAddy(p2, angx, dist, pt2);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,lw,RTREAL,lw,RTPOINT,pt2,RTSTR,L"",RTNONE);
	AcGePoint3d ptb, pte;
	ptb.x = pt1[X];
	ptb.y = pt1[Y];
	ptb.z = 0;
	pte.x = pt2[X];
	pte.y = pt2[Y];
	pte.z = 0;
	BAS_DRAW_FUN::makeline(ptb, pte, 2, lw, L"曲线要素");
}
void RoadAutoDesign::LineLR(ads_point p1, ads_point p2, double angx, double dist, double lw, int color)
{
	ads_point pt1, pt2;

	ptAddx(p1, angx, dist, pt1);
	ptAddx(p2, angx, dist, pt2);

	AcGePoint3d ptb, pte;
	ptb.x = pt1[X];
	ptb.y = pt1[Y];
	ptb.z = 0;
	pte.x = pt2[X];
	pte.y = pt2[Y];
	pte.z = 0;
	BAS_DRAW_FUN::makeline(ptb, pte, color, lw, L"曲线要素");

}
void RoadAutoDesign::wr_text(ads_point pt, double angx, double d, double wk, double ht, ACHAR* str)
{
	double l;
	ads_point ps;

	//	l = 2.0*DFACTOR;
	ptAddx(pt, angx, d, ps);
	//	ptAddy(ps,angx,wk,ps);
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,ps,RTREAL,ht,RTREAL,angx*180/PI,RTSTR,str,RTNONE);
	AcGePoint3d ptb;
	ptb.x = ps[X];
	ptb.y = ps[Y];
	ptb.z = 0;
	BAS_DRAW_FUN::maketext(ptb, str, angx, ht, 2, 3, L"宋体", L"曲线要素");
	//	acedCommandS(RTSTR,L"Text",RTPOINT,ps,RTREAL,ht,RTREAL,angx*180/PI,RTSTR,str,RTNONE);
}
void RoadAutoDesign::MidPoint(ads_point pt, double w, double h, double angx, ads_point Mpt)
{
	ads_point p1, p2;

	ads_polar(pt, angx, w, p1);
	ads_polar(p1, angx + 0.5 * PI, h, p2);
	Mpt[X] = 0.5 * (pt[X] + p2[X]);
	Mpt[Y] = 0.5 * (pt[Y] + p2[Y]);
}

/*===================================================================
函数名:DrawRoadbed
------------------------------------------------------------
函数功能:绘路基大样图
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190814
------------------------------------------------------------
备注：
=====================================================================*/
void RoadAutoDesign::DrawRoadbed()
{
	if (!m_OvalDrawPara.IsDrawRoadbed)
		return;

	//插入图框
	ACHAR dwgPath[256], FrameBlockName[256];
	_tcscpy(dwgPath, m_FramePath);
	_stprintf(FrameBlockName, L"图框-%s", Pro);
	AcDbObjectId pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, FrameBlockName);
	AcGePoint3d InsertPt(m_CurX + m_Frame_Width / 2.0, m_CurY + m_Frame_Height / 2.0, 0.0);
	if (pBlockId > 0)
	{
		AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setColorIndex(7);
		pBkRr->setLayer(L"图框");
		BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
	}

	//插入路基大样图
	BAS_DRAW_FUN::CreateLayer(L"路基大样图");
	_stprintf(dwgPath, L"%s\\lib\\路基大样图.dwg", SysPath);
	pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"路基大样图");
	InsertPt.set(m_CurX, m_CurY, 0.0);
	if (pBlockId > 0)
	{
		AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setColorIndex(7);
		pBkRr->setLayer(L"路基大样图");
		BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
	}

	//绘制工程数量表
	HdmDes HDes;
	//读文件		
	if (!HDes.ReadFile(false))//横断面标准
		return;

	HDes.ReadBDFile();

	//从鼻端文件中读取左右关联道路，若道路总数>0按互通处理，否则按分离式处理
	CString xl1;
	xl1.Format(L"%s,%s", HDes.road[0].pm->mdbname, HDes.road[0].roadname);

	//	SetHdmLm();//求路面

	HDes.FindAndReadLinkRoad_for3d(xl1);

	ads_printf(L"读数据库OK!\n");

	ads_printf(L"roadsum=%d\n", HDes.RoadSum);
	//横断面成果
	for (int i = 0; i < HDes.RoadSum; i++)
	{

		if (!HDes.ReadHdmResultFile(i))
			return;

		if (!HDes.ReadHdmAreaFile(i))
			return;
	}
	int iroad = 0;
	double OriX = 0.0;
	double OriY = 0.0;//左上角点
	OriX = m_CurX + 400.0;
	OriY = m_CurY + 180.0 + 190.0;
	//绘制图框
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	//传递指定桩号数组
	/*if (!if_PL_CT)
	{
		for (int i = 0; i < HDes.road[iroad].NCRS; i++)
		{
			if (HDes.road[iroad].hdmdes[i].cml>CTeml)
			{
				HDes.road[iroad].NCRS = i;
				break;
			}
		}
	}*/
	HDes.m_CrossTypeArray = m_CrossTypeArray;
	//绘制图框
	HDes.DrawTK(OriX, OriY, 2.0);
	//绘制文字
	HDes.DrawWord(OriX, OriY, 2.0);
	//修正横断面数组
	HDes.ModifyCrossArray(iroad);
	//标注文字
	HDes.CalTorWforPSandDM(iroad);
	HDes.DrawData(iroad, OriX, OriY, 2.0);
	acDocManager->unlockDocument(acDocManager->curDocument());

	double cenX = 0.0, cenY = 0.0;
	cenX = m_CurX + m_Frame_Width / 2.0;
	cenY = m_CurY + m_Frame_Height / 2.0;
	CString StartLC, EndLC;
	
	StartLC = pzLinep->XLC(m_HDrawPara.QD);
	EndLC = pzLinep->XLC(m_HDrawPara.ZD);
	
	if (m_TrimFlag == 1)//路基大样图布局出图
		DrawRoadbedOutput(cenX, cenY,StartLC,EndLC, m_FramePath);

	m_DrawingNum[6] = 1;
}

void RoadAutoDesign::NotePage()
{
	if (!m_OvalDrawPara.IsNotePage)
		return;

	//设置起始页码与总页码数
	int CurPageNum = m_OvalDrawPara.StartPageNum, TotalPageNum = 0;
	if (m_OvalDrawPara.TotalPageNum > 0)
		TotalPageNum = m_OvalDrawPara.TotalPageNum;
	else
		TotalPageNum = (int)((m_CurX - m_OriX) / 1000 + 0.1) + 1;

	//设置参数
	AcGePoint3d ptb(0.0, 0.0, 0.0);
	ACHAR tempStr[256];
	double CurX = m_OriX;

	for (int i = 1; i <= TotalPageNum; i++)
	{
		//标注当前页码
		ptb.x = CurX + 660.0;
		ptb.y = m_CurY + 537.5;
		if(!m_IsreadTK/*|| m_TrimFlag == 1*/)
		    _stprintf(tempStr, L"第 %d 页", CurPageNum++);
		else
			_stprintf(tempStr, L"%d", CurPageNum++);
		BAS_DRAW_FUN::maketext(ptb, tempStr, 0.0, 12.8, 7, 3, L"宋体", L"图框");

		//标注总页码
		ptb.x = CurX + 742.5;
		ptb.y = m_CurY + 537.5;

		if (!m_IsreadTK/* || m_TrimFlag == 1*/)
			_stprintf(tempStr, L"共 %d 页", TotalPageNum);
		else
			_stprintf(tempStr, L"%d", TotalPageNum);
		BAS_DRAW_FUN::maketext(ptb, tempStr, 0.0, 12.8, 7, 3, L"宋体", L"图框");

		CurX += 1000.0;
	}
}

void RoadAutoDesign::NoteDrawingName()
{
	if (!m_OvalDrawPara.IsNoteDrawingName)
		return;

	double CurX = m_OriX;
	double CurY = m_OriY - 10.0;
	ACHAR tempStr[256];
	AcGePoint3d tempPt(0.0, 0.0, 0.0);

	for (int i = 0; i < m_DrawingNum.size(); i++)
	{
		switch (i)
		{
		case 0:
			_tcscpy(tempStr, L"平面总体图");
			break;
		case 1:
			_tcscpy(tempStr, L"平面图");
			break;
		case 2:
			_tcscpy(tempStr, L"用地图");
			break;
		case 3:
			_tcscpy(tempStr, L"纵断面图");
			break;
		case 4:
			_tcscpy(tempStr, L"横断面图");
			break;
		case 5:
			_tcscpy(tempStr, L"直线、曲线及转角表");
			break;
		case 6:
			_tcscpy(tempStr, L"路基大样图");
			break;
		default:
			_tcscpy(tempStr, L"");
			break;
		}

		for (int j = 0; j < m_DrawingNum[i]; j++)
		{
			tempPt.x = CurX + 300.0;
			tempPt.y = CurY;
			BAS_DRAW_FUN::maketext(tempPt, tempStr, 0.0, 6.4, 7, 3, L"宋体", L"图框");

			CurX += 1000;
		}
	}
}

void RoadAutoDesign::DrawProfile_Gutter()
{
	//SetProfileDrawPara();
	m_VDrawPara.IsGutterMode = true;

	CString XZ;
	double Scml, Ecml, xscale, yscale, xinori, yinori, cml;
	int i, drawmode;

	CZDMHT zdmdraw;
	if (DrawZdm.GetSize() > 0)
		DrawZdm.RemoveAll();

	NZdmT = 0;
	int NZCG, NYCG, NZJK, NYJK, NZLMK, NYLMK, NBXDMX = 0;
	CgSZ* pZcg = NULL, * pYcg = NULL, * pZjk = NULL, * pYjk = NULL;
	LMKdata* pZlmk = NULL, * pYlmk = NULL;
	XLDataBase* pdbs = NULL, * pzdbs = NULL, * pydbs = NULL;
	XLDataBase* pBxDBS[10];//最多10个边线地面线
	pdbs = new  XLDataBase;
	pzLinep = NULL;
	pzLinep = m_pRoad;
	if (pzLinep == NULL)
	{
		return;
	}

	pdbs->Read_XLDbs(pzLinep->mdbname, L"纵断链表", pzLinep->RoadName);

	//读取坡度表数据
	if (m_VDrawPara.ZPInf == 1)//全幅
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"全幅");
	else if (m_VDrawPara.ZPInf == 2)//左幅
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"左幅");
	else if (m_VDrawPara.ZPInf == 3)//右幅
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"右幅");
	else//全部
	{
		pdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"全幅");
		pdbs->ReadAllXZ(pzLinep->mdbname, pzLinep->RoadName);
		if (pdbs->RoadZ == "左幅")
		{
			pzdbs = new  XLDataBase;
			pzdbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"左幅");
		}
		if (pdbs->RoadY == "右幅")
		{
			pydbs = new  XLDataBase;
			pydbs->Read_XLDbs(pzLinep->mdbname, L"坡度表", pzLinep->RoadName, L"右幅");
		}
	}
	BAS_DRAW_FUN ob;
	if (pdbs)
	{
		for (i = 0; i < pdbs->NPD; i++)
			pdbs->PDB[i].cml = ob.TYLC1(pdbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}
	if (pzdbs)
	{
		for (i = 0; i < pzdbs->NPD; i++)
			pzdbs->PDB[i].cml = ob.TYLC1(pzdbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}
	if (pydbs)
	{
		for (i = 0; i < pydbs->NPD; i++)
			pydbs->PDB[i].cml = ob.TYLC1(pydbs->PDB[i].ml, pdbs->NZDL, pdbs->ZDLArray);
	}

	//读取地面线数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"纵地面线表", pzLinep->RoadName);

	//读取桥隧数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"大中桥", pzLinep->RoadName/*,XZ*/);
	pdbs->Read_XLDbs(pzLinep->mdbname, L"隧道", pzLinep->RoadName/*,XZ*/);
	pdbs->Read_XLDbs(pzLinep->mdbname, L"小桥涵", pzLinep->RoadName);

	//读取超高数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"左超高分段表", pzLinep->RoadName);
	NZCG = pdbs->NCG;
	pZcg = new CgSZ[NZCG];
	for (i = 0; i < NZCG; i++)
	{
		pZcg[i] = pdbs->CG[i];
		pZcg[i].cml = pzLinep->TYLC(pZcg[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname, L"右超高分段表", pzLinep->RoadName);
	NYCG = pdbs->NCG;
	pYcg = new CgSZ[NYCG];
	for (i = 0; i < NYCG; i++)
	{
		pYcg[i] = pdbs->CG[i];
		pYcg[i].cml = pzLinep->TYLC(pYcg[i].xlc);
	}

	//读取超高数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"左加宽分段表", pzLinep->RoadName);
	NZJK = pdbs->NCG;
	pZjk = new CgSZ[NZJK];
	for (i = 0; i < NZJK; i++)
	{
		pZjk[i] = pdbs->CG[i];
		pZjk[i].cml = pzLinep->TYLC(pZjk[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname, L"右加宽分段表", pzLinep->RoadName);
	NYJK = pdbs->NCG;
	pYjk = new CgSZ[NYJK];
	for (i = 0; i < NYJK; i++)
	{
		pYjk[i] = pdbs->CG[i];
		pYjk[i].cml = pzLinep->TYLC(pYjk[i].xlc);
	}

	//读取路面宽数据
	pdbs->Read_XLDbs(pzLinep->mdbname, L"左路面宽分段表", pzLinep->RoadName);
	NZLMK = pdbs->NLMK;
	pZlmk = new LMKdata[NZLMK];
	for (i = 0; i < NZLMK; i++)
	{
		pZlmk[i] = pdbs->lmk[i];
		pZlmk[i].cml = pzLinep->TYLC(pZlmk[i].xlc);
	}
	pdbs->Read_XLDbs(pzLinep->mdbname, L"右路面宽分段表", pzLinep->RoadName);
	NYLMK = pdbs->NLMK;
	pYlmk = new LMKdata[NYLMK];
	for (i = 0; i < NYLMK; i++)
	{
		pYlmk[i] = pdbs->lmk[i];
		pYlmk[i].cml = pzLinep->TYLC(pYlmk[i].xlc);
	}

	if (pdbs->NPD == 0)
		return;

	//设置绘图参数
	xscale = m_VDrawPara.Hscale / 1000;		//x轴比例尺
	yscale = m_VDrawPara.Zscale / 1000;		//y轴比例尺
	xinori = m_CurX;				//绘图原点X
	yinori = m_CurY;				//绘图原点Y
	drawmode = m_VDrawPara.DrawMode;				//绘图模式
	Scml = m_pRoad->TYLC(m_VDrawPara.StDml);	//起点里程
	Ecml = m_pRoad->TYLC(m_VDrawPara.EdDml);	//终点里程
	pzLinep->FormLCB(100, Scml, Ecml);	//按100m间距生成里程表
	i = 0;
	int CurTno, j;
	CurTno = m_VDrawPara.SPage;
	_stprintf(m_VDrawPara.FrameBlockName, L"图框-%s", Pro);

	while (Scml < Ecml - 0.01)
	{
		//传递绘图数据
		zdmdraw.m_pm = m_pRoad;
		zdmdraw.CurTNo = CurTno + i;
		zdmdraw.m_TKName = m_FramePath;
		zdmdraw.m_dzgkname = m_VDrawPara.dzgkname;
		zdmdraw.m_bzym = m_VDrawPara.bzym;
		zdmdraw.pDBS = pdbs;
		zdmdraw.pZDBS = pzdbs;
		zdmdraw.pYDBS = pydbs;
		zdmdraw.NZCG = NZCG;
		zdmdraw.NYCG = NYCG;
		zdmdraw.ZCG = pZcg;
		zdmdraw.YCG = pYcg;
		zdmdraw.NZJK = NZJK;
		zdmdraw.NYJK = NYJK;
		zdmdraw.ZJK = pZjk;
		zdmdraw.YJK = pYjk;
		zdmdraw.NZLMK = NZLMK;
		zdmdraw.NYLMK = NYLMK;
		zdmdraw.ZLMK = pZlmk;
		zdmdraw.YLMK = pYlmk;
		zdmdraw.X_scale = xscale;
		zdmdraw.Y_scale = yscale;
		zdmdraw.X_inori = xinori + i * 1000;
		zdmdraw.Y_inori = yinori;
		zdmdraw.DrawMode = drawmode;
		zdmdraw.infwgx = m_VDrawPara.infwgx;
		zdmdraw.isdrawctrh = m_VDrawPara.ifDrawCtrH;
		zdmdraw.m_Scml = Scml;
		zdmdraw.NBxDmx = NBXDMX;

		m_CurX += 1000.0;
		//传递边界地面线数据
		for (j = 0; j < NBXDMX; j++)
			zdmdraw.BxDBS[j] = pBxDBS[j];
		//根据图幅设定传递相应参数
		if (drawmode)//施工
		{
			if (i == 0)//第一幅
			{
				if (m_VDrawPara.FForZF == 0)//分幅输出
				{
					//cml = Scml + xscale * 700;
					cml = Scml + xscale * 600;
					if (cml > Ecml)
						cml = Ecml;
				}
				else//整幅输出
				{
					cml = Ecml;
					//zdmdraw.NTKHt = (pdbs->Hmax-pdbs->Hmin)/yscale;
					//zdmdraw.WTKHt = zdmdraw.NTKHt+20;
					//ads_printf(L"yscale=%lf %lf %lf hmax=%lf hmin=%lf\n",yscale,zdmdraw.NTKHt,zdmdraw.WTKHt,pdbs->Hmax,pdbs->Hmin);
					//
				}
				zdmdraw.m_Ecml = cml;
				zdmdraw.IsFirst = true;
			}
			else
			{
				//cml = Scml + xscale * 700;
				cml = Scml + xscale * 600;
				if (cml > Ecml)
					cml = Ecml;
				zdmdraw.m_Ecml = cml;
				zdmdraw.IsFirst = false;
			}
		}
		else//初步设计
		{
			if (m_VDrawPara.FForZF == 0)//分幅输出
			{
				cml = Scml + xscale * 700;
				if (cml > Ecml)
					cml = Ecml;
			}
			else//整幅输出
			{
				cml = Ecml;
				//zdmdraw.NTKHt = (pdbs->Hmax-pdbs->Hmin)/yscale;
				//zdmdraw.WTKHt = zdmdraw.NTKHt+20;

			}

			zdmdraw.m_Ecml = cml;
		}

		SetWxpPara_Gutter(zdmdraw);

		zdmdraw.NTKLen = m_Frame_Width;
		zdmdraw.NTKHt = m_Frame_Height;

		zdmdraw.m_IsNoteScale = true;
		zdmdraw.m_VDrawPara = m_VDrawPara;

		zdmdraw.DrawZDM();

		Scml = zdmdraw.m_Ecml;
		DrawZdm.Add(zdmdraw);
		i++;
	}

	NZdmT = i;

	if (pZcg) delete[]pZcg;
	if (pYcg) delete[]pYcg;

	int Sum = 0;
	for (int i = 0; i < m_DrawingNum.size(); i++)
		Sum += m_DrawingNum[i];
	int DrawingNum = (int)((m_CurX - m_OriX) / 1000.0 + 0.1) - Sum;
	m_DrawingNum.push_back(DrawingNum);
}

void RoadAutoDesign::SetWxpPara_Gutter(CZDMHT& zdmdraw)
{
	zdmdraw.WayorRamp = 0;	//公路	暂时不考虑匝道
	zdmdraw.InfLmPt[0] = false;
	zdmdraw.InfLmPt[1] = false;
	zdmdraw.InfLmPt[2] = false;
	zdmdraw.InfLmPt[3] = false;
	zdmdraw.InfLmPt[4] = false;
	zdmdraw.InfLmPt[5] = false;
	zdmdraw.InfLmPt[6] = false;
	zdmdraw.InfLmPt[7] = false;
	zdmdraw.LmPtName[0] = _T("P1");
	zdmdraw.LmPtName[1] = _T("P2");
	zdmdraw.LmPtName[2] = _T("P3");
	zdmdraw.LmPtName[3] = _T("P4");
	zdmdraw.LmPtName[4] = _T("P5");
	zdmdraw.LmPtName[5] = _T("P6");
	zdmdraw.LmPtName[6] = _T("P7");
	zdmdraw.LmPtName[7] = _T("P8");

	//传递五线谱参数（倒序传入）
	zdmdraw.WXPnum = m_VDrawPara.WXPArray.size();
	if (!m_VDrawPara.WXPArray.size())
		return;

	int Count = 0;
	for (int i = m_VDrawPara.WXPArray.size() - 1; i >= 0; i--)
	{
		zdmdraw.wxp[Count] = m_VDrawPara.WXPArray[i];
		Count++;
	}
}

void RoadAutoDesign::DrawCross_Gutter()
{
	HdmDes Cross;

	Cross.CreateLayer(L"清表土");
	//读文件		
	if (!Cross.ReadFile(false))//横断面标准
		return;

	Cross.ReadBDFile();

	//从鼻端文件中读取左右关联道路，若道路总数>0按互通处理，否则按分离式处理
	CString xl1;
	xl1.Format(L"%s,%s", m_MdbName, m_RoadName);

	//	SetHdmLm();//求路面

	Cross.FindAndReadLinkRoad_for3d(xl1);

	ads_printf(L"读数据库OK!\n");

	//横断面成果
	for (int i = 0; i < Cross.RoadSum; i++)
	{

		if (!Cross.ReadHdmResultFile(i))
			return;

		if (!Cross.ReadHdmAreaFile(i))
			return;
	}

	int iroad;

	CString CKSml = Cross.road[0].hdmdes[0].ckml;
	CString CKEml = Cross.road[0].hdmdes[Cross.road[0].NCRS - 1].ckml;

	iroad = Cross.GetIRoadByName(m_RoadName);
	double sml, eml;
	sml = Cross.road[iroad].pm->TYLC(CKSml);
	eml = Cross.road[iroad].pm->TYLC(CKEml);

	int lnum;//列数
	lnum = 2;	//暂时设置为2列
	double zk, yk;
	zk = m_CDrawPara.EarthWidL;
	yk = m_CDrawPara.EarthWidR;
	Cross.StartX = m_CurX;
	Cross.StartY = m_CurY;   // 获取绘制起点
	CString TKname;
	TKname = m_FramePath;
	Cross.CurRoad = iroad;
	int spage;
	spage = 0;
	Cross.Ifpage = false;

	Sfactor = 1000.0 / m_CDrawPara.Scale;

	Cross.m_IsModifyGutter = true;
	Cross.m_GutterPath = m_GutterPath;
	Cross.m_GutterThick = m_GutterThick;
	Cross.m_IsDrawCrossEarthPile = m_CDrawPara.IsDrawEarthPile;
	Cross.m_IsDrawHengDuanMianWangGe = m_CDrawPara.IsDrawGrid;
	_stprintf(Cross.m_FrameBlockName, L"图框-%s", Pro);
	_stprintf(Cross.m_GutterBlockName, L"水沟-%s", Pro);

	Cross.DrawHdmMap_Gutter(iroad, sml, eml, zk, yk, TKname, lnum);

	m_CurX += Cross.MAPSUM * 1000.0;

	BAS_DRAW_FUN::SetLayerColor(L"图框", 7);	//20190805

	int Sum = 0;
	for (int i = 0; i < m_DrawingNum.size(); i++)
		Sum += m_DrawingNum[i];
	int DrawingNum = (int)((m_CurX - m_OriX) / 1000.0 + 0.1) - Sum;
	m_DrawingNum.push_back(DrawingNum);
}

void RoadAutoDesign::DrawCurInforOutput(double CenX, double CenY, CString s_ckml, CString e_ckml, CString TKname)
{
	//关闭捕捉对象
	BAS_DRAW_FUN::Set_OSMODE(0);
#ifdef VERSION_CAD2016
	acedCommandS(RTNONE);
	acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	acedCommandS(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);
	acedCommandS(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTNONE);
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);	//命令行提示关
	ads_command(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);	//十字架模式关
	ads_command(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);	//设置坐标系
#endif // VERSION_CAD2010
	AcGePoint3d pt;
	pt.x = CenX;
	pt.y = CenY;

	CString skml, ekml;
	skml = s_ckml;
	ekml = e_ckml;
	STuKuang* TK = NULL;
	TK = new STuKuang[1];

	TK->CenX = pt.x;
	TK->CenY = pt.y;
	TK->LtUpX = TK->CenX - m_Frame_Width / 2.0 - 50.0;
	TK->LtUpY = TK->CenY + m_Frame_Height / 2.0 + 10.0;

	TK->LtBmX = TK->CenX - m_Frame_Width / 2.0 - 50.0;
	TK->LtBmY = TK->CenY - m_Frame_Height / 2.0 - 30.0;

	TK->RtUpX = TK->CenX + m_Frame_Width / 2.0 + 10.0;
	TK->RtUpY = TK->CenY + m_Frame_Height / 2.0 + 10.0;

	TK->RtBmX = TK->CenX + m_Frame_Width / 2.0 + 10.0;
	TK->RtBmY = TK->CenY - m_Frame_Height / 2.0 - 30.0;

	TransFormTK_Roadbed(TK, 411.48 / 2.0, 284.90 / 2.0, 1);

	STuKuang *CurTK = &TK[0];
	ACHAR LayoutName[256];
	_stprintf(LayoutName, L"CurInfor_%s～%s", skml, ekml);
	CMyLayout::CreateLayout(LayoutName);

#ifdef VERSION_CAD2010 
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	ads_command(RTSTR, L"-OSNAP", RTSTR, L"OFF");
#endif	

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	acedCommandS(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);  //设置坐标系
	ads_command(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE); //清除布局中所有实体
#endif // VERSION_CAD2010

	ads_point p1, p2, p3, p4, z1, z2, ori, orix, oriy;
	double xmin, xmax, ymin, ymax;
	p1[X] = CurTK->newLtUpX;
	p1[Y] = CurTK->newLtUpY;

	p2[X] = CurTK->newRtUpX;
	p2[Y] = CurTK->newRtUpY;

	p3[X] = CurTK->newRtBmX;
	p3[Y] = CurTK->newRtBmY;

	p4[X] = CurTK->newLtBmX;
	p4[Y] = CurTK->newLtBmY;

	double jiao;
	BAS_DRAW_FUN::xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);
	jiao += 0.5*PI;
	ori[X] = CurTK->LtBmX;
	ori[Y] = CurTK->LtBmY;

	orix[X] = CurTK->RtBmX;
	orix[Y] = CurTK->RtBmY;

	oriy[X] = ori[X] + 10 * cos(jiao);
	oriy[Y] = ori[Y] + 10 * sin(jiao);

	//BAS_DRAW_FUN::getMinMaxXY(p1,p2,p3,p4,xmin,xmax,ymin,ymax);
	xmin = 0.0;
	xmax = (p3[X] - p4[X]);//*K;
	ymin = 0.0;
	ymax = (p2[Y] - p3[Y]);//*K;

	if (p1[X] < p4[X])xmin -= (p4[X] - p1[X]);//*K;
	if (p2[X] > p3[X])xmax += (p2[X] - p3[X]);//*K;					

	z1[X] = xmin;
	z1[Y] = ymin;
	z2[X] = xmax;
	z2[Y] = ymax;

#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
	ads_command(RTSTR, L"MSPACE", RTNONE);
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);

	ads_command(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, ori, RTPOINT, orix, RTPOINT, oriy, RTNONE);
	ads_command(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);
	ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE);
	ads_command(RTSTR, L"PSPACE", RTNONE);
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	ads_command(RTSTR, L"-VPORTS", RTSTR, L"LOCK", RTSTR, L"ON", RTSTR, L"ALL", RTSTR, L"", RTNONE);
	ads_command(RTSTR, L"ZOOM", RTSTR, L"E", RTNONE);
	ads_command(RTNONE);
#endif // VERSION_CAD2010
	ACHAR dwgPath[80];
	_tcscpy(dwgPath, TKname);
	AcDbObjectId pBlockId;
	pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"");
	if (pBlockId > 0)
	{
		AcGePoint3d InsertPt(411.48 / 2.0, 284.90 / 2.0, 0.0);
		AcDbBlockReference *pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setScaleFactors(AcGeScale3d(1, 1, 1));
		BAS_DRAW_FUN::AddEntityToDbs(pBkRr, 0, 1);
	}
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
	ads_command(RTNONE);
#endif // VERSION_CAD2010

	if (TK)delete[]TK;
}
void RoadAutoDesign::DrawRoadbedOutput(double CenX, double CenY, CString s_ckml, CString e_ckml, CString TKname)
{
	//关闭捕捉对象
	BAS_DRAW_FUN::Set_OSMODE(0);
#ifdef VERSION_CAD2016
	acedCommandS(RTNONE);
	acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	acedCommandS(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);
	acedCommandS(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTNONE);
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);	//命令行提示关
	ads_command(RTSTR, L"BLIPMODE", RTSTR, L"OFF", RTNONE);	//十字架模式关
	ads_command(RTSTR, L"UCSICON", RTSTR, L"OFF", RTNONE);	//设置坐标系
#endif // VERSION_CAD2010
	AcGePoint3d pt;
	pt.x = CenX;
	pt.y = CenY;

	CString skml, ekml;
	skml = s_ckml;
	ekml = e_ckml;
	STuKuang* TK = NULL;
	TK = new STuKuang[1];


	TK->CenX = pt.x;
	TK->CenY = pt.y;
	TK->LtUpX = TK->CenX - m_Frame_Width / 2.0 - 50.0;
	TK->LtUpY = TK->CenY + m_Frame_Height / 2.0 + 10.0;

	TK->LtBmX = TK->CenX - m_Frame_Width / 2.0 - 50.0;
	TK->LtBmY = TK->CenY - m_Frame_Height / 2.0 - 30.0;

	TK->RtUpX = TK->CenX + m_Frame_Width / 2.0 + 10.0;
	TK->RtUpY = TK->CenY + m_Frame_Height / 2.0 + 10.0;

	TK->RtBmX = TK->CenX + m_Frame_Width / 2.0 + 10.0;
	TK->RtBmY = TK->CenY - m_Frame_Height / 2.0 - 30.0;

	TransFormTK_Roadbed(TK, 411.48 / 2.0, 284.90 / 2.0, 1);

	STuKuang *CurTK = &TK[0];
	ACHAR LayoutName[256];
	_stprintf(LayoutName, L"Roadbed_%s～%s", skml, ekml);
	CMyLayout::CreateLayout(LayoutName);

#ifdef VERSION_CAD2010 
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
	ads_command(RTSTR, L"-OSNAP", RTSTR, L"OFF");
#endif	

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	acedCommandS(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);  //设置坐标系
	ads_command(RTSTR, L"ERASE", RTSTR, L"ALL", RTSTR, L"", RTNONE); //清除布局中所有实体
#endif // VERSION_CAD2010

	ads_point p1, p2, p3, p4, z1, z2, ori, orix, oriy;
	double xmin, xmax, ymin, ymax;
	p1[X] = CurTK->newLtUpX;
	p1[Y] = CurTK->newLtUpY;

	p2[X] = CurTK->newRtUpX;
	p2[Y] = CurTK->newRtUpY;

	p3[X] = CurTK->newRtBmX;
	p3[Y] = CurTK->newRtBmY;

	p4[X] = CurTK->newLtBmX;
	p4[Y] = CurTK->newLtBmY;

	double jiao;
	BAS_DRAW_FUN::xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);
	jiao += 0.5*PI;
	ori[X] = CurTK->LtBmX;
	ori[Y] = CurTK->LtBmY;

	orix[X] = CurTK->RtBmX;
	orix[Y] = CurTK->RtBmY;

	oriy[X] = ori[X] + 10 * cos(jiao);
	oriy[Y] = ori[Y] + 10 * sin(jiao);

	//BAS_DRAW_FUN::getMinMaxXY(p1,p2,p3,p4,xmin,xmax,ymin,ymax);
	xmin = 0.0;
	xmax = (p3[X] - p4[X]);//*K;
	ymin = 0.0;
	ymax = (p2[Y] - p3[Y]);//*K;

	if (p1[X] < p4[X])xmin -= (p4[X] - p1[X]);//*K;
	if (p2[X] > p3[X])xmax += (p2[X] - p3[X]);//*K;					

	z1[X] = xmin;
	z1[Y] = ymin;
	z2[X] = xmax;
	z2[Y] = ymax;

#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-VPORTS", RTSTR, L"POLYGONAL", RTPOINT, p1, RTPOINT, p2, RTPOINT, p3, RTPOINT, p4, RTSTR, L"CLOSE", RTNONE);
	ads_command(RTSTR, L"MSPACE", RTNONE);
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);

	ads_command(RTSTR, L"UCS", RTSTR, L"NEW", RTSTR, L"3POINT", RTPOINT, ori, RTPOINT, orix, RTPOINT, oriy, RTNONE);
	ads_command(RTSTR, L"PLAN", RTSTR, L"CURRENT", RTNONE);
	ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, z1, RTPOINT, z2, RTNONE);
	ads_command(RTSTR, L"PSPACE", RTNONE);
	ads_command(RTSTR, L"UCS", RTSTR, L"WORLD", RTNONE);
	ads_command(RTSTR, L"-VPORTS", RTSTR, L"LOCK", RTSTR, L"ON", RTSTR, L"ALL", RTSTR, L"", RTNONE);
	ads_command(RTSTR, L"ZOOM", RTSTR, L"E", RTNONE);
	ads_command(RTNONE);
#endif // VERSION_CAD2010
	ACHAR dwgPath[80];
	_tcscpy(dwgPath, TKname);
	AcDbObjectId pBlockId;
	pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"");
	if (pBlockId > 0)
	{
		AcGePoint3d InsertPt(411.48 / 2.0, 284.90 / 2.0, 0.0);
		AcDbBlockReference *pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setScaleFactors(AcGeScale3d(1, 1, 1));
		BAS_DRAW_FUN::AddEntityToDbs(pBkRr, 0, 1);
	}
#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
	acedCommandS(RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"-PLOT", RTSTR, L"Y", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"E", RTSTR, L"F", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"", RTSTR, L"",
		RTSTR, L"", RTSTR, L"Y", RTSTR, L"N", RTNONE);
	ads_command(RTNONE);
#endif // VERSION_CAD2010

	if (TK)delete[]TK;
}

void RoadAutoDesign::TransFormTK_Roadbed(STuKuang *TK, double CenX, double CenY, double scale)
{
	AcGeVector3d norm(0, 0, 1);
	norm.normalize();

	STuKuang *CurTK = &TK[0];

	CurTK->newCenX = CenX;
	CurTK->newCenY = CenY;

	AcGeMatrix3d xformR, xformT, xformS;
	double jiao;
	BAS_DRAW_FUN ob;
	ob.xyddaa(CurTK->LtBmX, CurTK->LtBmY, CurTK->RtBmX, CurTK->RtBmY, &jiao);

	AcGeVector3d  moveBy(CurTK->newCenX - CurTK->CenX, CurTK->newCenY - CurTK->CenY, 0.0);
	AcGePoint3d PP(CurTK->CenX, CurTK->CenY, 0.0);
	xformR.setToRotation(-1.0*jiao, norm, PP);
	xformT.setToTranslation(moveBy);
	CurTK->xform = xformT * xformR;

	AcGePoint3d StPt(CurTK->StX, CurTK->StY, 0.0);
	StPt.transformBy(CurTK->xform);
	CurTK->newStX = StPt.x;
	CurTK->newStY = StPt.y;

	AcGePoint3d EdPt(CurTK->EdX, CurTK->EdY, 0.0);
	EdPt.transformBy(CurTK->xform);
	CurTK->newEdX = EdPt.x;
	CurTK->newEdY = EdPt.y;

	AcGePoint3d LtUpPt(CurTK->LtUpX, CurTK->LtUpY, 0.0);
	LtUpPt.transformBy(CurTK->xform);
	CurTK->newLtUpX = LtUpPt.x;
	CurTK->newLtUpY = LtUpPt.y;

	AcGePoint3d LtBmPt(CurTK->LtBmX, CurTK->LtBmY, 0.0);
	LtBmPt.transformBy(CurTK->xform);
	CurTK->newLtBmX = LtBmPt.x;
	CurTK->newLtBmY = LtBmPt.y;

	AcGePoint3d RtUpPt(CurTK->RtUpX, CurTK->RtUpY, 0.0);
	RtUpPt.transformBy(CurTK->xform);
	CurTK->newRtUpX = RtUpPt.x;
	CurTK->newRtUpY = RtUpPt.y;

	AcGePoint3d RtBmPt(CurTK->RtBmX, CurTK->RtBmY, 0.0);
	RtBmPt.transformBy(CurTK->xform);
	CurTK->newRtBmX = RtBmPt.x;
	CurTK->newRtBmY = RtBmPt.y;
}

void RoadAutoDesign::DrawOverallHoriRoad()
{
	if (!m_OvalDrawPara.IsDrawHoriOvalDrawing)
		return;

	m_CurX = m_OriX;
	ReadDrawHorizontalPara();

	m_HDrawPara.DrawMode = 1;
	DrawCenterLine();
	//DrawHorizontalOutput();
	DrawHorizontalOutput_Trimflag_D();
}

/*===================================================================
函数名:GetBPDStAndEdfromJP
------------------------------------------------------------
函数功能:根据接坡信息形成原始变坡点数组
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:
------------------------------------------------------------
备注：	[i][0]里程 [i][1]高程 [i][2]竖曲线 [i][3]坡度
=====================================================================*/
void RoadAutoDesign::GetBPDStAndEdfromJP(int&BPDnum,JD_CENTER*pzLinep)
{
	double PD_array[MAXBPDNUM][4];
	bool Is_Constr_Start = true; 	//起点是否需要约束
	bool Is_Constr_End = true; 	//终点是否需要约束
	double JXPCMin = _wtof(m_PrfDesData.JXPCMin);
	double YBPCMin = _wtof(m_PrfDesData.YBPCMin);//一般坡长作为判断
	double  cml = 0.0, level = 0.0;

	for (int i = 0; i < MAXBPDNUM; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			PD_array[i][j] = 0.0;
		}
	}
	//存在接坡信息赋里程初始值
	if (m_JPInforArray.size() > 0)
	{
		m_JPInforArray[0].Stcml = pzLinep->DLArray[0].ELC;
		m_JPInforArray[0].Edcml = pzLinep->XLLength;
	}

	if (m_JPInforArray.size() > 0)
	{
		//关于前后约束情况,满足最小坡长要求
		if (m_JPInforArray[0].Bef_PC > YBPCMin)
			Is_Constr_Start = false;
		if (m_JPInforArray[0].Aft_PC > YBPCMin)
			Is_Constr_End = false;


		if (Is_Constr_Start && Is_Constr_End) //前后都需约束
		{
			//当道路总长小于两倍最小坡长，且含有前后端接坡约束，这时直接求交上变坡点
			if (YBPCMin < pzLinep->XLLength && pzLinep->XLLength < 2.5 * YBPCMin)
			{
				//起始坡段【0】
				PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

				//如果小范围内求交有交点则上交点
				if (AutoCalPVI(cml, level))
				{
					Is_NeedInitPro = false;
					PD_array[1][0] = cml;
					PD_array[1][1] = level;
					//终止坡段【2】
					PD_array[2][0] = m_JPInforArray[0].Edcml;
					PD_array[2][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 3;
				}
				else //无交点则赋起终点信息
				{
					PD_array[1][0] = m_JPInforArray[0].Edcml;
					PD_array[1][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 2;
				}
			}
			else
			{
				//起始坡段【0】
				PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

				//【1】
				if (m_JPInforArray[0].Bef_PC < YBPCMin)
					PD_array[1][0] = PD_array[0][0] + YBPCMin - m_JPInforArray[0].Bef_PC;
				PD_array[1][1] = (PD_array[1][0] - PD_array[0][0])*m_JPInforArray[0].Bef_PD*0.01 + PD_array[0][1]; //高程

				//终止坡段【2】
				PD_array[3][0] = m_JPInforArray[0].Edcml;
				PD_array[3][1] = m_JPInforArray[0].Aft_H;

				//【3】
				if (m_JPInforArray[0].Aft_PC < YBPCMin)
					PD_array[2][0] = PD_array[3][0] - (YBPCMin - m_JPInforArray[0].Aft_PC);

				PD_array[2][1] = PD_array[3][1] - (PD_array[3][0] - PD_array[2][0])*m_JPInforArray[0].Aft_PD*0.01;
				BPDnum = 4;
			}

			m_BPDArray.resize(BPDnum);
			for (int i = 0; i < BPDnum; i++)
			{
				m_BPDArray[i].cml = PD_array[i][0];
				m_BPDArray[i].Level = PD_array[i][1];
			}
		}
		else if (Is_Constr_Start && Is_Constr_End == false) //前约束后不约束
		{
			//起始坡段【0】
			PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
			PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

			//【1】
			if (m_JPInforArray[0].Bef_PC < YBPCMin)
				PD_array[1][0] = PD_array[0][0] + YBPCMin - m_JPInforArray[0].Bef_PC;

			PD_array[1][1] = (PD_array[1][0] - PD_array[0][0])*m_JPInforArray[0].Bef_PD*0.01 + PD_array[0][1]; //高程

			//终止坡段【2】
			PD_array[2][0] = m_JPInforArray[0].Edcml;
			PD_array[2][1] = m_JPInforArray[0].Aft_H;

			BPDnum = 3;

			m_BPDArray.resize(BPDnum);
			for (int i = 0; i < BPDnum; i++)
			{
				m_BPDArray[i].cml = PD_array[i][0];
				m_BPDArray[i].Level = PD_array[i][1];
			}
		}
		else if (Is_Constr_Start == false && Is_Constr_End) //前不约束后约束
		{
			//起始坡段【0】
			PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
			PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

			//终止坡段【2】
			PD_array[2][0] = m_JPInforArray[0].Edcml;
			PD_array[2][1] = m_JPInforArray[0].Aft_H;

			//【1】
			if (m_JPInforArray[0].Aft_PC < YBPCMin)
				PD_array[1][0] = PD_array[2][0] - (YBPCMin - m_JPInforArray[0].Aft_PC);

			PD_array[1][1] = PD_array[2][1] - (PD_array[2][0] - PD_array[1][0])*m_JPInforArray[0].Aft_PD*0.01;

			BPDnum = 3;

			m_BPDArray.resize(BPDnum);
			for (int i = 0; i < BPDnum; i++)
			{
				m_BPDArray[i].cml = PD_array[i][0];
				m_BPDArray[i].Level = PD_array[i][1];
			}
		}
		else //前后都不约束
		{
			if (YBPCMin < pzLinep->XLLength && pzLinep->XLLength < 2.5 * YBPCMin)
			{
				//起始坡段【0】
				PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

				//如果小范围内求交有交点则上交点
				if (AutoCalPVI(cml, level))
				{
					Is_NeedInitPro = false;
					PD_array[1][0] = cml;
					PD_array[1][1] = level;
					//终止坡段【2】
					PD_array[2][0] = m_JPInforArray[0].Edcml;
					PD_array[2][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 3;
				}
				else //无交点则赋起终点信息
				{
					PD_array[1][0] = m_JPInforArray[0].Edcml;
					PD_array[1][1] = m_JPInforArray[0].Aft_H;
					BPDnum = 2;
				}
			}
			else
			{
				//起始坡段【0】
				PD_array[0][0] = m_JPInforArray[0].Stcml;//里程 
				PD_array[0][1] = m_JPInforArray[0].Bef_H; //高程

				//终止坡段【1】
				PD_array[1][0] = m_JPInforArray[0].Edcml;
				PD_array[1][1] = m_JPInforArray[0].Aft_H;
				BPDnum = 2;
			}

			m_BPDArray.resize(BPDnum);
			for (int i = 0; i < BPDnum; i++)
			{
				m_BPDArray[i].cml = PD_array[i][0];
				m_BPDArray[i].Level = PD_array[i][1];
			}

		}
	}
	else
	{
		//起始坡段【0】
		PD_array[0][0] = pzLinep->DLArray[0].ELC;//里程 
		PD_array[0][1] = m_EarthLine[0].level; //高程

		//终止坡段【1】
		PD_array[1][0] = pzLinep->XLLength;
		PD_array[1][1] = m_EarthLine[m_EarthLine.size() -1].level;
		BPDnum = 2;

		m_BPDArray.resize(BPDnum);
		for (int i = 0; i < BPDnum; i++)
		{
			m_BPDArray[i].cml = PD_array[i][0];
			m_BPDArray[i].Level = PD_array[i][1];
		}
	}

	if (m_JPInforArray.size() > 0)
	{
		//前后端约束赋值
		if (Is_Constr_Start)
			m_JPInforArray[0].Is_JP_Bef = 1;
		if (Is_Constr_End)
			m_JPInforArray[0].Is_JP_Aft = 1;
	}

}

//求交自动上个坡度
bool RoadAutoDesign::AutoCalPVI(double &ml, double &H)
{
	ads_point from1, to1;//起点坡段线段两端点
	ads_point from2, to2;//终点坡段线段两端点
	ads_point result;	 //起终坡段交点
	from1[Z] = 0.0; to1[Z] = 0.0; from2[Z] = 0.0; to2[Z] = 0.0;

	from1[X] = m_JPInforArray[0].Stcml - 20.0;
	from1[Y] = m_JPInforArray[0].Bef_H - 20.0*m_JPInforArray[0].Bef_PD*0.01;

	to1[X] = m_JPInforArray[0].Stcml;
	to1[Y] = m_JPInforArray[0].Bef_H;

	from2[X] = m_JPInforArray[0].Edcml;
	from2[Y] = m_JPInforArray[0].Aft_H;

	to2[X] = m_JPInforArray[0].Edcml + 20.0;
	to2[Y] = m_JPInforArray[0].Aft_H + 20.0*m_JPInforArray[0].Aft_PD*0.01;

	/*int iTest = ads_inters(from1, to1, from2, to2, 0, result);

	ml = result[X];
	H = result[Y];*/

	int iTest =  BAS_DRAW_FUN::INTERS(&ml, &H, from1[X], from1[Y], to1[X], to1[Y], from2[X], from2[Y], to2[X], to2[Y]);

	//如果求交没有结果两线平行
	if (iTest >= 0)
		return true;
	else
		return false;
}

//获取初始方案起终里程高程
void RoadAutoDesign::GetStAndEdLC(double &StartLC, double &EndLC, double &StLevel, double &EdLevel,JD_CENTER*pzLinep)
{
	if (m_JPInforArray.size() > 0)
	{
		if (m_JPInforArray[0].Is_JP_Bef == 1 && m_JPInforArray[0].Is_JP_Aft == 1) //前后约束
		{
			//BPDnum = 4
			int PDnum;
			PDnum = (int)m_BPDArray.size();
			if (PDnum == 4)
			{
				StartLC = m_BPDArray[1].cml; StLevel = m_BPDArray[1].Level;
				EndLC = m_BPDArray[2].cml; EdLevel = m_BPDArray[2].Level;
			}
			else
			{
				//由于坡长限制，求交变坡点时考虑
				StartLC = m_BPDArray[0].cml; StLevel = m_BPDArray[0].Level;
				EndLC = m_BPDArray[PDnum - 1].cml; EdLevel = m_BPDArray[PDnum - 1].Level;
			}

		}
		else if (m_JPInforArray[0].Is_JP_Bef == 1 && m_JPInforArray[0].Is_JP_Aft == 0) //前约束后不约束
		{
			//BPDnum = 3
			StartLC = m_BPDArray[1].cml;  StLevel = m_BPDArray[1].Level;
			EndLC = m_BPDArray[2].cml;  EdLevel = m_BPDArray[2].Level;
		}
		else if (m_JPInforArray[0].Is_JP_Bef == 0 && m_JPInforArray[0].Is_JP_Aft == 1) //前不约束后约束
		{
			//BPDnum = 3
			StartLC = m_BPDArray[0].cml; StLevel = m_BPDArray[0].Level;
			EndLC = m_BPDArray[1].cml; EdLevel = m_BPDArray[1].Level;
		}
		else
		{
			//BPDnum = 2
			StartLC = pzLinep->DLArray[0].ELC;  StLevel = m_BPDArray[0].Level;
			EndLC = pzLinep->XLLength;  EdLevel = m_BPDArray[1].Level;
		}
	}
	else
	{
		StartLC = pzLinep->DLArray[0].ELC;  StLevel = m_BPDArray[0].Level;
		EndLC = pzLinep->XLLength;  EdLevel = m_BPDArray[1].Level;
	}

	//优化过程使用ml,这里转换
	for (int i = 0; i < m_BPDArray.size(); i++)
		m_BPDArray[i].ml = m_BPDArray[i].cml;
}


//根据桩号里程排序
//根据统一里程给数据排序，并删除相同的桩号
template <typename compara>
void RoadAutoDesign::SortData(vector<compara> &dataAry)
{

	compara tepArray;
	for (int i = 0; i < dataAry.size(); i++)
	{
		for (int j = i + 1; j < dataAry.size(); j++)
		{
			if (dataAry[i].ml > dataAry[j].ml)
			{
				tepArray = dataAry[j];
				dataAry[j] = dataAry[i];
				dataAry[i] = tepArray;
			}
		}
	}

	//删除相同的桩号
	for (int i = 0; i + 1 < dataAry.size(); i++)
	{
		if (fabs(dataAry[i + 1].ml - dataAry[i].ml) < 0.1)
		{
			dataAry.erase(dataAry.begin() + i);
			i--;
		}
	}
}



