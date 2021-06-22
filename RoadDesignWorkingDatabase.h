#pragma once
#include <vector>
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "BAS_DRAW_FUN.h"

struct RoadDataBase
{
	//道路信息
	CString MdbName;	//方案名
	CString RoadName;	//道路名
	CString Grade;		//道路等级
	int DesignVelocity;	//设计速度

	//平面绘图参数
	DrawHorizontalPara HDrawPara;

	//路面宽数组
	std::vector<LMKdata> Left_Road_Width_Array;
	std::vector<LMKdata> Right_Road_Width_Array;

	//超高标准数组
	std::vector<std::vector<double>> SurpArray;
	CString IG;	//路拱横坡
	CString MAXSuphI;	//最大超高

	//加宽标准数组
	std::vector<std::vector<double>> WidenArray;
	CString WidenType;

	//超高计算参数
	CString	HxCgJbl;	//缓和段最小超高渐变率
	CString	ZxCgJbl;	//直线段超高渐变率
	int WidenMode;		//加宽方式
	int SuphMode;		//超高方式
	int SupNWidLenMode;	//取整方式
public:
	//超高、加宽计算结果数据
	std::vector<CgSZ> SuphResultArray_L, SuphResultArray_R, WidenResultArray_L, WidenResultArray_R;

	//地面线数据
	std::vector<DMXYS> EarthLine;

	//坡度数据
	std::vector<PDTAB_ROAD> SlopeArray;

	//桥隧数据
	std::vector<BRITAB> BriArray;
	std::vector<TUNTAB> TunArray;

	//控制点标高数据	20190919
	std::vector<GCKZPt> LevelCtrlPtArray;

	std::vector<JPInfor>JPArray;
	//20190809	横断面数据
	//横断面基本参数数据
	//松方压实系数
	double YSXS[6];
	//填方分类
	std::vector<LDTLdata> FillType;
	//用地面积汇总分段
	std::vector<YongDiFDB> EarthSection;
	//用地类型表
	std::vector<YongDiLXB> EarthType;
	//挖方分类
	std::vector<WFFLdata> CutType;
	//关联断面路肩拉直
	std::vector<GLDMLZdata> GLDMLZ;
	//关联断面路堑边坡推平
	std::vector<GLDMLQBPTPdata> GLDMLQBPTP;
	//关联断面线间沟
	std::vector<GLDMXJGdata> GLDMXJG;
	//清表土厚度
	std::vector<hthddata> HTHD_L, HTHD_R;
	//沟底坡
	std::vector<GouPDdata> GouPD_L, GouPD_R;
	//超填宽度
	std::vector<ctkddata> CTKD_L, CTKD_R;
	//弧形边坡
	std::vector<arcbpata> ARCBP_L, ARCBP_R;
	//挡墙
	std::vector<dqdata> DQ_L, DQ_R;
	//低填路堤自动填平
	std::vector<LDTPData> LDTP_L, LDTP_R;
	//低挖路堑自动推平
	std::vector<LQTPData> LQTP_L, LQTP_R;
	//低填路堤自动加宽土路肩
	std::vector<TLJJKData> TLJJK_L, TLJJK_R;
	//路面厚度
	std::vector<LMHdata> LMHD_L, LMHD_R;
	//地面线内插宽
	std::vector<crswiddata> CRSWID_L, CRSWID_R;
	//横断面限制宽度
	std::vector<dmdata> DM_L, DM_R;
	//用地加宽
	std::vector<pwddata> PWD_L, PWD_R;
	//路堤边坡
	std::vector<LDBPdata> LDBP_L, LDBP_R;
	//路堑边坡
	std::vector<LDBPdata> LQBP_L, LQBP_R;
	//排水沟
	std::vector<LDSGdata> LDSG_L, LDSG_R;
	//侧沟
	std::vector<LQSGdata> LQSG_L, LQSG_R;
	//天沟
	std::vector<TGdata> TG_L, TG_R;
	//路面分层
	std::vector<RoadLayerData> RoadLayer_L, RoadLayer_R;

	//桩号序列参数
	std::vector<LCBdata> StationSequence;

