#pragma once
#include "Struct.h"
#include <vector>
#include "triDTM_road.h"
#include "ZDMHT1.h"


class RoadAutoDesign
{
public:
	//构造函数
	RoadAutoDesign(void);
	//析构函数
	~RoadAutoDesign(void);

	//主控函数
	void AutoDesign();

	//设置方案名，道路名函数
	void SetMdbName(CString MdbName);
	void SetRoadName(CString RoadName);

	//设置图框文件
	void SetFramePath(CString FramePath);
	void SetDrawOriCoor(double XCoor, double YCoor);
	void SetDataSource(bool IsFromMdb);

	//绘制改沟相关函数	20190822新增
	void AutoDrawGutter();
	void SetGutterPath(CString GutterPath);
	void SetGutterThick(double GutterThick);

	//页码，图名相关设置
	void SetTK(bool IsReadTK);

	//设置总体绘图参数
	void SetOverallDrawPara(DrawOverallPara OvalDrawPara);

	//设置平面绘图参数
	void SetHorizontalDrawPara(DrawHorizontalPara HDrawPara);

	//设置纵断面绘图参数
	void SetProfileDrawPara(DrawProfilePara VDrawPara);

	//设置横断面绘图参数
	void SetCrossDrawPara(DrawCrossPara CDrawPara);

	//设置横断面占地桩
	void SetCrossEarthPile(bool IsDrawCrossEarthPile);

	//设置指定桩号
	void SetCrossAssignedPileAttri(std::vector<CrossType>& CrossTypeArray);
private:
	//********************************成员变量********************************
	//****************基本参数****************
	//设计方案名，道路名
	CString m_MdbName;		//方案名
	CString m_RoadName;		//道路名
	JD_CENTER* m_pRoad;		//平面指针
	bool m_IsReadFromMdb;	//是否从数据库读取数据
	bool m_IsGutter;		//是否是改沟模式

	//****************平面计算参数****************
	//左右路面宽数组
	std::vector<LMKdata> m_RoadWidthArray_L, m_RoadWidthArray_R;	
	//超高标准
	std::vector<std::vector<double>> m_SurpArray;
	CString m_IG;	//路拱横坡
	CString m_MAXSuphI;	//最大超高
	//加宽标准
	std::vector<std::vector<double>> m_WidenArray;
	CString m_WidenType;	//加宽类型
	//超高计算参数
	CString	m_HxCgJbl;	//缓和段最小超高渐变率
	CString	m_ZxCgJbl;	//直线段超高渐变率
	int m_WidenMode;	//加宽方式
	int m_SuphMode;		//超高方式
	int m_SupNWidLenMode;	//取整方式
	//超高、加宽计算结果
	std::vector<CgSZ> m_SuphResultArray_L, m_SuphResultArray_R, m_WidenResultArray_L, m_WidenResultArray_R;

