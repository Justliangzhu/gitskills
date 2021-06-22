#pragma once
#include "BAS_DRAW_FUN.H"
#include "GTZDM.H"
#include "Struct.h"
#include <vector>
#include "triDTM_road.h"
#include "XLDataBase.h"
#include "HdmDes.h"
class CProfileOpt : public BAS_DRAW_FUN
{
public:
	CProfileOpt();
	~CProfileOpt();

public:

	HdmDes m_CrossSection;//横断面对象
	GCKZPt *GCKZD;
	XLDataBase xlmdb;
	BAS_DRAW_FUN basdraw;
	ProfileDesignStd m_PrfDesData; 	//纵断面设计规范参数
	//[i][0]里程 [i][1]高程 [i][2]竖曲线 [i][3]坡度
	double PD_array[MAXBPDNUM][4], BPD_Array[MAXBPDNUM][4];
	//*********优化需传入数值参数*********
	JD_CENTER  *m_pPlain;  //优化全局平面指针
	//DMX_ROAD  *pdmx;//地面线指针
	std::vector<DMXYS> m_DMXLine; //地面线表
	std::vector<JPInfor>m_JPInforArray; //接坡信息数组
	std::vector<PDTAB_ROAD>m_SlopeArray, m_BPDArray; //坡度数组  m_SlopeArray(初始方案) m_BPDArray(接坡点约束)

	//*********优化需赋初始值参数*********
	int	m_PopulationSize;//种群规模，即方案总数
	int	m_SlopePtSum;//変坡点总数，包括起终点
	int m_EvoluGeneration;//进化代数
	double CrossProbablity;//交叉概率
	double MutationPrabality;//变异概率
	double m_CutCost;//挖方单价
	double m_FillCost;//填方单价
	double m_BridgeCost;//桥梁单价
	double m_TunnelCost;//隧道单价
	double	m_MinBriFill; //设桥最小填高
	double	m_MinBriLeth; //自动设桥最小桥长
	double	m_MinTunFill;//设隧最小挖深
	double	m_MinTunLeth;//自动设隧最小隧长
	double	m_TunFill; //线隧分界填高
	double	m_BriFill; //桥线分界填高
	BOOL m_IsSetBaseBriHigh;//是否设置桥梁限制高度
	BOOL m_IsSetBaseTunLen;//是否设置隧道限制长度
	double m_BaseBriHigh;//桥梁基本高度
	double m_BaseTunLen;//隧道基本长度
	double m_StaMaxSlope;//车站限坡
	int m_ENDGeneration;//终止代数
	double m_ENDMONEY;//终止时费用的差值
	int m_EndType;//所选择的条件
	double m_MaxBriTwg;//最大桥梁高度
	double m_MaxBrigTwg;//最大桥梁高度
	int m_JXorRXD;//为0时是基线，为1时是绕行段
	int m_XlStye;//为-1时是基线，其余为对应的绕行段
	double m_MinBiLen;
	double Cost_xs;//工程土石方费用所占得权重值
	double Oprate_xs;//运营费用所占得权重值
	bool m_CrossStyle;//控制交叉进行的次数，如交叉时失败次数太多，则直接利用选出来的较优方案
	BOOL TempStopOpt;//优化在进化时退出
	BOOL TempStopCreat;//优化在生成初始方案时退出，停止生成
	int m_Stop;//停止优化
	BOOL m_Ex;//为true时有二线，为false时无二线
	int m_SearchStartIndex;
	BOOL    m_IsDrawProfile;//是否绘制纵断面图
	int m_LjorGm;//路肩坡或者是轨面坡
	BOOL    m_IsSQST;//是否拾取纵断面

	BOOL  m_IsSetDesH;//是否由程序自动设计起终点设计高程(起终点高程传入!!!)
	BOOL    m_IsProNowPlan;//是否优化现有的纵断面方案(优化现有方案！！！)
	int m_Xlstye;//-1为基线，0，1等为对应的绕行段
	int  m_BpdLC;//里程取10m，50m，100m，的倍数
	int m_PdXSWS;//坡度小数位数
	int m_DesHXsw;//设计标高小数位数