	RoadDataBase()
	{
		MdbName.Empty();
		RoadName.Empty();
		Grade.Empty();
		DesignVelocity = 0;

		Left_Road_Width_Array.clear();
		Right_Road_Width_Array.clear();

		SurpArray.clear();
		IG.Empty();
		MAXSuphI.Empty();

		WidenArray.clear();
		WidenType.Empty();

		HxCgJbl.Empty();
		ZxCgJbl.Empty();
		WidenMode = 0;
		SuphMode = 0;
		SupNWidLenMode = 0;

		SuphResultArray_L.clear();
		SuphResultArray_R.clear();
		WidenResultArray_L.clear();
		WidenResultArray_R.clear();

		EarthLine.clear();
		SlopeArray.clear();
		BriArray.clear();
		TunArray.clear();
		LevelCtrlPtArray.clear();
		JPArray.clear();

		FillType.clear();
		EarthSection.clear();
		EarthType.clear();
		CutType.clear();
		GLDMLZ.clear();
		GLDMLQBPTP.clear();
		GLDMXJG.clear();
		HTHD_L.clear();
		HTHD_R.clear();
		GouPD_L.clear();
		GouPD_R.clear();
		CTKD_L.clear();
		CTKD_R.clear();
		ARCBP_L.clear();
		ARCBP_R.clear();
		DQ_L.clear();
		DQ_R.clear();
		LDTP_L.clear();
		LDTP_R.clear();
		LQTP_L.clear();
		LQTP_R.clear();
		TLJJK_L.clear();
		TLJJK_R.clear();
		LMHD_L.clear();
		LMHD_R.clear();
		CRSWID_L.clear();
		CRSWID_R.clear();
		DM_L.clear();
		DM_R.clear();
		PWD_L.clear();
		PWD_R.clear();
		LDBP_L.clear();
		LDBP_R.clear();
		LQBP_L.clear();
		LQBP_R.clear();
		LDSG_L.clear();
		LDSG_R.clear();
		LQSG_L.clear();
		LQSG_R.clear();
		TG_L.clear();
		TG_R.clear();
		RoadLayer_L.clear();
		RoadLayer_R.clear();

		StationSequence.clear();
	}
};



class RoadDesignWorkingDatabase
{
private:
	/*-------------------------成员变量--------------------------*/
	//****************1.基础成员变量****************
	XLDataBase m_DataBase;	//数据库读写成员
	//****************2.成员变量****************
	std::vector<JD_CENTER*> m_RoadArray;		//道路平面指针数组
public:
	std::vector<RoadDataBase> m_RoadDatabaseArray;	//存储所有道路方案信息的数组

public:
	/*-------------------------成员函数（公开）--------------------------*/
	//****************1.基本函数****************
	//构造函数
	RoadDesignWorkingDatabase(void);
	//析构函数
	~RoadDesignWorkingDatabase(void);
	//保存数据到数据库
	void SaveAllData(CString MdbName, CString RoadName = L"主线");
	//删除活动数据库
	bool DeleteWorkingDatabase(CString MdbName, CString RoadName);
	//****************2.平面函数****************
	//创建道路平面指针（对外接口）
	JD_CENTER* CreateHorizontalRoad(CString MdbName, CString RoadName = L"主线", bool IsReadFromMdb = false);
	//设置道路平面指针
	void SetHorizontalRoad(JD_CENTER* pRoad);
	//获取道路等级和设计速度参数
	bool GetGradeAndVelocity(CString MdbName, CString RoadName, CString& Grade, int& DesignVelocity);

	//获取路面宽数据
	bool GetRoadWidthArray(CString MdbName, int LorR, std::vector<LMKdata>& RoadWidthArray, CString RoadName = L"主线");
	//设置路面宽数据
	void SetRoadWidthArray(CString MdbName, CString RoadName, int LorR, std::vector<LMKdata> RoadWidthArray, bool IsSaveToMdb = false);

	//获取道路超高标准数据
	bool GetRoadSuphStd(CString MdbName, CString RoadName, std::vector<std::vector<double>>& SurpArray, CString& IG, CString& MAXSuphI);
	//设置道路超高标准数据
	void SetRoadSuphStd(CString MdbName, CString RoadName, std::vector<std::vector<double>> SurpArray, CString IG, CString MAXSuphI, bool IsSaveToMdb = false);