	//****************纵面计算参数****************
	//地面线表
	std::vector<DMXYS> m_EarthLine;
	//纵断面设计规范参数
	ProfileDesignStd m_PrfDesData;
	//坡度数组
	std::vector<PDTAB_ROAD> m_SlopeArray;
	//信封区数组
	std::vector<EnvPt> MaxEnvPtArray, MinEnvPtArray;	//信封区域上限点	信封区域下限点
	//用户设置的高程控制点
	std::vector<GCKZPt> m_UserSetGCKZDArray_SX, m_UserSetGCKZDArray_XX;	
	//设定的起终高程
	double m_StartH, m_EndH;
	//设定的起终里程
	double m_StartLC, m_EndLC; 
	//临时地面线分段数组
	std::vector<CRSPT> m_CRS_PT;
	//桥隧数组
	std::vector<BRITAB> m_BriArray,m_YHGetBriArray;
	std::vector<TUNTAB> m_TunArray, m_YHGetTunArray;
	//是否绘制特征桩	20190829
	bool m_IsDrawFeaturePile;
	//控制标高数组
	std::vector<GCKZPt> m_LevelPtArray;	
	//接坡信息数组
	std::vector<JPInfor>m_JPInforArray;
	//坡度数组 m_SlopeArray(初始方案) m_BPDArray(接坡点约束)m_LastPDArray(最终方案)
	std::vector<PDTAB_ROAD>m_BPDArray, m_LastPDArray; 
	//是否需要根据地面线形成初始方案
	bool Is_NeedInitPro;
	//****************横断面计算参数****************
	//松方压实系数
	double m_YSXS[6];
	//填方分类
	std::vector<LDTLdata> m_FillType;
	//用地面积汇总分段
	std::vector<YongDiFDB> m_EarthSection;
	//用地类型表
	std::vector<YongDiLXB> m_EarthType;
	//挖方分类
	std::vector<WFFLdata> m_CutType;
	//关联断面路肩拉直
	std::vector<GLDMLZdata> m_GLDMLZ;
	//关联断面路堑边坡推平
	std::vector<GLDMLQBPTPdata> m_GLDMLQBPTP;
	//关联断面线间沟
	std::vector<GLDMXJGdata> m_GLDMXJG;
	//清表土厚度
	std::vector<hthddata> m_HTHD_L, m_HTHD_R;
	//沟底坡
	std::vector<GouPDdata> m_GouPD_L, m_GouPD_R;
	//超填宽度
	std::vector<ctkddata> m_CTKD_L, m_CTKD_R;
	//弧形边坡
	std::vector<arcbpata> m_ARCBP_L, m_ARCBP_R;
	//挡墙
	std::vector<dqdata> m_DQ_L, m_DQ_R;
	//低填路堤自动填平
	std::vector<LDTPData> m_LDTP_L, m_LDTP_R;
	//低挖路堑自动推平
	std::vector<LQTPData> m_LQTP_L, m_LQTP_R;
	//低填路堤自动加宽土路肩
	std::vector<TLJJKData> m_TLJJK_L, m_TLJJK_R;
	//路面厚度
	std::vector<LMHdata> m_LMHD_L, m_LMHD_R;
	//地面线内插宽
	std::vector<crswiddata> m_CRSWID_L, m_CRSWID_R;
	//横断面限制宽度
	std::vector<dmdata> m_DM_L, m_DM_R;
	//用地加宽
	std::vector<pwddata> m_PWD_L, m_PWD_R;
	//路堤边坡
	std::vector<LDBPdata> m_LDBP_L, m_LDBP_R;
	//路堑边坡
	std::vector<LDBPdata> m_LQBP_L, m_LQBP_R;
	//排水沟
	std::vector<LDSGdata> m_LDSG_L, m_LDSG_R;
	//侧沟
	std::vector<LQSGdata> m_LQSG_L, m_LQSG_R;
	//天沟
	std::vector<TGdata> m_TG_L, m_TG_R;

	//桩号序列
	double m_StationStep;	//步长
	bool m_Sta_IsAddTZZ, m_Sta_IsAddBri, m_Sta_IsAddTun;	//是否增加特征桩、桥梁、隧道
	std::vector<LCBdata> m_StationSequence;	//桩号序列数组

	//横断面地面线
	std::vector<HDMDMXdata_AutoDesign> m_CrossEarthLine;

	//横断面绘制用地桩
	bool m_IsDrawCrossEarthPile;
	//手动设置指定桩号数组
	std::vector<CrossType> m_CrossTypeArray;	

	//绘图参数
	double m_OriX, m_OriY, m_CurX, m_CurY;	//给定绘图原点X、Y	当前绘图原点X、Y
	CString m_FramePath;	//图框文件路径
	DrawOverallPara m_OvalDrawPara;	//总体绘图参数
	DrawHorizontalPara m_HDrawPara;	//平面绘图参数
	DrawProfilePara m_VDrawPara;	//纵面绘图参数
	DrawCrossPara m_CDrawPara;		//横断面绘图参数
	double m_Frame_Height, m_Frame_Width;	//图框长宽参数
	int m_TKPos;	//绘图位置
	int m_TrimFlag;	//打断方式 = 0;布局方式 =1 
	