	//*********全局自动变量*********
	int  m_TKNum;  //图框号
	CString	m_PlanMDB; //方案数据库
	CString m_RoadName; //线路名
	tagMSG msg;
	HWND m_hWnd;
	int m_TryNum;
	int Ngckzd;
	CString m_JHDS;
	CString	m_YouhuaMDB;
	int Thread_Num;
	double temp_mileage;
	double temp_desh;
	int NCM;//CM数组总数
	double* CM;//纵断面数据数组，每个变坡点有五个属性。
	//依次为：0里程，1高程，2坡度，3坡长，4竖曲线半径
	BOOL Tempbreak;//因选择出的方案过少而无法继续优化，提前退出
	double MaxX;//信封区域最高点
	double MinX;//信封区域最低点
	double Min_Operate_H;//所有方案中最小的克服高度值
	vector<double>m_BestPlan;//每一代最优方案的费用
	AcDbObjectId  m_ProId, m_TunId, m_BriId;//纵断面坡度，隧道，桥梁图形ID
	AcDbObject *m_pObj;
	//读规范参数进行赋值
	double	m_MaxLongitudinalSlope;//最大纵坡
	double	m_MinLongitudinalSlopeL;//最小纵坡长
	double	m_MaxSlopeDifference;//最大坡度代数差
	double	m_VerticalCurveRadius;//竖曲线半径
	double	m_HbPdc; //坡度合并的最小坡度代数差

	//根据接坡约束确定参数值
	double m_Startml; //优化终点里程
	double m_Endml; //优化起点里程
	double	m_EndDesH;//终点设计高程
	double	m_StartDesH;//起点设计高程

	vector<GCKZPt> m_AutoSetBriGCKZDArray;//自动设置的桥梁高程控制点
	vector<GCKZPt> m_BriGCKZDArray;//程序判断必须设置的桥梁高程控制点

	typedef struct
	{
		double Mileage;
		double Lever;
	}EnvPt;

	vector< EnvPt> MaxEnvPtArray;//信封区域上限点
	vector< EnvPt> MinEnvPtArray;//信封区域下限点

	typedef struct
	{
		double Mileage;//连续里程
		double DesH;//设计高程
		double VerticalCurveRadius;//竖曲线半径
		double Gradient;//坡度
		int Style;//1-平常坡段，0-站坪段
		double Cost;//该变坡点前后坡段范围内工程造价
		double LimitGrade;//该坡段对应的最大限坡值
		double QXZJ_SX;//上行曲线折减
		double QXZJ_XX;//下行曲线折减
	} SlopePt;//変坡点

	struct ProfilePlan
	{
		double S_high;//上行克服高度
		double X_high;//下行克服高度
		double Satisfaction;//满意度
		int ProfileIndex;//方案号索引
		double Cost;//造价
		int Fitness;//适应度值
		double SelProbability;//方案选择概率
		vector<SlopePt> SlopePtArray;//変坡点数组
		vector<BRITAB> BriArray;//桥梁数组
		int BriSum;
		vector<TUNTAB> TunArray;//隧道数组
		int TunSum;
		ProfilePlan()
		{
			S_high = X_high = Satisfaction = ProfileIndex = 0.0;
			Cost = Fitness = SelProbability = BriSum = TunSum = 0.0;

		}
	};//纵断面方案

	typedef struct
	{
		double stratMileage;
		double endMileage;
		double XP;//限坡
	}Station;//分段

	typedef struct
	{
		double minS;
		double maxS;
		int Style;//1-缓和曲线，2-车站,3-桥梁，4-隧道
	}subsection;//分段

	typedef struct
	{
		double cml;
		double EarH;
		double DesH;
	}TempCRS;//计算信封区域所要用的横断面数组

