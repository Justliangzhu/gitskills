// ZDMSJ.h: interface for the ZDMSJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZDMSJ_H__FD33ECE4_8D43_4325_824F_D78B76B1E5AB__INCLUDED_)
#define AFX_ZDMSJ_H__FD33ECE4_8D43_4325_824F_D78B76B1E5AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.H"
#include "GTZDM.H"
class ZDMSJ :public BAS_DRAW_FUN 
{
public:
	ZDMSJ();
	virtual ~ZDMSJ();
	void LINK_PDLINE();
    void BZBPD1(double  BPD_array[MAXBPDNUM][4],int NBPD,double CSB[6], int NLC);
    void createGTZDM_JDDATAFILE();
    void createGTZDM_INPUT();
    void changeDL();
    void addDL();
    void delDL();
    void add_BPD();
    void del_BPD();
    void changeBPD();
    void SAVEPD(void);
	void ZmLPSJ();
	void CZmLPSJ();
	void  queue(double a[][4] , int nrow, int inf );
	void AutoSlopeData(); //一键纵坡

private:

	//一键纵坡相关
	int izdm; //纵断面序号
	ProfileDesignStd m_PrfDesData; 	//纵断面设计规范参数
	std::vector<DMXYS> m_EarthLine; //地面线表
	std::vector<GCKZPt> m_LevelPtArray;	//控制标高数组
	std::vector<JPInfor>m_JPInforArray; 	//接坡信息数组
	std::vector<PDTAB_ROAD>m_SlopeArray,m_BPDArray,m_LastPDArray; //坡度数组 m_SlopeArray(初始方案) m_BPDArray(接坡点约束)
	std::vector<EnvPt> MaxEnvPtArray, MinEnvPtArray;	//信封区数组	//信封区域上限点	信封区域下限点
	std::vector<GCKZPt> m_UserSetGCKZDArray_SX, m_UserSetGCKZDArray_XX; 	//用户设置的高程控制点
	std::vector<CRSPT> m_CRS_PT; //临时地面线分段数组
	std::vector<BRITAB>m_BriArray;//桥梁数组
	std::vector<TUNTAB>m_TunArray;//隧道数组
	double m_StartH, m_EndH; //设定的起终高程
	double m_StartLC, m_EndLC; //设定的起终里程
	bool Is_NeedInitPro;//是否需要根据地面线形成初始方案
	//*********主要流程函数*********
	//变坡点优化
	void GetInitSlope(JD_CENTER* pRoad, std::vector<DMXYS>& EarthLine);
	//根据是否有接坡信息获取起终变坡点信息
	void GetBPDStAndEdfromJP(int &BPDnum, DMX_ROAD *pdmx);
	//获取优化起终里程
	void GetStAndEdLC(double &StartLC, double &EndLC, double &StLevel, double &EdLevel);
	//变坡点与交叉点数据排序
	template <typename compara> void SortData(vector<compara> &dataAry);
	//调整坡度分段
	void JudgeSection_ZMSJ();
	void JudgeSection_ZMSJ(std::vector<PDTAB_ROAD>BPDarray);
	//计算平顺地面线的三阶差商
	void GetThreeDiffQuotientPt_ZMSJ(std::vector<DMXYS>& EarthLine, std::vector<DMXYS>& SmoothedEarthLine, std::vector<DMXYS>& ThreeDiffQuotientPt);
	//计算一阶差商
	void GetOneDiffQuotientPt_ZMSJ(std::vector<DMXYS>& ThreeDiffQuotientPt, std::vector<DMXYS>& OneDiffQuotientPt);
	//获取变坡点
	void GetChangePt_ZMSJ(JD_CENTER* pRoad, std::vector<DMXYS>& OneDiffQuotientPt, std::vector<DMXYS>& ChangePt);
	//初始化坡度数组
	void IniSlopeArrray_ZMSJ(std::vector<DMXYS>& ChangePt);
	//生成各变坡点标高
	bool CreatPlanMiu_ZMSJ();
	//形成初始方案
	void FormInitSlope_ZMSJ();
	//形成最终纵断面方案
	void FormFinalSlope_ZMSJ();

	//*********计算函数*********
	//求交自动上变坡点
	bool AutoCalPVI(double &ml, double &H);
	//平顺地面线
	void SmoothEarthline_ZMSJ(double MinSlopeLen, int SmoothNum, std::vector<DMXYS>& EarthLine, std::vector<DMXYS>& SmoothedEarthLine);
	//获取平顺后的高程
	double GetSmoothedHigh_ZMSJ(int i, double MinSlopeLen, std::vector<DMXYS>& EarthLine);
	double GetSmoothedHigh_ZMSJ(int i, double MinSlopeLen, int tp, std::vector<DMXYS>& EarthLine);
	//生成第索引为SlopePtIndex的变坡点设计高程
	bool CreateSloptPtDesH_ZMSJ(int SlopePtIndex);
	//调整高程控制点
	void AdjustGCKZD_ZMSJ(double StartCml, double StartH, double EndCml, double EndH);
	//由高程控制点形成信封区域
	void GetEnvelop_ZMSJ(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry);
	//获取交点
	int GetJD_ZMSJ(double Scml, double SLever, double Ecml, double ELever,
		double& MAX_cml, double& MAX_H, double& MIN_cml, double& MIN_H);
	//插入一组高程控制点
	void InsertGCKZDArray_ZMSJ(std::vector<GCKZPt>& SX_GCKZDArry, std::vector<GCKZPt>& XX_GCKZDArry,
		std::vector<GCKZPt>& GCKZDArray, bool IsSX);
	//得到信封区域所确定的里程范围
	bool GetEnvelopRange_ZMSJ(double Mileage, double& MinH, double& MaxH);
	bool GetEnvelopRange_ZMSJ(double PreDesH, double PreMileage, double Mileage, double& MinH, double& MaxH);
	//查找高程控制点在数组中的位置
	void InsertGCKZD_ZMSJ(std::vector<GCKZPt>& GCKZDArry, GCKZPt KZD);
	//计算边坡点高程范围
	bool CalSloptPtDesHRange_ZMSJ(int SlopePtIndex, double& minH, double& maxH);
	//传入期望、方差、取值范围，得到按正态分布的随机数
	double NormalRandom_ZMSJ(double miu, double sigma, double min, double max);
	//得到均匀分布随机数
	double AverageRandom_ZMSJ(double min, double max);
	//计算x的概率密度函数
	double Normal_ZMSJ(double x, double miu, double sigma);
	//根据地面线计算填挖方最小高程
	void Env_GetMiu_ZMSJ(int PDNum, double& Miu);
	//获取地面线
	void GetDMX1_ZMSJ(double StratM, double EndM);
	//坡度取整
	void RoundSlope_ZMSJ(vector<PDTAB_ROAD>&SlopeArray);
	//合并坡度
	void MergeSlope_ZMSJ(vector<PDTAB_ROAD>&SlopeArray);
	//判断Profile方案的SlopetIndex变坡点能否被删除
	bool SlopePtCanDel_ZMSJ(int StartIndex, int EndIndex);
};

#endif // !defined(AFX_ZDMSJ_H__FD33ECE4_8D43_4325_824F_D78B76B1E5AB__INCLUDED_)