	//获取道路加宽标准数据
	bool GetRoadWidenStd(CString MdbName, CString RoadName, std::vector<std::vector<double>>& WidenArray, CString& WidenType);
	//设置道路加宽标准数据
	void SetRoadWidenStd(CString MdbName, CString RoadName, std::vector<std::vector<double>> WidenArray, CString WidenType, bool IsSaveToMdb = false);

	//获取道路超高计算数据
	bool GetSuphAndWidenPara(CString MdbName, CString RoadName, 
		CString& HxCgJbl, CString& ZxCgJbl, int& WidenMode, int& SuphMode, int& SupNWidLenMode);
	//设置道路超高计算数据
	void SetSuphAndWidenPara(CString MdbName, CString RoadName,
		CString HxCgJbl, CString ZxCgJbl, int WidenMode, int SuphMode, int SupNWidLenMode, bool IsSaveToMdb = false);

	//获取道路超高计算结果
	bool GetSuphResult(CString MdbName, CString RoadName, int LorR, std::vector<CgSZ>& SuphResultArray);
	//设置道路超高计算结果
	void SetSuphResult(CString MdbName, CString RoadName, int LorR, std::vector<CgSZ> SuphResultArray, bool IsSaveToMdb = false);

	//获取道路加宽计算结果
	bool GetWidenResult(CString MdbName, CString RoadName, int LorR, std::vector<CgSZ>& WidenResultArray);
	//设置道路加宽计算结果
	void SetWidenResult(CString MdbName, CString RoadName, int LorR, std::vector<CgSZ> WidenResultArray, bool IsSaveToMdb = false);

	//获取平面绘图参数
	bool GetHorizontalDarwPara(CString MdbName, CString RoadName, DrawHorizontalPara& HDrawPara);
	void SetHorizontalDrawPara(CString MdbName, CString RoadName, DrawHorizontalPara HDrawPara, bool IsSaveToFile = false);

	//****************3.纵断面函数****************
	//获取纵断面地面线数据
	bool GetEarthLine(CString MdbName, CString RoadName, std::vector<DMXYS>& EarthLine);
	//设置纵断面地面线数据
	void SetEarthLine(CString MdbName, CString RoadName, std::vector<DMXYS> EarthLine, bool IsSaveToMdb = false);

	//获取纵断面坡度数据
	bool GetSlopeData(CString MdbName, CString RoadName, std::vector<PDTAB_ROAD>& SlopeArray);
	//设置纵断面坡度数据
	void SetSlopeData(CString MdbName, CString RoadName, std::vector<PDTAB_ROAD> SlopeArray, bool IsSaveToMdb = false);

	//获取桥梁数据
	bool GetBriData(CString MdbName, CString RoadName, std::vector<BRITAB>& BriArray);
	//设置桥梁数据
	void SetBriData(CString MdbName, CString RoadName, std::vector<BRITAB> BriArray, bool IsSaveToMdb = false);

	//获取隧道数据
	bool GetTunData(CString MdbName, CString RoadName, std::vector<TUNTAB>& TunArray);
	//设置隧道数据
	void SetTunData(CString MdbName, CString RoadName, std::vector<TUNTAB> TunArray, bool IsSaveToMdb = false);

	//获取控制标高数据
	bool GetLevelCtrlData(CString MdbName, CString RoadName, std::vector<GCKZPt>& LevelCtrlPtArray);
	//设置设置标高数据
	void SetLevelCtrlData(CString MdbName, CString RoadName, std::vector<GCKZPt> LevelCtrlPtArray, bool IsSaveToMdb = false);

	//获取接坡信息数据
	bool GetJPData(CString MdbName, CString RoadName, std::vector<JPInfor>& JPArray);
	//设置接坡信息数据
	void SetJPData(CString MdbName, CString RoadName, std::vector<JPInfor> JPArray, bool IsSaveToMdb = false);

	//****************4.横断面函数****************
	//获取松方压实系数
	bool GetSFYSXS(CString MdbName, CString RoadName, double* YSXS);
	//设置松方压实系数
	void SetSFYSXS(CString MdbName, CString RoadName, double* YSXS, bool IsSaveToMdb = false);

	//获取填方分类
	bool GetFillType(CString MdbName, CString RoadName, std::vector<LDTLdata>& FillType);
	//设置填方分类
	void SetFillType(CString MdbName, CString RoadName, std::vector<LDTLdata> FillType, bool IsSaveToMdb = false);