	typedef struct
	{
		double mile;
		double high;
	}gridPoint;

	typedef struct
	{
		double StratMileage;//起点里程
		double StratDesH;//起点高程
		double EndMileage;//终点里程
		double EndDesH;//终点高程
		double Lenth;//分段长度
	} DMX_SubSection;//

	typedef struct
	{
		double Cml;
		double Earh;
		double Style;//性质
	}CRSPT;//分段
	vector<CRSPT> m_CRS_PT;//有效横断面数组

	typedef struct
	{
		double StratMileage;//起点里程
		double StratDesH;//起点高程
		double EndMileage;//终点里程
		double MaxEndDesH;//最大终点高程
		double MinEndDesH;//最小终点高程
		double MaxMileage;//信封区域上限分界里程
		double MinMileage;//信封区域下限分界里程
		int Style;//对应高程控制点的性质（0-必经点，1-上限点，2-下限点，3-上下限点）
	} Envelope;//信封区域

	vector<Envelope>envelope;

	int NGCKZD;
	vector<GCKZPt> GCKZDTAB; //高程控制点表

	vector<DMX_SubSection>m_dmx_SubSection;
	vector<gridPoint> ZMdmxPt;                  //纵断面地面线上的点；
	vector<gridPoint> PSdmxPt;                  //平顺地面线上的点；
	vector<gridPoint> ThreeDiffQuotientPt;      //三阶差商得到的点；
	vector<gridPoint> OneDiffQuotientPt;      //一阶差商得到的点；
	vector<gridPoint> ChangePt;      //斜率有突变的点；
	vector<gridPoint> JDdwxPt;                  //节点定位线上的点；
	vector<gridPoint> FwdPt;                    //纵断面地面线上的反弯点；

	vector<Station> m_Station;//车站分段数组 (不考虑)
	vector<subsection> m_subsection;//缓和曲线分段数组 (不考虑) 

	vector<TempCRS>tempCRS;//计算信封区域所要用的横断面数组
	vector<subsection> m_section_Tun_Bri;//桥梁和隧道分段数组

	ProfilePlan LastBestPlan;
	vector<ProfilePlan> m_ProfilePlanArray;//纵断面方案数组
	vector<ProfilePlan>SelPlanArray;//进行了选择算子得到的纵断面方案数组，交叉利用的数据
	vector<ProfilePlan>CrossPlanArray;//交叉算子得到的纵断面方案数组
	vector<ProfilePlan>ModifyPlanArray;//变异算子得到的纵断面方案数组
	vector<ProfilePlan>SelPlanArray1;//进行了选择算子得到的纵断面方案数组,变异利用的数据