	double m_PageLen, m_PageHeight;	//页码长，页码宽
	bool m_IsreadTK; //是否读取图框
	std::vector<int> m_DrawingNum;	//每种图的数量		0-总体图	1-平面图	2-用地图	3-纵断面图	4-横断面图	5-曲线要素图	6-路基大样图

	//改沟绘图参数
	CString m_GutterPath;
	double m_GutterThick;

	//********************************零、基础部分********************************
	void GetNEraseLine();

	//********************************一、平面部分********************************
	//****************1.计算道路宽度****************
	//计算道路宽度
	void CalRoadWidth();
	//根据规范计算道路宽度
	void CalRoadWidthByCode(int LorR, std::vector<LMKdata>& RoadWidthArray);
	//****************2.设置超高、加宽标准****************
	//设置超高标准
	void SetSuphStd();
	void CalRoadSuphStdByCode();
	//设置加宽标准
	void SetWidenStd();
	void CalRoadWidenStdByCode();
	//****************3.计算超高、加宽结果数据****************
	//获取超高标准数据
	void SetSuphCalPara();
	//计算超高、加宽结果数据
	void CalSuphNWiden();
	//********************************二、纵断面部分********************************
	//****************4.数模内插地面线****************
	bool GetEarthLine();	
	bool CalEarthLine();
	//****************5.自动计算坡度数组****************
	//*********主要流程函数*********
	//计算坡度数组，主控函数
	void CalSlopeData();
	//计算平顺地面线的三阶差商
	void GetThreeDiffQuotientPt(std::vector<DMXYS>& EarthLine, std::vector<DMXYS>& SmoothedEarthLine, std::vector<DMXYS>& ThreeDiffQuotientPt);
	//计算一阶差商
	void GetOneDiffQuotientPt(std::vector<DMXYS>& ThreeDiffQuotientPt, std::vector<DMXYS>& OneDiffQuotientPt);
	//获取变坡点
	void GetChangePt(JD_CENTER* pRoad, std::vector<DMXYS>& OneDiffQuotientPt, std::vector<DMXYS>& ChangePt);
	//初始化坡度数组
	void IniSlopeArrray(std::vector<DMXYS>& ChangePt);
	//调整初始方案的分段
	void JudgeSection();
	//生成各变坡点标高
	bool CreatPlanMiu();
	//形成最终纵断面方案
	void FormFinalSlope();
	//*********计算函数*********
	//平顺地面线
	void SmoothEarthline(double MinSlopeLen, int SmoothNum, std::vector<DMXYS>& EarthLine, std::vector<DMXYS>& SmoothedEarthLine);
	//获取平顺后的高程
	double GetSmoothedHigh(int i, double MinSlopeLen, std::vector<DMXYS>& EarthLine);
	double GetSmoothedHigh(int i, double MinSlopeLen, int tp, std::vector<DMXYS>& EarthLine);
	//生成第索引为SlopePtIndex的变坡点设计高程
	bool CreateSloptPtDesH(int SlopePtIndex);
	//调整高程控制点
	void AdjustGCKZD(double StartCml, double StartH, double EndCml, double EndH);
	//由高程控制点形成信封区域
	void GetEnvelop(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry);
	//获取交点
	int GetJD(double Scml, double SLever, double Ecml, double ELever,
		double& MAX_cml, double& MAX_H, double& MIN_cml, double& MIN_H);
	//插入一组高程控制点
	void InsertGCKZDArray(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry,
		std::vector<GCKZPt>& GCKZDArray, bool IsSX);
	//得到信封区域所确定的里程范围
	bool GetEnvelopRange(double Mileage, double& MinH, double& MaxH);
	bool GetEnvelopRange(double PreDesH, double PreMileage, double Mileage, double& MinH, double& MaxH);
	//查找高程控制点在数组中的位置
	void InsertGCKZD(std::vector<GCKZPt>& GCKZDArry, GCKZPt KZD);
	//计算边坡点高程范围
	bool CalSloptPtDesHRange(int SlopePtIndex, double& minH, double& maxH);
	//传入期望、方差、取值范围，得到按正态分布的随机数
	double NormalRandom(double miu, double sigma, double min, double max);
	//得到均匀分布随机数
	double AverageRandom(double min, double max);
	//计算x的概率密度函数
	double Normal(double x, double miu, double sigma);
	//根据地面线计算填挖方最小高程
	void Env_GetMiu(int PDNum, double& Miu);
	//获取地面线
	void GetDMX1(double StratM, double EndM);
	//坡度取整
	void RoundSlope();
	//合并坡度
	void MergeSlope();
	//判断Profile方案的SlopetIndex变坡点能否被删除
	bool SlopePtCanDel(int StartIndex, int EndIndex);
	//根据是否有接坡信息获取起终变坡点信息
	void GetBPDStAndEdfromJP(int &BPDnum,JD_CENTER*pzLinep);
	//求交自动上变坡点
	bool AutoCalPVI(double &ml, double &H);
	//获取优化起终里程
	void GetStAndEdLC(double &StartLC, double &EndLC, double &StLevel, double &EdLevel,JD_CENTER*pzLinep);
	//变坡点与交叉点数据排序
	template <typename compara> void SortData(vector<compara> &dataAry);
	//********************************三、横断面部分********************************
	//****************6.设置基本参数****************
	//主控函数
	void SetCrossParameter();
	//设置松方压实系数
	void SetYSYXPara();
	//设置填方分类
	void SetFillTypePara();
	//设置用地面积汇总分段表
	void SetEarthSectionPara();
	//设置用地类型表
	void SetEarthTypePara();
	//设置挖方分类
	void SetCutTypePara();
	//设置关联断面路肩拉直
	void SetGLDMLZPara();
	//设置关联断面路堑边坡推平
	void SetGLDMLQBPTPPara();
	//设置关联断面线间沟
	void SetGLDMXJGPara();
	//设置清表土厚度
	void SetHTHDPara(int LorR);
	//设置沟底坡
	void SetGouPDPara(int LorR);
	//设置超填宽度表
	void SetCTKDPara(int LorR);
	//设置弧形边坡表
	void SetARCBPPara(int LorR);
	//设置挡墙
	void SetDQPara(int LorR);
	//设置低填路堤自动填平
	void SetLDTPPara(int LorR);
	//设置低挖路堑自动推平
	void SetLQTPPara(int LorR);
	//设置低填路堤自动加宽土路肩
	void SetTLJJKPara(int LorR);
	//设置路面厚度
	void SetLMHDPara(int LorR);
	//设置地面线内插宽
	void SetCRSWIDPara(int LorR);
	//设置横断面限制宽度
	void SetDMPara(int LorR);
	//设置用地加宽
	void SetPWDPara(int LorR);
	//设置路堤边坡
	void SetLDBPPara(int LorR);
	//设置路堑边坡
	void SetLQBPPara(int LorR);
	//设置排水沟
	void SetLDSGPara(int LorR);
	//设置侧沟
	void SetLQSGPara(int LorR);
	//设置天沟
	void SetTGPara(int LorR);
	//****************7.计算桩号序列****************
	void CalStationSequence();
	//****************8.自动提取地面线****************
	//计算横断面地面线
	void CalCrossEarthLine();
	//计算横断面路面宽
	double CalCrossRoadWidth(double dml, std::vector<crswiddata> CRSWID);
	//写入横断面地面线文件
	void WriteCrossEarthFile();
	//写入占地宽文件
	void WriteZdkHdkFile_AutoDesign();
	//****************9.横断面自动设计****************
	void CrossAutoDesgin();
	void RefreshTSFAndYDArea();
	//****************10.绘平面图****************
	//绘制线路平面图
	void DrawHorizontalRoad();
	//绘制线路中线
	void DrawCenterLine();
	//绘制线路输出图（带图框，地形）
	void DrawHorizontalOutput();
	//绘制线路输出图(打断)
	void DrawHorizontalOutput_Trimflag_D();
	//绘制线路输出图(布局)
	void DrawHorizontalOutput_Trimflag_B(STuKuang *OriTK,int TKnum);
	void TransFormTK_PM(STuKuang *TK, double CenX, double CenY, double scale = 1.0);
	//读取平面绘图参数
	void ReadDrawHorizontalPara();
	//读取平面参数文件
	bool ReadCSFile(CString FilePath);
	//写入平面参数文件
	bool WriteCSFile(CString FilePath);
	//读取分幅输出参数
	bool ReadDrawSetFile(CString FilePath);
	//写入分幅输出参数
	bool WriteDrawSetFile(CString FilePath);
	//修改底图颜色与图层	20190918
	void ModifyBaseMap(ads_point& LeftBottom, ads_point& RightTop);
	//计算平面图幅
	void CalHoriDrawingNum();
	//****************11.绘用地图****************
	//绘制用地图
	void DrawHEarth();
	//****************12.绘纵断面图****************
	//绘制纵断面图
	void DrawProfile();
	//设置纵断面绘图参数
	void SetProfileDrawPara();
	//设置五线谱参数
	void SetWxpPara(CZDMHT& zdmdraw);
	//****************13.绘横断面图****************
	//绘横断面图
	void DrawCross();
	//****************14.绘曲线要素表****************
	//绘曲线要素表
	void DrawCurveInfor();
	void DrawCurInforOutput(double CenX, double CenY, CString s_ckml, CString e_ckml, CString TKname);//布局出图
	//删除实体
	int del_ent(double Wtab, double Htab, double ALPHA, ads_point Ppt, double DFACTOR);
	void ptAddx(ads_point p1, double angx, double x, ads_point pt);
	void ptAddy(ads_point p1, double angx, double y, ads_point pt);
	int wr_cmf_c(double ALPHA, ads_point Ppt, double DFACTOR, double Wtab, double Htab, int JDn, int SIP);
	int wr_cmf_s(double ALPHA, ads_point Ppt, double DFACTOR, double Wtab, double Htab, int JDn, int SIP);
	void LineUD(ads_point p1, ads_point p2, double angx, double dist, double lw);
	void LineLR(ads_point p1, ads_point p2, double angx, double dist, double lw, int color = 2);
	void wr_text(ads_point pt, double angx, double d, double wk, double ht, ACHAR* str);
	void MidPoint(ads_point pt, double w, double h, double angx, ads_point Mpt);
	//****************15.路基大样图****************
	void DrawRoadbed();
	void DrawRoadbedOutput(double CenX, double CenY, CString s_ckml, CString e_ckml, CString TKname);//布局出图
	void TransFormTK_Roadbed(STuKuang *TK, double CenX, double CenY, double scale = 1.0);
	//****************16.平面总体图****************
	void DrawOverallHoriRoad();

	//****************98.标注页码和图名****************
	void NotePage();
	void NoteDrawingName();
	
	//****************99.出改沟图****************
	//绘改沟纵断面图
	void DrawProfile_Gutter();
	//设置五线谱参数
	void SetWxpPara_Gutter(CZDMHT& zdmdraw);
	//绘改沟横断面图
	void DrawCross_Gutter();

public:
		BOOL m_IsDrawHengDuanMianWangGe;// 是否绘制横断面网格
		double CTsml, CTeml;
		bool if_PL_CT;
		bool isAllinView;
};