	//获取用地面积汇总分段表
	bool GetYongDiFDB(CString MdbName, CString RoadName, std::vector<YongDiFDB>& EarthSection);
	//设置用地面积汇总分段表
	void SetYongDiFDB(CString MdbName, CString RoadName, std::vector<YongDiFDB> EarthSection, bool IsSaveToMdb = false);

	//获取用地类型表
	bool GetEarthType(CString MdbName, CString RoadName, std::vector<YongDiLXB>& EarthType);
	//设置用地类型表
	void SetEarthType(CString MdbName, CString RoadName, std::vector<YongDiLXB> EarthType, bool IsSaveToMdb = false);

	//获取挖方分类
	bool GetCutType(CString MdbName, CString RoadName, std::vector<WFFLdata>& CutType);
	//设置挖方分类
	void SetCutType(CString MdbName, CString RoadName, std::vector<WFFLdata> CutType, bool IsSaveToMdb = false);

	//获取关联断面路肩拉直
	bool GetGLDMLZ(CString MdbName, CString RoadName, std::vector<GLDMLZdata>& GLDMLZ);
	//设置关联断面路肩拉直
	void SetGLDMLZ(CString MdbName, CString RoadName, std::vector<GLDMLZdata> GLDMLZ, bool IsSaveToMdb = false);

	//获取关联断面路堑边坡推平
	bool GetGLDMLQBPTP(CString MdbName, CString RoadName, std::vector<GLDMLQBPTPdata>& GLDMLQBPTP);
	//设置关联断面路堑边坡推平
	void SetGLDMLQBPTP(CString MdbName, CString RoadName, std::vector<GLDMLQBPTPdata> GLDMLQBPTP, bool IsSaveToMdb = false);

	//获取关联断面线间沟
	bool GetGLDMXJG(CString MdbName, CString RoadName, std::vector<GLDMXJGdata>& GLDMXJG);
	//设置关联断面线间沟
	void SetGLDMXJG(CString MdbName, CString RoadName, std::vector<GLDMXJGdata> GLDMXJG, bool IsSaveToMdb = false);

	//获取清表土厚度
	bool GetHTHD(CString MdbName, CString RoadName, int LorR, std::vector<hthddata>& HTHD);
	//设置清表土厚度
	void SetHTHD(CString MdbName, CString RoadName, int LorR, std::vector<hthddata> HTHD, bool IsSaveToMdb = false);

	//获取沟底坡
	bool GetGouPD(CString MdbName, CString RoadName, int LorR, std::vector<GouPDdata>& GouPD);
	//设置沟底坡
	void SetGouPD(CString MdbName, CString RoadName, int LorR, std::vector<GouPDdata> GouPD, bool IsSaveToMdb = false);

	//获取超填宽度表
	bool GetCTKD(CString MdbName, CString RoadName, int LorR, std::vector<ctkddata>& CTKD);
	//设置超填宽度表
	void SetCTKD(CString MdbName, CString RoadName, int LorR, std::vector<ctkddata> CTKD, bool IsSaveToMdb = false);

	//获取弧形边坡表
	bool GetARCBP(CString MdbName, CString RoadName, int LorR, std::vector<arcbpata>& ARCBP);
	//设置弧形边坡表
	void SetARCBP(CString MdbName, CString RoadName, int LorR, std::vector<arcbpata> ARCBP, bool IsSaveToMdb = false);

	//获取挡墙
	bool GetDQ(CString MdbName, CString RoadName, int LorR, std::vector<dqdata>& DQ);
	//设置挡墙
	void SetDQ(CString MdbName, CString RoadName, int LorR, std::vector<dqdata> DQ, bool IsSaveToMdb = false);

	//获取低填路堤自动填平
	bool GetLDTP(CString MdbName, CString RoadName, int LorR, std::vector<LDTPData>& LDTP);
	//设置低填路堤自动填平
	void SetLDTP(CString MdbName, CString RoadName, int LorR, std::vector<LDTPData> LDTP, bool IsSaveToMdb = false);

	//获取低挖路堑自动推平
	bool GetLQTP(CString MdbName, CString RoadName, int LorR, std::vector<LQTPData>& LQTP);
	//设置低挖路堑自动推平
	void SetLQTP(CString MdbName, CString RoadName, int LorR, std::vector<LQTPData> LQTP, bool IsSaveToMdb = false);