	//********************************纵断面优化相关函数********************************
	//优化主控函数
	void StartOptimization(vector<PDTAB_ROAD>&m_SlopeTAB,vector<BRITAB>&OutBriArray,vector<TUNTAB>&OutTunArray);
	//*********计算函数*********
	// 按地面线，给初始方案赋值
	void ForFirstPlan();
	//计算平顺地面线的三阶差商
	void GetThreeDiffQuotientPt();
	//计算一阶差商
	void GetOneDiffQuotientPt();
	//获得高程控制点数组
	void GetGCKZD(double StartM, double StartH, double EndM, double EndH);
	//获得已有方案
	void GetPlan();
	//给整个种群的里程赋值
	void CreateProfileMileage(bool ContinueOrStrat);
	void DealEartH(int NCRS,vector<DMXYS>&Crs);//处理地面线数据中高程为-100的点
	//读高程控制点表
	void Read_GCKZD();
	//获取优化起终里程
	void GetStAndEdLC(double &StartLC, double &EndLC, double &StLevel, double &EdLevel);
	//检测高程控制点是否在原始信封区域内
	bool CheckKZDIsInEnvelop(double Mileage, double& Lever, double StartMileage, double StartLever, double EndMileage, double EndLever);
	//清除不合理的高程控制点
	void RemoveGCKZD(vector<GCKZPt>& GCKZDArry, bool IsSXKZD);
	//搜索高程控制点中违规最严重的高程控制点，当都不违规时，返回-1
	int FindMaxSlopeGCKZDIndex(vector<GCKZPt>& GCKZDArry, bool IsSXKZD);
	//得到信封区域
	void GetEnvelop();
	//获取交点
	void GetJD(double Scml, double SLever, double Ecml, double ELever,
		double& MAX_cml, double& MAX_H, double& MIN_cml, double& MIN_H);
	void GetJD(double Scml, double SLever);
	//计算两控制点之间的信封区域点
	void CalEnvPtArray(GCKZPt SPt, GCKZPt Ept, vector<EnvPt>& EnvPtArray);
	//计算考虑折减之后的上下行坡度
	void CalLimitGrade(ProfilePlan& Profile, double Mileage, double Mileage1, double& MaxSlope_SX, double& MaxSlope_XX);
	//从线规中取隧道折减系数
	double GetTunZJXS(double tunlen);
	//用信封区域调整地面线数组
	void GetDMXfromEnvelop(double Scml, double SLever);
	//得到信封区域所确定的里程范围
	bool GetEnvelopRange(double Fronthigh, double FrontMileage, double NowMileage, double &TempMinH, double &TempMaxH);
	void JustEnvelopRangeByKZD(double Fronthigh, double FrontMileage, double NowMileage, double& MinH, double& MaxH);
	//自动设置隧道
	void AutoSetTUNforEnvelop(JD_CENTER *pz, double Th, double MinLen, double STH, int& NTun, vector<TUNTAB> &Tun_Array);
	//自动设置隧道
	void AutoSetTUN(JD_CENTER *pz, double Th, double MinLen, double STH, int& NTun, ProfilePlan &TempPlan);
	//自动设计桥梁
	void AutoSetBRIforEnvelop(JD_CENTER *pz, double Th, double MinLen, double STH, int& NBri, vector<BRITAB> &Bri_Array);
	//自动设计桥梁
	void AutoSetBRI(JD_CENTER* pz, double Th, double MinLen, double STH, int& NBri, ProfilePlan &TempPlan);
	//找有效地面线范围
	void GetDMXRange();
	//平顺地面线
	void PsDmx(double ps_R, int ps_N);
	//求得某点的平顺后高程
	double GetPsHigh(int i, double R);
	//求得某点的平顺后高程
	double GetPsHigh(int i, double R, int tp);
	//得到缓和曲线和车站的分段
	void GetTransitionAndStation();
	//线路分段，赋限坡值 
	void SectionLine();
	//初始化方案
	void GetFirstPlan(int PopulationSize);
	//利用地面线生成第索引为VerticalIndex的変坡点里程
	void GetPlanPtMileage(int VerticalIndex);
	// 调整初始方案的分段
	void JudgeSection();
	//调整方案里程取整
	void AdjustSlopeMileage(ProfilePlan& Plan);
	//坡度折减后，计算整个线路的最大限坡（折减后整条线路的最大限坡一定会小于规定的最大限坡值） (不考虑坡度折减)
	void GetMaxGrade(ProfilePlan& TempPlan);
	//用两种方法生成初始方案
	void CreatPlanByTwoMethod(bool ContinueOrStrat);
	//按期望生成方案
	void CreatPlanMiu(int &VerticalIndex);
	//初始化一个纵断面方案
	bool IniProfile(int VerticalIndex);
	//生成第索引为SlopePtIndex的変坡点设计高程
	bool CreateSloptPtDesH(int VerticalIndex, int SlopePtIndex);
	void Env_GetMiu(ProfilePlan &TempPlan, int PDNum, double &Miu);
	//根据地面线点数拟合坡度的期望值
	void GetDMX1(double StratM, int Num);
	//计算索引为SlopePtIndex的変坡点设计高程范围
	bool CalSloptPtDesHRange(int VerticalIndex, int SlopePtIndex, double& minH, double& maxH);
	//传入期望、方差、取值范围，得到按正态分布的随机数
	double NormalRandom(double miu, double sigma, double min, double max);
	//传入范围，得到均匀分布的随机数
	double AverageRandom(double min, double max);
	//计算x的概率密度函数
	double Normal(double x, double miu, double sigma);
	//计算一个方案的土石方总量
	bool CalEarthWork(ProfilePlan &TempPlan, double& FillArea, double& CutArea);
	//将変坡点数组转换为CM数组
	void TrnBpdToCM(int NBPD, double BPD_array[][4]);
	//释放内存空间
	void ReleaseRoom(int Stage);
	//排序
	void Taxis(vector<ProfilePlan>&Array);
	//计算Sw，Sw为考虑运营和工程结合后的满意度（运营计算）
	void Cal_Satisfaction(vector<ProfilePlan>&Array);
	//获得克服高度（运营计算）
	void Get_Operate_H(vector<ProfilePlan>&Array);
	//基于满意度的排序
	void Taxis_Satisfaction(vector<ProfilePlan>&Array);
	//计算坡度
	void CalAllPD(ProfilePlan &tempPlan);
	//得到适应度值
	void CalFitness();
	//计算基于满意度的多样性
	void CalDiff_Satisfaction(double& Diff);
	//计算选择概率
	void CalChooseProbability(double diff);
	//生成下一代
	bool CreateNextPopulation_Satisfaction(double diff);
	//选择算子
	void SelOperator();
	//变异算子
	void ModifyOperator();
	//交叉算子
	void CrossOperator();
	void CrossOperator_Satisfaction();
	//轮盘赌操作
	int Roulette_Wheel_Selection();
	//一次均匀变异得到的高程
	bool OneModifyH(ProfilePlan& TempPlan);
	//由前一个变坡点得到高程范围
	bool FrontGetRange(ProfilePlan& TempP, int NowPdNum, double& minH, double& maxH);
	//由后一个变坡点得到高程范围
	bool BackGetRange(ProfilePlan& TempP, int NowPdNum, double& minH, double& maxH);
	//算术交叉
	bool ArithmeticCross(ProfilePlan& tempPlan1, ProfilePlan& tempPlan2, bool GetBest);
	//启发式交叉
	bool HeuristicCross(ProfilePlan& tempPlan, bool GetBest);
	//启发式交叉
	bool HeuristicCross_Satisfaction(ProfilePlan& tempPlan, bool GetBest);
	//判断算法是否结束
	bool EndAlgorithm();
	//考虑限制坡度调整方案
	void AdjustPlanByLimitGrad(ProfilePlan& Profile);
	//删除隧道内的变坡点
	void DeleteSlopePtInTun(ProfilePlan& TempPlan);
	//判断Profile方案的StartIndex到EndIndex的变坡点能否被删除
	bool SlopePtCanDel(ProfilePlan& Profile, int StartIndex, int EndIndex);
	//计算最终方案的上下限制坡度
	void CalLastPlanLimitGrade(double StartMileage, double EndMileage, double& MaxSlope_SX, double& MaxSlope_XX);
	//计算曲线折减
	void CalQXZJ(double D1, double D2, double *U1, double *U2);
	//考虑坡度代数差调整坡度
	void AdjustPlanByDsc(ProfilePlan& Profile);
	//计算Profile方案的Index变坡点的可动范围，不可动返回false
	bool SlopePtLeverRange(ProfilePlan& Profile, int Index, double& MaxLever, double& MinLever);
	//合并坡段
	void MergeSlope(ProfilePlan& Profile);
	//计算最终方案的费用
	void CalLastPlanCost(ProfilePlan &TempPlan);
};