	//获取低填路堤自动加宽土路肩
	bool GetTLJJK(CString MdbName, CString RoadName, int LorR, std::vector<TLJJKData>& TLJJK);
	//设置低填路堤自动加宽土路肩
	void SetTLJJK(CString MdbName, CString RoadName, int LorR, std::vector<TLJJKData> TLJJK, bool IsSaveToMdb = false);

	//获取路面厚度
	bool GetLMHD(CString MdbName, CString RoadName, int LorR, std::vector<LMHdata>& LMHD);
	//设置路面厚度
	void SetLMHD(CString MdbName, CString RoadName, int LorR, std::vector<LMHdata> LMHD, bool IsSaveToMdb = false);

	//获取地面线内插宽
	bool GetCRSWID(CString MdbName, CString RoadName, int LorR, std::vector<crswiddata>& CRSWID);
	//设置地面线内插宽
	void SetCRSWID(CString MdbName, CString RoadName, int LorR, std::vector<crswiddata> CRSWID, bool IsSaveToMdb = false);

	//获取横断面限制宽度
	bool GetDM(CString MdbName, CString RoadName, int LorR, std::vector<dmdata>& DM);
	//设置横断面限制宽度
	void SetDM(CString MdbName, CString RoadName, int LorR, std::vector<dmdata> DM, bool IsSaveToMdb = false);

	//获取用地加宽
	bool GetPWD(CString MdbName, CString RoadName, int LorR, std::vector<pwddata>& PWD);
	//设置用地加宽
	void SetPWD(CString MdbName, CString RoadName, int LorR, std::vector<pwddata> PWD, bool IsSaveToMdb = false);

	//获取路堤边坡
	bool GetLDBP(CString MdbName, CString RoadName, int LorR, std::vector<LDBPdata>& LDBP);
	//设置路堤边坡
	void SetLDBP(CString MdbName, CString RoadName, int LorR, std::vector<LDBPdata> LDBP, bool IsSaveToMdb = false);

	//获取路堑边坡
	bool GetLQBP(CString MdbName, CString RoadName, int LorR, std::vector<LDBPdata>& LQBP);
	//设置路堑边坡
	void SetLQBP(CString MdbName, CString RoadName, int LorR, std::vector<LDBPdata> LQBP, bool IsSaveToMdb = false);

	//获取排水沟
	bool GetLDSG(CString MdbName, CString RoadName, int LorR, std::vector<LDSGdata>& LDSG);
	//设置排水沟
	void SetLDSG(CString MdbName, CString RoadName, int LorR, std::vector<LDSGdata> LDSG, bool IsSaveToMdb = false);

	//获取侧沟
	bool GetLQSG(CString MdbName, CString RoadName, int LorR, std::vector<LQSGdata>& LQSG);
	//设置侧沟
	void SetLQSG(CString MdbName, CString RoadName, int LorR, std::vector<LQSGdata> LQSG, bool IsSaveToMdb = false);

	//获取天沟
	bool GetTG(CString MdbName, CString RoadName, int LorR, std::vector<TGdata>& TG);
	//设置天沟
	void SetTG(CString MdbName, CString RoadName, int LorR, std::vector<TGdata> TG, bool IsSaveToMdb = false);

	//获取路面分层
	bool GetRoadLayer(CString MdbName, CString RoadName, int LorR, std::vector<RoadLayerData>& RoadLayerArray);
	//设置路面分层
	void SetRoadLayer(CString MdbName, CString RoadName, int LorR, std::vector<RoadLayerData> RoadLayerArray, bool IsSaveToMdb = false);

	//获取桩号序列
	bool GetSationSequence(CString MdbName, CString RoadName, std::vector<LCBdata>& StationSequence);
	//设置桩号序列
	void SetSationSequence(CString MdbName, CString RoadName, std::vector<LCBdata> StationSequence, bool IsSaveToMdb = false);

private:
	/*-------------------------成员函数（私有）--------------------------*/
	//****************1.基本函数****************
	//活动数据库是否存在
	bool Is_Database_Exist(CString MdbName, CString RoadName, int& Road_Index);
	//活动数据库是否存在并（如果不存在则）创建
	int Is_Database_Exist_And_Create(CString MdbName, CString RoadName);
	//创建活动数据库
	void CreateDatabase(CString MdbName, CString RoadName);

	//****************2.平面函数****************
	//道路设计标准
	bool Read_Road_Grade_And_Velocity(CString MdbName, CString RoadName);
	//******①平面指针******
	//道路平面指针是否存在
	bool Is_Horizontal_Road_Exist(CString MdbName, CString RoadName, int& Road_Index);
	//创建道路平面指针
	JD_CENTER* Create_Road_Pointer(CString MdbName, CString RoadName);
	//******②道路宽度******
	//读取道路宽数据
	bool Read_Road_Width_Data(CString MdbName, CString RoadName);
	//写入道路宽度数据
	void Write_Road_Width_Data(CString MdbName, CString RoadName);
	//******③超高标准******
	//读取超高标准数据
	bool Read_Road_Suph_Data(CString MdbName, CString RoadName);
	//写入超高标准数据
	void Write_Road_Suph_Data(CString MdbName, CString RoadName);
	//******④加宽标准******
	//读取加宽标准数据
	bool Read_Road_Widen_Data(CString MdbName, CString RoadName);
	//写入加宽标准数据
	void Write_Road_Widen_Data(CString MdbName, CString RoadName);
	//******⑤计算加宽和超高******
	//读取超高计算参数
	bool Read_Road_SuphCal_Data(CString MdbName, CString RoadName);
	//写入超高计算参数
	void Write_Road_SuphCal_Data(CString MdbName, CString RoadName);
	//读取超高、加宽结果数据
	bool Read_Suph_Result(CString MdbName, CString RoadName);
	bool Read_Widen_Result(CString MdbName, CString RoadName);
	//写入超高、加宽结果数据
	void Write_Suph_Result(CString MdbName, CString RoadName);
	void Write_Widen_Result(CString MdbName, CString RoadName);

	//平面绘图参数读写
	bool Read_Horizontal_Draw_Para(CString MdbName, CString RoadName);
	bool Read_Para_File(CString FileName, DrawHorizontalPara& HDrawPara);
	bool Write_Horizontal_Draw_Para(CString MdbName, CString RoadName);

	//****************3.纵断面函数****************
	//读取地面线数据
	bool Read_EarthLine(CString MdbName, CString RoadName);
	//写入地面线数据
	void Write_EarthLine(CString MdbName, CString RoadName);

	//读取坡度数据
	bool Read_Slope_Data(CString MdbName, CString RoadName);
	//写入坡度数据
	void Write_Slope_Data(CString MdbName, CString RoadName);

	//读取桥梁数据
	bool Read_Bridge(CString MdbName, CString RoadName);
	//写入桥梁数据
	void Write_Bridge(CString MdbName, CString RoadName);

	//读取隧道数据
	bool Read_Tunnel(CString MdbName, CString RoadName);
	//写入隧道数据
	void Write_Tunnel(CString MdbName, CString RoadName);

	//读取控制标高数据
	bool Read_LevelCtrlData(CString MdbName, CString RoadName);
	//写入控制标高数据
	void Write_LevelCtrlData(CString MdbName, CString RoadName);

	//读取接坡信息数据
	bool Read_JPData(CString MdbName, CString RoadName);
	//写入接坡信息数据
	void Write_JPData(CString MdbName, CString RoadName);


	//****************4.横断面函数****************
	//读取松方压实系数
	bool Read_SFYSXS(CString MdbName, CString RoadName);
	//写入松方压实系数
	void Write_SFYSXS(CString MdbName, CString RoadName);

	//读取填方分类系数
	bool Read_FillType(CString MdbName, CString RoadName);
	//写入填方分类系数
	void Write_FillType(CString MdbName, CString RoadName);

	//读取用地面积汇总分段表
	bool Read_YongDiFDB(CString MdbName, CString RoadName);
	//写入用地面积汇总分段表
	void Write_YongDiFDB(CString MdbName, CString RoadName);

	//读取用地类型表
	bool Read_EarthType(CString MdbName, CString RoadName);
	//写入用地类型表
	void Write_EarthType(CString MdbName, CString RoadName);

	//读取挖方分类
	bool Read_CutType(CString MdbName, CString RoadName);
	//写入挖方分类
	void Write_CutType(CString MdbName, CString RoadName);

	//读取关联断面路肩拉直
	bool Read_GLDMLZ(CString MdbName, CString RoadName);
	//写入关联断面路肩拉直
	void Write_GLDMLZ(CString MdbName, CString RoadName);

	//读取关联断面路堑边坡推平
	bool Read_GLDMLQBPTP(CString MdbName, CString RoadName);
	//写入关联断面路堑边坡推平
	void Write_GLDMLQBPTP(CString MdbName, CString RoadName);

	//读取关联断面线间沟
	bool Read_GLDMXJG(CString MdbName, CString RoadName);
	//写入关联断面线间沟
	void Write_GLDMXJG(CString MdbName, CString RoadName);

	//读取清表土厚度
	bool Read_HTHD(CString MdbName, CString RoadName);
	//写入清表土厚度
	void Write_HTHD(CString MdbName, CString RoadName);

	//读取沟底坡
	bool Read_GouPD(CString MdbName, CString RoadName);
	//写入沟底坡
	void Write_GouPD(CString MdbName, CString RoadName);

	//读取超填宽度表
	bool Read_CTKD(CString MdbName, CString RoadName);
	//写入超填宽度表
	void Write_CTKD(CString MdbName, CString RoadName);

	//读取弧形边坡表
	bool Read_ARCBP(CString MdbName, CString RoadName);
	//写入弧形边坡表
	void Write_ARCBP(CString MdbName, CString RoadName);

	//读取挡墙
	bool Read_DQ(CString MdbName, CString RoadName);
	//写入挡墙
	void Write_DQ(CString MdbName, CString RoadName);

	//读取低填路堤自动填平
	bool Read_LDTP(CString MdbName, CString RoadName);
	//写入低填路堤自动填平
	void Write_LDTP(CString MdbName, CString RoadName);

	//读取低挖路堑自动推平
	bool Read_LQTP(CString MdbName, CString RoadName);
	//写入低挖路堑自动推平
	void Write_LQTP(CString MdbName, CString RoadName);

	//读取低填路堤自动加宽土路肩
	bool Read_TLJJK(CString MdbName, CString RoadName);
	//写入低填路堤自动加宽土路肩
	void Write_TLJJK(CString MdbName, CString RoadName);
	
	//读取路面厚度
	bool Read_LMHD(CString MdbName, CString RoadName);
	//写入路面厚度
	void Write_LMHD(CString MdbName, CString RoadName);

	//读取地面线内插宽
	bool Read_CRSWID(CString MdbName, CString RoadName);
	//写入地面线内插宽
	void Write_CRSWID(CString MdbName, CString RoadName);

	//读取横断面限制宽度
	bool Read_DM(CString MdbName, CString RoadName);
	//写入横断面限制宽度
	void Write_DM(CString MdbName, CString RoadName);

	//读取用地加宽
	bool Read_PWD(CString MdbName, CString RoadName);
	//写入用地加宽
	void Write_PWD(CString MdbName, CString RoadName);

	//读取路堤边坡
	bool Read_LDBP(CString MdbName, CString RoadName);
	//写入路堤边坡
	void Write_LDBP(CString MdbName, CString RoadName);

	//读取路堑边坡
	bool Read_LQBP(CString MdbName, CString RoadName);
	//写入路堑边坡
	void Write_LQBP(CString MdbName, CString RoadName);

	//读取排水沟
	bool Read_LDSG(CString MdbName, CString RoadName);
	//写入排水沟
	void Write_LDSG(CString MdbName, CString RoadName);

	//读取侧沟
	bool Read_LQSG(CString MdbName, CString RoadName);
	//写入侧沟
	void Write_LQSG(CString MdbName, CString RoadName);

	//读取天沟
	bool Read_TG(CString MdbName, CString RoadName);
	//写入天沟
	void Write_TG(CString MdbName, CString RoadName);

	//读取路面分层
	bool Read_RoadLayer(CString MdbName, CString RoadName);
	//写入路面分层
	void Write_RoadLayer(CString MdbName, CString RoadName);

	//读取桩号序列
	bool Read_StationSequence(CString MdbName, CString RoadName);
	//写入桩号序列
	void Write_StationSequence(CString MdbName, CString RoadName);
};

